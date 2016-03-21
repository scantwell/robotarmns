import numpy
import time
from robotarm import Robot
r = Robot("/dev/ttyUSB0")

p = numpy.array([30,30,5])
d = numpy.array([87,9,22])
o = numpy.array([0,15])
r.connect()

i = raw_input()

r.pickUp(p)
r.dropOff(d)
r.goto(o)
r.claw_to(0)
r.arm_to(0)

time.sleep(90)

