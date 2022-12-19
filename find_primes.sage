def find_prime(bit_len, a=None, b=None, sign=1, optimize_a_b=None):
	#find a prime of the form p = 2^a*3^b*f + 1 for some small cofactor f of bitlength bit_len
	assert (a is not None and b is not None) or (a is None and b is None), "a,b must be either both not none, or both none"
	if a is None and b is None:
		#choose a and b to be balanced
		a = ceil(bit_len/2)
		b = ceil(bit_len/(2*log(3,2)))
	else: # recursion check
		if a < 0:
			a = 0
		if b < 0:
			b = 0

	f = 1
	p = 2^a * 3^b + (sign*1)
	while (not p.is_prime()) or log(p,2) < bit_len-1:
		f += 1
		p = (2^a * 3^b * f) + (sign*1)
	if optimize_a_b is not None:
		# This is a recursive option, which allows you to recursively reduce the size of a and b in exchange for smaller primes. optimize_a_b is the acceptable upper limit of bit length exceeding bit_len. not recommended but good to play around with.
		if log(p,2) > bit_len + optimize_a_b -1:
			p,a,b,f = find_prime(bit_len, a-1,b-1,sign, optimize_a_b)
	return p,a,b,f



print(find_prime(256, 256, 0,1))
print(find_prime(256,256, 0,-1))