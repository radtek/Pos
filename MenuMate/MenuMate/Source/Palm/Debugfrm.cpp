//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Debugfrm.h"
#include "DebugManager.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmDebug *frmDebug;
//---------------------------------------------------------------------------
__fastcall TfrmDebug::TfrmDebug(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebug::Button2Click(TObject *Sender)
{
	if(cbMessageType->ItemIndex  !=  -1 )
	{
		TDebugHelp *in = new TDebugHelp;
		in->MessageCount = StrToInt(edCount->Text);
		in->TriggerCount = StrToInt(edTriggerCount->Text);

		if(cbMessageType->ItemIndex  > 10 )
		{
			in->MessageType = (MESSAGE_TYPE)(0x00);
		}
		else
		{
			in->MessageType = (MESSAGE_TYPE)(cbMessageType->ItemIndex + 0xE0);
		}

		if(cbErrorType->ItemIndex  >= 24 )
		{ // Errors 256 and up the - 23 is correct.
			in->Action = (cbErrorType->ItemIndex - 23) + 255;
		}
		else if(cbErrorType->ItemIndex  >= 23 )
		{
			in->Action = (EProcessingErrorType)(cbErrorType->ItemIndex - 23) + 255;
		}
		else if(cbErrorType->ItemIndex  >= 22 )
		{
			in->Action = (EProcessingErrorType)((cbErrorType->ItemIndex - 22) + 200);
		}
		else if(cbErrorType->ItemIndex  >= 16 )
		{
			in->Action = (EProcessingErrorType)((cbErrorType->ItemIndex - 16) + 128);
		}
		else
		{
			in->Action = (EProcessingErrorType)(cbErrorType->ItemIndex + 1);
		}
		ManagerDebug->BugsAdd(in);
		lbBugs->Clear();
		ManagerDebug->BugsGet(lbBugs->Items);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebug::Button3Click(TObject *Sender)
{
	ManagerDebug->BugsDelete((TDebugHelp *)lbBugs->Items->Objects[lbBugs->ItemIndex]);
	lbBugs->Clear();
	ManagerDebug->BugsGet(lbBugs->Items);
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebug::Button4Click(TObject *Sender)
{
	lbBugs->Clear();
	ManagerDebug->BugsGet(lbBugs->Items);
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebug::Button1Click(TObject *Sender)
{
	Close();	
}
//---------------------------------------------------------------------------
