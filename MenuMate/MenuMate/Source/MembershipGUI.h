//---------------------------------------------------------------------------

#ifndef MembershipGUIH
#define MembershipGUIH

#include "Membership.h"
#include "Modules.h"
//---------------------------------------------------------------------------
class TMembershipGUI : public TMembership
{
	public :
	   TMembershipGUI(TModules &Modules);
	   TLoginSuccess virtual Login(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo);
	   void Initialise(Database::TDBTransaction &DBTransaction);
};

#endif
