//---------------------------------------------------------------------------

#ifndef AustriaFiscalDataObjectsH
#define AustriaFiscalDataObjectsH
#include "System.hpp"
#include <vector>
//---------------------------------------------------------------------------
class TChargeItemAustriaFiscal
{
	public:
        TChargeItemAustriaFiscal();
        ~TChargeItemAustriaFiscal();
		double Quantity;
		UnicodeString Description;
		double Amount;
		double VATRate;
		__int64 ChargeItemCase;    // Populate from Services or check
		UnicodeString ChargeItemCaseData;
		double VATAmount;
		UnicodeString AccountNumber;
		UnicodeString CostCenter;
		UnicodeString ProductGroup;
		UnicodeString ProductNumber;
		UnicodeString ProductBarcode;
		UnicodeString Unit;
		double UnitQuantity;
		double UnitPrice;
		TDateTime Moment; // Populate from Services or check
};
class TPayItemAustriaFiscal
{
	public:
        TPayItemAustriaFiscal();
        ~TPayItemAustriaFiscal();
		double Quantity;
		UnicodeString Description ;
		double Amount;
		__int64 PayItemCase;
		UnicodeString PayItemCaseData;
		UnicodeString AccountNumber;
		UnicodeString CostCenter;
		UnicodeString MoneyGroup;
		UnicodeString MoneyNumber;
		TDateTime Moment;
};

class TReceiptRequestAustriaFiscal
{
	public:
        TReceiptRequestAustriaFiscal();
        ~TReceiptRequestAustriaFiscal();
		UnicodeString CashBoxID;
		UnicodeString QueueID;
		UnicodeString TerminalID;
		UnicodeString ReceiptReference;
		TDateTime ReceiptMoment;    // Populate from Services or check
		std::vector<TChargeItemAustriaFiscal> ChargeItems;
		std::vector<TPayItemAustriaFiscal> PayItems;
		__int64 ReceiptCase;
		UnicodeString ReceiptCaseData;
		UnicodeString ReceiptAmount;
		UnicodeString User;
		UnicodeString Area;
		UnicodeString Customer;
		UnicodeString Settlement;
		UnicodeString PrevousReceiptID;
};

class TSignaturItemAustriaFiscal
{
	public:
        TSignaturItemAustriaFiscal();
        ~TSignaturItemAustriaFiscal();
		UnicodeString SignatureFormat;
		UnicodeString SignatureType;
		UnicodeString Caption;
		UnicodeString Data;
};

class TReceiptResponseAustriaFiscal
{
	public:
         TReceiptResponseAustriaFiscal();
        ~TReceiptResponseAustriaFiscal();
		UnicodeString CashBoxID;
		UnicodeString QueueID;
		UnicodeString QueueItemID;
		UnicodeString QueueRow;
		UnicodeString TerminalID;
		UnicodeString ReceiptReference;
		UnicodeString CashBoxIdentification;
		UnicodeString ReceiptIdentification;
		TDateTime ReceiptMoment;
		std::vector<TSignaturItemAustriaFiscal> Signatures;
		UnicodeString State;
		UnicodeString StateData;
        bool UnableToConnectToMenumateService;
        bool UnableToConnectToFiscalService;
};
#endif
