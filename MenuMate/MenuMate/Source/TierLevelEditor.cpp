//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "MMTouchKeyboard.h"
#include "MMTouchNumpad.h"
#include "TierLevelEditor.h"
#include "MMMessageBox.h"
#include "ManagerTierLevel.h"
#include "GlobalSettings.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmTierLevelEditor *frmTierLevelEditor;
//---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
__fastcall TfrmTierLevelEditor::TfrmTierLevelEditor(TComponent* Owner, Database::TDBControl &inDBControl) :
                                TZForm(Owner), DBControl(inDBControl)
{
   Reset();
}
// ---------------------------------------------------------------------------

__fastcall TfrmTierLevelEditor::~TfrmTierLevelEditor()
{

}

// ---------------------------------------------------------------------------
void __fastcall TfrmTierLevelEditor::FormShow(TObject *Sender)
{
   if(IsEditMode)
   {
     TierLevel->TierKey = TierId;
     GetTierLevelInformation();
     btnTierName->Caption = "Tier Name\r" + TierLevel->Name;
     btnTierLevel->Caption = "Level\r" + IntToStr(TierLevel->Level);
     btnPointsRequired->Caption = "Points Required\r" + IntToStr(TierLevel->PointRequired);
     btnBdayBonus->Caption = "Birthday Bonus\r" + IntToStr(TierLevel->BirthDayBonus);
     btnPricedPoints->Caption = CurrToStr(TierLevel->PointRedemRate);
     btnWeighedGrams->Caption = IntToStr(TierLevel->WeighedRedemWeight);
     btnWeighedPoints->Caption = IntToStr(TierLevel->WeighedRedemPoint);
     cbEmailHeadOffice->Checked = TierLevel->SendMailToHeadOffice;
     cbEmailMember->Checked =  TierLevel->SendMailToMember;
     cbChangeCard->Checked =  TierLevel->ChangeCard;
     cbAllowEarntRedemption->Checked = TierLevel->AllowEarntRedemption;
   }
   else
   {
     cbAllowEarntRedemption->Checked = true;
   }
}

// ---------------------------------------------------------------------------
void TfrmTierLevelEditor::Reset()
{
  TierLevel = new TTierLevel;
  TierLevel->Name="";
  TierLevel->Level = 0;
  TierLevel->PointRequired = 0;
  TierLevel->BirthDayBonus = 0;
  TierLevel->PointRedemRate = 0;
  TierLevel->WeighedRedemWeight = 0;
  TierLevel->WeighedRedemPoint = 0;
  TierLevel->SendMailToHeadOffice = false;
  TierLevel->SendMailToMember = false;
  TierLevel->ChangeCard = false;
  TierLevel->AllowEarntRedemption = true;
  TierLevel->CloudId = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmTierLevelEditor::btnCancelMouseClick(TObject *Sender)
{
  ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TfrmTierLevelEditor::btnOkMouseClick(TObject *Sender)
{
  AnsiString Message = "";
  bool isTierLevelexist = CheckTierLevelExist();
  bool isValidData = CheckTierLevelData(Message);
  if(!isTierLevelexist && isValidData)
  {
      TierLevel->SendMailToHeadOffice = cbEmailHeadOffice->Checked;
      TierLevel->SendMailToMember = cbEmailMember->Checked;
      TierLevel->ChangeCard = cbChangeCard->Checked;
      TierLevel->AllowEarntRedemption = cbAllowEarntRedemption->Checked;
      if(IsEditMode)
       {
         UpdateTierLevel();
       }
      else
       {
         AddTierLevel();
         if (TGlobalSettings::Instance().LoyaltyMateEnabled)
         {
           TManagerTierLevel ManagerTierLevel;
           ManagerTierLevel.createTierOnLoyaltyMate(TierLevel);
         }

       }
      ModalResult = mrOk;
  }

  if(isTierLevelexist)
   {
      MessageBox("Tier Level Already Exist.", "Warning", MB_ICONINFORMATION + MB_OK);
   }
  if(!isValidData)
   {
      MessageBox(Message, "Warning", MB_ICONINFORMATION + MB_OK);
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmTierLevelEditor::btnTierNameMouseClick(TObject *Sender)
{
    std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
    frmTouchKeyboard->MaxLength = 15;
    frmTouchKeyboard->AllowCarriageReturn = false;
    frmTouchKeyboard->StartWithShiftDown = false;
    frmTouchKeyboard->KeyboardText = TierLevel->Name;
    frmTouchKeyboard->Caption = "Enter Tier Name";
    if (frmTouchKeyboard->ShowModal() == mrOk)
    {
        TierLevel->Name = frmTouchKeyboard->KeyboardText;
        btnTierName->Caption = "Tier Name\r" + TierLevel->Name;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmTierLevelEditor::btnTierLevelMouseClick(TObject *Sender)
{
    TierLevel->Level = DisplayIntegerNumpad(TierLevel->Level,"Enter Tier level");
    btnTierLevel->Caption = "Level\r" + IntToStr(TierLevel->Level);
}
//---------------------------------------------------------------------------

void __fastcall TfrmTierLevelEditor::btnPointsRequiredMouseClick(TObject *Sender)
{
    TierLevel->PointRequired = DisplayIntegerNumpad(TierLevel->PointRequired,"Enter Points Required") ;
    btnPointsRequired->Caption = "Points Required\r" + IntToStr(TierLevel->PointRequired);
}
//---------------------------------------------------------------------------

void __fastcall TfrmTierLevelEditor::btnBdayBonusMouseClick(TObject *Sender)
{
    TierLevel->BirthDayBonus = DisplayIntegerNumpad(TierLevel->BirthDayBonus,"Enter Birthday Bonus");
    btnBdayBonus->Caption = "Birthday Bonus\r" + IntToStr(TierLevel->BirthDayBonus);
}
//---------------------------------------------------------------------------

void __fastcall TfrmTierLevelEditor::btnPricedPointsMouseClick(TObject *Sender)
{
    TierLevel->PointRedemRate = DisplayCurrencyNumpad(TierLevel->PointRedemRate,"Enter Point Price");
    btnPricedPoints->Caption = CurrToStr(TierLevel->PointRedemRate);
}
//---------------------------------------------------------------------------

void __fastcall TfrmTierLevelEditor::btnWeighedGramsMouseClick(TObject *Sender)
{
    TierLevel->WeighedRedemWeight = DisplayIntegerNumpad(TierLevel->WeighedRedemWeight,"Enter Weight");
    btnWeighedGrams->Caption = IntToStr(TierLevel->WeighedRedemWeight);
}
//---------------------------------------------------------------------------

void __fastcall TfrmTierLevelEditor::btnWeighedPointsMouseClick(TObject *Sender)
{
    TierLevel->WeighedRedemPoint = DisplayIntegerNumpad(TierLevel->WeighedRedemPoint,"Enter Points");
    btnWeighedPoints->Caption = IntToStr(TierLevel->WeighedRedemPoint);
}
//---------------------------------------------------------------------------
//Display numpad to ender interger data
int TfrmTierLevelEditor::DisplayIntegerNumpad(int initialValue, AnsiString caption)
{
    int retVal = 0;
    std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
    frmTouchNumpad->Caption = caption;
    frmTouchNumpad->btnOk->Visible = true;
    frmTouchNumpad->Mode = pmNumber;
    frmTouchNumpad->INTInitial = initialValue;
    if (frmTouchNumpad->ShowModal() == mrOk)
    {
        retVal = frmTouchNumpad->INTResult;
    }
    return retVal;
}
//---------------------------------------------------------------------------

//Display numpad to ender decimal data
Currency TfrmTierLevelEditor::DisplayCurrencyNumpad(Currency initialValue, AnsiString caption)
{
    Currency retVal = 0;
    std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
    frmTouchNumpad->Caption = caption;
    frmTouchNumpad->btnOk->Visible = true;
    frmTouchNumpad->Mode = pmCurrency;
    frmTouchNumpad->CURInitial = initialValue;
    if (frmTouchNumpad->ShowModal() == mrOk)
    {
        retVal = frmTouchNumpad->CURResult;
    }
    return retVal;
}
//---------------------------------------------------------------------------
//Add save tier level to database
void TfrmTierLevelEditor::AddTierLevel()
{
    Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();
    TDBTierLevel::AddTierLevel(DBTransaction,TierLevel);
    DBTransaction.Commit();
}
//---------------------------------------------------------------------------
//Update tier level data
void TfrmTierLevelEditor::UpdateTierLevel()
{
    Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();
    TDBTierLevel::UpdateTierLevel(DBTransaction,TierLevel);
    DBTransaction.Commit();
    if (TGlobalSettings::Instance().LoyaltyMateEnabled)
     {
        TManagerTierLevel ManagerTierLevel;
        ManagerTierLevel.updateTierOnLoyaltyMate(TierLevel);
     }
}
//---------------------------------------------------------------------------
//Get Tier Level Information
void TfrmTierLevelEditor::GetTierLevelInformation()
{
    Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();
    TDBTierLevel::GetTierLevelInformation(DBTransaction,TierLevel);
    DBTransaction.Commit();
}
//---------------------------------------------------------------------------
//Check that tier level already exist
bool TfrmTierLevelEditor::CheckTierLevelExist()
{
    bool retVal = true;
    Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();
    retVal = TDBTierLevel::CheckTierLevelExist(DBTransaction,TierLevel,IsEditMode);
    DBTransaction.Commit();
    return retVal;
}
//---------------------------------------------------------------------------
//Check that user have entered valid data
bool TfrmTierLevelEditor::CheckTierLevelData(AnsiString &message)
{
    bool retVal = true;
    if(TierLevel->Name =="")
    {
      message = "Please enter tier name.";
      return false;
    }
   if(TierLevel->Level <= 0)
    {
       message = "Please enter tier level.";
      return false;
    }
    if(TierLevel->PointRedemRate <= 0)
    {
       message = "Please enter point ratio for priced product.";
      return false;
    }
    if(TierLevel->WeighedRedemWeight <= 0)
    {
       message = "Please enter weight for weighed product.";
      return false;
    }
    if(TierLevel->WeighedRedemPoint <= 0)
    {
       message = "Please enter points for weighed product.";
      return false;
    }
    return true;
}

