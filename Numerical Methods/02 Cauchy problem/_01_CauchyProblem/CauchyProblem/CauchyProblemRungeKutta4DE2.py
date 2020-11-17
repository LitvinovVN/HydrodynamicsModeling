"""Решение задачи Коши методом Рунге-Кутта 4го порядка для ОДУ 1го порядка"""
import numpy as np
import math as mt
import matplotlib.pyplot as plt
from rungeKutta4 import rungeKutta4

""" Функция правой части ОДУ """
def f(t, y):
    f = np.zeros((2),'float')
    f[0] = y[1]
    f[1] = - mt.sin(y[0])
    return f

t0 = 0.
tEnd = 4. * np.pi
y0 = np.array([1., 0.])
tau = 0.25
t, y = rungeKutta4(f, t0, y0, tEnd, tau)

for n in range(0, 2):
    r = y[:, n]
    st = '$y$'
    sg = '-'
    if n == 1:
        st = '$\\frac{d y}{dt}$'
        sg = '--'
    plt.plot(t, r, sg, label = st)
plt.legend(loc = 0)
plt.xlabel('$t$')
plt.grid(True)
plt.show()