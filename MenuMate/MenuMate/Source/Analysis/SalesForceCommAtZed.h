//---------------------------------------------------------------------------

#ifndef SalesForceCommAtZedH
#define SalesForceCommAtZedH
#include "DeviceRealTerminal.h"
//---------------------------------------------------------------------------
class TSalesForceCommAtZed
{
    private:
         AnsiString Username;
         AnsiString Password;
         AnsiString MerchantId;
         AnsiString Url;
         bool PVFieldSalesForce;

         AnsiString UserPV;
         AnsiString PasswordPV;
         AnsiString MerchantIdPV;
         bool EnabledOnline;
         bool DisplayMesssage;

    public:
         TSalesForceCommAtZed();
         ~TSalesForceCommAtZed();
         bool CheckPocketVoucherPaymentType(Database::TDBTransaction &DBTransaction,AnsiString CompanyName);
         void CreatePVAsPaymentType(Database::TDBTransaction &DBTransaction);
         void PVCommunication(Database::TDBTransaction &DBTransaction,AnsiString CompanyName);
         void SalesForceCommunication(AnsiString CompanyName);
         void EnableOrDisablePV(AnsiString CompanyName);
         bool IsXeroActivated(Database::TDBTransaction &DBTransaction1);
         bool IsWebMateActivated(Database::TDBTransaction &DBTransaction1);
         bool IsThorActivated(Database::TDBTransaction &DBTransaction1);
         bool IsStockActivated(Database::TDBTransaction &DBTransaction1);
         UnicodeString GetversionNumber(Database::TDBTransaction &DBTransaction1);
         void FindingValuesOfFields(AnsiString accountName);
         void UpdatePVPaymentType(Database::TDBTransaction &DBTransaction , AnsiString name);
         bool IsChefMateActivated(Database::TDBTransaction &DBTransaction1);
         bool IsPalmMateActivated(Database::TDBTransaction &DBTransaction1);
         bool IsLoyaltyActivated(Database::TDBTransaction &DBTransaction1);
};
#endif
