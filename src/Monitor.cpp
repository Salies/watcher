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
    
    m.insert(pair<string, string>("cpu_name", processor_monitor.queryCIMV2("SELECT Name FROM Win32_Processor", L"Name")));
    m.insert(pair<string, string>("graphics_name", nvidia_monitor.get_device_name()));

    return m;
}

map<string, int> Monitor::getData(){
    map <string, int>m;

    m.insert(pair<string, int>("graphics_usage", nvidia_monitor.getDeviceUtilization()));
    m.insert(pair<string, int>("graphics_temp", nvidia_monitor.getDeviceTemperature()));
    m.insert(pair<string, int>("vram_usage", nvidia_monitor.getVRAMusage()));
    m.insert(pair<string, int>("cpu_usage", std::stoi(processor_monitor.queryCIMV2("SELECT PercentProcessorTime FROM Win32_PerfFormattedData_PerfOS_Processor WHERE Name='_Total'", L"PercentProcessorTime"))));
    m.insert(pair<string, int>("cpu_temp", processor_monitor.queryWMI()));
    m.insert(pair<string, int>("ram_usage", processor_monitor.getRAM()));

    return m;
}

void Monitor::exit() {
    processor_monitor.release();
}
