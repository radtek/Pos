//---------------------------------------------------------------------------

#ifndef DBSecurityScreenSettingsH
#define DBSecurityScreenSettingsH

//---------------------------------------------------------------------------

#include <Classes.hpp>

#include <vector>

#include "MM_DBCore.h"
#include "DeviceRealTerminal.h"

//---------------------------------------------------------------------------

struct TFilterInfo
{
    TFilterInfo( int inDBKey, UnicodeString inName ) { DBKey = inDBKey; Name = inName; }

    int           DBKey;
    UnicodeString Name;

    bool          Selected;
};

typedef std::vector<TFilterInfo> AUDIT_FILTER_LIST;

class TDBSecurityScreenSettings
{
    public:
        static void GetLocations(
                      Database::TDBTransaction& inDBTransaction,
                      AUDIT_FILTER_LIST& outList );

        static void GetTerminals(
                      Database::TDBTransaction& inDBTransaction,
                      AUDIT_FILTER_LIST& outList );

        static void GetCashiers(
                      Database::TDBTransaction& inDBTransaction,
                      AUDIT_FILTER_LIST& outList );

        static void GetPaymentTypes(
                      Database::TDBTransaction& inDBTransaction,
                      AUDIT_FILTER_LIST& outList );
};

//---------------------------------------------------------------------------

#endif
