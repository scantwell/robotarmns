//ARDUINO UNO DATA TYPE SIZES
// byte - 8 bits 1 byte
// int - 16 bits 2 bytes
// long - 32 bits 4 bytes

#include <Servo.h>
#include <math.h>
#include "robot_proto.h"

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

Servo pincer;
const int PINCER_MAX = 103;
const int PINCER_MIN = 40;

Servo arm; // servo to cm = (101 - 5)/(31.3 - 4.2) = 3.542435      3.5
const int ARM_MAX = 101; //4.2 cm 
const int ARM_MIN = 5; // 31.3 cm

//wheel diameters = 7.4 cm
//wheel c = 23.25cm 
//1 Revolution = 1.820 Seconds
//ms/cm = 78.2795
Servo left_wheel;
const int LEFT_WHEEL_FORWARD = 45;
const int LEFT_WHEEL_BACKWARD = 135;
      
Servo right_wheel;
const int RIGHT_WHEEL_FORWARD = 135;
const int RIGHT_WHEEL_BACKWARD = 45;

const int WHEEL_STOP = 90;
const int TURNING_RADIUS = 19;

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
  
  // Init Pincer
  // Wire Color: White
  pincer.attach(6);

  // Init Arm
  // Wire Color: Green
  arm.attach(3);

  // Init Left Wheel, range 0 - 180
  // Wire Color: Yellow
  left_wheel.attach(9);

  // Init Right Wheel, range 0 - 180
  // Wire Color: Yellow
  right_wheel.attach(11);

  
  // Write Starting Values
  // 40
  pincer.write(103);
  arm.write(103);  
  left_wheel.write(90);
  right_wheel.write(90);
  //while (!Serial) {
  //  ; // wait for serial port to connect. Needed for native USB port only
  //}
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
        ArmCommand armCommand;
        armCommand.id = header;
        armCommand.centimeters = centimeters;
        arm_to(&armCommand);
        break;
      case 2: // Claw Command
        ClawCommand clawCommand;
        clawCommand.id = header;
        clawCommand.centimeters = centimeters;
        claw_to(&clawCommand);
        break;
      case 3: // Move Command
        MoveCommand moveCommand;
        moveCommand.id = header;
        moveCommand.dir = dir;
        moveCommand.centimeters = centimeters;
        move_to(&moveCommand);
        break;
      case 4: // Rotate Command
        RotateCommand rotCommand;
        rotCommand.id = header;
        rotCommand.dir = dir;
        rotCommand.rot_degrees = centimeters;
        rotate(&rotCommand);
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

// ROBOT END


// MOVEMENT FUNCTIONS

Response arm_to(ArmCommand *command)
{
  Response resp;

  resp.command = (byte)command->id;//"Arm";
  resp.describe = "Moved Arm " + (String)command->centimeters + " cm.";
  arm.write(cm_to_arm_servo(command->centimeters));
  return resp;
}

Response claw_to(ClawCommand *command)
{
  Response resp;
  resp.command = (byte)command->id;

  pincer.write(cm_to_claw_servo(command->centimeters));
  resp.describe = "Moved Claw " + (String)command->centimeters + " cm.";
  return resp;
}

Response move_to(MoveCommand *command)
{ 
  Response resp;
  resp.command = (byte)command->id;
  
  int delay_ms = cm_to_move_delay((double)command->centimeters);
  
  if (command->dir == 2)
  {
    left_wheel.write(LEFT_WHEEL_BACKWARD);
    right_wheel.write(RIGHT_WHEEL_BACKWARD);
    resp.describe = "Moved Robot backward " + (String)command->centimeters + " cm." ;
  }
  else
  {
    left_wheel.write(LEFT_WHEEL_FORWARD);
    right_wheel.write(RIGHT_WHEEL_FORWARD);
    resp.describe = "Moved Robot forward " + (String)command->centimeters + " cm." ;
  }

  delay(delay_ms);
  
  left_wheel.write(WHEEL_STOP);
  right_wheel.write(WHEEL_STOP);
  
  return resp;
}

Response rotate(RotateCommand *command)
{
  Response resp;
  resp.command = (byte)command->id;
  int rVal;
  int lVal;
  
  if (command->rot_degrees < 0)
  {
    resp.error = true;
    resp.error_message = "Failed to rotate. Invalid negative degrees.";
    return resp;
  }
  
  int delay_ms = degrees_to_rotate_delay(command->rot_degrees);
  
  if (command->dir == 3)
  {
    rVal = RIGHT_WHEEL_FORWARD;
    lVal = LEFT_WHEEL_BACKWARD;
    resp.describe = "Rotated " + (String)command->rot_degrees + " degrees to the left.";
  }
  else if (command->dir == 4)
  {
    rVal = RIGHT_WHEEL_BACKWARD;
    lVal = LEFT_WHEEL_FORWARD;
    resp.describe = "Rotated " + (String)command->rot_degrees + " degrees to the right.";
  }
  else
  {
    // ERROR
    resp.error = true;
    resp.error_message = "Unkown rotation direction " + (String)command->dir;
    return resp;
  }
  
  right_wheel.write(rVal);
  left_wheel.write(lVal);
  
  delay(delay_ms);
  
  left_wheel.write(WHEEL_STOP);
  right_wheel.write(WHEEL_STOP);
  
  return resp;
}

// MOVEMENT END

// FREE FUNCTIONS

int cm_to_arm_servo(int cm)
{
  double rv_per_cm = 3.542435;
  int servo = round(rv_per_cm * cm + 9.868);
  servo = (servo - 106) * -1;
  if(servo > ARM_MAX)
  {
    servo = ARM_MAX;
  }
  else if(servo < ARM_MIN)
  {
    servo = ARM_MIN;
  }
  
  return servo;
}

int cm_to_claw_servo(int cm)
{
  double rv_per_cm = 12.8571429;
  int servo = round(rv_per_cm * cm + 40);
  servo = (servo - 143) * -1;
  if(servo > PINCER_MAX)
  {
    servo = PINCER_MAX;
  }
  else if(servo < PINCER_MIN)
  {
    servo = PINCER_MIN;
  }
  
  return servo;
}

int cm_to_move_delay(double cm)
{
  return round(cm * 78.2795);
}

int degrees_to_rotate_delay(unsigned int angle_degrees)
{
  double angle_rads = (angle_degrees * M_PI) / 180.0f;
  double cms = (TURNING_RADIUS * angle_rads);
  
  return cm_to_move_delay((cms/2.0f));
}

// FREE FUNCTION END
