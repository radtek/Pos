//---------------------------------------------------------------------------

#ifndef ListOptionContainerH
#define ListOptionContainerH

#include "ItemOption.h"
#include <Generics.Collections.hpp>

class TItemOption;

//---------------------------------------------------------------------------
class TListOptionContainer : public TList
{
public:

/*	typedef int __fastcall(*TListSortCompare)(void*Item1, void*Item2);
	TListSortCompare OptComp;*/

	void OptionDelete(TItemOption *in);
	void OptionClear();
	int OptionAdd(TItemOption *inOption);

	void OptionSort();

	void OptionCopyList(TListOptionContainer *inOptionList);
	void OptionAddList(TListOptionContainer *inOptionList);
	void OptionRetriveMaskedList(int Group, int Mask, TListOptionContainer *inOptionList);
	// When displaying the options loop though all groups 1 - 15 with each group
	// check the options mask against the items postion to determine weather or not it;s displayed.

	void OptionRetrieveForcedList( __int32 inGroup, __int32 inItemKey, TListOptionContainer* outForcedOptionList );

	bool Compare(TListOptionContainer *f1);
	int OptionIndex(TItemOption *Opt);
	TItemOption *OptionGet(int Index);

	TListOptionContainer();
	__fastcall virtual ~TListOptionContainer();

	bool operator==(TListOptionContainer &rhs);

protected:
	TItemOption* OptionWithGrpNo( __int32 inGrpNo, __int32 inOpKey );
    TItemOption* OptionWithflagNo( __int32 inGrpNo, __int32 flag, UnicodeString option_name, int order);
};

#endif
