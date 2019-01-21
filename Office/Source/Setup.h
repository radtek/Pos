//---------------------------------------------------------------------------

#ifndef SetupH
#define SetupH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <IBDatabase.hpp>
#include <IBSQL.hpp>
#include "Connections.h"
#include "Utilities.h"
#include <DB.hpp>
#include <IBCustomDataSet.hpp>
#include <IBQuery.hpp>
#include "NumericEdit.h"
//---------------------------------------------------------------------------
class TfrmSetup : public TForm
{
__published:	// IDE-managed Components
	TTabControl *tcCompanyNames;
	TPageControl *pcSettings;
	TTabSheet *tsMMConnection;
	TTabSheet *tsStockConnection;
    TTabSheet *tsChefMateConnection;
	TPanel *Panel2;
	TPanel *Panel4;
    TPanel *PanelChefMate4;
	TLabel *Label2;
	TGroupBox *GroupBox2;
	TLabel *Label4;
	TLabel *Label14;
	TLabel *Label15;
	TComboBox *cbMMRASEntry;
	TBitBtn *btnHelpMMRAS;
	TEdit *edMMRASUsername;
	TEdit *edMMRASPassword;
	TButton *btnDialMM;
	TButton *btnHangUpMM;
	TEdit *edMMDBName;
	TBitBtn *btnHelpMMDatabase;
	TButton *btnConnectMM;
	TStaticText *StaticText1;
	TCheckBox *chbMMRemote;
	TLabel *Label3;
	TEdit *edStockDBName;
	TButton *btnConnectStock;
	TBitBtn *btnHelpStockDatabase;
	TGroupBox *GroupBox3;
	TLabel *Label5;
	TLabel *Label16;
	TLabel *Label17;
	TComboBox *cbStockRASEntry;
	TBitBtn *btnHelpStockRAS;
	TEdit *edStockRASUsername;
	TEdit *edStockRASPassword;
	TButton *btnDialStock;
	TButton *btnHangUpStock;
	TStaticText *StaticText2;
	TCheckBox *chbStockRemote;
    TLabel *LabelChefMate3;
	TEdit *edChefMateDBName;
	TButton *btnConnectChefMate;
	TBitBtn *btnHelpChefMateDatabase;
	TGroupBox *GroupBoxChefMate3;
	TLabel *LabelChefMate5;
	TLabel *LabelChefMate16;
	TLabel *LabelChefMate17;
	TComboBox *cbChefMateRASEntry;
	TBitBtn *btnHelpChefMateRAS;
	TEdit *edChefMateRASUsername;
	TEdit *edChefMateRASPassword;
	TButton *btnDialChefMate;
	TButton *btnHangUpChefMate;
	TStaticText *StaticTextChefMate2;
	TCheckBox *chbChefMateRemote;
	TBitBtn *btnClose;
	TBitBtn *BitBtn2;
	TIBSQL *sqlSetStockCode;
	TIBTransaction *StockTransaction;
	TTabSheet *tsHeader;
	TPanel *Panel1;
	TLabel *Label1;
	TMemo *memCompanyDetails;
	TTabSheet *tsMenuSettings;
	TPanel *Panel6;
	TTabSheet *tsPayrollSettings;
	TPanel *Panel7;
	TButton *btnConnectCompany;
	TButton *btnAddRAS;
	TButton *Button1;
	TButton *btnSweepStock;
	TButton *btnSweepMenuMate;
	TButton *btnSweepChefMate;
	TEdit *edGSTNumber;
	TLabel *Label10;
	TLabel *Label11;
	TComboBox *cbMMDBServer;
	TButton *btnGetMMIP;
	TLabel *Label12;
	TComboBox *cbStockDBServer;
	TButton *btnGetStockIPClick;
	TLabel *LabelChefMate12;
	TComboBox *cbChefMateDBServer;
	TButton *btnGetChefMateIP;
	TCheckBox *chbHideStocktakeOnHand;
	TButton *Button2;
	TIBTransaction *MMTransaction;
	TIBSQL *sqlLocation;
	TComboBox *cbBarcodeSerialPort;
	TLabel *Label6;
	TEdit *edCommitPath;
	TLabel *Label7;
	TEdit *edMenuSavePath;
	TBitBtn *btnBrowseMMSavePath;
	TBitBtn *btnBrowseMMServerPath;
	TBitBtn *btnHelpMenuCommit;
	TBitBtn *btnHelpMenuSave;
	TLabel *Label8;
	TLabel *Label9;
	TEdit *edDefaultMenuGST;
	TEdit *edDefaultStockGST;
	TLabel *Label21;
	TCheckBox *chbSingleLocation;
	TComboBox *cbDefaultLocation;
	TLabel *Label27;
	TTabSheet *tsStocktakeSettings;
	TPanel *Panel3;
	TComboBox *cbStocktakeBarcodePos;
	TLabel *Label23;
	TEdit *edStocktakeExportFile;
	TLabel *Label25;
	TEdit *edStocktakePath;
	TLabel *Label22;
	TBitBtn *btnSelectStocktakePath;
	TEdit *edStocktakeImportFile;
	TLabel *Label26;
	TComboBox *cbStocktakeQtyPos;
	TLabel *Label24;
	TGroupBox *GroupBox1;
	TComboBox *cbPayrollSystem;
	TLabel *Label18;
	TEdit *edExportPath;
	TLabel *Label19;
	TBitBtn *btnSelectSysDestPath;
	TGroupBox *GroupBox4;
	TEdit *edMYOBJobCode;
	TLabel *Label13;
	TEdit *edMYOBPath;
	TLabel *Label20;
	TBitBtn *btnSelectMYOBPath;
	TComboBox *cbAccountSystem;
	TLabel *Label28;
	TButton *Button3;
	TEdit *edVIPCreditName;
	TLabel *Label29;
	TButton *btnSecurityRoles;
   TButton *btnTaxProfiles;
TButton *SyncStockLocation;
    TIBQuery *qrLocation;
    TIBTransaction *Transaction;
    TIBSQL *CheckStockLocationExist;
    TIBSQL *qrAddLocation;
    TIBSQL *sqlAddLK;
    TIBSQL *qrFindLocation;
    TRadioGroup *RBRoundingPoint;
    TCheckBox *chbAutoPrintReceiveTransferAudit;
    TCheckBox *chbAutoPrintStockTransferAudit;
    TCheckBox *chbSuppliersFromDefaultLocationsOnly;
    TLabel *Label30;
    TIBSQL *sqlGetNoOfPriceLevels;
    TIBSQL *sqlSetNoOfPriceLevels;
    TIBSQL *qrInsertInToPriceLevelItem;
    TIBSQL *qrMaxPriceLevels;
    TIBSQL *qrUpdatePriceLevels;
    TIBSQL *qrInsertInToPriceLevelItemSize;
    TIBSQL *qrSelectPriceLevelItemSize;
    TIBSQL *qrGenPriceLevelItemSizeKey;
    TIBSQL *qrMaxItemSizePriceLevels;
    TIBSQL *qrUpdateItemSizePriceLevels;
    TNumericEdit *edNoOfPriceLevels;
    TIBSQL *qrGetExistingPriceNames;

	void __fastcall chbMMRemoteClick(TObject *Sender);
	void __fastcall chbStockRemoteClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall btnAddRASClick(TObject *Sender);
	void __fastcall cbStockRASEntryChange(TObject *Sender);
	void __fastcall cbMMRASEntryChange(TObject *Sender);
//	void __fastcall OldDialUpActiveConnection(TObject *Sender, int Handle,
//          TRasConnStatusA &Status, AnsiString StatusString,
//          PChar EntryName, PChar DeviceType, PChar DeviceName);
//	void __fastcall OldDialUpEntryGet(TObject *Sender, _ARRAYOF char* EntryName,
//          const int EntryName_Size);
	void __fastcall btnHelpRASClick(TObject *Sender);
	void __fastcall btnHelpMMDatabaseClick(TObject *Sender);
	void __fastcall btnHelpStockDatabaseClick(TObject *Sender);
	void __fastcall btnHelpMenuCommitClick(TObject *Sender);
	void __fastcall btnHelpMenuSaveClick(TObject *Sender);
	void __fastcall btnBrowseMMServerPathClick(TObject *Sender);
	void __fastcall btnBrowseMMSavePathClick(TObject *Sender);
	void __fastcall btnConnectCompanyClick(TObject *Sender);
	void __fastcall btnDialStockClick(TObject *Sender);
	void __fastcall cbStockRASEntryClick(TObject *Sender);
	void __fastcall cbMMRASEntryClick(TObject *Sender);
	void __fastcall btnDialMMClick(TObject *Sender);
	void __fastcall btnHangUpMMClick(TObject *Sender);
	void __fastcall btnHangUpStockClick(TObject *Sender);

	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall StaticText2Click(TObject *Sender);
	void __fastcall StaticText1Click(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall tcCompanyNamesChange(TObject *Sender);
	void __fastcall tcCompanyNamesChanging(TObject *Sender,
          bool &AllowChange);
	void __fastcall btnConnectClick(TObject *Sender);


	void __fastcall btnSelectSysDestPathClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall pcSettingsChange(TObject *Sender);
	void __fastcall tcCompanyNamesDrawTab(TCustomTabControl *Control,
          int TabIndex, const TRect &Rect, bool Active);
	void __fastcall btnSweepMenuMateClick(TObject *Sender);
	void __fastcall btnSweepStockClick(TObject *Sender);
	void __fastcall cbDBServerDropDown(TObject *Sender);
	void __fastcall btnGetMMIPClick(TObject *Sender);
	void __fastcall btnGetStockIPClickClick(TObject *Sender);
	void __fastcall btnSelectMYOBPathClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall cbDefaultLocationDropDown(TObject *Sender);
	void __fastcall btnSelectStocktakePathClick(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall btnSecurityRolesClick(TObject *Sender);
    void __fastcall btnGetChefMateIPClick(TObject *Sender);
    void __fastcall btnSweepChefMateClick(TObject *Sender);
    void __fastcall cbChefMateRASEntryChange(TObject *Sender);
    void __fastcall btnHelpChefMateDatabaseClick(TObject *Sender);
    void __fastcall chbChefMateRemoteClick(TObject *Sender);
    void __fastcall btnDialChefMateClick(TObject *Sender);
    void __fastcall btnHangUpChefMateClick(TObject *Sender);
    void __fastcall cbChefMateRASEntryClick(TObject *Sender);
   void __fastcall btnTaxProfilesClick(TObject *Sender);
      void __fastcall SyncStockLocationClick(TObject *Sender);
    void __fastcall RBRoundingPointClick(TObject *Sender);
    void __fastcall edNoOfPriceLevelsKeyPress(TObject *Sender, char &Key);
   
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	BEGIN_MESSAGE_MAP
	  MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
	END_MESSAGE_MAP(TComponent)
private:	// User declarations
	bool RASAvailable;
	void LoadSettings();									// Load all companies
	void LoadSettings(TConnectionDetails *CompanyDetails);	// Load specific company's settings from registry
	void SaveSettings();
	void SaveSettings(TConnectionDetails *CurrentCompany);
	void SaveCurrentCompany();
	void LoadRASEntryLists(TConnectionDetails *CompanyDetails);

	TList *CompanyDetailsList;
	TConnectionDetails *GetCurrentCompany();

	TRetrieveNamesThread *RetrieveNamesThread;
	void __fastcall RetrieveNamesThreadTerminate(TObject *Sender);

	TStringList *ServersList;

	void GetInterbasePathComponents(AnsiString ConnectionString, AnsiString *Server, AnsiString *Path);
	void CreateIBConsoleSettings();
    void SaveNoOfPriceLevels();
    void UpdatePriceLevels(AnsiString isenabled, AnsiString alreadyEnabled, int currentPriceLevelsEnabled);
    int GetMaxPriceLevels(AnsiString isenabled);
    int GetMaxPriceLevelsInItemSize(AnsiString isenabled);
    void UpdateItemSizePriceLevels(AnsiString isenabled, AnsiString alreadyEnabled, int currentPriceLevelsEnabled);
    bool ShowNoOfPriceLevelMessage();
    AnsiString getSuffixValue(AnsiString priceLevelName);


public:		// User declarations
	__fastcall TfrmSetup(TComponent* Owner);
	bool FirstRun;
	bool InitialiseCompanies();
    int PrintStockTransfer;
    int PrintReceiveInvoice;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSetup *frmSetup;
//---------------------------------------------------------------------------
#endif

