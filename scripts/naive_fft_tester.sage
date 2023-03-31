# This sage script tests the evaluation of a polynomial over a coset (or subgroup) containing the 2^n-th roots of unity.

def subgroup_generator(order, field, gen):
    return gen^((field.order()-1)/order)

def eval_poly_over_subgp(gen, poly_coeffs, field):
    P = PolynomialRing(F, 'x')

    #construct polynomial
    f = P(0)
    for i in range(len(poly_coeffs)):
        f += P(poly_coeffs[i]*x^i)
    #evaluate polynomial
    evaluations = []
    for i in range(gen.multiplicative_order()):
        evaluations.append(f(gen^i))
    return evaluations




prime = 2^216*3^137 -1
F.<i> = GF(prime^2, modulus=x^2+1)
mult_gen = F.multiplicative_generator()
order = 2^3
poly_coeffs = [1,1,1,1,1,1,1,1]
print(eval_poly_over_subgp(subgroup_generator(order,F,mult_gen),poly_coeffs, F))