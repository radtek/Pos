//---------------------------------------------------------------------------

#ifndef VersionH
#define VersionH
//---------------------------------------------------------------------------

#define _PreReleaseBuild 0x2L

// Public methods
UnicodeString GetFileVersion();
UnicodeString GetFileVersionString();
int GetFileVersionMajor();
int GetFileVersionMinor();

// Internal methods
bool isPreReleaseBuild();
UnicodeString getFileVersionFromStringBlock();
UnicodeString getFileVersionFromFixedInfo();

//---------------------------------------------------------------------------
#endif
