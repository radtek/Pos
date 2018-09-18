//---------------------------------------------------------------------------
#ifndef MenuEditH
#define MenuEditH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
#include <IBDatabase.hpp>
#include <IBSQL.hpp>
#include <CheckLst.hpp>
#include "TouchButton.h"
#include "NumericEdit.h"
#include "TntStdCtrls.hpp"
#include "Preview.h"
#include "Barcodereader.h"
#include "UpdateCategories.h"
#include "MM_Menu.h"
#include "TaxProfileAssigner.h"
#include "TaxProfileController.h"
#include <OleServer.hpp>
#include <DB.hpp>
#include <IBCustomDataSet.hpp>
#include <IBQuery.hpp>
#include <ActnList.hpp>
#include "touchbutton.h"
#include "TouchBtn.h"
#include "TouchControls.h"
#include <vector>
#include <set>
#include <map>

#include "Dialog.h"
#include <memory>
#include "menu_key_generator.h"
#include "CSV.h"

#include "MenuTaxProfileProvider.h"
#include "Administration.h"
#include <IBDatabase.hpp>
#include <IBSQL.hpp>


//---------------------------------------------------------------------------
const WM_LOADMENU								= WM_USER + 4;
//const WM_LOADMENU_DB						= WM_USER + 4;
//const WM_LOADMENU_FILE					= WM_USER + 5;
//---------------------------------------------------------------------------
const MAX_COURSE_SIZES						= 25;

const MAX_DESCRIPTION_LENGTH				= 25;
const MAX_3RD_PARTY_CODE_LENGTH			= 50;

const enum NodeTypes 	{	NO_NODE, MENU_NODE, COURSE_NODE, ITEM_NODE,
	/*SIZES_NODE, SIZE_NODE,*/ ITEM_SIZE_NODE,
	OPTIONS_NODE, OPTION_NODE, SIZES_NODE, SIZE_NODE, CATEGORIES_NODE,
	CATEGORY_GROUP_NODE, CATEGORY_NODE,SERVING_COURSES_NODE,SERVING_COURSE_NODE };

enum eLocation {LocNone=0,LocBar=1,LocRestaurant=2,LocBottleStore=4,LocPalm=8};
//---------------------------------------------------------------------------
class TEditorNode;
class TItemSizeNode;
class TItemNode;
class TCourseNode;
class TMenuNode;
class TOptionNode;
class TOptionsNode;
class TSizeNode;
class TCategoryGroupNode;
class TCategoryGroupsNode;
class TCategoryNode;
class TServingCoursesNode;
class TServingCourseNode;
class TPriceLevelsName;

class i_generator;
class TAvailableMenuSize;

//class TSizesNode;
//---------------------------------------------------------------------------
// Used to find a union of categories in the csv and in the database.
// Yuckie!
//---------------------------------------------------------------------------
class  TCategoryBrief
{
public:
	TCategoryBrief(AnsiString name, bool deleted) : Name(name), Deleted(deleted) {}
	TCategoryBrief(AnsiString name) : Name(name), Deleted(false) {}
	
	AnsiString Name;
	bool Deleted;

	bool operator < (const TCategoryBrief &cat) const { return Name < cat.Name; }
	bool operator < (TCategoryBrief &cat) const { return Name < cat.Name; }

	bool operator == (const TCategoryBrief &cat) const { return Name == cat.Name; }
	bool operator == (TCategoryBrief &cat) const { return Name == cat.Name; }
};
// Map the group to the categories
typedef std::map<TCategoryBrief, std::set<TCategoryBrief> > TCategoryList;
//---------------------------------------------------------------------------
class TIterationAction;
class TSaveMenu;
class TLoadMenu;
struct TForcedOption;

class TItemSizeNode;

class TfrmMenuEdit : public TForm
{
	std::auto_ptr<i_generator<__int32> > new_menu_element_key_generator_;

	__published:	// IDE-managed Components
	TColorDialog *ColorDialog1;
	TSaveDialog *SaveDialog1;
	TPopupMenu *pmDrinksBevs;
	TMenuItem *Expand1;
	TMenuItem *Collapse1;
	TMenuItem *N10;
	TMenuItem *miDeleteDrinks;
	TMenuItem *miDuplicateDrink;
	TOpenDialog *OpenDialog1;
	TPopupMenu *pmForcedSides;
	TMenuItem *MenuItem3;
	TMenuItem *miDeleteSide;
	TMenuItem *miMoveSideUp;
	TPanel *Panel2;
	TPanel *Panel8;
	TPanel *Panel9;
	TLabel *lbeTitle;
	TPanel *Panel3;
	TProgressBar *LoadProgress;
	TTreeView *tvMenu;
	TButton *btnNewMenu;
	TPageControl *PageControl4;
	TTabSheet *tsProperties;
	TTabSheet *tsErrorsWarnings;
	TButton *btnErrorWarningSettings;
	TPageControl *PageControl1;
	TTabSheet *tsMenu;
	TTabSheet *tsCourse;
	TTabSheet *tsItem;
	TTabSheet *tsItemSize;
	TTabSheet *tsOptions;
	TTabSheet *tsOption;
	TGroupBox *GroupBox5;
	TRadioButton *rbDrinkMenu;
	TRadioButton *rbFoodMenu;
	TButton *btnNewCourse;
	TButton *btnEditMenu;
	TButton *btnNewItem;
	TButton *btnNewOption;
	TStaticText *lbeErrorsCount;
	TStaticText *lbeWarningsCount;
	TProgressBar *pbWarnings;
	TCheckBox *cbOptionsEnable;
	TStringGrid *sgOptions;
	TComboBox *ComboBox1;
	TLabel *Label6;
	TLabel *lblMaxRetailPrice;
	TEdit *edOptionName;
	TCheckBox *cbOptionEnable;
	TLabel *Label11;
	TComboBox *cbOptionGroup;
	TButton *btnDeleteOption;
	TLabel *Label7;
	TEdit *edOptionShortName;
	TStringGrid *sgCourse;
	TTabSheet *tsAvailableSizes;
	TGroupBox *GroupBox9;
	TListBox *lbAvailableSizes;
	TBitBtn *btnAddSize;
	TBitBtn *btnSizesDelete;
	TBitBtn *btnSizesEdit;
	TTabSheet *tsCategories;
	TGroupBox *GroupBox10;
	TListBox *lbAvailableCategories;
	TBitBtn *btnNewCategory;
	TBitBtn *btnDeleteCategory;
	TBitBtn *btnEditCategory;
	TLabel *Label5;
	TEdit *edCourseName;
	TCheckBox *cbClassEnable;
	TButton *btnDeleteCourse;
	TLabel *Label10;
	TPanel *Panel1;
	TIBTransaction *MMTransaction;
	TIBTransaction *StockTransaction;
	TIBSQL *qrMenuCount;
	TIBSQL *qrSizes;
	TIBSQL *qrCategories;
	TIBSQL *qrMenu;
	TIBSQL *qrSizeCategories;
	TIBSQL *qrOptions;
	TIBSQL *qrStockDetails;
	TIBSQL *qrMMLocations;
	TIBSQL *qrItemSides;
	TPanel *Panel4;
	TBitBtn *btnFind;
	TBitBtn *btnSavMenu;
	TBitBtn *btnLoadMenu;
	TBitBtn *btnPreview;
	TBitBtn *btnCheck;
	TBitBtn *btnCommit;
	TBitBtn *btnClose;
	TPageControl *pcItemDetails;
	TTabSheet *tsItemDetails;
	TLabel *Label1;
	TLabel *lbeDrinkColour;
	TLabel *Label2;
	TComboBox *cbItemName;
	TCheckBox *cbItemEnable;
	TTouchButton *btnItemColour;
	TButton *btnDeleteItem;
	TCheckBox *chbItemOnlySide;
	TTntEdit *tntedKitchenName;
	TTabSheet *tsItemSizes;
	TBitBtn *btnAddItemSize;
	TListBox *lbSizesUnused;
	TBitBtn *btnDeleteItemSize;
	TListBox *lbSizesUsed;
	TBitBtn *btnMoveSizeDown;
	TBitBtn *btnMoveSizeUp;
	TStringGrid *sgItem;
	TTabSheet *TabSheet6;
	TRadioButton *rbForcedSideGrp1;
	TRadioButton *rbForcedSideGrp5;
	TRadioButton *rbForcedSideGrp6;
	TRadioButton *rbForcedSideGrp2;
	TRadioButton *rbForcedSideGrp3;
	TRadioButton *rbForcedSideGrp7;
	TRadioButton *rbForcedSideGrp8;
	TRadioButton *rbForcedSideGrp4;
	TCheckBox *chbSideMultiSelect;
	TListBox *lbForcedSides;
	TButton *btnAddSide;
	TButton *btnRemoveSide;
	TButton *btnForcedSideMoveUp;
	TTabSheet *TabSheet10;
	TRadioButton *rbCondimentGrp1;
	TRadioButton *rbCondimentGrp5;
	TRadioButton *rbCondimentGrp6;
	TRadioButton *rbCondimentGrp2;
	TRadioButton *rbCondimentGrp3;
	TRadioButton *rbCondimentGrp7;
	TRadioButton *rbCondimentGrp8;
	TRadioButton *rbCondimentGrp4;
	TCheckListBox *chklstCondiments;
	TBitBtn *BitBtn1;
	TPageControl *pcItemSizeDetails;
	TTabSheet *tsItemSizePrice;
	TButton *btnDeleteSize;
	TGroupBox *GroupBox1;
	TGroupBox *GroupBox2;
	TLabel *Label14;
	TLabel *Label16;
	TNumericEdit *edItemCost;
	TCheckBox *CheckBox1;
	TTabSheet *tsSizeCategories;
	TButton *btnAddCategory;
	TButton *btnRemoveCategory;
	TButton *btnSyncCategories;
	TListBox *lbCategories;
	TTabSheet *TabSheet2;
	TListBox *lbSetMenuItems;
	TCheckBox *chbSetMenuMaster;
	TCheckBox *chbSetMenuItem;
	TCheckListBox *lbSetMenuMasters;
	TCheckBox *chbSetMenuStandardItem;
	TTabSheet *TabSheet3;
	TLabel *Label8;
	TLabel *Label12;
	TBitBtn *btnAddRecipe;
	TBitBtn *btnDeleteRecipe;
	TStringGrid *sgRecipe;
	TComboBox *edLoc;
	TBitBtn *btnSetLocationForAllItems;
	TBitBtn *btnAddRecipeToSizes;
	TBitBtn *btnAddRecipeToItems;
	TTabSheet *tsSize;
	TLabel *Label21;
	TEdit *edSizeName;
	TLabel *Label22;
	TTntEdit *tntedSizeKitchenName;
	TButton *btnSizeDeleteSize;
	TTntEdit *tntedCourseKitchenName;
	TLabel *Label23;
	TTntEdit *tntedOptionKitchenName;
	TLabel *Label24;
	TListBox *lbWarnings;
	TTabSheet *tsSizeLoyalty;
	TNumericEdit *neMemberPurchaseCount;
	TLabel *Label25;
	TNumericEdit *neMemberPurchaseDiscount;
	TLabel *Label26;
	TLabel *Label27;
	TBevel *Bevel1;
	TLabel *Label28;
	TBevel *Bevel2;
	TLabel *Label29;
	TNumericEdit *neLocationPurchaseCount;
	TNumericEdit *neLocationPurchaseDiscount;
	TLabel *Label30;
	TCheckBox *chbSideGroupSkip;
	TLabel *Label31;
	TBevel *Bevel3;
	TButton *btnSyncCategory;
	TBevel *Bevel4;
	TLabel *Label32;
	TLabel *edQty;
	TTabSheet *tsCategoryGroups;
	TGroupBox *GroupBox3;
	TListBox *lbCategoryGroups;
	TBitBtn *btnNewCategoryGroup;
	TBitBtn *btnDeleteCategoryGroup;
	TBitBtn *btnEditCategoryGroup;
	TComboBox *cbCategory;
	TLabel *Label33;
	TBitBtn *btnDeleteCategoryGroup2;
	TBitBtn *btnEditCategoryGroup2;
	TLabel *lbeCategoryGroup;
	TBitBtn *btnSetItemsPoints;
	TLabel *Label9;
	TNumericEdit *nePointsPercent;
	TLabel *Label34;
	TBevel *Bevel5;
	TLabel *Label35;
	TBevel *Bevel6;
	TComboBox *cb3rdPartyGroupCode;
	TButton *btnSync3rdPartyGroupItem;
	TButton *btnSync3rdPartyGroupCourse;
	TButton *btnSync3rdPartyGroupMenu;
	TTabSheet *tsCategory;
	TLabel *Label36;
	TBitBtn *btnCategoryDelete;
	TCheckBox *chbAvailableOnPalm;
	TTabSheet *tsServingCourses;
	TGroupBox *GroupBox6;
	TBitBtn *btnNewServingCourse;
	TBitBtn *btnServingCoursesEdit;
	TLabel *Label37;
	TBitBtn *btnUpServingCourse;
	TBitBtn *btnDownServingCourse;
	TBitBtn *btnServingCoursesDelete;
	TListBox *lbAvailableServingCourses;
	TFindDialog *FindDialog;
	TTabSheet *tsServingCourse;
	TLabel *Label38;
	TLabel *Label39;
	TEdit *edServingCourseName;
	TTntEdit *tntedServingCourseKitchenName;
	TButton *btnServingCourseDelete;
	TCheckBox *cbServingCourseEnable;
	TCheckBox *cbServingCoursesEnable;
	TEdit *edCategoryName;
	TLabel *Label40;
	TColorBox *cbTextColor;
	TTabSheet *tsPrinters;
	TCheckBox *cbUnderline;
	TCheckBox *cbBold;
	TCheckBox *cbLargeFont;
	TCheckBox *cbDoubleWidth;
	TCheckBox *cbDoubleHeight;
	TCheckBox *cbColour;
	TComboBox *cbServingCourse;
	TButton *btnSyncServingCourse;
	TCheckBox *cbUnderlineOptions;
	TCheckBox *cbBoldOptions;
	TCheckBox *cbColourOptions;
	TCheckBox *cbLargeFontOptions;
	TCheckBox *cbDoubleWidthOptions;
	TCheckBox *cbDoubleHeightOptions;
	TBevel *Bevel7;
	TLabel *Label42;
	TLabel *Label43;
	TBevel *Bevel8;
	TCheckBox *cbLocationDiscountAlways;
	TCheckBox *cbMemberDiscountAlways;
	TCheckBox *cbNoDefaultServingCourse;
	TButton *btnSyncNoDefaultSC;
	TButton *btnScanBarcode;
	TIBQuery *IBQuery1;
	TCheckBox *chbOptionMultiSelect;
	TCheckBox *chbOptionGroupSkip;
	TActionList *ActionList1;
	TCheckBox *cbPlusOption;
	TCheckBox *cbMinusOption;
	TBevel *Bevel9;
	TLabel *Label41;
	TCheckBox *cbSelectable;
	TButton *btnSyncSide;
	TButton *btnSyncOptions;
	TButton *btnSyncPrinting;
	TCheckBox *chbSizeWeighed;
	TCheckBox *cbFreeItem;
	TLabel *lbeTareWeight;
	TNumericEdit *neTareWeight;
	TButton *btnCoursePrices;
	TButton *Button1;
	TCheckBox *cbPalm;
	TCheckBox *cbBar;
	TCheckBox *cbRestaurant;
	TButton *btnAddRecipesToAllSizes;
	TLabel *lblItemHandheldName;
	TLabel *lblItemReceiptName;
    TLabel *lblItemIdentifier;
	TLabel *lblItemSizeIdentifier;
	TTntEdit *tntedItemHandheldName;
	TTntEdit *tntedItemReceiptName;  
	TTntEdit *tntedCourseHandheldName;
	TTntEdit *tntedCourseReceiptName;
    TTntEdit *tntedItemIdentifier;
    TTntEdit *tntedItemSizeIdentifier;
	TLabel *lblCourseReceiptName;
	TLabel *lblCourseHandheldName;
	TLabel *lblOptionHandheldName;
	TLabel *lblOptionReceiptName;
	TTntEdit *tntedOptionHandheldName;
	TTntEdit *tntedOptionReceiptName;
	TTntEdit *tntedSizeHandheldName;
	TTntEdit *tntedSizeReceiptName;
	TLabel *lblSizeHandheldName;
	TLabel *lblSizeReceiptName;
	TLabel *lblPointsRedemption;
	TBevel *bvlPointsRedemption;
	TCheckBox *cbCanBePaidForUsingPoints;
	TBitBtn *btnSetItemsSpendingPoints;
	TLabel *lblDefaultPatronCount;
	TEdit *edDefaultPatronCount;
	TButton *btnSyncDefaultPatronCount;
	TTabSheet *tsTaxProfiles;
	TListBox *lbUnassigned;
	TListBox *lbAssigned;
	TBitBtn *btnAddTP;
	TBitBtn *btnRemoveTP;
	TLabel *Label44;
	TLabel *Label45;
	TBitBtn *btnSyncTP;
	TNumericEdit *edCostGST;
	TLabel *Label13;
	TNumericEdit *edRecipeQty;
	TLabel *Label20;
	TNumericEdit *edItemMaxPrice;
	TLabel *lblGlCode;
	TEdit *edGlCode;
    TStringGrid *sgItemsize;
    TIBSQL *qrGetNoOfPriceLevels;
    TIBSQL *qrGetPriceLevelNames;
    TIBSQL *qrGetAllPriceLevelNames;
    TIBSQL *qrLocations;
    TLabel *lblPriceForPoint;
    TNumericEdit *nePriceForPoint;
    TIBSQL *qrGetTaxSettings;
    TComboBox *cbRevenueGroupCode;
    TBevel *Bevel10;
    TButton *btnSyncRevenueCodeItem;
    TButton *btnSyncRevenueCodeCourse;
    TButton *btnSyncRevenueCodeMenu;
    TLabel *Label3;
    TButton *btnGenItemID;
    TButton *btnGenItemSizeID;
    TIBSQL *qrItemIdentifier;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall tvMenuGetImageIndex(TObject *Sender,
	TTreeNode *Node);
	void __fastcall tvMenuGetSelectedIndex(TObject *Sender,
	TTreeNode *Node);
	void __fastcall tvMenuChange(TObject *Sender, TTreeNode *Node);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall btnNewCourseClick(TObject *Sender);
	void __fastcall btnDeleteCourseClick(TObject *Sender);
	void __fastcall sgCourseDrawCell(TObject *Sender, int ACol, int ARow,
	TRect &Rect, TGridDrawState State);
	void __fastcall sgCourseEnter(TObject *Sender);
	void __fastcall sgCourseExit(TObject *Sender);
	void __fastcall sgCourseKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift);
	void __fastcall sgCourseKeyUp(TObject *Sender, WORD &Key,
	TShiftState Shift);
	void __fastcall sgCourseSelectCell(TObject *Sender, int ACol, int ARow,
	bool &CanSelect);
	void __fastcall sgCourseSetEditText(TObject *Sender, int ACol, int ARow,
	const AnsiString Value);
	void __fastcall edCourseNameChange(TObject *Sender);
	void __fastcall cbBarClick(TObject *Sender);
	void __fastcall cbBarKeyPress(TObject *Sender, char &Key);
	void __fastcall cbRestaurantKeyPress(TObject *Sender, char &Key);
	void __fastcall cbPalmKeyPress(TObject *Sender, char &Key);
	void __fastcall cbClassEnableClick(TObject *Sender);
	void __fastcall tvMenuChanging(TObject *Sender, TTreeNode *Node,
	bool &AllowChange);
	void __fastcall tvMenuDeletion(TObject *Sender, TTreeNode *Node);
	void __fastcall cbItemNameChange(TObject *Sender);
	void __fastcall cbItemNameDropDown(TObject *Sender);
	void __fastcall cbItemNameKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift);
	void __fastcall cbItemEnableClick(TObject *Sender);
	void __fastcall stDrinkColourClick(TObject *Sender);
	void __fastcall btnNewItemClick(TObject *Sender);
	void __fastcall btnDeleteItemClick(TObject *Sender);
	void __fastcall sgItemDrawCell(TObject *Sender, int ACol, int ARow,
	TRect &Rect, TGridDrawState State);
	void __fastcall sgItemEnter(TObject *Sender);
	void __fastcall sgItemExit(TObject *Sender);
	void __fastcall sgItemKeyUp(TObject *Sender, WORD &Key,
	TShiftState Shift);
	void __fastcall sgItemSelectCell(TObject *Sender, int ACol, int ARow,
	bool &CanSelect);
	void __fastcall sgItemSetEditText(TObject *Sender, int ACol, int ARow,
	const AnsiString Value);
	void __fastcall cbOptionsEnableClick(TObject *Sender);
	void __fastcall btnNewOptionClick(TObject *Sender);
	void __fastcall edOptionShortNameChange(TObject *Sender);
	void __fastcall edOptionShortNameKeyPress(TObject *Sender,
	char &Key);
	void __fastcall cbOptionEnableClick(TObject *Sender);
	void __fastcall btnDeleteOptionClick(TObject *Sender);
	void __fastcall cbFreeItemClick(TObject *Sender);
	void __fastcall edItemPriceEnter(TObject *Sender);
	void __fastcall edKeyPress(TObject *Sender, char &Key);
	void __fastcall btnDeleteSizeClick(TObject *Sender);
	void __fastcall btnAddRecipeClick(TObject *Sender);
	void __fastcall btnDeleteRecipeClick(TObject *Sender);
	void __fastcall sgRecipeSelectCell(TObject *Sender, int ACol, int ARow,
	bool &CanSelect);
	void __fastcall WozedItemCategoryKeyPress(TObject *Sender, char &Key);
	void __fastcall btnAddRecipeToSizesClick(TObject *Sender);
	void __fastcall btnAddRecipeToItemsClick(TObject *Sender);
	void __fastcall btnAddItemSizeClick(TObject *Sender);
	void __fastcall btnMoveSizeUpClick(TObject *Sender);
	void __fastcall lbSizesUsedClick(TObject *Sender);
	void __fastcall lbSizesUnusedClick(TObject *Sender);
	void __fastcall btnAddSizeClick(TObject *Sender);
	void __fastcall btnSizesDeleteClick(TObject *Sender);
	void __fastcall btnMoveSizeDownClick(TObject *Sender);
	void __fastcall btnSizesEditClick(TObject *Sender);
	void __fastcall sgOptionsKeyPress(TObject *Sender, char &Key);
	void __fastcall sgItemKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift);
	void __fastcall Expand1Click(TObject *Sender);
	void __fastcall Collapse1Click(TObject *Sender);
	void __fastcall miDeleteDrinksClick(TObject *Sender);
	void __fastcall miDuplicateDrinkClick(TObject *Sender);
	void __fastcall tvMenuKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift);
	void __fastcall tvMenuMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y);
	void __fastcall tvMenuEditing(TObject *Sender, TTreeNode *Node,
	bool &AllowEdit);
	void __fastcall btnCheckClick(TObject *Sender);
	void __fastcall btnLoadMenuClick(TObject *Sender);
	void __fastcall btnCommitClick(TObject *Sender);
	void __fastcall btnSavMenuClick(TObject *Sender);
	void __fastcall tvMenuDragOver(TObject *Sender, TObject *Source,
	int X, int Y, TDragState State, bool &Accept);
	void __fastcall tvMenuDragDrop(TObject *Sender, TObject *Source,
	int X, int Y);
	void __fastcall sgOptionsKeyUp(TObject *Sender, WORD &Key,
	TShiftState Shift);
	void __fastcall edOptionNameChange(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall cbFreeItemKeyPress(TObject *Sender, char &Key);
	void __fastcall btnPreviewClick(TObject *Sender);
	void __fastcall btnDeleteItemSizeClick(TObject *Sender);
	void __fastcall edItemCostChange(TObject *Sender);
	void __fastcall edItemCostExit(TObject *Sender);
	void __fastcall btnNewMenuClick(TObject *Sender);
	void __fastcall btnEditMenuClick(TObject *Sender);
	void __fastcall chbSetMenuMasterClick(TObject *Sender);
	void __fastcall chbSetMenuItemClick(TObject *Sender);
	void __fastcall chbSetMenuStandardItemClick(TObject *Sender);
	void __fastcall lbSetMenuMastersClick(TObject *Sender);
	void __fastcall pmDrinksBevsPopup(TObject *Sender);
	void __fastcall btnScanBarcodeClick(TObject *Sender);
	void __fastcall btnErrorWarningSettingsClick(TObject *Sender);
	void __fastcall rbCondimentGrpClick(TObject *Sender);
	void __fastcall cbOptionGroupChange(TObject *Sender);
	void __fastcall chklstCondimentsClickCheck(TObject *Sender);
	void __fastcall sgOptionsSelectCell(TObject *Sender, int ACol, int ARow,
	bool &CanSelect);
	void __fastcall ComboBox1Change(TObject *Sender);
	void __fastcall ComboBox1Enter(TObject *Sender);
	void __fastcall ComboBox1KeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift);
	void __fastcall ComboBox1KeyPress(TObject *Sender, char &Key);
	void __fastcall ComboBox1Exit(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall sgOptionsDrawCell(TObject *Sender, int ACol, int ARow,
	TRect &Rect, TGridDrawState State);
	void __fastcall sgOptionsMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y);
	void __fastcall btnAddCategoryClick(TObject *Sender);
	void __fastcall btnRemoveCategoryClick(TObject *Sender);
	void __fastcall btnNewCategoryClick(TObject *Sender);
	void __fastcall btnSyncCategoriesClick(TObject *Sender);
	void __fastcall btnEditCategoryClick(TObject *Sender);
	void __fastcall btnDeleteCategoryClick(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall sgItemKeyPress(TObject *Sender, char &Key);
	void __fastcall cbItemEnableKeyPress(TObject *Sender, char &Key);
	void __fastcall btnSetLocationForAllItemsClick(TObject *Sender);
	void __fastcall btnAddSideClick(TObject *Sender);
	void __fastcall rbForcedSideGrpClick(TObject *Sender);
	void __fastcall chbSideMultiSelectClick(TObject *Sender);
	void __fastcall btnRemoveSideClick(TObject *Sender);
	void __fastcall lbForcedSidesMouseDown(TObject *Sender,
	TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall cbClassEnableKeyPress(TObject *Sender, char &Key);
	void __fastcall lbSizesUnusedKeyPress(TObject *Sender, char &Key);
	void __fastcall cbOptionGroupKeyPress(TObject *Sender, char &Key);
	void __fastcall GroupBox5Enter(TObject *Sender);
	void __fastcall rbFoodMenuKeyPress(TObject *Sender, char &Key);
	void __fastcall edLocChange(TObject *Sender);
	void __fastcall chbItemOnlySideClick(TObject *Sender);
	void __fastcall chbItemOnlySideKeyPress(TObject *Sender, char &Key);
	void __fastcall nePointsPercentExit(TObject *Sender);
	void __fastcall btnFindClick(TObject *Sender);
	void __fastcall FindDialogFind(TObject *Sender);
	void __fastcall btnForcedSideMoveUpClick(TObject *Sender);
	void __fastcall miMoveSideUpClick(TObject *Sender);
	void __fastcall nePointsPercentKeyPress(TObject *Sender, char &Key);
	void __fastcall kitchenNameChange(TObject *Sender);
	void __fastcall tntedSizeKitchenNameChange(TObject *Sender);
	void __fastcall tntedCourseKitchenNameChange(TObject *Sender);
	void __fastcall tntedOptionKitchenNameChange(TObject *Sender);
	void __fastcall tntedKitchenNameKeyPress(TObject *Sender, char &Key);
	void __fastcall lbWarningsDrawItem(TWinControl *Control, int Index,
	TRect &Rect, TOwnerDrawState State);
	void __fastcall lbWarningsDblClick(TObject *Sender);
	void __fastcall btnSetItemsSpendingPointsClick(TObject *Sender);
	void __fastcall neMemberPurchaseCountExit(TObject *Sender);
	void __fastcall neLocationPurchaseCountExit(TObject *Sender);
	void __fastcall neMemberPurchaseDiscountExit(TObject *Sender);
	void __fastcall neLocationPurchaseDiscountExit(TObject *Sender);
	void __fastcall chbSideGroupSkipClick(TObject *Sender);
	void __fastcall btnSyncCategoryClick(TObject *Sender);
	void __fastcall edQtyKeyPress(TObject *Sender, char &Key);
	void __fastcall btnNewCategoryGroupClick(TObject *Sender);
	void __fastcall cbCategoryDrawItem(TWinControl *Control, int Index,
	TRect &Rect, TOwnerDrawState State);
	void __fastcall cbCategorySelect(TObject *Sender);
	void __fastcall btnEditCategoryGroupClick(TObject *Sender);
	void __fastcall btnDeleteCategoryGroupClick(TObject *Sender);
	void __fastcall btnEditCategoryGroup2Click(TObject *Sender);
	void __fastcall btnDeleteCategoryGroup2Click(TObject *Sender);
	void __fastcall cb3rdPartyGroupCodeChange(TObject *Sender);
	void __fastcall btnSync3rdPartyGroupItemClick(TObject *Sender);
	void __fastcall btnSync3rdPartyGroupCourseClick(TObject *Sender);
	void __fastcall btnSync3rdPartyGroupMenuClick(TObject *Sender);
	void __fastcall chbAvailableOnPalmKeyPress(TObject *Sender, char &Key);
	void __fastcall btnSizeDeleteSizeClick(TObject *Sender);
	void __fastcall btnNewServingCourseClick(TObject *Sender);
	void __fastcall btnServingCoursesEditClick(TObject *Sender);
	void __fastcall btnServingCoursesDeleteClick(TObject *Sender);
	void __fastcall lbAvailableCategoriesKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift);
	void __fastcall lbCategoriesKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift);
	void __fastcall btnUpServingCourseClick(TObject *Sender);
	void __fastcall btnDownServingCourseClick(TObject *Sender);
	void __fastcall lbAvailableServingCoursesDrawItem(TWinControl *Control,
	int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall cbServingCourseEnableClick(TObject *Sender);
	void __fastcall cbServingCoursesEnableClick(TObject *Sender);
	void __fastcall lbAvailableServingCoursesKeyDown(TObject *Sender,
	WORD &Key, TShiftState Shift);
	void __fastcall btnServingCourseDeleteClick(TObject *Sender);
	void __fastcall tvMenuEndDrag(TObject *Sender, TObject *Target, int X,
	int Y);
	void __fastcall lbAvailableServingCoursesEnter(TObject *Sender);
	void __fastcall tvMenuEnter(TObject *Sender);
	void __fastcall btnCategoryDeleteClick(TObject *Sender);
	void __fastcall tntedServingCourseKitchenNameChange(TObject *Sender);
	void __fastcall cbTextColorChange(TObject *Sender);
	void __fastcall cbColourClick(TObject *Sender);
	void __fastcall cbServingCourseChange(TObject *Sender);
	void __fastcall btnSyncServingCourseClick(TObject *Sender);
	void __fastcall cbMemberDiscountAlwaysClick(TObject *Sender);
	void __fastcall cbLocationDiscountAlwaysClick(TObject *Sender);
	void __fastcall cbNoDefaultServingCourseClick(TObject *Sender);
	void __fastcall btnSyncNoDefaultSCClick(TObject *Sender);
	void __fastcall chbOptionMultiSelectClick(TObject *Sender);
	void __fastcall chbOptionGroupSkipClick(TObject *Sender);
	void __fastcall cbPlusOptionClick(TObject *Sender);
	void __fastcall pcItemDetailsChange(TObject *Sender);
	void __fastcall cbSelectableClick(TObject *Sender);
	void __fastcall btnSyncSideClick(TObject *Sender);
	void __fastcall btnSyncOptionsClick(TObject *Sender);
	void __fastcall btnSyncPrintingClick(TObject *Sender);
	void __fastcall chbSizeWeighedClick(TObject *Sender);
	void __fastcall neTareWeightEnter(TObject *Sender);
	void __fastcall neTareWeightExit(TObject *Sender);
	void __fastcall btnCoursePricesClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall btnAddRecipesToAllSizesClick(TObject *Sender);
	void __fastcall droidwareNameChange(TObject *Sender);
	void __fastcall cbCanBePaidForUsingPointsClicked(TObject *Sender);
	void __fastcall btnSetItemsPointsClick(TObject *Sender);
	void __fastcall OnLeaveDefaultPatronCount(TObject *Sender);
	void __fastcall SyncDefaultPatronCountClicked(TObject *Sender);
	void __fastcall btnAddTPMouseClick(TObject *Sender);
	void __fastcall btnRemoveTPMouseClick(TObject *Sender);
	void __fastcall lbUnassignedDblClick(TObject *Sender);
	void __fastcall lbAssignedDblClick(TObject *Sender);
	void __fastcall btnSyncTPClick(TObject *Sender);
	void __fastcall edCostGSTExit(TObject *Sender);
	void __fastcall edGlCodeChange(TObject *Sender);
	void __fastcall edGlCodeEnter(TObject *Sender);
	void __fastcall edGlCodeExit(TObject *Sender);
    void __fastcall sgItemSizeDrawCell(TObject *Sender, int ACol, int ARow,
	TRect &Rect, TGridDrawState State);
	void __fastcall sgItemSizeEnter(TObject *Sender);
	void __fastcall sgItemSizeExit(TObject *Sender);
	void __fastcall sgItemSizeKeyUp(TObject *Sender, WORD &Key,
	TShiftState Shift);
	void __fastcall sgItemSizeSelectCell(TObject *Sender, int ACol, int ARow,
	bool &CanSelect);
	void __fastcall sgItemSizeEditText(TObject *Sender, int ACol, int ARow,
	const AnsiString Value);
    void __fastcall sgItemSizeKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift);
    void __fastcall sgItemSizeKeyPress(TObject *Sender, char &Key);
    void __fastcall nePriceForPointExit(TObject *Sender);
    void __fastcall nePriceForPointChange(TObject *Sender);
    void __fastcall nePriceForPointEnter(TObject *Sender);
    //void __fastcall cbRevenueGroupCodeChange(TObject *Sender);
	void __fastcall btnSyncRevenueCodeItemClick(TObject *Sender);
	void __fastcall btnSyncRevenueCodeCourseClick(TObject *Sender);
	void __fastcall btnSyncRevenueCodeMenuClick(TObject *Sender);
    void __fastcall cbRevenueGroupCodeSelect(TObject *Sender);
    void __fastcall lbAvailableSizesClick(TObject *Sender);
    void __fastcall btnGenItemIDClick(TObject *Sender);
    void __fastcall btnGenItemSizeIDClick(TObject *Sender);
    void __fastcall ItemIdentifierChange(TObject *Sender);
    void __fastcall ItemSizeIdentifierChange(TObject *Sender);
    void __fastcall chbAvailableOnPalmClick(TObject *Sender);
protected:
	void __fastcall WMLoadMenu(TMessage& Message);
	void __fastcall WMLoadMenuFile(TMessage& Message);
	void __fastcall WMSysCommand(TWMSysCommand& Message);

	BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_LOADMENU, TMessage, WMLoadMenu)
	//			MESSAGE_HANDLER(WM_LOADMENU_DB, TMessage, WMLoadMenuDB)
	//			MESSAGE_HANDLER(WM_LOADMENU_FILE, TMessage, WMLoadMenuFile)
	MESSAGE_HANDLER(WM_SYSCOMMAND, TWMSysCommand, WMSysCommand);
	END_MESSAGE_MAP(TForm)

private:
	Menu::TDeletedServingCoursesInfo DeletedServingCoursesInfo;
	bool SaveFoodMenu;
	std::auto_ptr<TStringList> itemsList;
	std::auto_ptr<TList>	forcedSideList;

	bool LoadTree(int MenuKey);
	void resetReceipeQtyLebel();
	void LoadCategories(CategoryTranslation::TUpdateCategories *UpdateCategories);
	void LoadCategories();
	void LoadCategories(TCategoryList &CategoryList);
	bool NewMenu();

	bool RetrieveMMStock(AnsiString *Code, AnsiString *Desc, AnsiString *Unit,Currency *ConversionFactor ,AnsiString *ReductionUnit,AnsiString DefaultNewName);
	void RelabelDrinkCosts();
	bool MenuEdited;
	bool CompilingDrinkWarnings;
	int DrinkErrorCount;
	bool CompilingDrinkWarningsCompleted;
	AnsiString LastLocation;

	void RefreshMenuDetails();
	void RefreshSizes(TMenuNode *SizesData);
	void RefreshSize(TSizeNode *SizeData);
	void RefreshServingCourses(TServingCoursesNode *ServingData);
	void RefreshServingCourse(TServingCourseNode *ServingData);
	void RefreshCategories(TCategoryGroupNode *CategoriesData);
	void RefreshCategoryGroups(TCategoryGroupsNode *CategoryGroupsData);
	void RefreshCategory(TCategoryNode *CategoryData);
	void RefreshItemRecipe(TItemSizeNode *DrinkCostData);
	void RefreshItemSize(TItemSizeNode *DrinkCostData);
	void RefreshItem(TItemNode *DrinkData, bool isItemTranfer= false);
	void RefreshCourse(TCourseNode *ClassData);
	void RefreshMenu(TMenuNode *ClassesData);
	void RefreshOption(TOptionNode *OptionData);
	void RefreshOptions(TOptionsNode *OptionsData);

	AnsiString ReadCSVWord(AnsiString Line, int& Offset, char Delimiter, char Qualifier, char Escape=NULL);
	AnsiString EscapeString(AnsiString Line, char Qualifier='\"', char Escape='\\');
	bool SaveNode(TTreeNode *TreeNode);

	void FindSetMenuMasters(int SelectedSetMenuMask);
	void ClearSetMenuMasters();
	void FindSetMenuItems(int SelectedSetMenuMask);
	void ClearSetMenuItems();
	//	void LinkMenuSides();
	void GetAllSizes(TStringList *AllSizes);
	void GetAllCategories(TStringList *AllCategories);
	void GetAllCategoriesWithKeys(std::vector<Menu::TNameAndKey> *AllCategoriesWithKeys); 
	void GetAll3rdPartyGroups(TStrings *ThirdPartyGroups);
	void GetAllServingCourses(TStringList *AllServingCourses);
	void LoadServingCoursesPrior3Point4(TMenuNode *MenuData);
	void GetThirdPartyCodesListFromFile(std::vector<Menu::TThirdPartyCodeInfo> *thirdPartyCodes, TLoadMenu *inLoadMenu);
    void GetRevenueCodesListFromFile(std::map<int,AnsiString> &revenueCodesMap, TLoadMenu *inLoadMenu);
    AnsiString GetRevenueDecriptionFromCode(int code);
	AnsiString  GetThirdPartyCodeFromKeyFromFile(std::vector<Menu::TThirdPartyCodeInfo> *thirdPartyCodes, __int32 tpcKey);

	int InsertCategory(TCategoryGroupNode *CategoryGroup, AnsiString CategoryName, AnsiString GlCode);
	int InsertCategoryGroup(AnsiString GroupName);
	int InsertServingCourse(AnsiString ServingCourseName);
	bool FindCategory(AnsiString CategoryName, int *GroupIndex, int *CategoryIndex, bool *Deleted);
	bool FindCategoryGroup(AnsiString GroupName, int *GroupIndex);
	TTreeNode* TfrmMenuEdit::FindCategoryGroup(AnsiString GroupName);
	int InsertSize(AnsiString SizeName);
	void DeleteCategory(int GroupIndex, int CategoryIndex);
	void DeleteCategoryGroup(int GroupIndex);
	void DeleteSize(int Index);
	void DeleteServingCourse(int Index);
	void DeleteFromServingCourses(const AnsiString ServingCourseName);
	void AddToDeletedServingCourses(TServingCourseNode *ServingCourseNode);
	int EditCategory(AnsiString CategoryName, int GroupIndex, int CategoryIndex);
	int EditCategoryGroup(AnsiString GroupName, int GroupIndex);
	int EditSize(AnsiString SizeName, int Index);
	int EditServingCourse(AnsiString ServingCourseName, int Index);
	void SetAllForcedOptions( int MaxSelect, bool OptionGroupSkip);
	void SetForcedOptionToGroup(TOptionNode *ChangedOptionData );

	inline int NumberOfCourses() { return tvMenu->Items->GetFirstNode()->Count - 2; }

	Currency GetStockCost(AnsiString StockCode, AnsiString Location);

	Graphics::TBitmap *FCheck;
	Graphics::TBitmap *FNoCheck;
	void ToggleCheckbox(int ACol, int ARow);

	bool Registered;
	unsigned int Modules;

	void ClearTree();

	static bool __fastcall optionKeyInTheList( __int32 inOptionKey, std::vector<Menu::TNameAndKey> forcedOptions );

	__int32 __fastcall CategoryKeyWithName( AnsiString inCategory );
	__int32 __fastcall ServingCourseKeyWithName( AnsiString inSCName );

	//*******************************************************

	TButtonResult ShowLoadMenuDialog();

	//::::::::::::::::::::::::::::::::::::::::::::

	TItemSizeNode *selectedItemSizeNode;

	TMenuTaxProfileProvider *menuTaxProfileProvider;
	TTaxProfileAssigner     *menuTaxProfileAssigner;

	void ShowHideMaxRetailPrice(bool displayFlag);
	bool IsProfitTaxAssigned(TaxProfileKeyList taxProfileKeys);

	void initMenuTaxProfileProvider();

	void resetMenuTaxProfileProvider( TIBDatabase* inDatabase );
	void resetMenuTaxProfileProvider( TLoadMenu*   inLoadMenu );
	void resetTaxProfileAssigner( TMenuTaxProfileProvider* inMenuTaxProfileProvider );

	// This function must be called everytime an item size is selected in the Menu tree node
	void setTaxProfilesInAssignerForItemSize(TItemSizeNode* inItemSizeNode, TMenuTaxProfileProvider* inMenuTaxProfileProvider );

	void __fastcall assignedTaxProfilesChanged(TObject* sender, TStrings* const taxProfileContainer );

	//::::::::::::::::::::::::::::::::::::::::::::

	bool LoadMenu();

	bool LoadFromDB();
	bool LoadFromFile();
	bool LoadNew();

	//::::::::::::::::::::::::::::::::::::::::::
	//  Load Menu from DB
	//::::::::::::::::::::::::::::::::::::::::::
	bool LoadMenuFromDB();
	bool InitMenuTransaction();
	__int32 GetMenuDBKey();
	__int32 GetMenuDBCount( __int32 inMenuKey );

	bool LoadTree2(int MenuKey);
	TTreeNode* AddMenu(
	Menu::TMenuInfo *MenuInfo,
	Menu::TSizesInfo *SizesInfo,
	Menu::TCategoriesInfo *CategoriesInfo,
	Menu::TLocationsInfo *LocationsInfo,
	Menu::TServingCoursesInfo *ServingCoursesInfo);  // cww
	void* AddServingCourses(Menu::TServingCoursesInfo *ServingCoursesInfo,TTreeNode *ServingCoursesNode);
	void* AddServingCourse(Menu::TServingCoursesInfo *ServingCoursesInfo,TTreeNode *ServingCoursesNode,int i);
	TTreeNode* AddMenuCourse(TTreeNode *MenuNode, Menu::TCourseInfo *CourseInfo);

	TTreeNode* AddMenuItem( Menu::TMenuLoadDB& inMenuLoader, // Add MenuItem from DB
	TTreeNode*         inCourseNode,
	Menu::TItemInfo*   inItemInfo,
	TList*             inForcedSideList );
	TTreeNode* AddMenuItem( TLoadMenu*       inLoadMenu,  // Add MenuItem from XML file
	__int32          inItemHandle,
	TTreeNode*       inCourseNode,
	Menu::TItemInfo* inItemInfo,
	TList*           inForcedSideList );
	TTreeNode* CreateItemTreeNode( TTreeNode*       inCourseNode,
	Menu::TItemInfo* inItemInfo,
	TList*           inForcedSideList );

	TTreeNode* AddMenuSize(TTreeNode *ItemNode, Menu::TItemSizeInfo *ItemSizeInfo);
	void ReadRecipeDescriptionAndUnit( AnsiString  inStockCode,    AnsiString  inLocation,
	AnsiString& outDescription, AnsiString& outUnit, Currency& outConversionFactor );
	void LoadLocations(Menu::TLocationsInfo *LocationsInfo);
	void LoadLocations();

	//::::::::::::::::::::::::::::::::::::::::::
	//   Load Menu from a XML file
	//::::::::::::::::::::::::::::::::::::::::::
	bool LoadMenuFromFile( AnsiString FileName );
	bool LoadTreeFromXML( TLoadMenu* inLoadMenu );
	void InitProgressBar( int inMax );
	void InitControls();
	TTreeNode* CreateMenuNode(   TLoadMenu *inLoadMenu );
	void ReadMenuInfo(           TLoadMenu *inLoadMenu, Menu::TMenuInfo &inMenuInfo );
	void ReadSizesInfo(          TLoadMenu *inLoadMenu, Menu::TSizesInfo &inSizesInfo );
	void ReadCategoriesInfo(     TLoadMenu *inLoadMenu, Menu::TCategoriesInfo &inCategoriesInfo );
	void ReadServingCoursesInfo( TLoadMenu *inLoadMenu, Menu::TServingCoursesInfo &ServingCoursesInfo );

	bool CreateCourseNodes(     TLoadMenu *inLoadMenu );
	void ReadCourseOptionsInfo( TLoadMenu* inLoadMenu, __int32 inCourseID, Menu::TCourseInfo& inCourseInfo );

	bool CreateItemNodes(           TLoadMenu *inLoadMenu, __int32 inCourseHandle, WideString inCourseName, TTreeNode *inCourseNode );
	bool AssignForcedSidesReferences();
	void ReadItemForcedSidesInfo( const WideString &inCourseName, TLoadMenu *inLoadMenu, __int32 inItemID, Menu::TItemInfo &inItemInfo );
	void ReadItemForcedOptionsInfo( __int32    inGroupNumber,
	__int32    inItemKey,
	TLoadMenu *inLoadMenu,
	__int32    inItemHandle,
	std::vector<TForcedOption> &infOptions );
	void ReadItemSizeTaxProfilesInfo( TLoadMenu *inLoadMenu, __int32 inItemSizeID, Menu::TItemSizeInfo &inItemSizeInfo );

	bool CreateItemSizeNodes(         TLoadMenu *inLoadMenu, __int32 inItemID, TTreeNode *inItemNode );
	void ReadItemSizeBCategoriesInfo( TLoadMenu *inLoadMenu, __int32 inItemSizeID, Menu::TItemSizeInfo& inItemSizeInfo );
	void ReadItemSizeReceipesInfo(    TLoadMenu *inLoadMenu, __int32 inItemSizeID, Menu::TItemSizeInfo& inItemSizeInfo );

    void ReadItemSizePriceLevel( TLoadMenu *inLoadMenu, __int32 inItemSizeID, Menu::TItemSizeInfo& inItemSizeInfo );

	void StartStockTransaction();
    /*bool SyncRevenueCodeItem(TTreeNode *CurrentTreeNode, int revenueCode,AnsiString revenueDescription);
    bool Sync3rdPartyGroupItem(TTreeNode *CurrentTreeNode,AnsiString MasterThirdPartyCode);
    bool SyncRevenueCodeForCourse(TTreeNode *CurrentTreeNode,int revenueCode,AnsiString revenueDescription);
    bool Sync3rdPartyCodeForCourse(TTreeNode *CurrentTreeNode,AnsiString MasterThirdPartyCode);
    bool Sync3rdPartyCodeForMenu(TTreeNode *CurrentTreeNode,AnsiString MasterThirdPartyCode);
    bool SyncRevenueCodeForMenu(TTreeNode *CurrentTreeNode,int revenueCode,AnsiString revenueDescription);*/



	//::::::::::::::::::::::::::::::::::::::::::
	//   Save Menu to a XML file
	//::::::::::::::::::::::::::::::::::::::::::
	void SaveMenu( AnsiString inFileName, AnsiString inBackupFileName );
	void SaveMenuCategoryGroups( TSaveMenu* inSaveMenu, TTreeNode* inCategoryGroupsNode );
	void SaveMenuSizes( TSaveMenu* inSaveMenu, TTreeNode* inSizesTreeNode );
	void SaveMenuServingCourses( TSaveMenu* inSaveMenu, TTreeNode* inServingCoursesTreeNode );
	void SaveMenuThirdPartyGroups( TSaveMenu* inSaveMenu, TTreeNode* inMenuNode );
	void SaveMenuTaxProfiles( TSaveMenu* inSaveMenu, std::set<TaxProfile*>* const inTaxProfileStructure );
	void SaveMenuCourses( TSaveMenu* inSaveMenu, TTreeNode* inMenuNode );
	__int32 SaveMenuCourse( TSaveMenu* inSaveMenu, TCourseNode *inCourseData );
	void SaveMenuCourseItems( TSaveMenu* inSaveMenu, __int32 inCourseID, TTreeNode *inCourseNode );
	__int32 SaveMenuCourseItem( TSaveMenu* inSaveMenu, __int32 inCourseID, TItemNode *inItemData );
	void SaveMenuItemForcedSides( TSaveMenu* inSaveMenu, __int32 inItemID, TItemNode* inItemData );
	void SaveMenuItemForcedOptions( TSaveMenu* inSaveMenu, __int32 inItemHandle, TItemNode* inItemData );
	void SaveMenuItemSizes( TSaveMenu* inSaveMenu, __int32 inItemID, TTreeNode *inItemNode );
	__int32 SaveMenuItemSize( TSaveMenu* inSaveMenu, __int32 inItemID, TItemSizeNode* inDCData );
	void SaveMenuBreakdownCategories( TSaveMenu* inSaveMenu, __int32 inItemSizeID, TItemSizeNode* inDCData );
	void SaveMenuItemSizeRecipes( TSaveMenu* inSaveMenu, __int32 inItemSizeID, TItemSizeNode* inDCData );
	void SaveMenuItemSizeTaxProfiles( TSaveMenu* inSaveMenu, __int32 inItemSizeID, TItemSizeNode* inDCData );
	void SaveMenuItemOptions( TSaveMenu* inSaveMenu, __int32 inCourseID, TTreeNode* inItemNode );

    void SaveMenuItemSizePriceLevels( TSaveMenu* inSaveMenu, __int32 inItemSizeID, TItemSizeNode* inDCData );

	//*******************************************************

	void AddMenuItemForcedOptions( Menu::TMenuLoadDB& inMenuLoader, // Add Forced Options
	TItemNode*         inItemData,   // from DB
	Menu::TItemInfo*   inItemInfo );
	void AddMenuItemForcedOptions( TLoadMenu*       inLoadMenu,     // Add Forced Options
	__int32          inItemHandle,   // from XML file
	TItemNode*       inItemData,
	Menu::TItemInfo* inItemInfo );

	const std::auto_ptr<TfrmPreview> frmPreview;
	void GetAllThirdPartyGroupsFromTree(std::vector<Menu::TThirdPartyCodeInfo> &thirdPartyCodes);

	TTaxProfileAssigner *testTax;


	std::vector<TTreeNode *>
	get_all_item_sizes_in_course(
	TTreeNode &treeview_course_node);
	TTreeNode *
	get_current_course();

	void UpdateDuplicatedThirdPartyCodes(TTreeNode *ItemNode);
	void removeForcedOptionFromCourseItems(
	TTreeNode* currentCourseNode,
	TOptionNode* changedOptionData,
	int previousOptionGroup );
	AnsiString GetKitchenNameOfSize(AnsiString SizeName);
    void LoadPriceLevelNames();
    void LoadRecipeLocations(AnsiString stockcode);
    bool CheckIfNumeric(AnsiString Value);
    int UpdateSizeItem(AnsiString SizeName);
    Currency GetPriceExclusiveAmount(Currency menuPrice, Currency saleTaxPercentage, Currency serviceChargePercentage);
    bool isItemPriceIncludedTax;
    bool isItemPriceIncludeServiceCharge;
    std::map<int,AnsiString> revenueCodesMap;
    void SaveMenuRevenueCodes( TSaveMenu* inSaveMenu, TTreeNode* inMenuNode );
    int GetItemIdentifier(AnsiString genQuery);
    void EnableOrDisableGenButtons();
    void ResetItemAndItemSizeIdentifier();
    void SetItemAndItemSizeIdentifier();

public:		// User declarations
	__fastcall TfrmMenuEdit(TComponent* Owner);
	unsigned long GetNextSetMenuMask();
	TColor LastDrinkColour;
	void Execute();

	bool IterateOverMenuItems(TIterationAction &);
    //::::::::::::::::::::::::::::::::::::::::::
	//   Create Xml From CSv
	//::::::::::::::::::::::::::::::::::::::::::
    void CreateXmlFromCsvFile(std::vector<TimportMenu> allCsvMenus, bool itemSizeUniqueCase);

    void CsvSaveMenuCategoryGroups( TSaveMenu* saveMenu, TimportMenu menu );
    void CsvSaveMenuSizes( TSaveMenu* inSaveMenu, TimportMenu menu );
    void CsvSaveMenuServingCourses( TSaveMenu* inSaveMenu, TimportMenu menu );
    void CsvSaveMenuTaxProfiles( TSaveMenu* inSaveMenu, TimportMenu menu );
    void CsvSaveMenuCourses( TSaveMenu* inSaveMenu, TimportMenu menu ,bool itemSizeUniqueCase);

   __int32 CsvSaveMenuSingleCourse(TSaveMenu* inSaveMenu,Tcourse  course );
   __int32 CsvSaveMenuCourseItem( TSaveMenu* inSaveMenu,__int32 xmlCourseID,Titem item );
    void  CsvSaveMenuItemSizes(TSaveMenu* inSaveMenu,__int32 xmlItemID, TitemSize itemSize ,bool itemSizeUniqueCase1 );
    void  CsvSaveMenuItemSizeTaxProfiles( TSaveMenu* inSaveMenu, __int32 xmlitemSizeID, TitemSize itemSize );
    void  CsvSaveMenuItemSizeRecipes(  TSaveMenu* inSaveMenu, __int32 xmlitemSizeID, TitemSize itemSize );

    void  CsvSaveMenuItemSizePriceLevels(  TSaveMenu* inSaveMenu, __int32 xmlitemSizeID, TitemSize itemSize );

    int noOfPriceLevels;
       int allPriceLevels;
    std::map<int,Menu::TPriceLevelsName> PriceLevelsName;
    void UpdateItemForForcedOptions();
    std::map<AnsiString, TAvailableMenuSize > AllSizesForMenu;
    TStringList *ServingCoursesList; 
};
//---------------------------------------------------------------------------
struct TPrinterOptions
{
	bool PrintUnderlined;
	bool PrintBold;
	TColor PrintColour;
	int PrintFont;
	bool PrintDoubleWidth;
	bool PrintDoubleHeight;
};
//---------------------------------------------------------------------------
class TEditorNode : public TObject
{
private:
protected:
public:
	__int32    Key;               // DB Key
	AnsiString LongDescription;
	AnsiString ShortDescription;
	Currency Price;
	bool Enabled;

	virtual void Enable()  { Enabled = true; }
	virtual void Disable() { Enabled = false;}

	NodeTypes NodeType;
	TTreeNode *Owner;             // The node that this object connects.

	virtual TTreeNode *AddNode(NodeTypes NType, bool Expand = true) = 0;
	virtual TTreeNode *CopyNode(TTreeNode *Source) = 0;	// Use to move as well.

	virtual int GetImageIndex(TTreeView *DropTree, TTreeNode *DropNode) = 0;
	virtual int GetSelectedIndex(TTreeView *DropTree, TTreeNode *DropNode) = 0;
	virtual bool CanExcept(TTreeNode *N) = 0;
	virtual TEditorNode *Duplicate() = 0;
	void SetStateIndex()
	{
		Owner->StateIndex = -1;//(Enabled)?-1:DISABLED_PIC;
	}
	void CopyDetails(TEditorNode *S)
	{
		//S->Key              = Key;
		S->Key              = (*menu_key_generator_t::Instance())();
		S->NodeType         = NodeType;
		S->LongDescription  = LongDescription;
		S->ShortDescription = ShortDescription;
		S->Price            = Price;
		S->Enabled          = Enabled;
	}

	AnsiString HandheldName;
	AnsiString ReceiptName;

};
//---------------------------------------------------------------------------
class TMenuNode : public TEditorNode
{
private:
protected:
public:
	TMenuNode();
	TTreeNode *AddNode(NodeTypes N, bool Expand=true);
	TTreeNode *CopyNode(TTreeNode *S);
	TEditorNode *Duplicate();
	int GetImageIndex(TTreeView *DropTree, TTreeNode *DropNode);
	int GetSelectedIndex(TTreeView *DropTree, TTreeNode *DropNode);
	bool CanExcept(TTreeNode *N);
};
//---------------------------------------------------------------------------
class TSizesNode : public TEditorNode
{
private:
protected:
public:
	TSizesNode();
	TTreeNode *AddNode(NodeTypes N, bool Expand);
	TTreeNode *CopyNode(TTreeNode *S);
	TEditorNode *Duplicate();
	int GetImageIndex(TTreeView *DropTree, TTreeNode *DropNode);
	int GetSelectedIndex(TTreeView *DropTree, TTreeNode *DropNode);
	bool CanExcept(TTreeNode *N);
};
//---------------------------------------------------------------------------
class TCategoryGroupsNode : public TEditorNode
{
private:
protected:
public:
	TCategoryGroupsNode();
	TTreeNode *AddNode(NodeTypes N, bool Expand);
	TTreeNode *CopyNode(TTreeNode *S);
	TEditorNode *Duplicate();
	int GetImageIndex(TTreeView *DropTree, TTreeNode *DropNode);
	int GetSelectedIndex(TTreeView *DropTree, TTreeNode *DropNode);
	bool CanExcept(TTreeNode *N);
};
//---------------------------------------------------------------------------
class TServingCoursesNode : public TEditorNode
{
private:
protected:
public:
	TServingCoursesNode();
	TTreeNode *AddNode(NodeTypes N, bool Expand);

	TTreeNode *CopyNode(TTreeNode *S);
	TEditorNode *Duplicate();
	int GetImageIndex(TTreeView *DropTree, TTreeNode *DropNode);
	int GetSelectedIndex(TTreeView *DropTree, TTreeNode *DropNode);
	bool CanExcept(TTreeNode *N);
};
//---------------------------------------------------------------------------
class TSizeNode : public TEditorNode
{
private:
protected:
public:
	TSizeNode();
	TTreeNode *AddNode(NodeTypes N, bool Expand);
	TTreeNode *CopyNode(TTreeNode *S);
	TEditorNode *Duplicate();
	int GetImageIndex(TTreeView *DropTree, TTreeNode *DropNode);
	int GetSelectedIndex(TTreeView *DropTree, TTreeNode *DropNode);
	bool CanExcept(TTreeNode *N);

	WideString KitchenName;
	bool       Weighed;
	int		   Size_ID;
	int		   PalmID;

};
//---------------------------------------------------------------------------
class TCategoryGroupNode : public TEditorNode
{
private:
	bool Deleted;
protected:
public:
	TCategoryGroupNode();
	TTreeNode *AddNode(NodeTypes N, bool Expand);
	TTreeNode *CopyNode(TTreeNode *S);
	TEditorNode *Duplicate();
	int GetImageIndex(TTreeView *DropTree, TTreeNode *DropNode);
	int GetSelectedIndex(TTreeView *DropTree, TTreeNode *DropNode);
	bool CanExcept(TTreeNode *N);

	void SetDeleted(bool deleted);
	bool GetDeleted() { return Deleted; }
};
//---------------------------------------------------------------------------
class TCategoryNode : public TEditorNode
{
private:
	bool Deleted;
protected:
public:
	TCategoryNode();
	TTreeNode *AddNode(NodeTypes N, bool Expand);
	TTreeNode *CopyNode(TTreeNode *S);
	TEditorNode *Duplicate();
	int GetImageIndex(TTreeView *DropTree, TTreeNode *DropNode);
	int GetSelectedIndex(TTreeView *DropTree, TTreeNode *DropNode);
	bool CanExcept(TTreeNode *N);

	void SetDeleted(bool deleted);
	bool GetDeleted() { return Deleted; }
	AnsiString GLCode;
};
//---------------------------------------------------------------------------
class TServingCourseNode : public TEditorNode
{
private:
protected:
public:
	TServingCourseNode();
	TTreeNode *AddNode(NodeTypes N, bool Expand);
	TTreeNode *CopyNode(TTreeNode *S);
	TEditorNode *Duplicate();
	int GetImageIndex(TTreeView *DropTree, TTreeNode *DropNode);
	int GetSelectedIndex(TTreeView *DropTree, TTreeNode *DropNode);
	bool CanExcept(TTreeNode *N);
	void Enable();
	void Disable();

	WideString KitchenName;
	bool Deleted;
	TColor Colour;
	bool NewServingCourse; //add and delete in the same menu, ignore it
	bool Selectable;
	int DisplayOrder;   
};
//---------------------------------------------------------------------------
class TCourseNode : public TEditorNode
{
private:
protected:
public:
	TCourseNode();
	TTreeNode *AddNode(NodeTypes N, bool Expand);
	TTreeNode *CopyNode(TTreeNode *S);
	TEditorNode *Duplicate();
	void Enable();
	void Disable();
	int GetImageIndex(TTreeView *DropTree, TTreeNode *DropNode);
	int GetSelectedIndex(TTreeView *DropTree, TTreeNode *DropNode);
	bool CanExcept(TTreeNode *N);

	TTreeNode *GetOptionsNode();

	int        Location; // Palm/pos etc.
	WideString KitchenName;
	__int32    ServingCourseKey;
	AnsiString ServingCourse;
	bool       NoDefaultServingCourse;
};
//---------------------------------------------------------------------------
class TRecipe
{
public:
	/* Added so we don't have to alter existing code */
	TRecipe() { }

	/* Added because the compiler's default copy constructor
	* only does a straight binary copy. This isn't what we
	* want when our fields are objects, especially ones that
	* aren't ours. Ie, they may be dealing with dynamically
	* allocated memory and don't have their own copy mechanism.
	*/
	TRecipe(const TRecipe &init)
	{
		Key         = init.Key;
		StockCode   = init.StockCode;
		Description = init.Description;
		Unit        = init.Unit;
		Cost        = init.Cost;
		Quantity    = init.Quantity;
		Location    = init.Location;
		ConversionFactor = init.ConversionFactor;
		ReductionUnit = init.ReductionUnit;
	}

	/* So we can easily compare recipes */
	bool operator==(const TRecipe &rhs)
	{
		return StockCode == rhs.StockCode;
	}

	__int32    Key;
	AnsiString StockCode;
	AnsiString Description;
	AnsiString Unit;
	AnsiString ReductionUnit;
	Currency   Cost;
	double     Quantity;
	AnsiString Location; // Stock location to deduct from
	Currency ConversionFactor;
};
//---------------------------------------------------------------------------
struct TForcedOption {
	TForcedOption()
	: Name(""),
	ForcedOptionPK(0),
	OptionFK(0),
	ItemFK(0)
	{
	};

	TForcedOption(WideString inName,
	__int32 inForcedOptionPK,
	__int32 inOptionFK,
	__int32 inItemFK)
	: Name(inName),
	ForcedOptionPK(inForcedOptionPK),
	OptionFK(inOptionFK),
	ItemFK(inItemFK)
	{
	};

	WideString Name;
	__int32    ForcedOptionPK;
	__int32    OptionFK;
	__int32    ItemFK;
};
//---------------------------------------------------------------------------
class TItemNode : public TEditorNode
{
private:
protected:
public:
	TItemNode();
	__fastcall ~TItemNode();
	TTreeNode *AddNode(NodeTypes N, bool Expand);
	TTreeNode *CopyNode(TTreeNode *S);
	TEditorNode *Duplicate();
	void Enable();
	void Disable();
	int GetImageIndex(TTreeView *DropTree, TTreeNode *DropNode);
	int GetSelectedIndex(TTreeView *DropTree, TTreeNode *DropNode);
	bool CanExcept(TTreeNode *N);
	TColor Colour;
	TList *Condiments;
	std::map<int, std::map<AnsiString, TForcedOption> > ForcedOptions;
	TList *ForcedSides;
	bool ItemOnlySide;
	TRecipe DefaultStockRecipe;
	WideString KitchenName;
	TPrinterOptions PrinterOptions; // cww
    bool IsDragandDrop;
    int ItemIdentifier;
};
//---------------------------------------------------------------------------
class TForcedSide
{
public:
	__int32    Key;
	__int32    ItemKey;
	__int32    MasterItemKey;
	AnsiString CourseName;
	AnsiString ItemName;
	TItemNode *ItemNode;
	int GroupNumber;
	int MaxSelect;
	bool SideGroupSkip;
};


//---------------------------------------------------------------------------
class TItemSizeNode : public TEditorNode
{
private:
protected:
public:
	TItemSizeNode();
	__fastcall ~TItemSizeNode();
	TTreeNode *AddNode(NodeTypes N, bool Expand);
	TTreeNode *CopyNode(TTreeNode *S);
	TEditorNode *Duplicate();
	void Enable();
	void Disable();
	int GetImageIndex(TTreeView *DropTree, TTreeNode *DropNode);
	int GetSelectedIndex(TTreeView *DropTree, TTreeNode *DropNode);
	bool CanExcept(TTreeNode *N);
	Currency		Price;
	Currency		SpecialPrice;
	double			GST;
	Currency		TareWeight;
	bool			Free;
	double			PointsPercent;
	//	bool				UseRecipe;
	Currency		Cost;
	double			CostGST;
	Currency MaxRetailPrice;
	AnsiString		Barcode;
	TList		   *RecipeList;
	unsigned long	SetMenuMask;
	bool			SettingMenuMask;		// Forces RefreshItemSize to not clear the Set Menu Item checkbox as soon as it is checked.
	bool			SetMenuStandardItem;
	__int32   		CategoryKey;
	__property __int32 SizeKey = { read = GetSizeKey, write = SetSizeKey };
	AnsiString		Category;
	TStringList	   *CategoryList;
	std::vector<Menu::TNameAndKey> BCategoryList; // Breakdown Category List
	__int32		    ThirdPartyCodeKey;
	AnsiString		ThirdPartyCode;
	int				MemberPurchaseCount;
	double			MemberPurchaseDiscount;
	int				LocationPurchaseCount;
	double			LocationPurchaseDiscount;
	__int32         PLU;
	bool IsTaxExempt;
	bool HasServiceCharge;
	bool CanBePaidForUsingPoints;
	double AvailableQuantity;
	double DefaultQuantity;
	double WarningQuantity;
	bool DisableWhenCountReachesZero;
	int DefaultPatronCount;
    int RevenueCode;
    AnsiString RevenueCodeDescription;
	std::set<__int32> TaxProfileKeys;
	AnsiString KitchenName;
    int ItemSizeIdentifier; 

   std::map<int,Menu::TItemSizePriceLevel> ItemSizePriceLevels;
   std::map<int,Menu::TPriceLevelsName> PriceLevelsName;
   std::vector<Menu::TItemSizeTaxesPercentage> ItemSizeTaxPercent;
   Currency CostForPoints;

private:
	__int32 GetSizeKey() const { return SizeKey_; };
	void SetSizeKey(const __int32 newSizeKey) { SizeKey_ = newSizeKey; };

	__int32        SizeKey_;


};
//---------------------------------------------------------------------------
class TOptionsNode : public TEditorNode
{
private:
protected:
public:
	TOptionsNode();
	TTreeNode *AddNode(NodeTypes N, bool Expand);
	TTreeNode *CopyNode(TTreeNode *S);
	TEditorNode *Duplicate();
	void Enable();
	void Disable();
	int GetImageIndex(TTreeView *DropTree, TTreeNode *DropNode);
	int GetSelectedIndex(TTreeView *DropTree, TTreeNode *DropNode);
	bool CanExcept(TTreeNode *N);
};
//---------------------------------------------------------------------------
class TOptionNode : public TEditorNode
{
private:
protected:
public:
	TOptionNode();
	__fastcall ~TOptionNode();
	TTreeNode *AddNode(NodeTypes N, bool Expand);
	TTreeNode *CopyNode(TTreeNode *S);
	TEditorNode *Duplicate();
	void Enable();
	void Disable();
	int GetImageIndex(TTreeView *DropTree, TTreeNode *DropNode);
	int GetSelectedIndex(TTreeView *DropTree, TTreeNode *DropNode);
	bool CanExcept(TTreeNode *N);

	__int32 OptionKey;

	unsigned int CondimentsMask;	// Filled in when saved.
	int GroupNumber;					// 0 - No group. Between 0-8 (Will become 3 {or 4?} bits)
	bool PlusOption, MinusOption;
	WideString KitchenName;
	TPrinterOptions PrinterOptions;
	int MaxSelect;
	bool OptionGroupSkip;
};


class TAvailableMenuSize
{
public:
	__int32    Key;               // DB Key
	AnsiString LongDescription;
	WideString KitchenName;
    AnsiString HandheldName;
    AnsiString ReceiptName;
	bool       Weighed;
	int		   Size_ID;
	int		   PalmID;
};


//---------------------------------------------------------------------------
extern PACKAGE TfrmMenuEdit *frmMenuEdit;
//---------------------------------------------------------------------------
#endif

