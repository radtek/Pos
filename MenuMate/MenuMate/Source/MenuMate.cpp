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

USEFORM("DropDownVar.cpp", frmDropDownVar);
USEFORM("POS\ShowPrintout.cpp", frmShowPrintout);
USEFORM("POS\SelectKitchenDocket.cpp", frmSelectKitchenDocket);
USEFORM("Comms\SerialConfig.cpp", frmSerialConfig);
USEFORM("POS\Functions.cpp", frmFunctions);
USEFORM("PHSConfiguration.cpp", frmPHSConfiguration);
USEFORM("PrinterRedirect.cpp", frmPrinterRedirect);
USEFORM("TabManagerGUI.cpp", frmTabManager);
USEFORM("POS\GetMoney.cpp", frmControlTransaction);
USEFORM("DiscountEditor.cpp", frmDiscountEdit);
USEFORM("Maintenance\EditCustomer.cpp", frmEditCustomer);
USEFORM("EftChequeVerify.cpp", frmChequeVerfiy);
USEFORM("SelectTable2.cpp", FrmSelectTable2);
USEFORM("POS\SelectReceipt.cpp", frmSelectReceipt);
USEFORM("Maintenance\PrinterControlPanel.cpp", frmPrinterControlPanel);
USEFORM("POS\SelectPhoenixRoom.cpp", frmPhoenixRoom);
USEFORM("DbMod.cpp", frmDBMod);
USEFORM("SmartCards\SmartCardConfigGUI.cpp", frmSmartCardConfiguration);
USEFORM("GenericGroupGUI.cpp", frmPaymentGroupGUI);
USEFORM("Maintenance\PrinterMaintenance.cpp", frmPrinterMaintenance);
USEFORM("DeskTop\VerticalSelect.cpp", frmVerticalSelect);
USEFORM("POS\SelectActiveMenus.cpp", frmSelectActiveMenus);
USEFORM("CardSwipe.cpp", frmCardSwipe);
USEFORM("POS\PatronCount.cpp", frmPatronCount);
USEFORM("DropDown.cpp", frmDropDownFunc);
USEFORM("ParkedSales.cpp", frmParkedSales);
USEFORM("SendForm.cpp", frmSendForm);
USEFORM("POS\GUIDiscount.cpp", frmDiscount);
USEFORM("POS\Transfer.cpp", frmTransfer);
USEFORM("POS\SplitPayment.cpp", frmSplitPayment);
USEFORM("POS\ConfirmOrd.cpp", frmConfirmOrder);
USEFORM("Processing.cpp", frmProcessing);
USEFORM("Maintenance\UserMaintenance.cpp", frmUserMaintenance);
USEFORM("Maintain.cpp", frmMaintain);
USEFORM("Security\Login.cpp", frmLogin);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMMessageBox.cpp", frmMessageBox);
USEFORM("POS\HoldSend.cpp", frmHoldSend);
USEFORM("POS\SelectDish.cpp", frmSelectDish);
USEFORM("POS\BillGroup.cpp", frmBillGroup);
USEFORM("POS\ForcedOptions.cpp", frmSelectForcedOptions);
USEFORM("IntaMateConfiguration.cpp", frmIntaMateConfiguration);
USEFORM("PocketVoucher.cpp", frmPocketVoucher);
USEFORM("POS\TransferItemOrGuest.cpp", frmTransferItemOrGuest);
USEFORM("POS\SelectZed.cpp", frmSelectZed);
USEFORM("POS\SelectPaymentTypeAndValue.cpp", frmSelectPaymentTypeAndValue);
USEFORM("POS\SelectGlass.cpp", frmSelectGlass);
USEFORM("ReportDisplayNav.cpp", frmReportDisplayNav);
USEFORM("ProductSearch\ProductSearch.cpp", frmProductSearch);
USEFORM("SelectDateAndTime.cpp", frmSelectDateTime);
USEFORM("SetupGlCodes.cpp", frmSetupGlCodes);
USEFORM("POS\TaxRemoval.cpp", frmTaxRemoval);
USEFORM("ComboForm.cpp", frmComboForm);
USEFORM("EftPosDialogs.cpp", frmEftPos);
USEFORM("POS\GroupGUI.cpp", frmGroupGUI);
USEFORM("NewPaymentType.cpp", frmNewPaymentType);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchNumpad.cpp", frmTouchNumpad);
USEFORM("ListManager.cpp", frmListManager);
USEFORM("SmartCards\SmartcardPreloader.cpp", Smartcard_Preloader);
USEFORM("GenericGroupAssignment.cpp", frmGenericGroupAssignment);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchKeyboard.cpp", frmTouchKeyboard);
USEFORM("Casino Security Screen\TransactionAuditScreen.cpp", frmTransactionAuditScreen);
USEFORM("SetButtonFontSize.cpp", frmSetButtonFontSize);
USEFORM("POS\Paytype.cpp", frmPaymentType);
USEFORM("Casino Security Screen\MMSecurityScreenSettings.cpp", frmMMSecurityScreenSettings);
USEFORM("POS\MemberCreation.cpp", frmMemberCreation);
USEFORM("POS\Scales.cpp", frmScales);
USEFORM("Analysis.cpp", frmAnalysis);
USEFORM("LoyaltyMateOperationDialogBox.cpp", frmLoyaltyMateOperationDialogBox);
USEFORM("Security\Secure.cpp", frmSecurity);
USEFORM("POS\SelectCurrentMenu.cpp", frmSelectCurrentMenu);
USEFORM("Maintenance\MessageMaintenance.cpp", frmMessageMaintenance);
USEFORM("DealEditor.cpp", frmDealEditorForm);
USEFORM("Main.cpp", frmMain);
USEFORM("POS\SelectRoom.cpp", frmSelectRoom);
USEFORM("VirtualPrintersInterface.cpp", frmVirtualPrintersInterface);
USEFORM("Maintenance\HappyHourSettings.cpp", frmHappyHourSettings);
USEFORM("POS\Users.cpp", frmUsers);
USEFORM("POS\AddTab.cpp", frmAddTab);
USEFORM("ChitList.cpp", frmChitList);
USEFORM("Locations.cpp", frmLocations);
USEFORM("SystemConfig.cpp", frmSystemConfig);
USEFORM("Security\TimeClock.cpp", frmTimeClock);
USEFORM("XeroSettings.cpp", frmXeroSettings);
USEFORM("ReportDisplay.cpp", frmReportDisplay);
USEFORM("POS\SelectTable.cpp", frmSelectTable);
USEFORM("DealLevelsEditor.cpp", Form1);
USEFORM("ProcessWebOrder.cpp", frmProcessWebOrder);
USEFORM("Setup.cpp", frmSetup);
USEFORM("POS\MenuManager.cpp", frmMenuManager);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMColourPicker.cpp", frmColourPicker);
USEFORM("TerminalChitConfig.cpp", frmTerminalChitConfig);
USEFORM("DealsSetup.cpp", frmDeals);
USEFORM("SelectDiscounts.cpp", frmSelectDiscounts);
USEFORM("Maintenance\PaymentMaintenance.cpp", frmPaymentMaintenance);
USEFORM("POS\TipAmount.cpp", frmTipAmount);
USEFORM("POS\MenuManagerItemSizeSelection.cpp", frmItemSizeSelection);
USEFORM("DiscountTimes.cpp", frmDiscountTimes);
USEFORM("POS\TransferItems.cpp", frmTransferItem);
USEFORM("POS\PosMain.cpp", frmPOSMain);
USEFORM("POS\SalesforceAuthInfo.cpp", frmSalesforceAuthInfo);
USEFORM("TierLevels.cpp", frmTierLevel);
USEFORM("Maintenance\TaxMaintenance.cpp", frmTaxMaintenance);
USEFORM("ChitNumberGui.cpp", frmChitNumberGui);
USEFORM("CaptNamePhone.cpp", frmCaptNamePhone);
USEFORM("POS\Message.cpp", frmMessage);
USEFORM("MallExportResendReport.cpp", frmMallExportResendReport);
USEFORM("Maintenance\NewUser.cpp", frmNewUser);
USEFORM("POS\ForcedSides.cpp", frmSelectForcedSides);
USEFORM("AutoZReportingSetup.cpp", frmAutoZReportingSetup);
USEFORM("Maintenance\GeneralMaintenance.cpp", frmGeneralMaintenance);
USEFORM("POS\SelectRMSRoom.cpp", frmRMSRoom);
USEFORM("Tables_ChitNumbers.cpp", frmTables_ChitNumbers);
USEFORM("SelectSaveOption.cpp", frmSelectSaveOption);
USEFORM("Discounts.cpp", frmDiscounts);
USEFORM("PSectionInstructionEdit.cpp", frmPSectionInstructionEdit);
USEFORM("MMdata.cpp", frmData);
USEFORM("POS\SelectItem.cpp", frmSelectItem);
USEFORM("Chefmate\ChefMateSettings.cpp", frmChefmateSettings);
USEFORM("TierLevelEditor.cpp", frmTierLevelEditor);
USEFORM("MallExportRegenerateReport.cpp", frmMallExportRegenerateReport);
USEFORM("SyndCodeGui.cpp", frmSyndCodeGui);
USEFORM("SelectSizesAsList.cpp", frmSelectSizes);
USEFORM("POS\MenuManagerItemSettings.cpp", frmItemSettings);
USEFORM("Timer.cpp", frmClockTime);
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










