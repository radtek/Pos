//---------------------------------------------------------------------------
#ifndef MallHelperH
#define MallHelperH

#include <System.hpp>
#include "DeviceRealTerminal.h"
//---------------------------------------------------------------------------
class TMallHelper
{
    public:
        TMallHelper();
        UnicodeString GetExportType(int mallid);
        double GetOldAccumulatedSales(Database::TDBTransaction &dbTransaction, int fieldIndex,int mallid);
};
#endif
