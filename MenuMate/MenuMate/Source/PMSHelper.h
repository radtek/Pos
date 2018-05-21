//---------------------------------------------------------------------------

#ifndef PMSHelperH
#define PMSHelperH
#include "ManagerPMSCodes.h"
//---------------------------------------------------------------------------
class TPMSHelper
{
private:
    TIBSQL* GetRevenueCodesQuery(Database::TDBTransaction &DBTransaction);
    //TIBSQL* GetPMSPaymentQuery(Database::TDBTransaction &DBTransaction);
//    bool DefaultPaymentInitRequired();
//    void InitDefaultPaymentInDB();
public:
    bool LoadRevenueCodes(std::map<int,TRevenueCodeDetails> &RevenueCodesMap, Database::TDBTransaction &DBTransaction);
    void GetRevenueCode(TList *Orders);
//    void InitializePMSDefaultPayment();
    //void LoadPMSPaymentTypes(std::map<int, TPMSPaymentType> &PMSPaymentTypesMap);
};

#endif
