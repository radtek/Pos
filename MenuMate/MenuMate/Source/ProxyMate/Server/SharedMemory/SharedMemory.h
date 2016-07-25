//---------------------------------------------------------------------------

#ifndef SharedMemoryH
#define SharedMemoryH
//---------------------------------------------------------------------------

#include <vcl.h>

//---------------------------------------------------------------------------

enum TSharedMemoryManagerType
{
   smmtServer, smmtClient
};

enum TSharedMemoryType
{
   smtMemory, smtFile
};

//:::::::::::::::::::::::::::
//   TSharedMemoryManager
//:::::::::::::::::::::::::::
class TSharedMemoryManager : public TObject
{
	HANDLE  _hMapFile;
	LPCTSTR _pBuf;

	TSharedMemoryManagerType _smmType;
	TSharedMemoryType        _smType;

public:
	__fastcall TSharedMemoryManager();
	__fastcall ~TSharedMemoryManager();

	bool Init( TSharedMemoryManagerType inType, TSharedMemoryType inShareType, UnicodeString inName, __int32 inCapacity );

	BYTE             ReadByte(   __int64 inPosition );
	__int16          ReadInt16(  __int64 inPosition );
	unsigned __int16 ReadUInt16( __int64 inPosition );
	__int32          ReadInt32(  __int64 inPosition );
	unsigned __int32 ReadUInt32( __int64 inPosition );
	__int64          ReadInt64(  __int64 inPosition );
	unsigned __int64 ReadUInt64( __int64 inPosition );
	float            ReadSingle( __int64 inPosition );
	double           ReadDouble( __int64 inPosition );

	__int32 ReadBlock( __int64 inPosition, __int32 inCount, BYTE*             outBlock );
	__int32 ReadBlock( __int64 inPosition, __int32 inCount, __int16*          outBlock );
	__int32 ReadBlock( __int64 inPosition, __int32 inCount, unsigned __int16* outBlock );
	__int32 ReadBlock( __int64 inPosition, __int32 inCount, __int32*          outBlock );
	__int32 ReadBlock( __int64 inPosition, __int32 inCount, unsigned __int32* outBlock );
	__int32 ReadBlock( __int64 inPosition, __int32 inCount, __int64*          outBlock );
	__int32 ReadBlock( __int64 inPosition, __int32 inCount, unsigned __int64* outBlock );
	__int32 ReadBlock( __int64 inPosition, __int32 inCount, float*            outBlock );
	__int32 ReadBlock( __int64 inPosition, __int32 inCount, double*           outBlock );

	void Write( __int64 inPosition, BYTE             inValue );
	void Write( __int64 inPosition, __int16          inValue );
	void Write( __int64 inPosition, unsigned __int16 inValue );
	void Write( __int64 inPosition, __int32          inValue );
	void Write( __int64 inPosition, unsigned __int32 inValue );
	void Write( __int64 inPosition, __int64          inValue );
	void Write( __int64 inPosition, unsigned __int64 inValue );
	void Write( __int64 inPosition, float            inValue );
	void Write( __int64 inPosition, double           inValue );

	void WriteBlock( __int64 inPosition, __int32 inCount, byte*             inBlock);
	void WriteBlock( __int64 inPosition, __int32 inCount, __int16*          inBlock );
	void WriteBlock( __int64 inPosition, __int32 inCount, unsigned __int16* inBlock );
	void WriteBlock( __int64 inPosition, __int32 inCount, __int32*          inBlock );
	void WriteBlock( __int64 inPosition, __int32 inCount, unsigned __int32* inBlock );
	void WriteBlock( __int64 inPosition, __int32 inCount, __int64*          inBlock );
	void WriteBlock( __int64 inPosition, __int32 inCount, unsigned __int64* inBlock );
	void WriteBlock( __int64 inPosition, __int32 inCount, float*            inBlock );
	void WriteBlock( __int64 inPosition, __int32 inCount, double*           inBlock );

private:

	bool pInitServer( TSharedMemoryType inShareType, UnicodeString inName, __int64 inCapacity );
	bool pInitClient( TSharedMemoryType inShareType, UnicodeString inName, __int64 inCapacity );

	bool pInitServerSharedMemMappedFile( UnicodeString inName, __int64 inCapacity );
	bool pInitServerSharedMem(           UnicodeString inName, __int64 inCapacity );

	bool pInitClientSharedMemMappedFile( UnicodeString inName, __int64 inCapacity );
	bool pInitClientSharedMem(           UnicodeString inName, __int64 inCapacity );

	BYTE             pReadByteFromSharedMem(   PVOID inSharedMem, __int64 inPosition );
	__int16          pReadInt16FromSharedMem(  PVOID inSharedMem, __int64 inPosition );
	unsigned __int16 pReadUInt16FromSharedMem( PVOID inSharedMem, __int64 inPosition );
	__int32          pReadInt32FromSharedMem(  PVOID inSharedMem, __int64 inPosition );
	unsigned __int32 pReadUInt32FromSharedMem( PVOID inSharedMem, __int64 inPosition );
	__int64          pReadInt64FromSharedMem(  PVOID inSharedMem, __int64 inPosition );
	unsigned __int64 pReadUInt64FromSharedMem( PVOID inSharedMem, __int64 inPosition );
	float            pReadSingleFromSharedMem( PVOID inSharedMem, __int64 inPosition );
	double           pReadDoubleFromSharedMem( PVOID inSharedMem, __int64 inPosition );

	__int32 pReadBlockFromSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, byte*               outBlock );
	__int32 pReadBlockFromSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, __int16*          outBlock );
	__int32 pReadBlockFromSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, unsigned __int16* outBlock );
	__int32 pReadBlockFromSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, __int32*                outBlock );
	__int32 pReadBlockFromSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, unsigned __int32*       outBlock );
	__int32 pReadBlockFromSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, __int64*           outBlock );
	__int32 pReadBlockFromSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, unsigned __int64*  outBlock );
	__int32 pReadBlockFromSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, float*              outBlock );
	__int32 pReadBlockFromSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, double*             outBlock );
	__int32 pReadBlockFromSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inSize,  PVOID               outBlock );

	void  pWriteByteToSharedMem(   PVOID inSharedMem, BYTE             inValue, __int64 inPosition );
	void  pWriteInt16ToSharedMem(  PVOID inSharedMem, __int16          inValue, __int64 inPosition );
	void  pWriteUInt16ToSharedMem( PVOID inSharedMem, unsigned __int16 inValue, __int64 inPosition );
	void  pWriteInt32ToSharedMem(  PVOID inSharedMem, __int32          inValue, __int64 inPosition );
	void  pWriteUInt32ToSharedMem( PVOID inSharedMem, unsigned __int32 inValue, __int64 inPosition );
	void  pWriteInt64ToSharedMem(  PVOID inSharedMem, __int64          inValue, __int64 inPosition );
	void  pWriteUInt64ToSharedMem( PVOID inSharedMem, unsigned __int64 inValue, __int64 inPosition );
	void  pWriteSingleToSharedMem( PVOID inSharedMem, float            inValue, __int64 inPosition );
	void  pWriteDoubleToSharedMem( PVOID inSharedMem, double           inValue, __int64 inPosition );

	void pWriteBlockToSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, BYTE*             inBlock );
	void pWriteBlockToSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, __int16*          inBlock );
	void pWriteBlockToSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, unsigned __int16* inBlock );
	void pWriteBlockToSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, __int32*          inBlock );
	void pWriteBlockToSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, unsigned __int32* inBlock );
	void pWriteBlockToSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, __int64*          inBlock );
	void pWriteBlockToSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, unsigned __int64* inBlock );
	void pWriteBlockToSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, float*            inBlock );
	void pWriteBlockToSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inCount, double*           inBlock );

	PVOID pReadFromSharedMem( PVOID inSharedMem, __int64 inPosition, __int32 inSize );
	void  pWriteToSharedMem(  PVOID inSource, PVOID inSharedMem, __int64 inPosition, __int32 inSize );

};
//...........................

class TSharedMemControllerBuilder;

//:::::::::::::::::::::::::::
//   TSharedMemoryController
//:::::::::::::::::::::::::::
class TSharedMemoryController : public TObject
{
protected:
	TSharedMemoryManager    *_sharedMemoryManager;

	TSharedMemoryManagerType _sharedMemoryManagerType;

	long _serverCapacity;
	long _clientCapacity;

	long _serverBeginMemory;
	long _clientBeginMemory;

	TSharedMemoryController();

	bool InitServer( TSharedMemoryType inSharedMemType, UnicodeString inName, __int64 inServerCapacity, __int64 inClientCapacity );
	bool InitClient( TSharedMemoryType inSharedMemType, UnicodeString inName, __int64 inServerCapacity, __int64 inClientCapacity );

public:
	friend TSharedMemControllerBuilder;

	__property __int64 TotalCapacity                      = { read=GetTotalCapacity     };
	__property TSharedMemoryManagerType MemoryManagerType = { read=GetMemoryManagerType };

	BYTE             ReadByte(   __int64 inPosition );
	__int16          ReadInt16(  __int64 inPosition );
	unsigned __int16 ReadUInt16( __int64 inPosition );
	__int32          ReadInt32(  __int64 inPosition );
	unsigned __int32 ReadUInt32( __int64 inPosition );
	__int64          ReadInt64(  __int64 inPosition );
	unsigned __int64 ReadUInt64( __int64 inPosition );
	float            ReadSingle( __int64 inPosition );
	double           ReadDouble( __int64 inPosition );

	__int32 ReadBlock( __int64 inPosition, __int32 inCount, BYTE*               outBlock );
	__int32 ReadBlock( __int64 inPosition, __int32 inCount, __int16*          outBlock );
	__int32 ReadBlock( __int64 inPosition, __int32 inCount, unsigned __int16* outBlock );
	__int32 ReadBlock( __int64 inPosition, __int32 inCount, __int32*                outBlock );
	__int32 ReadBlock( __int64 inPosition, __int32 inCount, unsigned __int32*       outBlock );
	__int32 ReadBlock( __int64 inPosition, __int32 inCount, __int64*           outBlock );
	__int32 ReadBlock( __int64 inPosition, __int32 inCount, unsigned __int64*  outBlock );
	__int32 ReadBlock( __int64 inPosition, __int32 inCount, float*              outBlock );
	__int32 ReadBlock( __int64 inPosition, __int32 inCount, double*             outBlock );

	bool Write( __int64 inPosition, BYTE               inValue );
	bool Write( __int64 inPosition, __int16          inValue );
	bool Write( __int64 inPosition, unsigned __int16 inValue );
	bool Write( __int64 inPosition, __int32 				inValue );
	bool Write( __int64 inPosition, unsigned __int32 		inValue );
	bool Write( __int64 inPosition, __int64 			inValue );
	bool Write( __int64 inPosition, unsigned __int64  inValue );
	bool Write( __int64 inPosition, float 				inValue );
	bool Write( __int64 inPosition, double 			inValue );

	bool WriteBlock( __int64 inPosition, __int32 inCount, BYTE* 			   inBlock );
	bool WriteBlock( __int64 inPosition, __int32 inCount, __int16* 		   inBlock );
	bool WriteBlock( __int64 inPosition, __int32 inCount, unsigned __int16* inBlock );
	bool WriteBlock( __int64 inPosition, __int32 inCount, __int32* 			   inBlock );
	bool WriteBlock( __int64 inPosition, __int32 inCount, unsigned __int32* 	   inBlock );
	bool WriteBlock( __int64 inPosition, __int32 inCount, __int64* 		   inBlock );
	bool WriteBlock( __int64 inPosition, __int32 inCount, unsigned __int64*  inBlock );
	bool WriteBlock( __int64 inPosition, __int32 inCount, float* 			   inBlock );
	bool WriteBlock( __int64 inPosition, __int32 inCount, double* 		       inBlock );


private:
	__int64                  GetTotalCapacity();
	TSharedMemoryManagerType GetMemoryManagerType();

	void pSetBeginMemory( __int64 inPosition );

	__int64 pGetActualReadPosition(  __int64 inPosition, TSharedMemoryManagerType inMemoryManagerType );
	__int64 pGetActualWritePosition( __int64 inPosition, TSharedMemoryManagerType inMemoryManagerType );

	bool pPosInServerMemory( __int64 inPosition );
	bool pPosInClientMemory( __int64 inPosition );
};
//...........................

//:::::::::::::::::::::::::::
//   TSharedMemControllerBuilder
//:::::::::::::::::::::::::::
class TSharedMemControllerBuilder
{
public:
	TSharedMemControllerBuilder();

	static TSharedMemControllerBuilder& Instance()
	{
		static TSharedMemControllerBuilder s_instance;

		return s_instance;
	}

	TSharedMemoryController* BuildServerController( TSharedMemoryType inSharedMemType, UnicodeString inName, __int64 inServerCapacity, __int64 inClientCapacity );
	TSharedMemoryController* BuildClientController( TSharedMemoryType inSharedMemType, UnicodeString inName, __int64 inServerCapacity, __int64 inClientCapacity );

	TSharedMemoryController* BuildServerControllerMemMappedFile( UnicodeString inName, __int64 inServerCapacity, __int64 inClientCapacity );
	TSharedMemoryController* BuildClientControllerMemMappedFile( UnicodeString inName, __int64 inServerCapacity, __int64 inClientCapacity );

	TSharedMemoryController* BuildServerController( UnicodeString inName, __int64 inServerCapacity, __int64 inClientCapacity );
	TSharedMemoryController* BuildClientController( UnicodeString inName, __int64 inServerCapacity, __int64 inClientCapacity );
};

#endif
