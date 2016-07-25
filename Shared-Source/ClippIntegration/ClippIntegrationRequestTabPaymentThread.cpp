//---------------------------------------------------------------------------


#pragma hdrstop

#include "DeviceRealTerminal.h"
#include "ClippIntegrationRequestTabPaymentThread.h"
#include "Invoice.h"
#include "DBOrder.h"
#include "ManagerClippIntegrationHelper.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

TClippIntegrationRequestTabPaymentThread::TClippIntegrationRequestTabPaymentThread() : TClippIntegrationWriteMessageThread()
{ }

//---------------------------------------------------------------------------

TClippIntegrationThreadType TClippIntegrationRequestTabPaymentThread::GetClippIntegrationThreadType()
{
    return PaymentDetails;
}

//---------------------------------------------------------------------------

void TClippIntegrationRequestTabPaymentThread::StartServiceCommunication()
{
    try
    {
         //We need to create the service object from the data..
        //We will create the payment transaction from database and then with all compiled data we will create actual service object..
        TManagerClippIntegrationHelper managerClippIntegrationHelper;
        MMPaymentDetailRequest mmPaymentDetailRequest = managerClippIntegrationHelper.CreateMMPaymentDetailRequest(TabKey, RecievedMessageId, TotalPaymentAmount,
                                                                                                ClippProcessingFees);

        //Initiate the tab payment service communication..
        MMPaymentDetailResponse mmPaymentDetailResponse = RequestTabPayment(mmPaymentDetailRequest);

        //Finally we have recieved the data from clipp app, so we must update the tab amount on pos..
        managerClippIntegrationHelper.UpdateClippPaymentAmount(mmPaymentDetailResponse);
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

//---------------------------------------------------------------------------

MMPaymentDetailResponse TClippIntegrationRequestTabPaymentThread::RequestTabPayment(MMPaymentDetailRequest mmPaymentDetailRequest)
{
    try
    {
        return ClippIntegrationInterface->RequestTabPayment(mmPaymentDetailRequest);
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

//---------------------------------------------------------------------------
