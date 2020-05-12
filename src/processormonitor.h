#ifndef PROCESSORMONITOR_H
#define PROCESSORMONITOR_H

#include <Wbemidl.h>

class ProcessorMonitor{
public:
    bool init();
    void release();
    int queryCIMV2();
    int queryWMI();
    int getRAM();

private:
    IWbemLocator *pLoc;
    IEnumWbemClassObject* pEnumerator;
    IWbemServices *pSvcCIMV2, *pSvcWMI;
};

#endif // PROCESSORMONITOR_H
