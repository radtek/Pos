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

#include "UpgradeVersions.h"

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
	TOpenDialog *diaOpen;
    TPanel *pnBottom;
    TPanel *pnLeft;
    TPanel *pnTop;
    TPanel *Panel4;
    TPanel *pnMiddle;
    TMemo *Memo;
    TMemo *memMMCreateSQL;
    TPanel *pnProgress;
    TLabel *lbeProgress;
    TProgressBar *barProgress;
    TPanel *pnButtons;
    TTouchBtn *TouchBtn1;
    TPanel *Panel1;
    TListView *lvLog;
    TPanel *pnCloseBtn;
    TTouchBtn *TouchBtn2;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *lbeStartTime;
    TLabel *lbeFinishTime;
    TPanel *pnHeader;
    TPanel *c;
    TPanel *Panel3;
    TLabel *lbeHeader;
    TLabel *lbeDBPath;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall btnCloseClick(TObject *Sender);
   void __fastcall TouchBtn1MouseClick(TObject *Sender);
    void __fastcall lvLogCustomDrawItem(TCustomListView *Sender, TListItem *Item,
          TCustomDrawState State, bool &DefaultDraw);
    void __fastcall lvLogCustomDrawSubItem(TCustomListView *Sender, TListItem *Item,
          int SubItem, TCustomDrawState State, bool &DefaultDraw);
private:	// User declarations
   void Ver2_1_19_X_UpdateOrderTypeSecurity(Database::TDBTransaction &DBTransaction,TIBSQL *IBQuery,TSecurityTypes secType);
public:		// User declarations
	UnicodeString DBPath;
	UnicodeString ServerName;
	Database::TDBControl DBControl;   
	bool CreateRequired;
	bool BackupRequired;
	__fastcall TfrmMain(TComponent* Owner);
	void GetDBLocation();	
   void UpdateVersion2_1_18_X();
   void UpdateVersion2_1_19_X();
   void UpdateVersion2_2_00_X();
   void UpdateVersion2_3_00_X(); // Catagories and index fixes, Receipts ordered.
   void UpdateVersion2_4_00_X(); // Stock Locations @ receipe level.
   void UpdateVersion2_5_00_X(); // Payment types surcharges
   void UpdateVersion2_6_00_X(); // Rooms
   void UpdateVersion2_7_00_X();
	void UpdateVersion_2_8();
	void UpdateVersion_2_9();      
	void UpdateVersion_3_0();
	void UpdateVersion_3_1();
	void UpdateVersion_3_2();
	void UpdateVersion_3_21();
	void UpdateVersion_3_22();   
	void UpdateVersion_3_23();
	void UpdateVersion_3_24();
	void UpdateVersion_3_231();
	void UpdateVersion_3_232();	
	void UpdateVersion_3_3();
	void UpdateVersion_3_31();
	void UpdateVersion_3_32();		
	void UpdateVersion_3_33();
	void UpdateVersion_3_34();
	void UpdateVersion_3_35();
	void UpdateVersion_4_0();
	void UpdateVersion_4_1();
   void UpdateVersion_4_2();
   void UpdateVersion_4_3();
   void UpdateVersion_4_4();
   void UpdateVersion_4_5();
   void UpdateVersion_4_6();
   void UpdateVersion_4_7();
   void UpdateVersion_4_8();
   void UpdateVersion_4_9();
   void UpdateVersion_4_10();
   void UpdateVersion_4_11();
   void UpdateVersion_4_12();
   void UpdateVersion_4_13();
   void UpdateVersion_4_14();
	void UpdateVersion_5_0();
	void UpdateVersion_5_1();
   void UpdateVersion_5_2();
	void UpdateVersion_5_3();
	void UpdateVersion_5_4();
	void UpdateVersion_5_5();
	void UpdateVersion_5_6();
	void UpdateVersion_5_7();
	void UpdateVersion_5_8();
	void UpdateVersion_5_9();
	void UpdateVersion_5_10();
	void UpdateVersion_5_11();
	void UpdateVersion_5_12();
	void UpdateVersion_5_13();
	void UpdateVersion_5_13_0_3();
    void UpdateVersion_5_14();
	void UpdateVersion_5_15();
	void UpdateVersion_5_16();
	void UpdateVersion_5_17();

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// NEW PARSER FROM VERSION 5.18.
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	void UpdateVersionNew();

    void SetProgressLabel(   UnicodeString inText );
    void SetHeaderLabel(     UnicodeString inText );
    void SetDBPathLabel(     UnicodeString inText );
    void SetStartTimeLabel(  UnicodeString inText );
    void SetFinishTimeLabel( UnicodeString inText );
    void SetProgressBarMax( int  inMax );
    void StepProgressBar(   int  inStep );
    int  GetVersionCount();

	//..........................................................................

	bool DB1Connect();
	bool DB2Backup();
   void UpDateAll();
   void DB6Disconnect();
   void Log(AnsiString Log);
   void ToggleControls();
   void RemoveDemoMenuMateDB();

   bool HasBeenUpgradedTo(const UnicodeString &version);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
