//---------------------------------------------------------------------------

#ifndef LoyaltyMateUtilitiesH
#define LoyaltyMateUtilitiesH

#include "LogsDB.h"
#include "MM_DBCore.h"
#include "DeviceRealTerminal.h"
#include "ContactPoints.h"
#include "DBTierLevel.h"
#include "ManagerSyndCode.h"
#include "MMContactInfo.h"
//---------------------------------------------------------------------------

struct TLoyaltyMateTransaction
{
    public:
        int TransactionNumber;
        int ContactKey;
        AnsiString CloudUUID;
        TSyndCode SyndicateCode;
        Currency PointsBalance;
        Currency PointsDelta;
        TDateTime OccurredDate;
        int PointsType;
        AnsiString InvoiceNumber;
};

//---------------------------------------------------------------------------
class TLoyaltyMateMemberOperation
{
    public:
        TSyndCode SyndicateCode;
        TMMContactInfo MemberInfo;

        TLoyaltyMateMemberOperation(TSyndCode syndicateCode,TMMContactInfo memberInfo):
                                    SyndicateCode(syndicateCode),MemberInfo(memberInfo){}
};
//---------------------------------------------------------------------------
class TLoyaltyMateUtilities
{
    public:
        static int DeleteTransaction(int contactKey,int transactionNumber);
        static void SetTransaction(Database::TDBTransaction &DBTransaction,TLoyaltyMateTransaction transaction);
        static std::vector<int> GetAllContactsWithPendingTransactions();
        static std::vector<TLoyaltyMateTransaction> GetPendingTransactionForContact(int contactKey);
        static bool GetContactByCloudUUID(Database::TDBTransaction &DBTransaction,UnicodeString uuid,int &contactKey,int &siteId);
        static UnicodeString GetMemberCloudIdIfRegistered(Database::TDBTransaction &DBTransaction,int contactKey,int siteId);
        static bool SetMemberCloudId(Database::TDBTransaction &DBTransaction,int contactKey,int siteId,UnicodeString uuid,UnicodeString activationToken="");
        static bool IsLoyaltyMateEnabledGUID(UnicodeString uuid);
        static UnicodeString GetLoyaltyMateDisabledCloudUUID();
       static int GetPointsType(TPointsTransactionType pointType);
       static void SetLoyaltymateTransactions(Database::TDBTransaction &DBTransaction,int contactKey,TSyndCode syndicateCode,
                                              TContactPoints PointsToLoad,AnsiString inInvoiceNumber);
       static UnicodeString GetMemberCloudId(Database::TDBTransaction &DBTransaction,int contactKey);
};
//---------------------------------------------------------------------------

#endif
