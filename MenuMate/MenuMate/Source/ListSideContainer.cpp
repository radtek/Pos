//---------------------------------------------------------------------------


#pragma hdrstop

#include "ListSideContainer.h"
#include "Item.h"
#include "ListCourse.h"
#include "ListMenu.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

int __fastcall OptionCompare(void*Item1, void*Item2);
int __fastcall SideCompare(void*Item1, void*Item2);
int __fastcall SubOrderCompare(void*Item1, void*Item2);

// ------------------------------------------------------------------------------
int __fastcall SideCompare(void *Item1, void *Item2)
{
   TItemSide* Side1 = (TItemSide*)Item1;
   TItemSide* Side2 = (TItemSide*)Item2;

   if (Side1->GroupNo > Side2->GroupNo)
   {
	  return 1;
   }
   else if (Side1->GroupNo == Side2->GroupNo)
   {
	  if (Side1->IOO > Side2->IOO)
	  {
		 return 1;
	  }
	  else if (Side1->IOO == Side2->IOO)
	  {
		 return 0;
	  }
	  else
	  {
		 return -1;
	  }
   }
   else
   {
	  return -1;
   }
}

int SidesCompare(TListSideContainer *SideList1, TListSideContainer *SideList2)
{
   // May also have to sort based on master container.
   // Sort Options.
   SideList1->SideSort();
   SideList2->SideSort();

   // begin list comparasion.
   if (SideList1->Count > SideList2->Count)
   {
	  return 1;
   }
   else if (SideList1->Count == SideList2->Count)
   {
	  for (int i = 0; i < SideList1->Count; i++)
	  {
		 int RetVal = SideCompare(SideList1->SideGet(i), SideList1->SideGet(i)); // Overloaded Function.
		 if (RetVal != 0)
		 {
			return RetVal;
		 }
	  }
	  return 0;
   }
   else
   {
	  return -1;
   }
}


TListSideContainer::TListSideContainer()
{
   SideComp = &SideCompare;
}

__fastcall TListSideContainer::~TListSideContainer()
{
   while (Count > 0)
   {
	  SideDelete(SideGet(0));
   }
}

void TListSideContainer::SideDelete(TItemSide *in)
{
   if (IndexOf(in) != -1)
   {
	  if (in->Owner == this)
	  {
		 Remove(in);
		 delete(TItemSide*)in;
		 in = NULL;
	  }
	  else
	  {
		 Remove(in);
	  }
   }
}

void TListSideContainer::SideClear()
{
   while (Count > 0)
   {
	  SideDelete(SideGet(0));
   }
}

int TListSideContainer::SideAdd(TItem *inMasterItem, int MasterKey, int ItemKey, int inIOO, int inGroupNo, int inMaxSelect,
   bool inAllowSkip)
{
   TItemSide * Side = new TItemSide(this);
   Side->MasterItem = inMasterItem;
   Side->MasterItemKey = MasterKey;
   Side->ItemKey = ItemKey;
   Side->GroupNo = inGroupNo;
   Side->MaxSelect = inMaxSelect;
   Side->IOO = inIOO;
   Side->AllowSkip = inAllowSkip;
   return Add(Side);
}

int TListSideContainer::SideAdd(TItemSide *inSide)
{
   return Add(inSide);
}

void TListSideContainer::SideCopyList(TListSideContainer *inSideList)
{
   SideClear();
   for (int i = 0; i < inSideList->Count; i++)
   {
	  TItemSide *NewSide = inSideList->SideGet(i);
	  SideAdd(NewSide);
   }
}

void TListSideContainer::SideAddList(TListSideContainer *inSideList)
{
   for (int i = 0; i < inSideList->Count; i++)
   {
	  TItemSide *NewSide = inSideList->SideGet(i);
	  SideAdd(NewSide);
   }
}

int TListSideContainer::SideIndex(TItemSide *Side)
{
   return IndexOf(Side);
}

TItemSide *TListSideContainer::SideGet(int Index)
{
   if (Count > Index)
   {
	  return(TItemSide*)Items[Index];
   }
   return NULL;
}

bool TListSideContainer::Compare(TListSideContainer *SideList2)
{
   if (SideCompare(this, SideList2) == 0)
   {
	  return true;
   }
   return false;
}

void TListSideContainer::SideSort()
{
   Sort(SideComp);
}

void TListSideContainer::SidesRetriveMaskedList(int inGroupNo, TListSideContainer *inItemsList)
{

   inItemsList->SideClear();
   SideSort();
   for (int i = 0; i < Count; i++)
   {
	  TItemSide *NewSide = SideGet(i);
	  if (NewSide->GroupNo == inGroupNo)
	  {
		 TItem *Item = NewSide->MasterItem->Owner->Owner->FetchItemByKey(NewSide->ItemKey);
		 if (Item)
		 {
			NewSide->Item = Item;
			inItemsList->SideAdd(NewSide);
		 }
	  }
   }
}