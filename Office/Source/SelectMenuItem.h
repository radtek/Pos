//---------------------------------------------------------------------------

#ifndef SelectMenuItemH
#define SelectMenuItemH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "VirtualTrees.hpp"
#include <vector>
#include <map>
#include "VirtualTrees.hpp"
//---------------------------------------------------------------------------
class TfrmSelectMenuItem : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel10;
	TBitBtn *btnOk;
	TBitBtn *btnFind;
	TPanel *Panel9;
	TLabel *lbeTitle;
	TBitBtn *btnCancel;
	TVirtualStringTree *vtvItems;
	void __fastcall vtvItemsGetImageIndex(TBaseVirtualTree *Sender,
          PVirtualNode Node, TVTImageKind Kind, TColumnIndex Column,
          bool &Ghosted, int &ImageIndex);
	void __fastcall vtvItemsGetText(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
          WideString &CellText);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmSelectMenuItem(TComponent* Owner);
};
//---------------------------------------------------------------------------
class TSelectMenuItem
{
public:		// User declarations
	TSelectMenuItem();

	class TData
	{
	public:
		AnsiString	Name;
		void *		Data;
	};
	typedef class TData TCourse;
	typedef class TData TItem;

	int AddCourse(TCourse Course);
	int AddItem(int CourseIndex, TItem);
	void Clear();

	bool Execute();

	int SelectedCount();
	void *Selected(int Index);
private:
	std::vector<TData> fCourses;
	std::map<int, std::vector<TData> > fItems;

	std::vector<TData> fSelected;
};
//---------------------------------------------------------------------------
#endif
