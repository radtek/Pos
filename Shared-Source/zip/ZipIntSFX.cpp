/*
  ZipIntSFX.cpp - Source file of internal SFX unit for TZipBuilder
                  Code is derived from TZipBuilderSFX component
  TZipBuilder VCL by Chris Vleghert and Eric W. Engler
  v1.79
  Copyright (C) 2007  Roger Aelbrecht

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License (licence.txt) for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  contact: Zipbuilder AT drehoeksw DOT net
  DelphiZip maillist subscribe at http://www.geocities.com/SiliconValley/Orchard/8607/
  updates: http://www.drehoeksw.net

 last modified 2008-11-11
---------------------------------------------------------------------------*/
#include <vcl\vcl.h>
#pragma hdrstop
#if (__BORLANDC__ < 0x0550)
#include <shellapi.h>     // needed for BCB4
#endif
#include "ZipIntSFX.h"
#include "ZipBuilder.h"
#include "ZipMsg.h"
#include "ZBCompat.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#if (__BORLANDC__ >= 0x0560)
#define STRMSIZ __int64
#else
#define STRMSIZ int
#endif

#ifndef NO_SFX

const String RESOURCE_ERROR = "ZipMsgxx.res is probably not linked to the executable\nMissing String ID is: ";
const char SfxType_DZ[] = "DZSFX";
const char SfxType_BCB[] = "ZBSFX";
const char SFX_RELEASE[] = "3.3.1.0 November 28,2005";
//--  const ---------------------------------------------------
const int DZRES_SFX = 11611;  // stored UPX SFX stubs
const int SFX_HEADER_SIG = 0x1005a44;
const int SFX_HEADER_END_SIG = 0x10a5a44;
const int SFX_DETACHED_HEADER_SIG = 0x01145A44;
const int SFX_DETACHED_HEADER_END_SIG = 0x02145A44;
const int ZipLocalHeaderSig = 0x4034b50;
const int ZipCentralHeaderSig = 0x2014b50;
const int ZipEndOfCentralSig = 0x6054b50;
const int ZipExtLocalHeaderSig = 0x8074b50;
const Word SFXBufSize = 0x2000;
const int SE_CreateError = -1;
const int SE_CopyError = -2;
const int SE_OpenReadError = -3;
const int SE_SetDateError = -4;
const int SE_ZipBuilderError = -5;
const int SE_GeneralError = -9;
const int SE_NoFoundError = -10;
const int SE_OutOfMemError = -10;
const int SE_MemStreamError = -11;
const int SE_IconSizeError = -12;
const int SE_NoSpanningSupport = -101;
const int SE_ErrorInCentral = -102;

//----------------------------------------------------------------------------
//added version 3.1.0.0 callback function for EnumResourceNames
bool CALLBACK ResEnumFunc(HANDLE Handle, TCHAR* /*Type*/, TCHAR* Name, TStrings* lParam)
{
    String str1;
    TCHAR Buf[255];
    AnsiString SfxType;
    // if name has value smaller than 100000 it is not a named resource
    String str = (reinterpret_cast<int>(Name) > 100000) ? UpperCase(Name) : String("");

    // determine sfxtype for this resource
    if (str.Pos(SfxType_BCB) == 1)
        SfxType = SfxType_BCB;
    if (str.Pos(SfxType_DZ) == 1)
        SfxType = SfxType_DZ;
    if (!SfxType.IsEmpty()) // it is an sfxtype
    {
        if (str.Pos("_BIN_LANG\0") > 0)
        {
            str1 = "";
            // found resource, now read language id
            if (HANDLE h1 = FindResource(Handle, Name, RT_RCDATA))
            {
                if (HANDLE h2 = LoadResource(Handle, h1))
                {
                    if (char* ptr = static_cast<char*>(LockResource(h2)))
                    {
                        // get local info
                        GetLocaleInfo(MAKELCID(*ptr, SORT_DEFAULT), LOCALE_SNATIVELANGNAME, Buf, sizeof(Buf));
                        str1 = Buf;
                        UnlockResource(h2);
                    }
                    FreeResource(h2);
                }
            }
            str = str.SubString(str.Pos(SfxType) + SfxType.Length(), MaxInt);
            str.Delete(str.Pos("_BIN_LANG"), MaxInt);
            str1 = (str1.IsEmpty()) ? str + "=" + str.UpperCase() :
                   str1 + "=" + str.UpperCase();
            if (lParam->IndexOf(str1) == -1)
                lParam->Add(str1);
        }
    }
    return true;
}
//-----------------------------------------------------------------------------------
// Browse through all resource Directories.
pIRDatE __fastcall TZipIntSFX::BrowseResDir(pIRD ResStart, pIRD dir, int Depth)
{
    pIRDirE SingleRes = (pIRDirE)((char *)dir + sizeof(IMAGE_RESOURCE_DIRECTORY));
    pIRDatE Result = 0;

    for (int i = 0; i < dir->NumberOfNamedEntries + dir->NumberOfIdEntries; i++)
    {
        if ((Result = SearchResDirEntry(ResStart, SingleRes, Depth)) != 0)
            break;   // Found the one w're looking for.
        ++SingleRes;
    }
    return Result;
}

/*?TZipIntSFX::CheckZipBuilder---------------------------------------------
3.2.3 18 Nov 2004 RA If used as internal sfx
3.2.0.2 16 July 2004 added testing if zipbuilder busy
returns 0 : OK
        1 : no zipbuilder assigned
        2 : zipbuilder assigned and busy
*/
int __fastcall TZipIntSFX::CheckZipBuilder(void)
// returns true if FZipBuilder is present else false
{
    // need ZipBuilder component for messages, properties etc..
    TZipBuilder* FZipBuilder = static_cast<TZipBuilder*>(FBuilder);
    if (!FZipBuilder)
    {
        String Messg = "No TZipBuilder component assigned";
        Application->MessageBox(Messg.T_STR(), Application->Title.T_STR(), MB_OK | MB_ICONERROR);
        return 1;
    }
    return 0;
}
//?TZipIntSFX::CheckZipBuilder

#ifndef NO_SPAN
/*?TZipIntSFX::ConvertToSpanSFX----------------------------------------------
3.2.3 1 Aug 2005 RA corrected EOC content in detached header
3.2.3 24 Nov 2004 RA if used as InternalSFX OutFilePath is full destination filespec
3.2.3 20 Nov 2004 RA allow spannedsfx in ZipBuilder when used as internal sfx
3.2.1.2 21 July 2004 RA original value of ZipBuilder->KeepFreeOnDisk1 saved and restored
3.2.1.2 20 July 2004 RA corrected writing of CentralHeader to file
3.2.0.1 8 April 2004 RA changed to use CentralHeader and EOC as fileslist
added version 3.0 to support diskspanning
starts from archive on harddisk and creates a SFX with disk spanning
supports "spanned" disk on removable disks and "multi-volume" on non-removable
*/
int __fastcall TZipIntSFX::ConvertToSpanSFX(String OutFilePath)
{
    // need ZipBuilder for properties messages and actual sapnning functions etc..
    if (CheckZipBuilder() > 0)
        return SE_ZipBuilderError;
    TZipBuilder* FZipBuilder = static_cast<TZipBuilder*>(FBuilder);
    // now we create a temp sfx file name
    String TempDir;
    if (!FZipBuilder->TempDir.IsEmpty())
        TempDir = FZipBuilder->AppendSlash(FZipBuilder->TempDir);
    // outfilepath is the destination FZipFileName the source file
    String TempFile = TempDir + ExtractFileName(OutFilePath);
    // first we make sure we have a sfx bin for spanning

    int Result;
    Detached = true;
    if ((Result = DoConvertOrCreateSFX(TempFile, true)) != 0)
        return Result;
    // we need to execute List again
    FZipBuilder->FZip->List();
    // calulate the size of the sfx stub
    // internals sfx as full file spec in outfilepath
    String FPartFileName = ChangeFileExt(OutFilePath, ".zip");
    TFileStream* OutFile = new TFileStream(TempFile, fmOpenReadWrite);
    String FDrive = ExtractFileDrive(ExpandFileName(FPartFileName)) + "\\";
    unsigned int drt = GetDriveType(FDrive.T_STR());
    // we check the type of spanning used and set the Sig accordingly
    SpanSfxType Sig = stSpanSfx;
    if (drt == DRIVE_FIXED || drt == DRIVE_REMOTE || drt == DRIVE_RAMDISK ||
            FZipBuilder->SpanOptions.Contains(spNoVolumeName))
        Sig = stMVSfx; // set multi volume sig to alert the reader
    unsigned OrgKeepFree = FZipBuilder->KeepFreeOnDisk1; // save original value we will change it
    try
    {
        // we position at the end
        OutFile->Seek(0, soFromEnd);
        // prepare the header and append to file
        TSFXDetachedHeader recDet;
        AnsiString DetName = AnsiString(ChangeFileExt(ExtractFileName(FPartFileName), ""));
        if ((Sig == stMVSfx) && FUseDelphiBin)
            DetName += "001"; // add a part nummer to detached name for delphi SFXstub
        // stub will remove 3 chars when creating part numbers
        String DetExt = ExtractFileExt(FPartFileName);
        if (!DetExt.IsEmpty() && DetExt[1] == '.')
            DetExt.Delete(1, 1);
        recDet.Signature = SFX_DETACHED_HEADER_SIG;
        recDet.NameLen = DetName.Length();
        recDet.ExtLen = DetExt.Length();
        OutFile->Write(&recDet, sizeof(recDet));
        if (!DetName.IsEmpty())
            OutFile->Write(DetName.c_str(), DetName.Length());
        if (!DetExt.IsEmpty())
            OutFile->Write(DetExt.c_str(), DetExt.Length());
        // write an end header for backward searching
        recDet.Signature = SFX_DETACHED_HEADER_END_SIG;
        OutFile->Write(&recDet, sizeof(recDet));
        STRMSIZ FileListSize = OutFile->Size;
        ZipDirEntry* DirEntry;
        // add the size of the filelist first by reading central directory
        for (int i = 0; i < FZipBuilder->Count; ++i)
        {
            DirEntry = FZipBuilder->DirEntry[i];
            // we need for each file the ZipCentralHeader + filenamelength
            FileListSize += sizeof(ZipCentralHeader) + DirEntry->FileNameLength;
        }
        FileListSize += sizeof(int) + sizeof(ZipEndOfCentral); // number of entries + sig
        // reserve this space extra on first disk
        int Keepfree =  static_cast<int>(FZipBuilder->KeepFreeOnDisk1 + FileListSize);
        DWORD SectorsPCluster, BytesPSector, FreeClusters, TotalClusters;
        if (GetDiskFreeSpace(FDrive.T_STR(), &SectorsPCluster, &BytesPSector, &FreeClusters, &TotalClusters))
        {
            if (Keepfree % BytesPSector)
                FZipBuilder->FZip->KeepFreeOnDisk1 = ((Keepfree / BytesPSector) + 1) * BytesPSector;
            else
                FZipBuilder->FZip->KeepFreeOnDisk1 = Keepfree;
        }
        // let ZipBuilder do the spanning
        try
        {
            Result = FZipBuilder->FZip->WriteSpan(FZipBuilder->ZipFileName, FPartFileName, true);
        }
        __finally
        {
            if (Result != 0)
            {
                delete OutFile;
                DeleteFile(TempFile); // remove the useless/incomplete sfx stub
                FZipBuilder->KeepFreeOnDisk1 = OrgKeepFree; // restore original value
//                return Result; // writespan failed
            }
        }
        if (Result)
            return Result; // writespan failed

        // find the last part and read the EOC
        String SourceFile = FZipBuilder->ZipFileName;
        ZipEndOfCentral EOC;
        if (drt == DRIVE_FIXED || drt == DRIVE_REMOTE || drt == DRIVE_RAMDISK)
            FPartFileName = GetLastVolume(FPartFileName, EOC);
        // if original file is on same path as resulting files then change name of original file
        if (FZipBuilder->ZipFileName == FPartFileName)
            RenameFile(FPartFileName, ChangeFileExt(FPartFileName, ".spn"));
        // this will execute List() to get the centraldir
        FZipBuilder->FZip->ZipFileName = FPartFileName;
        // now that all information is collected we can set the original filename again
        RenameFile(ChangeFileExt(FPartFileName, ".spn"), FPartFileName);
        // write the central headers
        int CEHSig = CentralFileHeaderSig;
        STRMSIZ CEHOffset = OutFile->Position;
        for (int i = 0; i < FZipBuilder->Count; ++i)
        {
            DirEntry = FZipBuilder->DirEntry[i];
            OutFile->Write(&CEHSig, sizeof(CentralFileHeaderSig));
            // write the Header
            OutFile->Write(DirEntry, sizeof(ZipCentralHeader) - sizeof(CentralFileHeaderSig));
            // and the filename
            OutFile->Write(DirEntry->FileName.c_str(), DirEntry->FileNameLength);
        }
        // read the EOC from the original zip file
        int InFile = FileOpen(SourceFile, fmOpenRead | fmShareDenyWrite);
        ReadEOC(InFile, EOC);
        FileClose(InFile);
        // correct offset and size in the EOC
        EOC.CentralOffset =  static_cast<unsigned>(CEHOffset);
        EOC.CentralSize = static_cast<unsigned>(OutFile->Position - CEHOffset);
        EOC.ZipCommentLen = 0; // comment is stripped
        // wite the EOC
        OutFile->Write(&EOC, sizeof(ZipEndOfCentral));
        // and copy the tempfile to its destination
        if (!(drt == DRIVE_FIXED || drt == DRIVE_REMOTE || drt == DRIVE_RAMDISK))
        { // for removable disk we need to insert the first again
            bool RightDiskInserted = false;
            do
            {   // ask to insert the first disk
                String MsgStr = ZipFmtLoadStr(DS_InsertAVolume, ARRAYOFCONST((1))) +
                                    ZipFmtLoadStr(DS_InDrive, ARRAYOFCONST((FDrive)));
                if (MsgStr.IsEmpty())
                    MsgStr.printf(TEXT("Please insert disk volume %.d in drive %s"), 1, FDrive.T_STR());
                Application->MessageBox(MsgStr.T_STR(), Application->Title.T_STR(), MB_OK | MB_ICONINFORMATION);
                // check if right disk is inserted
                if (Sig == stSpanSfx)
                {
                    unsigned long Dummy1, Dummy2, DiskSerial;
                    TCHAR VolName[MAX_PATH];
                    GetVolumeInformation(FDrive.T_STR(), VolName, MAX_PATH, &DiskSerial, &Dummy1, &Dummy2, NULL, 0);
                    if (StrComp(VolName, TEXT("PKBACK# 001")) == 0)
                        RightDiskInserted = true;
                }
                else
                {
                    String DiskFile = FPartFileName.SubString(1, FPartFileName.Length()
                                          - ExtractFileExt(FPartFileName).Length()) + "001.zip";
                    if (FileExists(DiskFile))
                        RightDiskInserted = true;
                }
            }
            while (!RightDiskInserted);
        }
    }
    __finally
    {
        FZipBuilder->KeepFreeOnDisk1 = OrgKeepFree; // restore original value
        delete OutFile;  // close the file
        // finally copy the exe file to the destination if needed
        if (ExtractFilePath(OutFilePath) != ExtractFilePath(TempFile))
        {
            FZipBuilder->FZip->Copy_File(TempFile, OutFilePath);
            // and remove the temp file
            DeleteFile(TempFile);
        }
    }
    return Result;
}
//?TZipIntSFX::ConvertToSpanSFX
#endif

/*?TZipIntSFX::ConvertToSFX------------------------------------------------
3.2.3 18 Nov 2004 RA if used as InternalSFx then error raises exception and do not display message
3.2.1.2 20 July 2004 RA added detached = false;
  Converts the ZIP archive (according to the sfx related options of the ZipBuilder component)
   to a SFX archive and updates the ZipBuilder component to use the newly created file.
*/
int __fastcall TZipIntSFX::ConvertToSFX(void)
{
    Detached = false;
    // need ZipBuilder for properties messages etc..
    if (CheckZipBuilder() > 0)
        return SE_ZipBuilderError;
    return DoConvertOrCreateSFX(static_cast<TZipBuilder*>(FBuilder)->ZipFileName, false);
}
//?TZipIntSFX::ConvertToSFX

/*?TZipIntSFX::ConvertToZip------------------------------------------------
3.2.3 18 Nov 2004 RA if used as InternalSFX then error raises exception and do not display message
3.2.1.2 20 July 2004 RA added detached = false;
30 March 2004 RA allow SFX with bad length as query result
 Converts the SFX archive (in the ZipBuilder property's ZipFileName) to a ZIP archive*/

int __fastcall TZipIntSFX::ConvertToZip(void)
{
    int InFile, OutFile, InSize = -1, OutSize = -1, Result = SE_GeneralError;
    __int64 PosofEOC, Delta, SFXSize;
    ZipEndOfCentral EOC;
    Detached = false;
    //zipbuilder must exist;
    if (CheckZipBuilder() > 0)
        return SE_ZipBuilderError;
    // get sfx options from zipbuilder
    GetPropertiesFromZipBuilder();
    //file must exist
    TZipBuilder* FZipBuilder = static_cast<TZipBuilder*>(FBuilder);
    if (!FileExists(FZipBuilder->ZipFileName))
        throw EZipBuilder(CZ_NoExeSpecified);
    int QueryRes = QueryZip(FZipBuilder->ZipFileName);
    if (!(QueryRes == 241 || QueryRes == 225))// test if it is an SFX 241 is SFX 225 is SFX with bad length
        throw EZipBuilder(CZ_InputNotExe);
    // rename to zip
    String OutFileName = ChangeFileExt(FZipBuilder->ZipFileName, ".zip");
    TCursor SaveCursor = Screen->Cursor;
    Screen->Cursor = crHourGlass;
    try
    {
        // open in file
        if ((InFile = FileOpen(FZipBuilder->ZipFileName, fmOpenRead | fmShareDenyWrite)) == -1)
            Result = SE_OpenReadError;
        else
        {
            //read the EOC
            if ((PosofEOC = ReadEOC(InFile, EOC)) == 0)
                Result = SE_ErrorInCentral;
            else
            {
                //calculate delta ( if wrong offsets are stored in zip-file
                Delta = PosofEOC - (EOC.CentralOffset + EOC.CentralSize);
                //now find the first local header
                if ((SFXSize = GetFirstLocalHeader(InFile, EOC.CentralEntries,
                                                   static_cast<int>(EOC.ThisDiskNo),
                                                   static_cast<int>(EOC.CentralOffset + Delta))) < 0)
                    Result = SE_ErrorInCentral; //no local file header found
                else
                {
                    SFXSize += Delta;
                    //delete an existing output file
                    if (FileExists(OutFileName))
                        EraseFile(OutFileName, FZipBuilder->HowToDelete);
                    // open output file
                    if ((OutFile = FileCreate(OutFileName)) == -1)
                        Result = SE_CreateError;
                    else
                    {
                        FileSeek(InFile, SFXSize, soFromBeginning);
                        // write local data
                        Result = CopyBuffer(InFile, OutFile,
                                            static_cast<int>(EOC.CentralOffset - SFXSize + Delta));
                        if (!Result)
                        {
                            if ((Result = CopyAndAdjustCentralDir(InFile, OutFile,
                                                                  EOC.CentralEntries,
                                                                  static_cast<int>(EOC.ThisDiskNo),
                                                                  static_cast<int>(Delta - SFXSize))) == 0)
                                OutSize = FileSeek(OutFile, 0, soFromEnd);
                        }
                        FileClose(OutFile);
                    }
                }
            }
            if (!Result)
            {
                // read headers etc from input file
                if ((Result = ReadSFXProperties(InFile, static_cast<int>(SFXSize))) == 0)
                {
                    // get the icon from the sfx
                    ReadSFXIcon(FZipBuilder->ZipFileName);
                    InSize = FileSeek(InFile, 0, soFromEnd);
                }
            }
            FileClose(InFile);
        }
        //extra check
        if (!Result && (SFXSize == -1 || OutSize == -1 || InSize == -1))
            Result = SE_GeneralError;
        if (!Result)
            EraseFile(FZipBuilder->ZipFileName, FZipBuilder->HowToDelete);
        else
            if (FileExists(OutFileName))
                DeleteFile(OutFileName);
    }
    __finally
    {
        Screen->Cursor = SaveCursor;
    }
    return Result;
}
//?TZipIntSFX::ConvertToZip

//---------------------------------------------------------------------------
// copy and adjust central dirs and end of central dir
int __fastcall TZipIntSFX::CopyAndAdjustCentralDir(const int InFile, const int OutFile,
        const int NumEntries, const int DiskToAdjust,
        const int Offset)
{
    ZipCentralHeader CEH;
    ZipEndOfCentral EOC;
    int Result = SE_ErrorInCentral;
    char* Buf = new char[65536 * 3]; // filename + extrafield + filecomment
    try
    {
        for (int Cur = 0; Cur < NumEntries; Cur++)
        { // write all central directory entries
            if (FileRead(InFile, &CEH, sizeof(CEH)) != static_cast<int>(sizeof(CEH)) || CEH.HeaderSig != CentralFileHeaderSig)
                return Result;  //invalid record
            if (CEH.DiskStart == DiskToAdjust)
                CEH.RelOffLocal += Offset;
            if (FileWrite(OutFile, &CEH, sizeof(CEH)) != static_cast<int>(sizeof(CEH)))
                return SE_CopyError;
            //copy extra data
            if (CopyBuffer(InFile, OutFile, CEH.FileNameLen + CEH.ExtraLen + CEH.FileComLen))
                return SE_CopyError;
        }
        // read, adjust and write end of central
        if (FileRead(InFile, &EOC, sizeof(EOC)) != static_cast<int>(sizeof(EOC)) || EOC.HeaderSig != EndCentralDirSig)
            return Result;
        EOC.CentralOffset += Offset;
        if (FileWrite(OutFile, &EOC, sizeof(EOC)) != static_cast<int>(sizeof(EOC)))
            return SE_CopyError;
        // copy eventually existing comment
        if (CopyBuffer(InFile, OutFile, EOC.ZipCommentLen))
            return SE_CopyError;
        else
            Result = 0;
    }
    __finally
    {
        delete[] Buf;
    }
    return Result;
}

//---------------------------------------------------------------------------
// copy a buffer from a file to another, returns 0 on ok
// iReadLen: amount of data to copy, if -1, then up to eof
int __fastcall TZipIntSFX::CopyBuffer(const int InFile, const int OutFile,
                                      int ReadLen)
{
    int  SizeR, ToRead = SFXBufSize, Result = SE_CopyError;
    char* Buffer = NULL;
    try
    {
        Buffer = new char[SFXBufSize];
        Result = 0;     // No memory error!
        do
        {
            if (ReadLen >= 0)
                ToRead = min(ReadLen, static_cast<int>(SFXBufSize));
            SizeR = FileRead(InFile, Buffer, ToRead);
            if (FileWrite(OutFile, Buffer, SizeR) != SizeR)
            {
                Result = SE_CopyError;
                break;
            }
            if (ReadLen > 0)
                ReadLen -= SizeR;
            Application->ProcessMessages(); // Mostly for winsock.
        }
        while (SizeR == ToRead && ReadLen);
    }
    __finally
    {
        delete[] Buffer;
    }
    return Result;
}

/*?TZipIntSFX::CreateNewSFX------------------------------------------------
3.2.3 18 Nov 2004 RA if used as InternalSFX then error raises exception and do not display message
3.2.1.2 20 July 2004 RA added detached = false;
  Creates a new, empty SFX archive (according to the sfx related options of the
  ZipBuilder component) and updates the ZipBuilder component to use this newly created file.*/
int __fastcall TZipIntSFX::CreateNewSFX(const String& File)
{
    Detached = false;
    // need ZipBuilder
    if (CheckZipBuilder() > 0)
        return SE_ZipBuilderError;
    // file must be an EXE
    if (UpperCase(ExtractFileExt(File)) != ".EXE")
    {
        throw EZipBuilder(CZ_InputNotExe);
    }
    else
        return DoConvertOrCreateSFX(File, true);
}
//?TZipIntSFX::CreateNewSFX

/*?TZipIntSFX::CreateSFXBinStream------------------------------------------
3.2.3 18 NOv 2004 RA zipbuilder error message usage if stub b-noit found
added version 3.1.0.0 function to create SFXBinStream from resource or from file
returns false if error*/
bool __fastcall TZipIntSFX::CreateSFXBinStream(void)
{
    // what type of bin will be used
    String SfxType = FUseDelphiBin ? SfxType_DZ : SfxType_BCB;
    SFXBinStream = new TMemoryStream();
    // load it either from resource (if bcsfx##.res has been linked to the executable)
    // or by loading from file in SFXPath and check both versions if available
    ResVersion = "";
    bool ResExist =  LoadFromResource(SfxType);
    // look of file exists from file
    bool BinExists =  LookForBinFile(SfxType);
    // decide which will be used
    if (BinExists && ResExist)
    {
        if (ResVersion.IsEmpty() || (BinVersion.COMPARE(ResVersion) > 0))
        {    // if res has no version or bin version is larger (younger) use bin
            ResExist = false;
            SFXBinStream->Clear();
        }
    }
    // not found in exe image or bin is younger, so try to load it from file
    if (!ResExist && !BinExists)
    {
        String BinName = SfxType + FSFXLanguage + ".bin";
        throw EZipBuilder(SF_NoZipSFXBin, ARRAYOFCONST((BinName)));
    }
    if (!ResExist)
    {
        SFXBinStream->LoadFromFile(FSFXPath);
        SFXBinStream->Seek(0, soFromBeginning);
    }
    return true;
}
//?TZipIntSFX::CreateSFXBinStream

/*?TZipIntSFX::DoConvertOrCreateSFX----------------------------------------
3.2.3 18 Nov 2004 RA if used as InternalSFx then error raises exception and do not display message
17 May 2004 Corrected code for writing EOC to file
10 May 2004 Changed tests on string lengths and used error messages
1 April2004 Ra allow zip with bad comment length
*/
int __fastcall TZipIntSFX::DoConvertOrCreateSFX(const String& File, const bool CreateNew)
{
    String OutFileName;
    int InFile, OutFile, Result = SE_GeneralError;
    int SFXSize = -1, OutSize = -1, SFXBlkSize;
    __int64 ZipSize = -1;
    char* sfxblk;
    ZipEndOfCentral EOC;

    GetPropertiesFromZipBuilder();
    TZipBuilder* FZipBuilder = static_cast<TZipBuilder*>(FBuilder);
    if (!CreateNew)
    {
        // file must exist
        if (!FileExists(File))
            throw EZipBuilder(GE_NoZipSpecified);
        //file must be a zip
        int QueryRes = QueryZip(File);
        if (QueryRes != 242 && QueryRes != 226)// test if it is a zip 242 is zip 226 is zip with bad length
            throw EZipBuilder(SF_InputIsNoZip);
        //rename to exe
        OutFileName = ChangeFileExt(File, ".exe");
    }
    else
        OutFileName = File;
    // try to find fsfxpath
    // remove file name if given by user or remnant from previous call
    if (!ExtractFileExt(ExtractFileName(FSFXPath)).IsEmpty())
        FSFXPath = ExtractFilePath(FSFXPath);
    int PathLen = FSFXPath.Length();
    // If there is a trailing '\' in path, cut it off:
    if (PathLen && (FSFXPath[PathLen] == '\\'))
        FSFXPath.SetLength(PathLen - 1);
    if (!CreateSFXBinStream())
        return Result;
    //check string lengths
    if (FZipBuilder->SFXCaption.Length() > 255)
        throw EZipBuilder(SF_DlgTitleTooLong);
    if (FZipBuilder->SFXCommandLine.Length() > 255)
        throw EZipBuilder(SF_CmdLineTooLong);
    if (FZipBuilder->SFXDefaultDir.Length() > 255)
        throw EZipBuilder(SF_DefPathTooLong);
    if (FZipBuilder->SFXMessage.Length() > 255)
        throw EZipBuilder(SF_MsgTooLong);
    if (RegFailPath.Length() > 255)
        throw EZipBuilder(SF_FBkPathTooLong);
    //calculate overall headersize
    int L = FZipBuilder->SFXCaption.Length() + FZipBuilder->SFXDefaultDir.Length() +
            FZipBuilder->SFXCommandLine.Length() + FZipBuilder->SFXMessage.Length() +
            RegFailPath.Length();
    SFXBlkSize = ((sizeof(TSFXFileHeader) + L + 3) / 4) * 4; // DWord Alignment.
    sfxblk = new char[SFXBlkSize + 1];
    memset(sfxblk, 0, SFXBlkSize + 1); // fill all bytes with 0 first
    //save cursor and show wait
    TCursor SaveCursor = Screen->Cursor;
    Screen->Cursor = crHourGlass;
    try
    {
        // create header
        TSFXFileHeader SFXHead;
        SFXHead.Signature = SFX_HEADER_SIG;
        SFXHead.Size = static_cast<Word>(SFXBlkSize);
        SFXHead.Options = FMySFXOptions;
        SFXHead.DefOVW = FMySFXOverWriteMode;
        SFXHead.CaptionSize = static_cast<Byte>(FZipBuilder->SFXCaption.Length());
        SFXHead.PathSize = static_cast<Byte>(FZipBuilder->SFXDefaultDir.Length());
        SFXHead.CmdLineSize = static_cast<Byte>(FZipBuilder->SFXCommandLine.Length());
        SFXHead.RegFailPathSize = static_cast<Byte>(RegFailPath.Length());
        SFXHead.StartMsgSize = static_cast<Byte>(FMySFXMessage.Length());
        SFXHead.StartMsgType = FMySFXMessageFlags;
        // copy header to sfxblk
        memcpy(&sfxblk[0], &SFXHead, sizeof(TSFXFileHeader));
        //set end of fileheader
        TSFXFileEndOfHeader RecEnd;
        RecEnd.Signature = SFX_HEADER_END_SIG;
        RecEnd.HeaderSize = SFXBlkSize;
        // store strings
        int i = sizeof(TSFXFileHeader);
        if (FZipBuilder->SFXCaption.Length())
        {
            StrPCopy(sfxblk + i, AnsiString(FZipBuilder->SFXCaption));
            i += FZipBuilder->SFXCaption.Length();
        }
        if (FZipBuilder->SFXDefaultDir.Length())
        {
            StrPCopy(sfxblk + i, AnsiString(FZipBuilder->SFXDefaultDir));
            i += FZipBuilder->SFXDefaultDir.Length();
        }
        if (FZipBuilder->SFXCommandLine.Length())
        {
            StrPCopy(sfxblk + i, AnsiString(FZipBuilder->SFXCommandLine));
            i += FZipBuilder->SFXCommandLine.Length();
        }
        if (RegFailPath.Length())
        {
            StrPCopy(sfxblk + i, AnsiString(RegFailPath));
            i += RegFailPath.Length();
        }
        if (FMySFXMessage.Length())
            StrPCopy(sfxblk + i, AnsiString(FMySFXMessage));
        //deleting existing file
        if (FileExists(OutFileName))
            EraseFile(OutFileName, FZipBuilder->HowToDelete);
        Application->ProcessMessages(); // allow OS to effectively remove the file
        Sleep(20);
        // open and start writing
        if ((OutFile = FileCreate(OutFileName)) != -1)
        {
            // copy the SFX code from the memorystream in the outfile
            SFXSize =  static_cast<int>(SFXBinStream->Size);
            char* buff = new char[static_cast<int>(SFXBinStream->Size)];
            try
            {
                Result = 0; // OK
                SFXBinStream->ReadBuffer(buff, static_cast<int>(SFXBinStream->Size));
                if (FileWrite(OutFile, buff, SFXSize) != SFXSize)
                    Result = SE_CopyError;
            }
            __finally
            {
                delete[] buff;
                delete SFXBinStream;
            }
            if (!Result && !FZipBuilder->SFXIcon->Empty)
                Result = ReplaceIcon(OutFile, SFXSize, FZipBuilder->SFXIcon);
            // Copy the special SFX block to the destination.
            if (!Result)
            {
                if ((FileWrite(OutFile, sfxblk, SFXBlkSize) != SFXBlkSize) ||
                        (FileWrite(OutFile, &RecEnd, sizeof(RecEnd)) != static_cast<int>(sizeof(RecEnd))))
                    Result = SE_CopyError;
                else
                {
                    if (!CreateNew)
                    { //if conversion
                        if ((InFile = FileOpen(File, fmOpenRead | fmShareDenyWrite)) == -1)
                            Result = SE_OpenReadError;
                        else
                        {
                            //read end of central from file
                            __int64 PosofEOC;
                            if ((PosofEOC = ReadEOC(InFile, EOC)) < 0)
                                Result = SE_ErrorInCentral;
                            else
                            { //calculate delta (if wrong ofsets are stored in zip-file)
                                __int64 Delta = PosofEOC - (EOC.CentralOffset + EOC.CentralSize);
                                //currently , only nonspanned archives are supported
                                if ((EOC.ThisDiskNo != EOC.CentralDiskNo) ||
                                        (EOC.CentralEntries != EOC.TotalEntries))
                                    Result = SE_NoSpanningSupport;
                                else
                                { //write zipfile
                                    ZipSize = PosofEOC + sizeof(EOC) + EOC.ZipCommentLen;
                                    FileSeek(InFile, 0, soFromBeginning);
                                    //copy local data
                                    Result = CopyBuffer(InFile, OutFile,
                                                        static_cast<int>(EOC.CentralOffset + Delta));
                                    //copy and adjust central dir
                                    if (!Result)
                                        Result = CopyAndAdjustCentralDir(InFile, OutFile, EOC.CentralEntries,
                                                                         EOC.ThisDiskNo,
                                                                         static_cast<int>(Delta + SFXSize + SFXBlkSize + sizeof(RecEnd)));
                                    OutSize = FileSeek(OutFile, 0, soFromEnd);
                                }
                            }
                            FileClose(InFile);
                        }
                    }
                    else
                    { //create a new file
                        if (!Detached)
                        {
                            // fill in the EOC
                            memset(&EOC, '\0', sizeof(EOC));
                            EOC.HeaderSig = EndCentralDirSig;
                            EOC.CentralOffset = SFXSize + SFXBlkSize + sizeof(RecEnd);
                            //write it to the file
                            if (FileWrite(OutFile, &EOC, sizeof(EOC)) != static_cast<int>(sizeof(EOC)))
                                Result = SE_CopyError;
                            else
                                Result = 0;
                        }
                        else
                            Result = 0;
                        OutSize = FileSeek(OutFile, 0, soFromEnd);
                        ZipSize = 0;
                    }
                }
            }
            FileClose(OutFile);
        }
        else
            Result = SE_CreateError;
        //extra check
        if (!Result && (SFXSize == -1 || ZipSize == -1 || OutSize == -1))
            Result = SE_GeneralError;

        if (!Result && !CreateNew)
            EraseFile(File, FZipBuilder->HowToDelete);
        if (Result && FileExists(OutFileName))
            DeleteFile(OutFileName);
    }
    __finally
    {
        Screen->Cursor = SaveCursor;
        delete[] sfxblk;
    }
    return Result;
}
//?TZipIntSFX::DoConvertOrCreateSFX
//---------------------------------------------------------------------------
//// return the position of the first local file header in the file
// -1 on error, else not the absolute file position is read but the value from the central directory
int __fastcall TZipIntSFX::GetFirstLocalHeader(const int InFile, const int NumEntries,
        const int WhichDisk, const int Position)
{
    ZipCentralHeader CEH;
    int Result = SE_ErrorInCentral;
    if ((NumEntries < 1) || (FileSeek(InFile, Position, soFromBeginning) != Position))
        return Result;
    for (int i = 0; i < NumEntries; i++)
    {
        //read central dir
        if (FileRead(InFile, &CEH, sizeof(CEH)) != static_cast<int>(sizeof(CEH)) || CEH.HeaderSig != CentralFileHeaderSig)
            return SE_ErrorInCentral;
        // first ?
        if (((Result < 0) || ((signed)CEH.RelOffLocal < Result)) && (CEH.DiskStart == WhichDisk))
            Result = CEH.RelOffLocal; //store
        //read over extra data
        if (CEH.FileNameLen + CEH.ExtraLen + CEH.FileComLen)
            FileSeek(InFile, CEH.FileNameLen + CEH.ExtraLen + CEH.FileComLen, soFromCurrent);
    }
    return Result;
}

#ifndef NO_SPAN
/*?TZipIntSFX::GetLastVolume-------------------------------------------------
3.2.3 24 Nov 2004 RA added to find last volume of MV spanned sfx on hard disc
*/
String __fastcall TZipIntSFX::GetLastVolume(const String& FileName, ZipEndOfCentral& EOC)
{
    String Fname = FileName.SubString(1, FileName.Length() - 4);// remove extension
    // MV files are series with consecutive partnbrs in filename, highest number has EOC
    bool Finding = true;
    int PartNbr = -1;
    String s;
    while (Finding)
    {
        s = Fname + IntToStr(1002 + PartNbr).SubString(2, 3) + ".zip";
        if (!FileExists(s))
            break;
        ++PartNbr;
        int InFile = FileOpen(s, fmOpenRead | fmShareDenyWrite);
        if (ReadEOC(InFile, EOC) != -1)
        {
            Finding = false;
            if (EOC.ThisDiskNo != PartNbr)
                PartNbr = -1; // not valid last part
        }
        FileClose(InFile);
    }
    return s;
}
//?TZipBuidlerSFX::GetLastVolume
#endif

//---------------------------------------------------------------------------
void __fastcall TZipIntSFX::GetPropertiesFromZipBuilder(void)
{
    if (CheckZipBuilder() > 0)
        return;
    // use ZipBuilder properties, so convert them to our own properties
    TZipBuilder* FZipBuilder = static_cast<TZipBuilder*>(FBuilder);
    if (FZipBuilder)
    {
        // convert message
        FMySFXMessageFlags = MB_OK;
        String Msg = FZipBuilder->SFXMessage;
        if (!Msg.IsEmpty())
        {
            switch (Msg[1])
            {
            case 1:
                FMySFXMessageFlags = MB_OKCANCEL | MB_ICONINFORMATION;
                Msg.Delete(1, 1);
                break;
            case 2:
                FMySFXMessageFlags = MB_YESNO | MB_ICONQUESTION;
                Msg.Delete(1, 1);
                break;
            }
        }
        FMySFXMessage = Msg;
        // translate overwrite
        switch (FZipBuilder->SFXOverWriteMode)
        {
        case OvrConfirm:
            FMySFXOverWriteMode = somAsk;
            break;
        case OvrAlways:
            FMySFXOverWriteMode = somOverwrite;
            break;
        case OvrNever:
            FMySFXOverWriteMode = somSkip;
        }
        // translate options
        FMySFXOptions.Clear();
        if (FZipBuilder->SFXOptions.Contains(SFXAskCmdLine))
            FMySFXOptions = FMySFXOptions << soAskCmdLine;
        if (FZipBuilder->SFXOptions.Contains(SFXAskFiles))
            FMySFXOptions = FMySFXOptions << soAskFiles;
        if (FZipBuilder->SFXOptions.Contains(SFXAutoRun))
            FMySFXOptions = FMySFXOptions << soAutoRun;
        if (FZipBuilder->SFXOptions.Contains(SFXHideOverWriteBox))
            FMySFXOptions = FMySFXOptions << soHideOverWriteBox;
        if (FZipBuilder->SFXOptions.Contains(SFXNoSuccessMsg))
            FMySFXOptions = FMySFXOptions << soNoSuccessMsg;
        if (Detached)
            FMySFXOptions = FMySFXOptions << soDetached;
    }
    // add extra options not available in ZipBuilder
    if (SFXOptionsExt.Contains(soExpandVariables))
        FMySFXOptions = FMySFXOptions << soExpandVariables;
    if (SFXOptionsExt.Contains(soInitiallyHideFiles))
        FMySFXOptions = FMySFXOptions << soInitiallyHideFiles;
    if (SFXOptionsExt.Contains(soForceHideFiles))
        FMySFXOptions = FMySFXOptions << soForceHideFiles;
    if (SFXOptionsExt.Contains(soCheckAutoRunFileName))
        FMySFXOptions = FMySFXOptions << soCheckAutoRunFileName;
    if (SFXOptionsExt.Contains(soCanBeCancelled))
        FMySFXOptions = FMySFXOptions << soCanBeCancelled;
}

//---------------------------------------------------------------------------
// added version 3.1.0.0
String __fastcall TZipIntSFX::GetSFXLanguage(void)
{
    for (int i = 0; i < Included_Languages->Count; ++i)
    {
        if (Included_Languages->Values[Included_Languages->Names[i]] == FSFXLanguage)
            return Included_Languages->Names[i];
    }
    return FSFXLanguage.UpperCase();
}
//---------------------------------------------------------------------------
String __fastcall TZipIntSFX::GetVersion(void)
{
    return SFX_RELEASE;
}
//---------------------------------------------------------------------------
bool __fastcall TZipIntSFX::LoadFromResource(const String& SfxType)
{
    String str = SfxType + FSFXLanguage + "_BIN";
    void* inst = HInstance;
    HANDLE hFindSFX = FindResource(inst, str.T_STR() , RT_RCDATA);
    if ((hFindSFX == 0) && ModuleIsLib && MainInstance)
    {
        inst = MainInstance;
        hFindSFX = FindResource(inst, str.T_STR() , RT_RCDATA);
    }
    if (hFindSFX)
    {
        str = SfxType + FSFXLanguage + "_BIN_VERSION";
        HANDLE hFindVersion = FindResource(inst, str.T_STR(), RT_RCDATA);
        if (hFindVersion && (SizeofResource(inst, hFindVersion) >= sizeof(unsigned)))
        {     // load version from resource if available
            if (HANDLE hRes = LoadResource(inst, hFindVersion))
                if (void * Dataptr = LockResource(hRes))
                    ResVersion =  static_cast<char*>(Dataptr);
        }
        str = SfxType + FSFXLanguage + "_BIN_SIZE";
        HANDLE hFindSize = FindResource(inst, str.T_STR(), RT_RCDATA);
        if (hFindSize && (SizeofResource(inst, hFindSize) >= sizeof(unsigned)))
        {
            if (HANDLE hRes = LoadResource(inst, hFindSize))
            {
                if (void* Dataptr = LockResource(hRes))
                {
                    unsigned Size = *static_cast<unsigned*>(Dataptr);
                    if ((Size > 0) && (SizeofResource(inst, hFindSFX) >= Size))
                    {
                        if ((hRes = LoadResource(inst, hFindSFX)) != 0)
                        {
                            if ((Dataptr = LockResource(hRes)) != 0)
                            {
                                SFXBinStream->Write(Dataptr, Size);
                                SFXBinStream->Seek(0, soFromBeginning);
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}
//---------------------------------------------------------------------------
bool __fastcall TZipIntSFX::LookForBinFile(const String& SfxType)
{
    bool BinExists = true;
    TCHAR dirbuf[MAX_PATH];
    TZipBuilder* FZipBuilder = static_cast<TZipBuilder*>(FBuilder);
    do
    {
        if (FileExists(FSFXPath = FSFXPath + "\\" + SfxType + FSFXLanguage + ".bin"))
            break;
        // Try the current directory.
        if (FileExists(FSFXPath = SfxType + FSFXLanguage + ".bin"))
            break;
        // Try the application directory.
        if (FileExists(FSFXPath = ExtractFilePath(ParamStr(0)) + SfxType + FSFXLanguage + ".bin"))
            break;
        // Try the Windows System dir.
        ::GetSystemDirectory(dirbuf, MAX_PATH);
        if (FileExists(FSFXPath = String(dirbuf) + "\\" + SfxType + FSFXLanguage + ".bin"))
            break;
        // Try the Windows dir.
        ::GetWindowsDirectory(dirbuf, MAX_PATH);
        if (FileExists(FSFXPath = String(dirbuf) + "\\" + SfxType + FSFXLanguage + ".bin"))
            break;
        // Try the dir specified in the DLLDirectory property.
        if (!FZipBuilder->DLLDirectory.IsEmpty() &&
                FileExists(FSFXPath = FZipBuilder->DLLDirectory + "\\" + SfxType + FSFXLanguage + ".bin"))
            break;
        BinExists = false;   // bin not found
        break;
    }
    while (true);
    if (BinExists)
    {
        // get fileversion from the .Bin if available
        DWORD VersionHandle;
        int VersionSize = GetFileVersionInfoSize(SFXPath.T_STR(), & VersionHandle);
        if (VersionSize)
        {
            TCHAR* Buffer = new TCHAR[VersionSize];
            GetFileVersionInfo(SFXPath.T_STR(), VersionHandle, VersionSize, Buffer);
            TCHAR *buff;
            UINT buflen;
            TCHAR txtstr[100];
            TCHAR sbuff[30];
            struct LANGANDCODEPAGE
            {
                WORD wLanguage;
                WORD wCodePage;
            } *lpTranslate;
            UINT vsize;
            VerQueryValue(Buffer, TEXT("\\VarFileInfo\\Translation"), (LPVOID *)&lpTranslate, &vsize);
            wsprintf(sbuff, TEXT("\\StringFileInfo\\%04x%04x\\"), lpTranslate->wLanguage, lpTranslate->wCodePage);
#ifdef UNICODE
            if (VerQueryValue(Buffer, wcscat(wcscpy(txtstr, sbuff), TEXT("FileVersion")),
                              reinterpret_cast<void **>(&buff), &buflen))
                BinVersion = buff;
#else
            if (VerQueryValue(Buffer, strcat(strcpy(txtstr, sbuff), TEXT("FileVersion")),
                              reinterpret_cast<void **>(&buff), &buflen))
                BinVersion = buff;
#endif
            delete[] Buffer;
        }
    }
    return BinExists;
}
//----------------------------------------------------------------------------------------
// Find Resource directory entry.
pIRDatE __fastcall TZipIntSFX::LookForDirs(const void *SectionData,
        const DWord SectionVirtualStart,
        const int SectionLen, pIDD directories)
{
    pIRD  ResStart;
    pIRDatE Result = 0;
    pIDD  dirs = directories;

    dirs += IMAGE_DIRECTORY_ENTRY_RESOURCE;
    if (dirs->VirtualAddress && ((char *)dirs->VirtualAddress >= (char *)SectionVirtualStart) &&
            (char *)dirs->VirtualAddress < ((char *)SectionVirtualStart + SectionLen))
    {
        ResStart = (pIRD)((char *)SectionData + (dirs->VirtualAddress - SectionVirtualStart));
        Result = BrowseResDir(ResStart, ResStart, 0);
    }
    return Result;
}
//---------------------------------------------------------------------------
void __fastcall TZipIntSFX::Notification(Classes::TComponent* aComponent,
        Classes::TOperation Operation)
{
    TZipBuilder* FZipBuilder = static_cast<TZipBuilder*>(FBuilder);
    if (Operation == opRemove && aComponent == FZipBuilder)
        FBuilder = NULL;
}

//---------------------------------------------------------------------------
//3.2.3 RA bug removed when searching in archive with zipfilecomment
// read end of central record from iInFile and return its position
__int64 __fastcall TZipIntSFX::ReadEOC(const int InFile,
                                       ZipEndOfCentral &EOC)
{
    char *Buf;
    __int64 Result;
    bool OK = false;
    unsigned FileSize = FileSeek(InFile, 0, soFromEnd);
    int BufferSize = min(FileSize, 65535 + sizeof(EOC));
    // We do a check at the end of the file to speed things up if
    // there isn't a Zip archive comment.
    if ((Result = FileSeek(InFile, -static_cast<__int64>(sizeof(EOC)), soFromEnd)) != -1)
    {
        FileRead(InFile, &EOC, sizeof(EOC));
        if (EOC.HeaderSig != EndCentralDirSig)
        {  //not found at the end read in a buffer
            if (BufferSize > static_cast<int>(sizeof(EOC)))
            {
                if ((Result = FileSeek(InFile, -static_cast<__int64>(BufferSize), soFromEnd)) != -1)
                {
                    Buf = new char[BufferSize + 1];
                    try
                    {
                        if (FileRead(InFile, Buf, BufferSize) == BufferSize)
                        { //now search from the end towards the start of the buffer
                            for (int i = BufferSize - sizeof(EOC) - 1; i >= 0; i--)
                            {
                                ZipEndOfCentral* pEOC = reinterpret_cast<ZipEndOfCentral*>(Buf + i);
                                if (pEOC->HeaderSig == EndCentralDirSig)
                                {
                                    memcpy(&EOC, Buf + i, sizeof(EOC));
                                    Result += i;
                                    OK = true;
                                    break;
                                }
                            }
                        }
                    }
                    __finally
                    {
                        delete[] Buf;
                    }
                }
            }
        }
        else
            OK = true;
    }
    return OK ? Result : -1;
}

//---------------------------------------------------------------------------
int __fastcall TZipIntSFX::ReadNewSFXProperties(const int InFile, const int MaxPos)
{
    TSFXFileEndOfHeader FEOF;
    TSFXFileHeader  SFH;

    if (CheckZipBuilder() > 0)
        return SE_ZipBuilderError;
    if ((FileSeek(InFile, (int)(MaxPos - sizeof(FEOF)), soFromBeginning) == -1) ||
            (FileRead(InFile, &FEOF, sizeof(FEOF)) != static_cast<int>(sizeof(FEOF))) ||
            (FEOF.Signature != SFX_HEADER_END_SIG) ||
            (FileSeek(InFile, (int)(0 - sizeof(FEOF) - FEOF.HeaderSize), soFromCurrent) == -1) ||
            (FileRead(InFile, &SFH, sizeof(SFH)) != static_cast<int>(sizeof(SFH))) ||
            (SFH.Size != (int)(FEOF.HeaderSize)) ||
            (SFH.Signature != SFX_HEADER_SIG))
        return SE_GeneralError;
    // new sfx header found now parse it
    TZipBuilder* FZipBuilder = static_cast<TZipBuilder*>(FBuilder);
    switch (SFH.DefOVW)
    {
    case somOverwrite:
        FZipBuilder->SFXOverWriteMode = OvrConfirm;
        break;
    case somSkip:
        FZipBuilder->SFXOverWriteMode = OvrAlways ;
        break;
    default:
        FZipBuilder->SFXOverWriteMode = OvrConfirm;
    }
    FZipBuilder->SFXOptions.Clear();
    FZipBuilder->SFXOptions << SFXCheckSize; // compatibility
    if (SFH.Options.Contains(soAskCmdLine))
        FZipBuilder->SFXOptions = FZipBuilder->SFXOptions << SFXAskCmdLine;
    if (SFH.Options.Contains(soAskFiles))
        FZipBuilder->SFXOptions = FZipBuilder->SFXOptions << SFXAskFiles;
    if (SFH.Options.Contains(soAutoRun))
        FZipBuilder->SFXOptions = FZipBuilder->SFXOptions << SFXAutoRun;
    if (SFH.Options.Contains(soHideOverWriteBox))
        FZipBuilder->SFXOptions = FZipBuilder->SFXOptions << SFXHideOverWriteBox;
    if (SFH.Options.Contains(soNoSuccessMsg))
        FZipBuilder->SFXOptions = FZipBuilder->SFXOptions << SFXNoSuccessMsg;
    AnsiString Read;
    FZipBuilder->SFXCaption = "";
    if (SFH.CaptionSize)
    {
        Read.SetLength(SFH.CaptionSize);
        if ((FileRead(InFile, &Read[1], SFH.CaptionSize)) != SFH.CaptionSize)
            return SE_OpenReadError;
        FZipBuilder->SFXCaption = Read;
    }
    FZipBuilder->SFXDefaultDir = "";
    if (SFH.PathSize)
    {
        Read.SetLength(SFH.PathSize);
        if ((FileRead(InFile, &Read[1], SFH.PathSize)) != SFH.PathSize)
            return SE_OpenReadError;
        FZipBuilder->SFXDefaultDir = Read;
    }
    FZipBuilder->SFXCommandLine = "";
    if (SFH.CmdLineSize)
    {
        Read.SetLength(SFH.CmdLineSize);
        if ((FileRead(InFile, &Read[1], SFH.CmdLineSize)) != SFH.CmdLineSize)
            return SE_OpenReadError;
        FZipBuilder->SFXCommandLine = Read;
    }
    RegFailPath = "";
    if (SFH.RegFailPathSize)
    {
        Read.SetLength(SFH.RegFailPathSize);
        if ((FileRead(InFile, &Read[1], SFH.RegFailPathSize)) != SFH.RegFailPathSize)
            return SE_OpenReadError;
        RegFailPath = Read;
    }
    FZipBuilder->SFXMessage = "";
    if (SFH.StartMsgSize)
    {
        Read.SetLength(SFH.StartMsgSize);
        if ((FileRead(InFile, &Read[1], SFH.StartMsgSize)) != SFH.StartMsgSize)
            return SE_OpenReadError;
        FZipBuilder->SFXMessage = Read;
    }
    switch (SFH.StartMsgType)
    {
    case MB_OKCANCEL | MB_ICONINFORMATION:
        FZipBuilder->SFXMessage = "\1" + FZipBuilder->SFXMessage;
        break;
    case MB_YESNO | MB_ICONQUESTION:
        FZipBuilder->SFXMessage = "\2" + FZipBuilder->SFXMessage;
        break;
    }
    SFXOptionsExt.Clear();
    if (SFH.Options.Contains(soExpandVariables))
        SFXOptionsExt = SFXOptionsExt << soExpandVariables;
    if (SFH.Options.Contains(soInitiallyHideFiles))
        SFXOptionsExt = SFXOptionsExt << soInitiallyHideFiles;
    if (SFH.Options.Contains(soForceHideFiles))
        SFXOptionsExt = SFXOptionsExt << soForceHideFiles;
    if (SFH.Options.Contains(soCheckAutoRunFileName))
        SFXOptionsExt = SFXOptionsExt << soCheckAutoRunFileName;
    if (SFH.Options.Contains(soCanBeCancelled))
        SFXOptionsExt = SFXOptionsExt << soCanBeCancelled;
    return 0;
}

//---------------------------------------------------------------------------
int __fastcall TZipIntSFX::ReadSFXIcon(const String& File)
{
    if (CheckZipBuilder() > 0)
        return SE_ZipBuilderError;
    // read the 'custom' icon back from the executable
    HICON hico = ExtractIcon(HInstance, File.T_STR(), 0);
    if (hico && (hico != INVALID_HANDLE_VALUE))
    {
        TZipBuilder* FZipBuilder = static_cast<TZipBuilder*>(FBuilder);
        if (FZipBuilder->SFXIcon->Handle)
            FZipBuilder->SFXIcon->ReleaseHandle();
        FZipBuilder->SFXIcon->Handle = hico;
    }
    return 0;
}

//---------------------------------------------------------------------------
int __fastcall TZipIntSFX::ReadSFXProperties(const int InFile, const int MaxPos)
{
    int Result;
    if (CheckZipBuilder() > 0)
        return SE_ZipBuilderError;
    if ((Result = ReadNewSFXProperties(InFile, MaxPos)) != 0)
    {
        int Max = min(1032, MaxPos);
        char* Data = new char[Max + 1];
        try
        {
            // goto MaxPos-Max and read a complete buffer
            if ((FileSeek(InFile, MaxPos - Max, soFromBeginning) == -1) ||
                    (FileRead(InFile, Data, Max) != Max))
                Result = SE_OpenReadError;
            else
            {
                Result = 0; // OK, also if no header found( could be winzipsfx or something like this)
                // ok, blok read, now try to find MPV or MPU
                for (int i = 0; i <= Max - 3; i++)
                {
                    if ((Data[i] == 'M') && (Data[i + 1] == 'P') && ((Data[i + 2] == 'U') ||
                            (Data[i + 2] == 'V')))
                    {
                        // read back the orginal values from the MPU block
                        TZipBuilder* FZipBuilder = static_cast<TZipBuilder*>(FBuilder);
                        FZipBuilder->SFXOptions.Clear();
                        FZipBuilder->SFXOverWriteMode = OvrConfirm;
                        char cll = Data[i + 3];
                        if (cll & 1)
                            FZipBuilder->SFXOptions = FZipBuilder->SFXOptions << SFXAskCmdLine;
                        if (cll & 2)
                            FZipBuilder->SFXOptions = FZipBuilder->SFXOptions << SFXAskFiles;
                        if (cll & 4)
                            FZipBuilder->SFXOptions = FZipBuilder->SFXOptions << SFXHideOverWriteBox;
                        if (cll & 8)
                            FZipBuilder->SFXOverWriteMode = OvrAlways;
                        if (cll & 16)
                            FZipBuilder->SFXOverWriteMode = OvrNever;
                        if (!(cll & 32))
                            FZipBuilder->SFXOptions = FZipBuilder->SFXOptions << SFXCheckSize;
                        if (cll & 64)
                            FZipBuilder->SFXOptions = FZipBuilder->SFXOptions << SFXAutoRun;
                        if (Data[i + 2] == 'U')
                        {
                            FZipBuilder->SFXCaption = AnsiString(&Data[i+7], Data[i+4]);
                            FZipBuilder->SFXDefaultDir = AnsiString(&Data[Data[i+4] + i + 7], Data[i+5]);
                            FZipBuilder->SFXCommandLine = AnsiString(&Data[Data[i+4] + Data[i+5] + i + 7], Data[i+6]);
                        }
                        else
                        {
                            if (cll & 128)
                                FZipBuilder->SFXOptions = FZipBuilder->SFXOptions << SFXNoSuccessMsg;
                            char *Datastr = &Data[i + 8];
                            FZipBuilder->SFXCaption = AnsiString(&Datastr[1], Datastr[0]);
                            Datastr = Datastr + Datastr[0] + 1;
                            FZipBuilder->SFXDefaultDir = AnsiString(&Datastr[1], Datastr[0]);
                            Datastr = Datastr + Datastr[0] + 1;
                            FZipBuilder->SFXCommandLine = AnsiString(&Datastr[1], Datastr[0]);
                            Datastr = Datastr + Datastr[0] + 1;
                            FZipBuilder->SFXMessage = AnsiString(&Datastr[1], Datastr[0]);
                        }
                        break;
                    }
                }
            }
        }
        __finally
        {
            delete[] Data;
        }
    }
    return Result;
}

//---------------------------------------------------------------------------
// replace an icon in the given file
// 1.79. adapted for new WriteIconToStream function
int __fastcall TZipIntSFX::ReplaceIcon(const int& ExeFile, const int MaxPos,
                                       Graphics::TIcon* NewIcon)
{
    TMemoryStream *IconStream = NULL;
    char* SFXBuffer = NULL;
    IMAGE_DOS_HEADER *DOSHead;
    PEheader *Header;
    DWord  FoundAddr = 0;
    pISH SectionHeader;
    int Result = SE_NoFoundError;
    try
    {
        pIRDatE IRDataEntry;
        while (true)
        {
            try
            {
                // Put the SFX file into a buffer.
                SFXBuffer = new char[MaxPos];
                FileSeek(ExeFile, 0, soFromBeginning);
                FileRead(ExeFile, SFXBuffer, MaxPos);
                // Read and check the ZipSFX.bin file for an icon resource.
                DOSHead = (IMAGE_DOS_HEADER *)SFXBuffer;
                if (DOSHead->e_magic != IMAGE_DOS_SIGNATURE)
                    break;
                Header = (PEheader *)((char *)DOSHead + DOSHead->e_lfanew);
                if (Header->signature != IMAGE_NT_SIGNATURE)
                    break;
                SectionHeader = &Header->section_header;
                for (int Section = 0; Section < Header->_head.NumberOfSections; Section++)
                { // Walk through sections.
                    if (!StrComp((char *)&SectionHeader->Name[0], ".rsrc"))
                    { // Found the resource section.
                        IRDataEntry = LookForDirs(SFXBuffer + SectionHeader->PointerToRawData,
                                                  SectionHeader->VirtualAddress,
                                                  SectionHeader->SizeOfRawData,
                                                  &Header->opt_head.DataDirectory[0]);
                        if (!IRDataEntry)
                            break;
                        // Change memory offset to file offset.
                        FoundAddr = IRDataEntry->OffsetToData;
                        FoundAddr = FoundAddr - SectionHeader->VirtualAddress +
                                    SectionHeader->PointerToRawData;
                        break;
                    }
                    SectionHeader++;
                }
                if (!FoundAddr)
                    break;
                BITMAPINFO* BI = reinterpret_cast<BITMAPINFO*>(SFXBuffer + FoundAddr);
                // Save the icon data to a stream.
                IconStream = new TMemoryStream();
                WriteIconToStream(IconStream, NewIcon->Handle, BI->bmiHeader.biWidth,
                                  BI->bmiHeader.biHeight / 2, BI->bmiHeader.biBitCount);
                // Only handle icons with this size.
                if ((IconStream->Size - 22) == static_cast<int>(IRDataEntry->Size))
                {
                    // Reposition to the actual data and put it into a buffer.
                    IconStream->Seek(22, soFromBeginning);
                    IconStream->ReadBuffer(SFXBuffer, IRDataEntry->Size - 22);
                    // Go to start of the icon resource in the new created file.
                    FileSeek(ExeFile, static_cast<__int64>(FoundAddr), soFromBeginning);
                    // And write the changed icon data from the buffer.
                    FileWrite(ExeFile, SFXBuffer, IRDataEntry->Size - 22);
                    Result = 0;
                }
                else
                    Result = SE_IconSizeError;
            }
            catch (...)
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
        FileSeek(ExeFile, 0, soFromEnd);
    }
    return Result;
}

//---------------------------------------------------------------------------
// Search for the right(with our icon) resource directory entry.
pIRDatE __fastcall TZipIntSFX::SearchResDirEntry(pIRD ResStart, pIRDirE entry, int Depth)
{
    pIRDatE data;
    if (entry->NameIsString)
        return 0;    // No named resources.
    if (!Depth && entry->Id != 3)
        return 0;   // Only icon resources.
    if (Depth == 1 && entry->Id != 1)
        return 0; // Only icon with ID 0x1.
    if (!entry->DataIsDirectory)
    {
        data = (pIRDatE)((char *)ResStart + entry->OffsetToData);
        return data;
    }
    return BrowseResDir(ResStart, (pIRD)((char *)ResStart + entry->OffsetToDirectory), Depth + 1);
}

//----------------------------------------------------------------------------------------
void __fastcall TZipIntSFX::SetRegFailPath(const String Value)
{
    if (Value != FRegFailPath)
        FRegFailPath = Value;
}

//---------------------------------------------------------------------------
// added version 3.1.0.0
void __fastcall TZipIntSFX::SetSFXLanguage(const String Value)
{
    if (FSFXLanguage.UpperCase() != Value.UpperCase())
    {
        FSFXLanguage = Value.UpperCase();
        if (Included_Languages->IndexOfName(FSFXLanguage) > -1)
            FSFXLanguage = Included_Languages->Values[FSFXLanguage];
    }
}

//----------------------------------------------------------------------------------------
// NOTE: we will allow a dir to be specified that doesn't exist,
// since this is not the only way to locate the DLLs.
void __fastcall TZipIntSFX::SetSFXPath(const String Value)
{
    if (Value != SFXPath)
    {
        FSFXPath = Value;
        int ValLen = FSFXPath.Length();
        // If there is a trailing '\' in path, cut it off:
        if (ValLen && (FSFXPath[ValLen] == '\\'))
            FSFXPath.SetLength(ValLen - 1);
    }
}

//---------------------------------------------------------------------------
void __fastcall TZipIntSFX::SetVersion(const String Value)
{
// no action but we want it to be shown in the object inspector
}
//---------------------------------------------------------------------------
// write as required colour depth (bits per pixel)
int __fastcall TZipIntSFX::WriteIconToStream(Classes::TStream* Stream, HICON Icon,
        int Width, int Height, int depth)
{
    const int RC3_ICON = 1;
    ICONDIR IcoDir = {0};
    ICONINFO IconInfo;
    int Length = 0;

    if (depth <= 4)
        depth = 4;
    else
        if (depth <= 8)
            depth = 8;
        else
            if (depth <= 16)
                depth = 16;
            else
                if (depth <= 24)
                    depth = 24;
                else
                    return -1;
    int colors = 1 << depth;

    BITMAPINFO *BI = NULL;
    HDC dc = NULL;
    int sz;
    if (!GetIconInfo(Icon, &IconInfo))
        return -1;

    try
    {
        BITMAP cbm;
        if (!GetObject(IconInfo.hbmColor, sizeof(BITMAP), &cbm))
            return 0;
        if ((Width != cbm.bmWidth) || (Height != cbm.bmHeight))
            return 0;
        // ok should be acceptable
        sz = sizeof(BITMAPINFOHEADER);
        if (depth != 24)
            sz += colors * sizeof(RGBQUAD);    // pallet
        int cofs = sz;                       // offset to colorbits
        sz += Width * Height * depth / 8;    // bits
        int mofs = sz;                       // offset to maskbits
        sz += Width * Height / 8;
        // allocate memory for it
        BI = reinterpret_cast<BITMAPINFO*>(new BYTE[sz]);
        memset(BI, 0, sz);
        // set required attributes for colour bitmap
        BI->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        BI->bmiHeader.biWidth = Width;
        BI->bmiHeader.biHeight = Height;
        BI->bmiHeader.biPlanes = 1;
        BI->bmiHeader.biBitCount = static_cast<WORD>(depth);
        BI->bmiHeader.biCompression = BI_RGB;
        void* Bits = (reinterpret_cast<BYTE*>(BI)) + cofs;
        // prepare for mono mask bits
        BITMAPINFO MBI = {{0}};
        MBI.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        MBI.bmiHeader.biWidth = Width;
        MBI.bmiHeader.biHeight = Height;
        MBI.bmiHeader.biPlanes = 1;
        MBI.bmiHeader.biBitCount = 1;
        BYTE* MaskBits = (reinterpret_cast<BYTE*>(BI)) + mofs;
        dc = CreateCompatibleDC(NULL);
        if (dc)
        {
            if (GetDIBits(dc, IconInfo.hbmColor, 0, Height, Bits, BI, DIB_RGB_COLORS) > 0)
            {
                // ok get mask bits
                if (GetDIBits(dc, IconInfo.hbmMask, 0, Height, MaskBits, &MBI, DIB_RGB_COLORS) > 0)
                {
                    // good we have both
                    DeleteDC(dc);  // release it quick before anything can go wrong
                    dc = NULL;
                    IcoDir.idType = RC3_ICON;
                    IcoDir.idCount = 1;
                    IcoDir.idEntries[0].bWidth = static_cast<BYTE>(Width);
                    IcoDir.idEntries[0].bHeight = static_cast<BYTE>(Height);
                    IcoDir.idEntries[0].bColorCount = static_cast<BYTE>(depth);
                    IcoDir.idEntries[0].dwBytesInRes = sz;
                    IcoDir.idEntries[0].dwImageOffset = sizeof(ICONDIR);
                    BI->bmiHeader.biHeight = Height * 2; // color height includes mask bits
                    BI->bmiHeader.biSizeImage += MBI.bmiHeader.biSizeImage;
                }
            }
        }
        if (Stream)
        {
            Stream->Write(&IcoDir, sizeof(ICONDIR));
            Stream->Write(BI, sz);
        }
        Length = sz + sizeof(IcoDir);   // ok
    }
    __finally
    {
        if (dc)
            DeleteDC(dc);
        DeleteObject(IconInfo.hbmColor);
        DeleteObject(IconInfo.hbmMask);
        delete[] BI;
    }
    return Length;
}
//---------------------------------------------------------------------------
__fastcall TZipIntSFX::TZipIntSFX(Classes::TComponent *AOwner) : TComponent(AOwner)
{
    Included_Languages = new TStringList();
    EnumResourceNames(0, RT_RCDATA, reinterpret_cast<ENUMRESNAMEPROC>(ResEnumFunc),
                      reinterpret_cast<int>(Included_Languages));
    Detached = false;
}

//---------------------------------------------------------------------------
__fastcall TZipIntSFX::~TZipIntSFX(void)
{
    delete Included_Languages;
}

#endif // no_sfx
