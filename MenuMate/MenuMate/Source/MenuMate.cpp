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

USEFORM("SelectDiscounts.cpp", frmSelectDiscounts);
USEFORM("DealsSetup.cpp", frmDeals);
USEFORM("Setup.cpp", frmSetup);
USEFORM("ProcessWebOrder.cpp", frmProcessWebOrder);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMColourPicker.cpp", frmColourPicker);
USEFORM("TerminalChitConfig.cpp", frmTerminalChitConfig);
USEFORM("POS\MenuManager.cpp", frmMenuManager);
USEFORM("TierLevels.cpp", frmTierLevel);
USEFORM("POS\PosMain.cpp", frmPOSMain);
USEFORM("POS\MenuManagerItemSizeSelection.cpp", frmItemSizeSelection);
USEFORM("Maintenance\PaymentMaintenance.cpp", frmPaymentMaintenance);
USEFORM("DiscountTimes.cpp", frmDiscountTimes);
USEFORM("POS\SelectRoom.cpp", frmSelectRoom);
USEFORM("DealEditor.cpp", frmDealEditorForm);
USEFORM("Maintenance\HappyHourSettings.cpp", frmHappyHourSettings);
USEFORM("POS\Users.cpp", frmUsers);
USEFORM("POS\AddTab.cpp", frmAddTab);
USEFORM("Main.cpp", frmMain);
USEFORM("VirtualPrintersInterface.cpp", frmVirtualPrintersInterface);
USEFORM("Security\Secure.cpp", frmSecurity);
USEFORM("Maintenance\MessageMaintenance.cpp", frmMessageMaintenance);
USEFORM("POS\SelectTable.cpp", frmSelectTable);
USEFORM("ReportDisplay.cpp", frmReportDisplay);
USEFORM("XeroSettings.cpp", frmXeroSettings);
USEFORM("DealLevelsEditor.cpp", Form1);
USEFORM("ChitList.cpp", frmChitList);
USEFORM("SystemConfig.cpp", frmSystemConfig);
USEFORM("Security\TimeClock.cpp", frmTimeClock);
USEFORM("Locations.cpp", frmLocations);
USEFORM("POS\Message.cpp", frmMessage);
USEFORM("TierLevelEditor.cpp", frmTierLevelEditor);
USEFORM("Chefmate\ChefMateSettings.cpp", frmChefmateSettings);
USEFORM("SyndCodeGui.cpp", frmSyndCodeGui);
USEFORM("MallExportRegenerateReport.cpp", frmMallExportRegenerateReport);
USEFORM("DelayedPaymentTabs.cpp", frmDelayedPaymentTabs);
USEFORM("POS\SelectMember.cpp", frmSelectMember);
USEFORM("POS\MenuManagerItemSettings.cpp", frmItemSettings);
USEFORM("SelectSizesAsList.cpp", frmSelectSizes);
USEFORM("Timer.cpp", frmClockTime);
USEFORM("Maintenance\NewUser.cpp", frmNewUser);
USEFORM("Maintenance\TaxMaintenance.cpp", frmTaxMaintenance);
USEFORM("MallExportResendReport.cpp", frmMallExportResendReport);
USEFORM("SelectSaveOption.cpp", frmSelectSaveOption);
USEFORM("ChitNumberGui.cpp", frmChitNumberGui);
USEFORM("CaptNamePhone.cpp", frmCaptNamePhone);
USEFORM("POS\SelectRMSRoom.cpp", frmRMSRoom);
USEFORM("POS\SelectItem.cpp", frmSelectItem);
USEFORM("MMdata.cpp", frmData);
USEFORM("POS\SplitPayment.cpp", frmSplitPayment);
USEFORM("Discounts.cpp", frmDiscounts);
USEFORM("Tables_ChitNumbers.cpp", frmTables_ChitNumbers);
USEFORM("PSectionInstructionEdit.cpp", frmPSectionInstructionEdit);
USEFORM("AutoZReportingSetup.cpp", frmAutoZReportingSetup);
USEFORM("LoyaltyMateOperationDialogBox.cpp", frmLoyaltyMateOperationDialogBox);
USEFORM("SendForm.cpp", frmSendForm);
USEFORM("Maintenance\GeneralMaintenance.cpp", frmGeneralMaintenance);
USEFORM("POS\ForcedSides.cpp", frmSelectForcedSides);
USEFORM("POS\GUIDiscount.cpp", frmDiscount);
USEFORM("POS\Transfer.cpp", frmTransfer);
USEFORM("Maintain.cpp", frmMaintain);
USEFORM("POS\ConfirmOrd.cpp", frmConfirmOrder);
USEFORM("Processing.cpp", frmProcessing);
USEFORM("POS\SelectActiveMenus.cpp", frmSelectActiveMenus);
USEFORM("Maintenance\UserMaintenance.cpp", frmUserMaintenance);
USEFORM("IntaMateConfiguration.cpp", frmIntaMateConfiguration);
USEFORM("CardSwipe.cpp", frmCardSwipe);
USEFORM("ParkedSales.cpp", frmParkedSales);
USEFORM("TabManagerGUI.cpp", frmTabManager);
USEFORM("POS\PatronCount.cpp", frmPatronCount);
USEFORM("DropDown.cpp", frmDropDownFunc);
USEFORM("POS\BillGroup.cpp", frmBillGroup);
USEFORM("POS\ForcedOptions.cpp", frmSelectForcedOptions);
USEFORM("PocketVoucher.cpp", frmPocketVoucher);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMMessageBox.cpp", frmMessageBox);
USEFORM("Security\Login.cpp", frmLogin);
USEFORM("POS\HoldSend.cpp", frmHoldSend);
USEFORM("POS\SelectDish.cpp", frmSelectDish);
USEFORM("POS\GetMoney.cpp", frmControlTransaction);
USEFORM("EftChequeVerify.cpp", frmChequeVerfiy);
USEFORM("Maintenance\EditCustomer.cpp", frmEditCustomer);
USEFORM("POS\SelectKitchenDocket.cpp", frmSelectKitchenDocket);
USEFORM("Comms\SerialConfig.cpp", frmSerialConfig);
USEFORM("DropDownVar.cpp", frmDropDownVar);
USEFORM("POS\ShowPrintout.cpp", frmShowPrintout);
USEFORM("DiscountEditor.cpp", frmDiscountEdit);
USEFORM("PHSConfiguration.cpp", frmPHSConfiguration);
USEFORM("PrinterRedirect.cpp", frmPrinterRedirect);
USEFORM("POS\Functions.cpp", frmFunctions);
USEFORM("POS\SelectPhoenixRoom.cpp", frmPhoenixRoom);
USEFORM("DbMod.cpp", frmDBMod);
USEFORM("Maintenance\PrinterControlPanel.cpp", frmPrinterControlPanel);
USEFORM("Maintenance\PrinterMaintenance.cpp", frmPrinterMaintenance);
USEFORM("DeskTop\VerticalSelect.cpp", frmVerticalSelect);
USEFORM("SmartCards\SmartCardConfigGUI.cpp", frmSmartCardConfiguration);
USEFORM("GenericGroupGUI.cpp", frmPaymentGroupGUI);
USEFORM("POS\SelectReceipt.cpp", frmSelectReceipt);
USEFORM("SelectTable2.cpp", FrmSelectTable2);
USEFORM("SmartCards\SmartcardPreloader.cpp", Smartcard_Preloader);
USEFORM("GenericGroupAssignment.cpp", frmGenericGroupAssignment);
USEFORM("ListManager.cpp", frmListManager);
USEFORM("Casino Security Screen\TransactionAuditScreen.cpp", frmTransactionAuditScreen);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchKeyboard.cpp", frmTouchKeyboard);
USEFORM("NewPaymentType.cpp", frmNewPaymentType);
USEFORM("SetButtonFontSize.cpp", frmSetButtonFontSize);
USEFORM("POS\GroupGUI.cpp", frmGroupGUI);
USEFORM("EftPosDialogs.cpp", frmEftPos);
USEFORM("..\..\..\Shared-Source\Touch Utils\MMTouchNumpad.cpp", frmTouchNumpad);
USEFORM("Analysis.cpp", frmAnalysis);
USEFORM("POS\SelectCurrentMenu.cpp", frmSelectCurrentMenu);
USEFORM("POS\Paytype.cpp", frmPaymentType);
USEFORM("POS\MemberCreation.cpp", frmMemberCreation);
USEFORM("POS\Scales.cpp", frmScales);
USEFORM("Casino Security Screen\MMSecurityScreenSettings.cpp", frmMMSecurityScreenSettings);
USEFORM("ReportDisplayNav.cpp", frmReportDisplayNav);
USEFORM("POS\SelectZed.cpp", frmSelectZed);
USEFORM("POS\SelectPaymentTypeAndValue.cpp", frmSelectPaymentTypeAndValue);
USEFORM("POS\TaxRemoval.cpp", frmTaxRemoval);
USEFORM("POS\TransferItemOrGuest.cpp", frmTransferItemOrGuest);
USEFORM("POS\SelectGlass.cpp", frmSelectGlass);
USEFORM("ComboForm.cpp", frmComboForm);
USEFORM("SetupGlCodes.cpp", frmSetupGlCodes);
USEFORM("SelectDateAndTime.cpp", frmSelectDateTime);
USEFORM("CaptureCustomerDetails.cpp", frmCaptureDiscountDetails);
USEFORM("ShowEJournal.cpp", frmEJournal);
USEFORM("ProductSearch\ProductSearch.cpp", frmProductSearch);
USEFORM("RegenerateMallReport.cpp", frmRegenerateMallReport);
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










