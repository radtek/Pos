//---------------------------------------------------------------------------

#ifndef CashDenominationControllerH
#define CashDenominationControllerH
//---------------------------------------------------------------------------
#include <memory>
#include "ListManager.h"
#include "DBDenominations.h"

typedef std::map<int, TDenomination > TCashDenominationContainer;
class TCashDenominations
{
	TCashDenominationContainer CashDenominations;
	Currency Total;
   public :
    TCashDenominationContainer::iterator begin();
    TCashDenominationContainer::iterator end();
    int size();
    Currency GetTotal();
    void ResetTotal();
    void AddToTotal(Currency amount);
    void UpdateDenominationQuantity(int key, int Amount);
    int GetDenominationQuantity(int key);
    void SetCashDenomination(TDenomination denomination);
    bool IndexValid(Index);
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
		void OnEdit(int ItemIndex, int ColIndex = -1);
		void OnDelete(int ItemIndex, int ColIndex = -1);
        void OnDrawCell(int ARow, int ACol = -1);
		void PopulateListManager();
		bool WarnOperator(void);
        int SelectedRow;
   public :
		TCashDenominations CashDenominations;
		static TCashDenominations MasterCashDenominations;
		TCashDenominationController(TForm *inDisplayOwner,Database::TDBTransaction &inDBTransaction);
		TCashDenominationController(TForm *inDisplayOwner,Database::TDBTransaction &inDBTransaction, AnsiString DeviceName);
        bool PopulateDenominationValues(Database::TDBTransaction &DBTransaction);
		bool Run();
		AnsiString GetBagID(void);
		TCashDenominations Get();
		static TCashDenominations GetMaster();
};

TCashDenominations TCashDenominationController::MasterCashDenominations;
class TCashDenominationControllerInterface
{
 private:
   TCashDenominations CashDenominations;
   TCashDenominations MasterCashDenominations;
   AnsiString BagId;
   static TCashDenominationControllerInterface* instance;

        // Constructor and Destructor
        TCashDenominationControllerInterface();
        ~TCashDenominationControllerInterface();

    public:
        static TCashDenominationControllerInterface* Instance()
        {
            if(instance == NULL)
                instance = new TCashDenominationControllerInterface();

            return instance;
        }
     void SetCashDenominations(TCashDenominations inCashDenominations,bool isMaster = true);
     TCashDenominations GetCashDenominations(bool isMaster = true);
     void SetBagID(AnsiString bagId);
     AnsiString GetBagID();
     void ResetCashDenominations();
     void SaveDenominations(Database::TDBTransaction &DBTransaction,int z_key,UnicodeString inTerminalName);
};
#endif
