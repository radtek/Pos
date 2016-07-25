//---------------------------------------------------------------------------

#include "SharedMemory.h"

#include <stdio.h>
#include <conio.h>

#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::
//   TSharedMemoryManager
//:::::::::::::::::::::::::::
__fastcall TSharedMemoryManager::TSharedMemoryManager() : TObject()
{
	_hMapFile = NULL;
	_pBuf     = NULL;
}
//...........................................................................

__fastcall TSharedMemoryManager::~TSharedMemoryManager()
{
	if( _smmType == smmtServer )
	{
		if( _pBuf     != NULL ) UnmapViewOfFile( _pBuf );
		if( _hMapFile != NULL ) CloseHandle( _hMapFile );
	}
}
//...........................................................................

bool TSharedMemoryManager::Init( TSharedMemoryManagerType inType, TSharedMemoryType inShareType, UnicodeString inName, __int32 inCapacity )
{
	bool result = true;

	//:::::::::::::::::::::::::::::

	result = ( inType == smmtServer ) ? pInitServer( inShareType, inName, inCapacity ) : pInitClient( inShareType, inName, inCapacity );

	//:::::::::::::::::::::::::::::

	return result;
}
//...........................................................................

BYTE TSharedMemoryManager::ReadByte( __int64 inPosition )
{
	try
	{
		return pReadByteFromSharedMem( ( PVOID )_pBuf, inPosition );
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

__int16 TSharedMemoryManager::ReadInt16( __int64 inPosition )
{
	try
	{
		return pReadInt16FromSharedMem( ( PVOID )_pBuf, inPosition );
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

unsigned __int16 TSharedMemoryManager::ReadUInt16( __int64 inPosition )
{
	try
	{
		return pReadUInt16FromSharedMem( ( PVOID )_pBuf, inPosition );
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

__int32 TSharedMemoryManager::ReadInt32( __int64 inPosition )
{
	try
	{
		return pReadInt32FromSharedMem( ( PVOID )_pBuf, inPosition );
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

unsigned __int32 TSharedMemoryManager::ReadUInt32( __int64 inPosition )
{
	try
	{
		return pReadUInt32FromSharedMem( ( PVOID )_pBuf, inPosition );
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

__int64 TSharedMemoryManager::ReadInt64( __int64 inPosition )
{
	try
	{
		return pReadInt64FromSharedMem( ( PVOID )_pBuf, inPosition );
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

unsigned __int64 TSharedMemoryManager::ReadUInt64( __int64 inPosition )
{
	try
	{
		return pReadUInt64FromSharedMem( ( PVOID )_pBuf, inPosition );
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

float TSharedMemoryManager::ReadSingle( __int64 inPosition )
{
	try
	{
		return pReadSingleFromSharedMem( ( PVOID )_pBuf, inPosition );
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

double TSharedMemoryManager::ReadDouble( __int64 inPosition )
{
	try
	{
		return pReadDoubleFromSharedMem( ( PVOID )_pBuf, inPosition );
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

__int32 TSharedMemoryManager::ReadBlock( __int64 inPosition, __int32 inCount, byte* outBlock )
{
	try
	{
		return pReadBlockFromSharedMem( ( PVOID )_pBuf, inPosition, inCount, outBlock );
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

__int32 TSharedMemoryManager::ReadBlock( __int64 inPosition, __int32 inCount, __int16* outBlock )
{
	try
	{
		return pReadBlockFromSharedMem( ( PVOID )_pBuf, inPosition, inCount, outBlock );
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

__int32 TSharedMemoryManager::ReadBlock( __int64 inPosition, __int32 inCount, unsigned __int16* outBlock )
{
	try
	{
		return pReadBlockFromSharedMem( ( PVOID )_pBuf, inPosition, inCount, outBlock );
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

__int32 TSharedMemoryManager::ReadBlock( __int64 inPosition, __int32 inCount, __int32* outBlock )
{
	try
	{
		return pReadBlockFromSharedMem( ( PVOID )_pBuf, inPosition, inCount, outBlock );
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

__int32 TSharedMemoryManager::ReadBlock( __int64 inPosition, __int32 inCount, unsigned __int32* outBlock )
{
	try
	{
		return pReadBlockFromSharedMem( ( PVOID )_pBuf, inPosition, inCount, outBlock );
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

__int32 TSharedMemoryManager::ReadBlock( __int64 inPosition, __int32 inCount, __int64* outBlock )
{
	try
	{
		return pReadBlockFromSharedMem( ( PVOID )_pBuf, inPosition, inCount, outBlock );
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

__int32 TSharedMemoryManager::ReadBlock( __int64 inPosition, __int32 inCount, unsigned __int64* outBlock )
{
	try
	{
		return pReadBlockFromSharedMem( ( PVOID )_pBuf, inPosition, inCount, outBlock );
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

__int32 TSharedMemoryManager::ReadBlock( __int64 inPosition, __int32 inCount, float* outBlock )
{
	try
	{
		return pReadBlockFromSharedMem( ( PVOID )_pBuf, inPosition, inCount, outBlock );
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

__int32 TSharedMemoryManager::ReadBlock( __int64 inPosition, __int32 inCount, double* outBlock )
{
	try
	{
		return pReadBlockFromSharedMem( ( PVOID )_pBuf, inPosition, inCount, outBlock );
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

void TSharedMemoryManager::Write( __int64 inPosition, BYTE inValue )
{
	try
	{
		pWriteByteToSharedMem( ( PVOID )_pBuf, inValue, inPosition );
	}
	catch( ... )
	{
	}
}
//...........................................................................

void TSharedMemoryManager::Write( __int64 inPosition, __int16 inValue )
{
	try
	{
		pWriteInt16ToSharedMem( ( PVOID )_pBuf, inValue, inPosition );
	}
	catch( ... )
	{
	}
}
//...........................................................................

void TSharedMemoryManager::Write( __int64 inPosition, unsigned __int16 inValue )
{
	try
	{
		pWriteUInt16ToSharedMem( ( PVOID )_pBuf, inValue, inPosition );
	}
	catch( ... )
	{
	}
}
//...........................................................................

void TSharedMemoryManager::Write( __int64 inPosition, __int32 inValue )
{
	try
	{
		pWriteInt32ToSharedMem( ( PVOID )_pBuf, inValue, inPosition );
	}
	catch( ... )
	{
	}
}
//...........................................................................

void TSharedMemoryManager::Write( __int64 inPosition, unsigned __int32 inValue )
{
	try
	{
		pWriteUInt32ToSharedMem( ( PVOID )_pBuf, inValue, inPosition );
	}
	catch( ... )
	{
	}
}
//...........................................................................

void TSharedMemoryManager::Write( __int64 inPosition, __int64 inValue )
{
	try
	{
		pWriteInt64ToSharedMem( ( PVOID )_pBuf, inValue, inPosition );
	}
	catch( ... )
	{
	}
}
//...........................................................................

void TSharedMemoryManager::Write( __int64 inPosition, unsigned __int64 inValue )
{
	try
	{
		pWriteUInt64ToSharedMem( ( PVOID )_pBuf, inValue, inPosition );
	}
	catch( ... )
	{
	}
}
//...........................................................................

void TSharedMemoryManager::Write( __int64 inPosition, float inValue )
{
	try
	{
		pWriteSingleToSharedMem( ( PVOID )_pBuf, inValue, inPosition );
	}
	catch( ... )
	{
	}
}
//...........................................................................

void TSharedMemoryManager::Write( __int64 inPosition, double inValue )
{
	try
	{
		pWriteDoubleToSharedMem( ( PVOID )_pBuf, inValue, inPosition );
	}
	catch( ... )
	{
	}
}
//...........................................................................

void TSharedMemoryManager::WriteBlock( __int64 inPosition, __int32 inCount, byte* inBlock)
{
	try
	{
		pWriteBlockToSharedMem( ( PVOID )_pBuf, inPosition, inCount, inBlock );
	}
	catch( ... )
	{
	}
}
//...........................................................................

void TSharedMemoryManager::WriteBlock( __int64 inPosition, __int32 inCount, __int16* inBlock )
{
	try
	{
		pWriteBlockToSharedMem( ( PVOID )_pBuf, inPosition, inCount, inBlock );
	}
	catch( ... )
	{
	}
}
//...........................................................................

void TSharedMemoryManager::WriteBlock( __int64 inPosition, __int32 inCount, unsigned __int16* inBlock )
{
	try
	{
		pWriteBlockToSharedMem( ( PVOID )_pBuf, inPosition, inCount, inBlock );
	}
	catch( ... )
	{
	}
}
//...........................................................................

void TSharedMemoryManager::WriteBlock( __int64 inPosition, __int32 inCount, __int32* inBlock )
{
	try
	{
		pWriteBlockToSharedMem( ( PVOID )_pBuf, inPosition, inCount, inBlock );
	}
	catch( ... )
	{
	}
}
//...........................................................................

void TSharedMemoryManager::WriteBlock( __int64 inPosition, __int32 inCount, unsigned __int32* inBlock )
{
	try
	{
		pWriteBlockToSharedMem( ( PVOID )_pBuf, inPosition, inCount, inBlock );
	}
	catch( ... )
	{
	}
}
//...........................................................................

void TSharedMemoryManager::WriteBlock( __int64 inPosition, __int32 inCount, __int64* inBlock )
{
	try
	{
		pWriteBlockToSharedMem( ( PVOID )_pBuf, inPosition, inCount, inBlock );
	}
	catch( ... )
	{
	}
}
//...........................................................................

void TSharedMemoryManager::WriteBlock( __int64 inPosition, __int32 inCount, unsigned __int64* inBlock )
{
	try
	{
		pWriteBlockToSharedMem( ( PVOID )_pBuf, inPosition, inCount, inBlock );
	}
	catch( ... )
	{
	}
}
//...........................................................................

void TSharedMemoryManager::WriteBlock( __int64 inPosition, __int32 inCount, float* inBlock )
{
	try
	{
		pWriteBlockToSharedMem( ( PVOID )_pBuf, inPosition, inCount, inBlock );
	}
	catch( ... )
	{
	}
}
//...........................................................................

void TSharedMemoryManager::WriteBlock( __int64 inPosition, __int32 inCount, double* inBlock )
{
	try
	{
		pWriteBlockToSharedMem( ( PVOID )_pBuf, inPosition, inCount, inBlock );
	}
	catch( ... )
	{
	}
}
//...........................................................................

//===========================================================================
//  Private section
//===========================================================================

bool TSharedMemoryManager::pInitServer( TSharedMemoryType inShareType, UnicodeString inName, __int64 inCapacity)
{
	bool result = true;

	//:::::::::::::::::::::::::::::

	result = ( inShareType == smtFile ) ? pInitServerSharedMemMappedFile( inName, inCapacity ) : pInitServerSharedMem( inName, inCapacity );

	//:::::::::::::::::::::::::::::

	return result;
}
//...........................................................................

bool TSharedMemoryManager::pInitClient( TSharedMemoryType inShareType, UnicodeString inName, __int64 inCapacity )
{
	bool result = true;

	//:::::::::::::::::::::::::::::

	result = ( inShareType == smtFile ) ? pInitClientSharedMemMappedFile( inName, inCapacity ) : pInitClientSharedMem( inName, inCapacity );

	//:::::::::::::::::::::::::::::

	return result;
}
//...........................................................................

bool TSharedMemoryManager::pInitServerSharedMemMappedFile( UnicodeString inName, __int64 inCapacity )
{
	bool result = true;

	//:::::::::::::::::::::::::::::

	try
	{
		result = false;
	}
	catch( ... )
	{
		result = false;
	}

	//:::::::::::::::::::::::::::::

	return result;
}
//...........................................................................

bool TSharedMemoryManager::pInitServerSharedMem( UnicodeString inName, __int64 inCapacity )
{
	bool result = true;

	//:::::::::::::::::::::::::::::

	try
	{
		_hMapFile = CreateFileMapping(
						INVALID_HANDLE_VALUE,    // use paging file
						NULL,                    // default security
						PAGE_READWRITE,          // read/write access
						0,                       // max. object size
						inCapacity,              // buffer size
						inName.t_str() );        // name of mapping object

		_pBuf     = (LPTSTR) MapViewOfFile(
						_hMapFile,            // handle to map object
						FILE_MAP_ALL_ACCESS,  // read/write permission
						0,
						0,
						inCapacity );
	}
	catch( ... )
	{
		result = false;
	}

	//:::::::::::::::::::::::::::::

	return result;
}
//...........................................................................

bool TSharedMemoryManager::pInitClientSharedMemMappedFile( UnicodeString inName, __int64 inCapacity )
{
	bool result = true;

	//:::::::::::::::::::::::::::::

	try
	{
		result = false;
	}
	catch( ... )
	{
		result = false;
	}

	//:::::::::::::::::::::::::::::

	return result;
}
//...........................................................................

bool TSharedMemoryManager::pInitClientSharedMem( UnicodeString inName, __int64 inCapacity )
{
	bool result = true;

	//:::::::::::::::::::::::::::::

	try
	{
		_hMapFile = OpenFileMapping(
						FILE_MAP_ALL_ACCESS,   // read/write access
						FALSE,                 // do not inherit the name
						inName.t_str() );      // name of mapping object


		_pBuf     = (LPTSTR) MapViewOfFile(
						_hMapFile,            // handle to map object
						FILE_MAP_ALL_ACCESS,  // read/write permission
						0,
						0,
						inCapacity );
	}
	catch( ... )
	{
		result = false;
	}

	//:::::::::::::::::::::::::::::

	return result;
}
//...........................................................................

BYTE TSharedMemoryManager::pReadByteFromSharedMem( PVOID inSharedMem, __int64 inPosition )
{
	BYTE result = 0;

	//:::::::::::::::::::::::::::::

	try
	{
		LPCTSTR tmp = ( LPCTSTR )pReadFromSharedMem( inSharedMem, inPosition, sizeof( BYTE ) );

		result = *( ( BYTE * ) tmp );
	}
	catch( ... )
	{
 		throw;
	}

	//:::::::::::::::::::::::::::::

	return result;
}
//...........................................................................

__int16 TSharedMemoryManager::pReadInt16FromSharedMem( PVOID inSharedMem, __int64 inPosition )
{
	__int16 result = 0;

	//:::::::::::::::::::::::::::::

	try
	{
		LPCTSTR tmp = ( LPCTSTR )pReadFromSharedMem( inSharedMem, inPosition, sizeof( __int16 ) );

		result = *( ( __int16 * ) tmp );
	}
	catch( ... )
	{
		throw;
	}

	//:::::::::::::::::::::::::::::

	return result;
}
//...........................................................................

unsigned __int16 TSharedMemoryManager::pReadUInt16FromSharedMem( PVOID inSharedMem, __int64 inPosition )
{
	UINT result = 0;

	//:::::::::::::::::::::::::::::

	try
	{
		LPCTSTR tmp = ( LPCTSTR )pReadFromSharedMem( inSharedMem, inPosition, sizeof( unsigned __int16 ) );

		result = *( ( unsigned __int16 * ) tmp );
	}
	catch( ... )
	{
		throw;
	}

	//:::::::::::::::::::::::::::::

	return result;
}
//...........................................................................

__int32 TSharedMemoryManager::pReadInt32FromSharedMem( PVOID inSharedMem, __int64 inPosition )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::

	try
	{
		LPCTSTR tmp = ( LPCTSTR )pReadFromSharedMem( inSharedMem, inPosition, sizeof( __int32 ) );

		result = *( ( __int32 * ) tmp );
	}
	catch( ... )
	{
		throw;
	}

	//:::::::::::::::::::::::::::::

	return result;
}
//...........................................................................

unsigned __int32 TSharedMemoryManager::pReadUInt32FromSharedMem( PVOID inSharedMem, __int64 inPosition )
{
	UINT result = 0;

	//:::::::::::::::::::::::::::::

	try
	{
		LPCTSTR tmp = ( LPCTSTR )pReadFromSharedMem( inSharedMem, inPosition, sizeof( unsigned __int32 ) );

		result = *( ( unsigned __int32 * ) tmp );
	}
	catch( ... )
	{
		throw;
	}

	//:::::::::::::::::::::::::::::

	return result;
}
//...........................................................................

__int64 TSharedMemoryManager::pReadInt64FromSharedMem( PVOID inSharedMem, __int64 inPosition )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::

	try
	{
		LPCTSTR tmp = ( LPCTSTR )pReadFromSharedMem( inSharedMem, inPosition, sizeof( __int64 ) );

		result = *( ( __int64 * ) tmp );
	}
	catch( ... )
	{
		throw;
	}

	//:::::::::::::::::::::::::::::

	return result;
}
//...........................................................................

unsigned __int64 TSharedMemoryManager::pReadUInt64FromSharedMem( PVOID inSharedMem, __int64 inPosition )
{
	UINT result = 0;

	//:::::::::::::::::::::::::::::

	try
	{
		LPCTSTR tmp = ( LPCTSTR )pReadFromSharedMem( inSharedMem, inPosition, sizeof( unsigned __int64 ) );

		result = *( ( unsigned __int64 * ) tmp );
	}
	catch( ... )
	{
		throw;
	}

	//:::::::::::::::::::::::::::::

	return result;
}
//...........................................................................

float TSharedMemoryManager::pReadSingleFromSharedMem( PVOID inSharedMem, __int64 inPosition )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::

	try
	{
		LPCTSTR tmp = ( LPCTSTR )pReadFromSharedMem( inSharedMem, inPosition, sizeof( float ) );

		result = *( ( float * ) tmp );
	}
	catch( ... )
	{
		throw;
	}

	//:::::::::::::::::::::::::::::

	return result;
}
//...........................................................................


double TSharedMemoryManager::pReadDoubleFromSharedMem( PVOID inSharedMem, __int64 inPosition )
{
	UINT result = 0;

	//:::::::::::::::::::::::::::::

	try
	{
		LPCTSTR tmp = ( LPCTSTR )pReadFromSharedMem( inSharedMem, inPosition, sizeof( double ) );

		result = *( ( double * ) tmp );
	}
	catch( ... )
	{
		throw;
	}

	//:::::::::::::::::::::::::::::

	return result;
}
//...........................................................................

__int32 TSharedMemoryManager::pReadBlockFromSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, byte* outBlock )
{
	__int32 size       = inCount * sizeof( byte );
	PVOID   pvoidBlock = ( PVOID )outBlock;

	return pReadBlockFromSharedMem( inSharedMem, inPosition, size, pvoidBlock );
}
//...........................................................................

__int32 TSharedMemoryManager::pReadBlockFromSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, __int16* outBlock )
{
	__int32 size       = inCount * sizeof( __int16 );
	PVOID   pvoidBlock = ( PVOID )outBlock;

	return pReadBlockFromSharedMem( inSharedMem, inPosition, size, pvoidBlock );
}
//...........................................................................

__int32 TSharedMemoryManager::pReadBlockFromSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, unsigned __int16* outBlock )
{
	__int32 size       = inCount * sizeof( unsigned __int16 );
	PVOID   pvoidBlock = ( PVOID )outBlock;

	return pReadBlockFromSharedMem( inSharedMem, inPosition, size, pvoidBlock );
}
//...........................................................................

__int32 TSharedMemoryManager::pReadBlockFromSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, __int32* outBlock )
{
	__int32 size       = inCount * sizeof( __int32 );
	PVOID   pvoidBlock = ( PVOID )outBlock;

	return pReadBlockFromSharedMem( inSharedMem, inPosition, size, pvoidBlock );
}
//...........................................................................

__int32 TSharedMemoryManager::pReadBlockFromSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, unsigned __int32* outBlock )
{
	__int32 size       = inCount * sizeof( unsigned __int32 );
	PVOID   pvoidBlock = ( PVOID )outBlock;

	return pReadBlockFromSharedMem( inSharedMem, inPosition, size, pvoidBlock );
}
//...........................................................................

__int32 TSharedMemoryManager::pReadBlockFromSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, __int64* outBlock )
{
	__int32 size       = inCount * sizeof( __int64 );
	PVOID   pvoidBlock = ( PVOID )outBlock;

	return pReadBlockFromSharedMem( inSharedMem, inPosition, size, pvoidBlock );
}
//...........................................................................

__int32 TSharedMemoryManager::pReadBlockFromSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, unsigned __int64* outBlock )
{
	__int32 size       = inCount * sizeof( unsigned __int64 );
	PVOID   pvoidBlock = ( PVOID )outBlock;

	return pReadBlockFromSharedMem( inSharedMem, inPosition, size, pvoidBlock );
}
//...........................................................................

__int32 TSharedMemoryManager::pReadBlockFromSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, float* outBlock )
{
	__int32 size       = inCount * sizeof( float );
	PVOID   pvoidBlock = ( PVOID )outBlock;

	return pReadBlockFromSharedMem( inSharedMem, inPosition, size, pvoidBlock );
}
//...........................................................................

__int32 TSharedMemoryManager::pReadBlockFromSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, double* outBlock )
{
	__int32 size       = inCount * sizeof( double );
	PVOID   pvoidBlock = ( PVOID )outBlock;

	return pReadBlockFromSharedMem( inSharedMem, inPosition, size, pvoidBlock );
}
//...........................................................................

__int32 TSharedMemoryManager::pReadBlockFromSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inSize, PVOID outBlock )
{
	try
	{
		LPCTSTR tmp = ( LPCTSTR )pReadFromSharedMem( inSharedMem, inPosition, inSize );
		CopyMemory( outBlock, ( PVOID )( tmp ), inSize );

		delete[] tmp;

		return inSize;
	}
	catch( ... )
	{
		throw;
	}
}
//...........................................................................

void TSharedMemoryManager::pWriteByteToSharedMem( PVOID inSharedMem, BYTE inValue, __int64 inPosition )
{
	try
	{
		PVOID source = &inValue;

		pWriteToSharedMem( source, inSharedMem, inPosition, sizeof( BYTE ) );
	}
	catch( ... )
	{
		throw;
	}
}
//...........................................................................

void TSharedMemoryManager::pWriteInt16ToSharedMem( PVOID inSharedMem, __int16 inValue, __int64 inPosition )
{
	try
	{
		PVOID source = &inValue;

		pWriteToSharedMem( source, inSharedMem, inPosition, sizeof( __int16 ) );
	}
	catch( ... )
	{
 		throw;
	}
}
//...........................................................................

void TSharedMemoryManager::pWriteUInt16ToSharedMem( PVOID inSharedMem, unsigned __int16 inValue, __int64 inPosition )
{
	try
	{
		PVOID source = &inValue;

		pWriteToSharedMem( source, inSharedMem, inPosition, sizeof( unsigned __int16 ) );
	}
	catch( ... )
	{
 		throw;
	}
}
//...........................................................................

void TSharedMemoryManager::pWriteInt32ToSharedMem( PVOID inSharedMem, __int32 inValue, __int64 inPosition )
{
	try
	{
		PVOID source = &inValue;

		pWriteToSharedMem( source, inSharedMem, inPosition, sizeof( __int32 ) );
	}
	catch( ... )
	{
		throw;
	}
}
//...........................................................................

void TSharedMemoryManager::pWriteUInt32ToSharedMem( PVOID inSharedMem, unsigned __int32 inValue, __int64 inPosition )
{
	try
	{
		PVOID source = &inValue;

		pWriteToSharedMem( source, inSharedMem, inPosition, sizeof( unsigned __int32 ) );
	}
	catch( ... )
	{
 		throw;
	}
}
//...........................................................................

void TSharedMemoryManager::pWriteInt64ToSharedMem( PVOID inSharedMem, __int64 inValue, __int64 inPosition )
{
	try
	{
		PVOID source = &inValue;

		pWriteToSharedMem( source, inSharedMem, inPosition, sizeof( __int64 ) );
	}
	catch( ... )
	{
		throw;
	}
}
//...........................................................................

void TSharedMemoryManager::pWriteUInt64ToSharedMem( PVOID inSharedMem, unsigned __int64 inValue, __int64 inPosition )
{
	try
	{
		PVOID source = &inValue;

		pWriteToSharedMem( source, inSharedMem, inPosition, sizeof( unsigned __int64 ) );
	}
	catch( ... )
	{
		throw;
	}
}
//...........................................................................

void TSharedMemoryManager::pWriteSingleToSharedMem( PVOID inSharedMem, float inValue, __int64 inPosition )
{
	try
	{
		PVOID source = &inValue;

		pWriteToSharedMem( source, inSharedMem, inPosition, sizeof( float ) );
	}
	catch( ... )
	{
		throw;
	}
}
//...........................................................................

void TSharedMemoryManager::pWriteDoubleToSharedMem( PVOID inSharedMem, double inValue, __int64 inPosition )
{
	try
	{
		PVOID source = &inValue;

		pWriteToSharedMem( source, inSharedMem, inPosition, sizeof( double ) );
	}
	catch( ... )
	{
		throw;
	}
}
//...........................................................................

void TSharedMemoryManager::pWriteBlockToSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, BYTE* inBlock )
{
	try
	{
		__int32   size       = inCount * sizeof( BYTE );
		PVOID pvoidBlock = ( PVOID )inBlock;

		pWriteToSharedMem( pvoidBlock, inSharedMem, inPosition, size );
	}
	catch( ... )
	{
		throw;
	}
}
//...........................................................................

void TSharedMemoryManager::pWriteBlockToSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, __int16* inBlock )
{
	try
	{
		__int32   size       = inCount * sizeof( __int16 );
		PVOID pvoidBlock = ( PVOID )inBlock;

		pWriteToSharedMem( pvoidBlock, inSharedMem, inPosition, size );
	}
	catch( ... )
	{
		throw;
	}
}
//...........................................................................

void TSharedMemoryManager::pWriteBlockToSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, unsigned __int16* inBlock )
{
 	try
	{
		__int32   size       = inCount * sizeof( unsigned __int16 );
		PVOID pvoidBlock = ( PVOID )inBlock;

		pWriteToSharedMem( pvoidBlock, inSharedMem, inPosition, size );
	}
	catch( ... )
	{
		throw;
	}
}
//...........................................................................

void TSharedMemoryManager::pWriteBlockToSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, __int32* inBlock )
{
	try
	{
		__int32   size       = inCount * sizeof( __int32 );
		PVOID pvoidBlock = ( PVOID )inBlock;

		pWriteToSharedMem( pvoidBlock, inSharedMem, inPosition, size );
	}
	catch( ... )
	{
 		throw;
	}
}
//...........................................................................

void TSharedMemoryManager::pWriteBlockToSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, unsigned __int32* inBlock )
{
	try
	{
		__int32   size       = inCount * sizeof( unsigned __int32 );
		PVOID pvoidBlock = ( PVOID )inBlock;

		pWriteToSharedMem( pvoidBlock, inSharedMem, inPosition, size );
	}
	catch( ... )
	{
		throw;
	}
}
//...........................................................................

void TSharedMemoryManager::pWriteBlockToSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, __int64* inBlock )
{
	try
	{
		__int32 size       = inCount * sizeof( __int64 );
		PVOID   pvoidBlock = ( PVOID )inBlock;

		pWriteToSharedMem( pvoidBlock, inSharedMem, inPosition, size );
	}
	catch( ... )
	{
		throw;
	}
}
//...........................................................................

void TSharedMemoryManager::pWriteBlockToSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, unsigned __int64* inBlock )
{
	try
	{
		__int32   size       = inCount * sizeof( unsigned __int64 );
		PVOID pvoidBlock = ( PVOID )inBlock;

		pWriteToSharedMem( pvoidBlock, inSharedMem, inPosition, size );
	}
	catch( ... )
	{
		throw;
	}
}
//...........................................................................

void TSharedMemoryManager::pWriteBlockToSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, float* inBlock )
{
	try
	{
		__int32   size       = inCount * sizeof( float );
		PVOID pvoidBlock = ( PVOID )inBlock;

		pWriteToSharedMem( pvoidBlock, inSharedMem, inPosition, size );
	}
	catch( ... )
	{
		throw;
	}
}
//...........................................................................

void TSharedMemoryManager::pWriteBlockToSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, double* inBlock )
{
	try
	{
		__int32   size       = inCount * sizeof( double );
		PVOID pvoidBlock = ( PVOID )inBlock;

		pWriteToSharedMem( pvoidBlock, inSharedMem, inPosition, size );
	}
	catch( ... )
	{
		throw;
	}
}
//...........................................................................

PVOID TSharedMemoryManager::pReadFromSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inSize )
{
	BYTE* result = new BYTE( inSize );

	//::::::::::::::::::::::::::::::::::::::

	 CopyMemory( result, ( PVOID )( ( LPCTSTR )( inSharedMem ) + inPosition ), inSize );

	//::::::::::::::::::::::::::::::::::::::

	return result;
}
//...........................................................................

void TSharedMemoryManager::pWriteToSharedMem( PVOID inSource, PVOID inSharedMem, __int64 inPosition, __int32 inSize )
{
	CopyMemory( ( PVOID )( ( LPCTSTR )( inSharedMem ) + inPosition ), inSource, inSize );
}
//...........................................................................

//:::::::::::::::::::::::::::
//   TSharedMemoryController
//:::::::::::::::::::::::::::
//---------------------------------------------------------------------------
//	Protected
//---------------------------------------------------------------------------
TSharedMemoryController::TSharedMemoryController()
{
	_sharedMemoryManager = new TSharedMemoryManager();
}
//...........................................................................

bool TSharedMemoryController::InitServer( TSharedMemoryType inSharedMemType, UnicodeString inName, __int64 inServerCapacity, __int64 inClientCapacity )
{
   bool result = true;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		_serverCapacity = inServerCapacity;
		_clientCapacity = inClientCapacity;

		_sharedMemoryManagerType = smmtServer;
		_sharedMemoryManager->Init( _sharedMemoryManagerType, inSharedMemType, inName, TotalCapacity );
		pSetBeginMemory( TotalCapacity );
	}
	catch( ... )
	{
		result = false;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
//...........................................................................

bool TSharedMemoryController::InitClient( TSharedMemoryType inSharedMemType, UnicodeString inName, __int64 inServerCapacity, __int64 inClientCapacity )
{
	bool result = true;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		_serverCapacity = inServerCapacity;
		_clientCapacity = inClientCapacity;

		_sharedMemoryManagerType = smmtClient;
		_sharedMemoryManager->Init( _sharedMemoryManagerType, inSharedMemType, inName, TotalCapacity );
		pSetBeginMemory( TotalCapacity );
	}
	catch( ... )
	{
		result = false;
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
//...........................................................................

//---------------------------------------------------------------------------
//	Public
//---------------------------------------------------------------------------
BYTE TSharedMemoryController::ReadByte( __int64 inPosition )
{
	try
	{
		return _sharedMemoryManager->ReadByte( pGetActualReadPosition( inPosition, MemoryManagerType ) );
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

__int16 TSharedMemoryController::ReadInt16( __int64 inPosition )
{
	try
	{
		return _sharedMemoryManager->ReadInt16( pGetActualReadPosition( inPosition, MemoryManagerType ) );
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

unsigned __int16 TSharedMemoryController::ReadUInt16( __int64 inPosition )
{
	try
	{
		return _sharedMemoryManager->ReadUInt16( pGetActualReadPosition( inPosition, MemoryManagerType ) );
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

__int32 TSharedMemoryController::ReadInt32( __int64 inPosition )
{
	try
	{
		return _sharedMemoryManager->ReadInt32( pGetActualReadPosition( inPosition, MemoryManagerType ) );
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

unsigned __int32 TSharedMemoryController::ReadUInt32( __int64 inPosition )
{
	try
	{
		return _sharedMemoryManager->ReadUInt32( pGetActualReadPosition( inPosition, MemoryManagerType ) );
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

__int64 TSharedMemoryController::ReadInt64( __int64 inPosition )
{
	try
	{
		return _sharedMemoryManager->ReadInt64( pGetActualReadPosition( inPosition, MemoryManagerType ) );
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

unsigned __int64 TSharedMemoryController::ReadUInt64( __int64 inPosition )
{
	try
	{
		return _sharedMemoryManager->ReadUInt64( pGetActualReadPosition( inPosition, MemoryManagerType ) );
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

float TSharedMemoryController::ReadSingle( __int64 inPosition )
{
	try
	{
		return _sharedMemoryManager->ReadSingle( pGetActualReadPosition( inPosition, MemoryManagerType ) );
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

double TSharedMemoryController::ReadDouble( __int64 inPosition )
{
	try
	{
		return _sharedMemoryManager->ReadDouble( pGetActualReadPosition( inPosition, MemoryManagerType ) );
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

__int32 TSharedMemoryController::ReadBlock( __int64 inPosition, __int32 inCount, BYTE* outBlock )
{
	try
	{
		return _sharedMemoryManager->ReadBlock( pGetActualReadPosition( inPosition, MemoryManagerType ), inCount, outBlock);
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

__int32 TSharedMemoryController::ReadBlock( __int64 inPosition, __int32 inCount, __int16* outBlock )
{
	try
	{
		return _sharedMemoryManager->ReadBlock( pGetActualReadPosition( inPosition, MemoryManagerType ), inCount, outBlock);
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

__int32 TSharedMemoryController::ReadBlock( __int64 inPosition, __int32 inCount, unsigned __int16* outBlock )
{
	try
	{
		return _sharedMemoryManager->ReadBlock( pGetActualReadPosition( inPosition, MemoryManagerType ), inCount, outBlock);
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

__int32 TSharedMemoryController::ReadBlock( __int64 inPosition, __int32 inCount, __int32* outBlock )
{
	try
	{
		return _sharedMemoryManager->ReadBlock( pGetActualReadPosition( inPosition, MemoryManagerType ), inCount, outBlock);
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

__int32 TSharedMemoryController::ReadBlock( __int64 inPosition, __int32 inCount, unsigned __int32* outBlock )
{
	try
	{
		return _sharedMemoryManager->ReadBlock( pGetActualReadPosition( inPosition, MemoryManagerType ), inCount, outBlock);
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

__int32 TSharedMemoryController::ReadBlock( __int64 inPosition, __int32 inCount, __int64* outBlock )
{
	try
	{
		return _sharedMemoryManager->ReadBlock( pGetActualReadPosition( inPosition, MemoryManagerType ), inCount, outBlock);
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

__int32 TSharedMemoryController::ReadBlock( __int64 inPosition, __int32 inCount, unsigned __int64* outBlock )
{
	try
	{
		return _sharedMemoryManager->ReadBlock( pGetActualReadPosition( inPosition, MemoryManagerType ), inCount, outBlock);
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

__int32 TSharedMemoryController::ReadBlock( __int64 inPosition, __int32 inCount, float* outBlock )
{
	try
	{
		return _sharedMemoryManager->ReadBlock( pGetActualReadPosition( inPosition, MemoryManagerType ), inCount, outBlock);
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

__int32 TSharedMemoryController::ReadBlock( __int64 inPosition, __int32 inCount, double* outBlock )
{
	try
	{
		return _sharedMemoryManager->ReadBlock( pGetActualReadPosition( inPosition, MemoryManagerType ), inCount, outBlock);
	}
	catch( ... )
	{
		return 0;
	}
}
//...........................................................................

bool TSharedMemoryController::Write( __int64 inPosition, BYTE inValue )
{
	try
	{
		_sharedMemoryManager->Write( pGetActualWritePosition( inPosition, MemoryManagerType ), inValue );
		return true;
	}
	catch( ... )
	{
		return false;
	}
}
//...........................................................................

bool TSharedMemoryController::Write( __int64 inPosition, __int16 inValue )
{
	try
	{
		_sharedMemoryManager->Write( pGetActualWritePosition( inPosition, MemoryManagerType ), inValue );
		return true;
	}
	catch( ... )
	{
		return false;
	}
}
//...........................................................................

bool TSharedMemoryController::Write( __int64 inPosition, unsigned __int16 inValue )
{
	try
	{
		_sharedMemoryManager->Write( pGetActualWritePosition( inPosition, MemoryManagerType ), inValue );
		return true;
	}
	catch( ... )
	{
		return false;
	}
}
//...........................................................................

bool TSharedMemoryController::Write( __int64 inPosition, __int32 inValue )
{
	try
	{
		_sharedMemoryManager->Write( pGetActualWritePosition( inPosition, MemoryManagerType ), inValue );
		return true;
	}
	catch( ... )
	{
		return false;
	}
}
//...........................................................................

bool TSharedMemoryController::Write( __int64 inPosition, unsigned __int32 inValue )
{
	try
	{
		_sharedMemoryManager->Write( pGetActualWritePosition( inPosition, MemoryManagerType ), inValue );
		return true;
	}
	catch( ... )
	{
		return false;
	}
}
//...........................................................................

bool TSharedMemoryController::Write( __int64 inPosition, __int64 inValue )
{
	try
	{
		_sharedMemoryManager->Write( pGetActualWritePosition( inPosition, MemoryManagerType ), inValue );
		return true;
	}
	catch( ... )
	{
		return false;
	}
}
//...........................................................................

bool TSharedMemoryController::Write( __int64 inPosition, unsigned __int64 inValue )
{
	try
	{
		_sharedMemoryManager->Write( pGetActualWritePosition( inPosition, MemoryManagerType ), inValue );
		return true;
	}
	catch( ... )
	{
		return false;
	}
}
//...........................................................................

bool TSharedMemoryController::Write( __int64 inPosition, float inValue )
{
	try
	{
		_sharedMemoryManager->Write( pGetActualWritePosition( inPosition, MemoryManagerType ), inValue );
		return true;
	}
	catch( ... )
	{
		return false;
	}
}
//...........................................................................

bool TSharedMemoryController::Write( __int64 inPosition, double inValue )
{
	try
	{
		_sharedMemoryManager->Write( pGetActualWritePosition( inPosition, MemoryManagerType ), inValue );
		return true;
	}
	catch( ... )
	{
		return false;
	}
}
//...........................................................................

bool TSharedMemoryController::WriteBlock( __int64 inPosition, __int32 inCount, BYTE* inBlock )
{
	try
	{
		_sharedMemoryManager->WriteBlock( pGetActualWritePosition( inPosition, MemoryManagerType ), inCount, inBlock );
		return true;
	}
	catch( ... )
	{
		return false;
	}
}
//...........................................................................

bool TSharedMemoryController::WriteBlock( __int64 inPosition, __int32 inCount, __int16* inBlock )
{
	try
	{
		_sharedMemoryManager->WriteBlock( pGetActualWritePosition( inPosition, MemoryManagerType ), inCount, inBlock );
		return true;
	}
	catch( ... )
	{
		return false;
	}
}
//...........................................................................

bool TSharedMemoryController::WriteBlock( __int64 inPosition, __int32 inCount, unsigned __int16* inBlock )
{
	try
	{
		_sharedMemoryManager->WriteBlock( pGetActualWritePosition( inPosition, MemoryManagerType ), inCount, inBlock );
		return true;
	}
	catch( ... )
	{
		return false;
	}
}
//...........................................................................

bool TSharedMemoryController::WriteBlock( __int64 inPosition, __int32 inCount, __int32* inBlock )
{
	try
	{
		_sharedMemoryManager->WriteBlock( pGetActualWritePosition( inPosition, MemoryManagerType ), inCount, inBlock );
		return true;
	}
	catch( ... )
	{
		return false;
	}
}
//...........................................................................

bool TSharedMemoryController::WriteBlock( __int64 inPosition, __int32 inCount, unsigned __int32* inBlock )
{
	try
	{
		_sharedMemoryManager->WriteBlock( pGetActualWritePosition( inPosition, MemoryManagerType ), inCount, inBlock );
		return true;
	}
	catch( ... )
	{
		return false;
	}
}
//...........................................................................

bool TSharedMemoryController::WriteBlock( __int64 inPosition, __int32 inCount, __int64* inBlock )
{
	try
	{
		_sharedMemoryManager->WriteBlock( pGetActualWritePosition( inPosition, MemoryManagerType ), inCount, inBlock );
		return true;
	}
	catch( ... )
	{
		return false;
	}
}
//...........................................................................

bool TSharedMemoryController::WriteBlock( __int64 inPosition, __int32 inCount, unsigned __int64* inBlock )
{
	try
	{
		_sharedMemoryManager->WriteBlock( pGetActualWritePosition( inPosition, MemoryManagerType ), inCount, inBlock );
		return true;
	}
	catch( ... )
	{
		return false;
	}
}
//...........................................................................

bool TSharedMemoryController::WriteBlock( __int64 inPosition, __int32 inCount, float* inBlock )
{
	try
	{
		_sharedMemoryManager->WriteBlock( pGetActualWritePosition( inPosition, MemoryManagerType ), inCount, inBlock );
		return true;
	}
	catch( ... )
	{
		return false;
	}
}
//...........................................................................

bool TSharedMemoryController::WriteBlock( __int64 inPosition, __int32 inCount, double* inBlock )
{
	try
	{
		_sharedMemoryManager->WriteBlock( pGetActualWritePosition( inPosition, MemoryManagerType ), inCount, inBlock );
		return true;
	}
	catch( ... )
	{
		return false;
	}
}
//...........................................................................

//---------------------------------------------------------------------------
//	Private
//---------------------------------------------------------------------------
__int64 TSharedMemoryController::GetTotalCapacity()
{
	return _serverCapacity + _clientCapacity;
}
//...........................................................................

TSharedMemoryManagerType TSharedMemoryController::GetMemoryManagerType()
{
	return _sharedMemoryManagerType;
}
//...........................................................................

void TSharedMemoryController::pSetBeginMemory( __int64 inPosition )
{
	_serverBeginMemory = 0 ;
	_clientBeginMemory = _serverBeginMemory + _serverCapacity;
}
//...........................................................................

__int64 TSharedMemoryController::pGetActualReadPosition( __int64 inPosition, TSharedMemoryManagerType inMemoryManagerType )
{
	__int64 result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	if( inMemoryManagerType == smmtServer )
	{
		result = _clientBeginMemory + inPosition;

		if ( !pPosInClientMemory( result ) )
		{
			throw new Exception("Read out of the Client memory zone"); // Read out of the Server memory zone
		}
	}
	else
	{
		result = _serverBeginMemory + inPosition;

		if( !pPosInServerMemory( result ) )
		{
			throw new Exception("Read out of the Server memory zone"); // Read out of the Client memory zone
		}
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//...........................................................................

__int64 TSharedMemoryController::pGetActualWritePosition( __int64 inPosition, TSharedMemoryManagerType inMemoryManagerType )
{
	__int64 result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	if( inMemoryManagerType == smmtServer )
	{
		result = _serverBeginMemory + inPosition;

		if( !pPosInServerMemory( result ) )
		{
			throw new Exception("Write out of the Server memory zone"); // Write out of the Server memory zone
		}
	}
	else
	{
		result = _clientBeginMemory + inPosition;

		if( !pPosInClientMemory( result ) )
		{
			throw new Exception("Write out of the Client memory zone"); // Write out of the Client memory zone
		}
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//...........................................................................

bool TSharedMemoryController::pPosInServerMemory( __int64 inPosition )
{
	return ( inPosition >= _serverBeginMemory ) && ( inPosition < ( _serverBeginMemory + _serverCapacity ) );
}
//...........................................................................

bool TSharedMemoryController::pPosInClientMemory( __int64 inPosition )
{
	return ( inPosition >= _clientBeginMemory ) && ( inPosition < ( _clientBeginMemory + _clientCapacity ) );
}
//...........................................................................

//:::::::::::::::::::::::::::
//   SharedMemControllerBuilder
//:::::::::::::::::::::::::::
//---------------------------------------------------------------------------
//	Public
//---------------------------------------------------------------------------
TSharedMemControllerBuilder::TSharedMemControllerBuilder()
{
}
//...........................................................................

TSharedMemoryController* TSharedMemControllerBuilder::BuildServerController( TSharedMemoryType inSharedMemType, UnicodeString inName, __int64 inServerCapacity, __int64 inClientCapacity )
{
	TSharedMemoryController *result = new TSharedMemoryController();

	//:::::::::::::::::::::::::::::::::::::::::

	result->InitServer( inSharedMemType, inName, inServerCapacity, inClientCapacity );

	//:::::::::::::::::::::::::::::::::::::::::

	return result;
}
//...........................................................................

TSharedMemoryController* TSharedMemControllerBuilder::BuildClientController( TSharedMemoryType inSharedMemType, UnicodeString inName, __int64 inServerCapacity, __int64 inClientCapacity )
{
	TSharedMemoryController *result = new TSharedMemoryController();

	//:::::::::::::::::::::::::::::::::::::::::

	result->InitClient( inSharedMemType, inName, inServerCapacity, inClientCapacity );

	//:::::::::::::::::::::::::::::::::::::::::

	return result;
}
//...........................................................................

TSharedMemoryController* TSharedMemControllerBuilder::BuildServerControllerMemMappedFile( UnicodeString inName, __int64 inServerCapacity, __int64 inClientCapacity )
{
	TSharedMemoryController *result = new TSharedMemoryController();

	//:::::::::::::::::::::::::::::::::::::::::

	result->InitServer( smtFile, inName, inServerCapacity, inClientCapacity );

	//:::::::::::::::::::::::::::::::::::::::::

	return result;
}
//...........................................................................

TSharedMemoryController* TSharedMemControllerBuilder::BuildClientControllerMemMappedFile( UnicodeString inName, __int64 inServerCapacity, __int64 inClientCapacity )
{
	TSharedMemoryController *result = new TSharedMemoryController();

	//:::::::::::::::::::::::::::::::::::::::::

	result->InitClient( smtFile, inName, inServerCapacity, inClientCapacity );

	//:::::::::::::::::::::::::::::::::::::::::

	return result;
}
//...........................................................................

TSharedMemoryController* TSharedMemControllerBuilder::BuildServerController( UnicodeString inName, __int64 inServerCapacity, __int64 inClientCapacity )
{
	TSharedMemoryController *result = new TSharedMemoryController();

	//:::::::::::::::::::::::::::::::::::::::::

	result->InitServer( smtMemory, inName, inServerCapacity, inClientCapacity );

	//:::::::::::::::::::::::::::::::::::::::::

	return result;
}
//...........................................................................

TSharedMemoryController* TSharedMemControllerBuilder::BuildClientController( UnicodeString inName, __int64 inServerCapacity, __int64 inClientCapacity )
{
	TSharedMemoryController *result = new TSharedMemoryController();

	//:::::::::::::::::::::::::::::::::::::::::

	result->InitClient( smtMemory, inName, inServerCapacity, inClientCapacity );

	//:::::::::::::::::::::::::::::::::::::::::

	return result;
}
//...........................................................................

