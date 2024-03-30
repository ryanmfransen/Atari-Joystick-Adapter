/*
 * Author:    Ryan Fransen
 * Created:   February, 10, 2024
 * Version:   1.0
 * 
 * deviceinfo.hpp
 */

#ifndef DEVICEINFO_HPP_
#define DEVICEINFO_HPP_

class DeviceInfo {
public:
  // Public Methods
  static char *getName();
  static char *getManufacturer();
  static char *getModelNumber();
  static char *getSoftwareRevision();
  static char *getSerialNumber();
  static char *getFirmwareRevision();
  static char *getHardwareRevision();
  static int getBbatteryLevel();
};

#endif /* DEVICEINFO_HPP_ */