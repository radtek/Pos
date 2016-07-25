#include <vcl\vcl.h>
#pragma hdrstop

#include "Main.h"
#include "ZBCompat.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ZipBuilder"
#pragma resource "*.dfm"

TMainForm *MainForm;

/*TMainForm::TMainForm----------------------------------------------------------
1.76 19 June 2004 RA removed loading of DLL's, gives AV if not possible to load
*/
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
	StringGrid1->RowCount     =   2;  // First and last row are fixed.
	StringGrid1->Cells[0][0]  = "File Name";
	StringGrid1->Cells[1][0]  = "Compr. Size";
	StringGrid1->Cells[2][0]  = "Uncompr. Size";
	StringGrid1->Cells[3][0]  = "Date/Time";
	StringGrid1->Cells[4][0]  = "Ratio";
	StringGrid1->Cells[5][0]  = "Path";
  //Make sure "goColMoving" is false.
  StringGrid1->Options = StringGrid1->Options >> goColMoving;
	/* If we had args on the cmd line, then try to open the first one
		as a zip/exe file.  This is most useful in case user has an association
		to ".zip" that causes this program to run when user double clicks on a zip
		file in Explorer. */
	if ( ParamCount > 0 ) ZipBuilder1->ZipFileName = ParamStr( 1 );
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormResize( TObject* /*Sender*/ )
{
  ZipFName->Width = Width - 291;
	SetZipFName( ZipBuilder1->ZipFileName, false );
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::CloseButClick( TObject* /*Sender*/ )
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ZipOpenButClick( TObject* /*Sender*/ )
{
	static String FirstDir = "";

	if ( FirstDir.IsEmpty()) GetSpecialFolder( CSIDL_DESKTOPDIRECTORY, FirstDir );
	OpenDialog1->InitialDir = FirstDir;
	OpenDialog1->Title      = "Open Existing ZIP File";
	OpenDialog1->Filter     = "ZIP Files (*.ZIP, *.EXE)|*.zip;*.exe";
	OpenDialog1->FileName   = "";
	OpenDialog1->Options << ofHideReadOnly << ofShareAware << ofPathMustExist << ofFileMustExist;
	if ( OpenDialog1->Execute() ) {
		FirstDir = ExtractFilePath( OpenDialog1->FileName );
		/* Set the caption after assigning the filename. This
		   way, the filename will be null if the open failed. */
		SetZipFName( OpenDialog1->FileName, true );
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::NewZipButClick( TObject* /*Sender*/ )
{
	static String FirstDir = "";

	if ( FirstDir == "" ) GetSpecialFolder( CSIDL_DESKTOPDIRECTORY, FirstDir );
	OpenDialog1->InitialDir = FirstDir;
	OpenDialog1->FileName   = "";
	OpenDialog1->Filter     = "ZIP Files (*.ZIP)|*.zip";
	OpenDialog1->DefaultExt = "Zip";
	OpenDialog1->Title      = "Create New ZIP File";
	OpenDialog1->Options << ofHideReadOnly << ofShareAware;
	OpenDialog1->Options >> ofPathMustExist >> ofFileMustExist;
	if ( OpenDialog1->Execute() ) {
		FirstDir = ExtractFilePath( OpenDialog1->FileName );
		if ( FileExists( OpenDialog1->FileName ) ) {
			bool Ans = MessageDlg( "Overwrite Existing File: " + OpenDialog1->FileName + "?",
                          mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0 ) == mrYes;
			if(Ans) DeleteFile(OpenDialog1->FileName);
			else return;  // Don't use the new name.
		}
		SetZipFName( OpenDialog1->FileName, true );
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DeleteZipButClick( TObject* /*Sender*/ )
{
	if(FileExists(ZipBuilder1->ZipFileName))
  {
		bool Ans = MessageDlg( "Are you sure you want to delete: " + ZipBuilder1->ZipFileName
            + "?", mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0 ) == mrYes;
		if(Ans)
    {
			DeleteFile(ZipBuilder1->ZipFileName);
			SetZipFName("", true);
		}
    else return;  // Don't use the new name.
	}
  else ShowMessage( "Zip file not found: " + ZipBuilder1->ZipFileName );
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ExtractButClick( TObject* /*Sender*/ )
{
   if(!FileExists(ZipBuilder1->ZipFileName))
   {
		ShowMessage( "Error: file not found: " + ZipBuilder1->ZipFileName );
		return;
	}
	Extract->ShowModal();
	if((ExtractDir.IsEmpty()) || Canceled) return;

	if(ZipBuilder1->Count < 1)
  {
		ShowMessage( "Error - no files to extract" );
		return;
	}
	ZipBuilder1->FSpecArgs->Clear();
	// Get fspecs of selected files, unless user wants all files extracted.
	if(!AllFiles)
  {
    for(int i = 1; i < StringGrid1->RowCount - 1; ++i)
    {
      if(StringGrid1->Cells[6][i] == "Y") // is selected
      ZipBuilder1->FSpecArgs->Add( StringGrid1->Cells[5][i] + StringGrid1->Cells[0][i]);
    }
		if(ZipBuilder1->FSpecArgs->Count < 1)
    {
			ShowMessage( "Error - no files selected" );
			return;
		}
	}
	MsgForm->RichEdit1->Clear();
	MsgForm->Show();
	// Put this message into the message form.
	ZipBuilder1Message( this, 0, "Beginning Extract from " + ZipBuilder1->ZipFileName );

	ZipBuilder1->ExtrBaseDir = ExtractDir;
	ZipBuilder1->ExtrOptions.Clear();
	if(ExpandDirs) ZipBuilder1->ExtrOptions << ExtrDirNames;
	if(OverWr) ZipBuilder1->ExtrOptions << ExtrOverWrite;
	s = ::GetTickCount();
	try
  {
		ZipBuilder1->Extract();
	}
  catch ( ... )
  {
		ShowMessage( "Error in Extract; Fatal DLL Exception in Main" );
		return;
	}
	f = ::GetTickCount();
	TimeLabel->Caption = ShowLTime( s, f );
	String IsOne = (ZipBuilder1->SuccessCnt == 1) ? " was" : "s were";
	ShowMessage( IntToStr( ZipBuilder1->SuccessCnt ) + " file" + IsOne + " extracted" );
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::AddButClick( TObject* /*Sender*/ )
{
	if(ZipBuilder1->ZipFileName.IsEmpty())
  {
		ShowMessage( "Error - open a zip file first" );
		return;
	}
	AddFile->Left   = Left;
	AddFile->Top    = Top;
	AddFile->Width  = Width;
	AddFile->Height = Height;
	Canceled = false;
	AddFile->ShowModal();  // Let user pick filenames to add.
	if(Canceled) return;
	if ( !AddFile->SelectedList->Items->Count ) {
		ShowMessage( "No files selected" );
		return;
	}
	MsgForm->RichEdit1->Clear();
	MsgForm->Show();
	// Put this message into the message form.
	ZipBuilder1Message( this, 0, "Beginning Add to " + ZipBuilder1->ZipFileName );

	/* We want any DLL error messages to show over the top
		of the message form. */
	ZipBuilder1->AddOptions.Clear();
	switch(AddFile->ZipAction)
  {	// Default is plain ADD.
		case 2:
			ZipBuilder1->AddOptions = ZipBuilder1->AddOptions << AddUpdate;   // Update
			break;
		case 3:
			ZipBuilder1->AddOptions = ZipBuilder1->AddOptions << AddFreshen;  // Freshen
			break;
		case 4:
			ZipBuilder1->AddOptions = ZipBuilder1->AddOptions << AddMove;     // Move
	}
	if ( AddFile->RecurseCB->Checked )					// We want recursion.
		ZipBuilder1->AddOptions = ZipBuilder1->AddOptions << AddRecurseDirs;
	if ( AddFile->DirNameCB->Checked )					// We want dirnames.
		ZipBuilder1->AddOptions = ZipBuilder1->AddOptions << AddDirNames;
	if ( AddFile->DiskSpanCB->Checked )					// We disk spanning.
		ZipBuilder1->AddOptions = ZipBuilder1->AddOptions << AddDiskSpan;
	if ( AddFile->EncryptCB->Checked ) {				// We want a password.
		ZipBuilder1->AddOptions = ZipBuilder1->AddOptions << AddEncrypt;
	}
	ZipBuilder1->FSpecArgs->Clear();
	ZipBuilder1->FSpecArgs->Assign( AddFile->SelectedList->Items ); // Specify filenames.
	AddFile->SelectedList->Clear();
	s = ::GetTickCount();
	try
  {
		ZipBuilder1->Add();
	}
  catch ( ... )
  {
		ShowMessage( "Error in Add; Fatal DLL Exception in Main" );
		return;
	}
	f = ::GetTickCount();
	TimeLabel->Caption = ShowLTime( s, f );
	String IsOne = (ZipBuilder1->SuccessCnt == 1) ? " was" : "s were";
	ShowMessage( IntToStr( ZipBuilder1->SuccessCnt ) + " file" + IsOne + " added" );
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::DeleteButClick( TObject* /*Sender*/ )
{
	if(ZipBuilder1->Count < 1)
  {
		ShowMessage( "Error - no files to delete" );
		return;
	}
	bool Ans = MessageDlg( "Delete selected files from: " +
			ZipBuilder1->ZipFileName + "?", mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0 ) == mrYes;
	if ( !Ans ) return;

	ZipBuilder1->FSpecArgs->Clear();
  for(int i = 1; i < StringGrid1->RowCount - 1; ++i)
  {
    if(StringGrid1->Cells[6][i] == "Y") // is selected
      ZipBuilder1->FSpecArgs->Add((StringGrid1->Cells[5][i] + StringGrid1->Cells[0][i]).TrimLeft());
  }
	if ( ZipBuilder1->FSpecArgs->Count < 1 )
  {
		ShowMessage( "Error - no files selected");
		return;
	}

	MsgForm->RichEdit1->Clear();
	MsgForm->Show();
	// Put this message into the message form.
	ZipBuilder1Message(this, 0, "Beginning delete from " + ZipBuilder1->ZipFileName);

	s = ::GetTickCount();
	try
  {
		ZipBuilder1->Delete();
	}
  catch ( ... )
  {
		ShowMessage( "Fatal error trying to delete" );
		return;
	}
	f = ::GetTickCount();
	TimeLabel->Caption = ShowLTime( s, f );
	String IsOne = (ZipBuilder1->SuccessCnt == 1) ? " was" : "s were";
	ShowMessage( IntToStr( ZipBuilder1->SuccessCnt ) + " file" + IsOne + " deleted" );
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::TestButClick( TObject* /*Sender*/ )
{
	if(ZipBuilder1->Count < 1)
  {
		ShowMessage( "Error - nothing to Test" );
		return;
	}
	if ( ZipBuilder1->ZipFileName.IsEmpty()) return;
	MsgForm->RichEdit1->Clear();
	MsgForm->Show();
	ZipBuilder1Message( this, 0, "Beginning test of " + ZipBuilder1->ZipFileName );

	ZipBuilder1->FSpecArgs->Clear();
	ZipBuilder1->ExtrOptions << ExtrTest;
	ZipBuilder1->FSpecArgs->Add( "*.*" ); // Test all the files in the .zip.
	// IMPORTANT: In this release, you must test all files.
	s = ::GetTickCount();
	ZipBuilder1->Extract();  // This will really do a test.

	f = ::GetTickCount();
	TimeLabel->Caption = ShowLTime( s, f );
	if ( ZipBuilder1->Count + ZipBuilder1->DirOnlyCount == ZipBuilder1->SuccessCnt )
		ShowMessage( "All " + IntToStr( ZipBuilder1->Count + ZipBuilder1->DirOnlyCount ) + " files tested OK" );
	else
		ShowMessage( "ERROR: " + IntToStr( ZipBuilder1->Count + ZipBuilder1->DirOnlyCount - ZipBuilder1->SuccessCnt ) +
                     " files tested BAD, or skipped!" );
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::MsgButClick( TObject* /*Sender*/ )
{
	MsgForm->Show();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ConvertButClick( TObject* /*Sender*/ )
{
	int ConvertErr;

	if ( !ZipBuilder1->Count )
  {
		ShowMessage( "Error: no files in archive" );
		return;
	}
	// Determine which conversion is to be done.
	if ( UpperCase( ExtractFileExt( ZipBuilder1->ZipFileName ) ) == ".EXE" )
  {
		// Convert .EXE to .ZIP
		if ( (ConvertErr = ZipBuilder1->ConvertZIP()) == 0 )
			ShowMessage( "Filename is now: " + ZipBuilder1->ZipFileName );
		else
			ShowMessage( "Error " + IntToStr( ConvertErr ) + " occured in making .ZIP file" );
	}
  else
  {
		// Convert .ZIP to .EXE
		/* NOTE: If you put the ZBSFX__.BIN file into the WINDOWS
			or WINDOWS SYSTEM dir, then you don't need to set the
			SFXPath property below: */
		// ZipBuilder1->SFXPath = "c:\\windows\\system\\zbsfxus.bin";
		MakeSFX->ShowModal();
		if ( DoIt == false ) return;
		if ( (ConvertErr = ZipBuilder1->ConvertSFX()) == 0 )
			ShowMessage( "Filename is now: " + ZipBuilder1->ZipFileName );
		else
			ShowMessage( "Error " + IntToStr( ConvertErr ) + " occured in making .EXE file" );
	}
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::VerboseCBClick( TObject* /*Sender*/ )
{
	ZipBuilder1->Verbose = VerboseCB->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::TraceCBClick( TObject* /*Sender*/ )
{
	ZipBuilder1->Trace = TraceCB->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::UnattendedCBClick(TObject* /*Sender*/)
{
	ZipBuilder1->Unattended = UnattendedCB->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Showlasterror1Click( TObject* /*Sender*/ )
{
	if ( ZipBuilder1->ErrCode )
		ShowMessage( IntToStr( ZipBuilder1->ErrCode ) + " " + ZipBuilder1->Message );
	else
		ShowMessage( "No last error present" );
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Exit1Click( TObject* /*Sender*/ )
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Zipcomment1Click( TObject* /*Sender*/ )
{
	if ( !ZipBuilder1->ZipComment.IsEmpty()) {
		MsgForm->RichEdit1->Clear();
		MsgForm->RichEdit1->Lines->Add( ZipBuilder1->ZipComment );
		MsgForm->Show();
	} else
		ShowMessage( "No Zip comment in this zip file" );
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DLLversioninfo1Click( TObject* /*Sender*/ )
{
  ShowMessage(ZipBuilder1->FullVersionString());
}
//***********************ZipBuilder Event handling***************************
//---------------------------------------------------------------------------
void __fastcall TMainForm::ZipBuilder1Message( TObject* /*Sender*/, int ErrCode, String Message )
{
	MsgForm->RichEdit1->Lines->Append( Message );
	::PostMessage( MsgForm->RichEdit1->Handle, EM_SCROLLCARET, 0L, 0L );
	Application->ProcessMessages();
	if ( ErrCode > 0 && !ZipBuilder1->Unattended ) ShowMessage( "Error Msg: " + Message );
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::ZipBuilder1DirUpdate( TObject* /*Sender*/ )
{
	FillGrid();
	FilesLabel->Caption = IntToStr(ZipBuilder1->Count);
	if(UpperCase(ExtractFileExt(ZipBuilder1->ZipFileName)) == ".EXE" )
		ConvertBut->Caption = "Convert to ZIP";
	else ConvertBut->Caption = "Convert to EXE";
}

//***********************User defined functions *****************************
//---------------------------------------------------------------------------
String TMainForm::ShowLTime( long s, long f )
{
	String smin, ssec;

	int st = f - s;
	int sec = st / 1000;
	int min = sec / 60;
	sec %= 60;
	ssec = ( sec > 9 ) ? IntToStr( sec ) : "0" + IntToStr( sec );
	smin = ( min > 9 ) ? IntToStr( min ) : "0" + IntToStr( min );
	return smin + ":" + ssec;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::SetZipFName( String aCaption, bool AssignName )
{
	// Assigning the filename will cause the table of contents to be read.
	// and possibly reset it to an empty string (If error found).
	if(AssignName) ZipBuilder1->ZipFileName = aCaption;

 	ZipFName->Caption = (ZipBuilder1->ZipFileName.IsEmpty()) ? String( "<none>" ) :
 		MinimizeName( ZipBuilder1->ZipFileName, ZipFName->Canvas, ZipFName->Width );

	if(ZipFName->Canvas->TextWidth(ZipBuilder1->ZipFileName) > ZipFName->Width)
  {
		ZipFName->Hint     = ZipBuilder1->ZipFileName;
		ZipFName->ShowHint = true;
	}
  else ZipFName->ShowHint = false;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::SetZipTotals( void )
{
	StringGrid1->Cells[0][ZipBuilder1->Count + 1] = "Total";
	StringGrid1->Cells[1][ZipBuilder1->Count + 1] = IntToStr( (__int64)TotComp );
	StringGrid1->Cells[2][ZipBuilder1->Count + 1] = IntToStr( (__int64)TotUncomp );
	unsigned __int64 cs = TotComp, us = TotUncomp;
	if(us) StringGrid1->Cells[4][ZipBuilder1->Count + 1] =
     IntToStr(static_cast<__int64>(100 - cs * 100 / us - ((cs * 100 % us) >= us / 2))) + " % ";
	else StringGrid1->Cells[4][ZipBuilder1->Count + 1] = "0 % ";
	StringGrid1->Cells[5][ZipBuilder1->Count + 1] = "";
}
//---------------------------------------------------------------------------
bool __fastcall TMainForm::AskDirDialog( const HWND FormHandle, String &DirPath )
{
	BROWSEINFO  bi;
	LPTSTR		   lpBuffer;
	ITEMIDLIST *pidl;          // PIDL selected by user
	LPMALLOC    g_pMalloc;
	bool			Result = false;

	// Get the shell's allocator.
	if ( (SHGetMalloc( &g_pMalloc ) )== E_FAIL ) return false;

	// Allocate a buffer to receive browse information.
	if ( (lpBuffer = (LPTSTR)g_pMalloc->Alloc( MAX_PATH)) != 0 ) {
		bi.hwndOwner		= FormHandle;
		bi.pidlRoot			= NULL;
		bi.pszDisplayName	= lpBuffer;
		bi.lpszTitle		= TEXT("");
		bi.ulFlags			= 0;
		bi.lpfn				= NULL;
		bi.lParam			= 0;

		// if pidl = 0 then cancel is used.
		if ( (pidl = SHBrowseForFolder( &bi )) != 0 ) {
			// if 0 then pidl not part of namespace
			if ( SHGetPathFromIDList( pidl, lpBuffer ) ) {
				DirPath = lpBuffer;
				if ( DirPath[DirPath.Length()] != '\\' ) DirPath += '\\';
				Result  = true;
			}
			// Free the PIDL returned by SHBrowseForFolder.
			g_pMalloc->Free( pidl );
		}
		// Free the string buffer used for the name
		g_pMalloc->Free( lpBuffer );
	}
	// Release the shell's allocator.
	g_pMalloc->Release();
	return Result;
}

//---------------------------------------------------------------------------
/* Folder types are a.o.
 *	CSIDL_DESKTOPDIRECTORY, CSIDL_STARTMENU, CSIDL_SENDTO,
 * CSIDL_PROGRAMS, CSIDL_STARTUP etc.
 */
long __fastcall TMainForm::GetSpecialFolder( int aFolder, String &Location )
{
	long		   FolderErr = 0;   // No error.
	TCHAR		  *RealPath;
	ITEMIDLIST *pidl;
	LPMALLOC    g_pMalloc;

	// Get the shell's allocator.
	if ( ::SHGetMalloc( &g_pMalloc ) == E_FAIL ) return E_FAIL;

	// Allocate a buffer to receive the path information.
	if((RealPath = (TCHAR *)g_pMalloc->Alloc( MAX_PATH )) != NULL )
  {
		HRESULT hRes = ::SHGetSpecialFolderLocation( Handle, aFolder, &pidl );
		if(hRes == NOERROR)
    {
			bool Success = ::SHGetPathFromIDList( pidl, RealPath );
			if(Success)
      {
				Location = RealPath;
				Location += "\\";
			}
      else FolderErr = E_UNEXPECTED;
			// Free the PIDL allocated by SHGetSpecialFolderLocation.
			g_pMalloc->Free( pidl );
		}
    else FolderErr = hRes;
		// Free the string buffer used for the name
		g_pMalloc->Free( RealPath );
	}
  else FolderErr = E_OUTOFMEMORY;
	// Release the shell's allocator.
	g_pMalloc->Release();
	return FolderErr;
}

//**************************Grid functions **********************************
//---------------------------------------------------------------------------
void __fastcall TMainForm::FillGrid( void )
{
	// Remove everything from grid except col titles.
  TotUncomp = TotComp   = 0;
	StringGrid1->RowCount = 2;
	ClearGrid();
	if(!ZipBuilder1->Count) return;
	StringGrid1->RowCount = ZipBuilder1->Count + 2;
	for(int i = 1; i <= ZipBuilder1->Count; i++)
  {
		ZipDirEntry *zde = (ZipDirEntry *)ZipBuilder1->ZipContents->Items[i - 1];
		StringGrid1->Cells[0][i] = ExtractFileName( zde->FileName );
		StringGrid1->Cells[1][i] = IntToStr( (int)zde->CompressedSize );
		StringGrid1->Cells[2][i] = IntToStr( (int)zde->UncompressedSize );
		StringGrid1->Cells[3][i] = FormatDateTime( "ddddd  t", FileDateToDateTime(zde->DateTime));
		unsigned __int64 cs = zde->CompressedSize, us = zde->UncompressedSize;
		if(us) StringGrid1->Cells[4][i] = IntToStr((__int64)(100 - cs * 100 / us - ((cs * 100 % us) >= us / 2))) + "% ";
		else StringGrid1->Cells[4][i] = "0% ";
		StringGrid1->Cells[5][i] = ExtractFilePath(zde->FileName);
    StringGrid1->Cells[6][i] = "N"; // means not selected
		TotUncomp += us;
		TotComp   += cs;
	}
  SortGrid();
  SetZipTotals();
	StringGrid1->Row = 1;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::RenButClick( TObject* /*Sender*/ )
{
	RenForm->Show();
}
/*TMainForm::FormClose-------------------------------------------------------
1.79 14 November 2005 RA use property Dll_Load
1.79 21 October 2005 RA Only one dll to be loaded
*/
void __fastcall TMainForm::FormClose(TObject* /*Sender*/, TCloseAction &Action)
{
  ZipBuilder1->Dll_Load = false;
  Action = caFree;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ClearGrid(void)
{
  for(int i = 1; i < StringGrid1->RowCount -1; ++i)
  {       // we keep first and last row
    StringGrid1->Rows[i]->Clear();
  }
  // now make sure the last row is empty
  if(StringGrid1->RowCount == 2) StringGrid1->Rows[1]->SetText(TEXT(""));
  SetZipTotals();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SortGrid(void)
{
  TStringList* List = new TStringList();
  List->Sorted = true;
  for(int i = 1; i < StringGrid1->RowCount - 1; ++i)
                List->Add(StringGrid1->Rows[i]->Text);
  ClearGrid();
  for(int i = 0; i < List->Count; ++i)
    StringGrid1->Rows[i+1]->Text = List->Strings[i];
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::StringGrid1DrawCell(TObject* /*Sender*/, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
  UINT Format;
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
  if(StringGrid1->Cells[0][ARow] != "Total")
    {
      StringGrid1->Canvas->Brush->Color = (StringGrid1->Cells[6][ARow] == "Y") ? clHighlight : clWindow;
      StringGrid1->Canvas->Font = StringGrid1->Font;
      StringGrid1->Canvas->Font->Color = (StringGrid1->Cells[6][ARow] == "Y") ? clHighlightText :clBlack;
      StringGrid1->Canvas->FillRect(Rect);
    }
    else //total row
    {
      StringGrid1->Canvas->Brush->Color = clAqua;
      StringGrid1->Canvas->Font = StringGrid1->Font;
      StringGrid1->Canvas->Font->Color = clRed;
      StringGrid1->Canvas->FillRect(Rect);
    }
  }
  switch(ACol)
  {
    case 0:
    case 5:
      Format = DT_LEFT;
    break;
    default:
      Format = DT_RIGHT;
    break;
  }
  RECT R = RECT(Rect);
  TCHAR* text = StringGrid1->Cells[ACol][ARow].T_STR();
  DrawText(StringGrid1->Canvas->Handle, text, StrLen(text), &R, Format);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::StringGrid1SelectCell(TObject* /*Sender*/, int /*ACol*/,
      int ARow, bool& /*CanSelect*/)
{
  if((ARow != 0) && (ARow != StringGrid1->RowCount)) //1st and total not allowed
  {    //  Select or deselect cell
    StringGrid1->Cells[6][ARow] = (StringGrid1->Cells[6][ARow]== "N") ? "Y" : "N";
  }
  for(int i = 0; i < StringGrid1->ColCount; ++i) //forces repaint of whole row
    StringGrid1->Cells[i][ARow]= StringGrid1->Cells[i][ARow];
}
/*TMainForm::FormActivate----------------------------------------------------------
1.79 14 November 2005 RA use property Dll_Load
1.79 21 October 2005 RA Only one dll to be loaded
1.76 19 June 2004 RA Added for loading of DLL's
*/
void __fastcall TMainForm::FormActivate(TObject* /*Sender*/)
{
  ZipBuilder1->Dll_Load = true;;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ZipBuilder1ProgressDetails(TObject* /*Sender*/,
      TProgressDetails *Details)
{
	switch (Details->Order )
  {
		case TotalSize2Process:
			MsgForm->StatusBar1->Panels->Items[0]->Text = "Total size: " +
                              IntToStr(Details->TotalSize / 1024 ) + " Kb";
			break;

		case TotalFiles2Process:
			MsgForm->StatusBar1->Panels->Items[1]->Text = IntToStr(Details->TotalCount) + " files";
			break;

		case NewFile:
			MsgForm->FileBeingZipped->Caption = Details->ItemName;
			MsgForm->ProgressBar1->Position = 0;		// Current position of bar.
			break;

		case ProgressUpdate:
      MsgForm->ProgressBar1->Position = Details->ItemPerCent;
      MsgForm->ProgressBar2->Position = Details->TotalPerCent;
			break;

		case EndOfBatch:
			// Reset the progress bar and filename.
			MsgForm->FileBeingZipped->Caption = "";
			MsgForm->ProgressBar1->Position = 0;
			MsgForm->StatusBar1->Panels->Items[0]->Text = "";
			MsgForm->StatusBar1->Panels->Items[1]->Text = "";
			MsgForm->ProgressBar2->Position = 0;
	}
}
//---------------------------------------------------------------------------

