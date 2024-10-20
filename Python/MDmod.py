import numpy as np
from math import *
def md(n):
    """Creating MD"""
    return lambda *args: np.array(args[:2**n])
def md1(n):
    """MD Multiplication"""
    def mul(a, b):
        result = np.zeros(2**n)
        for i in range(2**n):
            for j in range(2**n):
                k = i ^ j  # XOR operation
                result[k] += a[i] * b[j]
        return result
    return mul
def md2(n):
    """MD Division"""
    def div(a, b):
        if n == 0:
            return np.array([a[0]/b[0]])
        if n == 1:
            denominator = b[0]**2 - b[1]**2
            return np.array([(a[0]*b[0] - a[1]*b[1]) / denominator, (a[1]*b[0] - a[0]*b[1]) / denominator])
        else:
            b1 = b[:2**(n-1)]
            b2 = b[2**(n-1):]
            numerator = md1(n)(a, np.concatenate((b1, -b2)))
            denominator = md1(n-1)(b1, b1) - md1(n-1)(b2, b2)
            return np.concatenate((md2(n-1)(numerator[:2**(n-1)], denominator), md2(n-1)(numerator[2**(n-1):], denominator)))
    return div
def mp(n):
    """MD Exponentiation"""
    def pow(a, b):
        result = np.zeros(2**n)
        result[0] = a**b[0]
        for i in range(1, 2**n):
            term = np.zeros(2**n)
            term[0] = cosh(b[i] * log(a))
            term[i] = sinh(b[i] * log(a))
            result = md1(n)(result, term)
        return result
    return pow