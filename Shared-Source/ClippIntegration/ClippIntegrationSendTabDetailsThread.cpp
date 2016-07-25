//---------------------------------------------------------------------------


#pragma hdrstop

#include "DeviceRealTerminal.h"
#include "ClippIntegrationSendTabDetailsThread.h"
#include "Invoice.h"
#include "DBOrder.h"
#include "ManagerClippIntegrationHelper.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

TClippIntegrationSendTabDetailsThread::TClippIntegrationSendTabDetailsThread() : TClippIntegrationWriteMessageThread()
{ }

//---------------------------------------------------------------------------

TClippIntegrationThreadType TClippIntegrationSendTabDetailsThread::GetClippIntegrationThreadType()
{
    return SendDetails;
}

//---------------------------------------------------------------------------

void TClippIntegrationSendTabDetailsThread::SetErrorCode(UnicodeString errorCode)
{
    _errorCode = errorCode;
}

//---------------------------------------------------------------------------

void TClippIntegrationSendTabDetailsThread::SetErrorDescription(UnicodeString errorDescription)
{
    _errorDescription = errorDescription;
}

//---------------------------------------------------------------------------

void TClippIntegrationSendTabDetailsThread::SetIsInErrorMode(bool isInErrorMode)
{
    _isInErrorMode = isInErrorMode;
}

//---------------------------------------------------------------------------
void TClippIntegrationSendTabDetailsThread::StartServiceCommunication()
{
    try
    {
        if(IsInErrorMode)
        {
            //Register the database transaction..
            Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
            TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
            dbTransaction.StartTransaction();

            //Get the clipp tab ref..
            UnicodeString clippTabRef = TDBClippTab::GetClippTabRefByTabKey(dbTransaction, TabKey);
            SendTabDetails(clippTabRef, RecievedMessageId, ErrorCode, ErrorDescription, TotalPaymentAmount);
            //Commit the transaction since we have finished all the transaction.
			dbTransaction.Commit();
        }
        else
        {
            //We need to create the service object from the data..
            //We will create the payment transaction from database and then with all compiled data we will create actual service object..
            TManagerClippIntegrationHelper managerClippIntegrationHelper;
            Sleep(1000);
            MMClippTabDetail mmClippTabDetail = managerClippIntegrationHelper.CreateMMClippTabDetail(TabKey, RecievedMessageId, TotalClippDiscounts);
            //Initiate the send tab details service communication..
            SendTabDetails(mmClippTabDetail);
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

//---------------------------------------------------------------------------

void TClippIntegrationSendTabDetailsThread::SendTabDetails(MMClippTabDetail mmClippTabDetail)
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

void TClippIntegrationSendTabDetailsThread::SendTabDetails(UnicodeString clippTabRef, UnicodeString recievedMessageId, UnicodeString errorCode,
                                                            UnicodeString errorDescription, Currency totalPaymentAmount)
{
    try
    {
         ClippIntegrationInterface->SendTabDetails(clippTabRef, recievedMessageId, errorCode, errorDescription, totalPaymentAmount);
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

//---------------------------------------------------------------------------
