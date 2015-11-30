#!/usr/bin/python2.7

import pygame
import serial
import struct

tty = "/dev/ttyUSB0"
robot = serial.Serial(tty, 9600, timeout=5)
robot.open()

pygame.init()
pygame.key.set_repeat(100, 100)

# COMMANDS
PINCER = '0'
ARM = '1'
FORWARD = '2' 
BACKWARD = '3'
TURN_LEFT = '4'
TURN_RIGHT = '5'

UP = "u" #273
DOWN = "d" #274
RIGHT = "r" #275
LEFT = "l" #276

def run():

    while True:
#        events = pygame.event.get()
#        for event in events:
        input = raw_input() 
        print input
        if input == UP:
            robot.write(struct.pack('I', 2))
            robot.write(struct.pack('I', 2500))
            print 'KEY UP'
        elif input == DOWN:
            print 'KEY DOWN'
            robot.write(struct.pack('I', 3))
            robot.write(struct.pack('I', 2500))
        elif input == RIGHT:
            print 'KEY RIGHT'
            robot.write(struct.pack('I', 5))
            robot.write(struct.pack('I', 1000))
        elif input == LEFT:
            print 'KEY LEFT'
            robot.write(struct.pack('I', 4))
            robot.write(struct.pack('I', 1000))
        elif input == "open":
            robot.write(struct.pack('I', 0))
            robot.write(struct.pack('I', 0))
        elif input == "close":
            robot.write(struct.pack('I', 0))
            robot.write(struct.pack('I', 100))
        elif input == "up":
            robot.write(struct.pack('I', 1))
            robot.write(struct.pack('I', 5))
        elif input == "down":
            robot.write(struct.pack('I', 1))
            robot.write(struct.pack('I', 100))
        print robot.read()
if __name__ == '__main__':
    run()
