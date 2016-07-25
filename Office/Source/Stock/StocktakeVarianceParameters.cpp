//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "StocktakeVarianceParameters.h"
#include "StockReportData.h"
#include "Reports.h"
#include "Connections.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "NumericEdit"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TfrmVarianceParams::TfrmVarianceParams(TComponent* Owner)	: TForm(Owner)
{
	pnlInitialisation->Parent = Panel1;
	pnlVariance->Parent       = Panel1;
	pnlReports->Parent        = Panel1;
	pnlResetUndo->Parent      = Panel1;
	rgpReportType->Parent      = Panel1;
	this->Height = 252;
	this->Width  = 337;
}
//---------------------------------------------------------------------------
void TfrmVarianceParams::SetUpScreen(TVarianceParams VarianceParams)
{
	switch (VarianceParams)
	{
		case vInitialisation :
			  pnlInitialisation->Align  = alClient;
			  pnlInitialisation->Visible = true;
			  this->Caption   = "Stocktake Initialisation";
			  lblTop->Caption = "NB All MenuMate sales adjustments will be held until this stocktake is completed";
			  break;
		case vVarianceReport :
			  pnlVariance->Visible = true;
			  pnlVariance->Align   = alTop;

			  this->Height   = 357;
			  Panel2->Height = 314;
			  pnlVarianceParams->Visible = true;
			  pnlVarianceParams->Align   = alBottom;

			  this->Caption   = "Stocktake Report";
			  lblTop->Caption = "Preview Variance Report selection";
			  break;
		case vReports :
			  pnlReports->Visible = true;
			  pnlReports->Align   = alTop;
			  btnPreview->Visible  = true;
			  btnOk->Visible       = false;
			  btnCancel->Kind      = bkClose;
			  this->Caption   = "Stocktake - Commit Completed";
			  lblTop->Caption = "both reports are always available from the Reports/Stock screen";
			  if (fRadioButtonValue == 0)
			  {
				  this->Height   = 357;
				  Panel2->Height = 314;
				  pnlVarianceParams->Visible = true;
				  pnlVarianceParams->Align   = alBottom;
			  }
			  else
			  {
				  Panel2->Height = 209;
				  pnlVarianceParams->Visible = false;
				  this->Height   = 252;
			  }
			  break;
		case vResetUndo :
			  pnlResetUndo->Align       = alClient;
			  pnlResetUndo->Visible = true;
			  this->Caption   = "Stocktake Initialisation";
			  lblTop->Caption = "This stocktake is already initialised. What do you wish to do?";
			  break;
		case vReportType :
			  rgpReportType->Align       = alClient;
			  rgpReportType->Visible = true;
			  this->Caption   = "Report Export";
			  lblTop->Caption = "Choose a report type for exporting.";
			  break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmVarianceParams::neTotalBankingIncChange(
		TObject *Sender)
{
	neTotalBankingExcl->Value = neTotalBankingInc->Value / (1 + (Currency(CurrentConnection.DefaultMenuGST) / 100));
}
//---------------------------------------------------------------------------
void __fastcall TfrmVarianceParams::neTotalBankingIncKeyPress(
		TObject *Sender, char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
		SelectNext((TWinControl *)Sender, true, true);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmVarianceParams::RadioButton2Click(TObject *Sender)
{
	// all Radio Buttons look here
	fRadioButtonValue = ((TRadioButton *)Sender)->Tag;
	if (pnlVariance->Visible == true)
	{
		neVarianceAmount->Enabled = false;
		if (((TRadioButton *)Sender)->Name == rbSetVarianceAmount->Name)
			neVarianceAmount->Enabled = true;
	}
	if (pnlReports->Visible == true)
	{
		SetUpScreen(vReports);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmVarianceParams::btnPreviewClick(TObject *Sender)
{
	switch (fRadioButtonValue)
	{
		case 0 : dmStockReportData->SetupStockVariance(StocktakeKey);
					if (frmReports->rvStock->SelectReport("repStocktakeVariance", false))
					{
						frmReports->rvStock->SetParam("TotalBankingInc", Results().TotalBankingInc);
						frmReports->rvStock->SetParam("TotalBankingExcl", Results().TotalBankingExcl);
						frmReports->rvStock->SetParam("UserVariance", Results().UserVariance);
						frmReports->rvStock->SetParam("ReportRange", "");
						frmReports->rvStock->Execute();
					}
					break;
		case 1 : std::auto_ptr<TStringList> StringList(new TStringList);
					StringList->Add(IntToStr(StocktakeKey));
					dmStockReportData->SetupStocktakeStockValuation(StringList.get());
					if (frmReports->rvStock->SelectReport("repStockValueByLocation", false))
					{
						frmReports->rvStock->SetParam("Heading", "Stock Value by Stocktake");
						frmReports->rvStock->Execute();
					}
					break;
	}
}
//---------------------------------------------------------------------------





