//---------------------------------------------------------------------------

#ifndef MembershipEBetH
#define MembershipEBetH

#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <IBTable.hpp>
#include "Membership.h"
#include "MembershipEbetTCP.h"
#include "Enum.h"

//---------------------------------------------------------------------------
class TMembershipEBet : public TMembership
{
	private :
		AnsiString IP;
		int Port;
		int TillID;
		std::auto_ptr<TMembershipEBetTCP> MembershipEBetTCP;
		void Initialise(Database::TDBTransaction &DBTransaction);

		// External Interface workhorse functions go here.
		void GetExternalContactDetails(Database::TDBTransaction &DBTransaction,TMMContactInfo &Info);
		void SetExternalContactDetails(Database::TDBTransaction &DBTransaction,int ExternalKey,TMMContactInfo &Info);
		void SyncPointsDetials(Database::TDBTransaction &DBTransaction,TMMContactInfo &Info);
		Currency LoyaltyGetExternalValue(Database::TDBTransaction &DBTransaction,int ExternalKey);
	public:
		TMembershipEBet(TModules &Modules,AnsiString IP, int Port,int inTillID);
		~TMembershipEBet();

		//Internal POS API Funtion overrides.

		void LoyaltyAddValue(Database::TDBTransaction &DBTransaction,TPointsTransaction &PointsTransaction);
		Currency LoyaltyGetValue(Database::TDBTransaction &DBTransaction,int InternalKey);

		void SetContactDetails(Database::TDBTransaction &DBTransaction,int InternalKey,TMMContactInfo &Info);
		void GetContactDetails(Database::TDBTransaction &DBTransaction,int InternalKey,TMMContactInfo &Info);

		TContactType GetContactType(int ContactKey);

		void GetContactListNumeric(Database::TDBTransaction &DBTransaction,TStringList *List);
		bool DeleteContact(int inContactKey);
		int GetContactByCode(int inContactCode);
		void SetContactCode(int inContactKey,int inContactID);
		void GetContactListPhone(Database::TDBTransaction &DBTransaction,TStringList *List);
		TLoginSuccess TestCardLogin(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo);
		TLoginSuccess TestKeyLogin(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo);
		int GetContactByCard(Database::TDBTransaction &DBTransaction,TMMContactInfo &Info);
		void GetDiscountDetails(Database::TDBTransaction &DBTransaction,int inContactKey,TMMContactInfo &Info);
		// Kiosk
		void GetKioskDisplayMsg(Database::TDBTransaction &DBTransaction,TMMContactInfo &Info,std::map<UnicodeString,UnicodeString> &Data);
		TLoginSuccess GetSubsPaid(Database::TDBTransaction &DBTransaction,int InternalKey);
		void SyncMembers(Database::TDBTransaction &DBTransaction,TfrmProcessing *Progress);
		UnicodeString ErrorMessage();
        TLoginSuccess FindMember(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo);

};

#endif
