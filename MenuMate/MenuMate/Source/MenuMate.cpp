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

USEFORM("SmartCards\SmartcardPreloader.cpp", Smartcard_Preloader);
USEFORM("DiscountEditor.cpp", frmDiscountEdit);
USEFORM("GenericGroupGUI.cpp", frmPaymentGroupGUI);
USEFORM("POS\BillGroup.cpp", frmBillGroup);
USEFORM("GenericGroupAssignment.cpp", frmGenericGroupAssignment);
USEFORM("SmartCards\SmartCardConfigGUI.cpp", frmSmartCardConfiguration);
USEFORM("DbMod.cpp", frmDBMod);
USEFORM("DeskTop\VerticalSelect.cpp", frmVerticalSelect);
USEFORM("SelectTable2.cpp", FrmSelectTable2);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchKeyboard.cpp", frmTouchKeyboard);
USEFORM("POS\SelectReceipt.cpp", frmSelectReceipt);
USEFORM("POS\Scales.cpp", frmScales);
USEFORM("NewPaymentType.cpp", frmNewPaymentType);
USEFORM("POS\PatronCount.cpp", frmPatronCount);
USEFORM("POS\SelectZed.cpp", frmSelectZed);
USEFORM("POS\MemberCreation.cpp", frmMemberCreation);
USEFORM("ListManager.cpp", frmListManager);
USEFORM("Casino Security Screen\TransactionAuditScreen.cpp", frmTransactionAuditScreen);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchNumpad.cpp", frmTouchNumpad);
USEFORM("SetButtonFontSize.cpp", frmSetButtonFontSize);
USEFORM("Maintenance\PrinterMaintenance.cpp", frmPrinterMaintenance);
USEFORM("POS\HoldSend.cpp", frmHoldSend);
USEFORM("POS\GetMoney.cpp", frmControlTransaction);
USEFORM("POS\GroupGUI.cpp", frmGroupGUI);
USEFORM("POS\SelectDish.cpp", frmSelectDish);
USEFORM("Security\Login.cpp", frmLogin);
USEFORM("Maintenance\UserMaintenance.cpp", frmUserMaintenance);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMMessageBox.cpp", frmMessageBox);
USEFORM("PHSConfiguration.cpp", frmPHSConfiguration);
USEFORM("POS\SelectKitchenDocket.cpp", frmSelectKitchenDocket);
USEFORM("POS\ShowPrintout.cpp", frmShowPrintout);
USEFORM("POS\Functions.cpp", frmFunctions);
USEFORM("Comms\SerialConfig.cpp", frmSerialConfig);
USEFORM("DropDownVar.cpp", frmDropDownVar);
USEFORM("PocketVoucher.cpp", frmPocketVoucher);
USEFORM("Maintenance\PrinterControlPanel.cpp", frmPrinterControlPanel);
USEFORM("ProductSearch\ProductSearch.cpp", frmProductSearch);
USEFORM("Maintenance\EditCustomer.cpp", frmEditCustomer);
USEFORM("POS\SelectPhoenixRoom.cpp", frmPhoenixRoom);
USEFORM("PrinterRedirect.cpp", frmPrinterRedirect);
USEFORM("ShowEJournal.cpp", frmEJournal);
USEFORM("POS\TaxRemoval.cpp", frmTaxRemoval);
USEFORM("SetUpPosPlus.cpp", frmSetUpPosPlus);
USEFORM("CaptureCustomerDetails.cpp", frmCaptureDiscountDetails);
USEFORM("SelectDateAndTime.cpp", frmSelectDateTime);
USEFORM("POS\SelectCurrentMenu.cpp", frmSelectCurrentMenu);
USEFORM("Maintenance\WalletConfiguration.cpp", frmWalletConfiguration);
USEFORM("RegenerateMallReport.cpp", frmRegenerateMallReport);
USEFORM("POS\Paytype.cpp", frmPaymentType);
USEFORM("EftPosDialogs.cpp", frmEftPos);
USEFORM("ReportDisplayNav.cpp", frmReportDisplayNav);
USEFORM("ParkedSales.cpp", frmParkedSales);
USEFORM("Casino Security Screen\MMSecurityScreenSettings.cpp", frmMMSecurityScreenSettings);
USEFORM("IntaMateConfiguration.cpp", frmIntaMateConfiguration);
USEFORM("POS\TransferItemOrGuest.cpp", frmTransferItemOrGuest);
USEFORM("SetupGlCodes.cpp", frmSetupGlCodes);
USEFORM("Maintenance\HappyHourSettings.cpp", frmHappyHourSettings);
USEFORM("POS\SelectPaymentTypeAndValue.cpp", frmSelectPaymentTypeAndValue);
USEFORM("DealEditor.cpp", frmDealEditorForm);
USEFORM("ComboForm.cpp", frmComboForm);
USEFORM("ProcessWebOrder.cpp", frmProcessWebOrder);
USEFORM("POS\SelectGlass.cpp", frmSelectGlass);
USEFORM("POS\ForcedOptions.cpp", frmSelectForcedOptions);
USEFORM("Processing.cpp", frmProcessing);
USEFORM("POS\SelectRoom.cpp", frmSelectRoom);
USEFORM("POS\AddTab.cpp", frmAddTab);
USEFORM("EftChequeVerify.cpp", frmChequeVerfiy);
USEFORM("POS\SelectTable.cpp", frmSelectTable);
USEFORM("VirtualPrintersInterface.cpp", frmVirtualPrintersInterface);
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
USEFORM("POS\Users.cpp", frmUsers);
USEFORM("POS\ConfirmOrd.cpp", frmConfirmOrder);
USEFORM("POS\SplitPayment.cpp", frmSplitPayment);
USEFORM("Tables_ChitNumbers.cpp", frmTables_ChitNumbers);
USEFORM("CardSwipe.cpp", frmCardSwipe);
USEFORM("POS\ForcedSides.cpp", frmSelectForcedSides);
USEFORM("MMdata.cpp", frmData);
USEFORM("SyndCodeGui.cpp", frmSyndCodeGui);
USEFORM("Security\Secure.cpp", frmSecurity);
USEFORM("POS\Transfer.cpp", frmTransfer);
USEFORM("DropDown.cpp", frmDropDownFunc);
USEFORM("TabManagerGUI.cpp", frmTabManager);
USEFORM("POS\SelectActiveMenus.cpp", frmSelectActiveMenus);
USEFORM("Timer.cpp", frmClockTime);
USEFORM("MallExportRegenerateReport.cpp", frmMallExportRegenerateReport);
USEFORM("Maintenance\TaxMaintenance.cpp", frmTaxMaintenance);
USEFORM("Maintenance\NewUser.cpp", frmNewUser);
USEFORM("PSectionInstructionEdit.cpp", frmPSectionInstructionEdit);
USEFORM("MallSalesTypeAssignment.cpp", frmMallSalesTypeAssignment);
USEFORM("Maintenance\GeneralMaintenance.cpp", frmGeneralMaintenance);
USEFORM("POS\GUIDiscount.cpp", frmDiscount);
USEFORM("AddSalesType.cpp", frmAddSalesType);
USEFORM("LoyaltyMateOperationDialogBox.cpp", frmLoyaltyMateOperationDialogBox);
USEFORM("SelectSizesAsList.cpp", frmSelectSizes);
USEFORM("ServingTime.cpp", frmServingTime);
USEFORM("MallExportResendReport.cpp", frmMallExportResendReport);
USEFORM("POS\MenuManagerItemSettings.cpp", frmItemSettings);
USEFORM("GuestList.cpp", Form2);
USEFORM("EFTPOSConfiguration.cpp", frmEFTPOSConfig);
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











