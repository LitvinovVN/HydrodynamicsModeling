"""Решение задачи Коши методом Эйлера - левая производная для ОДУ 1го порядка"""
import numpy as np
import math as mt
import matplotlib.pyplot as plt
from Euler import euler

""" Функция правой части ОДУ """
def f(t, y):
    res = 0
    # m - список пар, каждая пара - это точка сетки и значение функции правой части в этой точке
    m = [(0., 0.), (0.25, -0.2474), (0.5, -0.4794), (0.75, -0.6816), (1., -0.8414), (1.25, -0.9489), (1.5, -0.9974), (1.75, -0.9839), (2., -0.9093), (2.25, -0.7780), (2.5, -0.5984), (2.75, -0.3816), (3.0, -0.1411), (3.25, 0.1081), (3.5, 0.3507), (3.75, 0.5715), (4.0, 0.7568), (3.5, 0.3507), (4.25, 0.8949), (4.5, 0.9775), (4.75, 0.9992), (5.0, 0.9589)]
    for i in range(0, len(m)):
        curEl = m[i]
        fst = curEl[0]
        snd = curEl[1]
        if(mt.fabs(fst-t)<0.001):
            return snd
    return NaN
    
# t0 - начальное значение независимой переменной (первая точка расчетной сетки)
# tEnd - конечное значениe независимой переменной
# y0 - начальное условие
# tau - шаг расчетной сетки
# f - функция правой части
# euler(f, t0, y0, tEnd, tau) - вызов явного метода Эйлера
# yp - вектор значений погрешности
# yd - вектор значений точных решений
t0 = 0.
tEnd = 5.
y0 = 1
tau = 0.25
t, y = euler(f, t0, y0, tEnd, tau)

yp = []
for i in range(0, len(t)):
    yp.append(mt.cos(t[i]) - y[i])
    

ti = t0
td = []
td.append(t0)
yd = []
yd.append(y0)
while ti < tEnd:
    tau = min(tau, tEnd - ti)
    yd.append(mt.cos(ti))
    td.append(ti)
    ti = ti + 0.125

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