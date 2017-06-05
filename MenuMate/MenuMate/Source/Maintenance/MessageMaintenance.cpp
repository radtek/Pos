//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MessageMaintenance.h"
#include "Main.h"
#include "Message.h"
#include "MMTouchKeyboard.h"
#include "MMMessageBox.h"
#include "RunManager.h"
#include "MessageManager.h"
#include "ManagerPatron.h"
#include "MMLogging.h"
#include "ManagerFloat.h"
#include "MMTouchNumpad.h"
#include "DBDenominations.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
//TfrmMessageMaintenance *frmMessageMaintenance;
//---------------------------------------------------------------------------
__fastcall TfrmMessageMaintenance::TfrmMessageMaintenance(TComponent* Owner,Database::TDBControl &inDBControl)
	: TZForm(Owner), DBControl(inDBControl)
{
	Color = clBlack;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMessageMaintenance::FormShow(TObject *Sender)
{
    switch(MessageType)
    {
        case eMessage:
        {
            pnlLabel->Caption = "Message List";
            break;
        }
        case eCancelReason:
        {
            pnlLabel->Caption = "Cancel Reasons";
            break;
        }
        case eCreditReason:
        {
            pnlLabel->Caption = "Refund Reasons";
            break;
        }
        case eRunProgram:
        {
            pnlLabel->Caption = "Configured Programs.";
            break;
        }
        case ePatronTypes:
        {
            pnlLabel->Caption = "Configure Patron Types.";
            break;
        }
        case eSkimRefloat:
        {
            pnlLabel->Caption = "Deposit/Withdrawal Reasons";
            break;
        }
        case eWriteOff:
        {
            pnlLabel->Caption = "Writeoff Reasons";
            break;
        }
        case eCustomerTypes:
        {
            pnlLabel->Caption = "Customer Order Type Reasons";
            break;
        }
        case eCashDrawer:
        {
            pnlLabel->Caption = "Cash Drawer Reason";
            break;
        }
        case eCashDenomination:
        {
            pnlLabel->Caption = "Cash Denomination";
            break;
        }
    }

	this->Caption = pnlLabel->Caption;

	FormResize(NULL);
	ShowMessages();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMessageMaintenance::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMessageMaintenance::FormResize(TObject *Sender)
{
	/*ClientWidth = Panel1->Width + (Panel1->Left * 2);
	ClientHeight = Panel1->Height + (Panel1->Top * 2);
	Left = (Screen->Width - Width) / 2;
	Top = (Screen->Height - Height) / 2;*/
}
//---------------------------------------------------------------------------
void TfrmMessageMaintenance::ShowMessages()
{
	sgDisplay->ColWidths[0] = sgDisplay->ClientWidth * 1 / 3;
	sgDisplay->ColWidths[1] = sgDisplay->ClientWidth - sgDisplay->ColWidths[1] - 1;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
    switch(MessageType)
    {
      case eRunProgram:
      {
        ManagerRun->LoadMessages(DBTransaction,sgDisplay,MessageType);
        break;
      }
      case ePatronTypes:
      {
        TManagerPatron::Instance().GetPatronTypes(DBTransaction,sgDisplay);
        break;
      }
      case eCashDenomination:
      {
        LoadDenominations(DBTransaction);
        break;
      }
      default:
      {
         ManagerMessage->LoadMessages(DBTransaction,sgDisplay,MessageType);
         break;
      }

    }
	DBTransaction.Commit();
	sgDisplay->ColWidths[0] = sgDisplay->ClientWidth * 1 / 3;
	sgDisplay->ColWidths[1] = sgDisplay->ClientWidth - sgDisplay->ColWidths[1] - 1;

}
//---------------------------------------------------------------------------
void __fastcall TfrmMessageMaintenance::imgExitClick(TObject *Sender)
{
    if(CheckDefaultPatronTypes())
    {
	    Close();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMessageMaintenance::btnAddMessageClick(TObject *Sender)
{

    try
    {
        AnsiString CurrentCaption = "Enter Button Title";
        AnsiString CurrentMessage = "Enter Message";
        GetHeaders(CurrentCaption,CurrentMessage);
        std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
        frmTouchKeyboard->MaxLength = 15;
        frmTouchKeyboard->AllowCarriageReturn = false;
        frmTouchKeyboard->StartWithShiftDown = true;
        frmTouchKeyboard->MustHaveValue = true;
        frmTouchKeyboard->KeyboardText = "";
        frmTouchKeyboard->Caption = CurrentCaption;

        if (frmTouchKeyboard->ShowModal() == mrOk)
        {
            AnsiString ButtonTitle = frmTouchKeyboard->KeyboardText;
            Database::TDBTransaction DBTransaction(DBControl);
            DBTransaction.StartTransaction();
            if(MessageType == ePatronTypes)
            {
                bool Default = false;
                if(MessageBox("Do you want this to be the Default Patron Type?",
                                    " Default Patron Type?",MB_YESNO + MB_ICONQUESTION) == IDYES)
                {
                    Default = true;
                }
                TPatronType PatronType;
                PatronType.Name = ButtonTitle;
                PatronType.Default = Default;
                TManagerPatron::Instance().SetPatronType(DBTransaction,0,PatronType);
            }
            else
            {
                std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
                frmTouchKeyboard->MaxLength = 200;
                frmTouchKeyboard->AllowCarriageReturn = false;
                frmTouchKeyboard->StartWithShiftDown = true;
                frmTouchKeyboard->KeyboardText = "";
                if(MessageType == eCashDrawer)
                {
                   frmTouchKeyboard->MustHaveValue = true;
                   frmTouchKeyboard->MaxLength = 39;
                }
                frmTouchKeyboard->Caption = CurrentMessage;
                if(MessageType != eCashDenomination)
                {
                    if (frmTouchKeyboard->ShowModal() == mrOk)
                    {
                        if (MessageType == eRunProgram)
                        {
                            CurrentCaption = "Enter Program Button Title";
                            CurrentMessage = "Enter Run Command";
                            ManagerRun->Add(DBTransaction,ButtonTitle,frmTouchKeyboard->KeyboardText,ManagerRun->GetCount(DBTransaction,MessageType)+1,0);
                        }
                        else
                        {
                            ManagerMessage->Add(DBTransaction,ButtonTitle,frmTouchKeyboard->KeyboardText,ManagerMessage->GetCount(DBTransaction,MessageType)+1,MessageType);
                        }
                    }
                }
                else
                {
                   Currency denomination = 0.0;
                   denomination = GetDenominationValue(DBTransaction, denomination);
                   SaveDenomination(DBTransaction,0,ButtonTitle,denomination);
                }
            }
            DBTransaction.Commit();
        }
        ShowMessages();
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMessageMaintenance::btnEditMessageClick(TObject *Sender)
{
	try
	{
		if((int)sgDisplay->Objects[0][sgDisplay->Row] == 0)
		{
			MessageBox("There is nothing to Edit.", "Warning", MB_ICONWARNING + MB_OK);
		}
		else
		{
			AnsiString CurrentCaption = "Enter Button Title";
			AnsiString CurrentMessage = "Enter Message";
			GetHeaders(CurrentCaption,CurrentMessage);
			Database::TDBTransaction DBTransaction(DBControl);
			DBTransaction.StartTransaction();

			if(MessageType == ePatronTypes)
			{
				int Key = (int)sgDisplay->Objects[0][sgDisplay->Row];
				TPatronType PatronType = TManagerPatron::Instance().GetPatronTypes(DBTransaction,Key);
				std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
				frmTouchKeyboard->MaxLength = 15;
				frmTouchKeyboard->AllowCarriageReturn = false;
				frmTouchKeyboard->StartWithShiftDown = true;
				frmTouchKeyboard->MustHaveValue = true;				
				frmTouchKeyboard->KeyboardText = PatronType.Name;
				frmTouchKeyboard->Caption = "Patron Type";
				if (frmTouchKeyboard->ShowModal() == mrOk)
				{
					PatronType.Name = frmTouchKeyboard->KeyboardText;
					if(!PatronType.Default)
					{
						if(MessageBox("Do you want this to be the Default Patron Type?",
											" Default Patron Type?",MB_YESNO + MB_ICONQUESTION) == IDYES)
						{
							PatronType.Default = true;
						}
					}
					TManagerPatron::Instance().SetPatronType(DBTransaction,Key,PatronType);
				}
			}
			else
			{
				TManagerInterface *Manager = NULL;
				if (MessageType == eRunProgram)
				{
					Manager = ManagerRun;
				}
				else
				{
					Manager = ManagerMessage;
				}

	      	    std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
				frmTouchKeyboard->MaxLength = 15;
				frmTouchKeyboard->AllowCarriageReturn = false;
				frmTouchKeyboard->StartWithShiftDown = true;
                if(MessageType == eCashDrawer)
                {
                   frmTouchKeyboard->MustHaveValue = true;
                }
                AnsiString title = "";
                if(MessageType == eCashDenomination)
                {
                  title = TDBDenominations::GetDenominationTitle(DBTransaction,(int)sgDisplay->Objects[0][sgDisplay->Row]);
                }
                else
                {
                   title = Manager->GetTitle(DBTransaction,(int)sgDisplay->Objects[0][sgDisplay->Row]);
                }

				frmTouchKeyboard->KeyboardText = title;
				frmTouchKeyboard->Caption = CurrentCaption;
                if (frmTouchKeyboard->ShowModal() == mrOk)
                {
                    Manager->SetTitle(DBTransaction,(int)sgDisplay->Objects[0][sgDisplay->Row],frmTouchKeyboard->KeyboardText);
                    frmTouchKeyboard->MaxLength = 200;
                    frmTouchKeyboard->AllowCarriageReturn = false;
                    frmTouchKeyboard->StartWithShiftDown = true;
                    if(MessageType == eCashDrawer)
                    {
                       frmTouchKeyboard->MustHaveValue = true;
                       frmTouchKeyboard->MaxLength = 39;
                    }
                    if(MessageType != eCashDenomination)
                    {
                        frmTouchKeyboard->KeyboardText =  Manager->GetContent(DBTransaction,(int)sgDisplay->Objects[0][sgDisplay->Row]);
                        frmTouchKeyboard->Caption = CurrentMessage;
                        if (frmTouchKeyboard->ShowModal() == mrOk)
                        {
                            Manager->SetContent(DBTransaction,(int)sgDisplay->Objects[0][sgDisplay->Row],frmTouchKeyboard->KeyboardText);
                        }
                    }
                    else
                    {
                       Currency denomination = TDBDenominations::GetDenominationValue(DBTransaction,(int)sgDisplay->Objects[0][sgDisplay->Row]);
                       denomination = GetDenominationValue(DBTransaction, denomination);
                       SaveDenomination(DBTransaction,(int)sgDisplay->Objects[0][sgDisplay->Row],frmTouchKeyboard->KeyboardText,denomination);
                    }
                }

			}
			DBTransaction.Commit();
		}
		ShowMessages();
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMessageMaintenance::btnDelMessageClick(TObject *Sender)
{
    if((int)sgDisplay->Objects[0][sgDisplay->Row] == 0)
    {
        MessageBox("There is nothing to delete.", "Warning", MB_ICONWARNING + MB_OK);
    }
    else if(MessageBox("Are you sure wish to delete this Option?", "Warning", MB_ICONWARNING + MB_OKCANCEL) == ID_OK)
	{
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();

		if(MessageType == ePatronTypes)
		{
			TManagerPatron::Instance().Delete(DBTransaction,(int)sgDisplay->Objects[0][sgDisplay->Row]);
		}
		else
		{
			TManagerInterface *Manager = NULL;
			if (MessageType == eRunProgram)
			{
				Manager = ManagerRun;
			}
			else
			{
				Manager = ManagerMessage;
			}
            if(MessageType == eCashDenomination)
            {
              TDBDenominations::DeleteDenominations(DBTransaction,(int)sgDisplay->Objects[0][sgDisplay->Row]);
            }
            else
            {
              Manager->Delete(DBTransaction,(int)sgDisplay->Objects[0][sgDisplay->Row]);
            }

		}
		DBTransaction.Commit();		
	}
	ShowMessages();
}
//---------------------------------------------------------------------------
Currency TfrmMessageMaintenance::GetDenominationValue(Database::TDBTransaction &DBTransaction,Currency denominationValue)
{
    std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
    frmTouchNumpad->Caption = "Cash Denominations";
    frmTouchNumpad->btnSurcharge->Caption = "Ok";
    frmTouchNumpad->btnDiscount->Visible = false;
    frmTouchNumpad->btnSurcharge->Visible = true;
    frmTouchNumpad->IsInitialized = false;
    frmTouchNumpad->Mode = pmDecimal;
    frmTouchNumpad->CURInitial = denominationValue;
    if (frmTouchNumpad->ShowModal() == mrOk)
    {
        denominationValue = frmTouchNumpad->CURResult;
    }
    return denominationValue;
}
//---------------------------------------------------------------------------
Currency TfrmMessageMaintenance::LoadDenominations(Database::TDBTransaction &DBTransaction)
{
    std::vector<TDenomination> denominations;
    TDBDenominations::LoadDenominations(DBTransaction,denominations);
    if(denominations.size() == 0)
    {
       sgDisplay->RowCount = 2;
    }
    else
    {
       sgDisplay->RowCount = denominations.size() + 1;
    }


    sgDisplay->Cols[0]->Clear();
    sgDisplay->Cols[1]->Clear();
    sgDisplay->Cols[0]->Add("Button Title");
    sgDisplay->Cols[1]->Add("Denomination");

    for(std::vector<TDenomination>::iterator it = denominations.begin(); it != denominations.end(); ++it)
    {
         int Index = sgDisplay->Cols[0]->Add(it->Title);
         sgDisplay->Cols[0]->Objects[Index] = (TObject *)it->Key;
         Index = sgDisplay->Cols[1]->Add(FormatFloat("0.00",it->DenominationValue));
         sgDisplay->Cols[1]->Objects[Index] = (TObject *)it->Key;
    }
}
//---------------------------------------------------------------------------
void TfrmMessageMaintenance::GetHeaders(AnsiString& CurrentCaption, AnsiString& CurrentMessage)
{
    CurrentCaption = "Enter Button Title";
    CurrentMessage = "Enter Message";
    switch(MessageType)
    {
        case eMessage:
        {
            CurrentCaption = "Enter Button Title";
            CurrentMessage = "Enter Message";
            break;
        }
        case eCancelReason:
        {
            CurrentCaption = "Enter Cancel Button Title";
            CurrentMessage = "Enter Cancel Message";
            break;
        }
        case eCreditReason:
        {
            CurrentCaption = "Enter Refund Button Title";
            CurrentMessage = "Enter Refund Message";
            break;
        }
        case eRunProgram:
        {
            CurrentCaption = "Enter Program Button Title";
            CurrentMessage = "Enter Run Command";
            break;
        }
        case ePatronTypes:
        {
            CurrentCaption = "Enter Patron Type (i.e Adult)";
            CurrentMessage = "";
            break;
        }
        case eSkimRefloat:
        {
            CurrentCaption = "Enter Button Title";
            CurrentMessage = "Enter Deposit/Withdrawal Reason";
            break;
        }
        case eWriteOff:
        {
            CurrentCaption = "Enter Button Title";
            CurrentMessage = "Enter Wastage Reason";
            break;
        }
        case eCustomerTypes:
        {
            CurrentCaption = "Enter Button Title";
            CurrentMessage = "Enter Order Type Reason";
            break;
        }
        case eCashDrawer:
        {
            CurrentCaption = "Enter Button Title";
            CurrentMessage = "Enter Cash Drawer Reason";
            break;
        }
        case eCashDenomination:
        {
            CurrentCaption = "Enter Button Title";
            CurrentMessage = "Enter Cash Denomination";
            break;
        }
    }
}
//---------------------------------------------------------------------------
void TfrmMessageMaintenance::SaveDenomination(Database::TDBTransaction &DBTransaction,int key, AnsiString inTitle, Currency inValue)
{
   inTitle = inTitle.Trim();
   if(inTitle == "")
   {
      MessageBox("Denomination must have a title.", "Warning", MB_ICONWARNING + MB_OK);
   }
   else if(inValue == 0)
   {
      MessageBox("Denomination must have value greater than zero.", "Warning", MB_ICONWARNING + MB_OK);
   }
   else if(TDBDenominations::IsDenominationExist(DBTransaction,key,inTitle))
   {
     MessageBox("Denomination with same title already exist.", "Warning", MB_ICONWARNING + MB_OK);
   }
   else
   {
     TDenomination denomination;
     denomination.Key = key;
     denomination.Title = inTitle;
     denomination.DenominationValue = inValue;
     TDBDenominations::SaveDenomination(DBTransaction,denomination);
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMessageMaintenance::sgDisplayDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State)
{
  sgDisplay->Canvas->Font->Charset = DEFAULT_CHARSET;
  sgDisplay->Canvas->Font->Name="Tahoma";
  if(ARow > 0 && SelectedRow == ARow)
  {
    sgDisplay->Canvas->Brush->Color = clNavy;
    sgDisplay->Canvas->Font->Color = clWhite;
  }
  else
  {
    sgDisplay->Canvas->Font->Color = clWindowText;
    sgDisplay->Canvas->Brush->Color = clWhite;
  }

  UnicodeString CellContent = sgDisplay->Cells[ACol][ARow];
  sgDisplay->Canvas->Font->Size = 14;
  sgDisplay->Canvas->Font->Style= TFontStyles();

  if(ARow == 0)
  {
    sgDisplay->Canvas->Font->Style= TFontStyles() << fsBold;
  }
  sgDisplay->Canvas->FillRect(Rect);

  if(ACol == 0 || MessageType != eCashDenomination)
  {
    sgDisplay->Canvas->TextRect(Rect, Rect.Left + 5 , Rect.Top, CellContent);
  }
  else
  {
    Word SavedAlign = SetTextAlign(sgDisplay->Canvas->Handle,TA_RIGHT);
    sgDisplay->Canvas->TextRect(Rect, Rect.Right - 5, Rect.Top, CellContent);
    SetTextAlign(sgDisplay->Canvas->Handle, SavedAlign);
  }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void __fastcall TfrmMessageMaintenance::sgDisplaySelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect)
{
    if(ARow > 0)
    {
        SelectedRow = ARow;
    }
}
//---------------------------------------------------------------------------
bool TfrmMessageMaintenance::CheckDefaultPatronTypes()
{
    bool retVal = true;
    if((int)sgDisplay->Objects[0][sgDisplay->Row] > 0 && MessageType == ePatronTypes)
    {

        Database::TDBTransaction DBTransaction(DBControl);
        DBTransaction.StartTransaction();
        TIBSQL *query = DBTransaction.Query(DBTransaction.AddQuery());
        query->Close();
        query->SQL->Text = " SELECT a.PATRONTYPES_KEY, a.PATRON_TYPE, a.IS_DEFAULT "
                           " FROM PATRONTYPES a WHERE a.IS_DEFAULT = :IS_DEFAULT ";
        query->ParamByName("IS_DEFAULT")->AsString = "T" ;
        query->ExecQuery();
        if(!query->RecordCount)
        {
            retVal = false;
            MessageBox("Please Configure one of Patron Type to Default ", "Warning", MB_ICONWARNING + MB_OK);
        }
        DBTransaction.Commit();
    }
    return retVal;
}
//---------------------------------------------------------------------------


