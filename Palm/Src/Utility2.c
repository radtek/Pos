#define EXTERNAL 1

#include "MenuMate.h"

/***********************************************************************
 *
 * FUNCTION:    MmDoInitialiseCode
 *
 * DESCRIPTION: Do the body of the Initialisation Code
 *
 * PARAMETERS:  
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
void MmDoInitialiseCode(UInt16 *initialiseMode, Boolean isNewVersion, UInt16 *btLibRefNum,
														MmPreferenceTypePtr prefs, Boolean *reDiscoverBt,
														RegTempType* regTemp)
{
	UInt8							*srcePtr;
	UInt8							*destPtr;
	UInt16						i, j;
	UInt16						pointCount = 0;

	Err							err;

	BtLibDeviceAddressType	btDevices[MaxBtTransceivers];
	Char							btNamesBuffer[MaxBtTransceivers][btLibMaxDeviceNameLength];
	BtLibFriendlyNameType	btName[MaxBtTransceivers];
	Char							btSelectedDeviceName[btLibMaxDeviceNameLength+3];

	UInt8							btNumDevices;
	UInt8							btDevice;

	UInt16						btTimeoutInterByte;
	UInt16						btTimeoutFirstByte;
	UInt16						btTimeoutSendOrders;
	UInt16						btTimeoutGetAllOrders;
	
	UInt16						palmUniqueID;
//	UInt8							palmUniqueName[PalmUniqueNameLength + 1];
	UInt8							palmRegistrationName[PalmRegistrationNameLength + 1];



	//	If its NOT ResetAllPrefs ie. Its NOT a virgin install or a 'Delete DB and all Settings'
	if(	!	(*initialiseMode & InitResetAllPrefs))
	{
		//	If its a different version, trash the DB
		if(isNewVersion)
		{
//			ShowMessage("MenuMate version number has changed.\n\nDeleting database.");
			*initialiseMode |= InitZapAllDatabases;
			*initialiseMode |= InitResetAllPrefs;

			//	If old Prefs has Bt addresses in the wrong place then force Discovery
			if(	(StrAToI(&prefs->verNumStr[0]) == 2)		&&
				(	(StrAToI(&prefs->verNumStr[2]) < 7)	||
					(StrAToI(&prefs->verNumStr[4]) < 1)		)	)
			{
				*initialiseMode &= ~InitKeepRegistration;		//	Blitz Registration
				*initialiseMode &= ~InitKeepBluetoothNames;	//	Blitz BT names
				*reDiscoverBt = true;
			}
			//	Version 3.3 introduces new reigistration process
			else if(	(StrAToI(&prefs->verNumStr[0]) == 3)	&&
						(StrAToI(&prefs->verNumStr[2]) < 3)		)
			{
				*initialiseMode &= ~InitKeepRegistration;		//	Blitz Registration
				*initialiseMode |= InitKeepBluetoothNames;	//	Keep BT names
				*reDiscoverBt = true;
			}
			else
			{
				*initialiseMode |= InitKeepRegistration;		//	Keep Registration
				*initialiseMode |= InitKeepBluetoothNames;	//	Keep BT names
				*reDiscoverBt = false;
			}

			StrCopy(prefs->verNumStr, VersionNumberString);
		}
	}



	if(*initialiseMode & InitKeepRegistration)
	{
		// Preserve the registration name and number
		srcePtr = (UInt8*)&prefs->palmRegistrationName;
		destPtr = (UInt8*)&palmRegistrationName;
		for(i=0; i<sizeof(prefs->palmRegistrationName); i++)
		{
			*(destPtr++) = *(srcePtr++);
		}

		palmUniqueID = prefs->palmUniqueID;
	}


	
	if(*initialiseMode & InitKeepBluetoothNames)
	{
		//	Preserve all the Bluetooth names etc.
		srcePtr = (UInt8*)&prefs->btDevices[0];
		destPtr = (UInt8*)&btDevices[0];
		for(i=0; i<MaxBtTransceivers; i++)
		{
			for(j=0; j<sizeof(BtLibDeviceAddressType); j++)
			{
				*(destPtr++) = *(srcePtr++);
			}
		}

		for(i=0; i<MaxBtTransceivers; i++)
		{
		   StrCopy(btNamesBuffer[i], prefs->btNamesBuffer[i]);
		   btName[i].name						=	prefs->btName[i].name;
		   btName[i].nameLength				=	prefs->btName[i].nameLength;
		}
		StrCopy(btSelectedDeviceName, prefs->btSelectedDeviceName);

		btNumDevices							=	prefs->btNumDevices;
		btDevice									=	prefs->btDevice;

		btTimeoutInterByte					=	prefs->btTimeoutInterByte;
		btTimeoutFirstByte					=	prefs->btTimeoutFirstByte;
		btTimeoutSendOrders					=	prefs->btTimeoutSendOrders;
		btTimeoutGetAllOrders				=	prefs->btTimeoutGetAllOrders;
	}





	if(*initialiseMode & InitResetAllPrefs)
	{
		//	Initialise all globals.

		StrCopy(prefs->palmRegistrationName, "");
		prefs->palmUniqueID					=	0;


		for(i=0; i<MaxBtTransceivers; i++)
		{
		   prefs->btNamesBuffer[i][0]		= 0;
		   prefs->btName[i].name			= (UInt8 *)prefs->btNamesBuffer[i];
		   prefs->btName[i].nameLength	= btLibMaxDeviceNameLength;
		}

		prefs->btSelectedDeviceName[0]	=	0;

		for(i=0; i < btLibDeviceAddressSize; i++)
		{
			prefs->btPalmAddress.address[i] = 0;
		}

		prefs->btNumDevices					=	0;
		prefs->btDevice						=	NoValue8;
		
		prefs->btTimeoutInterByte			=	BtRxInterByteTimeout;
		prefs->btTimeoutFirstByte			=	BtRxFirstByteTimeout;
		prefs->btTimeoutSendOrders			=	BtSendOrdersTimeoutPerOrder;	//	In ms per order!
//		prefs->btTimeoutSendOrders			=	BtSendOrdersTimeout;
		prefs->btTimeoutGetAllOrders		=	BtGetAllOrdersTimeout;


#ifdef CountOrdersSent
		OrdersCount								=	0;
#endif

		prefs->current.course				=	NoValue16;
		prefs->current.recMenuIndex		=	NoValue16;
		prefs->current.recIndex				=	0;
		prefs->current.dishesServingCourseDefault	=	NoValue16;
		prefs->current.drinksServingCourseDefault	=	NoValue16;
		
		prefs->canScrollUp					=	false;
		prefs->sendLogEnabled				=	false;
		prefs->msgLogEnabled					=	false;
		prefs->rememberLastServCourse		=	false;
		prefs->isCashMode						=	false;

		prefs->current.fFontID				=	largeFont;

		prefs->current.tableNo				=	1;
		prefs->current.seatNo				=	1;
		prefs->staffIDNo						=	0;
		prefs->numUnsentOrders				=	0;
		prefs->current.setMenuGroup	 	= 	SetMenuGroupMinValue;

		prefs->current.mMenu					=	DishesMenu;
		prefs->current.memoRec				=	0;
		prefs->staffIDIndex					=	0;
		prefs->transactionNo					=	0;
		prefs->menuSyncNo						=	0;
		prefs->drinkSyncNo					=	0;
		prefs->infoSyncNo						=	0;

		prefs->loggedIn						=	false;
		prefs->userLogOffMins				=	0;
		prefs->roundingEnabled				=	1;
		prefs->billingIsPaid					=	0;
		
		prefs->current.setMenuMask			=	0;
		prefs->autoLogoutSecs				=	0;
		prefs->logoutTimeSecs				=	NoValue32;

		StrCopy(prefs->dishesMenuName, "No Food Menu");
		StrCopy(prefs->drinksMenuName, "No Drinks Menu");
		StrCopy(prefs->defaultSeatname, "");
		StrCopy(prefs->pIN, "    ");

		for(i=0; i<MaxRecentNotesListItems; i++)
		{
			prefs->recentNotes[i][0]		=	0;
		}

		MmResetHLStatusToAllDone(prefs, KeepNone);

#ifdef ReleaseVersion
		prefs->userSettings.advancedUser	=	false;
		prefs->userSettings.priceAdjust	=	true;	//	Temporary; will be set by Login later
		prefs->userSettings.autoDetails	=	false;
		prefs->userSettings.isLargeFont	=	true;
		prefs->userSettings.screenLockOn	=	true;
		prefs->userSettings.classicMode	=	false;
		prefs->userSettings.soundLevel.soundLevel0	=	false;
		prefs->userSettings.soundLevel.soundLevel1	=	true;
		prefs->userSettings.getAllTables	=	true;
#else
		prefs->userSettings.advancedUser	=	false;
		prefs->userSettings.priceAdjust	=	true;
		prefs->userSettings.autoDetails	=	false;
		prefs->userSettings.isLargeFont	=	true;
		prefs->userSettings.screenLockOn	=	false;
		prefs->userSettings.classicMode	=	false;
		prefs->userSettings.soundLevel.soundLevel0	=	false;
		prefs->userSettings.soundLevel.soundLevel1	=	true;
		prefs->userSettings.getAllTables	=	true;
#endif


		regTemp->autoReRegister = true;

		FntSetFont(prefs->current.fFontID);
	}





	if(*initialiseMode & InitKeepRegistration)
	{
		// Restore the registration name and number
		srcePtr = (UInt8*)&palmRegistrationName;
		destPtr = (UInt8*)&prefs->palmRegistrationName;
		for(i=0; i<sizeof(prefs->palmRegistrationName); i++)
		{
			*(destPtr++) = *(srcePtr++);
		}

		prefs->palmUniqueID = palmUniqueID;
	}



	if(*initialiseMode & InitKeepBluetoothNames)
	{
		//	Restore all the Bluetooth names etc.
		srcePtr = (UInt8*)&btDevices[0];
		destPtr = (UInt8*)&prefs->btDevices[0];
		for(i=0; i<MaxBtTransceivers; i++)
		{
			for(j=0; j<sizeof(BtLibDeviceAddressType); j++)
			{
				*(destPtr++) = *(srcePtr++);
			}
		}

		for(i=0; i<MaxBtTransceivers; i++)
		{
		   StrCopy(prefs->btNamesBuffer[i], btNamesBuffer[i]);
		   prefs->btName[i].name			=	btName[i].name;
		   prefs->btName[i].nameLength	=	btName[i].nameLength;
		}
		StrCopy(prefs->btSelectedDeviceName, btSelectedDeviceName);

		prefs->btNumDevices					=	btNumDevices;
		prefs->btDevice						=	btDevice;

		prefs->btTimeoutInterByte			=	btTimeoutInterByte;
		prefs->btTimeoutFirstByte			=	btTimeoutFirstByte;
		prefs->btTimeoutSendOrders			=	btTimeoutSendOrders;
		prefs->btTimeoutGetAllOrders		=	btTimeoutGetAllOrders;
	}

	//	Fill in the prefs->verNumStrShort to send to POS. Eg Ver 3.2.0.1 -> 3.2
	StrCopy(prefs->verNumStr, VersionNumberString);

	for(i=0, j=0; i<VerNumStrShortLength; i++, j++)
	{
		if(prefs->verNumStr[j] == '.')
			pointCount++;

		if(pointCount<2)
			prefs->verNumStrShort[j] = prefs->verNumStr[i];
		else
			prefs->verNumStrShort[j] = 0;	//	Pad version number with trailing zeros
	}



	

	//	Find and Load Bluetooth library
	err = SysLibFind(btLibName, btLibRefNum);

	if(err)
	{
		// Load it if it can't be found	
		err = SysLibLoad(sysFileTLibrary, sysFileCBtLib, btLibRefNum) ;
	}
	
	if(err == errNone)
	{
		//	Open the library
		err = BtOpenCloseLib(btLibRefNum, BtOpenOnly);
		
		//	Preserve the BtPalmAddress in a global
		err = BtLibGetGeneralPreference(*btLibRefNum, btLibPref_LocalDeviceAddress,
												&prefs->btPalmAddress, sizeof(prefs->btPalmAddress));
		
		//	Close the library
		err = BtOpenCloseLib(btLibRefNum, BtCloseOnly);

		err = BtLibAddrBtdToA(*btLibRefNum, &prefs->btPalmAddress, prefs->palmUniqueName,
																	sizeof(prefs->palmUniqueName));
	}
	else
	{
		StrCopy(prefs->palmUniqueName, "00:00:00:00:00:00");

		//	Zero the Prefs.btPalmAddress
		destPtr = (UInt8*)&prefs->btPalmAddress;

		for(j=0; j<sizeof(BtLibDeviceAddressType); j++)
		{
			*(destPtr++) = 0;
		}

#ifdef ReleaseVersion
		// Could not find BtLib;
		ShowMessage("Bluetooth Library not installed.\n\nBluetooth will not operate.");
#endif
	}


	for(i = 0; i < MaxBtTransceivers; i++)
	{
		prefs->btName[i].name = (UInt8 *)prefs->btNamesBuffer[i];
	}
}



/************************************************************
 *
 *  FUNCTION: MmInitAppInfo
 *
 *  DESCRIPTION: P11. Create an app info chunk if missing.  Set
 *		the category strings to a default.
 *
 *  PARAMETERS: database pointer
 *
 *  RETURNS: 0 if successful, errorcode if not
 *
 *************************************************************/
Err MmInitAppInfo(DmOpenRef dbP)
{
	UInt16						cardNo;
	MemHandle					h;
	LocalID						dbID;
	LocalID						appInfoID;
	MmAppInfoPtr				appInfoP;
	MmAppInfoPtr				nilP = 0;

	
	// We have a DmOpenRef and we want the database's app info block ID.
	// Get the database's dbID and cardNo and then use them to get the appInfoID.
	if (DmOpenDatabaseInfo(dbP, &dbID, NULL, NULL, &cardNo, NULL))
		return dmErrInvalidParam;
	if (DmDatabaseInfo(cardNo, dbID, NULL, NULL, NULL, NULL, NULL, 
			NULL, NULL, &appInfoID, NULL, NULL, NULL))
		return dmErrInvalidParam;


	// If no appInfoID exists then we must create a new one.	
	if (appInfoID == NULL)
	{
		h = DmNewHandle (dbP, sizeof (MmAppInfoType));
		if (! h) return dmErrMemError;
		
		appInfoID = MemHandleToLocalID (h);
		DmSetDatabaseInfo(cardNo, dbID, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &appInfoID, NULL, NULL, NULL);
	}


	// Lock the appInfoID and copy in defaults from our default structure.	
	appInfoP = MemLocalIDToLockedPtr(appInfoID, cardNo);

	// Clear the app info block.
	DmSet (appInfoP, 0, sizeof (MmAppInfoType), 0);

	// Initialize the categories.
	CategoryInitialize ((AppInfoPtr) appInfoP, LocalizedAppInfoStr);

	// Initialize reserved2.  This is actually redundant since the DmSet
	// call above has already set reserved2 to zero.  But this shows how
	// to set variables in addition to the categories.
	DmSet (appInfoP, (UInt32)&nilP->reserved2, sizeof(appInfoP->reserved2), 0);

	MemPtrUnlock(appInfoP);
	return 0;
}


/***********************************************************************
 *
 * FUNCTION:    atoh
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/
UInt32 MmAToH(char *string)
{
	Int16					i;
	Int32 				l = StrLen(string);
	UInt32			 	hex = 0L;
	UInt32				factor = 1L;
	if(l <= 8)
	{
		for(i=l-1; i >= 0; i--)
		{
			if((string[i]>='0') && (string[i]<='9'))
			{
				hex += (string[i]-'0') * factor;
				factor *= 0x10;
			}
			if((string[i] >= 'A') && (string[i] <= 'F'))
			{
				hex += (string[i] - 'A' + 0xA) * factor;
				factor *= 0x10;
			}
		}
	}
	return(hex);
}



/***********************************************************************
 *
 * FUNCTION:	  ResetHLStatusToAllDone
 *
 * DESCRIPTION:  Reset each of the Status flags unless input bit is set.
 *
 * PARAMETERS:	
 *
 * RETURNED:	  
 *
 ***********************************************************************/
void MmResetHLStatusToAllDone(MmPreferenceTypePtr prefs, UInt16 flags)
{
	if(!(flags & KeepRegister))
		prefs->hLStatus.HLRegisterStatus 			= HLStatusAllDone;

	if(!(flags & KeepInfoSync))
		prefs->hLStatus.HLInfoSyncStatus 			= HLStatusAllDone;

	if(!(flags & KeepMenuSync))
		prefs->hLStatus.HLMenuSyncStatus				= HLStatusAllDone;

	if(!(flags & KeepDrinkSync))
		prefs->hLStatus.HLDrinkSyncStatus			= HLStatusAllDone;

	if(!(flags & KeepLogin))
		prefs->hLStatus.HLLoginStatus 				= HLStatusAllDone;
	else if((flags & KeepLogin) && (prefs->hLStatus.HLLoginStatus != HLStatusAllDone))
		prefs->hLStatus.HLLoginStatus					= HLStatusToSend;

	if(!(flags & KeepSendSeatnames))
		prefs->hLStatus.HLSendSeatnamesStatus		= HLStatusAllDone;

	if(!(flags & KeepBtTimeout))
		prefs->hLStatus.HLBtTimeoutStatus		 	= HLStatusAllDone;

	if(!(flags & KeepSendOrders))
		prefs->hLStatus.HLSendOrdersStatus 			= HLStatusAllDone;

	if(!(flags & KeepCallAway))
		prefs->hLStatus.HLCallAwayStatus 			= HLStatusAllDone;

	if(!(flags & KeepBillSeats))
		prefs->hLStatus.HLBillSeatsStatus 			= HLStatusAllDone;

	if(!(flags & KeepTransferTables))
		prefs->hLStatus.HLTransferTablesStatus 	= HLStatusAllDone;

	if(!(flags & KeepGetAllOrders))
		prefs->hLStatus.HLGetAllOrdersStatus 		= HLStatusAllDone;
}




/***********************************************************************
 *
 * FUNCTION:		MmWriteThisBillingAction
 *
 * DESCRIPTION:	Writes groupNumber to Table/Seat record.
 *
 * PARAMETERS:		If recIndex == NoValue16 then a search is performed
 *						using Prefs.current.tableNo and thisSeat as passed.
 *
 *						If groupNumber == NoValue16 then Prefs.tableBillGroup
 *						will be used, else the value passed is used. NB. groupNumber
 *						may be 0 to cancel existing group association
 *
 *
 * RETURNED:		Returns Boolean indicating whether Seat write successful.
 *
 ***********************************************************************/
void MmWriteThisBillingAction(UInt16 recIndex, UInt16 action, DmOpenRef ordersDB)
{
	MemHandle				recHandle;
	OrderTypePtr			recPtr;
	Err						error;
	UInt8						thisBillingAction;

	
	recHandle = DmGetRecord(ordersDB, recIndex);
	if(recHandle)
	{
		recPtr = MemHandleLock(recHandle);

		thisBillingAction = recPtr->billing;

		if(action == BillingActionSelectThis		||
			action == BillingActionSelectTable		)
		{
			thisBillingAction |= BillingSeatMarked;
		}
		else //if(action == BillingActionUnselectThis or BillingActionCancelAll)
		{
			thisBillingAction &= ~BillingSeatMarked;
		}

		//	Write the billing action
		error = DmWrite(recPtr, (UInt8*)&recPtr->billing - (UInt8*)&recPtr->tableNumber,
											&thisBillingAction, sizeof(recPtr->billing));
		ErrFatalDisplayIf(error, "Could not write Billing Action to record.");


		MemPtrUnlock(recPtr);
		DmReleaseRecord(ordersDB, recIndex, true);
	}
}


/***********************************************************************
 *
 * FUNCTION:		MmBillingOperations
 *
 * DESCRIPTION:	Go through all records for this Table.
 *
 *						If thisGroup = 0 'select' the Marker and assign it group
 *						number 0.
 *
 *						Then deselect, mark as billed, and assign it group number
 *						Prefs.tableBillGroup.
 *
 *
 * PARAMETERS:		
 *
 * RETURNED:		
 *
 ***********************************************************************/
UInt16 MmBillingOperations(UInt16 action, DmOpenRef ordersDB, 
																	CurrentPrefsTypePtr prefs_current)
{
	MemHandle				recHandle;

	UInt16					recIndex;

	OrderTypePtr			recPtr;

	UInt16					numberOfRecs;
	
	UInt8						billing;
	UInt16					tableNumber;



	numberOfRecs = DmNumRecords(ordersDB);
	
	if(action == BillingActionConfirmAll	||
		action == BillingActionFindFirstInDB	)
	{
		recIndex = 0;
	}
	else
	{
		recIndex = MmFindSortTableSeat(prefs_current->tableNo, 1);	//	Find first Marker record
	}
	for(;recIndex < numberOfRecs; recIndex++)
	{
		recHandle = DmQueryNextInCategory (ordersDB, &recIndex, MarkerItem);
		if(recHandle)
		{
			if(action == BillingActionConfirmAll)
			{
				MmWriteThisBillingAction(recIndex, BillingActionUnselectThis, ordersDB);
			}
			else
			{
				//	Save stuff we'll need for later, so we can MemPtrUnlock
				recPtr = MemHandleLock(recHandle);
				tableNumber = recPtr->tableNumber;
				billing = recPtr->billing;
				MemPtrUnlock(recPtr);

				if(action == BillingActionFindFirstInDB)
				{
					if(billing & BillingSeatMarked)
						return recIndex;
				}

				else if(tableNumber == prefs_current->tableNo)
				{
					//	If a record exists for this seat, 'select' (or 'deselect') the Marker record
					switch(action)
					{
							//	Do Billing stuff, if applicable
						case BillingActionSelectTable:
							MmWriteThisBillingAction(recIndex, BillingActionSelectThis, ordersDB);
							break;
							
						case BillingActionCancelTable:
							MmWriteThisBillingAction(recIndex, BillingActionUnselectThis, ordersDB);
							break;
							
						case BillingActionFindFirstInTable:
							if(billing & BillingSeatMarked)
								return recIndex;
							break;
					}
				}
				else
				{
					recIndex = numberOfRecs;				//	Time to quit
				}
			}
		}
	}
	
	return NoValue16;
}





/***********************************************************************
 *
 * FUNCTION:    MmCopyCurrentOrderIndicesToRecord()
 *
 * DESCRIPTION: Copy the Prefs.curOrder DrinkMix Indices to Prefs.current.record.
 *
 * PARAMETERS:  .
 *
 * RETURNED:    .
 *
 ***********************************************************************/
void MmCopyCurrentOrderIndicesToRecord(DmOpenRef ordersDB, MmPreferenceTypePtr prefs)
//														UInt16 prefs_curRecIndex, CurrentOrderTypePtr prefs_curOrder
{
	MemHandle 					recHandle;
	OrderTypePtr				recPtr;

//	MenuTypePtr					recMenuPtr;
//	MemHandle 					recMenuHandle;

	Err							err;
	UInt16						i;									//	Local Counter
		
	//	Check if record exists before we Get it for writing
	recHandle = DmQueryRecord(ordersDB, prefs->current.recIndex);
	if(recHandle)
	{
		recHandle = DmGetRecord(ordersDB, prefs->current.recIndex);
		recPtr = MemHandleLock(recHandle);

		//	Write the new indices to DB record
		for(i=0; i < MaxLinkedItems; i++)	//	Write each in turn
		{
			//	Write each of the Indices
			err = DmWrite(recPtr,
						(UInt8*)&recPtr->menuIndex[i].index - (UInt8*)&recPtr->tableNumber,
						&prefs->curOrder.menuIndex[i].index,
						sizeof(prefs->curOrder.menuIndex[0].index));
			ErrFatalDisplayIf(err, "Could not write Linked Item Index to record.");

			//	Write each of the Size Types
			err = DmWrite(recPtr,
						(UInt8*)&recPtr->menuIndex[i].size - (UInt8*)&recPtr->tableNumber,
						&prefs->curOrder.menuIndex[i].size,
						sizeof(prefs->curOrder.menuIndex[0].size));
			ErrFatalDisplayIf(err, "Could not write Linked Item Size Type to record.");
		}

		//	Write the main Size Type
		err = DmWrite(recPtr,
						(UInt8*)&recPtr->size - (UInt8*)&recPtr->tableNumber,
						&prefs->curOrder.size, sizeof(prefs->curOrder.size));
		ErrFatalDisplayIf(err, "Could not write size type to record.");

		//	Write the price
		err = DmWrite(recPtr,
						(UInt8*)&recPtr->price - (UInt8*)&recPtr->tableNumber,
						&prefs->curOrder.price, sizeof(prefs->curOrder.price));
		ErrFatalDisplayIf(err, "Could not write price to record.");

		//	Write the servingCourse
		err = DmWrite(recPtr,
						(UInt8*)&recPtr->servingCourseID - (UInt8*)&recPtr->tableNumber,
						&prefs->curOrder.servingCourseID, sizeof(prefs->curOrder.servingCourseID));
		ErrFatalDisplayIf(err, "Could not write Serving Course to record.");

		MemPtrUnlock(recPtr);
		DmReleaseRecord(ordersDB, prefs->current.recIndex, true);
	}
}





/***********************************************************************
 *
 * FUNCTION:    MmLoadSelectList
 *
 * DESCRIPTION: Loads the database records into the Dishes list by
 *              writing pointers to strings into the list pointer array.
 *						Note: Offset allows the index to start further down the list
 *						Eg. If the Staff come after the Table names then offset steps
 *						over the Tables.
 *					 For CourseForcedOptions offset holds group bit position
 *
 * PARAMETERS:  frm - form containing list to load
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
UInt16 MmLoadSelectList(DBTypePtr dBs, MmPreferenceTypePtr prefs, UInt16 listType,
									UInt16* offset, Boolean* nextButton, UInt16* maxSelect,
										Char* *listItems, Char* *sizeListItems,
											ListStructTypePtr listItemsStruct,
												UInt16* sizeListIndices)
{
	UInt16					items;					//	Counter to add stuff to the list
	UInt16					i;							//	Counter to step through list items
	MemHandle				recHandle;
	MemHandle				recSidesHandle;

	OrderTypePtr			recOrderPtr;
	MenuTypePtr				recMenuPtr;
	MenuTypePtr				recCoursePtr;
	OptionsTypePtr			recOptionsPtr;
	ForcedSidesTypePtr	recForcedSidesPtr;
	SizeTypePtr				recSizePtr;
	SeatnameTypePtr		recTablePtr;
	SeatnameTypePtr		recSeatsPtr;
	InfoTypePtr				recInfoPtr;
	MiscDataTypePtr		recMiscDataPtr;
	ServingCourseTypePtr	recServingCoursePtr;

	UInt16					recIndex;				//	Index through the database
	UInt16					recMiscDataIndex;
	UInt16					recTablenameIndex;
	UInt16					recSeatnameIndex;
	UInt16					recForcedSidesIndex;

	UInt16					numServingCourseRecords;
	UInt16					numMenuRecords;
	UInt16					numOrderRecords;
	UInt16					numSizeRecords;
	UInt16					numSeatnameRecords;
	UInt16					numTablenameRecords;
	UInt16					numForcedSidesRecords;

	UInt16					thisTable = 0;
	UInt16					thisSeat = 0;
	UInt16					thisItemNumber;
	UInt32					itemInCourse;
	
	Boolean					doDisplay;
	Boolean					showNextButton;
	Boolean					firstPassFlag = true;

	Char						tempStr[MaxListItems];

	DmOpenRef				menuDB;
	DmOpenRef				menuSizesDB;
	DmOpenRef				optionsDB;
	DmOpenRef				forcedSidesDB;




	if(prefs->current.mMenu == DishesMenu)
	{
		menuDB			= dBs->mmDishesDB;
		menuSizesDB		= dBs->mmDishesSizesDB;
		optionsDB		= dBs->mmDishesOptionsDB;
		forcedSidesDB	= dBs->mmDishesForcedSidesDB;
	}
	else
	{
		menuDB			= dBs->mmDrinksDB;
		menuSizesDB		= dBs->mmDrinksSizesDB;
		optionsDB		= dBs->mmDrinksOptionsDB;
		forcedSidesDB	= dBs->mmDrinksForcedSidesDB;
	}
	

	

	// For each item in the list, store the record number in the list item that will
	//	display the record. Get the next record in the current category (ie Course).
	switch(listType)
	{
		case DisplayViewList:
			items = *offset;
			break;


		case ServingCourseAllList:
//		case ServingCourseAllSelctblList:
		case ServingCourseDishesList:
		case ServingCourseDrinksList:
			numServingCourseRecords = DmNumRecords(dBs->mmServingCourseDB);

			for(items=0, recIndex=1; items < MaxListItems && recIndex < numServingCourseRecords; recIndex++)
			{		
				recHandle = DmQueryRecord (dBs->mmServingCourseDB, recIndex);
				// If record is found, store the pointer to the item string in the list array.
				if(recHandle)
				{
					recServingCoursePtr = MemHandleLock(recHandle);

					//	Save recIndex and point to the item's text string in the database
					if(listType == ServingCourseAllList
						||
//						listType == ServingCourseAllSelctblList
//						||
						(listType == ServingCourseDishesList && recServingCoursePtr->menuType == DishesMenu)
						||
						(listType == ServingCourseDrinksList && recServingCoursePtr->menuType == DrinksMenu)
																																)
					{
						if(listType == ServingCourseAllList
							||
							recServingCoursePtr->selectable	)
						{
							listItemsStruct[items].recIndex = recIndex;
//							listItemsStruct[items].recIndex = recServingCoursePtr->uniqueID;
							listItems[items++] = recServingCoursePtr->standardName;
						}
					}

					MemPtrUnlock(recServingCoursePtr);
				}
				else				//	No record MemHandle = we're finished!
				{
					break;
				}
			}
			break;


		case CoursesList:
			numMenuRecords = DmNumRecords(menuDB);

			for(items=0, recIndex=0; items < MaxListItems && recIndex < numMenuRecords; recIndex++)
			{		
				recHandle = DmQueryNextInCategory (menuDB, &recIndex, MarkerItem);
				// If record is found, store the pointer to the item string in the list array.
				if(recHandle)
				{
					recCoursePtr = MemHandleLock(recHandle);

					//	Save recIndex and point to the item's text string in the database
					listItemsStruct[items].recIndex = recIndex;
					if(recCoursePtr->enabled == StatusAsEnabled)
						listItems[items++] = recCoursePtr->itemName;
					else
						listItems[items++] = recCoursePtr->disabledItemName;
					MemPtrUnlock(recCoursePtr);
				}
				else				//	No record MemHandle = we're finished!
				{
					break;
				}
			}
			break;


		case ItemsList:
		case ItemsListNotSetMenyKey:
			numMenuRecords = DmNumRecords(menuDB);

			//	Find the first item in the Course
			recIndex = MmFindDishOrDrinkIndex(menuDB, prefs->current.course, 0);

			//	Now find all the rest
			for(items=0; items < MaxListItems && recIndex < numMenuRecords; recIndex++)
			{		
				recHandle = DmQueryRecord(menuDB, recIndex);

				// If record is found, store the pointer to the item string in the list array.
				if(recHandle)
				{
					recMenuPtr = MemHandleLock(recHandle);

					if(recMenuPtr->courseNumber > prefs->current.course)
					{
						MemPtrUnlock(recMenuPtr);
						break;
					}
					else
					{
						if(listType == ItemsListNotSetMenyKey || recMenuPtr->onlyAsSide == false)
						{
							//	Search through sizes looking for even one thats available.
							for(i=0, doDisplay = false; i < MaxSizeTypes; i++)
							{		
								if(recMenuPtr->sizes[i].mayBeStdItem)
								{
									doDisplay = true;
									break;
								}
							}

							if(doDisplay)
							{
								//	Only show if its not a SetMenu Key item
								if( ! (listType == ItemsListNotSetMenyKey &&
										(recMenuPtr->sizes[i].setMenuMask & SetMenuKey)))
								{
									//	Save recIndex and point to the item's text string in the database
									listItemsStruct[items].recIndex = recIndex;

									//	Point to the item's text string in the database
									if(recMenuPtr->enabled == StatusAsEnabled)
										listItems[items++] = recMenuPtr->itemName;
									else
										listItems[items++] = recMenuPtr->disabledItemName;
								}
							}
						}
						MemPtrUnlock(recMenuPtr);
					}
				}
				else				//	No record MemHandle = we're finished!
				{
					break;
				}
			}

			break;


		case SetMenuItemsList:
			numMenuRecords = DmNumRecords(menuDB);

			//	Make first item 'Delete'
			items = 0;
			listItemsStruct[items].recIndex = 0;
			listItems[items] = "* Delete This Key Item *";
			thisItemNumber = 0;

			for(items=1, recIndex=0; items < MaxListItems && recIndex < numMenuRecords; recIndex++)
			{		
				recHandle = DmQueryRecord(menuDB, recIndex);
				// If record is found, store the pointer to the item string in the list array.
				if(recHandle)
				{
					recMenuPtr = MemHandleLock(recHandle);

					if(recMenuPtr->onlyAsSide == false)
					{
						for(i=0; i < MaxLinkedItems; i++)
						{
							//	Bail out on the last one
							if(recMenuPtr->sizes[i].size == (UInt16)NoValue16)
								break;

							if( (recMenuPtr->sizes[i].setMenuMask & prefs->current.setMenuMask) &&	
								!(recMenuPtr->sizes[i].setMenuMask & SetMenuKey)	&&
								 (recMenuPtr->itemNumber != thisItemNumber)						)
							{
								thisItemNumber = recMenuPtr->itemNumber;
								
								//	Save recIndex and point to the item's text string in the database
								listItemsStruct[items].recIndex = recIndex;

								//	Point to the item's text string in the database
								if(recMenuPtr->enabled == StatusAsEnabled)
								{
									listItems[items] = recMenuPtr->itemName;
								}
								else
								{
									listItems[items] = recMenuPtr->disabledItemName;
								}
								items++;
							}
						}
					}
					MemPtrUnlock(recMenuPtr);
				}
				else				//	No record MemHandle = we're finished!
				{
					break;
				}
			}
			break;



		case SizeList:
		case SetMenuSizeList:

			numSizeRecords = DmNumRecords(menuSizesDB);
			
			recHandle = DmQueryRecord(menuDB, prefs->current.recMenuIndex);
			if(recHandle)
			{
				//	This is our Menu item, with its prices
				recMenuPtr = MemHandleLock(recHandle);

				//	Go through the Size DB loading the applicable Size types
				for(items=0, i=0; items < MaxListItems/* && i < MaxSizeTypes*/; i++)
				{
					//	Bail out on the last one
					if(recMenuPtr->sizes[i].size == (UInt16)NoValue16)
						break;

					if((listType == SizeList												&&
						 recMenuPtr->sizes[i].mayBeStdItem) 								||
						(listType == SetMenuSizeList)										&&
						(recMenuPtr->sizes[i].setMenuMask & prefs->current.setMenuMask)	&&
						!(recMenuPtr->sizes[i].setMenuMask & SetMenuKey)					)
					{
						recHandle = DmQueryRecord(menuSizesDB,
													recMenuPtr->sizes[i].size);
						// If record found, store pointer to the item string in the list array.
						if(recHandle)
						{
							recSizePtr = MemHandleLock(recHandle);
							//	Point to the item's text string in the database
							sizeListItems[items] = recSizePtr->sizeName;
							sizeListIndices[items] = recMenuPtr->sizes[i].size;
							items++;
							MemPtrUnlock(recSizePtr);
						}
					}
				}	//	for()

				MemPtrUnlock(recMenuPtr);
			}

			break;


		case AllSizeList:
			numMenuRecords = DmNumRecords(menuSizesDB);
			
			//	Go through the Size DB loading the applicable Size types
			for(items=0, recIndex=1; items < MaxListItems && recIndex < numMenuRecords; recIndex++)
			{
				recHandle = DmQueryRecord(menuSizesDB, recIndex);
				// If record is found, store the pointer to the item string in the list array.
				if(recHandle)
				{
					recSizePtr = MemHandleLock(recHandle);
					//	Point to the item's text string in the database
					sizeListItems[items] = recSizePtr->sizeName;
					sizeListIndices[items] = recIndex;
					items++;
					MemPtrUnlock(recSizePtr);
				}
			}	//	for()

			break;



		case CourseOptionsPlus:
		case CourseOptionsMinus:

			numMenuRecords = DmNumRecords(optionsDB);

			//	Find the first item in the Course
			recIndex = MmFindCourseOptionIndex(optionsDB, prefs->current.course, 1);

			for(items=0; items < MaxOptionsItems && recIndex < numMenuRecords; recIndex++)
			{		
				recHandle = DmQueryRecord(optionsDB, recIndex);

				// If record is found, store the pointer to the item string in the list array.
				if(recHandle)
				{
					recOptionsPtr = MemHandleLock(recHandle);

					if(recOptionsPtr->courseNumber != prefs->current.course)
					{
						MemPtrUnlock(recOptionsPtr);
						break;
					}
					else if(	listType==CourseOptionsPlus  							&&
								(recOptionsPtr->optionFlags & OptionsPlusBit)		)
					{
						//	Point to the item's text string in the database
						listItems[items] = recOptionsPtr->optionNameFit;

						//	Save recIndex and point to the item's text string in the database
						listItemsStruct[items].recIndex = recIndex;
						items++;
						MemPtrUnlock(recOptionsPtr);
					}
					else if(	listType==CourseOptionsMinus						&&
								(recOptionsPtr->optionFlags & OptionsMinusBit)		)
					{
						//	Point to the item's text string in the database
						listItems[items] = recOptionsPtr->optionMinusNameFit;

						//	Save recIndex and point to the item's text string in the database
						listItemsStruct[items].recIndex = recIndex;
						items++;
						MemPtrUnlock(recOptionsPtr);
					}
					else
					{
						MemPtrUnlock(recOptionsPtr);
					}
				}
				else				//	No record MemHandle = we're finished!
				{
					break;
				}
			}

			break;




		case CourseForcedOptions:

			numMenuRecords = DmNumRecords(optionsDB);
			*nextButton = false;			//	Set default
			*maxSelect = 0;

			//	Find the first item in the Course
			recIndex = MmFindCourseOptionIndex(optionsDB, prefs->curOrder.courseNumber, 1);

			//	The optionItemMask identifies whether this item is applicable
			//	Bit shift to find the itemInCourse bit
			for(i=1, itemInCourse=1; i < prefs->curOrder.itemInCourse; i++)
			{
				itemInCourse<<=1;
			}
			
			for(items=0; items < MaxOptionsItems && recIndex < numMenuRecords; recIndex++)
			{		
				recHandle = DmQueryRecord(optionsDB, recIndex);

				// If record is found, store the pointer to the item string in the list array.
				if(recHandle)
				{
					recOptionsPtr = MemHandleLock(recHandle);


					if(recOptionsPtr->courseNumber != prefs->curOrder.courseNumber)
					{
						MemPtrUnlock(recOptionsPtr);
						break;
					}


					else if((recOptionsPtr->optionFlags & 0x0F) == *offset)	//	offset contains 'group'
					{
						if(firstPassFlag)
						{
							if(recOptionsPtr->optionFlags & OptionsNotMultiSelectBit)
								*maxSelect = 1;
							else
								*maxSelect = NoValue16;

							//	Always force display 'Next' if Multi-Select is required
							if(*maxSelect != 1
								||
								recOptionsPtr->optionFlags & OptionsNextButtonBit)
								showNextButton = true;
							else
								showNextButton = false;
						}


						//	The optionItemMask identifies whether this item is applicable
						if(recOptionsPtr->optionItemMask & itemInCourse)
						{
							if(showNextButton)
							{
								showNextButton = false;
								*nextButton = true;

								//	Make first item 'Next'
								listItemsStruct[items].recIndex = 0;
								listItems[items] = "    ***  Next  ***    ";
								items++;
							}


							//	Save recIndex and point to the item's text string in the database
							listItemsStruct[items].recIndex = recIndex;

							//	Point to the item's text string in the database
							if(listItemsStruct[items].selected)
								listItems[items] = recOptionsPtr->optionSelectedNameFit;
							else
								listItems[items] = recOptionsPtr->optionNameFit;

							items++;
						}
						firstPassFlag = false;
						MemPtrUnlock(recOptionsPtr);
					}
					else
					{
						MemPtrUnlock(recOptionsPtr);
					}
				}
				else				//	No record MemHandle = we're finished!
				{
					break;
				}
			}

			break;



		case ForcedSidesList:
			numMenuRecords = DmNumRecords(menuDB);
			numForcedSidesRecords = DmNumRecords(forcedSidesDB);

			thisItemNumber = 0;
			*nextButton = false;			//	Set default
			*maxSelect = 0;

			recHandle = DmQueryRecord(menuDB, prefs->curOrder.menuIndex[0].index);
			// Menu item contains index to start of Forced Sides for this item.
			if(recHandle)
			{
				recMenuPtr = MemHandleLock(recHandle);
				thisItemNumber = recMenuPtr->itemNumber;
				recForcedSidesIndex = recMenuPtr->forcedSidesIndex;
				MemPtrUnlock(recMenuPtr);
				
				//	Find and list any Forced Sides in this group
				for(items=0; items < MaxListItems && recForcedSidesIndex < numForcedSidesRecords;
																			recForcedSidesIndex++)
				{		
					recSidesHandle = DmQueryRecord(forcedSidesDB, recForcedSidesIndex);
					// If record is found, store the pointer to the item string in the list array.
					if(recSidesHandle)
					{
						recForcedSidesPtr = MemHandleLock(recSidesHandle);
							
						if(recForcedSidesPtr->masterItem == thisItemNumber)
						{
							if(recForcedSidesPtr->groupNumber == *offset)
							{
//"FFGarlic Herbed Bread|1|3|4|,1|T1|1|3|10|,2|T0|4|9|12|,4|T0|1|3|4|5|22|\n"
//"TFAngus T-Bone|1|,1|F1|1|3|10|,2|T1|4|9|12|,3|F2|1|3|4|5|22|\n"

								recHandle = DmQueryRecord(menuDB, recForcedSidesPtr->itemIndex & IsNotDrinkMask);
								// Menu item contains index to start of Forced Sides for this item.
								if(recHandle)
								{
									if(firstPassFlag)
									{
										firstPassFlag = false;

										if(recForcedSidesPtr->nextButton)
										{
											//	Make first item 'Next'
											listItemsStruct[items].recIndex = 0;
											listItems[items] = "    ***  Next  ***    ";
											items++;
										}
										
										*nextButton = recForcedSidesPtr->nextButton;
										*maxSelect = recForcedSidesPtr->maxSelect;
									}
									
									recMenuPtr = MemHandleLock(recHandle);

									//	Save recIndex and point to the item's text string in the database
									listItemsStruct[items].recIndex = recForcedSidesPtr->itemIndex & IsNotDrinkMask;

									//	Point to the item's text string in the database
									if(listItemsStruct[items].selected)
										listItems[items] = recMenuPtr->disabledItemName;
									else
										listItems[items] = recMenuPtr->itemName;

									items++;

									MemPtrUnlock(recMenuPtr);
									MemPtrUnlock(recForcedSidesPtr);
								}
								else	//	No record MemHandle = we're finished
								{
									MemPtrUnlock(recForcedSidesPtr);
									break;
								}
							}
							else	//	Not one of our group yet, but keep looking
							{
								MemPtrUnlock(recForcedSidesPtr);
							}
						}
						else	//	Not one of our group, so quit
						{
							MemPtrUnlock(recForcedSidesPtr);
							break;
						}
					}
					else	//	No record MemHandle = we're finished
					{
						break;
					}
				}		//	for()
			}
			break;



		case SeatsList:
			// For each item in the list, store the record number in the 
			// list item that will display the record.
			numMenuRecords = DmNumRecords(dBs->mmSeatnameDB);
			
			for(items=0, recIndex=1; items < MaxListItems && recIndex < numMenuRecords; recIndex++)
			{		
				recHandle = DmQueryRecord(dBs->mmSeatnameDB, recIndex);
				
				// If record is found, store the pointer to the item string in the list array.
				if(recHandle)
				{
					recSeatsPtr = MemHandleLock(recHandle);

					if(recSeatsPtr->tableNumber == prefs->current.tableNo)
					{
						//	Point to the item's text string in the database
//						ListItems[item++] = recSeatsPtr->nameFit;
						listItems[items] = recSeatsPtr->name;
						listItemsStruct[items].recIndex = recIndex;
						items++;
					}
					MemPtrUnlock(recSeatsPtr);
				}
			}
			break;


		case ActiveSeatsList:
			// For each item in the list, store a pointer to the name text string.
			numOrderRecords = DmNumRecords(dBs->mmOrdersDB);

			//	Find start of this Table
			recIndex = MmFindSortTableSeat(prefs->current.tableNo, 1);	//	Start at Seat 1

			for(items=0; items < MaxTables && recIndex < numOrderRecords; recIndex++)
			{		
				//	Find all the Marker records
				recHandle = DmQueryNextInCategory(dBs->mmOrdersDB, &recIndex, MarkerItem);
				
				// If record is found, store the pointer to the item string in the list array.
				if(recHandle)
				{
					recOrderPtr = MemHandleLock(recHandle);

					//	Only want this table
					if(recOrderPtr->tableNumber != prefs->current.tableNo	||
						recOrderPtr->tableNumber == LastTableRecord				)
					{
						MemPtrUnlock(recOrderPtr);
						break;
					}
					
					else
					{
						if(recOrderPtr->seatNumber != thisSeat)
						{
							thisSeat = recOrderPtr->seatNumber;

							//	The List pointers will point to respective objects in the
							//	ListItemsStruct[] array. These must be preloaded with
							//	Seat Number or Name strings, and their database indices in
							//	the case of Seat Names.

							listItemsStruct[items].recIndex = 0;	//	In case its not a Name

							numSeatnameRecords = DmNumRecords(dBs->mmSeatnameDB);

							for(recSeatnameIndex=1; recSeatnameIndex <= numSeatnameRecords;
																								recSeatnameIndex++)
							{		
								recHandle = DmQueryRecord(dBs->mmSeatnameDB, recSeatnameIndex);
								
								// If record is found, store the pointer to the item string in the list array.
								if(recHandle)
								{
									recSeatsPtr = MemHandleLock(recHandle);

									if(recSeatsPtr->tableNumber == prefs->current.tableNo	&&
										recSeatsPtr->seatNumber == recOrderPtr->seatNumber		)
									{
										//	Point to the item's text string in the database
										StrCopy(listItemsStruct[items].trimName, recSeatsPtr->name);
										listItems[items] = listItemsStruct[items].trimName;

										//	Store the actual table number in the recIndex field
										listItemsStruct[items].recIndex = recOrderPtr->seatNumber;
										MemPtrUnlock(recSeatsPtr);
										break;
									}
									else
									{
										MemPtrUnlock(recSeatsPtr);
									}
								}
							}

							if(listItemsStruct[items].recIndex == 0)
							{

								StrCopy(listItemsStruct[items].trimName, "Seat ");

								if(StrLen(prefs->defaultSeatname))
								{
									StrNCopy(listItemsStruct[items].trimName, prefs->defaultSeatname, MaxSeatLabelSize);	//	Limit size to button width
									StrCat(listItemsStruct[items].trimName, " ");
								}

								StrIToA(tempStr, recOrderPtr->seatNumber);
								StrCat(listItemsStruct[items].trimName, tempStr);

								listItems[items] = listItemsStruct[items].trimName;
								
								//	Store the actual seat number in the recIndex field
								listItemsStruct[items].recIndex = recOrderPtr->seatNumber;
							}
							items++;
						}
						MemPtrUnlock(recOrderPtr);
					}
				}
				else				//	No record MemHandle = we're finished!
				{
					break;
				}
			}

			break;



		case TablesList:
			// For each item in the list, store the record number in the 
			// list item that will display the record.
			numMenuRecords = DmNumRecords(dBs->mmTablenameDB);

			for(items=0, recIndex=1; items < MaxListItems && recIndex < numMenuRecords; recIndex++)
			{		
				recHandle = DmQueryRecord(dBs->mmTablenameDB, recIndex);
				
				// If record is found, store the pointer to the item string in the list array.
				if(recHandle)
				{
					recTablePtr = MemHandleLock(recHandle);

					//	Point to the item's text string in the database
					listItems[items++] = recTablePtr->name;

					MemPtrUnlock(recTablePtr);
				}
			}
			break;


		case ActiveTablesList:
			// For each item in the list, store a pointer to the name text string.
			numOrderRecords = DmNumRecords(dBs->mmOrdersDB);
			numTablenameRecords = DmNumRecords(dBs->mmTablenameDB);

			for(items=0, recIndex=0; items < MaxTables && recIndex < numOrderRecords; recIndex++)
			{		
				recHandle = DmQueryNextInCategory(dBs->mmOrdersDB, &recIndex, MarkerItem);
				
				// If record is found, store the pointer to the item string in the list array.
				if(recHandle)
				{
					recOrderPtr = MemHandleLock(recHandle);

					//	Only load table name once for each table
					if(recOrderPtr->tableNumber != thisTable	&&
						recOrderPtr->tableNumber != LastTableRecord				)
					{
						thisTable = recOrderPtr->tableNumber;

						//	The List pointers will point to respective objects in the
						//	ListItemsStruct[] array. These must now be with
						//	Table Number or Name strings, and their database indices in
						//	the case of Table Names.

						listItemsStruct[items].recIndex = 0;	//	In case its not a Name

						for(recTablenameIndex=1; recTablenameIndex < numTablenameRecords;
																									recTablenameIndex++)
						{		
							recHandle = DmQueryRecord(dBs->mmTablenameDB, recTablenameIndex);
							
							// If record is found, store the pointer to the item string in the list array.
							if(recHandle)
							{
								recTablePtr = MemHandleLock(recHandle);

								if(recTablePtr->tableNumber == recOrderPtr->tableNumber)
								{
									//	Point to the item's text string in the database
									StrCopy(listItemsStruct[items].trimName, recTablePtr->name);
									listItems[items] = listItemsStruct[items].trimName;

									//	Store the actual table number in the recIndex field
									listItemsStruct[items].recIndex = recOrderPtr->tableNumber;
									MemPtrUnlock(recTablePtr);
									break;
								}
								else
								{
									MemPtrUnlock(recTablePtr);
								}
							}
						}

						//	If there is no name for this table show 'Table xx' instead
						if(listItemsStruct[items].recIndex == 0)
						{
							StrIToA(tempStr, recOrderPtr->tableNumber);
							StrCopy(listItemsStruct[items].trimName, "Table ");
							StrCat(listItemsStruct[items].trimName, tempStr);

							listItems[items] = listItemsStruct[items].trimName;
							
							//	Store the actual table number in the recIndex field
							listItemsStruct[items].recIndex = recOrderPtr->tableNumber;
						}
						items++;
					}
					MemPtrUnlock(recOrderPtr);
				}
				else				//	No record MemHandle = we're finished!
				{
					break;
				}
			}
			break;


		case StaffList:
			// For each item in the list, store the record number in the 
			// list item that will display the record.
			numMenuRecords = DmNumRecords(dBs->mmInfoDB);

			*offset = 0;

			//	Make first item 'Refresh This List'
			listItemsStruct[0].recIndex = 0;
			listItems[0] = "* Refresh This List *";

			for(items=1, recIndex=1; items < MaxListItems && recIndex < numMenuRecords; recIndex++)
			{		
				recHandle = DmQueryRecord(dBs->mmInfoDB, recIndex);
				
				// If record is found, store the pointer to the item string in the list array.
				if(recHandle)
				{
					recInfoPtr = MemHandleLock(recHandle);

					//	Make sure its a Staff record but not the "Login" one
					if(recInfoPtr->recordType == StaffType &&
						StrCompare(recInfoPtr->name, "Login")	)
					{
						if(items == 1)
						{
							*offset = recIndex;				//	Do this once only
						}
						//	Point to the item's text string in the database
						listItems[items++] = recInfoPtr->name;
					}

					MemPtrUnlock(recInfoPtr);
				}
			}
			break;


		case PalmsList:
			// For each item in the list, store the record number in the 
			// list item that will display the record.
			numMenuRecords = DmNumRecords(dBs->mmInfoDB);

			*offset = 0;

			//	Make first item 'Refresh This List'
			listItemsStruct[0].recIndex = 0;
			listItems[0] = "* Refresh This List *";

			for(items=1, recIndex=1; items < MaxListItems && recIndex < numMenuRecords; recIndex++)
			{		
				recHandle = DmQueryRecord(dBs->mmInfoDB, recIndex);
				
				// If record is found, store the pointer to the item string in the list array.
				if(recHandle)
				{
					recInfoPtr = MemHandleLock(recHandle);

					//	Make sure its a Palms record
					if(recInfoPtr->recordType == PalmsType)
					{
						if(items == 1)
						{
							*offset = recIndex;				//	Do this once only
						}
						//	Point to the item's text string in the database
						listItems[items++] = recInfoPtr->name;
						listItemsStruct[items].recIndex = recIndex;
					}

					MemPtrUnlock(recInfoPtr);
				}
			}
			break;


		case CallAwayList:
			numServingCourseRecords = DmNumRecords(dBs->mmServingCourseDB);

			for(items=0, recIndex=1; items < MaxListItems && recIndex < numServingCourseRecords; recIndex++)
			{		
				recHandle = DmQueryNextInCategory (dBs->mmServingCourseDB, &recIndex, MarkerItem);

				// If record is found, store the pointer to the item string in the list array.
				if(recHandle)
				{
					recServingCoursePtr = MemHandleLock(recHandle);

					
					if(recServingCoursePtr->menuType == DishesMenu
						&&
						recServingCoursePtr->selectable)
					{
						//	If a match can be found with a record in the MiscDataDB
						//	point to the item's text string in the database
						//	and prepend with an asterisk (as disabled)

						recMiscDataIndex = MmFindSortCallAway(prefs->current.tableNo, recIndex);

						recHandle = DmQueryRecord(dBs->mmMiscDataDB, recMiscDataIndex);

						if(recHandle)
						{
							recMiscDataPtr = MemHandleLock(recHandle);

							if(recMiscDataPtr->recordType == CallAwayType			&&
								recMiscDataPtr->number == prefs->current.tableNo	&&
								recMiscDataPtr->recIndex == recIndex)
							{
								StrCopy(listItemsStruct[items].trimName, recServingCoursePtr->selectedName);
							}
							else
							{
								StrCopy(listItemsStruct[items].trimName, recServingCoursePtr->standardName);
							}

							MemPtrUnlock(recMiscDataPtr);
						}
						else
						{
							StrCopy(listItemsStruct[items].trimName, recServingCoursePtr->standardName);
						}

						listItems[items] = listItemsStruct[items].trimName;
							
						//	Store the index as well, so we can deal to it if it is chosen
						listItemsStruct[items].recIndex = recIndex;
						items++;
					}

					MemPtrUnlock(recServingCoursePtr);
				}
				else				//	No record MemHandle = we're finished!
				{
					break;
				}
			}
			break;
	}
	return items;
}



/***********************************************************************
 *
 * FUNCTION:    MmUInt16ToUserSettings
 *
 * DESCRIPTION: 
 *					 
 *
 * PARAMETERS:  User Settings as UInt16 (as received from POS), Prefs
 *
 * RETURNED:    User settings are set in Prefs
 *
 ***********************************************************************/
void MmUInt16ToUserSettings(UInt16 userSettingsUInt16, MmPreferenceTypePtr prefs)
{
	UInt16	soundLevel;
	
	
	prefs->userSettings.advancedUser					=	(userSettingsUInt16 & advancedUserBit)	? 1 : 0;
	prefs->userSettings.priceAdjust					=	(userSettingsUInt16 & priceAdjustBit)	? 1 : 0;
	prefs->userSettings.autoDetails					=	(userSettingsUInt16 & autoDetailsBit)	? 1 : 0;
	prefs->userSettings.isLargeFont					=	(userSettingsUInt16 & isLargeFontBit)	? 1 : 0;
	prefs->userSettings.screenLockOn					=	(userSettingsUInt16 & screenLockBit)	? 1 : 0;
	prefs->userSettings.classicMode					=	(userSettingsUInt16 & classicModeBit)	? 1 : 0;
	prefs->userSettings.soundLevel.soundLevel0	=	(userSettingsUInt16 & soundLevelBit0)	? 1 : 0;
	prefs->userSettings.soundLevel.soundLevel1	=	(userSettingsUInt16 & soundLevelBit1)	? 1 : 0;
	prefs->userSettings.getAllTables					=	(userSettingsUInt16 & getAllTablesBit)	? 1 : 0;
	
	if(prefs->userSettings.isLargeFont)
		prefs->current.fFontID = largeFont;
	else
		prefs->current.fFontID = stdFont;

	soundLevel = (	(prefs->userSettings.soundLevel.soundLevel0 ? 1:0) + 
						(prefs->userSettings.soundLevel.soundLevel1 ? 2:0)		);

	switch(soundLevel)
	{
		case 0:
			PrefSetPreference(prefAlarmSoundVolume, 0);
			break;

		case 1:
			PrefSetPreference(prefAlarmSoundVolume, sndMaxAmp/10);
			break;

		case 2:
			PrefSetPreference(prefAlarmSoundVolume, sndMaxAmp/3);
			break;

		case 3:
			PrefSetPreference(prefAlarmSoundVolume, sndMaxAmp);
	}
}




/***********************************************************************
 *
 * FUNCTION:    MmUserSettingsToUInt16
 *
 * DESCRIPTION: 
 *					 
 *
 * PARAMETERS:  Prefs
 *
 * RETURNED:    User Settings as UInt16 are returned for sending to POS
 *
 ***********************************************************************/
UInt16 MmUserSettingsToUInt16(MmPreferenceTypePtr prefs)
{
	return

	   (prefs->userSettings.advancedUser				?	advancedUserBit	: 0)  |
	   (prefs->userSettings.priceAdjust					?	priceAdjustBit		: 0)  |
	   (prefs->userSettings.autoDetails					?	autoDetailsBit		: 0)  |
	   (prefs->userSettings.isLargeFont					?	isLargeFontBit		: 0)  |
	   (prefs->userSettings.screenLockOn				?	screenLockBit		: 0)  |
	   (prefs->userSettings.classicMode					?	classicModeBit		: 0)  |
	   (prefs->userSettings.soundLevel.soundLevel0	?	soundLevelBit0		: 0)  |
	   (prefs->userSettings.soundLevel.soundLevel1	?	soundLevelBit1		: 0)  |
	   (prefs->userSettings.getAllTables				?	getAllTablesBit	: 0);
}






/***********************************************************************
 *
 * FUNCTION:    MmSetUserSettingsFromMenu
 *
 * DESCRIPTION: 
 *					 
 *
 * PARAMETERS:  
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
void MmSetUserSettingsPopLabel(FormPtr frm, UInt16 controlID, char* currentSetting)
{
	ControlType 			*popControlPtr;
	Int16						width;
	Int16						textLen;
	Boolean					fits = true;
	FontID					currentFont;
	char						tempStr[MaxInfoNameSize+1];


	popControlPtr = FrmGetObjectPtr(frm, FrmGetObjectIndex (frm, controlID));
	currentFont = FntGetFont();

	width = 75;
	FntSetFont(stdFont);
	StrCopy(tempStr, currentSetting);			//	Currently selected value


	textLen = StrLen(tempStr);						//	Untrimmed length
	FntCharsInWidth(tempStr, &width, &textLen, &fits);
	tempStr[textLen] = 0;							//	Terminate the string
	
	CtlSetLabel(popControlPtr, tempStr);

	FntSetFont(currentFont);
}





/***********************************************************************
 *
 * FUNCTION:    MmSetUserSettingsFromMenu
 *
 * DESCRIPTION: 
 *					 
 *
 * PARAMETERS:  
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
void MmSetUserSettings(FormPtr frm, MmPreferenceTypePtr prefs, Char* *listItems, 
								ListPtr listPtr, UInt16 controlID, 
									UInt16* soundLevel, ScreenLockType* screenLockStruct 	)
{
	Int16						formID;

	UInt16					items;					//	Counter to add stuff to the list
	UInt16					itemSelected;
	UInt16					soundLevelLocal;
	UInt16					menuListItem;
	
	MenuOptionType			menuChoice[] =
								{
									{	"Standard User",	"Advanced User",	"",			""			,2 },
									{	"All Tables",		"Current Only",	"",			""			,2 },
									{	"Standard Font",	"Small Font",		"",			""			,2	},
									{	"Disable",			"Enable",			"",			""			,2	}, 
									{	"Sound Off",		"Quiet",				"Medium",	"Loud"	,4	}, 
									{	"Lock Disable",	"Lock Enable",		"",			""			,2	}, 
									{	"Modern Display",	"Classic Display","",			""			,2	}
								};



	//	Link the Pop Triggers to the the menuChoice[]
	switch(controlID)
	{
		case UserSettingsUserLevelPopTrigger:		menuListItem = 0;	break;
		case UserSettingsTablesToGetPopTrigger:	menuListItem = 1;	break;
		case UserSettingsFontSizePopTrigger:		menuListItem = 2;	break;
		case UserSettingsAutoDetailsPopTrigger:	menuListItem = 3;	break;
		case UserSettingsSoundLevelPopTrigger:		menuListItem = 4;	break;
		case UserSettingsScreenLockPopTrigger:		menuListItem = 5;	break;
		case UserSettingsDisplayStylePopTrigger:	menuListItem = 6;	break;
		default:;
	}


	//	Now 'point' the ListItems to the start of each of the Display Views
	//	followed by each of the Serving Course names
	for(items=0; items < menuChoice[menuListItem].numChoices; items++)
		listItems[items] = menuChoice[menuListItem].displayName[items];


	formID = FrmGetFormId(frm);


	if(controlID != NoValue16)
	{
		MmLstSetFont(listPtr, prefs);

		LstSetListChoices(listPtr, listItems, items);

		//	Display the possible items list
		if(items > DishListSizeLargeFont -5)
			items = DishListSizeLargeFont -5;		//	Don't let list get too large

		LstSetHeight(listPtr, items);					//	Set the vertical size of the list

		LstSetSelection(listPtr, 0);

		//	Then get the option selected
		itemSelected = LstPopupList(listPtr);

	//	FrmHideObject(frm, FrmGetObjectIndex(frm, DetailsOptionsSelectedList));

		if(itemSelected != NoValue16)
		{
			switch(controlID)
			{
				case UserSettingsUserLevelPopTrigger:
					
					if(itemSelected == 0)
					{
						prefs->userSettings.advancedUser = false;
//						ShowMessage("Standard User selected");
					}
					else
					{
						prefs->userSettings.advancedUser = true;
//						ShowMessage("Advanced User selected");
					}
					break;



				case UserSettingsTablesToGetPopTrigger:
					
					if(itemSelected == 0)
					{
						prefs->userSettings.getAllTables = true;
						ShowMessage("All Tables will be updated when Beaming");
					}
					else
					{
						prefs->userSettings.getAllTables = false;
						ShowMessage("Only the Current Table will be updated when Beaming");
					}
					break;



				case UserSettingsFontSizePopTrigger:
					
					if(itemSelected == 0)
					{
						prefs->userSettings.isLargeFont = true;
						prefs->current.fFontID = largeFont;
					}
					else
					{
						prefs->userSettings.isLargeFont = false;
						prefs->current.fFontID = stdFont;
					}
					break;



				case UserSettingsAutoDetailsPopTrigger:
					
					if(itemSelected == 0)
					{
						prefs->userSettings.autoDetails = false;
						ShowMessage("Details screen will only display when required");
					}
					else
					{
						prefs->userSettings.autoDetails = true;
						ShowMessage("Details screen will auto-display every time an order is taken");
					}
					break;



				case UserSettingsSoundLevelPopTrigger:
					
					prefs->userSettings.soundLevel.soundLevel0	= (itemSelected & 1) ? 1 : 0;
					prefs->userSettings.soundLevel.soundLevel1	= (itemSelected & 2) ? 1 : 0;

					switch(itemSelected)
					{
						case 0:
							PrefSetPreference(prefAlarmSoundVolume, 0);
							break;

						case 1:
							PrefSetPreference(prefAlarmSoundVolume, sndMaxAmp/10);
							break;

						case 2:
							PrefSetPreference(prefAlarmSoundVolume, sndMaxAmp/3);
							break;

						case 3:
							PrefSetPreference(prefAlarmSoundVolume, sndMaxAmp);
					}
					*soundLevel = PrefGetPreference(prefAlarmSoundVolume);
					MmPlaySounds(thinkingSound);
					break;



				case UserSettingsScreenLockPopTrigger:
					
					if(itemSelected == 0)
					{
						prefs->userSettings.screenLockOn = false;
						screenLockStruct->action = disabled;			//	Disable for now
						ShowMessage("Sreen Safety Lock is Disabled");
					}
					else
					{
						prefs->userSettings.screenLockOn = true;
						screenLockStruct->action = lockOnNilEvent;	//	ScreenLock on next NilEvent
						if(prefs->loggedIn == false)
							ShowMessage("Sreen Safety Lock is Enabled");
					}
					break;



				case UserSettingsDisplayStylePopTrigger:
					
					if(itemSelected == 0)
					{
						prefs->userSettings.classicMode = false;
						ShowMessage("Modern Display with Serving Courses");
					}
					else
					{
						prefs->userSettings.classicMode = true;
						ShowMessage("Classic Display Selected");
					}
					break;

			}
		}
	}



	MmSetUserSettingsPopLabel(frm, UserSettingsUserLevelPopTrigger, 
										menuChoice[0].displayName[prefs->userSettings.advancedUser]);

	//	Note: Reversed logic on prefs->userSettings.getAllTables
	MmSetUserSettingsPopLabel(frm, UserSettingsTablesToGetPopTrigger, 
										menuChoice[1].displayName[!prefs->userSettings.getAllTables]);

	//	Note: Reversed logic on prefs->userSettings.isLargeFont
	MmSetUserSettingsPopLabel(frm, UserSettingsFontSizePopTrigger, 
										menuChoice[2].displayName[!prefs->userSettings.isLargeFont]);

	MmSetUserSettingsPopLabel(frm, UserSettingsAutoDetailsPopTrigger, 
										menuChoice[3].displayName[prefs->userSettings.autoDetails]);

	soundLevelLocal = (	(prefs->userSettings.soundLevel.soundLevel0 ? 1:0) + 
								(prefs->userSettings.soundLevel.soundLevel1 ? 2:0)		);
	MmSetUserSettingsPopLabel(frm, UserSettingsSoundLevelPopTrigger, 
										menuChoice[4].displayName[soundLevelLocal]);

	MmSetUserSettingsPopLabel(frm, UserSettingsScreenLockPopTrigger, 
										menuChoice[5].displayName[prefs->userSettings.screenLockOn]);

	MmSetUserSettingsPopLabel(frm, UserSettingsDisplayStylePopTrigger, 
										menuChoice[6].displayName[prefs->userSettings.classicMode]);
}




/***********************************************************************
 *
 * FUNCTION:    MmFindOptionIndex
 *
 * DESCRIPTION: Use the Course and Option numbers to locate the Index.
 *
 * PARAMETERS:  
 *
 * RETURNED:    .
 *
 ***********************************************************************/
UInt16 MmFindOptionIndex(DmOpenRef optionsDB, UInt8 courseNumber, UInt8 optionNumber)
{
	MemHandle					recHandle;
	UInt16						recIndex = 0;
	OptionsTypePtr				recOptionsPtr;
	UInt16						numRecords;


	numRecords = DmNumRecords(optionsDB);

	//	Find the first item in the Course
	recIndex = MmFindCourseOptionIndex(optionsDB, courseNumber, 1);

	for(;recIndex < numRecords; recIndex++)
	{		
		recHandle = DmQueryRecord(optionsDB, recIndex);

		// If record is found, store the pointer to the item string in the list array.
		if(recHandle)
		{
			recOptionsPtr = MemHandleLock(recHandle);

			if(recOptionsPtr->courseOptionNumber == optionNumber)
			{
				MemPtrUnlock(recOptionsPtr);
				return recIndex;
			}
			else if(recOptionsPtr->courseNumber > courseNumber)
			{
				MemPtrUnlock(recOptionsPtr);
				break;
			}
			MemPtrUnlock(recOptionsPtr);
		}
		else
		{
			MmLogAndDisplayError("Failed to find Options record", Reset);
		}
	}
	return 0;
}



/***********************************************************************
 *
 * FUNCTION:    MmProcessAllOrdersSentStatus
 *
 * DESCRIPTION: Skip through DB checking all records.
 *						If 'mode' == 'UnsendSentAndConfirmed' then orders that
 *						have been Sent and Confirmed are marked UnSent, UnConfirmed.
 *
 *						If 'mode' == 'UnsendSentButNotConfirmed' then orders that have been
 *						Sent but not Confirmed are marked UnSent, UnConfirmed.
 *
 *
 * PARAMETERS:  
 *
 * RETURNED:    	Number of unsent records. Prefs.numUnsentOrders is updated
 *
 ***********************************************************************/
UInt16 MmProcessAllOrdersSentStatus(UInt16 mode, DmOpenRef ordersDB,
														MmPreferenceType *prefs)
{
	UInt16				recIndex;
	MemHandle			recHandle;
	OrderTypePtr		recPtr;

	UInt16				numberOfRecords;
	Err					err;
	UInt16				status;



	//	Step through the DB, resetting the 'Sent' and 'Confirmed' status bits
	numberOfRecords = DmNumRecords(ordersDB);
	for(recIndex=0; recIndex < numberOfRecords; recIndex++)
	{
		recHandle = DmGetRecord(ordersDB, recIndex);
							
		if(recHandle)
		{
			recPtr = MemHandleLock(recHandle);
			if(recPtr->courseNumber != 0)			//	Don't do the Marker records
			{
				status = recPtr->status;
				
				//	If they've been sent, but not necessarily confirmed ...
				if(status & StatusAsSent)
				{
					//	if Either we want to unsend them and they've been sent and confirmed
					//	or we want to unsend them and they've been sent but not yet confirmed
					if(	((mode == UnsendSentAndConfirmed &&
								(status & StatusAsSentConfirmed)	)
						||	
							((mode == UnsendSentButNotConfirmed &&
								((status & StatusAsSentConfirmed) == 0)))  ) )
					{
						//	Mark this record as Unsent
						status &= StatusAsUnsent;				//	Reset the Sent status bit
						status &= StatusAsSentUnconfirmed;	//	Reset the Confirmed status bit
						err = DmWrite(recPtr, (Char*)&recPtr->status - (Char*)&recPtr->tableNumber,
												&status, sizeof(recPtr->status));
						ErrFatalDisplayIf(err, "Could not write 'UnSent' status to record.");
					}
				}
			}
			MemPtrUnlock(recPtr);
			err = DmReleaseRecord(ordersDB, recIndex, true);
			ErrFatalDisplayIf(err, "Could not release record after marking as UnSent.");
		}
	}


	
	//	Count the Unsents to be sure
	prefs->numUnsentOrders = MmNumUnsentOrdersDB(ordersDB);

//	if(prefs->numUnsentOrders)
//		prefs->hLStatus.HLSendOrdersStatus = HLStatusToSend;
	
	return prefs->numUnsentOrders;					//	Total unsent records
}



/***********************************************************************
 *
 * FUNCTION:		MmDeleteCurrentRecord
 *
 * DESCRIPTION:	Delete the current Order record and the Table/Seat
 *						'Marker' record if appropriate.
 *						
 *
 * PARAMETERS:		UInt16 withMarker, UInt16* numberRemoved
 *
 * RETURNED:		False if the records are deleted, true otherwise.
 *
 ***********************************************************************/
Boolean MmDeleteCurrentRecord(DmOpenRef DBName, UInt16 mode, UInt16 currentRecordIndex,
									UInt16* numUnsentOrders, UInt16* numberRemoved)
{
	MemHandle 			recHandle;
	UInt16				recMarkerIndex;
	UInt16				recIndex;
	OrderTypePtr		recPtr;
	UInt16				numberOfRecs;
	UInt16				error;
	Boolean				removed = false;
	OrderType			tempOrder;
	UInt8					itemCounter;
	UInt16				i;
	UInt16				status;


	//	If its not WithMarker, then we're just updating Options, Note etc
	if(mode == WithMarker || mode == SetMenuAndSlaves)
	{
		//	First copy current record into temp structure so we can compare
		recHandle = DmQueryRecord(DBName, currentRecordIndex);
		if(recHandle)
		{
			recPtr = MemHandleLock(recHandle);

			tempOrder.itemNumber		= recPtr->itemNumber;
			tempOrder.courseNumber	= recPtr->courseNumber;
			tempOrder.seatNumber		= recPtr->seatNumber;
			tempOrder.tableNumber	= recPtr->tableNumber;
			tempOrder.menuType		= recPtr->menuType;
			status						= recPtr->status;

			MemPtrUnlock(recPtr);
		}
	
		//	Now go through any other records with same dish number and reduce by 1
		numberOfRecs = DmNumRecords(DBName);
	
		for(i=1; currentRecordIndex+i < numberOfRecs; i++)
		{
			recHandle = DmQueryRecord(DBName, currentRecordIndex + i);
								
			if(recHandle)
			{
				recPtr = MemHandleLock(recHandle);
	
				if(recPtr->itemNumber	== tempOrder.itemNumber		&&
					recPtr->courseNumber	== tempOrder.courseNumber	&&
					recPtr->seatNumber	== tempOrder.seatNumber		&&
					recPtr->tableNumber	== tempOrder.tableNumber	&&
					recPtr->menuType		== tempOrder.menuType			)
				{
					//	Release the first ptr, then reopen for writing
					MemPtrUnlock(recPtr);
					recHandle = DmGetRecord(DBName, currentRecordIndex + i);
	
					if(recHandle)
					{
						recPtr = MemHandleLock(recHandle);
	
						itemCounter = recPtr->itemCounter - 1;
						error = DmWrite(recPtr,
									(UInt8*)&recPtr->itemCounter - (UInt8*)&recPtr->tableNumber,
									&itemCounter, sizeof(recPtr->itemCounter));
						ErrFatalDisplayIf(error, "Could not write new dishCount to record.");
	
						MemPtrUnlock(recPtr);
						DmReleaseRecord(DBName, currentRecordIndex + i, true);
					}
				}
				else
				{
					MemPtrUnlock(recPtr);
					break;								//	None more so bail out
				}
			}
		}
	}

	//	Either way, delete the correct record
	error = DmRemoveRecord(DBName, currentRecordIndex);//	Nuke the record
	ErrFatalDisplayIf(error, "Could not remove Order record");
	if(error)
	{
		return true;
	}
	else
	{
		*numberRemoved = 1;

		//	If it was unsent, update Prefs.numUnsentOrders
		if(	!(status & StatusAsSent)	)
			(*numUnsentOrders)--;
	}

	if(mode == WithMarker)								//	Also take out the Marker record
	{
		//	Go to the 'Marker' record
		recMarkerIndex = MmFindSortTableSeatExt(DBName, tempOrder.tableNumber,
																							tempOrder.seatNumber);

		recHandle = DmQueryRecord(DBName, recMarkerIndex);
							
		// If the record was found, check for Seat correction price
		if(recHandle)
		{
			recPtr = MemHandleLock(recHandle);
			if(recPtr->price == 0)						//	Only remove Marker if there's no price
			{
				recIndex = recMarkerIndex+1;			//	Going to find next Marker Index
				recHandle = DmQueryNextInCategory(DBName, &recIndex, MarkerItem);
		
				if(recHandle && (recIndex - recMarkerIndex == 1))	//	Only one record and the Marker
				{
					MemPtrUnlock(recPtr);
					error = DmRemoveRecord(DBName, recMarkerIndex);//	So nuke the Marker
//					ErrFatalDisplayIf(error, "Could not remove Order record");
					if(error)
						return true;
					else
						*numberRemoved = 2;
				}
				else
				{
					MemPtrUnlock(recPtr);
				}
			}
			else
			{
				MemPtrUnlock(recPtr);
			}
		}
	}
	return false;											//	Record removed at recIndex
}


/***********************************************************************
 *
 * FUNCTION:		MmDeleteSetMenuKeyAndOrders
 *
 * DESCRIPTION:	Delete SetMenu item and all slave orders from DB.
 *
 * PARAMETERS:		Prefs.current.seatNo limits the tent of the search.
 *
 * RETURNED:		True if successful, false otherwise.
 *
 ***********************************************************************/
Boolean MmDeleteSetMenuKeyAndOrders(DmOpenRef DBName, MmPreferenceTypePtr prefs)
{
	MemHandle			recHandle;
	OrderTypePtr		recPtr;
	UInt16				recIndex;
	UInt16				numberOfRecs;
	UInt16				thisSeat;						//	Current Seat number.
	UInt16				recMarkerIndex;
	UInt16				setMenuGroup;
	UInt16				recordsDeleted;


	//	First save off details of this SetMenu Key item so we can compare
	recHandle = DmQueryRecord(DBName, prefs->current.recIndex);
	if(recHandle)
	{
		recPtr = MemHandleLock(recHandle);

		thisSeat			= recPtr->seatNumber;
		setMenuGroup	= recPtr->setMenuGroup;

		MemPtrUnlock(recPtr);
	}
	
	//	Go through all the records for seat. Find the starting Seat index (recMarkerIndex),
	//	then check for any unsent orders in this setMenuGroup and delete them, and Key.

	numberOfRecs = DmNumRecords(DBName);

	recMarkerIndex = MmFindSortTableSeat(prefs->current.tableNo, prefs->current.seatNo);	//	Find Marker record for Table/Seat
	recIndex = recMarkerIndex + 1;

	while(recIndex < numberOfRecs)				//	Do for all records
	{
		recHandle = DmQueryRecord (DBName, recIndex);
		if(recHandle)
		{
			recPtr = MemHandleLock(recHandle);
			
			if(recPtr->seatNumber != thisSeat)
			{
				MemPtrUnlock(recPtr);
				break;									//	All done; out of here
			}
			else
			{
				//	Don't Blitz the Key item yet. We'll need it later to remove the Marker.
				if(recPtr->setMenuGroup == setMenuGroup && !(recPtr->setMenuMask & SetMenuKey))
				{
					MemPtrUnlock(recPtr);			//	Unlock each record as we go

					if(MmDeleteCurrentRecord(DBName, SetMenuAndSlaves, recIndex,
															&prefs->numUnsentOrders, &recordsDeleted))
					{
						ErrFatalDisplayIf(true, "Could not Delete Set Menu Record");
					}

					//	Everything has been shifted down one
					numberOfRecs -= recordsDeleted;
					
					if(recIndex < prefs->current.recIndex)
					{
						prefs->current.recIndex -= recordsDeleted;
					}
				}
				else
				{
					recIndex++;
				}
			}
		}
	}

	//	Now remove the SetMenu Key item, and also the Marker if necessary
	if(MmDeleteCurrentRecord(DBName, WithMarker, prefs->current.recIndex,
											&prefs->numUnsentOrders, &recordsDeleted))
	{
		ErrFatalDisplayIf(true, "Could not Delete Set Menu Key Record");
	}
	
	return false;
}


/***********************************************************************
 *
 * FUNCTION:		MmMarkOrDeleteUnsentOrders
 *
 * DESCRIPTION:	Delete all unsent orders in DB.
 *
 * PARAMETERS:		.
 *
 * RETURNED:		Number of old unsent orders that were in DB.
 *
 ***********************************************************************/
UInt16 MmMarkOrDeleteUnsentOrders(DmOpenRef mmOrdersDB, MmPreferenceTypePtr prefs, UInt16 deleteType)
{
	UInt16				numberOfRecs;
	UInt16				recIndex;
	MemHandle			recHandle;
	OrderTypePtr		recPtr;
	UInt16				totalOldRecords = 0;
	UInt16				recordsDeleted = 0;
	UInt16				status;
	UInt32				timeTaken;
	Err					err;

	recIndex = 0;	

	//	Check through all the records in case some are not sent
	numberOfRecs = DmNumRecords(mmOrdersDB);
	
	while(recIndex < numberOfRecs)
	{
		recHandle = DmQueryRecord(mmOrdersDB, recIndex);

		// If the record was found
		if(recHandle)
		{
			recPtr = MemHandleLock(recHandle);
			
			status = recPtr->status;
			timeTaken = recPtr->timeTaken;
			MemPtrUnlock(recPtr);
			

			if(!(status & StatusAsSent))
			{
				if(deleteType == MarkUnsentOrders)				//	MarkUnsentOrders
				{
					//	Set and write the status
					recHandle = DmGetRecord(mmOrdersDB, recIndex);
					if(recHandle)
					{
						recPtr = MemHandleLock(recHandle);

						if(!(recPtr->setMenuMask & SetMenuKey))
						{
							status |= StatusAsMarkedForDelete;
							err = DmWrite(recPtr, (Char*)&recPtr->status - (Char*)&recPtr->tableNumber,
													&status, sizeof(recPtr->status));
							ErrFatalDisplayIf(err, "Could not write 'Mark for Delete' status to record.");
						}

						MemPtrUnlock(recPtr);
						DmReleaseRecord(mmOrdersDB, recIndex, true);
						recIndex++;
					}
				}



				else if(deleteType == UnMarkUnsentOrders)		//	UnMarkUnsentOrders
				{
					recHandle = DmGetRecord(mmOrdersDB, recIndex);
					if(recHandle)
					{
						recPtr = MemHandleLock(recHandle);

						//	Set and write the status
						status &= ~StatusAsMarkedForDelete;
						err = DmWrite(recPtr, (Char*)&recPtr->status - (Char*)&recPtr->tableNumber,
												&status, sizeof(recPtr->status));
						ErrFatalDisplayIf(err, "Could not write 'UnMark for Delete' status to record.");
						MemPtrUnlock(recPtr);
						DmReleaseRecord(mmOrdersDB, recIndex, true);
						recIndex++;
					}
				}



				//	Not deleting, just counting
				else if(deleteType != DeleteMarkedOrders)	
				{
					//	Deal to it if it was taken before the last AutoLogoff
					if(timeTaken < prefs->logoutTimeSecs)
					{
						recIndex++;
						totalOldRecords++;
					}
					else	//	if(orderTimeSecs < logoutTimeSecs)
					{
						recIndex++;
					}
				}



				//	Go ahead and delete them
				else if((deleteType == DeleteMarkedOrders) && (status & StatusAsMarkedForDelete))
				{
					if(!(recPtr->setMenuMask & SetMenuKey))
					{
						MmDeleteCurrentRecord(mmOrdersDB, WithMarker, recIndex,
																	&prefs->numUnsentOrders, &recordsDeleted);
						numberOfRecs -= recordsDeleted;				//	Got fewer records than we had
						recIndex = recIndex - recordsDeleted + 1;	//	If Marker died then back up extra one
					}
				}



				else
				{
					recIndex++;
				}
			}
			else	//	if(!(status & StatusAsSent))
			{
				recIndex++;
			}
		}
	}
	return totalOldRecords;
}



/***********************************************************************
 *
 * FUNCTION:		MmToggleMarkedForDeleteOrder
 *
 * DESCRIPTION:	Toggle MarkedForDelete status for this Order.
 *
 * PARAMETERS:		
 *
 * RETURNED:		
 *
 ***********************************************************************/
void MmToggleMarkedForDeleteOrder(DmOpenRef mmOrdersDB, UInt16 recIndex)
{
	MemHandle				recHandle;
	OrderTypePtr			recPtr;

	UInt16					status;
	Err						error;


	//	Selection checkbox tapped: Toggle status if Item is to be marked for delete
	recHandle = DmGetRecord(mmOrdersDB, recIndex);
	if(recHandle)
	{
		recPtr = MemHandleLock(recHandle);	// Lock down the block so we can use it

		status = recPtr->status;

		if(!(recPtr->setMenuMask & SetMenuKey))
		{
			//	If its checked, then un-check it
			if(status & StatusAsMarkedForDelete)
			{
				//	Set and write the status
				status = recPtr->status & ~StatusAsMarkedForDelete;
				error = DmWrite(recPtr, (Char*)&recPtr->status - (Char*)&recPtr->tableNumber,
										&status, sizeof(recPtr->status));
			}
			else
			{
				//	Set and write the status
				status = recPtr->status | StatusAsMarkedForDelete;
				error = DmWrite(recPtr, (Char*)&recPtr->status - (Char*)&recPtr->tableNumber,
										&status, sizeof(recPtr->status));
			}

			ErrFatalDisplayIf(error, "Could not write 'Mark for Delete' status to record.");
			DmReleaseRecord(mmOrdersDB, recIndex, true);// Set dirty flag, update the database modification count.
		}
		MemPtrUnlock(recPtr);
	}
}







/***********************************************************************
 *
 * FUNCTION:		MmWarnIfUnsentOrders
 *
 * DESCRIPTION:	Check if old unsent orders exist. Delete if requested.
 *
 * PARAMETERS:		.
 *
 * RETURNED:		true if records were deleted, (use for redraw of screen).
 *
 ***********************************************************************/
Boolean MmWarnIfUnsentOrders(DmOpenRef DBName, MmPreferenceTypePtr prefs, 
										Boolean oldOrdersOnly, Int16 *ptrNumOldUnsentOrders)
{
	Boolean					unsentOrdersDoExist = false;
	Char						tempStr[10];

	//	Force a count up to refresh NumOldUnsentOrders if its unknown
	if(*ptrNumOldUnsentOrders == NoValue16)
	{
		*ptrNumOldUnsentOrders = MmMarkOrDeleteUnsentOrders(DBName, prefs, CountOldOrders);

		if(*ptrNumOldUnsentOrders == 0)
		{
			*ptrNumOldUnsentOrders = OldOrdersDone;
		}
	}


	//	Some OLD Unsent Orders exist, so offer to delete them
	if((*ptrNumOldUnsentOrders)	&&	(*ptrNumOldUnsentOrders != OldOrdersDone))
	{
		StrIToA(tempStr, *ptrNumOldUnsentOrders);
		if(FrmCustomAlert(ConfDeleteOldUnsentOrdersAlert, tempStr, "", "")
											== ConfDeleteOldUnsentOrdersShowUnsentOrders)
		{
			unsentOrdersDoExist = true;
		}

		*ptrNumOldUnsentOrders = OldOrdersDone;
	}
	//	Some normal Unsent Orders exist, so confirm that they mean to send them
	else if(!oldOrdersOnly	&&		prefs->numUnsentOrders)
	{
		StrIToA(tempStr, prefs->numUnsentOrders);
		if(FrmCustomAlert(ConfDeleteUnsentOrdersAlert, tempStr, "", "")
											== ConfDeleteUnsentOrdersShowUnsentOrders)
		{
			unsentOrdersDoExist = true;
		}
	}

	return unsentOrdersDoExist;
}


/***********************************************************************
 *
 * FUNCTION:		MmNukeTableSeatOrders
 *
 * DESCRIPTION:	Delete all sent orders from DB for a Table or Seat.
 *
 * PARAMETERS:		tableOrSeat identifies delete type
 *                Prefs.current.tableNo and Prefs.current.seatNo identify the
 *                correct item to Nuke as appropriate.
 *
 * RETURNED:		New recIndex value after deletions.
 *
 ***********************************************************************/
UInt16 MmNukeTableSeatOrders(DmOpenRef *DBName, UInt16 tableOrSeat, UInt16 recIndex,
										UInt16 tableNumber, UInt16 seatNumber,
											UInt16* numUnsentOrders)
{
	MemHandle			recHandle;
	OrderTypePtr		recPtr;
	UInt16				numberOfRecs;
	UInt16				recordsDeleted;
	UInt16				seatToDelete = 0;				//	Does this Seat die?
	UInt16				thisSeat = 1;					//	Current Seat number.
	Boolean				deleteConfirmed = false;
	UInt16				recMarkerIndex;
	Boolean				tableRemoved = false;
	Err					err;
	Char					tempStr[10];
	OrderType			recMarker	=	{0,0,0,0,0,0,0,0};

	
	//	Go through all the records. Find the starting and ending Table/Seat indices,
	//	for the next seat, while checking for any unsent orders. If none are found,
	//	delete the Seat's orders. Otherwise, flag a warning, before deleting.

	numberOfRecs = DmNumRecords(*DBName);

	if(recIndex == NoValue16)
	{
		//	Find Marker record for Table/Seat
		recIndex = MmFindSortTableSeatExt(DBName, tableNumber, seatNumber);
	}
	recMarkerIndex = recIndex;						//	Preserve Marker index as start

	while(recIndex < numberOfRecs)				//	Do for all records
	{
		recHandle = DmQueryRecord (*DBName, recIndex);
		if(recHandle)
		{
			recPtr = MemHandleLock(recHandle);
			
			if(tableOrSeat == ThisTablesOrders && tableRemoved == false)
			{
				if(recPtr->courseNumber == 0)		//	If its a Marker record
				{
					//	Don't delete seat orders straight away; They may be unsent
					if(seatToDelete == thisSeat)
					{
						//	Delete all orders for this seat
						while(recMarkerIndex < recIndex)	//	recIndex decrements!
						{
							err = DmRemoveRecord(*DBName, recMarkerIndex);	//	So nuke it
							ErrFatalDisplayIf(err, "Could not remove Table Order record");
							if(!err)
							{
								numberOfRecs--;		//	One less than before
								recIndex--;				//	Don't advance if deleted
							}
						}
						if(tableRemoved == true)
						{
							MemPtrUnlock(recPtr);
							break;						//	All done; out of here
						}
						if(recPtr->tableNumber != tableNumber && tableRemoved == false)
							tableRemoved = true;		//	Flag that the Table has been nuked
					}
					thisSeat = recPtr->seatNumber;//	Set the new current seat
					seatToDelete = recPtr->seatNumber;
					deleteConfirmed = false;
					recMarkerIndex = recIndex;		//	Preserve Marker index
				}
				
				//	If its not confirmed as having been Billed display a warning
				else if((recPtr->billing & BillingSeatBilled) != BillingSeatBilled &&
								seatToDelete != 0 && deleteConfirmed != true)
				{
					StrIToA(tempStr, recPtr->seatNumber);	// Convert Seat number to string
					if(FrmCustomAlert(ConfDeleteUnbilledSeatAlert, tempStr, "", "")
														== ConfDeleteUnbilledSeatOK)
					{
						// The delete was confirmed.
						deleteConfirmed = true;
					}
					else
					{
						// The delete was not confirmed.
						seatToDelete = 0;				//	This seat should not be deleted
						deleteConfirmed = false;
					}
				}
				MemPtrUnlock(recPtr);				//	Unlock each record as we go
			}

			//	Deleting this seat's orders
			else if(	tableOrSeat == ThisSeatsOrders &&
						recPtr->tableNumber == tableNumber &&
						recPtr->seatNumber == seatNumber			)
			{
				if(recPtr->tableNumber != LastTableRecord)	//	Don't blitz the last record
				{
					MemPtrUnlock(recPtr);

					if(MmDeleteCurrentRecord(DBName, WithMarker, recIndex,
														numUnsentOrders, &recordsDeleted))
						ErrFatalDisplayIf (true, "Could not Delete Record");

					numberOfRecs -= recordsDeleted;	//	Got fewer records than we had
					recIndex -= recordsDeleted;		//	If Marker died then back up one
				}
				else
				{
					MemPtrUnlock(recPtr);
				}
			}
			else	//	Not a table or a seat, so let's go home
			{
				MemPtrUnlock(recPtr);
				return recIndex;
			}
		}	//	if(recHandle)

		recIndex++;

	}	//	while(recIndex < numberOfRecs)
	return recIndex;
}


/***********************************************************************
 *
 * FUNCTION:    MmCallAwayRecordExists
 *
 * DESCRIPTION: Check if a CallAway Record exists
 *
 * PARAMETERS:  MiscData DB DmOpenRef.
 *
 * RETURNED:    recIndex (or NoValue16 if no record found)
 *
 ***********************************************************************/
UInt16 MmCallAwayRecordExists(DmOpenRef *DBName)
{
	MemHandle				recHandle;
	UInt16					recIndex;
	MiscDataTypePtr		recMiscDataPtr;
	UInt16					numRecords;


	numRecords = DmNumRecords(*DBName);
	recIndex=0;
	while(recIndex < numRecords)
	{		
		recHandle = DmQueryRecord(*DBName, recIndex);
		if(recHandle)
		{
			recMiscDataPtr = MemHandleLock(recHandle);

			if(recMiscDataPtr->recordType == CallAwayType)
			{
				MemPtrUnlock(recMiscDataPtr);
				return recIndex;
			}
			else
			{
				MemPtrUnlock(recMiscDataPtr);
				recIndex++;
			}
		}
	}
	return NoValue16;
}


/***********************************************************************
 *
 * FUNCTION:		MmNumUnsentOrdersDB
 *
 * DESCRIPTION:	Count all unsent orders in DB.
 *
 * PARAMETERS:		.
 *
 * RETURNED:		Number of unsent orders in DB.
 *
 ***********************************************************************/
UInt16 MmNumUnsentOrdersDB(DmOpenRef DBName)
{
	UInt16				numberOfRecs;
	UInt16				recIndex;
	MemHandle			recHandle;
	OrderTypePtr		recPtr;
	UInt16				unSentCounter = 0;

	recIndex = 0;	

	//	Check through all the records in case some are not sent
	numberOfRecs = DmNumRecords(DBName);
	
	while(recIndex < numberOfRecs)
	{
		recHandle = DmQueryRecord(DBName, recIndex++);

		// If the record was found
		if(recHandle)
		{
			recPtr = MemHandleLock(recHandle);
			if(!(recPtr->status & StatusAsSentConfirmed)) //StatusAsSent
			{
				unSentCounter++;
			}
			MemPtrUnlock(recPtr);
		}
	}

	return unSentCounter;
}



 /***********************************************************************
 *
 * FUNCTION:    LstSetFont
 *
 * DESCRIPTION: Desperately ugly force to get around lack of Palm API
 *              Consider using Palm glue function instead
 *              
 *
 * PARAMETERS:  
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
FontID MmLstSetFont(ListType *listPtr, MmPreferenceTypePtr prefs)
{
/*	UInt8*			point;

	point=(UInt8*)listPtr + 22;
	*point=(UInt8)fontId;
*/
	LstGlueSetFont(listPtr, prefs->current.fFontID);
	return prefs->current.fFontID;
}



/***********************************************************************
 *
 * FUNCTION:		MmRedrawList
 *
 * DESCRIPTION:	Format and Redraw a List.
 *
 * PARAMETERS:		listType as defined in MenuMate.h, eg CallAwayList
 *                listHeight: ==0 then DishListSizeStdFont used.
 *
 * RETURNED:		.
 *
 ***********************************************************************/
UInt16 MmRedrawList(ListPtr listPtr, UInt16 listType, UInt16 listHeight,
											Char* *listItems, MmPreferenceTypePtr prefs)
{
	UInt16 					items;
	UInt16 					offset = 0;



	items = MmLoadList(listType, &offset);

	MmLstSetFont(listPtr, prefs);

	LstSetListChoices(listPtr, listItems, items);

	if(listHeight == 0)
		listHeight = items;
	
	//	Display the possible items list
	if(prefs->current.fFontID == stdFont)
	{
		if(listHeight > DishListSizeStdFont -5)
			listHeight = DishListSizeStdFont -5;	//	Don't let list get too large
	}
	else
	{
		if(listHeight > DishListSizeLargeFont -5)
			listHeight = DishListSizeLargeFont -5;	//	Don't let list get too large
	}

	LstSetHeight(listPtr, listHeight);				//	Set the vertical size of the list

	if(listHeight > 7)
		LstSetPosition(listPtr, 18, 14);				//	Position at top edge

	LstSetSelection(listPtr, NoValue16);			//	Ensure no items are selected

	LstDrawList(listPtr);

	return items;
}



/***********************************************************************
 *
 * FUNCTION:		MmDisplayPopNameLabel
 *
 * DESCRIPTION:	Display the Size, Table or Seat names.
 *
 * PARAMETERS:		
 *
 * RETURNED:		
 *
 ***********************************************************************/
void MmDisplayPopNameLabel(FormPtr frm, DBTypePtr dBs, DmOpenRef DBName,
										UInt16 index, UInt16 labelID, Boolean recExists)
{
	SizeTypePtr			recSizePtr;
	SeatnameTypePtr	recTablenamePtr;
	SeatnameTypePtr	recSeatnamePtr;
	MemHandle 			recHandle;
	
	ControlType 		*popControlPtr;
	Int16					width;
	Int16					textLen;
	Boolean				fits = true;
	FontID				currentFont;
	char					tempStr[MaxInfoNameSize+1];
	
	UInt16				labelSpace = 80;

	
	
	popControlPtr = FrmGetObjectPtr(frm, FrmGetObjectIndex (frm, labelID));
	currentFont = FntGetFont();

	if(DBName == dBs->mmDrinksSizesDB || DBName == dBs->mmDishesSizesDB)
	{
		width = 100;
		FntSetFont(boldFont);
		StrCopy(tempStr, "Size");						//	In case a record is not found

		//	If a size name exists write it into the label
		recHandle = DmQueryRecord(DBName, index);
		if(recHandle)
		{
			recSizePtr = MemHandleLock(recHandle);

			StrCopy(tempStr, recSizePtr->sizeName);

			MemPtrUnlock(recSizePtr);
		}
	}
	else if(DBName == dBs->mmTablenameDB)
	{
		width = 75;
		FntSetFont(largeFont);
		StrCopy(tempStr, "Table Names");				//	In case a record is not found

		//	If a name exists for this table write it into the label
		if(recExists)
		{
			recHandle = DmQueryRecord(dBs->mmTablenameDB, index);
			if(recHandle)
			{
				recTablenamePtr = MemHandleLock(recHandle);

				StrCopy(tempStr, recTablenamePtr->name);

				MemPtrUnlock(recTablenamePtr);
			}
		}
	}
	else	//	Must be Seatname
	{
		width = 140;
		FntSetFont(largeFont);
		StrCopy(tempStr, "Seat Names");				//	In case a record is not found

		//	If a Seatname exists for this seat write it into the label
		if(recExists)
		{
			recHandle = DmQueryRecord(dBs->mmSeatnameDB, index);
			if(recHandle)
			{
				recSeatnamePtr = MemHandleLock(recHandle);

				if(recSeatnamePtr->name[0] != 0)
					StrCopy(tempStr, recSeatnamePtr->name);

				MemPtrUnlock(recSeatnamePtr);
			}
		}
	}

	textLen = StrLen(tempStr);						//	Untrimmed length
	FntCharsInWidth(tempStr, &width, &textLen, &fits);
	tempStr[textLen] = 0;							//	Terminate the string


	
	CtlSetLabel(popControlPtr, tempStr);

	FntSetFont(currentFont);
}


/***********************************************************************
 *
 * FUNCTION:    DetailsOptionsTrimList
 *
 * DESCRIPTION: Goes through the Options database records writing a copy
 *              of the Option text to optionNameFit that is trimmed to
 *              the width of the list. Fixes leakage in API scrolling.
 *
 * PARAMETERS:  frm - form containing list to load
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
void MmCourseOptionsTrimList(DBTypePtr dBs, MmPreferenceTypePtr prefs, RectangleType bounds)
{
	UInt16					item = 0;		//	Counter to step through list items or Options
	UInt16					recIndex = 0;
	MemHandle				recHandle;
	DmOpenRef				optionsDB;
	OptionsTypePtr			recOptionsPtr;
	UInt16					optionsInCourse = 0;
	UInt16					numRecords;

	Int16						listWidth;
	Int16						textLen;
	Boolean					fits = true;

	Char						tempStr[MaxOptionItemSize+4];
	Err						error;

	if(prefs->current.mMenu == DishesMenu)
		optionsDB = dBs->mmDishesOptionsDB;
	else
		optionsDB = dBs->mmDrinksOptionsDB;

	numRecords = DmNumRecords(optionsDB);

	//	Find the first Option in the Course
	recIndex = MmFindCourseOptionIndex(optionsDB, prefs->current.course, 1);


	for(item = 0; item < MaxOptionsItems && recIndex < numRecords; item++, recIndex++)
	{		
		recHandle = DmQueryRecord(optionsDB, recIndex);
		
		// If record is found, copy the trimmed item string into optionNameFit.
		if(recHandle)
		{
			recOptionsPtr = MemHandleLock(recHandle);

			if(recOptionsPtr->courseNumber > prefs->current.course)
			{
				MemPtrUnlock(recOptionsPtr);
				break;
			}
			else
			{
				MemPtrUnlock(recOptionsPtr);

				recHandle = DmGetRecord(optionsDB, recIndex);//	Open for read/write

				recOptionsPtr = MemHandleLock(recHandle);

				//	First trim the Plus Option version
				StrCopy(tempStr, recOptionsPtr->optionName);	//	Copy to play with

				// Determine the length of text that will fit within the bounds of the list.
				textLen = StrLen(tempStr);							//	Untrimmed length

				listWidth = bounds.extent.x -10;					//	Allow space for scroll arrows
				
				FntCharsInWidth(tempStr, &listWidth, &textLen, &fits);
				tempStr[textLen] = 0;								//	Terminate the string to listWidth.

				//	Write the 'Trimmed to Fit' Option
				error = DmWrite(recOptionsPtr,
										(char*)&recOptionsPtr->optionNameFit -
											(char*)&recOptionsPtr->courseNumber,
												tempStr, textLen+1);
				ErrFatalDisplayIf(error, "Could not write trimmed List Option.");

				//	Then trim the Minus Option version
				StrCopy(tempStr, "- ");
				StrCat(tempStr, recOptionsPtr->optionName);

				// Determine the length of text that will fit within the bounds of the list.
				textLen = StrLen(tempStr);							//	Untrimmed length
				listWidth = bounds.extent.x -10;					//	Allow space for scroll arrows
				
				FntCharsInWidth(tempStr, &listWidth, &textLen, &fits);
				tempStr[textLen] = 0;								//	Terminate the string to listWidth.

				//	Write the 'Trimmed to Fit' Option
				error = DmWrite(recOptionsPtr,
										(char*)&recOptionsPtr->optionMinusNameFit -
											(char*)&recOptionsPtr->courseNumber,
												tempStr, textLen+1);
				ErrFatalDisplayIf(error, "Could not write trimmed List Minus Option.");


				//	Finally trim the Selected Option version
				StrCopy(tempStr, "* ");
				StrCat(tempStr, recOptionsPtr->optionName);

				// Determine the length of text that will fit within the bounds of the list.
				textLen = StrLen(tempStr);							//	Untrimmed length
				listWidth = bounds.extent.x -10;					//	Allow space for scroll arrows
				
				FntCharsInWidth(tempStr, &listWidth, &textLen, &fits);
				tempStr[textLen] = 0;								//	Terminate the string to listWidth.

				//	Write the 'Trimmed to Fit' Option
				error = DmWrite(recOptionsPtr,
										(char*)&recOptionsPtr->optionSelectedNameFit -
											(char*)&recOptionsPtr->courseNumber,
												tempStr, textLen+1);
				ErrFatalDisplayIf(error, "Could not write trimmed List Selected Option.");

				MemPtrUnlock(recOptionsPtr);
				DmReleaseRecord(optionsDB, recIndex, true);

			}
		}
	}
}


/***********************************************************************
 *
 * FUNCTION:    MmGetTableNumber
 *
 * DESCRIPTION: Get table number from user dependant upon push buttons
 *
 * PARAMETERS:  controlID - id to button pressed.
 *
 * RETURNED:    Table or Seat number value as UInt16
 *
 ***********************************************************************/
/*UInt16 MmGetTableNumber (UInt16 controlID)
{
//	UInt16			number;

	if(controlID >= TableTable10s0PushButton &&
				controlID <= TableTable10s9PushButton)
		return ((controlID - TableTable10s0PushButton)*10);
	else if(controlID >= TableTable1s0PushButton &&
				controlID <= TableTable1s9PushButton)
		return (controlID - TableTable1s0PushButton);
	else
	{
		ErrDisplay("Invalid controlID in function TableGetNumber");
		return true;
	}
}
*/

/***********************************************************************
 *
 * FUNCTION:    MmGetSeatNumber
 *
 * DESCRIPTION: Get seat number from user dependant upon push buttons
 *
 * PARAMETERS:  controlID - id to button pressed.
 *
 * RETURNED:    Table or Seat number value as UInt16
 *
 ***********************************************************************/
/*UInt16 MmGetSeatNumber (UInt16 controlID)
{
//	UInt16	number;

	if (controlID >= SeatSeat10s0PushButton &&
				controlID <= SeatSeat10s9PushButton)
		return ((controlID - SeatSeat10s0PushButton)*10);
	else if(controlID >= SeatSeat1s0PushButton &&
				controlID <= SeatSeat1s9PushButton)
		return (controlID - SeatSeat1s0PushButton);
	else
	{
		ErrDisplay("Invalid controlID in function SeatGetNumber");
		return true;
	}
}
*/


#ifdef TestMenus
/***********************************************************************
 *
 * FUNCTION:    MmLoadSelectList
 *
 * DESCRIPTION: Loads the database records into the Dishes list by
 *              writing pointers to strings into the list pointer array.
 *						Note: Offset allows the index to start further down the list
 *						Eg. If the Staff come after the Table names then offset steps
 *						over the Tables.
 *					 For CourseForcedOptions offset holds group bit position
 *
 * PARAMETERS:  frm - form containing list to load
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
void MmGenerateOrders(DBTypePtr dBs, MmPreferenceTypePtr prefs, Boolean random,
											UInt16 *genTableOrders, 
											UInt16 *genSeatOrders, 
											UInt16 *genOrdersPerSeat, 
											UInt16 *genSeatnames, 
											UInt16 *lastItemOrdered		)
{
	MemHandle				recHandle;

	MenuTypePtr				recMenuPtr;
	ServingCourseTypePtr	recServingCoursePtr;

	UInt16					recMenuIndex;				//	Index through the database
	UInt16					recSeatnameIndex;

	DmOpenRef				menuDB;

	UInt16					numMenuRecords;
	UInt16					numServingCourseRecords;

	UInt16					servingCourseID;

	UInt16					totalOrders;
	UInt16					totalTables;
	UInt16					totalSeats;
	UInt16					ordersPerSeat;
	
	UInt16					tableCount;

	UInt16					itemsOrdered;
	UInt16					size;
	UInt8						currentMenu;


	Char						tempStr[MaxSeatNameSize];
	SeatnameType			seatnameRecord;
	Boolean					addSeatname;
	Boolean					newSeat;



	totalOrders		= 0;
	totalTables		= 0;
	totalSeats		= 0;

	currentMenu = prefs->current.mMenu;						//	Preserve
	numServingCourseRecords = DmNumRecords(dBs->mmServingCourseDB);
	
	recSeatnameIndex = 1;
	
	//	Set the random seed for consistant results for the Fixed generator
	if(!random)
		SysRandom(1);
	
	//	Go through Tables and Seats putting Orders on at random
//				for(prefs->current.tableNo = 1; prefs->current.tableNo < GenTableOrders; prefs->current.tableNo++)
	for(tableCount = 1;tableCount <= *genTableOrders; tableCount++)
	{
		if(random)
			prefs->current.tableNo = SysRandom(0) % *genTableOrders;
		else
			prefs->current.tableNo = tableCount;

		//	Also sprinkle some seat names around
		if((SysRandom(0) % *genTableOrders) % *genSeatnames == 0)
			addSeatname = true;
		else
			addSeatname = false;
		


		//	Do all the seats, either the number specified, or a random number
		for(prefs->current.seatNo = 1; prefs->current.seatNo <= *genSeatOrders; prefs->current.seatNo++)
		{
			newSeat = true;						//	Only add Seatname once
			
			if(random)
				ordersPerSeat = SysRandom(0) % *genOrdersPerSeat;
			else
				ordersPerSeat = *genOrdersPerSeat;
			


			//	Add Orders, either the number specified, or a random number
			for(itemsOrdered = 0; itemsOrdered < ordersPerSeat;)
			{
				//	Randomly select the menu
				prefs->current.mMenu = SysRandom(0) % 2;

				if(prefs->current.mMenu == DishesMenu)
					menuDB = dBs->mmDishesDB;
				else
					menuDB = dBs->mmDrinksDB;
			


				numMenuRecords = DmNumRecords(menuDB);

				servingCourseID = NoValue16;



				do
				{
					recHandle = DmQueryRecord(dBs->mmServingCourseDB, SysRandom(0) % numServingCourseRecords);
										
					if(recHandle)
					{
						recServingCoursePtr = MemHandleLock(recHandle);
						if(recServingCoursePtr->menuType == prefs->current.mMenu	&&
							recServingCoursePtr->recordType == ServingType)
						{
							servingCourseID = recServingCoursePtr->uniqueID;
							MemPtrUnlock(recServingCoursePtr);
							break;
						}
						MemPtrUnlock(recServingCoursePtr);
					}
				}while(servingCourseID == NoValue16);



				//	Generate random orders from anywhere in the menu
				recMenuIndex = SysRandom(0) % numMenuRecords;

				recHandle = DmQueryRecord(menuDB, recMenuIndex);
									
				if(recHandle)
				{
					recMenuPtr = MemHandleLock(recHandle);
					
					if(recMenuPtr->itemNumber)						//	Don't order Course identifiers
					{



						for(size=0; size<MaxSizeTypes; size++)
						{
							if(recMenuPtr->sizes[size].size != NoValue16 &&
								recMenuPtr->sizes[size].mayBeStdItem)
							{
								MmCreateRecordTemplate(prefs);	//	Fill in prefs->curOrder defaults

								prefs->curOrder.tableNumber 			= prefs->current.tableNo;
								prefs->curOrder.seatNumber 			= prefs->current.seatNo;
								prefs->curOrder.menuType				= prefs->current.mMenu;
								prefs->curOrder.courseNumber			= recMenuPtr->courseNumber;
								prefs->curOrder.itemNumber				= recMenuPtr->itemNumber;
								prefs->curOrder.itemCounter			= NoValue16;
								prefs->curOrder.menuIndex[0].index	= recMenuIndex;
								prefs->curOrder.servingCourseID		= servingCourseID;

								prefs->curOrder.status					= 0;
								prefs->curOrder.timeTaken				= TimGetSeconds();

								prefs->curOrder.setMenuMask			= recMenuPtr->sizes[size].setMenuMask;
								prefs->current.recIndex					= NoValue16;

								prefs->curOrder.size						= recMenuPtr->sizes[size].size;
								prefs->curOrder.menuIndex[0].size	= prefs->curOrder.size;
								
								if(recMenuPtr->sizes[size].setMenuMask & SetMenuKey)
								{
									prefs->curOrder.setMenuMask		= recMenuPtr->sizes[size].setMenuMask;
									prefs->curOrder.setMenuGroup		= prefs->current.setMenuGroup++;
								}
								else
								{
									prefs->curOrder.setMenuMask		= 0;
									prefs->curOrder.setMenuGroup		= 0;
								}


								if(addSeatname && newSeat)
								{
									newSeat = false;			//	Only add it once
									
									seatnameRecord.tableNumber = prefs->current.tableNo;
									seatnameRecord.seatNumber = prefs->current.seatNo;

									StrCopy(seatnameRecord.name, "Test ");

									StrIToA(tempStr, prefs->current.tableNo);
									StrCat(seatnameRecord.name, tempStr);

									StrCat(seatnameRecord.name, " ");

									StrIToA(tempStr, prefs->current.seatNo);
									StrCat(seatnameRecord.name, tempStr);

									seatnameRecord.status = StatusAsHasSeatnameToSend;
									MmGenerateSeatOrTableRecord(&dBs->mmSeatnameDB, &seatnameRecord, recSeatnameIndex);
									recSeatnameIndex++;
								}


								MmCreateOrdersRecord(prefs, dBs, lastItemOrdered);	//	Now create the new copy
								itemsOrdered++;
							}
						}
					}
						
					MemPtrUnlock(recMenuPtr);
				}
			}
		}
	}

	prefs->current.mMenu = currentMenu;							//	Restore
}
#endif




/***********************************************************************
 *
 * FUNCTION:    MmScreenLockFunctions
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  Whole bunch of stuff.
 *
 * RETURNED:    .
 *
 ***********************************************************************/
void MmScreenLockFunctions(UInt16 controlID, FormPtr frm, UInt16 previousFormID, UInt32 timeNow, 
										ScreenLockType *screenLockStruct)
{
	//	Allow the Centre control button to be double-clicked to unlock
	if(controlID == vchrRockerCenter)
	{
		if(screenLockStruct->unlockSecs == NoValue32)
			controlID = ScreenLock1Button;
		else
			controlID = ScreenLock2Button;
	}

	switch(controlID)
	{
   	case ScreenLock1Button:
			screenLockStruct->unlockSecs = timeNow + ScreenLockButtonSecs;
	
			FrmCopyLabel(frm, ScreenLockPress1then2Label, "Press to Unlock    >");

			FrmHideObject(frm, FrmGetObjectIndex(frm, ScreenLock1Button));

			FrmShowObject(frm, FrmGetObjectIndex(frm, ScreenLock2Button));
			FrmSetFocus(frm, FrmGetObjectIndex(frm, ScreenLock2Button));
			FrmUpdateForm (FrmGetActiveFormID(), frmRedrawUpdateCode);
			break;


		case ScreenLock2Button:
			if(timeNow < screenLockStruct->unlockSecs		&&
				screenLockStruct->unlockSecs != NoValue32		)
			{
				screenLockStruct->action = unlockOnNilEvent;

				FrmReturnToForm(previousFormID);
			}
			break;
	}	
}



/***********************************************************************
 *
 * FUNCTION:		MmDisplayTableSeatLabel
 *
 * DESCRIPTION:	Display the Table or Seat labels on the Main form.
 *
 * PARAMETERS:		
 *
 * RETURNED:		
 *
 ***********************************************************************/
void MmDisplayTableSeatLabel(FormPtr frm, UInt16 number, UInt16 labelID,
										MmPreferenceTypePtr prefs,
										DmOpenRef mmTablenameDB, DmOpenRef mmSeatnameDB)
{
	Boolean					handled = false;
	Char						numberStr[4];
	Char						tempStr[MaxSeatNameSize+5];	//	Allow for number eg 2.ABCDE

	SeatnameTypePtr		recTablenamePtr;
	SeatnameTypePtr		recSeatnamePtr;
	UInt16					recIndex;
	MemHandle 				recHandle;
	UInt16					numRecords;

	Int16						width;
	Int16						textLen;
	Int16						x, y;									//	Label coords
	Boolean					fits = true;
	FontID					currentFont;
	

	FrmHideObject(frm, FrmGetObjectIndex(frm, labelID));

	if(labelID == MainTablenoLabel)
	{
		if(prefs->current.tableNo == CashTable)
		{
			StrCopy(tempStr, "Cash");
		}
		else
		{
			StrCopy(tempStr, "Table ");
			StrIToA(numberStr, number);
			StrCat(tempStr, numberStr);
	
			//	If a table name exists for this table write it into the label
			numRecords = DmNumRecords(mmTablenameDB);
			
			for(recIndex=0; recIndex < numRecords; recIndex++)
			{
				recHandle = DmQueryRecord(mmTablenameDB, recIndex);
				if(recHandle)										//	Not all tables have names
				{
					recTablenamePtr = MemHandleLock(recHandle);
					if(recTablenamePtr->tableNumber == prefs->current.tableNo)
					{
						StrCopy(tempStr, recTablenamePtr->name);
					}
					MemPtrUnlock(recTablenamePtr);
				}
			}
		}
	}
	else
	{
		if(prefs->current.tableNo == CashTable)
		{
			StrCopy(tempStr, "Cash ");
		}
		else
		{
			StrCopy(tempStr, "Seat ");

			if(StrLen(prefs->defaultSeatname))
			{
				StrNCopy(tempStr, prefs->defaultSeatname, MaxSeatLabelSize);	//	Limit size to button width
				StrCat(tempStr, " ");
			}
		}
		StrIToA(numberStr, number);
		StrCat(tempStr, numberStr);

		//	If a table name exists for this table write it into the label
		numRecords = DmNumRecords(mmSeatnameDB);
		
		for(recIndex=1; recIndex < numRecords; recIndex++)
		{
			recHandle = DmQueryRecord(mmSeatnameDB, recIndex);
			if(recHandle)										//	Not all tables have names
			{
				recSeatnamePtr = MemHandleLock(recHandle);
				if(	recSeatnamePtr->tableNumber == prefs->current.tableNo		&&
						recSeatnamePtr->seatNumber	== prefs->current.seatNo		&&
						recSeatnamePtr->name[0] != 0)
				{
					StrCopy(tempStr, numberStr);
					StrCat(tempStr, ".");
					StrCat(tempStr, recSeatnamePtr->name);
				}
				MemPtrUnlock(recSeatnamePtr);
			}
		}
	}

	// Determine the length of text that will fit within the bounds of the button.
	currentFont = FntGetFont();
	FntSetFont(boldFont);
	textLen = StrLen(tempStr);							//	Untrimmed length
//	width = buttonPtr->bounds.extent.x - 4;		//	Allow additional space
	width = 56;												//	Allow additional space
	
	FntCharsInWidth(tempStr, &width, &textLen, &fits);
	tempStr[textLen] = 0;											//	Terminate the string
	
	//	Centre justify the label
	if(labelID == MainTablenoLabel)
	{
		FrmGetObjectPosition(frm, FrmGetObjectIndex (frm, MainTablenoLabel), &x, &y);
		FrmSetObjectPosition(frm, FrmGetObjectIndex (frm, MainTablenoLabel), 39 - (width / 2), y);
	}
	else
	{
		FrmGetObjectPosition(frm, FrmGetObjectIndex (frm, MainSeatnoLabel), &x, &y);
		FrmSetObjectPosition(frm, FrmGetObjectIndex (frm, MainSeatnoLabel), 120 - (width / 2), y);
	}
	
	FrmCopyLabel(frm,labelID,tempStr);

	FrmShowObject(frm, FrmGetObjectIndex(frm, labelID));

	FntSetFont(currentFont);
}


/***********************************************************************
 *
 * FUNCTION:		MmEnterSeatName
 *
 * DESCRIPTION: 
 *			 			NB. Setting the status bit here causes the Marker record
 *						status to be also set when an order is taken, so that when
 *						Orders are sent to the server the Seatname can be sent first.
 *
 * PARAMETERS:		Option Selected
 *
 * RETURNED:		
 *
 ***********************************************************************/
Boolean MmEnterSeatName(FormPtr frm, UInt16 *ptrRecSeatnameIndex, Boolean recExists,
											MmPreferenceTypePtr prefs, DmOpenRef mmSeatnameDB)
{
	FormPtr 						prvFormPtr;
	MemHandle 					recHandle;
//	UInt16						recIndex;
//	OrderTypePtr				recPtr;
	SeatnameTypePtr 			recSeatnamePtr;
	SeatnameType				seatnameRecord;
	MemHandle					textHandle;
	Char*							textPtr;
	
//	UInt16						status;
	Boolean						seatHasName;
	UInt16						i;
	Err							error;
	
	UInt16						fieldIndex;
	FieldPtr						fieldPtr;

	Char*							sourcePtr;
	Char*							destPtr;


	UInt16						buttonHit;


	prvFormPtr = FrmGetActiveForm();
	frm = FrmInitForm(EditLineForm);
	FrmDrawForm(frm);
	
	FrmHideObject(frm, FrmGetObjectIndex(frm, EditLineRecentPopIconBitMap));
	FrmHideObject(frm, FrmGetObjectIndex(frm, EditLineRecentButton));

	textHandle = MemHandleNew(MaxNoteLength + 1);
	textPtr = MemHandleLock(textHandle);
	
	//	Now get MemPtr to the Seatname record if it exists
	if(recExists)
	{
		recHandle = DmQueryRecord(mmSeatnameDB, *ptrRecSeatnameIndex);
		if(recHandle)
		{
			recSeatnamePtr = MemHandleLock(recHandle);
	
			//	Try and get existing name if possible
			if(recSeatnamePtr->name[0] != '\0'		&&
				StrCaselessCompare((const char *)recSeatnamePtr->name, "Seat Names"))
			{
				//	Allow existing seatnames to be sent as "" if it gets edited to delete it
				seatHasName = true;
				
				sourcePtr = (Char*)&recSeatnamePtr->name;
				destPtr = textPtr;
				for(i=0; recSeatnamePtr->name[i] != 0; i++)
					*destPtr++ = *sourcePtr++;
				*destPtr = 0;
			}
			else
			{
				seatHasName = false;
				
				StrCopy(textPtr, "");
			}
			MemHandleUnlock(recHandle);
		}
	}
	else
	{
		StrCopy(textPtr, "");
	}

	// Set the field to edit the note.
	fieldIndex = FrmGetObjectIndex(frm, EditLineNoteField);
	fieldPtr = FrmGetObjectPtr(frm, fieldIndex);
	FldSetTextHandle(fieldPtr, textHandle);
	FldDrawField(fieldPtr);
	FrmSetFocus(frm, fieldIndex);				// Set the insertion point blinking in the note field
	
	//	Allow a longer logoff time in case they are slow at 'typing'
	if(prefs->userLogOffMins < LogOffMinimumTime && prefs->userLogOffMins != 0)
		MmRefreshAutoLogout(true, LogOffMinimumTime);	//	Update the prefs->autoLogoutSecs global




	// Allow the user to type in a note.  Wait until a button is pushed. OK is 
	// the default button so if the app is switched the note is still entered.
	// The user must press cancel to not record the note.


	buttonHit = FrmDoDialog(frm);



//	SysKeyboardDialog(kbdAlpha);				//	Why doesn't this work???
	
	MmRefreshAutoLogout(true, 0);						//	Update the prefs->autoLogoutSecs global

	// Truncate the string to ensure it's not too long
	textPtr[MaxSeatNameSize] = '\0';

	//	Save the text if the Okay button was pressed. Otherwise trash it.
	if(buttonHit == EditLineOkayButton/* && textPtr[0] != 0*/)
	{
		for(i=0; textPtr[i] != 0; i++)
		{
			if((textPtr[i] < 0x20) || (textPtr[i] > 0x7A))
			{
				textPtr[i] = ' ';
			}
		}


		//	Setting the status bit here causes the Marker record status to be also set
		//	below so when Orders are sent to the server the Seatname can be sent first.
		seatnameRecord.tableNumber = prefs->current.tableNo;
		seatnameRecord.seatNumber = prefs->current.seatNo;
		StrCopy(seatnameRecord.name, textPtr);

		//	If there is text to work with, create a new record, or modify existing
		//	They might also have deleted all characters when editing an existing name
		if(textPtr[0] != 0 || seatHasName)
		{
			seatnameRecord.status = StatusAsHasSeatnameToSend;
			prefs->hLStatus.HLSendOrdersStatus = HLStatusToSend;//	Flag prefs->hLStatus as 'To send'
		}
		else
		{
			seatnameRecord.status = 0;					//	Don't want to send it for nothing
		}

		//	Modify the existing record or generate a new record at given index
		if(recExists)
		{
			//	If they've deleted all the chars to try to get rid of it, then help them out
			//	Otherwise, save what they've entered
			if(StrLen(seatnameRecord.name) == 0)
			{
				error = DmRemoveRecord(mmSeatnameDB, *ptrRecSeatnameIndex);
				ErrFatalDisplayIf(error, "Could not remove Seatname record");
			}
			else
			{
				recHandle = DmQueryRecord(mmSeatnameDB, *ptrRecSeatnameIndex);
				if(recHandle)
				{
					recHandle = DmGetRecord(mmSeatnameDB, *ptrRecSeatnameIndex);
					recSeatnamePtr = MemHandleLock(recHandle);
			
					error = DmWrite(recSeatnamePtr, 0, &seatnameRecord, sizeof(SeatnameType));
					ErrFatalDisplayIf(error, "Could not write modified Seatname record.");
					MemPtrUnlock(recSeatnamePtr);
					DmReleaseRecord(mmSeatnameDB, *ptrRecSeatnameIndex, true);

					prefs->hLStatus.HLSendSeatnamesStatus = HLStatusToSend;
				}
			}
		}
		else			//	Generate a new record at given index
		{
			//	If there is text to work with, create a new record
			if(textPtr[0] != 0)
			{
				MmGenerateSeatOrTableRecord(&mmSeatnameDB, &seatnameRecord, *ptrRecSeatnameIndex);

				prefs->hLStatus.HLSendSeatnamesStatus = HLStatusToSend;
			}
		}
/*
		//	Either way, set the status flag in the Marker record if it exists
		recIndex = MmFindSortTableSeat(prefs->current.tableNo, prefs->current.seatNo);//	Go to the 'Marker' record
		recHandle = DmQueryRecord(mmOrdersDB, recIndex);
		if(recHandle)
		{
			recPtr = MemHandleLock(recHandle);

			//	If there is a valid Marker record already, set the status bit now
			if((recPtr->tableNumber == prefs->current.tableNo) &&
				(recPtr->seatNumber == prefs->current.seatNo)	)
			{
				MemPtrUnlock(recPtr);					//	Release so we can open for write
				recHandle = DmGetRecord(mmOrdersDB, recIndex);
				if(recHandle)
				{
					recPtr = MemHandleLock(recHandle);

					//	If there is text to work with, modify existing status
					if(textPtr[0] != 0 || seatHasName)
						status = recPtr->status | StatusAsHasSeatnameToSend;
					else
						status = recPtr->status & ~StatusAsHasSeatnameToSend;

					error = DmWrite(recPtr, (Char*)&recPtr->status - (Char*)&recPtr->tableNumber,
											&status, sizeof(recPtr->status));
					ErrFatalDisplayIf(error, "Could not write 'Seatname' status to record.");

					//	Also write the Index of the Seatname in the DB
					error = DmWrite(recPtr, (Char*)&recPtr->menuIndex[0].index - (Char*)&recPtr->tableNumber,
											ptrRecSeatnameIndex, sizeof(recPtr->menuIndex[0].index));
					ErrFatalDisplayIf(error, "Could not write 'Seatname' Index to record.");
					DmReleaseRecord(mmOrdersDB, recIndex, true);// Set dirty flag, update the database modification count.
					MemPtrUnlock(recPtr);
				}
			}
			else
			{
				MemPtrUnlock(recPtr);
			}
		}
*/
	}

	FrmShowObject(frm, FrmGetObjectIndex(frm, EditLineRecentPopIconBitMap));
	FrmShowObject(frm, FrmGetObjectIndex(frm, EditLineRecentButton));


//	MemPtrUnlock(recSeatnamePtr);						//	Not needed
	MemPtrUnlock(textPtr);

	FrmDeleteForm(frm);									// Delete the field's new text from memory.
	FrmSetActiveForm(prvFormPtr);

	return true;
}



/***********************************************************************
 *
 * FUNCTION:    MmAddSideToRecord()
 *
 * DESCRIPTION: Add a Side to a Record using Prefs.currentOrder.
 *
 * PARAMETERS:  .
 *
 * RETURNED:    true if sides capacity full, false if successful.
 *
 ***********************************************************************/
Boolean MmAddSideToRecord(DBTypePtr dBs, MmPreferenceTypePtr prefs, 
											UInt16 sizeDBIndex)
{
	UInt16						sideIndex;


	//	Find next storage location for new side
	sideIndex = MmFindIndexForNextSide(&prefs->curOrder);
	
	if(sideIndex >= MaxLinkedItems)
	{
		return true;
	}


	//	Add this Side/DrinkMix item Index to this order
	prefs->curOrder.menuIndex[sideIndex].index = prefs->current.recMenuIndex;
	prefs->curOrder.menuIndex[sideIndex].size = sizeDBIndex;

	MmCopyCurrentOrderIndicesToRecord(dBs->mmOrdersDB, prefs);
	
	return false;
}





/***********************************************************************
 *
 * FUNCTION:    MmAddSideToRecord()
 *
 * DESCRIPTION: Add a Side to a Record using Prefs.currentOrder.
 *
 * PARAMETERS:  .
 *
 * RETURNED:    true if sides capacity full, false if successful.
 *
 ***********************************************************************/
/*Boolean MmAddSideToRecord(DBTypePtr dBs, MmPreferenceTypePtr prefs, 
									ListPtr listSizePtr, UInt16* sizeListIndices)
{
	UInt16						itemSelected;
	MemHandle					recHandle;

	SizeTypePtr					recSizePtr;

	UInt16						numSizeTypes;
	UInt16						offset = 0;	

	Boolean						firstPassFlag = true;
	Boolean						orderGood;
	Boolean						containsSetMenuKey;

	UInt16						sideIndex;									//	Local Counter
	
	DmOpenRef					menuDB;
	DmOpenRef					menuSizesDB;
//	DmOpenRef					optionsDB;




	if(prefs->current.mMenu == DishesMenu)
	{
		menuDB = dBs->mmDishesDB;
		menuSizesDB = dBs->mmDishesSizesDB;
//		optionsDB = dBs->mmDishesOptionsDB;
	}
	else
	{
		menuDB = dBs->mmDrinksDB;
		menuSizesDB = dBs->mmDrinksSizesDB;
//		optionsDB = dBs->mmDrinksOptionsDB;
	}



	//	Find next storage location for new side
	sideIndex = MmFindIndexForNextSide(&prefs->curOrder);
	
	if(sideIndex >= MaxLinkedItems)
	{
		return true;
	}

	// Before we commit to anything, get size type
	// .. in case they bail out
	numSizeTypes = MmCountNumSizeTypes(menuDB, AllSizesNotSetMenuKey, 
													prefs->current.recMenuIndex & IsNotDrinkMask, 
														&prefs->curOrder, sideIndex, 
															NoValue32, &containsSetMenuKey);


	//	Then get the Size selected
	//	If more than 1 size for this item, find which.
	orderGood = false;

	if(numSizeTypes == 1)
	{
		orderGood = true;
	}

	else if(numSizeTypes > 1)
	{
		itemSelected = MmPopupList(listSizePtr, SizeList, &offset, 0);

		if(itemSelected != NoValue16)
		{
			recHandle = DmQueryRecord(menuSizesDB, sizeListIndices[itemSelected]);
			// If found, store pointer to item string in list.
			if (recHandle)
			{
				recSizePtr = MemHandleLock(recHandle);

				orderGood = true;

				//	Point to item's text string in database
				prefs->curOrder.menuIndex[sideIndex].size = recSizePtr->size;
				MemPtrUnlock(recSizePtr);
			}
		}
	}

	//	Add this Side/DrinkMix item Index to this order
	if(orderGood)
	{
		prefs->curOrder.menuIndex[sideIndex].index = prefs->current.recMenuIndex;

		MmCopyCurrentOrderIndicesToRecord(dBs->mmOrdersDB, prefs);
	}
	
	return false;
}
*/



/***********************************************************************
 *
 * FUNCTION:		MmCountNumSizeTypes
 *
 * DESCRIPTION:	Number of Size Types for this Menu item.
 *						Set Prefs.curOrder.menuIndex[sizeIndex].size if there
 *						is only one size defined
 *
 * PARAMETERS:		
 *
 * RETURNED:		 
 *
 ***********************************************************************/
/*UInt16 MmCountNumSizeTypes(DmOpenRef *DBName, MmPreferenceTypePtr prefs, UInt16 countMode, 
										UInt16 recMenuIndex, UInt16 *defaultSizeIndex, 
											UInt32 *defaultSetMenuMask, Boolean *containsSetMenuKey)
{
	MemHandle			recMenuHandle;
	MenuTypePtr			recMenuPtr;
	UInt16				i;
	UInt16				numSizeTypes;
	UInt32				currentSetMenuMask;
					
	
	*containsSetMenuKey = false;
	numSizeTypes = 0;

	currentSetMenuMask = *defaultSetMenuMask & ~SetMenuKey;	//	Strip out the Key bit
	*defaultSetMenuMask = 0;
	
	recMenuHandle = DmQueryRecord(DBName, recMenuIndex);

	if(recMenuHandle)
	{
		recMenuPtr = MemHandleLock(recMenuHandle);

		//	Count the number of sizes to find if only one type
		//	Save the default size in case there turns out to be only one size
		for(i=0; i < MaxSizeTypes; i++)
		{
			switch(countMode)
			{
				case AllAvailableSizes:
					if(recMenuPtr->sizes[i].mayBeStdItem)
					{
						*defaultSizeIndex = recMenuPtr->sizes[i].size;
						*defaultSetMenuMask = recMenuPtr->sizes[i].setMenuMask;
						if(recMenuPtr->sizes[i].setMenuMask & SetMenuKey)
							*containsSetMenuKey = true;
						numSizeTypes += 1;
					}
					break;

				case AllSizesNotSetMenuKey:
//					if(recMenuPtr->sizes[i].mayBeStdItem)
//					{
						//	Only count if its not a SetMenu Key item
						if(!(recMenuPtr->sizes[i].setMenuMask & SetMenuKey))
						{
							*defaultSizeIndex = recMenuPtr->sizes[i].size;
							*defaultSetMenuMask = recMenuPtr->sizes[i].setMenuMask;
							if(recMenuPtr->sizes[i].setMenuMask & SetMenuKey)
								*containsSetMenuKey = true;
							numSizeTypes += 1;
						}
//					}
					break;

				case OnlySetMenuSizes:
					if((recMenuPtr->sizes[i].setMenuMask & currentSetMenuMask)
																		 == currentSetMenuMask)
					{
						*defaultSizeIndex = recMenuPtr->sizes[i].size;
						*defaultSetMenuMask = recMenuPtr->sizes[i].setMenuMask;
						if(recMenuPtr->sizes[i].setMenuMask & SetMenuKey)
							*containsSetMenuKey = true;
						numSizeTypes += 1;
					}
					break;

			}
		}
	
		MemPtrUnlock(recMenuPtr);
	}
	
	return numSizeTypes;
}
*/	
/*	//	If there is only one size, we set it up now
	if(numSizeTypes == 1)
	{
		currentOrderPtr->menuIndex[sideIndex].index = recMenuIndex;
		currentOrderPtr->menuIndex[sideIndex].size = defaultSize;
		
		//	Only alter the setMenuMask for new SetMenu Key orders
		if(countMode == AllAvailableSizes)
		{ 
			currentOrderPtr->size = defaultSize;

			if(*containsSetMenuKey == true)
				currentOrderPtr->setMenuMask = *defaultSetMenuMask;
		}

		if(countMode == OnlySetMenuSizes)
		{
			currentOrderPtr->size = defaultSize;
		}
	}
*/	






/***********************************************************************
 *
 * FUNCTION:		MmCountNumSizeTypes
 *
 * DESCRIPTION:	If only one size, defaultSizeIndex will give offset within recMenuPtr.
 *						
 *
 * PARAMETERS:		
 *
 * RETURNED:		 
 *
 ***********************************************************************/
UInt16 MmCountNumSizeTypes(MmPreferenceTypePtr prefs, UInt16 countMode, 
										MenuTypePtr recMenuPtr, UInt16 *defaultSizeIndex, 
											Boolean *containsSetMenuKey)
{
	UInt16				i;
	UInt16				numSizeTypes;
	UInt32				currentSetMenuMask;
					
	
	currentSetMenuMask = prefs->current.setMenuMask & ~SetMenuKey;
	
	*containsSetMenuKey = false;
	numSizeTypes = 0;

	//	Count the number of sizes to find if only one type
	//	Save the default size in case there turns out to be only one size
	for(i=0; i < MaxSizeTypes; i++)
	{
		switch(countMode)
		{
			case AllAvailableSizes:
				if(recMenuPtr->sizes[i].size != NoValue16
					&&
					recMenuPtr->sizes[i].mayBeStdItem)
				{
					*defaultSizeIndex = i;
					if(recMenuPtr->sizes[i].setMenuMask & SetMenuKey)
						*containsSetMenuKey = true;

					numSizeTypes += 1;
				}
				break;

			case AllSizesNotSetMenuKey:
					//	Only count if its not a SetMenu Key item
					if(!(recMenuPtr->sizes[i].setMenuMask & SetMenuKey))
					{
						*defaultSizeIndex = i;
						if(recMenuPtr->sizes[i].setMenuMask & SetMenuKey)
							*containsSetMenuKey = true;

						numSizeTypes += 1;
					}
//					}
				break;

			case OnlySetMenuSizes:
				if((recMenuPtr->sizes[i].setMenuMask & currentSetMenuMask)
																	 == currentSetMenuMask)
				{
					*defaultSizeIndex = i;
					if(recMenuPtr->sizes[i].setMenuMask & SetMenuKey)
						*containsSetMenuKey = true;

					numSizeTypes += 1;
				}
				break;

			case AllSizesForSide:
					//	Only count if its for this Side
					if(recMenuPtr->sizes[i].size != NoValue16
						&&
						!(recMenuPtr->sizes[i].setMenuMask & SetMenuKey))
					{
						*defaultSizeIndex = i;
						if(recMenuPtr->sizes[i].setMenuMask & SetMenuKey)
							*containsSetMenuKey = true;

						numSizeTypes += 1;
					}
				break;
		}
	}
	
	return numSizeTypes;
}
	





/***********************************************************************
 *
 * FUNCTION:    MmSetDefaultServingCourse()
 *
 * DESCRIPTION: Get a Serving Course for the current Order From the User.
 *						
 *
 * PARAMETERS:  .
 *
 * RETURNED:    
 *
 ***********************************************************************/
void MmSetDefaultServingCourse(MmPreferenceTypePtr prefs, 
												MenuTypePtr recMenuPtr, UInt8* previousCourse, 
													UInt16* servingCourseID, UInt16* servingCourseDefault, 
														UInt16* numServingCourses, Boolean* showServingCourses)
{
	//	If there's a servingCourseDefault set in the menu, just get on with it
	if(recMenuPtr->servingCourseDefault != NoValue16)
	{
		*servingCourseID = recMenuPtr->servingCourseDefault;

		if(prefs->rememberLastServCourse)					//	'Till' based preference, set in PalmMate
		{
			*servingCourseDefault = recMenuPtr->servingCourseDefault;	//	New default
		}
	}


	//	If there was only 1 serving course, then we're already set up
	else if(*numServingCourses == 1)
	{
		*servingCourseDefault = *servingCourseID;	//	New default
	}
	

	//	Otherwise check if we need to popup or not based on previous order
	else
	{
		//	Always default to previous Serving Course
		if(prefs->rememberLastServCourse)			//	'Till' based preference, set in PalmMate
		{
			//	Force popup if invalid servingCourseDefault
			if(*servingCourseDefault != NoValue16)
			{
				*servingCourseID = *servingCourseDefault;	//	Set to same as last order
				*showServingCourses = false;
			}
			else
			{
				*servingCourseID = NoValue16;			//	Force popup
				*showServingCourses = true;
			}
		}

		//	Not prefs->rememberLastServCourse, so keep same Serving Course, unless they change Course
		else
		{
			//	Force popup if invalid servingCourseDefault, or course has changed
			if(*servingCourseDefault != NoValue16	&&
				prefs->current.course == *previousCourse					)
			{
				*servingCourseID = *servingCourseDefault;	//	Set to same as last order
				*showServingCourses = false;
			}
			else
			{
				*servingCourseID = NoValue16;			//	Force popup
				*showServingCourses = true;
			}
		}
	}
}




/***********************************************************************
 *
 * FUNCTION:    MmAskUserForSize()
 *
 * DESCRIPTION: Get a Size for the current Order From the User.
 *						
 *
 * PARAMETERS:  .
 *
 * RETURNED:    Index to selected size in sizeDB.
 *
 ***********************************************************************/
Boolean MmAskUserForSize(DBTypePtr dBs, MmPreferenceTypePtr prefs, UInt16 sizeMode, 
										MenuTypePtr recMenuPtr, 
											ListPtr listSizePtr, UInt16* sizeListIndices, 
												UInt16* defaultSizeIndexPtr)
{
	UInt16						itemSelected;

	MemHandle					recSizeHandle;

	SizeTypePtr					recSizePtr;

	UInt16						offset = 0;
	UInt16						numSizeTypes;
	UInt16						countMode;
	UInt16						listType;
	
	UInt16						i;

	Boolean						orderGood = false;
	Boolean						containsSetMenuKey;
	
	UInt16						defaultSizeIndex;
//	UInt32						defaultSetMenuMask;

	DmOpenRef					menuDB;
	DmOpenRef					menuSizesDB;



	//	Set the countMode to count the sizes
	switch(sizeMode)
	{
		case SizeForNewOrder:
			countMode = AllAvailableSizes;
			listType = SizeList;
			break;

		case SizeForNewSetMenuItem:
			countMode = OnlySetMenuSizes;
			listType = SetMenuSizeList;
			break;

		case SizeForNewSide:
			countMode = AllSizesForSide;
			listType = SizeList;
			break;
	}
	
	

	if(prefs->current.mMenu == DishesMenu)
	{
		menuDB = dBs->mmDishesDB;
		menuSizesDB = dBs->mmDishesSizesDB;
	}
	else
	{
		menuDB = dBs->mmDrinksDB;
		menuSizesDB = dBs->mmDrinksSizesDB;
	}


	//	recMenuPtr may be index for main Item, or may be Side
	numSizeTypes = MmCountNumSizeTypes(prefs, countMode, recMenuPtr, 
													&defaultSizeIndex, 	//	This has Size if only one exists
														&containsSetMenuKey);


	//	If there is only one size, we set it up now
	if(numSizeTypes == 1)
	{
		orderGood = true;

		if(sizeMode == SizeForNewSide)
		{
			*defaultSizeIndexPtr = recMenuPtr->sizes[defaultSizeIndex].size;
		}
		else
		{
			prefs->curOrder.menuIndex[0].size = recMenuPtr->sizes[defaultSizeIndex].size;
			prefs->curOrder.size = recMenuPtr->sizes[defaultSizeIndex].size;

			if(containsSetMenuKey)
				prefs->curOrder.setMenuMask = recMenuPtr->sizes[defaultSizeIndex].setMenuMask;
		}
	}
	

	//	If more than 1 size is defined for this item, show & select
	//	Only pop up size list if specified, unless its a SetMenu Key
	else if(numSizeTypes > 1 			&& 
		(recMenuPtr->popupItemSizes || containsSetMenuKey)	)
	{
		//	Then get the Size selected
		itemSelected = MmPopupList(listSizePtr, listType, &offset, 0);

		if(itemSelected != NoValue16)
		{
			recSizeHandle = DmQueryRecord(menuSizesDB, sizeListIndices[itemSelected]);
			// If found, store pointer to item string in list.
			if(recSizeHandle)
			{
				recSizePtr = MemHandleLock(recSizeHandle);

				orderGood = true;

				*defaultSizeIndexPtr = recSizePtr->size;

				if(sizeMode != SizeForNewSide)
				{
					prefs->curOrder.size = recSizePtr->size;
					prefs->curOrder.menuIndex[0].size = recSizePtr->size;
				}

				MemPtrUnlock(recSizePtr);
			}

			if(sizeMode != SizeForNewSide)
			{
				//	This might be a SetMenu Key item. If a mask is found, set it in CurrentOrder
				for(i=0; i < MaxSizeTypes; i++)
				{
					if(recMenuPtr->sizes[i].size == prefs->curOrder.size)
					{
						//	Only load the SetMenuMask if its a Key item
						if(recMenuPtr->sizes[i].setMenuMask & SetMenuKey)
						{
							prefs->curOrder.setMenuMask =
												recMenuPtr->sizes[i].setMenuMask;
						}
						break;
					}
				}
			}
		}
	}

	return orderGood;
}





/***********************************************************************
 *
 * FUNCTION:		MmFindNextSideIndex
 *
 * DESCRIPTION:	Return index to next available side location in Prefs.curOrder
 *
 * PARAMETERS:		
 *
 * RETURNED:		 
 *
 ***********************************************************************/
UInt16 MmFindIndexForNextSide(CurrentOrderTypePtr currentOrderPtr)
{
	UInt16					sideCounter;
	
	//	Search for end of current list of Sides items
	sideCounter=0;
	while(sideCounter < MaxLinkedItems)
	{
		if(currentOrderPtr->menuIndex[sideCounter].index == NoValue16)
			break;
		else
			sideCounter++;
	}
	
	return sideCounter;
}




/***********************************************************************
 *
 * FUNCTION:    MmGetForcedSides
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
Boolean MmGetForcedSides(DBTypePtr dBs, MmPreferenceTypePtr prefs, 
										ListPtr listPtr, Char* *listItems, Char* *sizeListItems,
											ListStructTypePtr listItemsStruct, UInt16* sizeListIndices)
{
	UInt16					i;
	UInt16					numRecords;
	UInt16					items;
	UInt16					itemSelected;

	UInt16					group;

	Boolean					nextButton;
	UInt16					maxSelect;
	UInt16					totalSelected;

	UInt16					numSelectedInGroup;

	UInt16					sizeIndex;
	UInt16					saveRecMenuIndex;
	
	FontID					currentFontID;
	
	Boolean					broken = false;

	DmOpenRef				menuDB;
	DmOpenRef				forcedSidesDB;



	if(prefs->current.mMenu == DishesMenu)
	{
		menuDB			= dBs->mmDishesDB;
		forcedSidesDB	= dBs->mmDishesForcedSidesDB;
	}
	else
	{
		menuDB			= dBs->mmDrinksDB;
		forcedSidesDB	= dBs->mmDrinksForcedSidesDB;
	}
	
	numRecords = DmNumRecords(forcedSidesDB);

	totalSelected = 0;
	
	//	Step through all the Forced Sides groups
	for(group=1; group <= MaxForcedSidesGroups && !broken; group++)
	{
		saveRecMenuIndex = prefs->current.recMenuIndex;		//	Preserve
		
		//	Select as many as allowed for this group
		itemSelected = NoValue16;
		maxSelect = NoValue16;
		numSelectedInGroup = 0;

		//	Deselect everything in the list
		for(i=0; i < MaxListItems; i++)
		{		
			listItemsStruct[i].selected = false;
			listItemsStruct[i].recIndex = NoValue16;
			listItemsStruct[i].sizeIndex = NoValue16;
		}
		
		while(	!broken														&&
					itemSelected != 0											&&
				(numSelectedInGroup < maxSelect	||	maxSelect == 0)	)
		{
			if(totalSelected >= MaxLinkedItems)
			{
				FrmAlert(WarnMaxDrinkMixItemsAlert);
				group = MaxForcedSidesGroups;
				break;
			}
			
			items = MmLoadSelectList(dBs, prefs, ForcedSidesList, &group, &nextButton, &maxSelect,
												listItems, sizeListItems, listItemsStruct, sizeListIndices);


			//	For Forced Sides, no point showing list if there are no Sides for this Group
			if(items == 0)
			{
				itemSelected = 0;
			}
			else
			{
				currentFontID = MmLstSetFont(listPtr, prefs);

				LstSetListChoices(listPtr, listItems, items);

				//	Display the possible items list
				if(currentFontID == stdFont)
				{
					if(items > DishListSizeStdFont -1)
						items = DishListSizeStdFont -1;			//	Don't let list get too large
				}
				else
				{
					if(items > DishListSizeLargeFont -1)
						items = DishListSizeLargeFont -1;		//	Don't let list get too large
				}
				LstSetHeight(listPtr, items);						//	Set the vertical size of the list

				if(items < 5)
					LstSetPosition(listPtr, 5, 50-items*2);	//	Position above centre
				else
					LstSetPosition(listPtr, 5, 20-items*2);	//	Position above centre

				LstSetSelection(listPtr, NoValue16);	//	Ensure no items are selected

				//	Then get the Side selected
				itemSelected = LstPopupList(listPtr);


				if(itemSelected == NoValue16)
				{
					if(FrmAlert(ConfCancelForcedSideOrOptAlert) == ConfCancelForcedSideOrOptTRASHORDER)
					{
						broken = true;
					}
//					ShowMessage("Forced Sides:\n\nPlease select one.");
				}
				else if(		!nextButton
							||
								(nextButton && itemSelected != 0)
							)
				{
					//	Genuine selection: 
					//	Toggle selection bit OFF, and update the counters and indexes
					if(listItemsStruct[itemSelected].selected == true)
					{
						numSelectedInGroup--;
						totalSelected--;

						listItemsStruct[itemSelected].selected = false;
						listItemsStruct[itemSelected].sizeIndex = NoValue16;
					}

					//	Genuine selection: 
					//	Toggle selection bit ON, and update the counters and indexes
					else
					{
						MemHandle					recMenuHandle;
						MenuTypePtr					recMenuPtr;

						recMenuHandle = DmQueryRecord(menuDB, listItemsStruct[itemSelected].recIndex);

						if(recMenuHandle)
						{
							recMenuPtr = MemHandleLock(recMenuHandle);

							prefs->current.recMenuIndex = listItemsStruct[itemSelected].recIndex;
							
							//	Get Size for this Side
							if(MmAskUserForSize(dBs, prefs, SizeForNewSide, recMenuPtr, listPtr, sizeListIndices, &sizeIndex))
							{
								numSelectedInGroup++;
								totalSelected++;

								listItemsStruct[itemSelected].selected = true;
								listItemsStruct[itemSelected].sizeIndex = sizeIndex;
							}

							MemPtrUnlock(recMenuPtr);
						}
					}
				}
			}
		}


		//	Selected everything we need for this Group. Add all the new Forced Sides to the order.
		if(items != 0)
		{
			for(i=0; i < MaxListItems; i++)
			{		
				if(listItemsStruct[i].selected == true)
				{
	 				listItemsStruct[i].selected = false;

					prefs->current.recMenuIndex = listItemsStruct[i].recIndex;
					
					//	Returns true if no room for more sides
					if(MmAddSideToRecord(dBs, prefs, listItemsStruct[i].sizeIndex))
					{
						prefs->current.recMenuIndex = saveRecMenuIndex;		//	Restore saved
						broken = true;
						return broken;
					}
				}
			}
		}
		prefs->current.recMenuIndex = saveRecMenuIndex;		//	Restore saved
	}
	return broken;
}



/***********************************************************************
 *
 * FUNCTION:    MmGetForcedOptions
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
Boolean MmGetForcedOptions(DBTypePtr dBs, MmPreferenceTypePtr prefs, 
										ListPtr listPtr, Char* *listItems, Char* *sizeListItems,
											ListStructTypePtr listItemsStruct, UInt16* sizeListIndices)
{
	UInt16						i;
	UInt16						numRecords;
	UInt16						items;
	UInt16						itemSelected;


//	UInt16						recIndex;
	MemHandle					recHandle;

//	UInt16						recMenuIndex = 0;
	OptionsTypePtr				recOptionsPtr = 0;
	
	UInt8							*sourcePtr;
	
	
	UInt16						group;

	Boolean						nextButton;
	UInt16						maxSelect;
	UInt16						totalSelected;

	UInt16						numSelectedInGroup;
	
	FontID						currentFontID;
	
	Boolean						broken = false;

//	DmOpenRef					menuDB;
//	DmOpenRef					menuSizesDB;
	DmOpenRef					optionsDB;
//	DmOpenRef					forcedSidesDB;



	if(prefs->current.mMenu == DishesMenu)
	{
//		menuDB			= dBs->mmDishesDB;
//		menuSizesDB		= dBs->mmDishesSizesDB;
		optionsDB		= dBs->mmDishesOptionsDB;
//		forcedSidesDB	= dBs->mmDishesForcedSidesDB;
	}
	else
	{
//		menuDB			= dBs->mmDrinksDB;
//		menuSizesDB		= dBs->mmDrinksSizesDB;
		optionsDB		= dBs->mmDrinksOptionsDB;
//		forcedSidesDB	= dBs->mmDrinksForcedSidesDB;
	}
	
	numRecords = DmNumRecords(optionsDB);

	totalSelected = 0;
	
	//	Step through all the Forced Option groups
	for(group=1; group <= MaxForcedOptionGroups && !broken; group++)
	{
		//	Select as many as allowed for this group
		itemSelected = NoValue16;
		maxSelect = NoValue16;
		numSelectedInGroup = 0;

		//	Deselect everything in the list
		for(i=0; i < MaxListItems; i++)
		{		
			listItemsStruct[i].selected = false;
		}
		
		while(	!broken
				&&
					itemSelected != 0											
				&&
					(numSelectedInGroup < maxSelect	||	maxSelect == 0)	)
		{
			if(totalSelected >= MaxOptionsItems)
			{
				ShowMessage("Sorry, you have entered the maximum number of Options");
				group = MaxForcedOptionGroups;			//	Quit out now
				break;
			}
			
			items = MmLoadSelectList(dBs, prefs, CourseForcedOptions, &group, &nextButton, &maxSelect,
												listItems, sizeListItems, listItemsStruct, sizeListIndices);


			//	For Forced Sides, no point showing list if there are no Options for this Group
			if(items == 0)
			{
				itemSelected = 0;
			}
			else
			{
				currentFontID = MmLstSetFont(listPtr, prefs);

				LstSetListChoices(listPtr, listItems, items);

				//	Display the possible items list
				if(currentFontID == stdFont)
				{
					if(items > DishListSizeStdFont -1)
						items = DishListSizeStdFont -1;			//	Don't let list get too large
				}
				else
				{
					if(items > DishListSizeLargeFont -1)
						items = DishListSizeLargeFont -1;		//	Don't let list get too large
				}
				LstSetHeight(listPtr, items);						//	Set the vertical size of the list

				if(items < 5)
					LstSetPosition(listPtr, 5, 50-items*2);	//	Position above centre
				else
					LstSetPosition(listPtr, 5, 20-items*2);	//	Position above centre

				LstSetSelection(listPtr, NoValue16);	//	Ensure no items are selected

				//	Then get the Side selected
				itemSelected = LstPopupList(listPtr);


				if(itemSelected == NoValue16)
				{
					if(FrmAlert(ConfCancelForcedSideOrOptAlert) == ConfCancelForcedSideOrOptTRASHORDER)
					{
						broken = true;
					}
				}
				else if(	(nextButton && itemSelected != 0)	||	!nextButton)
				{
					//	Genuine selection: Set the Selected bit, and update the counters
					if(listItemsStruct[itemSelected].selected == true)
					{
						listItemsStruct[itemSelected].selected = false;
						numSelectedInGroup--;
						totalSelected--;
					}
					else
					{
						listItemsStruct[itemSelected].selected = true;
						numSelectedInGroup++;
						totalSelected++;
					}
				}
			}
		}
		
		//	Selected everything we need for this Group. Add all the new Forced Options to the order.
		if(items != 0)
		{
			for(i=0; i < MaxListItems; i++)
			{		
				if(listItemsStruct[i].selected == true)
				{
	 				listItemsStruct[i].selected = false;

					
					recHandle = DmQueryRecord(optionsDB, listItemsStruct[i].recIndex);

					// If record is found, store the option in the Prefs.curOrder
					if(recHandle)
					{
						recOptionsPtr = MemHandleLock(recHandle);

						sourcePtr = ((UInt8*)prefs->curOrder.options);

						while(*sourcePtr < 0x80)					//	Until we hit the 'last' one
						{
							sourcePtr++;
						}
						
						if(*sourcePtr == 0x80)
						{
							*sourcePtr = recOptionsPtr->courseOptionNumber | 0x80;
						}
						else
						{
							*sourcePtr &= 0x7F;
							sourcePtr++;
							*sourcePtr = recOptionsPtr->courseOptionNumber | 0x80;
						}
						
						MemPtrUnlock(recOptionsPtr);
					}
				}
			}
		}
	}
	return broken;
}



/***********************************************************************
 *
 * FUNCTION:		MmDisplayStaffNameLabel
 *
 * DESCRIPTION:	Display the Table or Seat labels.
 *
 * PARAMETERS:		
 *
 * RETURNED:		
 *
 ***********************************************************************/
void MmDisplayStaffNameLabel(FormPtr frm, DmOpenRef mmInfoDB, UInt16 index,
											UInt16 popTriggerID, UInt16 overviewPageType, Boolean loggedIn)
{
	InfoTypePtr			recPtr;
	MemHandle 			recHandle;
	static char			tempStr[MaxInfoNameSize+1];
	
	ControlType			*popTrigger;


	if(popTriggerID)	//	Not valid in all forms
	{
		popTrigger = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, popTriggerID));

		if(	popTriggerID != OverviewStaffPopTrigger
			||
				(	popTriggerID == OverviewStaffPopTrigger
					&&	
					overviewPageType != OverviewUnsentPage	)	)
		{
			//	If a Staff name exists for this write it into the label
			recHandle = DmQueryRecord(mmInfoDB, index);
			if(recHandle && loggedIn)
			{
				recPtr = MemHandleLock(recHandle);
				if(recPtr->number != 0)
					StrCopy(tempStr, recPtr->initial);

				MemPtrUnlock(recPtr);
			}
			else
			{
				StrCopy(tempStr, "Login");				//	Not found
			}

//			FrmHideObject(frm, FrmGetObjectIndex(frm, popTriggerID));
			CtlSetLabel(popTrigger, tempStr);
//			FrmShowObject(frm, FrmGetObjectIndex(frm, popTriggerID));
		}
	}
}




/***********************************************************************
 *
 * FUNCTION:		MmBeamOrdersWithOptionalCheckForUnsent
 *
 * DESCRIPTION:	Initiates Beaming, FrmGotoForm(BeamingForm).
 *						If checkIfUnsentOrders, beaming is conditional on finding unsent orders
 *
 * PARAMETERS:		
 *
 * RETURNED:		False if Beaming was initiated, True otherwise
 *
 ***********************************************************************/
Boolean MmBeamOrdersWithOptionalCheckForUnsent(Int16 formId, MmPreferenceTypePtr prefs, 
													Boolean checkIfUnsentOrders, UInt32* unsentOrdersCheckSecs)
{
	Boolean						doBeam = false;
	UInt32						timeNow;
	

	timeNow = TimGetSeconds();


	//	Only check for unsent orders if we've been asked to
	if(checkIfUnsentOrders)
	{
		if(!prefs->loggedIn)						//	Don't hassle them if they're not logged in
		{
			*unsentOrdersCheckSecs = 1;		//	... but nab them as soon as they do log in
		}
		
		//	Still within the time allowance
		else if(timeNow < *unsentOrdersCheckSecs)
		{
			*unsentOrdersCheckSecs = timeNow + UnsentOrdersCheckTimeout;	//	Give them more peace
		}
		
		//	Timed out. Do the checks
		else
		{
			if(prefs->numUnsentOrders)
			{
				if(FrmAlert(ConfDontBeamUnsentOrdersAlert) == ConfDontBeamUnsentOrdersBeamNow)
				{
					doBeam = true;
				}
				else
				{
					*unsentOrdersCheckSecs = timeNow + UnsentOrdersCheckTimeout;	//	Give them peace
				}
			}
		}
	}
	else
	{
		doBeam = true;
	}
	

	if(doBeam)
	{
		//	Beaming selected
		//	Bail out straight away if we're not logged in
		if(prefs->loggedIn												||
			prefs->hLStatus.HLLoginStatus == HLStatusToSend		||
			prefs->hLStatus.HLInfoSyncStatus == HLStatusToSend	||
			prefs->palmUniqueID == 0										)
		{
			FrmGotoForm(BeamingForm);

			//	No problem. Just go for it.
			return false;
		}

		//	Not logged in
		else
		{
			EventType *eventToAdd = (EventType *)MemPtrNew(sizeof(EventType));

			FrmAlert(WarnLoginNotLoggedInAlert);

			//	Generate events to 'click' the Login dropdown trigger
			MemSet(eventToAdd, sizeof(EventType), 0);
			eventToAdd->eType = ctlSelectEvent;	//keyDownEvent
			switch(formId)
			{
				case MainForm:
					eventToAdd->data.ctlEnter.controlID = MainStaffPopTrigger;
					EvtAddUniqueEventToQueue(eventToAdd, 0, true);
					break;
				case OverviewForm:
					eventToAdd->data.ctlEnter.controlID = OverviewStaffPopTrigger;
					EvtAddUniqueEventToQueue(eventToAdd, 0, true);
					break;
				case TableForm:
					eventToAdd->data.ctlEnter.controlID = TableStaffPopTrigger;
					EvtAddUniqueEventToQueue(eventToAdd, 0, true);
					break;
			}

			//	No real problem. Just need to log them in.
			return false;
		}
	}
	
	//	Housten. We have a problem.
	return true;
}





/***********************************************************************
 *
 * FUNCTION:		MmHLValidateMessage
 *
 * DESCRIPTION:	Confirms validity of a Message. If good returns Message Type,
 *							complete messageLength, and error level zero.
 *
 * PARAMETERS:		.
 *
 * RETURNED:		Nothing.
 *
 ***********************************************************************/
//#define DebugHL3

UInt16 MmHLValidateMessage(MmPreferenceTypePtr prefs, UInt8 *aLReceivePtr, UInt16* messageType,
										UInt16 messageIndex, UInt16* messageLength, UInt8* ptrErrorCode)
{
	UInt16						dataCount;
	UInt16						staffIDNo;
	UInt16						confirmCRC;
#ifdef DebugHL3
	Char							messageStr[15];
#endif

	//	Check each message in turn. messageIndex records where we are up to.
	if(aLReceivePtr[messageIndex] < HLRegister || aLReceivePtr[messageIndex] > HLEnd)
	{
		return(HLErrMessInvalidMessageNumber);
	}
		

	dataCount = aLReceivePtr[messageIndex + HLHdrByteCount] * 256 +
					aLReceivePtr[messageIndex + HLHdrByteCount+1];

	confirmCRC = MmHLConfirmCRC(dataCount, messageIndex);
	
	if(confirmCRC == 0)						//	If its a goody
	{
		*messageType = aLReceivePtr[messageIndex+HLHdrCommand];	//	Always first item in message


		*messageLength = HLHeaderSize + dataCount + 2;
		*ptrErrorCode = aLReceivePtr[messageIndex+HLHdrErrCode];


		if(prefs->infoSyncNo != aLReceivePtr[messageIndex+HLHdrInfoSync])
		{
			//	Unless this message stream includes a Login attempt log them out
			if(prefs->hLStatus.HLInfoSyncStatus != HLStatusToSend)
				prefs->loggedIn = false;
			
			prefs->hLStatus.HLInfoSyncStatus = HLStatusToSend;	//	Always check if InfoSync is OK

			#ifdef TestMessageLog
			if(prefs->msgLogEnabled)
			{
				MmPrvSyncLogTraceUI("\nInfoSyncNumber = %d", prefs->infoSyncNo);
				MmPrvSyncLogTraceUI("\nHLHdrInfoSync = %d", aLReceivePtr[messageIndex+HLHdrInfoSync]);
			}
			#endif
		}

		if(prefs->menuSyncNo != aLReceivePtr[messageIndex+HLHdrMenuSync])
			prefs->hLStatus.HLMenuSyncStatus	 = HLStatusToSend;

		if(prefs->drinkSyncNo != aLReceivePtr[messageIndex+HLHdrDrinkSync])
			prefs->hLStatus.HLDrinkSyncStatus = HLStatusToSend;
		
		//	Check the StaffID
		staffIDNo = 0;
		staffIDNo |= aLReceivePtr[messageIndex+HLHdrStaffID] << 8;
		staffIDNo |= aLReceivePtr[messageIndex+HLHdrStaffID+1];

		if(staffIDNo == 0)
		{
			prefs->loggedIn = false;

			#ifdef TestMessageLog
			if(prefs->msgLogEnabled)
			{
				MmPrvSyncLogTrace("\nLogged out by StaffID = 0");
			}
			#endif
		}

		return 0;		//	All good, finished.
	}
	else if(confirmCRC == 1)
	{
		*ptrErrorCode = HLErrWrongVersionNum;
		return 0;
	}
	else
	{
		//	ErrFatalDisplay("Broke! CRC");
		return HLErrInvalidMessageCRC;		//	Message from host was corrupt
	}
}



/***********************************************************************
 *
 * FUNCTION:		MmBtReceiveMessage
 *
 * DESCRIPTION:	Receive a full Bluetooth message.
 *
 * PARAMETERS:		
 *
 * RETURNED:		Nothing.
 *
 ***********************************************************************/
UInt16 MmBtReceiveMessage(UInt16 btPortId, MmPreferenceTypePtr prefs, 
										UInt16 *palmUniqueID, UInt8 *aLReceivePtr)
{
	Err					err;

	UInt16				messageEnd;
	UInt16				messageEndTemp;
	Boolean				isEscape = false;
	
	UInt16				btReceiveIndex;
	UInt16				aLReceiveIndex;
	UInt16				i=0;
	UInt16				btHeaderSize;
	UInt16				btTimeout;
	UInt16				thisPalmUniqueID;

	UInt8					thisChar;
	UInt16				thisChar16;
	
	Int32					sendTimeout;
	UInt32				bytesRead;
	
	Char					verNumStrShort[VerNumStrShortLength];
	

	btReceiveIndex = 0;
	aLReceiveIndex = 0;


	
	btHeaderSize = 15/*sizeof(BtMessageHeader)*/;

	//	Wait for at least a BtHeader's worth of chars

	if(prefs->hLStatus.HLSendOrdersStatus == HLStatusReadyToSend)
	{
		//	Provide site sensitive timeout, on a per-order basis. y = mx+c NB. btTimeoutSendOrders in ms
		sendTimeout = (Int32)((((UInt32)prefs->numUnsentOrders * (UInt32)prefs->btTimeoutSendOrders) / 10) + 
																				(UInt32)BtSendOrdersTimeoutOffset);
		err = SrmReceiveWait(btPortId, btHeaderSize, sendTimeout);
	}



	else if(prefs->hLStatus.HLGetAllOrdersStatus == HLStatusReadyToSend)
	{
		err = SrmReceiveWait(btPortId, btHeaderSize, (Int32)(prefs->btTimeoutGetAllOrders / 10));	//	Values is in millisecs
	}



	else if(prefs->hLStatus.HLBtTimeoutStatus == HLStatusReadyToSend)
	{
		err = SrmReceiveWait(btPortId, btHeaderSize, BtHLBtTimeoutTimeout);
	}



	else
	{
		err = SrmReceiveWait(btPortId, btHeaderSize, (Int32)(prefs->btTimeoutFirstByte));
	}

	if(!err)
	{
		messageEnd = btHeaderSize + 1;	//	This will update when we get a UInt8 count
		
		//	Search for the '~' = Start of Packet
		do
		{
			//	Get a character from the port
			err = BtReadSerial(btPortId, &thisChar, 1, prefs->btTimeoutInterByte, &bytesRead);

			if(err)
			{
				break;						//	Damn. Abandon Ship!
			}

			//	If its a start flag, trash everything and start over again
			switch(btReceiveIndex)
			{
				case 0:	//	Start flag
					if(thisChar == DLStartFlag)
					{
						btReceiveIndex++;
					}
					break;

					
				case 1:						//	Fetch in the Short Version Number
				case 2:
				case 3:
				case 4:
				case 5:
					verNumStrShort[i++] = thisChar;
					btReceiveIndex++;
					break;


				case 6:
					verNumStrShort[i++] = thisChar;
					if(StrNCaselessCompare(verNumStrShort, prefs->verNumStrShort,
																					VerNumStrShortLength))
					{

						ErrThrow(HLErrWrongVersionNum);
					}
					btReceiveIndex++;
					break;


				case 7:	//	PalmID
					thisPalmUniqueID = thisChar;				//	First half
					btReceiveIndex++;
					break;

				case 8:	//	PalmID
					thisChar16 = thisChar;
					thisPalmUniqueID = thisPalmUniqueID | (thisChar16<<8);

					if(prefs->hLStatus.HLRegisterStatus == HLStatusReadyToSend	||
						prefs->hLStatus.HLLoginStatus == HLStatusReadyToSend			)
					{
						prefs->palmUniqueID = thisPalmUniqueID;
					}
					
					//	This will need to be checked in MmBtTalk to check that we are the correct Palm
					*palmUniqueID = thisPalmUniqueID;
					
					btReceiveIndex++;
					break;

					
				case 9:	//	btTimeoutSend in ticks
					btTimeout = thisChar;				//	First half
					btReceiveIndex++;
					break;
					
				case 10:
					thisChar16 = thisChar;
					btTimeout = btTimeout | (thisChar16<<8);
					prefs->btTimeoutSendOrders = btTimeout;	//	In ms per order
					btReceiveIndex++;
					break;
					

				case 11:	//	btTimeoutGet in ticks
					btTimeout = thisChar;				//	First half
					btReceiveIndex++;
					break;
					
				case 12:
					thisChar16 = thisChar;
					btTimeout = btTimeout | (thisChar16<<8);
					prefs->btTimeoutGetAllOrders = btTimeout;
					btReceiveIndex++;
					break;
					

				case 13:	//	Look at the byte Count in the Header
					messageEndTemp = thisChar;			//	First half of count
					btReceiveIndex++;
					break;
					
				case 14:
					thisChar16 = thisChar;
					messageEnd = btHeaderSize + (messageEndTemp | (thisChar16<<8));
//ShowMessage("messageEnd %d", messageEnd);
					btReceiveIndex++;
					break;
					
				
				default:	//	Suck in the rest of the message
					aLReceivePtr[aLReceiveIndex] = thisChar;
					aLReceiveIndex++;
					btReceiveIndex++;
			}
		}while(btReceiveIndex < messageEnd);
	}


	if(err)
	{
		if(err == serErrLineErr)
		{
			SrmClearErr(btPortId);
			SrmReceiveFlush(btPortId, 0);
		}
		return err;
	}

	return 0;
}


/***********************************************************************
 *
 * FUNCTION:	  MmBtTalk
 *
 * DESCRIPTION:	Prepend message with header.
 *						Write the message to the Bluetooth port. 
 *						Then read the response message.
 *
 * PARAMETERS:	
 *
 * RETURNED:	  Boolean
 *
 ***********************************************************************/
UInt16 MmBtTalk(UInt16 btPortId, MmPreferenceTypePtr prefs, 
						UInt8 *ptrWriteMessage, UInt16 writeMessageLength, UInt8 *aLReceivePtr)
{
	UInt16				i, j;
	UInt16				palmUniqueID;
	UInt32 				p_bytesSent;
	
	Err					err;
	

	//	Prepend BT header: Wind pointer back to start of SendBuff array
	ptrWriteMessage[0]	= (UInt8)(TransmissionVersion);

	for(i=0, j=1; i<VerNumStrShortLength; i++, j++)
	{
		ptrWriteMessage[j] = (UInt8)(prefs->verNumStrShort[i]);
	}
	ptrWriteMessage[7]	= (UInt8)(prefs->palmUniqueID & 0x0FF);
	ptrWriteMessage[8]	= (UInt8)(prefs->palmUniqueID >> 8);
	ptrWriteMessage[9]	= (UInt8)(0);			//	btTimeoutSendOrders unused on inward message
	ptrWriteMessage[10]	= (UInt8)(0);
	ptrWriteMessage[11]	= (UInt8)(0);			//	btTimeoutGetAllOrders unused on outward message
	ptrWriteMessage[12]	= (UInt8)(0);

	ptrWriteMessage[13]	= (UInt8)(writeMessageLength & 0x0FF);
	ptrWriteMessage[14]	= (UInt8)(writeMessageLength >> 8);

	
	err = BtSendSerial(btPortId, ptrWriteMessage,
									writeMessageLength + 15/*sizeof(BtMessageHeader)*/, &p_bytesSent);
	if(err)
	{
//ShowDebug("Tx err %x", err);		
		return TalkFail;
	}

	err = MmBtReceiveMessage(btPortId, prefs, &palmUniqueID, aLReceivePtr);
	if(err)
	{
		return TalkFail;
	}
	else if(palmUniqueID != prefs->palmUniqueID)
	{
//ShowDebug("This Palm %u, Rx %u", prefs->palmUniqueID, palmUniqueID);
		return TalkWrongPalm;
	}

	return TalkOkay;
}




/***********************************************************************
 *
 * FUNCTION:    MmCloseBtPort
 *
 * DESCRIPTION: Close Bluetooth port unless it is already open
 *
 * PARAMETERS:  BtPortId
 *
 * RETURNED:    
 *
 ***********************************************************************/
Err MmCloseBtPort(UInt16* btPortId)
{
	Err					err = errNone;
	Err					error = errNone;		//	Try/Catch variable		
	

	ErrTry
	{
		if(*btPortId != NULL)
		{
//ShowMessage("Closing port.");
			err = BtCloseSerial(btPortId);
			if(err != errNone)
			{
//ShowMessage("Throwing BtErrPortClose.");
				ErrThrow(BtErrPortClose);
			}
		}
	}	//	ErrTry

	//	Error Traps
	ErrCatch(error)
	{
		switch(error)
		{
			case BtErrPortClose:
//				ShowMessage("Error closing port. Err %x", err);
				break;
				
		}
	}
	ErrEndCatch

	return err;
}



/***********************************************************************
 *
 * FUNCTION:    MmCloseIrdaPort
 *
 * DESCRIPTION: Close serial port unless it is already open
 *
 * PARAMETERS:  serialRefNum
 *
 * RETURNED:    
 *
 ***********************************************************************/
void MmCloseIrdaPort(UInt16 serialPortID, Boolean UseNewSerialManager)
{
	Err					err;
	SerSettingsType	settings;
	
	if (UseNewSerialManager)
	{
		DeviceInfoType deviceInfo;
		
		err = SrmGetDeviceInfo(serialPortID, &deviceInfo);
		if(err != serErrNotOpen && err != serErrBadPort)
		{
			err = SrmSetReceiveBuffer(serialPortID, NULL, NULL);	//	Restore the default queue
			err = SrmClose(serialPortID);			//	Save the batteries
			ErrNonFatalDisplayIf(err && (err != serErrNotOpen), "Serial Port Close Error.");
		}
	}
	else
	{
		err = SerGetSettings(serialPortID, &settings);
		if(err != serErrNotOpen && err != serErrBadPort)
		{
			err = SerSetReceiveBuffer(serialPortID, NULL, NULL);	//	Restore the default queue

			err = SerControl(serialPortID, serCtlIrDADisable, 0, 0);
			ErrFatalDisplayIf(err, "IrDA Port Close Error.");

			err = SerClose(serialPortID);			//	Save the batteries
			ErrFatalDisplayIf(err, "Serial Port Close Error.");
		}
	}
}



/***********************************************************************
 *
 * FUNCTION:		MmProgressCallback
 *
 * DESCRIPTION:	Progress Callback function to display the updating string
 *
 * PARAMETERS:		
 *
 * RETURNED:		
 *
 ***********************************************************************/
Boolean MmProgressCallback(PrgCallbackData *callbackData)
{
	char			chevrons[] = ">>>>>>>>>>>>>";

	StrCopy(callbackData->textP, &chevrons[callbackData->stage]);

	return (true);
}
