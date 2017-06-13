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

USEFORM("Maintenance\PrinterMaintenance.cpp", frmPrinterMaintenance);
USEFORM("DeskTop\VerticalSelect.cpp", frmVerticalSelect);
USEFORM("POS\SelectReceipt.cpp", frmSelectReceipt);
USEFORM("SelectTable2.cpp", FrmSelectTable2);
USEFORM("GenericGroupGUI.cpp", frmPaymentGroupGUI);
USEFORM("SmartCards\SmartCardConfigGUI.cpp", frmSmartCardConfiguration);
USEFORM("DbMod.cpp", frmDBMod);
USEFORM("POS\Functions.cpp", frmFunctions);
USEFORM("PrinterRedirect.cpp", frmPrinterRedirect);
USEFORM("PHSConfiguration.cpp", frmPHSConfiguration);
USEFORM("DiscountEditor.cpp", frmDiscountEdit);
USEFORM("Maintenance\PrinterControlPanel.cpp", frmPrinterControlPanel);
USEFORM("POS\SelectPhoenixRoom.cpp", frmPhoenixRoom);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchKeyboard.cpp", frmTouchKeyboard);
USEFORM("ListManager.cpp", frmListManager);
USEFORM("Casino Security Screen\TransactionAuditScreen.cpp", frmTransactionAuditScreen);
USEFORM("SetButtonFontSize.cpp", frmSetButtonFontSize);
USEFORM("GenericGroupAssignment.cpp", frmGenericGroupAssignment);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchNumpad.cpp", frmTouchNumpad);
USEFORM("SmartCards\SmartcardPreloader.cpp", Smartcard_Preloader);
USEFORM("POS\BillGroup.cpp", frmBillGroup);
USEFORM("POS\ForcedOptions.cpp", frmSelectForcedOptions);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMMessageBox.cpp", frmMessageBox);
USEFORM("PocketVoucher.cpp", frmPocketVoucher);
USEFORM("Maintenance\UserMaintenance.cpp", frmUserMaintenance);
USEFORM("POS\SelectActiveMenus.cpp", frmSelectActiveMenus);
USEFORM("IntaMateConfiguration.cpp", frmIntaMateConfiguration);
USEFORM("TabManagerGUI.cpp", frmTabManager);
USEFORM("POS\PatronCount.cpp", frmPatronCount);
USEFORM("ParkedSales.cpp", frmParkedSales);
USEFORM("DropDown.cpp", frmDropDownFunc);
USEFORM("EftChequeVerify.cpp", frmChequeVerfiy);
USEFORM("Maintenance\EditCustomer.cpp", frmEditCustomer);
USEFORM("POS\SelectKitchenDocket.cpp", frmSelectKitchenDocket);
USEFORM("POS\ShowPrintout.cpp", frmShowPrintout);
USEFORM("DropDownVar.cpp", frmDropDownVar);
USEFORM("POS\SelectDish.cpp", frmSelectDish);
USEFORM("Comms\SerialConfig.cpp", frmSerialConfig);
USEFORM("Security\Login.cpp", frmLogin);
USEFORM("POS\GroupGUI.cpp", frmGroupGUI);
USEFORM("POS\HoldSend.cpp", frmHoldSend);
USEFORM("POS\GetMoney.cpp", frmControlTransaction);
USEFORM("SelectDateAndTime.cpp", frmSelectDateTime);
USEFORM("ComboForm.cpp", frmComboForm);
USEFORM("RegenerateMallReport.cpp", frmRegenerateMallReport);
USEFORM("Maintenance\WalletConfiguration.cpp", frmWalletConfiguration);
USEFORM("ServingTime.cpp", frmServingTime);
USEFORM("Analysis.cpp", frmAnalysis);
USEFORM("CaptureCustomerDetails.cpp", frmCaptureDiscountDetails);
USEFORM("ShowEJournal.cpp", frmEJournal);
USEFORM("ProductSearch\ProductSearch.cpp", frmProductSearch);
USEFORM("POS\TaxRemoval.cpp", frmTaxRemoval);
USEFORM("EftPosDialogs.cpp", frmEftPos);
USEFORM("POS\SelectCurrentMenu.cpp", frmSelectCurrentMenu);
USEFORM("POS\Scales.cpp", frmScales);
USEFORM("NewPaymentType.cpp", frmNewPaymentType);
USEFORM("POS\MemberCreation.cpp", frmMemberCreation);
USEFORM("POS\SelectZed.cpp", frmSelectZed);
USEFORM("POS\SelectGlass.cpp", frmSelectGlass);
USEFORM("SetupGlCodes.cpp", frmSetupGlCodes);
USEFORM("POS\SelectPaymentTypeAndValue.cpp", frmSelectPaymentTypeAndValue);
USEFORM("POS\Paytype.cpp", frmPaymentType);
USEFORM("POS\TransferItemOrGuest.cpp", frmTransferItemOrGuest);
USEFORM("Processing.cpp", frmProcessing);
USEFORM("Casino Security Screen\MMSecurityScreenSettings.cpp", frmMMSecurityScreenSettings);
USEFORM("ReportDisplayNav.cpp", frmReportDisplayNav);
USEFORM("Maintenance\MessageMaintenance.cpp", frmMessageMaintenance);
USEFORM("POS\Users.cpp", frmUsers);
USEFORM("Maintenance\HappyHourSettings.cpp", frmHappyHourSettings);
USEFORM("DealEditor.cpp", frmDealEditorForm);
USEFORM("POS\SelectRoom.cpp", frmSelectRoom);
USEFORM("POS\AddTab.cpp", frmAddTab);
USEFORM("Main.cpp", frmMain);
USEFORM("VirtualPrintersInterface.cpp", frmVirtualPrintersInterface);
USEFORM("POS\SelectTable.cpp", frmSelectTable);
USEFORM("Security\TimeClock.cpp", frmTimeClock);
USEFORM("SystemConfig.cpp", frmSystemConfig);
USEFORM("ProcessWebOrder.cpp", frmProcessWebOrder);
USEFORM("TierLevelEditor.cpp", frmTierLevelEditor);
USEFORM("Chefmate\ChefMateSettings.cpp", frmChefmateSettings);
USEFORM("POS\Message.cpp", frmMessage);
USEFORM("Locations.cpp", frmLocations);
USEFORM("ReportDisplay.cpp", frmReportDisplay);
USEFORM("ChitList.cpp", frmChitList);
USEFORM("XeroSettings.cpp", frmXeroSettings);
USEFORM("DealLevelsEditor.cpp", Form1);
USEFORM("Setup.cpp", frmSetup);
USEFORM("POS\MenuManager.cpp", frmMenuManager);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMColourPicker.cpp", frmColourPicker);
USEFORM("TerminalChitConfig.cpp", frmTerminalChitConfig);
USEFORM("SelectDiscounts.cpp", frmSelectDiscounts);
USEFORM("DealsSetup.cpp", frmDeals);
USEFORM("Maintenance\PaymentMaintenance.cpp", frmPaymentMaintenance);
USEFORM("POS\MenuManagerItemSizeSelection.cpp", frmItemSizeSelection);
USEFORM("DiscountTimes.cpp", frmDiscountTimes);
USEFORM("POS\PosMain.cpp", frmPOSMain);
USEFORM("TierLevels.cpp", frmTierLevel);
USEFORM("CaptNamePhone.cpp", frmCaptNamePhone);
USEFORM("SendForm.cpp", frmSendForm);
USEFORM("DelayedPaymentTabs.cpp", frmDelayedPaymentTabs);
USEFORM("POS\SelectRMSRoom.cpp", frmRMSRoom);
USEFORM("POS\SelectItem.cpp", frmSelectItem);
USEFORM("ChitNumberGui.cpp", frmChitNumberGui);
USEFORM("SelectSaveOption.cpp", frmSelectSaveOption);
USEFORM("AutoZReportingSetup.cpp", frmAutoZReportingSetup);
USEFORM("LoyaltyMateOperationDialogBox.cpp", frmLoyaltyMateOperationDialogBox);
USEFORM("PSectionInstructionEdit.cpp", frmPSectionInstructionEdit);
USEFORM("POS\GUIDiscount.cpp", frmDiscount);
USEFORM("POS\ForcedSides.cpp", frmSelectForcedSides);
USEFORM("Security\Secure.cpp", frmSecurity);
USEFORM("Maintain.cpp", frmMaintain);
USEFORM("POS\SplitPayment.cpp", frmSplitPayment);
USEFORM("POS\ConfirmOrd.cpp", frmConfirmOrder);
USEFORM("MMdata.cpp", frmData);
USEFORM("Tables_ChitNumbers.cpp", frmTables_ChitNumbers);
USEFORM("CardSwipe.cpp", frmCardSwipe);
USEFORM("POS\SelectMember.cpp", frmSelectMember);
USEFORM("Discounts.cpp", frmDiscounts);
USEFORM("POS\Transfer.cpp", frmTransfer);
USEFORM("SyndCodeGui.cpp", frmSyndCodeGui);
USEFORM("Timer.cpp", frmClockTime);
USEFORM("POS\MenuManagerItemSettings.cpp", frmItemSettings);
USEFORM("MallExportRegenerateReport.cpp", frmMallExportRegenerateReport);
USEFORM("Maintenance\NewUser.cpp", frmNewUser);
USEFORM("Maintenance\TaxMaintenance.cpp", frmTaxMaintenance);
USEFORM("MallExportResendReport.cpp", frmMallExportResendReport);
USEFORM("Maintenance\GeneralMaintenance.cpp", frmGeneralMaintenance);
USEFORM("SelectSizesAsList.cpp", frmSelectSizes);
USEFORM("AddSalesType.cpp", frmAddSalesType);
USEFORM("PMSTaxCodes.cpp", frmTaxCodes);
USEFORM("MallSalesTypeAssignment.cpp", frmMallSalesTypeAssignment);
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
         Application->CreateForm(__classid(TfrmTaxCodes), &frmTaxCodes);
         Application->CreateForm(__classid(TfrmServingTime), &frmServingTime);
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










