def find_prime(bit_len, a=None, b=None, sign=1):
	#find a prime of the form p = 2^a*3^b*f + 1 for some small cofactor f of bitlength bit_len
	assert (a is not None and b is not None) or (a is None and b is None), "a,b must be either both not none, or both none"
	if a is None and b is None:
		#choose a and b to be balanced
		a = ceil(bit_len/2)
		b = ceil(bit_len/(2*log(3,2)))

	f = 1
	p = 2^a * 3^b + (sign*1)
	while (not p.is_prime()) or log(p,2) < bit_len:
		f += 1
		p = (2^a * 3^b * f) + (sign*1)
		
	return p,a,b,f



print(find_prime(256, 256, 0,1))
print(find_prime(256,256, 0,-1))