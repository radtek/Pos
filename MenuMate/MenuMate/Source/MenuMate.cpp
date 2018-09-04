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
USEFORM("TierLevels.cpp", frmTierLevel);
USEFORM("Maintenance\PaymentMaintenance.cpp", frmPaymentMaintenance);
USEFORM("POS\MenuManagerItemSizeSelection.cpp", frmItemSizeSelection);
USEFORM("DiscountTimes.cpp", frmDiscountTimes);
USEFORM("CaptNamePhone.cpp", frmCaptNamePhone);
USEFORM("Discounts.cpp", frmDiscounts);
USEFORM("Maintain.cpp", frmMaintain);
USEFORM("POS\SelectRMSRoom.cpp", frmRMSRoom);
USEFORM("DelayedPaymentTabs.cpp", frmDelayedPaymentTabs);
USEFORM("POS\SelectItem.cpp", frmSelectItem);
USEFORM("SendForm.cpp", frmSendForm);
USEFORM("SelectSaveOption.cpp", frmSelectSaveOption);
USEFORM("ChitNumberGui.cpp", frmChitNumberGui);
USEFORM("POS\Message.cpp", frmMessage);
USEFORM("ReportDisplay.cpp", frmReportDisplay);
USEFORM("Locations.cpp", frmLocations);
USEFORM("Chefmate\ChefMateSettings.cpp", frmChefmateSettings);
USEFORM("VirtualPrintersInterface.cpp", frmVirtualPrintersInterface);
USEFORM("SystemConfig.cpp", frmSystemConfig);
USEFORM("TierLevelEditor.cpp", frmTierLevelEditor);
USEFORM("Security\TimeClock.cpp", frmTimeClock);
USEFORM("Main.cpp", frmMain);
USEFORM("SelectDiscounts.cpp", frmSelectDiscounts);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMColourPicker.cpp", frmColourPicker);
USEFORM("TerminalChitConfig.cpp", frmTerminalChitConfig);
USEFORM("DealsSetup.cpp", frmDeals);
USEFORM("XeroSettings.cpp", frmXeroSettings);
USEFORM("ChitList.cpp", frmChitList);
USEFORM("DealLevelsEditor.cpp", Form1);
USEFORM("POS\MenuManager.cpp", frmMenuManager);
USEFORM("Setup.cpp", frmSetup);
USEFORM("Maintenance\GeneralMaintenance.cpp", frmGeneralMaintenance);
USEFORM("MallSalesTypeAssignment.cpp", frmMallSalesTypeAssignment);
USEFORM("POS\GUIDiscount.cpp", frmDiscount);
USEFORM("AddSalesType.cpp", frmAddSalesType);
USEFORM("POS\MenuManagerItemSettings.cpp", frmItemSettings);
USEFORM("Maintenance\NewUser.cpp", frmNewUser);
USEFORM("PSectionInstructionEdit.cpp", frmPSectionInstructionEdit);
USEFORM("Maintenance\TaxMaintenance.cpp", frmTaxMaintenance);
USEFORM("Analysis.cpp", frmAnalysis);
USEFORM("EFTPOSConfiguration.cpp", frmEFTPOSConfig);
USEFORM("SelectSizesAsList.cpp", frmSelectSizes);
USEFORM("LoyaltyMateOperationDialogBox.cpp", frmLoyaltyMateOperationDialogBox);
USEFORM("GuestList.cpp", Form2);
USEFORM("ServingTime.cpp", frmServingTime);
USEFORM("MallExportResendReport.cpp", frmMallExportResendReport);
USEFORM("Security\Secure.cpp", frmSecurity);
USEFORM("SyndCodeGui.cpp", frmSyndCodeGui);
USEFORM("MMdata.cpp", frmData);
USEFORM("Tables_ChitNumbers.cpp", frmTables_ChitNumbers);
USEFORM("POS\SelectMember.cpp", frmSelectMember);
USEFORM("POS\Users.cpp", frmUsers);
USEFORM("POS\ForcedSides.cpp", frmSelectForcedSides);
USEFORM("Maintenance\MessageMaintenance.cpp", frmMessageMaintenance);
USEFORM("AutoZReportingSetup.cpp", frmAutoZReportingSetup);
USEFORM("POS\SplitPayment.cpp", frmSplitPayment);
USEFORM("CardSwipe.cpp", frmCardSwipe);
USEFORM("Timer.cpp", frmClockTime);
USEFORM("POS\SelectActiveMenus.cpp", frmSelectActiveMenus);
USEFORM("POS\ConfirmOrd.cpp", frmConfirmOrder);
USEFORM("MallExportRegenerateReport.cpp", frmMallExportRegenerateReport);
USEFORM("POS\Transfer.cpp", frmTransfer);
USEFORM("DropDown.cpp", frmDropDownFunc);
USEFORM("TabManagerGUI.cpp", frmTabManager);
USEFORM("POS\ForcedOptions.cpp", frmSelectForcedOptions);
USEFORM("Maintenance\UserMaintenance.cpp", frmUserMaintenance);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMMessageBox.cpp", frmMessageBox);
USEFORM("PHSConfiguration.cpp", frmPHSConfiguration);
USEFORM("POS\SelectKitchenDocket.cpp", frmSelectKitchenDocket);
USEFORM("POS\SelectDish.cpp", frmSelectDish);
USEFORM("Security\Login.cpp", frmLogin);
USEFORM("PocketVoucher.cpp", frmPocketVoucher);
USEFORM("POS\GetMoney.cpp", frmControlTransaction);
USEFORM("POS\GroupGUI.cpp", frmGroupGUI);
USEFORM("POS\HoldSend.cpp", frmHoldSend);
USEFORM("ProductSearch\ProductSearch.cpp", frmProductSearch);
USEFORM("PrinterRedirect.cpp", frmPrinterRedirect);
USEFORM("POS\SelectPhoenixRoom.cpp", frmPhoenixRoom);
USEFORM("Maintenance\EditCustomer.cpp", frmEditCustomer);
USEFORM("Comms\SerialConfig.cpp", frmSerialConfig);
USEFORM("POS\Functions.cpp", frmFunctions);
USEFORM("DropDownVar.cpp", frmDropDownVar);
USEFORM("POS\ShowPrintout.cpp", frmShowPrintout);
USEFORM("Maintenance\PrinterControlPanel.cpp", frmPrinterControlPanel);
USEFORM("DeskTop\VerticalSelect.cpp", frmVerticalSelect);
USEFORM("Maintenance\PrinterMaintenance.cpp", frmPrinterMaintenance);
USEFORM("DbMod.cpp", frmDBMod);
USEFORM("SmartCards\SmartCardConfigGUI.cpp", frmSmartCardConfiguration);
USEFORM("GenericGroupGUI.cpp", frmPaymentGroupGUI);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchKeyboard.cpp", frmTouchKeyboard);
USEFORM("SelectTable2.cpp", FrmSelectTable2);
USEFORM("SmartCards\SmartcardPreloader.cpp", Smartcard_Preloader);
USEFORM("POS\BillGroup.cpp", frmBillGroup);
USEFORM("GenericGroupAssignment.cpp", frmGenericGroupAssignment);
USEFORM("DiscountEditor.cpp", frmDiscountEdit);
USEFORM("POS\SelectReceipt.cpp", frmSelectReceipt);
USEFORM("ListManager.cpp", frmListManager);
USEFORM("Casino Security Screen\TransactionAuditScreen.cpp", frmTransactionAuditScreen);
USEFORM("SetButtonFontSize.cpp", frmSetButtonFontSize);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchNumpad.cpp", frmTouchNumpad);
USEFORM("NewPaymentType.cpp", frmNewPaymentType);
USEFORM("POS\MemberCreation.cpp", frmMemberCreation);
USEFORM("POS\Scales.cpp", frmScales);
USEFORM("POS\SelectZed.cpp", frmSelectZed);
USEFORM("POS\PatronCount.cpp", frmPatronCount);
USEFORM("SetupGlCodes.cpp", frmSetupGlCodes);
USEFORM("POS\SelectPaymentTypeAndValue.cpp", frmSelectPaymentTypeAndValue);
USEFORM("POS\TransferItemOrGuest.cpp", frmTransferItemOrGuest);
USEFORM("Maintenance\HappyHourSettings.cpp", frmHappyHourSettings);
USEFORM("IntaMateConfiguration.cpp", frmIntaMateConfiguration);
USEFORM("ReportDisplayNav.cpp", frmReportDisplayNav);
USEFORM("ParkedSales.cpp", frmParkedSales);
USEFORM("Casino Security Screen\MMSecurityScreenSettings.cpp", frmMMSecurityScreenSettings);
USEFORM("Processing.cpp", frmProcessing);
USEFORM("POS\AddTab.cpp", frmAddTab);
USEFORM("POS\SelectRoom.cpp", frmSelectRoom);
USEFORM("POS\SelectGlass.cpp", frmSelectGlass);
USEFORM("ProcessWebOrder.cpp", frmProcessWebOrder);
USEFORM("POS\SelectTable.cpp", frmSelectTable);
USEFORM("EftChequeVerify.cpp", frmChequeVerfiy);
USEFORM("ComboForm.cpp", frmComboForm);
USEFORM("DealEditor.cpp", frmDealEditorForm);
USEFORM("SelectDateAndTime.cpp", frmSelectDateTime);
USEFORM("ShowEJournal.cpp", frmEJournal);
USEFORM("SetUpPosPlus.cpp", frmSetUpPosPlus);
USEFORM("CaptureCustomerDetails.cpp", frmCaptureDiscountDetails);
USEFORM("POS\SelectCurrentMenu.cpp", frmSelectCurrentMenu);
USEFORM("EftPosDialogs.cpp", frmEftPos);
USEFORM("POS\Paytype.cpp", frmPaymentType);
USEFORM("POS\TaxRemoval.cpp", frmTaxRemoval);
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











