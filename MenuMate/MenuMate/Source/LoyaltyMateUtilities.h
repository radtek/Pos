//---------------------------------------------------------------------------

#ifndef LoyaltyMateUtilitiesH
#define LoyaltyMateUtilitiesH

#include "LogsDB.h"
#include "MM_DBCore.h"
#include "DeviceRealTerminal.h"
#include "ContactPoints.h"
#include "DBTierLevel.h"
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

};
//---------------------------------------------------------------------------
class TLoyaltyMateMemberOperation
{
    public:
        TSyndCode SyndicateCode;
        TMMContactInfo MemberInfo;

        TLoyaltyMateMemberOperation(
                        TSyndCode syndicateCode,
                        TMMContactInfo memberInfo):
                                    SyndicateCode(syndicateCode),
                                    MemberInfo(memberInfo)
        {
        }
};

class TLoyaltyMateTierOperation
{
    public:
        TSyndCode SyndicateCode;
        TTierLevel* TierLevel;

        TLoyaltyMateTierOperation(
                        TSyndCode syndicateCode,
                        TTierLevel* tierLevel):
                                    SyndicateCode(syndicateCode),
                                    TierLevel(tierLevel)
        {
        }
};


//---------------------------------------------------------------------------
class TLoyaltyMateUtilities
{
    public:
        static int DeleteTransaction(
                            int contactKey,
                            int transactionNumber);

        static void SetTransaction(
                        Database::TDBTransaction &DBTransaction,
                        TLoyaltyMateTransaction transaction);

        static std::vector<int> GetAllContactsWithPendingTransactions();

        static std::vector<TLoyaltyMateTransaction> GetPendingTransactionForContact(int contactKey);

        static bool GetContactByCloudUUID(
                            Database::TDBTransaction &DBTransaction,
                            UnicodeString uuid,
                            int &contactKey,
                            int &siteId);

        static UnicodeString GetMemberCloudIdIfRegistered(
                            Database::TDBTransaction &DBTransaction,
                            int contactKey,
                            int siteId);

        static bool SetMemberCloudId(
                            Database::TDBTransaction &DBTransaction,
                            int contactKey,
                            int siteId,
                            UnicodeString uuid,
                            UnicodeString activationToken="");

        static UnicodeString GetMemberActivationCode(
                            Database::TDBTransaction &DBTransaction,
                            int contactKey,
                            int siteId);

        static void LoadMemberInformationForLatestActivationCode(
                                                        TMMContactInfo &outContactInfo);

        static bool IsLoyaltyMateEnabledGUID(UnicodeString uuid);

        static UnicodeString GetLoyaltyMateDisabledCloudUUID();

       static int GetPointsType(TPointsTransactionType pointType);

       static void SetLoyaltymateTransactions(
                                    Database::TDBTransaction &DBTransaction,
                                    int contactKey,
                                    TSyndCode syndicateCode,
                                    TContactPoints PointsToLoad);
};
//---------------------------------------------------------------------------

#endif
