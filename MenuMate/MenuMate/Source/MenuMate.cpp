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
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchNumpad.cpp", frmTouchNumpad);
USEFORM("SelectDiscounts.cpp", frmSelectDiscounts);
USEFORM("DealsSetup.cpp", frmDeals);
USEFORM("TerminalChitConfig.cpp", frmTerminalChitConfig);
USEFORM("Setup.cpp", frmSetup);
USEFORM("ProcessWebOrder.cpp", frmProcessWebOrder);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMColourPicker.cpp", frmColourPicker);
USEFORM("POS\MenuManager.cpp", frmMenuManager);
USEFORM("POS\PosMain.cpp", frmPOSMain);
USEFORM("POS\TransferItems.cpp", frmTransferItem);
USEFORM("POS\SalesforceAuthInfo.cpp", frmSalesforceAuthInfo);
USEFORM("TierLevels.cpp", frmTierLevel);
USEFORM("DiscountTimes.cpp", frmDiscountTimes);
USEFORM("POS\TipAmount.cpp", frmTipAmount);
USEFORM("Maintenance\PaymentMaintenance.cpp", frmPaymentMaintenance);
USEFORM("POS\MenuManagerItemSizeSelection.cpp", frmItemSizeSelection);
USEFORM("DealEditor.cpp", frmDealEditorForm);
USEFORM("VirtualPrintersInterface.cpp", frmVirtualPrintersInterface);
USEFORM("POS\SelectRoom.cpp", frmSelectRoom);
USEFORM("Maintenance\HappyHourSettings.cpp", frmHappyHourSettings);
USEFORM("POS\Users.cpp", frmUsers);
USEFORM("Main.cpp", frmMain);
USEFORM("POS\SelectCurrentMenu.cpp", frmSelectCurrentMenu);
USEFORM("Security\Secure.cpp", frmSecurity);
USEFORM("Maintenance\MessageMaintenance.cpp", frmMessageMaintenance);
USEFORM("ReportDisplay.cpp", frmReportDisplay);
USEFORM("POS\SelectTable.cpp", frmSelectTable);
USEFORM("DealLevelsEditor.cpp", Form1);
USEFORM("POS\AddTab.cpp", frmAddTab);
USEFORM("SystemConfig.cpp", frmSystemConfig);
USEFORM("XeroSettings.cpp", frmXeroSettings);
USEFORM("Security\TimeClock.cpp", frmTimeClock);
USEFORM("Locations.cpp", frmLocations);
USEFORM("ChitList.cpp", frmChitList);
USEFORM("TierLevelEditor.cpp", frmTierLevelEditor);
USEFORM("SyndCodeGui.cpp", frmSyndCodeGui);
USEFORM("SelectSizesAsList.cpp", frmSelectSizes);
USEFORM("MallExportRegenerateReport.cpp", frmMallExportRegenerateReport);
USEFORM("Chefmate\ChefMateSettings.cpp", frmChefmateSettings);
USEFORM("POS\SelectMember.cpp", frmSelectMember);
USEFORM("DelayedPaymentTabs.cpp", frmDelayedPaymentTabs);
USEFORM("POS\MenuManagerItemSettings.cpp", frmItemSettings);
USEFORM("Timer.cpp", frmClockTime);
USEFORM("MallExportResendReport.cpp", frmMallExportResendReport);
USEFORM("POS\Message.cpp", frmMessage);
USEFORM("Maintenance\NewUser.cpp", frmNewUser);
USEFORM("Maintenance\TaxMaintenance.cpp", frmTaxMaintenance);
USEFORM("ChitNumberGui.cpp", frmChitNumberGui);
USEFORM("CaptNamePhone.cpp", frmCaptNamePhone);
USEFORM("SelectSaveOption.cpp", frmSelectSaveOption);
USEFORM("MMdata.cpp", frmData);
USEFORM("Tables_ChitNumbers.cpp", frmTables_ChitNumbers);
USEFORM("PSectionInstructionEdit.cpp", frmPSectionInstructionEdit);
USEFORM("POS\SelectItem.cpp", frmSelectItem);
USEFORM("Discounts.cpp", frmDiscounts);
USEFORM("POS\ForcedSides.cpp", frmSelectForcedSides);
USEFORM("POS\SelectRMSRoom.cpp", frmRMSRoom);
USEFORM("AutoZReportingSetup.cpp", frmAutoZReportingSetup);
USEFORM("Maintenance\GeneralMaintenance.cpp", frmGeneralMaintenance);
USEFORM("POS\SplitPayment.cpp", frmSplitPayment);
USEFORM("SendForm.cpp", frmSendForm);
USEFORM("POS\GUIDiscount.cpp", frmDiscount);
USEFORM("POS\Transfer.cpp", frmTransfer);
USEFORM("Maintenance\UserMaintenance.cpp", frmUserMaintenance);
USEFORM("Maintain.cpp", frmMaintain);
USEFORM("POS\ConfirmOrd.cpp", frmConfirmOrder);
USEFORM("Processing.cpp", frmProcessing);
USEFORM("POS\SelectActiveMenus.cpp", frmSelectActiveMenus);
USEFORM("CardSwipe.cpp", frmCardSwipe);
USEFORM("ParkedSales.cpp", frmParkedSales);
USEFORM("POS\PatronCount.cpp", frmPatronCount);
USEFORM("DropDown.cpp", frmDropDownFunc);
USEFORM("IntaMateConfiguration.cpp", frmIntaMateConfiguration);
USEFORM("POS\BillGroup.cpp", frmBillGroup);
USEFORM("POS\ForcedOptions.cpp", frmSelectForcedOptions);
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
USEFORM("DbMod.cpp", frmDBMod);
USEFORM("Maintenance\PrinterMaintenance.cpp", frmPrinterMaintenance);
USEFORM("GenericGroupGUI.cpp", frmPaymentGroupGUI);
USEFORM("DeskTop\VerticalSelect.cpp", frmVerticalSelect);
USEFORM("SmartCards\SmartCardConfigGUI.cpp", frmSmartCardConfiguration);
USEFORM("POS\SelectReceipt.cpp", frmSelectReceipt);
USEFORM("SelectTable2.cpp", FrmSelectTable2);
USEFORM("Maintenance\PrinterControlPanel.cpp", frmPrinterControlPanel);
USEFORM("SmartCards\SmartcardPreloader.cpp", Smartcard_Preloader);
USEFORM("GenericGroupAssignment.cpp", frmGenericGroupAssignment);
USEFORM("ListManager.cpp", frmListManager);
USEFORM("SetButtonFontSize.cpp", frmSetButtonFontSize);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchKeyboard.cpp", frmTouchKeyboard);
USEFORM("Casino Security Screen\TransactionAuditScreen.cpp", frmTransactionAuditScreen);
USEFORM("POS\GroupGUI.cpp", frmGroupGUI);
USEFORM("EftPosDialogs.cpp", frmEftPos);
USEFORM("NewPaymentType.cpp", frmNewPaymentType);
USEFORM("Analysis.cpp", frmAnalysis);
USEFORM("LoyaltyMateOperationDialogBox.cpp", frmLoyaltyMateOperationDialogBox);
USEFORM("POS\Paytype.cpp", frmPaymentType);
USEFORM("POS\MemberCreation.cpp", frmMemberCreation);
USEFORM("POS\Scales.cpp", frmScales);
USEFORM("Casino Security Screen\MMSecurityScreenSettings.cpp", frmMMSecurityScreenSettings);
USEFORM("ReportDisplayNav.cpp", frmReportDisplayNav);
USEFORM("POS\SelectPaymentTypeAndValue.cpp", frmSelectPaymentTypeAndValue);
USEFORM("POS\SelectGlass.cpp", frmSelectGlass);
USEFORM("POS\SelectZed.cpp", frmSelectZed);
USEFORM("POS\TransferItemOrGuest.cpp", frmTransferItemOrGuest);
USEFORM("ComboForm.cpp", frmComboForm);
USEFORM("SelectDateAndTime.cpp", frmSelectDateTime);
USEFORM("POS\TaxRemoval.cpp", frmTaxRemoval);
USEFORM("SetupGlCodes.cpp", frmSetupGlCodes);
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










