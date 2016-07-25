//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <CheckLst.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <DB.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <Dialogs.hpp>
#include <ImgList.hpp>
#include <Buttons.hpp>
#include <map>
#include "Registration.h"

//---------------------------------------------------------------------------
enum TProduct { prMenuMate=1, prPalmMate=2, prOffice=3 };
//---------------------------------------------------------------------------
/*class TRegistrationInfo
{
public:
	AnsiString	ProductName;
	AnsiString	TerminalName;
	AnsiString	CompanyName;
	AnsiString	HardwareKey;
	AnsiString	RegistrationKey;
};
//---------------------------------------------------------------------------
class TTreeData
{
public:
	AnsiString Caption;
	AnsiString Column1;
};*/
//---------------------------------------------------------------------------

//typedef std::map<AnsiString,TMachineInfo> TRegistrationArray;

class TfrmMain : public TForm
{
__published:	// IDE-managed Components
   TPrintDialog *PrintDialog;
	TPageControl *pcRegistration;
	TTabSheet *tsInformation;
	TPanel *Panel5;
	TLabel *lbFaxInfo4;
	TLabel *lbFaxInfo3;
	TLabel *lbFaxInfo5;
	TLabel *lbFaxInfo6;
	TLabel *lbFaxInfo8;
	TLabel *lbFaxInfo7;
	TLabel *lbFaxInfo9;
	TLabel *lbFaxInfo10;
	TLabel *lbFaxInfo11;
	TLabel *lbFaxInfo13;
	TLabel *lbFaxInfo12;
	TLabel *lbFaxInfo14;
	TLabel *lbFaxInfo16;
	TLabel *lbFaxInfo15;
	TLabel *Label14;
	TLabel *Label15;
	TLabel *Label16;
	TTabSheet *tsIndividualLicences;
	TTreeView *tvLicences;
	TImageList *imlTreeImages;
	TButton *btnUnregister;
	TTabSheet *tsMultiLicences;
	TMemo *memCustomerKeys;
	TMemo *memRegKeys;
	TLabel *Label1;
	TLabel *Label2;
	TSaveDialog *SaveDialog;
	TPanel *Panel2;
	TButton *btnSave;
	TButton *btnPrint;
	TBitBtn *btnRefresh;
	TBitBtn *btnApply;
	TTabSheet *tsConnection;
	TGroupBox *GroupBox2;
	TComboBox *cbOfficeConnections;
	TLabel *Label3;
	TEdit *edConnectionName;
	TLabel *Label4;
	TRadioButton *rbUseMenuMateSettings;
	TRadioButton *rbUseOfficeSettings;
	TButton *btnRemoveKey;
	TButton *btnEnterKey;
	TTabSheet *TabSheet1;
	TPanel *Panel3;
	TLabel *Label22;
	TCheckBox *chbKitchenModule;
	TCheckBox *chbRoomsModule;
	TPanel *Panel4;
	TLabel *Label23;
	TLabel *Label24;
	TLabel *Label25;
	TLabel *Label26;
	TLabel *Label27;
	TLabel *Label28;
	TLabel *Label30;
	TLabel *Label31;
	TLabel *Label35;
	TLabel *Label36;
	TLabel *Label37;
	TCheckBox *chbTimeTracking;
	TCheckBox *cbMembership;
	TCheckBox *cbMYOB;
   TCheckBox *cbIntaMate;
	TCheckBox *cbIntergratedEftpos;
	TCheckBox *cbPhoenixHS;
   TPanel *Panel1;
   TLabel *Label46;
   TLabel *Label9;
   TLabel *Label44;
   TLabel *Label12;
   TLabel *Label13;
   TLabel *Label6;
   TComboBox *cbCustomerName;
   TEdit *edPalmLicences;
   TEdit *edPOSLicences;
   TEdit *edOffice;
	void __fastcall cbCustomerNameChange(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall edPOSLicencesChange(TObject *Sender);
   void __fastcall btnUnregisterClick(TObject *Sender);
   void __fastcall btnPrintClick(TObject *Sender);
   void __fastcall btnSaveClick(TObject *Sender);
   void __fastcall edPalmLicencesChange(TObject *Sender);
	void __fastcall pcRegistrationChange(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall tvLicencesDblClick(TObject *Sender);
	void __fastcall pcRegistrationChanging(TObject *Sender,
          bool &AllowChange);
	void __fastcall tvLicencesDeletion(TObject *Sender, TTreeNode *Node);
	void __fastcall btnRefreshClick(TObject *Sender);
	void __fastcall btnApplyClick(TObject *Sender);
	void __fastcall chbKitchenModuleClick(TObject *Sender);
	void __fastcall rbUseMenuMateSettingsClick(TObject *Sender);
	void __fastcall rbUseOfficeSettingsClick(TObject *Sender);
	void __fastcall cbOfficeConnectionsChange(TObject *Sender);
	void __fastcall btnRemoveKeyClick(TObject *Sender);
	void __fastcall btnEnterKeyClick(TObject *Sender);
   void __fastcall cbRoomsClick(TObject *Sender);
	void __fastcall chbTimeTrackingClick(TObject *Sender);
	void __fastcall cbMYOBClick(TObject *Sender);
	void __fastcall cbMembershipClick(TObject *Sender);
	void __fastcall edOfficeChange(TObject *Sender);
	void __fastcall cbIntaMateClick(TObject *Sender);
	void __fastcall cbIntergratedEftposClick(TObject *Sender);
	void __fastcall cbExpiryClick(TObject *Sender);
	void __fastcall cbPhoenixHSClick(TObject *Sender);
private:	// User declarations
	void GetKeys(TStrings *Strings, bool Friendly=false);
	int MenuMateCount, OfficeCount;
public:		// User declarations
	Database::TDBControl DBControl;
	bool OpenDatabase(AnsiString DBName);
	AnsiString ProductToRegister;
	AnsiString CompanyName;
	int POSLicences;
	int PalmLicences;
	int MenuMateModules;
	int OfficeLicences;


//   TRegistrationArray Registration;
//   TStringList *CompanyNames;

	__fastcall TfrmMain(TComponent* Owner);
//   void RefreshNameKeyMap();
//   void RegenCustomerKeys();
//   void CheckRegistrationKeys();
//   void RedrawDatabaseGrid();
//   void RedrawRemoveListBox();
//	virtual __fastcall ~TfrmMain();
//	void FormatRegInfo();


};


//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
