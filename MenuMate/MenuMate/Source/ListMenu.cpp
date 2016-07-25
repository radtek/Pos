//---------------------------------------------------------------------------


#pragma hdrstop

#include "ListMenu.h"
#include "ListCourse.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TListMenu::TListMenu()
{
   MenuKey = 0;
   Menu_Type = static_cast <TMenuType> (0);
   EnabledStateSync = 0;
   Description;
   SwapInCommand = eMenuAddReplace;
   Sizes = new TListSizeContainer;
}

std::auto_ptr<std::vector<TItem *> >
TListMenu::FetchItemsBySetMenuMask(
    const unsigned long set_menu_mask)
{
   std::auto_ptr<std::vector<TItem *> > collected_set_menu_items(
     new std::vector<TItem *>());

   for (int i = 0; i < Count; ++i) {
      TListCourse &course = *CourseGet(i);

      course.FetchItemsBySetMenuMask(*collected_set_menu_items,
                                     set_menu_mask);
   }

   if (collected_set_menu_items->size() == 0)
      collected_set_menu_items.reset();

   return collected_set_menu_items;
}

TItemSize *
TListMenu::FetchItemSizeByBarcode(
    const UnicodeString barcode)
{
        TItemSize *size = NULL;

        for (int i = 0; size == NULL && i < Count; ++i)
                size = CourseGet(i)->FetchItemSizeByBarcode(barcode);

        return size;
}

__fastcall TListMenu::~TListMenu()
{
   while (Count > 0)
   {
	  CourseDelete(CourseGet(0));
   }

   delete Sizes;
}

TListCourse * TListMenu::FetchCourseByName(UnicodeString CourseName)
{
   for (int i = 0; i < Count; i++)
   {
	  TListCourse *Course = CourseGet(i);
	  if (Course->Course_Name == CourseName)
	  {
		 return Course;
	  }
   }
   return NULL;
}

TListCourse * TListMenu::FetchCourseByID(long inCourse_ID)
{
   for (int i = 0; i < Count; i++)
   {
	  TListCourse *Course = CourseGet(i);
	  if (Course->Palm_ID == inCourse_ID)
	  {
		 return Course;
	  }
   }
   return NULL;
}

TListCourse * TListMenu::FetchCourseByKey(long inCourseKey)
{
   for (int i = 0; i < Count; i++)
   {
	  TListCourse *Course = CourseGet(i);
	  if (Course->Course_Key == inCourseKey)
	  {
		 return Course;
	  }
   }
   return NULL;
}

void TListMenu::CourseDelete(TListCourse *in)
{
   if (in != NULL)
   {
	  if (IndexOf(in) != -1)
	  {
		 if (in->Owner == this)
		 {
			Remove(in);
			delete(TListCourse*)in;
			in = NULL;
		 }
		 else
		 {
			Remove(in);
		 }
	  }
   }
}

int TListMenu::CourseAdd(TListCourse *in)
{
   in->Owner = this;
   return Add(in);
}

void TListMenu::CourseSort()
{
   if (OptComp != NULL)
	  Sort(OptComp);
}

void TListMenu::CourseCopyList(TListMenu *inContainer)
{
   while (Count > 0)
   {
	  CourseDelete(0);
   }
   for (int i = 0; i < inContainer->Count; i++)
   {
	  TListCourse *NewCourse = inContainer->CourseGet(i);
	  CourseAdd(NewCourse);
   }
}

TListCourse *TListMenu::CourseGet(int Index)
{
   if (Count > Index)
   {
	  return(TListCourse*)Items[Index];
   }
   return NULL;
}

// ---------------------------------------------------------------------------
TItem * TListMenu::FetchItemByID(long inItem_ID)
{
   for (int k = 0; k < Count; k++)
   {
	  TListCourse *Course = CourseGet(k);
	  for (int i = 0; i < Course->Count; i++)
	  {
		 TItem *Item = Course->ItemGet(i);
#ifdef PalmMate
		 if (Item->Palm_ID == inItem_ID)
		 {
			return Item;
		 }
#endif
#ifdef MenuMate
		 if (Item->Item_ID == inItem_ID)
		 {
			return Item;
		 }
#endif
	  }
   }
   return NULL;
}

// ---------------------------------------------------------------------------
TItem * TListMenu::FetchItemByKey(long inItemKey)
{
   for (int k = 0; k < Count; k++)
   {
	  TListCourse *Course = CourseGet(k);
	  for (int i = 0; i < Course->Count; i++)
	  {
		 TItem *Item = Course->ItemGet(i);
		 if (Item->ItemKey == inItemKey)
		 {
			return Item;
		 }
	  }
   }
   return NULL;
}

// ------------------------------------------------------------------------------

int TListMenu::ServingCourseSelectableCount()
{
   int Count = 0;
   std::vector <TServingCourse> ::iterator iServingCourse = ServingCourses.begin();
   while (iServingCourse != ServingCourses.end())
   {
	  if (iServingCourse->Selectable)
	  {
		 Count++;
	  }
	  iServingCourse++;
   }
   return Count;
}

int TListMenu::ServingCourseGetFirstVisibleIndex()
{
   int Index = -1;
   std::vector <TServingCourse> ::iterator iServingCourse = ServingCourses.begin();
   while (iServingCourse != ServingCourses.end())
   {
	  if (iServingCourse->Selectable)
	  {
		 Index = iServingCourse - ServingCourses.begin();
		 break;
	  }
	  iServingCourse++;
   }
   return Index;
}

bool TListMenu::GetServingCourseByID(USHORT ServingCourseID, TServingCourse &ServingCourse)
{
   bool Success = false;

   std::vector <TServingCourse> ::iterator iServingCourse = ServingCourses.begin();
   while (iServingCourse != ServingCourses.end())
   {
	  if (iServingCourse->ServingCourseID == ServingCourseID)
	  {
		 ServingCourse = *iServingCourse;
		 Success = true;
		 break;
	  }
	  iServingCourse++;
   }
   return Success;
}

TServingCourse TListMenu::GetServingCourse(int ServingCourseKey)
{
   TServingCourse RetVal;
   for (int i = 0; i < ServingCourses.size(); i++)
   {
	  if (ServingCourses[i].ServingCourseKey == ServingCourseKey)
	  {
		 RetVal = ServingCourses[i];
	  }
   }
   return RetVal;
}

bool TListMenu::GetServingCourseExists(int ServingCourseKey)
{
   bool ServingCourseExists = false;
   for (int i = 0; i < ServingCourses.size(); i++)
   {
	  if (ServingCourses[i].ServingCourseKey == ServingCourseKey)
	  {
		 ServingCourseExists = true;
	  }
   }
   return ServingCourseExists;
}