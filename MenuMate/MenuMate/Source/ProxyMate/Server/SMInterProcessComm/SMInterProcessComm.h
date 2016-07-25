//---------------------------------------------------------------------------

#ifndef SMInterProcessCommH
#define SMInterProcessCommH

//---------------------------------------------------------------------------

#include <vcl.h>

#include "SharedMemory.h"
#include "MessageReceiver.h"

//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::
//   TSMInterProcessComm
//:::::::::::::::::::::::::::
class TSMInterProcessComm : public TMsgReceiver
{
protected:
	bool _active;

	public:
	__fastcall TSMInterProcessComm();
	__fastcall ~TSMInterProcessComm();

	virtual bool Init( UnicodeString inName, __int64 inServerCapacity, __int64 inClientCapacity );

	virtual void Done(); // Disposes of the shared memory

	__property bool Active = { read=_active };

private:
	void publishHWNDHandle( HWND inHandle );

protected:
	__int64 _initPosition;

	TSharedMemoryController *_sharedMemoryController;

	virtual TSharedMemoryController* createSharedMemoryController( UnicodeString inName, __int64 inServerCapacity, __int64 inClientCapacity );
};

//:::::::::::::::::::::::::::
//   TSMInterProcessCommServer
//:::::::::::::::::::::::::::
class TSMInterProcessCommServer : public TSMInterProcessComm
{
public:
	__fastcall TSMInterProcessCommServer();
	__fastcall ~TSMInterProcessCommServer();

protected:
	__property HWND ClientWMsgHandle = { read=GetClientWMsgHandle };

	TSharedMemoryController* createSharedMemoryController( UnicodeString inName, __int64 inServerCapacity, __int64 inClientCapacity );

	HWND GetClientWMsgHandle();
};

//:::::::::::::::::::::::::::
//   TSMInterProcessCommClient
//:::::::::::::::::::::::::::
class TSMInterProcessCommClient : public TSMInterProcessComm
{
public:
	__fastcall TSMInterProcessCommClient();
	__fastcall ~TSMInterProcessCommClient();

protected:
	__property HWND ServerWMsgHandle = { read=GetServerWMsgHandle };

	TSharedMemoryController* createSharedMemoryController( UnicodeString inName, __int64 inServerCapacity, __int64 inClientCapacity );

	HWND GetServerWMsgHandle();
};

//---------------------------------------------------------------------------
#endif

//0xC000 through 0xFFFF  _INVALID_HWND
