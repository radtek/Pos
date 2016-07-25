//---------------------------------------------------------------------------


#pragma hdrstop

#include <cassert>
#include "Item.h"
#include "ItemSize.h"

#include <i_item_definition.hh>
#include <i_item_manager.hh>
#include <item_management_booter.hh>

//---------------------------------------------------------------------------

#pragma package(smart_init)

using item_management::i_item_definition;
using item_management::i_item_manager;

bool
TItem::SizeCanBePaidForUsingPoints(
    const UnicodeString &size_name)
{
        const TItemSize * const size = Sizes->SizeGet(size_name);
        assert(size != NULL);

        return size->CanBePaidForUsingPoints;
}

bool
TItem::GetEnabled()
const
{
   i_item_manager &item_manager = item_management::kickstarter::get_item_manager();
   const i_item_definition &item_definition = item_manager.get_item_definition(ItemKey);

   return item_definition.get_enabled();
}

#pragma warn -par

void
TItem::SetEnabled(bool enabled)
{
}

#pragma warn .par

const UnicodeString &
TItem::GetDecoratedName()
const
{
   i_item_manager &item_manager =
     item_management::kickstarter::get_item_manager();
   const i_item_definition &item_definition =
     item_manager.get_item_definition(ItemKey);

   return item_definition.get_decorated_name();
}

TItem::TItem(TListCourse *inOwner)
{
   Options = new TListOptionContainer;
   Sides = new TListSideContainer;
   Sizes = new TListSizeContainer;
   Owner = inOwner;
   Palm_ID = 0;
   Item_ID = 0;
   Course_Key = 0;
   MenuKey = 0; // Only used for printing
   SetColour = static_cast <TColor> (0);
   PrintingGroupOrder = 0;
   ViewableLocations = 0;
   OptionGroup = -1;

   Note = "";
   Item = "";
   Course = "";
   MenuName = "";
   PrintChitNumber = false;
   DisplaySizes = false;
   ExclusivelyAsSide = false;
   Enabled = true;
   ItemType = eFoodItem;
   ItemWeight.SetWeightIn_g(0);
}

__fastcall TItem::~TItem()
{
   delete Options;
   delete Sizes;
   delete Sides;
}

TItemOption * TItem::GetOptionByKey(int inOptionKey)
{
   TItemOption *CurrentOption;
   for (int i = 0; i < Options->Count; i++)
   {
	  CurrentOption = Options->OptionGet(i);
	  if (CurrentOption->OptionKey == inOptionKey)
	  {
		 return CurrentOption;
	  }
   }
   return NULL;
}

TItemOption * TItem::FetchOptionByName(UnicodeString OptionName)
{
   TItemOption *CurrentOption;
   for (int i = 0; i < this->Options->Count; i++)
   {
	  CurrentOption = this->Options->OptionGet(i);
	  if (CurrentOption->Name == OptionName)
	  {
		 return CurrentOption;
	  }
   }
   return NULL;
}
