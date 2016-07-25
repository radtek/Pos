//---------------------------------------------------------------------------

#ifndef OpSysH
#define OpSysH
//---------------------------------------------------------------------------
#include "Version.cpp"
#include "MM_DBCore.h"

class OpSys
{
   public:
   DWORD dwPlatformId;
   UnicodeString MMSoftwareVersion,MMDBVersion, SoftwareVersion;
   int DBMajorVersion,DBMinorVersion;
   int MajorVersion,MinorVersion;
   void GetDataBaseVersion(Database::TDBTransaction &DBTransaction,int &DBMajorVersion,int &DBMinorVersion);
   OpSys();
   void Initialise(Database::TDBTransaction &DBTransaction);
};

#endif
