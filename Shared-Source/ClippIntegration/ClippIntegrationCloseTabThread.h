//---------------------------------------------------------------------------

#ifndef ClippIntegrationCloseTabThreadH
#define ClippIntegrationCloseTabThreadH

//---------------------------------------------------------------------------

#include "ClippIntegrationWriteMessageThread.h"

class TClippIntegrationCloseTabThread : public TClippIntegrationWriteMessageThread
{
private:

    UnicodeString _errorCode;
    UnicodeString _errorDescription;
    bool _isInErrorMode;

    void SetErrorCode(UnicodeString errorCode);
    void SetErrorDescription(UnicodeString errorDescription);
    void SetIsInErrorMode(bool isInErrorMode);

    //This message will send back the tab data to the Clipp Server..
    MMClippMessageDetail CloseTab(MMClippTabDetail mmClippDetail);
    void CloseTab(UnicodeString clippTabRef, UnicodeString recievedMessageId, UnicodeString errorCode,
                                                            UnicodeString errorDescription, Currency totalPaymentAmount);

protected:

    //This is the method to get the type of the clipp integration thread..
    virtual TClippIntegrationThreadType GetClippIntegrationThreadType();

    //This is the abstract method from base class which is overridden here..
    //This message will initiate the serivce call to send all details of tab to clipp app which is required to validate and close the tab..
    virtual void StartServiceCommunication();

public:

    // Constructor
    TClippIntegrationCloseTabThread();

    __property UnicodeString ErrorCode = { read = _errorCode, write = SetErrorCode};
    __property UnicodeString ErrorDescription = { read = _errorDescription, write = SetErrorDescription};
    __property bool IsInErrorMode = { read = _isInErrorMode, write = SetIsInErrorMode};
};

#endif
