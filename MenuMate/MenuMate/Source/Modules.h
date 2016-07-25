//---------------------------------------------------------------------------

#ifndef ModulesH
#define ModulesH
//---------------------------------------------------------------------------
#include "Registration.h"
//---------------------------------------------------------------------------

typedef std::map<eRegisteredModules,std::map<UnicodeString,Variant> > TModType;

class TModules
{
	public:
	TModType Status;
};

#endif
