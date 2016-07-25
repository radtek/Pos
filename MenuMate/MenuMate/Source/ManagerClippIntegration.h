//---------------------------------------------------------------------------

#ifndef ManagerClippIntegrationH
#define ManagerClippIntegrationH

//---------------------------------------------------------------------------

#include "ClippIntegrationInterface.h"
#include <Extctrls.hpp>
#include "LogsDB.h"
#include "DBTab.h"
#include "ClippIntegrationReadMessageThread.h"
#include "ClippIntegrationUploadDetailsThread.h"
#include "ClippIntegrationSendTabDetailsThread.h"
#include "ClippIntegrationCloseTabThread.h"

class TManagerClippIntegration
{
public:

    //The class has been declared singleton so we need method to get the instance of the class..
    static TManagerClippIntegration* Instance()
    {
        if(instance == NULL)
            instance = new TManagerClippIntegration();

        return instance;
    }

    //Entry point to service based methods, which needs manual intervention..
    //i.e. these methods will be called by users..
    void CloseTab(int tabKey);
    void SendTabDetails(int tabKey);

    __property TNotifyEvent OnCloseTabTerminate = {read = OnCloseThreadTerminate, write = OnCloseThreadTerminate};

private:

    // Constructor and Destructor are declared private since we need singleton class..
    TManagerClippIntegration();
    ~TManagerClippIntegration();

    //Fields
    static TManagerClippIntegration* instance;

    //These boolean flags indicate if the threads are working or in idle state..
    bool readMessageThreadTerminated;

    TNotifyEvent OnCloseThreadTerminate;

    //Thread declarations..
    //These threads run independently of each other..
    //Threads are required to make all service communications independent of each other and also we dont want to block current UI thread..
    TClippIntegrationReadMessageThread* readMessageThread;


    //This timer is required to read messages in the specified interval,
    //The timer duration will have to be set from the settings..
    TTimer* clippReadMessageThreadTimer;

    //These methods starts or stops the timer to read the messages from the service..
    void StartClippReadMessageThreadTimer();
    void StopClippReadMessageThreadTimer();

    //These messages are used to initialize and start the threads for indivisual operations..
    void InitializeAndStartReadMessagesThread();
    void InitializeAndStartUploadTabDetailsThread(int tabKey);
    void InitializeAndStartCloseTabThread(int tabKey);

    //Events
    void __fastcall OnClippReadMessageThreadTimerTick(TObject* sender);
    void __fastcall OnClippReadMessageThreadTerminate(TObject* sender);
    void __fastcall OnClippCloseTabThreadTerminate(TObject* sender);
};

#endif
