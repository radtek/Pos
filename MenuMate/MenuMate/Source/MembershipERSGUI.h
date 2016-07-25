//---------------------------------------------------------------------------

#ifndef MembershipERSGUIH
#define MembershipERSGUIH
//---------------------------------------------------------------------------

#include "MembershipERS.h"

#include "Modules.h"

class TContactMemberERSGUI : public TMembershipERS
{
	public :
		TLoginSuccess Login(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo);
		TContactMemberERSGUI(TModules &Modules,Database::TDBControl &inIBDatabase);
		void Initialise(Database::TDBTransaction &DBTransaction);
};

#endif
