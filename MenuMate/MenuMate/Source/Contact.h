// ---------------------------------------------------------------------------

#ifndef ContactH
#define ContactH

#include <DB.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include "Enum.h"
#include "ManagerVariable.h"
#include <set>
#include <vector>
#include "MM_DBCore.h"
#include "ContactPoints.h"
#include "EventsContact.h"
#include "enumTab.h"
#include "enumContacts.h"
// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------



class TContact
{
private:
protected:
   bool fEnabled;

   void __fastcall SetEnabled(bool value);
   bool __fastcall GetEnabled();
   bool virtual CardBanned(Database::TDBTransaction &DBTransaction, int ContactKey, UnicodeString Atr);
   bool virtual CardReplaced(Database::TDBTransaction &DBTransaction, int ContactKey, UnicodeString Atr);

public:
   bool TabsEnabled;

   TContact();
   virtual ~TContact();

   TContactEvents OnSaveContact;

   TContactType ContactType;

   TMMTabType ContactTabType;

   TContactType virtual GetContactType(Database::TDBTransaction &DBTransaction, int ContactKey);
   void virtual Initialise(Database::TDBTransaction &DBTransaction);
   int virtual GetNextContactID(Database::TDBTransaction &DBTransaction);
   int virtual RecordCount(Database::TDBTransaction &DBTransaction);
   int virtual GetOrCreateContact(Database::TDBTransaction &DBTransaction, int inContactKey, TMMContactInfo &Info);
   int virtual GetContact(Database::TDBTransaction &DBTransaction, int inContactKey);
   int virtual GetContactByCard(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info);
   void virtual SetContactCard(Database::TDBTransaction &DBTransaction, int inContactKey, UnicodeString Card);
   void virtual SetContactDiscounts(Database::TDBTransaction &DBTransaction, int inContactKey, int DiscountKey);
   void virtual DeleteContactCard(Database::TDBTransaction &DBTransaction, UnicodeString Card, int inContactKey = 0);
   void virtual DeleteContactCards(Database::TDBTransaction &DBTransaction, int inContactKey);
   void virtual DeleteContactDiscounts(Database::TDBTransaction &DBTransaction, int inContactKey);

   int virtual GetContactIDByKey(Database::TDBTransaction &DBTransaction, int ContactKey);
   int virtual GetContactByProx(Database::TDBTransaction &DBTransaction, UnicodeString Prox);
   int virtual GetContactByName(Database::TDBTransaction &DBTransaction, UnicodeString Name);

   UnicodeString virtual GetContactName(Database::TDBTransaction &DBTransaction, int ContactKey);
   void virtual GetContactNameList(Database::TDBTransaction &DBTransaction, std::vector<UnicodeString> &vectorMembers, int inContactType);

   UnicodeString virtual GetContactNumber(Database::TDBTransaction &DBTransaction, int ContactKey);
   UnicodeString virtual GetContactInitials(Database::TDBTransaction &DBTransaction, int ContactKey);
   UnicodeString virtual GetContactNameByCard(Database::TDBTransaction &DBTransaction,TMMContactInfo &Info);

   void virtual GetContactDetails(Database::TDBTransaction &DBTransaction, int inMemberKey, TMMContactInfo &Info);
   void virtual SetContactDetails(Database::TDBTransaction &DBTransaction, int inContactKey, TMMContactInfo &Info);
   void virtual GetContactList(Database::TDBTransaction &DBTransaction, TStringList *List, UnicodeString Format = "%Name",
	  UnicodeString OrderBy = "NAME");
   int virtual GetContactKeyByName(Database::TDBTransaction &DBTransaction, UnicodeString Name);
   bool virtual DeleteContact(Database::TDBTransaction &DBTransaction, int inContactKey);
   bool virtual UndeleteContact(Database::TDBTransaction &DBTransaction, int inContactKey);
   bool virtual GetContactBlocked(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info);

   int virtual GetContactKey(Database::TDBTransaction &DBTransaction, int in3rdPartyKey);

   void GetTabs(Database::TDBTransaction &DBTransaction, TStringList *List);
   bool virtual GetContactExists(Database::TDBTransaction &DBTransaction, int inContactKey);
   __property bool Enabled =
   {
	  read = GetEnabled, write = SetEnabled
   };
   bool virtual UpdateContactKey(Database::TDBTransaction &DBTransaction, int OldContactKey, int NewContactKey);
   bool virtual SetContactType(Database::TDBTransaction &DBTransaction, int inContactKey, TContactType Type);
   int GetInternalKey(Database::TDBTransaction &DBTransaction, int inExternalKey);
   int virtual GetExternalKey(Database::TDBTransaction &DBTransaction,int InternalKey);
};

#endif
