//---------------------------------------------------------------------------

#ifndef DeviceRealTerminalH
#define DeviceRealTerminalH

#include "DeviceRealControl.h"
#include "ProxReader.h"

#include "ScaleModel.h"
#include "IMManager.h"
#include "ManagerPocketVoucher.h"
#include "PoleDisplay.h"
#include "SystemEvents.h"
#include "ListPaymentSystem.h"
#include "ManagerMembershipGUI.h"
#include "MMBarcodeReader.h"
#include "ManagerMenusPOS.h"
#include <cassert>
#include "ProcessingController.h"
#include "ManagerGeneralLedger.h"
#include "enumTerminalGlobalMessageTypes.h"
#include "BasePMS.h"
#include "PhoenixHotelSystem.h"

//---------------------------------------------------------------------------
class TDeviceRealTerminal : public TDeviceRealControl
{
public:

   #ifdef AutoMate
   class Network
   {
      public:
      Network();
      TList *DeviceImages;
      TDeviceImage* LocateByIP(AnsiString inIP);
      TDeviceImage* LocateByName(AnsiString inName);
   ~Network();
   void AddDeviceImageFromStatus(TReqStatus * Request);
   }Net;
   #endif

   std::auto_ptr<TScaleModel> Scales;
   std::auto_ptr<TIMManager> IMManager;
   std::auto_ptr<TManagerPocketVoucher> PocketVouchers;

   TPoleDisplay *PoleDisplay;

	static TDeviceRealTerminal& Instance() {
        static TDeviceRealTerminal singleton;
        return singleton;
    }

	TProxReader *ProxReader;

	std::auto_ptr<TTimer> Timer;
	void __fastcall OnTimerTick(TObject *Sender);
	__int64 EventLockOutLastFired;
	__int64 IMStatusLastFired;

	TSystemEvents EventLockOutTimer;
	void ResetEventLockOutTimer();

	TMMBarcodeReader *BarcodeReader;

   TManagerMenusPOS *
	GetMenuManagerAsPOS()
	{
		TManagerMenusPOS *manager =
		  dynamic_cast<TManagerMenusPOS *>(GetMenuManager());

		assert(manager != NULL);
		return manager;
	};

   __property TManagerMenusPOS *Menus = { read=GetMenuManagerAsPOS };

   TListPaymentSystem *PaymentSystem;

   TProcessingController ProcessingController;

   virtual void RegisterTransaction(Database::TDBTransaction &DBTransaction);
	std::auto_ptr<TManagerMembershipGUI> ManagerMembership;
	std::auto_ptr<TManagerGeneralLedger> ManagerGeneralLedger;
	virtual bool OpenDatabases();
	bool OpenMembershipDB(bool Reconfigure = false);

//   TTimer *TerminalTick;
   TDeviceRealTerminal();
   virtual __fastcall ~TDeviceRealTerminal();
   void SaveHdrFtr(TStrings *inHeader ,TStrings *inPHeader,TStrings *inFooter, TStrings *inVoidFooter,TStrings *inSubHeader);
   void SaveZedHeader(TStrings *inZedHeader);
   void LoadPrinterGraphic();
	void UpdateCurrentRecipes(UnicodeString Code,UnicodeString Location,Currency Cost, double CostGSTPercent);
	void UpdateMessage(TNetMessageMsg *Request);
	void Initialise(Database::TDBTransaction &DBTransaction);
	void TriggerTabStateChanged();
	// Events
	TSystemEvents AfterTabStateChanged;

	void BuildXMLVersion( TPOS_XMLBase &Data, int SiteID );

    // This collection holds number of global application messages shown throughout the system
    // by looking at the count we can determine whether a message box is already on screen if so do not display again
    std::map<TerminalGlobalMessageTypes,int> AppGlobalMessagesMap;
    std::auto_ptr<TBasePMS> BasePMS;
//    TBasePMS* BasePMS;

private:
//	void CreateDevice(Database::TDBTransaction &DBTransaction);
};
#endif
