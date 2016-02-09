from serial import Serial
from construct import Struct, ULInt8, ULInt16, CString, Flag, Padding
import struct
from time import sleep

_robotResponse = Struct("RobotResponse",
                        Flag("error", truth=0, falsehood=1, default=False),
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

    def open(self):
        super(RobotSerial, self).open()
        sleep(3)
        super(RobotSerial, self).flushOutput()
        super(RobotSerial, self).flushInput()

    def sendCommand(self, buf):
        super(RobotSerial, self).flushOutput()
        super(RobotSerial, self).flushInput()
        chksum = ULInt16("checksum").parse((sum(map(lambda x: int(struct.unpack('<B', x)[0]), buf))))
        retry = True
        while retry:
            super(RobotSerial, self).write(buf)
            super(RobotSerial, self).write(chksum)
            response = _robotResponse.parse(super(RobotSerial, self).read())
            if not response.error:
                retry = False