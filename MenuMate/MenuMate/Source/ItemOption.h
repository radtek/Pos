//---------------------------------------------------------------------------
#ifndef ItemOptionH
#define ItemOptionH

#include <System.hpp>
#include "PrintInfo.h"
#include "ListOptionContainer.h"

class TListOptionContainer;

class TItemOption : public TObject
{
public:
	unsigned int OptionKey;
	long Palm_ID;
	unsigned short OptionID;
	UnicodeString Name;
	UnicodeString KitchenName;
	bool Enabled;
	bool IsPlus;
	unsigned int ForcedMask;
	int Flags;
	int GroupNumber;
	bool PlusOption;
	bool MinusOption;
	bool AllowSkip;
	bool DisallowMuliSelect;
    int OptionOrder;
   // Currency Quantity;
	TPrintInfo FontInfo;

	TListOptionContainer *Owner;

	TItemOption();
	friend bool operator < (const TItemOption & f1, const TItemOption & f2);
	friend bool operator == (const TItemOption & f1, const TItemOption & f2);
};
#endif
