//---------------------------------------------------------------------------
#ifndef MMdataH
#define MMdataH
//---------------------------------------------------------------------------

#include "Requests.h"

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <Dialogs.hpp>
#include "enum.h"
#include "MMLogging.h"
#include <DB.hpp>
#include <IBCustomDataSet.hpp>
#include <IBQuery.hpp>
#include <IBSQL.hpp>
#include <ImgList.hpp>
#include <IBDatabase.hpp>

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif

#include "Comms.h"
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBTable.hpp>
#include <IBStoredProc.hpp>
#include <IBQuery.hpp>
#include <IBSQL.hpp>
#include <ImgList.hpp>
//#include "BrowseDr.hpp"
#include <IBUpdateSQL.hpp>
#include <IBEvents.hpp>
#include <DB.hpp>
#include <IBSQLMonitor.hpp>
#include <set>
#include <vector>
#include <iterator>
#include "ZForm.h"

//---------------------------------------------------------------------------
#ifdef PalmMate
#include "Palm.h"
#include "PalmMenus.h"
#endif

class TReqRecover;
class TReqPrintJob;
class TDevice;
class TLogin;
class TTableSeat;
class TReqPartialPayment;
struct SZone;
class TAccess;
class TReqLocationCheck;
class TReqSpecialTimeCheck;
class TReqMenuChange;
class TReqMenuAlter;
class TReqShutDown;

class TfrmData : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TImageList *ilTreeImages;
   TFontDialog *FontDialog;
   TPrinterSetupDialog *PrinterSetup;
   TIBSQL *IBSQL;
//   TdfsBrowseDirectoryDlg *Browse;
   TIBQuery *IBQueryDS;
   TIBDataSet *IBMenu;
   TIBDataSet *IBCourse;
   TIBDataSet *IBItem;
   TIBDataSet *IBOption;
   TIBDataSet *IBSize;
   TIBDataSet *IBItemSize;
   TDataSource *dsMenu;
   TDataSource *dsCourse;
   TDataSource *dsItem;
	TIBDataSet *IBRecipe;
   TDataSource *dsItemSize;
	TDataSource *dsQuery;
   TIBQuery *IBQuery;
   TIBQuery *IBAdjust;
	TIBQuery *IBQuerySecondary;
	TIBDataSet *IBMessages;
	TDataSource *dsMessages;
   TIBDataSet *IBCategories;

   TIBQuery *IBSecQuery;
	TIBDataSet *IBItemSide;

	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall CreateNewRecord(TDataSet *DataSet);
private:		// User declarations
	TFileStream *OrderLog;
	__fastcall TfrmData(TComponent* Owner);
public:		// User declarations
	void __fastcall dbTableFindOrCreate(SZone *Zone);


	void TreeViewChangeFont();
	TMemoryStream *MenuSync1Stream;
	TMemoryStream *MenuSync2Stream;
   // The Sync Number that hold the Plams in Sync with the PC menu orders.
	// this is updated every time the user makes a change to the Wroking DB.
   bool RequestClose;

	AnsiString GetPathFromUser(AnsiString Message);

	void SetTableName(AnsiString Name, int TableNo);
	bool SetTabName(AnsiString Name, long TabKey);
	bool LocateTableSeat(TTableSeat *TableSeat);
	bool LocateTab(long TabKey);

protected:
};

enum MenuType {mtMenu,mtCourse,mtDish,mtBevs,mtClass,mtBev,mtOpts,mtOpt,mtTable,mtSeat,mtNote,mtTotal};

struct MenuObj
{
   // Holds the Local Key on the Tree for the Dish/Course/Option etc... Ref back into DB
   long Key;
   // Holds Table Number seat number.
   int Number;
   // This hold the Local DB Key in the Orders table so we can tell what Dish/Beverage Order this node is.
   //  This is only used when you sigle click on a seat and manually add and remove orders using the Menu tree.
   long OrderKey;
   // Holds the CourseLK for the Dish on the Table/Seat side of the Tree Page.
   // This is used to lookup the OrdCourse table to allow the removal of a dish from a course.
   long CourseKey;
   // Type of Node.
   MenuType mtType;
   // Used to store the note (if any) for a order so it can easly be accessed on the tree.
   AnsiString Note;
   // Hint that will appear when user holds mose over the tree view.
   AnsiString Hint;
   // Contains the Course ID / Dish ID etc... for this node if applicable.
   int CourseID;
   int Item_ID;
   bool Enabled;
};

//---------------------------------------------------------------------------

struct SZone
{
   long TableNo;
   long SeatNo;
   long TabKey;                    // Tab LK
   AnsiString TabName;
   AnsiString TableName;
   AnsiString PartyName;
   TMMTabType TabStatus;
public:
    SZone();
};

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
extern PACKAGE TfrmData *frmData;
//---------------------------------------------------------------------------
#endif
