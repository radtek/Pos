/*
  ZBStructs.h - Header file with definitions of the structures used int PKZip Archives
  TZipBuilder VCL by Chris Vleghert and Eric W. Engler
  v1.79
  Copyright (C) 2005  Roger Aelbrecht

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

  last modified 2005-11-20
---------------------------------------------------------------------------*/
#ifndef ZipStructsH
#define ZipStructsH
#include <Windows.hpp> // for PEHeader info

#pragma option -a1
struct ZipDirEntry
{		// Data from the Central header
	unsigned char	MadeByVersion;
	unsigned char	HostVersionNo;
	Word				Version;
	Word				Flag;
	Word				CompressionMethod;
	long				DateTime;
	long				CRC32;
#ifdef ALLOW_2G
  unsigned CompressedSize;
  unsigned UncompressedSize;
#else
	long CompressedSize;
	long UncompressedSize;
#endif
	Word				FileNameLength;
	Word				ExtraFieldLength;
	unsigned short	FileCommentLen;
	unsigned short	StartOnDisk;
	unsigned short	IntFileAttrib;
	unsigned long	ExtFileAttrib;
	unsigned long	RelOffLocalHdr;
	String			FileName;
	String			FileComment;
	bool				Encrypted;
#ifndef UNICODE
	AnsiString ExtraData;
#else
	RawByteString ExtraData;
#endif
};
#pragma option -a.



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

//----------------consts--------------------------------------------
const int LocalFileHeaderSig   = 0x04034B50; // 'PK34 (in file: 504B0304)'
const int CentralFileHeaderSig = 0x02014B50; // 'PK12'
const int EndCentralDirSig     = 0x06054B50; // 'PK56'
const int ExtLocalSig			     = 0x08074B50; // 'PK78'
const int LocalDirEntrySize    = 42;         // Fixed size of zip dir entry in local zip directory.


#endif


