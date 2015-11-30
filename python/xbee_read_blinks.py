#! /usr/bin/python

import serial, time

tty = "/dev/ttyUSB0"

def main():
    serialStream = serial.Serial(tty, 9600, timeout=5)
    serialStream.open()
    serialStream.write('h')
    doread = True
    while(True):
        if doread:
            print readMessage(serialStream)
        
        text = raw_input('input:')

        if text is 'q':
            serialStream.close()
            break
    
        serialStream.write(text)
        
        if isInt(text):
            doread = False
            time.sleep( int(text) + 3)

        if not isInt(text):
            doread = True




def isInt(num):
    try:
        int(num)
        return True
    except ValueError:
        return False

def readMessage(serialStream):
    output = ""
    while not output.endswith('\r'):
        bytesToRead = serialStream.inWaiting()
        output += serialStream.read(bytesToRead)
    serialStream.flushOutput()
    return output

if __name__ == '__main__':
	main()
