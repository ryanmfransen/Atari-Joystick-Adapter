/*
 * Author:    Ryan Fransen
 * Created:   February, 10, 2024
 * Version:   1.0
 * 
 * Class: FireButton
 * 
 * firebutton.hpp
 * 
 * Atari
 * Joystick Adapter
 * 
 * The FireButton class extends the Button class and adds a button id. The button id is used to identify
 * which of the fire buttons it represents.
 * 
 * This manages:
 * id: The button identifer, Button 1, 2, 3 etc. [1...n]
 */

#ifndef FIREBUTTON_HPP_
#define FIREBUTTON_HPP_

#include <Arduino.h>

#include "button.hpp"

class FireButton : public Button {
private:
  // Private variables
  volatile uint8_t id;

public:
  // Public methods

  /**
   * Retrieve the button id.
   * 
   * @return the button id.
   */
  uint8_t getButtonId();

  // Public constructor
  /**
   * Cresate an instance of the FireButton class.
   * 
   * @param pin The pin number for the button
   * @param id The id number of the button
  */
  FireButton(uint8_t pin, u_int8_t id);
};
#endif /* FIREBUTTON_HPP_ */