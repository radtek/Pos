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
	if (MessageType == eMessage)
	{
		pnlLabel->Caption = "Message List";
	}
	else if (MessageType == eCancelReason)
	{
		pnlLabel->Caption = "Cancel Reasons";
	}
	else if (MessageType == eCreditReason)
	{
		pnlLabel->Caption = "Refund Reasons";
	}
	else if (MessageType == eRunProgram)
	{
		pnlLabel->Caption = "Configured Programs.";
	}
	else if (MessageType == ePatronTypes)
	{
		pnlLabel->Caption = "Configure Patron Types.";
	}
	else if (MessageType == eSkimRefloat)
	{
		pnlLabel->Caption = "Deposit/Withdrawal Reasons";
	}
	else if (MessageType == eWriteOff)
	{
		pnlLabel->Caption = "Wastage Reasons";
	}
	else if (MessageType == eCustomerTypes)
	{
		pnlLabel->Caption = "Customer Order Type Reasons";
	}
	else if (MessageType == eCashDrawer)
	{
		pnlLabel->Caption = "Cash Drawer Reason";
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
/*	if (Tag != Screen->Width)
	{
		int Temp = Tag;
		Tag = Screen->Width;

		ScaleBy(Screen->Width, Temp);
	}                                    */
	ClientWidth = Panel1->Width + (Panel1->Left * 2);
	ClientHeight = Panel1->Height + (Panel1->Top * 2);
	Left = (Screen->Width - Width) / 2;
	Top = (Screen->Height - Height) / 2;
}
//---------------------------------------------------------------------------
void TfrmMessageMaintenance::ShowMessages()
{
	sgDisplay->ColWidths[0] = sgDisplay->ClientWidth * 1 / 3;
	sgDisplay->ColWidths[1] = sgDisplay->ClientWidth - sgDisplay->ColWidths[1] - 1;

	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	if (MessageType == eRunProgram)
	{
		ManagerRun->LoadMessages(DBTransaction,sgDisplay,MessageType);
	}
	else if (MessageType == ePatronTypes)
	{
		TManagerPatron::Instance().GetPatronTypes(DBTransaction,sgDisplay);
	}
	else
	{
		ManagerMessage->LoadMessages(DBTransaction,sgDisplay,MessageType);
	}
	DBTransaction.Commit();
	sgDisplay->ColWidths[0] = sgDisplay->ClientWidth * 1 / 3;
	sgDisplay->ColWidths[1] = sgDisplay->ClientWidth - sgDisplay->ColWidths[1] - 1;

}
//---------------------------------------------------------------------------
void __fastcall TfrmMessageMaintenance::imgExitClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMessageMaintenance::btnAddMessageClick(TObject *Sender)
{

	try
	{
		AnsiString CurrentCaption = "Enter Button Title";
		AnsiString CurrentMessage = "Enter Message";

		if (MessageType == eMessage)
		{
			CurrentCaption = "Enter Button Title";
			CurrentMessage = "Enter Message";
		}
		else if (MessageType == eCancelReason)
		{
			CurrentCaption = "Enter Cancel Button Title";
			CurrentMessage = "Enter Cancel Message";
		}
		else if (MessageType == eCreditReason)
		{
			CurrentCaption = "Enter Refund Button Title";
			CurrentMessage = "Enter Refund Message";
		}
		else if (MessageType == eRunProgram)
		{
			CurrentCaption = "Enter Program Button Title";
			CurrentMessage = "Enter Run Command";
		}
		else if (MessageType == ePatronTypes)
		{
			CurrentCaption = "Enter Patron Type (i.e Adult)";
			CurrentMessage = "";
		}
		else if (MessageType == eSkimRefloat)
		{
			CurrentCaption = "Enter Button Title";
			CurrentMessage = "Enter Deposit/Withdrawal Reason";
		}
		else if (MessageType == eWriteOff)
		{
			CurrentCaption = "Enter Button Title";
			CurrentMessage = "Enter Wastage Reason";
		}
		else if (MessageType == eCustomerTypes)
		{
			CurrentCaption = "Enter Button Title";
			CurrentMessage = "Enter Order Type Reason";
		}
		else if (MessageType == eCashDrawer)
		{
			CurrentCaption = "Enter Button Title";
			CurrentMessage = "Enter Cash Drawer Reason";
		}

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
                }
				frmTouchKeyboard->Caption = CurrentMessage;
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
void __fastcall TfrmMessageMaintenance::btnEditMessageClick(
		TObject *Sender)
{

	try
	{
		if((int)sgDisplay->Objects[0][sgDisplay->Row] == 0)
		{
			MessageBox("There is nothing to Edit.", "Warning",
							MB_ICONWARNING + MB_OK);
		}
		else
		{
			AnsiString CurrentCaption = "Enter Button Title";
			AnsiString CurrentMessage = "Enter Message";
			if (MessageType == eMessage)
			{
				CurrentCaption = "Enter Button Title";
				CurrentMessage = "Enter Message";
			}
			else if (MessageType == eCancelReason)
			{
				CurrentCaption = "Enter Cancel Button Title";
				CurrentMessage = "Enter Cancel Message";
			}
			else if (MessageType == eCreditReason)
			{
				CurrentCaption = "Enter Refund Button Title";
				CurrentMessage = "Enter Refund Message";
			}
			else if (MessageType == eRunProgram)
			{
				CurrentCaption = "Enter Program Button Title";
				CurrentMessage = "Enter Run Command";
			}
			else if (MessageType == ePatronTypes)
			{
				CurrentCaption = "Enter Patron Type (i.e Adult)";
				CurrentMessage = "";
			}
			else if (MessageType == eSkimRefloat)
			{
				CurrentCaption = "Enter Button Title";
				CurrentMessage = "Enter Reason";
			}
			else if (MessageType == eWriteOff)
			{
				CurrentCaption = "Enter Button Title";
				CurrentMessage = "Enter Reason";
			}
			else if (MessageType == eCustomerTypes)
			{
				CurrentCaption = "Enter Button Title";
				CurrentMessage = "Enter Reason";
			}
			else if (MessageType == eCashDrawer)
			{
				CurrentCaption = "Enter Button Title";
				CurrentMessage = "Enter Cash Drawer Reason";
			}

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
				frmTouchKeyboard->KeyboardText =  Manager->GetTitle(DBTransaction,(int)sgDisplay->Objects[0][sgDisplay->Row]);
				frmTouchKeyboard->Caption = CurrentCaption;
				if (frmTouchKeyboard->ShowModal() == mrOk)
				{
					Manager->SetTitle(DBTransaction,(int)sgDisplay->Objects[0][sgDisplay->Row],frmTouchKeyboard->KeyboardText);
					frmTouchKeyboard->MaxLength = 200	;
					frmTouchKeyboard->AllowCarriageReturn = false;
					frmTouchKeyboard->StartWithShiftDown = true;
                    if(MessageType == eCashDrawer)
                    {
                       frmTouchKeyboard->MustHaveValue = true;
                    }
					frmTouchKeyboard->KeyboardText =  Manager->GetContent(DBTransaction,(int)sgDisplay->Objects[0][sgDisplay->Row]);
					frmTouchKeyboard->Caption = CurrentMessage;
					if (frmTouchKeyboard->ShowModal() == mrOk)
					{
						Manager->SetContent(DBTransaction,(int)sgDisplay->Objects[0][sgDisplay->Row],frmTouchKeyboard->KeyboardText);
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
	if(MessageBox("Are you sure wish to delete this Option?", "Warning",
		MB_ICONWARNING + MB_OKCANCEL) == ID_OK)
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
			Manager->Delete(DBTransaction,(int)sgDisplay->Objects[0][sgDisplay->Row]);
		}
		DBTransaction.Commit();		
	}
	ShowMessages();
}
//---------------------------------------------------------------------------

