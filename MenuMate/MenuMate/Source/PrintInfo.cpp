//---------------------------------------------------------------------------


#pragma hdrstop

#include "PrintInfo.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TPrintInfo::TPrintInfo()
{
	Underlined 			= false;
	Bold 					= false;
	Colour 				= fcBlack;
	Font 					= ftFontA;
	Width 				= fsNormalSize;
	Height				= fsNormalSize;
}

void TPrintInfo::Reset()
{
	Underlined 			= false;
	Bold 					= false;
	Colour 				= fcBlack;
	Font 					= ftFontA;
	Width 				= fsNormalSize;
	Height				= fsNormalSize;
}
