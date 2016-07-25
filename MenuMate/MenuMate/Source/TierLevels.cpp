// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TierLevels.h"
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "DBTierLevel.h"
#include "MMTouchNumpad.h"
#include "MMTouchKeyboard.h"
#include "MMMessageBox.h"
#include "DeviceRealTerminal.h"
#include "TierLevelEditor.h"
#include "ManagerTierLevel.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma resource "*.dfm"
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
__fastcall TfrmTierLevel::TfrmTierLevel(TComponent* Owner, Database::TDBControl &inDBControl) : TZForm(Owner), DBControl(inDBControl)
{
}

__fastcall TfrmTierLevel::~TfrmTierLevel()
{
}
// ---------------------------------------------------------------------------

void __fastcall TfrmTierLevel::FormShow(TObject *Sender)
{
	FormResize(this);
    SetGridColors(tgridSelection);
    PopulateTierLevels();
    btnSync->Visible = TGlobalSettings::Instance().LoyaltyMateEnabled;
}
// ---------------------------------------------------------------------------

void __fastcall TfrmTierLevel::FormResize(TObject *Sender)
{
    /*if (Screen->Width != Tag)
    {
            int temp = Tag;
            Tag = Screen->Width;
            if ((double)Screen->Width / Screen->Height < 1.4)
            {
                    ScaleBy(Screen->Width, temp);
            }
    }
    pnlMajor->Width = pnlMinor->Left -this->BorderWidth; */
}
// ---------------------------------------------------------------------------

void __fastcall TfrmTierLevel::btnAddMouseClick(TObject *Sender)
{
    std::auto_ptr<TfrmTierLevelEditor>(frmTierLevelEditor)(TfrmTierLevelEditor::Create<TfrmTierLevelEditor>(this,TDeviceRealTerminal::Instance().DBControl));
    frmTierLevelEditor->IsEditMode = false;
    frmTierLevelEditor->ShowModal();
    PopulateTierLevels();
}
// --------------------------------------------------------------------------

void __fastcall TfrmTierLevel::btnEditMouseClick(TObject *Sender)
{
  if(SelectedTierKey !=0)
   {
        std::auto_ptr<TfrmTierLevelEditor>(frmTierLevelEditor)(TfrmTierLevelEditor::Create<TfrmTierLevelEditor>(this,TDeviceRealTerminal::Instance().DBControl));
        frmTierLevelEditor->IsEditMode = true;
        frmTierLevelEditor->TierId = SelectedTierKey;
        frmTierLevelEditor->ShowModal();
        PopulateTierLevels();
   }
   else
   {
     MessageBox("Select a Tier Level", "Select a Tier Level", MB_ICONINFORMATION + MB_OK);
   }
}

//Load all the tier levels to the grid
// ---------------------------------------------------------------------------
void TfrmTierLevel::PopulateTierLevels()
{
    std::auto_ptr<TStringList>List(new TStringList);
    Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();
    TDBTierLevel::GetTierLevels(DBTransaction,List.get());

    tgridSelection->ColCount = 0;
    tgridSelection->RowCount = 0;

    tgridSelection->VertScrollDistance = (tgridSelection->Height / 9) * 6;
    tgridSelection->DefaultRowHeight = tgridSelection->Height / 9;

    tgridSelection->ColCount = 0;
    tgridSelection->RowCount = List->Count;
    for (int i = 0; i < List->Count; i++)
	{
          tgridSelection->Buttons[i][0]->Caption = List->Strings[i];
          tgridSelection->Buttons[i][0]->Tag = (int)List->Objects[i];
          if(SelectedTierKey == (int)List->Objects[i])
           {
             tgridSelection->Buttons[i][0]->Latched = true;
           }
	}
    DBTransaction.Commit();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmTierLevel::btnDeleteMouseClick(TObject *Sender)
{
  DeleteTier();
  PopulateTierLevels();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmTierLevel::btnCloseMouseClick(TObject *Sender)
{
	ModalResult = mrOk;
}
// ---------------------------------------------------------------------------

void __fastcall TfrmTierLevel::tgridSelectionMouseClick(TObject *Sender, TMouseButton Button,
          TShiftState Shift, TGridButton *GridButton)
{
    SelectedTierKey = GridButton->Tag;
}
//---------------------------------------------------------------------------
//Delete the tier level
void TfrmTierLevel::DeleteTier()
{
  if(SelectedTierKey !=0)
  {
    if(MessageBox("Do you want to delete selected tier level?", "Warning", MB_ICONWARNING + MB_OKCANCEL) == ID_OK)
     {
     	Database::TDBTransaction DBTransaction(DBControl);
	    DBTransaction.StartTransaction();
        TTierLevel* tierLevel  = new TTierLevel;
        tierLevel->TierKey = SelectedTierKey;
        TDBTierLevel::GetTierLevelInformation(DBTransaction,tierLevel);
        TDBTierLevel::DeleteTierLevel(DBTransaction,SelectedTierKey);
        DBTransaction.Commit();
        if (TGlobalSettings::Instance().LoyaltyMateEnabled)
         {
            TManagerTierLevel ManagerTierLevel;
            ManagerTierLevel.DeleteTierLevel(tierLevel);
         }
     }
  }
  else
  {
    MessageBox("Select a Tier Level", "Select a Tier Level", MB_ICONINFORMATION + MB_OK);
  }
}
void __fastcall TfrmTierLevel::btnSyncMouseClick(TObject *Sender)
{
  TManagerTierLevel ManagerTierLevel;
  ManagerTierLevel.SyncTierLevels();
  PopulateTierLevels();
}
//---------------------------------------------------------------------------

