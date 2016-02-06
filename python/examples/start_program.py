import argparse
from robotarm import Robot
from time import sleep

def run(options):
	robot = Robot(options.tty)
	robot.connect()
	robot.arm_to(10)
	robot.claw_to(0)
	robot.rotate(Robot.RIGHT, 10)
	robot.rotate(Robot.LEFT, 10)
	robot.move(Robot.FORWARD, 5)
	robot.move(Robot.BACKWARD, 5)
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
