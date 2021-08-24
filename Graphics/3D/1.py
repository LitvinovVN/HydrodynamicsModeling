# Используемые ресурсы:
# https://matplotlib.org/mpl_toolkits/mplot3d/tutorial.html

import matplotlib as mpl
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import matplotlib.pyplot as plt

# Создаём новый рисуноки и сохраняем на него ссылку в переменной с именем fig
fig = plt.figure()
# Добавляем в рисунок fig новый фрагмент и сохраняем на него ссылку в переменной ax
ax = fig.add_subplot(111, projection='3d')

theta = np.linspace(-4 * np.pi, 4 * np.pi, 100)
z = np.linspace(-2, 2, 100)
r = z**2 + 1
x = r * np.sin(theta)
y = r * np.cos(theta)
ax.plot(x, y, z, label='parametric curve')


x1 = np.linspace(-4, 4, 100)
y1 = np.linspace(0, 0, 100)
z1 = np.linspace(0, 0, 100)
ax.plot(x1, y1, z1, label='x')

x2 = np.linspace(0, 0, 100)
y2 = np.linspace(-4, 4, 100)
z2 = np.linspace(0, 0, 100)
ax.plot(x2, y2, z2, label='y')

x3 = np.linspace(0, 0, 100)
y3 = np.linspace(0, 0, 100)
z3 = np.linspace(-2, 2, 100)
ax.plot(x3, y3, z3, label='z')

ax.legend()
plt.show()