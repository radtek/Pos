//---------------------------------------------------------------------------


#pragma hdrstop

#include "DeviceRealTerminal.h"
#include "ClippIntegrationCloseTabThread.h"
#include "Invoice.h"
#include "DBOrder.h"
#include "ManagerClippIntegrationHelper.h"
#include "DBClippTab.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

TClippIntegrationCloseTabThread::TClippIntegrationCloseTabThread() : TClippIntegrationWriteMessageThread()
{ }

//---------------------------------------------------------------------------

TClippIntegrationThreadType TClippIntegrationCloseTabThread::GetClippIntegrationThreadType()
{
    return Close;
}

//---------------------------------------------------------------------------

void TClippIntegrationCloseTabThread::SetErrorCode(UnicodeString errorCode)
{
    _errorCode = errorCode;
}

//---------------------------------------------------------------------------

void TClippIntegrationCloseTabThread::SetErrorDescription(UnicodeString errorDescription)
{
    _errorDescription = errorDescription;
}

//---------------------------------------------------------------------------

void TClippIntegrationCloseTabThread::SetIsInErrorMode(bool isInErrorMode)
{
    _isInErrorMode = isInErrorMode;
}

//---------------------------------------------------------------------------

void TClippIntegrationCloseTabThread::StartServiceCommunication()
{
    try
    {
        //Register the database transaction..
        Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
        TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
        dbTransaction.StartTransaction();

        //We need to verify if the tab has any error attached, if yes then we will send the error message details to the clipp app..
        if(IsInErrorMode)
        {
            //Register the database transaction..
            Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
            TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
            dbTransaction.StartTransaction();
            //Get the clipp tab ref..
            UnicodeString clippTabRef = TDBClippTab::GetClippTabRefByTabKey(dbTransaction, TabKey);

            CloseTab(clippTabRef, RecievedMessageId, ErrorCode, ErrorDescription, TotalPaymentAmount);
            //Commit the transaction as we have completed all the transactions
            dbTransaction.Commit();
        }
        else
        {
            //We need to create the service object from the data..
            //We will create the payment transaction from database and then witkk all compiled data we will create actual service object..
            TManagerClippIntegrationHelper managerClippIntegrationHelper;
            MMClippTabDetail mmClippTabDetail = managerClippIntegrationHelper.CreateMMClippTabDetail(TabKey, RecievedMessageId, TipAmount, ClippProcessingFees, TotalClippDiscounts);

            //Initiate the close tab service communication..
            MMClippMessageDetail mmClippMessageDetail = CloseTab(mmClippTabDetail);

            //check that if clipp processing fee already stored in clipp_tab_payments
            bool success = TDBClippTab::IsProcessingFeeAlreadyApplied(dbTransaction, TabKey, 0);
            if(!success)
            {
               TDBClippTab::UpdateProcessingFeeAmount(dbTransaction, mmClippMessageDetail.ProcessingFeeAmount, TabKey, 0);
            }

            //Commit the transaction as we have completed all the transactions
            dbTransaction.Commit();   //MMClippMessageDetail->

            //Finally we have recieved the data from clipp app, so we must close the tab..
            managerClippIntegrationHelper.FinalizeClippTab(mmClippMessageDetail);
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

//---------------------------------------------------------------------------

MMClippMessageDetail TClippIntegrationCloseTabThread::CloseTab(MMClippTabDetail mmClippTabDetail)
{
    try
    {
        MMClippMessageDetail mmClippMessageDetail = ClippIntegrationInterface->CloseTab(mmClippTabDetail);
        return mmClippMessageDetail;
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

//---------------------------------------------------------------------------

void TClippIntegrationCloseTabThread::CloseTab(UnicodeString clippTabRef, UnicodeString recievedMessageId, UnicodeString errorCode,
                                                            UnicodeString errorDescription, Currency totalPaymentAmount)
{
    try
    {
         ClippIntegrationInterface->CloseTab(clippTabRef, recievedMessageId, errorCode, errorDescription, totalPaymentAmount);
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

//---------------------------------------------------------------------------
