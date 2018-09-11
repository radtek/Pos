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

USEFORM("POS\PosMain.cpp", frmPOSMain);
USEFORM("CaptNamePhone.cpp", frmCaptNamePhone);
USEFORM("TierLevels.cpp", frmTierLevel);
USEFORM("Maintenance\PaymentMaintenance.cpp", frmPaymentMaintenance);
USEFORM("POS\MenuManagerItemSizeSelection.cpp", frmItemSizeSelection);
USEFORM("DiscountTimes.cpp", frmDiscountTimes);
USEFORM("Discounts.cpp", frmDiscounts);
USEFORM("Maintain.cpp", frmMaintain);
USEFORM("POS\SelectRMSRoom.cpp", frmRMSRoom);
USEFORM("POS\SelectItem.cpp", frmSelectItem);
USEFORM("DelayedPaymentTabs.cpp", frmDelayedPaymentTabs);
USEFORM("SendForm.cpp", frmSendForm);
USEFORM("SelectSaveOption.cpp", frmSelectSaveOption);
USEFORM("Chefmate\ChefMateSettings.cpp", frmChefmateSettings);
USEFORM("TierLevelEditor.cpp", frmTierLevelEditor);
USEFORM("ChitNumberGui.cpp", frmChitNumberGui);
USEFORM("POS\Message.cpp", frmMessage);
USEFORM("Locations.cpp", frmLocations);
USEFORM("ReportDisplay.cpp", frmReportDisplay);
USEFORM("SystemConfig.cpp", frmSystemConfig);
USEFORM("VirtualPrintersInterface.cpp", frmVirtualPrintersInterface);
USEFORM("POS\SelectTable.cpp", frmSelectTable);
USEFORM("Security\TimeClock.cpp", frmTimeClock);
USEFORM("Main.cpp", frmMain);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMColourPicker.cpp", frmColourPicker);
USEFORM("XeroSettings.cpp", frmXeroSettings);
USEFORM("TerminalChitConfig.cpp", frmTerminalChitConfig);
USEFORM("SelectDiscounts.cpp", frmSelectDiscounts);
USEFORM("DealsSetup.cpp", frmDeals);
USEFORM("ChitList.cpp", frmChitList);
USEFORM("DealLevelsEditor.cpp", Form1);
USEFORM("POS\MenuManager.cpp", frmMenuManager);
USEFORM("Setup.cpp", frmSetup);
USEFORM("PSectionInstructionEdit.cpp", frmPSectionInstructionEdit);
USEFORM("Maintenance\GeneralMaintenance.cpp", frmGeneralMaintenance);
USEFORM("MallSalesTypeAssignment.cpp", frmMallSalesTypeAssignment);
USEFORM("POS\MenuManagerItemSettings.cpp", frmItemSettings);
USEFORM("Maintenance\NewUser.cpp", frmNewUser);
USEFORM("Maintenance\TaxMaintenance.cpp", frmTaxMaintenance);
USEFORM("GuestList.cpp", Form2);
USEFORM("ServingTime.cpp", frmServingTime);
USEFORM("EFTPOSConfiguration.cpp", frmEFTPOSConfig);
USEFORM("SelectSizesAsList.cpp", frmSelectSizes);
USEFORM("LoyaltyMateOperationDialogBox.cpp", frmLoyaltyMateOperationDialogBox);
USEFORM("AddSalesType.cpp", frmAddSalesType);
USEFORM("POS\GUIDiscount.cpp", frmDiscount);
USEFORM("MallExportResendReport.cpp", frmMallExportResendReport);
USEFORM("Security\Secure.cpp", frmSecurity);
USEFORM("Tables_ChitNumbers.cpp", frmTables_ChitNumbers);
USEFORM("MMdata.cpp", frmData);
USEFORM("SyndCodeGui.cpp", frmSyndCodeGui);
USEFORM("POS\SelectMember.cpp", frmSelectMember);
USEFORM("POS\Users.cpp", frmUsers);
USEFORM("POS\ForcedSides.cpp", frmSelectForcedSides);
USEFORM("Maintenance\MessageMaintenance.cpp", frmMessageMaintenance);
USEFORM("AutoZReportingSetup.cpp", frmAutoZReportingSetup);
USEFORM("POS\SplitPayment.cpp", frmSplitPayment);
USEFORM("CardSwipe.cpp", frmCardSwipe);
USEFORM("POS\ConfirmOrd.cpp", frmConfirmOrder);
USEFORM("POS\SelectActiveMenus.cpp", frmSelectActiveMenus);
USEFORM("MallExportRegenerateReport.cpp", frmMallExportRegenerateReport);
USEFORM("Timer.cpp", frmClockTime);
USEFORM("DropDown.cpp", frmDropDownFunc);
USEFORM("TabManagerGUI.cpp", frmTabManager);
USEFORM("POS\Transfer.cpp", frmTransfer);
USEFORM("POS\ForcedOptions.cpp", frmSelectForcedOptions);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMMessageBox.cpp", frmMessageBox);
USEFORM("Maintenance\UserMaintenance.cpp", frmUserMaintenance);
USEFORM("PHSConfiguration.cpp", frmPHSConfiguration);
USEFORM("POS\SelectKitchenDocket.cpp", frmSelectKitchenDocket);
USEFORM("POS\SelectDish.cpp", frmSelectDish);
USEFORM("POS\GetMoney.cpp", frmControlTransaction);
USEFORM("Security\Login.cpp", frmLogin);
USEFORM("PocketVoucher.cpp", frmPocketVoucher);
USEFORM("POS\HoldSend.cpp", frmHoldSend);
USEFORM("POS\GroupGUI.cpp", frmGroupGUI);
USEFORM("ProductSearch\ProductSearch.cpp", frmProductSearch);
USEFORM("POS\SelectZed.cpp", frmSelectZed);
USEFORM("PrinterRedirect.cpp", frmPrinterRedirect);
USEFORM("POS\SelectPhoenixRoom.cpp", frmPhoenixRoom);
USEFORM("Maintenance\EditCustomer.cpp", frmEditCustomer);
USEFORM("POS\Functions.cpp", frmFunctions);
USEFORM("DropDownVar.cpp", frmDropDownVar);
USEFORM("Comms\SerialConfig.cpp", frmSerialConfig);
USEFORM("POS\ShowPrintout.cpp", frmShowPrintout);
USEFORM("Maintenance\PrinterControlPanel.cpp", frmPrinterControlPanel);
USEFORM("DeskTop\VerticalSelect.cpp", frmVerticalSelect);
USEFORM("Maintenance\PrinterMaintenance.cpp", frmPrinterMaintenance);
USEFORM("DbMod.cpp", frmDBMod);
USEFORM("SmartCards\SmartCardConfigGUI.cpp", frmSmartCardConfiguration);
USEFORM("POS\SelectReceipt.cpp", frmSelectReceipt);
USEFORM("GenericGroupGUI.cpp", frmPaymentGroupGUI);
USEFORM("SelectTable2.cpp", FrmSelectTable2);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchKeyboard.cpp", frmTouchKeyboard);
USEFORM("SmartCards\SmartcardPreloader.cpp", Smartcard_Preloader);
USEFORM("POS\BillGroup.cpp", frmBillGroup);
USEFORM("DiscountEditor.cpp", frmDiscountEdit);
USEFORM("GenericGroupAssignment.cpp", frmGenericGroupAssignment);
USEFORM("Casino Security Screen\TransactionAuditScreen.cpp", frmTransactionAuditScreen);
USEFORM("ListManager.cpp", frmListManager);
USEFORM("SetButtonFontSize.cpp", frmSetButtonFontSize);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchNumpad.cpp", frmTouchNumpad);
USEFORM("POS\MemberCreation.cpp", frmMemberCreation);
USEFORM("POS\Scales.cpp", frmScales);
USEFORM("NewPaymentType.cpp", frmNewPaymentType);
USEFORM("POS\SelectPaymentTypeAndValue.cpp", frmSelectPaymentTypeAndValue);
USEFORM("SetupGlCodes.cpp", frmSetupGlCodes);
USEFORM("POS\PatronCount.cpp", frmPatronCount);
USEFORM("Maintenance\HappyHourSettings.cpp", frmHappyHourSettings);
USEFORM("POS\TransferItemOrGuest.cpp", frmTransferItemOrGuest);
USEFORM("IntaMateConfiguration.cpp", frmIntaMateConfiguration);
USEFORM("ReportDisplayNav.cpp", frmReportDisplayNav);
USEFORM("Casino Security Screen\MMSecurityScreenSettings.cpp", frmMMSecurityScreenSettings);
USEFORM("ParkedSales.cpp", frmParkedSales);
USEFORM("Processing.cpp", frmProcessing);
USEFORM("ProcessWebOrder.cpp", frmProcessWebOrder);
USEFORM("POS\SelectGlass.cpp", frmSelectGlass);
USEFORM("POS\AddTab.cpp", frmAddTab);
USEFORM("POS\SelectRoom.cpp", frmSelectRoom);
USEFORM("EftChequeVerify.cpp", frmChequeVerfiy);
USEFORM("ComboForm.cpp", frmComboForm);
USEFORM("DealEditor.cpp", frmDealEditorForm);
USEFORM("SelectDateAndTime.cpp", frmSelectDateTime);
USEFORM("CaptureCustomerDetails.cpp", frmCaptureDiscountDetails);
USEFORM("ShowEJournal.cpp", frmEJournal);
USEFORM("SetUpPosPlus.cpp", frmSetUpPosPlus);
USEFORM("RegenerateMallReport.cpp", frmRegenerateMallReport);
USEFORM("POS\SelectCurrentMenu.cpp", frmSelectCurrentMenu);
USEFORM("EftPosDialogs.cpp", frmEftPos);
USEFORM("POS\Paytype.cpp", frmPaymentType);
USEFORM("POS\TaxRemoval.cpp", frmTaxRemoval);
USEFORM("Maintenance\WalletConfiguration.cpp", frmWalletConfiguration);
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











