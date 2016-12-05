//---------------------------------------------------------------------------

#ifndef CashDenominationControllerH
#define CashDenominationControllerH
//---------------------------------------------------------------------------
#include <memory>
#include "ListManager.h"

class TBlindBalance
{
   public :
   TBlindBalance() :
                  CashDenominationName(""),
                  TransQty(0),
                  CashDenominationValue(0),
                  BlindBalance(0){}

   TBlindBalance( AnsiString inCashDenominationName,
                  int inTransQty, double inCashDenominationValue, Currency inBlindBalance) :
                  CashDenominationName(inCashDenominationName),
                  TransQty(inTransQty),
                  CashDenominationValue(inCashDenominationValue),
                  BlindBalance(inBlindBalance){}

   AnsiString CashDenominationName;
   int TransQty;
   double CashDenominationValue;
   Currency BlindBalance;
   void operator = (TBlindBalance rhs);
};

struct TCashDenominationDetails
{
   UnicodeString CashDenominationName;
   double DenominationValue;
};



typedef std::map< AnsiString, TBlindBalance > TBlindBalanceContainer;
typedef std::vector< AnsiString > TBlindBalanceSortedVector;
class TBlindBalances
{
	TBlindBalanceContainer BlindBalances;
	TBlindBalanceSortedVector BlindBalancesVector;
	Currency Total;

   public :
   TBlindBalanceContainer::iterator GetIterator(int index);
	TBlindBalanceContainer::iterator begin();
   TBlindBalanceContainer::iterator end();
	TBlindBalanceContainer::iterator find(AnsiString Value);

	TBlindBalanceSortedVector::iterator vectorbegin();
	TBlindBalanceSortedVector::iterator vectorend();

	void SetTotal(Currency sum, double qty);
	Currency GetTotal(void);
	void ClearTotal(void);
   int size();
   void SetBalance(TBlindBalance &Balance);

	void UpdateSystemBalance(AnsiString Payment, Currency Amount);
   void UpdateBlindBalance(AnsiString Payment, int qty, Currency Amount, double value);

   void UpdateSystemBalance(int Index, Currency Amount);
   void UpdateBlindBalance(int Index, Currency Amount);

   void RemoveBlindBalance(int Index);
   AnsiString BalanceName(int Index);
   Currency GetSystemBalance(int Index);
   Currency GetBlindBalance(int Index);
   Currency GetSystemBalance(AnsiString Name);
   Currency GetBlindBalance(AnsiString Name);
   bool IndexValid(Index);
	bool Empty();

	AnsiString BagID;
};

class TCashDenominationController
{
   private :
      std::auto_ptr<TfrmListManager> frmListManager;
      Database::TDBTransaction &DBTransaction;
		TForm *DisplayOwner;
		AnsiString Terminal;
		void OnClose(int ItemIndex, int ColIndex = -1);
		void OnAdd(int ItemIndex, int ColIndex = -1);
		void OnEdit(int ItemIndex, int ColIndex = -1);
		void OnDelete(int ItemIndex, int ColIndex = -1);
		void PopulateListManager();
		bool WarnOperator(void);
		void LoadBlindBalances(void);
		void UpdateBlindBalances(AnsiString BagID);
   public :
		TBlindBalances BlindBalances;
		static TBlindBalances MasterBalance;
		TCashDenominationController(TForm *inDisplayOwner,Database::TDBTransaction &inDBTransaction);
		TCashDenominationController(TForm *inDisplayOwner,Database::TDBTransaction &inDBTransaction, AnsiString DeviceName);
        void PopulateDenominationValues(Database::TDBTransaction &DBTransaction, std::vector<TCashDenominationDetails> &DenominationDetails);
		bool Run();
		AnsiString GetBagID(void);
		TBlindBalances Get();
		static TBlindBalances GetMaster();
};

		TBlindBalances TCashDenominationController::MasterBalance;
class TBlindBalanceControllerInterface
{
 private:
   TBlindBalances Balances;
   AnsiString BagId;
   static TBlindBalanceControllerInterface* instance;

        // Constructor and Destructor
        TBlindBalanceControllerInterface();
        ~TBlindBalanceControllerInterface();

    public:
        static TBlindBalanceControllerInterface* Instance()
        {
            if(instance == NULL)
                instance = new TBlindBalanceControllerInterface();

            return instance;
        }
     void SetBalances(TBlindBalances balances);
     void SetBagID(AnsiString bagId);
     TBlindBalances GetBalances();
     AnsiString GetBagID();
};
#endif
