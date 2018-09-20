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


struct TaskResult
{
   bool OperationSuccussful;
   AnsiString ErrorMessage;
};


class TLoyaltyMateThread: public TThread
{
    private:
        std::vector<TLoyaltyMateMemberOperation> uploadMap;
        bool processingMembers;
        bool UpdateMemberOnCloud(TSyndCode uuid,const TMMContactInfo* info);
        bool PostMemberTransactionsToCloud(TLoyaltyMateTransaction transaction);
        void LogErrorToDB(UnicodeString Function,UnicodeString Type,UnicodeString Msg,UnicodeString Terminal="");
        void UpdateMembers();
        void UploadPendingTransactions();
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
		void ThreadTerminated();
        void DownloadMemberFromCloudUsingEmail();
        void DownloadMemberFromCloudUsingUUID();
        void DownloadMemberFromCloudUsingCode();

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
		void CreateMemberOnCloud();
		void ThreadTerminated();

	protected:
		virtual void __fastcall Execute();

	public:
		TLoyaltyMateCreateMemberThread(TSyndCode inSyndicateCode, TMMContactInfo* inContactInfo);
        bool OperationSuccessful;
		AnsiString ErrorMessage;
};
//--------------------------------------------------------------------------
class TLoyaltyMateUpdateCardThread: public TThread
{
	private:
		TSyndCode _syndicateCode;
        AnsiString _uniqueId;
		AnsiString _memberCardCode;
		void UpdateMemberCardCodeOnCloud();
		void ThreadTerminated();

	protected:
		virtual void __fastcall Execute();

	public:
		TLoyaltyMateUpdateCardThread(TSyndCode inSyndicateCode, AnsiString uniqueId,AnsiString memberCardCode);
        bool OperationSuccessful;
		AnsiString ErrorMessage;
};
//--------------------------------------------------------------------------
class TLoyaltyMateSyncThread: public TThread
{
	private:
		TSyndCode _syndicateCode;
		void SyncCompanyDetails();
		void ThreadTerminated();

	protected:
		virtual void __fastcall Execute();

	public:
		TLoyaltyMateSyncThread(TSyndCode inSyndicateCode);
        bool OperationSuccessful;
		AnsiString ErrorMessage;
};
//--------------------------------------------------------------------------
class TLoyaltyMateGiftVoucherThread: public TThread
{
	private:
		TSyndCode _syndicateCode;
		void GetGiftVoucherBalance();
		void ThreadTerminated();

	protected:
		virtual void __fastcall Execute();

	public:
		TLoyaltyMateGiftVoucherThread(TSyndCode inSyndicateCode);
        TGiftCardDetail GiftCardDetail;;
        AnsiString GiftCardNumber;
        bool OperationSuccessful;
		AnsiString ErrorMessage;
};
//--------------------------------------------------------------------------
class TLoyaltyMateVoucherProcessThread: public TThread
{
	private:
		TSyndCode _syndicateCode;
        void ProcessVouchers();
		void ThreadTerminated();

	protected:
		virtual void __fastcall Execute();

	public:
        TLoyaltyMateVoucherProcessThread(TSyndCode inSyndicateCode);
        TVoucherUsageDetail VoucherUsageDetail;
        bool OperationSuccessful;
        AnsiString ErrorMessage;
};
//--------------------------------------------------------------------------
class TLoyaltyMatePocketVoucherThread: public TThread
{
	private:
		TSyndCode _syndicateCode;
		void GetPocketVoucherDetail();
		void ThreadTerminated();

	protected:
		virtual void __fastcall Execute();

	public:
        TLoyaltyMatePocketVoucherThread(TSyndCode inSyndicateCode);
        //Voucher fetch
        TVoucherDetail VoucherDetail;
        AnsiString VoucherNumber;
        bool OperationSuccessful;
        AnsiString ErrorMessage;
};
//--------------------------------------------------------------------------
class TLoyaltyMateReleaseVoucherThread: public TThread
{
	private:
		TSyndCode _syndicateCode;
		void ReleaseVouchers();
		void ThreadTerminated();

	protected:
		virtual void __fastcall Execute();

	public:
        TLoyaltyMateReleaseVoucherThread(TSyndCode inSyndicateCode);
        TReleasedVoucherDetail ReleasedVoucherDetail;
        bool OperationSuccessful;
        AnsiString ErrorMessage;
};
//--------------------------------------------------------------------------
class TLoyaltyMateOnlineOrderingThread: public TThread
{
	private:
		TSyndCode _syndicateCode;
		void SyncOnlineOrderingDetails();
		void ThreadTerminated();
        void UnsetSignalRStatusAtCloud();

	protected:
		virtual void __fastcall Execute();

	public:
        TLoyaltyMateOnlineOrderingThread(TSyndCode inSyndicateCode);
        bool OperationSuccessful;
        AnsiString ErrorMessage;
        bool UnsetSignalRStatus;
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

        void __fastcall LoyaltyTheadTerminate( TObject* sender );
        void InitiateLoyaltyThread();
        void StartLoyaltyThread();
        void StartLoyaltyThreadTimer();
        void StopLoyaltyThreadTimer();
        void __fastcall OnLoyaltyThreadTimerTick(TObject *Sender);

    public:

        static TManagerLoyaltyMate* Instance()
        {
            if(instance == NULL)
                instance = new TManagerLoyaltyMate();

            return instance;
        }

        // Sync memeber information with the cloud. All operations will be performed here using a new thread
        void SyncMemberDetailsWithCloud(TSyndCode syndicateCode,TMMContactInfo info);
        // creates the member on the cloud.
        TaskResult CreateMemberOnCloud(TSyndCode syndicateCode,TMMContactInfo &info);
        void TriggerPointSync();
};
//--------------------------------------------------------------------------
#endif
