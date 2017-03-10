//---------------------------------------------------------------------------

#ifndef GenericGroupGUIH
#define GenericGroupGUIH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"
//---------------------------------------------------------------------------
class TfrmGenericGroupGUI : public TZForm
{
  friend TZForm;
__published:	// IDE-managed Components
	TPanel *pnlMajor;
	TPanel *pnlGroups;
	TStringGrid *tgridGroups;
	TPanel *pnlMinor;
	TTouchBtn *btnClose;
	TTouchBtn *btnAdd;
	TTouchBtn *btnEdit;
	TTouchBtn *btnDelete;
	TTouchBtn *btnAssign;
	TPanel *Panel3;

    void __fastcall btnCloseMouseClick(TObject *Sender);
    void __fastcall btnAddMouseClick(TObject *Sender);
    void __fastcall btnEditMouseClick(TObject *Sender);
    void __fastcall btnDeleteMouseClick(TObject *Sender);
    void __fastcall btnAssignMouseClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall tgridGroupsClick(TObject *Sender);

public:		// User declarations

    virtual void AddGroup(){};
    virtual void EditGroup(){};
    virtual void DeleteGroup(){};
    virtual void AssignGroups(){};
    virtual void DisplayGroups(){};
    virtual void ItemSelected(){};
    void ButtonStatus(bool Status);
    __fastcall TfrmGenericGroupGUI(TComponent* Owner);
    _fastcall ~TfrmGenericGroupGUI();
};

#endif
