//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <memory>
#include "Excel.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "OpExcel"
#pragma link "OpShared"
#pragma link "OpXLXP"
#pragma link "OpDbOfc"
#pragma link "OpModels"
#pragma link "OpExcel"
#pragma link "OpOlkXP"
#pragma link "OpOutlk"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TfrmExcel::TfrmExcel(TComponent* Owner)	: TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TfrmExcel::OpExcelWindowDeactivate(TObject *Sender,
		const _Workbook *Wb, const ExcelWindow *Wn)
{
	// tidy up also on Excel Deactivate
	OpExcel->Visible   = false;
	OpExcel->Connected = false;
	this->Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmExcel::btnCloseExcelClick(TObject *Sender)
{
	// if they close the Form before Excel
	OpExcel->Visible   = false;
	OpExcel->Connected = false;
	this->Close();
}
//---------------------------------------------------------------------------
void TfrmExcel::ExportToExcel( TStringList *ExcelDataSets, AnsiString ACaption)
{
		try
		{
			TCursor Save_Cursor = Screen->Cursor;
			OpExcel->WindowState = xlwsNormal;
			std::auto_ptr<TOpDataSetModel> OpDataSetModel(new TOpDataSetModel(NULL));
			try
			{
				Screen->Cursor = crHourGlass;
			  //	OpDataSetModel->Dataset = NULL;

				OpExcel->Connected = true;
				OpExcel->Visible   = true;
				// always one entry in the stringList
				OpExcel->Caption   = CurrentConnection.CompanyName + " - " + ACaption;
				TDataSet *DataSet  = (TDataSet *)ExcelDataSets->Objects[0];
				DataSet->Open();

				TOpExcelWorkbook *oWorkBook = OpExcel->Workbooks->Add();
				// WorkSheet Name
				oWorkBook->Worksheets->Items[0]->Name = ExcelDataSets->Strings[0];
				TOpExcelRange *oRange       = oWorkBook->Worksheets->Items[0]->Ranges->Add();
				// locate range
				oRange->Address             = "A1";
				oRange->OfficeModel         = OpDataSetModel.get();
				OpDataSetModel->Dataset     = DataSet;
                    try{oRange->Populate();}
                  catch( ... )
    {
    }


				//AddChart( oWorkBook ,DataSet,ACaption);

				// for each other dataset, add a new workSheet
				for (int i=1; i<ExcelDataSets->Count; i++)
				{
					AddWorkSheet( (TDataSet *)ExcelDataSets->Objects[i],ExcelDataSets->Strings[i] );
				}
				Screen->Cursor              = Save_Cursor;
				OpExcel->Interactive        = True;
				DataSet->Close();
				OpExcel->WindowState = xlwsMaximized;
			}
			__finally
			{
				Screen->Cursor = Save_Cursor;
			}
		}
		catch (Sysutils::Exception &E)
		{
			// Excel may not be installed
								Application->MessageBox(AnsiString(E.Message).c_str(), "Exception",
				MB_ICONERROR + MB_OK);
				
			Application->ShowException(&E);

		}
}
//---------------------------------------------------------------------------
  //TOpXlChartType = (xlctArea, xlctBar, xlctLine, xlctPie,
  //xlctRadar, xlctXYScatter, xlct3DArea, xlct3DBar,
 // xlct3DLine, xlct3DPie, xlct3DColumn, xlctDoughnut,
void TfrmExcel::AddChart( TOpExcelWorkbook *oWorkBook ,TDataSet *DataSet, AnsiString ACaption)
{
  // may need recordCount
  int RecordCount = DataSet->RecordCount;
  TOpExcelChart *Chart = oWorkBook->Worksheets->Items[0]->Charts->Add();

  Chart->Left   = 10;
  Chart->Width  = 900;
  Chart->Height = 550;
  Chart->ChartType   = xlct3DColumn;
  Chart->DisplayName =  ACaption;

  //Tell the chart where to find the data
  Chart->DataRange = "K1:L" + IntToStr(RecordCount);

  // the data must be in consectutive rows eg Name Qty Count Sales
  // to make it load in Excel and create the Series
}
//---------------------------------------------------------------------------

void TfrmExcel::AddWorkSheet( TDataSet *DataSet, AnsiString WorkSheetName)
{
		// used to add extra worksheets for reports that use multiple datasets
		TCursor Save_Cursor = Screen->Cursor;
		std::auto_ptr<TOpDataSetModel> OpDataSetModel(new TOpDataSetModel(NULL));
		try
		{
            try{
			Screen->Cursor = crHourGlass;
			DataSet->Open();
			// Add another worksheet to the WorkBook
			TOpExcelWorksheet *oWorkSheet = OpExcel->Workbooks->Items[0]->Worksheets->Add();
			oWorkSheet->Name            = WorkSheetName;
			TOpExcelRange *oRange       = oWorkSheet->Ranges->Add();
			oRange->Address             = "A1";             // locate range

			oRange->OfficeModel         = OpDataSetModel.get();   // assign the model to the range
			OpDataSetModel->Dataset     = DataSet;
			oRange->Populate();                             // fill the range from the model
			DataSet->Close();
			Screen->Cursor              = Save_Cursor;
            }
             catch( ... )
    {
    }
		}

		__finally
		{
			Screen->Cursor = Save_Cursor;
		}
}
//---------------------------------------------------------------------------
void TfrmExcel::Cobb()
{
	OpExcel->Connected = true;
	OpExcel->Visible   = true;
	OpExcel->Caption   = CurrentConnection.CompanyName;

	TOpExcelWorkbook* oWorkBook = OpExcel->Workbooks->Add();
	// this loads the file automatically
	oWorkBook->Filename = "c:\\code\\Fran.xls";
	// just need to create one range and vary its address
	TOpExcelRange* Rng = oWorkBook->Worksheets->Items[0]->Ranges->Add();

	 // Company Name
	 Rng->Address = "B5";
	 Rng->SimpleText = CurrentConnection.CompanyName;
	 // Week Ending Date
	 Rng->Address = "B7";
	 Rng->SimpleText = "11 March 2005";

	 Rng->Address    = "B12:H12";
	 Rng->SimpleText = "211.99";

	 Rng->Address    = "B13:H13";
	 Rng->SimpleText = "2111.99";

	 Rng->Address    = "B21:H21";
	 Rng->SimpleText = "3111.99";
	 Rng->Address    = "B22:H22";
	 Rng->SimpleText = "4111.99";

	 TOpExcelWorksheet* MappingWorkSheet = OpExcel->Workbooks->Items[0]->Worksheets->Items[1];
	 TOpExcelRange* MappingRng = MappingWorkSheet->Ranges->Add();
	 MappingRng->Address    = "A1:A3";
	 Variant s = MappingRng->Value;

	 // think its like a refresh

	 oWorkBook->Activate();

	 ShowMessage(VarToStr(s));




	 //OpExcel1->Workbooks->Items[0]->Worksheets->Items[0]->Activate();
	 //ShowMessage("Wait");

	//TOpExcelWorkbook *oWorkBook;// = new TOpExcelWorkbook;
	//TOpExcelWorksheet *oSheet;//   = new TOpExcelWorksheet;
  //	TOpExcelRange *oRange;//       = new TOpExcelRange;

	// oWorkBook = OpExcel1->Workbooks->Add;            // Existing workbook
	 // Check for FileExists
	// oWorkBook->FileName = "c:\code\Fran.xls";
	// oSheet    = oWorkBook->Worksheets[0]; // add a worksheet


	//OpExcel1->RangeByName["Chris"]->Clear();
	//OpExcel1->RangeByName["Chris"]->Address = "B12";
	//OpExcel1->RangeByName["Chris"]->SimpleText = 1000.99;

}
//---------------------------------------------------------------------------
void TfrmExcel::SendEmail( AnsiString EmailFileName, AnsiString EmailAddress, AnsiString EmailSubject )
{
	TOpMailItem *myMailItem;
	try
	{
		if (Outlook->Connected == false)
		{
			Outlook->Connected = true;
		}
		myMailItem             = Outlook->CreateMailItem();
		myMailItem->MsgTo      = EmailAddress;
		myMailItem->Subject    = EmailSubject + " from " + CurrentConnection.CompanyName;
		//myMailItem->Body = not required 
		myMailItem->Attachments->Add(EmailFileName);
		myMailItem->Send();
	}
	__finally
	{
		delete myMailItem;
		if (Outlook->Connected)
		{
			Outlook->Connected = false;
		}
	}
}
//---------------------------------------------------------------------------



