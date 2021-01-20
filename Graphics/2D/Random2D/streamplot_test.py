# https://pyprog.pro/mpl/mpl_streamplot.html
# https://pyprog.pro/mpl/mpl_quiver.html

import numpy as np
import matplotlib.pyplot as plt

fig, ax = plt.subplots()

Y, X = np.mgrid[-2*np.pi:2*np.pi:20j,
                -2*np.pi:2*np.pi:20j]
U = np.sin(X) + np.cos(Y)
V = np.cos(X) + np.cos(Y)

#  Строим поток векторов:
#ax.streamplot(X, Y, U, V, 
#              color = 'b')

X, Y = np.mgrid[-2*np.pi:2*np.pi:25j,
                -2*np.pi:2*np.pi:25j]
U = np.sin(X) + np.cos(Y)
V = np.cos(X) + np.cos(Y)

#  Поверх потока строим векторное поле:
ax.quiver(X, Y, U, V, 
          color = '#2011af60')    #  RGBA

fig.set_figwidth(14)    #  ширина и
fig.set_figheight(14)    #  высота "Figure"

plt.show()