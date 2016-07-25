//---------------------------------------------------------------------------


#pragma hdrstop

#include "ItemSizeCategory.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TItemSizeCategory::TItemSizeCategory()
{
   CategoryKey = 0;
   Category = "";
   GLCode = "";
}

void TItemSizeCategory::CategoryCopy(TItemSizeCategory * Copy)
{
   CategoryKey = Copy->CategoryKey;
   Category = Copy->Category;
   GLCode = Copy->GLCode;
}
