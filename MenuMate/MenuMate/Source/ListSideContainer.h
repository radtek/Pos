//---------------------------------------------------------------------------

#ifndef ListSideContainerH
#define ListSideContainerH
//---------------------------------------------------------------------------
#include "ItemSide.h"
#include "ListCourse.h"
#include <Generics.Collections.hpp>

#ifdef  PalmMate
#include <map>
class TListCourse;
typedef std::map <long, TItem> ItemList;
typedef std::map <long, TListCourse> CourseList;
#endif


class TListSideContainer : public TList
{
public:
	typedef int __fastcall(*TListSortCompare)(void*Item1, void*Item2);

	TListSortCompare SideComp;

	void SideSort();
	bool Compare(TListSideContainer *f1);

	void SideDelete(TItemSide *in);
	void SideClear();
	int SideAdd(TItem *inMasterItem, int MasterKey, int ItemKey, int inIOO, int inGroupNo, int inMaxSelect, bool inAllowSkip);
	int SideAdd(TItemSide *inSide);
	void SidesRetriveMaskedList(int inGroupNo, TListSideContainer *inSidesList);

	void SideCopyList(TListSideContainer *inSideList);
	void SideAddList(TListSideContainer *inSideList);
	// When displaying the options loop though all groups 1 - 15 with each group
	// check the options mask against the items postion to determine weather or not it;s displayed.

	int SideIndex(TItemSide *Opt);
	TItemSide *SideGet(int Index);

	TListSideContainer();
	__fastcall virtual ~TListSideContainer();
};
#endif
