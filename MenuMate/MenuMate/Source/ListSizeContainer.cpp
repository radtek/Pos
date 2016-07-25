//---------------------------------------------------------------------------


#pragma hdrstop

#include "ListSizeContainer.h"
#include "ItemSize.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TListSizeContainer::TListSizeContainer()
{
}

__fastcall TListSizeContainer::~TListSizeContainer()
{
   SizeClear();
}

void TListSizeContainer::SizeClear()
{
   while (Count > 0)
   {
	  SizeDelete(SizeGet(0));
   }
}

TItemSize *
TListSizeContainer::SizeGetByBarcode(
    const UnicodeString barcode)
{
        for (int i = 0; i < Count; ++i) {
                TItemSize *size = SizeGet(i);
                if (size->Barcode == barcode)
                        return size;
        }

        return NULL;
}

TItemSize *TListSizeContainer::SizeGet(int Index)
{
   if (Count > Index)
   {
	  return(TItemSize*)Items[Index];
   }
   return NULL;
}

TItemSize *TListSizeContainer::SizeGet(UnicodeString Name)
{
   for (int i = 0; i < Count; i++)
   {
	  TItemSize *ItemSize = (TItemSize*)Items[i];
	  if (ItemSize->Name == Name)
	  {
		 return ItemSize;
	  }
   }
   return NULL;
}
TItemSize *TListSizeContainer::SizeGetByItemSizeKey(long ItemSizeKey)
{
   for (int i = 0; i < Count; i++)
   {
	  TItemSize *ItemSize = (TItemSize*)Items[i];
	  if (ItemSize->ItemSizeKey  == ItemSizeKey)
	  {
		 return ItemSize;
	  }
   }
   return NULL;
}
TItemSize *TListSizeContainer::SizeGetByPalmID(int PalmID)
{
   for (int i = 0; i < Count; i++)
   {
	  TItemSize *ItemSize = (TItemSize*)Items[i];
	  if (ItemSize->Palm_ID == PalmID)
	  {
		 return ItemSize;
	  }
   }
   return NULL;
}

void TListSizeContainer::SizeCopyList(TListSizeContainer *inSizeList)
{
   SizeClear();
   for (int i = 0; i < inSizeList->Count; i++)
   {
	  TItemSize *ItemSize = new TItemSize;
	  ItemSize->SizeCopy(inSizeList->SizeGet(i));
	  SizeAdd(ItemSize);
   }
}

int TListSizeContainer::SizeAdd(TItemSize *in)
{
   in->Owner = this;
   return Add(in);
}

void TListSizeContainer::SizeDelete(TItemSize *in)
{
   if (in != NULL)
   {
	  if (IndexOf(in) != -1)
	  {
		 if (in->Owner == this)
		 {
			Remove(in);
			delete(TItemSize*)in;
			in = NULL;
		 }
		 else
		 {
			Remove(in);
		 }
	  }
   }
}