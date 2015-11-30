#! /usr/bin/python

import serial, time

tty = "/dev/ttyUSB0"

def main():
    serialStream = serial.Serial(tty, 9600, timeout=5)
    serialStream.open()
    doread = True

    # Print welcome message
    print('Welcome. Please place a MiFare Tag on the NFC Reader.')

    while(True):

        try:
            rc = readMessage(serialStream)

            if rc[0] is '0':
                # NFC Tag read correctly
                print('NFC Tag was successfully read.')
                numBlinks = raw_input('Input number of blinks: ')

                if numBlinks is 'q':
                    serialStream.close()
                    break
                elif isInt(numBlinks):
                    serialStream.write(numBlinks)
                else:
                    # Help message?
                    print('help message')
                print "Arduino should be blinking."

            elif rc[0] is '1':
                # NFC Tag not read correctly
                print('NFC Tag was not read correctly, please try again')
        except IOError:
            pass
        

def isInt(num):
    try:
        int(num)
        return True
    except ValueError:
        return False

# Returns a list of messages from Arduino.
def readMessage(serialStream, timeout=5):
    output = ""
    while not output.endswith('\0'):
        if timeout <= 0:
            raise IOError()
        num = serialStream.inWaiting()
        if num > 0:
            output += serialStream.read(num)
            print output
            serialStream.flushOutput()
        timeout -= .5
        time.sleep(.5)
    return output.split('\0')[:-1]

if __name__ == '__main__':
	main()
