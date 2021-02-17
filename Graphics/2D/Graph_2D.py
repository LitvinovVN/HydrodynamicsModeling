import numpy as np
import math
import sys
import matplotlib.pyplot as plt

# Функция func_open(file_name, r): считывание данных из файла file_name
# и запись данных в массив M_array_of_strings, состоящий их строк
def func_open(file_name, r):
    with open(file_name, r) as big_file:
        M_list_of_strings = big_file.read().strip().split('\n')  
    return M_list_of_strings

# Функция func_listStr_to_Array(M_array_of_strings): преобразует список строк в список одномерных массивов чисел
def func_listStr_to_Array(M_list_of_strings):
    result = []
    for row in M_list_of_strings:
        M_list_numbersAsStrings = row.split('\t')
        M1 = []
        for item in M_list_numbersAsStrings:
            try:
                M1.append(float(item))      
            except ValueError:    
                continue        
        M2 = np.array(M1)
        result.append(M2)       
    return result  

# Функция func_convert_array_to_numpyArray(M_array) преобразует список одномерных массивов M_array в двумерный массив numpy
def func_convert_array_to_numpyArray(M_array):
    n = len(M_array)
    m = len(M_array[0])
    M_numpyArray = np.zeros((n, m))
    for i in range(n):
        for j in range(m):
            M_numpyArray[i, j] = M_array[i][j]    
    return M_numpyArray

# В данной функции массив размером 3727х352 преобразуется в трехмерный массив размером 14х77х87 (kol x n x m)
# Такое количество элементов получаем с помощью усреднения значений в плоскости XOY (сумма элементов массива делится на 3х4 (n x m))
# Функция func_A(M, n, m, kol, kn, km): возвращает трехмерный массив размером 14х77х87 (kol x n x m)
def func_A(M_numpyArray2D, n, m, kol, kn, km):  
    M1 = np.zeros((n, m, kol))   
    for k in range(kol):                  
        for i in range(n):
            for j in range(m):                 
                S1 = 0                                           
                for k1 in range(kn):
                    S2 = 0
                    for k2 in range(km):                        
                        i1 = kn*i + nfirst*(k+1) + k1
                        j1 = km*j + mfirst + k2
                        S2 = S2 + M_numpyArray2D[i1,j1]
                    S1 = S1 + S2                  
                n1 = n-i-1
                n2 = j
                M1[n1,n2,k] = S1/(kn*km)            
    return M1

def mod(A_array, B_array):
    V = np.zeros((n, m, kol))
    for k in range(kol):
        for i in range(n):
            for j in range(m):
                V[i,j,k] = math.sqrt(math.pow(A_array[i,j,k],2) + math.pow(B_array[i,j,k],2))
    return V

def norm(A_array, B_array):
    V = np.zeros((n, m, kol))
    for k in range(kol):
        for i in range(n):
            for j in range(m):
                a = B_array[i,j,k]
                if not(a > -sys.float_info.epsilon and a < sys.float_info.epsilon):
                    V[i,j,k] = A_array[i,j,k]*math.pow(B_array[i,j,k],1./3)/B_array[i,j,k]
    return V

# Данные из файлов записываем в списки массивов, состоящих из строк
A1_array_of_strings = func_open('u.txt', 'r') 
B1_array_of_strings = func_open('v.txt', 'r')
E1_array_of_strings = func_open('w.txt', 'r')   
C1_array_of_strings = func_open('a.txt', 'r')    

# Преобразует списки массивов, состоящие из строк в списки одномерных массивов чисел
A1_array = func_listStr_to_Array(A1_array_of_strings)
B1_array = func_listStr_to_Array(B1_array_of_strings)
E1_array = func_listStr_to_Array(E1_array_of_strings)
C1_array = func_listStr_to_Array(C1_array_of_strings)

kol = 14
kn = 3 
km = 4
n = len(A1_array)//(kol*kn) #77
m = len(A1_array[0])//(km) #87
print(len(A1_array))
print(len(A1_array[0]))
print("n=", n, "m=", m)
nfirst = 233
mfirst = 0
i = 1j

# Преобразует списки массивов, состоящие из строк в двумерные массивы numpy
A1_numpyArray2D = func_convert_array_to_numpyArray(A1_array)
B1_numpyArray2D = func_convert_array_to_numpyArray(B1_array)
E1_numpyArray2D = func_convert_array_to_numpyArray(E1_array)
C1_numpyArray2D = func_convert_array_to_numpyArray(C1_array)

# Создаем трехмерные массивы numpy
A = func_A(A1_numpyArray2D, n, m, kol, kn, km)
B = func_A(B1_numpyArray2D, n, m, kol, kn, km)
E = func_A(E1_numpyArray2D, n, m, kol, kn, km)
C = func_A(C1_numpyArray2D, n, m, kol, kn, km)

# Вычисляем двумерный массив D - длина вектора скорости водного потока в проекции на плоскость XOY
D = mod(A, B)

#
A2 = norm(A, D)
B2 = norm(B, D)

data = C[:,:,0]
#data = data.transpose()
plt.imshow(data, origin='lower')
plt.savefig('imshow_1.png', format='png', dpi=1200, transparent = True)
