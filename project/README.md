#CSCI 340 Final Project
##matt piazza + john anderson

basically, we have to read some MS-DOS, FAT16 [file](floppy.img) and three basic functions: 
+ initialize
+ read
+ dump

you can also turn on some fun debugging by changing line 8 of [disk.c](disk.c#L8) to `#define DEBUG 1`

[more info](http://www.win.tue.nl/~aeb/linux/fs/fat/fat-1.html) on the fat16 metadata used int physical\_disk

###example usage and output:
    ./dump floppy.img 35 x
    0x4600	54	68	69	73	20	69	73	20	61	20	73	69	6d	70	6c	65	
    0x4610	20	52	45	41	44	4d	45	20	66	69	6c	65	20	66	6f	72	
    0x4620	20	74	68	65	20	70	75	72	70	6f	73	65	20	6f	66	20	
    0x4630	68	61	76	69	6e	67	20	61	74	20	6c	65	61	73	74	20
    ...
    0x47f0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0
