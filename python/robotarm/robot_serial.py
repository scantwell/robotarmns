from serial import Serial
from construct import Struct, ULInt8, ULInt16, CString, Flag
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
                        ULInt8("command"),
                        CString("describe"),
                        Flag("error"),
                        CString("error_message")
                        )

#_robotResponse = CString("describe")

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

    def write(self, obj):
        buf = obj
        if isinstance(obj, Arm):
            buf = _armProtocol.build(obj)
        elif isinstance(obj, Claw):
            buf = _clawProtocol.build(obj)
        elif isinstance(obj, Move):
            buf = _moveProtocol.build(obj)
        elif isinstance(obj, Rotate):
            buf = _rotateProtocol.build(obj)
        super(RobotSerial, self).write(buf)
        self.readCommand()

    def readCommand(self):
        sleep(3)
        response = ""
        while super(RobotSerial, self).inWaiting() != 0:
            response += self.read()
        if response != "":
            #print "RESPONSE:{}".format(response)
            print _robotResponse.parse(response)
