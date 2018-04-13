//---------------------------------------------------------------------------

#ifndef PMSHelperH
#define PMSHelperH
#include "ManagerPMSCodes.h"
//---------------------------------------------------------------------------
class TPMSHelper
{
private:
    TIBSQL* GetRevenueCodesQuery(Database::TDBTransaction &DBTransaction);
public:
    bool LoadRevenueCodes(std::map<int,TRevenueCodeDetails> &RevenueCodesMap, Database::TDBTransaction &DBTransaction);
    void GetRevenueCode(TList *Orders);
};

#endif
