#ifndef PROCESSORMONITOR_H
#define PROCESSORMONITOR_H

#include <QObject>

#include <Wbemidl.h>

class ProcessorMonitor : public QObject {
    Q_OBJECT
public:
    IWbemLocator *pLoc;
    IEnumWbemClassObject* pEnumerator;
    IWbemServices *pSvcCIMV2;
    bool init();
private:
    void release();
    bool queryCIMV2();
};

#endif // PROCESSORMONITOR_H
