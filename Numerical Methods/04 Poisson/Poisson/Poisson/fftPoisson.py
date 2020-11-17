import numpay as np
import math as mt
from lu3 import solveLU3

def fftPoisson(f, l1, l2, n1, n2):
    """
        Численное решение задачи Дирихле для уравнения Пуассона, используется преобразование Фурье для переменной x
    """

    h1 = l1 / n1
    h2 = l2 / n2
    y = np.zeros((n1 + 1, n2 + 1), 'float')
    r = np.zeros((2 * n1), 'float')
    tt = np.zeros((n1 + 1), 'cfloat')

    for j in range(1, n2):
        for i in range(1, n1):
            r[i] = f(i * h1, j * h2)
            r[2 * n1 - i] = -r[i]
        rt = np.fft.rfft(r).imag


