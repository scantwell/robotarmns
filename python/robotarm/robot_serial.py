from serial import Serial
from construct import Struct, ULInt8, ULInt16, CString, Flag, Padding
import struct
from time import sleep

_robotResponse = Struct("RobotResponse",
                        Flag("success", truth=0, falsehood=1, default=False)
                        )

class RobotSerial(Serial):
    '''
    Serial connection to the robot that allows for sending and receiving commands.
    '''
    def __init__(self,
                 port=None,
                 baudrate=9600,
                 bytesize=8,
                 parity='N',
                 stopbits=1,
                 timeout=None,
                 xonxoff=False,
                 rtscts=False,
                 dsrdtr=False,
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
            dsrdtr=dsrdtr,
            **kwargs)

    def open(self):
        '''
        Opens a serial connection with the robot
        '''
        super(RobotSerial, self).open()
        sleep(3)
        super(RobotSerial, self).flushOutput()
        super(RobotSerial, self).flushInput()

    def sendCommand(self, buf):
        '''
        Sends a command to the robot
        :param buf: serialized robot command
        '''
        num = sum(map(lambda x: int(struct.unpack('<B', x)[0]), buf))
        chksum = ULInt16("checksum").build(num)
        retry = True
        while retry:
            super(RobotSerial, self).write(buf)
            super(RobotSerial, self).write(chksum)
            super(RobotSerial, self).flushInput()
            response = _robotResponse.parse(super(RobotSerial, self).read())
            if response.success:
                retry = False
                sleep(2)