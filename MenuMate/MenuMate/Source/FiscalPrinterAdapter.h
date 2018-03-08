//---------------------------------------------------------------------------

#ifndef FiscalPrinterAdapterH
#define FiscalPrinterAdapterH
#include "FiscalPrinterWSDL.h"
#include <System.hpp>
#include<list>
#include <memory>
//---------------------------------------------------------------------------
class TFiscalPrinterAdapter
{
    private:
       _di_IWCFServiceFiscalPrinter fiscalClient; // WCF Client
       void initFIClient();


    public:
       TFiscalPrinterAdapter();
       void TestCommunication();
};
#endif
