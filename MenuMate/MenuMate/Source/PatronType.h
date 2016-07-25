//---------------------------------------------------------------------------

#ifndef PatronTypeH
#define PatronTypeH
//---------------------------------------------------------------------------
#include <Classes.hpp>

class TPatronType
{
	public :
	UnicodeString Name;
	bool Default;
	int Count;
	TPatronType();
};
#endif
