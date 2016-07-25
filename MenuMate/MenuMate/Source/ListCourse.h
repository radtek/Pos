//---------------------------------------------------------------------------

#ifndef ListCourseH
#define ListCourseH

#include <Classes.hpp>
#include "Item.h"
#include <memory>
#include <vector>

//---------------------------------------------------------------------------
class TListMenu;

class TListCourse : public TList
{
public:
	long Palm_ID;
	UnicodeString Course_Name; // Course/Class
	UnicodeString CourseKitchenName;
	long ViewableLocations;
	int Course_Key;
	int DefaultServingCourseKey;
	bool No_Default_Serving_Course;

	TItem * FetchItemByName(UnicodeString Name);
   TItemSize *FetchItemSizeByBarcode(const UnicodeString);

   std::auto_ptr<std::vector<TItem *> >
   FetchItemsBySetMenuMask(const unsigned long);

   unsigned long
   FetchItemsBySetMenuMask(
     std::vector<TItem *> &collected_set_items,
     const unsigned long set_menu_mask);

	TListMenu *Owner;

	typedef int __fastcall(*TListSortCompare)(void*Item1, void*Item2);

	TListSortCompare OptComp;

	void ItemClear();
	int ItemAdd(TItem *in);
	void ItemDelete(TItem *in);
	void ItemSort();
	void ItemCopyList(TListCourse &inOptionList);
	TItem *ItemGet(int Index);
	virtual __fastcall ~TListCourse();
};
#endif
