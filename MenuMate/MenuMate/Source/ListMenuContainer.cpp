//---------------------------------------------------------------------------


#pragma hdrstop

#include "ListMenuContainer.h"
#include "ListMenu.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

// ------------------------------------------------------------------------------

__fastcall TListMenuContainer::~TListMenuContainer()
{
   while (Count > 0)
   {
	  MenuDelete(MenuGet(0));
   }
}

std::auto_ptr<std::vector<TItem *> >
TListMenuContainer::FetchItemsBySetMenuMask(
    const unsigned long set_menu_mask)
{
        std::auto_ptr<std::vector<TItem *> > set_menu_items;

        for (int i = 0; !set_menu_items.get() && i < Count; ++i)
                set_menu_items =
                    MenuGet(i)->FetchItemsBySetMenuMask(set_menu_mask);

        return set_menu_items;
}

std::auto_ptr<std::vector<TItem *> >
TListMenuContainer::FetchItemsBySetMenuMaskByMenu(
    const unsigned long set_menu_mask,
    const UnicodeString menu_name)
{
        std::auto_ptr<std::vector<TItem *> > set_menu_items =
            MenuGet(menu_name)->FetchItemsBySetMenuMask(set_menu_mask);

        return set_menu_items;
}

int TListMenuContainer::MenuAdd(TListMenu *in)
{
   in->Owner = this;
   return Add(in);
}

void TListMenuContainer::MenuDelete(TListMenu *in)
{
   if (in != NULL)
   {
	  if (IndexOf(in) != -1)
	  {
		 if (in->Owner == this)
		 {
			Remove(in);
			delete(TListMenu*)in;
			in = NULL;
		 }
		 else
		 {
			Remove(in);
		 }
	  }
   }
}

void TListMenuContainer::MenuSort()
{

}

void TListMenuContainer::MenuCopyList(TListMenu *inMenuCopyList)
{

}

void TListMenuContainer::MenuClear()
{
   while (Count > 0)
   {
	  MenuDelete(MenuGet(0));
   }
}

TListMenu *TListMenuContainer::MenuGet(int Index)
{
   if (Count > Index)
   {
	  return(TListMenu*)Items[Index];
   }
   return NULL;
}

TListMenu *TListMenuContainer::MenuGetNext(TListMenu * Menu)
{
   int CurrentIndex = IndexOf(Menu);
   if (CurrentIndex != -1)
   {
	  if (Count > (CurrentIndex + 1))
	  {
		 return(TListMenu*)Items[CurrentIndex + 1];
	  }
   }
   return NULL;
}

TListMenu *TListMenuContainer::MenuGet(UnicodeString MenuName)
{
   for (int i = 0; i < Count; i++)
   {
	  TListMenu *Menu = MenuGet(i);
	  if (Menu->MenuName == MenuName)
	  {
		 return Menu;
	  }
   }
   return NULL;
}

TItemSize *
TListMenuContainer::FetchItemSizeByBarcode(
    const UnicodeString barcode)
{
        TItemSize *size = NULL;

        for (int i = 0; size == NULL && i < Count; ++i)
                size = MenuGet(i)->FetchItemSizeByBarcode(barcode);

        return size;
}

// ------------------------------------------------------------------------------
