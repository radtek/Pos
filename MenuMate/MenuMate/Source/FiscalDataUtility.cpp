//---------------------------------------------------------------------------


#pragma hdrstop

#include "FiscalDataUtility.h"
#include "MMMessageBox.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
AnsiString TFiscalDataUtility::CreateVersionRequest()
{
    UnicodeString retValue = "";
    retValue = "ver 0000/r/n";
    return retValue;
}
//---------------------------------------------------------------------------
AnsiString TFiscalDataUtility::CreatePostRequest(PostType type)
{
    UnicodeString retValue = "";
    return retValue;
}
//---------------------------------------------------------------------------
bool TFiscalDataUtility::AnalyzeResponse(AnsiString inData, PostType type)
{
    bool retValue = false;
    switch(type)
    {
        case eFiscalVerNumber:
        {
            if(inData.Pos("0") == 1)
                retValue = true;
            break;
        }
        case eFiscalNormalReceipt:
        case eFiscalCopyReceipt:
        case eFiscalPracticeReceipt:
        case eFiscalProofReceipt:
        {
            retValue = AnalyzePostResponse(inData, type);
            break;
        }
        default:
            break;
    }
    return retValue;
}
//---------------------------------------------------------------------------
bool TFiscalDataUtility::AnalyzePostResponse(AnsiString inData, PostType type)
{
    bool retValue = false;
    inData = inData.Trim();
    AnsiString value = inData.SubString(0,1);
    PostType postType;
    int data = atoi(value.c_str());
    if(data == 0)
        retValue = true;

    return retValue;
}
//---------------------------------------------------------------------------
