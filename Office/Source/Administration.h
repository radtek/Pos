//---------------------------------------------------------------------------

#ifndef AdministrationH
#define AdministrationH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <IBDatabase.hpp>
#include <IBSQL.hpp>
#include <DB.hpp>
#include <IBCustomDataSet.hpp>
#include <IBQuery.hpp>
#include <IBServices.hpp>
#include <IBSQLMonitor.hpp>
#include <IBDatabaseInfo.hpp>
#include <ComCtrls.hpp>
#include "UpdateCategories.h"
#include "SyncMenuToRecipeForm.h"
#include <DBTables.hpp>
#include "CSV.h"


#include "SelectImportType.h"
//---------------------------------------------------------------------------
class TfrmAdministration : public TForm
{
__published:	// IDE-managed Components
	TOpenDialog *dlgOpen;
	TPanel *Panel2;
	TPanel *Panel13;
	TBitBtn *btnClose;
	TPanel *Panel5;
	TPanel *Panel10;
	TPanel *Panel11;
	TLabel *Label3;
	TPanel *Panel12;
	TBitBtn *btnClearSuppliers;
	TBitBtn *btnClearStock;
	TBitBtn *btnClearHistory;
	TSaveDialog *dlgSave;
	TIBSQL *qrAddSupplier;
	TIBTransaction *Transaction;
	TIBSQL *qrLK;
	TIBSQL *qrAddCategory;
	TIBQuery *qrFindCategory;
	TIBSQL *qrAddGroup;
	TIBQuery *qrFindGroup;
	TIBSQL *qrAddStock;
	TIBSQL *qrAddStockLocation;
	TIBQuery *qrFindSupplier;
	TIBSQL *qrAddStockSupplier;
	TIBSQL *qrClearSuppliers;
	TIBSQL *qrClearStock;
	TIBSQL *qrClearTransBatch;
	TBitBtn *btnImport;
	TBitBtn *btnBarcodes;
	TPanel *Panel1;
	TBitBtn *btnCreateDB;
	TIBSQL *qrClearImportTrack;
	TIBSQL *qrClearPurchaseOrders;
	TIBSQL *qrStocktakeProfiles;
	TBitBtn *btnAddStockLocations;
	TIBSQL *qrStockList;
	TBitBtn *btnShowReceipt;
	TIBQuery *qrReceipt;
	TMemo *memReceipt;
	TPanel *Panel3;
	TBitBtn *btnIBLog;
	TIBLogService *IBLogService;
	TBitBtn *BitBtn6;
	TIBSQLMonitor *IBSQLMonitor;
	TPanel *Panel4;
	TBitBtn *btnRepairMM;
	TBitBtn *btnRepairStock;
	TIBDatabaseInfo *IBDatabaseInfo;
	TIBQuery *qrContactGroup;
	TIBQuery *qrAddContactGroup;
	TIBQuery *qrLocations;
	TProgressBar *ProgressBar1;
	TBitBtn *BitBtn1;
	TIBSQL *qrCategories;
	TIBSQL *qrClearPurchaseStock;
	TIBSQL *qrClearStocktake;
	TIBSQL *qrAddBarcode;
	TButton *btnSecurity;
	TButton *btnUsers;
    TIBSQL *IBQuery;
    TIBQuery *qrUpdateStock;
    TIBQuery *qrFindStock;
    TIBQuery *qrUpdateLocation;
    TIBQuery *IBQuery1;
    TIBQuery *qrUpdateStockSupplier;
    TIBSQL *qrClearBarcodes;
   TIBSQL *qrImportSupplier;
   TIBQuery *qrUpdateSupplier;
    TIBTransaction *MMTransaction;
    TIBSQL *qrCSV;
    TIBSQL *qrcsv1;
    TIBSQL *qrGetPriceLevelLimit;
	void __fastcall btnImportClick(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall btnBarcodesClick(TObject *Sender);
	void __fastcall btnClearSuppliersClick(TObject *Sender);
	void __fastcall btnClearStockClick(TObject *Sender);
	void __fastcall btnCreateDBClick(TObject *Sender);
	void __fastcall btnClearHistoryClick(TObject *Sender);
	void __fastcall btnAddStockLocationsClick(TObject *Sender);
	void __fastcall btnShowReceiptClick(TObject *Sender);
	void __fastcall IBSQLMonitorSQL(AnsiString EventText,
          TDateTime EventTime);
	void __fastcall BitBtn6Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall btnIBLogClick(TObject *Sender);
	void __fastcall btnRepairMMClick(TObject *Sender);
	void __fastcall btnRepairStockClick(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall btnSecurityClick(TObject *Sender);
	void __fastcall btnUsersClick(TObject *Sender);
private:	// User declarations
	void RepairDB(TIBDatabase *IBDatabase);
	bool GetCmdLnOutput(AnsiString ExecCmd, TStrings *StringList);
    AnsiString Query(AnsiString Field, AnsiString retval, AnsiString Data);
   void ImportStock();
   void ImportSupplier();
    bool DuplicateCodes(TCsv *ImportCSV);
    int DuplicateCodesBikeShop(TCsv *ImportCSV,AnsiString &StockCode,int &Row);

   bool validate_weborder_target_uri(AnsiString uri);
	CategoryTranslation::TUpdateArchiveCategories *UpdateArchiveCategories;

public:		// User declarations
	__fastcall TfrmAdministration(TComponent* Owner);
    	bool Registered;
	unsigned int Modules;

    bool  NormalImportCaseValidations(TCsv *ImportCSV,bool menuUpdate,bool recipeupdate,bool itemSizeUniqueCase);
    bool CheckTaxFormat( AnsiString taxString,int col,int row);

     //functions for bikeshop import
    void BikeShopImport();
    bool SaveStockImport(TCsv *ImportCSV);
    int StockCode;
    TCsv ImportCSV;

   bool normalImport;
   bool removeDuplicateStockCode;
   bool CheckDuplicateStockCode(AnsiString StockCode);
};


class TservingCourse
{

   public :
    int servingCourseKey;
    AnsiString servingCourseName;

};

 class Tcourse
{
   public :
    int courseKey;
    AnsiString courseName;
    int corrospondingServingCourseKey;
    AnsiString  corrospondingServingCourseName;
};


 class Titem
{
   public :
    int itemKey;
    int courseKey;
    AnsiString itemName;
    AnsiString categoryGroup;

};
class TitemSizeTaxProfile
{
   public:
   int taxProfileKey;
   AnsiString taxName;
   double taxPercent;
   int taxType;


};

class Trecipe
{
 public:

 int qty;
 int recipeKey;
 AnsiString location ;
 AnsiString  stockCode;


};

class TitemSize : public TitemSizeTaxProfile ,public  Trecipe
{
   public :

    int itemSizeKey;
    AnsiString itemSizeName;

    int categoryGroupKey;
    AnsiString categoryGroupName;

    int categoryKey;
    AnsiString categoryName;

    double itemSizePrice;
    int courseKey;

    int itemKey;
    AnsiString itemName;

   std::vector<TitemSizeTaxProfile> itemSizeTaxes;
   std::vector<Trecipe> itemSizeRecipe;

   int sizeFKey;



};

class TallTaxProfiles
{
  public :

   int taxProfileKey;
   AnsiString  taxName;
   double taxPercent;
   int taxType;
};

class Tcategory
{
    public:

   int categoryKey;
   int categoryGroupKey;
   AnsiString categoryName;

};

class TcategoryGroup
{
    public:

    int categoryGroupKey;
    AnsiString categoryGroupName;


};

class Tsize
{
  public:
  int menuKey;
  int sizeKey;
  AnsiString sizeName;


};





class TimportMenu: public TservingCourse, public Tcourse ,public Titem ,public TitemSize ,public TallTaxProfiles,public TcategoryGroup,public Tcategory ,public Tsize
{

   public:
    bool itemSizeUnique;
    AnsiString menuName;
    double  menuType;
    int menuKey;
    int servingCourseKey;
    int courseKey;
    int itemKey;
    int itemSizeKey;
    int taxProfileKey;
    int categoryGroupKey;
    int categoryKey;
    int sizeKey;
  

    std::vector<TimportMenu>  FetchAllMenusInVector(TCsv *ImportCSV,TIBSQL *qrCSV,bool doRecipeUpdate,TIBSQL *qrcsv1,bool RemoveRecipe,TIBSQL *qrStockQuery);
    int  GetOrCreateMenuKey(AnsiString  menuName,TIBSQL *qrCSV);
    void SaveServingCourses(TCsv *ImportCSV, int row,TimportMenu &menu,TIBSQL *qrCSV);
    void SaveCourses(TCsv *ImportCSV, int row,TimportMenu &menu,TIBSQL *qrCSV);
    void SaveItem(TCsv *ImportCSV, int row,TimportMenu &menu,TIBSQL *qrCSV);
    void SaveItemSize(TCsv *ImportCSV, int row,TimportMenu &menu,TIBSQL *qrCSV,bool doRecipeUpdate);

    void SaveItemSizeTaxes(TCsv *ImportCSV, int row,TimportMenu &menu,TitemSize &itemSize,TIBSQL *qrCSV);
    double FetchTaxPercent( std::string percentString);


    void  LoadAllTaxProfiles(TimportMenu &menu,TIBSQL *qrCSV);
    void  LoadallCategoryGroups(TimportMenu &menu,TIBSQL *qrCSV);
    void  LoadallCategories(TimportMenu &menu,TIBSQL *qrCSV);
    void  LoadallSize(TimportMenu &menu,TIBSQL *qrCSV,TCsv *ImportCSV);
    void  LoadallItems(TimportMenu &menu,TIBSQL *qrCSV);
    void  LoadAllServingCourses(TimportMenu &menu,TIBSQL *qrCSV);
    void  LoadallUnsoldItemSize(TimportMenu &menu,TIBSQL *qrCSV,TIBSQL *qrCSV1,TCsv *ImportCSV);

    std::string delUnnecessary(std::string &str);

    std::string GetTaxName(std::string taxString);
    double GetTaxPercent(std::string taxString);
    int GetTaxKey(AnsiString taxName, double taxPercent,int taxType,TimportMenu &menu,TIBSQL *qrCSV);



    void SaveItemSizeCategoryGroup(TCsv *ImportCSV,int row,TimportMenu &menu,TitemSize &itemSize,TIBSQL *qrCSV);
    void SaveItemSizeCategory(TCsv *ImportCSV,int row,TimportMenu &menu,TitemSize &itemSize,TIBSQL *qrCSV);

    std::vector<TservingCourse> allServingCourse;
    std::vector<Tcourse> course;
    std::vector<Titem> allItem;


    std::vector<TallTaxProfiles> allTaxProfiles;
    std::vector<TcategoryGroup> allCategoryGroups;
    std::vector<Tcategory> allCategories;
    std::vector<Tsize> allMenuSize;
    std::vector<TitemSize> allItemSizeInMenu;
    bool CheckDuplicateStockCodeInDatabase(AnsiString StockCode ,TIBSQL *qrStockQuery);

};

class Tmenucontainer :public TimportMenu
{
  public:
    std::vector<TimportMenu> allCsvMenus;

};

//---------------------------------------------------------------------------
//extern PACKAGE TfrmAdministration *frmAdministration;
//---------------------------------------------------------------------------
#endif
