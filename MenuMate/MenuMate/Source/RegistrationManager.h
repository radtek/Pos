//---------------------------------------------------------------------------

#ifndef RegistrationManagerH
#define RegistrationManagerH

//---------------------------------------------------------------------------
class TRegistrationManager
{
private:
    void UploadRegistrationInfo();
public:
    void CheckRegistrationStatus();
    void ValidateCompanyInfo(AnsiString syndicateCode, int siteId);
};
#endif
