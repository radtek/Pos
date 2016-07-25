//---------------------------------------------------------------------------
#pragma hdrstop

#include "CreateForms.h"

#include "Utilities.h"
#include "Login.h"
#include "StockMain.h"
#include "Users.h"
#include "AddUser.h"
#include "AddStock.h"
#include "AddContact.h"

//#include "StockGroupVisible.h"
#include "Stocktake.h"
#include "ManualAdjust.h"
#include "PurchaseOrder.h"
#include "CreateOrders.h"
#include "ReceiveInvoice.h"
/*#include "AddContact.h"
#include "AddContact.h"
#include "AddContact.h"
#include "AddContact.h"
#include "AddContact.h"
#include "AddContact.h"
#include "AddContact.h"
#include "AddContact.h"
#include "AddContact.h"
#include "AddContact.h"
#include "AddContact.h"
#include "AddContact.h"
#include "AddContact.h"
#include "AddContact.h"
#include "AddContact.h"
#include "AddContact.h"
#include "AddContact.h"
#include "AddContact.h"*/


#include "MMReportData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
void CreateForms()
{
	dmUtilities						= new TdmUtilities(Application);
	frmLogin							= new TfrmLogin(Application);
	frmStockMain					= new TfrmStockMain(Application);
	frmUsers							= new TfrmUsers(Application);
	frmAddUser						= new TfrmAddUser(Application);
	frmAddStock						= new TfrmAddStock(Application);
	frmAddContact					= new TfrmAddContact(Application);
	frmStockGroupVisible			= new TfrmStockGroupVisible(Application);
	frmStocktake					= new TfrmStocktake(Application);
	frmManualAdjust				= new TfrmManualAdjust(Application);
	frmPurchaseOrder				= new TfrmPurchaseOrder(Application);
	frmCreateOrders				= new TfrmCreateOrders(Application);
	frmReceiveInvoice				= new TfrmReceiveInvoice(Application);
	frmSelectMenu					= new TfrmSelectMenu(Application);
	frmSelectStockItem			= new TfrmSelectStockItem(Application);
	frmSelectPurchaseOrder		= new TfrmSelectPurchaseOrder(Application);
	frmReceiveOrder				= new TfrmReceiveOrder(Application);
	frmInvoices						= new TfrmInvoices(Application);
	frmReceiveStockItem			= new TfrmReceiveStockItem(Application);
	frmSelectSupplier				= new TfrmSelectSupplier(Application);
	frmStock							= new TfrmStock(Application);
	frmSuppliers					= new TfrmSuppliers(Application);
	frmAddContact					= new TfrmAddContact(Application);
	frmPurchaseOrders				= new TfrmPurchaseOrders(Application);
	frmViewTrans					= new TfrmViewTrans(Application);
	frmTransferMulti				= new TfrmTransferMulti(Application);
	frmAddSupplierUnit			= new TfrmAddSupplierUnit(Application);
	frmSelectLocation				= new TfrmSelectLocation(Application);
	frmSelectCategory				= new TfrmSelectCategory(Application);
	frmCreateProfile				= new TfrmCreateProfile(Application);
	frmAdministration				= new TfrmAdministration(Application);
	frmMYOBPurchasesExport		= new TfrmMYOBPurchasesExport(Application);
	frmSetupPurchaseOrders		= new TfrmSetupPurchaseOrders(Application);
	frmAddStockCategoryGroup	= new TfrmAddStockCategoryGroup(Application);
	frmSelectMenuItem				= new TfrmSelectMenuItem(Application);

	frmDialog						= new TfrmDialog(Application);
	frmSelectDate					= new TfrmSelectDate(Application);

	frmBackup						= new TfrmBackup(Application);
	frmMenuEdit						= new TfrmMenuEdit(Application);
	frmGetBarcode					= new TfrmGetBarcode(Application);
	frmPreview						= new TfrmPreview(Application);
	frmReports						= new TfrmReports(Application);
	dmStockReportData				= new TdmStockReportData(Application);

	dmMMReportData 				= new TdmMMReportData(Application);

	frmPayrollExport				= new TfrmPayrollExport(Application);

	frmDebtorsMain					= new TfrmDebtorsMain(Application);
	frmSelectDebtor				= new TfrmSelectDebtor(Application);
	frmDebtors						= new TfrmDebtors(Application);
	frmDebtorInvoices				= new TfrmDebtorInvoices(Application);
}
//---------------------------------------------------------------------------
