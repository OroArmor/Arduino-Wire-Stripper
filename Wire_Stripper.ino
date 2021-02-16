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

int wireLength = 0;
int wireCount = 0;

enum MenuState {
  START,
  LENGTH,
  COUNT,
  CUTTING
};

MenuState menuState = START;


enum CutState {
  NONE = 0,
  STRIP_BEGINNING = 1,
  MIDDLE = 2,
  STRIP_END = 3
};

int cutState = NONE;


int index = 0;
long startTime;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  servo.attach(8);
  servo.write(SERVO_MIN);

  stepper.setSpeed(STEPPER_SPEED);

  startTime = millis();
}

boolean notInc = true;

void loop() {
  lcd.setCursor(0, 0);
  if (menuState == CUTTING) {
    lcd.clear();
    cutState++;
  }

  switch (menuState) {
    case START:
      lcd.print("Wire Cutter");
      lcd.setCursor(0, 1);
      lcd.print(String("Press the middle button to start").substring(index, index + 16));
      break;
    case LENGTH:
      lcd.print("Wire length: ");
      lcd.setCursor(0, 1);
      lcd.print(wireLength);
      break;
    case CUTTING:
      lcd.print("Wires to cut:");
      lcd.setCursor(13, 0);
      lcd.print((cutState == STRIP_BEGINNING ? "beg" : (cutState == MIDDLE ? "mid" : "end")));
      goto PRINT_WIRE_COUNT;
    case COUNT:
      lcd.print("Wire count: ");
PRINT_WIRE_COUNT:
      lcd.setCursor(0, 1);
      lcd.print(wireCount + 1);
      break;
  }

  if (cutState == STRIP_BEGINNING) {
    stepper.step(500);
    writeThenReturnServo(SERVO_MID);
  } else if (cutState == MIDDLE) {
    stepper.step(1000);
    writeThenReturnServo(SERVO_MID);
  } else if (cutState == STRIP_END) {
    stepper.step(500);
    writeThenReturnServo(SERVO_MAX);

    wireCount--;
    cutState = NONE;
  }

  if (wireCount == -1) {
    menuState = START;
    wireCount = 0;
    lcd.clear();
  }

  long deltaTime = millis() - startTime;
  if (deltaTime < 10000) {
    if (deltaTime < 9000 && deltaTime > 1000) {
      if (deltaTime % 500 < 50) {
        if (notInc) {
          index += 1;
          notInc = false;
        }
      } else {
        notInc = true;
      }
    }
  } else {
    index = 0;
    startTime = millis();
  }

  left.tick();
  middle.tick();
  right.tick();
}

void writeThenReturnServo(int setPosition) {
  servo.write(setPosition);
  delay(500);
  servo.write(SERVO_MIN);
  delay(500);
}

void leftButton(bool pushed) {
  if (pushed) {
    lcd.clear();
    switch (menuState) {
      case START:
      case CUTTING:
        break;
      case LENGTH:
        wireLength--;
        break;
      case COUNT:
        wireCount--;
        break;
    }
    if (wireLength < 0) {
      wireLength = 0;
    }

    if (wireCount < 0) {
      wireCount = 0;
    }
  }
}
void middleButton(bool pushed) {
  if (pushed) {
    lcd.clear();
    switch (menuState) {
      case START:
        menuState = LENGTH;
      case CUTTING:
        break;
      case LENGTH:
        menuState = COUNT;
        break;
      case COUNT:
        menuState = CUTTING;
        break;
    }
  }
}
void rightButton(bool pushed) {
  if (pushed) {
    lcd.clear();
    switch (menuState) {
      case START:
      case CUTTING:
        break;
      case LENGTH:
        wireLength++;
        break;
      case COUNT:
        wireCount++;
        break;
    }
  }
}

void leftButtonHold() {
  leftButton(true);
}
void middleButtonHold() {
}
void rightButtonHold() {
  rightButton(true);
}
