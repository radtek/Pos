//---------------------------------------------------------------------------
#ifndef ChefMateDataH
#define ChefMateDataH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <ImgList.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBTable.hpp>
#include <IBDatabaseInfo.hpp>
#include "Setup.h"
#include <IBSQL.hpp>
#include <DB.hpp>
#include <IBSQLMonitor.hpp>
#include <IBQuery.hpp>
#include <IBServices.hpp>
//---------------------------------------------------------------------------
class TdmChefMateData : public TDataModule
{
__published:	// IDE-managed Components
	TImageList *ilTreeImages;
	TImageList *ilStateImages;
    TIBDatabase *dbChefMate;
    TIBTransaction *trChefMate;
    TIBDatabaseInfo *dbinfChefMate;
	TIBSQLMonitor *IBSQLMonitor1;
	TIBQuery *IBQuery;
	TImageList *ImageList1;
	TIBValidationService *ValidationService;
	void __fastcall IBSQLMonitor1SQL(AnsiString EventText,
          TDateTime EventTime);
private:	// User declarations
public:		// User declarations
	__fastcall TdmChefMateData(TComponent* Owner);

	bool Connect(TLabel *Label);
	void CloseDB();
	void Disconnect();
	void Sweep(AnsiString DatabaseName);
};
//---------------------------------------------------------------------------
extern PACKAGE TdmChefMateData *dmChefMateData;
//---------------------------------------------------------------------------
#endif
