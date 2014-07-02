File-Editor
===========

A unix based interactive binary file editor inspired by pctools in the 80s. Each page is shown with binary and its ASCII side-by-side. Optimized for 80-col terminal. 80x24 or larger terminal space is required.


```
Filename : "../Downloads/Antelop.jpg", 3869024 bytes, READ/WRITE       256
256-bytes block #:19
Offset    Data
   1300 - D2 D3 D4 D5 D6 D7 D8 D9 DA E1 E2 E3 E4 E5 E6 E7 : STUVWXYZ[bcdefgh
   1310 - E8 E9 EA F1 F2 F3 F4 F5 F6 F7 F8 F9 FA FF C4 00 : ijkrstuvwxyz{#E
   1320 - 1F 01 00 03 01 01 01 01 01 01 01 01 01 00 00 00 : ?! #!!!!!!!!!
   1330 - 00 00 00 01 02 03 04 05 06 07 08 09 0A 0B FF C4 :    !"#$%&'()*+#E
   1340 - 00 B5 11 00 02 01 02 04 04 03 04 07 05 04 04 00 :  61 "!"$$#$'%$$
   1350 - 01 02 77 00 01 02 03 11 04 05 21 31 06 12 41 51 : !"w !"#1$%!1&2AQ
   1360 - 07 61 71 13 22 32 81 08 14 42 91 A1 B1 C1 09 23 : 'aq3"2?(4B?"2B)#
   1370 - 33 52 F0 15 62 72 D1 0A 16 24 34 E1 25 F1 17 18 : 3Rq5brR*6$4b%r78
   1380 - 19 1A 26 27 28 29 2A 35 36 37 38 39 3A 43 44 45 : 9:&'()*56789:CDE
   1390 - 46 47 48 49 4A 53 54 55 56 57 58 59 5A 63 64 65 : FGHIJSTUVWXYZcde
   13A0 - 66 67 68 69 6A 73 74 75 76 77 78 79 7A 82 83 84 : fghijstuvwxyz???
   13B0 - 85 86 87 88 89 8A 92 93 94 95 96 97 98 99 9A A2 : ???????????????#
   13C0 - A3 A4 A5 A6 A7 A8 A9 AA B2 B3 B4 B5 B6 B7 B8 B9 : $%&'()*+3456789:
   13D0 - BA C2 C3 C4 C5 C6 C7 C8 C9 CA D2 D3 D4 D5 D6 D7 : ;CDEFGHIJKSTUVWX
   13E0 - D8 D9 DA E2 E3 E4 E5 E6 E7 E8 E9 EA F2 F3 F4 F5 : YZ[cdefghijkstuv
   13F0 - F6 F7 F8 F9 FA FF DA 00 0C 03 01 00 02 11 03 11 : wxyz{#[ ,#! "1#1

h) Edit Hex, t) Edit Text, /) Search, q) Quit
</> : next/previous block, l) Load Block, w) Write Block
```

You can flip through the file block-by-block or load selective block. 

You can search from current block for string or hex (up to 40 bytes).

You can edit the file using hex mode or text mode. Changes will be highlighted on screen. 

And after changes, you can write the block back to the file. 

Building File-Editor is dead easy. Just run make or refer to the two lines Makefile. Only curses library is used.
