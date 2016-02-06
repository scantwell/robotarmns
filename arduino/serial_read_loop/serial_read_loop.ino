//ARDUINO UNO DATA TYPE SIZES
// byte - 8 bits 1 byte
// int - 16 bits 2 bytes
// long - 32 bits 4 bytes

#include <Servo.h>
#include <math.h>


// PROTCOL STUFF

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
  byte command;
  String describe;
  boolean error;
  String error_message;
  Response() { 
    command = 0;
    describe = ""; 
    error = false;
    error_message = "";
  }
};

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
  Serial.write(resp.command);
  sendField(resp.describe);
  sendFlag(resp.error);
  sendField(resp.error_message);
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
const int LEFT_WHEEL_FORWARD = 135;
const int LEFT_WHEEL_BACKWARD = 45;
      
Servo right_wheel;
const int RIGHT_WHEEL_FORWARD = 45;
const int RIGHT_WHEEL_BACKWARD = 135;

const int WHEEL_STOP = 90;
const int TURNING_RADIUS = 19;

union robot_int
{
  unsigned int value;
  byte bytes[2];  
};


int readInt()
{
  robot_int rv; 
  rv.bytes[0] = Serial.read();
  rv.bytes[1] = Serial.read();
  return rv.value;
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
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() {
  if( Serial.available() > 0)
  {
    Response resp;
    robot_int header;
    header.bytes[0] = Serial.read();
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
        resp.error = true;
        resp.error_message = "Wrong header? " + header.value;
        break;
    } // end of switch*/
    sendResponse(resp);
  } // end of while
  delay(2000);
}

// ROBOT END


// MOVEMENT FUNCTIONS

Response arm_to(ArmCommand command)
{
  Response resp;

  resp.command = (byte)command.id;//"Arm";
  resp.describe = "Moved Arm " + (String)command.centimeters + " cm.";
  arm.write(cm_to_arm_servo(command.centimeters));
  return resp;
}

Response claw_to(ClawCommand command)
{
  Response resp;
  resp.command = (byte)command.id;

  pincer.write(cm_to_claw_servo(command.centimeters));
  resp.describe = "Moved Claw " + (String)command.centimeters + " cm.";
  return resp;
}

Response move_to(MoveCommand command)
{ 
  Response resp;
  resp.command = (byte)command.id;
  
  int delay_ms = cm_to_move_delay((double)command.centimeters);
  
  if (command.dir == 2)
  {
    left_wheel.write(LEFT_WHEEL_BACKWARD);
    right_wheel.write(RIGHT_WHEEL_BACKWARD);
    resp.describe = "Moved Robot backward " + (String)command.centimeters + " cm." ;
  }
  else
  {
    left_wheel.write(LEFT_WHEEL_FORWARD);
    right_wheel.write(RIGHT_WHEEL_FORWARD);
    resp.describe = "Moved Robot forward " + (String)command.centimeters + " cm." ;
  }

  delay(delay_ms);
  
  left_wheel.write(WHEEL_STOP);
  right_wheel.write(WHEEL_STOP);
  
  return resp;
}

Response rotate(RotateCommand command)
{
  Response resp;
  resp.command = (byte)command.id;
  int rVal;
  int lVal;
  
  if (command.rot_degrees < 0)
  {
    resp.error = true;
    resp.error_message = "Failed to rotate. Invalid negative degrees.";
    return resp;
  }
  
  int delay_ms = degrees_to_rotate_delay(command.rot_degrees);
  
  if (command.dir == 3)
  {
    rVal = RIGHT_WHEEL_FORWARD;
    lVal = LEFT_WHEEL_BACKWARD;
    resp.describe = "Rotated " + (String)command.rot_degrees + " degrees to the left.";
  }
  else if (command.dir == 4)
  {
    rVal = RIGHT_WHEEL_BACKWARD;
    lVal = LEFT_WHEEL_FORWARD;
    resp.describe = "Rotated " + (String)command.rot_degrees + " degrees to the right.";
  }
  else
  {
    // ERROR
    resp.error = true;
    resp.error_message = "Unkown rotation direction " + (String)command.dir;
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
  int servo = round(rv_per_cm * cm + 40.868);
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

int degrees_to_rotate_delay(int angle_degrees)
{
  double angle_rads = (angle_degrees * M_PI) / 180.0f;
  double cms = (TURNING_RADIUS * angle_rads);
  
  return cm_to_move_delay((cms/2.0f));
}

// FREE FUNCTION END
