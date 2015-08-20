def gfmult(a,b):
    c = poly2int(int2poly(a)*int2poly(b))
    tmp2 = c >> 64
    
    tmp2 = tmp2 ^^ (tmp2 >> 60) ^^ (tmp2 >> 61) ^^ (tmp2 >> 62)
    tmp2 = tmp2 ^^ ((tmp2 << 4) & (2^64-1))  ^^\
            ((tmp2 << 3) & (2^64-1)) ^^\
            ((tmp2 << 2) & (2^64-1))

    tmp3 = c & (2^64-1)
    return tmp3 ^^ tmp2

def std_gfmult(a,b):
    ap,bp = int2poly(a),int2poly(b)
    abp = ap*bp
    abp = abp.mod(P)
    return poly2int(abp)

#############################################################

def test_gfmult():
    a = 0xff12123123
    b = 0xefefffffff
    ab = gfmult(a,b)
    abstd = std_gfmult(a,b)
    assert  ab == abstd , "std: " + hex(abstd) + ", opt: " + hex(ab)
    
############################################################


F.<X> = GF(2)[]
P = X^64 + X^4 + X^3 + X^2 + 1

def int2poly(y):
    x = bin(y)
    poly = 0
    for i in range(2,len(x)):
        poly = poly * X + int(x[i])
    return poly

def poly2int(y):
    x = y.coeffs()
    res = 0
    for i in range(len(x)):
        res += int(x[i])*2^i
    return res
