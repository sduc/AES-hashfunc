# R  : ring
# k  : number of non zero degrees
# deg: degree of the polynomial
def rand_poly(k,deg):
    F.<x> = GF(2)[]
    indices = []
    while len(indices) < k-2:
        i = randint(1,deg-1)
        if i not in indices:
            indices.append(i)
    p = x^deg + GF(2).one()
    for i in indices:
        p += x^i
    return p

TRIALS = 100

def find_sparse_irr_poly():
    F.<x> = GF(2)[]
    for k in range(2,10):
        for i in range(100):
            p = rand_poly(k,64)
            if p.is_irreducible():
                return p
    print "OMG I couldn't find a Polly Nomial"

def find_small():
    F.<x> = GF(2)[]
    while True:
        p = rand_poly(5,64)
        if p.is_irreducible(): #and (p - x^64).degree() < 15: 
            print p



