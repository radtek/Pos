//---------------------------------------------------------------------------

#ifndef ClippIntegrationUploadDetailsThreadH
#define ClippIntegrationUploadDetailsThreadH

//---------------------------------------------------------------------------

#include "ClippIntegrationInterface.h"
#include <Extctrls.hpp>
#include "LogsDB.h"
#include "DBTab.h"
#include "ClippIntegrationThreadBase.h"

class TClippIntegrationUploadDetailsThread : public TClippIntegrationThreadBase
{
private:

    //Fields..
    //The vector to hold the clipp tab details which needs to be uploaded to the service in response to the messages recieved..
    CLIPP_TAB_DETAIL_VECTOR _clippTabDetailVector;

    //This message will send back the tab data to the Clipp Server..
    void SendTabDetails(MMClippTabDetail mmClippDetail);

protected:

    //This is the method to get the type of the clipp integration thread..
    virtual TClippIntegrationThreadType GetClippIntegrationThreadType();

    //This is the abstract method from base class which is overridden here..
    //This message will initiate the serivce call to upload all pending messages..
    virtual void StartServiceCommunication();

public:

    // Constructor
    TClippIntegrationUploadDetailsThread();

    void InsertClippTabDetail(MMClippTabDetail mmClippTabDetail);

    //This boolean method actually indicates the length of the vector, to decide if we have some data that needs to be uploaded
    bool IsUploadRequired();

    //This function will clear the vector..
    void ClearVector();
};


#endif
