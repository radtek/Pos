//---------------------------------------------------------------------------

#ifndef ManagerLoyaltyMateH
#define ManagerLoyaltyMateH

#include "MMContactInfo.h"
#include "Membership.h"
#include "LogsDB.h"
#include "MM_DBCore.h"
#include "DBContacts.h"
#include "DeviceRealTerminal.h"
#include "LoyaltyMateInterface.h"
#include "SalesforceAuthInfo.h"
#include "Comms.h"
#include "LoyaltyPrinting.h"
#include "LoyaltyMateUtilities.h"
#include "GlobalSettings.h"
#include "DBTierLevel.h"
#include "ManagerSyndCode.h"

#define UPGRADE_CARD_IF_NEEDED 0x01


struct TPointsDetail
{
 AnsiString uuid;
 TDateTime StartDate;
 TDateTime EndDate;
 int PointsType;
 double PointBalance;
 bool ServiceResponse;
};


//---------------------------------------------------------------------------

class TLoyaltyMateThread: public TThread
{
    private:
        std::vector<TLoyaltyMateMemberOperation> uploadMap;
        bool processingMembers;

        bool updateMemberOnCloud(
                        TSyndCode uuid,
                        const TMMContactInfo* info);

        bool postMemberTransactionsToCloud(TLoyaltyMateTransaction transaction);

        void logErrorToDB(
                    UnicodeString Function,
                    UnicodeString Type,
                    UnicodeString Msg,
                    UnicodeString Terminal="");

        void updateMembers();
        void uploadPendingTransactions();


    protected:
        virtual void __fastcall Execute();

    public:
        TLoyaltyMateThread();
        void SetUploadQueue(std::vector<TLoyaltyMateMemberOperation> queue);
};
//---------------------------------------------------------------------------
class TLoyaltyMateDownloadMemberThread: public TThread
{
	private:
		TSyndCode  syndicateCode;
		bool replacePoints;
		void threadTerminated();
        void downloadMemberFromCloudUsingEmail();
        void downloadMemberFromCloudUsingUUID();
        void downloadMemberFromCloudUsingCode();

	protected:
		virtual void __fastcall Execute();

	public:
		TLoyaltyMateDownloadMemberThread(TSyndCode inSyndicateCode, bool inReplacePoints);
		bool OperationSuccessful;
		AnsiString ErrorMessage;
		TMMContactInfo ReturnContactInfo;
        bool DownLoadFromUUID;
        AnsiString UUID;
        bool DownLoadFromCode;
        AnsiString MemberCode;
        bool DownLoadFromEmail;
        AnsiString MemberEmail;
};
//--------------------------------------------------------------------------
class TLoyaltyMateCreateMemberThread: public TThread
{
	private:
		TSyndCode _syndicateCode;
		TMMContactInfo* _memberInfo;
		void createMemberOnCloud();
		void threadTerminated();

	protected:
		virtual void __fastcall Execute();

	public:
		TLoyaltyMateCreateMemberThread(TSyndCode inSyndicateCode, TMMContactInfo* inContactInfo);
        bool OperationSuccessful;
		AnsiString ErrorMessage;
};


class TLoyaltyMateUpdateCardThread: public TThread
{
	private:
		TSyndCode _syndicateCode;
        AnsiString _uniqueId;
		AnsiString _memberCardCode;
		void UpdateMemberCardCodeOnCloud();
		void threadTerminated();

	protected:
		virtual void __fastcall Execute();

	public:
		TLoyaltyMateUpdateCardThread(TSyndCode inSyndicateCode, AnsiString uniqueId,AnsiString memberCardCode);
        bool OperationSuccessful;
		AnsiString ErrorMessage;
};




class TLoyaltyMateTierThread: public TThread
{
	private:
		TSyndCode _syndicateCode;
		TTierLevel* _tierLevel;

		void createTierOnCloud();
        void updateTierOnCloud();
        void deleteTierOnCloud();
		void threadTerminated();

	protected:
		virtual void __fastcall Execute();

	public:
		TLoyaltyMateTierThread(TSyndCode inSyndicateCode, TTierLevel* tierLevel);
        bool OperationSuccessful;
		AnsiString ErrorMessage;
        bool IsUpdate;
        bool IsDelete;
};

class TLoyaltyMateTierSyncThread: public TThread
{
	private:
		TSyndCode _syndicateCode;
		void SyncTierLevels();
		void threadTerminated();

	protected:
		virtual void __fastcall Execute();

	public:
		TLoyaltyMateTierSyncThread(TSyndCode inSyndicateCode);
        bool OperationSuccessful;
		AnsiString ErrorMessage;
};



//--------------------------------------------------------------------------
class TManagerLoyaltyMate
{
    private:
        static TManagerLoyaltyMate* instance;
        TLoyaltyMateThread* loyaltyThread;
        TTimer* loyaltyThreadTimer;
        bool loyaltyThreadTerminated;
        std::vector<TLoyaltyMateMemberOperation> uploadQueue;

        // Constructor and Destructor
        TManagerLoyaltyMate();
        ~TManagerLoyaltyMate();

        void __fastcall loyaltyTheadTerminate( TObject* sender );
        void initiateLoyaltyThread();
        void startLoyaltyThread();

        void startLoyaltyThreadTimer();
        void stopLoyaltyThreadTimer();

        void __fastcall OnLoyaltyThreadTimerTick(TObject *Sender);

        void printActivationToken(
                            Database::TDBTransaction &DBTransaction,
                            UnicodeString activationToken,
                            UnicodeString membershipNumber,
                            UnicodeString memberName);

    public:

        static TManagerLoyaltyMate* Instance()
        {
            if(instance == NULL)
                instance = new TManagerLoyaltyMate();

            return instance;
        }
        bool CheckServiceStatus();
        // Sync memeber information with the cloud. All operations will be performed here using a new thread
        void SyncMemberDetailsWithCloud(
                                    TSyndCode syndicateCode,
                                    TMMContactInfo info);

        // creates the member on the cloud.
        bool CreateMemberOnCloud(
							TSyndCode syndicateCode,
                            TMMContactInfo &info);

        // gets member details from a uuid
        bool GetMemberDetailsFromUUID(
                                TSyndCode syndicateCode,
                                AnsiString uuid,
                                TMMContactInfo &outInfo,
                                bool replacePoints = false);

        // gets member details from a code
        bool GetMemberDetailsFromCode(
                                TSyndCode syndicateCode,
                                AnsiString memberCode,
                                TMMContactInfo &outInfo,
                                bool replacePoints = false);

        // creates the tier level on the cloud.
        bool CreateTierOnCloud(TSyndCode syndicateCode,TTierLevel* tierLevel);
        bool UpdateTierOnCloud(TSyndCode syndicateCode,TTierLevel* tierLevel);
        bool DeleteTierOnCloud(TSyndCode syndicateCode,TTierLevel* tierLevel);
        bool SyncTierLevelWithCloud(TSyndCode syndicateCode);
        bool UpdateMemberCardCodeOnCloud(TSyndCode syndicateCode,AnsiString uniqueId,AnsiString memberCardCode,AnsiString &ErrorMessage);
};
#endif
