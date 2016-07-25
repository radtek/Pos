//---------------------------------------------------------------------------

#ifndef DBTaxH
#define DBTaxH
//---------------------------------------------------------------------------

#include "MM_DBCore.h"
#include <vector>

//---------------------------------------------------------------------------
struct TTaxProfile
{
    int ProfileKey;
    UnicodeString Name;
    Double Rate;
    int Type;
};

class TDBTax
{
    public:
        static std::vector<TTaxProfile> GetAllProfilesOfType(Database::TDBTransaction &DBTransaction, int type);
};
#endif
