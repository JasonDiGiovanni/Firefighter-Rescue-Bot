class point:
    
    """
    x: A float
    y: A float
    objType: A string
        Acceptable Values: Normal, Bump, Edge
    """
    def __init__(self, x: float, y: float):
        self.x = x
        self.y = y

point_CyBot = point(0, 0)
pointTable_normal = []
pointTable_bumps = []
pointTable_edges = []
max_x = 0.0
min_x = -0.0
max_y = 0.0
min_y = -0.0
angleToTurn = ""
distanceToMove = ""

user_command = "None"\

