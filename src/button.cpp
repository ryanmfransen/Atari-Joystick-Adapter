/*
 * Author:    Ryan Fransen
 * Created:   February, 10, 2024
 * Version:   1.0
 * 
 * button.cpp
 */

#include "FunctionalInterrupt.h"
#include "button.hpp"

// Constructor
Button::Button(uint8_t pin) :
  pin{pin} {}

void Button::init() {
  // Set the pin to input and use a pullup resistor
  pinMode(pin, INPUT_PULLUP);

  // Set the current state of the pin
  this->state = LOW;

  // Let's attach the interrupt handler
  std::function<void(void)> stdFunct {std::bind(&Button::instanceIsr, this)};
  attachInterrupt(digitalPinToInterrupt(this->pin), stdFunct, CHANGE);
}

void Button::setStateFromPin() {
  // Read in the current pin state
  this->state = digitalRead(this->pin);

  // Set the button state changed flag
  this->state_changed = true;
}

void Button::resetStateChanged() {
  // Clear the state changed flag
  this->state_changed = false;
};

uint8_t Button::getState() {
  return this->state;
};

bool Button::getStateChanged() {
  return this->state_changed;
};

uint8_t Button::getPin() {
  return this->pin;
};

IRAM_ATTR void Button::instanceIsr() {
  // All we need to do in the isr is set the current state
  this->setStateFromPin();
}