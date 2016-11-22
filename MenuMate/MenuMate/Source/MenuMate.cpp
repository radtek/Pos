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

USEFORM("DealsSetup.cpp", frmDeals);
USEFORM("Maintenance\PaymentMaintenance.cpp", frmPaymentMaintenance);
USEFORM("SelectDiscounts.cpp", frmSelectDiscounts);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMColourPicker.cpp", frmColourPicker);
USEFORM("TerminalChitConfig.cpp", frmTerminalChitConfig);
USEFORM("TierLevels.cpp", frmTierLevel);
USEFORM("POS\SalesforceAuthInfo.cpp", frmSalesforceAuthInfo);
USEFORM("POS\PosMain.cpp", frmPOSMain);
USEFORM("POS\TipAmount.cpp", frmTipAmount);
USEFORM("POS\MenuManagerItemSizeSelection.cpp", frmItemSizeSelection);
USEFORM("POS\TransferItems.cpp", frmTransferItem);
USEFORM("DiscountTimes.cpp", frmDiscountTimes);
USEFORM("VirtualPrintersInterface.cpp", frmVirtualPrintersInterface);
USEFORM("Main.cpp", frmMain);
USEFORM("Locations.cpp", frmLocations);
USEFORM("POS\SelectRoom.cpp", frmSelectRoom);
USEFORM("POS\AddTab.cpp", frmAddTab);
USEFORM("DealEditor.cpp", frmDealEditorForm);
USEFORM("Maintenance\HappyHourSettings.cpp", frmHappyHourSettings);
USEFORM("Maintenance\MessageMaintenance.cpp", frmMessageMaintenance);
USEFORM("POS\Users.cpp", frmUsers);
USEFORM("DealLevelsEditor.cpp", Form1);
USEFORM("POS\SelectTable.cpp", frmSelectTable);
USEFORM("ProcessWebOrder.cpp", frmProcessWebOrder);
USEFORM("Setup.cpp", frmSetup);
USEFORM("POS\MenuManager.cpp", frmMenuManager);
USEFORM("SystemConfig.cpp", frmSystemConfig);
USEFORM("XeroSettings.cpp", frmXeroSettings);
USEFORM("ReportDisplay.cpp", frmReportDisplay);
USEFORM("ChitList.cpp", frmChitList);
USEFORM("Security\TimeClock.cpp", frmTimeClock);
USEFORM("SyndCodeGui.cpp", frmSyndCodeGui);
USEFORM("TierLevelEditor.cpp", frmTierLevelEditor);
USEFORM("MallExportRegenerateReport.cpp", frmMallExportRegenerateReport);
USEFORM("SelectSizesAsList.cpp", frmSelectSizes);
USEFORM("Chefmate\ChefMateSettings.cpp", frmChefmateSettings);
USEFORM("POS\SelectMember.cpp", frmSelectMember);
USEFORM("Timer.cpp", frmClockTime);
USEFORM("POS\MenuManagerItemSettings.cpp", frmItemSettings);
USEFORM("POS\SelectItem.cpp", frmSelectItem);
USEFORM("DelayedPaymentTabs.cpp", frmDelayedPaymentTabs);
USEFORM("Maintenance\TaxMaintenance.cpp", frmTaxMaintenance);
USEFORM("CaptNamePhone.cpp", frmCaptNamePhone);
USEFORM("ChitNumberGui.cpp", frmChitNumberGui);
USEFORM("POS\Message.cpp", frmMessage);
USEFORM("Maintenance\NewUser.cpp", frmNewUser);
USEFORM("MallExportResendReport.cpp", frmMallExportResendReport);
USEFORM("MMdata.cpp", frmData);
USEFORM("Maintenance\GeneralMaintenance.cpp", frmGeneralMaintenance);
USEFORM("POS\ForcedSides.cpp", frmSelectForcedSides);
USEFORM("AutoZReportingSetup.cpp", frmAutoZReportingSetup);
USEFORM("SelectSaveOption.cpp", frmSelectSaveOption);
USEFORM("POS\SelectRMSRoom.cpp", frmRMSRoom);
USEFORM("Discounts.cpp", frmDiscounts);
USEFORM("Tables_ChitNumbers.cpp", frmTables_ChitNumbers);
USEFORM("PSectionInstructionEdit.cpp", frmPSectionInstructionEdit);
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
USEFORM("PocketVoucher.cpp", frmPocketVoucher);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMMessageBox.cpp", frmMessageBox);
USEFORM("POS\BillGroup.cpp", frmBillGroup);
USEFORM("Security\Login.cpp", frmLogin);
USEFORM("POS\SelectDish.cpp", frmSelectDish);
USEFORM("POS\HoldSend.cpp", frmHoldSend);
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
USEFORM("Maintenance\PrinterControlPanel.cpp", frmPrinterControlPanel);
USEFORM("POS\SelectPhoenixRoom.cpp", frmPhoenixRoom);
USEFORM("DeskTop\VerticalSelect.cpp", frmVerticalSelect);
USEFORM("Maintenance\PrinterMaintenance.cpp", frmPrinterMaintenance);
USEFORM("DbMod.cpp", frmDBMod);
USEFORM("SmartCards\SmartCardConfigGUI.cpp", frmSmartCardConfiguration);
USEFORM("GenericGroupGUI.cpp", frmPaymentGroupGUI);
USEFORM("POS\SelectReceipt.cpp", frmSelectReceipt);
USEFORM("SelectTable2.cpp", FrmSelectTable2);
USEFORM("GenericGroupAssignment.cpp", frmGenericGroupAssignment);
USEFORM("SmartCards\SmartcardPreloader.cpp", Smartcard_Preloader);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchKeyboard.cpp", frmTouchKeyboard);
USEFORM("ListManager.cpp", frmListManager);
USEFORM("POS\MemberCreation.cpp", frmMemberCreation);
USEFORM("POS\Scales.cpp", frmScales);
USEFORM("EftPosDialogs.cpp", frmEftPos);
USEFORM("SetButtonFontSize.cpp", frmSetButtonFontSize);
USEFORM("NewPaymentType.cpp", frmNewPaymentType);
USEFORM("Casino Security Screen\TransactionAuditScreen.cpp", frmTransactionAuditScreen);
USEFORM("POS\GroupGUI.cpp", frmGroupGUI);
USEFORM("POS\Paytype.cpp", frmPaymentType);
USEFORM("Analysis.cpp", frmAnalysis);
USEFORM("LoyaltyMateOperationDialogBox.cpp", frmLoyaltyMateOperationDialogBox);
USEFORM("POS\SelectCurrentMenu.cpp", frmSelectCurrentMenu);
USEFORM("Security\Secure.cpp", frmSecurity);
USEFORM("ReportDisplayNav.cpp", frmReportDisplayNav);
USEFORM("Casino Security Screen\MMSecurityScreenSettings.cpp", frmMMSecurityScreenSettings);
USEFORM("POS\SelectZed.cpp", frmSelectZed);
USEFORM("POS\SelectPaymentTypeAndValue.cpp", frmSelectPaymentTypeAndValue);
USEFORM("POS\TransferItemOrGuest.cpp", frmTransferItemOrGuest);
USEFORM("POS\TaxRemoval.cpp", frmTaxRemoval);
USEFORM("POS\SelectGlass.cpp", frmSelectGlass);
USEFORM("ComboForm.cpp", frmComboForm);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchNumpad.cpp", frmTouchNumpad);
USEFORM("ProductSearch\ProductSearch.cpp", frmProductSearch);
USEFORM("SelectDateAndTime.cpp", frmSelectDateTime);
USEFORM("SetupGlCodes.cpp", frmSetupGlCodes);
USEFORM("RegenerateMallReport.cpp", frmRegenerateMallReport);
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










