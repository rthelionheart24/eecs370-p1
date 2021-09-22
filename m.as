	lw	0	2	mcand
	lw	0	3	mplier
	lw	0	4	mask
	lw	0	5	one
	noop
check	nor	3	4	6	// Obtain nor value between the mplier and the mask
	beq	5	6	do	// If the nor value is 1, junp to do branch
	beq	0	6	dont	// If the nor value is 0, jump to dont branch
do	add	2	2	7	// Left shift the mcand
	add	1	7	1	// Add subtotal to the final answer
dont	noop
cmask	add	4	4	4	// Left shift the mask
	add	4	5	4	// Increment mask
	beq	0	0	check
done	halt
mcand	.fill	1103
mplier	.fill	7043
mask	.fill	-2
one	.fill	1
