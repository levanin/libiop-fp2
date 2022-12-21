#!/usr/bin/env sage -python

from sage.all import *

class FpParam():
    def __init__(self, p, num_bits, euler, s, t, t_minus_1_over_2, mul_gen, root_of_unity, nqr, nqr_to_t, rsquared, rcubed, inv):
        self.p = p
        self.num_bits = num_bits
        self.euler = euler
        self.s = s
        self.t = t
        self.t_minus_1_over_2 = t_minus_1_over_2
        self.mul_gen = mul_gen
        self.root_of_unity = root_of_unity
        self.nqr = nqr
        self.nqr_to_t = nqr_to_t
        self.rsquared = rsquared
        self.rcubed = rcubed
        self.inv = inv

    def print_libiop(self, name):
        print(">>> Fp libiop parameters for {} <<<".format(name))
        print("const mp_size_t limbs = {};".format(ceil(self.num_bits/64)))
        print("libff::bigint<limbs> modulus(\"{}\");".format(self.p))

        print("Fp::Rsquared = libff::bigint<limbs>(\"{}\");".format(self.rsquared))
        print("Fp::Rcubed = libff::bigint<limbs>(\"{}\");".format(self.rcubed))
        print("Fp::inv = {};".format(self.inv))
        print("Fp::num_bits = {};".format(self.num_bits))
        print("Fp::euler = libff::bigint<limbs>(\"{}\");".format(self.euler))
        print("Fp::s = {};".format(self.s))
        print("Fp::t = libff::bigint<limbs>(\"{}\");".format(self.t))
        print("Fp::t_minus_1_over_2 = libff::bigint<limbs>(\"{}\");".format(self.t_minus_1_over_2))
        print("Fp::multiplicative_generator = Fp(\"{}\");".format(self.mul_gen))
        print("Fp::root_of_unity = Fp(\"{}\");".format(self.root_of_unity))
        print("Fp::nqr = Fp(\"{}\");".format(self.nqr))
        print("Fp::nqr_to_t = Fp(\"{}\");".format(self.nqr_to_t))


class Fp2Param():
    def __init__(self, p, num_bits, euler, s, t, t_minus_1_over_2, mul_gen, root_of_unity, nqr, nqr_to_t, frob_c0, frob_c1):
        self.p = p
        self.num_bits = num_bits
        self.euler = euler
        self.s = s
        self.t = t
        self.t_minus_1_over_2 = t_minus_1_over_2
        self.mul_gen = mul_gen
        self.root_of_unity = root_of_unity
        self.nqr = nqr
        self.nqr_to_t = nqr_to_t
        self.frob_c0 = frob_c0
        self.frob_c1 = frob_c1

    def print_libiop(self, name, simplify=True):
        print(">>> Fp2 libiop parameters for {} <<<".format(name))
        print("Fp2::euler = libff::bigint<2*limbs>(\"{}\");".format(self.euler))
        print("Fp2::s = {};".format(self.s))
        print("Fp2::t = libff::bigint<2*limbs>(\"{}\");".format(self.t))
        print("Fp2::t_minus_1_over_2 = libff::bigint<2*limbs>(\"{}\");".format(self.t_minus_1_over_2))
        print("Fp2::non_residue = Fp(\"{}\");".format(self.p - 1))
        print("Fp2::nqr = Fp2(Fp(\"{}\"),Fp(\"{}\"));".format(self.nqr.polynomial().list()[0], self.nqr.polynomial().list()[1]))
        print("Fp2::nqr_to_t = Fp2(Fp(\"{}\"),Fp(\"{}\"));".format(self.nqr_to_t.polynomial().list()[0], self.nqr_to_t.polynomial().list()[1]))
        print("Fp2::multiplicative_generator = Fp2(Fp(\"{}\"),Fp(\"{}\"));".format(self.mul_gen.polynomial().list()[0], self.mul_gen.polynomial().list()[1]))
        print("Fp2::root_of_unity = Fp2(Fp(\"{}\"), Fp(\"{}\"));".format(self.root_of_unity.polynomial().list()[0], self.root_of_unity.polynomial().list()[1]))
        print("Fp2::Frobenius_coeffs_c1[0] = Fp(\"{}\");".format(self.frob_c0))
        print("Fp2::Frobenius_coeffs_c1[1] = Fp(\"{}\");".format(self.frob_c1))
        if simplify:
            print("Fp2::simplify_mul = true;")


def generate_libff_Fp_model_params(prime):
    num_bits = ceil(log(prime, 2))
    euler = (prime-1)/2

    factorization = factor(prime-1)
    t = 0
    term_2 = factorization[0]
    counter = 0
    if term_2[0] != 2:
        raise BaseException("The prime decomposition doesn't have any factor 2."
        "The 'high 2-adicity' requirement isn't respected")
    while not(is_odd(t)):
        s = term_2[1] - counter
        t = (prime-1)/(2**s)
        counter = counter + 1
    is_odd(t)

    t_minus_1_over_2 = (t-1)/2
    multiplicative_generator = primitive_root(prime)
    root_of_unity = pow(multiplicative_generator, t, prime)
    nqr = least_quadratic_nonresidue(prime)
    nqr_to_t = pow(nqr, t, prime)

    #montgomery reduction params
    word_len_64_bits = 64
    W_64_bits = 2**(word_len_64_bits)
    k_64_bits = ceil(num_bits/word_len_64_bits)
    # print('k_64_bits (nb limbs) = {}'.format(k_64_bits))
    R_64_bits = mod(W_64_bits**k_64_bits, prime) # R_64_bits #why is this here
    Rsquared_64_bits = R_64_bits**2
    Rcubed_64_bits = R_64_bits**3
    inv_64_bits = hex(int(mod((1/-prime), W_64_bits)))

    return FpParam(prime, num_bits, euler, s, t, t_minus_1_over_2, multiplicative_generator, root_of_unity, nqr, nqr_to_t, Rsquared_64_bits, Rcubed_64_bits, inv_64_bits)

    # word_len_32_bits = 32
    # W_32_bits = 2**(32)
    # k_32_bits = ceil(num_bits/word_len_32_bits)
    # R_32_bits = mod(W_32_bits**k_32_bits, prime); R_32_bits
    # Rsquared_32_bits = R_32_bits**2
    # Rcubed_32_bits = R_32_bits**3
    # inv_32_bits = hex(int(mod(1/-prime, W_32_bits)))


def generate_libff_Fp2_model_params(prime):
    #requires p = 3 mod 4
    #uses twice the number of bits for an element
    num_bits = ceil(2*log(prime,2))

    # I don't know where this /2 term comes from? finding 2th root of unity?
    mult_group_size = (prime^2 - 1)
    euler = (mult_group_size)/2

    factorization = factor(mult_group_size)
    t = 0
    term_2 = factorization[0]
    counter = 0
    if term_2[0] != 2:
        raise BaseException("The prime decomposition doesn't have any factor 2."
        "The 'high 2-adicity' requirement isn't respected")
    # The code below is very strange. You are dividing the size of the mult. gp by the maximal power of 2, so of course it will never repeat the loop
    # while is_even(t):
    #     s = term_2[1] - counter
    #     t = ((prime+1)*(prime-1))/(2**s)
    #     counter = counter + 1

    # t corresponds to the order of the maximal subgroup not containing a point of order 2
    t = mult_group_size/(2**term_2[1])
    s = term_2[1]
    #another bad piece of code - is_odd is implied by the above lines
    is_odd(t)

    t_minus_1_over_2 = (t-1)/2

    F.<i> = GF(prime^2, modulus=x^2+1)
    multiplicative_gen = F.multiplicative_generator()

    #Calculate the maximal 2-th power root of unity
    root_of_unity = pow(multiplicative_gen, t, prime)

    #Generators are always non squares, and sage searches for gens from smallest to biggest - so we use this.
    nqr = multiplicative_gen

    nqr_to_t = pow(nqr, t, prime)

    #frobenius to even powers is always identity map
    # print('Frobenius_coeffs_c1[0] = {}'.format(1))
    # print('Frobenius_coeffs_c1[1] = {}'.format(prime-1))
    return Fp2Param(prime, num_bits, euler, s, t, t_minus_1_over_2, multiplicative_gen, root_of_unity, nqr, nqr_to_t, 1, prime-1)

# Not in use
# generate_libff_Fp_model_params(2^218*3^138*37 + 1).print_libiop("p441+")
# generate_libff_Fp_model_params(2^249*3^158*11 + 1).print_libiop("p503+")
# generate_libff_Fp_model_params(2^302*3^190*91 + 1).print_libiop("p610+")
# generate_libff_Fp_model_params(2^374*3^236*5 + 1).print_libiop("p751+")
# print()

generate_libff_Fp_model_params(2^218*3^138*37 + 1).print_libiop("p441+")
generate_libff_Fp_model_params(2^252*3^159*31 + 1).print_libiop("p509+")
generate_libff_Fp_model_params((2^307)*(3^192)*119 + 1).print_libiop("p619+")
generate_libff_Fp_model_params((2^372)*(3^239)*701 + 1).print_libiop("p761+")
print()

generate_libff_Fp_model_params(2^216*3^137 - 1).print_libiop("p434")
generate_libff_Fp2_model_params(2^216*3^137 - 1).print_libiop("p434x")
print()

generate_libff_Fp_model_params(2^0xfa*3^0x9f - 1).print_libiop("p503")
generate_libff_Fp2_model_params(2^0xfa*3^0x9f - 1).print_libiop("p503x")
print()

generate_libff_Fp_model_params(2^0x131*3^0xc0 - 1).print_libiop("p610")
generate_libff_Fp2_model_params(2^0x131*3^0xc0 - 1).print_libiop("p610x")
print()

generate_libff_Fp_model_params(2^0x174*3^0xef - 1).print_libiop("p751")
generate_libff_Fp2_model_params(2^0x174*3^0xef - 1).print_libiop("p751x")
print()
