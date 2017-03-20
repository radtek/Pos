//---------------------------------------------------------------------------

#ifndef XeroInvoiceBuilderH
#define XeroInvoiceBuilderH

#include <vcl.h>
#include <iostream.h>
#include <vector>
using namespace std;
//---------------------------------------------------------------------------

#include "XeroIntegration.h"
#include "PaymentTransaction.h"
#include "ItemComplete.h"

//---------------------------------------------------------------------------

class TXeroPayTypeDetail
{
  public:
    AnsiString AccountCode;
	AnsiString Description;
	double UnitAmount;
	double TaxAmount;
};

class TXeroCategoryDetail
{
  public:
    AnsiString AccountCode;
	AnsiString Description;
	double UnitAmount;
	double TaxAmount;
	double Quantity;
};


class TXeroInvoiceDetail
{
  public:
   AnsiString InvoiceNumber;
   AnsiString InvoiceReference;
   AnsiString ContactName;
   TDateTime  InvoiceDate;
   std::vector<TXeroPayTypeDetail> XeroPayTypeDetails;
   std::vector<TXeroCategoryDetail> XeroCategoryDetails;
};
struct NodeItem
{
    AnsiString ItemCode;
    AnsiString ItemName;
    AnsiString UnitAmount;
    AnsiString TaxAmount;
    AnsiString Qty;
};
class TXeroInvoiceBuilder
{
	public:
		static TXeroInvoiceBuilder& Instance()
		{
			static TXeroInvoiceBuilder singleton;
			return singleton;
		}

		TXeroInvoiceBuilder();
		~TXeroInvoiceBuilder();

		__property TXeroInvoice* XeroInvoice = { read = FXeroInvoice };

		bool BuildXeroInvoice( TPaymentTransaction& inPaymentTransaction );
        bool BuildXeroInvoice( TXeroInvoiceDetail&  XeroInvoiceDetails);
        std::vector<TItemComplete *> itemComplete;
        void CheckRoundingAmount(TXeroInvoiceDetail& XeroInvoiceDetail);
        double roundAmount;
        bool CheckInvoiceTotal(double invoiceTotal);

	protected:
		TXeroInvoice* FXeroInvoice;
		TPayment*     FXeroPayment;
		double        FPercentageToXero;
		void ClearXeroInvoice();

		bool ChargeToXero( TPaymentTransaction& inPaymentTransaction );

		bool CreateXeroInvoice( TPaymentTransaction& inPaymentTransaction );
        bool CreateXeroInvoice(TXeroInvoiceDetail &XeroInvoiceDetail);
		void AddItemToXeroInvoice( TXeroInvoice* inXeroInvoice, TItemMinorComplete* inOrder );
        void AddItemToXeroInvoice( TXeroInvoice* inXeroInvoice, TXeroCategoryDetail& PayTypeDetail , bool isAddRounding = false);
		AnsiString CalcAsString(AnsiString inPrice, AnsiString inQty, AnsiString &inTax );
		double GetTotalAmountToXero( TPayment* inXeroPayment );
		double GetTotalAmount( TPaymentTransaction& inPaymentTransaction );
        void CompressItemForInvoice(TXeroInvoice *FXeroInvoice, TPaymentTransaction& inPaymentTransaction);
        void AddPaymentToXeroInvoice( TXeroInvoice* inXeroInvoice, TXeroPayTypeDetail& PayTypeDetail );

    private:
        void AddItemToCollection(NodeItem xeroNodeItem, std::vector<NodeItem> &nodeItems);
        void AddItemToInvoiceXML(TXeroInvoice *FXeroInvoice, std::vector<NodeItem> nodeItems);
};

#endif
//---------------------------------------------------------------------------

