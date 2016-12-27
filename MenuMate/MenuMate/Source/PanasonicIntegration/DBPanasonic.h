//---------------------------------------------------------------------------

#ifndef DBPanasonicH
#define DBPanasonicH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "DBAccess.hpp"
#include "SQLServerUniProvider.hpp"
#include "Uni.hpp"
#include "UniProvider.hpp"
#include <DB.hpp>
#include "MemDS.hpp"
#include "PanasonicModels.h"
//---------------------------------------------------------------------------
class TfrmDBPanasonic : public TForm
{
__published:	// IDE-managed Components
    TUniConnection *UniDataBaseConnection; 
    TSQLServerUniProvider *SQLServerUniProvider1;
    TDataSource *DataSource1;
    TUniQuery *UniInsertQuery;
private:	// User declarations
public:		// User declarations
    __fastcall TfrmDBPanasonic(TComponent* Owner);
    void SendDataToServer(TPanasonicModels &panasonicModels);
    void InsertItemsToTItemList(TPanasonicItemList &itemList);
    void InsertProductDetailsInToTProduct(TPanasonicProduct &product);
    void InsertTransactionDBServerInformation(TPanasonicTransactionDBServerInformation &serverInfo);
};
extern PACKAGE TfrmDBPanasonic *frmDBPanasonic;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif
