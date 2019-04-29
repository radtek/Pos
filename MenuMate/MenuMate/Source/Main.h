//---------------------------------------------------------------------------
#ifndef MainH
#define MainH

#include "enum.h"
//---------------------------------------------------------------------------
#include <Controls.hpp>
#include <Classes.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <DBCtrls.hpp>
#include <Mask.hpp>
#include <Graphics.hpp>

#include <OleCtrls.hpp>
#include <jpeg.hpp>
#include "ZForm.h"
#include "SHDocVw_OCX.h"
#include "TouchControls.h"
#include "TouchGrid.h"
#include "pngimage.hpp"
#include "IdBaseComponent.hpp"
#include "IdComponent.hpp"
#include "IdIPWatch.hpp"

#include "ProxyMateManager.h"
#include "MMProxy.h"
#include "MMFloorPlanConnectorServer.h"
#include "ManagerVariable.h"

//---------------------------------------------------------------------------

class TMMCustomerDisplayManager;

class TfrmMain : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
    TTimer *tiNewMenuCheck;
    TMainMenu *DemoMenu;
    TMenuItem *DEMOMODE1;
    TTimer *tiCardSwipe;
    TLabel *lbeVersion;
    TTimer *tiMenuUpdateReq;
    TLabel *lbTitle;
    TImage *Image1;
    TMemo *MemInfo;
    TTouchGrid *tgridMenu;
    TLabel *lbeRegistration;
    TTimer *tiShowMsg;
    TTimer *tiFailedXeroInvoices;
    TTimer *tiMallExportTiming;
    TTimer *tiCheckZedRead;
    TTimer *tiBarStock;
    TTimer *tiBarStockturnover;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnAnalysisClick(TObject *Sender);
    void __fastcall btnSetupClick(TObject *Sender);
    void __fastcall btnRemindersClick(TObject *Sender);
    void __fastcall btnExitClick(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall btnMaintenanceClick(TObject *Sender);
    void __fastcall btnPOSClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormPaint(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall tiNewMenuCheckTimer(TObject *Sender);
    void __fastcall DEMOMODE1DrawItem(TObject *Sender, TCanvas *ACanvas,
          TRect &ARect, bool Selected);
    void __fastcall DEMOMODE1MeasureItem(TObject *Sender, TCanvas *ACanvas,
             int &Width, int &Height);
    void __fastcall tiCardSwipeTimer(TObject *Sender);
    void __fastcall tiMenuUpdateReqTimer(TObject *Sender);
    void __fastcall tgButtonsMouseClick(TObject *Sender,
          TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
    void __fastcall ImageTitleClick(TObject *Sender);
    void __fastcall tiShowMsgTimer(TObject *Sender);
    void __fastcall tiFailedXeroInvoicesTimer(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall FormDeactivate(TObject *Sender);
      void __fastcall tiBarStockTimer(TObject *Sender);
    void __fastcall tiBarStockturnoverTimer(TObject *Sender);

protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	BEGIN_MESSAGE_MAP
	  MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
	END_MESSAGE_MAP(TComponent)
private:	// User declarations
    __fastcall TfrmMain(TComponent* Owner);
    AnsiString CardSwipeString;
    AnsiString FMenusToImportFolder;
    void __fastcall updateHTMLDisplay(Database::TDBTransaction &DBTransaction);
    void __fastcall Quit();
    bool MessagesToBroadcast();
    void BroadcastMessage();
    void SyncCompanyDetails();
    void InitXeroIntegration();
    bool FailedXeroInvoivesToSend();
    void SendFailedXeroInvoices();
    void ResetFailedXeroInvoiceTimerInterval( unsigned inInvoiceCount );
    TProxyMateManager *proxyMateManager;
    void initProxyMateManager();
    bool proxyMateWillHandle( tagMSG &inMsg );
    TMMFloorPlanConnectorServer *fpConnector;
    bool fpConnectorUp;
    bool fpOpen;
    void OpenFPConnector();  // Open ProxyMate's FloorPlan connector
    void CloseFPConnector(); // Close ProxyMate's FloorPlan connector
    void showFloorPlan();
    void showNewFloorPlan( TMMFloorPlanConnectorServer* inConnector );
    void showOldFloorPlan();
    void __fastcall mmAppConnectorOpenConnectorAck( System::TObject* Sender, int inACK, TMMConnectorTypes inConnectorType, TMMConnectorServer* inConnector  );
    void __fastcall fpClientUp(      TObject* Sender );
    void __fastcall fpClientDown(    TObject* Sender );
    void __fastcall fpUpdatedTables( TObject* Sender, TLocationStatus* inUpdatedTables, int inCount );
    //::::::::::::::::::::::::::
    // Customer Display
    //::::::::::::::::::::::::::
    TMMCustomerDisplayManager *customer_display_manager;
    void openCustomerDisplayServer();
    void closeCustomerDisplayServer();
    AnsiString FMenusStockExchange;
    bool MenusToImportFolderExists();
    void CreateMenusToImportFolder();
    bool MenusToImport();
    void ImportMenus();
    void ImportMembers();
    bool MenusToStockExchange();
    bool MenusToStockExchangeFolderExists();
    void ImportStockExchange();
    void CreateMenusToExchangeStockFolder();
    void ReFormatIpToUrl();
    void EnableOnlineOrdering();
    void DisableOnlineOrdering();
    void UnloadSignalR();
    bool SyncOnlineOrderingDetails();
    void WriteDBPathAndIPToFile();
    void UnsetOrderingDetails();
    void SetUpAustriaFiscal();
    void VersionCompare();
    void  CreateWaiterAppPaymentType();
    bool CanCreateWaiterAppPaymentType(Database::TDBTransaction &DBTransaction);
public:		// User declarations
    void __fastcall AppMessage(tagMSG &Msg, bool &Handled);
    void __fastcall IdleHandler(TObject *Sender, bool &Done);
    // Used to determin weather access is allowed and that DB is up and running.
    bool Access;
    // Did the user gain entry.
    bool UserAccess;
    // Does the user have full access rights.
    bool FullAccess;
    void SetActive();
    void SetExclusive();
    // Used to store that fact that we have just booted.
    // Thus code is only run once in the FormShow Event.
    bool JstBooted;
    void RemoveOldLogs();
    bool RequestClose;
    virtual __fastcall ~TfrmMain();
    void __fastcall LaunchProcess();
    // For Mall Export Methods
    void ZForced();
    void EnablePOSBtn( bool inEnabled );
    void InitializeExportTimings(bool FirstMallSet);
    void SaveBoolVariable(vmVariables vmVar, bool CompName);
    bool FirstMallSet;
    void EnablePOSForSpecifiedMallSchedule();
    void SaveItemPriceIncludeTaxToDatabase(vmVariables vmVariable, bool value);
};

//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
