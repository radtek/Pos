//---------------------------------------------------------------------------

#ifndef DBTierLevelH
#define DBTierLevelH
//---------------------------------------------------------------------------
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <IBTable.hpp>
#include "MM_DBCore.h"
#include "MMContactInfo.h"
struct TTierLevel
{
    int TierKey;
    int CloudId;
    int Level;
    UnicodeString Name;
    int PointRequired;
    int BirthDayBonus;
    Currency PointRedemRate;
    int WeighedRedemWeight;
    int WeighedRedemPoint;
    bool SendMailToHeadOffice;
    bool SendMailToMember;
    bool ChangeCard;
    bool AllowEarntRedemption;
};

class TDBTierLevel
{
	public:
         TDBTierLevel();
         ~TDBTierLevel();
        static int GetBirthdayBonusPoints(Database::TDBTransaction &DBTransaction, int tierLevel);
        static void DeleteTierLevel(Database::TDBTransaction &DBTransaction,int tierKey);
        static void AddTierLevel(Database::TDBTransaction &DBTransaction, TTierLevel *TierLevel);
        static void UpdateTierLevel(Database::TDBTransaction &DBTransaction, TTierLevel *TierLevel) ;
        static void GetTierLevelInformation(Database::TDBTransaction &DBTransaction, TTierLevel *TierLevel);
        static void GetTierLevelInformationByTierLevel(Database::TDBTransaction &DBTransaction, TTierLevel *TierLevel);
        static bool CheckTierLevelExist(Database::TDBTransaction &DBTransaction, TTierLevel *TierLevel,bool IsEditMode);
        static void GetTierLevels(Database::TDBTransaction &DBTransaction,TStringList *TierLevels);
        static int GetNextTierLevel(Database::TDBTransaction &DBTransaction, int tierLevel);
        static int GetPointsrequired(Database::TDBTransaction &DBTransaction, int tierLevel);
        static int GetTierLevelAsPerEarnedPoints(Database::TDBTransaction &DBTransaction, double earnedPoints);
        static AnsiString GetTierLevelName(Database::TDBTransaction &DBTransaction, int tierLevel);
        static void UpdateMemberTierLevel(Database::TDBTransaction &DBTransaction, TMMContactInfo &Member);
        static int GetTierCloudId(Database::TDBTransaction &DBTransaction, int tierLevel);
        static int GetTierLevelFromCloudId(Database::TDBTransaction &DBTransaction,int cloudId);
        static void GetTierLevelOfMember(Database::TDBTransaction &DBTransaction,TMMContactInfo &Member);
        static bool IsAllowEarnedRedemption(Database::TDBTransaction &DBTransaction,int tierLevel);
        static void ClearAll(Database::TDBTransaction &DBTransaction);
};

#endif
