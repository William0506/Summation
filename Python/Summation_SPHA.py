f = open("D:\Programming\python\Bs.txt", 'r')#remember to change the file address
from SPHAmod import *
from math import ceil,factorial
from scipy.special import zeta
import os,sys
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
            cx, dx, ex = "", "", ""
            raise ValueError
    except:
        print("field num must be three non-negative integers separated by a comma.")
md = md(cx, dx, ex)
md1 = md1(cx, dx, ex)
md2 = md2(cx, dx, ex)
mp = mp(cx, dx, ex)
g=cx+dx+ex
r2 = np.zeros(2**(g))
d0 = np.full(2**(g), sys.float_info.max)
while not isinstance(k, np.ndarray) or len(k) != 2**(g):
    try:
        k_input = input(f"power(Using ',' to separate {2**(g)} parts.)(enter 'exit' or 'cls' to exit)?")
        if k_input == "exit" or k_input == "cls" or __file__.replace('\\', '/') in k_input:
            os._exit(0)
        k = md(*map(float, k_input.split(',')))
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
        if l <= 0:
            l = ""
            raise ValueError
        if l > 300: l = 300
        if k[0] % 1 == 0 and l > k[0] and k[0] > 0 and all(k[1:] == 0):
            l = int(k[0])+1
    except:
        print("precision must be a natural number.")
if np.all(k[1:] == 0) and k[0]!=-1:
    r1 = md(zeta(-k[0]), *([0] * (2**(g) - 1)))
elif np.all(k[1:] == 0) and k[0]==-1:
    r1 = md(0.577215664901532860606512090082402431042159335, *([0] * (2**(g) - 1)))
else:
    r1 = md(*([0] * 2**(g)))
for a in range(l + 1):
    if a % 2 == 1 and a != 1:
        continue
    d_temp = B[a] / factorial(a)
    d_temp = md1(md(*([d_temp] + [0] * (2**(g) - 1))), mp(n, md(k[0] - a + 1, *k[1:])))
    # b part
    if a == 0 and k[0] == -1 and np.all(k[1:] == 0):
        if n > 0:
            d_temp = md(log(n), *([0] * (2**(g) - 1)))
        else:
            d_temp = md(float('nan'), *([0] * (2**(g) - 1)))
        r1 = r1 + d_temp
        continue
    elif a > 0:
        for b in range(a - 1):
            d_temp = md1(d_temp, md(a + k[0] % 1 - int(k[0] % 1 != 0) - b, *([0] * (2**(g) - 1))))
            if k[0] != b:
                d_temp = md1(d_temp, md(1, *(k[1:] / (k[0] - b))))
            else:
                d_temp = md1(d_temp, md(0, *k[1:]))
    else:
        d_temp = md2(d_temp, md(k[0] % 1 - int(k[0] % 1 != 0) + 1, *([0] * (2**(g) - 1))))
        if k[0] != -1:
            d_temp = md2(d_temp, md(1, *(k[1:] / (k[0] + 1))))
        else:d_temp = md2(d_temp, md(0, *k[1:]))
    # c part
    if a == 1:
        pass
    elif a == 0 and k[0] == -1 and np.all(k[1:] == 0):
        d_temp = md1(d_temp, md(log(n), *([0] * (2**(g) - 1))))
    elif ceil(k[0]) - a > -1:
        for c in range(1, ceil(k[0]) - a + 1):
            d_temp = md1(d_temp, md((c + k[0] % 1 - int(k[0] % 1 != 0) + a) / (c + k[0] % 1 - int(k[0] % 1 != 0) + 1), *([0] * (2**(g) - 1))))
    else:
        for c in range(ceil(k[0]) - a + 1, 1):
            if c == -a and k[0] % 1 == 0:
                d_temp = md2(d_temp, md(1 - a, *([0] * (2**(g) - 1))))
            elif c == -1 and k[0] % 1 == 0:
                d_temp = md1(d_temp, md(a - 1, *([0] * (2**(g) - 1))))
            else:
                d_temp = md2(d_temp, md((c + k[0] % 1 - int(k[0] % 1 != 0) + a) / (c + k[0] % 1 - int(k[0] % 1 != 0) + 1), *([0] * (2**(g) - 1))))
    if np.all(np.abs(d_temp) >= np.abs(d0)) and a > k[0]:
        break
    r1 = r1 + d_temp
    d0 = d_temp
print("Approximation:", r1)
if n > 0 and n % 1 == 0:
    for i in range(1, int(n + 1)):
        r2 = r2 + mp(i, k)
elif n % 1 == 0:
    for i in range(int(n + 1), 1):
        r2 = r2 - mp(i, k)
else:r2 = None
print("Actual:", r2)
if r2 is not None:
    print("Error:", r1 - r2)