//---------------------------------------------------------------------------

#ifndef ForcedSideH
#define ForcedSideH

#include <Classes.hpp>
//---------------------------------------------------------------------------
class TForcedSide
{
	public:
   int MasterItemKey;
	int IOO;
	int GroupNo;
	int Max_Select;
   bool AllowSkip;
   AnsiString MenuLookup;
   AnsiString CourseLookup;
	AnsiString ItemLookup;
};
#endif
