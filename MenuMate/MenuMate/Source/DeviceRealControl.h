//---------------------------------------------------------------------------

#ifndef DeviceRealControlH
#define DeviceRealControlH

#include "MM_DBCore.h"
#include "DevicePC.h"
#include "SecurityPort.h"
#include "ManagerStock.h"
#include "ManagerNet.h"
#include "KitchenMod.h"
#include "MMContactInfo.h"

#include "NetMessageHappyHour.h"
#include "NetMessageInfoSync.h"
#include "NetMessageMsg.h"
#include "Membership.h"
#include "NetMessageSeatName.h"
 #include <fstream>
#include "ManagerMenus.h"
 #include <vector>
#include "PointsRulesSetUtils.h"
//---------------------------------------------------------------------------
struct BarExachange{
        bool IsRequiredBarStockPriceUpdate;
};
class TDeviceRealControl : public TDevicePC
{
	private:
		static TDeviceRealControl *active_instance;
		TManagerMenus *raw_menu_manager;

   public:
		TManagerMenus *
		GetMenuManager()
		{
			return raw_menu_manager;
		};

	Database::TDBControl DBControl;
	std::auto_ptr<TManagerStock> ManagerStock;
 	std::auto_ptr<TManagerNet> ManagerNet;

	TKitchenMod *KitchenMod;

   TMMContactInfo User;

  	TSecurityPort *SecurityPort;

	/*
	 * The TDeviceRealControl class isn't a singleton. In practice, it is but
	 * for the purposes of keeping the system living I've created this new
	 * "ActiveInstance" function in TDeviceRealControl.
	 *
	 * Each subclass of TDeviceRealControl defines their own Instance() method,
	 * unfortunately, to access the instance we need to include that specific
	 * class an all of it's source - including dependencies. This breaks other
	 * programs which depend on TDeviceControl's Menu management features.
	 *
	 * This is utilized by TItemMinorComplete and has been added to allow use
	 * of TItemMinorComplete in PalmMate, Kiosk and POS without the need for
	 * #ifdefs.
	 */
	static TDeviceRealControl &
	ActiveInstance()
	{
		return *active_instance;
	};

	bool Reboot;
   bool ShutDown;
   int SumStrings(TStrings *Strings);
	virtual void UpdateHappyHourInfo(TNetMessageHappyHour *Request);
	void GetHappyHourInfo(TNetMessageHappyHour *Request);
   virtual void UpdateClockSyncInfo();
	virtual void UpdateInfoSync(TNetMessageInfoSync *Request);
   virtual void UpdatePeople(Database::TDBTransaction &DBTransaction,AnsiString FileName);
	virtual void UpdateCurrentRecipes(UnicodeString Code,UnicodeString Location,Currency Cost,double CostGSTPercent)=0;
	virtual void UpdateMessage(TNetMessageMsg * Request) = 0;

	void Registered(bool *pRegistered, UnicodeString *pRegisteredName);
	int  RegisteredPalmLicences();
	void RegisteredModule(bool *pModuleRegistered, eRegisteredModules Module);




	void UpdateSeatName(TNetMessageSeatName *Request);
	bool ContainsCompanyName(TStrings *inHeader, UnicodeString CompanyName);
	void LoadHdrFtr();


	TDeviceRealControl(TManagerMenus *raw_menu_manager);
	virtual __fastcall ~TDeviceRealControl();
	void Initialise(Database::TDBTransaction &DBTransaction);

   virtual void RegisterTransaction(Database::TDBTransaction &DBTransaction);
	TMembership * GetMembershipSystemPreInit(Database::TDBTransaction &DBTransaction);
	virtual bool OpenDatabases();
	void CheckLocation(Database::TDBTransaction & DBTransaction);

	void CloseDB();
	bool OpenMenuMateDB();
	bool OpenStockDB(bool Reconfigure = false);

   bool SetSiteID(Database::TDBTransaction &DBTransaction,bool Reset = false);
	void StatusReport(TStringList *html);

      virtual void UpdateBarStockPrice(Database::TDBTransaction &DBTransaction,AnsiString FileName);
       bool SelectBarStockTurnOver(Database::TDBTransaction &DBTransaction,AnsiString FileName);

 private:
//	void CreateProfile(Database::TDBTransaction &DBTransaction);
//	void virtual CreateDevice(Database::TDBTransaction &DBTransaction) = 0;
   virtual LoadRegistrationConfig();
   bool insertMemberPointsAdjustmentRecordToDB( Database::TDBTransaction &DBTransaction, int contactKey, Currency adjustment );
   AnsiString ThirdPartyCode;
   AnsiString price;
   int Counter;
   int linenumber;
   int ItemQty;
   int Iqty;


};

#endif
