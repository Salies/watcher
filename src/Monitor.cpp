#include "Monitor.h"
#include "NvidiaMonitor.h"
#include "processormonitor.h"

#include <QDebug>
#include <QString>

NvidiaMonitor nvidia_monitor;
ProcessorMonitor processor_monitor;

QVariantMap Monitor::init(){
    QMap <QString, QVariant>m;

    nvidia_monitor.init();
    processor_monitor.init();
    
    m.insert("graphics_name", nvidia_monitor.get_device_name());

    return QVariantMap(m);
}

QVariantMap Monitor::getData(){
    QMap <QString, QVariant>m;
    qDebug() << "pegando dados por segundo";

    m.insert("graphics_usage", nvidia_monitor.getDeviceUtilization());
    m.insert("graphics_temp", nvidia_monitor.getDeviceTemperature());
    m.insert("cpu_usage", processor_monitor.queryCIMV2());
    m.insert("cpu_temp", processor_monitor.queryWMI());

    qDebug() << m;

    return QVariantMap(m);
}
