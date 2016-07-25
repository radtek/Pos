#ifndef MembershipThorlinkGUIH
#define MembershipThorlinkGUIH

#include "MembershipThorlink.h"
#include "Modules.h"
//---------------------------------------------------------------------------
class TMembershipThorlinkGUI : public TMembershipThorlink
{
		public :
		TMembershipThorlinkGUI(TModules &Modules,AnsiString AppKey, AnsiString SiteNo, AnsiString MerchantCode,AnsiString DeviceCode);
		TLoginSuccess virtual Login(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo);
};

#endif
