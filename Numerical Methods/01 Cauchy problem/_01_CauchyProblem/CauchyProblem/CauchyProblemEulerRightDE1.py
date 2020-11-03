"""Решение задачи Коши методом Эйлера - правая производная для ОДУ 1го порядка"""
import numpy as np
import math as mt
import matplotlib.pyplot as plt
from Euler import eulerRight

""" Функция правой части ОДУ """
def f(t, y):
    return -mt.sin(t)
    
# tn - начальное значение независимой переменной (первая точка расчетной сетки)
# tEnd - конечное значениe независимой переменной
# yn - начальное условие
# tau - шаг расчетной сетки
# f - функция правой части
# euler(f, t0, y0, tEnd, tau) - вызов явного метода Эйлера
# yp - вектор значений погрешности
# yd - вектор точных решений
tn = 3. * np.pi
tEnd = 0.
yn = -1.
tau = 0.0625
t, y = eulerRight(f, tn, yn, tEnd, tau)

yp = []
for i in range(0, len(t)):
    yp.append(mt.cos(t[i]) - y[i])
    

ti = tn
td = []
td.append(tn)
yd = []
yd.append(yn)
while ti > tEnd:
    tau = min(tau, mt.fabs(tEnd - ti))
    yd.append(mt.cos(ti))
    td.append(ti)
    ti = ti - 0.125

st = '$y$'
sg = '-'

st1 = '$yd$'
sg1 = '-'
    
plt.plot(t, y, sg, label = st)
plt.plot(td, yd, sg1, label = st1) 
#plt.plot(t, yp, sg, label = st) 
plt.legend(loc = 0)
plt.xlabel('$t$')
plt.grid(True)
plt.show()