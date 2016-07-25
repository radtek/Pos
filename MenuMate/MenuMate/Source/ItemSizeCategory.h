//---------------------------------------------------------------------------

#ifndef ItemSizeCategoryH
#define ItemSizeCategoryH

#include <System.hpp>

class TListCategoryContainer;

class TItemSizeCategory
{
public:
	TListCategoryContainer *Owner;
	int CategoryKey;
	UnicodeString Category;
    UnicodeString GLCode;
	void CategoryCopy(TItemSizeCategory * Copy);
	TItemSizeCategory();
};

#endif
