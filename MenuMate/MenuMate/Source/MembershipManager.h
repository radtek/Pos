//---------------------------------------------------------------------------

#ifndef MembershipManagerH
#define MembershipManagerH

#include "Membership.h"
#include "Contact.h"
#include "SystemEvents.h"
#include "MM_DBCore.h"
#include "Modules.h"
#include "MMSettings.h"
#include "ManagerSyndCode.h"
#include "PointsTransactions.h"
#include "ContactMemberApplied.h"

//---------------------------------------------------------------------------
enum eMemberSearchType {eSearchByName, eSearchByNumber, eSearchByPhone};
enum eMemberDisplayFilter {eMemberAll,eMemberFinanical,eMemberNonFinanical};

//---------------------------------------------------------------------------

class TManagerMembership
{
   protected :

   Database::TDBControl &DBControl;
   Database::TDBControl IBMembershipDatabase;
   TModules &Modules;
   TManagerVariable &Vars;

   void GetValidPointsTransactions(Database::TDBTransaction &DBTransaction,int ContactKey,std::vector<TPointsTransaction> &PointsTransactions);
   void UpdatePointsTransactions(Database::TDBTransaction &DBTransaction,std::vector<TPointsTransaction> &PointsTransactions);
	void GetReportMemberInfo(TMMContactInfo &Member,TSyndCode &SyndCode,UnicodeString Title,TStringList *Report);
   public :

	bool CheckVersionNumber(Database::TDBTransaction &IBTransaction);

   TManagerMembership(Database::TDBControl &inDBControl, TModules &inModules,TManagerVariable &inVars);
   virtual ~TManagerMembership();

	//TContactMember *MembershipSystem;
	std::auto_ptr<TMembership> MembershipSystem;

	void virtual Initialise(Database::TDBTransaction &DBTransaction);
	bool OpenMembershipDB(Database::TDBTransaction &IBTransaction,bool Reconfigure);

   void RegisterTransaction(Database::TDBTransaction &DBTransaction);

   void ArchivePoints(Database::TDBTransaction &DBTransaction,TPointsTransaction &PointsTransaction);   
	virtual Currency GetValue(Database::TDBTransaction &DBTransaction,int inContactKey);

   virtual TModalResult AddMember(
     TMMContactInfo &Info,
     const bool triggered_by_smartcard_event = true) = 0;

   TModalResult EditMember(Database::TDBTransaction &DBTransaction,TMMContactInfo &Info);
	TModalResult DeleteMember(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info);
	TLoginSuccess GetMember(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo,eMemberSource &MemberSource, bool AllowDB = false);
	virtual TLoginSuccess FindMember(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo);

	void SyncMembers(Database::TDBTransaction &DBTransaction,TfrmProcessing *Progress);
   void BuildXMLListMembership(Database::TDBTransaction &DBTransaction,TPOS_XMLBase &Data);
   void BuildXMLMemberUpdate(Database::TDBTransaction &DBTransaction,TMMContactInfo &Member,TPOS_XMLBase &Data);
};

#endif

