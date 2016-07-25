//---------------------------------------------------------------------------

#ifndef ClippIntegrationReadMessageThreadH
#define ClippIntegrationReadMessageThreadH

//---------------------------------------------------------------------------

#include "ClippIntegrationInterface.h"
#include "ClippIntegrationUploadDetailsThread.h"
#include <Extctrls.hpp>
#include "LogsDB.h"
#include "DBTab.h"

class TClippIntegrationReadMessageThread : public TClippIntegrationThreadBase
{
private:

    //The method will initiate the message queue to be ansorbed by pos once the communication to service completes..
    void ProcessMessages(CLIPP_MESSAGE_MAP clippMessageMap);

    //Helper methods for processing the data recieved from the services..
    void ProcessTabOpenMessage(MMClippMessage clippMessage);
    void ProcessTabUpdateMessage(MMClippMessage clippMessage);
    void ProcessTabPayMessage(MMClippMessage clippMessage);
    void ProcessTabCloseMessage(MMClippMessage clippMessage);

    //These messages are used to initialize and start the threads for indivisual operations..
    void InitializeAndStartSendTabDetailsThread(int tabKey, UnicodeString recievedMessageId, Currency tipAmount,
                                                    Currency processingFeesAmount);
    void InitializeAndStartSendTabDetailsThread(int tabKey, UnicodeString recievedMessageId, Currency totalPaymentAmount,
                                                     UnicodeString errorCode, UnicodeString errorDescription);
    void InitializeAndStartCloseTabThread(int tabKey, UnicodeString recievedMessageId, Currency tipAmount, Currency processingFeesAmount, std::vector<MMClippDiscount> clippDiscounts);
    void InitializeAndStartCloseTabThread(int tabKey, UnicodeString recievedMessageId, Currency totalPaymentAmount, UnicodeString errorCode, UnicodeString errorDescription, std::vector<MMClippDiscount> clippDiscounts);
    void InitializeAndStartTabPayThread(int tabKey, UnicodeString recievedMessageId, Currency totalPaymentAmount,
                                                    Currency processingFeesAmount);

protected:

    //This is the method to get the type of the clipp integration thread..
    virtual TClippIntegrationThreadType GetClippIntegrationThreadType();

    //This is the abstract method from base class which is overridden here..
    //This message will initiate the serivce call to read all pending messages..
    virtual void StartServiceCommunication();

public:

    // Constructor
    TClippIntegrationReadMessageThread();
};

#endif
