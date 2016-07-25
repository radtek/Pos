//---------------------------------------------------------------------------


#pragma hdrstop

#include "ListCourse.h"
#include "ItemUtilities.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

__fastcall TListCourse::~TListCourse()
{
   while (Count > 0)
   {
	  ItemDelete(ItemGet(0));
   }
}

unsigned long
TListCourse::FetchItemsBySetMenuMask(
  std::vector<TItem *> &collected_set_items,
  const unsigned long set_menu_mask)
{
   std::vector<TItem *>::size_type n_items_on_entry =
     collected_set_items.size();

   for (int i = 0; i < Count; ++i) {
      TItem * const item = ItemGet(i);

      if (item_has_sizes_with_set_menu_mask(*item, set_menu_mask) == true)
         collected_set_items.push_back(item);
   }

   return collected_set_items.size() - n_items_on_entry;
}

std::auto_ptr<std::vector<TItem *> >
TListCourse::FetchItemsBySetMenuMask(
    const unsigned long set_menu_mask)
{
   std::auto_ptr<std::vector<TItem *> > set_menu_items(
     new std::vector<TItem *>());

   if (FetchItemsBySetMenuMask(*set_menu_items, set_menu_mask) == 0)
      set_menu_items.reset();

   return set_menu_items;
}

void TListCourse::ItemDelete(TItem *in)
{
   if (in != NULL)
   {
	  if (IndexOf(in) != -1)
	  {
		 if (in->Owner == this)
		 {
			Remove(in);
			delete(TItem*)in;
			in = NULL;
		 }
		 else
		 {
			Remove(in);
		 }
	  }
   }
}

int TListCourse::ItemAdd(TItem *in)
{
   return Add(in);
}

TItemSize *
TListCourse::FetchItemSizeByBarcode(
    const UnicodeString barcode)
{
        TItemSize *size = NULL;

        for (int i = 0; size == NULL && i < Count; ++i)
                size = ItemGet(i)->Sizes->SizeGetByBarcode(barcode);

        return size;
}

void TListCourse::ItemClear()
{
   while (Count > 0)
   {
	  ItemDelete(ItemGet(0));
   }
}

void TListCourse::ItemSort()
{
   if (OptComp != NULL)
	  Sort(OptComp);
}

void TListCourse::ItemCopyList(TListCourse &inCourse)
{
   while (Count > 0)
   {
	  ItemDelete(0);
   }
   for (int i = 0; i < inCourse.Count; i++)
   {
	  TItem *NewItem = inCourse.ItemGet(i);
	  ItemAdd(NewItem);
   }
}

TItem *TListCourse::ItemGet(int Index)
{
   if (Count > Index)
   {
	  return(TItem*)Items[Index];
   }
   return NULL;
}

TItem * TListCourse::FetchItemByName(UnicodeString Name)
{
   for (int i = 0; i < Count; i++)
   {
	  TItem *CurrentItem = ItemGet(i);
	  if (CurrentItem->Item == Name)
	  {
		 return CurrentItem;
	  }
   }
   return NULL;
}
