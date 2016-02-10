from serial import Serial
from construct import Struct, ULInt8, ULInt16, CString, Flag, Padding
import struct
from time import sleep

_robotResponse = Struct("RobotResponse",
                        Flag("success", truth=0, falsehood=1, default=False),
                        #ULInt8("error")
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
        print "Sending command"
        num = sum(map(lambda x: int(struct.unpack('<B', x)[0]), buf))
        chksum = ULInt16("checksum").build(num)
        retry = True
        while retry:
            super(RobotSerial, self).write(buf)
            super(RobotSerial, self).write(chksum)
            super(RobotSerial, self).reset_input_buffer()
            response = _robotResponse.parse(super(RobotSerial, self).read())
            print "RESPONSE {}".format(response.success)
            if response.success:
                retry = False
                sleep(2) #Waiting for Arduino to clear its buffer
            #print "RETRYING"