//---------------------------------------------------------------------------

#ifndef MMPaymentSystemH
#define MMPaymentSystemH
//---------------------------------------------------------------------------

#include "PaymentTransaction.h"
#include "Processing.h"
#include "ProcessingController.h"
#include "GetMoney.h"
#include "Paytype.h"
#include "MMTransactionRecovery.h"

//---------------------------------------------------------------------------
/*
 * this class should hold the common methods and operations for both ListPaymentSystem and InvoicePaymentSystem
 * so that if another payment system comes into play in the future, we can extend this class and override the functionality
*/
class TMMPaymentSystem
{
	public:
		TMMPaymentSystem();
        ~TMMPaymentSystem();
		virtual bool ProcessTransaction(TPaymentTransaction &PaymentTransaction, bool isRecovery = false, bool isClippSale = false ) = 0;

		TListSecurityRefContainer *Security;
        std::auto_ptr<TStringList> logList;
	protected:

		bool PaymentComplete;
		bool PaymentAborted;
		bool RequestEFTPOSReceipt;
		TMMProcessingState State;
		TMMTransactionRecovery transactionRecovery;

		TfrmPaymentType* frmPaymentType;
		TfrmControlTransaction* frmControlTransaction;

		//std::auto_ptr<TfrmPaymentType> frmPaymentType;
		//std::auto_ptr<TfrmControlTransaction> frmControlTransaction;

		//---------------------------------------------------------------------------
		// database access
		//---------------------------------------------------------------------------
		int getTabInvoiceKey( Database::TDBTransaction &dbTransaction, int tabKey );
		void saveLeftOverOrders(  Database::TDBTransaction &dbTransaction, TList* ordersList );
		int getSplitDivisionsByTab( Database::TDBTransaction &dbTransaction, int tabKey );
		void setSplitDivisionsForTab( Database::TDBTransaction &dbTransaction, int tabKey, int divisions );
        UnicodeString getInvoiceNumberByInvoiceKey( Database::TDBTransaction &dbTransaction, int invoiceKey );
		//---------------------------------------------------------------------------
		// utility
		//---------------------------------------------------------------------------
		void initPaymentControlForms( TPaymentTransaction &transactionToControl, bool isRecovering );
		void resetPaymentControlForms( );
		void removeOrdersFromMemory( TList* Orders );
        void cleanUpControlFormsFromMemory();
};
#endif
