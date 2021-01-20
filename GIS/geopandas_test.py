# https://medium.com/nuances-of-programming/овладей-python-создавая-реальные-приложения-часть-2-ba6ae6d0456b
# https://python-visualization.github.io/folium/modules.html

import pandas as pd
import requests
from xml.etree import ElementTree
import numpy as np
import folium

ftiles = ["Stamen Toner", "Stamen Terrain", "Stamen Watercolor", "OpenStreetMap", "CartoDB positron", "CartoDB dark_matter"]
map = folium.Map(location=[47.06, 39.2], zoom_start=12, tiles=ftiles[5])

#Add Marker
folium.Marker(location=[47.055, 39.25], popup = "Coordinates: 47.055, 39.25", icon=folium.Icon(color = 'gray')).add_to(map)

#Multiple Markers
for coordinates in [[47.050, 39.2],[47.045, 39.2]]:
    folium.Marker(location=coordinates, icon=folium.Icon(color = 'green')).add_to(map)


folium.Circle(
    radius=100,
    location=[47.055, 39.28],
    popup="The Waterfront",
    color="crimson",
    fill=False,
).add_to(map)

folium.CircleMarker(
    location=[47.0, 39.1],
    radius=50,
    popup="Laurelhurst Park",
    color="#3186cc",
    fill=True,
    fill_color="#3186cc",
).add_to(map)

folium.PolyLine([[47.03, 39.01], [47.06, 39.01], [47.06, 39.1]]).add_to(map)

folium.Rectangle([[47.0, 39.0], [47.1, 39.3]]).add_to(map)

# Fit the map to contain a bounding box with the maximum zoom level possible.
map.fit_bounds([[47.0, 39.0], [47.1, 39.3]])

#for point in range(0, len(locationlist)):
#    folium.Marker(locationlist[point], popup=df_counters['Name'][point]).add_to(map)
map.save("1111.htm")



#import io
#from PIL import Image

#img_data = map._to_png(10)
#img = Image.open(io.BytesIO(img_data))
#img.save('image.png')

#!pip install pyscreenshot
#import pyscreenshot as ImageGrab

#im=ImageGrab.grab(bbox=(157,200,1154,800)) # X1,Y1,X2,Y2
#To grab the whole screen, omit the bbox parameter
 
#im.show()
#im.save('screenAreGrab.png',format='png')