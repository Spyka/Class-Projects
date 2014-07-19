*----------------------------------------------------------------------
* Assignment or Title: Programming Assignment #3
* Filename: prog3.s
* Date completed: 11/15/12
*----------------------------------------------------------------------
* Problem statement: Optimize the volume of a can based off its cost
* Input: Cost of side of can, Cost of ends of can, volume
* Output: Dimensions of most cost efficient can (h,d), with cost shown
* Error conditions tested: None
* Included files: None
* Method and/or pseudocode: 
*
* rad = .01
* old_cost = (calc w/ rad = .01)
* do {
*    radius = radius + .01
*    new_cost = (calc w/ new rad)
*    if( new_cost > old_cost)
*        print answer
*    old_cost = new_cost
*    }
*      
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* while:  if( new_cost > old_cost) goto done 
*      old_cost = new_cost
*      radius = radius + .01
*      new_cost = (calc w/ new rad)
*      goto while
*
* done:      cost = old_cost
*
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

********** CONSTANTS **********
TWO:          EQU    $40000000        
PI:            EQU    $40490FDA
ONE_THIRD:    EQU    $3EAAAAAb
INCREMENT:    EQU    $3C23D70A
*******************************

start:  initIO                  * Initialize (required for I/O)
        initF
        setEVT
    lineout  title
        lineout  p1
        floatin  buffer
        cvtaf    buffer,D5  * END cost
        lineout  p2
        floatin  buffer
        cvtaf    buffer,D6  * SIDE cost
        lineout  p3
        floatin  buffer
        cvtaf    buffer,D7  * VOLUME

**********************************************************************
** Calculus Answer
** Formula for the radius of the optimum can:
** radius = (((volume*side_cost)/(2*PI*end_cost))^(1/3)      

** numerator, volume*side_cost:
        move.l      D7,D1      * VOLUME
        fmul        D6,D1      * VOLUME*SIDE_COST 
        
** denominator, 2*PI*end_cost        
        move.l      D5,D2      * END_COST
        fmul        #TWO,D2    * END_COST * 2.0
        fmul        #PI,D2      * END_COST * 2.0 * PI

** now take result to 1/3 power
        fdiv        D2,D1        * numerator/denominator
        move.l      #ONE_THIRD,D0              
        fpow        D1,D0 *(numerator/denominator) ^ (1/3)
        
** Calulus answer done, now calculate diameter, height, cost
        move.l      D0,D1      * D1 has radius
        fmul        #TWO,D0    * D0 has diameter        
        cvtfa      diameter,#2
        
** calculate height = (volume / PI*r^2)
        move.l      D1,D2      * radius
        fmul        D2,D2      * radius^2
        fmul        #PI,D2      * radius^2*PI
        move.l      D7,D3      * copy of volume
        fdiv        D2,D3      * vol / PI*radius^2  HEIGHT --> D3
        move.l      D3,D0      
        cvtfa      height,#2
        
** calculate cost = SIDE_COST*SIDE_SURFACE + 2*END_COST*END_SURFACE
        *** side cost:
        move.l      #PI,D2
        fmul        #TWO,D2    * 2*PI
        fmul        D1,D2      * 2*PI*radius
        fmul        D3,D2      * 2*PI*radius*height  = side surface area
        fmul        D6,D2      * side surface area * SIDE_COST
        
        *** end cost:
        move.l      #PI,D0
        fmul        #TWO,D0    * 2*PI
        fmul        D1,D0      * 2*PI*radius
        fmul        D1,D0      * 2*PI*radius*radius
        fmul        D5,D0      * 2*PI*radius*radius*END_COST
        fadd        D2,D0
        cvtfa      cost,#2
        
** DONE, print the  calculus answer
        
        lineout    ans1
        lineout    ans2
        lineout    ans3
        
**********************************************************************
*** ADD YOUR CODE HERE FOR THE BRUTE FORCE ANSWER
*** Register usage:
*** D5 ->  END_COST
*** D6 ->  SIDE_COST
*** D7 ->  VOLUME

    move.l      #INCREMENT,D1    * initial radius of .01

********************    
*** initial cost ***
********************
            
** calculate height = (volume / PI*r^2)

        move.l      D1,D2      * radius
        fmul        D2,D2      * radius^2
        fmul        #PI,D2      * radius^2*PI
        move.l      D7,D3      * copy of volume
        fdiv        D2,D3      * vol / PI*radius^2  HEIGHT --> D3
        
** calculate cost = SIDE_COST*SIDE_SURFACE + 2*END_COST*END_SURFACE
        *** side cost:
        move.l      #PI,D2
        fmul        #TWO,D2    * 2*PI
        fmul        D1,D2      * 2*PI*radius
        fmul        D3,D2      * 2*PI*radius*height  = side surface area
        fmul        D6,D2      * side surface area * SIDE_COST
        
        *** end cost:
        move.l      #PI,D0
        fmul        #TWO,D0    * 2*PI
        fmul        D1,D0      * 2*PI*radius
        fmul        D1,D0      * 2*PI*radius*radius
        fmul        D5,D0      * 2*PI*radius*radius*END_COST
        fadd        D2,D0
    
    move.l        D0,D4    * initial cost -> D4

*************************    
*** new cost and loop ***
*************************

loop:    fadd        #INCREMENT,D1    *add increment to radius
                    *D1 -> rad + .01
                    
** calculate height = (volume / PI*r^2)

        move.l        D1,D2
    fmul        D2,D2      * radius^2
        fmul        #PI,D2      * radius^2*PI
        move.l      D7,D3      * copy of volume
        fdiv        D2,D3      * vol / PI*radius^2  HEIGHT --> D3
        
** calculate cost = SIDE_COST*SIDE_SURFACE + 2*END_COST*END_SURFACE
        *** side cost:
        move.l      #PI,D2
        fmul        #TWO,D2    * 2*PI
        fmul        D1,D2      * 2*PI*radius
        fmul        D3,D2      * 2*PI*radius*height  = side surface area
        fmul        D6,D2      * side surface area * SIDE_COST
        
        *** end cost:
        move.l      #PI,D0
        fmul        #TWO,D0    * 2*PI
        fmul        D1,D0      * 2*PI*radius
        fmul        D1,D0      * 2*PI*radius*radius
        fmul        D5,D0      * 2*PI*radius*radius*END_COST
        fadd        D2,D0    
    
    move.l        D0,D3    * new cost -> D3
    
********************************    
*** initialize loop or end  ***
********************************    
        
    fcmp        D4,D3    * compare old cost to new cost
    BGT        done    * if greater or equal goto done  (new cost >= old cost)
    move.l        D3,D4    * put "new" cost into old cost before looping
    BRA        loop
    

** DONE, calc and print the brute force answer
      
done:    move.l        D3,D0    * prepare final cost for convert
    cvtfa      cost,#2    
    
    move.l        D1,D4    *move rad into D4
    fmul        #TWO,D4    * D4 has diameter
    move.l        D4,D0        
        cvtfa      diameter,#2    
        
** calculate height = (volume / PI*r^2)
        move.l      D1,D2      * radius
        fmul        D2,D2      * radius^2
        fmul        #PI,D2      * radius^2*PI
        move.l      D7,D3      * copy of volume
        fdiv        D2,D3      * vol / PI*radius^2  HEIGHT --> D3
        move.l      D3,height      
        cvtfa      height,#2  
    
    lineout    ans1
        lineout    ans2
        lineout    ans3        
            
        break
        

title:      dc.b      'Program #3, masc 1368, Jacqueline Behr',0
skip:      dc.b    0
buffer:  ds.b    80      
p1:      dc.b    'Please enter the cost of the end material in dollars/cm^2:',0 
p2:      dc.b    'Please enter the cost of the side material in dollars/cm^2:',0 
p3:      dc.b    'Please enter the volume of the can in milliliters:',0 
ans1:    dc.b    'Can cost: '
cost:    ds.b    40
ans2:    dc.b    'Diameter: '
diameter: ds.b    40
ans3:    dc.b    'Height: '
height:  ds.b    40 
          end