#ifndef PROCESSORMONITOR_H
#define PROCESSORMONITOR_H

#include <Wbemidl.h>
#include <string>

class ProcessorMonitor{
public:
    bool init();
    void release();
    std::string queryCIMV2(std::string query, const wchar_t* get);
    int queryWMI();
    int getRAM();

private:
    IWbemLocator *pLoc;
    IEnumWbemClassObject* pEnumerator;
    IWbemServices *pSvcCIMV2, *pSvcWMI;
};

#endif // PROCESSORMONITOR_H
