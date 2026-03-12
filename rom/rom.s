;		.org	0x0008000

start:
        ldr     R2,=0xdeadbeef
        mov     R1,R2
        b       start

		.ltorg
