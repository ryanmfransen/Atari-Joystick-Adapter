/*
 * Author:    Ryan Fransen
 * Created:   February, 10, 2024
 * Version:   1.0
 * 
 * Class: Button
 * 
 * button.hpp
 * 
 * Atari
 * Joystick Adapter
 * 
 * The Button class encapsulates everything needed to support using buttons on
 * your retro joysticks.
 * 
 * This manages:
 * state:         The current state of the button [HIGH or LOW] indicating whether the button is pressed or not.
 * pin:           The pin number on the ESP32 for this button. [D1...Dn] etc
 * state_changed: Indicaters if the state of the button has changed. *
 */

#ifndef BUTTON_HPP_
#define BUTTON_HPP_

#include <Arduino.h>

class Button {
private:
  // Private variables
  volatile uint8_t state;
  volatile uint8_t pin;
  volatile bool state_changed;

  // Private methods
  /**
   * The callback method executed when a button (pin) changes state.
   * Get and store the current state from the pin.
   */
  IRAM_ATTR void instanceIsr(); 

public:
  // Public methods
  /**
   * Retrieve the pin number of the button.
   * 
   * @return the button pin number.
   */
  uint8_t getPin();

  /**
   * Check if the button has changed state.
   * 
   * @return true if the state has changed, false if not.
   */
  bool getStateChanged();

  /**
   * Reset the state of the button.
   */
  void resetStateChanged();

  /**
   * Retrieve the current stored button state.
   * 
   * @return HIGH if the button state is HIGH, or LOW if set to LOW.
   */
  uint8_t getState();

  /**
   * Set the current state of a button by reading the value from the pin.
   */
  void setStateFromPin();

  /**
   * Initialize a button.
   * Set the pin mode, create the interupt callback and set the state to LOW.
   */
  void init();

  // Public constructor
  /**
   * Create an instance of the Button class
   * 
   * @param pin The pin number for the button.
   */
  Button(uint8_t pin);
};
#endif /* BUTTON_HPP_ */