#ifndef ZipStructsH
#define ZipStructsH
#include <Windows.hpp> // for PEHeader info

 /*  The zipfile format is defined as below:
 *
 *   [Extra Extended Signature]
 *   NumberOfFiles *{ local header + Filename + [extra data] + [file data] + [data_descriptor] }
 *   NumberOfFiles *{ central directory + Filename + [extra data] + [filecomment] }
 *   End Of Central directory + [ZipFile comment]
 */

#pragma option -a1
struct ZipLocalHeader 
{
	unsigned long	HeaderSig;			// Should be 0x04034B50
	unsigned short	VersionNeed;
	unsigned short	Flag;
	unsigned short	ComprMethod;
	unsigned short	ModifTime;
	unsigned short	ModifDate;
	unsigned long	CRC32;
	unsigned long	ComprSize;
	unsigned long	UnComprSize;
	unsigned short	FileNameLen;
	unsigned short ExtraLen;
};
//	... and the filename itself,
// ... and the extra data.

struct ZipDataDescriptor 
{
	unsigned long	DataDescSig;		// Should be 0x08074B50
	unsigned long	CRC32;
	unsigned long	ComprSize;
	unsigned long	UnComprSize;
};


struct ZipCentralHeader 
{
	unsigned long	HeaderSig;			// Should be 0x02014B50
	unsigned char	VersionMadeBy0;   // Made by version number
	unsigned char	VersionMadeBy1;   // Made by host number
	unsigned short	VersionNeed;
	unsigned short	Flag;
	unsigned short	ComprMethod;
	unsigned short	ModifTime;
	unsigned short	ModifDate;
	unsigned long	CRC32;
	unsigned long	ComprSize;
	unsigned long	UnComprSize;
	unsigned short	FileNameLen;
	unsigned short ExtraLen;
	unsigned short	FileComLen;
	unsigned short	DiskStart;			// The number of the disk on which this file begins.
	unsigned short	IntFileAtt;
	unsigned long	ExtFileAtt;
	unsigned long	RelOffLocal;		// This is the offset from the start of the first disk on
												// which this file appears, to where the local header
												// should be found.
};
//	... and the filename itself,
// ... and the extra data,
// ... and the file comment.

struct ZipEndOfCentral 
{
	unsigned long	HeaderSig;			// Should be 0x06054B50
	unsigned short	ThisDiskNo;
	unsigned short	CentralDiskNo;
	unsigned short	CentralEntries;	// Total number of entries in the central dir on this disk.
	unsigned short	TotalEntries;		// Total number of entries in the central dir.
	unsigned long	CentralSize;
	unsigned long	CentralOffset;		// Offset of start of central directory with respect to the starting disk number.
	unsigned short	ZipCommentLen;
};
// And the comment itself.

/*
struct Zip64EOCLocator
{ 
	unsigned LocSig; 		       // Should be 0x07064B50
	unsigned DiskStt;
	__int64  RelOfs;
	unsigned NumberDisks;
} 

struct TZip64EndOfCentral
{
  unsigned LocSig;                       // Should be 0x06064B50
  __int64 EOCSize;                         // size of records after this
  TZipHost Host;
  unsigned short VersionNeed;
  unsigned ThisDisk;
  unsigned CentralDiskNo;
  __int64 CentralEntries;
  __int64 TotalCentralEntries;
  __int64 CentralSize;
  __int64 RelOfs;
};

*/               

#pragma option -a.

//----------------Icon resource info--------------------------------------------
#pragma option -a1
struct PEheader 
{		// Data from the local header
	DWORD                 signature;
	IMAGE_FILE_HEADER     _head;
	IMAGE_OPTIONAL_HEADER opt_head;
	IMAGE_SECTION_HEADER  section_header;
};
#pragma option -a.

typedef IMAGE_RESOURCE_DIR_STRING_U          *pIRDSU;
typedef const IMAGE_RESOURCE_DIRECTORY_ENTRY *pIRDirE;
typedef const IMAGE_DATA_DIRECTORY           *pIDD;
typedef const IMAGE_RESOURCE_DIRECTORY       *pIRD;
typedef IMAGE_RESOURCE_DATA_ENTRY            *pIRDatE;
typedef IMAGE_SECTION_HEADER                 *pISH;





#endif


