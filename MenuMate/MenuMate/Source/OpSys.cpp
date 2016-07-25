//---------------------------------------------------------------------------
#pragma hdrstop

#include "OpSys.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

OpSys::OpSys()
{
   OSVERSIONINFO osvi;
   char szVersion [80];
   memset(&osvi, 0, sizeof(OSVERSIONINFO));
   osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
   GetVersionEx (&osvi);
   dwPlatformId = osvi.dwPlatformId;
}

void OpSys::Initialise(Database::TDBTransaction &DBTransaction)
{
   GetDataBaseVersion(DBTransaction,DBMajorVersion,DBMinorVersion);
   MajorVersion = GetFileVersionMajor();
   MinorVersion = GetFileVersionMinor();

//   SoftwareVersion = GetFileVersion();
    SoftwareVersion = GetFileVersionString();

   MMSoftwareVersion = IntToStr(MajorVersion) + "." + IntToStr(MinorVersion);
   MMDBVersion = IntToStr(DBMajorVersion) + "." + IntToStr(DBMinorVersion);
}

void OpSys::GetDataBaseVersion(Database::TDBTransaction &DBTransaction,int &DBMajorVersion,int &DBMinorVersion)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
   "SELECT TIME_STAMP,VERSION_NUMBER FROM VERSIONHISTORY ORDER BY TIME_STAMP DESC, VERSION_NUMBER DESC";
   IBInternalQuery->ExecQuery();
   if(IBInternalQuery->RecordCount)
   {
   	UnicodeString Ver = IBInternalQuery->FieldByName("VERSION_NUMBER")->AsString;
      int FirstSectionEnd = Ver.Pos(".")-1;
      DBMajorVersion = StrToIntDef(Ver.SubString(1,FirstSectionEnd),-1);
      UnicodeString RestOfStr = Ver.SubString(Ver.Pos(".")+1,Ver.Length());
      int SecondSectionEnd = RestOfStr.Pos(".")-1;
      if(SecondSectionEnd <= 0) SecondSectionEnd = RestOfStr.Length();
      DBMinorVersion = StrToIntDef(RestOfStr.SubString(1,SecondSectionEnd),-1);
   }
   else
   {
		DBMajorVersion = -1;
      DBMinorVersion = -1;
	}
}
