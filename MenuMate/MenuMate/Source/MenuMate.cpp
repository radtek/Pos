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

USEFORM("PHSConfiguration.cpp", frmPHSConfiguration);
USEFORM("PrinterRedirect.cpp", frmPrinterRedirect);
USEFORM("POS\ShowPrintout.cpp", frmShowPrintout);
USEFORM("DropDownVar.cpp", frmDropDownVar);
USEFORM("POS\Functions.cpp", frmFunctions);
USEFORM("TabManagerGUI.cpp", frmTabManager);
USEFORM("POS\GetMoney.cpp", frmControlTransaction);
USEFORM("Comms\SerialConfig.cpp", frmSerialConfig);
USEFORM("Maintenance\EditCustomer.cpp", frmEditCustomer);
USEFORM("POS\SelectKitchenDocket.cpp", frmSelectKitchenDocket);
USEFORM("EftChequeVerify.cpp", frmChequeVerfiy);
USEFORM("DiscountEditor.cpp", frmDiscountEdit);
USEFORM("Maintenance\PrinterControlPanel.cpp", frmPrinterControlPanel);
USEFORM("POS\SelectReceipt.cpp", frmSelectReceipt);
USEFORM("SelectTable2.cpp", FrmSelectTable2);
USEFORM("GenericGroupAssignment.cpp", frmGenericGroupAssignment);
USEFORM("SmartCards\SmartcardPreloader.cpp", Smartcard_Preloader);
USEFORM("GenericGroupGUI.cpp", frmPaymentGroupGUI);
USEFORM("DeskTop\VerticalSelect.cpp", frmVerticalSelect);
USEFORM("Maintenance\PrinterMaintenance.cpp", frmPrinterMaintenance);
USEFORM("POS\SelectPhoenixRoom.cpp", frmPhoenixRoom);
USEFORM("SmartCards\SmartCardConfigGUI.cpp", frmSmartCardConfiguration);
USEFORM("DbMod.cpp", frmDBMod);
USEFORM("CardSwipe.cpp", frmCardSwipe);
USEFORM("POS\SelectActiveMenus.cpp", frmSelectActiveMenus);
USEFORM("IntaMateConfiguration.cpp", frmIntaMateConfiguration);
USEFORM("DropDown.cpp", frmDropDownFunc);
USEFORM("ParkedSales.cpp", frmParkedSales);
USEFORM("POS\PatronCount.cpp", frmPatronCount);
USEFORM("POS\GUIDiscount.cpp", frmDiscount);
USEFORM("Maintenance\UserMaintenance.cpp", frmUserMaintenance);
USEFORM("SendForm.cpp", frmSendForm);
USEFORM("POS\SplitPayment.cpp", frmSplitPayment);
USEFORM("POS\Transfer.cpp", frmTransfer);
USEFORM("POS\ConfirmOrd.cpp", frmConfirmOrder);
USEFORM("Maintain.cpp", frmMaintain);
USEFORM("Processing.cpp", frmProcessing);
USEFORM("Security\Login.cpp", frmLogin);
USEFORM("POS\SelectDish.cpp", frmSelectDish);
USEFORM("POS\HoldSend.cpp", frmHoldSend);
USEFORM("POS\BillGroup.cpp", frmBillGroup);
USEFORM("POS\ForcedOptions.cpp", frmSelectForcedOptions);
USEFORM("PocketVoucher.cpp", frmPocketVoucher);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMMessageBox.cpp", frmMessageBox);
USEFORM("POS\TransferItemOrGuest.cpp", frmTransferItemOrGuest);
USEFORM("POS\TaxRemoval.cpp", frmTaxRemoval);
USEFORM("ComboForm.cpp", frmComboForm);
USEFORM("POS\SelectGlass.cpp", frmSelectGlass);
USEFORM("POS\SelectZed.cpp", frmSelectZed);
USEFORM("POS\SelectPaymentTypeAndValue.cpp", frmSelectPaymentTypeAndValue);
USEFORM("ProductSearch\ProductSearch.cpp", frmProductSearch);
USEFORM("SelectDateAndTime.cpp", frmSelectDateTime);
USEFORM("SetupGlCodes.cpp", frmSetupGlCodes);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchNumpad.cpp", frmTouchNumpad);
USEFORM("EftPosDialogs.cpp", frmEftPos);
USEFORM("NewPaymentType.cpp", frmNewPaymentType);
USEFORM("POS\GroupGUI.cpp", frmGroupGUI);
USEFORM("Casino Security Screen\TransactionAuditScreen.cpp", frmTransactionAuditScreen);
USEFORM("SetButtonFontSize.cpp", frmSetButtonFontSize);
USEFORM("ListManager.cpp", frmListManager);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchKeyboard.cpp", frmTouchKeyboard);
USEFORM("POS\Scales.cpp", frmScales);
USEFORM("POS\MemberCreation.cpp", frmMemberCreation);
USEFORM("ReportDisplayNav.cpp", frmReportDisplayNav);
USEFORM("Casino Security Screen\MMSecurityScreenSettings.cpp", frmMMSecurityScreenSettings);
USEFORM("Analysis.cpp", frmAnalysis);
USEFORM("POS\Paytype.cpp", frmPaymentType);
USEFORM("Security\Secure.cpp", frmSecurity);
USEFORM("POS\SelectCurrentMenu.cpp", frmSelectCurrentMenu);
USEFORM("LoyaltyMateOperationDialogBox.cpp", frmLoyaltyMateOperationDialogBox);
USEFORM("Maintenance\MessageMaintenance.cpp", frmMessageMaintenance);
USEFORM("Maintenance\HappyHourSettings.cpp", frmHappyHourSettings);
USEFORM("DealEditor.cpp", frmDealEditorForm);
USEFORM("Main.cpp", frmMain);
USEFORM("VirtualPrintersInterface.cpp", frmVirtualPrintersInterface);
USEFORM("POS\SelectRoom.cpp", frmSelectRoom);
USEFORM("POS\AddTab.cpp", frmAddTab);
USEFORM("Locations.cpp", frmLocations);
USEFORM("POS\Users.cpp", frmUsers);
USEFORM("SystemConfig.cpp", frmSystemConfig);
USEFORM("ChitList.cpp", frmChitList);
USEFORM("Security\TimeClock.cpp", frmTimeClock);
USEFORM("XeroSettings.cpp", frmXeroSettings);
USEFORM("ReportDisplay.cpp", frmReportDisplay);
USEFORM("POS\SelectTable.cpp", frmSelectTable);
USEFORM("DealLevelsEditor.cpp", Form1);
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
USEFORM("ChitNumberGui.cpp", frmChitNumberGui);
USEFORM("MallExportResendReport.cpp", frmMallExportResendReport);
USEFORM("POS\Message.cpp", frmMessage);
USEFORM("Maintenance\NewUser.cpp", frmNewUser);
USEFORM("Maintenance\TaxMaintenance.cpp", frmTaxMaintenance);
USEFORM("AutoZReportingSetup.cpp", frmAutoZReportingSetup);
USEFORM("Maintenance\GeneralMaintenance.cpp", frmGeneralMaintenance);
USEFORM("POS\ForcedSides.cpp", frmSelectForcedSides);
USEFORM("Tables_ChitNumbers.cpp", frmTables_ChitNumbers);
USEFORM("PSectionInstructionEdit.cpp", frmPSectionInstructionEdit);
USEFORM("Discounts.cpp", frmDiscounts);
USEFORM("SelectSaveOption.cpp", frmSelectSaveOption);
USEFORM("POS\SelectRMSRoom.cpp", frmRMSRoom);
USEFORM("MMdata.cpp", frmData);
USEFORM("Chefmate\ChefMateSettings.cpp", frmChefmateSettings);
USEFORM("TierLevelEditor.cpp", frmTierLevelEditor);
USEFORM("SyndCodeGui.cpp", frmSyndCodeGui);
USEFORM("MallExportRegenerateReport.cpp", frmMallExportRegenerateReport);
USEFORM("SelectSizesAsList.cpp", frmSelectSizes);
USEFORM("POS\SelectItem.cpp", frmSelectItem);
USEFORM("Timer.cpp", frmClockTime);
USEFORM("POS\MenuManagerItemSettings.cpp", frmItemSettings);
USEFORM("DelayedPaymentTabs.cpp", frmDelayedPaymentTabs);
USEFORM("POS\SelectMember.cpp", frmSelectMember);
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










