//---------------------------------------------------------------------------


#pragma hdrstop

#include "CashDrawComPort.h"
#include "ManagerVariable.h"
#include "MMLogging.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

/**
 * Create com port controlled cash draw using supplied port number and length.
 *
 * @param inPortNumber Com port cash draw is plugged into.
 * @param inPortLength Size of the buffer to write to the port.
 * @param inPortHighChars
 */
TCashDrawComPort::TCashDrawComPort(char inPortNumber, int inPortLength, bool inPortHighChars)
{
    cashDrawPort = new TComPort(NULL);

    portNumber    = inPortNumber;
    portLength    = inPortLength;
    portHighChars = inPortHighChars;

    cashDrawPort->BaudRate    = br19200;
    cashDrawPort->DiscardNull = false;
    cashDrawPort->Port        = "COM" + IntToStr(portNumber);
}

/**
 * Open the cash draw using the selected com port.
 */
void TCashDrawComPort::OpenDraw()
{
    cashDrawPort->Open();

    unsigned char *buffer = new unsigned char [portLength];
    try
    {
        if(portHighChars)
        {
            FillMemory(buffer, portLength, 0xAA);
        }
        else
        {
            ZeroMemory(buffer, portLength);
        }
        cashDrawPort->Write(buffer, portLength);
    }
    __finally
    {
        delete []buffer;
        cashDrawPort->Close();
    }
}
