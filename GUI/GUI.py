# Author: Phillip Jones, highly altered by Simon Weydert
# Date: 10/30/2023
# Description: Client starter code that combines: 1) Simple GUI, 2) creation of a thread for
#              running the Client socket in parallel with the GUI, and 3) Simple recieven of mock sensor 
#              data for a server/cybot.for collecting data from the cybot.

# General Python tutorials (W3schools):  https://www.w3schools.com/python/

# Serial library:  https://pyserial.readthedocs.io/en/latest/shortintro.html 
import serial

# Time library
import time  

# Socket library:  https://realpython.com/python-sockets/  
# See: Background, Socket API Overview, and TCP Sockets  
import socket

# Tkinter GUI library
import tkinter as tk

# Thread library: https://www.geeksforgeeks.org/how-to-use-thread-in-tkinter-python/
import threading

# import function for finding absolute path to this python script
import os  

import numpy as np
import matplotlib.pyplot as plt

import globals as glb
from graph import *

HEADER_HELLO = "h"
HEADER_QUIT = "_q_"
HEADER_SCAN = "_s_"
HEADER_ROTATE = "_r_"
HEADER_MOVE = "_m_"
HEADER_MUSIC = "_p_"
HEADER_SPRAY = "_a_"

HEADER_POINT = "_P_"
HEADER_IR = "_I_"

END_STRING = "_"



##### START Define Functions  #########

def GUI_init():
    
    """
    Create a global tkinter GUI window. 
    It's global so that other functions outside of main can alter it,
    like the quit function. 
    """
    global window  
    window = tk.Tk()  



    # Last command label
    # Made global so that commandThread can modify
    global Last_command_Label  
    Last_command_Label = tk.Label(text="Last Command Sent: ")  # Create a Label
    Last_command_Label.pack()  # Pack the label into the window for display

    # Quit command Button
    quit_command_Button = tk.Button(text="Press to Quit", command=send_quit)
    quit_command_Button.pack()  # Pack the button into the window for display

    # Cybot Scan command Button
    scan_command_Button = tk.Button(text="Press to Scan", command=send_scan)
    scan_command_Button.pack()  # Pack the button into the window for display

    # Creates the angle button
    angle_Entry = tk.Entry(master=window)
    angle_Button = tk.Button(text="Turn to Angle", command=lambda: send_angle(angle_Entry))
    angle_Entry.pack()
    angle_Button.pack()

    # Creates the move button
    distance_Entry = tk.Entry(master=window)
    distance_Button = tk.Button(text="Move Centimeters", command=lambda: send_distance(distance_Entry))
    distance_Entry.pack()
    distance_Button.pack()

    # Creates the move button
    music_Button = tk.Button(text="Play Song", command=send_music)
    music_Button.pack()

    # Create the spray button
    spray_Button = tk.Button(text="Spray", command=send_spray)
    spray_Button.pack()

    # Create the reconnect button
    reconnect_Button = tk.Button(text="Reconnect", command=cybot_reconnect)
    reconnect_Button.pack()

    # TODO Make renegotiation button

    init_txt()
    cybot_connect()

    # Creates a Thread runs a fuction the function passed as "target"
    # Two threads are made: one for the user-command interface, and one for the graph
    GUI_thread = threading.Thread(target=command_thread)  
    graph_thread = threading.Thread(target=graph_init)
    read_thread = threading.Thread(target=scanForCharacters)


    # while True:
    #     i = 1

    GUI_thread.start() # Start the thread
    graph_thread.start()
    read_thread.start()

    print("Hello jamaica!")

    # Start event loop so the GUI can detect events such as button clicks, key presses, etc.
    window.mainloop()

    # DO NOT REMOVE, THIS STOPS CRASHES (Tf2 coconut moment IHU)
    # (This works by keeping the main loop alive)
    while glb.user_command != "Quit":
        i = 0
    

def cybot_reconnect():
    cybot_closeConnection()
    sleep(3)
    cybot_connect()

def init_txt():
    absolute_path = os.path.dirname(__file__) # Absoult path to this python script
    relative_path = "./"   # Path to sensor data file relative to this python script (./ means data file is in the same directory as this python script)
    full_path = os.path.join(absolute_path, relative_path) # Full path to sensor data file

    file_object = open(full_path + 'normalPoints.txt','r') # Open the file: file_object is just a variable for the file "handler" returned by open()
    while(file_object.readable()):
        read_message = file_object.readline()
        read_fields = read_message.split('\t')
        if(len(read_fields) >= 2):
            read_x = float(read_fields[0])
            read_y = float(read_fields[1])
            glb.pointTable_normal.append(glb.point(x=read_x, y=read_y))
            updateMaxes(read_x, read_y)
        else:
            break
    file_object.close()

    file_object = open(full_path + 'bumpPoints.txt','r') # Open the file: file_object is just a variable for the file "handler" returned by open()
    while(file_object.readable()):
        read_message = file_object.readline()
        read_fields = read_message.split('\t')
        if(len(read_fields) >= 2):
            read_x = float(read_fields[0])
            read_y = float(read_fields[1])
            glb.pointTable_bumps.append(glb.point(x=read_x, y=read_y))
            updateMaxes(read_x, read_y)
        else:
            break
    file_object.close()

    file_object = open(full_path + 'edgePoints.txt','r') # Open the file: file_object is just a variable for the file "handler" returned by open()
    while(file_object.readable()):
        read_message = file_object.readline()
        read_fields = read_message.split('\t')
        if(len(read_fields) >= 2):
            read_x = float(read_fields[0])
            read_y = float(read_fields[1])
            glb.pointTable_edges.append(glb.point(x=read_x, y=read_y))
            updateMaxes(read_x, read_y)
        else:
            break 
    file_object.close()

    file_object = open(full_path + 'cyBotPoint.txt','r') # Open the file: file_object is just a variable for the file "handler" returned by open()
    if(file_object.readable()):
        read_message = file_object.readline()
        read_fields = read_message.split('\t')
        if(len(read_fields) >= 2):
            read_x = float(read_fields[0])
            read_y = float(read_fields[1])
            glb.point_CyBot = glb.point(x=read_x, y=read_y)
            updateMaxes(read_x, read_y)
    file_object.close() # Important to close file once you are done with it!!                


def updateMaxes(x: float, y: float):
    if(x > glb.max_x):
        glb.max_x = x

    if(x < glb.min_x):
        glb.min_x = x
            
    if(y > glb.max_y):
        glb.max_y = y

    if(y < glb.min_y):
        glb.min_y = y


def scanForCharacters():
    global cybot
    global rx_byte
    rx_byte = bytearray(1)

    absolute_path = os.path.dirname(__file__) # Absoult path to this python script
    relative_path = "./"   # Path to sensor data file relative to this python script (./ means data file is in the same directory as this python script)
    full_path = os.path.join(absolute_path, relative_path) # Full path to sensor data file

    file_normalPoints = open(full_path + 'normalPoints.txt','a') # Open the file: file_object is just a variable for the file "handler" returned by open()
    file_bumpPoints = open(full_path + 'bumpPoints.txt','a') # Open the file: file_object is just a variable for the file "handler" returned by open()
    file_edgePoints = open(full_path + 'edgePoints.txt','a') # Open the file: file_object is just a variable for the file "handler" returned by open()
    file_cyBotPoint = open(full_path + 'cyBotPoint.txt','w') # Open the file: file_object is just a variable for the file "handler" returned by open()



    while glb.user_command != "Quit":
        

        """
        Scan for a packet header. 
        If you find an underscore, read a header. 
        """
        try:
            rx_byte = cybot.read(1)
        except:
            print("Can't read!")
            if glb.user_command == "Quit":
                break
        print(rx_byte.decode())
        header_message = ""
        if rx_byte.decode() == "_":

            header_message += rx_byte.decode()
            rx_byte = cybot.read(1)
            while(rx_byte.decode() != "_"):
                header_message += rx_byte.decode()
                rx_byte = cybot.read(1)
            header_message += rx_byte.decode()
            print("Header recieved: " + header_message)

        if header_message == HEADER_POINT:

            print("Getting point data from Cybot:\n")

            # Create or overwrite existing sensor scan data file
            # Open the file: file_object is just a variable for the file "handler" returned by open()
            rx_message = ""

            rx_byte = cybot.read(1)
            while rx_byte.decode() != "_":  # Collect sensor data until "_" recieved

                rx_message += rx_byte.decode()
                rx_byte = cybot.read(1)  # Wait for sensor response, readline expects message to end with "\n"

            print(rx_message)  # Convert message from bytes to String (i.e., decode), then print to the console for debugging purposes

            rx_segments = rx_message.split('\t')
            rx_x = float(rx_segments[0])
            rx_y = float(rx_segments[1])
            rx_type = rx_segments[2].strip()

            new_point = glb.point(rx_x, rx_y)

            
            
            if(rx_type == "obstacle"):
                glb.pointTable_normal.append(new_point)
                write_messsage = str(new_point.x) + '\t' + str(new_point.y) + '\n'
                file_normalPoints.writelines(write_messsage)
                file_normalPoints.flush()
            
            if(rx_type == "wall"):
                glb.pointTable_bumps.append(new_point)
                write_messsage = str(new_point.x) + '\t' + str(new_point.y) + '\n'
                file_bumpPoints.writelines(write_messsage)
                file_bumpPoints.flush()
            
            if(rx_type == "cliff"):
                glb.pointTable_edges.append(new_point)
                write_messsage = str(new_point.x) + '\t' + str(new_point.y) + '\n'
                file_edgePoints.writelines(write_messsage)
                file_edgePoints.flush()
            
            if(rx_type == "bot"):
                glb.point_CyBot = glb.point(rx_x, rx_y)
                write_messsage = str(new_point.x) + '\t' + str(new_point.y) + '\n'
                file_cyBotPoint.writelines(write_messsage)
                file_cyBotPoint.flush()
            
            

            updateMaxes(rx_x, rx_y)
            

            updateGraph()
        
        # END POINT HEADDER #
        
        if header_message == HEADER_IR:

            rx_message = ""
            
            rx_byte = cybot.read(1)
            while rx_byte.decode() != "_":

                rx_message += rx_byte.decode()
                rx_byte = cybot.read(1)
            
            print(rx_message)
        
        # END IR HEADER #
    
    # END WHILE #
    
    file_normalPoints.close() # Important to close file once you are done with it!!
    file_bumpPoints.close()
    file_edgePoints.close()
    file_cyBotPoint.close()

# END FUNCITON #

def send_music():
    glb.user_command = "Music"

def send_spray():
    glb.user_command = "Spray"


def send_distance(number_Entry: tk.Entry):
    glb.distanceToMove = number_Entry.get()
    glb.user_command = "Move"

    print(glb.distanceToMove)
    
def send_angle(number_Entry: tk.Entry):
    glb.angleToTurn = number_Entry.get()
    glb.user_command = "Turn"

    print(glb.angleToTurn)
    

# Quit Button action.  Tells the client to send a Quit request to the Cybot, and exit the GUI
def send_quit():
    global window  # Main GUI window

    glb.user_command = "Quit"
    time.sleep(1)
    window.destroy()  # Exit GUI


# Scan Button action.  Tells the client to send a scan request to the Cybot
def send_scan():
    glb.user_command = "Scan"  # Update the message for the Client to send


def send_move():
    glb.user_command = "Move"

def cybot_connect():
    # Define Globals
    global cybot
    global cybot_socket

    # A little python magic to make it more convient for you to adjust where you want the data file to live
    # Link for more info: https://towardsthecloud.com/get-relative-path-python
    absolute_path = os.path.dirname(__file__)  # Absoult path to this python script
    relative_path = "./"  # Path to sensor data file relative to this python script (./ means data file is in the same directory as this python script)
    full_path = os.path.join(absolute_path, relative_path)  # Full path to sensor data file
    filename = 'sensor-scan.txt'  # Name of file you want to store sensor data from your sensor scan command

    # Choose to create either a UART or TCP port socket to communicate with Cybot (Not both!!)
    # UART BEGIN
    # cybot = serial.Serial('COM100', 115200)  # UART (Make sure you are using the correct COM port and Baud rate!!)
    # UART END

    # TCP Socket BEGIN (See Echo Client example): https://realpython.com/python-sockets/#echo-client-and-server
    HOST = "192.168.1.1"  # The server's hostname or IP address
    PORT = 288  # The port used by the server
    cybot_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # Create a socket object
    cybot_socket.connect((HOST, PORT))  # Connect to the socket  (Note: Server must first be running)

    cybot = cybot_socket.makefile("rbw", buffering=0)  # makefile creates a file object out of a socket:  https://pythontic.com/modules/socket/makefile
    # TCP Socket END

    # Send some text: Either 1) Choose "Hello" or 2) have the user enter text to send
    send_packet = HEADER_HELLO  # 1) Hard code message to "Hello", or
    # send_message = input("Enter a message:") + '\n'   # 2) Have user enter text
    cybot.write(send_packet.encode())  # Convert String to bytes (i.e., encode), and send data to the server

    print("Sent to server: " + send_packet) 

def cybot_closeConnection():
    print("Client exiting, and closing file descriptor, and/or network socket\n")
    time.sleep(2)  # Sleep for 2 seconds
    cybot.close()  # Close file object associated with the socket or UART
    cybot_socket.close()  # Close the socket (NOTE: comment out if using UART interface, only use for network socket option)

def command_thread():
    
    # global Last_command_Label  # GUI label for displaying the last command sent to the Cybot
    
    # Send messages to server until user sends "quit"
    while glb.user_command != "Quit":

        # # Update the GUI to display command being sent to the CyBot
        # if glb.user_command != "None":
        #     command_display = "Last Command Sent:\t" + glb.user_command
        #     Last_command_Label.config(text=command_display)

        if glb.user_command == "Scan":
            send_message = HEADER_SCAN + END_STRING
            cybot.write(send_message.encode()) 
            print("Sent to cybot: " + send_message)
            glb.user_command = "None"

        elif glb.user_command == "Turn" and glb.angleToTurn != "":
            send_message = HEADER_ROTATE + glb.angleToTurn + END_STRING
            cybot.write(send_message.encode())
            print("Sent to cybot: " + send_message)  
            glb.user_command = "None"

        elif glb.user_command == "Move" and glb.distanceToMove != "":
            send_message = HEADER_MOVE + glb.distanceToMove + END_STRING
            cybot.write(send_message.encode())
            print("Sent to cybot: " + send_message)  
            glb.user_command = "None"

        elif glb.user_command == "Music":
            send_message = HEADER_MUSIC + END_STRING
            cybot.write(send_message.encode())
            print("Sent to cybot: " + send_message)  
            glb.user_command = "None"

        elif glb.user_command == "Spray":
            send_message = HEADER_SPRAY + END_STRING
            cybot.write(send_message.encode())
            print("Sent to cybot: " + send_message)  
            glb.user_command = "None"
        
        time.sleep(0.1)

    ### END LARGEST WHILE ###

    if glb.user_command == "Quit":
        send_message = HEADER_QUIT + END_STRING
        cybot.write(send_message.encode())
        print("Sent to cybot: " + send_message)  

    cybot_closeConnection()

# Main: Mostly used for setting up, and starting the GUI
def main():

    GUI_init()
    

### Run main ###
main()
