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

void arm_to(ArmCommand command)
{
}

void claw_to(ClawCommand command)
{ 
}

void move_to(MoveCommand command)
{ 
}

void rotate(RotateCommand command)
{ 
}

void sendResponse(Response resp)
{
  int len = resp.describe.length();
  char describe[len];

  resp.describe.toCharArray(describe, len);
  Serial.print(len, HEX);
  Serial.write(describe);
}

void loop() {
  while( Serial.available() > 0)
  {
    String dir;
    header.bytes[0] = Serial.read();
    switch(header.value)
    {
      // ARM COMMAND
      case 1:
        ArmCommand armCommand;
        armCommand.id = header.value;
        armCommand.cmd_length = (int)Serial.read();
        armCommand.centimeters = readInt();
        arm_to(armCommand);
        what = "Moved Arm " + (String)armCommand.centimeters + "cm.";
        break;
      case 2: // Claw Command
        ClawCommand clawCommand;
        clawCommand.id = header.value;
        clawCommand.cmd_length = (int)Serial.read();
        clawCommand.centimeters = readInt();
        claw_to(clawCommand);
        what = "Moved Claw " + (String)clawCommand.centimeters + "cm.";
        break;
      case 3: // Move Command
        MoveCommand moveCommand;
        moveCommand.id = header.value;
        moveCommand.cmd_length = (int)Serial.read();
        moveCommand.dir = (int)Serial.read();
        moveCommand.centimeters = readInt();
        move_to(moveCommand);
        dir = "forward";
        if (moveCommand.dir == 2)
        {
          dir = "backward"; 
        }
        what = "Moved Robot " + dir + " " + (String)moveCommand.centimeters + " cm." ;
        break;
      case 4: // Rotate Command
        RotateCommand rotCommand;
        rotCommand.id = header.value;
        rotCommand.cmd_length = (int)Serial.read();
        rotCommand.dir = (int)Serial.read();
        rotCommand.rot_degrees = readInt();
        rotate(rotCommand);
        dir = "left.";
        if (moveCommand.dir == 4)
        {
          dir = "right."; 
        }
        what = "Rotated " + (String)rotCommand.rot_degrees + " degrees to the " + dir;
        break;
      default:
        what = "Wrong header? " + header.value;
        break;
    } // end of switch
    resp.describe = what;
    sendResponse(resp);
  } // end of while
  delay(2000);
  //Serial.println("What hapened? " + what);
}
