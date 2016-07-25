#define EXTERNAL 1

#include "MenuMate.h"

/***********************************************************************
 *
 * FUNCTION:    MmOpenOrCreateDB
 *
 * DESCRIPTION: Creates a Database with the Name passed
 *						
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/
Boolean MmOpenOrCreateDB(DmOpenRef *dBName, Char* ptrName, DmOpenRef *dBsThisOne)
{
	Int16				error;						// Error code
	UInt16			cardNo;						// Card containing the application database
	LocalID			dbID;							// MemHandle for application database
	UInt16			dbAttrs;						// Database attributes
	UInt16			mode;							// Permissions when opening the database
//	SystemPreferencesType sysPrefs;			// The system's preferences
//	MmPreferenceType prefs;						// App state before closed last time
														// This will be false if the record is now hidden.
	LocalID 			appInfoID;					// 
	UInt16 			version;						// The database's version.


	mode = dmModeReadWrite;

	// P5. Find the application's database.
//	MmOrdersDB = DmOpenDatabaseByTypeCreator(MmDBType, MmAppType, mode);
	*dBName = 0;
	dbID = DmFindDatabase(0, ptrName);
	if(dbID)
		*dBName = DmOpenDatabase(0, dbID, mode);
	
	if(! *dBName)
	{
		// P5. The database doesn't exist, create it now.
//		if(*DBName == MemoDB)
//			error = DmCreateDatabase(0, MemoDBName, MemoAppType, MemoDBType, false);
//		else
			error = DmCreateDatabase(0, ptrName, MmAppType, MmDBType, false);

		ErrFatalDisplayIf(error, "Could not Create Database.");

	
	
		// P5. Check for error.  If the database couldn't be created report it.
		// The application should exit if the database couldn't be created.
		if(error)
			return true;
		
		// P5. Find the application's database.
		dbID = DmFindDatabase(0, ptrName);
		*dBName = DmOpenDatabase(0, dbID, mode);

		// P5. Get info about the database
		error = DmOpenDatabaseInfo(*dBName, &dbID, NULL, NULL, &cardNo, NULL);

		// P5. Check for fatal error.
		ErrFatalDisplayIf(error, "Could not get database info.");
	
		// P5. Get attributes for the database
		error = DmDatabaseInfo(0, dbID, NULL, &dbAttrs, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

		// P5. Check for fatal error.
		ErrFatalDisplayIf(error, "Could not get database info.");
	
		// P17. Set the backup flag in the attributes
		// This will ensure that the database will always get backed up
		// during HotSync, even if there is no conduit for this application.
//		dbAttrs = dbAttrs | dmHdrAttrBackup;
		dbAttrs = dbAttrs & ~dmHdrAttrBackup;	//	For MenuMate we don't want backup

		// P5. Set the new attributes in the database
		error = DmSetDatabaseInfo(0, dbID, NULL, &dbAttrs, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

		// P5. Check for fatal error.
		ErrFatalDisplayIf(error, "Could not set database info.");
		
		// P11. Store category info in the application's information block.
		// If error return so.
		if(MmInitAppInfo(*dBName))
			return true;


		MmInitialiseNewlyCreatedDB(*dBName);
	}
	else
		// P17. After a hard reset, the records may be sent to the device.
		// In this case the database might not have an app info block.
		// Check for an app info block, and if there isn't one create a
		// default one.
	{
		// P17. Get info about the database
		error = DmOpenDatabaseInfo(*dBName, &dbID, NULL, NULL, &cardNo, NULL);

		// P17. Check for fatal error.
		ErrFatalDisplayIf(error, "Could not get database info.");
	
		// P17. Get attributes for the database.  This is an apropriate place
		// to get the version number of the database for applications which
		// have multiple versions of databases.  If the application detects
		// a different version of the database it may decide to not use it,
		// to translate it, or not support some new or old features.
		error = DmDatabaseInfo(0, dbID, NULL, NULL, &version, NULL, NULL, NULL, 
			NULL, &appInfoID, NULL, NULL, NULL);

		// P17. Check for fatal error.
		ErrFatalDisplayIf(error, "Could not get database info.");

		if(appInfoID == NULL)
		{
			// P17. Store category info in the application's information block.
			if (MmInitAppInfo(*dBName))
				return true;
		}
		
		*dBsThisOne = *dBName;
	}


	return false;
}




/***********************************************************************
 *
 * FUNCTION:    MmOpenBluetoothCachedDeviceList
 *
 * DESCRIPTION: Creates a Database with the Name passed
 *						
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/
Boolean MmOpenBluetoothCachedDeviceListDB(DmOpenRef *dBName, Char* ptrName)
{
	Int16				error;						// Error code
	UInt16			cardNo;						// Card containing the application database
	LocalID			dbID;							// MemHandle for application database
//	UInt16			dbAttrs;						// Database attributes
	UInt16			mode;							// Permissions when opening the database
//	SystemPreferencesType sysPrefs;			// The system's preferences
//	MmPreferenceType prefs;						// App state before closed last time
														// This will be false if the record is now hidden.
	LocalID 			appInfoID;					// 
	UInt16 			version;						// The database's version.


	mode = dmModeReadWrite;

	// P5. Find the application's database.
//	MmOrdersDB = DmOpenDatabaseByTypeCreator(MmDBType, MmAppType, mode);
	*dBName = 0;
	dbID = DmFindDatabase(0, ptrName);
	if(dbID)
	{
//ShowDebug("Bluetooth DB found");
		*dBName = DmOpenDatabase(0, dbID, mode);
	}
	
	if(! *dBName)
	{
		// P5. The database doesn't exist, create it now.
//ShowDebug("Bt DB found, but not opened");
	}
	else
		// P17. After a hard reset, the records may be sent to the device.
		// In this case the database might not have an app info block.
		// Check for an app info block, and if there isn't one create a
		// default one.
	{
//ShowDebug("Bt cache DB found and opened");
		// P17. Get info about the database
		error = DmOpenDatabaseInfo(*dBName, &dbID, NULL, NULL, &cardNo, NULL);

		// P17. Check for fatal error.
		ErrFatalDisplayIf(error, "Could not get database info.");
	
		// P17. Get attributes for the database.  This is an apropriate place
		// to get the version number of the database for applications which
		// have multiple versions of databases.  If the application detects
		// a different version of the database it may decide to not use it,
		// to translate it, or not support some new or old features.
		error = DmDatabaseInfo(0, dbID, NULL, NULL, &version, NULL, NULL, NULL, 
			NULL, &appInfoID, NULL, NULL, NULL);

		// P17. Check for fatal error.
		ErrFatalDisplayIf(error, "Could not get database info.");

		if(appInfoID == NULL)
		{
			// P17. Store category info in the application's information block.
			if (MmInitAppInfo(*dBName))
				return true;
		}
	}


	return false;
}




/***********************************************************************
 *
 * FUNCTION:    MmCreateRecordTemplate
 *
 * DESCRIPTION: This routine sets up Prefs.current.record for creating a new record
 *						It should be used as the standard template for new records
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/
void MmCreateRecordTemplate(MmPreferenceTypePtr prefs)
{
	UInt16		i;
	
	//	Fill out the initial values for a new record
	prefs->curOrder.tableNumber 				= 1;
	prefs->curOrder.seatNumber 				= 1;
	prefs->curOrder.menuType	 				= 0;
	prefs->curOrder.courseNumber				= 0;
	prefs->curOrder.itemNumber					= 0;
	prefs->curOrder.itemCounter				= 0;
	prefs->curOrder.itemInCourse				= 0;
	prefs->curOrder.setMenuMask				= 0;
	prefs->curOrder.setMenuGroup				= SetMenuGroupMinValue;
	prefs->curOrder.size							= 0;
	for(i=0; i<MaxLinkedItems; i++)
	{
		prefs->curOrder.menuIndex[i].index	= NoValue16;
		prefs->curOrder.menuIndex[i].size	= NoValue16;
	}
	prefs->curOrder.status						= 0;
	prefs->curOrder.servingCourseID			= 0;
	prefs->curOrder.billing						= 0;
	prefs->curOrder.price						= NoValue32Signed;
//	prefs->curOrder.splitPrice					= 0;
//	prefs->curOrder.priceAdj					= 0;
	prefs->curOrder.timeTaken					= 0;
	prefs->curOrder.noteLength					= 0;
	prefs->curOrder.note[0]						= 0;
	prefs->curOrder.options[0]					= 0x80;

	prefs->current.recIndex						= NoValue16;
}


/***********************************************************************
 *
 * FUNCTION:    MmCopyOrderRecord
 *
 * DESCRIPTION: This routine creates a new record as a copy of the record passed
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/
void MmCopyOrderToCurrentRecord(MmPreferenceTypePtr prefs, DBTypePtr dBs)
{
	OrderTypePtr				recPtr;
	MemHandle 					recHandle;

	UInt8*						sourcePtr;
	UInt8*						destPtr;

	UInt16						recLength=0;
	UInt16						i;				//	Counter
	

	//	Get the record in question, the copy it bytewise across to Prefs.curOrder
	recHandle = DmQueryRecord(dBs->mmOrdersDB, prefs->current.recIndex);
	if(recHandle)
	{
		recPtr = MemHandleLock(recHandle);


		prefs->current.recMenuIndex = recPtr->menuIndex[0].index;


		//	Copy all fields before the NoteLength to Prefs.curOrder
		for(destPtr = (UInt8*)&prefs->curOrder, sourcePtr = (UInt8*)&recPtr->tableNumber;
						sourcePtr < &recPtr->noteLength; recLength++)
			*destPtr++ = *sourcePtr++;					//	Copy all this stuff straight across
	
		//	Also copy the NoteLength and Note
		for(i=0; i <= recPtr->noteLength; i++, recLength++)
			*destPtr++ = *sourcePtr++;					//	Copy the note straight across
		*destPtr = 0;										//	Terminate it
		
		if(recPtr->noteLength == 0)
		{
//			destPtr++;										//	Step over Note
			recLength++;
			sourcePtr = (UInt8*)&recPtr->options;
		}
		else
		{
			sourcePtr = (UInt8*)&recPtr->note + recPtr->noteLength;
		}
	
		//	Also copy the Options
		destPtr = (UInt8*)&prefs->curOrder.options;
		do
		{
			*destPtr = *sourcePtr;						//	Copy the Option
			destPtr++;
			recLength++;
		}while(*sourcePtr++ < 0x80);					//	Until we hit the 'last' one

		MemPtrUnlock(recPtr);
	}
}


/***********************************************************************
 *
 * FUNCTION:    MmCreatNewDuplicateOrder
 *
 * DESCRIPTION: Creates a new duplicate record as a copy of the Current record
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/
void MmCreatNewDuplicateOrder(MmPreferenceTypePtr prefs, DBTypePtr dBs, UInt16* lastItemOrdered)
{
	MenuTypePtr					recMenuPtr;
	MemHandle 					recMenuHandle;


	MmCopyOrderToCurrentRecord(prefs, dBs);

	if(prefs->current.mMenu == DishesMenu)
		recMenuHandle = DmQueryRecord(dBs->mmDishesDB, prefs->current.recMenuIndex);
	else
		recMenuHandle = DmQueryRecord(dBs->mmDrinksDB, prefs->current.recMenuIndex);

	if(recMenuHandle)
	{
		recMenuPtr = MemHandleLock(recMenuHandle);
		
		if(recMenuPtr->enabled & StatusAsEnabled)
		{
			prefs->current.setMenuMask = 0;
			prefs->current.recIndex = NoValue16;
			prefs->curOrder.status &= StatusAsUnsentUnconfirmed;
			prefs->curOrder.itemCounter = NoValue16;
			prefs->curOrder.timeTaken = TimGetSeconds();		//	Record the time stamp
			MmCreateOrdersRecord(prefs, dBs, lastItemOrdered);	//	Now create the new copy
			prefs->hLStatus.HLSendOrdersStatus = HLStatusToSend;//	Flag Prefs.hLStatus as 'To send'
		}
		else
		{
			FrmAlert(WarnMenuDishDisabledAlert);	//	Item is no longer enabled
		}
		
		MemPtrUnlock(recMenuPtr);
	}
}


/***********************************************************************
 *
 * FUNCTION:    MmCheckIfCurrentOrderSeatHasName
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    recIndex if this Seat has got a Seatname, else 0
 *
 ***********************************************************************/
/*
UInt16 MmCheckIfCurrentOrderSeatHasName(MmPreferenceTypePtr prefs, DmOpenRef seatnameDB)
{
	MemHandle 				recHandle;
	UInt16					recSeatnameIndex;
	SeatnameTypePtr 		recSeatnamePtr;
	

	//	Now check to see if this seat has a name that needs sending
	for(recSeatnameIndex=1; recSeatnameIndex < DmNumRecords(seatnameDB); recSeatnameIndex++)
	{
		recHandle = DmQueryRecord(seatnameDB, recSeatnameIndex);
		if(recHandle)
		{
			recSeatnamePtr = MemHandleLock(recHandle);

			//	If its correct table/seat and it needs sending
			if(recSeatnamePtr->tableNumber == prefs->curOrder.tableNumber	&&
				recSeatnamePtr->seatNumber == prefs->curOrder.seatNumber		&&
				((recSeatnamePtr->status & StatusAsHasSeatnameToSend) ==
															StatusAsHasSeatnameToSend)	)
			{
				MemPtrUnlock(recSeatnamePtr);
				return recSeatnameIndex;
			}
			else
			{
				MemPtrUnlock(recSeatnamePtr);
			}
		}
	}
	return NoValue16;
}
*/


/***********************************************************************
 *
 * FUNCTION:    MmCreateFinalRecord
 *
 * DESCRIPTION: This routine creates a new End record
 *
 * PARAMETERS:  
 *
 * RETURNED:    true if the record was sucessfully created.
 *
 ***********************************************************************/
Boolean MmCreateFinalRecord(MmPreferenceTypePtr prefs, DBTypePtr dBs, UInt16* lastItemOrdered)
{
	Boolean				error;

	//	Write a 'first' record to become our last record so searches work correctly.
	MmCreateRecordTemplate(prefs);

	prefs->curOrder.tableNumber 			= LastTableRecord;
	prefs->curOrder.seatNumber 			= LastSeat + 1;
	prefs->curOrder.status					= StatusAsSent | StatusAsSentConfirmed;
	prefs->curOrder.menuIndex[0].index 	= 0;

	prefs->current.recIndex					= NoValue16;
	prefs->current.tableNo					= prefs->curOrder.tableNumber;
	prefs->current.seatNo					= prefs->curOrder.seatNumber;

	error = MmCreateOrdersRecord(prefs, dBs, lastItemOrdered);
	prefs->current.tableNo					= 1;
	prefs->current.seatNo					= 1;
	
	prefs->numUnsentOrders--;

	return error;
}


/***********************************************************************
 *
 * FUNCTION:    MmCreateOrdersRecord
 *
 * DESCRIPTION: This routine creates a new Order record, and Marker record
 *						if necessary.
 *						NB. New record is positioned correctly (after Marker record)
 *
 * PARAMETERS:  
 *
 * RETURNED:    true if the record was sucessfully created.
 *
 ***********************************************************************/
Boolean MmCreateOrdersRecord(MmPreferenceTypePtr prefs, DBTypePtr dBs, UInt16* lastItemOrdered)
{
	MemHandle 					recHandle;
	UInt16						recIndex;
	UInt16						currentRecordIndex;
	UInt16						markerIndex;

	OrderType					recMarker;
	OrderTypePtr				recPtr;
	OrderTypePtr				recMarkerPtr;

	UInt16						numRecords;

	UInt16						setMenuGroup;

	MenuType						recMenuTemp	=	{0,0,0,0,0,0,0,0,0,0,0};

	Err							error;
	UInt16						attr;			// Record attributes (category)

	UInt8							recOrderTemp[sizeof(CurrentOrderType)];
	UInt8							*sourcePtr;
	UInt8							*destPtr;

	UInt16						recLength=0;
	UInt16						i;				//	Counter
	
#ifdef DebugTiming1
	UInt32						startTick, endTick;
	UInt32						startFuncTick, endFuncTick;
	UInt16						diffTicks;
#endif

	
#ifdef DebugTiming1
startFuncTick = TimGetTicks();
#endif

	//	Create a Marker record at the current Table/Seat
	for(i=0, destPtr = (UInt8*)&recMarker; i < sizeof(OrderType); i++)
		*destPtr++ = 0;								//	Zero out the record

	recMarker.tableNumber	= prefs->curOrder.tableNumber;	//	Make it a 'Marker' record
	recMarker.seatNumber		= prefs->curOrder.seatNumber;
	recMarker.options			= 0x80;
	

	//	Note where we are to line up the display
	*lastItemOrdered = prefs->curOrder.menuIndex[0].index;


	currentRecordIndex = prefs->current.recIndex;

	//	If necessary, find Menu Index and save in prefs->curOrder.menuIndex[0],
	//	then find price by adding up the menu components

	//	If this is to be a brand new record
	if(prefs->current.recIndex == NoValue16)
	{
		//	Find position for new Dish
		MmFindCurrentRecordIndex(&recIndex, NewDishCounter, FindOnly);
	}
	
	//	This is an existing record that is being modified by MmDetailsStoreOptionsOrNote.
	//	The record has been copied to prefs->curOrder and then deleted. prefs->curOrder has
	//	been updated to include the changes. Its index was preserved in prefs->current.record.
	else
	{
		recIndex = prefs->current.recIndex;
	}

	
#ifdef DebugTiming1
startTick = TimGetTicks();
#endif

	markerIndex = MmFindSortTableSeat(prefs->current.tableNo, prefs->current.seatNo);	//	Find start of Table/Seat's orders in Orders DB

	
#ifdef DebugTiming1
endTick = TimGetTicks();
diffTicks = endTick - startTick;
PrvSyncLogTrace("\n  MmFindSortTableSeat() = %dms", diffTicks*10);
#endif

	//	If position of new record is given, use it (We're modifying a record)
	if(currentRecordIndex != NoValue16)
		recIndex = currentRecordIndex;
	else
	{
		//	Check if a 'Marker' record exists for this Table/Seat. If not, write one.
		numRecords = DmNumRecords(dBs->mmOrdersDB);
		if(numRecords)
			recHandle = DmQueryRecord(dBs->mmOrdersDB, markerIndex);
		else
		{
			recHandle = 0;
			numRecords++;
		}

		//	if(recHandle), there is a record there. But is it a Marker record?
		if(recHandle)
		{
			recMarkerPtr = MemHandleLock(recHandle);	// Lock down the block so we can use it

			//	If its NOT a Marker record for this Table/Seat, then create one
			if(recMarkerPtr->tableNumber != prefs->curOrder.tableNumber ||
				recMarkerPtr->seatNumber != prefs->curOrder.seatNumber ||
				recMarkerPtr->courseNumber != 0)			//	If the Course != 0 then a Marker record doesn't exist yet.
			{
				MemPtrUnlock(recMarkerPtr);

				//	Create a new Marker record, and set it up with default stuff
				MmCreateMarkerRecord(prefs, dBs->mmOrdersDB, &markerIndex, 0);
				recIndex = markerIndex + 1;	//	New record is past Marker record
			}
			else
			{
				MemPtrUnlock(recMarkerPtr);
			}



			//	A Marker record does now exist

			//	For SetMenus, Marker holds the setMenuGroup, which is then used for each SetMenu item ordered
			//	If this order is a new SetMenuKey we better increase the display count
			if(prefs->curOrder.setMenuMask & SetMenuKey)
			{
				recHandle = DmGetRecord(dBs->mmOrdersDB, markerIndex);
				if(recHandle)
				{
					recMarkerPtr = MemHandleLock(recHandle);	// Lock down the block so we can use it

					//	Marker record holds setMenuGroup to use for NEXT order
					setMenuGroup = (recMarkerPtr->setMenuGroup + 1) | SetMSB16;	//	Ensure its 'unsent'

					error = DmWrite(recMarkerPtr, (Char*)&recMarkerPtr->setMenuGroup - (Char*)&recMarkerPtr->tableNumber,
											&setMenuGroup, sizeof(recMarkerPtr->setMenuGroup));
					MemPtrUnlock(recMarkerPtr);

					ErrFatalDisplayIf(error, "Could not write next 'setMenuGroup' to record.");
					DmReleaseRecord(dBs->mmOrdersDB, markerIndex, true);// Set dirty flag, update the database modification count.

					//	New SetMenuKey item adopts the new setMenuGroup of the Marker
					prefs->curOrder.setMenuGroup = setMenuGroup;
				}
			}
		}
	}


	//	Have Marker record. So now write the actual order record
	//	Create the new record at the correct position


	
	//	Copy all fields before the NoteLength to our temporary buffer
	for(destPtr = recOrderTemp, sourcePtr = (UInt8*)&prefs->curOrder.tableNumber;
					sourcePtr < &prefs->curOrder.noteLength; recLength++)
		*destPtr++ = *sourcePtr++;					//	Copy all this stuff straight across

	//	Also copy the NoteLength and Note
	for(i=0; i <= prefs->curOrder.noteLength; i++, recLength++)
		*destPtr++ = *sourcePtr++;					//	Copy the note straight across
	if(prefs->curOrder.noteLength == 0)
	{
		destPtr++;										//	Step over Note
		recLength++;
	}

	//	Also copy the Options
	sourcePtr = (UInt8*)&prefs->curOrder.options;//	Point to start of Options
	do
	{
		*destPtr = *sourcePtr;						//	Copy the Option
		destPtr++;
		recLength++;
	}while(*sourcePtr++ < 0x80);					//	Until we hit the 'last' one




	recHandle = DmNewRecord(dBs->mmOrdersDB, &recIndex, recLength);
	if(recHandle)			//	Set up the record to store
	{
		recPtr = MemHandleLock(recHandle);

		//	Write the record
		error = DmWrite(recPtr, 0, recOrderTemp, recLength);
		ErrFatalDisplayIf(error, "Could not write to new record.");

		DmRecordInfo(dBs->mmOrdersDB, recIndex, &attr, NULL, NULL);	//	Get the attr info
		attr &= ~dmRecAttrCategoryMask;						// Remove all category bits
		if(prefs->curOrder.tableNumber != LastTableRecord)
			attr |= 1;	//	Bit set => Standard item, ie not MarkerRecord	// prefs->curOrder.courseNumber;				//	... set to the course type
		DmSetRecordInfo(dBs->mmOrdersDB, recIndex, &attr, NULL);		//	Set the attr info

		MemPtrUnlock(recPtr);
		DmReleaseRecord(dBs->mmOrdersDB, recIndex, true);

		prefs->numUnsentOrders++;
	}
	
#ifdef DebugTiming1
endFuncTick = TimGetTicks();
diffTicks = endFuncTick - startFuncTick;
PrvSyncLogTrace("\nMmCreateOrdersRecord() = %dms", diffTicks*10);
#endif

	// Remember the index of the current record.
	prefs->current.recIndex = recIndex;

	return(true);	
}


/***********************************************************************
 *
 * FUNCTION:    MmCreateMarkerRecord
 *
 * DESCRIPTION: This routine creates a new Marker record.
 *
 * PARAMETERS:  
 *
 * RETURNED:    true if the record was sucessfully created.
 *
 ***********************************************************************/
Boolean MmCreateMarkerRecord(MmPreferenceTypePtr prefs, DmOpenRef ordersDB,
											UInt16* recIndexPtr, Int32 priceCorrection)
{
	OrderTypePtr			recPtr;
	MemHandle 				recHandle;
	OrderType				recMarker = {0,0,0,0,0,0,0,0,0,0,0,0};
	UInt16					i;

	Err						error;
	UInt16					attr;			// P11. record attributes (category)

	//	Create a Marker record at the current Table/Seat
	recMarker.tableNumber = prefs->curOrder.tableNumber;	//	Make it a'Marker' record
	recMarker.seatNumber = prefs->curOrder.seatNumber;
	
	for(i=0; i < MaxLinkedItems; i++)
		recMarker.menuIndex[i].index = NoValue16;//	Ensure indices reset

	//	The Marker record holds seat's price correction for items not on Palm menu
	recMarker.price = priceCorrection;

	recMarker.setMenuGroup = SetMenuGroupMinValue;

	recHandle = DmNewRecord(ordersDB, recIndexPtr, sizeof(OrderType));
	if(recHandle)
	{
		recPtr = MemHandleLock(recHandle);
		recMarker.status = recMarker.status | StatusAsSent | StatusAsSentConfirmed;

		//	So write a Marker record
		error = DmWrite(recPtr, 0, &recMarker, sizeof(OrderType));
		ErrFatalDisplayIf(error, "Could not write to new record.");

		DmRecordInfo (ordersDB, *recIndexPtr, &attr, NULL, NULL);	//	Get the attr info
		attr &= ~dmRecAttrCategoryMask;	// Remove all category bits => Marker record
		DmSetRecordInfo (ordersDB, *recIndexPtr, &attr, NULL);		//	Set the attr info

		MemPtrUnlock(recPtr);
		DmReleaseRecord(ordersDB, *recIndexPtr, true);// Set dirty flag, update the database modification count.
	}

	return(true);	
}


/***********************************************************************
 *
 * FUNCTION:    MmCreateGetAllOrdersRecord
 *
 * DESCRIPTION: This routine creates a new Order record.
 *
 * PARAMETERS:  
 *
 * RETURNED:    true if the record was sucessfully created.
 *
 ***********************************************************************/
Boolean MmCreateGetAllOrdersRecord(MmPreferenceTypePtr prefs, DmOpenRef ordersDB,
																					UInt16* recIndexPtr)
{
	OrderTypePtr				recPtr;
	MemHandle 					recHandle;

	UInt8							recOrderTemp[sizeof(CurrentOrderType)];
	UInt8							*sourcePtr;
	UInt8							*destPtr;

	UInt16						recLength=0;
	UInt16						i;				//	Counter
	
	Err							error;
	UInt16						attr;			// P11. record attributes (category)


	//	Copy all fields before the NoteLength to our temporary buffer
	for(destPtr = recOrderTemp, sourcePtr = (UInt8*)&prefs->curOrder.tableNumber;
					sourcePtr < &prefs->curOrder.noteLength; recLength++)
		*destPtr++ = *sourcePtr++;					//	Copy all this stuff straight across

	//	Also copy the NoteLength and Note
	for(i=0; i <= prefs->curOrder.noteLength; i++, recLength++)
		*destPtr++ = *sourcePtr++;					//	Copy the note straight across
	if(prefs->curOrder.noteLength == 0)
	{
		destPtr++;										//	Step over Note
		recLength++;
	}

	//	Also copy the Options
	sourcePtr = (UInt8*)&prefs->curOrder.options;//	Point to start of Options
	do
	{
		*destPtr = *sourcePtr;						//	Copy the Option
		destPtr++;
		recLength++;
	}while(*sourcePtr++ < 0x80);					//	Until we hit the 'last' one


	//	Create the new record at the correct position
	recHandle = DmNewRecord(ordersDB, recIndexPtr, recLength);
	if(recHandle)			//	Set up the record to store
	{
		recPtr = MemHandleLock(recHandle);

		//	Write the record
		error = DmWrite(recPtr, 0, recOrderTemp, recLength);
		ErrFatalDisplayIf(error, "Could not write to new record.");

		DmRecordInfo (ordersDB, *recIndexPtr, &attr, NULL, NULL);	//	Get the attr info
		attr &= ~dmRecAttrCategoryMask;			// Remove all category bits

		attr |= 1;	//	Bit set => Standard item, ie not MarkerRecord	// prefs->curOrder.courseNumber;				//	... set to the course type
		DmSetRecordInfo (ordersDB, *recIndexPtr, &attr, NULL);	//	Set the attr info

		MemPtrUnlock(recPtr);
		DmReleaseRecord(ordersDB, *recIndexPtr, true);
	}

	return(true);	
}






/***********************************************************************
 *
 * FUNCTION:    CreateRecord
 *
 * DESCRIPTION: P6. This routine creates a new memo record.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    true if the record was sucessfully created.
 *
 ***********************************************************************/
Boolean MmCreateMemoRecord(DmOpenRef *DBName, UInt16 recIndex)
{
	MemHandle		 	memoRec;
	MemPtr				memoPtr;
//	UInt16				index = 0;
	UInt16				attr;
	Err					error;
	Char					testString[] = "MenuMate Log\n";

	// Create a new first record in the database
	memoRec = DmNewRecord(*DBName, &recIndex, NewMemoSize);

	// Lock down the block containing the new record.
	memoPtr = MemHandleLock(memoRec);

	// Write a zero to the first UInt8 of the record to null terminate the new memo string.
	error = DmWrite(memoPtr, 0, &testString, StrLen(testString)+1);
	
	// Check for fatal error.
	ErrFatalDisplayIf(error, "Could not write to new record.");

	//	Set the Category for the Memo record to MemoCategory
	DmRecordInfo(*DBName, recIndex, &attr, NULL, NULL);	//	Get the attr info
	attr &= ~dmRecAttrCategoryMask;								// Remove all category bits
	attr |= MemoCategory;
	DmSetRecordInfo(*DBName, recIndex, &attr, NULL);		//	Set the attr info

	// Unlock the block of the new record.
	MemPtrUnlock(memoPtr);

	// Release the record to the database manager.  The true value indicates that
	// the record contains "dirty" data.  Release Record will set the record's 
	// dirty flag and update the database modification count.
	DmReleaseRecord(*DBName, recIndex, true);

	// Remember the index of the current record.
	return(true);	
}





/***********************************************************************
 *
 * FUNCTION:    MmGenerateMenuRecord
 *
 * DESCRIPTION: This routine creates a new Menu record
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/
void MmGenerateMenuRecord(DmOpenRef *DBName, MenuTypePtr newRecordPtr, UInt16 recIndex)
{
	MenuTypePtr				recPtr;
	MemHandle 				recHandle;
	Err						error;
	UInt16					attr;			// record attributes (Category)
	
	recHandle = DmNewRecord(*DBName, &recIndex, sizeof(MenuType));
	if(recHandle)
	{
		recPtr = MemHandleLock(recHandle);

		error = DmWrite(recPtr, 0, newRecordPtr, sizeof(MenuType));
		ErrFatalDisplayIf(error, "Could not write new Menu record.");
		
		// Set the category of the new record to the current CourseNumber.
		DmRecordInfo (*DBName, recIndex, &attr, NULL, NULL);
		attr &= ~dmRecAttrCategoryMask;				// Remove all category bits
		if(recPtr->itemNumber != 0)				//	Only if this is not a Course
			attr |= 1;	//	Bit set => Standard item, ie not Course Record
		DmSetRecordInfo (*DBName, recIndex, &attr, NULL);
		
		MemPtrUnlock(recPtr);
	
		// Release the record to the database manager.  The true value indicates that
		// the record contains "dirty" data.  Release Record will set the record's 
		// dirty flag and update the database modification count.
		DmReleaseRecord(*DBName, recIndex, true);
	}
}


/***********************************************************************
 *
 * FUNCTION:    MmGenerateSizeMenuRecord
 *
 * DESCRIPTION: This routine creates a new Size Menu record
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/
void MmGenerateSizeMenuRecord(DmOpenRef *DBName, SizeTypePtr newRecordPtr, UInt16 recIndex)
{
	SizeTypePtr				recPtr;
	MemHandle 				recHandle;
	Err						error;
	UInt16					attr;			// record attributes (category)
	
	// Create a new record in the database at 'recNumber' position
	recHandle = DmNewRecord(*DBName, &recIndex, sizeof(SizeType));
	if(recHandle)
	{
		recPtr = MemHandleLock(recHandle);
		error = DmWrite(recPtr, 0, newRecordPtr, sizeof(SizeType));
		ErrFatalDisplayIf(error, "Could not write new Size record.");
		DmRecordInfo (*DBName, recIndex, &attr, NULL, NULL);
		attr &= ~dmRecAttrCategoryMask;				// Remove all category bits
		DmSetRecordInfo (*DBName, recIndex, &attr, NULL);
		MemPtrUnlock(recPtr);
		DmReleaseRecord(*DBName, recIndex, true);
	}
}


/***********************************************************************
 *
 * FUNCTION:    MmGenerateSeatOrTableRecord
 *
 * DESCRIPTION: This routine creates a new Tablename record
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/
void MmGenerateSeatOrTableRecord(DmOpenRef *DBName, SeatnameTypePtr newRecordPtr, UInt16 recIndex)
{
	SeatnameTypePtr		recPtr;
	MemHandle 				recHandle;
	Err						error;
	UInt16					attr;			// record attributes (category)
	
	// Create a new record in the database at 'recIndex' position
	recHandle = DmNewRecord(*DBName, &recIndex, sizeof(SeatnameType));
	if(recHandle)
	{
		recPtr = MemHandleLock(recHandle);
		error = DmWrite(recPtr, 0, newRecordPtr, sizeof(SeatnameType));
		ErrFatalDisplayIf(error, "Could not write new Tablename or Seatname record.");
		DmRecordInfo (*DBName, recIndex, &attr, NULL, NULL);
		attr &= ~dmRecAttrCategoryMask;				// Remove all category bits
		DmSetRecordInfo (*DBName, recIndex, &attr, NULL);
		MemPtrUnlock(recPtr);
		DmReleaseRecord(*DBName, recIndex, true);
	}
}


/***********************************************************************
 *
 * FUNCTION:    MmGenerateOptionsRecord
 *
 * DESCRIPTION: This routine creates a new DishOption record at recNumber.
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/
void MmGenerateOptionsRecord(DmOpenRef *DBName, OptionsTypePtr newRecordPtr, UInt16 recIndex)
{
	OptionsTypePtr			recPtr;
	MemHandle 				recHandle;
	Err						error;
	UInt16					attr;			// record attributes (category)
	
	// Create a new record in the database at 'recNumber' position
	recHandle = DmNewRecord(*DBName, &recIndex, sizeof(OptionsType));
	if(recHandle)
	{
		recPtr = MemHandleLock(recHandle);
		error = DmWrite(recPtr, 0, newRecordPtr, sizeof(OptionsType));
		ErrFatalDisplayIf(error, "Could not write new Options record.");
		DmRecordInfo (*DBName, recIndex, &attr, NULL, NULL);
		attr &= ~dmRecAttrCategoryMask;		// Remove all category bits
		DmSetRecordInfo (*DBName, recIndex, &attr, NULL);
		MemPtrUnlock(recPtr);
		DmReleaseRecord(*DBName, recIndex, true);
	}
}


/***********************************************************************
 *
 * FUNCTION:    MmGenerateOptionsRecord
 *
 * DESCRIPTION: This routine creates a new DishOption record at recNumber.
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/
void MmGenerateForcedSideRecord(DmOpenRef *DBName, ForcedSidesTypePtr newRecordPtr, UInt16 recIndex)
{
	ForcedSidesTypePtr	recPtr;
	MemHandle 				recHandle;
	Err						error;
	UInt16					attr;			// record attributes (category)
	
	// Create a new record in the database at 'recNumber' position
	recHandle = DmNewRecord(*DBName, &recIndex, sizeof(ForcedSidesType));
	if(recHandle)
	{
		recPtr = MemHandleLock(recHandle);
		error = DmWrite(recPtr, 0, newRecordPtr, sizeof(ForcedSidesType));
		ErrFatalDisplayIf(error, "Could not write new Options record.");
		DmRecordInfo (*DBName, recIndex, &attr, NULL, NULL);
		attr &= ~dmRecAttrCategoryMask;		// Remove all category bits
		DmSetRecordInfo (*DBName, recIndex, &attr, NULL);
		MemPtrUnlock(recPtr);
		DmReleaseRecord(*DBName, recIndex, true);
	}
}


/***********************************************************************
 *
 * FUNCTION:    MmGenerateInfoRecord
 *
 * DESCRIPTION: This routine creates a new Info record at end of DB.
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/
void MmGenerateInfoRecord(DmOpenRef *DBName, InfoTypePtr newRecordPtr, UInt16 recIndex)
{
	InfoTypePtr				recPtr;
	MemHandle 				recHandle;
	Err						error;
	UInt16					attr;			// record attributes (category)
	
	// Create a new record in the database at 'recNumber' position
	recHandle = DmNewRecord(*DBName, &recIndex, sizeof(InfoType));
	if(recHandle)
	{
		recPtr = MemHandleLock(recHandle);
		error = DmWrite(recPtr, 0, newRecordPtr, sizeof(InfoType));
		ErrFatalDisplayIf(error, "Could not write new Info record.");
		DmRecordInfo (*DBName, recIndex, &attr, NULL, NULL);
		attr &= ~dmRecAttrCategoryMask;				// Remove all category bits
		DmSetRecordInfo (*DBName, recIndex, &attr, NULL);
		MemPtrUnlock(recPtr);
		DmReleaseRecord(*DBName, recIndex, true);
	}
}


/***********************************************************************
 *
 * FUNCTION:    MmGenerateMiscDataRecord
 *
 * DESCRIPTION: This routine creates a new MiscDataType record at end of DB.
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/
void MmGenerateMiscDataRecord(DmOpenRef *DBName, MiscDataTypePtr newRecordPtr, UInt16 recIndex)
{
	MiscDataTypePtr		recPtr;
	MemHandle 				recHandle;
	Err						error;
	UInt16					attr;			// record attributes (category)
	
	// Create a new record in the database at 'recNumber' position
	recHandle = DmNewRecord(*DBName, &recIndex, sizeof(MiscDataType));
	if(recHandle)
	{
		recPtr = MemHandleLock(recHandle);
		error = DmWrite(recPtr, 0, newRecordPtr, sizeof(MiscDataType));
		ErrFatalDisplayIf(error, "Could not write new MiscDataType record.");
		DmRecordInfo (*DBName, recIndex, &attr, NULL, NULL);
		attr &= ~dmRecAttrCategoryMask;				// Remove all category bits
		DmSetRecordInfo (*DBName, recIndex, &attr, NULL);
		MemPtrUnlock(recPtr);
		DmReleaseRecord(*DBName, recIndex, true);
	}
}



/***********************************************************************
 *
 * FUNCTION:    MmGenerateServingCourseRecord
 *
 * DESCRIPTION: This routine creates a new ServingCourse record at end of DB.
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/
void MmGenerateServingCourseRecord(DmOpenRef *DBName, ServingCourseTypePtr newRecordPtr, UInt16 recIndex)
{
	ServingCourseTypePtr	recPtr;
	MemHandle 				recHandle;
	Err						error;
	UInt16					attr;			// record attributes (category)
	
	// Create a new record in the database at 'recNumber' position
	recHandle = DmNewRecord(*DBName, &recIndex, sizeof(ServingCourseType));
	if(recHandle)
	{
		recPtr = MemHandleLock(recHandle);
		error = DmWrite(recPtr, 0, newRecordPtr, sizeof(ServingCourseType));
		ErrFatalDisplayIf(error, "Could not write new ServingCourseType record.");
		DmRecordInfo (*DBName, recIndex, &attr, NULL, NULL);
		attr &= ~dmRecAttrCategoryMask;				// Remove all category bits
		DmSetRecordInfo (*DBName, recIndex, &attr, NULL);
		MemPtrUnlock(recPtr);
		DmReleaseRecord(*DBName, recIndex, true);
	}
}



/***********************************************************************
 *
 * FUNCTION:		MmGetServingCourseIndex
 *
 * DESCRIPTION:	Using servingCourseID, searches for record 
 *						If found, fills in recServingCourseIndex and color
 *						
 *
 * PARAMETERS:		
 *
 * RETURNED:		true if found, or false.
 *
 ***********************************************************************/
Boolean MmGetServingCourseIndex(DmOpenRef mmServingCourseDB, UInt16 servingCourseID,
											UInt16* recServingCourseIndex, UInt32* color, Boolean* selectable)
{
	MemHandle 					recHandle;
	UInt16						recIndex;
	UInt16						numRecords;
	ServingCourseTypePtr 	recServingCoursePtr;


	
	numRecords = DmNumRecords(mmServingCourseDB);
	
	for(recIndex=1; recIndex<numRecords; recIndex++)
	{
		recHandle = DmQueryRecord(mmServingCourseDB, recIndex);
		if(recHandle)
		{
			recServingCoursePtr = MemHandleLock(recHandle);

			if(recServingCoursePtr->uniqueID == servingCourseID	&&
				recServingCoursePtr->uniqueID != NoValue16				)
			{
				*recServingCourseIndex = recIndex;
				*color = recServingCoursePtr->color;
				*selectable = recServingCoursePtr->selectable;

				MemPtrUnlock(recServingCoursePtr);
				return true;
			}
			else
			{
				MemPtrUnlock(recServingCoursePtr);
			}
		}
	}

	//	Set defaults since serving course not found
	*recServingCourseIndex = NoValue16;
	*color = 0;
	*selectable = false;

	return false;
}



/***********************************************************************
 *
 * FUNCTION:		MmGetServingCourseIndex
 *
 * DESCRIPTION:	Using servingCourseID, searches for record 
 *						If found, fills in recServingCourseIndex and color
 *						
 *
 * PARAMETERS:		
 *
 * RETURNED:		true if found, or false.
 *
 ***********************************************************************/
UInt16 MmGetServingCourseID(DmOpenRef DBName, UInt16 recServingCourseIndex)
{
	MemHandle 					recHandle;
	ServingCourseTypePtr 	recServingCoursePtr;
	UInt16						servingCourseID;



	recHandle = DmQueryRecord(DBName, recServingCourseIndex);
	if(recHandle)
	{
		recServingCoursePtr = MemHandleLock(recHandle);

		servingCourseID = recServingCoursePtr->uniqueID;

		MemPtrUnlock(recServingCoursePtr);
		
		return servingCourseID;
	}
	else
	{
		return NoValue16;
	}
}




/***********************************************************************
 *
 * FUNCTION:		MmGetFirstServingCourseForMenu
 *
 * DESCRIPTION:	
 *						
 *						
 *
 * PARAMETERS:		
 *
 * RETURNED:		True if found, or False.
 *
 ***********************************************************************/
UInt16 MmGetFirstServingCourseForMenu(DmOpenRef DBName, UInt8 menuType, UInt16* servingCourseID, 
														UInt16* recServingCourseIndex, UInt32* color)
{
	MemHandle 					recHandle;
	Boolean						foundServCourse = false;
	UInt16						numServingCourses = 0;
	UInt16						recIndex;
	UInt16						numRecords;
	ServingCourseTypePtr 	recServingCoursePtr;


	*recServingCourseIndex = NoValue16;
	*servingCourseID = NoValue16;
	*color = 0;

	numRecords = DmNumRecords(DBName);
	
	for(recIndex=0; recIndex<numRecords; recIndex++)
	{
		recHandle = DmQueryRecord(DBName, recIndex);
		if(recHandle)
		{
			recServingCoursePtr = MemHandleLock(recHandle);

			if(recServingCoursePtr->menuType == menuType)
			{
				numServingCourses++;
				if(!foundServCourse)
				{
					foundServCourse = true;
					//	Preserve details of the first SC found
					*servingCourseID = recServingCoursePtr->uniqueID;
					*recServingCourseIndex = recIndex;
					*color = recServingCoursePtr->color;
				}
			}
			MemPtrUnlock(recServingCoursePtr);
		}
	}
	return numServingCourses;
}



/***********************************************************************
 *
 * FUNCTION:    MmGetStaffID
 *
 * DESCRIPTION: Retrieves the Prefs.staffIDNo by reading its value.
 *
 * PARAMETERS:  Index of Record to get
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
void MmGetStaffID(UInt16 type, UInt16* ptrID, UInt16 recIndex, DmOpenRef mmInfoDB)
{
	MemHandle				recHandle;
	InfoTypePtr				recInfoPtr;
	UInt16					numRecords;

	
	numRecords = DmNumRecords(mmInfoDB);

	*ptrID = 0;
	
	if(recIndex < numRecords)
	{
		// Get the next record in the current category (ie Course).
		recHandle = DmQueryRecord(mmInfoDB, recIndex);
		if(recHandle)
		{
			recInfoPtr = MemHandleLock(recHandle);
			if(type == StaffType)
			{
				*ptrID = recInfoPtr->number;
			}
			else if(type == PalmsType)
			{
				*ptrID = recInfoPtr->uniqueID;
			}
			MemPtrUnlock(recInfoPtr);
		}
	}
}


/***********************************************************************
 *
 * FUNCTION:		MmGetTableNameIndex
 *
 * DESCRIPTION:	Get the Record Index for this Table.
 *
 * PARAMETERS:		
 *
 * RETURNED:		
 *
 ***********************************************************************/
Boolean MmGetTableNameIndex(DmOpenRef mmTablenameDB, Char* numberStrPtr, UInt16 *recIndex)
{
	UInt16					number;
	
	SeatnameTypePtr		recPtr;
	MemHandle 				recHandle;
	UInt16					numRecords;
	
	//	Convert table number label back into a number
	if(numberStrPtr[0] == ' ')
		number = StrAToI(numberStrPtr+1);			// Avoid space char
	else
		number = StrAToI(numberStrPtr);				// Get the numeric value

	//	If a table name exists for this table write it into the label
	numRecords = DmNumRecords(mmTablenameDB);
	
	for(*recIndex=0; *recIndex < numRecords; (*recIndex)++)
	{
		recHandle = DmQueryRecord(mmTablenameDB, *recIndex);
		if(recHandle)										//	Not all tables have names
		{
			recPtr = MemHandleLock(recHandle);
			if(recPtr->tableNumber == number)
			{
				MemPtrUnlock(recPtr);
				return true;										//	No point carrying on
			}
			MemPtrUnlock(recPtr);
		}
	}
	return false;
}


/***********************************************************************
 *
 * FUNCTION:		MmGetSeatnameIndexAndUpdate
 *
 * DESCRIPTION:	Get the Record Index for this Seatname and update if found
 *						on the basis of markForDeletion.
 *
 * PARAMETERS:		
 *
 * RETURNED:		false if unfound
 *
 ***********************************************************************/
Boolean MmGetSeatnameIndexAndUpdate(DBTypePtr dBs, UInt16 tableNumber, UInt16 seatNumber,
									UInt16 *recIndex, Char* seatName, Boolean markForDeletion)
{
	SeatnameTypePtr		recSeatnamePtr;
	MemHandle 				recHandle;
	char						null = 0;
	Err						error;
	Boolean					found = false;
	
	//	See if we can find the record
	*recIndex = MmFindSortSeatname(tableNumber, seatNumber);

	recHandle = DmQueryRecord(dBs->mmSeatnameDB, *recIndex);
	if(recHandle)
	{
		recSeatnamePtr = MemHandleLock(recHandle);

		//	Confirm its the correct record
		if(recSeatnamePtr->tableNumber == tableNumber &&
			recSeatnamePtr->seatNumber == seatNumber)
		{
			//	Found it and it needs to be replaced with the passed string
			found = true;
			
			if((markForDeletion == ReplaceSeatname) || (markForDeletion == MarkForDelete))
			{
				recHandle = DmQueryRecord(dBs->mmSeatnameDB, *recIndex);
				if(recHandle)
				{
					recHandle = DmGetRecord(dBs->mmSeatnameDB, *recIndex);
					recSeatnamePtr = MemHandleLock(recHandle);
	
					error = DmWrite(recSeatnamePtr,
						(UInt8*)&recSeatnamePtr->name - (UInt8*)&recSeatnamePtr->tableNumber,
						seatName, StrLen(seatName)+1);
					ErrFatalDisplayIf(error, "Could not write modified Seatname record.");
	
					MemPtrUnlock(recSeatnamePtr);
					DmReleaseRecord(dBs->mmSeatnameDB, *recIndex, true);
				}
			}
		}

		MemPtrUnlock(recSeatnamePtr);
	}
	return found;
}



/***********************************************************************
 *
 * FUNCTION:		MmZapDB
 *
 * DESCRIPTION:	Nuke the requested Database without asking.
 *
 * PARAMETERS:		Nothing.
 *
 * RETURNED:		true if successful.
 *
 ***********************************************************************/
Boolean MmZapDB(DmOpenRef *DBName, Char* ptrName, Boolean recreate, 
							UInt16 *dBDeleteBitMask, UInt16 dBToDelete)
{
	Int16			error = 0;					// P5. error code
	UInt16		cardNo;						// P5. card containing the application database
	LocalID		dbID;							// P5. handle for application database
	UInt16		dbAttrs;						// P5. database attributes
	UInt16		mode;							// P5. permissions when opening the database
//	SystemPreferencesType sysPrefs;		// P10. The system's preferences
	Boolean 		recordFound = false;		// P15. whether we use the record used before
													// This will be false if the record is now hidden.





	//	If this DB has already been deleted this time, don't try to do it again
	//	If you try, you will probably get a 'Free Handle' crash
	if(*dBDeleteBitMask & dBToDelete)
	{
		return false;
	}


	mode = dmModeReadWrite;

	//	Delete the database.
//	DmOpenDatabaseInfo(*DBName, &dbID, NULL, NULL, &cardNo, NULL);
//	error = DmDeleteDatabase(cardNo, dbID);

	dbID = DmFindDatabase(0, ptrName);

	if(dbID)
	{
		error = DmDeleteDatabase(0, dbID);

		if(!error)
		{
			*dBDeleteBitMask |= dBToDelete;
		}
		else if(error == dmErrDatabaseOpen)
		{
			DmCloseDatabase(*DBName);
			error = DmDeleteDatabase(0, dbID);

			if(!error)
			{
				*dBDeleteBitMask |= dBToDelete;
			}
			if(error)
			{
				ErrFatalDisplayIf(error, "Could not delete Database.");
			}
		}
		else
		{
			ErrFatalDisplayIf(error, "Could not delete Database.");
		}
	}
	else
	{
		return true;
	}

//ShowDebug("%Lx %x", *DBName, *dBDeleteBitMask);

	//	Recreate the database if required
	if(recreate)
	{
		error = DmCreateDatabase(0, ptrName, MmAppType, MmDBType, false);
		ErrFatalDisplayIf(error, "Could not Create Database.");
	
		// Check for error.  If the database couldn't be created report it.
	//	if (error)
	//		return true;
		
		// Find the new database.
		dbID = DmFindDatabase(0, ptrName);
	
		*DBName = DmOpenDatabase(0, dbID, mode);
	
		// Get info about the database
		error = DmOpenDatabaseInfo(*DBName, &dbID, NULL, NULL, &cardNo, NULL);
		ErrFatalDisplayIf(error, "Could not get database info.");
	
		// Get attributes for the database
		error = DmDatabaseInfo(0, dbID, NULL, &dbAttrs, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
		ErrFatalDisplayIf(error, "Could not get database info.");
	
		// Set the backup flag in the attributes
		// This will ensure that the database will always get backed up
		// during HotSync, even if there is no conduit for this application.
		dbAttrs = dbAttrs | dmHdrAttrBackup;
	
		// Set the new attributes in the database
		error = DmSetDatabaseInfo(0, dbID, NULL, &dbAttrs, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
		ErrFatalDisplayIf(error, "Could not set database info.");
	

		MmInitialiseNewlyCreatedDB(*DBName);
	
	}	
	
	return true;
}


/***********************************************************************
 *
 * FUNCTION:    MmDeleteAllTypeRecordsForMiscDataType
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    Number of records deleted
 *
 ***********************************************************************/
UInt16 MmDeleteAllTypeRecordsForMiscDataType(DmOpenRef DBName, UInt16 recordType)
{
	MemHandle				recHandle;
	UInt16					recIndex;
	MiscDataTypePtr		recMiscDataPtr;
	UInt16					numRecords;
	UInt16					error;
	UInt16					numRemoved;


	numRecords = DmNumRecords(DBName);
	recIndex=0;
	numRemoved=0;
	while(recIndex < numRecords)
	{		
		recHandle = DmQueryRecord(DBName, recIndex);
		if(recHandle)
		{
			recMiscDataPtr = MemHandleLock(recHandle);

			if(recMiscDataPtr->recordType == recordType)
			{
				MemPtrUnlock(recMiscDataPtr);
				error = DmRemoveRecord(DBName, recIndex);	//	Nuke the record
				ErrFatalDisplayIf(error, "Could not remove record");
				numRecords--;
				numRemoved++;
			}
			else
			{
				MemPtrUnlock(recMiscDataPtr);
				recIndex++;
			}
		}
	}
	return numRemoved;
}



/***********************************************************************
 *
 * FUNCTION:    MmDeleteAllServingCourseRecordsForMenu
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    Number of records deleted
 *
 ***********************************************************************/
UInt16 MmDeleteAllServingCourseRecordsForMenu(DmOpenRef DBName, UInt16 menuType)
{
	MemHandle				recHandle;
	UInt16					recIndex;
	ServingCourseTypePtr	recServingCoursePtr;
	UInt16					numRecords;
	UInt16					error;
	UInt16					numRemoved;


	numRecords = DmNumRecords(DBName);
	recIndex=0;
	numRemoved=0;
	while(recIndex < numRecords)
	{		
		recHandle = DmQueryRecord(DBName, recIndex);
		if(recHandle)
		{
			recServingCoursePtr = MemHandleLock(recHandle);

			if(recServingCoursePtr->menuType == menuType)
			{
				MemPtrUnlock(recServingCoursePtr);
				error = DmRemoveRecord(DBName, recIndex);	//	Nuke the record
				ErrFatalDisplayIf(error, "Could not remove record");
				numRecords--;
				numRemoved++;
			}
			else
			{
				MemPtrUnlock(recServingCoursePtr);
				recIndex++;
			}
		}
	}
	return numRemoved;
}

