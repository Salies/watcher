#include "NvidiaMonitor.h"

#include <iostream>

bool NvidiaMonitor::init(){
    nvmlReturn_t result;

    result = nvmlInit(); //initializes the API

    if(result != NVML_SUCCESS)
        return false;
    
    unsigned int device_count;
    result = nvmlDeviceGetCount(&device_count); //get number of devices

    if(result != NVML_SUCCESS || device_count < 1) //this version of this software doesn't support multiple devices
        return false;

    result = nvmlDeviceGetHandleByIndex(0, &device);

    if(result != NVML_SUCCESS)
        return false;

    std::cout << "nvidia monitor iniciado com sucesso\n";

    return true;
}

QString NvidiaMonitor::get_device_name(){
    char device_name[NVML_DEVICE_NAME_BUFFER_SIZE];
    nvmlReturn_t result = nvmlDeviceGetName(device, device_name, NVML_DEVICE_NAME_BUFFER_SIZE);

    if(result != NVML_SUCCESS)
        return "ERR";

    return QString(device_name);
}

int NvidiaMonitor::getDeviceUtilization(){
    nvmlUtilization_st device_utilization;
    nvmlReturn_t result;

    result = nvmlDeviceGetUtilizationRates(device, &device_utilization);

    if (result != NVML_SUCCESS)
        return -1;

    return (int) device_utilization.gpu;
}

int NvidiaMonitor::getDeviceTemperature(){
    nvmlReturn_t result;

    unsigned int device_temperature;
    result = nvmlDeviceGetTemperature(device, NVML_TEMPERATURE_GPU, &device_temperature);

    if (result != NVML_SUCCESS)
        return -1;

    return (int) device_temperature;
}

/*
References:
https://docs.nvidia.com/deploy/nvml-api/group__nvmlDeviceQueries.html#group__nvmlDeviceQueries (index: https://docs.nvidia.com/deploy/nvml-api/index.html)
https://gist.github.com/sakamoto-poteko/44d6cd19552fa7721b99
*/