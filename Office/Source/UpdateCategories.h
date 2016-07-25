//---------------------------------------------------------------------------

#ifndef UpdateCategoriesH
#define UpdateCategoriesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "VirtualTrees.hpp"
#include <CheckLst.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include <set>
#include <vector>
#include <map>
#include "ThemeMgr.hpp"
#include "MM_DBCore.h"
//---------------------------------------------------------------------------
const MAX_CATEGORY_GROUP_NAME_LENGTH	= 25;
const MAX_CATEGORY_NAME_LENGTH			= 30;
//---------------------------------------------------------------------------
namespace CategoryTranslation
{
//---------------------------------------------------------------------------
enum TCategoryType		{ ctUndefined, ctUnused, ctGroup, ctBreakdownGroup, ctCategory };
//---------------------------------------------------------------------------
class TfrmUpdateCategories;
class TCategoryTranslation : public TObject
{
private:
public:
	__fastcall TCategoryTranslation();
	virtual __fastcall ~TCategoryTranslation();


	AnsiString					OldCategoryName;
	AnsiString					NewCategoryName;

	TCategoryType				CategoryType;
	TCategoryTranslation		*Parent;	// The group that this category fits into (Only if of type ctCategory)
};
//---------------------------------------------------------------------------
class TCategoryGroup
{
private:
//	TList *Categories;
	std::vector <TCategoryTranslation *> Categories;
public:
	TCategoryTranslation		*Group;
	int CategoryCount();
	TCategoryTranslation		*GetCategory(int CategoryIndex);
	int AddCategory(TCategoryTranslation *Category);
};
//---------------------------------------------------------------------------
//class TCategoryTranslation;
//class TCategoryGroup;
class TUpdateCategories
{
friend TfrmUpdateCategories;

public:
	TUpdateCategories();
	~TUpdateCategories();

	void											AddOldCategory(AnsiString OldCategoryName);
	AnsiString									GetNewCategoryName(AnsiString OldCategoryName);
	int											GetCategoryPriority(AnsiString OldCategoryName);		// Lower number means higher priority

	TCategoryType								GetCategoryType(AnsiString OldCategoryName);

	int											GroupCount();
	TCategoryTranslation						*GetGroup(int GroupIndex);

	int											CategoryCount(int GroupIndex);
	TCategoryTranslation						*GetCategory(int GroupIndex, int CategoryIndex);

	void											AddCurrentGroup(AnsiString GroupName);
	void											AddCurrentCategory(AnsiString GroupName, AnsiString CategoryName);
	void											GetCurrentGroups(TStrings *Groups);
	void											GetCurrentCategories(AnsiString GroupName, TStrings *Groups);

	bool											Show();

private:
	TList											*CategoryTranslationList;
	TCategoryTranslation						*FindOldCategory(AnsiString OldCategoryName);
	TfrmUpdateCategories						*Form;

	TList											*CategoryGroups;
	void											AddGroup(TCategoryTranslation *CategoryGroup);
	void											AddCategory(AnsiString GroupName, TCategoryTranslation *CategoryGroup);

	typedef std::set<AnsiString>			TGroupSet;
	std::map<AnsiString, TGroupSet>		CurrentCategories;
};
//---------------------------------------------------------------------------
class TUpdateArchiveCategories : private TThread
{
public:
	TUpdateArchiveCategories(AnsiString DatabaseName, TProgressBar *ProgressBar=NULL);
	__fastcall ~TUpdateArchiveCategories();

	bool									StartUpdate();
	void									PauseUpdate();

	bool									Completed() { return fCompleted; }
protected:
	void __fastcall					Execute();
private:
	std::auto_ptr<TIBDatabase>		const	IBDatabase;
	Database::TDBTransaction		DBControl;

	Database::TcpIBSQL				sqlCategories;
	Database::TcpIBSQL				sqlArchiveCount;
	Database::TcpIBSQL				sqlArchive;
	Database::TcpIBSQL				sqlUpdateArchive;
	Database::TcpIBSQL				sqlFindCategory;
	Database::TcpIBSQL				sqlFindCategoryGroup;
	Database::TcpIBSQL				sqlAddCategory;
	Database::TcpIBSQL				sqlAddCategoryGroup;
	Database::TcpIBSQL				sqlSetCategory;

	TUpdateCategories					UpdateCategories;
	bool									Quit;

	class TCategoryGroup
	{
	public:
		AnsiString		GroupName;
		int				GroupKey;
	};
	class TCategory
	{
	public:
		AnsiString		CategoryName;
		int				CategoryKey;

		TCategoryGroup	Group;
	};
	typedef std::map<AnsiString, TCategory >	TCategories;
	typedef TCategories::iterator					TiCategories;
	TCategories											Categories;
	void													InitializeCategoryLookup();

	TProgressBar		*fProgressBar;
	int					ProgressPos;
	void __fastcall	UpdateGUI();

	bool fCompleted;
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TTreeData
{
public:
	TCategoryTranslation *CategoryTranslation;
	int ImageIndex;
	int SelectedIndex;
};
//---------------------------------------------------------------------------
class TfrmUpdateCategories : public TForm
{
__published:	// IDE-managed Components
	TPageControl *PageControl2;
	TTabSheet *tsSelectGroups;
	TCheckListBox *chlbOldCategories;
	TTabSheet *tsSelectCategories;
	TButton *btnNext1;
	TLabel *Label3;
	TLabel *Label4;
	TButton *btnNext2;
	TTreeView *tvCategories;
	TTreeView *tvGroupedCategories;
	TTabSheet *tsRename;
	TVirtualStringTree *vtvRename;
	TButton *Button1;
	TLabel *Label5;
	TEdit *edNewName;
	TButton *Button2;
	TComboBox *cbNewName;
	TLabel *Label8;
	TButton *Button3;
	TBitBtn *btnCommit;
	TButton *Button4;
	TLabel *Label9;
	TLabel *Label10;
	void __fastcall btnNext1Click(TObject *Sender);
	void __fastcall tvGroupedCategoriesMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall tvGroupedCategoriesDragOver(TObject *Sender,
          TObject *Source, int X, int Y, TDragState State, bool &Accept);
	void __fastcall tvGroupedCategoriesDragDrop(TObject *Sender,
          TObject *Source, int X, int Y);
	void __fastcall tvCategoriesDragDrop(TObject *Sender, TObject *Source,
          int X, int Y);
	void __fastcall tvCategoriesDragOver(TObject *Sender, TObject *Source,
          int X, int Y, TDragState State, bool &Accept);
	void __fastcall btnNext2Click(TObject *Sender);
	void __fastcall vtvRenameAfterPaint(TBaseVirtualTree *Sender,
          TCanvas *TargetCanvas);
	void __fastcall vtvRenameBeforePaint(TBaseVirtualTree *Sender,
          TCanvas *TargetCanvas);
	void __fastcall vtvRenameColumnDblClick(TBaseVirtualTree *Sender,
          TColumnIndex Column, TShiftState Shift);
	void __fastcall vtvRenameCreateEditor(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, IVTEditLink *EditLink);
	void __fastcall vtvRenameEdited(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column);
	void __fastcall vtvRenameEditing(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, bool &Allowed);
	void __fastcall vtvRenameFocusChanged(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column);
	void __fastcall vtvRenameFocusChanging(TBaseVirtualTree *Sender,
          PVirtualNode OldNode, PVirtualNode NewNode,
          TColumnIndex OldColumn, TColumnIndex NewColumn, bool &Allowed);
	void __fastcall vtvRenameGetImageIndex(TBaseVirtualTree *Sender,
          PVirtualNode Node, TVTImageKind Kind, TColumnIndex Column,
          bool &Ghosted, int &ImageIndex);
	void __fastcall vtvRenameGetText(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
          WideString &CellText);
	void __fastcall vtvRenameKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall edNewNameKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall edNewNameKeyPress(TObject *Sender, char &Key);
	void __fastcall edNewNameKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall vtvRenameExit(TObject *Sender);
	void __fastcall vtvRenameEnter(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall edNewNameChange(TObject *Sender);
	void __fastcall cbNewNameChange(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmUpdateCategories(TComponent* Owner);
	TUpdateCategories *UpdateCategories;
};
//---------------------------------------------------------------------------
} // namespace CategoryTranslation
//---------------------------------------------------------------------------
#endif
