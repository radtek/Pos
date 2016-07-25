// ---------------------------------------------------------------------------

#ifndef ReqPrintJobH
#define ReqPrintJobH
// ---------------------------------------------------------------------------
#include "Request.h"
#include "Printing.h"
#include "Printouts.h"
#include "enumPrinting.h"
#include "PaymentTransaction.h"

#include <Classes.hpp>

class TReqPrintJob : public TRequest
{
public:
	_fastcall TReqPrintJob(TDeviceDB *Device);
	_fastcall ~TReqPrintJob();

	DeviceType SenderType;
	PrintJobType JobType; // Order , Receipt;
	PrintJobRequestType RequestType; // Request, Response , Redirection.

	UnicodeString Sender; // Machine Name.
	UnicodeString Destination; // Machine Name.
	UnicodeString DestPrinterOverride; // Used on a redirection as Orignal printer failed.
	UnicodeString DocketNumber;

	TStringList *ReceiptHeader;
	TStringList *ReceiptPHeader;
	TStringList *ReceiptFooter;
    TStringList *ReceiptDetails;

	TDateTime TimeStamp;
	int JobAge; // Age of Print Job in the Que.
	// long 	LoyalCustomerLK;
	long WaitTime;
	int BarCodeData;
	int MembershipNameDisplay;

	TList *PrintJobs;
	TPrintouts *Printouts;
	TPaymentTransaction *Transaction;

	// For sorting the above list of orders.
	typedef int __fastcall(*TListSortCompare)(void*Item1, void*Item2);

	TListSortCompare ptrSortFunc;

	// std::set<long> Tabs;
	std::set<long>OrderLocalKeys;

	UnicodeString Waiter;
	// bool Loyalty;
	bool Echo;
	bool SignReceipt;
	bool AccountPayment;
	UnicodeString AccountInvoiceNumber;
	ePaymentType PaymentType;
	// void Clear();
	TStringList *ExtraInfo;
	TStringList *DeliveryInfo;
	TStringList *PaymentInfo;
	TStringList *OrderComments;


	std::map<UnicodeString, UnicodeString>MiscData;

	TStringList *TabHistory;

};
#endif
