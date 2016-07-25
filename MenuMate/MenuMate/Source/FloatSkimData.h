//---------------------------------------------------------------------------

#ifndef FloatSkimDataH
#define FloatSkimDataH
//---------------------------------------------------------------------------
#include "Forms.hpp"
#include "MM_DBCore.h"
#include "MMContactInfo.h"


enum FloatSkimTransaction {eRefloat, eSkim, eNochange, eInitial};
const AnsiString Transaction_types[4] = {"Deposit", "Withdrawal", "No Change", "Initial"};



class TFloatSkimData
{
	public:

	TFloatSkimData(TForm * inOwner, TMMContactInfo info, FloatSkimTransaction type, Currency inAmount, int inZed_Key);

	void PrintDocket(void);
	void InsertToDatabase(Database::TDBTransaction &DBTransaction);
	void runReason(void);

	private:
		TForm *Owner;
		TMMContactInfo UserInfo;
		FloatSkimTransaction TransType;
		Currency Amount;
		int Z_Key;
		AnsiString Reason;
};

#endif


