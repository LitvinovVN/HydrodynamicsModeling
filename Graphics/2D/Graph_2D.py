import numpy as np

# Функция func_open(file_name, r): считывание данных из файла file_name
# и запись данных в массив M_array_of_strings, состоящий их строк
def func_open(file_name, r):
    with open(file_name, r) as big_file:
        M_array_of_strings = big_file.read().split('\t')  
    return M_array_of_strings

# Функция func_open(M1, M_array_of_strings): исключение из массива строк символа "\n"
def func_exclude(M1, M_array_of_strings):
    for item in M_array_of_strings:
        try:
            M1.append(float(item))       
        except ValueError:    
            continue
    M2 = np.array(M1)      
    return M2


# Функция func_A(M, n, m, kol, kn, km): возвращает список M1 из kol массивов V(n, m)
def func_A(M, n, m, kol, kn, km):  
    M1 = []    
    for k in range(kol):
        V = np.zeros((n, m))            
        for i in range(n):
            for j in range(m):                 
                S1 = 0                             
                for k1 in range(kn):
                    for k2 in range(km):
                        i1 = kn * i + nfirst * (k+1) + k1
                        i2 = km * j + mfirst + k2
                        S1 = S1 + M[i2+i1*m]
                n1 = n-i-1
                n2 = j
                V[n1,n2] = S1/(kn*km)
        # print("___________k=", k)
        # print("\nS1=", S1)
        M1.append(V)
    return M1

B1 = []  # вектор v - проекция векора скорости на ось Y
A1 = []  # вектор u - проекция векора скорости на ось X
E1 = []  # вектор w - проекция векора скорости на ось Z
C1 = []  # вектор a - давление

kol = 14
n = 77
m = 87
kn = 3 
km = 4
nfirst = 233
mfirst = 0
i = 1j

# Данные из файлов записываем в массивы, состоящие из строк
A1_array_of_strings = func_open('u.txt', 'r') 
B1_array_of_strings = func_open('v.txt', 'r')
E1_array_of_strings = func_open('w.txt', 'r')   
C1_array_of_strings = func_open('a.txt', 'r')    

# Исключаем из массива строк символ "\n"
A1 = func_exclude(A1, A1_array_of_strings)
B1 = func_exclude(B1, B1_array_of_strings)
E1 = func_exclude(E1, E1_array_of_strings)
C1 = func_exclude(C1, C1_array_of_strings)


# Создаем список массивов
A = func_A(A1, n, m, kol, kn, km)
B = func_A(B1, n, m, kol, kn, km)
E = func_A(E1, n, m, kol, kn, km)
C = func_A(C1, n, m, kol, kn, km)
 
print(A)


