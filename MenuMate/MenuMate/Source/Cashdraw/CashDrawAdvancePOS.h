//---------------------------------------------------------------------------

#ifndef CashDrawAdvancePOSH
#define CashDrawAdvancePOSH
//---------------------------------------------------------------------------
#include "CashDraw.h"
#include "MallExportIMall.h"
//---------------------------------------------------------------------------
class TCashDrawAdvancePOS : public TCashDraw
{
public:
    void OpenDraw();
    void makeLogFile(UnicodeString str);
};
//---------------------------------------------------------------------------
#endif
