//---------------------------------------------------------------------------

#ifndef MembershipExternalGUIH
#define MembershipExternalGUIH

#include "MembershipExternal.h"
#include "Modules.h"
//---------------------------------------------------------------------------
class TMembershipExternalGUI : public TMembershipExternal
{
	public :
		TMembershipExternalGUI(TModules &Modules,AnsiString inIP, int inPort, int inTillID);
		TLoginSuccess virtual Login(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo);
		TLoginSuccess virtual Authorise(TMMContactInfo &UserInfo,Currency wrkPayAmount);
        bool AuthPIN(TMMContactInfo &UserInfo,AnsiString PIN,AnsiString &Message);
};

#endif
