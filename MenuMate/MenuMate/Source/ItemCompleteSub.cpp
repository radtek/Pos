//---------------------------------------------------------------------------


#pragma hdrstop

#include "ItemCompleteSub.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TItemCompleteSub::TItemCompleteSub()
{
}

__fastcall TItemCompleteSub::~TItemCompleteSub()
{
}

void TItemCompleteSub::Assign(TItemCompleteSub *RetItem)
{
   RetItem->Owner = Owner;
   TItemMinorComplete::Assign(RetItem);
}

int
TItemCompleteSub::DefaultPatronCount()
const
{
        return 0;
}

int
TItemCompleteSub::PatronCount(
  const int,
  const bool)
{
        return 0;
}

int
TItemCompleteSub::PatronCount()
const
{
        return 0;
}

