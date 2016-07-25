//---------------------------------------------------------------------------

#ifndef ListMenuH
#define ListMenuH
//---------------------------------------------------------------------------
#include "ListMenuContainer.h"
#include "ListSizeContainer.h"
#include "ListCourse.h"
#include <vector>

enum eMenuCommand {eMenuAddReplace,eMenuRemove};
enum TMenuType {eFoodMenu,eDrinksMenu};

class TListMenu : public TList
{
public:
	eMenuCommand SwapInCommand; // Ture if all menus begin swaped in are new. ( do not use internal )
	UnicodeString MenuName;
	long MenuKey;
	TMenuType Menu_Type;
	int EnabledStateSync; // Has Enable Disables Changed.
	UnicodeString Description;
	TListMenuContainer *Owner;
	TListSizeContainer *Sizes;

	TListMenu();
	virtual __fastcall ~TListMenu();

	TItem * FetchItemByID(long inItem_ID);
	TItem * FetchItemByKey(long inItem_Key);
        TItemSize *FetchItemSizeByBarcode(const UnicodeString);

	TListCourse * FetchCourseByName(UnicodeString CourseName);
	TListCourse * FetchCourseByID(long inCourse_ID);
	TListCourse * FetchCourseByKey(long inCourseKey);

        std::auto_ptr<std::vector<TItem *> >
            FetchItemsBySetMenuMask(const unsigned long);

	typedef int __fastcall(*TListSortCompare)(void*Item1, void*Item2);

	TListSortCompare OptComp;

	int CourseAdd(TListCourse *in);
	void CourseDelete(TListCourse *in);
	void CourseSort();
	void CourseCopyList(TListMenu *inOptionList);
	TListCourse *CourseGet(int Index);

	std::vector <TServingCourse> ServingCourses;
	TServingCourse LastSelectedServingCourse;

	TServingCourse GetServingCourse(int ServingCourseKey);
	bool GetServingCourseExists(int ServingCourseKey);
	bool GetServingCourseByID(USHORT ServingCourseID, TServingCourse &ServingCourse);
	int ServingCourseSelectableCount();
	int ServingCourseGetFirstVisibleIndex();
};
#endif
