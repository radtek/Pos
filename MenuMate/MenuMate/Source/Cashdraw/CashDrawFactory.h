//---------------------------------------------------------------------------

#ifndef CashDrawFactoryH
#define CashDrawFactoryH
//---------------------------------------------------------------------------

#include "CashDraw.h"
#include "CashDrawerPortsList.h"
#include "PrinterPhysical.h"

class TCashDrawFactory
{
public:
    static TCashDraw* CreateCashDraw( CashDrawerPorts selectedCashDrawPort, int inPortLength, bool inPortHighChars, TPrinterPhysical& inReceiptPrinter );
};
#endif
