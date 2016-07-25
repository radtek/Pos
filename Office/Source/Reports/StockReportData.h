//---------------------------------------------------------------------------

#ifndef StockReportDataH
#define StockReportDataH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "RpCon.hpp"
#include "RpConDS.hpp"
#include "RpDefine.hpp"
#include <DB.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <DB.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <IBSQL.hpp>
//---------------------------------------------------------------------------
class TdmStockReportData : public TDataModule
{
__published:	// IDE-managed Components
	TIBTransaction *StockTrans;
	TIBQuery *qrStock;
	TRvDataSetConnection *ravStock;
	TIBQuery *qrStockMovement;
	TRvDataSetConnection *ravStockMovement;
	TIBQuery *qrStockValue;
	TRvDataSetConnection *ravStockValue;
	TIBQuery *qrStocktake;
	TRvDataSetConnection *ravStocktake;
	TIBQuery *qrContactDetails;
	TRvDataSetConnection *ravContactDetails;
	TIBQuery *qrStockVariance;
	TRvDataSetConnection *ravStockVariance;
	TIBQuery *qrStockReorder;
	TRvDataSetConnection *ravStockReorder;
	TIBQuery *qrPurchaseOrder;
	TRvDataSetConnection *ravPurchaseOrder;
	TIBQuery *qrStockReceiptByCat;
	TRvDataSetConnection *ravStockReceiptByCat;
	TIBQuery *qrStockTransferFrom;
	TRvDataSetConnection *ravStockTransferFrom;
	TIBQuery *qrStockGroupVariance;
	TRvDataSetConnection *ravStockGroupVariance;
	TIBQuery *qrStockInquiry;
	TRvDataSetConnection *ravStockInquiry;
	TIBQuery *qrSupplierBackOrders;
	TRvDataSetConnection *ravSupplierBackOrders;
	TIBQuery *qrStockBarcodes;
	TRvDataSetConnection *ravStockBarcodes;
    TRvDataSetConnection *ravStockWriteOff;
    TIBQuery *qrStockWriteOff;
    TIBQuery *qrStockManufacture;
    TRvDataSetConnection *ravStockManufacture;
   TIBQuery *qrExportedInvoices;
   TRvDataSetConnection *ravExportedInvoices;
    TIBQuery *qrCommitedInvoices;
    TRvDataSetConnection *ravCommitedInvoices;
   TIBQuery *qrStockRequest;
   TRvDataSetConnection *ravStockRequest;
   TIBQuery *qrSupplierInvoice;
   TRvDataSetConnection *ravSupplierInvoice;
   TIBQuery *qrstockgroupnew;
   TRvDataSetConnection *ravstockgroupnew;
    TIBQuery *qrTransferAudit;
    TRvDataSetConnection *ravTransferAudit;
    TIBQuery *qrStockReconcialation;
    TRvDataSetConnection *ravStockReconcialation;
    

private:	// User declarations
	AnsiString ParamString(int Count, AnsiString Field, AnsiString ParamName="Param", AnsiString Operator="or");
public:		// User declarations
	__fastcall TdmStockReportData(TComponent* Owner);

	void SetupStockValueByGroup(TStrings *Groups);
	void SetupStockValueByLocation(TStrings *Locations, TStrings *Groups);
	void SetupStockBarcodes(TStrings *Groups);
//	void SetupStockDetailsByGroup(TStrings *Groups);
//	void SetupStockDetailsByLocation(TStrings *Locations, TStrings *Groups);
	void SetupStockDetails(TStrings *Locations, TStrings *Groups);
	void SetupStockMovement(TDateTime StartTime, TDateTime EndTime, AnsiString Transaction, TStrings *Locations, TStrings *Groups);
	void SetupStockTransfer(TDateTime StartTime, TDateTime EndTime, TStrings *Locations, TStrings *Groups, int TransferNumber);
	void SetupStockBatchTransfer(TDateTime StartTime, TDateTime EndTime);
	void SetupStockInquiry(TDateTime StartTime, TDateTime EndTime, TStrings *Locations, TStrings *Groups, TStrings *Transactions);
	void SetupStockReorderItem(TStrings *Locations, TStrings *Groups);
	//void SetupStocktakeHistory(TDateTime StartTime, TDateTime EndTime, TStrings *StocktakeFilter);
	void SetupSupplierPurchases(TDateTime StartTime, TDateTime EndTime, TStrings *Suppliers, TStrings *Invoices , int ByVal);
	void SetupSupplierDetails(TStrings *Suppliers);
	void SetupStockReorderBySupplier(TStrings *Suppliers);
	void SetupSupplierBackOrders(TStrings *Suppliers);
    void SetupWriteOffLocation(TDateTime StartTime, TDateTime EndTime, TStrings *Locations);
    void SetupManufactureRecipe(TDateTime StartTime, TDateTime EndTime);
    void SetupSupplierInvoice(TDateTime StartTime, TDateTime EndTime);

	void SetupStockVariance(int StocktakeKey);
	void SetupStockVariance(int StocktakeKey,int RadioButtonValue,AnsiString VarianceAmount);
	void SetupStocktake(int StocktakeKey, bool HideOnHand, bool HideBarcodes);
	void SetupStocktakeStockValuation(TStrings *Stocktakes);
	void SetupPurchaseOrder(int OrderKey);
    void SetupInvoices(AnsiString BatchKeys);
    void SetupInvoices(int BatchKey);
    void SetupStockRequestDetails(AnsiString Group,int StockReqKey,AnsiString Location) ;
   void  TransferAudit(int Transfer_no);
   void SetupSupplierPurchasesRuntime(AnsiString ReceiptNumber,  AnsiString Suppliers,  AnsiString Invoices);
   void SetupStockReconcialation(TDateTime StartTime, TDateTime EndTime, TStrings *Locations, TStrings *Groups) ;
};                                  
//---------------------------------------------------------------------------
extern PACKAGE TdmStockReportData *dmStockReportData;
//---------------------------------------------------------------------------
#endif
