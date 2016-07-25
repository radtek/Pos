//---------------------------------------------------------------------------
#pragma hdrstop

#include "ManagerThirdParty.h"

#ifdef MENUMATE
#include "MMLogging.h"
#endif
//---------------------------------------------------------------------------

#pragma package(smart_init)

TManagerThirdParty::TManagerThirdParty()
{
	Version = THRID_PARTY_VERSION;
	Product = "";
}

//---------------------------------------------------------------------------
void TManagerThirdParty::Initialise(UnicodeString inProduct)
{
	Product = inProduct;
}


UnicodeString TManagerThirdParty::GetCSVHeader()
{
	return "$Control," + Version + "," + Product;
}


