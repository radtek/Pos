//---------------------------------------------------------------------------

#ifndef RegistrationManagerH
#define RegistrationManagerH

//---------------------------------------------------------------------------
class TRegistrationManager
{
private:
    void UploadRegistrationInfo(AnsiString syndicateCode);
    void ValidateSiteIdAndSiteCode(Database::TDBTransaction &dbTransaction);
public:
    void CheckRegistrationStatus();
    void ValidateCompanyInfo(AnsiString syndicateCode, int siteId);
};
#endif
