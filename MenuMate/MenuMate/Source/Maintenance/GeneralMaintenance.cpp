

#include <vcl.h>
#pragma hdrstop

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef  PalmMate
#include "Palm.h"
#endif

#include "GeneralMaintenance.h"
#include "MMMessageBox.h"
#include "MMTouchKeyboard.h"
#include "MMTouchNumpad.h"
#include "DBSaleTimes.h"
#include "TableManager.h"
#include "DBTab.h"
#include "GUIDiscount.h"
#include "ManagerVariable.h"
#include "ListPaymentSystem.h"
#include "FreebieManager.h"
#include "Membership.h"
#include "MMLogging.h"
#include "SelectDish.h"
#include "Processing.h"
#include "ReportDisplay.h"
//#include "StringTableRes.h"
#include "StringTableVariables.h"
#include "ManagerTimeClock.h"
#include "MMLogging.h"
#include "Printing.h"
#include <StrUtils.hpp>
#include <process.h>
#include <memory>
#include "SetButtonFontSize.h"
#include "Registration.h"
#include "PrintOut.h"
#include "Comms.h"
#include "EFTPOSConfiguration.h"
#define semi_colon 0x3B

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "touchbtn"
#pragma link "touchcontrols"
#pragma link "TouchGrid"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmGeneralMaintenance::TfrmGeneralMaintenance(TComponent* Owner,Database::TDBControl &inDBControl)
: TZForm(Owner), DBControl(inDBControl)
{
}

#pragma warn -par

void __fastcall TfrmGeneralMaintenance::quick_member_setup_clicked(TObject *sender)
{
	//std::auto_ptr<TfrmQuickMemberSetupOptions> settings(
    //new TfrmQuickMemberSetupOptions(this));
	//settings->ShowModal();
}

#pragma warn .par

//---------------------------------------------------------------------------

void __fastcall
TfrmGeneralMaintenance::cbNagUserToSelectChitClicked(
TObject *)
{
	TManagerVariable &ref_mv = TManagerVariable::Instance();
	Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);

	tr.StartTransaction();
	ref_mv.SetDeviceBool(
	tr, vmNagUserToSelectChit, cbNagUserToSelectChit->Checked);
	tr.Commit();

	TGlobalSettings::Instance().NagUserToSelectChit =
	cbNagUserToSelectChit->Checked;
}

void __fastcall TfrmGeneralMaintenance::cbUseMenuDefaultPatronCountClicked(TObject *Sender)
{
	TGlobalSettings::Instance().EnableMenuPatronCount = cbUseMenuDefaultPatronCount->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmMenuPatronCount,TGlobalSettings::Instance().EnableMenuPatronCount);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbEnableExtendedDisplayClicked(TObject *Sender)
{
	TGlobalSettings::Instance().ExtendCurrencyDisplay = cbEnableExtendedDisplay->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmExtendCurrencyDisplay,TGlobalSettings::Instance().ExtendCurrencyDisplay);
	DBTransaction.Commit();
}


void __fastcall TfrmGeneralMaintenance::FormShow(TObject *Sender)
{
	FormResize(this);
	Database::TDBTransaction DBTransaction(DBControl);
	TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	DBTransaction.StartTransaction();
	edClearLogsXDays->Text		= IntToStr(TGlobalSettings::Instance().ClearLogsEveryXDays);
	udClearLogsXDays->Position = TGlobalSettings::Instance().ClearLogsEveryXDays;
	cbRevertCourse->Checked		= TGlobalSettings::Instance().RevertToFirstCourse;
	cbForceTender->Checked     = TDeviceRealTerminal::Instance().PaymentSystem->ForceTender;
	chbUseAutoLogout->Checked	= TGlobalSettings::Instance().AutoLogoutPOS;
	btnAutoLogoutTime->Caption	= TGlobalSettings::Instance().AutoLogoutSeconds;
	btnNonService->Caption		= TGlobalSettings::Instance().NonServicePeriodMinutes;
	tbtnMMSubNet->Caption      = IntToStr(TDeviceRealTerminal::Instance().ManagerNet->MMSubNet);
	cbForcedReasons->Checked 	= TGlobalSettings::Instance().ForcedReasons;
	edSeatLabel->Text = TGlobalSettings::Instance().SeatLabel;
	cbSaleAndMakeTimes->Checked = TManagerVariable::Instance().GetBool(DBTransaction,vmTrackSaleAndMakeTimes,false);
	cbFTEFTSales->Checked = TDeviceRealTerminal::Instance().PaymentSystem->FTBypassElecTranTyp;
	cbTableSeats->OnClick = NULL;
	cbTableSeats->Checked = TGlobalSettings::Instance().TablesEnabled;
	cbTableSeats->OnClick = cbTableSeatsClick;
	cbTabsInterface->OnClick = NULL;
	cbTabsInterface->Checked = TGlobalSettings::Instance().TabsEnabled;
	cbTabsInterface->OnClick = cbTabsInterfaceClick;
	cbForcedSidesFirst->Checked = TGlobalSettings::Instance().ForcedSidesFirst;
	btnNonService->Caption = TGlobalSettings::Instance().NonServicePeriodMinutes;
	cbDuplicateEftPos->Checked = TGlobalSettings::Instance().DuplicateEftPosReceipt;
	cbIncrementalSearch->Checked = TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->EnableIncrementalSearch;
	tbMinMembershipNumber->Caption = IntToStr(TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->MinMemberNumber);
	cbRecycleMembershipNumbers->Checked = TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->RecycleMemberNumber;
	//rgMemberPoleDisplay->ItemIndex = (int)TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->NameOnPoleDisplay;
	cbvmAllowSmartCardNotPresentMemberTransactions->Checked = TDeviceRealTerminal::Instance().ManagerMembership->AllowSmartCardNotPresentMemberTransactions;
	rgMemberFreeSalesRules->ItemIndex = (int)ManagerFreebie->MemberLoyaltyRewardsRules;
	tbMemberQuickButtons->Caption = IntToStr(TManagerVariable::Instance().GetInt(DBTransaction,vmMemberQuickButtonCount,1));
	tbtnLuckMemberPeriod->Caption = IntToStr(TGlobalSettings::Instance().LuckyMemberPeriod);
	cbOnlyEarnPoints->Checked = TManagerVariable::Instance().GetBool(DBTransaction,vmOnlyEarnsPointsWhileRedeeming);
	cbEarnPoints->Checked = TManagerVariable::Instance().GetBool(DBTransaction,vmEarnsPointsWhileRedeeming);
	cbPointsOnZed->Checked = TGlobalSettings::Instance().ShowPointsOnZed;
	cbDEnableBlindBalances->Checked = TGlobalSettings::Instance().EnableBlindBalances;
	cbEnableStaffHours->Checked = TGlobalSettings::Instance().EnableStaffHours;
	cbEnableCommission->Checked = TGlobalSettings::Instance().EnableCommission;
	cbEnablePrinterCounts->Checked = TGlobalSettings::Instance().EnablePrinterCounts;
	cbEnableHideCredCancels->Checked = TGlobalSettings::Instance().EnableHideCredsCancels;
	cbDontClearDataOnZed->Checked = TGlobalSettings::Instance().EnableDontClearZedData;
	cbSeparatePts->Checked = TGlobalSettings::Instance().EnableSeperateEarntPts;
	cbOnlyFullProducts->Checked = TGlobalSettings::Instance().SpendPtsOnlyFullProduct;
	cbRedeemOnSelectedItems->Checked = TGlobalSettings::Instance().SpendPtsOnlySelectItems;
	cbEarnPointsOnWeight->Checked = TGlobalSettings::Instance().EarntPointsOnWeight;
	cbEnableTableMode->Checked = TGlobalSettings::Instance().EnableTableDisplayMode;
	if(!TRegInfo::IsMagicMemories(DBTransaction))
	{
		cbEnablePaxCount->Visible = false;
		cbEnablePrinterCounts->Visible = false;
	}
	cbEnablePaxCount->Checked = TGlobalSettings::Instance().EnablePaxCount;
	cbPoleDisplayShowPoints->Checked = TGlobalSettings::Instance().ShowPointsOnDisplay;
	cbAARewardsReminder->Checked = TGlobalSettings::Instance().EnableAARewardsReminder;
	cbEnableAARewards->OnClick = NULL;
	cbEnableAARewards->Checked = TGlobalSettings::Instance().EnableAARewards;
	cbEnableAARewards->OnClick = cbEnableAARewardsClick;
	cbDisableInternalInvoicing->Checked = TGlobalSettings::Instance().DisableInternalInvoicing;
	cbIntegratedEftposANZ->Checked = TGlobalSettings::Instance().EnableEftPosANZ;
	cbIntegratedEftposSyncro->Checked = TGlobalSettings::Instance().EnableEftPosSyncro;
	cbIntegratedEftposIngenico->Checked = TGlobalSettings::Instance().EnableEftPosIngenico;
	cbIntegratedEftposCadmus->Checked = TGlobalSettings::Instance().EnableEftPosCadmus;
	cbCadmusCronos->Checked = TGlobalSettings::Instance().EnableEftPosCadmusCronos;
	cbIntegratedEftposDPS->Checked = TGlobalSettings::Instance().EnableEftPosDPS;
	cbEnableDPSTipping->Checked = TGlobalSettings::Instance().EnableDPSTipping;
	cbEnableDPSTipping->Enabled = TGlobalSettings::Instance().EnableEftPosDPS;
	cbICELink->Checked = TGlobalSettings::Instance().EnableEftPosIceLink;
	tbCadmusMerchantNumber->Caption = IntToStr(TManagerVariable::Instance().GetInt(DBTransaction,vmEftposMerchantNumber,1));
	cbShowInvoiceInfo->Checked = TGlobalSettings::Instance().ShowInvoiceInfoOnZed;
	cbCaptureCustomerName->Checked = TGlobalSettings::Instance().CaptureCustomerName;
	cbHoldSend->Checked = TGlobalSettings::Instance().EnableHoldSend;
	cbHideFreeItemsAtBilling->Checked = TGlobalSettings::Instance().HideFreeItemsAtBilling;
	cbEnableWaiterStation->Checked = TGlobalSettings::Instance().EnableWaiterStation;
	cbDisplayTax->Checked = TGlobalSettings::Instance().EnableDisplayTax;
	cbDisplayServiceCharge->Checked = TGlobalSettings::Instance().EnableDisplayServiceCharge;
	cbEnableNmiDisplay->Checked = TGlobalSettings::Instance().EnableNmiDisplay;
	cbDisplayTaxExempt->Checked = TGlobalSettings::Instance().EnableDisplayTaxRemoval;
	cbInclServiceChargeTaxInTaxVal->Checked = TGlobalSettings::Instance().IncludeServiceChargeTaxInTaxValue;
	cbStaffDisableManualLogin->Checked	= TManagerVariable::Instance().GetBool(DBTransaction,vmDisableManualLogin,false);
        rgLocationFreeSalesRules->ItemIndex = (int)ManagerFreebie->LocationLoyaltyRewardsRules;
        tbtnPointsGroup->Caption = IntToStr(TGlobalSettings::Instance().PointsPaymentGroupNumber);
	tbtnTabCreditGroup->Caption = IntToStr(TGlobalSettings::Instance().CreditPaymentGroupNumber);
	cbHideServingCourseLabels->Checked = TGlobalSettings::Instance().HideServingCourseLabels;
     cbPromptPatronCount->OnClick = NULL;
    cbPromptPatronCount->Checked = TGlobalSettings::Instance().PromptForPatronCount;
    cbPromptPatronCount->OnClick = cbPromptPatronCountClick;
	cbRememberLastServingCourse->Checked = TGlobalSettings::Instance().RememberLastServingCourse;
	cbDefaultToBevMenu->Checked = TGlobalSettings::Instance().DefaultToBevMenu;
	cbSummariseDiscountsonZed->Checked = TGlobalSettings::Instance().SummariseDiscountOnZed;
	cbRounding->Checked = TGlobalSettings::Instance().MidPointRoundsDown;
	cbDisableSaveConfirmation->Checked = TGlobalSettings::Instance().DisableConfirmationOnSave;
	cbDisableReceiptOnConfirmation->Checked = TGlobalSettings::Instance().DisableReceiptOnConfirmation;
	cbShowVoucherDetails->Checked = TGlobalSettings::Instance().ShowVoucherDetails;
	cbEnablePhoneOrders->Checked = TGlobalSettings::Instance().EnablePhoneOrders;
	cbEnableMembershipSmartCards->OnClick = NULL;
	cbEnableMembershipSmartCards->Checked = TManagerVariable::Instance().GetBool(DBTransaction,vmSmartCardMembership,false);
	cbEnableMembershipSmartCards->OnClick = cbEnableMembershipSmartCardsClick;
	cbSacleAutoConvertWeights->Checked = TGlobalSettings::Instance().AutoConvertScalesWeights;
	cbScaleEnterWeight->Checked = TGlobalSettings::Instance().AllowToEnterWeight;
	cbScaleEnterPrice->Checked = TGlobalSettings::Instance().AllowToEnterPrice;
	cbNotifyBirthday->Checked = TGlobalSettings::Instance().EnableBirthdayNotifications;
	cbAutoAddWebMembers->Checked = TGlobalSettings::Instance().AutoAddWebMembers;
	cbAcknowledgeFirstVisit->OnClick = NULL;
	cbAcknowledgeFirstVisit->Checked = TGlobalSettings::Instance().AcknowledgeFirstVisit;
	cbAcknowledgeFirstVisit->OnClick = cbAcknowledgeFirstVisitClick;
	cbEnableMemberSpendChits->Checked 	= TGlobalSettings::Instance().EnableMemberSpendChits;
	cbDrawByLocation->Checked 			= TGlobalSettings::Instance().LuckyDrawByLocationOnly;
	tbtnMemberSpendChitValue->Caption =  "$" + CurrToStr(TGlobalSettings::Instance().MemberSpendChitsAmount);
	cbDisplayStockPurchaseWeborders->Checked = TGlobalSettings::Instance().PrintStockWeborders;
	cbWarnIfOpenTableAndTabs->Checked = TGlobalSettings::Instance().WarnIfOpenTableAndTabs;
	cbStopIfOpenTableAndTabs->Checked = TGlobalSettings::Instance().StopIfOpenTableAndTabs;
	cbEnableTierLevels->Checked = TGlobalSettings::Instance().UseTierLevels ;
	tbBirthdayBuffer->Caption = IntToStr(TGlobalSettings::Instance().BirthdayBuffer);
	tbFirstVisitPoint->Caption = IntToStr(TGlobalSettings::Instance().FirstVisitPoint);
	//story MM1837
	cbShowTransactionSummaryGroups->Checked = TGlobalSettings::Instance().ShowTransactionSummaryGroups;
	cbShowBilledSalesTotals->Checked = TGlobalSettings::Instance().ShowBilledSalesTotals;
	cbShowComplimentarySalesTotals->Checked = TGlobalSettings::Instance().ShowComplimentarySalesTotals;
	cbShowChargedSalesTotals->Checked = TGlobalSettings::Instance().ShowChargedSalesTotals;
	cbShowTotals->Checked = TGlobalSettings::Instance().ShowTotals;
	cbShowBreakdownCategories->Checked = TGlobalSettings::Instance().ShowBreakdownCategories;
	cbShowDiscountReport->Checked = TGlobalSettings::Instance().ShowDiscountReport;
	cbShowPointsReport->Checked = TGlobalSettings::Instance().ShowPointsReport;
	cbShowPatronAverages->Checked = TGlobalSettings::Instance().ShowPatronAverages;
	cbShowProductionInfo->Checked = TGlobalSettings::Instance().ShowProductionInfo;
	cbShowAccountPurchases->Checked = TGlobalSettings::Instance().ShowAccountPurchases;
	cbShowAccountBalancesTabs->Checked = TGlobalSettings::Instance().ShowAccountBalancesTabs;
	cbShowAccountBalances->Checked = TGlobalSettings::Instance().ShowAccountBalances;
	cbCombineServiceChargeTaxAndSalesTax->Checked = TGlobalSettings::Instance().ShowServiceChargeTaxWithSalesTax;
	cbShowHourlySales->Checked = TGlobalSettings::Instance().ShowHourlySales;
	cbRevenueFiguresAreTaxAndServiceChargeInclusive->Checked = TGlobalSettings::Instance().RevenueFiguresAreTaxAndServiceChargeInclusive;
	tbMinRedemptionPoint->Caption = IntToStr(TGlobalSettings::Instance().MinRedemptionPoint);
	cbvmAllowMemberDetailscreen->Checked = TGlobalSettings::Instance().AllowMemberDetailscreen ;
    cbvmMandatoryMembershipCard->Checked= TGlobalSettings::Instance().MandatoryMembershipCard ;
    cbvmMandatoryMembershipCard->Checked= TGlobalSettings::Instance().MandatoryMembershipCard ;
    cbExcludeReceipt->Checked = TGlobalSettings::Instance().ExcludeReceipt;
    cbExcludeXReport->Checked = TGlobalSettings::Instance().ExcludeXReport;
    cbShowReprintDetails->Checked = TGlobalSettings::Instance().ShowReprintReceiptDetails;
    cbShowCashDrawerCount->Checked = TGlobalSettings::Instance().ShowCashDrawerOpeningsCount;


	int SerialPortNumber = TManagerVariable::Instance().GetInt(DBTransaction,vmEftposSerialPort);
	if(SerialPortNumber != -1)
	{
		cbEftposCadmusSerialPort->ItemIndex = SerialPortNumber;
	}
	else
	{
		cbEftposCadmusSerialPort->ItemIndex = 0;
	}

	//...........................................

	int GlobalProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction,eSystemProfiles,"Globals");

	if(GlobalProfileKey == 0)
	{
		GlobalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction,eSystemProfiles,"Globals");
	}

	UnicodeString StaffMessageOfTheDay = "";
	TManagerVariable::Instance().GetProfileStr(DBTransaction,GlobalProfileKey,vmStaffMessageOfTheDay,StaffMessageOfTheDay);
	memStaffMessage->Text = StaffMessageOfTheDay;

	int POSBtnSizeItem = 0;
	TManagerVariable::Instance().GetProfileInt( DBTransaction, GlobalProfileKey, vmPosGuiDimensionsColumns, POSBtnSizeItem );

	/* We zero the rgPOSBtnSize's OnClick callback as setting it's ItemIndex
	* triggers that event. The OnClick callback calls the
	* DetermineAndSetDefaultFontSize function. This function is meant to
	* be called /only/ when the User sets a /new/ button size.
	*/
	rgPOSBtnSize->OnClick = NULL;
	rgPOSBtnSize->ItemIndex = POSBtnSizeItem - 4;
	rgPOSBtnSize->OnClick = TfrmGeneralMaintenance::rgPOSBtnSizeClick;

	int RoundedTime = 0;
	TManagerVariable::Instance().GetProfileInt( DBTransaction, GlobalProfileKey, vmRoundingTime, RoundedTime );
    rgRoundingTimes->ItemIndex = RoundedTime;
    if(RoundedTime > 0)
    {
       rgRoundingTimes->ItemIndex = RoundedTime - 1;
    }
    else
    {
       rgRoundingTimes->ItemIndex = RoundedTime;
    }


	//	int g = 0;
	//	TManagerVariable::Instance().GetProfileIntDB(DBTransaction, TManagerVariable::Instance().DeviceProfileKey,vmDepositBagNum, g);
	cbDepositBagNum->Checked = TGlobalSettings::Instance().EnableDepositBagNum;
	//...........................................

	rgMemberNameOnRecipt->ItemIndex = Receipt->MembershipNameDisplay;

	Pages->ActivePage = tsPointOfSale;
	RedrawButtons(tbPOS);
	ReDrawLogs();

	lbeEntireSiteID->Caption = "Site ID " + IntToStr(TGlobalSettings::Instance().SiteID);


	UpdateTimeClockGrid(true);

	switch (TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->NameOnPoleDisplay)
	{
	case eMNPDNone:
		rbNoGreeting->Checked = true;
		cbPoleDisplayShowPoints->Enabled = false;
		break;
	case eMNPDAlias:
		rbShowAlias->Checked = true;
		cbPoleDisplayShowPoints->Enabled = true;
		break;
	case eMNPDFullName:
		rbShowName->Checked = true;
		cbPoleDisplayShowPoints->Enabled = true;
		break;
	}

	tbPOS->Latched = true;
        cbNagUserToSelectChit->Checked = TGlobalSettings::Instance().NagUserToSelectChit;
	cbUseMenuDefaultPatronCount->Checked = TGlobalSettings::Instance().EnableMenuPatronCount;
	cbEnableExtendedDisplay->Checked = TGlobalSettings::Instance().ExtendCurrencyDisplay;
	cbShowSessionDate->Checked = TGlobalSettings::Instance().ShowSessionDateInZed;
	cbShowAccumulatedTotal->Checked = TGlobalSettings::Instance().ShowAccumulatedZeds;
	for(short counter = 0; counter <= 23; counter++)
	{
		UnicodeString DisplayText;
		if(counter <= 9)
		{
			DisplayText = "0" + ((UnicodeString)counter)+ " hours";
		}
		else
		{
			DisplayText = ((UnicodeString)counter)+ " hours";
		}
		cbEndOfDay->Items->AddObject(DisplayText, ((TObject*)counter));
	}

	cbEndOfDay->ItemIndex = TGlobalSettings::Instance().EndOfDay;
	cbShowTaxSummary->Checked = TGlobalSettings::Instance().ShowTaxSummary;
	cbShowServiceChargeSummary->Checked = TGlobalSettings::Instance().ShowServiceChargeSummary;
	cbCombineServiceChargeAndServiceChargeTax->Checked = TGlobalSettings::Instance().ShowServiceChargeTaxWithServiceCharge;
	cbEnableClientDetails->Checked = TGlobalSettings::Instance().EnableClientDetails;
	TDeviceRealTerminal::Instance().LoadHdrFtr();
	tbzedHeaderText->Lines = TGlobalSettings::Instance().ZedHeader.get();
	tbzedHeaderText->Enabled = (TGlobalSettings::Instance().EnableClientDetails) ? true : false;
	cbEnableParkedSales->Checked = TGlobalSettings::Instance().EnableParkedSales;
        cbCombineServiceChargeAndServiceChargeTax->Enabled = TGlobalSettings::Instance().ShowServiceChargeTaxWithServiceCharge || !TGlobalSettings::Instance().ShowServiceChargeTaxWithSalesTax;
        cbCombineServiceChargeTaxAndSalesTax->Enabled = TGlobalSettings::Instance().ShowServiceChargeTaxWithSalesTax || !TGlobalSettings::Instance().ShowServiceChargeTaxWithServiceCharge;
        cbAllowPointPaymentByWeight->Checked = TGlobalSettings::Instance().AllowPointPaymentByWeight;
        cbAllowPointPaymentByValue->Checked = TGlobalSettings::Instance().AllowPointPaymentByValue ;
        cbAllowPointPaymentByWeight->Enabled = TGlobalSettings::Instance().UseTierLevels;
        cbUpdateTableGUIOnOrderStatus->Checked = TGlobalSettings::Instance().UpdateTableGUIOnOrderStatus;
        cbRefundPoints->Checked =	TGlobalSettings::Instance().AllowRefundPoints;
        cbShowCurrencySymbol->Checked = TGlobalSettings::Instance().ShowCurrencySymbol; // MM-2827
        cbSkipConfirmationOnFastTender->Checked = TGlobalSettings::Instance().SkipConfirmationOnFastTender;
        cbEmailZedClosingTill->Checked = TGlobalSettings::Instance().EmailZedReports; //MM-4104
        invalid_email_id = false; //MM-4104
        cbRoundOnBilling->Checked = TGlobalSettings::Instance().RoundOnBilling;

        tbRoundingOnBilling->Caption = FormatFloat("0.00",TGlobalSettings::Instance().RoundOnBillingAmount);

    cbTransferTableOnPrintPrelim->OnClick = NULL;
    cbTransferTableOnPrintPrelim->Checked = TGlobalSettings::Instance().TransferTableOnPrintPrelim;
    cbTransferTableOnPrintPrelim->OnClick = cbTransferTableOnPrintPrelimClick;
    cbEmailZedClosingTill->Checked = TGlobalSettings::Instance().EmailZedReports;
    invalid_email_id = false;
    cbShowRemovalReport->Checked = TGlobalSettings::Instance().ShowRemovalReport;
    cbEnableCancelCheckRemoval->Checked = TGlobalSettings::Instance().EnableCancelCheckRemoval;
    cbRevenueFiguresAreDiscountInclusive->Checked = TGlobalSettings::Instance().RevenueFiguresAreDiscountInclusive;
    cbShowPriceAdjustment->Checked = TGlobalSettings::Instance().ShowPriceAdjustment;
    cbEnableApprovedLogin->Checked = TGlobalSettings::Instance().EnableApprovedLogin;
    cbLinkClockInOutApprovedLogin->Checked = TGlobalSettings::Instance().ClockInOutApprovedLogin;
    cbLinkClockInOutApprovedLogin->Enabled = false;
    cbPointOnly->Checked = TGlobalSettings::Instance().PointOnly; //MM-4775
    if(cbEnableApprovedLogin->Checked)
	{
       cbLinkClockInOutApprovedLogin->Enabled = true;
	}

    //MM-4579
    cbResetPointBalancedPointRedeemed->Checked =   TGlobalSettings::Instance().PointRedeem;
    cbResetPointBalancedPointPurchased->Checked =   TGlobalSettings::Instance().PointPurchased;
    cbResetPointBalancedPointEarned->Checked  =    TGlobalSettings::Instance().PointEarned;
    edDay->Text =  TGlobalSettings::Instance().ResetDay;
    //MM-5085
    cbPromptPatronCountOnTableSales->OnClick = NULL;
    cbPromptPatronCountOnTableSales->Checked = TGlobalSettings::Instance().PromptPatronCountOnTableSales;
    cbPromptPatronCountOnTableSales->OnClick = cbPromptPatronCountOnTableSalesClick;
   // cbPostToXero->OnClick = NULL;
    cbPostToXero->Checked = TGlobalSettings::Instance().PostZToAccountingSystem;
  //  cbPostToXero->OnClick = cbPostToXeroClick;

    //MM 5610
    if(cbResetPointBalancedPointEarned->Checked || cbResetPointBalancedPointPurchased->Checked || cbResetPointBalancedPointRedeemed->Checked)
    {
        edDay->Enabled = true;
        cbConfirmResetPoint->Enabled = true;
    }
    cbConfirmResetPoint->Checked = TGlobalSettings::Instance().ConfirmResetPoint;
    cbGiftCardOnly->Checked = TGlobalSettings::Instance().GiftCardOnly  ;
    //cbIntegratedEftposSmartPay->Checked = TGlobalSettings::Instance().EnableEftPosSmartPay;
    cbDeleteItemSizeAfterSale->Checked = TGlobalSettings::Instance().DeleteItemSizeAfterSale;
    cbDisplayItemSizesAsList->Checked = TGlobalSettings::Instance().ShowItemSizesAsList;
    cbShowItemPricesInTransferScreen->Checked = TGlobalSettings::Instance().ShowItemPriceInTransferScreen;
    cbTerminalExemptFromHappyHour->Checked = TGlobalSettings::Instance().TerminalExemptFromHappyHour;

    cbPostMoneyToGLAccounts->OnClick = NULL;
    cbPostMoneyToGLAccounts->Checked = TGlobalSettings::Instance().PostMoneyToGlAccounts;
    cbPostMoneyToGLAccounts->OnClick = cbPostMoneyToGLAccountsClick;

    cbPostMoneyAsPayments->OnClick = NULL;

    cbPostMoneyAsPayments->Checked = TGlobalSettings::Instance().PostMoneyAsPayment;
    cbPostMoneyAsPayments->OnClick = cbPostMoneyAsPaymentsClick;

    cbPostMoneyToGLAccounts->Enabled = TGlobalSettings::Instance().PostZToAccountingSystem && TGlobalSettings::Instance().IsXeroEnabled;
    cbPostMoneyAsPayments->Enabled = TGlobalSettings::Instance().PostZToAccountingSystem;

    cbUseNMIStandardFontSize->Checked = TGlobalSettings::Instance().SetTextFontSizeOnCustomerDisplay;
    DisplayTextOnWeightLimit();
    cbNotifyForManuallyEnteredWeight->Checked = TGlobalSettings::Instance().NotifyForManuallyEnteredWeight; //
    cbShowScreenToSelectItems->Checked = TGlobalSettings::Instance().ShowScreenToSelectItemForPoint; //
    cbCountPointSpentInRevenue->Checked = TGlobalSettings::Instance().PontsSpentCountedAsRevenue;
    cbShowLargeFonts->Checked = TGlobalSettings::Instance().ShowLargeFonts;
    cbItemSearch->Checked= TGlobalSettings::Instance().ItemSearch;
    cbShowDarkBackgroundInPOS->Checked = TGlobalSettings::Instance().ShowDarkBackground;
    cbOpenCashDrawer->Checked = TGlobalSettings::Instance().OpenCashDrawer;
    cbMergeSimilarItem->Checked = TGlobalSettings::Instance().MergeSimilarItem;
    cbExcludeReceipt->Checked = TGlobalSettings::Instance().ExcludeReceipt;
    cbExcludeXReport->Checked = TGlobalSettings::Instance().ExcludeXReport;
    cbUseBIRFormatInXZReport->Checked = TGlobalSettings::Instance().UseBIRFormatInXZReport;
    isBIRSettingTicked = false;
    cbHideRoundingOnReceipt->Checked = TGlobalSettings::Instance().HideRoundingOnReceipt;
	cbCashDenominationEntry->Checked = TGlobalSettings::Instance().CashDenominationEntry;

    TManagerVariable::Instance().GetProfileBool( DBTransaction, GlobalProfileKey, vmUseMemberSubs, TGlobalSettings::Instance().UseMemberSubs );
    TManagerVariable::Instance().GetProfileBool( DBTransaction, GlobalProfileKey, vmIsBillSplittedByMenuType, TGlobalSettings::Instance().IsBillSplittedByMenuType );
    DBTransaction.Commit();
    cbUseMemberSubs->OnClick = NULL;
    cbUseMemberSubs->Checked = TGlobalSettings::Instance().UseMemberSubs;
    cbUseMemberSubs->OnClick = cbUseMemberSubsClick;
    cbFloatWithdrawFromCash->Checked = TGlobalSettings::Instance().FloatWithdrawFromCash;
    cbSplitBillByMenuType->Checked = TGlobalSettings::Instance().IsBillSplittedByMenuType;
//    cbIntegratedEftposSmartConnect->OnClick = NULL;
//    cbIntegratedEftposSmartConnect->Checked = TGlobalSettings::Instance().EnableEftPosSmartConnect;
//    cbIntegratedEftposSmartConnect->OnClick = cbIntegratedEftposSmartConnectClick;
    //cbIntegratedEftposAdyen->Checked = TGlobalSettings::Instance().EnableEftPosAdyen;
    cbCompanyDetailOnReprintReceipt->Checked = TGlobalSettings::Instance().EnableCompanyDetailOnReprintReceipt ;
    CustomizeCloudEFTPOS();
    FormResize(this);
}
//---------------------------------------------------------------------------
/*
    Refreshes the screen and saves the relevant setting to database.
*/
//---------------------------------------------------------------------------
void TfrmGeneralMaintenance::CustomizeCloudEFTPOS()
{
    cbIntegratedEftposSmartConnect->OnClick = NULL;
    cbIntegratedEftposAdyen->OnClick = NULL;
    if(TGlobalSettings::Instance().EnableEftPosSmartPay)
    {
        cbIntegratedEftposAdyen->Enabled                     = false;
        cbIntegratedEftposAdyen->Checked                     = false;
        cbIntegratedEftposSmartConnect->Enabled              = false;
        cbIntegratedEftposSmartConnect->Checked              = false;
        cbIntegratedEftposSmartPay->Enabled                  = true;
        cbIntegratedEftposSmartPay->Checked                  = true;
        TGlobalSettings::Instance().EnableEftPosAdyen        = false;
        TGlobalSettings::Instance().EnableEftPosSmartConnect = false;
        tbtnSmartLinkIp->Enabled                             = true;
        tbtnSmartLinkIp->Caption                             = "Smart Pay Details";
    }
    else if(TGlobalSettings::Instance().EnableEftPosSmartConnect)
    {
        cbIntegratedEftposAdyen->Enabled                     = false;
        cbIntegratedEftposAdyen->Checked                     = false;
        cbIntegratedEftposSmartPay->Enabled                  = false;
        cbIntegratedEftposSmartPay->Checked                  = false;
        cbIntegratedEftposSmartConnect->Enabled              = true;
        cbIntegratedEftposSmartConnect->Checked              = true;
        TGlobalSettings::Instance().EnableEftPosAdyen        = false;
        tbtnSmartLinkIp->Enabled                             = false;
        tbtnSmartLinkIp->Caption                             = "EFTPOS Network Details";
    }
    else if(TGlobalSettings::Instance().EnableEftPosAdyen)
    {
        cbIntegratedEftposSmartPay->Enabled                  = false;
        cbIntegratedEftposSmartPay->Checked                  = false;
        cbIntegratedEftposSmartConnect->Enabled              = false;
        cbIntegratedEftposSmartConnect->Checked              = false;
        TGlobalSettings::Instance().EnableEftPosSmartPay     = false;
        TGlobalSettings::Instance().EnableEftPosSmartConnect = false;
        cbIntegratedEftposAdyen->Checked                     = true;
        tbtnSmartLinkIp->Enabled                             = true;
        tbtnSmartLinkIp->Caption                             = "Adyen Details";
    }
    else if(!TGlobalSettings::Instance().EnableEftPosSmartPay && !TGlobalSettings::Instance().EnableEftPosAdyen &&
           !TGlobalSettings::Instance().EnableEftPosSmartConnect)
    {
        cbIntegratedEftposAdyen->Enabled                    = true;
        cbIntegratedEftposSmartPay->Enabled                 = true;
        cbIntegratedEftposSmartConnect->Enabled             = true;
        cbIntegratedEftposAdyen->Checked                    = false;
        cbIntegratedEftposSmartPay->Checked                 = false;
        cbIntegratedEftposSmartConnect->Checked             = false;
        tbtnSmartLinkIp->Enabled                            = false;
        tbtnSmartLinkIp->Caption                            = "EFTPOS Network Details";
    }
    cbIntegratedEftposSmartConnect->OnClick = cbIntegratedEftposSmartConnectClick;
    cbIntegratedEftposAdyen->OnClick = cbIntegratedEftposAdyenClick;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmEnableEftPosSmartPay,TGlobalSettings::Instance().EnableEftPosSmartPay);
    TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmEnableEftPosAdyen, TGlobalSettings::Instance().EnableEftPosAdyen);
    TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmEnableEftPosSmartConnect, TGlobalSettings::Instance().EnableEftPosSmartConnect);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------
void __fastcall TfrmGeneralMaintenance::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(this);
}
//---------------------------------------------------------------------------
void __fastcall TfrmGeneralMaintenance::FormResize(TObject *Sender)
{
   const int stdMinScreenHeight = 850;
   const int stdMaxScreenHeight = 1000;
   const int stdScreenWidth = 1600;
   const int screenHeight = 920;
   //	if (Tag != Screen->Width)
  //	{
		int Temp = Tag;
		Tag = Screen->Width;
		if((double)Screen->Width/Screen->Height < 1.4)
		{
			ScaleBy(Screen->Width, Temp);
	 }
  //	}
    
    int TempHeight = ClientHeight;
	pnlButtons->Top = Pages->Left;
	pnlButtons->Left = ClientWidth - pnlButtons->Width - Pages->Left;
	pnlButtons->Height = ClientHeight - (Pages->Left*2);
	Pages->Top = Pages->Left;
	Pages->Width = pnlButtons->Left - (Pages->Left*2);
	Pages->Height =  ClientHeight - (Pages->Left*2);

    Panel1->Height = (ClientHeight - (Pages->Left*2));
    Panel1->Width = pnlButtons->Left;
    //members screen layout.
    Panel5->Height = (TempHeight - (Pages->Left*2));
    Panel5->Width = pnlButtons->Left;

    Panel3->Height = (TempHeight - (Pages->Left*2));
    Panel3->Width = pnlButtons->Left;
    GroupBox10->Top = GroupBox5->Top;
    //Start arrange member screen controls...
    //if(Pages->ActivePage == tsMembershipOptions)
   // {
        GroupBox7->Top = (GroupBox1->Height + GroupBox1->Top);
        rgMemberNameOnRecipt->Top = (GroupBox7->Height + GroupBox7->Top);
        GroupBox16->Top = rgMemberNameOnRecipt->Top + rgMemberNameOnRecipt->Height;
        GroupBox3->Top = (GroupBox16->Height + GroupBox16->Top);
        GroupBox14->Top = GroupBox1->Top;
        GroupBox2->Top = (GroupBox14->Height + GroupBox14->Top);
        GroupBox15->Top = (GroupBox2->Height + GroupBox2->Top);
        rgLocationFreeSalesRules->Top = (GroupBox15->Height + GroupBox15->Top);
        rgMemberFreeSalesRules->Top = rgLocationFreeSalesRules->Top + rgLocationFreeSalesRules->Height;
        grpTierLevel->Top = rgMemberFreeSalesRules->Top + rgMemberFreeSalesRules->Height;
        grpMemberSubs->Top = grpTierLevel->Top + grpTierLevel->Height;
    //}
    if(TempHeight < stdMinScreenHeight)
    {
        if(Pages->ActivePage == tsPointOfSale)
        {
           GroupBox10->Height = Panel1->ClientHeight - GroupBox10->Top; //- (GroupBox10->Left);
        }
        if(Pages->ActivePage == tsMembershipOptions)
        {
            //end arrange member screen controls...
            tbBirthdayBuffer->Height = lbeBirthdayBuffer->Height + lbeFirstVisitPoint->Left;
            tbRestLastVisit->Height = lbeBirthdayBuffer->Height + lbeFirstVisitPoint->Left;
            tbFirstVisitPoint->Height = lbeBirthdayBuffer->Height + lbeFirstVisitPoint->Left;
        }

        if(Pages->ActivePage == tsEndOfDayOptions)
        {

           GroupBox25->Height = TempHeight - (GroupBox26->Left*8);
           tpZedInfo->Height = TempHeight - (cbEnableClientDetails->Height + cbWarnIfOpenTableAndTabs->Height + cbStopIfOpenTableAndTabs->Height + cbDepositBagNum->Height + cbDontClearDataOnZed->Height + lblEndOfDay->Height + cbPostToXero->Height + cbPostMoneyAsPayments->Height+ cbPostMoneyToGLAccounts->Height + cbEnablePaxCount->Height +cbOpenCashDrawer->Height + cbSummariseDiscountsonZed->Height +cbDisplayStockPurchaseWeborders->Height + cbEnableHideCredCancels->Height + cbCombineServiceChargeAndServiceChargeTax->Height + cbCombineServiceChargeTaxAndSalesTax->Height + cbEmailZedClosingTill->Height + GroupBox26->Height + tbtnConfigureEmail->Height + (GroupBox26->Left*4)); //150;
        }
        if(Pages->ActivePage == tsEftPos)
        {
           GroupBox9->Top = Label9->Top + Label9->Height;
           GroupBox13->Top = GroupBox9->Top + GroupBox9->Height;
           cbDuplicateEftPos->Top = GroupBox13->Top + GroupBox13->Height;
           smartLinkGroupBox->Top = cbDuplicateEftPos->Top + cbDuplicateEftPos->Height;
           GroupBox20->Top = smartLinkGroupBox->Top + smartLinkGroupBox->Height;
        }
    }
    if(TempHeight < stdMaxScreenHeight && ClientWidth != stdScreenWidth)
    {
        if(Pages->ActivePage == tsEndOfDayOptions)
        {
           GroupBox25->Height = TempHeight - (GroupBox26->Left*8);
           tpZedInfo->Height = TempHeight - (cbEnableClientDetails->Height + cbWarnIfOpenTableAndTabs->Height + cbStopIfOpenTableAndTabs->Height + cbDepositBagNum->Height + cbDontClearDataOnZed->Height + lblEndOfDay->Height + cbPostToXero->Height + cbPostMoneyAsPayments->Height+ cbPostMoneyToGLAccounts->Height + cbEnablePaxCount->Height +cbOpenCashDrawer->Height + cbSummariseDiscountsonZed->Height +cbDisplayStockPurchaseWeborders->Height + cbEnableHideCredCancels->Height + cbCombineServiceChargeAndServiceChargeTax->Height + cbCombineServiceChargeTaxAndSalesTax->Height + cbEmailZedClosingTill->Height + GroupBox26->Height + tbtnConfigureEmail->Height + (GroupBox26->Left*4)); //150;
        }
        if(TempHeight == screenHeight)
        {
            if(Pages->ActivePage == tsPointOfSale)
            {
               GroupBox10->Height = Panel1->ClientHeight - GroupBox10->Top;
            }
        }
    }

    GroupBox6->Top = GroupBox5->Top + GroupBox5->Height;
    GroupBox21->Top = GroupBox6->Top + GroupBox6->Height;
    gbItemBilling->Top = GroupBox21->Top + GroupBox21->Height;
    GroupBox19->Top = gbItemBilling->Top + gbItemBilling->Height;
    GroupBox23->Top = GroupBox19->Top + GroupBox19->Height;
    GroupBoxEJournal->Top = GroupBox23->Top + GroupBox23->Height;

    GroupBox25->Top = cbEnableClientDetails->Top;
    GroupBox26->Top = tbtnConfigureEmail->Top + tbtnConfigureEmail->Height;

    tpZedInfo->Top = cbEnableClientDetails->Top + cbEnableClientDetails->Height;
    cbWarnIfOpenTableAndTabs->Top = tpZedInfo->Height + tpZedInfo->Top;
    cbStopIfOpenTableAndTabs->Top = cbWarnIfOpenTableAndTabs->Top + cbWarnIfOpenTableAndTabs->Height;
    cbDepositBagNum->Top = cbStopIfOpenTableAndTabs->Top + cbStopIfOpenTableAndTabs->Height;
    cbDontClearDataOnZed->Top = cbDepositBagNum->Top + cbDepositBagNum->Height;
    lblEndOfDay->Top = cbDontClearDataOnZed->Top + cbDontClearDataOnZed->Height;
    cbEndOfDay->Top = cbDontClearDataOnZed->Top + cbDontClearDataOnZed->Height - (GroupBox26->Left);
    cbPostToXero->Top = cbEndOfDay->Top + cbEndOfDay->Height;
    cbPostMoneyAsPayments->Top = cbPostToXero->Top + cbPostToXero->Height;
    cbPostMoneyToGLAccounts->Top = cbPostMoneyAsPayments->Top + cbPostMoneyAsPayments->Height;
    cbEnablePaxCount->Top = cbPostMoneyToGLAccounts->Top + cbPostMoneyToGLAccounts->Height;
    cbOpenCashDrawer->Top = cbEnablePaxCount->Top + cbEnablePaxCount->Height;
    cbSummariseDiscountsonZed->Top = cbOpenCashDrawer->Top + cbOpenCashDrawer->Height;
    cbDisplayStockPurchaseWeborders->Top = cbSummariseDiscountsonZed->Top + cbSummariseDiscountsonZed->Height;
    cbEnableHideCredCancels->Top = cbDisplayStockPurchaseWeborders->Top + cbDisplayStockPurchaseWeborders->Height;
    cbCombineServiceChargeAndServiceChargeTax->Top = cbEnableHideCredCancels->Top + cbEnableHideCredCancels->Height;
    cbCombineServiceChargeTaxAndSalesTax->Top = cbCombineServiceChargeAndServiceChargeTax->Top + cbCombineServiceChargeAndServiceChargeTax->Height;
    cbEmailZedClosingTill->Top = cbCombineServiceChargeTaxAndSalesTax->Top + cbCombineServiceChargeTaxAndSalesTax->Height;
    tbtnConfigureEmail->Top = cbEmailZedClosingTill->Top + (GroupBox26->Left);
    GroupBox26->Top =  tbtnConfigureEmail->Top + tbtnConfigureEmail->Height; //150;
    //eftpos screen alignment.....
    GroupBox9->Top = Label9->Top + Label9->Height;
    GroupBox13->Top = GroupBox9->Top + GroupBox9->Height;
    cbDuplicateEftPos->Top = GroupBox13->Top + GroupBox13->Height;
    smartLinkGroupBox->Top = cbDuplicateEftPos->Top + cbDuplicateEftPos->Height;
    GroupBox20->Top = smartLinkGroupBox->Top + smartLinkGroupBox->Height;
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::btnAutoLogoutTimeClick(
TObject *Sender)
{
	std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
	frmTouchNumpad->Caption = "Enter number of seconds of inactivity before auto logout.";
	frmTouchNumpad->btnSurcharge->Caption = "Ok";
	frmTouchNumpad->btnDiscount->Visible = false;
	frmTouchNumpad->btnSurcharge->Visible = true;
	frmTouchNumpad->Mode = pmNumber;
	if (frmTouchNumpad->ShowModal() == mrOk)
	{
		btnAutoLogoutTime->Caption = frmTouchNumpad->INTResult;
		TGlobalSettings::Instance().AutoLogoutSeconds = frmTouchNumpad->INTResult;
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmAutoLogoutSeconds, frmTouchNumpad->INTResult);
		DBTransaction.Commit();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmGeneralMaintenance::lbeUseAutoLogoutClick(
TObject *Sender)
{
	chbUseAutoLogout->Checked = !chbUseAutoLogout->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TfrmGeneralMaintenance::chbUseAutoLogoutClick(
TObject *Sender)
{
	TGlobalSettings::Instance().AutoLogoutPOS = chbUseAutoLogout->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmAutoLogoutPOS, chbUseAutoLogout->Checked);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------
void __fastcall TfrmGeneralMaintenance::cbRevertCourseClick(
TObject *Sender)
{
	TGlobalSettings::Instance().RevertToFirstCourse = cbRevertCourse->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmRevertToFirstCourse,TGlobalSettings::Instance().RevertToFirstCourse);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------
void __fastcall TfrmGeneralMaintenance::edClearLogsXDaysClick(
TObject *Sender)
{
	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 2;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = edClearLogsXDays->Text;
	frmTouchKeyboard->Caption = "Enter Number Of Days";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
		edClearLogsXDays->Text = frmTouchKeyboard->KeyboardText;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmGeneralMaintenance::btnOkClick(TObject *Sender)
{
	try
	{
		TGlobalSettings::Instance().ClearLogsEveryXDays = StrToInt(edClearLogsXDays->Text);
		Database::TDBTransaction DBTransaction(DBControl);

		DBTransaction.StartTransaction();
		//story MM1837 remove
		//CommitSalesforceEndOfDayOptions(DBTransaction);
		DBTransaction.Commit();

		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmClearLogsDays,TGlobalSettings::Instance().ClearLogsEveryXDays);
		DBTransaction.Commit();
	}
	catch(Exception &E)
	{
		MessageBox(E.Message, "Error",MB_ICONWARNING + MB_OK);
	}
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmGeneralMaintenance::btnCancelClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmGeneralMaintenance::cbForceTenderClick(TObject *Sender)
{
	TDeviceRealTerminal::Instance().PaymentSystem->ForceTender = cbForceTender->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmForceTender,TDeviceRealTerminal::Instance().PaymentSystem->ForceTender);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbFTEFTSalesClick(TObject *Sender)
{
	TDeviceRealTerminal::Instance().PaymentSystem->FTBypassElecTranTyp = cbFTEFTSales->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmFTBypassElecTranTyp,TDeviceRealTerminal::Instance().PaymentSystem->FTBypassElecTranTyp);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------


void __fastcall TfrmGeneralMaintenance::btnNonServiceClick(TObject *Sender)
{
	std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
	frmTouchNumpad->Caption = "Enter maximum non service period. (Minutes)";
	frmTouchNumpad->btnSurcharge->Caption = "Ok";
	frmTouchNumpad->btnDiscount->Visible = false;
	frmTouchNumpad->btnSurcharge->Visible = true;
	frmTouchNumpad->Mode = pmNumber;
	if (frmTouchNumpad->ShowModal() == mrOk)
	{
		btnNonService->Caption = frmTouchNumpad->INTResult;
		TGlobalSettings::Instance().NonServicePeriodMinutes = frmTouchNumpad->INTResult;
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmNonServicePeriod, frmTouchNumpad->INTResult);
		DBTransaction.Commit();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbForcedReasonsClick(TObject *Sender)
{
	TGlobalSettings::Instance().ForcedReasons = cbForcedReasons->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmForcedReasons,TGlobalSettings::Instance().ForcedReasons);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------
void __fastcall TfrmGeneralMaintenance::edSeatLabelMouseUp(TObject *Sender,
TMouseButton Button, TShiftState Shift, int X, int Y)
{
	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 8;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().SeatLabel;
	frmTouchKeyboard->Caption = "Enter Seat Label";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
		edSeatLabel->Text = frmTouchKeyboard->KeyboardText;
		TGlobalSettings::Instance().SeatLabel = frmTouchKeyboard->KeyboardText;
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmSeatLabel,TGlobalSettings::Instance().SeatLabel);
		DBTransaction.Commit();
	}
}

bool
TfrmGeneralMaintenance::GetInputViaTouchKeyboard(
UnicodeString prompt,
UnicodeString &destination,
int max_length,
bool allow_cr,
bool start_with_caps)
{
	TModalResult mr;

	std::auto_ptr<TfrmTouchKeyboard> tkb(
	TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));

	tkb->MaxLength = max_length;
	tkb->AllowCarriageReturn = allow_cr;
	tkb->StartWithShiftDown = start_with_caps;
	tkb->KeyboardText = destination;
	tkb->Caption = prompt;

	if ((mr = tkb->ShowModal()) == mrOk)
	destination = tkb->KeyboardText;

	return mr;
}

//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbSaleAndMakeTimesClick(TObject *Sender)
{
	if(TDeviceRealTerminal::Instance().Modules.Status[eRegSaleTurnAround]["Registered"])
	{
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmTrackSaleAndMakeTimes,cbSaleAndMakeTimes->Checked);
		DBTransaction.Commit();
	}
	else if (cbSaleAndMakeTimes->Checked)
	{
		cbSaleAndMakeTimes->Checked = false;
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmTrackSaleAndMakeTimes,cbSaleAndMakeTimes->Checked);
		DBTransaction.Commit();
		MessageBox("The 'Sale' and 'Make Time' tracking module has not been registered", "Error", MB_OK + MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------


void __fastcall TfrmGeneralMaintenance::cbTableSeatsClick(TObject *Sender)
{
	bool Continue = true;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();

	if(!cbTableSeats->Checked)
	{
		if(frmSelectDish->ParkedSales->GetCount(DBTransaction) > 0)
		{
			Continue = false;
			MessageBox("There are Currently Parked Sales!\rYou must process them before disabling an Interface","Warning",MB_OK + MB_ICONWARNING);
			cbTableSeats->Checked = true;
		}
	}

	if (Continue)
	{
		TGlobalSettings::Instance().TablesEnabled = cbTableSeats->Checked;
		TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmTablesEnabled,TGlobalSettings::Instance().TablesEnabled);
	}
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbTabsInterfaceClick(
TObject *Sender)
{
	bool Continue = true;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();

	if(!cbTabsInterface->Checked)
	{
		if(frmSelectDish->ParkedSales->GetCount(DBTransaction) > 0)
		{
			Continue = false;
			MessageBox("There are Currently Parked Sales!\rYou must process them before disabling an Interface","Warning",MB_OK + MB_ICONWARNING);
			cbTabsInterface->Checked = true;
		}
	}

	if (Continue)
	{
		TGlobalSettings::Instance().TabsEnabled = cbTabsInterface->Checked;
		TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmTabsEnabled,TGlobalSettings::Instance().TabsEnabled);
	}
	DBTransaction.Commit();

}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbForcedSidesFirstClick(TObject *Sender)
{
	TGlobalSettings::Instance().ForcedSidesFirst = cbForcedSidesFirst->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmForcedSidesFirst,TGlobalSettings::Instance().ForcedSidesFirst);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::tbtnPointsGroupClick(TObject *Sender)
{
	std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
	frmTouchNumpad->Caption = "Enter Points Payment group number.";
	frmTouchNumpad->btnSurcharge->Caption = "Ok";
	frmTouchNumpad->btnDiscount->Visible = false;
	frmTouchNumpad->btnSurcharge->Visible = true;
	frmTouchNumpad->Mode = pmNumber;
	frmTouchNumpad->INTInitial = TGlobalSettings::Instance().PointsPaymentGroupNumber;
	if (frmTouchNumpad->ShowModal() == mrOk)
	{
		tbtnPointsGroup->Caption = IntToStr(frmTouchNumpad->INTResult);
		TGlobalSettings::Instance().PointsPaymentGroupNumber = frmTouchNumpad->INTResult;
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmPointsPaymentGroupNumber, frmTouchNumpad->INTResult);
		DBTransaction.Commit();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbDuplicateEftPosClick(
TObject *Sender)
{
	TGlobalSettings::Instance().DuplicateEftPosReceipt = cbDuplicateEftPos->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmDuplicateEftPosReceipt,TGlobalSettings::Instance().DuplicateEftPosReceipt);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------



void __fastcall TfrmGeneralMaintenance::cbIntegratedEftposANZClick(
TObject *Sender)
{
	TGlobalSettings::Instance().EnableEftPosANZ = cbIntegratedEftposANZ->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmEnableEftPosANZ,TGlobalSettings::Instance().EnableEftPosANZ);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbIntegratedEftposSyncroClick(
TObject *Sender)
{
	TGlobalSettings::Instance().EnableEftPosSyncro = cbIntegratedEftposSyncro->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmEnableEftPosSyncro,TGlobalSettings::Instance().EnableEftPosSyncro);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbPointsOnZedClick(TObject *Sender)
{
	TGlobalSettings::Instance().ShowPointsOnZed = cbPointsOnZed->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmShowPointsOnZed,TGlobalSettings::Instance().ShowPointsOnZed);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbShowInvoiceInfoClick(
TObject *Sender)
{
	TGlobalSettings::Instance().ShowInvoiceInfoOnZed = cbShowInvoiceInfo->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmShowInvoiceInfoOnZed,TGlobalSettings::Instance().ShowInvoiceInfoOnZed);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbStaffDisableManualLoginClick(
TObject *Sender)
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	Staff->DisableManualLogin = cbStaffDisableManualLogin->Checked;
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmDisableManualLogin,Staff->DisableManualLogin);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbIntegratedEftposIngenicoClick(
TObject *Sender)
{
	TGlobalSettings::Instance().EnableEftPosIngenico = cbIntegratedEftposIngenico->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmEnableEftPosIngenico,TGlobalSettings::Instance().EnableEftPosIngenico);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------



void __fastcall TfrmGeneralMaintenance::cbEarnPointsClick(TObject *Sender)
{
	if(!cbEarnPoints->Checked && cbOnlyEarnPoints->Checked)
	cbOnlyEarnPoints->Checked = cbEarnPoints->Checked;

	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmEarnsPointsWhileRedeeming,cbEarnPoints->Checked);
	DBTransaction.Commit();
	if(cbEarnPoints->Checked)
	{
		TGlobalSettings::Instance().SystemRules << eprEarnsPointsWhileRedeemingPoints;
	}
	else
	{
		TGlobalSettings::Instance().SystemRules >> eprEarnsPointsWhileRedeemingPoints;
	}

}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbIncrementalSearchClick(
TObject *Sender)
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmMemberIncrementalSearch,cbIncrementalSearch->Checked);
	TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->EnableIncrementalSearch = cbIncrementalSearch->Checked;
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::tbPOSClick(TObject *Sender)
{
	Pages->ActivePage = tsPointOfSale;
	RedrawButtons(Sender);
    FormResize(this);
}
//---------------------------------------------------------------------------


void __fastcall TfrmGeneralMaintenance::tbStaffClick(TObject *Sender)
{
	Pages->ActivePage = tsStaffOptions;
	RedrawButtons(Sender);

}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::tbMembershipClick(TObject *Sender)
{
	Pages->ActivePage = tsMembershipOptions;
    RedrawButtons(Sender);
    FormResize(this);
}
//---------------------------------------------------------------------------


void __fastcall TfrmGeneralMaintenance::tbEftposClick(TObject *Sender)
{
	Pages->ActivePage = tsEftPos;
	RedrawButtons(Sender);
    FormResize(this);
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::tbLoggingClick(TObject *Sender)
{
	Pages->ActivePage = tsLogging;
	RedrawButtons(Sender);
}
//---------------------------------------------------------------------------

//add by frn
void __fastcall TfrmGeneralMaintenance::tbCustomerDisplayClick(TObject *Sender)
{
	Pages->ActivePage = tsCustomerDisplay;
	RedrawButtons(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfrmGeneralMaintenance::tbPOSDisplayClick(TObject *Sender)
{
	Pages->ActivePage = tsPointOfSaleDisplay;
	RedrawButtons(Sender);
}
//---------------------------------------------------------------------------
void TfrmGeneralMaintenance::RedrawButtons(TObject * Sender)
{
	TTouchBtn* CurrentButton = (TTouchBtn*) Sender;
	//	CurrentButton->ButtonColor = clNavy;

	for (int i = 0; i < pnlButtons->ControlCount ; i++)
	{
		TTouchBtn* Button = (TTouchBtn*)pnlButtons->Controls[i];
		if(Button->Tag != -1 && Button != CurrentButton)
		{
			//			Button->ButtonColor = clMaroon;
			Button->Latched = false;
		}
		else
		{
			Button->Latched = true;
		}
	}
}

void __fastcall TfrmGeneralMaintenance::rgMemberPoleDisplayClick(
TObject *Sender)
{
	Database::TDBTransaction DBTransaction(DBControl);
	TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	DBTransaction.StartTransaction();
	// TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->NameOnPoleDisplay = (eMemberNameOnPoleDisplay)rgMemberPoleDisplay->ItemIndex;
	TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmMemberNameOnPoleDisplay,TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->NameOnPoleDisplay);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::tbLogsResetClick(TObject *Sender)
{
	TManagerLogs::Instance().ErrorsToLog->Clear();
	TManagerLogs::Instance().ErrorsToLog->Add(EXCEPTIONLOG);
	TManagerLogs::Instance().ErrorsToLog->Add(ERRORLOG);
	ReDrawLogs();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::ReDrawLogs()
{
	MemLogs->Lines = TManagerLogs::Instance().ErrorsToLog.get();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::tbLogDebugInfoClick(
TObject *Sender)
{
	TManagerLogs::Instance().ErrorsToLog->Add(DEBUGLOG);
	ReDrawLogs();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::tbLogPhoenixInfoClick(TObject *Sender)
{
	TManagerLogs::Instance().ErrorsToLog->Add(PHOENIXINTERFACELOG);
	ReDrawLogs();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::tbLogBTInfoClick(TObject *Sender)
{
	TManagerLogs::Instance().ErrorsToLog->Add(PALMBTLAYERLOG);
	ReDrawLogs();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::tbLogBTPacketInfoClick(TObject *Sender)
{
	TManagerLogs::Instance().ErrorsToLog->Add(PALMPACKETLAYERLOG);
	ReDrawLogs();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::tbLogBTDecodeInfoClick(TObject *Sender)
{
	TManagerLogs::Instance().ErrorsToLog->Add(PALMDECODEPACKETLOG);
	ReDrawLogs();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbOnLineLoggingClick(
TObject *Sender)
{
	//	TManagerLogs::Instance().OnLineLogging = cbOnLineLogging->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::rgMemberFreeSalesRulesClick(TObject *Sender)
{
	ManagerFreebie->MemberLoyaltyRewardsRules = (eFreeSalesRules)rgMemberFreeSalesRules->ItemIndex;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmMemberLoyaltyRewardsRules,ManagerFreebie->MemberLoyaltyRewardsRules);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::rgLocationFreeSalesRulesClick(
TObject *Sender)
{
	ManagerFreebie->LocationLoyaltyRewardsRules = (eFreeSalesRules)rgLocationFreeSalesRules->ItemIndex;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmLocationLoyaltyRewardsRules,ManagerFreebie->LocationLoyaltyRewardsRules);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::tbMemberQuickButtonsClick(
TObject *Sender)
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	int QucikButtonCount = TManagerVariable::Instance().GetInt(DBTransaction,vmMemberQuickButtonCount,4);
	std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
	frmTouchNumpad->Caption = "Enter the number of Quick Buttons.";
	frmTouchNumpad->btnSurcharge->Caption = "Ok";
	frmTouchNumpad->btnDiscount->Visible = false;
	frmTouchNumpad->btnSurcharge->Visible = true;
	frmTouchNumpad->Mode = pmNumber;
	frmTouchNumpad->INTInitial = QucikButtonCount;
	if (frmTouchNumpad->ShowModal() == mrOk)
	{
		tbMemberQuickButtons->Caption = IntToStr(frmTouchNumpad->INTResult);
		TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmMemberQuickButtonCount, frmTouchNumpad->INTResult);
	}
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbHideServingCourseLabelsClick(
TObject *Sender)
{
	TGlobalSettings::Instance().HideServingCourseLabels = cbHideServingCourseLabels->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmHideServingCourseLabels,cbHideServingCourseLabels->Checked);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbRememberLastServingCourseClick(
TObject *Sender)
{
	TGlobalSettings::Instance().RememberLastServingCourse = cbRememberLastServingCourse->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmRememberLastServingCourse,cbRememberLastServingCourse->Checked);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------


void __fastcall TfrmGeneralMaintenance::cbIntegratedEftposCadmusClick(
TObject *Sender)
{
	TGlobalSettings::Instance().EnableEftPosCadmus = cbIntegratedEftposCadmus->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmEnableEftPosCadmus,TGlobalSettings::Instance().EnableEftPosCadmus);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbEftposCadmusSerialPortChange(
TObject *Sender)
{
	int SerialPortNumber = -1;
	if(cbEftposCadmusSerialPort->ItemIndex != -1)
	{
		if(cbEftposCadmusSerialPort->ItemIndex != 0)
		{
			SerialPortNumber = cbEftposCadmusSerialPort->ItemIndex;
		}
		else
		{
			SerialPortNumber = -1;
		}
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmEftposSerialPort,SerialPortNumber);
		DBTransaction.Commit();
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::tbCadmusMerchantNumberClick(
TObject *Sender)
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	int MerchantNumber = TManagerVariable::Instance().GetInt(DBTransaction,vmEftposMerchantNumber,1);
	std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
	frmTouchNumpad->Caption = "Enter the Merchant Number.";
	frmTouchNumpad->btnSurcharge->Caption = "Ok";
	frmTouchNumpad->btnDiscount->Visible = false;
	frmTouchNumpad->btnSurcharge->Visible = true;
	frmTouchNumpad->Mode = pmNumber;
	frmTouchNumpad->INTInitial = MerchantNumber;
	if (frmTouchNumpad->ShowModal() == mrOk)
	{
		tbCadmusMerchantNumber->Caption = IntToStr(frmTouchNumpad->INTResult);
		TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmEftposMerchantNumber, frmTouchNumpad->INTResult);
	}
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbPromptPatronCountClick(TObject *Sender)
{
    TGlobalSettings::Instance().PromptForPatronCount = cbPromptPatronCount->Checked;
    Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();
    TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmPromptForPatronCount,TGlobalSettings::Instance().PromptForPatronCount);
    DBTransaction.Commit();
    if(cbPromptPatronCount->Checked)
    {
        cbPromptPatronCountOnTableSales->Checked = false;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::tbtnLuckMemberPeriodClick(
TObject *Sender)
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();

	int LuckyMemberPeriod = TManagerVariable::Instance().GetInt(DBTransaction,vmLuckyMemberPeriod,24);
	std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
	frmTouchNumpad->Caption = "Enter Lucky Member Period (Hours).";
	frmTouchNumpad->btnSurcharge->Caption = "Ok";
	frmTouchNumpad->btnDiscount->Visible = false;
	frmTouchNumpad->btnSurcharge->Visible = true;
	frmTouchNumpad->Mode = pmNumber;
	frmTouchNumpad->INTInitial = LuckyMemberPeriod;
	if (frmTouchNumpad->ShowModal() == mrOk)
	{
		tbtnLuckMemberPeriod->Caption = IntToStr(frmTouchNumpad->INTResult);
		TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmLuckyMemberPeriod, frmTouchNumpad->INTResult);
		TGlobalSettings::Instance().LuckyMemberPeriod	= frmTouchNumpad->INTResult;
	}
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::tbtnLuckMemberPeriodDayClick(
TObject *Sender)
{
	int LuckyMemberPeriod = 24;
	tbtnLuckMemberPeriod->Caption = IntToStr(LuckyMemberPeriod);
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmLuckyMemberPeriod, LuckyMemberPeriod);
	DBTransaction.Commit();
	TGlobalSettings::Instance().LuckyMemberPeriod	= LuckyMemberPeriod;
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::tbtnLuckMemberPeriodWeekClick(
TObject *Sender)
{
	int LuckyMemberPeriod = 24*7;
	tbtnLuckMemberPeriod->Caption = IntToStr(LuckyMemberPeriod);
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmLuckyMemberPeriod, LuckyMemberPeriod);
	DBTransaction.Commit();
	TGlobalSettings::Instance().LuckyMemberPeriod	= LuckyMemberPeriod;
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::tbtnLuckMemberPeriodMonthClick(
TObject *Sender)
{
	int LuckyMemberPeriod = 24*30;
	tbtnLuckMemberPeriod->Caption = IntToStr(LuckyMemberPeriod);
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmLuckyMemberPeriod, LuckyMemberPeriod);
	DBTransaction.Commit();
	TGlobalSettings::Instance().LuckyMemberPeriod	= LuckyMemberPeriod;
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::TouchBtn1MouseClick(
TObject *Sender)
{
	HWND PrevWnd;

	PrevWnd = FindWindow(_T("TfrmMain"), _T("LogsMate"));
	if (PrevWnd != NULL)
	{
		PrevWnd = GetWindow(PrevWnd, GW_OWNER);
		if (PrevWnd != NULL)
		{
			if (IsIconic(PrevWnd))
			{
				ShowWindow(PrevWnd, SW_RESTORE);
			}
			else
			{
				SetForegroundWindow(PrevWnd);
			}
		}
	}
	else
	{
		_wspawnl(
		P_NOWAIT,
		(ExtractFilePath(Application->ExeName) + UnicodeString("LogsMate.exe")).w_str(),
		(ExtractFilePath(Application->ExeName) + UnicodeString("LogsMate.exe")).w_str(),
		NULL);

		::Sleep(1000);
		PrevWnd = FindWindow(_T("TfrmMain"), _T("LogsMate"));
		if (PrevWnd != NULL)
		{
			PrevWnd = GetWindow(PrevWnd, GW_OWNER);
			if (PrevWnd != NULL)
			{
				if (IsIconic(PrevWnd))
				{
					ShowWindow(PrevWnd, SW_RESTORE);
				}
				else
				{
					SetForegroundWindow(PrevWnd);
				}
			}
		}
	}

}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbICELinkClick(TObject *Sender)
{
	TGlobalSettings::Instance().EnableEftPosIceLink = cbICELink->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmEnableEftPosIceLink,TGlobalSettings::Instance().EnableEftPosIceLink);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------


void __fastcall TfrmGeneralMaintenance::cbDefaultToBevMenuClick(
TObject *Sender)
{
	TGlobalSettings::Instance().DefaultToBevMenu  = cbDefaultToBevMenu->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmPOSDefaultsToBeverages,TGlobalSettings::Instance().DefaultToBevMenu);
	DBTransaction.Commit();
}

//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbSummariseDiscountsonZedClick(
TObject *Sender)
{
	TGlobalSettings::Instance().SummariseDiscountOnZed  = cbSummariseDiscountsonZed->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmSummariseDiscountOnZed,TGlobalSettings::Instance().SummariseDiscountOnZed);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbRoundingClick(TObject *Sender)
{
	TGlobalSettings::Instance().MidPointRoundsDown = cbRounding->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();


	int GlobalProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction,eSystemProfiles,"Globals");
	if(GlobalProfileKey == 0)
	{
		GlobalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction,eSystemProfiles,"Globals");
	}

	TManagerVariable::Instance().SetProfileBool(DBTransaction,GlobalProfileKey,vmMidPointRoundsDown, TGlobalSettings::Instance().MidPointRoundsDown);

	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::tbtnSyncroniseMembersipMouseClick(
TObject *Sender)
{
	try
	{
		std::auto_ptr<TfrmProcessing>(Processing)(TfrmProcessing::Create<TfrmProcessing>(this));
		Processing->CanCancel = true;
		Processing->Title = "Synchronising Members";
		Processing->Message = "Synchronising Members";
		Processing->ShowProgress = true;
		Processing->Cancelled = false;
		Processing->Top = 0;
		Processing->Left = 0;

		Database::TDBTransaction DBTransaction(DBControl);
		TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
		DBTransaction.StartTransaction();
		TDeviceRealTerminal::Instance().ManagerMembership->SyncMembers(DBTransaction,Processing.get());
		DBTransaction.Commit();
	}
	catch(Exception &E)
	{
		MessageBox(E.Message, "Error",MB_ICONWARNING + MB_OK);
	}

}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::tbMinMembershipNumberClick(
TObject *Sender)
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	int MinMembershipNum = TManagerVariable::Instance().GetInt(DBTransaction,vmMinMemberNumber,1000);
	std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
	frmTouchNumpad->Caption = "Enter the Min Membership Number.";
	frmTouchNumpad->btnSurcharge->Caption = "Ok";
	frmTouchNumpad->btnDiscount->Visible = false;
	frmTouchNumpad->btnSurcharge->Visible = true;
	frmTouchNumpad->Mode = pmNumber;
	frmTouchNumpad->INTInitial = MinMembershipNum;
	if (frmTouchNumpad->ShowModal() == mrOk)
	{
		tbMinMembershipNumber->Caption = IntToStr(frmTouchNumpad->INTResult);
		TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmMinMemberNumber, frmTouchNumpad->INTResult);
	}
	DBTransaction.Commit();
}



//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbRecycleMembershipNumbersClick(
TObject *Sender)
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->RecycleMemberNumber = cbRecycleMembershipNumbers->Checked;
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmRecycleMemberNumber,cbRecycleMembershipNumbers->Checked);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::tbSmartCardMouseClick(
TObject *Sender)
{
	TManagerLogs::Instance().ErrorsToLog->Add(SMARTCARDLOG);
	ReDrawLogs();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbOnlyEarnPointsClick(
TObject *Sender)
{
	if(cbOnlyEarnPoints->Checked)
	cbEarnPoints->Checked = cbOnlyEarnPoints->Checked;

	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmOnlyEarnsPointsWhileRedeeming,cbOnlyEarnPoints->Checked);
	DBTransaction.Commit();
	if(cbOnlyEarnPoints->Checked)
	{
		TGlobalSettings::Instance().SystemRules << eprOnlyEarnsPointsWhileRedeemingPoints;
	}
	else
	{
		TGlobalSettings::Instance().SystemRules >> eprOnlyEarnsPointsWhileRedeemingPoints;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbIntegratedEftposDPSClick(
TObject *Sender)
{
	TGlobalSettings::Instance().EnableEftPosDPS = cbIntegratedEftposDPS->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmEnableEftPosDPS,TGlobalSettings::Instance().EnableEftPosDPS);
	DBTransaction.Commit();

	cbEnableDPSTipping->Enabled = TGlobalSettings::Instance().EnableEftPosDPS;
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbEnableDPSTippingClick(
TObject *Sender)
{
	TGlobalSettings::Instance().EnableDPSTipping = cbEnableDPSTipping->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmEnableDPSTipping,TGlobalSettings::Instance().EnableDPSTipping);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbEnableMembershipSmartCardsClick(
TObject *Sender)
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	if(TDeviceRealTerminal::Instance().SetSiteID(DBTransaction))
	{
		TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmSmartCardMembership,cbEnableMembershipSmartCards->Checked);
		MessageBox("You will need to restart MenuMate for this to take effect.", "Restart Required", MB_OK + MB_ICONINFORMATION);
		lbeEntireSiteID->Caption = "Site ID " + IntToStr(TGlobalSettings::Instance().SiteID);
	}
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::memStaffMessageClick(
TObject *Sender)
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();

	UnicodeString StaffMessageOfTheDay = "";
	int GlobalProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction,eSystemProfiles,"Globals");
	if(GlobalProfileKey != 0)
	{
		TManagerVariable::Instance().GetProfileStr(DBTransaction,GlobalProfileKey,vmStaffMessageOfTheDay,StaffMessageOfTheDay);
	}

	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 125;
	frmTouchKeyboard->AllowCarriageReturn = true;
	frmTouchKeyboard->CloseOnDoubleCarriageReturn = false;
	frmTouchKeyboard->KeyboardText = StaffMessageOfTheDay;
	frmTouchKeyboard->Caption = "Enter A Staff Message";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
		StaffMessageOfTheDay = frmTouchKeyboard->memText->Text;

		int GlobalProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction,eSystemProfiles,"Globals");
		if(GlobalProfileKey == 0)
		{
			GlobalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction,eSystemProfiles,"Globals");
		}

		TManagerVariable::Instance().SetProfileStr(DBTransaction,GlobalProfileKey,vmStaffMessageOfTheDay, StaffMessageOfTheDay);
		memStaffMessage->Text = StaffMessageOfTheDay;
	}
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::tbtnAddDefaultDeptMouseClick(
TObject *Sender)
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TManagerTimeClock> ManagerTimeClock( new TManagerTimeClock);
	ManagerTimeClock->SetDefaultDept(DBTransaction);
	DBTransaction.Commit();
	UpdateTimeClockGrid(true);
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::tbtnDelDeptMouseClick(
TObject *Sender)
{
	int DelKey = 0;
	for (int i = 0; i < tbgTimeCodes->RowCount ; i++)
	{
		if(tbgTimeCodes->Buttons[i][0]->Latched)
		{
			DelKey = tbgTimeCodes->Buttons[i][0]->Tag;

		}
	}
            TMMContactInfo TempUserInfo;
            Database::TDBTransaction DBTransaction(DBControl);
            DBTransaction.StartTransaction();
            std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
            std::auto_ptr<TManagerTimeClock> ManagerTimeClock( new TManagerTimeClock);
            TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckPINChange);
	       if (Result == lsAccepted)
          {
            if(ManagerTimeClock->ClockedInDep(DBTransaction,DelKey))
            {
              MessageBox("Department is already Logged In,you can't delete department until department is Logged Out. ", "Error", MB_OK + MB_ICONERROR);
            }

            else
              {
              if (ManagerTimeClock->CheckClockedIn(DBTransaction,DelKey))
             {
                ManagerTimeClock->DelClockInDept(DBTransaction,DelKey);
                ManagerTimeClock->DelClockInDeptFromLoc(DBTransaction,DelKey);
                DBTransaction.Commit();
                UpdateTimeClockGrid(true);
              }
                 else
              {

                 ManagerTimeClock->DelClockInDeptFromLoc(DBTransaction,DelKey);
                 DBTransaction.Commit();
                 UpdateTimeClockGrid(true);
             }
             }
             }
    else if (Result == lsDenied)
	{
		MessageBox("You require change PIN rights to Clock In.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}



   /*      if(DelKey != 0)
       {
	     	try
      {
        MessageBox("elsecase", "Error", MB_OK + MB_ICONERROR);
        ManagerTimeClock->DelClockInDept(DBTransaction,DelKey);
			DBTransaction.Commit();
			UpdateTimeClockGrid(true);
		}
		catch(Exception &E)
		{
			MessageBox(E.Message, "Error",MB_ICONWARNING + MB_OK);
		}
	}   */


  }

//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::tbtnAddDeptMouseClick(
TObject *Sender)
{
	try
	{
		std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
		frmTouchKeyboard->MaxLength = 25;
		frmTouchKeyboard->AllowCarriageReturn = false;
		frmTouchKeyboard->StartWithShiftDown = false;
		frmTouchKeyboard->KeyboardText = "";
		frmTouchKeyboard->Caption = "Enter Department Name";
		if (frmTouchKeyboard->ShowModal() == mrOk)
		{
			AnsiString DeptName = frmTouchKeyboard->KeyboardText;

			std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
			frmTouchNumpad->Mode = pmNumber;
			frmTouchNumpad->Caption = "Enter Department Code.";
			frmTouchNumpad->btnSurcharge->Caption = "Ok";
			frmTouchNumpad->btnDiscount->Visible = false;
			frmTouchNumpad->btnSurcharge->Visible = true;
			frmTouchNumpad->INTInitial = 1;
			if (frmTouchNumpad->ShowModal() == mrOk)
			{
				int DeptCode = frmTouchNumpad->INTResult;
             //   AnsiString DeptName = frmTouchKeyboard->KeyboardText;
				Database::TDBTransaction DBTransaction(DBControl);
				DBTransaction.StartTransaction();
                std::auto_ptr<TManagerTimeClock> ManagerTimeClock( new TManagerTimeClock);
                ManagerTimeClock->AddClockInDept(DBTransaction,DeptName,DeptCode);
                DBTransaction.Commit();
				UpdateTimeClockGrid(true);
			}
		}
	}
	catch (Exception &E)
	{
		MessageBox(E.Message, "Error", MB_OK + MB_ICONERROR);
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}
//---------------------------------------------------------------------------

void TfrmGeneralMaintenance::UpdateTimeClockGrid(bool GridEnabled)
{
	tbgTimeCodes->ColCount = 0;
	tbgTimeCodes->RowCount = 0;

	std::auto_ptr<TStringList> ClockInDeptList(new TStringList);

	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TManagerTimeClock> ManagerTimeClock( new TManagerTimeClock);
	ManagerTimeClock->GetClockInDeptList(DBTransaction,ClockInDeptList.get());
	DBTransaction.Commit();

	tbgTimeCodes->ColCount = 1;
	tbgTimeCodes->RowCount = ClockInDeptList->Count;

	for (int i = 0; i < tbgTimeCodes->RowCount ; i++)
	{
		tbgTimeCodes->Buttons[i][0]->Enabled = true;
		tbgTimeCodes->Buttons[i][0]->Caption = ClockInDeptList->Strings[i];
		tbgTimeCodes->Buttons[i][0]->Tag = (int)(ClockInDeptList->Objects[i]);
	}
}

void __fastcall TfrmGeneralMaintenance::tbtnDeptUpMouseClick(
TObject *Sender)
{
	int Key = 0;
	for (int i = 0; i < tbgTimeCodes->RowCount ; i++)
	{
		if(tbgTimeCodes->Buttons[i][0]->Latched)
		{
			Key = tbgTimeCodes->Buttons[i][0]->Tag;
		}
	}

	if(Key != 0)
	{
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		std::auto_ptr<TManagerTimeClock> ManagerTimeClock( new TManagerTimeClock);
		ManagerTimeClock->MoveDisplayOrderUp(DBTransaction,Key);
		DBTransaction.Commit();
		UpdateTimeClockGrid(true);
	}

	for (int i = 0; i < tbgTimeCodes->RowCount ; i++)
	{
		if(Key == tbgTimeCodes->Buttons[i][0]->Tag)
		{
			tbgTimeCodes->Buttons[i][0]->Latched = true;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::tbtnDeptDownMouseClick(
TObject *Sender)
{
	int Key = 0;
	for (int i = 0; i < tbgTimeCodes->RowCount ; i++)
	{
		if(tbgTimeCodes->Buttons[i][0]->Latched)
		{
			Key = tbgTimeCodes->Buttons[i][0]->Tag;
		}
	}

	if(Key != 0)
	{
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		std::auto_ptr<TManagerTimeClock> ManagerTimeClock( new TManagerTimeClock);
		ManagerTimeClock->MoveDisplayOrderDown(DBTransaction,Key);
		DBTransaction.Commit();
		UpdateTimeClockGrid(true);
	}

	for (int i = 0; i < tbgTimeCodes->RowCount ; i++)
	{
		if(Key == tbgTimeCodes->Buttons[i][0]->Tag)
		{
			tbgTimeCodes->Buttons[i][0]->Latched = true;
		}
	}


}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::tbtnMMSubNetClick(TObject *Sender)
{
	std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
	frmTouchNumpad->Caption = "Enter the MenuMate Subnet";
	frmTouchNumpad->btnSurcharge->Caption = "Ok";
	frmTouchNumpad->btnDiscount->Visible = false;
	frmTouchNumpad->btnSurcharge->Visible = true;
	frmTouchNumpad->Mode = pmNumber;
	if (frmTouchNumpad->ShowModal() == mrOk)
	{
		TDeviceRealTerminal::Instance().ManagerNet->MMSubNet = frmTouchNumpad->INTResult;
		tbtnMMSubNet->Caption = IntToStr(TDeviceRealTerminal::Instance().ManagerNet->MMSubNet);

		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmMMSubNet, TDeviceRealTerminal::Instance().ManagerNet->MMSubNet);
		DBTransaction.Commit();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::tbtnDeptEditMouseClick(
TObject *Sender)
{
	int DeptKey = 0;
	for (int i = 0; i < tbgTimeCodes->RowCount ; i++)
	{
		if(tbgTimeCodes->Buttons[i][0]->Latched)
		{
			DeptKey = tbgTimeCodes->Buttons[i][0]->Tag;
		}
	}

	if(DeptKey != 0)
	{
		try
		{
			std::auto_ptr<TManagerTimeClock> ManagerTimeClock( new TManagerTimeClock);
			Database::TDBTransaction DBTransaction(DBControl);
			DBTransaction.StartTransaction();

			std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
			frmTouchKeyboard->MaxLength = 25;
			frmTouchKeyboard->AllowCarriageReturn = false;
			frmTouchKeyboard->StartWithShiftDown = false;
			frmTouchKeyboard->KeyboardText = ManagerTimeClock->GetClockInDeptName(DBTransaction,DeptKey);
			frmTouchKeyboard->Caption = "Alter Department Name";
			if (frmTouchKeyboard->ShowModal() == mrOk)
			{
				AnsiString DeptName = frmTouchKeyboard->KeyboardText;

				std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
				frmTouchNumpad->Mode = pmNumber;
				frmTouchNumpad->Caption = "Enter Department Code.";
				frmTouchNumpad->btnSurcharge->Caption = "Ok";
				frmTouchNumpad->btnDiscount->Visible = false;
				frmTouchNumpad->btnSurcharge->Visible = true;
				frmTouchNumpad->INTInitial = ManagerTimeClock->GetClockInDeptCode(DBTransaction,DeptKey);
				if (frmTouchNumpad->ShowModal() == mrOk)
				{
					int DeptCode = frmTouchNumpad->INTResult;

					ManagerTimeClock->UpdateClockInDept(DBTransaction,DeptName,DeptCode,DeptKey);
					UpdateTimeClockGrid(true);
				}
			}
			DBTransaction.Commit();
			UpdateTimeClockGrid(true);
		}
		catch(Exception &E)
		{
			MessageBox(E.Message, "Error",MB_ICONWARNING + MB_OK);
		}
	}
}
//---------------------------------------------------------------------------


void __fastcall TfrmGeneralMaintenance::tbtnTabCreditGroupClick(
TObject *Sender)
{
	std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
	frmTouchNumpad->Caption = "Enter Tab Credit Payment group number.";
	frmTouchNumpad->btnSurcharge->Caption = "Ok";
	frmTouchNumpad->btnDiscount->Visible = false;
	frmTouchNumpad->btnSurcharge->Visible = true;
	frmTouchNumpad->Mode = pmNumber;
	frmTouchNumpad->INTInitial = TGlobalSettings::Instance().CreditPaymentGroupNumber;
	if (frmTouchNumpad->ShowModal() == mrOk)
	{
		tbtnTabCreditGroup->Caption = IntToStr(frmTouchNumpad->INTResult);
		TGlobalSettings::Instance().CreditPaymentGroupNumber = frmTouchNumpad->INTResult;
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmCreditPaymentGroupNumber, frmTouchNumpad->INTResult);
		DBTransaction.Commit();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbvmAllowSmartCardNotPresentMemberTransactionsClick(TObject *Sender)
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TDeviceRealTerminal::Instance().ManagerMembership->AllowSmartCardNotPresentMemberTransactions = cbvmAllowSmartCardNotPresentMemberTransactions->Checked;
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmAllowSmartCardNotPresentMemberTransactions,TDeviceRealTerminal::Instance().ManagerMembership->AllowSmartCardNotPresentMemberTransactions);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbSacleAutoConvertWeightsClick(
TObject *Sender)
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TGlobalSettings::Instance().AutoConvertScalesWeights = cbSacleAutoConvertWeights->Checked;
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmAutoConvertScalesWeights,TGlobalSettings::Instance().AutoConvertScalesWeights);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbScaleEnterWeightClick(TObject *Sender)
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TGlobalSettings::Instance().AllowToEnterWeight = cbScaleEnterWeight->Checked;
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmAllowEnterWeight,TGlobalSettings::Instance().AllowToEnterWeight);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbScaleEnterPriceClick(TObject *Sender)
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TGlobalSettings::Instance().AllowToEnterPrice = cbScaleEnterPrice->Checked;
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmAllowEnterPrice,TGlobalSettings::Instance().AllowToEnterPrice);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbEnableAARewardsClick(TObject *Sender)
{
	if(TGlobalSettings::Instance().EnableEftPosSyncro)
	{
		TGlobalSettings::Instance().EnableAARewards = cbEnableAARewards->Checked;
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmEnableAARewards,TGlobalSettings::Instance().EnableAARewards);
		DBTransaction.Commit();
	}
	else
	{
		cbEnableAARewards->OnClick = NULL;
		cbEnableAARewards->Checked = false;
		cbEnableAARewards->OnClick = cbEnableAARewardsClick;
		TGlobalSettings::Instance().EnableAARewards = cbEnableAARewards->Checked;
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmEnableAARewards,TGlobalSettings::Instance().EnableAARewards);
		DBTransaction.Commit();
		MessageBox("You must have Provenco EFTPOS installed.\r", "Sorry you need Provenco EFTPOS.", MB_OK + MB_ICONQUESTION);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmGeneralMaintenance::rgMemberNameOnReciptClick(
TObject *Sender)
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	Receipt->MembershipNameDisplay = rgMemberNameOnRecipt->ItemIndex;
	TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmMembersNameOnReceipt,Receipt->MembershipNameDisplay);
	DBTransaction.Commit();

}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::TouchBtn19MouseClick(
TObject *Sender)
{
	TManagerLogs::Instance().ErrorsToLog->Add(INTAMATEINTERFACELOG);
	ReDrawLogs();
}
//---------------------------------------------------------------------------
void __fastcall TfrmGeneralMaintenance::cbCadmusCronosClick(TObject *Sender)
{
	TGlobalSettings::Instance().EnableEftPosCadmusCronos = cbCadmusCronos->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmEnableEftPosCadmusCronos,TGlobalSettings::Instance().EnableEftPosCadmusCronos);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbDisableInternalInvoicingClick(TObject *Sender)
{
	TGlobalSettings::Instance().DisableInternalInvoicing = cbDisableInternalInvoicing->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmDisableInternalInvoicing,TGlobalSettings::Instance().DisableInternalInvoicing);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbAARewardsReminderClick(
TObject *Sender)
{
	TGlobalSettings::Instance().EnableAARewardsReminder = cbAARewardsReminder->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmEnableAARewardsReminder,TGlobalSettings::Instance().EnableAARewardsReminder);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbDEnableBlindBalancesClick(TObject *Sender)
{
	TGlobalSettings::Instance().EnableBlindBalances = cbDEnableBlindBalances->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmEnableBlindBalances,TGlobalSettings::Instance().EnableBlindBalances);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbEndOfDayChange(TObject *Sender)
{
	TGlobalSettings::Instance().EndOfDay = cbEndOfDay->ItemIndex;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmEndOfDay,TGlobalSettings::Instance().EndOfDay);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbEndOfDaySelect(TObject *Sender)
{
	TGlobalSettings::Instance().EndOfDay = cbEndOfDay->ItemIndex;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmEndOfDay,TGlobalSettings::Instance().EndOfDay);
	DBTransaction.Commit();
}

//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbDisableSaveConfirmationClick(TObject *Sender)
{
	TGlobalSettings::Instance().DisableConfirmationOnSave = cbDisableSaveConfirmation->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmDisableConfirmationOnSave,TGlobalSettings::Instance().DisableConfirmationOnSave);
	DBTransaction.Commit();
}


void __fastcall TfrmGeneralMaintenance::cbDisableReceiptOnConfirmationClick(
TObject *Sender)
{
	TGlobalSettings::Instance().DisableReceiptOnConfirmation = cbDisableReceiptOnConfirmation->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmDisableReceiptOnConfirmation, TGlobalSettings::Instance().DisableReceiptOnConfirmation);
	DBTransaction.Commit();
}


void __fastcall TfrmGeneralMaintenance::cbEnableShowVoucherDetails(
TObject *Sender)
{
	TGlobalSettings::Instance().ShowVoucherDetails =
	cbShowVoucherDetails->Checked;
	Database::TDBTransaction DBTransaction(DBControl);

	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(
	DBTransaction,
	vmShowVoucher,
	TGlobalSettings::Instance().ShowVoucherDetails);
	DBTransaction.Commit();
}


void __fastcall TfrmGeneralMaintenance::cbEnablePhoneOrdersMouseClick(TObject *sender)
{
	TGlobalSettings::Instance().EnablePhoneOrders = cbEnablePhoneOrders->Checked;
	Database::TDBTransaction DBTransaction(DBControl);

	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(
	DBTransaction,
	vmEnablePhoneOrders,
	TGlobalSettings::Instance().EnablePhoneOrders);
	DBTransaction.Commit();
}

//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbEnableParkedSalesClicked(TObject *sender)
{
	TGlobalSettings::Instance().EnableParkedSales = cbEnableParkedSales->Checked;

	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmEnableParkedSales,TGlobalSettings::Instance().EnableParkedSales);
	DBTransaction.Commit();

	//::::::::::::::::::::::::::::::::::::

	if( !cbEnableParkedSales->Checked  )
	{
		Database::TDBTransaction DBTransaction( TDeviceRealTerminal::Instance().DBControl );
		DBTransaction.StartTransaction();

		ClearAllParkedSales( DBTransaction );

		DBTransaction.Commit();
	}

}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::lbeEntireSiteIDMouseUp(
TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
int Y)
{
	if(Shift.Contains(ssCtrl))
	{
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		if(TDeviceRealTerminal::Instance().SetSiteID(DBTransaction,true))
		{
			MessageBox("You will need to restart MenuMate for this to take effect.", "Restart Required", MB_OK + MB_ICONINFORMATION);
			lbeEntireSiteID->Caption = "Site ID " + IntToStr(TGlobalSettings::Instance().SiteID);
		}
		DBTransaction.Commit();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::TouchBtn20MouseClick(
TObject *Sender)
{
	TManagerLogs::Instance().ErrorsToLog->Add(XMLINTERFACELOG);
	ReDrawLogs();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbNotifyBirthdayClick(
TObject *Sender)
{
	TGlobalSettings::Instance().EnableBirthdayNotifications = cbNotifyBirthday->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmEnableBirthdayNotifications,TGlobalSettings::Instance().EnableBirthdayNotifications);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbAcknowledgeFirstVisitClick(
TObject *Sender)
{
	TGlobalSettings::Instance().AcknowledgeFirstVisit = cbAcknowledgeFirstVisit->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmAcknowledgeFirstVisit,TGlobalSettings::Instance().AcknowledgeFirstVisit);
	DBTransaction.Commit();

	if(TGlobalSettings::Instance().AcknowledgeFirstVisit == true)
	{
		if ( MessageBox("Do you wish all members to be set to 'Not Visited'?", "Clear last visit times.", MB_YESNO + MB_ICONQUESTION) == IDYES)
		{
			Database::TDBTransaction DBTransaction(DBControl);
			DBTransaction.StartTransaction();
			TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->ClearLastVisitTimes(DBTransaction);
			DBTransaction.Commit();
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::tbRestLastVisitClick(
TObject *Sender)
{
	if ( MessageBox("This can take some time. \r Are you sure?", "Reseting Members last visit times.", MB_YESNO + MB_ICONQUESTION) == IDYES)
	{
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->ResetLastVisitTimes(DBTransaction);
		DBTransaction.Commit();
		MessageBox("Finished Reseting Members.", "All Done.", MB_OK + MB_ICONINFORMATION);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbEnableMemberSpendChitsClick(TObject *Sender)
{
	TGlobalSettings::Instance().EnableMemberSpendChits = cbEnableMemberSpendChits->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmEnableMemberSpendChits,TGlobalSettings::Instance().EnableMemberSpendChits);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::tbtnMemberSpendChitValueClick(
TObject *Sender)
{
	std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
	frmTouchNumpad->Caption = "Amount Required For a Chit";
	frmTouchNumpad->btnSurcharge->Caption = "Ok";
	frmTouchNumpad->btnDiscount->Visible = false;
	frmTouchNumpad->btnSurcharge->Visible = true;
	frmTouchNumpad->Mode = pmCurrency;
	frmTouchNumpad->CURInitial = TGlobalSettings::Instance().MemberSpendChitsAmount;
	if (frmTouchNumpad->ShowModal() == mrOk)
	{
		TGlobalSettings::Instance().MemberSpendChitsAmount = frmTouchNumpad->CURResult;
		tbtnMemberSpendChitValue->Caption =  "$" + CurrToStr(TGlobalSettings::Instance().MemberSpendChitsAmount);

		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmMemberSpendChitsAmount,CurrToStr(TGlobalSettings::Instance().MemberSpendChitsAmount));
		DBTransaction.Commit();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbDrawByLocationClick(TObject *Sender)
{
	TGlobalSettings::Instance().LuckyDrawByLocationOnly = cbDrawByLocation->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmLuckyMemberDrawByLocationOnly,TGlobalSettings::Instance().LuckyDrawByLocationOnly);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbAutoAddWebMembersClick(TObject *Sender)
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TGlobalSettings::Instance().AutoAddWebMembers = cbAutoAddWebMembers->Checked;
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmAutoAddWebMembers,TGlobalSettings::Instance().AutoAddWebMembers);
	DBTransaction.Commit();

}

//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::rgPOSBtnSizeClick(TObject *Sender)
{
	TGlobalSettings  &ref_gs = TGlobalSettings::Instance();
	TManagerVariable &ref_mv = TManagerVariable::Instance();

	int prof_key;
	Database::TDBTransaction tr(DBControl);

	tr.StartTransaction();
	if (!(prof_key = ref_mv.GetProfile(tr, eSystemProfiles, "Globals")))
	prof_key = ref_mv.SetProfile(tr, eSystemProfiles, "Globals");
	tr.Commit();

	ref_gs.DefaultColumns = ((TRadioGroup *)Sender)->ItemIndex + 4;
	ref_gs.DefaultRows    = ((TRadioGroup *)Sender)->ItemIndex + 8;

	tr.StartTransaction();
	ref_mv.SetProfileInt(tr, prof_key, vmPosGuiDimensionsColumns,
	ref_gs.DefaultColumns);
	ref_mv.SetProfileInt(tr, prof_key, vmPosGuiDimensionsRows,
	ref_gs.DefaultRows);

	if (TfrmSetButtonFontSize::IsCurrentSizeDefault())
	TfrmSetButtonFontSize::ResetToDefaultFontSize();
	else {
		if (MessageBox(
					"A custom font size of "
					+ IntToStr(TfrmSetButtonFontSize::GetCurrentFontSize(tr, prof_key))
					+ "pts has been set. Do you want to set it to this button's default "
					"font size of "
					+ IntToStr(TfrmSetButtonFontSize::GetDefaultFontSize())
					+ "pts?", "Overwrite current font size?",
					MB_YESNO) == IDYES) {
			TfrmSetButtonFontSize::ResetToDefaultFontSize();
		}
	}

	tr.Commit();
}

void __fastcall TfrmGeneralMaintenance::btnSetPOSFontSizeMouseClick(
TObject *Sender)
{
	std::auto_ptr<TfrmSetButtonFontSize> sel_btn_fnt_sz(
	new TfrmSetButtonFontSize(this));
	sel_btn_fnt_sz->ShowModal();
}

//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::tbEndOfDayClick(TObject *Sender)
{
	Pages->ActivePage = tsEndOfDayOptions;
	RedrawButtons(Sender);
    FormResize(this);
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::bankingMouseClick(TObject *Sender)
{
	CallBlindBalance();
}
//---------------------------------------------------------------------------

void TfrmGeneralMaintenance::CallBlindBalance(void)
{
	/*Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TBlindBalanceController BlindBalanceController(this, DBTransaction);
	BlindBalanceController.Run();*/
}
void __fastcall TfrmGeneralMaintenance::EnableStaffHoursOnClick(TObject *Sender)
{
	TGlobalSettings::Instance().EnableStaffHours = cbEnableStaffHours->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmEnableStaffHours,TGlobalSettings::Instance().EnableStaffHours);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbEnableCommissionClick(TObject *Sender)
{
	TGlobalSettings::Instance().EnableCommission = cbEnableCommission->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmEnableCommission,TGlobalSettings::Instance().EnableCommission);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------
//story MM1837 remove
/*void TfrmGeneralMaintenance::CommitSalesforceEndOfDayOptions(
Database::TDBTransaction &tr)
{
	TGlobalSettings &gsi  = TGlobalSettings::Instance();
	TManagerVariable &mvi = TManagerVariable::Instance();

	int global_pkey = mvi.GetProfile(tr, eSystemProfiles, "Globals");

	if (!global_pkey)
		global_pkey = mvi.SetProfile(tr, eSystemProfiles, "Globals");


gsi.SalesforceUploadEnabled = cbUploadToSalesforce->Checked;
	gsi.SalesforceUploadStock   = cbUploadStockToSalesforce->Checked;
	gsi.SalesforceUsername      = edSfUsername->Text;
	gsi.SalesforcePassword      = edSfPassword->Text;
	gsi.SalesforceSecurityToken = edSfSecurityToken->Text;

	mvi.SetProfileBool(tr, global_pkey, vmSalesforceUploadEnabled,
					gsi.SalesforceUploadEnabled);
	mvi.SetProfileStr(tr, global_pkey, vmSalesforceUsername,
					gsi.SalesforceUsername);
	mvi.SetProfileStr(tr, global_pkey, vmSalesforcePassword,
					gsi.SalesforcePassword);
	mvi.SetProfileStr(tr, global_pkey, vmSalesforceSecurityToken,
					gsi.SalesforceSecurityToken);

	mvi.SetDeviceBool(tr, vmSalesforceUploadStock, gsi.SalesforceUploadStock);
}*/

void __fastcall TfrmGeneralMaintenance::cbEnablePrinterCountsClick(TObject *Sender)
{
	TGlobalSettings::Instance().EnablePrinterCounts = cbEnablePrinterCounts->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmEnablePrinterCounts,TGlobalSettings::Instance().EnablePrinterCounts);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbEnablePaxCountClick(TObject *Sender)
{
	TGlobalSettings::Instance().EnablePaxCount = cbEnablePaxCount->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmEnablePaxCount,TGlobalSettings::Instance().EnablePaxCount);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbDisplayStockPurchaseWebordersClick(TObject *Sender)

{
	TManagerVariable &mv = TManagerVariable::Instance();

	int pk;
	Database::TDBTransaction tr(DBControl);

	tr.StartTransaction();
	if (!(pk = mv.GetProfile(tr, eSystemProfiles, "Globals")))
	pk = mv.SetProfile(tr, eSystemProfiles, "Globals");
	tr.Commit();

	TGlobalSettings::Instance().PrintStockWeborders =
	cbDisplayStockPurchaseWeborders->Checked;

	tr.StartTransaction();
	mv.SetProfileBool(tr, pk, vmPrintStockWeborders,
	cbDisplayStockPurchaseWeborders->Checked);
	tr.Commit();

}
//---------------------------------------------------------------------------
//story MM1837 remove
/*void __fastcall
TfrmGeneralMaintenance::cbUploadToSalesforceClick(
TObject *Sender)
{
	bool is_checked = cbUploadToSalesforce->Checked;
	edSfUsername->Enabled = is_checked;
	edSfPassword->Enabled = is_checked;
	edSfSecurityToken->Enabled = is_checked;
}*/
/*
void __fastcall
TfrmGeneralMaintenance::cbUploadStockToSalesforceClick(
TObject *Sender)
{
	;
}*/

/*void __fastcall TfrmGeneralMaintenance::edSfClick(TObject *Sender)
{
	static const UnicodeString prompts[3] = {
	"Username", "Password", "Security Token"
	};

	TEdit *ed = reinterpret_cast<TEdit *>(Sender);
UnicodeString edbuf = ed->Text;

	if (GetInputViaTouchKeyboard("Please enter your Salesforce "
								+ prompts[ed->Tag], edbuf, 64, false, false))
		ed->Text = edbuf;
}*/
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbHideCredsCancelsClick(TObject *Sender)
{
	TGlobalSettings::Instance().EnableHideCredsCancels = cbEnableHideCredCancels->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmHideCredsCancels,TGlobalSettings::Instance().EnableHideCredsCancels);
	DBTransaction.Commit();

}

void __fastcall TfrmGeneralMaintenance::cbDepositBagNumClick(TObject *Sender)
{
	TGlobalSettings::Instance().EnableDepositBagNum = cbDepositBagNum->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmDepositBagNum,TGlobalSettings::Instance().EnableDepositBagNum);
	DBTransaction.Commit();
	cbDontClearDataOnZed->Enabled = true;
	if(cbDepositBagNum->Checked)
	cbDontClearDataOnZed->Enabled = false;

}

void __fastcall TfrmGeneralMaintenance::cbDontClearDataOnZedClick(TObject *Sender)
{
	TGlobalSettings::Instance().EnableDontClearZedData = cbDontClearDataOnZed->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmDontClearZedData,TGlobalSettings::Instance().EnableDontClearZedData);
	DBTransaction.Commit();
	cbDepositBagNum->Enabled = true;
	cbEnableStaffHours->Enabled = true;
	cbEnableCommission->Enabled = true;
	cbEnablePrinterCounts->Enabled = true;
	cbEnablePaxCount->Enabled = true;

    cbEmailZedClosingTill->Enabled = true;
    tbtnConfigureEmail->Enabled = true;
	//story MM1837 remove
	//cbUploadToSalesforce->Enabled = true;
	if(cbDontClearDataOnZed->Checked)
	{
		cbDepositBagNum->Enabled = false;
		cbDepositBagNum->Checked = false;
		cbEnableStaffHours->Enabled = false;
		cbEnableStaffHours->Checked = false;
		cbEnableCommission->Enabled = false;
		cbEnableCommission->Checked = false;
		cbEnablePrinterCounts->Enabled = false;
		cbEnablePrinterCounts->Checked = false;
		cbEnablePaxCount->Enabled = false;
		cbEnablePaxCount->Checked = false;

        cbEmailZedClosingTill->Enabled = false;
        tbtnConfigureEmail->Enabled = false;
        cbEmailZedClosingTill->Checked = false;

	}
}

void __fastcall TfrmGeneralMaintenance::cbSeparatePtsClick(TObject *Sender)
{
	TGlobalSettings::Instance().EnableSeperateEarntPts = cbSeparatePts->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmEnableSeperateEarntPts,TGlobalSettings::Instance().EnableSeperateEarntPts);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbCaptureCustomerNameClick(TObject *Sender)
{
	TGlobalSettings::Instance().CaptureCustomerName = cbCaptureCustomerName->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmCaptureCustomer,TGlobalSettings::Instance().CaptureCustomerName);
	DBTransaction.Commit();
}



void __fastcall TfrmGeneralMaintenance::cbHoldSendClick(TObject *Sender)
{
	TGlobalSettings::Instance().EnableHoldSend = cbHoldSend->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmHoldSend,TGlobalSettings::Instance().EnableHoldSend);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbOnlyFullProductsClick(TObject *Sender)
{
	TGlobalSettings::Instance().SpendPtsOnlyFullProduct = cbOnlyFullProducts->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmSpendPtsOnlyFullProduct,TGlobalSettings::Instance().SpendPtsOnlyFullProduct);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbRedeemOnSelectedItemsClick(TObject *Sender)
{
	TGlobalSettings::Instance().SpendPtsOnlySelectItems = cbRedeemOnSelectedItems->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmSpendPtsOnlySelectItems,TGlobalSettings::Instance().SpendPtsOnlySelectItems);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbEarnPointsOnWeightClick(TObject *Sender)
{
	TGlobalSettings::Instance().EarntPointsOnWeight = cbEarnPointsOnWeight->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmEarntPointsOnWeight, TGlobalSettings::Instance().EarntPointsOnWeight);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbHideFreeItemsAtBillingClick(TObject *Sender)
{
	TGlobalSettings::Instance().HideFreeItemsAtBilling = cbHideFreeItemsAtBilling->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmHideFreeItemsAtBilling,TGlobalSettings::Instance().HideFreeItemsAtBilling);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbEnableWaiterStationClick(TObject *Sender)
{
	TGlobalSettings::Instance().EnableWaiterStation = cbEnableWaiterStation->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetProfileBool(
	DBTransaction,
	TManagerVariable::Instance().DeviceProfileKey ,
	vmEnableWaiterStation,
	TGlobalSettings::Instance().EnableWaiterStation);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbPoleDisplayShowPointsClick(TObject *Sender)
{
	TGlobalSettings::Instance().ShowPointsOnDisplay = cbPoleDisplayShowPoints->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmPoleDisplayShowPoints,TGlobalSettings::Instance().ShowPointsOnDisplay);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::rbNoGreetingClick(TObject *Sender)
{
	cbPoleDisplayShowPoints->Enabled = false;
	Database::TDBTransaction DBTransaction(DBControl);
	TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	DBTransaction.StartTransaction();
	TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->NameOnPoleDisplay = eMNPDNone;
	TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmMemberNameOnPoleDisplay,TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->NameOnPoleDisplay);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::rbShowAliasClick(TObject *Sender)
{
	cbPoleDisplayShowPoints->Enabled = true;
	Database::TDBTransaction DBTransaction(DBControl);
	TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	DBTransaction.StartTransaction();
	TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->NameOnPoleDisplay = eMNPDAlias;
	TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmMemberNameOnPoleDisplay,TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->NameOnPoleDisplay);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::rbShowNameClick(TObject *Sender)
{
	cbPoleDisplayShowPoints->Enabled = true;
	Database::TDBTransaction DBTransaction(DBControl);
	TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	DBTransaction.StartTransaction();
	TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->NameOnPoleDisplay = eMNPDFullName;
	TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmMemberNameOnPoleDisplay,TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->NameOnPoleDisplay);
	DBTransaction.Commit();
}


void __fastcall TfrmGeneralMaintenance::rgRoundingTimesClick(TObject *Sender)
{
	TGlobalSettings  &ref_gs = TGlobalSettings::Instance();
	TManagerVariable &ref_mv = TManagerVariable::Instance();

	int prof_key;
	Database::TDBTransaction tr(DBControl);

	tr.StartTransaction();
#pragma warn -pia
	if (!(prof_key = ref_mv.GetProfile(tr, eSystemProfiles, "Globals")))
	prof_key = ref_mv.SetProfile(tr, eSystemProfiles, "Globals");
#pragma warn .pia
	tr.Commit();

	ref_gs.RoundingTime = rgRoundingTimes->ItemIndex + 1;

	tr.StartTransaction();
	ref_mv.SetProfileInt(tr, prof_key, vmRoundingTime,
	ref_gs.RoundingTime);
	tr.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbDisplayTaxClick(TObject *Sender)
{
	TGlobalSettings::Instance().EnableDisplayTax = cbDisplayTax->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmEnableDisplayTax,TGlobalSettings::Instance().EnableDisplayTax);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbEnableTableModeClick(TObject *Sender)
{
	TGlobalSettings::Instance().EnableTableDisplayMode = cbEnableTableMode->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,
	vmEnableTableDisplayMode,TGlobalSettings::Instance().EnableTableDisplayMode);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbDisplayServiceChargeClick(TObject *Sender)
{
	TGlobalSettings::Instance().EnableDisplayServiceCharge = cbDisplayServiceCharge->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmEnableDisplayServiceCharge,TGlobalSettings::Instance().EnableDisplayServiceCharge);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbEnableNmiDisplayClick(TObject *Sender)
{
	TGlobalSettings::Instance().EnableNmiDisplay = cbEnableNmiDisplay->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmEnableNmiDisplay,TGlobalSettings::Instance().EnableNmiDisplay);
	DBTransaction.Commit();
}
void __fastcall TfrmGeneralMaintenance::cbDisplayTaxExemptClick(TObject *Sender)
{
	TGlobalSettings::Instance().EnableDisplayTaxRemoval = cbDisplayTaxExempt->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmEnableDisplayTaxRemoval,TGlobalSettings::Instance().EnableDisplayTaxRemoval);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbInclServiceChargeTaxInTaxValClick(TObject *Sender)
{
	TGlobalSettings::Instance().IncludeServiceChargeTaxInTaxValue = cbInclServiceChargeTaxInTaxVal->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmInclServiceChargeTaxInTaxVal,TGlobalSettings::Instance().IncludeServiceChargeTaxInTaxValue);
	DBTransaction.Commit();
}

//---------------------------------------------------------------------------

// This function is for saving the status if the text box of Accumulated Zed Total is
// checked or not
void __fastcall TfrmGeneralMaintenance::cbShowAccumulatedTotalClick(TObject *Sender)
{

	TGlobalSettings  &gs = TGlobalSettings::Instance();
	TManagerVariable &mv = TManagerVariable::Instance();

	int global_profile_key;
	Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);

	tr.StartTransaction();

	// This is used to retain the state of the checkbox if the POS is exited
#pragma warn -pia
	if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
	global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
#pragma warn .pia

	mv.SetProfileBool(
	tr,
	global_profile_key,
	vmShowAccumulatedZed,
	gs.ShowAccumulatedZeds = cbShowAccumulatedTotal->Checked);

	tr.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbShowSessionDateClick(TObject *Sender)
{
	TGlobalSettings::Instance().ShowSessionDateInZed = cbShowSessionDate->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmShowSessionDateInZed,TGlobalSettings::Instance().ShowSessionDateInZed);
	DBTransaction.Commit();
}
void __fastcall TfrmGeneralMaintenance::cbShowTaxSummaryClick(TObject *Sender)
{
	TGlobalSettings  &gs = TGlobalSettings::Instance();
	TManagerVariable &mv = TManagerVariable::Instance();

	int global_profile_key;
	Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);

	tr.StartTransaction();

	// This is used to retain the state of the checkbox if the POS is exited
#pragma warn -pia
	if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
	global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
#pragma warn .pia

	mv.SetProfileBool(
	tr,
	global_profile_key,
	vmShowTaxSummary,
	gs.ShowTaxSummary = cbShowTaxSummary->Checked);

	tr.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbShowServiceChargeSummaryClick(TObject *Sender)
{
	TGlobalSettings  &gs = TGlobalSettings::Instance();
	TManagerVariable &mv = TManagerVariable::Instance();

	int global_profile_key;
	Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);

	tr.StartTransaction();

	// This is used to retain the state of the checkbox if the POS is exited
#pragma warn -pia
	if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
	global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
#pragma warn .pia

	mv.SetProfileBool(
	tr,
	global_profile_key,
	vmShowServiceChargeSummary,
	gs.ShowServiceChargeSummary = cbShowServiceChargeSummary->Checked);

	cbCombineServiceChargeAndServiceChargeTax->Enabled = (TGlobalSettings::Instance().ShowServiceChargeSummary) ? true : false;
	if(!TGlobalSettings::Instance().ShowServiceChargeSummary)
	{
		cbCombineServiceChargeAndServiceChargeTax->Checked = false;
	}

	tr.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbEnableClientDetailsClick(TObject *Sender)
{
	TGlobalSettings  &gs = TGlobalSettings::Instance();
	TManagerVariable &mv = TManagerVariable::Instance();

	int global_profile_key;
	Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);

	tr.StartTransaction();

	// This is used to retain the state of the checkbox if the POS is exited
#pragma warn -pia
	if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
	global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
#pragma warn .pia

	mv.SetProfileBool(
	tr,
	global_profile_key,
	vmEnableClientDetails,
	gs.EnableClientDetails = cbEnableClientDetails->Checked);

	tr.Commit();

	tbzedHeaderText->Enabled = (TGlobalSettings::Instance().EnableClientDetails) ? true : false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmGeneralMaintenance::cbWarnIfOpenTableAndTabsClick(TObject *Sender)
{
	cbStopIfOpenTableAndTabs->Enabled = !cbWarnIfOpenTableAndTabs->Checked;

	TGlobalSettings::Instance().WarnIfOpenTableAndTabs = cbWarnIfOpenTableAndTabs->Checked;
	TGlobalSettings::Instance().StopIfOpenTableAndTabs = cbStopIfOpenTableAndTabs->Checked;

	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();

	TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmWarnIfOpenTableAndTabs, TGlobalSettings::Instance().WarnIfOpenTableAndTabs);
	TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmStopIfOpenTableAndTabs, TGlobalSettings::Instance().StopIfOpenTableAndTabs);

	DBTransaction.Commit();

	/*TGlobalSettings  &gs = TGlobalSettings::Instance();
	TManagerVariable &mv = TManagerVariable::Instance();

	int global_profile_key;
	Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);

	tr.StartTransaction();

	// This is used to retain the state of the checkbox if the POS is exited
#pragma warn -pia
	if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
	global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
#pragma warn .pia

	mv.SetProfileBool(
	tr,
	global_profile_key,
	vmWarnIfOpenTableAndTabs,
	gs.WarnIfOpenTableAndTabs = cbWarnIfOpenTableAndTabs->Checked);

	tr.Commit(); */
}
//---------------------------------------------------------------------------
void __fastcall TfrmGeneralMaintenance::cbStopIfOpenTableAndTabsClick(TObject *Sender)
{
	cbWarnIfOpenTableAndTabs->Enabled = !cbStopIfOpenTableAndTabs->Checked;

	TGlobalSettings::Instance().StopIfOpenTableAndTabs = cbStopIfOpenTableAndTabs->Checked;
	TGlobalSettings::Instance().WarnIfOpenTableAndTabs = cbWarnIfOpenTableAndTabs->Checked;

	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();

	TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmWarnIfOpenTableAndTabs, TGlobalSettings::Instance().WarnIfOpenTableAndTabs);
	TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmStopIfOpenTableAndTabs, TGlobalSettings::Instance().StopIfOpenTableAndTabs);

	DBTransaction.Commit();

	/*TGlobalSettings  &gs = TGlobalSettings::Instance();
	TManagerVariable &mv = TManagerVariable::Instance();

	int global_profile_key;
	Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);

	tr.StartTransaction();

	// This is used to retain the state of the checkbox if the POS is exited
#pragma warn -pia
	if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
	global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
#pragma warn .pia

	mv.SetProfileBool(
	tr,
	global_profile_key,
	vmStopIfOpenTableAndTabs,
	gs.StopIfOpenTableAndTabs = cbStopIfOpenTableAndTabs->Checked);

	tr.Commit();*/
}
//---------------------------------------------------------------------------
//story mm1837
void __fastcall TfrmGeneralMaintenance::cbShowTransactionSummaryGroupsClick(TObject *Sender)
{
	TGlobalSettings  &gs = TGlobalSettings::Instance();
	TManagerVariable &mv = TManagerVariable::Instance();

	int global_profile_key;
	Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);

	tr.StartTransaction();

	// This is used to retain the state of the checkbox if the POS is exited
#pragma warn -pia
	if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
	global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
#pragma warn .pia

	mv.SetProfileBool(
	tr,
	global_profile_key,
	vmShowTransactionSummaryGroups,
	gs.ShowTransactionSummaryGroups = cbShowTransactionSummaryGroups->Checked);

	tr.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbShowBilledSalesTotalsClick(TObject *Sender)
{
	TGlobalSettings  &gs = TGlobalSettings::Instance();
	TManagerVariable &mv = TManagerVariable::Instance();

	int global_profile_key;
	Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);

	tr.StartTransaction();

	// This is used to retain the state of the checkbox if the POS is exited
#pragma warn -pia
	if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
	global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
#pragma warn .pia

	mv.SetProfileBool(
	tr,
	global_profile_key,
	vmShowBilledSalesTotals,
	gs.ShowBilledSalesTotals = cbShowBilledSalesTotals->Checked);

	tr.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbShowComplimentarySalesTotalsClick(TObject *Sender)
{
	TGlobalSettings  &gs = TGlobalSettings::Instance();
	TManagerVariable &mv = TManagerVariable::Instance();

	int global_profile_key;
	Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);

	tr.StartTransaction();

	// This is used to retain the state of the checkbox if the POS is exited
#pragma warn -pia
	if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
	global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
#pragma warn .pia

	mv.SetProfileBool(
	tr,
	global_profile_key,
	vmShowComplimentarySalesTotals,
	gs.ShowComplimentarySalesTotals= cbShowComplimentarySalesTotals->Checked);

	tr.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbShowChargedSalesTotalsClick(TObject *Sender)
{
	TGlobalSettings  &gs = TGlobalSettings::Instance();
	TManagerVariable &mv = TManagerVariable::Instance();

	int global_profile_key;
	Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);

	tr.StartTransaction();

	// This is used to retain the state of the checkbox if the POS is exited
#pragma warn -pia
	if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
	global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
#pragma warn .pia

	mv.SetProfileBool(
	tr,
	global_profile_key,
	vmShowChargedSalesTotals,
	gs.ShowChargedSalesTotals = cbShowChargedSalesTotals->Checked);

	tr.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbShowTotalsClick(TObject *Sender)
{
	TGlobalSettings  &gs = TGlobalSettings::Instance();
	TManagerVariable &mv = TManagerVariable::Instance();

	int global_profile_key;
	Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);

	tr.StartTransaction();

	// This is used to retain the state of the checkbox if the POS is exited
#pragma warn -pia
	if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
	global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
#pragma warn .pia

	mv.SetProfileBool(
	tr,
	global_profile_key,
	vmShowTotals,
	gs.ShowTotals = cbShowTotals->Checked);

	tr.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbShowBreakdownCategoriesClick(TObject *Sender)
{
	TGlobalSettings  &gs = TGlobalSettings::Instance();
	TManagerVariable &mv = TManagerVariable::Instance();

	int global_profile_key;
	Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);

	tr.StartTransaction();

	// This is used to retain the state of the checkbox if the POS is exited
#pragma warn -pia
	if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
	global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
#pragma warn .pia

	mv.SetProfileBool(
	tr,
	global_profile_key,
	vmShowBreakdownCategories,
	gs.ShowBreakdownCategories = cbShowBreakdownCategories->Checked);

	tr.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbShowDiscountReportClick(TObject *Sender)
{
	TGlobalSettings  &gs = TGlobalSettings::Instance();
	TManagerVariable &mv = TManagerVariable::Instance();

	int global_profile_key;
	Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);

	tr.StartTransaction();

	// This is used to retain the state of the checkbox if the POS is exited
#pragma warn -pia
	if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
	global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
#pragma warn .pia

	mv.SetProfileBool(
	tr,
	global_profile_key,
	vmShowDiscountReport,
	gs.ShowDiscountReport = cbShowDiscountReport->Checked);

	tr.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbShowPointsReportClick(TObject *Sender)
{
	TGlobalSettings  &gs = TGlobalSettings::Instance();
	TManagerVariable &mv = TManagerVariable::Instance();

	int global_profile_key;
	Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);

	tr.StartTransaction();

	// This is used to retain the state of the checkbox if the POS is exited
#pragma warn -pia
	if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
	global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
#pragma warn .pia

	mv.SetProfileBool(
	tr,
	global_profile_key,
	vmShowPointsReport,
	gs.ShowPointsReport = cbShowPointsReport->Checked);

	tr.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbShowPatronAveragesClick(TObject *Sender)
{
	TGlobalSettings  &gs = TGlobalSettings::Instance();
	TManagerVariable &mv = TManagerVariable::Instance();

	int global_profile_key;
	Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);

	tr.StartTransaction();

	// This is used to retain the state of the checkbox if the POS is exited
#pragma warn -pia
	if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
	global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
#pragma warn .pia

	mv.SetProfileBool(
	tr,
	global_profile_key,
	vmShowPatronAverages,
	gs.ShowPatronAverages = cbShowPatronAverages->Checked);

	tr.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbShowProductionInfoClick(TObject *Sender)
{
	TGlobalSettings  &gs = TGlobalSettings::Instance();
	TManagerVariable &mv = TManagerVariable::Instance();

	int global_profile_key;
	Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);

	tr.StartTransaction();

	// This is used to retain the state of the checkbox if the POS is exited
#pragma warn -pia
	if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
	global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
#pragma warn .pia

	mv.SetProfileBool(
	tr,
	global_profile_key,
	vmShowProductionInfo,
	gs.ShowProductionInfo = cbShowProductionInfo->Checked);

	tr.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbShowAccountPurchasesClick(TObject *Sender)
{
	TGlobalSettings  &gs = TGlobalSettings::Instance();
	TManagerVariable &mv = TManagerVariable::Instance();

	int global_profile_key;
	Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);

	tr.StartTransaction();

	// This is used to retain the state of the checkbox if the POS is exited
#pragma warn -pia
	if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
	global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
#pragma warn .pia

	mv.SetProfileBool(
	tr,
	global_profile_key,
	vmShowAccountPurchases,
	gs.ShowAccountPurchases = cbShowAccountPurchases->Checked);

	tr.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbShowAccountBalancesTabsClick(TObject *Sender)
{
	TGlobalSettings  &gs = TGlobalSettings::Instance();
	TManagerVariable &mv = TManagerVariable::Instance();

	int global_profile_key;
	Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);

	tr.StartTransaction();

	// This is used to retain the state of the checkbox if the POS is exited
#pragma warn -pia
	if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
	global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
#pragma warn .pia

	mv.SetProfileBool(
	tr,
	global_profile_key,
	vmShowAccountBalancesTabs,
	gs.ShowAccountBalancesTabs = cbShowAccountBalancesTabs->Checked);

	tr.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbShowAccountBalancesClick(TObject *Sender)
{
	TGlobalSettings  &gs = TGlobalSettings::Instance();
	TManagerVariable &mv = TManagerVariable::Instance();

	int global_profile_key;
	Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);

	tr.StartTransaction();

	// This is used to retain the state of the checkbox if the POS is exited
#pragma warn -pia
	if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
	global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
#pragma warn .pia

	mv.SetProfileBool(
	tr,
	global_profile_key,
	vmShowAccountBalances,
	gs.ShowAccountBalances = cbShowAccountBalances->Checked);

	tr.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbShowHourlySalesClick(TObject *Sender)
{
	TGlobalSettings  &gs = TGlobalSettings::Instance();
	TManagerVariable &mv = TManagerVariable::Instance();

	int global_profile_key;
	Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);

	tr.StartTransaction();

	// This is used to retain the state of the checkbox if the POS is exited
#pragma warn -pia
	if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
	global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
#pragma warn .pia

	mv.SetProfileBool(
	tr,
	global_profile_key,
	vmShowHourlySales,
	gs.ShowHourlySales = cbShowHourlySales->Checked);

	tr.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbRevenueFiguresAreTaxAndServiceChargeInclusiveClick(TObject *Sender)
{
	TGlobalSettings  &gs = TGlobalSettings::Instance();
	TManagerVariable &mv = TManagerVariable::Instance();

	int global_profile_key;
	Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);

	tr.StartTransaction();

	// This is used to retain the state of the checkbox if the POS is exited
#pragma warn -pia
	if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
	global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
#pragma warn .pia

	mv.SetProfileBool(
	tr,
	global_profile_key,
	vmRevenueFiguresAreTaxAndServiceChargeInclusive,
	gs.RevenueFiguresAreTaxAndServiceChargeInclusive = cbRevenueFiguresAreTaxAndServiceChargeInclusive->Checked);

	tr.Commit();
}
//---------------------------------------------------------------------------
void __fastcall TfrmGeneralMaintenance::tbzedHeaderTextClick(TObject *Sender)
{
	std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
	frmTouchKeyboard->MaxLength = 0;
	frmTouchKeyboard->AllowCarriageReturn = true;
	frmTouchKeyboard->CloseOnDoubleCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = tbzedHeaderText->Lines->Text;
	frmTouchKeyboard->Caption = "Enter Zed Header";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
		tbzedHeaderText->Lines->Text = frmTouchKeyboard->KeyboardText;
		TDeviceRealTerminal::Instance().SaveZedHeader(tbzedHeaderText->Lines);
		TDeviceRealTerminal::Instance().LoadHdrFtr();
		tbzedHeaderText->Lines = TGlobalSettings::Instance().ZedHeader.get();
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmGeneralMaintenance::cbCombineServiceChargeAndServiceChargeTaxClick(TObject *Sender)
{
	TGlobalSettings  &gs = TGlobalSettings::Instance();
	TManagerVariable &mv = TManagerVariable::Instance();

	gs.ShowServiceChargeTaxWithServiceCharge = cbCombineServiceChargeAndServiceChargeTax->Checked;

	//Both the settings have to be mutually exclusive hence disable the other check box, if the current setting is enabled..
	cbCombineServiceChargeTaxAndSalesTax->Enabled = !cbCombineServiceChargeAndServiceChargeTax->Checked;

	int global_profile_key;
	Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);

	tr.StartTransaction();

	if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
	global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");

	mv.SetProfileBool(tr, global_profile_key, vmCombineServiceChargeTaxWithServiceCharge, gs.ShowServiceChargeTaxWithServiceCharge);

	tr.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbCombineServiceChargeTaxAndSalesTaxClick(TObject *Sender)
{
	TGlobalSettings  &gs = TGlobalSettings::Instance();
	TManagerVariable &mv = TManagerVariable::Instance();

	gs.ShowServiceChargeTaxWithSalesTax = cbCombineServiceChargeTaxAndSalesTax->Checked;

	//Both the settings have to be mutually exclusive hence disable the other check box, if the current setting is enabled..
	cbCombineServiceChargeAndServiceChargeTax->Enabled = !cbCombineServiceChargeTaxAndSalesTax->Checked;

	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();

	int global_profile_key;
	if (!(global_profile_key = mv.GetProfile(DBTransaction, eSystemProfiles, "Globals")))
	global_profile_key = mv.SetProfile(DBTransaction, eSystemProfiles, "Globals");

	mv.SetProfileBool(DBTransaction, global_profile_key, vmCombineServiceChargeTaxWithSalesTax, gs.ShowServiceChargeTaxWithSalesTax);
	DBTransaction.Commit();
}

//.............................................................................

void TfrmGeneralMaintenance::ClearAllParkedSales( Database::TDBTransaction &DBTransaction )
{
	std::auto_ptr<TManagerParkedSales> parkedSales( new TManagerParkedSales );

	Database::TDBTransaction tr( TDeviceRealTerminal::Instance().DBControl );

	tr.StartTransaction();
	if( parkedSales->GetCount( tr ) > 0 )
	{
		parkedSales->DeleteAll( tr );
	}
	tr.Commit();

}
//.............................................................................

//---------------------------------------------------------------------------
void __fastcall TfrmGeneralMaintenance::cbEnableTierLevelsClick(TObject *Sender)
{
	TGlobalSettings::Instance().UseTierLevels = cbEnableTierLevels->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmUseTierLevels,TGlobalSettings::Instance().UseTierLevels);
	DBTransaction.Commit();
	cbAllowPointPaymentByWeight->Enabled = TGlobalSettings::Instance().UseTierLevels;
	if(!TGlobalSettings::Instance().UseTierLevels)
	{
		cbAllowPointPaymentByValue->Checked = true;
		cbAllowPointPaymentByValueClick(NULL);
	}
}

void __fastcall TfrmGeneralMaintenance::tbBirthdayBufferClick(TObject *Sender)
{
	std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
	frmTouchNumpad->Caption = "Enter Days between 0 to 10.";
	frmTouchNumpad->btnOk->Visible = true;
	frmTouchNumpad->Mode = pmNumber;
	frmTouchNumpad->INTInitial = TGlobalSettings::Instance().BirthdayBuffer;
	if (frmTouchNumpad->ShowModal() == mrOk && frmTouchNumpad->INTResult >= 0
			&& frmTouchNumpad->INTResult < 11)
	{
		tbBirthdayBuffer->Caption = IntToStr(frmTouchNumpad->INTResult);
		TGlobalSettings::Instance().BirthdayBuffer = frmTouchNumpad->INTResult;
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmBirthdayBuffer,
		TGlobalSettings::Instance().BirthdayBuffer);
		DBTransaction.Commit();
	}
}

void __fastcall TfrmGeneralMaintenance::tbFirstVisitPointClick(TObject *Sender)
{
	std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
	frmTouchNumpad->Caption = "Enter Points For First Visit.";
	frmTouchNumpad->btnOk->Visible = true;
	frmTouchNumpad->Mode = pmNumber;
	frmTouchNumpad->INTInitial = TGlobalSettings::Instance().FirstVisitPoint;
	if (frmTouchNumpad->ShowModal() == mrOk)
	{
		tbFirstVisitPoint->Caption = IntToStr(frmTouchNumpad->INTResult);
		TGlobalSettings::Instance().FirstVisitPoint = frmTouchNumpad->INTResult;
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmFirstVisitPoint,
		TGlobalSettings::Instance().FirstVisitPoint);
		DBTransaction.Commit();
	}
}
//Chnge tripurary
void __fastcall TfrmGeneralMaintenance::tbMinRedemptionPointClick(
TObject *Sender)
{
	if(TGlobalSettings::Instance().UseTierLevels)
	{

		std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
		frmTouchNumpad->Caption = "Enter the Min Redemption Point.";
		frmTouchNumpad->btnOk->Visible = true;
		frmTouchNumpad->Mode = pmNumber;
		frmTouchNumpad->INTInitial = TGlobalSettings::Instance().MinRedemptionPoint;
		if (frmTouchNumpad->ShowModal() == mrOk)
		{
			tbMinRedemptionPoint->Caption = IntToStr(frmTouchNumpad->INTResult);
			TGlobalSettings::Instance().MinRedemptionPoint = frmTouchNumpad->INTResult;
			Database::TDBTransaction DBTransaction(DBControl);
			DBTransaction.StartTransaction();
			TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmMinRedemptionPoint,
			TGlobalSettings::Instance().MinRedemptionPoint);
			DBTransaction.Commit();
		}
	}
	else
	{ 	MessageBox("Please enable Tier Level setting to start using this feature.","Warning",MB_OK + MB_ICONWARNING);

	}
}

void __fastcall TfrmGeneralMaintenance::cbvmAllowMemberDetailscreenClick(TObject *Sender)
{
	TGlobalSettings::Instance().AllowMemberDetailscreen = cbvmAllowMemberDetailscreen->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmAllowMemberDetailscreen,TGlobalSettings::Instance().AllowMemberDetailscreen);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbvmMandatoryMembershipCardClick(TObject *Sender)
{
    //     MessageBox("Please enable Tier Level setting to start using this feature.");

   	TGlobalSettings::Instance().MandatoryMembershipCard = cbvmMandatoryMembershipCard->Checked;
    Database::TDBTransaction DBTransaction(DBControl);
   	DBTransaction.StartTransaction();
   	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmMandatoryMembershipCard,TGlobalSettings::Instance().MandatoryMembershipCard);
	DBTransaction.Commit();
}


void __fastcall TfrmGeneralMaintenance::cbAllowPointPaymentByWeightClick(TObject *Sender)
{
	if(TGlobalSettings::Instance().UseTierLevels && !TGlobalSettings::Instance().AllowPointPaymentByValue
			&& !cbAllowPointPaymentByWeight->Checked)
	{
		MessageBox("This setting is mandatory to be able to redeem or spend points.", "Warning.", MB_OK + MB_ICONINFORMATION);
		cbAllowPointPaymentByWeight->Checked = true;
	}
	TGlobalSettings::Instance().AllowPointPaymentByWeight = cbAllowPointPaymentByWeight->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmAllowPointPaymentByWeight,
	TGlobalSettings::Instance().AllowPointPaymentByWeight);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbAllowPointPaymentByValueClick(TObject *Sender)
{
	if((TGlobalSettings::Instance().UseTierLevels && !TGlobalSettings::Instance().AllowPointPaymentByWeight
				&& !cbAllowPointPaymentByValue->Checked) || (!TGlobalSettings::Instance().UseTierLevels && !cbAllowPointPaymentByValue->Checked))
	{
		MessageBox("This setting is mandatory to be able to redeem or spend points.", "Warning.", MB_OK + MB_ICONINFORMATION);
		cbAllowPointPaymentByValue->Checked = true;
	}
	TGlobalSettings::Instance().AllowPointPaymentByValue = cbAllowPointPaymentByValue->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmAllowPointPaymentByValue,
	TGlobalSettings::Instance().AllowPointPaymentByValue);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbUpdateTableGUIOnOrderStatusClick(TObject *Sender)
{
	TGlobalSettings::Instance().UpdateTableGUIOnOrderStatus = cbUpdateTableGUIOnOrderStatus->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmUpdateTableGUIOnOrderStatus,
	TGlobalSettings::Instance().UpdateTableGUIOnOrderStatus);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbRefundPointsClick(TObject *Sender)
{
	TGlobalSettings::Instance().AllowRefundPoints = cbRefundPoints->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmAllowRefundPoints,TGlobalSettings::Instance().AllowRefundPoints);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbShowCurrencySymbolClick(TObject *Sender)
{
	TGlobalSettings::Instance().ShowCurrencySymbol = cbShowCurrencySymbol->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
        TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->NameOnPoleDisplay = eMNPDNone;
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmShowCurrencySymbol, TGlobalSettings::Instance().ShowCurrencySymbol);
        TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmMemberNameOnPoleDisplay,TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->NameOnPoleDisplay); // MM-2827
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbSkipConfirmationOnFastTenderClick(TObject *Sender)
{
	TGlobalSettings::Instance().SkipConfirmationOnFastTender = cbSkipConfirmationOnFastTender->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmSkipConfirmationOnFastTender, TGlobalSettings::Instance().SkipConfirmationOnFastTender);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbEmailZedClosingTillClick(TObject *Sender)
{
   TGlobalSettings::Instance().EmailZedReports = cbEmailZedClosingTill->Checked;
   Database::TDBTransaction DBTransaction(DBControl);
   DBTransaction.StartTransaction();

   TManagerVariable::Instance().SetProfileBool(
   DBTransaction,
   TManagerVariable::Instance().DeviceProfileKey ,
   vmEmailZedReport,
   TGlobalSettings::Instance().EmailZedReports);
   DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::tbtnConfigureEmailClick(TObject *Sender)
{
        UnicodeString email_ids;

        std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
        frmTouchKeyboard->MaxLength = 300;
        frmTouchKeyboard->AllowCarriageReturn = false;
        frmTouchKeyboard->StartWithShiftDown = true;
        frmTouchKeyboard->MustHaveValue = true;
        frmTouchKeyboard->KeyboardText = TGlobalSettings::Instance().SaveEmailId;
        frmTouchKeyboard->Caption = "Please enter recipients email id seperated by semi colon";
        frmTouchKeyboard->lbeCaption->Font->Size = 18;
        if (frmTouchKeyboard->ShowModal() == mrOk)
        {
           email_ids = frmTouchKeyboard->KeyboardText;

           ValidateEmailId(email_ids);

           while (invalid_email_id && MessageBox("Entered email address is invalid.", "Invalid Email", MB_RETRYCANCEL) == IDRETRY)
           {
             if (frmTouchKeyboard->ShowModal() == mrOk)
             {
                invalid_email_id = false;
                email_ids = frmTouchKeyboard->KeyboardText;
             }
             else
             {
                break;
             }
            ValidateEmailId(email_ids);
          }
        }
        if(!invalid_email_id)
        {
          TGlobalSettings::Instance().SaveEmailId = frmTouchKeyboard->KeyboardText;

          Database::TDBTransaction DBTransaction(DBControl);
          DBTransaction.StartTransaction();
          TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmSaveEmailId,TGlobalSettings::Instance().SaveEmailId);
          DBTransaction.Commit();
        }
}

void __fastcall TfrmGeneralMaintenance::ValidateEmailId(UnicodeString emailidslist)
{
    TMMContactInfo SmartCardContact;

    bool emailValid = SmartCardContact.ValidEmail();

    TStringList *emails = new TStringList;
    AnsiString TrimmedLine, list_of_emails;
    list_of_emails =  emailidslist;
    const char *Line = list_of_emails.c_str();

    for(int i=0; i < list_of_emails.Length() ; i++)
    {
        if(Line[i] == semi_colon)
        {
            SmartCardContact.EMail = TrimmedLine;
            emailValid = SmartCardContact.ValidEmail();
            if(emailValid)
            {

              emails->Add(TrimmedLine);

            }
            else
            {
                invalid_email_id = true;
            }
            TrimmedLine = "";
        }
        else if(i == list_of_emails.Length() - 1)
        {
            TrimmedLine += Line[i];

            SmartCardContact.EMail = TrimmedLine;
            emailValid = SmartCardContact.ValidEmail();
            if(emailValid)
            {

              emails->Add(TrimmedLine);
            }

            else
            {
                invalid_email_id = true;
            }

        }
        else
        {
            TrimmedLine += Line[i];
        }
    }

}
//-----------------------------------------------------------------------------------------------------
void __fastcall TfrmGeneralMaintenance::cbRoundOnBillingClick(TObject *Sender)
{
	TGlobalSettings::Instance().RoundOnBilling = cbRoundOnBilling->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmRoundOnBilling, TGlobalSettings::Instance().RoundOnBilling);
	DBTransaction.Commit();
}
//-----------------------------------------------------------------------------------------------------
void __fastcall TfrmGeneralMaintenance::tbRoundingOnBillingClick(TObject *Sender)
{
	std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
	frmTouchNumpad->Caption = "Enter Rounding Amount";
	frmTouchNumpad->btnSurcharge->Caption = "Ok";
	frmTouchNumpad->btnDiscount->Visible = false;
	frmTouchNumpad->btnSurcharge->Visible = true;
	frmTouchNumpad->Mode = pmCurrency;
	frmTouchNumpad->CURInitial = TGlobalSettings::Instance().RoundOnBillingAmount;
	if (frmTouchNumpad->ShowModal() == mrOk)
	{
		TGlobalSettings::Instance().RoundOnBillingAmount = frmTouchNumpad->CURResult;
		tbRoundingOnBilling->Caption = FormatFloat("0.00",TGlobalSettings::Instance().RoundOnBillingAmount);
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceNum(DBTransaction,vmRoundOnBillingAmount,TGlobalSettings::Instance().RoundOnBillingAmount);
		DBTransaction.Commit();
	}
}
//MM-4579
void __fastcall TfrmGeneralMaintenance::cbResetMemberPointPurchasedClick(TObject *Sender)
{
       TGlobalSettings::Instance().PointPurchased = cbResetPointBalancedPointPurchased->Checked;
        if(cbResetPointBalancedPointPurchased->Checked)
        {
            edDay->Enabled = true;
            cbConfirmResetPoint->Enabled = true; //MM 5610
        }
        else if(!cbResetPointBalancedPointPurchased->Checked && !cbResetPointBalancedPointRedeemed->Checked && !cbResetPointBalancedPointEarned->Checked)
        {
            edDay->Enabled = false;
            cbConfirmResetPoint->Enabled = false; //MM 5610
            cbConfirmResetPoint->Checked = false; //MM 5610
            TGlobalSettings::Instance().ConfirmResetPoint = cbConfirmResetPoint->Checked; //MM5610
        }
        Database::TDBTransaction DBTransaction(DBControl);
    	DBTransaction.StartTransaction();
        TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmConfirmResetPoint,TGlobalSettings::Instance().ConfirmResetPoint); //MM5610
	    TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmPointPurchased,TGlobalSettings::Instance().PointPurchased);
    	DBTransaction.Commit();
}

//MM-4579
void __fastcall TfrmGeneralMaintenance::cbResetMemberPointRedeemedClick(TObject *Sender)
{
        TGlobalSettings::Instance().PointRedeem = cbResetPointBalancedPointRedeemed->Checked;
        if(cbResetPointBalancedPointRedeemed->Checked)
        {
            edDay->Enabled = true;
            cbConfirmResetPoint->Enabled = true; //MM 5610
        }
        else if(!cbResetPointBalancedPointPurchased->Checked && !cbResetPointBalancedPointRedeemed->Checked && !cbResetPointBalancedPointEarned->Checked)
        {
            edDay->Enabled = false;
            cbConfirmResetPoint->Enabled = false; //MM 5610
            cbConfirmResetPoint->Checked = false; //MM 5610
            TGlobalSettings::Instance().ConfirmResetPoint = cbConfirmResetPoint->Checked; //MM5610
        }

    Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
        TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmConfirmResetPoint,TGlobalSettings::Instance().ConfirmResetPoint); //MM5610
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmPointRedeemed,TGlobalSettings::Instance().PointRedeem);
	DBTransaction.Commit();
}

//MM-4579
void __fastcall TfrmGeneralMaintenance::cbResetMemberPointEarnedClick(TObject *Sender)
{
      TGlobalSettings::Instance().PointEarned = cbResetPointBalancedPointEarned->Checked;
        if(cbResetPointBalancedPointEarned->Checked)
        {
            edDay->Enabled = true;
            cbConfirmResetPoint->Enabled = true; //MM 5610
        }
        else if(!cbResetPointBalancedPointPurchased->Checked && !cbResetPointBalancedPointRedeemed->Checked && !cbResetPointBalancedPointEarned->Checked)
        {
            edDay->Enabled = false;
            cbConfirmResetPoint->Enabled = false; //MM 5610
            cbConfirmResetPoint->Checked = false; //MM 5610
            TGlobalSettings::Instance().ConfirmResetPoint = cbConfirmResetPoint->Checked; //MM5610


        }
    Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
    TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmConfirmResetPoint,TGlobalSettings::Instance().ConfirmResetPoint); //MM5610
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmPointEarned,TGlobalSettings::Instance().PointEarned);
	DBTransaction.Commit();
}
//MM 4579
void _fastcall TfrmGeneralMaintenance::edDayClick(TObject *Sender)
{
    std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
	frmTouchNumpad->Caption = "Enter number of days before points reset";
	frmTouchNumpad->btnSurcharge->Caption = "Ok";
	frmTouchNumpad->btnDiscount->Visible = false;
	frmTouchNumpad->btnSurcharge->Visible = true;
	frmTouchNumpad->Mode = pmNumber;
	if (frmTouchNumpad->ShowModal() == mrOk)
	{
        //MM 5610
        if(frmTouchNumpad->INTResult < 0)
        {
            MessageBox("Please input number of days", "Error", MB_OK + MB_ICONERROR);
        }
        else
        {
    		edDay->Text = frmTouchNumpad->INTResult;
	    	TGlobalSettings::Instance().ResetDay = frmTouchNumpad->INTResult;
		    Database::TDBTransaction DBTransaction(DBControl);
    		DBTransaction.StartTransaction();
	    	TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmResetDay, frmTouchNumpad->INTResult);
		    DBTransaction.Commit();
        }
	}
}
void __fastcall TfrmGeneralMaintenance::cbTransferTableOnPrintPrelimClick(TObject *Sender)
{
    if(cbTransferTableOnPrintPrelim->Checked)
    {
       if (MessageBox("This activates Delayed Payments. At the time of printing a prelim docket "
       "from a table, a receipt number will be allocated, the transaction will not be settled. "
       "The prelim can go to the customer as a receipt and transaction settled later."
       "\n\nDo you wish to continue ?", "Confirmation", MB_ICONWARNING + MB_OKCANCEL) != ID_OK)
       {
         cbTransferTableOnPrintPrelim->OnClick = NULL;
         cbTransferTableOnPrintPrelim->Checked = false;
         cbTransferTableOnPrintPrelim->OnClick = cbTransferTableOnPrintPrelimClick;
       }
    }
	TGlobalSettings::Instance().TransferTableOnPrintPrelim = cbTransferTableOnPrintPrelim->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmTransferTableOnPrintPrelim, TGlobalSettings::Instance().TransferTableOnPrintPrelim);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbShowRemovalReportClick(TObject *Sender)
{
    TGlobalSettings  &gs = TGlobalSettings::Instance();
	TManagerVariable &mv = TManagerVariable::Instance();

	int global_profile_key;
	Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);

	tr.StartTransaction();

	// This is used to retain the state of the checkbox if the POS is exited
#pragma warn -pia
	if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
	global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
#pragma warn .pia

	mv.SetProfileBool(
	tr,
	global_profile_key,
	vmShowRemovalReport,
	gs.ShowRemovalReport = cbShowRemovalReport->Checked);

	tr.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbEnableCancelCheckRemovalClick(TObject *Sender)
{
  	TGlobalSettings::Instance().EnableCancelCheckRemoval = cbEnableCancelCheckRemoval->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmEnableCancelCheckRemoval, TGlobalSettings::Instance().EnableCancelCheckRemoval);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbRevenueFiguresAreDiscountInclusiveClick(TObject *Sender)
{
	TGlobalSettings::Instance().RevenueFiguresAreDiscountInclusive = cbRevenueFiguresAreDiscountInclusive->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmRevenueFiguresAreDiscountInclusive,TGlobalSettings::Instance().RevenueFiguresAreDiscountInclusive);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbShowPriceAdjustmentClick(TObject *Sender)
{
	TGlobalSettings::Instance().ShowPriceAdjustment = cbShowPriceAdjustment->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmShowPriceAdjustment,TGlobalSettings::Instance().ShowPriceAdjustment);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbEnableApprovedLoginClick(TObject *Sender)
{
    TGlobalSettings::Instance().EnableApprovedLogin = cbEnableApprovedLogin->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmEnableApprovedLogin,TGlobalSettings::Instance().EnableApprovedLogin);
	DBTransaction.Commit();
    cbLinkClockInOutApprovedLogin->Enabled = true;
    if(!cbEnableApprovedLogin->Checked)
	{
       cbLinkClockInOutApprovedLogin->Enabled = false;
       cbLinkClockInOutApprovedLogin->Checked = false;
	}
}

void __fastcall TfrmGeneralMaintenance::cbLinkClockInOutApprovedLoginClick(TObject *Sender)
{
    TGlobalSettings::Instance().ClockInOutApprovedLogin = cbLinkClockInOutApprovedLogin->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmClockInOutApprovedLogin,TGlobalSettings::Instance().ClockInOutApprovedLogin);
	DBTransaction.Commit();
}
//MM-4775
void __fastcall TfrmGeneralMaintenance::cbPointOnlyClick(TObject *Sender)
{
    TGlobalSettings::Instance().PointOnly = cbPointOnly->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
    TManagerVariable::Instance().SetDeviceBool(DBTransaction,
    vmPointOnly,TGlobalSettings::Instance().PointOnly);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbPromptPatronCountOnTableSalesClick(TObject *Sender)
{
    TGlobalSettings::Instance().PromptPatronCountOnTableSales = cbPromptPatronCountOnTableSales->Checked;
    Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();
    TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmPromptPatronCountOnTableSales,TGlobalSettings::Instance().PromptPatronCountOnTableSales);
    DBTransaction.Commit();
    if(cbPromptPatronCountOnTableSales->Checked)
    {
       cbPromptPatronCount->Checked = false;
    }
}

void _fastcall TfrmGeneralMaintenance::cbPostToXeroClick(TObject *Sender)
{
	TGlobalSettings::Instance().PostZToAccountingSystem = cbPostToXero->Checked;

	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmPostZToAccountingSystem,TGlobalSettings::Instance().PostZToAccountingSystem);

    if(!cbPostToXero->Checked)
    {
        cbPostMoneyAsPayments->OnClick=NULL;
        cbPostMoneyToGLAccounts->OnClick=NULL;

        cbPostMoneyAsPayments->Checked  = false;
        TGlobalSettings::Instance().PostMoneyAsPayment = cbPostMoneyAsPayments->Checked;
       	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmPostMoneyAsPayment,TGlobalSettings::Instance().PostMoneyAsPayment);

        cbPostMoneyToGLAccounts->Checked = false;
        TGlobalSettings::Instance().PostMoneyToGlAccounts= cbPostMoneyToGLAccounts->Checked;
        TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmPostMoneyToGlAccounts,TGlobalSettings::Instance().PostMoneyToGlAccounts);

        cbPostMoneyAsPayments->OnClick=cbPostMoneyAsPaymentsClick;
        cbPostMoneyToGLAccounts->OnClick=cbPostMoneyToGLAccountsClick;

    }
    cbPostMoneyToGLAccounts->Enabled = TGlobalSettings::Instance().PostZToAccountingSystem && TGlobalSettings::Instance().IsXeroEnabled;
    cbPostMoneyAsPayments->Enabled =  TGlobalSettings::Instance().PostZToAccountingSystem;

    DBTransaction.Commit();
}
//MM 5610
void _fastcall TfrmGeneralMaintenance::cbConfirmResetPointClick(TObject *Sender)
{
    TGlobalSettings::Instance().ConfirmResetPoint = cbConfirmResetPoint->Checked;
    Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmConfirmResetPoint,TGlobalSettings::Instance().ConfirmResetPoint);
	DBTransaction.Commit();
}

void _fastcall TfrmGeneralMaintenance::cbGiftCardOnlyClick(TObject *Sender)
{
    TGlobalSettings::Instance().GiftCardOnly = cbGiftCardOnly->Checked;
    Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmGiftCardOnly,TGlobalSettings::Instance().GiftCardOnly);
	DBTransaction.Commit();
}

void _fastcall TfrmGeneralMaintenance::cbIntegratedEftposSmartPayClick(TObject *Sender)
{
	TGlobalSettings::Instance().EnableEftPosSmartPay = cbIntegratedEftposSmartPay->Checked;
    CustomizeCloudEFTPOS();
}

void _fastcall TfrmGeneralMaintenance::tbtnSmartLinkIpClick(TObject *Sender)
{
    std::auto_ptr<TfrmEFTPOSConfig>(frmEFTPosConfig)(TfrmEFTPOSConfig::Create<TfrmEFTPOSConfig>(this));
    frmEFTPosConfig->Left = ((Screen->Width/2) - (frmEFTPosConfig->Width/2));
    frmEFTPosConfig->Top = ((Screen->Height/2) - (frmEFTPosConfig->Height/2));
    frmEFTPosConfig->ShowModal();
}


void _fastcall TfrmGeneralMaintenance::cbDeleteItemSizeAfterSaleClick(TObject *Sender)
{
    Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();
    TGlobalSettings::Instance().DeleteItemSizeAfterSale = cbDeleteItemSizeAfterSale->Checked;
    TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmDeleteItemSizeAfterSale,TGlobalSettings::Instance().DeleteItemSizeAfterSale);
    DBTransaction.Commit();
}

//------------------------------------------------------------------------------------------
void __fastcall TfrmGeneralMaintenance::cbDisplayItemSizesAsListClick(TObject *Sender)
{
 	TGlobalSettings::Instance().ShowItemSizesAsList = cbDisplayItemSizesAsList->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmShowItemSizesAsList, TGlobalSettings::Instance().ShowItemSizesAsList);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbShowItemPricesInTransferScreenClick(TObject *Sender)
{
 	TGlobalSettings::Instance().ShowItemPriceInTransferScreen = cbShowItemPricesInTransferScreen->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmShowItemPriceInTransferScreen, TGlobalSettings::Instance().ShowItemPriceInTransferScreen);
	DBTransaction.Commit();
}
//------------------------------------------------------------------------------------------
void __fastcall TfrmGeneralMaintenance::cbTerminalExemptFromHappyHourClick(TObject *Sender)
{
 	TGlobalSettings::Instance().TerminalExemptFromHappyHour = cbTerminalExemptFromHappyHour->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmTerminalExemptFromHappyHour, TGlobalSettings::Instance().TerminalExemptFromHappyHour);
	DBTransaction.Commit();
}
///------------------------------------------------------------------------------------------------------
void _fastcall TfrmGeneralMaintenance::cbPostMoneyAsPaymentsClick(TObject *Sender)
{
 	TGlobalSettings::Instance().PostMoneyAsPayment = cbPostMoneyAsPayments->Checked;

	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmPostMoneyAsPayment,TGlobalSettings::Instance().PostMoneyAsPayment);

    if(cbPostMoneyAsPayments->Checked)
    {
        cbPostMoneyToGLAccounts->OnClick=NULL;
        cbPostMoneyToGLAccounts->Checked = false;
        TGlobalSettings::Instance().PostMoneyToGlAccounts =cbPostMoneyToGLAccounts->Checked;
        TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmPostMoneyToGlAccounts,TGlobalSettings::Instance().PostMoneyToGlAccounts);

        cbPostMoneyToGLAccounts->OnClick=cbPostMoneyToGLAccountsClick;
    }
    DBTransaction.Commit();
}
//------------------------------------------------------------------------------------------------------
void _fastcall TfrmGeneralMaintenance::cbPostMoneyToGLAccountsClick(TObject *Sender)
{
  	TGlobalSettings::Instance().PostMoneyToGlAccounts = cbPostMoneyToGLAccounts->Checked;

	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmPostMoneyToGlAccounts,TGlobalSettings::Instance().PostMoneyToGlAccounts);

    if(cbPostMoneyToGLAccounts->Checked)
    {
        cbPostMoneyAsPayments->OnClick=NULL;
        cbPostMoneyAsPayments->Checked = false;
        TGlobalSettings::Instance().PostMoneyAsPayment =cbPostMoneyAsPayments->Checked;
        TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmPostMoneyAsPayment,TGlobalSettings::Instance().PostMoneyAsPayment);
        cbPostMoneyAsPayments->OnClick=cbPostMoneyAsPaymentsClick;
    }
    DBTransaction.Commit();
}

//MM-7935
void __fastcall TfrmGeneralMaintenance::cbUseNMIStandardFontSizeClick(TObject *Sender)
{
 	TGlobalSettings::Instance().SetTextFontSizeOnCustomerDisplay = cbUseNMIStandardFontSize->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmSetTextFontSize,TGlobalSettings::Instance().SetTextFontSizeOnCustomerDisplay);
	DBTransaction.Commit();
}

//--------------------------------------------------------//
void __fastcall TfrmGeneralMaintenance::tbEnterWeightClick(TObject *Sender)
{
	std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
	frmTouchNumpad->Caption = "Enter Weight Limit";
    frmTouchNumpad->btnOk->Caption = "Ok";
	frmTouchNumpad->btnDiscount->Visible = false;
	frmTouchNumpad->btnOk->Visible = true;
    frmTouchNumpad->btnCancel->Visible = true;
	frmTouchNumpad->Mode = pmWeight;
	frmTouchNumpad->CURInitial = TGlobalSettings::Instance().WeightLimit;
	if (frmTouchNumpad->ShowModal() == mrOk)
	{
		TGlobalSettings::Instance().WeightLimit = frmTouchNumpad->CURResult;
        DisplayTextOnWeightLimit();
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceNum(DBTransaction,vmWeightLimit,TGlobalSettings::Instance().WeightLimit);
		DBTransaction.Commit();
	}
}
//----------------------------------------------------------------------------------------------
void TfrmGeneralMaintenance::DisplayTextOnWeightLimit()
{
     if(TGlobalSettings::Instance().WeightLimit == 0.000)
    {
        tbEnterWeight->Caption = "";
    }
    else
    {
        tbEnterWeight->Caption = FormatFloat("0.000",TGlobalSettings::Instance().WeightLimit);
    }
}
//----------------------------------------------------------------------------------------------
void __fastcall TfrmGeneralMaintenance::cbNotifyForManuallyEnteredWeightClicked(TObject *Sender)
{
 	TGlobalSettings::Instance().NotifyForManuallyEnteredWeight = cbNotifyForManuallyEnteredWeight->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmNotifyForManuallyEnteredWeight, TGlobalSettings::Instance().NotifyForManuallyEnteredWeight);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbShowScreenToSelectItemsClick(TObject *Sender)
{
    TGlobalSettings::Instance().ShowScreenToSelectItemForPoint = cbShowScreenToSelectItems->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmShowScreenToSelectItemForPoint, TGlobalSettings::Instance().ShowScreenToSelectItemForPoint);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbCountPointSpentInRevenueClick(TObject *Sender)
{
    TGlobalSettings::Instance().PontsSpentCountedAsRevenue = cbCountPointSpentInRevenue->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmPontsSpentCountedAsRevenue, TGlobalSettings::Instance().PontsSpentCountedAsRevenue);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbShowLargeFontsClick(TObject *Sender)
{
    TGlobalSettings::Instance().ShowLargeFonts = cbShowLargeFonts->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmShowLargeFonts, TGlobalSettings::Instance().ShowLargeFonts);
	DBTransaction.Commit();
}
//------------------------------------------------------------------------------------------------------
void __fastcall TfrmGeneralMaintenance::cbItemSearchClick(TObject *Sender)
{
    TGlobalSettings::Instance().ItemSearch = cbItemSearch->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmItemSearch, TGlobalSettings::Instance().ItemSearch);
	DBTransaction.Commit();
}
//------------------------------------------------------------------------------------------------------
void __fastcall TfrmGeneralMaintenance::cbShowDarkBackgroundInPOSClick(TObject *Sender)
{
    TGlobalSettings::Instance().ShowDarkBackground = cbShowDarkBackgroundInPOS->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmShowDarkBackgroundInPOS, TGlobalSettings::Instance().ShowDarkBackground);
	DBTransaction.Commit();
}
//------------------------------------------------------------------------------------------------------
void __fastcall TfrmGeneralMaintenance::cbOpenCashDrawerClick(TObject *Sender)
{
    TGlobalSettings::Instance().OpenCashDrawer = cbOpenCashDrawer->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmOpenCashDrawer, TGlobalSettings::Instance().OpenCashDrawer);
	DBTransaction.Commit();
}
//----------------------------------------------------------------------------------------------------------------------------------
void __fastcall TfrmGeneralMaintenance::cbMergeSimilarItemClick(TObject *Sender)
{
    TGlobalSettings::Instance().MergeSimilarItem = cbMergeSimilarItem->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmMergeSimilarItem, TGlobalSettings::Instance().MergeSimilarItem);
	DBTransaction.Commit();
}
//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
void __fastcall TfrmGeneralMaintenance::cbUseBIRFormatInXZReportClick(TObject *Sender)
{
    isBIRSettingTicked = true;
    TGlobalSettings::Instance().UseBIRFormatInXZReport = cbUseBIRFormatInXZReport->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmUseBIRFormatInXZReport, TGlobalSettings::Instance().UseBIRFormatInXZReport);
	DBTransaction.Commit();
    if(cbUseBIRFormatInXZReport->Checked)
    {
        cbShowDiscountReport->Checked = false;
        cbShowTransactionSummaryGroups->Checked = false;
        cbShowTransactionSummaryGroups->Enabled = false;
        cbShowTaxSummary->Enabled = false;
        cbShowDiscountReport->Enabled = false;
        cbEnableHideCredCancels->Enabled = false;
        cbShowSessionDate->Checked = false;
        cbShowSessionDate->Enabled = false;
        cbEnableHideCredCancels->Checked = false;
        cbShowAccumulatedTotal->Checked = false;
        cbShowAccumulatedTotal->Enabled = false;
        cbShowTaxSummary->Checked = false;
        EnableDisableSectionsForBIRFormat();
    }
    else
    {
       cbShowTransactionSummaryGroups->Enabled = true;
       cbShowTaxSummary->Enabled = true;
       cbShowDiscountReport->Enabled = true;
       cbEnableHideCredCancels->Enabled = true;
       cbShowSessionDate->Enabled = true;
       cbShowAccumulatedTotal->Enabled = true;
       EnableDisableSectionsForBIRFormat();
    }
}
void __fastcall TfrmGeneralMaintenance::cbExcludeReceiptClick(TObject *Sender)
{
    TGlobalSettings::Instance().ExcludeReceipt = cbExcludeReceipt->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmExcludeReceipt, TGlobalSettings::Instance().ExcludeReceipt);
	DBTransaction.Commit();
}
//----------------------------------------------------------------------------------------------------------------------------------
void __fastcall TfrmGeneralMaintenance::cbExcludeXReportClick(TObject *Sender)
{
    TGlobalSettings::Instance().ExcludeXReport = cbExcludeXReport->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmExcludeXReport, TGlobalSettings::Instance().ExcludeXReport);
	DBTransaction.Commit();
}
//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
void TfrmGeneralMaintenance::EnableDisableSectionsForBIRFormat()
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	int global_profile_key;
	// This is used to retain the state of the checkbox if the POS is exited
#pragma warn -pia
	if (!(global_profile_key = TManagerVariable::Instance().GetProfile(DBTransaction, eSystemProfiles, "Globals")))
	global_profile_key = TManagerVariable::Instance().SetProfile(DBTransaction, eSystemProfiles, "Globals");
#pragma warn .pia
	TManagerVariable::Instance().SetProfileBool(DBTransaction,global_profile_key,vmShowTaxSummary, TGlobalSettings::Instance().ShowTaxSummary = cbUseBIRFormatInXZReport->Checked);
	TManagerVariable::Instance().SetProfileBool(DBTransaction,global_profile_key,vmShowTransactionSummaryGroups, TGlobalSettings::Instance().ShowTransactionSummaryGroups = cbUseBIRFormatInXZReport->Checked);
    TManagerVariable::Instance().SetProfileBool(DBTransaction,global_profile_key,vmShowDiscountReport, TGlobalSettings::Instance().ShowDiscountReport = cbUseBIRFormatInXZReport->Checked);
    TManagerVariable::Instance().SetProfileBool(DBTransaction,global_profile_key,vmShowAccumulatedZed, TGlobalSettings::Instance().ShowAccumulatedZeds = cbUseBIRFormatInXZReport->Checked);
    DBTransaction.Commit();
}
void TfrmGeneralMaintenance::CheckSettingsOfZed()
{
    if(cbUseBIRFormatInXZReport->Checked)
    {
        cbShowTransactionSummaryGroups->Enabled = false;
        cbDEnableBlindBalances->Checked = false;
        cbShowTransactionSummaryGroups->Checked = false;
        cbShowBilledSalesTotals->Checked = false;
        cbShowComplimentarySalesTotals->Checked = false;
        cbShowChargedSalesTotals->Checked = false;
        cbShowTotals->Checked = false;
        cbShowBreakdownCategories->Checked = false;
        cbShowRemovalReport->Checked = false;
        cbShowPointsReport->Checked = false;
        cbShowPatronAverages->Checked = false;
        cbShowProductionInfo->Checked = false;
        cbShowAccountPurchases->Checked = false;
        cbShowAccountBalancesTabs->Checked = false;
        cbShowAccountBalances->Checked = false;
        cbShowHourlySales->Checked = false;
        cbShowServiceChargeSummary->Checked = false;
        cbEnableStaffHours->Checked = false;
        cbEnableCommission->Checked = false;
        cbRevenueFiguresAreTaxAndServiceChargeInclusive->Checked = false;
        cbPointsOnZed->Checked = false;
        cbShowInvoiceInfo->Checked = false;
        cbRevenueFiguresAreDiscountInclusive->Checked = false;
        cbShowPriceAdjustment->Checked = false;

        cbShowDiscountReport->Checked = false;
        cbShowTransactionSummaryGroups->Checked = false;
        cbShowTransactionSummaryGroups->Enabled = false;
        cbShowTaxSummary->Enabled = false;
        cbShowDiscountReport->Enabled = false;
        cbEnableHideCredCancels->Enabled = false;
        cbShowSessionDate->Checked = false;
        cbShowSessionDate->Enabled = false;
        cbEnableHideCredCancels->Checked = false;
        cbShowAccumulatedTotal->Checked = false;
        cbShowAccumulatedTotal->Enabled = false;
        cbShowTaxSummary->Checked = false;
        EnableDisableSectionsForBIRFormat();
    }
    else
    {
       cbShowTransactionSummaryGroups->Enabled = true;
       cbShowTaxSummary->Enabled = true;
       cbShowDiscountReport->Enabled = true;
       cbEnableHideCredCancels->Enabled = true;
       cbShowSessionDate->Enabled = true;
       cbShowAccumulatedTotal->Enabled = true;
       EnableDisableSectionsForBIRFormat();
    }
}
void __fastcall TfrmGeneralMaintenance::cbHideRoundingOnReceiptClick(TObject *Sender)
{
    TGlobalSettings::Instance().HideRoundingOnReceipt = cbHideRoundingOnReceipt->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmHideRoundingOnReceipt, TGlobalSettings::Instance().HideRoundingOnReceipt);
	DBTransaction.Commit();
}

void __fastcall TfrmGeneralMaintenance::cbUseBIRFormatInXZReportMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
  int X, int Y)
  {
     CheckSettingsOfZed();
  }

void __fastcall TfrmGeneralMaintenance::cbCashDenominationEntryClick(TObject *Sender)
{
    TGlobalSettings::Instance().CashDenominationEntry = cbCashDenominationEntry->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmCashDenominationEntry, TGlobalSettings::Instance().CashDenominationEntry);
	DBTransaction.Commit();
}

//----------------------------------------------------------------------------
bool TfrmGeneralMaintenance::IsEligibleForTrue()
{
    UnicodeString variable_key = "2001, 7038";

    bool retVal = false;
    Database::TDBTransaction DBTransaction(DBControl);
    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
    DBTransaction.RegisterQuery(IBInternalQuery);
    DBTransaction.StartTransaction();
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text = " SELECT a.INTEGER_VAL, a.Profile_key, a.VARIABLES_KEY FROM VARSPROFILE a where a.VARIABLES_KEY IN (" + variable_key + ") ";
    //IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = 7038;
    IBInternalQuery->ExecQuery();
    for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
    {
         if(IBInternalQuery->FieldByName("VARIABLES_KEY")->AsInteger == 2001)
         {
             if(IBInternalQuery->FieldByName("INTEGER_VAL")->AsInteger > 0)
             {
                retVal = true;
                break;
             }
         }
         if(IBInternalQuery->FieldByName("VARIABLES_KEY")->AsInteger == 7038)
         {
             if(IBInternalQuery->FieldByName("INTEGER_VAL")->AsInteger > 0)
             {
                retVal = true;
                break;
             }
         }
    }
    DBTransaction.Commit();
    return retVal;
}
//----------------------------------------------------------------------------
void __fastcall TfrmGeneralMaintenance::cbUseMemberSubsClick(TObject *Sender)
{
    if(!IsEligibleForTrue())
    {
        TManagerVariable &mv = TManagerVariable::Instance();

        int pk;
        Database::TDBTransaction tr(DBControl);

        tr.StartTransaction();
        if (!(pk = mv.GetProfile(tr, eSystemProfiles, "Globals")))
        pk = mv.SetProfile(tr, eSystemProfiles, "Globals");
        tr.Commit();

        TGlobalSettings::Instance().UseMemberSubs =
        cbUseMemberSubs->Checked;

        tr.StartTransaction();
        mv.SetProfileBool(tr, pk, vmUseMemberSubs,
        cbUseMemberSubs->Checked);
        tr.Commit();
        MessageBox("All terminals need to be restarted for this selection to work properly","Information", MB_OK + MB_ICONINFORMATION);
    }
    else
    {
       if(cbUseMemberSubs->Checked)
       {
           MessageBox("Functionality works with Menumate Loyalty only, Please disable any other Membership first to use this functionality","Information", MB_OK + MB_ICONINFORMATION);
           cbUseMemberSubs->Checked = false;
       }
    }
}//--------------------------------------------------------------------------------------------------------------void __fastcall TfrmGeneralMaintenance::cbFloatWithdrawFromCashClick(TObject *Sender){
    TGlobalSettings::Instance().FloatWithdrawFromCash = cbFloatWithdrawFromCash->Checked;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmFloatWithdrawFromCash, TGlobalSettings::Instance().FloatWithdrawFromCash);
	DBTransaction.Commit();
}//-----------------------------------------------------------------------------void __fastcall TfrmGeneralMaintenance::cbShowCashDrawerCountClick(TObject *Sender){	TGlobalSettings  &gs = TGlobalSettings::Instance();	TManagerVariable &mv = TManagerVariable::Instance();

	int global_profile_key;
	Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);

	tr.StartTransaction();

	// This is used to retain the state of the checkbox if the POS is exited
#pragma warn -pia
	if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
	global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
#pragma warn .pia

	mv.SetProfileBool(
	tr,
	global_profile_key,
	vmShowCashDrawerOpeningsCount,
	gs.ShowCashDrawerOpeningsCount = cbShowCashDrawerCount->Checked);

	tr.Commit();}//-----------------------------------------------------------------------------void __fastcall TfrmGeneralMaintenance::cbShowReprintDetailsClick(TObject *Sender){	TGlobalSettings  &gs = TGlobalSettings::Instance();	TManagerVariable &mv = TManagerVariable::Instance();

	int global_profile_key;
	Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);

	tr.StartTransaction();

	// This is used to retain the state of the checkbox if the POS is exited
#pragma warn -pia
	if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
	global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
#pragma warn .pia

	mv.SetProfileBool(
	tr,
	global_profile_key,
	vmShowReprintReceiptDetails,
	gs.ShowReprintReceiptDetails = cbShowReprintDetails->Checked);

	tr.Commit();}//--------------------------------------------------------------------------------------------------------------void __fastcall TfrmGeneralMaintenance::cbSplitBillByMenuTypeClick(TObject *Sender){
    TGlobalSettings  &ref_gs = TGlobalSettings::Instance();
	TManagerVariable &ref_mv = TManagerVariable::Instance();

	int isBillSplittted;
	Database::TDBTransaction tr(DBControl);

	tr.StartTransaction();
#pragma warn -pia
	if (!(isBillSplittted = ref_mv.GetProfile(tr, eSystemProfiles, "Globals")))
	isBillSplittted = ref_mv.SetProfile(tr, eSystemProfiles, "Globals");
#pragma warn .pia
	tr.Commit();

	ref_gs.IsBillSplittedByMenuType = cbSplitBillByMenuType->Checked;

	tr.StartTransaction();
	ref_mv.SetProfileBool(tr, isBillSplittted, vmIsBillSplittedByMenuType, ref_gs.IsBillSplittedByMenuType);
	tr.Commit();
}//-------------------------------------------------------------------------------------
void __fastcall TfrmGeneralMaintenance::cbIntegratedEftposSmartConnectClick(TObject *Sender)
{
    TGlobalSettings::Instance().EnableEftPosSmartConnect = cbIntegratedEftposSmartConnect->Checked;
    CustomizeCloudEFTPOS();
    MessageBox("MenuMate Restart Required.", "Info", MB_OK + MB_ICONINFORMATION);
}

//----------------------------------------------------------------------------
void _fastcall TfrmGeneralMaintenance::cbIntegratedEftposAdyenClick(TObject *Sender)
{
    TGlobalSettings::Instance().EnableEftPosAdyen = cbIntegratedEftposAdyen->Checked;
    if(TGlobalSettings::Instance().EnableEftPosAdyen)
        MessageBox("Please provide Adyen Details by using button below.","Info",MB_OK + MB_ICONINFORMATION);
    CustomizeCloudEFTPOS();
}
//----------------------------------------------------------------------------
void __fastcall TfrmGeneralMaintenance::cbCompanyDetailOnReprintReceiptClick(TObject *Sender)
{
    //
   TGlobalSettings::Instance().EnableCompanyDetailOnReprintReceipt = cbCompanyDetailOnReprintReceipt->Checked;

}
