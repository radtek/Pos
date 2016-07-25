//---------------------------------------------------------------------------
#ifndef BeamingH
#define BeamingH

//	Baudrates
#ifdef IrDA
#define CradleBaudRate						57600//38400//57600
#define BeamBaudRate							57600//38400//57600
#else
#define CradleBaudRate						19200
#define BeamBaudRate							19200
#endif

//	Buffer Sizes
#define MyReceiveBuffSize					1024//2000 + DLMaxPacketDataSize*2 + 35	//	Must have 33 bytes overhead
#define MyDLReceiveBuffSize				DLMaxPacketDataSize*2

#ifdef DebugFakeMenu
	#define MyALReceiveBuffSize			30000
#else
//	43000 seems to be max size for Palm III
//	50000 for Palm IIIx
	#ifdef PalmZire
		#define MyALReceiveBuffSize		20000
	#else
		#define MyALReceiveBuffSize		40000
	#endif
#endif

#define PrgMinMessageLength				500

#define MyDLSendBuffSize					64
#define MySendBuffSize						10000
#define MySendBuffMargin					200

//	HL Message Header
#define HLHeaderSize							10
#define HLMaxMessageDataSize				MySendBuffSize - HLHeaderSize - 4
#define HLHdrCommand							0
#define HLHdrTransNo							1
#define HLHdrInfoSync						2
#define HLHdrMenuSync						3
#define HLHdrDrinkSync						4
#define HLHdrStaffID							5
#define HLHdrErrCode							7
#define HLHdrByteCount						8

//	DL Packet Header
#define DLStartFlag							0x7E
#define DLEscapeCharacter					0x7D
#define DLMaxPacketDataSize				46
#define DLPacketPaddingCharacter			' '
#define DLSlaveAddressHigh					0
#define DLSlaveAddressLow					1
/*
//	Attempts and Timeouts
#define DLConnectAttempts					3
#define DLConnectTimeout					5	//	=50ms
#define DLSingleAttempts					3
#define DLSingleTimeout						50	//	=500ms
#define IrBusyResponses						5
#define HLMessageAttempts					3
#define HLMaxNullMessages					5
*/
//	Attempts and Timeouts
#define DLConnectAttempts					3//3
#define DLConnectTimeout					1//2	//	SysTicksPerSecond()/DLConnectTimeout

#define DLSingleAttempts					10//3
#define DLRxFirstByteTimeout				1//1	//	SysTicksPerSecond()/DLRxFirstByteTimeout

#define DLRxInterByteTimeout				100//3	//	SysTicksPerSecond()/DLRxInterByteTimeout

#define BtSerialOpenAttempts				2	//	NB Tries one extra time if only 1 Bt device listed
#define BtRxFirstByteTimeout				1000	//	Default Timeout in system ticks
#define BtRxInterByteTimeout				100//3	//	SysTicksPerSecond()/DLRxInterByteTimeout

#define BtSendOrdersTimeoutPerOrder		250	//	Default Timeout in ms per order!
#define BtSendOrdersTimeoutOffset		500	//	y = mx + c. This is c, in SysTicks
#define BtSendOrdersTimeoutMax			2000	//	Default Maximum Timeout for debug only
#define BtSendOrdersTimeout				2000	//	Default Timeout (obsolete)
#define BtHLBtTimeoutTimeout				200	//	Time out the HLBtTimeout message quickly
#define BtGetAllOrdersTimeout				2000	//	Default Total transmission time in system ticks
	
#define IrBusyResponses						1000//1000//4
#define IrNoResponseAttempts				3
#define HLMessageAttempts					200//3
#define IrTrxReqAttempts			

#define BtMessageAttempts					1

typedef enum hLCommand						//	High level command options
{	
	HLRegister = 0xE0,						//	Must be first item in list
	HLInfoSync,
	HLMenuSync,
	HLDrinkSync,
	HLLogin,			//	NB: IF ADDING MESSAGE TYPES !!! See below
	HLSendSeatnames,
	HLBtTimeout,
	HLSendOrders,
	HLCallAway,
	HLBillSeats,
	HLTransferTables,
	HLGetAllOrders,
	HLEnd											//	Must be last item in list
}HLCommand;
//	NB: IF ADDING MESSAGE TYPES !!!!!!!!!!!!!!!
//	Also change
//		HLStatusType in MenuMate.h
//		Change MmResetHLStatusToAllDone()





typedef enum hLStatusOptions				//	High level message Status options
{
	HLStatusAllDone = 0,
	HLStatusToSend,
	HLStatusReadyToSend,
	HLStatusIsSent,
	HLStatusIsConfirmed,
	HLStatusNegativeAck,
	HLStatusNotLoggedIn
}HLStatusOptions;





typedef enum hLMessageErrors				//	High level message Errors from server
{
	HLErrDisplayErrorMsg = 1,
	HLErrInvalidMessageCRC,
	HLErrCorruptMessage,
	HLErrMismatchMessageData,
	HLErrInfoNotSyncError,
	HLErrDishesNotSyncError,
	HLErrDrinksNotSyncError,
	HLErrServerDBError,
	HLErrNotLoggedIn,
	HLErrWrongPIN,
	HLErrUserNotFound,
	HLErrMaxUsersReached,
	HLErrPalmIDNotFound,
	HLErrWrongRegistrationPIN,
	HLErrNotInRegistrationMode,
	HLErrNoPrinterFound,		//	Warning! Throws out, so GetAllOrders doesn't execute
	HLErrWrongVersionNum,
	HLErrUnspecifiedError,

	//	Warnings
	HLWarnWarning = 128,
	HLWarnDisplayWarningMsg,
	HLWarnNoMenuFound,
	HLWarnPrintingRedirected,
	HLWarnPrinterOffline,
	HLWarnDuplicateOrder,
	HLWarnCallAwayPrinterNotSetUp,
	HLWarnKitchenPrinterNotSetUp,

	// Palm to server errors
	HLErrToServer = 200,
	
	HLErrResponseOkay = 255
}HLMessageErrors;



typedef enum beamingThrowErrors			//	Beaming Errors from server
{
	HLErrMessDisplayErrorMsg = 1,
	HLErrMessInvalidMessageHostCRC,
	HLErrMessInvalidMessagePalmCRC,
	HLErrMessCorruptMessage,
	HLErrMessMismatchMessageData,
	HLErrMessDishesNotSyncError,
	HLErrMessDrinksNotSyncError,
	HLErrMessServerDBError,
	HLErrMessInvalidMessageNumber,
	HLErrMessNullMessageExpected,

//	Non HLMessage Errors Follow
	HLErrMessUnknownResponseType,
	HLErrMessBeamingFailure,
	HLErrMessBeamingTooLarge,
	HLErrMessNotLoggedIn,
	HLErrMessWrongPIN,
	HLErrMessUserNotFound,
	HLErrMessMaxUsersReached,
	HLErrMessPalmIDNotFound,
	HLErrMessWrongRegistrationPIN,
	HLErrMessNotInRegistrationMode,
	HLErrMessNoDishesMenuFound,
	HLErrMessNoDrinksMenuFound,
	HLErrMessNoPrinterFound,
	HLErrMessWrongVersionNum,
	HLErrMessUnspecifiedError,

//	Message Parse Errors Follow
	HLParseOkay = 50,
	HLParseErrorLogin,
	HLParseErrorInfo,
	HLParseInfoSyncNoID,
	HLParseInfoSyncBadID,
	HLParseErrorMenuNoSizes,
	HLParseErrorMenuNoServingCourses,
	HLParseNotValidOrder,
	HLParseInvalidSetMenuNumber,


//	Message Assemble Errors Follow
	HLAssembleMessageOkay = 100,
	HLAssembleOrdersTooMany,
	HLAssembleOrdersMoreToCome,
	HLAssembleOrdersTableTooHigh,
	HLAssembleOrdersSeatTooHigh,
	HLAssemblePalmUniqueIDNoID,
	HLAssemblePalmRegNoRegName,


	HLErrMessOkay = 255,

//	Bluetooth Connection errors
	BtErrNoDevices = appErrorClass,
	BtErrCancelled,
	BtErrNoDeviceSelected,
	BtErrBtLibOpenClose,
	
	BtErrPortOpen,
	BtErrPortClose,

	BtLibErrNoAddress,

	BtErrOkay

}BeamingThrowErrors;


typedef enum talkErrorCodes
{
	TalkOkay = 0,
	TalkNoConnect,
	TalkBusy,
	TalkTooLarge,
	TalkNack,
	TalkWrongPalm,
	TalkFail
}TalkErrorCodes;


typedef enum appCommand						//	Application level command options
{
	SlaveType,
	SoftwareVersion,
	HardwareVersion,
	SerialNumber,
	SlaveAddress,
	InterfaceVersion,
	PcTrxReq = 50,
	PcTrxData,
	PcTrxNoData,
	PcDisReq,
	PcTrxAck,
	IrTrxReq = 100,
	IrTrxData,
	IrTrxNoData,
	IrDisReq
}AppCommand;

typedef enum appResponse		//	Application level response options
{
	SlaveAckR,
	SlaveNackR,
	SlaveAckNoDataR,
	SlaveReservedR,
	SlaveInvalidDataR,
	SlaveInvalidCommandR,
	SlaveBusyR = 7
}AppResponse;

typedef enum linkControl
{
	DLConnect,
	DLDisconnect,
	DLSingle = 5,
	DLAcknowledge,
	DLBufferOverflow,
	DLError
}LinkControl;


/*//	High Level message stuff
typedef struct hLMessageHeader
{
	UInt8 commandWord;
	UInt8 menuSyncNumber;
	UInt8 staffIdNumber;
	UInt8 statusError;
	UInt16  dataCount;
}HLMessageHeader;

typedef struct hLMessage
{
 	HLMessageHeader HLHeader;			// header;
	Char data[MySendBuffSize];
	UInt16 CRC;
}HLMessage;

typedef HLMessageHeader *HLMessageHeaderPtr;
typedef HLMessage *HLMessagePtr;
*/

//	DataLink Layer stuff
typedef struct controlCharacter
{
	UInt16 slaveAddressLow	: 8;
	UInt16 slave				: 1;
	UInt16 reply				: 1;
	UInt16 sequence			: 1;
	UInt16 reserved			: 1;	// Reserved.
	UInt16 dataLinkControl	: 4;
}ControlCharacter;

typedef struct dataLinkHeader
{
	UInt8 startFlag;
	UInt8 slaveAddressHigh;
	ControlCharacter control;
	UInt8 dataCount;
}DataLinkHeader;

typedef struct dLPacket
{
 	DataLinkHeader DLHeader;			// header;
	UInt8 data[DLMaxPacketDataSize];
	UInt16 CRC;
}DLPacket;

typedef DataLinkHeader *DataLinkHeaderPtr;
typedef DLPacket *DLPacketPtr;


typedef struct btMessageHeader
{
	UInt8 startFlag;
	UInt8 verNumShort[6];
	UInt16 PalmID;
	UInt16 btTimeoutSendOrders;
	UInt16 btTimeoutGetAllOrders;
	UInt16 dataCount;
}BtMessageHeader;
typedef BtMessageHeader *BtMessageHeaderPtr;

//---------------------------------------------------------------------------
#endif

