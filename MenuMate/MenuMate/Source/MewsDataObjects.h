//---------------------------------------------------------------------------

#ifndef MewsDataObjectsH
#define MewsDataObjectsH
#include "System.hpp"
#include <vector>
//---------------------------------------------------------------------------
class TServiceMews
{
     public:
     UnicodeString  Id;
     bool IsActive;
     UnicodeString Name;
     UnicodeString StartTime;
     UnicodeString EndTime;
};

class TOutlet
{
    public:
    UnicodeString Id;
    bool IsActive;
    UnicodeString Name;
};

class TAccountingCategory
{
     public:
     UnicodeString Id;
     bool IsActive;
     UnicodeString Name;
     UnicodeString Code;
     UnicodeString ExternalCode;
     UnicodeString LedgerAccountCode;
     UnicodeString PostingAccountCode;
     UnicodeString CostCenterCode;
     UnicodeString Classification;
};

class TCustomerMews
{
     public:
     UnicodeString Id;
     UnicodeString Number;
     UnicodeString Title;
     UnicodeString Gender;
     UnicodeString FirstName;
     UnicodeString LastName;
     std::vector<UnicodeString> Classifications;
     UnicodeString Email;
     UnicodeString Phone;
};

class TCustomerSearch
{
    public:
    UnicodeString ClientToken;
    UnicodeString AccessToken;
    UnicodeString FirstName;
    UnicodeString LastName;
    UnicodeString Name;
    UnicodeString SpaceId;
};

class TExtent
{
    public:
    bool Spaces;
    bool SpaceCategories;
    bool SpaceFeatures;
    bool Inactive;
};

class TBasicInquiry
{
    public:
    UnicodeString ClientToken;
    UnicodeString AccessToken;
    TExtent Extent;
} ;

class TSpace
{
	public:
	UnicodeString Id;
	bool IsActive;
	UnicodeString ParentSpaceId;
	UnicodeString CategoryId;
	UnicodeString Type;
	UnicodeString Number;
	UnicodeString FloorNumber;
	UnicodeString BuildingNumber;
	UnicodeString State;
};

class TSpaceDetails
{
    public:
    std::vector<TSpace> Spaces;
};

class TCategory
{
    public:
    UnicodeString Code;
};

class TUnitCost
{
    public:
    double Amount;
    UnicodeString Currency;
    double Tax;
};


class TItemMews
{
    public:
    UnicodeString Type;
    UnicodeString Name;
    int UnitCount;
    TUnitCost UnitCost;
    TDateTime ConsumedUtc;
    TCategory Category;
};

class TBill
{
    public:
    UnicodeString OutletId;
    UnicodeString Number;
    TDateTime ClosedUtc;
    std::vector<TItemMews> Items;
};

class TOrder
{
    public:
    UnicodeString ClientToken;
    UnicodeString AccessToken;
    UnicodeString CustomerId;
    UnicodeString ServiceId;
    TDateTime ConsumptionUtc;
    std::vector<TBill> Bills;
    std::vector<TItemMews> Items;
};

class TAccountingCategoriesMapping
{
    public:
    int CategoryMapId;
    int CategoryKey;
    UnicodeString MMCategoryName;
    UnicodeString MewsCategoryId;
    UnicodeString MewsCategoryCode;
};

#endif
