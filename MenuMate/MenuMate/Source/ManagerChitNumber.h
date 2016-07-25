//---------------------------------------------------------------------------

#ifndef ManagerChitNumberH
#define ManagerChitNumberH
//---------------------------------------------------------------------------
#include <system.hpp>
#include <vector>
#include "MM_DBCore.h"
#include "ChitNumber.h"
#include "DBChit.h"
#include "Graphics.hpp"
#include "DeviceRealTerminal.h"

class TChitStats
{
	public:
		int HeldOrderCount;
		bool IsLatest;
		TChitStats::TChitStats(){IsLatest = false;HeldOrderCount=0;}
};

class TManagerChitNumber
{
   private :
    std::map<int,TChitNumber>	ChitNumbers;
	std::map<int,TChitNumber>::iterator ptrChitNumbers;
	std::map<UnicodeString,TChitStats> ChitStatistics;
    int CurrentKey;
	int TerminalKey;
	int LastAccesedChitKey;
    void ClearDefault(Database::TDBTransaction &DBTransaction);

    static const chit_option_name_set_t no_options;
    static const chit_option_name_set_t no_option_additions;
    static const chit_option_name_set_t no_options_deleted;
    static const chit_option_rename_map_t no_options_renamed;

    bool listed_chit_present_;

   void UpdateRemovedTaxList(TChitNumber &ChitNumber);
   void AddRemovedTaxes(Database::TDBTransaction &DBTransaction, int chit_key, int tax_profilekey);
   void DeleteRemovedTaxes(Database::TDBTransaction &DBTransaction, int chit_key) ;
   void GetRemovedTaxesList(Database::TDBTransaction &DBTransaction, int chit_key, TStringList* removelist, int type);
   void LoadRemovedTaxesList(Database::TDBTransaction &DBTransaction,TChitNumber &ChitNumber);

   void DeleteAppliedDiscounts(Database::TDBTransaction &DBTransaction, int chit_key);
   void AddAppliedDiscounts(Database::TDBTransaction &DBTransaction, int chit_key, int discount_key);
   void GetAppliedDiscountLists(Database::TDBTransaction &DBTransaction, int chit_key, TStringList* discountlist);
   void UpdateDiscountLists(TChitNumber &ChitNumber);
   void LoadAppliedDiscountLists(Database::TDBTransaction &DBTransaction,TChitNumber &ChitNumber);



   public :
	const bool ListedChitsArePresent() const;

	static TManagerChitNumber& Instance() {
        static TManagerChitNumber singleton;
        return singleton;
    }

      TManagerChitNumber();
      void LoadAll(Database::TDBTransaction &DBTransaction);
      void Load(Database::TDBTransaction &DBTransaction);
      void Initialise(Database::TDBTransaction &DBTransaction,int DeviceKey);
      void Add(Database::TDBTransaction &DBTransaction,
               TChitNumber &ChitNumber,
               const chit_option_name_set_t &options = no_options);
      void Remove(Database::TDBTransaction &DBTransaction,int ChitNumberKey);
      void Update(Database::TDBTransaction &DBTransaction,
                  TChitNumber ChitNumber,
                  const chit_option_name_set_t &options_to_be_added =
                    no_option_additions,
                  const chit_option_name_set_t &options_to_be_deleted =
                    no_options_deleted,
                  const chit_option_rename_map_t &options_to_be_renamed =
                    no_options_renamed);
      void First(bool EnabledCodesOnly = true);
      void Next(bool EnabledCodesOnly = true);
      bool Eof();
	  bool Bof();
      bool Empty();
      int Size();

      TChitNumber &ChitNumber();
      TChitNumber &ChitNumberByKey(int ChitNumberKey);
   	  void GetNextChitNumber(Database::TDBTransaction &DBTransaction,TChitNumber &Chit);
      void ResetChitNumber(Database::TDBTransaction &DBTransaction);      
      TChitNumber GetDefault();
	  bool GetDefaultEnabled();
	  void ReLoadChitStatistics(Database::TDBTransaction &DBTransaction);
	  UnicodeString GetColorForChitOption(UnicodeString chitoption);
	  void SetLastAccessedChitKey(int key){ LastAccesedChitKey = key;}
	  int GetLastAccessedChitKey(){ return LastAccesedChitKey;}
      TChitNumber LoadFromKey(Database::TDBTransaction &DBTransaction, int ChitKey);
      //check for online pickup or delivery
      bool CheckIsOnLinePickupOrDeliveryChecked(Database::TDBTransaction &DBTransaction, AnsiString caption, TStringList *getList);

      int GetMaxChitNumber();

};

#endif
