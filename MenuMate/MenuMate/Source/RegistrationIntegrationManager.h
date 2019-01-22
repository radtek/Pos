//---------------------------------------------------------------------------

#ifndef RegistrationIntegrationManagerH
#define RegistrationIntegrationManagerH
#include "RegistrationWSDL.h"
#include "RegistrationAttributes.h"


//---------------------------------------------------------------------------
class TRegistrationIntegrationManager
{
private:
    // Private Members
    _di_IRegistrationIntegrationWebService registrationClient; // WCF client
    // initiates the registration  Client
    void InitRegClient();
    AnsiString GetSyndCodeForRegistration();
public:
    TRegistrationIntegrationManager();
    ~TRegistrationIntegrationManager();

    bool UploadRegistrationInfo(TTerminal terminalInfo);

};

#endif
