#ifndef A4990MotorShield_h
#define A4990MotorShield_h

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined (__AVR_ATmega32U4__)
  #define A4990MOTORSHIELD_USE_20KHZ_PWM
#endif

#include <Arduino.h>

class A4990MotorShield 
{
  public:
    // Constructor
    //A4990MotorShield();
    
    // Public methods
    static void setM1Speed(int speed);
    static void setM2Speed(int speed);
    static void setSpeeds(int m1Speed, int m2Speed);
    static void flipM1(bool flip);
    static void flipM2(bool flip);
    static bool getFault();
    
  private:
    static void initPinsAndMaybeTimer();
    const static unsigned char _M1DIR;
    const static unsigned char _M2DIR;
    const static unsigned char _M1PWM;
    const static unsigned char _M2PWM;
    const static unsigned char _FAULT;
    static bool _flipM1;
    static bool _flipM2;
    static inline void init()
    {
      static bool initialized = false;
      if (!initialized)
      {
        initialized = true;
        initPinsAndMaybeTimer();
      }
    }
};
#endif