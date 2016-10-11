//---------------------------------------------------------------------------
#ifndef MMDataH
#define MMDataH
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
class TdmMMData : public TDataModule
{
__published:	// IDE-managed Components
	TImageList *ilTreeImages;
	TImageList *ilStateImages;
	TIBDatabase *dbMenuMate;
	TIBTransaction *trMenuMate;
	TIBDatabaseInfo *dbinfMM;
	TIBDataSet *dtUsers;
	TIBDataSet *dtUsersTime;
	TDataSource *dsUsers;
	TIBSQLMonitor *IBSQLMonitor1;
	TIBDataSet *dtMenu;
	TDataSource *dsMenu;
	TIBDataSet *dtCourses;
	TDataSource *dsCourses;
	TIBDataSet *dtSizes;
	TIBDataSet *dtOptions;
	TIBDataSet *dtItems;
	TDataSource *dsItems;
	TDataSource *dsItemSizes;
	TIBDataSet *dtRecipe;
	TDataSource *dsLocations;
	TIBQuery *qrLocations;
	TIBSQL *qrCategories;
	TIBQuery *qrSizeCategories;
	TIBQuery *dtItemSizes;
	TIBQuery *IBQuery;
	TImageList *ImageList1;
	TIBValidationService *ValidationService;
	TIBQuery *qrItemSides;
    TIBSQL *IBSQL1;
	void __fastcall IBSQLMonitor1SQL(AnsiString EventText,
          TDateTime EventTime);
private:	// User declarations

public:		// User declarations
	__fastcall TdmMMData(TComponent* Owner);

	bool Connect(TLabel *Label);
	void CloseDB();
	void Disconnect();
	void Sweep(AnsiString DatabaseName);
    AnsiString GetTerminalName();
	void Registered(bool *pRegistered, unsigned int *pRegisteredModules=0, AnsiString *pRegisteredName=NULL);
};
//---------------------------------------------------------------------------
extern PACKAGE TdmMMData *dmMMData;
//---------------------------------------------------------------------------
#endif
