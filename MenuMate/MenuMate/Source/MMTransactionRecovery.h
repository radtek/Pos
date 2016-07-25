//---------------------------------------------------------------------------

#ifndef MMTransactionRecoveryH
#define MMTransactionRecoveryH
//---------------------------------------------------------------------------

#include "PaymentTransaction.h"
#include "ListSecurityRefContainer.h"

class TMMTransactionRecovery
{
	public:
		TMMTransactionRecovery();

		void ProcessTransactionRecovery();
		void SaveRecoveryInformation(
                        TPaymentTransaction &PaymentTransaction,
                        TListSecurityRefContainer* securityContainer );
		bool LoadRecoveryInformation(
                        TPaymentTransaction &RecoverTransaction,
                        TListSecurityRefContainer* securityContainer );

		void ClearRecoveryInfo();

		AnsiString RecoveryFilePayments;
		AnsiString RecoveryFileOrders;
		AnsiString RecoveryFilePaymentSystem;
		AnsiString RecoveryFileSecurity;

	private:
		bool isInvoicePaymentTransaction( TPaymentTransaction &transaction );

        void saveTransactionInfoToFile( TPaymentTransaction &PaymentTransaction);
        void savePaymentTypesInfoToFile( TPaymentTransaction &PaymentTransaction );
        void saveOrdersInfoToFile( TPaymentTransaction &PaymentTransaction );
		void saveSecurtyInfoToFile( TListSecurityRefContainer* securityContainer );

        void loadTransactionInfoFromFile( TPaymentTransaction &PaymentTransaction );
        void loadPaymentTypesInfoFromFile( TPaymentTransaction &PaymentTransaction );
        void loadOrdersInfoFromFile( TPaymentTransaction &PaymentTransaction );
		void loadSecurtyInfoFromFile( TListSecurityRefContainer* securityContainer );

        void logUserFromTransactionFile( TPaymentTransaction &PaymentTransaction );
        int getStaffKeyFromTransactionFile();

        bool areAllRecoveryFilesPresent();
};

#endif
