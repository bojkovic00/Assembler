.extern c

.section text
ldr r1,$7
ldr r2,$2
cmp r1,r2
jgt %a
ldr r3,$1
a:
halt


.section text2
jmp %a
jmp %b
jmp %c
ldr r1,50
ldr r2,$6
ldr r4,$b
ldr r4,$a
ldr r4,$c
b:
add r1,r2
mul r1,r2
ldr r3,[r3+5]
ldr r3,[r3+c]
ldr r3,[r3]
jgt c



.end