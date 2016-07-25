//---------------------------------------------------------------------------

#ifndef MM_MenuH
#define MM_MenuH
#include <Classes.hpp>
#include <Graphics.hpp>
#include <IBDatabase.hpp>
#include <IBSQL.hpp>
//#include <memory>
#include "MM_DBCore.h"
#include <vector>
#include "Weight.h"
#include "TaxProfile.h"

//---------------------------------------------------------------------------
namespace Menu
{
//---------------------------------------------------------------------------
const UnicodeString MenuSQL =
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
		"Course.ServingCourses_Key,"
		"ServingCourses.ServingCourse_Name,"

		"Item.Item_Key,"
		"Item.Item_Name,"
		"Item.IAO,"
		"Item.Item_Kitchen_Name,"
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
		"ItemSize.Size_ID,"
		"ItemSize.Size_Name,"
		"ItemSize.Size_Kitchen_Name,"
		"ItemSize.Price,"
        "ItemSize.MaxRetailPrice, "
		"ItemSize.Special_Price,"
		"ItemSize.GST_Percent,"
		"ItemSize.Cost,"
		"ItemSize.Cost_GST_Percent,"
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
		"ItemSize.Tare_Weight,"
		"ItemSize.PLU,"
        "ItemSize.IsInvisible,"
                "ItemSize.can_be_paid_for_using_points, "

                "ItemSize.enabled, "
                "available_quantity, "
                "default_quantity, "
                "warning_quantity, "
                "disable_when_count_reaches_zero, "

		"ItemSize.default_patron_count, "
        "ItemSize.PRICE_FOR_POINTS, "

		"ArcCategories.Category, "
        "ArcCategories.GL_CODE, "
		"ArcCategories.Category_Key,"
		"CategoryGroups.Name Category_Group_Name,"
		"CategoryGroups.CategoryGroups_Key,"
		"ItemSize.ThirdPartyCodes_Key,"
		"ThirdPartyCodes.Code ThirdPartyCode,"

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
		"Item.Receipt_Name Item_Receipt_Name,"
        "ItemSize.Handheld_Name ItemSize_Handheld_Name, "
        "ItemSize.Receipt_Name ItemSize_Receipt_Name "
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

	"Where "
		"Menu.Menu_Key = :Menu_Key "
	"Order By "
		"Menu.Menu_Name,"
		"Course.CAO,"
		"Item.IAO,"
		"ItemSize.ISAO,"
		"Recipe.Stock_Code,"
		"Recipe.Stock_Location,"
		"Recipe.Stock_Code";

/*
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
		"Menu.Menu_Key = :Menu_Key "
	"Order By "
		"Menu.Menu_Name,"
		"Course.CAO,"
		"Item.IAO,"
		"ItemSize.ISAO,"
		"Recipe.Stock_Code,"
		"Recipe.Stock_Location,"
		"Recipe.Stock_Code";
*/

const AnsiString MenuCountSQL =
	"Select "
		"Count(Menu.Menu_Key) "
	"From "
		"Menu "
	"Where "
		"Menu.Menu_Key = :Menu_Key ";


const UnicodeString AvailableSizesSQL =
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
		"Menu_Key = :Menu_Key and Sizes.ISINVISIBLE ='F' "
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

const UnicodeString ItemSizeTaxProfilesSQL =
	"Select "
        "TaxProfiles.Profile_Key, "
		"Name, "
		"Rate, "
		"Type, "
        "Priority "
	"From "
		"TaxProfiles Inner Join TaxProfiles_ItemSize "
        "On TaxProfiles.Profile_Key = TaxProfiles_ItemSize.Profile_Key "
	"Where "
		"ItemSize_Key = :ItemSize_Key "
	"Order By "
		"Type, Priority";
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

const UnicodeString FirstFrom  = " MenuServingCourses , ServingCourses ";
const UnicodeString FirstWhere = " MenuServingCourses.ServingCourses_Key = ServingCourses.ServingCourses_Key and "
											" MenuServingCourses.Menu_Key = :Menu_Key ";
const UnicodeString SecondFrom  = " ServingCourses ";
const UnicodeString SecondWhere = " DELETED = 'F' ";
//---------------------------------------------------------------------------
const UnicodeString AvailableCategoriesSQL =
		"Select "
			"CategoryGroups.CategoryGroups_Key,"
			"CategoryGroups.Name Category_Group_Name,"
			"ArcCategories.Category_Key,"
			"ArcCategories.Category , "
            "ArcCategories.GL_Code "
		"From "
			"ArcCategories Left Join CategoryGroups On "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
		"Where "
			"ArcCategories.Visible = 'T' "
		"Order By "
			"CategoryGroups.Name,"
			"ArcCategories.Category";

const UnicodeString CourseOptionsSQL =
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
        "Options.Receipt_Name Option_Receipt_Name, "
        "OPTION_ORDER "
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
		"ArcCategories.Category, "
        "ArcCategories.GL_CODE "
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

const UnicodeString ForcedItemOptionsSQL =
		"SELECT "
			"OPTIONS.OPTIONS_KEY as option_key, OPTIONS.OPTION_NAME as option_name, OPTIONS.FLAGS as flags "
		"FROM "
			"ITEM, OPTIONS INNER JOIN FORCEDOPTIONS ON OPTIONS.OPTIONS_KEY = FORCEDOPTIONS.OPTION "
		"WHERE "
			"( FORCEDOPTIONS.ITEM = ITEM.ITEM_KEY ) AND ( ITEM.ITEM_KEY = :item_key )";

const UnicodeString AllForcedItemOptionsSQL =
		"SELECT "
			"OPTIONS.OPTIONS_KEY as option_key, OPTIONS.OPTION_NAME as option_name "
		"FROM "
			"ITEM, OPTIONS INNER JOIN FORCEDOPTIONS ON OPTIONS.OPTIONS_KEY = FORCEDOPTIONS.OPTION "
		"WHERE "
			"( FORCEDOPTIONS.ITEM = ITEM.ITEM_KEY ) AND ( ITEM.ITEM_KEY = :item_key )";

const UnicodeString ThirdPartyCodesSQL =
		"SELECT "
			"THIRDPARTYCODES_KEY as KEY, CODE, CODETYPE, VISIBLE, DESCRIPTION "
		"FROM "
			"THIRDPARTYCODES";
//---------------------------------------------------------------------------

const UnicodeString PriceLevelSQL =
            "SELECT "
            "a.PRICELEVELITEMSIZE_KEY, a.ITEMSIZE_KEY, a.PRICELEVEL_KEY, a.PRICE, a.ISENABLED ,pl.PRICELEVEL_NAME "
            "FROM  "
            "PRICELEVELITEMSIZE a inner join  PRICELEVELS PL on pl.PRICELEVEL_KEY =a.PRICELEVEL_KEY "
            "where a.ITEMSIZE_KEY = :ITEMSIZE_KEY ";
///----------------------------------------------------
class TDBKey
{
public:
    __int32 Key;
};
//---------------------------------------------------------------------------
class TNameAndKey: public TDBKey
{
public:
	TNameAndKey( __int32 inKey, UnicodeString inName );

    UnicodeString Name;
};
//---------------------------------------------------------------------------
class TSizeRecipeInfo : public TDBKey
{
public:
	//int			  Recipe_Key;
	UnicodeString Stock_Code;
	double		  Qty;
	Currency	  Stock_GST_Percent;
	Currency	  Stock_Unit_Cost;
	UnicodeString Stock_Location;
};
//---------------------------------------------------------------------------
class TSizeCategoryInfo : public TDBKey
{
public:
	//int			  Category_Key;
	UnicodeString Category;
    UnicodeString GLCode;
};
//---------------------------------------------------------------------------
class TItemSizePriceLevel : public TDBKey
{
   public:
    int PriceLevelKey;
    Currency Price;
    AnsiString Name;

};

////----------------------------------------------
class TItemSizeInfo : public TDBKey
{
public:
	int			  Size_ID;
	UnicodeString Size_Name;
	WideString	  Size_Kitchen_Name;

	UnicodeString Size_Handheld_Name;
	UnicodeString Size_Receipt_Name;

	Currency	  Price;
    Currency MaxRetailPrice;
	Currency	  Special_Price;
	TWeight       Tare_Weight;
    double		  GST_Percent;
	Currency	  Cost;
	double		  Cost_GST_Percent;
	bool		  Free;
	double		  Points_Percent;
	int			  Setmenu_Mask;
	bool		  Available_As_Standard;
	bool		  No_Recipe;
	UnicodeString Barcode;
	int			  Mem_Sale_Count;
	double		  Mem_Discount_Percent;
	int			  Loc_Sale_Count;
	double	      Loc_Discount_Percent;

	UnicodeString Category;
	UnicodeString Category_Group_Name;
    UnicodeString GLCode;
	int 		  CategoryKey;
	int	      	  Category_Group_Key;
	int			  ThirdPartyCodes_Key;
	UnicodeString ThirdPartyCode;
    bool IsInvisible;
	std::vector<TSizeCategoryInfo> Categories;
	std::vector<TSizeRecipeInfo>   Recipes;
    std::vector<TaxProfile>        TaxProfiles;
    std::map<int,TItemSizePriceLevel> PriceLevels;

	int			  PLU;

	bool enabled;
        Currency available_quantity;
        Currency default_quantity;
        Currency warning_quantity;
        bool disable_when_count_reaches_zero;
        bool CanBePaidForUsingPoints;

	int DefaultPatronCount;
    Currency	  PriceForPoints;
};
//---------------------------------------------------------------------------
class TItemSideInfo : public TDBKey
{
public:

	int			  Master_Item_Key;
	UnicodeString Course_Name;
	int			  Item_Key;
//	int			IOO;
	UnicodeString Item_Name;
	int			  Group_Number;
	int			  Max_Select;
	bool		  Allow_Skip;
};
//---------------------------------------------------------------------------
class TItemInfo : public TDBKey
{
public:
	//int			  Item_Key;
	UnicodeString Item_Name;
	UnicodeString Item_Kitchen_Name;
	UnicodeString Item_Handheld_Name;
	UnicodeString Item_Receipt_Name;

	bool		  Enabled;
	TColor		  Button_Colour;
	int			  Item_ID;
	int			  PalmID;
	bool		  Exclusively_As_Side;
	bool		  Print_Chit;
	bool		  Display_Sizes;
	bool 		  Print_Underlined;
	bool 		  Print_Bold;
	TColor 		  Print_Colour;
	int 		  Print_Font;
	bool 		  Print_Double_Width;
	bool 		  Print_Double_Height;

	__int32       IAO;

	std::vector<TItemSideInfo> Sides;
	std::vector<TNameAndKey>   ForcedOptions;
};
//---------------------------------------------------------------------------
class TCourseOptionInfo : public TDBKey
{
public:
	//int			   Option_Key;
	UnicodeString  Option_Name;
	UnicodeString  Option_Kitchen_Name;
	UnicodeString  Option_Handheld_Name;
	UnicodeString  Option_Receipt_Name;

	unsigned short Option_ID;
	int			   Forced_Mask;
	int			   Flags;
	int			   GroupNumber;
	int			   PlusOption;
	int			   MinusOption;
	bool 		   Print_Underlined;
	bool 		   Print_Bold;
	TColor 		   Print_Colour;
	int 		   Print_Font;
	bool 		   Print_Double_Width;
	bool 		   Print_Double_Height;
	int		       Max_Select;
	bool		   Allow_Skip;
    int            Option_order;
};
//---------------------------------------------------------------------------
enum TLocation { lcNone=0, lcBar=1, lcRestaurant=2, lcBottleStore=4, lcPalm=8};
//---------------------------------------------------------------------------
class TCourseInfo : public TDBKey
{
public:
	//int			  Course_Key;
	UnicodeString  Course_Name;
	UnicodeString  Course_Kitchen_Name;
	UnicodeString  Course_Handheld_Name;
	UnicodeString  Course_Receipt_Name;

	TLocation	  View_Location;
	int			  Course_ID;
	int			  PalmID;
	int			  ServingCourses_Key;
	UnicodeString ServingCourse_Name;
	bool          No_Default_Serving_Course;

	std::vector<TCourseOptionInfo> Options;
};
//---------------------------------------------------------------------------
class TSizeInfo : public TDBKey
{
public:
	UnicodeString Size_Name;
	UnicodeString Size_Kitchen_Name;

	UnicodeString Size_Handheld_Name;
	UnicodeString Size_Receipt_Name;

	bool          Weighted_Size;
	int		      Size_ID;
	int		      PalmID;
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
	//int			  ServingCourses_Key;
	UnicodeString ServingCourse_Name;
	UnicodeString ServingCourse_Kitchen_Name;

	bool		  Enabled;
	bool		  Deleted;                    // was Serving_Course_Deleted
	TColor		  Colour;
	bool		  Selectable;
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
	//int Category_Group_Key;
	UnicodeString Category_Group_Name;
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
	std::vector<UnicodeString> Locations;
};
//---------------------------------------------------------------------------
enum TMenuType { mtFoodMenu = 0, mtBeverageMenu = 1};
//---------------------------------------------------------------------------
class TMenuInfo : public TDBKey
{
public:
	//int			  Menu_Key;
	UnicodeString Menu_Name;
	TMenuType	  Menu_Type;
	bool		  Palmable;
};
//---------------------------------------------------------------------------
class TThirdPartyCodeInfo : public TDBKey
{
public:
	AnsiString Code;
	__int32    Code_Type;
	bool	   Visible;
    AnsiString Description;
};
//---------------------------------------------------------------------------
class TMenuLoad
{
public:
	virtual bool	GetMenu(
								TMenuInfo				*MenuInfo,
								TSizesInfo				*SizesInfo,
								TCategoriesInfo		*CategoriesInfo,
								TLocationsInfo			*LocationsInfo,
								TServingCoursesInfo	*ServingCoursesInfo)				= 0;

	virtual bool	GetAvailableSizes(TSizesInfo *SizesInfo)						= 0;
	virtual bool	GetAvailableCategories(TCategoriesInfo *CategoriesInfo)	= 0;
	virtual bool	GetAvailableLocations(TLocationsInfo *LocationsInfo)		= 0;
	virtual bool	GetAvailableServingCourses(TServingCoursesInfo *ServingCoursesInfo) = 0;

	virtual int		Count()																	= 0;
	virtual int		Position()																= 0;

	virtual bool	GetNextCourse(TCourseInfo *CourseInfo)							= 0;
	virtual bool	GetNextItem(TItemInfo *ItemInfo)									= 0;
	virtual bool	GetNextItemSize(TItemSizeInfo *ItemSizeInfo)					= 0;

private:
	virtual void   LoadServingCourses(TServingCoursesInfo *ServingCoursesInfo,bool Enabled ) = 0;

protected:
	int				fPosition;
	int				fCount;

};
//---------------------------------------------------------------------------
typedef std::vector<TMenuInfo> TMenusInfo;
//---------------------------------------------------------------------------
class TMenuLoadDB : public TMenuLoad
{
private:
	Database::TDBTransaction	DBControl;

	Database::TcpIBSQL			sqlMenu;
	Database::TcpIBSQL			sqlMenuCount;
	Database::TcpIBSQL			sqlAvailableSizes;
	Database::TcpIBSQL			sqlAvailableCategories;
	Database::TcpIBSQL			sqlAvailableLocations;
    Database::TcpIBSQL			sqlItemSizeTaxProfiles;
	Database::TcpIBSQL			sqlItemSides;
	Database::TcpIBSQL			sqlCourseOptions;
	Database::TcpIBSQL			sqlSizeCategories;
	Database::TcpIBSQL			sqlAvailableServingCourses;
    Database::TcpIBSQL			sqlItemSizePriceLevels;


	int	fMenuKey;

	int	fLastCourse;
	int	fLastItem;
	int	fLastItemSize;

	int	fPalmID;

	void	Next();
public:
	TMenuLoadDB(TIBDatabase *MMDatabase, int MenuKey=0);
	~TMenuLoadDB();

	void	EnumMenus(TMenusInfo &MenuInfo);

	bool	GetMenu(
					TMenuInfo			*MenuInfo,
					TSizesInfo			*SizesInfo,
					TCategoriesInfo		*CategoriesInfo,
					TLocationsInfo		*LocationsInfo,
					TServingCoursesInfo	*ServingCoursesInfo);

	bool	GetAvailableSizes(TSizesInfo *SizesInfo);
	bool	GetAvailableCategories(TCategoriesInfo *CategoriesInfo);
	bool	GetAvailableLocations(TLocationsInfo *LocationsInfo);
	bool	GetAvailableServingCourses(TServingCoursesInfo *ServingCoursesInfo);
	void    LoadServingCourses(TServingCoursesInfo *ServingCoursesInfo,bool Enabled );
    void    LoadItemSizeTaxProfiles(TItemSizeInfo *ItemSizeInfo);
    void    LoadItemSizePriceLevels(TItemSizeInfo *ItemSizeInfo);
	TServingCourseInfo FindOrCreateNoServingCourse();

	int	Count();
	int	Position();

	bool	GetNextCourse(TCourseInfo *CourseInfo);
	bool	GetNextItem(TItemInfo *ItemInfo);
	bool	GetNextItemSize(TItemSizeInfo *ItemSizeInfo);

	int	GetMaxPalmID();

	//:::::::::::::::::::::::::::::::::::::::::::

	bool GetForcedItemOptionKeysWithGroupNumber(
							 unsigned int inGroupNumber,
							 unsigned int inItemKey,
				std::vector<TNameAndKey>& inForcedItemOptions );

	bool GetForcedItemOptionKeys(
							 unsigned int inItemKey,
				std::vector<TNameAndKey>& inForcedItemOptions );

	bool GetThirdPartyCodes( std::vector<TThirdPartyCodeInfo>& outCodes );
};
//---------------------------------------------------------------------------
/*class TMenuLoadFile : public TMenuLoad
{
private:
	std::auto_ptr<TStringList>		const fMenuCSV;

public:
	TMenuLoadFile(UnicodeString FileName);
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


