//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DB.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <ComCtrls.hpp>
#include <IBSQL.hpp>
#include <ExtCtrls.hpp>
#include <IBServices.hpp>
#include <IBTable.hpp>
#include <Dialogs.hpp>

#include "MM_DBCore.h"
#include "TouchBtn.h"
#include "TouchControls.h"
//---------------------------------------------------------------------------

enum TSecurityTypes { secOrderedBy, secCredit, secCancel,secPriceAdjust,secFloatSet,secFloatAdjust,secZedded,secOpenDraw,secBilledBy,secDiscountedBy, secNone};
enum TSecurityFields { secfUserKey, secfEvent, secfFrom, secfTo,secfTimeStamp};
enum OrderStatus {NormalOrder,DeleteOrder,CreditOrder,CreditNonExistingOrder};
enum TContactType {eStaff, eLoyalty,eDeletedStaff, eDeletedLoyalty};
const char* const SecurityTypes[] = {"Ordered By",
                                       "Credit",
                                       "Cancel",
                                       "Price Adjust",
                                       "Float Set",
                                       "Float Adjustment",
                                       "Till Z Off",
                                       "Manually Opened Cash Drawer",
                                       "Billed By",
                                       "Discounted By",
                                       NULL};



class TfrmMain : public TForm
{
__published:	// IDE-managed Components
   TMemo *Memo;
   TMemo *memMMCreateSQL;
   TProgressBar *barProgress;
   TLabel *lbeProgress;
	TOpenDialog *diaOpen;
   TTouchBtn *TouchBtn1;
   TTouchBtn *TouchBtn2;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall btnCloseClick(TObject *Sender);
   void __fastcall TouchBtn1MouseClick(TObject *Sender);
private:	// User declarations
   void Ver2_1_19_X_UpdateOrderTypeSecurity(Database::TDBTransaction &DBTransaction,TIBSQL *IBQuery,TSecurityTypes secType);
public:		// User declarations
	AnsiString DBPath;
	AnsiString ServerName;
	Database::TDBControl DBControl;   
	bool CreateRequired;
	__fastcall TfrmMain(TComponent* Owner);
	void GetDBLocation();	
   void Patch();
	bool DB1Connect();
	bool DB2Backup();
   void UpDateAll();
   void DB6Disconnect();
   void Log(AnsiString Log);
   void ToggleControls();
   void RemoveDemoMenuMateDB();
   int GetUserFromTabKey(Database::TDBTransaction &DBTransaction,int TabKey,AnsiString &UserName);
   int GetUserFromDayArcBillKey(Database::TDBTransaction &DBTransaction,int Key, AnsiString &UserName);
   int GetUserFromArcBillKey(Database::TDBTransaction &DBTransaction,int Key, AnsiString &UserName);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
