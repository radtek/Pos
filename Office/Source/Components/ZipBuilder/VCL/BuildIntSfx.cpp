//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "BuildIntSfx.h"
#include "ZipMsg.h"
#include "ZipBuildDefs.h"
#include "ZipUtils.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


#ifdef INTERNAL_SFX
#define min(x,y) ((x)<(y)?(x):(y))
TInternalSFX::TInternalSFX(TObject *owner)
{
  FOwner = dynamic_cast<TZipWorker*>(owner);
}

//---------------------------------------------------------------------------------
// Search for the right(with our icon) resource directory entry.
DWord __fastcall TInternalSFX::SearchResDirEntry(pIRD ResStart, pIRDirE entry, int Depth)
{
	pIRDatE data;

#if __BORLANDC__ < 1328		// BCB1=0x0520(1312)
	if(entry->u.s.NameIsString) return 0;			// No named resources.
	if(!Depth && entry->u.Id != 3) return 0;			// Only icon resources.
	if(Depth == 1 && entry->u.Id != 1) return 0;	// Only icon with ID 0x1.
	if(!entry->u2.s.DataIsDirectory)
	{
		data = (pIRDatE)((char *)ResStart + entry->u2.OffsetToData);
		if(data->Size == 744)	return data->OffsetToData;
	} else
		return BrowseResDir(ResStart, (pIRD)((char *)ResStart + entry->u2.s.OffsetToDirectory), Depth + 1);
#endif
#if __BORLANDC__ == 1328		// BCB3=0x0530(1328)
	if(entry->_s1.NameIsString) return 0;			// No named resources.
	if(!Depth && entry->Id != 3) return 0;			// Only icon resources.
	if(Depth == 1 && entry->Id != 1) return 0;	// Only icon with ID 0x1.
	if(!entry->_s2.DataIsDirectory)
	{
		data = (pIRDatE)((char *)ResStart + entry->OffsetToData);
		if(data->Size == 744)	return data->OffsetToData;
	} else
		return BrowseResDir(ResStart, (pIRD)((char *)ResStart + entry->_s2.OffsetToDirectory), Depth + 1);
#endif
// And again! changed because unnamed unions are now possible.
#if __BORLANDC__ > 1343		// BCB4=0x0540(1344)
	if(entry->NameIsString) return 0;				// No named resources.
	if(!Depth && entry->Id != 3) return 0;			// Only icon resources.
	if(Depth == 1 && entry->Id != 1) return 0;	// Only icon with ID 0x1.
	if(!entry->DataIsDirectory)
	{
		data = (pIRDatE)((char *)ResStart + entry->OffsetToData);
		if(data->Size == 744)	return data->OffsetToData;
	} else
		return BrowseResDir(ResStart, (pIRD)((char *)ResStart + entry->OffsetToDirectory ), Depth + 1);
#endif
	return 0;
}
//-----------------------------------------------------------------------------------
// Browse through all resource Directories.
DWord __fastcall TInternalSFX::BrowseResDir(pIRD ResStart, pIRD dir, int Depth)
{
	pIRDirE SingleRes = (pIRDirE)((char *)dir + sizeof(IMAGE_RESOURCE_DIRECTORY));
	DWord	Result = 0;

	for(int i = 0; i < dir->NumberOfNamedEntries + dir->NumberOfIdEntries; i++)
	{
		Result = SearchResDirEntry(ResStart, SingleRes, Depth);
		if(Result) break;   // Found the one w're looking for.
		SingleRes++;
	}
	return Result;
}
//----------------------------------------------------------------------------------------
// Find Resource directory entry.
DWord __fastcall TInternalSFX::LookForDirs(const void *SectionData,
                                          const DWord SectionVirtualStart,
															            const int SectionLen, pIDD directories)
{
	pIRD  ResStart;
	DWord Result = 0;
	pIDD  dirs = directories;

	dirs += IMAGE_DIRECTORY_ENTRY_RESOURCE;
	if(dirs->VirtualAddress &&
     ((char *)dirs->VirtualAddress >= (char *)SectionVirtualStart) &&
		(char *)dirs->VirtualAddress < ((char *)SectionVirtualStart + SectionLen))
	{
		ResStart = (pIRD)((char *)SectionData + (dirs->VirtualAddress - SectionVirtualStart));
		Result = BrowseResDir( ResStart, ResStart, 0);
	}
	return Result;
}
//----------------------------------------------------------------------------
// Replace the icon resource in the copied ZipSFX.bin
// Return values: All ok: =0   Error: <0
int __fastcall TInternalSFX::ReplaceIcon(int SFXFile, int SFXSize)
{
const
	SE_NoFoundError   = -10,
	SE_MemStreamError = -11,
	SE_IconSizeError  = -12;

	char *SFXBuffer = NULL;
	TMemoryStream *IconStream = NULL;
	IMAGE_DOS_HEADER *DOSHead;
	PEheader *Header;
	DWord  FoundAddr = 0;
	pISH SectionHeader;
	int Result = SE_NoFoundError;
  TZipWorker *zip = dynamic_cast<TZipWorker *>(FOwner);
	try
	{
  	for(;;)
  	{
  		try
  		{
  			// Put the SFX file into a buffer.
  			SFXBuffer = new char[SFXSize];
  			FileSeek(SFXFile, 0, soFromBeginning);
  			FileRead(SFXFile, SFXBuffer, SFXSize);
  			// Read and check the ZipSFX.bin file for an icon resource.
  			DOSHead = (IMAGE_DOS_HEADER *)SFXBuffer;
  			if(DOSHead->e_magic != IMAGE_DOS_SIGNATURE) break;
  			Header = (PEheader *)((char *)DOSHead + DOSHead->e_lfanew);
  			if(Header->signature != IMAGE_NT_SIGNATURE) break;
  			SectionHeader = &Header->section_header;
  			for(int Section = 0; Section < Header->_head.NumberOfSections; Section++)
  			{	// Walk through sections.
  				if(!StrComp((char *)&SectionHeader->Name[0], ".rsrc"))
  				{	// Found the resource section.
  					FoundAddr = LookForDirs(SFXBuffer + SectionHeader->PointerToRawData,
  					                        SectionHeader->VirtualAddress,
  							                    SectionHeader->SizeOfRawData,
  							                    &Header->opt_head.DataDirectory[0]);
  					if(!FoundAddr) break;
  					// Change memory offset to file offset.
  					FoundAddr = FoundAddr - SectionHeader->VirtualAddress +
  					                        SectionHeader->PointerToRawData;
  					break;
  				}
  				SectionHeader++;
  			}
  			if(!FoundAddr) break;
  			// Save the icon data to a stream.
  			IconStream = new TMemoryStream();
  			zip->SFXIcon->SaveToStream(IconStream);
  			// Only handle icons with this size.
  			if(IconStream->Size == 766)
  			{
  				// Reposition to the actual data and put it into a buffer.
  				IconStream->Seek(22, soFromBeginning);
  				IconStream->ReadBuffer(SFXBuffer, 744);
  				// Go to start of the icon resource in the new created file.
  				FileSeek(SFXFile, (SeekInt)FoundAddr, soFromBeginning);
  				// And write the changed icon data from the buffer.
  				FileWrite(SFXFile, SFXBuffer, 744);
  				Result = 0;
  			}
  			else Result = SE_IconSizeError;
  		}
  		catch( ... )
  		{
  			Result = SE_MemStreamError;
  		}
  		break;
  	}
  }
  __finally
  {
		delete[] SFXBuffer;
		delete IconStream;
		FileSeek(SFXFile, 0, soFromEnd);
	}
	return Result;
}

//---------------------------------------------------------------------------
// Function to copy the central header of an archive and change while copying
// the Local Header offsets and finally the Central Header offset.
// We return 0 if no error or -2 (SE_CopyError) in case something goes wrong.
int __fastcall TInternalSFX::RWCentralDir(int &OutFile,
                                         struct ZipEndOfCentral &EOC,
                                         int OffsetChange)
{
	struct ZipCentralHeader CEH;
	int Result = 0;
  TZipWorker *zip = dynamic_cast<TZipWorker *>(FOwner);
	try
	{
		for(int i = 0; i < EOC.TotalEntries; i++)
		{
			// Read a central header.
			if(FileRead(FInFileHandle, &CEH, sizeof(CEH)) != sizeof(CEH))
				throw EZipBuilder(DS_CEHBadRead);
			if(CEH.HeaderSig != CentralFileHeaderSig) throw EZipBuilder(DS_CEHWrongSig);
			// Change the offset.
			CEH.RelOffLocal += OffsetChange;
			// Write this changed central header to disk
			if(FileWrite(OutFile, &CEH, sizeof(CEH)) != sizeof(CEH))
				throw EZipBuilder(DS_CEHBadWrite);
			// And the remaining bytes of the central header.
			if(zip->CopyBuffer(FInFileHandle, OutFile, CEH.FileNameLen + CEH.ExtraLen + CEH.FileComLen))
				throw EZipBuilder(DS_CEHBadCopy);
		}
		// Skip the EOC record (we already have read it.)
		if(FileSeek(FInFileHandle, (SeekInt)sizeof(EOC), 1 ) == -1) throw EZipBuilder(DS_EOCBadSeek);
		// Write the changed EndOfCentral directory record.
		EOC.CentralOffset += OffsetChange;
		if(FileWrite(OutFile, &EOC, sizeof(EOC)) != sizeof(EOC))
			throw EZipBuilder(DS_EOCBadWrite);
		// And finally the archive comment itself.
		if(zip->CopyBuffer(FInFileHandle, OutFile, EOC.ZipCommentLen))
			throw EZipBuilder(DS_EOCBadCopy);
	}
	catch( ... )
	{
		Result = -2;
	}
	return Result;
}
// TInternalSFX::ConvertSFX------------------------------------------------------
int __fastcall TInternalSFX::ConvertSFX(bool AutoExeViaAdd)
{
	const
		SE_CreateError = -1,  // Error in open of OutFile.
		SE_CopyError = -2,  // Read or write error during copy.
		SE_OpenReadError = -3,  // Error in open of InFile.
		SE_GeneralError = -9;
	int InFile, OutFile, Result = SE_GeneralError;
	int SFXSize = -1, ZipSize = -1, OutSize = -1, SFXBlkSize;
	char dirbuf[MAX_PATH], *sfxblk, cll = 0;
  TZipWorker* zip = dynamic_cast<TZipWorker *>(FOwner);

	int L = zip->SFXCaption.Length() + zip->SFXDefaultDir.Length() +
	        zip->SFXCommandLine.Length() + zip->SFXMessage.Length();
	SFXBlkSize = ((12 + L + 3) / 4)* 4;	// DWord Alignment.
  try
  {
	/* Do a simple validation to ensure that the 4 variable length text
	 * fields are small enough to fit inside the SFX control block. */
    if(zip->SFXCaption.Length() > 255)
          throw EZipBuilder(SF_DlgTitleTooLong);
    if(zip->SFXCommandLine.Length() > 255)
           throw EZipBuilder(SF_CmdLineTooLong);
    if(zip->SFXDefaultDir.Length() > 255)
           throw EZipBuilder(SF_DefPathTooLong);
    if(zip->SFXMessage.Length() > 255)
           throw EZipBuilder(SF_MsgTooLong);
  }
  catch (EZipBuilder &ers)
  {
			// All SFX creation errors will be catched and returned in this one message.
    zip->ShowExceptionError(ers);
    return Result;
  }
	// Try to find the SFX binary file: ZIPSFX.BIN
	// Look in the location given by the SFXPath property first.
  AnsiString SFXPath = zip->SFXPath;
	do
  {
    if(SFXPath.Length() && SFXPath[SFXPath.Length()] != '\\')
         SFXPath += "\\"; // make sure it is a path
		if(FileExists(SFXPath = ExtractFilePath(SFXPath) + "ZipSFX.bin")) break;
		// Try the current directory.
		if(FileExists(SFXPath = "ZipSFX.bin")) break;
		// Try the application directory.
		if(FileExists(SFXPath = ExtractFilePath(ParamStr(0)) + "\\ZipSFX.bin")) break;
		// Try the Windows System dir.
		::GetSystemDirectory(dirbuf, MAX_PATH);
		if(FileExists(SFXPath = AnsiString(dirbuf) + "\\ZipSFX.bin")) break;
		// Try the Windows dir.
		::GetWindowsDirectory(dirbuf, MAX_PATH);
		if(FileExists(SFXPath = AnsiString(dirbuf ) + "\\ZipSFX.bin")) break;
		// Try the dir specified in the DLLDirectory property.
		if(zip->DLLDirectory != "" && FileExists(SFXPath = zip->DLLDirectory + "\\ZipSFX.bin")) break;
		zip->ShowZipMessage(SF_NoZipSFXBin);
		SFXPath = "";
		return Result;
	} while(true);
  zip->SFXPath = SFXPath;
	try
	{
		sfxblk = new char[SFXBlkSize + 1];
	}
	catch( ... )
	{
		zip->ShowZipMessage(GE_NoMem);
		return Result;
	}
	// Create a packed byte with various 1 bit settings.
	memset(sfxblk, '\0', SFXBlkSize);
  strncpy(sfxblk, "MPV", 3);
	if(zip->SFXOptions.Contains(SFXAskCmdLine)) cll  = 1;			// Don't ask user if he wants to run cmd line.
	if(zip->SFXOptions.Contains(SFXAskFiles))	cll |= 2;			// Allow user to edit files in selection box.
	if(zip->SFXOptions.Contains(SFXHideOverWriteBox)) cll |= 4;	// Hide overwrite mode box at runtime.
	if(zip->SFXOverWriteMode == OvrAlways) cll |= 8;						// Default = ovrConfirm.
	if(zip->SFXOverWriteMode == OvrNever) cll |= 16;
	if(!zip->SFXOptions.Contains(SFXCheckSize)) cll |= 32;			// No size checking if false.
	if(zip->SFXOptions.Contains(SFXAutoRun)) cll |= 64;				// Use AutoRun if file starts with '!'.
	if(zip->SFXOptions.Contains(SFXNoSuccessMsg)) cll |= 128;		// Don't show success message after extraction.
	sfxblk[3] = cll;
	sfxblk[5] = 1;
	sfxblk[6] = Char(LOBYTE(LOWORD(SFXBlkSize)));
	sfxblk[7] = Char(HIBYTE(LOWORD(SFXBlkSize)));

	int j = zip->SFXCaption.Length();
	sfxblk[8] = (BYTE)j;
	StrPCopy(&sfxblk[9], zip->SFXCaption);

	sfxblk[j + 9] = (BYTE)zip->SFXDefaultDir.Length();
	StrPCopy(&sfxblk[j + 10], zip->SFXDefaultDir);
	j += zip->SFXDefaultDir.Length();

	sfxblk[j + 10] = (BYTE)zip->SFXCommandLine.Length();
	StrPCopy(&sfxblk[j + 11], zip->SFXCommandLine);
	j += zip->SFXCommandLine.Length();

	sfxblk[j + 11] = (BYTE)zip->SFXMessage.Length();
	StrPLCopy(&sfxblk[j + 12], zip->SFXMessage, zip->SFXMessage.Length());

	// Create the destination.
	String OutFileName;
	// SFX code will be added to a new archive we are
	// going to create with an extension of .EXE
	if(AutoExeViaAdd) OutFileName = zip->ZipFileName;
  else
  {
		if(UpperCase(ExtractFileExt(zip->ZipFileName)) != ".ZIP")
		{
			zip->ShowZipMessage(SF_InputIsNoZip);
      if(sfxblk) delete[] sfxblk;
			return Result;
		}
		OutFileName = ChangeFileExt(zip->ZipFileName, ".exe");
	}

	if(FileExists(OutFileName)) EraseFile(OutFileName, zip->HowToDelete);
	if((OutFile = FileCreate(OutFileName)) != -1)
	{
		// Copy the SFX code to destination .EXE file.
		if((InFile = FileOpen(zip->SFXPath, fmOpenRead | fmShareDenyWrite )) != -1)
		{
			Result = zip->CopyBuffer(InFile, OutFile, -1);
			SFXSize = FileSeek(InFile, 0, 2);
			FileClose(InFile);
			// Copy the SFX code to destination .EXE file.
			if(!Result && !zip->SFXIcon->Empty)
              Result = ReplaceIcon(OutFile, SFXSize);
		}
		else Result = SE_OpenReadError;
		// Copy the special SFX block to the destination.
		if(!Result)
		{
			if(FileWrite(OutFile, sfxblk, SFXBlkSize) != SFXBlkSize) Result = SE_CopyError;
			if(!Result)
			{
				struct ZipEndOfCentral EOC;	// We need to create a End-of-central-dir header.
				if(AutoExeViaAdd)
				{
					memset(&EOC, '\0', sizeof(EOC));
					EOC.HeaderSig = EndCentralDirSig;
					EOC.CentralOffset = SFXSize + SFXBlkSize;	// Central offset=EOC offset=end of SFX code.
					// Copy the EOC header to the .exe file.
					if(FileWrite(OutFile, &EOC, sizeof(EOC)) != sizeof(EOC))	Result = SE_CopyError;
					// Let's close the file and get out - we don't
					// have a zipfile to append in this case.
				}
				else
				{
					// Copy the ZIP file to the destination (BUG fix)
					try
					{
						zip->OpenEOC(EOC, true);	// Read the EOC or we get an exception.
            FInFileHandle = zip->FInFileHandle; // copy filehandle for internal use
						FileSeek(FInFileHandle, 0, 0);
						// If we got a warning in List() we assume it's a pre v1.5 .ZIP.
						// (converted back from a .EXE ) and we will not change the offsets.
						if(zip->FWrongZipStruct)
						{
							Result = zip->CopyBuffer(FInFileHandle, OutFile, -1);
							ZipSize = zip->FRealFileSize;
						}
						else
						{
							// Copy until we get at the start of the central header.
							if((Result = zip->CopyBuffer(FInFileHandle, OutFile, EOC.CentralOffset)) == 0)
								// Now read all headers and change the offsets.
								Result = RWCentralDir(OutFile, EOC, SFXSize + SFXBlkSize);
							ZipSize = zip->FFileSize;
						}
					}
					catch ( ... )
					{
						Result = SE_OpenReadError;
					}
					if(FInFileHandle != -1) FileClose(FInFileHandle);
				}
			}
		}
		OutSize = FileSeek(OutFile, 0, 2);
		FileClose(OutFile);
	}
	else Result = SE_CreateError;

	if(Result || !AutoExeViaAdd)
	{
		// An extra check if file is ok.
		if(!Result && (SFXSize == -1 || ZipSize == -1 || OutSize == -1 ||
		              OutSize != SFXSize + ZipSize + SFXBlkSize))
			Result = SE_GeneralError;

		if(!Result)
		{
			EraseFile(zip->FZipFileName, zip->FHowToDelete);
			zip->ZipFileName = OutFileName;	// The .EXE file is now the default archive and invoke List().
		}
		else DeleteFile(OutFileName);
	}
	if(sfxblk) delete[] sfxblk;
	return Result;
}
// TInternalSFX::ConvertSFX

// TInternalSFX::ConvertZIP------------------------------------------------------
// Convert an .EXE archive to a .ZIP archive.
// Returns 0 if good, or else a negative error code.
int __fastcall TInternalSFX::ConvertZIP(void)
{
	const
		 SE_CreateError   = -1,  // Error in creation of OutFile.
		 SE_CopyError     = -2,  // Seek error in InFile.
		 SE_OpenReadError = -3,  // Error in open of InFile.
		 SE_GeneralError  = -9,
		 SE_OutOfMemError = -10;
	int OutFile, InSize = -1, OutSize = -1, SFXBlkSize, Result = SE_GeneralError;
	char *sfxblk = NULL, *sfxstr, cll;
	struct ZipEndOfCentral EOC;
  TZipWorker *zip = dynamic_cast<TZipWorker *>(FOwner);

	// Create the destination.
	zip->StartWaitCursor();
	Result = SE_CopyError;
	String OutFileName = ChangeFileExt(zip->FZipFileName, ".zip");

	if(FileExists(OutFileName)) EraseFile(OutFileName, zip->FHowToDelete);
	if((OutFile = FileCreate(OutFileName)) != -1)
	{
		try
		{
			SFXBlkSize = min(1032, FSFXOffset);
			sfxblk = new char[SFXBlkSize];
			zip->OpenEOC(EOC, true);		// Read the EOC record or we get an exception.
      FInFileHandle = zip->FInFileHandle; // copy filehandle for internal use
			// Step over SFX code at the begin of the .EXE file
			// and read the SFX MPU or MPV block.
			if(FileSeek(FInFileHandle, (SeekInt)(FSFXOffset - SFXBlkSize), 0) != -1 &&
			            FileRead(FInFileHandle, sfxblk, SFXBlkSize) == SFXBlkSize)
			{
				// Find the start of the MPV or MPU block.
				for(int i = 0; i <= SFXBlkSize - 3; i++)
				{
					if((sfxblk[i] == 'M') && (sfxblk[i + 1] == 'P') && ((sfxblk[i + 2] == 'U') ||
					   (sfxblk[i + 2] == 'V')))
					{
						// Read the 'custom' icon back from the executable.
						HICON IconHandle = ExtractIcon(HInstance, zip->FZipFileName.c_str(), 0);
						if(IconHandle && (int)IconHandle != 1)
						{
							if(zip->FSFXIcon->Handle) zip->FSFXIcon->ReleaseHandle();
							zip->FSFXIcon->Handle = IconHandle;
						}
						// Read back the original values from the MPU block.
						zip->FSFXOptions.Clear();
						zip->FSFXOverWriteMode = OvrConfirm;
						cll = sfxblk[i + 3];
						if(cll & 1)	 zip->FSFXOptions = zip->FSFXOptions << SFXAskCmdLine;
						if(cll & 2)	 zip->FSFXOptions = zip->FSFXOptions << SFXAskFiles;
						if(cll & 4)	 zip->FSFXOptions = zip->FSFXOptions << SFXHideOverWriteBox;
						if(cll & 8)	 zip->FSFXOverWriteMode = OvrAlways;
						if(cll & 16)	 zip->FSFXOverWriteMode = OvrNever;
						if(!(cll & 32)) zip->FSFXOptions = zip->FSFXOptions << SFXCheckSize;
						if(cll & 64)	 zip->FSFXOptions = zip->FSFXOptions << SFXAutoRun;
            if(sfxblk[i + 2] == 'U')
            {
							zip->FSFXCaption = AnsiString(&sfxblk[i+7], sfxblk[i+4]);
							zip->FSFXDefaultDir = AnsiString(&sfxblk[sfxblk[i+4] + i+7], sfxblk[i+5]);
							zip->FSFXCommandLine = AnsiString(&sfxblk[sfxblk[i+4] + sfxblk[i+5] + i+7], sfxblk[i+6]);
						}
						else
						{
							if(cll & 128)	 zip->FSFXOptions = zip->FSFXOptions << SFXNoSuccessMsg;
							sfxstr = &sfxblk[i + 8];
							zip->FSFXCaption = AnsiString(&sfxstr[1], sfxstr[0]);
							sfxstr = sfxstr + sfxstr[0] + 1;
							zip->FSFXDefaultDir = AnsiString(&sfxstr[1], sfxstr[0]);
							sfxstr = sfxstr + sfxstr[0] + 1;
							zip->FSFXCommandLine = AnsiString(&sfxstr[1], sfxstr[0]);
							sfxstr = sfxstr + sfxstr[0] + 1;
							zip->FSFXMessage = AnsiString(&sfxstr[1], sfxstr[0]);
						}
						break;
					}
				}
				// If we got a warning in List() we assume it's a pre v1.5 .EXE.
				// and we will not change the offsets.
				if(zip->FWrongZipStruct)
				{
					Result = zip->CopyBuffer(FInFileHandle, OutFile, -1);
					InSize = zip->FRealFileSize;
				}
				else
				{
					// Copy until the start of the first Central header.
					if((Result = zip->CopyBuffer(FInFileHandle, OutFile, EOC.CentralOffset - FSFXOffset )) == 0)
						// Now read all headers and change the offsets.
						Result = RWCentralDir(OutFile, EOC, -FSFXOffset);
					InSize = zip->FFileSize;
				}
			}
		}
		catch (const MEMEXCEPT &me)
		{		// All memory allocation errors.
			Result = SE_OutOfMemError;
		}
		catch( ... )
		{
			Result = SE_OpenReadError;
		}
		if(FInFileHandle != -1) FileClose(FInFileHandle);
		OutSize = FileSeek(OutFile, 0, 2);
		FileClose(OutFile);
	}
	else Result = SE_CreateError;

	if(!Result && (InSize == -1 || OutSize == -1 || OutSize != InSize - FSFXOffset))
		Result = SE_GeneralError;

	if(!Result)
	{
		EraseFile(zip->FZipFileName, zip->FHowToDelete);
		zip->ZipFileName = OutFileName;	// The .ZIP file is now the default archive and invoke List().
	}
	else DeleteFile(OutFileName);

	if(sfxblk) delete[] sfxblk;
	zip->StopWaitCursor();
  return Result;
}
// TInternalSFX::ConvertZIP

#endif // INTERNAL_SFX



