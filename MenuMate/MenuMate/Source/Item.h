//---------------------------------------------------------------------------

#ifndef ItemH
#define ItemH

#include "ListSizeContainer.h"
#include "ListOptionContainer.h"
#include "ListSideContainer.h"
#include "ItemOption.h"
#include "ItemMinor.h"
#include "Weight.h"

class TListCourse;
//---------------------------------------------------------------------------
enum TItemType {eFoodItem,eDrinksItem};

class TItem : public TItemMinor
{
public:
	int Palm_ID;
	long Item_ID;
	long Course_Key;
	long OptionGroup;
	// TODO 4 -o Michael -c Printing : Remove When Printing Engine Finished
	long MenuKey; // Only used for printing
	bool PrintChitNumber;
	bool DisplaySizes;

   __property bool Enabled = { read = GetEnabled, write = SetEnabled };

	bool ExclusivelyAsSide;
	UnicodeString Note;
	TColor SetColour;

	TItemType ItemType;
	int PrintingGroupOrder;

	long ViewableLocations; // Used to Only Show orders in the Locations allowable
	// for that Item. i.e Determines weather
	// an ORDER can be seen on the Palm.

	TListCourse *Owner;

	TListSizeContainer *Sizes;
	TListOptionContainer *Options;
	TListSideContainer *Sides;

	TItemOption * TItem::GetOptionByKey(int inOptionKey);
	TItemOption * FetchOptionByName(UnicodeString OptionName);
	TItem(TListCourse *inOwner);
	virtual __fastcall ~TItem();

    const UnicodeString &GetDecoratedName() const;
    bool SizeCanBePaidForUsingPoints(const UnicodeString &);
	TWeight ItemWeight;
private:
   bool GetEnabled() const;
   void SetEnabled(bool enabled);
};

#endif
