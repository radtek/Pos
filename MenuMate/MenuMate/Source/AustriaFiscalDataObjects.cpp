//---------------------------------------------------------------------------


#pragma hdrstop

#include "AustriaFiscalDataObjects.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
TChargeItemAustriaFiscal::TChargeItemAustriaFiscal()
{
	Quantity = 0.0;
	UnicodeString Description;
	Amount = 0.0;
	VATRate = 0.0;
	ChargeItemCase = 0;
	ChargeItemCaseData = "";
	VATAmount = 0.0;
	AccountNumber = "";
	CostCenter = "";
	ProductGroup = "";
	ProductNumber = "";
	ProductBarcode = "";
	Unit = "";
	UnitQuantity = 0.0;
	UnitPrice = 0.0;
	//Moment = "";
}
TChargeItemAustriaFiscal::~TChargeItemAustriaFiscal()
{
}
TPayItemAustriaFiscal::TPayItemAustriaFiscal()
{
	Quantity = 0.0;
	Description = "";
	Amount = 0.0;
	PayItemCase = 0;
	PayItemCaseData = "";
	AccountNumber = "";
	CostCenter = "";
	MoneyGroup = "";
	MoneyNumber = "";
}
TPayItemAustriaFiscal::~TPayItemAustriaFiscal()
{
}
TReceiptRequestAustriaFiscal::TReceiptRequestAustriaFiscal()
{
	CashBoxID = "";
	QueueID = "";
	TerminalID = "";
	ReceiptReference = "";
	ChargeItems.clear();
	PayItems.clear();
	ReceiptCase = 0;
	ReceiptCaseData = "";
	ReceiptAmount = "";
	User = "";
	Area = "";
	Customer = "";
	Settlement = "";
	PrevousReceiptID = "";
}
TReceiptRequestAustriaFiscal::~TReceiptRequestAustriaFiscal()
{
}
TSignaturItemAustriaFiscal::TSignaturItemAustriaFiscal()
{
    SignatureFormat = "";
    SignatureType = "";
    Caption = "";
    Data = "";
}
TSignaturItemAustriaFiscal::~TSignaturItemAustriaFiscal()
{
}
TReceiptResponseAustriaFiscal::TReceiptResponseAustriaFiscal()
{
	CashBoxID = "";
	QueueID = "";
	QueueItemID = "";
	QueueRow = "";
	TerminalID = "";
	ReceiptReference = "";
	CashBoxIdentification = "";
	ReceiptIdentification = "";
	Signatures.clear();
	State = "";
	StateData = "";
}
TReceiptResponseAustriaFiscal::~TReceiptResponseAustriaFiscal()
{
}
