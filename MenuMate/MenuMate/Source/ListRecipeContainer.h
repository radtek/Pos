//---------------------------------------------------------------------------

#ifndef ListRecipeContainerH
#define ListRecipeContainerH
//---------------------------------------------------------------------------
#include <Generics.Collections.hpp>

enum eStockAdjustmentType {eStockSale,eStockWriteOffs,eStockRefund};

class TItemRecipe;

class TListRecipeContainer : public TList
{
public:
	eStockAdjustmentType AdjustmentType; // eStockAdjustmentType {eSale,eWriteOffs};

	void RecipeClear();
	void RecipeDelete(TItemRecipe *in);
	int RecipeAdd(TItemRecipe *inOption);
	void RecipeCopyList(TListRecipeContainer *inRecipeList);
	TItemRecipe *RecipeGet(int Index);
	void Inverse();
	TListRecipeContainer();
	__fastcall virtual ~TListRecipeContainer();
	void Disable();
	void Enable();

};

#endif
