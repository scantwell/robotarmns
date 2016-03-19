from commands import Arm, Claw, Move, Rotate
from robot_serial import RobotSerial
from threading import Thread
from numpy import array, cross, dot, linalg, ndarray, resize
from math import acos, degrees

class Position(object):
    ''' Represents the position and direction of the robot on the current coordinate system
    '''
    def __init__(self, at, direction):
        self.at = at
        self.direction = direction


class Robot(object):

    FORWARD = 1
    BACKWARD = 2
    LEFT = 3
    RIGHT = 4
    _ARM_OFFSET = 3.5  # Height from the ground to the center point of the claw when arm_to(0)
    _CLAW_OFFSET = 8  # Length from the center of the robot to center of claw
    _CLAW_MAX = 8
    _CLAW_MIN = 0

    def __init__(self, device=''):
        self._connection = RobotSerial(device, baudrate=9600)
        self._isConnected = False
        self._sender = None
        self._commands = []
        self._position = Position(array([0, 0]), array([0, 1]))
        self._hasDroppedItem = False  # Flag to indicate if the last command was a drop off

    def arm_to(self, cm):
        '''Move the arm to the given centimeters above the ground.
        params cm: Centimeters from the ground to center of the claw.'''
        assert isinstance(cm, float) or isinstance(cm, int), "Invalid centimeters"
        cm -= Robot._ARM_OFFSET
        if cm < 0:
            cm = 0
        self._addCommand(Arm(int(round(cm))))

    def claw_to(self, cm):
        '''Sets the distance between the claw.
        params cm: Centimeters between the two claw arms.'''
        assert isinstance(cm, float) or isinstance(cm, int), "Invalid centimeters"
        self._addCommand(Claw(cm))

    def connect(self):
        '''
        Connects to the robot.
        '''
        self.disconnect()
        self._connection.open()
        self._isConnected = True
        self._sender = Thread(target=self._send_robot)
        self._sender.daemon = True
        self._sender.start()

    def disconnect(self):
        '''
        Disconnect from the robot.
        '''
        self._isConnected = False
        if self._sender is not None:
            self._sender.join()
        self._connection.close()

    def dropOff(self, pos):
        '''
        Drops off item at the passed position.
        :param pos: A three dimensional numpy.array
        '''
        assert isinstance(pos, ndarray)
        self.claw_to(Robot._CLAW_MIN)
        self.arm_to(pos.item(2) + Robot._ARM_OFFSET)
        self.goto(resize(pos, (1, 2))[0], claw_movement=True)
        self.claw_to(Robot._CLAW_MAX)
        self._hasDroppedItem = True

    def goto(self, to, claw_movement=False):
        '''
        Moves to location specified by to
        :param to: A two dimensional numpy.array
        :param claw_movement: Flag which indicates if the robot is positioning for to pick up an item
        '''
        assert isinstance(to, ndarray)
        # If the robot just dropped something off we need to move out of the way before going anywhere
        if self._hasDroppedItem:
            self._clearObstacle()
        v = to - self._position.at
        # Robot is at location to move to
        if v.any() == 0:
            return
        mag = self._getMagnitude(v)
        v_dir = self._normalize(v)
        robot_dir, deg = self._getRotation(v_dir)
        if claw_movement:
            m = mag - self._CLAW_OFFSET
            to = to * (float(m)/mag)
            mag = m
        self._rotate(robot_dir, deg)
        self._move(Robot.FORWARD, mag)
        self._setPosition(to, v_dir)

    def _clearObstacle(self):
        mag = 10  # Degrees to back up to clear an obstacle.
        to = mag * -1 * self._position.direction
        pos = self._position.at + to
        self._move(Robot.BACKWARD, mag)
        self._setPosition(pos, self._position.direction)
        self._hasDroppedItem = False

    def _move(self, direction, cm):
        '''Moves the robot in the given direction for the given centimeters.
        params direction: Direction in which to move. ie Robot.FORWARD
        params cm: Centimeters the robot should move.'''
        assert isinstance(direction, int)
        assert isinstance(cm, int)
        if not (direction == Robot.FORWARD or direction == Robot.BACKWARD):
            raise RuntimeError("Move direction is not recognized.")
        self._addCommand(Move(direction, cm))

    def pickUp(self, pos):
        '''
        Pick up an item at position specified.
        :param pos: 3d numpy array
        '''
        assert isinstance(pos, ndarray)
        self.claw_to(Robot._CLAW_MAX)
        a = int(round(pos.item(2) - self._ARM_OFFSET))
        if a < 0:
            a = 0
        self.arm_to(a)
        self.goto(resize(pos, (1, 2))[0], claw_movement=True)
        self.claw_to(Robot._CLAW_MIN)

    def _rotate(self, direction, degrees):
        '''Rotates the robot in the direction specified by the given degrees.
        params direction: Direction in which to start moving. ie Robot.LEFT is counter-clockwise'''
        assert isinstance(direction, int)
        if not (direction == Robot.LEFT or direction == Robot.RIGHT):
            raise RuntimeError("Turning direction is not recognized.")
        if degrees < 0 or degrees > 360:
            raise RuntimeError("Invalid degrees.")
        if degrees == 0:
            return
        self._addCommand(Rotate(direction, degrees))

    def _addCommand(self, command):
        '''
        Adds a command to the queue to send to the robot.
        :param command: Command to add
        '''
        assert isinstance(command, (Arm, Claw, Move, Rotate))
        self._commands.append(command)

    def _getMagnitude(self, v):
        '''
        Get the magnitude of a vector
        :param v: ndarray
        :return: integer value
        '''
        return int(round(linalg.norm(v)))

    def _getRotation(self, vnorm):
        '''
        Gets the direction and degrees from a vnormal
        :param vnorm: ndarray normalized
        :return: tuple direction, degrees
        '''
        dprod = dot(vnorm, self._position.direction)
        r = acos(dprod)
        d = int(round(degrees(r)))
        dir = Robot.RIGHT
        if cross(vnorm, self._position.direction) < 0:
            dir = Robot.LEFT
        return dir, d

    def _normalize(self, v):
        return v / linalg.norm(v)

    def _setPosition(self, at, dir):
        '''
        Sets the position of the robot
        :param at: ndarray of the robot
        :param dir: normalized ndarray representing the direction of the robot
        '''
        self._position.at = at
        self._position.direction = dir

    def _send_robot(self):
        '''
        Thread run function to send commands to the robot.
        '''
        while True and self._isConnected:
            if len(self._commands) > 0:
                cmd = self._commands.pop(0).serialize()
                self._connection.sendCommand(cmd)
