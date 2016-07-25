#define EXTERNAL 1

#include "MenuMate.h"

//#include <BtCommVdrv.h>

//#include "BTSerial.h"
//#include "Bluetooth.h"

/***********************************************************************
 *
 * FUNCTION:    BtOpenCloseLib
 *
 * DESCRIPTION: Initialises Bluetooth by opening via lib.
 *
 * PARAMETERS:
 *
 *					*btLibRefNum
 *						pointer to the Bt Library.
 *
 *					device
 *						Bluetooth address to connect to.
 *
 * RETURNED:    Err
 *
 ***********************************************************************/
Err BtOpenCloseLib(UInt16 *btLibRefNum, UInt16 action)
{
	Err					err;


	//	Ensure the Library is closed before we attempt to open it
	err = BtLibClose(*btLibRefNum);

	if(action != BtCloseOnly)
	{
		err = BtLibOpen(*btLibRefNum, false);
		
		if(action == BtOpenOnly || err != btLibErrNoError)
		{
			return err;
		}
		
		err = BtLibClose(*btLibRefNum);
		if(err)
		{
			return err;
		}
	}

	return err;
}


/***********************************************************************
 *
 * FUNCTION:    BtOpenSerial
 *
 * DESCRIPTION: Opens a Bluetooth serial connection.
 *
 * PARAMETERS:
 *
 *					p_btPortId
 *						is filled in with the ID of the new connection.
 *
 *					device
 *						Bluetooth address to connect to.
 *
 * RETURNED:    Err
 *
 ***********************************************************************/
Err BtOpenSerial(UInt16 *p_btPortId, BtLibDeviceAddressType device)
{
	SrmOpenConfigType		config;
	BtVdOpenParams			btParams;
	BtLibSdpUuidType		sppUuid;

	
	if (device.address[0] == 0 &&
		 device.address[1] == 0 &&
		 device.address[2] == 0 &&
		 device.address[3] == 0 &&
		 device.address[4] == 0 &&
		 device.address[5] == 0)
	{
		return BtLibErrNoAddress;
	}
	
// Set up a SerialPort service class UUID
	MemSet(&sppUuid, sizeof(sppUuid), 0 );

	BtLibSdpUuidInitialize(sppUuid, btLibSdpUUID_SC_SERIAL_PORT, btLibUuidSize16);

//ShowMessage("ChannelId %x", ChannelId);

// Set up a bluetooth parameter block: play client role,
// and use sdp to search for the serial port profile service class uuid.
	MemSet(&btParams, sizeof(btParams), 0);

	btParams.role							= btVdClient;
	btParams.encrypt						= false;
	btParams.authenticate				= false;

	btParams.u.client.remoteDevAddr.address[0] = device.address[0];
	btParams.u.client.remoteDevAddr.address[1] = device.address[1];
	btParams.u.client.remoteDevAddr.address[2] = device.address[2];
	btParams.u.client.remoteDevAddr.address[3] = device.address[3];
	btParams.u.client.remoteDevAddr.address[4] = device.address[4];
	btParams.u.client.remoteDevAddr.address[5] = device.address[5];
	

//	btParams.u.client.method			= btVdUseChannelId;	// we know the RFCOMM channel id ...
//	btParams.u.client.u.channelId		= 0x53;					// ... and this is it
//	btParams.u.client.u.channelId		= ChannelId;			// ... and this is it
//	ChannelId--;

	btParams.u.client.method			= btVdUseUuidList;
	btParams.u.client.u.uuidList.tab	= &sppUuid;
	btParams.u.client.u.uuidList.len = 1;

// Set up the SrmExtOpen param block to reference our bluetooth param block
	MemSet(&config, sizeof(config), 0);		//	Set everything to 0 first

	config.function						= 0;
	config.drvrDataP						= (MemPtr)&btParams;
	config.drvrDataSize					= sizeof(BtVdOpenParams);

	return SrmExtOpen(sysFileCVirtRfComm, &config, sizeof(config), p_btPortId);
}


/*
My guess is that something is leaving the library open. Could be the RFCOMM
virtual serial driver, or you - if you are doing Bluetooth discovery.  Get
the libRefNum, then call btLibClose repeatedly until it returns an error.
*/


/***********************************************************************
 *
 * FUNCTION:    BtCloseSerial
 *
 * DESCRIPTION: Closes a Bluetooth serial connection.
 *
 * PARAMETERS:
 *
 *						p_btPortId
 *						ID of the connection to close.
 *
 * RETURNED:    Nothing
 *
 ***********************************************************************/
Err BtCloseSerial(UInt16 *p_btPortId)
{
	Err				err;
	
	
	if(*p_btPortId == NULL)
	{
		return errNone;
	}
	
	err = SrmClose(*p_btPortId);
	
	if(err == errNone)
	{
		*p_btPortId = NULL;
	}
	
	return err;
}


/***********************************************************************
 *
 * FUNCTION:    BtSendSerial
 *
 * DESCRIPTION: Sends bytes of data through Bluetooth serial connection.
 *
 * PARAMETERS:
 *
 *					btPortId
 *						ID of the connection to send data through.
 *
 *					data
 *						pointer to the data to send
 *
 *					dataLength
 *						number of bytes to send
 *
 *					p_bytesSent
 *						Number of bytes actually sent
 *
 * RETURNED:    Error
 *
 ***********************************************************************/
Err BtSendSerial(UInt16 btPortId, const void *data, UInt16 dataLength, UInt32 *p_bytesSent)
{
	Err					err;
	UInt32				count;
	
	
	if(btPortId == NULL)
	{
//ShowMessage("btPortId == NULL");
		return serErrLineErr;
	}
	
	count = SrmSend(btPortId, data, dataLength, &err);
	
	if(err == serErrLineErr)
	{
//ShowMessage("err == serErrLineErr");
		SrmClearErr(btPortId);
	}
	
	if(p_bytesSent != NULL)
	{
		*p_bytesSent = count;
	}
	
	return err;
}


/***********************************************************************
 *
 * FUNCTION:    BtSendSerialStr
 *
 * DESCRIPTION: Sends bytes of data through Bluetooth serial connection,
 *					then appends a \r\n pair.
 *
 * PARAMETERS:
 *
 *					btPortId
 *						ID of the connection to send data through.
 *
 *					data
 *						pointer to the data to send
 *
 *					dataLength
 *						number of bytes to send
 *
 *					p_bytesSent
 *						Number of bytes actually sent
 *
 * RETURNED:    Error
 *
 ***********************************************************************/
Err BtSendSerialStr(UInt16 btPortId, const Char *text, UInt16 length, UInt32 *p_bytesSent)
{
	Err				err;
	UInt32			count;
	
	
	if(btPortId == NULL)
	{
		return serErrLineErr;
	}
	
	//	First send the body of the string
	count = SrmSend(btPortId, text, length, &err);
	
	if(err == errNone)
	{
		count += SrmSend(btPortId, "\r\n", 2, &err);
	}
	
	if(err == serErrLineErr)
	{
		SrmClearErr(btPortId);
	}
	
	if(p_bytesSent != NULL)
	{
		*p_bytesSent = count;
	}
	
	return err;
}


/***********************************************************************
 *
 * FUNCTION:    BtReadSerial
 *
 * DESCRIPTION: Read bytes of data with timeout.
 *
 * PARAMETERS:
 *
 *					btPortId
 *						ID of the connection to send data through.
 *
 *					buffer
 *						pointer to the buffer to fill in with received data.
 *
 *					buffLen
 *						number of bytes to wait for.
 *
 *					timeout
 *						time in ticks to wait for.
 *
 * RETURNED:    The error code.
 *
 ***********************************************************************/
Err BtReadSerial(UInt16 btPortId, void *buffer, UInt32 buffLen, UInt32 timeout, UInt32 *p_bytesRead)
{
	Err						err;
	
	if(btPortId == NULL)
	{
		*p_bytesRead = 0;
		return serErrLineErr;
	}
	*p_bytesRead = SrmReceive(btPortId, buffer, buffLen, timeout, &err);

	if(err == serErrLineErr)
	{
		SrmClearErr(btPortId);
	}

	return err;
}


/***********************************************************************
 *
 * FUNCTION:    BtReadSerialBuff
 *
 * DESCRIPTION: Reads bytes of data in FIFO and returns immediately.
 *
 * PARAMETERS:
 *
 *					btPortId
 *						ID of the connection to send data through.
 *
 *					buffer
 *						pointer to the buffer to fill in with received data.
 *
 *					buffLen
 *						max number of bytes to get
 *
 * RETURNED:    Number of bytes read.
 *
 ***********************************************************************/
Err BtReadSerialBuff(UInt16 btPortId, void *buffer, UInt32 buffLen, UInt32 *p_bytesRead)
{
	Err						err;
	UInt32					bytes;
	
	if(btPortId == NULL)
	{
		*p_bytesRead = 0;
		return serErrLineErr;
	}
	
	err = SrmReceiveCheck(btPortId, &bytes);
	
	if(err != errNone)
	{
		if(err == serErrLineErr)
		{
			SrmClearErr(btPortId);
		}
		*p_bytesRead = 0;
		return err;
	}
	
	if(buffLen < bytes)
	{
		bytes = buffLen;
	}
	
	if(bytes == 0)
	{
		*p_bytesRead = 0;
		return errNone;
	}
	
	*p_bytesRead = SrmReceive(btPortId, buffer, bytes, 0, &err);
	
	if(err == serErrLineErr)
	{
		SrmClearErr(btPortId);
	}
	
	return err;
}


/***********************************************************************
 *
 * FUNCTION:    BtDiscoverDevice
 *
 * DESCRIPTION: Presents a list of descoverable devices to select 1.
 *
 * PARAMETERS:
 *
 *					p_btPortId
 *						pointer to the ID of the current connection.
 *
 *					p_device
 *						pointer to struct to fill in with selected device.
 *
 * RETURNED:	sysErrLibNotFound, sysErrNoFreeRAM, sysErrNoFreeLibSlots
 *					btLibErrAlreadyOpen, btLibErrInUseByService,
 *					btLibErrOutOfMemory, btLibErrRadioInitFailed
 *					btLibErrCanceled, btLibErrNotOpen, btLibErrStackNotOpen
 *
 ***********************************************************************/
/*
Err BtDiscoverDevice(UInt16 *p_btPortId, BtLibDeviceAddressType *p_device)
{
	Err							err;
//	BtLibClassOfDeviceType	filterList[] = {	btLibCOD_ServiceAny | 
//															btLibCOD_Major_Computer | 
//															btLibCOD_Minor_Any };
	UInt16	 					gBtLibRefNum = 0;

	if(*p_btPortId != NULL)
	{
		BtCloseSerial(p_btPortId);
	}
	
	if(SysLibFind( btLibName, &gBtLibRefNum))
	{
		// load it if it can't be found	
		err = SysLibLoad( sysFileTLibrary , sysFileCBtLib, &gBtLibRefNum) ;
		
		if(err != errNone)
		{
			// Could not find BtLib
			return err;
		}
	}
	
	err = BtLibOpen(gBtLibRefNum, false);
	
	if(err != btLibErrAlreadyOpen)
		return err;

	
//	BtLibRegisterManagementNotification(gBtLibRefNum, BtLibMECallbackProc, 0);
	err = BtLibDiscoverSingleDevice(	gBtLibRefNum, 
												NULL,
//												filterList,
//												2,
												NULL,
												0,
												p_device,
												false,
												true);
	if(err)
		return err;


	BtLibClose(gBtLibRefNum);
	
	return err;	
}
*/

/***********************************************************************
 *
 * FUNCTION:    BtDiscoverMultipleDevices
 *
 * DESCRIPTION: Presents a list of discoverable devices to select n.
 *
 * PARAMETERS:
 *
 *					*btLibRefNum
 *						pointer to the Bt Library.
 *
 *					p_btPortId
 *						pointer to the ID of the current connection.
 *
 * RETURNED:	sysErrLibNotFound, sysErrNoFreeRAM, sysErrNoFreeLibSlots
 *					btLibErrAlreadyOpen, btLibErrInUseByService,
 *					btLibErrOutOfMemory, btLibErrRadioInitFailed
 *					btLibErrCanceled, btLibErrNotOpen, btLibErrStackNotOpen
 *
 ***********************************************************************/
Err BtDiscoverMultipleDevices(UInt16 *btLibRefNum, UInt16 *p_btPortId, Boolean showLastList)
{
	Err							err;
	BtLibClassOfDeviceType	filterList[] = {	btLibCOD_ServiceAny | 
															btLibCOD_Major_Computer | 
//															btLibCOD_Minor_Comp_Any
															btLibCOD_Minor_Comp_Desktop
																								};
	UInt8		 					numDevSelected = 0;
	Boolean						addressAsName = false;

	UInt16						i;


	if(*p_btPortId != NULL)
	{
		BtCloseSerial(p_btPortId);
	}

	//	Ensure the Library is closed before we attempt to open it
	for(i=0; i<5; i++)
	{
		if(BtLibClose(*btLibRefNum) == btLibErrNotOpen)
			break;
	}
	
	err = BtLibOpen(*btLibRefNum, false);
	
	if(err == btLibErrAlreadyOpen)
		;
//		ShowMessage("Bluetooth library already open");
	
	else if(err)
		return err;
	

/*
BtLibDiscoverMultipleDevices

Parameters 	-> btLibRefNum Reference number for the Bluetooth library.
				-> instructionTxt
						Text displayed at the top of the selection box. Pass NULL to display the default text. The default text is 'Select one or more devices:'
				-> buttonTxt
						Text for the OK button. Pass NULL to display the default text. The default button text is 'OK'
				-> deviceFilterList
						Array of BtLibClassOfDeviceTypes. This function presents to the user only the remote devices whose class matches a class in this list. If deviceFilterList is NULL, this function presents to the user all discovered devices.
				-> deviceFilterListLen
						Number of elements in deviceFilterList.
				<- numDevicesSelected
						Number of selected devices. To obtain the actual device list, use the BtLibGetSelectedDevices function.
				-> addressAsName
						If true, display the Bluetooth addresses of the remote devices instead of their names. This option is available for debugging purposes.
				-> showLastList
						If true, causes all other parameters to be ignored and displays the same list as the previous call to BtLibDiscoverMultipleDevices.
*/
	err = BtLibDiscoverMultipleDevices(	*btLibRefNum,//UInt16 btLibRefNum,
													NULL,//Char *instructionTxt,
													NULL,//Char *buttonTxt,
													filterList,//BtLibClassOfDeviceType *deviceFilterList,
													1,//UInt8 deviceFilterListLen,
													&numDevSelected,//UInt8 *numDevicesSelected,
													addressAsName,//Boolean addressAsName,
													showLastList);//Boolean showLastList)
	return err;
}


/***********************************************************************
 *
 * FUNCTION:    BtGetSelectedDevices
 *
 * DESCRIPTION: Get discovered devices and friendly names.
 *
 * PARAMETERS:
 *
 *					*btLibRefNum
 *						pointer to the Bt Library.
 *
 *					p_device
 *						pointer to struct to fill in with selected device.
 *
 *					*p_name
 *						pointer to BtLibFriendlyNameType struct.
 *
 *					*p_btNumDevices
 *						pointer to prefs->btNumDevices.
 *
 * RETURNED:	sysErrLibNotFound, sysErrNoFreeRAM, sysErrNoFreeLibSlots
 *					btLibErrAlreadyOpen, btLibErrInUseByService,
 *					btLibErrOutOfMemory, btLibErrRadioInitFailed
 *					btLibErrCanceled, btLibErrNotOpen, btLibErrStackNotOpen
 *
 ***********************************************************************/
Err BtGetSelectedDevices(	UInt16 *btLibRefNum,
									BtLibDeviceAddressType *p_device,
									BtLibFriendlyNameType *p_name,
									UInt8 *p_btNumDevices)
{
	Err							err;
	UInt16						btDevice;
/*
BtLibGetSelectedDevices

Parameters	-> btLibRefNum
						Reference number for the Bluetooth library.
				<->selectedDeviceArray
						Array into which the results of the BtLibDiscoverMultipleDevices function should be placed. You must allocate this array of BtLibDeviceAddressTypes.
				-> arraySize Number of elements in the
						selectedDeviceArray you allocated.
				<- numDevicesReturned
						Number of results placed in selectedDeviceArray.
*/

	err = BtLibGetSelectedDevices(		*btLibRefNum,//UInt16 btLibRefNum,
													p_device,//BtLibDeviceAddressType *selectedDeviceArray,
													MaxBtTransceivers,//UInt8 arraySize,
													p_btNumDevices);//UInt8 *numDevicesReturned);
	if(err)
		return err;


	for(btDevice = 0; btDevice < *p_btNumDevices; btDevice++)
	{
/*
Err BtLibGetRemoteDeviceName(	UInt16 btLibRefNum,
										BtLibDeviceAddressTypePtr remoteDeviceP,
										BtLibFriendlyNameType *nameP,
										BtLibGetNameEnum retrievalMethod)
Parameters 	-> btLibRefNum
						Reference number for the Bluetooth library.
				-> remoteDeviceP
						Pointer to a BtLibDeviceAddressType containing the address of the device whose name is desired.
				<-> nameP
						Pointer to a BtLibFriendlyNameType structure in which to store the results of the lookup. You must allocate this structure and the name buffer it points to. You also must specify the size of the buffer in the nameLength field of the structure. When the function returns, the nameLength field contains the actual length of the name. This parameter must not be NULL.
				-> retrievalMethod
						Method used to retrieve the user-friendly remote device name.
*/
//ShowMessage("%d p_device %x,\np_name %x", btDevice, p_device, p_name);

		err = BtLibGetRemoteDeviceName(	*btLibRefNum,
													p_device,
													p_name,
													btLibCachedOnly);
		if(err)
			return err;

		p_device++;
		p_name++;
	}

	BtLibClose(*btLibRefNum);
	
	return err;	
}


/***********************************************************************
 *
 * FUNCTION:    BtSelectDevice
 *
 * DESCRIPTION: Selects a Bluetooth device for connection.
 *
 * PARAMETERS:
 *
 *					FormPtr frm
 *
 * RETURNED:   btDevice selected (as prefs->btDevice)
 *
 ***********************************************************************/
void BtSelectDevice(FormPtr frm, BtLibFriendlyNameType *p_name,
														Char *btSelectedDeviceName, UInt8 *prefs_btDevice)
{
	Int16						formId;
	ListPtr					listPtr;
	UInt16					offset;
	UInt8						btDevice;

	formId = FrmGetFormId(frm);
	
	switch(formId)
	{
		case MainForm:
			listPtr = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, MainDishesList));
			break;

		case TableForm:
			listPtr = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, TableNamesList));
			break;
	
		case OverviewForm:
			listPtr = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, OverviewDishesList));
			break;
	
		case BeamingForm:
			listPtr = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, BeamingBtDevicesList));
			break;

		default:
			return;
	}

	offset = 0;

	btDevice = MmPopupList(listPtr, BtDevicesList, &offset, 0);

	if(btDevice > MaxBtTransceivers)
	{
		btDevice = NoValue8;
	}

	if(btDevice != NoValue8)
	{
		*prefs_btDevice = btDevice;
		if(p_name[btDevice].name != NULL)
		{
			StrCopy(btSelectedDeviceName, "* ");
			StrCat(btSelectedDeviceName, (Char *)p_name[btDevice].name);
		}
	}
	else if(	*prefs_btDevice < MaxBtTransceivers		&&
				p_name[*prefs_btDevice].name != NULL		)
	{
		StrCopy(btSelectedDeviceName, "* ");
		StrCat(btSelectedDeviceName, (Char *)p_name[*prefs_btDevice].name);
		ShowMessage("Will connect to \n%s", btSelectedDeviceName+2);
	}
}



/***********************************************************************
 *
 * FUNCTION:     MmDiscoverBtDevices
 *
 * DESCRIPTION:  Just what it says. 
 *
 * PARAMETERS:   FormPtr frm.
 *
 * RETURNED:     
 *
 ***********************************************************************/
void MmDiscoverBtDevices(FormPtr frm, Boolean showLastList, UInt16 *btLibRefNum,
									UInt16 *btPortId, MmPreferenceTypePtr prefs, Boolean *reDiscoverBt)
{
	Err						err;

	UInt8						btDevice;
	
	if(*reDiscoverBt == false && showLastList == false)
		ShowMessage("This option will delete all existing Bluetooth devices.\n\n"
						"Use option 'Find More Bluetooth Devices' if you want to add more "
						"devices to the existing list");
	
	if(FrmAlert(WarnBluetoothDiscoveryAlert) == WarnBluetoothDiscoveryOK)
	{
		err = BtDiscoverMultipleDevices(btLibRefNum, btPortId, showLastList);
//ShowMessage("err %x", err);
		if(err == btLibErrCanceled)
		{
			;
		}
		else if(err)
		{
			LogPortErr(err);
		}
		else
		{
		   //	Prepare the Bluetooth friendly name buffer
			for(btDevice = 0; btDevice < MaxBtTransceivers; btDevice++)
			{
				prefs->btNamesBuffer[btDevice][0] = 0;
				prefs->btName[btDevice].name = (UInt8 *)prefs->btNamesBuffer[btDevice];
				prefs->btName[btDevice].nameLength = btLibMaxDeviceNameLength;
			}

			err = BtGetSelectedDevices(btLibRefNum, &prefs->btDevices[0],
														 &prefs->btName[0], &prefs->btNumDevices);

			prefs->btDevice = NoValue8;

			if(err)
			{
				prefs->btNumDevices = 0;
				prefs->btDevice = NoValue8;
				LogPortErr(err);
			}
			else
			{
				if(prefs->btNumDevices == 1)
				{
					prefs->btDevice = 0;
					if(prefs->btName[0].name != NULL)
					{
						StrCopy(prefs->btSelectedDeviceName, "* ");
						StrCat(prefs->btSelectedDeviceName, (Char *)prefs->btName[0].name);
						ShowMessage("Will connect to \n%s", prefs->btSelectedDeviceName+2);
					}
				}
				else if(prefs->btNumDevices > 1)
				{
					BtSelectDevice(frm, prefs->btName, prefs->btSelectedDeviceName, &prefs->btDevice);
				}
			}
//ShowMessage("%d Devices Selected", prefs->btNumDevices);
/*	ShowMessage("%d Devices Selected\nDevice Selected:\n%hx:%hx:%hx:\n%hx:%hx:%hx",
				prefs->btNumDevices,
				prefs->btDevices[0].address[5],
				prefs->btDevices[0].address[4],
				prefs->btDevices[0].address[3],
				prefs->btDevices[0].address[2],
				prefs->btDevices[0].address[1],
				prefs->btDevices[0].address[0]);
*/

			MmSavePrefs();
		}
	}
}

