/*
 * Author:    Ryan Fransen
 * Created:   February, 10, 2024
 * Version:   1.0
 * 
 * firebutton.cpp
 */

#include "firebutton.hpp"

// Constructor
FireButton::FireButton(uint8_t pin, u_int8_t id) :
  Button{pin},
  id{id} {}

uint8_t FireButton::getButtonId() {
  return this->id;
};