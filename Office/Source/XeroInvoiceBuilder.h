//---------------------------------------------------------------------------

#ifndef XeroInvoiceBuilderH
#define XeroInvoiceBuilderH
    #include <vcl.h>
    #include <iostream.h>
#endif
//---------------------------------------------------------------------------

#include <vector>

#include "MM_StockTransactions.h"

#include "XeroIntegration.h"

//---------------------------------------------------------------------------

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

		bool BuildXeroInvoice( AnsiString                           inContactName,
                               AnsiString                           inInvoiceNumber,
                               AnsiString                           inInvoiceRef,
                               TDateTime                            inInvoiceDate,
                               TDateTime                            inInvoiceDueDate,
                               std::vector<Stock::TInvoiceItemInfo> inInvoiceItems );

	protected:
		TXeroInvoice* FXeroInvoice;

		void ClearXeroInvoice();

		bool CreateXeroInvoice( AnsiString                           inContactName,
                                AnsiString                           inInvoiceNumber,
                                AnsiString                           inInvoiceRef,
                                TDateTime                            inInvoiceDate,
                                TDateTime                            inInvoiceDueDate,
                                std::vector<Stock::TInvoiceItemInfo> inInvoiceItems );
		void AddItemToXeroInvoice( TXeroInvoice* inXeroInvoice, Stock::TInvoiceItemInfo inItem );
};

//---------------------------------------------------------------------------

