#ifndef CommsH
#define CommsH

#include <Printers.hpp>
#include "Printing.h"
#include "CPortCtl.hpp"
#include "CPort.hpp"
#include "MMLogging.h"    // Error Handling.

enum SLAVE_COMMANDS {SLAVE_ACK_R, SLAVE_NACK_R, SLAVE_ACK_NO_DATA_R, SLAVE_PRN_ERROR_R, SLAVE_INVALID_DATA_R,
							SLAVE_INVALID_COMMAND_R, SLAVE_BUSY_R = 7,SLAVE_NO_RESPONSE,SLAVE_CORRUPT_DATA};

class TComms
{
private:
	#ifdef  PalmMate
	// Orders the List of TConnect by address.
	OrderByAddress();
	#endif
	int PrinterIndex;

	//CONSTRUCTOR.
	__fastcall TComms();

public:

    static TComms& Instance() {
        static TComms singleton;
        return singleton;
    }

	__fastcall TComms::~TComms();

	int MaxBusyCount;

	//Creates a TConnect and attemps to conect it to the address & add it to TList of connections.
	void InitialiseWinConnect();

	//DEBUG TEMPERORY CODE:
	bool StartLogging;
	bool MonitorEnabled;

	// Will Install the Connections in the Database into the TConnectLst.
	void _fastcall LoadConnectLst();

	void _fastcall Initialise(Database::TDBTransaction &DBTransaction);

	// Inbound Data for the processing level.
	TMemoryStream *Process_Inbound;
	TMemoryStream *Process_Outbound;

	TPrinterPhysical ReceiptPrinter;
// RecievData Datalink Level Time Out Retry Count.
//	Default = 100.
// Set to 5 During Connect autodection for speed.
	void KickLocalDraw(Database::TDBTransaction &DBTransaction);

	TComPort *SerialKickPort;
	char SerialKickPortNumber;
	int SerialKickPort_Length;
	bool SerialKickPortHighChars;
};


#endif

