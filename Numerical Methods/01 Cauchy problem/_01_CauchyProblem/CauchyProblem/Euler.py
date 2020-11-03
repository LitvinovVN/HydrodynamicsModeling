import numpy as np
import Gauss
import math as mt

""" Явный метод Эйлера - левая производная"""

def euler(f, t0, y0, tEnd, tau):
    t = []
    y = []
    t.append(t0)
    y.append(y0)

    ti = t0
    yi = y0

    while ti < tEnd:
        tau = min(tau, tEnd - ti)
        yi = yi + tau * f(ti, yi)
        ti = ti + tau
        t.append(ti)
        y.append(yi)
    return np.array(t), np.array(y)

""" Явный метод Эйлера - правая производная"""

def eulerRight(f, t0, y0, tEnd, tau):
    t = []
    y = []
    t.append(t0)
    y.append(y0)

    ti = t0
    yi = y0

    while ti > tEnd:
        tau = min(tau, mt.fabs(tEnd - ti))
        yi = yi - tau * f(ti, yi)
        ti = ti - tau
        t.append(ti)
        y.append(yi)
    t.reverse()
    y.reverse()
    return np.array(t), np.array(y)

""" Неявный метод Эйлера - левая производная"""

def eulerImplicit(f, t0, y0, tEnd, tau):
    n = int((tEnd - t0)//tau + 1)

    t = []
    t.append(t0)
    ti = t0    
    a = []
    for row in range(0, n):
        if row>0:
            ti = ti + tau
            t.append(ti)   
        a.append([])
        for col in range(0, n + 1):
            a[row].append(0)

    a[0][0] = 1 #Начальное условие
    a[0][n] = y0
    col = 0
    for row in range(1, n):                 
        a[row][row-1] = -1
        a[row][row] = 1
        a[row][n] = tau * f(t[row])

    a = np.array(a)

    #a = np.array([[ 2. ,  10.0, -3.0,  38.0],
                #[ -3.,   -24.,  5.0,  -86.0],
                #[ 1.,   3.,   -5.,   27.0 ]])
        
    y = Gauss.gaussFunc(a)    
    
    return np.array(t), np.array(y)

""" Неявный метод Эйлера - правая производная"""

def eulerImplicit(f, t0, y0, tEnd, tau):
    n = int((tEnd - t0)//tau + 1)

    t = []
    t.append(t0)
    ti = t0    
    a = []
    for row in range(0, n):
        if row>0:
            ti = ti + tau
            t.append(ti)   
        a.append([])
        for col in range(0, n + 1):
            a[row].append(0)

    a[0][0] = 1 #Начальное условие
    a[0][n] = y0
    col = 0
    for row in range(1, n):                 
        a[row][row-1] = -1
        a[row][row] = 1
        a[row][n] = tau * f(t[row])

    a = np.array(a)

    #a = np.array([[ 2. ,  10.0, -3.0,  38.0],
                #[ -3.,   -24.,  5.0,  -86.0],
                #[ 1.,   3.,   -5.,   27.0 ]])
        
    y = Gauss.gaussFunc(a)    
    
    return np.array(t), np.array(y)