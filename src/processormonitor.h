#ifndef PROCESSORMONITOR_H
#define PROCESSORMONITOR_H

#include <QObject>

#include <Wbemidl.h>

class ProcessorMonitor : public QObject {
    Q_OBJECT
public:
    bool init();
    void release();
    int queryCIMV2();
    int queryWMI();

private:
    IWbemLocator *pLoc;
    IEnumWbemClassObject* pEnumerator;
    IWbemServices *pSvcCIMV2, *pSvcWMI;
};

#endif // PROCESSORMONITOR_H
