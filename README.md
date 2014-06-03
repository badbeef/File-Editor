File-Editor
===========

A unix based interactive binary file editor inspired by pctools in the 80s. Each page is shown with binary and its ASCII side-by-side. Optimized for 80-col terminal. 80x24 or larger terminal space is required.



    100 - 5F 5F 73 74 75 62 73 00 00 00 00 00 00 00 00 00 : __stubs
    110 - 5F 5F 54 45 58 54 00 00 00 00 00 00 00 00 00 00 : __TEXT
    120 - 5A 29 00 00 01 00 00 00 A8 00 00 00 00 00 00 00 : Z)  !   )
    130 - 5A 29 00 00 01 00 00 00 00 00 00 00 00 00 00 00 : Z)  !
    140 - 08 04 00 80 00 00 00 00 06 00 00 00 00 00 00 00 : ($ ?    &
    150 - 5F 5F 73 74 75 62 5F 68 65 6C 70 65 72 00 00 00 : __stub_helper
    160 - 5F 5F 54 45 58 54 00 00 00 00 00 00 00 00 00 00 : __TEXT
    170 - 04 2A 00 00 01 00 00 00 28 01 00 00 00 00 00 00 : $*  !   (!
    180 - 04 2A 00 00 02 00 00 00 00 00 00 00 00 00 00 00 : $*  "
    190 - 00 04 00 80 00 00 00 00 00 00 00 00 00 00 00 00 :  $ ?
    1A0 - 5F 5F 63 73 74 72 69 6E 67 00 00 00 00 00 00 00 : __cstring
    1B0 - 5F 5F 54 45 58 54 00 00 00 00 00 00 00 00 00 00 : __TEXT
    1C0 - 30 2B 00 00 01 00 00 00 1B 02 00 00 00 00 00 00 : 0+  !   ;"
    1D0 - 30 2B 00 00 03 00 00 00 00 00 00 00 00 00 00 00 : 0+  #
    1E0 - 02 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 : "
    1F0 - 5F 5F 75 6E 77 69 6E 64 5F 69 6E 66 6F 00 00 00 : __unwind_info


You can flip through the file block-by-block or load selective block. You can edit the file using hex mode or text mode. Changes will be highlighted on screen. And after changes, you can write the block back to the file. 

Building it is dead easy. Just run make or refer to the two lines Makefile. Only curses library is used.
