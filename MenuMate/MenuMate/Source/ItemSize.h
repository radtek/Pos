//---------------------------------------------------------------------------

#ifndef ItemSizeH
#define ItemSizeH
//---------------------------------------------------------------------------
#include "ListRecipeContainer.h"
#include "ListCategoryContainer.h"
#include "ListSizeContainer.h"
#include "Weight.h"
#include "TaxProfile.h"
#include "MM_Menu.h"
#include <key_types.hh>

#include <vector>

class TItemSize
{
public:
	TListSizeContainer *Owner;
	__int32 ItemSizeKey;
	int Palm_ID;
	int Size_ID;
	UnicodeString Name;
	UnicodeString SizeKitchenName;
	UnicodeString Barcode;
   Currency Cost; // Cost is only used if there is no recipe
    double GSTPercent;
	double CostGSTPercent;
	double PointsPercent;
	bool Available_As_Standard;
	TListRecipeContainer *Recipes;
	unsigned long SetMenuMask;
	bool SetMenuMaster; // Is a Master Item        1-X-
	bool SetMenuItem;
	long ThirdPartyKey;
	UnicodeString ThirdPartyCode;
	bool Weighted;
	TWeight TareWeight;

	int MemberFreeSaleCount;
	double MemberFreeSaleDiscount;
	int LocationFreeSaleCount;
	double LocationFreeSaleDiscount;

        bool CanBePaidForUsingPoints;

	TListCategoryContainer *Categories;

    std::vector<TaxProfile> TaxProfiles;
    std::map<int,Menu::TItemSizePriceLevel> PriceLevels;

   __property Currency Price = { read = GetPrice, write = SetPrice };
   __property Currency MaxRetailPrice = { read = GetMaxRetailPrice, write = SetMaxRetailPrice };
   __property Currency HappyPrice = { read = GetSpecialPrice, write = SetSpecialPrice };



	TItemSize(item_key_type host_item_key = 0);
	virtual __fastcall ~TItemSize();
	void SizeCopy(TItemSize * Copy); // Ordered via a master    0111
   int PLU;

   const UnicodeString &GetDecoratedName() const;
   bool IsEnabled() const;

   bool IsProductCountEnabled();

   int DefaultPatronCount(int count);
   int DefaultPatronCount() const;
   Currency CostForPoints;
   bool IsCanBePaidForUsingPoints;
   int  RevenueCode;
   int  ItemSizeIdentifierKey;

private:
   item_key_type host_item_key_;
   int default_patron_count_;

   Currency GetPrice() const;
   Currency GetMaxRetailPrice() const;
   Currency GetSpecialPrice() const;

   void SetPrice(const Currency &price);
   void SetMaxRetailPrice(const Currency &maxRetailPrice);
   void SetSpecialPrice(const Currency &price);
};

#endif
