//---------------------------------------------------------------------------

#include "DeviceRealTerminal.h"
#include "ManagerClippIntegration.h"
#include "Invoice.h"
#include "DBOrder.h"
#include "ManagerClippIntegrationHelper.h"


#pragma hdrstop

//---------------------------------------------------------------------------

TManagerClippIntegration* TManagerClippIntegration::instance = NULL;

TManagerClippIntegration::TManagerClippIntegration() :
                            readMessageThreadTerminated(true),
                            readMessageThread(NULL)
{
    //Initiate the class and its member variables..
    clippReadMessageThreadTimer = new TTimer(NULL);
    clippReadMessageThreadTimer->Enabled = false;
    //TODO: It has to be mapped to the global settings
    clippReadMessageThreadTimer->Interval = 10000;
    clippReadMessageThreadTimer->OnTimer = OnClippReadMessageThreadTimerTick;

    StartClippReadMessageThreadTimer();
}

//---------------------------------------------------------------------------

TManagerClippIntegration::~TManagerClippIntegration()
{
    //Remove all references..
    delete clippReadMessageThreadTimer;
    delete readMessageThread;
}

//---------------------------------------------------------------------------

void TManagerClippIntegration::CloseTab(int tabKey)
{
    InitializeAndStartCloseTabThread(tabKey);
}

//---------------------------------------------------------------------------

void TManagerClippIntegration::SendTabDetails(int tabKey)
{
    InitializeAndStartUploadTabDetailsThread(tabKey);
}

//---------------------------------------------------------------------------

void __fastcall TManagerClippIntegration::OnClippReadMessageThreadTimerTick(TObject* sender)
{
    if(readMessageThreadTerminated)
    {
        InitializeAndStartReadMessagesThread();
    }
    else
    {
        StopClippReadMessageThreadTimer();
    }
}

//---------------------------------------------------------------------------

void __fastcall TManagerClippIntegration::OnClippReadMessageThreadTerminate(TObject* sender)
{
    readMessageThreadTerminated = true;
    StartClippReadMessageThreadTimer();
}

//---------------------------------------------------------------------------

void __fastcall TManagerClippIntegration::OnClippCloseTabThreadTerminate(TObject* sender)
{
    if(OnCloseTabTerminate)
    {
        OnCloseTabTerminate(sender);
    }
}

//---------------------------------------------------------------------------

void TManagerClippIntegration::InitializeAndStartReadMessagesThread()
{
    if(readMessageThreadTerminated)
    {
        //Now we need to set the fields to intimate the program that the worker thread is running hence we need to stop all other function..
        readMessageThreadTerminated = false;
        StopClippReadMessageThreadTimer();

        //Initialize the worker thread to read messages..
        readMessageThread = new TClippIntegrationReadMessageThread();

        //Assign the terminated event to the thread..
        readMessageThread->OnTerminate = OnClippReadMessageThreadTerminate;

        //Start the worker thread..
        readMessageThread->Start();
    }
}

//---------------------------------------------------------------------------

void TManagerClippIntegration::InitializeAndStartUploadTabDetailsThread(int tabKey)
{
    //Create a new send tab details thread and assign the fields to it..
    TClippIntegrationSendTabDetailsThread* sendTabDetailsThread = new TClippIntegrationSendTabDetailsThread();
    sendTabDetailsThread->TabKey = tabKey;

    //Now we will start the send tab details thread, it will communicate to service for sending details of the tab..
    sendTabDetailsThread->Start();
}

//---------------------------------------------------------------------------

void TManagerClippIntegration::InitializeAndStartCloseTabThread(int tabKey)
{
    //Create a new close tab thread and assign the fields to it..
    TClippIntegrationCloseTabThread* closeTabThread = new TClippIntegrationCloseTabThread();
    closeTabThread->TabKey = tabKey;

    //Now we will start the close tab thread, it will communicate to service for closing the tab..
    closeTabThread->Start();
    closeTabThread->OnTerminate = OnClippCloseTabThreadTerminate;
}

//---------------------------------------------------------------------------

void TManagerClippIntegration::StartClippReadMessageThreadTimer()
{
    clippReadMessageThreadTimer->Enabled = true;
}

//---------------------------------------------------------------------------

void TManagerClippIntegration::StopClippReadMessageThreadTimer()
{
    clippReadMessageThreadTimer->Enabled = false;
}

//---------------------------------------------------------------------------
