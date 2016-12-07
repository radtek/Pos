//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <Filectrl.hpp>

#include "enum.h"
#include "Requests.h"
#include "MMdata.h"
#include "MMLogging.h"
#include "Printing.h"
#include "Comms.h"
#include "Pause.h"
#include "GetMoney.h"

USEFORM("TerminalChitConfig.cpp", frmTerminalChitConfig);
USEFORM("DealsSetup.cpp", frmDeals);
USEFORM("SelectDiscounts.cpp", frmSelectDiscounts);
USEFORM("Setup.cpp", frmSetup);
USEFORM("ProcessWebOrder.cpp", frmProcessWebOrder);
USEFORM("POS\MenuManager.cpp", frmMenuManager);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMColourPicker.cpp", frmColourPicker);
USEFORM("POS\PosMain.cpp", frmPOSMain);
USEFORM("DiscountTimes.cpp", frmDiscountTimes);
USEFORM("POS\TransferItems.cpp", frmTransferItem);
USEFORM("TierLevels.cpp", frmTierLevel);
USEFORM("POS\SalesforceAuthInfo.cpp", frmSalesforceAuthInfo);
USEFORM("Maintenance\PaymentMaintenance.cpp", frmPaymentMaintenance);
USEFORM("POS\MenuManagerItemSizeSelection.cpp", frmItemSizeSelection);
USEFORM("POS\TipAmount.cpp", frmTipAmount);
USEFORM("DealEditor.cpp", frmDealEditorForm);
USEFORM("POS\SelectRoom.cpp", frmSelectRoom);
USEFORM("VirtualPrintersInterface.cpp", frmVirtualPrintersInterface);
USEFORM("Maintenance\HappyHourSettings.cpp", frmHappyHourSettings);
USEFORM("Main.cpp", frmMain);
USEFORM("Security\Secure.cpp", frmSecurity);
USEFORM("POS\SelectCurrentMenu.cpp", frmSelectCurrentMenu);
USEFORM("Maintenance\MessageMaintenance.cpp", frmMessageMaintenance);
USEFORM("ReportDisplay.cpp", frmReportDisplay);
USEFORM("DealLevelsEditor.cpp", Form1);
USEFORM("POS\SelectTable.cpp", frmSelectTable);
USEFORM("XeroSettings.cpp", frmXeroSettings);
USEFORM("POS\Users.cpp", frmUsers);
USEFORM("POS\AddTab.cpp", frmAddTab);
USEFORM("SystemConfig.cpp", frmSystemConfig);
USEFORM("Locations.cpp", frmLocations);
USEFORM("Security\TimeClock.cpp", frmTimeClock);
USEFORM("ChitList.cpp", frmChitList);
USEFORM("SyndCodeGui.cpp", frmSyndCodeGui);
USEFORM("MallExportRegenerateReport.cpp", frmMallExportRegenerateReport);
USEFORM("TierLevelEditor.cpp", frmTierLevelEditor);
USEFORM("Chefmate\ChefMateSettings.cpp", frmChefmateSettings);
USEFORM("DelayedPaymentTabs.cpp", frmDelayedPaymentTabs);
USEFORM("POS\SelectMember.cpp", frmSelectMember);
USEFORM("SelectSizesAsList.cpp", frmSelectSizes);
USEFORM("Timer.cpp", frmClockTime);
USEFORM("POS\MenuManagerItemSettings.cpp", frmItemSettings);
USEFORM("Maintenance\NewUser.cpp", frmNewUser);
USEFORM("POS\Message.cpp", frmMessage);
USEFORM("MallExportResendReport.cpp", frmMallExportResendReport);
USEFORM("ChitNumberGui.cpp", frmChitNumberGui);
USEFORM("Maintenance\TaxMaintenance.cpp", frmTaxMaintenance);
USEFORM("CaptNamePhone.cpp", frmCaptNamePhone);
USEFORM("SelectSaveOption.cpp", frmSelectSaveOption);
USEFORM("PSectionInstructionEdit.cpp", frmPSectionInstructionEdit);
USEFORM("Tables_ChitNumbers.cpp", frmTables_ChitNumbers);
USEFORM("POS\SelectItem.cpp", frmSelectItem);
USEFORM("Discounts.cpp", frmDiscounts);
USEFORM("MMdata.cpp", frmData);
USEFORM("POS\SelectRMSRoom.cpp", frmRMSRoom);
USEFORM("Maintenance\GeneralMaintenance.cpp", frmGeneralMaintenance);
USEFORM("POS\ForcedSides.cpp", frmSelectForcedSides);
USEFORM("AutoZReportingSetup.cpp", frmAutoZReportingSetup);
USEFORM("LoyaltyMateOperationDialogBox.cpp", frmLoyaltyMateOperationDialogBox);
USEFORM("POS\SplitPayment.cpp", frmSplitPayment);
USEFORM("DropDownVar.cpp", frmDropDownVar);
USEFORM("PHSConfiguration.cpp", frmPHSConfiguration);
USEFORM("POS\SelectKitchenDocket.cpp", frmSelectKitchenDocket);
USEFORM("POS\ShowPrintout.cpp", frmShowPrintout);
USEFORM("POS\Functions.cpp", frmFunctions);
USEFORM("PrinterRedirect.cpp", frmPrinterRedirect);
USEFORM("Comms\SerialConfig.cpp", frmSerialConfig);
USEFORM("TabManagerGUI.cpp", frmTabManager);
USEFORM("POS\GetMoney.cpp", frmControlTransaction);
USEFORM("Maintenance\EditCustomer.cpp", frmEditCustomer);
USEFORM("DiscountEditor.cpp", frmDiscountEdit);
USEFORM("EftChequeVerify.cpp", frmChequeVerfiy);
USEFORM("SelectTable2.cpp", FrmSelectTable2);
USEFORM("POS\SelectReceipt.cpp", frmSelectReceipt);
USEFORM("SmartCards\SmartcardPreloader.cpp", Smartcard_Preloader);
USEFORM("Maintenance\PrinterControlPanel.cpp", frmPrinterControlPanel);
USEFORM("Maintenance\PrinterMaintenance.cpp", frmPrinterMaintenance);
USEFORM("POS\SelectPhoenixRoom.cpp", frmPhoenixRoom);
USEFORM("DbMod.cpp", frmDBMod);
USEFORM("SmartCards\SmartCardConfigGUI.cpp", frmSmartCardConfiguration);
USEFORM("GenericGroupGUI.cpp", frmPaymentGroupGUI);
USEFORM("DeskTop\VerticalSelect.cpp", frmVerticalSelect);
USEFORM("CardSwipe.cpp", frmCardSwipe);
USEFORM("POS\SelectActiveMenus.cpp", frmSelectActiveMenus);
USEFORM("POS\PatronCount.cpp", frmPatronCount);
USEFORM("DropDown.cpp", frmDropDownFunc);
USEFORM("ParkedSales.cpp", frmParkedSales);
USEFORM("POS\GUIDiscount.cpp", frmDiscount);
USEFORM("POS\Transfer.cpp", frmTransfer);
USEFORM("Maintenance\UserMaintenance.cpp", frmUserMaintenance);
USEFORM("Maintain.cpp", frmMaintain);
USEFORM("SendForm.cpp", frmSendForm);
USEFORM("POS\SplitPayment.cpp", frmSplitPayment);
USEFORM("Processing.cpp", frmProcessing);
USEFORM("POS\ConfirmOrd.cpp", frmConfirmOrder);
USEFORM("Maintain.cpp", frmMaintain);
USEFORM("POS\SelectActiveMenus.cpp", frmSelectActiveMenus);
USEFORM("CardSwipe.cpp", frmCardSwipe);
USEFORM("ParkedSales.cpp", frmParkedSales);
USEFORM("POS\PatronCount.cpp", frmPatronCount);
USEFORM("DropDown.cpp", frmDropDownFunc);
USEFORM("IntaMateConfiguration.cpp", frmIntaMateConfiguration);
USEFORM("POS\BillGroup.cpp", frmBillGroup);
USEFORM("POS\ForcedOptions.cpp", frmSelectForcedOptions);
USEFORM("PocketVoucher.cpp", frmPocketVoucher);
USEFORM("Maintenance\UserMaintenance.cpp", frmUserMaintenance);
USEFORM("Security\Login.cpp", frmLogin);
USEFORM("POS\HoldSend.cpp", frmHoldSend);
USEFORM("Security\Login.cpp", frmLogin);
USEFORM("POS\SelectDish.cpp", frmSelectDish);
USEFORM("POS\GetMoney.cpp", frmControlTransaction);
USEFORM("TabManagerGUI.cpp", frmTabManager);
USEFORM("EftChequeVerify.cpp", frmChequeVerfiy);
USEFORM("DiscountEditor.cpp", frmDiscountEdit);
USEFORM("Maintenance\EditCustomer.cpp", frmEditCustomer);
USEFORM("Comms\SerialConfig.cpp", frmSerialConfig);
USEFORM("POS\SelectKitchenDocket.cpp", frmSelectKitchenDocket);
USEFORM("POS\ShowPrintout.cpp", frmShowPrintout);
USEFORM("DropDownVar.cpp", frmDropDownVar);
USEFORM("PHSConfiguration.cpp", frmPHSConfiguration);
USEFORM("PrinterRedirect.cpp", frmPrinterRedirect);
USEFORM("POS\Functions.cpp", frmFunctions);
USEFORM("POS\SelectPhoenixRoom.cpp", frmPhoenixRoom);
USEFORM("DbMod.cpp", frmDBMod);
USEFORM("Maintenance\PrinterMaintenance.cpp", frmPrinterMaintenance);
USEFORM("GenericGroupGUI.cpp", frmPaymentGroupGUI);
USEFORM("DeskTop\VerticalSelect.cpp", frmVerticalSelect);
USEFORM("SmartCards\SmartCardConfigGUI.cpp", frmSmartCardConfiguration);
USEFORM("POS\SelectReceipt.cpp", frmSelectReceipt);
USEFORM("SelectTable2.cpp", FrmSelectTable2);
USEFORM("Maintenance\PrinterControlPanel.cpp", frmPrinterControlPanel);
USEFORM("SmartCards\SmartcardPreloader.cpp", Smartcard_Preloader);
USEFORM("ListManager.cpp", frmListManager);
USEFORM("GenericGroupAssignment.cpp", frmGenericGroupAssignment);
USEFORM("SetButtonFontSize.cpp", frmSetButtonFontSize);
USEFORM("Casino Security Screen\TransactionAuditScreen.cpp", frmTransactionAuditScreen);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchKeyboard.cpp", frmTouchKeyboard);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMMessageBox.cpp", frmMessageBox);
USEFORM("POS\BillGroup.cpp", frmBillGroup);
USEFORM("POS\ForcedOptions.cpp", frmSelectForcedOptions);
USEFORM("IntaMateConfiguration.cpp", frmIntaMateConfiguration);
USEFORM("PocketVoucher.cpp", frmPocketVoucher);
USEFORM("POS\TransferItemOrGuest.cpp", frmTransferItemOrGuest);
USEFORM("POS\SelectZed.cpp", frmSelectZed);
USEFORM("POS\SelectPaymentTypeAndValue.cpp", frmSelectPaymentTypeAndValue);
USEFORM("POS\SelectGlass.cpp", frmSelectGlass);
USEFORM("ProductSearch\ProductSearch.cpp", frmProductSearch);
USEFORM("SelectDateAndTime.cpp", frmSelectDateTime);
USEFORM("SetupGlCodes.cpp", frmSetupGlCodes);
USEFORM("ComboForm.cpp", frmComboForm);
USEFORM("POS\TaxRemoval.cpp", frmTaxRemoval);
USEFORM("EftPosDialogs.cpp", frmEftPos);
USEFORM("POS\GroupGUI.cpp", frmGroupGUI);
USEFORM("NewPaymentType.cpp", frmNewPaymentType);
USEFORM("EftPosDialogs.cpp", frmEftPos);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchNumpad.cpp", frmTouchNumpad);
USEFORM("Analysis.cpp", frmAnalysis);
USEFORM("ListManager.cpp", frmListManager);
USEFORM("GenericGroupAssignment.cpp", frmGenericGroupAssignment);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchKeyboard.cpp", frmTouchKeyboard);
USEFORM("SetButtonFontSize.cpp", frmSetButtonFontSize);
USEFORM("Casino Security Screen\TransactionAuditScreen.cpp", frmTransactionAuditScreen);
USEFORM("POS\MemberCreation.cpp", frmMemberCreation);
USEFORM("POS\Paytype.cpp", frmPaymentType);
USEFORM("ReportDisplayNav.cpp", frmReportDisplayNav);
USEFORM("POS\Scales.cpp", frmScales);
USEFORM("ReportDisplayNav.cpp", frmReportDisplayNav);
USEFORM("Casino Security Screen\MMSecurityScreenSettings.cpp", frmMMSecurityScreenSettings);
USEFORM("POS\SelectZed.cpp", frmSelectZed);
USEFORM("POS\SelectPaymentTypeAndValue.cpp", frmSelectPaymentTypeAndValue);
USEFORM("POS\TransferItemOrGuest.cpp", frmTransferItemOrGuest);
USEFORM("POS\SelectGlass.cpp", frmSelectGlass);
USEFORM("ComboForm.cpp", frmComboForm);
USEFORM("POS\TaxRemoval.cpp", frmTaxRemoval);
USEFORM("SetupGlCodes.cpp", frmSetupGlCodes);
USEFORM("SelectDateAndTime.cpp", frmSelectDateTime);
USEFORM("ProductSearch\ProductSearch.cpp", frmProductSearch);
USEFORM("ShowEJournal.cpp", frmEJournal);
//---------------------------------------------------------------------------
HANDLE Mutex;

bool AlreadyRunning()
{
	HWND PrevWnd;

   Mutex = CreateMutex(NULL, false, _T("MenuMateUniqueName"));
   if ((Mutex != NULL) && (GetLastError() == ERROR_ALREADY_EXISTS))
	{
	   PrevWnd = FindWindow(_T("TfrmMain"), _T("MenuMate"));
	if (PrevWnd != NULL)
	{
		PrevWnd = GetWindow(PrevWnd, GW_OWNER);
   	}
   	if (PrevWnd != NULL)
   	{
   		if (IsIconic(PrevWnd))
      	{
      		ShowWindow(PrevWnd, SW_SHOWMAXIMIZED);
      	}
      	else
      	{
				SetForegroundWindow(PrevWnd);
			}
		}
		CloseHandle(Mutex);
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
//WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
        if (AlreadyRunning()) return 0;
        try
        {
            Application->Initialize();
            Application->Title = "MenuMate";

         srand(time(NULL));      //Seed random number generator for lucky member draws

		 Application->CreateForm(__classid(TfrmMain), &frmMain);
         Application->CreateForm(__classid(TfrmSelectCurrentMenu), &frmSelectCurrentMenu);
         Application->CreateForm(__classid(TfrmEJournal), &frmEJournal);
         Application->Run();
		}
		__finally
		{
			ReleaseMutex(Mutex);
			CloseHandle(Mutex);
		}
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	return 0;
}
//---------------------------------------------------------------------------










