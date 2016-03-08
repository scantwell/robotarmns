//ARDUINO UNO DATA TYPE SIZES
// byte - 8 bits 1 byte
// int - 16 bits 2 bytes
// long - 32 bits 4 bytes

#include <math.h>
#include "robot_proto.h"
#include "robot.h"

void sendField(String value)
{
  int len = value.length()  + 1;
  char retval[len];
  value.toCharArray(retval, len);
  Serial.write(retval);
  Serial.write(0x00);
}

void sendFlag(boolean value)
{
  if (value)
  {
    Serial.write(0x01);
  }
  else
  {
    Serial.write(0x00);
  }
  Serial.write(0x00);
}

void sendResponse(Response resp)
{
  Serial.write(resp.code);
  //sendField(resp.describe);
  //sendFlag(resp.error);
  //sendField(resp.error_message);
  //Serial.write(0x7F);
  //Serial.flush();
}

// PROTCOL END


// ROBOT 
Robot* ROBOT;

const byte DATA_PASS = 0;
const byte DATA_FAIL = 1;

unsigned int readUnsignedInt()
{
  byte lsb = Serial.read();
  byte msb = Serial.read();
  return ((msb << 8) | lsb);
}

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);

  ROBOT = new Robot(3, 6, 9, 11);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() { 
  Response resp;
  while (Serial.available() != 6)
  {
    while (Serial.available())
    {
      Serial.read();
    }
    resp.code = DATA_FAIL;
    sendResponse(resp);
    delay(2000);
  }
  
  unsigned int header = (unsigned int)Serial.read();
  unsigned int dir = (unsigned int)Serial.read();
  unsigned int centimeters = readUnsignedInt();
  unsigned int checksum = readUnsignedInt();

  unsigned int chksum = getChecksum(header, dir, centimeters);
  if( checksum == chksum)
  {
    resp.code = DATA_PASS;
    switch(header)
    {
      // ARM COMMAND
      case 1:
        ROBOT->arm(centimeters);
        break;
      case 2: // Claw Command
        ROBOT->claw(centimeters);
        break;
      case 3: // Move Command
        ROBOT->move(dir, centimeters);
        break;
      case 4: // Rotate Command 
        ROBOT->rotate(dir, centimeters);
        break;
      default:
        resp.code = DATA_FAIL;
        break;
    } // end of switch
  } // end of checksum
  else
  {
    resp.code = DATA_FAIL;
  }
  while (Serial.available() == 0)
  {
    sendResponse(resp);
    delay(1500);
  }
}

unsigned int getChecksum(unsigned int header, unsigned int dir, unsigned int centimeters)
{
  unsigned int sum = header + dir;
  sum += ((centimeters << 8) >> 8); // lsb
  sum += (centimeters >> 8); //msb
  return sum;
}

