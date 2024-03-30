/*
 * Author:    Ryan Fransen
 * Created:   February, 10, 2024
 * Version:   1.0
 * 
 * Atari
 * Joystick Adapter
 * {{UP, RIGHT, DOWN, LEFT}, FIRE, (Additional Buttons 2, 3, 4)}
 * 
 * main.cpp
 *
 * 
 * Atari Joystick Adapter. This program reads in the button states of an Atari joystick and
 * converts those button presses into a Bluetooth gamepad controller.
 * 
 * The four directions on the joystick are mapped to the gamepads D-pad buttons, or hats.
 * The fire button maps to the gamepad button 1. Three additional buttons are included to manage
 * additional buttons used for the emulator and can be used to control buttons on the game system itself.
 * 
 * The program uses hardware interupts to detect button presses.
 * 
 */

#include <Arduino.h>
#include <BleGamepad.h>

#include "deviceinfo.hpp"
#include "button.hpp"
#include "firebutton.hpp"

// Joystick Fire Button id's
uint8_t joystick_fire_button_1 = 1;
uint8_t joystick_fire_button_2 = 2;
uint8_t joystick_fire_button_3 = 3;
uint8_t joystick_fire_button_4 = 4;

// Joystick direction GPIO Pins
const int joystick_up_pin    = D1;
const int joystick_right_pin = D2;
const int joystick_down_pin  = D3;
const int joystick_left_pin  = D4;

// Joystick Fire Button GPIO Pins
const int joystick_fire_button_pin_1 = D5;
const int joystick_fire_button_pin_2 = D6;
const int joystick_fire_button_pin_3 = D7;
const int joystick_fire_button_pin_4 = D8;

// Battery level
const int battery_level = 100;

// Joystick d-pad hat state map. We'll use this to determine the hat value to send.
// based on the d-pad switch states
int hat_map[2][2][2][2];

// Let's map the button indexes
const uint8_t BUTTON_UP    = 0;
const uint8_t BUTTON_RIGHT = 1;
const uint8_t BUTTON_DOWN  = 2;
const uint8_t BUTTON_LEFT  = 3;

// D-Pad buttons
std::vector<Button> joystick_direction_buttons = {
  Button(joystick_up_pin),
  Button(joystick_right_pin),
  Button(joystick_down_pin),
  Button(joystick_left_pin)
};

// Fire buttons
std::vector<FireButton> joystick_fire_buttons = {
  FireButton(joystick_fire_button_pin_1, joystick_fire_button_1),
  FireButton(joystick_fire_button_pin_2, joystick_fire_button_2),
  FireButton(joystick_fire_button_pin_3, joystick_fire_button_3),
  FireButton(joystick_fire_button_pin_4, joystick_fire_button_4)
};

// Gamepad constructor
BleGamepad gamepad(
  DeviceInfo::getName(), 
  DeviceInfo::getManufacturer(), 
  DeviceInfo::getBbatteryLevel());

/*
 * Function: set_hat_map
 * ---------------------
 * Initializes the hat map. The hat map is used to set the value to send for each of the hat button values.
 * We send a single byte value which represents the state of the hat, including all four hat buttons.
 * 
 * We'll use a four dimension two element array to map the values.  This makes it easy to
 *  take the pin values from the four direction buttons to determine the hat value to send, 0-8, per below.
 * 
 * In order: CENTERED[0], UP[1], UP_RIGHT[2], RIGHT[3], DOWN_RIGHT[4], DOWN[5], DOWN_LEFT[6], LEFT[7], UP_LEFT[8]
 * 
 * LEFT_UP:8     UP:1          UP_RIGHT:2 
 * ⎡    ▲    ⎤   ⎡    ▲    ⎤   ⎡    ▲    ⎤
 * ⎢    0    ⎟   ⎢    0    ⎟   ⎢    0    ⎟
 * ⎢◀︎ 0   1 ▷⎟   ⎢◁ 1   1 ▷⎟   ⎢◁ 1   0 ▶︎⎟
 * ⎢    1    ⎟   ⎢    1    ⎟   ⎢    1    ⎟ 
 * ⎣    ▽    ⎦   ⎣    ▽    ⎦   ⎣    ▽    ⎦ 
 * 
 * LEFT:7        CENTERED:0    RIGHT:3
 * ⎡    △    ⎤   ⎡    △    ⎤   ⎡    △    ⎤
 * ⎢    1    ⎟   ⎢    1    ⎟   ⎢    1    ⎟
 * ⎢◀︎ 0   1 ▷⎟   ⎢◁ 1   1 ▷⎟   ⎢◁ 1   0 ▶︎⎟
 * ⎢    1    ⎟   ⎢    1    ⎟   ⎢    1    ⎟
 * ⎣    ▽    ⎦   ⎣    ▽    ⎦   ⎣    ▽    ⎦
 * 
 * DOWN_LEFT:6   DOWN:5        DOWN_RIGHT:4
 * ⎡    △    ⎤   ⎡    △    ⎤   ⎡    △    ⎤
 * ⎢    1    ⎟   ⎢    1    ⎟   ⎢    1    ⎟ 
 * ⎢◀︎ 0   1 ▷⎟   ⎢◁ 1   1 ▷⎟   ⎢◁ 1   0 ▶︎⎟
 * ⎢    0    ⎟   ⎢    0    ⎟   ⎢    0    ⎟
 * ⎣    ▼    ⎦   ⎣    ▼    ⎦   ⎣    ▼    ⎦
 * 
*/
void set_hat_map() {
  hat_map[1][1][1][1] = HAT_CENTERED;   // △ ▷ ▽ ◁
  hat_map[0][1][1][1] = HAT_UP;         // ▲ ▷ ▽ ◁ 
  hat_map[0][0][1][1] = HAT_UP_RIGHT;   // ▲ ▶︎ ▽ ◁ 
  hat_map[1][0][1][1] = HAT_RIGHT;      // △ ▶︎ ▽ ◁ 
  hat_map[1][0][0][1] = HAT_DOWN_RIGHT; // △ ▼ ▶︎ ◁ 
  hat_map[1][1][0][1] = HAT_DOWN;       // △ ▷ ▼ ◁ 
  hat_map[1][1][0][0] = HAT_DOWN_LEFT;  // △ ▷ ▼ ◀︎
  hat_map[1][1][1][0] = HAT_LEFT;       // △ ▷ ▽ ◀︎
  hat_map[0][1][1][0] = HAT_UP_LEFT;    // ▲ ▷ ▽ ◀︎
}

/*
 * Function: configureGamepad
 * --------------------------
 * Configures the bluetooth gamepad instance. 
 * 
*/
void configureGamepad(BleGamepadConfiguration *gamepad_config) {
  // Set the controller type
  gamepad_config->setControllerType(CONTROLLER_TYPE_GAMEPAD);

  // Auto sending report, instead of manual report
  gamepad_config->setAutoReport(true);

  // Set the button count
  gamepad_config->setButtonCount(joystick_fire_buttons.size());

  // Set the count, d-pads are configured as hats in the library
  gamepad_config->setHatSwitchCount(1);

  // Configure the device settings
  gamepad_config->setModelNumber(DeviceInfo::getModelNumber());
  gamepad_config->setSoftwareRevision(DeviceInfo::getSoftwareRevision());
  gamepad_config->setSerialNumber(DeviceInfo::getSerialNumber());
  gamepad_config->setFirmwareRevision(DeviceInfo::getFirmwareRevision());
  gamepad_config->setHardwareRevision(DeviceInfo::getHardwareRevision());
}

/*
 * Function: setup
 * ---------------
 * The main application function.  Executes once at startup.
 * 
*/
void setup() {
  // Initialize the direction Button instances
  for (Button &button : joystick_direction_buttons) {
    button.init();
  }

  // Initialize the Button instances
  for (FireButton &button : joystick_fire_buttons) {
    button.init();
  }

  // Set the hat map values
  set_hat_map();

  BleGamepadConfiguration gamepad_config;

  // Configure the gamepad
  configureGamepad(&gamepad_config);

  // Start the Task service
  gamepad.begin(&gamepad_config);
}

/*
 * Function: loop
 * --------------
 * The main application service loop
 * 
*/
void loop() {

  /* The main loop of the program.
   * All we do in here is check if the joystick button state flags are marked as changed.
   * We maintain state inside the button objects, then when those states are detected in here,
   * we will update the gamepad controller accordingly, and reset the state changed flag. This ensures
   * we are not continuously reading the digital pins and sending data over bluetooth, as we only want to
   * send data when something has changed, either a button was pressed or a button was released.
  */
  if (gamepad.isConnected()) {  
    // Check if the joystick fire buttons have changed state
    for (FireButton &button : joystick_fire_buttons) {
      if (button.getStateChanged()) {
        button.resetStateChanged();
        if (button.getState()) {
          gamepad.release(button.getButtonId());
        } else {
          gamepad.press(button.getButtonId());
        }
      }
    }

    // Check if the joystick direction buttons have changed state
    for (Button &button : joystick_direction_buttons) {
      if (button.getStateChanged()) {
        button.resetStateChanged();
        gamepad.setHat1(hat_map
          [joystick_direction_buttons[BUTTON_UP].getState()]
          [joystick_direction_buttons[BUTTON_RIGHT].getState()]
          [joystick_direction_buttons[BUTTON_DOWN].getState()]
          [joystick_direction_buttons[BUTTON_LEFT].getState()]
        );
      }
    }
  }
}