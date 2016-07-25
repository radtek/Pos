//---------------------------------------------------------------------------
// Component:        MM_StockTransactions
// Author:           Peter Dunlop
// Version:          3.3
// C++ Builder:      Version 6
// Date:             26 Aug 2004
// Copyright:        © 2004 MenuMate Ltd.
//
// This is a Blah blah
//
// 26-08-2004 First revision.
//---------------------------------------------------------------------------
#ifndef MM_StockTransactionsH
#define MM_StockTransactionsH
//---------------------------------------------------------------------------
#include "MM_StockCore.h"
#include <vector>
#include <set>
//---------------------------------------------------------------------------
namespace Stock
{
//---------------------------------------------------------------------------
enum TTransactionType { ttTransfer, ttReceipt, ttSale, ttWriteoff,ttRefund, ttStocktake };
//---------------------------------------------------------------------------
class TTransactionBatchInfo
{
private:
public:
	TTransactionBatchInfo() {}
	TTransactionBatchInfo(TTransactionType iBatchType) :
			BatchID(0), Supplier_Key(0), Closed(false), Total_Cost(0), Total_GST(0), Exported(false)
		{	BatchType = iBatchType; Created = Now(); }

	int					BatchID;
	TTransactionType	BatchType;
	AnsiString			User_ID;
	AnsiString			User_Name;
	TDateTime			Created;
	// Used for Invoice receipt
	AnsiString			Reference;		// Invoice Number
	AnsiString			Order_Number;	// For purchase orders
	int					Supplier_Key;	// Not sure if used. Is stored in db but lifetime unknown
	AnsiString			Supplier_Name;
	AnsiString			Supplier_ID;
	AnsiString			Export_1;
	AnsiString			Export_2;
	AnsiString			Export_3;
	Currency				Total_Cost;		// Only fill in for invoice receipt. Otherwise, automatically calculated.
	Currency				Total_GST;		// Only fill in for invoice receipt. Otherwise, automatically calculated.
	bool					Closed;
	bool					Exported;
};
//---------------------------------------------------------------------------
class TTransactionInfo
{
private:
public:
	TTransactionInfo(TTransactionType iTransactionType, TTransactionBatchInfo const& iBatchInfo) :
		Qty(0), Supplier_Key(0), Order_Qty(0), Unit_Cost(0), GST_Percent(0), Total_GST(0)
		{ TransactionType = iTransactionType; BatchInfo = iBatchInfo; }

	TTransactionBatchInfo	BatchInfo;
	TTransactionType			TransactionType;
	AnsiString					StockCode;
	AnsiString					Description;
	AnsiString					Stock_Category;
	AnsiString					Stock_Group;
	AnsiString					GL_Code;
	AnsiString					Location;
	AnsiString					Unit;
	Currency						Unit_Cost;
	Currency						Total_Cost;
	Currency						GST_Percent;
	Currency						Total_GST;
	double						Qty;
	// For stock receipt
	int							Supplier_Key;
	AnsiString					Supplier_Name;
	AnsiString					Supplier_Code;
	// Stock receipt via purchase order
	AnsiString					Order_Number;
	AnsiString					Order_Unit;
	double						Order_Qty;
	// Transfer
	AnsiString					Purchaser_Name;
	AnsiString					Operator;
	AnsiString					Note;
};
//---------------------------------------------------------------------------
class TTransferInfo
{
private:
public:
	int			StockKey;
	double		Quantity;
	AnsiString	Source;
	AnsiString	Destination;
};
//---------------------------------------------------------------------------
typedef std::vector<TTransferInfo> TcTransferInfo;
typedef TcTransferInfo::const_iterator TiTransferInfo;
//---------------------------------------------------------------------------
class TInvoiceItemInfo
{
private:
public:
	TInvoiceItemInfo() : Stock_Key(0), Purchase_Stock_Key(0), Supplier_Unit_Cost(0), Total_GST(0),Total_Cost(0), Order_Qty(0), BackOrder(0) {}

	int			Stock_Key;
	int			Purchase_Stock_Key;
	AnsiString	Location;
	AnsiString	Supplier_Code;
	AnsiString	Order_Unit;
	double		Order_Qty;
	Currency		Total_GST;
	Currency		Total_Cost;
	double		Supplier_Unit_Size;
	Currency		Supplier_Unit_Cost;
	double		BackOrder;
};
//---------------------------------------------------------------------------
typedef std::vector<TInvoiceItemInfo> TcInvoiceItemInfo;
typedef TcInvoiceItemInfo::const_iterator TiInvoiceItemInfo;
//---------------------------------------------------------------------------
class TStocktakeItemInfo
{
private:
public:
	int			StockKey;
	AnsiString	Location;
	double		Counted;
	double		Variance;
};
//---------------------------------------------------------------------------
typedef std::vector<TStocktakeItemInfo> TcStocktakeItemInfo;
typedef TcStocktakeItemInfo::const_iterator TiStocktakeItemInfo;
//---------------------------------------------------------------------------
class TSaleItemInfo
{
public:
	AnsiString			StockCode;
	AnsiString			Location;
	double				Qty;
	AnsiString          DeviceName;
	TTransactionType	AdjustmentType;
};
//---------------------------------------------------------------------------

class TStockLocationDetails
{
public:
  TStockLocationDetails() :
	  GST_Percent(0), Latest_Cost(0), Average_Cost(0), On_Hand(0), Transfer(0), Inwards(0) {}
  AnsiString	StockCode;
  AnsiString	Description;
  Currency		GST_Percent;
  AnsiString	Stock_Category;
  AnsiString	Stock_Group;
  AnsiString	GL_Code;
  AnsiString	Stocktake_Unit;
  AnsiString	Location;
  Currency		Latest_Cost;
  Currency		Average_Cost;
  double		On_Hand;

  double		Writeoffs_Pending;
  double		Sales_Pending;


  TDateTime	Last_Stocktake;
  double		Opening;
  double		Writeoff;
  double		Sales;
  double		Stocktake;
  double		Transfer;
  double		Inwards;


};

class TStockTransaction : public TStockControl
{
private:
	Database::TcpIBSQL	sqlCreateBatch;
	Database::TcpIBSQL	sqlUpdateBatch;
	Database::TcpIBSQL	sqlCreateTransaction;

	Database::TcpIBSQL	sqlStockDetails;
	Database::TcpIBSQL	sqlAdjustStock;


protected:
	bool				fCreateBatch(TTransactionBatchInfo& BatchInfo);
	bool				fUpdateBatch(TTransactionBatchInfo& BatchInfo);	// Adjusts the cost and GST fields
	void				fAddBatchTransaction(TTransactionInfo const& TransactionInfo);

	void				fSetStockParams(int StockKey, AnsiString Location, TTransactionType TransactionType,
									Currency AverageCost, Currency LatestCost,
									double Qty);

	bool				fGetStockDetails(int StockKey, AnsiString Location, TStockLocationDetails& StockLocationDetails);
	void				fReadStockDetails(TStockLocationDetails& StockLocationDetails);

public:
						TStockTransaction(TIBDatabase *IBDatabase);
};
//---------------------------------------------------------------------------
class TReceiveInvoice : public TStockTransaction
{
private:
	Database::TcpIBSQL	sqlAdjustSupplierStock;
	Database::TcpIBSQL	sqlAdjustPurchaseStock;

	bool						fReceiveInvoiceItem(TTransactionBatchInfo const& BatchInfo, TiInvoiceItemInfo const &InvoiceItemInfo);

	void						fSetSupplierStockCost(int StockKey, int SupplierKey, AnsiString SupplierUnit,
										AnsiString SupplierCode, double SupplierUnitSize, Currency SupplierUnitCost);
public:
	TReceiveInvoice(TIBDatabase *IBDatabase);

	bool						ReceiveInvoice(TTransactionBatchInfo& BatchInfo, TcInvoiceItemInfo const& InvoiceItems);
};
//---------------------------------------------------------------------------
class TTransferStock : public TStockTransaction
{
private:
	void	fTransferStock(TTransactionBatchInfo const& BatchInfo, TiTransferInfo const& TransferInfo);

public:
	TTransferStock(TIBDatabase *IBDatabase);

	bool	TransferStock(TTransactionBatchInfo& BatchInfo, TcTransferInfo const& Transfers);
};
//---------------------------------------------------------------------------
struct TStocktakeDetails
{
	int				StocktakeKey;
	int				BatchKey;
	AnsiString		Name;
	AnsiString		Location;

	bool				Initialised;
	TDateTime		InitialisedTime;
	AnsiString		InitialisedBy;

	bool				Committed;
	TDateTime		CommittedTime;
	AnsiString		CommittedBy;
};
//---------------------------------------------------------------------------
typedef std::vector<TStocktakeDetails> TStocktakeList;
//---------------------------------------------------------------------------
struct TStocktakeProfile
{
	int					ProfileKey;
	AnsiString			Name;
	std::vector<int>	StockGroups;
};
//---------------------------------------------------------------------------
typedef std::vector<TStocktakeProfile> TStocktakeProfiles;
//---------------------------------------------------------------------------
class TStocktakeControl : public TStockTransaction
{
public:
	TStocktakeControl(TIBDatabase *IBDatabase);

	void	EnumStocktakes(TStocktakeList &StocktakeList, bool Uninitialised, bool Initialised, bool Committed);
	int	CreateStocktake(const TStocktakeProfiles &StocktakeProfiles, const AnsiString &Name, const AnsiString &Location);
	bool	DeleteStocktake(int StocktakeKey);
	bool	Initialise(int StocktakeKey, AnsiString UserID, AnsiString UserName, bool CopyOnHand);
	bool	SetCount(int StockKey, AnsiString Location, Currency Count, Currency OnHand, Currency &Variance);
	bool	CommitStocktake(TTransactionBatchInfo& BatchInfo, int StocktakeKey, TcStocktakeItemInfo const& StocktakeItems);
	bool	CancelInitialisation(int StocktakeKey);
	bool	CreateStockSCV(int StocktakeKey, AnsiString FileName);
   void  ResetInitialisation(int StocktakeKey, bool CopyOnHand);


private:
	Database::TcpIBSQL	sqlStocktakeHistory;
	Database::TcpIBSQL	sqlUpdateStockLocation;

	void	fCancelInitialisation(int StocktakeKey);
	void	fUpdateStocktakeItem(TTransactionBatchInfo const& BatchInfo, int StocktakeKey, TiStocktakeItemInfo const& StocktakeInfo);
};
//---------------------------------------------------------------------------
class TStockSale : public TStockTransaction
{
private:
	Database::TcpIBSQL		sqlSelectStock;
	Database::TcpIBSQL		sqlUpdateStockLocation;

	TTransactionBatchInfo	fBatchInfo;
	bool							fEnabled;

	Currency						fTotalCost;
	Currency						fTotalGST;

public:
	TStockSale(TIBDatabase *IBDatabase);

	bool	AdjustStock(TSaleItemInfo& SaleItemInfo, AnsiString Operator, AnsiString Reason);
	bool	Close(bool Commit);
};
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------
#endif

