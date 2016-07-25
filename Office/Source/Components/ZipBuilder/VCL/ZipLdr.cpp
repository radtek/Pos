//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <lzexpand.h>
#include "ZipBuilder.h"
#include "ZipLdr.h"
#include "ZipStrs.h"
#include "ZipMsg.h"
#include "ZipBuildDefs.h"
#include "ZipUtils.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
char*	RType = "BinFile";
const int zldTemp = 1;
const int	zldAuto = 2;
const int	zldFixed = 4;
const int DllRes[2]	= {RDLL_Unz, RDLL_Zip};
const int VersRes[2] = {RDLL_UVer, RDLL_ZVer};
const char* DllNames[2] = {"UNZDLL.DLL","ZIPDLL.DLL"};
const char* ExecNames[2] = {"UnzDllExec","ZipDllExec"};
const char* VersNames[2] = {"GetUnzDllVersion","GetZipDllVersion"};
const char* PrivNames[2] = {"GetUnzDllPrivVersion", "GetZipDllPrivVersion"};


TZipLibLoader::TZipLibLoader(TObject *owner, bool type)
{
  Clear();
	fOwner = dynamic_cast<TZipBase*>(owner);
	IsZip = type ? true : false;
	loadLevel = 0;
	fPath = DllNames[IsZip];
  TmpFileName = "";
}

TZipLibLoader::~TZipLibLoader(void)
{
	if(hndl)  ::FreeLibrary(hndl);
	hndl = NULL;
  if((TmpFileName != "") && FileExists(TmpFileName)) DeleteFile(TmpFileName);
	loadLevel = 0;
}

void TZipLibLoader::Clear(void)
{
  hndl = 0;
	fExecFunc = NULL;
	fVersFunc = NULL;
	fPrivFunc = NULL;
	fVer = 0;
	fPriv = 0;
}

void TZipLibLoader::Abort(void)
{
  if(hndl && fExecFunc)
       fExecFunc(NULL);
}

// 1.73 27 July 2003 RA / RP unload wrong version
int TZipLibLoader::LoadDll(int minver, bool level)
{
	DoLoad(level ? zldFixed : zldAuto);
	if(fVer < minver)
	{
		DoUnload(zldFixed | zldAuto | zldTemp);
		throw EZipBuilder(LD_BadDll,fPath);
	}
	return hndl ? fVer : 0;
}

// TZipLibLoader::LoadLib
// 1.73 15 September 2003 RA added parameter MustExist
//1.73.2.4 10 September 2003 RP new function
int TZipLibLoader::LoadLib(AnsiString Fullpath, bool MustExist)
{
  if(hndl) FreeLibrary(hndl);
  Clear();
  unsigned oldMode = SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
  try
  {
    hndl = ::LoadLibrary(Fullpath.c_str());
    if(hndl)
    {
      fExecFunc = (ZDllExecAddress)::GetProcAddress(hndl, ExecNames[IsZip]);
      fVersFunc = (ZDllVersionAddress)::GetProcAddress(hndl, VersNames[IsZip]);
      fPrivFunc = (ZDllPrivFuncAddress)::GetProcAddress(hndl, PrivNames[IsZip]);
      fPath = GetLoadedPath();
    }
  }
  __finally
  {
    SetErrorMode(oldMode);
  }
  if(!hndl)
  {
    Clear();
    loadLevel = 0;
    if(MustExist) throw EZipBuilder(LD_NoDll, Fullpath);
    return 0;
  }
  if(fExecFunc && fVersFunc)
  {
    fVer = fVersFunc();
    if(fPrivFunc) fPriv = fPrivFunc();
  }
  if((fVer < 153) || (fVer > 300))
  {
    Fullpath = fPath;
    FreeLibrary(hndl);
    Clear();
    loadLevel = 0;
    if(MustExist) throw EZipBuilder(LD_BadDll, Fullpath);
  }
  return fPriv;
}
// TZipLibLoader::LoadLib

void TZipLibLoader::Unload(bool level)
{
	DoUnload(level ? zldFixed : zldAuto);
}

int __fastcall TZipLibLoader::GetLoaded(void)
{
  return hndl ? fVer : 0;
}

// TZipLibLoader::Expand
//1.73.2.4 10 September 2003 RP new function
int TZipLibLoader::Expand(AnsiString src, AnsiString dest)
{
  OFSTRUCT sTOF;
  OFSTRUCT dTOF;
  int sH = -1, dH = -1;
  int Result = 0;
  try
  {
    sH = LZOpenFile(src.c_str(), &sTOF, OF_READ);
    dH = LZOpenFile(dest.c_str(), &dTOF, OF_CREATE);
    if((sH > 0) && (dH >= 0)) Result = LZCopy(sH, dH);
  }
  __finally
  {
    if(sH >= 0) LZClose(sH);
    if(dH >= 0) LZClose(dH);
  }
  return Result;
}

// 1.80 7 September 2003 RA trace message adapted
int TZipLibLoader::DoUnload(int level)
{
	loadLevel = (loadLevel & ~level) & 7;	// reset bit
	if (!loadLevel && hndl)
	{
    if(dynamic_cast<TZipBase*>(fOwner)->Verbose)
      dynamic_cast<TZipBase*>(fOwner)->Report(zacMessage,0,ZipLoadStr(LD_DllUnloaded,"unloaded ")+ fPath,0);
		::FreeLibrary(hndl);
		hndl = NULL;
	}
	if (!hndl)
  {
    Clear();
    loadLevel = 0;
  }
  return fVer;
}
// 1.76 13 May 2004 RP throw exception in case of error
// 1.73 (2 August 2003 RA) added minver parameter, loading by call to LoadDll
int TZipLibLoader::DoExec(void *info, int minver)
{
  LoadDll(minver, false);
	int r = fExecFunc(info);
	DoUnload(zldTemp);
  if(r < 0)
  {
    switch(r)
    {
      case -1:
      case -2: throw EZipBuilder(GE_DLLAbort);
      case -3: throw EZipBuilder(GE_DLLBusy);
      case -4: throw EZipBuilder(GE_DLLCancel);
      case -5: throw EZipBuilder(GE_DLLMem);
      case -6: throw EZipBuilder(GE_DLLStruct);
      case -8: throw EZipBuilder(GE_DLLEvent);
      default: throw EZipBuilder(GE_DLLCritical,ARRAYOFCONST((r)));
    }
  }
  return r;
}

AnsiString __fastcall TZipLibLoader::GetLoadedPath(void)
{
	String p = "";
	String buf;
	buf.SetLength(MAX_PATH);
	if(hndl && GetModuleFileName(hndl, buf.c_str(), MAX_PATH))
		p = buf.c_str();
	return p;
}
/* TZipLibLoader::DoLoad
1.73 15 September RA extra parameter in LoadLib
1.73.2.6 10 September 2003 RP only load resource if later or no other found
1.73 24 July 2003 RA fix
*/
int TZipLibLoader::DoLoad(int level)
{
  loadLevel = (loadLevel | level) & 7;
  if(!hndl)
	{
		TZipBase* Owner = dynamic_cast<TZipBase*>(fOwner);
		try
		{
			Owner->StartWaitCursor();
			fVer = 0;
			loadLevel = level;
			String FullPath = "";
			String dir = Owner->DLLDirectory;
    	if(TmpFileName != "") FullPath = TmpFileName;
    	if(FullPath == "")
    	{
      	if(dir != "")
      	{
        	FullPath = PathConcat(dir, DllNames[IsZip]);
        	if(dir[1] == '.')
          	FullPath = PathConcat(ExtractFilePath(ParamStr(0)),FullPath);
        	if(!FileExists(FullPath)) FullPath = "";
      	}
    	}
    	if(FullPath == "")
			    FullPath = DllNames[IsZip];  // let Windows search the std dirs
    	AnsiString RVrs = LoadStr(VersRes[IsZip]);
    	int RVer = StrToIntDef(RVrs.SubString(1,5),0);
    	if(RVer > LoadLib(FullPath, false))
    	{
      	if(LoadLib(GetResDllPath(), true) < 17300)  // res dll older than 1.73
        	 LoadLib(FullPath, false); //could not load resource
    	}
    	if(Owner->Verbose)
      	Owner->Report(zacMessage,0,ZipLoadStr(LD_DllLoaded,"loaded ") + fPath,0);
    }
    __finally
    {
    	Owner->StopWaitCursor();	
    }	
  }
	return fVer;
}
// TZipLibLoader::DoLoad

/*? TZipLibLoader::GetResDllPath
1.73.2.6 7 September 2006 extract 'compressed' resource files
*/
AnsiString __fastcall TZipLibLoader::GetResDllPath(void)
{
  Word m;
	AnsiString Result = "";
	bool done = false;
  AnsiString tmp = LoadStr(VersRes[IsZip]);   
  TZipBase* Owner = dynamic_cast<TZipBase*>(fOwner);
  int ver = StrToIntDef(tmp.SubString(1,5),0);
  if( ver > 17300) // works from version 1.73.0.0 onwards
  {
    if(IsZip)
      TmpFileName = Owner->MakeTempFileName("ZMZ",".dll");
    else
      TmpFileName = Owner->MakeTempFileName("ZMU",".dll");
    TResourceStream* rs = new TResourceStream((int)HInstance, DllRes[IsZip], RType);
    try
    {
      if(rs)
      {
        TFileStream* fs;
        try
        {
					rs->Read(&m, 2);
					if(m == 0)
					{
            fs = new TFileStream(TmpFileName, fmCreate);
						rs->Position = 6;
						done = (fs->CopyFrom(rs,rs->Size - 6) == (rs->Size - 6));
					}
					else if(m == 2)
          {
            tmp = Owner->MakeTempFileName("ZMt",".tmp");
            rs->Position = 6;
            fs = new TFileStream(tmp, fmCreate);
            done = (fs->CopyFrom(rs,rs->Size - 6) == (rs->Size - 6));
            delete fs;
            fs = NULL; // to avoid second delete in __finally
            if(done)
            {
              done = Expand(tmp, TmpFileName) > 100000; // expand returns size of expanded dll
              DeleteFile(tmp);
            }
          }
        }
				__finally
        {
					delete fs;
				}
      }
    }
    __finally
    {
      delete rs;
      if(!done && FileExists(TmpFileName))
         DeleteFile(TmpFileName);
      if(!FileExists(TmpFileName)) TmpFileName = "";
      else Result = TmpFileName;
    }
  }
  return Result;
}

/*? TZipLibLoader::GetVer
1.73.2.8 2 Oct 2003 new getter
*/
int __fastcall TZipLibLoader::GetVer(void)
{
	DoLoad(zldTemp);
	DoUnload(zldTemp);
  return fVer;
}
//? TZipLibLoader::GetVer

/*? TZipLibLoader::GetPath
1.73.2.8 2 Oct 2003 new getter
*/
AnsiString __fastcall TZipLibLoader::GetPath(void)
{
	DoLoad(zldTemp);
	DoUnload(zldTemp);
  return fPath;
}
//? TZipLibLoader::GetPath

/*? TZipLibLoader::GetPriv
1.73.2.8 2 Oct 2003 new getter
*/
int __fastcall TZipLibLoader::GetPriv(void)
{
	DoLoad(zldTemp);
	DoUnload(zldTemp);
  return fPriv;
}
//? TZipLibLoader::GetPriv



