#ifndef robot_proto_h
#define robot_proto_h
#include "Arduino.h"

// PROTCOL STUFF

typedef struct
{
  unsigned int id;
  unsigned int centimeters;
}ArmCommand;

typedef struct
{
  unsigned int id;
  unsigned int centimeters;
}ClawCommand;

typedef struct
{
  unsigned int id;
  unsigned int dir; // Direction to move in FORWARD/BACKWARD
  unsigned int centimeters;
}MoveCommand;

typedef struct
{
  unsigned int id;
  unsigned int dir; // Direction to rotate in LEFT/RIGHT
  unsigned int rot_degrees; // how much to rotate;
}RotateCommand;

struct Response
{
  byte code;
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

typedef struct Response Response;

#endif
