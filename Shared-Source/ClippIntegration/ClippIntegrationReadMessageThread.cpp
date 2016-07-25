//---------------------------------------------------------------------------

#pragma hdrstop

#include "DeviceRealTerminal.h"
#include "ClippIntegrationReadMessageThread.h"
#include "Invoice.h"
#include "DBOrder.h"
#include "ManagerClippIntegrationHelper.h"
#include "ClippIntegrationCloseTabThread.h"
#include "ClippIntegrationSendTabDetailsThread.h"
#include "ClippIntegrationRequestTabPaymentThread.h"
#include "SelectDish.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

TClippIntegrationReadMessageThread::TClippIntegrationReadMessageThread() : TClippIntegrationThreadBase()
{ }

//---------------------------------------------------------------------------

TClippIntegrationThreadType TClippIntegrationReadMessageThread::GetClippIntegrationThreadType()
{
    return Read;
}

//---------------------------------------------------------------------------

void TClippIntegrationReadMessageThread::StartServiceCommunication()
{
    try
    {
        CLIPP_MESSAGE_MAP clippMessageMap = ClippIntegrationInterface->GetClippMessages(Current);
        ProcessMessages(clippMessageMap);
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

//---------------------------------------------------------------------------

void TClippIntegrationReadMessageThread::ProcessMessages(CLIPP_MESSAGE_MAP clippMessageMap)
{
    try
    {
        if(!clippMessageMap.empty())
        {
            for (CLIPP_MESSAGE_MAP::iterator mapIterator = clippMessageMap.begin(); mapIterator != clippMessageMap.end(); ++mapIterator)
            {
                switch(mapIterator->first)
                {
                    case TabOpen:
                    {
                        for (CLIPP_MESSAGE_VECTOR::iterator vectorIterator = mapIterator->second.begin(); vectorIterator != mapIterator->second.end(); ++vectorIterator)
                        {
                            MMClippMessage mmClippMessage = *vectorIterator;
                            ProcessTabOpenMessage(mmClippMessage);
                        }
                        break;
                    }
                    case TabUpdate:
                    {
                        for (CLIPP_MESSAGE_VECTOR::iterator vectorIterator = mapIterator->second.begin(); vectorIterator != mapIterator->second.end(); ++vectorIterator)
                        {
                            MMClippMessage mmClippMessage = *vectorIterator;
                            ProcessTabUpdateMessage(mmClippMessage);
                        }
                        break;
                    }
                    case TabClose:
                    {
                        for (CLIPP_MESSAGE_VECTOR::iterator vectorIterator = mapIterator->second.begin(); vectorIterator != mapIterator->second.end(); ++vectorIterator)
                        {
                            MMClippMessage mmClippMessage = *vectorIterator;
                            ProcessTabCloseMessage(mmClippMessage);
                        }
                        break;
                    }
                    case TabPay:
                    {
                        for (CLIPP_MESSAGE_VECTOR::iterator vectorIterator = mapIterator->second.begin(); vectorIterator != mapIterator->second.end(); ++vectorIterator)
                        {
                            MMClippMessage mmClippMessage = *vectorIterator;
                            ProcessTabPayMessage(mmClippMessage);
                        }
                        break;
                    }
                }
            }
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

//---------------------------------------------------------------------------

void TClippIntegrationReadMessageThread::ProcessTabOpenMessage(MMClippMessage mmClippMessage)
{
    int errorTabKey = 0;
    try
    {
        //Register the database transaction..
        Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
        TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
        dbTransaction.StartTransaction();

        //Create the data to be stored in the tab and clipp tab tables...
        TManagerClippIntegrationHelper managerClippIntegrationHelper;
        TClippTab clippTab = managerClippIntegrationHelper.CreateClippTab(mmClippMessage);

        //Create the tab for Clipp..
        int tabKey = TDBClippTab::CreateAndSetClippTabDetails(dbTransaction, clippTab);
        errorTabKey = tabKey;

        //Commit the transaction as we have completed all the transactions
        dbTransaction.Commit();

        //We have created the tab now we need to communicate back the details to clipp..
        //We will spawn a new thread in order to upload details since we can not hold this worker thread for every thing..
        InitializeAndStartSendTabDetailsThread(tabKey, mmClippMessage.MessageId, mmClippMessage.TipAmount,
                                                    mmClippMessage.ProcessingFeeAmount);
    }
    catch(Exception &E)
    {
        //send error message to clipp tab can't bhe opened
        InitializeAndStartSendTabDetailsThread(errorTabKey, mmClippMessage.MessageId, mmClippMessage.TotalPaymentAmount,
                                                        UnicodeString("1004"), UnicodeString("Unable to open Tab."));
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

//---------------------------------------------------------------------------

void TClippIntegrationReadMessageThread::ProcessTabUpdateMessage(MMClippMessage mmClippMessage)
{
    try
    {
        //Register the database transaction..
        Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
        TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
        dbTransaction.StartTransaction();

        //We need to update the tab limit as stated by clipp app message recieved..
        int tabKey = TDBTab::SetTabDetails(dbTransaction, StrToInt(mmClippMessage.PosTabRef), mmClippMessage.TabLimit);

        bool tabExist = TDBClippTab::IsTabExist(dbTransaction, tabKey);

        //Commit the transaction as we have completed all the transactions
        dbTransaction.Commit();

        if(tabExist)
        {
            //We have created the tab now we need to communicate back the details to clipp..
            //We will spawn a new thread in order to upload details since we can not hold this worker thread for every thing..
            InitializeAndStartSendTabDetailsThread(StrToInt(mmClippMessage.PosTabRef), mmClippMessage.MessageId, mmClippMessage.TipAmount,
                                                        mmClippMessage.ProcessingFeeAmount);
        }
        else
        {
            InitializeAndStartSendTabDetailsThread(StrToInt(mmClippMessage.PosTabRef), mmClippMessage.MessageId, mmClippMessage.TotalPaymentAmount,
                                                        UnicodeString("1002"), UnicodeString("Tab not found on Pos."));
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

//---------------------------------------------------------------------------

void TClippIntegrationReadMessageThread::ProcessTabCloseMessage(MMClippMessage mmClippMessage)
{
    try
    {
        //Register the database transaction..
       Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
        TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
        dbTransaction.StartTransaction();

        bool tabExist = TDBClippTab::IsTabExist(dbTransaction, StrToInt(mmClippMessage.PosTabRef));
        if(!tabExist)
        {
            //Since tab doesn't exist so return the error messge to clipp
            InitializeAndStartSendTabDetailsThread(StrToInt(mmClippMessage.PosTabRef), mmClippMessage.MessageId, mmClippMessage.TotalPaymentAmount,
                                                        UnicodeString("1002"), UnicodeString("Tab not found on Pos."));
        }

         //We need to verify that tab is not in processing stage, if it is then we will return the
        //error meesage to the clipp..
        else if((!frmSelectDish->IsOrderinProgress)&&(StrToInt(mmClippMessage.PosTabRef) == frmSelectDish->ClippTabKey))
        {
            //Check if tab is closed already
            bool isTabClosed = TDBClippTab::IsTabClosed(dbTransaction, StrToInt(mmClippMessage.PosTabRef));

            if(isTabClosed)
            {
                InitializeAndStartCloseTabThread(StrToInt(mmClippMessage.PosTabRef), mmClippMessage.MessageId, mmClippMessage.TotalPaymentAmount, UnicodeString("1001"),
                                                UnicodeString("Tab closed on Pos."), mmClippMessage.Discounts );
            }
            else
            {
                //Lock the tab since we are about to make a final payment towards the tab..
                TDBTab::SetTabLocked(dbTransaction, StrToInt(mmClippMessage.PosTabRef), true,
                                        "The customer has requested to close tab from clipp app.");

                //Now we will start the close tab thread, lets not block the current thread..
                InitializeAndStartCloseTabThread(StrToInt(mmClippMessage.PosTabRef), mmClippMessage.MessageId, mmClippMessage.TipAmount,
                                                        mmClippMessage.ProcessingFeeAmount, mmClippMessage.Discounts);
            }

        }
        else
        {
            if(!frmSelectDish->IsOrderinProgress)
            {
                InitializeAndStartCloseTabThread(StrToInt(mmClippMessage.PosTabRef), mmClippMessage.MessageId, mmClippMessage.TipAmount,
                                                        mmClippMessage.ProcessingFeeAmount, mmClippMessage.Discounts);
            }
            else
            {
                //since tab is in use so return the error messge to clipp
                InitializeAndStartCloseTabThread(StrToInt(mmClippMessage.PosTabRef), mmClippMessage.MessageId, mmClippMessage.TotalPaymentAmount, UnicodeString("1006"),
                                                UnicodeString("Tab is in use."), mmClippMessage.Discounts);
            }
        }

        //Commit the transaction as we have completed all the transactions
        dbTransaction.Commit();

    }
    catch(Exception &E)
    {
        //since exception occue send error message to clipp
        InitializeAndStartCloseTabThread(StrToInt(mmClippMessage.PosTabRef), mmClippMessage.MessageId, mmClippMessage.TotalPaymentAmount, UnicodeString("1005"),
                                                UnicodeString("Unable to close Tab."), mmClippMessage.Discounts);
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

//---------------------------------------------------------------------------

void TClippIntegrationReadMessageThread::ProcessTabPayMessage(MMClippMessage mmClippMessage)
{
    try
    {
        //Now we will start the tab payment thread, lets not block the current thread..
        InitializeAndStartTabPayThread(StrToInt(mmClippMessage.PosTabRef), mmClippMessage.MessageId, mmClippMessage.TotalPaymentAmount,
                                                    mmClippMessage.ProcessingFeeAmount);
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

//---------------------------------------------------------------------------

void TClippIntegrationReadMessageThread::InitializeAndStartSendTabDetailsThread(int tabKey, UnicodeString recievedMessageId,
                                                            Currency tipAmount, Currency processingFeesAmount)
{
    //Create a new close tab thread and assign the fields to it..
    TClippIntegrationSendTabDetailsThread* sendTabDetailsThread = new TClippIntegrationSendTabDetailsThread();
    sendTabDetailsThread->TabKey = tabKey;
    sendTabDetailsThread->RecievedMessageId = recievedMessageId;
    sendTabDetailsThread->TipAmount = tipAmount;
    sendTabDetailsThread->ClippProcessingFees = processingFeesAmount;

    //Now we will start the send tab details thread, it will communicate to service for updating the tab data..
    sendTabDetailsThread->Start();
}

//---------------------------------------------------------------------------

void TClippIntegrationReadMessageThread::InitializeAndStartSendTabDetailsThread(int tabKey, UnicodeString recievedMessageId,
                                            Currency totalPaymentAmount, UnicodeString errorCode, UnicodeString errorDescription)
{
    //Create a new close tab thread and assign the fields to it..
    TClippIntegrationSendTabDetailsThread* sendTabDetailsThread = new TClippIntegrationSendTabDetailsThread();
    sendTabDetailsThread->TabKey = tabKey;
    sendTabDetailsThread->RecievedMessageId = recievedMessageId;
    sendTabDetailsThread->TotalPaymentAmount = totalPaymentAmount;
    sendTabDetailsThread->ErrorCode = errorCode;
    sendTabDetailsThread->ErrorDescription = errorDescription;
    sendTabDetailsThread->IsInErrorMode = true;

    //Now we will start the send tab details thread, it will communicate to service for updating the tab data..
    sendTabDetailsThread->Start();
}

//---------------------------------------------------------------------------

void TClippIntegrationReadMessageThread::InitializeAndStartCloseTabThread(int tabKey, UnicodeString recievedMessageId,
                                                            Currency tipAmount, Currency processingFeesAmount, std::vector<MMClippDiscount> clippDiscounts)
{
    //Create a new close tab thread and assign the fields to it..
    TClippIntegrationCloseTabThread* closeTabThread = new TClippIntegrationCloseTabThread();
    closeTabThread->TabKey = tabKey;
    closeTabThread->RecievedMessageId = recievedMessageId;
    closeTabThread->TipAmount = tipAmount;
    closeTabThread->ClippProcessingFees = processingFeesAmount;
    closeTabThread->TotalClippDiscounts = clippDiscounts;

    //Now we will start the close tab thread, it will communicate to service for closing the tab..
    closeTabThread->Start();
}

//---------------------------------------------------------------------------

void TClippIntegrationReadMessageThread::InitializeAndStartCloseTabThread(int tabKey, UnicodeString recievedMessageId, Currency totalPaymentAmount, UnicodeString errorCode,
                                                UnicodeString errorDescription, std::vector<MMClippDiscount> clippDiscounts)
{
    //Create a new close tab thread and assign the fields to it..
    TClippIntegrationCloseTabThread* closeTabThread = new TClippIntegrationCloseTabThread();
    closeTabThread->TabKey = tabKey;
    closeTabThread->RecievedMessageId = recievedMessageId;
    closeTabThread->TotalPaymentAmount = totalPaymentAmount;
    closeTabThread->ErrorCode = errorCode;
    closeTabThread->ErrorDescription = errorDescription;
    closeTabThread->TotalClippDiscounts = clippDiscounts;
    closeTabThread->IsInErrorMode = true;

    //Now we will start the close tab thread, it will communicate to service for closing the tab..
    closeTabThread->Start();
}

//---------------------------------------------------------------------------

void TClippIntegrationReadMessageThread::InitializeAndStartTabPayThread(int tabKey, UnicodeString recievedMessageId, Currency totalPaymentAmount,
                                                    Currency processingFeesAmount)
{
    //Create a new request tab payment thread and assign the fields to it..
    TClippIntegrationRequestTabPaymentThread* requestTabPaymentThread = new TClippIntegrationRequestTabPaymentThread();

    requestTabPaymentThread->TabKey = tabKey;
    requestTabPaymentThread->RecievedMessageId = recievedMessageId;
    requestTabPaymentThread->TotalPaymentAmount = totalPaymentAmount;
    requestTabPaymentThread->ClippProcessingFees = processingFeesAmount;

    //Now we will start the request tab payment thread, it will communicate to service to pay for the tab..
    requestTabPaymentThread->Start();
}

//---------------------------------------------------------------------------
