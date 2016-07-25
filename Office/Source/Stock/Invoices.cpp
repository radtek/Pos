//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Invoices.h"
#include "StockData.h"
#include "Utilities.h"
#include "SelectStockItem.h"
#include "Connections.h"
#include "SelectDate.h"
#include "StockReportData.h"
#include "Reports.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TfrmInvoices *frmInvoices;
//---------------------------------------------------------------------------
__fastcall TfrmInvoices::TfrmInvoices(TComponent* Owner)
: TForm(Owner),
frmReceiveStockItem(new TfrmReceiveStockItem(NULL)),
frmReceiveInvoice(new TfrmReceiveInvoice(NULL))
{
	//	LastReference = "";
}
//---------------------------------------------------------------------------
void __fastcall TfrmInvoices::FormShow(TObject *Sender)
{
	FormResize(NULL);
	if (!Transaction->InTransaction) Transaction->StartTransaction();
	qrContact->Close();
	qrContact->ParamByName("CONTACT_LK")->AsString = ContactKey;
	qrContact->Open();
	lbeTitle->Caption = qrContact->FieldByName("Company_Name")->AsString;

	LoadTree();
	if (tvInvoices->Items->Count > 0)
	{
		if (tvInvoices->Items->Item[0]->Count > 0)
		{
			tvInvoices->Selected = tvInvoices->Items->Item[0]->getFirstChild();
			tvInvoices->Items->Item[0]->Expand(false);
		}
		else
		{
			tvInvoices->Selected = tvInvoices->Items->Item[0];
			tvInvoices->Selected->Expand(false);
			lbeReference->Caption = "";
		}
	}
	else
	{
		lbeReference->Caption = "";
	}
	tvInvoices->SetFocus();
    frmReceiveInvoice->IsSavedPackingSlip = false;
}
//---------------------------------------------------------------------------
void TfrmInvoices::LoadTree()
{
	tvInvoices->Items->BeginUpdate();
	tvInvoices->OnChange = NULL;
	tvInvoices->Items->Clear();
	qrInvoice->Close();

	TTreeNode *DateNode			= NULL;
	TTreeNode *ReferenceNode	= NULL;

	int LastMonth = 0, LastYear = 0;

	for (qrInvoice->Open(); !qrInvoice->Eof; qrInvoice->Next())
	{
		if (qrInvoice->FieldByName("Trans_Month")->AsInteger != LastMonth || qrInvoice->FieldByName("Trans_Year")->AsInteger != LastYear)
		{
			LastMonth							= qrInvoice->FieldByName("Trans_Month")->AsInteger;
			LastYear								= qrInvoice->FieldByName("Trans_Year")->AsInteger;
			TDateTime Date						= EncodeDate(LastYear, LastMonth, 1);

			DateNode								= tvInvoices->Items->Add(NULL, FormatDateTime("mmmm, yyyy", Date));
			DateNode->ImageIndex				= ICON_CLOSED_FOLDER_INDEX;
			DateNode->SelectedIndex			= ICON_OPEN_FOLDER_INDEX;
			DateNode->Data						= (void *)NULL;
		}
		ReferenceNode							= tvInvoices->Items->AddChild(DateNode, qrInvoice->FieldByName("Reference")->AsString);
		ReferenceNode->ImageIndex			= ICON_NOTEPAD_INDEX;
		ReferenceNode->SelectedIndex		= ICON_NOTEPAD_INDEX;
		ReferenceNode->Data					= (void *)qrInvoice->FieldByName("Trans_Number")->AsInteger;
	}
	tvInvoices->Items->EndUpdate();
	tvInvoices->OnChange = tvInvoicesChange;
}
//---------------------------------------------------------------------------
void __fastcall TfrmInvoices::FormClose(TObject *Sender,
TCloseAction &Action)
{
	if (Transaction->InTransaction) Transaction->Commit();
	FindDialog->CloseDialog();
}
//---------------------------------------------------------------------------
void __fastcall TfrmInvoices::FormResize(TObject *Sender)
{
	ResizeGrids();
}
//---------------------------------------------------------------------------
void __fastcall TfrmInvoices::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(NULL);
}
//---------------------------------------------------------------------------
void TfrmInvoices::ResizeGrids()
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
void __fastcall TfrmInvoices::btnCloseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmInvoices::btnNewInvoiceClick(TObject *Sender)
{
	AnsiString NewInvoiceRef = "";
    frmReceiveInvoice->IsSavedPackingSlip = false;

	NewInvoiceRef = InputBox("New Invoice", "Enter the invoice reference.", "");
	if (NewInvoiceRef != "")
	{
		if (InvoiceExists(NewInvoiceRef))
		{
			Application->MessageBox("This invoice has already been entered.",
			"Error",
			MB_OK | MB_ICONERROR);
			return;
		}

		frmReceiveInvoice->OrderKey			= 0;
		frmReceiveInvoice->SupplierKey		= ContactKey;
		frmReceiveInvoice->InvoiceReference	= NewInvoiceRef;
        InvoiceRef=   NewInvoiceRef;
        frmReceiveInvoice->IsPurchaseOrderMode = false;
		if (frmReceiveInvoice->ShowModal() == mrOk)
		{
			try
			{
				qrInvoice->DisableControls();
				if (Transaction->InTransaction) Transaction->Commit();
				FormShow(NULL);
			}
			__finally
			{
				qrInvoice->EnableControls();
				ResizeGrids();
			}
		}
	}
	tvInvoices->SetFocus();
}
//---------------------------------------------------------------------------
bool TfrmInvoices::InvoiceExists(AnsiString InvoiceRef)
{
	qrInvoiceExists->Close();
	qrInvoiceExists->ParamByName("COMPANY_NAME")->AsString   = qrContact->FieldByName("COMPANY_NAME")->AsString;
	qrInvoiceExists->ParamByName("REFERENCE")->AsString      = InvoiceRef;
	qrInvoiceExists->Open();
	bool RetVal = (qrInvoiceExists->Fields->Fields[0]->AsInteger != 0);
	qrInvoiceExists->Close();
	return RetVal;
}
//---------------------------------------------------------------------------
void __fastcall TfrmInvoices::btnUpdateInvoiceClick(TObject *Sender)
{
    AnsiString newInvoiceRef = "";
    bool RetVal;
    bool isInvoiceEditable = false;
	if (qrInvoice->IsEmpty())
	{
		Application->MessageBox("There is no invoice to update.\r"
		"To enter a new invoice, click 'New Invoice'.",
		"Error",
		MB_OK | MB_ICONERROR);
		return;
	}
	if (tvInvoices->Selected && tvInvoices->Selected->Level > 0 && qrInvoice->Locate("Trans_Number", int(tvInvoices->Selected->Data), TLocateOptions()))
	{
		AnsiString transactionType = qrInvoiceDetail->FieldByName("TRANSACTION_TYPE")->AsString;

		if(transactionType != "Packing Slip")
		{
			Application->MessageBox("Please select a packing slip to update, invoice can not be updated.\r",
			"Error",
			MB_OK | MB_ICONERROR);
			return;
		}

		if (Application->MessageBox("This invoice has already been entered. Are you sure you want to update it?","Error",MB_OKCANCEL | MB_ICONQUESTION) == ID_OK)
		{

             if (qrIsCommitted->Database->Connected && !qrIsCommitted->Transaction->InTransaction)
            {
                qrIsCommitted->Transaction->StartTransaction();
            }
            qrIsCommitted->Close();
            qrIsCommitted->ParamByName("ISCOMMITTED_PACKINGSLIP")->AsString = "T";
            qrIsCommitted->ParamByName("SUPPLIER_KEY")->AsInteger = ContactKey;
            qrIsCommitted->ParamByName("REFERENCE")->AsString = qrInvoice->FieldByName("Reference")->AsString;
            qrIsCommitted->Open();
            RetVal = (qrIsCommitted->Fields->Fields[0]->AsString != "");
            qrIsCommitted->Close();

            if(!RetVal)
            {
                newInvoiceRef = qrInvoice->FieldByName("Reference")->AsString;
               	if(InputQuery("Update Invoice #", "Update the invoice reference.", newInvoiceRef))
                {
                    newInvoiceRef = newInvoiceRef.Trim();
                    if(newInvoiceRef != "")
                    {
                       isInvoiceEditable = true;
                    }
                    else
                    {
                       isInvoiceEditable = false;
                    }
                }
            }
            else
            {
                if(Application->MessageBox("Update Invoice ", "Update the invoice reference.", MB_YESNO + MB_ICONQUESTION) == ID_YES)
                {
                    isInvoiceEditable = true;
                    newInvoiceRef = qrInvoice->FieldByName("Reference")->AsString;
                }
            }    
            if(isInvoiceEditable)
            {
                if (newInvoiceRef != "")
                {
                    if(newInvoiceRef != qrInvoice->FieldByName("Reference")->AsString)
                    {
                        if (InvoiceExists(newInvoiceRef))
                        {
                            Application->MessageBox("This invoice has already been entered.",
                            "Error",
                            MB_OK | MB_ICONERROR);
                            return;
                        }
                    }
                }

                frmReceiveInvoice->OrderKey					= 0;
                frmReceiveInvoice->SupplierKey				= ContactKey;
                frmReceiveInvoice->InvoiceReference			= newInvoiceRef;
                InvoiceRef= newInvoiceRef;
                frmReceiveInvoice->dtpInvoiceDate->Date	= int(qrInvoice->FieldByName("Created")->AsDateTime);
                frmReceiveInvoice->dtpInvoiceDate->Time	= qrInvoice->FieldByName("Created")->AsDateTime - frmReceiveInvoice->dtpInvoiceDate->Date;
                frmReceiveInvoice->IsPackingSlipUpdateMode = (transactionType == "Packing Slip");
                frmReceiveInvoice->BatchKey = (qrInvoiceDetail->FieldByName("BATCH_KEY")->AsInteger);


             qrSupplier->Close();
            qrSupplier->ParamByName("Supplier_Key")->AsInteger = ContactKey;
            qrSupplier->Open();
             frmReceiveInvoice->SupplierName=qrSupplier->FieldByName("Company_Name")->AsString;



            if(RetVal)
            {
                frmReceiveInvoice->IsSavedPackingSlip = true;
            }
            else
            {
                frmReceiveInvoice->IsSavedPackingSlip = false;
            }

                if (frmReceiveInvoice->ShowModal() == mrOk)
                {
                    try
                    {
                        qrInvoice->DisableControls();
                        if (Transaction->InTransaction) Transaction->Commit();
                        FormShow(NULL);
                    }
                    __finally
                    {
                        qrInvoice->EnableControls();
                        ResizeGrids();
                    }
                }
            }
	    }
    }
	tvInvoices->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TfrmInvoices::qrInvoiceDetailAfterScroll(TDataSet *DataSet)
{
	((TFloatField *)qrInvoiceDetail->FieldByName("UNIT_COST"))->currency = true;
	((TFloatField *)qrInvoiceDetail->FieldByName("TOTAL_COST"))->currency = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmInvoices::qrInvoiceTotalAfterScroll(TDataSet *DataSet)
{
	//((TFloatField *)DataSet->FieldByName("TOTAL_COST"))->currency = true;
	//((TFloatField *)DataSet->FieldByName("TOTAL_COST"))->currency = true;
	//((TFloatField *)DataSet->FieldByName("TOTAL_GST"))->currency = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmInvoices::tvInvoicesChange(TObject *Sender,
TTreeNode *Node)
{
	if (Node->Level == 1)// && Node->Parent->AbsoluteIndex != 0)
	{
		qrInvoiceDetail->Close();
		qrInvoiceTotal->Close();
		if (qrInvoice->Locate("Trans_Number", int(Node->Data), TLocateOptions()))
		{
			lbeReference->Caption = "Reference: " + qrInvoice->FieldByName("Reference")->AsString + " - " + FormatDateTime("d mmmm yyyy", qrInvoice->FieldByName("Created")->AsDateTime);
             InvoiceRef= qrInvoice->FieldByName("Reference")->AsString ;
          	qrInvoiceDetail->Open();
			((TFloatField *)qrInvoiceDetail->FieldByName("UNIT_COST" ))->currency = true;
			((TFloatField *)qrInvoiceDetail->FieldByName("TOTAL_COST"))->currency = true;
			((TFloatField *)qrInvoiceDetail->FieldByName("TOTAL_COST"))->DisplayFormat = "####0.00";
			((TFloatField *)qrInvoiceDetail->FieldByName("UNIT_COST" ))->DisplayFormat = "####0.00";
             BatchKeyVal= qrInvoiceDetail->FieldByName("BATCH_KEY")->AsString ;


			qrInvoiceTotal->Open();
			((TFloatField *)qrInvoiceTotal->FieldByName("TOTAL_COST"     ))->currency = true;
			((TFloatField *)qrInvoiceTotal->FieldByName("TOTAL_COST_INCL"))->currency = true;
			((TFloatField *)qrInvoiceTotal->FieldByName("TOTAL_GST"      ))->currency = true;
			((TFloatField *)qrInvoiceTotal->FieldByName("TOTAL_COST"     ))->DisplayFormat = "#####0.00";
			((TFloatField *)qrInvoiceTotal->FieldByName("TOTAL_COST_INCL"))->DisplayFormat = "#####0.00";
			((TFloatField *)qrInvoiceTotal->FieldByName("TOTAL_GST"      ))->DisplayFormat = "#####0.00";
			//lbeGST->Caption = FloatToStrF(qrInvoiceTotal->FieldByName("TOTAL_INVOICE_COST")->AsFloat + qrInvoiceTotal->FieldByName("TOTAL_INVOICE_TAX")->AsFloat, ffCurrency, 19, CurrencyDecimals);
			ResizeGrids();

			//As per story 3 (Packing Slip) Update button will be enabled only for Packing Slips.
			AnsiString transactionType = qrInvoiceDetail->FieldByName("TRANSACTION_TYPE")->AsString;
			TransactionNumber = qrInvoiceDetail->FieldByName("TRANS_NUMBER" )->AsInteger;

			if(transactionType == "Packing Slip")
			{
				btnUpdateInvoice->Enabled = true;
                btnPrintInvoice->Enabled = false;
			}
			else
			{
				btnUpdateInvoice->Enabled = false;
                  btnPrintInvoice->Enabled = true;
			}
		}
		else
		{
			qrInvoiceDetail->Close();
			qrInvoiceTotal->Close();
			lbeReference->Caption	= "";
			//lbeGST->Caption			= "";
		}
	}
	else
	{
		qrInvoiceDetail->Close();
		qrInvoiceTotal->Close();
		lbeReference->Caption	= "";
		//lbeGST->Caption			= "";
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmInvoices::FindDialogFind(TObject *Sender)
{
	TTreeNode *CurItem;
	if (tvInvoices->Selected)
	{
		if (FindDialog->Options.Contains(frDown))
		{
			CurItem = tvInvoices->Selected->GetNext();
		}
		else
		{
			CurItem = tvInvoices->Selected->GetPrev();
		}
	}
	else
	{
		CurItem = tvInvoices->Items->GetFirstNode();
	}
	while (CurItem)
	{
		AnsiString FindText, NodeText;
		if (FindDialog->Options.Contains(frMatchCase))
		{
			FindText = FindDialog->FindText;
			NodeText = CurItem->Text;
		}
		else
		{
			FindText = FindDialog->FindText.UpperCase();
			NodeText = CurItem->Text.UpperCase();
		}
		if (FindDialog->Options.Contains(frWholeWord))
		{
			if (FindText == NodeText)
			{
				tvInvoices->Selected = CurItem;
				break;
			}
		}
		else
		{
			if (NodeText.AnsiPos(FindText))
			{
				tvInvoices->Selected = CurItem;
				break;
			}
		}
		if (FindDialog->Options.Contains(frDown))
		{
			CurItem = CurItem->GetNext();
		}
		else
		{
			CurItem = CurItem->GetPrev();
		}
	}
	if (!CurItem)
	{
		Application->MessageBox("No more occurances found.", "Search Complete", MB_ICONINFORMATION + MB_OK);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmInvoices::btnFindClick(TObject *Sender)
{
	FindDialog->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TfrmInvoices::tvInvoicesDblClick(TObject *Sender)
{
	btnUpdateInvoiceClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmInvoices::tvInvoicesKeyPress(TObject *Sender,
char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
		tvInvoicesDblClick(Sender);
	}
}
//---------------------------------------------------------------------------

/*SELECT
	CODE,
	DESCRIPTION,
	LOCATION,
	ORDER_QTY,
	ORDER_UNIT,
	QTY,
	UNIT_COST,
	UNIT,
	(UNIT_COST*QTY) TOTAL_COST,
	(CAST(GST_PERCENT AS DOUBLE PRECISION)*UNIT_COST*QTY/100) TOTAL_GST
FROM
	STOCKTRANS
WHERE
	Supplier_Key = :Supplier_Key And
	REFERENCE = :REFERENCE
ORDER BY
	DESCRIPTION, LOCATION */

/*
SELECT
	SUM (CAST(UNIT_COST AS DOUBLE PRECISION)*GST_PERCENT*QTY/100) O_TOTAL_INVOICE_TAX,

	Sum (Total_GST) TOTAL_INVOICE_TAX,
	SUM (UNIT_COST*QTY) TOTAL_INVOICE_COST
FROM
	STOCKTRANS
WHERE
	Supplier_Key = :Supplier_Key AND
	REFERENCE = :REFERENCE
ORDER BY
	DESCRIPTION
*/
void __fastcall TfrmInvoices::btnPrintInvoiceClick(TObject *Sender)
{
        qrSupplier->ParamByName("Supplier_Key")->AsInteger = ContactKey;
        qrSupplier->Open();
        AnsiString supplierName=qrSupplier->FieldByName("Company_Name")->AsString;
        AnsiString refNo =InvoiceRef;
        AnsiString receiptNumber =BatchKeyVal;

        
            if (dmStockReportData->StockTrans->DefaultDatabase->Connected && !dmStockReportData->StockTrans->InTransaction)
            {
              dmStockReportData->StockTrans->StartTransaction();
            }
         	dmStockReportData->SetupSupplierPurchasesRuntime(receiptNumber, supplierName, refNo);;
			   if (frmReports->rvStock->SelectReport("repSupplierPurchaseStock", false))
            {
               frmReports->rvStock->Execute();
            }

            if (dmStockReportData->StockTrans->DefaultDatabase->Connected)
		{
			dmStockReportData->StockTrans->Commit();
		}
}

//---------------------------------------------------------------------------

