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

USEFORM("POS\ShowPrintout.cpp", frmShowPrintout);
USEFORM("DropDownVar.cpp", frmDropDownVar);
USEFORM("DiscountEditor.cpp", frmDiscountEdit);
USEFORM("Maintenance\EditCustomer.cpp", frmEditCustomer);
USEFORM("POS\SelectKitchenDocket.cpp", frmSelectKitchenDocket);
USEFORM("POS\Functions.cpp", frmFunctions);
USEFORM("PrinterRedirect.cpp", frmPrinterRedirect);
USEFORM("PHSConfiguration.cpp", frmPHSConfiguration);
USEFORM("POS\GetMoney.cpp", frmControlTransaction);
USEFORM("TabManagerGUI.cpp", frmTabManager);
USEFORM("DeskTop\VerticalSelect.cpp", frmVerticalSelect);
USEFORM("EftChequeVerify.cpp", frmChequeVerfiy);
USEFORM("Comms\SerialConfig.cpp", frmSerialConfig);
USEFORM("POS\SelectReceipt.cpp", frmSelectReceipt);
USEFORM("SmartCards\SmartCardConfigGUI.cpp", frmSmartCardConfiguration);
USEFORM("SelectTable2.cpp", FrmSelectTable2);
USEFORM("Maintenance\PrinterControlPanel.cpp", frmPrinterControlPanel);
USEFORM("DbMod.cpp", frmDBMod);
USEFORM("POS\SelectPhoenixRoom.cpp", frmPhoenixRoom);
USEFORM("GenericGroupGUI.cpp", frmPaymentGroupGUI);
USEFORM("Maintenance\PrinterMaintenance.cpp", frmPrinterMaintenance);
USEFORM("POS\SelectActiveMenus.cpp", frmSelectActiveMenus);
USEFORM("POS\ConfirmOrd.cpp", frmConfirmOrder);
USEFORM("ParkedSales.cpp", frmParkedSales);
USEFORM("POS\PatronCount.cpp", frmPatronCount);
USEFORM("CardSwipe.cpp", frmCardSwipe);
USEFORM("POS\SplitPayment.cpp", frmSplitPayment);
USEFORM("AutoZReportingSetup.cpp", frmAutoZReportingSetup);
USEFORM("POS\Transfer.cpp", frmTransfer);
USEFORM("POS\GUIDiscount.cpp", frmDiscount);
USEFORM("Maintain.cpp", frmMaintain);
USEFORM("Maintenance\UserMaintenance.cpp", frmUserMaintenance);
USEFORM("SendForm.cpp", frmSendForm);
USEFORM("DropDown.cpp", frmDropDownFunc);
USEFORM("Processing.cpp", frmProcessing);
USEFORM("Security\Login.cpp", frmLogin);
USEFORM("POS\HoldSend.cpp", frmHoldSend);
USEFORM("PocketVoucher.cpp", frmPocketVoucher);
USEFORM("POS\SelectDish.cpp", frmSelectDish);
USEFORM("IntaMateConfiguration.cpp", frmIntaMateConfiguration);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMMessageBox.cpp", frmMessageBox);
USEFORM("POS\ForcedOptions.cpp", frmSelectForcedOptions);
USEFORM("POS\BillGroup.cpp", frmBillGroup);
USEFORM("POS\SelectZed.cpp", frmSelectZed);
USEFORM("POS\SelectPaymentTypeAndValue.cpp", frmSelectPaymentTypeAndValue);
USEFORM("ReportDisplayNav.cpp", frmReportDisplayNav);
USEFORM("Casino Security Screen\MMSecurityScreenSettings.cpp", frmMMSecurityScreenSettings);
USEFORM("ComboForm.cpp", frmComboForm);
USEFORM("POS\TransferItemOrGuest.cpp", frmTransferItemOrGuest);
USEFORM("ProductSearch\ProductSearch.cpp", frmProductSearch);
USEFORM("POS\SelectGlass.cpp", frmSelectGlass);
USEFORM("POS\TaxRemoval.cpp", frmTaxRemoval);
USEFORM("SelectDateAndTime.cpp", frmSelectDateTime);
USEFORM("Casino Security Screen\TransactionAuditScreen.cpp", frmTransactionAuditScreen);
USEFORM("POS\GroupGUI.cpp", frmGroupGUI);
USEFORM("EftPosDialogs.cpp", frmEftPos);
USEFORM("GenericGroupAssignment.cpp", frmGenericGroupAssignment);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchNumpad.cpp", frmTouchNumpad);
USEFORM("ListManager.cpp", frmListManager);
USEFORM("SetButtonFontSize.cpp", frmSetButtonFontSize);
USEFORM("SmartCards\SmartcardPreloader.cpp", Smartcard_Preloader);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchKeyboard.cpp", frmTouchKeyboard);
USEFORM("POS\Paytype.cpp", frmPaymentType);
USEFORM("POS\MemberCreation.cpp", frmMemberCreation);
USEFORM("SetupGlCodes.cpp", frmSetupGlCodes);
USEFORM("POS\Scales.cpp", frmScales);
USEFORM("Analysis.cpp", frmAnalysis);
USEFORM("NewPaymentType.cpp", frmNewPaymentType);
USEFORM("LoyaltyMateOperationDialogBox.cpp", frmLoyaltyMateOperationDialogBox);
USEFORM("Security\Secure.cpp", frmSecurity);
USEFORM("POS\SelectCurrentMenu.cpp", frmSelectCurrentMenu);
USEFORM("Maintenance\MessageMaintenance.cpp", frmMessageMaintenance);
USEFORM("DealLevelsEditor.cpp", Form1);
USEFORM("DealEditor.cpp", frmDealEditorForm);
USEFORM("Main.cpp", frmMain);
USEFORM("POS\SelectRoom.cpp", frmSelectRoom);
USEFORM("Maintenance\HappyHourSettings.cpp", frmHappyHourSettings);
USEFORM("POS\AddTab.cpp", frmAddTab);
USEFORM("POS\Users.cpp", frmUsers);
USEFORM("SystemConfig.cpp", frmSystemConfig);
USEFORM("POS\SelectTable.cpp", frmSelectTable);
USEFORM("ChitList.cpp", frmChitList);
USEFORM("Locations.cpp", frmLocations);
USEFORM("Security\TimeClock.cpp", frmTimeClock);
USEFORM("XeroSettings.cpp", frmXeroSettings);
USEFORM("VirtualPrintersInterface.cpp", frmVirtualPrintersInterface);
USEFORM("ReportDisplay.cpp", frmReportDisplay);
USEFORM("POS\MenuManager.cpp", frmMenuManager);
USEFORM("ProcessWebOrder.cpp", frmProcessWebOrder);
USEFORM("Setup.cpp", frmSetup);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMColourPicker.cpp", frmColourPicker);
USEFORM("TerminalChitConfig.cpp", frmTerminalChitConfig);
USEFORM("DealsSetup.cpp", frmDeals);
USEFORM("SelectDiscounts.cpp", frmSelectDiscounts);
USEFORM("Maintenance\PaymentMaintenance.cpp", frmPaymentMaintenance);
USEFORM("POS\MenuManagerItemSizeSelection.cpp", frmItemSizeSelection);
USEFORM("POS\TipAmount.cpp", frmTipAmount);
USEFORM("DiscountTimes.cpp", frmDiscountTimes);
USEFORM("POS\TransferItems.cpp", frmTransferItem);
USEFORM("POS\PosMain.cpp", frmPOSMain);
USEFORM("POS\SalesforceAuthInfo.cpp", frmSalesforceAuthInfo);
USEFORM("TierLevels.cpp", frmTierLevel);
USEFORM("CaptNamePhone.cpp", frmCaptNamePhone);
USEFORM("Maintenance\TaxMaintenance.cpp", frmTaxMaintenance);
USEFORM("ChitNumberGui.cpp", frmChitNumberGui);
USEFORM("MallExportResendReport.cpp", frmMallExportResendReport);
USEFORM("POS\SelectItem.cpp", frmSelectItem);
USEFORM("MMdata.cpp", frmData);
USEFORM("Maintenance\NewUser.cpp", frmNewUser);
USEFORM("POS\Message.cpp", frmMessage);
USEFORM("Maintenance\GeneralMaintenance.cpp", frmGeneralMaintenance);
USEFORM("POS\ForcedSides.cpp", frmSelectForcedSides);
USEFORM("SelectSaveOption.cpp", frmSelectSaveOption);
USEFORM("Tables_ChitNumbers.cpp", frmTables_ChitNumbers);
USEFORM("POS\SelectRMSRoom.cpp", frmRMSRoom);
USEFORM("PSectionInstructionEdit.cpp", frmPSectionInstructionEdit);
USEFORM("Discounts.cpp", frmDiscounts);
USEFORM("MallExportRegenerateReport.cpp", frmMallExportRegenerateReport);
USEFORM("TierLevelEditor.cpp", frmTierLevelEditor);
USEFORM("Chefmate\ChefMateSettings.cpp", frmChefmateSettings);
USEFORM("SelectSizesAsList.cpp", frmSelectSizes);
USEFORM("Timer.cpp", frmClockTime);
USEFORM("POS\MenuManagerItemSettings.cpp", frmItemSettings);
USEFORM("POS\SelectMember.cpp", frmSelectMember);
USEFORM("DelayedPaymentTabs.cpp", frmDelayedPaymentTabs);
USEFORM("SyndCodeGui.cpp", frmSyndCodeGui);
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










