from commands import Arm, Claw, Move, Rotate
from robot_serial import RobotSerial
from threading import Thread
from numpy import array, cross, dot, linalg, resize
from math import acos, degrees

class Position(object):
    def __init__(self, at, direction):
        #assert isinstance(at, array) and isinstance(direction, array)
        self.at = at
        self.direction = direction


class Robot(object):

    FORWARD = 1
    BACKWARD = 2
    LEFT = 3
    RIGHT = 4
    _ARM_OFFSET = 3.5 # Height from the ground to the center point of the claw when arm_to(0)

    def __init__(self, device='/dev/tty.usbmodem1421'):
        self._connection = RobotSerial(device, baudrate=9600)
        self._isConnected = False
        self._sender = None
        self._commands = []
        self._position = Position(array([0, 0]), array([0, 1]))

    def arm_to(self, cm):
        '''Move the arm to the given centimeters above the ground.
        params cm: Centimeters from the ground to center of the claw.'''
        assert isinstance(cm, float) or isinstance(cm, int), "Invalid centimeters"
        self._addCommand(Arm(cm))

    def claw_to(self, cm):
        '''Sets the distance between the claw.
        params cm: Centimeters between the two claw arms.'''
        assert isinstance(cm, float) or isinstance(cm, int), "Invalid centimeters"
        self._addCommand(Claw(cm))

    def connect(self):
        self.disconnect()
        self._connection.open()
        self._isConnected = True
        self._sender = Thread(target=self._send_robot)
        self._sender.daemon = True
        self._sender.start()

    def disconnect(self):
        self._isConnected = False
        if self._sender is not None:
            self._sender.join()
        self._connection.close()

    def dropOff(self, pos):
        self.claw_to(0)
        self.arm_to(pos.item(2) + Robot._ARM_OFFSET)
        self.goto(resize(pos, (1, 2)))
        self.claw_to(5)

    def setPosition(self, pos, direction):
        #assert isinstance(vector, Vector)
        self._position.at = pos
        self._position.direction = self._normalize(direction)

    def goto(self, to):
        '''
        Moves to x y
        :param x:
        :param y:
        :return:
        '''
        #assert isinstance(to, array)
        v = self._normalize(to - self._position.at)
        mag = int(round(linalg.norm(to)))
        dir, deg = self._getRotation(v)
        print "Rotating {} degrees in {} direction.".format(deg, dir)
        #self.rotate(dir, deg)
        print "Moving forward {}".format(mag)
        self.move(Robot.FORWARD, mag)
        self._position.at = to
        self._position.direction = v

    def move(self, direction, cm):
        '''Moves the robot in the given direction for the given centimeters.
        params direction: Direction in which to move. ie Robot.FORWARD
        params cm: Centimeters the robot should move.'''
        assert isinstance(direction, int)
        assert isinstance(cm, int)

        if not (direction == Robot.FORWARD or direction == Robot.BACKWARD):
            raise RuntimeError("Move direction is not recoginized.")
        self._addCommand(Move(direction, cm))

    def pickUp(self, pos):
        self.arm_to(0)
        self.claw_to(5)
        self.goto(resize(pos, (1, 2)))
        self.claw_to(0)
        self.arm_to(pos.item(2))

    def rotate(self, direction, degrees):
        '''Rotates the robot in the direction specified by the given degrees.
        params direction: Direction in which to start moving. ie Robot.LEFT is counter-clockwise'''
        assert isinstance(direction, int) or isinstance()

        if not (direction == Robot.LEFT or direction == Robot.RIGHT):
            raise RuntimeError("Turning direction is not recognized.")
        if degrees < 0 or degrees > 360:
            raise RuntimeError("Invalid degrees.")
        self._addCommand(Rotate(direction, degrees))

    def _addCommand(self, command):
        self._commands.append(command)

    def _getRotation(self, vnorm):
        r = acos(dot(vnorm, self._position.direction))
        print "r {}".format(r)
        d = int(degrees(r))
        dir = Robot.RIGHT
        if cross(vnorm, self._position.direction) < 0:
            dir = Robot.LEFT
        return dir, d

    def _normalize(self, v):
        return v / linalg.norm(v)

    def _send_robot(self):
        while True and self._isConnected:
            if len(self._commands) > 0:
                cmd = self._commands.pop(0).serialize()
                self._connection.sendCommand(cmd)