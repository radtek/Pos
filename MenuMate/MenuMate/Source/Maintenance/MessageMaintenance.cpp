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
#include "ServingTime.h"
#include "GlobalSettings.h"
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
        case eRevenueCodes:
        {
            managerPMSCodes = new TManagerPMSCodes();
            managerPMSCodes->RevenueCodesMap.clear();
            pnlLabel->Caption = "Revenue Codes";
            break;
        }
        case eServingTimes:
        {
            managerPMSCodes = new TManagerPMSCodes();
            managerPMSCodes->TimeSlots.clear();
            pnlLabel->Caption = "Serving Times";
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
      case eRevenueCodes:
      {
        sgDisplay->Cols[0]->Add("Code");
        sgDisplay->Cols[1]->Add("Description");
        LoadRevenueCodes(DBTransaction);
        break;
      }
      case eServingTimes:
      {
        sgDisplay->Cols[0]->Add("Meal Code");
        sgDisplay->Cols[1]->Add("Timing");
        LoadServingDetails(DBTransaction);
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
    if(MessageType == eServingTimes || MessageType == eRevenueCodes)
        delete managerPMSCodes;
	Close();
    if(CheckDefaultPatronTypes())
    {
	    Close();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMessageMaintenance::btnAddMessageClick(TObject *Sender)
{

    switch(MessageType)
    {
        case eRevenueCodes:
        {
            AddRevenueCode(Sender);
            break;
        }
        case eServingTimes:
        {
            AddServingTime(Sender);
            break;
        }
        default:
        {
            AddMessage(Sender);
            break;
        }

    }
}
//----------------------------------------------------------------------------
void TfrmMessageMaintenance::AddMessage(TObject *Sender)
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
            else if(MessageType == eRevenueCodes)
            {
                int key = (int)sgDisplay->Objects[0][sgDisplay->Row];
                UpdateRevenueCode(DBTransaction,key);
            }
            else if(MessageType == eServingTimes)
            {
                int key = (int)sgDisplay->Objects[0][sgDisplay->Row];
                UpdateMealDetails(DBTransaction,key);
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
        AnsiString val = (int)sgDisplay->Objects[0][sgDisplay->Row];
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();

		if(MessageType == ePatronTypes)
		{
			TManagerPatron::Instance().Delete(DBTransaction,(int)sgDisplay->Objects[0][sgDisplay->Row]);
		}
        else if(MessageType == eRevenueCodes)
        {
            managerPMSCodes->DeleteRevenueCode(DBTransaction,(int)sgDisplay->Objects[0][sgDisplay->Row]);
        }
        else if(MessageType == eServingTimes)
        {
            managerPMSCodes->DeleteMealDetails(DBTransaction,(int)sgDisplay->Objects[0][sgDisplay->Row]);
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
        case eRevenueCodes:
        {
            CurrentCaption = "Enter Revenue Code";
            CurrentMessage = "Enter Revenue Description";
            break;
        }
    }
}
//---------------------------------------------------------------------------
void TfrmMessageMaintenance::SaveDenomination(Database::TDBTransaction &DBTransaction,int key, AnsiString inTitle, Currency inValue)
{
    try
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
   catch(Exception &E)
   {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
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
    return  retVal;
}
//---------------------------------------------------------------------------
void TfrmMessageMaintenance::AddRevenueCode(TObject *Sender)
{
    std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
    frmTouchNumpad->Mode = pmNumber;
    frmTouchNumpad->Caption = "Enter Revenue Code";
    frmTouchNumpad->btnSurcharge->Caption = "Ok";
    frmTouchNumpad->btnDiscount->Visible = false;
    frmTouchNumpad->btnSurcharge->Visible = true;
    if(TGlobalSettings::Instance().PMSType == Oracle)
       frmTouchNumpad->SetMaxLengthValue(2);
    else if(TGlobalSettings::Instance().PMSType == SiHot)
        frmTouchNumpad->SetMaxLengthValue(3);

    frmTouchNumpad->INTInitial = 0;
    std::map<int, TRevenueCodeDetails>::iterator iter;
    if ((frmTouchNumpad->ShowModal() == mrOk && frmTouchNumpad->INTResult > 0) &&
        ((frmTouchNumpad->INTResult < 17 && TGlobalSettings::Instance().PMSType == Oracle) ||
            (frmTouchNumpad->INTResult < 101 && TGlobalSettings::Instance().PMSType == SiHot)))
    {
        iter = managerPMSCodes->RevenueCodesMap.find(frmTouchNumpad->INTResult);
        if(iter == managerPMSCodes->RevenueCodesMap.end())
        {
            Database::TDBTransaction DBTransaction(DBControl);
            DBTransaction.StartTransaction();

            std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
            frmTouchKeyboard->MaxLength = 20;
            frmTouchKeyboard->AllowCarriageReturn = false;
            frmTouchKeyboard->StartWithShiftDown = true;
            frmTouchKeyboard->KeyboardText = "";
            frmTouchKeyboard->Caption = "Enter Revenue Code Description";

            if(frmTouchKeyboard->ShowModal() == mrOk)
            {
                TRevenueCodeDetails globalDetails;
                if(managerPMSCodes->RevenueCodesMap.size() == 0)
                   globalDetails.IsDefault = true;
                globalDetails.RevenueCodeDescription = frmTouchKeyboard->KeyboardText;
                managerPMSCodes->RevenueCodesMap.insert(std::pair<int,TRevenueCodeDetails>(frmTouchNumpad->INTResult,globalDetails));
                std::map<int,TRevenueCodeDetails> localMap;
                localMap.clear();
                TRevenueCodeDetails localDetails;
                if(managerPMSCodes->RevenueCodesMap.size() == 1)
                    localDetails.IsDefault = true;
                else
                    localDetails.IsDefault = false;
                localDetails.RevenueCodeDescription = frmTouchKeyboard->KeyboardText;
                localMap.insert(std::pair<int,TRevenueCodeDetails>(frmTouchNumpad->INTResult,localDetails));
                managerPMSCodes->SaveRevenueCodesToDB(DBTransaction,localMap);
                if(localDetails.IsDefault)
                    MessageBox("Being first Revenue code, it will be saved as default one","Information",MB_OK + MB_ICONINFORMATION);
            }
            DBTransaction.Commit();
            ShowMessages();
        }
        else
        {
            MessageBox("Please Select a different Revenue Code","WARNING",MB_ICONWARNING + MB_OK);
        }
    }
    else if(TGlobalSettings::Instance().PMSType == Oracle && (frmTouchNumpad->INTResult == 0 || frmTouchNumpad->INTResult > 16))
    {
        MessageBox("Revenue code value can be 1 to 16 only.","WARNING",MB_ICONWARNING + MB_OK);
    }
    else if(TGlobalSettings::Instance().PMSType == SiHot && (frmTouchNumpad->INTResult == 0 || frmTouchNumpad->INTResult > 100))
    {
        MessageBox("Revenue code value can be 1 to 100 only.","WARNING",MB_ICONWARNING + MB_OK);
    }
}
//---------------------------------------------------------------------------
void TfrmMessageMaintenance::LoadRevenueCodes(Database::TDBTransaction &DBTransaction)
{
    sgDisplay->Cols[0]->Clear();
    sgDisplay->Cols[1]->Clear();
    sgDisplay->Cols[0]->Add("Code");
    sgDisplay->Cols[1]->Add("Decription");
    managerPMSCodes->GetRevenueCodesDetails(DBTransaction,sgDisplay,managerPMSCodes->RevenueCodesMap);
}
//---------------------------------------------------------------------------
void TfrmMessageMaintenance::UpdateRevenueCode(Database::TDBTransaction &DBTransaction, int key)
{
    std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
    frmTouchNumpad->Mode = pmNumber;
    frmTouchNumpad->Caption = "Enter Revenue Code";
    frmTouchNumpad->btnSurcharge->Caption = "Ok";
    frmTouchNumpad->btnDiscount->Visible = false;
    frmTouchNumpad->btnSurcharge->Visible = true;
    frmTouchNumpad->MaxLength = 3;
    if(TGlobalSettings::Instance().PMSType == Oracle)
       frmTouchNumpad->SetMaxLengthValue(2);
    else if(TGlobalSettings::Instance().PMSType == SiHot)
        frmTouchNumpad->SetMaxLengthValue(3);

    frmTouchNumpad->INTInitial = key;

    std::map<int, TRevenueCodeDetails>::iterator iter;
    if ((frmTouchNumpad->ShowModal() == mrOk && frmTouchNumpad->INTResult > 0) &&
        ((frmTouchNumpad->INTResult < 17 && TGlobalSettings::Instance().PMSType == Oracle) ||
            (frmTouchNumpad->INTResult < 101 && TGlobalSettings::Instance().PMSType == SiHot)))
    {
        iter = managerPMSCodes->RevenueCodesMap.find(frmTouchNumpad->INTResult);
        if(iter == managerPMSCodes->RevenueCodesMap.end() || (frmTouchNumpad->INTResult == key))
        {
            Database::TDBTransaction DBTransaction(DBControl);
            DBTransaction.StartTransaction();

            std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
            frmTouchKeyboard->MaxLength = 20;
            frmTouchKeyboard->AllowCarriageReturn = false;
            frmTouchKeyboard->StartWithShiftDown = true;
            frmTouchKeyboard->KeyboardText = managerPMSCodes->RevenueCodesMap[key].RevenueCodeDescription;
            frmTouchKeyboard->Caption = "Enter Revenue Code Description";

            if(frmTouchKeyboard->ShowModal() == mrOk)
            {
                TRevenueCodeDetails codeDetails;
                codeDetails.IsDefault = false;
                codeDetails.RevenueCodeDescription = frmTouchKeyboard->KeyboardText;
                managerPMSCodes->RevenueCodesMap.insert(std::pair<int,TRevenueCodeDetails>(frmTouchNumpad->INTResult,codeDetails));
                managerPMSCodes->EditRevenueCode(DBTransaction,key,frmTouchNumpad->INTResult,codeDetails);
                // Update Item Sizes with Revenue Codes
                managerPMSCodes->UpdateItemSizes(DBTransaction,frmTouchNumpad->INTResult,key);
            }
            DBTransaction.Commit();
            ShowMessages();
        }
        else
        {
            MessageBox("Please Select a different Revenue Code. This Revenue code already exists.",
                        "WARNING",MB_ICONWARNING + MB_OK);
        }
    }
    else if(TGlobalSettings::Instance().PMSType == Oracle && (frmTouchNumpad->INTResult == 0 || frmTouchNumpad->INTResult > 16))
    {
        MessageBox("Revenue code value can be 1 to 16 only.","WARNING",MB_ICONWARNING + MB_OK);
    }
    else if(TGlobalSettings::Instance().PMSType == SiHot && (frmTouchNumpad->INTResult == 0 || frmTouchNumpad->INTResult > 100))
    {
        MessageBox("Revenue code value can be 1 to 100 only.","WARNING",MB_ICONWARNING + MB_OK);
    }
}
//---------------------------------------------------------------------------
void TfrmMessageMaintenance::AddServingTime(TObject *Sender)
{
    std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
    frmTouchNumpad->Mode = pmNumber;
    frmTouchNumpad->Caption = "Enter Meal Code";
    frmTouchNumpad->btnSurcharge->Caption = "Ok";
    frmTouchNumpad->btnDiscount->Visible = false;
    frmTouchNumpad->btnSurcharge->Visible = true;
    frmTouchNumpad->SetMaxLengthValue(2);
    frmTouchNumpad->INTInitial = 0;
    AnsiString mealName = "";
    if (frmTouchNumpad->ShowModal() == mrOk && frmTouchNumpad->INTResult >0)
    {
        mealName = frmTouchNumpad->INTResult;
        TTimeSlots slots;
        std::auto_ptr<TfrmServingTime> frmServingTime(TfrmServingTime::Create<TfrmServingTime>(this));
        frmServingTime->Caption = "Meal Start Time";
        frmServingTime->TimeType = startTime;
        frmServingTime->Left = (Screen->Width - frmServingTime->Width) / 2;
        frmServingTime->Top  = (Screen->Height - frmServingTime->Height) / 2;
        if(frmServingTime->ShowModal() == mrOk)
        {
            if(frmServingTime->Time1 == "00:00:00")
            {
                MessageBox("Invalid Start Time","",MB_OK);
                return;
            }
            if(ValidateTimeSlot(frmServingTime->Time1, 0))
            {
                frmServingTime->TimeType = endTime;
                frmServingTime->Caption = "Meal End Time";
                if(frmServingTime->ShowModal() == mrOk)
                {
                    if(frmServingTime->Time2 == "00:00:00")
                    {
                        MessageBox("Invalid End Time","WARNING",MB_OK);
                        return;
                    }
                    if(ValidateTimeSlot(frmServingTime->Time2, 0))
                    {
                        slots.MealName = mealName;
                        slots.StartTime = frmServingTime->Time1;
                        slots.EndTime  =  frmServingTime->Time2;
                        if(managerPMSCodes->TimeSlots.size() == 0)
                        {
                            slots.IsDefault = true;
                            MessageBox("Being first Serving Time, it will be saved as default one","Information",MB_OK + MB_ICONINFORMATION);
                        }
                        else
                            slots.IsDefault = false;
                        managerPMSCodes->TimeSlots.push_back(slots);
                        InsertMealSlotToDB(slots);
                    }
                    else
                      MessageBox("End Time falls under already configured time slots","WARNING",MB_ICONWARNING + MB_OK);
                }
             }
             else
                MessageBox("Start Time falls under already configured time slots","WARNING",MB_ICONWARNING + MB_OK);
        }
    }
    else if(frmTouchNumpad->INTResult == 0)
    {
        MessageBox("Meal code should be more than 0","WARNING",MB_ICONWARNING + MB_OK);
    }
}
//----------------------------------------------------------------------------
void TfrmMessageMaintenance::InsertMealSlotToDB(TTimeSlots slots)
{
    Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();
    managerPMSCodes->InsertTimeSlots(DBTransaction,slots);
    DBTransaction.Commit();
    ShowMessages();
}
//----------------------------------------------------------------------------
bool TfrmMessageMaintenance::ValidateTimeSlot(TDateTime time, int key)
{
    bool retValue = true;
    if(managerPMSCodes->TimeSlots.size() > 0)
    {
        std::vector<TTimeSlots>::iterator slotsIT = managerPMSCodes->TimeSlots.begin();
        for(;slotsIT != managerPMSCodes->TimeSlots.end(); ++slotsIT)
        {
            if(time >= slotsIT->StartTime && time <= slotsIT->EndTime)
            {
                if(key == 0)
                {
                    retValue = false;
                    break;
                }
                else
                {
                    if(key == slotsIT->key)
                        retValue = true;
                    else
                    {
                        retValue = false;
                        break;
                    }
                }
            }
            else
            {
               retValue = true;
            }
        }
    }
    return retValue;
}
//---------------------------------------------------------------------------
void TfrmMessageMaintenance::LoadServingDetails(Database::TDBTransaction &DBTransaction)
{
    sgDisplay->Cols[0]->Clear();
    sgDisplay->Cols[1]->Clear();
    sgDisplay->Cols[0]->Add("Meal Code");
    sgDisplay->Cols[1]->Add("Timing");
    managerPMSCodes->GetMealDetails(DBTransaction,sgDisplay,managerPMSCodes->TimeSlots);
}
//---------------------------------------------------------------------------
void TfrmMessageMaintenance::UpdateMealDetails(Database::TDBTransaction &DBTransaction, int key)
{
    std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
    frmTouchNumpad->Mode = pmNumber;
    frmTouchNumpad->Caption = "Enter Meal Code";
    frmTouchNumpad->btnSurcharge->Caption = "Ok";
    frmTouchNumpad->btnDiscount->Visible = false;
    frmTouchNumpad->btnSurcharge->Visible = true;
    int initialValue = 0;
    for(int mealCodeIndex = 0; mealCodeIndex < managerPMSCodes->TimeSlots.size(); mealCodeIndex++)
    {
        if(managerPMSCodes->TimeSlots[mealCodeIndex].key == key)
        {
            initialValue = atoi(managerPMSCodes->TimeSlots[mealCodeIndex].MealName.c_str());
            break;
        }
    }
    frmTouchNumpad->INTInitial = initialValue;
    frmTouchNumpad->SetMaxLengthValue(2);
    AnsiString mealName = "";
    if (frmTouchNumpad->ShowModal() == mrOk && frmTouchNumpad->INTResult > 0)
    {
        mealName = frmTouchNumpad->INTResult;
        TTimeSlots slots;
        std::auto_ptr<TfrmServingTime> frmServingTime(TfrmServingTime::Create<TfrmServingTime>(this));
        frmServingTime->Caption = "Meal Start Time";
        frmServingTime->TimeType = startTime;
        frmServingTime->Left = (Screen->Width - frmServingTime->Width) / 2;
        frmServingTime->Top  = (Screen->Height - frmServingTime->Height) / 2;
        if(frmServingTime->ShowModal() == mrOk)
        {
            if(frmServingTime->Time1 == "00:00:00")
            {
                MessageBox("Invalid Start Time","WARNING",MB_OK);
                return;
            }
            if(ValidateTimeSlot(frmServingTime->Time1,key))
            {
                frmServingTime->TimeType = endTime;
                frmServingTime->Caption = "Meal End Time";
                if(frmServingTime->ShowModal() == mrOk)
                {
                    if(frmServingTime->Time2 == "00:00:00")
                    {
                        MessageBox("Invalid End Time","WARNING",MB_OK);
                        return;
                    }
                    if(ValidateTimeSlot(frmServingTime->Time2,key))
                    {
                        slots.key = key;
                        slots.MealName = mealName;
                        slots.StartTime = frmServingTime->Time1;
                        slots.EndTime  =  frmServingTime->Time2;
                        managerPMSCodes->EditMeal(DBTransaction,slots);
                    }
                    else
                      MessageBox("End Time falls under already configured time slots","",MB_OK);
                }
             }
             else
                MessageBox("Start Time falls under already configured time slots","",MB_OK);
        }
    }
    else if(frmTouchNumpad->INTResult == 0)
    {
        MessageBox("Meal code should be more than 0","WARNING",MB_ICONWARNING + MB_OK);
    }
}
//---------------------------------------------------------------------------
