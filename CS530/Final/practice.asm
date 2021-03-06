prog    start   0
bsrch   equ     6000
offb    equ     8
        base    delta
        ldb     #delta
        lds     #3
        +ldx    @foo
        +lda    beta,x
        add     gamma
        ldx     @zeta
first   norm
        svc     13
        hio
        ldt     #alpha
        +ldt    #foo
        lda     @alpha
        lda     @foo
        addr    s,t
        shiftr  t,2
        shiftl  s,1
        tixr    t
        j       first
        +j      first
        clear   t
        +ldt    #50000
        ldx     #0
        +jsub   bsrch
addl    lda     alpha,x
        add     beta,x
        sta     gamma,x
        addr    s,t
        compr   x,t
        +jlt    addl
        jlt     addl
        rsub

alpha   resw    100
beta    resw    100
gamma   resw    100
delta   word    20
zeta    byte    c'EOF'
eta     byte    x'fffffe'
theta   byte    c'eof'
buffer  resb    3000
foo     word    1
        end     prog