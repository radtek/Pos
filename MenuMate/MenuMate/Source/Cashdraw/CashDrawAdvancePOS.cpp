//---------------------------------------------------------------------------


#pragma hdrstop

#include "CashDrawAdvancePOS.h"
#include "PortTalk.h"
#include "MMLogging.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

/**
 * Open the cash draw on the Advance POS.
 */
void TCashDrawAdvancePOS::OpenDraw()
{
    HANDLE hFile = NULL;
    try
    {
        // IOCTL Codes
        #define GPD_TYPE 56053
        #define ADV_OPEN_CTL_CODE CTL_CODE(GPD_TYPE, 0x900, METHOD_BUFFERED, FILE_ANY_ACCESS)
        #define ADV_STATUS_CTL_CODE CTL_CODE(GPD_TYPE, 0x901, METHOD_BUFFERED, FILE_ANY_ACCESS)
        // uWhichDrawer = 1 => CD#1, uWhichDrawer = 2 => CD#2
        BOOL bRet;
        UCHAR uDrawer = 0x01;
        ULONG ulBytesReturned = 0;
        // Open the driver
        hFile = CreateFile("\\\\.\\ADVSYS", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL, 0);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"POS CM-5200 Unable to open Cash Drawer Device Driver!");
        }
        else
        {
            // Turn on the Cash Drawer Output (Fire the required solenoid)
            makeLogFile("Opening cash drawer Class - TCashDrawAdvancePOS");
            bRet = DeviceIoControl(hFile, ADV_OPEN_CTL_CODE, &uDrawer, sizeof(uDrawer), NULL, 0, &ulBytesReturned, NULL);
            if (bRet == FALSE || ulBytesReturned != 1)
            {
                TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"POS CM-5200 Failed to write to cash drawer driver");
                CloseHandle(hFile);
            }
            else
            {
                CloseHandle(hFile);
            }
        }
    }
    catch(Exception &Err)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"Error Opening Cash Drawer." + Err.Message);
        if (hFile != INVALID_HANDLE_VALUE)
        {
            CloseHandle(hFile);
        }
    }
}
//---------------------------------------------------------------------------
void TCashDrawAdvancePOS::makeLogFile(UnicodeString str)
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
