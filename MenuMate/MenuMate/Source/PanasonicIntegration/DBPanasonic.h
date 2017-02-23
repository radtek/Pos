//---------------------------------------------------------------------------

#ifndef DBPanasonicH
#define DBPanasonicH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include "DBAccess.hpp"
#include "SQLServerUniProvider.hpp"
#include "Uni.hpp"
#include "UniProvider.hpp"
#include <DB.hpp>
#include "MemDS.hpp"
#include "PanasonicModels.h"
//---------------------------------------------------------------------------
class TDBPanasonic
{
private:	// User declarations
    TSQLServerUniProvider *SQLServerUniProvider;
    TDataSource *DataSource;
    TUniQuery *UniInsertQuery;
    TComponent *Owner;
public:		// User declarations
    TDBPanasonic();
    TUniConnection *UniDataBaseConnection;
    void SendDataToServer(TPanasonicModels &panasonicModels);
    void InsertItemsToTItemList(TPanasonicItemList &itemList);
    void InsertProductDetailsInToTProduct(TPanasonicProduct &product);
    void InsertTransactionDBServerInformation(TPanasonicTransactionDBServerInformation &serverInfo);
};

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif