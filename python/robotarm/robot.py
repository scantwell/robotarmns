from commands import Arm, Claw, Move, Rotate
from robot_serial import RobotSerial
from threading import Thread

class Robot(object):

    FORWARD = 1
    BACKWARD = 2
    LEFT = 3
    RIGHT = 4

    def __init__(self, device='/dev/tty.usbmodem1421'):
        self._connection = RobotSerial(device, baudrate=9600)
        self._isConnected = False;
        self._sender = None
        self._commands = []

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

    def move(self, direction, cm):
        '''Moves the robot in the given direction for the given centimeters.
        params direction: Direction in which to move. ie Robot.FORWARD
        params cm: Centimeters the robot should move.'''
        assert isinstance(direction, int)

        if not (direction == Robot.FORWARD or direction == Robot.BACKWARD):
            raise RuntimeError("Move direction is not recoginized.")
        self._addCommand(Move(direction, cm))

    def rotate(self, direction, degrees):
        '''Rotates the robot in the direction specified by the given degrees.
        params direction: Direction in which to start moving. ie Robot.LEFT is counter-clockwise'''
        assert isinstance(direction, int)

        if not (direction == Robot.LEFT or direction == Robot.RIGHT):
            raise RuntimeError("Turning direction is not recognized.")
        if degrees < 0 or degrees > 360:
            raise RuntimeError("Invalid degrees.")
        self._addCommand(Rotate(direction, degrees))

    def _addCommand(self, command):
        self._commands.append(command)

    def _send_robot(self):
        while True and self._isConnected:
            if len(self._commands) > 0:
                cmd = self._commands.pop(0).serialize()
                self._connection.sendCommand(cmd)