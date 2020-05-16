#ifndef NVIDIAMONITOR_H
#define NVIDIAMONITOR_H

#include <string>
#include "nvml.h"

class NvidiaMonitor{
public:
	bool init(); //initializes the NVML service/connection and gathers some data about the devices (number, names)
	int getDeviceUtilization(); //get the GPU utilization percentage
	int getDeviceTemperature(); //get the GPU temperature in Celsius degrees
	int getVRAMusage();
    std::string get_device_name(); //returns the name of the device

private:
	nvmlDevice_t device;
};
#endif //NVIDIAMONITOR_H

/*
get_device_name(): might be changed to a map later to support multiple devices
*/
