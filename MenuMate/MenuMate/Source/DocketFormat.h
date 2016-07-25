//---------------------------------------------------------------------------

#ifndef DocketFormatH
#define DocketFormatH

#include "MM_DBCore.h"
#include <map>

//---------------------------------------------------------------------------
class TDocketFormat
{
    public :
    TDocketFormat();
    void Load(Database::TDBTransaction &DBTransaction);

  	std::map<long,UnicodeString> KeyItemHeader;
	WideString BulletSide; 	// " > "
	WideString BulletOpt; 	// "    "
	WideString BulletSetMenuItem; // "    "
	WideString NoteHeader; 	// "Note: "
	WideString MixHeader; 	// "  Mix: "
	WideString SideHeader; 	// "  Sides: "
	WideString DishesPerSeatMultiplerString;
	bool UnRavelKitchenPrinting;
};

#endif
