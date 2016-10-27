//---------------------------------------------------------------------------

#ifndef DBContactsH
#define DBContactsH
#include "MM_DBCore.h"
#include "MMContactInfo.h"
#include "enumContacts.h"
#include "Commission.h"
//---------------------------------------------------------------------------

enum MemberMode { eInvalidMode, eSmartCardMode, eProxCardMode, eSwipeCardMode, eMemberCodeMode };

class TDBContacts
{

    private:
        static int getNextPointTransactionID(Database::TDBTransaction &DBTransaction);
        static void setPointsTransactionEntry(
                                        Database::TDBTransaction &DBTransaction,
                                        int contactKey,
                                        TDateTime timestamp,
                                        TPointsTransactionType adjustmentType,
                                        TPointsTransactionAccountType adjustmentSubType,
                                        Currency adjustment,
                                        TPointsExportStatus exportStatus,
                                        TDateTime exportTimeStamp);
        static UnicodeString getPointsTransactionInsertQuery();


	public:
        static int GetContactByMemberNumberSiteID(Database::TDBTransaction &DBTransaction,UnicodeString MemberNumber, int SiteID);
        static bool GetContactDetailsByCode(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info, AnsiString cardCode, MemberMode &inMemberMode);
        static bool GetContactDetailsByMemberCode(Database::TDBTransaction &DBTransaction,AnsiString cardCode,TMMContactInfo &Info);
        static bool GetContactDetailsBySwipeCard(Database::TDBTransaction &DBTransaction,AnsiString cardCode,TMMContactInfo &Info);
        static bool GetContactDetailsByProxCard(Database::TDBTransaction &DBTransaction,AnsiString cardCode,TMMContactInfo &Info);
        static int GetContactByEmail(Database::TDBTransaction &DBTransaction,AnsiString Email);
        static int GetContactByEmail(Database::TDBTransaction &DBTransaction,AnsiString Email,int contactKey);
        static int GetContactByNamePhone(Database::TDBTransaction &DBTransaction,UnicodeString Name, UnicodeString Phone);
        static void GetContactDetails(Database::TDBTransaction &DBTransaction, int inContactKey, TMMContactInfo &Info);
        static void GetDiscountDetails(Database::TDBTransaction &DBTransaction, int inContactKey, TMMContactInfo &Info);
        static bool GetContactExists(Database::TDBTransaction &DBTransaction, int inContactKey);
        static int GetOrCreateContact(Database::TDBTransaction &DBTransaction, int inContactKey, TContactType inContactType, TMMContactInfo &Info);
        static void SetContactDetails(Database::TDBTransaction &DBTransaction, int inContactKey,TContactType inContactType, TMMContactInfo &Info);
        static void SetContactCard(Database::TDBTransaction &DBTransaction, int inContactKey, UnicodeString Card);
        static void DeleteContactCard(Database::TDBTransaction &DBTransaction, UnicodeString Card, int inContactKey = 0);
        static void DeleteContactCards(Database::TDBTransaction &DBTransaction, int inContactKey);
        static void DeleteContactDiscounts(Database::TDBTransaction &DBTransaction, int inContactKey);
        static void SetContactDiscounts(Database::TDBTransaction &DBTransaction, int inContactKey, int DiscountKey);
        void GetContactNameList(Database::TDBTransaction &DBTransaction, std::vector<ContactGroup> &vectorMembers, bool inProfile);
        static int HasAccountProfile(Database::TDBTransaction &DBTransaction, int inContactKey);
        static int GetCurrentGroups(Database::TDBTransaction &DBTransaction, int inContactKey, TMMContactInfo &Info);
        static int GetAvailableGroups(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info);
        static int GetSummaGroup(Database::TDBTransaction &DBTransaction,int inContactKey, TMMContactInfo &Info);
        static void SetCurrentGroups(Database::TDBTransaction &DBTransaction, int inContactKey, ContactGroup Info);
        static void RemoveCurrentGroups(Database::TDBTransaction &DBTransaction, int inContactKey);
        static void SetSummaGroup(Database::TDBTransaction &DBTransaction, int inContactKey, TMMContactInfo &Info);
        static void GetAllTypes(Database::TDBTransaction &DBTransaction, std::vector<int> &inGroupKeys);
        static void RemoveDefaultGroup(Database::TDBTransaction &DBTransaction, int inContactKey);

        static void GetStaff(Database::TDBTransaction &DBTransaction, std::vector<TCommission> &inStaff);
        static void GetStaff(Database::TDBTransaction &DBTransaction, std::vector<TCommission> &inStaff, int ZedKey);
        static AnsiString GetContactName(Database::TDBTransaction &DBTransaction, int ContactKey);
        static void GetPointsBalances(Database::TDBTransaction &DBTransaction,int ContactKey, TContactPoints & Points);
        static bool GetContactCards(Database::TDBTransaction &DBTransaction,int inContactsKey, std::set <UnicodeString> &Cards);
        static void ClearPIN(Database::TDBTransaction &DBTransaction, int inContactKey, TContactType Type);
        static AnsiString GetPIN(Database::TDBTransaction &DBTransaction, int inContactKey);
        static void SaveCustomerAndNumber( Database::TDBTransaction &DBTransaction, TCustomer Customer );
        static TCustomer GetCustomerAndRoomNumber( Database::TDBTransaction &DBTransaction, const int InvoiceKey );
        //duplicated method from loyalty mate utilities
        static UnicodeString GetMemberCloudIdIfRegistered(Database::TDBTransaction &DBTransaction,int contactKey,int siteId);
        static TDateTime GetCardCreationDate( Database::TDBTransaction &DBTransaction, int inContactKey);
        static void SetCardCreationDate( Database::TDBTransaction &DBTransaction, int inContactKey, TDateTime inCreationDate);
        static void GetTierLevelAndMemberType(Database::TDBTransaction &DBTransaction,TMMContactInfo &Member);
        static int GetLoyaltyKey(Database::TDBTransaction &dBTransaction, int tabKey );
        static int GenerateLoyaltymateKey(Database::TDBTransaction &DBTransaction);
        static void GetEarnedAndLoadedPointsBalances(Database::TDBTransaction &DBTransaction,int ContactKey, TContactPoints & Points);
        static bool IsFirstVisitRewarded(Database::TDBTransaction &DBTransaction,int ContactKey);
        static void SetFirstVisitRewardStatus(Database::TDBTransaction &DBTransaction,int ContactKey);
        static AnsiString GetContactProxCard(Database::TDBTransaction &DBTransaction,int ContactKey);
};

#endif
