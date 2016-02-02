from serial import Serial
from construct import Struct, ULInt8, ULInt16
from commands import Arm, Claw, Move, Rotate

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
        if buf is not None:
            raise RuntimeError("Unrecognized command.")
        self.write(buf)