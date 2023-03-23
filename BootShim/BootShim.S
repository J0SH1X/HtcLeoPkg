.code 32
.text
.globl _start
_start:
	/* see if we need to relocate */
	adr			r0, _Payload //load the payload adress into r0

	ldr			r1, =0x2C000000 //Load the expected payload base into r1
	cmp			r0, r1			  //Compare the adresses
	beq			_Entry			//Jump to entry if the adress is the same
	ldr 		r2, =2097152 //=0x00200000

_CopyLoop:
	/* memcpy */
	ldr		r3, [r0], #4
	str		r3, [r1], #4
	subs	r2, r2, #4		//substract the size that got copied from fd size
	bgt		_CopyLoop		//branch if greater than zero

	/* we're relocated, jump to the right address */

_Entry:
	ldr		r5, =0x2C000000
	bx		r5

.text
.align 4
_Payload: