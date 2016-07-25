//---------------------------------------------------------------------------

#ifndef ClippIntegrationWriteMessageThreadH
#define ClippIntegrationWriteMessageThreadH

//---------------------------------------------------------------------------

#include "ClippIntegrationThreadBase.h"
#include "ClippIntegrationInterface.h"

class TClippIntegrationWriteMessageThread : public TClippIntegrationThreadBase
{
private:

    //This is the tab key, message Id, tip amount, total payment amount and clipp processing fees which is required here for initiating the communication back to service..
    int _tabKey;
    UnicodeString _recievedMessageId;
    Currency _tipAmount;
    Currency _clippProcessingFees;
    Currency _totalPaymentAmount;
    std::vector<MMClippDiscount> _totalClippDiscounts;

    //Set the local tab key, message Id, tip amount, total payment amount and clipp processing fees..
    void SetTabKey(int tabKey);
    void SetRecievedMessageId(UnicodeString recievedMessageId);
    void SetTipAmount(Currency tipAmount);
    void SetClippProcessingFees(Currency clippProcessingFees);
    void SetTotalPaymentAmount(Currency totalPaymentAmount);
    void SetTotalClippDiscount(std::vector<MMClippDiscount> totalClippDiscounts);

protected:

    //This is the method to get the type of the clipp integration thread..
    virtual TClippIntegrationThreadType GetClippIntegrationThreadType() = 0;

    //This is the abstract method which every derived class needs to override..
    virtual void StartServiceCommunication() = 0;

public:

    // Constructor
    TClippIntegrationWriteMessageThread();

    //Get and Set the local tab key, message Id, tip amount, total payment amount and clipp processing fees..
    __property int TabKey = { read = _tabKey, write = SetTabKey};
    __property UnicodeString RecievedMessageId = { read = _recievedMessageId, write = SetRecievedMessageId};
    __property Currency TipAmount = { read = _tipAmount, write = SetTipAmount};
    __property Currency ClippProcessingFees = { read = _clippProcessingFees, write = SetClippProcessingFees};
    __property Currency TotalPaymentAmount = { read = _totalPaymentAmount, write = SetTotalPaymentAmount};
    __property std::vector<MMClippDiscount> TotalClippDiscounts = { read = _totalClippDiscounts, write = SetTotalClippDiscount};
};

#endif
