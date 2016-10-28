//--------------------------------------------------------------------------

#ifndef ManagerMembershipH
#define ManagerMembershipH

#include "Membership.h"
#include "Contact.h"
#include "SystemEvents.h"
#include "MM_DBCore.h"
#include "Modules.h"
#include "MMSettings.h"
#include "ManagerSyndCode.h"
#include "ContactMemberApplied.h"
#include "POS_XMLBase.h"

//---------------------------------------------------------------------------
enum eMemberSearchType {eSearchByName, eSearchByNumber, eSearchByPhone};
enum eMemberDisplayFilter {eMemberAll,eMemberFinanical,eMemberNonFinanical};

//---------------------------------------------------------------------------

class TManagerMembership
{
   private :
        // Phone order flow member selection enabled or not.
        // This value combined with phone-order-flow setting in the maintainane section is used to bypass few GUI forms and directly sets the member when used within POS
        bool POFMemberSelectionEnabled;

   protected :

   Database::TDBControl &DBControl;
   Database::TDBControl IBMembershipDatabase;
   TModules &Modules;

   void GetValidPointsTransactions(Database::TDBTransaction &DBTransaction,int ContactKey,TPointsTransactionContainer &PointsTransactions);
   void UpdatePointsTransactions(Database::TDBTransaction &DBTransaction,std::vector<TPointsTransaction> &PointsTransactions);
	void GetReportMemberInfo(TMMContactInfo &Member,TSyndCode &SyndCode,UnicodeString Title,TStringList *Report);
   public :

	bool CheckVersionNumber(Database::TDBTransaction &IBTransaction);

   TManagerMembership(Database::TDBControl &inDBControl, TModules &inModules);
   virtual ~TManagerMembership();

	//TContactMember *MembershipSystem;
	std::auto_ptr<TMembership> MembershipSystem;

	void virtual Initialise(Database::TDBTransaction &DBTransaction);
	bool OpenMembershipDB(Database::TDBTransaction &IBTransaction,bool Reconfigure);

   void RegisterTransaction(Database::TDBTransaction &DBTransaction);

   void ArchivePoints(Database::TDBTransaction &DBTransaction, TPointsTransaction &PointsTransaction);

   TModalResult AddMember(
     TMMContactInfo &Info, bool IsBarcodeCard = false,
     const bool triggered_by_preloaded_card = false);

   TModalResult EditMember(Database::TDBTransaction &DBTransaction,TMMContactInfo &Info);

	TModalResult DeleteMember(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info);
	TLoginSuccess GetMember(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo,eMemberSource &MemberSource, bool AllowDB = false);
	virtual TLoginSuccess FindMember(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo);

	void SyncMembers(Database::TDBTransaction &DBTransaction,TfrmProcessing *Progress);
   void BuildXMLListMembership(Database::TDBTransaction &DBTransaction,TPOS_XMLBase &Data);
   void BuildXMLMemberUpdate(Database::TDBTransaction &DBTransaction,TMMContactInfo &Member,TPOS_XMLBase &Data);
   TLoginSuccess virtual Authorise(TMMContactInfo &UserInfo,Currency wrkPayAmount);
   virtual TLoginSuccess IsCard(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo);

    void SetPhoneOrderFlowMemberSelection(bool value){ POFMemberSelectionEnabled = value;}
    bool IsPOFMemberSelectionEnabled(){ return POFMemberSelectionEnabled; }
    virtual bool MemberCodeScanned(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo,AnsiString memberCardCode);
    virtual bool UpdateMemberCardCode(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo,AnsiString memberCardCode);


};

#endif

