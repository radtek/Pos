#ifndef MembershipEBetGUIH
#define MembershipEBetGUIH

#include "MembershipEBet.h"
#include "Modules.h"
//---------------------------------------------------------------------------
class TMembershipEBetGUI : public TMembershipEBet
{
	public :
		TMembershipEBetGUI(TModules &Modules,AnsiString inIP, int inPort, int inTillID);
		TLoginSuccess virtual Login(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo);
};

#endif