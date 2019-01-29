//---------------------------------------------------------------------------

#ifndef RegistrationManagerH
#define RegistrationManagerH

//---------------------------------------------------------------------------
class TRegistrationManager
{
private:
    bool UploadRegistrationInfo(Database::TDBTransaction &dbTransaction, AnsiString syndicateCode);
    void ValidateSiteIdAndSiteCode(Database::TDBTransaction &dbTransaction);
    void ResetHeaderFooter();
public:
    void CheckRegistrationStatus();
    bool ValidateCompanyInfo(Database::TDBTransaction &dbTransaction,AnsiString syndicateCode, int siteId);
};
#endif
