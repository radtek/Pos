//---------------------------------------------------------------------------


#pragma hdrstop

#include "ItemOption.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

bool operator < (const TItemOption & x, const TItemOption & y)
{
	return x.Name < y.Name;
};

bool operator == (const TItemOption & x, const TItemOption & y)
{
	if ((x.Palm_ID == y.Palm_ID) && (x.OptionID == y.OptionID) && (x.Name == y.Name) && (x.Enabled == y.Enabled))
	{
		return true;
	}
	return false;
};

TItemOption::TItemOption()
{
   OptionKey = 0;
   Palm_ID = 0;
   OptionID = 0;
   ForcedMask = 0;
   Flags = 0;
   IsPlus = true;
   Name = "";
   KitchenName = "";
   Enabled = false;
   AllowSkip = true;
   DisallowMuliSelect = false;
 //  Quantity = 0;

   FontInfo.Reset();
}