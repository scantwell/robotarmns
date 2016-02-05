//ARDUINO UNO DATA TYPE SIZES
// byte - 8 bits 1 byte
// int - 16 bits 2 bytes
// long - 32 bits 4 bytes

#include <Servo.h>
#include <Math.h>

Servo pincer;
const int PINCER_MAX = 103;
const int PINCER_MIN = 0;

Servo arm; // servo to cm = (101 - 5)/(31.3 - 4.2) = 3.542435      3.5
const int ARM_MAX = 101; //4.2 cm 
const int ARM_MIN = 5; // 31.3 cm

//wheel diameters = 7.4 cm
//wheel c = 23.25cm 
//1 Revolution = 1.820 Seconds
//ms/cm = 78.2795
Servo left_wheel;
const int LEFT_WHEEL_FORWARD = 135;
const int LEFT_WHEEL_BACKWARD = 45;
      
Servo right_wheel;
const int RIGHT_WHEEL_FORWARD = 45;
const int RIGHT_WHEEL_BACKWARD = 135;

const int WHEEL_STOP = 90;

union robot_int
{
  unsigned int value;
  byte bytes[2];  
};

struct ArmCommand
{
  int id;
  int cmd_length; // length of data in bytes
  int centimeters;
};

struct ClawCommand
{
  int id;
  int cmd_length; // length of data in bytes
  int centimeters;
};

struct MoveCommand
{
  int id;
  int cmd_length; // length of data in bytes
  int dir; // Direction to move in FORWARD/BACKWARD
  int centimeters;
};

struct RotateCommand
{
  int id;
  int cmd_length; // length of data in bytes
  int dir; // Direction to rotate in LEFT/RIGHT
  int rot_degrees; // how much to rotate;
};

struct Response
{
  String command;
  String describe;
  boolean error;
  String error_message;
  Response() { 
    command = "0";
    describe = ""; 
    error = false;
    error_message = "";
  }
};

int readInt()
{
  robot_int rv; 
  rv.bytes[0] = Serial.read();
  rv.bytes[1] = Serial.read();
  return rv.value;
}

byte bytes[2];
String what;
robot_int header;
Response resp;

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
}

Response arm_to(ArmCommand command)
{
  Response resp;
  resp.error = false;
  resp.error_message = move_arm_to(command);
  if(resp.error_message != "")
  {
    resp.error = true;
  }
  resp.describe = "Moved Arm " + (String)command.centimeters + " cm.";
  return resp;
}

Response claw_to(ClawCommand command)
{
  Response resp;
  resp.error = false;
  resp.error_message = move_claw_to(command);
  if(resp.error_message != "")
  {
    resp.error = true;
  }
  resp.describe = "Moved Claw " + (String)command.centimeters + " cm.";
  return resp;
}

Response move_to(MoveCommand command)
{  
  Response resp;
  
  resp.error = false;
  resp.error_message = move_robot_to(command);
  if(resp.error_message != "")
  {
    resp.error = true;
  }
  
  String dir = "forward";
  
  if (command.dir == 2)
  {
    dir = "backward"; 
  }
  resp.describe = "Moved Robot " + dir + " " + (String)command.centimeters + " cm." ;
  return resp;
}

Response rotate(RotateCommand command)
{
  Response resp;
  if (command.rot_degrees < 0)
  {
    resp.error = true;
    resp.error_message = "Failed to rotate. Invalid negative degrees.";
  }
  
  resp.error_message = rotate_robot_to(command);
  if(resp.error_message != "")
  {
    resp.error = true;
  }
  
  String dir = "left.";
  if (command.dir == 4)
  {
    dir = "right."; 
  }
  resp.describe = "Rotated " + (String)command.rot_degrees + " degrees to the " + dir;
  return resp;
}

void sendField(String value)
{
   int len = value.length();
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

void sendInt8(int value)
{
  robot_int rv;
  rv.value = value;
  Serial.write(rv.bytes[0]);
}

void sendResponse(Response resp)
{
  sendField(resp.command);
  sendField(resp.describe);
  sendFlag(resp.error);
  sendField(resp.error_message);
}

String move_arm_to(ArmCommand command)
{
  arm.write(cm_to_arm_servo(command.centimeters));
  return "";
}

String move_claw_to(ClawCommand command)
{
  
  return "";  
}

String move_robot_to(MoveCommand command)
{
  int delay_ms = cm_to_move_delay(command.centimeters);
  if(command.dir = 1)
  {
    left_wheel.write(LEFT_WHEEL_FORWARD);
    right_wheel.write(RIGHT_WHEEL_FORWARD);
  }
  else
  {
    left_wheel.write(LEFT_WHEEL_BACKWARD);
    right_wheel.write(RIGHT_WHEEL_BACKWARD);
  }
  delay(delay_ms);
  
  left_wheel.write(WHEEL_STOP);
  right_wheel.write(WHEEL_STOP);
  
  return "";  
}

String rotate_robot_to(RotateCommand command)
{
  
  return "";  
}

int cm_to_arm_servo(int cm)
{
  float rv_per_cm = 3.542435;
  int servo = round(rv_per_cm * cm + 9.868);
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

int cm_to_move_delay(int cm)
{
  return round(cm * 78.2795);
}

void loop() {
  while( Serial.available() > 0)
  {
    String dir;
    header.bytes[0] = Serial.read();
    Response resp;
    switch(header.value)
    {
      // ARM COMMAND
      case 1:
        ArmCommand armCommand;
        armCommand.id = header.value;
        armCommand.cmd_length = (int)Serial.read();
        armCommand.centimeters = readInt();
        resp = arm_to(armCommand);
        break;
      case 2: // Claw Command
        ClawCommand clawCommand;
        clawCommand.id = header.value;
        clawCommand.cmd_length = (int)Serial.read();
        clawCommand.centimeters = readInt();
        resp = claw_to(clawCommand);
        
        break;
      case 3: // Move Command
        MoveCommand moveCommand;
        moveCommand.id = header.value;
        moveCommand.cmd_length = (int)Serial.read();
        moveCommand.dir = (int)Serial.read();
        moveCommand.centimeters = readInt();
        resp = move_to(moveCommand);
        break;
      case 4: // Rotate Command
        RotateCommand rotCommand;
        rotCommand.id = header.value;
        rotCommand.cmd_length = (int)Serial.read();
        rotCommand.dir = (int)Serial.read();
        rotCommand.rot_degrees = readInt();
        resp = rotate(rotCommand);
        break;
      default:
        resp.error = false;
        resp.error_message = "Wrong header? " + header.value;
        break;
    } // end of switch
    sendResponse(resp);
  } // end of while
  delay(2000);
}
