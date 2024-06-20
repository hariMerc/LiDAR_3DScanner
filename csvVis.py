import csv
import matplotlib.pyplot as plt
import numpy as np

custom_cmap = plt.get_cmap('hsv')
fig = plt.figure()
ax = plt.axes(projection='3d')

with open('Scan20240613_1438.csv', 'r') as csvfile:
    plots = csv.DictReader(csvfile)

    x = []
    y = []
    z = []
    for row in plots:
        x.append(float(row['x_val']))
        y.append(float(row['y_val']))
        z.append(float(row['z_val']))

# Convert lists to NumPy arrays
x = np.array(x)
y = np.array(y)
z = np.array(z)

c = np.sqrt(x**2 + y**2 + z**2) 

scatter = ax.scatter3D(x, y, z, s=2, c=z, cmap=custom_cmap, alpha=1, marker='.')
# Set background color
fig.patch.set_facecolor('#121212')
ax.set_facecolor('#121212')
fig.colorbar(scatter, ax = ax, shrink = 0.75, aspect = 12)
plt.axis('off')
plt.show()
