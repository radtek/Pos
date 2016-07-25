#define EXTERNAL 1

#include "MenuMate.h"

/***********************************************************************
 *
 * FUNCTION:		MmDLAddEscape
 *
 * DESCRIPTION:	Add Escape charcters to outgoing messages.
 *
 * PARAMETERS:		
 *
 * RETURNED:		Nothing.
 *
 ***********************************************************************/
static void MmDLAddEscape(UInt8* *dLSendBuff, UInt8* *ptrPacket, UInt16* bytesToSend,
																		Boolean dLEscapingEnabled)
{
	if(	(**ptrPacket == DLStartFlag)			||			//	0x7E
			(**ptrPacket == DLEscapeCharacter)	||			//	0x7D
		  ((**ptrPacket < ' ') && dLEscapingEnabled))	//	ASCII control characters escaped
	{
//MmPrvSyncLogTraceUI("\nEscaping %x", **ptrPacket);
		*(*dLSendBuff)++ = DLEscapeCharacter;				//	0x7D

		*(*dLSendBuff)++ = *(*ptrPacket)++ ^ 0x20;		//	Complement bit 5 (6th bit)

		(*bytesToSend)++;											//	Increment bytesToSend
	}
	else
	{
		*(*dLSendBuff)++ = *(*ptrPacket)++;					//	Copy data and point to next
	}
}


/***********************************************************************
 *
 * FUNCTION:		MmDLSendPacket
 *
 * DESCRIPTION:	Send a single Ir Packet.
 *
 * PARAMETERS:		
 *
 * RETURNED:		Nothing.
 *
 ***********************************************************************/
//UInt16 MmDLSendPacket(UInt16 serialRefNum, UInt8* dLSendBuff,
UInt16 MmDLSendPacket(UInt16 serialPortID, Boolean UseNewSerialManager, UInt8* dLSendBuff,
							DLPacketPtr ptrThisPacket, Boolean dLEscapingEnabled)
{
	Err					err = 1;
	UInt16				numSent;
	UInt16				packetIndex=1;

//	UInt32				checkBytes;
	
//	UInt16				i;
	

#ifdef DebugDL2
	UInt16				k;
	UInt8					*ptrPacketTemp;
#endif

	UInt8					*ptrPacket;

	UInt8					*ptrDLSendBuff;
	UInt16				dataCount;
	UInt16				bytesToSend;
	

	ptrPacket = (UInt8*)ptrThisPacket;

	ptrDLSendBuff = dLSendBuff;
	dataCount = ptrThisPacket->DLHeader.dataCount;
	bytesToSend = 5 + dataCount + 2;

	*ptrDLSendBuff++ = *ptrPacket++;			//	Copy StartFlag

#ifdef DebugDL2
if(ALCommandType == 101)
{
ptrPacketTemp = ptrPacket;
for(k=0; k<5+dataCount+2; k++)
{
	PrvSyncLogTrace("\nPacket[%d] = %x", k, *ptrPacketTemp);
	ptrPacketTemp++;
}
}	
#endif

	//	Copy header and data, copying escaped characters to the DLSendBuff
	//	Insert Escape character for any StartFlag, escape or control characters
	while(packetIndex < 5)
	{
		//	Add escape characters
		MmDLAddEscape(&ptrDLSendBuff, &ptrPacket, &bytesToSend, dLEscapingEnabled);
		packetIndex++;
	}

	//	Copy the data, providing escaping as required
	ptrPacket = (UInt8*)&ptrThisPacket->data;
	while(packetIndex < 5+dataCount)
	{
		//	Add escape characters
		MmDLAddEscape(&ptrDLSendBuff, &ptrPacket, &bytesToSend, dLEscapingEnabled);
		packetIndex++;
	}

	//	Copy the CRC, providing escaping as required
	ptrPacket = (UInt8*)&ptrThisPacket->CRC;
	while(packetIndex < 5+dataCount+2)
	{
		//	Add escape characters
		MmDLAddEscape(&ptrDLSendBuff, &ptrPacket, &bytesToSend, dLEscapingEnabled);
		packetIndex++;
	}

#ifdef DebugDL2
if(ALCommandType == 101)
{
PrvSyncLogTrace("\nDLSendBuff[x] = ");
for(k=0; k<bytesToSend; k++)
{
	PrvSyncLogTrace(" %x", dLSendBuff[k]);
}
}
#endif

//	SerReceiveFlush(serialRefNum, 0);					//	Trash any garbage


#ifdef DebugDL2
MmPrvSyncLogTraceUI("\n      Txd ALPacket Type = %d", dLSendBuff[5] * 256 + dLSendBuff[6]);
#endif


	//	Send the packet
	if (UseNewSerialManager)
	{
		numSent = SrmSend(serialPortID, dLSendBuff, bytesToSend, &err);
	}
	else
	{
		numSent = SerSend(serialPortID, dLSendBuff, bytesToSend, &err);
	}

	if(err)
	{

#ifdef DebugDL2
MmPrvSyncLogTrace("\n      Transmit Error");
#endif

		if (UseNewSerialManager)
		{
			err = SrmClearErr(serialPortID);
#ifdef DebugDL2
			if (err)
			{
				MmPrvSyncLogTrace("\n      Clear Error Error");
			}
#endif
		}
		else
		{
			SerClearErr(serialPortID);
		}
		return err;
	}

	return 0;
}


/***********************************************************************
 *
 * FUNCTION:		MmDLReceivePacket
 *
 * DESCRIPTION:	Receive a single Ir Packet.
 *
 * PARAMETERS:		
 *
 * RETURNED:		Nothing.
 *
 ***********************************************************************/
UInt16 MmDLReceivePacket(UInt16 serialPortID, Boolean useNewSerialManager,
								UInt8* dLReceiveBuff,	DLPacketPtr ptrThisPacket, UInt16 timeout)
{
	Err					err;

	UInt16				packetEnd;
	Boolean				isEscape = false;
	
	UInt16				calcCRC16, realCRC16;				//	CRC temp values
	UInt16				DLReceiveIndex;
	UInt16				i;
	UInt16				dataLinkHeaderSize;
	
	Boolean				seekingStartFlag;
	UInt8					thisChar;
	
	UInt8					*ptrPacket;
	

	DLReceiveIndex = 0;
	
	//	Ensure buffer is cleared
	for(i=0; i<MyDLReceiveBuffSize; i++)
		dLReceiveBuff[i] = 0;

	dataLinkHeaderSize = sizeof(DataLinkHeader);

	//	Wait for at least a DLHeader's worth of chars
	if(useNewSerialManager)
	{
		err = SrmReceiveWait(serialPortID, dataLinkHeaderSize, timeout);
	}
	else
	{
		err = SerReceiveWait(serialPortID, dataLinkHeaderSize, timeout);
	}

	seekingStartFlag = true;

	if(!err)
	{
		packetEnd = dataLinkHeaderSize + 1;			//	This will update when we get a UInt8 count
		
		//	Search for the '~' = Start of Packet
		do
		{
			//	Get a character from the port
			if (useNewSerialManager)
			{
				SrmReceive(serialPortID, &thisChar, 1, timeout, &err);
#ifdef DebugDL2
if (SysRandom(0) % 100 > 99) 
{
	err = 774;
	MmPrvSyncLogTrace("Forced Error");					
}
#endif
			}
			else
			{
				SerReceive(serialPortID, &thisChar, 1, timeout, &err);
			}

			if(err)
			{
				break;											//	Damn. Abandon Ship!
			}

			//	If its a start flag, trash everything and start over again
			if(thisChar == DLStartFlag)
			{
				seekingStartFlag = false;

				dLReceiveBuff[0] = DLStartFlag;
				DLReceiveIndex = 1;
				timeout = SysTicksPerSecond()/DLRxInterByteTimeout;
				continue;										//	Crash back to start for next char
			}

			//	If its the Control Char. in the Header
			if(DLReceiveIndex == 3)
			{
				//	If it wasn't a slave response, we don't want it. So start again.
				if(!(thisChar & 0x80))
				{
					seekingStartFlag = true;
					dLReceiveBuff[3] = thisChar;
					DLReceiveIndex = 0;
					timeout = SysTicksPerSecond()/DLRxFirstByteTimeout;
				}
			}

			if(!seekingStartFlag)
			{

				//	Look at the UInt8 Count in the Header
				if(DLReceiveIndex == 4)
				{
					packetEnd = 5 + thisChar + 2;				//	Know UInt8 count now
				}

				//	Must be a good char, so copy it to the buffer
				//	Shuffle up to remove escape characters.
				if(thisChar == DLEscapeCharacter && isEscape == false)
				{
					isEscape = true;								//	Trash Char. Next will be escaped
				}
				else if(isEscape == true)
				{
					dLReceiveBuff[DLReceiveIndex] = thisChar^0x20;	//	Complement bit 5 and copy across
					DLReceiveIndex++;
					isEscape = false;
				}
				else
				{
					dLReceiveBuff[DLReceiveIndex] = thisChar;
					DLReceiveIndex++;
				}
			}
		}while(DLReceiveIndex < packetEnd);
	}


	if(err)
	{
		if (err == serErrLineErr)
		{
			if (useNewSerialManager)
			{
#ifdef DebugDLMike
UInt32 StatusField;
UInt16 LineErrors;
SrmGetStatus(serialPortID,&StatusField,&LineErrors);
MmPrvSyncLogTraceUI("Line Error %d",LineErrors);				
#endif
				SrmClearErr (serialPortID);
				SrmReceiveFlush(serialPortID, 0);
			}
			else
			{
				SerClearErr (serialPortID);
				SerReceiveFlush(serialPortID, 0);
			}
		}
		return err;
	}

	//	CRC16 calculation stuff. Step through the message up to (not including) CRC.
	calcCRC16 = StartCRC(dLReceiveBuff[0]);
	for(i=1; i < DLReceiveIndex-2; i++)
	{
		calcCRC16 = UpdateCRC(calcCRC16, dLReceiveBuff[i]);
	}
	realCRC16 = (dLReceiveBuff[DLReceiveIndex-2]*256) + dLReceiveBuff[DLReceiveIndex-1];

	if(calcCRC16 != realCRC16)
	{
		//	All stuffed up: Start again
		return NoValue16;
	}

	//	Copy the received response into the packet fields
	ptrPacket = (UInt8*)&ptrThisPacket->DLHeader;

	//	Copy the header straight across
	i=0;
	while(i < 5)
	{
		*ptrPacket++ = dLReceiveBuff[i++];
	}

	//	Now copy the data
	ptrPacket = (UInt8*)&ptrThisPacket->data[0];
	while(i < 5 + dLReceiveBuff[4])
	{
		*ptrPacket++ = dLReceiveBuff[i++];
	}

	//	Now copy the CRC
	ptrPacket = (UInt8*)&ptrThisPacket->CRC;
	*ptrPacket++ = calcCRC16 >> 8;							//	Copy high UInt8 of CRC
	*ptrPacket++ = calcCRC16 & 0xFF;							//	Copy low UInt8 of CRC
	

	return 0;
}


/***********************************************************************
 *
 * FUNCTION:	  MmDLConnect
 *
 * DESCRIPTION:  Perform a Datalink layer connect.
 *
 * PARAMETERS:	
 *
 * RETURNED:	  Boolean
 *
 ***********************************************************************/
Boolean MmDLConnect(UInt16 serialPortID, Boolean UseNewSerialManager, UInt8 *dLSendBuff,
				UInt8 *dLReceiveBuff, Boolean *dLSequenceNo, Boolean *dLEscapingEnabled)
{
	DLPacket					writePacket;				//	Locally assembled Send packet
	DLPacket				 	readPacket;					//	Locally assembled Received packet
	UInt16					retryCount = 0;
//	UInt16					i;

	//	Set up packet header
	*dLSequenceNo = 0;
//	MmDLPacketHeader(DLHeaderPtr, slave, reply, sequence, dataLinkControl, dataCount)
	MmDLFillPacketHeader(&writePacket.DLHeader, 0, 1, *dLSequenceNo, DLConnect, 1);
	
	//Disable escaping by setting global, and writing as only data UInt8
	*dLEscapingEnabled = false;
	writePacket.data[0] = *dLEscapingEnabled;

	// Generate CRC.
	writePacket.CRC = MmDLCalcCRC(&writePacket);

	while(retryCount++ < DLConnectAttempts)		//	Limit the number of connect attempts
	{

		if(!MmDLSendPacket(serialPortID, UseNewSerialManager, dLSendBuff, &writePacket, *dLEscapingEnabled))
		{
			if(!MmDLReceivePacket(serialPortID, UseNewSerialManager, dLReceiveBuff,
									&readPacket, SysTicksPerSecond()/DLConnectTimeout))
			{
				if(!MmDLValidatePacket(&readPacket, DLConnect))
				{
					return false;							//	Connection successful
				}
			}
		}
	}
	return true;
}


/***********************************************************************
 *
 * FUNCTION:	  MmDLDisconnect
 *
 * DESCRIPTION:  Perform a Datalink layer disconnect.
 *
 * PARAMETERS:	
 *
 * RETURNED:	  Boolean
 *
 ***********************************************************************/
//Boolean MmDLDisconnect(UInt16 serialRefNum, UInt8* dLSendBuff, UInt8* dLReceiveBuff,
Boolean MmDLDisconnect(UInt16 serialPortID, Boolean UseNewSerialManager, UInt8* dLSendBuff, UInt8* dLReceiveBuff,
										Boolean *dLSequenceNo, Boolean *dLEscapingEnabled)
{
	DLPacket					writePacket;				//	Locally assembled Send packet
	DLPacket				 	readPacket;					//	Locally assembled Received packet
	UInt16					retryCount = 0;

	//	Set up packet header
	*dLSequenceNo = 0;
//	MmDLPacketHeader(DLHeaderPtr, slave, reply, sequence, dataLinkControl, dataCount)
	MmDLFillPacketHeader(&writePacket.DLHeader, 0, 1, *dLSequenceNo, DLDisconnect, 0);
	
	//Disable escaping by setting global, and writing as only data byte
	*dLEscapingEnabled = false;
	writePacket.data[0] = *dLEscapingEnabled;

	// Generate CRC.
	writePacket.CRC = MmDLCalcCRC(&writePacket);

	while(retryCount++ < DLConnectAttempts)		//	Limit the number of disconnect attempts
	{
//PrvSyncLogTrace("\n  DL DisConnect Attempt %d", retryCount);
//		if(!MmDLSendPacket(serialRefNum, dLSendBuff, &writePacket, *dLEscapingEnabled))
		if(!MmDLSendPacket(serialPortID, UseNewSerialManager, dLSendBuff, &writePacket, *dLEscapingEnabled))
		{
			// Retrieve Acknowledge (or crapout).
//			if(!MmDLReceivePacket(serialRefNum, dLReceiveBuff,
			if(!MmDLReceivePacket(serialPortID, UseNewSerialManager, dLReceiveBuff,
									&readPacket, SysTicksPerSecond()/DLConnectTimeout))
			{
				// Check Acknowledge. Move into new header.
				if(!MmDLValidatePacket(&readPacket, DLDisconnect))
				{
//PrvSyncLogTrace("\n  DisConnect Success");
					return false;							//	Disconnection successful
				}
			}
		}
	}
	return false;
}


/***********************************************************************
 *
 * FUNCTION:	  MmDLCalcCRC
 *
 * DESCRIPTION:  Calculate a Datalink layer CRC.
 *
 * PARAMETERS:	
 *
 * RETURNED:	  
 *
 ***********************************************************************/
 UInt16 MmDLCalcCRC(DLPacketPtr ptrThisPacket)
 {
	UInt16						i=1;
	UInt16						dataCount;
	UInt16 						CRC = 0;
	UInt8							*ptrWritePacket;			//	To step through packet

	ptrWritePacket = (UInt8*)ptrThisPacket;
	CRC =  StartCRC(ptrWritePacket[0]);
	for(i=1; i < 5; i++)					 				//	Dont include data or CRC field.
	{
		CRC = UpdateCRC(CRC, ptrWritePacket[i]);
	}
	
	dataCount = ptrWritePacket[i-1];					//	Find how much data there is

	ptrWritePacket = (UInt8*)&ptrThisPacket->data[0];
	for(i=0; i < dataCount; i++)				 		//	Do data field.
	{
		CRC = UpdateCRC(CRC, ptrWritePacket[i]);
	}
	return CRC;
}


