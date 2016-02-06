from commands import Arm, Claw, Move, Rotate
from robot_serial import RobotSerial
import threading

class Robot(object):

    FORWARD = 1
    BACKWARD = 2
    LEFT = 3
    RIGHT = 4

    def __init__(self, device='/dev/tty.usbmodem1421'):
        self._connection = RobotSerial(device, baudrate=9600, timeout=15)
        self._isConnected = False;
        self._reader = None

    def arm_to(self, cm):
        '''Move the arm to the given centimeters above the ground.
        params cm: Centimeters from the ground to center of the claw.'''
        assert isinstance(cm, float) or isinstance(cm, int), "Invalid centimeters"
        command = Arm(cm)
        self._connection.write(command)

    def claw_to(self, cm):
        '''Sets the distance between the claw.
        params cm: Centimeters between the two claw arms.'''
        assert isinstance(cm, float) or isinstance(cm, int), "Invalid centimeters"
        command = Claw(cm)
        self._connection.write(command)

    def connect(self):
        self.disconnect()
        self._connection.open()
        self._isConnected = True
        self._reader = None #threading.Thread(target=self._read_from_robot,)
        #self._reader.start()

    def disconnect(self):
        self._isConnected = False
        if self._reader is not None:
            self._reader.join()
        self._connection.close()

    def move(self, direction, cm):
        '''Moves the robot in the given direction for the given centimeters.
        params direction: Direction in which to move. ie Robot.FORWARD
        params cm: Centimeters the robot should move.'''
        assert isinstance(direction, int)

        if not (direction == Robot.FORWARD or direction == Robot.BACKWARD):
            raise RuntimeError("Move direction is not recoginized.")
        command = Move(direction, cm)
        self._connection.write(command)

    def rotate(self, direction, degrees):
        '''Rotates the robot in the direction specified by the given degrees.
        params direction: Direction in which to start moving. ie Robot.LEFT is counter-clockwise'''
        assert isinstance(direction, int)

        if not (direction == Robot.LEFT or direction == Robot.RIGHT):
            raise RuntimeError("Turning direction is not recognized.")
        if degrees < 0 or degrees > 360:
            raise RuntimeError("Invalid degrees.")

        command = Rotate(direction, degrees)
        self._connection.write(command)

    def read_from_robot(self):
        msg = self._connection.readCommand()
        while msg is not None:
            print msg
            msg = self._connection.readCommand()

    def _read_from_robot(self):
        while True:
            if self._isConnected:
                self._connection.readCommand()
                #if msg is not None:
                #    print msg
            else:
                break