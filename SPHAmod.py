#The iteration order is E=>D=>C
import numpy as np
from math import cosh,sinh,cos,sin,log
def md(x, y, z):
    """Creating SPHA"""
    return lambda *args: np.array(args[:2**(x+y+z)])
def md1(x, y, z):
    """SPHA Multiplication"""
    def mul(a, b):
        n = x + y + z
        if n == 0 : return np.array([a[0]*b[0]])
        if n == 1:
            if x == 1 : return np.array([a[0]*b[0] - a[1]*b[1], a[0]*b[1] + a[1]*b[0]])
            elif y == 1 : return np.array([a[0]*b[0] + a[1]*b[1], a[0]*b[1] + a[1]*b[0]])
            else : return np.array([a[0]*b[0], a[0]*b[1] + a[1]*b[0]])
        else:
            a1, a2 = a[:2**(n-1)], a[2**(n-1):]
            b1, b2 = b[:2**(n-1)], b[2**(n-1):]
            if z > 0:
                c1 = md1(x, y, z-1)(a1, b1)
                c2 = md1(x, y, z-1)(a1, b2) + md1(x, y, z-1)(a2, b1)
            elif y > 0:
                c1 = md1(x, y-1, z)(a1, b1) + md1(x, y-1, z)(a2, b2)
                c2 = md1(x, y-1, z)(a1, b2) + md1(x, y-1, z)(a2, b1)
            else:
                c1 = md1(x-1, y, z)(a1, b1) - md1(x-1, y, z)(a2, b2)
                c2 = md1(x-1, y, z)(a1, b2) + md1(x-1, y, z)(a2, b1)
            return np.concatenate((c1, c2))
    return mul
def md2(x, y, z):
    """SPHA Division"""
    def div(a, b):
        n = x + y + z
        if n == 0 : return np.array([a[0]/b[0]])
        if n == 1:
            if x == 1:
                denominator = b[0]**2 + b[1]**2
                return np.array([(a[0]*b[0] + a[1]*b[1]) / denominator,(a[1]*b[0] - a[0]*b[1]) / denominator])
            elif y == 1:
                denominator = b[0]**2 - b[1]**2
                return np.array([(a[0]*b[0] - a[1]*b[1]) / denominator,(a[1]*b[0] - a[0]*b[1]) / denominator])
            else : return np.array([a[0]*b[0] / b[0]**2,(a[1]*b[0] - a[0]*b[1]) / b[0]**2])
        else:
            b1 = b[:2**(n-1)]
            b2 = b[2**(n-1):]
            numerator = md1(x, y, z)(a, np.concatenate((b1, -b2)))
            if z > 0:
                denominator = md1(x, y, z-1)(b1, b1)
                return np.concatenate((md2(x, y, z-1)(numerator[:2**(n-1)], denominator), md2(x, y, z-1)(numerator[2**(n-1):], denominator)))
            elif y > 0:
                denominator = md1(x, y-1, z)(b1, b1) - md1(x, y-1, z)(b2, b2)
                return np.concatenate((md2(x, y-1, z)(numerator[:2**(n-1)], denominator), md2(x, y-1, z)(numerator[2**(n-1):], denominator)))
            else:
                denominator = md1(x-1, y, z)(b1, b1) + md1(x-1, y, z)(b2, b2)
                return np.concatenate((md2(x-1, y, z)(numerator[:2**(n-1)], denominator), md2(x-1, y, z)(numerator[2**(n-1):], denominator)))
    return div
def ps(index, x, y, z):#informal function
    if index >= 2**(x+y): return 0
    binary = bin(index)[2:].zfill(x+y+z)[y+z:]
    ones_count = binary.count('1')
    return (ones_count % 2 == 0)*2-1
def mp(x, y, z):
    """SPHA Exponentiation"""
    def pow(a, b):
        n = x + y + z
        result = np.zeros(2**n)
        result[0] = a**b[0]
        for i in range(1, 2**n):
            term = np.zeros(2**n)
            if ps(i, x, y, z) > 0:
                term[0] = cosh(b[i] * log(abs(a)))
                term[i] = sinh(b[i] * log(abs(a)))
            elif ps(i, x, y, z) < 0:
                term[0] = cos(b[i] * log(abs(a)))
                term[i] = sin(b[i] * log(abs(a)))
            else:
                term[0] = 1
                if a!=0:
                    term[i] = b[i] * log(a)
            result = md1(x, y, z)(result, term)
        return result
    return pow