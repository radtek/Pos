//---------------------------------------------------------------------------


#pragma hdrstop

#include "ItemSize.h"

#include <item_management_booter.hh>
#include <i_item_manager.hh>
#include <i_item_definition.hh>
#include <i_size_definition.hh>

//---------------------------------------------------------------------------

#pragma package(smart_init)

using item_management::i_item_manager;
using item_management::i_size_definition;
using item_management::i_item_definition;

int
TItemSize::DefaultPatronCount(
    int count)
{
        return default_patron_count_ = count;
}

int
TItemSize::DefaultPatronCount()
const
{
        return default_patron_count_;
}

bool
TItemSize::IsProductCountEnabled()
{
	i_item_manager &item_manager =
	 item_management::kickstarter::get_item_manager();
   const i_size_definition &size_definition =
	 item_manager.get_size_definition(host_item_key_, ItemSizeKey);

   return size_definition.get_available_quantity() >= 0;
}

bool
TItemSize::IsEnabled()
const
{
   i_item_manager &item_manager =
     item_management::kickstarter::get_item_manager();
   const i_size_definition &size_definition =
     item_manager.get_size_definition(host_item_key_, ItemSizeKey);

   return size_definition.get_enabled();
}

const UnicodeString &
TItemSize::GetDecoratedName()
const
{
   i_item_manager &item_manager =
     item_management::kickstarter::get_item_manager();
   const i_size_definition &size_definition =
     item_manager.get_size_definition(host_item_key_, ItemSizeKey);

   return size_definition.get_decorated_name();
}

Currency
TItemSize::GetPrice()
const
{
   i_item_manager &item_manager =
     item_management::kickstarter::get_item_manager();
   const i_size_definition &size_definition =
     item_manager.get_size_definition(host_item_key_, ItemSizeKey);

   return size_definition.get_price();
}

Currency
TItemSize::GetMaxRetailPrice()
const
{
   i_item_manager &item_manager =
     item_management::kickstarter::get_item_manager();
   const i_size_definition &size_definition =
     item_manager.get_size_definition(host_item_key_, ItemSizeKey);

   return size_definition.get_maxRetailPrice();
}

Currency
TItemSize::GetSpecialPrice()
const
{
   i_item_manager &item_manager =
     item_management::kickstarter::get_item_manager();
   const i_size_definition &size_definition =
     item_manager.get_size_definition(host_item_key_, ItemSizeKey);

   return size_definition.get_special_price();
}

#pragma warn -par

/* No ops. A TItemSize no longer stores state for Price or Happy Price.
 * All references to those values force a fetch from the item manager.
 */

void
TItemSize::SetPrice(const Currency &price)
{
}

void
TItemSize::SetMaxRetailPrice(const Currency &price)
{
}

void
TItemSize::SetSpecialPrice(const Currency &special_price)
{
}

#pragma warn .par

TItemSize::TItemSize(item_key_type host_item_key)
  : host_item_key_(host_item_key)
{
   Recipes = new TListRecipeContainer;
   Categories = new TListCategoryContainer;
   SetMenuMask = 0;
   SetMenuMaster = false; // Is a Master Item        1-X-
   SetMenuItem = false;
   Cost = 0;
   CostGSTPercent = 0;
   GSTPercent = 0;
   MemberFreeSaleCount = 0;
   MemberFreeSaleDiscount = 0;
   LocationFreeSaleCount = 0;
   LocationFreeSaleDiscount = 0;
   ThirdPartyKey = 0;
   ThirdPartyCode = "";
   PLU = -1;
   Weighted = false;
   ItemSizeKey = 0;
   default_patron_count_ = 0;
   CostForPoints = 0;
   IsCanBePaidForUsingPoints = false;  // check can be paid by points..
   RevenueCode = 0;
   ItemSizeIdentifierKey = 0;
}

__fastcall TItemSize::~TItemSize()
{
   delete Recipes;
   delete Categories;
}

// ------------------------------------------------------------------------------

void TItemSize::SizeCopy(TItemSize * Copy)
{
   Palm_ID = Copy->Palm_ID;
   Size_ID = Copy->Size_ID;
   Name = Copy->Name;
   Barcode = Copy->Barcode;
   Cost = Copy->Cost;
   MemberFreeSaleCount = Copy->MemberFreeSaleCount;
   MemberFreeSaleDiscount = Copy->MemberFreeSaleDiscount;
   LocationFreeSaleCount = Copy->LocationFreeSaleCount;
   LocationFreeSaleDiscount = Copy->LocationFreeSaleDiscount;
   Available_As_Standard = Copy->Available_As_Standard;
   SetMenuMask = Copy->SetMenuMask;
   SetMenuMaster = Copy->SetMenuMaster;
   SetMenuItem = Copy->SetMenuItem;
   Recipes->RecipeCopyList(Copy->Recipes);
   Categories->CategoryCopyList(Copy->Categories);
   ThirdPartyKey = Copy->ThirdPartyKey;
   ThirdPartyCode = Copy->ThirdPartyCode;
	PLU = Copy->PLU;
   host_item_key_ = Copy->host_item_key_;
   ItemSizeKey = Copy->ItemSizeKey;
   default_patron_count_ = Copy->default_patron_count_;
   PriceLevels= Copy->PriceLevels;
   IsCanBePaidForUsingPoints = Copy->IsCanBePaidForUsingPoints;  // check can be paid by points..
   ItemSizeIdentifierKey = Copy->ItemSizeIdentifierKey;
}
