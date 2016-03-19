//ARDUINO UNO DATA TYPE SIZES
// byte - 8 bits 1 byte
// int - 16 bits 2 bytes
// long - 32 bits 4 bytes

#include <math.h>
#include "robot_proto.h"
#include "robot.h"

// ROBOT 
Robot* ROBOT;
const byte DATA_PASS = 0;
const byte DATA_FAIL = 1; // Code to repeat transmission

// Function declartations
unsigned int getChecksum(unsigned int, unsigned int, unsigned int);
void sendResponse(int);
unsigned int readUnsignedInt();


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
  int response_code;
  // If all packets were not received 
  while (Serial.available() != 6)
  {
    //Clear the buffer
    while (Serial.available())
    {
      Serial.read();
    }
    sendResponse(DATA_FAIL);
    delay(2000);
  }
  
  unsigned int header = (unsigned int)Serial.read();
  unsigned int dir = (unsigned int)Serial.read();
  unsigned int centimeters = readUnsignedInt();
  unsigned int checksum = readUnsignedInt();

  unsigned int chksum = getChecksum(header, dir, centimeters);
  if( checksum == chksum)
  {
    response_code = DATA_PASS;
    switch(header)
    {
      case 1: // Arm Command
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
        response_code = DATA_FAIL;
        break;
    } // end of switch
  } // end of checksum
  else
  {
    response_code = DATA_FAIL;
  }
  // Wait until some data is available
  while (Serial.available() == 0)
  {
    sendResponse(response_code);
    delay(1500);
  }
}

// Summation on all of the packets excluding the checksum itself
unsigned int getChecksum(unsigned int header, unsigned int dir, unsigned int centimeters)
{
  unsigned int sum = header + dir;
  sum += ((centimeters << 8) >> 8); // lsb
  sum += (centimeters >> 8); //msb
  return sum;
}

// Sends DATA_FAIL/DATA_PASS to caller
void sendResponse(int code)
{
  Serial.write(code);
}

// Parse unsigned integer from the serial buffer
unsigned int readUnsignedInt()
{
  byte lsb = Serial.read();
  byte msb = Serial.read();
  return ((msb << 8) | lsb);
}

