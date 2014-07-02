/*

The MIT License (MIT)

Copyright (c) 1993, 2014 Chui-Tin Yen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include <stdlib.h>
#include <curses.h>
#include <fcntl.h>
#include <string.h>

#define BLOCK_SIZE 256
#define KEY_SIZE 40
#define NEXT 1
#define PREV 2
#define RAND 3
#define SILENT_NEXT 4
#define TOPOFFILE -1
#define BOTOFFILE -2
#define INVALIDPOS -3
#define ABORTED -4
#define SX 10
#define SY 3
#define TX 60
#define TY 3
#define ENTER 10

/*
 * This is a File-Editor file that I wrote to have better control of what to 
 * change in a (mostly binary) file. It was written long time ago when my C skill
 * isn't very good. so please forgive the abuse of global variables.  :)
 */
unsigned char buf[BLOCK_SIZE + KEY_SIZE];
char hex[16] = "0123456789ABCDEF";
int maxY, maxX, hy, hx, ty, tx, fd, en = 0;
long pos, length;   // displayed position(in bytes) and total length of current file
bool ReadOnly = FALSE, Changed = FALSE;

void error(char *msg)
{
  move(LINES - 1, 0);
  clrtobot();
  mvprintw(LINES - 1, 10, "%s", msg);
  if (msg != "")
    beep();
  refresh();
}

void moveh(int en)
{
  hy = en / 32;
  hx = en - hy * 32;
  hx += SX + hx / 2;
  hy += SY;
  move(hy, hx);
}

void movet(int en)
{
  ty = en / 32;
  tx = TX + (en - ty * 32) / 2;
  ty += TY;
  move(ty, tx);
}

void putt(unsigned char c)
{
  if ((c < 32) || (c >= 128 && c < 160))
    c += 32;
  else if ((c == 127) || (c == 255))
    c = '#';
  else if (c > 127 && c < 255) 
    c -= 127;
  addch(c);
}

bool AbortChanges()
{
  int c;
  if (Changed)
  {
    error("Abort Changes? y/N ");
    c = getch();
    if (c == 'y' || c == 'Y')
      return TRUE;
    else
      return FALSE;
  } 
  else
    return TRUE;
}

void BlockPrint(int bold_pos, int bold_len) 
{
  int i, j, endf;
  int standout = 0;
  endf = length - pos;
  if (endf > BLOCK_SIZE) {
    endf = BLOCK_SIZE;
  }
  mvprintw(0, 70, "%4d", endf);
  error("");
  attrset(A_BOLD);
  move(1, 19);
  clrtoeol();
  mvprintw(1, 18, "%d", pos / 256);
  attrset(A_NORMAL);
  for (i = endf; i < BLOCK_SIZE; i++)
    buf[i] = 0;
  move(SY, 0);
  for (i = 0; i < 16; i++)
  {
    printw("%7X - ", pos+i*16);
    for (j = 0; j < 16; j++) {
      if (i * 16 + j >= bold_pos && i * 16 + j < bold_pos + bold_len) {
	attrset(A_BOLD);
	standout = 1;
      }
      printw("%c%c ", hex[buf[i*16 + j] / 16], hex[buf[i*16 + j] % 16]);
      if (standout) {
	attrset(A_NORMAL);
	standout = 1;
      }
    }
    printw(": ");
    for (j = 0; j < 16; j++)
    {
      char chr;
      chr = buf[i*16 + j];
      if (i * 16 + j >= bold_pos && i * 16 + j < bold_pos + bold_len) {
	attrset(A_BOLD);
	standout = 1;
      }
      putt(chr);
      if (standout) {
	attrset(A_NORMAL);
	standout = 1;
      }
    }
    addch('\n');
  }
  refresh();
  Changed = FALSE;
}

int BlockRead(int MODE)
{
  if (MODE == PREV)
    if(lseek(fd, pos -= BLOCK_SIZE, 0) == -1L)
    {
      error("Begin of file");
      pos += BLOCK_SIZE;
      return TOPOFFILE;
    }
  if (MODE == NEXT || MODE == SILENT_NEXT)
  {
    pos += BLOCK_SIZE;
    if (pos >= length)
    {
      error("End of file");
      pos -= BLOCK_SIZE;
      return BOTOFFILE;
    }
    else 
      lseek(fd, pos, 0);
  }
  if (MODE == RAND)
    if (pos < 0 || pos >= length)
    {
      error("Block not found");
      return INVALIDPOS;
    }
    else
      lseek(fd, pos, 0);
  if (read(fd, buf, BLOCK_SIZE) <= 0) {
    error("Read Error");
    return INVALIDPOS;
  }
  if (MODE != SILENT_NEXT) {
    BlockPrint(0, 0);
  }
  return 0;
}

BlockWrite()
{
  int a, b, len;
  a = lseek(fd, pos, 0);
  len = length - pos;
  if (len > BLOCK_SIZE) {
    len = BLOCK_SIZE;
  }
  b = write(fd, buf, len);
  move(LINES - 1, 0);
  clrtobot();
  mvprintw(LINES - 1, 10, "Block Written");
  refresh();
  Changed = FALSE;
}

void EditHex()
{
  int c, byte;
  unsigned char val;
  move(20, 0);
  clrtobot();
  move(21, 0);
  printw("'0'..'9', 'A'..'F' to edit\n");
  printw("ENTER to finish");
  moveh(en);
  refresh();
  attrset(A_REVERSE);
  while ((c = getch()) != ENTER)
  {
    if (c >= 'a' && c <= 'f')
      c = c - 'a' + 'A';
    if (c == KEY_RIGHT)
      en += 2 - en % 2;
    if (c == KEY_LEFT)
      en -= 2 - en % 2;
    if (c == KEY_UP)
      en -= 32;
    if (c == KEY_DOWN)
      en += 32;
    if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'))
    {
      Changed = TRUE;
      if (c >= '0' && c <= '9')
	val = c - '0';
      else
	val = c - 'A' + 10;
      byte = en / 2;
      if ((en % 2) == 0)
	buf[byte] = val * 16 + buf[byte] - (int)(buf[byte] / 16) * 16;
      else
	buf[byte] = (int)(buf[byte] / 16) * 16 + val;
      addch(c);
      movet(en);
      putt(buf[byte]);
      ++en;
    }
    if (en > 511)
      en -= 512;
    if (en < 0)
      en += 512;
    moveh(en);
    refresh();
  }
  attrset(A_NORMAL);
  move(20, 0);
  clrtobot();
}

EditText()
{
  int c, byte;
  move(20, 0);
  clrtobot();
  move(21, 0);
  printw("arrow keys to move around, ENTER to finish");
  printw("Others to change ");
  attrset(A_REVERSE);
  movet(en); 
  refresh();
  while ((c = getch()) != ENTER)
  {
    if (c == KEY_RIGHT)
      en += 2 - en % 2;
    if (c == KEY_LEFT)
      en -= 2 - en % 2;
    if (c == KEY_UP)
      en -= 32;
    if (c == KEY_DOWN)
      en += 32;
    if (c >= 32 && c <= 128)
    {
      Changed = TRUE;
      byte = en / 2;
      buf[byte] = (int)c;
      moveh(en);
      printw("%c%c", hex[buf[byte] / 16], hex[buf[byte] % 16]);
      movet(en);
      addch(buf[byte]);
      en += 2;
    }
    if (en > 511)
      en -= 512;
    if (en < 0)
      en += 512;
    movet(en);
    refresh();
  }
  attrset(A_NORMAL);
  move(20, 0);
  clrtobot();
}

void findData()
{
  int c, i, len, digit, value;
  unsigned char key[KEY_SIZE + 1];
  int offset;
  int buf_len;
  unsigned char *found;

  move(20, 0);
  clrtobot();
  mvprintw(21, 0, "Find h)ex or t)ext : ");
  refresh();
  c = getch();
  move(22, 0);
  clrtobot();
  if (c != 'h' && c != 't') {
    return;
  }
  move(20, 0);
  mvprintw(22, 0, "Enter %s string : ", c == 'h' ? "hex" : "text");
  echo();
  getnstr(key, KEY_SIZE);
  key[KEY_SIZE] = 0;
  noecho();
  if (c == 'h') {
    /* convert key to hex */
    digit = 0;
    len = 0;
    value = 0;
    for (i = 0; i <= KEY_SIZE; i++) {
      switch(key[i]) {
      case '\0':
	if (digit) {
	  error("incomplete hex");
	  return;
	}
	goto done;
      case ' ' :
	if (digit) {
	  error("incomplete hex");
	  return;
	}
	digit = 0;
	continue;
      default:
	if (key[i] >= '0' && key[i] <= '9') {
	  value = key[i] - '0';
	} else if (key[i] >= 'a' && key[i] <= 'f') {
	  value = key[i] - 'a' + 10;
	} else if (key[i] >= 'A' && key[i] <= 'F') {
	  value = key[i] - 'A' + 10;
	} else {
	  error("invalid hex string");
	  return;
	}
	if (digit == 0) {
	  key[len] = value;
	  digit = 1;
	} else {
	  key[len] = key[len] * 16 + value;
	  digit = 0;
	  len++;
	}
      }
    }
  } else {
    len = strlen(key);
  }
done:
  /* start with the current block */
  do {
    if (length - pos >= BLOCK_SIZE + KEY_SIZE - 1) {
      /* middle of file */
      read(fd, &buf[BLOCK_SIZE], KEY_SIZE - 1);
      buf_len = BLOCK_SIZE + KEY_SIZE - 1;
    } else if (length - pos > BLOCK_SIZE) {
      /* 2nd last block with not enough space for extra full key */
      read(fd, &buf[BLOCK_SIZE], length - pos - BLOCK_SIZE);
      buf_len = length - pos;
    } else {
      /* last block, with 1 block or less data left */
      buf_len = length - pos;
    }
    found = memmem(buf, buf_len, key, len);
    if (found) {
      offset = found - buf;
      BlockPrint(offset, len);
      error("Found at offset");
      printf(" %d", offset);
      return;
    }
    
    /* update the block id on screen */
    if (pos % 256000 == 0) {
      /* refresh too often brings down performance */
      attrset(A_BOLD);
      move(1, 19);
      clrtoeol();
      mvprintw(1, 18, "%d", pos / 256);
      attrset(A_NORMAL);
      refresh();
    }
  } while (BlockRead(SILENT_NEXT) == 0);
  error("Not found");
  BlockPrint(0, 0);
  return;
}

void LoadBlock()
{
  char sector[20];
  long oldpos;
  move(20, 0);
  clrtobot();
  mvprintw(22, 0, "Block (256 bytes) number to read : ");
  refresh();
  echo();
  getnstr(sector, sizeof(sector) - 1);
  noecho();
  oldpos = pos;
  pos = atoi(sector) * BLOCK_SIZE;
  if (BlockRead(RAND) != 0)
    pos = oldpos;
  move(22, 0);
  clrtoeol();
}  

void BlockEdit()
{
  int c;
  bool CONTINUE = TRUE;
  do
  {
    move(20, 0);
    printw("h) Edit Hex, t) Edit Text, /) Search, q) Quit\n");
    printw("</> : next/previous block, l) Load Block, w) Write Block");
    refresh();
    c = getch();
    switch (c)
    {
      case 'h' :
      case 'H' : EditHex(); break;
      case 't' :
      case 'T' : EditText(); break;
      case '<' : 
	if (AbortChanges())
	  BlockRead(PREV);
	break;
      case '>' :
	if (AbortChanges())
	  BlockRead(NEXT); 
	break;
      case 'l' :
      case 'L' : 
	if (AbortChanges())
	  LoadBlock(); 
	break;
      case 'w' :
      case 'W' : 
	BlockWrite(); 
	break;
      case 'q' :
      case 'Q' : 
	if (AbortChanges())
	  CONTINUE = FALSE;
	break;
      case '/' :
	findData();
	break;
      default  : 
      {
	move(LINES - 1, 30);
	printw("Unknown Command");
	beep();
      }
	break;
    }
  } while (CONTINUE);
}


int main(int argc, char ** argv)
{
  char name[256], Status[12];
  int c;

  if (argc == 1)
  {
    printf("file to be edit : ");
    scanf("%255s", name);
  }
  if (argc == 2) {
    strncpy(name, argv[1], sizeof(name));
  }
  name[sizeof(name) - 1] = 0;
  fd = open(name, O_RDWR);
  if (fd == -1)
  {
    fd = open(name, O_RDONLY);
    if (fd == -1)
    {
      printf("Error reading file %s\n", name);
      exit(1);
    }
    else
      ReadOnly = TRUE;
  }
  printf("\n\nReading file %s...\n\n", name);
  length = lseek(fd, 0L, 2);
  initscr();
  keypad(stdscr, TRUE);
  refresh();
  cbreak();
  noecho();
  if (ReadOnly)
    strcpy(Status, "READ ONLY");
  else
    strcpy(Status, "READ/WRITE");
  attrset(A_REVERSE);
  printw("Filename : \"%s\", %d bytes, %s\n", name, length, Status);
  attrset(A_NORMAL);
  printw("256-bytes block #:\n");
  printw("Offset    Data\n");
  pos = 0;
  BlockRead(RAND);
  BlockEdit();
  close(fd);
  move(LINES-1, 0);
  refresh();
  endwin();
  return 0;
}
