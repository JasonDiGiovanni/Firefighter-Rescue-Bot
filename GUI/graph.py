# Description: Display using a polar plot the distance measurements collected by a CyBot 180 degree 
# sensor scan at 4 degree increments from 0 to 180 degrees. Sensor data read from a text file.

# Original Code example modified by: Phillip Jones (10/02/2021), (05/15/2023)
# Original polar plot code example from matplot: https://matplotlib.org/stable/gallery/pie_and_polar_charts/polar_demo.html

# Useful matplotlib tutorial: https://matplotlib.org/stable/tutorials/introductory/pyplot.html
# Useful best practices Quick Start: https://matplotlib.org/stable/tutorials/introductory/quick_start.html
# General Python Reference/Tutorials: https://www.w3schools.com/python/ 

# Quick YouTube Overviews (See above links as primary resources for additional details): 
# - Quick Polar Plot (subplot) Overview: https://www.youtube.com/watch?v=pb-pZtvkGPM
# - Quick subplots Overview : https://www.youtube.com/watch?v=Tqph7_qMujk

#Import/Include useful math and plotting functions
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import os  # import function for finding absolute path to this python script
from time import sleep

import globals as glb

global firstCall
firstCall = "yes"

def rippedFunction():
    # Enter x and y coordinates of points and colors
    xs = [0, 2, -3, -1.5]
    ys = [0, 3, 1, -2.5]
    colors = ['m', 'g', 'r', 'b']

    # Select length of axes and the space between tick labels
    xmin, xmax, ymin, ymax = -5, 5, -5, 5
    ticks_frequency = 1

    # Plot points
    fig, ax = plt.subplots(figsize=(10, 10))
    ax.scatter(xs, ys, c=colors)

    # Draw lines connecting points to axes
    for x, y, c in zip(xs, ys, colors):
        ax.plot([x, x], [0, y], c=c, ls='--', lw=1.5, alpha=0.5)
        ax.plot([0, x], [y, y], c=c, ls='--', lw=1.5, alpha=0.5)

    # Set identical scales for both axes
    ax.set(xlim=(xmin-1, xmax+1), ylim=(ymin-1, ymax+1), aspect='equal')

    # Set bottom and left spines as x and y axes of coordinate system
    ax.spines['bottom'].set_position('zero')
    ax.spines['left'].set_position('zero')

    # Remove top and right spines
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)

    # Create 'x' and 'y' labels placed at the end of the axes
    ax.set_xlabel('x', size=14, labelpad=-24, x=1.03)
    ax.set_ylabel('y', size=14, labelpad=-21, y=1.02, rotation=0)

    # Create custom major ticks to determine position of tick labels
    x_ticks = np.arange(xmin, xmax+1, ticks_frequency)
    y_ticks = np.arange(ymin, ymax+1, ticks_frequency)
    ax.set_xticks(x_ticks[x_ticks != 0])
    ax.set_yticks(y_ticks[y_ticks != 0])

    # Create minor ticks placed at each integer to enable drawing of minor grid
    # lines: note that this has no effect in this example with ticks_frequency=1
    ax.set_xticks(np.arange(xmin, xmax+1), minor=True)
    ax.set_yticks(np.arange(ymin, ymax+1), minor=True)

    # Draw major and minor grid lines
    ax.grid(which='both', color='grey', linewidth=1, linestyle='-', alpha=0.2)

    # Draw arrows
    arrow_fmt = dict(markersize=4, color='black', clip_on=False)
    ax.plot((1), (0), marker='>', transform=ax.get_yaxis_transform(), **arrow_fmt)
    ax.plot((0), (1), marker='^', transform=ax.get_xaxis_transform(), **arrow_fmt)

    plt.show()

def graph_init():

    global fig
    global ax

    fig = plt.figure()
    ax = fig.add_subplot(111) # arguments here could be interesting

    updateGraph()

    global firstCall
    firstCall = "no"

    # fig.canvas.draw()
    # fig.canvas.flush_events()

    # wait loop that just keeps the graph from closing
    while glb.user_command != "Quit":
        plt.pause(.1)

    plt.close(fig)

def clearAndFormatGraph():
    
    ax.clear()
    
    ax.spines['bottom'].set_position('zero')
    ax.spines['left'].set_position('zero')

    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)

    # Find furthest edges of the graph to make a square, and add a little more than the radius of the cybot
    abs_dim = max(abs(glb.min_x), abs(glb.max_x), abs(glb.min_y), abs(glb.max_y)) + 20
    ax.set_xlim(left=-abs_dim, right=abs_dim)
    ax.set_ylim(bottom=-abs_dim, top=abs_dim)

    # How to customize this?
    ax.grid(which='both', color='grey', linewidth=1, linestyle='-', alpha=0.3)


def updateGraph():

    clearAndFormatGraph()

    # Radius 16cm circle; the cybot is 32cm in diameter
    CyBot_circle = patches.Circle(xy=[glb.point_CyBot.x, glb.point_CyBot.y], radius=16, color="black")
    ax.add_patch(p=CyBot_circle)

    # TODO Find a better way to update the graph than printing the entire list every time we update.
    i: glb.point
    for i in glb.pointTable_normal:
        ax.scatter(x=i.x, y=i.y, c="blue", s=4)
    
    for i in glb.pointTable_bumps:
        ax.scatter(x=i.x, y=i.y, c='#ff0096', s=4)

    for i in glb.pointTable_edges:
        ax.scatter(x=i.x, y=i.y, c="red", s=4)

    
    # ax.scatter(x=glb.point_CyBot.x, y=glb.point_CyBot.y, c="black", markersize=4000)
    
    # print(type(pts4))

    # TODO Can I remove this?
    try:
        fig.canvas.draw()
        fig.canvas.flush_events()
    except:
        print("Drawing Error!")




# def graphData(filename):
#     # A little python magic to make it more convient for you to ajust where you want the data file to live
#     # Link for more info: https://towardsthecloud.com/get-relative-path-python 
#     absolute_path = os.path.dirname(__file__) # Absoult path to this python script
#     relative_path = "./"   # Path to sensor data file relative to this python script (./ means data file is in the same directory as this python script
#     full_path = os.path.join(absolute_path, relative_path) # Full path to sensor data file
#     #filename = 'sensor-scan.txt' # Name of sensor data file

#     # angle_degrees: a vector (i.e., array of numbers) for which each element is an angle at which the sensor makes a distance measurement.
#     # Units: degrees
#     angle_degrees = [] # Initially empty

#     # angle_radians: a vector that contains converted elements of vector angle_degrees into radians 
#     # Units radians
#     angle_radians = [] # Initially empty

#     # distance: a vector, where each element is the corresponding distance measured at each angle in vector angle_degrees
#     # Units: meters
#     distanceIR = []      # Initially empty
#     distanceSON = []

#     # Open file containing CyBot sensor scan from 0 - 180 degrees
#     file_object = open(full_path + filename,'r') # Open the file: file_object is just a variable for the file "handler" returned by open()
#     file_header = file_object.readline() # Read and store the header row (i.e., 1st row) of the file into file_header
#     file_data = file_object.readlines()  # Read the rest of the lines of the file into file_data
#     file_object.close() # Important to close file one you are done with it!!

#     # For each line of the file split into columns, and assign each column to a variable
#     for line in file_data[0:-1]: 
#         data = line.split()    # Split line into columns (by default delineates columns by whitespace)
#         if (data == []):continue
#         print(data)
#         angle_degrees.append(float(data[0]))  # Column 0 holds the angle at which distance was measured
#         distanceIR.append(float(data[1]))       # Column 1 holds the distance that was measured at a given angle  
#         distanceSON.append(float(data[2]))
        
#     # Convert python sequence (list of strings) into a numpy array
#     angle_degrees = np.array(angle_degrees) # Avoid "TypeError: can't multiply sequence by non-int of type float"
#                                             # Link for more info: https://www.freecodecamp.org/news/typeerror-cant-multiply-sequence-by-non-int-of-type-float-solved-python-error/
                                            
#     angle_radians = (np.pi/180) * angle_degrees # Convert degrees into radians


#     print(angle_radians)
#     print(distanceIR)         
#     print(distanceSON)


#     # Create a polar plot
#     fig, ax = plt.subplots(subplot_kw={'projection': 'polar'}) # One subplot of type polar
#     ax.plot(angle_radians, distanceIR, color='r', linewidth=4.0)  # Plot distance verse angle (in radians), using red, line width 4.0
#     ax.plot(angle_radians, distanceSON, color='b', linewidth=4.0)  # Plot distance verse angle (in radians), using red, line width 4.0

#     ax.set_xlabel('Distance (m)', fontsize = 14.0)  # Label x axis
#     ax.set_ylabel('Angle (degrees)', fontsize = 14.0) # Label y axis
#     ax.xaxis.set_label_coords(0.5, 0.15) # Modify location of x axis label (Typically do not need or want this)
#     ax.tick_params(axis='both', which='major', labelsize=14) # set font size of tick labels
#     ax.set_rmax(100)                    # Saturate distance at 2.5 meters
#     ax.set_rticks([20, 40, 60, 80, 100])   # Set plot "distance" tick marks at .5, 1, 1.5, 2, and 2.5 meters
#     ax.set_rlabel_position(-22.5)     # Adjust location of the radial labels
#     ax.set_thetamax(180)              # Saturate angle to 180 degrees
#     ax.set_xticks(np.arange(0,np.pi+.1,np.pi/4)) # Set plot "angle" tick marks to pi/4 radians (i.e., displayed at 45 degree) increments
#                                                 # Note: added .1 to pi to go just beyond pi (i.e., 180 degrees) so that 180 degrees is displayed
#     ax.grid(True)                     # Show grid lines

#     # Create title for plot (font size = 14pt, y & pad controls title vertical location)
#     ax.set_title("Mock-up Polar Plot of CyBot Sensor Scan from 0 to 180 Degrees", size=14, y=1.0, pad=-24) 
#     plt.show()  # Display plot



#graphData('sensor-scan.txt')
#graphCartesian([1], [2], ['r'])