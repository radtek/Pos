//---------------------------------------------------------------------------

#ifndef MembershipExternalH
#define MembershipExternalH

#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <IBTable.hpp>
#include "Membership.h"
#include "Enum.h"
#include "MembershipGeneralLedgerTCP.h"

//---------------------------------------------------------------------------
class TMembershipExternal : public TMembership
{
	protected :
		AnsiString IP;
		int Port;
		int TillID;
		std::auto_ptr<TMembershipGeneralLedgerTCP> MembershipGeneralLedgerTCP;
		void Initialise(Database::TDBTransaction &DBTransaction);

		// External Interface workhorse functions go here.
		void GetExternalContactDetails(Database::TDBTransaction &DBTransaction,TMMContactInfo &Info);
		void SetExternalContactDetails(Database::TDBTransaction &DBTransaction,int ExternalKey,TMMContactInfo &Info);
		void SyncPointsDetials(Database::TDBTransaction &DBTransaction,TMMContactInfo &Info);
		Currency LoyaltyGetExternalValue(Database::TDBTransaction &DBTransaction,int ExternalKey);
	public:
		TMembershipExternal(TModules &Modules,AnsiString IP, int Port,int inTillID);
		~TMembershipExternal();

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
		void GetDiscountDetails(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info);
		// Kiosk
		void GetKioskDisplayMsg(Database::TDBTransaction &DBTransaction,TMMContactInfo &Info,std::map<UnicodeString,UnicodeString> &Data);
		TLoginSuccess GetSubsPaid(Database::TDBTransaction &DBTransaction,int InternalKey);
		void SyncMembers(Database::TDBTransaction &DBTransaction,TfrmProcessing *Progress);
		TLoginSuccess FindMember(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo);
		int GetSiteID(AnsiString Card);
        virtual TLoginSuccess IsCard(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo);
    	void virtual GetReportMemberInfo(Database::TDBTransaction &DBTransaction, TMMContactInfo &Member, TStringList *Report);
        bool GetExternalContactDetialsFromKey(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info);
        void TransactionClosed(Database::TDBTransaction &DBTransaction,TMMContactInfo &Member);

};
#endif
