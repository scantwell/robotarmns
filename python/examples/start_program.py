import argparse
from robotarm.robot import Robot
from time import sleep

def run(options):
	robot = Robot(options.tty)
	sleep(2)
	robot.arm_to(10)
	sleep(2)
	robot.disconnect()

if __name__ == "__main__":
	parser = argparse.ArgumentParser(
	description='''
Short startup program for the robot.
Before running this program please allow space for the robot to move.''')
	parser.add_argument('-d', dest='tty', type=str, required=True,
help="TTY for the robot communicator.")
	options = parser.parse_args()
	run(options)
