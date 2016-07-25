//---------------------------------------------------------------------------

#ifndef ItemSideH
#define ItemSideH

#include <System.hpp>
//---------------------------------------------------------------------------

class TListSideContainer;
class TItem;

class TItemSide : public TObject
{
public:
	TItem *MasterItem;
	TItem *Item;
	long MasterItemKey;
	long ItemKey;
	int GroupNo;
	int MaxSelect;
	long IOO;
	bool AllowSkip;
	TListSideContainer *Owner;

	TItemSide(TListSideContainer *inOwner);
	/*
	friend bool operator<(const TItemSide &f1, const TItemSide &f2);
	friend bool operator==(const TItemSide &f1, const TItemSide &f2); */
};
#endif
