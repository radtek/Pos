#define EXTERNAL 1

#include "MenuMate.h"

#ifdef DebugDL
/***********************************************************************
 *
 * FUNCTION:	  DisplayRcvdPacket
 *
 * DESCRIPTION:  .
 *
 * PARAMETERS:	
 *
 * RETURNED:	  
 *
 ***********************************************************************/
UInt16 DisplayPacketCounts(Int16 packetsToWrite, Int16 packetsToRead)
{
	FormPtr					frm;
	
	Char						tempStr[35];

//	RectangleType			space;
	UInt16					thisLabel;
	FormLabelType 			*labelPtr;

	frm = FrmGetActiveForm();
	
	thisLabel = BeamingPacketsToWriteLabel;

	labelPtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, thisLabel));
	StrPrintF(tempStr, "%d", packetsToWrite);		// Convert Count to string

	//	Set the Rectangle area to be cleared before the new price is drawn.
//	RctSetRectangle(&space, labelPtr->pos.x, labelPtr->pos.y, 45, 12);
//	WinEraseRectangle(&space, 0);

	FrmHideObject(frm, FrmGetObjectIndex(frm, thisLabel));
	FrmCopyLabel(frm, thisLabel, tempStr);
	FrmShowObject(frm, FrmGetObjectIndex(frm, thisLabel));



	thisLabel = BeamingPacketsToReadLabel;

	labelPtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, thisLabel));
	StrPrintF(tempStr, "%d", packetsToRead);		// Convert Count to string

	//	Set the Rectangle area to be cleared before the new price is drawn.
//	RctSetRectangle(&space, labelPtr->pos.x, labelPtr->pos.y, 45, 12);
//	WinEraseRectangle(&space, 0);

	FrmHideObject(frm, FrmGetObjectIndex(frm, thisLabel));
	FrmCopyLabel(frm, thisLabel, tempStr);
	FrmShowObject(frm, FrmGetObjectIndex(frm, thisLabel));


	return 0;
}


/***********************************************************************
 *
 * FUNCTION:	  DisplayRcvdPacket
 *
 * DESCRIPTION:  .
 *
 * PARAMETERS:	
 *
 * RETURNED:	  
 *
 ***********************************************************************/
UInt16 DisplayRcvdPacket(UInt16 rcvdPackets, DLPacketPtr ptrThisPacket)
{
	FormPtr					frm;
	
	Char						tempStr[35];

	RectangleType			space;
	UInt16					thisLabel;
	FormLabelType 			*labelPtr;
	UInt16					i, j, maxBytes;

	frm = FrmGetActiveForm();

	switch(rcvdPackets)
	{
		case 1:
			thisLabel = BeamingPacket1Label;
			break;

		case 2:
			thisLabel = BeamingPacket2Label;
			break;
			
		case 3:
			thisLabel = BeamingPacket3Label;
			break;

		case 4:
			thisLabel = BeamingPacket4Label;
			break;

	}

	labelPtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, thisLabel));

	//	Convert the data to ASCII character pairs
	maxBytes = ptrThisPacket->DLHeader.dataCount;
	if(maxBytes > 10)
		maxBytes = 10;
		
	for(i=1, j=0; i<maxBytes; i++)
	{
		tempStr[j++] = ((ptrThisPacket->data[i]) >> 4) + '0';
		if(tempStr[j-1] > '9')
			tempStr[j-1] += 7;
		tempStr[j++] = (ptrThisPacket->data[i] & 0x0f) + '0';
		if(tempStr[j-1] > '9')
			tempStr[j-1] += 7;
		tempStr[j++] = 0x20;
	}
	tempStr[j] = 0;							//	Terminate string

	//	Set the Rectangle area to be cleared before the new price is drawn.
//	RctSetRectangle(&space, 1, labelPtr->pos.y, 159, 12);
//	WinEraseRectangle(&space, 0);

	FrmHideObject(frm, FrmGetObjectIndex(frm, thisLabel));
	FrmCopyLabel(frm, thisLabel, tempStr);
	FrmShowObject(frm, FrmGetObjectIndex(frm, thisLabel));
	return 0;
}


/***********************************************************************
 *
 * FUNCTION:	  DisplayPacketCountSent
 *
 * DESCRIPTION:  .
 *
 * PARAMETERS:	
 *
 * RETURNED:	  
 *
 ***********************************************************************/
UInt16 DisplayPacketCountSent(UInt16 sentPackets, DLPacketPtr ptrThisPacket)
{
	FormPtr					frm;
	
//	Char						priceStr[11] = "          ";
	Char						tempStr[10];
//	UInt8*					sourcePtr, destPtr;

//	RectangleType			space;
	UInt16					thisLabel;
	UInt16					seqLabel;
	FormLabelType 			*labelPtr;

	frm = FrmGetActiveForm();

	switch(sentPackets)
	{
		case 1:
			thisLabel = BeamingSent1Label;
			seqLabel = BeamingSentSeq1Label;
			break;

		case 2:
			thisLabel = BeamingSent2Label;
			seqLabel = BeamingSentSeq2Label;
			break;

		case 3:
			thisLabel = BeamingSent3Label;
			seqLabel = BeamingSentSeq3Label;
			break;

		case 4:
			thisLabel = BeamingSent4Label;
			seqLabel = BeamingSentSeq4Label;
			break;

		case 5:
			thisLabel = BeamingSent5Label;
			seqLabel = BeamingSentSeq5Label;
			break;
	}

	labelPtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, thisLabel));
	StrPrintF(tempStr, "%d", ptrThisPacket->data[1]);		// Convert Count to string

	//	Set the Rectangle area to be cleared before the new price is drawn.
//	RctSetRectangle(&space, labelPtr->pos.x, labelPtr->pos.y, 45, 12);
//	WinEraseRectangle(&space, 0);

	FrmHideObject(frm, FrmGetObjectIndex(frm, thisLabel));
	FrmCopyLabel(frm, thisLabel, tempStr);
	FrmShowObject(frm, FrmGetObjectIndex(frm, thisLabel));


	labelPtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, seqLabel));
	StrPrintF(tempStr, "%d", (UInt16)ptrThisPacket->DLHeader.control.sequence);		// Convert Count to string

	//	Set the Rectangle area to be cleared before the new price is drawn.
//	RctSetRectangle(&space, labelPtr->pos.x, labelPtr->pos.y, 15, 12);
//	WinEraseRectangle(&space, 0);

	FrmHideObject(frm, FrmGetObjectIndex(frm, seqLabel));
	FrmCopyLabel(frm, seqLabel, tempStr);
	FrmShowObject(frm, FrmGetObjectIndex(frm, seqLabel));
	return 0;
}


/***********************************************************************
 *
 * FUNCTION:	  DisplayPacketCountRcvd
 *
 * DESCRIPTION:  .
 *
 * PARAMETERS:	
 *
 * RETURNED:	  
 *
 ***********************************************************************/
UInt16 DisplayPacketCountRcvd(UInt16 rcvdPackets, DLPacketPtr ptrThisPacket)
{
	FormPtr					frm;
	
//	Char						priceStr[11] = "          ";
	Char						tempStr[10];
//	UInt8*					sourcePtr, destPtr;

//	RectangleType			space;
	UInt16					thisLabel;
	UInt16					seqLabel;
	FormLabelType 			*labelPtr;

	frm = FrmGetActiveForm();

	switch(rcvdPackets)
	{
		case 1:
			thisLabel = BeamingRcvd1Label;
			seqLabel = BeamingRcvdSeq1Label;
			break;

		case 2:
			thisLabel = BeamingRcvd2Label;
			seqLabel = BeamingRcvdSeq2Label;
			break;

		case 3:
			thisLabel = BeamingRcvd3Label;
			seqLabel = BeamingRcvdSeq3Label;
			break;

		case 4:
			thisLabel = BeamingRcvd4Label;
			seqLabel = BeamingRcvdSeq4Label;
			break;

		case 5:
			thisLabel = BeamingRcvd5Label;
			seqLabel = BeamingRcvdSeq5Label;
			break;
	}

	labelPtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, thisLabel));
	StrPrintF(tempStr, "%d", ptrThisPacket->data[0]);		// Convert Count to string

	//	Set the Rectangle area to be cleared before the new price is drawn.
//	RctSetRectangle(&space, labelPtr->pos.x, labelPtr->pos.y, 45, 12);
//	WinEraseRectangle(&space, 0);

	FrmHideObject(frm, FrmGetObjectIndex(frm, thisLabel));
	FrmCopyLabel(frm, thisLabel, tempStr);
	FrmShowObject(frm, FrmGetObjectIndex(frm, thisLabel));


	labelPtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, seqLabel));
	StrPrintF(tempStr, "%d", (UInt16)ptrThisPacket->DLHeader.control.sequence);		// Convert Count to string

	//	Set the Rectangle area to be cleared before the new price is drawn.
//	RctSetRectangle(&space, labelPtr->pos.x, labelPtr->pos.y, 15, 12);
//	WinEraseRectangle(&space, 0);

	FrmHideObject(frm, FrmGetObjectIndex(frm, seqLabel));
	FrmCopyLabel(frm, seqLabel, tempStr);
	FrmShowObject(frm, FrmGetObjectIndex(frm, seqLabel));
	return 0;
}
#endif




/***********************************************************************
 *
 * FUNCTION:		MmParseRegistrationMessage
 *
 * DESCRIPTION:	Assembles the data for a Login Message to Server.
 *
 * PARAMETERS:		.
 *
 * RETURNED:		Nothing.
 *
 ***********************************************************************/
static void MmParseRegistrationMessage(UInt16 messageLength)
{
	UInt16				i, j;
	UInt16				thisChar16;



	messageLength -= 2;				//	Don't attempt to Parse the CRC!

	i = HLHeaderSize;


	//	Get the palmUniqueID
	Prefs.palmUniqueID = ALReceivePtr[i++];//	Low Byte

	thisChar16 = ALReceivePtr[i++];			//	High Byte
	Prefs.palmUniqueID = Prefs.palmUniqueID | (thisChar16<<8);


	//	Get palmRegistrationName string
	j = 0;
	while(i<messageLength && ALReceivePtr[i] != 0)
	{
		Prefs.palmRegistrationName[j++] = ALReceivePtr[i++];	//	Copy palmRegistrationName
	}

	for(; j < PalmRegistrationNameLength; j++)
	{
		Prefs.palmRegistrationName[j] = 0;							//	Terminate the string
	}

	if(StrLen(Prefs.palmRegistrationName) == 0)
		StrCopy(Prefs.palmRegistrationName, RegTemp.newName);


	MmSavePrefs();															//	Save these now in case we break
}




/***********************************************************************
 *
 * FUNCTION:		MmParseInfoSyncMessage
 *
 * DESCRIPTION:	Assembles the data for a Login Message to Server.
 *
 * PARAMETERS:		.
 *
 * RETURNED:		Nothing.
 *
 ***********************************************************************/
static void MmParseInfoSyncMessage(UInt16 messageLength)
{
	Int16					error = 0;					// Error code
	UInt16				i, j;
	UInt16				infoRecNum = 0;
	UInt16				recIndex;
	UInt16				recTableIndex;
	
	InfoType				infoRecord;
	SeatnameType		tablenameRecord;
	Char					tempString[MaxInfoNameSize];
	UInt16				index = 0;
/*
#ifndef DebugFakeMenu
	UInt16				dataCount;
	UInt8				uniqueIDString[PalmUniqueNameLength +1];
#endif
*/	

#ifdef DebugFakeMenu
static Boolean	toggle = false;

for(i=0; i<MyALReceiveBuffSize; i++)
	ALReceivePtr[i] = 0;				//	Zero out the buffer

if(toggle)
	toggle = false;
else
	toggle = true;

if(toggle)
{
	StrCopy((char*)ALReceivePtr,

"~~~~~~~~~~"
"[Info]\n"
"Guest\n"
"[Staff]\n"
"1|Michael|MDB\n"
"2|Susan|SJBR\n"					//	Oversize Initial
"3|Fred|FD\n"
"4|Sylvia|SEN\n"
"5|Hazel|HMN\n"
"6|Bill|WWW\n"
"7|Kirsten|KN\n"
"\n"
"[Palms]\n"
"276|Palm 1\n"
"3|Very Long Palm Registration Name\n"					//	
"32767|3rd Palm Name\n"
"\n"
"[Table]\n"
"1|Corner 1\n"
"2|Corner 2\n"
"3|Aisle 1\n"
"4|Aisle 2\n"
"5|Aisle 3\n"
"6|Window Seat over in the Back Corner\n"				//	Oversize Table name
"\n");
}
else
{
	StrCopy((char*)ALReceivePtr,

"~~~~~~~~~~"
"[Info]\n"
"\n"
"[Table]\n"
"1|Window 1\n"
"2|Window 2\n"
"3|Window 3\n"
"4|Window 4\n"
"5|Window 5\n"
"6|Window 6\n"
"7|Window 7\n"
"8|Window 8\n"
"9|Window 9\n"
"10|Centre 1\n"
"11|Centre 2\n"
"12|Centre 3\n"
"13|Centre 4\n"
"14|Centre 5\n"
"15|Centre 2\n"
"16|Centre 3\n"
"17|B1\n"
"18|B2\n"
"19|B3\n"
"20|B4\n"
"\n"
"[Staff]\n"
"1|Stephen|SJB\n"
"2|Julie|JFK\n"
"3|Rumplestiltskin|RSR\n"	//	Oversize name
"\n");
}
messageLength = StrLen((char*)ALReceivePtr);

#endif

	messageLength -= 2;				//	Don't attempt to Parse the CRC!


//MmPrvSyncLogTraceUI("\nALReceivePtr[HLHdrMenuSync] = %d", ALReceivePtr[HLHdrMenuSync]);
//MmPrvSyncLogTraceUI("\nHLMenuSyncStatus = %d", Prefs.hLStatus.HLMenuSyncStatus);

	i = HLHeaderSize;

	//	Find start of [Info] block in string
	while(i<messageLength && ALReceivePtr[i] != '[')
		i++;
	i++;																//	Step over [
	
	//	Find end of [Info] block in string
	j = 0;
	while(i<messageLength && ALReceivePtr[i] != ']')
	{
		tempString[j++] = ALReceivePtr[i++];				//	Copy Info to a temp string
	}
	tempString[j] = 0;											//	Terminate the string

	if(StrCaselessCompare((const char *)tempString, "Info") != 0)
		ErrThrow(HLParseErrorInfo);
	i += 2;															//	Point to Default Seatname


	//	Get Default Seatname
	j = 0;
	while(i<messageLength && ALReceivePtr[i] != '\n' && j < MaxSeatNameSize)
	{
		tempString[j++] = ALReceivePtr[i++];				//	Copy Info to a temp string
	}
	tempString[j] = 0;											//	Terminate the string

	StrCopy(Prefs.defaultSeatname, tempString);



	MmZapDB(&MmInfoDB, MmInfoDBName, RecreateDB, &DBDeleteBitMask, MmInfoMaskBit);
	recIndex = 1;



	while(i<messageLength)
	{
		//	Find start of [Staff] or [Table]  or [Palms] or [Payments] block in string
		while(i<messageLength && ALReceivePtr[i] != '[')
			i++;
		i++;															//	Step over [
		
		//	Find end of [  ] block in string
		j = 0;
		while(i<messageLength && ALReceivePtr[i] != ']')
		{
			tempString[j++] = ALReceivePtr[i++];			//	Copy Info to a temp string
		}
		tempString[j] = 0;										//	Terminate the string
		




		if(StrCaselessCompare((const char *)tempString, "Staff") == 0)
		{
//"[Staff]\n"
//"1|Michael|MDB\n"
//"2|Susan|SJBR\n"					//	Oversize Initial
//"\n"
			i += 2;
			
			infoRecord.recordType = StaffType;

			//	Find start of Staff/Table blocks in string (if any exist)
			while(i<messageLength && ALReceivePtr[i] != '\n')	//	Check if Staff/Table item exists
			{
				j = 0;
				while(i<messageLength && ALReceivePtr[i] != '|')
				{
					tempString[j++] = ALReceivePtr[i++];	//	Copy Staff/Table No to temp string
				}
				tempString[j] = 0;								//	Terminate the string
				infoRecord.number = StrAToI(tempString);	//	Preserve the Number
		
				i++;
		
				j = 0;
				while(i<messageLength && ALReceivePtr[i] != '|' && ALReceivePtr[i] != '\n')
				{
					//	Don't overrun record.name size
					if(j < MaxInfoNameSize)
						infoRecord.name[j++] = ALReceivePtr[i];	//	Copy the string until '|' or '\n'
					i++;
				}
				infoRecord.name[j] = 0;
				i++;													//	Step over the '\n'
				
				j=0;
				while(i<messageLength && ALReceivePtr[i] != '\n')
				{
					if(j < MaxInfoInitialSize)					//	Don't overrun record.initial size
						infoRecord.initial[j++] = ALReceivePtr[i];	//	Copy the string until '\n'
					i++;
				}
				infoRecord.initial[j] = 0;
				i++;													//	Step over the '\n'

				infoRecNum++;										//	Not used for Staff
				
				MmGenerateInfoRecord(&MmInfoDB, &infoRecord, recIndex++);
			}
			i++;														//	Step over the '\n'
		}




		else if(StrCaselessCompare((const char *)tempString, "Palms") == 0)
		{
//"[Palms]\n"
//"1|Palm 1\n"
//"2|Very Long Palm Registration Name\n"					//	
//"3|3rd Palm Name\n"
//"\n"
			i += 2;
			
			infoRecord.recordType = PalmsType;

			//	Find start of Staff/Table blocks in string (if any exist)
			while(i<messageLength && ALReceivePtr[i] != '\n')	//	Check if Staff/Table item exists
			{
				j = 0;
				while(i<messageLength && ALReceivePtr[i] != '|')
				{
					tempString[j++] = ALReceivePtr[i++];	//	Copy PalmID to temp string
				}
				tempString[j] = 0;								//	Terminate the string
				infoRecord.uniqueID = StrAToI(tempString);//	Preserve the Number
		
				i++;
		
				j = 0;
				while(i<messageLength && ALReceivePtr[i] != '|' && ALReceivePtr[i] != '\n')
				{
					//	Don't overrun record.name size
					if(j < MaxInfoNameSize)
						infoRecord.name[j++] = ALReceivePtr[i];	//	Copy the string until '|' or '\n'
					i++;
				}
				infoRecord.name[j] = 0;
				i++;													//	Step over the '\n'
				
				infoRecord.initial[0] = 0;
				infoRecord.number = infoRecNum++;

				MmGenerateInfoRecord(&MmInfoDB, &infoRecord, recIndex++);
			}
			i++;														//	Step over the '\n'
		}






		else if(StrCaselessCompare((const char *)tempString, "Payment Types") == 0)
		{
//"[Payment Types]\n"
//"Cash\n"
//"EFTPOS\n"
//"Cheque\n"
//"\n"
			i += 2;
			
			infoRecord.recordType = PaymentTypesType;

			//	Find start of Staff/Table blocks in string (if any exist)
			while(i<messageLength && ALReceivePtr[i] != '\n')	//	Check if Staff/Table item exists
			{
				j = 0;
				while(i<messageLength && ALReceivePtr[i] != '|' && ALReceivePtr[i] != '\n')
				{
					//	Don't overrun record.name size
					if(j < MaxInfoNameSize)
						infoRecord.name[j++] = ALReceivePtr[i];	//	Copy the string until '|' or '\n'
					i++;
				}
				infoRecord.name[j] = 0;
				i++;													//	Step over the '\n'
				
				infoRecord.initial[0] = 0;
				infoRecord.uniqueID = infoRecNum;			//	ID is not currently used
				infoRecord.number = infoRecNum++;

				MmGenerateInfoRecord(&MmInfoDB, &infoRecord, recIndex++);
			}
			i++;														//	Step over the '\n'
		}





		else if(StrCaselessCompare((const char *)tempString, "Table") == 0)
		{
			MmZapDB(&MmTablenameDB, MmTablenameDBName, RecreateDB, &DBDeleteBitMask, MmTablenameMaskBit);
			MmZapDB(&MmSeatnameDB, MmSeatnameDBName, RecreateDB, &DBDeleteBitMask, MmSeatnameMaskBit);

			recTableIndex = 1;

//"[Table]\n"
//"1|Corner 1\n"
//"2|Corner 2\n"
//"3|Aisle 1\n"
//"\n");
			i += 2;
			
			//	Find start of Staff/Table blocks in string (if any exist)
			while(i<messageLength && ALReceivePtr[i] != '\n')	//	Check if Staff/Table item exists
			{
 				j = 0;
 				while(i<messageLength && ALReceivePtr[i] != '|')
				{
					tempString[j++] = ALReceivePtr[i++];	//	Copy Staff/Table No to temp string
				}
				tempString[j] = 0;								//	Terminate the string
				tablenameRecord.tableNumber = StrAToI(tempString);	//	Preserve the Number
		
				i++;
		
				j = 0;
				while(i<messageLength && ALReceivePtr[i] != '|' && ALReceivePtr[i] != '\n')
				{
					//	Don't overrun record.name size
					if(j < MaxTableNameSize)
						tablenameRecord.name[j++] = ALReceivePtr[i];	//	Copy the string until '|' or '\n'
					i++;
				}
				tablenameRecord.name[j] = 0;
				i++;													//	Step over the '\n'


				MmGenerateSeatOrTableRecord(&MmTablenameDB, &tablenameRecord, recTableIndex++);
			}
			i++;														//	Step over the '\n'
		}
		else
		{
			ErrThrow(HLParseErrorInfo);
		}
	}

	
	Prefs.infoSyncNo = ALReceivePtr[HLHdrInfoSync];		//	Update new InfoSync

//	Prefs.infoSyncNumber	=	Prefs.infoSyncNo;
//	Prefs.palmIDNumber	=	Prefs.palmUniqueID;
//	StrCopy(Prefs.palmUniqueName, Prefs.palmUniqueName);

	MmSavePrefs();													//	Save these now in case we break
}




/***********************************************************************
 *
 * FUNCTION:		MmParseLoginMessage
 *
 * DESCRIPTION:	Assembles the data for a Login Message to Server.
 *
 * PARAMETERS:		.
 *
 * RETURNED:		Nothing.
 *
 ***********************************************************************/
static void MmParseLoginMessage(UInt16 messageLength)
{
	Int16					error = 0;					// Error code
	UInt16				i, j;
	UInt16				recIndex = 0;				//	Used first for Table then Menu records
	
	Char					tempString[MaxInfoNameSize];
//	UInt16				index = 0;
	UInt16				userSettingsUInt16;
#ifndef DebugFakeMenu
//	UInt16				dataCount;
#endif
	

#ifdef DebugFakeMenu
	static Boolean	toggle = false;
#endif
	
	
#ifdef DebugFakeMenu

for(i=0; i<MyALReceiveBuffSize; i++)
	ALReceivePtr[i] = 0;

if(toggle)
	toggle = false;
else
	toggle = true;

if(toggle)
{
	StrCopy((char*)ALReceivePtr,

"~~~~~~~~~~"
"[User]\n"
"15\n"						//	Transaction number
"1\n"							//	Time To Logoff (Mins)
"1\n"							//	Default Serving Course selection mode: 0 = Force popup, 1 = As previous
"91\n"						//	User Settings:	0x005B = 91
/*
	advancedUser;
	priceAdjust;
	autoDetails;
	isLargeFont;
	screenLock;
	classicMode;
	soundLevelBit1;
	soundLevelBit2;
*/

"\n");
}
else
{
	StrCopy((char*)ALReceivePtr,

"~~~~~~~~~~"
"[User]\n"
"10\n"						//	Transaction number
"20\n"						//	Time To Logoff (Mins)
"0\n"							//	Default Serving Course selection mode: 0 = Force popup, 1 = As previous
"0\n"							//	No User Settings

"\n");
}
messageLength = StrLen((char*)ALReceivePtr);

#endif


//MmPrvSyncLogTraceUI("\nALReceivePtr[HLHdrMenuSync] = %d", ALReceivePtr[HLHdrMenuSync]);
//MmPrvSyncLogTraceUI("\nHLMenuSyncStatus = %d", Prefs.hLStatus.HLMenuSyncStatus);

	messageLength -= 2;				//	Don't attempt to Parse the CRC!
	
	i = HLHeaderSize;
	j = 0;


	//	Find start of [User] block in string
	while(i<messageLength && ALReceivePtr[i] != '[')
	{
		i++;
	}
	i++;																//	Step over [
	
	//	Find end of [User] block in string
	while(i<messageLength && ALReceivePtr[i] != ']')
	{
		tempString[j++] = ALReceivePtr[i++];				//	Copy User to a temp string
	}
	tempString[j] = 0;											//	Terminate the string
	j = 0;
	if(StrCaselessCompare((const char *)tempString, "User") != 0)
		ErrThrow(HLParseErrorLogin);
	i += 2;															//	Point to Prefs.userLogOffMins




	//	Get the Transaction Number
	while(i<messageLength && ALReceivePtr[i] != '\n')
	{
		tempString[j++] = ALReceivePtr[i++];				//	Copy Info to a temp string
	}
	tempString[j] = 0;											//	Terminate the string
//	Prefs.transactionNo = StrAToI(tempString);			//	Preserve the Transaction Number
//MmPrvSyncLogTraceUI("\nTransactionNumber = %d", Prefs.transactionNo);
//	Prefs.transactionNumber		=	Prefs.transactionNo;

	i++;
	j = 0;




	//	Get the Logoff time
	while(i<messageLength && ALReceivePtr[i] != '\n')
	{
		tempString[j++] = ALReceivePtr[i++];				//	Copy Info to a temp string
	}
	tempString[j] = 0;											//	Terminate the string
	Prefs.userLogOffMins = StrAToI(tempString);			//	Preserve the Prefs.userLogOffMins
//MmPrvSyncLogTraceUI("\nUserLogOffMins = %d", Prefs.userLogOffMins);
//	Prefs.userLogOffMins		=	Prefs.userLogOffMins;

	i++;
	j = 0;




	//	Default Serving Course selection mode
	while(i<messageLength && ALReceivePtr[i] != '\n')
	{
		tempString[j++] = ALReceivePtr[i++];					//	Copy Info to a temp string
	}
	tempString[j] = 0;												//	Terminate the string
	Prefs.rememberLastServCourse = StrAToI(tempString);	//	Preserve the rememberLastServCourse

	i++;
	j = 0;





	//	Get the User Settings
	while(i<messageLength && ALReceivePtr[i] != '\n')
	{
		tempString[j++] = ALReceivePtr[i++];					//	Copy Info to a temp string
	}
	tempString[j] = 0;												//	Terminate the string
	i++;																	//	Step over \n
	j = 0;

	userSettingsUInt16 = StrAToI(tempString);
	MmUInt16ToUserSettings(userSettingsUInt16, &Prefs);

	MmSavePrefs();													//	Save these now in case we break

	SoundLevel = PrefGetPreference(prefAlarmSoundVolume);

	//	Clear out any messages that might be personal to this user
	MmDeleteAllTypeRecordsForMiscDataType(MmMiscDataDB, MessageType);
//	MmZapDB(&MmMiscDataDB, MmMiscDataDBName, RecreateDB, &DBDeleteBitMask, MmMiscDataMaskBit);

//MmPrvSyncLogTrace("\nFinished Parsing");
}



/***********************************************************************
 *
 * FUNCTION:		MmParseMenuSyncMessage
 *
 * DESCRIPTION:	Parse the newly received text file for Menu data etc.
 *
 * PARAMETERS:		.
 *
 * RETURNED:		Nothing.
 *
 ***********************************************************************/
/*static void MmParseMenuSyncMessage(UInt16 messageLength)
{
	Int16						error=0;				// Error code
	UInt16					i, j;
	UInt16					recIndex;			//	Used first for Menu records
	UInt16					recOptionIndex = 0;
	
	MenuType					record;
	OptionsType				option;
	
	UInt16					courseCounter;
	UInt16					itemCounter;		//	Provides Index into MmDishesDB
	UInt16					optionCounter;
	Char						tempStr[10];
#ifdef DebugFakeMenu
	static Boolean	toggle = false;
#endif
	
	MmZapDB(&MmDishesDB, MmDishesDBName, RecreateDB);
	MmZapDB(&MmDishesOptionsDB, MmDishesOptionsDBName, RecreateDB);
	MmZapDB(&MmOrdersDB, MmOrdersDBName, RecreateDB);
	MmZapDB(&MmSeatnameDB, MmSeatnameDBName, RecreateDB);

	
#ifdef DebugFakeMenu

for(i=0; i<MyALReceiveBuffSize; i++)
	ALReceivePtr[i] = 0;

if(toggle)
	toggle = false;
else
	toggle = true;

if(toggle)
{

	StrCopy((char*)ALReceivePtr,

"~~~~~~~~~~"
"|Dinner Menu|\n"
"[Sizes]\n"
"Standard|0\n"
"Large|0\n"
"Small|0\n"
"Main Size|0\n"
"Entree Size|0\n"
"\n"
"[Entree]\n"
"1|2|3|4||\n"
"2|Garlic Herbed Bread|300|||||\n"
"3|Soup Du Jour|550!7|||||\n"			//	7 is a SetMenuMask bitmask!
"4|Spicy Lamb Salad|1100|||||\n"
"5|P & K Balls|1000|||||\n"
"6|Trio of Fettucine|0|2|||||\n"
"7|Deep-Fried Camembert|1000|||||\n"
"{As Main|}\n"
"\n"
"[Main Meals]\n"
"1|2|3|4||\n"
"8|Chicken Breasts|1650|||||\n"
"9|Capsicum & Cashew Pan|1650|||||\n"
"10|Chargrilled Chicken|1650|||||\n"
"11|Fish of the Day|1650!7|||||\n"
"12|Salmon Steak|1650|||||\n"
"13|Pork Loin|1850!6|||||\n"
"14|12 Pointer|2250|||||\n"
"{Fresh Garden Salad|Wedges|Gourmet Potatoes|Fresh Steamed Vegetables|}\n"
"\n"
"[Steaks]\n"
"1|2|3|4||\n"
"15|Bulls Eye|1850|||||\n"
"16|Stags Sirloin|1850|||||\n"
"17|Giant T-Bone|1950|||||\n"
"18|Ham Steak|1850|||||\n"
"19|Roast Of The Day|1250!1|||||\n"
"{Field Mushrooms|Garlic butter|Herb sausage & onion|Italian Sauce & Kumara|}\n"
"\n"
"[Specials]\n"
"1|2|3|4||\n"
"20|Backpackers Steak|850|||||\n"
"21|Fish & Chips|850|||||\n"
"22|Pancake Rock Lasagne|850|||||\n"
"23|Chicken Vol Au Vent|850|||||\n"
"24|Fettucine|850|||||\n"
"25|3 Egg Omelette|850|||||\n"
"26|Campfire Roast|850|||||\n"
"{}\n"
"\n"
"[SetMenus]\n"
"1|2|3|4||\n"
"27|Senior 2 Course|1250!32769|||||\n"	//	Master has MSB set
"28|Senior 3 Course|1500!32770|||||\n"
"29|Kids Free Main + Dessert|0!32772|||||\n"
"{}\n"
"\n"
"[Extras]\n"
"1|2|3|4||\n"
"30|Crispy Onion Rings|300|||||\n"
"31|Bowl Of Mushrooms|350|||||\n"
"{}\n"
"\n"
"[Kids]\n"
"1|2|3|4||\n"
"32|Kids Soup|350|||||\n"
"33|Kids Nachos|450|||||\n"
"34|Kids Hot Dog|350|||||\n"
"35|Kids F&C|350|||||\n"
"36|Kids Chicken Nuggets|350|||||\n"
"37|Kids Pizza|400|||||\n"
"38|Kids Burger|400|||||\n"
"{}\n"
"\n"
"[Desserts]\n"
"1|2|3|4||\n"
"39|Apple Crumble|750!7|||||\n"
"40|Mud Cake|750!4|||||\n"
"41|Cheese Cake|750|||||\n"
"42|Velvet Dream|750!3|||||\n"
"43|Ice Cream|750!6|||||\n"
"44|Childs Dessert|250|||||\n"
"45|Cheese Board|1000|||||\n"
"46|Free Dessert|0!4|||||\n"
"{}\n"
"\n");
}
else
{
	StrCopy((char*)ALReceivePtr,


"~~~~~~~~~~"
"[Main1]\n"
"0029|Garlic Brd|300|||||\n"
"0030|Herb Brd|300|||||\n"
"0031|Soup|550|||||\n"
"0032|Pita|950|||||\n"
"0033|P&K Balls|1000|||||\n"
"0034|F & C|1000|||||\n"
"0035|M&B Om|1000|||||\n"
"0036|Caj Chck|1150|||||\n"
"0037|Fett|1200|||||\n"
"0038|Burger|1200|||||\n"
"{A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z|}\n"
"\n"
"[Main2]\n"
"0039|Or Chck Sa|1150|||||\n"
"0040|Ribeye|1600|||||\n"
"0041|Squid|1050|||||\n"
"0042|Stgs Grll|1250|||||\n"
"0043|Prk Med|1250|||||\n"
"0044|Vge Psta|1000|||||\n"
"0045|Pan Fsh|1200|||||\n"
"0046|Crssnt|1000|||||\n"
"{}\n"
"\n"
"[Specials]\n"
"0047|Chef's|1202|||||\n"
"{}\n"
"\n"
"[Bar Snack]\n"
"0071|Br Pza|800|||||\n"
"0072|Br Stk Snd|850|||||\n"
"0073|Br Nch|800|||||\n"
"0074|Br Qch|800|||||\n"
"0075|Br Chkn|800|||||\n"
"0076|Br Trpl Sn|800|||||\n"
"0077|Br Sea Bsk|750|||||\n"
"0078|Br Bf Snd|750|||||\n"
"0079|Br Om|750|||||\n"
"0080|Br Wdgs|500|||||\n"
"{}\n"
"\n"
"[Coffees]\n"
"0081|Fltr Coff|200|||||\n"
"0082|Latte|350|||||\n"
"0083|Vienna|325|||||\n"
"0084|Bell T|200|||||\n"
"0085|Earl Grey|200|||||\n"
"0086|Lemon T|200|||||\n"
"0087|Esprs|350|||||\n"
"0088|Int16 Blk|350|||||\n"
"0089|Hot Choc|400|||||\n"
"0090|Capp|350|||||\n"
"0091|Irish|550|||||\n"
"0092|Monks|550|||||\n"
"0093|Clypso|550|||||\n"
"0094|P of W|550|||||\n"
"0095|Mex Coff|550|||||\n"
"0096|Bundy|550|||||\n"
"0097|Jffa Dlt|650|||||\n"
"{}\n"
"\n"
"[Coffee2]\n"
"{}\n"
"\n"
"[Night Caps]\n"
"0098|Barros Vin|325|||||\n"
"0099|Barros 20|900|||||\n"
"0100|R&B Port|300|||||\n"
"0101|TT Port|600|||||\n"
"0102|R M Cognac|500|||||\n"
"0103|Barros 10|500|||||\n"
"0104|O M Sherry|300|||||\n"
"0105|Taylors Ruby|600|||||\n"
"0106|Taylors 10Pt|1200|||||\n"
"{}\n"
"\n");
}

messageLength = StrLen((char*)ALReceivePtr);

#endif

	messageLength -= 2;				//	Don't attempt to Parse the CRC!
	
	i = HLHeaderSize; j = 0;



	courseCounter=1;
	optionCounter=1;
	recIndex = 0;													//	Dishes/Drinks start at 0
	record.uniqueID = 0;											//	Ensure first uniqueID is sensible


	//	Find the |Menu Name| block in message
	while(i<messageLength && ALReceivePtr[i] != '|')	//	Find the first |
	{
		if(ALReceivePtr[i] == '[')								//	Oops. There's no name
			break;
		i++;
	}

	if(ALReceivePtr[i] != '[')									//	OK. There is a name
	{
		i++;															//	Get past the |
	
		j=0;
		//	Get the menuID first
		while(i<messageLength && ALReceivePtr[i] != '|')//	Copy the menuID
		{
			tempStr[j++] = ALReceivePtr[i++];			//	Copy the string until the |
		}
		tempStr[j] = 0;
		if(StrLen(tempStr) > 0)
			record.menuID = (UInt8)StrAToI(tempStr);

		i++;															//	Step over the |
	
		//	Now get the menu name
	
		j=0;
		while(ALReceivePtr[i] != '|')
		{
			Prefs.dishesMenuName[j++] = ALReceivePtr[i++];		//	Copy the string until the |
		}
		Prefs.dishesMenuName[j] = 0;
	}
	else
	{
		StrCopy(Prefs.dishesMenuName, "Unknown Name");			//	Stick in something to display
	}



	//	Outer loop cycles through Courses
	while(i<messageLength -1)									//	Don't try to parse if only \n
	{
		//	Find [Course] blocks in message
		while(i<messageLength && ALReceivePtr[i] != '[')
			i++;
		i++;															//	Get past the [

		j=0;
		while(i<messageLength && ALReceivePtr[i] != ']')
		{
			record.itemName[j++] = ALReceivePtr[i++];		//	Copy the string until the ]
		}
		record.itemName[j] = 0;
		StrCopy(record.disabledItemName, "* ");
		StrCat(record.disabledItemName, record.itemName);

		record.courseNumber = courseCounter;
		record.itemNumber = 0;									//	Zero => Course name
		record.enabled = StatusAsEnabled;					//	All enabled by default

		record.uniqueID++;										//	UniqueID is one above last drink
		record.setMenuMask = 0;

		MmGenerateMenuRecord(&MmDishesDB, &record, recIndex++);//	Generate the Course record

		//	Find start of Dish blocks in string
		while(i<messageLength && ALReceivePtr[i] != '\n')	//	Get to the end of the line
			i++;
		i++;															//	Step to start of Dish number

		//	Now do the Dishes
		itemCounter=1;

		while(i<messageLength && ALReceivePtr[i] != '{')
		{
			record.itemNumber = itemCounter++;

			//	Copy the uniqueID
			j=0;
			while(i<messageLength && ALReceivePtr[i] != '|')	//	Copy the uniqueID
			{
				tempStr[j++] = ALReceivePtr[i++];			//	Copy the string until the |
			}
			tempStr[j] = 0;
			record.uniqueID = StrAToI(tempStr);				//	Convert and store uniqueID

			i++;

			//	Get the Dish name
			j=0;
			while(i<messageLength && ALReceivePtr[i] != '|')
			{
				if(j < MaxMenuItemSize)							//	Don't overrun record.itemName size
					record.itemName[j++] = ALReceivePtr[i];//	Copy the string until the |
				i++;
			}
			record.itemName[j] = 0;


			StrCopy(record.disabledItemName, "* ");
			StrCat(record.disabledItemName, record.itemName);

			i++;														//	Step over the |
			j=0;

			while(	i<messageLength	 		&&
						ALReceivePtr[i] != '\n'	&&
						ALReceivePtr[i] != '|')					//	Copy the Price
			{
				tempStr[j++] = ALReceivePtr[i++];			//	Copy the string until the |
			}
			tempStr[j] = 0;
			record.price = StrAToI(tempStr);					//	Convert and store price
			record.enabled = StatusAsEnabled;				//	All enabled by default

			record.setMenuMask = 0;
			tempStr[0] = 0;

			//	If its a 'SetMenu' dish it will have the group mask after a |
			if(ALReceivePtr[i] == '|')
			{
				i++;													//	Step over the |
				j=0;

				while(i<messageLength && ALReceivePtr[i] != '\n')
				{
					tempStr[j++] = ALReceivePtr[i++];		//	Copy the string until the |
				}
				tempStr[j] = 0;
				record.setMenuMask = StrAToI(tempStr);		//	Convert and store price
			}
			

			MmGenerateMenuRecord(&MmDishesDB, &record, recIndex++);	//	Generate a Dish record
			i++;														//	Step over the \n
		}
		i++;															//	Step over the {
		
		optionCounter=1;											//	Count options for this Course
		while(i<messageLength && ALReceivePtr[i] != '}')//	Do the Options
		{
			option.courseNumber = courseCounter;
			option.courseOptionNumber = optionCounter++;

			j=0;

			while(i<messageLength && ALReceivePtr[i] != '|')
			{
				option.optionName[j++] = ALReceivePtr[i++];//	Copy the string until the |
			}
			if(j > MaxOptionItemSize)
				j = MaxOptionItemSize;							//	Don't accepts too long strings
			option.optionName[j] = 0;
			MmGenerateOptionsRecord(&MmDishesOptionsDB, &option, recOptionIndex++);
			i++;														//	Step over the |
		}
//MmPrvSyncLogTrace("\nGot Here!");
		courseCounter++;
		i+=3;															//	Step to start of next item
	}
#ifdef DebugHL3
MmPrvSyncLogTraceUI("\nMenuSyncNumber = %d", Prefs.menuSyncNo);
MmPrvSyncLogTraceUI("\nTransactionNumber = %d", Prefs.transactionNo);
#endif

}
*/

/***********************************************************************
 *
 * FUNCTION:		MmParseMenuSyncMessage
 *
 * DESCRIPTION:	Parse the newly received text file for Food or Drink data etc.
 *
 * PARAMETERS:		.
 *
 * RETURNED:		Nothing.
 *
 ***********************************************************************/
static void MmParseMenuSyncMessage(UInt16 messageLength, UInt16 menuType)
{
	Int16						error=0;				// Error code
	UInt16					i, j;
	UInt16					recSizeIndex;			//	
	UInt16					recItemIndex;			//	
	UInt16					recOptionIndex;
	UInt16					recForcedSidesIndex;
	UInt16					recServingCourseIndex;
	UInt16					currentCourseIndex;	//	Allow easy access to size type for Drink items

	MenuType					record;
	OptionsType				option;
	SizeType					sizeRecord;
	ForcedSidesType		forcedSide;
	ServingCourseType		servingCourseRecord;
	Char						tempString[MaxMenuItemSize];
	
//	UInt8						mMenuTemp;
	UInt16					recServingCourseIndexTemp;
	UInt32					colorTemp;
	Boolean					selectable;

	Boolean					isSetMenuKey;


	UInt16					courseCounter;		//	Provides Index into MmDrinksDB
	UInt16					itemCounter;
	UInt16					itemInCourse;
	UInt16					optionCounter;
	UInt16					sizeCounter;
	UInt16					servingCourseDefault;

	UInt16					numRemoved;


	DmOpenRef				*menuDB;
	DmOpenRef				*sizesDB;
	DmOpenRef				*optionsDB;
	DmOpenRef				*forcedSidesDB;

	UInt8						menuID;
	Char						*menuNamePtr;



#ifdef DebugFakeMenu
	static Boolean	toggleDishes = false;
	static Boolean	toggleDrinks = false;
#endif
	
	if(menuType == DishesMenu)
	{
		MmZapDB(&MmDishesDB, MmDishesDBName, RecreateDB, &DBDeleteBitMask, MmDishesMaskBit);
		MmZapDB(&MmDishesSizesDB, MmDishesSizesDBName, RecreateDB, &DBDeleteBitMask, MmDishesSizesMaskBit);
		MmZapDB(&MmDishesOptionsDB, MmDishesOptionsDBName, RecreateDB, &DBDeleteBitMask, MmDishesOptionsMaskBit);
		MmZapDB(&MmDishesForcedSidesDB, MmDishesForcedSidesDBName, RecreateDB, &DBDeleteBitMask, MmDishesForcedSidesMaskBit);
		MmZapDB(&MmMiscDataDB, MmMiscDataDBName, RecreateDB, &DBDeleteBitMask, MmMiscDataMaskBit);
		MmZapDB(&MmOrdersDB, MmOrdersDBName, RecreateDB, &DBDeleteBitMask, MmOrdersMaskBit);
	}
	else	
	{
		MmZapDB(&MmDrinksDB, MmDrinksDBName, RecreateDB, &DBDeleteBitMask, MmDrinksMaskBit);
		MmZapDB(&MmDrinksSizesDB, MmDrinksSizesDBName, RecreateDB, &DBDeleteBitMask, MmDrinksSizesMaskBit);
		MmZapDB(&MmDrinksOptionsDB, MmDrinksOptionsDBName, RecreateDB, &DBDeleteBitMask, MmDrinksOptionsMaskBit);
		MmZapDB(&MmDrinksForcedSidesDB, MmDrinksForcedSidesDBName, RecreateDB, &DBDeleteBitMask, MmDrinksForcedSidesMaskBit);
		MmZapDB(&MmOrdersDB, MmOrdersDBName, RecreateDB, &DBDeleteBitMask, MmOrdersMaskBit);
	}

	numRemoved = MmDeleteAllServingCourseRecordsForMenu(MmServingCourseDB, menuType);

//ShowMessage("numRemoved %d", numRemoved);
	
#ifdef DebugFakeMenu

MemSet(ALReceivePtr, MyALReceiveBuffSize, 0);


if(menuType == DishesMenu)
{
	if(toggleDishes)
		toggleDishes = false;
	else
		toggleDishes = true;
}
else
{
	if(toggleDrinks)
		toggleDrinks = false;
	else
		toggleDrinks = true;
}

if(menuType == DishesMenu)
{
//	if(toggleDishes)
//	{

		StrCopy((char*)ALReceivePtr,

"~~~~~~~~~~"

//	DisplayItemSizes (T/F) ForcedSideOnly (T/F)|Name|Size T or F or K SetMenuMask|, Forced Sides|
//
//	Eg. "TFChicken Cranberry & Brie|51|67T3|88|,1|T1|5|7|11|,2|T0|8|9|10|\n"
//	
//	SetMenus: T or F = True/False available as standard item, K = Key
//
// Forced Sides: GroupNo (1-8), Max Select.
//	... where T or F = True/False 'Continue' button available
//	... where 0 = Select as many as you like then hit Continue (Must be T).
//	... 		 1 = Select 1 auto move on (or allow Continue to skip this group if T).
//
//	Forced Options: This comes in 3 parts:
//		1st char -> Option control bit-flags
//			Bit 3		Next Button displayed (ie allow skip)
//			Bit 2		Available as minus option
//			Bit 1		Available as plus option
//			Bit 0		(Not) Multi-Select (ie 0 = multi select, 1 = select 1 only)
//
//		2nd char -> Group number
//			They may be prompted up to 16 times with lists of forced options
//
//		8 chars  -> Bit mask of items that will force select this option
//
//	eg.	E101FFFFFF	->	Will put "Next" at top of list
//								Will display in + and - options list,
//								Will allow multi-select
//								will appear in 1st forced option list,
//								will pop up for first 25 items in course



"~~~~~~~~~"
"|RESTAURANT|\n"
"[Sizes]\n"
"$1.00\n"
"$1.50\n"
"$2.00\n"
"$2.50\n"
"$3.00\n"
"$3.50\n"
"$4.00\n"
"$4.50\n"
"$5.00\n"
"$5.50\n"
"$6.00\n"
"$6.50\n"
"$7.00\n"
"$7.50\n"
"+fries and salad\n"
"12.50\n"
"120Z\n"
"14 oz.\n"
"8 oz.\n"
"Afghan\n"
"Amaretto\n"
"BANANA\n"
"BLUE\n"
"BLUECHEESE\n"
"BOTTLE\n"
"BOWL\n"
"Bacon\n"
"Baileys\n"
"Belgium Slice\n"
"Black Forest\n"
"CHOC\n"
"COKE\n"
"COMP.\n"
"Caramel\n"
"Carrot\n"
"Carrot Cake Sml\n"
"Cheese\n"
"Cheese & Bacon\n"
"Chicken\n"
"Chocolate\n"
"Chocolate Mud\n"
"Citrus Slice\n"
"Cointreau\n"
"DOUBLE\n"
"DOZ\n"
"Default\n"
"Drambuie\n"
"ENTREE SIZE\n"
"Eggs\n"
"Espresso Tart\n"
"FAMILY\n"
"FANTA\n"
"FOR ONE\n"
"FOR TWO\n"
"Fried Egg\n"
"Fruit\n"
"GLASS\n"
"Galliano\n"
"Ginger Slice\n"
"Grand Marnier\n"
"HALF\n"
"Hawaiian\n"
"Hazelnut\n"
"Irish Cream\n"
"Jamesons\n"
"Kahlua\n"
"LARGE\n"
"Lemon\n"
"Lemon & White Choc.\n"
"Lemon Citrus\n"
"Lemon Meringue\n"
"Lolly\n"
"MAIN SIZE\n"
"MEDIUM\n"
"MEDIUM RARE\n"
"MEDIUM WELL\n"
"Nut\n"
"ON ITS OWN\n"
"Orange\n"
"PACK\n"
"PESTO\n"
"PINT\n"
"PLAIN\n"
"Pancakes\n"
"Passionfruit\n"
"RARE\n"
"RASPBERRY\n"
"REGULAR\n"
"SINGLE\n"
"SMALL\n"
"SPECIAL PINT\n"
"SUNDRIED TOMATO\n"
"Sacher Torte\n"
"Salmon\n"
"Savoury\n"
"Seafood\n"
"Sencha lemon and ginger\n"
"Smoked Salmon\n"
"Standard\n"
"Supreme\n"
"TAPENADE\n"
"Thai Chicken\n"
"Tia Maria\n"
"UNIT\n"
"VINAIGRETT\n"
"Vanilla\n"
"WELL DONE\n"
"WHOLE\n"
"WITH ICECREAM\n"
"WITH SALAD\n"
"White Choc Mousse Cake\n"
"White Chocolate\n"
"With Aioli\n"
"With Salsa\n"
"\n"
"[ServingCourses]\n"
"1|No Serving Course|F32896\n"
"2|Entrees|T51200\n"
"3|Mains|T255\n"
"4|Desserts|T14418140\n"
"\n"
"[BREADS & ENTREES]2\n"
"TFBreads + Dips|46T28|\n"
"TFGarlic Bread|46T31|\n"
"TFSweet Chilli Flatbread|46|\n"
"TFRed Pepper Flatbread|46|\n"
"TFSoup of the Day|46T3|\n"
"TFSweetbreads|46T3|\n"
"TFOnion + Feta Tart|46T16|53T16|54T4|61T8|\n"
"TFSeared Scallops|46|19|18|17|\n"
"TFAntipasto Platter|53|54|\n"
"TFMushroom Sandwich|46F3|\n"
"TFVege Tortalinni|46|\n"
"TFChilli Prawn Noodles|46|\n"
"TFPasta of day|46|\n"
"{2000000000WAIT TO CALL|2000000000TALK TO WAITER|2000000000WITH MAINS|2000000000MAIN SIZE|2000000000ALL TOGETHER|2000000000NO BREAD|2000000000NO BACON|2000000000NO CREAM|2000000000NO BUTTER|2000000000NO GARLIC|2000000000NO CREAM CHEESE|2000000000NO CHEESE|2000000000NO CUCUMBER|2000000000NO MUSHROOMS|2000000000NO TOMATO|2000000000NO NUTS|2000000000NO ONION|2000000000No truffle oil|A100000040Dressing on side|A100000040Sauce on Side|A100000040With Butter|2000000000No corriander yogurt|2000000000GLUTEN FREE|2000000000TAKE-AWAY|}\n"
"\n"
"[BRUNCH]0\n"
"TFSoup of the Day|46|\n"
"TFMuesli|46|\n"
"TFScrambled Eggs|46|\n"
"TFEggs Benedict|27|98|\n"
"TFPancakes|46|\n"
"TFJack's Flash Breakfast|46|\n"
"TFVege Omelette|46|\n"
"TFDaily Pasta|46|\n"
"TFKumara&Smoke Chix Salad|46|\n"
"TFJF Caesar Salad|46|39|\n"
"TFVege Tortalinni|48|73|\n"
"TFChilli Prawn Noodles|48|73|\n"
"TFSide of Bacon|46|\n"
"TFSide of Sausage|46|\n"
"TFSide of Egg|46|\n"
"TFSide of Rosemary Potato|46|\n"
"TFSide of Mushroom|46|\n"
"TFSide of Tomatoes|46|\n"
"TFSide of Pancakes|46|\n"
"TFSide of Fries|46|\n"
"TFSide of Wedges|46|\n"
"TFSide Salad|46|\n"
"TFSide of Toast|46|\n"
"{2000000000WAIT TO CALL|2000000000ALL TOGETHER|2000000000AS A STARTER|2000000000TALK TO WAITER|2000000000No Bacon|2000000000No Bananas|2000000000No Beans|2000000000No Blueberries|2000000000No Bread|2000000000No Cream|2000000000No Eggs|A100004000Poached Eggs|A100004000Fried Eggs|A100004000Scrambled Eggs|2000000000No Hash Brown|2000000000No Kumara|2000000000No Maple Syrup|2000000000No Mushrooms|2000000000No Pepper|2000000000No Salsa|2000000000No Sausage|2000000000No Sour Cream|2000000000No Spinach|2000000000No Toast|2000000000No Tomato|2000000000No Yoghurt|2000000000GLUTON FREE|}\n"
"\n"
"[SANDWICHES]0\n"
"TFThai Chicken Burger|46|\n"
"TFSteak Sandwich|46|\n"
"TFRoast Mushroom Sandwich|46|\n"
"TFCCB Panini|46|15|\n"
"TFClassic Kiwi Panini|46|15|\n"
"TFChicken  Indienne panini|46|15|\n"
"TFLamb Panini|46|15|\n"
"TFNeptune Panini|46|15|\n"
"TFGreek Panini|46|15|\n"
"TFBacon&Blue  Panini|46|15|\n"
"TFMargherita Panini|46|15|\n"
"TFSupreme Panini|46|15|\n"
"TFDYO Panini|46|15|\n"
"{A100001000chicken|A100001000capsicum|A100001000Spinach|A100001000Pineapple|A100001000Prawns|A100001000Banana|A100001000onion|A100001000mushrooms|A100001000blue cheese|A100001000Sour Cream|A100001000sweet chilli|A100001000hummus|A100001000brie|A100001000salami|A100001000lamb|A100001000feta|A100001000ham|A100001000pepperoni|A100001000Mozzarella|A100001000sundried toms|A100001000cream cheese|A100001000pesto|A100001000olives|A100001000tomato|A100001000Zucchini|A100001000With mains|A100001000As a starter|2300000000M/W|2300000000Med|2300000000Rare|2300000000Well Done|2300000000Med Rare|}\n"
"\n"
"[PIZZAS]3\n"
"TFSweet Chilli Flatbread|46|\n"
"TFRed Pepper Flatbread|46|\n"
"TFChicken Cranberry & Brie|51|67T3|88|,1|T1|5|7|11|,2|T0|8|9|10|\n"
"TFClassic Kiwi|51|67|88|\n"
"TFMargherita with Salami|51|67F3|88|\n"
"TFNeptune|51|67|88|\n"
"TFMushroom  Bacon & Blue Cheese|51T1|67|88T2|\n"
"TFChicken Indienne|51|67|88|\n"
"TFGreek|51|67|88|\n"
"TFPeppered Lamb|51|67|88|\n"
"TFSupreme|51|67|88|\n"
"TFKids Pizza|38|62|\n"
"TFHALF N HALF PIZZAS|51|67|,1|T1|67|\n"
"TFGluten Free Pizza Base|46|\n"
"{A100001000Half CCB|A100001000Half Margherita|A100001000Half Classic Kiwi|A100001000Half Supreme|A100001000Half Neptune|A100001000Half Peppered Lamb|A100001000Half Mushroom,Bacon,Blue|A100001000Half Greek|A100001000Half  Chicken Indienne|A100001000Half kids pizza|3300001000WAIT TO CALL|3300001000ALL TOGETHER|3300001000AS A STARTER|3300001000TALK TO WAITER|A200001000NO CAPSICUM|A200001000NO CHEESE|3200000000NO DAIRY|3200000000NO DRESSING|A200001000NO FETA|A200001000NO MUSHROOMS|A200001000NO NUTS|A200001000NO PINEAPPLE|A200001000NO CAPERS|A200001000NO ONION|A200001000NO PARMESAN|A200001000NO BANANA|A200001000NO OLIVES|A200001000NO SOURCREAM|A200001000NO SPINACH|A200001000NO TOMATO|A200001000NO YOGHURT|A200001000EASY ON THE CHEESE|}\n"
"\n"
"[LUNCH]0\n"
"TFSoup of the Day|46|\n"
"TFVege Omelette|46|\n"
"TFSeared Scallops|46|\n"
"TFOnion + feta tart|46|\n"
"TFSweetbreads|46|\n"
"TFChilli Prawn Noodles|48|73|\n"
"TFVege Tortalinni|48|73|\n"
"TFDaily Pasta|46|\n"
"TFKumara Smoked Chix Salad|46|\n"
"TFJF Caesar Salad|46|39|\n"
"{2000000000WAIT TO CALL|2000000000ALL TOGETHER|2000000000NO MEAT|2000000000NO OLIVES|2000000000DRESSING ON SIDE|2000000000NO DRESSING|2000000000No Corriander Yogurt|2100000000No truffle oil|2000000000AIOLI INSTEAD|2000000000NO MUSHROOMS|2000000000NO BACON|2000000000NO ONION|2000000000NO TOMATO|2000000000NO CAPSICUM|2000000000NO CELERY|2000000000NO CREAM CHEESE|2000000000NO CUCUMBER|2000000000NO SOURCREAM|2000000000HALF PORTION|2000000000GLUTEN FREE|2100000000Rare|2100000000Med Rare|2100000000Medium|2100000000Med Well|2100000000Well Done|2100000200NO ANCHOVIES|}\n"
"\n"
"[MAINS]3\n"
"TFChicken|46T28|\n"
"TFFillet of Beef|46|\n"
"TFLamb Rump|46|\n"
"TFVenison|46T28|\n"
"TFVege Terrine|46|,1|T1|11|30|\n"
"TFFish of the Day|46|\n"
"TFChilli Prawn Noodles|48|73|\n"
"TFVege Tortalinni|48|73|,1|F1|57|58|\n"
"TFPasta of day|48|73|\n"
"TFThai Chicken Burger|46|\n"
"TFSteak Sandwich|46|\n"
"TFKumara Smoked Chix Salad|46|\n"
"TFJF Caesar Salad|46|39|\n"
"TFSoup of the Day|46|\n"
"TFSeared Scallops|46|\n"
"TFSweetbreads|46|\n"
"TFOnion + feta tart|46|\n"
"TFMushroom Sandwich|46|\n"
"TF$40 Set Menu|46K1|\n"
"TF$35 Set Menu|46K2|\n"
"TFSet Menu with Sizes|46K16|61K8|44K4|\n"
"{2000000000TALK TO WAITER|2000000000WAIT TO CALL|2000000000ALL TOGETHER|2000000000AS A STARTER|2000000000NO BLUECHEESE|2000000000NO CAPSICUM|2000000000NO CELERY|2000000000NO CUCUMBER|2000000000NO DRESSING|2000000000NO EGGPLANT|2000000000NO CHILLI|2000000000NO JUS|2000000000NO MEAT|2000000000NO MUSHROOMS|2000000000NO ONION|2000000000NO PARMESAN|2000000000NO SPROUTS|2000000000NO TOMATO|2000000000DRESSING ON SIDE|2000000000GLUTON FREE|2000000000HALF PORTION|220000003FVegetables|220000003FSalad|A100000002Rare|A100000002Medium Rare|A100000002Medium|A100000002Medium Well|A100000002Well Done|A100000002Blue|2000000000TAKE-AWAY|}\n"
"\n"
"[CABINET]0\n"
"TFSALAD|46|\n"
"TFBAGEL|46|\n"
"TFCROISSANT|46|,2|T0|103|105|106|111|118|119|\n"
"TFCUSTARD SQUARE|46|\n"
"TFSAVOURY SLICE|46|\n"
"TFSLICE|34|40|77|56|59|42|29|\n"
"TFBISCUIT|46|\n"
"TFMUFFIN|46|\n"
"TFSCONE|46|\n"
"TFCAKE / GATEAU|36|35|111|37|41|93|71|79|30|22|72|69|\n"
"TFPECAN PIE|10|\n"
"TFSWEET TART|40|56|77|\n"
"TFSAVOURY TART|46|\n"
"TFSANDWICH|46|\n"
"TFCHEF'S SPECIAL SAVOURY|6|7|8|9|10|11|14|13|12|\n"
"TFCHEF'S SPECIAL SWEET|6|7|8|10|12|13|9|11|\n"
"TFBISCOTTI|46|\n"
"TFDESTINY CHOCS|46|\n"
"{A100008E20Cream on side|A100008E20Yoghurt on side|2400000180Butter|2200005190Heated|A100000000TAKEAWAY|2300000040Caramel|2400000180No butter|2300000040Cherry|2300000040Chocolate|2300000040Lemon|2300000040Orange|A100000180Savoury|A100000182Sweet|A100000007Bacon|A100000007Beef|A100000006Breakfast|A100000007Chicken|A100000007Ham|A100000007Lamb|A100000006Pork|A100000006Salmon|A100000001Fruit|A100008E20Plain|A100000001Garden|A100000001Noodle|A100000001Pasta|A100000003Vegetarian|2200000000Danish|2200005190Not Heated|2200000000Pain au Chocolat|2300000040Raisin|2300000040Almond|A100000005Salami|A100000001Potato|2300000040NUT|}\n"
"\n"
"[COFFEE&TEA]4\n"
"TFFlat White|88|26|\n"
"TFLong Black|46|\n"
"TFCappuccino|88|26|\n"
"TFLatte|46|\n"
"TFLatte Bowl|46|\n"
"TFMocha|88|26|\n"
"TFShort Black|46|\n"
"TFVienna|46|\n"
"TFMachiato|46|\n"
"TFHot Chocolate|88|26|\n"
"TFHot Blackcurrant|46|\n"
"TFChai Latte|88|26|\n"
"TFFluffy|46|\n"
"TFHot Water|46|\n"
"TFEnglish Breakfast Tea|46|\n"
"TFEarl Grey Tea|46|\n"
"TFBengal Chai Tea|46|\n"
"TFChamomile Tea|46|\n"
"TFGran's Garden Tea|46|\n"
"TFJapanese Lime Tea|46|\n"
"TFLemon Citrus Tea|46|\n"
"TFPeach & Apricot Tea|46|\n"
"TFPeppermint Tea|46|\n"
"TFSencha Ginger & Lemon Tea|46|\n"
"TFSmoothies|46|\n"
"TFStaff Coffee|46|\n"
"TFPiazza D'ora 500g Beans|46|\n"
"TFSOY MILK|46|\n"
"TFSYRUP|34|40|63|64|106|\n"
"TFLIQUEUR COFFEE|28|43|47|58|65|103|\n"
"{2000000000TRIM MILK|2000000000DECAF|2000000000IN A GLASS|2000000000BIG CUP|2000000000EXTRA JAFFAS|2000000000WITH CINNAMON|2000000000NO SPRINKLES|2000000000AMERICANO|2000000000SINGLE SHOT|2000000000WITH LEMON|2000000000DOUBLE SHOT|2000000000WEAK|2000000000WITH RUNNY CREAM|2000000000NO MILK|2000000000COLD MILK ON SIDE|2000000000HOT MILK ON SIDE|2000000000BOWL|2000000000With Honey|2000000000Hot water on side|2200000100Short|2200000100Long|2000000000IN A MUG|2301000000Iced Chocolate|2301000000Iced Coffee|2301000000Berry|6301000000Banana|A100000000for 2 people|2000000000ONE SUGAR|}\n"
"\n"
"[SNACKS]0\n"
"TFFries|99|\n"
"TFWedges|99|\n"
"TFSpring Rolls|46|\n"
"TFVeges|46|\n"
"TFSalad|46|\n"
"TFSour Cream|46|\n"
"TFAioli|46|\n"
"TFSalsa|46|\n"
"TFBluebird Chips|46|\n"
"TFStaff Lunch|46|\n"
"TFStaff Dinner|46|\n"
"TFStaff Meal Single|46|\n"
"{2100000000as a starter|2100000000No salt|2100000000With Sweet Chilli sauce|2100000000With Tomato sauce|2100000000No Tom Sauce With Aioli Inst|2100000000No Sour Cream With Aioli Ins|2600000003AIOLI|2600000003SALSA|2600000003SOUR CREAM|2600000003TOM. SAUCE|}\n"
"\n"
"[CHILDREN'S MENU]0\n"
"TFSandwiches|46|\n"
"TFCorn Chips & Cheese|46|\n"
"TFChicken Breast|46|\n"
"TFBlue Cod & Fries|46|\n"
"TFChicken Nuggets|46|\n"
"TFSpring Rolls|46|\n"
"TFFresh Fruit|46|\n"
"TFChocolate Bar|46|\n"
"TFBluebird Potato Chips|46|\n"
"TFStir Fried Vegetables|46|\n"
"TFJuice Box|46|\n"
"TFFries|46|\n"
"TFWedges|46|\n"
"TFChildren's Art Lunchbox|46|\n"
"TFIce Cream Sundae|46|\n"
"TFKapiti Ice Cream Tub|46|\n"
"{2000000000WITH TOMATO SAUCE|2000000000NO CHEESE|2000000000no salsa|2000000000NO SOURCREAM|2000000000NO TOMATO|2000000000SALSA ETC ON SIDE|2000000000SWEET CHILLI SAUCE|2000000000HALF PORTION|2000000000WELL DONE|2000000000TAKE-AWAY|2000000000WAIT TO CALL|2000000000AS A STARTER|2000000000ALL TOGETHER|2000000000WITH MAINS|2300008000Raspberry|2300008000Chocolate|2300008000Vanilla|2100004000Strawberry|2100004000Chocolate*|2200002001100s & 1000s|2200002001Honey|2200002001Marmite|2200002001Raspberry Jam|2200002001Stawberry Jam|2200002001Peanut Butter|}\n"
"\n"
"[DESSERTS]4\n"
"TFLemon Creme Brulee|46T28|\n"
"TFChoc + Kalua Mousse|46T1|\n"
"TFPeach + Blueberry Tart|46T28|\n"
"TFRasberry Sorbet|46|\n"
"TFCheese Plate|46T1|\n"
"TFClassic Sundae|46|\n"
"TFScoop of Ice Cream|46|\n"
"TF$35 Birthday Cake|46|\n"
"TFBirthday Cake Surcharge|46|\n"
"{2000000000WAIT TO CALL|2000000000ALL TOGETHER|A100000020STRAWBERRY|A100000020CHOCOLATE|A100000020PLAIN|A100000020CARAMEL SAUCE|2000000000WITH ICECREAM|2000000000NO CREAM|2000000000IN A GLASS|2000000000NO NUTS|2000000000EXTRA ICECREAM|2000000000INSERT CANDLE|2000000000TAKE-AWAY|}\n"
"\n"
"[TAKEAWAY COFFEE]0\n"
"TFFlat White|19|18|\n"
"TFLong Black|19|18|\n"
"TFCappuccino|19|18|\n"
"TFLatte|19|18|\n"
"TFMocha|19|18|\n"
"TFMachiato|19|18|\n"
"TFHot Chocolate|19|18|\n"
"TFChai Latte|19|18|\n"
"TFHot Blackcurrant|19|18|\n"
"TFSOY MILK|46|\n"
"TFCaramel Syrup|46|\n"
"TFChocolate Syrup|46|\n"
"TFHazelnut Syrup|46|\n"
"TFIrish Cream Syrup|46|\n"
"TFVanilla Syrup|46|\n"
"{2000000000TRIM MILK|2000000000DECAF|2000000000WITH CINNAMON|2000000000NO SPRINKLES|2000000000SINGLE SHOT|2000000000DOUBLE SHOT|2000000000WEAK|2000000000WITH RUNNY CREAM|2000000000ONE SUGAR|2000000000TWO SUGARS|2000000000THREE SUGARS|2000000000EXTRA JAFFAS|2000000000NO MARSHMALLOWS|}\n"
"\n"
"[TAKEAWAY PIZZA/ SNACKS]0\n"
"TFRed Pepper Flatbread|46|\n"
"TFSweet Chilli Flatbread|46|\n"
"TFChicken Cranberry & Brie|51|67|88|\n"
"TFClassic Kiwi|51|67|88|\n"
"TFPeppered Lamb|51|67|88|\n"
"TFChicken Indienne|51|67|88|\n"
"TFNeptune|51|67|88|\n"
"TFMushroom, Bacon,Blue|51|67|88|\n"
"TFGreek|51|67|88|\n"
"TFMargherita|51|67|88|\n"
"TFSupreme|51|67|88|\n"
"TFKids Pizza|38|62|\n"
"TFHALF N HALF PIZZAS|51|67|\n"
"TFFries|46|\n"
"TFAioli|46|\n"
"TFSalsa|46|\n"
"TFWedges|46|\n"
"TFGluten Free Pizza Base|46|\n"
"{A100001000Half CCB|A100001000Half Lamb|A100001000Half Greek|A100001000Half Supreme|A100001000Half Classic Kiwi|A100001000Half Margherita|A100001000Half Neptune|A100001000Half Mush,Bacon&Blue|A100001000Half Chicken Indienne|A100001000Half Kids Pizza|2000000000WAIT TO CALL|2000000000TALK TO WAITER|2000000000NO CAPSICUM|2000000000NO CHEESE|2000000000NO DAIRY|2000000000NO DRESSING|2000000000NO FETA|2000000000NO Spinach|2000000000NO MUSHROOMS|2000000000NO Prawns|2000000000NO NUTS|2000000000NO ONION|2000000000NO PARMESAN|2000000000No Banana|2000000000NO SOURCREAM|2000000000NO Olives|2000000000NO TOMATO|2000000000NO YOGHURT|2000000000EASY ON THE CHEESE|2000000000TAKEAWAY|}\n"
"\n"
"[VOUCHERS]0\n"
"TF$20 Gift Voucher|46|\n"
"TF$50 Gift Voucher|46|\n"
"TFFirst National Voucher|46|\n"
"TFFood and Beverage|46|\n"
"TFRoom Hire|46|\n"
"TFJF HEINEKEN    VIP MEMBER PACK|46|\n"
"{}\n"
"\n"
"[CROCKERY BREAKAGE]0\n"
"TFDinner Plate|46|\n"
"TFLunch Plate|46|\n"
"TFSide Plate|46|\n"
"TFPasta Plate|46|\n"
"TFLatte Bowl|46|\n"
"TFLatte Cup|46|\n"
"TFFlatwhite Cup|46|\n"
"TFShortblack Cup|46|\n"
"TFMug|46|\n"
"TFLarge Saucer|46|\n"
"TFMedium Saucer|46|\n"
"TFShortblack Saucer|46|\n"
"TFMilk Jug|46|\n"
"TFGreen Sushi Plate|46|\n"
"TFNest Bowl|46|\n"
"{}\n"
"\n"
"[NOTES]0\n"
"TFNote for Kitchen|46|\n"
"TFNote for Pizza|46|\n"
"{}\n"
"\n"
/**/





/*
"|Food|\n"
"[Sizes]\n"
"1/2 Shell\n"
"18.95\n"
"2 Course\n"
"24.95\n"
"3 Course\n"
"A\n"
"B\n"
"Child\n"
"Deep Fried\n"
"Default\n"
"L\n"
"LIGHT MEAL\n"
"LM\n"
"M\n"
"MAIN MEAL\n"
"Marinated\n"
"Mornay\n"
"Set Menu Light\n"
"\n"
"[ServingCourses]\n"
"1|Entrees|T255\n"
"2|Mains|T65280\n"
"3|Desserts|T16711680\n"
"4|No Serving Course|F0\n"					//	F = not available for selection
"\n"
"[Breads]1\n"
"TFHOT BREAD|10T3|\n"
"TFGARLIC BREAD|10T3|\n"
"{2000000000HOLD|}\n"
"\n"
"[Kids Meals]2\n"
"TFCHICKEN|10|\n"
"TFW DOG|10|\n"
"TFSQUID|10|\n"
"TFFISH|10|\n"
"TFSALAD|10|\n"
"{210000001FWith Mains|210000001FWith Entrees|210000001FASAP|2000000000fish pan fried|}\n"
"\n"
"[Entree]1\n"
"TFCHOWDER|10T3|\n"
"TFSOUP|10T3|\n"
"TFE HOT POT|10|\n"
"TFSHRIMP|10|\n"
"TFPOT SKINS|10|\n"
"TFCAMEMBERT|10T3|\n"
"TFE MUSHROOM|10|\n"
"TFE SQUID|10|\n"
"TFE CHICK|10|\n"
"TFE SCALLOP|9|17|\n"
"TFEEL|10|\n"
"TFS/F COCKTAIL|10T3|\n"
"TFMUSSELS 1/2|1|16|\n"
"TFMUSSELS MAR|1|16|\n"
"TFE PRAWNS|10|\n"
"TFPLATTER|10|\n"
"{B100000003HOLD|A20000000BMain to Come|A20000000BNo Mains|A20000000Bas a main|A20000000Bserve with main/s|3400000001NO sour cream|3400000003No bacon|3400000003NO onion|B100000003forks|}\n"
"\n"
//	DisplayItemSizes (T/F) ForcedSideOnly (T/F)|Name|Size T or F or K SetMenuMask|, Forced Sides|
//	... where T or F = True/False available as standard item, K = Key

// Forced Sides: GroupNo (1-8), Max Select.
//	... where T or F = True/False 'Continue' button available
//	... where 0 = Select as many as you like then hit Continue (Must be T).
//	... 		 1 = Select 1 auto move on (or allow Continue to skip this group if T).


//
//	This comes in 3 parts:
//		1st char -> Option control bit-flags
//			Bit 3		Next Button displayed (ie allow skip)
//			Bit 2		Available as minus option
//			Bit 1		Available as plus option
//			Bit 0		(Not) Multi-Select (ie 0 = multi select, 1 = select 1 only)
//
//		2nd char -> Group number
//			They may be prompted up to 16 times with lists of forced options
//
//		8 chars  -> Bit mask of items that will force select this option
//
//	eg.	E101FFFFFF	->	Will put "Next" at top of list
//								Will display in + and - options list,
//								Will allow multi-select
//								will appear in 1st forced option list,
//								will pop up for first 25 items in course
"[Light and Main]2\n"
"TFPAN/FISH|13T3|14|\n"
"TFFISH/MORN|13T3|14|\n"
"TFD/F FISH|13|14|\n"
"TFBASKET|13T3|14|,1|T1|90|91|92|,2|T0|93|94|95|,4|T0|96|97|98|\n"
"TFSALMON|13|\n"
"TFSEAFOOD/LA|13|14|\n"
"TFCRUMB/SCAL|13|14|\n"
"TFSCALL/MORN|13T3|14|\n"
"TFSQUID|13|14|\n"
"TFFLOUNDER|10|\n"
"TFHOT/ POT|13|14|\n"
"TFRIB/EYE|13|14|\n"
"TFFILLET|10|\n"
"TFT BONE|10|\n"
"TFCHICK/TEND|13|14|\n"
"TFPAN/CHICK|13|14|\n"
"TFPORK|13|14|\n"
"TFHAM|13|14|\n"
"TFTURKEY|13|14|\n"
"TFMUSHROOM|13|14|\n"
"TFVEGE/LASAG|10|\n"
"TFSALAD/MEAL|10|\n"
"{2000000000HOLD|A100003810Rare|A100003830Med Rare|A100003830Med|A100003830Med Well|A100003810Well Done|32003FFFDFchips|32003FFFFFcros|32003FFFFFhalf & half|52003FFFFFNO pot|4000000000NO Sauce|4000000000NO Apricot Sauce|4000000000NO Mushroom Sauce|4000000000NO Sweet & Sour Sauce|4000000000NO Tomato Sauce|4000000000NO Tartare Sauce|2000000000Worcester Sauce|4000000000Sauce Seperate|4000000000NO Mussels|4000000000NO Garlic Butter|4000000000NO Pepper Butter|2000000000WITH tomato sauce|2000000000WITH Apricot Sauce|2000000000WITH Mushroom Sauce|2000000000WITH Sweet & Sour|2000000000WITH Tartare Sauce|2000000000WITH Garlic Butter|2000000000WITH Pepper Butter|}\n"
"\n"
"[Desserts]3\n"
"TFCHOC SHELL|10T2|\n"
"TFPAVLOVA|10T2|\n"
"TFCRUMBLE|10T2|\n"
"TFCHEESECAKE|10T2|\n"
"TFCHOC NUT SUND|10T2|\n"
"TFCARAMEL FUDGE|10T2|\n"
"TFXMAS PUD|10|\n"
"TFC/M Straw Sundae|10|\n"
"TFC/M Choc Sundae|10|\n"
"TFC/M Caramel Sundae|10|\n"
"TFC/M Plain Ice|10|\n"
"TFBirthday Cake|10|\n"
"TFSmall Choc Sundae|10|\n"
"TFSmall Caramel Sundae|10|\n"
"TFSmall Straw Sundae|10|\n"
"{2000000000HOLD|6000000000Cream|6000000000Ice Cream|6000000000Nuts|2000000000Candle|20000000001 x Spoon|20000000002 x Spoon|20000000003 x Spoon|2000000000Ice Cream not Cream|6000000000Sauce|6000000000100's & 1000's|2000000000Express|}\n"
"\n"
"[Coffee]0\n"
"TFCappuccino|10|\n"
"TFFlat White|10|\n"
"TFLatte|10|\n"
"TFLong Black|10|\n"
"TFShort Black|10|\n"
"TFAmericano|10|\n"
"TFMoccachino|10|\n"
"TFHot Chocolate|10|\n"
"TFTea|10|\n"
"TFHot Water|10|\n"
"TFFLUFFY|10|\n"
"TFIrish|10|\n"
"TFRoman|10|\n"
"TFMexican|10|\n"
"TFCalypso|10|\n"
"TFMonks|10|\n"
"TFBaileys|10|\n"
"TFDrambuie|10|\n"
"TFGrand Marnier|10|\n"
"TFJamanican|10|\n"
"TFOpal Nera|10|\n"
"TFSambuca|10|\n"
"{2100000001Chocolate|2100000001Cinnamon|2100000001No Dusting|2000000000WITH Trim Milk|2200000100ENGLISH BREAKFAST|2200000100EARL GREY|2200000100PEPPERMINT|2200000100CAMOMILE|2200000100GREEN TEA|2200000100LEMON|}\n"
"\n"
"[Miscellaneous]4\n"
"TFLighters|10|\n"
"TFJanola|10|\n"
"{}\n"
"\n"
"[Sides]0\n"
"TFChips|10|\n"
"TFCroquettes|10|\n"
"TFHalf and Half|10|\n"
"TFSalad|10|8|\n"
"TFEggs|10|\n"
"TFMushrooms|10|\n"
"TFTomato|10|\n"
"TFOnion|10|\n"
"TFGarlic Butter|10|\n"
"TTXtra Ice Cream|10|\n"
"{}\n"
"\n"
"[Set Menus]0\n"
"TF2 Course Set Menu|10K3|\n"
"TF3 Course Set Menu|10K2|\n"
"{}\n"
"\n"
*/


/*
"|FOOD|\n"
"[Sizes]\n"
"DEFAULT\n"
"SINGLE\n"
"DOUBLE\n"
"BOTTLE\n"
"GLASS\n"
"LARGE\n"
"SMALL\n"
"BATTERED\n"
"PANFRIED\n"
"GRILLED\n"
"TRAF LIGHT\n"
"PINK PANTH\n"
"GREEN SLIM\n"
"CARAFE\n"
"SEAFOOD\n"
"SWEET CHILLI\n"
"\n"
"[Entree]\n"
//	DisplayItemSizes (T/F) ForcedSideOnly (T/F)|Name|servingCourse;Size T or F or K SetMenuMask|, Forced Sides|
//	... where T or F = True/False available as standard item, K = Key
// Forced Sides: GroupNo (1-8), Max Select.
//	... where T or F = True/False 'Continue' button available
//	... where 0 = Select as many as you like then hit Continue (Must be T).
//	... 		 1 = Select 1 auto move on (or allow Continue to skip this group if T).
"TFGarlic Herbed Bread|1|3|4|,1|T1|1|3|10|,2|T0|4|9|12|,4|T0|1|3|4|5|22|\n"
"FFSoup Du Jour|1T7|2F2|\n"			//	7 & 2 are SetMenuMask bitmasks!
"FTSpicy Lamb Salad|1|\n"
"FFP & K Balls|1T2|\n"
"FFTrio of Fettucine|2T2|\n"
"FFDeep-Fried Camembert|1F2|\n"
"{210000000FAs Main|}\n"
"\n"
"[Mains]\n"
"TFAngus Scotch Fillet|1|\n"
"TFAngus T-Bone|1|,1|F1|1|3|10|,2|T1|4|9|12|,3|F2|1|3|4|5|22|\n"
"TFAngus Pure Sirloin|1|\n"
"TFBeef Schnitzel|1|\n"
"TFChicken Marsala|1|\n"
"TFChicken Gabriel|1|\n"
"{2100000013WELL DONE|2100000013MEDIUM WELL|2100000013MEDIUM|2100000013MEDIUM RARE|2100000013RARE|2100000013BLUE|2200000015VEGES|2200000015SALAD|230000000FFRIES|230000000FPOTATOE|6000000001NO DRESSING|6000000001NO GRAVY|6000000001NO WEDGES|6000000001NO SOUR CREAM|6000000001NO SALAD|6000000001NO VEGE|6000000001NO FRIES|6000000001NO TOMATO|6000000001NO CHEESE|6000000001NO SAUCE|6000000001NO MUS & GAR TOP|6000000001WITH WEDGES|6000000001WITH MAYO|6000000001SOUR CREAM|6000000001BUTTER|6000000001RUNNY EGGS|}\n"
"\n"
"[Steaks]\n"
"FFBulls Eye|1|\n"
"FFStags Sirloin|1|\n"
"FFGiant T-Bone|1|\n"
"FFHam Steak|1|\n"
"FFRoast Of The Day|1T1|\n"
"{610000000FField Mushrooms|410000000FGarlic butter|410000000FHerb sausage & onion|210000000FItalian Sauce & Kumara|}\n"
"\n"
"[Specials]\n"
"FFBackpackers Steak|1|\n"
"FFFish & Chips|1|\n"
"FFPancake Rock Lasagne|1T16|3T16|\n"
"FFMonday Special|1K16|\n"
"FFChicken Vol Au Vent|1|\n"
"FF3 Egg Omelette|1|\n"
"FFCampfire Roast|1|\n"
"{}\n"
"\n"
*/
/*
"|Food|\n"
"[Sizes]\n"
"1-2\n"
"4-6\n"
"Default\n"
"\n"
"[ServingCourses]\n"
"1|Entrees|255\n"
"2|Mains|32768\n"
"3|Desserts|8388608\n"
"\n"
"[Brunch]0\n"
"TFEuro Breakfast|3|\n"
"TFBircher Museli|3|\n"
"TFOatmeal Porridge|3|\n"
"TFPancakes|3|\n"
"TFFrench Toast|3|\n"
"TFFrittatta|3|\n"
"TFEggs Benedict|3|\n"
"{}\n"
"\n"
"[Salads]0\n"
"TFGreek|3|\n"
"TFCaesar|3|\n"
"TFThai Beef|3|\n"
"TFChicken Spag|3|\n"
"TFTomato & Boccocinni|3|\n"
"{}\n"
"\n"
"[Lite meals]0\n"
"TFCalamari Rings|3|\n"
"TFChowder|3|\n"
"TFChicken Wrap|3|\n"
"TFSteak Sandwich|3|\n"
"TFChicken Curry|3|\n"
"TFStir Fry|3|\n"
"TFMeatloaf|3|\n"
"TFFish & Chips|3|\n"
"{}\n"
"\n"
"[After 5]0\n"
"TFMezzi|1|2|\n"
"TFAnti|1|2|\n"
"TFMussels|3|\n"
"TFTempura Veg|1|2|\n"
"TFDeli Platter|1|2|\n"
"TFKapiti Cheese|1|2|\n"
"TFBowl of Fries|3|\n"
"{}\n"
"\n"
"[Euro Dispense Cabinet]0\n"
"TFLasagna|3|\n"
"TFBroc/Blue Cheese Filo|3|\n"
"TFPumpkin/Chicken Filo|3|\n"
"TFChicken/Capsicum Panini|3|\n"
"TFHam/Tomato  Panini|3|\n"
"TFBacon/Avo/Cheese Panini|3|\n"
"TFVegetable & Feta Panini|3|\n"
"TFSalmon            Panini|3|\n"
"TFBacon Cabatta|3|\n"
"TFBeef Cabatta|3|\n"
"TFCrossiant|3|\n"
"TFPo-Boy|3|\n"
"TFGreek Salad|3|\n"
"TFSpagetti Salad|3|\n"
"TFCaesar Salad|3|\n"
"TFOrange Ricotta Cake|3|\n"
"TFLemon Almond Cake|3|\n"
"TFVegan Cake|3|\n"
"TFChocolate Cheese Cake|3|\n"
"TFChocolate Brownie|3|\n"
"TFWhite Chocolate Tart|3|\n"
"TFAmerican Brownie|3|\n"
"TFShortbread Caramel|3|\n"
"TFEarl Gray Tea loaf Muffin|3|\n"
"TFBlueberry Muffin|3|\n"
"TFLemon Muffin|3|\n"
"TFDate Scone|3|\n"
"TFSpinnach/Capsicum Muffin|3|\n"
"TFTomato/Basil Muffin|3|\n"
"TFS O D|3|\n"
"TFMisc|3|\n"
"TFTurkish|3|\n"
"{}\n"
"\n"
*/
/*
"|FOOD|\n"
"[Sizes]\n"
"Default\n"
"Half\n"
"Large\n"
"Small\n"
"Standard\n"
"\n"
"[ServingCourses]\n"
"1|Entrees|255\n"
"22|Mains|47872\n"
"15|Desserts|16711680\n"
"\n"
"[SALAD & SIDES]1\n"
"TF1Steamed Rice|4|3|\n"
"TF2Cube (Cold tofu)|1|\n"
"TF3Spinach|1|\n"
"TF4Tofu ( Tofu Salad)|1|\n"
"TF5Soy Beans|1|\n"
"TF6Cabbge|1|\n"
"TF7Surume (Dried Squid)|1|\n"
"TF8Tako (Octopus Balls)|1|\n"
"TF9Spinach Small|1|\n"
"TF10Tofu Small|1|\n"
"{6100000080Spicy|6100000080Non Spicy|6100000000Bacon|6100000000Dressing|6100000000Mayonase|6100000000Salt|6100000000Bonito flakes|6100000000Onion|6100000000Tomato|}\n"
"\n"
"[YAKITORI]22\n"
"TF11Yakitori|1|\n"
"TF12Skin|1|\n"
"TF13Mince|1|\n"
"TF14Leek|1|\n"
"TF15Giblet|1|\n"
"TF16Livers|1|\n"
"TF17Heart|1|\n"
"TF18Wings|1|\n"
"TF19Sasami|1|\n"
"TF20Pork|1|\n"
"TF21Steak|1|\n"
"TF22Tongue|1|\n"
"TF23Sausage|1|\n"
"TF24Mush|1|\n"
"TF25Capsi|1|\n"
"TF26Squid|1|\n"
"TF27Prawns|1|\n"
"TF28Shrimp|1|\n"
"TF29Vege|1|\n"
"TF30Kumara|1|\n"
"TF31Garlic|1|\n"
"TF32Corn|1|\n"
"TF33Yasai|1|\n"
"TF34FT|1|\n"
"TF35S-combo|1|\n"
"TF36RB|1|\n"
"TF37Y-set|1|\n"
"TF38V-set|1|\n"
"TF39Donburi|1|\n"
"TF40S-don|1|\n"
"TF41B Steak Wiz Anxhovy|1|\n"
"TF42C Eggplant Wiz Ginger soy|1|\n"
"{6000000000TARE|6000000000SALT|6000000000BUTTER|6000000000YAKITORI|6000000000MINCE|6000000000LEEK|6000000000PORK|6000000000WING|6000000000GARLIC|6000000000ZUCCHINI|6000000000CORN|6000000000YASAI|6000000000TOFU|6000000000SPINACH|6000000000SPINACH NO BACON|6000000000PEPPER|6000000000V-CAPSI|6000000000V-PUMP|6000000000V-ONION|6000000000MUSTARD|}\n"
"\n"
"[KUSHIAGE]22\n"
"TF43Onion|1|\n"
"TF44Pumpkin|1|\n"
"TF45Squidgy Stuff|1|\n"
"TF46Kumara|1|\n"
"TF47Steak|1|\n"
"TF48Pork|1|\n"
"TF49Chicken|1|\n"
"TF50Wing|1|\n"
"TF51Shitake|1|\n"
"TF52Cheese|1|\n"
"TF53Prawns|1|\n"
"TF54Squid|1|\n"
"TF55Fish|1|\n"
"TF56Sasami|1|\n"
"TF57Banana|1|\n"
"TF58Almond|1|\n"
"TF59EX-Mayo|1|\n"
"TF60EX-Tar tar|1|\n"
"TF61A Tofu & Chicken Patties|1|\n"
"TF62D Greentea Icecream|1|\n"
"{}\n"
"\n"
"[DESSERT]15\n"
"TFICE CREAM|1|\n"
"TFAPPLE ICE|1|\n"
"TFBANANA ICE|1|\n"
"TFKUMARA ICE|1|\n"
"TFCHOC CHOC|1|\n"
"TFAPPLE CHOC|1|\n"
"TFBANANA CHOC|1|\n"
"TFKUMARA CHOC|1|\n"
"TFEX VANILLA|1|\n"
"TFEX CHOC|1|\n"
"{6000000000CREAM|6000000000MAPLE SYRUP|}\n"
"\n"
"[EXTRA]0\n"
"TFChoc ICE|1|\n"
"TFVanilla ICE|1|\n"
"TFSpinach Dressing|1|\n"
"TFMayo|1|\n"
"TFWasabi|1|\n"
"TFSeaweed|1|\n"
"TFYakatori Sauce|1|\n"
"TFTartare Sauce|1|\n"
"TFMustard Sauce|1|\n"
"TFMustard Mayo Sauce|1|\n"
"TFWasabi Mayo|1|\n"
"{}\n"
"\n"
"[YAKITORI HALF]22\n"
"TFYakitori 1/2|1|\n"
"TFSkin 1/2|1|\n"
"TFMince 1/2|1|\n"
"TFLeek 1/2|1|\n"
"TFGiblet 1/2|1|\n"
"TFLiver 1/2|1|\n"
"TFHeart 1/2|1|\n"
"TFWing 1/2|1|\n"
"TFPork 1/2|1|\n"
"TFSteal 1/2|1|\n"
"TFTongue 1/2|1|\n"
"TFSausage 1/2|1|\n"
"TFSquid 1/2|1|\n"
"TFPrawn|1|\n"
"TFShrimp 1/2|1|\n"
"TFKumara 1/2|1|\n"
"TFGarlic 1/2|1|\n"
"TFCorn 1/2|1|\n"
"TFYasai 1/2|1|\n"
"TFRB 1/2|1|\n"
"{6000000000Tare|6000000000Salt|}\n"
"\n"
"[KUSHIAGE HALF]22\n"
"TFOnion 1/2|1|\n"
"TFPumpkin 1/2|1|\n"
"TFKumara 1/2|1|\n"
"TFSteak 1/2|1|\n"
"TFPork 1/2|1|\n"
"TFChicken 1/2|1|\n"
"TFWing 1/2|1|\n"
"TFShitake 1/2|1|\n"
"TFCheese 1/2|1|\n"
"TFPrawns 1/2|1|\n"
"TFSquid 1/2|1|\n"
"TFFish 1/2|1|\n"
"TFSasami 1/2|1|\n"
"TFBanana 1/2|1|\n"
"TFAlmond 1/2|1|\n"
"{}\n"
"\n"
*/


);
/*
	}
	else	//	Must be 2nd Dishes menu
	{

		StrCopy((char*)ALReceivePtr,

"~~~~~~~~~~"
"||\n"
"[Sizes]\n"
"Default\n"
"\n");
	}
*/
}
else
{
//	if(toggleDrinks)
//	{

		StrCopy((char*)ALReceivePtr,

"~~~~~~~~~~"
"|BAR|\n"
"[Sizes]\n"
"$4 Glass\n"
"$4 PINT\n"
"200ml bottle\n"
"241 Pint\n"
"Apple\n"
"BLUE\n"
"BLUECHEESE\n"
"BOTTLE\n"
"Bacardi\n"
"Baileys\n"
"CARAMEL\n"
"CATALINA\n"
"CHOC\n"
"COKE\n"
"Citrus\n"
"Cointreau\n"
"DEFAULT\n"
"DOUBLE\n"
"DOZ\n"
"Dbl with premiuim juice\n"
"Dbl with standard juice\n"
"FAMILY\n"
"FANTA\n"
"Frangelico\n"
"GLASS\n"
"Gin\n"
"HALF\n"
"HANDLE\n"
"LARGE\n"
"MEDIUM\n"
"MEDIUM RARE\n"
"MEDIUM WELL\n"
"NYE SPECIAL\n"
"Orange\n"
"PACK\n"
"PERFECT POUR\n"
"PESTO\n"
"PINT\n"
"PIZZA / PINT DEAL\n"
"RARE\n"
"RASPBERRY\n"
"REGULAR\n"
"SINGLE\n"
"SINGLE\n"
"SMALL\n"
"SPECIAL PINT\n"
"SUNDRIED TOMATO\n"
"TAPENADE\n"
"TWO BOTTLES\n"
"UNIT\n"
"VINAIGRETT\n"
"Vanilla\n"
"Vodka\n"
"WELL DONE\n"
"WHOLE\n"
"\n"
"[ServingCourses]\n"
"5|Beverages|T16711680\n"
"1|No Serving Course|F32896\n"
"\n"
"[BEER]5\n"
"TFDB Export|25|12|38|2|4|39|\n"
"TFExport Gold  EXTRA COLD|17|\n"
"TFDB Draught|25|12|38|2|4|39|\n"
"TFMonteiths Black|25|12|38|2|4|39|\n"
"TFMonteiths Original|25|12|38|2|4|39|\n"
"TFMonteiths Pilsner|25|12|38|2|4|39|\n"
"TFMonteiths Radler|25|12|38|2|4|39|\n"
"TFTui|25|12|38|2|4|39|\n"
"TFAmstel Light Bulk|25|12|38|2|4|39|\n"
"TFHeineken XLN|17|\n"
"TFMonteith's Celtic Red|17|\n"
"TFMonteith's Winter Ale|17|\n"
"TFMonteiths Summer Ale|17|\n"
"TFMonteith's Golden|17|\n"
"TFBecks|17|\n"
"TFCarlsberg Green|17|\n"
"TFCascade Lager|17|\n"
"TFCrown Lager|17|\n"
"TFDB Export Dry|17|\n"
"TFGrolsch|17|\n"
"TFGuinness|17|\n"
"TFHoegaarden|17|\n"
"TFSol|17|\n"
"TFTiger|17|\n"
"TFHeineken 650ml|17|\n"
"TFBudvar|17|\n"
"TFCoopers Sparkling Ale|17|\n"
"TFLimburg Czech|17|\n"
"TFLimburg Witbier|17|\n"
"{2000000000LIME|2000000000SHANDY|}\n"
"\n"
"[NON ALCHOLIC]5\n"
"TFOrange Juice|17|\n"
"TFGrapefruit Juice|17|\n"
"TFCranberry Juice|17|\n"
"TFPineapple Juice|17|\n"
"TFTomato Juice|17|\n"
"TFAppletiser|17|\n"
"TFApple & Raspberry Juice|17|\n"
"TFNectarine Juice|17|\n"
"TFApple & Boysenberry Juice|17|\n"
"TFApple Juice|17|\n"
"TFApricot Juice|17|\n"
"TFBundaberg Ginger Beer|17|\n"
"TFCoke|29|45|\n"
"TFDiet Coke|29|45|\n"
"TFLemonade|29|45|\n"
"TFFanta|29|45|\n"
"TFGingerale|29|45|\n"
"TFL & P|29|45|\n"
"TFTonic Water|29|45|\n"
"TFSoda|17|\n"
"TFLemonade Lime & Bitters|29|45|\n"
"TFLime & Soda|29|45|\n"
"TFCoke & Raspberry|29|45|\n"
"TFLemonade & Raspberry|29|45|\n"
"TFSquash & Lemonade|29|45|\n"
"TFRed Bull|17|\n"
"TFKiwi Blue Mineral Water|17|\n"
"TFSpider|14|23|41|\n"
"TFMilk|17|\n"
"TFStaff Soda|25|38|\n"
"{2000000000NO ICE|2000000000SPICY|2000000000BITTERS|2000000000NO CREAM|2110000000Trim|2110000000Blue|2400000000Iced Chocolate|2400000000Iced Coffee|2400000000Banana|2400000000Berry|2200000010Spiced|2200000010Unspiced|}\n"
"\n"
"[WHITE WINES]5\n"
"TFJacobs Creek (200ml)|17|\n"
"TFLindauer Fraise|17|,1|F1|16416|16417|16418|16419|,2|T0|16428|16429|16430|16431|\n"
"TFSelaks Sparkling|8|\n"
"TFDuetz 200ml|8|\n"
"TFDuetz 750ml|8|\n"
"TFCloudy Bay Pelorus|8|\n"
"TFBollinger|8|\n"
"TFDrylands Sauv Blanc|8|25|\n"
"TFMount Riley SB|8|25|\n"
"TFStation Road|8|25|\n"
"TFShingle Peak SB|8|25|\n"
"TFPegasus Bay Sauv Sem|8|25|\n"
"TFAstrolabe Sauv|8|25|\n"
"TFGunn Estate Skeetfield|8|25|\n"
"TFSacred Hill Chardonnay|8|25|\n"
"TFNobilo PB Chard|8|25|\n"
"TFChard Farm Closeburn|8|25|\n"
"TFSpy Valley Chardonnay|8|25|\n"
"TFJudd Estate|8|25|\n"
"TFClearview Reserve|8|\n"
"TFDrylands Chardonnay|8|25|\n"
"TFWild Earth Pinot Gris|25|8|\n"
"TFCamshorn Dry Riesling|8|25|\n"
"TFZilzie Viognier|8|25|\n"
"TFMain Divide Riesling|8|25|\n"
"TFHuntaway Pinot Gris|8|25|\n"
"TFJohanneshof Gewurz|8|25|\n"
"TFAmisfield Rocky Roll Riesling|8|\n"
"TF$4 Monthly Sauv|1|\n"
"TF$4 Monthly Chard|1|\n"
"{}\n"
"\n"
"[RED WINES]5\n"
"TFDrylands Pinot Noir|8|25|\n"
"TFChard Farm Fin La Mor|8|25|\n"
"TFRabbit Ranch|8|25|\n"
"TFMelness Pinot Noir|8|25|\n"
"TFAmisfield|8|\n"
"TFVilla Maria Reserve|8|\n"
"TFMontrose Black Shiraz|8|25|\n"
"TFHardy's Oomoo shiraz|8|25|\n"
"TFPeter Lehmann Shiarz|8|25|\n"
"TFTaylor's Cabernet|8|25|\n"
"TFRosemount Cab Merlot|8|25|\n"
"TFJim Barry Coverdrive|8|25|\n"
"TFMt Riley Merlot Malbec|8|25|\n"
"TFMatheson Merlot Cab|8|25|\n"
"TFSelaks Merlot|8|25|\n"
"TFRed Rock Merlot|8|25|\n"
"TF$4 Monthly red|1|\n"
"TF$5 Corkage|17|\n"
"TF$10 Corkage|17|\n"
"{}\n"
"\n"
"[HOUSE SPIRITS]5\n"
"TFChatelle Brandy|18|43|21|\n"
"TFCoruba Rum|18|43|21|\n"
"TFBacardi|18|43|21|\n"
"TFJohnnie Walker Red|18|43|\n"
"TFHeaven Hill|18|43|\n"
"TFSmirnoff Vodka|18|43|21|\n"
"TFGordons Gin|18|43|21|\n"
"{}\n"
"\n"
"[PREMIUM SPIRITS]5\n"
"TFJim Beam Bourbon|18|43|\n"
"TFAppleton Rum|18|43|\n"
"TFBombay Sapphire|18|43|\n"
"TFCaptain Morgan Spiced|18|43|\n"
"TFTangueray Gin|18|43|\n"
"TFBundaberg Rum|18|43|\n"
"TFJack Daniels|18|43|\n"
"TFSouthern Comfort|18|43|\n"
"TFSkyy Vodka|18|43|\n"
"TF42 Below Kiwi|18|43|\n"
"TF42 Below Feijoa|18|43|\n"
"TF42 Below Passionfruit|18|43|\n"
"TF42 Below Manuka Honey|18|43|\n"
"TFSouth Gin|18|43|\n"
"TFSmirnoff Range|5|15|34|52|\n"
"TFAbsoult Rasberri|18|43|\n"
"TFAbsoult Mandarin|18|43|\n"
"{2000000000TONIC|2000000000LEMONADE|2000000000GINGERALE|2000000000DIET|2000000000SODA|2000000000LIME|2000000000L & P|2000000000NO ICE|2000000000BITTERS|2000000000MILK|2000000000ORANGE JUICE|2000000000PINEAPPLE JUICE|2000000000CRANBERRY JUICE|2000000000GRAPEFRUIT JUICE|2000000000TALL|2000000000COKE|}\n"
"\n"
"[SPECIALTY WHISKYS/TEQUILA]5\n"
"TFBalvenie Malt|18|43|\n"
"TFCanadian Club|18|43|\n"
"TFCourvoisier VSOP Cognac|18|43|\n"
"TFCragganmore Malt Whisky|18|43|\n"
"TFGlenfiddich|18|43|\n"
"TFJ & B Rare Whiskey|18|43|\n"
"TFJamesons Whiskey|18|43|\n"
"TFJohnnie Walker Black|18|43|\n"
"TFLaphroig|18|43|\n"
"TFMakers Mark Bourbon|18|43|\n"
"TFThe Macallan Malt|18|43|\n"
"TFTalisker|18|43|\n"
"TFThe Glenlivet 12YO Whiskey|18|43|\n"
"TFOld Bushmills Black Bush Whiskey|18|43|\n"
"TFCourvoisier VS 3 Star Cognac|18|43|\n"
"TFGlen Farclas 10YO Malt|18|43|\n"
"TFGlen Ord 12YO Single Malt|18|43|\n"
"TFCaol lla 12YO Single Malt|18|43|\n"
"TFClynelish 14YO Single Malt|18|43|\n"
"TFBulleit Bourbon|18|43|\n"
"TFJose Cuervo G C Reposado Tequila|18|43|\n"
"TFJose Cuervo Gran Centario Plata Tequila|18|43|\n"
"TFJose Cuervo Tradicional Tequila|18|43|\n"
"TFCuervo Gold Tequila|18|43|\n"
"TFAgavero Tequila|18|43|\n"
"{}\n"
"\n"
"[COCKTAILS / RTDs]5\n"
"TFBlack Russian|18|43|\n"
"TFBloody Mary|17|\n"
"TFHarvey Wall Banger|17|\n"
"TFQuick F**k|17|\n"
"TFCocksucking Cowboy|17|\n"
"TFMargarita|17|\n"
"TFCocktail Special|9|16|24|53|26|10|\n"
"TFJF MASSIVE MOJITIO|17|\n"
"TFGinger Spritza|17|\n"
"TFPimms Punch|18|43|\n"
"TFArchers Lime|17|\n"
"TFArchers Raspberry|17|\n"
"TFArchers Peach|17|\n"
"TFArchers Apple|17|\n"
"TFSkyy|17|\n"
"TFBaileys Glide|17|\n"
"TFSmirnoff Ice|17|\n"
"TFSmirnoff Double Black|17|\n"
"TFHarvest Draught Cider|17|\n"
"TFBrightstone Dry Cider|17|\n"
"TFRed Bull and Vodka|17|\n"
"{}\n"
"\n"
"[PORT/ SHERRY/CIGARETTES]5\n"
"TFPykes Tawny Port|8|25|\n"
"TFTaylors Spec Tawny Port|17|\n"
"TFTaylors Spec Ruby Port|25|\n"
"TFTaylors 10 Yr OLd|25|\n"
"TFSherry Harveys Bristol Cream|25|\n"
"TFB & H Golden Mild 20s|17|\n"
"TFMatches|17|\n"
"TFPall Mall Menthol 20s|17|\n"
"TFB & H Special Filter 20s|17|\n"
"{}\n"
"\n"
"[LIQUEURS A-F]5\n"
"TFAmaretto|18|43|\n"
"TFArchers Peach Schnapps|18|43|\n"
"TFApple Liqueur|18|43|\n"
"TFBaileys|18|43|36|\n"
"TFBanana Liqueur|18|43|\n"
"TFBlue Curacao|18|43|\n"
"TFButterscotch Liqueur|18|43|\n"
"TFCassis|18|43|\n"
"TFCampari|18|43|\n"
"TFCointreau|18|43|\n"
"TFCreme de Cacao Brown|18|43|\n"
"TFCreme de Caco White|18|43|\n"
"TFCreme de Menthe|18|43|\n"
"TFDe Kuyper Wild Strawberry|18|43|\n"
"TFDrambuie|18|43|\n"
"TFDubonnet|18|43|\n"
"TFFrangelico|18|43|\n"
"{2000000000COKE|2000000000TONIC|2000000000LEMONADE|2000000000GINGERALE|2000000000DIET|2000000000SODA|2000000000LIME|2000000000L & P|2000000000BITTERS|2000000000NO ICE|2000000000MILK|2000000000TALL|}\n"
"\n"
"[LIQUEURS G-Z]5\n"
"TFJagermeister|18|43|\n"
"TFKahlua|18|43|\n"
"TFMartini Extra Dry|18|43|\n"
"TFMalibu|18|43|\n"
"TFMidori|18|43|\n"
"TFOpal Nera|18|43|\n"
"TFOpal Nera Bianco|18|43|\n"
"TFOuzo|18|43|\n"
"TFPimms|18|43|\n"
"TFStones Ginger Wine|18|43|\n"
"TFGreen Chartreuse|18|43|\n"
"TFGrappa|18|43|\n"
"TFTia Maria|18|43|\n"
"TFGalliano|18|43|\n"
"TFGlayva|18|43|\n"
"TFGrand Marnier|18|43|\n"
"TFSabra Choc-Orange|18|43|\n"
"TFTriple Sec|18|43|\n"
"TFMango Liqueur|18|43|\n"
"TFVanilla Vodka Cream|18|43|\n"
"{2000000000LIME|2000000000L & P|2000000000BITTERS|2000000000NO ICE|2000000000MILK|2000000000TALL|2000000000COKE|2000000000TONIC|2000000000LEMONADE|2000000000GINGERALE|2000000000DIET|2000000000SODA|}\n"
"\n"
"[BREAKAGES]5\n"
"TFHalf Pint|17|\n"
"TFPint|17|\n"
"TFCatalina Pilsner 355 ml|17|\n"
"TFWater Glass|17|\n"
"TFWine Glass|17|\n"
"TFSparkling|17|\n"
"TFIrish Coffee 250 ml|17|\n"
"TFLarge Wine|17|\n"
"TFLarge Sparkling|17|\n"
"TFSoda 355 ml|17|\n"
"TFTumbler Colour Dimple|17|\n"
"TFPort Glass|17|\n"
"TFBrandy|17|\n"
"TFSmall Shot|17|\n"
"TFTall Shot|17|\n"
"TFTulip Glass|17|\n"
"TFInverness Tumbler|17|\n"
"TFMartini Small|17|\n"
"TFMartini Large|17|\n"
"TFPlain Tumbler|17|\n"
"{}\n"
"\n"

/*
"|Beverages|\n"
"[Sizes]\n"
"12oz Glass\n"
"12oz Shandy\n"
"500ml\n"
"Baby Bottle\n"
"Bottle\n"
"Carafe\n"
"Default\n"
"Double\n"
"Glass\n"
"Jug\n"
"Kids-Regular\n"
"Kids-Small\n"
"Old Boys Special 12oz\n"
"Pint\n"
"Single\n"
"Triple\n"
"Unit\n"
"\n"
"[ServingCourses]\n"
"1|No Serving Course|F32896\n"
"5|Beverages|T16711680\n"
"\n"
"[Tap Beer]5\n"
"TFLion Brown|1|14|2|13|\n"
"TFSpeights|1|14|2|13|\n"
"TFMac's Gold|1|14|2|13|\n"
"TFCanterbury Draugh|1|14|2|13|\n"
"{2000000000Raspberry|2000000000Lime|}\n"
"\n"
"[Bottle Beer]5\n"
"TFDB Export Gold|5|\n"
"TFHeineken|5|\n"
"TFSteinlager|5|\n"
"TFSteinlager Lite|5|\n"
"TFStella Artois|5|\n"
"TFSpeights Old Dark|5|\n"
"{2000000000With half glass of lemonade|2000000000Lime|}\n"
"\n"
"[Spirits]5\n"
"TFBacardi|15|8|\n"
"TFBourbon|15|8|\n"
"TFBrandy|15|8|\n"
"TFGin|15|8|\n"
"TFRum|15|8|\n"
"TFVodka|15|8|\n"
"TFWhiskey|15|8|\n"
"TFJack Daniels|15|8|\n"
"TFPimms|15|8|\n"
"TFSouthern Comfort|15|8|\n"
"TFTequila|15|8|\n"
"{21000007FFCoke|21000007FFDiet Coke|21000007FFLemonade|21000007FFSoda|21000007FFGingerale|21000007FFLime|21000007FFRaspberry|21000007FFTonic|21000007FFWater|2000000000On Ice|2000000000No Ice|2000000000Tall Glass|21000007FFLemon Squash|21000007FForange juice|2100000000Lemon Juice|21000007FFL & P|2000000000No Cream|2000000000No Lemon|2000000000Slice Lemon|2000000000Spiced|2000000000With Ice|2000000000dash of|21000007FFFanta|2000000000In a 12oz glass|}\n"
"\n"
"[Wine]5\n"
"TFRiverlands Chardonnay|9|5|\n"
"TFRiverlands Sauv Blanc|9|5|\n"
"TFRiverlands Cabernet|9|5|\n"
"TFVilla Maria PB Riesling|9|5|\n"
"TFMontana Gisborne Chard|9|5|\n"
"TFVilla Maria PB Chard|9|5|\n"
"TFChurch Road Chard|9|5|\n"
"TFVilla PB Sav Blanc|9|5|\n"
"TFVidal Sauv Blanc|9|5|\n"
"TFVilla Maria VS Chard/Chen|9|5|\n"
"TFCorbans Muller Thurgau|9|5|\n"
"TFLindauer Brut|4|5|\n"
"TFLindauer Fraise|4|5|\n"
"TFBernadino Spumante|5|\n"
"TFVilla Maria V.S.C.M.Shira|9|5|\n"
"TFVilla Maria PB Merlot Cab|9|5|\n"
"TFManara Rock Cab Sauv|9|5|\n"
"TFChurch Road Merlot Cab.|9|5|\n"
"TFManara Rock Shiraz|9|5|\n"
"TFStoneleigh Pinot Noir|9|5|\n"
"{20000000001 Glass|20000000002 Glasses|20000000003 Glasses|20000000004 Glasses|20000000005 Glasses|20000000006 Glasses|20000000007 Glasses|200000000010 Glasses|2000000000In a 12oz glass|20000000001/2 Lemonade|}\n"
"\n"
"[Managers List]5\n"
"TFRuben Hall Chard|9|5|\n"
"TFRuben Hall Sav Blanc|9|5|\n"
"TFManara Rock Chard|5|\n"
"TFOyster Bay Sav Blanc|9|5|\n"
"TFRuben Hall Cab/Merlot|9|5|\n"
"TFPreece Shiraz|9|5|\n"
"TFEsk Valley /Merlot|5|\n"
"TFVidal Estate Pinot Noir|5|\n"
"TFVidal Estate Cab Sav Mer|5|\n"
"{20000000001 Glass|20000000002 Glasses|20000000003 Glasses|20000000004 Glasses|20000000005 Glasses|20000000006 Glasses|20000000007 Glasses|20000000008 Glasses|20000000009 Glasses|2000000000In a 12oz Glass|20000000001/2 Lemonade|}\n"
"\n"
"[Cask Wine]5\n"
"TFCask Dry|9|6|\n"
"TFCask Medium|9|6|\n"
"TFCask Red|9|6|\n"
"{}\n"
"\n"
"[Non Alcoholic]5\n"
"TFPost Mix|1|14|10|11|12|\n"
"TFGingerale|1|14|11|12|\n"
"TFWater Glass|1|14|11|12|\n"
"TFWater Bottle|5|\n"
"TFLem Lime Bit|1|14|11|12|\n"
"TFApple Juice|1|14|\n"
"TFOrange Juice|1|14|11|\n"
"TFCranberry Juice|1|\n"
"TFGrapefruit Juice|1|\n"
"TFPineapple Juice|1|\n"
"TFTomato Juice|1|14|\n"
"TFKids Pink Panther|9|\n"
"TFKids Traffic Lights|9|\n"
"TFKids Green Slime|9|\n"
"TFKids Cold Milo|9|\n"
"TFKids Thick Shake|9|\n"
"TFIce Chocolate|9|\n"
"TFBundaberg Ginger Beer|5|\n"
"{2000000000Lime|2000000000Raspberry|2000000000Lemon Squash|2000000000Sasparilla|2000000000Cloves|2000000000No Ice|2000000000With Ice|2000000000See Server|2200000400Spiced|2200000400Not Spiced|2000000000Bitters|2300000000No Glasses|23000000081 Glass|23000000082 Glasses|23000000083 Glasses|23000000084 Glasses|23000000085 Glasses|23000000086 Glasses|23000000088 Glasses|230000000810 Glasses|230000000820 Glasses|2100000001Coke|2100000001Coke & Raspberry|2100000001Diet Coke|2100000001Lemonade|2100000001Lemonade & Rasp|2100000001Orange|2100000001Orange & Lemonade|2100000001Fanta|2100000001Soda|2100000001Tonic|2100000001L & P|}\n"
"\n"
"[Mocktails]5\n"
"TFIce Cream Soda|9|\n"
"{}\n"
"\n"
"[Spirit Bottles]5\n"
"TFSmirnoff Ice (Red)|5|\n"
"TFJim Beam & Cola|5|\n"
"TFCoruba & Cola|5|\n"
"TFBrightsone Cider|5|\n"
"{2000000000No Ice|2000000000Tall Glass|2000000000In a 12oz glass|}\n"
"\n"
"[Cocktails]5\n"
"TFCustom Cocktail|9|\n"
"TFBrandy Alexander|9|\n"
"TFBloody Mary|9|\n"
"TFBlack Russian|9|\n"
"TFDry Martini|9|\n"
"TFHarvey Wallbanger|9|\n"
"TFJapanese Slipper|9|\n"
"TFLong Island Ice Tea|9|\n"
"TFLong Tall Cowboy|9|\n"
"TFMaidens Prayer|9|\n"
"TFMargarita|9|\n"
"TFPina Colada|9|\n"
"TFSea Breeze|9|\n"
"TFWhite Russian|9|\n"
"TFMidori Splice|9|\n"
"TFEgg-nog|1|\n"
"{}\n"
"\n"
"[Liqueurs A - J]5\n"
"TFArmaretto|15|8|\n"
"TFBaileys|15|8|\n"
"TFCinzano|15|8|\n"
"TFCointreau|15|8|\n"
"TFCreme de Cacao|15|8|\n"
"TFCreme de Menthe|15|8|\n"
"TFDrambuie|15|8|\n"
"TFFrangelico|15|8|\n"
"TFGalliano|15|8|\n"
"TFGalliano Sambuca- Black|15|8|\n"
"TFGrand Marnier|15|8|\n"
"TFGreen Ginger|15|8|\n"
"{2000000000Coke|2000000000Diet Coke|2000000000Lemonade|2000000000Orange|2000000000Fanta|2000000000Tonic|2000000000Soda|2000000000Gingerale|2000000000Raspberry|2000000000Lime|2000000000Lemon Squash|2000000000Orange Juice|2000000000Lemon Juice|2000000000Milk|2000000000With Ice|2000000000On Ice|2000000000No Ice|2000000000Water|2000000000No Cream|2000000000No Lemon|2000000000Slice Lemon|2000000000Spiced|2000000000Tall Glass|2000000000In a 12oz glass|}\n"
"\n"
"[Liqueurs K - Z]5\n"
"TFKahlua|15|8|\n"
"TFMalibu|15|8|\n"
"TFMidori|15|8|\n"
"TFOuzo|15|8|\n"
"TFSchnapps Peach|15|8|\n"
"TFTia Maria|15|8|\n"
"TFVok Advocat Cherry|15|8|\n"
"TFVok Blue Curacao|15|8|\n"
"TFVok Advocat Yellow|15|8|\n"
"TFVok Parfait Amour|15|8|\n"
"TFVok Triple Sec|15|8|\n"
"TFOpal Nera White|15|8|\n"
"TFOpal Nera Black|15|8|\n"
"{2000000000Coke|2000000000Diet Coke|2000000000Lemonade|2000000000Orange|2000000000Fanta|2000000000Tonic|2000000000Soda|2000000000Gingerale|2000000000Raspberry|2000000000Lime|2000000000Lemon Squash|2000000000Orange Juice|2000000000Lemon Juice|2000000000Milk|2000000000With Ice|2000000000On Ice|2000000000No Ice|2000000000Water|2000000000No Cream|2000000000No Lemon|2000000000Slice Lemon|2000000000Spiced|2000000000Tall Glass|2000000000In a 12oz glass|}\n"
"\n"
"[Sherry & Ports]5\n"
"TFSherry Medium|9|\n"
"TFSherry Dry|9|\n"
"TFPort Tawny|9|\n"
"TFTawny Port 10yr|9|\n"
"{}\n"
"\n"
"[Miscellaneous]5\n"
"TFMiscellaneous|17|\n"
"{}\n"
"\n"
*/


/*
"|Drinks Menu|\n"
"[Sizes]\n"
"Default\n"
"Glass\n"
"Bottle\n"
"MemHandle\n"
"Can\n"
"Jug\n"
"Sml Carf\n"
"Lge Carf\n"
"Tumbler\n"
"Int16\n"
"Tall\n"
"Single\n"
"Double\n"
"\n"
"[Beer]\n"
//	DisplayItemSizes|Name|Size,Price TorF SetMenuMask|(TorF=available as standard item)
"TDB Draught|2|3|4|6|\n"
"TLion Brown|3|4|6|\n"
"TCD Draught|2|3|4|6|\n"
"TDB Export|2|4|6|\n"			//	NB. || => Size not available for this drink.
"TSteinlager|2|3|4|6|\n"
"{Beer Mat|Straw|Ice|Umbrella|}\n"
"\n"
"[Wine]\n"
"TSauvognon Blanc|2|\n"
"TChardonay|3|2|\n"
"TLindauer Special Reserve|3|2|\n"
"TRiverlands Cabernet Sauv|3|2|\n"
"TRosemount Shiraz|3|\n"
"TSaints Cab Merlot|3|2|\n"
"THouse Dry|2|\n"
"THouse Medium|2|\n"
"{Straw|Lemon|}\n"
"\n"
"[Non Alcoholic]\n"
"TCoke|2|7|8|\n"
"TDiet Coke|2|7|8|\n"
"TRaspberry|2|7|8|\n"
"TLemonade|2|7|8|\n"
"TTropicana Orange|2|\n"
"TOrange Juice|2|\n"
"TGrapefruit Juice|2|\n"
"TPineapple Juice|2|\n"
"TTomato Juice|2|\n"
"TMineral Water|2|\n"
"TAppletise|2|\n"
"TGrapetise|2|\n"
"{Diet|Straw|Lemon|}\n"
"\n"
"[Spirits]\n"
"TBacardi|12|13|\n"
"TBourbon|12|13|\n"
"TBrandy|12|13|\n"
"TGin|12|13|\n"
"TRum|12|13|\n"
"TVodka|12|13|\n"
"TWhiskey|12|13|\n"
"TJack Daniels|12|13|\n"
"TKahlua|12|13|\n"
"TAppletons Rum|12|13|\n"
"TBudaberg Rum|12|13|\n"
"TChivas Regal|12|13|\n"
"TGlenfidich Scotch|12|13|\n"
"TBaileys|12,350|13,550|\n"
"TGrand Marnier|12|13|\n"
"{Tall Glass|No Ice|Water|Coke|Diet Coke|Lemonade|Diet Lemonade|L & P|Raspberry|Soda|Tonic|}\n"
"\n"
"[RTDs]\n"
"TBrightstone Cider|3|\n"
"TCoruba & Cola|3|\n"
"TGinger Beer|3|\n"
"TJack Daniels & Cola|3|\n"
"TStolli. Lemon Russki|3|\n"
"TMidori & Lemonade|3|\n"
"TMiami Wine Cooler|3|\n"
"TV|3|\n"
"{}\n"
"\n"
"[Aperitifs, Port & Sherry]\n"
"TArtillery Port|2|\n"
"TBarros 10 Yr Port|2|\n"
"TCognac Remy|2|\n"
"THarveys Fino Sherry|2|\n"
"TOld Masters Cream Sherry|2|\n"
"{}\n"
*/"\n");
/*
	}
	else	//	Must be 2nd Drinks Menu
	{

		StrCopy((char*)ALReceivePtr,

"~~~~~~~~~~"
"||\n"
"[Sizes]\n"
"Default\n"
"\n");

	}
*/
}

messageLength = StrLen((char*)ALReceivePtr);

#endif


	messageLength -= 2;				//	Don't attempt to Parse the CRC!
	
	
	i = HLHeaderSize;
	j = 0;

	//	Set up to use either the Dishes or Drinks DBs
	if(menuType == DishesMenu)
	{
		menuID = DishesMenu;
		menuDB = &MmDishesDB;
		sizesDB = &MmDishesSizesDB;
		optionsDB = &MmDishesOptionsDB;
		forcedSidesDB = &MmDishesForcedSidesDB;
		menuNamePtr = Prefs.dishesMenuName;
		itemCounter = 0;
	}
	else
	{
		menuID = DrinksMenu;
		menuDB = &MmDrinksDB;
		sizesDB = &MmDrinksSizesDB;
		optionsDB = &MmDrinksOptionsDB;
		forcedSidesDB = &MmDrinksForcedSidesDB;
		menuNamePtr = Prefs.drinksMenuName;
		itemCounter = 0x4000;						//	Bit 14 is set for Drinks
	}



	//	If there's nothing to parse, go back right now
	if(messageLength <= HLHeaderSize)
	{
		StrCopy(menuNamePtr, "Not Loaded");			//	Stick in something to display
		return;
	}









/*
"|Drinks Menu|\n"
"[Sizes]\n"
*/
	//	Find the |Menu Name| block in message
	while(i<messageLength && ALReceivePtr[i] != '|')	//	Find the first |
	{
		if(ALReceivePtr[i] == '[')							//	Oops. There's no name
			break;
		i++;
	}

	if(ALReceivePtr[i] != '[')								//	OK. There is a name
	{
		i++;														//	Get past the |
	
/*		j=0;
		//	Get the menuID first
		while(i<messageLength && ALReceivePtr[i] != '|')//	Copy the menuID
		{
			tempString[j++] = ALReceivePtr[i++];		//	Copy the string until the |
		}
		tempString[j] = 0;
		if(StrLen(tempString) > 0)
			record.menuID = (UInt8)StrAToI(tempString);

		i++;														//	Step over the |
*/	
		//	Now get the menu name
		j=0;
		while(ALReceivePtr[i] != '|')
		{
			menuNamePtr[j++] = ALReceivePtr[i++];		//	Copy the string until the |
			if(j > MaxMenuItemSize)
				j = MaxMenuItemSize;							//	Don't accept too long strings
		}
		menuNamePtr[j] = 0;
	}
	else
	{
		StrCopy(menuNamePtr, "Unknown Name");			//	Stick in something to display
	}



/*
"[Sizes]\n"
"Glass\n"
"Bottle\n"
"Single\n"
"Double\n"
"\n"
*/


	//	First get the sizes
	sizeCounter = 1;
	recSizeIndex = 1;										//	Sizes Indices start at 1

	//	Find [Size] block in string
	while(i<messageLength -1 && ALReceivePtr[i] != '[')
		i++;
	i++;															//	Get past the [

	j=0;
	while(i<messageLength -1 && ALReceivePtr[i] != ']')
	{
		tempString[j++] = ALReceivePtr[i++];			//	Copy the string until the ]
	}
	tempString[j++] = 0;										//	Terminate the string

	if(StrCaselessCompare((const char *)tempString, "Sizes") != 0)
		ErrThrow(HLParseErrorMenuNoSizes);

	i+=2;															//	Step to start of Size Type


	//	Get the Sizes and create records for them
	while(i<messageLength && ALReceivePtr[i] != '\n')
	{
		sizeRecord.size = sizeCounter++;

		j=0;
		while(i<messageLength && ALReceivePtr[i] != '\n')
		{
			sizeRecord.sizeName[j++] = ALReceivePtr[i++];//	Copy the string until the \n
			if(j > MaxMenuItemSize)
				j = MaxMenuItemSize;								//	Don't accept too long strings
		}
		sizeRecord.sizeName[j] = 0;

		MmGenerateSizeMenuRecord(sizesDB, &sizeRecord, recSizeIndex++);
		i++;															//	Step over the \n
	}
	i++;																//	Step over the \n
		



	//	Next get the Serving Courses
	//	Find [ServingCourse] block in string
	while(i<messageLength -1 && ALReceivePtr[i] != '[')
		i++;
	i++;															//	Get past the [

	j=0;
	while(i<messageLength -1 && ALReceivePtr[i] != ']')
	{
		tempString[j++] = ALReceivePtr[i++];			//	Copy the string until the ]
	}
	tempString[j++] = 0;										//	Terminate the string

	if(StrCaselessCompare((const char *)tempString, "ServingCourses") != 0)
		ErrThrow(HLParseErrorMenuNoServingCourses);

	i+=2;															//	Step to start of ServingCourse Type

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	//	Get the ServingCourses and create records for them
/*
"[ServingCourses]\n"
"1|Entrees|255\n"
"22|Mains|65280\n"
"15|Desserts|12255232\n"
"5|Beverages|0\n"
"\n"

"[ServingCourses]\n"
"1|Entrees|255\n"
"22|Mains|47872\n"
"15|Desserts|16711680\n"
"\n"
"[SALAD & SIDES]1\n"

*/

	//	If there are no Serving Courses, crash out!
	if(ALReceivePtr[i] == '\n')	//	Check if ServingCourse item exists
	{
		ErrThrow(HLParseErrorMenuNoServingCourses);
	}


	//	If there are any Serving Courses, deal to them
	else	//	Check if ServingCourse item exists
	{
		servingCourseRecord.recordType = ServingType;
		if(menuType == DishesMenu)
			recServingCourseIndex = 1;							//	ServingCourse Indices start at 1
		else
			recServingCourseIndex = dmMaxRecordIndex ;	//	Store at end of DB

		while(i<messageLength && ALReceivePtr[i] != '\n')	//	Check if ServingCourse item exists
		{
			//	Get the uniqueID
			j = 0;
			while(i<messageLength && ALReceivePtr[i] != '|')
			{
				tempString[j++] = ALReceivePtr[i++];	//	Copy uniqueID to temp string
			}
			tempString[j] = 0;								//	Terminate the string
			servingCourseRecord.uniqueID = StrAToI(tempString);//	Preserve the uniqueID

			i++;													//	Step over the '|'

			//	Get the Name
			j = 0;
			while(i<messageLength && ALReceivePtr[i] != '|' && ALReceivePtr[i] != '\n')
			{
				//	Don't overrun record.name size
				if(j < MaxMessageFromServerLength)
					servingCourseRecord.standardName[j++] = ALReceivePtr[i];//	Copy the string until '|'
				i++;
			}
			servingCourseRecord.standardName[j] = 0;

			StrCopy(servingCourseRecord.selectedName, "* ");
			StrCat(servingCourseRecord.selectedName, servingCourseRecord.standardName);

			i++;													//	Step over the '|'




			//	Get the Selectable property
			if(ALReceivePtr[i++] == 'T')
			{
				servingCourseRecord.selectable = true;
			}
			else
			{
				servingCourseRecord.selectable = false;
			}
			




			//	Get the Color
			j = 0;
			while(i<messageLength && ALReceivePtr[i] != '\n')
			{
				tempString[j++] = ALReceivePtr[i++];	//	Copy Color to temp string
			}
			tempString[j] = 0;								//	Terminate the string
			servingCourseRecord.color = StrAToI(tempString);//	Preserve the uniqueID


			servingCourseRecord.menuType = menuType;

			if(!MmGetServingCourseIndex(MmServingCourseDB, servingCourseRecord.uniqueID,
													&recServingCourseIndexTemp, &colorTemp, &selectable)	)
			{
				MmGenerateServingCourseRecord(&MmServingCourseDB, &servingCourseRecord, recServingCourseIndex);

				if(menuType == DishesMenu)
					recServingCourseIndex++;
			}
	//		}

			i++;													//	Step over the '\n'
		}
	}
		



	//	Recycle courseCounter
	courseCounter = 1;

	recItemIndex = 0;
	recOptionIndex = 1;
	recForcedSidesIndex = 0;
//	record.uniqueID = 0;											//	Ensure first uniqueID is sensible
//	record.setMenuMask = 0;

/*
"[Beer]\n"
*/
	//	Outer loop cycles through Courses
	while(i<messageLength)
	{
		//	Find [Course] in string
		while(i<messageLength && ALReceivePtr[i] != '[')
			i++;
		i++;															//	Get past the [

		itemCounter++;
		itemInCourse = 0;
		currentCourseIndex = recItemIndex;

		//	Set up the record in the order of the struct
		record.menuID = menuID;
		record.courseNumber = courseCounter;
		record.itemNumber = 0;									//	Zero => Course name
		record.itemInCourse = 0;								//	Zero => Course name
		record.enabled = StatusAsEnabled;					//	All enabled by default
		record.popupItemSizes = false;
		record.onlyAsSide = false;
//		record.setMenuMask = 0;
		record.uniqueID = itemCounter;						//	UniqueID is one above last drink
		record.courseIndex = currentCourseIndex;			//	Allow easy access to size type
		record.forcedSidesIndex = recForcedSidesIndex;
		j=0;
		while(i<messageLength && ALReceivePtr[i] != ']')
		{
			record.itemName[j++] = ALReceivePtr[i++];		//	Copy the string until the ]
			if(j > MaxMenuItemSize)
				j = MaxMenuItemSize;								//	Don't accept too long strings
		}
		record.itemName[j] = 0;
		i++;															//	Step over the ']'


		StrCopy(record.disabledItemName, "* ");
		StrCat(record.disabledItemName, record.itemName);
//		record.price = 0;
		for(j=0; j < MaxSizeTypes; j++)						//	Only MaxSizeTypes standard sizes
		{
			record.sizes[j].size = (UInt16)NoValue16;//	Make unavailable
			record.sizes[j].price = 0;	//	Zero out the price
			record.sizes[j].setMenuMask = 0;
			record.sizes[j].mayBeStdItem = false;
		}



		//	Get the Default Serving Course for this Menu Course
		j=0;
		tempString[0] = 0;

		while(i<messageLength && ALReceivePtr[i] != '\n')
		{
			tempString[j++] = ALReceivePtr[i++];	//	Copy the string until the '\n'
		}
		tempString[j] = 0;

		if(StrLen(tempString))
		{
			record.servingCourseDefault = (UInt16)StrAToI(tempString);
			if(record.servingCourseDefault == 0)
				record.servingCourseDefault = NoValue16;
		}
		else
		{
			record.servingCourseDefault = NoValue16;
		}
		servingCourseDefault = record.servingCourseDefault;


		//	Write the Course record
		MmGenerateMenuRecord(menuDB, &record, recItemIndex);//	Generate the Course record
		
		recItemIndex++;
		i++;															//	Step to start of first Item
	
/*
//	DisplayItemSizes (T/F) ForcedSideOnly (T/F)|Name|Size,T or F or K SetMenuMask|
//	... where T or F = True/False available as standard item, K = Key
// Forced Sides: GroupNo (1-8), Max Select.
//	... where T or F = True/False 'Continue' button available
//	... where 0 = Select as many as you like then hit Continue (Must be T).
//	... 		 1 = Select 1 auto move on (or allow Continue to skip this group if T).
"FFGarlic Herbed Bread|1|3|4|,1|T1|1|3|10|,2|T0|4|9|12|,4|T0|1|3|4|5|22|\n"
"FFSoup Du Jour|1T7|2F2|\n"			//	7 & 2 are SetMenuMask bitmasks!
"FTSpicy Lamb Salad|1|\n"
"FFP & K Balls|1T2|\n"
"FFTrio of Fettucine|2T2|\n"
"FFDeep-Fried Camembert|1F2|\n"
*/

		//	Now for the Items
		while(i<messageLength && ALReceivePtr[i] != '{')	//	Brace is start of Options
		{
			//	First check the popupItemSizes type
			if(ALReceivePtr[i] == 'T')
				record.popupItemSizes = true;
			else
				record.popupItemSizes = false;

			i++;

			
			//	First check if Available only as Forced Side
			if(ALReceivePtr[i] == 'T')
				record.onlyAsSide = true;
			else
				record.onlyAsSide = false;

			
			itemCounter++;
			itemInCourse++;
			record.enabled = StatusAsEnabled;				//	All enabled by default
			record.itemNumber = itemCounter;
			record.itemInCourse = itemInCourse;
			record.courseIndex = currentCourseIndex;		//	Allow easy access to size type
			record.forcedSidesIndex = recForcedSidesIndex;
			record.servingCourseDefault = servingCourseDefault;

			record.uniqueID = itemCounter;					//	Convert and store uniqueID

			i++;


			//	Get the Item name
			j=0;
			while(i<messageLength && ALReceivePtr[i] != '|')
			{
				record.itemName[j++] = ALReceivePtr[i++];	//	Copy the string until the |
				if(j > MaxMenuItemSize)
					j = MaxMenuItemSize;							//	Don't accept too long strings
			}
			record.itemName[j] = 0;
			StrCopy(record.disabledItemName, "* ");
			StrCat(record.disabledItemName, record.itemName);
	
			i++;														//	Step over the |

			//	Copy the Size for each of the size options
			sizeCounter = 0;
			while(i<messageLength && ALReceivePtr[i] != ',' && ALReceivePtr[i] != '\n')
			{
				//	Copy the size
				j=0;
				while(i<messageLength && ALReceivePtr[i] < ':' )//	Copy the size number
				{
					tempString[j++] = ALReceivePtr[i++];		//	Copy the string until the ','
				}
				tempString[j] = 0;
				record.sizes[sizeCounter].size = (UInt16)StrAToI(tempString);
				record.sizes[sizeCounter].price = 0;	//	Force all prices to zero
				record.sizes[sizeCounter].mayBeStdItem = true;	//	By default
/*
//	DisplayItemSizes (T/F) ForcedSideOnly (T/F)|Name|Size,T or F or K SetMenuMask|
//	... where T or F = True/False available as standard item, K = Key
// Forced Sides: GroupNo (1-8), Max Select.
//	... where T or F = True/False 'Continue' button available
//	... where 0 = Select as many as you like then hit Continue (Must be T).
//	... 		 1 = Select 1 auto move on (or allow Continue to skip this group if T).
"FFGarlic Herbed Bread|1|3|4|,1|T1|1|3|10|,2|T0|4|9|12|,4|T0|1|3|4|5|22|\n"
*/
				//	Get any setMenuMask if end delimiter was not '|'
				if(ALReceivePtr[i] != '|')
				{
					if(ALReceivePtr[i] == 'K')
						isSetMenuKey = true;
					else
						isSetMenuKey = false;

					if(ALReceivePtr[i] == 'T' || ALReceivePtr[i] == 'K')
						record.sizes[sizeCounter].mayBeStdItem = true;
					else
						record.sizes[sizeCounter].mayBeStdItem = false;

					i++;													//	Step over the delimiter
	
					//	Copy the setMenuMask
					j=0;
					while(i<messageLength && ALReceivePtr[i] != '|')
					{
						tempString[j++] = ALReceivePtr[i++];	//	Copy the string until the |
					}
					tempString[j] = 0;
	
					//	Write 0 if no setMenuMask
					if(StrLen(tempString) != 0)
					{
						record.sizes[sizeCounter].setMenuMask = (UInt32)StrAToI(tempString);
//						record.setMenuMask |= (UInt32)StrAToI(tempString);
					}
					else
					{
						record.sizes[sizeCounter].setMenuMask = 0;
					}
					
					//	If this is a SetMenu Key item, set the MSB
					if(isSetMenuKey)
					{
						record.sizes[sizeCounter].setMenuMask |= SetMenuKey;
					}
				}
				else	//	Not a setMenuItem
				{
					record.sizes[sizeCounter].setMenuMask = 0;
				}

				sizeCounter++;
				i++;													//	Step over the '|'
			}

			while(sizeCounter < MaxSizeTypes)				//	Only MaxSizeTypes standard sizees
			{
				record.sizes[sizeCounter].size = (UInt16)NoValue16;	//	Make unavailable
				record.sizes[sizeCounter].price = 0;	//	Zero out the price
				record.sizes[sizeCounter].setMenuMask = 0;
				record.sizes[sizeCounter].mayBeStdItem = false;
				sizeCounter++;
			}


			MmGenerateMenuRecord(menuDB, &record, recItemIndex);//	Generate an Item record

			recItemIndex++;


			//	Process the Forced Sides if they exist
			if(ALReceivePtr[i] == '\n')
			{
				i++;	//	Step over the \n
			}
			else
			{
				i++;	//	Step over the ','

				forcedSide.masterItem = record.itemNumber;

/*
//	DisplayItemSizes (T/F) ForcedSideOnly (T/F)|Name|Size,T or F or K SetMenuMask|
//	... where T or F = True/False available as standard item, K = Key
// Forced Sides: GroupNo (1-8), Max Select.
//	... where T or F = True/False 'Continue' button available
//	... where 0 = Select as many as you like then hit Continue (Must be T).
//	... 		 1 = Select 1 auto move on (or allow Continue to skip this group if T).
"FFGarlic Herbed Bread|1|3|4|,1|T1|1|3|10|,2|T0|4|9|12|,4|T0|1|3|4|5|22|\n"
"FFSoup Du Jour|1T7|2F2|\n"			//	7 & 2 are SetMenuMask bitmasks!
*/
				while(i<messageLength && ALReceivePtr[i] != '\n')	
				{
					//	Get the Group Number
					j=0;
					while(ALReceivePtr[i] != '|')
					{
						tempString[j++] = ALReceivePtr[i++];	//	Copy the string until the '|'
					}
					tempString[j] = 0;
					forcedSide.groupNumber = (UInt16)StrAToI(tempString);
					i++;	//	Step over the '|'


					//	Get the Max Select Next Button status
					if(ALReceivePtr[i] == 'T')
						forcedSide.nextButton = true;
					else
						forcedSide.nextButton = false;

					i++;													//	Step over the identifier


					//	Get the Max Select Number
					j=0;
					while(ALReceivePtr[i] != '|')
					{
						tempString[j++] = ALReceivePtr[i++];	//	Copy the string until the '|'
					}
					tempString[j] = 0;
					forcedSide.maxSelect = (UInt16)StrAToI(tempString);
					i++;	//	Step over the '|'
					
					
					//	Ensure there is no possibility of multi-select having no Next
					if(forcedSide.maxSelect == 0)
					{
						forcedSide.nextButton = true;
					}


//"FFGarlic Herbed Bread|1|3|4|,1|T1|1|3|10|,2|T0|4|9|12|,4|T0|1|3|4|5|22|\n"
//"TFAngus T-Bone|1|,1|F1|1|3|10|,2|T1|4|9|12|,3|F2|1|3|4|5|22|\n"

					//	Get the itemNumber for each of the Forced Sides for this Group
					while(ALReceivePtr[i] != ',' && ALReceivePtr[i] != '\n')
					{
						j=0;
						while(ALReceivePtr[i] != '|')
						{
							tempString[j++] = ALReceivePtr[i++];//	Copy the string until the '|'
						}
						tempString[j] = 0;
						forcedSide.itemIndex = (UInt16)StrAToI(tempString) - 1;	//	Indexes start at 0!
						i++;	//	Step over the '|'

						MmGenerateForcedSideRecord(forcedSidesDB, &forcedSide, recForcedSidesIndex);

						recForcedSidesIndex++;
					}
					if(ALReceivePtr[i] == ',')
						i++;	//	Step over the ',' but not '\n'
				}	//	while(ALReceivePtr[i] != '\n')
				i++;	//	Step over the '\n'
			}	//	if(ALReceivePtr[i] == '\n') .. else
		}	//	Loop through all menu items
		i++;															//	Step over the '{'
		
/*
"{2100000001Fresh Garden Salad|3100000002Wedges|1100000003Gourmet Potatoes|3100000004Fresh Steamed Vegetables|}\n"
"{Beer Mat|Straw|Ice|Umbrella|}\n"
"\n"
"[Wine]\n"
*/
		optionCounter=1;												//	Count options for this Course
		while(i<messageLength && ALReceivePtr[i] != '}')	//	Do the Options
		{
			option.courseNumber = courseCounter;
			option.courseOptionNumber = optionCounter++;

			j=0;

			//	First UInt8 is the flags in hex (2 chars)
			//	See OptionsType for definition of flag bits
			while(i<messageLength && j < 2)
			{
				tempString[j++] = ALReceivePtr[i++];			//	Get 2 chars
			}
			tempString[j] = 0;

			option.optionFlags = (UInt8)MmAToH(tempString);


			j=0;

			//	Next 4 bytes are the Item bitmask in hex (8 chars)
			while(i<messageLength && j < 8)
			{
				tempString[j++] = ALReceivePtr[i++];			//	Get 8 chars
			}
			tempString[j] = 0;

			option.optionItemMask = MmAToH(tempString);




			j=0;

			while(i<messageLength && ALReceivePtr[i] != '|')
			{
				option.optionName[j++] = ALReceivePtr[i++];	//	Copy the string until the |
				if(j > MaxOptionItemSize)
					j = MaxOptionItemSize;							//	Don't accept too long strings
			}
			option.optionName[j] = 0;
			MmGenerateOptionsRecord(optionsDB, &option, recOptionIndex++);
			i++;															//	Step over the |
		}
//MmPrvSyncLogTrace("\nGot Here!");
		courseCounter++;
		i+=3;																//	Step to start of next item
	}
//MmPrvSyncLogTraceUI("\nAlReceivePtr index = %d", i);
/*
	//	Extract the Prefs.transactionNo and the Prefs.drinkSyncNo from the message
	Prefs.transactionNo = ALReceivePtr[HLHdrTransNo];
	Prefs.drinkSyncNo = ALReceivePtr[HLHdrDrinkSync];
	Prefs.transactionNumber	=	Prefs.transactionNo;
	Prefs.drinkSyncNumber	=	Prefs.drinkSyncNo;

	MmSavePrefs();													//	Save these now in case we break
*/
#ifdef DebugHL3
MmPrvSyncLogTraceUI("\nDrinkSyncNumber = %d", Prefs.drinkSyncNo);
MmPrvSyncLogTraceUI("\nTransactionNumber = %d", Prefs.transactionNo);
#endif

}


/***********************************************************************
 *
 * FUNCTION:		MmParseGetAllOrdersMessage
 *
 * DESCRIPTION:	Parse the newly received text file for Menu data etc.
 *
 * PARAMETERS:		.
 *
 * RETURNED:		Boolean. Completed = false, User cancelled = true.
 *
 ***********************************************************************/
static Boolean MmParseGetAllOrdersMessage(UInt16 messageLength)
{
	MemHandle				recMenuHandle;
	MemHandle				recMarkerHandle;
	OrderTypePtr			recMarkerPtr;
	MenuTypePtr				recMenuPtr;
	MenuTypePtr				recDrinkMenuPtr;

	UInt16					recIndex;
	UInt16					recMarkerIndex;
	UInt16					recSizeIndex;
	UInt16					recSeatnameIndex;
	SeatnameType			seatnameRecord;
	
	UInt8						flags;				//	First Flag in message stream
	UInt8						flags2;				//	Second (optional) Flag in message stream
	UInt8						sectionFlag;
	UInt8						enabled;				//	For dish/drink Enabled

	Err						error;
						
	UInt16					i, j;
	
	UInt16					currentTable;
	UInt16					currentSeat;
	
	UInt16					currentTableNumber;	//	Preserve Prefs.current.tableNo

	UInt16					currentDishIndex;
//	UInt8						currentMenu;
	UInt16					uniqueID;
	UInt16					setMenuGroup;
	UInt16					setMenuGroupTemp;

	UInt16					previousTable		= 0;
	UInt16					previousSeat		= 0;
	UInt16					previousDish		= 0;
	UInt16					previousOrderType	= 0;

	UInt16					itemCounter = 0;
	
	Boolean					doTable				= true;
	Boolean					doSeat				= true;

	Boolean					lastTable			= false;
	Boolean					lastSeat				= false;
	Boolean					lastDish				= false;

	Boolean					lastLinkedItem		= false;

	Boolean					isDrinkItem;
//	Int32						splitPrice;
	Int32						priceCorrection;	//	Server sends correction for items not on Palm menu
	UInt32					priceTemp;

	DmOpenRef				menuDB;
	UInt16					numDishesDB;
	UInt16					numDrinksDB;

	UInt32					startTick, endTick;
	ProgressPtr				prgPtr;
	UInt16					count;
	EventType				event;
	Boolean					cancelPending;
	Boolean					doDisplayProgress;


#ifdef CountOrdersSent
	UInt16 					debugOrdersRxd = 0;
#endif



#ifdef DebugFakeMenu
	UInt8	fakeOrders[] =	
			{	HLGetAllOrders, '~', '~', '~', '~', '~', '~', '~', '~',

	//	sectionFlag
//1 UInt8	Sections Flag					//	Bit 7 = Has Disable Dishes
												//	Bit 6 = ReEnable all Dishes
												//	Bit 5 = ReEnable all Drinks
												//	Bit 4 = Seat Names
                                    // Bit 3 = GetOrders/SendOrders
                                    // Bit 2 = Message
                                    // Bit 1 = Billing Data
                                    // Bit 0 = NA

//	0x80	//	No Seatnames or orders, no disables

//	0x88,	//	No Seatnames. Orders, no disables

//	0x98,	//	Seatnames and orders, no disables

//	0x90,	//	Seatnames, No Orders


	//	Order
	0x08,	//	Orders

/*
	//	Seatnames, Order
	0x98,	//	Seatname, Order

	//	Seatnames
		0x01,										//	Table
			0x81,									//	Seat 1
				0x46, 0x72, 0x65, 0xE4,		//	Fred

		0x82,										//	Table
			0x01,									//	Seat 1
				0x42, 0x69, 0x6C, 0xEC,		//	Bill
			0x82,									//	Seat 2
				0x4D, 0x69, 0x6B, 0xE5,		//	Mike

	//	Single Dish Order
		0x81,
			0x81,
				0x02, 0x80,	0x80, 0x03, 0x01
*/
	//	Price Correction with single order
	0x81, 0x81,	//	Table/Seat
					0x00, 0xA0, 0x00, 0x98, 0x96, 0x80

/*
	//	Disable, Seatnames, Order
	0xD8,	//	Enable All, Disables, Seatname, Order
		0x80, 0x04,

	//	Seatnames
		0x82,										//	Table
			0x01,									//	Seat 1
				0x42, 0x69, 0x6C, 0xEC,		//	Bill
			0x82,									//	Seat 2
				0x4D, 0x69, 0x6B, 0xE5,		//	Mike

	//	Single Dish Order
		0x81,
			0x81,
				0x02, 0x80,	0x80, 0x03, 01
*/


	//	Real Message (cancel Seatnames) causes CRASH!
//	0x90, 0x81, 0x01, 0x80, 0x02, 0x80, 0x03, 0x80, 0x84, 0x80

/*
//	Transmission Message First Flag
#define FlagsCreditDish					0x01		//	Used for Transmission Message Flags
#define FlagsHasDishOrDrink			0x02
#define FlagsHasSeatname				0x04
#define FlagsHasItemLinks				0x08		//	Mixed drink (split), or linked dish (Side)
#define FlagsHasNote 					0x10
#define FlagsHasPriceAdj				0x20
#define FlagsHasOption 					0x40
#define FlagsHasSetMenuMask			0x80

//	Transmission Message Second Flag
#define FlagsNA1							0x01		//	Used for Transmission Message Flags
#define FlagsNA2							0x02
#define FlagsNA3							0x04
#define FlagsNA4 							0x08
#define FlagsNA5 							0x10
#define FlagsHasPriceCorrection		0x20		//	Dollar value from POS for non Palm items
#define FlagsHasSplitBill				0x40
*/




/*	//	Multiple SetMenu Dish/drink orders
	0x88,													//	Section Flag
	0x02, 0x81,											//	Table, Seat
					0x02, 0x80, 0x00, 0x03, 0x01,
	//	Price Correction without orders
	0xB7, 0x81,
					0x00, 0xA0, 0x00, 0x00, 0x1B, 0x6C
*/
//88 02 81 02 80 00 02 01 01 02 80 80 0C 01 02 84 81 00 A0 00 00 03 E3


/*
0x88, 0x03, 0x81, 0x82, 0x80, 0x00, 0x60, 0x82, 0x80, 0x00, 0x62, 0x82, 0x80, 0x00, 0x63, 0x82, 0x80, 0x80, 0xAF, 0x04, 0x01, 0x02, 0x80,
0x00, 0x05, 0x0A, 0x80, 0x00, 0x3A, 0x80, 0x30, 0x02, 0x80, 0xC0, 0x01, 0x02, 0x02, 0x02, 0x80, 0x00, 0x3E, 0x02, 0x80, 0xC0, 0x20, 0x03, 0x83, 0x82, 0x80, 0x00, 0x1A, 0x82, 0x80, 0x00, 0x5F,
0x82, 0x80, 0x00, 0x84, 0x82, 0x80, 0x00, 0xAF, 0x02, 0x80, 0xC0, 0x48, 0x05, 0x09, 0x01, 0x02, 0x80, 0x00, 0x2A, 0x0A, 0x80, 0x80, 0x3A, 0x80, 0x7C, 0x02, 0x02, 0x80, 0x00, 0x05, 0x82, 0x80,
0x00, 0x60, 0x82, 0x80, 0x00, 0x84, 0x82, 0x80, 0x00, 0xAF, 0x02, 0x80, 0xC0, 0x04, 0x01, 0x03, 0x02, 0x80, 0x00, 0x05, 0x02, 0x80, 0x80, 0x15, 0x84, 0x02, 0x80, 0x00, 0x15, 0x7A, 0x80, 0x80,
0x3A, 0x00, 0x30, 0x80, 0x2A, 0x0D, 0x8E, 0x00, 0x1E, 0x4E, 0x6F, 0x20, 0x68, 0x6F, 0x6F, 0xE6, 0x14, 0x01, 0x5A, 0x80, 0x80, 0x04, 0x80, 0x27, 0x0A, 0x8F, 0x42, 0x6C, 0x61, 0x20, 0x62, 0x6C,
0xE1, 0x02, 0x02, 0x80, 0x00, 0x02, 0x02, 0x80, 0x00, 0x0B, 0x02, 0x80, 0x00, 0x10, 0x82, 0x80, 0x00, 0x1A, 0x02, 0x80, 0x00, 0x1E, 0x02, 0x80, 0x00, 0x24, 0x02, 0x80, 0x00, 0x32, 0x82, 0x80,
0x00, 0x5F, 0x82, 0x80, 0x00, 0x66, 0x02, 0x80, 0x00, 0x71, 0x02, 0x80, 0x00, 0x87, 0x02, 0x80, 0x00, 0x96, 0x82, 0x80, 0x00, 0xAE, 0x02, 0x80, 0x40, 0x01, 0x01, 0x02, 0x80, 0x40, 0x14, 0x03,
0x02, 0x80, 0x40, 0x27, 0x03, 0x02, 0x80, 0x40, 0x3D, 0x04, 0x02, 0x80, 0x40, 0x4E, 0x01, 0x02, 0x80, 0x40, 0x67, 0x01, 0x02, 0x80, 0x40, 0x7F, 0x01, 0x02, 0x80, 0x40, 0x94, 0x01, 0x02, 0x80,
0x40, 0x9E, 0x01, 0x02, 0x80, 0x40, 0xAD, 0x04, 0x02, 0x80, 0x40, 0xC0, 0x04, 0x0A, 0x80, 0x40, 0xD1, 0x04, 0x40, 0xD1, 0x04, 0xC0, 0x01, 0x01, 0x06, 0x02, 0x80, 0xC0, 0xE6, 0x07, 0x83, 0x7A,
0x80, 0x80, 0x3A, 0x00, 0x27, 0x80, 0x30, 0x86, 0xFF, 0x38, 0x4E, 0x6F, 0x20, 0x6D, 0x75, 0x73, 0x68, 0x72, 0x6F, 0x6F, 0x6D, 0xF3, 0xAC, 0x81, 0x02, 0x80, 0x00, 0x29, 0x02, 0x80, 0xC0, 0x02,
0x02
*/




	//	Disables
//	0xE0,	//	Enable All, Disables
//			0x00, 0x04, 0x00, 0x07, 0x40, 0x04, 0xC0, 0x06

//	0xE8,	//	Enable All, Disables, Orders
//			0x00, 0x04, 0x40, 0x04, 0xC0, 0x06,
//			0x81, 0x81, 0x00, 0xA0, 0x00, 0x00, 0x01, 0x5E


	//	Single Dish Order
//	0x81, 0x81,
//	0x82, 0x80,	0x40, 0x33,	0x02,	0x82, 0x80, 0xC0, 0x35, 0x0C


	//	Real Order causes CRASH!
//	0xE9, 0x1E, 0x07, 0x03, 0x03, 0x01, 0xFF, 0x00, 0x3E,
	//	Enable all, has Disables


/*
	0xE8,
		0x00, 0x64, 0x00, 0x6E, 0x00, 0x6F, 0x80, 0xA1,
	//Dishes
	0x01, 0x81,
		0x0A, 0x80, 0x00, 0x21, 0x80, 0x27, 0x82, 0x80, 0x00, 0x61,
		0x82, 0x80, 0x00, 0x62, 0x82, 0x80, 0x00, 0xAE,
		0x0A, 0x80, 0xC0, 0x6E, 0x08, 0x40, 0x6E, 0x08, 0x40, 0x6E, 0x08, 0xC0, 0x6E, 0x08, 0x08,
	
	0x83, 0x81,
		0x02, 0x80, 0x00, 0x02, 0x02, 0x80, 0x00, 0x09,
		0x82, 0x80, 0x00, 0xAE, 0x02, 0x80, 0x80, 0xB3
*/


	//	Order with Side
//	0x81, 0x81,
//	0x0A, 0x80,	0x80, 0x02,	0x80,	0x03//, 0xE1, 0x03


/*
	//	Multiple Dish/drink orders
	0x88,
	0x81, 0x01, 0x02, 0x80,	0x00, 0x02,
					0x02, 0x80, 0x40, 0x02, 0x02,
					0x02, 0x80, 0xC0, 0x02, 0x02,
			0x02, 0x02, 0x80, 0x40, 0x02, 0x02,
					0x02, 0x80, 0xC0, 0x02, 0x02,
			0x83, 0x02, 0x80, 0x40, 0x02, 0x02,
					0x02, 0x80, 0xC0, 0x02, 0x02

*/
/*
	//	Multiple Dish/drink orders, 1 each per table
	0x01, 0x01, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0x00, 0x03,
					0x82, 0x80, 0x00, 0x04,
					0x82, 0x80, 0x00, 0x05,
					0x82, 0x80, 0x00, 0x06,
					0x82, 0x80, 0x00, 0x07,
					0x82, 0x80, 0x00, 0x08,
					0x82, 0x80, 0x00, 0x09,
					0x82, 0x80, 0x40, 0x02, 0x02,
					0x82, 0x80, 0x40, 0x03, 0x03,
					0x82, 0x80, 0x40, 0x04, 0x04,
					0x82, 0x80, 0x40, 0x05, 0x06,
					0x82, 0x80, 0x40, 0x06, 0x02,
					0x82, 0x80, 0x40, 0x08, 0x02,
					0x82, 0x80, 0x40, 0x09, 0x03,
					0x82, 0x80, 0x40, 0x0A, 0x03,
					0x82, 0x80, 0xC0, 0x0C, 0x03,
			0x02, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0x00, 0x03,
					0x82, 0x80, 0x00, 0x04,
					0x82, 0x80, 0x00, 0x05,
					0x82, 0x80, 0x00, 0x06,
					0x82, 0x80, 0x00, 0x07,
					0x82, 0x80, 0x00, 0x08,
					0x82, 0x80, 0x00, 0x09,
					0x82, 0x80, 0x40, 0x02, 0x02,
					0x82, 0x80, 0x40, 0x03, 0x03,
					0x82, 0x80, 0x40, 0x04, 0x04,
					0x82, 0x80, 0x40, 0x05, 0x06,
					0x82, 0x80, 0x40, 0x06, 0x02,
					0x82, 0x80, 0x40, 0x08, 0x02,
					0x82, 0x80, 0x40, 0x09, 0x03,
					0x82, 0x80, 0x40, 0x0A, 0x03,
					0x82, 0x80, 0xC0, 0x0C, 0x03,
			0x83, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0x00, 0x03,
					0x82, 0x80, 0x00, 0x04,
					0x82, 0x80, 0x00, 0x05,
					0x82, 0x80, 0x00, 0x06,
					0x82, 0x80, 0x00, 0x07,
					0x82, 0x80, 0x00, 0x08,
					0x82, 0x80, 0x00, 0x09,
					0x82, 0x80, 0x40, 0x02, 0x02,
					0x82, 0x80, 0x40, 0x03, 0x03,
					0x82, 0x80, 0x40, 0x04, 0x04,
					0x82, 0x80, 0x40, 0x05, 0x06,
					0x82, 0x80, 0x40, 0x06, 0x02,
					0x82, 0x80, 0x40, 0x08, 0x02,
					0x82, 0x80, 0x40, 0x09, 0x03,
					0x82, 0x80, 0x40, 0x0A, 0x03,
					0x82, 0x80, 0xC0, 0x0C, 0x03,
	0x02, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0x00, 0x03,
					0x82, 0x80, 0x00, 0x04,
					0x82, 0x80, 0x00, 0x05,
					0x82, 0x80, 0x00, 0x06,
					0x82, 0x80, 0x00, 0x07,
					0x82, 0x80, 0x00, 0x08,
					0x82, 0x80, 0x00, 0x09,
					0x82, 0x80, 0x40, 0x02, 0x02,
					0x82, 0x80, 0x40, 0x03, 0x03,
					0x82, 0x80, 0x40, 0x04, 0x04,
					0x82, 0x80, 0x40, 0x05, 0x06,
					0x82, 0x80, 0x40, 0x06, 0x02,
					0x82, 0x80, 0x40, 0x08, 0x02,
					0x82, 0x80, 0x40, 0x09, 0x03,
					0x82, 0x80, 0x40, 0x0A, 0x03,
					0x82, 0x80, 0xC0, 0x0C, 0x03,
	0x03, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0x00, 0x03,
					0x82, 0x80, 0x00, 0x04,
					0x82, 0x80, 0x00, 0x05,
					0x82, 0x80, 0x00, 0x06,
					0x82, 0x80, 0x00, 0x07,
					0x82, 0x80, 0x00, 0x08,
					0x82, 0x80, 0x00, 0x09,
					0x82, 0x80, 0x40, 0x02, 0x02,
					0x82, 0x80, 0x40, 0x03, 0x03,
					0x82, 0x80, 0x40, 0x04, 0x04,
					0x82, 0x80, 0x40, 0x05, 0x06,
					0x82, 0x80, 0x40, 0x06, 0x02,
					0x82, 0x80, 0x40, 0x08, 0x02,
					0x82, 0x80, 0x40, 0x09, 0x03,
					0x82, 0x80, 0x40, 0x0A, 0x03,
					0x82, 0x80, 0xC0, 0x0C, 0x03,
	0x04, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0x00, 0x03,
					0x82, 0x80, 0x00, 0x04,
					0x82, 0x80, 0x00, 0x05,
					0x82, 0x80, 0x00, 0x06,
					0x82, 0x80, 0x00, 0x07,
					0x82, 0x80, 0x00, 0x08,
					0x82, 0x80, 0x00, 0x09,
					0x82, 0x80, 0x40, 0x02, 0x02,
					0x82, 0x80, 0x40, 0x03, 0x03,
					0x82, 0x80, 0x40, 0x04, 0x04,
					0x82, 0x80, 0x40, 0x05, 0x06,
					0x82, 0x80, 0x40, 0x06, 0x02,
					0x82, 0x80, 0x40, 0x08, 0x02,
					0x82, 0x80, 0x40, 0x09, 0x03,
					0x82, 0x80, 0x40, 0x0A, 0x03,
					0x82, 0x80, 0xC0, 0x0C, 0x03,
	0x05, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0x00, 0x03,
					0x82, 0x80, 0x00, 0x04,
					0x82, 0x80, 0x00, 0x05,
					0x82, 0x80, 0x00, 0x06,
					0x82, 0x80, 0x00, 0x07,
					0x82, 0x80, 0x00, 0x08,
					0x82, 0x80, 0x00, 0x09,
					0x82, 0x80, 0x40, 0x02, 0x02,
					0x82, 0x80, 0x40, 0x03, 0x03,
					0x82, 0x80, 0x40, 0x04, 0x04,
					0x82, 0x80, 0x40, 0x05, 0x06,
					0x82, 0x80, 0x40, 0x06, 0x02,
					0x82, 0x80, 0x40, 0x08, 0x02,
					0x82, 0x80, 0x40, 0x09, 0x03,
					0x82, 0x80, 0x40, 0x0A, 0x03,
					0x82, 0x80, 0xC0, 0x0C, 0x03,
	0x06, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0x00, 0x03,
					0x82, 0x80, 0x00, 0x04,
					0x82, 0x80, 0x00, 0x05,
					0x82, 0x80, 0x00, 0x06,
					0x82, 0x80, 0x00, 0x07,
					0x82, 0x80, 0x00, 0x08,
					0x82, 0x80, 0x00, 0x09,
					0x82, 0x80, 0x40, 0x02, 0x02,
					0x82, 0x80, 0x40, 0x03, 0x03,
					0x82, 0x80, 0x40, 0x04, 0x04,
					0x82, 0x80, 0x40, 0x05, 0x06,
					0x82, 0x80, 0x40, 0x06, 0x02,
					0x82, 0x80, 0x40, 0x08, 0x02,
					0x82, 0x80, 0x40, 0x09, 0x03,
					0x82, 0x80, 0x40, 0x0A, 0x03,
					0x82, 0x80, 0xC0, 0x0C, 0x03,
	0x07, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0x00, 0x03,
					0x82, 0x80, 0x00, 0x04,
					0x82, 0x80, 0x00, 0x05,
					0x82, 0x80, 0x00, 0x06,
					0x82, 0x80, 0x00, 0x07,
					0x82, 0x80, 0x00, 0x08,
					0x82, 0x80, 0x00, 0x09,
					0x82, 0x80, 0x40, 0x02, 0x02,
					0x82, 0x80, 0x40, 0x03, 0x03,
					0x82, 0x80, 0x40, 0x04, 0x04,
					0x82, 0x80, 0x40, 0x05, 0x06,
					0x82, 0x80, 0x40, 0x06, 0x02,
					0x82, 0x80, 0x40, 0x08, 0x02,
					0x82, 0x80, 0x40, 0x09, 0x03,
					0x82, 0x80, 0x40, 0x0A, 0x03,
					0x82, 0x80, 0xC0, 0x0C, 0x03,
	0x08, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0x00, 0x03,
					0x82, 0x80, 0x00, 0x04,
					0x82, 0x80, 0x00, 0x05,
					0x82, 0x80, 0x00, 0x06,
					0x82, 0x80, 0x00, 0x07,
					0x82, 0x80, 0x00, 0x08,
					0x82, 0x80, 0x00, 0x09,
					0x82, 0x80, 0x40, 0x02, 0x02,
					0x82, 0x80, 0x40, 0x03, 0x03,
					0x82, 0x80, 0x40, 0x04, 0x04,
					0x82, 0x80, 0x40, 0x05, 0x06,
					0x82, 0x80, 0x40, 0x06, 0x02,
					0x82, 0x80, 0x40, 0x08, 0x02,
					0x82, 0x80, 0x40, 0x09, 0x03,
					0x82, 0x80, 0x40, 0x0A, 0x03,
					0x82, 0x80, 0xC0, 0x0C, 0x03,
	0x09, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0x00, 0x03,
					0x82, 0x80, 0x00, 0x04,
					0x82, 0x80, 0x00, 0x05,
					0x82, 0x80, 0x00, 0x06,
					0x82, 0x80, 0x00, 0x07,
					0x82, 0x80, 0x00, 0x08,
					0x82, 0x80, 0x00, 0x09,
					0x82, 0x80, 0x40, 0x02, 0x02,
					0x82, 0x80, 0x40, 0x03, 0x03,
					0x82, 0x80, 0x40, 0x04, 0x04,
					0x82, 0x80, 0x40, 0x05, 0x06,
					0x82, 0x80, 0x40, 0x06, 0x02,
					0x82, 0x80, 0x40, 0x08, 0x02,
					0x82, 0x80, 0x40, 0x09, 0x03,
					0x82, 0x80, 0x40, 0x0A, 0x03,
					0x82, 0x80, 0xC0, 0x0C, 0x03,
	0x0A, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0x00, 0x03,
					0x82, 0x80, 0x00, 0x04,
					0x82, 0x80, 0x00, 0x05,
					0x82, 0x80, 0x00, 0x06,
					0x82, 0x80, 0x00, 0x07,
					0x82, 0x80, 0x00, 0x08,
					0x82, 0x80, 0x00, 0x09,
					0x82, 0x80, 0x40, 0x02, 0x02,
					0x82, 0x80, 0x40, 0x03, 0x03,
					0x82, 0x80, 0x40, 0x04, 0x04,
					0x82, 0x80, 0x40, 0x05, 0x06,
					0x82, 0x80, 0x40, 0x06, 0x02,
					0x82, 0x80, 0x40, 0x08, 0x02,
					0x82, 0x80, 0x40, 0x09, 0x03,
					0x82, 0x80, 0x40, 0x0A, 0x03,
					0x82, 0x80, 0xC0, 0x0C, 0x03,
	0x0B, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0x00, 0x03,
					0x82, 0x80, 0x00, 0x04,
					0x82, 0x80, 0x00, 0x05,
					0x82, 0x80, 0x00, 0x06,
					0x82, 0x80, 0x00, 0x07,
					0x82, 0x80, 0x00, 0x08,
					0x82, 0x80, 0x00, 0x09,
					0x82, 0x80, 0x40, 0x02, 0x02,
					0x82, 0x80, 0x40, 0x03, 0x03,
					0x82, 0x80, 0x40, 0x04, 0x04,
					0x82, 0x80, 0x40, 0x05, 0x06,
					0x82, 0x80, 0x40, 0x06, 0x02,
					0x82, 0x80, 0x40, 0x08, 0x02,
					0x82, 0x80, 0x40, 0x09, 0x03,
					0x82, 0x80, 0x40, 0x0A, 0x03,
					0x82, 0x80, 0xC0, 0x0C, 0x03,
	0x0C, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0x00, 0x03,
					0x82, 0x80, 0x00, 0x04,
					0x82, 0x80, 0x00, 0x05,
					0x82, 0x80, 0x00, 0x06,
					0x82, 0x80, 0x00, 0x07,
					0x82, 0x80, 0x00, 0x08,
					0x82, 0x80, 0x00, 0x09,
					0x82, 0x80, 0x40, 0x02, 0x02,
					0x82, 0x80, 0x40, 0x03, 0x03,
					0x82, 0x80, 0x40, 0x04, 0x04,
					0x82, 0x80, 0x40, 0x05, 0x06,
					0x82, 0x80, 0x40, 0x06, 0x02,
					0x82, 0x80, 0x40, 0x08, 0x02,
					0x82, 0x80, 0x40, 0x09, 0x03,
					0x82, 0x80, 0x40, 0x0A, 0x03,
					0x82, 0x80, 0xC0, 0x0C, 0x03,
	0x0D, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0x00, 0x03,
					0x82, 0x80, 0x00, 0x04,
					0x82, 0x80, 0x00, 0x05,
					0x82, 0x80, 0x00, 0x06,
					0x82, 0x80, 0x00, 0x07,
					0x82, 0x80, 0x00, 0x08,
					0x82, 0x80, 0x00, 0x09,
					0x82, 0x80, 0x40, 0x02, 0x02,
					0x82, 0x80, 0x40, 0x03, 0x03,
					0x82, 0x80, 0x40, 0x04, 0x04,
					0x82, 0x80, 0x40, 0x05, 0x06,
					0x82, 0x80, 0x40, 0x06, 0x02,
					0x82, 0x80, 0x40, 0x08, 0x02,
					0x82, 0x80, 0x40, 0x09, 0x03,
					0x82, 0x80, 0x40, 0x0A, 0x03,
					0x82, 0x80, 0xC0, 0x0C, 0x03,
	0x0E, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0x00, 0x03,
					0x82, 0x80, 0x00, 0x04,
					0x82, 0x80, 0x00, 0x05,
					0x82, 0x80, 0x00, 0x06,
					0x82, 0x80, 0x00, 0x07,
					0x82, 0x80, 0x00, 0x08,
					0x82, 0x80, 0x00, 0x09,
					0x82, 0x80, 0x40, 0x02, 0x02,
					0x82, 0x80, 0x40, 0x03, 0x03,
					0x82, 0x80, 0x40, 0x04, 0x04,
					0x82, 0x80, 0x40, 0x05, 0x06,
					0x82, 0x80, 0x40, 0x06, 0x02,
					0x82, 0x80, 0x40, 0x08, 0x02,
					0x82, 0x80, 0x40, 0x09, 0x03,
					0x82, 0x80, 0x40, 0x0A, 0x03,
					0x82, 0x80, 0xC0, 0x0C, 0x03,
	0x0F, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0x00, 0x03,
					0x82, 0x80, 0x00, 0x04,
					0x82, 0x80, 0x00, 0x05,
					0x82, 0x80, 0x00, 0x06,
					0x82, 0x80, 0x00, 0x07,
					0x82, 0x80, 0x00, 0x08,
					0x82, 0x80, 0x00, 0x09,
					0x82, 0x80, 0x40, 0x02, 0x02,
					0x82, 0x80, 0x40, 0x03, 0x03,
					0x82, 0x80, 0x40, 0x04, 0x04,
					0x82, 0x80, 0x40, 0x05, 0x06,
					0x82, 0x80, 0x40, 0x06, 0x02,
					0x82, 0x80, 0x40, 0x08, 0x02,
					0x82, 0x80, 0x40, 0x09, 0x03,
					0x82, 0x80, 0x40, 0x0A, 0x03,
					0x82, 0x80, 0xC0, 0x0C, 0x03,
	0x10, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x11, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x12, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x13, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x14, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x15, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x16, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x17, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x18, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x19, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x1A, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x1B, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x1C, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x1D, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x1E, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x1F, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x20, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x21, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x22, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x23, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x24, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x25, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x26, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x27, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x28, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x29, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x2A, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x2B, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x2C, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x2D, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x2E, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x2F, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x30, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x31, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x32, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x33, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x34, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x35, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x36, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x37, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x38, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x39, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x3A, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x3B, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x3C, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x3D, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x3E, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0x3F, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02,
	0xC0, 0x81, 0x82, 0x80,	0x00, 0x02,
					0x82, 0x80, 0xC0, 0x02, 0x02
*/
	//	Single DrinkMix Order
//	0x88,	//	Section Flag
//	0x81, 0x81, 0x0A, 0x80, 0xC0,	0x69, 0x08, 0xC0, 0x00, 0x08,	0x08

	//	Single Dish Order with Price correction
//	0x01, 0x01,		//	Table 1, Seat 1
//	0x82, 0xA0,	0x00, 0x00, 0x03, 0xE8, 0x00, 0x02, 0x82, 0x80, 0x80, 0x0A,
//			0x83,
//	0x82, 0xA0,	0x00, 0x00, 0x03, 0xEA, 0x00, 0x03, 0x82, 0x80, 0x80, 0x0B,
//	0x82, 0x81,		//	Table 2, Seat 1
//	0x80, 0xA0,	0x00, 0x00, 0x01, 0xF4//, 0x00, 0x02, 0x82, 0x80, 0x80, 0x0A,

//	0x80, 0xA0,	0x00, 0x00, 0x03, 0xE8

	//	Seatnames
//	0x82, 0x01, 0x42, 0x69, 0x6C, 0xEC,		//	Seat 1 Bill
//			0x02, 0x4D, 0x69, 0x6B, 0xE5,		//	Seat 2 Mike
//			0x03, 0x4A, 0x69, 0x6C, 0xEC,		//	Seat 3 Jill
//			0x04, 0x4A, 0x6F, 0x68, 0xEE,		//	Seat 4 John
//			0x85, 0x46, 0x72, 0x65, 0xE4,		//	Seat 5 Fred

	//	Seatname Deletes
//	0x01, 0x01, 0x80,
//			0x82, 0x80,
//	0x82,
//			0x02, 0x80,
//			0x83, 0x80

	//	Single Drink Order
//	0x81, 0x81, 0x02,	0x80, 0xC0, 0x0A, 0x02
	
	//	Multiple Dish and Drink Orders with Seatnames
/*
	0x01, 0x01, 0x82, 0x00, 0x00, 0x02,
//					0x82, 0x00, 0x00, 0x02,
//					0x82, 0x00, 0x00, 0x02,
//					0x82, 0x00, 0x00, 0x02,
//					0x82, 0x00, 0x00, 0x02,
//					0x82, 0x00, 0x00, 0x02,
//					0x82, 0x00, 0x00, 0x09,
					0x82, 0x00, 0x00, 0x09,
					0x82, 0x00, 0x80, 0x09,
			0x03, 0x82, 0x00, 0x40, 0x02, 0x02, 	0x82, 0x00, 0x80, 0x09,
			0x04, 0x02, 0xC0, 0x80, 0x03,	0x01,	0x00, 0x00, 0x01, 0xF4,	//	DrinkMix item
			0x85, 0x02, 0xC0, 0x80, 0x03,	0x01, 0x00, 0x00, 0x01, 0xF4	//	DrinkMix item
*/
//	0x82,	0x01, 0x82, 0x00, 0x00, 0x01, 			0x82, 0x00, 0x80, 0x08,
//			0x02, 0x82, 0x00, 0x00, 0x02, 			0x82, 0x00, 0x80, 0x09,
//			0x03, 0x82, 0x00, 0x00, 0x03, 			0x82, 0x00, 0x80, 0x0A,
//			0x04, 0x82, 0x00, 0x00, 0x04, 			0x82, 0x00, 0x80, 0x0B,
//			0x85, 0x82, 0x00, 0x00, 0x05, 			0x82, 0x00, 0x80, 0x0C

//	0x05,	0x02, 0x82, 0x00, 0x00, 0x01,				0x82, 0x00, 0x80, 0x08,
//			0x03, 0x82, 0x00, 0x40, 0x02, 0x02, 	0x82, 0x00, 0x80, 0x09,
//			0x04, 0x82, 0x00, 0x00, 0x03, 			0x82, 0x00, 0xC0, 0x0A,	0x01,
//			0x06, 0x82, 0x00, 0x00, 0x04, 			0x82, 0x00, 0x80, 0x0B,
//			0xA2, 0x82, 0x00, 0x00, 0x05, 			0x82, 0x00, 0x80, 0x0C,

//	0xA5,	0x01, 0x82, 0x00, 0x00, 0x01, 			0x82, 0x00, 0x80, 0x08,
//			0x02, 0x82, 0x00, 0x00, 0x02, 			0x82, 0x00, 0x80, 0x09,
//			0x03, 0x82, 0x00, 0x00, 0x03, 			0x82, 0x00, 0x80, 0x0A,
//			0x04, 0x82, 0x00, 0x00, 0x04, 			0x82, 0x00, 0x80, 0x0B,
//			0x85, 0x82, 0x00, 0x00, 0x05, 			0x82, 0x00, 0x80, 0x0C


//	0x82, 0x03,																	//	Table, Seat
//					0x84, 0x00, 0x42, 0x69, 0x6C, 0xEC,					//	Flag, 'Bill'
//			0x01,																	//	Seat
//					0xE2, 0x00, 0x00, 0x04, 0x83, 0xFF, 0x6A,			//	Flag, Dish, Opt, PriceAdj,
//					0xD2, 0x00, 0xC0,	0x04,	0x01,	0x01,	0x82, 		//	Flag, Drink, Size, Opt1,2
//						0x48, 0x65, 0x6C, 0x6C, 0xEF,						//	'Hello'
//					0xD2, 0x00, 0x40,	0x04,	0x01,	0x01,	0x82, 		//	Flag, Drink, Size, Opt1,2
//						0x48, 0x65, 0x6C, 0x6C, 0xEF,						//	'Hello'
//					0x8A, 0x00,	0xC0,	0x0A, 0x40,	0x07, 0xC0,	0x09, 0x02,	//	Flag, Mix 1,2,3, Size
//			0x84,																	//	Seat
//					0x84, 0x00, 0x53, 0x75, 0xE5,							//	Flag, 'Sue'
//					0xC2, 0x00, 0x40,	0x01,	0x03, 0x01, 0x02,	0x83,	//	Flag, Drink, Size, Opt 1,2,3
//					0x82, 0x00, 0xC0, 0x05, 0x04							//	Flag, Drink, Size

	};


	for(i=0; i<MyALReceiveBuffSize; i++)
		ALReceivePtr[i] = 0;					//	Zero out the buffer
	
	for(i=0; i<sizeof(fakeOrders); i++)
		ALReceivePtr[i] = fakeOrders[i];	//	Copy the Fake orders
	
	messageLength = i + 2;						//	Allow for the CRC
	ALReceivePtr[HLHdrByteCount] = (i - HLHeaderSize) >> 8;
	ALReceivePtr[HLHdrByteCount+1] = (i - HLHeaderSize) & 0xFF;

#endif



	messageLength -= 2;								//	Don't attempt to Parse the CRC!
	
	i = HLHeaderSize;

	currentTableNumber = Prefs.current.tableNo;

	recIndex = 0;
	recSizeIndex = 0;
	recSeatnameIndex = DmNumRecords(MmSeatnameDB);	//	New names will be added at the end
	
	numDishesDB = DmNumRecords(MmDishesDB);
	numDrinksDB = DmNumRecords(MmDrinksDB);

	//	Only show the progress bar if its worthwhile
	if(messageLength > PrgMinMessageLength)
	{
		doDisplayProgress = true;

		// Starts the progress dialog. Identifies the callback function.
		prgPtr = PrgStartDialog("Processing Orders ...", * MmProgressCallback, NULL);
	
		//	Talk to the callback function and pass it the "stage" value.  
		count = 12;
		PrgUpdateDialog(prgPtr, 0, count, NULL, true);
	
		startTick = TimGetTicks();
	}
	else
		doDisplayProgress = false;
	
	cancelPending = false;
	

	sectionFlag = ALReceivePtr[i++];



	//	Only enable stuff if there are to be new disables

	//	Go through entire DB enabling all Dishes if there are disabled dishes
	if(sectionFlag & SectionReEnableAllDishes)
	{
		enabled = StatusAsEnabled;
		//	Go through entire DB enabling all dishes
		for(j=0; j < numDishesDB; j++)
		{
			recMenuHandle = DmGetRecord(MmDishesDB, j);
			// If the record was found, enable it
			if(recMenuHandle)
			{
				recMenuPtr = MemHandleLock(recMenuHandle);
				if(recMenuPtr->enabled != StatusAsEnabled)
				{
					error = DmWrite(recMenuPtr,
								(Char*)&recMenuPtr->enabled - (Char*)&recMenuPtr->menuID,
											&enabled, sizeof(recMenuPtr->enabled));
					ErrFatalDisplayIf(error, "Could not write 'Enabled' status to record.");
				}
				MemPtrUnlock(recMenuPtr);
				error = DmReleaseRecord(MmDishesDB, j, true);
			}
			
		}
	}
		
	//	Go through entire DB enabling all drinks if there are disabled drinks
	if(sectionFlag & SectionReEnableAllDrinks)
	{
		enabled = StatusAsEnabled;
		for(j=0; j < numDrinksDB; j++)
		{
			recMenuHandle = DmGetRecord(MmDrinksDB, j);
			// If the record was found, enable it
			if (recMenuHandle)
			{
				recDrinkMenuPtr = MemHandleLock(recMenuHandle);
				if(recDrinkMenuPtr->enabled != StatusAsEnabled)
				{
					error = DmWrite(recDrinkMenuPtr,
						(Char*)&recDrinkMenuPtr->enabled - (Char*)&recDrinkMenuPtr->menuID,
											&enabled, sizeof(recDrinkMenuPtr->enabled));
					ErrFatalDisplayIf(error, "Could not write 'Enabled' status to record.");
				}
				MemPtrUnlock(recDrinkMenuPtr);
				error = DmReleaseRecord(MmDrinksDB, j, true);
			}
		}
	}
		

	//	Now Disable those Dishes and Drinks as required

	if(sectionFlag & SectionHasDisableItems)
	{
		enabled = StatusAsDisabled;

		
		do
		{
			//	Next will be the Course or Dish

			uniqueID = 0;
			uniqueID |= ALReceivePtr[i++] << 8;
			uniqueID |= ALReceivePtr[i++];

			//	If its a Dish
			if((uniqueID & IsDrinkMask) != IsDrinkMask)
			{
				currentDishIndex = MmFindSortItemsUniqueID(MmDishesDB, uniqueID & 0x7FFF);

				recMenuHandle = DmQueryRecord(MmDishesDB, currentDishIndex & 0x7FFF);
				// If the record was found, Disable it
				if (recMenuHandle)
				{
					recMenuHandle = DmGetRecord(MmDishesDB, currentDishIndex & 0x7FFF);
					recMenuPtr = MemHandleLock(recMenuHandle);
					error = DmWrite(recMenuPtr,
								(Char*)&recMenuPtr->enabled - (Char*)&recMenuPtr->menuID,
											&enabled, sizeof(recMenuPtr->enabled));
					ErrFatalDisplayIf(error, "Could not write 'Disabled' status to record.");
					MemPtrUnlock(recMenuPtr);
					error = DmReleaseRecord(MmDishesDB, currentDishIndex & 0x7FFF, true);
				}
			}
			else	//	It must be a Drink
			{
				currentDishIndex = MmFindSortItemsUniqueID(MmDrinksDB, uniqueID & 0x7FFF);
				
				recMenuHandle = DmQueryRecord(MmDrinksDB, (currentDishIndex & ~IsDrinkMask & 0x7FFF));
				// If the record was found, Disable it
				if (recMenuHandle)
				{
					recMenuHandle = DmGetRecord(MmDrinksDB, (currentDishIndex & ~IsDrinkMask & 0x7FFF));
					recDrinkMenuPtr = MemHandleLock(recMenuHandle);
					error = DmWrite(recDrinkMenuPtr,
						(Char*)&recDrinkMenuPtr->enabled - (Char*)&recDrinkMenuPtr->menuID,
											&enabled, sizeof(recDrinkMenuPtr->enabled));
					ErrFatalDisplayIf(error, "Could not write 'Disabled' status to record.");
					MemPtrUnlock(recDrinkMenuPtr);
					error = DmReleaseRecord(MmDrinksDB, (currentDishIndex & ~IsDrinkMask & 0x7FFF), true);
				}
			}
		}while(i < messageLength && uniqueID < 0x8000);
	}
	


//ShowDebug("Section Flag %x", sectionFlag);

	//	Might be a Seatname
	if(sectionFlag & SectionHasSeatname)
	{
//ShowDebug("SectionHasSeatname");

		MmZapDB(&MmSeatnameDB, MmSeatnameDBName, RecreateDB, &DBDeleteBitMask, MmSeatnameMaskBit);

		recSeatnameIndex = 1;
		
		while(i < messageLength)
		{
			//	Do the Table
			if(doTable == true)
			{
				doTable = false;								//	Don't do again
				seatnameRecord.tableNumber = ALReceivePtr[i++];
				if(seatnameRecord.tableNumber >= 0x80)	//	Must be last Table
				{
					lastTable = true;
					seatnameRecord.tableNumber &= 0x7F;	//	Don't store the MSB
				}
				currentTable = seatnameRecord.tableNumber;
			}
			else
			{
				seatnameRecord.tableNumber = currentTable;
			}
		
			//	Now do the Seats
			seatnameRecord.seatNumber = ALReceivePtr[i++];
			if(seatnameRecord.seatNumber >= 0x80)		//	Must be last Seat
			{
				lastSeat = true;
				seatnameRecord.seatNumber &= 0x7F;		//	Don't store the MSB
			}
	
			//	Get the seatname
			j=0;
			do
			{
				if(j < MaxSeatNameSize)
					seatnameRecord.name[j++] = ALReceivePtr[i++] & 0x7F;	//	Strip MSB
				else
					ALReceivePtr[i++];
			}while(ALReceivePtr[i-1] < 0x80);
	
			seatnameRecord.name[j] = 0;					//	Termination null
	
			//	Generate a new seatname
			MmGenerateSeatOrTableRecord(&MmSeatnameDB, &seatnameRecord, recSeatnameIndex);
			
			recSeatnameIndex++;

			if(lastSeat)
			{
				lastSeat = false;
				doTable = true;

				if(lastTable)
				{
					break;
				}
			}
		}
	}



/*
			Was above. Trash later.

			//	If the seatName wasn't found, but there is a name, generate a new seatname
			if(!(MmGetSeatnameIndexAndUpdate(&DBs,
								seatnameRecord.tableNumber, seatnameRecord.seatNumber,
									&recSeatnameIndex, seatnameRecord.name, ReplaceSeatname)) &&
				(seatnameRecord.name[0] != 0))
			{
				//	Create a record at the correct index in the DB
				MmGenerateSeatOrTableRecord(&MmSeatnameDB, &seatnameRecord, recSeatnameIndex);
			}
*/









/*
//	Transmission Message First Flag
#define FlagsCreditDish					0x01		//	Used for Transmission Message Flags
#define FlagsHasDishOrDrink			0x02
#define FlagsHasSeatname				0x04
#define FlagsHasItemLinks				0x08		//	Mixed drink (split), or linked dish (Side)
#define FlagsHasNote 					0x10
#define FlagsHasPriceAdj				0x20
#define FlagsHasOption 					0x40
#define FlagsHasSetMenuMask			0x80

//	Transmission Message Second Flag
#define FlagsNA1							0x01		//	Used for Transmission Message Flags
#define FlagsNA2							0x02
#define FlagsNA3							0x04
#define FlagsNA4 							0x08
#define FlagsNA5 							0x10
#define FlagsHasPriceCorrection		0x20		//	Dollar value from POS for non Palm items
#define FlagsHasSplitBill				0x40
*/









	//	Now create the Orders
	if(sectionFlag & SectionHasGetOrSendOrders)
	{
		doTable		= true;
		doSeat		= true;
	
		lastTable	= false;
		lastSeat		= false;
		
		


		//	Main Orders Loop
		while(i < messageLength && !cancelPending)
		{
			//	Set up a Prefs.curOrder profile
			MmCreateRecordTemplate(&Prefs);						//	Fill in Prefs.curOrder defaults
			Prefs.curOrder.status = StatusAsSent | StatusAsSentConfirmed;
	
			//	Do the Table
			if(doTable == true)
			{
				//	Only show the progress bar if its worthwhile
				if(doDisplayProgress)
				{
					//	Update the progress counter
					endTick = TimGetTicks();
					if(endTick - startTick > 50)			//	Update each half second
					{
						startTick = endTick;
	
						if(--count == 0)
							count = 12;
	
						//	Talk to the callback function and pass it the "stage" value.  
						PrgUpdateDialog(prgPtr, 0, count, NULL, true);

				      EvtGetEvent(&event, 0);				// Check for an event, now.

						if(!PrgHandleEvent(prgPtr, &event))
						{
							if(PrgUserCancel(prgPtr))
								cancelPending = true;
						}
					}
				}

				doTable = false;								//	Don't do again
				Prefs.curOrder.tableNumber = ALReceivePtr[i++];
				if(Prefs.curOrder.tableNumber >= 0x80)	//	Must be last Table
				{
					lastTable = true;
					Prefs.curOrder.tableNumber &= 0x7F;	//	Don't store the MSB
				}
				currentTable = Prefs.curOrder.tableNumber;
			}
			else
			{
				Prefs.curOrder.tableNumber = currentTable;
			}
		



			//	Now do the Seats
			if(doSeat == true)
			{
				doSeat = false;								//	Don't do again
				Prefs.curOrder.seatNumber = ALReceivePtr[i++];
				if(Prefs.curOrder.seatNumber >= 0x80)		//	Must be last Seat
				{
					lastSeat = true;
					Prefs.curOrder.seatNumber &= 0x7F;		//	Don't store the MSB
				}
				currentSeat = Prefs.curOrder.seatNumber;
			}
			else
			{
				Prefs.curOrder.seatNumber = currentSeat;
			}
		
	



			//	Get the Flags UInt8(s). (There are always two)
			flags = ALReceivePtr[i++];
			flags2 = ALReceivePtr[i++];
			

			if(flags2 & FlagsHasPriceCorrection)
			{
				priceCorrection = 0;

				priceTemp			=	ALReceivePtr[i++];
				priceCorrection	|=	priceTemp << 24;

				priceTemp			=	ALReceivePtr[i++];
				priceCorrection	|=	priceTemp << 16;

				priceTemp			=	ALReceivePtr[i++];
				priceCorrection	|=	priceTemp << 8;

				priceTemp			=	ALReceivePtr[i++];
				priceCorrection	|=	priceTemp;

				//	Create a Marker record if necessary
				previousSeat = Prefs.curOrder.seatNumber;
				previousTable = Prefs.curOrder.tableNumber;

				MmCreateMarkerRecord(&Prefs, MmOrdersDB, &recIndex, priceCorrection);
				setMenuGroup = SetMenuGroupMinValue;
				recMarkerIndex = recIndex;
				recIndex++;
			}
			else
			{
				priceCorrection = NoValue32Signed;
			}





			//	Now do the Dishes. (Just like home).
			if(flags & FlagsHasDishOrDrink)
			{
				//	Create a Marker record if necessary
				if(	previousSeat != Prefs.curOrder.seatNumber 	||
						previousTable != Prefs.curOrder.tableNumber		)
				{
					MmCreateMarkerRecord(&Prefs, MmOrdersDB, &recIndex, priceCorrection);
					setMenuGroup = SetMenuGroupMinValue;
					recMarkerIndex = recIndex;
					recIndex++;
				}





				//	Set the Menu Type selector
				if(ALReceivePtr[i] & IsDrinkMask8)
				{
					isDrinkItem = true;
					Prefs.curOrder.menuType = DrinksMenu;
					menuDB = MmDrinksDB;
				}
				else
				{
					isDrinkItem = false;
					Prefs.curOrder.menuType = DishesMenu;
					menuDB = MmDishesDB;
				}
		


				//	Get the Unique ID for Dish/Drink
				uniqueID = 0;
				uniqueID |= ALReceivePtr[i++] << 8;
				uniqueID |= ALReceivePtr[i++];
	
//MmPrvSyncLogTraceUI("\n1 UniqueID = %d", uniqueID);
				if(uniqueID >= 0x8000)										//	Must be last Dish
				{
					lastDish = true;
					uniqueID &= 0x7FFF;										//	Don't store the MSB
				}
	
				//	Find the Dish/Drink in the menu via its uniqueID
				currentDishIndex = MmFindSortItemsUniqueID(menuDB, uniqueID);
				Prefs.curOrder.menuIndex[0].index = currentDishIndex;
				Prefs.curOrder.menuIndex[0].size  = ALReceivePtr[i++];
				Prefs.curOrder.size  = Prefs.curOrder.menuIndex[0].size;
//PrvSyncLogTrace("\n1 Index = %d, UniqueID = %d,\n     Size = %d", currentDishIndex, uniqueID, Prefs.curOrder.menuIndex[j].size);
		
				//	Get the servingCourseID for Dish/Drink
				uniqueID = 0;
				uniqueID |= ALReceivePtr[i++] << 8;
				uniqueID |= ALReceivePtr[i++];
				Prefs.curOrder.servingCourseID = uniqueID;
	
	
				if((!isDrinkItem	&&	(currentDishIndex > numDishesDB)) ||
					( isDrinkItem	&&	(currentDishIndex > numDrinksDB))		)
				{
//					Prefs.current.mMenu = currentMenu;						//	Restore
					Prefs.current.tableNo = currentTableNumber;		//	Restore
					ErrThrow(HLParseNotValidOrder);
				}
	


				//	If this item has sides or is a drink mix there must be multiples
				if(flags & FlagsHasItemLinks)
				{
					j = 1;
					
					lastLinkedItem = false;

					do
					{
						uniqueID = 0;
						uniqueID |= ALReceivePtr[i++] << 8;
						uniqueID |= ALReceivePtr[i++];
			
						if(uniqueID >= 0x8000)								//	Must be last Dish
						{
							lastLinkedItem = true;
							uniqueID &= 0x7FFF;								//	Don't store the MSB
						}
			
						//	Find the Dish/Drink in the menu via its uniqueID
						Prefs.curOrder.menuIndex[j].index = MmFindSortItemsUniqueID(menuDB, uniqueID);
						Prefs.curOrder.menuIndex[j].size  = ALReceivePtr[i++];
	
						j++;
	
					}while((!lastLinkedItem) && (j < MaxLinkedItems));
	
					//	There must also be a main size type
					Prefs.curOrder.size  = ALReceivePtr[i++];
				}
				



				//	Get the SetMenuGroup if appropriate. SetMenuMask is set later
				if(flags & FlagsHasSetMenuMask)
				{
//					Prefs.curOrder.setMenuGroup = ALReceivePtr[i++];	//	Old 8 bit value

					setMenuGroupTemp = 0;
					setMenuGroupTemp |= ALReceivePtr[i++] << 8;
					setMenuGroupTemp |= ALReceivePtr[i++];

					if(setMenuGroupTemp >= SetMenuGroupMinValue)		//	Just = 0, with MSB set
					{
						ErrThrow(HLParseInvalidSetMenuNumber);
					}

					Prefs.curOrder.setMenuGroup = setMenuGroupTemp;
					
					//	Find the highest numbered setMenuGroup
					if((setMenuGroupTemp | SetMSB16) > setMenuGroup)
					{
						setMenuGroup = setMenuGroupTemp | SetMSB16;

						//	Go back and save the setMenuGroup into the previous Marker record
						recMarkerHandle = DmGetRecord(MmOrdersDB, recMarkerIndex);
						if(recMarkerHandle)
						{
							Err	err;
							
							recMarkerPtr = MemHandleLock(recMarkerHandle);

							err = DmWrite(recMarkerPtr, (Char*)&recMarkerPtr->setMenuGroup - (Char*)&recMarkerPtr->tableNumber,
															&setMenuGroup, sizeof(recMarkerPtr->setMenuGroup));
							ErrFatalDisplayIf(err, "Could not write to Marker record for setMenuGroup.");

							MemPtrUnlock(recMarkerPtr);
							err = DmReleaseRecord(MmOrdersDB, recMarkerIndex, true);
							ErrFatalDisplayIf(err, "Could not release record after writing setMenuGroup.");

//ShowDebug("%d, %d, %d, %d", Prefs.curOrder.tableNumber, Prefs.curOrder.seatNumber, recMarkerIndex, setMenuGroup);
						}
					}
				}
		



				//	Now do the Options.
				if(flags & FlagsHasOption)
				{
					j=0;
					do
					{
						Prefs.curOrder.options[j++] = ALReceivePtr[i++];	//	Copy Options
					}while(!(ALReceivePtr[i-1] & 0x80));		//	while(MSB not set)
				}	




				//	If there is a note
				if(flags & FlagsHasNote)
				{
					j=0;
					do
					{
						if(j < MaxNoteLength)
						{
							Prefs.curOrder.note[j++] = ALReceivePtr[i] & 0x7F;	//	Strip MSB
						}
						i++;
					}while(ALReceivePtr[i-1] < 0x80);
					Prefs.curOrder.noteLength = j;
				}


		
	
				//	Look up Course Number, Item number and price for this dish
				recMenuHandle = DmQueryRecord (menuDB, Prefs.curOrder.menuIndex[0].index);
				if(recMenuHandle)
				{
					recMenuPtr = MemHandleLock(recMenuHandle);

					Prefs.curOrder.courseNumber = recMenuPtr->courseNumber;
					Prefs.curOrder.itemNumber = recMenuPtr->itemNumber;
					Prefs.curOrder.itemInCourse = recMenuPtr->itemInCourse;

					if(flags & FlagsHasSetMenuMask)
					{
						//	Find and set the correct mask
						for(j=0; j < MaxSizeTypes; j++)
						{
							if(recMenuPtr->sizes[j].size == Prefs.curOrder.size)
							{
								//	Save the setMenuMask with the order
								Prefs.curOrder.setMenuMask = recMenuPtr->sizes[j].setMenuMask;
								break;
							}
						}
					}

					MemPtrUnlock(recMenuPtr);
				}
	
		
	
	

				//	Check for multiple dishes to set itemCounter
				if(	Prefs.curOrder.menuIndex[0].index	== previousDish	&&
						Prefs.curOrder.seatNumber		 		== previousSeat	&&
						Prefs.curOrder.tableNumber				== previousTable	&&
						Prefs.curOrder.menuType					==	previousOrderType)
				{
					itemCounter++;
				}
				else
				{
					//	Either way, set these up for next time
					previousTable		= Prefs.curOrder.tableNumber;
					previousSeat		= Prefs.curOrder.seatNumber;
					previousDish		= Prefs.curOrder.menuIndex[0].index;
					previousOrderType	= Prefs.curOrder.menuType;
					itemCounter			= 0;						//	Reset for new dish
				}
		
				Prefs.curOrder.itemCounter = itemCounter;	//	Need to save with record
				Prefs.current.recIndex = NoValue16;
		
		

#ifdef CountOrdersSent
debugOrdersRxd++;
#endif

				MmCreateGetAllOrdersRecord(&Prefs, MmOrdersDB, &recIndex);	//	Now create the new record
				recIndex++;
			}
			else		//	Okay. There were no dishes or drinks
			{
				lastDish = true;
			}
			
			if(lastDish)
			{
				lastDish = false;
				doSeat = true;
	
				if(lastSeat)
				{
					lastSeat = false;
					doTable = true;
	
					if(lastTable)
					{
						break;
					}
				}
			}
		}	//	if()
	}	//	Do Dishes/Drinks while()


#ifdef CountOrdersSent
MmPrvSyncLogTraceUI("\nOrders Rxd = %d,", debugOrdersRxd);
if(OrdersCount != debugOrdersRxd)
	FrmAlert(WarnTxdRxdOrdersDontEquateAlert);

OrdersCount = debugOrdersRxd;
#endif

//	Prefs.current.mMenu = currentMenu;							//	Restore if no Orders
	Prefs.current.tableNo = currentTableNumber;



	//	Might be a Message (Actually, From Message followed by actual message)
	if(sectionFlag & SectionHasMessage)
	{
		//	Get the From Message
		j=0;
		do
		{
			if(j < MaxMessageFromServerLength)
			{
				ServerMessageFrom[j++] = ALReceivePtr[i] & 0x7F;	//	Strip MSB
			}
			i++;
		}while(ALReceivePtr[i-1] < 0x80);

		ServerMessageFrom[j] = 0;			//	Termination null for From string

		//	Get the Actual Message
		j=0;
		do
		{
			if(j < MaxMessageFromServerLength)
			{
				ServerMessageIs[j++] = ALReceivePtr[i] & 0x7F;	//	Strip MSB
			}
			i++;
		}while(ALReceivePtr[i-1] < 0x80);

		ServerMessageIs[j] = 0;				//	Termination null for new string

	}
	else
	{
		ServerMessageIs[0] = 0;				//	Termination null because no string
	}


	//	Set Happy Hour flag
	if(sectionFlag & SectionIsInHappyHour)
	{
		InHappyHour = true;
	}
	else
	{
		InHappyHour = false;
	}




	if(doDisplayProgress)
	{
		PrgStopDialog(prgPtr, true);		//	Erase the progress dialog.
	}
	
//	Prefs.current.setMenuGroup = SetMenuGroupMinValue;	//	Reset since we've synchronised

	MmSavePrefs();								//	Save these now in case we break

	if(cancelPending)
		return cancelPending;
	else
		return 0;
}


/***********************************************************************
 *
 * FUNCTION:		MmAssemblePalmUniqueIDMessage		
 *
 * DESCRIPTION:	Assembles the data for a Registration Message.
 *						Gets palmUniqueName from BT address
 *
 * PARAMETERS:		.
 *
 * RETURNED:		.
 *
 ***********************************************************************/
static void MmAssemblePalmUniqueIDMessage(UInt16* ptrSendIndex, UInt8* buffer,
																UInt16* dataCount)
{
	UInt16				i;									//	Local counter
	UInt16				length;

	*dataCount = 0;

	length = StrLen(Prefs.palmUniqueName);
	if(length == 0 || length > PalmUniqueNameLength)
		ErrThrow(HLAssemblePalmUniqueIDNoID);

	for(i=0; i<length; i++)
	{
		buffer[(*ptrSendIndex)++] = Prefs.palmUniqueName[i];	//	Copy the BT address
		(*dataCount)++;
	}
	buffer[(*ptrSendIndex)++] = 0;
	(*dataCount)++;
}



/***********************************************************************
 *
 * FUNCTION:		MmAssemblePalmRegistrationMessage		
 *
 * DESCRIPTION:	Assembles the data for a Registration Message.
 *
 * PARAMETERS:		.
 *
 * RETURNED:		.
 *
 ***********************************************************************/
static void MmAssemblePalmRegistrationMessage(UInt16* ptrSendIndex, UInt8* buffer,
																UInt16* dataCount)
{
	UInt16				i;									//	Local counter
	UInt16				length;

	*dataCount = 0;


	//	Unique ID of the Palm we are to replace
	buffer[(*ptrSendIndex)++] = (UInt8)(RegTemp.uniqueID >> 8);
	(*dataCount)++;
	buffer[(*ptrSendIndex)++] = (UInt8)(RegTemp.uniqueID & 0xFF);
	(*dataCount)++;


	//	Try to re-register using existing conditions (in case battery just went flat)
	if(RegTemp.autoReRegister)
	{
		StrCopy(RegTemp.newName, "");
	}
	else
	{
		//	Registration name
		length = StrLen(RegTemp.newName);
		if(length == 0 || length > PalmRegistrationNameLength)
		{
			ErrThrow(HLAssemblePalmRegNoRegName);
		}
	}

	for(i=0; i<length; i++)
	{
		buffer[(*ptrSendIndex)++] = RegTemp.newName[i];	//	Copy the name across
		(*dataCount)++;
	}
	buffer[(*ptrSendIndex)++] = 0;
	(*dataCount)++;


	//	Registration PIN
	for(i=0; i<MaxPINSize; i++)
	{
		buffer[(*ptrSendIndex)++] = RegTemp.pIN[i];		//	Copy the PIN across
		(*dataCount)++;
	}


//ShowMessage("Palm Flags = %d", RegTemp.flags);


	if(RegTemp.syncPrintingToID == 0)
	{
		RegTemp.flags &= ~RegSyncPrinting;					//	Don't let ID of 0 sneak out
	}
	

	//	Registration Flags
	buffer[(*ptrSendIndex)++] = (UInt8)(RegTemp.flags);
	(*dataCount)++;

//ShowMessage("Palm Flags = %d", RegTemp.flags);


	//	Add syncPrintingToID, or 0
	if(RegTemp.flags & RegSyncPrinting)
	{
		buffer[(*ptrSendIndex)++] = (UInt8)(RegTemp.syncPrintingToID >> 8);
		(*dataCount)++;
		buffer[(*ptrSendIndex)++] = (UInt8)(RegTemp.syncPrintingToID & 0xFF);
		(*dataCount)++;
	}
	else
	{
		buffer[(*ptrSendIndex)++] = 0;
		(*dataCount)++;
		buffer[(*ptrSendIndex)++] = 0;
		(*dataCount)++;
	}
}




/***********************************************************************
 *
 * FUNCTION:		MmAssembleLoginMessage		
 *
 * DESCRIPTION:	Assembles the data for a Login Message.
 *
 * PARAMETERS:		.
 *
 * RETURNED:		.
 *
 ***********************************************************************/
static void MmAssembleLoginMessage(UInt16* ptrSendIndex, UInt8* buffer,
																UInt16* dataCount)
{
	UInt16				i;									//	Local counter
	UInt16				staffIDNumber;

	*dataCount = 0;

	//	First Paste the new Prefs.staffIDNo
	MmGetStaffID(StaffType, &staffIDNumber, Prefs.staffIDIndex, MmInfoDB);
	buffer[(*ptrSendIndex)++] = (UInt8)(staffIDNumber >> 8);	//	Insert selected staffID Number
	buffer[(*ptrSendIndex)++] = (UInt8)(staffIDNumber & 0xFF);


	#ifdef TestMessageLog
	if(Prefs.msgLogEnabled)
	{
		MmPrvSyncLogTrace("\nPIN Number = ");
	}
	#endif

	//	Copy the Prefs.pIN Number to the buffer
	for(i=0; i < MaxPINSize; i++)
	{
		buffer[*ptrSendIndex] = Prefs.pIN[i];			//	Copy the data across
		(*ptrSendIndex)++;

		#ifdef TestMessageLog
		if(Prefs.msgLogEnabled)
		{
			MmPrvSyncLogTraceUI("%c", Prefs.pIN[i]);		//	Log the Prefs.pIN
		}
		#endif
	}
	*dataCount = i+2;									//	Include new Prefs.staffIDNo
}




/***********************************************************************
 *
 * FUNCTION:		MmAssembleSendSeatnamesMessage
 *
 * DESCRIPTION:	Assembles the data for a Billing Message to Server.
 *						This function  scans of the DB checking
 *						for records marked for Billing.
 *
 *
 * PARAMETERS:		.
 *
 * RETURNED:		.
 *
 ***********************************************************************/
static void MmAssembleSendSeatnamesMessage(UInt16* ptrSendIndex, UInt16* dataCount)
{
	UInt16				numberOfRecords;
	MemHandle			recHandle;

	UInt16				recIndex;
	SeatnameTypePtr	recPtr;
	
	Boolean				firstPass			= true;
	
	UInt16				highestTable		= 0;
	UInt16				highestSeat			= 0;
	
	UInt16				thisTable			= 0;
	
	UInt16				i;
	
	

	*dataCount = *ptrSendIndex;

	//	Go through all the records for this database
	numberOfRecords = DmNumRecords(MmSeatnameDB);

	//	Step through the DB, checking for Seatnames that need sending
	recIndex=0;
	while(recIndex < numberOfRecords)
	{
		recHandle = DmQueryRecord (MmSeatnameDB, recIndex);
		
		// If the record was found, only process if Billing requested
		if(recHandle)
		{
			recPtr = MemHandleLock(recHandle);


			if(recPtr->status & StatusAsHasSeatnameToSend)
			{
				//	Table Number
				if(recPtr->tableNumber != thisTable)
				{
					SendBuff[*ptrSendIndex] = (UInt8)(recPtr->tableNumber);
					highestTable = (*ptrSendIndex)++;	//	Remember, so we can change later

					thisTable = recPtr->tableNumber;
					if(firstPass == false)
						SendBuff[highestSeat] |= 0x80;	//	Set MSB to show its last Course
					firstPass = false;						//	First pass through
				}
				//	Seat Number
				SendBuff[*ptrSendIndex] = (UInt8)(recPtr->seatNumber);
				highestSeat = (*ptrSendIndex)++;			//	Remember, so we can change later

				//	Seat Name
				i=0;
				do
				{
					SendBuff[(*ptrSendIndex)++] = (UInt8)(recPtr->name[i]);
					i++;
				}while(recPtr->name[i] != 0);

				SendBuff[(*ptrSendIndex) -1] |= 0x80;	//	Last character

				MemPtrUnlock(recPtr);
			}
			else
			{
				MemPtrUnlock(recPtr);
			}

		}	//	End if(recHandle)
		recIndex++;
	}	//	End for loop
	
	//	The dataCount is the difference between starting and ending *ptrSendIndex
	*dataCount = *ptrSendIndex - *dataCount;
	
	if(*dataCount)
	{
		SendBuff[highestTable] 	|= 0x80;	//	Set MSB to show its the last Table
		SendBuff[highestSeat] |= 0x80;	//	Set MSB to show its last Seat
	}
}	




/***********************************************************************
 *
 * FUNCTION:		MmAssembleOrdersMessage
 *
 * DESCRIPTION:	Assembles the data for a Message of Orders to Server.
 *
 * PARAMETERS:		.
 *
 * RETURNED:		.
 *
 ***********************************************************************/
static UInt16 MmAssembleOrdersMessage(UInt16* ptrSendIndex, UInt16* dataCount, Boolean isBilling)
{
	UInt16					numberOfRecords;
	MemHandle				recHandle;

	UInt16					recIndex;
	OrderTypePtr			recPtr;
	MenuTypePtr				recMenuPtr;
//	MenuTypePtr				recDrinkPtr;
	UInt8						*optionsPtr;
	
	DmOpenRef				menuDB;

	Err						err;
	UInt16					funcStatus = HLAssembleMessageOkay;
	UInt16					status;										//	Local copy of record status
	UInt8						flags;
	UInt8						seatSectionFlag;
	UInt16					flagIndex;
	UInt16					seatSectionFlagIndex;
	UInt16					uniqueID;

	UInt32					timeNow;
	UInt32					longestTime;
	UInt32					secsSinceOrdered;
	UInt32					previousLongTime;
	Boolean					priceAdjustAdded;
	Boolean					firstPass;
	Boolean					wasTable;
	
	UInt16					highestTable	= 0;
	UInt16					highestSeat		= 0;
	UInt16					highestDish		= 0;
	UInt16					highestOption	= 0;
	
	UInt16					thisTable		= 0;
	UInt16					thisSeat			= NoValue16;
	UInt16					thisDish			= NoValue16;
	UInt16					newDish;
	UInt16					thisCounter		= 0;
	UInt16					thisOption		= 0;

	Boolean					insertTime		= false;
	UInt16					timeIndex		= 0;
	
	UInt16					i;
	UInt8						option;

#ifdef CountOrdersSent
	UInt16					debugOrdersSent = 0;
#endif

	*dataCount = *ptrSendIndex;
	firstPass = true;
	priceAdjustAdded = false;
	//	Get Current time so Time difference may be found
	timeNow = TimGetSeconds();
	longestTime = 0;
	



	//	Go through all the records for this database
	numberOfRecords = DmNumRecords(MmOrdersDB);

	//	Step through the DB, assembling the unsent record data
	//	Bail out if we have processed the number of records asked for (avoid buffer overflow)
	for(recIndex=0; recIndex < numberOfRecords; recIndex++)
	{
		//	Also bail out if we have too many records (avoid buffer overflow)
		if(*ptrSendIndex > MySendBuffSize)
		{
			funcStatus = HLAssembleOrdersTooMany;
			break;
		}

		recHandle = DmGetRecord(MmOrdersDB, recIndex);
		
		// If the record was found, only process if not already sent or confirmed
		if(recHandle)
		{
			recPtr = MemHandleLock(recHandle);

			//	If not a Billing operation: Check that Record is not sent
			//	If it is a Billing operation: Check that Record is marked for splitting and
			//	it is not a Marker record.
			if(	!isBilling													&&
					!(recPtr->status & StatusAsSent)									
//				 																				||
//				(	isBilling													&&
//					(recPtr->splitting & SplittingOrderMarked)		&&
//					(recPtr->courseNumber != 0)									)		
																									)
			{
				wasTable = false;

				//	First send table no.
				if(recPtr->tableNumber != thisTable)
				{
					wasTable = true;
					if(!firstPass)
					{
						timeIndex = *ptrSendIndex;			//	We'll write the time in here later
						*ptrSendIndex +=2;
						insertTime = true;
						previousLongTime = longestTime;
						longestTime = 0;						//	Reset for next Seat
					}

					if(recPtr->tableNumber <= LastTable)
						SendBuff[*ptrSendIndex] = (UInt8)(recPtr->tableNumber);
					else
						ErrThrow(HLAssembleOrdersTableTooHigh);
						
					thisTable = recPtr->tableNumber;
					highestTable = (*ptrSendIndex)++;	//	Remember so we can come back and change it
					if(highestSeat > 0)						//	Avoid first loop
					{
						SendBuff[highestSeat] |= 0x80;	//	Set MSB to show its the last Seat
						highestSeat = 0;						//	Reset the counter
					}
					thisSeat = NoValue16;					//	Ensure all lower entries restart
					thisDish = NoValue16;
					thisCounter = 0;
				}

				//	Set up the flags
				flags = 0;
				seatSectionFlag = 0;

				if(recPtr->noteLength == 0)
					option = recPtr->options;
				else
					option = *(&recPtr->note + recPtr->noteLength);

				if(option != 0x80)
				{
					flags |= FlagsHasOption;
				}

				if(recPtr->price != NoValue32Signed)
					flags |= FlagsHasPriceAdj;

				if(recPtr->noteLength > 0)
					flags |= FlagsHasNote;

				if(recPtr->setMenuMask)
					flags |= FlagsHasSetMenuMask;

				if(recPtr->menuIndex[1].index != NoValue16)
					flags |= FlagsHasItemLinks;

				//	Avoid sending 'Credit' bit when sending original order for splits
				if(recPtr->status & StatusAsCreditDish)
				{
					flags |= FlagsCreditDish;
				}


				//	Next is the seat no.					
				if(recPtr->seatNumber != thisSeat)
				{
					if(wasTable == false)					//	Haven't just done a Table

					{
						timeIndex = *ptrSendIndex;			//	We'll write the time in here later
						*ptrSendIndex +=2;
						insertTime = true;
						previousLongTime = longestTime;
						longestTime = 0;						//	Reset for next Seat
					}


					//	Paste in the Seat Number
					if(recPtr->seatNumber <= LastSeat)
						SendBuff[*ptrSendIndex] = (UInt8)(recPtr->seatNumber);
					else
						ErrThrow(HLAssembleOrdersSeatTooHigh);
						
					thisSeat = recPtr->seatNumber;
					highestSeat = (*ptrSendIndex)++;		//	Remember so we can come back and change it
					if(highestDish > 0)						//	Avoid first loop
					{
						SendBuff[highestDish] |= 0x80;	//	Set MSB to show its the last Course
						highestDish = 0;						//	Reset the counter
					}
					thisDish = NoValue16;
					thisCounter = 0;
					
					//	Write the section flag, and remember where it is for later
					//	There may be orders yet
					SendBuff[*ptrSendIndex] = (UInt8)(seatSectionFlag);
					seatSectionFlagIndex = (*ptrSendIndex)++;

					//	... and paste the Flags UInt8
					flagIndex = *ptrSendIndex;				//	Save so we can change if Dish/Drink
					SendBuff[(*ptrSendIndex)++] = (UInt8)(flags);
				}
				else	//	Is same seat, so just carry on with orders etc.
				{
					//	Paste in the Flags UInt8
					flagIndex = *ptrSendIndex;				//	Save so we can change if Dish/Drink
					SendBuff[(*ptrSendIndex)++] = (UInt8)(flags);
				}


				//	Now do the Dishes (with Options and Note)

				if(recPtr->menuType == DishesMenu)
					menuDB = MmDishesDB;
				else
					menuDB = MmDrinksDB;
					
				//	Look up the uniqueID for this Dish/Drink
				recHandle = DmQueryRecord(menuDB, recPtr->menuIndex[0].index);
				if(recHandle)
				{
					recMenuPtr = MemHandleLock(recHandle);
					newDish = recMenuPtr->uniqueID;
//if(newDish == 0)
//	ShowMessage("UniqueID = 0");
					MemPtrUnlock(recMenuPtr);
				}

				//	Set the Drinks bit if this is a Drink
				if(recPtr->menuType != DishesMenu)
					newDish |= IsDrinkMask;

				//	If its a genuine dish, thats different to the last
				if(recPtr->courseNumber != 0 &&
					(newDish != thisDish || recPtr->itemCounter != thisCounter)	)
				{
#ifdef CountOrdersSent
debugOrdersSent++;
#endif
					seatSectionFlag |= SectionHasGetOrSendOrders;
					SendBuff[seatSectionFlagIndex] |= (UInt8)(seatSectionFlag);
					
					SendBuff[flagIndex] |= FlagsHasDishOrDrink;

					//	Now do the Dishes. (Always. Just like home).

					highestDish = *ptrSendIndex;	//	Remember so we can come back and change it

					SendBuff[(*ptrSendIndex)++] = (UInt8)(newDish >> 8);
					SendBuff[(*ptrSendIndex)++] = (UInt8)(newDish & 0xFF);

					SendBuff[(*ptrSendIndex)++] = (UInt8)(recPtr->menuIndex[0].size);

					SendBuff[(*ptrSendIndex)++] = (UInt8)(recPtr->servingCourseID >> 8);
					SendBuff[(*ptrSendIndex)++] = (UInt8)(recPtr->servingCourseID & 0xFF);

					thisDish = newDish;
					thisCounter = recPtr->itemCounter;
					
			
					//	If this is a food with sides or drink mix, there must be multiples
					if(flags & FlagsHasItemLinks)
					{
						int j = 1;

						while((recPtr->menuIndex[j].index != NoValue16) && (j < MaxLinkedItems))
						{
							//	Look up the uniqueID for this Dish/Drink
							recHandle = DmQueryRecord(menuDB, recPtr->menuIndex[j].index);
							if(recHandle)
							{
								recMenuPtr = MemHandleLock(recHandle);
								uniqueID = recMenuPtr->uniqueID;
								MemPtrUnlock(recMenuPtr);
							}

							SendBuff[(*ptrSendIndex)++] = (UInt8)(uniqueID >> 8);
							SendBuff[(*ptrSendIndex)++] = (UInt8)(uniqueID & 0xFF);

							SendBuff[(*ptrSendIndex)++] = (UInt8)(recPtr->menuIndex[j].size);
							j++;
						}

						SendBuff[(*ptrSendIndex)-3] |= 0x80;		//	Set the MSB of the index

						//	Also send main size type
						SendBuff[(*ptrSendIndex)++] = (UInt8)(recPtr->size);
					}

					
					//	Output the SetMenuGroup if appropriate
					if(flags & FlagsHasSetMenuMask)
					{
//						SendBuff[(*ptrSendIndex)++] = (UInt8)(recPtr->setMenuGroup);	//	Old 8 bit value

						SendBuff[(*ptrSendIndex)++] = (UInt8)(recPtr->setMenuGroup >> 8);
						SendBuff[(*ptrSendIndex)++] = (UInt8)(recPtr->setMenuGroup & 0xFF);
					}

					
					//	Now do the options. Bit 6 = 1 -> Minus Option
					if(flags & FlagsHasOption)
					{
						if(recPtr->noteLength > 1)				//	Point to start of Options
							optionsPtr = ((UInt8*)&recPtr->options + recPtr->noteLength -1);
						else
							optionsPtr = ((UInt8*)&recPtr->options);	//	No note attached
	
						while(*optionsPtr < 0x80)
						{
							SendBuff[(*ptrSendIndex)++] = (UInt8)(*optionsPtr++);	//	Send the first options
						}
						SendBuff[(*ptrSendIndex)++] = (UInt8)(*optionsPtr++);	//	Send the last option
					}
					


					//	Send any Price Adjustments
					if(flags & FlagsHasPriceAdj)
					{
						SendBuff[(*ptrSendIndex)++] = (UInt8) (recPtr->price >> 24);	//	Send Upper UInt8.
						SendBuff[(*ptrSendIndex)++] = (UInt8)((recPtr->price >> 16)&0xFF);
						SendBuff[(*ptrSendIndex)++] = (UInt8)((recPtr->price >> 8 )&0xFF);
						SendBuff[(*ptrSendIndex)++] = (UInt8) (recPtr->price &0xFF);	//	Send Lower UInt8.
					}



					//	Send any note if attached
					if(flags & FlagsHasNote)
					{
						i = 0;
					
						optionsPtr = (UInt8*)&recPtr->note;
						while(i++ < recPtr->noteLength)
						{
							SendBuff[(*ptrSendIndex)++] = (UInt8)(*optionsPtr++);	//	Send the note.
						}
						SendBuff[(*ptrSendIndex)-1] |= 0x80;//	Show its the last UInt8
					}


					//	Finally, for each Dish find if it is the oldest
					if(recPtr->timeTaken > longestTime)
					{
						longestTime = recPtr->timeTaken;
					}

					//	Only write the time if orders have, in fact, been placed in the buffer
					if(insertTime == true)
					{
						//	Add the Time difference UInt16 for PREVIOUS Seat
						secsSinceOrdered = timeNow - previousLongTime;
						if(secsSinceOrdered > 0xFFFF)
						{
							secsSinceOrdered = 0xFFFF;
						}
						
						SendBuff[timeIndex] = (UInt8)(secsSinceOrdered >> 8);
						SendBuff[timeIndex+1] = (UInt8)(secsSinceOrdered & 0xFF);
					}
					firstPass = false;
					insertTime = false;

				}
				//	Mark this record as Sent (but not Confirmed)
				status = recPtr->status;
				status |= StatusAsSent;
				status &= ~StatusAsSentConfirmed;
				err = DmWrite(recPtr, (Char*)&recPtr->status - (Char*)&recPtr->tableNumber,
										&status, sizeof(recPtr->status));
				ErrFatalDisplayIf(err, "Could not write 'Sent' status to record.");
			}	//	End if((recPtr->status & StatusAsConfirmed) == 0)

			MemPtrUnlock(recPtr);

		}	//	End if(recHandle)

		err = DmReleaseRecord(MmOrdersDB, recIndex, true);
		ErrFatalDisplayIf(err, "Could not release record after marking as Sent.");

	}	//	End for loop
	

	//	Always add the Time difference UInt16 for last Seat
	secsSinceOrdered = timeNow - longestTime;
	if(secsSinceOrdered > 0xFFFF)
	{
		secsSinceOrdered = 0xFFFF;
	}
	
	SendBuff[(*ptrSendIndex)++] = (UInt8)(secsSinceOrdered >> 8);
	SendBuff[(*ptrSendIndex)++] = (UInt8)(secsSinceOrdered & 0xFF);

	//	The dataCount is the difference between starting and ending *ptrSendIndex
	*dataCount = *ptrSendIndex - *dataCount;

	//	If only the time was written, scotch it and adjust the counters
	if(*dataCount == 2)
	{
		*dataCount = 0;
		*ptrSendIndex -= 2;
	}
	
	if(*dataCount)
	{
		SendBuff[highestTable] 	|= 0x80;		//	Set MSB to show its the last Table
		SendBuff[highestSeat] 	|= 0x80;		//	Set MSB to show its the last Seat
		SendBuff[highestDish] 	|= 0x80;		//	Set MSB to show its the last Dish
	}

#ifdef CountOrdersSent
MmPrvSyncLogTraceUI("\nCur = %d,", OrdersCount);
MmPrvSyncLogTraceUI(" Sent = %d,", debugOrdersSent);
OrdersCount+=debugOrdersSent;
MmPrvSyncLogTraceUI(" Tot = %d,", OrdersCount);
#endif

	if(funcStatus == HLAssembleOrdersTooMany)
		return HLAssembleOrdersMoreToCome;
	else
		return HLAssembleMessageOkay;
}


/***********************************************************************
 *
 * FUNCTION:		MmAssembleCallAwayMessage
 *
 * DESCRIPTION:	Assembles the data for a CallAway Message to Server.
 *
 *
 * PARAMETERS:		.
 *
 * RETURNED:		.
 *
 ***********************************************************************/
static void MmAssembleCallAwayMessage(UInt16* ptrSendIndex, UInt16* dataCount)
{
	MemHandle				recHandle;

	UInt16					recMiscDataIndex;
	MiscDataTypePtr		recMiscDataPtr;
	
	UInt16					numMiscDataRecords;
	UInt16					highestTable		= 0;
	UInt16					highestCourse		= 0;
	
	UInt16					thisTable			= 0;

	Boolean					firstPass			= true;

	*dataCount = *ptrSendIndex;

	//	Go through all the records for this database
	numMiscDataRecords = DmNumRecords(MmMiscDataDB);

	//	Step through the DB, assembling the unsent record data
	recMiscDataIndex=0;
	while(recMiscDataIndex < numMiscDataRecords)
	{
		recHandle = DmQueryRecord(MmMiscDataDB, recMiscDataIndex);
		if(recHandle)
		{
			recMiscDataPtr = MemHandleLock(recHandle);

			if(recMiscDataPtr->recordType == CallAwayType)
			{
				if(thisTable != recMiscDataPtr->number)
				{
					SendBuff[*ptrSendIndex] = (UInt8)(recMiscDataPtr->number);	//	This Table
//ShowDebug("Sending: Table %d", (UInt8)recMiscDataPtr->number);
					highestTable = (*ptrSendIndex)++;	//	Remember, so we can change later

					thisTable = recMiscDataPtr->number;
					if(firstPass == false)
					{
						SendBuff[highestCourse] |= 0x80;	//	Set MSB to show its last ServingCourse
					}
					firstPass = false;						//	First pass through
				}

				//	Serving Courses
//ShowDebug("Sending: SC %d", recMiscDataPtr->uniqueID);
				SendBuff[*ptrSendIndex] = (UInt8)(recMiscDataPtr->uniqueID >> 8);
				highestCourse = (*ptrSendIndex)++;		//	Remember, so we can change later
				SendBuff[(*ptrSendIndex)++] = (UInt8)(recMiscDataPtr->uniqueID & 0xFF);
				MemPtrUnlock(recMiscDataPtr);
			}
			else
			{
				MemPtrUnlock(recMiscDataPtr);
				break;
			}

		}	//	End if(recHandle)
		recMiscDataIndex++;
	}	//	End for loop

	//	The dataCount is the difference between starting and ending *ptrSendIndex
	*dataCount = *ptrSendIndex - *dataCount;
	
	if(*dataCount)
	{
		SendBuff[highestTable] 	|= 0x80;	//	Set MSB to show its the last Table
		SendBuff[highestCourse] |= 0x80;	//	Set MSB to show its last Course
	}
}



/***********************************************************************
 *
 * FUNCTION:		MmAssembleBillingMessage
 *
 * DESCRIPTION:	Assembles the data for a Billing Message to Server.
 *						This function  scans of the DB checking
 *						for records marked for Billing.
 *
 *
 * PARAMETERS:		.
 *
 * RETURNED:		.
 *
 ***********************************************************************/
static void MmAssembleBillingMessage(UInt16* ptrSendIndex, UInt16* dataCount)
{
	UInt16				numberOfRecords;
	MemHandle			recHandle;

	UInt16				recIndex;
	OrderTypePtr		recPtr;
	
	Boolean				firstPass			= true;
	
	UInt16				highestTable		= 0;
	UInt16				highestSeat			= 0;
	
	UInt16				thisTable			= 0;
	

	*dataCount = *ptrSendIndex;

	//	Go through all the records for this database
	numberOfRecords = DmNumRecords(MmOrdersDB);

	//	Step through the DB, assembling the billing data
	recIndex=0;
	while(recIndex < numberOfRecords)
	{
		recHandle = DmQueryNextInCategory (MmOrdersDB, &recIndex, MarkerItem);
		
		// If the record was found, only process if Billing requested
		if(recHandle)
		{
			recPtr = MemHandleLock(recHandle);


			if(recPtr->billing & BillingSeatMarked)
			{
				if(recPtr->tableNumber != thisTable)
				{
					SendBuff[*ptrSendIndex] = (UInt8)(recPtr->tableNumber);
					highestTable = (*ptrSendIndex)++;	//	Remember, so we can change later

					thisTable = recPtr->tableNumber;
					if(firstPass == false)
						SendBuff[highestSeat] |= 0x80;	//	Set MSB to show its last Course
					firstPass = false;						//	First pass through
				}
				//	Seats
				SendBuff[*ptrSendIndex] = (UInt8)(recPtr->seatNumber);
				highestSeat = (*ptrSendIndex)++;			//	Remember, so we can change later
				MemPtrUnlock(recPtr);
			}
			else
			{
				MemPtrUnlock(recPtr);
			}

		}	//	End if(recHandle)
		recIndex++;
	}	//	End for loop
	
	//	The dataCount is the difference between starting and ending *ptrSendIndex
	*dataCount = *ptrSendIndex - *dataCount;
	
	if(*dataCount)
	{
		SendBuff[highestTable] 	|= 0x80;	//	Set MSB to show its the last Table
		SendBuff[highestSeat] |= 0x80;	//	Set MSB to show its last Seat
	}
}



/***********************************************************************
 *
 * FUNCTION:	  MmHLCalcCRC
 *
 * DESCRIPTION:  Calculate a High Level layer CRC.
 *
 * PARAMETERS:	
 *
 * RETURNED:	  
 *
 ***********************************************************************/
 void MmHLCalcCRC(UInt16 messageStart, UInt16 dataCount, UInt16* ptrSendIndex)
 {
	UInt16						i;
	UInt16 						CRC = 0;

	SendBuff[messageStart+HLHdrByteCount] = (UInt8)(dataCount >> 8);
	SendBuff[messageStart+HLHdrByteCount+1] = (UInt8)(dataCount & 0xFF);

	CRC =  StartCRC(SendBuff[messageStart]);
	for(i=messageStart+1; i < messageStart+HLHeaderSize + dataCount; i++)
	{
		CRC = UpdateCRC(CRC, SendBuff[i]);
	}
	
	//	Write the CRC into the Message
	SendBuff[(*ptrSendIndex)++] = (UInt8)(CRC >> 8);
	SendBuff[(*ptrSendIndex)++] = (UInt8)(CRC & 0xFF);
}


/***********************************************************************
 *
 * FUNCTION:	  MmDLFillPacketHeader
 *
 * DESCRIPTION:  Fill in the data packet header
 *
 * PARAMETERS:	
 *
 * RETURNED:	  Boolean
 *
 ***********************************************************************/
void MmFillMessageHeader(UInt16 hLCommandType, UInt16* ptrSendIndex, UInt16 statusError)
{
	SendBuff[(*ptrSendIndex)++] = (UInt8)(hLCommandType);
	SendBuff[(*ptrSendIndex)++] = (UInt8)(Prefs.transactionNo);
	SendBuff[(*ptrSendIndex)++] = (UInt8)(Prefs.infoSyncNo);
	SendBuff[(*ptrSendIndex)++] = (UInt8)(Prefs.menuSyncNo);
	SendBuff[(*ptrSendIndex)++] = (UInt8)(Prefs.drinkSyncNo);
	SendBuff[(*ptrSendIndex)++] = (UInt8)(Prefs.staffIDNo >> 8);
	SendBuff[(*ptrSendIndex)++] = (UInt8)(Prefs.staffIDNo & 0xFF);
	SendBuff[(*ptrSendIndex)++] = (UInt8)(statusError);
	SendBuff[(*ptrSendIndex)++] = (UInt8)(0);
	SendBuff[(*ptrSendIndex)++] = (UInt8)(0);
}



//////////////////////////////////////////////////////
/*
"[Entree]\n"
"1|Garlic Herbed Bread|300\n"
"2|Soup Du Jour|550\n"
"3|Spicy Lamb Salad|1100\n"
"4|Pumpkin and Kumara Balls|1000\n"
"5|Trio of Fettucine|1050\n"
"6|Deep-Fried Camembert|1000\n"
"{As Main|}\n"
"\n"
*/

//	Temporary test commands
#ifdef DebugHLFake
HLCommandType	HLCmdE0	 = {0,
									{0xE0, 21, 1, 2, 3, 0, 0, 0,
									0xFF	},
									TalkOkay};
HLCommandType	HLCmdE3	 = {0,
									{0xE3, 21, 1, 2, 3, 0, 0, 0,
									'[','E','n','t','r','e','e',']',0x0A,
									'1','|','G','a','r','l','i','c',' ','H','e','r','b','e','d',' ','B','r','e','a','d','|','3','0','0',0x0A,
									'{','A','s',' ','M','a','i','n','|','}',0x0A,
									0x0A,
									0xFF	},
									TalkOkay};
HLCommandType	HLCmdE4	 = {0,
									{0xE4, 21, 1, 2, 3, 0, 0, 0,
									0xFF	},
									TalkOkay};
HLCommandType *Commands[5] = {&HLCmdE0, &HLCmdE3, &HLCmdE4};
UInt16	CommandCounter = 0;
/***********************************************************************
 *
 * FUNCTION:	  MmBeamMessages   (Only when DebugHLFake enabled!!!!)
 *
 * DESCRIPTION:  
 *
 * PARAMETERS:	  Accepts messageLength as input and returns success status
 *						eg. TalkOkay, TalkFail
 *
 * RETURNED:	  
 *
 ***********************************************************************/
UInt16 MmBeamMessage(UInt16 messageLength)//   (Only when DebugHLFake enabled!!!!)
{
	UInt16					i;
	UInt16					CRC;
	HLCommandType			*ptrMessage;

	messageLength++;

	ptrMessage = Commands[CommandCounter++];
	
	for(i=0; ptrMessage->message[i] != 0xFF; i++)
	{
		ALReceivePtr[i] = ptrMessage->message[i];
	}
	ptrMessage->messageLength = i;
	ALReceivePtr[HLHdrByteCount] = (ptrMessage->messageLength - HLHeaderSize) >> 8;
	ALReceivePtr[HLHdrByteCount+1] = (ptrMessage->messageLength - HLHeaderSize) & 0xFF;
	
	CRC =  StartCRC(ALReceivePtr[0]);
	for(i=1; i < ptrMessage->messageLength; i++)
	{
		CRC = UpdateCRC(CRC, ALReceivePtr[i]);
	}
	
	ALReceivePtr[i] = CRC >> 8;
	ALReceivePtr[i+1] = CRC & 0xFF;

//	ALReceiveIndex = i+2;									//	Step over CRC
	return ptrMessage->returnValue;						//	Return status
}


#endif


/***********************************************************************
 *
 * FUNCTION:	  MmBeamMessage
 *
 * DESCRIPTION:  
 *
 * PARAMETERS:	  Accepts messageLength as input and returns success status
 *						eg. TalkOkay, TalkFail
 *
 * RETURNED:	  
 *
 ***********************************************************************/
static UInt16 MmBeamMessage(UInt8* ptrSendBuffer, UInt16 messageLength)
{
	UInt16				btTalkResult = TalkBusy;
	UInt16				i;


	for(i=0; i < BtMessageAttempts; i++)
	{
		btTalkResult = MmBtTalk(BtPortId, &Prefs, ptrSendBuffer, messageLength, ALReceivePtr);


		if(btTalkResult == TalkOkay			||
			btTalkResult == TalkNoConnect)
		{
			return btTalkResult;
		}
	}
	return btTalkResult;									//	Return status
}



/***********************************************************************
 *
 * FUNCTION:	  MmHLConfirmCRC
 *
 * DESCRIPTION:  Calculate a High Level layer CRC to confirm received message.
 *
 * PARAMETERS:	
 *
 * RETURNED:	  
 *
 ***********************************************************************/
 UInt16 MmHLConfirmCRC(UInt16 dataCount, UInt16 messageIndex)
 {
	UInt16						i;
	UInt16 						SentCRC = 0;
	UInt16						CalcCRC;

	CalcCRC =  StartCRC(ALReceivePtr[messageIndex]);
	for(i=messageIndex+1; i < messageIndex + HLHeaderSize + dataCount; i++)
	{
		CalcCRC = UpdateCRC(CalcCRC, ALReceivePtr[i]);
	}
	SentCRC = ALReceivePtr[i] * 256;
	i++;
	SentCRC += ALReceivePtr[i];
	if(SentCRC != CalcCRC)
	{
		if(SentCRC == CalcCRC + 1)
			return 1;						//	Special Case. Version number was wrong
		else
			return NoValue16;				//	Its broke.
	}
	
	return 0;								//	No it ain't.
}


/***********************************************************************
 *
 * FUNCTION:	  MmMarkSentAsConfirmed
 *
 * DESCRIPTION:  Marks all records currently set as 'Sent' to 'Confirmed'.
 *
 * PARAMETERS:	
 *
 * RETURNED:	  
 *
 ***********************************************************************/
void MmMarkSentAsConfirmed(void)
{
	UInt16					numberOfRecords;
	MemHandle				recHandle;
	UInt16					recIndex;
	OrderTypePtr			recPtr;
//	SeatnameTypePtr		recSeatnamePtr;
	UInt16					status;
	Err						err;

	UInt16					recordsDeleted;
	

	Prefs.numUnsentOrders = 0;
	NumOldUnsentOrders = 0;			//	Used to prevent old orders being sent (especially by a new user)

	//	Step through the DB, setting 'Confirmed' status for 'Sent' records
	//	and resetting the StatusAsHasSeatnameToSend bit in the status UInt8
	numberOfRecords = DmNumRecords(MmOrdersDB);
	for(recIndex=0; recIndex < numberOfRecords; recIndex++)
	{
		recHandle = DmGetRecord(MmOrdersDB, recIndex);
							
		// If the record was found, check for unConfirmed sent order
		if(recHandle)
		{
			recPtr = MemHandleLock(recHandle);

			//	If this record is a 'Credit' order, it should be removed
			if(recPtr->status & StatusAsCreditDish)
			{
				MemPtrUnlock(recPtr);
				err = DmReleaseRecord(MmOrdersDB, recIndex, true);
				ErrFatalDisplayIf(err, "Could not release record after marking as Confirmed.");

				err = MmDeleteCurrentRecord(MmOrdersDB, WithMarker, recIndex,
														&Prefs.numUnsentOrders, &recordsDeleted);
				ErrFatalDisplayIf(err, "Could not remove Current Record as Credit");

				numberOfRecords -= recordsDeleted;	//	Got fewer records than we had
				recIndex -= recordsDeleted;			//	If Marker died then back up extra one
			}

			//	else if record is sent, but not confirmed (no use re-confirming again)
			else if(	(recPtr->status & StatusAsSent)		 &&
						((recPtr->status & StatusAsSentConfirmed) == 0))
			{
				//	Mark this record as Confirmed as sent
				status = recPtr->status | StatusAsSentConfirmed;	//	Set the Confirmed status bit
				err = DmWrite(recPtr, (Char*)&recPtr->status - (Char*)&recPtr->tableNumber,
										&status, sizeof(recPtr->status));

				MemPtrUnlock(recPtr);
				err = DmReleaseRecord(MmOrdersDB, recIndex, true);
				ErrFatalDisplayIf(err, "Could not release record after marking as Confirmed.");
			}

			else
			{
				MemPtrUnlock(recPtr);
				err = DmReleaseRecord(MmOrdersDB, recIndex, true);
				ErrFatalDisplayIf(err, "Could not release record in MmMarkSentAsConfirmed.");
			}
		}
	}
}


/***********************************************************************
 *
 * FUNCTION:	  MmProcessMessages
 *
 * DESCRIPTION:  Write data packets to the Ir port through the	Datalink layer. 
 *						Connect if necessary.
 *
 *						Every full transaction involves 2 request/response pairs
 *
 * PARAMETERS:	
 *
 * RETURNED:	  Boolean
 *
 ***********************************************************************/
static void MmProcessMessages(UInt16 *processError)
{
	FormPtr				frm;

	UInt16				hLThisCommand;
	UInt16				HLThisResponse;
	UInt16				statusError = 0;
	UInt16				dataCount = 0;
	UInt16				dataCountTotal = 0;
	UInt16				sendIndex = 0;
	UInt16				nullSendIndex = 0;

	UInt8					*HLStatusPtr;

	UInt8					*beamMessagesDonePtr;

	Boolean				finished = false;
	Boolean				resendingMessage = false;
	UInt16				nullCounter = 0;

	UInt16				talkResult;
	UInt16				messageLength;
	UInt16				messageStart;
	UInt16				i, j;
	UInt8					errorCode = 0;
	UInt16				error = 0;
	UInt16				asmErr = 0;
	
	UInt16				userSettingsUInt16;

	UInt8					*ptrMessage;

	UInt16				warningCount = 0;

#ifdef DebugTiming2
	UInt32				startTick, endTick;
	UInt16				diffTicks;
#endif

	
	
	//	Reset all the BeamMessagesDone status flags to HLStatusAllDone
	for(i=0, beamMessagesDonePtr = &BeamMessagesDone.HLRegisterStatus;
													i < HLEnd - HLRegister; i++, beamMessagesDonePtr++)
	{
		*beamMessagesDonePtr = HLStatusAllDone;	//	Initialise the progress counter
	}
	

	//	If the Palm has become de-registered, force a registration attempt using existing conditions.
	//	If the Prefs have been blown away (eg. flat battery), force a registration attempt.
	//	If this Palm is already in the DB, PalmMate will register it and just carry on.
	if(Prefs.palmUniqueID == 0		&& 	!RegTemp.registering)
	{
		RegTemp.autoReRegister = true;
	}

	if(RegTemp.autoReRegister	&&	!RegTemp.forceInfoSync)
		Prefs.hLStatus.HLRegisterStatus = HLStatusToSend;



	//	Always do a GetAllOrders
	Prefs.hLStatus.HLGetAllOrdersStatus = HLStatusToSend;


	
	if(MmCallAwayRecordExists(&MmMiscDataDB) != NoValue16)
		Prefs.hLStatus.HLCallAwayStatus = HLStatusToSend;
	else
		Prefs.hLStatus.HLCallAwayStatus = HLStatusAllDone;


	if(Prefs.numUnsentOrders == 0)
	{
		Prefs.hLStatus.HLSendOrdersStatus = HLStatusAllDone;
	}
	else if(Prefs.numUnsentOrders != NoValue16 && Prefs.numUnsentOrders != 0)
	{
		Prefs.hLStatus.HLSendOrdersStatus = HLStatusToSend;
	}


	if(MmBillingOperations(BillingActionFindFirstInDB, MmOrdersDB, &Prefs.current) != NoValue16)
		Prefs.hLStatus.HLBillSeatsStatus = HLStatusToSend;


	//	Keep doing until all requests and responses are done (or timeout)
	while(finished == false)
	{
		//	This whole section is skipped on resend (the message is already in the buffer)
		if(!resendingMessage)
		{
			for(i=0; i < MySendBuffSize + MySendBuffMargin; i++)	//	Ensure Buffer empty
				SendBuff[i] = (UInt8)(0);
	
//MmPrvSyncLogTrace("\nReg Inf Mn Dk Lgn Bt Od CA BS Tf GO\n");
	
			//	If any transactions crapped out last time, resend now (with old Prefs.transactionNo)
			for(i=HLRegister, HLStatusPtr = &Prefs.hLStatus.HLRegisterStatus;
						i < HLEnd; i++, HLStatusPtr++)
			{
				if((*HLStatusPtr == HLStatusReadyToSend) ||
					(*HLStatusPtr == HLStatusIsSent)				)	//	If it never advanced past IsSent
				{
					*HLStatusPtr = HLStatusToSend;					//	... then reset it
				}
//MmPrvSyncLogTraceUI("__%d", *HLStatusPtr);	//	Display all the flags
			}
	
			//	Work through the commands in sequence, checking if any need processing
			for(hLThisCommand = HLRegister, HLStatusPtr = &Prefs.hLStatus.HLRegisterStatus;
					hLThisCommand < HLEnd;		hLThisCommand++, HLStatusPtr++)
			{
				if(*HLStatusPtr == HLStatusToSend)
				{
					//	Don't process these if we're not logged in
					if(	(!Prefs.loggedIn && (hLThisCommand == HLSendOrders))	||
							(!Prefs.loggedIn && (hLThisCommand == HLGetAllOrders))	)
					{
						continue;
					}
					else
					{
						break;									//	Ah. Here's some work
					}
				}
			}
				
			if(hLThisCommand == HLEnd)
			{
//MmPrvSyncLogTrace("\nAll Done");
				break;					//	Nothing left to do so break out of outer while()
			}


			ptrMessage = SendBuff;							//	This is the default buffer

			sendIndex = 0;
			sendIndex += 15/*sizeof(BtMessageHeader)*/;	//	Leave room for our msg header
	
			messageStart = sendIndex;
	
			//	Set up the next message to go out
			switch(hLThisCommand)
			{
				case HLRegister:
	
					RegTemp.regSuccessful = false;

					MmFillMessageHeader(hLThisCommand, &sendIndex, statusError);
					
					MmAssemblePalmUniqueIDMessage(&sendIndex, &SendBuff[0], &dataCount);
					dataCountTotal = dataCount;

					//	Also, RegistrationMessage.
					//	Try to re-register using existing conditions (in case battery just went flat)
					MmAssemblePalmRegistrationMessage(&sendIndex, &SendBuff[0], &dataCount);
					dataCountTotal += dataCount;

					MmHLCalcCRC(messageStart, dataCountTotal, &sendIndex);
					*HLStatusPtr = HLStatusReadyToSend;
					BeamMessagesDone.HLRegisterStatus	= HLStatusReadyToSend;
					break;

					
				case HLInfoSync:
	
					MmFillMessageHeader(hLThisCommand, &sendIndex, statusError);
					MmHLCalcCRC(messageStart, 0, &sendIndex);
					*HLStatusPtr = HLStatusReadyToSend;
					BeamMessagesDone.HLInfoSyncStatus	= HLStatusReadyToSend;
					break;

					
				case HLMenuSync:
	
					MmFillMessageHeader(hLThisCommand, &sendIndex, statusError);
					MmHLCalcCRC(messageStart, 0, &sendIndex);
					*HLStatusPtr = HLStatusReadyToSend;
					BeamMessagesDone.HLMenuSyncStatus	= HLStatusReadyToSend;
					break;

					
				case HLDrinkSync:

					MmFillMessageHeader(hLThisCommand, &sendIndex, statusError);
					MmHLCalcCRC(messageStart, 0, &sendIndex);
					*HLStatusPtr = HLStatusReadyToSend;
					BeamMessagesDone.HLDrinkSyncStatus	= HLStatusReadyToSend;
					break;

					
				case HLLogin:
	
					MmFillMessageHeader(hLThisCommand, &sendIndex, statusError);
					MmAssemblePalmUniqueIDMessage(&sendIndex, &SendBuff[0], &dataCount);
					dataCountTotal = dataCount;
					MmAssembleLoginMessage(&sendIndex, &SendBuff[0], &dataCount);
					dataCountTotal += dataCount;
					MmHLCalcCRC(messageStart, dataCountTotal, &sendIndex);
					*HLStatusPtr = HLStatusReadyToSend;
					BeamMessagesDone.HLLoginStatus	= HLStatusReadyToSend;
					break;

					
				case HLSendSeatnames:
	
					MmFillMessageHeader(hLThisCommand, &sendIndex, statusError);
					MmAssembleSendSeatnamesMessage(&sendIndex, &dataCount);
					MmHLCalcCRC(messageStart, dataCount, &sendIndex);
					*HLStatusPtr = HLStatusReadyToSend;
					BeamMessagesDone.HLSendSeatnamesStatus	= HLStatusReadyToSend;
					break;

	
				case HLBtTimeout:	//	Request btTimeout
	
					MmFillMessageHeader(hLThisCommand, &sendIndex, statusError);
					MmHLCalcCRC(messageStart, 0, &sendIndex);
					*HLStatusPtr = HLStatusReadyToSend;
					BeamMessagesDone.HLBtTimeoutStatus	= HLStatusReadyToSend;
					break;

					
				case HLSendOrders:
	
					MmFillMessageHeader(hLThisCommand, &sendIndex, statusError);
					asmErr = MmAssembleOrdersMessage(&sendIndex, &dataCount, IsSendOrders);
	
					if(asmErr == HLAssembleMessageOkay || asmErr == HLAssembleOrdersMoreToCome)
					{
						MmHLCalcCRC(messageStart, dataCount, &sendIndex);
						*HLStatusPtr = HLStatusReadyToSend;
					}
					else if(asmErr == HLAssembleOrdersTooMany)
					{
						ErrThrow(HLAssembleOrdersTooMany);
					}
					BeamMessagesDone.HLSendOrdersStatus	= HLStatusReadyToSend;
					break;

	
				case HLCallAway:
	
					MmFillMessageHeader(hLThisCommand, &sendIndex, statusError);
					MmAssembleCallAwayMessage(&sendIndex, &dataCount);
					MmHLCalcCRC(messageStart, dataCount, &sendIndex);
					*HLStatusPtr = HLStatusReadyToSend;
					BeamMessagesDone.HLCallAwayStatus	= HLStatusReadyToSend;
					break;
	
	
				case HLBillSeats:
	
					MmFillMessageHeader(hLThisCommand, &sendIndex, statusError);
					MmAssembleBillingMessage(&sendIndex, &dataCount);
					MmHLCalcCRC(messageStart, dataCount, &sendIndex);
					*HLStatusPtr = HLStatusReadyToSend;
					BeamMessagesDone.HLBillSeatsStatus	= HLStatusReadyToSend;
					break;

	
				case HLTransferTables:
					BeamMessagesDone.HLTransferTablesStatus	= HLStatusReadyToSend;
					break;

	
				case HLGetAllOrders:
	
					MmFillMessageHeader(hLThisCommand, &sendIndex, statusError);
	
					//	If Prefs.userSettings.getAllTables == true => orders for All tables are retrieved
					//	else only the current Table is returned
					//	Prefs.current.tableNo ensures that Current Table orders are always got
					SendBuff[sendIndex] = (UInt8)(Prefs.current.tableNo);
					sendIndex++;
	

					//	Send User Settings to POS in case they've changed, (we get them back during a login)
					userSettingsUInt16 = MmUserSettingsToUInt16(&Prefs);
					
					SendBuff[sendIndex] = (UInt8)(userSettingsUInt16 >> 8);	//	High byte
					sendIndex++;
	
					SendBuff[sendIndex] = (UInt8)(userSettingsUInt16 & 0xFF);
					sendIndex++;

					MmHLCalcCRC(messageStart, 3, &sendIndex);
	
					*HLStatusPtr = HLStatusReadyToSend;
					BeamMessagesDone.HLGetAllOrdersStatus	= HLStatusReadyToSend;
					break;
			}
		}	//	if(!resendingMessage)	ie. This whole section was skipped on resend
			//											(message already in buffer)
	
		resendingMessage = false;						//	Make sure we resend only once

//ShowDebug("Command %x", hLThisCommand);		
/*
switch(hLThisCommand)
{
	case HLRegister:			ShowMessage("Command %x: Register",			hLThisCommand);	break;
	case HLInfoSync:			ShowMessage("Command %x: InfoSync",			hLThisCommand);	break;
	case HLMenuSync:			ShowMessage("Command %x: MenuSync",			hLThisCommand);	break;
	case HLDrinkSync:			ShowMessage("Command %x: DrinkSync",		hLThisCommand);	break;
	case HLLogin:				ShowMessage("Command %x: Login",				hLThisCommand);	break;
	case HLSendSeatnames:	ShowMessage("Command %x: Seatnames",		hLThisCommand);	break;
	case HLBtTimeout:			ShowMessage("Command %x: BtTimeout",		hLThisCommand);	break;
	case HLSendOrders:		ShowMessage("Command %x: SendOrders",		hLThisCommand);	break;
	case HLCallAway:			ShowMessage("Command %x: CallAway",			hLThisCommand);	break;
	case HLBillSeats:			ShowMessage("Command %x: BillSeats",		hLThisCommand);	break;
	case HLTransferTables:	ShowMessage("Command %x: TransferTables",	hLThisCommand);	break;
	case HLGetAllOrders:		ShowMessage("Command %x: GetAllOrders",	hLThisCommand);	break;
}
*/		

		//
		//
		//	Send the Message, from SendBuff[0] to SendBuff[sendIndex]
		//
		//


		talkResult = MmBeamMessage(ptrMessage, sendIndex);
//ShowDebug("talkResult = %x", talkResult);

		






		if(talkResult == TalkOkay)						//	If its not TalkOkay, its busted
		{
/*ShowMessage("%x %x\n%x %x\n%x %x\n%x %x\n%x %x\n%x %x\n%x %x\n%x %x\n%x %x\n%x %x",
								ALReceivePtr[0], ReceiveBuffTemp[0],
								ALReceivePtr[1], ReceiveBuffTemp[1],
								ALReceivePtr[2], ReceiveBuffTemp[2],
								ALReceivePtr[3], ReceiveBuffTemp[3],
								ALReceivePtr[4], ReceiveBuffTemp[4],
								ALReceivePtr[5], ReceiveBuffTemp[5],
								ALReceivePtr[6], ReceiveBuffTemp[6],
								ALReceivePtr[7], ReceiveBuffTemp[7],
								ALReceivePtr[8], ReceiveBuffTemp[8],
								ALReceivePtr[9], ReceiveBuffTemp[9]
																				);
*/
			if(ALReceivePtr[0] != 0)
			{
				error = MmHLValidateMessage(&Prefs, ALReceivePtr, &HLThisResponse, 0, &messageLength, &errorCode);

//ShowDebug("Response = %x\nError = %u\nError Code = %u", HLThisResponse, error, errorCode);
/*
switch(HLThisResponse)
{
	case HLRegister:			ShowMessage("Response: Register\nError = %d",		error);	break;
	case HLInfoSync:			ShowMessage("Response: InfoSync\nError = %d",		error);	break;
	case HLMenuSync:			ShowMessage("Response: MenuSync\nError = %d",		error);	break;
	case HLDrinkSync:			ShowMessage("Response: DrinkSync\nError = %d",		error);	break;
	case HLLogin:				ShowMessage("Response: Login\nError = %d",			error);	break;
	case HLSendSeatnames:	ShowMessage("Response: Seatnames\nError = %d",		error);	break;
	case HLBtTimeout:			ShowMessage("Response: BtTimeout\nError = %d",		error);	break;
	case HLSendOrders:		ShowMessage("Response: SendOrders\nError = %d",		error);	break;
	case HLCallAway:			ShowMessage("Response: CallAway\nError = %d",		error);	break;
	case HLBillSeats:			ShowMessage("Response: BillSeats\nError = %d",		error);	break;
	case HLTransferTables:	ShowMessage("Response: TransferTables\nError = %d",error);	break;
	case HLGetAllOrders:		ShowMessage("Response: GetAllOrders\nError = %d",	error);	break;
}
*/
				if(!error)		//	0 = no errors
				{
					//	If response code is good process the response, else deal with the wreckage
					if(errorCode > HLWarnWarning)
					{
						switch(HLThisResponse)
						{
							case HLRegister:
								//	Registered, so clear RegTemp
								RegTemp.registering			= false;	//	All over; Go back to normal form
								RegTemp.regSuccessful		= true;	//	Display "Successful" below
								RegTemp.forceInfoSync		= false;
								RegTemp.palmListChanged		= false;
								RegTemp.uniqueID				= 0;
								RegTemp.flags					= 0;
								RegTemp.syncPrintingToID	= 0;

								MmParseRegistrationMessage(messageLength);

								StrCopy(RegTemp.newName, "");
								StrCopy(RegTemp.syncPrintingToName, "");

								Prefs.hLStatus.HLInfoSyncStatus			= HLStatusToSend;	//	Refresh InfoSync list
								Prefs.hLStatus.HLRegisterStatus			= HLStatusAllDone;
								BeamMessagesDone.HLRegisterStatus		= HLStatusIsSent;

								break;

								
							case HLInfoSync:
								MmParseInfoSyncMessage(messageLength);
								if(RegTemp.forceInfoSync == true)
								{
									RegTemp.forceInfoSync = false;
									RegTemp.palmListChanged = true;
									MmResetHLStatusToAllDone(&Prefs, KeepNone);	//	Reset the lot
								}

								Prefs.loggedIn = false;
								Prefs.hLStatus.HLInfoSyncStatus			= HLStatusAllDone;
								BeamMessagesDone.HLInfoSyncStatus		= HLStatusIsSent;

								break;

								
							case HLMenuSync:
								if(errorCode == HLWarnNoMenuFound)
								{
									//	Save the warning into the array for display when we return
									processError[warningCount] = HLErrMessNoDishesMenuFound;
									
									if(warningCount < MaxProcessErrorValues - 1)
										warningCount++;
								}

								MmParseMenuSyncMessage(messageLength, DishesMenu);
								MmResetHLStatusToAllDone(&Prefs, KeepInfoSync | KeepDrinkSync |
																								KeepLogin);

								Prefs.menuSyncNo = ALReceivePtr[HLHdrMenuSync];
							
								MmSavePrefs();				//	Save these now in case we break
							
								Prefs.hLStatus.HLGetAllOrdersStatus		= HLStatusToSend;
								BeamMessagesDone.HLMenuSyncStatus		= HLStatusIsSent;
								break;

								
							case HLDrinkSync:
								if(errorCode == HLWarnNoMenuFound)
								{
									//	Save the warning into the array for display when we return
									processError[warningCount] = HLErrMessNoDrinksMenuFound;
									
									if(warningCount < MaxProcessErrorValues - 1)
										warningCount++;
								}
	
//ShowDebug("Command %x", hLThisCommand);

								MmParseMenuSyncMessage(messageLength, DrinksMenu);
								MmResetHLStatusToAllDone(&Prefs, KeepInfoSync | KeepLogin);

								Prefs.drinkSyncNo = ALReceivePtr[HLHdrDrinkSync];
							
								MmSavePrefs();				//	Save these now in case we break
							
								Prefs.hLStatus.HLGetAllOrdersStatus		= HLStatusToSend;
								BeamMessagesDone.HLDrinkSyncStatus		= HLStatusIsSent;
								break;

								
							case HLLogin:
								//	Copy the Staff Name initial to the Label
								MmParseLoginMessage(messageLength);
								Prefs.loggedIn = true;

								MmGetStaffID(StaffType, &Prefs.staffIDNo, Prefs.staffIDIndex, MmInfoDB);	//	This is for real now
								
								if(Prefs.userLogOffMins)							//	Prefs.userLogOffMins = 0 => forever
									Prefs.autoLogoutSecs = TimGetSeconds() + (Prefs.userLogOffMins * 60);
								else
									Prefs.autoLogoutSecs = NoValue32;
								MmSavePrefs();					//	Save this now in case we break

//								*delayScreenLock = true;
						
								StrCopy(Prefs.pIN, "PIN#");//	Blitz Prefs.pIN for next time
						
								Prefs.hLStatus.HLLoginStatus				= HLStatusAllDone;
								BeamMessagesDone.HLLoginStatus			= HLStatusIsSent;
								break;

								
							case HLSendSeatnames:
								Prefs.hLStatus.HLSendSeatnamesStatus	= HLStatusAllDone;
								BeamMessagesDone.HLSendSeatnamesStatus	= HLStatusIsSent;
								break;

	
							case HLBtTimeout:
								//	We were just requesting a btTimeout, which has been
								//	sorted in MmReceiveMessage()
								Prefs.hLStatus.HLBtTimeoutStatus			= HLStatusAllDone;
								BeamMessagesDone.HLBtTimeoutStatus		= HLStatusIsSent;
								break;

							
							case HLSendOrders:
								Prefs.transactionNo++;		//	Successful transaction, so increment
						
								MmMarkSentAsConfirmed();

								Prefs.hLStatus.HLSendOrdersStatus		= HLStatusAllDone;
								BeamMessagesDone.HLSendOrdersStatus		= HLStatusIsSent;
								
								//	Also, trash Timeout request if Send Orders successful
								//	(may have been left in buffer from a previous beam attempt)
								Prefs.hLStatus.HLBtTimeoutStatus			= HLStatusAllDone;
								BeamMessagesDone.HLBtTimeoutStatus		= HLStatusIsSent;
								break;

			
							case HLCallAway:
								//	Trash all CallAway records
								MmDeleteAllTypeRecordsForMiscDataType(MmMiscDataDB, CallAwayType);

								Prefs.hLStatus.HLCallAwayStatus			= HLStatusAllDone;
								BeamMessagesDone.HLCallAwayStatus		= HLStatusIsSent;
								break;

				
							case HLBillSeats:
								MmBillingOperations(BillingActionConfirmAll, MmOrdersDB, &Prefs.current);
								
								Prefs.hLStatus.HLBillSeatsStatus			= HLStatusAllDone;
								BeamMessagesDone.HLBillSeatsStatus		= HLStatusIsSent;
								break;

				
							case HLTransferTables:
								Prefs.hLStatus.HLTransferTablesStatus	= HLStatusAllDone;
								BeamMessagesDone.HLTransferTablesStatus	= HLStatusIsSent;
								break;

	
							case HLGetAllOrders:
								if((ALReceivePtr[HLHdrByteCount] != 0) || (ALReceivePtr[HLHdrByteCount+1] != 0))
								{
									//	First shut down Serial Port to save batteries
									MmCloseBtPort(&BtPortId);

									frm = FrmGetActiveForm();
									MmPlaySounds(thinkingSound);

									MmZapDB(&MmOrdersDB, MmOrdersDBName, RecreateDB, &DBDeleteBitMask, MmOrdersMaskBit);

									if(MmParseGetAllOrdersMessage(messageLength))
									{
										FrmAlert(WarnUserCancelledAlert);
									}
									else
									{
										MmResetHLStatusToAllDone(&Prefs, KeepNone);	//	Reset the lot
									}
								}
								Prefs.numUnsentOrders = 0;					//	Reset in case no orders were sent

								Prefs.hLStatus.HLGetAllOrdersStatus			= HLStatusAllDone;
								BeamMessagesDone.HLGetAllOrdersStatus		= HLStatusIsSent;
								break;

			
							default:
								ErrThrow(HLErrMessUnknownResponseType);
						}	//	switch()



						//	Deal with any Warnings
						if((errorCode > HLWarnWarning) && (errorCode < HLErrResponseOkay))
						{
							switch(errorCode)
							{
								case HLWarnPrintingRedirected:
								case HLWarnPrinterOffline:
								case HLWarnCallAwayPrinterNotSetUp:
								case HLWarnKitchenPrinterNotSetUp:


									//	Save the warning into the array for display when we return
									processError[warningCount] = errorCode;

									if(warningCount < MaxProcessErrorValues - 1)
										warningCount++;
									

//									if(HLThisResponse == HLSendOrders)
//									{
//										MmMarkSentAsConfirmed();
//										Prefs.hLStatus.HLSendOrdersStatus	= HLStatusAllDone;
//									}
									//	If we don't reset the status it will keep sending it
									HLStatusPtr = &Prefs.hLStatus.HLRegisterStatus + (HLThisResponse - HLRegister);
									*HLStatusPtr = HLStatusAllDone;
									break;
									

								case HLWarnDuplicateOrder:
									break;
									
							}	//	switch(errorCode)
						}	//	if(errorCode > HLErrNonFatal)
					}	//	if(errorCode == 0 || errorCode > HLErrNonFatal)
					
					//	The returned errorCode shows an error. Respond appropriately
					else
					{
						if(errorCode != HLErrWrongVersionNum	&&	HLThisResponse == HLSendOrders)
						{
//ShowDebug("errorCode Mark Orders Unsent");
							MmProcessAllOrdersSentStatus(UnsendSentButNotConfirmed, MmOrdersDB, &Prefs);	//	Set as Unsent
						}

						switch(errorCode)
						{
							case HLErrDisplayErrorMsg:				//	Generic message

								messageLength -= 2;					//	Don't attempt to Parse the CRC!

								i = HLHeaderSize;

								//	Copy the message string
								j = 0;
								while(i<messageLength && j < MaxNoteLength)
								{
									TempStr[j++] = ALReceivePtr[i++];	//	Copy message
								}
								TempStr[j] = 0;								//	Terminate the string

								ErrThrow(HLErrMessDisplayErrorMsg);
								break;
	
							case HLErrInvalidMessageCRC:			//	My message to host was corrupt
								ErrThrow(HLErrMessInvalidMessageHostCRC);
								break;
	
							case HLErrCorruptMessage:
								ErrThrow(HLErrMessCorruptMessage);
								break;
	
							case HLErrMismatchMessageData:		//	Usually because no data sent
								//	If we don't reset the status it will keep sending it
								HLStatusPtr = &Prefs.hLStatus.HLRegisterStatus + (HLThisResponse - HLRegister);
								*HLStatusPtr = HLStatusAllDone;
								ErrThrow(HLErrMessMismatchMessageData);
								break;
	
							case HLErrInfoNotSyncError:
								//	If we are thrown out due to the Palm names being wrong
								//	reset everything and force an InfoSync, then kick back to Reg form
								if(hLThisCommand == HLRegister)
								{
									MmResetHLStatusToAllDone(&Prefs, KeepNone);	//	Reset the lot

									RegTemp.registering			= true;	//	Return straight to Reg form
									RegTemp.regSuccessful		= false;	//	Dont't display "Successful"
									RegTemp.forceInfoSync		= true;	//	Force InfoSync
									RegTemp.palmListChanged		= false;
									RegTemp.uniqueID				= 0;
									RegTemp.flags					= 0;
									RegTemp.syncPrintingToID	= 0;
									StrCopy(RegTemp.newName, "");
									StrCopy(RegTemp.syncPrintingToName, "");

									StrCopy(Prefs.palmRegistrationName, "");
								}
								Prefs.hLStatus.HLInfoSyncStatus = HLStatusToSend;
								break;
	
							case HLErrDishesNotSyncError:
								MmResetHLStatusToAllDone(&Prefs, KeepInfoSync | KeepDrinkSync | KeepLogin);
								Prefs.hLStatus.HLMenuSyncStatus	= HLStatusToSend;
								ErrThrow(HLErrMessDishesNotSyncError);
								break;
	
							case HLErrDrinksNotSyncError:
								MmResetHLStatusToAllDone(&Prefs, KeepInfoSync | KeepLogin);
								Prefs.hLStatus.HLDrinkSyncStatus = HLStatusToSend;
								ErrThrow(HLErrMessDrinksNotSyncError);
								break;
	
							case HLErrServerDBError:
								MmResetHLStatusToAllDone(&Prefs, KeepNone);		//	Reset the lot
								Prefs.hLStatus.HLMenuSyncStatus = HLStatusToSend;
								Prefs.hLStatus.HLDrinkSyncStatus = HLStatusToSend;
								ErrThrow(HLErrMessServerDBError);
								break;
	
							case HLErrPalmIDNotFound:
								Prefs.palmUniqueID = 0;				//
								MmResetHLStatusToAllDone(&Prefs, KeepNone);	//	Reset all Prefs.hLStatus flags
								ErrThrow(HLErrMessPalmIDNotFound);
								break;
	
							case HLErrNotLoggedIn:
							case HLErrWrongPIN:
							case HLErrUserNotFound:
							case HLErrMaxUsersReached:
								Prefs.staffIDNo = 0;					//
								Prefs.loggedIn = false;
								Prefs.hLStatus.HLLoginStatus = HLStatusAllDone;
								switch(errorCode)
								{
									case HLErrNotLoggedIn:
										ErrThrow(HLErrMessNotLoggedIn);
										break;
		
									case HLErrWrongPIN:
										ErrThrow(HLErrMessWrongPIN);
										break;

									case HLErrUserNotFound:
										ErrThrow(HLErrMessUserNotFound);
										break;

									case HLErrMaxUsersReached:
										MmResetHLStatusToAllDone(&Prefs, KeepNone);
										ErrThrow(HLErrMessMaxUsersReached);
										break;

								}

							case HLErrWrongRegistrationPIN:

 								ErrThrow(HLErrMessWrongRegistrationPIN);
								break;


							case HLErrNotInRegistrationMode:

								if(RegTemp.autoReRegister)
								{
									RegTemp.autoReRegister = false;
									RegTemp.registering = true;
									
									ErrThrow(HLAssemblePalmRegNoRegName);
								}
								else
								{
									ErrThrow(HLErrMessNotInRegistrationMode);
								}
								break;


							case HLErrNoPrinterFound:
								//	Make sure it happens again when things are sorted out
								*HLStatusPtr = HLStatusToSend;
								ErrThrow(HLErrMessNoPrinterFound);	//	No printer was found
								break;
	

							case HLErrWrongVersionNum:
								MmResetHLStatusToAllDone(&Prefs, KeepNone);
								ErrThrow(HLErrMessWrongVersionNum);
								break;
	

							default:
								ErrThrow(HLErrMessUnspecifiedError);
								break;
						}	//	switch(errorCode)
					}
				}	//	if(!error)
				else
				{
					if(HLThisResponse == HLSendOrders)
					{
//ShowDebug("error Mark Orders Unsent");
						MmProcessAllOrdersSentStatus(UnsendSentButNotConfirmed, MmOrdersDB, &Prefs);	//	Set as Unsent
					}

					if(error == HLErrMessInvalidMessageNumber)
					{
						ErrThrow(HLErrMessInvalidMessageNumber);	//	Not a valid Message Command
					}
					else
					{
						ErrThrow(HLErrMessInvalidMessagePalmCRC);	//	Message from host was corrupt
					}
				}
			}	//	ALReceivePtr[0] != 0
			else
			{
				ErrThrow(HLErrMessInvalidMessageNumber);			//	Need a valid message id
			}
		}	//	if(talkResult == TalkOkay)


		else	//	talkResult != TalkOkay
		{
			if(hLThisCommand == HLSendOrders)					//	We were trying to Send Orders
			{
//ShowDebug("talkResult Mark Orders Unsent");
				MmProcessAllOrdersSentStatus(UnsendSentButNotConfirmed, MmOrdersDB, &Prefs);	//	Mark all unconfirmed as Unsent
			}


			*HLStatusPtr = HLStatusToSend;						//	Rewind for resend



			if(talkResult == TalkTooLarge)
			{
				ErrThrow(HLErrMessBeamingTooLarge);				//	Report crapout.
			}

			else if(talkResult == TalkWrongPalm)
			{
//ShowDebug("talkResult = TalkWrongPalm");
				ErrThrow(HLErrMessBeamingFailure);				//	Report crapout.
			}

			else
			{
				//	Send btTimeout request message if it was a Send or GetOrders that failed
				if(hLThisCommand == HLSendOrders || hLThisCommand == HLGetAllOrders)
				{
					Prefs.hLStatus.HLBtTimeoutStatus = HLStatusToSend;
				}
				ErrThrow(HLErrMessBeamingFailure);				//	Report crapout.
			}
		}	//	else

		

		for(i=HLRegister, HLStatusPtr = &Prefs.hLStatus.HLRegisterStatus;
					i < HLEnd; i++, HLStatusPtr++)
		{
			//	Not all messages will always be sent on first attempt
			if(*HLStatusPtr != HLStatusAllDone)
				finished = false;									//	We can't go yet.

		}


		//	If there were more Orders to be sent
		if(asmErr == HLAssembleOrdersMoreToCome)		//	Ensure more orders are sent next loop
		{
			Prefs.hLStatus.HLSendOrdersStatus = HLStatusToSend;
		}
	}	//	Outer while().  Keep doing until all requests and responses are done (or timeout)



/*
	if(printingWarning)
		return printingWarning;

	else if(warningNoDishesMenuAvailable)
		return HLErrMessNoDishesMenuFound;

	else if(warningNoDrinksMenuAvailable)
		return HLErrMessNoDrinksMenuFound;

	else
		return 0;											//	no errors
*/
}


/***********************************************************************
 *
 * FUNCTION:    MmBeamingFormHandleEvent
 *
 * DESCRIPTION: Handles processing of events for the Beaming form.
 *
 * PARAMETERS:  event - the most recent event.
 *
 * RETURNED:    True if the event is handled, false otherwise.
 *
 ***********************************************************************/
Boolean MmBeamingFormHandleEvent(EventPtr event)
{
	FormPtr				frm;
	Boolean				handled = false;
	UInt16				eventType;
	
	UInt16				currentTableNumber;
	UInt16				currentSeatNumber;
	
	UInt16				i;
	
	UInt16				processError[MaxProcessErrorValues];//	Return variable for MmProcessMessages()
	
	
#ifdef ReleaseVersion
	UInt16				btserialOpenAttempts;
	UInt16				serialOpenAttemptsCount;
#endif


#ifndef DebugFakeMenu
#ifndef StandAlonePalm
	Err					err;					//	Serial errors variable
	Boolean				btOkay = false;
#endif
#endif


#ifdef DebugForceMessage
	UInt8					*ptrMessage;
	UInt8					nullMessage[HLHeaderSize+2];
	UInt16				sendIndex = 0;
	UInt8	fakeMessage[] =	{'~', 'H', 'i', ' ', 'M', 'i', 'k', 'e'};
#endif


#ifdef DebugFakeMenu
	UInt16				sendIndex = 0;
	UInt16				statusError = 0;
	UInt8	fakeHeader[] =	{HLSendOrders, '~', '~', '~', '~', '~', '~', '~', '~'};

#ifndef StandAlonePalm
	UInt16				dataCount;
	UInt16				dataCountTotal;
#else
//	UInt16				sectionFlagIndex;
#endif
	static Boolean		toggle = false;
#endif


//UInt16 gPortID;
	UInt32					flags = 0; 
	UInt16					flagsSize = sizeof(flags); 
	


	//	Preserve
	currentTableNumber	= Prefs.current.tableNo;
	currentSeatNumber		= Prefs.current.seatNo;


		
	eventType = event->eType;


	ErrTry
	{
		if(eventType == frmOpenEvent)
		{
			ALReceivePtr = 0;

			
			//	Warn that it may take some time to send if large number of orders
			if(Prefs.numUnsentOrders > 100)	//	Rather arbitrary number
			{
				UInt16	timeSecs;


				MmPlaySounds(errorSound);		

				timeSecs = (((UInt32)Prefs.numUnsentOrders * (UInt32)Prefs.btTimeoutSendOrders) / 1000) + 
																				((UInt32)BtSendOrdersTimeoutOffset/100);

				ShowMessage("%u orders to send\n\nEstimated Time:\n    %Lu mins, %Lu secs",
										Prefs.numUnsentOrders, (UInt32)(timeSecs/60), (UInt32)(timeSecs%60)	);
			}

	  		// The form was told to open.  It has already been loaded and activated.
			frm = FrmGetActiveForm();
			
			// Draw the Beaming form.
			FrmDrawForm(frm);
	

#ifndef DebugFakeMenu
#ifndef StandAlonePalm

			if(NewSerialManagerPresent)
			{
				//	Tidy Bluetooth port in case it broke last time
				if(BtPortId != NULL)
				{
					MmCloseBtPort(&BtPortId);
				}

				err = BtOpenCloseLib(&BtLibRefNum, BtOpenAndClose);




				//	Check whether the Palm local address is valid
				err = BtOpenCloseLib(&BtLibRefNum, BtOpenOnly);
/*
if(err)
{
	ShowMessage("BtLibOpen broke");
	ShowMessage("0 BtLibRefNum %x\nerr %x\n%hx:%hx:%hx:\n%hx:%hx:%hx",
					BtLibRefNum,
					err,
					Prefs.btPalmAddress.address[5],
					Prefs.btPalmAddress.address[4],
					Prefs.btPalmAddress.address[3],
					Prefs.btPalmAddress.address[2],
					Prefs.btPalmAddress.address[1],
					Prefs.btPalmAddress.address[0]);
}
*/
				//	Preserve the BtPalmAddress in a global
				err = BtLibGetGeneralPreference(BtLibRefNum, btLibPref_LocalDeviceAddress,
													&Prefs.btPalmAddress, sizeof(Prefs.btPalmAddress));
					
				
				if(err == btLibErrFailed)
				{
					ShowMessage("Bluetooth Error.\n\nPalm will now reset to clear the error.");
					SysReset();
				}

				err = BtOpenCloseLib(&BtLibRefNum, BtCloseOnly);

/*				addressGood = false;
				for(i=0; i<6; i++)
				{
					if(Prefs.btPalmAddress.address[i] != 0)
					{
						addressGood = true;
						break;
					}
				}

				if(!addressGood)
				{
					ShowMessage("Address Broken");
					ErrThrow(BtErrCancelled);
				}
*/


				//	 If the port is already open, close it first
				if(BtPortId != NULL)
				{
					MmCloseBtPort(&BtPortId);
				}

				//	Open then close the library to ensure its there
				err = BtOpenCloseLib(&BtLibRefNum, BtOpenAndClose);

				if(err == btLibErrCanceled)
				{
					ErrThrow(BtErrCancelled);
				}
				else if(err)
				{
					ErrThrow(BtErrBtLibOpenClose);
				}
				else
				{
/*		Various old Debug Messages, grouped to make actual code more readable!!!
ShowMessage("BtSerialOpen device %d\n%x %x\n%s",
													Prefs.btDevice,
													Prefs.btDevices[0].address[1],
													Prefs.btDevices[0].address[0],
													Prefs.btName[Prefs.btDevice].name
																										);
*/
	//ShowMessage("BtLibRefNum %x\nBtPortId %x", BtLibRefNum, BtPortId);
	//ShowMessage("BtSerialOpen err %x\nBtPortId %x\nPrefs.btDevice %d", err, BtPortId, Prefs.btDevice);

/*
ShowMessage("BtSerialOpen device %d\nAddress %x %x %x %x %x %x",
													Prefs.btDevice,
													Prefs.btDevices[0].address[5],
													Prefs.btDevices[0].address[4],
													Prefs.btDevices[0].address[3],
													Prefs.btDevices[0].address[2],
													Prefs.btDevices[0].address[1],
													Prefs.btDevices[0].address[0]
																							);
*/

//if(serialOpenAttempt>1)
//ShowMessage("err %x\nBtPortId %x\nserialOpenAttempt %d", err, BtPortId, serialOpenAttempt);

//ShowMessage("BtSerialOpen err %x\nBtPortId %x", err, BtPortId);
					
					if(Prefs.btNumDevices == 1)
						btserialOpenAttempts = BtSerialOpenAttempts + 1;
					else
						btserialOpenAttempts = BtSerialOpenAttempts;
					
					while(btOkay == false)
					{
						btOkay = true;

						if(Prefs.btDevice == NoValue8)
						{
							ErrThrow(BtErrNoDeviceSelected);
						}
						else
						{
							//	Okay. So actually open the Virtual serial port
							err = 1;
							serialOpenAttemptsCount = 0;

							while( err	&&
									(err != btLibErrCanceled)	&&
									(serialOpenAttemptsCount < btserialOpenAttempts))
							{
								err = BtOpenSerial(&BtPortId, Prefs.btDevices[Prefs.btDevice]);

								if(err)
								{
									MmCloseBtPort(&BtPortId);
								}
								serialOpenAttemptsCount++;
							}

							//	If we never connected, offer to try another Bt device
							if(err != errNone	&&
								serialOpenAttemptsCount >= btserialOpenAttempts)
							{
								if(Prefs.btNumDevices > 1)
								{
									btOkay = false;
									err = 0;

									if(FrmAlert(InfBeamFailSelectNewAlert) == InfBeamFailSelectNewSelectNew)
									{
										BtSelectDevice(frm, Prefs.btName, Prefs.btSelectedDeviceName, &Prefs.btDevice);
									}
								}
							}
						}

						if(err)
						{
							BtPortId = NULL;
						}
						
						if(err == btLibErrCanceled)
						{
							ErrThrow(BtErrCancelled);
						}
						
						else if(err == BtLibErrNoAddress)
						{
							ErrThrow(BtLibErrNoAddress);
						}

						else if(err)
						{
							ErrThrow(BtErrPortOpen);
						}
					}// While(btOkay == false)
				}//err = BtOpenCloseLib(&BtLibRefNum, BtOpenAndClose);  if(err)..  else
			}
#endif
#endif

#ifdef DebugDL2
TotalPacketsSent = 0;
TotalPacketsReceived = 0;
#endif






//	For all transport options, set up receive buffer

			ALReceivePtr = MemPtrNew(MyALReceiveBuffSize);

			if(ALReceivePtr)
			{
//				MmPrvSyncLogTrace("\nALReceive buffer memory Locked");
			}
			else
			{
				ShowMessage("Unable to create ALReceive buffer");
			}






//	Debug force out a Send message (defined at start of this function)
#ifdef DebugForceMessage

	ptrMessage = SendBuff + sizeof(BtMessageHeader);	//	Leave room for our msg header

	for(sendIndex=0; sendIndex<sizeof(fakeMessage); sendIndex++)
		ptrMessage[sendIndex] = fakeMessage[sendIndex];	//	Copy the Fake Header

	MmBeamMessage(ptrMessage, sendIndex);
	

	//	First shut down Serial Port to save batteries
	MmCloseBtPort(&BtPortId);

	return true;
#endif






//	Set up for StandAlone mode, using built in menu

#ifdef StandAlonePalm
	#define	Fake				//	Ensure that the real beaming doesn't happen
	{
		MmParseInfoSyncMessage(100);
		MmParseMenuSyncMessage(100, DishesMenu);
		MmParseMenuSyncMessage(100, DrinksMenu);
		Prefs.numUnsentOrders = 0;
		NumOldUnsentOrders = 0;
	}
//	if(MmParseGetAllOrdersMessage(1000))
//	{
//		FrmAlert(WarnUserCancelledAlert);
//	}

#endif






#ifdef DebugFakeMenu
	#define	Fake

	#ifndef StandAlonePalm
	
//	NB. toggle simulates the sending and receiving of messages in 2 phases
// for the emulator. It is initialised as false.
/*
	if(toggle)
		toggle = false;
	else
		toggle = true;
*/	

	toggle = true;
	if(toggle)
	{
/*
					MmFillMessageHeader(HLRegister, &sendIndex, statusError);
					MmAssemblePalmUniqueIDMessage(&sendIndex, &SendBuff[0], &dataCount);
					dataCountTotal = dataCount;
					MmAssemblePalmRegistrationMessage(&sendIndex, &SendBuff[0], &dataCount);
					dataCountTotal += dataCount;
//					MmHLCalcCRC(messageStart, dataCount, &sendIndex);
*/



		MmParseInfoSyncMessage(100);
		MmParseMenuSyncMessage(100, DishesMenu);
		MmParseMenuSyncMessage(100, DrinksMenu);
		MmParseLoginMessage(100);

//		if(MmParseGetAllOrdersMessage(1000))
//		{
//			FrmAlert(WarnUserCancelledAlert);
//		}

		Prefs.loggedIn = true;
		Prefs.autoLogoutSecs = TimGetSeconds() + 2;	//	2 Free secs
		Prefs.numUnsentOrders = 0;
		NumOldUnsentOrders = 0;
	}
	else
	{
//		MmAssembleInfoSyncMessage(&sendIndex, &SendBuff[0], &dataCount);

		for(sendIndex=0; sendIndex<sizeof(fakeHeader); sendIndex++)
			SendBuff[sendIndex] = (UInt8)(fakeHeader[sendIndex]);	//	Copy the Fake Header
/*		
//		MmAssembleBillingMessage(&sendIndex, &dataCount, false);
//		MmMarkSentAsConfirmed();


		MmAssembleCallAwayMessage(&sendIndex, &dataCount);
		Prefs.hLStatus.HLCallAwayStatus	= HLStatusAllDone;



		MmAssembleOrdersMessage(&sendIndex, &dataCount, IsSendOrders);
		dataCountTotal = dataCount;

		MmBeamMessage(SendBuff, sendIndex);

		MmMarkSentAsConfirmed();
		Prefs.hLStatus.HLSendOrdersStatus	= HLStatusAllDone;

//		MmZapDB(&MmOrdersDB, MmOrdersDBName, RecreateDB);
*/
		MmParseGetAllOrdersMessage(1000);
	}

	#endif		//	StandAlonePalm
#endif			//	DebugFakeMenu





			//	Go through and clear the warnings
			for(i=0; i < MaxProcessErrorValues; i++)
			{
				processError[i] = 0;
			}



#ifndef	Fake

			MmProcessMessages(&processError[0]);

#endif	






			//	First shut down Serial Port to save batteries
			MmCloseBtPort(&BtPortId);

			MmPlaySounds(finishedSound);
		}	//	if FrmOpenEvent
	
	}	//	ErrTry









	//	Error Traps
	ErrCatch(error)
	{
		//	First shut down Serial Port to save batteries
		MmCloseBtPort(&BtPortId);

		MmPlaySounds(errorSound);

//		delayScreenLock = true;

		switch(error)
		{
			case HLErrMessDisplayErrorMsg:
				ShowMessage(TempStr);
//				FrmCustomAlert(InfBeamingFailureAlert, TempStr, "", "");
//				MmLogAndDisplayError(TempStr, Reset);
				break;
				
			case HLErrMessInvalidMessageHostCRC:
				MmLogAndDisplayError("CRC error in Message to Host", Reset);
				break;
				
			case HLErrMessInvalidMessagePalmCRC:
				MmLogAndDisplayError("CRC error in Message to Palm", Reset);
				break;
				
			case HLErrMessCorruptMessage:
				MmLogAndDisplayError("Corrupt Message Received at Server", Reset);
				break;
				
			case HLErrMessMismatchMessageData:
				MmLogAndDisplayError("Mismatched Message Length", NoReset);
				break;
				
			case HLErrMessDishesNotSyncError:
				FrmAlert(WarnDishesDBNotSyncAlert);
				FrmAlert(WarnBeamWillDestroyDishesDBAlert);
				break;
				
			case HLErrMessDrinksNotSyncError:
				FrmAlert(WarnDrinksDBNotSyncAlert);
				FrmAlert(WarnBeamWillDestroyDrinksDBAlert);
				break;
				
			case HLErrMessServerDBError:
				FrmAlert(WarnServerDatabaseErrorAlert);
				FrmAlert(WarnBeamWillDestroyDishesDBAlert);
//				MmLogAndDisplayError("HLErrMess: Server Database Error");
				break;
				
			case HLErrMessInvalidMessageNumber:
				MmLogAndDisplayError("Invalid Message Number", Reset);
				break;
				
			case HLErrMessNullMessageExpected:
				MmLogAndDisplayError("Null Message Expected", Reset);
				break;
				


			//	Non HLMessage Errors Follow

			case HLErrMessUnknownResponseType:
				MmLogAndDisplayError("Unknown Response Type", Reset);
				break;
	
			case HLErrMessNotLoggedIn:
//				FrmAlert(WarnLoginNotLoggedInAlert);	//	This warning now issued below instead
//				loginFailed = true;
				break;
				
			case HLErrMessWrongPIN:
				FrmAlert(WarnLoginWrongPINAlert);
//				loginFailed = true;
				break;
				
			case HLErrMessUserNotFound:
				FrmAlert(WarnLoginUserListChangedAlert);
//				loginFailed = true;
				break;
				
			case HLErrMessMaxUsersReached:
				FrmAlert(WarnLoginTooManyPalmsAlert);
//				loginFailed = true;
				break;
				
			case HLErrMessBeamingFailure:
				FrmCustomAlert(InfBeamingFailureAlert, "Sorry, connected to Bluetooth okay, but MenuMate server was busy.\n\nPlease try again.", "", "");
//				ShowMessage("Send T %d\nRecv T %d", Prefs.btTimeoutSendOrders, Prefs.btTimeoutGetAllOrders);
				break;
				
			case HLErrMessBeamingTooLarge:
				FrmAlert(InfBeamingTooLargeAlert);
				break;
				
			case HLErrMessUnspecifiedError:
				MmLogAndDisplayError("Unspecified Error", Reset);
				break;
	
			case HLParseErrorLogin:
				MmLogAndDisplayError("Error Parsing Login Message", Reset);
//				loginFailed = true;
				break;
	
			case HLParseErrorInfo:
				MmLogAndDisplayError("Error Parsing Info Message", Reset);
				break;
	
			case HLParseInfoSyncNoID:
				MmLogAndDisplayError("Palm has no Serial Number", Reset);
				break;
	
			case HLParseInfoSyncBadID:
				MmLogAndDisplayError("Palm's Serial Number incorrect", Reset);
				break;
	
			case HLParseErrorMenuNoSizes:
				MmLogAndDisplayError("Menu has no Sizes", NoReset);
				break;
	
			case HLParseErrorMenuNoServingCourses:
				MmLogAndDisplayError("Menu has no Serving Courses", NoReset);
				break;
	
			case HLParseNotValidOrder:
				MmLogAndDisplayError("An Order has an incorrect Index", Reset);
				break;
	
			case HLParseInvalidSetMenuNumber:
				MmLogAndDisplayError("An Order has an invalid Set Menu number", Reset);
				break;
	
			case HLAssemblePalmUniqueIDNoID:
				MmLogAndDisplayError("Error Assembling PalmUniqueID Message", Reset);
				break;
				
			case HLAssemblePalmRegNoRegName:

				ShowMessage("No Palm Registration Name has been entered");
				RegTemp.registering = true;
				break;
				
			case HLAssembleOrdersTableTooHigh:
				ShowMessage("A table number that is too high was found.\nSorry, cannot send.");
				break;
				
			case HLAssembleOrdersSeatTooHigh:
				ShowMessage("A seat number that is too high was found.\nSorry, cannot send.");
				break;
				
			case HLAssembleOrdersTooMany:
				MmLogAndDisplayError("Attempting to send too many orders", Reset);
				break;
				
			case HLErrMessPalmIDNotFound:	
				FrmAlert(InfPalmUniqueIDnotFoundAlert);
				break;

			case HLErrMessWrongRegistrationPIN:	
				ShowMessage("Sorry. Your Registration PIN is incorrect");
				break;

			case HLErrMessNotInRegistrationMode:	
				ShowMessage("The POS server is not in Registration Mode");
				break;

			case HLErrMessNoPrinterFound:	
				FrmAlert(InfNoPrinterFoundAlert);
				break;

			case HLErrMessWrongVersionNum:	
				ShowMessage("Palm and POS Versions do not match\nPalm version is %s", Prefs.verNumStrShort);
				MmLogAndDisplayError("Reset Palm now", Reset);
				break;

			case HLErrMessOkay:
				break;
				


			//	Bluetooth Connection Errors
			case BtErrNoDevices:
				ShowMessage("No Bluetooth devices are available.\nPlease try again.");
				break;
				
			case BtErrCancelled:
				ShowMessage("Cancelled by user");
				ALReceivePtr = 0;
				break;
				
			case BtErrNoDeviceSelected:
				ShowMessage("No Bluetooth Device Selected. Please select one now.");
				break;
				
			case BtErrBtLibOpenClose:
				MmLogAndDisplayError("Error opening Bluetooth Library", Reset);
				break;
				
			case BtErrPortOpen:
				FrmCustomAlert(InfBeamingFailureAlert, "Either:\nYou are out of range\n\nOr\nBluetooth device is Busy.\n\nPlease try again.", "", "");
				break;
				
			case BtErrPortClose:
				MmLogAndDisplayError("Error closing Bluetooth Port", Reset);
				break;
				
			case BtLibErrNoAddress:
				ShowMessage("No Bluetooth device found.\nPlease setup Bluetooth devices");
				break;
				
			case BtErrOkay:
				break;
				

			default:
				MmLogAndDisplayError("Unspecified default Messaging Error", Reset);
		}

		handled = true;
	}
	ErrEndCatch






	if(eventType == frmOpenEvent)
	{
		if(ALReceivePtr)
		{
			if(MemPtrFree(ALReceivePtr))
			{
				MmLogAndDisplayError("Unable to Free ALReceivePtr Memory", Reset);
			}
			else
			{
//				MmPrvSyncLogTrace("\nALReceive buffer memory Unlocked");
			}
		}
		


		//	Go through and display any warnings generated in the communication in succession
		for(i=0; i < MaxProcessErrorValues; i++)
		{
			if(processError[i])
			{
				switch(processError[i])
				{
					case HLErrMessNoDishesMenuFound:		FrmAlert(InfNoDishesMenuFoundAlert);		break;
					case HLErrMessNoDrinksMenuFound:		FrmAlert(InfNoDrinksMenuFoundAlert);		break;

					case HLWarnPrintingRedirected:		FrmAlert(InfPrintingRedirectedAlert);		break;
					case HLWarnPrinterOffline:				FrmAlert(InfPrinterOfflineAlert);			break;
					case HLWarnCallAwayPrinterNotSetUp:	FrmAlert(InfCallAwayPrinterNotSetUpAlert);break;
					case HLWarnKitchenPrinterNotSetUp:	FrmAlert(InfKitchenPrinterNotSetUpAlert);	break;
				}
			}
		}



		//	Restore
		Prefs.current.tableNo	= currentTableNumber;
		Prefs.current.seatNo	= currentSeatNumber;

		AutoTablePageShortSecs = TimGetSeconds() + AutoGotoTablePageShortSecs;


#ifdef ReleaseVersion
		if(  !RegTemp.autoReRegister	&&
				Prefs.palmUniqueID == 0	&&
				RegTemp.registering == false)	//	Force return to Registering Page
		{
			FrmAlert(WarnPalmNotRegisteredAlert);
			RegTemp.registering = true;
			RegTemp.regSuccessful = false;
		}
#endif
		
		if(RegTemp.regSuccessful)
		{
			UInt16	initialiseMode;


			if(!RegTemp.autoReRegister)
			{
				if(StrLen(Prefs.palmRegistrationName))
					ShowMessage("Registration Successful as: %s", Prefs.palmRegistrationName);

				initialiseMode = 0;
				initialiseMode = initialiseMode | InitKeepRegistration;
				initialiseMode = initialiseMode | InitKeepBluetoothNames;
				initialiseMode = initialiseMode | InitZapAllDBsNotInfo;	//	Keep InfoDB for Palms, Staff, Tables
				MmInitialiseAll(initialiseMode);									//	Blitz everything else
			}
			RegTemp.autoReRegister	= false;	//	All done
			RegTemp.regSuccessful 	= false;
		}



		if(RegTemp.palmListChanged)
		{
			RegTemp.palmListChanged = false;
			
			ShowMessage("Palm List Changed.\n\nPlease Re-register");
		}

	
		//	If the server sent a message we better display it
		if(ServerMessageIs[0] != 0)
		{
			FrmCustomAlert(InfMessageReceivedAlert, ServerMessageFrom, ServerMessageIs, "");
			ServerMessageIs[0] = 0;				//	Clear the Server Message
		}



		//	Remind that they're not logged in
		if(!Prefs.loggedIn && Prefs.hLStatus.HLLoginStatus != HLStatusToSend)
		{
			if(RegTemp.registering == false	&&	!Prefs.userSettings.advancedUser)
			{
				FrmAlert(WarnLoginNotLoggedInAlert);
//				loginFailed = true;
			}
		}

		//	Go back to the appropriate form
		PreviousForm = OverviewForm;
		if(Prefs.userSettings.classicMode)
			OverviewPageType = OverviewOrdersOnlyPage;
		else
			OverviewPageType = OverviewTakeOrders;


#ifdef ReleaseVersion




//ShowDebug("Send Orders: %u\nGet All Orders: %u", Prefs.btTimeoutSendOrders, Prefs.btTimeoutGetAllOrders);


		if(RegTemp.registering == true)
		{
			AutoTablePageShortSecs = NoValue32;
			ScreenLockStruct.action = disabled;					//	Turn off for now, gets turned on after Login
			FrmGotoForm(RegisterForm);
		}


		//	Not registering, normal transmission
		else
		{
			ScreenLockStruct.timeSecs = TimGetSeconds() + AutoScreenLockShortTimeSecs;
			
			if(	BeamMessagesDone.HLGetAllOrdersStatus != HLStatusIsSent
				||
					processError											)	//	Something broke
			{
				ScreenLockStruct.action = awaitingTimeSecs;	//	Await next AutoScreenLockxxxxTimeSecs timeout
			}
			else	//	Normal successful transmission
			{
				ScreenLockStruct.action = lockOnNilEvent;	//	Straight to ScreenLock on next NilEvent
			}

			
			//	Return to the appropriate form
			if(	Prefs.loggedIn == false
				||
					BeamMessagesDone.HLLoginStatus == HLStatusIsSent			//	Login succeeded
				||
					BeamMessagesDone.HLLoginStatus == HLStatusReadyToSend)	//	Login attempt Failed
			{
				FrmGotoForm(TableForm);
			}
			else
			{
				FrmGotoForm(OverviewForm);
			}
		}

#else
		FrmGotoForm(OverviewForm);
#endif


		handled = true;
	}

	DBDeleteBitMask = 0;

	return(handled);
}

