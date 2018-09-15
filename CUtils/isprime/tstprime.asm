# Tests a single number for primality.
# Argument: Integer to test
# Returns: 1 if prime, 0 if not.

testPrime:
_testPrime:
__testPrime:

___testPrime:
	push	ebp		# Create stack frame.
	mov	ebp, esp
	push	ecx		# Save registers that we clobber
	push	edx
	xor	edx, edx	# Clear EDX.
	pushl	# Set up a temp-space on the stack for later.

	mov	eax, [ebp+20]	# Grab the argument.
				# We do these first so we don't try to square-root anything less than 3.
	cmp	eax, 2		# Set up flags.
	je	.isprime	# 2 is prime.
	jpe	.notprime	# If even (and not 2), it's not prime.
	jl	.notprime	# If less than 2, it's not prime.

	fildl	[ebp+20]	# Put the argument into the FPU.
	fsqrt			# Square-root it.
	fistpl	[ebp+16]	# Save the result in our temp-space.

	mov 	ecx, [ebp+16]	# Set up our testing loop by putting square-root in ECX.
	cmp 	ecx, 0		# Make sure it's odd.
	jpo	.divloop
	inc 	ecx
	.divloop:
		#mov 	edx, word [ebp+20]	# Put hiword of arg into EDX.
		mov	eax, dword [ebp+20]	# Put loword of arg into EAX.
		div	ecx	#Divide!
		cmp	edx, 0	# See if it divided evenly by testing the remainder.
		je	.notprime	# If it did, it's not prime.

		sub	ecx, 2	# Get next divisor.
		cmp	ecx, 1	# Check if ECX is 1 (3 when we divided).
	jne	.divloop	# If not, then we haven't tried all the divisors, so we jump back. Otherwise it's prime, so fall through.
		
	

	.isprime:
		mov 	eax, 1
		jmp 	.end

	.notprime:
		mov 	eax, 0

	.end:
		popl 	# Free temp space, restore clobbered registers, and return.
		pop 	edx
		pop 	ecx
		pop 	ebp
		ret
