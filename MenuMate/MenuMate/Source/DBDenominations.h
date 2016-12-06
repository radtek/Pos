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
    static void AddDenominations(Database::TDBTransaction &DBTransaction,TDenomination inDenomination);
    static void LoadDenominations(Database::TDBTransaction &DBTransaction,std::vector<TDenomination> &inDenominations);
    static void DeleteDenominations(Database::TDBTransaction &DBTransaction,int key);
    static Currency GetDenominationValue(Database::TDBTransaction &DBTransaction,int key);
    static AnsiString GetDenominationTitle(Database::TDBTransaction &DBTransaction,int key);
    static void SetDenominationValue(Database::TDBTransaction &DBTransaction,int key,Currency inValue);
    static void SetDenominationTitle(Database::TDBTransaction &DBTransaction,int key,AnsiString inTitle);
    static int GetDenominationKey(Database::TDBTransaction &DBTransaction);
    static bool IsDenominationExist(Database::TDBTransaction &DBTransaction,AnsiString inTitle);
};
#endif
