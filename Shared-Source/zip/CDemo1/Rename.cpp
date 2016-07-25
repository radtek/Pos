#include <vcl.h>
#pragma hdrstop

#include "Rename.h"
#include "ZBCompat.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TRenForm *RenForm;

//---------------------------------------------------------------------------
__fastcall TRenForm::TRenForm( TComponent *Owner ) : TForm( Owner )
{
  ZipRenameList = new TList();
	StringGrid1->Cells[0][0] = "Old name";
	StringGrid1->Cells[1][0] = "New name";
	StringGrid1->Cells[2][0] = "Date/Time";
}
//---------------------------------------------------------------------------
__fastcall TRenForm::~TRenForm(void)
{
  ClearZipRenList();
	delete ZipRenameList;
}
//---------------------------------------------------------------------------
void __fastcall TRenForm::FormShow( TObject* /*Sender*/ )
{
	GDateTime = 0;
	StringGrid1->RowCount = 2;
	StringGrid1->Rows[1]->Clear();
	RenForm->Caption = "Rename items in zip archive: " + ExtractFileName( MainForm->ZipBuilder1->ZipFileName );
	OldCombo->Clear();
	ClearZipRenList();
	for (int i = 1; i <= MainForm->ZipBuilder1->Count; i++ )
  {
		ZipDirEntry *zde = (ZipDirEntry *)MainForm->ZipBuilder1->ZipContents->Items[i - 1];
		OldCombo->Items->Add( zde->FileName );
	}
}
//---------------------------------------------------------------------------
void __fastcall TRenForm::ClearZipRenList( void )
{
	ZipRenameRec *RenRec;

	for (int i = 0; i < ZipRenameList->Count; i++)
  {
		RenRec = (ZipRenameRec *)ZipRenameList->Items[i];
		delete RenRec;
	}
	ZipRenameList->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TRenForm::AddBtnClick( TObject* /*Sender*/ )
{
	ZipRenameRec *RenRec;
	String        AddItem;

	AddItem = OldCombo->Text;
	if(AddItem != NewEdit->Text || DTEdit->Text != "")
  {
		if(AddItem.Length() > 0 && NewEdit->Text.Length() > 0)
    {
			if(StringGrid1->RowCount > 2 || (StringGrid1->RowCount == 2 && StringGrid1->Cells[0][1] != "") )
				StringGrid1->RowCount++;
			RenRec = new ZipRenameRec;
			RenRec->Source   = AddItem;
			RenRec->Dest     = NewEdit->Text;
      try
      {
				if(DTEdit->Text != "")
					  RenRec->DateTime = DateTimeToFileDate(StrToDateTime(DTEdit->Text));
        else RenRec->DateTime = 0;
			}
      catch( ... )
      {
				delete RenRec;
				throw;
			}
			ZipRenameList->Add(RenRec);
			StringGrid1->Cells[0][StringGrid1->RowCount - 1] = AddItem;
			StringGrid1->Cells[1][StringGrid1->RowCount - 1] = NewEdit->Text;
			StringGrid1->Cells[2][StringGrid1->RowCount - 1] = DTEdit->Text;
      if(StringGrid1->RowCount == 2) StringGrid1->Cells[3][1] = "Y"; // default selected
      else StringGrid1->Cells[3][StringGrid1->RowCount - 1] = "N";
		}
		OldCombo->Text = "";
		NewEdit->Text  = "";
		DTEdit->Text   = "";
	}
}
//---------------------------------------------------------------------------
void __fastcall TRenForm::RemoveBtnClick( TObject* /*Sender*/ )
{
	ZipRenameRec *RenRec;
  for(int i = StringGrid1->RowCount -1; i > 0; --i)
  {
    if(StringGrid1->Cells[3][i] == "Y")  // is selected
    {
      RenRec = (ZipRenameRec *)ZipRenameList->Items[i-1];
      delete RenRec;
      ZipRenameList->Delete(i-1);
      StringGrid1->Rows[i]->Clear();
    }
  }
  // move empty rows to back
  for(int i = 1; i < StringGrid1->RowCount; ++i)
  {
    if(StringGrid1->Cells[0][i] =="")
    {
      for(int j = i; j < StringGrid1->RowCount -1; ++j) StringGrid1->Rows[j] = StringGrid1->Rows[j+1];
      StringGrid1->Rows[StringGrid1->RowCount -1]->Clear();
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TRenForm::CancelBitBtnClick( TObject* /*Sender*/ )
{
	Hide();
}
//---------------------------------------------------------------------------
void __fastcall TRenForm::OkBitBtnClick( TObject* Sender)
{
	int RenameErr;

	AddBtnClick(Sender);

	MsgForm->RichEdit1->Clear();
	MsgForm->Show();
	// Put this message into the message form's memo.
	MainForm->ZipBuilder1Message( this, 0, "Begin renaming entries in: " + MainForm->ZipBuilder1->ZipFileName );

	RenameErr = MainForm->ZipBuilder1->Rename( *ZipRenameList, GDateTime );
	if(RenameErr)
		ShowMessage( "Error " + IntToStr( RenameErr ) + " in rename zip specification(s)" );
	MsgForm->Hide();
	Hide();
}
//---------------------------------------------------------------------------
void __fastcall TRenForm::OldComboClick( TObject* /*Sender*/ )
{
	NewEdit->Text = OldCombo->Items->Strings[OldCombo->ItemIndex];
	NewEdit->SetFocus();
	NewEdit->SelStart = NewEdit->Text.Length();
}
//---------------------------------------------------------------------------
void __fastcall TRenForm::DTAllBtnClick(TObject* /*Sender*/)
{
	if ( DTEdit->Text != "" )
		GDateTime = DateTimeToFileDate(StrToDateTime(DTEdit->Text));
	else
		GDateTime = 0;
}
//---------------------------------------------------------------------------
void __fastcall TRenForm::StringGrid1DrawCell(TObject* /*Sender*/, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
//  UINT Format;
  if (State.Contains(gdFixed))  // for fixed cells
  {
    StringGrid1->Canvas->Brush->Color = clBtnFace;
    StringGrid1->Canvas->Font->Name = "Arial Black";
    StringGrid1->Canvas->Font->Size = 9;
    StringGrid1->Canvas->Font->Color = clBlack;
    StringGrid1->Canvas->FillRect(Rect);
    Frame3D(StringGrid1->Canvas, Rect, clBtnHighlight, clBtnShadow,1);
  }
  else // for normal cells
  {
    StringGrid1->Canvas->Brush->Color = (StringGrid1->Cells[3][ARow] == "Y") ? clHighlight : clWindow;
    StringGrid1->Canvas->Font = StringGrid1->Font;
    StringGrid1->Canvas->Font->Color = (StringGrid1->Cells[3][ARow] == "Y") ? clHighlightText :clBlack;
    StringGrid1->Canvas->FillRect(Rect);
  }
  RECT R = RECT(Rect);
  TCHAR* text = StringGrid1->Cells[ACol][ARow].T_STR();
  DrawText(StringGrid1->Canvas->Handle, text, StrLen(text), &R, DT_LEFT);
}
//---------------------------------------------------------------------------
void __fastcall TRenForm::StringGrid1SelectCell(TObject* /*Sender*/, int /*ACol*/,
      int ARow, bool& /*CanSelect*/)
{
  if(ARow) //1st  not allowed
  {    //  Select or deselect cell
    StringGrid1->Cells[3][ARow] = (StringGrid1->Cells[3][ARow]== "N") ? "Y" : "N";
  }
  for(int i = 0; i < StringGrid1->ColCount; ++i) //forces repaint of whole row
    StringGrid1->Cells[i][ARow]= StringGrid1->Cells[i][ARow];
}
//---------------------------------------------------------------------------

