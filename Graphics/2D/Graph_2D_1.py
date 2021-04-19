import numpy as np
import seaborn as sns
import math
import sys
import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.axes_grid1.inset_locator import InsetPosition
from matplotlib.colors import ListedColormap

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
        M_list_numbersAsStrings = row.split('   ')
        M1 = []
        for item in M_list_numbersAsStrings:
            try:
                M1.append(float(item))      
            except ValueError:    
                continue        
        M2 = np.array(M1)
        result.append(M2)       
    return result  

# Функция func_convert_array_to_numpyArray(M_array) преобразует список одномерных массивов M_array в двумерный
# массив numpy
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

# construct cmap
#flatui = ["#ffffff", "#ff8c00", "#ffff00", "#adff2f", "#00ff00", "#40e0d0", "#0000ff", "#00008b",]
#flatui = ["#9b59b6", "#3498db", "#95a5a6", "#e74c3c", "#34495e", "#2ecc71"]
#my_cmap = ListedColormap(sns.color_palette(flatui).as_hex())
my_cmap = sns.light_palette("Navy", as_cmap=True)
cmap = ListedColormap(sns.color_palette("Spectral",256))

# Функция для построения графиков 2D, входные данные: массив A,
# цветовая карта - 'CMRmap', имя файла - 'imshow_C.png'
def graph_2D(A_array, cmap_use, file_name):
    data = A_array[:,:]
    fig, (ax, cax) = plt.subplots(ncols=2,figsize=(7,3), 
                        gridspec_kw={"width_ratios":[1,0.05]})
    fig.subplots_adjust(wspace=0.3)
    im  = ax.imshow(data, cmap=cmap_use, origin='lower')
    ax.set_ylabel("Ось Y")
    ax.set_xlabel("Ось Х")
    fig.colorbar(im, cax=cax, ax=ax)
    plt.savefig(file_name, format='png', dpi=1200, transparent = True)
    return



# Данные из файлов записываем в списки массивов, состоящих из строк
A1_array_of_strings = func_open('writed_4.txt', 'r') 

# Преобразует списки массивов, состоящие из строк в списки одномерных массивов чисел
A1_array = func_listStr_to_Array(A1_array_of_strings)

gridX = 38
gridY = 38
gridZ = 1
kol = gridZ
kn = 1
km = 1
n = gridY//kn
m = gridX//km
nfirst = 38
mfirst = 0
i = 1j

# Преобразует списки массивов, состоящие из строк в двумерные массивы numpy
A = func_convert_array_to_numpyArray(A1_array)

graph_2D(A, 'jet', 'imshow_D.png')

plt.show()

