//---------------------------------------------------------------------------

#ifndef PrinterCashDrawH
#define PrinterCashDrawH
//---------------------------------------------------------------------------
#include "CashDraw.h"
#include "PortTalk.h"
#include "PrinterPhysical.h"
#include "Printout.h"
//---------------------------------------------------------------------------
class TPrinterCashDraw : public TCashDraw
{
protected:
    virtual bool LoadPrinter(TPrinterPhysical& Printer) =0;
    virtual void SendOpenDrawCommands(TPrintout* Printout) =0;

public:
    void OpenDraw();
};
//---------------------------------------------------------------------------

class TCashDrawReceiptPrinter : public TPrinterCashDraw
{
private:
    TPrinterPhysical ReceiptPrinter;

protected:
    bool LoadPrinter(TPrinterPhysical& Printer);
    void SendOpenDrawCommands(TPrintout* Printout);

public:
    TCashDrawReceiptPrinter(TPrinterPhysical& inReceiptPrinter);
};
//---------------------------------------------------------------------------

class TCashDrawCashDrawPrinter : public TPrinterCashDraw
{
private:
    int PortLength;

protected:
    bool LoadPrinter(TPrinterPhysical& Printer);
    void SendOpenDrawCommands(TPrintout* Printout);

public:
    TCashDrawCashDrawPrinter(int inPortLength);
};
//---------------------------------------------------------------------------
#endif
