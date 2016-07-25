//---------------------------------------------------------------------------

#ifndef MembershipThorlinkH
#define MembershipThorlinkH

#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <IBTable.hpp>
#include "Membership.h"
#include "Enum.h"
#include "ThorlinkClient.h"

//---------------------------------------------------------------------------


class TMembershipThorlink : public TMembership
{
    private:
	int BtnSelection;

   	protected :

        AnsiString AppKey;
        AnsiString SiteNo;
        AnsiString MerchantCode;
        AnsiString DeviceCode;
		std::auto_ptr<TThorlinkClient> ThorClient;
		void Initialise(Database::TDBTransaction &DBTransaction);

		// External Interface workhorse functions go here.
		void GetExternalContactDetails(Database::TDBTransaction &DBTransaction,TMMContactInfo &Info);
		void SetExternalContactDetails(Database::TDBTransaction &DBTransaction,int ExternalKey,TMMContactInfo &Info);

	public:
        std::set<int> ThorVoucherKeys;
		TMembershipThorlink(TModules &Modules,AnsiString AppKey, AnsiString SiteNo, AnsiString MerchantCode,AnsiString DeviceCode);
		~TMembershipThorlink();

		void SetContactDetails(Database::TDBTransaction &DBTransaction,int InternalKey,TMMContactInfo &Info);
        void GetContactDetails(Database::TDBTransaction &DBTransaction,int InternalKey,TMMContactInfo &Info);

        void UpdateThorlinkVouchers(Database::TDBTransaction &DBTransaction);
		TContactType GetContactType(int ContactKey);

		void GetContactListNumeric(Database::TDBTransaction &DBTransaction,TStringList *List);
		bool DeleteContact(int inContactKey);
		int GetContactByCode(int inContactCode);
		void SetContactCode(int inContactKey,int inContactID);
		void GetContactListPhone(Database::TDBTransaction &DBTransaction,TStringList *List);
        void GetKioskDisplayMsg(Database::TDBTransaction &DBTransaction,TMMContactInfo &Info,std::map<UnicodeString,UnicodeString> &Data);
        int GetContactByCard(Database::TDBTransaction &DBTransaction,TMMContactInfo &Info);
		TLoginSuccess TestCardLogin(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo);
		TLoginSuccess TestKeyLogin(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo);
		TLoginSuccess GetSubsPaid(Database::TDBTransaction &DBTransaction,int InternalKey);
        TLoginSuccess FindMember(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo);
        virtual TLoginSuccess IsCard(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo);
        bool GetExternalContactDetialsFromKey(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info);
        AnsiString SendRequestForPay(std::vector<TTenderDetails> tenderDetailsList,std::vector<TItemDetailsThor> itemsList);
        AnsiString SendRequestForRefund(TRefundTransaction refundTransaction);
        int GenerateThorVoucherKey(Database::TDBTransaction &DBTransaction);
        bool CallInitMethod();
};
#endif

