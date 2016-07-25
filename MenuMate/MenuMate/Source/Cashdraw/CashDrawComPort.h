//---------------------------------------------------------------------------

#ifndef CashDrawComPortH
#define CashDrawComPortH
//---------------------------------------------------------------------------
#include "CashDraw.h"
#include "CPort.hpp"
//---------------------------------------------------------------------------
class TCashDrawComPort : public TCashDraw
{
private:
    TComPort *cashDrawPort;
    char portNumber;
    int portLength;
    bool portHighChars;

public:
    TCashDrawComPort(char inPortNumber, int inPortLength, bool inPortHighChars);
    void OpenDraw();
};
//---------------------------------------------------------------------------
#endif
