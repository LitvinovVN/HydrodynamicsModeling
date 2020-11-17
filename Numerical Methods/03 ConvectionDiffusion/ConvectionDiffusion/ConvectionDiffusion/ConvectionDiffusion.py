
import matplotlib.pyplot as plt
from fdm import fdm

r = 100.
def f(x):
    return 0.
def v(x):
    return -r
mu1 = 0.
mu2 = 1.
a = 0.
b = 1.
nList = [10, 100]
sgList = ['-', '--', '-.', ':']
k = 0
for n in nList:
    x, y = fdm(v, f, mu1, mu2, a, b, n, 0)
    sl = 'n=' + str(n) + ' (upwind)'
    sg = sgList[k]
    k = k + 1
    plt.plot(x, y, sg, label = sl)

    x, y = fdm(v, f, mu1, mu2, a, b, n, 1)
    sl = 'n=' + str(n) + ' (central)'
    sg = sgList[k]
    k = k + 1
    plt.plot(x, y, sg, label = sl)
plt.xlabel('x')
plt.grid(True)
plt.legend(loc = 0)
plt.show()