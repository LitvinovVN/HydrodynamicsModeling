"""Решение задачи Коши методом Эйлера для ОДУ 1го порядка"""
import numpy as np
import math as mt
import matplotlib.pyplot as plt
from Euler import euler

""" Функция правой части ОДУ """
def f(t, y):
    return -mt.sin(t)
    

t0 = 0.
tEnd = 4. * np.pi
y0 = 1
tau = 0.0625
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

#st1 = '$yd$'
#sg1 = '-'
    
#plt.plot(t, y, sg, label = st)
#plt.plot(td, yd, sg1, label = st1) 
plt.plot(t, yp, sg, label = st) 
plt.legend(loc = 0)
plt.xlabel('$t$')
plt.grid(True)
plt.show()