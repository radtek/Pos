//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PreviewData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmPreviewData *frmPreviewData;
//---------------------------------------------------------------------------
__fastcall TfrmPreviewData::TfrmPreviewData(TComponent* Owner)	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
__fastcall TfrmPreviewData::TfrmPreviewData(TComponent* Owner, TIBQuery* theQuery,bool ShowForm ) : TForm(Owner)
{
	if (theQuery)
	{
		if (!theQuery->Active) theQuery->Open();
		if (theQuery->RecordCount != 0 )
		{
			ShowForm = true;
		}
		else
		{
			ShowForm = false;
			return;
		}	
		dsPreviewData->DataSet = theQuery;
		WindowState = wsMaximized;

	}
}
//---------------------------------------------------------------------------
