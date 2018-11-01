//---------------------------------------------------------------------------

#ifndef ManagerMembershipGUIH
#define ManagerMembershipGUIH
#include "MembershipManagerSmartCards.h"
//---------------------------------------------------------------------------
class TManagerMembershipGUI : public TManagerMembershipSmartCards
{
	public :
   TModalResult AddMember(TMMContactInfo &Info, bool IsBarcodeCard = false,
	                       const bool triggered_by_preloaded_card = false);

	bool GetMembershipDetailsFromGUI(
	  Database::TDBTransaction &tr,
	  TMMContactInfo &destination,
	  const bool triggered_by_preloaded_card);

	TModalResult EditMember(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info, bool donotUseEmail = false);

   // Used for Select Members Screen.
	eMemberSearchType DefaultSearchType;
	eVisibleInterface	DefaultInterfaceDisplay;

	TManagerMembershipGUI(Database::TDBControl &inDBControl,TMMSettings &inSystem, TModules &inModules);
	void Initialise(Database::TDBTransaction &DBTransaction);
    bool AddGiftCard(Database::TDBTransaction &tr,TMMContactInfo &Info);
    bool GiftCardSecurityCheck(Database::TDBTransaction &tr);

};

#endif
