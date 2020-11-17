import numpy as np

""" Метод Рунге-Кутта 4 порядка для ОДУ 2го порядка """
def rungeKutta4(f, t0, y0, tEnd, tau):
    def increment(f, t, y, tau):
        k0 = tau * f(t, y)
        k1 = tau * f(t + tau/2., y + k0 / 2.)
        k2 = tau * f(t + tau/2., y + k1 / 2.)
        k3 = tau * f(t + tau, y + k2)
        return (k0 + 2.*k1 + 2.*k2 + k3)/6
    t = []
    y = []
    t.append(t0)
    y.append(y0)
    while t0 < tEnd:
        tau = min(tau, tEnd - t0)
        y0 = y0 + increment(f, t0, y0, tau)
        t0 = t0 + tau
        t.append(t0)
        y.append(y0)
    return np.array(t), np.array(y)

