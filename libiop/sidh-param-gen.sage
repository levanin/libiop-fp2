for e3 in range(100, 200):
    for e2 in range(e3, 300):
        p = 2^e2 * 3^e3 + 1
        if is_prime(p):
            print("p = 2^{} * 3^{} + 1".format(e2, e3))

print("done")
