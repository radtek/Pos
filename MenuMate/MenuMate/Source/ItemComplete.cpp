//---------------------------------------------------------------------------


#pragma hdrstop

#include "ItemComplete.h"
#include "DBSecurity.h"
#include <memory>
//---------------------------------------------------------------------------

#pragma package(smart_init)

bool
TItemComplete::discounts_compare(TItemComplete &other_item)
{
	std::vector<TDiscount>::iterator i = DiscountsBegin();
	std::vector<TDiscount>::iterator j = DiscountsEnd();
	std::vector<TDiscount>::iterator k = other_item.DiscountsBegin();

	bool is_equivalent = DiscountSize() == other_item.DiscountSize();

	for ( ; i != j && is_equivalent; ++i, ++k)
		is_equivalent &= *i == *k;

	return is_equivalent;
}

bool
TItemComplete::options_compare(TItemComplete &other_item)
{
	if ((OptionsSelected != NULL) != (other_item.OptionsSelected != NULL))
		return false;
	else if (!OptionsSelected)
		return true;

	return *OptionsSelected == *other_item.OptionsSelected;
}

bool
TItemComplete::shallow_compare(TItemComplete &other_item)
{
	bool is_equivalent =
	  ItemKey == other_item.ItemKey
	  && discounts_compare(other_item)
	  && options_compare(other_item)
	  && FPrice == other_item.FPrice
	  && SubOrders->Count == other_item.SubOrders->Count
	  && Size == other_item.Size
	  && Note == other_item.Note;

	return is_equivalent;
}

TItemComplete::TItemComplete()
{
	ClearPrice();
	TabKey = 0;
	MenuKey = 0;
	TimeKey = 0;
	Cost = 0;
	Palm_ID = 0;
	TableNo = 0;
	RoomNo = 0;
	SeatNo = 0;
	WebKey = 0;
	ItemType = eFoodItem;
	ThirdPartyKey = 0;
	CoursePrintingOrder = 0;
	OrderedLocation = "";
	ContainerTabType = TabNormal;
	TabType = TabNormal;
	DuplicateDetected = false;
	SetMenuGroup = 0;
	OptionsSelected = new TListOptionContainer;
	OriginalItemKey = 0;
	ItemOrderedFrom = NULL;
	_activeChitNumberKey = 0;
    //MM2038
    IdName="";
    IdNumber="";

}

__fastcall TItemComplete::~TItemComplete()
{
	delete OptionsSelected;

}

TItemComplete::TItemComplete(
  const TItemComplete &initializer)
  : TItemMinorComplete(initializer)
{
   Course_Key = initializer.Course_Key;
   Course_ID = initializer.Course_ID;
   MasterContainer_ID = initializer.MasterContainer_ID;
   Customer_Key = initializer.Customer_Key;
   ItemOrderedFrom = initializer.ItemOrderedFrom;
   Palm_ID = initializer.Palm_ID;
   TableNo = initializer.TableNo;
   SeatNo = initializer.SeatNo;
   TabKey = initializer.TabKey;
   MenuKey = initializer.MenuKey;
   OrderType = initializer.OrderType;
   OrderedLocation = initializer.OrderedLocation;
   TabType = initializer.TabType;
   DuplicateDetected = initializer.DuplicateDetected;
   HappyHour = initializer.HappyHour;
   PrintChitNumber = initializer.PrintChitNumber;
   ItemType = initializer.ItemType;
   Terminal = Terminal;
   TabName = initializer.TabName;
   TabContainerName = initializer.TabContainerName;
   PartyName = initializer.PartyName;
   Note = initializer.Note;
   DiscountedBy = initializer.DiscountedBy;
   CreditedBy = initializer.CreditedBy;
   MasterContainer = initializer.MasterContainer;
   TimeKey = initializer.TimeKey;
   WebKey = initializer.WebKey;
   //MM2038
   IdName = initializer.IdName;
   IdNumber = initializer.IdNumber;

   OptionsSelected = new TListOptionContainer();
   OptionsSelected->OptionCopyList(initializer.OptionsSelected);

   OrderTypeMessage = initializer.OrderTypeMessage;
   ContactsKey = initializer.ContactsKey;

   _activeChitNumberKey = initializer._activeChitNumberKey;

}

TItemComplete &
TItemComplete::operator=(
  const TItemComplete &rhs)
{
   if (this == &TItemMinorComplete::operator=(rhs))
      return *this;

   Course_Key = rhs.Course_Key;
   Course_ID = rhs.Course_ID;
   MasterContainer_ID = rhs.MasterContainer_ID;
   Customer_Key = rhs.Customer_Key;
   ItemOrderedFrom = rhs.ItemOrderedFrom;
   Palm_ID = rhs.Palm_ID;
   TableNo = rhs.TableNo;
   SeatNo = rhs.SeatNo;
   TabKey = rhs.TabKey;
   MenuKey = rhs.MenuKey;
   OrderType = rhs.OrderType;
   OrderedLocation = rhs.OrderedLocation;
   TabType = rhs.TabType;
   DuplicateDetected = rhs.DuplicateDetected;
   HappyHour = rhs.HappyHour;
   PrintChitNumber = rhs.PrintChitNumber;
   ItemType = rhs.ItemType;
   Terminal = Terminal;
   TabName = rhs.TabName;
   TabContainerName = rhs.TabContainerName;
   PartyName = rhs.PartyName;
   Note = rhs.Note;
   DiscountedBy = rhs.DiscountedBy;
   CreditedBy = rhs.CreditedBy;
   MasterContainer = rhs.MasterContainer;
   TimeKey = rhs.TimeKey;
   //MM2038
   IdName = rhs.IdName;
   IdNumber = rhs.IdNumber;
   OptionsSelected = new TListOptionContainer();
   OptionsSelected->OptionCopyList(rhs.OptionsSelected);
   /*std::auto_ptr<TListOptionContainer> new_options(
     new TListOptionContainer());
   new_options->OptionCopyList(rhs.OptionsSelected);

   delete OptionsSelected;
   OptionsSelected = new_options.release();*/

   OrderTypeMessage = rhs.OrderTypeMessage;
   ContactsKey = rhs.ContactsKey;

   _activeChitNumberKey = rhs._activeChitNumberKey;

}

void TItemComplete::Assign(TItemMinor *BaseItem)
{
	TItemMinorComplete::Assign(BaseItem);

	TItemComplete *RetItem = dynamic_cast <TItemComplete*> (BaseItem);
    if (RetItem != NULL)
    {
        RetItem->Course_Key = Course_Key;
        RetItem->Course_ID = Course_ID;
        RetItem->MasterContainer_ID = MasterContainer_ID;
        RetItem->Customer_Key = Customer_Key;

        RetItem->ItemOrderedFrom = ItemOrderedFrom;
        // Req Top Level Info.
        RetItem->Palm_ID = Palm_ID;
        RetItem->TableNo = TableNo;
        RetItem->SeatNo = SeatNo;
        RetItem->TabKey = TabKey;
        RetItem->MenuKey = MenuKey;
        RetItem->OrderType = OrderType;

        RetItem->OrderedLocation = OrderedLocation;
        RetItem->TabType = TabType;

        RetItem->DuplicateDetected = DuplicateDetected;
        RetItem->HappyHour = HappyHour;
        RetItem->PrintChitNumber = PrintChitNumber;
        RetItem->ItemType = ItemType;
        RetItem->Terminal = Terminal;
        RetItem->TabName = TabName;
        RetItem->TabContainerName = TabContainerName;
        RetItem->PartyName = PartyName;
        RetItem->Note = Note;
        RetItem->DiscountedBy = DiscountedBy;
        RetItem->CreditedBy = CreditedBy;
        RetItem->MasterContainer = MasterContainer;
        RetItem->TimeKey = TimeKey;
        RetItem->OptionsSelected = new TListOptionContainer();
        RetItem->OptionsSelected->OptionCopyList(OptionsSelected);

        RetItem->OrderTypeMessage = OrderTypeMessage;
        RetItem->ContactsKey = ContactsKey;

        RetItem->SetActiveChitNumberKey(GetActiveChitNumberKey());
        RetItem->ChitNumber = ChitNumber;
        //MM2038
       RetItem->IdName = IdName;
       RetItem->IdNumber = IdNumber;

    }
}