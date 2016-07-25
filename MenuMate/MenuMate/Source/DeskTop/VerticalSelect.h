//---------------------------------------------------------------------------

#ifndef VerticalSelectH
#define VerticalSelectH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "touchcontrols.h"
#include "touchgrid.h"
#include "TouchControls.h"
#include "TouchGrid.h"
#include <map>
#include "ZForm.h" 
//---------------------------------------------------------------------------
class TVerticalSelection
{
private:
public:
	TVerticalSelection() {
           IsDisabled = false;
        };

	TVerticalSelection(const UnicodeString &title, const bool close_selection)
	  : Title(title),
       CloseSelection(close_selection)
	{
          IsDisabled = false;
	};

	void
	SetButtonColors(
		TColor unlatched,
		TColor latched,
		TColor disabled = clBtnFace)
	{
		Properties["Color"]         = unlatched;
		Properties["DisabledColor"] = disabled;
		Properties["LatchedColor"]  = latched;
	};

	~TVerticalSelection() { };

   UnicodeString Title;
   bool CloseSelection;
   bool IsDisabled;
   std::map<UnicodeString,Variant> Properties;
};

class TfrmVerticalSelect : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
   TTouchGrid *tgridSelection;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall tgridSelectionMouseClick(TObject *Sender,
          TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
private:
	__fastcall TfrmVerticalSelect(TComponent* Owner);
	void UpdateSelectionList();	// User declarations
	TColor GetItemButtonFontColour(TColor inColor);
public:		// User declarations

	bool __fastcall GetFirstSelectedItem(TVerticalSelection &SelectedItem);
   std::vector<TVerticalSelection> Items;
   TVerticalSelection Selected;
};



//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif
