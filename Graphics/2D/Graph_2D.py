
B1 = []
A1 = []
E1 = []
C1 = []

# Данные из файлов записываем в массивы, состоящие из строк
with open('v.txt', 'r') as big_file:
    B1_array_of_strings = big_file.read().split('\t')    

with open('u.txt', 'r') as big_file:
    A1_array_of_strings = big_file.read().split('\t')    

with open('w.txt', 'r') as big_file:
    E1_array_of_strings = big_file.read().split('\t')    

with open('a.txt', 'r') as big_file:
    C1_array_of_strings = big_file.read().split('\t')
   

# Исключаем из массива строк символ "\n"
for item in B1_array_of_strings:
    try:
        B1.append(float(item))       
    except ValueError:        
        continue
    
for item in A1_array_of_strings:
    try:
        A1.append(float(item))       
    except ValueError:        
        continue

for item in E1_array_of_strings:
    try:
        E1.append(float(item))       
    except ValueError:        
        continue

for item in C1_array_of_strings:
    try:
        C1.append(float(item))       
    except ValueError:        
        continue

kol = 14
n = 17
m = 87
kn = 3
km = 4
nfirst = 233
mfirst = 0
i = 1j

V = []
def func_A(A):
    for k in range(kol):
        for i in range(n):
            for j in range(m):
                S1 = 0            
                for k1 in range(kn):
                    for k2 in range(km):
                        i1 = kn * i + nfirst * (k+1) + k1
                        i2 = km * j + mfirst + k2
                        S1 = S1 + A[i1,i2]
                V[n-i-1,j] = S1/(kn*km)
    
