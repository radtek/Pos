//---------------------------------------------------------------------------

#ifndef BlindBalanceControllerH
#define BlindBalanceControllerH
//---------------------------------------------------------------------------
#include <memory>
#include "ListManager.h"

class TBlindBalance
{
   public :
   TBlindBalance() :
                    PaymentName(""),
                    PaymentGroup(0),
                    TransQty(0),
                    BlindBalance(0),
                    SystemBalance(0){}

   TBlindBalance( AnsiString inPaymentName, int inPaymentGroup,
                  int inTransQty, Currency inBlindBalance, Currency inSystemBalance) :
                  PaymentName(inPaymentName),
                  PaymentGroup(inPaymentGroup),
                  TransQty(inTransQty),
                  BlindBalance(inBlindBalance),
                  SystemBalance(inSystemBalance){}

    AnsiString PaymentName;
    int PaymentGroup;
    int TransQty;
    Currency BlindBalance;
    Currency SystemBalance;
    void operator = (TBlindBalance rhs);
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
    void SetTotal(Currency sum);
    Currency GetTotal(void);
    void ClearTotal(void);
    int size();
    void SetBalance(TBlindBalance &Balance);
    void UpdateSystemBalance(AnsiString Payment, Currency Amount);
    void UpdateBlindBalance(AnsiString Payment, Currency Amount);
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

class TBlindBalanceController
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
        void OnDrawCell(int ARow, int ACol = -1);
		void PopulateListManager();
		bool WarnOperator(void);
		void LoadBlindBalances(void);
		void UpdateBlindBalances(AnsiString BagID);
        bool IsMaster;
   public :
		TBlindBalances BlindBalances;
		static TBlindBalances MasterBalance;
		TBlindBalanceController(TForm *inDisplayOwner,Database::TDBTransaction &inDBTransaction,bool isMaster);
		TBlindBalanceController(TForm *inDisplayOwner,Database::TDBTransaction &inDBTransaction,bool isMaster, AnsiString DeviceName);
		bool Run();
		AnsiString GetBagID(void);
		TBlindBalances Get();
		static TBlindBalances GetMaster();
};

TBlindBalances TBlindBalanceController::MasterBalance;
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
