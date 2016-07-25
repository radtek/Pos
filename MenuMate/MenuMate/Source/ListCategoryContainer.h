//---------------------------------------------------------------------------

#ifndef ListCategoryContainerH
#define ListCategoryContainerH
//---------------------------------------------------------------------------
#include <Generics.Collections.hpp>

class TItemSizeCategory;

class TListCategoryContainer : public TList
{
public:
	int FinancialCategoryKey;
	UnicodeString FinancialCategoryGroup;
	UnicodeString FinancialCategory;
    UnicodeString FinancialCategoryGLCode;
	void CategoryClear();
	void CategoryDelete(TItemSizeCategory *in);
	int CategoryAdd(TItemSizeCategory *inSize);
	void CategoryCopyList(TListCategoryContainer *inCategoryList);
	TItemSizeCategory *CategoryGet(int Index);
	TItemSizeCategory *CategoryGet(UnicodeString Name);
	TListCategoryContainer();
	__fastcall virtual ~TListCategoryContainer();
	TItemSizeCategory *CategoryGetByKey(int Key);
};
#endif
