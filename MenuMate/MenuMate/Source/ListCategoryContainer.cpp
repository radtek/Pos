//---------------------------------------------------------------------------


#pragma hdrstop

#include "ListCategoryContainer.h"
#include "ItemSizeCategory.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

TListCategoryContainer::TListCategoryContainer()
{
   FinancialCategory = "";
   FinancialCategoryGroup = "";
   FinancialCategoryGLCode = "";
}

__fastcall TListCategoryContainer::~TListCategoryContainer()
{
   CategoryClear();
}

void TListCategoryContainer::CategoryClear()
{
   while (Count > 0)
   {
	  CategoryDelete(CategoryGet(0));
   }
}

TItemSizeCategory *TListCategoryContainer::CategoryGet(int Index)
{
   if (Count > Index)
   {
	  return(TItemSizeCategory*)Items[Index];
   }
   return NULL;
}

TItemSizeCategory *TListCategoryContainer::CategoryGetByKey(int Key)
{
   TItemSizeCategory *Retval = NULL;
   for (int i = 0; i < Count; i++)
   {
      if(((TItemSizeCategory*)Items[i])->CategoryKey == Key)
      {
       	Retval = (TItemSizeCategory*)Items[i];
      }
   }
   return Retval;
}


void TListCategoryContainer::CategoryCopyList(TListCategoryContainer *inCategoryList)
{
   CategoryClear();
   FinancialCategory = inCategoryList->FinancialCategory;
   FinancialCategoryGroup = inCategoryList->FinancialCategoryGroup;
   FinancialCategoryKey = inCategoryList->FinancialCategoryKey;
   FinancialCategoryGLCode = inCategoryList->FinancialCategoryGLCode;
   for (int i = 0; i < inCategoryList->Count; i++)
   {
	  TItemSizeCategory *ItemCategory = new TItemSizeCategory;
	  ItemCategory->CategoryCopy(inCategoryList->CategoryGet(i));
	  CategoryAdd(ItemCategory);
   }
}

int TListCategoryContainer::CategoryAdd(TItemSizeCategory *in)
{
   in->Owner = this;
   return Add(in);
}

void TListCategoryContainer::CategoryDelete(TItemSizeCategory *in)
{
   if (in != NULL)
   {
	  if (IndexOf(in) != -1)
	  {
		 if (in->Owner == this)
		 {
			Remove(in);
			delete(TItemSizeCategory*)in;
			in = NULL;
		 }
		 else
		 {
			Remove(in);
		 }
	  }
   }
}
