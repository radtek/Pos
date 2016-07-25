//---------------------------------------------------------------------------
#ifndef DeviceKioskH
#define DeviceKioskH
//---------------------------------------------------------------------------
#include <memory>
#include "Enum.h"
#include "MM_DBCore.h"
#include "ManagerMembership.h"
#include "MMlogging.h"

#include "DevicePC.h"
#include "GlobalSettings.h"

#include <mshtml.h>
#include <oleacc.h>
#include "SHDocVw_OCX.h"
#include <OleCtrls.hpp>
#include <SHDocVw.hpp>

enum eDispMode {eDispModePoints,eDispModeFinancial};

class TDeviceKiosk : public TDevicePC
{
private :
	TWebBrowser *Display;
	bool Enabled;
	UnicodeString Printer;
	eDispMode Mode;
public:
   bool htmlbusy;
	std::auto_ptr<TTimer> Timer;

	void __fastcall OnTimerTick(TObject *Sender);
  	TSystemEvents EventTimer;

	void OnRefreshDisplay(TSystemEvents *Sender);
    bool UserDisplayed;
	void OnRefreshUsers(TSystemEvents *Sender);

	__int64 EventTimerLastFired;
	__int64 DisplayLastUpdated;

	std::auto_ptr<TManagerMembership> ManagerMembership;
    std::auto_ptr<TManagerLogs> Logging;


   virtual void RegisterTransaction(Database::TDBTransaction &DBTransaction);

	Database::TDBControl DBControl;

   bool OpenDatabases();
	bool OpenMenuMateDB();
	bool OpenMembershipDB(bool Reconfigure = false);
	void CloseDatabases();
	bool Online();

   TDeviceKiosk(TWebBrowser *inwebDisplay);
   virtual __fastcall ~TDeviceKiosk();
	void Navigate(TStringList *Html);
	void __fastcall RefreshDisplay();

	void Initialise(Database::TDBTransaction &DBTransaction);
   void CardSwipe(Database::TDBTransaction &DBTransaction, UnicodeString Card);
   void CheckLocation(Database::TDBTransaction & DBTransaction);

	void GetKioskDisplayMsg(Database::TDBTransaction & DBTransaction,TMMContactInfo &Member, std::map<UnicodeString,UnicodeString> &Data);
	void PrintKioskDocket(TStringList *Docket);
    void SetDisplayMode(Database::TDBTransaction & DBTransaction,eDispMode Mode);
    void PrintMemberDocket(Database::TDBTransaction & DBTransaction, int MembershipNumber);
    eDispMode GetDisplayMode();
	void DisplayEntrances(Database::TDBTransaction & DBTransaction);
	void SetMembershipMode(Database::TDBTransaction & DBTransaction, eMembershipType inMode);
	eMembershipType GetMembershipType();
};

#endif
