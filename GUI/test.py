import threading
from graph import *
import globals as glb

glb.pointTable_normal.append(glb.point(1, 1))
glb.pointTable_normal.append(glb.point(0.75, 0.75))
glb.pointTable_normal.append(glb.point(0.6, 0.6))

glb.pointTable_bumps.append(glb.point(-2, 2))
glb.pointTable_edges.append(glb.point(3, -2))

graph_thread = threading.Thread(target=graph_init)
graph_thread.start()

sleep(3)

glb.pointTable_normal.append(glb.point(1.5, 0.75))
glb.point_CyBot = glb.point(-1, 1)


# updateGraph()