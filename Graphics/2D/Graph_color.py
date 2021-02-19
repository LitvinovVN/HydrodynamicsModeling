import matplotlib.pyplot as plt

clr = ['mediumorchid', 'blueviolet', 'navy', 'royalblue', 'darkslategrey', 'limegreen', 'darkgreen', 'yellow', 'darkorange', 'orangered']

data_name = ['А', 'Б', 'В', 'Г', 'Д', 'Е', 'Ж', 'З', 'И', 'К']
data_values = [10, 12, 14, 16, 21, 22, 25, 15, 7, 9]
for tt in range(len(data_name)):
    plt.barh(data_name[tt], data_values[tt],
             height = 0.2, color = clr[tt], alpha = 0.7, label = str(data_name[tt])+" - "+str(data_values[tt])+' %',
             zorder = 2)
plt.show()
#plt.yticks(xs, data[0:10], rotation = 0)