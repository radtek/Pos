//---------------------------------------------------------------------------

#ifndef AdjustmentContactH
#define AdjustmentContactH
//---------------------------------------------------------------------------

#include <System.hpp>
#include <vector>

class TAdjustmentTransaction
{
    public:
        Currency Adjustment;
        int AdjustmentType;
        int AdjustmentSubType;
        TDateTime TimeStamp;
        int InvoiceNumber;
        int ContactKey;

        TAdjustmentTransaction();
};

class TAdjustmentContact
{
    public:
        int ContactKey;
        AnsiString Name;
        AnsiString MemberNumber;
        int SiteId;
        std::vector<TAdjustmentTransaction> Adjustments;

        TAdjustmentContact();
};

#endif
