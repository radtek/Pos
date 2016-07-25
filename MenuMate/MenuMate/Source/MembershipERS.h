// ---------------------------------------------------------------------------

#ifndef MembershipERSH
#define MembershipERSH

#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <IBTable.hpp>
#include "Membership.h"
#include "Enum.h"

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
class TMembershipERS : public TMembership
{
private:
   static bool RunOnce;

   Database::TDBControl &IBDatabase;

   int DBIndex;

   void Get3rdPartyContactDetails(Database::TDBTransaction &DBTransaction, int ExternalKey, TMMContactInfo &Info);
   void Set3rdPartyContactDetails(Database::TDBTransaction &DBTransaction, int ExternalKey, TMMContactInfo &Info);
   void Synchronize3rdPartyDetails(Database::TDBTransaction &DBTransaction, int InternalKey, TMMContactInfo &Info);
   void SyncAndFix3rdPartyTabs(Database::TDBTransaction &DBTransaction);
   UnicodeString EnsureASCII(UnicodeString Data);
   bool ProcessBirthday(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info);
   int GetExternalKey(Database::TDBTransaction &DBTransaction, int InternalKey);
public:
   void Initialise(Database::TDBTransaction &DBTransaction);
   void InitTransaction(Database::TDBTransaction &DBTransaction);
   TMembershipERS(TModules &Modules,Database::TDBControl &inIBDatabase);
   ~TMembershipERS();

   void LoyaltyAddValue(Database::TDBTransaction &DBTransaction, TPointsTransaction &PointsTransaction);
   Currency LoyaltyGetValue(Database::TDBTransaction &DBTransaction, int inContactKey);

   int RecordCount(Database::TDBTransaction &DBTransaction);

   TContactType GetContactType(int ContactKey);
   TLoginSuccess GetSubsPaid(Database::TDBTransaction &DBTransaction, int ExternalKey);
   void GetContactDetails(Database::TDBTransaction &DBTransaction, int InternalKey, TMMContactInfo &Info);
   void SetContactDetails(Database::TDBTransaction &DBTransaction, int InternalKey, TMMContactInfo &Info);
   void GetContactList(Database::TDBTransaction &DBTransaction, TStringList *List, UnicodeString Format, UnicodeString OrderBy);
   void GetContactListNumeric(Database::TDBTransaction &DBTransaction, TStringList *List);
   bool DeleteContact(int InternalKey);
   int GetContactByCode(int inContactCode);
   void SetContactCode(int InternalKey, int inContactID);
   void GetContactListPhone(Database::TDBTransaction &DBTransaction, TStringList *List);
   int GetContactKey(Database::TDBTransaction &DBTransaction, int ExternalKey);
   TLoginSuccess TestCardLogin(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo);
   TLoginSuccess TestKeyLogin(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo);
   int GetContactByCard(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo);

   bool CheckVersionNumber(Database::TDBTransaction &DBTransaction);
   UnicodeString GetContactNumber(Database::TDBTransaction &DBTransaction, int InternalKey);
   Currency LoyaltyGetValueFromMemberKey(Database::TDBTransaction &DBTransaction, int ExternalKey);
   bool GetContactNameUnique(Database::TDBTransaction &DBTransaction, UnicodeString ContactName);
   void GetDiscountDetails(Database::TDBTransaction &DBTransaction, int ExternalKey, TMMContactInfo &Info);
   void SyncMembers(Database::TDBTransaction &DBTransaction, TfrmProcessing *Progress);

   // Kiosk
   void GetKioskDisplayMsg(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info, std::map <UnicodeString, UnicodeString> &Data);
   void SetVisitCount(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo);
   void GetReportMemberOnPremisesInfo(Database::TDBTransaction &DBTransaction, TStringList *Report);

	virtual void FastContactListing(TIBSQL **, Database::TDBTransaction &,
	                                int, int, bool);
};

bool TMembershipERS::RunOnce;

#endif
