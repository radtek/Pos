//---------------------------------------------------------------------------

#ifndef RegistrationInterfaceH
#define RegistrationInterfaceH
#include "RegistrationWSDL.h"
#include "RegistrationAttributes.h"



enum MMRegistrationResponseCode
{
  Successful,
  AuthenticationFailed,
  NoNewSettingChange,
  BadRequestError,
  SiteCodeNotExist,
  SiteCodeInAcive,
  RegistrationUpdateFailed,
  CompanyInfoValidationFailed,
  FailedDueToException
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
    MMRegistrationServiceResponse CreateMMResponse(CompanySiteModelResponse* inWCFResponse );
    MMRegistrationServiceResponse CreateExceptionFailedResponse(AnsiString inMessage );
public:
    TRegistrationInterface();
    ~TRegistrationInterface();
    MMRegistrationServiceResponse UploadRegistrationInfo(TTerminalModel terminalInfo, AnsiString syndicateCode);
    MMRegistrationServiceResponse ValidateCompanyInfo(AnsiString syndicateCode, int siteId);
};

#endif