import numpy as np
from math import *
def md(n):
    """Creating ME"""
    return lambda *args: np.array(args[:2**n])
def md1(n):
    """ME Multiplication"""
    def mul(a, b):
        if n == 0:
            return np.array([a[0]*b[0]])
        if n == 1:
            return np.array([a[0]*b[0], a[0]*b[1] + a[1]*b[0]])
        else:
            a1, a2 = a[:2**(n-1)], a[2**(n-1):]
            b1, b2 = b[:2**(n-1)], b[2**(n-1):]
            c1 = md1(n-1)(a1, b1)
            c2 = md1(n-1)(a1, b2) + md1(n-1)(a2, b1)
            return np.concatenate((c1, c2))
    return mul
def md2(n):
    """ME Division"""
    def div(a, b):
        if n == 0:
            return np.array([a[0]/b[0]])
        if n == 1:
            denominator = b[0]**2
            return np.array([(a[0]*b[0]) / denominator, (a[1]*b[0] - a[0]*b[1]) / denominator])
        else:
            b1 = b[:2**(n-1)]
            b2 = b[2**(n-1):]
            numerator = md1(n)(a, np.concatenate((b1, -b2)))
            denominator = md1(n-1)(b1, b1)
            return np.concatenate((md2(n-1)(numerator[:2**(n-1)], denominator), md2(n-1)(numerator[2**(n-1):], denominator)))
    return div
def mp(n):
    """ME Exponentiation"""
    def pow(a, b):
        result = np.zeros(2**n)
        result[0] = a**b[0]
        for i in range(1, 2**n):
            term = np.zeros(2**n)
            term[0] = 1
            term[i] = b[i] * log(a)
            result = md1(n)(result, term)
        return result
    return pow