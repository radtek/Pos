//---------------------------------------------------------------------------

#ifndef WriteOffH
#define WriteOffH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>

#include "Forms.hpp"
#include "MMContactInfo.h"
#include "MM_DBCore.h"
#include "ItemComplete.h"

//---------------------------------------------------------------------------


class TWriteOff
{
public:
	TWriteOff(TForm * inOwner);
	void InsertToDatabase(Database::TDBTransaction &DBTransaction, TList *OrderObjects, AnsiString inStaff, std::vector<int> &inKeys);
	void InsertToDatabase(Database::TDBTransaction &DBTransaction, TItemComplete *inOrder, AnsiString inStaff, std::vector<int> &inKeys);
	void InsertToDatabase(Database::TDBTransaction &DBTransaction, TItemCompleteSub *inOrder, AnsiString inStaff, std::vector<int> &inKeys);
	void runReason(void);
	AnsiString GetReason(void){return Reason;}

private:
	TForm *Owner;
	AnsiString Reason;
	int Parent_Key;
};
#endif
