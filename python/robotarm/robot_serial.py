from serial import Serial
from construct import Struct, ULInt8, ULInt16, PascalString
from commands import Arm, Claw, Move, Rotate
from time import sleep

_armProtocol = Struct("ArmProtocol",
    ULInt8("commandId"),
    #ULInt8("flags"),
    ULInt8("length"),
    ULInt16("centimeters"),
    #ULInt8("footer")
    )

_clawProtocol = Struct("ClawProtocol",
    ULInt8("commandId"),
    #ULInt8("flags"),
    ULInt8("length"),
    ULInt16("centimeters"),
    #ULInt8("footer")
    )

_moveProtocol = Struct("MoveProtocol",
    ULInt8("commandId"),
    #ULInt8("flags"),
    ULInt8("length"),
    ULInt8("direction"),
    ULInt16("centimeters"),
    #ULInt8("footer")
    )

_rotateProtocol = Struct("RotateProtocol",
    ULInt8("commandId"),
    #ULInt8("flags"),
    ULInt8("length"),
    ULInt8("direction"),
    ULInt16("degrees"),
    #ULInt8("footer")
    )

_robotResponse = Struct("RobotResponse",
                        PascalString("describe", length_field = ULInt8("length"))
                        )

class RobotSerial(Serial):
    def __init__(self,
                 port=None,
                 baudrate=9600,
                 bytesize=8,
                 parity='N',
                 stopbits=1,
                 timeout=None,
                 xonxoff=False,
                 rtscts=False,
                 write_timeout=None,
                 dsrdtr=False,
                 inter_byte_timeout=None,
                 **kwargs):
        super(RobotSerial, self).__init__(
            port=port,
            baudrate=baudrate,
            bytesize=bytesize,
            parity=parity,
            stopbits=stopbits,
            timeout=timeout,
            xonxoff=xonxoff,
            rtscts=rtscts,
            write_timeout=write_timeout,
            dsrdtr=dsrdtr,
            inter_byte_timeout=inter_byte_timeout,
            **kwargs)

    def sendCommand(self, command):
        buf = None
        if isinstance(command, Arm):
            buf = _armProtocol.build(command)
        elif isinstance(command, Claw):
            buf = _clawProtocol.build(command)
        elif isinstance(command, Move):
            buf = _moveProtocol.build(command)
        elif isinstance(command, Rotate):
            buf = _rotateProtocol.build(command)
        if buf is None:
            raise RuntimeError("Unrecognized command.")
        self.write(buf)
        self.readCommand()

    def readCommand(self):
        sleep(3)
        print "READING COMMAND"
        response = ""
        while super(RobotSerial, self).inWaiting() != 0:
            response += self.read()
        print response
