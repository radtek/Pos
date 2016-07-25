// ---------------------------------------------------------------------------

#ifndef WebProcUtilH
#define WebProcUtilH

#include "PaymentTransaction.h"
#include "WebOrder.h"
#include "Printing.h"

// ---------------------------------------------------------------------------
class TWebProcUtil
{
	static void __fastcall ProcessKitchenMod(bool Finial, TPaymentTransaction &PaymentTransaction);
	static void __fastcall AutoPrintReceipts(TMMTabType TabType, TPaymentTransaction &PaymentTransaction);
	static void __fastcall PrintKitchenDockets(TPaymentTransaction &PaymentTransaction, int WebKey, UnicodeString JobName,UnicodeString DeviceName);
	static void __fastcall ProcessPatrons(TPaymentTransaction &PaymentTransaction, eTransactionType TransType, int DefaultCovers);
	static void __fastcall ProcessChitNumbers(TForm *inDisplayOwner, TPaymentTransaction &PaymentTransaction);
	static void __fastcall ProcessSecurity(TPaymentTransaction &PaymentTransaction);
	static void __fastcall ProcessStock(TPaymentTransaction &PaymentTransaction);
	static void __fastcall ProcessTimeTracking(TPaymentTransaction &PaymentTransaction);

private:
	static void __fastcall completeOrderToChefMate(TPaymentTransaction* inTransaction);
	static void __fastcall callAwayToChefMate(Database::TDBTransaction &DBTransaction,TCallAwayComplete* inCallAway);
    static bool __fastcall checkAutoPrintReceipts(TMMTabType TabType);
    static UnicodeString checkWebOrderType(Database::TDBTransaction &DBTransaction, int key);

public:
	static void __fastcall ProcessWebOrder(TForm *inDisplayOwner, Database::TDBTransaction &DBTransaction, TWebOrder &WebOrder, TChitNumber &WebOrderChitNumber);
	static void __fastcall ProcessWebOrder(Database::TDBTransaction &DBTransaction, eTransactionType TransType, int defaultCovers, TMMContactInfo &Staff, TList * OrdersList, TMMContactInfo &Member,
		int TimeKey = 0 , int WebKey = 0);
	static void __fastcall ProcessCallAway(Database::TDBTransaction &DBTransaction, int TableNumber, TStringList *Courses);
	static void __fastcall ProcessReceipt(Database::TDBTransaction &DBTransaction, TMMTabType TabType, int Primary, int Secondary);
	static void __fastcall TWebProcUtil::ProcessPrintJob(Database::TDBTransaction &DBTransaction, UnicodeString TransNo, UnicodeString DeviceName,std::vector<UnicodeString>GUIDS);


};
#endif
