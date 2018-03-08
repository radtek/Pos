//---------------------------------------------------------------------------


#pragma hdrstop

#include "FiscalPrinterAdapter.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TFiscalPrinterAdapter::TFiscalPrinterAdapter()
{
    initFIClient();
}
//---------------------------------------------------------------------------
void TFiscalPrinterAdapter::initFIClient()
{
    bool useWSDL = false;
    AnsiString fiscalURL = AnsiString("http://localhost:8744/MenumateServices/FiscalPrinter/");

    fiscalClient = GetIWCFServiceFiscalPrinter(
                            useWSDL, fiscalURL, NULL );
}
//---------------------------------------------------------------------------
void TFiscalPrinterAdapter::TestCommunication()
{
    FiscalDataDetails *dataDetails = new FiscalDataDetails();
    dataDetails->Billno = 555;
    fiscalClient->PrintFiscalReceipt(dataDetails);
}
