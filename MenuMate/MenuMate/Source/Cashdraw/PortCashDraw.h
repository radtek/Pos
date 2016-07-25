//---------------------------------------------------------------------------

#ifndef PortCashDrawH
#define PortCashDrawH
//---------------------------------------------------------------------------
#include "CashDraw.h"
#include "PortTalk.h"
//---------------------------------------------------------------------------
class TPortCashDraw : public TCashDraw
{
protected:
    virtual void SendOpenDrawCommands(TPortTalk* PortTalk) =0;
public:
    void OpenDraw();
};
//---------------------------------------------------------------------------

class TCashDrawJavlin : public TPortCashDraw
{
protected:
    void SendOpenDrawCommands(TPortTalk* PortTalk);
};
//---------------------------------------------------------------------------

class TCashDrawGladius : public TPortCashDraw
{
protected:
    void SendOpenDrawCommands(TPortTalk* PortTalk);
};
//---------------------------------------------------------------------------

class TCashDrawPT6200 : public TPortCashDraw
{
protected:
    void SendOpenDrawCommands(TPortTalk* PortTalk);
};
//---------------------------------------------------------------------------

class TCashDrawWillPosA10 : public TPortCashDraw
{
protected:
    void SendOpenDrawCommands(TPortTalk* PortTalk);
};
//---------------------------------------------------------------------------

class TCashDrawWillPosA20 : public TPortCashDraw
{
protected:
    void SendOpenDrawCommands(TPortTalk* PortTalk);
};
//---------------------------------------------------------------------------

class TCashDrawFlytechCD4B8B78 : public TPortCashDraw
{
protected:
    void SendOpenDrawCommands(TPortTalk* PortTalk);
};
//---------------------------------------------------------------------------

class TCashDrawFlytechCDB4B8x2 : public TPortCashDraw
{
protected:
    void SendOpenDrawCommands(TPortTalk* PortTalk);
};
//---------------------------------------------------------------------------

class TCashDrawProtechPanelPc : public TPortCashDraw
{
protected:
    void SendOpenDrawCommands(TPortTalk* PortTalk);
};
//---------------------------------------------------------------------------

class TCashDrawProtechPS3100 : public TPortCashDraw
{
protected:
    void SendOpenDrawCommands(TPortTalk* PortTalk);
};
//---------------------------------------------------------------------------

class TCashDrawVPos485D36 : public TPortCashDraw
{
protected:
    void SendOpenDrawCommands(TPortTalk* PortTalk);
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

class TCashDrawVPos335 : public TPortCashDraw
{
protected:
    void SendOpenDrawCommands(TPortTalk* PortTalk);
};
//---------------------------------------------------------------------------

class TCashDrawEbnXPos752 : public TPortCashDraw
{
protected:
    void SendOpenDrawCommands(TPortTalk* PortTalk);
};
//---------------------------------------------------------------------------

class TCashDrawToripos815 : public TPortCashDraw
{
protected:
    void SendOpenDrawCommands(TPortTalk* PortTalk);
};
//---------------------------------------------------------------------------

class TCashDrawAerpos3435 : public TPortCashDraw
{
protected:
    void SendOpenDrawCommands(TPortTalk* PortTalk);
};
//---------------------------------------------------------------------------
#endif
