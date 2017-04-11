//---------------------------------------------------------------------------

#ifndef SalesTypeItemDetailsH
#define SalesTypeItemDetailsH
#include <System.hpp>>
//---------------------------------------------------------------------------

enum eItemStatus {eAssigned, eRemoved};

struct TItemDetails
{
    eItemStatus ItemStatus;
    UnicodeString ItemName;
};
#endif
