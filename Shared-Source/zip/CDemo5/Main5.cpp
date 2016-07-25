#include <vcl.h>
#pragma hdrstop

#include "Main5.h"
#include "ZBCompat.h"
#pragma package( smart_init )

#pragma link "ZipBuilder"
#pragma resource "*.dfm"

TMainForm *MainForm;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm( TComponent *Owner ) : TForm( Owner )
{
  SelRect.Left = SelRect.Top = SelRect.Right = SelRect.Bottom = -1;//selection invisible
  StringGrid1->Selection = SelRect;
  DesignDir = GetCurrentDir();
	StringGrid1->RowCount     =   2;  // First row is fixed, and used for column headers.
	StringGrid1->Cells[0][0]  = "File Name";
	StringGrid1->Cells[1][0]  = "Compr. Size";
	StringGrid1->Cells[2][0]  = "Uncompr. Size";
	StringGrid1->Cells[3][0]  = "Date/Time";
  StringGrid1->ColWidths[0] = 310;
	StringGrid1->ColWidths[1] =  84;
	StringGrid1->ColWidths[2] =  94;
	StringGrid1->ColWidths[3] = 125;
  StringGrid1->ColWidths[4] = 0; // invisible column to hold selected value
	ZipBuilder1->Dll_Load = true;
}
//---------------------------------------------------------------------------
__fastcall TMainForm::~TMainForm(void)
{
  	ZipBuilder1->Dll_Load = false;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::CloseButClick(TObject* /*Sender*/)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::NewZipButClick(TObject* /*Sender*/)
{
	OpenDialog1->Title = "Create New ZIP File";
	OpenDialog1->Options << ofHideReadOnly << ofShareAware;
	OpenDialog1->Options >> ofPathMustExist >> ofFileMustExist >> ofExtensionDifferent;
	OpenDialog1->FileName   = "";
	OpenDialog1->DefaultExt = "exe";
	OpenDialog1->Filter     = "EXE Files (*.EXE)|*.exe";
	if(!OpenDialog1->Execute()) return;
	String FileName = OpenDialog1->FileName;

	if(OpenDialog1->Options.Contains(ofExtensionDifferent))
  {
		ShowMessage( "Error: your new archive must end in .EXE" );
		return;
	}
	if(FileExists(FileName))
  {
		if(MessageDlg( "Overwrite Existing File: " + FileName + "?",
				mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
			DeleteFile(FileName);
		else return;	// Don't use the new name.
	}
	SetZipFName(FileName);

  if(MessageDlg("Protect the archive with a password?",
	 	   	mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
  {
		ZipBuilder1->Password = ZipBuilder1->GetAddPassword();
		if(ZipBuilder1->Password == "" )
    {
			// The 2 password's entered by user didn't match.
			// We'll give him/her one more try.
			if ( MessageDlg( "Do you want another chance to enter the password?",
					mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0 ) == mrYes )
      {
				ZipBuilder1->Password = ZipBuilder1->GetAddPassword();
				if(ZipBuilder1->Password == "") return;
			}
      else return;
		}
	 }
	ZipBuilder1->ZipFileName = FileName;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DeleteZipButClick(TObject* /*Sender*/)
{
	if(FileExists( ZipBuilder1->ZipFileName))
  {
		if(MessageDlg( "Are you sure you want to delete: " + ZipBuilder1->ZipFileName + "?",
				mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0 ) == mrYes ) {
			if(DeleteFile(ZipBuilder1->ZipFileName))
      {
				SetZipFName( "<none>" );
				ZipBuilder1->ZipFileName = "";
			}
		}
	}
  else ShowMessage( "Zip file not found: " + ZipBuilder1->ZipFileName );
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ZipBuilder1DirUpdate(TObject* /*Sender*/)
{
	FillGrid();
	FilesLabel->Caption = IntToStr(ZipBuilder1->Count);
	SetZipFName(ZipBuilder1->ZipFileName);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FillGrid( void )
{
	// Remove everything from grid except col titles.
  for(int i = 1; i < StringGrid1->RowCount; ++i) StringGrid1->Rows[i]->Clear();
	StringGrid1->RowCount = 2;
	if(!ZipBuilder1->Count) return;
	StringGrid1->RowCount = ZipBuilder1->Count + 1;
	for(int i = 1; i < StringGrid1->RowCount; i++ )
  {
    ZipDirEntry* zde = ZipBuilder1->DirEntry[i - 1];
		StringGrid1->Cells[0][i] = zde->FileName;
		StringGrid1->Cells[1][i] = IntToStr( (int)zde->CompressedSize );
		StringGrid1->Cells[2][i] = IntToStr( (int)zde->UncompressedSize );
		StringGrid1->Cells[3][i] = FormatDateTime( "ddddd  t", FileDateToDateTime(zde->DateTime));
    StringGrid1->Cells[4][i] = "N"; // not selected
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::AddButClick(TObject* /*Sender*/)
{
	String IsOne;

	// In this program, the first Add will cause the SFX code to be added,
	// and will make the SFX control block.  So, we need to set the SFX
	// properties now!
	ZipBuilder1->SFXDefaultDir = "C:\\";	// default extract directory.

	if ( ZipBuilder1->ZipFileName == "" )
  {
		ShowMessage( "Error - create a new archive first" );
		return;
	}
	AddFilesForm->Left   = Left;
	AddFilesForm->Top    = Top;
	AddFilesForm->Width  = Width;
	AddFilesForm->Height = Height;
	Cancelled = false;
	AddFilesForm->ShowModal();	// Let user pick filenames to add.
	if(Cancelled) return;
	if(AddFilesForm->SelectedList->Items->Count == 0)
  {
		ShowMessage( "No files selected" );
		return;
	}
	ZipBuilder1->Verbose = false;
	ZipBuilder1->Trace   = false;
	ZipBuilder1->AddOptions.Clear();
	if(ZipBuilder1->Password > "")
		ZipBuilder1->AddOptions = ZipBuilder1->AddOptions << AddEncrypt;	// We want a password.
	ZipBuilder1->FSpecArgs->Clear();
	ZipBuilder1->FSpecArgs->Assign( AddFilesForm->SelectedList->Items );		// Specify filenames.
	AddFilesForm->SelectedList->Clear();
  SetCurrentDir(DesignDir); // allow relative filepaths in object inspector
	try
  {
		ZipBuilder1->Add();
	}
  catch ( ... )
  {
		ShowMessage( "Error in Add; Fatal Exception in mainunit" );
		return;
	}
	if(ZipBuilder1->SuccessCnt == 1) IsOne = " was";
	else IsOne = "s were";
	ShowMessage(IntToStr(ZipBuilder1->SuccessCnt) + " file" + IsOne + " added" );
}
//---------------------------------------------------------------------------
// This is the "OnMessage" event handler
void __fastcall TMainForm::ZipBuilder1Message(TObject* /*Sender*/, int ErrCode, String Message)
{
	if(ErrCode > 0) ShowMessage("Error Msg from DLL: " + Message);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DeleteButClick(TObject* /*Sender*/)
{
	if(ZipBuilder1->Count < 1)
  {
		ShowMessage( "Error - no files to delete" );
		return;
	}
	if(MessageDlg( "Delete selected files from: " + ZipBuilder1->ZipFileName + "?",
			mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0 ) != mrYes) return;
	ZipBuilder1->FSpecArgs->Clear();
  for(int i = 1; i < StringGrid1->RowCount; i ++)
    if(StringGrid1->Cells[4][i] == "Y") ZipBuilder1->FSpecArgs->Add( StringGrid1->Cells[0][i] );
	if(ZipBuilder1->FSpecArgs->Count < 1)
  {
		ShowMessage( "Error - no files selected" );
		return;
	}
	ZipBuilder1->Verbose = false;
	ZipBuilder1->Trace   = false;
 	try
  {
		ZipBuilder1->Delete();
	}
  catch ( ... )
  {
		ShowMessage( "Fatal error trying to delete" );
		return;
	}
	String IsOne = (ZipBuilder1->SuccessCnt == 1) ? " was" : "s were";
	ShowMessage( IntToStr( ZipBuilder1->SuccessCnt ) + " file" + IsOne + " deleted" );
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SetZipFName( String aCaption )
{
	ZipFName->Caption = MinimizeName( aCaption, ZipFName->Canvas, ZipFName->Width );
	if(ZipFName->Canvas->TextWidth(aCaption) > ZipFName->Width)
  {
		ZipFName->Hint         = aCaption;
		ZipFName->ShowHint     = true;
	} else ZipFName->ShowHint = false;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::StringGrid1DrawCell(TObject* /*Sender*/, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
  if (State.Contains(gdFixed))  // for fixed cells
  {
    StringGrid1->Canvas->Brush->Color = clBtnFace;
    StringGrid1->Canvas->Font->Name = "Arial Black";
    StringGrid1->Canvas->Font->Size = 8;
    StringGrid1->Canvas->Font->Color = clBlack;
    StringGrid1->Canvas->FillRect(Rect);
    Frame3D(StringGrid1->Canvas, Rect, clBtnHighlight, clBtnShadow,1);
  }
  else // for normal cells
  {
    StringGrid1->Canvas->Brush->Color = (StringGrid1->Cells[4][ARow] == "Y") ? clHighlight : clWindow;
    StringGrid1->Canvas->Font = StringGrid1->Font;
    StringGrid1->Canvas->Font->Color = (StringGrid1->Cells[4][ARow] == "Y") ? clHighlightText :clBlack;
    StringGrid1->Canvas->FillRect(Rect);
  }
  UINT Format = (ACol == 0) ? DT_LEFT : DT_RIGHT;
  RECT R = RECT(Rect);
  TCHAR* text = StringGrid1->Cells[ACol][ARow].T_STR();
  DrawText(StringGrid1->Canvas->Handle, text, StrLen(text), &R, Format);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::StringGrid1SelectCell(TObject* /*Sender*/, int /*ACol*/,
      int ARow, bool& /*CanSelect*/)
{
  if(ARow != 0) //1st  not allowed
  {    //  Select or deselect cell
    StringGrid1->Cells[4][ARow] = (StringGrid1->Cells[4][ARow]== "N") ? "Y" : "N";
  }
  for(int i = 0; i < StringGrid1->ColCount; ++i) //forces repaint of whole row
    StringGrid1->Cells[i][ARow]= StringGrid1->Cells[i][ARow];
}
//---------------------------------------------------------------------------

