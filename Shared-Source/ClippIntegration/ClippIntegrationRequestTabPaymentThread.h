//---------------------------------------------------------------------------

#ifndef ClippIntegrationRequestTabPaymentThreadH
#define ClippIntegrationRequestTabPaymentThreadH

//---------------------------------------------------------------------------

#include "ClippIntegrationInterface.h"
#include "ClippIntegrationWriteMessageThread.h"

class TClippIntegrationRequestTabPaymentThread : public TClippIntegrationWriteMessageThread
{
private:

    //This message will send back the tab data to the Clipp Server..
    MMPaymentDetailResponse RequestTabPayment(MMPaymentDetailRequest mmPaymentDetailRequest);

protected:

    //This is the method to get the type of the clipp integration thread..
    virtual TClippIntegrationThreadType GetClippIntegrationThreadType();

    //This is the abstract method from base class which is overridden here..
    //This message will initiate the serivce call to request payment for the tab..
    virtual void StartServiceCommunication();

public:

    // Constructor
    TClippIntegrationRequestTabPaymentThread();
};

#endif
