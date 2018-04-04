//---------------------------------------------------------------------------
#ifndef StockDataH
#define StockDataH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBTable.hpp>
#include <IBSQL.hpp>
#include <IBQuery.hpp>
#include <IBSQLMonitor.hpp>
#include <IBServices.hpp>
#include <IBDatabaseInfo.hpp>
#include "Setup.h"
#include <DB.hpp>
//---------------------------------------------------------------------------
namespace DBVersion
{
	enum DBVersions
	{
		V2_4,
		V2_7,
		V3_0,
		V3_3,
		V3_31,
		V3_4,
		V4_0,
		V5_0,
        V5_5,
        V5_6,
        V5_7,
        V6_0,
        V6_1,
        V6_2,
        V6_2_1 ,
        V6_2_2,
        V6_2_3,
        V6_18_0,
        V6_22_0,
        V6_23_0,
        V6_24_0,
	};
};
//---------------------------------------------------------------------------
class TdmStockData : public TDataModule
{
__published:	// IDE-managed Components
	TDataSource *DataSource1;
	TIBDatabase *dbStock;
	TIBTransaction *trStock;
	TIBQuery *IBQuery1;
	TIBQuery *IBQuery2;
	TIBBackupService *IBBackupService1;
	TIBDatabaseInfo *dbinfStock;
	TIBSQL *Query;
	TIBSQL *sqlNow;
	TIBTransaction *trTime;
	TIBValidationService *ValidationService;
	TIBRestoreService *IBRestoreService1;
private:	// User declarations
	bool HasDBVersion(AnsiString Version);
	bool RequiresUpdateTo(DBVersion::DBVersions version);
	void GetTableNames(TStrings *Fields);
    void UpdateAverageCost(TStrings *Fields);
	void GetFieldNames(AnsiString TableName, TStrings *Fields);
	bool WaitForSingleUser();
	bool BackupDB(AnsiString FileName, TLabel *Label);
	bool RestoreDB(AnsiString FileName, TLabel *Label, bool ReplaceDB);
	bool Update2_4();
	bool Update2_7();
	bool Update3_0();
	bool Update3_31();
	bool Update4_0();
	bool Update5_0();
    bool Update5_5();
    bool Update5_6();
    bool Update5_7();
    bool Update6_0();
    bool Update6_1();
    bool Update6_2();
    bool Update6_2_1();
    bool Update6_2_2();
    bool Update6_2_3();
    bool Update6_18_0();

    void createGenerators5_7(TIBQuery* query);
    void createTables5_7(TIBQuery* query);

    void setDBVersion(TIBQuery* query, AnsiString version);

    void RunSQL( AnsiString s );
    void RestartTransaction();
    bool FieldExists(AnsiString tableName, AnsiString fieldName);
    bool TableExists(AnsiString tableName);
    bool GeneratorExists(AnsiString generatorName);
    bool TriggerExists(AnsiString triggerName);
    bool ConstrantExists(AnsiString constrantName);
    void CreateGenerators6_0();
    void UpdateTables6_0();
    void CreateTables6_0();
    void CreateTrigger6_0();
    void UpdateTables6_1();
    void UpdateTables6_2_2();
    void CreateGenerators6_2_3();
    void UpdateTables6_2_3();
    bool HasDBVersionExist(AnsiString Version);
    public:		// User declarations
	__fastcall TdmStockData(TComponent* Owner);
	bool CreateDB(AnsiString DatabaseName);
	bool Connect(TLabel *Label);
	void Disconnect();
	void CloseDB();
	void Sweep(AnsiString DatabaseName);
	bool UpdateDB(TLabel *Label);
    AnsiString DefaultLocation;
    void UpdateTables6_22_0();
    bool Update6_22_0();
    void UpdateTables6_23_0();
    bool Update6_23_0();
    void  UpdateTables6_24_0();
    bool Update6_24_0();

//	bool TransferStock(TIBTransaction *Transaction, int Key, double Quantity,
//				AnsiString Source, AnsiString Destination, AnsiString UserName, AnsiString UserID);
//	bool CreateStockLocation(TIBTransaction *Transaction, int Key, AnsiString Location);
};
//---------------------------------------------------------------------------
extern PACKAGE TdmStockData *dmStockData;
//---------------------------------------------------------------------------
#endif
