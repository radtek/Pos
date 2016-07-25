//---------------------------------------------------------------------------
#ifndef MenuManagerH
#define MenuManagerH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Dialogs.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include "TouchGrid.h"
#include <vector>

#include "MM_DBCore.h"

#include "DeviceRealTerminal.h"
//---------------------------------------------------------------------------
class TfrmMenuManager : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel2;
	TPanel *pnlCourses;
	TTouchBtn *btnCourseUp;
	TTouchBtn *btnCourseDown;
	TPanel *Panel4;
	TPanel *Panel3;
	TLabel *lbeMenuName;
   TScrollBox *sbCourses;
	TTouchBtn *TouchButton;
   TTimer *btnTimer;
   TOpenDialog *OpenDialog1;
	TTouchBtn *TouchBtn1;
	TTouchBtn *TouchBtn2;
	TTouchBtn *TouchBtn3;
	TTouchBtn *btnDrinksDishes;
	TTouchGrid *menuItemGrid;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall TouchButtonMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall btnCloseClick(TObject *Sender);
   void __fastcall sbDishesClick(TObject *Sender);
   void __fastcall btnTimerTimer(TObject *Sender);
   void __fastcall btnDrinksDishesMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall btnDrinksDishesMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall btnRemoveMenuClick(TObject *Sender);
   void __fastcall tbtnAddMenuClick(TObject *Sender);
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall pnlCoursesResize(TObject *Sender);
   void __fastcall btnCourseUpMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall btnCourseDownMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall menuItemGridMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift,
          TGridButton *GridButton);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
   BEGIN_MESSAGE_MAP
     MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
   END_MESSAGE_MAP(TComponent)

private:	// User declarations
	__fastcall TfrmMenuManager(TComponent* Owner,TManagerMenusPOS *inManagerMenus,Database::TDBControl &inIBDatabase);

	static TForm *WinOwner;
	void __fastcall CreateParams(Controls::TCreateParams &params);

	void __fastcall CheckBoxClick(TObject *Sender);
	long ShowingMenu;
	AnsiString MenuName;
	TTouchBtn *CurrentCourseBtn;
	void RefreshCourseButtons();
	void ShowItems();
	Database::TDBControl &IBDatabase;
	TManagerMenusPOS *ManagerMenus;

    const TColor CL_UNSELECTED_COURSE;
    const TColor CL_SELECTED_COURSE;

public:		// User declarations
	TfrmMenuManager static *Create(TForm* Owner,TManagerMenusPOS *inManagerMenus,Database::TDBControl &inIBDatabase);
    void RedrawMenus();
    bool MenuEdited;
};
//---------------------------------------------------------------------------
#endif
