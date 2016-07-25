//---------------------------------------------------------------------------
#ifndef ListMenuContainerH
#define ListMenuContainerH

#include <Classes.hpp>
#include <memory>
#include <vector>

class TListMenu;
class TItemSize;
class TItem;

class TListMenuContainer : public TList
{
public:
	typedef int __fastcall(*TListSortCompare)(void*Item1, void*Item2);

	TListSortCompare OptComp;

	int MenuAdd(TListMenu *in);
	void MenuDelete(TListMenu *in);
	void MenuClear();
	void MenuSort();
	void MenuCopyList(TListMenu *inCategoryList);
	TListMenu *MenuGet(int Index);
	TListMenu *MenuGet(UnicodeString MenuName);
	TListMenu *MenuGetNext(TListMenu * Menu);

        TItemSize *FetchItemSizeByBarcode(const UnicodeString);

        std::auto_ptr<std::vector<TItem *> >
            FetchItemsBySetMenuMask(const unsigned long);

        // retrieves set menu items specific to a menu
        std::auto_ptr<std::vector<TItem *> >
            FetchItemsBySetMenuMaskByMenu(const unsigned long, const UnicodeString);

	// std::map<int,TServingCourse> ItemsOrderedServingCourses;
	virtual __fastcall ~TListMenuContainer();
};
#endif
