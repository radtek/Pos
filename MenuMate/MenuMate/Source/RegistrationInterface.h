//---------------------------------------------------------------------------

#ifndef RegistrationInterfaceH
#define RegistrationInterfaceH
#include "RegistrationWSDL.h"
#include "RegistrationAttributes.h"


enum MMRegistrationResponseCode
{
  Successful = 1,
  AuthenticationFailed,
  RegistrationUpdateFailed
};


class MMRegistrationServiceResponse
{
    public:
        bool                  IsSuccesful;
        AnsiString            Message;
        AnsiString            Description;
        MMRegistrationResponseCode ResponseCode;
        MMRegistrationServiceResponse(bool  inSuccess,AnsiString inMessage,AnsiString  inDescription,MMRegistrationResponseCode inResponseCode)
            : IsSuccesful(inSuccess),Message(inMessage),Description(inDescription),ResponseCode(inResponseCode)
        {
        }
};

//---------------------------------------------------------------------------
class TRegistrationInterface
{
private:
    // Private Members
    _di_IRegistrationIntegrationWebService registrationClient; // WCF client
    // initiates the registration  Client
    void InitRegClient();
    MMRegistrationServiceResponse CreateMMResponse(RegistrationResponse* inWCFResponse );
    AnsiString GetSyndCodeForRegistration();

public:
    TRegistrationInterface();
    ~TRegistrationInterface();
    MMRegistrationServiceResponse UploadRegistrationInfo(TTerminal terminalInfo);
};

#endif
