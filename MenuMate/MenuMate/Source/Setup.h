//---------------------------------------------------------------------------
#ifndef SetupH
#define SetupH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <DBCtrls.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Grids.hpp>
#include <Mask.hpp>

#include "enum.h"
//#include "BrowseDr.hpp"
#include <IBServices.hpp>
#include <Dialogs.hpp>
#include <CheckLst.hpp>
#include "MM_DBCore.h"
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"
#include "ManagerVariable.h"
//---------------------------------------------------------------------------
class TfrmSetup : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
   TPageControl *PageControl;
   TTabSheet *tsPrinters;
   TPanel *Panel6;
   TTabSheet *tsApplications;
	TComboBox *cbSecuritySerialPort;
	TLabel *Label20;
	TComboBox *cbBarCodePort;
	TLabel *Label8;
	TComboBox *cbProxReader;
	TLabel *Label4;
	TComboBox *cbPoleDisplayMode;
	TComboBox *cbPoleDisplay;
	TLabel *Label11;
	TCheckBox *cbUseHighChars;
	TComboBox *cbCashDrawPort;
	TLabel *Label1;
	TEdit *edSerialKickCharsCount;
	TUpDown *UpDown1;
	TTouchBtn *tbtnReconfigMMDB;
	TTouchBtn *tbtnReconfigMemDB;
	TTouchBtn *tbtnReconfigStockDB;
	TLabel *lbeMMDBIP;
	TLabel *lbeMMDBPath;
	TLabel *lbeMemDBIP;
	TLabel *lbeMemDBPath;
	TLabel *lbeStockDBIP;
	TLabel *lbeStockDBPath;
	TTouchBtn *tbtnReconfigSNTPIP;
	TLabel *lbeSNTPIP;
	TTouchBtn *tbtnIPSettingsRefresh;
	TGroupBox *gbPoleDisplay;
	TGroupBox *gbNewbookDisplay;
	TEdit *edTopLine;
	TEdit *edBottomLine;
	TGroupBox *GroupBox1;
	TLabel *Label2;
	TTouchBtn *btnSelectStockMasterPath;
	TEdit *edStockMasterExport;
	TTouchBtn *tbtnStatusReport;
	TTouchBtn *btnDebuggerTest;
   TButton *Configure;
   TButton *Button1;
   TComboBox *cbProxFormat;
   TLabel *Label3;
   TComboBox *cbWeighScales;
   TButton *btnWeighScalesConfig;
   TButton *tbtnProxSerialConfig;
	TButton *btnTestScales;
	TButton *Button2;
	TRadioGroup *rgMembershipType;
	TComboBox *cbScalesType;
    	TComboBox *cbNewbookType;
    TTabSheet *tsExport;
    TGroupBox *gbMall;
    TComboBox *cbMallLoc;
    TEdit *edTenantNo;
    TLabel *Label6;
    TEdit *edMallPath;
    TEdit *edClassCode;
    TEdit *edTradeCode;
    TEdit *edOutletCode;
    TLabel *lbTenantNo;
    TLabel *lbFileLocation;
    TLabel *lbClassCode;
    TLabel *lbTradeCode;
    TLabel *lbOutletNumber;
    TLabel *lbMallType;
    TLabel *lbBranchCode;
    TLabel *lbTerminalNumber;
    TLabel *lbSerialNumber;
    TEdit *edBranchCode;
    TEdit *edTerminalNo;
    TEdit *edSerialNo;
    TPanel *pnlButtons;
    TTouchBtn *btnClose;
    TLabel *lbVersion;

    TComboBox *cbBarcodeFormat;
    TLabel *lblBarcodeFormat;
    TLabel *lblNewBook;
    TCheckBox *cbShowCustomerDisplay;
    TTouchBtn *btnResendReport;
    TLabel *lbFTPPath;
    TLabel *lbFTPUserName;
    TLabel *lbFTPPassword;
    TEdit *edFTPServer;
    TEdit *edFTPPath;
    TEdit *edFTPUserName;
    TEdit *edFTPPassword;
    TLabel *lbFTPServer;
    TTouchBtn *btnRegenReport;
    TTouchBtn *btnAssignSalesType;
    TEdit *edConsolidatedDBPaths;
    TCheckBox *cbEnableConsolidatedRep;
   void __fastcall imgCloseClick(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall PageControlChanging(TObject *Sender, bool &AllowChange);
   void __fastcall cbCashDrawPortChange(TObject *Sender);
	void __fastcall btnSelectStockMasterPathClick(TObject *Sender);
	void __fastcall PageControlChange(TObject *Sender);
	void __fastcall btnDemoModeClick(TObject *Sender);
	void __fastcall cbPoleDisplayChange(TObject *Sender);
   void __fastcall edTopLineMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall edBottomLineMouseUp(TObject *Sender,
			 TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall cbUseHighCharsClick(TObject *Sender);
   void __fastcall UpDown1Click(TObject *Sender, TUDBtnType Button);
   void __fastcall edStockMasterExportMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall cbPoleDisplayModeChange(TObject *Sender);
	void __fastcall cbProxReaderChange(TObject *Sender);
	void __fastcall cbBarCodePortChange(TObject *Sender);
	void __fastcall cbSecuritySerialPortChange(TObject *Sender);
	void __fastcall tbtnIPSettingsRefreshMouseClick(TObject *Sender);
	void __fastcall tbtnReconfigMMDBMouseClick(TObject *Sender);
	void __fastcall tbtnReconfigMemDBMouseClick(TObject *Sender);
	void __fastcall tbtnReconfigStockDBMouseClick(TObject *Sender);
	void __fastcall tbtnReconfigSNTPIPMouseClick(TObject *Sender);
	void __fastcall tbtnStatusReportMouseClick(TObject *Sender);
	void __fastcall btnDebuggerTestMouseClick(TObject *Sender);
   void __fastcall ConfigureClick(TObject *Sender);
   void __fastcall Button1Click(TObject *Sender);
   void __fastcall cbProxFormatChange(TObject *Sender);
   void __fastcall cbWeighScalesChange(TObject *Sender);
   void __fastcall btnWeighScalesConfigClick(TObject *Sender);
   void __fastcall tbtnProxSerialConfigClick(TObject *Sender);
	void __fastcall btnTestScalesClick(TObject *Sender);
	void __fastcall ConfigPoleDisplayClick(TObject *Sender);
	void __fastcall rgMembershipTypeClick(TObject *Sender);
	void __fastcall cbScalesTypeChange(TObject *Sender);
    void __fastcall cbMallLocChange(TObject *Sender);
    void __fastcall cbNewbookTypeChange(TObject *Sender);
    void __fastcall edTenantNoMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
    void __fastcall edMallPathMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
    void __fastcall edClassCodeMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
    void __fastcall edTradeCodeMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
    void __fastcall edOutletCodeMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
    void __fastcall edBranchCodeMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
    void __fastcall edTerminalNoMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
    void __fastcall edSerialNoMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
    void __fastcall cbBarcodeFormatChange(TObject *Sender);
    void __fastcall cbShowCustomerDisplayClick(TObject *Sender);
    void __fastcall btnResendReportMouseClick(TObject *Sender);
    void __fastcall edFTPServerMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
    void __fastcall edFTPPathMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
    void __fastcall edFTPUserNameMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
    void __fastcall edFTPPasswordMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);

      TTouchBtn *tbtnReconfigEmailHost;       //MM-4104
      TTouchBtn *tbtnReconfigEmailId;
      TTouchBtn *tbtnReconfigEmailPassword;
      TTouchBtn *tbtnReconfigEmailHostPort;

      void __fastcall tbtnReconfigEmailHostMouseClick(TObject *Sender);
      void __fastcall tbtnReconfigEmailIdMouseClick(TObject *Sender);
      void __fastcall tbtnReconfigEmailPasswordMouseClick(TObject *Sender);
      void __fastcall tbtnReconfigEmailHostPortMouseClick(TObject *Sender);

      UnicodeString DisplayInputForm(UnicodeString initialValue, AnsiString caption);

    void __fastcall btnRegenerateReportMouseClick(TObject *Sender);
    void __fastcall btnAssignSalesTypeMouseClick(TObject *Sender);
    void __fastcall edConsolidatedDBPathsMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall cbEnableConsolidatedRepClick(TObject *Sender);


      //MM-4104


private:	// User declarations
	__fastcall TfrmSetup(TComponent* Owner,Database::TDBControl &inIBDatabase);
    void refreshCustomerDisplay();
    void loadCashDrawerPorts();
    void setCashDrawerPort();

	Database::TDBControl &IBDatabase;

    bool invalid_id;//MM-4104

public:		// User declarations

	//Undo Table Move Information --------------------------------------------------
	bool RestaurantLayoutsChanged;
	void UpdateLists();

    //Mall Export Functions
    void InitializeMallExport();
    void ChangeMallType();
    void SetupTenantNumber();
    void SetupMallPath();
    void SetupClassCode();
    void SetupTradeCode();
    void SetupOutletCode();
    void SetupBranchCode();
    void SetupTerminalNumber();
    void SetupSerialNumber();
    void SetupFTPServer();
    void SetupFTPPath();
    void SetupFTPUserName();
    void SetupFTPPassword();
    void SetupConsolidatedPaths();

    void NoSelectedMall();
    void EnableSMComponents();
    void EnableRobinsonComponents();
    void EnableAyalaComponents();
    void EnablePowerPlantComponents();
    void EnableCapitalandComponents();
    void EnableAlphalandComponents();
    void EnableMegaworldComponents();
    void EnableShangrilaComponents();
    void EnableDLFMallComponents();
    void EnableFederalLandComponents();
    void EnableFieldComponent(bool isMallPathSet, bool isTerminalNoSet, bool isClassCodeSet, bool isTradeCodeSet,
                              bool isOutletCodeSet, bool isSerialNoSet, bool isTenantNoSet, bool isBranchCodeSet,
                              bool isFTPServerSet, bool isFTPPathSet, bool isFTPUserNameSet, bool isFTPPasswordSet,
                              bool isConsolidatedRepSet, bool isEnableConsolidatedRepSet);
    void ClearAllComponentValue();
    void SaveCompValueinDBStr(vmVariables vmVar, UnicodeString CompName);
    void SaveCompValueinDBInt(vmVariables vmVar, int CompName);
    void SaveCompValueinDBBool(vmVariables vmVar, bool CompName);
    void ClearCompValueinDBStr(vmVariables vmVar, UnicodeString CompName, TEdit *CompfrmName);
    void ClearCompValueinDBInt(vmVariables vmVar, int CompName, TEdit *CompfrmName);
    void SaveCompValueinDBStrUnique(vmVariables vmVar, UnicodeString CompName);
    void ClearCompValueinDBStrUnique(vmVariables vmVar, UnicodeString CompName, TEdit *CompfrmName);
    void SaveCompValueinDBIntUnique(vmVariables vmVar, int CompName);
    void ClearCompValueinDBIntUnique(vmVariables vmVar, int CompName, TEdit *CompfrmName);
    void SaveCompValueinDBBoolUnique(vmVariables vmVar, bool CompName);
    void ClearCompValueinDBBoolUnique(vmVariables vmVar, bool CompName, TCheckBox *CompfrmName);
    void SetSpecificMall(int MallIdx);
    void CheckMallPath();
    void SaveMESettings();

    UnicodeString RenameBranchCode();
    UnicodeString RenameSerialNumber();
    UnicodeString RenameTenantNumber();
};
//---------------------------------------------------------------------------
#endif
