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

USEFORM("CaptNamePhone.cpp", frmCaptNamePhone);
USEFORM("POS\SelectItem.cpp", frmSelectItem);
USEFORM("TierLevels.cpp", frmTierLevel);
USEFORM("POS\SelectRMSRoom.cpp", frmRMSRoom);
USEFORM("DelayedPaymentTabs.cpp", frmDelayedPaymentTabs);
USEFORM("SendForm.cpp", frmSendForm);
USEFORM("POS\MenuManagerItemSizeSelection.cpp", frmItemSizeSelection);
USEFORM("POS\PosMain.cpp", frmPOSMain);
USEFORM("POS\SelectMember.cpp", frmSelectMember);
USEFORM("AutoZReportingSetup.cpp", frmAutoZReportingSetup);
USEFORM("Maintenance\MessageMaintenance.cpp", frmMessageMaintenance);
USEFORM("Discounts.cpp", frmDiscounts);
USEFORM("POS\ForcedSides.cpp", frmSelectForcedSides);
USEFORM("Maintain.cpp", frmMaintain);
USEFORM("DealLevelsEditor.cpp", Form1);
USEFORM("ChitNumberGui.cpp", frmChitNumberGui);
USEFORM("POS\Message.cpp", frmMessage);
USEFORM("Setup.cpp", frmSetup);
USEFORM("ChitList.cpp", frmChitList);
USEFORM("Chefmate\ChefMateSettings.cpp", frmChefmateSettings);
USEFORM("SelectSaveOption.cpp", frmSelectSaveOption);
USEFORM("TierLevelEditor.cpp", frmTierLevelEditor);
USEFORM("Main.cpp", frmMain);
USEFORM("ReportDisplay.cpp", frmReportDisplay);
USEFORM("Locations.cpp", frmLocations);
USEFORM("DealsSetup.cpp", frmDeals);
USEFORM("DiscountTimes.cpp", frmDiscountTimes);
USEFORM("Maintenance\PaymentMaintenance.cpp", frmPaymentMaintenance);
USEFORM("XeroSettings.cpp", frmXeroSettings);
USEFORM("TerminalChitConfig.cpp", frmTerminalChitConfig);
USEFORM("POS\MenuManager.cpp", frmMenuManager);
USEFORM("SelectDiscounts.cpp", frmSelectDiscounts);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMColourPicker.cpp", frmColourPicker);
USEFORM("LoyaltyMateOperationDialogBox.cpp", frmLoyaltyMateOperationDialogBox);
USEFORM("AddSalesType.cpp", frmAddSalesType);
USEFORM("SelectSizesAsList.cpp", frmSelectSizes);
USEFORM("POS\MenuManagerItemSettings.cpp", frmItemSettings);
USEFORM("Maintenance\TaxMaintenance.cpp", frmTaxMaintenance);
USEFORM("EFTPOSConfiguration.cpp", frmEFTPOSConfig);
USEFORM("PSectionInstructionEdit.cpp", frmPSectionInstructionEdit);
USEFORM("MallSalesTypeAssignment.cpp", frmMallSalesTypeAssignment);
USEFORM("POS\GUIDiscount.cpp", frmDiscount);
USEFORM("Maintenance\GeneralMaintenance.cpp", frmGeneralMaintenance);
USEFORM("Analysis.cpp", frmAnalysis);
USEFORM("MallExportResendReport.cpp", frmMallExportResendReport);
USEFORM("ServingTime.cpp", frmServingTime);
USEFORM("MMdata.cpp", frmData);
USEFORM("GuestList.cpp", Form2);
USEFORM("DropDown.cpp", frmDropDownFunc);
USEFORM("POS\Users.cpp", frmUsers);
USEFORM("Security\Secure.cpp", frmSecurity);
USEFORM("MallExportRegenerateReport.cpp", frmMallExportRegenerateReport);
USEFORM("POS\SplitPayment.cpp", frmSplitPayment);
USEFORM("CardSwipe.cpp", frmCardSwipe);
USEFORM("Tables_ChitNumbers.cpp", frmTables_ChitNumbers);
USEFORM("SyndCodeGui.cpp", frmSyndCodeGui);
USEFORM("POS\ConfirmOrd.cpp", frmConfirmOrder);
USEFORM("POS\SelectActiveMenus.cpp", frmSelectActiveMenus);
USEFORM("POS\Transfer.cpp", frmTransfer);
USEFORM("Maintenance\NewUser.cpp", frmNewUser);
USEFORM("TabManagerGUI.cpp", frmTabManager);
USEFORM("Timer.cpp", frmClockTime);
USEFORM("POS\ShowPrintout.cpp", frmShowPrintout);
USEFORM("Security\TimeClock.cpp", frmTimeClock);
USEFORM("POS\HoldSend.cpp", frmHoldSend);
USEFORM("PHSConfiguration.cpp", frmPHSConfiguration);
USEFORM("POS\SelectKitchenDocket.cpp", frmSelectKitchenDocket);
USEFORM("POS\SelectDish.cpp", frmSelectDish);
USEFORM("POS\GroupGUI.cpp", frmGroupGUI);
USEFORM("Security\Login.cpp", frmLogin);
USEFORM("Maintenance\UserMaintenance.cpp", frmUserMaintenance);
USEFORM("POS\GetMoney.cpp", frmControlTransaction);
USEFORM("ProductSearch\ProductSearch.cpp", frmProductSearch);
USEFORM("ShowEJournal.cpp", frmEJournal);
USEFORM("POS\Functions.cpp", frmFunctions);
USEFORM("PrinterRedirect.cpp", frmPrinterRedirect);
USEFORM("PocketVoucher.cpp", frmPocketVoucher);
USEFORM("POS\SelectPhoenixRoom.cpp", frmPhoenixRoom);
USEFORM("DropDownVar.cpp", frmDropDownVar);
USEFORM("Maintenance\EditCustomer.cpp", frmEditCustomer);
USEFORM("SmartCards\SmartCardConfigGUI.cpp", frmSmartCardConfiguration);
USEFORM("Maintenance\PrinterControlPanel.cpp", frmPrinterControlPanel);
USEFORM("DbMod.cpp", frmDBMod);
USEFORM("Comms\SerialConfig.cpp", frmSerialConfig);
USEFORM("DeskTop\VerticalSelect.cpp", frmVerticalSelect);
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
USEFORM("SetupGlCodes.cpp", frmSetupGlCodes);
USEFORM("POS\SelectPaymentTypeAndValue.cpp", frmSelectPaymentTypeAndValue);
USEFORM("Maintenance\HappyHourSettings.cpp", frmHappyHourSettings);
USEFORM("EftChequeVerify.cpp", frmChequeVerfiy);
USEFORM("DealEditor.cpp", frmDealEditorForm);
USEFORM("POS\SelectRoom.cpp", frmSelectRoom);
USEFORM("POS\AddTab.cpp", frmAddTab);
USEFORM("POS\TransferItemOrGuest.cpp", frmTransferItemOrGuest);
USEFORM("ParkedSales.cpp", frmParkedSales);
USEFORM("Casino Security Screen\MMSecurityScreenSettings.cpp", frmMMSecurityScreenSettings);
USEFORM("Processing.cpp", frmProcessing);
USEFORM("VirtualPrintersInterface.cpp", frmVirtualPrintersInterface);
USEFORM("SystemConfig.cpp", frmSystemConfig);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMMessageBox.cpp", frmMessageBox);
USEFORM("ProcessWebOrder.cpp", frmProcessWebOrder);
USEFORM("POS\SelectGlass.cpp", frmSelectGlass);
USEFORM("POS\ForcedOptions.cpp", frmSelectForcedOptions);
USEFORM("POS\SelectTable.cpp", frmSelectTable);
USEFORM("POS\TaxRemoval.cpp", frmTaxRemoval);
USEFORM("ComboForm.cpp", frmComboForm);
USEFORM("POS\SelectCurrentMenu.cpp", frmSelectCurrentMenu);
USEFORM("CaptureCustomerDetails.cpp", frmCaptureDiscountDetails);
USEFORM("SetUpPosPlus.cpp", frmSetUpPosPlus);
USEFORM("EftPosDialogs.cpp", frmEftPos);
USEFORM("ReportDisplayNav.cpp", frmReportDisplayNav);
USEFORM("POS\Paytype.cpp", frmPaymentType);
USEFORM("RegenerateMallReport.cpp", frmRegenerateMallReport);
USEFORM("SelectDateAndTime.cpp", frmSelectDateTime);
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











