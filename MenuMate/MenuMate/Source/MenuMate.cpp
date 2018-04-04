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

USEFORM("DiscountTimes.cpp", frmDiscountTimes);
USEFORM("POS\PosMain.cpp", frmPOSMain);
USEFORM("DealsSetup.cpp", frmDeals);
USEFORM("SelectDiscounts.cpp", frmSelectDiscounts);
USEFORM("POS\MenuManagerItemSizeSelection.cpp", frmItemSizeSelection);
USEFORM("Maintenance\PaymentMaintenance.cpp", frmPaymentMaintenance);
USEFORM("SendForm.cpp", frmSendForm);
USEFORM("CaptNamePhone.cpp", frmCaptNamePhone);
USEFORM("TierLevels.cpp", frmTierLevel);
USEFORM("POS\SelectRMSRoom.cpp", frmRMSRoom);
USEFORM("POS\SelectItem.cpp", frmSelectItem);
USEFORM("DelayedPaymentTabs.cpp", frmDelayedPaymentTabs);
USEFORM("Chefmate\ChefMateSettings.cpp", frmChefmateSettings);
USEFORM("Locations.cpp", frmLocations);
USEFORM("TierLevelEditor.cpp", frmTierLevelEditor);
USEFORM("POS\AddTab.cpp", frmAddTab);
USEFORM("DealEditor.cpp", frmDealEditorForm);
USEFORM("ProcessWebOrder.cpp", frmProcessWebOrder);
USEFORM("POS\SelectRoom.cpp", frmSelectRoom);
USEFORM("SystemConfig.cpp", frmSystemConfig);
USEFORM("Security\TimeClock.cpp", frmTimeClock);
USEFORM("Main.cpp", frmMain);
USEFORM("POS\SelectTable.cpp", frmSelectTable);
USEFORM("VirtualPrintersInterface.cpp", frmVirtualPrintersInterface);
USEFORM("ReportDisplay.cpp", frmReportDisplay);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMColourPicker.cpp", frmColourPicker);
USEFORM("XeroSettings.cpp", frmXeroSettings);
USEFORM("POS\MenuManager.cpp", frmMenuManager);
USEFORM("ChitList.cpp", frmChitList);
USEFORM("TerminalChitConfig.cpp", frmTerminalChitConfig);
USEFORM("ChitNumberGui.cpp", frmChitNumberGui);
USEFORM("POS\Message.cpp", frmMessage);
USEFORM("SelectSaveOption.cpp", frmSelectSaveOption);
USEFORM("Setup.cpp", frmSetup);
USEFORM("DealLevelsEditor.cpp", Form1);
USEFORM("PSectionInstructionEdit.cpp", frmPSectionInstructionEdit);
USEFORM("Maintenance\GeneralMaintenance.cpp", frmGeneralMaintenance);
USEFORM("POS\MenuManagerItemSettings.cpp", frmItemSettings);
USEFORM("Maintenance\TaxMaintenance.cpp", frmTaxMaintenance);
USEFORM("Maintenance\NewUser.cpp", frmNewUser);
USEFORM("GuestList.cpp", Form2);
USEFORM("ServingTime.cpp", frmServingTime);
USEFORM("MallSalesTypeAssignment.cpp", frmMallSalesTypeAssignment);
USEFORM("SelectSizesAsList.cpp", frmSelectSizes);
USEFORM("LoyaltyMateOperationDialogBox.cpp", frmLoyaltyMateOperationDialogBox);
USEFORM("MallExportResendReport.cpp", frmMallExportResendReport);
USEFORM("AddSalesType.cpp", frmAddSalesType);
USEFORM("POS\GUIDiscount.cpp", frmDiscount);
USEFORM("Tables_ChitNumbers.cpp", frmTables_ChitNumbers);
USEFORM("Security\Secure.cpp", frmSecurity);
USEFORM("MMdata.cpp", frmData);
USEFORM("POS\SelectMember.cpp", frmSelectMember);
USEFORM("POS\ForcedSides.cpp", frmSelectForcedSides);
USEFORM("AutoZReportingSetup.cpp", frmAutoZReportingSetup);
USEFORM("Maintain.cpp", frmMaintain);
USEFORM("POS\SplitPayment.cpp", frmSplitPayment);
USEFORM("SyndCodeGui.cpp", frmSyndCodeGui);
USEFORM("Discounts.cpp", frmDiscounts);
USEFORM("Timer.cpp", frmClockTime);
USEFORM("CardSwipe.cpp", frmCardSwipe);
USEFORM("MallExportRegenerateReport.cpp", frmMallExportRegenerateReport);
USEFORM("POS\SelectActiveMenus.cpp", frmSelectActiveMenus);
USEFORM("POS\Users.cpp", frmUsers);
USEFORM("POS\ConfirmOrd.cpp", frmConfirmOrder);
USEFORM("Maintenance\MessageMaintenance.cpp", frmMessageMaintenance);
USEFORM("TabManagerGUI.cpp", frmTabManager);
USEFORM("POS\Transfer.cpp", frmTransfer);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMMessageBox.cpp", frmMessageBox);
USEFORM("DropDown.cpp", frmDropDownFunc);
USEFORM("POS\ForcedOptions.cpp", frmSelectForcedOptions);
USEFORM("Maintenance\UserMaintenance.cpp", frmUserMaintenance);
USEFORM("PocketVoucher.cpp", frmPocketVoucher);
USEFORM("POS\SelectDish.cpp", frmSelectDish);
USEFORM("POS\GetMoney.cpp", frmControlTransaction);
USEFORM("POS\GroupGUI.cpp", frmGroupGUI);
USEFORM("Security\Login.cpp", frmLogin);
USEFORM("POS\ShowPrintout.cpp", frmShowPrintout);
USEFORM("POS\HoldSend.cpp", frmHoldSend);
USEFORM("PrinterRedirect.cpp", frmPrinterRedirect);
USEFORM("Comms\SerialConfig.cpp", frmSerialConfig);
USEFORM("PHSConfiguration.cpp", frmPHSConfiguration);
USEFORM("Maintenance\EditCustomer.cpp", frmEditCustomer);
USEFORM("DropDownVar.cpp", frmDropDownVar);
USEFORM("POS\SelectKitchenDocket.cpp", frmSelectKitchenDocket);
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
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchKeyboard.cpp", frmTouchKeyboard);
USEFORM("SmartCards\SmartcardPreloader.cpp", Smartcard_Preloader);
USEFORM("POS\BillGroup.cpp", frmBillGroup);
USEFORM("DiscountEditor.cpp", frmDiscountEdit);
USEFORM("Casino Security Screen\TransactionAuditScreen.cpp", frmTransactionAuditScreen);
USEFORM("ListManager.cpp", frmListManager);
USEFORM("GenericGroupAssignment.cpp", frmGenericGroupAssignment);
USEFORM("SetButtonFontSize.cpp", frmSetButtonFontSize);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchNumpad.cpp", frmTouchNumpad);
USEFORM("POS\MemberCreation.cpp", frmMemberCreation);
USEFORM("POS\Scales.cpp", frmScales);
USEFORM("NewPaymentType.cpp", frmNewPaymentType);
USEFORM("POS\SelectPaymentTypeAndValue.cpp", frmSelectPaymentTypeAndValue);
USEFORM("Casino Security Screen\MMSecurityScreenSettings.cpp", frmMMSecurityScreenSettings);
USEFORM("SetupGlCodes.cpp", frmSetupGlCodes);
USEFORM("ReportDisplayNav.cpp", frmReportDisplayNav);
USEFORM("POS\SelectZed.cpp", frmSelectZed);
USEFORM("POS\Paytype.cpp", frmPaymentType);
USEFORM("IntaMateConfiguration.cpp", frmIntaMateConfiguration);
USEFORM("Processing.cpp", frmProcessing);
USEFORM("POS\PatronCount.cpp", frmPatronCount);
USEFORM("Maintenance\HappyHourSettings.cpp", frmHappyHourSettings);
USEFORM("EftChequeVerify.cpp", frmChequeVerfiy);
USEFORM("POS\SelectGlass.cpp", frmSelectGlass);
USEFORM("POS\TransferItemOrGuest.cpp", frmTransferItemOrGuest);
USEFORM("ComboForm.cpp", frmComboForm);
USEFORM("ParkedSales.cpp", frmParkedSales);
USEFORM("SelectDateAndTime.cpp", frmSelectDateTime);
USEFORM("ProductSearch\ProductSearch.cpp", frmProductSearch);
USEFORM("CaptureCustomerDetails.cpp", frmCaptureDiscountDetails);
USEFORM("ShowEJournal.cpp", frmEJournal);
USEFORM("POS\SelectCurrentMenu.cpp", frmSelectCurrentMenu);
USEFORM("POS\TaxRemoval.cpp", frmTaxRemoval);
USEFORM("RegenerateMallReport.cpp", frmRegenerateMallReport);
USEFORM("EftPosDialogs.cpp", frmEftPos);
USEFORM("SetUpPosPlus.cpp", frmSetUpPosPlus);
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










