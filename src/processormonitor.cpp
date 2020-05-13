#define _WIN32_DCOM
#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>
#include <Windows.h>
#include <vector>

#pragma comment(lib, "wbemuuid.lib")

#include "processormonitor.h"

using std::cout;
using std::hex;
using std::endl;

bool ProcessorMonitor::init(){
    HRESULT hres;

    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres))
    {
        cout << "Failed to initialize COM library. Error code = 0x"
            << hex << hres << endl;
        return 1;                  // Program has failed.
    }

    hres = CoInitializeSecurity(
        NULL,
        -1,                          // COM authentication
        NULL,                        // Authentication services
        NULL,                        // Reserved
        RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
        RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
        NULL,                        // Authentication info
        EOAC_NONE,                   // Additional capabilities 
        NULL                         // Reserved
    );


    if (FAILED(hres))
    {
        cout << "Failed to initialize security. Error code = 0x"
            << hex << hres << endl;
        CoUninitialize();
        return 1;                    // Program has failed.
    }

    pLoc = NULL;

    hres = CoCreateInstance(
        CLSID_WbemLocator,             
        0, 
        CLSCTX_INPROC_SERVER, 
        IID_IWbemLocator, (LPVOID *) &pLoc);
 
    if (FAILED(hres))
    {
        cout << "Failed to create IWbemLocator object."
            << " Err code = 0x"
            << hex << hres << endl;
        CoUninitialize();
        return false;
    }

    /*
        connect CIMV2
    */
    pSvcCIMV2 = NULL;

    hres = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &pSvcCIMV2);
    
    if (FAILED(hres))
    {
        cout << "Could not connect. Error code = 0x" << hex << hres << endl;
        pLoc->Release();     
        CoUninitialize();
        return false;
    }

    cout << "Connected to ROOT\\CIMV2 WMI namespace" << endl;

    hres = CoSetProxyBlanket(pSvcCIMV2, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);

    if (FAILED(hres))
    {
        cout << "Could not set proxy blanket. Error code = 0x" << hex << hres << endl;
        pSvcCIMV2->Release();
        pLoc->Release();     
        CoUninitialize();
        return false;
    }

    /*
        connect WMI
    */
    pSvcWMI = NULL;

    hres = pLoc->ConnectServer(_bstr_t(L"ROOT\\WMI"), NULL, NULL, 0, NULL, 0, 0, &pSvcWMI);
    
    if (FAILED(hres))
    {
        cout << "Could not connect. Error code = 0x" << hex << hres << endl;
        pLoc->Release();     
        CoUninitialize();
        return false;
    }

    cout << "Connected to ROOT\\WMI WMI namespace" << endl;

    hres = CoSetProxyBlanket(pSvcWMI, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);

    if (FAILED(hres))
    {
        cout << "Could not set proxy blanket. Error code = 0x" << hex << hres << endl;
        pSvcWMI->Release();
        pLoc->Release();     
        CoUninitialize();
        return false;
    }

    return true;
}

void ProcessorMonitor::release(){
    pSvcCIMV2->Release();
    pSvcWMI->Release();
    pLoc->Release();
    pEnumerator->Release();
    CoUninitialize();
    cout << "released";
}

std::string ProcessorMonitor::queryCIMV2(std::string query, const wchar_t* get){
    pEnumerator = NULL;
    HRESULT hres = pSvcCIMV2->ExecQuery(
        bstr_t("WQL"), 
        bstr_t(query.c_str()),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator);
    
    if (FAILED(hres))
    {
        cout << "Query for operating system name failed."
            << " Error code = 0x" 
            << hex << hres << endl;
        pSvcCIMV2->Release();
        pLoc->Release();
        CoUninitialize();
        return "-1";
    }
 
    IWbemClassObject *pclsObj = NULL;
    ULONG uReturn = 0;
   
    BSTR usage = NULL;
    while (pEnumerator)
    {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, 
            &pclsObj, &uReturn);

        if(0 == uReturn)
        {
            break;
        }

        VARIANT vtProp;

        hr = pclsObj->Get(get, 0, &vtProp, 0, 0);
        usage = vtProp.bstrVal;
        VariantClear(&vtProp);

        pclsObj->Release();
    }

    char* converted = _com_util::ConvertBSTRToString(usage);

    return std::string(converted);
}

int ProcessorMonitor::queryWMI(){
    HRESULT hr;
    pEnumerator = NULL;
    IWbemClassObject* pclsObj = NULL;
    ULONG uReturn = 0;

    HRESULT hres = pSvcWMI->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT CurrentTemperature FROM MSAcpi_ThermalZoneTemperature"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator);

    if (FAILED(hres))
    {
        cout << "Query for operating system name failed."
            << " Error code = 0x"
            << hex << hres << endl;
        pSvcWMI->Release();
        pLoc->Release();
        CoUninitialize();
        return -1;
    }

    std::vector<int> temps;
    while (pEnumerator)
    {
        hr = pEnumerator->Next(WBEM_INFINITE, 1,
            &pclsObj, &uReturn);

        if (0 == uReturn)
        {
            break;
        }

        VARIANT vtProp;

        hr = pclsObj->Get(L"CurrentTemperature", 0, &vtProp, 0, 0);
        temps.push_back(vtProp.uintVal);
        VariantClear(&vtProp);

        pclsObj->Release();
    }

    int mediumTemp = 0;
    for (std::vector<int>::iterator it = temps.begin(); it != temps.end(); it++)
        mediumTemp += *it;
    mediumTemp = mediumTemp / temps.size();

    return (mediumTemp / 10) - 273; //273.15 would be the more accurate approximation, but it's too ugly for the program XD
}

int ProcessorMonitor::getRAM() {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
    DWORDLONG physMemUsed = totalPhysMem - memInfo.ullAvailPhys;

    return int((100 * physMemUsed) / totalPhysMem);
}

/*
Reference: https://docs.microsoft.com/en-us/windows/win32/wmisdk/example--getting-wmi-data-from-the-local-computer
*/
