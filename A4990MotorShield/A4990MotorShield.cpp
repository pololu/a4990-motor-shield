#include "A4990MotorShield.h"
unsigned char A4990MotorShield::_M1DIR;
unsigned char A4990MotorShield::_M2DIR;
unsigned char A4990MotorShield::_M1PWM;
unsigned char A4990MotorShield::_M2PWM;
unsigned char A4990MotorShield::_FAULT;
bool A4990MotorShield::_flipM1;
bool A4990MotorShield::_flipM2;

A4990MotorShield::A4990MotorShield()
{
  _M1DIR = 7;
  _M1PWM = 9;
  _M2DIR = 8;
  _M2PWM = 10;
  _FAULT = 6;
  _flipM1 = _flipM2 = false;
}

void A4990MotorShield::initPinsAndMaybeTimer()
{
  pinMode(_M1PWM, OUTPUT);
  pinMode(_M2PWM, OUTPUT);
  pinMode(_M1DIR, OUTPUT);
  pinMode(_M2DIR, OUTPUT);
  pinMode(_FAULT, INPUT);
  digitalWrite(_FAULT, HIGH); //enable pullup
#ifdef A4990MOTORSHIELD_USE_20KHZ_PWM
  // Timer 1 configuration
  // prescaler: clockI/O / 1
  // outputs enabled
  // phase-correct PWM
  // top of 400
  //
  // PWM frequency calculation
  // 16MHz / 1 (prescaler) / 2 (phase-correct) / 400 (top) = 20kHz
  TCCR1A = 0b10100000;
  TCCR1B = 0b00010001;
  ICR1 = 400;
#endif
}

void A4990MotorShield::flipM1(bool flip)
{
  A4990MotorShield::_flipM1 = flip;
}

void A4990MotorShield::flipM2(bool flip)
{
  A4990MotorShield::_flipM2 = flip;
}

// speed is a number between -400 and 400
void A4990MotorShield::setM1Speed(int speed)
{
  init(); // initialize if necessary
    
  bool reverse = 0;
  
  if (speed < 0)
  {
    speed = -speed; // make speed a positive quantity
    reverse = 1;    // preserve the direction
  }
  if (speed > 400)  // Max 
    speed = 400;
    
#ifdef A4990MOTORSHIELD_USE_20KHZ_PWM
  OCR1A = speed;
#else
  analogWrite(_M1PWM, speed * 51 / 80); // default to using analogWrite, mapping 400 to 255
#endif 

  if (reverse ^ A4990MotorShield::_flipM1) // flip if speed was negative or flip_M1 setting is active, but not both
    digitalWrite(_M1DIR, HIGH);
  else
    digitalWrite(_M1DIR, LOW);
}

// speed is a number between -400 and 400
void A4990MotorShield::setM2Speed(int speed)
{
  init(); // initialize if necessary
    
  bool reverse = 0;
  
  if (speed < 0)
  {
    speed = -speed;  // Make speed a positive quantity
    reverse = 1;  // Preserve the direction
  }
  if (speed > 400)  // Max PWM duty cycle
    speed = 400;
    
#ifdef A4990MOTORSHIELD_USE_20KHZ_PWM
  OCR1B = speed;
#else
  analogWrite(_M2PWM, speed * 51 / 80); // default to using analogWrite, mapping 400 to 255
#endif

  if (reverse ^ A4990MotorShield::_flipM2) // flip if speed was negative or flip_M2 setting is active, but not both
    digitalWrite(_M2DIR, HIGH);
  else
    digitalWrite(_M2DIR, LOW);
}

// set speed for both motors
// speed is a number between -400 and 400
void A4990MotorShield::setSpeeds(int m1Speed, int m2Speed)
{
  setM1Speed(m1Speed);
  setM2Speed(m2Speed);
}

bool A4990MotorShield::getFault()
{
  return digitalRead(_FAULT) == LOW;
}