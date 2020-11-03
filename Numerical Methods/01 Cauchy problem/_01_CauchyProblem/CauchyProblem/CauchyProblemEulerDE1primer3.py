"""Решение задачи Коши методом Эйлера - левая производная для ОДУ 1го порядка"""
import numpy as np
import math as mt
import matplotlib.pyplot as plt
from Euler import euler

""" Функция правой части ОДУ """
def f(t, y):
    res = 0
    for i in range(0, len(m)):
        curEl = m[i]
        fst = curEl[0]
        snd = curEl[1]
        if(mt.fabs(fst-t)<0.001):
            return snd
    return NaN

# m - список пар, каждая пара - это точка сетки и значение функции правой части в этой точке. список m считывается из файла dataSin.txt
m = []    
file = open("dataSin.txt")
for line in file:
    print(line)
    line = line.replace(',', '.')
    line = line[:len(line)-1]
    k = line.split('\t')
    fst =  float(k[0])
    snd = float(k[1])
    m.append((fst, snd))

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