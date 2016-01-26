#!/usr/bin/python2.7

import argparse
import serial
import struct
import sys
import platform

tty = '/dev/ttyUSB0'

if platform.system() == 'Darwin':
	tty = '/dev/ttys002'

# Commands
CLAW = 0
ARM = 1
UP = 2
DOWN = 3
LEFT = 4
RIGHT = 5


def main():
    args = parse_arguments()

    # noinspection PyBroadException
    robot = serial.Serial(tty, 9600, timeout=5)

    try:
        exit_code = 1
        cmd = args.command
        val = args.value

        robot.open()

        # Command Switch

        if cmd == 'forward':
            run_command(robot, UP, val)

        elif cmd == 'backward':
            run_command(robot, DOWN, val)

        elif cmd == 'left':
            run_command(robot, LEFT, val)

        elif cmd == 'right':
            run_command(robot, RIGHT, val)

        elif cmd == 'claw':
            run_command(robot, CLAW, val)

        elif cmd == 'arm':
            run_command(robot, ARM, val)
        else:
            exit_code = 2 //sys.exit(2)

    except serial.SerialException as e:
        print e
    finally:
        robot.close()
        sys.exit(exit_code)

def run_command(robot, command, value):
    robot.write(struct.pack('I', command))
    robot.write(struct.pack('I', value))
    return robot.read()


def parse_arguments():
    parser = argparse.ArgumentParser()
    parser.add_argument('command',
                        type=str,
                        help='Command to run')
    parser.add_argument('value',
                        type=int,
                        help='Value to run command with')
    return parser.parse_args()


if __name__ == "__main__":
    main()
