//---------------------------------------------------------------------------

#ifndef RegistrationManagerH
#define RegistrationManagerH

//---------------------------------------------------------------------------
class TRegistrationManager
{
private:
    bool UploadRegistrationInfo(AnsiString syndicateCode);
    void ValidateSiteIdAndSiteCode(Database::TDBTransaction &dbTransaction);
public:
    void CheckRegistrationStatus();
    bool ValidateCompanyInfo(AnsiString syndicateCode, int siteId);
};
#endif
