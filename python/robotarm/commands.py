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

class Arm(object):
    def __init__(self, centimeters):
        assert isinstance(centimeters, int) or isinstance(centimeters, float), "Centimeters must be integer or float value."
        self.commandId = get_command_id("Arm")
        self.centimeters = centimeters
        self.length = 10

class Claw(object):
    def __init__(self, centimeters):
        assert isinstance(centimeters, int) or isinstance(centimeters, float), "Centimeters must be integer or float value."
        self.commandId = get_command_id("Claw")
        self.centimeters = centimeters
        self.length = 10

class Move(object):
    def __init__(self, direction, centimeters):
        assert isinstance(direction, int), "Direction must be integer value."
        assert isinstance(centimeters, int) or isinstance(centimeters, float), "Centimeters must be integer or float value."
        self.commandId = get_command_id("Move")
        self.direction = direction
        self.centimeters = centimeters
        self.length = 10

    def __str__(self):
        return 'CommandId: {}, Direction: {}, Centimeters: {}'.format(self.commandId, self.direction, self.centimeters)

# Should there be a direction or should it be dictated whether the number is negative
class Rotate(object):
    def __init__(self, direction, degrees):
        assert isinstance(direction, int), "Direction must be integer value."
        assert isinstance(degrees, int), "Degrees must be integer value."
        self.commandId = get_command_id("Rotate")
        self.direction = direction
        self.degrees = degrees
        self.length = 10




