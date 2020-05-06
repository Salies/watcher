#include "NvidiaMonitor.h"

#include <iostream>

void NvidiaMonitor::init(){
    nvmlReturn_t result;
    
    std::cout << "nvidia monitor iniciado\n";

    result = nvmlInit(); //initializes the API

    /*if(result != NVML_SUCCESS)
        return;*/
    
    unsigned int device_count;
    result = nvmlDeviceGetCount(&device_count); //get number of devices

    /*if(result != NVML_SUCCESS){
        return;
    }*/

    std::cout << "O sistema esta rodando em " << device_count << " placa(s)\n";

    if(device_count < 1) //this version of this software doesn't support multiple devices
        return;

    result = nvmlDeviceGetHandleByIndex(0, &device);

    /*if(result != NVML_SUCCESS)
        return;*/

    char device_name[NVML_DEVICE_NAME_BUFFER_SIZE];
    result = nvmlDeviceGetName(device, device_name, NVML_DEVICE_NAME_BUFFER_SIZE);

    if(result != NVML_SUCCESS)
        return;

    std::cout << "Nome do dispositivo: " << device_name << "\n";

    std::cout << "Uso da GPU: " << getDeviceUtilization() << "%\n";

    std::cout << "Temperatura da GPU: " << getDeviceTemperature() << " graus Celsius\n";
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