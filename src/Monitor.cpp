#include "Monitor.h"
#include "NvidiaMonitor.h"
#include "processormonitor.h"

NvidiaMonitor nvidia_monitor;
ProcessorMonitor processor_monitor;

using namespace std;

map<string, string> Monitor::init(){
    map <string, string>m;

    nvidia_monitor.init();
    processor_monitor.init();
    
    m.insert(pair<string, string>("graphics_name", nvidia_monitor.get_device_name()));

    return m;
}

map<string, int> Monitor::getData(){
    map <string, int>m;

    m.insert(pair<string, int>("graphics_usage", nvidia_monitor.getDeviceUtilization()));
    m.insert(pair<string, int>("graphics_temp", nvidia_monitor.getDeviceTemperature()));
    m.insert(pair<string, int>("cpu_usage", processor_monitor.queryCIMV2()));
    m.insert(pair<string, int>("cpu_temp", processor_monitor.queryWMI()));
    m.insert(pair<string, int>("ram_usage", processor_monitor.getRAM()));

    return m;
}

void Monitor::exit() {
    processor_monitor.release();
}
