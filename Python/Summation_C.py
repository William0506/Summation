f = open("D:\Programming\python\Bs.txt", 'r')#remember to change the file address
from math import factorial,log,ceil
from scipy.special import zeta
import os
n=k=k2=l=""
r2=0
d0=10e+9999
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
        k=input("power,real part(enter 'exit' or 'cls' to exit)?")
        if k=="exit" or k=="cls" or __file__.replace('\\','/') in k:
            os._exit(0)
        k=float(k)
    except:
        print("power,real part must be a real number.")
while not isinstance(k2,float):
    try:
        k2=input("power,imaginary part(enter 'exit' or 'cls' to exit)?")
        if k2=="exit" or k2=="cls" or __file__.replace('\\','/') in k2:
            os._exit(0)
        k2=float(k2)
    except:
        print("power,imaginary part must be a real number.")
while not isinstance(l,int):
    try:
        l=input("precision(must be a natural number)(enter 'exit' or 'cls' to exit)?")
        if l=="exit" or l=="cls" or __file__.replace('\\','/') in l:
            os._exit(0)
        l=int(l)
        if l<=0:
            l=""
            raise ValueError
        if l > 300 : l = 300
        if k[0] % 1 == 0 and l > k and k > 0 and k2 == 0 : l = int(k)
        elif k%1==0 and l>k and k==0 : l=1
    except:
        print("precision must be a natural number.")
if k2==0:
    r1=zeta(-k)
else:
    r1=0
for a in range(l+1):
    if a%2==1 and a!=1:
        continue
    d=B[a]/factorial(a)*n**complex(k-a+1,k2)
    #b part
    if a==0 and k==-1:
        d=log(n)
        r1+=d
        continue
    elif a>0:
        for b in range(a-1):
            d*=a+k%1-int(k%1!=0)-b
            if k!=b:
                d*=complex(1,k2/(k-b))
            else:
                d*=complex(0,k2)
    else:
        d/=k%1-int(k%1!=0)+1
        if k!=-1:
            d/=complex(1,k2/(k+1))
        else:
            d/=complex(0,k2)
    #c part
    if a==1:
        pass
    elif a==0 and k==-1 and k2==0:
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
        r2+=i**complex(k,k2)
elif n%1==0:
    for i in range(int(n+1),1):
        r2-=i**complex(k,k2)
else:
    r2=None
print("Actual:", r2)
if r2 is not None:
    print("Error:", r1-r2)
