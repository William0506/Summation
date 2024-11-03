f = open("D:\Programming\python\B.txt", 'r')#remember to change the file address
from math import factorial,log,ceil
from scipy.special import zeta
import os,sys
n=k=l=""
B=[]
for t in f.readlines():
    B.append(float(t))
f.close()
while not isinstance(n,float):
    try:
        n=input("upper bound of summation(enter 'exit' or 'cls' to exit)?")
        if n=="exit" or n=="cls" or __file__.replace('\\','/') in n:
            os._exit(0)
        n=float(n)
    except:
        print("upper bound of summation must be a real number.")
while not isinstance(k,float):
    try:
        k=input("power(enter 'exit' or 'cls' to exit)?")
        if k=="exit" or k=="cls" or __file__.replace('\\','/') in k:
            os._exit(0)
        k=float(k)
    except:
        print("power must be a real number.")
while not isinstance(l,int):
    try:
        l=input("precision(must be a natural number)(enter 'exit' or 'cls' to exit)?")
        if l=="exit" or l=="cls" or __file__.replace('\\','/') in l:
            os._exit(0)
        l=int(l)
        if l<0:
            l=""
            raise ValueError
        if l>300 : l=300
        if k%1==0 and l>k and k>0 : l=int(k)+1
        elif k%1==0 and l>k and k==0 : l=1
    except:
        print("precision must be a natural number.")
r2=0
d0=sys.float_info.max
if k!=-1:r1=zeta(-k)
else:r1=0.577215664901532860606512090082402431042159335
for a in range(l+1):
    if a%2==1 and a!=1:
        continue
    d=B[a]/factorial(a)*n**(k-a+1)
    #b part
    if a==0 and k==-1:
        if n>0:
            d=log(n)
        else:
            d=float('nan')
        r1+=d
        continue
    elif a>0:
        for b in range(a-1):
            d*=a+k%1-int(k%1!=0)-b
    else:
        d/=k%1-int(k%1!=0)+1
    #c part
    if a==1:
        pass
    elif a==0 and k==-1:
        d*=log(n)
    elif ceil(k)-a>-1:
        for c in range(1,ceil(k)-a+1):
            d*=(c+k%1-int(k%1!=0)+a)/(c+k%1-int(k%1!=0)+1)
    else:
        for c in range(ceil(k)-a+1,1):
            if c==-a and k%1==0:
                d/=1-a
            elif c==-1 and k%1==0:
                d*=a-1
            else:
                d/=(c+k%1-int(k%1!=0)+a)/(c+k%1-int(k%1!=0)+1)
    if abs(d)>=abs(d0) and a>k:
        break
    r1+=d
    d0=d
print("Approximation:", r1)
if n>0 and n%1==0:
    for i in range(1,int(n+1)):
        r2+=i**k
elif n%1==0:
    for i in range(int(n+1),1):
        r2-=i**k
else:
    r2=None
print("Actual:", r2)
print("Error:", r1-r2)
