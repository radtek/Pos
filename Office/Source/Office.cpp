//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USEFORM("Setup.cpp", frmSetup);
USEFORM("Backup.cpp", frmBackup);
USEFORM("MMData.cpp", dmMMData);
USEFORM("Dialog.cpp", frmMenuLoad);
USEFORM("MenuEdit.cpp", frmMenuEdit);
USEFORM("Main.cpp", frmMain);
USEFORM("Preview.cpp", frmPreview);
USEFORM("Users.cpp", frmUsers);
USEFORM("AddUser.cpp", frmAddUser);
USEFORM("Login.cpp", frmLogin);
USEFORM("SelectMenu.cpp", frmSelectMenu);
USEFORM("Stock\ViewTrans.cpp", frmViewTrans);
USEFORM("Stock\AddStock.cpp", frmAddStock);
USEFORM("Stock\PurchaseOrder.cpp", frmPurchaseOrder);
USEFORM("Stock\StockData.cpp", dmStockData);
USEFORM("Stock\Stocktake.cpp", frmStocktake);
USEFORM("Stock\StocktakeProfiles.cpp", frmStocktakeProfiles);
USEFORM("Stock\SelectDate.cpp", frmSelectDate);
USEFORM("Stock\StockMain.cpp", frmStockMain);
USEFORM("Stock\SelectStockItem.cpp", frmSelectStockItem);
USEFORM("Stock\SelectPurchaseOrder.cpp", frmSelectPurchaseOrder);
USEFORM("Stock\ReceiveOrder.cpp", frmReceiveOrder);
USEFORM("Stock\Invoices.cpp", frmInvoices);
USEFORM("Stock\ReceiveStockItem.cpp", frmReceiveStockItem);
USEFORM("Stock\SelectSupplier.cpp", frmSelectSupplier);
USEFORM("Stock\Stock.cpp", frmStock);
USEFORM("Stock\Suppliers.cpp", frmSuppliers);
USEFORM("Stock\PurchaseOrders.cpp", frmPurchaseOrders);
USEFORM("SelectCompany.cpp", frmSelectCompany);
USEFORM("GetBarcode.cpp", frmGetBarcode);
USEFORM("ErrorWarningSettings.cpp", frmErrorWarningSettings);
USEFORM("SelectCategory.cpp", frmSelectCategory);
USEFORM("Reports\Reports.cpp", frmReports);
USEFORM("Stock\SelectLocation.cpp", frmSelectLocation);
USEFORM("Stock\AddSupplierUnit.cpp", frmAddSupplierUnit);
USEFORM("Stock\CreateProfile.cpp", frmCreateProfile);
USEFORM("Splash.cpp", frmSplash);
USEFORM("Utilities.cpp", dmUtilities); /* TDataModule: File Type */
USEFORM("Administration.cpp", frmAdministration);
USEFORM("Stock\TransferMulti.cpp", frmTransferMulti);
USEFORM("Stock\AddContact.cpp", frmAddContact);
USEFORM("Debtors\DebtorInvoices.cpp", frmDebtorInvoices);
USEFORM("Debtors\AddDebtor.cpp", frmAddDebtor);
USEFORM("Debtors\SelectDebtor.cpp", frmSelectDebtor);
USEFORM("Debtors\Debtors.cpp", frmDebtors);
USEFORM("Debtors\DebtorsMain.cpp", frmDebtorsMain);
USEFORM("Stock\ReceiveInvoice.cpp", frmReceiveInvoice);
USEFORM("Stock\MYOBPurchasesExport.cpp", frmMYOBPurchasesExport);
USEFORM("Stock\SetupPurchaseOrders.cpp", frmSetupPurchaseOrders);
USEFORM("Stock\AddStockCategoryGroup.cpp", frmAddStockCategoryGroup);
USEFORM("Debtors\AddDebtorCompany.cpp", frmAddDebtorCompany);
USEFORM("SetupLocations.cpp", frmSetupLocations);
USEFORM("SelectMenuItem.cpp", frmSelectMenuItem);
USEFORM("Reports\StockReportData.cpp", dmStockReportData); /* TDataModule: File Type */
USEFORM("Reports\MMReportData.cpp", dmMMReportData); /* TDataModule: File Type */
USEFORM("Stock\CreateOrders.cpp", frmCreateOrders);
USEFORM("UpdateCategories.cpp", frmUpdateCategories);
USEFORM("BetaAgreement.cpp", frmBetaAgreement);
USEFORM("Stock\SelectSupplierStockItem.cpp", frmSelectSupplierStockItem);
USEFORM("Stock\StocktakeVarianceParameters.cpp", frmVarianceParams);
USEFORM("Stock\SelectTransferLocations.cpp", frmSelectTransferLocations);
USEFORM("PreviewData.cpp", frmPreviewData);
USEFORM("Stock\AddSupplierBarcode.cpp", frmAddSupplierBarcode);
USEFORM("Stock\SelectStocktake.cpp", frmSelectStocktake);
USEFORM("Reports\Excel.cpp", frmExcel);
USEFORM("Stock\LocationRename.cpp", frmUpdateLocations);
USEFORM("TimeClock\TimeClockForm.cpp", frmTimeClockExport);
USEFORM("ExportForm.cpp", frmExport);
USEFORM("Accounts\AccountsExportForm.cpp", frmAccountsExport);
USEFORM("Accounts\EditAccountCustomerForm.cpp", frmEditAccountCustomer);
USEFORM("Cashup\CashupForm.cpp", frmCashup);
USEFORM("BarcodeSync\SyncMenuToRecipeForm.cpp", BarcodeSyncForm);
USEFORM("MenuEditor\CourseItemPricesForm.cpp", frmCourseItemPrices);
USEFORM("SqlViewer.cpp", SqlView);
USEFORM("Security\ConfigureRolesForm.cpp", frmConfigureRoles);
USEFORM("CreateRecipe.cpp", frmCreateRecipe);
USEFORM("Stock\WriteOffStock.cpp", frmWriteOffStock);
USEFORM("DeleteRecipe.cpp", frmDeleteRecipe);
USEFORM("RecipeBox.cpp", frmRecipeBox);
USEFORM("UseRecipe.cpp", frmUseRecipe);
USEFORM("EditRecipe.cpp", frmEditRecipe);
USEFORM("processingWeborder.cpp", frmProcessingWeborder);
USEFORM("ChefMateData.cpp", dmChefMateData); /* TDataModule: File Type */
USEFORM("TaxProfile\TaxProfileGUI.cpp", frmTaxProfile);
USEFORM("Stock\UpdateSupplierCosts.cpp", frmUpdateSupplierCosts);
USEFORM("Stock\SelectImportType.cpp", frmSelectImportType);
USEFORM("Stock\SelectSupplierStockRequest.cpp", frmSelectSupplierStockRequest);
USEFORM("Stock\StockRequestTransfer.cpp", frmStockRequestTransfer);
USEFORM("Stock\StockRequest.cpp", frmStockRequest);
USEFORM("Stock\StockRequestHistory.cpp", frmStockRequestHistory);
USEFORM("Stock\StockGroup.cpp", frmStockGroup);
USEFORM("Stock\StockSubstitute.cpp", frmStockSubstitute);
USEFORM("Stock\LocationSelection.cpp", frmLocationSelection);
USEFORM("Stock\SelectLocationSupplier.cpp", frmSelectLocationSupplier);
USEFORM("InputCategory.cpp", frmInputCategory);
//---------------------------------------------------------------------------
HANDLE Mutex;
bool AlreadyRunning()
{
	HWND PrevWnd;

   Mutex = CreateMutex(NULL, false, "MenuMateOfficeUniqueName");
   if ((Mutex != NULL) && (GetLastError() == ERROR_ALREADY_EXISTS))
   {
		PrevWnd = FindWindow("TfrmMain", "MenuMate Office");
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
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		if (AlreadyRunning()) return 0;
        SysLocale.FarEast = true;
		Application->Initialize();
		Application->Title = "MenuMate Office";
		Application->CreateForm(__classid(TfrmMain), &frmMain);
		Application->CreateForm(__classid(TSqlView), &SqlView);
		Application->CreateForm(__classid(TfrmStockRequestTransfer), &frmStockRequestTransfer);
		Application->CreateForm(__classid(TfrmStockRequest), &frmStockRequest);
		Application->CreateForm(__classid(TfrmStockRequestHistory), &frmStockRequestHistory);
		Application->CreateForm(__classid(TfrmStockGroup), &frmStockGroup);
		Application->CreateForm(__classid(TfrmLocationSelection), &frmLocationSelection);
		Application->CreateForm(__classid(TfrmSelectLocationSupplier), &frmSelectLocationSupplier);
		Application->CreateForm(__classid(TfrmInputCategory), &frmInputCategory);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	return 0;
}
//---------------------------------------------------------------------------











