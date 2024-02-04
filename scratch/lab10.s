.text 
.global _start
.extern printf

_start:
        adr x0, a
        ldr d19, [x0] // Load A value (-1)

        adr x1, bnum 
        ldr d20, [x1] // Load B value (1)

        adr x2, n
        ldr d21, [x2] // Load N value (2)

        adr x0, lower
        fmov d0,d19
        bl printf 
       
        adr x0, upper // b is upper 
        fmov d0,d20 
        bl printf
       
        adr x0, rectangle
        fmov d0,d21
        bl printf 
       
        bl midpoint

        midpoint: 
                fsub d15, d2, d1 // b - a 
                fdiv d16, d15, d3 // w = (b - a) / n

                adr x6, a
                ldr d29, [x6, #0]

                adr x5, n100
                ldr x4, [x5, #0]  // n as int 

                mov x26, #0 

                //adr x1, totalsum // NOT BEING USED 
                //ldr d14, [x1, #0] // result = 0.0  

                adr x2, two1
                ldr d31, [x2, #0] // load the value of 2.0 

                adr x3, n0
                ldr d30, [x3, #0] // load the value of 0.0 

                fdiv d9, d15, d31 // x = w / 2 
                fadd d23, d29, d9 // x = a+w/2 
                fmov d17, d30
        loop1: 
                cmp x26, x4 // i < n 
                b.ge EXIT
                bl function1
                fadd d17, d17, d8
                fadd d16, d16, d15 
                add x26, x26, #1
                b loop1

        function1:
                adr x0, totalzero 
                ldr d4, [x0, #0]

                 // 8.32^4 
                 adr x0, cof1
                 ldr d16, [x0, #0] 

                fmov d13, d9 // Move x into 


                fmul d13, d13, d13 // x * x = x^2
                fmul d13, d13, d13  // x^2 * x^2 = x^4
                fmul d20, d16, d13 // 8.32x^4 
                fadd d4, d20, d4 // 8.32x^4 + 0 

                // 6.53x^2 
                adr x0, cof2 
                ldr d17, [x0, #0]

                fmov d12, d9 // Move x into

                fmul d12, d12, d12 // x * x = x^2 
                fmul d21, d17, d12 // 6.53x^2 
                fsub d4, d4, d21  // 0 - 6.53x^2 

                // 9.34x
                adr x0, cof3
                ldr d18, [x0, #0]
                
                fmov d11, d9 
        
                fmul d22, d18, d11 // 9.34x 
                fadd d4, d22, d4 // 9.34x + 0 

                // 12.32 
                adr x0, cof4
                ldr d19, [x0, #0]
                
                // NO move is done because of not x is needed 

                fadd d4, d19, d4  

                // 8.32x^4 - 6.53x^2 + 9.34x + 12.32 

                fmov d8, d4 // y is now in d8
                BR X30
        function1_end: 
EXIT:
    fmov x0, d17 
    adr x0, final 
    bl printf
    mov x0, #0
    mov w8, #93
    SVC #0
    

.data
a: .double -1.0 // left limit
bnum: .double 1.0 // right limit
n: .double 2.0 // number of rectangles under the curve

final: .asciz "Sum = %lf\n"
lower: .asciz "Lower Bound = %lf\n"
upper: .asciz "Upper Bound = %lf\n"
rectangle: .asciz "Rectangle = %lf\n"

cof1: .double 8.32  
cof2: .double 6.53
cof3: .double 9.34
cof4: .double 12.32 
totalsum: .double 0.0 
two1: .double 2.0 
totalzero: .double 0.0
n100: .dword 100
n0: .double 0.0 

.bss
result: .skip 8 // the result of integral

.end 
