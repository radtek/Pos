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

USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchKeyboard.cpp", frmTouchKeyboard);
USEFORM("SmartCards\SmartcardPreloader.cpp", Smartcard_Preloader);
USEFORM("SelectTable2.cpp", FrmSelectTable2);
USEFORM("POS\BillGroup.cpp", frmBillGroup);
USEFORM("GenericGroupGUI.cpp", frmPaymentGroupGUI);
USEFORM("DeskTop\VerticalSelect.cpp", frmVerticalSelect);
USEFORM("Maintenance\PrinterMaintenance.cpp", frmPrinterMaintenance);
USEFORM("Maintenance\PrinterControlPanel.cpp", frmPrinterControlPanel);
USEFORM("POS\SelectReceipt.cpp", frmSelectReceipt);
USEFORM("SmartCards\SmartCardConfigGUI.cpp", frmSmartCardConfiguration);
USEFORM("DbMod.cpp", frmDBMod);
USEFORM("POS\MemberCreation.cpp", frmMemberCreation);
USEFORM("NewPaymentType.cpp", frmNewPaymentType);
USEFORM("POS\Scales.cpp", frmScales);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchNumpad.cpp", frmTouchNumpad);
USEFORM("DiscountEditor.cpp", frmDiscountEdit);
USEFORM("GenericGroupAssignment.cpp", frmGenericGroupAssignment);
USEFORM("ListManager.cpp", frmListManager);
USEFORM("SetButtonFontSize.cpp", frmSetButtonFontSize);
USEFORM("Casino Security Screen\TransactionAuditScreen.cpp", frmTransactionAuditScreen);
USEFORM("POS\SelectDish.cpp", frmSelectDish);
USEFORM("POS\GroupGUI.cpp", frmGroupGUI);
USEFORM("Security\Login.cpp", frmLogin);
USEFORM("POS\GetMoney.cpp", frmControlTransaction);
USEFORM("PocketVoucher.cpp", frmPocketVoucher);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMMessageBox.cpp", frmMessageBox);
USEFORM("POS\ForcedOptions.cpp", frmSelectForcedOptions);
USEFORM("Maintenance\UserMaintenance.cpp", frmUserMaintenance);
USEFORM("DropDown.cpp", frmDropDownFunc);
USEFORM("POS\SelectKitchenDocket.cpp", frmSelectKitchenDocket);
USEFORM("PHSConfiguration.cpp", frmPHSConfiguration);
USEFORM("DropDownVar.cpp", frmDropDownVar);
USEFORM("Comms\SerialConfig.cpp", frmSerialConfig);
USEFORM("Maintenance\EditCustomer.cpp", frmEditCustomer);
USEFORM("POS\SelectPhoenixRoom.cpp", frmPhoenixRoom);
USEFORM("POS\Functions.cpp", frmFunctions);
USEFORM("POS\ShowPrintout.cpp", frmShowPrintout);
USEFORM("POS\HoldSend.cpp", frmHoldSend);
USEFORM("PrinterRedirect.cpp", frmPrinterRedirect);
USEFORM("ProductSearch\ProductSearch.cpp", frmProductSearch);
USEFORM("CaptureCustomerDetails.cpp", frmCaptureDiscountDetails);
USEFORM("ShowEJournal.cpp", frmEJournal);
USEFORM("SelectDateAndTime.cpp", frmSelectDateTime);
USEFORM("Maintenance\WalletConfiguration.cpp", frmWalletConfiguration);
USEFORM("SetUpPosPlus.cpp", frmSetUpPosPlus);
USEFORM("Analysis.cpp", frmAnalysis);
USEFORM("POS\SelectCurrentMenu.cpp", frmSelectCurrentMenu);
USEFORM("RegenerateMallReport.cpp", frmRegenerateMallReport);
USEFORM("EftPosDialogs.cpp", frmEftPos);
USEFORM("POS\TaxRemoval.cpp", frmTaxRemoval);
USEFORM("ReportDisplayNav.cpp", frmReportDisplayNav);
USEFORM("POS\SelectZed.cpp", frmSelectZed);
USEFORM("IntaMateConfiguration.cpp", frmIntaMateConfiguration);
USEFORM("POS\Paytype.cpp", frmPaymentType);
USEFORM("POS\SelectPaymentTypeAndValue.cpp", frmSelectPaymentTypeAndValue);
USEFORM("SetupGlCodes.cpp", frmSetupGlCodes);
USEFORM("Casino Security Screen\MMSecurityScreenSettings.cpp", frmMMSecurityScreenSettings);
USEFORM("POS\TransferItemOrGuest.cpp", frmTransferItemOrGuest);
USEFORM("ComboForm.cpp", frmComboForm);
USEFORM("ParkedSales.cpp", frmParkedSales);
USEFORM("Maintenance\HappyHourSettings.cpp", frmHappyHourSettings);
USEFORM("POS\PatronCount.cpp", frmPatronCount);
USEFORM("EftChequeVerify.cpp", frmChequeVerfiy);
USEFORM("POS\SelectGlass.cpp", frmSelectGlass);
USEFORM("Processing.cpp", frmProcessing);
USEFORM("ProcessWebOrder.cpp", frmProcessWebOrder);
USEFORM("DealEditor.cpp", frmDealEditorForm);
USEFORM("POS\SelectRoom.cpp", frmSelectRoom);
USEFORM("POS\AddTab.cpp", frmAddTab);
USEFORM("POS\SelectTable.cpp", frmSelectTable);
USEFORM("ReportDisplay.cpp", frmReportDisplay);
USEFORM("VirtualPrintersInterface.cpp", frmVirtualPrintersInterface);
USEFORM("SystemConfig.cpp", frmSystemConfig);
USEFORM("Main.cpp", frmMain);
USEFORM("Security\TimeClock.cpp", frmTimeClock);
USEFORM("Chefmate\ChefMateSettings.cpp", frmChefmateSettings);
USEFORM("TierLevelEditor.cpp", frmTierLevelEditor);
USEFORM("Locations.cpp", frmLocations);
USEFORM("ChitNumberGui.cpp", frmChitNumberGui);
USEFORM("SelectSaveOption.cpp", frmSelectSaveOption);
USEFORM("POS\Message.cpp", frmMessage);
USEFORM("DealLevelsEditor.cpp", Form1);
USEFORM("Setup.cpp", frmSetup);
USEFORM("ChitList.cpp", frmChitList);
USEFORM("POS\MenuManager.cpp", frmMenuManager);
USEFORM("XeroSettings.cpp", frmXeroSettings);
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
USEFORM("Maintain.cpp", frmMaintain);
USEFORM("POS\SelectMember.cpp", frmSelectMember);
USEFORM("Discounts.cpp", frmDiscounts);
USEFORM("AutoZReportingSetup.cpp", frmAutoZReportingSetup);
USEFORM("POS\ForcedSides.cpp", frmSelectForcedSides);
USEFORM("SyndCodeGui.cpp", frmSyndCodeGui);
USEFORM("POS\SplitPayment.cpp", frmSplitPayment);
USEFORM("Security\Secure.cpp", frmSecurity);
USEFORM("POS\ConfirmOrd.cpp", frmConfirmOrder);
USEFORM("Tables_ChitNumbers.cpp", frmTables_ChitNumbers);
USEFORM("MMdata.cpp", frmData);
USEFORM("Maintenance\MessageMaintenance.cpp", frmMessageMaintenance);
USEFORM("POS\Users.cpp", frmUsers);
USEFORM("TabManagerGUI.cpp", frmTabManager);
USEFORM("POS\Transfer.cpp", frmTransfer);
USEFORM("POS\SelectActiveMenus.cpp", frmSelectActiveMenus);
USEFORM("CardSwipe.cpp", frmCardSwipe);
USEFORM("MallExportRegenerateReport.cpp", frmMallExportRegenerateReport);
USEFORM("Timer.cpp", frmClockTime);
USEFORM("POS\MenuManagerItemSettings.cpp", frmItemSettings);
USEFORM("Maintenance\NewUser.cpp", frmNewUser);
USEFORM("Maintenance\TaxMaintenance.cpp", frmTaxMaintenance);
USEFORM("MallSalesTypeAssignment.cpp", frmMallSalesTypeAssignment);
USEFORM("Maintenance\GeneralMaintenance.cpp", frmGeneralMaintenance);
USEFORM("PSectionInstructionEdit.cpp", frmPSectionInstructionEdit);
USEFORM("POS\GUIDiscount.cpp", frmDiscount);
USEFORM("LoyaltyMateOperationDialogBox.cpp", frmLoyaltyMateOperationDialogBox);
USEFORM("AddSalesType.cpp", frmAddSalesType);
USEFORM("SelectSizesAsList.cpp", frmSelectSizes);
USEFORM("MallExportResendReport.cpp", frmMallExportResendReport);
USEFORM("ServingTime.cpp", frmServingTime);
USEFORM("GuestList.cpp", Form2);
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










