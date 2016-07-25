//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "StockRequestHistory.h"
#include "Utilities.h"
#include "SelectStockItem.h"
#include "Connections.h"
#include "SelectDate.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmStockRequestHistory *frmStockRequestHistory;

//---------------------------------------------------------------------------
__fastcall TfrmStockRequestHistory::TfrmStockRequestHistory(TComponent* Owner)
        : TForm(Owner), frmStockRequest(new TfrmStockRequest(NULL))
{
}

//---------------------------------------------------------------------------

void __fastcall TfrmStockRequestHistory::FormShow(TObject *Sender)
{
	FormResize(NULL);

	if (!Transaction->InTransaction)
        Transaction->StartTransaction();

	lbeTitle->Caption = "Stock Request Details";

	LoadStockRequestsIntoTree();

	if (tvStockRequests->Items->Count > 0)
	{
		if (tvStockRequests->Items->Item[1]->Count > 0)
		{
			tvStockRequests->Selected = tvStockRequests->Items->Item[1]->getFirstChild();
			tvStockRequests->Items->Item[1]->Expand(false);
		}
		else
		{
			tvStockRequests->Selected = tvStockRequests->Items->Item[0];
			tvStockRequests->Selected->Expand(false);
			lbeReference->Caption = "";
		}
	}
	else
	{
		lbeReference->Caption = "";
	}

	tvStockRequests->SetFocus();
}

//---------------------------------------------------------------------------

void TfrmStockRequestHistory::LoadStockRequestsIntoTree()
{
	tvStockRequests->Items->BeginUpdate();
	tvStockRequests->OnChange = NULL;
	tvStockRequests->Items->Clear();

	qrStockRequest->Close();

	TTreeNode *LocationNode = NULL;
	TTreeNode *RequestTypeNode = NULL;
    TTreeNode *RequestNumberNode = NULL;

	AnsiString LastLocation = "", LastRequestType = "";

	for (qrStockRequest->Open(); !qrStockRequest->Eof; qrStockRequest->Next())
	{
		if (qrStockRequest->FieldByName("LOCATION")->AsString != LastLocation)
		{
            LastLocation = qrStockRequest->FieldByName("LOCATION")->AsString;
			LocationNode = tvStockRequests->Items->Add(NULL, LastLocation);

			LocationNode->ImageIndex = ICON_CLOSED_FOLDER_INDEX;
			LocationNode->SelectedIndex = ICON_OPEN_FOLDER_INDEX;

			LocationNode->Data = (void *)NULL;
             LastRequestType = "";
		}

        if (qrStockRequest->FieldByName("REQUEST_TYPE")->AsString != LastRequestType)
		{
            LastRequestType = qrStockRequest->FieldByName("REQUEST_TYPE")->AsString;
			RequestTypeNode = tvStockRequests->Items->AddChild(LocationNode, LastRequestType);

			RequestTypeNode->ImageIndex = ICON_CLOSED_FOLDER_INDEX;
			RequestTypeNode->SelectedIndex = ICON_OPEN_FOLDER_INDEX;

			RequestTypeNode->Data = (void *)NULL;
		}

		RequestNumberNode = tvStockRequests->Items->AddChild(RequestTypeNode, qrStockRequest->FieldByName("REQUEST_NUMBER")->AsInteger);

		RequestNumberNode->ImageIndex = ICON_NOTEPAD_INDEX;
		RequestNumberNode->SelectedIndex = ICON_NOTEPAD_INDEX;

		RequestNumberNode->Data = (void *)qrStockRequest->FieldByName("REQUEST_NUMBER")->AsInteger;
	}

	tvStockRequests->Items->EndUpdate();
    tvStockRequests->FullExpand();

	tvStockRequests->OnChange = tvStockRequestsChange;
}

//---------------------------------------------------------------------------

void __fastcall TfrmStockRequestHistory::FormClose(TObject *Sender, TCloseAction &Action)
{
	if (Transaction->InTransaction)
            Transaction->Commit();

	FindDialog->CloseDialog();
}

//---------------------------------------------------------------------------

void __fastcall TfrmStockRequestHistory::FormResize(TObject *Sender)
{
	ResizeGrids();
}

//---------------------------------------------------------------------------

void __fastcall TfrmStockRequestHistory::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(NULL);
}

//---------------------------------------------------------------------------

void TfrmStockRequestHistory::ResizeGrids()
{
	const ColCount2 = 5;
	TDBGrid *Grid = dbgDetails;

	Grid->TitleFont->Size = Grid->Font->Size;

	const PercArray2[ColCount2] = {40, 15, 11, 20, 14};

	int WidthSoFar = 0;
	int PercLeft = 100;

	for (int i=0; i<Grid->Columns->Count; i++)
	{
		int ColWidth = (Grid->ClientWidth - WidthSoFar) * PercArray2[i] / PercLeft - 1;
		if (PercArray2[i] != 0)
		{
			Grid->Columns->Items[i]->Width = ColWidth;
			WidthSoFar += ColWidth + 1;
			PercLeft -= PercArray2[i];
		}
		if (i == Grid->Columns->Count-1)
		{
			Grid->Columns->Items[i]->Width += 1;
		}
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmStockRequestHistory::btnCloseClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------

void __fastcall TfrmStockRequestHistory::btnNewStockRequestClick(TObject *Sender)
{
    frmStockRequest->RequestNumber = 0;

    if (frmStockRequest->ShowModal() == mrOk)
    {
        try
        {
            qrStockRequest->DisableControls();

            if (Transaction->InTransaction)
                Transaction->Commit();

            FormShow(NULL);
        }

        __finally
        {
            qrStockRequest->EnableControls();
          ResizeGrids();
        }
    }

    tvStockRequests->SetFocus();
}

//---------------------------------------------------------------------------

void __fastcall TfrmStockRequestHistory::btnUpdateStockRequestClick(TObject *Sender)
{
	if (qrStockRequest->IsEmpty())
	{
		Application->MessageBox("There is no stock request to update.\rTo enter a new stock request, click 'New Stock Request'.", "Error", MB_OK | MB_ICONERROR);
		return;
	}
	if (tvStockRequests->Selected && tvStockRequests->Selected->Level > 1 && qrStockRequest->Locate("REQUEST_NUMBER", int(tvStockRequests->Selected->Data), TLocateOptions()))
	{
        bool isUpdateAllowed = qrStockRequest->FieldByName("ISCOMMITTED")->AsString == "F";

        if(!isUpdateAllowed)
        {
            Application->MessageBox("The stock request is already committed.\rTo enter a new stock request, click 'New Stock Request'.", "Error", MB_OK | MB_ICONERROR);
		    return;
        }

		if (Application->MessageBox("This stock request has already been entered. Are you sure you want to update it?", "Error", MB_OKCANCEL | MB_ICONQUESTION) == ID_OK)
		{
                       frmStockRequest->RequestNumber = int(tvStockRequests->Selected->Data);

			if (frmStockRequest->ShowModal() == mrOk)
			{
				try
				{
					qrStockRequest->DisableControls();
					if (Transaction->InTransaction)
                                           Transaction->Commit();
					FormShow(NULL);
				}
				__finally
				{
					qrStockRequest->EnableControls();
					ResizeGrids();
				}
			}
		}
	}

	tvStockRequests->SetFocus();
}

//---------------------------------------------------------------------------

void __fastcall TfrmStockRequestHistory::tvStockRequestsChange(TObject *Sender, TTreeNode *Node)
{
	if (Node && Node->Level == 2)
	{
		qrStockRequestDetails->Close();

		if (qrStockRequest->Locate("REQUEST_NUMBER", int(Node->Data), TLocateOptions()))
		{
			lbeReference->Caption = "Stock Request: " + qrStockRequest->FieldByName("REQUEST_NUMBER")->AsString + " - " + FormatDateTime("d mmmm yyyy", qrStockRequest->FieldByName("DATE_GENERATED")->AsDateTime);
            btnUpdateStockRequest->Enabled = qrStockRequest->FieldByName("ISCOMMITTED")->AsString == "F";

			qrStockRequestDetails->Open();

			((TFloatField *)qrStockRequestDetails->FieldByName("STOCK_REQUEST_UNIT_COST" ))->currency = true;
			((TFloatField *)qrStockRequestDetails->FieldByName("STOCK_REQUEST_UNIT_COST" ))->DisplayFormat = "$####0.00";

			ResizeGrids();
		}
		else
		{
			qrStockRequestDetails->Close();
			lbeReference->Caption	= "";
		}
	}
	else
	{
		qrStockRequestDetails->Close();
		lbeReference->Caption	= "";
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmStockRequestHistory::FindDialogFind(TObject *Sender)
{
	TTreeNode *currentSelectedItem;

	if (tvStockRequests->Selected)
	{
		if (FindDialog->Options.Contains(frDown))
		{
			currentSelectedItem = tvStockRequests->Selected->GetNext();
		}
		else
		{
			currentSelectedItem = tvStockRequests->Selected->GetPrev();
		}
	}
	else
	{
		currentSelectedItem = tvStockRequests->Items->GetFirstNode();
	}
	while (currentSelectedItem)
	{
		AnsiString FindText, NodeText;

		if (FindDialog->Options.Contains(frMatchCase))
		{
			FindText = FindDialog->FindText;
			NodeText = currentSelectedItem->Text;
		}
		else
		{
			FindText = FindDialog->FindText.UpperCase();
			NodeText = currentSelectedItem->Text.UpperCase();
		}
		if (FindDialog->Options.Contains(frWholeWord))
		{
			if (FindText == NodeText)
			{
				tvStockRequests->Selected = currentSelectedItem;
				break;
			}
		}
		else
		{
			if (NodeText.AnsiPos(FindText))
			{
				tvStockRequests->Selected = currentSelectedItem;
				break;
			}
		}
		if (FindDialog->Options.Contains(frDown))
		{
			currentSelectedItem = currentSelectedItem->GetNext();
		}
		else
		{
			currentSelectedItem = currentSelectedItem->GetPrev();
		}
	}
	if (!currentSelectedItem)
	{
		Application->MessageBox("No more occurances found.", "Search Complete", MB_ICONINFORMATION + MB_OK);
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmStockRequestHistory::btnFindClick(TObject *Sender)
{
	FindDialog->Execute();
}

//---------------------------------------------------------------------------

void __fastcall TfrmStockRequestHistory::tvStockRequestsDblClick(TObject *Sender)
{
	btnUpdateStockRequestClick(NULL);
}

//---------------------------------------------------------------------------

void __fastcall TfrmStockRequestHistory::tvStockRequestsKeyPress(TObject *Sender, char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
		tvStockRequestsDblClick(Sender);
	}
}

//---------------------------------------------------------------------------
void __fastcall TfrmStockRequestHistory::btnDeleteStockRequestClick(
      TObject *Sender)
{
                try{
                if(!tvStockRequests->Selected)
                {
		Application->MessageBox("There is no stock request Selected.\rPlease select stock request .", "Error", MB_OK | MB_ICONERROR);
		return;


                }  int reqNumber = int(tvStockRequests->Selected->Data);


	if (Application->MessageBox("This stock request has been Deleted. Are you sure you want to delete it?", "Error", MB_OKCANCEL | MB_ICONQUESTION) == ID_OK)
		{
	if(!Transaction->InTransaction)
		    Transaction->StartTransaction();

	qrUpdateStockRequest->ParamByName("REQUEST_NUMBER")->AsInteger =reqNumber;

 	qrUpdateStockRequest->ParamByName("ISDELETED")->AsString	= 'T';

		qrUpdateStockRequest->ExecSQL();
			qrUpdateStockRequest->Close();


                if(Transaction->InTransaction)
		   Transaction->Commit();
LoadStockRequestsIntoTree();

      }

      }	catch (Exception &E)
		{}
}
//---------------------------------------------------------------------------

