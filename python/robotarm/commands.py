from construct import Struct, ULInt8, ULInt16, CString, Flag, Padding

__all__ = [ "Arm",
            "Claw",
            "Move",
            "Rotate"]

_commandIds = {"Arm": 1,
              "Claw": 2,
              "Move": 3,
              "Rotate": 4}

_armProtocol = Struct("ArmProtocol",
    ULInt8("commandId"),
    Padding(1), #Padding zeros for 8 bits
    ULInt16("centimeters"),
    )

_clawProtocol = Struct("ClawProtocol",
    ULInt8("commandId"),
    Padding(1), #Padding zeros for 8 bits
    ULInt16("centimeters"),
    )

_moveProtocol = Struct("MoveProtocol",
    ULInt8("commandId"),
    ULInt8("direction"),
    ULInt16("centimeters"),
    )

_rotateProtocol = Struct("RotateProtocol",
    ULInt8("commandId"),
    ULInt8("direction"),
    ULInt16("degrees"),
    )

def get_command_id(command_name):
    commandId = _commandIds.get(command_name, None)
    if commandId is None:
        raise RuntimeError("Invalid command.")
    return commandId

class Command(object):
    '''
    Base class for commands
    '''
    def serialize(self):
        return self._protocol.build(self)

    def deserialize(self, buf):
        return self._protocol.parse(buf)

class Arm(Command):
    '''
    Robot Arm Command
    '''
    def __init__(self, centimeters):
        assert isinstance(centimeters, int) or isinstance(centimeters, int), "Centimeters must be integer or float value."
        self._protocol = _armProtocol
        self.commandId = get_command_id("Arm")
        self.centimeters = centimeters

class Claw(Command):
    '''
    Robot Claw Command
    '''
    def __init__(self, centimeters):
        assert isinstance(centimeters, int) or isinstance(centimeters, int), "Centimeters must be integer or float value."
        self._protocol = _clawProtocol
        self.commandId = get_command_id("Claw")
        self.centimeters = centimeters

class Move(Command):
    '''
    Robot Move Command
    '''
    def __init__(self, direction, centimeters):
        assert isinstance(direction, int), "Direction must be integer value."
        assert isinstance(centimeters, int) or isinstance(centimeters, int), "Centimeters must be integer or float value."
        self._protocol = _moveProtocol
        self.commandId = get_command_id("Move")
        self.direction = direction
        self.centimeters = centimeters

    def __str__(self):
        return 'CommandId: {}, Direction: {}, Centimeters: {}'.format(self.commandId, self.direction, self.centimeters)

# Should there be a direction or should it be dictated whether the number is negative
class Rotate(Command):
    '''
    Robot Rotate Command
    '''
    def __init__(self, direction, degrees):
        assert isinstance(direction, int), "Direction must be integer value."
        assert isinstance(degrees, int), "Degrees must be integer value."
        self._protocol = _rotateProtocol
        self.commandId = get_command_id("Rotate")
        self.direction = direction
        self.degrees = degrees



