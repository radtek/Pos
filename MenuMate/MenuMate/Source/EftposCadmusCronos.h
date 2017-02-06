//---------------------------------------------------------------------------

#ifndef EftposCadmusCronosH
#define EftposCadmusCronosH
//---------------------------------------------------------------------------

#include <OleServer.hpp>
#include <memory>
#include "Eftpos.h"
#include "CronosWrapper.h"
#include <safearry.h>
#include <map>
#include <utility>
#include "CadmusEftposLib_OCX.h"

#define EFT_STATE_TIMEOUT 30
enum TEftPosCadmusCronosStates { EftCC_Connecting = 0,EftCC_Disconnecting,EftCC_Purchase,EftCC_Refund,
                                 EftCC_Payment,EftCC_ChequeAuth, EftCC_Logon, EftCC_Settlement,
                                 EftCC_ManualPAN,EftCC_ReqLastReceipt,EftCC_DisplayMenu, EftCC_CancelEFTPOS,
                                 EftCC_TransactionCompletionAdvice,
                                 EftCC_MerchantMessageStatus,EftCC_BusyWithDialog,EftCC_Idle,EftCC_Unknown};

enum TEftPosCadmusCSVCol { EFTCOL = 0,APPCOL,RESULTCOL,ERRORTXT};

class TStateResultIndex
{
   public:
   TStateResultIndex(){}
   TStateResultIndex(int inState,AnsiString inCommand,AnsiString inResultCode) : State(inState),Command(inCommand),ResultCode(inResultCode){}
   int State;
   AnsiString Command;
   AnsiString ResultCode;
   bool operator < (const TStateResultIndex& other) const
   {
      if (State < other.State)
      {
          return true;
      }
      else if(State == other.State)
      {
         if (Command < other.Command)
         {
            return true;
         }
         else if(Command == other.Command)
         {
            if (ResultCode < other.ResultCode)
            {
               return true;
            }
            else
            {
               return false;
            }
         }
         else
         {
            return false;
         }
      }
      else
      {
         return false;
      }
   }
};

bool operator == (const TStateResultIndex& x , const TStateResultIndex& y)
{
   if (x.State == y.State && x.Command == y.Command && x.ResultCode == y.ResultCode) return true;
   else return false;
}


typedef void (__closure *TMMAction)();
class TStateResultMsg
{
   public:
   TStateResultMsg();
   TStateResultMsg(eTransactionResult inResult, AnsiString inResultText, bool inEventCompleted, bool inAdditionalInfo = false,TMMAction inAction = NULL);
   eTransactionResult Result;
   AnsiString ResultText;
   TMMAction Action;
   bool EventCompleted;
   bool AdditionalInfo;
   void AssignTo(TEftPosTransaction *EftTrans);
   void Assign(eTransactionResult inResult, AnsiString inResultText, bool inEventCompleted);
};


class TEftPosCadmusCronos : public TEftPos
{
	private :
   TEftPosCadmusCronosStates State;
   TStateResultMsg           StateResult;
   AnsiString CadmusMessage;
   bool Online;
   ChronosEFTPOS ChronosEFTPOSInterface;
   void __fastcall EFT2POSMessage(ChronosEFTPOS* Sender, AnsiString Text);
   void __fastcall InitialiseEFTPOS();   

   void ParseMessge();
   bool __fastcall BeginState(TEftPosCadmusCronosStates inState,AnsiString Packet);
   void SetState(TEftPosCadmusCronosStates State);
   void WaitIfBusy();
   void SetBusy();
   void ClearBusy();
   bool WaitOnStateComplete(bool StateAllowTimeOut = false);

	AnsiString BuildManualPurchasePacket(Currency Purchase,AnsiString Card, int Month, int Year);
	AnsiString BuildChequeAuthPacket(Currency ChequeAmount, AnsiString BankAndBranch,
												AnsiString AccountNumber,AnsiString ChequeSerialNumber);
	AnsiString BuildInitPacket();
   AnsiString BuildQuitPacket();
	AnsiString BuildLogonPacket();
	AnsiString BuildCancelPacket();
	AnsiString BuildSigOkPacket(bool SigOk);
	AnsiString BuildReprintReceiptPacket();
	AnsiString BuildSettlementCutoverPacket();
	AnsiString BuildSettlementEnquiryPacket(TDateTime Date);
	AnsiString BuildRefundPacket(Currency Refund, AnsiString Card);
	AnsiString BuildCancelTransactionPacket(AnsiString inID);
   AnsiString BuildPurchasePacket(Currency Purchase, Currency CashOut);
   AnsiString BuildCashOutPacket(Currency CashOut);
   AnsiString BuildUtilityPacket();

	void SendPacket(AnsiString Data);

   AnsiString LastErrorMessage;

   TEftPosCadmusCronosStates MessageToCronoState(AnsiString Msg,AnsiString SubType);
   void Print();

	protected :
		void __fastcall TransactionEvent(TEftPosTransaction *EftTrans,eTransactionResult Result);
	public:
		TEftPosCadmusCronos();
		~TEftPosCadmusCronos();

		int MerchantNumber;

      void Initialise();
      void DoControlPannel();
      void ProcessEftPos(eEFTTransactionType TxnType,Currency AmtPurchase,Currency AmtCash,
									 UnicodeString TxnRef,ePANSource PANSource,
									 UnicodeString CardSwipe,int ExpiryMonth, int ExpiryYear);
		void ChequeVerify( Currency AmtPurchase, AnsiString ChequeSerial, AnsiString ChequeBranch,
							 AnsiString ChequeAccount, ePANSource PANSource, AnsiString TxnRef = "");
		bool CheckChequeBranch(AnsiString Branch);
		bool CheckChequeSerial(AnsiString ChequeSerial);
		bool CheckChequeAccount(AnsiString ChequeAccount);
		void CancelTransaction(AnsiString inID);

		AnsiString GetRefNumber();
		void __fastcall DoSettlementCutover();
		void __fastcall DoSettlementEnquiry();
		void __fastcall DoPrintLocalTotals();
		void __fastcall DoLogon();
		void __fastcall ReprintReceipt();
      void __fastcall DoEFTUtils();
      bool DoConnected();
};
#endif
