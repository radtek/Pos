//---------------------------------------------------------------------------

#ifndef DBDenominationsH
#define DBDenominationsH
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include "MM_DBCore.h"



struct TDenomination
{
    int Key;
    AnsiString Title;
    Currency DenominationValue;
    int Quantity;
    Currency Total;
};

//---------------------------------------------------------------------------
class TDBDenominations
{
   public:
    TDBDenominations();
    ~TDBDenominations();
    static void SaveDenomination(Database::TDBTransaction &DBTransaction,TDenomination inDenomination);
    static void AddDenomination(Database::TDBTransaction &DBTransaction,TDenomination inDenomination);
    static void UpdateDenomination(Database::TDBTransaction &DBTransaction,TDenomination inDenomination);
    static void LoadDenominations(Database::TDBTransaction &DBTransaction,std::vector<TDenomination> &inDenominations);
    static void DeleteDenominations(Database::TDBTransaction &DBTransaction,int key);
    static Currency GetDenominationValue(Database::TDBTransaction &DBTransaction,int key);
    static AnsiString GetDenominationTitle(Database::TDBTransaction &DBTransaction,int key);
    static int GetDenominationKey(Database::TDBTransaction &DBTransaction);
    static bool IsDenominationExist(Database::TDBTransaction &DBTransaction, int key, AnsiString inTitle);
    static void TDBDenominations::SaveZedDenominations(Database::TDBTransaction &DBTransaction,
                                            int zedKey,
                                            UnicodeString inTerminalName,
                                            UnicodeString inDenominationTitle,
                                            Currency inDenominationValue,
                                            int  inDenominationQty);
};
#endif
