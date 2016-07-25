//---------------------------------------------------------------------------
#ifndef TransControlH
#define TransControlH
#pragma link "CSDEFTLib_OCX"

#include "CSDEFTLib_OCX.h"
#include <set>
#include <iterator>

//---------------------------------------------------------------------------
enum eTransactionStages {eSTAGE0,eSTAGE1,eSTAGE2,eSTAGE3};
//enum eUserOptions{eBack,eClose,eCloseandPrint};
// Stage 1 = New Transaction.
// Stage 2 = Reg Values Written and Eftpos Processed.
// Stage 3 = Printing Complete. (This stage is rerun on a powerfailure to always print/reprint last receipt.)
// Stage 4 = Cleanup Stage Complete (Value written to Reg at end of this stage = Stage 1)

class CControlTransaction : public TObject
{
   TCsdEft *Eftpos;
   bool Intergrated;
   bool PowerFailed;
   bool SecondarySwipe;
   eTransactionResult EFTState;
   AnsiString RegKey;
   void __fastcall EftposTransactionEvent(TObject *Sender);
public:
   bool Temporary;
   std::set<long> Orders;
   TList *TemporaryOrders;

   AnsiString TxnRef;
   AnsiString ErrorMessage;

   Currency AmountPaid;
   bool SplitPayment;
   Currency Change;
   Currency EftCashOut;
   Currency EftPay;
   Currency CreditPay;
   Currency CreditCashOut;
   Currency Credit;
   Currency AmexPay;
   Currency AmexCashOut;
   Currency DinersPay;
   Currency DinersCashOut;
   Currency BarterPay;
   Currency BarterCashOut;
   Currency ChequeCashOut;
   Currency ChequePay;
   Currency CashTendered;
   Currency TotalCredit;
   Currency LeftOwing;
   Currency Rounding;
	Currency Discount;

   Currency TotalElectronicCashOut;
   Currency TotalElectronicPay;

   CControlTransaction();
   virtual __fastcall ~CControlTransaction();
   eTransactionResult ProcessTransaction();
   void RecoverStage1();
   void Stage1();
   void Stage2();
   void Stage3();
   eTransactionResult RetriveElectronicTransactionResult();
   eUserOptions RetrieveUserOptions();
   void Recover();
   bool GetCardSwipe();
   AnsiString CardTrack2;
    void InitGetMoney();
    bool ElectronicTransExits();
};

extern CControlTransaction *TransControl;
#endif
