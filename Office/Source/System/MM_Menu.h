//---------------------------------------------------------------------------
#ifndef MM_MenuH
#define MM_MenuH
#include <Classes.hpp>
#include <Graphics.hpp>
#include <IBDatabase.hpp>
#include <IBSQL.hpp>
//#include <Dialogs.hpp>
//#include <memory>
#include "MM_DBCore.h"

#include <vector>
#include <set>
//---------------------------------------------------------------------------
namespace Menu
{
//---------------------------------------------------------------------------
const AnsiString MenuSQL =
	"Select "
		"Menu.Menu_Key,"
		"Menu.Menu_Name,"
		"Menu.Menu_Type,"
		"Menu.Palmable,"

		"Course.Course_Key,"
		"Course.Course_ID,"
		"Course.Course_Name,"
		"Course.Course_Kitchen_Name,"
		"Course.View_Location,"
		"Course.No_Default_Serving_Course,"

		"Item.Item_Key,"
		"Item.Item_Name,"
		"Item.Item_Kitchen_Name,"
        "Item.ITEM_IDENTIFIER,"

		"Item.Enabled,"
		"Item.Button_Colour,"
		"Item.Item_Category,"
		"Item.Exclusively_As_Side,"
		"Item.Item_ID,"
		"Item.Print_Chit,"
		"Item.Display_Sizes,"
		"Item.Print_Underlined,"
		"Item.Print_Bold,"
		"Item.Print_Colour,"
		"Item.Print_Font,"
		"Item.Print_Double_Width,"
		"Item.Print_Double_Height,"

		"ItemSize.ItemSize_Key,"
        "ItemSize.Item_Key ItemSize_ItemKey,"
        "ItemSize.Sizes_Key ItemSize_SizeKey,"
		"ItemSize.Size_ID,"
		"ItemSize.Size_Name,"
		"ItemSize.Size_Kitchen_Name,"
		"ItemSize.Price,"
		"ItemSize.MAXRETAILPRICE, "
		"ItemSize.Special_Price,"
		"ItemSize.GST_Percent,"
		"ItemSize.Cost,"
		"ItemSize.Cost_GST_Percent,"
		"ItemSize.Tare_Weight,"
		"ItemSize.Free,"
		"ItemSize.Points_Percent,"
		"ItemSize.Setmenu_Mask,"
		"ItemSize.Available_As_Standard,"
		"ItemSize.No_Recipe,"
		"ItemSize.Barcode,"
		"ItemSize.Mem_Sale_Count,"
		"ItemSize.Mem_Discount_Percent,"
		"ItemSize.Loc_Sale_Count,"
		"ItemSize.Loc_Discount_Percent,"
      "ItemSize.Handheld_Name ItemSize_Handheld_Name, "
      "ItemSize.Receipt_Name ItemSize_Receipt_Name, "

      "ItemSize.can_be_paid_for_using_points, "
/*
      "ItemSize.has_service_charge, "
      "ItemSize.tax_exempt, "
*/
      "ItemSize.available_quantity, "
      "ItemSize.default_quantity, "
      "ItemSize.warning_quantity, "
      "ItemSize.disable_when_count_reaches_zero, "

      "ItemSize.default_patron_count, "

      "ItemSize.PLU,"
      "ItemSize.PLU, "
      "ItemSize.PRICE_FOR_POINTS, " // add line for price for points..
      "ItemSize.REVENUECODE, "
      "ItemSize.ITEMSIZE_IDENTIFIER, "
      "REVENUECODEDETAILS.REVENUECODE_DESCRIPTION,"

		"ArcCategories.Category_Key,"
      "ArcCategories.Category,"
		"CategoryGroups.CategoryGroups_key Category_Group_Key,"
      "CategoryGroups.Name               Category_Group_Name,"
		"ItemSize.ThirdPartyCodes_Key,"
		"ThirdPartyCodes.Code Third_Party_Code,"

		"Recipe.Recipe_Key,"
		"Recipe.Stock_Code,"
		"Recipe.Qty,"
		"StockCosts.Cost_GST_Percent,"
		"StockCosts.Cost Stock_Unit_Cost,"
		"Recipe.Stock_Location,"
		"Recipe.Stock_Location Order_Location, "
		"ServingCourses.ServingCourse_Name, "
        "ServingCourses.ServingCourses_Key, "

        // For Droidware
        "Course.Handheld_Name Course_Handheld_Name, "
        "Course.Receipt_Name Course_Receipt_Name, "
		"Item.Handheld_Name Item_Handheld_Name,"
		"Item.Receipt_Name Item_Receipt_Name "
	"From "
		"Menu Left Join Course on "
			"Menu.Menu_Key =  Course.Menu_Key "
		"Left Join Item on "
			"Course.Course_Key = Item.Course_Key "
		"Left Join ItemSize on "
			"Item.Item_Key = ItemSize.Item_Key "
		"Left Join ArcCategories on "
			"ItemSize.Category_Key = ArcCategories.Category_Key "
		"Left Join CategoryGroups on "
			"CategoryGroups.CategoryGroups_Key = ArcCategories.CategoryGroups_Key "
		"Left Join Recipe On "
			"ItemSize.ItemSize_Key = Recipe.ItemSize_Key "
		"Left Join StockCosts On "
			"StockCosts.Stock_Code = Recipe.Stock_Code and StockCosts.Location = Recipe.Stock_Location "
		"Left Join ThirdPartyCodes On "
			"ItemSize.ThirdPartyCodes_Key = ThirdPartyCodes.ThirdPartyCodes_Key "
		"Left Join ServingCourses On "
			"Course.ServingCourses_Key = ServingCourses.ServingCourses_Key "
		"Left Join REVENUECODEDETAILS On "
			"ITEMSIZE.REVENUECODE = REVENUECODEDETAILS.REVENUECODE "

	"Where "
		"Menu.Menu_Key = :Menu_Key "
//		"And Course.Course_Key <> 0 and Course.Course_Key is not null "
//		"And Item.Item_Key <> 0 and Item.Item_Key is not null "
//		"And ItemSize.ItemSize_Key <> 0 and ItemSize.ItemSize_Key is not null "
	"Order By "
		"Menu.Menu_Name,"
		"Course.CAO,"
		"Item.IAO,"
		"ItemSize.ISAO,"
		"Recipe.Stock_Code,"
		"Recipe.Stock_Location,"
		"Recipe.Stock_Code";

const AnsiString MenuCountSQL =
	"Select "
		"Count(Menu.Menu_Key) "
	"From "
		"Menu Left Join Course on "
			"Menu.Menu_Key =  Course.Menu_Key "
		"Left Join Item on "
			"Course.Course_Key = Item.Course_Key "
		"Left Join ItemSize on "
			"Item.Item_Key = ItemSize.Item_Key "
		"Left Join Recipe On "
			"ItemSize.ItemSize_Key = Recipe.ItemSize_Key "
	"Where "
		"Menu.Menu_Key = :Menu_Key";

const AnsiString AvailableSizesSQL =
	"Select "
		"Sizes_Key,"
		"Size_ID,"
		"Size_Name,"
		"Size_Kitchen_Name,"
        "Handheld_Name, "
        "Receipt_Name, "
		"Weighted_Size "
	"From "
		"Sizes "
	"Where "
		"Menu_Key = :Menu_Key "
	"Order By "
		"Size_Name";

const AnsiString AvailableLocationsSQL =
	"Select "
		"Name "
	"From "
		"Locations "
	"Where "
		"Location_Type <> 0 "
	"Order By "
		"Name";

//---------------------------------------------------------------------------
// this block - all to do with Serving Courses
const AnsiString AvailableServingCoursesSQL =
		"Select "
			"ServingCourses.ServingCourses_Key,"
			"ServingCourses.SERVINGCOURSE_NAME,"
			"ServingCourses.SERVINGCOURSE_KITCHEN_NAME,"
			"ServingCourses.DELETED,"
			"ServingCourses.DISPLAY_ORDER,"
			"ServingCourses.Colour, "
			"ServingCourses.Selectable "
		"From "
			"%s "
		"Where "
			"%s "
		"Order By "
			"ServingCourses.DISPLAY_ORDER";

const AnsiString FirstFrom  = " MenuServingCourses , ServingCourses ";
const AnsiString FirstWhere = " MenuServingCourses.ServingCourses_Key = ServingCourses.ServingCourses_Key and "
									 "MenuServingCourses.Menu_Key = :Menu_Key ";
const AnsiString SecondFrom  = " ServingCourses ";
const AnsiString SecondWhere = " DELETED = 'F' ";
//---------------------------------------------------------------------------
const AnsiString AvailableCategoriesSQL =
		"Select "
			"CategoryGroups.CategoryGroups_Key,"
			"CategoryGroups.Name Category_Group_Name,"
			"ArcCategories.Category_Key,"
			"ArcCategories.Category, "
            "ArcCategories.GL_Code "
		"From "
			"ArcCategories Left Join CategoryGroups On "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
		"Where "
			"ArcCategories.Visible = 'T' "
		"Order By "
			"CategoryGroups.Name,"
			"ArcCategories.Category";

const AnsiString CourseOptionsSQL =
	"Select "
		"Options_Key,"
		"Option_ID,"
		"Course.Course_Key,"
		"Option_Name,"
		"Option_Kitchen_Name,"
        "Course.Handheld_Name, "
        "Course.Receipt_Name, "
		"Flags,"
		"Forced_Mask, "
		"Print_Underlined,"
		"Print_Bold,"
		"Print_Colour,"
		"Print_Font,"
		"Print_Double_Width,"
		"Print_Double_Height, "
        // For Droidware.
        "Options.Handheld_Name Option_Handheld_Name, "
        "Options.Receipt_Name Option_Receipt_Name "
	"From "
		"Options Left Join Course On "
			"Options.Course_Key = Course.Course_Key "
	"Where "
		"Course.Menu_Key = :Menu_Key "
	"Order By "
		"Course.CAO,"
		"Options.Option_Order";

const AnsiString ItemSidesSQL =
		"Select "
            "ItemSides.ItemSides_Key,"
			"C1.Course_Name,"
			"ItemSides.Master_Item_Key,"
			"I1.Item_Name,"
			"I1.Item_Key,"
			"ItemSides.Group_No Group_Number,"
			"ItemSides.Max_Select,"
			"ItemSides.Allow_Skip "
	  "From "
			"ItemSides Left Join Item I1 On "
			"ItemSides.Item_Key = I1.Item_Key "
			"Left Join Course C1 On "
			"I1.Course_Key = C1.Course_Key "
			"Left Join Item MasterItem On "
			"ItemSides.Master_Item_Key = MasterItem.Item_Key "
			"Left Join Course MasterCourse On "
			"MasterItem.Course_Key = MasterCourse.Course_Key "
	  "Where "
			"C1.Menu_Key = :Menu_Key "
	  "Order By "
			"MasterCourse.CAO,"
			"MasterItem.IAO,"
			"ItemSides.IOO";

const AnsiString SizeCategoriesSQL =
	"Select "
		"ItemSizeCategory.ItemSize_Key,"
		"ArcCategories.Category_Key, "
		"ArcCategories.Category "
	"From "
		"Menu Left Join Course on "
			"Menu.Menu_Key =  Course.Menu_Key "
		"Left Join Item on "
			"Course.Course_Key = Item.Course_Key "
		"Left Join ItemSize on "
			"ItemSize.Item_Key = Item.Item_Key "
		"Left Join ItemSizeCategory on "
			"ItemSizeCategory.ItemSize_Key = ItemSize.ItemSize_Key "
		"Left Join ArcCategories On "
			"ArcCategories.Category_Key = ItemSizeCategory.Category_Key "
	"Where "
		"Menu.Menu_Key = :Menu_Key And "
		"ItemSizeCategory.ItemSize_Key Is Not NULL "
	"Order By "
		"Menu.Menu_Name,"
		"Course.CAO,"
		"Item.IAO,"
		"ItemSize.ISAO,"
		"ArcCategories.Category";

const AnsiString ForcedItemOptionsSQL =
		"SELECT "
			"FORCEDOPTIONS.ID as foption_key, OPTIONS.OPTIONS_KEY as option_key, "
            "OPTIONS.OPTION_NAME as option_name, OPTIONS.FLAGS as flags "
		"FROM "
			"ITEM, OPTIONS INNER JOIN FORCEDOPTIONS ON OPTIONS.OPTIONS_KEY = FORCEDOPTIONS.OPTION "
		"WHERE "
			"( FORCEDOPTIONS.ITEM = ITEM.ITEM_KEY ) AND ( ITEM.ITEM_KEY = :item_key )";

const AnsiString AllForcedItemOptionsSQL =
		"SELECT "
			"OPTIONS.OPTIONS_KEY as option_key, OPTIONS.OPTION_NAME as option_name "
		"FROM "
			"ITEM, OPTIONS INNER JOIN FORCEDOPTIONS ON OPTIONS.OPTIONS_KEY = FORCEDOPTIONS.OPTION "
		"WHERE "
			"( FORCEDOPTIONS.ITEM = ITEM.ITEM_KEY ) AND ( ITEM.ITEM_KEY = :item_key )";

const AnsiString ThirdPartyCodesSQL =
		"SELECT "
			"THIRDPARTYCODES_KEY as KEY, CODE, CODETYPE, VISIBLE, DESCRIPTION "
		"FROM "
			"THIRDPARTYCODES";
const AnsiString RevenueCodesSQL =
		"SELECT "
			"REVENUECODE , REVENUECODE_DESCRIPTION "
		"FROM "
			"REVENUECODEDETAILS ORDER BY REVENUECODE";

const AnsiString TaxProfileKeysSQL =
		"SELECT "
			"PROFILE_KEY as KEY "
		"FROM "
			"TAXPROFILES_ITEMSIZE "
      "WHERE "
         "ITEMSIZE_KEY = :itemsize_key";

 const AnsiString ItemSizePriceLevel =
         "SELECT "
                "a.PRICELEVELITEMSIZE_KEY, "
                "a.ITEMSIZE_KEY, "
                "a.PRICELEVEL_KEY, "
                "a.PRICE, "
                "PRICELEVELS.PRICELEVEL_NAME "
         "FROM "
                 "PRICELEVELITEMSIZE a inner join PRICELEVELS on PRICELEVELS.PRICELEVEL_KEY =a.PRICELEVEL_KEY "
         "WHERE  "
               "a.ITEMSIZE_KEY = :ITEMSIZE_KEY and a.ISENABLED = :isEnabled ";

const AnsiString ItemSizeTaxPercent =
        "SELECT a.ITEMSIZE_KEY, "
                 "Sum(TAXPROFILES.RATE) RATE, "
                 "cast(sum(CASE WHEN TAXPROFILES.TYPE = 2 THEN TAXPROFILES.RATE END) as numeric(17, 4)) AS ServiceCharge, "
                 "cast(sum(CASE WHEN TAXPROFILES.TYPE = 0 THEN  TAXPROFILES.RATE END) as numeric(17, 4)) AS Tax  "
        "FROM ITEMSIZE a  "
         "left join  TAXPROFILES_ITEMSIZE on TAXPROFILES_ITEMSIZE.ITEMSIZE_KEY=a.ITEMSIZE_KEY "
         "left join TAXPROFILES on TAXPROFILES.PROFILE_KEY=TAXPROFILES_ITEMSIZE.PROFILE_KEY "
         "WHERE  "
               "a.ITEMSIZE_KEY = :ITEMSIZE_KEY "
         "GROUP BY "
         "a.ITEMSIZE_KEY ";

//---------------------------------------------------------------------------
class TDBKey
{
public:
    __int32 Key;
};
//---------------------------------------------------------------------------
class TNameAndKey: public TDBKey
{
public:
   TNameAndKey( __int32 inKey, WideString inName );

   WideString Name;
};
//---------------------------------------------------------------------------
class TSizeRecipeInfo : public TDBKey
{
public:
	WideString	Stock_Code;
	double		Qty;
	Currency	Stock_GST_Percent;
	Currency	Stock_Unit_Cost;
	WideString	Stock_Location;
};
//---------------------------------------------------------------------------
class TSizeCategoryInfo : public TDBKey
{
public:
	WideString	Category;
};
//---------------------------------------------------------------------------
class TItemSizePriceLevel
{
    public:
        int PriceLevelKey;
        AnsiString PriceLevelName;
        Currency Price;
};
//-----------------------------------------------------------------------------
class TPriceLevelsName
{
    public:
        int PriceLevelKey;
        AnsiString PriceLevelName;
};
//---------------------------------------------------------------------------
class TItemSizeTaxesPercentage
{
    public:
        Currency SalesTaxPercent;
        Currency ServiceChargePercent;
};
//---------------------------------------------------------------------------
class TItemSizeInfo : public TDBKey
{
public:
    __int32     Item_Key;
    __int32     Size_Key;
	int			Size_ID;
	WideString	Size_Name;
	WideString	Size_Kitchen_Name;
    WideString  Size_Handheld_Name;
    WideString  Size_Receipt_Name;
	Currency	Price;
	Currency MaxRetailPrice;
	Currency	Special_Price;
	double		GST_Percent;
	Currency	Cost;
	double		Cost_GST_Percent;
	bool		Free;
	Currency	Tare_Weight;
	double		Points_Percent;
	int			Setmenu_Mask;
	bool		Available_As_Standard;
	bool		No_Recipe;
	WideString	Barcode;
	int			Mem_Sale_Count;
	double		Mem_Discount_Percent;
	int			Loc_Sale_Count;
	double		Loc_Discount_Percent;
    __int32     CategoryKey;
	WideString	Category;
	WideString	Category_Group_Name;
	int			ThirdPartyCodes_Key;
	WideString	Third_Party_Code;
    int         RevenueCode;
    AnsiString  RevenueCodeDescription;
    __int32     PLU;
    bool HasServiceCharge;
    bool IsTaxExempt;
    double AvailableQuantity;
    double DefaultQuantity;
    double WarningQuantity;
    bool DisableWhenCountReachesZero;
    bool CanBePaidForUsingPoints;
    int DefaultPatronCount;
    int ItemSizeIdentifier;
    std::vector<TSizeCategoryInfo> Categories;
    std::vector<TSizeRecipeInfo>   Recipes;
    std::set<__int32> TaxProfileKeys;
    std::map<int,TItemSizePriceLevel> ItemSizePriceLevels;
    Currency	PriceForPoints;
    std::vector<TItemSizeTaxesPercentage> ItemSizeTaxPercent;
};
//---------------------------------------------------------------------------
class TItemSideInfo : public TDBKey
{
public:

	int			Master_Item_Key;
	WideString	Course_Name;
	int			Item_Key;
	//int			IOO;
	WideString	Item_Name;
	int			Group_Number;
	int			Max_Select;
	bool		Allow_Skip;
};
//--------------------------------------------------------------------------

class TItemOptionInfo {
public:
   TItemOptionInfo() { };

   TItemOptionInfo(const WideString &inName,
                   const __int32 inForcedOptionPK,
                   const __int32 inItemFK,
                   const __int32 inOptionFK)
     : Name_(inName),
       ForcedOptionPK_(inForcedOptionPK),
       ItemFK_(inItemFK),
       OptionFK_(inOptionFK)
   {
   };

   const WideString &Name() const { return Name_; };
   __int32 ForcedOptionPK() const { return ForcedOptionPK_; };
   __int32 ItemFK() const { return ItemFK_; };
   __int32 OptionFK() const { return OptionFK_; };

private:
   WideString Name_;
   __int32 ForcedOptionPK_;
   __int32 ItemFK_;
   __int32 OptionFK_;
};

class TItemInfo : public TDBKey
{
public:
	WideString	Item_Name;
	WideString	Item_Kitchen_Name;
	WideString	Item_Handheld_Name;
	WideString	Item_Receipt_Name;
	bool		Enabled;
	TColor	Button_Colour;
	int		Item_ID;
	int		PalmID;
	bool		Exclusively_As_Side;
	bool		Print_Chit;
	bool		Display_Sizes;
	bool 		Print_Underlined;
	bool 		Print_Bold;
	TColor 	Print_Colour;
	int 		Print_Font;
	bool 		Print_Double_Width;
	bool 		Print_Double_Height;
    int Itemidentifier;
   std::vector<TItemSideInfo>   Sides;
   std::vector<TItemOptionInfo> ForcedOptions;
};
//---------------------------------------------------------------------------
class TCourseOptionInfo : public TDBKey
{
public:
	WideString		Option_Name;
	WideString		Option_Kitchen_Name;
   WideString      Option_Handheld_Name;
   WideString      Option_Receipt_Name;


	unsigned short	Option_ID;
	int				Forced_Mask;
	int				Flags;
	int				GroupNumber;
	int				PlusOption;
	int				MinusOption;
	bool 			Print_Underlined;
	bool 			Print_Bold;
	TColor 			Print_Colour;
	int 			Print_Font;
	bool 			Print_Double_Width;
	bool 			Print_Double_Height;
	int		    	Max_Select;
	bool		   	Allow_Skip;
};
//---------------------------------------------------------------------------
enum TLocation { lcNone=0, lcBar=1, lcRestaurant=2, lcBottleStore=4, lcPalm=8};
//---------------------------------------------------------------------------
class TCourseInfo : public TDBKey
{
public:
	WideString	Course_Name;
	WideString	Course_Kitchen_Name;
   WideString  Course_Handheld_Name;
   WideString  Course_Receipt_Name;

	TLocation	View_Location;
	int			Course_ID;
	int			PalmID;
    __int32     ServingCourse_Key;
	WideString  ServingCourse_Name;
	bool        No_Default_Serving_Course;

	std::vector<TCourseOptionInfo> Options;
};
//---------------------------------------------------------------------------
class TSizeInfo : public TDBKey
{
public:
	WideString	Size_Name;
	WideString	Size_Kitchen_Name;

    WideString  Size_Handheld_Name;
    WideString  Size_Receipt_Name;

	bool		Weighed;
	int			Size_ID;
	int			PalmID;
};
//---------------------------------------------------------------------------
class TSizesInfo
{
public:
	std::vector<TSizeInfo> Sizes;
};
//---------------------------------------------------------------------------
class TServingCourseInfo : public TDBKey
{
public:
	WideString	ServingCourse_Name;
	WideString	ServingCourse_Kitchen_Name;
	bool		Enabled;
	bool		Deleted;
	TColor		Colour;
	bool		Selectable;
   int   DisplayOrder;
};
//---------------------------------------------------------------------------
class TServingCoursesInfo
{
public:
	std::vector<TServingCourseInfo> ServingCourses;
};
//---------------------------------------------------------------------------
class TDeletedServingCoursesInfo
{
public:
	// used for capturing the deleted Serving Courses for the POS
	std::vector<TServingCourseInfo> DeletedServingCourseVector;
};
//---------------------------------------------------------------------------
class TCategoryInfo : public TDBKey
{
public:
	WideString Category_Name;
    WideString Category_GL_Code;      
};

//---------------------------------------------------------------------------
class TCategoryGroupInfo : public TDBKey
{
public:
	WideString Category_Group_Name;
	std::vector<TCategoryInfo> Categories;
};
//---------------------------------------------------------------------------
class TCategoriesInfo
{
public:
	std::vector<TCategoryGroupInfo> CategoryGroups;
};
//---------------------------------------------------------------------------
class TLocationsInfo
{
public:
	std::vector<WideString> Locations;
};
//---------------------------------------------------------------------------
enum TMenuType { mtFoodMenu = 0, mtBeverageMenu = 1};
//---------------------------------------------------------------------------
class TMenuInfo : public TDBKey
{
public:
	AnsiString	Menu_Name;
	TMenuType	Menu_Type;
	bool		Palmable;
};
//---------------------------------------------------------------------------
class TThirdPartyCodeInfo : public TDBKey
{
public:
	WideString Code;
	__int32    Code_Type;
	bool	   Visible;
    WideString Description;
};
//---------------------------------------------------------------------------
class TRevenueCodesInfo : public TDBKey
{
public:
    int code;
    WideString codeDescription;
};
//---------------------------------------------------------------------------
class TMenuLoad
{
protected:
	int				fCount;
	int				fPosition;
public:
	virtual bool	GetMenu(
								TMenuInfo				*MenuInfo,
								TSizesInfo				*SizesInfo,
								TCategoriesInfo		*CategoriesInfo,
								TLocationsInfo			*LocationsInfo,
								TServingCoursesInfo	*ServingCoursesInfo)				= 0;

	virtual bool   GetAvailableSizes(TSizesInfo *SizesInfo)						= 0;
	virtual bool   GetAvailableCategories(TCategoriesInfo *CategoriesInfo)	= 0;
	virtual bool   GetAvailableLocations(TLocationsInfo *LocationsInfo)   = 0;
	virtual bool   GetAvailableServingCourses(TServingCoursesInfo *ServingCoursesInfo) = 0;
	virtual void   LoadServingCourses(TServingCoursesInfo *ServingCoursesInfo,bool Enabled ) = 0;


	virtual int		Count()																	= 0;
	virtual int		Position()																= 0;

	virtual bool	GetNextCourse(TCourseInfo *CourseInfo)							= 0;
	virtual bool	GetNextItem(TItemInfo *ItemInfo)									= 0;
	virtual bool	GetNextItemSize(TItemSizeInfo *ItemSizeInfo)					= 0;
};
//---------------------------------------------------------------------------
typedef std::vector<TMenuInfo> TMenusInfo;
//---------------------------------------------------------------------------
class TMenuLoadDB : public TMenuLoad
{
private:
	Database::TDBTransaction  dbTransaction;

	Database::TcpIBSQL sqlMenu;
	Database::TcpIBSQL sqlMenuCount;
	Database::TcpIBSQL sqlAvailableSizes;
	Database::TcpIBSQL sqlAvailableCategories;
	Database::TcpIBSQL sqlAvailableLocations;
	Database::TcpIBSQL sqlItemSides;
	Database::TcpIBSQL sqlCourseOptions;
	Database::TcpIBSQL sqlSizeCategories;
	Database::TcpIBSQL sqlAvailableServingCourses;
	Database::TcpIBSQL sqlOptions;

	int	fMenuKey;

	int	fLastCourse;
	int	fLastItem;
	int	fLastItemSize;

	int	fPalmID;

	void Next();

public:
	TMenuLoadDB(TIBDatabase *MMDatabase, int MenuKey=0);
	~TMenuLoadDB();

	void EnumMenus(TMenusInfo &MenuInfo);

	bool GetMenu(
                  TMenuInfo			  *MenuInfo,
                  TSizesInfo		  *SizesInfo,
                  TCategoriesInfo	  *CategoriesInfo,
                  TLocationsInfo	  *LocationsInfo,
                  TServingCoursesInfo *ServingCoursesInfo);

	bool GetAvailableSizes(TSizesInfo *SizesInfo);
	bool GetAvailableCategories(TCategoriesInfo *CategoriesInfo);
	bool GetAvailableLocations(TLocationsInfo *LocationsInfo);
	bool GetAvailableServingCourses(TServingCoursesInfo *ServingCoursesInfo);
	void LoadServingCourses(TServingCoursesInfo *ServingCoursesInfo,bool Enabled );


	int	Count();
	int	Position();

	bool GetNextCourse(TCourseInfo *CourseInfo);
	bool GetNextItem(TItemInfo *ItemInfo);
	bool GetNextItemSize(TItemSizeInfo *ItemSizeInfo);

   void getTaxProfileKeysForItemSize(
           __int32 inItemSizeKey,
           std::set<__int32>* const outKeys );

	int	GetMaxPalmID();

    bool GetForcedItemOptionKeysWithGroupNumber( unsigned int inGroupNumber, unsigned int inItemKey,
                                                 std::vector<TItemOptionInfo> &inForcedOptions );
    bool GetForcedItemOptionKeysWithGroupNumber( unsigned int inGroupNumber, unsigned int inItemKey,
                                                 std::vector<TNameAndKey> &inForcedItemOptions );
    bool GetForcedItemOptionKeys( unsigned int inItemKey,
                                  std::vector<TNameAndKey> &inForcedItemOptions );

    bool GetThirdPartyCodes( std::vector<TThirdPartyCodeInfo>& outCodes );
    void GetAllRevenueCodesFromDB(std::map<int,AnsiString> &revenueCodesMap);
    void GeTItemSizePriceLevels(int inItemSizeKey, std::map<int,TItemSizePriceLevel>* ItemSizePriceLevels );
    void GeTItemSizeTaxPercentage(int inItemSizeKey, std::vector<TItemSizeTaxesPercentage> &itemSizeTaxPercentage);
};
//---------------------------------------------------------------------------
/*class TMenuLoadFile : public TMenuLoad
{
private:
	std::auto_ptr<TStringList>		const fMenuCSV;

public:
	TMenuLoadFile(AnsiString FileName);
	~TMenuLoadFile();

	bool				GetMenu(
								TMenuInfo			*MenuInfo,
								TSizesInfo			*SizesInfo,
								TCategoriesInfo	*CategoriesInfo,
								TLocationsInfo		*LocationsInfo);

	bool				GetAvailableSizes(TSizesInfo *SizesInfo);
	bool 				GetAvailableCategories(TCategoriesInfo *CategoriesInfo);
	bool				GetAvailableLocations(TLocationsInfo *LocationsInfo);

	int				Count();
	int				Position();

	bool				GetNextCourse(TCourseInfo *CourseInfo);
	bool				GetNextItem(TItemInfo *ItemInfo);
	bool				GetNextItemSize(TItemSizeInfo *ItemSizeInfo);
};*/
//---------------------------------------------------------------------------
} // namespace Menu
//---------------------------------------------------------------------------
#endif

