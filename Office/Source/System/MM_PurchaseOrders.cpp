//---------------------------------------------------------------------------
#pragma hdrstop

#include "MM_PurchaseOrders.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
using namespace Stock;
//---------------------------------------------------------------------------
TPurchaseOrdersControl::TPurchaseOrdersControl(TIBDatabase *IBDatabase) : TStockControl(IBDatabase),
			sqlDeleteOrder(		new TIBSQL(NULL)),
			sqlCloseOrder(			new TIBSQL(NULL)),
			sqlCloseOrderItems(	new TIBSQL(NULL))
{
	fDBTransaction.RegisterQuery(sqlDeleteOrder);
	fDBTransaction.RegisterQuery(sqlCloseOrder);
	fDBTransaction.RegisterQuery(sqlCloseOrderItems);
	sqlDeleteOrder->SQL->Text		= "Delete From PurchaseOrder Where Order_LK = :OrderKey";

	sqlCloseOrder->SQL->Text =
		"Update "
			"PurchaseOrder "
		"Set "
			"Received = 'T' "
		"Where "
			"Order_LK = :OrderKey";
	sqlCloseOrderItems->SQL->Text	=
		"Update "
			"PurchaseStock "
		"Set "
			"Qty_On_Order = 0 "
		"Where "
			"Order_FK = :OrderKey";
}
//---------------------------------------------------------------------------
TPurchaseOrdersControl::~TPurchaseOrdersControl()
{
}
//---------------------------------------------------------------------------
bool TPurchaseOrdersControl::OrderComplete(int OrderKey)
{
	bool Complete = false;
	if (OrderKey != 0)
	{
		fDBTransaction.StartTransaction();
		try
		{
			Database::TcpIBSQL sqlOrderReceived(new TIBSQL(NULL));
			fDBTransaction.RegisterQuery(sqlOrderReceived);
			sqlOrderReceived->SQL->Text	=
				"Select "
					"Max(Qty_On_Order) Max_Qty_On_Order "
				"From "
					"PurchaseStock "
				"Where "
					"Order_FK = :OrderKey ";
			sqlOrderReceived->ParamByName("OrderKey")->AsInteger = OrderKey;
			sqlOrderReceived->ExecQuery();
			Complete = (sqlOrderReceived->Fields[0]->AsInteger <= 0);
		}
		__finally
		{
			fDBTransaction.CommitTransaction();
		}
	}
	return Complete;
}
//---------------------------------------------------------------------------
bool TPurchaseOrdersControl::CloseOrder(int OrderKey)
{
	fDBTransaction.StartTransaction();
	try
	{
		sqlCloseOrderItems->Close();
		sqlCloseOrderItems->ParamByName("OrderKey")->AsInteger = OrderKey;
		sqlCloseOrderItems->ExecQuery();
		// Mark as received (ie Closed)
		sqlCloseOrder->Close();
		sqlCloseOrder->ParamByName("OrderKey")->AsInteger = OrderKey;
		sqlCloseOrder->ExecQuery();
		// See if the order was found.
		bool Success = (sqlCloseOrder->RowsAffected == 1);
		fDBTransaction.CommitTransaction();
		return Success;
	}
	catch (Exception &E)
	{
		fDBTransaction.UndoTransaction();
		return false;
	}
}
//---------------------------------------------------------------------------
bool TPurchaseOrdersControl::DeleteOrder(int OrderKey)
{
	fDBTransaction.StartTransaction();
	try
	{
		// Delete the order out of DB
		sqlDeleteOrder->Close();
		sqlDeleteOrder->ParamByName("OrderKey")->AsInteger = OrderKey;
		sqlDeleteOrder->ExecQuery();
		bool Success = (sqlDeleteOrder->RowsAffected == 1);
		fDBTransaction.CommitTransaction();
		return Success;
	}
	catch (Exception &E)
	{
		fDBTransaction.UndoTransaction();
		return false;
	}
}
//---------------------------------------------------------------------------

