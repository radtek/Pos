//---------------------------------------------------------------------------


#pragma hdrstop

#include "ItemSide.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TItemSide::TItemSide(TListSideContainer *inOwner)
{
   MasterItem = NULL;
   MasterItemKey = 0;
   ItemKey = 0;
   Owner = inOwner;
   Item = NULL;
   GroupNo = 0;
   IOO = 0;
   AllowSkip = true;
};
