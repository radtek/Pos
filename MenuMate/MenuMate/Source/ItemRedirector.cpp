//---------------------------------------------------------------------------


#pragma hdrstop

#include "ItemRedirector.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
TItemRedirector::TItemRedirector(TItemsCompleteCompressed *inCompressed)
{
	Selected = false;
	ItemObject = NULL;
	ParentRedirector = NULL;
	CompressedContainer = inCompressed;
}
//---------------------------------------------------------------------------
