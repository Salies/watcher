#ifndef NVIDIAMONITOR_H
#define NVIDIAMONITOR_H

#include <string>
#include "nvml.h"

class NvidiaMonitor{
public:
	nvmlDevice_t device;
	bool init(); //initializes the NVML service/connection and gathers some data about the devices (number, names)
	int getDeviceUtilization(); //get the GPU utilization percentage
	int getDeviceTemperature(); //get the GPU temperature in Celsius degrees
    std::string get_device_name(); //returns the name of the device
};
#endif //NVIDIAMONITOR_H

/*
get_device_name(): might be changed to QMap later to support multiple devices
*/
