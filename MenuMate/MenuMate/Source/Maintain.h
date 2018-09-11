// ---------------------------------------------------------------------------
// TODO 1 -o Michael -c Improvement: Broadcast User Change
#ifndef MaintainH
#define MaintainH

#define LINE_LBE_OFFSET 20
#define LINE_ICON_OFFSET 15

// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>

#include <stdio.h>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <Mask.hpp>
#include <Graphics.hpp>
#include <ImgList.hpp>

#include <FileCtrl.hpp>
#include "Enum.h"
#include "Comms.h"
#include <CheckLst.hpp>
#include <IBServices.hpp>
#include "MM_DBCore.h"
#include "Processing.h"
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"
#include "XeroSettings.h"

#include "VerticalSelect.h"
#define SelectionFormPtr std::auto_ptr<TfrmVerticalSelect>

// ---------------------------------------------------------------------------
class TfrmMaintain : public TZForm
{
	friend TZForm;
	__published: // IDE-managed Components

	TPanel *pnlMajor;
	TPanel *pnlMinor;
	TGroupBox *gbContacts;
	TTouchBtn *btnLoyalty;
	TTouchBtn *TouchBtn2;
	TGroupBox *GroupBox1;
	TTouchBtn *tbPHSInterface;
	TGroupBox *GroupBox2;
	TTouchBtn *tbtnDiscounts;
	TTouchBtn *tbtnHappyHour;
	TGroupBox *GroupBox3;
	TTouchBtn *btnMsgCredits;
	TTouchBtn *btnBroadcasts;
	TTouchBtn *btnMsgCancels;
	TTouchBtn *tbtnPaymentTypes;
	TGroupBox *GroupBox4;
	TTouchBtn *btnChangeTable;
	TTouchBtn *tbtnLocations;
	TTouchBtn *btnChangeRooms;
	TGroupBox *GroupBox5;
	TTouchBtn *tbtnMiscSettings;
	TTouchBtn *tbRunPrograms;
	TTouchBtn *tbtnPrinterMaintenance;
	TTouchBtn *tbtnPatronTypes;
	TTouchBtn *btnClose;
	TTouchBtn *tbtnSmartCards;
	TTouchBtn *TouchBtn1;
	TTouchBtn *tbIntaMate;
	TTouchBtn *tbtnPocketVouchers;
	TTouchBtn *tbtnEBet;
	TTouchBtn *tchbtnWebMate;
	TTouchBtn *btnGUI;
	TTouchBtn *btnSkimReasons;
	TTouchBtn *TouchBtn3;
	TTouchBtn *TouchBtnReservations;
	TTouchBtn *TouchBtnLoyaltyMate;
	TTouchBtn *TouchBtn4;
	TTouchBtn *tbtnDeals;
    TTouchBtn *tbtnTaxSettings;
    TTouchBtn *TouchBtnBarExchange;
    TTouchBtn *TouchBtnThorlink;
    TTouchBtn *TouchBtnFiscalStorage;
    //add by frn
    TTouchBtn *tchbtnDrinkCommand;
    TPanel *pnlButtons1;
    TTouchBtn *tbtnMaintenance;
    TTouchBtn *tbtnQuickMessages;
    TTouchBtn *tbtnInterfaces;
    TTouchBtn *TouchBtnClipInterface;
    TPageControl *Pages;
    TTabSheet *tsMaintenance;
    TTabSheet *tsQuickMessages;
    TTabSheet *tsInterfaces;
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel4;
    TPanel *Panel5;
    TPanel *Panel16;
    TPanel *Panel7;
    TTouchBtn *btnTierLevels;
    TTouchBtn *btnAccountingInterface;
    TTouchBtn *tbtnCashDrawer;
    TTouchBtn *tbtnCashDenomination;

    //*****************


	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall imgExitClick(TObject *Sender);
	void __fastcall btnTableNameClick(TObject *Sender);
	void __fastcall btnPrinterMaintenanceClick(TObject *Sender);
	void __fastcall btnUsersClick(TObject *Sender);
	void __fastcall btnLoyaltyClick(TObject *Sender);
	void __fastcall btnHappyHourClick(TObject *Sender);
	void __fastcall BitBtn2Click(TObject *Sender);
	void __fastcall BtnPaymentClick(TObject *Sender);
	void __fastcall btnChangeRoomsClick(TObject *Sender);
	void __fastcall btnDiscountsClick(TObject *Sender);
	void __fastcall tbtnLocationsClick(TObject *Sender);
	void __fastcall tbPHSInterfaceClick(TObject *Sender);
	void __fastcall TouchBtn3MouseClick(TObject *Sender);
	void __fastcall tbtnSmartCardsMouseClick(TObject *Sender);
	void __fastcall tbtnMiscSettingsMouseClick(TObject *Sender);
	void __fastcall TouchBtn1MouseClick(TObject *Sender);
	void __fastcall tbIntaMateMouseClick(TObject *Sender);
	void __fastcall tbtnPocketVouchersMouseClick(TObject *Sender);
	void __fastcall tbtnEBetMouseClick(TObject *Sender);
	void __fastcall tchbtnWebMateMouseClick(TObject *Sender);
	void __fastcall btnGUIMouseClick(TObject *Sender);
	void __fastcall btnSkimMouseclick(TObject *Sender);
	void __fastcall WriteOffMouseClick(TObject *Sender);
	void __fastcall TouchBtnReservationsMouseClick(TObject *Sender);
	void __fastcall TouchBtnLoyaltyMateMouseClick(TObject *Sender);
	void __fastcall CustomerOrderTypesClick(TObject *Sender);
	void __fastcall tbtnDealsMouseClick(TObject *Sender);
    void __fastcall tbtnTaxSettingsMouseClick(TObject *Sender);
    void __fastcall btnTierLevelsMouseClick(TObject *Sender);
 	void __fastcall tchbtnDrinkCommandMouseClick(TObject *Sender);
 	void __fastcall TouchBtnThorlinkClick(TObject *Sender);
    //add by frn
    void __fastcall tbMaintenanceClick(TObject *Sender);
    void __fastcall tbQuickMessagesClick(TObject *Sender);
    void __fastcall tbInterfacesClick(TObject *Sender);
    void RedrawButtons(TObject * Sender);
    void __fastcall TouchBtnBarExchangeMouseClick(TObject *Sender);
    void __fastcall btnAccountingInterfaceMouseClick(TObject *Sender);
    void __fastcall TouchBtnClipInterfaceMouseClick(TObject *Sender);
    TTouchBtn *TouchBtnRunRateBoard;
    void __fastcall TouchBtnRunRateBoardMouseClick(TObject *Sender);
    TTouchBtn *TouchBtnMemberSalesOnly;
   // void __fastcall TouchBtnMembersaleOnlyClick(TObject *Sender);
   
   TTouchBtn *touchBtnSecurity;
   void __fastcall TouchBtnSecurityMouseClick(TObject *Sender);
   void __fastcall TouchBtnFiscalMouseClick(TObject *Sender);

    //*****************

protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	BEGIN_MESSAGE_MAP MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)END_MESSAGE_MAP(TForm)

private:
	__fastcall TfrmMaintain(TComponent* Owner);

	AnsiString CustCardString;
	TfrmProcessing *DemoProcessing;
	bool DemoComplete;
	bool DemoOk;

    TLoginSuccess VerifyUserAuthorization();
    TVerticalSelection CreateSelectionItem(UnicodeString title, UnicodeString color, bool isCloseSelectionAllowed, int fontColor = 0, int action = 0);
	void __fastcall ServiceStatus(UnicodeString Msg, Database::TIBServiceType ServiceType);
	void __fastcall ServiceComplete(bool Success, UnicodeString ErrorMsg, Database::TIBServiceType ServiceType);
	void __fastcall ExportIntaMateData();
	void __fastcall PushBackItem( SelectionFormPtr inSelectionForm, UnicodeString inTitle, bool inWithAction, int inAction, TColor inColor, bool inCloseSelection );
	void __fastcall ExportIntaMate( int inAction );
	void __fastcall ExportIntaMateAll();
	void __fastcall ExportIntaMateVersion();
	void __fastcall ExportIntaMateDataProduct();
	void __fastcall ExportIntaMateDataPayments();
	void __fastcall ExportIntaMateDataCategoriesGroups();
	void __fastcall ExportIntaMateListStaff();
	void __fastcall ExportIntaMateDataMembers();
	void __fastcall ExportIntaMateDiscounts();
	void __fastcall ExportIntaMateFixed();
	void __fastcall ExportIntaMatePatronCounts();
	void __fastcall ExportIntaMateMemberGroups();
	void __fastcall ResetWebMate();
	void __fastcall RefreshWebMateBtnColor();
	void __fastcall RefreshReservationBtnColor();
	void __fastcall RefreshLoyaltyMateBtnColor();
	void __fastcall RefreshBarExchangeBtnColor();
    void __fastcall RefreshDrinkCommandButtonColor();
    void __fastcall RefreshThorlinkButtonColor();
	void __fastcall  SaveServerCheck();
   // void __fastcall RefreshMembersaleOnly();
	TLoginSuccess __fastcall CheckAccess(SecurityCheck AccessCheck);
    bool DisplayLoyaltyMateSettings(Database::TDBTransaction &DBTransaction);
	bool DisplayBarExchangeSettings(Database::TDBTransaction &DBTransaction);

    bool ShowDrinkCommandSettings(Database::TDBTransaction & DBTransaction);
    bool ShowThorlinkSettings(Database::TDBTransaction & DBTransaction);
	void CustomerOrderTypes(void);
    bool ServerEnable;
   	UnicodeString XeroMachineName;
	UnicodeString XeroFolderPath;
	UnicodeString XeroUserName;
	UnicodeString XeroPassword;
   	UnicodeString MYOBMachineName;
	UnicodeString MYOBFolderPath;
	UnicodeString MYOBUserName;
	UnicodeString MYOBPassword;
    bool _awaitingPaymentInXero;
	void InitXeroIntegrationConfig();
	void XeroSettings();
    void MYOBSettings();
	void SaveAccountingConfig(AccountingType accountingType);
    void SetupGLCodes();
    void __fastcall RefreshRunRateBoard();
    bool DisplayRunRateSettingsOnly(Database::TDBTransaction &DBTransaction);
    void ManageGiftCardValidations(Database::TDBTransaction &DBTransaction);
    void PeachTreeSettings();
    void EnablePanasonicIntegration();
    void SaveServerIp(Database::TDBTransaction &dbTransaction);
    void SaveEnabledState(Database::TDBTransaction &dbTransaction);
    void SelectPMSType();
    bool SetUpSiHot() ;
    bool SetUpPhoenix();
    bool SetUpOracle();
    bool SetUpMews();
public: // User declarations
};
#endif
