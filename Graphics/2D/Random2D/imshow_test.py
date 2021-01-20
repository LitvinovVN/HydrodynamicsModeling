import numpy as np
import matplotlib.pyplot as plt

# Независимая (x) и зависимая (y) переменные
#x = np.linspace(0, 10, 50)
#y = x
# Построение графика
#plt.title('Линейная зависимость y = x') # заголовок
#plt.xlabel('x') # ось абсцисс
#plt.ylabel('y') # ось ординат
#plt.grid() # включение отображения сетки
#plt.plot(x, y) # построение графика
#plt.show()

np.random.seed(19680801)
data = np.random.randn(25, 25)
plt.imshow(data)
plt.savefig('imshow_test.eps', format='eps', dpi=600)
plt.savefig('imshow_test.svg', format='svg', dpi=1200)
plt.savefig('imshow_test.png', format='png', dpi=1200, transparent = True)
#plt.show()


import seaborn as sns
sns.set_theme()
uniform_data = np.random.rand(100, 100)
ax = sns.heatmap(uniform_data)
ax.figure.savefig('seaborn_heatmap.svg', format='svg', dpi=1200)


corr = np.corrcoef(np.random.randn(20, 200))
mask = np.zeros_like(corr)
mask[np.triu_indices_from(mask)] = True
with sns.axes_style("white"):
    f, ax = plt.subplots(figsize=(7, 5))
    ax = sns.heatmap(corr, mask=mask, vmax=.3, square=True)
    ax.figure.savefig('seaborn_heatmap2.svg', format='svg', dpi=1200)