import matplotlib.pyplot as plt
import numpy as np
import matplotlib.tri as mtri
from celluloid import Camera
from scipy.spatial import voronoi_plot_2d

fig = plt.figure()

camera = Camera(fig)






for i in range(1, 9):
    coordinates = np.loadtxt("coordinates.txt")
    file_name = "./triangle_files/gathered_triangles_{}.txt".format(i)
    x_values = [x[0] for x in coordinates]
    y_values = [x[1] for x in coordinates]
    #print(x_values)
    #print(y_values)
    plt.plot(x_values, y_values, 'or')
    triangles = np.loadtxt(file_name)
    triang = mtri.Triangulation(coordinates[:,0], coordinates[:,1], triangles=triangles)
    plt.triplot(triang, marker = None, c = 'black')
    #plt.show()
    camera.snap()
    
ani = camera.animate()
ani.save('ani1.gif')



