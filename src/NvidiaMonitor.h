#ifndef NVIDIAMONITOR_H
#define NVIDIAMONITOR_H

#include <QObject>
#include "nvml.h"

class NvidiaMonitor : public QObject {
	Q_OBJECT
public:
	nvmlDevice_t device;
	void init(); //initializes the NVML service/connection and gathers some data about the devices (number, names)
private:
	int getDeviceUtilization(); //get the GPU utilization percentage
	int getDeviceTemperature(); //get the GPU temperature in Celsius degrees
//signals:	
};
#endif //NVIDIAMONITOR_H