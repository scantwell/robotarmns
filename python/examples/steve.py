#!/usr/bin/python2.7

import serial
import struct

tty = "/dev/tty.usbserial-A603GDO7"
robot = serial.Serial(tty, 9600, timeout=5)
robot.close()
robot.open()


# COMMANDS
UP = "up"
DOWN = "down"
RIGHT = "right"
LEFT = "left"
OPEN = "open"
CLOSE = "close"
RAISE = "raise"
LOWER = "lower"

def run():
    while True:
        input = raw_input()
        if input == UP:
            robot.write(struct.pack('I', 2))
            robot.write(struct.pack('I', 2500))
        elif input == DOWN:
            robot.write(struct.pack('I', 3))
            robot.write(struct.pack('I', 2500))
        elif input == RIGHT:
            robot.write(struct.pack('I', 5))
            robot.write(struct.pack('I', 1000))
        elif input == LEFT:
            robot.write(struct.pack('I', 4))
            robot.write(struct.pack('I', 1000))
        elif input == OPEN:
            robot.write(struct.pack('I', 0))
            robot.write(struct.pack('I', 0))
        elif input == CLOSE:
            robot.write(struct.pack('I', 0))
            robot.write(struct.pack('I', 100))
        elif input == RAISE:
            robot.write(struct.pack('I', 1))
            robot.write(struct.pack('I', 5))
        elif input == LOWER:
            robot.write(struct.pack('I', 1))
            robot.write(struct.pack('I', 100))
        print robot.read()

if __name__ == '__main__':
    run()
