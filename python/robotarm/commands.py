__all__ = [ "Arm",
            "Claw",
            "Move",
            "Rotate"]

_commandIds = {"Arm": 1,
              "Claw": 2,
              "Move": 3,
              "Rotate": 4}

def get_command_id(command_name):
    commandId = _commandIds.get(command_name, None)
    if commandId is None:
        raise RuntimeError("Invalid command.")
    return commandId

class _Command(object):
    def __init__(self, command_name):
        self.commandId = _commandIds.get(command_name, None)
        if self.commandId is None:
            raise RuntimeError("Invalid command.")

class Arm(object):
    def __init__(self, centimeters):
        assert isinstance(centimeters, int) or isinstance(centimeters, float), "Centimeters must be integer or float value."
        self.commandId = get_command_id("Arm")
        self.centimeters = centimeters
        self.length = 10

class Claw(_Command):
    def __init__(self, centimeters):
        assert isinstance(centimeters, int) or isinstance(centimeters, float), "Centimeters must be integer or float value."
        super(Claw, self).__init__("Claw")
        self.centimeters = centimeters

class Move(_Command):
    def __init__(self, direction, centimeters):
        assert isinstance(direction, int), "Direction must be integer value."
        assert isinstance(centimeters, int) or isinstance(centimeters, float), "Centimeters must be integer or float value."
        super(Move, self).__init__("Move")
        self.direction = direction
        self.centimeters = centimeters

# Should there be a direction or should it be dictated whether the number is negative
class Rotate(_Command):
    def __init__(self, direction, degrees):
        assert isinstance(direction, int), "Direction must be integer value."
        assert isinstance(degrees, int), "Degrees must be integer value."
        super(Rotate, self).__init__("Rotate")
        self.direction = direction
        self.degrees = degrees




