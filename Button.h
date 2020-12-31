#pragma once

class Button {
  private:
    int pin;
    int value = 0;
    void (*onChange)(bool);
    void (*onHold)();
    long lastPushed = 0;
    bool notHeldThisLoop = true;

  public:
    Button(int pin, void (*onChange)(bool), void (*onHold)()) {
      this->pin = pin;
      this->onChange = onChange;
      this->onHold = onHold;

      pinMode(pin, INPUT);
      this->value = digitalRead(pin);
    }

    void tick() {
      int newValue = digitalRead(pin);
      if (newValue != this->value) {
        this->onChange(newValue == 1);
        lastPushed = millis();
        this->value = newValue;
      }

      // when held for more than 500 ms, with at least 100ms between, run onHold
      if (this->isPushed() && millis() - lastPushed > 500) {
        if ((millis() - lastPushed) % 100 < 5 && this->notHeldThisLoop) {
          this->onHold();
          this->notHeldThisLoop = false;
        } else {
          this->notHeldThisLoop = true;
        }
      }
    }

    boolean isPushed() {
      return this->value == 1;
    }
};
