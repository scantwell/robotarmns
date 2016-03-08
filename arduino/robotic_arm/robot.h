#include <math.h>
#include <Servo.h>

class Robot
{
private:
  Servo arm_serv, claw_serv, l_wheel, r_wheel;
  int PINCER_MAX = 103;
  int PINCER_MIN = 40;
  
  int ARM_MAX = 101; //4.2 cm 
  int ARM_MIN = 5; // 31.3 cm
  
  int LEFT_WHEEL_FORWARD = 45;
  int LEFT_WHEEL_BACKWARD = 135;
        
  int RIGHT_WHEEL_FORWARD = 135;
  int RIGHT_WHEEL_BACKWARD = 45;
  
  int WHEEL_STOP = 90;
  int TURNING_RADIUS = 19;

  float DEGREES_TO_RADIANS = M_PI/180.0;
  float CM_TO_WHEEL_DELAY = 78.2795;
  void reset();
  
public:
  Robot(int arm_pin, int claw_pin, int left_wheel_pin, int right_wheel_pin);
  void arm(unsigned int centimeters);
  void claw(unsigned int centimeters);
  void move(unsigned int dir, unsigned int centimeters);
  void rotate(unsigned int dir, unsigned int deg);
  
  int FORWARD = 1;
  int BACKWARD = 2;
  int LEFT = 3;
  int RIGHT = 4;
};

Robot::Robot(int arm, int claw, int left_wheel, int right_wheel)
{
  this->arm_serv.attach(arm);
  this->claw_serv.attach(claw);
  this->l_wheel.attach(left_wheel);
  this->r_wheel.attach(right_wheel);
  
  this->arm(0);
  this->move(this->FORWARD, 0);
  this->claw(0);
}

void Robot::arm(unsigned int cm)
{
  double rv_per_cm = 3.542435;
  int s_val = round(this->ARM_MAX - (rv_per_cm * cm)); // Value of the servo
 
  if(s_val > ARM_MAX)
  {
    s_val = ARM_MAX;
  }
  else if(s_val < ARM_MIN)
  {
    s_val = ARM_MIN;
  }
  this->arm_serv.write(s_val);
}

void Robot::claw(unsigned int cm)
{
  double rv_per_cm = 12.8571429;
  int s_val = round(this->PINCER_MAX - (rv_per_cm * cm));

  if(s_val > PINCER_MAX)
  {
    s_val = PINCER_MAX;
  }
  else if(s_val < PINCER_MIN)
  {
    s_val = PINCER_MIN;
  }
  this->claw_serv.write(s_val);
}

void Robot::move(unsigned int dir, unsigned int cm)
{
  int delay_ms = round(cm * CM_TO_WHEEL_DELAY);
  
  if (dir == this->BACKWARD)
  {
    //port_write_backward();
    this->l_wheel.write(this->LEFT_WHEEL_BACKWARD);
    this->r_wheel.write(this->RIGHT_WHEEL_BACKWARD);
  }
  else
  {
    //port_write_forward();
    this->l_wheel.write(this->LEFT_WHEEL_FORWARD);
    this->r_wheel.write(this->RIGHT_WHEEL_FORWARD);
  }

  delay(delay_ms);

  //port_write_stop();
  this->l_wheel.write(this->WHEEL_STOP);
  this->r_wheel.write(this->WHEEL_STOP);
}

void Robot::rotate(unsigned int dir, unsigned int deg)
{
  int rVal;
  int lVal;

  double angle_rads = deg * DEGREES_TO_RADIANS;
  double cms = (TURNING_RADIUS * angle_rads);
  int delay_ms = round((cms/2.0f) * CM_TO_WHEEL_DELAY);
  
  if (dir == this->LEFT)
  {
    rVal = RIGHT_WHEEL_FORWARD;
    lVal = LEFT_WHEEL_BACKWARD;
  }
  else if (dir == this->RIGHT)
  {
    rVal = RIGHT_WHEEL_BACKWARD;
    lVal = LEFT_WHEEL_FORWARD;
  }
  else
  {
    // NOT CAUGHT
  }
  
  this->r_wheel.write(rVal);
  this->l_wheel.write(lVal);
  
  delay(delay_ms);
  
  this->l_wheel.write(WHEEL_STOP);
  this->r_wheel.write(WHEEL_STOP);
}

// Private

void Robot::reset()
{
  this->arm(0);
  this->claw(0);
  this->move(this->FORWARD, 0);
}


