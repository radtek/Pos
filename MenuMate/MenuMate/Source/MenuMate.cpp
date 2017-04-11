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
USEFORM("SelectDiscounts.cpp", frmSelectDiscounts);
USEFORM("POS\MenuManagerItemSizeSelection.cpp", frmItemSizeSelection);
USEFORM("Maintenance\PaymentMaintenance.cpp", frmPaymentMaintenance);
USEFORM("POS\MenuManager.cpp", frmMenuManager);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMColourPicker.cpp", frmColourPicker);
USEFORM("TerminalChitConfig.cpp", frmTerminalChitConfig);
USEFORM("POS\SelectItem.cpp", frmSelectItem);
USEFORM("DelayedPaymentTabs.cpp", frmDelayedPaymentTabs);
USEFORM("POS\SelectRMSRoom.cpp", frmRMSRoom);
USEFORM("CaptNamePhone.cpp", frmCaptNamePhone);
USEFORM("TierLevels.cpp", frmTierLevel);
USEFORM("DiscountTimes.cpp", frmDiscountTimes);
USEFORM("POS\PosMain.cpp", frmPOSMain);
USEFORM("POS\SelectRoom.cpp", frmSelectRoom);
USEFORM("POS\AddTab.cpp", frmAddTab);
USEFORM("Security\TimeClock.cpp", frmTimeClock);
USEFORM("Main.cpp", frmMain);
USEFORM("VirtualPrintersInterface.cpp", frmVirtualPrintersInterface);
USEFORM("POS\SelectTable.cpp", frmSelectTable);
USEFORM("POS\Users.cpp", frmUsers);
USEFORM("DealEditor.cpp", frmDealEditorForm);
USEFORM("Maintenance\HappyHourSettings.cpp", frmHappyHourSettings);
USEFORM("Maintenance\MessageMaintenance.cpp", frmMessageMaintenance);
USEFORM("ChitList.cpp", frmChitList);
USEFORM("XeroSettings.cpp", frmXeroSettings);
USEFORM("Setup.cpp", frmSetup);
USEFORM("DealLevelsEditor.cpp", Form1);
USEFORM("ProcessWebOrder.cpp", frmProcessWebOrder);
USEFORM("TierLevelEditor.cpp", frmTierLevelEditor);
USEFORM("Chefmate\ChefMateSettings.cpp", frmChefmateSettings);
USEFORM("SystemConfig.cpp", frmSystemConfig);
USEFORM("POS\Message.cpp", frmMessage);
USEFORM("ReportDisplay.cpp", frmReportDisplay);
USEFORM("Locations.cpp", frmLocations);
USEFORM("Maintenance\NewUser.cpp", frmNewUser);
USEFORM("POS\MenuManagerItemSettings.cpp", frmItemSettings);
USEFORM("MallExportRegenerateReport.cpp", frmMallExportRegenerateReport);
USEFORM("POS\SelectMember.cpp", frmSelectMember);
USEFORM("SyndCodeGui.cpp", frmSyndCodeGui);
USEFORM("Timer.cpp", frmClockTime);
USEFORM("SelectSizesAsList.cpp", frmSelectSizes);
USEFORM("MallExportResendReport.cpp", frmMallExportResendReport);
USEFORM("Maintenance\TaxMaintenance.cpp", frmTaxMaintenance);
USEFORM("ChitNumberGui.cpp", frmChitNumberGui);
USEFORM("Maintenance\GeneralMaintenance.cpp", frmGeneralMaintenance);
USEFORM("LoyaltyMateOperationDialogBox.cpp", frmLoyaltyMateOperationDialogBox);
USEFORM("PSectionInstructionEdit.cpp", frmPSectionInstructionEdit);
USEFORM("SendForm.cpp", frmSendForm);
USEFORM("SelectSaveOption.cpp", frmSelectSaveOption);
USEFORM("MMdata.cpp", frmData);
USEFORM("Security\Secure.cpp", frmSecurity);
USEFORM("AutoZReportingSetup.cpp", frmAutoZReportingSetup);
USEFORM("Maintain.cpp", frmMaintain);
USEFORM("POS\GUIDiscount.cpp", frmDiscount);
USEFORM("POS\ForcedSides.cpp", frmSelectForcedSides);
USEFORM("Discounts.cpp", frmDiscounts);
USEFORM("POS\Transfer.cpp", frmTransfer);
USEFORM("Tables_ChitNumbers.cpp", frmTables_ChitNumbers);
USEFORM("POS\SplitPayment.cpp", frmSplitPayment);
USEFORM("POS\ConfirmOrd.cpp", frmConfirmOrder);
USEFORM("IntaMateConfiguration.cpp", frmIntaMateConfiguration);
USEFORM("CardSwipe.cpp", frmCardSwipe);
USEFORM("TabManagerGUI.cpp", frmTabManager);
USEFORM("POS\SelectActiveMenus.cpp", frmSelectActiveMenus);
USEFORM("ParkedSales.cpp", frmParkedSales);
USEFORM("POS\PatronCount.cpp", frmPatronCount);
USEFORM("DropDown.cpp", frmDropDownFunc);
USEFORM("POS\ForcedOptions.cpp", frmSelectForcedOptions);
USEFORM("Maintenance\UserMaintenance.cpp", frmUserMaintenance);
USEFORM("PocketVoucher.cpp", frmPocketVoucher);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMMessageBox.cpp", frmMessageBox);
USEFORM("Security\Login.cpp", frmLogin);
USEFORM("POS\SelectDish.cpp", frmSelectDish);
USEFORM("POS\HoldSend.cpp", frmHoldSend);
USEFORM("POS\GetMoney.cpp", frmControlTransaction);
USEFORM("POS\GroupGUI.cpp", frmGroupGUI);
USEFORM("EftChequeVerify.cpp", frmChequeVerfiy);
USEFORM("Maintenance\EditCustomer.cpp", frmEditCustomer);
USEFORM("POS\SelectKitchenDocket.cpp", frmSelectKitchenDocket);
USEFORM("Comms\SerialConfig.cpp", frmSerialConfig);
USEFORM("POS\BillGroup.cpp", frmBillGroup);
USEFORM("POS\ShowPrintout.cpp", frmShowPrintout);
USEFORM("PrinterRedirect.cpp", frmPrinterRedirect);
USEFORM("DropDownVar.cpp", frmDropDownVar);
USEFORM("DiscountEditor.cpp", frmDiscountEdit);
USEFORM("PHSConfiguration.cpp", frmPHSConfiguration);
USEFORM("POS\Functions.cpp", frmFunctions);
USEFORM("POS\SelectPhoenixRoom.cpp", frmPhoenixRoom);
USEFORM("DbMod.cpp", frmDBMod);
USEFORM("Maintenance\PrinterControlPanel.cpp", frmPrinterControlPanel);
USEFORM("Maintenance\PrinterMaintenance.cpp", frmPrinterMaintenance);
USEFORM("DeskTop\VerticalSelect.cpp", frmVerticalSelect);
USEFORM("SmartCards\SmartCardConfigGUI.cpp", frmSmartCardConfiguration);
USEFORM("POS\SelectReceipt.cpp", frmSelectReceipt);
USEFORM("GenericGroupGUI.cpp", frmPaymentGroupGUI);
USEFORM("SelectTable2.cpp", FrmSelectTable2);
USEFORM("SmartCards\SmartcardPreloader.cpp", Smartcard_Preloader);
USEFORM("GenericGroupAssignment.cpp", frmGenericGroupAssignment);
USEFORM("ListManager.cpp", frmListManager);
USEFORM("Casino Security Screen\TransactionAuditScreen.cpp", frmTransactionAuditScreen);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchKeyboard.cpp", frmTouchKeyboard);
USEFORM("SetButtonFontSize.cpp", frmSetButtonFontSize);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchNumpad.cpp", frmTouchNumpad);
USEFORM("NewPaymentType.cpp", frmNewPaymentType);
USEFORM("EftPosDialogs.cpp", frmEftPos);
USEFORM("POS\SelectCurrentMenu.cpp", frmSelectCurrentMenu);
USEFORM("POS\MemberCreation.cpp", frmMemberCreation);
USEFORM("POS\Paytype.cpp", frmPaymentType);
USEFORM("ReportDisplayNav.cpp", frmReportDisplayNav);
USEFORM("Processing.cpp", frmProcessing);
USEFORM("POS\Scales.cpp", frmScales);
USEFORM("Casino Security Screen\MMSecurityScreenSettings.cpp", frmMMSecurityScreenSettings);
USEFORM("POS\SelectPaymentTypeAndValue.cpp", frmSelectPaymentTypeAndValue);
USEFORM("POS\SelectZed.cpp", frmSelectZed);
USEFORM("POS\TransferItemOrGuest.cpp", frmTransferItemOrGuest);
USEFORM("Analysis.cpp", frmAnalysis);
USEFORM("POS\SelectGlass.cpp", frmSelectGlass);
USEFORM("ComboForm.cpp", frmComboForm);
USEFORM("SetupGlCodes.cpp", frmSetupGlCodes);
USEFORM("SelectDateAndTime.cpp", frmSelectDateTime);
USEFORM("ShowEJournal.cpp", frmEJournal);
USEFORM("POS\TaxRemoval.cpp", frmTaxRemoval);
USEFORM("ProductSearch\ProductSearch.cpp", frmProductSearch);
USEFORM("CaptureCustomerDetails.cpp", frmCaptureDiscountDetails);
USEFORM("RegenerateMallReport.cpp", frmRegenerateMallReport);
USEFORM("Maintenance\WalletConfiguration.cpp", frmWalletConfiguration);
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
         Application->CreateForm(__classid(TfrmRegenerateMallReport), &frmRegenerateMallReport);
         Application->CreateForm(__classid(TfrmRegenerateMallReport), &frmRegenerateMallReport);
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










