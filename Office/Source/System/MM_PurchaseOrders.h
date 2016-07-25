//---------------------------------------------------------------------------
#ifndef MM_PurchaseOrdersH
#define MM_PurchaseOrdersH
//---------------------------------------------------------------------------
#include "MM_StockCore.h"
//---------------------------------------------------------------------------
namespace Stock
{
//---------------------------------------------------------------------------
class TPurchaseOrdersControl : public TStockControl
{
private:
	Database::TcpIBSQL	sqlDeleteOrder;
	Database::TcpIBSQL	sqlCloseOrder;
	Database::TcpIBSQL	sqlCloseOrderItems;

protected:
public:
				TPurchaseOrdersControl(TIBDatabase *IBDatabase);
	virtual	~TPurchaseOrdersControl();

	bool		OrderComplete(int OrderKey);
	bool		CloseOrder(int OrderKey);
	bool		DeleteOrder(int OrderKey);
};
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------
#endif
