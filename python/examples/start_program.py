import argparse
from robotarm import Robot
from time import sleep

def run(options):
	robot = Robot(options.tty)
	robot.connect()
	robot.claw_to(5)
	robot.move(Robot.FORWARD, 31)
	robot.claw_to(1)
	robot.arm_to(30)
	sleep(10)
	robot.rotate(Robot.RIGHT, 108)
	robot.move(Robot.FORWARD, 34)
	robot.arm_to(22)
	robot.claw_to(5)
	robot.move(Robot.BACKWARD, 20)
	robot.rotate(Robot.LEFT, 90)
	sleep(120)
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
