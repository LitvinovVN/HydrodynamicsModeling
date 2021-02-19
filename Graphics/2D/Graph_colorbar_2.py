import matplotlib.pyplot as plt
import numpy as np; np.random.seed(1)
from mpl_toolkits.axes_grid1.inset_locator import InsetPosition

fig, (ax, cax) = plt.subplots(ncols=2,figsize=(7,3), 
                  gridspec_kw={"width_ratios":[1,0.05]})
fig.subplots_adjust(wspace=0.3)
im  = ax.imshow(np.random.rand(11,16), vmin=0, vmax=1)
ax.set_ylabel("y label")


fig.colorbar(im, cax=cax, ax=[ax])

plt.show()