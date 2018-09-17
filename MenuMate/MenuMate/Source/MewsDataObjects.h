//---------------------------------------------------------------------------

#ifndef MewsDataObjectsH
#define MewsDataObjectsH
#include "System.hpp"
#include <vector>
//---------------------------------------------------------------------------
class TPromotions
{
    public:
    bool BeforeCheckIn;
    bool AfterCheckIn;
    bool DuringStay;
    bool BeforeCheckOut;
    bool AfterCheckOut;
};

class TServiceMews
{
     public:
     UnicodeString  Id;
     bool IsActive;
     UnicodeString Name;
     UnicodeString StartTime;
     UnicodeString EndTime;
     TPromotions Promotions;
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
     std::vector<UnicodeString> Address;
     TDateTime BirthDate;
     UnicodeString BirthPlace;
     UnicodeString CategoryId;
     std::vector<UnicodeString> Classifications;
     TDateTime CreatedUtc;
     UnicodeString Email;
     UnicodeString FirstName;
     UnicodeString Gender;
     UnicodeString Id;
     UnicodeString LanguageCode;
     UnicodeString LastName;
     UnicodeString LoyaltyCode;
     UnicodeString NationalityCode;
     UnicodeString Notes;
     UnicodeString Number;
     UnicodeString Phone;
     UnicodeString SecondLastName;
     UnicodeString TaxIdentificationNumber;
     UnicodeString Title;
     TDateTime UpdatedUtc;
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

class TSpaceCategory
{
	public:
	UnicodeString Id;
	bool IsActive;
	UnicodeString Name;
	UnicodeString ShortName;
	UnicodeString Description;
	int Ordering;
	int UnitCount;
	int ExtraUnitCount;
};

class TSpaceDetails
{
    public:
    std::vector<TSpace> Spaces;
    std::vector<TSpaceCategory> SpaceCategories;
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


#endif
