*----------------------------------------------------------------------
* Assignment or Title: Programming Assignment #2
* Filename: prog2.s
* Date completed: 10/31/2012 
*----------------------------------------------------------------------
* Problem statement: Find the day of the week
* Input: Date in the range 01/01/1800 to 12/31/9999
* Output: What day it is: Sunday, Monday, etc. 
* Error conditions tested: None
* Included files: None
* Method and/or pseudocode:
*    Read month, day, year separately from user input
*    Calc:
*        a = (14-month)/12
*        y = year - a
*        m = month + 12a - 2
*        d = (day + y + y/4 - y/100 + y/400 + 31m/12) mod 7

* References: None
*----------------------------------------------------------------------
*
        ORG    $0
        DC.L    $3000          * Stack pointer value after a reset
        DC.L    start          * Program counter value after a reset
        ORG    $3000          * Start at location 3000 Hex
*
*----------------------------------------------------------------------
*
#minclude /home/ma/cs237/bsvc/iomacs.s
#minclude /home/ma/cs237/bsvc/evtmacs.s
*
*----------------------------------------------------------------------
*
* Register use
*
*----------------------------------------------------------------------
*
start:  initIO                  * Initialize (required for I/O)
    setEVT            * Error handling routines
*    initF            * For floating point macros only    

    lineout    title
    lineout    prompt
    linein    buffer
    cvta2    buffer,#2    * cvt month into two's comp
    move.l    D0,D1        * month --> D1
    cvta2    buffer+3,#2    * cvt day into two's comp
    move.l    D0,D2        * day --> D2
    cvta2    buffer+6,#4    * cvt year to two's comp
    move.l    D0,D3        * year --> D3
    
    *** a = (14-month)/12 --> D4
    
    move.l    #14,D4
    sub.l    D1,D4        * 14-month --> D4
    ext.l    D4        * prepare for division
    divs    #12,D4        * a --> D4

    *** y = year-a
    
    move.w    D3,D5        * year --> D5
    sub.w    D4,D5        * y --> D5
    
    *** m = month+12a-2
    
    move.w    D4,D6        * a --> D6
    muls    #12,D6        * 12a --> D6
    add.w    D1,D6        * month + (12a) --> D6
    sub.w    #2,D6        * m --> D6
    
    *** d = (day+y+(y/4)-(y/100)+(y/400)+(31m/12))mod7
    
    move.w    D6,D7        * m --> D7
    muls    #31,D7        * 31m --> D7
    divs    #12,D7        * (31m/12) --> D7
    move.w    D5,D4        * y --> D4
    ext.l    D4        * prepare for division
    divs    #400,D4        * (y/400) --> D4
    add.w    D4,D7        * (y/400)+(31m/12) --> D7
    move.w    D5,D4        * y --> D4
    ext.l    D4        * prepare for division
    divs    #100,D4        * (y/100) --> D4
    sub.w    D4,D7        * (y/400)+(31m/12)-(y/100) --> D7
    move.w    D5,D4        * y --> D4
    ext.l    D4        * prepare for division
    divs    #4,D4        * (y/4) --> D4
    add.w    D4,D7        * (y/4)+(y/400)+(31m/12)-(y/100) --> D7
    add.w    D5,D7        * y+(above D7) --> D7
    add.w    D2,D7        * day+(above D7) --> D7
    ext.l    D7        * prepare for division
    divs    #7,D7        * divide by 7 gives: rem|quo
    swap    D7        * quo|rem
    
    *** final output, what day it is

    muls    #12,D7        * 12d --> D7  (Creating offset)
    
    lea    days,A0        * Days Array (Sunday, Mon...) --> A0
    adda.l    D7,A0        * Add offset to array to get correct day
    move.l    (A0)+,day    * Moving the day into the storage space allocated
    move.l    (A0)+,day+4    * continuing to move the day
    move.l    (A0),day+8    * finishing moving the day to be printed
    
    lineout    answer        * print final product: The day of the week
    
        break                  * Terminate execution
*
*----------------------------------------------------------------------
*      Storage declarations

skipln:    ds.b    1
title:    dc.b    'CS 237 Program #2, Jacqueline Behr, masc1368',0
prompt:    dc.b    'Please enter a date (mm/dd/yyyy):',0
buffer:    ds.b    80
skip:    ds.b    1
answer:    dc.b    'The day of the week is '
day:    ds.b    12
days:    dc.b    'Sunday.    ',0
    dc.b    'Monday.    ',0
    dc.b    'Tuesday.  ',0
    dc.b    'Wednesday. ',0
    dc.b    'Thursday.  ',0
    dc.b    'Friday.    ',0
    dc.b    'Saturday.  ',0

        end