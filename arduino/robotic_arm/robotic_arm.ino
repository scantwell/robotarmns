// Robotic Arm Arduino Code
// 11/10/15
/*
rotate(float degrees, int direction)


// protocol

1 = FORWARD
2 = BACKWARD
3 = LEFT
4 = RIGHT

degrees = 0 - 360

// DISTANCE FUNCTIONS
1 = move(int direction, float cm)
2 = rotate(int direction, float degrees)
3 = arm_to(float cm)
4 = claw_to(float cm)

*/


#include <Servo.h> 
const int BUFFER_SIZE = 256;

// open 10.5 cm
// closed 0 cm
// arm distance
Servo pincer;
const int PINCER_MAX = 103;
const int PINCER_MIN = 0;

Servo arm; // servo to cm = (101 - 5)/(31.3 - 4.2) = 3.542435      3.5
const int ARM_MAX = 101; //4.2 cm 
const int ARM_MIN = 5; // 31.3 cm

Servo left_wheel;
const int LEFT_WHEEL_FORWARD = 135;
const int LEFT_WHEEL_BACKWARD = 45;
      
Servo right_wheel;
const int RIGHT_WHEEL_FORWARD = 45;
const int RIGHT_WHEEL_BACKWARD = 135;


// Value containers
int arm_value;
int pincer_value;

unsigned long readULongFromBytes() {
  union u_tag {
    byte b[4];
    unsigned long ulval;
  } u;
  u.b[0] = Serial.read();
  u.b[1] = Serial.read();
  u.b[2] = Serial.read();
  u.b[3] = Serial.read();
  return u.ulval;
}

void setup() 
{ 
  // Init Serial Connction
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
  pincer_value = 95;
  arm.write(103);  
  arm_value = 95;
  left_wheel.write(90);
  right_wheel.write(90);
} 


void loop() 
{
  unsigned long command, value, return_value;
  
  while(Serial.available() < 8) ;
  command = readULongFromBytes();
  value = readULongFromBytes();
  Serial.print(command);
  Serial.print(value);
  
  switch(command) {
   case 0:
     // Pincer Command
     return_value = move_pincer(value);
     break;
   case 1:
     // Arm Command
     return_value = move_arm(value);
     break;
   case 2:
     // Forward Command
     return_value = move_forward(value);
     break;
   case 3:
     // Backward Command
     return_value = move_backward(value);
     break;
   case 4:
     // Left Command
     return_value = turn_left(value);
     break;
   case 5:
     // Right Command
     return_value = turn_right(value);
     break;
   default:
     // Panic
     return_value = command;
     break;
       
  }
  
  Serial.write(return_value);
}

//  Debug method used to test all servos and methods 
void run_diagnostics() {
  // Move Arm
  arm.write(5);
  delay(8000);
  arm.write(100);
  delay(8000);
  
  // Move Forward/Backward/Left/Right
  move_forward(100);
  delay(500);
  move_backward(100);
  delay(500);
  turn_left(100);
  delay(500);
  turn_right(100);
  delay(500);
  turn_right(100);
  delay(500);
  turn_left(100);
  delay(500);
  
  // Move Pincer
  move_pincer(0);
  delay(2000);
  move_pincer(100);
}

// Move pincer slowly from current position
int move_pincer(int degree)
{
  degree = degree < PINCER_MIN ? PINCER_MIN : degree;
  degree = degree > PINCER_MAX ? PINCER_MAX : degree;
  
  if ( degree < pincer_value) {
    for (int i = pincer_value; i > degree; --i)
    {
      pincer.write(i);
      delay(15);
    }
  } else { 
    for (int i = pincer_value; i < degree; ++i)
    {
      pincer.write(i);
      delay(15);
    }
  }
  pincer_value = degree;
  return 0;
}

// Move arm slowly from current position
int move_arm(int degree) {
 degree = degree < ARM_MIN ? ARM_MIN : degree;
 degree = degree > ARM_MAX ? ARM_MAX : degree;
 
 if ( degree < arm_value) {
   for (int i = arm_value; i > degree; --i) {
     arm.write(i);
     delay(15);
   }   
 } else {  
   for (int i = arm_value; i < degree; ++i ) {
     arm.write(i);
     delay(15);
   } 
 }
 arm_value = degree; 
 return 0;
}

// Move forward for ms Milliseconds
int move_forward(int ms)
{
  right_wheel.write(135);
  left_wheel.write(45);
  delay(ms);
  right_wheel.write(90);
  left_wheel.write(90);
  return 0;
}

// Move backward for ms Milliseconds
int move_backward(int ms)
{
  right_wheel.write(45);
  left_wheel.write(135);
  delay(ms);
  right_wheel.write(90);
  left_wheel.write(90);
  return 0;
}

// Turn left for ms Milliseconds
int turn_left(int ms)
{
  right_wheel.write(135);
  left_wheel.write(135);
  delay(ms);
  right_wheel.write(90);
  left_wheel.write(90);
  return 0;  
}

// Turn right for ms Milliseconds
int turn_right(int ms)
{
  right_wheel.write(45);
  left_wheel.write(45);
  delay(ms);
  right_wheel.write(90);
  left_wheel.write(90);  
  return 0;
}

int cm_to_servo(float cm)
{
  float rv_per_cm = 3.542435;
  return round(rv_per_cm * cm + 9.868);
}

