//---------------------------------------------------------------------------

#ifndef MYOBInvoiceBuilderH
#define MYOBInvoiceBuilderH
//---------------------------------------------------------------------------
#include <vcl.h>
#include <iostream.h>
#include <vector>
using namespace std;
//---------------------------------------------------------------------------

#include "PaymentTransaction.h"
#include "ItemComplete.h"
#include "MYOBIntegration.h"

//---------------------------------------------------------------------------
class TMYOBCategoryDetail
{
  public:
    AnsiString GLCode;
	AnsiString Description;
	double Amount;
    double TaxAmount;
    AnsiString TaxStatus;
//	AnsiString TaxName;
//    double TaxRate;
};


class TMYOBInvoiceDetail
{
  public:
   AnsiString InvoiceNumber;
   AnsiString InvoiceReference;
   AnsiString CustomerName;
   AnsiString JobCode;
   TDateTime  InvoiceDate;
   std::vector<TMYOBCategoryDetail> MYOBCategoryDetails;
};

class TMYOBInvoiceBuilder
{
	public:
		static TMYOBInvoiceBuilder& Instance()
		{
			static TMYOBInvoiceBuilder singleton;
			return singleton;
		}

		TMYOBInvoiceBuilder();
		~TMYOBInvoiceBuilder();

		__property TMYOBInvoice* MYOBInvoice = { read = FMYOBInvoice };

        bool BuildMYOBInvoice( TMYOBInvoiceDetail&  MYOBInvoiceDetails);
        std::vector<TItemComplete *> itemComplete;

	protected:
		TMYOBInvoice* FMYOBInvoice;
		void ClearMYOBInvoice();

        bool CreateMYOBInvoice(TMYOBInvoiceDetail &MYOBInvoiceDetail);
        void AddItemToMYOBInvoice( TMYOBInvoice* inMYOBInvoice, TMYOBCategoryDetail& PayTypeDetail );
		AnsiString CalcAsString(AnsiString inPrice, AnsiString inQty, AnsiString &inTax );
};

#endif
//---------------------------------------------------------------------------

