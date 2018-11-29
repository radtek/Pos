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

USEFORM("TierLevels.cpp", frmTierLevel);
USEFORM("POS\PosMain.cpp", frmPOSMain);
USEFORM("POS\SelectItem.cpp", frmSelectItem);
USEFORM("DelayedPaymentTabs.cpp", frmDelayedPaymentTabs);
USEFORM("CaptNamePhone.cpp", frmCaptNamePhone);
USEFORM("DiscountTimes.cpp", frmDiscountTimes);
USEFORM("POS\MenuManagerItemSizeSelection.cpp", frmItemSizeSelection);
USEFORM("Maintain.cpp", frmMaintain);
USEFORM("Maintenance\MessageMaintenance.cpp", frmMessageMaintenance);
USEFORM("POS\ForcedSides.cpp", frmSelectForcedSides);
USEFORM("Discounts.cpp", frmDiscounts);
USEFORM("POS\SelectMember.cpp", frmSelectMember);
USEFORM("AutoZReportingSetup.cpp", frmAutoZReportingSetup);
USEFORM("SendForm.cpp", frmSendForm);
USEFORM("POS\SelectRMSRoom.cpp", frmRMSRoom);
USEFORM("ChitNumberGui.cpp", frmChitNumberGui);
USEFORM("ReportDisplay.cpp", frmReportDisplay);
USEFORM("Locations.cpp", frmLocations);
USEFORM("ChitList.cpp", frmChitList);
USEFORM("DealLevelsEditor.cpp", Form1);
USEFORM("POS\Message.cpp", frmMessage);
USEFORM("Security\TimeClock.cpp", frmTimeClock);
USEFORM("Main.cpp", frmMain);
USEFORM("SystemConfig.cpp", frmSystemConfig);
USEFORM("SelectSaveOption.cpp", frmSelectSaveOption);
USEFORM("TierLevelEditor.cpp", frmTierLevelEditor);
USEFORM("Chefmate\ChefMateSettings.cpp", frmChefmateSettings);
USEFORM("Maintenance\PaymentMaintenance.cpp", frmPaymentMaintenance);
USEFORM("DealsSetup.cpp", frmDeals);
USEFORM("Setup.cpp", frmSetup);
USEFORM("TerminalChitConfig.cpp", frmTerminalChitConfig);
USEFORM("SelectDiscounts.cpp", frmSelectDiscounts);
USEFORM("POS\MenuManager.cpp", frmMenuManager);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMColourPicker.cpp", frmColourPicker);
USEFORM("XeroSettings.cpp", frmXeroSettings);
USEFORM("POS\GUIDiscount.cpp", frmDiscount);
USEFORM("SelectSizesAsList.cpp", frmSelectSizes);
USEFORM("LoyaltyMateOperationDialogBox.cpp", frmLoyaltyMateOperationDialogBox);
USEFORM("AddSalesType.cpp", frmAddSalesType);
USEFORM("Maintenance\TaxMaintenance.cpp", frmTaxMaintenance);
USEFORM("Maintenance\NewUser.cpp", frmNewUser);
USEFORM("Maintenance\GeneralMaintenance.cpp", frmGeneralMaintenance);
USEFORM("PSectionInstructionEdit.cpp", frmPSectionInstructionEdit);
USEFORM("MallSalesTypeAssignment.cpp", frmMallSalesTypeAssignment);
USEFORM("EFTPOSConfiguration.cpp", frmEFTPOSConfig);
USEFORM("Analysis.cpp", frmAnalysis);
USEFORM("MallExportResendReport.cpp", frmMallExportResendReport);
USEFORM("POS\MenuManagerItemSettings.cpp", frmItemSettings);
USEFORM("ServingTime.cpp", frmServingTime);
USEFORM("GuestList.cpp", Form2);
USEFORM("Security\Secure.cpp", frmSecurity);
USEFORM("SyndCodeGui.cpp", frmSyndCodeGui);
USEFORM("MMdata.cpp", frmData);
USEFORM("POS\Users.cpp", frmUsers);
USEFORM("CardSwipe.cpp", frmCardSwipe);
USEFORM("POS\ConfirmOrd.cpp", frmConfirmOrder);
USEFORM("Tables_ChitNumbers.cpp", frmTables_ChitNumbers);
USEFORM("POS\SplitPayment.cpp", frmSplitPayment);
USEFORM("MallExportRegenerateReport.cpp", frmMallExportRegenerateReport);
USEFORM("Timer.cpp", frmClockTime);
USEFORM("POS\SelectActiveMenus.cpp", frmSelectActiveMenus);
USEFORM("POS\Transfer.cpp", frmTransfer);
USEFORM("TabManagerGUI.cpp", frmTabManager);
USEFORM("DropDown.cpp", frmDropDownFunc);
USEFORM("VirtualPrintersInterface.cpp", frmVirtualPrintersInterface);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMMessageBox.cpp", frmMessageBox);
USEFORM("POS\ShowPrintout.cpp", frmShowPrintout);
USEFORM("PHSConfiguration.cpp", frmPHSConfiguration);
USEFORM("POS\SelectKitchenDocket.cpp", frmSelectKitchenDocket);
USEFORM("POS\GroupGUI.cpp", frmGroupGUI);
USEFORM("POS\SelectDish.cpp", frmSelectDish);
USEFORM("POS\HoldSend.cpp", frmHoldSend);
USEFORM("POS\GetMoney.cpp", frmControlTransaction);
USEFORM("Maintenance\UserMaintenance.cpp", frmUserMaintenance);
USEFORM("Security\Login.cpp", frmLogin);
USEFORM("ProductSearch\ProductSearch.cpp", frmProductSearch);
USEFORM("ShowEJournal.cpp", frmEJournal);
USEFORM("Maintenance\EditCustomer.cpp", frmEditCustomer);
USEFORM("PrinterRedirect.cpp", frmPrinterRedirect);
USEFORM("POS\SelectPhoenixRoom.cpp", frmPhoenixRoom);
USEFORM("PocketVoucher.cpp", frmPocketVoucher);
USEFORM("POS\Functions.cpp", frmFunctions);
USEFORM("DropDownVar.cpp", frmDropDownVar);
USEFORM("Comms\SerialConfig.cpp", frmSerialConfig);
USEFORM("Maintenance\PrinterControlPanel.cpp", frmPrinterControlPanel);
USEFORM("DbMod.cpp", frmDBMod);
USEFORM("DeskTop\VerticalSelect.cpp", frmVerticalSelect);
USEFORM("SmartCards\SmartCardConfigGUI.cpp", frmSmartCardConfiguration);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchKeyboard.cpp", frmTouchKeyboard);
USEFORM("SelectTable2.cpp", FrmSelectTable2);
USEFORM("SmartCards\SmartcardPreloader.cpp", Smartcard_Preloader);
USEFORM("POS\BillGroup.cpp", frmBillGroup);
USEFORM("GenericGroupAssignment.cpp", frmGenericGroupAssignment);
USEFORM("DiscountEditor.cpp", frmDiscountEdit);
USEFORM("GenericGroupGUI.cpp", frmPaymentGroupGUI);
USEFORM("Casino Security Screen\TransactionAuditScreen.cpp", frmTransactionAuditScreen);
USEFORM("SetButtonFontSize.cpp", frmSetButtonFontSize);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchNumpad.cpp", frmTouchNumpad);
USEFORM("ListManager.cpp", frmListManager);
USEFORM("Maintenance\PrinterMaintenance.cpp", frmPrinterMaintenance);
USEFORM("POS\Scales.cpp", frmScales);
USEFORM("NewPaymentType.cpp", frmNewPaymentType);
USEFORM("POS\SelectReceipt.cpp", frmSelectReceipt);
USEFORM("POS\MemberCreation.cpp", frmMemberCreation);
USEFORM("POS\PatronCount.cpp", frmPatronCount);
USEFORM("POS\SelectZed.cpp", frmSelectZed);
USEFORM("POS\SelectPaymentTypeAndValue.cpp", frmSelectPaymentTypeAndValue);
USEFORM("Maintenance\HappyHourSettings.cpp", frmHappyHourSettings);
USEFORM("SetupGlCodes.cpp", frmSetupGlCodes);
USEFORM("ParkedSales.cpp", frmParkedSales);
USEFORM("POS\TransferItemOrGuest.cpp", frmTransferItemOrGuest);
USEFORM("Casino Security Screen\MMSecurityScreenSettings.cpp", frmMMSecurityScreenSettings);
USEFORM("EftChequeVerify.cpp", frmChequeVerfiy);
USEFORM("POS\SelectRoom.cpp", frmSelectRoom);
USEFORM("Processing.cpp", frmProcessing);
USEFORM("POS\AddTab.cpp", frmAddTab);
USEFORM("POS\SelectTable.cpp", frmSelectTable);
USEFORM("DealEditor.cpp", frmDealEditorForm);
USEFORM("POS\SelectGlass.cpp", frmSelectGlass);
USEFORM("POS\ForcedOptions.cpp", frmSelectForcedOptions);
USEFORM("ComboForm.cpp", frmComboForm);
USEFORM("ProcessWebOrder.cpp", frmProcessWebOrder);
USEFORM("SelectDateAndTime.cpp", frmSelectDateTime);
USEFORM("POS\SelectCurrentMenu.cpp", frmSelectCurrentMenu);
USEFORM("POS\TaxRemoval.cpp", frmTaxRemoval);
USEFORM("CaptureCustomerDetails.cpp", frmCaptureDiscountDetails);
USEFORM("SetUpPosPlus.cpp", frmSetUpPosPlus);
USEFORM("EftPosDialogs.cpp", frmEftPos);
USEFORM("POS\Paytype.cpp", frmPaymentType);
USEFORM("ReportDisplayNav.cpp", frmReportDisplayNav);
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
         Application->CreateForm(__classid(TfrmSetUpPosPlus), &frmSetUpPosPlus);
         Application->CreateForm(__classid(TfrmServingTime), &frmServingTime);
         Application->CreateForm(__classid(TfrmEFTPOSConfig), &frmEFTPOSConfig);
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











