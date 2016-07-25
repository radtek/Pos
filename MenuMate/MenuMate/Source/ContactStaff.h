//---------------------------------------------------------------------------

#ifndef ContactStaffH
#define ContactStaffH

#include <forms.hpp>
#include <DB.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include "Enum.h"
#include "Contact.h"
#include "Access.h"

#include "POS_XMLBase.h"

class TContactStaff : public TContact
{
   private :
		bool TestAccessLevel(TAccess UserAccess, SecurityCheck RequiredAccess, UnicodeString &Description);
      TLoginSuccess TestIDLogin(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo, SecurityCheck Destination, UnicodeString &Description);
		TLoginSuccess TestKeyLogin(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo, SecurityCheck Destination, UnicodeString &Description);
		TLoginSuccess TestCardLogin(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo, SecurityCheck Destination, UnicodeString &Description);
		TLoginSuccess TestProxLogin(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo, SecurityCheck Destination, UnicodeString &Description);
      TLoginSuccess AdminAccountToGodMode(Database::TDBTransaction &DBTransaction, UnicodeString PIN, int AccountKey);
     
	public:
		TContactStaff(Database::TDBTransaction &DBTransaction);
		bool DisableManualLogin;
				
		TDateTime GetLastUserReportTime(Database::TDBTransaction &DBTransaction,int inContactKey);
		void SetLastUserReportTime(Database::TDBTransaction &DBTransaction,int inContactKey,TDateTime DateTime);


		TLoginSuccess Login(TForm* LoginFormOwner,Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo, SecurityCheck RequiredAccess);
		bool TestAccessLevel(TMMContactInfo &UserInfo, SecurityCheck RequiredAccess);

      void AdminCreateAccount(Database::TDBTransaction &DBTransaction, UnicodeString PIN);
      void BuildXMLListStaff(Database::TDBTransaction &DBTransaction,TPOS_XMLBase &Data);

      bool CheckUserAccessLogin(Database::TDBTransaction &DBTransaction, int contact_key, SecurityCheck Destination,
      UnicodeString &Description, TMMContactInfo &UserInfo);

};

#endif

