//---------------------------------------------------------------------------

#ifndef MMInvoicePaymentSystemH
#define MMInvoicePaymentSystemH
//---------------------------------------------------------------------------

#include "ListPaymentSystem.h"

//---------------------------------------------------------------------------
class TMMInvoicePaymentSystem : public TListPaymentSystem
{
    public:
        TMMInvoicePaymentSystem();

        // for InvoicePaymentSystem, the MasterPaymentTransaction will contain the combined orders from all the invoices
        bool ProcessTransaction(TPaymentTransaction &MasterPaymentTransaction, bool isRecovery = false);
        void SendFiscalPrint(TPaymentTransaction &MasterPaymentTransaction);

	private:

		void _processMultipleInvoiceTransaction( TPaymentTransaction &MasterPaymentTransaction);
		void _processMultipleInvoicePartialPayTransaction( TPaymentTransaction &MasterPaymentTransaction );
		void _processMultipleInvoiceSplitPayTransaction( TPaymentTransaction &MasterPaymentTransaction );

		void _performPostInvoiceTransactionSetOperations( TPaymentTransaction &MasterPaymentTransaction );
		void _performPostInvoiceTransactionOperations( TPaymentTransaction &paymentTransaction );
		void _buildInvoicePaymentTransactions(
                            TPaymentTransaction &MasterPaymentTransaction,
							std::vector<TPaymentTransaction> &invoicePaymentTransactions );
        void _assignPaymentsOnTransactionSet(
                            TPaymentTransaction &MasterPaymentTransaction,
                            std::vector<TPaymentTransaction> &invoicePaymentTransactions );

		void _extractOrdersByTabKey( TPaymentTransaction &transaction, __int64 tabKey, TList* destinationOrderList );
		void _removeInvoicesCannotBePayed( std::vector<TPaymentTransaction> &transactionSet, Currency requestedTotal );
		void _reCalculatePaymentPercentageOnLastInvoice( std::vector<TPaymentTransaction> &transactionSet, double &splitPercentage, Currency &requestedTotal );
		void _populateOrdersFromInvoices( TPaymentTransaction &paymentTransaction, std::vector<TPaymentTransaction> &transactionSet );
		void _loadDivisionsLeftFromTabs( TPaymentTransaction &MasterPaymentTransaction );
		void _setDivisionsLeftForTabs( TPaymentTransaction &MasterPaymentTransaction );
		void _preparePaymentTypes( TPaymentTransaction &paymentTransaction );
        void _clearPaymentsOnTransactionSet( std::vector<TPaymentTransaction> &transactionSet );
        void _extractUsedPaymentsFromTransaction( TPaymentTransaction &paymentTransaction, std::vector<TPayment*> &usedPayments );
		void _distributePaymentOnTransactionSet( TPayment* payment, std::vector<TPaymentTransaction> &transactionSet );
		void _clearArchivedReceipt();
};
#endif
