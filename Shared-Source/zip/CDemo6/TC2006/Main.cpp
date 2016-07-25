#include <vcl\vcl.h>
#pragma hdrstop

#include "Main.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
// progam link changed for TurboC++ 2006 Explorer
#pragma link "ZBldr179C10.bpi"
#pragma resource "*.dfm"

TMainForm *MainForm;

/*TMainForm::TMainForm----------------------------------------------------------
1.76 19 June 2004 RA removed loading of DLL's, gives AV if not possible to load
*/
__fastcall TMainForm::TMainForm(TComponent *Owner) : TForm(Owner)
{
// code added for static linking in TurboC++ 2006 Explorer
	ZipBuilder1 = new TZipBuilder(this);
	ZipBuilder1->OnDirUpdate = ZipBuilder1DirUpdate;
	ZipBuilder1->OnMessage = ZipBuilder1Message;
	ZipBuilder1->OnProgressDetails = ZipBuilder1ProgressDetails;
	// because TurboC++ builds the application in a dir one lower than the files
	// we need to adjust the current dir
	SetCurrentDir("../");
	// end added code
	StringGrid1->RowCount     =   2;  // First row is fixed, and used for column headers.
	StringGrid1->Cells[0][0]  = "File Name";
	StringGrid1->Cells[1][0]  = "Compr. Size";
	StringGrid1->Cells[2][0]  = "Uncompr. Size";
	StringGrid1->Cells[3][0]  = "Date Time";
	StringGrid1->Cells[4][0]  = "Ratio";
	StringGrid1->Cells[5][0]  = "Path";

	/* If we had args on the cmd line, then try to open the first one
		as a zip/exe file.  This is most useful in case user has an association
		to ".zip" that causes this program to run when user double clicks on a zip
		file in Explorer. */
	if(ParamCount > 0)  ZipBuilder1->ZipFileName = ParamStr(1);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FillGrid( void ) {
	// Remove everything from grid except col titles.
	 for(int i = 1; i < StringGrid1->RowCount; ++i)
       StringGrid1->Rows[i]->Clear();
	StringGrid1->RowCount = ZipBuilder1->Count + 2;
	if ( !ZipBuilder1->Count ) return;

	TotUncomp = TotComp   = 0;
	for ( int i = 1; i <= ZipBuilder1->Count; i++ )
  {
		ZipDirEntry *zde = (ZipDirEntry *)ZipBuilder1->ZipContents->Items[i - 1];
		StringGrid1->Cells[0][i] = ExtractFileName( zde->FileName );
		StringGrid1->Cells[1][i] = IntToStr( zde->CompressedSize );
		StringGrid1->Cells[2][i] = IntToStr( zde->UncompressedSize );
		StringGrid1->Cells[3][i] = FormatDateTime( "ddddd  t", FileDateToDateTime( zde->DateTime ) );
		unsigned long cs = zde->CompressedSize, us = zde->UncompressedSize;
		if(us)
			StringGrid1->Cells[4][i] = IntToStr( 100 - cs * 100 / us - ((cs * 100 % us) >= us / 2) ) + "% ";
		else
			StringGrid1->Cells[4][i] = "0% ";
		StringGrid1->Cells[5][i] = ExtractFilePath( zde->FileName );
    StringGrid1->Cells[6][i] = "N"; // means not selected
		TotUncomp += zde->UncompressedSize;
		TotComp   += zde->CompressedSize;
	}
  SortGrid();
	SetZipTotals();
  StringGrid1->Row = 1;
}
//---------------------------------------------------------------------------
bool __fastcall TMainForm::ZipOpenArchive()
{
	if ( ZipOpenDir == "" ) GetSpecialFolder( CSIDL_DESKTOPDIRECTORY, ZipOpenDir );
	OpenDialog1->InitialDir = ZipOpenDir;
	OpenDialog1->Title      = "Open Existing ZIP File";
	OpenDialog1->Filter     = "ZIP Files (*.ZIP)|*.zip";
	OpenDialog1->FileName   = "";
  OpenDialog1->Options << ofHideReadOnly << ofShareAware << ofPathMustExist;
	if(OpenDialog1->Execute())
  {
		ZipOpenDir = ExtractFilePath( OpenDialog1->FileName );
		// Assigning the filename will cause the table of contents to be read.
		ZipBuilder1->ZipFileName = OpenDialog1->FileName;
		/* Set the caption after assigning the filename. This
		   way, the filename will be null if the open failed. */
		SetZipFName( ZipBuilder1->ZipFileName );
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::NewZipButClick( TObject* /*Sender*/ )
{
	static String FirstDir = "";

	if ( FirstDir == "" ) GetSpecialFolder( CSIDL_DESKTOPDIRECTORY, FirstDir );
	OpenDialog1->InitialDir = FirstDir;
	OpenDialog1->Title      = "Create New ZIP File";
	OpenDialog1->FileName   = "";
	OpenDialog1->Filter     = "ZIP Files (*.ZIP)|*.zip";
	OpenDialog1->DefaultExt = "Zip";
	OpenDialog1->Options << ofHideReadOnly << ofShareAware;
	OpenDialog1->Options >> ofPathMustExist >> ofFileMustExist;
	if(OpenDialog1->Execute())
  {
		FirstDir = ExtractFilePath( OpenDialog1->FileName );
		if(UpperCase(ExtractFileExt(OpenDialog1->FileName)) != ".ZIP")
    {
			ShowMessage( "Error: your new archive must end in .ZIP" );
			return;
		}
		if(FileExists(OpenDialog1->FileName))
    {
			bool Ans = MessageDlg( "Overwrite Existing File: " + OpenDialog1->FileName + "?",
                          mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0 ) == mrYes;
			if(Ans) DeleteFile( OpenDialog1->FileName );
			else return;  // Don't use the new name.
		}
		ZipBuilder1->ZipFileName = OpenDialog1->FileName;
		SetZipFName( ZipBuilder1->ZipFileName );
	}
  else return;
	if(ZipBuilder1->ZipFileName != "") AddSpan();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ExtractButClick( TObject* /*Sender*/ )
{
	static String FirstDir = "";

	ZipOpenDir = FirstDir;
	if(!ZipOpenArchive()) return;
	FirstDir = ZipOpenDir;

	if(ZipBuilder1->ZipFileName == "" ) return;

	Extract->ShowModal();
	if((ExtractDir == "" ) || Canceled ) return;

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
		if ( ZipBuilder1->FSpecArgs->Count < 1 ) {
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
	if ( ExpandDirs )
		ZipBuilder1->ExtrOptions << ExtrDirNames;
	if ( OverWr )
		ZipBuilder1->ExtrOptions << ExtrOverWrite;
	try {
		ZipBuilder1->Extract();
	} catch ( ... ) {
		ShowMessage( "Error in Extract; Fatal DLL Exception in Main" );
		return;
	}
	String IsOne = (ZipBuilder1->SuccessCnt == 1) ? " was" : "s were";
	ShowMessage( IntToStr( ZipBuilder1->SuccessCnt ) + " file" + IsOne + " extracted" );
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ZipBuilder1DirUpdate( TObject* /*Sender*/ )
{
	FillGrid();
	FilesLabel->Caption = IntToStr( ZipBuilder1->Count );
	SetZipFName( ZipBuilder1->ZipFileName );
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::AddButClick( TObject* /*Sender*/ )
{
	static String FirstDir = "";

	ZipOpenDir = FirstDir;
	if(!ZipOpenArchive()) return;
	FirstDir = ZipOpenDir;

	if ( ZipBuilder1->ZipFileName == "" ) return;
	AddSpan();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::AddSpan()
{
	Canceled = false;
	AddFile->ShowModal();  // Let user pick filenames to add.
	if(Canceled) return;

	if(!AddFile->SelectedList->Items->Count)
  {
		ShowMessage( "No files selected" );
		return;
	}
	MsgForm->RichEdit1->Clear();
	MsgForm->Show();
	// Put this message into the message form.
	ZipBuilder1Message( this, 0, "Beginning Add to " + ZipBuilder1->ZipFileName );

	ZipBuilder1->AddOptions.Clear();
	if ( AddFile->RecurseCB->Checked )					// We want recursion.
		ZipBuilder1->AddOptions = ZipBuilder1->AddOptions << AddRecurseDirs;
	if ( AddFile->DirNameCB->Checked )					// We want dirnames.
		ZipBuilder1->AddOptions = ZipBuilder1->AddOptions << AddDirNames;
	if(FormatCB->Checked)								// We want disk spanning with formating
		ZipBuilder1->AddOptions  = ZipBuilder1->AddOptions << AddDiskSpanErase;
	else															// We want normal disk spanning
		ZipBuilder1->AddOptions  = ZipBuilder1->AddOptions << AddDiskSpan;
	if ( AddFile->EncryptCB->Checked )					// We want a password.
		ZipBuilder1->AddOptions = ZipBuilder1->AddOptions << AddEncrypt;

	ZipBuilder1->FSpecArgs->Clear();
	ZipBuilder1->FSpecArgs->Assign( AddFile->SelectedList->Items ); // Specify filenames.
	AddFile->SelectedList->Clear();

	try
  {
		ZipBuilder1->Add();
	}
  catch ( ... )
  {
		ShowMessage( "Error in Add; Fatal DLL Exception in Main" );
		return;
	}
	String IsOne = (ZipBuilder1->SuccessCnt == 1) ? " was" : "s were";
	ShowMessage( IntToStr( ZipBuilder1->SuccessCnt ) + " file" + IsOne + " added" );
}
//---------------------------------------------------------------------------
// This is the "OnMessage" event handler.
void __fastcall TMainForm::ZipBuilder1Message( TObject* /*Sender*/, int ErrCode, String Message )
{
	MsgForm->RichEdit1->Lines->Append( Message );
	::PostMessage( MsgForm->RichEdit1->Handle, EM_SCROLLCARET, 0L, 0L );
	Application->ProcessMessages();
	if(ErrCode > 0) ShowMessage( "Error Msg: " + Message );
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SetZipFName(String aCaption)
{
	ZipFName->Caption = MinimizeName( aCaption, ZipFName->Canvas, ZipFName->Width );
	if ( ZipFName->Caption == "" ) ZipFName->Caption = "<none>";
	if(ZipFName->Canvas->TextWidth(aCaption) > ZipFName->Width)
  {
		ZipFName->Hint = aCaption;
		ZipFName->ShowHint = true;
	} else ZipFName->ShowHint = false;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::WriteBttnClick( TObject* /*Sender*/ )
{
	static String FirstDir = "";
	String InFile, OutFile;

	ZipOpenDir = FirstDir;
	if(!ZipOpenArchive()) return;
	FirstDir = ZipOpenDir;

	InFile = ZipBuilder1->ZipFileName;
	if(InFile == "") return;

	if(AskDirDialog(MainForm->Handle, OutFile))
  {
		OutFile += ExtractFileName( InFile );
		MsgForm->RichEdit1->Clear();
		MsgForm->Show();
 		ZipBuilder1->WriteSpan( InFile, OutFile );
		MsgForm->Hide();
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ReadBttnClick( TObject* /*Sender*/ )
{
	String InFile, OutPath;

	OpenDialog1->Options << ofHideReadOnly << ofShareAware << ofPathMustExist;
	OpenDialog1->Title      = "Open spanned ZIP archive on last disk";
	OpenDialog1->Filter     = "ZIP Files (*.ZIP)|*.zip";
	OpenDialog1->FileName   = "";
	OpenDialog1->InitialDir = "A:\\";
	OpenDialog1->DefaultExt = "zip";
	if(OpenDialog1->Execute())
  {
		if(AskDirDialog( MainForm->Handle, OutPath))
    {
      OutPath += ExtractFileName( InFile );
			MsgForm->RichEdit1->Clear();
			MsgForm->Show();
			if(!ZipBuilder1->ReadSpan(InFile, OutPath))
      {
				ZipBuilder1->ZipFileName = OutPath;
			  SetZipFName( ZipBuilder1->ZipFileName );
			}
 			MsgForm->Hide();
		}
	}
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
void __fastcall TMainForm::Exit1Click(TObject* /*Sender*/ )
{
	Close();
}
//---------------------------------------------------------------------------
bool __fastcall TMainForm::AskDirDialog( const HWND FormHandle, String &DirPath )
{
	BROWSEINFO  bi;
	LPSTR		   lpBuffer;
	ITEMIDLIST *pidl;          // PIDL selected by user
	LPMALLOC    g_pMalloc;
	bool			Result = false;

	// Get the shell's allocator.
	if ( (SHGetMalloc( &g_pMalloc ) )== E_FAIL ) return false;

	// Allocate a buffer to receive browse information.
	if((lpBuffer = (LPSTR)g_pMalloc->Alloc(MAX_PATH)) != 0)
  {
		bi.hwndOwner		= FormHandle;
		bi.pidlRoot			= NULL;
		bi.pszDisplayName	= lpBuffer;
		bi.lpszTitle		= "";
		bi.ulFlags			= 0;
		bi.lpfn				= NULL;
		bi.lParam			= 0;

		// if pidl = 0 then cancel is used.
		if((pidl = SHBrowseForFolder(&bi)) != 0)
    {
			// if 0 then pidl not part of namespace
			if(SHGetPathFromIDList(pidl, lpBuffer))
      {
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
 * CSIDL_PROGRAMS, CSIDL_STARTUP
 */
long __fastcall TMainForm::GetSpecialFolder( int aFolder, String &Location )
{
	long		   FolderErr = 0;   // No error.
	char		  *RealPath;
	ITEMIDLIST *pidl;
	LPMALLOC    g_pMalloc;

	// Get the shell's allocator.
	if(::SHGetMalloc( &g_pMalloc ) == E_FAIL ) return E_FAIL;

	// Allocate a buffer to receive the path information.
	if ( (RealPath = (char *)g_pMalloc->Alloc( MAX_PATH )) != NULL )
  {
		HRESULT hRes = ::SHGetSpecialFolderLocation( Handle, aFolder, &pidl );
		if(hRes == NOERROR)
    {
			bool Success = ::SHGetPathFromIDList( pidl, RealPath );
			if(Success)
      {
				Location = RealPath;
				Location += "\\";
			} else FolderErr = E_UNEXPECTED;
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
//---------------------------------------------------------------------------
void __fastcall TMainForm::DLLversioninfo1Click( TObject* /*Sender*/ )
{
  ShowMessage(ZipBuilder1->FullVersionString());
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormResize( TObject* /*Sender*/ )
 {
	ZipFName->Width = Width - 291;
	SetZipFName( ZipBuilder1->ZipFileName );
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SetZipTotals( void )
{
	StringGrid1->Cells[0][ZipBuilder1->Count + 1] = "Total";
	StringGrid1->Cells[1][ZipBuilder1->Count + 1] = IntToStr( TotComp );
	StringGrid1->Cells[2][ZipBuilder1->Count + 1] = IntToStr( TotUncomp );
	unsigned long cs = TotComp, us = TotUncomp;
	if(us)
		StringGrid1->Cells[4][ZipBuilder1->Count + 1] = IntToStr( 100 - cs * 100 / us - ((cs * 100 % us) >= us / 2) ) + " % ";
	else
		StringGrid1->Cells[4][ZipBuilder1->Count + 1] = "0 % ";
	StringGrid1->Cells[5][ZipBuilder1->Count + 1] = "";
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Messages1Click( TObject* /*Sender*/ )
{
	MsgForm->Show();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::MaxVolSizeEditChange( TObject* /*Sender*/ )
{
	ZipBuilder1->MaxVolumeSize = StrToIntDef(  MaxVolSizeEdit->Text, 0 );
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FreeDisk1EditChange( TObject* /*Sender*/ )
{
	ZipBuilder1->KeepFreeOnDisk1 = StrToIntDef( FreeDisk1Edit->Text, 0 );
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::MinFreeVolEditChange( TObject* /*Sender*/ )
{
 ZipBuilder1->MinFreeVolumeSize = StrToIntDef( MinFreeVolEdit->Text, 65536 );
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SortGrid(void)
{
  TStringList* List = new TStringList();
  List->Sorted = true;
  for(int i = 1; i < StringGrid1->RowCount - 1; ++i)
                List->Add(StringGrid1->Rows[i]->Text);
  for(int i = 1; i < StringGrid1->RowCount; ++i)
       StringGrid1->Rows[i]->Clear();
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
  char* text = StringGrid1->Cells[ACol][ARow].c_str();
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
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormClose(TObject* /*Sender*/, TCloseAction &Action)
{
	ZipBuilder1->Dll_Load = false;
  Action = caFree;
}
/*TMainForm::FormActivate----------------------------------------------------------
1.76 19 June 2004 RA Added for loading of DLL's
1.79 25 October 2005 RA only one dll to be loaded
*/
void __fastcall TMainForm::FormActivate(TObject* /*Sender*/)
{
	ZipBuilder1->Dll_Load = true;
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

