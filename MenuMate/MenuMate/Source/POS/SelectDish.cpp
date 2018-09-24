// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <algorithm>
#include <cassert>
#include "SelectTable2.h"
#include <mshtml.h>
#include <oleacc.h>
#include <math.h>
#include <inifiles.hpp>
#include <DateUtils.hpp>
#include <mshtml.h>
#include <oleacc.h>
#include <math.h>
#include <i_item_manager.hh>
#include <cassert>
#include "SelectDish.h"
#include "GlobalSettings.h"

#include "DBSaleTimes.h"
#include "DBTab.h"
#include "DBTierLevel.h"
#include "MMData.h"
#include "DBWebUtil.h"
#include "DBTables.h"
#include "DBSecurity.h"
#include "DBOrder.h"
#include "POSMain.h"

#include "Enum.h"
#include "Comms.h"
#include "Secure.h"
#include "ContactStaff.h"
#include "Message.h"
#include "ParkedSales.h"
#include "Contact.h"
#include "HoldSend.h"
#include "Invoice.h"
#include "Rooms.h"
#include "Weight.h"
#include "ScaleControler.h"
#include "EftPos.h"
#include "Membership.h"
#include "CardSwipe.h"
#include "SmartcardPreloader.h"
#include "SmartCardAPI.h"
#include "XReport.h"
#include "Printing.h"
#include "ReqPrintJob.h"
#include "Printout.h"
#include "Reports.h"
#include "ReportManager.h"
#include "DropDown.h"
#include "DropDownVar.h"
#include "ReportDisplay.h"
#include "ReportDisplayNav.h"
#include "GUIDiscount.h"
#include "GUIScale.h"
#include "Functions.h"
#include "WriteOff.h"
#include "EnableFloorPlan.h"
#include "DocketManager.h"
#include "VerticalSelect.h"
#include "AddTab.h"
//#include "StringTableRes.h"
#include "StringTableVariables.h"
#include "ListCourse.h"
#include "ItemSize.h"
#include "ItemRecipe.h"
#include "ItemSizeCategory.h"
#include "ForcedOptions.h"
#include "ForcedSides.h"
#include "SCDPWDChecker.h"
#include "ListSecurityRefContainer.h"
#include "OrderUtils.h"
#include "SelectCurrentMenu.h"
#include "BillGroup.h"
#include "Maintain.h"
#include "SelectTable.h"
#include "SelectGlass.h"
#include "SelectReceipt.h"
#include "GetMoney.h"
#include "SelectItem.h"
#include "Processing.h"
#include "SelectActiveMenus.h"
#include "ProductSearch.h"
#include "EditCustomer.h"
#include "SelectMember.h"
#include "SelectDiscounts.h"
#include "Analysis.h"
#include "ShowPrintout.h"
#include "PayType.h"
#include "ReportUtilities.h"
#include "CaptNamePhone.h"
#include "TransactionAuditScreen.h"
#include "SelectSizesAsList.h"
#include "SelectPaymentTypeAndValue.h"
#include "SelectDateAndTime.h"
#include "SelectSaveOption.h"
#include "DelayedPaymentTabs.h"
#include "DrinkCommandData.h"
#include "ManagerStock.h"
#include "ManagerVariable.h"
#include "FreebieManager.h"
#include "TableManager.h"
#include "RunManager.h"
#include "ReceiptManager.h"
#include "ManagerDiscount.h"
#include "ManagerThirdParty.h"
#include "ManagerPatron.h"
#include "ProxyMateManager.h"
#include "ThemeManager.h"
#include "ManagerWebDispatch.h"
#include "ManagerReports.h"
#include "ManagerFloat.h"
#include "DealManager.h"
#include "ReportUtilities.h"
#include "InitializeDCSession.h"
#include "ManagerCloudSync.h"
#include "ManagerLoyaltyVoucher.h"
#include "MessageManager.h"
#include "ManagerEJournal.h"
#include "ManagerClippIntegration.h"
#include "ManagerHappyHour.h"
#include "ManagerDelayedPayment.h"
#include "MallExportManager.h"
#include "MMTouchKeyboard.h"
#include "MMMessageBox.h"
#include "MMTouchNumpad.h"
#include "PaySubsUtility.h"
#include "ManagerReportExport.h"
#include "GuestList.h"
#include "FiscalPrinterAdapter.h"
#include "SCDPatronUtility.h"
#include "SaveLogs.h"
// ---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma link "TouchNumpad"
#pragma link "TouchNumpad"
#pragma link "SHDocVw_OCX"
#pragma link "TouchPages"
#pragma link "touchbtn"
#pragma link "touchcontrols"
#pragma link "touchgrid"
#pragma link "touchnumpad"
#pragma link "touchpages"
#pragma link "SHDocVw_OCX"
#pragma link "SHDocVw_OCX"
#pragma resource "*.dfm"
TfrmSelectDish *frmSelectDish;

// extern bool operator < (TListOptionContainer &x,TListOptionContainer &y);

void TfrmSelectDish::RemoveSideItemFromItem(
  TItemCompleteSub *side_item,
  TItemComplete *)
{
   side_item->ReturnToAvailability();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::ItemAlteredRefresh(Messages::TMessage &Message)
{
   UpdateMenuItemsAfterLoginScreen();
}
// ---------------------------------------------------------------------------
__fastcall TfrmSelectDish::TfrmSelectDish(TComponent* Owner) : TZForm(Owner), ParkedSales(new TManagerParkedSales())
{
	TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->OnCardInserted.RegisterForEvent(OnSmartCardInserted);
	TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->OnCardRemoved.RegisterForEvent(OnSmartCardRemoved);
	TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->OnCardUpdated.RegisterForEvent(OnSmartCardInserted);
    isExtendedDisplayActive = false;
    patronsStore.clear();
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::CalculateAndDisplayTotalServiceCharge() const
{
	TContainerOrders &orders    =	*SeatOrders.at(SelectedSeat)->Orders;
	Currency serviceCharge      = 0;
    Currency serviceChargeTax   = 0;

    for (int i = 0; i < SeatOrders[SelectedSeat]->Orders->Count; i++)
	{
		 TItemMinorComplete *Order = (TItemMinorComplete*)SeatOrders[SelectedSeat]->Orders->Items[i];

         serviceCharge      += Order->ServiceChargeSides_BillCalc();
         serviceChargeTax   += Order->ServiceChargeTaxSides_BillCalc();
	}

    total_service_charge = serviceCharge + 0.0;

    if( !TGlobalSettings::Instance().IncludeServiceChargeTaxInTaxValue )
    {
        total_service_charge = total_service_charge + serviceChargeTax;
    }

    serviceChargeFigure->Caption = UnicodeString::CurrToStrF(
                                    RoundToNearest(total_service_charge, 0.01, TGlobalSettings::Instance().MidPointRoundsDown),
                                    UnicodeString::sffNumber, 2) + " ";
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::CalculateAndDisplayTotalTax() const
{
	TContainerOrders &orders =  	*SeatOrders.at(SelectedSeat)->Orders;
	Currency tax                = 0.0;
    Currency serviceChargeTax   = 0.0;

	for (int i = 0, j = orders.Count; i < j; i++)
    {
        TItemMinorComplete *Order = (TItemMinorComplete*)orders.Items[i];

        serviceChargeTax    += Order->ServiceChargeTaxSides_BillCalc();
        tax                 += Order->TotalTaxSides_BillCalc();
    }

    total_tax = tax + 0.0;

    if( !TGlobalSettings::Instance().IncludeServiceChargeTaxInTaxValue )
    {
        // bill calulator includes service charge tax in the total tax figure by default
        // if the setting is set to false, then we are to remove the service charge tax portion
        total_tax = total_tax - serviceChargeTax;
    }

    taxFigure->Caption = UnicodeString::CurrToStrF(
                               RoundToNearest(total_tax, 0.01, TGlobalSettings::Instance().MidPointRoundsDown),
                               UnicodeString::sffNumber, 2) + " ";
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::UpdateTaxLabels()
{
	CalculateAndDisplayTotalServiceCharge();
	CalculateAndDisplayTotalTax();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::FormCreate(TObject *Sender)
{
	FormResize(Sender);
    IsWaiterLogged = false;
	PosRefresh = true;
	LastTableNo = 1;
	LastSeatNumber = 1;
	CurrentTimeKey = 0;
	LastEnabledStateSync = -1;
	CurrentTenderButton = NULL;
	DisplayingPlusOptions = true;
	Always_Prompt = false;
	OrderHeld = false;
	CurrentServingCourse.Reset(TDeviceRealTerminal::Instance().Menus->DefaultServingCourse);
	tbtnSelectTable->Enabled = TGlobalSettings::Instance().TablesEnabled;
	tbtnSelectTable->Visible = TGlobalSettings::Instance().TablesEnabled;
	tbtnMembership->Enabled = TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Registered"];
	lbeTotalCost->Caption = CurrToStrF(0, ffNumber, CurrencyDecimals) + " ";
	lbeChange->Caption    = CurrToStrF(0, ffNumber, CurrencyDecimals) + " ";
	TSeatOrders *Temp = new TSeatOrders(0);
	Temp->Orders->OnCurrentItemRedirectorChanged = CurrentItemChange;
	Temp->Orders->HideServingCourseLabels = TGlobalSettings::Instance().HideServingCourseLabels;
	Temp->SeatName = "Table Account";
	SeatOrders.push_back(Temp);
	tbtnParkSales->Tag = tbtnParkSales->ButtonColor;
    customerDisp.FirstVisit=false;
    customerDisp.HappyBirthDay=false;
    customerDisp.TierLevel=0;
    TGlobalSettings::Instance().TierLevelChange=0;
    unsigned __int32 maxSeatCount = getMaxSeatCount();

	for( unsigned __int32 i = 1; i <= maxSeatCount; i++)
	{
		TSeatOrders *Temp = new TSeatOrders(i);
		Temp->Orders->OnCurrentItemRedirectorChanged = CurrentItemChange;
		Temp->Orders->HideServingCourseLabels = TGlobalSettings::Instance().HideServingCourseLabels;
		SeatOrders.push_back(Temp);
	}

	if (TDeviceRealTerminal::Instance().Menus != NULL)
	{
		TDeviceRealTerminal::Instance().Menus->OnMenuChanged.RegisterForEvent(OnMenuChange);
		TDeviceRealTerminal::Instance().Menus->OnTillsMenusAltered.RegisterForEvent(OnTillsMenusAltered);
	}

	BeforeItemOrdered.RegisterForEvent(OnBeforeItemOrdered);
	AfterItemOrdered.RegisterForEvent(OnAfterItemOrdered);
	AfterSaleProcessed.RegisterForEvent(OnAfterSaleProcessed);
	AfterSelectedItemChanged.RegisterForEvent(RefreshModifyGui);
//    if(TGlobalSettings::Instance().AutoLogoutPOS && TGlobalSettings::Instance().AutoLogoutSeconds > 0)
	    TDeviceRealTerminal::Instance().EventLockOutTimer.RegisterForEvent(OnLockOutTimer);
	TDeviceRealTerminal::Instance().PaymentSystem->OnAfterTransactionComplete.RegisterForEvent(TransactionComplete);

	tmPosRefresh->Enabled = true;
    frmProcessWebOrder.reset(new TfrmProcessWebOrder(this));
    cmClientManager.reset(new TChefmateClientManager());
    createTransactionAuditScreen();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::FormDestroy(TObject *Sender)
{
	tmPosRefresh->Enabled = false;
	for (UINT i = 0; i < SeatOrders.size(); i++)
	{
		delete SeatOrders[i];
        SeatOrders[i] = NULL;
	}

	if (TDeviceRealTerminal::Instance().Menus != NULL)
	{
		TDeviceRealTerminal::Instance().Menus->OnMenuChanged.DeregisterForEvent(OnMenuChange);
		TDeviceRealTerminal::Instance().Menus->OnTillsMenusAltered.DeregisterForEvent(OnTillsMenusAltered);
	}
	BeforeItemOrdered.DeregisterForEvent(OnBeforeItemOrdered);
	AfterItemOrdered.DeregisterForEvent(OnAfterItemOrdered);
	AfterSaleProcessed.DeregisterForEvent(OnAfterSaleProcessed);
	AfterSelectedItemChanged.DeregisterForEvent(RefreshModifyGui);
//    if(TGlobalSettings::Instance().AutoLogoutPOS && TGlobalSettings::Instance().AutoLogoutSeconds > 0)
	    TDeviceRealTerminal::Instance().EventLockOutTimer.DeregisterForEvent(OnLockOutTimer);
	TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->OnCardInserted.DeregisterForEvent(OnSmartCardInserted);
	TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->OnCardRemoved.DeregisterForEvent(OnSmartCardRemoved);
	TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->OnCardUpdated.DeregisterForEvent(OnSmartCardInserted);
	TDeviceRealTerminal::Instance().PaymentSystem->OnAfterTransactionComplete.DeregisterForEvent(TransactionComplete);
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::SyncGridFontSizes()
{
	int font_size = TGlobalSettings::Instance().posButtonFontSize&~0x80;

   tgridItemOptions->Font->Size = font_size;
   tgridItemSetMenuItems->Font->Size = font_size;
  tgridItemSideItems->Font->Size = font_size;
   tgridOrderItem->Font->Size = font_size;
}

// ---------------------------------------------------------------------------
ChitResult TfrmSelectDish::InitializeChit()
{
    ChitResult result;
    Database::TDBTransaction transaction(TDeviceRealTerminal::Instance().DBControl);
    transaction.StartTransaction();
    TChitNumberController controller(this, transaction);
    result = controller.GetDefaultChitNumber(ChitNumber);
    tbtnChitNumber->Visible = result != ChitDisabled;
    if (tbtnChitNumber->Visible)
    {
        tbtnChitNumber->Caption = ChitNumber.Name;
    }
    if(ChitNumber.Valid())
    {
        GetChitDiscountList(transaction, ChitNumber.DiscountList);
    }
    transaction.Commit();
    return result;
}
// ---------------------------------------------------------------------------
ChitResult TfrmSelectDish::SetupChit(Database::TDBTransaction &tr)
{
     TChitNumberController controller(this, tr);
     ChitResult selection_result = controller.GetChitNumber(true, ChitNumber);
     tbtnChitNumber->Caption =
    ChitNumber.ChitNumberKey ? ChitNumber.GetChitNumber()
                               : UnicodeString(L"Chit");
    if(ChitNumber.Valid())
    {
        TGlobalSettings::Instance().TabPrintName = "";
        TGlobalSettings::Instance().TabPrintPhone = "";
    }

    ChangeMenuToChitDefault();

    if(ChitNumber.IsDisplayTablesEnabled)
    {
        showTablePicker();
    }
    //fadil
    //MM2035andMM2036
    if(ChitNumber.IsCaptureCustomerNameAndPhone)
    {
        ChitCaptAndSaveName();
    }

    //MM2063
    if(ChitNumber.IsCaptureCustomerDetails)
    {
        if(TGlobalSettings::Instance().EnablePhoneOrders)
        TDeviceRealTerminal::Instance().ManagerMembership->SetPhoneOrderFlowMemberSelection(true);

        if (TDeviceRealTerminal::Instance().Modules.Status[eSmartCardSystem]["Enabled"])
        {
            // If there is a card inserted go stright to editing that user.
            if (TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->CardOk)
            {
                Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
                TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);

                TMMContactInfo UserInfo;

                DBTransaction.StartTransaction();
                std::auto_ptr<TContactStaff>Staff(new TContactStaff(DBTransaction));
                TLoginSuccess Result = Staff->Login(this, DBTransaction, UserInfo, CheckAccountManager);

                if (Result == lsAccepted)
                {
                    TDeviceRealTerminal::Instance().ManagerMembership->EditMember(DBTransaction, SeatOrders[SelectedSeat]->Orders->AppliedMembership);
                }
                else if (Result == lsDenied)
                {
                    MessageBox("You do not have access to the loyalty settings.", "Error", MB_OK + MB_ICONERROR);
                }
                else if (Result == lsPINIncorrect)
                {
                    MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
                }
                DBTransaction.Commit();
            }
            else if (TDeviceRealTerminal::Instance().ManagerMembership->AllowSmartCardNotPresentMemberTransactions)
            {
                TMMContactInfo TempUserInfo;
                Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
                TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
                DBTransaction.StartTransaction();
                ApplyMembership(DBTransaction, TempUserInfo);
                DBTransaction.Commit();
                //MM2064
                CustName = TempUserInfo.Name;
                CustPhone = TempUserInfo.Phone;
                CustAddress = TempUserInfo.LocationAddress;
                frmProcessWebOrder->MemberInfo = TempUserInfo;
            }
        }
        else if (TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Enabled"])
        {
            TMMContactInfo TempUserInfo;
            Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
            TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
            DBTransaction.StartTransaction();
            ApplyMembership(DBTransaction, TempUserInfo);
            DBTransaction.Commit();
            if (!TGlobalSettings::Instance().EnablePhoneOrders)
               {
                  AutoLogOut();
               }
            //MM2064
            CustName = TempUserInfo.Name;
            CustPhone = TempUserInfo.Phone;
            CustAddress = TempUserInfo.LocationAddress;
            frmProcessWebOrder->MemberInfo = TempUserInfo;
            //MM-1647: Ask for chit if it is enabled for every order.
            NagUserToSelectChit();
        }
        else
        {
            MessageBox("Membership is not Enabled.", "Error", MB_OK + MB_ICONERROR);
        }
    }
   if(ChitNumber.PromptForPickUpDeliveryTime && IsChitPromptFormActive)
    {
       CaptureDeliveryTime();
    }

    return selection_result;

}
// ---------------------------------------------------------------------------
void TfrmSelectDish::ChangeMenuToChitDefault()
{
    if (!ChitNumber.HasDefaultMenu())
        return;
 else {
  TDeviceRealTerminal &drt = TDeviceRealTerminal::Instance();
        TManagerMenusPOS &mmp = *drt.Menus;
        const UnicodeString &default_menu = ChitNumber.DefaultMenu();

        if(default_menu != "None")
        {
            if (!mmp.MenuInCurrent(default_menu)) {
                std::vector<UnicodeString> current_menus = mmp.GetCurrentMenus();
                TNetMessageMenuChanged request;
                Database::TDBTransaction tr(drt.DBControl);

                request.Menu_Names[default_menu] = eMenuAddReplace;
                for (std::vector<UnicodeString>::const_iterator i =
                       current_menus.begin(); i != current_menus.end(); ++i)
                    request.Menu_Names[*i] = eMenuAddReplace;

                request.Broadcast = false;

                tr.StartTransaction();
                    mmp.MenuChanged(tr, &request);
                    mmp.SwapInNewMenus();
                    mmp.SetMenuList(tr, drt.ID.DeviceKey);
                tr.Commit();
            }
            mmp.SetCurrentMenu(default_menu);
        }
        else
        {
            mmp.SetCurrentMenu(TDeviceRealTerminal::Instance().Menus->VisibleMenu->MenuName);
        }

    }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::FormShow(TObject *Sender)
{

     IsTabBillProcessed=false;
     SyncGridFontSizes();
     IsSubSidizeProfileExist=false;
     IsSubSidizeProcessed =false;
     startCustomerDisplayServer();
	 // Serving Course Display Options may have changed.
	 RedrawServingCourses();
	 ResetPOS();

	TSeatOrders *Temp = SeatOrders[0];
	Temp->Orders->HideServingCourseLabels = TGlobalSettings::Instance().HideServingCourseLabels;
	SeatOrders[0] = Temp;

    unsigned __int32 maxSeatCount = getMaxSeatCount();
	for( unsigned __int32 i = 1; i <= maxSeatCount; i++)
	{
		TSeatOrders *Temp = SeatOrders[i];
		Temp->Orders->HideServingCourseLabels = TGlobalSettings::Instance().HideServingCourseLabels;
		SeatOrders[i] = Temp;
	}

	tbtnSelectTable->Enabled = TGlobalSettings::Instance().TablesEnabled;
	tbtnSelectTable->Visible = TGlobalSettings::Instance().TablesEnabled;
	tiClock->Enabled = true;

	tbtnDollar1->Caption = GetTenderStrValue( vmbtnDollar1 );
	tbtnDollar2->Caption = GetTenderStrValue( vmbtnDollar2 );
	tbtnDollar3->Caption = GetTenderStrValue( vmbtnDollar3 );
	tbtnDollar4->Caption = GetTenderStrValue( vmbtnDollar4 );
	tbtnDollar5->Caption = GetTenderStrValue( vmbtnDollar5 );
    setParkedSalesBtnColor();
	SetGridColors(tgridOrderCourse);
    SetGridColors(tgridServingCourse);
	SetGridColors(tgridItemSideCourses);
	SetGridColors(btngridModify);
	SetGridColors(tgridItemOptions);
	SetGridColors(tgridItemServingCourse);
	SetGridColors(tgridItemSetMenuItems);
	SetGridColors(tgridSeats);
	SetNumpadColors(tnpQuantity);

    if(TGlobalSettings::Instance().ShowLargeFonts )
    {
      tgridOrderCourse->Font->Size = 18;
      //tgridItemServingCourse->Font->Size =14;
    }
    else
    {
      tgridOrderCourse->Font->Size = 12;
     // tgridItemServingCourse->Font->Size =14;
    }
	InitXeroIntegration();
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	TManagerChitNumber::Instance().Load(DBTransaction);
    DBTransaction.Commit();

    FormResize(Sender);
    if(TGlobalSettings::Instance().EnableTableDisplayMode)
    {
          showTablePicker();
    }

    setParkedSaleBtnStatus();
    CheckDiscountPoints = false;
    if(TGlobalSettings::Instance().ShowCurrencySymbol)
    {
       lbeTotal->Caption = "Total (" + CurrencyString + ")";
    }
    else
    {
      lbeTotal->Caption = "Total";
    }
    resetTransactionAuditScreen();
    sec_ref = 0;

	DBTransaction.StartTransaction();
    std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
    stHappyHour->Visible = false;
    UserForceHappyHourRight=false;
    UserForceHappyHourRight = Staff->TestAccessLevel( TDeviceRealTerminal::Instance().User, CheckAllowForcedHappyHour);
    DBTransaction.Commit();
    IsParkSalesEnable = false;
    if(TGlobalSettings::Instance().ItemSearch )
    {
        lbDisplay->Height = (lbDisplay->Height - 60);
        lbDisplay->Top = (lbDisplay->Top + 58);

        tedtSearchItem->Left =   8;
        tedtSearchItem->Top =    51;
        tedtSearchItem->Width  = 127;
        tedtSearchItem->Height = 54;
        tedtSearchItem->Visible = true;
        std::auto_ptr<TfrmProductSearch> frmProductSearch(TfrmProductSearch::Create<TfrmProductSearch>(Screen->ActiveForm));
        frmProductSearch->SearchType = 0;
        tedtSearchItem->Text = "";
        tbtnSearch->Visible = true;
    }
    else
    {
         tbtnSearch->Visible = false;
         tedtSearchItem->Visible = false;
    }
    SetPOSBackgroundColor();
    isChitDiscountExist = false;

    if(TGlobalSettings::Instance().WebMateEnabled)
        ProcessWebOrders(false);

    tiPMSRoom->Enabled = TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot && TGlobalSettings::Instance().EnableCustomerJourney;
    tbtnChitNumber->Caption = "Chit";
    ChitNumber = TChitNumber();
    if(!tiPMSRoom->Enabled)
    tiChitDelay->Enabled = TGlobalSettings::Instance().NagUserToSelectChit;
    InitializeChit();
    isWalkInUser = true;
    isRoomNoUiCalled = false;
    IsChitPromptFormActive=true;   
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::AdjustScreenSize()
{
    if(TGlobalSettings::Instance().ExtendCurrencyDisplay && !isExtendedDisplayActive)
    {
        isExtendedDisplayActive = true;

        pnlItemPickerBorder->Width = (pnlItemPickerBorder->Width + 120);
        pnlItemPickerBorder->Left = (pnlItemPickerBorder->Left - 120);
        tgridSeats->Width = (tgridSeats->Width - 120);
        lbDisplay->Width = (lbDisplay->Width + 120);
        lbDisplay->Font->Size = (lbDisplay->Font->Size - 2);
        tgridOrderItem->Width = (tgridOrderItem->Width - 120);
        lbeTotalCost->Width = (lbeTotalCost->Width + 120);
        lbeChange->Width = (lbeChange->Width + 120);
        lbeTotalCost->Font->Size = (lbeTotalCost->Font->Size - 2);
        lbeChange->Font->Size = (lbeChange->Font->Size - 2);
        tbtnMembership->Width = (tbtnMembership->Width + 120);
        btnRemove->Width = (btnRemove->Width + 39);
        btnUp->Left = (btnUp->Left + 39);
        btnUp->Width = (btnUp->Width + 39);
        btnDown->Left = (btnDown->Left + 39 + 39);
        btnDown->Width = (btnDown->Width + 39);

        tbtnDollar1Left = tbtnDollar1->Left;
        tbtnDollar1->Left = tbtnDollar1Left - 5;
        tbtnDollar1Width = tbtnDollar1->Width;
        tbtnDollar1->Width = (tbtnDollar1Width - 15);

        tbtnDollar2Left = tbtnDollar2->Left;
        tbtnDollar2->Left = (tbtnDollar2Left - 15);
        tbtnDollar2Width = tbtnDollar2->Width;
        tbtnDollar2->Width = (tbtnDollar2Width - 15);

        tbtnDollar3Left = tbtnDollar3->Left;
        tbtnDollar3->Left = (tbtnDollar3Left - 15 - 15);
        tbtnDollar3Width = tbtnDollar3->Width;
        tbtnDollar3->Width = (tbtnDollar3Width - 15);

        tbtnDollar4Left = tbtnDollar4->Left;
        tbtnDollar4->Left = (tbtnDollar4Left - 15 - 15 - 15);
        tbtnDollar4Width = tbtnDollar4->Width;
        tbtnDollar4->Width = (tbtnDollar4Width - 15);

        tbtnDollar5Left = tbtnDollar5->Left;
        tbtnDollar5->Left = (tbtnDollar5Left - 15 - 15 - 15 - 15);
        tbtnDollar5Width = tbtnDollar5->Width;
        tbtnDollar5->Width = (tbtnDollar5Width - 15);
    }
    else if(!TGlobalSettings::Instance().ExtendCurrencyDisplay && isExtendedDisplayActive)
    {
        isExtendedDisplayActive = false;

        pnlItemPickerBorder->Width = (pnlItemPickerBorder->Width - 120);
        pnlItemPickerBorder->Left = (pnlItemPickerBorder->Left + 120);
        tgridSeats->Width = (tgridSeats->Width + 120);
        lbDisplay->Width = (lbDisplay->Width - 120);
        lbDisplay->Font->Size = (lbDisplay->Font->Size + 2);
        tgridOrderItem->Width = (tgridOrderItem->Width + 120);
        lbeTotalCost->Width = (lbeTotalCost->Width - 120);
        lbeChange->Width = (lbeChange->Width - 120);
        lbeTotalCost->Font->Size = (lbeTotalCost->Font->Size + 2);
        lbeChange->Font->Size = (lbeChange->Font->Size + 2);
        tbtnMembership->Width = (tbtnMembership->Width - 120);
        btnRemove->Width = (btnRemove->Width - 39);
        btnUp->Left = (btnUp->Left - 39);
        btnUp->Width = (btnUp->Width - 39);
        btnDown->Left = (btnDown->Left - 39 - 39);
        btnDown->Width = (btnDown->Width - 39);

        tbtnDollar1->Left = tbtnDollar1Left;
        tbtnDollar1->Width = tbtnDollar1Width;

        tbtnDollar2->Left = tbtnDollar2Left;
        tbtnDollar2->Width = tbtnDollar2Width;

        tbtnDollar3->Left = tbtnDollar3Left;
        tbtnDollar3->Width = tbtnDollar3Width;

        tbtnDollar4->Left = tbtnDollar4Left;
        tbtnDollar4->Width = tbtnDollar4Width;

        tbtnDollar5->Left = tbtnDollar5Left;
        tbtnDollar5->Width = tbtnDollar5Width;
    }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::WebOrder(Messages::TMessage& Message)
{
	if (TGlobalSettings::Instance().WebMateEnabled && TDeviceRealTerminal::Instance().DBControl.Connected()) // If we not connected dont bother.
	{
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();
		if (!OrdersPending() && !OrdersParked(DBTransaction))
		{
			ProcessWebOrders();
		}
		DBTransaction.Commit();
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::ProcessWebOrders(bool Prompt)
{     Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);

    try
    {
        DBTransaction.StartTransaction();

        if(!NotifyLastWebOrder(DBTransaction))
        {
            bool WebOrdersPending = TDBWebUtil::WebOrdersPending(DBTransaction);
            if (WebOrdersPending)
            {
                frmProcessWebOrder->Execute();
                this->SetFocus();
            }
            else if (Prompt)
            {
                MessageBox("No Web Orders Pending", "No Web Orders Pending", MB_OK + MB_ICONWARNING);
            }
            //after processing web order again load chit specific to terminal
            TManagerChitNumber::Instance().Load(DBTransaction);
            DBTransaction.Commit();
        }
        else
        {
           DBTransaction.Commit();
        }
    }
    catch(Exception & E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        DBTransaction.Rollback();
    }
}
// ---------------------------------------------------------------------------
bool TfrmSelectDish::IsPricedBarcode(AnsiString &data) const
{
	const AnsiString pricedBarcodePrefix = "02";
	int length = 13;
	const int pos = 1;
	if (data.Length() == (length - 1))
	{
		data.Insert("0", 1);
    }
    switch(TGlobalSettings::Instance().BarcodeFormat)
    {
       //Defaul Fromat -- FF CCCCC PCD PPPP CD
     case 0:
        length = 13;
      break;
     //Default(5 Digit Price) -- FF CCCCC PCD PPPPP CD
     case 1:
        length = 14;
      break;
     //Default(6 Digit Price) -- F2 CCCCC XXXXXX CD
     case 2:
        length = 14;
        break;
     //Weighed And Priced Fromat -- FF CCCCC WWWWWW PPPPPP CD
     case 4:
        length = 20;
      break;
    }

    // making sure the data is the right length and the prefix is in the
    // correct position
	return (data.Length() == length) && (data.Pos(pricedBarcodePrefix) == pos);
}
// ---------------------------------------------------------------------------
bool TfrmSelectDish::IsWeighedBarcode(AnsiString &data) const
{
	const AnsiString pricedBarcodePrefix = "02";
	int length = 14;
	const int pos = 1;
	if (data.Length() == (length - 1))
	{
		data.Insert("0", 1);
    }
        // making sure the data is the right length and the prefix is in the
    // correct position
	return (data.Length() == length) && (data.Pos(pricedBarcodePrefix) == pos);
}
// ---------------------------------------------------------------------------
Currency TfrmSelectDish::GetPriceFromBarcode(AnsiString barcode)
{
  Currency ItemPrice = 0;
  switch(TGlobalSettings::Instance().BarcodeFormat)
    {
     //Defaul Fromat -- FF CCCCC PCD PPPP CD
     case 0:
        ItemPrice = Currency(barcode.SubString(9, 2) + "." + barcode.SubString(11, 2));
      break;
     //Default(5 Digit Price) -- FF CCCCC PCD PPPPP CD
     case 1:
        ItemPrice = Currency(barcode.SubString(9, 3) + "." + barcode.SubString(12, 2));
      break;
     //Default(6 Digit Price) -- F2 CCCCC XXXXXX CD
     case 2:
        ItemPrice = Currency(barcode.SubString(8, 4) + "." + barcode.SubString(12, 2));
      break;
     //Weighed And Priced Fromat -- FF CCCCC WWWWWW PPPPPP CD
     case 4:
        ItemPrice = Currency(barcode.SubString(14, 4) + "." + barcode.SubString(18, 2));
      break;
    }
   return ItemPrice;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::CardSwipe(Messages::TMessage& Message)
{
    TGlobalSettings::Instance().IsThorlinkTender = false;
	enum TCardType
	{
		eUnknown, eKitchenDocket, eStaffCard, eMemberCard, eTabCard, eDiscountCard, eEmbeddedPriceBarcode
	};

	TCardType DataType = eUnknown;
	int TimeKey = 0;
	int TabKey = 0;
    bool newCard = false;
	TMMContactInfo TempUserInfo;
	AnsiString Data = *((AnsiString*)Message.WParam);

	TempUserInfo.CardStr = Data.SubString(1, 80);
	TempUserInfo.ProxStr = Data.SubString(1, 80);
    TLoginSuccess LoginResult = lsDenied;

	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff>Staff(new TContactStaff(DBTransaction));

	if (Data.Pos("MMK") != 0) // Kitchen Docket.
	{
		int Index = Data.Pos("MMK");
		DataType = eKitchenDocket;
		TimeKey = StrToInt("0x" + Data.SubString(Index + 3, Data.Length()));
	}
	else if (Data.Pos("MMPROX") != 0)
	{
		if (Staff->Login(this, DBTransaction, TempUserInfo, CheckPOS) == lsAccepted)
		{
			DataType = eStaffCard;
		}
		else if(!TGlobalSettings::Instance().LoyaltyMateEnabled)
		{
			LoginResult = TDeviceRealTerminal::Instance().ManagerMembership->FindMember(DBTransaction, TempUserInfo);
			if (LoginResult == lsAccepted || LoginResult == lsAccountBlocked)
			{
				DataType = eMemberCard;
				TabKey = TDBTab::GetTabByOwner(DBTransaction, TempUserInfo.ContactKey);
			}
		}
	}
	else if (Staff->Login(this, DBTransaction, TempUserInfo, CheckPOS) == lsAccepted)
	{
		DataType = eStaffCard;
	}
	else
	{
		if (TDBTab::GetTabByCard(DBTransaction, Data.SubString(1, 80)) != 0 && SelectedTable == 0)
		{
			DataType = eTabCard;
			TabKey = TDBTab::GetTabByCard(DBTransaction, Data.SubString(1, 80));
		}
		else if (ManagerDiscount->GetDiscount(DBTransaction, Data.SubString(1, 80)) != 0)
		{
			DataType = eDiscountCard;
		}
		else if(TDeviceRealTerminal::Instance().ManagerMembership->IsCard(DBTransaction, TempUserInfo) == lsAccepted &&
                !TGlobalSettings::Instance().IsThorlinkSelected && !TGlobalSettings::Instance().LoyaltyMateEnabled)
		{
			LoginResult = TDeviceRealTerminal::Instance().ManagerMembership->FindMember(DBTransaction, TempUserInfo);
			if (LoginResult == lsAccepted || LoginResult == lsAccountBlocked)
			{
				DataType = eMemberCard;
				TabKey = TDBTab::GetTabByOwner(DBTransaction, TempUserInfo.ContactKey);
			}
		}
		else if((TDeviceRealTerminal::Instance().ManagerMembership->IsCard(DBTransaction, TempUserInfo) == lsUserNotFound)
                 && TGlobalSettings::Instance().IsDrinkCommandEnabled)
        {
              TMMContactInfo ContactInfo;
              ContactInfo.ProxStr = TempUserInfo.ProxStr;
              TDeviceRealTerminal::Instance().ManagerMembership->AddMember(ContactInfo);
              newCard = true;
        }
        else if(!TGlobalSettings::Instance().IsThorlinkSelected && !TGlobalSettings::Instance().LoyaltyMateEnabled)
		{
            LoginResult = TDeviceRealTerminal::Instance().ManagerMembership->FindMember(DBTransaction, TempUserInfo);
            if (LoginResult == lsAccepted || LoginResult == lsAccountBlocked)
            {
                DataType = eMemberCard;
                TabKey = TDBTab::GetTabByOwner(DBTransaction, TempUserInfo.ContactKey);
            }
		}
	}
	DBTransaction.Commit();

	if (DataType == eKitchenDocket) // Kitchen Docket.
	{
		DBTransaction.StartTransaction();
		TDBSaleTimes::CloseMakeStartTime(DBTransaction, TimeKey);
		DBTransaction.Commit();

		// Tell ChefMate this Orders been swiped off.
		if (TDeviceRealTerminal::Instance().KitchenMod->Enabled)
		{
			DBTransaction.StartTransaction();

			std::auto_ptr<TStringList>ChefMateIPList(new TStringList);
			TManagerPhysicalPrinter ManagerPhysicalPrinter;
			ManagerPhysicalPrinter.GetPrinterServerList(DBTransaction, ChefMateIPList.get(), ptChefMate_Printer);

			std::auto_ptr<TNetMessageChefMate>ChefRequest(new TNetMessageChefMate);
			ChefRequest->OrderNumber = 0;
			ChefRequest->RemoveOrder = false;
			ChefRequest->OrderTimeScanned = true;
			ChefRequest->TillColour = TGlobalSettings::Instance().KitchenColour;
			ChefRequest->ChitNumber = ChitNumber.ChitNumber;
			ChefRequest->Device = TDeviceRealTerminal::Instance().ID.Name;
			ChefRequest->User = TDeviceRealTerminal::Instance().User.Name;
			ChefRequest->TimeKey = TimeKey;
			for (int i = 0; i < ChefMateIPList->Count; i++)
			{
				AnsiString ChefMateIP = ChefMateIPList->Strings[i];
				TDeviceRealTerminal::Instance().KitchenMod->SendRequest(ChefRequest.get(), ChefMateIP);
			}
			DBTransaction.Commit();
		}

	}
	else if (DataType == eStaffCard)
	{
		DBTransaction.StartTransaction();
		if (Staff->Login(this, DBTransaction, TempUserInfo, CheckPOS) == lsAccepted)
		{
			StaffChanged(TempUserInfo);
		}
		DBTransaction.Commit();
	}
	else if (DataType == eTabCard)
	{
		if (OrdersPending())
		{
			DBTransaction.StartTransaction();

			int SelectedTab = TabKey;
			Currency CreditLimit = TDBTab::GetTabCreditLimit(DBTransaction, SelectedTab);
			bool DoProcessOrders = true;
			if (CreditLimit != -1)
			{
				Currency OrdersTotal = 0;
				for (int b = 0; b < SeatOrders[0]->Orders->Count; b++)
				{
					TItemComplete *Order = SeatOrders[0]->Orders->Items[b];
					OrdersTotal += Order->TotalPriceAdjustmentSides();
				}
				if ((OrdersTotal + TDBTab::GetTabBalance(DBTransaction, SelectedTab)) > CreditLimit)
				{
					AnsiString CreditMessage = "This Tab Balance is " + FloatToStrF(TDBTab::GetTabBalance(DBTransaction, SelectedTab), ffNumber, 15, 2);
					MessageBox("Credit Limit of " + FloatToStrF(CreditLimit, ffNumber, 15, 2) + " Reached.\r" + CreditMessage + "\r" + "You cannot save these orders.", "Error",
						MB_OK + MB_ICONERROR);
					DoProcessOrders = false;
				}
			}

			if (!TabPINOk(DBTransaction, SelectedTab))
			{
				DoProcessOrders = false;
			}

			if (DoProcessOrders && TDBTab::GetTabLocked(DBTransaction, SelectedTab))
			{
				AnsiString CreditMessage = "This Tab is locked";
				AnsiString CreditMessage1 = "Reason :" + TDBTab::GetTabLockedReason(DBTransaction, SelectedTab);
				MessageBox(CreditMessage + "\r" + CreditMessage1 + "\r" + "You cannot save these orders here.", "Error", MB_OK + MB_ICONERROR);
				DoProcessOrders = false;
			}

			DBTransaction.Commit();

			TSaveOrdersTo OrderContainer;
			if (DoProcessOrders)
			{
				DBTransaction.StartTransaction();
				AnsiString TabName = "";
				if (TDBTab::GetTabOwner(DBTransaction, SelectedTab) != 0)
				{
					TabName = TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetContactName(DBTransaction, TDBTab::GetTabOwner(DBTransaction, SelectedTab));
				}
				else
				{
					TabName = TDBTab::GetTabName(DBTransaction, SelectedTab);
				}

				OrderContainer.Location["TabKey"] = SelectedTab;
				OrderContainer.Location["TMMTabType"] = TabNormal;
				OrderContainer.Location["TMMDisplayMode"] = eTabs;
				OrderContainer.Location["ContainerName"] = TabName;
				OrderContainer.Location["TabName"] = TabName;
				OrderContainer.Location["PartyName"] = "";
				OrderContainer.Location["SelectedSeat"] = 0;
				OrderContainer.Location["SelectedTable"] = 0;
				OrderContainer.Location["RoomNumber"] = 0;

				bool OrderConfimOk = true;
				if (!TGlobalSettings::Instance().DisableConfirmationOnSave)
				{
					OrderContainer.Values.push_back(TSaveOrdersTo::StringValuePair("Total", InitialMoney.GrandTotal));
					Currency Balance = TDBTab::GetTabBalance(DBTransaction, SelectedTab);
					OrderContainer.Values.push_back(TSaveOrdersTo::StringValuePair("Balance", Balance));
					OrderContainer.Values.push_back(TSaveOrdersTo::StringValuePair("New Balance", Balance + InitialMoney.GrandTotal));

                    TfrmConfirmOrder* frmConfirmOrder = new TfrmConfirmOrder(this, OrderContainer);
					if (!TGlobalSettings::Instance().DisableReceiptOnConfirmation)
					{
						SetReceiptPreview(DBTransaction, frmConfirmOrder->ReceiptDisplay, OrderContainer.Location["TMMTabType"], OrderContainer.Location["ContainerName"],
							OrderContainer.Location["TabName"], OrderContainer.Location["PartyName"], OrderContainer.Location["TabKey"]);
					}

					if (frmConfirmOrder->ShowModal() != mrOk)
					{
						OrderConfimOk = false;
					}
                    delete frmConfirmOrder;
                    frmConfirmOrder = NULL;
				}

				DBTransaction.Commit();

				if (OrderConfimOk)
				{
					std::auto_ptr<TfrmProcessing>(Processing)(TfrmProcessing::Create<TfrmProcessing>(this));
					Processing->Message = "Posting Orders";
					Processing->Show();

					DBTransaction.StartTransaction();
					bool PaymentComplete = ProcessOrders(tbtnSave, DBTransaction, OrderContainer.Location["TabKey"], // Tab
						OrderContainer.Location["TMMTabType"], // Tab Type
						OrderContainer.Location["ContainerName"], OrderContainer.Location["TabName"], OrderContainer.Location["PartyName"], OrderContainer.Location["PrintPreLimReceipt"],
						// Print Prelim Receipt.
						OrderContainer.Location["SelectedTable"], OrderContainer.Location["SelectedSeat"], OrderContainer.Location["RoomNumber"]);
					Processing->Close();

                    TStringList* logList = new TStringList();
                    logList->Clear();

					if (PaymentComplete)
					{
						DBTransaction.Commit();
						ResetPOS();
                        logList->Add("Trabsaction commit of CardSwipe().");
                        TSaveLogs::RecordFiscalLogs(logList);
					}
                    else
                    {
                        logList->Add("Payment not completed in CardSwipe().");
                        TSaveLogs::RecordFiscalLogs(logList);
                    }
                    delete logList;
                    logList = NULL;

					if (!OrdersPending()) // All Orders Posted No Exceptions
					{
						if (OrderContainer.Location["BillOff"])
						{
							TfrmBillGroup* frmBillGroup  = new  TfrmBillGroup(this, TDeviceRealTerminal::Instance().DBControl);
							frmBillGroup->ResetForm();
							if (OrderContainer.Location["TMMTabType"] == TabMember)
							{
								DBTransaction.StartTransaction();
								TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetContactDetails(DBTransaction, OrderContainer.Location["MemberKey"], frmBillGroup->TempUserInfo);
								DBTransaction.Commit();
							}
							frmBillGroup->ResetSelection();
							frmBillGroup->CurrentDisplayMode = OrderContainer.Location["TMMDisplayMode"];
							frmBillGroup->CurrentTabType = OrderContainer.Location["TMMTabType"];
							frmBillGroup->CurrentSelectedTab = OrderContainer.Location["TabKey"];
							frmBillGroup->CurrentTable = OrderContainer.Location["SelectedTable"];
							frmBillGroup->CurrentRoom = OrderContainer.Location["RoomNumber"];

                            DBTransaction.StartTransaction();

                            //Get the patrons for the current selected table, verify if it is already keyed in
                            std::vector<TPatronType> selectedTablePatrons = TDBTables::GetPatronCount(DBTransaction, frmBillGroup->CurrentTable);
                            int patronCount = GetCount(selectedTablePatrons);

                            if(TGlobalSettings::Instance().PromptForPatronCount && patronCount > 0)
                            {
                                frmBillGroup->PatronTypes = selectedTablePatrons;
                            }
                            DBTransaction.Commit();

							if (OrderContainer.Location["TabKey"] != 0)
							{
								frmBillGroup->SelectedTabs.insert(OrderContainer.Location["TabKey"]);
							}
							frmBillGroup->ShowModal();
							setPatronCount( frmBillGroup->PatronCount );
                            delete frmBillGroup;
                            frmBillGroup = NULL;
						}
					}
				}
			}
		}
	}
	else if (DataType == eMemberCard)
	{
    	if(LoginResult == lsAccepted || LoginResult == lsAccountBlocked)
        {
            DBTransaction.StartTransaction();
            ApplyMembership(DBTransaction, TempUserInfo);
            DBTransaction.Commit();
        }
	}
	else if (DataType == eDiscountCard)
	{
        if((SeatOrders[SelectedSeat]->Orders->AppliedMembership.ContactKey == 0) || (!TPaySubsUtility::IsLocalLoyalty()) ||
            SeatOrders[SelectedSeat]->Orders->AppliedMembership.Points.PointsRulesSubs.Contains(eprAllowDiscounts))
        {
            DBTransaction.StartTransaction();
            ApplyDiscount(DBTransaction, ManagerDiscount->GetDiscount(DBTransaction, Data.SubString(1, 80)), SeatOrders[SelectedSeat]->Orders->List);
            RedrawSeatOrders();
            TotalCosts();
            UpdateExternalDevices();
            DBTransaction.Commit();
        }
	}
	else
	{
		AnsiString Barcode = Data;
        bool ItemFound = false;
        if(!newCard)
        {
           ItemFound = false;
        }
        else
        {
           ItemFound = true;
        }
        bool isSameMenuTypeItemExist ;
		if (IsPricedBarcode(Barcode))
		{
			AnsiString ProductCode = Barcode.SubString(3, 5);
			Currency Price  = GetPriceFromBarcode(Barcode);

			std::pair<TItem*, TItemSize*> ItemInfo = GetLoadedItemFromBarcode(ProductCode);

			TItem *Item = ItemInfo.first;
			TItemSize *ItemSize = ItemInfo.second;

			ItemFound = Item != NULL && ItemSize != NULL;
            isSameMenuTypeItemExist = true;

            if(TGlobalSettings::Instance().IsBillSplittedByMenuType && ItemFound)
                isSameMenuTypeItemExist = CheckItemCanBeAddedToSeat(Item);

			if (ItemFound && Item->Enabled && isSameMenuTypeItemExist)
			{
               //Check for weighed and priced barcode
                 if(TGlobalSettings::Instance().BarcodeFormat == 4)
                    {
                       Item->ItemWeight.SetWeightIn_g(Currency(Barcode.SubString(8,6)));
                       Price= Price / Item->ItemWeight.AsKiloGrams()  ;
                    }
                    else
                    {
                       Item->SetQty(Price/ItemSize->Price);
                       Item->IsPriceBarcodedItem = true;
                       Price = 0;
                    }
				BeforeItemOrdered.Occured();
				if (BeforeItemOrdered.EventsFailed)
				{
					MessageBox(BeforeItemOrdered.ErrorMessage, BeforeItemOrdered.ErrorTitle, MB_OK + MB_ICONERROR);
				}

				DBTransaction.StartTransaction();
                if(Always_Prompt)
                {
                    AddItemToSeat(DBTransaction, Item, false, ItemSize, Price);
                }
                else
                {
				    AddItemToSeat(DBTransaction, Item, false, ItemSize, Price, true);
                }
				DBTransaction.Commit();

				AfterItemOrdered.Occured();
				if (AfterItemOrdered.EventsFailed)
				{
					MessageBox(AfterItemOrdered.ErrorMessage, AfterItemOrdered.ErrorTitle, MB_OK + MB_ICONERROR);
				}
			}
		}
        //Check for weighed barcode
        if(IsWeighedBarcode(Barcode) && TGlobalSettings::Instance().BarcodeFormat == 3)
          {
            AnsiString ProductCode = Barcode.SubString(3, 5);
			std::pair<TItem*, TItemSize*> ItemInfo = GetLoadedItemFromBarcode(ProductCode);

			TItem *Item = ItemInfo.first;
			TItemSize *ItemSize = ItemInfo.second;
			ItemFound = Item != NULL && ItemSize != NULL;

            isSameMenuTypeItemExist = true;
             if(TGlobalSettings::Instance().IsBillSplittedByMenuType && ItemFound)
                isSameMenuTypeItemExist = CheckItemCanBeAddedToSeat(Item);

			if (ItemFound && Item->Enabled && isSameMenuTypeItemExist)
			{
                Item->ItemWeight.SetWeightIn_g(Currency(Barcode.SubString(8,6)));
				BeforeItemOrdered.Occured();
				if (BeforeItemOrdered.EventsFailed)
				{
					MessageBox(BeforeItemOrdered.ErrorMessage, BeforeItemOrdered.ErrorTitle, MB_OK + MB_ICONERROR);
				}

				DBTransaction.StartTransaction();
                if(Always_Prompt)
                {
                    AddItemToSeat(DBTransaction, Item, false, ItemSize);
                }
                else
                {
				    AddItemToSeat(DBTransaction, Item, false, ItemSize, 0, true);
                }

				DBTransaction.Commit();
				AfterItemOrdered.Occured();
				if (AfterItemOrdered.EventsFailed)
				{
					MessageBox(AfterItemOrdered.ErrorMessage, AfterItemOrdered.ErrorTitle, MB_OK + MB_ICONERROR);
				}
			}
		}

		if(!ItemFound)
		{
			std::pair<TItem*, TItemSize*> itemAndSize = GetLoadedItemFromBarcode(Data);
			TItem *Item = itemAndSize.first;
			TItemSize *ItemSize = itemAndSize.second;

			ItemFound = Item != NULL && ItemSize != NULL;

            isSameMenuTypeItemExist = true;
             if(TGlobalSettings::Instance().IsBillSplittedByMenuType && ItemFound)
                isSameMenuTypeItemExist = CheckItemCanBeAddedToSeat(Item);

			if (ItemFound && Item->Enabled && isSameMenuTypeItemExist)
			{
				BeforeItemOrdered.Occured();
				if (BeforeItemOrdered.EventsFailed)
				{
					MessageBox(BeforeItemOrdered.ErrorMessage, BeforeItemOrdered.ErrorTitle, MB_OK + MB_ICONERROR);
				}
				else
				{
					DBTransaction.StartTransaction();
                    if(Always_Prompt)
                    {
                        AddItemToSeat(DBTransaction, Item, false, ItemSize);
                    }
                    else
                    {
                        AddItemToSeat(DBTransaction, Item, false, ItemSize, 0, true);
                    }
					DBTransaction.Commit();
				}

				AfterItemOrdered.Occured();
				if (AfterItemOrdered.EventsFailed)
				{
					MessageBox(AfterItemOrdered.ErrorMessage, AfterItemOrdered.ErrorTitle, MB_OK + MB_ICONERROR);
				}
			}
		}

        if(!ItemFound && TGlobalSettings::Instance().IsThorlinkSelected)
        {
            DBTransaction.StartTransaction();
            if(TDeviceRealTerminal::Instance().ManagerMembership->IsCard(DBTransaction, TempUserInfo) == lsAccepted)
            {
                LoginResult = TDeviceRealTerminal::Instance().ManagerMembership->FindMember(DBTransaction, TempUserInfo);
                if (LoginResult == lsAccepted || LoginResult == lsAccountBlocked)
                {
                    DataType = eMemberCard;
                    TabKey = TDBTab::GetTabByOwner(DBTransaction, TempUserInfo.ContactKey);
                    ApplyMembership(DBTransaction, TempUserInfo);
                    ItemFound = true;
                }
            }
            DBTransaction.Commit();
        }

		if(!ItemFound && !(TGlobalSettings::Instance().IsThorlinkSelected) && IsChitPromptFormActive)
		{     
              if(!TGlobalSettings::Instance().LoyaltyMateEnabled)
              {
                 MessageBox("No Item Found, Press OK to continue.", "No Item Found", MB_OK + MB_ICONWARNING);
              }
              else
              {
                  DBTransaction.StartTransaction();
                  GetMemberByBarcode(DBTransaction,Data);
                  DBTransaction.Commit();
              }
		}
	}
}
// ---------------------------------------------------------------------------
std::pair<TItem*, TItemSize*> TfrmSelectDish::GetLoadedItemFromBarcode(UnicodeString inBarcode)
{
	pair<TItem*, TItemSize*> ReturnValue(NULL, NULL);
	bool ItemFound = false;
	for (int i = 0; i < TDeviceRealTerminal::Instance().Menus->Current->Count && !ItemFound; i++)
	{
		TListMenu *Menu = TDeviceRealTerminal::Instance().Menus->Current->MenuGet(i);
		for (int j = 0; j < Menu->Count && !ItemFound; j++)
		{
			TListCourse *Course = Menu->CourseGet(j);
            Always_Prompt = Course->No_Default_Serving_Course;

			for (int i = 0; i < Course->Count && !ItemFound; i++)
			{
				TItem *Item = Course->ItemGet(i);

				for (int j = 0; j < Item->Sizes->Count && !ItemFound; j++)
				{
					TItemSize *ItemSize = Item->Sizes->SizeGet(j);
					if (ItemSize->Barcode == inBarcode)
					{
                        if(!Item->ExclusivelyAsSide)  //check item as exclusively not as a side..
                        {
                            ItemFound = true;
                            ReturnValue.first = Item;
                            ReturnValue.second = ItemSize;
                            if (!Item->Enabled)
                            {
                                MessageBox("Cannot order the item " + Item->Item + "( " + ItemSize->Name + ") as it has been disabled.",
                                           "Cannot order item",
                                           MB_OK | MB_ICONEXCLAMATION);
                            }
                        }
					}
				}
			}
		}
	}
	return ReturnValue;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::stCloseClick(TObject *Sender)
{
	Close();
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::SetItemNote(AnsiString Note)
{
	if (lbDisplay->ItemIndex > -1)
	{
		TItemRedirector *Redirector = (TItemRedirector*)lbDisplay->Items->Objects[lbDisplay->ItemIndex];
		if (Redirector->ItemType.Contains(itNormalItem))
		{
			TItemComplete *Order = (TItemComplete*)Redirector->ItemObject;
			Order->Note = Note;
		}
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
	{
		int Temp = Tag;
		Tag = Screen->Width;
		if ((double)Screen->Width / Screen->Height < 1.4)
		{
			ScaleBy(Screen->Width, Temp);
		}
		else
		{
			double Multiplier = Screen->Width / 1024.0;

			tbtnWebOrders->Width = tbtnWebOrders->Width * Multiplier;
			tbtnFunctions->Width = tbtnFunctions->Width * Multiplier;
			tbtnParkSales->Width = tbtnParkSales->Width * Multiplier;
			tbtnReprintReceipts->Width = tbtnReprintReceipts->Width * Multiplier;
			tbtnOpenDrawer->Width = tbtnOpenDrawer->Width * Multiplier;
			tbtnSystem->Width = tbtnSystem->Width * Multiplier;

			tbtnDollar1->Width = tbtnDollar1->Width * Multiplier;
			tbtnDollar2->Width = tbtnDollar2->Width * Multiplier;
			tbtnDollar3->Width = tbtnDollar3->Width * Multiplier;
			tbtnDollar4->Width = tbtnDollar4->Width * Multiplier;
			tbtnDollar5->Width = tbtnDollar5->Width * Multiplier;

			tbtnDollar1->Left = tbtnDollar1->Left * Multiplier;
			tbtnDollar2->Left = tbtnDollar2->Left * Multiplier;
			tbtnDollar3->Left = tbtnDollar3->Left * Multiplier;
			tbtnDollar4->Left = tbtnDollar4->Left * Multiplier;
			tbtnDollar5->Left = tbtnDollar5->Left * Multiplier;
		}

	}
	int Gap = Panel1->Left;

	tgridSeats->Top = ClientHeight - tgridSeats->Height - Panel1->Top;
	tgridSeats->Width = ClientWidth - tgridSeats->Left - (tbtnSelectTable->Left * 2);

	pnlItemPickerBorder->Top = Panel1->Top + Panel1->Height + Gap;
	pnlItemPickerBorder->Left = ClientWidth - pnlItemPickerBorder->Width - pnlOrdering->Left;
	pnlItemPickerBorder->Height = ClientHeight - pnlItemPickerBorder->Top - tgridSeats->Height - (Panel1->Top * 2);

	Panel1->Width = ClientWidth - (Panel1->Left * 2);

	tbtnTender->Top = ClientHeight - tbtnTender->Height - Panel1->Top;

	tbtnSelectTable->Top = tbtnTender->Top;
	tbtnChangeTable->Top = tbtnTender->Top;

	pnlOrdering->Top = Panel1->Top + Panel1->Height + Gap;
	pnlOrdering->Left = Panel1->Left;
	pnlOrdering->Width = pnlItemPickerBorder->Left - (Gap * 2);
	pnlOrdering->Height = ClientHeight - pnlOrdering->Top - tgridSeats->Height - (Panel1->Top * 2);

	tbtnCashSale->Top = tbtnTender->Top;
	tbtnSave->Top = tbtnTender->Top;
	tbtnDollar1->Top = tbtnTender->Top;
	tbtnDollar2->Top = tbtnTender->Top;
	tbtnDollar3->Top = tbtnTender->Top;
	tbtnDollar4->Top = tbtnTender->Top;
	tbtnDollar5->Top = tbtnTender->Top;

	tbtnTender->Left = pnlItemPickerBorder->Left;
	tbtnTender->Width = (pnlItemPickerBorder->Width - Panel1->Top) / 2;
	tbtnCashSale->Left = tbtnTender->Left + tbtnTender->Width + Panel1->Top;
	tbtnCashSale->Width = pnlItemPickerBorder->Width - tbtnTender->Width - Panel1->Top;

	btnUp->Top = pnlItemPickerBorder->ClientHeight - btnUp->Height - Panel1->Top;
	lbeChange->Top = btnUp->Top - lbeChange->Height - (Panel1->Top / 2);
	lbeTotalCost->Top = lbeChange->Top - lbeTotalCost->Height - (Panel1->Top / 2);
	lbChange->Top = lbeChange->Top + ((lbeChange->Height / 2) - (lbChange->Height / 2));
	lbeTotal->Top = lbeTotalCost->Top + ((lbeTotalCost->Height / 2) - (lbeTotal->Height / 2));
	btnDown->Top = btnUp->Top;
	btnRemove->Top = btnUp->Top;

	lbDisplay->Top = tbtnMembership->Top + tbtnMembership->Height + Gap;

	lbDisplay->Height = pnlItemPickerBorder->ClientHeight - lbDisplay->Top - (pnlItemPickerBorder->ClientHeight - lbeTotalCost->Top) - Panel1->Top;
	pnlItemModify->Top = pnlItemPickerBorder->Top;
	pnlItemModify->Height = ClientHeight - pnlItemModify->Top - tgridSeats->Height - (Panel1->Top * 2);
	pnlItemModify->Left = pnlOrdering->Left;
	pnlItemModify->Width = pnlOrdering->Width;

	// The intention is to get 7 course buttons down the left hand side and scroll for 6 of them.

	tgridOrderCourse->DefaultRowHeight = (tgridOrderCourse->ClientHeight - (tgridOrderCourse->VertScrollButtonHeight * 2) - (tgridOrderCourse->BorderWidth * 4) -
		(tgridOrderCourse->ButtonGapHeight * 7)) / 7;

	tgridOrderCourse->VertScrollDistance = tgridOrderCourse->DefaultRowHeight * 6;

	/*
	if (Screen->Height <= 600)
	{
	tgridOrderItem->Font->Size = 8;
	tgridOrderCourse->Font->Size = 8;
	}
	else if (Screen->Height <= 800)
	{
	tgridOrderItem->Font->Size = 12;
	tgridOrderCourse->Font->Size = 12;
	}
	else
	{
	tgridOrderItem->Font->Size = 14;
	tgridOrderCourse->Font->Size = 14;
	}
	 */
     //MM2038
  //  TGlobalSettings::Instance().TabPrintName = " ";
  //  TGlobalSettings::Instance().TabPrintPhone = " ";

    CreateTaxLabels();
    SetupTaxLabels();
	RefreshSeats();
	RedrawCourses();
    AdjustScreenSize();
    //CheckDiscountPoints = false;
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::CreateTaxLabels()
{
   taxLabel.reset(new TLabel(pnlItemPickerBorder));
   taxFigure.reset(new TLabel(pnlItemPickerBorder));
   serviceChargeLabel.reset(new TLabel(pnlItemPickerBorder));
   serviceChargeFigure.reset(new TLabel(pnlItemPickerBorder));

   taxLabel->Parent = pnlItemPickerBorder;
   taxFigure->Parent = pnlItemPickerBorder;
   serviceChargeLabel->Parent = pnlItemPickerBorder;
   serviceChargeFigure->Parent = pnlItemPickerBorder;
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::SetupTaxLabels()
{
	const unsigned short space_required = VertSpaceRequiredForLabels();
	unsigned short y_position = lbeChange->Top;

	taxLabel->Visible = false;
	serviceChargeLabel->Visible = false;
	taxFigure->Visible = false;
	serviceChargeFigure->Visible = false;

	if (!space_required)
		return;

	MakeRoomForLabels(space_required);


	if (TGlobalSettings::Instance().EnableDisplayTax)
    {
		taxLabel->Caption = "Tax";
		InitializeLabel(*taxLabel.get());

		taxFigure->Caption =
                    UnicodeString::CurrToStrF(Currency(0.0),
                                              UnicodeString::sffNumber,
                                              2) + " ";
		InitializeLabel(*taxFigure.get(), true);

		PlaceLabels(*taxLabel.get(),
                        *taxFigure.get(),
                        y_position);
	}

   if (TGlobalSettings::Instance().EnableDisplayServiceCharge)
   {
		serviceChargeLabel->Caption = "Service";
		InitializeLabel(*serviceChargeLabel.get());

		InitializeLabel(*serviceChargeFigure.get(), true);
		serviceChargeFigure->Caption =
                    UnicodeString::CurrToStrF(Currency(0.0),
                                              UnicodeString::sffNumber,
                                              2)  + " ";

		PlaceLabels(*serviceChargeLabel.get(),
   	                    *serviceChargeFigure.get(),
                        y_position);
	}
}
// ---------------------------------------------------------------------------
unsigned short TfrmSelectDish::VertSpaceRequiredForLabels()
const
{
	return LabelsToShow()
	       * (lbeChange->Height + VertSpaceSeparatingTotalAndChange());
}
// ---------------------------------------------------------------------------
unsigned short TfrmSelectDish::VertSpaceSeparatingTotalAndChange() const
{
	const unsigned short change = lbeChange->Top;
	const unsigned short total =  lbeTotalCost->Top;

	return std::max<unsigned short>(change, total)
	       - std::min<unsigned short>(change, total)
	       - lbeChange->Height;
}
// ---------------------------------------------------------------------------
unsigned short TfrmSelectDish::LabelsToShow() const
{
	return TGlobalSettings::Instance().EnableDisplayServiceCharge
	       + TGlobalSettings::Instance().EnableDisplayTax;
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::MakeRoomForLabels(unsigned short space_required)
{
	lbChange->Top -= space_required;
	lbDisplay->Height -= space_required;
	lbeChange->Top -= space_required;
	lbeTotal->Top -= space_required;
	lbeTotalCost->Top -= space_required;
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::InitializeLabel(TLabel &label, bool displays_value)
{
	label.AutoSize = false;

	if (displays_value) {
		label.Alignment = lbeChange->Alignment;
		label.Font = lbeChange->Font;
		label.Height = lbeChange->Height;
		label.Left = lbeChange->Left;
		label.Width = lbeChange->Width;
	} else {
		label.Alignment = lbChange->Alignment;
		label.Font = lbChange->Font;
		label.Height = lbChange->Height;
		label.Left = lbChange->Left;
		label.Width = lbChange->Width;
	}

	label.Visible = true;
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::PlaceLabels(TLabel &title_label, TLabel &value_label,  unsigned short &y_position)
{
	const unsigned short space_separating_title_and_value =
	  VertSpaceSeparatingTitleAndValueLabels();
	const unsigned short space_separating_label_row =
	  VertSpaceSeparatingTotalAndChange();

	title_label.Top = y_position + space_separating_title_and_value;
	value_label.Top = y_position;

	y_position -= value_label.Height + space_separating_label_row;
}
// ---------------------------------------------------------------------------
unsigned short TfrmSelectDish::VertSpaceSeparatingTitleAndValueLabels() const
{
	const unsigned short title = lbeTotal->Top;
	const unsigned short value = lbeTotalCost->Top;

	return std::max<unsigned short>(title, value)
	       - std::min<unsigned short>(title, value);
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(this);
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::CurrentItemChange(TItemRedirector *ItemRedirector)
{

	if (TDeviceRealTerminal::Instance().Menus->VisibleMenu != NULL)
	{
		if (ItemRedirector->ItemType.Contains(itSubItem))
		{
			if (ItemRedirector->ItemType.Contains(itPrevItem))
			{
				btnRemove->Enabled = false;
			}
			else
			{
				btnRemove->Enabled = true;
			}
		}
		else if (ItemRedirector->ItemType.Contains(itSetMenuMasterItem))
		{
			if (ItemRedirector->ItemType.Contains(itPrevItem))
			{
				btnRemove->Enabled = false;
			}
			else
			{
				btnRemove->Enabled = true;
			}
		}
		else if (ItemRedirector->ItemType.Contains(itPrevItem))
		{
			btnRemove->Enabled = false;
		}
		else if (ItemRedirector->ItemType.Contains(itOption))
		{
			btnRemove->Enabled = true;
		}
		else if (ItemRedirector->ItemType.Contains(itMembershipDisplay))
		{
            if(!(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot &&
                TGlobalSettings::Instance().EnableCustomerJourney))
			btnRemove->Enabled = true;
		}
		else if (ItemRedirector->ItemType.Contains(itServingCourseDisplay))
		{
			btnRemove->Enabled = false;
		}
		else if (ItemRedirector->ItemType.Contains(itNormalItem))
		{
			btnRemove->Enabled = true;
		}
		else if (ItemRedirector->ItemType.Contains(itDiscountDisplay))
		{
			btnRemove->Enabled = true;
		}
		else
		{
			btnRemove->Enabled = false;
		}
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::FormClose(TObject *Sender, TCloseAction &Action)
{
	/* Clear the display incase a Screen refresh causes a redraw
	and the items no longer exist. */
	lbDisplay->Clear();
	for (UINT i = 0; i < SeatOrders.size(); i++)
	{
		try
		{
			while (SeatOrders[i]->Orders->Count != 0)
			{
				TItemComplete *Item = SeatOrders[i]->Orders->Items[0];
				SeatOrders[i]->Orders->Remove(Item);
				delete Item;
                Item = NULL;

			}
			while (SeatOrders[i]->Orders->PrevCount != 0)
			{
				delete SeatOrders[i]->Orders->PrevItems[0];
				SeatOrders[i]->Orders->DeletePrev(0);
			}
		}
		__finally
		{
			SeatOrders[i]->Orders->Clear();
			SeatOrders[i]->Orders->AppliedMembership.Clear();
		}
	}
	tiClock->Enabled = false;

	WaitingForSwipe = false;
	Panel1->Enabled = true;
	tbtnTender->Enabled = true;
	tbtnSave->Enabled = true;
	tbtnCashSale->Enabled = true;

	CurrentTender = 0;
	tbtnTender->Caption = "Tender";
	tbtnCashSale->Caption = "Cash Sale";
	LastSale = 0;
	TDeviceRealTerminal::Instance().PoleDisplay->UpdatePoleDisplayDefault();
    if(TGlobalSettings::Instance().ItemSearch)
    {
        DefocusControl(tedtSearchItem, false);
    }
}
// ---------------------------------------------------------------------------
bool have_orders_been_taken(std::vector<TSeatOrders *> &seat_orders)
{
	bool orders_present = false;

	for (std::vector<TSeatOrders *>::size_type i = 0;
	     i != seat_orders.size() && orders_present == false; ++i)
	        orders_present = seat_orders[i]->Orders->Count != 0;

	return orders_present;
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::purge_unsent_orders()
{
    for (std::vector<TSeatOrders *>::size_type i = 0; i != SeatOrders.size(); ++i)
         {
            TContainerOrders &orders = *SeatOrders[i]->Orders;

            try {

                    while (orders.Count != 0)
                    {
                            TItemComplete &item = *orders.Items[0];
                            item.ReturnToAvailability();
                            orders.Remove(&item);
                            delete &item;
                    }

                    while (orders.PrevCount != 0)
                    {
                        delete orders.PrevItems[0];
                        orders.DeletePrev(0);
                    }
                 }
             __finally
             {
                orders.Clear();
                orders.ClearPrev();
                orders.AppliedMembership.Clear();
             }
         }
    patronsStore.clear();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::FormCloseQuery(TObject *, bool &can_close)
{
	can_close = have_orders_been_taken(SeatOrders) == false;

	if (can_close == false) {
		can_close =
		    MessageBox("Delete unsent orders?",
			       "Warning",
			       MB_YESNO | MB_ICONQUESTION)
		    == IDYES;
		if (can_close == false)
			return;

		purge_unsent_orders();
	}

	ResetPOS();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tbtnCashSaleClick(TObject *Sender)
{
    if(TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL)
    {
        TGlobalSettings::Instance().GiftCard_Megaworld = 0;
        TGlobalSettings::Instance().GiftCard_MegaworldForDaily = 0;
        TGlobalSettings::Instance().CheckSaleMegaworld =0;
        TGlobalSettings::Instance().CheckSaleMegaworldForDaily = 0;
    }
	if (CurrentTender != 0)
	{
		CurrentTender = 0;
		lbeChange->Caption = CurrToStrF(0, ffNumber, CurrencyDecimals) + " ";
		tbtnTender->Caption = "Tender";
		if (TDeviceRealTerminal::Instance().PaymentSystem->ForceTender)
		{
			tbtnCashSale->Caption = "Cash Sale Disabled";
			tbtnCashSale->Enabled = false;
		}
		else
		{
			tbtnCashSale->Caption = "Cash Sale";
			tbtnCashSale->Enabled = true;
		}
		TotalCosts();
		UpdateExternalDevices();
	}
	else
	{
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
		DBTransaction.StartTransaction();

        TPaymentTransaction PaymentTransaction(DBTransaction);
        PaymentTransaction.Type = eTransQuickSale;
        TDeviceRealTerminal::Instance().PaymentSystem->PaymentsReload(PaymentTransaction);
        TPayment *CashPayment = PaymentTransaction.PaymentFind(CASH);

        if(SaveTransactionDetails(CashPayment->Name))
        {
            bool PaymentComplete = ProcessOrders(Sender, DBTransaction, 0, // Tab
                TabCashAccount, // Tab Type
                "Sale", // Tab Container Name
                "Sale", // Tab Name
                "", // PartyName
                false, // Print Prelim Receipt.
                0, // Table
                0, // Seat
                0); // Room
            TStringList* logList = new TStringList();
            logList->Clear();

            if (PaymentComplete)
            {
                logList->Add("Trabsaction commit of tbtnCashSaleClick().");
                TSaveLogs::RecordFiscalLogs(logList);
                DBTransaction.Commit();
                ResetPOS();
            }
            else
            {
                logList->Add("payment not completed in tbtnCashSaleClick().");
                TSaveLogs::RecordFiscalLogs(logList);
            }
            delete logList;
            logList = NULL;
        }
        DisplayRoomNoUI();
	}
    AutoLogOut();
    if(TGlobalSettings::Instance().EnableTableDisplayMode)
    {
          showTablePicker();
    }
    if(IsChitPromptFormActive)
    {
     NagUserToSelectChit();
    }
    CheckMandatoryMembershipCardSetting(tbtnMembership);
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::RedrawSeatOrders()
{
	SeatOrders[SelectedSeat]->Orders->RefreshDisplay() ;
	lbDisplay->Clear();

	for (int i = 0; i < SeatOrders[SelectedSeat]->Orders->CompressedCount; i++)
	{
		TItemsCompleteCompressed* CompressedItems = SeatOrders[SelectedSeat]->Orders->CompressedItems[i];
 		lbDisplay->Items->AddStrings(CompressedItems->Display);
	}

	if (SeatOrders[SelectedSeat]->Orders->AppliedMembership.ContactKey != 0)
	{
		tbtnMembership->Caption = "Membership Applied :";
           tbtnMembership->ButtonColor = clNavy;
	}
	else
	{
		tbtnMembership->Caption = "Membership";
	}
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::UpdateExternalDevices(bool UpdateTopPoleDisplay, bool UpdateBottomPoleDisplay)
{
    try
    {
        AnsiString PoleDisplayItem;
        Currency PoleDisplayItemPrice;
        for (int i = 0; i < SeatOrders[SelectedSeat]->Orders->Count; i++)
        {
            TItemComplete *Order = SeatOrders[SelectedSeat]->Orders->Items[i];

            if (SeatOrders[SelectedSeat]->Orders->LastItemAdded == Order)
            {
                PoleDisplayItemPrice = Order->TotalPriceAdjustment();
                PoleDisplayItem = Order->Item;
            }
            else
            {
                for (UINT j = 0; j < Order->SubOrders->Count; j++)
                {
                    TItemCompleteSub *SubOrder = (TItemCompleteSub*)Order->SubOrders->Items[j];
                    if (SeatOrders[SelectedSeat]->Orders->LastItemAdded == SubOrder)
                    {
                        PoleDisplayItemPrice = SubOrder->TotalPriceAdjustmentSides();
                        PoleDisplayItem = SubOrder->Item;
                    }
                }
            }
        }
        AnsiString PoleDisplayPrice = CurrToStrF(PoleDisplayItemPrice, ffNumber, CurrencyDecimals);
        AnsiString StringTotal = CurrToStrF(InitialMoney.GrandTotal, ffNumber, CurrencyDecimals);

        if (!UpdateTopPoleDisplay)
        {
            PoleDisplayItem = "";
            PoleDisplayPrice = "";
        }
        AnsiString BottomLeft = "";
        if (!UpdateBottomPoleDisplay)
        {
            BottomLeft = "";
            StringTotal = "";
        }

        if (SeatOrders[SelectedSeat]->Orders->Count != 0)
        {
            BottomLeft = "Sub Total ";
            TDeviceRealTerminal::Instance().PoleDisplay->UpdatePoleDisplay(PoleDisplayItem, PoleDisplayPrice, BottomLeft, StringTotal);
            TDeviceRealTerminal::Instance().SecurityPort->SetData(PoleDisplayItem + " " + PoleDisplayPrice);
            TDeviceRealTerminal::Instance().SecurityPort->SetData(BottomLeft + StringTotal);
            LastSale = 0;
        }
    }
    catch(Exception & E)
    {
         TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::TotalCosts()
{
    try
    {
        InitialMoney.Clear();
        if(ChitNumber.Valid())
        {
           AssignremovedTaxesList();
           AssignDiscountLists();
           HighlightSelectedItem();
        }
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction.StartTransaction();
        TPaymentTransaction PaymentTransaction(DBTransaction);

        TDeviceRealTerminal::Instance().PaymentSystem->PaymentsLoadTypes(PaymentTransaction);
        DBTransaction.Commit();
        std::auto_ptr<TList>OrdersList(new TList);

        for (int i = 0; i < SeatOrders[SelectedSeat]->Orders->Count; i++)
            {
                    TItemMinorComplete *Order = (TItemMinorComplete*)SeatOrders[SelectedSeat]->Orders->Items[i];
                    OrdersList->Add(Order);
            }

        PaymentTransaction.Orders->Assign(OrdersList.get());
        PaymentTransaction.Membership.Assign(Membership);
        PaymentTransaction.ProcessPoints();



        InitialMoney.Recalc(PaymentTransaction,false);

        if (InitialMoney.PaymentDue != 0)
        {
            if (CurrentTender != 0)
            {
                InitialMoney.PaymentAmount = CurrentTender;
                TPayment *CashPayment = PaymentTransaction.PaymentFind(CASH);
                if (CashPayment != NULL)
                {
                    CashPayment->SetPay(CurrentTender);
                }
                InitialMoney.Recalc(PaymentTransaction,false);
            }
        }

        UpdateTaxLabels();

            customerDisp.TierLevel=     TGlobalSettings::Instance().TierLevelChange ;

        lbeTotalCost->Caption = CurrToStrF(InitialMoney.RoundedGrandTotal, ffNumber, CurrencyDecimals) + " ";
        lbeChange->Caption    = CurrToStrF(RoundToNearest(InitialMoney.Change, 0.01, TGlobalSettings::Instance().MidPointRoundsDown), ffNumber, CurrencyDecimals) + " ";

        customerDisplaySendOrder( SeatOrders );
    }
    catch(Exception & E)
    {
         TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }

}
// ---------------------------------------------------------------------------
__fastcall TDataBtn::TDataBtn(Classes::TComponent* AOwner) : TTouchBtn(AOwner) // TCustomStaticText(AOwner)
{
	Item = NULL;
	Font->Color = clBlack;
	Font->Name = "Arial";
	Font->Size = frmSelectDish->tgridOrderCourse->Font->Size;
	Font->Style = TFontStyles() << fsBold;
	BevelInner = bvNone;
	// BevelOuter	 		= bvNone;
	// Item->SpecialID	= 0;
}
// ---------------------------------------------------------------------------
__fastcall TDataBtn::~TDataBtn()
{
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tmPosRefreshTimer(TObject *Sender)
{
	if (TDeviceRealTerminal::Instance().DBControl.Connected()) // If we not connected dont bother.
	{
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();
		if (!OrdersPending() && !OrdersParked(DBTransaction) && TDeviceRealTerminal::Instance().Menus->New->Count > 0)
		{
			TDeviceRealTerminal::Instance().Menus->SwapInNewMenus();
			TDeviceRealTerminal::Instance().Menus->SetMenuList(DBTransaction, TDeviceRealTerminal::Instance().ID.DeviceKey);
		}

		DBTransaction.Commit();
        if((!IsSubSidizeProcessed)  &&!IsParkSalesEnable )
        setParkedSalesBtnColor();

		if (TDeviceRealTerminal::Instance().Menus->VisibleMenu != NULL && !OrdersPending() && TDeviceRealTerminal::Instance().Menus->VisibleMenu->EnabledStateSync != TDeviceRealTerminal::Instance()
			.Menus->UpdateSync[TDeviceRealTerminal::Instance().Menus->VisibleMenu->MenuKey])
		{
			TDeviceRealTerminal::Instance().Menus->UpdateSync[TDeviceRealTerminal::Instance().Menus->VisibleMenu->MenuKey] = TDeviceRealTerminal::Instance().Menus->VisibleMenu->EnabledStateSync;
			RedrawMenu();
		}
              // BarExachange barexchang;
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tiChitDelayComplete(TObject *)
{
    tiChitDelay->Enabled = false;
    if(Screen->ActiveForm->ClassNameIs("TfrmSelectDish"))
    {     
        Database::TDBTransaction t(TDeviceRealControl::ActiveInstance().DBControl);
        t.StartTransaction();
            SetupChit(t);
        t.Commit();
    }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tiClockTimer(TObject *Sender)
{
    try
    {
        TCHAR szTime[64];
        //std::<Initializer_list> t1;     for (auto itr = myvec.cbegin(); itr != myvec.cend(); ++itr)
        int Length = GetTimeFormat(NULL, NULL, NULL, _T("h':'mm':'ss tt"), szTime, sizeof(szTime));
        AnsiString TheTime = " ";
        for (int i = 0; i < Length; i++)
        {
            TheTime += szTime[i];
        }
        AnsiString LastTime = stTime->Caption.Trim();
        if (LastTime != TheTime.Trim())
        {
            stTime->Caption = TheTime;
            Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
            DBTransaction.StartTransaction();
            std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
            UserForceHappyHourRight = Staff->TestAccessLevel( TDeviceRealTerminal::Instance().User, CheckAllowForcedHappyHour);
            DBTransaction.Commit();

            if (TGlobalSettings::Instance().ForceHappyHour)
            {
                stHappyHour->Visible = true;
            }
            else
            {
                //check if current time fall under any profile then it will return true;
                TManagerHappyHour* isHappyHour = new TManagerHappyHour();
                int priceLevel;
                stHappyHour->Tag=1;
                 bool happyHour = isHappyHour->IsCurrentTimeHappyHour(TDeviceRealTerminal::Instance().ID.DeviceKey,priceLevel);

                if (!TGlobalSettings::Instance().TerminalExemptFromHappyHour)
                {
                    TDateTime CurrentTime = Time();
                    bool InTimeSpan;

                    if (happyHour)
                    {
                        InTimeSpan = true;
                        stHappyHour->Visible = true;
                        stHappyHour->Tag=priceLevel;
                    }
                    else
                    {
                        stHappyHour->Visible = false;

                    }
                }
                else
                {
                    stHappyHour->Visible = false;
                }

               delete isHappyHour;
               isHappyHour = NULL;
            }
        }

        // Rest poledisplay.
        if (Now() > (LastSale + (1.0 / 24.0 / 60.0 / 6.0)) && double(LastSale) != 0) // 10 seconds.
        {
            TDeviceRealTerminal::Instance().PoleDisplay->UpdatePoleDisplayDefault();
            LastSale = 0;
        }

        if(TGlobalSettings::Instance().WebMateEnabled && TDeviceRealTerminal::Instance().DBControl.Connected())
        {
            //Check For Web Orders.
            Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
           try{  DBTransaction.StartTransaction();
            bool WebOrdersPending = TDBWebUtil::WebOrdersPending(DBTransaction);
            //notify message for webmate interface is enabled or not
            if(!NotifyLastWebOrder(DBTransaction))
            {
                if (WebOrdersPending)
                {
                    if(TGlobalSettings::Instance().AutoAcceptWebOrders)
                    {
                        ProcessWebOrders(false);
                    }
                    else
                    {
                       tbtnWebOrders->ButtonColor= clGreen;
                       tbtnWebOrders->Font->Color =clWhite;
                    }
                }
                else
                {
                     if(TGlobalSettings::Instance().ShowDarkBackground)
                     {
                        tbtnWebOrders->ButtonColor = 14342874;
                     }
                     else
                     {
                        tbtnWebOrders->ButtonColor = clWhite;
                     }
                     tbtnWebOrders->Font->Color =0x002193F6;
                }
            }
            DBTransaction.Commit();
            }
               catch(Exception &E)
                {
                  DBTransaction.Rollback();
                   TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
                }
        }

        // Mall Export Codes
        bool CloseSelectDish = false;
        std::auto_ptr<TMallExportManager> MEM(new TMallExportManager());

        if(TGlobalSettings::Instance().MallIndex == ROBINSONMALL)
        {
            CloseSelectDish = MEM->IMallManager->ScheduledExitOnSelectDish();

            if(CloseSelectDish)
            {
                Close();
            }
        }
    }
    catch(Exception & E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::lbDisplayDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State)
{

    static int CL_STANDARD = clBlack;
	static int CL_STANDARD_OPTION = clNavy;
	static int CL_STANDARD_SUB = clTeal;
	static int CL_STANDARD_SETMENU_KEY = clMaroon;
	static int CL_STANDARD_SETMENU_ITEM = clBlack;
	static int CL_STANDARD_MEMBER_INFO = clWhite;
	static int CL_STANDARD_DISCOUNT_INFO = clPurple;

	static int CL_PREVIOUS = RGB(90, 90, 90);
	static int CL_PREVIOUS_OPTION = RGB(90, 90, 90);
	static int CL_PREVIOUS_SUB = RGB(90, 90, 90);
	static int CL_PREVIOUS_SETMENU_KEY = clMaroon;
	static int CL_PREVIOUS_SETMENU_ITEM = RGB(90, 90, 90);

	TItemRedirector *ItemRedirector = (TItemRedirector*)lbDisplay->Items->Objects[Index];
    bool MidPointRoundsDown = TGlobalSettings::Instance().MidPointRoundsDown;
    Currency DollarValue = Currency(0);
	UnicodeString DollarAmount = CurrToStrF(0, ffNumber, CurrencyDecimals);
	bool DiscountApplied = false;
	if (!ItemRedirector)
	{
		return;
	}
	TCanvas *pCanvas = ((TListBox*)Control)->Canvas;
	TColor Color = pCanvas->Brush->Color;
	TBrushStyle Style = pCanvas->Brush->Style;
	pCanvas->FillRect(Rect); // clear the rectangle

	if (State.Contains(odSelected))
	{
		pCanvas->Font->Style = TFontStyles() << fsBold;
		pCanvas->Font->Color = (TColor)CL_STANDARD_MEMBER_INFO;
		pCanvas->Brush->Color = CL_BUTTONCOLOUR_SELECTED_SEAT;
		pCanvas->Brush->Style = bsSolid;
		pCanvas->FillRect(Rect); // clear the rectangle
	}

	if (ItemRedirector->ItemType.Contains(itPrevItem))
	{
		if (ItemRedirector->ItemType.Contains(itSubItem))
		{
			TItemMinorComplete *SubItem = (TItemMinorComplete*)ItemRedirector->ItemObject;
            DollarValue = RoundToNearest(ItemRedirector->CompressedContainer->Count * SubItem->TotalPriceAdjustment(), 0.01, MidPointRoundsDown);
			DollarAmount = CurrToStrF(DollarValue, ffNumber, CurrencyDecimals);
			DiscountApplied = (SubItem->DiscountSize() != 0);
			pCanvas->Font->Style = TFontStyles();
			if (!State.Contains(odSelected))
				pCanvas->Font->Color = (TColor)CL_PREVIOUS_SUB;
		}
		else if (ItemRedirector->ItemType.Contains(itOption))
		{
			DollarAmount = "";
			pCanvas->Font->Style = TFontStyles();
			if (!State.Contains(odSelected))
				pCanvas->Font->Color = (TColor)CL_PREVIOUS_OPTION;
		}
		else if (ItemRedirector->ItemType.Contains(itPromoItem))
		{
			DollarAmount = "";
			pCanvas->Font->Style = TFontStyles();
			if (!State.Contains(odSelected))
				pCanvas->Font->Color = (TColor)CL_PREVIOUS_SETMENU_ITEM;
		}
		else if (ItemRedirector->ItemType.Contains(itSetMenuMasterItem))
		{
			TItemComplete *Order = (TItemComplete*)ItemRedirector->CompressedContainer->ItemsList[0];
            DollarValue = RoundToNearest(ItemRedirector->CompressedContainer->Count * Order->TotalPriceAdjustment(), 0.01, MidPointRoundsDown);
			DollarAmount = CurrToStrF(DollarValue, ffNumber, CurrencyDecimals);

			DiscountApplied = (Order->TotalDiscount() != 0);
			pCanvas->Font->Style = TFontStyles();
			if (!State.Contains(odSelected))
				pCanvas->Font->Color = (TColor)CL_PREVIOUS_SETMENU_KEY;
		}
		else
		{
			TItemMinorComplete *PrevOrder = (TItemMinorComplete*)ItemRedirector->ItemObject;
            DollarValue = RoundToNearest(ItemRedirector->CompressedContainer->Count * PrevOrder->TotalPriceAdjustment(), 0.01, MidPointRoundsDown);
			DollarAmount = CurrToStrF(DollarValue, ffNumber, CurrencyDecimals);

			pCanvas->Font->Style = TFontStyles();
			if (!State.Contains(odSelected))
				pCanvas->Font->Color = (TColor)CL_PREVIOUS;
		}
	}
	else
	{
		if (ItemRedirector->ItemType.Contains(itSubItem))
		{
			TMMContactInfo Member =
              SeatOrders[SelectedSeat]->Orders->AppliedMembership;

			TItemMinorComplete *SubItem = (TItemMinorComplete*)ItemRedirector->ItemObject;
            DollarValue = RoundToNearest(ItemRedirector->CompressedContainer->Count * SubItem->TotalPriceAdjustment(), 0.01, MidPointRoundsDown);
			DollarAmount = CurrToStrF(DollarValue, ffNumber, CurrencyDecimals);
			DiscountApplied = (SubItem->DiscountSize() != 0);

			pCanvas->Font->Style = TFontStyles() << fsBold;
			if (!State.Contains(odSelected))
				pCanvas->Font->Color = (TColor)CL_STANDARD_SUB;
		}
		else if (ItemRedirector->ItemType.Contains(itOption))
		{
			DollarAmount = "";
			pCanvas->Font->Style = TFontStyles();
			if (!State.Contains(odSelected))
				pCanvas->Font->Color = (TColor)CL_STANDARD_OPTION;
		}
		else if (ItemRedirector->ItemType.Contains(itPromoItem))
		{
			DollarAmount = "";
			pCanvas->Font->Style = TFontStyles() << fsBold;
			if (!State.Contains(odSelected))
				pCanvas->Font->Color = (TColor)CL_STANDARD_SETMENU_ITEM;
		}
		else if (ItemRedirector->ItemType.Contains(itMembershipDisplay))
		{
            if(SeatOrders[SelectedSeat]->Orders->AppliedMembership.ContactKey)
            {
                DollarAmount = "Pts " + FormatFloat("0.00", ItemRedirector->CompressedContainer->Container->AppliedMembership.Points.getPointsBalance(pasDatabase));
            }
            else if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot &&
                TGlobalSettings::Instance().EnableCustomerJourney )
            {
		 	    DollarAmount = "";
            }
			pCanvas->Font->Style = TFontStyles() << fsBold;
			pCanvas->Font->Color = (TColor)CL_STANDARD_MEMBER_INFO;
			pCanvas->Brush->Color = clGreen;
			pCanvas->Brush->Style = bsSolid;
			pCanvas->FillRect(Rect); // clear the rectangle
		}
		else if (ItemRedirector->ItemType.Contains(itEarntPts))
		{
			DollarAmount = FormatFloat("0.00", ItemRedirector->CompressedContainer->Container->AppliedMembership.Points.getPointsBalance(ptstLoyalty));

			pCanvas->Font->Style = TFontStyles() << fsBold;
			pCanvas->Font->Color = (TColor)CL_STANDARD_MEMBER_INFO;
			pCanvas->Brush->Color = clGreen;
			pCanvas->Brush->Style = bsSolid;
			pCanvas->FillRect(Rect); // clear the rectangle
		}
		else if (ItemRedirector->ItemType.Contains(itLoadedPts))
		{
			DollarAmount = FormatFloat("0.00", ItemRedirector->CompressedContainer->Container->AppliedMembership.Points.getPointsBalance(ptstAccount));

			pCanvas->Font->Style = TFontStyles() << fsBold;
			pCanvas->Font->Color = (TColor)CL_STANDARD_MEMBER_INFO;
			pCanvas->Brush->Color = clGreen;
			pCanvas->Brush->Style = bsSolid;
			pCanvas->FillRect(Rect); // clear the rectangle
		}
		else if (ItemRedirector->ItemType.Contains(itMembershipDisplayNote))
		{
			DollarAmount = "";

			pCanvas->Font->Style = TFontStyles() << fsBold;
			pCanvas->Font->Color = (TColor)CL_STANDARD_MEMBER_INFO;
			pCanvas->Brush->Color = clGreen;
			pCanvas->Brush->Style = bsSolid;
			pCanvas->FillRect(Rect); // clear the rectangle
		}
		else if (ItemRedirector->ItemType.Contains(itDiscountDisplay))
		{
			Currency *Discount = (Currency*)ItemRedirector->ItemObject;
            *Discount = RoundToNearest(*Discount, 0.01, MidPointRoundsDown);
			DollarAmount = CurrToStrF(*Discount, ffNumber, CurrencyDecimals);
			pCanvas->Font->Style = TFontStyles() << fsBold;
			pCanvas->Font->Color = (TColor)CL_STANDARD_MEMBER_INFO;
			pCanvas->Brush->Color = (TColor)CL_STANDARD_DISCOUNT_INFO;
			pCanvas->Brush->Style = bsSolid;
			pCanvas->FillRect(Rect); // clear the rectangle
		}
		else if (ItemRedirector->ItemType.Contains(itSetMenuMasterItem))
		{
			TItemComplete *Order = (TItemComplete*)ItemRedirector->CompressedContainer->ItemsList[0];
            DollarValue = RoundToNearest(ItemRedirector->CompressedContainer->Count * Order->TotalPriceAdjustment(), 0.01, MidPointRoundsDown);
			DollarAmount = CurrToStrF(DollarValue, ffNumber, CurrencyDecimals);

			DiscountApplied = (Order->DiscountSize() != 0);
			pCanvas->Font->Style = TFontStyles() << fsBold;
			if (!State.Contains(odSelected))
				pCanvas->Font->Color = (TColor)CL_STANDARD_SETMENU_KEY;
		}
		else if (ItemRedirector->ItemType.Contains(itServingCourseDisplay))
		{

                if(TGlobalSettings::Instance().ShowCurrencySymbol)
                {
                    DollarAmount = CurrencyString;
                }
                else
                {
                    DollarAmount = "";
                }
   		            pCanvas->Font->Style = TFontStyles() << fsBold;
			        pCanvas->Font->Color = ButtonColors[BUTTONTYPE_FULL][ATTRIB_FONTCOLOR];
 			        pCanvas->Brush->Color = ButtonColors[BUTTONTYPE_FULL][ATTRIB_BUTTONCOLOR];
 			        pCanvas->Brush->Style = bsSolid;
			        pCanvas->FillRect(Rect); // clear the rectangle
		}
		else
		{
			TItemMinorComplete *SubItem = (TItemMinorComplete*)ItemRedirector->ItemObject;
			/*
			 * To calculate DollarValue use ItemRedirector->CompressedContainer->Count instead of Qty
			 * The count is number of times an item appears in the lbDisplay
			 * With TotalPriceAdjustment() you get ItemPrice*Qty, hence no need to multiply it again with Qty
			 */
            DollarValue = RoundToNearest(ItemRedirector->CompressedContainer->Count * SubItem->TotalPriceAdjustment(), 0.01, MidPointRoundsDown);
			DollarAmount = CurrToStrF(DollarValue, ffNumber, CurrencyDecimals);
			DiscountApplied = (SubItem->DiscountSize() != 0);

			    pCanvas->Font->Style = TFontStyles() << fsBold;
			    if (!State.Contains(odSelected))
				pCanvas->Font->Color = (TColor)CL_STANDARD;
                btnRemove->Enabled = true;

		}
    }
//    }
	int TextWidth = Rect.right - Rect.left;
	int DollarWidth = TextWidth / 4;

	if (DollarAmount == "")
		DollarWidth = 0;

	Rect.right = Rect.left + (TextWidth - DollarWidth);

	UnicodeString Caption = ((TListBox*)Control)->Items->Strings[Index];
    memberName = ((TListBox*)Control)->Items->Strings[0];
    memberDetails = Caption;

	int TextHeight = DrawTextExW(pCanvas->Handle, Caption.w_str(), Caption.Length(), &TRect(Rect.left, Rect.top, Rect.right, Rect.bottom), DT_CALCRECT | DT_WORDBREAK | DT_NOPREFIX, NULL) + 4;


	TRect CentredRect = Rect;
	int RowHeight = Rect.Bottom - Rect.Top;
	if (RowHeight > TextHeight)
	{
		CentredRect.Top += (RowHeight - TextHeight) / 2;
		CentredRect.Bottom = CentredRect.Top + TextHeight;
	}

	DrawTextExW(pCanvas->Handle, Caption.w_str(), Caption.Length(), &CentredRect, DT_WORDBREAK | DT_NOPREFIX, NULL);


	if (DollarAmount != "")
	{
		int DollarTextHeight = DrawTextExW(pCanvas->Handle, DollarAmount.c_str(), DollarAmount.Length(), &TRect(Rect.right, Rect.top, Rect.right + DollarWidth - 5, Rect.bottom),
			DT_CALCRECT | DT_WORDBREAK | DT_NOPREFIX, NULL) + 4;

		TRect DollarCentredRect = Rect;
		int RowHeight = Rect.Bottom - Rect.Top;
		if (RowHeight > DollarTextHeight)
		{
			DollarCentredRect.Top += (RowHeight - DollarTextHeight) / 2;
			DollarCentredRect.Bottom = DollarCentredRect.Top + DollarTextHeight;
		}

		TRect DollarRect = DollarCentredRect;
		DollarRect.Left = Rect.right;
		DollarRect.right = DollarRect.Left + DollarWidth - 5;

		if (DiscountApplied)
		{
			pCanvas->Font->Style = TFontStyles() << fsBold;
			pCanvas->Font->Color = (TColor)CL_STANDARD_MEMBER_INFO;
			pCanvas->Brush->Color = (TColor)CL_STANDARD_DISCOUNT_INFO;
			pCanvas->Brush->Style = bsSolid;
			pCanvas->FillRect(DollarRect); // clear the rectangle
		}
		DrawTextExW(pCanvas->Handle, DollarAmount.w_str(), DollarAmount.Length(), &DollarRect, DT_WORDBREAK | DT_NOPREFIX | DT_RIGHT, NULL);

	}

	pCanvas->Brush->Color = Color;
	pCanvas->Brush->Style = Style;

   /* // code for disabling the cash button if Mandatory Membership Card seting in maintance settings is true
          if(TGlobalSettings::Instance().MandatoryMembershipCard)
             {
                  tbtnCashSale->Enabled = false;
             }
              else
             {
                 tbtnCashSale->Enabled = true;

             }
                   */


 }
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tbtnTenderClick(TObject *Sender)
{
    IsParkSalesEnable = true;
	IsSubSidizeOrderCancil=false;
	customerDisp.HappyBirthDay = false;
	customerDisp.FirstVisit = false;
    TGlobalSettings::Instance().IsThorPay = true;
    isRoomNoUiCalled = false;
    tabKey=0;
    bool showRoomNoUI = true;
	if (CurrentTender != 0)
	{
		if (InitialMoney.PaymentDue > 0)
		{
            showRoomNoUI = false;
			MessageBox("You must tender at least the total price of the goods. \r " + CurrToStrF(InitialMoney.PaymentDue, ffNumber, CurrencyDecimals) + " Still Owing", "Error",
				MB_OK + MB_ICONERROR);
		}
		else
		{
			Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
			TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
			DBTransaction.StartTransaction();
			bool PaymentComplete = ProcessOrders(Sender, DBTransaction, 0, // Tab
				TabCashAccount, // Tab Type
				TDeviceRealTerminal::Instance().ID.Name + CASH_ACCOUNT, // Tab Container Name
				TDeviceRealTerminal::Instance().ID.Name + CASH_ACCOUNT, // Tab Name
				"", // PartyName
				false, // Print Prelim Receipt.
				0, // Table
				0, // Seat
				0); // Room
            TStringList* logList = new TStringList();
            logList->Clear();
			if (PaymentComplete)
			{
                logList->Add("Trabsaction commit of tbtnTenderClick().");
                TSaveLogs::RecordFiscalLogs(logList);

				DBTransaction.Commit();
				ResetPOS();
			}
            else
            {
                logList->Add("payment not completed received in tbtnTenderClick().");
                TSaveLogs::RecordFiscalLogs(logList);
            }
            delete logList;
            logList = NULL;
		}
	}
	else
	{
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
		DBTransaction.StartTransaction();
		bool PaymentComplete = ProcessOrders(Sender, DBTransaction, 0, // Tab
			TabCashAccount, // Tab Type
			TDeviceRealTerminal::Instance().ID.Name + CASH_ACCOUNT, // Tab Container Name
			TDeviceRealTerminal::Instance().ID.Name + CASH_ACCOUNT, // Tab Name
			"", // PartyName
			false, // Print Prelim Receipt.
			0, // Table
			0, // Seat
			0); // Room

        TStringList* logList = new TStringList();
        logList->Clear();

		if (PaymentComplete)
		{
            logList->Add("Trabsaction commit of tbtnTenderClick() part 2.");
            TSaveLogs::RecordFiscalLogs(logList);

			DBTransaction.Commit();
			ResetPOS();
		}
		if(!PaymentComplete)
		{
			TSaveOrdersTo OrderContainer;
			int tempTabkey;
			tempTabkey=tabKey;
			AnsiString  tabName   = TDBTab::GetTabName(DBTransaction,tabKey);
			bool IsSusidizedPaymentType=TDBTab::GetTabDiscountExists(DBTransaction ,tabKey);

			if(tabKey>0 && tabName!=""&&!IsSubSidizeProcessed )
			{

				if(IsSusidizedPaymentType)
				{
					SubsidizedDiscountApply(tabKey);
				}
				//  SubsidizedDiscountApply(tabKey);

				IsSubSidizeProcessed=false;
				OrderContainer.Location["TabKey"] = tempTabkey;
				OrderContainer.Location["TMMTabType"] = TabNormal;
				OrderContainer.Location["TMMDisplayMode"] = eTabs;
				OrderContainer.Location["ContainerName"] = tabName;
				OrderContainer.Location["TabName"] = tabName;
				OrderContainer.Location["PartyName"] = "";
				OrderContainer.Location["SelectedSeat"] = 0;
				OrderContainer.Location["SelectedTable"] = 0;
				OrderContainer.Location["RoomNumber"] = 0;
               if(!IsSubSidizeOrderCancil)
				SaveTabData(OrderContainer);
                else
                {
                    ManagerDiscount->ClearDiscounts(SeatOrders[SelectedSeat]->Orders->List);
                    RedrawSeatOrders();
                    TotalCosts();
                }
				ModalResult = mrCancel;
			}
			else
			{   if(IsSubSidizeProcessed)
				IsSubSidizeOrderCancil=true;
				IsSubSidizeProcessed=false;
			}
             logList->Add("payment not completed received in tbtnTenderClick() part 2");
                TSaveLogs::RecordFiscalLogs(logList);
		}
        delete logList;
        logList = NULL;
	}

	if(!IsSubSidizeProcessed&&!IsSubSidizeOrderCancil)
	{
        AutoLogOut();
		if(TGlobalSettings::Instance().EnableTableDisplayMode)
		{
			showTablePicker();
		}

        if(showRoomNoUI)
            DisplayRoomNoUI();

		//MM-1647: Ask for chit if it is enabled for every order.
            if(IsChitPromptFormActive)
            {
              	NagUserToSelectChit();
            }

	}

    IsParkSalesEnable=false;
    TDrinkCommandData::Instance().ItemDetailsReceived.DeregisterForEvent(OnItemDetailsReceived);

}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::FormHide(TObject *Sender)
{
	tiClock->Enabled = false;

    customerDisplayReset();
    stopCustomerDisplayServer();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::SetLocation(eLocation Value)
{
	if (Value != fLocation)
	{
		fLocation = Value;
		PosRefresh = true;
	}
}
// ---------------------------------------------------------------------------
bool TfrmSelectDish::LoyaltyPending()
{
	bool LoyaltyPending = false;
	for (UINT i = 0; i < SeatOrders.size(); i++)
	{
		if (SeatOrders[i]->Orders->AppliedMembership.ContactKey != 0)
		{
			LoyaltyPending = true;
			break;
		}
	}
	return LoyaltyPending;
}
// ---------------------------------------------------------------------------
bool TfrmSelectDish::OrdersPending()
{
	bool OrdersTaken = false;
	for (UINT i = 0; i < SeatOrders.size(); i++)
	{
		if (SeatOrders[i]->Orders->Count > 0)
		{
			OrdersTaken = true;
			break;
		}
	}
	return OrdersTaken;
}
// ---------------------------------------------------------------------------
bool TfrmSelectDish::OrdersParked(Database::TDBTransaction &DBTransaction)
{
	bool OrdersParked = false;
	if (ParkedSales->GetCount(DBTransaction) > 0)
	{
		OrdersParked = true;
	}
	return OrdersParked;
}
// ---------------------------------------------------------------------------
bool TfrmSelectDish::DeleteAllUnsentAndProceed(Database::TDBTransaction &DBTransaction)
{
	bool retval = true;
	if (OrdersPending() || OrdersParked(DBTransaction))
	{
        isRoomNoUiCalled = false;
        IsAutoLogOutInSelectDish = false;
		if (MessageBox("Delete Parked and unsent orders?", "Warning", MB_YESNO + MB_ICONQUESTION) == IDYES)
		{
			ClearAllParkedSales(DBTransaction);
			/* Clear the display incase a Screen refresh causes a redraw
			and the items no longer exist. */
			lbDisplay->Clear();
			for (UINT i = 0; i < SeatOrders.size(); i++)
                                TManagerFreebie::UndoFreeCount(DBTransaction, SeatOrders[i]->Orders->List);

                        purge_unsent_orders();

                        Membership.Clear();
			TDBSaleTimes::VoidSaleTime(DBTransaction, CurrentTimeKey);
			ResetPOS();
		}
		else
		{
			retval = false;
		}
	}
	else
	{
		for (UINT i = 0; i < SeatOrders.size(); i++)
		{
			try
			{
				while (SeatOrders[i]->Orders->PrevCount != 0)
				{
					delete SeatOrders[i]->Orders->PrevItems[0];
					SeatOrders[i]->Orders->DeletePrev(0);
				}
			}
			__finally
			{
				SeatOrders[i]->Orders->ClearPrev();
				SeatOrders[i]->Orders->AppliedMembership.Clear();
				Membership.Clear();
			}
		}
		ResetPOS();
	}
	return retval;
}
// ---------------------------------------------------------------------------
bool TfrmSelectDish::DeleteUnsentAndProceed(Database::TDBTransaction &DBTransaction)
{
	bool retval = true;
    bool chcekitems = true;

	if (OrdersPending())
	{
		if (MessageBox("Delete unsent orders?", "Warning", MB_YESNO + MB_ICONQUESTION) == IDYES)
		{
           chcekitems = CheckItemsPrintCancel();
           patronsStore.clear();
           if(!chcekitems)
           {
              TMMContactInfo TempUserInfo;
              TempUserInfo = TDeviceRealTerminal::Instance().User;
              std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
              chcekitems = Staff->TestAccessLevel(TempUserInfo, CheckCredit);
              AnsiString Username;
              if(!chcekitems)
              {
                TempUserInfo.Clear();
                TLoginSuccess Result = Staff->Login(this, DBTransaction, TempUserInfo, CheckCredit);
                if (Result == lsAccepted)
                {
                  chcekitems = true;

                }
                else if (Result == lsDenied)
                {
                   chcekitems = false;
                   retval = false;
                   MessageBox("You do not have access rights to Cancels.", "Error", MB_OK + MB_ICONERROR);
                }
                else if (Result == lsPINIncorrect)
                {
                  chcekitems = false;
                  retval = false;
                  MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
                }
                else if (Result == lsCancel)
                {
                  chcekitems = false;
                  retval = false;
                }
              }
              if(chcekitems)
              {
                Username = TempUserInfo.Name;
                SaveRemoveItems(Username);
              }
           }

            if(chcekitems)
            {
                lbDisplay->Clear();
                for (UINT i = 0; i < SeatOrders.size(); i++)
                {
                    try
                    {
                        SeatOrders[i]->Orders->pmsAccountDetails.RoomNumber = "";
                        SeatOrders[i]->Orders->pmsAccountDetails.RoomBedNumber = "";
                        SeatOrders[i]->Orders->pmsAccountDetails.FirstName = "";
                        SeatOrders[i]->Orders->pmsAccountDetails.LastName = "";
                        TManagerFreebie::UndoFreeCount(DBTransaction, SeatOrders[i]->Orders->List);
                        while (SeatOrders[i]->Orders->Count != 0)
                        {
                            SeatOrders[i]->Orders->Items[0]->RoomNo = 0;
                            SeatOrders[i]->Orders->Items[0]->RoomNoStr = "";
                            SeatOrders[i]->Orders->Items[0]->FirstName = "";
                            SeatOrders[i]->Orders->Items[0]->LastName = "";
                            TItemComplete *Item = SeatOrders[i]->Orders->Items[0];
                            Item->ReturnToAvailability(Item->GetQty());

                            SeatOrders[i]->Orders->Remove(Item);
                            delete Item;
                            Item = NULL;
                        }
                        while (SeatOrders[i]->Orders->PrevCount != 0)
                        {
                            SeatOrders[i]->Orders->PrevItems[0]->RoomNo = 0;
                            SeatOrders[i]->Orders->PrevItems[0]->RoomNoStr = "";
                            SeatOrders[i]->Orders->PrevItems[0]->FirstName = "";
                            SeatOrders[i]->Orders->PrevItems[0]->LastName = "";
                            TItemMinorComplete *item = SeatOrders[i]->Orders->PrevItems[0];
                            item->ReturnToAvailability(item->GetQty());

                            delete item;
                            item = NULL;
                            SeatOrders[i]->Orders->DeletePrev(0);
                        }
                    }
                    __finally
                    {
                        SeatOrders[i]->Orders->Clear();
                        SeatOrders[i]->Orders->ClearPrev();
                        SeatOrders[i]->Orders->AppliedMembership.Clear();
                        Membership.Clear();
                    }
                }

                TDBTab::ReleaseTab(DBTransaction, TDeviceRealTerminal::Instance().ID.Name, 0);
                TDBTables::ClearPatronCounts(DBTransaction);
                tbtnMembership->Caption = "Membership";
                SetSelectedSeat();
                TotalCosts();
                UpdateExternalDevices();
                TDBSaleTimes::VoidSaleTime(DBTransaction, CurrentTimeKey);
                CurrentTimeKey = 0;
                CloseSidePanel();
            }

		}
		else
		{
			retval = false;
		}
	}
	else
	{
		lbDisplay->Clear();
		for (UINT i = 0; i < SeatOrders.size(); i++)
		{
			try
			{
				while (SeatOrders[i]->Orders->PrevCount != 0)
				{
					delete SeatOrders[i]->Orders->PrevItems[0];
					SeatOrders[i]->Orders->DeletePrev(0);
				}
			}
			__finally
			{
				SeatOrders[i]->Orders->Clear();
				SeatOrders[i]->Orders->ClearPrev();
				SeatOrders[i]->Orders->AppliedMembership.Clear();
				Membership.Clear();
			}
		}
		tbtnMembership->Caption = "Membership";
		TDBTab::ReleaseTab(DBTransaction, TDeviceRealTerminal::Instance().ID.Name, 0);
        TDBTables::ClearPatronCounts(DBTransaction);
		CloseSidePanel();
	}
	return retval;
}
// ---------------------------------------------------------------------------
bool TfrmSelectDish::DeleteUnsentDCAndProceed(Database::TDBTransaction &DBTransaction)
{
	bool retval = true;
    bool chcekitems = true;

	if (OrdersPending())
	{
		if (MessageBox("Deleting unsent orders!", "Warning", MB_OK + MB_ICONEXCLAMATION) == IDOK)
		{
           chcekitems = CheckItemsPrintCancel();

            if (dc_item_show)
            {
              lbDisplay->Clear();
              AnsiString memberNo = "";
              int end_idx , strt_idx = 0;
              char*  memberDetails_char = new char[memberName.Length() + 1];
              strcpy(memberDetails_char,memberName.c_str());
              int len = 0;
              for (int i = memberName.Length()-1 ; i > 0 ; i--)
              {
                 if (memberDetails_char[i] == ')' )
                 {
                    end_idx = i;
                 }
                 if (memberDetails_char[i] == '(' )
                 {
                    strt_idx = i;
                    len = (end_idx - strt_idx -1);
                    break;
                 }
               }
               memberNo = memberName.SubString(strt_idx+2, len);
               AnsiString cardId = TDrinkCommandData::Instance().GetCardIdFromMemberNo(memberNo);
               TDrinkCommandData::Instance().UpdateTimeStampToNull(cardId) ;
               dc_item_show = false;
               chcekitems = CheckItemsPrintCancel();

               if(!chcekitems)
               {
                  TMMContactInfo TempUserInfo;
                  TempUserInfo = TDeviceRealTerminal::Instance().User;
                  std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
                  chcekitems = Staff->TestAccessLevel(TempUserInfo, CheckCredit);
                  AnsiString Username;
                  if(!chcekitems)
                  {
                    TempUserInfo.Clear();
                    TLoginSuccess Result = Staff->Login(this, DBTransaction, TempUserInfo, CheckCredit);
                    if (Result == lsAccepted)
                    {
                      chcekitems = true;

                    }
                    else if (Result == lsDenied)
                    {
                       chcekitems = false;
                       retval = false;
                       MessageBox("You do not have access rights to Cancels.", "Error", MB_OK + MB_ICONERROR);
                    }
                    else if (Result == lsPINIncorrect)
                    {
                      chcekitems = false;
                      retval = false;
                      MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
                    }
                    else if (Result == lsCancel)
                    {
                      chcekitems = false;
                      retval = false;
                    }
                  }
                  if(chcekitems)
                  {
                    Username = TempUserInfo.Name;
                    SaveRemoveItems(Username);
                  }
               }

                if(chcekitems)
                {
                    lbDisplay->Clear();
                    for (UINT i = 0; i < SeatOrders.size(); i++)
                    {
                        try
                        {
                            TManagerFreebie::UndoFreeCount(DBTransaction, SeatOrders[i]->Orders->List);
                            while (SeatOrders[i]->Orders->Count != 0)
                            {
                                TItemComplete *Item = SeatOrders[i]->Orders->Items[0];
                                Item->ReturnToAvailability(Item->GetQty());

                                SeatOrders[i]->Orders->Remove(Item);
                                delete Item;
                                Item = NULL;
                            }
                            while (SeatOrders[i]->Orders->PrevCount != 0)
                            {
                              TItemMinorComplete *item = SeatOrders[i]->Orders->PrevItems[0];
                              item->ReturnToAvailability(item->GetQty());

                              delete item;
                              item = NULL;
                              SeatOrders[i]->Orders->DeletePrev(0);
                            }
                        }
                        __finally
                        {
                            SeatOrders[i]->Orders->Clear();
                            SeatOrders[i]->Orders->ClearPrev();
                            SeatOrders[i]->Orders->AppliedMembership.Clear();
                            Membership.Clear();
                        }
                    }

                    TDBTab::ReleaseTab(DBTransaction, TDeviceRealTerminal::Instance().ID.Name, 0);
                    TDBTables::ClearPatronCounts(DBTransaction);
                    tbtnMembership->Caption = "Membership";
                    SetSelectedSeat();
                    TotalCosts();
                    UpdateExternalDevices();
                    TDBSaleTimes::VoidSaleTime(DBTransaction, CurrentTimeKey);
                    CurrentTimeKey = 0;
                    CloseSidePanel();
                }
            }
			retval = false;
		}
	}
	else
	{
		lbDisplay->Clear();
		for (UINT i = 0; i < SeatOrders.size(); i++)
		{
			try
			{
				while (SeatOrders[i]->Orders->PrevCount != 0)
				{
					delete SeatOrders[i]->Orders->PrevItems[0];
					SeatOrders[i]->Orders->DeletePrev(0);
				}
			}
			__finally
			{
				SeatOrders[i]->Orders->Clear();
				SeatOrders[i]->Orders->ClearPrev();
				SeatOrders[i]->Orders->AppliedMembership.Clear();
				Membership.Clear();
			}
		}
		tbtnMembership->Caption = "Membership";
		TDBTab::ReleaseTab(DBTransaction, TDeviceRealTerminal::Instance().ID.Name, 0);
        TDBTables::ClearPatronCounts(DBTransaction);
		CloseSidePanel();
	}
	return retval;
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::SetSelectedSeat(bool selectGuest) //changes for 5900
{  
    if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot &&
        TGlobalSettings::Instance().EnableCustomerJourney && selectGuest && !SeatOrders[0]->Orders->Count && !SeatOrders[0]->isChangeTablePressed)
    {
        GetItemsFromTable(0, tgridSeats->Buttons[0][0]);
    }
	for (int i = 0; i < tgridSeats->ColCount; i++)
	{
		if (SeatOrders[i + 1]->SeatName != "")
		{
			tgridSeats->Buttons[0][i]->Caption = SeatOrders[i + 1]->SeatName;

		}
		else
		{
			tgridSeats->Buttons[0][i]->Caption = TGlobalSettings::Instance().SeatLabel + " " + IntToStr(i + 1);

		}

        if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot &&
            TGlobalSettings::Instance().EnableCustomerJourney && SeatOrders[i + 1]->RoomNumber != "" )
        {
             tgridSeats->Buttons[0][i]->Caption = SeatOrders[i + 1]->RoomNumber;
        }

		if (!tgridSeats->Buttons[0][i]->Latched && tgridSeats->Buttons[0][i]->Color != ButtonColors[BUTTONTYPE_LOCKED][ATTRIB_BUTTONCOLOR])
		{
			if (SeatOrders[i + 1]->Orders->Count == 0 && SeatOrders[i + 1]->Orders->PrevCount == 0)
			{ // Seat is empty
				tgridSeats->Buttons[0][i]->Color = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
				tgridSeats->Buttons[0][i]->FontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
                if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot &&
                 TGlobalSettings::Instance().EnableCustomerJourney && SelectedSeat == (i + 1))
                    {  
						tgridSeats->Buttons[0][i]->Latched = true;
						selectGuest = false;
                    }
			}
			else
			{
                if(selectGuest)
                {   
                    selectGuest = false;
                    tgridSeats->Buttons[0][i]->Color = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_BUTTONCOLOR];
                    tgridSeats->Buttons[0][i]->FontColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_FONTCOLOR];
                    tgridSeats->Buttons[0][i]->LatchedColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_BUTTONCOLOR];
                    tgridSeats->Buttons[0][i]->LatchedFontColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_FONTCOLOR];

                    GetItemsFromTable(i, tgridSeats->Buttons[0][i]); //changes for 5900..
                    if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot &&
                    TGlobalSettings::Instance().EnableCustomerJourney && (i + 1 )!= SelectedSeat)
                    {    
                        tgridSeats->Buttons[0][i]->Color = ButtonColors[BUTTONTYPE_FULL][ATTRIB_BUTTONCOLOR];
    				    tgridSeats->Buttons[0][i]->FontColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_FONTCOLOR];
                    }
                }
                else
                {
                    tgridSeats->Buttons[0][i]->Color = ButtonColors[BUTTONTYPE_FULL][ATTRIB_BUTTONCOLOR];
    				tgridSeats->Buttons[0][i]->FontColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_FONTCOLOR];

					if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot &&
                    TGlobalSettings::Instance().EnableCustomerJourney && SeatOrders[i + 1]->RoomNumber != "" && SelectedSeat == (i + 1))
                    {    
                        tgridSeats->Buttons[0][i]->Color = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_BUTTONCOLOR];
                        tgridSeats->Buttons[0][i]->FontColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_FONTCOLOR];
                        tgridSeats->Buttons[0][i]->LatchedColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_BUTTONCOLOR];
                        tgridSeats->Buttons[0][i]->LatchedFontColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_FONTCOLOR];
                      
                    }
                }

			}
		}
		else if (tgridSeats->Buttons[0][i]->Latched && tgridSeats->Buttons[0][i]->Color != ButtonColors[BUTTONTYPE_LOCKED][ATTRIB_BUTTONCOLOR])
		{    
			tgridSeats->Buttons[0][i]->Color = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_BUTTONCOLOR];
			tgridSeats->Buttons[0][i]->FontColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_FONTCOLOR];
			tgridSeats->Buttons[0][i]->LatchedColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_BUTTONCOLOR];
			tgridSeats->Buttons[0][i]->LatchedFontColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_FONTCOLOR];
		}
	}
    SeatOrders[0]->isChangeTablePressed = false;
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::RefreshSeats()
{
    try
    {
        UpdateTableButton();

        tbtnChangeTable->Visible = (SelectedTable > 0);
        tgridSeats->Visible = (SelectedTable > 0);

        tbtnTender->Visible = !(SelectedTable > 0);
        tbtnSave->Visible = !(SelectedTable > 0);
        tbtnCashSale->Visible = !(SelectedTable > 0);
        tbtnDollar1->Visible = !(SelectedTable > 0);
        tbtnDollar2->Visible = !(SelectedTable > 0);
        tbtnDollar3->Visible = !(SelectedTable > 0);
        tbtnDollar4->Visible = !(SelectedTable > 0);
        tbtnDollar5->Visible = !(SelectedTable > 0);
    }
    catch(Exception & E)
    {
         TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::btnUpMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (lbDisplay->ItemIndex > 0)
	{
		lbDisplay->ItemIndex--;
		// Step over Serving Courses.
		TItemRedirector *Redirector = (TItemRedirector*)lbDisplay->Items->Objects[lbDisplay->ItemIndex];
		if (Redirector->ItemType.Contains(itServingCourseDisplay))
		{
			if (lbDisplay->ItemIndex == 0)
			{
				lbDisplay->ItemIndex++;
			}
			else
			{
				lbDisplay->ItemIndex--;
			}
		}
	}
	HighlightSelectedItem();
	AfterSelectedItemChanged.Occured();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::btnDownMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (lbDisplay->ItemIndex >= 0)
	{
		lbDisplay->ItemIndex++;

		// Step over Serving Courses.
		TItemRedirector *Redirector = (TItemRedirector*)lbDisplay->Items->Objects[lbDisplay->ItemIndex];
		if (Redirector->ItemType.Contains(itServingCourseDisplay))
		{
			lbDisplay->ItemIndex++;
		}
	}
	HighlightSelectedItem();
	AfterSelectedItemChanged.Occured();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::btnTimerTimer(TObject *Sender)
{
	tbtnToggleMenus->Tag = 1;
	btnTimer->Enabled = false;
	SelectNewMenus();
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::RedrawMenu()
{
    if (TDeviceRealTerminal::Instance().Menus->VisibleMenu == NULL)
    {
        tbtnToggleMenus->Caption = "No Menu";
    }
    else
    {
        if(TDeviceRealTerminal::Instance().Menus->VisibleMenu->MenuName.IsEmpty())
        {
            TDeviceRealTerminal::Instance().Menus->VisibleMenu = TDeviceRealTerminal::Instance().Menus->Current->MenuGet(frmSelectCurrentMenu->SelectedMenuIndex);
            RedrawServingCourses();
        }
        tbtnToggleMenus->Caption = TDeviceRealTerminal::Instance().Menus->VisibleMenu->MenuName;
    }
    RedrawCourses();
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::RedrawCourses()
{

	int OldSelectedButtonType = -1;

	for (int i = 0; i < tgridOrderCourse->RowCount; i++)
	{
		if (tgridOrderCourse->Buttons[i][0]->Latched)
		{
			OldSelectedButtonType = tgridOrderCourse->Buttons[i][0]->Tag;
		}
	}

	tgridOrderCourse->ColCount = 0;
	tgridOrderCourse->RowCount = 0;

	if (TDeviceRealTerminal::Instance().Menus->VisibleMenu != NULL)
	{
		tgridOrderCourse->ColCount = 1;
		tgridOrderCourse->RowCount = TDeviceRealTerminal::Instance().Menus->VisibleMenu->Count;

		tgridOrderCourse->DefaultRowHeight = (tgridOrderCourse->ClientHeight - (tgridOrderCourse->VertScrollButtonHeight * 2) - (tgridOrderCourse->BorderWidth * 4) -
			(tgridOrderCourse->ButtonGapHeight * 7)) / 7;

		for (int j = 0; j < TDeviceRealTerminal::Instance().Menus->VisibleMenu->Count; j++)
		{
			TListCourse *Course = TDeviceRealTerminal::Instance().Menus->VisibleMenu->CourseGet(j);
			if (Course != NULL)
			{
				tgridOrderCourse->RowHeights[j] = tgridOrderCourse->DefaultRowHeight;
				tgridOrderCourse->Buttons[j][0]->Caption = Course->Course_Name;
				tgridOrderCourse->Buttons[j][0]->Color = ButtonColors[BUTTONTYPE_UNSELECTED][ATTRIB_BUTTONCOLOR];
				tgridOrderCourse->Buttons[j][0]->FontColor = clBlack;
				tgridOrderCourse->Buttons[j][0]->Tag = Course->Course_Key;
				tgridOrderCourse->Buttons[j][0]->LatchedColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_BUTTONCOLOR];
				tgridOrderCourse->Buttons[j][0]->LatchedFontColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_FONTCOLOR];
			}
		}

		TGridButton *SelectedGridButton = NULL;
		if (OldSelectedButtonType != -1)
		{
			for (int i = 0; i < tgridOrderCourse->RowCount; i++)
			{
				if (tgridOrderCourse->Buttons[i][0]->Tag == OldSelectedButtonType)
				{
					tgridOrderCourse->Buttons[i][0]->Latched = true;
					SelectedGridButton = tgridOrderCourse->Buttons[i][0];
				}
			}

			if (SelectedGridButton == NULL && tgridOrderCourse->RowCount > 0)
			{
				tgridOrderCourse->Buttons[0][0]->Latched = true;
				SelectedGridButton = tgridOrderCourse->Buttons[0][0];
			}
		}
		else if (tgridOrderCourse->RowCount > 0)
		{
			tgridOrderCourse->Buttons[0][0]->Latched = true;
			SelectedGridButton = tgridOrderCourse->Buttons[0][0];
		}

		// This will redraw this courses items.
		if (SelectedGridButton)
		{
			tgridOrderCourseMouseClick(tgridOrderCourse, mbLeft, TShiftState(), SelectedGridButton);
		}
		else
		{
			RedrawItems();
		}
	}
	else
	{
		RedrawItems();
	}
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::RedrawItems()
{
	tgridOrderItem->ColCount = 0;
	tgridOrderItem->RowCount = 0;

	if (TDeviceRealTerminal::Instance().Menus->VisibleMenu != NULL)
	{
		int CourseKey = -1;
		for (int i = 0; i < tgridOrderCourse->RowCount; i++)
		{
			if (tgridOrderCourse->Buttons[i][0]->Latched)
			{
				CourseKey = tgridOrderCourse->Buttons[i][0]->Tag;
			}
		}

        TListCourse *Course = TDeviceRealTerminal::Instance().Menus->VisibleMenu->FetchCourseByKey(CourseKey);

        if (Course != NULL)
        {
            tgridOrderItem->RowCount = TGlobalSettings::Instance().DefaultRows;
            tgridOrderItem->ColCount = TGlobalSettings::Instance().DefaultColumns;

            for (int i = 0; i < tgridOrderItem->RowCount; i++)
            {
                for (int j = 0; j < tgridOrderItem->ColCount; j++)
                {
                    TItem *Item = Course->ItemGet((i * tgridOrderItem->ColCount) + j);
                    if (Item != NULL)
                    {
                        tgridOrderItem->Buttons[i][j]->Visible = true;
                        tgridOrderItem->Buttons[i][j]->Caption = Item->GetDecoratedName();
                        tgridOrderItem->Buttons[i][j]->Enabled = Item->Enabled;
                        tgridOrderItem->Buttons[i][j]->Color = Item->SetColour;
                        tgridOrderItem->Buttons[i][j]->DisabledColor = clBtnFace;
                        tgridOrderItem->Buttons[i][j]->FontColor = GetItemButtonFontColour(Item->SetColour);
                        tgridOrderItem->Buttons[i][j]->Tag = Item->ItemKey;

                        bool SetMenuOnly = true;
                        for (UINT i = 0; i < Item->Sizes->Count; i++)
                        {
                            TItemSize *CurrentSize = Item->Sizes->SizeGet(i);
                            if (CurrentSize->Available_As_Standard)
                            {
                                SetMenuOnly = false;
                            }
                        }

                        if (SetMenuOnly || (Item->ExclusivelyAsSide))
                        {
                            tgridOrderItem->Buttons[i][j]->Enabled = false;
                        }
                        else
                        {
                            tgridOrderItem->Buttons[i][j]->Enabled = Item->Enabled;
                        }
                    }
                }
            }

            for (int k = 0; k < tgridOrderItem->RowCount; k++)
            {
                for (int j = 0; j < tgridOrderItem->ColCount; j++)
                {
                    if (tgridOrderItem->Buttons[k][j]->Caption == "")
                    {
                        tgridOrderItem->Buttons[k][j]->Visible = false;
                    }
                }
            }
        }
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::ListTimerTimer(TObject *Sender)
{

	if (lbDisplay->ItemIndex < 0)
		return;

	ListTimer->Enabled = false;

	OpenSidePanel();
	tbtnMemberDisplayPageUp->Visible   = false;
	tbtnMemberDisplayPageDown->Visible = false;

	btngridModifyMouseClick(btngridModify->Buttons[1][0],
                           mbLeft,
							TShiftState(),
	                        btngridModify->Buttons[0][0]);
	btngridModify->Buttons[1][0]->Latched = true;

	pcItemModifyDisplayQuantity();
	pcItemModify->ActivePage = tsQuantity;

/*	lbDisplay->Tag = 1;
	ListTimer->Enabled = false;
	// Get the adjustment amount.
	if (CheckAdJustItemQty())
	{
		if (lbDisplay->ItemIndex > -1)
		{
			TItemRedirector *ItemRedirector = (TItemRedirector*)lbDisplay->Items->Objects[lbDisplay->ItemIndex];
			std::auto_ptr<TfrmTouchNumpad>frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
			frmTouchNumpad->Caption = "Enter Number to Order";
			frmTouchNumpad->btnSurcharge->Caption = "Ok";
			frmTouchNumpad->btnSurcharge->Visible = true;
			frmTouchNumpad->btnDiscount->Visible = false;
			frmTouchNumpad->Mode = pmDecimal;
			frmTouchNumpad->INTInitial = ItemRedirector->CompressedContainer->Count;
			if (frmTouchNumpad->ShowModal() == mrOk)
			{
				Currency NewQt = frmTouchNumpad->CURResult;
				if (NewQt == 0)
				{
					NewQt = 1;
					MessageBox("Qty cannot be Zero, Use remove item to remove this item.", "Error", MB_OK + MB_ICONINFORMATION);
				}
				AdjustItemQty(NewQt);
			}
		}
	} */
}
// ---------------------------------------------------------------------------
bool TfrmSelectDish::CheckAdJustItemQty()
{
	bool RetVal = false;
	if (lbDisplay->ItemIndex > -1)
	{
		TItemRedirector *ItemRedirector = (TItemRedirector*)lbDisplay->Items->Objects[lbDisplay->ItemIndex];
		if (ItemRedirector->ItemType.Contains(itSetMenuMasterItem) || ItemRedirector->ItemType.Contains(itPromoItem))
		{
			MessageBox("You must order set menu items individually", "Error", MB_OK + MB_ICONINFORMATION);
		}
		else if (ItemRedirector->ItemType.Contains(itPrevItem))
		{
			MessageBox("This item has already been ordered", "Error", MB_OK + MB_ICONINFORMATION);
		}
		else if (ItemRedirector->ItemType.Contains(itOption))
		{
			MessageBox("You must order options individually", "Error", MB_OK + MB_ICONINFORMATION);
		}
		else if (ItemRedirector->ItemType.Contains(itSubItem))
		{
			MessageBox("You must order sides individually", "Error", MB_OK + MB_ICONINFORMATION);
		}
		else if (ItemRedirector->ItemType.Contains(itNormalItem) || ItemRedirector->ItemType.Contains(itPromoItem))
		{
			if (((TItemComplete*)ItemRedirector->ParentRedirector->ItemObject)->IsFreebe)
			{
				MessageBox("You cannot copy a discount reward item.", "Error", MB_OK + MB_ICONINFORMATION);
			}
			else
			{
				RetVal = true;
			}
		}
	}
	return RetVal;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::lbDisplayMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	// Starts MultiOrder Timer.
	ListTimer->Enabled = true;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::lbDisplayMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	// Stops MultiOrder Timer.
	ListTimer->Enabled = false;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::lbDisplayClick(TObject *Sender)
{
	if(tsQuantity->TabVisible)
	{
		if (lbDisplay->ItemIndex > -1)
		{
 			TItemRedirector *Redirector = (TItemRedirector*)lbDisplay->Items->Objects[lbDisplay->ItemIndex];
			Redirector->CompressedContainer->Container->CurrentItemRedirector = Redirector;
			Redirector->CompressedContainer->Container->SelectedIndex = lbDisplay->ItemIndex;
		}
		AfterSelectedItemChanged.Occured();
		if (pnlItemModify->Visible == false)
		{
			OpenSidePanel();
		}
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::HighlightSelectedItem()
{
	if (lbDisplay->ItemIndex == -1)
	{
		// Last Item on the list might have been removed.
		if (SeatOrders[SelectedSeat]->Orders->SelectedIndex >= lbDisplay->Count)
		{
			SeatOrders[SelectedSeat]->Orders->SelectedIndex = lbDisplay->Count - 1;
		}
		lbDisplay->ItemIndex = SeatOrders[SelectedSeat]->Orders->SelectedIndex;
	}

	if (lbDisplay->ItemIndex > -1)
	{
		TItemRedirector *Redirector = (TItemRedirector*)lbDisplay->Items->Objects[lbDisplay->ItemIndex];
		if (Redirector->ItemType.Contains(itServingCourseDisplay))
		{
			lbDisplay->ItemIndex++;
			Redirector = (TItemRedirector*)lbDisplay->Items->Objects[lbDisplay->ItemIndex];
		}
		Redirector->CompressedContainer->Container->CurrentItemRedirector = Redirector;
		Redirector->CompressedContainer->Container->SelectedIndex = lbDisplay->ItemIndex;
		lbDisplay->Selected[lbDisplay->ItemIndex] = true;
	}
}
// ---------------------------------------------------------------------------
bool TfrmSelectDish::ProcessOrders(TObject *Sender, Database::TDBTransaction &DBTransaction, int SelectedTab, TMMTabType TabType, AnsiString TabContainerName, AnsiString TabName,
	AnsiString PartyName, bool PrintPrelim, int TableNo, int SeatNo, int RoomNo, bool BillOff,AnsiString DelayedInvoiceNumber)
{
    CanClose=false;
    IsParkSalesEnable = true;
    bool order_was_resumed_via_hold_and_send = false;
    bool PaymentComplete = false;
    TPaymentTransaction PaymentTransaction(DBTransaction);
    PaymentTransaction.PartyName = PartyName;
    AnsiString BeveragesInvoiceNumber = "";
    bool isBeveragesInvGenerated = false;
     //To check whether payment done is by linking the fast tenderkey with payment
    bool isProcessedQuickPayment = false;

    if(TGlobalSettings::Instance().IsBillSplittedByMenuType && TGlobalSettings::Instance().TransferTableOnPrintPrelim
            && DelayedInvoiceNumber.Pos("L") != 0)
    {
        isBeveragesInvGenerated = true;
        BeveragesInvoiceNumber = DelayedInvoiceNumber;
    }

	try
	{
        PaymentTransaction.TypeOfSale = TypeOfSale;
        if(IsProcessQuickPayment(Sender))
        {
          PaymentTransaction.IsQuickPayTransaction = true;
          PaymentTransaction.QuickPaymentName = GetQuickPaymentName(Sender);
          Sender = tbtnTender;
          isProcessedQuickPayment = true;
        }
        if(ChitNumber.IsNonChargableChit)
           {
              PaymentTransaction.TypeOfSale = NonChargableSale;
              TDiscount CurrentDiscount;
              if( ManagerDiscount->GetNonChargableDiscount(DBTransaction,CurrentDiscount))   //GetDiscount(DBTransaction, -5, CurrentDiscount);
                {
                  PaymentTransaction.Discounts.push_back(CurrentDiscount);
                }
           }


		std::auto_ptr<TList>OrdersList(new TList);
        PaymentTransaction.SalesType = eCash;

		TMMContactInfo Member;
		int SecRefNumber = TDBSecurity::GetNextSecurityRef(PaymentTransaction.DBTransaction);
		TDateTime OrderedTimeStamp = Now();
        AnsiString BevTabName = "";
        int BevTabKey;
        if(isBeveragesInvGenerated)
        {
            BevTabName = TabName;
            BevTabKey = SelectedTab;
        }
		for (UINT iSeat = 0; iSeat < SeatOrders.size(); iSeat++)
		{
			for (int i = 0; i < SeatOrders[iSeat]->Orders->Count; i++)
			{
				TItemComplete *Order = SeatOrders[iSeat]->Orders->Items[i];
                if(TGlobalSettings::Instance().TransferTableOnPrintPrelim && PrintPrelim && Order->ItemType && !isBeveragesInvGenerated &&
                        TGlobalSettings::Instance().IsBillSplittedByMenuType && DelayedInvoiceNumber != "" )
                {
                    isBeveragesInvGenerated = true;
                    BeveragesInvoiceNumber = "L" + Invoice->GetBeveragesInvoiceNumber(DBTransaction);
                    BevTabName = TGlobalSettings::Instance().ReceiptNumberLabel + BeveragesInvoiceNumber;
                    //Create Tab
                    BevTabKey = TDBTab::GetOrCreateTab(DBTransaction, 0);
                    TDBTab::SetTabType(DBTransaction, BevTabKey, TabDelayedPayment);
                    TDBTab::SetTabName(DBTransaction, BevTabKey, BevTabName);

                }

				Order->TabKey = SelectedTab;
				if (TabContainerName == "" && TabType == TabTableSeat)
				{
					Order->TabContainerName = "Table # " + IntToStr(TableNo); // + " Seat # "  + IntToStr((iSeat == 0) ? SeatNo : SeatOrders[iSeat]->SeatNumber);
				}
				else
				{
					Order->TabContainerName = TabContainerName;
				}
				Order->TabName = TabName;
                //MM2038
                Order->IdName = TGlobalSettings::Instance().TabPrintName;
                Order->IdNumber = TGlobalSettings::Instance().TabPrintPhone;
				Order->TabType = TabType;
                if(!TDeviceRealTerminal::Instance().BasePMS->Enabled)
				    Order->RoomNo = RoomNo;
				Order->TableNo = TableNo;
				Order->SeatNo = (iSeat == 0) ? SeatNo : SeatOrders[iSeat]->SeatNumber;
                if(TabType == TabDelayedPayment)
                  Order->SeatNo = 0;
				Order->OrderType = NormalOrder;
				if (!TGlobalSettings::Instance().UnRavelKitchenPrinting)
				{
					Order->TimeStamp = OrderedTimeStamp;
				}
    		    Order->Terminal = TDeviceRealTerminal::Instance().ID.Name;
				Order->OrderedLocation = TDeviceRealTerminal::Instance().ID.Location;
				Member = SeatOrders[iSeat]->Orders->AppliedMembership;
                Order->Loyalty_Key = SeatOrders[iSeat]->Orders->AppliedMembership.ContactKey;
                if(TGlobalSettings::Instance().TransferTableOnPrintPrelim && PrintPrelim && Order->ItemType &&
                            TGlobalSettings::Instance().IsBillSplittedByMenuType && BeveragesInvoiceNumber != "")
                {
                    Order->TabKey = BevTabKey;
				    Order->TabName = BevTabName;
                    Order->TabContainerName = BevTabName;
                }

				OrdersList->Add(Order);

				// Complete Order Security.
				Order->Security->SetSecurityRefNumber(SecRefNumber);
				TSecurityReference SecRef;
				SecRef.UserKey = TDeviceRealTerminal::Instance().User.ContactKey;
				SecRef.Event = SecurityTypes[secOrderedBy];
				SecRef.From = TDeviceRealTerminal::Instance().User.Name;
				SecRef.To = TDeviceRealTerminal::Instance().User.Initials;
				SecRef.TimeStamp = Now();
				Order->Security->SecurityUpdate(secOrderedBy, SecRef);

				for (int j = 0; j < Order->SubOrders->Count; j++)
				{
					TItemCompleteSub *SubOrder = Order->SubOrders->SubOrderGet(j);
					if (SubOrder)
					{
						SubOrder->Security->SetSecurityRefNumber(SecRefNumber);
						TSecurityReference SecRef;
						SecRef.UserKey = TDeviceRealTerminal::Instance().User.ContactKey;
						SecRef.Event = SecurityTypes[secOrderedBy];
						SecRef.From = TDeviceRealTerminal::Instance().User.Name;
						SecRef.To = TDeviceRealTerminal::Instance().User.Initials;
						SecRef.TimeStamp = Now();
						SubOrder->Security->SecurityUpdate(secOrderedBy, SecRef);
					}
				}

			}
		}

        if(Sender == tbtnSave)
            PaymentTransaction.IgnoreLoyaltyKey = true;
        else
            PaymentTransaction.IgnoreLoyaltyKey = false;
		PaymentTransaction.Membership.Assign(Membership);
		PaymentTransaction.Orders->Assign(OrdersList.get());
		PaymentTransaction.Recalc();
		// --------------------------------------------------------------------

		if (OrdersList->Count > 0 || Sender == tbtnTender)
		{

			// Retrive Chit Number ------------------------------------------------
			TChitNumberController ChitNumberController(this, DBTransaction);
			ChitResult Result = ChitNumberController.GetChitNumber(false, ChitNumber);
			switch(Result)
			{
			case ChitDisabled:
				tbtnChitNumber->Visible = false;
				PaymentTransaction.ChitNumber.Clear();
				break;
			case ChitOk:
				tbtnChitNumber->Visible = true;
				tbtnChitNumber->Caption = ChitNumber.GetChitNumber();
				PaymentTransaction.ChitNumber = ChitNumber;
                if(ChitNumber.DiscountList.size() == 0 && ChitNumber.ApplyDiscountsList->Count >0 && !isChitDiscountExist)
                {
                      GetChitDiscountList(DBTransaction, ChitNumber.DiscountList);
                }
				break;
			case ChitCancelled:
				tbtnChitNumber->Caption = ChitNumber.Name;
				throw EAbort("Cancelled by User.");
				break;
			case ChitNone:
				PaymentTransaction.ChitNumber.Clear();
				break;
			}

			// --------------------------------------------------------------------
			// Sort Transaction Balance -------------------------------------------
            TotalCosts();
			UpdateExternalDevices();

			if (ChitNumber.Valid())
			{
				for (int i = 0; i < OrdersList->Count; i++)
				{
					TItemComplete *Order = (TItemComplete*)OrdersList->Items[i];
					Order->ChitNumber = PaymentTransaction.ChitNumber;
				}
			}
			if (Sender == tbtnCashSale)
			{
				PaymentTransaction.Type = eTransQuickSale;
				TDeviceRealTerminal::Instance().PaymentSystem->PaymentsReload(PaymentTransaction);
				TPayment *CashPayment = PaymentTransaction.PaymentFind(CASH);
				if (CashPayment == NULL)
				{
					throw Exception("Unable to load Cash Payment.");
				}
				CashPayment->SetPay(PaymentTransaction.Money.PaymentDue);
			}
			else if (Sender == tbtnTender)
			{
				if (CurrentTender == 0)
				{
					PaymentTransaction.Type = eTransOrderSet;
				}
				else
				{
					PaymentTransaction.Type = eTransQuickSale;
					TDeviceRealTerminal::Instance().PaymentSystem->PaymentsReload(PaymentTransaction);
					TPayment *CashPayment = PaymentTransaction.PaymentFind(CASH);
					if (CashPayment == NULL)
					{
						throw Exception("Unable to load Cash Payment.");
					}
					CashPayment->SetPay(InitialMoney.PaymentAmount);
				}
			}
			// --------------------------------------------------------------------
			if(TGlobalSettings::Instance().CaptureCustomerName)
			{
				 TCustNameAndOrderType* CustNameAndOrderType = TCustNameAndOrderType::Instance();
				if(!CustNameAndOrderType->IsNameCaught)
				{
					CustNameAndOrderType->CatchCustNameAndOrderType(DisplayNameKeypad(), DisplayCustomerTypeReasons(), SeatOrders);
				}
				PaymentTransaction.CustomerOrder = CustNameAndOrderType->GetStringPair();
			}

			if (Sender == tbtnCashSale || Sender == tbtnTender)
			{
                for(int indexStore = 0; indexStore < patronsStore.size(); indexStore++)
                {
                    if(patronsStore[indexStore].Count != 0)
                    {
                         PaymentTransaction.Patrons = patronsStore;
                         break;
                    }
                }

				if((Sender == tbtnCashSale || ((Sender == tbtnTender)&& CurrentTender != 0) || isProcessedQuickPayment) && TGlobalSettings::Instance().EnableMenuPatronCount)
				{
                   PaymentTransaction.CalculatePatronCountFromMenu();
                   StorePatronsInformation(PaymentTransaction);
                }

				// ask for patron count if this is a quick sale
				if((Sender == tbtnCashSale ||  ( Sender == tbtnTender && CurrentTender != 0) || isProcessedQuickPayment) && TGlobalSettings::Instance().PromptForPatronCount)
				{
					if (TManagerPatron::Instance().GetCount(PaymentTransaction.DBTransaction) > 0)
					{
                        InitializePatronForQuickSale(PaymentTransaction);
					}
					else
					{
                        MessageBox("There are no Patron Types Configured.", "Patron Error.", MB_OK + MB_ICONERROR);
                    }
                }

                bool isGuestExist = true;
                isGuestExist = LoadPMSGuestDetails(PaymentTransaction);

                if(isGuestExist)
                {
				    PaymentComplete = TDeviceRealTerminal::Instance().PaymentSystem->ProcessTransaction(PaymentTransaction);
                    if(PaymentComplete)
                    {
                        TStringList* logList = new TStringList();
                        logList->Add("After calling ProcessTransaction() in POS and payment was completed.");
                        TSaveLogs::RecordFiscalLogs(logList);
                        delete logList;
                        logList = NULL;
                    }
                }
                
                customerDisp.TierLevel = TGlobalSettings::Instance().TierLevelChange ;

                if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot && TGlobalSettings::Instance().EnableCustomerJourney )
                {
                    IsAutoLogOutInSelectDish = true;
                 }

				if (PaymentComplete)
				{
					// Security Records are processed in TransProcessTransaction.

                    TStringList* logList = new TStringList();
                    logList->Add("Updating stock.");
                    TSaveLogs::RecordFiscalLogs(logList);

					TDeviceRealTerminal::Instance().ManagerStock->UpdateStock(DBTransaction, OrdersList.get(), TDeviceRealTerminal::Instance().User.Name);
                    if (!PaymentTransaction.CreditTransaction)
                       HideSoldItems(PaymentTransaction.DBTransaction,OrdersList.get());
                    isChitDiscountExist = false;
                    logList->Clear();
                     logList->Add("After Updating stock.");
                    TSaveLogs::RecordFiscalLogs(logList);
                    delete logList;
                    logList = NULL;
				}
				else
				{
                    Database::TDBTransaction OrdersTransaction(TDeviceRealTerminal::Instance().DBControl);
                    TDeviceRealTerminal::Instance().RegisterTransaction(OrdersTransaction);
                    OrdersTransaction.StartTransaction();

                    SelectedTab = TDBTab::GetTab(OrdersTransaction, TDeviceRealTerminal::Instance().ID.Name + CASH_ACCOUNT, TabCashAccount);
                    if (SelectedTab != 0)
                    {
                        TDBTab::ClearTab(OrdersTransaction, SelectedTab);
                    }

					// Remove all the Ordered By Secutiry Records.
					for (int i = 0; i < OrdersList->Count; i++)
					{
						TItemComplete *Order = (TItemComplete*)OrdersList->Items[i];
						TDBSecurity::ClearSecurity(OrdersTransaction, Order->Security, secOrderedBy);
						Order->Security->SecurityDelete(Order->Security->SecurityGetType(secOrderedBy));
						for (int i = 0; i < Order->SubOrders->Count; i++)
						{
							TItemCompleteSub *SubOrder = Order->SubOrders->SubOrderGet(i);
							TDBSecurity::ClearSecurity(OrdersTransaction, SubOrder->Security, secOrderedBy);
							SubOrder->Security->SecurityDelete(SubOrder->Security->SecurityGetType(secOrderedBy));
						}
					}
                    if(PaymentTransaction.Orders->Count != SeatOrders[SelectedSeat]->Orders->Count)
                    {
                       SeatOrders[SelectedSeat]->Orders->Clear();
                       for(int index = 0; index < PaymentTransaction.Orders->Count; index++)
                       {
                           TItemComplete *itemComplete = (TItemComplete*)PaymentTransaction.Orders->Items[index];
                           if(itemComplete->GetQty() != 0)
                               SeatOrders[SelectedSeat]->Orders->Add(itemComplete,itemComplete->ItemOrderedFrom);
                       }
                    }

                    if (Membership.Member.ContactKey != PaymentTransaction.Membership.Member.ContactKey)
                        ApplyMembership(OrdersTransaction, PaymentTransaction.Membership.Member);

                    OrdersTransaction.Commit();
                    RedrawSeatOrders();
                    TotalCosts();
                    UpdateExternalDevices();
                    HighlightSelectedItem();
                    isChitDiscountExist = true;

                    TStringList* logList = new TStringList();
                    logList->Add("Payment was not completed in select dish.");
                    TSaveLogs::RecordFiscalLogs(logList);
                    delete logList;
                    logList = NULL;
				}
			}
			else
			{
                if(TGlobalSettings::Instance().EnableMenuPatronCount)
                   PaymentTransaction.CalculatePatronCountFromMenu();
                else
                   PaymentTransaction.Patrons = patronsStore;
				std::auto_ptr<TfrmProcessing>(frmProcessing)(TfrmProcessing::Create<TfrmProcessing>(this));
				frmProcessing->Message = "Posting Orders";
				frmProcessing->Show();
                //remove applied discount from items..
                if(ChitNumber.Valid())
                {
                   RemoveChitDiscountList();
                   TGlobalSettings::Instance().TabPrintName = "";
                   TGlobalSettings::Instance().TabPrintPhone = "";
                }
                //Set order Identification Number
                int identificationNumber = 0;

                if(TableNo !=0)
                   identificationNumber = TDBOrder::GetOrderIdentificationNumberForTable(DBTransaction,TableNo);
                else
                   identificationNumber = TDBOrder::GetOrderIdentificationNumberForTab(DBTransaction,TabName);

                std::set<int>SeatCounter;
				for (int o = 0; o < OrdersList->Count; o++)
				{
					TItemComplete *Order = (TItemComplete*)OrdersList->Items[o];
                    if (Order->SeatNo > 0)
                    {
                        SeatCounter.insert(Order->SeatNo);
                    }

                    if(TGlobalSettings::Instance().IsBillSplittedByMenuType && Order->ItemType && DelayedInvoiceNumber != "" )
                    {
                        Order->DelayedInvoiceNumber = BeveragesInvoiceNumber;
                    }
                    else
                    {
                        Order->DelayedInvoiceNumber = DelayedInvoiceNumber;
                    }
                    Order->OrderIdentificationNo = identificationNumber;
 					if (Order->ServingCourse.ServingCourseKey < 1)
					{
						Order->ServingCourse = TDeviceRealTerminal::Instance().Menus->DefaultServingCourse;
						TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, "Order has no Serving Course");
					}
                   for (int j = 0; j < Order->SubOrders->Count; j++)
					{
                       TItemCompleteSub *SubOrder = Order->SubOrders->SubOrderGet(j);

                       if(TGlobalSettings::Instance().IsBillSplittedByMenuType && SubOrder->SubItemType && DelayedInvoiceNumber != "" )
                       {
                            SubOrder->DelayedInvoiceNumber = BeveragesInvoiceNumber;
                       }
                       else
                       {
                            SubOrder->DelayedInvoiceNumber = DelayedInvoiceNumber;
                       }
                       SubOrder->DelayedInvoiceNumber = Order->DelayedInvoiceNumber;
                       SubOrder->OrderIdentificationNo = identificationNumber;
                    }
				}
                int updatePatronCount = GetUpdatedPatronCount(DBTransaction,TableNo,SeatCounter);
				TDBOrder::ProcessOrders(DBTransaction, OrdersList.get()); // Put Orders in DB where required.
                if(TableNo > 0)
                {
                  TDBTables::SetPatronNumbers(DBTransaction,TableNo,updatePatronCount);
                }

               if(DelayedInvoiceNumber!="")
                {
                    AddSecurityOnMoveOrderToTab(DBTransaction,OrdersList.get());
                }

                LoadPMSGuestDetails(PaymentTransaction);

				for (int i = 0; i < OrdersList->Count; i++)
				{
					TItemComplete *Order = (TItemComplete*)OrdersList->Items[i];                    
				  	TDBSecurity::ProcessSecurity(DBTransaction, Order->Security);
                    int PmsTableNumber = 0;
                    if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot &&
                            TGlobalSettings::Instance().EnableCustomerJourney)
                    {
                        if(Order->TabType == TabRoom)
                            PmsTableNumber = Order->RoomNo;
                        else
                            PmsTableNumber = Order->TableNo;

                        TDBSecurity::SavePMSGuestDetails(PaymentTransaction, Order, PmsTableNumber, Order->SeatNo);
                    }

					for (int j = 0; j < Order->SubOrders->Count; j++)
					{
						TItemCompleteSub *SubOrder = Order->SubOrders->SubOrderGet(j);
						if (SubOrder)
						{
                           TDBSecurity::ProcessSecurity(DBTransaction, SubOrder->Security);

                           if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot &&
                            TGlobalSettings::Instance().EnableCustomerJourney)
                                TDBSecurity::SavePMSGuestDetails(PaymentTransaction, SubOrder, PmsTableNumber, Order->SeatNo);
						}
					}
				}
				TDeviceRealTerminal::Instance().ManagerStock->UpdateStock(DBTransaction, OrdersList.get(), TDeviceRealTerminal::Instance().User.Name);
                HideSoldItems(PaymentTransaction.DBTransaction,OrdersList.get());
				frmProcessing->Close();
				PaymentComplete = true;
			}
			if (PaymentComplete)
			{
                TStringList* logList = new TStringList();
                logList->Add("Before calling CloseSaleStartTime");
                TSaveLogs::RecordFiscalLogs(logList);
				TDBSaleTimes::CloseSaleStartTime(DBTransaction, CurrentTimeKey); // Close the Sale Key for Chefmate.

				if (OrdersList->Count > 0)
				{
					// Print the Orders In the Kitchen if any.
					std::auto_ptr<TReqPrintJob>Request(new TReqPrintJob(&TDeviceRealTerminal::Instance()));
					std::auto_ptr<TPaymentTransaction>PrintTransaction(new TPaymentTransaction(DBTransaction));
					// Copy over the Patron Counts for Printing.
					PrintTransaction->Patrons = PaymentTransaction.Patrons;
					PrintTransaction->CustomerOrder = PaymentTransaction.CustomerOrder;

                    logList->Clear();
                    logList->Add("Before Printing docket.");
                    TSaveLogs::RecordFiscalLogs(logList);

					if (!PaymentTransaction.CreditTransaction)
					{
						Request->BarCodeData = CurrentTimeKey;
						Request->Transaction = PrintTransaction.get();
						Request->JobType = pjKitchen;
						TItemComplete *Order = (TItemComplete*)OrdersList->Items[0];
						Request->Waiter = Order->Security->SecurityGetField(secOrderedBy, secfFrom);
						Request->Transaction->Membership.Assign(Membership);
						PrintTransaction->ChitNumber = PaymentTransaction.ChitNumber;
						Request->MiscData["PartyName"] = PartyName;
						for (int i = 0; i < OrdersList->Count; i++)
						{
							TSecurityReference *OldSecRef = Order->Security->SecurityGetType(secCredit);
							if (OldSecRef == NULL)
							{
								PrintTransaction->Orders->Add(OrdersList->Items[i]);

                             }
                         }

						Request->Transaction->Money.Recalc(*Request->Transaction);
						bool TabBilledOff = BillOff || !(TabType == TabTableSeat || TabType == TabNormal || TabType == TabStaff || TabType == TabMember);

						//	Check whether the Hold and send option is checked on the chit options, when a chit number is present
						bool CanProceedWithChitNumberHoldAndSendOption =  Result == ChitNone || Result == ChitDisabled ? true : Result == ChitOk && ChitNumber.IsHoldAndSendDisabled ? false : true;

						OrderHeld = false;
						if (TGlobalSettings::Instance().EnableHoldSend
							&& TabBilledOff
							&& TabType != TabInvoice
							&& CanProceedWithChitNumberHoldAndSendOption )
						{
							if ((OrderHeld = !DisplayHoldSend(OrdersList, PrintTransaction->Patrons)) == 0)
							{
								PrintTransaction->Orders->Assign(OrdersList.get());
								PrintTransaction->ChitNumber = ChitNumber;
								PrintTransaction->CustomerOrder = TCustNameAndOrderType::Instance()->GetStringPair();
								Request->MiscData["PartyName"] = ((TItemComplete*)OrdersList->Items[0])->PartyName;
							}
						}

                        order_was_resumed_via_hold_and_send = PrintTransaction->Orders->Count > 0 && !OrderOnHold;
                        double totalQuantity = 0;
                        for(int l = 0; l < PrintTransaction->Orders->Count; l++)
                        {
                            totalQuantity += ((TItemComplete*)PrintTransaction->Orders->Items[l])->GetQty();
                        }
                        if(totalQuantity == 0)
                           order_was_resumed_via_hold_and_send = false;
						// SENDS A COMPLETE ORDER AFTER TENDERING IT ...
						// OR CASHERING IT OUT.
						if (order_was_resumed_via_hold_and_send)
						{
                            std::auto_ptr<TKitchen> Kitchen(new TKitchen());
							Kitchen->Initialise(DBTransaction);
                            PrintTransaction->TypeOfSale = PaymentTransaction.TypeOfSale;
                            PrintTransaction->ChitNumber = ChitNumber;

                            if(TDeviceRealTerminal::Instance().BasePMS->Enabled )
                            {
                                if(PaymentTransaction.Phoenix.AccountName != "")
                                {
                                    if(TGlobalSettings::Instance().PMSType != SiHot)
                                    {
                                        PrintTransaction->Customer.RoomNumber = PaymentTransaction.Customer.RoomNumber;
                                        PrintTransaction->Phoenix.AccountName = PaymentTransaction.Phoenix.AccountName;
                                    }
                                    else if(TGlobalSettings::Instance().PMSType == SiHot)
                                    {
                                        PrintTransaction->Customer.RoomNumberStr = PaymentTransaction.Customer.RoomNumberStr;
                                        PrintTransaction->Phoenix.AccountName = PaymentTransaction.Phoenix.AccountName;
                                    }
                                }
                                else
                                {
                                    if(TGlobalSettings::Instance().PMSType != SiHot)
                                        PrintTransaction->Customer = TCustomer(0,0,"");
                                    else
                                        PrintTransaction->Customer = TCustomer("",0,"");
                                }
                            }
                            else if(!TRooms::Instance().Enabled && !TDeviceRealTerminal::Instance().BasePMS->Enabled)
                                PrintTransaction->Customer = TCustomer(0,0,"");
                            Request->Transaction->TypeOfSale = PaymentTransaction.TypeOfSale;
                              //MM-4563
                            std::vector<TPatronType> selectedTablePatrons = TDBTables::GetPatronCount(DBTransaction, SelectedTable);
                           int patronCount = GetCount(selectedTablePatrons);
                            if(patronCount > 0)
                            {
                                Request->Transaction->Patrons = selectedTablePatrons;
                            }

                            Kitchen->GetPrintouts(DBTransaction, Request.get());
                            Request->Transaction = PrintTransaction.get();
                            Request->Printouts->Print(devPC);
                            ManagerDockets->Archive(DBTransaction,Request.get());
                            completeOrderToChefMate( PrintTransaction.get() );

                            logList->Clear();
                            logList->Add("Docket printed and order sent to chef.");
                            TSaveLogs::RecordFiscalLogs(logList);
						}
					}

					// Print Prelim / Invoice if any Invoice sale.
					if (TabType == TabNormal && TGlobalSettings::Instance().AutoPrintTabReceipts || TabType == TabStaff && TGlobalSettings::Instance()
						.AutoPrintStaffReceipts || TabType == TabMember && TGlobalSettings::Instance().AutoPrintMemberReceipts || TabType == TabTableSeat && TGlobalSettings::Instance()
						.AutoPrintTableReceipts || TabType == TabCashAccount && TGlobalSettings::Instance().AutoPrintCashReceipts || TabType == TabRoom && TGlobalSettings::Instance()
						.AutoPrintRoomReceipts || TabType == TabInvoice && TGlobalSettings::Instance().AutoPrintInvoiceReceipts || PrintPrelim)
					{
						bool OrdersLoadedFromTabs = false;
						TMMContactInfo InvoiceOwnerInfo;

						///Delayed Print prelimn receipt in case of GST
                        std::auto_ptr<TList>FoodOrdersList(new TList);
                        std::auto_ptr<TList>BevOrdersList(new TList);
                        int Size = 1;

                        if(TGlobalSettings::Instance().IsBillSplittedByMenuType && TabType == TabDelayedPayment &&
                                    TGlobalSettings::Instance().TransferTableOnPrintPrelim)
                        {
                            TManagerDelayedPayment::Instance().SplitDelayedPaymentOrderByMenuType(OrdersList.get(), FoodOrdersList.get(), BevOrdersList.get());
                        }
						if(BevOrdersList->Count && FoodOrdersList->Count)
                            Size = 2;

						for(int index = 0; index < Size; index++)
                        {
                            TPaymentTransaction InvoiceTransaction(DBTransaction);
                            InvoiceTransaction.Membership.Assign(Membership);
                            TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
                            if(TGlobalSettings::Instance().CaptureCustomerName)
                            {
                                InvoiceTransaction.CustomerOrder = TCustNameAndOrderType::Instance()->GetStringPair();
                            }
                            std::auto_ptr<TReqPrintJob>TempReceipt(new TReqPrintJob(&TDeviceRealTerminal::Instance()));

                            if (TGlobalSettings::Instance().SaveAndPrintPrintsPartialOrder && TabType != TabInvoice || TabType == TabCashAccount)
                            {

                                logList->Clear();
                                logList->Add("If block of TGlobalSettings::Instance().SaveAndPrintPrintsPartialOrder");
                                TSaveLogs::RecordFiscalLogs(logList);

                                if (OrdersList->Count != 0)
                                {
                                    std::set<__int64>SelectedTabs;

                                    if(TGlobalSettings::Instance().IsBillSplittedByMenuType && TabType == TabDelayedPayment &&
                                        TGlobalSettings::Instance().TransferTableOnPrintPrelim && Size == 2)
                                    {
                                        if(index)
                                        {
                                            InvoiceTransaction.Orders->Assign(BevOrdersList.get());
                                            TDBOrder::GetTabKeysFromOrders(BevOrdersList.get(), SelectedTabs);
                                        }
                                        else
                                        {
                                            InvoiceTransaction.Orders->Assign(FoodOrdersList.get());
                                            TDBOrder::GetTabKeysFromOrders(FoodOrdersList.get(), SelectedTabs);
                                        }
                                    }
                                    else
                                    {
                                        InvoiceTransaction.Orders->Assign(OrdersList.get());
                                        TDBOrder::GetTabKeysFromOrders(OrdersList.get(), SelectedTabs);
                                    }

                                    InvoiceTransaction.Money.CreditAvailable = TDBTab::GetTabsCredit(DBTransaction, SelectedTabs);
                                    InvoiceTransaction.Money.Recalc(InvoiceTransaction);
                                }
                            }
                            else
                            {
                                std::set<__int64>InvoiceTabs;

                                logList->Clear();
                                logList->Add("Else block of TGlobalSettings::Instance().SaveAndPrintPrintsPartialOrder");
                                TSaveLogs::RecordFiscalLogs(logList);

                                if(TGlobalSettings::Instance().IsBillSplittedByMenuType && TabType == TabDelayedPayment &&
                                        TGlobalSettings::Instance().TransferTableOnPrintPrelim )
                                {
                                    if(Size == 2)
                                    {
                                        if(index)
                                        {
                                            TDBOrder::GetTabKeysFromOrders(BevOrdersList.get(), InvoiceTabs);
                                        }
                                        else
                                        {
                                            TDBOrder::GetTabKeysFromOrders(FoodOrdersList.get(), InvoiceTabs);
                                        }
                                    }
                                    else
                                    {
                                        TDBOrder::GetTabKeysFromOrders(OrdersList.get(), InvoiceTabs);
                                    }
                                }
                                else if (TabType == TabTableSeat)
                                { // Retrive the Tab Key for this Table/Seat.
                                    TDBTables::GetTabKeys(DBTransaction, TableNo, InvoiceTabs);
                                }
                                else if (TabType == TabRoom)
                                { // Retrive the Tab Key for this Table/Seat.
                                    SelectedTab = TDBRooms::GetRoomTab(DBTransaction, RoomNo);
                                    InvoiceTabs.insert(SelectedTab);
                                }
                                else
                                {
                                    InvoiceTabs.insert(SelectedTab);
                                }

                                TDBOrder::GetOrdersFromTabKeys(DBTransaction, InvoiceTransaction.Orders, InvoiceTabs);
                                InvoiceTransaction.Money.CreditAvailable = TDBTab::GetTabsCredit(DBTransaction, InvoiceTabs);
                                InvoiceTransaction.Money.Recalc(InvoiceTransaction);
                                OrdersLoadedFromTabs = true;
                            }

                            if (TGlobalSettings::Instance().EnableMenuPatronCount)
                            {
                                InvoiceTransaction.CalculatePatronCountFromMenu();
                            }
                            else
                                InvoiceTransaction.Patrons = patronsStore;

                            if(TabType == TabDelayedPayment && (!TGlobalSettings::Instance().EnableMenuPatronCount))
                                InvoiceTransaction.Patrons = TDBTab::GetDelayedPatronCount(InvoiceTransaction.DBTransaction, SelectedTab);

                            if (InvoiceTransaction.Money.TotalAdjustment != 0)
                            {
                                InvoiceTransaction.TotalAdjustment = InvoiceTransaction.Money.TotalAdjustment;
                                InvoiceTransaction.DiscountReason = InvoiceTransaction.TotalAdjustment < 0 ? "Discount " : "Surcharge";
                            }

                            TempReceipt->JobType = pjReceiptReceipt;
                            TempReceipt->PaymentType = ptPreliminary;

                            if (TabType == TabInvoice)
                            {
                                logList->Clear();
                                logList->Add("Tab type was invoice.");
                                TSaveLogs::RecordFiscalLogs(logList);

                                TempReceipt->JobType = pjReceiptInvoice;
                                TempReceipt->PaymentType = ptFinal;

                                int InvoiceKey = TDBTab::GetTabInvoice(DBTransaction, SelectedTab);
                                InvoiceTransaction.InvoiceNumber = Invoice->GetInvoiceNumber(DBTransaction, InvoiceKey);
                                int ContactKey = Invoice->GetInvoiceOwner(DBTransaction, InvoiceKey);
                                TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetContactDetails(DBTransaction, ContactKey, InvoiceOwnerInfo);
                                TempReceipt->ExtraInfo->Add("Name: " + InvoiceOwnerInfo.Name);
                                TempReceipt->ExtraInfo->Add("Member No. " + InvoiceOwnerInfo.MembershipNumber);
                            }
                            else if (TabType == TabRoom)
                            {
                                logList->Clear();
                                logList->Add("Tab type was Room.");
                                TSaveLogs::RecordFiscalLogs(logList);

                                TempReceipt->ExtraInfo->Add("Room Number # " + IntToStr(RoomNo));
                                TempReceipt->ExtraInfo->Add("Guest " + TDBRooms::GetPartyName(DBTransaction, RoomNo));
                            }

                            // Print Invoice.

                            logList->Clear();
                                logList->Add("Printing invoice.");
                                TSaveLogs::RecordFiscalLogs(logList);

                            TempReceipt->Transaction = &InvoiceTransaction;
                            if(TDeviceRealTerminal::Instance().BasePMS->Enabled ||
                               (!TRooms::Instance().Enabled && !TDeviceRealTerminal::Instance().BasePMS->Enabled))
                                TempReceipt->Transaction->Customer = TCustomer(0,0,"");
                            TempReceipt->SignReceipt = true;
                            TempReceipt->SenderType = devPC;
                            TempReceipt->Waiter = TDeviceRealTerminal::Instance().User.Name;
                            TempReceipt->MiscData["PartyName"] = PartyName;

                            Receipt->GetPrintouts(DBTransaction, TempReceipt.get(), TComms::Instance().ReceiptPrinter);
                            TempReceipt->Printouts->Print(TDeviceRealTerminal::Instance().ID.Type);

                            if (TGlobalSettings::Instance().PrintSignatureReceiptsTwice)
                            {
                                TempReceipt->Printouts->Print(TDeviceRealTerminal::Instance().ID.Type);
                            }

                            if (OrdersLoadedFromTabs)
                            {
                                while (InvoiceTransaction.Orders->Count != 0)
                                {
                                    delete(TItemComplete*)InvoiceTransaction.Orders->Items[0];
                                    InvoiceTransaction.Orders->Delete(0);
                                }
                                logList->Clear();
                                logList->Add("Order Deleted from Invoice transaction.");
                                TSaveLogs::RecordFiscalLogs(logList);
                            }
                        }
					}
				}

				if (!order_was_resumed_via_hold_and_send)
				{
					// SO FAR THIS FUNCTION CALL IS NEVER USED.
                    if(PaymentTransaction.CreditTransaction)
                      creditOrderToChefMate(SeatOrders);
				}
                //call method to send points details...
                SendPointValueToRunRate( PaymentTransaction );
				// Clear all Orders and membership.
				// Note you can have a sale with no orders but that has membership
				// which needs to be cleared anyway.
				lbDisplay->Clear();
				for (UINT iSeat = 0; iSeat < SeatOrders.size(); iSeat++)
				{
					for (int i = 0; i < SeatOrders[iSeat]->Orders->Count; i++)
					{
						delete SeatOrders[iSeat]->Orders->Items[i];
					}
					for (int j = 0; j < SeatOrders[iSeat]->Orders->PrevCount; j++)
					{
						delete SeatOrders[iSeat]->Orders->PrevItems[j];
					}
					SeatOrders[iSeat]->Orders->Clear();
					SeatOrders[iSeat]->Orders->ClearPrev();
					SeatOrders[iSeat]->Orders->AppliedMembership.Clear();
                    SeatOrders[iSeat]->Orders->pmsAccountDetails.RoomNumber = "";
                    SeatOrders[iSeat]->Orders->pmsAccountDetails.RoomBedNumber = "";
                    SeatOrders[iSeat]->Orders->pmsAccountDetails.FirstName = "";
                    SeatOrders[iSeat]->Orders->pmsAccountDetails.LastName = "";
				}
                logList->Clear();
                logList->Add("All seat order deleted.");
                TSaveLogs::RecordFiscalLogs(logList);

				AfterSaleProcessed.Occured();
                //clear phone number and customer name after successful sale
                TGlobalSettings::Instance().TabPrintName = "";
                TGlobalSettings::Instance().TabPrintPhone = "";
                OrdersList->Clear();
                memNote->Lines->Clear();
                memOverview->Lines->Clear();
                patronsStore.clear();
                storedPatronCountFromMenu = 0;
                PaymentTransaction.PatronCountFromMenu = 0;

                logList->Clear();
                logList->Add("End of processorder");
                TSaveLogs::RecordFiscalLogs(logList);

                delete logList;
                logList = NULL;
			}
            else
            {
                ExtractPatronInformation(PaymentTransaction);
            }
		}
		else
		{
			MessageBox("Nothing to Bill.", "Info", MB_OK + MB_ICONINFORMATION);
		}

        if(TGlobalSettings::Instance().CaptureCustomerName)
        {
            TCustNameAndOrderType::Instance()->ClearNameCaught();
        }
	}
	catch(EAbort & E)
	{
        if(!CanClose)
        {
	    	MessageBox(E.Message, "Abort", MB_OK + MB_ICONERROR);
        }

	}
	catch(Exception & E)
	{
        if(!CanClose)
        {
		MessageBox("Unable to process this order.\r" "Please report the following message to your service provider :\r\r" + E.Message, "Error", MB_OK + MB_ICONERROR);
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);

		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Tab Data Type :" + IntToStr(TabType) + " Selected Tab Key " + IntToStr(SelectedTab) + "Tab Name :" + TabName);
		PaymentComplete = false;
        TStringList* logList = new TStringList();
        logList->Clear();
        logList->Add("Exception in select dish process transaction().");
        TSaveLogs::RecordFiscalLogs(logList);

        delete logList;
        logList = NULL;
        }
	}

    if(PaymentComplete)
     {
        PaymentTransaction.PaymentsClear();
        TStringList* logList = new TStringList();
        logList->Clear();
        logList->Add("Payment info cleared.");
        TSaveLogs::RecordFiscalLogs(logList);

        delete logList;
        logList = NULL;
     }
    IsParkSalesEnable = false;
	return PaymentComplete;
}
// ---------------------------------------------------------------------------
int TfrmSelectDish::GetUpdatedPatronCount(Database::TDBTransaction &DBTransaction,int TableNumber,std::set<int> SeatCounter)
{
    int patronCount = TDBTables::GetPatronNumbers(DBTransaction,TableNumber);
    for (std::set<int>::iterator seatNumber = SeatCounter.begin(); seatNumber != SeatCounter.end(); advance(seatNumber,1))
        {
            if(!TDBTables::IsSeatPresent(DBTransaction,TableNumber,*seatNumber))
            {
              patronCount++;
            }
        }
    return patronCount;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tbtnChangeTableClick(TObject *Sender)
{
    //MM-1649: Clear out all the table patrons since we have cancelled the table selection...
    if(SelectedTable > 0)
    {
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);

        DBTransaction.StartTransaction();

        std::vector<TPatronType> selectedTablePatrons = TDBTables::GetPatronCount(DBTransaction, SelectedTable);
        int patronCount = GetCount(selectedTablePatrons);

        if(TGlobalSettings::Instance().PromptForPatronCount && patronCount > 0 && TDBTables::IsEmpty(DBTransaction, SelectedTable))
        {
            TDBTables::ClearPatronCount(DBTransaction, SelectedTable);
        }

        DBTransaction.Commit();
    }
	if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot &&
            TGlobalSettings::Instance().EnableCustomerJourney)
    {
    	SeatOrders[0]->isChangeTablePressed = true;
     }
	showTablePicker();
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::UpdateTableButton()
{
    tbtnSelectTable->Enabled = true;

      if(TGlobalSettings::Instance().ShowLargeFonts )
              {
               tbtnSelectTable->Font->Size = 18;
              }
      else
            {
             tbtnSelectTable->Font->Size = 12;
            }



	if (SelectedTable > 0)
	{
		tbtnSelectTable->ButtonColor = CL_BUTTONCOLOUR_SELECTED_SEAT;
		if (!OrdersPending())
		{
            //tbtnSelectTable->Enabled = !TGlobalSettings::Instance().EnableWaiterStation;


			if (SelectedParty != "")
			{

				if (SelectedTabContainerName != "")
				{
					tbtnSelectTable->Caption = "Bill: " + SelectedTabContainerName + " (" + SelectedParty + ")";
				}
				else
				{
					tbtnSelectTable->Caption = "Bill: " + IntToStr(SelectedTable) + " (" + SelectedParty + ")";
				}
			}
			else
			{

				if (SelectedTabContainerName != "")
				{
					tbtnSelectTable->Caption = "Bill: " + SelectedTabContainerName;
				}
				else
				{
					tbtnSelectTable->Caption = "Bill: " + IntToStr(SelectedTable);
				}
			}

		}
		else
		{
			if (SelectedParty != "")
			{
				if (SelectedTabContainerName != "")
				{
					tbtnSelectTable->Caption = "Save " + SelectedTabContainerName + " (" + SelectedParty + ")";
				}
				else
				{
					tbtnSelectTable->Caption = "Save Table " + IntToStr(SelectedTable) + " (" + SelectedParty + ")";
				}
			}
			else
			{
				if (SelectedTabContainerName != "")
				{
					tbtnSelectTable->Caption = "Save " + SelectedTabContainerName + " ";
				}
				else
				{
					tbtnSelectTable->Caption = "Save Table " + IntToStr(SelectedTable) + " ";
				}
			}
		}

}
	else
	{
		tbtnSelectTable->ButtonColor = CL_BUTTONCOLOUR_STD;
		tbtnSelectTable->Caption = "Select table";
        tbtnSelectTable->Font->Size = 12;

	}

          if(tbtnSelectTable->Caption == "Select table")
          {
          tbtnSelectTable->Width = 153;
          }
      else
          {
          tbtnSelectTable->Width = 180;
          }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::btnDollarMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	CurrentTenderButton = (TTouchBtn*)Sender;
	CurrentTenderButton->Tag = 0;
	TimerTender->Enabled = true;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::TimerTenderTimer(TObject *Sender)
{
   TimerTender->Enabled = false;
   std::auto_ptr<TfrmSelectPaymentTypeAndValue>frmSelectPaymentTypeAndValue(TfrmSelectPaymentTypeAndValue::Create<TfrmSelectPaymentTypeAndValue>(this));
   double amount = 0;
   if (TryStrToFloat(CurrentTenderButton->Caption,amount))
    {
      frmSelectPaymentTypeAndValue->InitialValue = StrToCurr( CurrentTenderButton->Caption );
    }
    else
    {
      frmSelectPaymentTypeAndValue->SelectedPayment = CurrentTenderButton->Caption;
    }

   if (frmSelectPaymentTypeAndValue->ShowModal() == mrOk)
    {
        AnsiString Result = "";

        if(frmSelectPaymentTypeAndValue->SelectedPayment != "")
        {
           Result = frmSelectPaymentTypeAndValue->SelectedPayment;
           CurrentTender = 0;
           tbtnTender->Caption = "Tender";
           tbtnCashSale->Caption = "Cash Sale";
           tbtnCashSale->Enabled = true;
           TotalCosts();
           UpdateExternalDevices();
        }
        else if(frmSelectPaymentTypeAndValue->NumericResult > 0)
        {
           Result = CurrToStrF(frmSelectPaymentTypeAndValue->NumericResult,
                                     ffNumber, CurrencyDecimals );
           CurrentTenderButton->Tag = 1;
        }

        if(Result != "")
        {
           Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	   DBTransaction.StartTransaction();
           int precision = 8;
           CurrentTenderButton->Caption = Result;

           if (CurrentTenderButton->Name == "tbtnDollar1")
            {
                    TManagerVariable::Instance().SetDeviceStr(DBTransaction, vmbtnDollar1, CurrentTenderButton->Caption);
            }
            else if (CurrentTenderButton->Name == "tbtnDollar2")
            {
                    TManagerVariable::Instance().SetDeviceStr(DBTransaction, vmbtnDollar2, CurrentTenderButton->Caption);
            }
            else if (CurrentTenderButton->Name == "tbtnDollar3")
            {
                    TManagerVariable::Instance().SetDeviceStr(DBTransaction, vmbtnDollar3, CurrentTenderButton->Caption);
            }
            else if (CurrentTenderButton->Name == "tbtnDollar4")
            {
                    TManagerVariable::Instance().SetDeviceStr(DBTransaction, vmbtnDollar4, CurrentTenderButton->Caption);
            }
            else if (CurrentTenderButton->Name == "tbtnDollar5")
            {
                    TManagerVariable::Instance().SetDeviceStr(DBTransaction, vmbtnDollar5, CurrentTenderButton->Caption);
            }
            DBTransaction.Commit();
        }

    }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::btnDollarMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TimerTender->Enabled = false;   //4775
    TTouchBtn *Btn;
    int pos;

    if ((Btn = dynamic_cast<TTouchBtn*>(Sender)) != NULL)
    {
        double amount = 0;
     //   UnicodeString _str = Btn->Caption;
     //   AnsiString _astr = _str;
     //   RemoveCharsFromString( Btn->Caption.c_str(), "," );
         do
         {
            pos = Btn->Caption.Pos( "," );
            Btn->Caption = Btn->Caption.Delete( pos, 1 );   // Remove all commas
         }
	     while( pos > 0 );

        if (!TryStrToFloat(Btn->Caption,amount))
        {
           if (SeatOrders[0]->Orders->Count > 0)
            {
               if(SaveTransactionDetails(UnicodeString(Btn->Caption))) //
               {
                 ProcessQuickPayment(Sender,Btn->Caption);
               }

            }
           else
           {
              	MessageBox("Nothing to Bill.", "Info", MB_OK + MB_ICONINFORMATION);
           }
        }
        else
        {
            if (Btn->Tag == 0) // Toggle
            {
                if (SeatOrders[0]->Orders->Count > 0)
                {
                        CurrentTender += BtnCaptionToCurrency( Btn );
                        tbtnTender->Caption = "Tender " + CurrToStrF(CurrentTender, ffNumber, CurrencyDecimals);
                        tbtnCashSale->Caption = "Clear Tender";
                        tbtnCashSale->Enabled = true;
                        TotalCosts();
                        UpdateExternalDevices();
                }
            }
            Btn->Tag = 0;
        }
    }
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::ProcessQuickPayment(TObject *Sender,AnsiString paymentName)
{
//4775
    CurrentTender = 0;
    tbtnTender->Caption = "Tender";
    tbtnCashSale->Caption = "Cash Sale";
    tbtnCashSale->Enabled = true;
    TotalCosts();
    UpdateExternalDevices();
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
    DBTransaction.StartTransaction();
    bool PaymentComplete = ProcessOrders(Sender, DBTransaction, 0, // Tab
			TabCashAccount, // Tab Type
		        "Sale", // Tab Container Name
			"Sale", // Tab Name
			"", // PartyName
			false, // Print Prelim Receipt.
			0, // Table
			0, // Seat
			0); // Room

     TStringList* logList = new TStringList();
     logList->Clear();

    if (PaymentComplete)
    {
        DBTransaction.Commit();
        ResetPOS();
        logList->Add("Trabsaction commit of tbtnSaveMouseClick().");
        TSaveLogs::RecordFiscalLogs(logList);
    }
    else
    {
        logList->Add("Payment not completed in ProcessQuickPayment().");
        TSaveLogs::RecordFiscalLogs(logList);
    }

   AutoLogOut();
   if(TGlobalSettings::Instance().EnableTableDisplayMode)
    {
      showTablePicker();
    }
    DisplayRoomNoUI();
    NagUserToSelectChit();
}
// ---------------------------------------------------------------------------
bool TfrmSelectDish::IsProcessQuickPayment(TObject *Sender)
{
   return  (Sender ==  tbtnDollar1) ||
           (Sender ==  tbtnDollar2) ||
           (Sender ==  tbtnDollar3) ||
           (Sender ==  tbtnDollar4) ||
           (Sender ==  tbtnDollar5);

}
// ---------------------------------------------------------------------------
AnsiString TfrmSelectDish::GetQuickPaymentName(TObject *Sender)
{
    TTouchBtn *Btn;
    if ((Btn = dynamic_cast<TTouchBtn*>(Sender)) != NULL)
    {
      return Btn->Caption;
    }
   return "";
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tbtnRunProgramClick()
{
	std::auto_ptr<TfrmMessage>frmMessage(TfrmMessage::Create<TfrmMessage>(this, TDeviceRealTerminal::Instance().DBControl));
	frmMessage->MessageType = eRunProgram;
	if (frmMessage->ShowModal() == mrOk)
	{
		ManagerRun->RunProgram(frmMessage->TextResult);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tbtnLuckyMemberClick()
{
	if (TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Enabled"])
	{
		TMMContactInfo LuckyMember;
		UnicodeString LocationFilter = "";
		if (TGlobalSettings::Instance().LuckyDrawByLocationOnly)
		{
			LocationFilter = TDeviceRealTerminal::Instance().ID.Location;
		}

		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
		DBTransaction.StartTransaction();
		TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetLuckyMember(DBTransaction, LuckyMember, TGlobalSettings::Instance().LuckyMemberPeriod, LocationFilter);
		DBTransaction.Commit();
		if (LuckyMember.ContactKey == 0)
		{
			MessageBox("There are no Lucky members to draw from.", "Lucky Member", MB_OK + MB_ICONINFORMATION);
		}
		else
		{
			MessageBox("The Lucky Member is\rName : " + LuckyMember.Name + "\rKnown As : " + LuckyMember.Alias + "\rNo. " + LuckyMember.MembershipNumber, "Lucky Member", MB_OK + MB_ICONINFORMATION);
		}
	}
	else
	{
		MessageBox("Membership is not Enabled.", "Error", MB_OK + MB_ICONERROR);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tbtnCallAwayClick()
{
	try
	{
		std::auto_ptr<TCallAwayComplete>CallAway(new TCallAwayComplete);
                Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);


		if (SelectedTable != 0)
		{
                        DBTransaction.StartTransaction();
                        UnicodeString tablename =  TDBTables::GetTableName(DBTransaction, SelectedTable);
			if (MessageBox("Do you wish to call away " + tablename + " ?", "Error", MB_YESNO + MB_ICONQUESTION) == ID_YES)
			{
				CallAway->TableNo = SelectedTable;
			}
                        DBTransaction.Commit();
		}

		if (CallAway->TableNo == 0)
		{
            std::auto_ptr<TEnableFloorPlan> floorPlan(new TEnableFloorPlan());
			TFloorPlanReturnParams floorPlanReturnParams;

			// Runs new web app of floorPlan
            if(floorPlan->Run( ( TForm* )this, true, floorPlanReturnParams ) )
//                      if( TEnableFloorPlan::Instance()->Run( ( TForm* )this, true, floorPlanReturnParams ) )
			{
 //				FloorPlanReturnParams = FloorPlan->RunFloorPlan();
				CallAway->TableNo = floorPlanReturnParams.TabContainerNumber;
			}
            floorPlan.reset();
		}

		if (CallAway->TableNo != 0)
		{

			DBTransaction.StartTransaction();

			CallAway->TableName = TDBTables::GetTableName(DBTransaction, CallAway->TableNo);
			CallAway->PartyName = TDBTables::GetPartyName(DBTransaction, CallAway->TableNo);

			std::auto_ptr<TfrmMessage>frmMessage(TfrmMessage::Create<TfrmMessage>(this, TDeviceRealTerminal::Instance().DBControl));
			frmMessage->MessageType = eCallAway;
			if (frmMessage->ShowModal() == mrOk)
			{
				CallAway->Courses->AddStrings(frmMessage->TextResults.get());

				std::auto_ptr<TPaymentTransaction>TempTransaction(new TPaymentTransaction(DBTransaction));
				std::auto_ptr<TReqPrintJob>NormalRequest(new TReqPrintJob(&TDeviceRealTerminal::Instance()));
				NormalRequest->Transaction = TempTransaction.get();
				NormalRequest->SenderType = devPalm;
				NormalRequest->TimeStamp = Now();
				NormalRequest->Waiter = TDeviceRealTerminal::Instance().User.Name;
				NormalRequest->Transaction->Membership.Assign(Membership);
				NormalRequest->MiscData["PartyName"] = CallAway->PartyName;

				NormalRequest->Transaction->Money.Recalc(*NormalRequest->Transaction);
                                std::auto_ptr<TKitchen> Kitchen(new TKitchen());
                                Kitchen->Initialise(DBTransaction);
				Kitchen->GetPrintouts(DBTransaction, CallAway.get(), NormalRequest.get());
				callAwayToChefMate(CallAway.get());
				NormalRequest->Printouts->Print(devPC);
                                //Set Table status for call away
                                TDBTables::SetTableBillingStatus(DBTransaction,CallAway->TableNo,eCallAwayStatus);
			}
			DBTransaction.Commit();
		}
	}
	catch(Exception & E)
	{
		MessageBox("An Error has Occured.\r" + E.Message, "Error", MB_OK + MB_ICONERROR);
	}
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::OnMenuChange(TSystemEvents *Sender)
{
	CurrentServingCourse.Reset(TDeviceRealTerminal::Instance().Menus->DefaultServingCourse);
	RedrawServingCourses();
	RedrawMenu();
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::OnTillsMenusAltered(TSystemEvents *Sender)
{
	// New Menus are pending.
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::NagUserToSelectChit()
{
    try
    {
        if (TGlobalSettings::Instance().NagUserToSelectChit && !ChitNumber.Assigned() && Active)
        {
            Database::TDBTransaction t(TDeviceRealControl::ActiveInstance().DBControl);

            t.StartTransaction();
            SetupChit(t);

            t.Commit();
        }
    }
    catch(Exception & E)
    {
         TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::OnBeforeItemOrdered(TSystemEvents *Sender)
{
	if (Always_Prompt)
	{
		CurrentServingCourse.Reset(TDeviceRealTerminal::Instance().Menus->DefaultServingCourse);
		SetSelectedServingCourse(GetServingCourse());
	}

	if (CurrentServingCourse.Name == "")
	{
		Sender->ErrorMessage = "You Must Select a Serving Course.";
		Sender->ErrorTitle = "No Serving Course Selected.";
		Sender->HaltProceedingEvents = true;
		Sender->EventsFailed = true;
	}
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::TransactionComplete(TSystemEvents *Sender)
{
	Currency LastTotal = TDeviceRealTerminal::Instance().PaymentSystem->LastTotal;
	Currency LastChange = TDeviceRealTerminal::Instance().PaymentSystem->LastChange;
	// Update Transaction Totals.
	lbeTotalCost->Caption = CurrToStrF(LastTotal, ffNumber, CurrencyDecimals) + " ";
	lbeChange->Caption    = CurrToStrF(LastChange, ffNumber, CurrencyDecimals) + " ";

	// Update Pole Display.
	if (LastTotal != 0 || LastChange != 0)
	{
		TDeviceRealTerminal::Instance().PoleDisplay->UpdatePoleDisplay("Totl", CurrToStrF(LastTotal, ffNumber, CurrencyDecimals), "Change", CurrToStrF(LastChange, ffNumber, CurrencyDecimals));
	}
	else
	{
		TDeviceRealTerminal::Instance().PoleDisplay->UpdatePoleDisplayDefault();
	}
	LastSale = Now();

	//::::::::::::::::::::::::::::::::::::::::::
     customerDisp.TierLevel=     TGlobalSettings::Instance().TierLevelChange ;

    if( Sender != NULL )
    {
        customerDisplayOrderPaid( LastTotal, LastChange );
    }
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::OnLockOutTimer(TSystemEvents *Sender)
{
   LockOutUser();
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::LockOutUser()
{
   	if ((Active && IsAutoLogOutInSelectDish)|| isRoomNoUiCalled || TGlobalSettings::Instance().IsAutoLoggedOut)
	{
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();
		std::auto_ptr<TContactStaff>Staff(new TContactStaff(DBTransaction));
		TLoginSuccess Result = lsDenied;
        TGlobalSettings::Instance().IsAutoLoggedOut = false;
        bool PaymentAccessResult = true;
        isRoomNoUiCalled = false;
			try
			{
                while (Result == lsDenied || Result == lsPINIncorrect )
                {
                    TMMContactInfo TempUserInfo;
                    Result = Staff->Login(this, DBTransaction, TempUserInfo, CheckPOS);
                    bool PaymentAccessResult = Staff->TestAccessLevel(TempUserInfo, CheckPaymentAccess);
                    IsWaiterLogged =  !PaymentAccessResult;

                    if (Result == lsAccepted)
                    {
                        if(CloseChitForm())
                        {
                            TGlobalSettings::Instance().IsAutoLoggedOut = true;
                        }
                        bool LogIn = false;
                        if(Screen->ActiveForm->ClassNameIs("TfrmTouchNumpad"))
                        {
                            isRoomNoUiCalled = true;
                        }
                        if (TDeviceRealTerminal::Instance().User.ContactKey != TempUserInfo.ContactKey)
                        {
                            if (!StaffChanged(TempUserInfo))
                            {
                                Result = lsDenied; // Loop back and have another go.
                            }
                        }
                        else
                        {
                            TManagerLogs::Instance().Add(__FUNC__, DEBUGLOG, "Staff not swapped out Contact ID's Match: " + IntToStr(TDeviceRealTerminal::Instance().User.ContactID));

                        }
                        if (Result == lsAccepted)
                        {
                           UpdateMenuItemsAfterLoginScreen();
                        }

                    }
                    else if (Result == lsDenied)
                    {
                        MessageBox("You do not have access to item sales.", "Error", MB_OK + MB_ICONERROR);
                    }
                    else if (Result == lsPINIncorrect)
                    {
                        MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
                    }
                    else if (Result == lsCancel)
                    {
                        CanClose = true;
                        FormCloseQuery(NULL, CanClose);
                        if (CanClose)
                        {
                            IsChitPromptFormActive=false;
                            Close();
                           if(CloseChitForm())
                           {
                                Screen->ActiveForm->Close();
                           }
                           IsAutoLogOutInSelectDish = false;

                        }
                        else
                        {
                            Result = lsDenied;
                        }
                    }
			}

			}
			catch(Exception & E)
			{
				//MessageBox("Auto-lockout Login Error Please write this down and report it to MenuMate Ltd :" + E.Message, "Error", MB_OK + MB_ICONERROR);
				TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Auto-lockout Login Error Please write this down and report it to MenuMate Ltd :" + E.Message);
				Result = lsDenied;
                CloseActiveForm();
			}

		DBTransaction.Commit();
		TDeviceRealTerminal::Instance().ResetEventLockOutTimer();
      // tiChitDelay->Enabled = TGlobalSettings::Instance().NagUserToSelectChit
                              //&& Result == lsAccepted;
        InitializeQuickPaymentOptions();

	}
}
// ---------------------------------------------------------------------------

bool TfrmSelectDish::CloseChitForm()
{
    bool CloseChitRetval=false;
     if(
        (Screen->ActiveForm->Name == "frmTouchKeyboard")    ||
        (Screen->ActiveForm->Name == "frmTouchNumpad")      ||
        (Screen->ActiveForm->Name == "frmCaptNamePhone")    ||
        (Screen->ActiveForm->Name == "frmVerticalSelect")   ||
        (Screen->ActiveForm->Name == "frmChitList")         ||
        (Screen->ActiveForm->Name == "frmMessageBox") ||
        Screen->ActiveForm->ClassNameIs("TfrmSelectDateTime"))
    {    
        CloseChitRetval=true;
    }
    return CloseChitRetval;
}

bool TfrmSelectDish::StaffChanged(TMMContactInfo TempUserInfo)
{
	bool RetVal = false;
	bool OrdersTaken = false;

	for (UINT i = 0; i < SeatOrders.size(); i++)
	{
		if (SeatOrders[i]->Orders->Count > 0)
		{
			OrdersTaken = true;
			break;
		}
	}

	if (OrdersTaken)
	{
		if (MessageBox("There is a partially complete order. Continue?", "Error", MB_OKCANCEL + MB_ICONQUESTION) == ID_OK)
		{
			tbtnUserName->Caption = TempUserInfo.Name;
			TDeviceRealTerminal::Instance().User = TempUserInfo;
			CloseSidePanel(); // Make them re-open it to check security.
			RetVal = true;
            Refresh();
		}
	}
	else
	{
		tbtnUserName->Caption = TempUserInfo.Name;
		TDeviceRealTerminal::Instance().User = TempUserInfo;
		RetVal = true;
        Refresh();
	}

	if (RetVal)
	{
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();
		std::auto_ptr<TContactStaff>Staff(new TContactStaff(DBTransaction));
        bool PaymentAccessResult = Staff->TestAccessLevel(TempUserInfo, CheckPaymentAccess);
        IsWaiterLogged =  !PaymentAccessResult;
        InitializeQuickPaymentOptions();
        DBTransaction.Commit();
		for (UINT iSeat = 0; iSeat < SeatOrders.size(); iSeat++)
		{
			for (int i = 0; i < SeatOrders[iSeat]->Orders->Count; i++)
			{
				TItemComplete *Order = SeatOrders[iSeat]->Orders->Items[i];
				TSecurityReference SecRef;
				SecRef.UserKey = TDeviceRealTerminal::Instance().User.ContactKey;
				SecRef.Event = SecurityTypes[secOrderedBy];
				SecRef.From = TDeviceRealTerminal::Instance().User.Name;
				SecRef.To = TDeviceRealTerminal::Instance().User.Initials;
				SecRef.TimeStamp = Now();
				Order->Security->SecurityUpdate(secOrderedBy, SecRef);
			}
		}
		TDeviceRealTerminal::Instance().ResetEventLockOutTimer();
	}
	return RetVal;
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::OnAfterItemOrdered(TSystemEvents *Sender)
{
    if (TGlobalSettings::Instance().NagUserToSelectChit && !ChitNumber.Assigned())
    {
        Database::TDBTransaction t(TDeviceRealControl::ActiveInstance().DBControl);
        t.StartTransaction();
            SetupChit(t);
        t.Commit();
    }  
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::RedrawServingCourses()
{
	tgridServingCourse->ColCount = 0;
	tgridServingCourse->RowCount = 0;

	if (TDeviceRealTerminal::Instance().Menus->VisibleMenu != NULL)
	{
		std::vector<TServingCourse>::iterator itServingCourse;

		int ColCount = 0;
		for (itServingCourse = TDeviceRealTerminal::Instance().Menus->VisibleMenu->ServingCourses.begin(); itServingCourse != TDeviceRealTerminal::Instance().Menus->VisibleMenu->ServingCourses.end();
			itServingCourse++)
		{
			if (itServingCourse->Selectable)
			{
				ColCount++;
			}
		}

		if (ColCount != 0)
		{
			tgridServingCourse->ColCount = ColCount;
			tgridServingCourse->RowCount = 1;

			int i = 0;
			for (itServingCourse = TDeviceRealTerminal::Instance().Menus->VisibleMenu->ServingCourses.begin();
				itServingCourse != TDeviceRealTerminal::Instance().Menus->VisibleMenu->ServingCourses.end(); itServingCourse++)
			{
				if (itServingCourse->Selectable)
				{
                    tgridServingCourse->Font->Size=14;
					tgridServingCourse->Buttons[0][i]->Caption = itServingCourse->Name;
                    tgridServingCourse->Buttons[0][i]->Color = ButtonColors[BUTTONTYPE_UNSELECTED][ATTRIB_BUTTONCOLOR];
					tgridServingCourse->Buttons[0][i]->FontColor = ButtonColors[BUTTONTYPE_UNSELECTED][ATTRIB_FONTCOLOR];
					tgridServingCourse->Buttons[0][i]->LatchedColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_BUTTONCOLOR];
					tgridServingCourse->Buttons[0][i]->LatchedFontColor = clWhite;//ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_FONTCOLOR];
					tgridServingCourse->Buttons[0][i]->Tag = itServingCourse->ServingCourseKey;
					tgridServingCourse->Buttons[0][i]->Visible = itServingCourse->Selectable;
					i++;
				}
			}
		}
	}
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::RedrawServingCoursesOverview()
{
	std::vector<TServingCourse>::iterator itServingCourse;

	tgridItemServingCourse->ColCount = TDeviceRealTerminal::Instance().Menus->VisibleMenu->ServingCourses.size();
	tgridItemServingCourse->RowCount = 1;

	int i = 0;
	for (itServingCourse = TDeviceRealTerminal::Instance().Menus->VisibleMenu->ServingCourses.begin(); itServingCourse != TDeviceRealTerminal::Instance().Menus->VisibleMenu->ServingCourses.end();
		itServingCourse++)
	{
		tgridItemServingCourse->Buttons[0][i]->Caption = itServingCourse->Name;
		tgridItemServingCourse->Buttons[0][i]->Color = ButtonColors[BUTTONTYPE_UNSELECTED][ATTRIB_BUTTONCOLOR];
		tgridItemServingCourse->Buttons[0][i]->FontColor = clBlack;//ButtonColors[BUTTONTYPE_UNSELECTED][ATTRIB_FONTCOLOR];
		tgridItemServingCourse->Buttons[0][i]->LatchedColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_BUTTONCOLOR];
		tgridItemServingCourse->Buttons[0][i]->LatchedFontColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_FONTCOLOR];
		tgridItemServingCourse->Buttons[0][i]->Tag = itServingCourse->ServingCourseKey;
		i++;
	}

	// Select the Current items Serving course if any.
	if (lbDisplay->ItemIndex > -1)
	{
		TItemRedirector *ListItem = (TItemRedirector*)lbDisplay->Items->Objects[lbDisplay->ItemIndex];
		if ((ListItem->ItemType.Contains(itNormalItem) || ListItem->ItemType.Contains(itSetMenuMasterItem)) && !ListItem->ItemType.Contains(itPrevItem))
		{
			TItemComplete * Item = (TItemComplete*)ListItem->ItemObject;

			for (int i = 0; i < tgridItemServingCourse->ColCount; i++)
			{
				if (tgridItemServingCourse->Buttons[0][i]->Tag == Item->ServingCourse.ServingCourseKey)
				{
					tgridItemServingCourse->Buttons[0][i]->Color = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_BUTTONCOLOR];
					tgridItemServingCourse->Buttons[0][i]->FontColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_FONTCOLOR];
				}
			}
		}
	}

}
// ---------------------------------------------------------------------------
void TfrmSelectDish::SetSelectedServingCourse(int SelectedServingCourse)
{
	if (SelectedServingCourse == 0)
	{
		if (TGlobalSettings::Instance().RememberLastServingCourse)
		{
			if (!CurrentServingCourse.Selectable)
			{
				CurrentServingCourse.Reset(TDeviceRealTerminal::Instance().Menus->DefaultServingCourse);
			}
			else
			{
				SelectedServingCourse = CurrentServingCourse.ServingCourseKey;
			}
		}
		else
		{
			CurrentServingCourse.Reset(TDeviceRealTerminal::Instance().Menus->DefaultServingCourse);
		}
	}

	if (SelectedServingCourse != 0 && TDeviceRealTerminal::Instance().Menus->VisibleMenu != NULL)
	{
		CurrentServingCourse = TDeviceRealTerminal::Instance().Menus->VisibleMenu->GetServingCourse(SelectedServingCourse);
		TDeviceRealTerminal::Instance().Menus->VisibleMenu->LastSelectedServingCourse = CurrentServingCourse;

	}
	for (int i = 0; i < tgridServingCourse->ColCount; i++)
	{
		if (CurrentServingCourse.ServingCourseKey == tgridServingCourse->Buttons[0][i]->Tag)
		{
			tgridServingCourse->Buttons[0][i]->Color = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_BUTTONCOLOR];
			tgridServingCourse->Buttons[0][i]->Font->Color = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_FONTCOLOR];
		}
		else
		{
			tgridServingCourse->Buttons[0][i]->Color = ButtonColors[BUTTONTYPE_UNSELECTED][ATTRIB_BUTTONCOLOR];
			tgridServingCourse->Buttons[0][i]->FontColor = clBlack;//ButtonColors[BUTTONTYPE_UNSELECTED][ATTRIB_FONTCOLOR];
		}
        if(tgridServingCourse->Buttons[0][i]->Color == clNavy)
            tgridServingCourse->Buttons[0][i]->FontColor = clWhite;
	}
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::OnAfterSaleProcessed(TSystemEvents *Sender)
{
    ResetChit();
	CloseSidePanel();
	RedrawServingCourses();
	if (TGlobalSettings::Instance().RememberLastServingCourse)
	{
		SetSelectedServingCourse(CurrentServingCourse.ServingCourseKey);
	}
	else
	{
		CurrentServingCourse.Reset(TDeviceRealTerminal::Instance().Menus->DefaultServingCourse);
	}
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::RefreshModifyGui(TSystemEvents *Sender)
{
	if (lbDisplay->ItemIndex > -1)
	{
		if (pnlItemModify->Visible == true)
		{
			OpenSidePanel();
		}
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::OpenSidePanel()
{
	// pcItemModify->ActivePage = tsOverview;
	RedrawModifyOptionsBtnGrid();
	pnlItemModify->Visible = true;
	pnlItemModify->BringToFront();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::CloseSidePanel()
{
	pnlItemModify->Visible = false;
	pnlItemModify->SendToBack();
	btngridModify->RowCount = 0;
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::RemoveItem(Database::TDBTransaction &DBTransaction)
{
   bool Allowed = true;
   bool suffleDiscounds = false;
    if (lbDisplay->ItemIndex > -1)
    {
        TItemRedirector *ListItem = (TItemRedirector*)lbDisplay->Items->Objects[lbDisplay->ItemIndex];
		if (ListItem->ItemType.Contains(itSubItem))
		{
			if (!ListItem->ItemType.Contains(itPrevItem))
			{
				// SubItem is stored in a TList.
				int CurrentIndex = ListItem->SubOrdersList->IndexOf(ListItem->ItemObject);
				if (CurrentIndex != -1)
				{
                    TItemCompleteSub *subitem =reinterpret_cast<TItemCompleteSub *>(ListItem->ItemObject);
                    TItemComplete * Item = (TItemComplete*)ListItem->ItemObject;
                    if(Item->PrintCancel)
                    {//
                      TMMContactInfo TempUserInfo;
                      TempUserInfo = TDeviceRealTerminal::Instance().User;
                      std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
                      Allowed = Staff->TestAccessLevel(TempUserInfo, CheckCredit);
                      AnsiString Username;
                      if(!Allowed)
                      {
                        TempUserInfo.Clear();
                        TLoginSuccess Result = Staff->Login(this, DBTransaction, TempUserInfo, CheckCredit);
                        if (Result == lsAccepted)
                        {
                          Allowed = true;

                        }
                        else if (Result == lsDenied)
                        {
                           Allowed = false;
                           MessageBox("You do not have access rights to Cancels.", "Error", MB_OK + MB_ICONERROR);
                        }
                        else if (Result == lsPINIncorrect)
                        {
                          Allowed = false;
                          MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
                        }
                      }
                      if(Allowed)
                      {
                        Username = TempUserInfo.Name;
                        CancelItemsBill(Item, Username);
                      }
                    }

                   if(Allowed)
                    {
                        std::auto_ptr<TList>List(new TList());
                        List->Add(ListItem->ItemObject);
                        TManagerFreebie::UndoFreeCount(DBTransaction, List.get());
                        RemoveSideItemFromItem(subitem,reinterpret_cast<TItemComplete *>(ListItem->ParentRedirector->ItemObject));
                        ListItem->SubOrdersList->Remove(ListItem->ItemObject);
                        delete(TItemCompleteSub*)ListItem->ItemObject;
                        (TItemCompleteSub*)ListItem->ItemObject = NULL;
                        ListItem->ItemObject = NULL;
                        CurrentIndex--;
                        if (CurrentIndex > -1)
                        {
                            ListItem->CompressedContainer->Container->LastItemSelected = (TItemMinorComplete*)ListItem->SubOrdersList->Items[CurrentIndex];
                        }
                        suffleDiscounds = true;

                    }
				}
			}
		}
		else if (ListItem->ItemType.Contains(itSetMenuMasterItem))
		{
			if (!ListItem->ItemType.Contains(itPrevItem))
			{
				// Remove and Setmenu Items and the key.
				int GroupToDelete = ((TItemComplete*)ListItem->ItemObject)->SetMenuGroup;
				int i = 0;
				while (i < SeatOrders[SelectedSeat]->Orders->Count)
				{
					if (SeatOrders[SelectedSeat]->Orders->Items[i]->SetMenuMask != 0)
					{
						if (SeatOrders[SelectedSeat]->Orders->Items[i]->SetMenuGroup == GroupToDelete)
						{
                            SeatOrders[SelectedSeat]->Orders->Items[i]->ReturnToAvailability();
							delete SeatOrders[SelectedSeat]->Orders->Items[i];
							SeatOrders[SelectedSeat]->Orders->Delete(i);
						}
						else
						{
							i++;
						}
					}
					else
					{
						i++;
					}
				}
                suffleDiscounds = true;
			}
		}
		else if (ListItem->ItemType.Contains(itOption))
		{
			if (!ListItem->ItemType.Contains(itPrevItem))
			{
				TItemOption * Option = (TItemOption*)ListItem->ItemObject;
				ListItem->OptList->OptionDelete(Option);
			}
		}
		else if (ListItem->ItemType.Contains(itMembershipDisplay) || ListItem->ItemType.Contains(itMembershipDisplayNote))
		{
            RemoveMembership(DBTransaction);
            if (dc_item_show)
            {
              Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
              DBTransaction.StartTransaction();
              bool Proceed = DeleteUnsentDCAndProceed(DBTransaction);
              lbDisplay->Clear();
              AnsiString memberNo = "";
              int end_idx , strt_idx = 0;
              char*  memberDetails_char = new char[memberName.Length() + 1];
              strcpy(memberDetails_char,memberName.c_str());
              int len = 0;
              for (int i = memberName.Length()-1 ; i > 0 ; i--)
              {
                 if (memberDetails_char[i] == ')' )
                 {
                    end_idx = i;
                 }
                 if (memberDetails_char[i] == '(' )
                 {
                    strt_idx = i;
                    len = (end_idx - strt_idx -1);
                    break;
                 }
              }
               memberNo = memberName.SubString(strt_idx+2, len);
               AnsiString cardId = TDrinkCommandData::Instance().GetCardIdFromMemberNo(memberNo) ;
               TDrinkCommandData::Instance().UpdateTimeStampToNull(cardId)  ;
               dc_item_show = false;
            }
            else
            {
               RemoveMembership(DBTransaction);
            }
           CheckMandatoryMembershipCardSetting(tbtnMembership);
		}
		else if (ListItem->ItemType.Contains(itDiscountDisplay))
		{
            if(TGlobalSettings::Instance().IsThorlinkSelected)
            {
                ManagerDiscount->ClearThorVouchersDiscounts(SeatOrders[SelectedSeat]->Orders->List);
            }
			ManagerDiscount->ClearDiscounts(SeatOrders[SelectedSeat]->Orders->List);
			/* Clear any points that were to be redeemed though discounts */
			SeatOrders[SelectedSeat]->Orders->AppliedMembership.Points.ClearBySource(pasDiscount);
		}
		else if (ListItem->ItemType.Contains(itNormalItem))
		{
			int qty;
			TItemComplete * Item = (TItemComplete*)ListItem->ItemObject;;
            if( Item->ItemKey > 0 )
                Item->ReturnToAvailability();

			/* Handheld logic changes only the quantity of an item. This is the
          * same when you alter the item's quantity in the Quantity screen.
			 *
			 * Compare this with the situation where you order many of the same
			 * item by ordering it several times. In this case, there are really
			 * multiple copies of that item stored. The order-list draw logic
			 * compresses these into a single list entry. When you remove the item
			 * in the list, it really removes one of those multiple items in the
          * list of ordered items.
          */

          if(Item->PrintCancel)
            {// login details....
              TMMContactInfo TempUserInfo;
              TempUserInfo = TDeviceRealTerminal::Instance().User;
              std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
              Allowed = Staff->TestAccessLevel(TempUserInfo, CheckCredit);
              AnsiString Username;
              if(!Allowed)
              {
                TempUserInfo.Clear();
                TLoginSuccess Result = Staff->Login(this, DBTransaction, TempUserInfo, CheckCredit);
                if (Result == lsAccepted)
                {
                  Allowed = true;

                }
                else if (Result == lsDenied)
                {
                   Allowed = false;
                   MessageBox("You do not have access rights to Cancels.", "Error", MB_OK + MB_ICONERROR);
                }
                else if (Result == lsPINIncorrect)
                {
                  Allowed = false;
                  MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
                }
              }
              if(Allowed)
              {
                Username = TempUserInfo.Name;
                CancelItemsBill(Item, Username);
              }
            }

          if(Allowed)
          {
                bool isFullRemove = false;

                if ((qty = Item->GetQty()) > 1)
                {
                   if(Item->WeightedPrice.WeightedItem)
                    {
                       isFullRemove = true;
                    }
                    else
                    {
                      Item->SetQtyCustom(Currency(qty) - 1);
                    }
                   // check value for more than 1 qty..
                }
                else
                {
                   isFullRemove = true;
                }

              if(isFullRemove)
              {
                if(Item->DiscountFind(DiscModeCombo) || Item->DiscountFind(DiscModeDeal))
                    {
                        ManagerDiscount->ClearDiscounts(SeatOrders[SelectedSeat]->Orders->List);
                        /* Clear the Old points used though discounts.*/
                        Membership.Member.Points.ClearBySource(pasDiscount);
                    }
                    AnsiString PoleDisplayPrice = CurrToStrF(-(Item->Price()), ffNumber, CurrencyDecimals);
                    TDeviceRealTerminal::Instance().PoleDisplay->UpdatePoleDisplay("-" + Item->Item, PoleDisplayPrice, "", "");
                    std::auto_ptr<TList>List(new TList());
                    List->Add(Item);
                    TManagerFreebie::UndoFreeCount(DBTransaction, List.get());
                   if (SeatOrders[SelectedSeat]->Orders->Remove(Item) != -1)
                    {
                       ListItem->ItemObject = NULL;
                       delete Item;
                       Item = NULL;
                    }

              }
              suffleDiscounds = true;
              // update discount list..
              /*int count = DiscountDetails.size();
              for(std::vector<TDiscountDetails>::iterator it = DiscountDetails.begin(); it != DiscountDetails.end(); ++it)
              {
                 for(int k = 0; k < it->OrderList->Count; k++)
                 {
                     TItemMinorComplete *CheckItem = (TItemMinorComplete *)it->OrderList->Items[k];
                     if(CheckItem == Item)
                     {
                        //int index = it->OrderList->IndexOf(CheckItem);
                        it->OrderList->Remove(CheckItem);
                        break;
                     }
                 }
              }*/

          }
        }

        CheckDeals(DBTransaction);
		RedrawSeatOrders();
		HighlightSelectedItem();
        if (lbDisplay->ItemIndex > -1)
        {
            if (pnlItemModify->Visible == true)
            {
                OpenSidePanel();
            }
        }
		AfterSelectedItemChanged.Occured();
        if(suffleDiscounds)
        {
           ManageDiscounts();
           RedrawSeatOrders();
           HighlightSelectedItem();
        }

  }
}
// ---------------------------------------------------------------------------
int TfrmSelectDish::GetServingCourse()
{
	int RetVal = 0;
	try
	{
		// Display Reports List
		std::auto_ptr<TfrmVerticalSelect>SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

		std::vector<TServingCourse>::iterator itServingCourse;
		int i = 1;
		for (itServingCourse = TDeviceRealTerminal::Instance().Menus->VisibleMenu->ServingCourses.begin(); itServingCourse != TDeviceRealTerminal::Instance().Menus->VisibleMenu->ServingCourses.end();
			itServingCourse++)
		{
			if (itServingCourse->Selectable)
			{
				TVerticalSelection Item1;
				Item1.Title = itServingCourse->Name;
				Item1.Properties["Action"] = IntToStr(itServingCourse->ServingCourseKey);
				Item1.Properties["Color"] = IntToStr(clNavy);
               	Item1.CloseSelection = true;
				SelectionForm->Items.push_back(Item1);
			}
			i++;
		}

		SelectionForm->ShowModal();
		TVerticalSelection SelectedItem;
		if (SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel")
		{
			RetVal = StrToIntDef(SelectedItem.Properties["Action"], 0);
		}
	}
	catch(Exception & E)
	{
		MessageBox(E.Message, "Error", MB_OK + MB_ICONERROR);
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
	return RetVal;
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::ChangeItemsServingCourse()
{
	if (lbDisplay->ItemIndex > -1)
	{
		TItemRedirector *ListItem = (TItemRedirector*)lbDisplay->Items->Objects[lbDisplay->ItemIndex];
		if ((ListItem->ItemType.Contains(itNormalItem) || ListItem->ItemType.Contains(itSetMenuMasterItem)) && !ListItem->ItemType.Contains(itPrevItem))
		{
			TItemComplete * Item = (TItemComplete*)ListItem->ItemObject;
			int SelectedServingCourse = GetServingCourse();
			if (SelectedServingCourse != 0 && TDeviceRealTerminal::Instance().Menus->VisibleMenu != NULL)
			{
				TServingCourse NewServingCourse = TDeviceRealTerminal::Instance().Menus->VisibleMenu->GetServingCourse(SelectedServingCourse);
				Item->ServingCourse = NewServingCourse;
			}
			RedrawSeatOrders();
		}
	}
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::SetReceiptPreview(Database::TDBTransaction &DBTransaction, TStringList * ReceiptDisplay, TMMTabType TabType, AnsiString TabContainerName, AnsiString TabName,
	AnsiString PartyName, int SelectedTab, int TableNo, int SeatNo, int RoomNo)

{
	TPaymentTransaction PrintTransaction(DBTransaction);
	std::auto_ptr<TList>NewOrdersList(new TList);
	std::auto_ptr<TList>OldOrdersList(new TList);
    PrintTransaction.ChitNumber = ChitNumber;
	TDateTime OrderedTimeStamp = Now();
    if(TDeviceRealTerminal::Instance().BasePMS->Enabled ||
       !TRooms::Instance().Enabled && !TDeviceRealTerminal::Instance().BasePMS->Enabled)
        PrintTransaction.Customer = TCustomer(0,0,"");
    PrintTransaction.Membership.Assign(Membership);
	for (UINT iSeat = 0; iSeat < SeatOrders.size(); iSeat++)
	{
		for (int i = 0; i < SeatOrders[iSeat]->Orders->Count; i++)
		{
			TItemComplete *Order = SeatOrders[iSeat]->Orders->Items[i];
			if (TabContainerName == "" && TabType == TabTableSeat)
			{
				Order->TabContainerName = "Table # " + IntToStr(TableNo); // + " Seat # "  + IntToStr((iSeat == 0) ? SeatNo : SeatOrders[iSeat]->SeatNumber);
			}
			else
			{
				Order->TabContainerName = TabContainerName;
			}
			Order->TabName = TabName;
            //MM2038
            Order->IdName = TGlobalSettings::Instance().TabPrintName;
            Order->IdNumber = TGlobalSettings::Instance().TabPrintPhone;
			Order->TabType = TabType;
			Order->ContainerTabType = TabType;
			if(!TDeviceRealTerminal::Instance().BasePMS->Enabled)
				Order->RoomNo = RoomNo;
			Order->TableNo = TableNo;
			Order->SeatNo = (iSeat == 0) ? SeatNo : SeatOrders[iSeat]->SeatNumber;
			Order->OrderType = NormalOrder;
			if (!TGlobalSettings::Instance().UnRavelKitchenPrinting)
			{
				Order->TimeStamp = OrderedTimeStamp;
			}
			Order->Terminal = TDeviceRealTerminal::Instance().ID.Name;
			Order->OrderedLocation = TDeviceRealTerminal::Instance().ID.Location;
			Order->Loyalty_Key = SeatOrders[iSeat]->Orders->AppliedMembership.ContactKey;
            if(Order->GetQty() != 0)
    			NewOrdersList->Add(Order);
		}
	}

    if((TGlobalSettings::Instance().PromptForPatronCount || TGlobalSettings::Instance().PromptPatronCountOnTableSales)  && TableNo > 0)
    {
        std::vector<TPatronType> selectedTablePatrons = TDBTables::GetPatronCount(DBTransaction, SelectedTable);
        int patronCount = GetCount(selectedTablePatrons);
        if(patronCount > 0)
        {
            PrintTransaction.Patrons = selectedTablePatrons;
        }
    }
    patronsStore = PrintTransaction.Patrons;
	if(TGlobalSettings::Instance().CaptureCustomerName)
	{
		PrintTransaction.CustomerOrder = TCustNameAndOrderType::Instance()->GetStringPair();
	}

	TMMContactInfo InvoiceOwnerInfo;
	std::auto_ptr<TReqPrintJob>TempReceipt(new TReqPrintJob(&TDeviceRealTerminal::Instance()));

	if (TGlobalSettings::Instance().SaveAndPrintPrintsPartialOrder && TabType != TabInvoice || TabType == TabCashAccount)
	{
		if (NewOrdersList->Count != 0)
		{
			PrintTransaction.Orders->Assign(NewOrdersList.get());

			std::set<__int64>SelectedTabs;
			TDBOrder::GetTabKeysFromOrders(PrintTransaction.Orders, SelectedTabs);
			PrintTransaction.Money.CreditAvailable = TDBTab::GetTabsCredit(DBTransaction, SelectedTabs);
			PrintTransaction.Money.Recalc(PrintTransaction);
		}
	}
	else
	{

		// enum eTabType { TabNormal, TabStaff, TabMember, TabTableSeat, TabHandheldCashAccount, TabRoom, TabInvoice, TabWeb, TabParkedSale, TabHeld, TabNone, TabCashAccount };
		std::set<__int64>InvoiceTabs;
		if (TabType == TabTableSeat)
		{ // Retrive the Tab Key for this Table/Seat.
			TDBTables::GetTabKeys(DBTransaction, TableNo, InvoiceTabs);
		}
		else if (TabType == TabRoom)
		{ // Retrive the Tab Key for this Table/Seat.
			SelectedTab = TDBRooms::GetRoomTab(DBTransaction, RoomNo);
			if (SelectedTab)
			{
				InvoiceTabs.insert(SelectedTab);
			}
		}
		else
		{
			InvoiceTabs.insert(SelectedTab);
		}

		TDBOrder::GetOrdersFromTabKeys(DBTransaction, OldOrdersList.get(), InvoiceTabs);
		PrintTransaction.Orders->Assign(NewOrdersList.get(), laOr);
		PrintTransaction.Orders->Assign(OldOrdersList.get(), laOr);
		std::set<__int64>SelectedTabs;
		TDBOrder::GetTabKeysFromOrders(PrintTransaction.Orders, SelectedTabs);
		PrintTransaction.Money.CreditAvailable = TDBTab::GetTabsCredit(DBTransaction, SelectedTabs);
		PrintTransaction.Money.Recalc(PrintTransaction);
	}

	if(TGlobalSettings::Instance().EnableMenuPatronCount)
	{
        PrintTransaction.CalculatePatronCountFromMenu();
    }

	if (PrintTransaction.Money.TotalAdjustment != 0)
	{
		PrintTransaction.TotalAdjustment = PrintTransaction.Money.TotalAdjustment;
		PrintTransaction.DiscountReason = PrintTransaction.TotalAdjustment < 0 ? "Discount " : "Surcharge";
	}

	TempReceipt->JobType = pjReceiptReceipt;
	TempReceipt->PaymentType = ptPreliminary;

	if (TabType == TabInvoice)
	{
		TempReceipt->JobType = pjReceiptInvoice;
		TempReceipt->PaymentType = ptFinal;

		int InvoiceKey = TDBTab::GetTabInvoice(DBTransaction, SelectedTab);
		PrintTransaction.InvoiceNumber = Invoice->GetInvoiceNumber(DBTransaction, InvoiceKey);
		int ContactKey = Invoice->GetInvoiceOwner(DBTransaction, InvoiceKey);
		TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetContactDetails(DBTransaction, ContactKey, InvoiceOwnerInfo);
		TempReceipt->ExtraInfo->Add("Name: " + InvoiceOwnerInfo.Name);
		TempReceipt->ExtraInfo->Add("Member No. " + InvoiceOwnerInfo.MembershipNumber);
	}
	else if (TabType == TabRoom)
	{
		TempReceipt->ExtraInfo->Add("Room Number # " + IntToStr(RoomNo));
		TempReceipt->ExtraInfo->Add("Guest " + TDBRooms::GetPartyName(DBTransaction, RoomNo));
	}

	// Print Invoice.

	TempReceipt->Transaction = &PrintTransaction;
	TempReceipt->SenderType = devPC;
	TempReceipt->Waiter = TDeviceRealTerminal::Instance().User.Name;
	TempReceipt->MiscData["PartyName"] = PartyName;

	// PrintTransaction.Orders->Assign(OrdersList.get());
	PrintTransaction.Money.Recalc(PrintTransaction);

	TPrinterPhysical DefaultScreenPrinter;
	DefaultScreenPrinter.BoldCharPerLine = 35;
	DefaultScreenPrinter.NormalCharPerLine = 35;
	DefaultScreenPrinter.PhysicalPrinterKey = TComms::Instance().ReceiptPrinter.PhysicalPrinterKey;
	Receipt->GetPrintouts(DBTransaction, TempReceipt.get(), DefaultScreenPrinter, eDispBCOff);
	TempReceipt->Printouts->PrintToStrings(ReceiptDisplay);

	if (OldOrdersList->Count > 0)
	{
		while (OldOrdersList->Count != 0)
		{
			delete(TItemComplete*)OldOrdersList->Items[0];
			OldOrdersList->Delete(0);
		}
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::btngridModifyMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
    int c = 0;
    if (GridButton)
	{
		tbtnMemberDisplayPageUp->Visible = false;
		tbtnMemberDisplayPageDown->Visible = false;
		eBtnToDisplay ButtonID = (eBtnToDisplay)GridButton->Tag;
		switch(ButtonID)
		{
		case eBTDOverView:
			pcItemModifyDisplayOverview();
			pcItemModify->ActivePage = tsOverview;
			break;
		case eBTDQty:
            pcItemModifyDisplayQuantity();
            pcItemModify->ActivePage = tsQuantity;
			break;
		case eBTDOptionsPlus:
			pcItemModifyDisplayOptions(true);
			pcItemModify->ActivePage = tsOptions;
			break;
		case eBTDOptionsMinus:
			pcItemModifyDisplayOptions(false);
			pcItemModify->ActivePage = tsOptions;
			break;
		case eBTDSetMenu:
			if (pcItemModifyDisplaySetMenu())
			{
				pcItemModify->ActivePage = tsSetMenu;
			}
			break;
		case eBTDSides:
			RedrawItemSideCourses();
			pcItemModify->ActivePage = tsSides;
			break;
		case eBTDPrice:
			GridButton->Latched = false;
			pcItemModifyDisplayPrice();
			break;
		case eBTDDiscountDetails:
			pcItemModifyDisplayDiscountDetails();
			pcItemModify->ActivePage = tsWebPage;
			tbtnMemberDisplayPageUp->Visible = true;
			tbtnMemberDisplayPageDown->Visible = true;
			break;
		case eBTDMembership:
		case eBTDMemberPurchases:
		case eBTDMemberFavourites:
		case eBTDMemberPoints:
        case eBTDThorVouchers:
			pcItemModifyDisplayMember(ButtonID);
			pcItemModify->ActivePage = tsWebPage;
			tbtnMemberDisplayPageUp->Visible = true;
			tbtnMemberDisplayPageDown->Visible = true;
			break;
        case eBTDChangeBarcode:
            AssignBarcodeToMember();
            GridButton->Latched = false;
            break;
		default:
			pcItemModify->ActivePage = tsOverview;
		}
	}
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::RedrawModifyOptionsBtnGrid(bool Reset)
{
	if (lbDisplay->ItemIndex > -1)
	{
		const int MaxButtonCount = 7;

		TItemRedirector *ListItem = (TItemRedirector*)lbDisplay->Items->Objects[lbDisplay->ItemIndex];

		TBtnToDisplay ButtonsSet;

		if (ListItem->ItemType.Contains(itSetMenuMasterItem))
		{
			ButtonsSet << eBTDSetMenu;
			if (!ListItem->ItemType.Contains(itPrevItem))
			{
				ButtonsSet << eBTDPrice;
				ButtonsSet << eBTDRemove;
			}
		}
		else if (ListItem->ItemType.Contains(itPrevItem))
		{
			ButtonsSet << eBTDOverView;
			ButtonsSet << eBTDPrice;
			ButtonsSet << eBTDDiscountDetails;
		}
		else if (ListItem->ItemType.Contains(itSubItem))
		{
			ButtonsSet << eBTDSides;
			ButtonsSet << eBTDPrice;
			ButtonsSet << eBTDRemove;
		}
		else if (ListItem->ItemType.Contains(itDiscountDisplay))
		{
			ButtonsSet << eBTDDiscountDetails;
			ButtonsSet << eBTDRemove;
		}
		else if (ListItem->ItemType.Contains(itNormalItem))
		{
			ButtonsSet << eBTDOverView;
			ButtonsSet << eBTDOptionsPlus;
			ButtonsSet << eBTDOptionsMinus;
			ButtonsSet << eBTDSides;
			if (!ListItem->ItemType.Contains(itPromoItem))
			{
				ButtonsSet << eBTDQty;
				ButtonsSet << eBTDPrice;
			}
			ButtonsSet << eBTDDiscountDetails;
			ButtonsSet << eBTDRemove;
		}
		else if (ListItem->ItemType.Contains(itOption))
		{
			ButtonsSet << eBTDOptionsPlus;
			ButtonsSet << eBTDOptionsMinus;
			ButtonsSet << eBTDRemove;
		}
		else if (ListItem->ItemType.Contains(itMembershipDisplay) || ListItem->ItemType.Contains(itMembershipDisplayNote) ||
					ListItem->ItemType.Contains(itEarntPts) || ListItem->ItemType.Contains(itLoadedPts))
		{
            ButtonsSet << eBTDMembership;
            ButtonsSet << eBTDMemberPurchases;
            ButtonsSet << eBTDMemberFavourites;
            ButtonsSet << eBTDMemberPoints;
            ButtonsSet << eBTDThorVouchers;
            ButtonsSet << eBTDChangeBarcode;
            ButtonsSet << eBTDRemove;


		}
		else if (ListItem->ItemType.Contains(itServingCourseDisplay))
		{
			ButtonsSet << eBTDOverView;
		}
		int OldSelectedButtonType = -1;
		for (int i = 0; i < btngridModify->RowCount; i++)
		{
			if (btngridModify->Buttons[i][0]->Latched)
			{
				OldSelectedButtonType = btngridModify->Buttons[i][0]->Tag;
			}
		}

		btngridModify->RowCount = 0;

		tbtnMemberDisplayPageUp->Visible = false;
		tbtnMemberDisplayPageDown->Visible = false;

		if (ButtonsSet.Contains(eBTDOverView))
		{
			btngridModify->RowCount++;
			AnsiString Caption = "Overview";
			btngridModify->Buttons[btngridModify->RowCount - 1][0]->Caption = Caption;
			btngridModify->Buttons[btngridModify->RowCount - 1][0]->Tag = int(eBTDOverView);
		}

		if (ButtonsSet.Contains(eBTDMembership))
		{
			btngridModify->RowCount++;
			AnsiString Caption = "Membership";
			btngridModify->Buttons[btngridModify->RowCount - 1][0]->Caption = Caption;
			btngridModify->Buttons[btngridModify->RowCount - 1][0]->Tag = int(eBTDMembership);
		}

		if (ButtonsSet.Contains(eBTDMemberFavourites))
		{
			btngridModify->RowCount++;
			AnsiString Caption = "Favourites";
			btngridModify->Buttons[btngridModify->RowCount - 1][0]->Caption = Caption;
			btngridModify->Buttons[btngridModify->RowCount - 1][0]->Tag = int(eBTDMemberFavourites);
		}

		if (ButtonsSet.Contains(eBTDMemberPurchases))
		{
			btngridModify->RowCount++;
			AnsiString Caption = "Purchases";
			btngridModify->Buttons[btngridModify->RowCount - 1][0]->Caption = Caption;
			btngridModify->Buttons[btngridModify->RowCount - 1][0]->Tag = int(eBTDMemberPurchases);
		}

		if (ButtonsSet.Contains(eBTDMemberPoints))
		{
			btngridModify->RowCount++;
			AnsiString Caption = "Points History";
			btngridModify->Buttons[btngridModify->RowCount - 1][0]->Caption = Caption;
			btngridModify->Buttons[btngridModify->RowCount - 1][0]->Tag = int(eBTDMemberPoints);
		}

        if ((ButtonsSet.Contains(eBTDThorVouchers))
          && (TGlobalSettings::Instance().IsThorlinkSelected || TGlobalSettings::Instance().LoyaltyMateEnabled))
		{
			btngridModify->RowCount++;
			AnsiString Caption = "Vouchers";
			btngridModify->Buttons[btngridModify->RowCount - 1][0]->Caption = Caption;
			btngridModify->Buttons[btngridModify->RowCount - 1][0]->Tag = int(eBTDThorVouchers);
		}


        if (ButtonsSet.Contains(eBTDChangeBarcode) &&
            TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Registered"]  &&
            TGlobalSettings::Instance().MembershipType == MembershipTypeMenuMate  &&
            TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->CardInserted)
		{
			btngridModify->RowCount++;
			AnsiString Caption = "Assign Barcode";
			btngridModify->Buttons[btngridModify->RowCount - 1][0]->Caption = Caption;
			btngridModify->Buttons[btngridModify->RowCount - 1][0]->Tag = int(eBTDChangeBarcode);
		}

		if (ButtonsSet.Contains(eBTDQty))
		{
			btngridModify->RowCount++;
			AnsiString Caption = "Quantity";
			btngridModify->Buttons[btngridModify->RowCount - 1][0]->Caption = Caption;
			btngridModify->Buttons[btngridModify->RowCount - 1][0]->Tag = int(eBTDQty);
		}

		if (ButtonsSet.Contains(eBTDOptionsPlus))
		{
			btngridModify->RowCount++;
			AnsiString Caption = "Options +";
			btngridModify->Buttons[btngridModify->RowCount - 1][0]->Caption = Caption;
			btngridModify->Buttons[btngridModify->RowCount - 1][0]->Tag = int(eBTDOptionsPlus);
		}

		if (ButtonsSet.Contains(eBTDOptionsMinus))
		{
			btngridModify->RowCount++;
			AnsiString Caption = "Options -";
			btngridModify->Buttons[btngridModify->RowCount - 1][0]->Caption = Caption;
			btngridModify->Buttons[btngridModify->RowCount - 1][0]->Tag = int(eBTDOptionsMinus);
		}

		if (ButtonsSet.Contains(eBTDSides))
		{
			btngridModify->RowCount++;
			AnsiString Caption = "Sides";
			btngridModify->Buttons[btngridModify->RowCount - 1][0]->Caption = Caption;
			btngridModify->Buttons[btngridModify->RowCount - 1][0]->Tag = int(eBTDSides);
		}

		if (ButtonsSet.Contains(eBTDSetMenu))
		{
			btngridModify->RowCount++;
			AnsiString Caption = "Set Menu Items";
			btngridModify->Buttons[btngridModify->RowCount - 1][0]->Caption = Caption;
			btngridModify->Buttons[btngridModify->RowCount - 1][0]->Tag = int(eBTDSetMenu);
		}

		if (ButtonsSet.Contains(eBTDPrice))
		{
			btngridModify->RowCount++;
			AnsiString Caption = "Change Price";
			btngridModify->Buttons[btngridModify->RowCount - 1][0]->Caption = Caption;
			btngridModify->Buttons[btngridModify->RowCount - 1][0]->Tag = int(eBTDPrice);
		}

		if (ButtonsSet.Contains(eBTDDiscountDetails))
		{
			btngridModify->RowCount++;
			AnsiString Caption = "Discount Details";
			btngridModify->Buttons[btngridModify->RowCount - 1][0]->Caption = Caption;
			btngridModify->Buttons[btngridModify->RowCount - 1][0]->Tag = int(eBTDDiscountDetails);
		}




		if (ButtonsSet.Contains(eBTDRemove))
		{
			btnRemove->Enabled = true;
		}

		if (OldSelectedButtonType != -1)
		{
			bool ButtonSelected = false;
			for (int i = 0; i < btngridModify->RowCount; i++)
			{
				if (btngridModify->Buttons[i][0]->Tag == OldSelectedButtonType)
				{
					btngridModifyMouseClick(btngridModify->Buttons[i][0], mbLeft, TShiftState(), btngridModify->Buttons[i][0]);
					btngridModify->Buttons[i][0]->Latched = true;
					ButtonSelected = true;
				}
			}

			if (!ButtonSelected && btngridModify->RowCount > 0)
			{
				btngridModifyMouseClick(btngridModify->Buttons[0][0], mbLeft, TShiftState(), btngridModify->Buttons[0][0]);
				btngridModify->Buttons[0][0]->Latched = true;
			}
		}
		else if (btngridModify->RowCount > 0)
		{
			btngridModifyMouseClick(btngridModify->Buttons[0][0], mbLeft, TShiftState(), btngridModify->Buttons[0][0]);
			btngridModify->Buttons[0][0]->Latched = true;
		}

		if (btngridModify->RowCount < MaxButtonCount)
		{
			while (btngridModify->RowCount < MaxButtonCount)
			{
				btngridModify->RowCount++;
				AnsiString Caption = "";
				btngridModify->Buttons[btngridModify->RowCount - 1][0]->Visible = false;
			}
		}
	}
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::SetServingCourseBtnSizes()
{
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::pcItemModifyDisplayOptions(bool PlusOpt)
{
	static unsigned long color_sel[2] =
	{
		clNavy, clGreen
	};

	TGlobalSettings &ref_gs = TGlobalSettings::Instance();

	TListOptionContainer *item_options;
	TItemOption *option;
	TListOptionContainer *options;
	TItemComplete *order;
	int i = 0;
	int j = 0;
	int k = 0;

	tgridItemOptions->RowCount = 0;
	tgridItemOptions->ColCount = ref_gs.DefaultColumns;

	if (lbDisplay->ItemIndex == -1)
		return;

	DisplayingPlusOptions = PlusOpt;

	order = static_cast<TItemComplete*>(GetSelectedItem(TDisplayItemTypes() << itNormalItem << itOption));
	if (!order || !(options = order->ItemOrderedFrom->Options)->Count)
		return;

	tgridItemOptions->RowCount = options->Count / ref_gs.DefaultColumns + (options->Count % ref_gs.DefaultColumns != 0);

	for (; k < options->Count; k++)
	{
		option = options->OptionGet(k);
		if ((option->PlusOption && PlusOpt) || (option->MinusOption && !PlusOpt))
		{
			tgridItemOptions->Buttons[j][i]->Enabled = 1;
			tgridItemOptions->Buttons[j][i]->Visible = 1;
			tgridItemOptions->Buttons[j][i]->Caption = option->Name;

			tgridItemOptions->Buttons[j][i]->Color = color_sel[order->OptionsSelected->OptionIndex(option) != -1];

			tgridItemOptions->Buttons[j][i]->FontColor = GetItemButtonFontColour(tgridItemOptions->Buttons[j][i]->Color);
			tgridItemOptions->Buttons[j][i]->Tag = option->OptionKey;

			j += (i = ++i % ref_gs.DefaultColumns) == 0;
		}
	}

	while (j < tgridItemOptions->RowCount)
	{
		tgridItemOptions->Buttons[j][i]->Visible = false;
		j += (i = ++i % ref_gs.DefaultColumns) == 0;
	}
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::RedrawSetMenuItems()
{
        TDeviceRealTerminal &drt = TDeviceRealTerminal::Instance();
        TGlobalSettings &gs = TGlobalSettings::Instance();
        TTouchGrid *tgsmi = tgridItemSetMenuItems;

        std::auto_ptr<std::vector<TItem *> > set_menu_items;

        //set_menu_items = drt.Menus->FetchItemsBySetMenuMask(SetMenuMask);
		if (SelectedOrderMenuName.data() != NULL)
		{
			set_menu_items = drt.Menus->FetchItemsBySetMenuMaskByMenu(SetMenuMask,  SelectedOrderMenuName);
		}

        std::vector<TItem *>::size_type i = 0;
        std::vector<TItem *>::size_type j;
        const int n_columns = gs.DefaultColumns;

        tgridItemSetMenuItems->RowCount = 0;

        if (!set_menu_items.get()
            || (j = set_menu_items->size()) == 0)
                return;

        tgridItemSetMenuItems->ColCount = n_columns;
        tgridItemSetMenuItems->RowCount = j / n_columns + (j % n_columns != 0);

        for ( ; i < j; ++i) {
                TItem &item = *(*set_menu_items)[i];
                const int x = i % n_columns;
                const int y = i / n_columns;

                tgsmi->Buttons[y][x]->Caption = item.GetDecoratedName();
                tgsmi->Buttons[y][x]->Color = item.SetColour;
                tgsmi->Buttons[y][x]->DisabledColor = clBtnFace;
                tgsmi->Buttons[y][x]->Enabled = item.Enabled;
                tgsmi->Buttons[y][x]->FontColor =
                    GetItemButtonFontColour(item.SetColour);
                tgsmi->Buttons[y][x]->Tag = item.ItemKey;
                tgsmi->Buttons[y][x]->Visible = true;
        }

        for (int l = i % n_columns ; l; l = ++i % n_columns)
                tgsmi->Buttons[i / n_columns][l]->Visible = false;
}
// ---------------------------------------------------------------------------
bool TfrmSelectDish::pcItemModifyDisplaySetMenu()
{
	bool RetVal = false;
	if (lbDisplay->ItemIndex > -1)
	{
		TItemMinor *Order = GetSelectedSetMenuMaster();
		if (Order)
		{
			RetVal = true;
			// Selected Item
			SetMenuMask = Order->SetMenuMask;
			SetMenuGroupNo = Order->SetMenuGroup;
            SelectedOrderMenuName = Order->MenuName;

            RedrawSetMenuItems();
#if 0
			tgridItemSetMenuItems->ColCount = 0;
			tgridItemSetMenuItems->RowCount = 0;

			std::vector<TItem*>Dishes;

			TListMenu *SetMenuMenu = TDeviceRealTerminal::Instance().Menus->IsACurrentMenu(Order->MenuName);
			if (SetMenuMenu)
			{
				for (int j = 0; j < SetMenuMenu->Count; j++)
				{
					TListCourse *Course = SetMenuMenu->CourseGet(j);
					for (int i = 0; i < Course->Count; i++)
					{
						TItem *Item = Course->ItemGet(i);
						for (UINT x = 0; x < Item->Sizes->Count; x++)
						{
							if ((Order->SetMenuMask & Item->Sizes->SizeGet(x)->SetMenuMask) && !Item->Sizes->SizeGet(x)->SetMenuMaster)
							{
								// If any Items->Size has the Set Menu mask add that item.
								Dishes.push_back(Item);
								break;
							}
						}
					}
				}
			}
			else
			{
				MessageBox("Unable to Locate the Menu for the Set Menu Item. Is it Loaded into the POS?", "Error", MB_OK + MB_ICONERROR);
			}

			if (Dishes.size() > 0)
			{
				tgridItemSetMenuItems->ColCount = TGlobalSettings::Instance().DefaultColumns;
				tgridItemSetMenuItems->RowCount = TGlobalSettings::Instance().DefaultRows;

				for (int i = 0; i < tgridItemSetMenuItems->RowCount; i++)
				{
					for (int j = 0; j < tgridItemSetMenuItems->ColCount; j++)
					{
						int Index = ((i * tgridItemSetMenuItems->ColCount) + j);
						if (Index < Dishes.size())
						{
							TItem *Item = Dishes[Index];
							if (Item != NULL)
							{
								tgridItemSetMenuItems->Buttons[i][j]->Visible = true;
								tgridItemSetMenuItems->Buttons[i][j]->Caption = Item->GetDecoratedName();
								tgridItemSetMenuItems->Buttons[i][j]->Color = Item->SetColour;
								tgridItemSetMenuItems->Buttons[i][j]->DisabledColor = clBtnFace;
								tgridItemSetMenuItems->Buttons[i][j]->FontColor = GetItemButtonFontColour(Item->SetColour);
								tgridItemSetMenuItems->Buttons[i][j]->Tag = Item->ItemKey;
								tgridItemSetMenuItems->Buttons[i][j]->Enabled = Item->Enabled;
							}
						}
					}
				}

				for (int i = 0; i < tgridItemSetMenuItems->RowCount; i++)
				{
					for (int j = 0; j < tgridItemSetMenuItems->ColCount; j++)
					{
						if (tgridItemSetMenuItems->Buttons[i][j]->Caption == "")
						{
							tgridItemSetMenuItems->Buttons[i][j]->Visible = false;
						}
					}
				}
			}
#endif
		}
		else
		{
			MessageBox("Item Selected is not part of a Set Menu.", "Error", MB_OK + MB_ICONERROR);
		}
	}
	return RetVal;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::btnRemoveMouseClick(TObject *Sender)
{

 Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
 DBTransaction.StartTransaction();

  if (lbDisplay->ItemIndex > -1)
   {
    RemoveItem(DBTransaction);
    if (!OrdersPending())
    {
        btnOkMouseClick(btnOk);
    }

    if (lbDisplay->Items->Count == 0)
    {
        TDBSaleTimes::VoidSaleTime(DBTransaction, CurrentTimeKey);
        CurrentTimeKey = 0;
        AutoLogOut();
        DisplayRoomNoUI();
        //MM-1647: Ask for chit if it is enabled for every order.
        NagUserToSelectChit();
        sec_ref = 0;
        patronsStore.clear();
    }
   DBTransaction.Commit();
   UpdateTableButton();
   TotalCosts();
   UpdateExternalDevices();
   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tgridItemServingCourseMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
	if (lbDisplay->ItemIndex > -1)
	{
		TItemRedirector *ListItem = (TItemRedirector*)lbDisplay->Items->Objects[lbDisplay->ItemIndex];
		if ((ListItem->ItemType.Contains(itNormalItem) || ListItem->ItemType.Contains(itSetMenuMasterItem)) && !ListItem->ItemType.Contains(itPrevItem))
		{
			TItemComplete * Item = (TItemComplete*)ListItem->ItemObject;
			int SelectedServingCourse = GridButton->Tag;
			if (SelectedServingCourse != 0 && TDeviceRealTerminal::Instance().Menus->VisibleMenu != NULL)
			{
				if (TDeviceRealTerminal::Instance().Menus->VisibleMenu->GetServingCourseExists(SelectedServingCourse))
				{
					TServingCourse NewServingCourse = TDeviceRealTerminal::Instance().Menus->VisibleMenu->GetServingCourse(SelectedServingCourse);
					Item->ServingCourse = NewServingCourse;
					RedrawSeatOrders();
					HighlightSelectedItem();

					for (int i = 0; i < tgridItemServingCourse->ColCount; i++)
					{
						if (tgridItemServingCourse->Buttons[0][i] == GridButton)
						{
							tgridItemServingCourse->Buttons[0][i]->Color = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_BUTTONCOLOR];
							tgridItemServingCourse->Buttons[0][i]->FontColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_FONTCOLOR];
						}
						else
						{
							tgridItemServingCourse->Buttons[0][i]->Color = ButtonColors[BUTTONTYPE_UNSELECTED][ATTRIB_BUTTONCOLOR];
							tgridItemServingCourse->Buttons[0][i]->FontColor = ButtonColors[BUTTONTYPE_UNSELECTED][ATTRIB_FONTCOLOR];
						}
					}
				}
			}
		}
	}
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::pcItemModifyDisplayMember(eBtnToDisplay ButtonID)
{
	try
	{
		std::auto_ptr<TStringList>Report(new TStringList);

		TMMContactInfo Member = SeatOrders[SelectedSeat]->Orders->AppliedMembership;
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
		DBTransaction.StartTransaction();
		// Tab->GetReportMemberInfo(DBTransaction,Report.get());
		// Invoice->GetReportMemberInfo(DBTransaction,Report.get());

		if (ButtonID == eBTDMemberPurchases)
		{
			TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetReportMemberStart(DBTransaction, Member, Report.get());
			std::auto_ptr<TContactStaff>Staff(new TContactStaff(DBTransaction));
			if (Staff->TestAccessLevel(TDeviceRealTerminal::Instance().User, CheckViewMembersDetials))
			{
				TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetReportMemberPurchaseInfo(DBTransaction, Member, Report.get());
			}
			else
			{
				TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetReportMemberInfoRestricted(DBTransaction, Member, Report.get());
			}
			TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetReportMemberStop(DBTransaction, Member, Report.get());
		}
		else if (ButtonID == eBTDMemberFavourites)
		{
			TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetReportMemberStart(DBTransaction, Member, Report.get());
			TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetReportMemberFavouritesInfo(DBTransaction, Member, Report.get());
			TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetReportMemberStop(DBTransaction, Member, Report.get());
		}
		else if (ButtonID == eBTDMemberPoints)
		{
			TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetReportMemberStart(DBTransaction, Member, Report.get());
			std::auto_ptr<TContactStaff>Staff(new TContactStaff(DBTransaction));
			if (Staff->TestAccessLevel(TDeviceRealTerminal::Instance().User, CheckViewMembersDetials))
			{
				TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetReportMemberPointsInfo(DBTransaction, Member, Report.get());
			}
			else
			{
				TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetReportMemberInfoRestricted(DBTransaction, Member, Report.get());
			}
			TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetReportMemberStop(DBTransaction, Member, Report.get());
		}


        else if (ButtonID == eBTDThorVouchers)
		{
            if(TGlobalSettings::Instance().MembershipType == MembershipTypeMenuMate
                && TGlobalSettings::Instance().LoyaltyMateEnabled && Membership.Applied())
            {
                TManagerLoyaltyVoucher ManagerLoyaltyVoucher;
                ManagerLoyaltyVoucher.DisplayMemberVouchers(DBTransaction,Member);
                SeatOrders[SelectedSeat]->Orders->AppliedMembership = Member;
                ApplyMemberDiscounts(DBTransaction);
                TotalCosts();
                RedrawSeatOrders();
                HighlightSelectedItem();
            }
           else
            {
                TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetReportMemberStart(DBTransaction, Member, Report.get());
                std::auto_ptr<TContactStaff>Staff(new TContactStaff(DBTransaction));
                if (Staff->TestAccessLevel(TDeviceRealTerminal::Instance().User, CheckViewMembersDetials))
                {
                    TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetReportMemberInfo(DBTransaction, Member, Report.get());
                    ManagerDiscount->GetReportDiscountInfo(DBTransaction, Member.AutoAppliedDiscounts, Report.get());
                    ManagerFreebie->GetReportMemberInfo(DBTransaction, Member.ContactKey, Report.get());
                }
                else
                {
                    TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetReportMemberInfoRestricted(DBTransaction, Member, Report.get());
                }
                TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetReportMemberStop(DBTransaction, Member, Report.get());
                GetThorVouchers();
            }
        }

		else
		{
			TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetReportMemberStart(DBTransaction, Member, Report.get());
			std::auto_ptr<TContactStaff>Staff(new TContactStaff(DBTransaction));
			if (Staff->TestAccessLevel(TDeviceRealTerminal::Instance().User, CheckViewMembersDetials))
			{
				TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetReportMemberInfo(DBTransaction, Member, Report.get());
				ManagerDiscount->GetReportDiscountInfo(DBTransaction, Member.AutoAppliedDiscounts, Report.get());
				ManagerFreebie->GetReportMemberInfo(DBTransaction, Member.ContactKey, Report.get());
			}
			else
			{
				TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetReportMemberInfoRestricted(DBTransaction, Member, Report.get());
			}
			TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetReportMemberStop(DBTransaction, Member, Report.get());

		}

		DBTransaction.Commit();
		Navigate(webDisplay, Report.get());
        //UnicodeString msg1 = Report->Strings[0]
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		MessageBox(E.Message, "Error", MB_OK + MB_ICONERROR);
	}
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::pcItemModifyDisplayQuantity()
{
	if (TDeviceRealTerminal::Instance().Menus->VisibleMenu != NULL)
	{
		// Display the Item Details If any
		if (lbDisplay->ItemIndex > -1)
		{
			TItemRedirector *ListItem = (TItemRedirector*)lbDisplay->Items->Objects[lbDisplay->ItemIndex];
			if (ListItem->ItemType.Contains(itNormalItem) && !ListItem->ItemType.Contains(itPrevItem))
			{
				TItemComplete * Item = (TItemComplete*)ListItem->ItemObject;
				if (Item)
				{
					QtyDisplay->SetNumeric(ListItem->CompressedContainer->Count);
				}
			}
		}
	}
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::pcItemModifyDisplayOverview()
{
	if (TDeviceRealTerminal::Instance().Menus->VisibleMenu != NULL)
	{
		RedrawServingCoursesOverview();
		memNote->Lines->Clear();

		// Display the Item Details If any
		if (lbDisplay->ItemIndex > -1)
		{
			TItemRedirector *ListItem = (TItemRedirector*)lbDisplay->Items->Objects[lbDisplay->ItemIndex];
			if (ListItem->ItemType.Contains(itNormalItem) && !ListItem->ItemType.Contains(itPrevItem))
			{
				TItemComplete * Item = (TItemComplete*)ListItem->ItemObject;
				if (Item)
				{
					memNote->Lines->Add(Item->Note);
				}
			}
		}
		// Display the Item Details If any
		memOverview->Lines->Clear();
		if (lbDisplay->ItemIndex > -1)
		{
			TItemRedirector *ListItem = (TItemRedirector*)lbDisplay->Items->Objects[lbDisplay->ItemIndex];
			if (ListItem->ItemType.Contains(itNormalItem) || ListItem->ItemType.Contains(itPrevItem))
			{
				TItemComplete * Item = (TItemComplete*)ListItem->ItemObject;
				if (Item)
				{
					Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
					DBTransaction.StartTransaction();
					std::auto_ptr<TContactStaff>Staff(new TContactStaff(DBTransaction));
					for (int i = 0; i < Item->Security->Count; i++)
					{
						TSecurityReference *Security = Item->Security->SecurityGet(i);
						memOverview->Lines->Add(Security->Event + " " + Staff->GetContactName(DBTransaction, Security->UserKey) + " " + Security->TimeStamp.FormatString("dd-mm-yy hh:nn:ss")
							+ " " + Security->TerminalName + " " + Security->From + " " + Security->To + " " + Security->Note);
					}
					DBTransaction.Commit();
					std::auto_ptr<TStringList>Report(new TStringList);
					if (ListItem->ItemType.Contains(itNormalItem))
					{
						DBTransaction.StartTransaction();
						TDeviceRealTerminal::Instance().ManagerStock->GetStockOnHand(DBTransaction, Item->SalesRecipesToApply, Report.get());
						DBTransaction.Commit();
						Navigate(webStockLevels, Report.get());
					}
					else
					{
						std::auto_ptr<TListRecipeContainer>RecipeContainer(new TListRecipeContainer);
						DBTransaction.StartTransaction();
						TDeviceRealTerminal::Instance().ManagerStock->GetStockOnHand(DBTransaction, RecipeContainer.get(), Report.get());
						DBTransaction.Commit();
						Navigate(webStockLevels, Report.get());
					}
				}
			}
		}
	}
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::pcItemModifyDisplayDiscountDetails()
{
	if (TDeviceRealTerminal::Instance().Menus->VisibleMenu != NULL)
	{
		// Display the Item Details If any
		if (lbDisplay->ItemIndex > -1)
		{
			std::auto_ptr<TStringList>Report(new TStringList);

			TItemRedirector *ListItem = (TItemRedirector*)lbDisplay->Items->Objects[lbDisplay->ItemIndex];
			if (ListItem->ItemType.Contains(itNormalItem) || ListItem->ItemType.Contains(itPrevItem))
			{
				TItemMinorComplete * Item = (TItemComplete*)ListItem->ItemObject;
				if (Item)
				{
					// Show discount info for single item.
					ManagerDiscount->GetReportItemDiscountsHeader(Report.get());
					ManagerDiscount->GetReportItemDiscounts(Item, Report.get());
					ManagerDiscount->GetReportItemDiscountsFooter(Report.get());
				}
			}
			else
			{
				ManagerDiscount->GetReportItemDiscountsHeader(Report.get());
				for (int i = 0; i < SeatOrders[SelectedSeat]->Orders->Count; i++)
				{
					TItemComplete *Item = SeatOrders[SelectedSeat]->Orders->Items[i];
					ManagerDiscount->GetReportItemDiscounts(Item, Report.get());
				}
				ManagerDiscount->GetReportItemDiscountsFooter(Report.get());
			}

			Navigate(webDisplay, Report.get());
		}
	}
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::Navigate(TWebBrowser *WebBrowser, TStringList *Html)
{
	std::auto_ptr<TMemoryStream>StreamRpt(new TMemoryStream);
	Html->SaveToStream(StreamRpt.get());
	IPersistStreamInit *psi = NULL;
	StreamRpt->Seek(0, 0);
	TStreamAdapter *sa = new TStreamAdapter(StreamRpt.get(), soReference);
	_di_IDispatch doc = WebBrowser->Document;
	if (doc == NULL)
	{
		OleVariant url = "about:blank";
		WebBrowser->Navigate2(url);
		while (doc == NULL)
		{
			Application->ProcessMessages();
			doc = WebBrowser->Document;
		}
	}

	TComInterface<IHTMLDocument2>HTMLDocument;
	TComInterface<IHTMLWindow2>parentWindow;

	if (SUCCEEDED(WebBrowser->Document->QueryInterface(IID_IHTMLDocument2, (LPVOID*) & HTMLDocument)))
	{
        IHTMLElement* pBodyElem = 0;
		HRESULT hr = HTMLDocument->get_body(&pBodyElem);
		if (SUCCEEDED(hr) && pBodyElem != NULL)
		{
			IHTMLBodyElement* pBody = 0;
			hr = pBodyElem->QueryInterface(IID_IHTMLBodyElement, (void**) & pBody);
			if (SUCCEEDED(hr) && pBody != NULL)
			{
				// hide 3D border
				IHTMLStyle* pStyle = 0;
				hr = pBodyElem->get_style(&pStyle);
				if (SUCCEEDED(hr) && pStyle != NULL)
				{
					pStyle->put_borderStyle(BSTR("none"));
					pStyle->Release();
				}
				// hide scrollbars
				pBody->put_scroll(BSTR("no"));
			}
			pBodyElem->Release();
		}
	}

	if (doc->QueryInterface(IID_IPersistStreamInit, (void**) & psi) == S_OK)
	{
		if (psi)
			psi->Load(*sa);
	}
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::RedrawItemSideCourses()
{
	TDeviceRealTerminal &ref_drt = TDeviceRealTerminal::Instance();

	TListCourse *sel_course = NULL;
	TListMenu *vmenu = ref_drt.Menus->VisibleMenu;
	int i = 0, j = 0, k = 0;
	int n_cols = TGlobalSettings::Instance().DefaultColumns;
	int n_side_courses;

	if (!vmenu || !(n_side_courses = vmenu->Count))
		return;

	tgridItemSideCourses->ColCount = 1;
	tgridItemSideCourses->RowCount = n_side_courses;

	for (TListCourse * curr_course; k < n_side_courses; k++)
	{
		if (!(curr_course = vmenu->CourseGet(k)))
			continue;

		tgridItemSideCourses->Buttons[k][0]->Caption = curr_course->Course_Name;
		tgridItemSideCourses->Buttons[k][0]->Color = ButtonColors[BUTTONTYPE_UNSELECTED][ATTRIB_BUTTONCOLOR];
		tgridItemSideCourses->Buttons[k][0]->FontColor = ButtonColors[BUTTONTYPE_UNSELECTED][ATTRIB_FONTCOLOR];
		tgridItemSideCourses->Buttons[k][0]->LatchedColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_BUTTONCOLOR];
		tgridItemSideCourses->Buttons[k][0]->LatchedFontColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_FONTCOLOR];
		tgridItemSideCourses->Buttons[k][0]->Tag = curr_course->Course_Key;

		if (!tgridItemSideCourses->Buttons[k][0]->Latched)
			tgridItemSideCourses->Buttons[k][0]->Latched = false;
		else
			sel_course = curr_course;
	}

	tgridItemSideCourses->Buttons[0][0]->Latched = sel_course == NULL;
	if (!sel_course)
		RedrawItemSideItems(vmenu->CourseGet(0));
	else
		RedrawItemSideItems(sel_course);
}
// ---------------------------------------------------------------------------
TColor TfrmSelectDish::GetItemButtonFontColour(TColor inColor)
{
	TColor RetVal = clWhite;
	double R, G, B; // input RGB values
	R = GetRValue(inColor);
	G = GetGValue(inColor);
	B = GetBValue(inColor);
	double L = (0.2126 * R) + (0.7152 * G) + (0.0722 * B);
	if (L > 128)
	{
		RetVal = clBlack;
	}
	else
	{
		RetVal = clWhite;
	}
	return RetVal;
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::RedrawItemSideItems(TListCourse *sel_course)
{
	TItem *curr_item;
	TItemSize *curr_size;

	int n_cols = TGlobalSettings::Instance().DefaultColumns;
	int n_items = sel_course->Count;
	int n_rows = sel_course->Count;
	int i = 0, j = 0, k = 0, l, m;

	if (!n_items)
		return;

	n_rows = n_items / n_cols + (n_items % n_cols != 0);

	tgridItemSideItems->ColCount = n_cols;
	tgridItemSideItems->RowCount = n_rows;

	for (; k < n_items; k = i + j * n_cols)
	{
		curr_item = sel_course->ItemGet(k);
                bool item_is_enabled = curr_item->Enabled;

		tgridItemSideItems->Buttons[j][i]->Caption =
                  curr_item->GetDecoratedName();
		tgridItemSideItems->Buttons[j][i]->Color = curr_item->SetColour;
		tgridItemSideItems->Buttons[j][i]->DisabledColor = clBtnFace;
                tgridItemSideItems->Buttons[j][i]->Enabled = item_is_enabled;
		tgridItemSideItems->Buttons[j][i]->FontColor = GetItemButtonFontColour(curr_item->SetColour);
		tgridItemSideItems->Buttons[j][i]->Tag = curr_item->ItemKey;
		tgridItemSideItems->Buttons[j][i]->Visible = 1;

		for (int n = curr_item->Sizes->Count, l = m = 0; l < n && !m; l++)
			m = (curr_size = curr_item->Sizes->SizeGet(l))->Available_As_Standard;

		tgridItemSideItems->Buttons[j][i]->Enabled =
                  m & item_is_enabled;
		j += (i = ++i % n_cols) == 0;
	}

	for (; j < tgridItemSideItems->RowCount; )
	{
		tgridItemSideItems->Buttons[j][i]->Visible = 0;
		j += (i = ++i % n_cols) == 0;
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tgridItemSideCoursesMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
	int i = 0;

	if (!tgridItemSideCourses->RowCount)
		return;

	for (; i < tgridItemSideCourses->RowCount; i++)
		if (tgridItemSideCourses->Buttons[i][0]->Latched)
		{
			RedrawItemSideItems(TDeviceRealTerminal::Instance().Menus->VisibleMenu->CourseGet(i));
			return;
		}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tgridItemSideItemsMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
	if (lbDisplay->ItemIndex == -1)
	{
		MessageBox("Please select a parent item to add this side to.", "Select Item", MB_OK + MB_ICONERROR);
	}
	else
	{
		TItemRedirector *ItemRedirector = (TItemRedirector*)lbDisplay->Items->Objects[lbDisplay->ItemIndex];
		TItemComplete *MasterOrder = NULL;
		if (ItemRedirector->ItemType.Contains(itSubItem))
		{
			ItemRedirector = (TItemRedirector*)ItemRedirector->ParentRedirector;
			MasterOrder = (TItemComplete*)ItemRedirector->ItemObject;
		}
		else if (ItemRedirector->ItemType.Contains(itOption))
		{
			ItemRedirector = (TItemRedirector*)ItemRedirector->ParentRedirector;
			MasterOrder = (TItemComplete*)ItemRedirector->ItemObject;
		}
		else
		{
			MasterOrder = (TItemComplete*)ItemRedirector->ItemObject;
		}

		// Find the Item.

		TItem *Item = TDeviceRealTerminal::Instance().Menus->VisibleMenu->FetchItemByKey(GridButton->Tag);

         bool isSameMenuTypeItemExist = true;

        if(TGlobalSettings::Instance().IsBillSplittedByMenuType && Item)
            isSameMenuTypeItemExist = CheckItemCanBeAddedToSeat(Item);

		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();

        if(isSameMenuTypeItemExist)
        {
            TItemCompleteSub *SubItem =
            AddSubItemToItem(DBTransaction, Item, MasterOrder);
            if (SubItem == NULL)
               return;

            if (SubItem->PriceEach() == -999.99)
            {
                std::auto_ptr<TfrmTouchNumpad>frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
                frmTouchNumpad->Caption = "Enter Amount";
                frmTouchNumpad->btnSurcharge->Caption = "Ok";
                frmTouchNumpad->btnDiscount->Visible = false;
                frmTouchNumpad->btnSurcharge->Visible = true;
                frmTouchNumpad->Mode = pmCurrency;
                frmTouchNumpad->CURInitial = 0;
                if (frmTouchNumpad->ShowModal() == mrOk)
                {
                    SubItem->SetPriceLevelCustom(frmTouchNumpad->CURResult);
                    SubItem->PriceLevel0 = frmTouchNumpad->CURResult;
                    SubItem->PriceLevel1 = frmTouchNumpad->CURResult;
                }
                else
                {
                    SubItem->SetPriceLevelCustom(0);
                    SubItem->PriceLevel0 = 0;
                    SubItem->PriceLevel1 = 0;
                }
                if(TGlobalSettings::Instance().UseMemberSubs)
                   SubItem->wasOpenItem = true;
            }
		// Apply Member Specific Discounts.
            std::auto_ptr<TList>OrdersList(new TList);
            for(int totalItems = 0 ; totalItems < SeatOrders[SelectedSeat]->Orders->Count ; totalItems++)
            {
                 OrdersList->Add((TItemComplete*)SeatOrders[SelectedSeat]->Orders->Items[totalItems]);
            }

            DBTransaction.Commit();
            ItemRedirector->CompressedContainer->Container->LastItemSelected = SubItem;
            ManageDiscounts();
            CheckDeals(DBTransaction); // a sub item is added to the orders list. check for possible deals here
            RedrawSeatOrders();
            HighlightSelectedItem();
            RefreshModifyGui(NULL);
            TotalCosts();
            UpdateExternalDevices();
        }
	}
	UpdateTableButton();
}
// ---------------------------------------------------------------------------
bool TfrmSelectDish::pcItemModifyDisplayPrice()
{
   int index = lbDisplay->ItemIndex;

   if (index > -1)
   {
      TLoginSuccess can_alter_discount;
      TLoginSuccess can_alter_price;
      std::auto_ptr<TfrmSelectDiscounts> item_modification_dlg(TfrmSelectDiscounts::Create<TfrmSelectDiscounts>(this));
      bool modifying_a_stack = false;
      std::auto_ptr<TContactStaff> staff_member;
      TMMContactInfo temp_user_info(TDeviceRealTerminal::Instance().User);
      Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);
      TItemRedirector *item_redirector;
      TItemMinorComplete *item_to_edit;

      union
      {
         TItemMinorComplete *as_minor_complete;
         TItemComplete *as_complete;
      }  original_item;

      /* It's okay to edit any kind of item excepting options. */
		item_to_edit = original_item.as_minor_complete =
		  GetSelectedItem(TDisplayItemTypes()
                        << itNormalItem
                        << itSubItem
                        << itSetMenuMasterItem
								<< itPrevItem);

      std::auto_ptr<TList> allOrders(new TList());
      GetAllOrders(allOrders.get());

      /*
       * Needed so we can navigate the item tree. This is only really needed
       * when we're altering a side item so we can access the item that
       * contains the side.
       */
      item_redirector = reinterpret_cast<TItemRedirector *>(lbDisplay->Items->Objects[index]);

      tr.StartTransaction();

      /*
       * Determine whether the currently logged in user has the privileges
       * to alter an item's discount, surcharge or price options.
		 */
      staff_member.reset(new TContactStaff(tr));

      can_alter_discount = staff_member->Login(this, tr, temp_user_info, CheckDiscountBill);
      can_alter_price = staff_member->Login(this, tr, temp_user_info, CheckPriceAj);

      tr.Commit();

      /*
       * There's nothing to do if no suitable item is selected.
       * Since nothing has changed, there's really no point to update
       * the database, redraw prices or update external devices.
       */
      if (!item_to_edit)
      {
         pcItemModifyDisplayOverview();
         pcItemModify->ActivePage = tsOverview;
         return false;
      }
      TMMContactInfo &Member = SeatOrders[SelectedSeat]->Orders->AppliedMembership;
      item_modification_dlg->SetMember(Member);
      /* Perform the modifications. */
		item_modification_dlg->Execute(item_to_edit,
												 item_redirector,
                                     can_alter_discount == lsAccepted,
                                     can_alter_price == lsAccepted, allOrders.get());
  /* Was the order previously parked or saved? */
      if (item_redirector->ItemType.Contains(itPrevItem))
      {
         /* If so, we've got to update it's price and quantity information. */
         tr.StartTransaction();

         TDBOrder::UpdateOrder(tr, original_item.as_minor_complete);
         TDBSecurity::ClearSecurity(tr, original_item.as_minor_complete->Security, secPriceAdjust);
         TDBSecurity::ProcessSecurity(tr, original_item.as_minor_complete->Security);
         tr.Commit();
      }

      /* And it's all done. */
      pcItemModifyDisplayOverview();
      pcItemModify->ActivePage = tsOverview;

      ManageDiscounts();

      HighlightSelectedItem();
      TotalCosts();
      UpdateExternalDevices();
      SetSelectedItem(item_to_edit);

      return true;
   }
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::SetSelectedItem(TItemMinorComplete *Order)
{
	SeatOrders[SelectedSeat]->Orders->Select(Order);
	RedrawSeatOrders();
	HighlightSelectedItem();
}
// ---------------------------------------------------------------------------
TItemMinorComplete * TfrmSelectDish::GetSelectedItem(TDisplayItemTypes Filter)
{
	TItemMinorComplete * RetVal = NULL;
	if (lbDisplay->ItemIndex > -1)
	{
		TItemRedirector *ListItem = (TItemRedirector*)lbDisplay->Items->Objects[lbDisplay->ItemIndex];
		bool PassedFilter = true;
		if (ListItem->ItemType.Contains(itSubItem) && !Filter.Contains(itSubItem))
			PassedFilter = false;
		if (ListItem->ItemType.Contains(itPrevItem) && !Filter.Contains(itPrevItem))
			PassedFilter = false;
		if (ListItem->ItemType.Contains(itSetMenuMasterItem) && !Filter.Contains(itSetMenuMasterItem))
			PassedFilter = false;
		if (ListItem->ItemType.Contains(itOption) && !Filter.Contains(itOption))
			PassedFilter = false;
		if (ListItem->ItemType.Contains(itNormalItem) && !Filter.Contains(itNormalItem))
			PassedFilter = false;
		// These Redirector types are not supported here because they do not resolve to TItemMinors
		if (ListItem->ItemType.Contains(itMembershipDisplayNote))
			PassedFilter = false;
		if (ListItem->ItemType.Contains(itMembershipDisplay))
			PassedFilter = false;
		if (ListItem->ItemType.Contains(itDiscountDisplay))
			PassedFilter = false;
		if (ListItem->ItemType.Contains(itEarntPts))
			PassedFilter = false;
		if (ListItem->ItemType.Contains(itLoadedPts))
			PassedFilter = false;

		if (PassedFilter)
		{
			// Find the Redirectors TItemMinor For options this is the parent Item.
			if (ListItem->ItemType.Contains(itOption))
			{
				ListItem = (TItemRedirector*)ListItem->ParentRedirector;
			}

			RetVal = static_cast<TItemMinorComplete*>(ListItem->ItemObject);
		}

	}
	return RetVal;
}
// ---------------------------------------------------------------------------
TItemMinor * TfrmSelectDish::GetSelectedSetMenuMaster()
{
	TItemMinor * RetVal = NULL;
	if (lbDisplay->ItemIndex > -1)
	{
		TItemRedirector *ListItem = (TItemRedirector*)lbDisplay->Items->Objects[lbDisplay->ItemIndex];
		// Is a Master Item
		if (ListItem->ItemType.Contains(itSetMenuMasterItem))
		{
			// Master Item is a Previous Order.
			if (ListItem->ItemType.Contains(itPrevItem))
			{
				RetVal = static_cast<TItemMinor*>(ListItem->ItemObject);
			}
			else
			{
				RetVal = static_cast<TItemMinor*>(ListItem->CompressedContainer->ItemsList[0]);
			}
		}
	}
	return RetVal;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tbtnToggleMenusMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	tbtnToggleMenus->Tag = 0;
	btnTimer->Enabled = true;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tbtnToggleMenusMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	btnTimer->Enabled = false;
	bool SelectionRequired = false;
	if (tbtnToggleMenus->Tag == 0) // Toggle
	{
		if (TDeviceRealTerminal::Instance().Menus->Current->Count <= 2)
		{
			TDeviceRealTerminal::Instance().Menus->ToggleMenus();
			if (TDeviceRealTerminal::Instance().Menus->VisibleMenu == NULL)
			{
				SelectionRequired = true;
			}
			else if (pnlItemModify->Visible && GetModifyOptionSelected() == eBTDSides)
			{
				RedrawItemSideCourses();
			}
		}
		else
		{
			if (frmSelectCurrentMenu->ShowModal() == mrOk)
			{
				if (frmSelectCurrentMenu->SelectedMenuIndex != -1)
				{
					TDeviceRealTerminal::Instance().Menus->VisibleMenu = TDeviceRealTerminal::Instance().Menus->Current->MenuGet(frmSelectCurrentMenu->SelectedMenuIndex);
					if (TDeviceRealTerminal::Instance().Menus->VisibleMenu == NULL)
					{
						SelectionRequired = true;
					}
					else if (pnlItemModify->Visible && GetModifyOptionSelected() == eBTDSides)
					{
						RedrawItemSideCourses();
					}
				}
				else
				{
					SelectionRequired = true;
				}
			}
		}
	}

	if (SelectionRequired)
	{
		tbtnToggleMenus->Tag = 0;
		SelectNewMenus();
	}
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::SelectNewMenus()
{
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
        bool AskForLogin = false;
	if (!TDeviceRealTerminal::Instance().Menus->GetMenusExist(DBTransaction))
	{
		MessageBox("There are no menus to change to. Add One to the Database.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (DeleteAllUnsentAndProceed(DBTransaction))
	{
		std::auto_ptr<TfrmSelectActiveMenus>frmSelectActiveMenus(TfrmSelectActiveMenus::Create(this, TDeviceRealTerminal::Instance().Menus, DBTransaction));
		if (frmSelectActiveMenus->ShowModal() == mrOk)
		{
			bool Broadcast = false;
			if (MessageBox("Do you wish all terminals to use this menu configuration?", "Update all terminals", MB_YESNO + MB_ICONQUESTION) == IDYES)
			{
				TMMContactInfo TempUserInfo;
				std::auto_ptr<TContactStaff>Staff(new TContactStaff(DBTransaction));
				TLoginSuccess Result = Staff->Login(this, DBTransaction, TempUserInfo, CheckDisable);
				if (Result == lsAccepted)
				{
					Broadcast = true;
                    Refresh();
				}
				else if (Result == lsDenied)
				{
					MessageBox("You do not have access to change Menus system wide.", "Error", MB_OK + MB_ICONERROR);
				}
				else if (Result == lsPINIncorrect)
				{
					MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
				}
			}
                        else
                         {
                           AskForLogin = true;
                         }
			std::auto_ptr<TNetMessageMenuChanged>dbRequest(new TNetMessageMenuChanged);
			for (int i = 0; i < frmSelectActiveMenus->pnlMenus->ControlCount; i++)
			{
				if (((TTouchBtn*)frmSelectActiveMenus->pnlMenus->Controls[i])->ButtonColor == clGreen)
				{
					dbRequest->Menu_Names[(((TTouchBtn*)frmSelectActiveMenus->pnlMenus->Controls[i])->Caption)] = eMenuAddReplace;
				}
			}
			dbRequest->Broadcast = Broadcast;
			TDeviceRealTerminal::Instance().Menus->MenuChanged(DBTransaction, dbRequest.get());
			TDeviceRealTerminal::Instance().Menus->SwapInNewMenus();
			TDeviceRealTerminal::Instance().Menus->SetMenuList(DBTransaction, TDeviceRealTerminal::Instance().ID.DeviceKey);
		}
                else
                {
                   AskForLogin = true;
                }
	}
	DBTransaction.Commit();
    if (AskForLogin)
    {
       AutoLogOut();
    }

    DisplayRoomNoUI();

    //MM-1647: Ask for chit if it is enabled for every order.
    NagUserToSelectChit();
}
// ---------------------------------------------------------------------------
eBtnToDisplay TfrmSelectDish::GetModifyOptionSelected()
{
	eBtnToDisplay RetVal = eBTDNone;
	for (int i = 0; i < btngridModify->RowCount; i++)
	{
		if (btngridModify->Buttons[i][0]->Latched)
		{
			RetVal = static_cast<eBtnToDisplay>(btngridModify->Buttons[i][0]->Tag);
		}
	}
	return RetVal;
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::PaintItemToDisplay(long itemKey, long sizesKey , AnsiString qtyValue , AnsiString billValue)
{
    TItem *Item = TDeviceRealTerminal::Instance().Menus->FetchItemByKey(itemKey);
	if (Item)
	{
        dc_item_show = true;
        bool Proceed = true;
        SelectedOrderMenuName = Item->MenuName;

		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();

        TItemComplete *SetMenuDish = new TItemComplete;

        SetMenuDish->ItemOrderedFrom = Item;
        SetMenuDish->TableNo = 000;
        SetMenuDish->SeatNo = 0;
        SetMenuDish->Item = Item->Item;
        SetMenuDish->ItemKitchenName = Item->ItemKitchenName;
        SetMenuDish->ItemKey = Item->ItemKey;
        SetMenuDish->Item_ID = Item->Item_ID;
        SetMenuDish->MenuName = Item->MenuName;
        // TODO 4 -o Michael -c Printing : Remove When Printing Engine Finished
        SetMenuDish->MenuKey = Item->MenuKey;
        SetMenuDish->ItemType = Item->ItemType;
        SetMenuDish->PrintChitNumber = Item->PrintChitNumber;
        SetMenuDish->Terminal = "DC_Term";
        SetMenuDish->TimeStamp = Now();
        SetMenuDish->TransNo = TDBOrder::GetNextTransNumber(DBTransaction);
        SetMenuDish->Note = "";
        SetMenuDish->Course = Item->Course;
        SetMenuDish->CourseKitchenName = Item->CourseKitchenName;
        SetMenuDish->Security->SetSecurityRefNumber(TDBSecurity::GetNextSecurityRef(DBTransaction));

        int ServingCourseKey = Item->Owner->DefaultServingCourseKey;
        if (Item->Owner->DefaultServingCourseKey == 0)
        {
            ServingCourseKey = frmSelectDish->GetServingCourse();
        }

        SetMenuDish->ServingCourse = TDeviceRealTerminal::Instance().Menus->GetServingCourse(ServingCourseKey);

        // Sort Recipes
        TItemSize* itemSize = Item->Sizes->SizeGetByItemSizeKey(sizesKey);
        if(itemSize)
        {
             SetMenuDish->SalesRecipesToApply->RecipeCopyList(itemSize->Recipes);
            SetMenuDish->Categories->CategoryCopyList(itemSize->Categories);

            SetMenuDish->GSTPercent = itemSize->GSTPercent;
            SetMenuDish->TaxProfiles = itemSize->TaxProfiles;
        }

        // Sort Cost.
        SetMenuDish->Cost = 0;
        if (Item->Sizes->SizeGetByItemSizeKey(sizesKey)->Recipes->Count > 0)
        {
            for (UINT i = 0; i < Item->Sizes->SizeGetByItemSizeKey(sizesKey)->Recipes->Count; i++)
            {
                SetMenuDish->Cost += Item->Sizes->SizeGetByItemSizeKey(sizesKey)->Recipes->RecipeGet(i)->Cost;
            }
        }
        else
        {
            SetMenuDish->Cost = Item->Sizes->SizeGetByItemSizeKey(sizesKey)->Cost; // Default cost if no recipes.
        }

        SetMenuDish->SetPriceLevelCustom(0); // Set Menu Items have no price. Just a cost.
        SetMenuDish->Size = Item->Sizes->SizeGetByItemSizeKey(sizesKey)->Name;

        SetMenuDish->ClaimAvailability();

        SetMenuDish->SetMenuMask = Item->Sizes->SizeGetByItemSizeKey(sizesKey)->SetMenuMask;
        SetMenuDish->SetMenu = Item->Sizes->SizeGetByItemSizeKey(sizesKey)->SetMenuItem;
        SetMenuDish->SetMenuMaster = false;
        SetMenuDish->SizeKitchenName = Item->Sizes->SizeGetByItemSizeKey(sizesKey)->SizeKitchenName;
        SetMenuDish->ThirdPartyKey = Item->Sizes->SizeGetByItemSizeKey(sizesKey)->ThirdPartyKey;
        SetMenuDish->ThirdPartyCode = Item->Sizes->SizeGetByItemSizeKey(sizesKey)->ThirdPartyCode;
        SetMenuDish->PLU = Item->Sizes->SizeGetByItemSizeKey(sizesKey)->PLU;

        SetMenuDish->PointsPercent = Item->Sizes->SizeGetByItemSizeKey(sizesKey)->PointsPercent;

   //     SetMenuDish->PriceLevel1 =   VerifyPriceLevelPrice(DBTransaction, Item->Sizes->SizeGetByItemSizeKey(sizesKey)->ItemSizeKey, Item->Sizes->SizeGetByItemSizeKey(sizesKey)->PriceLevels[stHappyHour->Tag].Price);

        int priceLevel = atoi(billValue.c_str()) ;
        int qtyDC = atoi(qtyValue.c_str());
        double priceLeveld =  (priceLevel / 100.0);
        double price = double((priceLeveld * 1000) / (qtyDC));

        SetMenuDish->PriceLevel0 = price;
        SetMenuDish->PriceLevel1 =  VerifyPriceLevelPrice(DBTransaction, Item->Sizes->SizeGetByItemSizeKey(sizesKey)->ItemSizeKey, Item->Sizes->SizeGetByItemSizeKey(sizesKey)->PriceLevels[stHappyHour->Tag].Price);

        SetMenuDish->SetPriceLevel0();
        SetMenuDish->SetPriceLevel1();

        SetMenuDish->MemberFreeSaleCount = Item->Sizes->SizeGetByItemSizeKey(sizesKey)->MemberFreeSaleCount;
        SetMenuDish->MemberFreeSaleDiscount = Item->Sizes->SizeGetByItemSizeKey(sizesKey)->MemberFreeSaleDiscount;
        SetMenuDish->LocationFreeSaleCount = Item->Sizes->SizeGetByItemSizeKey(sizesKey)->LocationFreeSaleCount;
        SetMenuDish->LocationFreeSaleDiscount = Item->Sizes->SizeGetByItemSizeKey(sizesKey)->LocationFreeSaleDiscount;

        SetMenuDish->SetMenuGroup = SetMenuGroupNo;

        SetMenuDish->OrderType = NormalOrder;
        SetMenuDish->HappyHour = stHappyHour->Visible;

        double qtyValF = 0.0;
        if(price != 0)
        {
               qtyValF = double(qtyDC/1000.0);
        }
        else
        {
            qtyValF = 0;
        }

        SetMenuDish->SetQty(StrToCurr(qtyValF));

        std::auto_ptr<TListOptionContainer>ForcedOptionsList(new TListOptionContainer);
        std::auto_ptr<TListSideContainer>ForcedSidesList(new TListSideContainer);
        // Do Forced Sides.
        std::auto_ptr<TfrmSelectForcedOptions>frmSelectForcedOptions(TfrmSelectForcedOptions::Create<TfrmSelectForcedOptions>(this));
        std::auto_ptr<TfrmSelectForcedSides>frmSelectForcedSides(TfrmSelectForcedSides::Create<TfrmSelectForcedSides>(this, DBTransaction));

        if (TGlobalSettings::Instance().ForcedSidesFirst)
        {
            for (int i = 1; i < 16 && Proceed; i++) // Max 16 Possible Forced Options Groups.
            {
                ForcedSidesList->Clear();
                frmSelectForcedSides->MasterItem = SetMenuDish;
                Item->Sides->SidesRetriveMaskedList(i, ForcedSidesList.get());
                if (ForcedSidesList->Count > 0)
                {
                    frmSelectForcedSides->ForcedSidesList = ForcedSidesList.get();
                    if (frmSelectForcedSides->ShowModal() == mrCancel)
                    {
                        SetMenuDish->ReturnToAvailability();
                        delete SetMenuDish;
                        SetMenuDish = NULL;
                        Proceed = false;
                    }
                }
            }

            // Do Forced Options.
            for (int i = 1; i < 16 && Proceed; i++) // Max 16 Possible Forced Options Groups.
            {
                ForcedOptionsList->OptionClear();
                retrieveForcedOptionList( i, Item, ForcedOptionsList.get() );
                //Item->Options->OptionRetriveMaskedList(i, Item->OptionGroup, ForcedOptionsList.get());
                if (ForcedOptionsList->Count > 0)
                {
                    frmSelectForcedOptions->ForcedOptionsList = ForcedOptionsList.get();
                    if (frmSelectForcedOptions->ShowModal() == mrCancel)
                    {
                        SetMenuDish->ReturnToAvailability();
                        delete SetMenuDish;
                        SetMenuDish = NULL;
                        Proceed = false;
                    }
                    else
                    {
                        SetMenuDish->OptionsSelected->OptionAddList(frmSelectForcedOptions->SelectedOptions);
                    }
                }
            }
        }
        else
        {
            // Do Forced Options.
            for (int i = 1; i < 16 && Proceed; i++) // Max 16 Possible Forced Options Groups.
            {
                ForcedOptionsList->OptionClear();
                retrieveForcedOptionList( i, Item, ForcedOptionsList.get() );
                //Item->Options->OptionRetriveMaskedList(i, Item->OptionGroup, ForcedOptionsList.get());
                if (ForcedOptionsList->Count > 0)
                {
                    frmSelectForcedOptions->ForcedOptionsList = ForcedOptionsList.get();
                    if (frmSelectForcedOptions->ShowModal() == mrCancel)
                    {
                        SetMenuDish->ReturnToAvailability();
                        delete SetMenuDish;
                        SetMenuDish= NULL;
                        Proceed = false;
                    }
                    else
                    {
                        SetMenuDish->OptionsSelected->OptionAddList(frmSelectForcedOptions->SelectedOptions);
                    }
                }
            }

            for (int i = 1; i < 16 && Proceed; i++) // Max 16 Possible Forced Options Groups.
            {
                ForcedSidesList->Clear();
                frmSelectForcedSides->MasterItem = SetMenuDish;
                Item->Sides->SidesRetriveMaskedList(i, ForcedSidesList.get());
                if (ForcedSidesList->Count > 0)
                {
                    frmSelectForcedSides->ForcedSidesList = ForcedSidesList.get();
                    if (frmSelectForcedSides->ShowModal() == mrCancel)
                    {
                        SetMenuDish->ReturnToAvailability();
                        delete SetMenuDish;
                        SetMenuDish = NULL;
                        Proceed = false;
                    }
                }
            }
        }

        DBTransaction.Commit();
        if (Proceed)
        {
            SeatOrders[SelectedSeat]->Orders->Add(SetMenuDish, SetMenuDish->ItemOrderedFrom);
            RedrawSeatOrders();
            HighlightSelectedItem();
            TotalCosts();
            UpdateExternalDevices();
            UpdateTableButton();
        }
	}
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::PaintItemToDisplay(long itemKey, int GlassType)
{
    TItem *Item = TDeviceRealTerminal::Instance().Menus->FetchItemByKey(itemKey);
	if (Item)
	{
        bool Proceed = true;
        SelectedOrderMenuName = Item->MenuName;

		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();

        TItemComplete *SetMenuDish = new TItemComplete;
        SetMenuDish->ItemOrderedFrom = Item;
        SetMenuDish->TableNo = 0;
        SetMenuDish->SeatNo = 0;
        SetMenuDish->Item = Item->Item;
        SetMenuDish->ItemKitchenName = Item->ItemKitchenName;
        SetMenuDish->ItemKey = Item->ItemKey;
        SetMenuDish->Item_ID = Item->Item_ID;
        SetMenuDish->MenuName = Item->MenuName;
        // TODO 4 -o Michael -c Printing : Remove When Printing Engine Finished
        SetMenuDish->MenuKey = Item->MenuKey;
        SetMenuDish->ItemType = Item->ItemType;
        SetMenuDish->PrintChitNumber = Item->PrintChitNumber;
        SetMenuDish->TimeStamp = Now();
        SetMenuDish->TransNo = TDBOrder::GetNextTransNumber(DBTransaction);
        SetMenuDish->Note = "";
        SetMenuDish->Course = Item->Course;
        SetMenuDish->CourseKitchenName = Item->CourseKitchenName;
        SetMenuDish->Security->SetSecurityRefNumber(TDBSecurity::GetNextSecurityRef(DBTransaction));

        int ServingCourseKey = Item->Owner->DefaultServingCourseKey;
        if (Item->Owner->DefaultServingCourseKey == 0)
        {
            ServingCourseKey = frmSelectDish->GetServingCourse();
        }

        SetMenuDish->ServingCourse = TDeviceRealTerminal::Instance().Menus->GetServingCourse(ServingCourseKey);

        // Sort Recipes
        TItemSize* itemSize = Item->Sizes->SizeGet(GlassType);
        if(itemSize)
        {
             SetMenuDish->SalesRecipesToApply->RecipeCopyList(itemSize->Recipes);
             SetMenuDish->Categories->CategoryCopyList(itemSize->Categories);
        }

        // Sort Cost.
        SetMenuDish->Cost = 0;
        if (Item->Sizes->SizeGet(GlassType)->Recipes->Count > 0)
        {
            for (UINT i = 0; i < Item->Sizes->SizeGet(GlassType)->Recipes->Count; i++)
            {
                SetMenuDish->Cost += Item->Sizes->SizeGet(GlassType)->Recipes->RecipeGet(i)->Cost;
            }
        }
        else
        {
            SetMenuDish->Cost = Item->Sizes->SizeGet(GlassType)->Cost; // Default cost if no recipes.
        }

        SetMenuDish->SetPriceLevelCustom(0); // Set Menu Items have no price. Just a cost.
        SetMenuDish->Size = Item->Sizes->SizeGet(GlassType)->Name;

        SetMenuDish->ClaimAvailability();

        SetMenuDish->SetMenuMask = Item->Sizes->SizeGet(GlassType)->SetMenuMask;
        SetMenuDish->SetMenu = Item->Sizes->SizeGet(GlassType)->SetMenuItem;
        SetMenuDish->SetMenuMaster = Item->Sizes->SizeGet(GlassType)->SetMenuMaster;
        SetMenuDish->SizeKitchenName = Item->Sizes->SizeGet(GlassType)->SizeKitchenName;
        SetMenuDish->ThirdPartyKey = Item->Sizes->SizeGet(GlassType)->ThirdPartyKey;
        SetMenuDish->ThirdPartyCode = Item->Sizes->SizeGet(GlassType)->ThirdPartyCode;
        SetMenuDish->PLU = Item->Sizes->SizeGet(GlassType)->PLU;

        SetMenuDish->PointsPercent = Item->Sizes->SizeGet(GlassType)->PointsPercent;
        SetMenuDish->PriceLevel0 = Item->Sizes->SizeGet(GlassType)->Price;
        SetMenuDish->PriceLevel1 =  VerifyPriceLevelPrice(DBTransaction,Item->Sizes->SizeGet(GlassType)->ItemSizeKey,Item->Sizes->SizeGet(GlassType)->PriceLevels[stHappyHour->Tag].Price);

        SetMenuDish->MemberFreeSaleCount = Item->Sizes->SizeGet(GlassType)->MemberFreeSaleCount;
        SetMenuDish->MemberFreeSaleDiscount = Item->Sizes->SizeGet(GlassType)->MemberFreeSaleDiscount;
        SetMenuDish->LocationFreeSaleCount = Item->Sizes->SizeGet(GlassType)->LocationFreeSaleCount;
        SetMenuDish->LocationFreeSaleDiscount = Item->Sizes->SizeGet(GlassType)->LocationFreeSaleDiscount;

        SetMenuDish->SetMenuGroup = SetMenuGroupNo;

        SetMenuDish->OrderType = NormalOrder;
        SetMenuDish->HappyHour = stHappyHour->Visible;

        std::auto_ptr<TListOptionContainer>ForcedOptionsList(new TListOptionContainer);
        std::auto_ptr<TListSideContainer>ForcedSidesList(new TListSideContainer);
        // Do Forced Sides.
        std::auto_ptr<TfrmSelectForcedOptions>frmSelectForcedOptions(TfrmSelectForcedOptions::Create<TfrmSelectForcedOptions>(this));
        std::auto_ptr<TfrmSelectForcedSides>frmSelectForcedSides(TfrmSelectForcedSides::Create<TfrmSelectForcedSides>(this, DBTransaction));

        if (TGlobalSettings::Instance().ForcedSidesFirst)
        {
            for (int i = 1; i < 16 && Proceed; i++) // Max 16 Possible Forced Options Groups.
            {
                ForcedSidesList->Clear();
                frmSelectForcedSides->MasterItem = SetMenuDish;
                Item->Sides->SidesRetriveMaskedList(i, ForcedSidesList.get());
                if (ForcedSidesList->Count > 0)
                {
                    frmSelectForcedSides->ForcedSidesList = ForcedSidesList.get();
                    if (frmSelectForcedSides->ShowModal() == mrCancel)
                    {
                        SetMenuDish->ReturnToAvailability();
                        delete SetMenuDish;
                        SetMenuDish = NULL;
                        Proceed = false;
                    }
                }
            }

            // Do Forced Options.
            for (int i = 1; i < 16 && Proceed; i++) // Max 16 Possible Forced Options Groups.
            {
                ForcedOptionsList->OptionClear();
                retrieveForcedOptionList( i, Item, ForcedOptionsList.get() );
                //Item->Options->OptionRetriveMaskedList(i, Item->OptionGroup, ForcedOptionsList.get());
                if (ForcedOptionsList->Count > 0)
                {
                    frmSelectForcedOptions->ForcedOptionsList = ForcedOptionsList.get();
                    if (frmSelectForcedOptions->ShowModal() == mrCancel)
                    {
                        SetMenuDish->ReturnToAvailability();
                        delete SetMenuDish;
                        SetMenuDish = NULL;
                        Proceed = false;
                    }
                    else
                    {
                        SetMenuDish->OptionsSelected->OptionAddList(frmSelectForcedOptions->SelectedOptions);
                    }
                }
            }
        }
        else
        {
            // Do Forced Options.
            for (int i = 1; i < 16 && Proceed; i++) // Max 16 Possible Forced Options Groups.
            {
                ForcedOptionsList->OptionClear();
                retrieveForcedOptionList( i, Item, ForcedOptionsList.get() );
                //Item->Options->OptionRetriveMaskedList(i, Item->OptionGroup, ForcedOptionsList.get());
                if (ForcedOptionsList->Count > 0)
                {
                    frmSelectForcedOptions->ForcedOptionsList = ForcedOptionsList.get();
                    if (frmSelectForcedOptions->ShowModal() == mrCancel)
                    {
                        SetMenuDish->ReturnToAvailability();
                        delete SetMenuDish;
                        SetMenuDish = NULL;
                        Proceed = false;
                    }
                    else
                    {
                        SetMenuDish->OptionsSelected->OptionAddList(frmSelectForcedOptions->SelectedOptions);
                    }
                }
            }

            for (int i = 1; i < 16 && Proceed; i++) // Max 16 Possible Forced Options Groups.
            {
                ForcedSidesList->Clear();
                frmSelectForcedSides->MasterItem = SetMenuDish;
                Item->Sides->SidesRetriveMaskedList(i, ForcedSidesList.get());
                if (ForcedSidesList->Count > 0)
                {
                    frmSelectForcedSides->ForcedSidesList = ForcedSidesList.get();
                    if (frmSelectForcedSides->ShowModal() == mrCancel)
                    {
                        SetMenuDish->ReturnToAvailability();
                        delete SetMenuDish;
                        SetMenuDish = NULL;
                        Proceed = false;
                    }
                }
            }
        }

        DBTransaction.Commit();
        if (Proceed)
        {
            SeatOrders[SelectedSeat]->Orders->Add(SetMenuDish, SetMenuDish->ItemOrderedFrom);
        }
        RedrawSeatOrders();
        HighlightSelectedItem();
        TotalCosts();
        UpdateExternalDevices();
        UpdateTableButton();
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tgridItemSetMenuItemsMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
    TItem *Item = TDeviceRealTerminal::Instance().Menus->FetchItemByKey(GridButton->Tag);
	if (Item)
	{
        SelectedOrderMenuName = Item->MenuName;

		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();

		bool Proceed = true;
		int GlassType = 0;
		if (Item->Sizes->Count == 0)
		{
			MessageBox("This item has no Size.", "Error", MB_OK + MB_ICONERROR);
			Proceed = false;
		}
		else if (Item->Sizes->Count > 1 && Item->DisplaySizes)
		{
            if(TGlobalSettings::Instance().ShowItemSizesAsList)
            {
                std::auto_ptr<TfrmSelectSizes>frmSelectSizes(TfrmSelectSizes::Create<TfrmSelectSizes>(this));
                frmSelectSizes->Caption =    Item->GetDecoratedName() +" - Select a Size";
                frmSelectSizes->Sizes->SizeCopyList(Item->Sizes);
                frmSelectSizes->SetMenuMask = SetMenuMask;
                frmSelectSizes->ShowModal();
                GlassType = frmSelectSizes->SelectedItemSizeGrid;
            }
            else
            {
                std::auto_ptr<TfrmSelectGlass>frmSelectGlass(TfrmSelectGlass::Create<TfrmSelectGlass>(this));
                frmSelectGlass->Sizes->SizeCopyList(Item->Sizes);
                frmSelectGlass->SetMenuMask = SetMenuMask;
                if (frmSelectGlass->Preprocess(true) == -1)
                {
                    frmSelectGlass->ShowModal();
                }

                if (frmSelectGlass->SelectedGlass == -1) // Menu Change while selecting glass.
                {
                    Proceed = false;
                }
                GlassType = frmSelectGlass->SelectedGlass;
            }
		}
		else // Only 1 Size or Defaulting to First Size they can change this in options.
		{
			// if the First Iems Size is Not a Set Menu Size then show the glasses
			if (Item->Sizes->SizeGet(0)->SetMenuMask & SetMenuMask)
			{
				GlassType = 0;
			}
			else
			{
                    if(TGlobalSettings::Instance().ShowItemSizesAsList)
                    {
                        std::auto_ptr<TfrmSelectSizes>frmSelectSizes(TfrmSelectSizes::Create<TfrmSelectSizes>(this));
                        frmSelectSizes->Caption =    Item->GetDecoratedName() +" - Select a Size";
                        frmSelectSizes->Sizes->SizeCopyList(Item->Sizes);
                        frmSelectSizes->SetMenuMask = SetMenuMask;
                        frmSelectSizes->ShowModal();
                        if (frmSelectSizes->SelectedItemSizeGrid == -1) // Menu Change while selecting glass.
                        {
                            Proceed = false;
                        }
                        GlassType = frmSelectSizes->SelectedItemSizeGrid;
                    }
                    else
                    {
                        std::auto_ptr<TfrmSelectGlass>frmSelectGlass(TfrmSelectGlass::Create<TfrmSelectGlass>(this));
                        frmSelectGlass->Sizes->SizeCopyList(Item->Sizes);
                        frmSelectGlass->SetMenuMask = SetMenuMask;
                        if (frmSelectGlass->Preprocess(true) == -1)
                        {
                            frmSelectGlass->ShowModal();
                        }
                        if (frmSelectGlass->SelectedGlass == -1) // Menu Change while selecting glass.
                        {
                            Proceed = false;
                        }
                        GlassType = frmSelectGlass->SelectedGlass;
                    }
			}
		}
		DBTransaction.Commit();

        if(Proceed)
            PaintItemToDisplay(GridButton->Tag, GlassType);
    }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::btnOkMouseClick(TObject *Sender)
{
   // must check for whether an order item selected and then only proceed with setting quantities.
   // it is not enough to depend on the number of items in lbDisplay list box because when applied membership, the member name is also treated as an item
   TItemMinorComplete *SelectedOrder = GetSelectedItem(TDisplayItemTypes() << itNormalItem << itSubItem << itSetMenuMasterItem);

   if (pcItemModify->ActivePage == tsQuantity && SelectedOrder != NULL )
       CheckAndSetItemQuantity();

   pnlItemModify->Visible = false;
   pnlItemModify->SendToBack();
   btngridModify->RowCount = 0;
}
// ---------------------------------------------------------------------------
double TfrmSelectDish::CountInstances(TItemComplete &archetype)
{
   TContainerOrders &active_order = *SeatOrders[SelectedSeat]->Orders;
   double n_instances = static_cast<double>(archetype.GetQty());

   for (int i = 0, j = active_order.Count; i != j; ++i) {
      TItemComplete &instance =
        reinterpret_cast<TItemComplete &>(*active_order.Items[i]);

      n_instances +=
        ((&archetype != &instance)
         && (archetype.shallow_compare(instance))
             * static_cast<double>(instance.GetQty()));
   }

   return n_instances;
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::CheckAndSetItemQuantity()
{
   if (lbDisplay->ItemIndex < 0)
	  return;

   TItemRedirector *item_reference = reinterpret_cast<TItemRedirector *>(lbDisplay->Items->Objects[lbDisplay->ItemIndex]);
   TItemComplete &item = *reinterpret_cast<TItemComplete *>(item_reference->ParentRedirector->ItemObject);

   if(item.PrintCancel)
   {
     if(ChangeItemQty)
     {
        ChangeItemQty = false;
        MessageBox("Cannot Change Item Quantity, Please Remove Item First and Add it Again", "Error", MB_OK + MB_ICONERROR);
     }
   }
   else
   {
       const double current_quantity_ordered = CountInstances(item);
       double requested_quantity_aux = 0.00;
       //check for weighted item is not exceed the max weight limit
       if(TGlobalSettings::Instance().WeightLimit < QtyDisplay->Numeric() && item.WeightedPrice.WeightedItem && TGlobalSettings::Instance().WeightLimit !=0.000)
       {
            MessageBox("Weight exceeds maximum value of " + TGlobalSettings::Instance().WeightLimit + " kg. Unable to proceed ", "Weight Exceeds", MB_OK + MB_ICONINFORMATION);
       }
       else
       {
	       if(item.WeightedPrice.WeightedItem && TGlobalSettings::Instance().NotifyForManuallyEnteredWeight)
	       {
	          item.isManuallyEnteredWeight = true;
	       }
           if ( QtyDisplay->Numeric() > 0.00 )
           {
               requested_quantity_aux = QtyDisplay->Numeric();
               if(TGlobalSettings::Instance().DeleteItemSizeAfterSale && requested_quantity_aux < 1.00)
               {
                   requested_quantity_aux =  1.00;
               }
           }
           else
           {
               requested_quantity_aux = current_quantity_ordered;
           }
           const double requested_quantity = requested_quantity_aux;
           double difference = requested_quantity - current_quantity_ordered;
           std::vector<UnicodeString> items_that_couldnt_be_acquired;

           if (item.IsFreebe)
           {
               return;
           }

           UnicodeString i = item.Item;

           MergeIdenticalOrders(item);

           if (requested_quantity < current_quantity_ordered)
            {
              item.ReturnToAvailability(current_quantity_ordered - requested_quantity);
            }
           else if (!item.ClaimAvailability(difference, &items_that_couldnt_be_acquired))
           {
                UnicodeString message = "Cannot increase the quantity to "
                                        + FloatToStrF(requested_quantity,
                                                      ffNumber,
                                                      4,
                                                      2)
                                        + ". There are not enough of the following "
                                          "items in stock:\n";
                for (std::vector<UnicodeString>::const_iterator i =
                         items_that_couldnt_be_acquired.begin();
                     i != items_that_couldnt_be_acquired.end(); ++i)
                        message = message
                                  + "  - "
                                  + *i
                                  + "\n";

                MessageBox(message,
                           "Can't change the quantity",
                           MB_ICONEXCLAMATION | MB_OK);
                return;
           }

           AdjustItemQty(requested_quantity);
        }
     }
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::MergeIdenticalOrders(TItemComplete &selected_item)
{
   /* Needed. For some reason, Embarcadero optimizes out the GetQty() call
    * erroneously which breaks the damned code...
    */
   const int qty = selected_item.GetQty();

   if (qty == 1.0 && !selected_item.SubordersSize())
   {
      std::vector<TItemComplete *> clones;
      TContainerOrders &orders = *SeatOrders[SelectedSeat]->Orders;
      TItemComplete *walker;

      for (unsigned int i = 0, j = orders.Count; i < j; i++)
      {
         walker = orders.Items[i];
         if (&selected_item != walker && selected_item.shallow_compare(*walker))
            clones.push_back(walker);
      }

      for (unsigned int i = 0, j = clones.size(); i < j; i++)
         orders.Delete(clones[i]);
      clones.clear();
   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tgridItemOptionsMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
	TItemComplete *Order = (TItemComplete*)GetSelectedItem(TDisplayItemTypes() << itNormalItem << itOption);
	if (Order)
	{
		TItem *Item = Order->ItemOrderedFrom;
		if (Item)
		{
			TItemOption *Option = Item->GetOptionByKey(GridButton->Tag);
			if (Option)
			{
				if (Order->OptionsSelected->OptionIndex(Option) == -1) // adding the option.
				{
					Order->OptionsSelected->OptionAdd(Option);
					GridButton->Color = clGreen;
					Option->IsPlus = DisplayingPlusOptions;
				}
				else // Removeing the option.
				{
					Order->OptionsSelected->OptionDelete(Option);
					GridButton->Color = clNavy;
					Option->IsPlus = DisplayingPlusOptions;
				}
			}
			SetSelectedItem(Order);
		}
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tgridOrderCourseMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
	RedrawItems();
	if (TDeviceRealTerminal::Instance().Menus->VisibleMenu)
	{
		TListCourse *Course = TDeviceRealTerminal::Instance().Menus->VisibleMenu->FetchCourseByKey(GridButton->Tag);
		Always_Prompt = Course->No_Default_Serving_Course;
		if (Course)
		{
			if (TDeviceRealTerminal::Instance().Menus->VisibleMenu->ServingCourses.size() == 1)
			{
				std::vector<TServingCourse>::iterator itServingCourse;
				int index = TDeviceRealTerminal::Instance().Menus->VisibleMenu->ServingCourseGetFirstVisibleIndex();
				if (index != -1)
				{
					TServingCourse ServingCourse = TDeviceRealTerminal::Instance().Menus->VisibleMenu->ServingCourses[index];
					SetSelectedServingCourse(ServingCourse.ServingCourseKey);
				}
				else
				{
					itServingCourse = TDeviceRealTerminal::Instance().Menus->VisibleMenu->ServingCourses.begin();
					SetSelectedServingCourse(itServingCourse->ServingCourseKey);
				}
			}
			else
			{
				SetSelectedServingCourse(Course->DefaultServingCourseKey);
			}
		}
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tgridOrderItemMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
    BeforeItemOrdered.Occured();
	if (BeforeItemOrdered.EventsFailed)
	{
		MessageBox(BeforeItemOrdered.ErrorMessage, BeforeItemOrdered.ErrorTitle, MB_OK + MB_ICONERROR);
	}
	else
	{
		TItem *Item = TDeviceRealTerminal::Instance().Menus->VisibleMenu->FetchItemByKey(GridButton->Tag);
        bool isSameMenuTypeItemExist = true;

        if(TGlobalSettings::Instance().IsBillSplittedByMenuType && Item)
            isSameMenuTypeItemExist = CheckItemCanBeAddedToSeat(Item);

		if (Item && isSameMenuTypeItemExist)
		{
			Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
			DBTransaction.StartTransaction();
            Item->ItemWeight.SetWeightIn_g(0);
			AddItemToSeat(DBTransaction, Item, false);
			DBTransaction.Commit();
		}
  	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tgridServingCourseMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
	SetSelectedServingCourse(GridButton->Tag);
    GridButton->FontColor = clWhite;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tbtnUserNameMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	OnLockOutTimer(NULL);
    //CheckUpdateMenuSetting();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tbtnChitNumberMouseClick(TObject *)
{
    std::auto_ptr<TStringList>get_list(new TStringList);
    //TStringList *get_list = new TStringList;
    Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);
    tr.StartTransaction();

    TCustNameAndOrderType *cust_name_type_instance =TGlobalSettings::Instance().CaptureCustomerName ? TCustNameAndOrderType::Instance() : 0x0;

        switch (SetupChit(tr))
         {
            case ChitCancelled:
                ChitNumber.Clear();
            /* fall through */
            case ChitDisabled:
                return;
            case ChitOk:
                 ClearDiscountLists(ChitNumber.DiscountList);
                break;
            default:
                assert(0x0
                       && "We should never get here! Insane chit result!");
                abort();
                break;
        }
         GetChitDiscountList(tr, ChitNumber.DiscountList);
    tr.Commit();

    if (cust_name_type_instance && cust_name_type_instance->IsNameCaught)
        cust_name_type_instance->CatchCustNameAndOrderType(DisplayNameKeypad(), DisplayCustomerTypeReasons(), SeatOrders);
    TotalCosts();
    UpdateExternalDevices();
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::SearchItems()
{
    std::auto_ptr<TfrmSelectItem> frmSearchItem(TfrmSelectItem::Create<TfrmSelectItem>(Screen->ActiveForm));
    frmSearchItem->StartWithShiftDown = false;
    frmSearchItem->MustHaveValidItem = true;
    frmSearchItem->Caption = "Search Item";
    int modalResult = frmSearchItem->ShowModal();
    if (modalResult == mrOk)
    {
        std::pair<TItem*, TItemSize*> itemAndSize = frmSearchItem->GetSelectedItemAndItemSize();
        OrderSearchedItem(itemAndSize);
    }
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::ResetChit()
{
    ChitNumber = TChitNumber();
    tbtnChitNumber->Caption = "Chit";
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tbtnFunctionsMouseClick(TObject *Sender)
{
	try
	{
		std::auto_ptr<TfrmFunctions>frmFunctions(TfrmFunctions::Create<TfrmFunctions>(this));
		if (TGlobalSettings::Instance().EnablePaxCount)
                	frmFunctions->btnPaxCount->Show();

		frmFunctions->ShowModal();
		int Action = frmFunctions->GetBtnSelection();
        TGlobalSettings::Instance().IsDiscountSelected = true;
                bool askForLogin = false;
		switch(Action)
		{
		case 1:
			{
				tbtnCallAwayClick();
			}break;
		case 2:
			{
				tbtnRunProgramClick();
			}break;
		case 3:
			{
				tbtnDiscountClick();
			}break;
		case 4:
			{
				tbtnLuckyMemberClick();
			}break;
		case 5:
			{
				tbtnFlashReportsClick();
                                askForLogin = true;
			}break;
		case 6:
			{
				ForceHappyHourClick();
			}break;
		case 7:
			{
				WriteOffBillAsWastage();
			}break;
		case 8:
			{
				AlterFloat();
			}break;
		case 9:
			{
				EnterPaxCount();
			}break;
		case 10:
			{
				tbtnDiscountClick(true);
			}break;
        case 11:
			{
				SearchItems();
			}break;
               case 12:
			{
				ChangeCard();
			}break;
            case 13:
            {
                OpenTransactionAuditScreen();
			}break;
            case 15:
            {
                DoCloundSync();
			}break;
            case 16:
            {
                CheckGiftCardBalance();
			}break;

        }
       if (askForLogin)
       {
         AutoLogOut();
       }
        //MM-1647: Ask for chit if it is enabled for every order.
        NagUserToSelectChit();
	}
	catch(Exception & E)
	{
		MessageBox(E.Message, "Error", MB_OK + MB_ICONERROR);
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tbtnParkSalesMouseClick(TObject *Sender)
{
	OrderHeld = false;
	if (!OrdersPending())
	{ // Display Parked Sales.
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();
        if (ParkedSales->GetCount(DBTransaction) > 0)
         {
			try
			{
				if (frmParkedSales == NULL)
				{
					Application->CreateForm(__classid(TfrmParkedSales), &frmParkedSales);
				}
				frmParkedSales->ParkedSales = ParkedSales;
				if (frmParkedSales->ShowModal() == mrOk)
				{

					TParkedSale *Sale = ParkedSales->GetParkedSale(DBTransaction, frmParkedSales->SelectedParkedSaleTabKey);
					if (Sale != NULL)
					{
						Sale->AssignParkedSale(SeatOrders);
						ParkedSales->Delete(DBTransaction, frmParkedSales->SelectedParkedSaleTabKey);
						SelectedSeat = Sale->SelectedSeat;
						SelectedTable = Sale->SelectedTable;
						SelectedTabContainerName = Sale->SelectedTabContainerName;
						SelectedParty = Sale->SelectedParty;
                        selectedRoomNumberStr = SeatOrders[SelectedSeat]->Orders->pmsAccountDetails.RoomNumber;
                        if(TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount.Pos(selectedRoomNumberStr))
                        {
                            isWalkInUser = true;
                            SiHotAccount.AccountDetails.clear();
                        }
                        else if(!SiHotAccount.AccountDetails.size())
                        {
                             GetRoomDetails();
                        }

                        //MM-1649: Ask for patron count if it is enabled..
                        if(SelectedTable > 0)
                        {
                            //Get the patrons for the current selected table, verify if it is already keyed in
                            std::vector<TPatronType> selectedTablePatrons = TDBTables::GetPatronCount(DBTransaction, SelectedTable);
                            int patronCount = GetCount(selectedTablePatrons);
                        }

						if (Sale->StoreMembership)
						{
							Membership.Assign(Sale->Membership);
						}
						delete Sale;
                        Sale = NULL;
					}
					else
					{
						MessageBox("Sorry, unable to load this parked sale.\rPlease restart Menumate POS and try again.",
							"Sorry Unable to load parked sale.", MB_OK + MB_ICONWARNING);
						if (MessageBox("Delete this Parked Sale?", "Warning", MB_YESNO + MB_ICONQUESTION) == IDYES)
						{
							ParkedSales->Delete(DBTransaction, frmParkedSales->SelectedParkedSaleTabKey);
						}
					}
				}
			}
            catch(Exception & E)
            {
                    TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
            }
			delete frmParkedSales;
			frmParkedSales = NULL;
		}

		setParkedSalesBtnColor();

		if (SelectedSeat != 0)
		{
			SetSelectedSeat();
		}

		if (TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->CardOk)
		{ // Restore Membership, Reminds the user to remove the smart card.
			OnSmartCardInserted(NULL);
		}
		else
		{
			OnSmartCardRemoved(NULL);
		}

		RefreshSeats();
		RedrawSeatOrders();
		TotalCosts();
		UpdateExternalDevices();
		DBTransaction.Commit();
	}
	else
	{ // Park the current sale.
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();

		TParkedSale *Sale = new TParkedSale;

        if(TGlobalSettings::Instance().IsThorlinkSelected)
        {
           RemoveMembership(DBTransaction);
        }
		// Only store the membership if we not using smart cards.
		if (TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->Enabled)
		{
			if (TDeviceRealTerminal::Instance().ManagerMembership->AllowSmartCardNotPresentMemberTransactions)
			{
				Sale->StoreMembership = true;
			}
		}
		else
		{
			Sale->StoreMembership = true;
		}

		if (Sale->StoreMembership)
		{
			Sale->Membership.Assign(Membership);
		}
		Sale->SetSale(TDeviceRealTerminal::Instance().User.Name, SeatOrders);
		Sale->Time = Now();
		Sale->SelectedSeat = SelectedSeat;
		Sale->SelectedTable = SelectedTable;
		Sale->SelectedTabContainerName = SelectedTabContainerName;
		Sale->SelectedParty = SelectedParty;
        setParkedSale( Sale );
        delete Sale;
        Sale = NULL;
		lbDisplay->Clear();
		for (UINT i = 0; i < SeatOrders.size(); i++)
		{
			try
			{
				while (SeatOrders[i]->Orders->Count != 0)
				{
					SeatOrders[i]->Orders->Delete(0);
				}
				// Prev Items are not Stored Delete them.
				while (SeatOrders[i]->Orders->PrevCount != 0)
				{
					delete SeatOrders[i]->Orders->PrevItems[0];
					SeatOrders[i]->Orders->DeletePrev(0);
				}
			}
			__finally
			{
				SeatOrders[i]->Orders->Clear();
				SeatOrders[i]->Orders->ClearPrev();
				SeatOrders[i]->Orders->AppliedMembership.Clear();
			}
		}

       // if (tbtnParkSales->ButtonColor != 0x002193F6)
	   if (tbtnParkSales->ButtonColor != clGreen)
        {
			tbtnParkSales->Tag = tbtnParkSales->ButtonColor;
		}
        setParkedSalesBtnColor();
		RedrawSeatOrders();
		TDBSaleTimes::VoidSaleTime(DBTransaction, CurrentTimeKey);
		DBTransaction.Commit();
		CurrentTimeKey = 0;
		ResetPOS();
        DisplayRoomNoUI();
	}
      AutoLogOut();
    //MM-1647: Ask for chit if it is enabled for every order.
    NagUserToSelectChit();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tbtnReprintReceiptsMouseClick(TObject *Sender)
{
	if (ManagerReceipt->ReceiptsExist())
	{
          TMMContactInfo TempUserInfo;
          Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	      DBTransaction.StartTransaction();
	      std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
          bool AccessForReprint = Staff->TestAccessLevel( TDeviceRealTerminal::Instance().User, CheckReprintReceipt);
          if(AccessForReprint)
          {
                TDeviceRealTerminal::Instance().User.LatestLoginContactKey =TDeviceRealTerminal::Instance().User.ContactKey;
                TDeviceRealTerminal::Instance().User.LatestLoginName  = TDeviceRealTerminal::Instance().User.Name;
                DBTransaction.Commit();
                ManagerReceipt->PrintLastReceipt();
          }
          else
          {
              TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckReprintReceipt);
              DBTransaction.Commit();
              TDeviceRealTerminal::Instance().User.LatestLoginContactKey =0;
              TDeviceRealTerminal::Instance().User.LatestLoginName  = "";
              if (Result == lsAccepted)
              {
                    TDeviceRealTerminal::Instance().User.LatestLoginContactKey =TempUserInfo.ContactKey;
                    TDeviceRealTerminal::Instance().User.LatestLoginName  = TempUserInfo.Name;
                    ManagerReceipt->PrintLastReceipt();
              }
              else if (Result == lsDenied)
              {
                    MessageBox("You do not have access privileges to reprint receipt.", "Error", MB_OK + MB_ICONERROR);
              }
              else if (Result == lsPINIncorrect)
              {
                    MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
              }
          }
    }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tbtnOpenDrawerMouseClick(TObject *Sender)
{
	TMMContactInfo TempUserInfo;
	frmSelectDish->PreUserInfo = TDeviceRealTerminal::Instance().User;
    bool openCashDrawer = false;

	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff>Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this, DBTransaction, TempUserInfo, CheckOpenDrawer);
	DBTransaction.Commit();
    UnicodeString quickMessage = "";
	if (Result == lsAccepted)
	{
		DBTransaction.StartTransaction();
		TDeviceRealTerminal::Instance().User = TempUserInfo;
        TStringList *MessageList = new TStringList;
        ManagerMessage->GetListTitle(DBTransaction, MessageList, eCashDrawer);
        if(MessageList->Count == 1)
        {
            delete MessageList;
            MessageList = NULL;
        }
        else
        {
            delete MessageList;
            MessageList = NULL;
            std::auto_ptr <TfrmMessage> frmMessage(TfrmMessage::Create <TfrmMessage> (this, TDeviceRealTerminal::Instance().DBControl));
            frmMessage->MessageType = eCashDrawer;
            if(frmMessage->ShowModal() == mrOk)
            {
               quickMessage = frmMessage->TextResult;
            }
            else
            {
               openCashDrawer = true;
            }
        }
        if(!openCashDrawer)
        {
            std::auto_ptr <TManagerFloat> (FloatManager)(new TManagerFloat((TForm*)this));
            FloatManager->OpenCashDrawerAccordingToPrinter(DBTransaction);
            TDBSecurity::ProcessSecurity(DBTransaction, TDBSecurity::GetNextSecurityRef(DBTransaction), TDeviceRealTerminal::Instance().User.ContactKey, SecurityTypes[secOpenDraw],
            TDeviceRealTerminal::Instance().User.Name, TDeviceRealTerminal::Instance().User.Initials, Now(), TDeviceRealTerminal::Instance().ID.Name, quickMessage);
        }
		DBTransaction.Commit();
	}
	else if (Result == lsDenied)
	{
		MessageBox("You do not have access to Open Drawer.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}
    AutoLogOut();
    //MM-1647: Ask for chit if it is enabled for every order.
    NagUserToSelectChit();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tbtnSystemMouseClick (TObject *Sender)
{
	IsSubSidizeProcessed=true;
	IsTabBillProcessed=true;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	bool Proceed = false;
    if(dc_item_show && TGlobalSettings::Instance().IsDrinkCommandEnabled)
    {
      Proceed = DeleteUnsentDCAndProceed(DBTransaction);
      AnsiString memberNo = "";
      int end_idx , strt_idx = 0;
      char*  memberDetails_char = new char[memberName.Length() + 1];
      strcpy(memberDetails_char,memberName.c_str());
      int len = 0;
      for (int i = memberName.Length()-1 ; i > 0 ; i--)
      {
         if (memberDetails_char[i] == ')' )
         {
            end_idx = i;
         }
         if (memberDetails_char[i] == '(' )
         {
            strt_idx = i;
            len = (end_idx - strt_idx -1);
            break;
         }
      }
       memberNo = memberName.SubString(strt_idx+2, len);
       AnsiString cardId = TDrinkCommandData::Instance().GetCardIdFromMemberNo(memberNo) ;
       TDrinkCommandData::Instance().UpdateTimeStampToNull(cardId)  ;
    }
    else
    {
       Proceed = DeleteUnsentAndProceed(DBTransaction);
    }
	DBTransaction.Commit();

	if (Proceed)
	{
		std::auto_ptr<TfrmPOSMain>frmPOSMain(TfrmPOSMain::Create<TfrmPOSMain>(this, TDeviceRealTerminal::Instance().DBControl));
		frmPOSMain->ShowModal();
        bool isExitPressed = false;
		if (frmPOSMain->ExitPOS)
		{
            isExitPressed = true;
			Close();
		}
        else if (frmPOSMain->RedrawMenus)
        {
            if(frmPOSMain->MenuEdited)
            {
                RedrawMenu();
                UpdateMenuItemsAfterLoginScreen();
            }
        }
		else if (frmPOSMain->SendHeldOrders)
		{
			std::auto_ptr<TList>OrdersList(new TList);
			std::vector<TPatronType> Patrons;
			while(SendButtonClick(OrdersList, Patrons))
			{
				DBTransaction.StartTransaction();
				SendOrderListToKitchen(DBTransaction, OrdersList.get(), Patrons);
				DBTransaction.Commit();
				OrderHeld = false;
			}
		}
		ResetPOS();
        AutoLogOut();
        if(frmPOSMain->ShowTablePicker && TGlobalSettings::Instance().EnableTableDisplayMode)
        {
           showTablePicker();
        }
        if(!isExitPressed)
            DisplayRoomNoUI();
        NagUserToSelectChit();

        tbtnDollar1->Caption = GetTenderStrValue( vmbtnDollar1 );
        tbtnDollar2->Caption = GetTenderStrValue( vmbtnDollar2 );
        tbtnDollar3->Caption = GetTenderStrValue( vmbtnDollar3 );
        tbtnDollar4->Caption = GetTenderStrValue( vmbtnDollar4 );
        tbtnDollar5->Caption = GetTenderStrValue( vmbtnDollar5 );
	}
	IsSubSidizeProcessed=false;
	IsTabBillProcessed=false;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tbtnMembershipMouseClick(TObject *Sender)
{
  //mm-5145
     if(TGlobalSettings::Instance().MandatoryMembershipCard )
     {
            // is membeship is already applied then make the membership button color should not change to red
            if((SeatOrders[SelectedSeat]->Orders->AppliedMembership.ContactKey == 0))
            {
                 tbtnMembership->ButtonColor = clRed;
            }
            else
            {
                tbtnMembership->ButtonColor = clNavy;
            }
     }
    else
    {

             tbtnMembership->ButtonColor = clNavy;   //mm-5145

        if(TGlobalSettings::Instance().EnablePhoneOrders)
           TDeviceRealTerminal::Instance().ManagerMembership->SetPhoneOrderFlowMemberSelection(true);

     	if (TDeviceRealTerminal::Instance().Modules.Status[eSmartCardSystem]["Enabled"])
    	{
	 	// If there is a card inserted go stright to editing that user.
	    	if (TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->CardOk)
	    	{
			Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
			TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);

			TMMContactInfo UserInfo;

			DBTransaction.StartTransaction();
			std::auto_ptr<TContactStaff>Staff(new TContactStaff(DBTransaction));
			TLoginSuccess Result = Staff->Login(this, DBTransaction, UserInfo, CheckAccountManager);

			if (Result == lsAccepted)
			{
				TDeviceRealTerminal::Instance().ManagerMembership->EditMember(DBTransaction, SeatOrders[SelectedSeat]->Orders->AppliedMembership);
			}
			else if (Result == lsDenied)
			{
				MessageBox("You do not have access to the loyalty settings.", "Error", MB_OK + MB_ICONERROR);
			}
			else if (Result == lsPINIncorrect)
			{
				MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
			}
			DBTransaction.Commit();
	    	}
	    	else if (TDeviceRealTerminal::Instance().ManagerMembership->AllowSmartCardNotPresentMemberTransactions)
	    	{
	    		TMMContactInfo TempUserInfo;
	    		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	    		TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	    		DBTransaction.StartTransaction();
	        	ApplyMembership(DBTransaction, TempUserInfo);
		    	DBTransaction.Commit();

	    	}
	    }
        	else if (TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Enabled"])
        	{
	        	TMMContactInfo TempUserInfo;
	        	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	        	TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	        	DBTransaction.StartTransaction();


                if (dc_item_show)
                {
                  bool Proceed = DeleteUnsentDCAndProceed(DBTransaction);
                  lbDisplay->Clear();
                  AnsiString memberNo = "";
                  int end_idx , strt_idx = 0;
                  char*  memberDetails_char = new char[memberName.Length() + 1];
                  strcpy(memberDetails_char,memberName.c_str());
                  int len = 0;
                  for (int i = memberName.Length()-1 ; i > 0 ; i--)
                  {
                     if (memberDetails_char[i] == ')' )
                     {
                        end_idx = i;
                     }
                     if (memberDetails_char[i] == '(' )
                     {
                        strt_idx = i;
                        len = (end_idx - strt_idx -1);
                        break;
                     }
                  }
                   memberNo = memberName.SubString(strt_idx+2, len);
                   AnsiString cardId = TDrinkCommandData::Instance().GetCardIdFromMemberNo(memberNo) ;
                   TDrinkCommandData::Instance().UpdateTimeStampToNull(cardId)  ;
                   dc_item_show = false;
                }

                if(TGlobalSettings::Instance().LoyaltyMateEnabled)
                {
                    GetLoyaltyMember(DBTransaction,TempUserInfo);
                }
                else
                {
                    ApplyMembership(DBTransaction, TempUserInfo);
                }

	            DBTransaction.Commit();
                if (!TGlobalSettings::Instance().EnablePhoneOrders)
                {
                    AutoLogOut();
                }
                NagUserToSelectChit();
            }
        	else
            {
                MessageBox("Membership is not Enabled.", "Error", MB_OK + MB_ICONERROR);
            }
    }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tbtnSaveMouseClick(TObject *Sender)
{
    IsSubSidizeProcessed=false;
	if(IsSubSidizeOrderCancil)
	{
        IsSubSidizeOrderCancil=false;

        return;

    }
	if (CurrentTender != 0)
	{
		MessageBox("You must clear the tender amount before saving orders.", "Error", MB_OK + MB_ICONERROR);
		return;
	}
	if (SeatOrders[0]->Orders->Count > 0)
	{
		TotalCosts();
		UpdateExternalDevices();

		TSaveOrdersTo OrderContainer;

		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
		DBTransaction.StartTransaction();

		bool Proceed = GetOrderContainer(DBTransaction, OrderContainer) == mrOk;

        // check whether table is linked to clipp tab
        TMMTabType type = TDBTab::GetLinkedTableAndClipTab(DBTransaction, OrderContainer.Location["TabKey"] );

        if(TGlobalSettings::Instance().IsThorlinkSelected)
        {
           RemoveMembership(DBTransaction);
        }
        // Commits any newly created Tabs.
        DBTransaction.Commit();

		if (Proceed && OrderContainer.Location["TabKey"] > 0)
		{
			DBTransaction.StartTransaction();
			bool TabisLocked = TDBTab::GetTabLocked(DBTransaction, OrderContainer.Location["TabKey"]);
			AnsiString CreditMessage1 = "Reason :" + TDBTab::GetTabLockedReason(DBTransaction, OrderContainer.Location["TabKey"]);
			DBTransaction.Commit();

			if (TabisLocked)
			{
                //show message that tab is locked
                Proceed = ShowTabLockedMessage(CreditMessage1);
			}
			else
			{
				DBTransaction.StartTransaction();

                //check Tab is in use
                Proceed = CheckTabIsInUse(DBTransaction, OrderContainer.Location["TabKey"]);

				DBTransaction.Commit();
			}
		}

		if (Proceed)
		{
			std::auto_ptr<TfrmProcessing>(frmProcessing)(TfrmProcessing::Create<TfrmProcessing>(this));
			frmProcessing->Message = "Posting Orders";
			frmProcessing->Show();

			bool DoProcessOrders = true;

			if (OrderContainer.Location["TMMTabType"] == TabNormal || OrderContainer.Location["TMMTabType"] == TabStaff || OrderContainer.Location["TMMTabType"] == TabMember || OrderContainer.Location["TMMTabType"] == TabClipp
                || type == TabClipp)
			{
			   	DBTransaction.StartTransaction();

                //check credit limit exceedes or not
                DoProcessOrders = CheckCreditLimitExceeds(DBTransaction, OrderContainer.Location["TabKey"]);

			    DBTransaction.Commit();
			}

			bool PaymentComplete = false;
			if (DoProcessOrders)
			{
				DBTransaction.StartTransaction();

                // save party name to database if changed
                if (OrderContainer.PartyNameChanged)
                {
					if (OrderContainer.Location["TMMTabType"] == TabTableSeat)
					{
						TDBTables::SetPartyName(DBTransaction, OrderContainer.Location["SelectedTable"], OrderContainer.Location["PartyName"]);
					}
					else if (OrderContainer.Location["TMMTabType"] == TabRoom)
					{
						TDBRooms::SetPartyName(DBTransaction, OrderContainer.Location["RoomNumber"], OrderContainer.Location["PartyName"]);
					}
				}

                if(OrderContainer.Location["TMMTabType"] == TabTableSeat)
                {
                   if(OrderContainer.Location["PrintPreLimReceipt"])
                   {
                      TMMContactInfo TempUserInfo;
                      TempUserInfo = TDeviceRealTerminal::Instance().User;
                      std::auto_ptr<TContactStaff>Staff(new TContactStaff(DBTransaction));
                      bool allowed = Staff->TestAccessLevel(TempUserInfo,CheckPaymentAccess);
                      if(allowed && TGlobalSettings::Instance().TransferTableOnPrintPrelim)
                        {
                            std::auto_ptr<TList>FoodOrdersList(new TList);
                            std::auto_ptr<TList>BevOrdersList(new TList);

                            TDBTables::SetTableBillingStatus(DBTransaction,OrderContainer.Location["SelectedTable"],eNoneStatus);
                            SouceTableForDelayedPayment =  OrderContainer.Location["ContainerName"];

                            if(TGlobalSettings::Instance().IsBillSplittedByMenuType)
                            {
                                LoadFoodAndBevList(FoodOrdersList.get(), BevOrdersList.get());
                            }

                            if(TGlobalSettings::Instance().IsBillSplittedByMenuType && !FoodOrdersList->Count && BevOrdersList->Count)
                            {
                                TManagerDelayedPayment::Instance().MoveOrderToTab(DBTransaction,OrderContainer, false);
                            }
                            else
                            {
                                TManagerDelayedPayment::Instance().MoveOrderToTab(DBTransaction,OrderContainer);
                            }
                        }
					 else
                        {
                            TDBTables::SetTableBillingStatus(DBTransaction,OrderContainer.Location["SelectedTable"],ePrelim);
                        }
                   }
                   else
                   {
                      TDBTables::SetTableBillingStatus(DBTransaction,OrderContainer.Location["SelectedTable"],eNoneStatus);
                   }
                }
				PaymentComplete = ProcessOrders(Sender,
				DBTransaction,
				OrderContainer.Location["TabKey"], 				// Tab
				OrderContainer.Location["TMMTabType"], 			// Tab Type
				OrderContainer.Location["ContainerName"],
				OrderContainer.Location["TabName"],
				OrderContainer.Location["PartyName"],
				OrderContainer.Location["PrintPreLimReceipt"],	// Print Prelim Receipt.
				OrderContainer.Location["SelectedTable"],
				OrderContainer.Location["SelectedSeat"],
				OrderContainer.Location["RoomNumber"],
				OrderContainer.Location["BillOff"],
                OrderContainer.Location["DelayedInvoiceNumber"]);

                TStringList* logList = new TStringList();
                logList->Clear();

				if (PaymentComplete)
				{
					DBTransaction.Commit();
                    logList->Add("Trabsaction commit of tbtnSaveMouseClick().");
                    TSaveLogs::RecordFiscalLogs(logList);

                    if((OrderContainer.Location["TMMTabType"] == TabClipp || type == TabClipp)
                            && (!OrderContainer.Location["BillOff"]))
                    {   //send clipp tab details
                        TManagerClippIntegration* sendClippTabKey = TManagerClippIntegration::Instance();
                        sendClippTabKey->SendTabDetails(OrderContainer.Location["TabKey"]);
                     }
					ResetPOS();

				}
				else
				{
                    logList->Add("Trabsaction rollback of tbtnSaveMouseClick().");
                    TSaveLogs::RecordFiscalLogs(logList);
					DBTransaction.Rollback();
				}
                delete logList;
                logList = NULL;
			}
			frmProcessing->Close();

			if (!OrdersPending() && IsChitPromptFormActive) // All Orders Posted No Exceptions
			{
                if( (OrderContainer.Location["BillOff"] ) && (OrderContainer.Location["TMMTabType"] == TabClipp || type == TabClipp ))
                {
                        //close clipp tab
                        TManagerClippIntegration* sendClippTabKey = TManagerClippIntegration::Instance();
                        sendClippTabKey->CloseTab(OrderContainer.Location["TabKey"]);
                }

                else if (OrderContainer.Location["BillOff"])
				{
					frmProcessing->Close();
				    TfrmBillGroup* frmBillGroup  = new  TfrmBillGroup(this, TDeviceRealTerminal::Instance().DBControl);
					frmBillGroup->ResetForm();
					if (OrderContainer.Location["TMMTabType"] == TabMember)
					{
						DBTransaction.StartTransaction();
						TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetContactDetails(DBTransaction, OrderContainer.Location["MemberKey"], frmBillGroup->TempUserInfo);
						frmBillGroup->ApplyMembership(DBTransaction, frmBillGroup->TempUserInfo);
						DBTransaction.Commit();
					}
					frmBillGroup->ResetSelection();
					frmBillGroup->CurrentDisplayMode = OrderContainer.Location["TMMDisplayMode"];
					frmBillGroup->CurrentTabType = OrderContainer.Location["TMMTabType"];
					frmBillGroup->CurrentSelectedTab = OrderContainer.Location["TabKey"];
					frmBillGroup->CurrentTable = OrderContainer.Location["SelectedTable"];
					frmBillGroup->CurrentRoom = OrderContainer.Location["RoomNumber"];

                    DBTransaction.StartTransaction();

                    //Get the patrons for the current selected table, verify if it is already keyed in
                    std::vector<TPatronType> selectedTablePatrons = TDBTables::GetPatronCount(DBTransaction, frmBillGroup->CurrentTable);
                    int patronCount = GetCount(selectedTablePatrons);

                    if(TGlobalSettings::Instance().PromptForPatronCount && patronCount > 0)
                    {
                        frmBillGroup->PatronTypes = selectedTablePatrons;
                    }
                    DBTransaction.Commit();

					if (OrderContainer.Location["TabKey"] != 0)
					{
                       const int tab_key = OrderContainer.Location["TabKey"];
                       DBTransaction.StartTransaction();
                       frmBillGroup->AddToSelectedTabs(DBTransaction, tab_key);
                       //frmBillGroup->SplitItemsInSet(DBTransaction, tab_key);
                       DBTransaction.Commit();
					}
					frmBillGroup->ShowModal();
					setPatronCount(  frmBillGroup->PatronCount );
                    delete frmBillGroup;
                    frmBillGroup = NULL;
				}
                DisplayRoomNoUI();
			}
		}
		else
		{
			bool OrdersTaken = false;
			for (UINT e = 0; e < SeatOrders.size(); e++)
			{
				if (SeatOrders[e]->Orders->Count > 0)
				{
					OrdersTaken = true;
					break;
				}
			}
			if (OrdersTaken)
			{
				if (MessageBox("Delete unsent orders?", "Warning", MB_YESNO + MB_ICONQUESTION) == IDYES)
				{
					lbDisplay->Clear();
                    patronsStore.clear();
					for (UINT f = 0; f < SeatOrders.size(); f++)
					{
						try
						{
                                                        TItemMinorComplete *item;
							for (int j = 0; j < SeatOrders[f]->Orders->Count; j++)
							{
                                                                item = SeatOrders[f]->Orders->Items[j];
                                                                item->ReturnToAvailability();
								delete item;
                                item = NULL;
							}
							for (int h = 0; h < SeatOrders[f]->Orders->PrevCount; h++)
							{
                                                                item = SeatOrders[f]->Orders->Items[h];
                                                                item->ReturnToAvailability();
                                                                delete item;
                                                                item = NULL;
							}
						}
						__finally
						{
							SeatOrders[f]->Orders->Clear();
							SeatOrders[f]->Orders->ClearPrev();
							SeatOrders[f]->Orders->AppliedMembership.Clear();
						}
					}
					DBTransaction.StartTransaction();
					TDBTab::ReleaseTab(DBTransaction, TDeviceRealTerminal::Instance().ID.Name, 0);
					DBTransaction.Commit();
					ChitNumber.Clear();
					CurrentTender = 0;
					tbtnTender->Caption = "Tender";
					tbtnCashSale->Caption = "Cash Sale";
                    DisplayRoomNoUI();
				}
			}
			else
			{
				ChitNumber.Clear();
				DBTransaction.StartTransaction();
				TDBTab::ReleaseTab(DBTransaction, TDeviceRealTerminal::Instance().ID.Name, 0);
				DBTransaction.Commit();
				CurrentTender = 0;
				tbtnTender->Caption = "Tender";
				tbtnCashSale->Caption = "Cash Sale";
			}
		}
       AutoLogOut();

        //MM-1647: Ask for chit if it is enabled for every order.
        NagUserToSelectChit();
	}

    RedrawItems();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tgridSeatsMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
    try
    {
        for (int i = 0; i < tgridSeats->ColCount; i++)
        {            
            tgridSeats->Buttons[0][i]->Latched = false;
        }

         GridButton->Latched = true;

        int CurrentSeat = tgridSeats->Col(GridButton) + 1; // Zero Indexed.
        if (CurrentSeat == SelectedSeat)
        {
            std::auto_ptr<TfrmTouchKeyboard>frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
            frmTouchKeyboard->MaxLength = 32;
            frmTouchKeyboard->AllowCarriageReturn = false;
            frmTouchKeyboard->StartWithShiftDown = true;
            frmTouchKeyboard->KeyboardText =SeatOrders[CurrentSeat]->SeatName;    
            frmTouchKeyboard->Caption = "Enter a name for this " + TGlobalSettings::Instance().SeatLabel + ".";
            if (frmTouchKeyboard->ShowModal() == mrOk)
            {
                if (frmTouchKeyboard->KeyboardText == "")
                {
                    SeatOrders[CurrentSeat]->SeatName = "";
                    GridButton->Caption = TGlobalSettings::Instance().SeatLabel + " " + IntToStr(CurrentSeat);
                    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
                    DBTransaction.StartTransaction();
                    TDBTables::SetSeatName(DBTransaction, frmTouchKeyboard->KeyboardText, SelectedTable, CurrentSeat);
                    DBTransaction.Commit();
                }
                else
                {
                    SeatOrders[SelectedSeat]->SeatName = frmTouchKeyboard->KeyboardText;
                    GridButton->Caption = SeatOrders[SelectedSeat]->SeatName;
                    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
                    DBTransaction.StartTransaction();
                    TDBTables::SetSeatName(DBTransaction, frmTouchKeyboard->KeyboardText, SelectedTable, CurrentSeat);
                    DBTransaction.Commit();
                }
            }
        }
        else
        {
            if(Sender == NULL)
            {
               GetItemsFromTable(tgridSeats->Col(GridButton), GridButton, false);
            }
            else
            {
               GetItemsFromTable(tgridSeats->Col(GridButton), GridButton, true);
            }
        }
    }
    catch(Exception & E)
    {
         TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tbtnSelectTableMouseClick(TObject *Sender)
{
  try
  {
    int delayedTabKey = 0;
	if (SelectedTable == 0)
	{
		if (CurrentTender != 0)
		{
			MessageBox("You must clear the tender amount before saving orders.", "Error", MB_OK + MB_ICONERROR);
			return;
		}
		showTablePicker();
	}
	else
	{
        bool OrderConfimOk = true;
		if (!OrdersPending())
		{

            TfrmBillGroup* frmBillGroup  = new  TfrmBillGroup(this, TDeviceRealTerminal::Instance().DBControl);
			frmBillGroup->CurrentTable = SelectedTable;
			frmBillGroup->CurrentDisplayMode = eTables;

            Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
			DBTransaction.StartTransaction();

            //Get the patrons for the current selected table, verify if it is already keyed in
            std::vector<TPatronType> selectedTablePatrons = TDBTables::GetPatronCount(DBTransaction, frmBillGroup->CurrentTable);
            int patronCount = GetCount(selectedTablePatrons);

            if(TGlobalSettings::Instance().PromptForPatronCount && patronCount > 0)
            {
                frmBillGroup->PatronTypes = selectedTablePatrons;
            }

            DBTransaction.Commit();

			frmBillGroup->ShowModal();
			setPatronCount( frmBillGroup->PatronCount );

			SelectedTable = 0;
			SelectedTabContainerName = "";
			SelectedSeat = 0;

            //MM-1647: Clear Chit..
            ChitNumber.Clear();

			DBTransaction.StartTransaction();

			TDBTab::ReleaseTab(DBTransaction, TDeviceRealTerminal::Instance().ID.Name, 0);

              //MM-1649: Clear out all the table patrons since we have cancelled the table selection...
            if(TGlobalSettings::Instance().PromptForPatronCount && patronCount > 0 && TDBTables::IsEmpty(DBTransaction, frmBillGroup->CurrentTable))
            {
                TDBTables::ClearPatronCount(DBTransaction, frmBillGroup->CurrentTable);
            }
            delete frmBillGroup;
            frmBillGroup = NULL;
            DBTransaction.Commit();

			lbDisplay->Clear();

            unsigned __int32 maxSeatCount = getMaxSeatCount();

			for( unsigned __int32 i = 1; i <= maxSeatCount; i++)
			{
				while (SeatOrders[i]->Orders->Count != 0)
				{
					TItemComplete *Item = SeatOrders[i]->Orders->Items[0];
					SeatOrders[i]->Orders->Remove(Item);
                    SeatOrders[i]->RoomNumber = "";
					delete Item;
                    Item = NULL;

				}
				while (SeatOrders[i]->Orders->PrevCount != 0)
				{
					delete SeatOrders[i]->Orders->PrevItems[0];
					SeatOrders[i]->Orders->DeletePrev(0);
				}
				SeatOrders[i]->Orders->AppliedMembership.Clear();
				SeatOrders[i]->Orders->Clear();
				SeatOrders[i]->Orders->ClearPrev();
			}

		}
		else
		{
			Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
			TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
			DBTransaction.StartTransaction();

            SelectedParty = SetPartyNameOnChitSettings(DBTransaction, SelectedParty, SelectedTable);

			if (SeatOrders[0]->Orders->Count != 0)
			{
                if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot &&
                                TGlobalSettings::Instance().EnableCustomerJourney)
                {
                    SeatOrders[1]->isDefaultGuest = true;
                }
				// Check for orders on Seat 0 and move them to selected seat.
				tgridSeatsMouseClick(NULL, TMouseButton() << mbLeft, TShiftState(), tgridSeats->Buttons[0][0]);
			}

 			if(TGlobalSettings::Instance().CaptureCustomerName)
              {
                    TCustNameAndOrderType* CustNameAndOrderType = TCustNameAndOrderType::Instance();
                    if(!CustNameAndOrderType->IsNameCaught)
                    {
                        CustNameAndOrderType->CatchCustNameAndOrderType(DisplayNameKeypad(), DisplayCustomerTypeReasons(), SeatOrders);
                    }
              }

			TSaveOrdersTo OrderContainer;

			OrderContainer.Location["TabKey"] = 0;
			OrderContainer.Location["TMMTabType"] = TabTableSeat;
			OrderContainer.Location["TMMDisplayMode"] = eTables;
			OrderContainer.Location["ContainerName"] = SelectedTabContainerName;
			OrderContainer.Location["TabName"] = "";
			OrderContainer.Location["PartyName"] = SelectedParty;
			OrderContainer.Location["SelectedSeat"] = 0;
			OrderContainer.Location["SelectedTable"] = SelectedTable;
			OrderContainer.Location["RoomNumber"] = 0;
			OrderContainer.AllowPartyNameChanged = true;


			if (!TGlobalSettings::Instance().DisableConfirmationOnSave)
			{
				OrderContainer.Values.push_back(TSaveOrdersTo::StringValuePair("Total", InitialMoney.GrandTotal));
				Currency Balance = TDBTables::GetTableBalance(DBTransaction, SelectedTable);
				OrderContainer.Values.push_back(TSaveOrdersTo::StringValuePair("Balance", Balance));
				OrderContainer.Values.push_back(TSaveOrdersTo::StringValuePair("New Balance", Balance + InitialMoney.GrandTotal));

                TfrmConfirmOrder* frmConfirmOrder = new TfrmConfirmOrder(this, OrderContainer);
				if (!TGlobalSettings::Instance().DisableReceiptOnConfirmation)
				{
					SetReceiptPreview(DBTransaction, frmConfirmOrder->ReceiptDisplay, OrderContainer.Location["TMMTabType"], OrderContainer.Location["ContainerName"],
					OrderContainer.Location["TabName"], OrderContainer.Location["PartyName"], OrderContainer.Location["TabKey"], OrderContainer.Location["SelectedTable"],
					OrderContainer.Location["SelectedSeat"], OrderContainer.Location["RoomNumber"]);
				}

				if (frmConfirmOrder->ShowModal() != mrOk)
				{
					OrderConfimOk = false;
					if (TGlobalSettings::Instance().CaptureCustomerName)
					{
                        TCustNameAndOrderType::Instance()->ClearNameCaught();
                    }
				}
                else
                {
                   if(TGlobalSettings::Instance().IsThorlinkSelected)
                    {
                       RemoveMembership(DBTransaction);
                    }
                   SelectedParty = SetPartyNameOnChitSettings(DBTransaction, SelectedParty, SelectedTable, true);

                   if(OrderContainer.Location["PrintPreLimReceipt"])
                   {
                        TMMContactInfo TempUserInfo;
                        TempUserInfo = TDeviceRealTerminal::Instance().User;
                        std::auto_ptr<TContactStaff>Staff(new TContactStaff(DBTransaction));
                        bool allowed = Staff->TestAccessLevel(TempUserInfo,CheckPaymentAccess);
                        if(allowed && TGlobalSettings::Instance().TransferTableOnPrintPrelim)
                        {
                            SouceTableForDelayedPayment =  OrderContainer.Location["ContainerName"];
                            std::auto_ptr<TList>FoodOrdersList(new TList);
                            std::auto_ptr<TList>BevOrdersList(new TList);

                            if(TGlobalSettings::Instance().IsBillSplittedByMenuType)
                            {
                                LoadFoodAndBevList(FoodOrdersList.get(), BevOrdersList.get());
                            }

                            if(TGlobalSettings::Instance().IsBillSplittedByMenuType && !FoodOrdersList->Count && BevOrdersList->Count)
                            {
                                TManagerDelayedPayment::Instance().MoveOrderToTab(DBTransaction,OrderContainer, false);
                            }
                            else
                            {
                                TManagerDelayedPayment::Instance().MoveOrderToTab(DBTransaction,OrderContainer);
                            }
                            delayedTabKey = OrderContainer.Location["TabKey"];
                        }
                        else
                        {
                            TDBTables::SetTableBillingStatus(DBTransaction,OrderContainer.Location["SelectedTable"],ePrelim);
                        }
                   }
                   else
                   {
                     TDBTables::SetTableBillingStatus(DBTransaction,OrderContainer.Location["SelectedTable"],eNoneStatus);
                   }
                }
                delete frmConfirmOrder;
                frmConfirmOrder = NULL;
			}

            int SeatKey  = TDBTables::GetOrCreateSeat(DBTransaction, SelectedTable, SelectedSeat);
            int TabKey = TDBTab::GetOrCreateTab(DBTransaction,TDBTables::GetTabKey(DBTransaction,SeatKey));
            if(delayedTabKey != 0)
            {
                TDBTab::SetDelayedPatronCount(DBTransaction,delayedTabKey,patronsStore);
            }
             // check whether table's guest is linked to clipp tab
            TMMTabType type = TDBTab::GetLinkedTableAndClipTab(DBTransaction, TabKey );

            bool Proceed = GetOrderContainerForTab(DBTransaction, OrderContainer) == mrOk;

			DBTransaction.Commit();

            if(Proceed && TabKey > 0 && type == TabClipp)
            {
                DBTransaction.StartTransaction();
                bool TabisLocked = TDBTab::GetTabLocked(DBTransaction, TabKey);
                AnsiString CreditMessage1 = "Reason :" + TDBTab::GetTabLockedReason(DBTransaction, TabKey);
                DBTransaction.Commit();

                if (TabisLocked)
                {
                    OrderConfimOk = ShowTabLockedMessage(CreditMessage1);
                }
                else
                {
                    DBTransaction.StartTransaction();
                    OrderConfimOk = CheckTabIsInUse(DBTransaction, TabKey);
                    DBTransaction.Commit();
                }
            }

			if (OrderConfimOk)
			{
                if(TGlobalSettings::Instance().IsThorlinkSelected)
                {
                  RemoveMembership(DBTransaction);
                }
				std::auto_ptr<TfrmProcessing>(frmProcessing)(TfrmProcessing::Create<TfrmProcessing>(this));
				frmProcessing->Message = "Posting Orders";
				frmProcessing->Show();

                bool DoProcessOrders = true;

                // if table's guest  is linked to clipp tab
                if (type == TabClipp)
                {
                    DBTransaction.StartTransaction();
                    DoProcessOrders = CheckCreditLimitForTable(DBTransaction, TabKey, InitialMoney.GrandTotal);
                    DBTransaction.Commit();
                }

             	if (DoProcessOrders)
			    {
                    DBTransaction.StartTransaction();

                    // set party name if changed
                    if (OrderContainer.PartyNameChanged)
                    {
                       TDBTables::SetPartyName(DBTransaction, OrderContainer.Location["SelectedTable"], OrderContainer.Location["PartyName"]);
                    }

                    bool PaymentComplete = ProcessOrders(tbtnSave,
                    DBTransaction,
                    OrderContainer.Location["TabKey"], // Tab
                    OrderContainer.Location["TMMTabType"], // Tab Type
                    OrderContainer.Location["ContainerName"],
                    OrderContainer.Location["TabName"],
                    OrderContainer.Location["PartyName"],
                    OrderContainer.Location["PrintPreLimReceipt"],	// Print Prelim Receipt.
                    OrderContainer.Location["SelectedTable"],
                    OrderContainer.Location["SelectedSeat"],
                    OrderContainer.Location["RoomNumber"],
                    OrderContainer.Location["BillOff"],
                    OrderContainer.Location["DelayedInvoiceNumber"]);
                    frmProcessing->Close();

                    TStringList* logList = new TStringList();
                    logList->Clear();

                    if (PaymentComplete)
                    {
                        logList->Add("Trabsaction commit of tbtnSelectTable(().");
                        TSaveLogs::RecordFiscalLogs(logList);

                        DBTransaction.Commit();
                        ResetPOS();

                    }
                    else
                    {
                        logList->Add("Payment not completed for tbtnSelectTable(().");
                        TSaveLogs::RecordFiscalLogs(logList);
                    }
                    if(type == TabClipp)
                    {
                        //send clipp tab details
                        TManagerClippIntegration* sendClippTabKey = TManagerClippIntegration::Instance();
                        sendClippTabKey->SendTabDetails(TabKey);
                    }
                    delete logList;
                    logList = NULL;

                }

				if (!OrdersPending() && IsChitPromptFormActive) // All Orders Posted No Exceptions
				{    
					if (OrderContainer.Location["BillOff"])
					{
					    TfrmBillGroup* frmBillGroup  = new  TfrmBillGroup(this, TDeviceRealTerminal::Instance().DBControl);
                        frmBillGroup->ResetForm();
						frmBillGroup->CurrentDisplayMode = OrderContainer.Location["TMMDisplayMode"];
						frmBillGroup->CurrentTabType = OrderContainer.Location["TMMTabType"];
						frmBillGroup->CurrentSelectedTab = OrderContainer.Location["TabKey"];
						frmBillGroup->CurrentTable = OrderContainer.Location["SelectedTable"];

                        DBTransaction.StartTransaction();

                        //Get the patrons for the current selected table, verify if it is already keyed in
                        std::vector<TPatronType> selectedTablePatrons = TDBTables::GetPatronCount(DBTransaction, frmBillGroup->CurrentTable);
                        int patronCount = GetCount(selectedTablePatrons);

                        if(TGlobalSettings::Instance().PromptForPatronCount && patronCount > 0)
                        {
                            frmBillGroup->PatronTypes = selectedTablePatrons;
                        }

						frmBillGroup->ResetSelection();

                        TDBTables::GetTabKeysWithOrders(DBTransaction, OrderContainer.Location["SelectedTable"], &frmBillGroup->SelectedTabs);
						DBTransaction.Commit();

                        frmBillGroup->ShowModal();

                        //MM-1647: Clear Chit..
                        ChitNumber.Clear();

						setPatronCount( frmBillGroup->PatronCount );

                        //All Ok Clear the table patron counts
                        //MM-1649: Clear patron count if it is enabled..
                        DBTransaction.StartTransaction();
                        if(TGlobalSettings::Instance().PromptForPatronCount && patronCount > 0 && TDBTables::IsEmpty(DBTransaction, frmBillGroup->CurrentTable))
                        {
                            TDBTables::ClearPatronCount(DBTransaction, frmBillGroup->CurrentTable);
                        }
                        delete frmBillGroup;
                        frmBillGroup = NULL;
                        DBTransaction.Commit();

					}
				}

			}
			else
			{
				SelectedSeat = 0;
                if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot &&
                                TGlobalSettings::Instance().EnableCustomerJourney)
                {
                    SeatOrders[1]->isDefaultGuest = true;
                }
				tgridSeatsMouseClick(NULL, TMouseButton() << mbLeft, TShiftState(), tgridSeats->Buttons[0][0]);
			}

		}
		RefreshSeats();

        AutoLogOut();

        //MM-2534: If the user has cancelled to save the table then he should not be prompted to select the table.
        if(TGlobalSettings::Instance().EnableTableDisplayMode && OrderConfimOk)
		{
			showTablePicker();
		}

        if(OrderConfimOk && !(lbDisplay->ItemIndex == -1 && lbDisplay->Count >0) )
            DisplayRoomNoUI();
		//MM-1647: Ask for chit if it is enabled for every order.
		NagUserToSelectChit();

	}
	RedrawItems();
  }
  catch(Exception & E)
  {
	 MessageBox(E.Message, "Error", MB_OK + MB_ICONERROR);
	 TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
  }
}
// ---------------------------------------------------------------------------
int TfrmSelectDish::GetCount(std::vector<TPatronType> patronTypes)
{
    int patronCount = 0;
    try
    {
        if(!patronTypes.empty())
        {
            for(std::vector<TPatronType>::iterator it = patronTypes.begin(); it != patronTypes.end(); ++it)
            {
                (patronCount) += it->Count;
            }
        }
    }
    catch(Exception & E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
    return patronCount;
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::InitializeTablePatrons()
{
    try
    {
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
        DBTransaction.StartTransaction();

        //Get the patrons for the current selected table, verify if it is already keyed in
        std::vector<TPatronType> selectedTablePatrons = TDBTables::GetPatronCount(DBTransaction, SelectedTable);
        int patronCount = GetCount(selectedTablePatrons);

        //Verify if the patrons have already not been assigned, if selected table is a valid table and the setting for patron count is enabled
        if(patronCount <= 0 && SelectedTable > 0 && (TGlobalSettings::Instance().PromptForPatronCount || TGlobalSettings::Instance().PromptPatronCountOnTableSales))
        {
            if (TManagerPatron::Instance().GetCount(DBTransaction) > 0)
            {
                std::vector<TPatronType> patrons = GetPatronCount(DBTransaction);

                //Set to table patron count table...
                if(ArePatronsChanged(selectedTablePatrons,patrons))
                {
                    //call for restructure
                    RestructureBillForPatrons(patrons);
                }
                TDBTables::SetPatronCount(DBTransaction, SelectedTable, patrons);

                if(!patrons.empty())
                {
                    patronCount = GetCount(patrons);
                    setPatronCount(patronCount);
                }
            }
            else
            {
                MessageBox("There are no Patron Types Configured.", "Patron Error.", MB_OK + MB_ICONERROR);
            }

            DBTransaction.Commit();
        }
    }
    catch(Exception & E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }

}
// ---------------------------------------------------------------------------
void TfrmSelectDish::ResetPOS()
{
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	TDBTab::ReleaseTab(DBTransaction, TDeviceRealTerminal::Instance().ID.Name);
	DBTransaction.Commit();

	CloseSidePanel();
	SelectedTable = 0;
	SelectedTabContainerName = "";
	SelectedParty = "";
	SelectedSeat = 0;
	LastSale = 0;
    TypeOfSale = RegularSale;
	RefreshSeats();
	setPatronCount( 1 );
	SetSelectedSeat();
	btnRemove->Enabled = false;
	CurrentTimeKey = 0;

	if (!OrderOnHold)
	{
		ChitNumber.Clear();
	}
	tbtnUserName->Caption = TDeviceRealTerminal::Instance().User.Name;
	tbtnMembership->Caption = "Membership";
	WaitingForSwipe = false;
	Panel1->Enabled = true;
	tbtnTender->Enabled = true;
	tbtnSave->Enabled = true;
	tbtnCashSale->Enabled = true;
	lbDisplay->Clear();
	tbtnTender->Visible = true;
	tbtnSave->Visible = true;
	if (TDeviceRealTerminal::Instance().PaymentSystem->ForceTender)
	{
		tbtnCashSale->Caption = "Cash Sale Disabled";
		tbtnCashSale->Enabled = false;
	}
	else
	{
		tbtnCashSale->Caption = "Cash Sale";
		tbtnCashSale->Enabled = true;
	}
	tbtnSelectTable->Visible = TGlobalSettings::Instance().TablesEnabled;
	tbtnDollar1->Visible = true;
	tbtnDollar2->Visible = true;
	tbtnDollar3->Visible = true;
	tbtnDollar4->Visible = true;
	tbtnDollar5->Visible = true;
	CurrentTender = 0;
	tbtnTender->Caption = "Tender";
    InitializeQuickPaymentOptions();
	UpdateExternalDevices();

	if (TGlobalSettings::Instance().RevertToFirstCourse)
	{
		if (tgridOrderCourse->RowCount > 0)
		{
			tgridOrderCourse->SetVisibleRow(0);
			tgridOrderCourse->Buttons[0][0]->Latched = true;
			tgridOrderCourseMouseClick(tgridOrderCourse, mbLeft, TShiftState(), tgridOrderCourse->Buttons[0][0]);
		}
	}

    TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->ResetPoints();
    Membership.Clear();
	if (TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->CardOk)
	{
        // Restore Membership, Reminds the user to remove the smart card.
		OnSmartCardInserted(NULL);
	}
	else
	{
		OnSmartCardRemoved(NULL);
	}

	// Triggering a Members card change above recalculates the bill total.
	// This would reset the last total and change to zero. This call puts it back.
	TransactionComplete(NULL);
	ReloadChitNumberStatistics();
    CheckMandatoryMembershipCardSetting(tbtnMembership);
    if (TDeviceRealTerminal::Instance().PaymentSystem->ForceTender) //disable cash button
      {
         tbtnCashSale->Caption = "Cash Sale Disabled";
         tbtnCashSale->Enabled = false;
      }
      CustPhone = "";
      CustName = "";
      CustAddress = "";

  RefreshMenu();
  InitializeChit(); // initialize default chit...
  IsAutoLogOutInSelectDish = true;
  TGlobalSettings::Instance().DiningBal = 0;
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::InitializeQuickPaymentOptions()
{
    bool enableQuickPayment = !TGlobalSettings::Instance().EnableWaiterStation && !IsWaiterLogged && !TGlobalSettings::Instance().PointOnly && !TDeviceRealTerminal::Instance().PaymentSystem->ForceTender;
    bool enableCashSale = !TGlobalSettings::Instance().PointOnly && !TGlobalSettings::Instance().EnableWaiterStation  &&  !IsWaiterLogged
                           && !TDeviceRealTerminal::Instance().PaymentSystem->ForceTender;
    tbtnCashSale->Enabled = enableCashSale;
    tbtnTender->Enabled =  (!TGlobalSettings::Instance().EnableWaiterStation && !IsWaiterLogged);
    tbtnDollar1->Enabled = enableQuickPayment;
    tbtnDollar2->Enabled = enableQuickPayment;
    tbtnDollar3->Enabled = enableQuickPayment;
    tbtnDollar4->Enabled = enableQuickPayment;
    tbtnDollar5->Enabled = enableQuickPayment;

}
// ---------------------------------------------------------------------------
void TfrmSelectDish::ReloadChitNumberStatistics()
{
    // re-load chit option statistics if hold and send is enabled
	if(TGlobalSettings::Instance().EnableHoldSend)
	{
		Database::TDBTransaction transaction(TDeviceRealTerminal::Instance().DBControl);
		TManagerChitNumber::Instance().ReLoadChitStatistics(transaction);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::memNoteClick(TObject *Sender)
{
	if (lbDisplay->ItemIndex > -1)
	{
		TItemRedirector *ListItem = (TItemRedirector*)lbDisplay->Items->Objects[lbDisplay->ItemIndex];
		if ((ListItem->ItemType.Contains(itNormalItem) || ListItem->ItemType.Contains(itSetMenuMasterItem)) && !ListItem->ItemType.Contains(itPrevItem))
		{
			TItemComplete * Item = (TItemComplete*)ListItem->ItemObject;
			if (Item)
			{
				std::auto_ptr<TfrmTouchKeyboard>frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
				frmTouchKeyboard->MaxLength = 0;
				frmTouchKeyboard->AllowCarriageReturn = true;
				frmTouchKeyboard->CloseOnDoubleCarriageReturn = true;
				frmTouchKeyboard->KeyboardText = Item->Note;
				frmTouchKeyboard->Caption = "Enter A Note";
				if (frmTouchKeyboard->ShowModal() == mrOk)
				{
					Item->Note = frmTouchKeyboard->memText->Text;
					memNote->Lines->Clear();
					memNote->Lines->Add(Item->Note);
					SetSelectedItem(Item);
				}
			}
		}
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::ForceHappyHourClick()
{
   int happyHourProfileKey=0;

   if (UserForceHappyHourRight)
	{


        if (TGlobalSettings::Instance().ForceHappyHour)
     	{
	    	TGlobalSettings::Instance().ForceHappyHour = false;
            stHappyHour->Tag= 1;
	    }
	   else
	    {
           if (TGlobalSettings::Instance().HappyHourExempt)return;


            std::vector<THappyHour> HappyHourProfiles;
            //get the available profiles  in list
            TManagerHappyHour *loadProfiles= new TManagerHappyHour();
            HappyHourProfiles = loadProfiles->LoadHappyHourProfiles(false);

            if (HappyHourProfiles.size()==1)
            {
               	TGlobalSettings::Instance().ForceHappyHour = true;
                happyHourProfileKey = HappyHourProfiles[0].ProfileKey;
            }
            else
            {
                std::auto_ptr<TfrmMessage> frmMessage(TfrmMessage::Create<TfrmMessage>(this, TDeviceRealTerminal::Instance().DBControl));
                frmMessage->MessageType = eHHProfile;
                //frmMessage->ShowKeyboard =false;

                if (frmMessage->ShowModal() == mrOk)
                {
                    TGlobalSettings::Instance().ForceHappyHour = true;
                    happyHourProfileKey = frmMessage->Key;
                }

            }

          //loading the profiles name in combo box
            for(int profile = 0; profile < HappyHourProfiles.size();profile++)
            {

              if(HappyHourProfiles[profile].ProfileKey == happyHourProfileKey)
              {
                    stHappyHour->Tag= HappyHourProfiles[profile].Pricelevel;

                    TGlobalSettings::Instance().HappyHourMembersOnly =  false;
                    TGlobalSettings::Instance().NoPtsEarnedDuringHappyHour =  false;
                    for(std::vector<int>::iterator hhSettingKey = HappyHourProfiles[profile].HappyHourProfileSettings.begin(); hhSettingKey !=  HappyHourProfiles[profile].HappyHourProfileSettings.end(); hhSettingKey++)
                    {
                       int settingVal = *hhSettingKey;
                        switch(settingVal)
                        {

                            case 1:
                                TGlobalSettings::Instance().HappyHourMembersOnly = true;
                                break;
                            case 2:
                                TGlobalSettings::Instance().NoPtsEarnedDuringHappyHour = true;
                                break;
                        }
                    }

              }

            }



     	}
    }
    else
    {
    	MessageBox("This user does not have permission to force  " "Happy Hour", "Permission Denied", MB_OK | MB_ICONERROR);
    }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tbtnFlashReportsClick()
{
	try
	{
		std::auto_ptr<TfrmReports>frmReports(TfrmReports::Create<TfrmReports>(this));
		std::auto_ptr<TfrmVerticalSelect>SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));
		frmReports->ShowModal();

		int ReportNumber = frmReports->GetBtnSelection();
		switch(ReportNumber)
		{
		case 1:
			{ // Stock Report.
				std::auto_ptr<TfrmReportDisplayNav>frmReportDisplay(TfrmReportDisplayNav::Create<TfrmReportDisplayNav>(this));
				SelectionForm->Items.clear();

				if (TDeviceRealTerminal::Instance().Menus->VisibleMenu != NULL)
				{

					TVerticalSelection Item;
					Item.Title = "Cancel";
					Item.Properties["Color"] = IntToStr(clMaroon);
					Item.CloseSelection = true;
					SelectionForm->Items.push_back(Item);

					for (int j = 0; j < TDeviceRealTerminal::Instance().Menus->VisibleMenu->Count; j++)
					{
						TListCourse *Course = TDeviceRealTerminal::Instance().Menus->VisibleMenu->CourseGet(j);
						if (Course != NULL)
						{
							Item.Title = Course->Course_Name;
							Item.Properties["Color"] = IntToStr(ButtonColors[BUTTONTYPE_UNSELECTED][ATTRIB_BUTTONCOLOR]);
							Item.Properties["CourseKey"] = Course->Course_Key;
							Item.CloseSelection = true;
							SelectionForm->Items.push_back(Item);
						}
					}

					SelectionForm->ShowModal();
					TVerticalSelection SelectedItem;
					if (SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel")
					{
						std::auto_ptr<TStringList>Report(new TStringList);
						TListCourse *Course = TDeviceRealTerminal::Instance().Menus->VisibleMenu->FetchCourseByKey(SelectedItem.Properties["CourseKey"]);
						Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
						DBTransaction.StartTransaction();
						TDeviceRealTerminal::Instance().ManagerStock->GetCourseStockReport(DBTransaction, Course, Report.get());
						DBTransaction.Commit();
						frmReportDisplay->Navigate(Report.get());
						frmReportDisplay->ShowModal();
					}
				}
			}break;
		case 2:
			{ // Poitns Transaction
				std::auto_ptr<TStringList>Report(new TStringList);
				std::auto_ptr<TfrmReportDisplayNav>frmReportDisplay(TfrmReportDisplayNav::Create<TfrmReportDisplayNav>(this));
				WideString ReportLocation = "";
				Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
				DBTransaction.StartTransaction();
				if(TGlobalSettings::Instance().EnableSeperateEarntPts)
					TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetReportPointsTransactions(DBTransaction, Report.get(), true);
				else
					TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetReportPointsTransactions(DBTransaction, Report.get());
				DBTransaction.Commit();

				frmReportDisplay->Navigate(Report.get());
				frmReportDisplay->ShowModal();
			}break;
		case 3:
			{ // Points Transferred
				std::auto_ptr<TStringList>Report(new TStringList);
				std::auto_ptr<TfrmReportDisplayNav>frmReportDisplay(TfrmReportDisplayNav::Create<TfrmReportDisplayNav>(this));
				WideString ReportLocation = "";
				Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
				DBTransaction.StartTransaction();

                /* TODO 1 -o Michael -c Descrespancy : This is more weridness.
                	one of these report is a "Points Transferred Report" and the other is
                    a "Points Held & Transferred Report".
                    Nither of these report are Invalid based on
                    EnableSeperateEarntPts setting as SeperateEarntPts is accounted for on the smart cards
                    so shold have access to both reports. What is is trying to be achived here?

                    Oh and it's bad codeing practice to use function overloading for a funtion that does
                    two descritly different things, just rename the funtion.
                    */

				if(TGlobalSettings::Instance().EnableSeperateEarntPts)
					TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetReportPointsTransferred(DBTransaction, Report.get(), true);
				else
					TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetReportPointsTransferred(DBTransaction, Report.get());

				DBTransaction.Commit();

				frmReportDisplay->Navigate(Report.get());
				frmReportDisplay->ShowModal();
			}break;
		case 4:
		case 5: // Consumption Report
			{
				std::auto_ptr<TManagerReports>rep_mgr(new TManagerReports((TForm*)this));
				std::auto_ptr<TContactStaff>staff;
				std::auto_ptr<TfrmProcessing>working_dlg(TfrmProcessing::Create<TfrmProcessing>(this));
				TMMContactInfo temp_info;
				Database::TDBTransaction trans(TDeviceRealTerminal::Instance().DBControl);

				working_dlg->Message = "Please Wait...";
				TDeviceRealTerminal::Instance().RegisterTransaction(trans);

				trans.StartTransaction();
				staff.reset(new TContactStaff(trans));
				trans.Commit();

				if (!staff->TestAccessLevel(TDeviceRealTerminal::Instance().User, CheckZeds))
				{
					TLoginSuccess login_result;

					PreUserInfo = TDeviceRealTerminal::Instance().User;
					trans.StartTransaction();
					login_result = staff->Login(this, trans, temp_info, CheckZeds);
					trans.Commit();

					switch(login_result)
					{
					case lsDenied:
						MessageBox("This user does not have permission to print " "consumption reports.", "Permission Denied", MB_OK | MB_ICONERROR);
						break;
					case lsPINIncorrect:
						MessageBox("The login was unsuccessful.", "Login failure", MB_OK | MB_ICONERROR);
						break;
					case lsAccepted:
						working_dlg->Show();
						trans.StartTransaction();
						if (ReportNumber == 4)
							rep_mgr->PrintConsumption(trans);
						else
							rep_mgr->PrintConsumptionByMenu(trans, TDeviceRealTerminal::Instance().Menus->VisibleMenu->MenuName);
						trans.Commit();
						/* Fall through */
					default:
						break;
					}
					TDeviceRealTerminal::Instance().User = PreUserInfo;
				}
				else
				{
					working_dlg->Show();
					trans.StartTransaction();
					if (ReportNumber == 4)
						rep_mgr->PrintConsumption(trans);
					else
						rep_mgr->PrintConsumptionByMenu(trans, TDeviceRealTerminal::Instance().Menus->VisibleMenu->MenuName);
					trans.Commit();
				}
			}break;
		case 6:
			{ // Member Points
				std::auto_ptr<TStringList>Report(new TStringList);
				std::auto_ptr<TfrmReportDisplayNav>frmReportDisplay(TfrmReportDisplayNav::Create<TfrmReportDisplayNav>(this));
				WideString ReportLocation = "";
				Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
				DBTransaction.StartTransaction();
				eMemberSource MemberSource;
				TMMContactInfo TempUserInfo;
				TLoginSuccess Result = TDeviceRealTerminal::Instance().ManagerMembership->GetMember(DBTransaction, TempUserInfo, MemberSource);
				DBTransaction.Commit();
				if (Result == lsAccepted || Result == lsAccountBlocked)
				{
					DBTransaction.StartTransaction();
					TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetReportMemberStart(DBTransaction, TempUserInfo, Report.get());
					TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetReportMemberAudit(DBTransaction, TempUserInfo, Report.get(), TDeviceRealTerminal::Instance().ID.Name);
					TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetReportMemberStop(DBTransaction, TempUserInfo, Report.get());
					DBTransaction.Commit();
					frmReportDisplay->Navigate(Report.get());
					frmReportDisplay->ShowModal();
				}
			}break;
		case 7: // Site Summary
			{
				Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
				DBTransaction.StartTransaction();

				std::auto_ptr<TfrmAnalysis>(frmAnalysis)(TfrmAnalysis::Create<TfrmAnalysis>(this));
				UnicodeString DeviceName = TDeviceRealTerminal::Instance().ID.Name;
				bool PrinterExists;

				if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
				{
					PrinterExists = false;
				}

				std::auto_ptr<TPrintout>Printout(new TPrintout);

				if (!PrinterExists)
				{
					TPrinterPhysical DefaultScreenPrinter;
					DefaultScreenPrinter.NormalCharPerLine = 40;
					DefaultScreenPrinter.BoldCharPerLine = 40;
					Printout->Printer = DefaultScreenPrinter;
				}
				else
				{
					Printout->Printer = TComms::Instance().ReceiptPrinter;
				}

				frmAnalysis->SiteSummaryReport(DBTransaction, Printout.get(), frmAnalysis->GetPrevZedTime(DBTransaction));
				DBTransaction.Commit();

				Printout->PrintFormat->PartialCut();

				std::auto_ptr<TfrmShowPrintout>(frmShowPrintout)(TfrmShowPrintout::Create<TfrmShowPrintout>(this));
				Printout->PrintToStream(frmShowPrintout->CurrentPrintout.get());

				frmShowPrintout->btnCancel->Caption = "Close";
				frmShowPrintout->btnClosePrint->Caption = "Print";
				frmShowPrintout->Execute();

			}break;
		case 8: // Tabs and Tables
			{
				Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
				DBTransaction.StartTransaction();

				std::auto_ptr<TfrmProcessing>(Processing)(TfrmProcessing::Create<TfrmProcessing>(this));
				Processing->Message = "Generating Table Tab Summary ...";
				Processing->Show();

				std::auto_ptr<TManagerReports>(ManagerReports)(new TManagerReports((TForm*)(this)));
				ManagerReports->PrintTableTabSummaryReport(DBTransaction);
				DBTransaction.Commit();
			}break;
		case 9: // Float Adjustments.
			{
				Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
				DBTransaction.StartTransaction();
				std::auto_ptr<TfrmAnalysis>(frmAnalysis)(TfrmAnalysis::Create<TfrmAnalysis>(this));
				UnicodeString DeviceName = TDeviceRealTerminal::Instance().ID.Name;
				frmAnalysis->PrintFloatAdjustments(DBTransaction, DeviceName);
				DBTransaction.Commit();
			}break;
		case 10: // X Report
			{
                TTransactionInfoProcessor::Instance().RemoveEntryFromMap(TDeviceRealTerminal::Instance().ID.Name);
				Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
				DBTransaction.StartTransaction();
                ReportManager reportManager;
                XReport* xReport = reportManager.GetXReport(&TGlobalSettings::Instance(), &DBTransaction);
                xReport->DisplayAndPrint();
				DBTransaction.Commit();
			}break;
		case 11: // E-Journal
			{
                std::auto_ptr<TManagerEJournal> managerEJournal(new TManagerEJournal());
                managerEJournal->TriggerEJournal();
			}break;
		case 12: // Consolidated Zed
			{
                std::auto_ptr<TManagerEJournal> managerEJournal(new TManagerEJournal());
                managerEJournal->TriggerEJournal(true);
			}break;
        case 13: // Havana Report
			{
                std::auto_ptr<TManagerReportExport> managerReportExport(new TManagerReportExport());
                managerReportExport->ExportReport(1);
			}break;
        case 14: // Genrating Room Payment Summary
			{

				Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
				DBTransaction.StartTransaction();

				std::auto_ptr<TfrmProcessing>(Processing)(TfrmProcessing::Create<TfrmProcessing>(this));
				Processing->Message = "Genrating Room Payment Summary ...";
				Processing->Show();

				std::auto_ptr<TManagerReports>(ManagerReports)(new TManagerReports((TForm*)(this)));
				ManagerReports->PrintPMSRoomPaymentReport(DBTransaction);
				DBTransaction.Commit();
			}break;
		}
	}
	catch(Exception & E)
	{
		MessageBox(E.Message, "Error", MB_OK + MB_ICONERROR);
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
}
// ---------------------------------------------------------------------------
TModalResult TfrmSelectDish::GetOrderContainer(Database::TDBTransaction &DBTransaction, TSaveOrdersTo &OrderContainer)
{
	TModalResult Retval = mrOk;
	try
	{
        //mm2036
        if(ChitNumber.IsAutoSaveOnTab)
        {
            Retval = setupAutoSaveTabContainer( DBTransaction, OrderContainer );
        }
        else
        {
            std::auto_ptr<TfrmSelectSaveOption>SelectionForm(TfrmSelectSaveOption::Create<TfrmSelectSaveOption>(this));
            do
            {
                 // Display Reports List
                Retval = mrOk;
                bool isSCDApplied = false;
                if (SelectionForm->ShowModal() != mrCancel)
                {
                    if(SelectionForm->SelectedTabType ==13)
                    {
                        TSCDPWDChecker SCDChecker;
                         std::auto_ptr<TList> allOrders(new TList());
                           GetAllOrders(allOrders.get());

                        for(int x = (SeatOrders[0]->Orders->Count - 1) ; x >= 0; x--)
                        {
                            TItemMinorComplete *order = reinterpret_cast<TItemMinorComplete *>(SeatOrders[0]->Orders->Items[x]);
                            for(std::vector<TDiscount>::iterator it = order->Discounts.begin(); it != order->Discounts.end(); ++it)
                            {
                                TDiscount CurrentDiscount;
                                CurrentDiscount.DiscountKey =  it->DiscountKey;
                                 ManagerDiscount->GetDiscount(DBTransaction, CurrentDiscount.DiscountKey, CurrentDiscount);

                                  if((SCDChecker.SeniorCitizensCheck(CurrentDiscount, allOrders.get(), true))
                                       && (SCDChecker.PWDCheck(CurrentDiscount, allOrders.get(), true)))
                                  {
                                      isSCDApplied = true;
                                  }
                            }
                        }
                     }

                     if(isSCDApplied)
                     {
                        MessageBox("Order with SCD Discount can't be saved to clipp Tab.", "Error", MB_OK + MB_ICONERROR);
                        Retval = mrAbort;
                     }
                     else
                     {
	                    OrderContainer.Location["TMMDisplayMode"] = SelectionForm->DisplayMode;
	                    OrderContainer.Location["TMMTabType"] = SelectionForm->SelectedTabType;

                        if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot &&
                            TGlobalSettings::Instance().EnableCustomerJourney && SeatOrders[0]->Orders->Count)
                        {
                                UnicodeString AccNumber = "";
                                int TabNo;
                                TItemComplete *order = reinterpret_cast<TItemComplete *>(SeatOrders[0]->Orders->Items[0]);
                                AccNumber = order->AccNo;
                                OrderContainer.Location["AccNo"] = AccNumber;
                        }

	                    switch(int(OrderContainer.Location["TMMTabType"]))
	                    {
	                        case TabDelayedPayment:
	                        case TabNormal:
	                        case TabStaff:
	                        case TabClipp:
	                        {
	                            bool allowed = false;
	                            OrderContainer.Location["SelectedTable"] = 0;
	                            OrderContainer.Location["SelectedSeat"] = 0;
	                            OrderContainer.Location["RoomNumber"] = 0;
	                            if(OrderContainer.Location["TMMTabType"] == TabDelayedPayment)
	                              {
	                                TMMContactInfo TempUserInfo;
	                                std::auto_ptr<TContactStaff>Staff(new TContactStaff(DBTransaction));
	                                TLoginSuccess Result = Staff->Login(this, DBTransaction, TempUserInfo, CheckPaymentAccess);
	                                if (Result == lsAccepted)
	                                {
	                                  allowed = true;
	                                }
	                                else if (Result == lsDenied)
	                                {
	                                        MessageBox("You do not have access rights to the Delayed Payment Tabs.", "Error", MB_OK + MB_ICONERROR);
	                                }
	                                else if (Result == lsPINIncorrect)
	                                {
	                                        MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	                                }
	                              }
	                             else
	                             {
	                                allowed = true;
	                             }

	                            if(allowed)
	                            {
	                               if (GetTabContainer(DBTransaction, TMMTabType(OrderContainer.Location["TMMTabType"]), OrderContainer) != mrOk)
	                               {
	                                 Retval = mrAbort;
	                               }
	                            }
	                            else
	                            {
	                                Retval = mrAbort;
	                            }
	                        }break;
	                    case TabMember:
	                        {   if(!TGlobalSettings::Instance().IsThorlinkSelected)
	                            {
	                            if (TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Enabled"])
	                            {
	                                TMMContactInfo TempUserInfo;
	                                if (SeatOrders[0]->Orders->AppliedMembership.ContactKey != 0)
	                                {
	                                    TempUserInfo = SeatOrders[0]->Orders->AppliedMembership;
	                                }
	                                else
	                                {

	                                    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	                                    TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	                                    DBTransaction.StartTransaction();
	                                    eMemberSource MemberSource;
	                                    TLoginSuccess Result = TDeviceRealTerminal::Instance().ManagerMembership->GetMember(DBTransaction, TempUserInfo, MemberSource);
	                                    DBTransaction.Commit();
	                                    if (Result == lsAccountBlocked)
	                                    {
	                                        MessageBox("Account Blocked " + TempUserInfo.Name + " " + TempUserInfo.AccountInfo, "Account Blocked", MB_OK + MB_ICONINFORMATION);
	                                        Retval = mrAbort;
	                                    }
	                                    if (Result == lsCancel)
	                                    {
	                                        Retval = mrAbort;
	                                    }

	                                }

	                                if (Retval != mrAbort)
	                                {
	                                    OrderContainer.Location["MemberKey"] = TempUserInfo.ContactKey;
	                                    int TabKey = TDBTab::GetTabByOwner(DBTransaction, TempUserInfo.ContactKey);
	                                    SubsidizedDiscountApply(TabKey);
	                                    if (TabKey != 0)
	                                    {
	                                        OrderContainer.Location["TabKey"] = TabKey;
	                                        OrderContainer.Location["ContainerName"] = TempUserInfo.Name;
	                                        OrderContainer.Location["TabName"] = TempUserInfo.Name;
	                                        OrderContainer.Location["SelectedTable"] = 0;
	                                        OrderContainer.Location["SelectedSeat"] = 0;
	                                        OrderContainer.Location["RoomNumber"] = 0;
	                                    }
	                                    else
	                                    {
	                                        if (TempUserInfo.TabEnabled && TabKey == 0 && TempUserInfo.ContactKey != 0)
	                                        {
	                                            TabKey = TDBTab::GetOrCreateTab(DBTransaction, 0);
	                                            TDBTab::SetTabOwner(DBTransaction, TabKey, TempUserInfo.ContactKey, TabMember);
	                                            TDBTab::SetTabName(DBTransaction, TabKey, TempUserInfo.Name);
	                                            TDBTab::SetTabPermanent(DBTransaction, TabKey, true);
	                                            OrderContainer.Location["TabKey"] = TabKey;
	                                            OrderContainer.Location["ContainerName"] = TempUserInfo.Name;
	                                            OrderContainer.Location["TabName"] = TempUserInfo.Name;
	                                            OrderContainer.Location["SelectedTable"] = 0;
	                                            OrderContainer.Location["SelectedSeat"] = 0;
	                                            OrderContainer.Location["RoomNumber"] = 0;
	                                        }
	                                        else
	                                        {
	                                            MessageBox("No Membership Tab for " + TempUserInfo.Name, "Error", MB_OK + MB_ICONERROR);
	                                            Retval = mrAbort;
	                                        }
	                                    }

	                                    if (Retval != mrAbort)
	                                    {
	                                        if (!TGlobalSettings::Instance().DisableConfirmationOnSave)
	                                        {
	                                            OrderContainer.Values.push_back(TSaveOrdersTo::StringValuePair("Total", InitialMoney.GrandTotal));
	                                            Currency Balance = TDBTab::GetTabBalance(DBTransaction, TabKey);
	                                            OrderContainer.Values.push_back(TSaveOrdersTo::StringValuePair("Balance", Balance));
	                                            OrderContainer.Values.push_back(TSaveOrdersTo::StringValuePair("New Balance", Balance + InitialMoney.GrandTotal));

	                                            TfrmConfirmOrder* frmConfirmOrder = new TfrmConfirmOrder(this, OrderContainer);
	                                            if (!TGlobalSettings::Instance().DisableReceiptOnConfirmation)
	                                            {
	                                                SetReceiptPreview(DBTransaction, frmConfirmOrder->ReceiptDisplay, OrderContainer.Location["TMMTabType"], OrderContainer.Location["ContainerName"],
	                                                    OrderContainer.Location["TabName"], OrderContainer.Location["PartyName"], OrderContainer.Location["TabKey"], OrderContainer.Location["SelectedTable"],
	                                                    OrderContainer.Location["SelectedSeat"], OrderContainer.Location["RoomNumber"]);
	                                            }

	                                            if (frmConfirmOrder->ShowModal() != mrOk)
	                                            {
	                                                Retval = mrAbort;
	                                            }
	                                            delete frmConfirmOrder;
                                                frmConfirmOrder = NULL;
	                                        }
	                                    }
	                                }
	                            }
	                            else
	                            {
	                                MessageBox("Membership is not Enabled.", "Error", MB_OK + MB_ICONERROR);
	                            }
	                        }
	                        else
	                        {
	                          MessageBox("No Membership Tab for Thor.", "Error", MB_OK + MB_ICONERROR);
	                          Retval = mrAbort;
	                        }
	                        }break;
	                    case TabTableSeat:
	                        {

	                            std::auto_ptr<TEnableFloorPlan> floorPlan (new TEnableFloorPlan());
	                            TFloorPlanReturnParams floorPlanReturnParams;

	                            // Runs new web app of floorPlan
                                if(floorPlan->Run( ( TForm* )this, true, floorPlanReturnParams ))
//	                            if( TEnableFloorPlan::Instance()->Run( ( TForm* )this, true, floorPlanReturnParams ) )
	                            {
	                                OrderContainer.Location["TabKey"       ] = 0;
	                                OrderContainer.Location["SelectedTable"] = floorPlanReturnParams.TabContainerNumber;
	                                OrderContainer.Location["ContainerName"] = floorPlanReturnParams.TabContainerName;
	                                OrderContainer.Location["PartyName"    ] = floorPlanReturnParams.PartyName;
	                                OrderContainer.Location["RoomNumber"   ] = 0;
	                                OrderContainer.AllowPartyNameChanged     = true;

	                                if( GetTableContainer( DBTransaction, OrderContainer ) != mrOk )
	                                {
	                                    Retval = mrAbort;
	                                }
                                    floorPlan.reset();
	                            }
	                            else
	                            {
                                    floorPlan.reset();
	                                Retval = mrAbort;
	                            }
	                        }break;
	                    case TabRoom:
	                        {
	                            if (TRooms::Instance().SelectRoom(DBTransaction, eRoomFilterAll) == mrOk)
	                            {
	                                int RoomTabKey = TDBRooms::GetRoomTab(DBTransaction, TRooms::Instance().SelectedRoom->RoomNo);
	                                OrderContainer.Location["TabKey"] = RoomTabKey;
	                                OrderContainer.Location["SelectedTable"] = 0;
	                                OrderContainer.Location["SelectedSeat"] = 0;
	                                OrderContainer.Location["ContainerName"] = TRooms::Instance().SelectedRoom->Name;
	                                OrderContainer.Location["TabName"] = TRooms::Instance().SelectedRoom->Name;
	                                OrderContainer.Location["PartyName"] = TRooms::Instance().SelectedRoom->PartyName;
	                                OrderContainer.Location["RoomNumber"] = TRooms::Instance().SelectedRoom->RoomNo;
	                                OrderContainer.AllowPartyNameChanged = true;

	                                if (!TGlobalSettings::Instance().DisableConfirmationOnSave)
	                                {
	                                    OrderContainer.Values.push_back(TSaveOrdersTo::StringValuePair("Total", InitialMoney.GrandTotal));
	                                    Currency Balance = TDBTab::GetTabBalance(DBTransaction, RoomTabKey);
	                                    OrderContainer.Values.push_back(TSaveOrdersTo::StringValuePair("Balance", Balance));
	                                    OrderContainer.Values.push_back(TSaveOrdersTo::StringValuePair("New Balance", Balance + InitialMoney.GrandTotal));

	                                     TfrmConfirmOrder* frmConfirmOrder = new TfrmConfirmOrder(this, OrderContainer);
	                                    if (!TGlobalSettings::Instance().DisableReceiptOnConfirmation)
	                                    {
	                                        SetReceiptPreview(DBTransaction, frmConfirmOrder->ReceiptDisplay, OrderContainer.Location["TMMTabType"], OrderContainer.Location["ContainerName"],
	                                            OrderContainer.Location["TabName"], OrderContainer.Location["PartyName"], OrderContainer.Location["TabKey"], OrderContainer.Location["SelectedTable"],
	                                            OrderContainer.Location["SelectedSeat"], OrderContainer.Location["RoomNumber"]);
	                                    }

	                                    if (frmConfirmOrder->ShowModal() != mrOk)
	                                    {
	                                        Retval = mrAbort;
	                                    }
	                                    delete frmConfirmOrder;
                                        frmConfirmOrder = NULL;
	                                }
	                            }
	                            else
	                            {
	                                Retval = mrAbort;
	                            }
	                        }break;
	                    case TabInvoice:
	                        {
	                            if(!TGlobalSettings::Instance().IsThorlinkSelected)
	                            {
	                            if (TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Enabled"])
	                            {
	                                TMMContactInfo TempUserInfo;
	                                eMemberSource MemberSource;
	                                std::auto_ptr<TContactStaff>Staff(new TContactStaff(DBTransaction));
	                                TLoginSuccess Result = Staff->Login(this, DBTransaction, TempUserInfo, CheckPayInvoice);
	                                if (Result == lsAccepted)
	                                {
	                                    TempUserInfo.Clear();
	                                    TLoginSuccess Result = TDeviceRealTerminal::Instance().ManagerMembership->GetMember(DBTransaction, TempUserInfo, MemberSource);
	                                    if (Result == lsAccepted)
	                                    {
	                                        if (TempUserInfo.TabEnabled)
	                                        {
	                                            Currency Total = 0;
	                                            Currency TotalExcludingGST = 0;

	                                            for (int i = 0; i < SeatOrders[0]->Orders->Count; i++)
	                                            {
	                                                TItemComplete *Order = SeatOrders[0]->Orders->Items[i];
	                                                Total += Order->TotalPriceAdjustmentSides();
	                                                TotalExcludingGST += Order->TotalPriceAdjustmentSidesExclGST();
	                                            }

	                                            int OwnerKey = TempUserInfo.ContactKey;
	                                            int InvoiceTabKey = TDBTab::GetOrCreateTab(DBTransaction, 0);
	                                            int InvoiceKey = Invoice->CreateInvoice(DBTransaction, OwnerKey, TotalExcludingGST, Total, "");

	                                            TDBTab::SetTabToInvoice(DBTransaction, InvoiceTabKey, InvoiceKey);
	                                            TDBTab::SetTabOwner(DBTransaction, InvoiceTabKey, OwnerKey, TabInvoice);

	                                            OrderContainer.Location["TabKey"] = InvoiceTabKey;
	                                            OrderContainer.Location["SelectedTable"] = 0;
	                                            OrderContainer.Location["SelectedSeat"] = 0;
	                                            OrderContainer.Location["ContainerName"] = Invoice->GetInvoiceNumber(DBTransaction, InvoiceKey);
	                                            OrderContainer.Location["TabName"] = OrderContainer.Location["ContainerName"];
	                                            OrderContainer.Location["PartyName"] = "";
	                                            OrderContainer.Location["RoomNumber"] = 0;
	                                        }
	                                        else
	                                        {
	                                            MessageBox("Credit disabled for this member. \r" + TempUserInfo.Name + " " + TempUserInfo.AccountInfo + "\rSorry No Credit, No Invoices.", "Credit Disabled",
	                                                MB_OK + MB_ICONINFORMATION);
	                                            Retval = mrAbort;
	                                        }
	                                    }
	                                    else if (Result == lsAccountBlocked)
	                                    {
	                                        MessageBox("Account Blocked " + TempUserInfo.Name + " " + TempUserInfo.AccountInfo, "Account Blocked", MB_OK + MB_ICONINFORMATION);
	                                        Retval = mrAbort;
	                                    }
	                                    else
	                                    {
	                                        Retval = mrAbort;
	                                    }
	                                }
	                                else if (Result == lsDenied)
	                                {
	                                    MessageBox("You do not have access rights to the Invoice System.", "Error", MB_OK + MB_ICONERROR);
	                                    Retval = mrAbort;
	                                }
	                                else if (Result == lsPINIncorrect)
	                                {
	                                    MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	                                    Retval = mrAbort;
	                                }
	                            else
	                            {
	                                Retval = mrAbort;
	                            }
							}
	                            else
	                            {
	                                MessageBox("Membership is not Enabled.", "Error", MB_OK + MB_ICONERROR);
	                            }
	                         }
	                        else
	                        {
	                          MessageBox("No Membership Tab for Thor.", "Error", MB_OK + MB_ICONERROR);
	                          Retval = mrAbort;
	                        }
	                        }break;

	                    default:
	                        Retval = mrCancel;
	                    }
	                }
                }
                else
                {
                    Retval = mrCancel;
                }
            }
            while (Retval == mrAbort);
        }
	}
	catch(Exception & E)
	{
		MessageBox(E.Message, "Error", MB_OK + MB_ICONERROR);
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		Retval = mrCancel;
	}
	return Retval;
}
// ---------------------------------------------------------------------------
TModalResult TfrmSelectDish::GetTabContainer(Database::TDBTransaction &DBTransaction, TMMTabType TabType, TSaveOrdersTo &OrderContainer)
{
	TModalResult Retval = mrOk;
	try
	{
		// Display Reports List
		std::auto_ptr<TfrmVerticalSelect>SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

		TVerticalSelection Item;
		Item.Title = "Cancel";
		Item.Properties["Color"] = IntToStr(clMaroon);
		Item.CloseSelection = true;
		SelectionForm->Items.push_back(Item);

		std::auto_ptr<TStringList>TabList(new TStringList);
		switch(TabType)
		{
		    case TabNormal:
			{
				Item.Title = "New Tab";
				Item.Properties["TabKey"] = -1;
				Item.Properties["Color"] = clNavy;
				Item.CloseSelection = true;
				SelectionForm->Items.push_back(Item);

				TDBTab::GetTabs(DBTransaction, TabList.get(), TabType);

				for (int i = 0; i < TabList->Count; i++)
				{
					Item.Title = TabList->Strings[i];
					Item.Properties["TabKey"] = (int)TabList->Objects[i];
					Item.Properties["Color"] = clInfoBk;
					Item.CloseSelection = true;
					SelectionForm->Items.push_back(Item);
				}
			}break;
		    case TabStaff:
			{
				std::auto_ptr<TContactStaff>Staff(new TContactStaff(DBTransaction));
				Staff->GetTabs(DBTransaction, TabList.get());
				for (int i = 0; i < TabList->Count; i++)
				{
					Item.Title = TabList->Strings[i];
					Item.Properties["TabKey"] = (int)TabList->Objects[i];
					Item.Properties["Color"] = clInfoBk;
					Item.CloseSelection = true;
					SelectionForm->Items.push_back(Item);
				}
			}break;
            case TabClipp:
			{
                CurrentTabType = TabClipp;
                TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

                IBInternalQuery->SQL->Text = "SELECT " "TAB.TAB_KEY, " "TAB.TAB_NAME NAME " "FROM " "TAB "
                "INNER JOIN CLIPP_TAB on tab.TAB_KEY = CLIPP_TAB.CLIPP_TAB_TAB_KEY " "WHERE "
                "CLIPP_TAB.IS_TAB_CLOSED = 'F' " /*"AND " "TAB.TAB_KEY NOT IN (SELECT SEAT.TAB_KEY FROM SEAT WHERE SEAT.TAB_KEY IS NOT NULL)" " AND ( TAB.TAB_TYPE = " + IntToStr
                (CurrentTabType) + " OR TAB.TAB_TYPE = " + IntToStr(TabCashAccount) + " ) " +*/ " ORDER BY " "TAB.TAB_KEY DESC";
                IBInternalQuery->ExecQuery();

                for(;!IBInternalQuery->Eof;IBInternalQuery->Next())
				{
					Item.Title = IBInternalQuery->FieldByName("NAME")->AsString;
					Item.Properties["TabKey"] = IBInternalQuery->FieldByName("TAB_KEY")->AsInteger;
					Item.Properties["Color"] = clInfoBk;
					Item.CloseSelection = true;
					SelectionForm->Items.push_back(Item);
				}
			}break;
     		}

                bool isItemSelected = false;
                AnsiString SelectedTabName = "";
                int SelectedTabKey = -1;
                TVerticalSelection SelectedItem;
                if(TabType == TabDelayedPayment)
                {
                    TfrmDelayedPaymentTabs *frmDelayedPaymentTabs = new TfrmDelayedPaymentTabs(this);
                    if(frmDelayedPaymentTabs->ShowModal() == mrOk)
                    {
                      isItemSelected = true;
                      SelectedTabKey = frmDelayedPaymentTabs->SelectedTabKey;
                      SelectedTabName = frmDelayedPaymentTabs->SelectedTabName;

                      if(!TGlobalSettings::Instance().IsBillSplittedByMenuType ||
                        (SeatOrders[SelectedSeat]->Orders->Items[0]->ItemType && (frmDelayedPaymentTabs->DelayedInvoiceNumber.Pos("L") != 0))
                        || (!SeatOrders[SelectedSeat]->Orders->Items[0]->ItemType && (frmDelayedPaymentTabs->DelayedInvoiceNumber.Pos("L") == 0)))
                        {
                            OrderContainer.Location["DelayedInvoiceNumber"] = frmDelayedPaymentTabs->DelayedInvoiceNumber;
                        }
                        else
                        {
                            MessageBox("You can't save different menu types items together.", "Error", MB_OK + MB_ICONERROR);
					        return mrAbort;
                        }
                    }
                }
                else
                {
                  SelectionForm->ShowModal();
                  isItemSelected =  SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel";
                  if(isItemSelected)
                   {
                     if(CheckIfSubsidizedDiscountValid(SelectedItem.Properties["TabKey"]))
                     {
                        SelectedTabKey =  SelectedItem.Properties["TabKey"];
                        SelectedTabName = SelectedItem.Title;
                     }
                     else
                     {
                        MessageBox("The Tab selected has an invalid type of subsidized profile\n"
                        "Dicounts of value 0 or equal to item price amount are not valid ","Caution",MB_OK + MB_ICONWARNING);
                        isItemSelected = false;
                        Retval = mrAbort;
                     }
                   }
                }

		if (isItemSelected)
		{
			if (SelectedTabKey == -1)
			{
				TMMContactInfo currentUserInfo = TDeviceRealTerminal::Instance().User;
				std::auto_ptr<TContactStaff>Staff(new TContactStaff(DBTransaction));
				if (!Staff->TestAccessLevel(TDeviceRealTerminal::Instance().User, CheckAccountCreation))
				{
					MessageBox("You do not have the privileges to create a new tab!", "Error", MB_OK + MB_ICONERROR);
					return mrAbort;
				}

				std::auto_ptr<TfrmAddTab>frmAddTab(TfrmAddTab::Create(this));
				frmAddTab->LoadDetails(DBTransaction, 0);
				if (frmAddTab->ShowModal() == mrOk)
				{
					if (TDBTab::GetTab(DBTransaction, frmAddTab->TabName, TabNormal) != 0)
					{
						MessageBox("A Tab with this name already exists. Please select a different name.", "Warning", MB_OK + MB_ICONWARNING);
						Retval = mrAbort;
					}
					else
					{
						int TabyKey = TDBTab::GetOrCreateTab(DBTransaction, 0);

						TDBTab::SetTabDetails(DBTransaction, TabyKey, frmAddTab->TabName, frmAddTab->TabIDType, frmAddTab->TabIDNumber, frmAddTab->TabIDExpiry, TabNormal);
						TDBTab::SetTabCard(DBTransaction, TabyKey, frmAddTab->CardString);
						OrderContainer.Location["TabKey"] = TabyKey;
						OrderContainer.Location["ContainerName"] = frmAddTab->TabName;
						OrderContainer.Location["TabName"] = frmAddTab->TabName;

						if (!TGlobalSettings::Instance().DisableConfirmationOnSave)
						{
							OrderContainer.Values.push_back(TSaveOrdersTo::StringValuePair("Total", InitialMoney.GrandTotal));
							Currency Balance = TDBTab::GetTabBalance(DBTransaction, TabyKey);
							OrderContainer.Values.push_back(TSaveOrdersTo::StringValuePair("Balance", Balance));
							OrderContainer.Values.push_back(TSaveOrdersTo::StringValuePair("New Balance", Balance + InitialMoney.GrandTotal));

                            TfrmConfirmOrder* frmConfirmOrder = new TfrmConfirmOrder(this, OrderContainer);
							if (!TGlobalSettings::Instance().DisableReceiptOnConfirmation)
							{
								SetReceiptPreview(DBTransaction, frmConfirmOrder->ReceiptDisplay, OrderContainer.Location["TMMTabType"], OrderContainer.Location["ContainerName"],
                                                                OrderContainer.Location["TabName"], OrderContainer.Location["PartyName"], OrderContainer.Location["TabKey"], OrderContainer.Location["SelectedTable"],
								OrderContainer.Location["SelectedSeat"], OrderContainer.Location["RoomNumber"]);
							}

							if (frmConfirmOrder->ShowModal() != mrOk)
							{
								Retval = mrAbort;
							}
                            delete frmConfirmOrder;
                            frmConfirmOrder = NULL;
						}
					}
				}
				else
				{
					Retval = mrAbort;
				}
			}
			else
			{
    		    SubsidizedDiscountApply(SelectedTabKey) ;
				if(!IsSubSidizeOrderCancil)
				{
					OrderContainer.Location["TabKey"] = SelectedTabKey;
					OrderContainer.Location["ContainerName"] = SelectedTabName;
					OrderContainer.Location["TabName"] = SelectedTabName;
				}
                else
				{
                    Retval = mrAbort;
                    ManagerDiscount->ClearDiscounts(SeatOrders[SelectedSeat]->Orders->List);
                    RedrawSeatOrders();
                    TotalCosts();
                }


				if (!TabPINOk(DBTransaction, SelectedTabKey))
				{
					Retval = mrAbort;
				}

				if (Retval == mrOk)
				{
//                     if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot &&
//                            TGlobalSettings::Instance().EnableCustomerJourney )
//                    {
//                        UnicodeString AccountNumber = TDBTab::GetAccountNumber(DBTransaction, OrderContainer.Location["TabKey"]);
//                        if(AccountNumber != "" && AccountNumber.Compare(OrderContainer.Location["AccNo"]) )
//                        {
//                            MessageBox("Order with different room no can't be saved..", "Error", MB_OK + MB_ICONERROR);
//                            return mrAbort;
//                        }
//                    }

					if (!TGlobalSettings::Instance().DisableConfirmationOnSave)
					{
						OrderContainer.Values.push_back(TSaveOrdersTo::StringValuePair("Total", InitialMoney.GrandTotal));
						Currency Balance = TDBTab::GetTabBalance(DBTransaction, SelectedTabKey);
						OrderContainer.Values.push_back(TSaveOrdersTo::StringValuePair("Balance", Balance));
						OrderContainer.Values.push_back(TSaveOrdersTo::StringValuePair("New Balance", Balance + InitialMoney.GrandTotal));
                        double orderValue = 0;
                        for(int i = 0; i < SeatOrders[SelectedSeat]->Orders->Count; i++)
                        {
                            TItemComplete *item = (TItemComplete*)SeatOrders[SelectedSeat]->Orders->Items[i];
                            item->RunBillCalculator();
                            orderValue += (double)item->FinalPrice_BillCalc();
                            for(int j = 0; j < item->SubOrders->Count; j++)
                            {
                                TItemComplete *subItem = ((TItemComplete*)item->SubOrders->Items[j]);
                                subItem->RunBillCalculator();
                                orderValue += (double)subItem->FinalPrice_BillCalc();
                            }
                        }
                        if(orderValue != 0)
                        {
                            TfrmConfirmOrder* frmConfirmOrder = new TfrmConfirmOrder(this, OrderContainer);
                            if (!TGlobalSettings::Instance().DisableReceiptOnConfirmation)
                            {
                                SetReceiptPreview(DBTransaction, frmConfirmOrder->ReceiptDisplay, OrderContainer.Location["TMMTabType"], OrderContainer.Location["ContainerName"],
                                OrderContainer.Location["TabName"], OrderContainer.Location["PartyName"], OrderContainer.Location["TabKey"], OrderContainer.Location["SelectedTable"],
                                OrderContainer.Location["SelectedSeat"], OrderContainer.Location["RoomNumber"]);
                            }

                            // if clipp tab order is in progress
                            IsOrderinProgress = true;
                            ClippTabKey = SelectedTabKey;

                            if (frmConfirmOrder->ShowModal() != mrOk)
                            {
                                Retval = mrAbort;
                                IsOrderinProgress = false;
                            }
                            else
                            {
                                IsOrderinProgress = false;
                                if(TGlobalSettings::Instance().IsThorlinkSelected)
                                {
                                   RemoveMembership(DBTransaction);
                                }
                            }
                            delete frmConfirmOrder;
                            frmConfirmOrder = NULL;
                        }
					}
				}

			}
		}
		else
		{
			Retval = mrAbort;
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		Retval = mrCancel;
	}
	return Retval;
}
// ---------------------------------------------------------------------------
TModalResult TfrmSelectDish::GetTableContainer(Database::TDBTransaction &DBTransaction, TSaveOrdersTo &OrderContainer)
{
	TModalResult Retval = mrOk;
	try
	{

		if(TGlobalSettings::Instance().CaptureCustomerName)
		{
			TCustNameAndOrderType* CustNameAndOrderType = TCustNameAndOrderType::Instance();
			if(!CustNameAndOrderType->IsNameCaught)
			{
				CustNameAndOrderType->CatchCustNameAndOrderType(DisplayNameKeypad(), DisplayCustomerTypeReasons(), SeatOrders);
			}
		}
        SelectedParty = SetPartyNameOnChitSettings(DBTransaction, SelectedParty, SelectedTable);
		// Display Reports List
		std::auto_ptr<TfrmVerticalSelect>SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

		SelectionForm->Caption = "Table " + IntToStr(static_cast<int>(OrderContainer.Location["SelectedTable"])) + "(" + OrderContainer.Location["ContainerName"] + ")";

		TVerticalSelection Item;
		Item.Title = "Cancel";
		Item.Properties["Color"] = IntToStr(clMaroon);
		Item.CloseSelection = true;
		SelectionForm->Items.push_back(Item);

        __int64 maxSeatCount = getMaxSeatCount();

		for( __int64 i = 1; i <= maxSeatCount; i++ )
		{
			Item.Title = IntToStr(i) + ". " + TGlobalSettings::Instance().SeatLabel;
			Item.Properties["TabName"] = Item.Title;
			Item.Properties["TabKey"] = 0;
			Item.Properties["Color"] = clInfoBk;
			Item.Properties["SelectedSeat"] = i;
			Item.CloseSelection = true;
			SelectionForm->Items.push_back(Item);
		}

		std::auto_ptr<TStringList>TabList(new TStringList);
		TDBTables::GetSeats(DBTransaction, TabList.get(), static_cast<int>(OrderContainer.Location["SelectedTable"]));
		for (int i = 0; i < TabList->Count; i++)
		{
			int SeatNo = TDBTables::GetSeatNoFromTabKey(DBTransaction, (int)TabList->Objects[i]);
			TVerticalSelection Item = SelectionForm->Items[SeatNo];
			Item.Title = TabList->Strings[i] + " [Active]";
			Item.Properties["TabKey"] = (int)TabList->Objects[i];
			SelectionForm->Items[SeatNo] = Item;
		}

		do
		{
			Retval = mrOk;
			SelectionForm->ShowModal();
			TVerticalSelection SelectedItem;
			if (SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel")
			{
				int TabKey = SelectedItem.Properties["TabKey"];

                if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot &&
                            TGlobalSettings::Instance().EnableCustomerJourney )
                {
                    UnicodeString AccountNumber = TDBTab::GetAccountNumber(DBTransaction, TabKey);
                    if(AccountNumber != "" && AccountNumber.Compare(OrderContainer.Location["AccNo"]) )
                    {
                        MessageBox("Order with different room no can't be saved..", "Error", MB_OK + MB_ICONERROR);
                        return mrAbort;
                    }
                }

				OrderContainer.Location["TabKey"] = TabKey;
				OrderContainer.Location["SelectedSeat"] = SelectedItem.Properties["SelectedSeat"];
				OrderContainer.Location["TabName"] = SelectedItem.Properties["TabName"];
				if (!TGlobalSettings::Instance().DisableConfirmationOnSave)
				{
					OrderContainer.Values.push_back(TSaveOrdersTo::StringValuePair("Total", InitialMoney.GrandTotal));
					Currency Balance = TDBTab::GetTabBalance(DBTransaction, TabKey);
					OrderContainer.Values.push_back(TSaveOrdersTo::StringValuePair("Balance", Balance));
					OrderContainer.Values.push_back(TSaveOrdersTo::StringValuePair("New Balance", Balance + InitialMoney.GrandTotal));

                   TfrmConfirmOrder* frmConfirmOrder = new TfrmConfirmOrder(this, OrderContainer);


					if (!TGlobalSettings::Instance().DisableReceiptOnConfirmation)
					{
						SetReceiptPreview(DBTransaction, frmConfirmOrder->ReceiptDisplay, OrderContainer.Location["TMMTabType"], OrderContainer.Location["ContainerName"],
							OrderContainer.Location["TabName"], OrderContainer.Location["PartyName"], OrderContainer.Location["TabKey"], OrderContainer.Location["SelectedTable"],
							OrderContainer.Location["SelectedSeat"], OrderContainer.Location["RoomNumber"]);
					}

					if (frmConfirmOrder->ShowModal() != mrOk)
					{
						Retval = mrAbort;
					}
                    else
                    {
                       // save party name on table..
                       SelectedParty = SetPartyNameOnChitSettings(DBTransaction, SelectedParty, OrderContainer.Location["SelectedTable"], true);
                       if(OrderContainer.Location["PrintPreLimReceipt"])
                       {
                         TDBTables::SetTableBillingStatus(DBTransaction,OrderContainer.Location["SelectedTable"],ePrelim);
                       }
                       else
                       {
                         TDBTables::SetTableBillingStatus(DBTransaction,OrderContainer.Location["SelectedTable"],eNoneStatus);
                       }

                        if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot &&
                                TGlobalSettings::Instance().EnableCustomerJourney)
                        {
                            TDBTables::SetSeatName(DBTransaction, selectedRoomNumberStr, OrderContainer.Location["SelectedTable"],
                                    OrderContainer.Location["SelectedSeat"]);
                        }
                    }
                    delete frmConfirmOrder;
                    frmConfirmOrder = NULL;
				}
			}
			else
			{
				Retval = mrCancel;
			}
		}
		while (Retval == mrAbort);

	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		Retval = mrCancel;
	}
	return Retval;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tnpQuantityClick(TObject *Sender, TNumpadKey Key)
{
   static const double maximum_quantity = 9999.0;

   if (!CheckAdJustItemQty() || lbDisplay->ItemIndex == -1)
      return;
   else if (QtyDisplay->Numeric() > maximum_quantity)
      QtyDisplay->SetNumeric(static_cast<int>(QtyDisplay->Numeric()) / 10);
   else
        ChangeItemQty = true;
        CheckAndSetItemQuantity();
}
// ---------------------------------------------------------------------------
bool TfrmSelectDish::TabPINOk(Database::TDBTransaction &DBTransaction, int inTabKey)
{
	std::auto_ptr<TContactStaff>Staff(new TContactStaff(DBTransaction));
	AnsiString TabPIN = TDBTab::GetTabPIN(DBTransaction, inTabKey);
	bool TabPINProceed = false;
	if (TabPIN != "" && !Staff->TestAccessLevel(TDeviceRealTerminal::Instance().User, CheckTabPINOverride))
	{
		std::auto_ptr<TfrmTouchNumpad>frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
		frmTouchNumpad->Caption = "Enter the PIN for this Tab";
		frmTouchNumpad->btnSurcharge->Caption = "Ok";
		frmTouchNumpad->btnDiscount->Visible = false;
		frmTouchNumpad->btnSurcharge->Visible = true;
		frmTouchNumpad->Mode = pmPIN;
		if (frmTouchNumpad->ShowModal() == mrOk)
		{
			if (frmTouchNumpad->STRResult == TabPIN)
			{
				TabPINProceed = true;
			}
			else
			{
				MessageBox("Incorrect PIN", "Error", MB_OK);
			}
		}
	}
	else
	{
		TabPINProceed = true;
	}
	return TabPINProceed;
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::WriteOffBillAsWastage()
{
	TMMContactInfo TempUserInfo;
	bool Allowed = false;
	TempUserInfo.Clear();

	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	DBTransaction.StartTransaction();

	std::auto_ptr<TContactStaff>Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this, DBTransaction, TempUserInfo, CheckCredit);
	if (Result == lsAccepted)
	{
		Allowed = true;
	}
	else if (Result == lsDenied)
	{
		MessageBox("You do not have access rights to Credits / Wastage.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}
	DBTransaction.Commit();

	if (Allowed)
	{

		bool Allow = Staff->TestAccessLevel(TempUserInfo, CheckWriteOff);
		if (Allow)
		{

			if (MessageBox("Do you wish to write off these items as wastage?", "Write off as Wastage", MB_YESNO + MB_ICONQUESTION + MB_DEFBUTTON1) == IDYES)
			{

				TWriteOff WriteOff(this);
				WriteOff.runReason();

				std::auto_ptr<TList>OrdersList(new TList);
				for (UINT iSeat = 0; iSeat < SeatOrders.size(); iSeat++)
				{
					for (int i = 0; i < SeatOrders[iSeat]->Orders->Count; i++)
					{
						TItemComplete *Order = SeatOrders[iSeat]->Orders->Items[i];
						Order->Reason = WriteOff.GetReason();
						OrdersList->Add(Order);
					}
				}

				DBTransaction.StartTransaction();

				// std::vector<TItemMinorComplete> AllOrders;

				for (int i = 0; i < OrdersList->Count; i++)
				{
					TItemComplete *Order = (TItemComplete*)OrdersList->Items[i];
					// TItemMinorComplete *temp = Order;

					Order->WastageRecipesToApply->RecipeCopyList(Order->SalesRecipesToApply);
					Order->WastageRecipesToApply->AdjustmentType = eStockWriteOffs;
					Order->SalesRecipesToApply->Clear();

					// WriteOff.InsertToDatabase(DBTransaction, Order, TempUserInfo);

					for (int j = 0; j < Order->SubOrders->Count; j++)
					{
						TItemCompleteSub *SubOrder = (TItemCompleteSub*)Order->SubOrders->Items[j];

						SubOrder->WastageRecipesToApply->RecipeCopyList(Order->SalesRecipesToApply);
						SubOrder->WastageRecipesToApply->AdjustmentType = eStockWriteOffs;
						SubOrder->SalesRecipesToApply->Clear();

						// temp = SubOrder;

						// WriteOff.InsertToDatabase(DBTransaction, SubOrder, TempUserInfo);
					}

				}

				std::vector<int>WriteOffKeys;

				WriteOff.InsertToDatabase(DBTransaction, OrdersList.get(), TDeviceRealTerminal::Instance().User.Name, WriteOffKeys);

				TDeviceRealTerminal::Instance().ManagerStock->UpdateStock(DBTransaction, OrdersList.get(), TDeviceRealTerminal::Instance().User.Name);
				DBTransaction.Commit();

				for (UINT iSeat = 0; iSeat < SeatOrders.size(); iSeat++)
				{
					for (int i = 0; i < SeatOrders[iSeat]->Orders->Count; i++)
					{
						delete SeatOrders[iSeat]->Orders->Items[i];;
					}
					for (int j = 0; j < SeatOrders[iSeat]->Orders->PrevCount; j++)
					{
						delete SeatOrders[iSeat]->Orders->PrevItems[j];
					}
					SeatOrders[iSeat]->Orders->Clear();
					SeatOrders[iSeat]->Orders->ClearPrev();
					SeatOrders[iSeat]->Orders->AppliedMembership.Clear();
				}
				ResetPOS();
			}
		}
		else
		{
			MessageBox("You do not have access rights to Write off.", "Error", MB_OK + MB_ICONERROR);
		}
	}
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::EnterPaxCount()
{
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	DBTransaction.StartTransaction();

	TPaxCountController PaxCountController(this, DBTransaction);
	PaxCountController.Run();
	PaxCountController.frmListManager->tbtnDelete->Show();

	if(PaxCountController.frmListManager->ShowModal() == mrOk)
    {
		PaxCountController.Save(TDeviceRealTerminal::Instance().ID.Name);
    }
     DBTransaction.Commit();
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::AlterFloat()
{
	// Display Reports List
    if(TGlobalSettings::Instance().EnableDontClearZedData)
    {
       MessageBox("Slave terminal can not perform Float function", "Warning", MB_OK + MB_ICONQUESTION);
       return;
    }
	std::auto_ptr<TfrmVerticalSelect>SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

	TVerticalSelection Item;
	Item.Title = "Cancel";
	Item.Properties["Color"] = IntToStr(clMaroon);
	Item.CloseSelection = true;
	SelectionForm->Items.push_back(Item);

	TVerticalSelection Item1;
	Item1.Title = "Set Float";
	Item1.Properties["Action"] = IntToStr(1);
	Item1.Properties["Color"] = IntToStr(clNavy);
	Item1.CloseSelection = true;
	SelectionForm->Items.push_back(Item1);

	TVerticalSelection Item2;
	Item2.Title = "Adjust Float";
	Item2.Properties["Action"] = IntToStr(2);
	Item2.Properties["Color"] = IntToStr(clNavy);
	Item2.CloseSelection = true;
	SelectionForm->Items.push_back(Item2);

	SelectionForm->ShowModal();
	TVerticalSelection SelectedItem;
	if (SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel")
	{
		int Action = StrToIntDef(SelectedItem.Properties["Action"], 0);
		switch(Action)
		{
		case 1:
			{
				try
				{
					TMMContactInfo TempUserInfo;
					Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
					DBTransaction.StartTransaction();
					std::auto_ptr<TContactStaff>Staff(new TContactStaff(DBTransaction));
					bool AllowFloat = false;
					TLoginSuccess Result = Staff->Login(this, DBTransaction, TempUserInfo, CheckSetFloat);
					if (Result == lsAccepted)
					{
						AllowFloat = true;
					}
					else if (Result == lsDenied)
					{
						MessageBox("You do not have access rights to set floats.", "Error", MB_OK + MB_ICONERROR);
					}
					else if (Result == lsPINIncorrect)
					{
						MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
					}

					if (AllowFloat)
					{
						std::auto_ptr<TManagerFloat>(FloatManager)(new TManagerFloat((TForm*)this));
						FloatManager->SetFloat(DBTransaction, TempUserInfo);
					}
					DBTransaction.Commit();
				}
				catch(Exception & E)
				{
					MessageBox(E.Message, "Error", MB_OK + MB_ICONERROR);
					TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
				}
			}break;
		case 2:
			{
				try
				{
					Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
					DBTransaction.StartTransaction();
					std::auto_ptr<TContactStaff>Staff(new TContactStaff(DBTransaction));

					TMMContactInfo TempUserInfo;
					bool AllowAdjustment = false;
					TempUserInfo.Clear();
					TLoginSuccess Result = Staff->Login(this, DBTransaction, TempUserInfo, CheckAdjustFloat);
					DBTransaction.Commit();
					if (Result == lsAccepted)
					{
						AllowAdjustment = true;
					}
					else if (Result == lsDenied)
					{
						MessageBox("You do not have access rights to skim or re-float the till.", "Error", MB_OK + MB_ICONERROR);
					}
					else if (Result == lsPINIncorrect)
					{
						MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
					}

					if (AllowAdjustment)
					{
						DBTransaction.StartTransaction();
						std::auto_ptr<TManagerFloat>(FloatManager)(new TManagerFloat((TForm*)this));
						FloatManager->AlterFloat(DBTransaction, TempUserInfo);
						DBTransaction.Commit();
					}
				}
				catch(Exception & E)
				{
					MessageBox(E.Message, "Error", MB_OK + MB_ICONERROR);
					TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
				}
			}break;
		}
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::webDisplayBeforeNavigate2(TObject *ASender, const IDispatch *pDisp, OleVariant &URL, OleVariant &Flags, OleVariant &TargetFrameName, OleVariant &PostData,
	OleVariant &Headers, WordBool &Cancel)

{
	TWebBrowser *Browser;
    //Browser->Document
	if ((Browser = dynamic_cast<TWebBrowser*>(ASender)) != NULL)
	{
		std::auto_ptr<TManagerWebDispatch>Dispatch(new TManagerWebDispatch(this, Browser));
		UnicodeString Data = URL;
		Dispatch->Execute(Data, Cancel);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tbtnWebOrdersMouseClick(TObject *Sender)
{
    if(TGlobalSettings::Instance().WebMateEnabled)
    {
	    ProcessWebOrders(true);
    }
    else
    {
        MessageBox("Please, Setup Webmate first.", "Webmate is Disabled.", MB_OK + MB_ICONWARNING);
    }

}
// ---------------------------------------------------------------------------
void TfrmSelectDish::InitXeroIntegration()
{
	TXeroIntegration::Instance().XeroMachineName = TGlobalSettings::Instance().XeroMachineName;
	TXeroIntegration::Instance().XeroFolderPath  = TGlobalSettings::Instance().XeroFolderPath;
	TXeroIntegration::Instance().XeroUserName    = TGlobalSettings::Instance().XeroUserName;
	TXeroIntegration::Instance().XeroPassword    = TGlobalSettings::Instance().XeroPassword;

	TXeroIntegration::Instance().Validate();
}
//---------------------------------------------------------------------------
void TfrmSelectDish::OpenTPConnector()
{
	tpConnectorUp = false;
	tpOpen        = false;

	unsigned __int32 params[2];
	params[0] = 128;                //maxLocations;
	params[1] = getMaxTableCount(); //maxTablesInLocation;

	TProxyMateManager::Instance().AppConnectorServer->OpenConnectorAck = mmAppConnectorOpenConnectorAck;
	tpConnector = ( TMMTablePickerConnectorServer* )TProxyMateManager::Instance().AppConnectorServer->OpenConnector( mmcTablePicker, params, 2 );

	if( tpConnector != NULL )
	{
		tpConnector->OnClientUp           = tpClientUp;
		tpConnector->OnClientDown         = tpClientDown;
		tpConnector->OnSelectedTableEvent = tpSelectedTable;
	}
}
//---------------------------------------------------------------------------
void TfrmSelectDish::CloseTPConnector()
{
	TProxyMateManager::Instance().AppConnectorServer->CloseConnector( mmcTablePicker );
	TProxyMateManager::Instance().AppConnectorServer->OpenConnectorAck = NULL;
	tpConnector = NULL;
}
//---------------------------------------------------------------------------
void TfrmSelectDish::showTablePicker()
{
    try
    {
        if( tpConnectorUp )
        {
            showNewTablePicker( tpConnector );
        }
        else
        {
            showOldTablePicker();
        }

        //MM-1647: Ask for chit if it is enabled for every order.
        NagUserToSelectChit();
        //MM-1649: Ask for patron count if it is enabled..
        InitializeTablePatrons();
    }
    catch(Exception & E)
    {
         TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}
//---------------------------------------------------------------------------
void TfrmSelectDish::showNewTablePicker( TMMTablePickerConnectorServer* inConnector )
{
    try
    {
        if( !tpOpen )
        {
            std::vector<TLocationStatus> currentTables;
            //setCurrentTables( currentTables );
            inConnector->ShowTablePicker( currentTables );
            tpOpen = true;

            currentTables.clear();
        }
    }
    catch(Exception & E)
    {
         TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}
//---------------------------------------------------------------------------
void TfrmSelectDish::showOldTablePicker()
{
    try
    {
        bool tableSelected = false;
        TFloorPlanReturnParams floorPlanReturnParams;
        // Runs new web app of floorPlan
        std::auto_ptr<TEnableFloorPlan>floorPlan(new TEnableFloorPlan());
        if(floorPlan->Run( ( TForm* )this, true, floorPlanReturnParams ))
//        if( TEnableFloorPlan::Instance()->Run( ( TForm* )this, true, floorPlanReturnParams ) )
        {
            tableSelected            = true;
            SelectedTable            = floorPlanReturnParams.TabContainerNumber;
            SelectedTabContainerName = floorPlanReturnParams.TabContainerName;
            SelectedParty = floorPlanReturnParams.PartyName;

            refreshSelectedSeat();
            RefreshSeats();
            if( TGlobalSettings::Instance().CaptureCustomerName )
            {
                TCustNameAndOrderType::Instance()->LoadFromOrdersDatabase( SelectedTable );
            }
        }
        floorPlan.reset();
    }
    catch(Exception & E)
    {
         MessageBox(E.Message,"",MB_OK);
         TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}
//---------------------------------------------------------------------------
 // NOT USED!!!
void __fastcall TfrmSelectDish::mmAppConnectorOpenConnectorAck( System::TObject* Sender, int inACK, TMMConnectorTypes inConnectorType, TMMConnectorServer* inConnector  )
{
	switch( inConnectorType )
	{
		case mmcTablePicker: tpConnectorUp = true; break;
	}
}
//---------------------------------------------------------------------------
 // NOT USED!!!
void __fastcall TfrmSelectDish::tpClientUp( TObject* Sender )
{
	tpConnectorUp = true;  // Might not be needed
}
//---------------------------------------------------------------------------
 // NOT USED!!!
void __fastcall TfrmSelectDish::tpClientDown( TObject* Sender  )
{
	tpConnectorUp = false;
	tpOpen        = false;
}
//---------------------------------------------------------------------------
 // NOT USED!!!
void __fastcall TfrmSelectDish::tpSelectedTable( TObject* Sender, TFPTableData* inData )
{
	tpOpen = false;

	if( ( inData->Location > 0 ) && ( inData->Table > 0 ) && ( inData->Action == 1 ) )
	{
		SelectedTable            = inData->Table;
		SelectedTabContainerName = inData->TableName;

		refreshSelectedSeat();
		RefreshSeats();
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::refreshSelectedSeat()
{
	lbDisplay->Clear();
	SelectedSeat = 0;
	for (int i = 0; i < tgridSeats->ColCount; i++)
	{
		tgridSeats->Buttons[0][i]->Color = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
		tgridSeats->Buttons[0][i]->Latched = false;
	}

    unsigned __int32 maxSeatCount = getMaxSeatCount();

	for( unsigned __int32 i = 1; i <= maxSeatCount; i++ )
	{
		while (SeatOrders[i]->Orders->PrevCount != 0)
		{
			delete SeatOrders[i]->Orders->PrevItems[0];
			SeatOrders[i]->Orders->DeletePrev(0);
		}

		SeatOrders[i]->Orders->ClearPrev();
		SeatOrders[i]->SeatName = "";
        if(!SeatOrders[0]->isChangeTablePressed)
        {
            SeatOrders[i]->RoomNumber = "";
            SeatOrders[i]->Orders->pmsAccountDetails.RoomNumber = "";
            SeatOrders[i]->Orders->pmsAccountDetails.FirstName = "";
            SeatOrders[i]->Orders->pmsAccountDetails.LastName = "";
            SeatOrders[i]->Orders->pmsAccountDetails.RoomBedNumber = "";
        }
	}
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	TDBTab::ReleaseTab(DBTransaction, TDeviceRealTerminal::Instance().ID.Name, 0);
	TDBOrder::GetPrevOrders(DBTransaction, TDeviceRealTerminal::Instance().ID.Name, SelectedTable, 0, SeatOrders);

    if(TGlobalSettings::Instance().IsThorlinkSelected)
    {
       RemoveMembership(DBTransaction);
    }
	DBTransaction.Commit();
	SetSelectedSeat(true);
	RedrawSeatOrders();
}
//---------------------------------------------------------------------------
UnicodeString TfrmSelectDish::GetTenderStrValue( vmVariables inVariable )
{
	UnicodeString str          = "";
	Currency      defaultValue = 0;

	switch( inVariable )
	{
		case vmbtnDollar1: defaultValue = 1;  break;
		case vmbtnDollar2: defaultValue = 5;  break;
		case vmbtnDollar3: defaultValue = 10; break;
		case vmbtnDollar4: defaultValue = 20; break;
		case vmbtnDollar5: defaultValue = 50; break;
	}

	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	int precision = 7;
	str = TManagerVariable::Instance().GetStr(DBTransaction, inVariable, FloatToStrF(defaultValue, ffNumber, precision, CurrencyDecimals));
        /*TPaymentTransaction PaymentTransaction(DBTransaction);
        PaymentTransaction.Type = eTransQuickSale;
        TDeviceRealTerminal::Instance().PaymentSystem->PaymentsReload(PaymentTransaction);
        TPayment *CashPayment = PaymentTransaction.PaymentFind(str);
        if(CashPayment==NULL)
        {
           str  =FloatToStrF(defaultValue, ffNumber, precision, CurrencyDecimals);
        }*/

        return CheckCurrencyStr( str );
}
//---------------------------------------------------------------------------
UnicodeString TfrmSelectDish::CheckCurrencyStr( UnicodeString inCurrencyStr )
{
	if( inCurrencyStr.SubString( 1, 1 ) == "$" ) // Take off the dollar symbol ($)
	{
		inCurrencyStr = inCurrencyStr.SubString( 2, inCurrencyStr.Length() - 1 );
	}

	int decimalSeparator = inCurrencyStr.Pos( "." );
	UnicodeString fractionalStr = inCurrencyStr.SubString( decimalSeparator + 1, 2 ); // Get two digits of the decimal part

	if( fractionalStr == "00" ) // Take off the decimal part if it's zero
	{
		inCurrencyStr = inCurrencyStr.SubString( 1, decimalSeparator - 1 );
	}

	return inCurrencyStr;
}
//---------------------------------------------------------------------------
Currency TfrmSelectDish::BtnCaptionToCurrency( TTouchBtn* inBtn )
{
	 UnicodeString currencyStr = "0.0";

	 if( inBtn->Name == "tbtnDollar1" )
	 {
		currencyStr = GetTenderStrValue( vmbtnDollar1 );
	 }
	 if( inBtn->Name == "tbtnDollar2" )
	 {
		currencyStr = GetTenderStrValue( vmbtnDollar2 );
	 }
	 if( inBtn->Name == "tbtnDollar3" )
	 {
		currencyStr = GetTenderStrValue( vmbtnDollar3 );
	 }
	 if( inBtn->Name == "tbtnDollar4" )
	 {
		currencyStr = GetTenderStrValue( vmbtnDollar4 );
	 }
	 if( inBtn->Name == "tbtnDollar5" )
	 {
		currencyStr = GetTenderStrValue( vmbtnDollar5 );
	 }

	 int commaPos;
	 do
	 {
		commaPos = currencyStr.Pos( "," );
		currencyStr.Delete( commaPos, 1 );   // Remove all commas
	 }
	 while( commaPos > 0 );

	 //::::::::::::::::::::::::::::::::::::

	return StrToCurr( currencyStr );
}
//---------------------------------------------------------------------------
void TfrmSelectDish::SendOrderListToKitchen(Database::TDBTransaction &DBTransaction, TList *OrdersList, std::vector<TPatronType> Patrons)
{
	std::auto_ptr<TReqPrintJob>Request(new TReqPrintJob(&TDeviceRealTerminal::Instance()));
	std::auto_ptr<TPaymentTransaction>PrintTransaction(new TPaymentTransaction(DBTransaction));

	PrintTransaction->Patrons = Patrons;
	if (TGlobalSettings::Instance().CaptureCustomerName)
	{
		PrintTransaction->CustomerOrder = TCustNameAndOrderType::Instance()->GetStringPair();
	}

	Request->BarCodeData = CurrentTimeKey;
	Request->Transaction = PrintTransaction.get();
	Request->JobType = pjKitchen;
	TItemComplete *Order = (TItemComplete*)OrdersList->Items[0];
	Request->Waiter = Order->Security->SecurityGetField(secOrderedBy, secfFrom);
	//Request->Transaction->Membership.Assign(Membership);
	PrintTransaction->ChitNumber = ChitNumber;
	Request->MiscData["PartyName"] = ((TItemComplete*)OrdersList->Items[0])->PartyName;
	for (int i = 0; i < OrdersList->Count; i++)
	{
		TSecurityReference *OldSecRef = Order->Security->SecurityGetType(secCredit);
		if (OldSecRef == NULL)
		{
			PrintTransaction->Orders->Add(OrdersList->Items[i]);
		}
	}

	Request->Transaction->Money.Recalc(*Request->Transaction);
    std::auto_ptr<TKitchen> Kitchen(new TKitchen());
    Kitchen->Initialise(DBTransaction);
	Kitchen->GetPrintouts(DBTransaction, Request.get());
	Request->Printouts->Print(devPC);
	ManagerDockets->Archive(Request.get());

	//::::::::::::::::::::::::::::::::::::::::

	completeHeldOrdersToChefMate( PrintTransaction.get() );
}
//---------------------------------------------------------------------------
void TfrmSelectDish::HoldButtonClick(std::auto_ptr <TList> &OrderObjects, std::vector<TPatronType> Patrons)
{
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	if (ChitNumber.Valid())
	{
		int ActiveChitKey = TDBActiveChit::GetOrCreateActiveChit(DBTransaction, ChitNumber);
		TDBHeldOrder::HoldOrders(DBTransaction, OrderObjects.get(), Patrons, ActiveChitKey);
	}
	else
	{
		TDBHeldOrder::HoldOrders(DBTransaction, OrderObjects.get(), Patrons);
	}
	DBTransaction.Commit();
	// reset the chit number once the order is held

}
// ---------------------------------------------------------------------------
bool TfrmSelectDish::SendButtonClick(std::auto_ptr <TList> &OrderObjects, std::vector<TPatronType> &Patrons)
{
	std::auto_ptr <TfrmTables_ChitNumbers> frmTables_ChitNumbers(new TfrmTables_ChitNumbers(this, TDeviceRealTerminal::Instance().DBControl));
	bool SendSuccess = frmTables_ChitNumbers->ShowModal() == mrOk;
	if(SendSuccess)
	{
		OrderObjects = frmTables_ChitNumbers->Orders;
		ChitNumber 	 = frmTables_ChitNumbers->ChitNumber;
		Patrons 	 = frmTables_ChitNumbers->Patrons;

		RetrieveHeldCustomerName(OrderObjects);
	}
	return SendSuccess;
}
//---------------------------------------------------------------------------
AnsiString TfrmSelectDish::DisplayCustomerTypeReasons()
{
	std::auto_ptr <TfrmMessage> frmMessage(TfrmMessage::Create <TfrmMessage> (this, TDeviceRealTerminal::Instance().DBControl));
	frmMessage->MessageType = eCustomerTypes;
	frmMessage->ShowModal();
	return frmMessage->TextResult;
}
// ---------------------------------------------------------------------------
bool TfrmSelectDish::SendRelatedOrders(std::auto_ptr <TList> &OrderObjects, std::vector<TPatronType> &Patrons)
{
	bool Sent = false;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::vector<SendOrder_t> HeldOrders;
	std::auto_ptr<TSendFormController> sendFormController(new TSendFormController);
	sendFormController->LoadHeldOrders(DBTransaction, HeldOrders);
	std::set<int> ProcessedKeys;

	// checks whether the held orders match with the current order, if so send them
	if(OrderObjects.get()->Count > 0)
	{
		// 	get only the first order of current order list. Because for every order in the current list,
		//  apart from item details, the tabType, ActiveChitKey, TabName are similar and these are the only information
		//	we need to match up with the existing held orders
		TItemComplete *currentOrder = (TItemComplete *)OrderObjects->Items[0];
		//TChitNumber currentOrderChitNumber = currentOrder->GetActiveChitNumberKey() == -1 ? ChitNumber : TDBActiveChit::LoadChitNumberFromKey(DBTransaction, currentOrder->GetActiveChitNumberKey());

		for (int i = 0; i < HeldOrders.size(); i++)
		{
			SendOrder_t heldOrder = HeldOrders.at(i);

			if(ProcessedKeys.find(heldOrder.GetKey()) == ProcessedKeys.end())
			{
				bool processOrder = false;
				if(currentOrder->TabType == eTabType::TabTableSeat
					   && heldOrder.GetID() == "Table"
					   && heldOrder.GetRelatedTableNumber() == currentOrder->TableNo)
				{
					processOrder = true;
				}
				else if(currentOrder->TabType != eTabType::TabTableSeat
							&& currentOrder->TabType != eTabType::TabCashAccount
							&& heldOrder.GetID() == "Tab"
							&& heldOrder.GetRelatedTabName() == currentOrder->TabName)
				{
					processOrder = true;
				}
				else if(heldOrder.GetID() == "Chit")
				{
					//TChitNumber heldOrderChitNumber = TDBActiveChit::LoadChitNumberFromKey(DBTransaction, heldOrder.GetRelatedChitKey());

					// Compare active chit key of current order with active chit key of held order
					if(currentOrder->GetActiveChitNumberKey() == heldOrder.GetRelatedChitKey())
						processOrder = true;
				}
				else if(heldOrder.GetID() == "POS"
							&& heldOrder.GetCaption() == currentOrder->Terminal
							&& currentOrder->GetActiveChitNumberKey() == -1
							&& currentOrder->TableNo == 0
							&& (currentOrder->TabType == TabCashAccount
							 || currentOrder->TabType == TabNone))
				{
					processOrder = true;
				}
				else
				{
					processOrder = false;
                }

				if(processOrder)
				{
                    sendFormController->AddOrder(heldOrder.GetCaption(),heldOrder.GetKey());
					ProcessedKeys.insert(heldOrder.GetKey());
                }
			}
		}
	}

	// deleting from held order table
    ProcessedKeys.clear();
	std::auto_ptr<TList> SentOrders;
	TChitNumber SentChitNumber;
	std::vector<TPatronType> SentPatrons;

	SentOrders.reset(new TList);

	for(std::multimap<UnicodeString, int>::iterator it = sendFormController->begin(); it != sendFormController->end(); it++)
	{
		if (ProcessedKeys.find(it->second) == ProcessedKeys.end())
		{
			TDBHeldOrder::GetHeldOrdersFromKey(DBTransaction, SentOrders.get(), SentChitNumber, SentPatrons, it->second);
			ProcessedKeys.insert(it->second);
		}
	}

	for(std::set<int>::iterator it = ProcessedKeys.begin(); it != ProcessedKeys.end(); it++)
	{
        TDBHeldOrder::DeleteHeldOrderByKey(DBTransaction, *it);
	}

	DBTransaction.Commit();

	if(ProcessedKeys.size() > 0)
	{
		OrderObjects = SentOrders;
		ChitNumber 	 = SentChitNumber;
		Patrons 	 = SentPatrons;

		RetrieveHeldCustomerName(OrderObjects);

		Sent = true;
	}

	return Sent;
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::RetrieveHeldCustomerName(std::auto_ptr <TList> &OrderObjects)
{
	if (TGlobalSettings::Instance().CaptureCustomerName)
	{
		TItemComplete *Order = (TItemComplete*)OrderObjects->Items[0];
		TCustNameAndOrderType *CustName = TCustNameAndOrderType::Instance();
		CustName->CatchCustNameAndOrderType(Order->ContactsKey, Order->OrderTypeMessage);
		for (int i = 1; i < OrderObjects->Count && !CustName->Valid(); i++)
		{
			Order = (TItemComplete*)OrderObjects->Items[i];
			if (Order->ContactsKey != -1 && Order->OrderTypeMessage != "")
			{
				CustName->CatchCustNameAndOrderType(Order->ContactsKey, Order->OrderTypeMessage);
			}
		}
		CustName->ClearNameCaught();
	}
}
// ---------------------------------------------------------------------------
bool TfrmSelectDish::DisplayHoldSend(std::auto_ptr <TList> &OrderObjects, std::vector<TPatronType> &Patrons)
{
	bool Sent = false;
	std::auto_ptr <TfrmHoldSend> frmHoldSend(TfrmHoldSend::Create <TfrmHoldSend>(this));
	frmHoldSend->ShowModal();
	if (frmHoldSend->RetVal == IDOK)
	{
		HoldButtonClick(OrderObjects, Patrons);
		/*	Following line commented out, so that when in a cash sale, we will not show the form to select which orders to send
			instead we will send related orders right away
			Without completely commenting out the code, its better if we can provide a setting in maintenance area
		*/

		//	Sent = SendButtonClick(OrderObjects, Patrons);

		// redirecting the function call to send out related orders
		Sent = SendRelatedOrders(OrderObjects, Patrons);
	}
	else
	{
		HoldButtonClick(OrderObjects, Patrons);
	}
	return Sent;
}
// ---------------------------------------------------------------------------
int TfrmSelectDish::DisplayNameKeypad()
{
	AnsiString Name = "";
    int CustNameKey = -1;
	std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (Screen->ActiveForm));
	frmTouchKeyboard->MaxLength = 30;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = "";
	frmTouchKeyboard->Caption = "Enter the Customers Name";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
		Name = frmTouchKeyboard->KeyboardText;
	}

    if (Name != "")
    {
    //Create new contact to store name in the Contacts Table

        Database::TDBTransaction CustomerTransaction(TDeviceRealTerminal::Instance().DBControl);
        CustomerTransaction.StartTransaction();
        std::auto_ptr<TContact> Customer(new TContact());
        Customer->Enabled = true;
        Customer->ContactType = eCasualCustomer;
        CustNameKey = Customer->GetContactByName(CustomerTransaction, Name);
        if(CustNameKey == 0)
        {
            TMMContactInfo Info;
            Info.Name = Name;
            Info.SiteID = -1;
            Customer->SetContactDetails(CustomerTransaction,0,Info);
            CustNameKey = Customer->GetContactByName(CustomerTransaction, Name);
            CustomerTransaction.Commit();
        }
    }
    return CustNameKey;
}
//---------------------------------------------------------------------------
void TfrmSelectDish::setPatronCount( __int32 inPatronCount )
{
	_patronCount = inPatronCount;
}
//---------------------------------------------------------------------------
__int32 TfrmSelectDish::getPatronCount()
{
	return ( _patronCount > 0 ) ? _patronCount : 1;
}
// ---------------------------------------------------------------------------
std::vector<TPatronType> TfrmSelectDish::QueryForPatronCount(TPaymentTransaction &PaymentTransaction)
{
    std::vector<TPatronType> patrons;

	if (TManagerPatron::Instance().GetCount(PaymentTransaction.DBTransaction) > 0)
	{
		std::auto_ptr <TfrmPatronCount> frmPatronCount(TfrmPatronCount::Create <TfrmPatronCount> (this, TDeviceRealTerminal::Instance().DBControl));
        frmPatronCount->InitPatrons = PaymentTransaction.Patrons;
        frmPatronCount->ShowModal();
		patrons = frmPatronCount->Patrons;
	}

	return patrons;
}
// ---------------------------------------------------------------------------
std::vector<TPatronType> TfrmSelectDish::GetPatronCount(Database::TDBTransaction &dbTransaction)
{
	std::vector<TPatronType> patrons;
    try
    {
        if (TManagerPatron::Instance().GetCount(dbTransaction) > 0)
        {
            std::auto_ptr <TfrmPatronCount> frmPatronCount(TfrmPatronCount::Create <TfrmPatronCount> (this, TDeviceRealTerminal::Instance().DBControl));

            frmPatronCount->ShowModal();
            patrons = frmPatronCount->Patrons;
        }
    }
    catch(Exception & E)
    {
         TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
	return patrons;
}
//---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::
// ChefMate
//::::::::::::::::::::::::::::::::::::::::::::::::::

void TfrmSelectDish::completeOrderToChefMate( TPaymentTransaction* inTransaction )
{
  try
  {
    CMC_ERROR error = cmClientManager->SendCompleteOrder(inTransaction);
	if( error == CMC_ERROR_FAILED )
	{
		MessageBox( "Menumate failed to send an complete order to Chefmate",
					"Chefmate",
					MB_OK + MB_ICONWARNING);
	}
  }
 catch(Exception & Err)
   {}
}
//---------------------------------------------------------------------------
void TfrmSelectDish::completeHeldOrdersToChefMate( TPaymentTransaction* inTransaction )
{
     CMC_ERROR error = cmClientManager->SendCompleteOrder(inTransaction);    //mm-4979
	if( error == CMC_ERROR_FAILED )
	{
		MessageBox( "Menumate failed to send an complete held order to Chefmate",
					"Chefmate",
					MB_OK + MB_ICONWARNING);
	}
}
//---------------------------------------------------------------------------
void TfrmSelectDish::creditOrderToChefMate( const std::vector<TSeatOrders *> &seat_orders )
{
    if(seat_orders[SelectedSeat]->Orders && seat_orders[SelectedSeat]->Orders->Count > 0)
    {
        CMC_ERROR error = cmClientManager->SendCreditOrder( seat_orders[SelectedSeat]->Orders );

        if( error == CMC_ERROR_FAILED )
        {
            MessageBox( "Menumate failed to send an credit order to Chefmate",
                        "Chefmate",
                        MB_OK + MB_ICONWARNING);
        }
    }
}
//---------------------------------------------------------------------------
void __stdcall TfrmSelectDish::callAwayToChefMate( TCallAwayComplete* inCallAway )
{
	CMC_ERROR error = cmClientManager->SendCallAwayOrder( inCallAway );

	if( error == CMC_ERROR_FAILED )
	{
		MessageBox( "Menumate failed to send an call away order to Chefmate",
					"Chefmate",
					MB_OK + MB_ICONWARNING);
	}
}
//---------------------------------------------------------------------------
void TfrmSelectDish::retrieveForcedOptionList(
                                       __int32 inIndex,
                                        TItem* inItem,
                         TListOptionContainer* const outForcedOtionList )
{
	inItem->Options->OptionRetrieveForcedList(
                        inIndex,
                        inItem->ItemKey,
                        outForcedOtionList );
}
//---------------------------------------------------------------------------
unsigned __int32 TfrmSelectDish::getMaxTableCount()
{
    std::auto_ptr<TEnableFloorPlan> floorPlan(new TEnableFloorPlan());
    unsigned __int32 value = floorPlan->GetMaxTableCount();
    floorPlan.reset();
    return value;
//    return TEnableFloorPlan::Instance()->GetMaxTableCount();
}
//.............................................................................
unsigned __int32 TfrmSelectDish::getMaxSeatCount()
{
    std::auto_ptr<TEnableFloorPlan> floorPlan(new TEnableFloorPlan());
    unsigned __int32 value = floorPlan->GetMaxSeatCount();
    floorPlan.reset();
    return value;
//    return TEnableFloorPlan::Instance()->GetMaxSeatCount();
}
//.............................................................................
bool SortItemByPrice(TItemMinorComplete *lhs, TItemMinorComplete *rhs)
{
	return lhs->PriceEach() < rhs->PriceEach();
}
//.............................................................................
void TfrmSelectDish::CheckDeals(Database::TDBTransaction &DBTransaction)
{
	IDealManager* dealManager = TDealManager::Instance();
	std::vector<int> dealCatKeys = dealManager->GetCategoryKeysByPriority();
	std::vector<TItemMinorComplete*> itemsInCategory;
	std::multimap<TItemMinorComplete*, int> catItemMap;
	TDiscount DealDiscount;
	DealDiscount.DiscountKey = dsMMDealKey;
	DealDiscount.MaximumValue = 0;
	ManagerDiscount->ClearDiscount(SeatOrders[SelectedSeat]->Orders->List, DealDiscount);
	MapItemsAndCategories(catItemMap, DealDiscount.MaximumValue);

	for (std::vector<int>::iterator catIt = dealCatKeys.begin(); catIt != dealCatKeys.end(); catIt++)
	{
		itemsInCategory.clear();
		for (std::multimap<TItemMinorComplete*, int>::iterator catItemMapIt = catItemMap.begin(); catItemMapIt != catItemMap.end(); catItemMapIt++) //for (std::multimap<TItemComplete*, int>::iterator catItemMapIt = catItemMap.begin(); catItemMapIt != catItemMap.end(); catItemMapIt++)
		{
			if (catItemMapIt->second == *catIt
                && !catItemMapIt->first->DiscountFind(DiscModeCombo))  // prevent adding items for deals if they already have combo discounts applied
			{
				itemsInCategory.push_back(catItemMapIt->first);
			}
		}
		std::sort(itemsInCategory.begin(), itemsInCategory.end(), SortItemByPrice);
		std::vector<TDealResult> deals = dealManager->GetDiscountedPrices(*catIt, Currency((int)itemsInCategory.size()));

		for (std::vector<TDealResult>::iterator dealsIt = deals.begin(); dealsIt != deals.end(); dealsIt++)
		{
			DealDiscount.Name = dealsIt->DealName;
			DealDiscount.Description = "Deal Discount - " + dealsIt->DealName;
			DealDiscount.Priority = dealsIt->Priority;
			DealDiscount.Amount = dealsIt->Price;
			DealDiscount.ComboAmount = dealsIt->Price;
			ManagerDiscount->GetDiscount(DBTransaction, dsMMDealKey, DealDiscount);
			TList *Orders = new TList();
			std::vector<TItemMinorComplete*>::iterator itemIt;
			itemIt = itemsInCategory.begin();
			TItemMinorComplete* item = NULL;
			TItemMinorComplete* prevItem = NULL;
			for (int i = 0; i < dealsIt->Quantity; i++, itemIt++)
			{
				item = *itemIt;
				if(item != prevItem)
				{
                    item->DealSelectedItems++;
					Orders->Add(item);
                    prevItem = item;
				}
                else
                {
                    prevItem->DealSelectedItems++;
                }

				catItemMap.erase(*itemIt);
			}
			itemsInCategory.erase(itemsInCategory.begin(), itemIt);
			ManagerDiscount->AddDiscount(Orders, DealDiscount);
			delete Orders;
            Orders = NULL;
		}
	}
}
//.............................................................................
void TfrmSelectDish::MapItemsAndCategories(std::multimap<TItemMinorComplete*, int> &itemCatMap, Currency &totalItemPrice)    //void TfrmSelectDish::MapItemsAndCategories(std::multimap<TItemComplete*, int> &itemCatMap, Currency &totalItemPrice)
{
	totalItemPrice = 0;
    /* without having to iterate through orders and subOrders we could use the flattern_item_tree method with a predicate that always returns true */
    for (int i = 0; i < SeatOrders[SelectedSeat]->Orders->Count; i++)
	{
		TItemComplete* item = SeatOrders[SelectedSeat]->Orders->Items[i];
		for (int j = 0; j < item->Categories->Count; j++)
		{
			TItemSizeCategory* category = item->Categories->CategoryGet(j);
			int catKey = category->CategoryKey;
            item->PrevDealSelectedItems = 0;
			item->DealSelectedItems = 0;
			for (int qty = 1; qty <= item->GetQty(); qty++)
			{
				itemCatMap.insert(std::pair<TItemComplete*, int>(item, catKey));
			}
		}

        // add sub orders (sides) in to the category map as well
        for(int k=0; k < item->SubOrders->Count ; k++)
        {
            TItemCompleteSub* itemSub = item->SubOrders->SubOrderGet(k);
            for (int j = 0; j < itemSub->Categories->Count; j++)
            {
                TItemSizeCategory* subCategory = itemSub->Categories->CategoryGet(j);
                int subCatKey = subCategory->CategoryKey;
                itemSub->PrevDealSelectedItems = 0;
                itemSub->DealSelectedItems = 0;
                for (int qty = 1; qty <= itemSub->GetQty(); qty++)
                {
                    itemCatMap.insert(std::pair<TItemCompleteSub*, int>(itemSub, subCatKey));
                }
            }
        }
        totalItemPrice += item->Price();
	}
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::GetAllOrders(TList* outOrders)
{
    for(int i = 0; i < SeatOrders.size(); i++)
    {
        for(int j = 0; j < SeatOrders[i]->Orders->Count; j++)
        {
            outOrders->Add(SeatOrders[i]->Orders->Items[j]);
        }
    }
}

//::::::::::::::::::::::::::
// Customer Display
//::::::::::::::::::::::::::
void TfrmSelectDish::startCustomerDisplayServer()
{
    if( TGlobalSettings::Instance().ShowCustomerDisplay )
    {
        customer_display_timer        = new TTimer( this );
        customer_display_finish_timer = new TTimer( this );

        customer_display_manager = TMMCustomerDisplayManager::Instance();
        customer_display_manager->SetTimer(       customer_display_timer );
        customer_display_manager->SetFinishTimer( customer_display_finish_timer );

        customerDisplayResetPOSTotals();
    }
}
//.............................................................................
void TfrmSelectDish::stopCustomerDisplayServer()
{
    if( TGlobalSettings::Instance().ShowCustomerDisplay )
    {
        delete customer_display_timer;
        customer_display_timer = NULL;
        delete customer_display_finish_timer;
        customer_display_finish_timer = NULL;
    }
}
//.............................................................................
void TfrmSelectDish::customerDisplayResetPOSTotals()
{
    if( TGlobalSettings::Instance().ShowCustomerDisplay )
    {
        total_service_charge = 0;
        total_tax            = 0;
    }
}
//.............................................................................
void TfrmSelectDish::customerDisplayReset()
{
    if( TGlobalSettings::Instance().ShowCustomerDisplay )
    {
        customer_display_manager->Reset();
    }
}
//.............................................................................
void TfrmSelectDish::customerDisplaySendOrder( std::vector <TSeatOrders*> inSeatOrder )
{
    //customer_display_manager->SendOrderFromFile( "C:\\Code\\GIT\\remoteMenumate\\Second Display\\XMLOrder.xml" );

    if( TGlobalSettings::Instance().ShowCustomerDisplay )
    {
        UnicodeString changeStr = "0.00";
        customerDisp.changeStr=  changeStr;

      customer_display_manager->SendOrder(inSeatOrder, customerDisp );
     // customerDisp.HappyBirthDay = false;
     // customerDisp.FirstVisit = false;
    }
}
//.............................................................................
void TfrmSelectDish::customerDisplayOrderPaid( Currency inLastTotal, Currency inLastChange )
{
    if( TGlobalSettings::Instance().ShowCustomerDisplay )
    {
        customer_display_manager->SendOrderPaid(
                                     inLastTotal,
                                     inLastChange,
                                     total_service_charge,
                                     total_tax );

        customerDisplayResetPOSTotals();
    }
}
//.............................................................................
void TfrmSelectDish::AutoLogOut()
{
    if (TGlobalSettings::Instance().AutoLogoutPOS && TGlobalSettings::Instance().AutoLogoutSeconds == 0)
     {
        LockOutUser();
     }
}

//:::::::::::::::::::::::::::::::::::::::::::::::
// Parked Sales
//:::::::::::::::::::::::::::::::::::::::::::::::

void TfrmSelectDish::setParkedSaleBtnStatus()
{
    tbtnParkSales->Enabled = TGlobalSettings::Instance().EnableParkedSales;
}
//.............................................................................
void TfrmSelectDish::setParkedSalesBtnColor()
{
	Database::TDBTransaction DBTransaction( TDeviceRealTerminal::Instance().DBControl );
	DBTransaction.StartTransaction();

    if( ParkedSales->GetCount( DBTransaction ) != 0 )
    {
       tbtnParkSales->ButtonColor = clGreen;
       // tbtnParkSales->ButtonColor =0x002193F6;
       tbtnParkSales->Font->Color=clWhite;
    }
    else
    {
        if(TGlobalSettings::Instance().ShowDarkBackground)
        {
           tbtnParkSales->ButtonColor = 14342874;
        }
        else
        {
           tbtnParkSales->ButtonColor = tbtnParkSales->Tag;
        }
        tbtnParkSales->Font->Color=  0x002193F6;
    }

	DBTransaction.Commit();
}
//.............................................................................
void TfrmSelectDish::setParkedSale( TParkedSale* inSale )
{
    if( TGlobalSettings::Instance().EnableParkedSales )
    {
        Database::TDBTransaction transaction( TDeviceRealTerminal::Instance().DBControl );

        transaction.StartTransaction();
        ParkedSales->SetParkedSale( transaction, inSale );
        transaction.Commit();

        customerDisplayReset();
        customerDisplayResetPOSTotals();
    }
}
//.............................................................................
void TfrmSelectDish::ClearAllParkedSales( Database::TDBTransaction &DBTransaction )
{
	if( ParkedSales->GetCount( DBTransaction ) > 0 )
	{
		ParkedSales->DeleteAll( DBTransaction );
	}
	//tbtnParkSales->ButtonColor = clMaroon;
}
//.............................................................................
void TfrmSelectDish::ChitCaptAndSaveName()
{
    std::auto_ptr<TfrmCaptNamePhone>(frmCaptNamePhone)(TfrmCaptNamePhone::Create<TfrmCaptNamePhone>(this));
    TGlobalSettings::Instance().IsAutoLoggedOut = true;
    if (frmCaptNamePhone->ShowModal() == mrOk)
    {
        CustName = frmCaptNamePhone->edCustomerName->Text;
        CustPhone = frmCaptNamePhone->edCustomerPhone->Text;
        //MM2038
        TGlobalSettings::Instance().TabPrintName = CustName;
        TGlobalSettings::Instance().TabPrintPhone = CustPhone;
    }
    else
    {
       CustName = "";
       CustPhone = "";
    }
    TGlobalSettings::Instance().IsAutoLoggedOut = false;

    if (CustName != "")
    {
        //Create new contact to store name in the Contacts Table
        int CustNameKey = -1;
        Database::TDBTransaction CustomerTransaction(TDeviceRealTerminal::Instance().DBControl);
        CustomerTransaction.StartTransaction();
        std::auto_ptr<TContact> Customer(new TContact());
        Customer->Enabled = true;
        Customer->ContactType = eMaxGamingMember;
        CustNameKey = Customer->GetContactByName(CustomerTransaction, CustName);
        if(CustNameKey == 0)
        {
            TMMContactInfo Info;
            Info.Name = CustName; //save name to db contact
            Info.Phone = CustPhone; //save phone number to contact
            Info.SiteID = -1;
            Customer->SetContactDetails(CustomerTransaction,0,Info);
            CustNameKey = Customer->GetContactByName(CustomerTransaction, CustName);
            CustomerTransaction.Commit();
        }
    }
}
// ---------------------------------------------------------------------------
TModalResult TfrmSelectDish::setupAutoSaveTabContainer(Database::TDBTransaction &DBTransaction, TSaveOrdersTo &OrderContainer)
{
    TModalResult Retval = mrAbort;
    bool uniqueTab = false;

    try
    {
        std::auto_ptr<TfrmAddTab>frmAddTab(TfrmAddTab::Create(this));
        frmAddTab->LoadDetails(DBTransaction, 0);
        //frn
        frmAddTab->btnName->Caption = CustName;
        frmAddTab->TabName=CustName;
        frmAddTab->btnNumber->Caption = CustPhone;
        frmAddTab->TabIDNumber = CustPhone;
        frmAddTab->AllowCancel = false;

        // forcing user to create a unique tab
        while( !uniqueTab && frmAddTab->ShowModal() == mrOk )
        {
            uniqueTab = TDBTab::GetTab(DBTransaction, frmAddTab->TabName, TabNormal) == 0;

            if ( !uniqueTab )
            {
                MessageBox("A Tab with this name already exists. Please select a different name.", "Warning", MB_OK + MB_ICONWARNING);
            }
        }

        if (uniqueTab)
        {
            int TabyKey = TDBTab::GetOrCreateTab(DBTransaction, 0);
            TDBTab::SetTabDetails(DBTransaction, TabyKey, frmAddTab->TabName, frmAddTab->TabIDType, frmAddTab->TabIDNumber, frmAddTab->TabIDExpiry, TabNormal);
            TDBTab::SetTabCard(DBTransaction, TabyKey, frmAddTab->CardString);
            OrderContainer.Location["TabKey"] = TabyKey;
            OrderContainer.Location["ContainerName"] = frmAddTab->TabName;
            OrderContainer.Location["TabName"] = frmAddTab->TabName;

            // setting return value here, in case if the execution did not go through the next section
            Retval = mrOk;

            if (!TGlobalSettings::Instance().DisableConfirmationOnSave)
            {
                OrderContainer.Values.push_back(TSaveOrdersTo::StringValuePair("Total", InitialMoney.GrandTotal));
                Currency Balance = TDBTab::GetTabBalance(DBTransaction, TabyKey);
                OrderContainer.Values.push_back(TSaveOrdersTo::StringValuePair("Balance", Balance));
                OrderContainer.Values.push_back(TSaveOrdersTo::StringValuePair("New Balance", Balance + InitialMoney.GrandTotal));

                TfrmConfirmOrder* frmConfirmOrder = new TfrmConfirmOrder(this, OrderContainer);
                if (!TGlobalSettings::Instance().DisableReceiptOnConfirmation)
                {
                    SetReceiptPreview(DBTransaction, frmConfirmOrder->ReceiptDisplay, OrderContainer.Location["TMMTabType"], OrderContainer.Location["ContainerName"],
                    OrderContainer.Location["TabName"], OrderContainer.Location["PartyName"], OrderContainer.Location["TabKey"], OrderContainer.Location["SelectedTable"],
                    OrderContainer.Location["SelectedSeat"], OrderContainer.Location["RoomNumber"]);
                }

                Retval = frmConfirmOrder->ShowModal();
                delete frmConfirmOrder;
                frmConfirmOrder = NULL;
            }
        }
    }
  catch(Exception & E)
  {
	 TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
  }
    return Retval;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Transaction Audit Screen
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void TfrmSelectDish::createTransactionAuditScreen()
{
    frmTransactionAuditScreen = TfrmTransactionAuditScreen::Create<TfrmTransactionAuditScreen>(this);
}
//.............................................................................
void TfrmSelectDish::resetTransactionAuditScreen()
{
    frmTransactionAuditScreen->Reset();
}
//.............................................................................
void TfrmSelectDish::OpenTransactionAuditScreen()
{
    TMMContactInfo TempUserInfo;
	bool Allowed = false;
	TempUserInfo.Clear();

	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	DBTransaction.StartTransaction();

	std::auto_ptr<TContactStaff>Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this, DBTransaction, TempUserInfo, CheckTransactionAuditAccess);
	if (Result == lsAccepted)
	{
		Allowed = true;
	}
	else if (Result == lsDenied)
	{
		MessageBox("You do not have access rights to Transactions Live Audit.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}
	DBTransaction.Commit();

    if(Allowed)
    {
        frmTransactionAuditScreen->ShowModal();
    }
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::AssignremovedTaxesList()
{

  TStringList *RemoveTaxes = new TStringList;

  for(int i=0; i < ChitNumber.RemovedTaxList->Count; i++)
  {
     RemoveTaxes->Add(ChitNumber.RemovedTaxList->Strings[i]);
  }

  for(int i=0; i < ChitNumber.RemovedServiceChargeList->Count; i++)
  {
     RemoveTaxes->Add(ChitNumber.RemovedServiceChargeList->Strings[i]);
  }
    for (UINT iSeat = 0; iSeat < SeatOrders.size(); iSeat++)
    {
        for (int i = 0; i < SeatOrders[iSeat]->Orders->Count; i++)
        {
            TItemComplete *Order = (TItemComplete*)SeatOrders[iSeat]->Orders->Items[i];
            Order->RemovedTaxes = RemoveTaxes;

            for( int j = 0; j < Order->SubOrders->Count; j++ )
            {
                TItemCompleteSub *SideItem = Order->SubOrders->SubOrderGet(j);
                SideItem->RemovedTaxes = RemoveTaxes;
            }
        }
    }
}
// ---------------------------------------------------------------------------
bool TfrmSelectDish::CheckItemsPrintCancel()
{
   bool prelimCheck = true;

    for (UINT i = 0; i < SeatOrders.size(); i++)
    {
      for (int k = 0; k < SeatOrders[i]->Orders->Count; k++)
      {
          TItemComplete *itemmain = SeatOrders[i]->Orders->Items[k];
          if(itemmain->PrintCancel)
          {
              prelimCheck = false;
          }
          for (int l = 0; l < itemmain->SubOrders->Count; l++)
          {
           TItemComplete *subitem = (TItemComplete *)itemmain->SubOrders->Items[l];
           if(subitem->PrintCancel)
           {
              prelimCheck = false;
           }
        }
      }
    }
    return prelimCheck;
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::SaveRemoveItems(AnsiString& User)
{
    for (UINT i = 0; i < SeatOrders.size(); i++)
    {
      for (int k = 0; k < SeatOrders[i]->Orders->Count; k++)
      {
          TItemComplete *itemmain = SeatOrders[i]->Orders->Items[k];
          if(itemmain->PrintCancel)
          {
              CancelItemsBill(itemmain, User,true);
          }
      }
    }
    sec_ref = 0;
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::CancelItemsBill(TItemComplete *cancelitem, AnsiString& User, bool isFullItemRemove)  //Mm-4327
{
    try
    {
        int qty = cancelitem->GetQty();
        Currency decimalOffset = cancelitem->GetQty() - qty;
        Currency reducedQty = (qty > 1) ? 1.0 + decimalOffset : cancelitem->GetQty();
        Currency reducedPrice =  cancelitem->BillCalcResult.FinalPrice * reducedQty/cancelitem->GetQty();
        if(isFullItemRemove|| cancelitem->WeightedPrice.WeightedItem)
        {
            reducedQty = cancelitem->GetQty();
            reducedPrice= cancelitem->BillCalcResult.FinalPrice ;
        }


        int val = 0;
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();

        if(sec_ref == 0)
        {
          sec_ref = TDBSecurity::GetNextSecurityRef(DBTransaction);
          TDBSecurity::ProcessSecurity(DBTransaction, sec_ref, TDeviceRealTerminal::Instance().User.ContactKey,
          SecurityTypes[secCheckRemoval], TDeviceRealTerminal::Instance().User.Name,
          TDeviceRealTerminal::Instance().User.Initials, Now(),TDeviceRealTerminal::Instance().ID.Name);
        }


		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        TIBSQL *IBInsertQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
        IBInsertQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_CANCELITEMS, 1) FROM RDB$DATABASE ";
		IBInternalQuery->ExecQuery();
		val = IBInternalQuery->Fields[0]->AsInteger;
		IBInternalQuery->Close();


		IBInsertQuery->SQL->Text =
		"INSERT INTO CANCELITEMS (" "CANCELITEMS_KEY, " "ITEM_NAME, " "QTY, " "SIZE_NAME, " "MENU_NAME, " "PRICE, "
		"TIME_STAMP, " "TERMINAL_NAME, " "ORDER_LOCATION, " "SECURITY_REF, " "ZED_PRINT, " "REMOVE_USER) " "VALUES ("
		":CANCELITEMS_KEY, " ":ITEM_NAME, " ":QTY, " ":SIZE_NAME, " ":MENU_NAME, " ":PRICE, " ":TIME_STAMP, " ":TERMINAL_NAME, "
		":ORDER_LOCATION, " ":SECURITY_REF, " ":ZED_PRINT, " ":REMOVE_USER) ";

		IBInsertQuery->ParamByName("CANCELITEMS_KEY")->AsString = val;
		IBInsertQuery->ParamByName("ITEM_NAME")->AsString = cancelitem->Item;
		IBInsertQuery->ParamByName("QTY")->AsCurrency = reducedQty;
		IBInsertQuery->ParamByName("SIZE_NAME")->AsString = cancelitem->Size;
		IBInsertQuery->ParamByName("MENU_NAME")->AsString = cancelitem->MenuName;
        IBInsertQuery->ParamByName("PRICE")->AsCurrency = fabs(reducedPrice);
		IBInsertQuery->ParamByName("TIME_STAMP")->AsDateTime = Now();
		IBInsertQuery->ParamByName("TERMINAL_NAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
		IBInsertQuery->ParamByName("ORDER_LOCATION")->AsString = TDeviceRealTerminal::Instance().ID.Location;
		IBInsertQuery->ParamByName("SECURITY_REF")->AsInteger = sec_ref;
        IBInsertQuery->ParamByName("ZED_PRINT")->AsInteger = 0;
        IBInsertQuery->ParamByName("REMOVE_USER")->AsString = User;
        IBInsertQuery->ExecQuery();
        IBInsertQuery->Close();

        for (int i = 0; i < cancelitem->SubOrders->Count; i++)
        {

            TItemCompleteSub *SubOrder = cancelitem->SubOrders->SubOrderGet(i);

            if(SubOrder->PrintCancel)
            {
                IBInternalQuery->ExecQuery();
                val = IBInternalQuery->Fields[0]->AsInteger;
                IBInternalQuery->Close();
                IBInsertQuery->ParamByName("CANCELITEMS_KEY")->AsString = val;
                IBInsertQuery->ParamByName("ITEM_NAME")->AsString = SubOrder->Item;
                IBInsertQuery->ParamByName("QTY")->AsCurrency = SubOrder->GetQty();
                IBInsertQuery->ParamByName("SIZE_NAME")->AsString = SubOrder->Size;
                IBInsertQuery->ParamByName("MENU_NAME")->AsString = SubOrder->MenuName;
                IBInsertQuery->ParamByName("PRICE")->AsCurrency = fabs(SubOrder->BillCalcResult.FinalPrice);
                IBInsertQuery->ParamByName("TIME_STAMP")->AsDateTime = Now();
                IBInsertQuery->ParamByName("TERMINAL_NAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
                IBInsertQuery->ParamByName("ORDER_LOCATION")->AsString = TDeviceRealTerminal::Instance().ID.Location;
                IBInsertQuery->ParamByName("SECURITY_REF")->AsInteger = sec_ref;
                IBInsertQuery->ParamByName("ZED_PRINT")->AsInteger = 0;
                IBInsertQuery->ParamByName("REMOVE_USER")->AsString = User;
                IBInsertQuery->ExecQuery();
                IBInsertQuery->Close();
            }

        }
        DBTransaction.Commit();
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::CaptureDeliveryTime()
{
   TfrmSelectDateTime *frmSelectDateTime = new TfrmSelectDateTime(this);
   frmSelectDateTime->MinutesToAdd = ChitNumber.AddMinutes;
   if(frmSelectDateTime->ShowModal()==mrOk)
    ChitNumber.DeliveryTime = frmSelectDateTime->SelectedTime;
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::AddSecurityOnMoveOrderToTab(Database::TDBTransaction &DBTransaction,TList *OrderList)
{
  TDateTime TransferTime = Now();
   for (int i = 0; i < OrderList->Count; i++)
    {
            TItemComplete *Order = (TItemComplete*)OrderList->Items[i];
            TSecurityReference *SecRef = new TSecurityReference;
            SecRef->UserKey = TDeviceRealTerminal::Instance().User.ContactKey;
            SecRef->Event =   SecurityTypes[secTransferredPrelim];
            SecRef->From =    SouceTableForDelayedPayment;
            SecRef->Note =    "";
            SecRef->To =      TDBTab::GetTabName(DBTransaction,Order->TabKey );
            SecRef->TimeStamp = TransferTime;
            Order->Security->SecurityAddUniqueRef(DBTransaction,Order->OrderKey,SecRef);
            for (int j = 0; j < Order->SubOrders->Count; j++)
            {
                TItemCompleteSub *SubOrder = Order->SubOrders->SubOrderGet(j);
                if (SubOrder)
                {
                    TSecurityReference *SecRef = new TSecurityReference;
                    SecRef->UserKey = TDeviceRealTerminal::Instance().User.ContactKey;
                    SecRef->Event =   SecurityTypes[secTransferredPrelim];
                    SecRef->From =    SouceTableForDelayedPayment;
                    SecRef->Note =    "";
                    SecRef->To =      TDBTab::GetTabName(DBTransaction,Order->TabKey );
                    SecRef->TimeStamp = TransferTime;
                    SubOrder->Security->SecurityAddUniqueRef(DBTransaction,SubOrder->OrderKey,SecRef);
                }
            }
    }
}
// ---------------------------------------------------------------------------
TModalResult TfrmSelectDish::GetOrderContainerForTab(Database::TDBTransaction &DBTransaction, TSaveOrdersTo &OrderContainer)
{
	TModalResult Retval = mrOk;
	try
	{
		if(ChitNumber.IsAutoSaveOnTab)
		{
			Retval = setupAutoSaveTabContainer( DBTransaction, OrderContainer );
		}
		else
		{
		  //	do
		 //	{
				/*OrderContainer.Location["SelectedTable"] = 0;
				OrderContainer.Location["SelectedSeat"] = 0;
				OrderContainer.Location["RoomNumber"] = 0;    */
				if (GetTabContainerForTab(DBTransaction, TMMTabType(OrderContainer.Location["TMMTabType"]), OrderContainer) != mrOk) //tab type tab key
				{
					Retval = mrAbort;
				}


		//	}
		  //	while (Retval == mrAbort);
		}
	}
	catch(Exception & E)
	{
		MessageBox(E.Message, "Error", MB_OK + MB_ICONERROR);
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		Retval = mrCancel;
	}
	return Retval;
}
// ---------------------------------------------------------------------------
TModalResult TfrmSelectDish::GetTabContainerForTab(Database::TDBTransaction &DBTransaction, TMMTabType TabType, TSaveOrdersTo &OrderContainer)
{
	TModalResult Retval = mrOk;
	try
	{
		int TabKey = OrderContainer.Location["TabKey"];
		if (!TabPINOk(DBTransaction, TabKey))
		{
			Retval = mrAbort;
		}
		if (Retval == mrOk)
		{
			if (!TGlobalSettings::Instance().DisableConfirmationOnSave)
			{
				OrderContainer.Values.push_back(TSaveOrdersTo::StringValuePair("Total", InitialMoney.GrandTotal));
				Currency Balance = TDBTab::GetTabBalance(DBTransaction, TabKey);
				OrderContainer.Values.push_back(TSaveOrdersTo::StringValuePair("Balance", Balance));
				OrderContainer.Values.push_back(TSaveOrdersTo::StringValuePair("New Balance", Balance + InitialMoney.GrandTotal));

                TfrmConfirmOrder* frmConfirmOrder = new TfrmConfirmOrder(this, OrderContainer);
				if (!TGlobalSettings::Instance().DisableReceiptOnConfirmation)
				{
					SetReceiptPreview(DBTransaction, frmConfirmOrder->ReceiptDisplay, OrderContainer.Location["TMMTabType"], OrderContainer.Location["ContainerName"],
					OrderContainer.Location["TabName"], OrderContainer.Location["PartyName"], OrderContainer.Location["TabKey"], OrderContainer.Location["SelectedTable"],
					OrderContainer.Location["SelectedSeat"], OrderContainer.Location["RoomNumber"]);
				}
                Retval =  mrOk;
                delete frmConfirmOrder;
                frmConfirmOrder = NULL;
			}
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		Retval = mrCancel;
	}
	return Retval;
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::SaveTabData(TSaveOrdersTo &OrderContainer)
{
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	DBTransaction.StartTransaction();
	bool Proceed = GetOrderContainerForTab(DBTransaction, OrderContainer) == mrOk;
	DBTransaction.Commit();

	if (Proceed && OrderContainer.Location["TabKey"] > 0)
	{
		DBTransaction.StartTransaction();
		bool TabisLocked = TDBTab::GetTabLocked(DBTransaction, OrderContainer.Location["TabKey"]);
		AnsiString CreditMessage1 = "Reason :" + TDBTab::GetTabLockedReason(DBTransaction, OrderContainer.Location["TabKey"]);
		DBTransaction.Commit();

		if (TabisLocked)
		{
            Proceed = ShowTabLockedMessage(CreditMessage1);
		}
		else
		{
			DBTransaction.StartTransaction();
            Proceed = CheckTabIsInUse(DBTransaction, OrderContainer.Location["TabKey"]);
			DBTransaction.Commit();
		}
	}

	if (Proceed)
	{
		std::auto_ptr<TfrmProcessing>(frmProcessing)(TfrmProcessing::Create<TfrmProcessing>(this));
		frmProcessing->Message = "Posting Orders";
		frmProcessing->Show();

		bool DoProcessOrders = true;
		if (OrderContainer.Location["TMMTabType"] == TabNormal || OrderContainer.Location["TMMTabType"] == TabStaff || OrderContainer.Location["TMMTabType"] == TabMember)
		{
			DBTransaction.StartTransaction();

            //check credit limit exceeds
            DoProcessOrders = CheckCreditLimitExceeds(DBTransaction, OrderContainer.Location["TabKey"]);
			DBTransaction.Commit();
		}

		bool PaymentComplete = false;
		if (DoProcessOrders)
		{
			DBTransaction.StartTransaction();
			if (OrderContainer.PartyNameChanged)
			{
				if (OrderContainer.Location["TMMTabType"] == TabTableSeat)
				{
					TDBTables::SetPartyName(DBTransaction, OrderContainer.Location["SelectedTable"], OrderContainer.Location["PartyName"]);
				}
				else if (OrderContainer.Location["TMMTabType"] == TabRoom)
				{
					TDBRooms::SetPartyName(DBTransaction, OrderContainer.Location["RoomNumber"], OrderContainer.Location["PartyName"]);
				}
			}
			PaymentComplete = ProcessOrders(tbtnSave,
			DBTransaction,
			OrderContainer.Location["TabKey"], 				// Tab
			OrderContainer.Location["TMMTabType"], 			// Tab Type
			OrderContainer.Location["ContainerName"],
			OrderContainer.Location["TabName"],
			OrderContainer.Location["PartyName"],
			OrderContainer.Location["PrintPreLimReceipt"],	// Print Prelim Receipt.
			OrderContainer.Location["SelectedTable"],
			OrderContainer.Location["SelectedSeat"],
			OrderContainer.Location["RoomNumber"],
			OrderContainer.Location["BillOff"]);

            TStringList* logList = new TStringList();
            logList->Clear();

			if (PaymentComplete)
			{
                logList->Add("Trabsaction commit of SaveTabData(().");
                TSaveLogs::RecordFiscalLogs(logList);

				DBTransaction.Commit();
				ResetPOS();

			}
			else
			{
				DBTransaction.Rollback();
                logList->Add("Trabsaction rollback of SaveTabData(().");
                TSaveLogs::RecordFiscalLogs(logList);
			}
            delete logList;
            logList = NULL;
		}
		frmProcessing->Close();
		if (!OrdersPending())
		{
			if (OrderContainer.Location["BillOff"])
			{
				frmProcessing->Close();

                TfrmBillGroup* frmBillGroup  = new  TfrmBillGroup(this, TDeviceRealTerminal::Instance().DBControl);
				frmBillGroup->ResetForm();
				if (OrderContainer.Location["TMMTabType"] == TabMember)
				{
					DBTransaction.StartTransaction();
					TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetContactDetails(DBTransaction, OrderContainer.Location["MemberKey"], frmBillGroup->TempUserInfo);
					frmBillGroup->ApplyMembership(DBTransaction, frmBillGroup->TempUserInfo);
					DBTransaction.Commit();
				}
				frmBillGroup->ResetSelection();
				frmBillGroup->CurrentDisplayMode = OrderContainer.Location["TMMDisplayMode"];
				frmBillGroup->CurrentTabType = OrderContainer.Location["TMMTabType"];
				frmBillGroup->CurrentSelectedTab = OrderContainer.Location["TabKey"];
				frmBillGroup->CurrentTable = OrderContainer.Location["SelectedTable"];
				frmBillGroup->CurrentRoom = OrderContainer.Location["RoomNumber"];
				DBTransaction.StartTransaction();
				std::vector<TPatronType> selectedTablePatrons = TDBTables::GetPatronCount(DBTransaction, frmBillGroup->CurrentTable);
				int patronCount = GetCount(selectedTablePatrons);

				if((TGlobalSettings::Instance().PromptForPatronCount)&& patronCount > 0)
				{
					frmBillGroup->PatronTypes = selectedTablePatrons;
				}
				DBTransaction.Commit();

				if (OrderContainer.Location["TabKey"] != 0)
				{
					const int tab_key =
					OrderContainer.Location["TabKey"];
					DBTransaction.StartTransaction();
					frmBillGroup->AddToSelectedTabs(DBTransaction,tab_key);
					//frmBillGroup->SplitItemsInSet(DBTransaction,tab_key);
					DBTransaction.Commit();
				}
				frmBillGroup->ShowModal();
				setPatronCount(  frmBillGroup->PatronCount );
                delete frmBillGroup;
                frmBillGroup = NULL;
			}
		}
	}
	else
	{
		bool OrdersTaken = false;
		for (UINT e = 0; e < SeatOrders.size(); e++)
		{
			if (SeatOrders[e]->Orders->Count > 0)
			{
				OrdersTaken = true;
				break;
			}
		}
		if (OrdersTaken)
		{
			if (MessageBox("Delete unsent orders?", "Warning", MB_YESNO + MB_ICONQUESTION) == IDYES)
			{
				lbDisplay->Clear();
                patronsStore.clear();
				for (UINT f = 0; f < SeatOrders.size(); f++)
				{
					try
					{
						TItemMinorComplete *item;
						for (int j = 0; j < SeatOrders[f]->Orders->Count; j++)
						{
							item = SeatOrders[f]->Orders->Items[j];
							item->ReturnToAvailability();
							delete item;
                            item = NULL;
						}
						for (int h = 0; h < SeatOrders[f]->Orders->PrevCount; h++)
						{
							item = SeatOrders[f]->Orders->Items[h];
							item->ReturnToAvailability();
							delete item;
                            item = NULL;
						}
					}
					__finally
					{
						SeatOrders[f]->Orders->Clear();
						SeatOrders[f]->Orders->ClearPrev();
						SeatOrders[f]->Orders->AppliedMembership.Clear();
					}
				}
				DBTransaction.StartTransaction();
				TDBTab::ReleaseTab(DBTransaction, TDeviceRealTerminal::Instance().ID.Name, 0);
				DBTransaction.Commit();
				ChitNumber.Clear();
				CurrentTender = 0;
				tbtnTender->Caption = "Tender";
				tbtnCashSale->Caption = "Cash Sale";
			}
		}
		else
		{
			ChitNumber.Clear();
			DBTransaction.StartTransaction();
			TDBTab::ReleaseTab(DBTransaction, TDeviceRealTerminal::Instance().ID.Name, 0);
			DBTransaction.Commit();
			CurrentTender = 0;
			tbtnTender->Caption = "Tender";
			tbtnCashSale->Caption = "Cash Sale";
		}
	}
	RedrawItems();
}
// ---------------------------------------------------------------------------
bool TfrmSelectDish::SaveTransactionDetails(UnicodeString &pay_type)
{
    bool RetVal = true;

    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
    DBTransaction.StartTransaction();

    TPaymentTransaction PaymentTransaction(DBTransaction);
    PaymentTransaction.Type = eTransQuickSale;
    TDeviceRealTerminal::Instance().PaymentSystem->PaymentsReload(PaymentTransaction);
    //MM 4775
    TPayment *CashPayment;

        CashPayment = PaymentTransaction.PaymentFind(pay_type);

    int  tabkey=    TDeviceRealTerminal::Instance().PaymentSystem->GetPaymentTabName(DBTransaction,CashPayment->Name);
    AnsiString  tabName   = TDBTab::GetTabName(DBTransaction,tabkey);
	if(tabkey==0 && tabName=="")
	return true;
    TSaveOrdersTo OrderContainer;

    if(tabkey>0 && tabName!="")
    {
          if(!IsSubSidizeOrderCancil)
        SubsidizedDiscountApply(tabkey);
        OrderContainer.Location["TabKey"] = tabkey;
        OrderContainer.Location["TMMTabType"] = TabNormal;
        OrderContainer.Location["TMMDisplayMode"] = eTabs;
        OrderContainer.Location["ContainerName"] = tabName;
        OrderContainer.Location["TabName"] = tabName;
        OrderContainer.Location["PartyName"] = "";
        OrderContainer.Location["SelectedSeat"] = 0;
        OrderContainer.Location["SelectedTable"] = 0;
        OrderContainer.Location["RoomNumber"] = 0;
          if(!IsSubSidizeOrderCancil)
		{
			SaveTabData(OrderContainer);
            ModalResult = mrCancel;
			IsSubSidizeProcessed=false;
		}
		else
		{
            	ManagerDiscount->ClearDiscounts(SeatOrders[SelectedSeat]->Orders->List);
			    RedrawSeatOrders();
            	TotalCosts();
	  //	AfterSelectedItemChanged.Occured();

        }
         DBTransaction.Commit();

        RetVal = false;
    }
	IsSubSidizeProcessed=false;
     IsSubSidizeOrderCancil=false;
      return RetVal;
}
// ---------------------------------------------------------------------------
bool TfrmSelectDish::SubsidizedDiscountApply(int tabkey)
{
    IsSubSidizeOrderCancil=false;
	IsSubSidizeProcessed=true;
	IsSubSidizeProfileExist=false;
	bool IsTabDiscountApplied=false;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	DBTransaction.StartTransaction();
	/***************************************************************************/
	Info.Clear();
	Info=  TDBTab::GetDiscountDetails(DBTransaction, tabkey, Info);
	for (std::set <int> ::iterator ptrDiscountKey = Info.AutoAppliedDiscounts.begin();
	ptrDiscountKey != Info.AutoAppliedDiscounts.end(); ptrDiscountKey++)
	{   IsSubSidizeProfileExist=true;
		TDiscount CurrentDiscount;
		std::auto_ptr<TList> allOrders(new TList());
		GetAllOrders(allOrders.get());
		CurrentDiscount.DiscountKey = *ptrDiscountKey;
		ManagerDiscount->GetDiscount(DBTransaction, *ptrDiscountKey, CurrentDiscount);
		ManagerDiscount->ClearDiscount(SeatOrders[SelectedSeat]->Orders->List, CurrentDiscount);
		ApplyDiscount(DBTransaction, *ptrDiscountKey, SeatOrders[SelectedSeat]->Orders->List);
		IsTabDiscountApplied=true;
	}
	DBTransaction.Commit();
	if(!IsTabDiscountApplied)
	return false;

	ParkSaletemp(0);
	ParkSaletemp(0);
	try
	{
		SubsidizedQuantityApply(true);
		// IsSubSidizeOrderCancil=false;
        double orderValue = 0;
        for(int i = 0; i < SeatOrders[SelectedSeat]->Orders->Count; i++)
        {
            TItemComplete *item = (TItemComplete*)SeatOrders[SelectedSeat]->Orders->Items[i];
            item->RunBillCalculator();
            orderValue += (double)item->FinalPrice_BillCalc();
            for(int j = 0; j < item->SubOrders->Count; j++)
            {
                TItemComplete *subItem = ((TItemComplete*)item->SubOrders->Items[j]);
                subItem->RunBillCalculator();
                orderValue += (double)subItem->FinalPrice_BillCalc();
            }
        }
        if(orderValue > 0)
		    tbtnTenderClick(tbtnTender);
		IsSubSidizeProfileExist=false;



	for (UINT i = 0; i < SeatOrders.size(); i++)
	{
		try
		{
			while (SeatOrders[i]->Orders->Count != 0)
			{
				SeatOrders[i]->Orders->Delete(0);
			}
			// Prev Items are not Stored Delete them.
			while (SeatOrders[i]->Orders->PrevCount != 0)
			{
				delete SeatOrders[i]->Orders->PrevItems[0];
				SeatOrders[i]->Orders->DeletePrev(0);
			}
		}
		__finally
		{
			SeatOrders[i]->Orders->Clear();
			SeatOrders[i]->Orders->ClearPrev();
			SeatOrders[i]->Orders->AppliedMembership.Clear();
		}
	}
	RedrawSeatOrders();
	ResetPOS();

		/***************/
		/* if(IsSubSidizeOrderCancil)
	ParkSaletemp(temptabkey);
else */
		ParkSaletemp(temptabkey-1);


	}

    __finally
			{
      // ParkedSales->Delete(DBTransaction, tabkey);
    // ParkedSales->Delete(DBTransaction, tabkey+1);
     }
       // if(!PaymentComplete)
     //  {
       //  IsTabDiscountApplied =false;

	//  } else
	//{
	if(!IsSubSidizeOrderCancil)
	SubsidizedQuantityApply(false);
	// }

	IsSubSidizeProcessed=false;
	return IsTabDiscountApplied;
	/***************************************************************************/






}
// ---------------------------------------------------------------------------
bool TfrmSelectDish::ParkSaletemp(int tabkey)
{
	OrderHeld = false;
	if (!OrdersPending())
	{
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();
		if (ParkedSales->GetCount(DBTransaction) > 0)
		{
			try
			{

				TParkedSale *Sale = ParkedSales->GetParkedSale(DBTransaction, tabkey);
				if (Sale != NULL)
				{

					Sale->AssignParkedSale(SeatOrders);
			   //		ParkedSales->Delete(DBTransaction, tabkey-1);
					ParkedSales->Delete(DBTransaction, tabkey);
					ParkedSales->Delete(DBTransaction, tabkey+1);
					SelectedSeat = Sale->SelectedSeat;
					SelectedTable = Sale->SelectedTable;
					SelectedTabContainerName = Sale->SelectedTabContainerName;
					SelectedParty = Sale->SelectedParty;
					if(SelectedTable > 0)
					{
						//Get the patrons for the current selected table, verify if it is already keyed in
						std::vector<TPatronType> selectedTablePatrons = TDBTables::GetPatronCount(DBTransaction, SelectedTable);
						int patronCount = GetCount(selectedTablePatrons);
					}

					if (Sale->StoreMembership)
					{
						Membership.Assign(Sale->Membership);
					}

					delete Sale;
                    Sale = NULL;
				}
			}
			__finally
			{

				delete frmParkedSales;
				frmParkedSales = NULL;

			}
		}
		if (SelectedSeat != 0)
		{
			SetSelectedSeat();
		}

		if (TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->CardOk)
		{
			OnSmartCardInserted(NULL);
		}
		else
		{
			OnSmartCardRemoved(NULL);
		}

		RefreshSeats();
		RedrawSeatOrders();
		TotalCosts();
		UpdateExternalDevices();
		DBTransaction.Commit();
	}
	else
	{ // Park the current sale.
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();

		TParkedSale *Sale = new TParkedSale;
		if (TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->Enabled)
		{
			if (TDeviceRealTerminal::Instance().ManagerMembership->AllowSmartCardNotPresentMemberTransactions)
			{
				Sale->StoreMembership = true;
			}
		}
		else
		{
			Sale->StoreMembership = true;
		}

		if (Sale->StoreMembership)
		{
			Sale->Membership.Assign(Membership);
		}
		Sale->SetSale(TDeviceRealTerminal::Instance().User.Name, SeatOrders);
		Sale->Time = Now();
		Sale->SelectedSeat = SelectedSeat;
		Sale->SelectedTable = SelectedTable;
		Sale->SelectedTabContainerName = SelectedTabContainerName;
		Sale->SelectedParty = SelectedParty;
		settempParkedSale( Sale );
		lbDisplay->Clear();
		TDBSaleTimes::VoidSaleTime(DBTransaction, CurrentTimeKey);
		CurrentTimeKey = 0;
		DBTransaction.Commit();
	}
	AutoLogOut();
    DisplayRoomNoUI();
	NagUserToSelectChit();
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::settempParkedSale( TParkedSale* inSale )
{
	//if( TGlobalSettings::Instance().EnableParkedSales )
	//{
		Database::TDBTransaction transaction( TDeviceRealTerminal::Instance().DBControl );

		transaction.StartTransaction();
		temptabkey= ParkedSales->SettempParkedSale( transaction, inSale );
		transaction.Commit();
		customerDisplayReset();
		customerDisplayResetPOSTotals();
	//}
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::SubsidizedQuantityApply(bool IsBilledQty)
 {    Currency BilledQty=0;

    	for (int i = 0; i < SeatOrders[SelectedSeat]->Orders->Count; i++)
	{
		Currency ActualBilledPrice=SeatOrders[SelectedSeat]->Orders->Items[i]->Price() ;
		Currency ActualBilledDiscount=SeatOrders[SelectedSeat]->Orders->Items[i]->TotalDiscount_BillCalc() ;
         Currency ActualBilledSurcharge=SeatOrders[SelectedSeat]->Orders->Items[i]->TotalSurchargeSides_BillCalc() ;

		Currency BilledPrice= ActualBilledPrice+ActualBilledDiscount-ActualBilledSurcharge;
		Currency TabPrice= ActualBilledPrice-BilledPrice;
		if(BilledPrice==0)
		{
			BilledQty=0;
		}
		else
		{
			BilledQty=  SeatOrders[SelectedSeat]->Orders->Items[i]->GetQty()*(BilledPrice)/ActualBilledPrice;
		}

		Currency TabBilledQty=     SeatOrders[SelectedSeat]->Orders->Items[i]->GetQty()-BilledQty;

		if(BilledPrice>ActualBilledPrice)
		{
		TabBilledQty= SeatOrders[SelectedSeat]->Orders->Items[i]->GetQty();
			BilledQty=0;
		}
		if(IsBilledQty)
		SeatOrders[SelectedSeat]->Orders->Items[i]->SetQtyCustom(BilledQty)  ;
        else
          {
          	SeatOrders[SelectedSeat]->Orders->Items[i]->SetQtyCustom(TabBilledQty)  ;

  	SeatOrders[i]->Orders->AppliedMembership.Clear();
				Membership.Clear();
          }

        SeatOrders[SelectedSeat]->Orders->Items[i]->DiscountsClear();

	}
 }
// ---------------------------------------------------------------------------
void TfrmSelectDish::CheckMandatoryMembershipCardSetting(TObject * Sender)
{
   TTouchBtn* CurrentButton = (TTouchBtn*) Sender;
   if(TGlobalSettings::Instance().MandatoryMembershipCard )
   {
      CurrentButton->ButtonColor = clRed;
   }
   else
   {
      CurrentButton->ButtonColor = clNavy;
   }

}
// ---------------------------------------------------------------------------
void TfrmSelectDish::GetItemsFromTable(int seatkey, TGridButton *GridButton, bool isCalledFromGuestSeat)
{
    try
    {
        seatkey += 1;
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction.StartTransaction();

        TTableSeat TableSeat;
        // Find its tab.
        __int64 NewTabKey = TDBTables::GetTabKey(DBTransaction, SelectedTable, seatkey);
        bool isTabLocked = false;
        if (NewTabKey != 0)
        {
            if (TDBTab::LockTab(DBTransaction, TDeviceRealTerminal::Instance().ID.Name, NewTabKey))
            {   
                // Unlock Old Tab.
                TTableSeat OldTableSeat;
                OldTableSeat.TableNo = SelectedTable;
                OldTableSeat.SeatNo = SelectedSeat;
                __int64 OldTabKey = TDBTables::GetTabKey(DBTransaction, SelectedTable, SelectedSeat);
                if (OldTabKey != 0)
                {
                    TDBTab::ReleaseTab(DBTransaction, TDeviceRealTerminal::Instance().ID.Name, OldTabKey);
                }
                // Move on to New (Locked)Seat.
                SelectedSeat = seatkey;
                isTabLocked = true;
            }
            else
            {    
                GridButton->Latched = false;
                GridButton->Color = ButtonColors[BUTTONTYPE_LOCKED][ATTRIB_BUTTONCOLOR];

                AnsiString LockedBy = TDBTab::GetLocker(DBTransaction, NewTabKey);
                if (MessageBox("This " + TGlobalSettings::Instance().SeatLabel + "/Tab is in use by " + LockedBy + "\rDo you wish to override this lock", "Warning",
                        MB_YESNO + MB_ICONWARNING) == ID_YES)
                {
                    TDBTab::ReleaseTab(DBTransaction, TDeviceRealTerminal::Instance().ID.Name, NewTabKey);
                    if (TDBTab::LockTab(DBTransaction, TDeviceRealTerminal::Instance().ID.Name, NewTabKey))
                    {
                        __int64 OldTabKey = TDBTables::GetTabKey(DBTransaction, SelectedTable, SelectedSeat);
                        if (OldTabKey != 0)
                        {
                            TDBTab::ReleaseTab(DBTransaction, TDeviceRealTerminal::Instance().ID.Name, OldTabKey);
                        }
                        // Move on to New (Locked)Seat.
                        GridButton->Latched = true;
                        GridButton->Color = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
                        SelectedSeat = seatkey;
                    }
                    else
                    {
                        MessageBox("Unable to Unlock " + TGlobalSettings::Instance().SeatLabel + "/Tab Try again Later", "Warning", MB_OK + MB_ICONWARNING);
                    }
                }
            }
        }
        else
        {
            SelectedSeat = seatkey;
        }
        DBTransaction.Commit();

        GetNextAvailableSeatAndLoadOrders(isCalledFromGuestSeat);
        // Check for orders on Seat 0 and move them to selected seat.
         if (SeatOrders[0]->Orders->Count != 0 && SelectedSeat != 0)
        {
            while (SeatOrders[0]->Orders->Count != 0)
            {
                SeatOrders[SelectedSeat]->Orders->Add(SeatOrders[0]->Orders->Items[0], SeatOrders[0]->Orders->Items[0]->ItemOrderedFrom);
                SeatOrders[0]->Orders->Delete(SeatOrders[0]->Orders->Items[0]);
            }
            SeatOrders[0]->Orders->Clear();
        }

        if (SeatOrders[0]->Orders->AppliedMembership.ContactKey != 0)
        {
            SeatOrders[SelectedSeat]->Orders->AppliedMembership = SeatOrders[0]->Orders->AppliedMembership;
            SeatOrders[0]->Orders->AppliedMembership.Clear();
        }
        RedrawSeatOrders();
        TotalCosts();
        UpdateExternalDevices();
        CloseSidePanel();
        if(lbDisplay->ItemIndex == -1 && lbDisplay->Count >0 )
        {
            isTabLocked = false;
        }

        if(SeatOrders[SelectedSeat]->isDefaultGuest && isCalledFromGuestSeat)
            SeatOrders[SelectedSeat]->isDefaultGuest = false;

        if(( !SeatOrders[SelectedSeat]->isDefaultGuest && SeatOrders[SelectedSeat]->RoomNumber.Trim() == "") && isCalledFromGuestSeat
                    && !SeatOrders[SelectedSeat]->wasGuestSelected)
        {
            DisplayRoomNoUI();
        }
        if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot //&& !SeatOrders[SelectedSeat]->isDefaultGuest //SeatOrders[SelectedSeat]->RoomNumber == ""
                     && selectedRoomNumberStr != "" && TGlobalSettings::Instance().EnableCustomerJourney )
        {
            if(SeatOrders[SelectedSeat]->RoomNumber.Trim() != "" && isCalledFromGuestSeat)
            {
                GridButton->Caption = SeatOrders[SelectedSeat]->RoomNumber;
                if(SeatOrders[SelectedSeat]->Orders->Count)
                {
                    SeatOrders[SelectedSeat]->Orders->pmsAccountDetails.RoomNumber = SeatOrders[SelectedSeat]->Orders->Items[0]->RoomNoStr;
                    SeatOrders[SelectedSeat]->Orders->pmsAccountDetails.FirstName = SeatOrders[SelectedSeat]->Orders->Items[0]->FirstName;
                    SeatOrders[SelectedSeat]->Orders->pmsAccountDetails.LastName = SeatOrders[SelectedSeat]->Orders->Items[0]->LastName;
                }
            }
            else if(isCalledFromGuestSeat && ((!SeatOrders[SelectedSeat]->isDefaultGuest ) ))  //|| (SeatOrders[SelectedSeat]->RoomNumber.Trim() == "" && SelectedSeat == 1)
            {
                DisplayRoomNoUI();
                GridButton->Caption = selectedRoomNumberStr;
            }
            RedrawSeatOrders();
        }
    }
    catch(Exception & E)
    {
         TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}
// ---------------------------------------------------------------------------
UnicodeString TfrmSelectDish::SetPartyNameOnChitSettings(Database::TDBTransaction &DBTransaction, AnsiString partyName, int tableNo,  bool setPartyName)
{
    try
    {
        if(ChitNumber.Valid())
        {
           if(ChitNumber.IsCaptureCustomerNameAndPhone && ChitNumber.SaveCustomerName)
           {
              partyName = CustName;
              if(setPartyName)
              {
                TDBTables::SetPartyName(DBTransaction, tableNo, partyName);
              }
           }
           else
           {
              if(ChitNumber.IsCaptureCustomerDetails && ChitNumber.SaveMemberName)
              {
                 partyName = CustName;
                 if(setPartyName)
                 {
                   TDBTables::SetPartyName(DBTransaction, tableNo, partyName);
                 }
              }
              else
              {
                 if(ChitNumber.IsCaptureCustomerDetails && ChitNumber.SaveMemberAddress)
                 {
                    partyName = PrepareAddress(CustAddress);
                    if(setPartyName)
                    {
                      TDBTables::SetPartyName(DBTransaction, tableNo, partyName);
                    }
                 }
              }
           }
        }
    }
    catch(Exception & E)
    {
         TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
    return partyName;
}
// ---------------------------------------------------------------------------
UnicodeString TfrmSelectDish::PrepareAddress(UnicodeString str_name)
{
	AnsiString strname = str_name;
	UnicodeString temp_data;
	int name_len = str_name.Length();
	int data_strtidx = 0;

    temp_data = str_name.SubString(data_strtidx, 28);

    char* temp_char = new char[strname.Length()+1];
   	strcpy(temp_char, strname.c_str());

    for(int i = 28; i > 0; i--)
    {
       if(!IsAlpha(temp_char[i]))
       {
         temp_data = str_name.SubString(data_strtidx, i);
         break;
       }
    }
   return temp_data;
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::YesGoForSessionWithDC(int memPoints, AnsiString memberPoints,AnsiString memNo,int contactKey)
{
   TDrinkCommandData::Instance().ItemDetailsReceived.RegisterForEvent(OnItemDetailsReceived);
   TMMProcessingState State(Screen->ActiveForm, "Processing Transaction With DC Please Wait...", "Processing DC Transaction");
   TDeviceRealTerminal::Instance().ProcessingController.Push(State);
   if (memPoints > 0 )                                                         // Session can be made only when points are more than 0
   {
       std::auto_ptr<TInitializeDCSession> im(
                                               new TInitializeDCSession());
       im->StartSession(memPoints,memberPoints,memNo,contactKey);
   }
   TDeviceRealTerminal::Instance().ProcessingController.Pop();
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::OnItemDetailsReceived(TSystemEvents *Sender)
{
      PaintItemToDisplay(Sender->ItemKey, Sender->ItemSizeKey , Sender->QtyValue , Sender->BillValue);
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::RefreshMenu()
{
    if(TGlobalSettings::Instance().DeleteItemSizeAfterSale)
    {
        for (int i = 0; i < TDeviceRealTerminal::Instance().Menus->Current->Count; i++)
        {
            std::auto_ptr<TNetMessageMenuChanged> Request( new TNetMessageMenuChanged );
            Request->Broadcast         = true;
            Request->CompareToDataBase = false;
            TListMenu *Menu = TDeviceRealTerminal::Instance().Menus->Current->MenuGet(i);
            Request->Menu_Names[Menu->MenuName] = eMenuAddReplace;
            TDeviceRealTerminal::Instance().Menus->UpdateMenuChanged(TDeviceRealTerminal::Instance().DBControl,Request.get() );
        }
        TDeviceRealTerminal::Instance().Menus->SwapInNewMenus();
    }
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::HideSoldItems(Database::TDBTransaction &DBTransaction,TList *OrdersList)
{
    if(TGlobalSettings::Instance().DeleteItemSizeAfterSale)
         {
            for (int i = 0; i < OrdersList->Count; i++)
            {
                TItemComplete *Order = (TItemComplete*)OrdersList->Items[i];

                AnsiString sizeName= Order->Size.Trim();
                int itemKey=  Order->ItemKey;
                int menuKey=Order->MenuKey;

                TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
                TIBSQL *IBInsertQuery = DBTransaction.Query(DBTransaction.AddQuery());


                IBInternalQuery->Close();
                IBInsertQuery->SQL->Text =
                "UPDATE ITEMSIZE A  SET A.ISINVISIBLE= :ISINVISIBLE WHERE A.ITEM_KEY = :ITEM_KEY AND A.SIZE_NAME = :SIZE_NAME ";

                IBInsertQuery->ParamByName("ISINVISIBLE")->AsString = "T";
                IBInsertQuery->ParamByName("ITEM_KEY")->AsInteger = itemKey;
                IBInsertQuery->ParamByName("SIZE_NAME")->AsString = sizeName;
                IBInsertQuery->ExecQuery();

                IBInternalQuery->Close();
                 IBInsertQuery->SQL->Text =
                "UPDATE SIZES A  SET A.ISINVISIBLE= :ISINVISIBLE WHERE A.MENU_KEY = :MENU_KEY AND A.SIZE_NAME = :SIZE_NAME ";

                IBInsertQuery->ParamByName("ISINVISIBLE")->AsString = "T";
                IBInsertQuery->ParamByName("MENU_KEY")->AsInteger = menuKey;
                IBInsertQuery->ParamByName("SIZE_NAME")->AsString = sizeName;
                IBInsertQuery->ExecQuery();
            }
         }
}
// ---------------------------------------------------------------------------
Currency  TfrmSelectDish::VerifyPriceLevelPrice(Database::TDBTransaction &DBTransaction,int itemSizeKey,Currency Price)
{
    try
    {

         int noOfPriceLevelInDataBase;
         TIBSQL *IBInternalQuery2   = DBTransaction.Query( DBTransaction.AddQuery() );
         IBInternalQuery2->Close();
         IBInternalQuery2->SQL->Text =
                                         "SELECT "
                                             "INTEGER_VAL "
                                         "FROM "
                                              "VARIABLES "
                                        "WHERE "
                                              "VARIABLES_KEY = 5014; ";
           IBInternalQuery2->ExecQuery();
           noOfPriceLevelInDataBase= IBInternalQuery2->Fields[0]->AsInteger;

           if (noOfPriceLevelInDataBase==0)noOfPriceLevelInDataBase=2;

     if ( StrToInt(stHappyHour->Tag)>  noOfPriceLevelInDataBase  )
       {
            TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text =
                      "Select PRICELEVEL_KEY,PRICE from PRICELEVELITEMSIZE pls where "
                      " pls.ITEMSIZE_KEY = :ITEMSIZE_KEY and pls.PRICELEVEL_KEY = :PRICELEVEL_KEY ";

            IBInternalQuery->ParamByName("ITEMSIZE_KEY")->AsInteger = itemSizeKey;
            IBInternalQuery->ParamByName("PRICELEVEL_KEY")->AsInteger = 1;
            IBInternalQuery->ExecQuery();

            if(!IBInternalQuery->Eof)
            {
                  Price= IBInternalQuery->FieldByName("PRICE")->AsCurrency;

            }

       }else
       {

           TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text =
                      "Select PRICELEVEL_KEY,PRICE from PRICELEVELITEMSIZE pls where "
                      " pls.ITEMSIZE_KEY = :ITEMSIZE_KEY and pls.PRICELEVEL_KEY = :PRICELEVEL_KEY ";

            IBInternalQuery->ParamByName("ITEMSIZE_KEY")->AsInteger = itemSizeKey;
            IBInternalQuery->ParamByName("PRICELEVEL_KEY")->AsInteger = stHappyHour->Tag;
            IBInternalQuery->ExecQuery();

            if(!IBInternalQuery->Eof)
            {
                  Currency fetchPrice= IBInternalQuery->FieldByName("PRICE")->AsCurrency;
                  if (fetchPrice != Price)
                  {
                    Price=  fetchPrice;
                  }
            }
       }

        return  Price;


        }
        catch(Exception & E)
        {
            TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
            throw;
        }
}
// ---------------------------------------------------------------------------
void  TfrmSelectDish::CheckLastAddedItem()
{
		for (UINT iSeat = 0; iSeat < SeatOrders.size(); iSeat++)
		{
			for (int i = 0; i < SeatOrders[iSeat]->Orders->Count; i++)
			{
				TItemComplete *Order = SeatOrders[iSeat]->Orders->Items[i];
                Order->LastAddedItem = false;
			}
		}
}
// ---------------------------------------------------------------------------
bool TfrmSelectDish::NotifyLastWebOrder(Database::TDBTransaction &DBTransaction)
{
    bool retVal = false;
    tiClock->Enabled = false;
    TGlobalSettings::Instance().NotifyPOSForLastWebOrder = GetValue(DBTransaction);
    if(TGlobalSettings::Instance().NotifyPOSForLastWebOrder)
    {
       UpdateValue();
       TGlobalSettings::Instance().NotifyPOSForLastWebOrder = GetValue(DBTransaction);
       retVal = true;
       MessageBox("Web Orders Cannot Be Processed Please Enable Webmate Interface", "", MB_OK + MB_ICONWARNING);
    }
    tiClock->Enabled = true;
    return retVal;
}
// ---------------------------------------------------------------------------
bool TfrmSelectDish::GetValue(Database::TDBTransaction &DBTransaction)
{

    bool retVal = false;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();

    IBInternalQuery->SQL->Text ="SELECT a.PROFILE_KEY, a.INTEGER_VAL FROM VARSPROFILE a "
	"left join PROFILE on PROFILE.PROFILE_KEY=a.PROFILE_KEY "
	" where a.VARIABLES_KEY=4134 and PROFILE.NAME=:NAME "  ;
	IBInternalQuery->ParamByName("NAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
	IBInternalQuery->ExecQuery();
	int web_mate = IBInternalQuery->FieldByName("INTEGER_VAL")->AsInteger;

    if(web_mate > 0)
    {
      retVal = true;
    }
    else
    {
      retVal = false;
    }
    return retVal;
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::UpdateValue()
{
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
 	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();

    IBInternalQuery->SQL->Text ="UPDATE VARSPROFILE a SET a.INTEGER_VAL =:INTEGER_VAL "
	" where a.VARIABLES_KEY=4134 and a.PROFILE_KEY= :PROFILE_KEY "  ;
	IBInternalQuery->ParamByName("PROFILE_KEY")->AsInteger = TDeviceRealTerminal::Instance().ID.ProfileKey;
    IBInternalQuery->ParamByName("INTEGER_VAL")->AsInteger = 0;
	IBInternalQuery->ExecQuery();
    DBTransaction.Commit();
}
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
bool  TfrmSelectDish::CheckCreditLimitExceeds(Database::TDBTransaction &dBTransaction, int tabKey)
{
    Currency CreditLimit = TDBTab::GetTabCreditLimit(dBTransaction, tabKey);
    if (CreditLimit != -1)
    {
        Currency OrdersTotal = 0;
        for (int b = 0; b < SeatOrders[0]->Orders->Count; b++)
        {
            TItemComplete *Order = SeatOrders[0]->Orders->Items[b];
            OrdersTotal += Order->TotalPriceAdjustmentSides();
        }
        if ((OrdersTotal + TDBTab::GetTabBalance(dBTransaction, tabKey)) > CreditLimit)
        {
            return ShowTabCreditLimitExceedsMessage(dBTransaction, CreditLimit);
        }
    }
    return true;
}
//--------------------------------------------------------------------------------------------------------------------------
bool TfrmSelectDish::CheckTabIsInUse(Database::TDBTransaction &dBTransaction, int tabKey)
{
    bool Proceed = true;
    try
    {
        if (!TDBTab::LockTab(dBTransaction, TDeviceRealTerminal::Instance().ID.Name, tabKey))
        {
            AnsiString LockedBy = TDBTab::GetLocker(dBTransaction, tabKey);
            if (MessageBox("This " + TGlobalSettings::Instance().SeatLabel + "/Tab is in use by " + LockedBy + "\rDo you wish to override this lock", "Warning",
                    MB_YESNO + MB_ICONWARNING) == ID_YES)
            {
                TDBTab::ReleaseTab(dBTransaction, TDeviceRealTerminal::Instance().ID.Name, tabKey);
                if (!TDBTab::LockTab(dBTransaction, TDeviceRealTerminal::Instance().ID.Name, tabKey))
                {
                    MessageBox("Unable to Unlock " + TGlobalSettings::Instance().SeatLabel + "/Tab Try again Later", "Warning", MB_OK + MB_ICONWARNING);
                    Proceed = false;
                }
            }
            else
            {
                Proceed = false;
            }
        }
    }
    catch(Exception & E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
    }
    return Proceed;
}
//----------------------------------------------------------------------------------------------------------------------------
bool TfrmSelectDish::ShowTabLockedMessage(AnsiString creditMessage1)
{
    AnsiString CreditMessage = "This Tab is locked";
    MessageBox(CreditMessage + "\r" + creditMessage1 + "\r" + "You cannot save these orders here.", "Error", MB_OK + MB_ICONERROR);
    return false;
}
//------------------------------------------------------------------------------------------------------------------------------
bool  TfrmSelectDish::CheckCreditLimitForTable(Database::TDBTransaction &dBTransaction, int tabKey, Currency OrdersTotal)
{
    Currency CreditLimit = TDBTab::GetTabCreditLimit(dBTransaction, tabKey);
    if (CreditLimit != -1)
    {
        if ((OrdersTotal + TDBTab::GetTabBalance(dBTransaction, tabKey)) > CreditLimit)
        {
            return ShowTabCreditLimitExceedsMessage(dBTransaction, CreditLimit);
        }
    }
    return true;
}
//--------------------------------------------------------------------------------------------------------------------------
bool  TfrmSelectDish::ShowTabCreditLimitExceedsMessage(Database::TDBTransaction &dBTransaction, Currency creditLimit)
{
    AnsiString CreditMessage = "Current Tab Balance is " + FloatToStrF(TDBTab::GetTabBalance(dBTransaction, tabKey), ffNumber, 15, 2);
    AnsiString CreditMessage1 = "Current Tab Credit Limit is " + FloatToStrF(creditLimit, ffNumber, 15, 2);
    MessageBox("Tab Credit Limit Reached.\r" + CreditMessage + "\r" + CreditMessage1 + "\r" + "You cannot save these orders here.", "Error", MB_OK + MB_ICONERROR);
    return false;
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::AddItemToSeat(Database::TDBTransaction& inDBTransaction,TItem* inItem,	bool  inSetMenuItem,
								   TItemSize* inItemSize,Currency  inPrice , bool IsItemSearchedOrScan)
{
    CheckLastAddedItem(); // check any added item in list;
	TItemComplete *Order = createItemComplete( inDBTransaction, inItem, inSetMenuItem, inItemSize, IsItemSearchedOrScan );



     if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot &&
            TGlobalSettings::Instance().EnableCustomerJourney && Order != NULL)
    {
        UnicodeString defaultTransaction = TManagerVariable::Instance().GetStr(inDBTransaction,vmSiHotDefaultTransactionName);
        std::vector<UnicodeString> GuestDetails = LoadGuestDetails(defaultTransaction);

        //if guest details size is zero then we need to store the details od room number str which was entered through UI.
        if(!GuestDetails.size())
        {
            GetRoomDetails();
            GuestDetails = LoadGuestDetails(defaultTransaction);
        }

        if(GuestDetails.size())
        {
            Order->AccNo = GuestDetails.at(0);
            Order->RoomNoStr = GuestDetails.at(1);
            Order->FirstName = GuestDetails.at(2);
            Order->LastName = "";
            if(GuestDetails.size() > 3)
                Order->LastName = GuestDetails.at(3);

            for (int i = 0; i < Order->SubOrders->Count; i++)
            {
                TItemCompleteSub *CurrentSubOrder = (TItemCompleteSub*)Order->SubOrders->Items[i];
                CurrentSubOrder->RoomNoStr = GuestDetails.at(1);
                CurrentSubOrder->FirstName = GuestDetails.at(2);
                CurrentSubOrder->LastName = "";
                if(GuestDetails.size() > 3)
                    CurrentSubOrder->LastName = GuestDetails.at(3);
                CurrentSubOrder->AccNo = GuestDetails.at(0);
            }
        }
    }

	if (inPrice != 0)
	{
		Order->SetPriceLevelCustom(inPrice);
		Order->PriceLevel0 = inPrice;
		Order->PriceLevel1 = inPrice;
	}
	else if (Order != NULL && Order->PriceEach() == -999.99)
	{
		std::auto_ptr<TfrmTouchNumpad>frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
		frmTouchNumpad->Caption = "Enter Amount";
		frmTouchNumpad->btnSurcharge->Caption = "Ok";
		frmTouchNumpad->btnDiscount->Visible = false;
		frmTouchNumpad->btnSurcharge->Visible = true;
		frmTouchNumpad->Mode = pmCurrency;
		frmTouchNumpad->CURInitial = 0;
		if (frmTouchNumpad->ShowModal() == mrOk)
		{
			Order->SetPriceLevelCustom(frmTouchNumpad->CURResult);
			Order->PriceLevel0 = frmTouchNumpad->CURResult;
			Order->PriceLevel1 = frmTouchNumpad->CURResult;
		}
		else
		{
			Order->SetPriceLevelCustom(0);
			Order->PriceLevel0 = 0;
			Order->PriceLevel1 = 0;
		}
        if(TGlobalSettings::Instance().UseMemberSubs)
        {
           Order->wasOpenItem = true;
        }
	}

	if (Order != NULL)
	{
       bool itemAdded = false;

       if(TGlobalSettings::Instance().MergeSimilarItem)
       {
           for (int i = 0; i < SeatOrders[SelectedSeat]->Orders->Count; i++)
           {
               TItemComplete *Item = SeatOrders[SelectedSeat]->Orders->Items[i];
               double currentQty  = Item->GetQty();
               Item->SetQtyCustom(1);
               if(TOrderUtils::Match(Item,Order) && !TOrderUtils::SoloItem(Order))
               {
                  itemAdded = true;
                  Item->SetQtyCustom(currentQty + 1);
                  break;
               }
               else
               {
                   Item->SetQtyCustom(currentQty);
               }
            }
            if(!itemAdded)
              SeatOrders[SelectedSeat]->Orders->Add( Order, inItem );
       }
       else
       {
          SeatOrders[SelectedSeat]->Orders->Add( Order, inItem );
       }
        //if(!itemAdded)
        //  SeatOrders[SelectedSeat]->Orders->Add( Order, inItem );
        if((SeatOrders[SelectedSeat]->Orders->AppliedMembership.ContactKey == 0) || (!TPaySubsUtility::IsLocalLoyalty()) ||
            SeatOrders[SelectedSeat]->Orders->AppliedMembership.Points.PointsRulesSubs.Contains(eprAllowDiscounts))
        {
     		TManagerFreebie::IsPurchasing(inDBTransaction, SeatOrders[SelectedSeat]->Orders->List);
        }
		CheckDeals(inDBTransaction);

        ApplyMemberDiscounts(inDBTransaction, false);

	}

	RedrawSeatOrders();
	TotalCosts();
	UpdateExternalDevices();
	HighlightSelectedItem();
	UpdateTableButton();
}
// ---------------------------------------------------------------------------
TItemComplete * TfrmSelectDish::createItemComplete(
					Database::TDBTransaction& DBTransaction,
									   TItem* Item,
										bool  SetMenuItem,
								   TItemSize* inItemSize,
                                   bool IsItemSeaarchedOrScan )
{
	TItemComplete *itemComplete = new TItemComplete;
	TItemSize *itemSize;

	//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	itemComplete->ItemOrderedFrom = Item;
	itemComplete->TableNo = 0;
	itemComplete->SeatNo = 0;
	itemComplete->ItemKey = Item->ItemKey;
	itemComplete->Item = Item->Item;
	itemComplete->ItemKitchenName = Item->ItemKitchenName;
	itemComplete->Item_ID = Item->Item_ID;
	itemComplete->MenuName = Item->MenuName;
	// TODO 4 -o  Michael -c Printing : Remove When Printing Engine Finished
	itemComplete->MenuKey = Item->MenuKey;
	itemComplete->ItemType = Item->ItemType;
	itemComplete->PrintChitNumber = Item->PrintChitNumber;
	itemComplete->OrderType = NormalOrder;
	itemComplete->HappyHour = stHappyHour->Visible;
	itemComplete->TimeStamp = Now();
	itemComplete->TransNo = TDBOrder::GetNextTransNumber(DBTransaction);

	itemComplete->Note = "";
	itemComplete->Course = Item->Course;
	itemComplete->CourseKitchenName = Item->CourseKitchenName;
    if(IsItemSeaarchedOrScan)   // get default serving course for scanned and serached items..
    {
        itemComplete->ServingCourse =  TDeviceRealTerminal::Instance().Menus->GetServingCourse(GetDefaultServingCourse(itemComplete->ItemKey));
    }
    else
    {
        itemComplete->ServingCourse = CurrentServingCourse;
    }
	itemComplete->ItemAppearanceOrder = Item->ItemAppearanceOrder;
	itemComplete->FontInfo = Item->FontInfo;
	itemComplete->Loyalty_Key = SeatOrders[SelectedSeat]->Orders->AppliedMembership.ContactKey;
	itemComplete->SetQty(Item->GetQty());
   	itemComplete->IsPriceBarcodedItem = Item->IsPriceBarcodedItem;
    itemComplete->PrintCancel = false;
    itemComplete->LastAddedItem = true; // check item is last added or not..
    itemComplete->IsPayByPoints = false; // initialize pay by points to be false..
	if (inItemSize != NULL)
	{
		itemSize = inItemSize;
	}
	else if (Item->Sizes->Count == 0)
	{
		delete itemComplete;
		MessageBox("This item has no Size.", "Error", MB_OK + MB_ICONERROR);
		itemComplete = NULL;
		return itemComplete;
	}
	else if (Item->Sizes->Count > 1 && Item->DisplaySizes)
	{
         if(TGlobalSettings::Instance().ShowItemSizesAsList)
         {
            std::auto_ptr<TfrmSelectSizes>frmSelectSizes(TfrmSelectSizes::Create<TfrmSelectSizes>(this));
            frmSelectSizes->Caption =    Item->GetDecoratedName() + " - Select a Size";
            frmSelectSizes->Sizes->SizeCopyList(Item->Sizes);
            frmSelectSizes->ShowModal();
            if (frmSelectSizes->SelectedItemSizeGrid == -1) // Menu Change while selecting glass.
            {
                delete itemComplete;
                itemComplete = NULL;
                return itemComplete;
            }
            itemSize = Item->Sizes->SizeGet(frmSelectSizes->SelectedItemSizeGrid);
         }
        else
        {
            std::auto_ptr<TfrmSelectGlass>frmSelectGlass(TfrmSelectGlass::Create<TfrmSelectGlass>(this));

            frmSelectGlass->Sizes->SizeCopyList(Item->Sizes);
            if (frmSelectGlass->Preprocess() == -1)
            {
                frmSelectGlass->ShowModal();
            }
            if (frmSelectGlass->SelectedGlass == -1) // Menu Change while selecting glass.
            {
                delete itemComplete;
                itemComplete = NULL;
                return itemComplete;
            }

            itemSize = Item->Sizes->SizeGet(frmSelectGlass->SelectedGlass);
        }
	}
	else // Only 1 Size or Defaulting to First Size they can change this in options.
	{
		itemSize = Item->Sizes->SizeGet(0);
	}
	itemComplete->Size = itemSize->Name;

	/* Eagerly claim an instance of the item+size */
        Database::TDBTransaction dBTransaction(TDeviceRealTerminal::Instance().DBControl);
        dBTransaction.StartTransaction();

     bool isItemUsingPCD = TDBOrder::IsItemUsingPCD(dBTransaction, Item->ItemKey, itemSize->Name);
     if(isItemUsingPCD)
     {
		itemComplete->ClaimAvailability();

     }
     dBTransaction.Commit();

	itemComplete->SizeKitchenName = itemSize->SizeKitchenName;
    itemComplete->GSTPercent = itemSize->GSTPercent;
	itemComplete->TaxProfiles = itemSize->TaxProfiles;
	itemComplete->PointsPercent = itemSize->PointsPercent;
	itemComplete->SetMenuMask = itemSize->SetMenuMask;
	itemComplete->SetMenu = itemSize->SetMenuItem && SetMenuItem;
	itemComplete->SetMenuMaster = itemSize->SetMenuMaster;
	itemComplete->ThirdPartyKey = itemSize->ThirdPartyKey;
	itemComplete->ThirdPartyCode = itemSize->ThirdPartyCode;
    itemComplete->RevenueCode = itemSize->RevenueCode;
	itemComplete->PLU = itemSize->PLU;

	itemComplete->MemberFreeSaleCount = itemSize->MemberFreeSaleCount;
	itemComplete->MemberFreeSaleDiscount = itemSize->MemberFreeSaleDiscount;
	itemComplete->LocationFreeSaleCount = itemSize->LocationFreeSaleCount;
	itemComplete->LocationFreeSaleDiscount = itemSize->LocationFreeSaleDiscount;

	if (itemComplete->SetMenuMaster && itemComplete->SetMenuMask != 0)
	{
		itemComplete->SetMenuGroup = TDBOrder::GetNextSetMenuGroupNumber(DBTransaction);
	}

	// Sort Categories
	itemComplete->Categories->CategoryCopyList(itemSize->Categories);
	// Sort Recipes
	itemComplete->SalesRecipesToApply->RecipeCopyList(itemSize->Recipes);

	itemComplete->PriceLevel0 = itemSize->Price;
   	itemComplete->PriceLevel1 = VerifyPriceLevelPrice(DBTransaction,itemSize->ItemSizeKey,itemSize->PriceLevels[stHappyHour->Tag].Price);



	// Calculate Membership Pricing.
    itemComplete->ResetPrice();
    if((SeatOrders[SelectedSeat]->Orders->AppliedMembership.ContactKey == 0)  || (!TPaySubsUtility::IsLocalLoyalty())
         || SeatOrders[SelectedSeat]->Orders->AppliedMembership.Points.PointsRulesSubs.Contains(eprAllowDiscounts))
	   ManagerDiscount->AddDiscountsByTime(DBTransaction, itemComplete);

	itemComplete->Cost = itemSize->Cost; // Get default cost if assigned.
    itemComplete->MaxRetailPrice = itemSize->MaxRetailPrice;
	itemComplete->CostGSTPercent = itemSize->CostGSTPercent;
    // add cost for points...
	itemComplete->ItemPriceForPoints = itemSize->CostForPoints;
    itemComplete->ItemPriceForPointsOriginal = itemSize->CostForPoints;
    itemComplete->IsCanBePaidForUsingPoints = itemSize->CanBePaidForUsingPoints;  // check can be paid by points..
	if (itemSize->Weighted)
	{
		itemComplete->WeightedPrice.AssignTareWeight(itemSize->TareWeight);
		TWeight Weight;
		std::auto_ptr<TScaleController>ScalesControler(new TScaleController(this, TDeviceRealTerminal::Instance().Scales.get(), TDeviceRealTerminal::Instance().PoleDisplay, itemComplete,
				TGlobalSettings::Instance().AutoConvertScalesWeights));
        if(Item->ItemWeight.GetWeight() == 0)
           {
                if (ScalesControler->GetWeight(Weight) != mrOk) // Menu Change while selecting glass.
                {
                    /* We have to make sure the count is reversed. When we have more
                         * time we should implement a RAII mechanism for this.
                         */
                        itemComplete->ReturnToAvailability();
                    delete itemComplete;
                    itemComplete = NULL;
                    return itemComplete;
                }
           }
        else
           {
               Weight.Assign(Item->ItemWeight);
           }
        // Pos Price is always price per kg.
        Weight.GetWeightIn_Kg(Weight);
		itemComplete->SetWeight(Weight);

        if((TGlobalSettings::Instance().NotifyForManuallyEnteredWeight) && (!ScalesControler->WeighedItem))
        itemComplete->isManuallyEnteredWeight=true;

	}

        itemComplete->CanBePaidForUsingPoints(
          Item->SizeCanBePaidForUsingPoints(itemComplete->Size));

	if (!itemComplete->SetMenuMaster)
	{
		std::auto_ptr<TListOptionContainer>ForcedOptionsList(new TListOptionContainer);
		std::auto_ptr<TListSideContainer>ForcedSidesList(new TListSideContainer);
		// Do Forced Sides.
		std::auto_ptr<TfrmSelectForcedOptions>frmSelectForcedOptions(TfrmSelectForcedOptions::Create<TfrmSelectForcedOptions>(this));
		std::auto_ptr<TfrmSelectForcedSides>frmSelectForcedSides(TfrmSelectForcedSides::Create<TfrmSelectForcedSides>(this, DBTransaction));

		if (TGlobalSettings::Instance().ForcedSidesFirst)
		{
			for (int i = 1; i < 16; i++) // Max 16 Possible Forced Options Groups.
			{
				ForcedSidesList->Clear();
				frmSelectForcedSides->MasterItem = itemComplete;
				Item->Sides->SidesRetriveMaskedList(i, ForcedSidesList.get());
				if (ForcedSidesList->Count > 0)
				{
					frmSelectForcedSides->ForcedSidesList = ForcedSidesList.get();
					if (frmSelectForcedSides->ShowModal() == mrCancel)
					{
					        itemComplete->ReturnToAvailability();
						delete itemComplete;
						itemComplete = NULL;
						return itemComplete;
					}
				}
			}

			// Do Forced Options.
			for (int i = 1; i < 16; i++) // Max 16 Possible Forced Options Groups.
			{
				ForcedOptionsList->OptionClear();
				retrieveForcedOptionList( i, Item, ForcedOptionsList.get() );
				//Item->Options->OptionRetriveMaskedList(i, Item->OptionGroup, ForcedOptionsList.get());
				if (ForcedOptionsList->Count > 0)
				{
					frmSelectForcedOptions->ForcedOptionsList = ForcedOptionsList.get();
					if (frmSelectForcedOptions->ShowModal() == mrCancel)
					{
                                                itemComplete->ReturnToAvailability();
						delete itemComplete;
						itemComplete = NULL;
						return itemComplete;
					}
					itemComplete->OptionsSelected->OptionAddList(frmSelectForcedOptions->SelectedOptions);
				}
			}
		}
		else
		{
			// Do Forced Options.
			for (int i = 1; i < 16; i++) // Max 16 Possible Forced Options Groups.
			{
				ForcedOptionsList->OptionClear();
				retrieveForcedOptionList( i, Item, ForcedOptionsList.get() );
				//Item->Options->OptionRetriveMaskedList(i, Item->OptionGroup, ForcedOptionsList.get());
				if (ForcedOptionsList->Count > 0)
				{
					frmSelectForcedOptions->ForcedOptionsList = ForcedOptionsList.get();
					if (frmSelectForcedOptions->ShowModal() == mrCancel)
					{
                                                itemComplete->ReturnToAvailability();
						delete itemComplete;
						itemComplete = NULL;
						return itemComplete;
					}
					itemComplete->OptionsSelected->OptionAddList(frmSelectForcedOptions->SelectedOptions);
				}
			}

			for (int i = 1; i < 16; i++) // Max 16 Possible Forced Options Groups.
			{
				ForcedSidesList->Clear();
				frmSelectForcedSides->MasterItem = itemComplete;
				Item->Sides->SidesRetriveMaskedList(i, ForcedSidesList.get());
				if (ForcedSidesList->Count > 0)
				{
					frmSelectForcedSides->ForcedSidesList = ForcedSidesList.get();
					if (frmSelectForcedSides->ShowModal() == mrCancel)
					{
                                                itemComplete->ReturnToAvailability();
						delete itemComplete;
						itemComplete = NULL;
						return itemComplete;
					}
				}
			}
		}
	}
    itemComplete->PatronCount(
    itemComplete->DefaultPatronCount());
	// Complete itemComplete Security.
	/* This is updated when the item is finally sold but is required here for the
	REceipt preview if the receipt is sorted by itemCompleteed By */
	TSecurityReference SecRef;
	SecRef.UserKey = TDeviceRealTerminal::Instance().User.ContactKey;
	SecRef.Event = SecurityTypes[secOrderedBy];
	SecRef.From = TDeviceRealTerminal::Instance().User.Name;
	SecRef.To = TDeviceRealTerminal::Instance().User.Initials;
	SecRef.TimeStamp = Now();
	itemComplete->Security->SecurityUpdate(secOrderedBy, SecRef);

	CurrentTimeKey = TDBSaleTimes::OpenSaleStartTime(DBTransaction, CurrentTimeKey);
	itemComplete->TimeKey = CurrentTimeKey;
	//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	return itemComplete;
}
// ---------------------------------------------------------------------------
TItemCompleteSub * TfrmSelectDish::AddSubItemToItem(Database::TDBTransaction &DBTransaction, TItem *Item, TItemComplete *MasterOrder)
{
    int SelectedSize;
	TItemCompleteSub *NewSubOrder = new TItemCompleteSub;
	NewSubOrder->Item = Item->Item;
	NewSubOrder->ItemKey = Item->ItemKey;
	NewSubOrder->Item_ID = Item->Item_ID;
	NewSubOrder->TransNo = TDBOrder::GetNextTransNumber(DBTransaction);
	NewSubOrder->Course = Item->Course;
	NewSubOrder->CourseKitchenName = Item->CourseKitchenName;
	NewSubOrder->ItemKitchenName = Item->ItemKitchenName;
	NewSubOrder->MenuName = Item->MenuName;
	NewSubOrder->TimeStamp = MasterOrder->TimeStamp;
	NewSubOrder->Cost = 0;
    NewSubOrder->PrintCancel = false;
	/* If the item is not weighed then the side Qty needs to match it's parents. */
	if (!MasterOrder->WeightedPrice.WeightedItem)
	{
		NewSubOrder->SetQty(MasterOrder->GetQty());
	}

	NewSubOrder->FontInfo = Item->FontInfo;
	NewSubOrder->ServingCourse = MasterOrder->ServingCourse;

	if (Item->Sizes->Count == 0)
	{
		MessageBox("This item has no Size.", "Error", MB_OK + MB_ICONERROR);
		delete NewSubOrder;
		return NULL;
	}
	else if (Item->Sizes->Count > 1 && Item->DisplaySizes)
	{
         if(TGlobalSettings::Instance().ShowItemSizesAsList)
         {
            std::auto_ptr<TfrmSelectSizes>frmSelectSizes(TfrmSelectSizes::Create<TfrmSelectSizes>(this));
            frmSelectSizes->Caption =    Item->GetDecoratedName() + " - Select a Size";
            frmSelectSizes->Sizes->SizeCopyList(Item->Sizes);
            frmSelectSizes->ShowModal();
            if (frmSelectSizes->SelectedItemSizeGrid == -1) // Menu Change while selecting glass.
            {
                	delete NewSubOrder;
			        return NULL;
            }
            if (Item->Sizes->SizeGet(frmSelectSizes->SelectedItemSizeGrid)->SetMenuMaster)
		    {
                MessageBox("You can not add a set menu as a side.", "Error", MB_OK + MB_ICONERROR);
                delete NewSubOrder;
                return NULL;
		    }
            SelectedSize = frmSelectSizes->SelectedItemSizeGrid;
         }
        else
        {
            std::auto_ptr<TfrmSelectGlass>frmSelectGlass(TfrmSelectGlass::Create<TfrmSelectGlass>(this));
            frmSelectGlass->Sizes->SizeCopyList(Item->Sizes);
            if (frmSelectGlass->Preprocess() == -1)
            {
                frmSelectGlass->ShowModal();
            }

            if (frmSelectGlass->SelectedGlass == -1) // Menu Change while selecting glass.
            {
                delete NewSubOrder;
                return NULL;
            }
            if (Item->Sizes->SizeGet(frmSelectGlass->SelectedGlass)->SetMenuMaster)
            {
                MessageBox("You can not add a set menu as a side.", "Error", MB_OK + MB_ICONERROR);
                delete NewSubOrder;
                return NULL;
            }
            SelectedSize = frmSelectGlass->SelectedGlass;
        }
		// Sort Categories
		NewSubOrder->Categories->CategoryCopyList(Item->Sizes->SizeGet(SelectedSize)->Categories);
		NewSubOrder->Size = Item->Sizes->SizeGet(SelectedSize)->Name;
        Database::TDBTransaction dBTransaction(TDeviceRealTerminal::Instance().DBControl);
        dBTransaction.StartTransaction();
        bool isItemUsingPCD = TDBOrder::IsItemUsingPCD(dBTransaction, Item->ItemKey, NewSubOrder->Size);
         dBTransaction.Commit();
         if(isItemUsingPCD)
         {
                NewSubOrder->ClaimAvailability();
         }

		NewSubOrder->SizeKitchenName = Item->Sizes->SizeGet(SelectedSize)->SizeKitchenName;
        NewSubOrder->GSTPercent = Item->Sizes->SizeGet(SelectedSize)->GSTPercent; // Get default cost if assigned.
		NewSubOrder->TaxProfiles = Item->Sizes->SizeGet(SelectedSize)->TaxProfiles; // Get default cost if assigned.
		NewSubOrder->PointsPercent = Item->Sizes->SizeGet(SelectedSize)->PointsPercent;
		NewSubOrder->MemberFreeSaleCount = Item->Sizes->SizeGet(SelectedSize)->MemberFreeSaleCount;
		NewSubOrder->MemberFreeSaleDiscount = Item->Sizes->SizeGet(SelectedSize)->MemberFreeSaleDiscount;
		NewSubOrder->LocationFreeSaleCount = Item->Sizes->SizeGet(SelectedSize)->LocationFreeSaleCount;
		NewSubOrder->LocationFreeSaleDiscount = Item->Sizes->SizeGet(SelectedSize)->LocationFreeSaleDiscount;
		NewSubOrder->ThirdPartyKey = Item->Sizes->SizeGet(SelectedSize)->ThirdPartyKey;
		NewSubOrder->ThirdPartyCode = Item->Sizes->SizeGet(SelectedSize)->ThirdPartyCode;
		NewSubOrder->PLU = Item->Sizes->SizeGet(SelectedSize)->PLU;
        NewSubOrder->RevenueCode = Item->Sizes->SizeGet(SelectedSize)->RevenueCode;
		// Sort Recipes
		NewSubOrder->SalesRecipesToApply->RecipeCopyList(Item->Sizes->SizeGet(SelectedSize)->Recipes);
		NewSubOrder->Cost = Item->Sizes->SizeGet(SelectedSize)->Cost; // Get default cost if assigned.
		NewSubOrder->CostGSTPercent = Item->Sizes->SizeGet(SelectedSize)->CostGSTPercent;
		NewSubOrder->PriceLevel0 = Item->Sizes->SizeGet(SelectedSize)->Price;
		NewSubOrder->PriceLevel1 = VerifyPriceLevelPrice(DBTransaction,Item->Sizes->SizeGet(SelectedSize)->ItemSizeKey,Item->Sizes->SizeGet(SelectedSize)->PriceLevels[stHappyHour->Tag].Price);
	// Calculate Membership Pricing.
		NewSubOrder->HappyHour = stHappyHour->Visible;
		NewSubOrder->Loyalty_Key = MasterOrder->Loyalty_Key;
		NewSubOrder->ResetPrice();
        //add cost for item in sub item...
        NewSubOrder->ItemPriceForPoints = Item->Sizes->SizeGet(SelectedSize)->CostForPoints;
        NewSubOrder->ItemPriceForPointsOriginal = Item->Sizes->SizeGet(SelectedSize)->CostForPoints;
        NewSubOrder->IsCanBePaidForUsingPoints = Item->Sizes->SizeGet(SelectedSize)->CanBePaidForUsingPoints;
        //check item is can be paid by points..
        NewSubOrder->SubItemType = Item->ItemType;

        if(((SeatOrders[SelectedSeat]->Orders->AppliedMembership.ContactKey == 0 ) || (!TPaySubsUtility::IsLocalLoyalty())
            || SeatOrders[SelectedSeat]->Orders->AppliedMembership.Points.PointsRulesSubs.Contains(eprAllowDiscounts)))
	       ManagerDiscount->AddDiscountsByTime(DBTransaction, NewSubOrder);

		if (Item->Sizes->SizeGet(SelectedSize)->Weighted)
		{
			NewSubOrder->WeightedPrice.AssignTareWeight(Item->Sizes->SizeGet(SelectedSize)->TareWeight);
            NewSubOrder->WeightedPrice.WeightedItem=true;
			TWeight Weight;
			std::auto_ptr<TScaleController>ScalesControler(new TScaleController(this, TDeviceRealTerminal::Instance().Scales.get(), TDeviceRealTerminal::Instance().PoleDisplay, NewSubOrder,
					TGlobalSettings::Instance().AutoConvertScalesWeights));
			if (ScalesControler->GetWeight(Weight) != mrOk) // Menu Change while selecting glass.
			{
				NewSubOrder->ReturnToAvailability();
				delete NewSubOrder;
				NewSubOrder = NULL;
				return NewSubOrder;
			}
			// Pos Price is always price per kg.
			Weight.GetWeightIn_Kg(Weight);
			NewSubOrder->SetWeight(Weight);

            if((TGlobalSettings::Instance().NotifyForManuallyEnteredWeight) && (!ScalesControler->WeighedItem))
                NewSubOrder->isManuallyEnteredWeight=true;

		}
	}
	else // Only 1 Size or Defaulting to First Size they can change this in options.
	{
		if (Item->Sizes->SizeGet(0)->SetMenuMaster)
		{
			MessageBox("You can not add a set menu as a side.", "Error", MB_OK + MB_ICONERROR);
			delete NewSubOrder;
			return NULL;
		}
		NewSubOrder->Categories->CategoryCopyList(Item->Sizes->SizeGet(0)->Categories);
		NewSubOrder->Size = Item->Sizes->SizeGet(0)->Name;
        Database::TDBTransaction dBTransaction(TDeviceRealTerminal::Instance().DBControl);
        dBTransaction.StartTransaction();
        bool isItemUsingPCD = TDBOrder::IsItemUsingPCD(dBTransaction, Item->ItemKey, NewSubOrder->Size);
         dBTransaction.Commit();
         if(isItemUsingPCD)
         {
            NewSubOrder->ClaimAvailability();
         }

		NewSubOrder->SizeKitchenName = Item->Sizes->SizeGet(0)->SizeKitchenName;
        NewSubOrder->GSTPercent = Item->Sizes->SizeGet(0)->GSTPercent; // Get default cost if assigned.
		NewSubOrder->TaxProfiles = Item->Sizes->SizeGet(0)->TaxProfiles; // Get default cost if assigned.
		NewSubOrder->PointsPercent = Item->Sizes->SizeGet(0)->PointsPercent;

		NewSubOrder->MemberFreeSaleCount = Item->Sizes->SizeGet(0)->MemberFreeSaleCount;
		NewSubOrder->MemberFreeSaleDiscount = Item->Sizes->SizeGet(0)->MemberFreeSaleDiscount;
		NewSubOrder->LocationFreeSaleCount = Item->Sizes->SizeGet(0)->LocationFreeSaleCount;
		NewSubOrder->LocationFreeSaleDiscount = Item->Sizes->SizeGet(0)->LocationFreeSaleDiscount;
		NewSubOrder->ThirdPartyKey = Item->Sizes->SizeGet(0)->ThirdPartyKey;
		NewSubOrder->ThirdPartyCode = Item->Sizes->SizeGet(0)->ThirdPartyCode;
		NewSubOrder->PLU = Item->Sizes->SizeGet(0)->PLU;
        NewSubOrder->RevenueCode = Item->Sizes->SizeGet(0)->RevenueCode;
		// Sort Recipes
		NewSubOrder->SalesRecipesToApply->RecipeCopyList(Item->Sizes->SizeGet(0)->Recipes);
		NewSubOrder->Cost = Item->Sizes->SizeGet(0)->Cost; // Get default cost if assigned.
		NewSubOrder->CostGSTPercent = Item->Sizes->SizeGet(0)->CostGSTPercent;
		NewSubOrder->PriceLevel0 = Item->Sizes->SizeGet(0)->Price;
	  	NewSubOrder->PriceLevel1 =  VerifyPriceLevelPrice(DBTransaction,Item->Sizes->SizeGet(0)->ItemSizeKey,Item->Sizes->SizeGet(0)->PriceLevels[stHappyHour->Tag].Price);
		// Calculate Membership Pricing.
		NewSubOrder->HappyHour = stHappyHour->Visible;
		NewSubOrder->Loyalty_Key = MasterOrder->Loyalty_Key;
		NewSubOrder->ResetPrice();
        NewSubOrder->ItemPriceForPoints = Item->Sizes->SizeGet(0)->CostForPoints;
        NewSubOrder->ItemPriceForPointsOriginal = Item->Sizes->SizeGet(0)->CostForPoints;
        NewSubOrder->IsCanBePaidForUsingPoints = Item->Sizes->SizeGet(0)->CanBePaidForUsingPoints;
        NewSubOrder->SubItemType = Item->ItemType;

    if(((SeatOrders[SelectedSeat]->Orders->AppliedMembership.ContactKey == 0 ) || (!TPaySubsUtility::IsLocalLoyalty())
         || SeatOrders[SelectedSeat]->Orders->AppliedMembership.Points.PointsRulesSubs.Contains(eprAllowDiscounts)))
	   ManagerDiscount->AddDiscountsByTime(DBTransaction, NewSubOrder);

		if (Item->Sizes->SizeGet(0)->Weighted)
		{
			NewSubOrder->WeightedPrice.AssignTareWeight(Item->Sizes->SizeGet(0)->TareWeight);
			TWeight Weight;
			std::auto_ptr<TScaleController>ScalesControler(new TScaleController(this, TDeviceRealTerminal::Instance().Scales.get(), TDeviceRealTerminal::Instance().PoleDisplay, NewSubOrder,
					TGlobalSettings::Instance().AutoConvertScalesWeights));
			if (ScalesControler->GetWeight(Weight) != mrOk) // Menu Change while selecting glass.
			{
				NewSubOrder->ReturnToAvailability();
				delete NewSubOrder;
				NewSubOrder = NULL;
				return NewSubOrder;
			}
			// Pos Price is always price per kg.
			Weight.GetWeightIn_Kg(Weight);
			NewSubOrder->SetWeight(Weight);
            if((TGlobalSettings::Instance().NotifyForManuallyEnteredWeight) && (!ScalesControler->WeighedItem))
                NewSubOrder->isManuallyEnteredWeight=true;

		}
	}

        NewSubOrder->CanBePaidForUsingPoints(
          Item->SizeCanBePaidForUsingPoints(NewSubOrder->Size));

	// Complete Order Security.
	/* This is updated when the item is finally sold but is required here for the
	REceipt preview if the receipt is sorted by Ordered By */
	TSecurityReference SecRef;
	SecRef.UserKey = TDeviceRealTerminal::Instance().User.ContactKey;
	SecRef.Event = SecurityTypes[secOrderedBy];
	SecRef.From = TDeviceRealTerminal::Instance().User.Name;
	SecRef.To = TDeviceRealTerminal::Instance().User.Initials;
	SecRef.TimeStamp = Now();
	NewSubOrder->Security->SecurityUpdate(secOrderedBy, SecRef);
	MasterOrder->SubOrders->SubOrderAdd(NewSubOrder);
	MasterOrder->MasterContainer = MasterOrder->Size;
    if((SeatOrders[SelectedSeat]->Orders->AppliedMembership.ContactKey == 0) || (!TPaySubsUtility::IsLocalLoyalty()) ||
       SeatOrders[SelectedSeat]->Orders->AppliedMembership.Points.PointsRulesSubs.Contains(eprAllowDiscounts))
	  TManagerFreebie::IsPurchasing(DBTransaction, MasterOrder->SubOrders);
	return NewSubOrder;
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::AssignDiscountLists()
{
   Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
   DBTransaction.StartTransaction();
   std::auto_ptr<TList> allOrders(new TList());
   GetAllOrders(allOrders.get());
   TDiscount CurrentDiscount;
   TSCDPWDChecker SCDChecker;
   isChitDiscountExist = false;

   if(SeatOrders[SelectedSeat]->Orders->Count == 0)
   {
       return;
   }

   for(int i=0; i < ChitNumber.DiscountList.size(); i++)
   {
         if(ChitNumber.DiscountList[i].IsComplimentaryDiscount())
          {
            TypeOfSale = ComplimentarySale;
          }
          else if( ChitNumber.DiscountList[i].IsNonChargableDiscount())
          {
             TypeOfSale = NonChargableSale;
          }
          ManagerDiscount->ClearDiscount(SeatOrders[SelectedSeat]->Orders->List, ChitNumber.DiscountList[i]);
//          if(SCDChecker.SeniorCitizensCheck(ChitNumber.DiscountList[i], allOrders.get()))
          if((SCDChecker.SeniorCitizensCheck(CurrentDiscount, allOrders.get()))
               && (SCDChecker.PWDCheck(CurrentDiscount, allOrders.get())))
          {
               bool isDiscountApplied = ApplyDiscount(DBTransaction, ChitNumber.DiscountList[i], SeatOrders[SelectedSeat]->Orders->List);
              if(isDiscountApplied && SeatOrders[SelectedSeat]->Orders->List->Count >0 )
              {
                    ChitNumber.DiscountList[i].Type = 0;
              }
              if(!isDiscountApplied)
              {
                    ChitNumber.DiscountList.erase(ChitNumber.DiscountList.begin()+i);
                    i--;
                    isChitDiscountExist = true;
              }
          }
     }

   DBTransaction.Commit();
   RedrawSeatOrders();
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::ClearDiscountLists(std::vector<TDiscount> DiscountList)
{
    ChitNumber.DiscountList.erase(ChitNumber.DiscountList.begin(),ChitNumber.DiscountList.end());
    // RemoveDiscountList(List);
    RedrawSeatOrders();
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::GetChitDiscountList(Database::TDBTransaction &dBTransaction, std::vector<TDiscount> DiscountList)
{
  if((SeatOrders[SelectedSeat]->Orders->AppliedMembership.ContactKey == 0) || (!TPaySubsUtility::IsLocalLoyalty()) ||
           SeatOrders[SelectedSeat]->Orders->AppliedMembership.Points.PointsRulesSubs.Contains(eprAllowDiscounts))
  {
      TDiscount currentDiscount;
      for(int i = 0 ; i < ChitNumber.ApplyDiscountsList->Count; i++)
      {
        int discount_key = (int)ChitNumber.ApplyDiscountsList->Objects[i];
        ManagerDiscount->GetDiscount(dBTransaction, discount_key, currentDiscount);
        ChitNumber.DiscountList.push_back(currentDiscount);
      }
  }
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::RemoveChitDiscountList()
{
   RemoveDiscountList(ChitNumber.ApplyDiscountsList);
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::RemoveDiscountList(TStringList *List)
{
   Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
   DBTransaction.StartTransaction();
   std::auto_ptr<TList> allOrders(new TList());
   GetAllOrders(allOrders.get());
   TDiscount CurrentDiscount;
   for(int i=0; i < List->Count; i++)
   {
      ManagerDiscount->GetDiscount(DBTransaction, (int)List->Objects[i], CurrentDiscount);
      ManagerDiscount->ClearDiscount(SeatOrders[SelectedSeat]->Orders->List, CurrentDiscount);
   }
   DBTransaction.Commit();
}

//---------------------------------------------------------------------------
void TfrmSelectDish::ApplyMemberDiscounts(Database::TDBTransaction &DBTransaction,bool isInitiallyApplied)
{
    std::map<int, TMembershipDiscountList> OpenDiscountAmount;
    std::map<int, TMembershipDiscountList>::iterator ii;
	if (SeatOrders[SelectedSeat]->Orders->AppliedMembership.ContactKey != 0)
	{
        if(SeatOrders[SelectedSeat]->Orders->AppliedMembership.ContactKey != 0 && !isInitiallyApplied)
        {
               // Extract open Discount and assign the open discount amount to Current discount amount
               for (int i = 0; i < SeatOrders[SelectedSeat]->Orders->Count; i++)
               {
                  TItemMinorComplete *Item = SeatOrders[SelectedSeat]->Orders->Items[i];
                  int k = Item->Discounts.size();
                  for(std::vector<TDiscount>::iterator ptrDiscount = Item->Discounts.begin(); ptrDiscount != Item->Discounts.end(); ++ptrDiscount)
                  {
                        TDiscount CurrentDiscount = *ptrDiscount;
                        if(CurrentDiscount.Source == dsMMMembership)
                        {
                            CurrentDiscount.PercentAmount = CurrentDiscount.OriginalAmount;
                            CurrentDiscount.Amount = CurrentDiscount.OriginalAmount;
                            OpenDiscountAmount[ptrDiscount->DiscountKey].Discount = CurrentDiscount;
                            OpenDiscountAmount[ptrDiscount->DiscountKey].IsApplied = false;
                        }
                  }
               }

        }
        ManagerDiscount->ClearMemberDiscounts(SeatOrders[SelectedSeat]->Orders->List);
        ManagerDiscount->ClearMemberExemtDiscounts(SeatOrders[SelectedSeat]->Orders->List);
		for (std::set<int>::iterator ptrDiscountKey = SeatOrders[SelectedSeat]->Orders->AppliedMembership.AutoAppliedDiscounts.begin();
			ptrDiscountKey != SeatOrders[SelectedSeat]->Orders->AppliedMembership.AutoAppliedDiscounts.end(); ptrDiscountKey++)
		{
            ii = OpenDiscountAmount.find(*ptrDiscountKey);
            if(ii != OpenDiscountAmount.end())
            {
                TDiscount CurrentDiscount = ii->second.Discount;
                ApplyDiscount(DBTransaction, CurrentDiscount, SeatOrders[SelectedSeat]->Orders->List, ii->second.IsApplied, dsMMMembership);
            }
            else
            {
                ApplyDiscount(DBTransaction, *ptrDiscountKey, SeatOrders[SelectedSeat]->Orders->List,true,dsMMMembership);
            }
		}
	}
    OpenDiscountAmount.clear();
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::GetThorVouchers()
{
   if(TGlobalSettings::Instance().MembershipType == MembershipTypeThor && TGlobalSettings::Instance().IsThorlinkSelected)
   {
        TGlobalSettings::Instance().IsThorVoucherSelected = true;
        std::auto_ptr<TfrmMessage> frmMessage(TfrmMessage::Create<TfrmMessage>(this, TDeviceRealTerminal::Instance().DBControl));
		frmMessage->MessageType = eThorDiscountReason;

        if(ManagerDiscount->IsVouchersAvailable())
        {
            if (frmMessage->ShowModal() == mrOk)
            {
               int voucherCode = frmMessage->Key;
               if (frmMessage->Key == -1)
                {
                    ManagerDiscount->ClearThorVouchersDiscounts(SeatOrders[SelectedSeat]->Orders->List);
                    RedrawSeatOrders();
                    TotalCosts();
                    UpdateExternalDevices();
                }
                else
                {
                  TDiscount CurrentDiscount;
                  CurrentDiscount.DiscountKey = ManagerDiscount->GetDiscountKeyForVoucher(frmMessage->Key);
                  if(CurrentDiscount.DiscountKey > 0)
                   {
                        RemoveMembershipDiscounts();
                        SeatOrders[SelectedSeat]->Orders->AppliedMembership.AutoAppliedDiscounts.clear();
                        TSCDPWDChecker SCDChecker;
                        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
                        DBTransaction.StartTransaction();
                        std::auto_ptr<TList> allOrders(new TList());
                        GetAllOrders(allOrders.get());
                        ManagerDiscount->GetDiscount(DBTransaction, CurrentDiscount.DiscountKey, CurrentDiscount);

                        if(CurrentDiscount.IsComplimentaryDiscount())
                        {
                          TypeOfSale = ComplimentarySale;
                        }
                        else if( CurrentDiscount.IsNonChargableDiscount())
                        {
                          TypeOfSale = NonChargableSale;
                        }

//                        if(SCDChecker.SeniorCitizensCheck(CurrentDiscount, allOrders.get()))
                        if((SCDChecker.SeniorCitizensCheck(CurrentDiscount, allOrders.get()))
                            && (SCDChecker.PWDCheck(CurrentDiscount, allOrders.get())))
                        {
                          SeatOrders[SelectedSeat]->Orders->AppliedMembership.AutoAppliedDiscounts.insert(CurrentDiscount.DiscountKey);
                          ApplyDiscount(DBTransaction, CurrentDiscount.DiscountKey, SeatOrders[SelectedSeat]->Orders->List,dsMMMembership);
                        }
                        RedrawSeatOrders();
                        TotalCosts();
                        UpdateExternalDevices();
                        TGlobalSettings::Instance().IsThorVoucherSelected = true;
                        DBTransaction.Commit();
                        if( TypeOfSale == ComplimentarySale ||  TypeOfSale == NonChargableSale)
                        {
                          tbtnTenderClick(tbtnTender);
                        }
                   }
                   else
                     {
                        MessageBox("No discount for this voucher has been setup in Menumate.", "Warning", MB_ICONWARNING + MB_OK);
                     }
                }
            }
        }
   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tbtnDiscountClick(bool combo)
{
	TMMContactInfo TempUserInfo;
	TempUserInfo = TDeviceRealTerminal::Instance().User;
	bool AllowDiscount = false;
	AnsiString DiscountMenu = "";
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff>Staff(new TContactStaff(DBTransaction));

	if (!AllowDiscount)
	{
		TLoginSuccess Result = Staff->Login(this, DBTransaction, TempUserInfo, combo ? CheckComboDiscounts :  CheckDiscountBill);
		if (Result == lsAccepted)
		{
			AllowDiscount = true;
		}
		else if (Result == lsDenied)
		{
			MessageBox("You do not have access rights to Discounts / Surcharges.", "Error", MB_OK + MB_ICONERROR);
		}
		else if (Result == lsPINIncorrect)
		{
			MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
		}
        if((SeatOrders[SelectedSeat]->Orders->AppliedMembership.ContactKey != 0) && (TPaySubsUtility::IsLocalLoyalty() &&
           !SeatOrders[SelectedSeat]->Orders->AppliedMembership.Points.PointsRulesSubs.Contains(eprAllowDiscounts)))
        {
            MessageBox("Discounts are disabled for this Member.", "INFORMATION", MB_OK + MB_ICONINFORMATION);
            AllowDiscount = false;
        }
	}
	DBTransaction.Commit();

	if (AllowDiscount)
	{
		std::auto_ptr<TfrmMessage> frmMessage(TfrmMessage::Create<TfrmMessage>(this, TDeviceRealTerminal::Instance().DBControl));
		frmMessage->MessageType = eDiscountReason;
		frmMessage->allow_combo = combo;
		if (frmMessage->ShowModal() == mrOk)
		{
			DBTransaction.StartTransaction();
			if (frmMessage->Key == -1)
			{
				ManagerDiscount->ClearDiscounts(SeatOrders[SelectedSeat]->Orders->List);
				RedrawSeatOrders();
				TotalCosts();
				UpdateExternalDevices();
			}
			else
			{
               TDiscount CurrentDiscount;
               TSCDPWDChecker SCDChecker;
               std::auto_ptr<TList> allOrders(new TList());
               GetAllOrders(allOrders.get());
               CurrentDiscount.DiscountKey = frmMessage->Key;
               ManagerDiscount->GetDiscount(DBTransaction, CurrentDiscount.DiscountKey, CurrentDiscount);

               if(CurrentDiscount.IsComplimentaryDiscount())
               {
                  TypeOfSale = ComplimentarySale;
               }
               else if( CurrentDiscount.IsNonChargableDiscount())
               {
                 TypeOfSale = NonChargableSale;
               }

//              if(SCDChecker.SeniorCitizensCheck(CurrentDiscount, allOrders.get()))
//              if((SCDChecker.SeniorCitizensCheck(CurrentDiscount, allOrders.get()))
//                  && (SCDChecker.PWDCheck(CurrentDiscount, allOrders.get())))
//              {
//                 ApplyDiscount(DBTransaction, frmMessage->Key, SeatOrders[SelectedSeat]->Orders->List);
//              }
                std::vector<TPatronType> patrons;
                patrons.clear();
                if(SelectedTable != 0)
                    patrons = TDBTables::GetPatronCount(DBTransaction, SelectedTable);
                std::auto_ptr<TSCDPatronUtility> patronUtility(new TSCDPatronUtility());
//                if(patronUtility->CanByPassSCDValidity(allOrders.get(),patrons,CurrentDiscount))
                if(patronUtility->CanByPassSCDValidity(SeatOrders[SelectedSeat]->Orders->List,patrons,CurrentDiscount))
                {
                    bool ProcessDiscount = SCDChecker.PWDCheck(CurrentDiscount, SeatOrders[SelectedSeat]->Orders->List);
                    if(ProcessDiscount)
                        ApplyDiscount(DBTransaction, frmMessage->Key, SeatOrders[SelectedSeat]->Orders->List);
                }
                else
                {
                    bool ProcessDiscount = SCDChecker.SeniorCitizensCheck(CurrentDiscount, SeatOrders[SelectedSeat]->Orders->List) &&
                                           SCDChecker.PWDCheck(CurrentDiscount, SeatOrders[SelectedSeat]->Orders->List);
                    if(ProcessDiscount)
                        ApplyDiscount(DBTransaction, frmMessage->Key, SeatOrders[SelectedSeat]->Orders->List);
                }
              RedrawSeatOrders();
              TotalCosts();
              UpdateExternalDevices();
            }

            DBTransaction.Commit();
            if( TypeOfSale == ComplimentarySale ||  TypeOfSale == NonChargableSale)
            {
              if(CheckDiscountPoints)               //MM-3908
              {
                  CheckDiscountPoints = false;      //MM-3908
              }
              else
              {
                tbtnTenderClick(tbtnTender);
              }
            }
        }
	 }
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::AdjustItemQty(Currency Count)
{
   TItemRedirector *redirector =  reinterpret_cast<TItemRedirector *>(lbDisplay->Items->Objects[lbDisplay->ItemIndex]);
   TItemComplete *selected_item = reinterpret_cast<TItemComplete *>(redirector->ParentRedirector->ItemObject);
   double oldQuantity = selected_item->GetQty();
   Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
   DBTransaction.StartTransaction();

   std::auto_ptr<TList>List(new TList());
   List->Add(selected_item);

   TManagerFreebie::UndoFreeCount(DBTransaction, List.get());
   // Adjust the QTY
   if(selected_item->WeightedPrice.WeightedItem)
   {
     TWeight weight;
     weight.SetWeightIn_Kg(Count);
	 selected_item->SetWeight(weight);
   }
   else
   {
        selected_item->SetQtyCustom(Count);
   }

   ManageDiscounts();

   // Redo the free count.
    if((SeatOrders[SelectedSeat]->Orders->AppliedMembership.ContactKey == 0) || (!TPaySubsUtility::IsLocalLoyalty()) ||
    SeatOrders[SelectedSeat]->Orders->AppliedMembership.Points.PointsRulesSubs.Contains(eprAllowDiscounts))
    {
       TManagerFreebie::IsPurchasing(DBTransaction, List.get());
    }
   CheckDeals(DBTransaction);
   DBTransaction.Commit();

   TotalCosts();
   RedrawSeatOrders();
   HighlightSelectedItem();

   UnicodeString pole_display_str = FormatFloat("0.00", selected_item->GetQty()) + " " + selected_item->Item;

   AnsiString PoleDisplayPrice =  CurrToStrF(((selected_item->TotalPriceAdjustment())), ffNumber, CurrencyDecimals);
   TDeviceRealTerminal::Instance().PoleDisplay->UpdatePoleDisplay(pole_display_str, PoleDisplayPrice, "", "");
   TDeviceRealTerminal::Instance().SecurityPort->SetData(pole_display_str + " " + PoleDisplayPrice);

}
// ---------------------------------------------------------------------------
void TfrmSelectDish::ManageDiscounts()
{
   std::map<TDateTime, TDiscountDetails> DiscountMap;
   bool isDiscountAdded = false;
   std::vector<TDiscountMode> exemptFilter;
   exemptFilter.push_back(DiscModeDeal);
   exemptFilter.push_back(DiscModeCombo);
   for (int i = 0; i < SeatOrders[SelectedSeat]->Orders->Count; i++)
   {
        isDiscountAdded = false;
		TItemMinorComplete *Item = SeatOrders[SelectedSeat]->Orders->Items[i];
		for(std::vector<TDiscount>::iterator ptrDiscount = Item->Discounts.begin(); ptrDiscount != Item->Discounts.end(); ++ptrDiscount)
        {
             TDiscount CurrentDiscount = *ptrDiscount;
             TDateTime discountTime = CurrentDiscount.DiscountAppliedTime;
             if(ptrDiscount->Source != dsMMMembership && ptrDiscount->Mode != DiscModeDeal && ptrDiscount->Mode != DiscModeCombo &&
               ptrDiscount->Source != dsMMLocationReward && ptrDiscount->Source != dsMMMembershipReward)
            {

                for (std::map<TDateTime, TDiscountDetails>::iterator i = DiscountMap.begin(); i != DiscountMap.end(); ++i)
                {
                    if(i->second.Discount.DiscountKey == ptrDiscount->DiscountKey && i->second.Discount.Mode == ptrDiscount->Mode)
                    {
                        if(i->second.Discount.Mode == DiscModeCurrency || i->second.Discount.Mode == DiscModePoints || i->second.Discount.Mode == DiscModeCombo)
                        {
                              if(i->second.Discount.OriginalAmount == ptrDiscount->OriginalAmount && i->second.DiscountTime == ptrDiscount->DiscountAppliedTime)
                              {
                                 isDiscountAdded = true;
                              }
                        }
                        if(i->second.Discount.Mode == DiscModePercent)
                        {
                              if(i->second.Discount.OriginalAmount == ptrDiscount->OriginalAmount && i->second.DiscountTime == ptrDiscount->DiscountAppliedTime)
                              {
                                 isDiscountAdded = true;
                              }
                        }
                        if(i->second.Discount.Mode == DiscModeItem)
                        {
                              if(i->second.Discount.OriginalAmount == ptrDiscount->OriginalAmount && i->first == ptrDiscount->DiscountAppliedTime)
                              {
                                 isDiscountAdded = true;
                              }
                        }
                    }
                }
                if(!isDiscountAdded)
                {
                    DiscountMap[CurrentDiscount.DiscountAppliedTime].OrderList = new TList();
                    DiscountMap[CurrentDiscount.DiscountAppliedTime].Discount = CurrentDiscount;
                    DiscountMap[CurrentDiscount.DiscountAppliedTime].DiscountTime = CurrentDiscount.DiscountAppliedTime;
                    DiscountMap[CurrentDiscount.DiscountAppliedTime].OrderList->Add(Item);
                }
                else
                {
                    DiscountMap[CurrentDiscount.DiscountAppliedTime].OrderList->Add(Item);
                }
            }
        }
	}
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    ApplyMemberDiscounts(DBTransaction,false);

    for (std::map<TDateTime, TDiscountDetails>::iterator i = DiscountMap.begin(); i != DiscountMap.end(); ++i)
    {
        TDiscount CurrentDiscount = i->second.Discount;
        if(CurrentDiscount.Mode == DiscModeCurrency || CurrentDiscount.Mode == DiscModePoints || CurrentDiscount.Mode == DiscModeItem || CurrentDiscount.Mode == DiscModeCombo)
        {
           CurrentDiscount.Amount = CurrentDiscount.OriginalAmount;
        }
        if(CurrentDiscount.Mode == DiscModePercent)
        {
           CurrentDiscount.PercentAmount = CurrentDiscount.OriginalAmount;
        }
        ApplyDiscount(DBTransaction, CurrentDiscount, i->second.OrderList, CurrentDiscount.Source);
    }

    DBTransaction.Commit();
}
// ---------------------------------------------------------------------------
bool TfrmSelectDish::ApplyDiscount(Database::TDBTransaction &DBTransaction, int DiscountKey, TList *Orders,
                                              bool isInitiallyApplied, TDiscountSource DiscountSource)
{
    bool isDiscountApplied = false;
    if (Orders->Count > 0)
    {
        TDiscount CurrentDiscount;
        CurrentDiscount.DiscountKey = DiscountKey;
        if(ManagerDiscount->GetDiscount(DBTransaction, CurrentDiscount.DiscountKey, CurrentDiscount))
        {
          isDiscountApplied = ApplyDiscount(DBTransaction, CurrentDiscount, Orders, isInitiallyApplied, DiscountSource);
        }
        else
        {
            MessageBox("Member Discount not found in discount table.", "Error", MB_ICONWARNING + MB_OK);
        }
        return isDiscountApplied;
    }
}
//------------------------------------------------------------------------------------------------------------------------------
bool TfrmSelectDish::PromptForDiscountDescription(TDiscount &currentDiscount)
{
     bool bailout = false;
     bool processDiscount = true;
     if (currentDiscount.Type == dtPromptDescription || currentDiscount.Type == dtPromptDescriptionAmount)
        {
            std::auto_ptr<TfrmTouchKeyboard>frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
            do
            {
                frmTouchKeyboard->MaxLength = 200;
                frmTouchKeyboard->AllowCarriageReturn = false;
                frmTouchKeyboard->StartWithShiftDown = true;
                frmTouchKeyboard->MustHaveValue = true;
                frmTouchKeyboard->KeyboardText = currentDiscount.Description;
                frmTouchKeyboard->Caption = "Please enter a discount / surcharge Description";
                if (frmTouchKeyboard->ShowModal() == mrOk)
                {
                    currentDiscount.Description = frmTouchKeyboard->KeyboardText;
                }
                else
                {
                    bailout = true;
                    CheckDiscountPoints = true;
                }
            }
            while (frmTouchKeyboard->KeyboardText == "" && !bailout);
            if (bailout)
            {
                processDiscount = false;
            }
        }
    return processDiscount;
}
//------------------------------------------------------------------------------------------------------------------------
bool TfrmSelectDish::PromptForDiscountAmount(TDiscount &currentDiscount)
{
  bool processDiscount = true;
  if (currentDiscount.Type == dtPromptAmount || currentDiscount.Type == dtPromptDescriptionAmount)
   {
        std::auto_ptr<TfrmDiscount>frmDiscount(TfrmDiscount::Create<TfrmDiscount>(this));
        frmDiscount->Mode = currentDiscount.Mode;
        frmDiscount->CURInitial = currentDiscount.Amount;
        frmDiscount->PERCInitial = currentDiscount.PercentAmount;
        frmDiscount->TotalValue = InitialMoney.GrandTotal;
        if(frmDiscount->Mode == DiscModeCombo)
        {
           frmDiscount->IsComboDiscount = true;
        }
        if (frmDiscount->ShowModal() == mrOk)
        {
            currentDiscount.Mode = frmDiscount->Mode;
            if (frmDiscount->Mode == DiscModeCurrency || frmDiscount->Mode == DiscModePoints)   // add points details for open discounts..
            {
                currentDiscount.Amount = RoundToNearest(frmDiscount->CURResult, MIN_CURRENCY_VALUE, TGlobalSettings::Instance().MidPointRoundsDown);
                currentDiscount.OriginalAmount = currentDiscount.Amount;
                if (currentDiscount.Amount != frmDiscount->CURResult)
                {
                   MessageBox("The Discount has been rounded!.", "Warning", MB_ICONWARNING + MB_OK);
                }
            }
           if (frmDiscount->Mode == DiscModeItem)
            {
                currentDiscount.Amount = RoundToNearest(frmDiscount->CURResult, MIN_CURRENCY_VALUE, TGlobalSettings::Instance().MidPointRoundsDown);
                currentDiscount.OriginalAmount = currentDiscount.Amount;
                if (currentDiscount.Amount != frmDiscount->CURResult)
                {
                   MessageBox("The Discount has been rounded!.", "Warning", MB_ICONWARNING + MB_OK);
                }
            }
            else if (frmDiscount->Mode == DiscModeSetPrice)
            {
                currentDiscount.Amount = RoundToNearest(frmDiscount->CURResult, MIN_CURRENCY_VALUE, TGlobalSettings::Instance().MidPointRoundsDown);
                currentDiscount.OriginalAmount = currentDiscount.Amount;
                if (currentDiscount.Amount != frmDiscount->CURResult)
                {
                   MessageBox("The Discount has been rounded!.", "Warning", MB_ICONWARNING + MB_OK);
                }
            }
            //add changes for open discount combo...
            else if (frmDiscount->Mode == DiscModeCombo)
            {
                currentDiscount.Amount = RoundToNearest(frmDiscount->CURResult, MIN_CURRENCY_VALUE, TGlobalSettings::Instance().MidPointRoundsDown);
                currentDiscount.OriginalAmount = currentDiscount.Amount;
                if (currentDiscount.Amount != frmDiscount->CURResult)
                {
                   MessageBox("The Discount has been rounded!.", "Warning", MB_ICONWARNING + MB_OK);
                }
            }
            else
            {
                 currentDiscount.PercentAmount = frmDiscount->PERCResult;
                 if(frmDiscount->Mode == DiscModePercent)
                 {
                    currentDiscount.OriginalAmount = currentDiscount.PercentAmount;
                 }
            }
        }
        else
        {
                processDiscount = false;
        }
        if(processDiscount)
        {
           currentDiscount.DiscountAppliedTime = Now();
        }

   }
   return processDiscount;
}
///------------------------------------------------------------------------------------------------------------------------
bool TfrmSelectDish::ApplyDiscount(Database::TDBTransaction &DBTransaction, TDiscount &CurrentDiscount, TList *Orders, bool isInitiallyApplied, TDiscountSource DiscountSource)
{
    if (Orders->Count == 0 )
     return false;

    bool ProcessDiscount = true;
    if(DiscountSource == dsMMMembership)
    {
       CurrentDiscount.IsThorBill = TGlobalSettings::Instance().MembershipType == MembershipTypeThor && TGlobalSettings::Instance().IsThorlinkSelected;
       if(CurrentDiscount.IsThorBill)
       {
            ManagerDiscount->GetThorlinkDiscount(DBTransaction,CurrentDiscount);
       }
    }

    if(CurrentDiscount.Source == dsMMUser || CurrentDiscount.Source == dsMMMembership)
    {
        CurrentDiscount.Source = DiscountSource;
    }
    else if(CurrentDiscount.Source == dsMMMebersPoints)
    {
        if(Membership.Applied())
        {
            Currency ProductValue = TOrderUtils::TotalPriceAdjustmentSides(Orders); //TOrderUtils::TotalPriceSides(Orders);
            CurrentDiscount.MaximumValue = Membership.Member.Points.getPointsBalance(pasDatabase,ptstLoyalty);
            CurrentDiscount.Amount = ProductValue;
            ManagerDiscount->ClearDiscount(Orders, CurrentDiscount);
            ManagerDiscount->AddDiscount(Orders, CurrentDiscount);
        }
        else
            ProcessDiscount = false;
    }
    //call prompt for description of discount
    if(isInitiallyApplied)
    {
        ProcessDiscount = PromptForDiscountDescription(CurrentDiscount);
    }

    if(ProcessDiscount && isInitiallyApplied)
    {
        ProcessDiscount = PromptForDiscountAmount(CurrentDiscount);
    }

    // apply open discount for membership
    if (ProcessDiscount)
    {

        if(CurrentDiscount.Mode == DiscModeCombo && checkQuantityAndSpliitItemsForCombo(Orders, DBTransaction))
        {
           RedrawSeatOrders();
        }
        ManagerDiscount->ClearDiscount(Orders, CurrentDiscount);
        ManagerDiscount->CheckDiscount(this, CurrentDiscount, &Orders);

        if(Orders == NULL)
        {
            CheckDeals(DBTransaction);
            return false;
        }
        for(int x = 0 ; x< Orders->Count; x++)
        {
          TItemMinorComplete * order = reinterpret_cast<TItemMinorComplete *>(Orders->Items[x]);
          order->DiscountByTypeRemove(DiscModeDeal);
        }

        for(int i = 0 ; i < Orders->Count ; i++)
        {
            TItemComplete *ic = (TItemComplete*)Orders->Items[i];
            for(int j = 0 ; j < ic->Discounts.size() ; j++)
            {
                 TDiscount DiscountItem = ic->Discounts[j];
                 if(DiscountItem.DiscountKey == CurrentDiscount.DiscountKey)
                 {
                     ManagerDiscount->ClearThorVouchersDiscounts(Orders);
                 }
            }
        }
        ///////////////////////////////////////////////////////////////////////
//        RestructureBillForSplit();
        ApplyDiscountWithRestructure(Orders, CurrentDiscount);
        ///////////////////////////////////////////////////////////////////////
        //ManagerDiscount->AddDiscount(Orders, CurrentDiscount);

        CheckDeals(DBTransaction);

        if( CurrentDiscount.Source == dsMMMebersPoints && SeatOrders[SelectedSeat]->Orders->AppliedMembership.ContactKey != 0)
        {
            TMMContactInfo &Member = SeatOrders[SelectedSeat]->Orders->AppliedMembership;
            TPointsTypePair Pair(pttRedeemed,ptstLoyalty);
            TPointsType Type(pasDiscount,Pair,pesNone);
            Membership.Member.Points.ClearBySource(pasDiscount);
            Membership.Member.Points.Load(Type, CurrentDiscount.Amount);
        }
        return true;
    }
    else
    {
         return false;
    }
}

// ---------------------------------------------------------------------------
bool TfrmSelectDish::checkQuantityAndSpliitItemsForCombo(TList *Orders, Database::TDBTransaction &DBTransaction)
{
    bool itemsSplitted = false;

    for (int i = 0; i < Orders->Count; i++)
	{
        TItemMinorComplete *or = reinterpret_cast<TItemMinorComplete *>(Orders->Items[i]);
        while( or->GetQty() > 1)
        {
            /*
             * executes only for manual quantity modifications
             * split the items in the original order
             * add them to the original seat's collection
             * reduce the quantity of current item
             */

             TItemComplete *splitItem = new TItemComplete(*reinterpret_cast<TItemComplete*>(or));
             TItemMinorComplete *sp_order = reinterpret_cast<TItemMinorComplete*>(splitItem);
             sp_order->SetQty(1);
             /*
              * because this is now a seperate order, we need to have a new transaction number applied to it
              */
             sp_order->TransNo = TDBOrder::GetNextTransNumber(DBTransaction);

             Orders->Add(sp_order);
             or->SetQty(or->GetQty() - 1);
             itemsSplitted = true;
         }
    }

    return itemsSplitted;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Membership
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ---------------------------------------------------------------------------
void TfrmSelectDish::RemoveMembershipDiscounts()
{
	for (int i = 0; i < SeatOrders[SelectedSeat]->Orders->Count; i++)
	{
		TItemMinorComplete *Item = SeatOrders[SelectedSeat]->Orders->Items[i];
		Item->DiscountByTypeRemove(dsMMMembership);
	}
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::ChangeCard()
{
  try
   {
      if(TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->CardInserted)
      {
         TDeviceRealTerminal::Instance().ManagerMembership->RestoreCardWithPoints();
         OnSmartCardInserted(NULL);
      }
      else
      {
         MessageBox("Please Insert Card in Reader", "Error No Card", MB_OK + MB_ICONERROR);
      }
   }
   catch(Exception &E)
    {
      MessageBox(E.Message, "Error", MB_OK + MB_ICONERROR);
    }
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::AssignBarcodeToMember()
{
  Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
  DBTransaction.StartTransaction();
  try
   {
      if(TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->CardInserted)
      {
         AnsiString memberCardCode = "";
         TMMContactInfo TempUserInfo;
	     TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->GetContactInfo(TempUserInfo);
         TempUserInfo.ContactKey = TDBContacts::GetContactByMemberNumberSiteID(DBTransaction, TempUserInfo.MembershipNumber,TempUserInfo.SiteID);
         std::auto_ptr <TfrmCardSwipe> frmCardSwipe(TfrmCardSwipe::Create <TfrmCardSwipe> (this));
         frmCardSwipe->tbOtherOpt->Visible = false;
         frmCardSwipe->ShowModal();
         if (frmCardSwipe->ModalResult == mrOk)
           {
              memberCardCode = AnsiString(frmCardSwipe->SwipeString).SubString(1, 50);
              if(TDeviceRealTerminal::Instance().ManagerMembership->UpdateMemberCardCode(DBTransaction, TempUserInfo, memberCardCode))
              {
                TDBContacts::UpdateMemberCardCodeToDB(DBTransaction, TempUserInfo, memberCardCode);
                TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->FormatCardToFactory();
                MessageBox("Please Remove Card From Reader.", "Information", MB_OK);
              }
           }

         DBTransaction.Commit();
      }
      else
      {
         MessageBox("Please Insert Card in Reader", "Error No Card", MB_OK + MB_ICONERROR);
      }
   }
   catch(Exception &E)
    {
      DBTransaction.Rollback();
      MessageBox(E.Message, "Error", MB_OK + MB_ICONERROR);
    }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tbtnMemberDisplayPageUpMouseClick(TObject *Sender)
{
	TComInterface<IHTMLDocument2>HTMLDocument;
	TComInterface<IHTMLWindow2>parentWindow;

	_di_IDispatch doc = webDisplay->Document;
	if (doc != NULL)
	{
		if (SUCCEEDED(webDisplay->Document->QueryInterface(IID_IHTMLDocument2, (LPVOID*) & HTMLDocument)))
		{
			if (SUCCEEDED(HTMLDocument->get_parentWindow(&parentWindow)))
			{
				parentWindow->scrollBy(0, -400);
			}
		}
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tbtnMemberDisplayPageDownMouseClick(TObject *Sender)
{
	TComInterface<IHTMLDocument2>HTMLDocument;
	TComInterface<IHTMLWindow2>parentWindow;
    //HTMLDocument2Ifc.execCommand('Bold', false,0);
    //((Sender as TWebBrowser).Document as IHTMLDocument2)->body.style.fontFamily:='Arial';


	_di_IDispatch doc = webDisplay->Document;
	if (doc != NULL)
	{
		if (SUCCEEDED(webDisplay->Document->QueryInterface(IID_IHTMLDocument2, (LPVOID*) & HTMLDocument)))
		{
			if (SUCCEEDED(HTMLDocument->get_parentWindow(&parentWindow)))
			{
				parentWindow->scrollBy(0, 400);
			}
		}
	}
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::OnSmartCardInserted(TSystemEvents *Sender)
{
  TDeviceRealTerminal &drt = TDeviceRealTerminal::Instance();
  if(!Membership.Applied() && (Active || drt.ManagerMembership->ManagerSmartCards->CardInserted))
  {
	TMMContactInfo info;
    drt.ManagerMembership->ManagerSmartCards->GetContactInfo(info);
	if (TSmartcard_Preloader::is_preloaded_card(info))
    {
		if (!TGlobalSettings::Instance().QMSIsEnabled || ignore_preloaded_card ||
           (ignore_preloaded_card = drt.ManagerMembership->AddMember(info, false ,true)) != mbOK)
			  return;
	}

	if (info.Valid())
     {
		Database::TDBTransaction DBTransaction(drt.DBControl);
		TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
		DBTransaction.StartTransaction();
        TManagerLoyaltyVoucher ManagerLoyaltyVoucher;
        ManagerLoyaltyVoucher.DisplayMemberVouchers(DBTransaction,info);
		ApplyMembership(DBTransaction, info);
		DBTransaction.Commit();
	}
  }
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::OnSmartCardRemoved(TSystemEvents *Sender)
{
	if (TDeviceRealTerminal::Instance().Modules.Status[eSmartCardSystem]["Enabled"])
	{
		for (int i = 0; i < lbDisplay->Items->Count; i++)
		{
			TItemRedirector *ListItem = (TItemRedirector*)lbDisplay->Items->Objects[i];
			if (ListItem->ItemType.Contains(itMembershipDisplay) || ListItem->ItemType.Contains(itMembershipDisplayNote))
			{
				lbDisplay->ItemIndex = i;
				ListItem->CompressedContainer->Container->CurrentItemRedirector = ListItem;
				ListItem->CompressedContainer->Container->SelectedIndex = lbDisplay->ItemIndex;
				btnRemoveMouseClick(NULL);
			}
		}

		ignore_preloaded_card = false;
	}
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::RemoveMembership(Database::TDBTransaction &DBTransaction)
{
    try
    {
        if (bool(TDeviceRealTerminal::Instance().Modules.Status[eSmartCardSystem]["Enabled"]) &&
                 TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->CardOk)
        {
            MessageBox("To Remove Membership, Remove the Smart Card from the Reader.", "Error", MB_OK + MB_ICONERROR);
        }
        else
        {

            // Remove all Free Items.
            TManagerFreebie::UndoFreeCount(DBTransaction, SeatOrders[SelectedSeat]->Orders->List);
            for (int i = 0; i < SeatOrders[SelectedSeat]->Orders->Count; i++)
            {
                TItemComplete *Item = SeatOrders[SelectedSeat]->Orders->Items[i];
                // Calculate Membership Pricing.
                Item->Loyalty_Key = 0;
                Item->ResetPrice();

                for (int j = 0; j < Item->SubOrders->Count; j++)
                {
                    TItemMinorComplete *CurrentSubOrder = (TItemMinorComplete*)Item->SubOrders->Items[j];
                    CurrentSubOrder->Loyalty_Key = 0;
                    CurrentSubOrder->ResetPrice();
                }
            }

            SeatOrders[SelectedSeat]->Orders->AppliedMembership.Clear();
            Membership.Clear();
            RemoveMembershipDiscounts();
            TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->ResetPoints();
        }
    }
    catch(Exception & E)
    {
         TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

// ---------------------------------------------------------------------------
void TfrmSelectDish::ApplyMembership(Database::TDBTransaction &DBTransaction, TMMContactInfo &Member)
{
     customerDisp.HappyBirthDay = false;
     customerDisp.FirstVisit = false;
	 eMemberSource MemberSource = emsManual;
	 TLoginSuccess Result = TDeviceRealTerminal::Instance().ManagerMembership->GetMember(DBTransaction, Member, MemberSource);

     if((TPaySubsUtility::IsLocalLoyalty() && !Member.Points.PointsRulesSubs.Contains(eprAllowDiscounts)) && Result == lsAccepted)
     {
        ManagerDiscount->ClearDiscounts(SeatOrders[SelectedSeat]->Orders->List);
        CheckDeals(DBTransaction);
     }

     if (Result == lsAccountBlocked)
      {
            MessageBox("Account Blocked " + Member.Name + " " + Member.AccountInfo, "Account Blocked", MB_OK + MB_ICONINFORMATION);
      }
	 else if (Result == lsAccepted)
	  {

		bool ApplyToAllSeats = false;
		if (SelectedTable != 0 && !LoyaltyPending())
		{
			if (MessageBox("Do you wish to apply this membership to all seats?", "Query", MB_YESNO + MB_ICONQUESTION) == IDYES)
			{
				ApplyToAllSeats = true;
			}
		}
        if(TGlobalSettings::Instance().LoyaltyMateEnabled)
        {
           TManagerDiscount managerDiscount;
           managerDiscount.GetMembershipDiscounts(DBTransaction,Member.AutoAppliedDiscounts);
        }
		SeatOrders[SelectedSeat]->Orders->AppliedMembership = Member;
		Membership.Assign(Member, MemberSource);
		// Sort out Free Drinks and stuff.
		std::vector<int>SeatsToApply;
		if (ApplyToAllSeats == true)
		{
			for (UINT iSeat = 0; iSeat < SeatOrders.size(); iSeat++)
			{
				SeatsToApply.push_back(iSeat);
			}
		}
		else
		{
			SeatsToApply.push_back(SelectedSeat);
		}

		for (UINT iSeat = 0; iSeat < SeatsToApply.size(); iSeat++)
		{
			// Clear the Member Discounts from the orders.
			ManagerDiscount->ClearMemberDiscounts(SeatOrders[SeatsToApply[iSeat]]->Orders->List);
            ManagerDiscount->ClearThorVouchersDiscounts(SeatOrders[SelectedSeat]->Orders->List);

			SeatOrders[SeatsToApply[iSeat]]->Orders->AppliedMembership = Member;

			for (int i = 0; i < SeatOrders[SeatsToApply[iSeat]]->Orders->Count; i++)
			{
				TItemComplete *Order = SeatOrders[SeatsToApply[iSeat]]->Orders->Items[i];
                if(TPaySubsUtility::IsLocalLoyalty() && !Member.Points.PointsRulesSubs.Contains(eprAllowDiscounts))
                {
                    ChitNumber.DiscountList.clear();
                    Order->ClearAllDiscounts();
                }
				Order->Loyalty_Key = SeatOrders[SeatsToApply[iSeat]]->Orders->AppliedMembership.ContactKey;
				Order->ResetPrice();

				// Sort out the Sides.
				for (int j = 0; j < Order->SubOrders->Count; j++)
				{
					TItemMinorComplete *CurrentSubOrder = (TItemMinorComplete*)Order->SubOrders->Items[j];
                    if(TPaySubsUtility::IsLocalLoyalty() && !Member.Points.PointsRulesSubs.Contains(eprAllowDiscounts))
                    {
                       CurrentSubOrder->ClearAllDiscounts();
                    }
					CurrentSubOrder->Loyalty_Key = SeatOrders[SeatsToApply[iSeat]]->Orders->AppliedMembership.ContactKey;
					CurrentSubOrder->ResetPrice();
				}

                if((SeatOrders[SelectedSeat]->Orders->AppliedMembership.ContactKey == 0 ) || (!TPaySubsUtility::IsLocalLoyalty())
                     || SeatOrders[SelectedSeat]->Orders->AppliedMembership.Points.PointsRulesSubs.Contains(eprAllowDiscounts))
                   ManagerDiscount->AddDiscountsByTime(DBTransaction, Order);
			}

			// Calculate Members Freebie rewards.
			// Rewards do not cascade though sides, which is the normal discount functionality.
          if((!TPaySubsUtility::IsLocalLoyalty()) ||
             SeatOrders[SelectedSeat]->Orders->AppliedMembership.Points.PointsRulesSubs.Contains(eprAllowDiscounts))
			 TManagerFreebie::IsPurchasing(DBTransaction, SeatOrders[SeatsToApply[iSeat]]->Orders->List);

			// Apply Member Specific Discounts.
			ApplyMemberDiscounts(DBTransaction);

		}
		RedrawSeatOrders();
        if((TGlobalSettings::Instance().IsDrinkCommandEnabled))
        {
            int contactKey = Member.ContactKey;
            AnsiString memberPoints = Member.Points.getPointsBalance();
            memberPoints = memberPoints * 100;
            AnsiString memNo = Member.MembershipNumber;
            int memPoints = atoi(memberPoints.c_str());

            // Asks for DC Session only if DC is enabled and points are more than 0
            // now prompt does not ask for consent to initialize session
            if((memPoints > 0))
            {
                 YesGoForSessionWithDC(memPoints, memberPoints,memNo,contactKey);
            }
            else
            {
               TMMProcessingState State(Screen->ActiveForm, "Redirecting to Purchase Points", "Please Wait");
               TDeviceRealTerminal::Instance().ProcessingController.Push(State);
               Sleep(1200);
               tbtnTenderClick(tbtnTender);                                      // Redirecting to Payment Screen #7678
               TDeviceRealTerminal::Instance().ProcessingController.Pop();
            }
        }
		TotalCosts();
		UpdateExternalDevices();

        eMemberNameOnPoleDisplay nameOnPoleDisplay = TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->NameOnPoleDisplay;

		if ( nameOnPoleDisplay != eMNPDNone)
		{
            Member.PoleDisplayName = Member.RefreshPoleDisplayName( nameOnPoleDisplay );

			if( TGlobalSettings::Instance().ShowPointsOnDisplay )
			{
				AnsiString ShowPoints = FloatToStr(Member.Points.getPointsBalance());
                TDeviceRealTerminal::Instance().PoleDisplay->UpdatePoleDisplay(Member.PoleDisplayName, "", "Points:", ShowPoints);
				TDeviceRealTerminal::Instance().SecurityPort->SetData(Member.PoleDisplayName);
				TDeviceRealTerminal::Instance().SecurityPort->SetData("Points:" + ShowPoints);
			}
			else
			{
				TDeviceRealTerminal::Instance().PoleDisplay->UpdatePoleDisplay("Welcome", Member.PoleDisplayName);
				TDeviceRealTerminal::Instance().SecurityPort->SetData("Welcome " + Member.PoleDisplayName);
			}
			LastSale = 0;
		}
		TMembership *membershipSystem = TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem.get();
        customerDisp.HappyBirthDay = TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->DisplayBirthDayNotification(DBTransaction,Member);
        customerDisp.FirstVisit=TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->DisplayFirstVisitNotification(DBTransaction,Member);
		AnsiString ShowPoints = FloatToStr( Member.Points.getPointsBalance() );
        AnsiString name = Member.PoleDisplayName;
		TDeviceRealTerminal::Instance().SecurityPort->SetData(Member.PoleDisplayName);
		TDeviceRealTerminal::Instance().SecurityPort->SetData("Points:" + ShowPoints);
        if(!TGlobalSettings::Instance().IsThorlinkTender)
        {
            GetThorVouchers();
            TGlobalSettings::Instance().IsThorlinkTender = false;
            TGlobalSettings::Instance().IsThorVoucherSelected = false;
        }
		LastSale = 0;
    }
}
// ---------------------------------------------------------------------------
void TfrmSelectDish::GetMemberByBarcode(Database::TDBTransaction &DBTransaction,AnsiString Barcode)
{
 	TDeviceRealTerminal &drt = TDeviceRealTerminal::Instance();
	TMMContactInfo info;
    bool memberExist = drt.ManagerMembership->LoyaltyMemberSelected(DBTransaction,info,Barcode,true);
    if(memberExist)
     {
     if (info.Valid())
     {
        TManagerLoyaltyVoucher ManagerLoyaltyVoucher;
        ManagerLoyaltyVoucher.DisplayMemberVouchers(DBTransaction,info);
		ApplyMembership(DBTransaction, info);
     }
     }

}
// ---------------------------------------------------------------------------
void TfrmSelectDish::GetLoyaltyMember(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info)
{

     eMemberSource MemberSource = emsManual;
     TLoginSuccess Result = TDeviceRealTerminal::Instance().ManagerMembership->GetMember(DBTransaction, Info, MemberSource);

     if (Result == lsAccountBlocked)
      {
            MessageBox("Account Blocked " + Info.Name + " " + Info.AccountInfo, "Account Blocked", MB_OK + MB_ICONINFORMATION);
      }
	 else if (Result == lsAccepted)
	  {
        if(Info.ContactKey != 0)
        {
            TDeviceRealTerminal &drt = TDeviceRealTerminal::Instance();
            bool memberExist = drt.ManagerMembership->LoyaltyMemberSelected(DBTransaction,Info,Info.MemberCode,false);

            if(memberExist)
             {
             if (Info.Valid())
             {
                TManagerLoyaltyVoucher ManagerLoyaltyVoucher;
                ManagerLoyaltyVoucher.DisplayMemberVouchers(DBTransaction,Info);
                ApplyMembership(DBTransaction, Info);
                }
             }
         }
     }

}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TCustNameAndOrderType
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

TCustNameAndOrderType* TCustNameAndOrderType::CustNameAndOrderType = NULL;
// ---------------------------------------------------------------------------
TCustNameAndOrderType* TCustNameAndOrderType::Instance()
{
	if(!CustNameAndOrderType)
	{
		CustNameAndOrderType = new TCustNameAndOrderType();
	}
	return CustNameAndOrderType;
}
// ---------------------------------------------------------------------------
TCustNameAndOrderType::TCustNameAndOrderType()
{
	CustNameKey = -1;
    OrderTypeMessage = "";
    NameCaught = false;
}
// ---------------------------------------------------------------------------
TCustNameAndOrderType::~TCustNameAndOrderType()
{

}
// ---------------------------------------------------------------------------
void TCustNameAndOrderType::CatchCustNameAndOrderType(int CustKey, AnsiString OrderType, std::vector <TSeatOrders*>& SeatOrders)
{
    CustNameKey = CustKey;
    OrderTypeMessage = OrderType;
    for (UINT iSeat = 0; iSeat < SeatOrders.size(); iSeat++)
    {
        for (int i = 0; i < SeatOrders[iSeat]->Orders->Count; i++)
        {
            SeatOrders[iSeat]->Orders->Items[i]->ContactsKey = CustNameKey;
            SeatOrders[iSeat]->Orders->Items[i]->OrderTypeMessage = OrderTypeMessage;
        }
    }
    NameCaught = true;
}
// ---------------------------------------------------------------------------
void TCustNameAndOrderType::LoadFromOrdersDatabase(int TableNumber)
{
    //Load contacts key and order type message from database
    try
    {
        std::pair<int, AnsiString> CustNameAndOrderType;
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction.StartTransaction();
        TDBOrder::GetCNaOTFromTableNum(DBTransaction, TableNumber, CustNameAndOrderType);
        DBTransaction.Commit();
        if(CustNameAndOrderType.first != -1)
        {
            CustNameKey = CustNameAndOrderType.first;
            OrderTypeMessage = CustNameAndOrderType.second;
        }
        else
        {
            NameCaught = false;
        }
    }
    catch(Exception & E)
    {
         TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}
// ---------------------------------------------------------------------------
void TCustNameAndOrderType::ClearNameCaught()
{
    NameCaught = false;
}
// ---------------------------------------------------------------------------
AnsiString TCustNameAndOrderType::GetCustNameFromKey(int Key)
{
    AnsiString CustomerName = "";
    if (Key != -1)
    {
        Database::TDBTransaction CustomerTransaction(TDeviceRealTerminal::Instance().DBControl);
        CustomerTransaction.StartTransaction();
        std::auto_ptr<TContact> Customer(new TContact());
        Customer->Enabled = true;
        Customer->ContactType = eCasualCustomer;
        CustomerName = Customer->GetContactName(CustomerTransaction, Key);
        CustomerTransaction.Commit();
    }
    return CustomerName;
}
// ---------------------------------------------------------------------------
std::pair<AnsiString, AnsiString> TCustNameAndOrderType::GetStringPair()
{
	if (TGlobalSettings::Instance().CaptureCustomerName)
	{
		AnsiString custName     = GetCustNameFromKey( CustNameKey );
		AnsiString orderTypeMsg = OrderTypeMessage;

		return make_pair( custName, orderTypeMsg );
	}
	else
	{
		return make_pair( "", "" );
	}
}
// ---------------------------------------------------------------------------
bool TCustNameAndOrderType::Valid()
{
	return CustNameKey != -1 && OrderTypeMessage != "";
}
// ---------------------------------------------------------------------------
bool TCustNameAndOrderType::NameAndOrderTypeLoaded()
{
	return TGlobalSettings::Instance().CaptureCustomerName && (CustNameKey != -1 || OrderTypeMessage != "");
}


void TfrmSelectDish::SendPointValueToRunRate( TPaymentTransaction &inTransaction )
{
	if( TGlobalSettings::Instance().IsRunRateBoardEnabled )
	{
        TRunRateClient *cmClient = new TRunRateClient();
        cmClient->SendPointDetailsToRunRate( inTransaction );
	}
}
//-----------------------------------------------------------------------------------------------------
void TfrmSelectDish::DoCloundSync()
{
  if (TGlobalSettings::Instance().LoyaltyMateEnabled)
     {
        TManagerCloudSync ManagerCloudSync;
        ManagerCloudSync.SyncCompanyDetails();
        ManageDiscounts();
        TotalCosts();
        RedrawSeatOrders();
        HighlightSelectedItem();
     }
}
//----------------------------------------------------------------------------------------------------------------------
void TfrmSelectDish::CheckGiftCardBalance()
{
    std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
    frmTouchKeyboard->MaxLength = 50;
    frmTouchKeyboard->AllowCarriageReturn = false;
    frmTouchKeyboard->StartWithShiftDown = false;
    frmTouchKeyboard->MustHaveValue = true;
    frmTouchKeyboard->KeyboardText = "";
    frmTouchKeyboard->Caption = "Enter Gift Card Number";
    if (frmTouchKeyboard->ShowModal() == mrOk && frmTouchKeyboard->KeyboardText.Trim() != "")
    {
        AnsiString giftCardNumber = frmTouchKeyboard->KeyboardText.Trim();
        TManagerLoyaltyVoucher ManagerLoyaltyVoucher;
        TGiftCardDetail GiftCardDetail;
        ManagerLoyaltyVoucher.GetGiftVoucherDetail(giftCardNumber,GiftCardDetail);
        switch(GiftCardDetail.StatusCode)
        {
             case 1:
             case 3:
             case 4:
             {
                 AnsiString messageString = "Gift Card ("+ giftCardNumber +") - Remaining Balance is "+ FormatFloat("0.00",GiftCardDetail.PointBalance);
                 if((double)GiftCardDetail.StartDate > double(0) && GiftCardDetail.StatusCode == 4)
                    messageString += " and Valid From " + GiftCardDetail.StartDate.FormatString("DD-MM-YYYY");
                  if((double)GiftCardDetail.ExpiryDate > double(0))
                    messageString += " and Expiry Date is " + GiftCardDetail.ExpiryDate.FormatString("DD-MM-YYYY");
                 MessageBox(messageString, "Information", MB_OK + MB_ICONINFORMATION);
             }
             break;
             case 2:
             MessageBox("Gift Card not found please try another card.", "Warning", MB_OK + MB_ICONINFORMATION);
             break;
        }
    }
}
//----------------------------------------------------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tedtSearchItemChange(TObject *Sender)
{
    if(itemSearch || tedtSearchItem->Text == "")
    {
        TfrmProductSearch*  frmproductSearch = new TfrmProductSearch(this);
        frmproductSearch->LastSearchIndex = 0;
        bool isItemFound = frmproductSearch->QuickSearch(tedtSearchItem->Text);

        if(isItemFound)
        {
            frmproductSearch->DisplayData();
        }
        else
        {
            frmproductSearch->tListItems->ItemIndex = -1;
            frmproductSearch->DisplayData();
        }
        delete frmproductSearch;
    }
}
//----------------------------------------------------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tedtSearchItemKeyPress(TObject *Sender, wchar_t &Key)
{

    if (Key == VK_RETURN)
    {
        AddSearchedItemToSeat();
        tedtSearchItem->Text = "";
        DefocusControl(tedtSearchItem, false);
    }
}
 //-------------------------------------------------------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tbtnSearchMouseClick(TObject *Sender)
{
     SearchItems();
     DefocusControl(tedtSearchItem, false);
}
//------------------------------------------------------------------------------------------------------------------------------
void TfrmSelectDish::AddSearchedItemToSeat()
{
    TfrmProductSearch*  frmProductSearch = new TfrmProductSearch(this);
    frmProductSearch->SearchText = tedtSearchItem->Text;
    frmProductSearch->QuickSearch(tedtSearchItem->Text);
    frmProductSearch->LoadSelectedItemInformation();
    int modalResult;
    if(frmProductSearch->tListItems->Count == 1)
    {
        modalResult = mrOk;
    }
    else if(frmProductSearch->tListItems->Count == 0)
    {
        MessageBox("Item not found.", "Error", MB_ICONWARNING + MB_OK);
    }
    else
    {
        modalResult = frmProductSearch->ShowModal();
    }
    if (modalResult == mrOk)
    {
        std::pair<TItem*, TItemSize*> itemAndSize = frmProductSearch->GetSelectedItemAndItemSize();
        OrderSearchedItem(itemAndSize);
    }
    delete frmProductSearch;
}
//------------------------------------------------------------------------------------------------------------------------------
void TfrmSelectDish:: OrderSearchedItem(std::pair<TItem*, TItemSize*> &itemAndSize)
{
    TItem *Item = itemAndSize.first;
    TItemSize *ItemSize = itemAndSize.second;

    bool ItemFound = Item != NULL && ItemSize != NULL;
    bool isSameMenuTypeItemExist = true;

    if(TGlobalSettings::Instance().IsBillSplittedByMenuType && ItemFound)
        isSameMenuTypeItemExist = CheckItemCanBeAddedToSeat(Item);

    if (ItemFound && Item->Enabled && isSameMenuTypeItemExist)
    {
        Always_Prompt = CheckForServingCoursePrompt(Item->ItemKey);
        BeforeItemOrdered.Occured();
        if (BeforeItemOrdered.EventsFailed)
        {
            MessageBox(BeforeItemOrdered.ErrorMessage, BeforeItemOrdered.ErrorTitle, MB_OK + MB_ICONERROR);
        }
        else
        {
            Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
            DBTransaction.StartTransaction();
            if(Always_Prompt)
            {
                AddItemToSeat(DBTransaction, Item, false, ItemSize);
            }
            else
            {
                AddItemToSeat(DBTransaction, Item, false, ItemSize, 0, true);
            }
            DBTransaction.Commit();
        }

        AfterItemOrdered.Occured();
        if (AfterItemOrdered.EventsFailed)
        {
            MessageBox(AfterItemOrdered.ErrorMessage, AfterItemOrdered.ErrorTitle, MB_OK + MB_ICONERROR);
        }
    }
}
//-----------------------------------------------------------------------------------------------------------------------------
bool TfrmSelectDish::IsTextBoxFocused()
{
    if(tedtSearchItem->Focused())
    {
        return true;
    }
    else
    {
        return false;
    }

}
//-----------------------------------------------------------------------------------------------------------------------------
void TfrmSelectDish::SetPOSBackgroundColor()
{
    if(TGlobalSettings::Instance().ShowDarkBackground)
    {
        frmSelectDish->Color = 14342874;
        Panel1->Color = 14342874;
        tgridOrderItem->GridColor = 14342874;
        pnlItemPickerBorder->Color = 14342874;
        lbDisplay->Color = 14342874;
        tbtnWebOrders->ButtonColor = 14342874;
        tbtnParkSales->ButtonColor = 14342874;
    }
    else
    {
        frmSelectDish->Color = clWhite;
        Panel1->Color = clWhite;
        tgridOrderItem->GridColor = clWhite;
        pnlItemPickerBorder->Color = clWhite;
        lbDisplay->Color = clWhite;
        tgridOrderCourse->GridColor = RGB(255,255,255);
        tgridOrderCourse->Color	= RGB(255,255,255);
        tgridServingCourse->GridColor = RGB(255,255,255);
        tgridServingCourse->Color	= RGB(255,255,255);
        tbtnWebOrders->ButtonColor = clWhite;
        tbtnParkSales->ButtonColor = clWhite;
    }

}
//-----------------------------------------------------------------------------------------------------------------------------
void TfrmSelectDish::UpdateMenuItemsAfterLoginScreen()
{
    RedrawItems();
    RedrawItemSideCourses();
    RedrawSetMenuItems();
}
//-----------------------------------------------------------------------------------------------------------------------------
int TfrmSelectDish::GetDefaultServingCourse(int item_key)
{
    int retVal = 0;
	try
	{
        Database::TDBTransaction transaction(TDeviceRealTerminal::Instance().DBControl);
        transaction.StartTransaction();
		TIBSQL *IBInternalQuery = transaction.Query(transaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			" Select a.SERVINGCOURSES_KEY FROM COURSE a inner join Item b on b.COURSE_KEY = a.COURSE_KEY where b.ITEM_KEY = :ITEM_KEY " ;
		IBInternalQuery->ParamByName("ITEM_KEY")->AsInteger = item_key;
		IBInternalQuery->ExecQuery();
        if(IBInternalQuery->RecordCount)
        {
             retVal	= IBInternalQuery->FieldByName("SERVINGCOURSES_KEY")->AsInteger;
        }
        transaction.Commit();
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
   return retVal;
}
//-----------------------------------------------------------------------------------------------------------------------------
bool TfrmSelectDish::CheckForServingCoursePrompt(int item_key)
{
    bool retVal = false;
	try
	{
        Database::TDBTransaction transaction(TDeviceRealTerminal::Instance().DBControl);
        transaction.StartTransaction();
		TIBSQL *IBInternalQuery = transaction.Query(transaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			" SELECT a.NO_DEFAULT_SERVING_COURSE FROM COURSE a INNER JOIN ITEM b on b.COURSE_KEY = a.COURSE_KEY where b.ITEM_KEY = :ITEM_KEY " ;
		IBInternalQuery->ParamByName("ITEM_KEY")->AsInteger = item_key;
		IBInternalQuery->ExecQuery();
        if(IBInternalQuery->RecordCount)
        {
             retVal = (IBInternalQuery->FieldByName("NO_DEFAULT_SERVING_COURSE")->AsString == "T") ? true : false;
        }
        transaction.Commit();
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
   return retVal;
}
//-----------------------------------------------------------------------------------------------------------------------------
bool TfrmSelectDish::CheckItemCanBeAddedToSeat(TItem *item)
{
    bool isMenuTypeSame = true;

    if(SeatOrders[SelectedSeat]->Orders->Count && !SelectedTable)
    {
        if(item->ItemType != SeatOrders[SelectedSeat]->Orders->Items[0]->ItemType)
        {
            isMenuTypeSame = false;
            MessageBox("Items with different menu types can't be ordered at the same time.", "Error", MB_ICONWARNING + MB_OK);
        }
    }

    return isMenuTypeSame;
}
//---------------------------------------------------------------------------------------------------------
void TfrmSelectDish::LoadFoodAndBevList(TList *foodOrdersList, TList *bevOrdersList)
{
    for (UINT iSeat = 0; iSeat < SeatOrders.size(); iSeat++)
    {
        for (int i = 0; i < SeatOrders[iSeat]->Orders->Count; i++)
        {
            TItemComplete* item = SeatOrders[iSeat]->Orders->Items[i];

            if(item->ItemType)
                bevOrdersList->Add(item);
            else
                foodOrdersList->Add(item);
        }
    }
}
//----------------------------------------------------------------------------
bool TfrmSelectDish::CheckIfSubsidizedDiscountValid(int tabKey)
{
    bool retValue = true;
    AnsiString message = "";
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
	try
	{
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

        IBInternalQuery->SQL->Text = "SELECT d.DISCOUNT_MODE, d.AMOUNT, d.PERCENTAGE FROM TABDISCOUNTS t "
                                     "LEFT JOIN DISCOUNTS d on t.DISCOUNT_KEY = d.DISCOUNT_KEY WHERE "
                                     "t.TAB_KEY = :TAB_KEY";
        IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = tabKey;
        IBInternalQuery->ExecQuery();
        for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            switch(IBInternalQuery->FieldByName("DISCOUNT_MODE")->AsInteger)
            {
                case DiscModeCurrency:
                {
                    if(IBInternalQuery->FieldByName("AMOUNT")->AsCurrency == 0)
                    {
                        retValue = false;
                    }
                    break;
                }
                case DiscModePercent:
                {
                    if((double)IBInternalQuery->FieldByName("PERCENTAGE")->AsCurrency == 0 ||
                      (double)IBInternalQuery->FieldByName("PERCENTAGE")->AsCurrency == 100)
                    {
                        retValue = false;
                    }
                    break;
                }
                case DiscModeSetPrice:
                {
                    if(IBInternalQuery->FieldByName("AMOUNT")->AsCurrency == 0)
                    {
                        retValue = false;
                    }
                    break;
                }
                case DiscModeCombo:
                {
                    if(IBInternalQuery->FieldByName("AMOUNT")->AsCurrency == 0)
                    {
                        retValue = false;
                    }
                    break;
                }
                case DiscModeDeal:
                {
                    if(IBInternalQuery->FieldByName("AMOUNT")->AsCurrency == 0)
                    {
                        retValue = false;
                    }
                    break;
                }
                case DiscModeItem:
                {
                    if(IBInternalQuery->FieldByName("AMOUNT")->AsCurrency == 0)
                    {
                        retValue = false;
                    }
                    break;
                }
                default:
                   break;
            }
        }
    }
	catch(Exception &err)
	{
        DBTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,err.Message);
	}
    return retValue;
}
//----------------------------------------------------------------------------
void TfrmSelectDish::DisplayRoomNoUI()
{
    if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot &&
            TGlobalSettings::Instance().EnableCustomerJourney && Screen->ActiveForm->ClassNameIs("TfrmSelectDish"))
    {
        std::auto_ptr<TfrmTouchNumpad>frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
        frmTouchNumpad->Caption = "Enter the Room Number";
        frmTouchNumpad->btnSurcharge->Caption = "Ok";
        frmTouchNumpad->btnDiscount->Caption = "Walk In";
        frmTouchNumpad->btnDiscount->Visible = true;
        frmTouchNumpad->btnSurcharge->Visible = true;
        frmTouchNumpad->btnDiscount->Color = clGreen;
        frmTouchNumpad->Mode = pmSTR;
        frmTouchNumpad->NUMSTRInitial = "";
        isRoomNoUiCalled = true;
		SiHotAccount.AccountDetails.clear();
        if (frmTouchNumpad->ShowModal() == mrOk && frmTouchNumpad->BtnExit == 1 && frmTouchNumpad->NUMSTRResult != "")
        {
            selectedRoomNumberStr = frmTouchNumpad->NUMSTRResult;
            isWalkInUser = false;
            GetRoomDetails();
            RedrawSeatOrders();
            SeatOrders[SelectedSeat]->wasGuestSelected = true;
        }
        else if(atoi(frmTouchNumpad->NUMSTRResult.t_str()) == 0 && frmTouchNumpad->BtnExit == 2)
        {
            selectedRoomNumberStr = TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount;
            isWalkInUser = true;
            //For displaying room details like member.
            Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
            TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
            dbTransaction.StartTransaction();
            LoadDefaultGuestDetailsToSeatOrders(TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount,
                                    TManagerVariable::Instance().GetStr(dbTransaction,vmSiHotDefaultTransactionName),
                                    TDeviceRealTerminal::Instance().BasePMS->DefaultAccountNumber);
            dbTransaction.Commit();
            RedrawSeatOrders();
            SeatOrders[SelectedSeat]->wasGuestSelected = true;
        }
        else if(frmTouchNumpad->BtnExit == 2 && abs(atoi(frmTouchNumpad->NUMSTRResult.t_str())) > 0)
        {
            isRoomNoUiCalled = false;
            MessageBox("Walkin cannot be selected with room number.", "Error", MB_OK + MB_ICONERROR);
            DisplayRoomNoUI();
        }
        else if(frmTouchNumpad->BtnExit == 1 && frmTouchNumpad->NUMSTRResult == "")
        {
            MessageBox("No Guest was selected","Error",MB_OK);
            DisplayRoomNoUI();
        }
    }
    isRoomNoUiCalled = false;
}
//------------------------------------------------------------------------------
void TfrmSelectDish::GetRoomDetails()
{
    try
    {
        isRoomNoUiCalled = false;
        SiHotAccount = TSiHotAccounts();
        std::vector<TSiHotAccounts> SiHotAccounts;
        TSiHotAccounts guestAccount;
        guestAccount.AccountNumber =  selectedRoomNumberStr;
        SiHotAccounts.push_back(guestAccount);
        TDeviceRealTerminal::Instance().BasePMS->GetRoomStatus(SiHotAccounts,TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress,TDeviceRealTerminal::Instance().BasePMS->TCPPort);

        if(SiHotAccounts.size())
        {
            bool isOkPressed = false;
            UnicodeString selectedAccountNumber = "";
            if(SiHotAccounts.size() > 1)
            {
                std::auto_ptr<TfrmGuestList> frmGuestList(TfrmGuestList::Create<TfrmGuestList>(this));
                frmGuestList->GuestAccounts = SiHotAccounts;
                frmGuestList->Caption = "Select Any Guest";
                if(frmGuestList->ShowModal() == mrOk)
                {
                    isOkPressed = true;
                    selectedAccountNumber = frmGuestList->SelectedAccountNumber.c_str();
                }
                else
                {
                    DisplayRoomNoUI();
                }
            }
            for(std::vector<TSiHotAccounts>::iterator it = SiHotAccounts.begin(); it != SiHotAccounts.end() ; ++it)
            {
                if((isOkPressed && it->AccountNumber == selectedAccountNumber) || (SiHotAccounts.size() == 1))
                {
                     SiHotAccount.AccountNumber = it->AccountNumber;
                    for(std::vector<TAccountDetails>::iterator accIt = it->AccountDetails.begin(); accIt != it->AccountDetails.end(); ++accIt)
                    {
                        TAccountDetails accountDetails;
                        accountDetails.RoomNumber = accIt->RoomBedNumber;//selectedRoomNumberStr;
                        accountDetails.LastName = accIt->LastName;
                        accountDetails.FirstName = accIt->FirstName;
                        accountDetails.CreditLimit = accIt->CreditLimit;
                        accountDetails.RoomBedNumber = accIt->RoomBedNumber;
                        selectedRoomNumberStr = accountDetails.RoomBedNumber;
                        SiHotAccount.AccountDetails.push_back(accountDetails);

                        //For displaying room details like member.
                        LoadDefaultGuestDetailsToSeatOrders(accIt->RoomBedNumber, accIt->FirstName, it->AccountNumber, accIt->LastName);
                    }
                }
            }
        }
        else if(SiHotAccounts.size() == 0)
        {
            DisplayRoomNoUI();
        }
    }
    catch(Exception &err)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,err.Message);
	}
}
//-----------------------------------------------------------------------------------------------
bool TfrmSelectDish::LoadRoomDetailsToPaymentTransaction(TPaymentTransaction &inTransaction)
{
    bool isRoomDetailsLoaded = false;
    try
    {
        if(SiHotAccount.AccountDetails.size() || isWalkInUser)
        {
            for(std::vector<TAccountDetails>::iterator accIt = SiHotAccount.AccountDetails.begin(); accIt != SiHotAccount.AccountDetails.end(); ++accIt)
            {
                inTransaction.Phoenix.AccountNumber = SiHotAccount.AccountNumber;
                inTransaction.Phoenix.AccountName = accIt->FirstName + " " + accIt->LastName;
                inTransaction.Phoenix.RoomNumber = selectedRoomNumberStr;
                inTransaction.Phoenix.FirstName = accIt->FirstName;
                inTransaction.Phoenix.LastName = accIt->LastName;
                inTransaction.SalesType = eRoomSale;
                inTransaction.Customer.RoomNumberStr = inTransaction.Phoenix.RoomNumber;
                isRoomDetailsLoaded = true;
            }

            if(isWalkInUser)
              isRoomDetailsLoaded = true;

            for (int i = 0; i < inTransaction.Orders->Count; i++)
            {
                TItemComplete *Order = (TItemComplete*)inTransaction.Orders->Items[i];
                if(Order->TabType != TabNone && Order->TabType != TabCashAccount)
                    break;
                if(Order->TabType != TabCashAccount)
                {
                    Order->TabType          = TabRoom;
                    Order->TabContainerName = inTransaction.Phoenix.RoomNumber;
                    Order->TabName = inTransaction.Phoenix.RoomNumber;
                }
                Order->RoomNoStr = inTransaction.Phoenix.RoomNumber;
                Order->FirstName = inTransaction.Phoenix.FirstName;
                Order->LastName = inTransaction.Phoenix.LastName;
                Order->AccNo = inTransaction.Phoenix.AccountNumber;
                Order->RoomNo = inTransaction.Phoenix.RoomNumber != "" ? StrToInt(inTransaction.Phoenix.RoomNumber) : 0;

                for (int i = 0; i < Order->SubOrders->Count; i++)
                {
                    TItemCompleteSub *CurrentSubOrder = (TItemCompleteSub*)Order->SubOrders->Items[i];
                    if(Order->TabType != TabNone && Order->TabType != TabCashAccount)
                        break;
                    CurrentSubOrder->RoomNoStr = inTransaction.Phoenix.RoomNumber;
                    CurrentSubOrder->FirstName = inTransaction.Phoenix.FirstName;
                    CurrentSubOrder->LastName = inTransaction.Phoenix.LastName;
                    CurrentSubOrder->AccNo = inTransaction.Phoenix.AccountNumber;
                    CurrentSubOrder->RoomNo = inTransaction.Phoenix.RoomNumber != "" ? StrToInt(inTransaction.Phoenix.RoomNumber) : 0;
                }
            }
        }
    }
    catch(Exception &err)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,err.Message);
	}
    return isRoomDetailsLoaded;
}
//-------------------------------------------------------------------------------------------------
void __fastcall TfrmSelectDish::tiPMSRoomInputTimer(TObject *Sender)
{    
    tiPMSRoom->Enabled = false;
    if(!(lbDisplay->ItemIndex == -1 && lbDisplay->Count >0) )
    { 
        DisplayRoomNoUI();
    }
    tiChitDelay->Enabled = TGlobalSettings::Instance().NagUserToSelectChit;
}
//-----------------------------------------------------------------------------------------------------
std::vector<UnicodeString> TfrmSelectDish::LoadGuestDetails(UnicodeString defaultTransaction)
{
    std::vector<UnicodeString> guestDetails;
    bool isGuestDetailsLoaded = false;
    if(lbDisplay->ItemIndex == -1 && lbDisplay->Count >0 )
    {
        TItemRedirector *ItemRedirector = (TItemRedirector*)lbDisplay->Items->Objects[0];
        if(ItemRedirector->ItemType.Contains(itNormalItem))
        {
            isWalkInUser = false;
            SiHotAccount.AccountDetails.clear();
            TItemComplete *CompressedOrder = (TItemComplete*)ItemRedirector->CompressedContainer->ItemsList[0];
            guestDetails.push_back(CompressedOrder->AccNo);
            guestDetails.push_back(CompressedOrder->RoomNoStr);
            guestDetails.push_back(CompressedOrder->FirstName);
            guestDetails.push_back(CompressedOrder->LastName);
            isGuestDetailsLoaded = true;
        }
    }

    for(int index = 0; index < SeatOrders[SelectedSeat]->Orders->CompressedCount; index++)
    {      
        TItemRedirector *ItemRedirector = (TItemRedirector*)lbDisplay->Items->Objects[index];

        if (ItemRedirector->ItemType.Contains(itNormalItem) || ItemRedirector->ItemType.Contains(itPrevItem))
        {
            TItemMinorComplete * CompressedOrder = (TItemMinorComplete*)ItemRedirector->ItemObject;
            if (CompressedOrder)
            {
                if(!CompressedOrder->AccNo.Compare(TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount))
                    isWalkInUser = false;
               // SiHotAccount.AccountDetails.clear();
                guestDetails.push_back(CompressedOrder->AccNo);
                guestDetails.push_back(CompressedOrder->RoomNoStr);
                guestDetails.push_back(CompressedOrder->FirstName);
                guestDetails.push_back(CompressedOrder->LastName);
                isGuestDetailsLoaded = true;
            }
        }
    }

    if(!SeatOrders[SelectedSeat]->Orders->Count && (SiHotAccount.AccountDetails.size() || isWalkInUser) && !isGuestDetailsLoaded)
    {
        if(isWalkInUser)
        {
            guestDetails.push_back(TDeviceRealTerminal::Instance().BasePMS->DefaultAccountNumber);
            guestDetails.push_back(TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount);
            guestDetails.push_back(defaultTransaction);
        }
        else
        {
            for(std::vector<TAccountDetails>::iterator accIt = SiHotAccount.AccountDetails.begin(); accIt != SiHotAccount.AccountDetails.end(); ++accIt)
            {
                guestDetails.push_back(SiHotAccount.AccountNumber);
                guestDetails.push_back(accIt->RoomNumber);
                guestDetails.push_back(accIt->FirstName);
                guestDetails.push_back(accIt->LastName);
            }
        }
    }
    else if(SeatOrders[SelectedSeat]->Orders->Count && !isGuestDetailsLoaded)
    {     
        TItemComplete *Order = SeatOrders[SelectedSeat]->Orders->Items[0];
        guestDetails.push_back(Order->AccNo);
        guestDetails.push_back(Order->RoomNoStr);
        guestDetails.push_back(Order->FirstName);
        guestDetails.push_back(Order->LastName);
    }
    return guestDetails;
}
//-------------------------------------------------------------------------------------------------------------------------
bool TfrmSelectDish::CloseActiveForm()
{
    if(Screen->ActiveForm->ClassNameIs("TfrmTouchNumpad"))
    {
        isRoomNoUiCalled = false;
        IsAutoLogOutInSelectDish = false;
        Screen->ActiveForm->Close();
    }
}
//----------------------------------------------------------------------------
void TfrmSelectDish::StorePatronsInformation(TPaymentTransaction &PaymentTransaction)
{
    for(int indexPatron = 0; indexPatron < PaymentTransaction.Patrons.size(); indexPatron++)
    {
        if(PaymentTransaction.Patrons[indexPatron].Default)
        {
            storedPatronCountFromMenu = PaymentTransaction.Patrons[indexPatron].Count;
            PaymentTransaction.PatronCountFromMenu = PaymentTransaction.Patrons[indexPatron].Count;
            break;
        }
    }
//    patronsStore = PaymentTransaction.Patrons;
}
//----------------------------------------------------------------------------
void TfrmSelectDish::InitializePatronForQuickSale(TPaymentTransaction &PaymentTransaction)
{
    int totalPatronCount = 0;
    for(int indexPatrons = 0; indexPatrons < patronsStore.size(); indexPatrons++)
    {
        if(patronsStore[indexPatrons].Default)
        {
            int backStore = patronsStore[indexPatrons].Count - storedPatronCountFromMenu;
            PaymentTransaction.Patrons[indexPatrons].Count += patronsStore[indexPatrons].Count;
            //backStore;
        }
        else
            PaymentTransaction.Patrons[indexPatrons].Count += patronsStore[indexPatrons].Count;
        totalPatronCount += patronsStore[indexPatrons].Count;
    }
    if(totalPatronCount == 0)
          TManagerPatron::Instance().SetDefaultPatrons(PaymentTransaction.DBTransaction, PaymentTransaction.Patrons, 1);

    PaymentTransaction.Patrons = QueryForPatronCount(PaymentTransaction);
    patronsStore = PaymentTransaction.Patrons;
}
//----------------------------------------------------------------------------
void TfrmSelectDish::ExtractPatronInformation(TPaymentTransaction &PaymentTransaction)
{
    patronsStore = PaymentTransaction.Patrons;
    storedPatronCountFromMenu = PaymentTransaction.PatronCountFromMenu;
    for(int indexPatrons = 0; indexPatrons < patronsStore.size(); indexPatrons++)
    {
        if(patronsStore[indexPatrons].Default)
        {
           patronsStore[indexPatrons].Count -= storedPatronCountFromMenu;
           break;
        }
    }
}
//----------------------------------------------------------------------------
void TfrmSelectDish::ApplyDiscountWithRestructure(TList *Orders, TDiscount discount)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TPaymentTransaction paymentTransaction(DBTransaction);
        MakeDummyPaymentTransaction(Orders,paymentTransaction);

        std::vector<TPatronType> patrons;
        patrons.clear();
        if(SelectedTable != 0)
            patrons = TDBTables::GetPatronCount(paymentTransaction.DBTransaction, SelectedTable);
        paymentTransaction.Patrons = patrons;

        std::auto_ptr<TSCDPatronUtility> patronUtility(new TSCDPatronUtility());
        if(patronUtility->CheckPatronsAvailable(paymentTransaction))
        {
            std::auto_ptr <TList> SCDOrders(new TList);
            SCDOrders->Clear();
            std::auto_ptr <TList> NormalOrders(new TList);
            NormalOrders->Clear();

            patronUtility->DivideBill(paymentTransaction,SCDOrders,NormalOrders);
            if(discount.IsSeniorCitizensDiscount())
               ManagerDiscount->AddDiscount(SCDOrders.get(), discount);
            else
               ManagerDiscount->AddDiscount(NormalOrders.get(), discount);

            paymentTransaction.Orders->Clear();
            for(int indexSCD = 0; indexSCD < SCDOrders->Count; indexSCD++)
            {
                TItemComplete *Order = (TItemComplete *)SCDOrders->Items[indexSCD];
                paymentTransaction.Orders->Add(Order);
            }
            for(int indexNormal = 0; indexNormal < NormalOrders->Count; indexNormal++)
            {
                TItemComplete *Order = (TItemComplete *)NormalOrders->Items[indexNormal];
                paymentTransaction.Orders->Add(Order);
            }
            paymentTransaction.Money.Recalc(paymentTransaction);
        }
        else
        {
            ManagerDiscount->AddDiscount(paymentTransaction.Orders, discount);
        }
        ExtractFromDummyPaymentTransaction(paymentTransaction, Orders);
        DBTransaction.Commit();
    }
    catch(Exception &Ex)
    {
        DBTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
}
//----------------------------------------------------------------------------
void TfrmSelectDish::MakeDummyPaymentTransaction(TList* Orders,TPaymentTransaction &paymentTransaction)
{
    for (int index = 0; index < Orders->Count; index++)
    {
        TItemComplete *ic = (TItemComplete*)Orders->Items[index];
        paymentTransaction.Orders->Add(ic);
    }
    paymentTransaction.Membership.Assign(Membership);
    paymentTransaction.Money.Recalc(paymentTransaction);
}
//----------------------------------------------------------------------------
void TfrmSelectDish::ExtractFromDummyPaymentTransaction(TPaymentTransaction &paymentTransaction,TList *Orders)
{
   Orders->Clear();
   for(int index = 0; index < paymentTransaction.Orders->Count; index++)
   {
       TItemComplete *itemComplete = (TItemComplete*)paymentTransaction.Orders->Items[index];
       if(itemComplete->GetQty() != 0)
           Orders->Add(itemComplete);
   }
}
//----------------------------------------------------------------------------
bool TfrmSelectDish::ArePatronsChanged(std::vector<TPatronType> patronsOld,std::vector<TPatronType> patronsNew)
{
    bool retValue = false;
    std::auto_ptr<TSCDPatronUtility> scdpatronUtility(new TSCDPatronUtility());
    retValue = scdpatronUtility->ArePatronsChanged(patronsOld,patronsNew);
    return retValue;
}
//----------------------------------------------------------------------------
void TfrmSelectDish::RestructureBillForPatrons(std::vector<TPatronType> patrons)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TPaymentTransaction paymentTransaction(DBTransaction);
        paymentTransaction.Patrons = patrons;
        MakeDummyPaymentTransaction(SeatOrders[SelectedSeat]->Orders->List,paymentTransaction);
        std::auto_ptr<TSCDPatronUtility> scdpatronUtility(new TSCDPatronUtility());
        scdpatronUtility->RestructureBill(paymentTransaction);
        ExtractFromDummyPaymentTransaction(paymentTransaction, SeatOrders[SelectedSeat]->Orders->List);
        paymentTransaction.Money.Recalc(paymentTransaction);
        RedrawSeatOrders();
        DBTransaction.Commit();
    }
    catch(Exception &Ex)
    {
        DBTransaction.Rollback();
        MessageBox(Ex.Message,"",MB_OK);
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
}
//----------------------------------------------------------------------------
bool TfrmSelectDish::LoadPMSGuestDetails(TPaymentTransaction &PaymentTransaction)
{
    bool isGuestExist = true;
    if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot && TGlobalSettings::Instance().EnableCustomerJourney )
    {
        isRoomNoUiCalled = false;
        IsAutoLogOutInSelectDish = false;
        if(isWalkInUser)
        {    
            PaymentTransaction.Phoenix.AccountNumber = TDeviceRealTerminal::Instance().BasePMS->DefaultAccountNumber;;
            PaymentTransaction.Phoenix.AccountName = TManagerVariable::Instance().GetStr(PaymentTransaction.DBTransaction,vmSiHotDefaultTransactionName);
            PaymentTransaction.Phoenix.RoomNumber = TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount;
            PaymentTransaction.Phoenix.FirstName = TManagerVariable::Instance().GetStr(PaymentTransaction.DBTransaction,vmSiHotDefaultTransactionName);
            PaymentTransaction.SalesType = eRoomSale;
            PaymentTransaction.Customer.RoomNumberStr = PaymentTransaction.Phoenix.RoomNumber;
        }
        isGuestExist = LoadRoomDetailsToPaymentTransaction(PaymentTransaction);
    }
    return isGuestExist;
}
//-------------------------------------------------------------------------------------
void TfrmSelectDish::LoadDefaultGuestDetailsToSeatOrders(UnicodeString roomNo, UnicodeString firstName, UnicodeString accNo, UnicodeString lastName)
{
    SeatOrders[SelectedSeat]->Orders->pmsAccountDetails.RoomNumber = roomNo;
    SeatOrders[SelectedSeat]->Orders->pmsAccountDetails.FirstName = firstName;
    SeatOrders[SelectedSeat]->Orders->pmsAccountDetails.LastName = lastName;
    SeatOrders[SelectedSeat]->Orders->pmsAccountDetails.RoomBedNumber = roomNo;
    SeatOrders[SelectedSeat]->RoomNumber = roomNo;     //tocheck if can comment
    if(SeatOrders[SelectedSeat]->Orders->Count > 0)
    {
        for(int index = 0; index < SeatOrders[SelectedSeat]->Orders->Count; index++)
        {
            SeatOrders[SelectedSeat]->Orders->Items[index]->RoomNoStr = roomNo;
            SeatOrders[SelectedSeat]->Orders->Items[index]->FirstName = firstName;
            SeatOrders[SelectedSeat]->Orders->Items[index]->LastName = lastName;
            SeatOrders[SelectedSeat]->Orders->Items[index]->AccNo = accNo;
        }
    }
}
//-------------------------------------------------------------------------------------
void TfrmSelectDish::GetNextAvailableSeatAndLoadOrders(bool isCalledFromGuestSeat)
{
  if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot &&
        TGlobalSettings::Instance().EnableCustomerJourney && !isCalledFromGuestSeat  && SelectedSeat != 0 && !SeatOrders[0]->isChangeTablePressed)
    {
        unsigned __int32 maxSeatCount = getMaxSeatCount();

        if(SeatOrders[0]->Orders->Count)
        {
            TItemComplete *firstItem =  SeatOrders[0]->Orders->Items[0];

            for( unsigned __int32 i = 1; i <= maxSeatCount; i++)
            {
                SeatOrders[i]->Orders->RefreshDisplay();
                bool isSeatSelected = false;

                if(SeatOrders[i]->Orders->CompressedCount)
                {
                    for(int index = 0; index < SeatOrders[i]->Orders->CompressedCount; index++)
                    {
                        TItemsCompleteCompressed* CompressedItem = SeatOrders[i]->Orders->CompressedItems[index]; //selected seatchanged to i to do if bug come

                        TItemRedirector *Redirector = (TItemRedirector *)CompressedItem->Display->Objects[0];

                        if ((Redirector->ItemType.Contains(itNormalItem) ||Redirector->ItemType.Contains(itPrevItem)) )
                        {
                            TItemMinorComplete * CompressedOrder = (TItemMinorComplete*)Redirector->ItemObject;
                            if (CompressedOrder)
                            {
                                if(!firstItem->RoomNoStr.Compare(CompressedOrder->RoomNoStr))
                                {
                                    isSeatSelected = true;
                                }
                                break;
                            }
                        }
                        else if(Redirector->ItemType.Contains(itMembershipDisplay) && SeatOrders[i]->Orders->CompressedCount == 1)
                        {
                            isSeatSelected = true;
                            break;
                        }
                    }
                }
                if(SeatOrders[i]->Orders->CompressedCount == 0 || isSeatSelected)
                {
                    while (SeatOrders[0]->Orders->Count != 0)
                    {
                        SeatOrders[i]->Orders->Add(SeatOrders[0]->Orders->Items[0], SeatOrders[0]->Orders->Items[0]->ItemOrderedFrom);
                        SeatOrders[0]->Orders->Delete(SeatOrders[0]->Orders->Items[0]);
                    }
                    SeatOrders[0]->Orders->Clear();
                    SelectedSeat = i;
                    SeatOrders[SelectedSeat]->RoomNumber = firstItem->RoomNoStr;
                    LoadDefaultGuestDetailsToSeatOrders(firstItem->RoomNoStr, firstItem->FirstName, firstItem->AccNo ,firstItem->LastName);
                    break;
                }

            }
        }
        else
        {
            for( unsigned __int32 i = 1; i <= maxSeatCount; i++)
            {
                SeatOrders[i]->Orders->RefreshDisplay();
                bool isSeatSelected = false;
                if(SeatOrders[i]->Orders->CompressedCount)
                {
                    for(int index = 0; index < SeatOrders[i]->Orders->CompressedCount; index++)
                    {
                        TItemsCompleteCompressed* CompressedItem = SeatOrders[i]->Orders->CompressedItems[index];

                        TItemRedirector *Redirector = (TItemRedirector *)CompressedItem->Display->Objects[0];

                        if ((Redirector->ItemType.Contains(itPrevItem)) )
                        {
                            TItemMinorComplete * CompressedOrder = (TItemMinorComplete*)Redirector->ItemObject;
                            if (CompressedOrder)
                            {
                                if(!selectedRoomNumberStr.Trim().Compare(CompressedOrder->RoomNoStr.Trim()))
                                {
                                    isSeatSelected = true;
                                }
                                break;
                            }
                        }
                        else if(Redirector->ItemType.Contains(itMembershipDisplay) && SeatOrders[i]->Orders->CompressedCount == 1)
                        {
                            SeatOrders[i]->RoomNumber = "";
                            lbDisplay->Clear();
                            isSeatSelected = true;
                            break;
                        }
                    }
                }
                else
                {
                    isSeatSelected = true;
                }
                if(isSeatSelected)
                {
                    while (SeatOrders[0]->Orders->Count != 0)
                    {
                        SeatOrders[i]->Orders->Add(SeatOrders[0]->Orders->Items[0], SeatOrders[0]->Orders->Items[0]->ItemOrderedFrom);
                        SeatOrders[0]->Orders->Delete(SeatOrders[0]->Orders->Items[0]);
                    }
                    SeatOrders[0]->Orders->Clear();
                    SelectedSeat = i;
                    SeatOrders[SelectedSeat]->RoomNumber = selectedRoomNumberStr;
                    break;
                }

            }
        }
    }
}
//--------------------------------------------------------------------------------

