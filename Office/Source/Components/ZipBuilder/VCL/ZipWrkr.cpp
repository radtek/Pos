/* TZipWorker component v1.60 by Chris Vleghert
 * a C++ Builder 3, 4 , 5 and 6 wrapper for the freeware ZIP and UNZIP DLL's
 * from Chris Vleghert and E.W. Engler.
 *	 e-mail: englere@abraxis.com
 *  www:    http://www.geocities.com/SiliconValley/Network/2114
 *  v1.77 by Roger Aelbrecht September 23, 2004.
 *          http://home.tiscali.be/driehoeksw
 */

#include <vcl\vcl.h>
#pragma hdrstop

#include "ZipWrkr.h"
#include "ZipMsg.h"
#include "ZipBuildDefs.h"
#include "ZipStrs.h"
#include "ZipUtils.h"
#include "ZipDlg.h"
#include "ZipCtx.h"

#pragma package(smart_init)	// Used in BCB 3,4,5 ignored in BCB 1
#pragma resource "*.res"

#if (__BORLANDC__ >= 0x0560) // added 1.74
#pragma link "vcl.bpi"
#endif

// Added by Russell Peters to compile/build under BCB6
#define min(x,y) ((x)<(y)?(x):(y))
#define max(x,y) ((x)>(y)?(x):(y))
#if (__BORLANDC__ >= 0x0560)   // changed 1.74
#define STRMSIZ(x) __int64(x)
#else
#define STRMSIZ(x) int(x)     // changed 1.74
#endif
//end of add
const char extend[256] = {0};

// ================== changed or New functions =============================

/* TZipWorker::Rename----------------------------------------------------------
1.78 18 Mar 2005 RP Fix bug with only data descriptor uncompressed size
1.77 20 August 2004 RA progress adjusted
1.75 18 February 2004 RP allow >2G
1.73.3.2 11 October 2003 RP changed comment variable
1.73.3.2 8 Oct 2003 RA if filename has extended chars change HostVer + extfileattrib
1.73 23 september 2003 RA changing date/time of protected file is not allowed
1.73 7 August 2003 RA init FOutFileHandle
1.73 (16 July 2003) RP use SetSlash + ConvertOEM
1.73 (14 July 2003) RA convertion/re-convertion of filenames with OEM chars
Function to read a Zip archive and change one or more file specifications.
Source and Destination should be of the same type. (path or file)
If NewDateTime is 0 then no change is made in the date/time fields.
Return values:
0            All Ok.
-7           Rename errors. See ZipMsgXX.rc
-8           Memory allocation error.
-9           General unknown Rename error.
-10          Dest should also be a filename.
*/
int __fastcall TZipWorker::Rename(TList &RenameList, unsigned long DateTime)
{
	ZipEndOfCentral    EOC;
	ZipCentralHeader   CEH;
	ZipLocalHeader     LOH;
	String             OrigFileName;
	String             MsgStr;
	String             OutFilePath;
	char *Buffer = NULL;
  __int64 FullSize = 0;
	int	TotalBytesWrite;
	int				    Result = 0;
	ZipRenameRec		*RenRec;
  TMZipDataList* MDZD = NULL;
	FShowProgress = zspNone;
	FInFileName   = FZipFileName;
	FInFileHandle = -1;
  FOutFileHandle = -1;
  FZipBusy = true;
	StartWaitCursor();
	// If we only have a source path make sure the destination is also a path.
	for(int i = 0; i < RenameList.Count; i++)
	{
		RenRec = (ZipRenameRec *)RenameList.Items[i];
		RenRec->Source = SetSlash(RenRec->Source,psdExternal);
		RenRec->Dest   = SetSlash(RenRec->Dest,psdExternal);
    RemoveDriveSpec(RenRec->Source);
    RemoveDriveSpec(RenRec->Dest);
    if(RenRec->Source.AnsiPos("*") || RenRec->Source.AnsiPos("?") ||
       RenRec->Dest.AnsiPos("*") || RenRec->Dest.AnsiPos("?"))
    {
      ShowZipMessage(AD_InvalidName); // no wildcards allowed
      StopWaitCursor();
      FZipBusy = false;
      return -7;  // Rename error
    }
		if(!ExtractFileName(RenRec->Source).Length())
		{	// Assume it's a path.
			// Make sure destination is a path also.
			RenRec->Dest   = SetSlash(ExtractFilePath(RenRec->Dest),psdInternal);
			RenRec->Source = SetSlash(RenRec->Source, psdInternal);
		}
		else if(!ExtractFileName(RenRec->Dest).Length())
    	{
	      StopWaitCursor();
        FZipBusy = false;
			  return -10;  // Dest should also be a filename.
      }
	}
	try
	{
    Buffer = new char[FBufSize];
		// Check the input file.
		if(!FileExists(FZipFileName))	throw EZipBuilder(GE_NoZipSpecified);
		if((OutFilePath = MakeTempFileName()) == "") throw EZipBuilder(DS_NoTempFile );

		// Create the output file.
		FOutFileHandle = FileCreate(OutFilePath);
		if(FOutFileHandle == -1)	throw EZipBuilder(DS_NoOutFile );

		// The following function will read the EOC and some other stuff:
		OpenEOC(EOC, false);  // do not throw exceptions yet

		// Get the date-time stamp and save for later.
		FDateStamp = FileGetDate(FInFileHandle);

		// Now we now the number of zipped entries in the zip archive
		FTotalDisks = EOC.ThisDiskNo;
		if(EOC.ThisDiskNo) throw EZipBuilder(RN_NoRenOnSpan);

		// Go to the start of the input file.
		if(FileSeek(FInFileHandle, 0i64, 0) == -1)	throw EZipBuilder(DS_FailedSeek);

		// Write the SFX header if present.
		if(CopyBuffer(FInFileHandle, FOutFileHandle, FSFXOffset))
			throw EZipBuilder(RN_ZipSFXData);

		// Go to the start of the Central directory.
		if(FileSeek(FInFileHandle, static_cast<__int64>(EOC.CentralOffset), 0) == -1)
			throw EZipBuilder(DS_FailedSeek);

		MDZD = new TMZipDataList(EOC.TotalEntries);		// create class
    FullSize = EOC.CentralSize + EOC.ZipCommentLen + sizeof(ZipEndOfCentral);
		// Read for every entry: The central header and save information for later use.
		for(int i = 0; i < EOC.TotalEntries; i++)
		{
			// Read a central header.
      ReadJoin(&CEH, sizeof(CEH), DS_CEHBadRead);
			if(CEH.HeaderSig != CentralFileHeaderSig)	throw EZipBuilder( DS_CEHWrongSig);

			// Now the filename.
      ReadJoin( Buffer, CEH.FileNameLen, DS_CENameLen);
			// Save the file name info in the MDZD structure.
			MDZD->Items[i]->FileNameLen = CEH.FileNameLen;
      MDZD->Items[i]->FileName = new char[CEH.FileNameLen + 1];
      StrLCopy(MDZD->Items[i]->FileName, Buffer, CEH.FileNameLen);
      MDZD->Items[i]->FileName[CEH.FileNameLen] = '\0';
			//convert OEM char set in original file else we don't find the file
      FHostNum = CEH.VersionMadeBy1;
      FHostVer = CEH.VersionMadeBy0;
      AnsiString Name = ConvertOEM(MDZD->Items[i]->FileName, cpdOEM2ISO);
      StrCopy(MDZD->Items[i]->FileName,Name.c_str());
      //DiskStart is not used in this function and we need FHostNum later
      MDZD->Items[i]->DiskStart = static_cast<short>((FHostNum << 8) + FHostVer);
			MDZD->Items[i]->RelOffLocal = CEH.RelOffLocal;
      MDZD->Items[i]->ComprSize  = CEH.ComprSize;   // RP may not be valid in local header
       // if encrypted use CRC32 as flag
      MDZD->Items[i]->CRC32 = CEH.Flag & 1;
      MDZD->Items[i]->DateTime    = DateTime;
			// We need the total number of bytes we are going to read for the progress event.
      FullSize += sizeof(LOH) + CEH.ComprSize + CEH.FileNameLen + CEH.ExtraLen;
        // plus DataDescriptor if present.
      if(DescUsed(CEH)) FullSize += sizeof(ZipDataDescriptor);
			// Seek past the extra field and the file comment.
			if(FileSeek(FInFileHandle, static_cast<__int64>(CEH.ExtraLen + CEH.FileComLen), 1 ) == -1)
				throw EZipBuilder(DS_FailedSeek);
		}
		FShowProgress = zspFull; //true;
    Report(zacCount,zprCompressed, "", EOC.TotalEntries);
		Report(zacSize,zprCompressed, "", FullSize);
		// Read for every zipped entry: The local header, variable data, fixed data
		// and if present the Data descriptor area.
		for(int i = 0; i < EOC.TotalEntries; i++)
		{
			// Seek to the first/next entry.
			FileSeek(FInFileHandle, static_cast<__int64>(MDZD->Items[i]->RelOffLocal), 0);

  		// First the local header.
      ReadJoin(&LOH, sizeof(LOH), DS_LOHBadRead);
			if(LOH.HeaderSig != LocalFileHeaderSig)	throw EZipBuilder(DS_LOHWrongSig);

			// Now the filename.
      ReadJoin( Buffer, LOH.FileNameLen, DS_LONameLen);
			// Set message info on the start of this new fileread now we still have the old filename.
			MsgStr = ZipLoadStr(RN_ProcessFile) + MDZD->Items[i]->FileName;
			// Check if the original path and/or filename needs to be changed.
			OrigFileName = SetSlash(MDZD->Items[i]->FileName,psdExternal);
			for(int m = 0; m < RenameList.Count; m++)
      {
				RenRec = (ZipRenameRec *)RenameList.Items[m];
        int k = UpperCase(OrigFileName).Pos(UpperCase(RenRec->Source));
        if(k)
        {
          OrigFileName.Delete(k, RenRec->Source.Length());
          OrigFileName.Insert(RenRec->Dest, k);
          LOH.FileNameLen = MDZD->Items[i]->FileNameLen = (unsigned short)OrigFileName.Length();
          for(k = 1; k <= OrigFileName.Length(); k++)
                  if(OrigFileName[k] == '\\') OrigFileName[k] = '/';
          MsgStr += ZipLoadStr(RN_RenameTo) + OrigFileName;
          delete[] MDZD->Items[i]->FileName;
          MDZD->Items[i]->FileName = new char[OrigFileName.Length() + 1];
          StrPLCopy(MDZD->Items[i]->FileName, OrigFileName, LOH.FileNameLen + 1);
          //check if new filename has extended chars if it is we need to set FHostVer to NTFS
          if(!(HasExtendedChars(OrigFileName) && !(MDZD->Items[i]->DiskStart & 0xFF00)))
          {
            FHostNum = static_cast<unsigned char>((MDZD->Items[i]->DiskStart & 0xFF00) >> 8);
            FHostVer = static_cast<unsigned char>(MDZD->Items[i]->DiskStart & 0xFF);
            OrigFileName = ConvertOEM(OrigFileName, cpdISO2OEM);
          }
          StrPLCopy(MDZD->Items[i]->FileName, OrigFileName, OrigFileName.Length() + 1);
          MDZD->Items[i]->FileNameLen = static_cast<short>(OrigFileName.Length());
        // Change Date and Time if needed.
          try
          {
            if(RenRec->DateTime)
            {
              // not allowed to change date on protected file
              if(MDZD->Items[i]->CRC32)
                  throw EZipBuilder(RN_InvalidDateTime);
              // test if valid date/time will throw error if not
              FileDateToDateTime(RenRec->DateTime);
              MDZD->Items[i]->DateTime = RenRec->DateTime;
            }
          }
          catch(...)
          {
             ShowZipMessage(RN_InvalidDateTime, MDZD->Items[i]->FileName);
          }
        }
			}
      Report(zacMessage,0,MsgStr,0);

			// Change Date and/or Time if needed.
			if(MDZD->Items[i]->DateTime)
			{
				LOH.ModifDate = HIWORD(MDZD->Items[i]->DateTime );
				LOH.ModifTime = LOWORD(MDZD->Items[i]->DateTime );
			}
			// Change info for later while writing the central dir.
			MDZD->Items[i]->RelOffLocal = FileSeek(FOutFileHandle, 0i64, 1);
      // Calculate the bytes we are going to write;
			TotalBytesWrite = sizeof(LOH) + LOH.FileNameLen + LOH.ExtraLen + //LOH.ComprSize;
                        MDZD->Items[i]->ComprSize;
      Report(zacItem,0,SetSlash(MDZD->Items[i]->FileName,psdExternal),TotalBytesWrite);
			// Write the local header to the destination.
			WriteJoin(&LOH, sizeof(LOH), DS_LOHBadWrite);

			// Write the filename.
			WriteJoin(MDZD->Items[i]->FileName, LOH.FileNameLen, DS_LOHBadWrite);

			// And the extra field
			if(CopyBuffer(FInFileHandle, FOutFileHandle, LOH.ExtraLen))
				throw EZipBuilder(DS_LOExtraLen);

			// Read and write Zipped data
			if(CopyBuffer(FInFileHandle, FOutFileHandle, /*LOH.*/ MDZD->Items[i]->ComprSize))
				throw EZipBuilder(DS_ZipData);

			// Read DataDescriptor if present.
			if(LOH.Flag & 0x0008)
				if(CopyBuffer(FInFileHandle, FOutFileHandle, sizeof(ZipDataDescriptor)))
					throw EZipBuilder(DS_DataDesc);
		}	// Now we have written al entries.

		// Now write the central directory with possibly changed offsets and filename(s).
		FShowProgress = zspNone; //false;
		for(int i = 0; i < EOC.TotalEntries; i++)
		{
			// Read a central header.
      ReadJoin(&CEH, sizeof(CEH), DS_CEHBadRead);
			if(CEH.HeaderSig != CentralFileHeaderSig)	throw EZipBuilder(DS_CEHWrongSig);

			// Change Date and/or Time if needed.
			if(MDZD->Items[i]->DateTime)
			{
				CEH.ModifDate = HIWORD(MDZD->Items[i]->DateTime);
				CEH.ModifTime = LOWORD(MDZD->Items[i]->DateTime);
			}
			// Now the filename.
      ReadJoin( Buffer, CEH.FileNameLen, DS_CENameLen);

			// Save the first Central directory offset for use in EOC record.
			if(!i)	EOC.CentralOffset = static_cast<unsigned long>(FileSeek(FOutFileHandle, 0i64, 1));

			// Change the central header info with our saved information.
			CEH.RelOffLocal = static_cast<unsigned long>(MDZD->Items[i]->RelOffLocal);
			EOC.CentralSize = EOC.CentralSize - CEH.FileNameLen + MDZD->Items[i]->FileNameLen;
			CEH.FileNameLen = MDZD->Items[i]->FileNameLen;
        //check if new filename has extended chars if it is we need to set ExtFileAttributes
        // and update version made by
      if(HasExtendedChars(MDZD->Items[i]->FileName) && !(MDZD->Items[i]->DiskStart & 0xFF00))
      {
        CEH.VersionMadeBy0 = DEF_HOST_VER & 0xFF;
        CEH.VersionMadeBy1 = (DEF_HOST_VER & 0xFF00) >> 8;
        CEH.ExtFileAtt = (((CEH.ExtFileAtt & FILE_ATTRIBUTE_DIRECTORY ? S_IFDIR + S_IEXEC : S_IFREG) +
                      S_IREAD +
                      (CEH.ExtFileAtt & FILE_ATTRIBUTE_READONLY ? 0 : S_IWRITE)) << 16) +
                      CEH.ExtFileAtt;
      }
      else
      {
        CEH.VersionMadeBy0 = static_cast<unsigned char>(MDZD->Items[i]->DiskStart & 0xFF) ;
        CEH.VersionMadeBy1 = static_cast<unsigned char>((MDZD->Items[i]->DiskStart & 0xFF00) >> 8);
      }
			// Write this changed central header to disk
			WriteJoin(&CEH, sizeof(CEH), DS_CEHBadWrite);

			// Write to destination the central filename.
			WriteJoin(MDZD->Items[i]->FileName, CEH.FileNameLen, DS_CEHBadWrite);

			// And the extra field
			if(CopyBuffer(FInFileHandle, FOutFileHandle, CEH.ExtraLen))
				throw EZipBuilder(DS_CEExtraLen);

			// And the file comment.
			if(CopyBuffer(FInFileHandle, FOutFileHandle, CEH.FileComLen))
				throw EZipBuilder(DS_CECommentLen);
		}
		// Write the changed EndOfCentral directory record.
		EOC.CentralDiskNo = 0;
		EOC.ThisDiskNo = 0;
		WriteJoin(&EOC, sizeof(EOC), DS_EOCBadWrite);

		// Skip past the original EOC to get to the ZipComment if present. v1.52m
		if(FileSeek(FInFileHandle, (int)sizeof( EOC ), 1) == -1)
			throw EZipBuilder(DS_FailedSeek);

		// And finally the archive comment
		// ==================Changed By Jim Turner=======
		if((FEOCComment != "") &&
        (FileWrite(FOutFileHandle, FEOCComment.c_str(),FEOCComment.Length()) < 0))
					throw EZipBuilder(DS_EOArchComLen, true);
	}
	catch (EZipBuilder &ers)
	{	// All Rename specific errors..
		ShowExceptionError(ers);
		Result = -7;
	}
	catch (const MEMEXCEPT &me)
	{		// All memory allocation errors.
		ShowZipMessage(GE_NoMem);
		Result = -8;
	}
	catch(const Exception &E)
	{
		ShowZipMessage(DS_ErrorUnknown, "\n" + E.Message );
		Result = -9;
	}
	catch ( ... )
	{							// The remaining errors, should not occur.
		ShowZipMessage(DS_ErrorUnknown);
		Result = -9;
	}
	delete[] Buffer;
	if(MDZD) delete MDZD;

	// Give final progress info at the end.
  Report(zacEndOfBatch,0,"",0);
  CloseFileHandle(FInFileHandle);
	if(FOutFileHandle != -1)
	{
		FileSetDate(FOutFileHandle, FDateStamp);
		FileClose(FOutFileHandle);
		if(Result) DeleteFile(OutFilePath);  // An error somewhere, OutFile is not reliable.
		else
		{
			EraseFile(FZipFileName, FHowToDelete);
			RenameFile(OutFilePath, FZipFileName);
			List();
		}
	}
  FZipBusy = false;
	StopWaitCursor();
	return Result;
}
// TZipWorker::Rename


#ifndef NO_SPAN
/*? TZipWorker::WriteSpan-------------------------------------------------------
1.77 31 August 2004 RA - allow cancel during read
1.77 18 August 2004 RA added All in overwrite dialog
1.77 5 August 2004 RA parameter added to allow different progress types
1.76 1 May 2004 RA init FInFileHAnlde to avoid OS exception in case of error
1.73 20 August 2003 RA bug fix when writing CEH to disk in larger block
1.73 (16 June 2003) bug fix setting of file handles and all errors in try catch
1.73 (30 May 2003) use of ReadJoin + Delimitpath
1.73 (18 May 2003) RA changed to use callback also for message
Function to read a Zip source file and write it back to one or more disks.
Return values:
 0			All Ok.
 -7			WriteSpan errors. See ZipMsgXX.rc and ZipMsg.h
 -8			Memory allocation error.
 -9			General unknown WriteSpan error.
*/
int __fastcall TZipWorker::WriteSpan(const String InFileName, const String OutFileName, bool UseXProgress )
{
	char *Buffer = NULL;
  bool BatchStarted = false;
	int Result = 0;
	struct ZipEndOfCentral    EOC;
	struct ZipLocalHeader     LOH;
	struct ZipDataDescriptor  DD;
	struct ZipCentralHeader   CEH;
  TMZipDataList* MDZD = NULL;
  FZipBusy = true;
  FErrCode = 0;    // 1.72 legal operation so errorcode should be 0
	FFreeOnDisk	= 0;
	FNewDisk = true;
	FDiskWritten = 0;
	FInFileHandle = -1;
	FOutFileName = OutFileName;
	FOutFileHandle = -1;
	FShowProgress = zspNone; //false;
	StartWaitCursor();
	try
	{
    if(ExtractFileName(OutFileName) == "") throw EZipBuilder(DS_NoOutFile);
	  FDrive = DelimitPath(ExtractFileDrive(OutFileName), false);
    // now change back to input and do the split
    FInFileName = InFileName;
    FTotalDisks = -1;
    FDiskNr	= 0;
		if(!FileExists(InFileName)) throw EZipBuilder(DS_NoInFile);
		OpenEOC(EOC, true); // fill EOC

		// Get the date-time stamp and save for later.
		FDateStamp = FileGetDate(FInFileHandle);

		// Now we now the number of zipped entries in the zip archive.
		// And we go back to the start the zip archive.
		if(FileSeek(FInFileHandle, 0i64, 0) == -1) throw EZipBuilder(DS_FailedSeek);

		// Allocate the read/write buffer.
		Buffer = new char[FBufSize];
		MDZD = new TMZipDataList(EOC.TotalEntries);
		// Write extended local Sig. needed for a spanned archive.
    FZipDiskAction = zdaOk; // make sure each split action starts with default action
		WriteSplit(&ExtLocalSig, 4, 0);

		// Read for every zipped entry: The local header, variable data, fixed data
		// and if present the Data decriptor area.
    if(UseXProgress)
    {
      Report(zacXItem, zprSplitting, ZipLoadStr(PR_Splitting), FFileSize);
      FShowProgress = zspExtra;
    }
    else
    {
      Report(zacCount, zprCompressed, "", EOC.TotalEntries);
      Report(zacSize, zprCompressed, "", FFileSize);
      FShowProgress = zspFull;
      BatchStarted = true;
    }

	for(int i = 0; i < EOC.TotalEntries; i++)
		{
      if(FCancel) throw EZipBuilder(DS_Canceled, false);
			// First the local header.
      ReadJoin(&LOH, sizeof(LOH),DS_LOHBadRead);
			if(LOH.HeaderSig != LocalFileHeaderSig) throw EZipBuilder(DS_LOHWrongSig);

			// Now the filename.
      ReadJoin(Buffer, LOH.FileNameLen, DS_LONameLen);

			// Save some information for later. ( on the last disk(s) ).
		 	MDZD->Items[i]->DiskStart = (unsigned short)FDiskNr;
			MDZD->Items[i]->FileNameLen = LOH.FileNameLen;
      MDZD->Items[i]->FileName = new char[LOH.FileNameLen + 1];
			StrLCopy(MDZD->Items[i]->FileName, Buffer, LOH.FileNameLen);
      MDZD->Items[i]->FileName[LOH.FileNameLen] = '\0';
			// Give message and progress info on the start of this new file read.
			if(OnMessage)
			{
        String MsgStr = ZipLoadStr(GE_CopyFile) +
                          SetSlash(MDZD->Items[i]->FileName, psdExternal);
				Report(zacMessage,0,MsgStr,0);
			}
			long TotalBytesWrite = sizeof(LOH) + LOH.FileNameLen + LOH.ExtraLen + LOH.ComprSize;
			if(DescUsed(LOH))	TotalBytesWrite += sizeof(DD);
      if(!UseXProgress) Report(zacItem, zprCompressed,
                               SetSlash(MDZD->Items[i]->FileName, psdExternal).c_str(),
                               TotalBytesWrite);
			// Write the local header to the destination.
			WriteSplit(&LOH, sizeof(LOH), sizeof(LOH) + LOH.FileNameLen + LOH.ExtraLen);

			// Save the offset of the LOH on this disk for later.
			MDZD->Items[i]->RelOffLocal = FDiskWritten - sizeof(LOH);

			// Write the filename.
			WriteSplit(Buffer, LOH.FileNameLen, 0);

			// And the extra field
			RWSplitData(Buffer, LOH.ExtraLen, DS_LOExtraLen);

			// Read Zipped data !!!For now assume we know the size!!!
			RWSplitData(Buffer, LOH.ComprSize, DS_ZipData);

			// Read DataDescriptor if present.
			if(DescUsed(LOH))	RWSplitData((char *)&DD, sizeof(DD), DS_DataDesc);
		}	// Ready, we have written al entries to disk.
    Report(zacMessage,0, ZipLoadStr(GE_CopyFile) + ZipLoadStr(DS_CopyCentral),0);
    if(!UseXProgress) Report(zacItem, 0,ZipLoadStr(DS_CopyCentral),
             EOC.CentralSize + sizeof(EOC) + EOC.ZipCommentLen);
		// Now write the central directory with changed offsets.
		unsigned short StartCentral = (unsigned short)FDiskNr;
		unsigned long  CentralOffset;
    unsigned int WritePtr = 0;
    unsigned int BlockSize;
    char*  CEHBlock = NULL;
		for(int i = 0; i < EOC.TotalEntries; i++)
		{
			if(FCancel) throw EZipBuilder(DS_Canceled, false);
			// Read a central header.
      ReadJoin(&CEH, sizeof(CEH),DS_CEHBadRead);
			if(CEH.HeaderSig != CentralFileHeaderSig) throw EZipBuilder(DS_CEHWrongSig);

			// Now the filename.
      ReadJoin(Buffer, CEH.FileNameLen, DS_CENameLen);

			// Change the central directory with information stored previously in MDZD.
			int k = MDZD->IndexOf(AnsiString(Buffer, CEH.FileNameLen));
			CEH.DiskStart = MDZD->Items[k]->DiskStart;
			CEH.RelOffLocal = static_cast<unsigned long>(MDZD->Items[k]->RelOffLocal);
      // check if we can write in larger block
      if(i && (FFreeOnDisk > (sizeof(CEH) + CEH.FileNameLen + CEH.ExtraLen + CEH.FileComLen))) // new method write in larger block
      {
        if(!CEHBlock)
        {
          BlockSize = min(FBufSize, static_cast<int>(FFreeOnDisk));
          CEHBlock = new char[BlockSize];
        }
        if(WritePtr > (BlockSize - sizeof(CEH) - CEH.FileNameLen - CEH.ExtraLen - CEH.FileComLen))
        {
          WriteSplit(CEHBlock, WritePtr, 0);// not enough room write to disk first
          WritePtr = 0;
          delete[] CEHBlock;
          CEHBlock = NULL;
          if(FFreeOnDisk > sizeof(CEH) + CEH.FileNameLen + CEH.ExtraLen + CEH.FileComLen)
          {
            BlockSize = min(FBufSize, static_cast<int>(FFreeOnDisk)); // adapt to new free on disk if needed
            CEHBlock = new char[BlockSize];
          }
        }
        if(CEHBlock)
        {
          memcpy(&CEHBlock[WritePtr], &CEH, sizeof(CEH));
          WritePtr += sizeof(CEH);
          memcpy(&CEHBlock[WritePtr], Buffer, CEH.FileNameLen);
          WritePtr += CEH.FileNameLen;
          if(CEH.ExtraLen || CEH.FileComLen)
          {
            WriteSplit(CEHBlock, WritePtr, 0);// write to disk first
            WritePtr = 0;
            delete[] CEHBlock;
            CEHBlock = NULL;
            RWSplitData(Buffer, CEH.ExtraLen, DS_CEExtraLen); // then extra field
            RWSplitData(Buffer, CEH.FileComLen, DS_CECommentLen); // then the comment
          }
        }
      }
      if(!CEHBlock) // not enough room use old method or first file
      {
        // Write this changed central header to disk
        // and make sure it fit's on one and the same disk.
        WriteSplit(&CEH, sizeof(CEH), sizeof(CEH) + CEH.FileNameLen + CEH.ExtraLen + CEH.FileComLen);

        // Save the first Central directory offset for use in EOC record.
        if(!i) CentralOffset = FDiskWritten - sizeof(CEH);

        // Write to destination the central filename and the extra field.
        WriteSplit(Buffer, CEH.FileNameLen, 0);

        // And the extra field
        RWSplitData(Buffer, CEH.ExtraLen, DS_CEExtraLen);

        // And the file comment.
        RWSplitData(Buffer, CEH.FileComLen, DS_CECommentLen);

      }
		}
    if(WritePtr)  WriteSplit(CEHBlock, WritePtr, 0); // write last part
    delete[] CEHBlock;
		// Write the changed EndOfCentral directory record.
		EOC.CentralDiskNo = StartCentral;
		EOC.ThisDiskNo = (unsigned short)FDiskNr;
		EOC.CentralOffset = CentralOffset;
		WriteSplit(&EOC, sizeof(EOC), sizeof(EOC) + EOC.ZipCommentLen);

		// Skip past the original EOC to get to the ZipComment if present. v1.52k
		if(FileSeek(FInFileHandle, (__int64)sizeof(EOC), 1) == -1)
			throw EZipBuilder(DS_FailedSeek);

		// And finally the archive comment itself.
		RWSplitData(Buffer, EOC.ZipCommentLen, DS_EOArchComLen);
		FShowProgress = zspNone; //false;
	}
	catch (EZipBuilder &ews)
	{	// All WriteSpan specific errors.
		ShowExceptionError(ews);
		Result = -7;
	}
	catch (const MEMEXCEPT &me)
	{		// All memory allocation errors.
		ShowZipMessage(GE_NoMem);
		Result = -8;
	}
	catch (const Exception &E)
	{
		ShowZipMessage(DS_ErrorUnknown, "\n" + E.Message);
		Result = -9;
	}
	catch ( ... )
	{							// The remaining errors, should not occur.
		ShowZipMessage(DS_ErrorUnknown);
		Result = -9;
	}
	// Give the last progress info on the end of this file read.
  if(BatchStarted) Report(zacEndOfBatch, 0,"",0);

	delete[] Buffer;
	if(MDZD) delete MDZD;
	FileSetDate(FOutFileHandle, FDateStamp);
  CloseFileHandle(FOutFileHandle);
  CloseFileHandle(FInFileHandle);
  if(!Result) // if Result is error then rename is not needed
  {
    AnsiString LastFileName = FOutFileName;
    CreateMVFileName(LastFileName, false);
    if(FDriveFixed && FSpanOptions.Contains(spCompatName))
    {  // do we need to change the name of the last file written?
      FZipDiskStatus.Clear();
      TModalResult Res;
      AnsiString MsgQ;
      if(FileExists(FOutFileName))
      {
      	MsgQ = ZipFmtLoadStr(DS_AskDeleteFile, ARRAYOFCONST((FOutFileName)));
        FZipDiskStatus = FZipDiskStatus << zdsSameFileName;
        if(OnStatusDisk && !FAnswerAll.Contains(zaaYesOvrwrt))
        {
        	FZipDiskAction = zdaOk;
          OnStatusDisk(Builder, FDiskNr, FOutFileName, FZipDiskStatus, FZipDiskAction);
          if(FZipDiskAction == zdaOk) Res = IDYES;
          else Res = IDNO;
        }
        else if(FAnswerAll.Contains(zaaYesOvrwrt)) Res = IDYES;
        else
              Res = ZipMessageDlg( ZipLoadStr(FM_Confirm),MsgQ,
                      zmtWarning + DHC_WrtSpnDel, TMsgDlgButtons() << mbYes << mbNo);

        if(!Res) ShowZipMessage(DS_NoMem, "");
        if(Res == IDNO) Report(zacMessage, DS_NoRenamePart,
                            ZipFmtLoadStr(DS_NoRenamePart, ARRAYOFCONST((LastFileName))),0);
        if(Res == IDYES) DeleteFile(FOutFileName);         // if it exists delete old one
      }
      RenameFile(LastFileName,FOutFileName); // rename will fail if old still exists
    }
  }
  FTotalDisks = static_cast<unsigned short>(FDiskNr);
  FZipBusy = false;
	StopWaitCursor();
	return Result;
}
//? TZipWorker::WriteSpan
#endif

#ifndef NO_SPAN
/*? TZipWorker::ReadSpan--------------------------------------------------------
1.77 31 August 2004 RA - allow cancel during read
1.77 5 August 2004 RA handle normal/ eXtended progress
1.75 18 February 2004 RP Allow >2G
1.73 (12 July 2003) RA made Report type zacItem for each file
1.73 (30 May 2003) use of ReadJoin + Delimitpath
1.73 (18 May 2003) RA use ZipAbort exception
Function to read a split up Zip source file from multiple disks and write it to one destination file.
Return values:
0			All Ok.
-7			ReadSpan errors. See ZipMsgXX.rc and ZipMsg.h
-8			Memory allocation error.
-9			General unknown ReadSpan error.
*/
int __fastcall TZipWorker::ReadSpan( const String InFileName, String &OutFilePath, bool UseXProgress )
{
	char   *Buffer = NULL;
	int     Result = 0;
  __int64 FullSize = 0;
  bool BatchStarted = false;
	struct  ZipEndOfCentral    EOC;
	struct  ZipLocalHeader     LOH;
	struct  ZipDataDescriptor  DD;
	struct  ZipCentralHeader   CEH;
  TMZipDataList *MDZD = NULL;
	FDrive = DelimitPath(ExtractFileDrive(InFileName), false);
  FDriveFixed = IsFixedDrive(FDrive); // 1.72
	FDiskNr = -1;
	FNewDisk = false;
	FShowProgress = zspNone; //false;
  FErrCode = 0; // 1.72
	FInFileName = InFileName;
	FInFileHandle = -1;
  FUnzBusy = true;
	StartWaitCursor();
	try
	{
		Buffer = new char[FBufSize];

		// If we don't have a filename we make one first.
		if(ExtractFileName(OutFilePath) != "")
		{
			EraseFile(OutFilePath, FHowToDelete);
			OutFilePath = ChangeFileExt(OutFilePath, ".zip");
		}
		else if((OutFilePath = MakeTempFileName()) == "")	throw EZipBuilder(DS_NoTempFile);
    // find volume with central dir
    if((Result = GetLastVolume(FInFileName,EOC, false)) == -1)
    {
      StopWaitCursor();
      FUnzBusy = false;
      delete[] Buffer; // avoid mem leak
      return -9;
    }// error already shown
    if(Result == 1) throw EZipAbort();
		// Create the output file.
    if((FOutFileHandle = FileCreate(OutFilePath)) == -1) throw EZipBuilder(DS_NoOutFile);
		// Get the date-time stamp and save for later.
		FDateStamp = FileGetDate(FInFileHandle);
		// Now we now the number of zipped entries in the zip archive
		// and the starting disk number of the central directory.
		FTotalDisks = EOC.ThisDiskNo;
		if(EOC.ThisDiskNo != EOC.CentralDiskNo)
			GetNewDisk(EOC.CentralDiskNo);	// We have to request a previous disk first.
		// We go to the start of the Central directory. v1.52j
		if(FileSeek(FInFileHandle, static_cast<__int64>(EOC.CentralOffset), 0) == -1)
			throw EZipBuilder(DS_FailedSeek);

		MDZD = new TMZipDataList(EOC.TotalEntries);
    FullSize = EOC.CentralSize + EOC.ZipCommentLen + sizeof(ZipEndOfCentral);
		// Read for every entry: The central header and save information for later use.
		for(int i = 0; i < EOC.TotalEntries; i++)
		{
		  if(FCancel) throw EZipBuilder(DS_Canceled, false);
			// Read a central header.
			while(FileRead(FInFileHandle, &CEH, sizeof(CEH)) != sizeof(CEH))
			{
				// It's possible that we have the central header split up.
				if(FDiskNr >= EOC.ThisDiskNo)	throw EZipBuilder(DS_CEHBadRead);
				// We need the next disk with central header info.
				GetNewDisk(FDiskNr + 1);
			}
			if(CEH.HeaderSig != CentralFileHeaderSig)	throw EZipBuilder(DS_CEHWrongSig);

			// Now the filename.
      ReadJoin(Buffer, CEH.FileNameLen, DS_CENameLen);
			// Save the file name info in the MDZD structure.
			MDZD->Items[i]->FileNameLen	= CEH.FileNameLen;
      MDZD->Items[i]->FileName = new char[CEH.FileNameLen + 1];
			StrLCopy(MDZD->Items[i]->FileName, Buffer, CEH.FileNameLen);
      MDZD->Items[i]->FileName[CEH.FileNameLen] = '\0';

			// Save the compressed size, we need this because WinZip sets this
			// sometimes to zero in the local header. New v1.52d
			MDZD->Items[i]->ComprSize = CEH.ComprSize;

			// We need the total number of bytes we are going to read for the progress event.
      FullSize += (sizeof(LOH) + CEH.ComprSize + CEH.FileNameLen + CEH.ExtraLen);
       // plus DataDescriptor if present.
      if(DescUsed(CEH)) FullSize += sizeof(DD);
			// Seek past the extra field and the file comment.
			if(FileSeek(FInFileHandle, static_cast<__int64>(CEH.ExtraLen + CEH.FileComLen), 1) == -1)
				throw EZipBuilder(DS_FailedSeek);
		}
		// Now we need the first disk and start reading.
		GetNewDisk(0);
    if(UseXProgress)
    {
      Report(zacXItem, zprJoining, ZipLoadStr(PR_Joining),FullSize);
      FShowProgress = zspExtra;
    }
    else
    {
      Report(zacCount, zprCompressed,"", EOC.TotalEntries + 1);
      Report(zacSize, zprCompressed,"", FullSize);
      FShowProgress = zspFull;
      BatchStarted = true;
    }
		// Read extended local Sig. first; is only present if it's a spanned archive.
		unsigned long ExtendedSig;
		if(FileRead(FInFileHandle, &ExtendedSig, 4) != 4 || ExtendedSig != ExtLocalSig)
			throw EZipBuilder(DS_ExtWrongSig);

		// Read for every zipped entry: The local header, variable data, fixed data
		// and if present the Data decriptor area.
		for(int i = 0; i < EOC.TotalEntries; i++)
		{
      if(FCancel) throw EZipBuilder(DS_Canceled, false);
			// First the local header.
			while(FileRead(FInFileHandle, &LOH, sizeof(LOH)) != sizeof(LOH))
			{
				// Check if we are at the end of a input disk not very likely but...
				if(FileSeek(FInFileHandle, 0i64, 1) != FileSeek(FInFileHandle, 0i64, 2))
					throw EZipBuilder(DS_LOHBadRead);
				// Well, it seems we are at the end, so get a next disk.
				GetNewDisk(FDiskNr + 1);
			}
			if(LOH.HeaderSig != LocalFileHeaderSig)	throw EZipBuilder(DS_LOHWrongSig);

			// Now the filename, should be on the same disk as the LOH record.
      ReadJoin(Buffer, LOH.FileNameLen , DS_LONameLen);

			// Change some info for later while writing the central dir.
			int k = MDZD->IndexOf(AnsiString(Buffer, LOH.FileNameLen));
			MDZD->Items[k]->DiskStart = 0;
			MDZD->Items[k]->RelOffLocal = FileSeek(FOutFileHandle, 0, 1);

			// Give message and progress info on the start of this new file read.
			String MsgStr = ZipLoadStr(GE_CopyFile) + SetSlash(MDZD->Items[k]->FileName, psdExternal);
      Report(zacMessage,0,MsgStr,0);
			long BytesToWrite = sizeof(LOH) + LOH.FileNameLen + LOH.ExtraLen + LOH.ComprSize;
			if(DescUsed(LOH)) BytesToWrite += sizeof(DD);
      if(!UseXProgress) Report(zacItem, zprCentral ,
                               SetSlash(MDZD->Items[k]->FileName, psdExternal).c_str(),
                               BytesToWrite);
			// Write the local header to the destination.
			WriteJoin(&LOH, sizeof(LOH), DS_LOHBadWrite);

			// Write the filename.
			WriteJoin(Buffer, LOH.FileNameLen, DS_LOHBadWrite);

			// And the extra field
			RWJoinData(Buffer, LOH.ExtraLen, DS_LOExtraLen);

			// Read Zipped data, if the size is not known use the size from the central header.
			if(!LOH.ComprSize) LOH.ComprSize = MDZD->Items[k]->ComprSize;	// New v1.52d
			RWJoinData(Buffer, LOH.ComprSize, DS_ZipData);

			// Read DataDescriptor if present.
			if(DescUsed(LOH)) RWJoinData((char *)&DD, sizeof(DD), DS_DataDesc);
		}	// Now we have written al entries to the (hard)disk.
		// Now write the central directory with changed offsets.
    if(!UseXProgress) Report(zacItem, zprCentral, ZipLoadStr(PR_CentrlDir), EOC.TotalEntries);
		for(int i = 0; i < EOC.TotalEntries; i++)
		{
			if(FCancel) throw EZipBuilder(DS_Canceled, false);
			// Read a central header which can be span more than one disk.
			while(FileRead(FInFileHandle, &CEH, sizeof(CEH)) != sizeof(CEH))
			{
				// Check if we are at the end of a input disk.
				if(FileSeek(FInFileHandle, 0i64, 1) != FileSeek(FInFileHandle, 0i64, 2))
					throw EZipBuilder(DS_CEHBadRead);
				// Well, it seems we are at the end, so get a next disk.
				GetNewDisk(FDiskNr + 1);
			}
			if(CEH.HeaderSig != CentralFileHeaderSig)	throw EZipBuilder(DS_CEHWrongSig);

			// Now the filename.
      ReadJoin(Buffer, CEH.FileNameLen, DS_CENameLen);
			// Save the first Central directory offset for use in EOC record.
			if(!i) EOC.CentralOffset = static_cast<unsigned long>(FileSeek(FOutFileHandle, 0i64, 1));

			// Change the central header info with our saved information.
			int k = MDZD->IndexOf(AnsiString(Buffer, CEH.FileNameLen));
			CEH.RelOffLocal = static_cast<unsigned long>(MDZD->Items[k]->RelOffLocal);
      CEH.DiskStart = 0;

			// Write this changed central header to disk
			// and make sure it fit's on one and the same disk.
			WriteJoin(&CEH, sizeof(CEH), DS_CEHBadWrite);

			// Write to destination the central filename and the extra field.
			WriteJoin(Buffer, CEH.FileNameLen, DS_CEHBadWrite);

			// And the extra field
			RWJoinData(Buffer, CEH.ExtraLen, DS_CEExtraLen);

			// And the file comment.
			RWJoinData(Buffer, CEH.FileComLen, DS_CECommentLen);
		}
		// Write the changed EndOfCentral directory record.
		EOC.CentralDiskNo = 0;
		EOC.ThisDiskNo = 0;
		WriteJoin(&EOC, sizeof(EOC), DS_EOCBadWrite);

		// Skip past the original EOC to get to the ZipComment if present. v1.52m
		if(FileSeek(FInFileHandle, static_cast<__int64>(sizeof(EOC)), 1) == -1)
			throw EZipBuilder(DS_FailedSeek);

		// And finally the archive comment itself.
		RWJoinData(Buffer, EOC.ZipCommentLen, DS_EOArchComLen);
	}
	catch (EZipBuilder &ers)
	{	// All ReadSpan specific errors.
		ShowExceptionError(ers);
		Result = -7;
	}
	catch (const MEMEXCEPT &me)
	{		// All memory allocation errors.
		ShowZipMessage(GE_NoMem);
		Result = -8;
	}
	catch(const Exception &E)
	{
		ShowZipMessage(DS_ErrorUnknown, "\n" + E.Message);
		Result = -9;
	}
	catch ( ... )
	{							// The remaining errors, should not occur.
		ShowZipMessage(DS_ErrorUnknown);
		Result = -9;
	}
	// Give final progress info at the end.
  if(BatchStarted) Report(zacEndOfBatch, 0, "", 0);
	delete[] Buffer;
	if(MDZD) delete MDZD;
  CloseFileHandle(FInFileHandle);
	if(FOutFileHandle != -1)
	{
		FileSetDate(FOutFileHandle, FDateStamp);
	  FileClose(FOutFileHandle);
		if(Result)
		{
			DeleteFile(OutFilePath);
			OutFilePath = "";
		}
	}
  FUnzBusy = false;
	StopWaitCursor();
	return Result;
}
//? TZipWorker::ReadSpan
#endif

#ifndef NO_SPAN
/*? TZipWorker::CheckForDisk-------------------------------------------------
1.77 31 August 2004 RP - allow cancel on hard drive
1.77 16 August 2004 RA allow unattended split on harddisk
*/
void __fastcall TZipWorker::CheckForDisk(bool Writing)
{
  FDriveFixed = IsFixedDrive(FDrive);
  if(FDriveFixed)
	{		// If it is a fixed disk we don't want a new one.
		FNewDisk = false;
    if(FCancel) throw EZipBuilder(DS_Canceled, false);
		return;
	}
	else Report(zacTick, 0, "", 0);              // just ProcessMessages

	int Res = IDOK;
  int MsgFlag = zmtWarning + DHC_SpanNxtW;   // or error?
	String MsgStr;

	// First check if we want a new one or if there is a disk (still) present.
	while(FNewDisk || (Res == IDOK && !GetDriveProps()))
	{
    if(FUnattended && !FDriveFixed) throw EZipBuilder(DS_NoUnattSpan);
		if(FDiskNr < 0)
		{		// -1=ReadSpan(), 0=WriteSpan()
			MsgStr = ZipLoadStr(DS_InsertDisk);
			MsgFlag = zmtError + DHC_SpanNxtR;//MsgFlag or MB_ICONERROR;
		}
		else
		{
			if(Writing)
			{		// Are we from ReadSpan() or WriteSpan()?
				// This is an estimate, we can't know if every future disk has the same space available and
				// if there is no disk present we can't determine the size unless it's set by MaxVolumeSize.
				__int64 SizeOfDisk = FMaxVolumeSize ? min( FSizeOfDisk, FMaxVolumeSize ) : FSizeOfDisk;
				if(SizeOfDisk > 0)
				{
					FTotalDisks = (unsigned short)max((__int64)FDiskNr,
                       (FFileSize + 4 + FFreeOnDisk1) / SizeOfDisk);	//v1.52j
					MsgStr = ZipFmtLoadStr(DS_InsertVolume, ARRAYOFCONST((FDiskNr + 1, FTotalDisks + 1)));
				}
				else
				{
					FTotalDisks = -1;
					MsgStr = ZipFmtLoadStr(DS_InsertAVolume, ARRAYOFCONST((FDiskNr + 1)));
				}
			}
			else MsgStr = ZipFmtLoadStr(DS_InsertVolume,  ARRAYOFCONST((FDiskNr + 1, FTotalDisks + 1)));
		  MsgStr += ZipFmtLoadStr(DS_InDrive, ARRAYOFCONST((FDrive.SubString(1, 1))));
    }
    if(!(FDiskNr == 0 && GetDriveProps() && Writing)) // don't ask for 1st disk if one is present
    {
      if(OnGetNextDisk)
      {	// v1.60L
        bool AbortAction = false;
        OnGetNextDisk(Builder, FDiskNr + 1,
                          FTotalDisks + 1, FDrive.SubString( 1, 1), AbortAction);
        Res = AbortAction ? IDABORT : IDOK;
      }
      else Res = ZipMessageDlg(AnsiString(""), MsgStr, MsgFlag,
                               TMsgDlgButtons() << mbOK << mbCancel);

    }
		FNewDisk = false;
	}
	// Check if user pressed Cancel or Mem is running out.
	if(Res != IDOK) throw EZipBuilder(DS_Canceled, false);
	if(!Res) throw EZipBuilder(DS_NoMem);
}
//? TZipWorker::CheckForDIsk
#endif

#ifndef NO_SPAN
/*? TZipWorker::CreatMVFileName---------------------------------------------------
1.77 19 August 2004 RA Allow creation of >99 parts with compatnames
*/
void __fastcall TZipWorker::CreateMVFileName(String& FileName, bool StripPartNbr)
{  // changes filename into multi volume filename
  if(FSpanOptions.Contains(spCompatName))
  {
    String ext;
    if(FDiskNr != FTotalDisks)
    {
      if(FDiskNr < 9) ext = ".z0";
      else ext = ".z";
      ext += IntToStr(FDiskNr + 1);
    }
    else ext = ".zip";
    FileName = ChangeFileExt(FileName, ext);
  }
  else
  {
    int StripLen = StripPartNbr ? 3 : 0;
    FileName = FileName.SubString( 1, FileName.Length() - ExtractFileExt( FileName ).Length() - StripLen )
                             + IntToStr(1001 + FDiskNr).SubString(2, 3)
                             + ExtractFileExt(FileName);
  }
}
//? TZipWorker::CreateMVFileName
#endif

/*? TZipWorker::OpenEOC--------------------------------------------------------
1.77 19 August 2004 RA Allow >99 parts with compatnames
1.76 30 May 2004 Check file <4G
1.75 18 February 2004 allow >2G
1.73.3.2 8 Oct 2003 RA RP allow max 512 whitespace chars at end of file and save comment
changed 1.73 (22 May 2003) RA use ReadJoin
Function to find the EOC record at the end of the archive (on the last disk.)
We can get a return value( true::Found, false::Not Found ) or an exception if not found.
*/
bool __fastcall TZipWorker::OpenEOC(struct ZipEndOfCentral &EOC, bool DoExcept)
{
  I64Rec SizeOfFile;
	unsigned long  Sig;
	int			   DiskNo = 0;
	bool			   First = false;
	AnsiString ZipBuf;

	FZipComment = "";
	FZipEOC     =  0;
  FEOCComment = "";
	// Open the input archive, presumably the last disk.
	if((FInFileHandle = FileOpen(FInFileName, fmShareDenyWrite | fmOpenRead )) == -1)
	{
		if(DoExcept) throw EZipBuilder(DS_NoInFile);
		ShowZipMessage(DS_FileOpen);
		return false;
	}
	// First a check for the first disk of a spanned archive,
	// could also be the last so we don't issue a warning yet.
	if(FileRead(FInFileHandle, &Sig, 4) == 4 && Sig == ExtLocalSig &&
		 FileRead(FInFileHandle, &Sig, 4) == 4 && Sig == LocalFileHeaderSig)
	{
		First = true;
		FIsSpanned = true;
	}
	// Next we do a check at the end of the file to speed things up if
	// there isn't a Zip archive comment.
  SizeOfFile.I = FileSeek(FInFileHandle, -(__int64)(signed)(sizeof(EOC)), 2);
  if(SizeOfFile.I >= 0)
    SizeOfFile.I += sizeof(ZipEndOfCentral);
  if(SizeOfFile.Rec.Hi)

  {
    CloseFileHandle(FInFileHandle);
    if(Verbose)  Report(zacMessage, 0, "Opening EOC seek error" +
                            IntToStr(SizeOfFile.I / (1024 * 1024)) + "Mb", 0);
    if(DoExcept)
    {
      if(SizeOfFile.I != -1) throw EZipBuilder(LI_FileTooBig);
      throw  EZipBuilder(DS_NoValidZip);
    }
    return false;
  }
  FFileSize = SizeOfFile.Rec.Lo;
  if(FFileSize >= sizeof(ZipEndOfCentral))
	{
		FRealFileSize = FFileSize;		// There could follow a correction on FFileSize.
		if(FileRead(FInFileHandle, &EOC, sizeof(EOC)) == sizeof(EOC) &&
				EOC.HeaderSig == EndCentralDirSig)
		{
			FZipEOC = FFileSize - sizeof(EOC);
      return true;
		}
	}
	// Now we try to find the EOC record within the last 65535 + sizeof( EOC ) bytes
	// of this file because we don't know the Zip archive comment length at this time.
	try
	{
		int Size = min(FFileSize, (long)(65535 + sizeof(EOC)));
		ZipBuf.SetLength(Size);
		if(FileSeek(FInFileHandle, (__int64)(-Size), 2) == -1) throw EZipBuilder(DS_FailedSeek);
    ReadJoin(ZipBuf.c_str(), Size,DS_EOCBadRead);
		for(int i = Size - sizeof(EOC)- 1; i >= 0; --i)
    {
      ZipEndOfCentral* pEOC = reinterpret_cast<ZipEndOfCentral*>(ZipBuf.c_str() + i);
      if(pEOC->HeaderSig == EndCentralDirSig)
			{
				FZipEOC = FFileSize - Size + i;
				memcpy(&EOC, ZipBuf.c_str() + i, sizeof(EOC));
        //If we have ZipComment: Save it,No codepage translation yet, wait for CEH read.
        if(EOC.ZipCommentLen)
					FEOCComment = ZipBuf.SubString(i + sizeof(EOC) + 1 , EOC.ZipCommentLen);
				// Check if we really are at the end of the file, if not correct the filesize
				// and give a warning. (It should be an error but we are nice.)
				if(i + sizeof(EOC) + EOC.ZipCommentLen - Size)
				{
					FFileSize += (i + sizeof(EOC) + (unsigned)EOC.ZipCommentLen - Size);
					// Now we need a check for WinZip Self Extractor which makes SFX files which
					// allmost always have garbage at the end (Zero filled at 512 byte boundary!)
					// In this special case 'we' don't give a warning.
					bool ShowGarbageMsg = true;
          if(FRealFileSize - (unsigned)FFileSize < 512)
					{
						int j = i + sizeof(EOC) + EOC.ZipCommentLen;
            while(ZipBuf[j] < '/' && j <= Size) j++;
						if(j == Size + 1) ShowGarbageMsg = false;
					}
					if(ShowGarbageMsg) ShowZipMessage(LI_GarbageAtEOF);
				}
				return true;
      }
		}
	}
	catch ( ... )
	{
		if(DoExcept) throw;
	}
  CloseFileHandle(FInFileHandle);    // don't leave it open
	if(DoExcept)
	{ // Get the volume number if it's disk from a set. - 1.72 moved
    if(FVolumeName.SubString(1, 8) == "PKBACK# ")
		      DiskNo = StrToIntDef(FVolumeName.SubString(9, 3), 0);
    else
    {
      AnsiString	ext = UpperCase(ExtractFileExt(FInFileName));
			DiskNo = 0;
      if(ext.SubString(1,2) == ".Z")
           DiskNo = StrToIntDef(ext.SubString(2, ext.Length() - 2),0);
      if(DiskNo <= 0)
           DiskNo = StrToIntDef(FInFileName.SubString(FInFileName.Length() -
                                                   ext.Length() - 3 +1, 3),0);

    }
		if(!First && DiskNo) throw EZipBuilder(DS_NotLastInSet, DiskNo);
		throw EZipBuilder((First) ? ((DiskNo == 1) ? DS_FirstInSet : DS_FirstFileOnHD) : DS_NoValidZip);
	}
	return false;
}
//? TZipWorker::OpenEOC

/*? TZipWorker::NameOfPart-----------------------------------------------------
1.77 19 August 2004 RA Allow >99 parts with compatnames
1.73 18 September 2003 RA accept file extension .zip for compat as valid last part
1.73 16 September 2003 RP new function to avoid negative part numbers
*/
#ifndef NO_SPAN
AnsiString __fastcall TZipWorker::NameOfPart(AnsiString fn, bool compat)
{
  TSearchRec SRec;
  AnsiString Result = "";
  int n, r;
  AnsiString fs;
  if(compat) fs = fn + ".z??*";
  else fs = fn + "???.zip";
  r = FindFirst(fs , faAnyFile, SRec);
  while(r == 0)
  {
    if(compat)
    {
      fs = UpperCase(ExtractFileExt(SRec.Name).SubString(3,20));
      if(fs == "IP") n = 99999;
      else n = StrToIntDef(fs, 0);
    }
    else n = StrToIntDef(SRec.Name.SubString(SRec.Name.Length()-6,3), 0);
    if(n > 0)
    {
      Result = SRec.Name; // possible name
      break;
    }
    r = FindNext(SRec);
  }
  FindClose(SRec);
  return Result;
}
#endif
//?TZipWorker::NameOfPart

#ifndef NO_SPAN
/*? TZipWorker::GetLastVolume--------------------------------------------------
1.77 21 August 2004 RP improve orphan file tolerance
1.76 9 June 2004 RA fix finding part of non-split file
1.76 25 April 2004 RA do not throw exception for MVolume part with faultly/no EOC
1.75 16 March 2004 RA stop exception if unwanted file does not exist
1.73 16 September 2003 RA use of NameOfPart function
1.73 12 September 2003 RA do not accept negative partnumbers
1.73  (9 July 2003) RA creation of first part name improved
*/
int __fastcall TZipWorker::GetLastVolume(String FileName,ZipEndOfCentral& EOC, bool AllowNotExists)
// Locate last of multi volume or last disk of spanned
// returns
// 0 = OK
// -1 = error
// 1 = user aborted loop = avoid asking for last disk if not needed
{
  AnsiString Fname;
  AnsiString sName;
  int PartNbr = -1;
  TSearchRec SRec;
  FInFileHandle = -1;
  FDrive = ExtractFileDrive(ExpandFileName(FileName)) + "\\";
  AnsiString Ext = UpperCase(ExtractFileExt(FileName));
  AnsiString Path = ExtractFilePath(FileName);
  bool FMVolume= false;
  try
  {
    FDriveFixed = IsFixedDrive(FDrive);
    GetDriveProps();  // check valid drive
    if(!FileExists(FileName)) // if file exsits we have one volume zip file or spanned on removable disks
    {
      Fname = FileName.SubString(1, FileName.Length() - Ext.Length());// remove extension
      FMVolume = true; // file did not exist maybe it is a multi volume
      if(FDriveFixed) // if file not exists on harddisk then only Multi volume parts are possible
      {  // filename is of type ArchiveXXX.zip
        // MV files are series with consecutive partnbrs in filename, highest number has EOC
        if(Ext == ".ZIP")
        {
          bool Finding = true;
          while(Finding)
          {
            AnsiString s = Fname + IntToStr(1002 + PartNbr).SubString(2, 3) + ".zip";
            if(!FileExists(s))
            {
              PartNbr = -1;
              break;
            }
            ++PartNbr;
            FInFileName = s;
            if(OpenEOC(EOC, false))
            {
              Finding = false;
              if(EOC.ThisDiskNo != PartNbr) PartNbr = -1; // not valid last part
            }
            CloseFileHandle(FInFileHandle);
          }
        }
        if(PartNbr == -1)
        {
          if (AllowNotExists) return 1;
          throw EZipBuilder(DS_FileOpen); // non found
        }
        FileName = Fname + IntToStr(1001 + PartNbr).SubString(2, 3) + ".zip";
        // check if filename.z01 exists then it is part of MV with compat names and cannot be used
        if(FileExists(ChangeFileExt(FileName, ".z01"))) throw EZipBuilder(DS_FileOpen); // cannot be used
      }
      else // if we have an MV archive copied to a removable disk
      {
        // accept any MV filename on disk
        sName = NameOfPart(Fname, false);
        if(sName == "") sName = NameOfPart(Fname, true);
        if(sName == "")
        {
        	if(AllowNotExists) return 1;
        	throw EZipBuilder(DS_FileOpen);
        }
        FileName = Path + sName;
      }
    }
    // zip file exists or we got an acceptable part in multivolume or spanned archive
    FInFileName = FileName; // use class variable for other functions
    while(!OpenEOC(EOC, false)) // does this part contains the central dir
    {  // it is not the disk with central dir so ask for the last disk
      CloseFileHandle(FInFileHandle);
      CheckForDisk(false); // does the request for new disk
      if(FDriveFixed)
      {
        if(FMVolume)
        {
          if(AllowNotExists) return -1;
          throw EZipBuilder(DS_FileOpen); // it was not a valable part
        }
        AllowNotExists = false; // next error needs to be displayed always
        throw EZipBuilder(DS_NoValidZip); // file with EOC is not on fixed disk
      }
      // for spanned archives on cdrom's or floppies
      if(OnGetNextDisk)
      {	// v1.60L
        bool Abort = false;
        OnGetNextDisk(Builder, 0, 0, FDrive.SubString( 1, 1), Abort);
        if(Abort)  // we allow abort by the user
        {
          CloseFileHandle(FInFileHandle);
          return 1;
        }
        GetDriveProps();  // check drive spec and get volume name
      }
      else
      { // if no event handler is used
        FNewDisk = true;
        FDiskNr = -1; // read operation
        CheckForDisk(false); // ask for new disk
      }
      if(FMVolume)
      { // we have removable disks with multi volume archives get the file name on this disk
        sName = NameOfPart(Fname, FSpanOptions.Contains(spCompatName));
        if(sName == "")
        { // no acceptable file on this disk so not a disk of the set
          ShowZipMessage(DS_FileOpen);
          return -1; //error
        }
        FInFileName = Path + sName;
      }
    }
    if(FMVolume) // got a multi volume part so need more checks
    {
      // is this first file of a spanned
      if(!FIsSpanned && (EOC.ThisDiskNo == 0) && (PartNbr >= 0)) throw EZipBuilder(DS_FileOpen);
      // part and EOC equal?
      if(FDriveFixed && (EOC.ThisDiskNo != PartNbr)) throw EZipBuilder(DS_NoValidZip);
    }
  }
  catch(EZipBuilder &ezl)
	{
    if(!AllowNotExists) ShowExceptionError(ezl);
    FInFileName = ""; // don't use the file
    CloseFileHandle(FInFileHandle);
    return -1;
	}
  return 0;
}
// TZipWorker::GetLastVolume
#endif

/*? TZipWorker::GetNewDisk------------------------------------------------------
1.77 19 August 2004 RP - improve logic
1.73 (28 July 2003) RP / RA added verbose message
*/
#ifdef NO_SPAN
void __fastcall TZipWorker::GetNewDisk(int /*DiskSeq*/)
{
  throw EZipBuilder(DS_NoDiskSpan);
#else
void __fastcall TZipWorker::GetNewDisk(int DiskSeq)
{
	FileClose(FInFileHandle);	// Close the file on the old disk first.
	FDiskNr = DiskSeq;
  while(true)
  {
      do
      {
        FNewDisk = true;
        FInFileHandle = -1; //to prevent second close after user choses cancel
        CheckForDisk(false);
      }while(!IsRightDisk());
      if(Verbose)
        Report(zacMessage, 0,ZipFmtLoadStr(TM_GetNewDisk, ARRAYOFCONST((FInFileName))), 0);
      // Open the the input archive on this disk.
      if((FInFileHandle = FileOpen(FInFileName, fmShareDenyWrite | fmOpenRead)) != -1)
          break; // found
      if(FDriveFixed) throw EZipBuilder(DS_NoInFile);
      else ShowZipMessage(DS_NoInFile, "");
  }
#endif
}
//? TZipWorker::GetNewDisk

#ifndef NO_SPAN
/*? TZipWorker::WriteSplit------------------------------------------------------
1.77 23 Sep 2004 RP - use YesToAll
1.77 18 August 2004 RA added All in overwrite dialog
1.77 16 August 2004 RA allow unattended split on harddisk
1.77 15 August 2004 RP progress
1.73.3.3 15 October 2003 RP Remove duplicated code
1.73.3.2 9 Oct 2003 RA set KeepFreeOnDisk1 and KeepFreeOnAlldisk to sector boundaries
1.73 11 July 2003 RP corrected asking disk status
1.73 (7 July 2003) RA changed OnMessage and OnProgress to Report calls
1.73 (18 May 2003) RP changed to use ZipAbort
This function actually writes the zipped file to the destination while taking care
of disk changes and disk boundary crossings.
In case of an write error or user abort an EZipBuilder Exception is thrown.
*/
void __fastcall TZipWorker::WriteSplit(const void *Buffer, int Len, int MinSize)
{
	int Res, MaxLen;
	char *Buf = (char *)Buffer;  // Used if Buffer doesn't fit on the present disk.
  Report(zacTick, 0, "", 0);
  if(Cancel) throw EZipBuilder(DS_Canceled, false);
	while(true)
	{	// Keep writing until error or buffer is empty.
		// Check if we have an output file already opened, if not: create one, do checks gather info.
		if(FOutFileHandle == -1)
		{
			String DiskFile = FOutFileName, MsgQ;
      CheckForDisk(true);
			// If we write on a fixed disk the filename must change.
			// We will get something like: FileNamexxx.zip where xxx is 001,002 etc.
      // if CompatNames are used we get FileName.zxx wher xx is 01, 02 etc..
			if(FDriveFixed || FSpanOptions.Contains(spNoVolumeName))
                                  CreateMVFileName(DiskFile, false);
			if(!FDriveFixed && FSpanOptions.Contains(spWipeFiles))
			{
				if(!OnGetNextDisk || (OnGetNextDisk && FZipDiskAction == zdaErase))
				{	// Added v1.60L
	 				// Do we want a format first? To be save we don't do this on a harddisk.
					FDriveNr = FDrive.UpperCase()[1] - 'A';
          if(FSpanOptions.Contains(spNoVolumeName))
              FVolumeName = "ZipSet_" + IntToStr(FDiskNr); //default name
          else FVolumeName = "PKBACK# " + IntToStr(1001 + FDiskNr).SubString(2, 3);
					// Ok=6, NoFormat=-3 , Cancel=-2, Error =-1
					switch(ZipFormat())
					{   // Start the format and wait until finished...
						case -1:  throw EZipBuilder(DS_Canceled, true);			// Error
						case -2:  throw EZipAbort();	// Canceled
            default: DiskFreeAndSize(3); // reread disksize values after format
					}
				}
			}
			// Do we want to overwrite an existing file
      int DiskSeq;
      if(FSpanOptions.Contains(spNoVolumeName)) DiskSeq = FDiskNr + 1;
    	else DiskSeq =  StrToIntDef(FVolumeName.SubString(9, 3), 1);
      FZipDiskStatus.Clear();
      if(FileExists(DiskFile))
      {
				// A more specific check if we have a previous disk from this set. Not too smart but...
				if(FileAge(DiskFile) == FDateStamp && DiskSeq - 1 < FDiskNr && !FDriveFixed) // don't ask for fixed drive
				{
					MsgQ = ZipFmtLoadStr(DS_AskPrevFile, ARRAYOFCONST((DiskSeq)));
					FZipDiskStatus << zdsPreviousDisk;
				}
				else
				{
					MsgQ = ZipFmtLoadStr(DS_AskDeleteFile, ARRAYOFCONST((DiskFile)));
					FZipDiskStatus << zdsSameFileName;
				}
			}
      else
      {
      	if(!FDriveFixed)
      	{
        	if(FSizeOfDisk - FFreeOnDisk) FZipDiskStatus << zdsHasFiles;		// But not the same name
        	else FZipDiskStatus << zdsEmpty;
        }
      }
      if(OnStatusDisk && !FAnswerAll.Contains(zaaYesOvrwrt))
      {
      	FZipDiskAction = zdaOk;					// The default action
        OnStatusDisk(Builder, DiskSeq, DiskFile, FZipDiskStatus, FZipDiskAction);
        switch(FZipDiskAction)
        {
        	case zdaCancel: Res = IDCANCEL;	break;
          case zdaReject: Res = IDNO;		break;
          case zdaYesToAll: FAnswerAll = FAnswerAll << zaaYesOvrwrt;
          case zdaErase:
          case zdaOk:		 Res = IDOK; break;
          default : Res = IDOK;
        }
      }
      else
      {
        if((FZipDiskStatus.Contains(zdsPreviousDisk) || FZipDiskStatus.Contains(zdsSameFileName)) &&
           !FAnswerAll.Contains(zaaYesOvrwrt))
        {  // if no OnStatusDisk event
          Res = ZipMessageDlg(ZipLoadStr(FM_Confirm),MsgQ, zmtWarning + DHC_SpanOvr,
                              TMsgDlgButtons() << mbYes << mbNo << mbCancel << mbYesToAll);
          if(Res == mrYesToAll)
          {
          	FAnswerAll = FAnswerAll << zaaYesOvrwrt;
          	Res = IDOK;
          }
        }
        else Res = IDOK;
      }
      if(!Res) throw EZipBuilder(DS_NoMem);
      if(Res == IDCANCEL) throw EZipBuilder(DS_Canceled, false);
      if(Res == IDNO)
      {		// we will try again...
        FDiskWritten = 0;
        FNewDisk = true;
        continue;
      }
			// Create the output file.
			if((FOutFileHandle = FileCreate(DiskFile)) == -1)
      {//change proposed by Pedro Araujo
        MsgQ = ZipLoadStr( DS_NoOutFile);
        Res = ZipMessageDlg("", MsgQ, zmtError + DHC_SpanNoOut,
                            TMsgDlgButtons() << mbRetry << mbCancel);
        if(!Res) throw EZipBuilder(DS_NoMem );
				if(Res != IDRETRY ) throw EZipAbort();
				FDiskWritten = 0;
				FNewDisk = true;
				continue;
      }
			// Get the free space on this disk, correct later if neccessary.
			DiskFreeAndSize(1);
			// Set the maximum number of bytes that can be written to this disk(file).
			if(FMaxVolumeSize) FFreeOnDisk = min(FMaxVolumeSize, FFreeOnDisk);
      // Set the maximum number of bytes that can be written to this disk(file).
      // Reserve space on/in all the disk/file.
      if(!FDiskNr && (FFreeOnDisk1 || FFreeOnAllDisks)) // only one calculation if needed
      {  // set these properties on sector boundaries
        DWORD	SectorsPCluster, BytesPSector, FreeClusters, TotalClusters;
        if(GetDiskFreeSpace(FDrive.c_str(), &SectorsPCluster, &BytesPSector, &FreeClusters, &TotalClusters))
        {
          if(FFreeOnDisk1  % BytesPSector)
            FFreeOnDisk1 = ((FFreeOnDisk1 / BytesPSector) + 1) * BytesPSector;
          if(FFreeOnAllDisks  % BytesPSector)
            FFreeOnAllDisks = ((FFreeOnAllDisks / BytesPSector) + 1) * BytesPSector;
        }
      }
      FFreeOnDisk -= FFreeOnAllDisks;
			// Reserve space on/in the first disk(file).
			if(!FDiskNr) FFreeOnDisk -= FFreeOnDisk1;

			// Do we still have enough free space on this disk.
			if(FFreeOnDisk < FMinFreeVolSize)
			{	// No, too bad...
        CloseFileHandle(FOutFileHandle);
				DeleteFile(DiskFile);
				if(OnStatusDisk)
				{						// v1.60L
          int DiskSeq;
          if(FSpanOptions.Contains(spNoVolumeName)) DiskSeq = FDiskNr + 1;
					else DiskSeq = StrToIntDef(FVolumeName.SubString(9, 3), 1);
					FZipDiskAction = zdaOk;					// The default action
					FZipDiskStatus = TZipDiskStatus() << zdsNotEnoughSpace;
					OnStatusDisk(Builder, DiskSeq, DiskFile, FZipDiskStatus, FZipDiskAction);
					switch(FZipDiskAction)
					{
						case zdaCancel: Res = IDCANCEL;	break;
						case zdaOk:
						case zdaErase:
						case zdaReject: Res = IDRETRY;
					}
				}
				else
				{
          MsgQ = ZipLoadStr(DS_NoDiskSpace);
				  Res = ZipMessageDlg("", MsgQ, zmtError + DHC_SpanSpace,
                              TMsgDlgButtons() << mbRetry << mbCancel);
				}
				if(!Res) throw EZipBuilder(DS_NoMem);
				if(Res != IDRETRY ) throw EZipAbort();
				FDiskWritten = 0;
				FNewDisk = true;
				continue;
			}
			// Set the volume label of this disk if it is not a fixed one.
			if(!FDriveFixed && !FSpanOptions.Contains(spNoVolumeName))
			{
				FVolumeName = "PKBACK# " + IntToStr(1001 + FDiskNr).SubString(2, 3);
				if(!::SetVolumeLabel(FDrive.c_str(), FVolumeName.c_str()))
					throw EZipBuilder(DS_NoVolume);
			}
		}
		// Check if we have at least MinSize available on this disk,
		// headers are not allowed to cross disk boundaries. ( if zero than don't care.)
		if(MinSize && MinSize > FFreeOnDisk)
		{
			FileSetDate(FOutFileHandle, FDateStamp);
      CloseFileHandle(FOutFileHandle);
			FDiskWritten = 0;
			FDiskNr++;
			FNewDisk = true;
			continue;
		}
		// Don't try to write more bytes than allowed on this disk.
		MaxLen = min(Len, static_cast<int>(FFreeOnDisk));
		Res = FileWrite(FOutFileHandle, Buf, MaxLen);
		// Give some progress info while writing.
		// While processing the central header we don't want messages.
	  if(FShowProgress != zspNone)
    {
      if(FShowProgress == zspExtra) Report(zacXProgress, 0, "", MaxLen);
      else Report(zacProgress, 0, "", MaxLen);
    }
		if(Res == -1) throw EZipBuilder(DS_NoWrite);	// A write error (disk removed?).
    FDiskWritten += Res;
		FFreeOnDisk  -= MaxLen;
		if(MaxLen == Len) break;

		// We still have some data left, we need a new disk.
		FileSetDate(FOutFileHandle, FDateStamp);
    CloseFileHandle(FOutFileHandle);
		FFreeOnDisk = FDiskWritten = 0;
		FDiskNr++;
		FNewDisk = true;
		Buf += MaxLen;
		Len -= MaxLen;
	}
}
// TZipWorker::WriteSplit
#endif

/*? TZipWorker::WriteJoin-------------------------------------------------------
1.77 13 August 2004 RP progress
1.73 (16 May 2003) RA changed to use callback
*/
void __fastcall TZipWorker::WriteJoin(void *Buffer, int BufferSize, int DSErrIdent)
{
	if(FileWrite(FOutFileHandle, Buffer, BufferSize) != BufferSize)
		throw EZipBuilder(DSErrIdent);

	// Give some progress info while writing.
	// While processing the central header we don't want messages.
  if(FShowProgress == zspExtra) Report(zacXProgress, 0, "", BufferSize);
  else if(FShowProgress == zspFull) Report(zacProgress, 0, "", BufferSize);
}
//? TZipWorker::WriteJoin

#ifndef NO_STREAM
/*?TZipWorker::AddStreamToFile--------------------------------------------------
1.76 10 June 2004 RA fix access to FSpecArgs
 1.73 (29 July 2003) RA changed trace messages
 FileAttr are set to 0 as default.
 FileAttr can be one or a logical combination of the following types:
 FILE_ATTRIBUTE_ARCHIVE, FILE_ATTRIBUTE_HIDDEN, FILE_ATTRIBUTE_READONLY, FILE_ATTRIBUTE_SYSTEM.
 FileName is as default an empty string.
 FileDate is default the system date.*/
int __fastcall TZipWorker::AddStreamToFile( String FileName, DWORD FileDate, DWORD FileAttr )
{
  if(FileName.Length())
  {
    FFSpecArgs->Clear();
    FFSpecArgs->Add(FileName);
  }
  if(!FileDate)
  {
    SYSTEMTIME st;
    FILETIME   ft;
    WORD       FatDate, FatTime;
    ::GetLocalTime(&st);
    ::SystemTimeToFileTime(&st, &ft);
    ::FileTimeToDosDateTime(&ft, &FatDate, &FatTime);
    FileDate = ((DWORD)FatDate << 16) + FatTime;
  }
  FSuccessCnt = 0;
  // Check if wildcards are set.
  if(FFSpecArgs->Count)
  {
    if( FFSpecArgs->Strings[0].AnsiPos("*") || FFSpecArgs->Strings[0].AnsiPos("?"))
        ShowZipMessage(AD_InvalidName);
    else ExtAdd(1, FileDate, FileAttr, NULL);
  }
  else ShowZipMessage(AD_NothingToZip);
	return FErrCode;
}
// TZipWorker::AddStreamToFile
#endif

/*? TZipWorker::List-----------------------------------------------------------
1.76  9 June 2004 add Trace messages
1.76 28 April 2004 test Active
1.75 13 March 2004 give progress
1.75 18 February 2004 RP allow file>2G
1.73.3.2 Oct 11 2003 RP convert saved comment
1.73 (26 July 2003) RA - added test for empty ZipFileName
1.73 (15 July 2003) RP / RA ReadJoin
*/
int __fastcall TZipWorker::List(void) // All work is local - no DLL calls.
{
	struct ZipEndOfCentral  EOC;
	struct ZipCentralHeader CEH;
	ZipDirEntry *pzd = NULL;
	__int64	OffsetDiff = 0;
	char Name[ MAX_PATH ];
	bool LiE = true;

	// Zero out any previous entries.
	FreeZipDirEntryRecords();
	FRealFileSize = 0;
	FZipSOC	= 0;
	FSFXOffset = 0;  //must be before the following "if"
  FZipComment = "";
	FIsSpanned = false;
	FDirOnlyCount = 0;
  FErrCode = 0;  // reset previous error code
  char MadeOS = 0;   	// 1.73.3.2
	char MadeVer = 20;   // 1.73.3.2

  if(!Active)
  {
    FDelaying = FDelaying << zdiList;
    return FErrCode;
  }
  FInFileName = FZipFileName;
#ifndef NO_SPAN
  // Locate last of multi volume or last disk of spanned
  int Result = GetLastVolume(FZipFileName,EOC,true);
  if(Result < 0) return FErrCode; // error exception should been thrown when detected;
  if(Result == 1) // Don't complain - this may intentionally be a new zip file.
#else
	if(!FileExists(FZipFileName))
#endif
  {
    if(OnDirUpdate) OnDirUpdate(Builder);
    return FErrCode;
  }
#ifdef NO_SPAN
  try
  {
    OpenEOC(EOC, true);  // exception if not
  }
  catch (EZipBuilder &ezl)
  {
  	ShowExceptionError(ezl);
  }
#endif
  if(FInFileHandle == -1)               // was problem
  {
    if(OnDirUpdate) OnDirUpdate(Builder);
    return FErrCode;
  }
	try
	{
  	StartWaitCursor();
  	try
  	{
  		FTotalDisks = EOC.ThisDiskNo;	// Needed if GetNewDisk() is called.
  		// This could also be set to True if it's the first and only disk.
  		if(EOC.ThisDiskNo > 0) FIsSpanned = true;
  		// Do we have to request for a previous disk first?
  		if(EOC.ThisDiskNo != EOC.CentralDiskNo)
  		{
#ifndef NO_SPAN
  			GetNewDisk(EOC.CentralDiskNo);
  			FFileSize = FileSeek(FInFileHandle, 0, 2);	//v1.52j
  			OffsetDiff = EOC.CentralOffset;	//v1.52j
#else
        throw EZipBuilder(DS_NoDiskSpan, true);
#endif
  		}
  		else	//v1.52j
  			// Due to the fact that v1.3, v1.4 and v1.45 programs do not change the archives
  			// EOC and CEH records in case of a SFX conversion (and back) we have to
  			// make this extra check.
  			OffsetDiff = FFileSize - EOC.CentralSize - sizeof(EOC) - EOC.ZipCommentLen;

  		FZipSOC  = static_cast<unsigned long >(OffsetDiff);
  		FSFXOffset = FFileSize;
  		if(FFileSize == 22) FSFXOffset = 0; //v1.52L
      FWrongZipStruct = false;
      if(EOC.CentralOffset != OffsetDiff)
      {
        FWrongZipStruct = true;            // We need this in the ConvertXxx functions.
        ShowZipMessage(LI_WrongZipStruct);
      }
        // Now we can go to the start of the Central directory.
      if(FileSeek(FInFileHandle, OffsetDiff, 0) == -1)
        throw EZipBuilder(LI_ReadZipError, true);

	    Report(zacXItem, zprLoading, ZipLoadStr(PR_Loading), EOC.TotalEntries);
  		// Read every entry: The central header and save the information.
#ifdef _DEBUG
  		if(FTrace) Report(zacMessage, 0, Format("List - expecting %d files",
  		                                       ARRAYOFCONST((EOC.TotalEntries))),0);
#endif
  		FZipContents->Capacity = EOC.TotalEntries;
  		for(int i = 0; i < EOC.TotalEntries; i++)
      {
  			// Read a central header.
  			while(FileRead(FInFileHandle, &CEH, sizeof(CEH)) != sizeof(CEH))
        {	//v1.52j
  				// It's possible that we have the central header split up.
  				if(FDiskNr >= EOC.ThisDiskNo)	throw EZipBuilder(DS_CEHBadRead);
  				// We need the next disk with central header info.
#ifndef NO_SPAN
  				GetNewDisk(FDiskNr + 1);
#else
          throw EZipBuilder(DS_NoDiskSpan, true);
#endif
  			}
  			if(CEH.HeaderSig != CentralFileHeaderSig) throw EZipBuilder(DS_CEHWrongSig);

  			// Now the filename.
        ReadJoin( Name, CEH.FileNameLen,DS_CENameLen);
        // Save version info globally for use by codepage translation routine
  			FHostNum = CEH.VersionMadeBy1;
  			FHostVer = CEH.VersionMadeBy0;
        if(FHostNum > 0)	// not msdos
        {
					MadeOS = FHostNum;
					MadeVer = FHostVer;
        }
  			Name[CEH.FileNameLen] = '\0';
#ifdef _DEBUG
        if(FTrace) Report(zacMessage, 0, Format("List - [%d] \"%s\"",
                                         ARRAYOFCONST((i,Name))),0);
#endif
  			// Create a new ZipDirEntry record
  			pzd = new ZipDirEntry;	// These will be deleted in: FreeZipDirEntryRecords().
  			// Copy the information from the central header.
  			memcpy(pzd, &CEH.VersionMadeBy0, LocalDirEntrySize );
       	pzd->FileName  = SetSlash(ConvertOEM(Name, cpdOEM2ISO), psdExternal);
  			pzd->Encrypted = pzd->Flag & 0x01;
  			// Read the extra data if present new v1.6
        pzd->ExtraData = "";  // empty
        if(CEH.ExtraLen)
  			{
          pzd->ExtraData.SetLength(CEH.ExtraLen);
          ReadJoin(pzd->ExtraData.c_str(), CEH.ExtraLen ,LI_ReadZipError);
  			}
  			// Read the FileComment if present and save.
         if(CEH.FileComLen)
        {
          pzd->FileComment.SetLength(CEH.FileComLen);
          ReadJoin(pzd->FileComment.c_str(), CEH.FileComLen ,DS_CECommentLen);
  			}
  			if(FUseDirOnlyEntries || ExtractFileName( pzd->FileName ) != "" )
  			{
#ifdef _DEBUG
          if(FTrace) Report(zacMessage, 0, Format("List - adding \"%s\" [%s]",
                                     ARRAYOFCONST((pzd->FileName,pzd->FileComment))),0);
#endif
  				// Add it to our contents tabel.
  				FZipContents->Add( pzd );
  				// Notify user of the next entry in the ZipDir.
  				if(OnNewName) OnNewName(Builder, i + 1, pzd);
  			}
  			else
  			{
  				FDirOnlyCount++;
#ifdef _DEBUG
           if(FTrace) Report(zacMessage, 0, Format("List - dropped dir [%d]",
                                   ARRAYOFCONST((FDirOnlyCount))),0);
#endif
  				pzd->ExtraData = "";
          pzd->FileName = "";
          pzd->FileComment = "";
  				delete pzd;
  			}
  			// Calculate the earliest Local Header start.
  			if(FSFXOffset > CEH.RelOffLocal) FSFXOffset = CEH.RelOffLocal;
  			Report(zacXProgress, zprLoading, ZipLoadStr(PR_Loading), 1);
        if(Cancel)  throw EZipBuilder( DS_Canceled, true);
  		}
  		FTotalDisks = EOC.ThisDiskNo;			// We need this when we are going to extract.
      LiE = false;
    }
  	catch (EZipBuilder &ezl)
  	{		// Catch all Zip List specific errors.
  		ShowExceptionError(ezl);
  	}
  	catch (const MEMEXCEPT &me)
  	{
  		ShowZipMessage(GE_NoMem);
     }
  	catch(const Exception &E)
  	{
  		ShowZipMessage(LI_ErrorUnknown, "\n" + E.Message);
  	}
  	catch ( ... )
  	{
  		ShowZipMessage(LI_ErrorUnknown);
  	}
  }
  __finally
  {
    Report(zacEndOfBatch, zprLoading, "", 0);
  	StopWaitCursor();
    CloseFileHandle(FInFileHandle);
		if(LiE)
		{
			FZipFileName = "";
			FSFXOffset = 0;
		}
  	else
    {
      FSFXOffset += static_cast<int>(OffsetDiff - EOC.CentralOffset);	// Correct the offset for v1.3, 1.4 and 1.45
      FHostNum = MadeOS;  // if any not dos assume comment not oem
      FHostVer = MadeVer;
      FZipComment = ConvertOEM(FEOCComment, cpdOEM2ISO);
    }
    // Let user's program know we just refreshed the zip dir contents.
  	if(OnDirUpdate) OnDirUpdate(Builder);
  }
  return FErrCode;
}
// TZipWorker::List

/*? TZipWorker::CopyZippedFiles-------------------------------------------------
1.76 9 June 2004 RA properly assign not done files to FSpecArgs
1.75 18 February 2004 allow >2G
1.74 14 January 2004 RP allow create destination file
1.73 1 Oct 2003 RA corrected slashes in central dir
1.73 8 August 2003 RA close InFileHandle after error, set to -1 in non-error case
1.73 (31 July 2003) RA close InFileHandle for Spanerror
1.73 (24 July 2003) RA init OutFileHandle
1.73 (12 July 2003) RP string extra data
1.73 (2 June 2003) RA Use of ConvertOEM
Function to copy one or more zipped files from the zip archive to another zip archive
FSpecArgs in source is used to hold the filename(s) to be copied.
When this function is ready FSpecArgs contains the file(s) that where not copied.
Return values:
 0            All Ok.
 -6           CopyZippedFiles Busy
 -7           CopyZippedFiles errors. See ZipMsgXX.rc
 -8           Memory allocation error.
 -9           General unknown CopyZippedFiles error.
*/
int __fastcall TZipWorker::CopyZippedFiles(TZipWorker *DesTZipWorker, bool DeleteFromSource,
                                            OvrOpts OverwriteDest)
{
	ZipEndOfCentral  EOC;
	ZipCentralHeader CEH;
	ZipDirEntry *zde, *pzd;
	String OutFilePath;
	char* Buffer = NULL;
	int	 Result = 0;
	int	 In2FileHandle = -1; //to avoid external exception in case of error RAEL
	TStringList* NotCopiedFiles = NULL;
	bool Found;
	int	 DestMemCount;
  TMZipDataList* MDZD = NULL;
	StartWaitCursor();
  FZipBusy = true;
	FShowProgress = zspNone;
  FOutFileHandle = -1;
	try
	{
		// Are source and destination different?
		if(DesTZipWorker == this || !AnsiStrIComp(ZipFileName.c_str(),
		                                           DesTZipWorker->ZipFileName.c_str()))
			throw EZipBuilder(CF_SourceIsDest);
    //testing for diskspan
    // The following function a.o. opens the input file no. 1.
    OpenEOC(EOC, false);
    if(DesTZipWorker->IsSpanned || FIsSpanned)
    {
      CloseFileHandle(FInFileHandle);
      throw EZipBuilder(CF_NoCopyOnSpan, true);
    }
		Buffer = new char[FBufSize];

		// Now check for every source file if it is in the destination archive and determine what to do.
		// we use the three most significant bits from the Flag field from ZipDirEntry to specify the action
		// None           = 000xxxxx, Destination no change. Action: Copy old Dest to New Dest
		// Add            = 001xxxxx (New).                  Action: Copy Source to New Dest
		// Overwrite      = 010xxxxx (OvrAlways)             Action: Copy Source to New Dest
		// AskToOverwrite = 011xxxxx (OvrConfirm)	Action to perform: Overwrite or NeverOverwrite
		// NeverOverwrite = 100xxxxx (OvrNever)				  Action: Copy old Dest to New Dest
		for(int s = 0; s < FSpecArgs->Count; s++)
		{
      AnsiString FSpec = FSpecArgs->Strings[s];
      RemoveDriveSpec(FSpec); //allow drive name in FSpecArg
			Found = false;
			for(int d = 0; d < DesTZipWorker->Count; d++)
			{
				zde = (ZipDirEntry *)DesTZipWorker->ZipContents->Items[d];
				if(!AnsiStrIComp(FSpec.c_str(), zde->FileName.c_str()))
				{
					Found = true;
					zde->Flag &= 0x1FFF;	// Clear the three upper bits.
					zde->Flag |= (OverwriteDest == OvrAlways) ? 0x4000ui16 : (OverwriteDest == OvrNever) ? 0x8000ui16 : 0x6000ui16;
					break;
				}
			}
			if(!Found)
			{	// Add the Filename to the list and set flag
				zde = new ZipDirEntry;
				DesTZipWorker->FZipContents->Add(zde);
				zde->FileName       = FSpec;
				zde->FileName.Unique();
				zde->FileNameLength = (Word)FSpecArgs->Strings[s].Length();
				zde->Flag |= 0x2000;		// (a new entry)
				zde->ExtraData = NULL;	//Needed when deleting zde
			}
		}
		// Make a temporary filename like: C:\...\zipxxxx.zip for the new destination
		if((OutFilePath = MakeTempFileName()) == "")	throw EZipBuilder(DS_NoTempFile);

		// Create the output file.
		FOutFileHandle = FileCreate(OutFilePath);
		if(FOutFileHandle == -1)	throw EZipBuilder(DS_NoOutFile);

		// Open the second input archive, i.e. the original destination.
		// 1.74 RP allow file to not exist
		if(((In2FileHandle = FileOpen(DesTZipWorker->ZipFileName, fmShareDenyWrite | fmOpenRead )) == -1)
    	   && !FileExists(DesTZipWorker->ZipFileName))
			     if((In2FileHandle = FileCreate(DesTZipWorker->ZipFileName)) == -1)
			          throw EZipBuilder(CF_DestFileNoOpen);

		// Get the date-time stamp and save for later.
		FDateStamp = FileGetDate(In2FileHandle);

		// Write the SFX header if present.
		if(CopyBuffer(In2FileHandle, FOutFileHandle, DesTZipWorker->SFXOffset))
			throw EZipBuilder(CF_SFXCopyError);

		NotCopiedFiles = new TStringList();
		// Now walk trough the destination, copying and replacing
		DestMemCount = DesTZipWorker->FZipContents->Count;
		MDZD = new TMZipDataList(DestMemCount);  // create class

		// Copy the local data and save central header info for later use.
		for(int d = 0; d < DestMemCount; d++)
		{
			zde = (ZipDirEntry *)DesTZipWorker->ZipContents->Items[d];
			if((zde->Flag & 0xE000) == 0x6000)
			{	// Ask first if we may overwrite.
				bool Overwrite = false;
				// Do we have a event assigned for this then don't ask.
				if(OnCopyZipOverwrite) OnCopyZipOverwrite(DesTZipWorker, zde->FileName, Overwrite);
				else
			   Overwrite = (ZipMessageDlg("",
                       Format(ZipLoadStr(CF_OverwriteYN),
                              ARRAYOFCONST((zde->FileName, DesTZipWorker->ZipFileName))),
                        zmtConfirmation + DHC_CpyZipOvr,
                        TMsgDlgButtons() << mbYes << mbNo) == IDYES) ? true : false;
        zde->Flag &= 0x1FFF;	// Clear the three upper bits.
				zde->Flag |= (Overwrite) ? 0x4000ui16 : 0x8000ui16;
			}
			// Change info for later while writing the central dir in new Dest.
			MDZD->Items[d]->RelOffLocal = FileSeek(FOutFileHandle, 0, 1);

			if((zde->Flag & 0x6000) == 0x0000)
			{	// Copy from original dest to new dest.
				// Set the file pointer to the start of the local header.
				FileSeek(In2FileHandle,static_cast<__int64>(zde->RelOffLocalHdr), 0);
				if(CopyBuffer(In2FileHandle, FOutFileHandle, sizeof(ZipLocalHeader) +
				                                                    zde->FileNameLength +
				                                                    zde->ExtraFieldLength +
				                                                    zde->CompressedSize))
					throw EZipBuilder(CF_CopyFailed, DesTZipWorker->ZipFileName, DesTZipWorker->ZipFileName);
				if(zde->Flag & 0x8000)
				{
					NotCopiedFiles->Add(zde->FileName);
					// Delete also from FSpecArgs, should not be deleted from source later.
					FSpecArgs->Delete(FSpecArgs->IndexOf(zde->FileName));
				}
			}
			else
			{	// Copy from source to new dest.
				// Find the filename in the source archive and position the file pointer.
				for(int s = 0; s < Count; s++)
				{
					pzd = (ZipDirEntry *)ZipContents->Items[s];
					if(!AnsiStrIComp(pzd->FileName.c_str(), zde->FileName.c_str()))
					{
						FileSeek(FInFileHandle, static_cast<__int64>(pzd->RelOffLocalHdr), 0);
						if(CopyBuffer(FInFileHandle, FOutFileHandle, sizeof(ZipLocalHeader) +
						                                                    pzd->FileNameLength +
						                                                    pzd->ExtraFieldLength +
						                                                    pzd->CompressedSize))
							throw EZipBuilder(CF_CopyFailed, ZipFileName, DesTZipWorker->ZipFileName);
						break;
					}
				}
			}
			// Save the file name info in the MDZD structure.
			MDZD->Items[d]->FileNameLen = zde->FileNameLength;
      MDZD->Items[d]->FileName    = new char[zde->FileNameLength + 1];
			StrCopy(MDZD->Items[d]->FileName, zde->FileName.c_str());
		}	// Now we have written al entries.

		// Now write the central directory with possibly changed offsets.
		// Remember the EOC we are going to use is from the wrong input file!
		EOC.CentralSize = 0;
		for(int d = 0; d < DestMemCount; d++)
		{
			zde = (ZipDirEntry *)DesTZipWorker->ZipContents->Items[d];
			Found = false;
			// Rebuild the CEH structure.
			if((zde->Flag & 0x6000) == 0x0000)
			{	// Copy from original dest to new dest.
				pzd = (ZipDirEntry *)DesTZipWorker->ZipContents->Items[d];
				Found = true;
			}
			else
			{	// Copy from source to new dest.
				// Find the filename in the source archive and position the file pointer.
				for(int s = 0; s < Count; s++)
				{
					pzd = (ZipDirEntry *)ZipContents->Items[s];
					if(!AnsiStrIComp(pzd->FileName.c_str(), zde->FileName.c_str()))
					{
						Found = true;
						break;
					}
				}
			}
			if(!Found)
      	throw EZipBuilder(CF_SourceNotFound, zde->FileName, ZipFileName);
			memcpy(&CEH.VersionMadeBy0, pzd, sizeof(ZipCentralHeader )- 4);
			CEH.HeaderSig = CentralFileHeaderSig;
			CEH.Flag &= 0x1FFF;
			CEH.RelOffLocal = static_cast<unsigned long>(MDZD->Items[d]->RelOffLocal);
			// Save the first Central directory offset for use in EOC record.
			if(!d) EOC.CentralOffset = FileSeek(FOutFileHandle, 0, 1);
			EOC.CentralSize += (sizeof(CEH) + CEH.FileNameLen + CEH.ExtraLen + CEH.FileComLen);

			// Write this changed central header to disk
			WriteJoin(&CEH, sizeof(CEH), DS_CEHBadWrite);
      //if filename was converted OEM2ISO then we have to reconvert before copying
      FHostNum = CEH.VersionMadeBy1;
      FHostVer = CEH.VersionMadeBy0;
      StrCopy(MDZD->Items[d]->FileName,SetSlash(ConvertOEM(MDZD->Items[d]->FileName, cpdISO2OEM), psdInternal).c_str());
			// Write to destination the central filename.
			WriteJoin(MDZD->Items[d]->FileName, CEH.FileNameLen, DS_CEHBadWrite);

			// And the extra field from zde or pzd.
			if(CEH.ExtraLen) WriteJoin(pzd->ExtraData.c_str(), CEH.ExtraLen, DS_CEExtraLen);

			// And the file comment.
      if(CEH.FileComLen) WriteJoin(pzd->FileComment.c_str(), CEH.FileComLen, DS_CECommentLen);
		}
		EOC.TotalEntries = EOC.CentralEntries = (unsigned short)DestMemCount;
		EOC.ZipCommentLen = (unsigned short)DesTZipWorker->ZipComment.Length();

		// Write the changed EndOfCentral directory record.
		WriteJoin(&EOC, sizeof(EOC), DS_EOCBadWrite);

		// And finally the archive comment
		FileSeek(In2FileHandle, static_cast<__int64>(DesTZipWorker->ZipEOC + sizeof(EOC)), 0);
		if(CopyBuffer(In2FileHandle, FOutFileHandle, DesTZipWorker->ZipComment.Length()))
			throw EZipBuilder(DS_EOArchComLen);
    CloseFileHandle(FInFileHandle);
		// Now delete all copied files from the source when deletion is wanted.
		if(DeleteFromSource && FSpecArgs->Count > 0)
    {
      FZipBusy = false;
      Delete();	// Delete files specified in FSpecArgs and update the contents.
      FInFileHandle = -1; // closed by Delete and List
    }
		FSpecArgs->Assign(NotCopiedFiles);	// Info for the caller.
	}
	catch(EZipBuilder &ers)
	{	// All CopyZippedFiles specific errors..
		ShowExceptionError(ers);
		Result = -7;
	}
	catch(const MEMEXCEPT &me)
	{		// All memory allocation errors.
		ShowZipMessage(GE_NoMem);
		Result = -8;
	}
	catch(const Exception &E)
	{
		ShowZipMessage(DS_ErrorUnknown, "\n" + E.Message);
		Result = -9;
	}
	catch( ... )
	{								// The remaining errors, should not occur.
		ShowZipMessage(DS_ErrorUnknown);
		Result = -9;
	}
	if(MDZD) delete MDZD;
	delete NotCopiedFiles;
	delete[] Buffer;
  CloseFileHandle(FInFileHandle);
  CloseFileHandle(In2FileHandle);
  if(FOutFileHandle != -1)
  {
		FileSetDate(FOutFileHandle, FDateStamp);
		FileClose(FOutFileHandle);
		if(Result) DeleteFile(OutFilePath);		// An error somewhere, OutFile is not reliable.
		else
		{
			EraseFile(DesTZipWorker->FZipFileName, DesTZipWorker->HowToDelete);
			if(!RenameFile(OutFilePath, DesTZipWorker->FZipFileName))
				EraseFile(OutFilePath, DesTZipWorker->HowToDelete);
		}
	}
	DesTZipWorker->List();	// Update the old(possibly some entries were added temporarily) or new destination.
	StopWaitCursor();
  FZipBusy = false;
	return Result;
}
// TZipWorker::CopyZippedFiles

/*? TZipWorker::Starting---------------------------------------------------------
1.77 17 August 2004 RA property BufSize in kBytes
1.76 31 May 2004 New function
*/
void __fastcall TZipWorker::Starting(void)
{
  inherited::Starting();
}
//? TZipWorker::Starting

/*? TZipWorker::Done---------------------------------------------------------
1.76 31 May 2004 New function
*/
void __fastcall TZipWorker::Done(void)
{
  inherited::Done();
}
//? TZipWorker:Done

/*? TZipWorker::GetAddPassword --------------------------------------------------
1.76 25 May 2004 changed
1.76 10 May 2004 change loading strings
*/
String __fastcall TZipWorker::GetAddPassword(TPasswordButton& Response)
{
	AnsiString p1, p2 = "";

  if(FUnattended) ShowZipMessage(PW_UnatAddPWMiss);
	else
	{
		if((Response = GetPassword(ZipLoadStr(PW_Caption), ZipLoadStr(PW_MessageEnter),
		                         DHC_AddPwrd1, TMsgDlgButtons() << mbOK << mbCancel, p1))
                             == mbOK && p1 != "")
		{
			if((Response = GetPassword(ZipLoadStr(PW_Caption), ZipLoadStr(PW_MessageConfirm),
			                           DHC_AddPwrd2, TMsgDlgButtons() << mbOK << mbCancel, p2))
                                 == mbOK && p2 != "")
			{
				if(AnsiCompareStr(p1,p2))
				{
					ShowZipMessage(GE_WrongPassword );
          p2 = "";
				}
			}
		}
	}
	return p2;
}

/*? TZipWorker::GetExtrPassword -------------------------------------------------
1.76 25 May 2004 changed
1.76 10 May 2004 change loading strings
  Same as GetAddPassword, but does NOT verify
*/
String __fastcall TZipWorker::GetExtrPassword(TPasswordButton& Response)
{
	AnsiString Result = "";

	if(FUnattended ) ShowZipMessage(PW_UnatExtPWMiss);
	else
		Response = GetPassword(ZipLoadStr(PW_Caption), ZipLoadStr(PW_MessageEnter),
		                DHC_ExtrPwrd, TMsgDlgButtons() << mbOK << mbCancel << mbAll, Result);
	return Result;
}

/*? TZipWorker::GetPassword---------------------------------------------------
1.76 25 May 2004 no external GlobalResult
*/
TPasswordButton __fastcall TZipWorker::GetPassword(String DialogCaption,
                                                    String MsgTxt,
                                                    int ctx,
                                                    TPasswordButtons pwb,
                                                    String &ResultStr )
{
  TModalResult GModalResult;
  AnsiString msg = MsgTxt;
  ResultStr = "";
  GModalResult = ZipMessageDialog(DialogCaption, msg, zmtPassword + (ctx & 0xFFFF), pwb);
  switch(GModalResult)
  {
    case mrOk:
   		ResultStr = msg;
   		return mbOK;
   	case mrCancel: return mbCancel;
   	case mrAll: return mbNoToAll; // CancelAll
   	default: return mbAbort;
  }
}
//? TZipWorker::GetPassword

/*? TZipWorker::GetPassword----------------------------------------------------
1.76 25 May 2004 no external GlobalResult
*/
TPasswordButton __fastcall TZipWorker::GetPassword(String DialogCaption,
                                                    String MsgTxt,
                                                    TPasswordButtons pwb,
                                                    String &ResultStr )
{
  return GetPassword(DialogCaption, MsgTxt, DHC_Password, pwb, ResultStr);
}
//? TZipWorker.GetPassword

/*? TZipWorker::DllCallBack---------------------------------------------------------
1.77 4 July 2004 changed callbackStruct + bytes written
1.76 25 April 2004 changes to GetXXXPasswordd
1.76 24 April 2004
*/
void __fastcall TZipWorker::DllCallBack(CallBack::PZCallBackStruct cbr)
{
  if(fIsDestructing) return;
  AnsiString CMsg = "";
  unsigned XIdent = cbr->ActionCode;
  ActionCodes ActionCode = static_cast<ActionCodes>(XIdent & 63);
  XIdent &= 0xFFFFFF00;
  int XPreamble = 0;
  if(XIdent)
  {
    XIdent = XIdent >> 8;
    XPreamble = XIdent & 255; // ofset of filename
    XIdent = XIdent >> 8 ;    // ident of message
  }

  int ErrorCode = cbr->ErrorCode;
  __int64 FileSize = cbr->FileSize;
  try
  {
    if((ActionCode <= zacSize) || (ActionCode == zacXItem) || (ActionCode == zacXProgress))
    {
      unsigned wrote = 0;
      AnsiString m = "";
      if(ActionCode != zacTick) m = ExtMsg(CMsg, cbr, XIdent, XPreamble);
      switch(ActionCode)
      {
        case zacSize:
          FileSize += static_cast<__int64>(ErrorCode) << 32;
          ErrorCode = 0;
        break;
        case zacItem:
        case zacProgress:
        case zacEndOfBatch:
          wrote = reinterpret_cast<unsigned>(cbr->Data);
        break;
      }
      Report(ActionCode, ErrorCode, m, FileSize, wrote);
      return;
    }
    switch(ActionCode)
    {
      case zacNewName:
      {
        if(OnSetNewName)
        {
          String OldFileName = ExtMsg(CMsg, cbr, XIdent, XPreamble);
          bool IsChanged = false;
          OnSetNewName(Builder, OldFileName, IsChanged);
          if(IsChanged) StrPLCopy(cbr->FileNameOrMsg, OldFileName, 512);
          cbr->ErrorCode = IsChanged;
        }
        if(!FileSize && OnSetAddName)
        {
          AnsiString OldFileName = ExtMsg(CMsg, cbr, XIdent, XPreamble);
          AnsiString OrigName = SetSlash(reinterpret_cast<char*>(cbr->Data),psdExternal);
          bool IsChanged = false;
          OnSetAddName(Builder, OldFileName, OrigName, IsChanged);
          if(IsChanged) StrPLCopy(cbr->FileNameOrMsg, SetSlash(OldFileName, psdInternal), 512);
          cbr->ErrorCode = IsChanged;
        }
      }
      break;
      case zacPassword:
      {	// New or other password needed during Extract() or Add().
        String  pwd = "";
        unsigned RptCount = static_cast<unsigned>(FileSize);
        TPasswordButton Response = mbOK;
        if(OnPasswordError)
        {
          OnPasswordError(Builder, cbr->IsOperationZip, pwd,
                                    ExtMsg(CMsg, cbr, XIdent, XPreamble), RptCount, Response);
          if(Response != mbOK ) pwd = "";
        }
        else pwd = (ErrorCode & 0x1) ? GetAddPassword(Response) : GetExtrPassword(Response);
        if(pwd != "")
        {
          StrPLCopy(cbr->FileNameOrMsg, pwd, PWLEN + 1);
          cbr->ErrorCode = 1;
        }
        else RptCount = cbr->ErrorCode = 0;
        cbr->FileSize = min( RptCount, 15U );
        if(Response == mbNoToAll )	// Cancel all
          cbr->ActionCode = 0;
        if(Response == mbAbort ) Cancel = true;  // Abort
      }
      break;
      case zacCRCError:
      {	// CRC32 error, (default action is extract/test the file).
          bool DoExtract = true;	// This was default for versions <1.6
          if(OnCRC32Error ) OnCRC32Error(Builder,
                                      ExtMsg(CMsg, cbr, XIdent, XPreamble),
                                      ErrorCode, static_cast<unsigned>(FileSize),
                                      DoExtract );
          cbr->ErrorCode = DoExtract;
          // This will let the Dll know it should send some warnings.
          if(!OnCRC32Error) cbr->ErrorCode = 2;
      }
      break;
      case zacOverwrite:	// Extract(UnZip) Overwrite ask.
        if(OnExtractOverwrite)
        {
          bool DoOverwrite = FileSize;
          OnExtractOverwrite(Builder, ExtMsg(CMsg, cbr, XIdent, XPreamble),
                                      ErrorCode & 0x10000, DoOverwrite, ErrorCode & 0xFFFF );
          cbr->FileSize = DoOverwrite;
        }
      break;
      case zacSkipped:	// Extract(UnZip) and Skipped.
        if(ErrorCode)
        {
          FErrCode = (int)(char)(ErrorCode & 0xFF);
          FFullErrCode = ErrorCode;
        }
        if(OnExtractSkipped) OnExtractSkipped(Builder, ExtMsg(CMsg, cbr, XIdent, XPreamble),
                                    (UnZipSkipTypes)((cbr->FileSize & 0xFF)- 1), cbr->ErrorCode );
        else
          if(OnMessage) OnMessage(Builder, GE_Unknown, "Skipped " +
                                   ExtMsg(CMsg, cbr, XIdent, XPreamble) + " " +
                                   IntToStr(static_cast<int>((cbr->FileSize & 0xFF) -1)));
      break;
      case zacComment:	// Add(Zip) FileComments. v1.60L
        if(OnFileComment)
        {
          String FileComment = cbr->FileComment;
          bool IsChanged = false;
          OnFileComment(Builder, ExtMsg(CMsg, cbr, XIdent, XPreamble),
                                 FileComment, IsChanged );
          if(IsChanged && FileComment != "")
            StrPLCopy( cbr->FileNameOrMsg, FileComment, 511);
          else
             cbr->FileNameOrMsg[0] = 0;
          cbr->ErrorCode = IsChanged;
          cbr->FileSize  = min(FileComment.Length(), 511);
        }
      break;
  #ifndef NO_STREAM
      case zacStream:	// Stream2Stream extract. v1.6M
        try
        {
          FZipStream->SetSize( STRMSIZ(FileSize)); //Changed R. Peters for BCB6
        }
        catch ( ... )
        {
          cbr->ErrorCode = 1;
          cbr->FileSize = NULL;
          break;
        }
        cbr->FileSize = (long)FZipStream->Memory;
      break;
  #endif
      case zacData:  // Set Extra data changed  1.73
      if(OnFileExtra)
      {
        FStoredExtraData.SetLength(cbr->FileSize);
        if(cbr->FileSize) memcpy(FStoredExtraData.c_str(),cbr->Data,cbr->FileSize);
        bool IsChanged = false;
        OnFileExtra(Builder, ExtMsg(CMsg, cbr, XIdent, XPreamble),
                             FStoredExtraData, IsChanged);
        cbr->ErrorCode = (int)IsChanged;
        if(IsChanged)
        {
          unsigned xlen = FStoredExtraData.Length();
          if(xlen && xlen < 512)
              memcpy(cbr->FileNameOrMsg,FStoredExtraData.c_str(),xlen);
          else cbr->Data = reinterpret_cast<unsigned*>(FStoredExtraData.c_str());
          cbr->FileSize = xlen;
          cbr->ErrorCode = -1;
        }
      }
      break;
      case zacExtName: // request for a new path+name just before zipping or extracting
      {
        if(OnSetExtName)
        {
          AnsiString OldFileName = ExtMsg(CMsg, cbr, XIdent, XPreamble);
          bool IsChanged = false;
          OnSetExtName(Builder, OldFileName, IsChanged);
          AnsiString Msg = ExtMsg(CMsg, cbr, XIdent, XPreamble);
          if(IsChanged && (OldFileName != Msg) && (IsPathOnly(OldFileName) == IsPathOnly(Msg)))
                StrPLCopy( cbr->FileNameOrMsg, OldFileName, 512);
          cbr->ErrorCode = IsChanged;
        }
      }
      break;
    }
    Report(zacNone, 0, "", 0);            // process messages
  }
  catch(const Exception &ezl)   // changed 1.73
  {
    if(FEventErr == "") FEventErr = " #" + IntToStr(ActionCode) + " \"" + ezl.Message+ "\"";
    Cancel = true;
	}
}
//? TZipWorker::DllCallBack

/*? TZipWorker::ExtAdd---------------------------------------------------------
1.77 5 August 2004 RA parameter added in writespan
1.76 26 May 2004 RP use shielded FSpecArgs
1.76 13 May 2004 RP change RootDir support and allow Freshen/Update with no args
1.76 24 April 2004 RP change checking no files
1.76 17 April 2004 RP change exception reporting
1.75 16 March 2004 RA only forceDir on hard drive
1.73 7 September 2003 RA allow empty FSpecArgs for Update and Freshen
1.73 6 September 2003 RA throw exception if dll returns negative value
1.73 4 August 2003 RA bug fix for ' ' as password + Add Dir test moved inside try catch
1.73 2 August 2003 RA removed loading of dll will be done in Exec
1.73 (17 July 2003) RP reject '< ' as password ' '
1.73 (16 July 2003) RA load Dll in try except
1.73 (16 July 2003) RP trim filenames
1.73 (15 July 2003) RA remove '<' from filename
1.73 (14 June 2003) RP changed dll handling
UseStream = 0 ==> Add file to zip archive file.
UseStream = 1 ==> Add stream to zip archive file.
UseStream = 2 ==> Add stream to another (zipped) stream.
*/
#ifndef NO_STREAM
void __fastcall TZipWorker::ExtAdd(int UseStream, DWORD StrFileDate, DWORD StrFileAttr, TMemoryStream *MemStream)
#else
void __fastcall TZipWorker::ExtAdd(int UseStream, DWORD /*StrFileDate*/, DWORD /*StrFileAttr*/, TMemoryStream* /*MemStream*/)
#endif
{
	String TmpZipName;
	FSuccessCnt = FErrCode = 0;
	FMessage = "";
  if(!UseStream && !FFSpecArgs->Count)
  {
    if(!(FAddOptions.Contains(AddFreshen) || FAddOptions.Contains(AddUpdate)))
    {
      ShowZipMessage(AD_NothingToZip);
      return;
    }
    FAddOptions = FAddOptions >> AddUpdate;
    FAddOptions = FAddOptions << AddFreshen; // convert update to freshen
    FFSpecArgs->Add("*.*");// do freshen all
  }
  if(FAddOptions.Contains(AddDiskSpanErase))
  {
    FAddOptions = FAddOptions << AddDiskSpan; // to make certain
#ifndef NO_SPAN
    FSpanOptions = FSpanOptions << spWipeFiles;
#endif
  }
#ifdef NO_SPAN
	if(FAddOptions.Contains(AddDiskSpan))
	{
		ShowZipMessage(DS_NoDiskSpan);
		return;
	}
#endif
	/* We must allow a zipfile to be specified that doesn't already exist,
	 * so don't check here for existence. */
	if(UseStream < 2 && FZipFileName == "")  // Make sure we have a zip filename.
	{
		ShowZipMessage(GE_NoZipSpecified);
		return;
	}
	// We can not do an Unattended Add if we don't have a password.
	if(FUnattended && FAddOptions.Contains(AddEncrypt) && FPassword == "")
	{
		ShowZipMessage(AD_UnattPassword);
		return;
	}
	// If we are using disk spanning, first create a temporary file to do the Add.
	if(UseStream < 2 && FAddOptions.Contains(AddDiskSpan))
	{
#ifdef NO_SPAN
    ShowZipMessage(DS_NoDiskSpan);
    return;
#else
		// We can't do this type of Add() on a spanned archive.
		if(FAddOptions.Contains(AddFreshen) || FAddOptions.Contains(AddUpdate))
		{
			ShowZipMessage(AD_NoFreshenUpdate);
			return;
		}
      // We can't Add and copy a SFX archive as spanned.
		if(UpperCase(ExtractFileExt(FZipFileName)) == ".EXE")
		{
			ShowZipMessage(DS_NoSFXSpan);
			return;
		}
		TmpZipName = MakeTempFileName( "", "" );
    if(FVerbose && OnMessage) OnMessage(Builder, 0,
                            ZipFmtLoadStr(GE_TempZip,ARRAYOFCONST((TmpZipName))));
#endif
	}
	else TmpZipName = FZipFileName; //not spanned create the outfile directly
	// Make sure we can't get back in here while work is going on.
  if(FZipBusy) return;
	if(UseStream < 2 && UpperCase(ExtractFileExt(FZipFileName)) == ".EXE" && !FSFXOffset &&
	             !FileExists(FZipFileName))
	{
#ifdef INTERNAL_SFX
		try
		{
			// This is the first 'Add' operation following creation of a new
			// .EXE archive.  We need to add the SFX code now, before we add
			// the files.
			AutoExeViaAdd = true;
			int SFXResult = ConvertSFX(true);
			AutoExeViaAdd = false;
			if(SFXResult)	throw EZipBuilder(AD_AutoSFXWrong, SFXResult);
		}
		catch (EZipBuilder &ers)
		{
			// All SFX creation errors will be catched and returned in this one message.
			ShowExceptionError(ers);
			return;
		}
#else
		ShowZipMessage(SF_NoSFXSupport);
		return;
#endif
	}
	Cancel = false;
  FZipBusy = true;
  StartWaitCursor();
	try
	{
		try
		{
       // make certain destination can exist
      if((UseStream < 2) && (FDriveFixed || !FAddOptions.Contains(AddDiskSpan)))
      // 1.75 RA
      {
        if(FAddOptions.Contains(AddForceDest))
          ForceDirectory(ExtractFilePath(FZipFileName));
        if(!DirExists(ExtractFilePath(FZipFileName)))
          throw EZipBuilder(AD_NoDestDir, ExtractFilePath(FZipFileName));
      }
			ZipParms = new ZipDLL::ZipParms;
			memset(ZipParms, 0, sizeof(*ZipParms));
			SetZipSwitches(TmpZipName);
#ifndef NO_STREAM
			if(UseStream == 1)
			{
				ZipParms->fUseInStream	 = true;
				ZipParms->fInStream		 = FZipStream->Memory;
				ZipParms->fInStreamSize = FZipStream->Size;
				ZipParms->fStrFileAttr	 = StrFileAttr;
				ZipParms->fStrFileDate	 = StrFileDate;
			}
			if(UseStream == 2)
			{
				ZipParms->fUseOutStream	= true;
				ZipParms->fOutStream		= FZipStream->Memory;
				ZipParms->fOutStreamSize	= MemStream->Size + 6;
				ZipParms->fUseInStream		= true;
				ZipParms->fInStream			= MemStream->Memory;
				ZipParms->fInStreamSize	= MemStream->Size;
			}
#endif
       // 1.76 set global RootDir
      int RootLen;
	    if((RootLen = FRootDir.Length()) != 0) 		// New v1.60L use of RootDir
      {
        ZipParms->fGRootDir = new char[RootLen + 1];
        StrPLCopy(ZipParms->fGRootDir, FRootDir, RootLen + 1);
      }
      int FDSCnt = FFSpecArgs->Count;
      if(!FDSCnt && (FAddOptions.Contains(AddFreshen) || FAddOptions.Contains(AddUpdate)))
          FDSCnt = 1;
      ZipParms->fArgc = FDSCnt;

			ZipParms->fFDS = new FileData[FDSCnt]; //[FFSpecArgs->Count];
      memset(ZipParms->fFDS, 0, FFSpecArgs->Count * sizeof(FileData));
			for(int i = 0; i < FDSCnt; i++)
			{
        AnsiString FDSSpec;
        if(!FFSpecArgs->Count) FDSSpec = "";
        else FDSSpec = FFSpecArgs->Strings[i];
				int len = FDSSpec.Length(), b, p = 1;// RootLen;	// begin password, pointer correction.
				// Added to version 1.60L to support recursion and encryption on a FFileSpec basis.
				// Regardless of what AddRecurseDirs is set to, a '>' will force recursion, and a '|' will stop recursion.
				ZipParms->fFDS[i].fRecurse = ZipParms->fRecurse;				// Set default
				if(FDSSpec[1] == '>')
				{
					ZipParms->fFDS[i].fRecurse = true;
					p++;
				}
				if(FDSSpec[1] == '|')
				{
					ZipParms->fFDS[i].fRecurse = false;
					p++;
				}
				// Further it is possible the specify a password after the FFileSpec, separated by a '<'
				// If there is no other text after the '<' then, an existing password, is temporarily canceled.
				ZipParms->fFDS[i].fEncrypt = ZipParms->fEncrypt;			// Set default
				//changed R. Aelbrecht when default password is used we delete an already deleted object
        if(ZipParms->pZipPassword) // set default password if exists
        {
           ZipParms->fFDS[i].fPassword = new char[StrLen(ZipParms->pZipPassword) + 1];
           StrCopy(ZipParms->fFDS[i].fPassword, ZipParms->pZipPassword);
        }
				if((b = AnsiPos( "<", FDSSpec)) != 0 )
				{	// Found...
					ZipParms->fFDS[i].fEncrypt = true;		// the new default, but...
          delete[] ZipParms->fFDS[i].fPassword; //changed R. Aelbrecht delete default pasword first
					ZipParms->fFDS[i].fPassword = NULL;
          AnsiString tmp = FDSSpec.SubString(b + 1, 1);
          if((tmp == "") || (tmp == " "))
          {
						ZipParms->fFDS[i].fEncrypt = false;	// No password, so cancel for this FFspecArg
            len -= tmp.Length() + 1;
          }
					else
					{
						ZipParms->fFDS[i].fPassword = new char[len - b + 1];
            StrPLCopy( ZipParms->fFDS[i].fPassword, FDSSpec.SubString(b + 1, len - b), len - b + 1);
						len = b - 1;
					}
				}
				// And to set the RootDir, possibly later with override per FSpecArg v1.70
        // 1.76 only set if different to global
        ZipParms->fFDS[i].fRootDir = NULL;
        AnsiString tmp = FFSpecArgs->Strings[i].SubString(p, len - p + 1).Trim();
        if(tmp != "")
        {
          ZipParms->fFDS[i].fFileSpec = new char[tmp.Length() + 1];
          StrPLCopy(ZipParms->fFDS[i].fFileSpec, tmp, tmp.Length());
        }
        else ZipParms->fFDS[i].fFileSpec = NULL;
			}
			ZipParms->fSeven = 7;
      FEventErr = "";                      // added
       FSuccessCnt = fZipDll->Exec(ZipParms, Min_ZipDll_Vers);
			// If Add was successfull and we want spanning copy the temporary file to the destination.
			if(UseStream < 2 && FSuccessCnt && (FAddOptions.Contains(AddDiskSpan) ||
		                                    FAddOptions.Contains(AddDiskSpanErase)))
			{
#ifdef NO_SPAN
				throw EZipBuilder(DS_NoDiskSpan, true);
#else
        if(WriteSpan(TmpZipName, FZipFileName, true)) FSuccessCnt = 0;
				DeleteFile(TmpZipName);
#endif
			}
#ifndef NO_STREAM
			if(UseStream == 2 && FSuccessCnt == 1)
      	   FZipStream->SetSize(STRMSIZ(ZipParms->fOutStreamSize));//Changed R. Peters for BCB6
#endif
		}
    catch (EZipBuilder &ers)
    {
      if(FEventErr != "") ers.Message += FEventErr;
      ShowExceptionError(ers);
    }
    catch(const Exception &E)
    {
      if(!FUnattended) ShowMessage(E.Message);
      if(OnMessage) OnMessage(Builder,9,E.Message);
    }
		catch ( ... )
		{
			ShowZipMessage(GE_FatalZip);
		}
	}
	__finally
	{
		FFSpecArgs->Clear();
		FFSpecArgsExcl->Clear();
		/* Free the memory for the zipfilename and parameters
	 	* we know we had a filename, so we'll dispose it's space. */
    if(ZipParms)
    {
      delete[] ZipParms->pSuffix;
      delete[] ZipParms->pZipFN;
      delete[] ZipParms->pZipPassword;
      delete[] ZipParms->fTempPath;
      delete[] ZipParms->fArchComment;
      delete[] ZipParms->fGRootDir;  //1.76
      for(int i = ZipParms->fArgc - 1; i >= 0; i--)
      {
        delete[] ZipParms->fFDS[i].fFileSpec;
        delete[] ZipParms->fFDS[i].fPassword;	// v1.60L
        delete[] ZipParms->fFDS[i].fRootDir;	// v1.60L
      }
      delete[] ZipParms->fFDS;
      for(int i = ZipParms->fTotExFileSpecs - 1; i >= 0; i--)
        delete[] ZipParms->fExFiles[i].fFileSpec;
      delete[] ZipParms->fExFiles;
      delete ZipParms;
    }
		ZipParms = NULL;
    StopWaitCursor();
    fZipDll->Unload(false);
    FZipBusy = false;
		Cancel	= false;
    FStoredExtraData = "";                   // release held data
		if(FSuccessCnt > 0) List();  // Update the Zip Directory by calling List() function.
	}
}
// TZipWorker::ExtAdd

/*? TZipWorker::ZCallback-------------------------------------------------------
1.76 01 May 2004 RP change return type and value to return flag for exception
1.76 24 April 2004 RP use DllCallback
 1.73 ( 1 June 2003) changed for new callback
 Dennis Passmore (Compuserve: 71640,2464) contributed the idea of passing an
 instance handle to the DLL, and, in turn, getting it back from the callback.
 This lets us reference variables in the TZipWorker class from within the
 callback function.  Way to go Dennis!
 RCV: But while doing it like this you can only access public data and member
 functions and we need access to private data (the properties ErrCode and
 Message are read only).
 This function is now(v1.5) a static member function of TZipWorker. */
int __stdcall TZipWorker::ZCallback( CallBack::PZCallBackStruct cbr )
{
  TZipWorker *tzb = (TZipWorker *)cbr->Caller;
  tzb->DllCallBack(cbr);
  if(tzb->FEventErr != "") return 10106;           // handled exception
  else return tzb->Cancel ? 1 : 0;
}
// TZipWorker::ZCallback

/*? TZipWorker::SetCancel-------------------------------------------------------
1.76 10 May 2004 remove AbortDlls
1.76 28 April 2004 add side effect of aborting dll when setting true
1.73 (14 June 2003) RP did not work with multithread dlls
*/
void __fastcall TZipWorker::SetCancel(bool Value)
{
  if(Cancel != Value)
  {
    inherited::SetCancel(Value);
  }
}
// TZipWorker::SetCancel

/*? TZipWorker::AbortDlls---------------------------------------------------------------------------
1.76 28 April 2004 abort both dlls (only if loaded)
*/
void __fastcall TZipWorker::AbortDlls(void)
{
  if(fZipDll) fZipDll->Abort();
  if(fUnzDll) fUnzDll->Abort();
  Cancel = true;

  }
//? TZipWorker::Abortdlls

/*? TZipWorker::ExtExtract------------------------------------------------------
1.77 5 August 2004 RA parameter added in readspan
1.76 28 April 2004 do not check destination when testing (ExtrTest is active)
1.73 17 September 2003 RA avoid same exception shown twice
1.73 6 September 2003 RA throw exception if dll returns negative value
1.73 4 August 2003 RA removed loading of dll will be done in Exec
1.73 (22 July 2003) RA exception handling for EZipMaster + fUnzBusy := False when dll load error
1.73 (16 July 2003) RA catch and display dll load errors
1.73 (18 June 2003) RA bug fix UnZipParms->fExtractDir when FExtrBaseDir not used
1.73 (14 June 2003) RP new dll handling
1.73 (22 May 2003) RA use of DelimitPah
1.73 (20 May 2003 RA added ForceDirectories and ExtrForceDir option
 UseStream = 0 ==> Extract file from zip archive file.
 UseStream = 1 ==> Extract stream from zip archive file.
 UseStream = 2 ==> Extract (zipped) stream from another stream.*/
#ifndef NO_STREAM
void __fastcall TZipWorker::ExtExtract(int UseStream, TMemoryStream *MemStream)
#else
void __fastcall TZipWorker::ExtExtract(int UseStream, TMemoryStream* /*MemStream*/)
#endif
{
	String TmpZipName;
	int	 OldPRC = FPasswordReqCount;

	FSuccessCnt = FErrCode = 0;
	FMessage = "";

  if(UseStream < 2)
  {
    if(FZipFileName == "")
    {
      ShowZipMessage(GE_NoZipSpecified, "");
      return;
    }
    if(FZipContents->Count == 0) List(); // List was probably aborted try again
    if(FZipContents->Count == 0)
    {
      if(!FErrCode) ShowZipMessage(DS_FileOpen, "");
      return;
    }
  }
   // Make sure we can't get back in here while work is going on
  if(FUnzBusy) return;
	// We have to be carefull doing an unattended Extract when a password is needed
	// for some file in the archive.
	if(FUnattended && FPassword == "" && !OnPasswordError)
	{
		FPasswordReqCount = 0;
		ShowZipMessage(EX_UnAttPassword, "");
	}
	Cancel  = false;
  FUnzBusy = true;

	// We do a check if we need UnSpanning first, this depends on
	// The number of the disk the EOC record was found on. ( provided by List() )
	// If we have a spanned set consisting of only one disk we don't use ReadSpan().
	if(FTotalDisks)
	{
#ifdef NO_SPAN
		FUnzBusy = false;
		ShowZipMessage(DS_NoDiskSpan, "");
		return;
#else
		if(FTempDir == "")
		{
			char NewName[MAX_PATH];
			::GetTempPath(MAX_PATH, NewName);
			TmpZipName = NewName;
		}
		else TmpZipName = DelimitPath(FTempDir, true);
		if(ReadSpan(FZipFileName, TmpZipName, true))
		{
      FUnzBusy = false;
			return;
		}
		// We returned without an error, now  TmpZipName contains a real name.
#endif
	}
	else TmpZipName = FZipFileName;
  try
  {
  	try   // Pass in a ptr to parms.
  	{
      UnZipParms = new UnzipDLL::UnZipParms;
  		memset(UnZipParms, 0, sizeof(*UnZipParms));
      SetUnZipSwitches(TmpZipName);
      UnZipParms->fExtractDir = new char[MAX_PATH + 1];
      GetCurrentDirectory(MAX_PATH, UnZipParms->fExtractDir);
      if(!UseStream && (FExtrBaseDir != "")&& !FExtrOptions.Contains(ExtrTest))
      {
        StrLCopy(UnZipParms->fExtractDir, FExtrBaseDir.c_str(), MAX_PATH);
        SetCurrentDir(FExtrBaseDir);
        if(!(DirExists(FExtrBaseDir) ||
            (FExtrOptions.Contains(ExtrForceDirs) && ForceDirectory(FExtrBaseDir))))
           throw EZipBuilder(EX_NoExtrDir,FExtrBaseDir);
      }
  		UnZipParms->fUFDS = new UnzFileData[FFSpecArgs->Count];
  		memset(UnZipParms->fUFDS, 0, sizeof(UnzFileData) * FFSpecArgs->Count);
       // 1.70 added test - speeds up extract all
      if(FFSpecArgs->Count && (FFSpecArgs->Strings[0]) != "*.*")
      {
        for(int i = 0; i < FFSpecArgs->Count; i++)
        {
          UnZipParms->fUFDS[i].fFileSpec = new char[FFSpecArgs->Strings[i].Length() + 1];
          StrPLCopy(UnZipParms->fUFDS[i].fFileSpec, FFSpecArgs->Strings[i], FFSpecArgs->Strings[i].Length() + 1);
        }
        UnZipParms->fArgc = FFSpecArgs->Count;
      }
      else UnZipParms->fArgc = 0;
#ifndef NO_STREAM
  		if(UseStream == 1) for(int i = 0; i < Count; i++)
  		{	// Find the wanted file in the ZipDirEntry list.
  			ZipDirEntry *zde = (ZipDirEntry *)ZipContents->Items[i];
  			if(!AnsiStrIComp(FFSpecArgs->Strings[0].c_str(), zde->FileName.c_str()))
  			{	// Found?
  				FZipStream->SetSize(STRMSIZ(zde->UncompressedSize));//Changed R. Peters for BCB6
  				UnZipParms->fUseOutStream  = true;
  				UnZipParms->fOutStream     = FZipStream->Memory;
  				UnZipParms->fOutStreamSize = zde->UncompressedSize;
  				UnZipParms->fArgc          = 1;
  				break;
  			}
  		}
  		if(UseStream == 2)
  		{
  			UnZipParms->fUseInStream	  = true;
  			UnZipParms->fInStream		  = MemStream->Memory;
  			UnZipParms->fInStreamSize  = MemStream->Size;
  			UnZipParms->fUseOutStream  = true;
  			UnZipParms->fOutStream     = FZipStream->Memory;
  			UnZipParms->fOutStreamSize = FZipStream->Size;
  		}
#endif
  		UnZipParms->fSeven = 7;
      FEventErr = "";
  	  	// fArgc is now the no. of filespecs we want extracted.
  	  if(!UseStream || (UseStream && UnZipParms->fUseOutStream))
  			  FSuccessCnt = fUnzDll->Exec(UnZipParms, Min_UnzDll_Vers);
    }
     catch (EZipBuilder &ezl)
    {  // Catch all Zip specific erro rs.
      if(FEventErr  != "") ezl.Message += FEventErr;
      ShowExceptionError(ezl);
      FSuccessCnt = 0;
    }
  	catch (...)
  	{
  		ShowZipMessage( EX_FatalUnZip );
      FSuccessCnt = 0;
  	}
  }
  __finally
  {
#ifndef NO_STREAM
  		// Remove from memory if stream is not Ok.
    if(UseStream && FSuccessCnt != 1) FZipStream->Clear();
#endif
  		// If UnSpanned we still have this temporary file hanging around.
    if(FTotalDisks > 0) DeleteFile(TmpZipName);

		FFSpecArgs->Clear();
		// Free all allocated memory.
    if(UnZipParms)
    {
      delete[] UnZipParms->pZipFN;
      delete[] UnZipParms->pZipPassword;
      if(!UseStream) SetCurrentDir(UnZipParms->fExtractDir);
      delete[] UnZipParms->fExtractDir;
      for(int i = UnZipParms->fArgc - 1; i >= 0; i--)	delete[] UnZipParms->fUFDS[i].fFileSpec;
      delete[] UnZipParms->fUFDS;
      delete UnZipParms;
    }
		UnZipParms = NULL;
	}
	if(FUnattended && FPassword == "" && !OnPasswordError) PasswordReqCount = OldPRC;

  fUnzDll->Unload(false);
  FUnzBusy = false;
	Cancel  = false;
	// No need to call the List() function contents unchanged.
}
// TZipWorker::ExtExtract

/*? TZipWorker::SetActive
1.76
*/
void __fastcall TZipWorker::SetActive(bool Value)
{
  if(Active != Value)
  {
    FActive = Value;
    if(FActive)
    {      // do delayed action
      if(FDelaying.Contains(zdiList)) List();
      if(FDelaying.Contains(zdiComment)) SetZipComment(FZipComment);
    }
    FDelaying.Clear();
  }
}
//? TZipWorker::SetActive

/*? TZBulder::SetFileName---------------------------------------------------------------------------
1.76 27 April 2004 List controlled by 'Inactive'
*/
void __fastcall TZipWorker::SetFileName(String Value)
{
	FZipFileName = Value;
	// Automatically build a new TList of contents in "ZipContents".
  List();
}
//? TZipWorker::SetFileName

/*? TZipWorker::TZipWorker-----------------------------------------------------
1.76 27 April 2004 - clear Active and Delaying
1.75.0.2 3 March 2004 - new event
1.73.3.2 10 Oct 2003 RA added setup_extend
1.73( 30 July 2003) RA TInternalSFX class added
1.73 (24 June 2003) RA set default value TargetHostVer
1.73 (17 May 2003) - RP clear event variables
*/
__fastcall TZipWorker::TZipWorker(void) : TZipBase()
{
	FZipContents 		= new TList();
	FFSpecArgs  		= new TStringList();
	FFSpecArgsExcl	= new TStringList();
  FHandle = Application->Handle;
  fZipDll = new TZipDll(this);		// new 1.73
	fUnzDll = new TUnzDll(this);		// new 1.73
	fIsDestructing = false;					// new 1.73
  FDelaying.Clear();
  FActive = false;
  FZipBusy = false;
  FUnzBusy = false;
#ifdef INTERNAL_SFX
  FIntSFX = new TInternalSFX(this); // new 1.73
#endif
#ifndef NO_SFX
  FSFXIcon = new TIcon();
#endif
  Setup_extend(); // init extend table
  FOnCheckTerminate = NULL;
  OnTick      = NULL;
  FOnFileExtra = NULL;
  FOnDirUpdate = NULL;
  OnProgress  = NULL;
  OnTotalProgress = NULL;
  OnItemProgress = NULL;
  OnMessage   = NULL;
  FOnSetNewName = NULL;
  FOnNewName   = NULL;
  FOnPasswordError = NULL;
  FOnCRC32Error = NULL;
  FOnExtractOverwrite = NULL;
  FOnExtractSkipped = NULL;
  FOnCopyZipOverwrite = NULL;
  FOnFileComment = NULL;
  FOnFileExtra = NULL;
  FOnSetAddName = NULL;
  FOnSetExtName = NULL;
#ifndef NO_SPAN
  FOnGetNextDisk = NULL;
  FOnStatusDisk = NULL;
  FConfirmErase = true;
#endif

	ZipParms				= NULL;
	UnZipParms = NULL;
	FZipFileName			= "";
  FPassword = "";
  FPasswordReqCount = 1;
	FSuccessCnt 			=  0;
  FAddCompLevel = 9;
	AutoExeViaAdd = false;
  FUnattended = false;
	FRealFileSize			=  0;
	FSFXOffset				=  0;
	FZipSOC					=  0;
  FFreeOnDisk1 = 0;
  FFreeOnAllDisks = 0;
  FMaxVolumeSize = 0;
  FMinFreeVolSize = 65536;
	FIsSpanned        	= false;
	FZipComment = "";
  FHowToDelete = htdAllowUndo;
  FVersionInfo = ZIPBUILDERVERSION;
  FVer = ZIPBUILDERVER;
  FCodePage = cpAuto;

  FAddStoreSuffixes = AddStoreExts() << assGIF << assPNG << assZ << assZIP << assZOO << assARC
														<< assLZH << assARJ << assTAZ << assTGZ << assLHA
														<< assRAR << assACE << assCAB << assGZ << assGZIP << assJAR;
	FDirOnlyCount			= 0;
  FSpanOptions.Clear();
  FBufSize  = 65536;
#ifndef NO_SPAN
  FConfirmErase = true;
#endif
#ifndef NO_STREAM
  FZipStream				= new TZipStream();
#endif
}
//? TZipWorker::TZipWorker

/*? TZipWorker::SetZipComment---------------------------------------------------
1.78 4 Oct 2004 RA add test fileexists for NO_SPAN
1.76 27 April 2004 test Active
1.75 18 February 2004 allow >2G
1.73.3.2 11 Oct 2003 RP allow preset comment
1.73 (10 June 2003) RA bugfix
changed 1.73 (22 May 2003) RA use of ReadJoin
*/
void __fastcall TZipWorker::SetZipComment(String zComment)
{
	struct ZipEndOfCentral  EOC;
	bool Fatal = false;
	FInFileHandle = -1;
  if(!Active)
  {
    FDelaying = FDelaying  << zdiComment;
    FZipComment = zComment;
    return;
  }
	try
  {
    if(!FZipFileName.Length() ) throw EZipBuilder(GE_NoZipSpecified);
#ifndef NO_SPAN
    // for multi volume find last volume and allow that file does not exists
    GetLastVolume(FZipFileName, EOC, true); // wil read existing comment
#else
    if(FileExists(FZipFileName)) OpenEOC(EOC,false);
#endif
    FZipComment = zComment;
    if(FInFileHandle != -1)
    {
      FileClose(FInFileHandle);  // must reopen for read/write
      AnsiString CommentBuf = ConvertOEM(zComment, cpdISO2OEM);
      if(CommentBuf == FEOCComment) return; // same - nothing to do
      int len = CommentBuf.Length();
      FInFileHandle = FileOpen( FInFileName, fmShareDenyWrite | fmOpenReadWrite );
      if(FInFileHandle == -1)
          throw EZipBuilder(DS_FileOpen);
      __int64 FileOfs = FZipEOC; // convert to 64 bit
      if(FileSeek(FInFileHandle, FileOfs, 0 ) == -1)
          throw EZipBuilder(DS_FailedSeek);
      ReadJoin( &EOC, sizeof(EOC), DS_EOCBadRead);
      EOC.ZipCommentLen = static_cast<short>(len);
      FileOfs = -static_cast<__int64>(sizeof(EOC));
      if(FileSeek(FInFileHandle, FileOfs, 1) == -1)
        throw EZipBuilder(DS_FailedSeek);
      Fatal = true;
      if(FileWrite(FInFileHandle, &EOC, sizeof(EOC)) != sizeof(EOC))
        throw EZipBuilder(DS_EOCBadWrite);
      if(FileWrite( FInFileHandle, FZipComment.c_str(), len ) != len )
        throw EZipBuilder(DS_NoWrite);
      Fatal = false;
    // if SetEOF fails we get garbage at the end of the file, not nice but
    // also not important.
      SetEndOfFile((HANDLE)FInFileHandle);
    }
  }
	catch (EZipBuilder &ezl)
  {
		ShowExceptionError(ezl);
		FZipComment = "";
	}
	catch (const MEMEXCEPT &me)
  {
		ShowZipMessage(GE_NoMem);
		FZipComment = "";
  }
  CloseFileHandle(FInFileHandle);
	if(Fatal)	// Try to read the zipfile, maybe it still works.
	   List();
}
// TZipWorker::SetZipComment

/*? TZipWorker.IndexOf-----------------------------------------------------------
1.80
 Find specified filespec  returns index of Directory entry (-1 - not found)
*/
int __fastcall TZipWorker::IndexOf(const AnsiString fname)
{
  for(int i = 0; i < Count; ++i)
  {
    if(FileNameMatch(fname, static_cast<ZipDirEntry*>(DirEntry[i])->FileName)) return i;
  }
  return -1;
}
//? TZipWorker.IndexOf

/*? TZipWorker::Find-------------------------------------------------------------
1.73.4
 Find specified filespec after idx (<0 - from beginning)
 returns pointer to Directory entry (nil - not found)
 sets idx to index of found entry (-1 not found)
*/
ZipDirEntry* __fastcall TZipWorker::Find(const AnsiString fspec, int& idx)
{
  ZipDirEntry* Result;
  if(idx < 0) idx = -1;
  int c = Count - 1;
  while(idx < c)
  {
    Result = DirEntry[++idx];
    if(FileNameMatch(fspec, Result->FileName)) return Result;
  }
  idx = -1;
  return NULL;
}
//? TZipWorker::Find

/*?TZipWorker::Clear------------------------------------------------------------
1.77 17 August 2004 RA default for FBufSize to 64K
1.76 26 May 2004 expanded
1.74 14 January 2004 RP added
*/
void __fastcall TZipWorker::Clear(void)
{
  fIsDestructing = true;                   // stop callbacks
  AbortDlls();
  FCancel = true;
  FreeZipDirEntryRecords();
  FReenter = false;
  FDelaying.Clear();
	FRealFileSize = 0;
	FZipSOC = 0;
	FSFXOffset = 0;
	FZipComment = "";
	FIsSpanned = false;
	FFSpecArgs->Clear();
	FFSpecArgsExcl->Clear();
  FZipBusy = false;
  FUnzBusy = false;
	FZipFileName = "";
  FPassword = "";
  FPasswordReqCount = 1;
  FEncrypt = false;
  FSuccessCnt = 0;
  FAddCompLevel = 9;
  AutoExeViaAdd = false;
  FUnattended = false;
  FFreeOnDisk1			=  0;			// Don't leave anything free.
  FFreeOnAllDisks = 0;         // use all space
	FMaxVolumeSize			=  0;			// Use the maximum disk size.
	FMinFreeVolSize		= 65536;		// Reject disks with less bytes than...
	FCodePage         	= cpAuto;
	FIsSpanned        	= false;
  FDriveFixed = false;
	FZipComment = "";
	FHowToDelete			= htdAllowUndo;
	FAddStoreSuffixes 	= AddStoreExts() << assGIF << assPNG << assZ << assZIP << assZOO << assARC
														<< assLZH << assARJ << assTAZ << assTGZ << assLHA
														<< assRAR << assACE << assCAB << assGZ << assGZIP << assJAR;
	FUseDirOnlyEntries	= false;
	FDirOnlyCount			= 0;
  FBufSize = 65536;
#ifndef NO_SPAN
  FSpanOptions.Clear();
	FConfirmErase			= true;
#endif
  inherited::Clear();
}
//? TZipWorker::Clear

/*? SetZipSwitches---------------------------------------------------------------
1.75 13 March 2004 RP - supply app window handle
1.73  2 August 2003 RA - remove zpVersion parameter
1.73  1 August 2003 RP - set required dll interface version
*/
void __fastcall TZipWorker::SetZipSwitches(String &NameOfZipFile)
{
  if(FZipComment.Length())
  {
  	ZipParms->fArchComment = new char[FZipComment.Length() + 1];
  	StrPLCopy(ZipParms->fArchComment, FZipComment, FZipComment.Length() + 1);
  }
  ZipParms->fArchiveFilesOnly = ( FAddOptions.Contains(AddArchiveOnly) )  ? true : false;
  ZipParms->fResetArchiveBit  = ( FAddOptions.Contains(AddResetArchive) ) ? true : false;
  // Copy the list of file specs. to exclude to a char pointer array.
  if(FFSpecArgsExcl->Count)
  {
  	ZipParms->fTotExFileSpecs = FFSpecArgsExcl->Count;
  	ZipParms->fExFiles = new ExcludedFileSpec[FFSpecArgsExcl->Count];
  	for(int i = 0; i < FFSpecArgsExcl->Count; i++)
  	{
  		memset(ZipParms->fExFiles + i, 0, sizeof(ExcludedFileSpec));
  		ZipParms->fExFiles[i].fFileSpec = new char[FFSpecArgsExcl->Strings[i].Length() + 1];
  		StrPLCopy(ZipParms->fExFiles[i].fFileSpec, FFSpecArgsExcl->Strings[i],
  		                                           FFSpecArgsExcl->Strings[i].Length() + 1);
  	}
  }
  // New in v 1.6M Dll 1.6017, used when Add Move is choosen.
  if(FHowToDelete == htdAllowUndo) ZipParms->fHowToMove = 1;
    // allow OEM2ISo conversion in Delete function
  ZipParms->fWantedCodePage = FCodePage;
	if(FTempDir.Length())
	{			// Name of temp directory.
		ZipParms->fTempPath = new char[FTempDir.Length() + 1];
		StrPLCopy(ZipParms->fTempPath, FTempDir, FTempDir.Length() + 1);
	}
	ZipParms->fVersion = ZIPVERSION;		// version we expect the DLL to be
	ZipParms->fCaller = this;				// point to our VCL instance; returned in callback.

			/* Since we don't want the DLL's to generate their own dialog boxes
				for error reporting, we'll set the window handle. Since we
				are doing this, we must also set fQuiet to true. */
	ZipParms->fHandle = FHandle;				// Used for DLL dialogs - esp: password.
  ZipParms->fVCLVer = ZIPVERSION;
	ZipParms->fQuiet  = true;				   // We'll report errors upon notification in our callback.
	ZipParms->ZCallbackFunc = ZCallback;	// Pass addr of function to be called from DLL.
	ZipParms->fJunkSFX = false;				// if True, Convert input .EXE file to .ZIP.
	String SufStr = FExtAddStoreSuffixes; //new 1.71 add extra suffixes
	for(int i = 0; i < 1 + (int)assEXE; i++ ) AddSuffix((AddStoreSuffix)i, SufStr, i);
	if(SufStr.Length())
	{
		SufStr.Delete(SufStr.Length(), 1);
		ZipParms->pSuffix = new char[SufStr.Length() + 1];
		StrPLCopy(ZipParms->pSuffix, SufStr, SufStr.Length() + 1);
	}
			// if True, Include volume label from root dir.
	ZipParms->fVolume = (FAddOptions.Contains(AddVolume)) ? true : false;
	ZipParms->fExtra  = false;		// if True, Include extended file attributes.
      // if True, Exclude files earlier than specified date.
  ZipParms->fUseDate = (FAddOptions.Contains(AddFromDate)) ? true : false;
  memset(ZipParms->fDate,'\0',8);    //set array to 0
  String format = "mmddyy";
  String DateFormat = FFromDate.FormatString(format);
  strcpy(ZipParms->fDate,DateFormat.c_str());

	ZipParms->fLevel = FAddCompLevel;		// Compression level (0 - 9, 0=none and 9=best).
	ZipParms->fCRLF_LF = false;		// if True, Translate text file CRLF to LF (if dest Unix).
  ZipParms->fGrow = (FAddOptions.Contains(AddSafe)) ? false: true;  // new v1.71

	ZipParms->fDeleteEntries = false;		// distinguish between Add and Delete
	ZipParms->fTraceEnabled	= (FTrace)	 ? true : false;
	ZipParms->fVerboseEnabled = (FVerbose) ? true : false;
	if (ZipParms->fTraceEnabled && !FVerbose ) ZipParms->fVerboseEnabled = true;	// If tracing, we want verbose also.

			// Convert all filenames to 8x3 format.
	ZipParms->fForce = (FAddOptions.Contains(AddForceDOS)) ? true : false;
			// Make zipfile's timestamp same as newest file.
	ZipParms->fLatestTime = (FAddOptions.Contains(AddZipTime))  ? true : false;
			// Dangerous, beware!
	ZipParms->fMove = (FAddOptions.Contains(AddMove)) ? true : false;
	ZipParms->fFreshen = (FAddOptions.Contains(AddFreshen)) ? true : false;
	ZipParms->fUpdate = (FAddOptions.Contains(AddUpdate)) ? true : false;
	if(ZipParms->fFreshen && ZipParms->fUpdate) ZipParms->fFreshen = false;  	// Update has precedence over freshen.
	ZipParms->fEncrypt = (FAddOptions.Contains(AddEncrypt))	? true : false;

			/* NOTE: If user wants recursion, then he probably also wants
				AddDirNames, but we won't demand it. */
	ZipParms->fRecurse = (FAddOptions.Contains(AddRecurseDirs)) ? true : false;

			// Don't store dirnames by themselves is the default.
	ZipParms->fNoDirEntries = (FAddOptions.Contains(AddSeparateDirs)) ? false : true;
	ZipParms->fJunkDir = (!FAddOptions.Contains(AddDirNames)) ? true : false;
	ZipParms->fSystem	 = (FAddOptions.Contains(AddHiddenFiles)) ? true : false;

	ZipParms->pZipFN = new char[NameOfZipFile.Length() + 1];
	StrPLCopy( ZipParms->pZipFN, NameOfZipFile, NameOfZipFile.Length() + 1);	// Name of zip file.
	if(FPassword.Length())
	{
		ZipParms->pZipPassword = new char[FPassword.Length() + 1];
		StrPLCopy( ZipParms->pZipPassword, FPassword, PWLEN + 1);			// Password for encryption.
	}
}
// SetZipSwitches

// TZipWorker::GetZipComment---------------------------------------------------
// 1.73.3.2 11 Oct 2003 RP comment now converted when read
// 1.73 (2 June 2003) RA use ConvertOEM
String __fastcall TZipWorker::GetZipComment(void)
{
  return FZipComment;
}
// TZipWorker::GetZipComment

// TZipWorker::ConvertOEM--------------------------------------------------
// 1.73 (24 June 2003) RA HostVer parameter now struct zipHostInfo
// 1.73 (22 June 2003) RA added Host Ver parameters
// 1.73 (26 May 2003) RP use String for buffer
// new 1.73 (15 May 2003) replacement function that should be able to handle MBCS
/* Convert filename (and file comment string) into "internal" charset (ISO).
 * This function assumes that Zip entry filenames are coded in OEM (IBM DOS)
 * codepage when made on:
 *  -> DOS (this includes 16-bit Windows 3.1)  (FS_FAT_  {0} )
 *  -> OS/2                                    (FS_HPFS_ {6} )
 *  -> Win95/WinNT with Nico Mak's WinZip      (FS_NTFS_ {11} && hostver == "5.0" {50} )
 *
 * All other ports are assumed to code zip entry filenames in ISO 8859-1.
 * But norton Zip v1.0 sets the host byte as OEM(0) but does use the ISO set,
 * thus archives made by NortonZip are not recognized as being ISO.
 * (In this case you need to set the CodePage property manualy to cpNone.)
 * When ISO is used in the zip archive there is no need for translation
 * and so we call this cpNone.
 */
String __fastcall TZipWorker::ConvertOEM(const String Source, CodePageDirection Direction)
{
  String  Result = Source;
  if ( FCodePage == cpAuto &&
	   (FHostNum == FS_FAT_ || FHostNum == FS_HPFS_ || (FHostNum == FS_NTFS_ && FHostVer == 50))
		 || FCodePage == cpOEM )
	{
    String buf;
    buf.SetLength(2 * Source.Length() + 1); // allow worst case - all double
    if(Direction == cpdOEM2ISO) OemToChar(Source.c_str(), buf.c_str());
    else CharToOem(Source.c_str(),buf.c_str());
    Result = buf.c_str();
  }
  return Result;
}
// TZipWorker::ConvertOEM

// TZipWorker::ReadJoin--------------------------------------------------------
// 1.73 (22 May 2003) RP new function to partner WriteJoin
void __fastcall TZipWorker::ReadJoin(void *Buffer, int BufferSize, int DSErrIdent)
{
  if(FileRead(FInFileHandle, Buffer, BufferSize) != BufferSize)
    throw EZipBuilder(DSErrIdent);
}
// TZipWorker::ReadJoin

//---------------------------------------------------------------------------
void __fastcall TZipWorker::SetVersionInfo(String Value)
{
	// We do not want that this can be changed, but we do want to see it in the OI.
}
//---------------------------------------------------------------------------
void __fastcall TZipWorker::SetPasswordReqCount(unsigned int prc)
{
	if(prc != FPasswordReqCount)	FPasswordReqCount = min(prc, 15U);
}

/*? TZipWorker::FreeZipDirEntryRecords------------------------------------------
1.73 (12 July 2003 RP string ExtraData
 Free the storage used for dir entries and Empty FZipContents.
*/
void __fastcall TZipWorker::FreeZipDirEntryRecords(void)
{
  if(!ZipContents->Count) return;
	// Dispose of the memory pointed-to by all entries.
	for(int i = 0; i < FZipContents->Count; i++)
		if(FZipContents->Items[i])
		{
			reinterpret_cast<ZipDirEntry *>(FZipContents->Items[i])->ExtraData = "";
			delete reinterpret_cast<ZipDirEntry *>(FZipContents->Items[i]);
		}
	// Delete all pointers from the TList itself.
	FZipContents->Clear();
	// The FZipContents TList will be deleted when the component destructor is called.
}
// TZipWorker::FreeZipDirEntryRecords

/*? TZipWorker.BeforeDestruction
1.73 3 July 2003 RP stop callbacks
*/
void __fastcall TZipWorker::BeforeDestruction(void)
{
  fIsDestructing = true; // stop callbacks
  inherited::BeforeDestruction();
}

/*? TZipWorker::~TZipWorker----------------------------------------------------
1.73 (30 July 2003) RA use TInternalSFX
1.73 (16 June 2003) RP set IsDestroying & new dll handling
*/
__fastcall TZipWorker::~TZipWorker( void )
{
  fIsDestructing = true;		// new 1.73
  AbortDlls();
	FreeZipDirEntryRecords();
	delete FZipContents;
	delete FFSpecArgsExcl;
	delete FFSpecArgs;
#ifndef NO_SPAN
#endif
#ifndef NO_SFX
	delete FSFXIcon;
#endif
#ifdef INTERNAL_SFX
  delete FIntSFX;
#endif
#ifndef NO_STREAM
  delete FZipStream;
#endif
  delete fUnzDll;
	delete fZipDll;
}
//? TZipWorker::~TZipWorker

#ifndef NO_SPAN
/*? TZipWorker::IsRightDisk-----------------------------------------------------
1.73 (29 June 2003) RP amended
*/
bool __fastcall TZipWorker::IsRightDisk(void)
{
  if(!FDriveFixed && (FVolumeName == "PKBACK# " + IntToStr(1001 + FDiskNr).SubString(2, 3)) &&
    (FileExists(FInFileName))) return true;
  CreateMVFileName(FInFileName, true);
  // For fixed disks the disk is always right, we only need to change the filename.
  if(!FDriveFixed) return FileExists(FInFileName); // must exist
  else return true;
}
//? TZipWorker::IsRightDisk
#endif

#ifndef NO_SPAN
/*? TZipWorker::ClearFloppy-----------------------------------------------------
1.73 (10 July 2003) RP changed trace messages
*/
void __fastcall TZipWorker::ClearFloppy(String Dir)
{
	TSearchRec SRec;
  AnsiString Dirname;
  if(FindFirst(Dir + "*.*", faAnyFile, SRec) == 0)
  {
    do
    {
      if(SRec.Attr == faDirectory && SRec.Name != "." && SRec.Name != "..")
      {
        Dirname = Dir + SRec.Name + (AnsiString)"\\" ;
        ClearFloppy(Dirname);
        if(FTrace) Report(zacMessage, 0, ZipFmtLoadStr(TM_Erasing,ARRAYOFCONST((Dirname))), 0);
        else Report(zacTick, 0, "", 0);
        RemoveDir(Dirname);
      }
      else
      {
        if(FTrace) Report(zacMessage, 0,ZipFmtLoadStr(TM_Deleting,ARRAYOFCONST((Dirname))), 0);
      	DeleteFile(Dir + SRec.Name);
      }
     } while(FindNext(SRec) == 0);
    FindClose(SRec);
  }
}
//? TZipWorker::ClearFloppy
#endif

#ifndef NO_SPAN
/*? TZipWorker::ZipFormat--------------------------------------------------------
1.73 (27 June 2003) RP change handling of 'No'
 return values
 -1 Error on last format, Drive may be formatable
 -2 last format cancelled
 -3 drive is not foramtable
*/
int FormatFloppy(HWND WND, String Drive)
{
	const unsigned int SHFMT_ID_DEFAULT = 0xFFFF; //defining constants avoids including shlobj.h
	const unsigned int SHFMT_OPT_FULL = 1;

	typedef int __stdcall (*TSHFormatDrive)(HWND WND, unsigned int Drive,unsigned int fmtID,
	                                          unsigned int Options);
	TSHFormatDrive SHFormatDrive = NULL;
	int Result = -3;
	char DriveLetter = Drive.UpperCase()[1];
	if(!((Drive.Length() > 1) && (Drive[2] == ':') && !((DriveLetter < 'A') || (DriveLetter > 'Z'))))
			return Result;
	if(GetDriveType(Drive.c_str()) != DRIVE_REMOVABLE) return Result;
	unsigned int drv = DriveLetter - 'A';
	long OldErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
	HINSTANCE hLib = LoadLibrary("Shell32");
	if(hLib)
	{
		SHFormatDrive = (TSHFormatDrive)::GetProcAddress(hLib, "SHFormatDrive");
		if(SHFormatDrive)
		{
			try
			{
					Result = SHFormatDrive(WND, drv, SHFMT_ID_DEFAULT, SHFMT_OPT_FULL);
			}
			__finally
			{
				FreeLibrary(hLib);
			}
			SetErrorMode(OldErrorMode);
		}
	}
  return Result;
}

int __fastcall TZipWorker::ZipFormat(void)
{
	int Result = -3;
  if(FSpanOptions.Contains(spTryFormat) && !FDriveFixed)  // changed 1.72
	     Result = FormatFloppy(Application->Handle, FDrive);
	if(Result == -3)
	{
    if(FConfirmErase)
    {
		  AnsiString Msg = ZipFmtLoadStr(FM_Erase, ARRAYOFCONST((FDrive)));
		  int  Res = ZipMessageDlg(ZipLoadStr(FM_Confirm), Msg,
                               zmtWarning + DHC_FormErase,
                               TMsgDlgButtons() << mbYes << mbNo);
		  if(Res != IDYES) return -3;  // no  was -2; // cancel
    }
		ClearFloppy(FDrive);
		Result = 0;
	}
	AnsiString Vol;
	if(FVolumeName.Length() > 11) Vol = FVolumeName.SubString(1, 11);
	else Vol = FVolumeName;
	if(Result == 0 && !(FSpanOptions.Contains(spNoVolumeName))) SetVolumeLabel(FDrive.c_str(), Vol.c_str());
  return Result;
}
//? TZipWorker::ZipFormat
#endif

//----------------------------ZipDataList-----------------------------------
MDZipData* TMZipDataList::GetItems(int Index)
{
  if(Index > Count) throw EZipBuilder(GE_RangeError, ARRAYOFCONST((Index, Count-1)));
	return static_cast<MDZipData*>(TList::Items[Index]);
}
//-----------------------------
__fastcall TMZipDataList::TMZipDataList(int TotalEntries) : TList()
{
	Capacity = TotalEntries;
	for(int i = 0; i < TotalEntries; i++)
	{
		MDZipData* MDZD = new MDZipData;
		MDZD->FileName = NULL;
		Add(MDZD);
	}
}
//------------------------------
__fastcall TMZipDataList::~TMZipDataList(void)
{
	if(Count > 0)
	{
		for(int i = Count - 1; i >= 0; i--)
		{
			MDZipData* MDZD = Items[i];
      if(MDZD->FileName) delete[] MDZD->FileName;
			if(MDZD) delete MDZD;	// free the memory pointed-to by this entry
		}
	Clear();	// delete all pointer from the list
	}
}
//-------------------------------
int TMZipDataList::IndexOf(const AnsiString& fname)
{
  int Result;
	for(Result = 0; Result < Count; Result++)
	{
    MDZipData* MDZD = Items[Result];
		if(CompareText(fname,(AnsiString)MDZD->FileName) == 0) break;// case insensitive compare
	}
	// Should not happen, but maybe in a bad archive..
	if(Result == Count) throw EZipBuilder(DS_EntryLost, true);

  return Result;
}
//----------------------------------------------------------------------------
// DeleteFile with a pure AnsiString wasn't possible in BCB1.
#if __BORLANDC__ < 1328		// BCB1=0x0520(1312)
namespace Sysutils {
	bool __fastcall DeleteFile( const System::AnsiString aStr ) {
		return ::DeleteFile( aStr.c_str() );
	}
}
#endif

//---------------------------------------------------------------------------
int __fastcall TZipWorker::GetCount(void)
{
	return (FZipFileName != "") ? FZipContents->Count : 0;
}

//---------------------------------------------------------------------------
bool __fastcall TZipWorker::IsFixedDrive(AnsiString drv)
{
  int drt = ::GetDriveType(drv.c_str());
  return !(drt == DRIVE_REMOVABLE || drt == DRIVE_CDROM);
}

//------------------------------------------------------------------------------
// Add suffixes not contained in the set
void __fastcall TZipWorker::SetExtAddStoreSuffixes (String Value)
{
  if(Value != "")
  {
		int i = 1;
    AnsiString str;
    char c = ':';
		while(i <= Value.Length())
    {
			if((c = Value[i]) != '.') str += ".";
			while((c != ':') && (i <= Value.Length()))
      {
        c = (Value.IsDelimiter(",;:",i)) ? ':' : Value[i];
        str += c;
        ++i;
      }
		}
		if(c != ':') str += ":";
		FExtAddStoreSuffixes = LowerCase(str);
	}
	else FExtAddStoreSuffixes = "";
}

//---------------------------------------------------------------------------
// Add a new suffix to the suffix string if contained in the set 'FAddStoreSuffixes'
void __fastcall TZipWorker::AddSuffix( const AddStoreSuffix SufOption, String &sStr, int sPos )
{
	static char *SuffixStrings[] = { "gif", "png", "z", "zip", "zoo","arc", "lzh", "arj", "taz",
                                   "tgz", "lha", "rar", "ace", "cab", "gz", "gzip", "jar", "exe" };
	if(FAddStoreSuffixes.Contains(SufOption))
		sStr += ("." + AnsiString( SuffixStrings[sPos]) + ":");
}

// SetUnZipSwitches---------------------------------------------------------------
//1.73  4 August 2003 RA - remove UzpVersion parameter
void __fastcall TZipWorker::SetUnZipSwitches( String &NameOfZipFile)
{
	UnZipParms->fVersion = ZIPBUILDERVER;		// Version we tell DLL we are
	UnZipParms->fCaller  = this;				// point to our VCL instance; returned in Report.

	/* Since we don't want the DLLs to generate their own dialog boxes
		for error reporting, we'll set the window handle. Since we
		are doing this, we must also set fQuiet to true.					*/
	UnZipParms->fHandle = FHandle;				// Used for DLL dialogs - esp: password.
	UnZipParms->fQuiet  = true;  				// We'll report errors upon notification in our Report.
	UnZipParms->ZCallbackFunc = ZCallback;	// Pass addr of function to be called from DLL.

  UnZipParms->fWantedCodePage = FCodePage;
	UnZipParms->fTraceEnabled  = (FTrace) ? true : false;
	UnZipParms->fVerboseEnabled = (FVerbose) ? true : false;
	if(UnZipParms->fTraceEnabled && !UnZipParms->fVerboseEnabled)	UnZipParms->fVerboseEnabled = true;	// If tracing, we want verbose also.

	UnZipParms->fComments = false;				// Zipfile comments - not supported.
	UnZipParms->fConvert = false;				// ASCII/EBCDIC conversion - not supported.
	UnZipParms->fDirectories = (FExtrOptions.Contains(ExtrDirNames)) ? true : false;
	UnZipParms->fOverwrite = (FExtrOptions.Contains(ExtrOverWrite)) ? true : false;
	UnZipParms->fFreshen = (FExtrOptions.Contains(ExtrFreshen)) ? true : false;
	UnZipParms->fUpdate = (FExtrOptions.Contains(ExtrUpdate)) ? true : false;
	UnZipParms->fTest	= (FExtrOptions.Contains(ExtrTest))	? true : false;

	// Update has precedence over freshen.
	if(UnZipParms->fFreshen && UnZipParms->fUpdate) UnZipParms->fFreshen = false;

	UnZipParms->pZipFN = new char[NameOfZipFile.Length() + 1];	// Allocate room for null terminated string.
	StrPLCopy(UnZipParms->pZipFN, NameOfZipFile, NameOfZipFile.Length() + 1);		// Name of zip file.

	UnZipParms->fPwdReqCount = FPasswordReqCount;
	// We have to be carefull doing an unattended Extract when a password is needed
	// for some file in the archive. We set it to an unlikely password, this way
	// encrypted files won't be extracted.
	// From verion 1.60 and up the event OnPasswordError is called in this case.
	UnZipParms->pZipPassword = new char[FPassword.Length() + 1];	// Allocate room for null terminated string.
	StrPLCopy(UnZipParms->pZipPassword, FPassword, PWLEN + 1);	// Password for encryption/decryption.
}
// SetUnZipSwitches

//---------------------------------------------------------------------------
int __fastcall TZipWorker::Add(void)
{
  ExtAdd( 0, 0, 0, NULL);
  return FErrCode;
}

#ifndef NO_STREAM
// TZipWorker::AddStreamToStream-----------------------------------------------
// 1.73( 21 June 2003) RA FSuccessCnt set to 0 in case of error previous count remained
TZipStream * __fastcall TZipWorker::AddStreamToStream(TMemoryStream *InStream)
{
  FSuccessCnt = 0;
	if(InStream == FZipStream)
	{
		ShowZipMessage(AD_InIsOutStream);
		return NULL;
	}
	if(InStream->Size)
	{
		FZipStream->SetSize(InStream->Size + 6);
		ExtAdd(2, 0, 0, InStream);
		// The size of the output stream is reset by the dll in ZipParms2 in fOutStreamSize.
		// Also the size is 6 bytes more than the actual output size because:
		// - the first two bytes are used as flag, STORED=0 or DEFLATED=8.
		//   short int Flag = *(short int *)ZipBuilder1->FZipStream->Memory;
		// - the next four bytes are set to the calculated CRC value.
		//   unsigned long Crc = *(unsigned long *)((char *)ZipBuilder1->FZipStream->Memory + 2);
		// The size is reset from Inputsize +6 to the actual data size +6.
		// (you do not have to set the size yourself, in fact it won't be taken into account.
		// The start of the stream is set to the actual data start.
		if(FSuccessCnt == 1) FZipStream->Position = 6;
		else FZipStream->SetSize(0);
	}
	else ShowZipMessage(AD_NothingToZip);
	return FZipStream;
}
// TZipWorker::AddStreamToStream
#endif

// TZipWorker::Delete---------------------------------------------------------
// 1.73 6 September 2003 RA throw exception if dll returns negative value
// 1.73 2 August 2003 RA removed loading of dll will be done in Exec
// 1.73 (16 July 2003) RA catch and display dll load errors
// 1.73 (13 July 2003) RA for spanned archive no exception but show mesage
int __fastcall TZipWorker::Delete( void )
{
  ZipEndOfCentral  EOC;
	FSuccessCnt = FErrCode = 0;
	FMessage = "";
	if(!FFSpecArgs->Count)
	{
		ShowZipMessage(DL_NothingToDel);
		return FErrCode;
	}
  if(!FileExists(FZipFileName))
	{
		ShowZipMessage(GE_NoZipSpecified);
		return FErrCode;
	}
  OpenEOC(EOC, false);
  CloseFileHandle(FInFileHandle);
  if(FIsSpanned)
  {
  	ShowZipMessage(DL_NoDelOnSpan); //function not allowed in spanned archive
		return FErrCode;
  }
  // Make sure we can't get back in here while work is going on
  if(FZipBusy) return FErrCode;
  FZipBusy = true;     // delete uses the ZIPDLL, so it shares the FZipBusy flag
  Cancel = false;
  try
	{
		try
		{
			ZipParms = new ZipDLL::ZipParms;
			memset(ZipParms, 0, sizeof(*ZipParms));
		  SetZipSwitches(FZipFileName);
		  // Override "Add" behavior assumed by SetZipSwitches.
		  ZipParms->fDeleteEntries = true;
		  ZipParms->fGrow	= false;
		  ZipParms->fJunkDir = false;
		  ZipParms->fMove = false;
		  ZipParms->fFreshen = false;
		  ZipParms->fUpdate = false;
		  ZipParms->fRecurse = false;	 // Bug fix per Angus Johnson.
		  ZipParms->fEncrypt = false;   // You don't need the password to delete a file.

		  ZipParms->fFDS = new FileData[FFSpecArgs->Count];
			  for( int i = 0; i < FFSpecArgs->Count; i++ )
        {
				  memset(ZipParms->fFDS + i, 0, sizeof(FileData));
				  ZipParms->fFDS[i].fFileSpec = new char[FFSpecArgs->Strings[i].Length() + 1];
				  StrPLCopy(ZipParms->fFDS[i].fFileSpec, FFSpecArgs->Strings[i], FFSpecArgs->Strings[i].Length() + 1);
			  }
			ZipParms->fSeven = 7;	// used to QC the data structure passed to DLL
		  // argc is now the no. of filespecs we want deleted.
		  ZipParms->fArgc = FFSpecArgs->Count;
      FEventErr = "";                      // added
      FSuccessCnt = fZipDll->Exec(ZipParms, Min_ZipDll_Vers);
		}
     catch (EZipBuilder &ers)
    {
      if(FEventErr != "") ers.Message += FEventErr;
      ShowExceptionError(ers);
    }
		catch ( ... )
		{
			ShowZipMessage(GE_FatalZip);
		}
	}
	__finally
	{
		FFSpecArgs->Clear();
		FFSpecArgsExcl->Clear();
		delete[] ZipParms->pSuffix;
		delete[] ZipParms->pZipFN;
		delete[] ZipParms->pZipPassword;
		delete[] ZipParms->fTempPath;
		delete[] ZipParms->fArchComment;
		for(int i = ZipParms->fArgc - 1; i >= 0; i--) delete[] ZipParms->fFDS[i].fFileSpec;
		delete[] ZipParms->fFDS;
		for(int i = ZipParms->fTotExFileSpecs - 1; i >= 0; i--)	delete[] ZipParms->fExFiles[i].fFileSpec;
		delete[] ZipParms->fExFiles;
		delete ZipParms;
		ZipParms = NULL;

		fZipDll->Unload(false);
    FZipBusy = false;
		Cancel = false;
		if(FSuccessCnt > 0) List();  // Update the Zip Directory by calling List() function.
	}
  return FErrCode;
}
// TZipWorker::Delete

#ifndef NO_STREAM
//---------------------------------------------------------------------------
void __fastcall TZipStream::SetPointer(void *Ptr, int Size)
{
	TMemoryStream::SetPointer(Ptr, Size);
}
#endif

#ifndef NO_STREAM
// TZipWorker::ExtractFileToStream---------------------------------------------
// 1.73( 21 June 2003) RA FSuccessCnt set to 0 in case of error previous count remained
TZipStream * __fastcall TZipWorker::ExtractFileToStream(String FileName )
{
  FSuccessCnt = 0;
	// Use FileName if set, if not expect the filename in the FFSpecArgs.
	if(FileName != "")
	{
		FFSpecArgs->Clear();
		FFSpecArgs->Add(FileName);
	}
  if(FFSpecArgs->Count)
  {
	  FZipStream->Clear();
	  ExtExtract(1, NULL);
	  return (FSuccessCnt != 1) ? NULL : FZipStream;
  }
  else ShowZipMessage(AD_NothingToZip);
  return NULL;
}
// TZipWorker::ExtractFileToStream
#endif

#ifndef NO_STREAM
// TZipWorker::ExtractStreamToStream-------------------------------------------
// 1.73( 21 June 2003) RA FSuccessCnt set to 0 in case of error previous count remained
TZipStream * __fastcall TZipWorker::ExtractStreamToStream(TMemoryStream *InStream,
                                                             unsigned long OutSize)
{
  FSuccessCnt = 0;
	if(InStream == FZipStream)
	{
		ShowZipMessage(AD_InIsOutStream);
		return NULL;
	}
	FZipStream->Clear();
	FZipStream->SetSize(STRMSIZ(OutSize));//Changed R. Peters for BCB6
	ExtExtract(2, InStream);
	return (FSuccessCnt != 1) ? NULL : FZipStream;
}
// TZipWorker::ExtractStreamToStream
#endif

//---------------------------------------------------------------------------
int __fastcall TZipWorker::Extract(void)
{
  ExtExtract(0, NULL);
  return FErrCode;
}

/*? TZipWorker::Copy_file-------------------------------------------------------------
1.75 18 February 2004 Allow >2G
 Returns 0 if good copy, or a negative error code.
*/
int __fastcall TZipWorker::Copy_File(const String InFileName, const String OutFileName)
{
	const
		SE_CreateError   = -1,	// Error in open or creation of OutFile.
		SE_OpenReadError = -3,	// Error in open or Seek of InFile.
		SE_SetDateError  = -4,	// Error setting date/time of OutFile.
		SE_GeneralError  = -9;
	int InFile, OutFile, Result = SE_OpenReadError;
	__int64 InSize = -1, OutSize = -1;
	FShowProgress = zspNone;
	if(!FileExists(InFileName)) return Result;
	StartWaitCursor();
	if((InFile = FileOpen(InFileName, fmOpenRead | fmShareDenyWrite )) != -1)
	{
		if(FileExists(OutFileName)) EraseFile(OutFileName, FHowToDelete);
		if((OutFile = FileCreate(OutFileName)) != -1)
		{
			Result = CopyBuffer(InFile, OutFile, -1);
			if(!Result && FileSetDate(OutFile, FileGetDate(InFile)) != 0)
			        Result = SE_SetDateError;
			OutSize = FileSeek(OutFile, 0i64, 2);
      CloseFileHandle(OutFile);
		}
		else Result = SE_CreateError;
		InSize = FileSeek(InFile, 0i64, 2);
    CloseFileHandle(InFile);
	}
	// An extra check if the filesizes are the same.
	if(!Result && (InSize == -1 || OutSize == -1 || InSize != OutSize))
			Result = SE_GeneralError;
	// Don't leave a corrupted outfile lying around. (SetDateError is not fatal!)
	if(Result && Result != SE_SetDateError)
		 DeleteFile( OutFileName );
	StopWaitCursor();
	return Result;
}

// TZipWorker::CopyBuffer------------------------------------------------------
// 1.73 (16 May 2003) RA changed to use callback
// Used by CopyFile, ConvertSFX, ConvertZip and CopyZippedFiles.
// If ReadLen == -1 the remaining part of InFile is copied.
// If ReadLen >= 0 than ReadLen bytes are copied.
int __fastcall TZipWorker::CopyBuffer(int &InFile, int &OutFile, int ReadLen)
{
	const	SE_CopyError = -2;	// Write error or no memory during copy.
	int SizeR, ToRead = FBufSize;
  int  Result = SE_CopyError;
	char *Buffer = NULL;

	try
	{
		Buffer = new char[FBufSize];
		Result = 0;					// No memory error!
		do
		{
			if(ReadLen >= 0) ToRead = min(ReadLen, FBufSize);
			SizeR = FileRead(InFile, Buffer, ToRead);
			if(FileWrite(OutFile, Buffer, SizeR) != SizeR)
			{
				Result = SE_CopyError;
				break;
			}
      if(FShowProgress != zspNone) Report(zacProgress, 0,"", SizeR);
      else Report(zacTick,0,"",0);
			if(ReadLen > 0) ReadLen -= SizeR;
		}
		while(SizeR == ToRead && ReadLen);
	}
	catch ( ... ) { }
	delete[] Buffer;
	return Result;
}
// TZipWorker::CopyBuffer

// TZipWorker::Load_Zip_Dll----------------------------------------------------
// 1.73 (16 July 2003) RA catch and diplay dll load errors
// 1.73 (14 June 2003) RP new dll handling
// This is new code that tries to locate the DLL before loading it.
// The user can specify a dir in the DLLDirectory property.
// The user's dir is our first choice, but we'll still try the
// standard Windows DLL dirs (App. start dir, Current dir, Windows System, Windows, Path).
int __fastcall TZipWorker::Load_Zip_Dll( void )  //changed in v1.70
{
  int Result;
  try
  {
    Result = fZipDll->LoadDll(Min_ZipDll_Vers,true);
  }
  catch( EZipBuilder &ews)
  {
    ShowExceptionError(ews);
    Result = 0;
  }
  return Result;
}
// TZipWorker::Load_Zip_Dll

// TZipWorker::Load_Unz_Dll----------------------------------------------------
// 1.73 (16 July 2003) RA catch and diplay dll load errors
// 1.73 (14 June 2003) RP new dll handling
// This is new code that tries to locate the DLL before loading it.
// The user can specify a dir in the DLLDirectory property.
// The user's dir is our first choice, but we'll still try the
// standard Windows DLL dirs (App. start dir, Current dir, Windows System, Windows, Path).
int __fastcall TZipWorker::Load_Unz_Dll(void)  //changed in v1.70
{
  int Result;
  try
  {
    Result = fUnzDll->LoadDll(Min_UnzDll_Vers,true);
  }
  catch(EZipBuilder &ews)
  {
    ShowExceptionError(ews);
    Result = 0;
  }
  return Result;
}
// TZipWorker::Load_Unz_Dll

// TZipWorker::Unload_Zip_Dll--------------------------------------------------
// 1.73 (14 June 2003) RP new dll handling
void __fastcall TZipWorker::Unload_Zip_Dll(void)
{
  fZipDll->Unload(true);
}
// TZipWorker::Unload_Zip_Dll

// TZipWorker::Unload_Unz_Dll--------------------------------------------------
// 1.73 (14 June 2003) RP new dll handling
void __fastcall TZipWorker::Unload_Unz_Dll(void)
{
  fUnzDll->Unload(true);
}
// TZipWorker::Unload_Unz_Dll

//---------------------------------------------------------------------------
String __fastcall TZipWorker::ZipDllPath(void)
{
  return fZipDll->Path;
}
//---------------------------------------------------------------------------
String __fastcall TZipWorker::UnzDllPath(void)
{
  return fUnzDll->Path;
}

// TZipWorker::GetZipVers------------------------------------------------------
// 1.73 (14 June 2003) RP new dll handling
int __fastcall TZipWorker::GetZipVers(void)
{
  return fZipDll->Version;
}
// TZipWorker::GetZipVers

// TZipWorker::GetUnzVers------------------------------------------------------
// 1.73 (14 June 2003) RP new dll handling
int  __fastcall TZipWorker::GetUnzVers(void)
{
  return fUnzDll->Version;
}
// TZipWorker::GetUnzVers

//---------------------------------------------------------------------------
// Replacement for the functions DiskFree and DiskSize.
// This should solve problems with drives > 2Gb and UNC filenames.
// Path FDrive ends with a backslash.
// Action=1 FreeOnDisk, 2=SizeOfDisk, 3=Both
void __fastcall TZipWorker::DiskFreeAndSize(int Action)
{	// RCV150199
	DWORD					SectorsPCluster, BytesPSector, FreeClusters, TotalClusters;
	__int64				LDiskFree = -1, LSizeOfDisk = -1;
	HMODULE				Lib = ::GetModuleHandle( "kernel32" );
	GDiskFreeSpaceEx	GetDiskFreeSpaceEx;

	if(Lib)
	{
		GetDiskFreeSpaceEx = (GDiskFreeSpaceEx)::GetProcAddress(Lib, "GetDiskFreeSpaceExA");
		if(GetDiskFreeSpaceEx)	// We probably have W95+OSR2 or better.
			if(!GetDiskFreeSpaceEx(FDrive.c_str(), &LDiskFree, &LSizeOfDisk, NULL))
				LDiskFree = LSizeOfDisk = -1;
		::FreeLibrary(Lib);	//v1.52j
	}
	// Do we have W95 original or W95+OSR1 or an error?
	if(LDiskFree == -1)
	{	// We use this because DiskFree/Size don't support UNC drive names.
		if(GetDiskFreeSpace(FDrive.c_str(), &SectorsPCluster, &BytesPSector, &FreeClusters, &TotalClusters))
		{
			LDiskFree   = (__int64)BytesPSector * SectorsPCluster * FreeClusters;
			LSizeOfDisk = (__int64)BytesPSector * SectorsPCluster * TotalClusters;
		}
	}
	if(Action & 1) FFreeOnDisk = LDiskFree;
	if(Action & 2) FSizeOfDisk = LSizeOfDisk;
}

// TZipWorker::GetDriveProps---------------------------------------------------
// 1.73 (30 July 2003) RA set VolName to 0 to avoid rubbish in FVolumeName
// Check to see if drive in FDrive is a valid drive.
// If so, put it's volume label in FVolumeName,
//        put it's size in FSizeOfDisk,
//        put it's free space in FDiskFree,
//        and return true.
// was IsDiskPresent
// If not valid, return false.
// Called by List() and CheckForDisk().
bool __fastcall TZipWorker::GetDriveProps(void)
{
	unsigned long Dummy1, Dummy2;
	char DriveLetter = FDrive.UpperCase()[1], VolName[MAX_PATH];
  VolName[0] = '\0'; // avoid string with rubbish
	if(DriveLetter != '\\')
	{	// Only for local drives
		if(DriveLetter < 'A' || DriveLetter > 'Z')
			throw EZipBuilder(DS_NotaDrive, FDrive);
		if(!(::GetLogicalDrives() & (1 << (DriveLetter - 'A'))))
			throw EZipBuilder(DS_DriveNoMount, FDrive);
	}
	// Turn off critical errors.
	unsigned int OldMode = ::SetErrorMode(SEM_FAILCRITICALERRORS);
  unsigned long DiskSerial;
	if(!FDriveFixed && !::GetVolumeInformation(FDrive.c_str(), VolName, MAX_PATH, &DiskSerial, &Dummy1, &Dummy2, NULL, 0))
	{  // nor for fixed drives
		// W'll get this if there is a disk but it is not or wrong formatted
		// so this disk can only be used when we also want formatting.
		::SetErrorMode( OldMode );	// v1.52j
		return (GetLastError() == 31 && FAddOptions.Contains(AddDiskSpanErase)) ? true : false;
	}
  FVolumeName = VolName;
  // Get free disk space and size.
	DiskFreeAndSize(3);	// RCV150199

	// Restore old error mode.
	::SetErrorMode( OldMode );

	// -1 is not very likely to happen since GetVolumeInformation catches errors.
	// But on W95(+OSR1) and a UNC filename w'll get also -1, this would prevent
	// opening the file. !!!Potential error while using spanning with a UNC filename!!!
	return(DriveLetter == '\\' || ( DriveLetter != '\\' && FSizeOfDisk != -1)) ? true : false;
}
// TZipWorker::GetDriveProps

/*? TZipWorker::AppendSlash
1.76
*/
AnsiString __fastcall TZipWorker::AppendSlash(AnsiString sDir)
{
  if(!sDir.IsEmpty() && sDir[sDir.Length()] != '\\')
    return sDir + "\\";
  else
    return sDir;
}
//? TZipWorker::AppendSlash

//---------------------------------------------------------------------------
ZipDirEntry* __fastcall TZipWorker::GetDirEntry(int idx)
{
	return reinterpret_cast<ZipDirEntry *>(FZipContents->Items[idx]);
}

// TZipWorker::FullVersionString-----------------------------------------------
// 1.73 (14 June 2003) RP new dll handling
String __fastcall TZipWorker::FullVersionString(void) //new 1.70
{
	String Result = "ZipBuilder " + ZIPBUILDERBUILD + " "
#ifdef NO_SPAN
	+ " -SPAN "
#endif
#ifdef NO_SFX
	+ " -SFX "
#endif
#ifdef NO_STREAM
  + " -STREAM "
#endif
	;
  Result += ", ZipDll " + FileVersion(fZipDll->Path);
	Result += ", UnzDll " + FileVersion(fUnzDll->Path);
	return Result;
}
// TZipWorker::FullVersionString

#ifndef NO_SPAN
//---------------------------------------------------------------------------
// Read data from the input file with a maximum of 8192(BufSize) bytes per read
// and write this to the output file.
// In case of an error an EZipBuilder Exception is thrown and this will
// be caught in WriteSpan.
void __fastcall TZipWorker::RWSplitData(char *Buffer, int ReadLen, int ZSErrVal)
{
	int SizeR, ToRead;

	while(ReadLen > 0)
	{
		ToRead = min(ReadLen, FBufSize);
		if((SizeR = FileRead(FInFileHandle, Buffer, ToRead)) != ToRead)
			throw EZipBuilder(ZSErrVal);
		WriteSplit(Buffer, SizeR, 0);
		ReadLen -= SizeR;
	}
}
#endif

#ifndef NO_SPAN
//---------------------------------------------------------------------------
void __fastcall TZipWorker::RWJoinData(char *Buffer, int ReadLen, int DSErrIdent)
{
	int SizeR, ToRead;

	while(ReadLen > 0)
	{
		ToRead = min(ReadLen, FBufSize);
		if((SizeR = FileRead(FInFileHandle, Buffer, ToRead)) != ToRead)
		{
			// Check if we are at the end of a input disk.
			if(FileSeek(FInFileHandle, 0, 1) != FileSeek(FInFileHandle, 0, 2))
				throw EZipBuilder(DSErrIdent);
			// It seems we are at the end, so get a next disk.
			GetNewDisk(FDiskNr + 1);
		}
		if(SizeR > 0)
		{ // Fix by Scott Schmidt v1.52n
			WriteJoin(Buffer, SizeR, DSErrIdent);
			ReadLen -= SizeR;
		}
	}
}
#endif

#ifdef INTERNAL_SFX
// TZipWorker::ConvertZIP------------------------------------------------------
// 1.73( 30 July 2003) RA use TInternalSFX class
// 1.73( 21 June 2003) RA FSuccessCnt set to 0
// Convert an .EXE archive to a .ZIP archive.
// Returns 0 if good, or else a negative error code.
int __fastcall TZipWorker::ConvertZIP(void)
{
	const
    SE_OpenReadError = -3,  // Error in open of InFile.
    SE_GeneralError  = -9;
  int Result = SE_GeneralError;
  FSuccessCnt = 0;
	FShowProgress = zspNone;
	if(!FileExists(FZipFileName))
	{
		ShowZipMessage(CZ_NoExeSpecified);
		return SE_OpenReadError;
	}
	if(UpperCase(ExtractFileExt(FZipFileName)) != ".EXE")
	{
		ShowZipMessage(CZ_InputNotExe);
		return SE_OpenReadError;
	}
  // The FSFXOffset is the location where the zip file starts inside
	// the .EXE archive.  It is calculated during a ZipBuilder List operation.
	// Since a LIST is done when a filename is assigned, we know that
	// a LIST has already been done on the correct archive.
	if(!FSFXOffset)
	{
		ShowZipMessage(CZ_SFXTypeUnknown);
		return Result;
	}
  FIntSFX->FSFXOffset = FSFXOffset;
  Result = FIntSFX->ConvertZIP();
  return Result;
}
// TZipWorker::ConvertZIP
#endif

#ifdef INTERNAL_SFX
// TZipWorker::ConvertSFX------------------------------------------------------
// 1.73( 30 July 2003) RA use TInternalSFX class
// 1.73( 21 June 2003) RA FSuccessCnt set to 0
int __fastcall TZipWorker::ConvertSFX(bool AutoExeViaAdd)
{
  const SE_GeneralError = -9;
  FShowProgress = zspNone;
  FSuccessCnt = 0;
  int Result = SE_GeneralError;
	if(!FileExists(FZipFileName) && !AutoExeViaAdd)
	{
		ShowZipMessage( GE_NoZipSpecified );
		return Result;
	}
  StartWaitCursor();
  Result = FIntSFX->ConvertSFX(AutoExeViaAdd);
  StopWaitCursor();
  return Result;
}
//---------------------------------------------------------------------------
// Convert an .ZIP archive to a .EXE archive.
// Returns 0 if good, or else a negative error code.
// Because we want backward compatible we keep this function.
int __fastcall TZipWorker::ConvertSFX(void)
{
	// Call the new ConvertSFX() function.
	return ConvertSFX(false);
}
#endif

#ifndef NO_SFX
// IsZipSFX---------------------------------------------------------------------
/* Return value:
	0 = The specified file is not a SFX
	1 = It is one
*/
int __fastcall TZipWorker::IsZipSFX(const String SFXExeName)
{
 int r = QueryZip(SFXExeName);  // SFX = 1 + 128 + 64
 int Result = 0;
 if((r & (1 | 128 | 64)) == (1 | 128 | 64))
    Result = 1;
 return Result;
}
// IsZipSFX
#endif

/*? TZipWorker::ExtMsg----------------------------------------------------------
1.77 27 July 2004 RP New function
*/
AnsiString __fastcall TZipWorker::ExtMsg(AnsiString CMsg, CallBack::PZCallBackStruct cbr,
                                        unsigned XIdent, int XPreamble)
{
  if(CMsg == "")
  {
    if(cbr->Zero == 0) CMsg = cbr->FName;
    else CMsg = cbr->FileNameOrMsg;
    CMsg = SetSlash(CMsg, psdExternal);
    if(XIdent != 0)
    {
      AnsiString xm = ZipLoadStr(XIdent);
      if(xm != "")
        CMsg = xm + " " + CMsg.SubString(XPreamble, CMsg.Length() - XPreamble);
    }
  }
  return CMsg;
}
//? TZipWorker::ExtMsg

/* TZipWorker::CloseFileHandle-------------------------------------------------------
1.80 25 February 2004 RA renamed was CloseFile
1.80 28 August 2003 RA new function for closing file handles
*/
void __fastcall TZipWorker::CloseFileHandle(int &FileHandle)
{
  if(FileHandle != -1)
  {
    FileClose(FileHandle);
    FileHandle = -1;
  }
}
// TZipWorker::CloseFile

/*? TZipWorker::IsPathOnly--------------------------------------------------------
1.75.1.1 20 March 2004 RA fix IsPathOnly
1.75 RP new function
*/
bool __fastcall TZipWorker::IsPathOnly(const String f)
{
  if(f != "")
  {
    char c = f[f.Length()];
    if((c == '/') || (c=='\\')) return true;
  }
  return false;
}
//? TZipbuidler::IsPathOnly

// TZipBuidler::Setup_extend-------------------------------------------------------
// 1.73.3.2 10 Oct 2003 RA New function to init upper table
void __fastcall TZipWorker::Setup_extend(void)
{
  unsigned char* p = (unsigned char *)extend;
  for(int i = 0;i < 256; ++i)
     *p++ = (unsigned char)i;
  CharToOemBuff(extend+1,(char *)extend+1,255);
  p = (unsigned char *)extend;
  for(int i = 0; i < 256; ++i)
       *p = *p++ != i;
}
//TZipBuidler::Setup_extend

// TZipWorker::HasExtendedChars ------------------------------------------------
// 1.73.3.2 8 Oct 2003 RA New function to test if file name has extended chars
bool __fastcall TZipWorker::HasExtendedChars(const AnsiString FileName)
{
  for(int i = 1; i < FileName.Length(); ++i)
    if(extend[static_cast<unsigned char>(FileName[i])]) return true;
  return false;
}
// TZipWorker::HasExtendedChars

//------------------------------------------------------------------------------
void   __fastcall TZipWorker::RemoveDriveSpec(AnsiString& FullPathName)
{
  if(ExtractFileDrive(FullPathName) != "")
  {
    int position = FullPathName.Pos("\\");
    FullPathName = FullPathName.SubString(position + 1, FullPathName.Length() - position);
  }
}







