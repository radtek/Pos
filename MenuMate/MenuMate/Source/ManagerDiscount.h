//---------------------------------------------------------------------------
#ifndef ManagerDiscountH
#define ManagerDiscountH

#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <IBTable.hpp>
#include <set>
#include "Enum.h"
#include "MM_DBCore.h"
#include "MMContactInfo.h"
#include "Discount.h"
#include "POS_XMLBase.h"
#include "ItemMinorComplete.h"
//---------------------------------------------------------------------------

enum eDiscountFilter {
    exclude_combos,
    include_combos,
    only_combos,
    exclude_member_exempt
};

#define __num_discount_list_combo_filters  (only_combos + 1)

class TMiniDiscount
{
	public:
	TMiniDiscount(UnicodeString inDiscription,Currency inAmount) : Discription(inDiscription), Amount(inAmount) {}

	Currency Amount;
	UnicodeString Discription;

	bool operator<(const TMiniDiscount &f1)
	{
		bool Retval = false;
		if(Discription < f1.Discription)
		{
			Retval = true;
		}
		else if (Discription == f1.Discription)
		{
			if(Amount < f1.Amount)
			{
				Retval = true;
			}
		}
		return Retval;
	}
	friend bool operator<(const TMiniDiscount &f1, const TMiniDiscount &f2);
};

bool operator < (const TMiniDiscount &f1,const TMiniDiscount &f2)
{
		bool Retval = false;
		if(f1.Discription < f2.Discription)
		{
			Retval = true;
		}
		else if (f1.Discription == f2.Discription)
		{
			if(f1.Amount < f2.Amount)
			{
				Retval = true;
			}
		}
		return Retval;
};

class TDiscountTimes
{
   public:
   int DiscountTimeKey;
   int DiscountKey;
   TDateTime StartTime;
   TDateTime EndTime;
};

class TManagerDiscount
{
   private :
    bool fEnabled;
    void __fastcall SetEnabled(bool value);
    bool __fastcall GetEnabled();

    void GetDiscountKeysByTime(Database::TDBTransaction &DBTransaction,TDateTime &CurrentTime, std::set<int> &DiscountKeys);
    void AddCurrencyModeDiscount(TList * DiscountItems,TDiscount DiscountToBeApplied);
    void AddComboDiscount(TList * DiscountItems,TDiscount DiscountToBeApplied);
    void AddDealDiscount(TList * DiscountItems,TDiscount DiscountToBeApplied);
    void AddItemModeDiscount(TList * DiscountItems,TDiscount DiscountToBeApplied);
    void AddPercentageModeDiscount(TList * DiscountItems,TDiscount DiscountToBeApplied);
    void AddSetPriceDiscount(TList *DiscountItems,TDiscount DiscountToBeApplied);
    void AddPointModeDiscount(TList *DiscountItems,TDiscount DiscountToBeApplied);
    Currency GetOrderTotal(TList *DiscountItems,TDiscount DiscountToBeApplied, double maxDiscountQty);
    double GetOrderQty(TList *DiscountItems);
    Currency GetOrderPointsTotal(TList *DiscountItems,TDiscount DiscountToBeApplied, double maxDiscountQty);
    void CopyDiscountDetails(TDiscount& destination,TDiscount& source);
	public:
	void Initialise();
    __property bool Enabled  = { read=GetEnabled, write=SetEnabled };
    TManagerDiscount();
    ~TManagerDiscount();

    void GetVoucherListThor(Database::TDBTransaction &,TStringList *destination_list,bool ShowPointsAsDiscount = true);
    void GetDiscountList(Database::TDBTransaction &,TStringList *destination_list,std::vector<eDiscountFilter> filters, bool ShowPointsAsDiscount = true);
    void GetDiscountListByPriority(Database::TDBTransaction &, TStringList *);
    UnicodeString GetDiscountDescription(Database::TDBTransaction &DBTransaction,long DiscountKey);
    bool GetDiscount(Database::TDBTransaction &DBTransaction,long DiscountKey, TDiscount &Discount);
    bool GetThorlinkDiscount(Database::TDBTransaction &DBTransaction, TDiscount &Discount);
    bool SetAmountDB(Database::TDBTransaction &DBTransaction,long DiscountKey, Currency Amount);
    bool GetNonChargableDiscount(Database::TDBTransaction &DBTransaction, TDiscount &Discount);
    void GetReportDiscountInfo(Database::TDBTransaction &DBTransaction,std::set<int> &DiscountsList,TStringList *Report);
    void GetReportItemDiscounts(TItemMinorComplete *Item,TStringList *Report);
    void GetReportItemDiscountsHeader(TStringList *Report);
    void GetReportItemDiscountsFooter(TStringList *Report);
    void GetDiscountTime(Database::TDBTransaction &DBTransaction,int DiscountTimeKey,TDiscountTimes &DiscountTimes);
    void GetDiscountTimes(Database::TDBTransaction &DBTransaction,int DiscountKey, std::vector<TDiscountTimes> &DiscountTimes);
    int GetDiscountKeyByCode(Database::TDBTransaction &DBTransaction,AnsiString DiscountCode);
    AnsiString GetDiscountCodeByKey(Database::TDBTransaction &DBTransaction,long DiscountKey);
    void DiscountCodeToKey(Database::TDBTransaction &DBTransaction,std::set<AnsiString> &AutoAppliedDiscountsID, std::set<int> &AutoAppliedDiscounts);
    void DiscountKeyToCode(Database::TDBTransaction &DBTransaction,std::set<AnsiString> &AutoAppliedDiscountsID, std::set<int> &AutoAppliedDiscounts);
    void DeleteDiscount(Database::TDBTransaction &DBTransaction,long DiscountKey);
    void SetDiscount(Database::TDBTransaction &DBTransaction,long DiscountKey,TDiscount Discount);
    void AddDiscount(TList * DiscountItems,TDiscount DiscountToBeApplied);
    void DeleteDiscountTime(Database::TDBTransaction &DBTransaction,TDiscountTimes &DiscountTime);
    void AddDiscountTime(Database::TDBTransaction &DBTransaction,TDiscountTimes &DiscountTime);
    void AddDiscountsByTime(Database::TDBTransaction &DBTransaction,TItemMinorComplete *Order);
    void DecreasePriority(Database::TDBTransaction &DBTransaction, int DiscountKey);
    void IncreasePriority(Database::TDBTransaction &DBTransaction, int DiscountKey);
    void DecreaseAppearanceOrder(Database::TDBTransaction &DBTransaction, int DiscountKey);
    void IncreaseAppearanceOrder(Database::TDBTransaction &DBTransaction, int DiscountKey);
    void ClearDiscounts(TList * DiscountItems);
    void ClearThorVouchersDiscounts(TList * DiscountItems);
    void ClearDiscount(TList * DiscountItems,TDiscount DiscountToBeApplied);
    void ClearMemberDiscounts(TList * DiscountItems);
    void ClearDiscountDB(Database::TDBTransaction &DBTransaction,const std::set<long> &OrderKeys);
    void SetDiscountAmountDB(Database::TDBTransaction &DBTransaction,TList * DiscountedItems);
    void DeleteCard(Database::TDBTransaction &DBTransaction,UnicodeString Card);
    void DeleteCard(Database::TDBTransaction &DBTransaction,int DiscountKey);
    void SetCard(Database::TDBTransaction &DBTransaction,int DiscountKey, UnicodeString Card);
    int GetDiscount(Database::TDBTransaction &DBTransaction,UnicodeString Card);
    int GetDiscountCard(Database::TDBTransaction &DBTransaction,UnicodeString Card);
    Currency GetDiscountTotal(TList * DiscountItems);

    /*This function is broken for getting a $ value discount.
    Currency GetDiscountValue(TList * DiscountItems,TDiscount &Discount); */
    bool DiscountCodeExisits(Database::TDBTransaction &DBTransaction,long DiscountKey, AnsiString DiscountCode);
    void BuildXMLListDiscounts(Database::TDBTransaction &DBTransaction,TPOS_XMLBase &Data);
    void CheckDiscount(TForm *inDisplayOwner, TDiscount inDiscount, TList **Orders);
    static void GetDiscountCategories(Database::TDBTransaction &DBTransaction,long DiscountKey, TDiscount &Discount);
    static void GetThorVoucherCategories(Database::TDBTransaction &DBTransaction,long DiscountKey, TDiscount &Discount);
    static void SetDiscountCategories(Database::TDBTransaction &DBTransaction,long DiscountKey, TDiscount &Discount);
    static void PopulateDiscountGroupPerType(long DiscountKey, TDiscount &Discount);
    int GetDiscountKeyForVoucher(int id);
    void DeleteCloudDiscounts(Database::TDBTransaction &DBTransaction,std::vector<AnsiString> discountsCodes);
    bool IsCloudDiscount(Database::TDBTransaction &DBTransaction,long discountKey);
    void DeleteDiscounts(Database::TDBTransaction &DBTransaction);
    void ClearMemberExemtDiscounts(TList * DiscountItems);
    bool IsVouchersAvailable();
    void GetMembershipDiscounts(Database::TDBTransaction &DBTransaction,std::set<int> &discountKeys);
};

extern TManagerDiscount *ManagerDiscount;

#endif
