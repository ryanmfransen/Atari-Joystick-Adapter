/*
 * Author:    Ryan Fransen
 * Created:   February, 10, 2024
 * Version:   1.0
 * 
 * Atari
 * Joystick Adapter
 * {{UP, RIGHT, DOWN, LEFT}, FIRE}
 * 
 * deviceinfo.hpp
 *
 */

#include "deviceinfo.hpp"

char *DeviceInfo::getName() {
  return (char *)"Atari Joystick";
};

char *DeviceInfo::getManufacturer() {
  return (char *)"12 Oranges Software";
};

char *DeviceInfo::getModelNumber() {
  return (char *)"1.0";
};

char *DeviceInfo::getSoftwareRevision() {
  return  (char *)"Software Revision 1";
};

char *DeviceInfo::getSerialNumber() { 
   return (char *)"1";
};

char *DeviceInfo::getFirmwareRevision() {
  return (char *)"1.0";
};

char *DeviceInfo::getHardwareRevision() {
  return (char *)"1.0";
};

int DeviceInfo::getBbatteryLevel() {
  return 100;
};