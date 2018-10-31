//---------------------------------------------------------------------------

#ifndef TabManagerGUIH
#define TabManagerGUIH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>

#include "Enum.h"
#include "ContactStaff.h"
#include "MMTouchKeyboard.h"
#include "AddTab.h"
#include "Rooms.h"
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"
#include "MMContactInfo.h"
#include "TouchGrid.h"
//---------------------------------------------------------------------------
class TfrmTabManager : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
    TPanel *pnlDisplay;
    TPanel *Panel2;
    TTouchBtn *btnSeatsUp;
    TTouchBtn *btnSeatsDown;
    TTouchBtn *btnShowNormalTabs;
    TTouchBtn *btnMemberTabs;
    TStaticText *lbePartyName;
    TStaticText *Label1;
    TTouchBtn *btnStaffTabs;
    TPanel *Panel3;
    TTouchBtn *btnClose;
    TTouchBtn *btnLockTab;
    TTouchBtn *btnManInvoice;
    TTouchBtn *btnTabCredit;
    TTouchBtn *btnAddNewTab;
    TTouchBtn *btnPermanent;
    TTouchBtn *btnRemoveTab;
    TTouchBtn *btnChangeDetails;
    TTouchBtn *btnPINTab;
    TTouchBtn *TouchButton1;
    TPanel *pnlTabs;
    TScrollBox *sbTabs;
    TLabel *lbeCredit;
    TLabel *lbeCreditLimit;
    TLabel *lbeLockedReason;
    TLabel *lbeIDType;
    TLabel *lbeIDNumber;
    TLabel *lbeIDExpire;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *lbeBalance;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TStaticText *lbeTabName;
    TLabel *Label2;
    TLabel *lbeTotalPoints;
    TPageControl *pgControl;
    TTabSheet *tsTabManager;
    TTabSheet *tsDiscounts;
    TGroupBox *GroupBox6;
    TPanel *Panel4;
    TTouchGrid *tgDiscounts;
    TPanel *Panel1;
    TTouchBtn *TouchBtn1;
    TTouchBtn *btnSubsidisedProfile;
    void __fastcall btnShowNormalTabsClick(TObject *Sender);
    void __fastcall btnMemberTabsClick(TObject *Sender);
    void __fastcall btnStaffTabsClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnCloseClick(TObject *Sender);
    void __fastcall pnlTabsResize(TObject *Sender);
    void __fastcall btnSeatsUpMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall btnSeatsDownMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall TouchButtonTabClick(TObject *Sender);
    void __fastcall btnAddNewTabClick(TObject *Sender);
    void __fastcall btnPermanentClick(TObject *Sender);
    void __fastcall btnLockTabClick(TObject *Sender);
    void __fastcall btnAddCreditToTabClick();
    void __fastcall btnRefundCreditToTabClick();
    void __fastcall btnChangeDetailsClick(TObject *Sender);
    void __fastcall btnSetCreditLimitClick();
    void __fastcall FormResize(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall btnPINTabClick(TObject *Sender);
    void __fastcall TouchButton1Click(TObject *Sender);
    void __fastcall btnManInvoiceClick(TObject *Sender);
    void __fastcall btnTabCreditClick(TObject *Sender);
    void __fastcall btnRemoveTabClick(TObject *Sender);
    void __fastcall tgDiscountsMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
    void __fastcall btnSubsidisedProfileMouseClick(TObject *Sender);
    void __fastcall TouchBtn1MouseClick(TObject *Sender);
private:	// User declarations
	__fastcall TfrmTabManager(TComponent* Owner,Database::TDBControl &inDBControl);
	
	void ShowTabsDetails();
   	TMMContactInfo TempUserInfo;

	Database::TDBControl &DBControl;

    void RedrawDiscounts();
    AnsiString CheckDiscountApplicability(int discountKey);
    void CustomizeForOnlineOrderingTabs(int SelectedTabs);
public:		// User declarations

    TTouchBtn *CurrentTabBtn;
    TMMDisplayMode CurrentDisplayMode;
    eTabType CurrentTabType;
    int SelectedTable, SelectedSeat, SelectedTab, SelectedRoomNo,LastTableNo;
    AnsiString CurrentTabContainerName,SelectedTabName, SelectedPartyName;
    void RefreshTabDetails();
    TMMContactInfo Info;
    std::auto_ptr <TStringList> DiscountList;
    int ScrollPosition;
    int MaxScrollPosition;
};
//---------------------------------------------------------------------------
#endif
