//---------------------------------------------------------------------------

#pragma hdrstop

#include "ZipUtils.h"
#include "ZipStructs.h"
#include "ZipBuildDefs.h"
#include "ZipStrs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

/* FileNameMatch----------------------------------------------------
 1.73 (22 May 2003) - RP  filename match similar to dlls
 modified for handling MBCS Strings
*/
// ===========================================================================
static int pat_match(  const char *p, const char *s,
                       const char *pe, const char *se);
/* ===========================================================================
  Adapted by R.Peters from many sources
  Break the pattern and string into name and extension parts and match
  each separately using pat_match().
		*s :: Filename to match against wildcard pattern.
		*p :: Dos wildcard pattern to match.
*/
bool __export FileNameMatch(const AnsiString pat, const AnsiString spec)
{
  const char *pe, *p2e;		// pattern sections e=end
  const char *se, *s2e;		// string sections 2=start of second part
  char c;
  if(!pat.Length() || !spec.Length())  return false;
  const char *p = pat.c_str();
  const char *s = spec.c_str();
  // find extension (and eos)
  pe = 0;
  for(p2e = p;(c = *p2e) != 0; ++p2e)
  {
    if(c == '.') pe = p2e;
    else
      if(c == '\\' || c == '/') pe = 0;               // still in path
      else
        if (LeadBytes.Contains(c))
          ++p2e;        // ignore 2nd byte
  }
  if(!pe)                // no dot in pattern
  {
    for(s2e = s; *s2e; ++s2e)  // find eos
      ;
    return pat_match(p,s, p2e, s2e);
  }
  // find extension (and eos)
  se = 0;
  for(s2e = s;(c = *s2e) != 0; ++s2e)
  {
    if(c == '.')  se = s2e; 		// at last '.'
    else
      if(c == '\\' || c == '/')    // still in path
        se = 0;
      else
        if(LeadBytes.Contains(c))
          ++s2e;        // ignore 2nd byte
  }
  if(!se)    // no sExt
    se = s2e; 			// end
  // if pattern ext <> '.*' then exts must match
  if((pe[1] != '*' || pe[2]) && !pat_match( pe, se, p2e,s2e))
    return false;
  return pat_match( p, s, pe, se);  // match rest
}
/*
 * Nonrecursively compare the sh pattern p, with the string s,
 * and return 1 if they match, and 0 if they don't.
		*p :: pattern to match (ex: *.*).
		*s :: String to match it to (ex: FNAME.DAT).
		pe :: past end of pattern
		se :: past end of string
 */
static int pat_match(  const char *p, const char *s,
                       const char *pe, const char *se)
{
  char c,cs;
  const char *pa=0;		// pattern position last '*' - null = none
  const char *sa;		// string position last '*'
  for ( ;; )
  {	// keep scanning until end or unrecoverable mismatch
    if(s == se)
    {	// run out of string - only '*''s allowed in pattern
      while(*p == '*') p++;
      return (p >= pe);		// ok if end of pattern
    }
    // more string to process
    if((c = *p) == '*')
    {		// on mask
      if( ++p >= pe ) 	// last char - rest of string must match
        return 1;
      // note scan point incase must back up
      pa = p;
      sa = s;
    }
    else
    {
      cs = *s;
      // if successful match, inc pointers and keep trying
      if((( cs == c ) || (toupper(cs) == toupper(c) )
           || (cs == '\\' && c == '/') || (cs == '/' && c== '\\')
           || (c == '?' &&  cs))
          && (LeadBytes.Contains(c) ? (*++p == *++s) : true))
      {
        ++p;
        ++s;
      }
      else
      {  // no match
        if(!pa)      	// any asterisk?
          return 0;		// no - no match
        p = pa;                   // back up
        s = ++sa;                 //  next char in string
      }
    }
  }
}
// FileNameMatch

/*? ForceDirectory------------------------------------------------
 1.73 (18 June 2003) RA bug fix wrong parameters for Delimithpath
 1.73 (22 May 2003) RA use of DelimithPath
 1.73 (20 May 2003) RA new function
 force the existence of a directory (and its parents)
 return values
 true = dir exits or creation successfull
*/
bool __export ForceDirectory(const AnsiString Dir)
{
  bool Result = true;
  if(Dir == "") return Result;
  AnsiString sDir = DelimitPath(Dir, false);
  if(DirExists(sDir) || (ExtractFilePath(sDir) == sDir)) return Result; // avoid 'c:\xyz:\' problem.
  if(!ForceDirectory(ExtractFilePath(sDir))) Result = false;
  else Result = CreateDirectory(sDir.c_str(),NULL);
  return Result;
}
//? ForceDirectory

//--------------------------------------------------------
// 1.73 (22 May 2003) RP new MBCS safe function to change path separator
AnsiString __export SetSlash(const AnsiString path, bool forward)
{
  char f,r;
  char c;
  AnsiString Result = path;
  const char * pth = path.c_str();
  int len = path.Length();
  if(forward)
  {
    f = '\\';
    r = '/';
  }
  else
  {
    f = '/';
    r = '\\';
  }
  if(len)
  {
    unsigned i = 1;       // first result char
    while((c = *pth) != 0)
    {
      if(c == f)
        Result[i] = r;    // will change address of Result.data
      else
        if(LeadBytes.Contains(c))
        {
          ++pth;
          ++i;
        }
      ++pth;
      ++i;
    }
  }
  return Result;
}
// SetSlash

/* DelimitPath-----------------------------------------------------
 1.73 (26 July 2003) RA changed handling of empty strings
 1.73 (30 May 2003) RP changed signature + allow either slash
 1.73 (22 May 2003) RP new function to add/remove trailing slash
*/
AnsiString __export DelimitPath(const AnsiString path, bool sep)
{
  AnsiString Result = path;
  if(!path.Length())   // if empty or null
  {
    if(sep) Result = '\\';
    return Result;
  }
  const char *e = path.c_str();
  char c, b = 0;
  char used = '\\';
  while((c = *e++) != 0)
  {
    if(LeadBytes.Contains(c)) ++e;
    b = c;
    if(b == '/' || b == '\\') used = b;
  }
  if((b == '\\' || b == '/') != sep)
  {
    if(sep)
      Result = path + used;
    else
      Result = String(path.c_str(), path.Length() - 1);
  }
  return Result;
}
//? DelimitPath

/*? DirExists-------------------------------------------------------
 1.73 (12 July 2003) RP return true empty string (current directory)
 1.73 (20 May 2003) RA made function a member of ZipBuilder
*/
bool __export DirExists(const AnsiString Name)
{
	if(Name == "") return true;
	DWORD Code = ::GetFileAttributes(Name.c_str());
	return (Code != 0xFFFFFFFF) & ((FILE_ATTRIBUTE_DIRECTORY & Code) != 0);
}
//? DirExists

/*? FileVersion-----------------------------------------------------
*/
AnsiString __export FileVersion(AnsiString fname)
{
	AnsiString Result = "?.?.?.?";
  AnsiString Buffer;
  DWORD VersionHandle;
  int VersionSize = GetFileVersionInfoSize(fname.c_str(), & VersionHandle);
  if(VersionSize)
  {
    Buffer.SetLength(VersionSize);
    GetFileVersionInfo(fname.c_str(), VersionHandle, VersionSize, Buffer.c_str());
    char *buff;
    UINT buflen;
    char txtstr[100];
    char sbuff[30];
    struct LANGANDCODEPAGE
    {
      WORD wLanguage;
      WORD wCodePage;
    } *lpTranslate;
    UINT vsize;
    VerQueryValue(Buffer.c_str(),"\\VarFileInfo\\Translation",(LPVOID *)&lpTranslate,&vsize);
    wsprintf(sbuff,"\\StringFileInfo\\%04x%04x\\",lpTranslate->wLanguage,lpTranslate->wCodePage);
    if(VerQueryValue(Buffer.c_str(),StrCat(StrCopy(txtstr,sbuff),"FileVersion"),
                   reinterpret_cast<void **>(&buff),&buflen))
       Result = AnsiString(buff);
	}
	return Result;
}
//? FileVersion

/*? QueryZip-----------------------------------------------------------
1.75.0.5 10 March 2004 RA change filesize to int64
1.75 18 February 2004 allow >2G
1.73 02 July 2003 RP
 Return value:
 Bit Mapped result (if > 0)
 bits 0..3 = S L P D     - start of file
 S 1 = EXE file
 L 2 = Local Header
 P 4 = first part of split archive
 D 8 = Central Header
 bit 4 = Correct comment length (clear if junk at end of file)
 bits  5..7 = Loc Cen End
End 128 = end of central found (must be set for any chance at archive)
Cen 64 = linked Central Directory start (should be set unless split directory)
Loc 32 = Local Header linked to first Central (should be set unless split file)
 -7  = Open, read or seek error
 -9  = exception error
 Good file results    All cases 16 less if comment length (or file length) is wrong
 Zip = 128+64+32+16+2 = 242
 SFX = 128+64+32+16+1 = 241
 Last Part Span = 128+16 (144 - only EOC)
								= 128+16+8 (152 - split directory)
								= 128+64+16 (208 - split file)
*/
int __export QueryZip(const AnsiString Name)
{
  ZipEndOfCentral EOC;
  ZipCentralHeader CentralHead;
  unsigned Sig;
  bool EOCPossible = false;
  int fileHandle = -1;
  int res = 0;
  int Result = -7;
  int Size;
  __int64 fileSize;
  unsigned char* ZipBuf = NULL;
  bool NotDone = true;
  try
  {
    try
    {
        // Open the input archive, presumably the last disk.
      if((fileHandle = FileOpen(Name, fmShareDenyWrite | fmOpenRead )) == -1) return Result;
      Result = 0; // rest errors normally file too small
       // first we check if the start of the file has an IMAGE_DOS_SIGNATURE
      if(FileRead(fileHandle, &Sig, 4) != 4) return Result;
      if(LOWORD(Sig) == IMAGE_DOS_SIGNATURE) res = 1;
      else if(Sig == LocalFileHeaderSig) res = 2;
			else if(Sig == CentralFileHeaderSig) res = 8; // part of split Central Directory
			else if(Sig == ExtLocalSig) res = 4;        // first part of span
      // A test for a zip archive without a ZipComment.
			fileSize = FileSeek(fileHandle, -(__int64)sizeof(EOC), 2);
			if(fileSize == -1) return Result;  // not zip - too small
        // A test for a zip archive without a ZipComment.
      if(FileRead(fileHandle, &EOC, sizeof(EOC)) == sizeof(EOC) &&
				                    EOC.HeaderSig == EndCentralDirSig)
			{
        EOCPossible = true;
				res |= 128;              // EOC
				if(!EOC.ZipCommentLen) res |= 16;     // good comment length
				if(EOC.CentralDiskNo == EOC.ThisDiskNo)
				{                           // verify start of central
					if((FileSeek(fileHandle, (__int64)EOC.CentralOffset, 0) != -1)
						&& (FileRead(fileHandle, &CentralHead, sizeof(CentralHead)) == sizeof(CentralHead))
					&& CentralHead.HeaderSig == CentralFileHeaderSig)
					{
						res |= 64;           // has linked Central
						if((CentralHead.DiskStart == EOC.ThisDiskNo)
							&& (FileSeek(fileHandle, (__int64)CentralHead.RelOffLocal, 0) != -1)
							&& (FileRead(fileHandle, &Sig, sizeof(Sig)) == sizeof(Sig))
							&& Sig == LocalFileHeaderSig)
							   res |= 32;         // linked local
					}
            NotDone = false;
				}
        if(NotDone) res &= 0x01F;            // remove rest
      }
      if(NotDone)
      {
              // try to locate EOC
        fileSize += sizeof(EOC);
        Size = 65535 + sizeof(EOC);
        if(Size > fileSize) Size = static_cast<int>(fileSize);
        ZipBuf = new unsigned char[Size + 1];
        __int64 ReadPos = fileSize - Size;
        if((FileSeek(fileHandle, ReadPos, 0) != -1)
          && (FileRead(fileHandle, ZipBuf, Size) == Size))
        {
          // Finally try to find the EOC record within the last 65K...
          ZipEndOfCentral* pEOC = reinterpret_cast<ZipEndOfCentral*>(ZipBuf + Size - sizeof(EOC));
          while((unsigned char*)pEOC > ZipBuf)     // reverse search
          {
            --(char*)pEOC;
            if(pEOC->HeaderSig == EndCentralDirSig)
            {                         // possible EOC found
              res |= 128;          // EOC
              // check correct length comment
              unsigned BufPos = static_cast<unsigned>(ReadPos) + ((unsigned char*)pEOC - ZipBuf);
              if((int)(BufPos + sizeof(EOC)) + pEOC->ZipCommentLen == fileSize)
                               res |= 16; // good comment length
              if(pEOC->CentralDiskNo == pEOC->ThisDiskNo)
              {                       // verify start of central
                if((FileSeek(fileHandle, (__int64)pEOC->CentralOffset, 0) != -1)
                  && (FileRead(fileHandle, &CentralHead, sizeof(CentralHead)) == sizeof(CentralHead))
                  && CentralHead.HeaderSig == CentralFileHeaderSig)
                {
                  res |= 64;       // has linked Central
                  if(CentralHead.DiskStart == pEOC->ThisDiskNo
                    && (FileSeek(fileHandle,(__int64)CentralHead.RelOffLocal, 0) != -1)
                    && (FileRead(fileHandle, &Sig, sizeof(Sig)) == sizeof(Sig))
                    && Sig == LocalFileHeaderSig)
                    res |= 32;     // linked local
                }
                break;
              }
              res &= 0x01F;        // remove rest
            }
          }                            // while
        }
        if(EOCPossible && (res & 128) == 0)
          res |= 128;
      }
    }
    catch(...)
    {
      	Result = -9;                     // exception
    }
  }
  __finally
  {
    if(fileHandle != -1) FileClose(fileHandle);
		if(!Result) Result = res;
    if(ZipBuf) delete[] ZipBuf;
  }
  return Result;
}
//? QueryZip

/*? PathConcat-------------------------------------------------
 1.73 RP new function
	add extra to path ensuring single backslash
*/
AnsiString __export PathConcat(AnsiString path, AnsiString extra)
{
  int PathLen = path.Length();
  AnsiString Result = path;
  if(PathLen)
  {
    char PathLast = path[PathLen];
    if((PathLast != ':') && extra.Length())
    {
      if((extra[1] == '\\') == (PathLast == '\\'))
      {
        if(PathLast == '\\') Result  = path.SubString(1, PathLen - 1); // remove trailing
        else Result = path + "\\"; // append trailing
      }
    }
  }
  Result += extra;
  return Result;
}
//? PathConcat

/*? IsWild
1.73.4
 returns true if filespec contains wildcard(s)
*/

bool __export IsWild(const AnsiString fspec)
{
  return fspec.Pos("*") || fspec.Pos("?");
}
//? IsWild

/* EraseFile---------------------------------------------------------------------
1.77 moved from ZBuilder
 Delete a file and put it in the recyclebin on demand.
*/
int EraseFile(const String Fname, DeleteOpts How)
{
	SHFILEOPSTRUCT SHF;
	char* FromStr = NULL;
	int	Result  = -1;
	String DelFileName = Fname;
	// If we do not have a full path then FOF_ALLOWUNDO does not work!?
	if(ExtractFilePath(Fname) == "" )
		DelFileName = GetCurrentDir() + '\\' + Fname;
	// We need to be able to 'Delete' without getting an error
	// if the file does not exists as in ReadSpan() can occur
	if(!FileExists(DelFileName)) return Result;
	try {
		int dfnLen = DelFileName.Length() + 2;
		FromStr    = new char[dfnLen];
		SHF.hwnd   = Application->Handle;
		SHF.wFunc  = FO_DELETE;
		SHF.pFrom  = FromStr;
		SHF.pTo    = NULL;
		SHF.fFlags = FOF_NOCONFIRMATION | FOF_SILENT | FOF_NOERRORUI;
    if(How == htdAllowUndo) SHF.fFlags |= FOF_ALLOWUNDO;
		StrPLCopy(FromStr, DelFileName, dfnLen);
		FromStr[dfnLen - 1] = '\0';
		Result = SHFileOperation(&SHF);
	}
	catch( ... ) { }
	delete[] FromStr;
	return Result;
}
