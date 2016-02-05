//ARDUINO UNO DATA TYPE SIZES
// byte - 8 bits 1 byte
// int - 16 bits 2 bytes
// long - 32 bits 4 bytes

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
}

Response arm_to(ArmCommand command)
{
  Response resp;
  resp.error = false;
  resp.describe = "Moved Arm " + (String)command.centimeters + " cm.";
  return resp;
}

Response claw_to(ClawCommand command)
{
  Response resp;
  resp.error = false;
  resp.describe = "Moved Claw " + (String)command.centimeters + " cm.";
  return resp;
}

Response move_to(MoveCommand command)
{  
  Response resp;
  resp.error = false;
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
  String dir = "left.";
  if (command.dir == 4)
  {
    dir = "right."; 
  }
  resp.describe = "Rotated " + (String)command.rot_degrees + " degrees to the " + dir;
  return resp;
}

void sendResponse(Response resp)
{
  int len = resp.describe.length();
  char describe[len + 4];
  char buf[len];
  resp.describe.toCharArray(buf, len);
  sprintf(describe, "\\x%02X%s", len, buf);
  Serial.print(describe);
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
  //Serial.println("What hapened? " + what);
}
