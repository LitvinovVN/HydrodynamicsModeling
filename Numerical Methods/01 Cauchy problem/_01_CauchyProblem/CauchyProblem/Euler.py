import numpy as np

""" Метод Эйлера"""

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
