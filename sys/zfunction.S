/* zfunction.S - zfunction */

		.text
		.globl	zfunction

/*------------------------------------------------------------------------
 * zfunction - clear the 20th through 27th bits of param (counting from
 *	       the left, starting at 0), shift the result 8 bits to the
 *	       left and fill the right-most 8 bits with 1s
 *
 *	       long zfunction(long param);
 *
 *	       Bits 20..27 counting from the left of a 32 bit value are
 *	       bits 11..4 counting from the right, so they are cleared
 *	       with the mask 0xfffff00f.
 *------------------------------------------------------------------------
 */
zfunction:
		pushl	%ebp
		movl	%esp,%ebp

		movl	8(%ebp),%eax		/* eax = param		*/
		andl	$0xfffff00f,%eax	/* clear bits 20..27	*/
		shll	$8,%eax			/* shift left 8 bits	*/
		orl	$0x000000ff,%eax	/* set low 8 bits	*/

		popl	%ebp
		ret
