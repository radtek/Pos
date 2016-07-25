//---------------------------------------------------------------------------


#pragma hdrstop

#include "DeviceRealTerminal.h"
#include "ClippIntegrationUploadDetailsThread.h"
#include "Invoice.h"
#include "DBOrder.h"
#include "ManagerClippIntegrationHelper.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

TClippIntegrationUploadDetailsThread::TClippIntegrationUploadDetailsThread() : TClippIntegrationThreadBase()
{ }

//---------------------------------------------------------------------------

TClippIntegrationThreadType TClippIntegrationUploadDetailsThread::GetClippIntegrationThreadType()
{
    return Upload;
}

//---------------------------------------------------------------------------

void TClippIntegrationUploadDetailsThread::StartServiceCommunication()
{
    try
    {
        if(!_clippTabDetailVector.empty())
        {
            for(CLIPP_TAB_DETAIL_VECTOR::iterator it = _clippTabDetailVector.begin(); it != _clippTabDetailVector.end(); ++it)
            {
                MMClippTabDetail mmClippTabDetail = *it;
                SendTabDetails(mmClippTabDetail);
            }
        }

        //Once we finish off uploading the data we need to clear off all the data from the vector...
        ClearVector();
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

//---------------------------------------------------------------------------

void TClippIntegrationUploadDetailsThread::SendTabDetails(MMClippTabDetail mmClippTabDetail)
{
    try
    {
        ClippIntegrationInterface->SendTabDetails(mmClippTabDetail);
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

//---------------------------------------------------------------------------

void TClippIntegrationUploadDetailsThread::InsertClippTabDetail(MMClippTabDetail mmClippTabDetail)
{
    _clippTabDetailVector.push_back(mmClippTabDetail);
}

//---------------------------------------------------------------------------

void TClippIntegrationUploadDetailsThread::ClearVector()
{
    _clippTabDetailVector.clear();
}

//---------------------------------------------------------------------------

bool TClippIntegrationUploadDetailsThread::IsUploadRequired()
{
    return !_clippTabDetailVector.empty();
}
