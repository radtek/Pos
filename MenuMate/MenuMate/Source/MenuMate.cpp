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

USEFORM("GenericGroupAssignment.cpp", frmGenericGroupAssignment);
USEFORM("POS\BillGroup.cpp", frmBillGroup);
USEFORM("Casino Security Screen\TransactionAuditScreen.cpp", frmTransactionAuditScreen);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchNumpad.cpp", frmTouchNumpad);
USEFORM("SetButtonFontSize.cpp", frmSetButtonFontSize);
USEFORM("GenericGroupGUI.cpp", frmPaymentGroupGUI);
USEFORM("DiscountEditor.cpp", frmDiscountEdit);
USEFORM("SmartCards\SmartCardConfigGUI.cpp", frmSmartCardConfiguration);
USEFORM("SmartCards\SmartcardPreloader.cpp", Smartcard_Preloader);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchKeyboard.cpp", frmTouchKeyboard);
USEFORM("SelectTable2.cpp", FrmSelectTable2);
USEFORM("POS\MemberCreation.cpp", frmMemberCreation);
USEFORM("POS\SelectReceipt.cpp", frmSelectReceipt);
USEFORM("SetupGlCodes.cpp", frmSetupGlCodes);
USEFORM("POS\PatronCount.cpp", frmPatronCount);
USEFORM("POS\SelectZed.cpp", frmSelectZed);
USEFORM("NewPaymentType.cpp", frmNewPaymentType);
USEFORM("Maintenance\PrinterMaintenance.cpp", frmPrinterMaintenance);
USEFORM("ListManager.cpp", frmListManager);
USEFORM("POS\Scales.cpp", frmScales);
USEFORM("Security\Login.cpp", frmLogin);
USEFORM("Maintenance\UserMaintenance.cpp", frmUserMaintenance);
USEFORM("POS\HoldSend.cpp", frmHoldSend);
USEFORM("POS\GetMoney.cpp", frmControlTransaction);
USEFORM("POS\SelectDish.cpp", frmSelectDish);
USEFORM("POS\ShowPrintout.cpp", frmShowPrintout);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMMessageBox.cpp", frmMessageBox);
USEFORM("POS\GroupGUI.cpp", frmGroupGUI);
USEFORM("POS\SelectKitchenDocket.cpp", frmSelectKitchenDocket);
USEFORM("PHSConfiguration.cpp", frmPHSConfiguration);
USEFORM("POS\Functions.cpp", frmFunctions);
USEFORM("Comms\SerialConfig.cpp", frmSerialConfig);
USEFORM("DropDownVar.cpp", frmDropDownVar);
USEFORM("DbMod.cpp", frmDBMod);
USEFORM("DeskTop\VerticalSelect.cpp", frmVerticalSelect);
USEFORM("Maintenance\PrinterControlPanel.cpp", frmPrinterControlPanel);
USEFORM("ShowEJournal.cpp", frmEJournal);
USEFORM("ProductSearch\ProductSearch.cpp", frmProductSearch);
USEFORM("POS\SelectPhoenixRoom.cpp", frmPhoenixRoom);
USEFORM("PocketVoucher.cpp", frmPocketVoucher);
USEFORM("Maintenance\EditCustomer.cpp", frmEditCustomer);
USEFORM("PrinterRedirect.cpp", frmPrinterRedirect);
USEFORM("EftPosDialogs.cpp", frmEftPos);
USEFORM("SetUpPosPlus.cpp", frmSetUpPosPlus);
USEFORM("CaptureCustomerDetails.cpp", frmCaptureDiscountDetails);
USEFORM("SelectDateAndTime.cpp", frmSelectDateTime);
USEFORM("POS\TaxRemoval.cpp", frmTaxRemoval);
USEFORM("POS\SelectCurrentMenu.cpp", frmSelectCurrentMenu);
USEFORM("Maintenance\WalletConfiguration.cpp", frmWalletConfiguration);
USEFORM("ReportDisplayNav.cpp", frmReportDisplayNav);
USEFORM("POS\Paytype.cpp", frmPaymentType);
USEFORM("RegenerateMallReport.cpp", frmRegenerateMallReport);
USEFORM("ParkedSales.cpp", frmParkedSales);
USEFORM("POS\SelectRoom.cpp", frmSelectRoom);
USEFORM("EftChequeVerify.cpp", frmChequeVerfiy);
USEFORM("POS\TransferItemOrGuest.cpp", frmTransferItemOrGuest);
USEFORM("Casino Security Screen\MMSecurityScreenSettings.cpp", frmMMSecurityScreenSettings);
USEFORM("Maintenance\HappyHourSettings.cpp", frmHappyHourSettings);
USEFORM("POS\SelectPaymentTypeAndValue.cpp", frmSelectPaymentTypeAndValue);
USEFORM("POS\SelectGlass.cpp", frmSelectGlass);
USEFORM("DealEditor.cpp", frmDealEditorForm);
USEFORM("POS\ForcedOptions.cpp", frmSelectForcedOptions);
USEFORM("ComboForm.cpp", frmComboForm);
USEFORM("ProcessWebOrder.cpp", frmProcessWebOrder);
USEFORM("POS\AddTab.cpp", frmAddTab);
USEFORM("VirtualPrintersInterface.cpp", frmVirtualPrintersInterface);
USEFORM("Processing.cpp", frmProcessing);
USEFORM("POS\SelectTable.cpp", frmSelectTable);
USEFORM("SystemConfig.cpp", frmSystemConfig);
USEFORM("Main.cpp", frmMain);
USEFORM("Security\TimeClock.cpp", frmTimeClock);
USEFORM("TierLevelEditor.cpp", frmTierLevelEditor);
USEFORM("Chefmate\ChefMateSettings.cpp", frmChefmateSettings);
USEFORM("SelectSaveOption.cpp", frmSelectSaveOption);
USEFORM("Locations.cpp", frmLocations);
USEFORM("ReportDisplay.cpp", frmReportDisplay);
USEFORM("ChitNumberGui.cpp", frmChitNumberGui);
USEFORM("POS\Message.cpp", frmMessage);
USEFORM("DealLevelsEditor.cpp", Form1);
USEFORM("ChitList.cpp", frmChitList);
USEFORM("Setup.cpp", frmSetup);
USEFORM("POS\MenuManager.cpp", frmMenuManager);
USEFORM("XeroSettings.cpp", frmXeroSettings);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMColourPicker.cpp", frmColourPicker);
USEFORM("SelectDiscounts.cpp", frmSelectDiscounts);
USEFORM("TerminalChitConfig.cpp", frmTerminalChitConfig);
USEFORM("DealsSetup.cpp", frmDeals);
USEFORM("Maintenance\PaymentMaintenance.cpp", frmPaymentMaintenance);
USEFORM("POS\MenuManagerItemSizeSelection.cpp", frmItemSizeSelection);
USEFORM("DiscountTimes.cpp", frmDiscountTimes);
USEFORM("POS\PosMain.cpp", frmPOSMain);
USEFORM("TierLevels.cpp", frmTierLevel);
USEFORM("CaptNamePhone.cpp", frmCaptNamePhone);
USEFORM("POS\SelectItem.cpp", frmSelectItem);
USEFORM("DelayedPaymentTabs.cpp", frmDelayedPaymentTabs);
USEFORM("POS\SelectRMSRoom.cpp", frmRMSRoom);
USEFORM("SendForm.cpp", frmSendForm);
USEFORM("Maintain.cpp", frmMaintain);
USEFORM("Discounts.cpp", frmDiscounts);
USEFORM("AutoZReportingSetup.cpp", frmAutoZReportingSetup);
USEFORM("POS\SelectMember.cpp", frmSelectMember);
USEFORM("Maintenance\MessageMaintenance.cpp", frmMessageMaintenance);
USEFORM("POS\ConfirmOrd.cpp", frmConfirmOrder);
USEFORM("CardSwipe.cpp", frmCardSwipe);
USEFORM("POS\ForcedSides.cpp", frmSelectForcedSides);
USEFORM("POS\SplitPayment.cpp", frmSplitPayment);
USEFORM("Tables_ChitNumbers.cpp", frmTables_ChitNumbers);
USEFORM("POS\Users.cpp", frmUsers);
USEFORM("SyndCodeGui.cpp", frmSyndCodeGui);
USEFORM("Security\Secure.cpp", frmSecurity);
USEFORM("MMdata.cpp", frmData);
USEFORM("DropDown.cpp", frmDropDownFunc);
USEFORM("TabManagerGUI.cpp", frmTabManager);
USEFORM("MallExportRegenerateReport.cpp", frmMallExportRegenerateReport);
USEFORM("POS\Transfer.cpp", frmTransfer);
USEFORM("Timer.cpp", frmClockTime);
USEFORM("POS\SelectActiveMenus.cpp", frmSelectActiveMenus);
USEFORM("Maintenance\GeneralMaintenance.cpp", frmGeneralMaintenance);
USEFORM("Maintenance\NewUser.cpp", frmNewUser);
USEFORM("MallSalesTypeAssignment.cpp", frmMallSalesTypeAssignment);
USEFORM("EFTPOSConfiguration.cpp", frmEFTPOSConfig);
USEFORM("PSectionInstructionEdit.cpp", frmPSectionInstructionEdit);
USEFORM("SelectSizesAsList.cpp", frmSelectSizes);
USEFORM("POS\GUIDiscount.cpp", frmDiscount);
USEFORM("AddSalesType.cpp", frmAddSalesType);
USEFORM("Maintenance\TaxMaintenance.cpp", frmTaxMaintenance);
USEFORM("LoyaltyMateOperationDialogBox.cpp", frmLoyaltyMateOperationDialogBox);
USEFORM("POS\MenuManagerItemSettings.cpp", frmItemSettings);
USEFORM("ServingTime.cpp", frmServingTime);
USEFORM("GuestList.cpp", Form2);
USEFORM("MallExportResendReport.cpp", frmMallExportResendReport);
USEFORM("Analysis.cpp", frmAnalysis);
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











