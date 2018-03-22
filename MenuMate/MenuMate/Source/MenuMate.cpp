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

USEFORM("GenericGroupGUI.cpp", frmPaymentGroupGUI);
USEFORM("POS\SelectReceipt.cpp", frmSelectReceipt);
USEFORM("SelectTable2.cpp", FrmSelectTable2);
USEFORM("SmartCards\SmartCardConfigGUI.cpp", frmSmartCardConfiguration);
USEFORM("DbMod.cpp", frmDBMod);
USEFORM("SmartCards\SmartcardPreloader.cpp", Smartcard_Preloader);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchKeyboard.cpp", frmTouchKeyboard);
USEFORM("Maintenance\PrinterControlPanel.cpp", frmPrinterControlPanel);
USEFORM("POS\SelectPhoenixRoom.cpp", frmPhoenixRoom);
USEFORM("Maintenance\PrinterMaintenance.cpp", frmPrinterMaintenance);
USEFORM("DeskTop\VerticalSelect.cpp", frmVerticalSelect);
USEFORM("ListManager.cpp", frmListManager);
USEFORM("Casino Security Screen\TransactionAuditScreen.cpp", frmTransactionAuditScreen);
USEFORM("SetButtonFontSize.cpp", frmSetButtonFontSize);
USEFORM("GenericGroupAssignment.cpp", frmGenericGroupAssignment);
USEFORM("NewPaymentType.cpp", frmNewPaymentType);
USEFORM("POS\Scales.cpp", frmScales);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchNumpad.cpp", frmTouchNumpad);
USEFORM("POS\MemberCreation.cpp", frmMemberCreation);
USEFORM("DiscountEditor.cpp", frmDiscountEdit);
USEFORM("POS\BillGroup.cpp", frmBillGroup);
USEFORM("POS\ForcedOptions.cpp", frmSelectForcedOptions);
USEFORM("Maintenance\UserMaintenance.cpp", frmUserMaintenance);
USEFORM("POS\GetMoney.cpp", frmControlTransaction);
USEFORM("PocketVoucher.cpp", frmPocketVoucher);
USEFORM("POS\SelectDish.cpp", frmSelectDish);
USEFORM("EftChequeVerify.cpp", frmChequeVerfiy);
USEFORM("DropDown.cpp", frmDropDownFunc);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMMessageBox.cpp", frmMessageBox);
USEFORM("Maintenance\EditCustomer.cpp", frmEditCustomer);
USEFORM("PHSConfiguration.cpp", frmPHSConfiguration);
USEFORM("Comms\SerialConfig.cpp", frmSerialConfig);
USEFORM("POS\Functions.cpp", frmFunctions);
USEFORM("DropDownVar.cpp", frmDropDownVar);
USEFORM("POS\SelectKitchenDocket.cpp", frmSelectKitchenDocket);
USEFORM("Security\Login.cpp", frmLogin);
USEFORM("POS\ShowPrintout.cpp", frmShowPrintout);
USEFORM("POS\GroupGUI.cpp", frmGroupGUI);
USEFORM("PrinterRedirect.cpp", frmPrinterRedirect);
USEFORM("POS\HoldSend.cpp", frmHoldSend);
USEFORM("CaptureCustomerDetails.cpp", frmCaptureDiscountDetails);
USEFORM("ProductSearch\ProductSearch.cpp", frmProductSearch);
USEFORM("SelectDateAndTime.cpp", frmSelectDateTime);
USEFORM("ComboForm.cpp", frmComboForm);
USEFORM("SetUpPosPlus.cpp", frmSetUpPosPlus);
USEFORM("Maintenance\WalletConfiguration.cpp", frmWalletConfiguration);
USEFORM("RegenerateMallReport.cpp", frmRegenerateMallReport);
USEFORM("Analysis.cpp", frmAnalysis);
USEFORM("POS\TaxRemoval.cpp", frmTaxRemoval);
USEFORM("ShowEJournal.cpp", frmEJournal);
USEFORM("POS\SelectCurrentMenu.cpp", frmSelectCurrentMenu);
USEFORM("EftPosDialogs.cpp", frmEftPos);
USEFORM("ReportDisplayNav.cpp", frmReportDisplayNav);
USEFORM("Casino Security Screen\MMSecurityScreenSettings.cpp", frmMMSecurityScreenSettings);
USEFORM("SetupGlCodes.cpp", frmSetupGlCodes);
USEFORM("POS\SelectPaymentTypeAndValue.cpp", frmSelectPaymentTypeAndValue);
USEFORM("POS\SelectGlass.cpp", frmSelectGlass);
USEFORM("POS\TransferItemOrGuest.cpp", frmTransferItemOrGuest);
USEFORM("IntaMateConfiguration.cpp", frmIntaMateConfiguration);
USEFORM("Processing.cpp", frmProcessing);
USEFORM("POS\SelectZed.cpp", frmSelectZed);
USEFORM("POS\PatronCount.cpp", frmPatronCount);
USEFORM("POS\Paytype.cpp", frmPaymentType);
USEFORM("ParkedSales.cpp", frmParkedSales);
USEFORM("Maintenance\HappyHourSettings.cpp", frmHappyHourSettings);
USEFORM("POS\Users.cpp", frmUsers);
USEFORM("Maintenance\MessageMaintenance.cpp", frmMessageMaintenance);
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
USEFORM("POS\ForcedSides.cpp", frmSelectForcedSides);
USEFORM("Security\Secure.cpp", frmSecurity);
USEFORM("AutoZReportingSetup.cpp", frmAutoZReportingSetup);
USEFORM("SyndCodeGui.cpp", frmSyndCodeGui);
USEFORM("POS\SelectMember.cpp", frmSelectMember);
USEFORM("POS\SplitPayment.cpp", frmSplitPayment);
USEFORM("Maintain.cpp", frmMaintain);
USEFORM("Tables_ChitNumbers.cpp", frmTables_ChitNumbers);
USEFORM("POS\GUIDiscount.cpp", frmDiscount);
USEFORM("MMdata.cpp", frmData);
USEFORM("LoyaltyMateOperationDialogBox.cpp", frmLoyaltyMateOperationDialogBox);
USEFORM("POS\SelectActiveMenus.cpp", frmSelectActiveMenus);
USEFORM("TabManagerGUI.cpp", frmTabManager);
USEFORM("CardSwipe.cpp", frmCardSwipe);
USEFORM("POS\Transfer.cpp", frmTransfer);
USEFORM("POS\ConfirmOrd.cpp", frmConfirmOrder);
USEFORM("Timer.cpp", frmClockTime);
USEFORM("Discounts.cpp", frmDiscounts);
USEFORM("MallExportRegenerateReport.cpp", frmMallExportRegenerateReport);
USEFORM("POS\MenuManagerItemSettings.cpp", frmItemSettings);
USEFORM("Maintenance\NewUser.cpp", frmNewUser);
USEFORM("Maintenance\TaxMaintenance.cpp", frmTaxMaintenance);
USEFORM("Maintenance\GeneralMaintenance.cpp", frmGeneralMaintenance);
USEFORM("AddSalesType.cpp", frmAddSalesType);
USEFORM("PSectionInstructionEdit.cpp", frmPSectionInstructionEdit);
USEFORM("SelectSizesAsList.cpp", frmSelectSizes);
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










