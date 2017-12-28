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

USEFORM("Maintenance\PaymentMaintenance.cpp", frmPaymentMaintenance);
USEFORM("POS\MenuManagerItemSizeSelection.cpp", frmItemSizeSelection);
USEFORM("DiscountTimes.cpp", frmDiscountTimes);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMColourPicker.cpp", frmColourPicker);
USEFORM("TerminalChitConfig.cpp", frmTerminalChitConfig);
USEFORM("DealsSetup.cpp", frmDeals);
USEFORM("SelectDiscounts.cpp", frmSelectDiscounts);
USEFORM("POS\SelectRMSRoom.cpp", frmRMSRoom);
USEFORM("DelayedPaymentTabs.cpp", frmDelayedPaymentTabs);
USEFORM("POS\SelectItem.cpp", frmSelectItem);
USEFORM("CaptNamePhone.cpp", frmCaptNamePhone);
USEFORM("SendForm.cpp", frmSendForm);
USEFORM("POS\PosMain.cpp", frmPOSMain);
USEFORM("TierLevels.cpp", frmTierLevel);
USEFORM("VirtualPrintersInterface.cpp", frmVirtualPrintersInterface);
USEFORM("Main.cpp", frmMain);
USEFORM("POS\SelectTable.cpp", frmSelectTable);
USEFORM("SystemConfig.cpp", frmSystemConfig);
USEFORM("Security\TimeClock.cpp", frmTimeClock);
USEFORM("Maintenance\MessageMaintenance.cpp", frmMessageMaintenance);
USEFORM("Maintenance\HappyHourSettings.cpp", frmHappyHourSettings);
USEFORM("POS\SelectRoom.cpp", frmSelectRoom);
USEFORM("POS\AddTab.cpp", frmAddTab);
USEFORM("ProcessWebOrder.cpp", frmProcessWebOrder);
USEFORM("POS\Users.cpp", frmUsers);
USEFORM("DealEditor.cpp", frmDealEditorForm);
USEFORM("DealLevelsEditor.cpp", Form1);
USEFORM("ChitList.cpp", frmChitList);
USEFORM("XeroSettings.cpp", frmXeroSettings);
USEFORM("POS\MenuManager.cpp", frmMenuManager);
USEFORM("Setup.cpp", frmSetup);
USEFORM("Locations.cpp", frmLocations);
USEFORM("Chefmate\ChefMateSettings.cpp", frmChefmateSettings);
USEFORM("TierLevelEditor.cpp", frmTierLevelEditor);
USEFORM("POS\Message.cpp", frmMessage);
USEFORM("ReportDisplay.cpp", frmReportDisplay);
USEFORM("Maintenance\NewUser.cpp", frmNewUser);
USEFORM("MallExportRegenerateReport.cpp", frmMallExportRegenerateReport);
USEFORM("POS\MenuManagerItemSettings.cpp", frmItemSettings);
USEFORM("Maintenance\TaxMaintenance.cpp", frmTaxMaintenance);
USEFORM("Timer.cpp", frmClockTime);
USEFORM("AddSalesType.cpp", frmAddSalesType);
USEFORM("GuestList.cpp", Form2);
USEFORM("MallExportResendReport.cpp", frmMallExportResendReport);
USEFORM("MallSalesTypeAssignment.cpp", frmMallSalesTypeAssignment);
USEFORM("SelectSizesAsList.cpp", frmSelectSizes);
USEFORM("PSectionInstructionEdit.cpp", frmPSectionInstructionEdit);
USEFORM("Maintenance\GeneralMaintenance.cpp", frmGeneralMaintenance);
USEFORM("LoyaltyMateOperationDialogBox.cpp", frmLoyaltyMateOperationDialogBox);
USEFORM("POS\ForcedSides.cpp", frmSelectForcedSides);
USEFORM("AutoZReportingSetup.cpp", frmAutoZReportingSetup);
USEFORM("Security\Secure.cpp", frmSecurity);
USEFORM("Maintain.cpp", frmMaintain);
USEFORM("Tables_ChitNumbers.cpp", frmTables_ChitNumbers);
USEFORM("POS\GUIDiscount.cpp", frmDiscount);
USEFORM("SelectSaveOption.cpp", frmSelectSaveOption);
USEFORM("ChitNumberGui.cpp", frmChitNumberGui);
USEFORM("POS\ConfirmOrd.cpp", frmConfirmOrder);
USEFORM("POS\SelectMember.cpp", frmSelectMember);
USEFORM("SyndCodeGui.cpp", frmSyndCodeGui);
USEFORM("POS\Transfer.cpp", frmTransfer);
USEFORM("Discounts.cpp", frmDiscounts);
USEFORM("MMdata.cpp", frmData);
USEFORM("POS\SplitPayment.cpp", frmSplitPayment);
USEFORM("POS\PatronCount.cpp", frmPatronCount);
USEFORM("CardSwipe.cpp", frmCardSwipe);
USEFORM("ParkedSales.cpp", frmParkedSales);
USEFORM("POS\SelectActiveMenus.cpp", frmSelectActiveMenus);
USEFORM("TabManagerGUI.cpp", frmTabManager);
USEFORM("EftChequeVerify.cpp", frmChequeVerfiy);
USEFORM("DropDown.cpp", frmDropDownFunc);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMMessageBox.cpp", frmMessageBox);
USEFORM("POS\ForcedOptions.cpp", frmSelectForcedOptions);
USEFORM("PocketVoucher.cpp", frmPocketVoucher);
USEFORM("Maintenance\UserMaintenance.cpp", frmUserMaintenance);
USEFORM("POS\SelectDish.cpp", frmSelectDish);
USEFORM("Security\Login.cpp", frmLogin);
USEFORM("POS\GroupGUI.cpp", frmGroupGUI);
USEFORM("POS\GetMoney.cpp", frmControlTransaction);
USEFORM("POS\HoldSend.cpp", frmHoldSend);
USEFORM("POS\SelectKitchenDocket.cpp", frmSelectKitchenDocket);
USEFORM("Comms\SerialConfig.cpp", frmSerialConfig);
USEFORM("Maintenance\EditCustomer.cpp", frmEditCustomer);
USEFORM("DropDownVar.cpp", frmDropDownVar);
USEFORM("POS\ShowPrintout.cpp", frmShowPrintout);
USEFORM("PrinterRedirect.cpp", frmPrinterRedirect);
USEFORM("DiscountEditor.cpp", frmDiscountEdit);
USEFORM("PHSConfiguration.cpp", frmPHSConfiguration);
USEFORM("POS\Functions.cpp", frmFunctions);
USEFORM("POS\SelectPhoenixRoom.cpp", frmPhoenixRoom);
USEFORM("Maintenance\PrinterControlPanel.cpp", frmPrinterControlPanel);
USEFORM("DeskTop\VerticalSelect.cpp", frmVerticalSelect);
USEFORM("Maintenance\PrinterMaintenance.cpp", frmPrinterMaintenance);
USEFORM("DbMod.cpp", frmDBMod);
USEFORM("SmartCards\SmartCardConfigGUI.cpp", frmSmartCardConfiguration);
USEFORM("POS\SelectReceipt.cpp", frmSelectReceipt);
USEFORM("GenericGroupGUI.cpp", frmPaymentGroupGUI);
USEFORM("SelectTable2.cpp", FrmSelectTable2);
USEFORM("SmartCards\SmartcardPreloader.cpp", Smartcard_Preloader);
USEFORM("POS\BillGroup.cpp", frmBillGroup);
USEFORM("Casino Security Screen\TransactionAuditScreen.cpp", frmTransactionAuditScreen);
USEFORM("ListManager.cpp", frmListManager);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchKeyboard.cpp", frmTouchKeyboard);
USEFORM("GenericGroupAssignment.cpp", frmGenericGroupAssignment);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchNumpad.cpp", frmTouchNumpad);
USEFORM("SetButtonFontSize.cpp", frmSetButtonFontSize);
USEFORM("POS\MemberCreation.cpp", frmMemberCreation);
USEFORM("NewPaymentType.cpp", frmNewPaymentType);
USEFORM("POS\Scales.cpp", frmScales);
USEFORM("POS\SelectCurrentMenu.cpp", frmSelectCurrentMenu);
USEFORM("EftPosDialogs.cpp", frmEftPos);
USEFORM("POS\Paytype.cpp", frmPaymentType);
USEFORM("Processing.cpp", frmProcessing);
USEFORM("IntaMateConfiguration.cpp", frmIntaMateConfiguration);
USEFORM("ReportDisplayNav.cpp", frmReportDisplayNav);
USEFORM("POS\SelectZed.cpp", frmSelectZed);
USEFORM("POS\TransferItemOrGuest.cpp", frmTransferItemOrGuest);
USEFORM("Casino Security Screen\MMSecurityScreenSettings.cpp", frmMMSecurityScreenSettings);
USEFORM("POS\SelectPaymentTypeAndValue.cpp", frmSelectPaymentTypeAndValue);
USEFORM("POS\SelectGlass.cpp", frmSelectGlass);
USEFORM("SetupGlCodes.cpp", frmSetupGlCodes);
USEFORM("ComboForm.cpp", frmComboForm);
USEFORM("SelectDateAndTime.cpp", frmSelectDateTime);
USEFORM("CaptureCustomerDetails.cpp", frmCaptureDiscountDetails);
USEFORM("ShowEJournal.cpp", frmEJournal);
USEFORM("POS\TaxRemoval.cpp", frmTaxRemoval);
USEFORM("ProductSearch\ProductSearch.cpp", frmProductSearch);
USEFORM("Maintenance\WalletConfiguration.cpp", frmWalletConfiguration);
USEFORM("RegenerateMallReport.cpp", frmRegenerateMallReport);
USEFORM("Analysis.cpp", frmAnalysis);
USEFORM("SetUpPosPlus.cpp", frmSetUpPosPlus);
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










