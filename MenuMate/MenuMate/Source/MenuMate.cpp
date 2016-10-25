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
USEFORM("POS\MenuManager.cpp", frmMenuManager);
USEFORM("ProcessWebOrder.cpp", frmProcessWebOrder);
USEFORM("Setup.cpp", frmSetup);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMColourPicker.cpp", frmColourPicker);
USEFORM("POS\PosMain.cpp", frmPOSMain);
USEFORM("DiscountTimes.cpp", frmDiscountTimes);
USEFORM("POS\TransferItems.cpp", frmTransferItem);
USEFORM("TierLevels.cpp", frmTierLevel);
USEFORM("POS\SalesforceAuthInfo.cpp", frmSalesforceAuthInfo);
USEFORM("POS\MenuManagerItemSizeSelection.cpp", frmItemSizeSelection);
USEFORM("Maintenance\PaymentMaintenance.cpp", frmPaymentMaintenance);
USEFORM("POS\TipAmount.cpp", frmTipAmount);
USEFORM("DealEditor.cpp", frmDealEditorForm);
USEFORM("POS\SelectRoom.cpp", frmSelectRoom);
USEFORM("VirtualPrintersInterface.cpp", frmVirtualPrintersInterface);
USEFORM("Main.cpp", frmMain);
USEFORM("POS\SelectCurrentMenu.cpp", frmSelectCurrentMenu);
USEFORM("Maintenance\HappyHourSettings.cpp", frmHappyHourSettings);
USEFORM("Maintenance\MessageMaintenance.cpp", frmMessageMaintenance);
USEFORM("ReportDisplay.cpp", frmReportDisplay);
USEFORM("DealLevelsEditor.cpp", Form1);
USEFORM("POS\SelectTable.cpp", frmSelectTable);
USEFORM("XeroSettings.cpp", frmXeroSettings);
USEFORM("Locations.cpp", frmLocations);
USEFORM("POS\AddTab.cpp", frmAddTab);
USEFORM("POS\Users.cpp", frmUsers);
USEFORM("ChitList.cpp", frmChitList);
USEFORM("Security\TimeClock.cpp", frmTimeClock);
USEFORM("SystemConfig.cpp", frmSystemConfig);
USEFORM("TierLevelEditor.cpp", frmTierLevelEditor);
USEFORM("Chefmate\ChefMateSettings.cpp", frmChefmateSettings);
USEFORM("SyndCodeGui.cpp", frmSyndCodeGui);
USEFORM("SelectSizesAsList.cpp", frmSelectSizes);
USEFORM("MallExportRegenerateReport.cpp", frmMallExportRegenerateReport);
USEFORM("POS\SelectMember.cpp", frmSelectMember);
USEFORM("DelayedPaymentTabs.cpp", frmDelayedPaymentTabs);
USEFORM("POS\MenuManagerItemSettings.cpp", frmItemSettings);
USEFORM("Timer.cpp", frmClockTime);
USEFORM("POS\SelectItem.cpp", frmSelectItem);
USEFORM("MallExportResendReport.cpp", frmMallExportResendReport);
USEFORM("Maintenance\NewUser.cpp", frmNewUser);
USEFORM("POS\Message.cpp", frmMessage);
USEFORM("ChitNumberGui.cpp", frmChitNumberGui);
USEFORM("Maintenance\TaxMaintenance.cpp", frmTaxMaintenance);
USEFORM("CaptNamePhone.cpp", frmCaptNamePhone);
USEFORM("SelectSaveOption.cpp", frmSelectSaveOption);
USEFORM("Tables_ChitNumbers.cpp", frmTables_ChitNumbers);
USEFORM("MMdata.cpp", frmData);
USEFORM("PSectionInstructionEdit.cpp", frmPSectionInstructionEdit);
USEFORM("Discounts.cpp", frmDiscounts);
USEFORM("POS\ForcedSides.cpp", frmSelectForcedSides);
USEFORM("Maintenance\GeneralMaintenance.cpp", frmGeneralMaintenance);
USEFORM("POS\SelectRMSRoom.cpp", frmRMSRoom);
USEFORM("AutoZReportingSetup.cpp", frmAutoZReportingSetup);
USEFORM("LoyaltyMateOperationDialogBox.cpp", frmLoyaltyMateOperationDialogBox);
USEFORM("POS\SplitPayment.cpp", frmSplitPayment);
USEFORM("POS\Transfer.cpp", frmTransfer);
USEFORM("SendForm.cpp", frmSendForm);
USEFORM("POS\GUIDiscount.cpp", frmDiscount);
USEFORM("Maintenance\UserMaintenance.cpp", frmUserMaintenance);
USEFORM("Maintain.cpp", frmMaintain);
USEFORM("Processing.cpp", frmProcessing);
USEFORM("POS\ConfirmOrd.cpp", frmConfirmOrder);
USEFORM("POS\SelectActiveMenus.cpp", frmSelectActiveMenus);
USEFORM("CardSwipe.cpp", frmCardSwipe);
USEFORM("ParkedSales.cpp", frmParkedSales);
USEFORM("POS\PatronCount.cpp", frmPatronCount);
USEFORM("DropDown.cpp", frmDropDownFunc);
USEFORM("IntaMateConfiguration.cpp", frmIntaMateConfiguration);
USEFORM("POS\ForcedOptions.cpp", frmSelectForcedOptions);
USEFORM("POS\BillGroup.cpp", frmBillGroup);
USEFORM("PocketVoucher.cpp", frmPocketVoucher);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMMessageBox.cpp", frmMessageBox);
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
USEFORM("GenericGroupGUI.cpp", frmPaymentGroupGUI);
USEFORM("Maintenance\PrinterMaintenance.cpp", frmPrinterMaintenance);
USEFORM("DeskTop\VerticalSelect.cpp", frmVerticalSelect);
USEFORM("DbMod.cpp", frmDBMod);
USEFORM("SmartCards\SmartCardConfigGUI.cpp", frmSmartCardConfiguration);
USEFORM("POS\SelectReceipt.cpp", frmSelectReceipt);
USEFORM("SelectTable2.cpp", FrmSelectTable2);
USEFORM("Maintenance\PrinterControlPanel.cpp", frmPrinterControlPanel);
USEFORM("SmartCards\SmartcardPreloader.cpp", Smartcard_Preloader);
USEFORM("GenericGroupAssignment.cpp", frmGenericGroupAssignment);
USEFORM("ListManager.cpp", frmListManager);
USEFORM("SetButtonFontSize.cpp", frmSetButtonFontSize);
USEFORM("Casino Security Screen\TransactionAuditScreen.cpp", frmTransactionAuditScreen);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchKeyboard.cpp", frmTouchKeyboard);
USEFORM("POS\GroupGUI.cpp", frmGroupGUI);
USEFORM("EftPosDialogs.cpp", frmEftPos);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchNumpad.cpp", frmTouchNumpad);
USEFORM("NewPaymentType.cpp", frmNewPaymentType);
USEFORM("Analysis.cpp", frmAnalysis);
USEFORM("Security\Secure.cpp", frmSecurity);
USEFORM("POS\Paytype.cpp", frmPaymentType);
USEFORM("POS\MemberCreation.cpp", frmMemberCreation);
USEFORM("POS\Scales.cpp", frmScales);
USEFORM("Casino Security Screen\MMSecurityScreenSettings.cpp", frmMMSecurityScreenSettings);
USEFORM("ReportDisplayNav.cpp", frmReportDisplayNav);
USEFORM("POS\SelectPaymentTypeAndValue.cpp", frmSelectPaymentTypeAndValue);
USEFORM("POS\SelectGlass.cpp", frmSelectGlass);
USEFORM("POS\SelectZed.cpp", frmSelectZed);
USEFORM("POS\TransferItemOrGuest.cpp", frmTransferItemOrGuest);
USEFORM("POS\TaxRemoval.cpp", frmTaxRemoval);
USEFORM("SetupGlCodes.cpp", frmSetupGlCodes);
USEFORM("ComboForm.cpp", frmComboForm);
USEFORM("SelectDateAndTime.cpp", frmSelectDateTime);
USEFORM("ProductSearch\ProductSearch.cpp", frmProductSearch);
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










