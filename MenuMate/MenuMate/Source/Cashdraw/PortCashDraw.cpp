//---------------------------------------------------------------------------


#pragma hdrstop

#include <memory>

#include "PortCashDraw.h"
#include "MMLogging.h"


#pragma package(smart_init)

//---------------------------------------------------------------------------

/**
 * Open the cash draw using the cash draw port on the terminal.
 */
void TPortCashDraw::OpenDraw()
{
    try
    {
        std::auto_ptr<TPortTalk> PortTalk(new TPortTalk);
        if(PortTalk->Initialise())
        {
            makeLogFile("Opening cash drawer Class - TPortCashDraw");
            SendOpenDrawCommands(PortTalk.get());
        }
    }
    catch(Exception &Err)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"Error Opening Cash Drawer." + Err.Message);
    }
}

//---------------------------------------------------------------------------
// TCashDrawJavlin
//---------------------------------------------------------------------------

void TCashDrawJavlin::SendOpenDrawCommands(TPortTalk* PortTalk)
{
    PortTalk->OutPort(0x04B8, 0xF1);
    //wait 200 ms
    ::Sleep(200);
    PortTalk->OutPort(0x04B8, 0xF0);

    PortTalk->OutPort(0x04B8, 0xF2);
    //wait 200 ms
    ::Sleep(200);
    PortTalk->OutPort(0x04B8, 0xF0);
}

//---------------------------------------------------------------------------
// TCashDrawGladius
//---------------------------------------------------------------------------

void TCashDrawGladius::SendOpenDrawCommands(TPortTalk* PortTalk)
{
    PortTalk->OutPort(0x240, 0xF1);
    //wait 200 ms
    ::Sleep(200);
    PortTalk->OutPort(0x240, 0xF0);

    PortTalk->OutPort(0x240, 0xF2);
    //wait 200 ms
    ::Sleep(200);
    PortTalk->OutPort(0x240, 0xF0);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// TCashDrawPT6200
//---------------------------------------------------------------------------

void TCashDrawPT6200::SendOpenDrawCommands(TPortTalk* PortTalk)
{
    PortTalk->OutPort(0x40B9, 0xFF);
    ::Sleep(10);
    PortTalk->OutPort(0x40B9, 0xF7);
    ::Sleep(10);
    PortTalk->OutPort(0x40B9, 0xFF);
    ::Sleep(10);
}

//---------------------------------------------------------------------------
// TCashDrawWillPosA10
//---------------------------------------------------------------------------

void TCashDrawWillPosA10::SendOpenDrawCommands(TPortTalk* PortTalk)
{
    PortTalk->OutPort(0x044D, 0x40);
    ::Sleep(500);
    PortTalk->OutPort(0x044D, 0x38);
    ::Sleep(1000);

    PortTalk->OutPort(0x044D, 0x80);
    ::Sleep(500);
    PortTalk->OutPort(0x044D, 0x38);
    ::Sleep(500);
}

//---------------------------------------------------------------------------
// TCashDrawWillPosA20
//---------------------------------------------------------------------------

void TCashDrawWillPosA20::SendOpenDrawCommands(TPortTalk* PortTalk)
{
    PortTalk->OutPort(0x048D, 0x10);
    ::Sleep(500);
    PortTalk->OutPort(0x048D, 0x0E);
    ::Sleep(1000);
    PortTalk->OutPort(0x048D, 0x20);
    ::Sleep(500);
    PortTalk->OutPort(0x048D, 0x0E);
    ::Sleep(1000);
}

//---------------------------------------------------------------------------
// TCashDrawFlytechCD4B8B78
//---------------------------------------------------------------------------

void TCashDrawFlytechCD4B8B78::SendOpenDrawCommands(TPortTalk* PortTalk)
{
    PortTalk->OutPort(0x04B8, 0x06);
    //wait 200 ms
    ::Sleep(300);
    PortTalk->OutPort(0x04B8, 0x00);
}

//---------------------------------------------------------------------------
// TCashDrawFlytechCDB4B8x2
//---------------------------------------------------------------------------

void TCashDrawFlytechCDB4B8x2::SendOpenDrawCommands(TPortTalk* PortTalk)
{
    // Fire both ports.
    PortTalk->OutPort(0x04B8, 0x0F);
    ::Sleep(300);
    PortTalk->OutPort(0x04B8, 0x00);
}

//---------------------------------------------------------------------------
// TCashDrawProtechPanelPc
//---------------------------------------------------------------------------

void TCashDrawProtechPanelPc::SendOpenDrawCommands(TPortTalk* PortTalk)
{
    // Fire both ports.
    PortTalk->OutPort(0x048F, 0x03);
    ::Sleep(300);
    PortTalk->OutPort(0x048F, 0x0B);
}

//---------------------------------------------------------------------------
// TCashDrawProtechPS3100
//---------------------------------------------------------------------------

void TCashDrawProtechPS3100::SendOpenDrawCommands(TPortTalk* PortTalk)
{
    // Fire both ports.
    PortTalk->OutPort(0x048F, 0x40); // 64 decimal.
    ::Sleep(300);
    PortTalk->OutPort(0x048F, 0x02);
}

//---------------------------------------------------------------------------
// TCashDrawVPos335
//---------------------------------------------------------------------------

void TCashDrawVPos335::SendOpenDrawCommands(TPortTalk* PortTalk)
{
    // Fire both ports.
    PortTalk->OutPort(0x048C, 0x0C);
    ::Sleep(300);
    PortTalk->OutPort(0x048C, 0x00);
}
 //---------------------------------------------------------------------------
// TCashDrawVPos485 D36 MotherBoard
//---------------------------------------------------------------------------

void TCashDrawVPos485D36::SendOpenDrawCommands(TPortTalk* PortTalk)
{
    // Fire both ports.
    PortTalk->OutPort(0x0482, 0x10);
    ::Sleep(300);
    PortTalk->OutPort(0x0482, 0x00);
}

//---------------------------------------------------------------------------
// TCashDrawEbnXPos752
//---------------------------------------------------------------------------

void TCashDrawEbnXPos752::SendOpenDrawCommands(TPortTalk* PortTalk)
{
    // The EBN has only one cash-drawer port.
    // The specification states that bit 4 must be sent to kick.
    PortTalk->OutPort(0x280, 0x08);
    // Waiting 300ms as a precaution - just to let the IO filter through.
    ::Sleep(300);
    // And close - we don't want the volts flying down forever!?
    PortTalk->OutPort(0x280, 0x00);
}

//---------------------------------------------------------------------------
// TCashDrawToripos815
//---------------------------------------------------------------------------

void TCashDrawToripos815::SendOpenDrawCommands(TPortTalk* PortTalk)
{
    /*
        Register location : A24
        Size              : 8bit
        Open command      : 40 ( sets the 6th bit to true )
    */

    // open drawer
    PortTalk->OutPort(0xA24, 0x40);

    // wait
    ::Sleep(300);

    // close drawer command must be set before manually closing drawer
    PortTalk->OutPort(0xA24, 0x00);
}

//---------------------------------------------------------------------------
// TCashDrawAerpos
//---------------------------------------------------------------------------

void TCashDrawAerpos3435::SendOpenDrawCommands(TPortTalk* PortTalk)
{
    PortTalk->OutPort(0x284, 0x01);
    //wait 200 ms
    ::Sleep(200);
    PortTalk->OutPort(0x284, 0x04);

    PortTalk->OutPort(0x284, 0x02);
    //wait 200 ms
    ::Sleep(200);
    PortTalk->OutPort(0x284, 0x04);
}
//---------------------------------------------------------------------------
void TPortCashDraw::makeLogFile(UnicodeString str)
{
    AnsiString fileName = ExtractFilePath(Application->ExeName) + "EFTPOSCashDrawer_Logs.txt" ;
    std::auto_ptr<TStringList> List(new TStringList);
    if (FileExists(fileName) )
    {
      List->LoadFromFile(fileName);
    }
    List->Add(" "+ str +  "\n");
    List->SaveToFile(fileName );
}
