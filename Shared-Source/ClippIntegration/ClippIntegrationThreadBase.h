//---------------------------------------------------------------------------

#ifndef ClippIntegrationThreadBaseH
#define ClippIntegrationThreadBaseH

//---------------------------------------------------------------------------

#include "ClippIntegrationInterface.h"

enum TClippIntegrationThreadType
{
    Read,
    Upload,
    SendDetails,
    Close,
    PaymentDetails
};

class TClippIntegrationThreadBase : public TThread
{
private:

    //Fields..
    TClippIntegrationInterface* _clippIntegrationInterface;
    TClippIntegrationThreadType _clippIntegrationThreadType;

protected:

    //This is the method to get the type of the clipp integration thread..
    virtual TClippIntegrationThreadType GetClippIntegrationThreadType() = 0;

    //This is the abstract method which every derived class needs to override..
    virtual void StartServiceCommunication() = 0;

    //The virtual method from TThread class..
    virtual void __fastcall Execute();

    //These are protected properties to be used in derived classes for reading local private fields..
    __property TClippIntegrationInterface* ClippIntegrationInterface = { read = _clippIntegrationInterface };

public:

    // Constructor
    TClippIntegrationThreadBase();
};

#endif
