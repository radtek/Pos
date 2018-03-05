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

USEFORM("DbMod.cpp", frmDBMod);
USEFORM("SmartCards\SmartCardConfigGUI.cpp", frmSmartCardConfiguration);
USEFORM("Maintenance\PrinterMaintenance.cpp", frmPrinterMaintenance);
USEFORM("SelectTable2.cpp", FrmSelectTable2);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchKeyboard.cpp", frmTouchKeyboard);
USEFORM("GenericGroupGUI.cpp", frmPaymentGroupGUI);
USEFORM("POS\SelectReceipt.cpp", frmSelectReceipt);
USEFORM("POS\Functions.cpp", frmFunctions);
USEFORM("POS\SelectPhoenixRoom.cpp", frmPhoenixRoom);
USEFORM("Maintenance\PrinterControlPanel.cpp", frmPrinterControlPanel);
USEFORM("DeskTop\VerticalSelect.cpp", frmVerticalSelect);
USEFORM("Casino Security Screen\TransactionAuditScreen.cpp", frmTransactionAuditScreen);
USEFORM("ListManager.cpp", frmListManager);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchNumpad.cpp", frmTouchNumpad);
USEFORM("SetButtonFontSize.cpp", frmSetButtonFontSize);
USEFORM("GenericGroupAssignment.cpp", frmGenericGroupAssignment);
USEFORM("SmartCards\SmartcardPreloader.cpp", Smartcard_Preloader);
USEFORM("POS\BillGroup.cpp", frmBillGroup);
USEFORM("DiscountEditor.cpp", frmDiscountEdit);
USEFORM("PHSConfiguration.cpp", frmPHSConfiguration);
USEFORM("POS\ForcedOptions.cpp", frmSelectForcedOptions);
USEFORM("Maintenance\UserMaintenance.cpp", frmUserMaintenance);
USEFORM("PocketVoucher.cpp", frmPocketVoucher);
USEFORM("EftChequeVerify.cpp", frmChequeVerfiy);
USEFORM("ParkedSales.cpp", frmParkedSales);
USEFORM("CardSwipe.cpp", frmCardSwipe);
USEFORM("POS\SelectActiveMenus.cpp", frmSelectActiveMenus);
USEFORM("DropDown.cpp", frmDropDownFunc);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMMessageBox.cpp", frmMessageBox);
USEFORM("POS\SelectKitchenDocket.cpp", frmSelectKitchenDocket);
USEFORM("Maintenance\EditCustomer.cpp", frmEditCustomer);
USEFORM("PrinterRedirect.cpp", frmPrinterRedirect);
USEFORM("DropDownVar.cpp", frmDropDownVar);
USEFORM("Comms\SerialConfig.cpp", frmSerialConfig);
USEFORM("Security\Login.cpp", frmLogin);
USEFORM("POS\GetMoney.cpp", frmControlTransaction);
USEFORM("POS\GroupGUI.cpp", frmGroupGUI);
USEFORM("POS\SelectDish.cpp", frmSelectDish);
USEFORM("POS\HoldSend.cpp", frmHoldSend);
USEFORM("POS\ShowPrintout.cpp", frmShowPrintout);
USEFORM("POS\MemberCreation.cpp", frmMemberCreation);
USEFORM("SelectDateAndTime.cpp", frmSelectDateTime);
USEFORM("ComboForm.cpp", frmComboForm);
USEFORM("Maintenance\WalletConfiguration.cpp", frmWalletConfiguration);
USEFORM("Analysis.cpp", frmAnalysis);
USEFORM("RegenerateMallReport.cpp", frmRegenerateMallReport);
USEFORM("SetUpPosPlus.cpp", frmSetUpPosPlus);
USEFORM("CaptureCustomerDetails.cpp", frmCaptureDiscountDetails);
USEFORM("ShowEJournal.cpp", frmEJournal);
USEFORM("ProductSearch\ProductSearch.cpp", frmProductSearch);
USEFORM("POS\TaxRemoval.cpp", frmTaxRemoval);
USEFORM("EftPosDialogs.cpp", frmEftPos);
USEFORM("POS\SelectCurrentMenu.cpp", frmSelectCurrentMenu);
USEFORM("NewPaymentType.cpp", frmNewPaymentType);
USEFORM("POS\Scales.cpp", frmScales);
USEFORM("Casino Security Screen\MMSecurityScreenSettings.cpp", frmMMSecurityScreenSettings);
USEFORM("ReportDisplayNav.cpp", frmReportDisplayNav);
USEFORM("SetupGlCodes.cpp", frmSetupGlCodes);
USEFORM("POS\SelectZed.cpp", frmSelectZed);
USEFORM("POS\SelectPaymentTypeAndValue.cpp", frmSelectPaymentTypeAndValue);
USEFORM("POS\SelectGlass.cpp", frmSelectGlass);
USEFORM("POS\TransferItemOrGuest.cpp", frmTransferItemOrGuest);
USEFORM("IntaMateConfiguration.cpp", frmIntaMateConfiguration);
USEFORM("POS\Paytype.cpp", frmPaymentType);
USEFORM("POS\PatronCount.cpp", frmPatronCount);
USEFORM("Processing.cpp", frmProcessing);
USEFORM("TabManagerGUI.cpp", frmTabManager);
USEFORM("Maintenance\HappyHourSettings.cpp", frmHappyHourSettings);
USEFORM("Maintenance\MessageMaintenance.cpp", frmMessageMaintenance);
USEFORM("POS\Users.cpp", frmUsers);
USEFORM("DealEditor.cpp", frmDealEditorForm);
USEFORM("POS\SelectRoom.cpp", frmSelectRoom);
USEFORM("ProcessWebOrder.cpp", frmProcessWebOrder);
USEFORM("POS\AddTab.cpp", frmAddTab);
USEFORM("Main.cpp", frmMain);
USEFORM("VirtualPrintersInterface.cpp", frmVirtualPrintersInterface);
USEFORM("SystemConfig.cpp", frmSystemConfig);
USEFORM("POS\SelectTable.cpp", frmSelectTable);
USEFORM("Security\TimeClock.cpp", frmTimeClock);
USEFORM("Chefmate\ChefMateSettings.cpp", frmChefmateSettings);
USEFORM("ReportDisplay.cpp", frmReportDisplay);
USEFORM("TierLevelEditor.cpp", frmTierLevelEditor);
USEFORM("Locations.cpp", frmLocations);
USEFORM("POS\Message.cpp", frmMessage);
USEFORM("ChitList.cpp", frmChitList);
USEFORM("DealLevelsEditor.cpp", Form1);
USEFORM("Setup.cpp", frmSetup);
USEFORM("XeroSettings.cpp", frmXeroSettings);
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
USEFORM("POS\SelectItem.cpp", frmSelectItem);
USEFORM("DelayedPaymentTabs.cpp", frmDelayedPaymentTabs);
USEFORM("POS\SelectRMSRoom.cpp", frmRMSRoom);
USEFORM("SendForm.cpp", frmSendForm);
USEFORM("ChitNumberGui.cpp", frmChitNumberGui);
USEFORM("SelectSaveOption.cpp", frmSelectSaveOption);
USEFORM("POS\ForcedSides.cpp", frmSelectForcedSides);
USEFORM("Security\Secure.cpp", frmSecurity);
USEFORM("AutoZReportingSetup.cpp", frmAutoZReportingSetup);
USEFORM("LoyaltyMateOperationDialogBox.cpp", frmLoyaltyMateOperationDialogBox);
USEFORM("POS\GUIDiscount.cpp", frmDiscount);
USEFORM("Maintain.cpp", frmMaintain);
USEFORM("Tables_ChitNumbers.cpp", frmTables_ChitNumbers);
USEFORM("POS\SplitPayment.cpp", frmSplitPayment);
USEFORM("POS\Transfer.cpp", frmTransfer);
USEFORM("MMdata.cpp", frmData);
USEFORM("POS\SelectMember.cpp", frmSelectMember);
USEFORM("POS\ConfirmOrd.cpp", frmConfirmOrder);
USEFORM("Discounts.cpp", frmDiscounts);
USEFORM("SyndCodeGui.cpp", frmSyndCodeGui);
USEFORM("Timer.cpp", frmClockTime);
USEFORM("MallExportRegenerateReport.cpp", frmMallExportRegenerateReport);
USEFORM("POS\MenuManagerItemSettings.cpp", frmItemSettings);
USEFORM("Maintenance\NewUser.cpp", frmNewUser);
USEFORM("Maintenance\TaxMaintenance.cpp", frmTaxMaintenance);
USEFORM("Maintenance\GeneralMaintenance.cpp", frmGeneralMaintenance);
USEFORM("SelectSizesAsList.cpp", frmSelectSizes);
USEFORM("PSectionInstructionEdit.cpp", frmPSectionInstructionEdit);
USEFORM("AddSalesType.cpp", frmAddSalesType);
USEFORM("MallSalesTypeAssignment.cpp", frmMallSalesTypeAssignment);
USEFORM("GuestList.cpp", Form2);
USEFORM("MallExportResendReport.cpp", frmMallExportResendReport);
USEFORM("ServingTime.cpp", frmServingTime);
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










