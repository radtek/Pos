//---------------------------------------------------------------------------

#ifndef PrinterMaintenanceH
#define PrinterMaintenanceH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <Db.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <IBCustomDataSet.hpp>
#include <IBQuery.hpp>
#include <DB.hpp>
#include "PrintingSections.h"
#include <Dialogs.hpp>
#include "SystemEvents.h"
#include "MM_DBCore.h"
#include "ZForm.h"
#include "GraphicPrinterSettings.h"
#include "TouchBtn.h"
#include "TouchControls.h"
#include "TouchGrid.h"
#include "TouchPages.h"
//---------------------------------------------------------------------------
enum TDeviceType {dtPalm, dtPC};
//---------------------------------------------------------------------------

class TPrinterModelSettings : public TObject
{
public:
	int PrinterModel;
	int NormalFontCPL;
	int BoldFontCPL;
	int CutterGap;
	int TopMargin;
	bool AlwaysUseFullCut;
	bool KanjiPrinter;
    int PrinterCodePage;
    UnicodeString PrinterInitCmd;
	UnicodeString PrinterCodePageOn;
	UnicodeString PrinterCodePageOff;
	char SingleLineChar;
	char DoubleLineChar;
	TEmulationMode Emulation;
   TGraphicPrinterSettings GraphicSettings;
};

class TSUCouseInfo
{
private:
public:
	TSUCouseInfo() : CourseKey(0), MenuKey(0),CourseName(""),CourseKitchenName("") {}
	int CourseKey;
	int MenuKey;
	UnicodeString CourseName;
	UnicodeString CourseKitchenName;
};

//---------------------------------------------------------------------------
class TfrmPrinterMaintenance : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPageControl *pgControl;
	TTabSheet *tsPrinting;
	TPanel *Panel52;
	TImage *imgPalm;
	TPanel *pnlPrinters;
	TPanel *pnlPrinterHeading;
	TPanel *pnlButtons;
	TTabSheet *tsKitchenFormat;
	TTabSheet *tsPrinters;
	TPanel *Panel5;
	TPanel *Panel11;
	TGroupBox *gbPrinterSpecs;
	TLabel *Label9;
	TListBox *lbPrinters;
	TPanel *Panel6;
	TGroupBox *GroupBox1;
	TColorDialog *ColorDialog;
	TTabSheet *tsServingCoursePrinting;
	TTabSheet *tsBreakdownCats;
	TPanel *Panel8;
	TTabSheet *tsDevicePrinterProfiles;
	TPanel *Panel28;
	TPanel *Panel14;
	TTouchGrid *tgridDeviceList;
	TPanel *Panel49;
	TPanel *Panel15;
	TTouchGrid *tgridProfileList;
	TPanel *Panel17;
	TTouchBtn *tbPrinterProfiles;
	TTouchGrid *tgridPrinterProfiles;
	TPanel *Panel1;
	TPanel *Panel2;
	TTouchGrid *tgridMenu;
	TPanel *Panel3;
	TPanel *Panel4;
	TTouchGrid *tgridCourse;
	TPanel *Panel18;
	TPanel *Panel20;
	TGroupBox *GroupBox5;
	TLabel *Label10;
	TLabel *Label15;
	TLabel *Label16;
	TLabel *Label18;
	TLabel *Label19;
	TLabel *Label21;
	TEdit *edSideBullet;
	TEdit *edOptionsBullet;
	TEdit *edNoteHeader;
	TEdit *edSidesHeader;
	TEdit *edMixHeader;
	TEdit *edPerSeatMultiplier;
	TPanel *Panel9;
	TImage *Image7;
	TImage *Image8;
	TPanel *Panel10;
	TListBox *lbPrintOpt;
	TPanel *Panel31;
	TRichEdit *Display1;
	TPanel *Panel7;
	TPanel *Panel32;
	TPanel *Panel33;
	TPanel *Panel34;
	TTouchBtn *tbtnClearAll;
	TTouchBtn *tbtnSelectAll;
	TPanel *Panel12;
	TPanel *Panel13;
	TTouchGrid *tgridSCPrinterProfiles;
	TPanel *Panel19;
	TPanel *Panel21;
	TTouchGrid *tgridServingCourses;
	TPanel *Panel22;
	TTouchBtn *tbtnServingCourseClear;
	TTouchBtn *tbtnSCSelectAll;
	TLabel *Label1;
	TEdit *edSetMenuItemBullet;
	TTabSheet *tsReceiptOptions;
	TTouchBtn *tbPrintTo;
	TTouchBtn *tbKitchenTemplateUp;
	TTouchBtn *tbKitchenTemplateDown;
	TTouchBtn *tbInsertPrintConfig;
	TTouchBtn *tbRemovePrintConfig;
	TTouchBtn *tbProperties;
   TTouchBtn *tbtnManuallyAddPrinter;
   TListBox *lbPrinterModels;
   TLabel *lbeProinterModel;
   TTouchBtn *btnRefreshPrinterModels;
   TTouchBtn *tbChefMateColour;
   TTouchBtn *tbChefMateIP;
   TTouchBtn *btnDelChefPrinter;
   TGroupBox *GroupBox3;
   TPanel *pnlExpertPrinterSpecs;
   TLabel *lbeNormalFontCPL;
   TLabel *lbeBoldFontCPL;
   TLabel *lbeTopMargin;
   TLabel *Label12;
   TLabel *Label13;
   TLabel *Label14;
   TLabel *Label17;
   TPanel *pnlNormalFontCPL;
	TTouchBtn *btnNormalFontDown;
   TPanel *pnlBoldFontCPL;
	TTouchBtn *btnBoldFontUp;
   TPanel *pnlCutterGap;
	TTouchBtn *btnCutterGapDown;
   TPanel *pnlTopMargin;
	TTouchBtn *btnTopMarginUp;
	TTouchBtn *btnNormalFontUp;
	TTouchBtn *btnCutterGapUp;
	TTouchBtn *btnTopMarginDown;
	TTouchBtn *btnBoldFontDown;
	TTouchBtn *btnKitchenTopMarginUp;
   TPanel *pnlKitchenTopMargin;
	TTouchBtn *btnKitchenTopMarginDown;
   TTouchBtn *btnTest;
   TRadioButton *rbFullCut;
   TRadioButton *rbPartialCut;
   TTouchBtn *tbtnPrinterProfile;
   TTouchPages *tpsReceipt;
   TTouchSheet *TouchSheet1;
   TPanel *Panel23;
   TPanel *Panel24;
   TLabel *Label3;
   TPanel *Panel25;
   TPanel *Panel29;
   TPanel *Panel30;
   TImage *Image1;
   TTouchBtn *btnLoadPrinterGraphic;
   TTouchBtn *btnRefresh;
   TTouchBtn *btnHeaderFooterSave;
   TTouchBtn *tbtnTestHeader;
   TTouchBtn *btnLoadHdrFtr;
   TTouchPages *tpReceipt;
   TTouchSheet *tpHeader;
   TRichEdit *memHeader;
   TTouchSheet *tpPreliminary;
   TRichEdit *memPHeader;
   TTouchSheet *tpFooter;
   TRichEdit *memFooter;
   TComboBox *cbReceiptPrinter;
   TTouchSheet *TouchSheet3;
   TPanel *Panel41;
   TPanel *Panel42;
   TPanel *Panel35;
   TLabel *Label2;
   TCheckBox *cbPrintNoteWithDiscount;
   TPanel *Panel43;
   TImage *Image4;
   TImage *Image9;
   TPanel *Panel44;
   TListBox *lbReceiptPrintOpt;
   TTouchBtn *tbtnReceiptTestPrint;
   TTouchBtn *tbReceiptTemplateUp;
   TTouchBtn *tbReceiptTemplateDown;
   TTouchBtn *tbReceiptInsert;
   TTouchBtn *tbReceiptRemove;
   TTouchBtn *tbReceiptProperties;
   TPanel *Panel36;
   TPanel *Panel37;
	TRichEdit *reReceiptDisplay;
   TPanel *Panel38;
   TPanel *Panel39;
   TPanel *Panel40;
   TTouchBtn *tbSelectReceiptPrinter;
   TTouchBtn *tbtnReceiptTemplates;
   TTouchBtn *btnDeleteDevice;
   TGroupBox *GroupBox2;
   TCheckBox *cbAlwaysPrintReceiptTenderedSales;
   TCheckBox *cbReceiptBarCodes;
   TCheckBox *cbDoublePrint;
   TCheckBox *cbAlwaysPrintCashSales;
   TCheckBox *cbDuplicateReceipts;
   TCheckBox *cbAPTROS;
   TCheckBox *cbAPSROS;
   TCheckBox *cbAPMOS;
   TCheckBox *cbAPTAROS;
   TCheckBox *cbAPCROS;
   TCheckBox *cbAPRROS;
   TCheckBox *cbAPIROS;
   TCheckBox *cbSaveAndPrint;
   TPanel *Panel26;
   TPanel *Panel27;
   TPanel *Panel45;
   TListBox *lbReceiptPrintConfig;
   TTouchBtn *tbtnOk;
   TTouchBtn *tbtnDeletePrinter;
   TTouchBtn *tbtnAddEditProfiles;
   TTouchBtn *tbtnIndentSectionLeft;
   TTouchBtn *tbtnIndentSectionRight;
   TPanel *Panel46;
   TPanel *Panel47;
   TTouchBtn *tbtnKitchenIndentRight;
   TTouchBtn *tbtnKitchenIndentLeft;
   TPanel *Panel16;
   TListBox *lbPrintConfig;
   TTouchBtn *tbtnKitchenTemplates;
	TTouchBtn *tbtnTestCharSets;
	TCheckBox *cbAPWOOS;
	TTouchGrid *tgPrnProfileBreakDwn;
	TTouchGrid *tgridBreakDwn;
    TTouchBtn *tbtnReceiptNumberLabel;
    TTouchBtn *TouchBtn1;
    TTouchBtn *tbtnReprintLabel;
    TCheckBox *cbPrintZeroTaxes;
    TCheckBox *cbPrintNoticeOnTransfer;
    //TTouchBtn *tbtnReceiptNumber;
    TCheckBox *cbCaptureRefundReference;
    TCheckBox *cbHideTaxInvoice;
    TCheckBox *cbExportReprintReceipt;
    TRichEdit *memCustomizeFooter;
    TGroupBox *GroupBoxCustom;
    TTabSheet *tsReceiptOptionsInfo;
    TTouchPages *tpsReceiptInfo;
    TTouchSheet *TouchSheetGeneralInfo;
    TPanel *PanelGeneralInfo;
    TTouchSheet *TouchSheetCustomInfo;
    TPanel *PanelCustomInfo;
    TLabel *LabelReceiptDigits;
    TPageControl *pgControl123;
    TCheckBox *cbSetFooter;
    TTouchBtn *tbtnReceiptNumber;
    TCheckBox *cbPrintVoid;
    TCheckBox *cbShowVoidNumber;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall btnDeleteDeviceClick(TObject *Sender);
	void __fastcall tbInsertPrintConfigMouseDown(TObject *Sender,
			 TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall tbRemovePrintConfigMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall tbKitchenTemplateUpClick(TObject *Sender);
	void __fastcall tbKitchenTemplateDownClick(TObject *Sender);
	void __fastcall tbPropertiesMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall tbPrintToClick(TObject *Sender);
	void __fastcall edPerSeatMultiplierMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall edNoteHeaderMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall edSidesHeaderMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall edMixHeaderMouseUp(TObject *Sender, TMouseButton Button,
			 TShiftState Shift, int X, int Y);
	void __fastcall edSideBulletMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall edOptionsBulletMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall lbPrintersClick(TObject *Sender);
	void __fastcall btnSelectPrinterTypeClick(TObject *Sender);
	void __fastcall btnNormalFontUpMouseDown(TObject *Sender,
			 TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall btnBoldFontUpMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall btnCutterGapUpMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall btnTopMarginUpMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall btnKitchenTopMarginUpMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall btnNormalFontDownMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall btnBoldFontDownMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall btnCutterGapDownMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall btnTopMarginDownMouseDown(TObject *Sender,
			 TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall btnKitchenTopMarginDownMouseDown(TObject *Sender,
			 TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall tbPrinterPropertiesClick(TObject *Sender);
	void __fastcall tbtnOkMouseClick(TObject *Sender);
	void __fastcall tgridDeviceListMouseClick(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
	void __fastcall tgridProfileListMouseClick(TObject *Sender,
          TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
	void __fastcall tgridMenuMouseClick(TObject *Sender, TMouseButton Button,
			 TShiftState Shift, TGridButton *GridButton);
	void __fastcall tgridCourseMouseClick(TObject *Sender,
          TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
	void __fastcall tgridPrinterProfilesMouseClick(TObject *Sender,
          TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
	void __fastcall tbtnSelectAllMouseClick(TObject *Sender);
	void __fastcall tbtnClearAllMouseClick(TObject *Sender);
	void __fastcall tgridSCPrinterProfilesMouseClick(TObject *Sender,
          TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
	void __fastcall tbtnSCSelectAllMouseClick(TObject *Sender);
	void __fastcall tgridServingCoursesMouseClick(TObject *Sender,
          TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
	void __fastcall tbtnAddEditProfileMouseClick(TObject *Sender);
	void __fastcall tbtnServingCourseClearMouseClick(TObject *Sender);
	void __fastcall edSetMenuItemBulletMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall cbDuplicateReceiptsClick(TObject *Sender);
	void __fastcall cbPrintNoteWithDiscountClick(TObject *Sender);
	void __fastcall cbReceiptPrinterChange(TObject *Sender);
	void __fastcall cbReceiptPrinterDropDown(TObject *Sender);
	void __fastcall btnLoadPrinterGraphicMouseClick(TObject *Sender);
	void __fastcall btnRefreshMouseClick(TObject *Sender);
	void __fastcall memFooterMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnHeaderFooterSaveMouseClick(TObject *Sender);
	void __fastcall tbtnTestHeaderMouseClick(TObject *Sender);
	void __fastcall btnLoadHdrFtrMouseClick(TObject *Sender);
	void __fastcall memHeaderChange(TObject *Sender);
	void __fastcall memHeaderMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall memPHeaderMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall cbAlwaysPrintReceiptTenderedSalesClick(TObject *Sender);
	void __fastcall cbAlwaysPrintCashSalesClick(TObject *Sender);
	void __fastcall cbReceiptBarCodesClick(TObject *Sender);
	void __fastcall cbDoublePrintClick(TObject *Sender);
	void __fastcall cbSaveAndPrintClick(TObject *Sender);
	void __fastcall cbAPTROSClick(TObject *Sender);
	void __fastcall cbAPSROSClick(TObject *Sender);
	void __fastcall cbAPMOSClick(TObject *Sender);
	void __fastcall cbAPTAROSClick(TObject *Sender);
	void __fastcall cbAPCROSClick(TObject *Sender);
	void __fastcall cbAPRROSClick(TObject *Sender);
	void __fastcall cbAPIROSClick(TObject *Sender);
	void __fastcall ManuallyAddPrinterMouseClick(TObject *Sender);
   void __fastcall btnRefreshPrinterModelsMouseClick(TObject *Sender);
   void __fastcall tbChefMateColourMouseClick(TObject *Sender);
   void __fastcall tbChefMateIPMouseClick(TObject *Sender);
   void __fastcall btnDelChefPrinterMouseClick(TObject *Sender);
   void __fastcall btnTestMouseClick(TObject *Sender);
   void __fastcall DeletePrinterMouseClick(TObject *Sender);
   void __fastcall AssignPrinterConfigMouseClick(TObject *Sender);
   void __fastcall lbPrinterModelsClick(TObject *Sender);
   void __fastcall rbFullCutMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall rbPartialCutMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall tbtnPrinterProfileMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall tbSelectReceiptPrinterMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall tbReceiptTemplateUpMouseClick(TObject *Sender);
   void __fastcall tbReceiptTemplateDownMouseClick(TObject *Sender);
   void __fastcall tbReceiptPropertiesMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall tbReceiptInsertMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall tbReceiptRemoveMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall tbtnReceiptTemplatesMouseClick(TObject *Sender);
   void __fastcall tbtnReceiptTestPrintMouseClick(TObject *Sender);
   void __fastcall tbtnIndentSectionLeftMouseClick(TObject *Sender);
   void __fastcall tbtnIndentSectionRightMouseClick(TObject *Sender);
   void __fastcall lbReceiptPrintConfigDrawItem(TWinControl *Control,
          int Index, TRect &Rect, TOwnerDrawState State);
   void __fastcall tbtnKitchenIndentLeftMouseClick(TObject *Sender);
   void __fastcall tbtnKitchenIndentRightMouseClick(TObject *Sender);
   void __fastcall tbtnKitchenTemplatesMouseClick(TObject *Sender);
   void __fastcall tbtnTestCharSetsMouseClick(TObject *Sender);
   void __fastcall cbAPWOOSClick(TObject *Sender);
	void __fastcall tgPrnProfileBreakDwnMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
	void __fastcall tbPrinterProfilesBrkDwnMouseClick(TObject *Sender);
	void __fastcall tgridBreakDwnMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
	void __fastcall tbtnSelectAllCatMouseClick(TObject *Sender);
	void __fastcall tbtnClearAllCatMouseClick(TObject *Sender);
    void __fastcall tbtnReceiptNumberLabelMouseClick(TObject *Sender);
    void __fastcall tbtnReprintLabelMouseClick(TObject *Sender);
    void __fastcall cbPrintZeroTaxesClick(TObject *Sender);
    void __fastcall cbPrintNoticeOnTransferClick(TObject *Sender);
    void __fastcall cbHideTaxInvoiceClick(TObject *Sender);
    void __fastcall cbCaptureRefundReferenceClick(TObject *Sender);
    void __fastcall cbExportReprintReceiptClick(TObject *Sender);
    void __fastcall cbSetFooterClick(TObject *Sender);
    void __fastcall memCustomizeFooterMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall tbtnReceiptNumberAutoRepeat(TObject *Sender);
    void __fastcall cbPrintVoidClick(TObject *Sender);
    void __fastcall cbShowVoidNumberClick(TObject *Sender);
    //void __fastcall cbExportReprintReceiptClick(TObject *Sender);
   // void __fastcall cbCaptureRefundReferenceClick(TObject *Sender);
    //void __fastcall cbHideTaxInvoiceClick(TObject *Sender);
    //void __fastcall PanelCustomInfoClick(TObject *Sender);
    //void __fastcall tbtnReceiptNumberMouseClick(TObject *Sender);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	 BEGIN_MESSAGE_MAP
			MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
	 END_MESSAGE_MAP(TForm)

private:	// User declarations
	__fastcall TfrmPrinterMaintenance(TComponent* Owner,Database::TDBControl &inDBControl);
	
	void UpdateDeviceDisplay();
	void UpdateProfileListDisplay();

	void UpdatePrinterDisplay();
	void UpdateSCPrinterDisplay();

	void UpdateMenuDisplay();
	void GetCourseInfo(Database::TDBTransaction &DBTransaction);
	void UpdateCourseDisplay();

	void UpdateServingCourseDisplay();
    void UpdateKitchenFormatProfileListDisplay();
   void UpdateReceiptFormatProfileListDisplay();
      
	void GetDistinctServingCourseList(TStringList *ServingCourses);
	void SaveCoursePrinterProfiles();
	void SaveCurrentServingCourseSettings();
	void SaveDevicePrinterProfiles();
    void SaveHeaderLabels();
	bool Modified;
	bool KitchenTemplateModified;
	bool ReceiptTemplateModified;

	TSystemEvents EventPhysicalPropertiesChanged;

	int CurrentMenuKey;
	int CurrentPrinterProfileKey;
	int CurrentReceiptPrinterKey;
	__int64 CurrentDeviceKey;

	typedef std::set<__int64> typProfileKeyList;
			// Device Key
	std::map<__int64,typProfileKeyList> DevicesPrinterProfile;

			// Course Key
	std::map<int,TSUCouseInfo> CourseInfo;

	void GetPrinterProfileCourses(int VirtualPrinterKey,std::map<int,std::vector<int> > &MenuCourseList);
	void GetPrinterProfileServingCourses(int VirtualPrinterKey,std::map<int,std::vector<int> > &PrinterServingCourse);
    void GetPrinterProfileBreakDown(int VirtualPrinterKey,  std::map <int, std::vector <int> > &List);

			// PrinterProfileKey //Menu Key // List of course Keys.
	std::map<int,std::map<int,std::vector<int> > > PrinterProfileCourses;
	std::map<int,std::vector<int> >PrinterProfileServingCourses;
    std::map<int,std::vector<int> >PrinterProfileBreakDownCategories;


	Database::TDBControl &DBControl;

	void MMEnumPrinters(UnicodeString PrinterContainer, std::map<UnicodeString,DWORD> &PrinterData );
    bool IsDBRegistered;
    void CheckVoidFooterSetting();
public:		// User declarations
	void DisplayKitchenPrinterInstructionSet();
    void DisplayReceiptPrinterInstructionSet();

	void DisplayKitchenTemplate();
    void DisplayReceiptTemplate();

	void SaveKitchenTemplate();
    void SaveReceiptTemplate();

	void __fastcall DrawKitchenDocket();
	void __fastcall DrawReceiptDocket();

	void ShowPrinterDetails();
	void UpdatePhysicalPrinterList(Database::TDBTransaction &DBTransaction);
	void DeletePrinter();
	void OnPhysicalPropertiesChanged(TSystemEvents *Sender);
	void OnNewMenus(TSystemEvents *Sender);
	void OnKitchenPropertyChanged(TSystemEvents *Sender);
	void OnReceiptPropertyChanged(TSystemEvents *Sender);
	void SaveBreakDownPrinterProfiles();
    void UpdatePrinterBreakDownDisplay();
    void UpdateBreakDownDisplay();

};

//---------------------------------------------------------------------------
#endif
