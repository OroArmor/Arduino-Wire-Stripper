#include <Stepper.h>
#include <LiquidCrystal.h>
#include <Servo.h>

#include "Button.h"

#define LEFT_BUTTON 29
#define MID_BUTTON 35
#define RIGHT_BUTTON 53

#define STEPPER_PIN_1 9
#define STEPPER_PIN_2 10
#define STEPPER_PIN_3 11
#define STEPPER_PIN_4 12
#define STEPPER_SPEED 10
#define STEPPER_STEPS_PER_REVOLUTION 2048

#define SERVO_PIN 8
#define SERVO_MIN 0
#define SERVO_MAX 180
#define SERVO_MID (SERVO_MAX - SERVO_MIN) / 2 + SERVO_MIN

#define LCD_RESET 7
#define LCD_ENABLE 6
#define LCD_DATA_4 5
#define LCD_DATA_5 4
#define LCD_DATA_6 3
#define LCD_DATA_7 2

LiquidCrystal lcd(LCD_RESET, LCD_ENABLE, LCD_DATA_4, LCD_DATA_5, LCD_DATA_6, LCD_DATA_7);

Stepper stepper(STEPPER_STEPS_PER_REVOLUTION, STEPPER_PIN_1, STEPPER_PIN_2, STEPPER_PIN_3, STEPPER_PIN_4);

void leftButton(bool);
void middleButton(bool);
void rightButton(bool);
void leftButtonHold();
void middleButtonHold();
void rightButtonHold();

Button left(LEFT_BUTTON, leftButton, leftButtonHold);
Button middle(MID_BUTTON, middleButton, middleButtonHold);
Button right(RIGHT_BUTTON, rightButton, rightButtonHold);

int servo_position = 0;
Servo servo;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");

  servo.attach(8);

  stepper.setSpeed(STEPPER_SPEED);
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(servo_position);
  lcd.print("   ");
  stepper.step(2);
  servo.write(servo_position);
  left.tick();
  middle.tick();
  right.tick();
}

void leftButton(bool pushed) {
  if (pushed) {
      servo_position = SERVO_MIN;
  }
}
void middleButton(bool pushed) {
  if (pushed) {
    servo_position = SERVO_MID;
  }
}
void rightButton(bool pushed) {
  if (pushed) {
      servo_position = SERVO_MAX;
  }
}

void leftButtonHold() {
  leftButton(true);
}
void middleButtonHold() {
  middleButton(true);
}
void rightButtonHold() {
  rightButton(true);
}
