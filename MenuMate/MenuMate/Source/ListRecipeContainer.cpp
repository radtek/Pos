//---------------------------------------------------------------------------


#pragma hdrstop

#include "ListRecipeContainer.h"
#include "ItemRecipe.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

TListRecipeContainer::TListRecipeContainer()
{
   AdjustmentType = eStockSale;
}

__fastcall TListRecipeContainer::~TListRecipeContainer()
{
   RecipeClear();
}

void TListRecipeContainer::RecipeClear()
{
   while (Count > 0)
   {
	  RecipeDelete(RecipeGet(0));
   }
}

TItemRecipe *TListRecipeContainer::RecipeGet(int Index)
{
   if (Count > Index)
   {
	  return(TItemRecipe*)Items[Index];
   }
   return NULL;
}

void TListRecipeContainer::RecipeCopyList(TListRecipeContainer *inRecipeList)
{
   RecipeClear();
   for (int i = 0; i < inRecipeList->Count; i++)
   {
	  TItemRecipe *ItemRecipe = new TItemRecipe;
	  *ItemRecipe = *(inRecipeList->RecipeGet(i));
	  RecipeAdd(ItemRecipe);
   }
}

void TListRecipeContainer::Inverse()
{
   for (int i = 0; i < Count; i++)
   {
	  TItemRecipe *ItemRecipe = RecipeGet(i);
	  ItemRecipe->Qty = -ItemRecipe->Qty;
	  ItemRecipe->Cost = -ItemRecipe->Cost;
   }
}

int TListRecipeContainer::RecipeAdd(TItemRecipe *in)
{
   in->Owner = this;
   return Add(in);
}

void TListRecipeContainer::RecipeDelete(TItemRecipe *in)
{
   if (in != NULL)
   {
	  if (IndexOf(in) != -1)
	  {
		 if (in->Owner == this)
		 {
			Remove(in);
			delete(TItemRecipe*)in;
			in = NULL;
		 }
		 else
		 {
			Remove(in);
		 }
	  }
   }
}

void TListRecipeContainer::Disable()
{
   for (int i = 0; i < Count; i++)
   {
	  TItemRecipe *ItemRecipe = RecipeGet(i);
	  ItemRecipe->Enabled = false;
   }
}

void TListRecipeContainer::Enable()
{
   for (int i = 0; i < Count; i++)
   {
	  TItemRecipe *ItemRecipe = RecipeGet(i);
	  ItemRecipe->Enabled = true;
   }
}