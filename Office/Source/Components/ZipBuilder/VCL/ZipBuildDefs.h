#ifndef ZipBuildDefsH
#define ZipBuildDefsH

const AnsiString RESOURCE_ERROR = "ZipMsgXX.res is probably not linked to the executable\nMissing String ID is: %d";
const AnsiString RESOURCE_ERROR1 = "\nZipMsgXX.res is probably not linked in the executable";
  // moved from zipbuilder.h to here because const in .h file prevent creation of
  // pre-compiled header files
  // Needed in ConvertOem2Iso function.
const int FS_FAT_ = 0;
const int FS_HPFS_ = 6;
const int FS_NTFS_ = 11;
const int DEF_HOST_VER = 0x0B16;    // SysNo = 11 : NTFS; Verno = 22 :2.2
const long LocalFileHeaderSig   = 0x04034B50; // 'PK34 (in file: 504B0304)'
const long CentralFileHeaderSig = 0x02014B50; // 'PK12'
const long EndCentralDirSig     = 0x06054B50; // 'PK56'
const long ExtLocalSig			  = 0x08074B50; // 'PK78'
const int BUSY_ERROR = -255;                  // new 1.72y
const int Reentry_Error = 0x4000000;          // new 1.76
const int  LocalDirEntrySize    = 42;         // Fixed size of zip dir entry in local zip directory.


const String ZIPBUILDERVERSION = "1.78";
const String ZIPBUILDERBUILD    = "1.78.3.0";
      /* file version format changed to 1.72.0.2
                 1 - major version number  : needs sync with TZipMaster
                72 - minor                 : needs sync
                 0 - update                : no sync
                 2 - build number          : no sync  */
  // new constants 1.73
const int ZIPBUILDERVER = 178;
const int Min_ZipDll_Vers = 177;
const int Min_UnzDll_Vers = 177;

const int ZIPVERSION = 178;
const int UNZIPVERSION = 177;

   // constants for external attribute from stas.h
const int S_IFREG = 0x8000; // regular file
const int S_IFDIR = 0x4000; // directory
const int S_IREAD = 0x0100; // owner may read
const int S_IWRITE= 0x0080; // owner may write
const int S_IEXEC = 0x0040; // owner may execute (dir search)






#endif
