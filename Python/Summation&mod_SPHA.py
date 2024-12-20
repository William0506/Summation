f = open("D:\Programming\python\Bs.txt", 'r')#remember to change the file address
import numpy as np
from math import log,cos,sin,cosh,sinh,ceil,factorial,pi,e
from scipy.special import zeta
import os,sys
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
            a1, a2, b1, b2= a[:2**(n-1)], a[2**(n-1):], b[:2**(n-1)], b[2**(n-1):]
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
        if a != 0:
            abs_a = abs(a)
            log_abs_a = log(abs_a)
            if a > 0 : result[0] = a**b[0]
            else:
                result[0] = abs_a**b[0] * cos(b[0]*pi)
                result[2**(n-1)] = abs_a**b[0] * sin(b[0]*pi)
            for i in range(1, 2**n):
                term = np.zeros(2**n)
                ps_i = ps(i, x, y, z)
                if ps_i > 0:
                    term[0] = cosh(b[i] * log_abs_a)
                    term[i] = sinh(b[i] * log_abs_a)
                elif ps_i < 0:
                    term[0] = cos(b[i] * log_abs_a)
                    term[i] = sin(b[i] * log_abs_a)
                else:
                    term[0] = 1
                    term[i] = b[i] * log_abs_a
                result = md1(x, y, z)(result, term)
                if a < 0 : result *= e**(ps_i * pi * b[i])
        else:
            if np.all(b == 0) : result[0] = 1
        return result
    return pow
n = k = l = cx = dx = ex = ""
B=[]
for t in f.readlines():
    B.append(float(t))
f.close()
# prefix
while not isinstance(n, float):
    try:
        n = input("upper bound of summation(enter 'exit' or 'cls' to exit)?")
        if n == "exit" or n == "cls" or __file__.replace('\\', '/') in n:
            os._exit(0)
        n = float(n)
    except:
        print("upper bound of summation must be a real number.")
while not isinstance(cx, int) or not isinstance(dx, int) or not isinstance(ex, int):
    try:
        cdex = input("field num (three non-negative integers:cx,dx,ex) (enter 'exit' or 'cls' to exit)?")
        if cdex == "exit" or cdex == "cls" or __file__.replace('\\', '/') in cdex:
            os._exit(0)
        cx, dx, ex = map(int, cdex.split(','))
        if cx < 0 or dx < 0 or ex < 0:
            cx, dx, ex = "", ""
            raise ValueError
    except:
        print("field num must be three non-negative integers separated by a comma.")
if n < 0 and cx == dx == ex == 0:
    md_ = md(1, 0, 0)
    md1_ = md1(1, 0, 0)
    md2_ = md2(1, 0, 0)
    mp_ = mp(1, 0, 0)
    g=1
else:
    md_ = md(cx, dx, ex)
    md1_ = md1(cx, dx, ex)
    md2_ = md2(cx, dx, ex)
    mp_ = mp(cx, dx, ex)
    g=cx+dx+ex
r2 = np.zeros(2**(g))
d0 = np.full(2**(g), sys.float_info.max)
while not isinstance(k, np.ndarray) or len(k) != 2**(g):
    try:
        k_input = input(f"power(Using ',' to separate {2**(g)} parts.)(enter 'exit' or 'cls' to exit)?")
        if k_input == "exit" or k_input == "cls" or __file__.replace('\\', '/') in k_input:
            os._exit(0)
        k = md_(*map(float, k_input.split(',')))
        if len(k) != 2**(g):
            raise ValueError
    except:
        print(f"power must be {2**(g)} numbers in the field you set.")
while not isinstance(l, int):
    try:
        l = input("precision(must be a natural number)(enter 'exit' or 'cls' to exit)?")
        if l == "exit" or l == "cls" or __file__.replace('\\', '/') in l:
            os._exit(0)
        l = int(l)
        if l < 0:
            l = ""
            raise ValueError
        if l > 300: l = 300
        if k[0] % 1 == 0 and l > k[0] and k[0] > 0 and all(k[1:] == 0):
            l = int(k[0])+1
        elif l>k[0] and k[0]==0 and np.all(k[1:] == 0): l=1
    except:
        print("precision must be a natural number.")
if np.all(k[1:] == 0) and k[0]!=-1:
    r1 = md_(zeta(-k[0]), *([0] * (2**(g) - 1)))
elif k[0]==-1:
    r1 = md_(0.577215664901532860606512090082402431042159335, *([0] * (2**(g) - 1)))
else:
    r1 = md_(*([0] * 2**(g)))
for a in range(l + 1):
    if a % 2 == 1 and a != 1:
        continue
    d_temp = B[a] / factorial(a)
    d_temp = md1_(md_(*([d_temp] + [0] * (2**(g) - 1))), mp_(n, md_(k[0] - a + 1, *k[1:])))
    # b part
    if a == 0 and k[0] == -1 and np.all(k[1:] == 0):
        if n > 0:
            d_temp = md_(log(n), *([0] * (2**(g) - 1)))
        else:
            d_temp = md_(float('nan'), *([0] * (2**(g) - 1)))
        r1 = r1 + d_temp
        continue
    elif a > 0:
        for b in range(a - 1):
            d_temp = md1_(d_temp, md_(a + k[0] % 1 - int(k[0] % 1 != 0) - b, *([0] * (2**(g) - 1))))
            if k[0] != b:
                d_temp = md1_(d_temp, md_(1, *(k[1:] / (k[0] - b))))
            else:
                d_temp = md1_(d_temp, md_(0, *k[1:]))
    else:
        d_temp = md2_(d_temp, md_(k[0] % 1 - int(k[0] % 1 != 0) + 1, *([0] * (2**(g) - 1))))
        if k[0] != -1:
            d_temp = md2_(d_temp, md_(1, *(k[1:] / (k[0] + 1))))
        else:d_temp = md2_(d_temp, md_(0, *k[1:]))
    # c part
    if a == 1:
        pass
    elif a == 0 and k[0] == -1 and np.all(k[1:] == 0):
        d_temp = md1_(d_temp, md_(log(n), *([0] * (2**(g) - 1))))
    elif ceil(k[0]) - a > -1:
        for c in range(1, ceil(k[0]) - a + 1):
            d_temp = md1_(d_temp, md_((c + k[0] % 1 - int(k[0] % 1 != 0) + a) / (c + k[0] % 1 - int(k[0] % 1 != 0) + 1), *([0] * (2**(g) - 1))))
    else:
        for c in range(ceil(k[0]) - a + 1, 1):
            if c == -a and k[0] % 1 == 0:
                d_temp = md2_(d_temp, md_(1 - a, *([0] * (2**(g) - 1))))
            elif c == -1 and k[0] % 1 == 0:
                d_temp = md1_(d_temp, md_(a - 1, *([0] * (2**(g) - 1))))
            else:
                d_temp = md2_(d_temp, md_((c + k[0] % 1 - int(k[0] % 1 != 0) + a) / (c + k[0] % 1 - int(k[0] % 1 != 0) + 1), *([0] * (2**(g) - 1))))
    if np.all(np.abs(d_temp) >= np.abs(d0)) and a > k[0]:
        break
    r1 = r1 + d_temp
    d0 = d_temp
print("Approximation:", r1)
if n > 0 and n % 1 == 0:
    for i in range(1, int(n + 1)):
        r2 = r2 + mp_(i, k)
elif n % 1 == 0:
    for i in range(int(n + 1), 1):
        r2 = r2 - mp_(i, k)
else:r2 = None
print("Actual:", r2)
if r2 is not None:
    print("Error:", r1 - r2)
    
