//---------------------------------------------------------------------------

#ifndef WebOrderH
#define WebOrderH
//---------------------------------------------------------------------------
#include <Classes.hpp>

class TWebOrder
{
	public:
   TWebOrder();
	Clear();
	bool Valid();
   UnicodeString GUID;
   UnicodeString StoreName;
   TDateTime DateOrdered;
   TDateTime DateExpected;
   Currency PrePaid;
   int WebKey;
   int TabKey;
   int TimeKey;
   int ContactKey;
};
#endif
