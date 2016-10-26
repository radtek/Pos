// ---------------------------------------------------------------------------

#ifndef MembershipManagerSmartCardsH
#define MembershipManagerSmartCardsH
// ---------------------------------------------------------------------------
#include "ManagerMembership.h"
#include "SmartCardAPI.h"
#include "PaymentTransaction.h"
#include "DBTierLevel.h"
#include "DBContacts.h"
#include <Dateutils.hpp>
// ---------------------------------------------------------------------------
class TfrmLoyaltyMateOperationDialogBox;
// ---------------------------------------------------------------------------
class TManagerMembershipSmartCards : public TManagerMembership
{

public:
   bool CardReplaced(Database::TDBTransaction &DBTransaction, int ContactKey, UnicodeString Atr);
   bool CardBanned(Database::TDBTransaction &DBTransaction, int ContactKey, UnicodeString Atr);
   void SaveContactInfoEditedToSmartCard(TMMContactInfo &inContactInfo);

   virtual TModalResult AddMember(
     TMMContactInfo &Info, bool IsBarcodeCard = false,
     const bool triggered_by_preloaded_card = false) = 0;

   void SaveContactInfoAddedToSmartCard(
	  TMMContactInfo &inContactInfo,
	  bool can_overwrite_already_initialized_card = false);

   void SaveContactInfoReassignedToSmartCard(TMMContactInfo &inContactInfo);
   bool SavePointsTransactionsToSmartCard(TContactPoints &Points,AnsiString inInvoiceNumber,bool PointsFromCloud = false);
   bool SavePointsTransactionsForBarcodeCard(TContactPoints &Points,TMMContactInfo &UserInfo,AnsiString inInvoiceNumber, bool PointsFromCloud = false);
   void AddDefaultPoints(Database::TDBTransaction &DBTransaction,TContactPoints &Points,int contactkey);
   void BlockAllOfContactsSmartCards(Database::TDBTransaction &DBTransaction, int contactKey);
   void SetSmartCardRestorePoint(Database::TDBTransaction &DBTransaction, TMMContactInfo &MMContactInfo, TSmartCardBlock *RestorePoint,
	  TSyndCode &SyndCode);
   bool GetSmartCardRestorePointByCard(Database::TDBTransaction &DBTransaction, TSmartCardBlock &RestorePoint);
   bool GetSmartCardRestorePointByContact(Database::TDBTransaction &DBTransaction, int ContactKey, TSmartCardBlock &RestorePoint);
   bool GetSmartCardRestorePointBySmartCardKey(Database::TDBTransaction &DBTransaction, int SmartCardKey, TSmartCardBlock &RestorePoint);
   bool CardCheckBannedList(Database::TDBTransaction &DBTransaction, int ContactKey, UnicodeString Atr);
   int GetNextSmartCardNumber(Database::TDBTransaction &DBTransaction);
   void GetReportCardInfo(Database::TDBTransaction &DBTransaction,int inSmartCardKey, TSmartCardBlock *SmartCardBlock, TStringList *Report);
   void GetReportMemberCardsInfo(Database::TDBTransaction &DBTransaction, TMMContactInfo &Member, TStringList *Report);
   void GetSmartCardReportForCardKey(Database::TDBTransaction &DBTransaction, int SmartCardKey, TStringList *Report);
   void virtual Initialise(Database::TDBTransaction &DBTransaction);

   TManagerSyndCode ManagerSyndicateCode;
   TManagerSyndCode& GetSyndicateCodeManager();

   std::auto_ptr <TManagerSmartCard> ManagerSmartCards;

   void OnCardInserted(TSystemEvents *Sender);
   void LocalCardInsertedHandler(TSystemEvents *Sender);
   void LoyaltymateCardInsertedHandler(TSystemEvents *Sender);

   void OnUnknownCardInserted(TSystemEvents *Sender);
   void OnUnableToDecodeCard(TSystemEvents *Sender);
   void OnBlankCardInserted(TSystemEvents *Sender);
   void OnUpDateRestorePoint(TSystemEvents *Sender);
   void OnCardCorrupt(TSystemEvents *Sender);
   void OnCardUnreadable(TSystemEvents *Sender);

   void BeginMemberTransaction();
   void EndMemberTransaction();
   void RestoreCard();
   void RestoreCardWithPoints();
   void ReassignCard();
   void ExploreCard();

   bool ProcessPoints(TPaymentTransaction &PaymentTransaction);

   bool CardIssueInProgress;
   bool AllowSmartCardNotPresentMemberTransactions;

   TLoginSuccess GetMember(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo, eMemberSource &MemberSource,
	  bool AllowDB = false);
   TLoginSuccess FindMember(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo);

   virtual Currency GetValue(Database::TDBTransaction &DBTransaction, int inContactKey);
   TManagerMembershipSmartCards(Database::TDBControl &inDBControl, TModules &inModules);
   ~TManagerMembershipSmartCards();
   bool MemberCodeScanned(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo,AnsiString memberCardCode);
   bool UpdateMemberCardCode(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo,AnsiString memberCardCode);
   bool GetMemberDetailFromBarcode(TMMContactInfo &MMContactInfo,AnsiString memberCode);
   bool GetMemberDetailFromEmail(TMMContactInfo &MMContactInfo);
   void SyncBarcodeMemberDetailWithCloud(TMMContactInfo &MMContactInfo);
   void RewardBirthdaybenefit(TPaymentTransaction &PaymentTransaction);
   void RewardFirstVisitPoints(TPaymentTransaction &PaymentTransaction);
private:
    void SaveContactProfileOnDownload(Database::TDBTransaction &DBTransaction,TMMContactInfo &Info);
    void SaveContactInfoOnDownload(Database::TDBTransaction &DBTransaction,TMMContactInfo &Info);
	int GetSmartCardStatus(Database::TDBTransaction &DBTransaction, int inContactKey, UnicodeString Atr);
	void BlockNewSmartCard(Database::TDBTransaction &DBTransaction, TMMContactInfo &MMContactInfo, TSyndCode &SyndCode);
    void performLoyaltyMateOperations();
	bool runMemberDownloadThread(TSyndCode CurrentSyndicateCode, TMMContactInfo &SmartCardContact,
                                 bool useUUID, bool useMemberCode, bool useEmail,bool &memberNotExist);
	AnsiString GetActivationEmailFromUser();
	TDateTime GetCardCreationDateFromContactKey(Database::TDBTransaction &DBTransaction, int inContactKey);
    TfrmLoyaltyMateOperationDialogBox* _lmOperationDialogBox;
	void __fastcall loyaltyMateMemberCreationCompleted(TObject* sender);
    void GetMemberDetail(TMMContactInfo &MMContactInfo);
    void SaveTransactionInvoiceDetail(TPaymentTransaction &PaymentTransaction);
    void LinkMembers(Database::TDBTransaction &DBTransaction, int contactToReplace, int contactKey, MemberMode memberMode);
    bool LinkSmartCard(Database::TDBTransaction &DBTransaction,int contactKey,TMMContactInfo &SmartCardContact);
    bool HasCard(Database::TDBTransaction &DBTransaction,int contactKey);
protected:
    bool createMemberOnLoyaltyMate(TSyndCode syndicateCode, TMMContactInfo &inContactInfo);   //this method is protected so it can be called from ManagerMembershipGUI
    void UpdateMemberCardCodeToDB(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo,AnsiString memberCardCode);
};

#endif
