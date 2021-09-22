	lw	0	2	mcand
	lw	0	3	mplier
	lw	0	4	mask
	lw	0	5	one
	lw	0	7	maxl
	noop
check	nor	3	4	6	// Obtain nor value between the mplier and the mask
	beq	0	6	do	// If the nor value is 0, jump to do branch
	beq	0	0	nextd
	noop
do	add	1	2	1	// Add subtotal to the final answer
	noop
nextd	add	2	2	2	// Left shift the mplier
	noop
cmask	add	4	4	4	// Left shift the mask
	add	4	5	4	// Increment mask
	lw	0	6	neg1
	add	7	6	7	// Update the number of iterations remaining
	beq	7	0	done
	beq	0	0	check
done	halt
mcand	.fill	1103
mplier	.fill	7043
mask	.fill	-2
one	.fill	1
neg1	.fill	-1
maxl	.fill	15
