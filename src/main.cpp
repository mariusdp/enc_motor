/*
  Rotary Encoder - Controlling a DC Motor using L293D Shield
  modified on 23 Feb 2019
  by Saeed Hosseini
  https://electropeak.com/learn/
*/

#include <Arduino.h> 
#include <AFMotor.h>
 
#define CLK 2
#define DT  5
#define SW  3

// int encoder_detect;
// int button_detect; 

AF_DCMotor motor(1, MOTOR12_64KHZ);
 
int motor_dir = 0;
int State;
int old_State, change;
volatile int motor_speed = 0;
volatile boolean buttonState = false;

void encoder_detect ()  {
  buttonState = false;
  State = digitalRead(CLK);
  if (State != old_State)
  {
    if (digitalRead(DT) != State)
    {
      motor_speed ++;
      if (motor_speed >= 255) motor_speed = 255;
    }
    else {
      motor_speed --;
      if (motor_speed <= -255) motor_speed = -255;
    }
  }
  old_State = State;
}
 
void button_detect()
{
  buttonState = true;
}

void setup() {
  Serial.begin(9600);
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);
  old_State = digitalRead(CLK);
  attachInterrupt (digitalPinToInterrupt(CLK), encoder_detect, CHANGE);
  attachInterrupt (digitalPinToInterrupt(SW), button_detect, FALLING);
}

void loop() {
  if (!buttonState)
  {
    if (motor_speed > 0)
    {
      motor.setSpeed(motor_speed);
      motor.run(FORWARD);
      Serial.print("Move Forward: ");
      Serial.println(motor_speed);
    }
    else if (motor_speed < 0)
    {
      motor.setSpeed((motor_speed) * (-1));
      motor.run(BACKWARD);
      Serial.print("Move Backward: ");
      Serial.println(motor_speed);
    }
  }
  if (buttonState == true || motor_speed == 0)
  {
    motor.run(RELEASE);
    Serial.println("Break");
  }
}
