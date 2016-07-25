#define EXTERNAL 1

#include "MenuMate.h"

/***********************************************************************
 *
 * FUNCTION:     MmOpenOrCreateAllDBs
 *
 * DESCRIPTION:  
 *               
 *
 * PARAMETERS:   None.
 *
 * RETURNED:     None
 *
 ***********************************************************************/
Boolean MmOpenOrCreateAllDBs(UInt16 initialiseMode)
{
	if(MmOpenOrCreateDB(&MmOrdersDB, MmOrdersDBName, &DBs.mmOrdersDB))
		return true;												//	Crash out on error


	MmRefreshAutoLogout(true, 0);			//	Update the Prefs.autoLogoutSecs global


//	Do next for MmInfoDB
	if(!(initialiseMode & InitZapAllDBsNotInfo))
		if(MmOpenOrCreateDB(&MmInfoDB, MmInfoDBName, &DBs.mmInfoDB))
			return true;											//	Crash out on error

//	Do next for MmMiscDataDB
	if(MmOpenOrCreateDB(&MmMiscDataDB, MmMiscDataDBName, &DBs.mmMiscDataDB))
		return true;												//	Crash out on error

//	Do next for MmDishesDB
	if(MmOpenOrCreateDB(&MmDishesDB, MmDishesDBName, &DBs.mmDishesDB))
		return true;												//	Crash out on error

//	Do next for MmDrinksSizesDB
	if(MmOpenOrCreateDB(&MmDishesSizesDB, MmDishesSizesDBName, &DBs.mmDishesSizesDB))
		return true;												//	Crash out on error

//	Do next for MmDishesOptionsDB
	if(MmOpenOrCreateDB(&MmDishesOptionsDB, MmDishesOptionsDBName, &DBs.mmDishesOptionsDB))
		return true;												//	Crash out on error

//	Do next for MmDishesForcedSidesDB
	if(MmOpenOrCreateDB(&MmDishesForcedSidesDB, MmDishesForcedSidesDBName, &DBs.mmDishesForcedSidesDB))
		return true;												//	Crash out on error

//	Do next for MmDrinksDB
	if(MmOpenOrCreateDB(&MmDrinksDB, MmDrinksDBName, &DBs.mmDrinksDB))
		return true;												//	Crash out on error

//	Do next for MmDrinksSizesDB
	if(MmOpenOrCreateDB(&MmDrinksSizesDB, MmDrinksSizesDBName, &DBs.mmDrinksSizesDB))
		return true;												//	Crash out on error

//	Do next for MmDrinksOptionsDB
	if(MmOpenOrCreateDB(&MmDrinksOptionsDB, MmDrinksOptionsDBName, &DBs.mmDrinksOptionsDB))
		return true;												//	Crash out on error

//	Do next for MmDrinksForcedSidesDB
	if(MmOpenOrCreateDB(&MmDrinksForcedSidesDB, MmDrinksForcedSidesDBName, &DBs.mmDrinksForcedSidesDB))
		return true;												//	Crash out on error

//	Do next for MmServingCourseDB
	if(MmOpenOrCreateDB(&MmServingCourseDB, MmServingCourseDBName, &DBs.mmServingCourseDB))
		return true;												//	Crash out on error

//	Do next for MmTablenameDB
	if(MmOpenOrCreateDB(&MmTablenameDB, MmTablenameDBName, &DBs.mmTablenameDB))
		return true;												//	Crash out on error

//	Do next for MmSeatnameDB
	if(MmOpenOrCreateDB(&MmSeatnameDB, MmSeatnameDBName, &DBs.mmSeatnameDB))
		return true;												//	Crash out on error



	NumOldUnsentOrders = MmMarkOrDeleteUnsentOrders(MmOrdersDB, &Prefs, CountOldOrders);

	Prefs.numUnsentOrders = MmNumUnsentOrdersDB(MmOrdersDB);

	return false;
}




/***********************************************************************
 *
 * FUNCTION:    MmSavePrefs
 *
 * DESCRIPTION: Save off the Prefs stored in the global Prefs struct.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
void MmSavePrefs(void)
{
	// Write the state information.
	PrefSetAppPreferences (MmAppType, MmPrefID, MmVersionNum, &Prefs, 
		sizeof(MmPreferenceType), true);
}


/***********************************************************************
 *
 * FUNCTION:    MmInitialiseNewlyCreatedDB
 *
 * DESCRIPTION: Initialise a Database with the Name passed
 *						
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/
void MmInitialiseNewlyCreatedDB(DmOpenRef dBName)
{
	if(dBName == MmOrdersDB)
	{
		Prefs.numUnsentOrders		=	0;
		DBs.mmOrdersDB					= MmOrdersDB;
		MmCreateFinalRecord(&Prefs, &DBs, &LastItemOrdered);
		MmSavePrefs();
	}


	else if(dBName == MmInfoDB)
	{
		InfoType infoRecord			= {StaffType, 0, 0, "Login", "***"};

		DBs.mmInfoDB					= MmInfoDB;
		MmGenerateInfoRecord(&MmInfoDB, &infoRecord, 0);

		Prefs.hLStatus.HLInfoSyncStatus = HLStatusToSend;
//		Prefs.transactionNo			=	0;
		Prefs.infoSyncNo				=	0;
		Prefs.staffIDNo				=	0;
		Prefs.staffIDIndex	 		=	0;
		Prefs.loggedIn					=	false;
	}


	else if(dBName == MmDishesDB)
	{
		Prefs.menuSyncNo				=	0;
		DBs.mmDishesDB					= MmDishesDB;
		Prefs.hLStatus.HLMenuSyncStatus = HLStatusToSend;			//	Need Menu Sync
	}


	else if(dBName == MmDishesSizesDB)
	{
		SizeType sizeRecord 			= {0, "None Available"};		//	Keep it local

		DBs.mmDishesSizesDB			= MmDishesSizesDB;
		MmGenerateSizeMenuRecord(&MmDishesSizesDB, &sizeRecord, 0);
	}

	else if(dBName == MmDishesOptionsDB)
	{
		OptionsType optionsRecord	= {0, 0, 0, 0, "Dishes Options"};

		DBs.mmDishesOptionsDB		= MmDishesOptionsDB;
		MmGenerateOptionsRecord(&MmDishesOptionsDB, &optionsRecord, 0);
	}



	else if(dBName == MmDrinksDB)
	{
		Prefs.drinkSyncNo				=	0;
		DBs.mmDrinksDB					= MmDrinksDB;
		Prefs.hLStatus.HLDrinkSyncStatus = HLStatusToSend;			//	Need Drink Sync
	}

	else if(dBName == MmDrinksSizesDB)
	{
		SizeType sizeRecord			= {0, "None Available"};

		DBs.mmDrinksSizesDB			= MmDrinksSizesDB;
		MmGenerateSizeMenuRecord(&MmDrinksSizesDB, &sizeRecord, 0);
	}

	else if(dBName == MmDrinksOptionsDB)
	{
		OptionsType optionsRecord	= {0, 0, 0, 0, "Drinks Options"};

		DBs.mmDrinksOptionsDB		= MmDrinksOptionsDB;
		MmGenerateOptionsRecord(&MmDrinksOptionsDB, &optionsRecord, 0);
	}


	else if(dBName == MmServingCourseDB)
	{
		ServingCourseType servingCourseRecord = {ServingType, NoValue16, NoValue16, false, 0, "Serving", "* Serving"};

		DBs.mmServingCourseDB		= MmServingCourseDB;
		MmGenerateServingCourseRecord(&MmServingCourseDB, &servingCourseRecord, 0);
	}


	else if(dBName == MmTablenameDB)
	{
		SeatnameType seatnameRecord = {0, 0, 0, "Table Names"};

		DBs.mmTablenameDB				= MmTablenameDB;
		MmGenerateSeatOrTableRecord(&MmTablenameDB, &seatnameRecord, 0);
	}

	else if(dBName == MmSeatnameDB)
	{
		SeatnameType seatnameRecord = {0, 0, 0, "Seat Names"};

		DBs.mmSeatnameDB				= MmSeatnameDB;
		MmGenerateSeatOrTableRecord(&MmSeatnameDB, &seatnameRecord, 0);
	}

	DBs.mmDishesForcedSidesDB		= MmDishesForcedSidesDB;
	DBs.mmDrinksForcedSidesDB		= MmDrinksForcedSidesDB;
	DBs.mmMiscDataDB					= MmMiscDataDB;
//	DBs.memoDB							= MemoDB;
}



/************************************************************
 *
 *  FUNCTION:    MmCompareItemsUniqueID
 *
 *  DESCRIPTION: Compare two Menu records.
 *
 *  PARAMETERS:  r1    - database record 1
 *				     r2    - database record 2
 *
 *  RETURNS:    -1 if record one is less
 *		           1 if record two is less
 *
 *************************************************************/ 
static Int16 MmCompareItemsUniqueID(MenuTypePtr r1, MenuTypePtr r2)
{
	if(r1->uniqueID < r2->uniqueID)
		return -1;
	else if(r1->uniqueID > r2->uniqueID)
		return 1;
	else
		return 0;
}


/************************************************************
 *
 *  FUNCTION:    MmCompareDrinksUniqueID
 *
 *  DESCRIPTION: Compare two Menu records.
 *
 *  PARAMETERS:  r1    - database record 1
 *				     r2    - database record 2
 *
 *  RETURNS:    -1 if record one is less
 *		           1 if record two is less
 *
 *************************************************************/ 
/*static Int16 MmCompareDrinksUniqueID(MenuTypePtr r1, MenuTypePtr r2)
{
	if(r1->uniqueID < r2->uniqueID)
		return -1;
	else if(r1->uniqueID > r2->uniqueID)
		return 1;
	else
		return 0;
}
*/

/************************************************************
 *
 *  FUNCTION:    MmCompareMenuDBRecords
 *
 *  DESCRIPTION: Compare two Menu records.
 *
 *  PARAMETERS:  r1    - database record 1
 *				     r2    - database record 2
 *               extra - extra data, not used in the function
 *
 *  RETURNS:    -1 if record one is less
 *		           1 if record two is less
 *
 *  WARNING!    As this function is used for both Dishes and Courses the
 *              Dish and Course numbers must be in the same positions.
 *
 *************************************************************/ 
static Int16 MmCompareCourseDish (MenuTypePtr r1, MenuTypePtr r2/*, Int16 sortType*/)
//			,SortRecordInfoPtr info1, SortRecordInfoPtr info2, MemHandle appInfoH)
{
	if(r1->courseNumber < r2->courseNumber)
		return -1;
	else if(r1->courseNumber > r2->courseNumber)
		return 1;
	else if(r1->itemNumber < r2->itemNumber)
		return -1;
	else if(r1->itemNumber > r2->itemNumber)
		return 1;
	else
		return 0;
}


/************************************************************
 *
 *  FUNCTION:    MmCompareCourseOption
 *
 *  DESCRIPTION: Compare two Option records.
 *
 *  PARAMETERS:  r1    - database record 1
 *				     r2    - database record 2
 *               extra - extra data, not used in the function
 *
 *  RETURNS:    -1 if record one is less
 *		           1 if record two is less
 *
 *  WARNING!    As this function is used for both Options and Courses the
 *              Option and Course numbers must be in the same positions.
 *
 *************************************************************/ 
static Int16 MmCompareCourseOption (OptionsTypePtr r1, OptionsTypePtr r2/*, Int16 sortType*/)
//			,SortRecordInfoPtr info1, SortRecordInfoPtr info2, MemHandle appInfoH)
{
	if(r1->courseNumber < r2->courseNumber)
		return -1;
	else if(r1->courseNumber > r2->courseNumber)
		return 1;
	else if(r1->courseOptionNumber < r2->courseOptionNumber)
		return -1;
	else if(r1->courseOptionNumber > r2->courseOptionNumber)
		return 1;
	else
		return 0;
}


/************************************************************
 *
 *  FUNCTION:    MmCompareTableSeat
 *
 *  DESCRIPTION: Compare two Seat records.
 *
 *  PARAMETERS:  r1    - database record 1
 *				     r2    - database record 2
 *               extra - extra data, not used in the function
 *
 *  RETURNS:    -1 if record one is less
 *		           1 if record two is less
 *
 *************************************************************/ 
static Int16 MmCompareTableSeat (OrderTypePtr r1, OrderTypePtr r2/*, Int16 sortType*/)
//			,SortRecordInfoPtr info1, SortRecordInfoPtr info2, MemHandle appInfoH)
{
	if(r1->tableNumber < r2->tableNumber)
		return -1;
	else if(r1->tableNumber > r2->tableNumber)
		return 1;
	else if(r1->seatNumber < r2->seatNumber)
		return -1;
	else if(r1->seatNumber > r2->seatNumber)
		return 1;
	else
		return -1;
}


/************************************************************
 *
 *  FUNCTION:    MmCompareSeatname
 *
 *  DESCRIPTION: Compare two Seatname records.
 *
 *  PARAMETERS:  r1    - database record 1
 *				     r2    - database record 2
 *               extra - extra data, not used in the function
 *
 *  RETURNS:    -1 if record one is less
 *		           1 if record two is less
 *
 *************************************************************/ 
static Int16 MmCompareSeatname (SeatnameTypePtr r1, SeatnameTypePtr r2)
{
	if(r1->tableNumber < r2->tableNumber)
		return -1;
	else if(r1->tableNumber > r2->tableNumber)
		return 1;
	else if(r1->seatNumber < r2->seatNumber)
		return -1;
	else if(r1->seatNumber > r2->seatNumber)
		return 1;
	else
		return -1;
}


/************************************************************
 *
 *  FUNCTION:    MmCompareDB
 *
 *  DESCRIPTION: Compare two records for Table, Seat, Course, Dish, DishCounter.
 *
 *  PARAMETERS:  r1    - database record 1
 *				     r2    - database record 2
 *               extra - extra data, not used in the function
 *
 *  RETURNS:    -1 if record one is less
 *		           1 if record two is less
 *
 *************************************************************/ 
Int16 MmCompareDB (OrderTypePtr r1, OrderTypePtr r2/*, Int16 sortType*/)
//			,SortRecordInfoPtr info1, SortRecordInfoPtr info2, MemHandle appInfoH)
{
	if(r1->tableNumber < r2->tableNumber)
		return -1;
	else if(r1->tableNumber > r2->tableNumber)
		return 1;
	else if(r1->seatNumber < r2->seatNumber)
		return -1;
	else if(r1->seatNumber > r2->seatNumber)
		return 1;
	else if(r1->menuType < r2->menuType)
		return -1;
	else if(r1->menuType > r2->menuType)
		return 1;
	else if(r1->courseNumber < r2->courseNumber)
		return -1;
	else if(r1->courseNumber > r2->courseNumber)
		return 1;
	else if(r1->itemNumber < r2->itemNumber)
		return -1;
	else if(r1->itemNumber > r2->itemNumber)
		return 1;
	else if(r1->itemCounter < r2->itemCounter)
		return -1;
	else if(r1->itemCounter > r2->itemCounter)
		return 1;
//	else if(r1->size < r2->size)
//		return -1;
//	else if(r1->size > r2->size)
//		return 1;
	else
		return 0;
}


/************************************************************
 *
 *  FUNCTION:    MmCompareCallAway
 *
 *  DESCRIPTION: Compare two CallAway records.
 *
 *  PARAMETERS:  r1    - database record 1
 *				     r2    - database record 2
 *               extra - extra data, not used in the function
 *
 *  RETURNS:    -1 if record one is less
 *		           1 if record two is less
 *
 *************************************************************/ 
static Int16 MmCompareCallAway (MiscDataTypePtr r1, MiscDataTypePtr r2)
{
	if(r1->recordType < r2->recordType)
		return -1;
	else if(r1->recordType > r2->recordType)
		return 1;
	else if(r1->number < r2->number)		//	= Table
		return -1;
	else if(r1->number > r2->number)
		return 1;
	else if(r1->recIndex < r2->recIndex)
		return -1;
	else if(r1->recIndex > r2->recIndex)
		return 1;
	else
		return -1;
}


/***********************************************************************
 *
 * FUNCTION:    MmFindSortItemsUniqueID
 *
 * DESCRIPTION: Find Index Position in MmDishesDB where uniqueID is
 *
 * PARAMETERS:  
 *
 * RETURNED:    Index Position in MmDishesDB where uniqueID is
 *
 ***********************************************************************/
UInt16 MmFindSortItemsUniqueID(DmOpenRef *DBName, UInt16 uniqueID)
{
	MenuType	recDummy	=	{0,0,0,0,0,0,0,0};

	recDummy.uniqueID = uniqueID;	//	Give it something to search on
	
	//	Sort through the Dishes database looking for the UniqueID
	return DmFindSortPosition(DBName, &recDummy, NULL,
										(DmComparF *)MmCompareItemsUniqueID, NULL) - 1;
}


/***********************************************************************
 *
 * FUNCTION:    MmFindSortDrinksUniqueID
 *
 * DESCRIPTION: Find Index Position in MmDrinksDB where uniqueID is
 *
 * PARAMETERS:  
 *
 * RETURNED:    Index Position in MmDrinksDB where uniqueID is
 *
 ***********************************************************************/
/*UInt16 MmFindSortDrinksUniqueID(UInt16 uniqueID)
{
	MenuType	recDummy	=	{0,0,0,0,0,0,0,0};

	recDummy.uniqueID = uniqueID;	//	Give it something to search on
	
	//	Sort through the Dishes database looking for the UniqueID
	return DmFindSortPosition(MmDrinksDB, &recDummy, NULL,
										(DmComparF *)MmCompareDrinksUniqueID, NULL) - 1;
}
*/

/***********************************************************************
 *
 * FUNCTION:    FindSortTableSeat
 *
 * DESCRIPTION: Find Index Position in MmOrdersDB where start of Table/Seat
 *						records are
 *
 * PARAMETERS:  None. Uses globals Prefs.current.tableNo and Prefs.current.seatNo
 *              and MmOrdersDB
 *
 * RETURNED:    Index Position in MmOrdersDB where start of Table/Seat
 *              records are.
 *
 ***********************************************************************/
UInt16 MmFindSortTableSeat(UInt16 tableNumber, UInt16 seatNumber)
{
	OrderType		recMarker	=	{0,0,0,0,0,0,0,0};

	recMarker.tableNumber = tableNumber;	//	Make it a 'Marker' record
	recMarker.seatNumber = seatNumber;
	
	//	Sort through the Orders database in the order: Table/Seat
	return DmFindSortPosition(MmOrdersDB, &recMarker, NULL,
										(DmComparF *)MmCompareTableSeat, NULL);
}


/***********************************************************************
 *
 * FUNCTION:    FindSortTableSeat
 *
 * DESCRIPTION: Find Index Position in MmOrdersDB where start of Table/Seat
 *						records are
 *
 * PARAMETERS:  UInt16 tableNumber, UInt16 seatNumber, DmOpenRef *DBName (MmOrdersDB)
 *
 * RETURNED:    Index Position in MmOrdersDB where start of Table/Seat
 *              records are.
 *
 ***********************************************************************/
UInt16 MmFindSortTableSeatExt(DmOpenRef DBName, UInt16 tableNumber, UInt16 seatNumber)
{
	OrderType		recMarker	=	{0,0,0,0,0,0,0,0};

	recMarker.tableNumber = tableNumber;	//	Make it a 'Marker' record
	recMarker.seatNumber = seatNumber;
	
	//	Sort through the Orders database in the order: Table/Seat
	return DmFindSortPosition(DBName, &recMarker, NULL,
										(DmComparF *)MmCompareTableSeat, NULL);
}


/***********************************************************************
 *
 * FUNCTION:    FindSortSeatname
 *
 * DESCRIPTION: Find Index Position in MmSeatnameDB
 *
 * PARAMETERS:  
 *
 * RETURNED:    Index Position in MmSeatnameDB where Table/Seat record goes
 *
 ***********************************************************************/
UInt16 MmFindSortSeatname(UInt16 tableNumber, UInt16 seatNumber)
{
	SeatnameType		record;
	
	record.tableNumber = tableNumber;
	record.seatNumber = seatNumber;
	
	//	Sort through the Orders database in the order: Table/Seat
	return DmFindSortPosition(MmSeatnameDB, &record, NULL,
										(DmComparF *)MmCompareSeatname, NULL);
}



/***********************************************************************
 *
 * FUNCTION:    MmFindSortCallAway
 *
 * DESCRIPTION: Find Index Position in MmMessageDB
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/
UInt16 MmFindSortCallAway(UInt16 table, UInt16 servingCourseIndex)
{
	MiscDataType			record;
	
	record.recordType = CallAwayType;
	record.number = table;
	record.recIndex = servingCourseIndex;
	
	//	Sort through the Info database in the order: Table/Course
	return DmFindSortPosition(MmMiscDataDB, &record, NULL,
										(DmComparF *)MmCompareCallAway, NULL);
}



/***********************************************************************
 *
 * FUNCTION:		MmQueryNextInCourse
 *
 * DESCRIPTION:	Search the Database for next record of this courseNumber.
 *
 * PARAMETERS:		Nothing.
 *
 * RETURNED:		true if successful.
 *
 ***********************************************************************/
/*MemHandle = MmQueryNextInCourse((DmOpenRef *DBName, UInt16* recIndex, UInt16 courseNumber);
{
	UInt16				numRecords;

	numRecords = DmNumRecords(*DBName);

	for(;*recIndex < numRecords; recIndex++)
	{		
		recHandle = DmQueryRecord(*DBName, recIndex);
		// If record is found, store the pointer to the item string in the list array.
		if(recHandle)
		{
			recPtr = MemHandleLock(recHandle);
			MemPtrUnlock(recPtr);
			return recHandle;
		}
	}
	return NULL;
}
*/

/***********************************************************************
 *
 * FUNCTION:		MmFindCurrentRecordIndex
 *
 * DESCRIPTION:	Find current Order record and update recIndex & Prefs.current.record.
 *
 * PARAMETERS:		MemPtr to recIndex, Boolean updateDishCounter. Uses Prefs.curOrder
 *
 * RETURNED:		True if record found. Index into MmOrdersDB updated.
 *
 ***********************************************************************/
Boolean MmFindCurrentRecordIndex(UInt16* recIndexPtr, Boolean newDishCounter,
																				Boolean setPriceAdjust)
{
	MemHandle 			recHandle;
	UInt16				recIndex;
	OrderTypePtr 		recPtr;

	Boolean				wasFound = false;

	//	First find the correct position in the DB
	//	Then check previous dish to see if it is of this type.
	//	If so, set up itemCounter as appropriate

	//	Sort through the Orders database in the order: Table/Seat/Course/Dish/Counter
	//	NB!!!	For new orders (itemCounter = 255) this returns the index for the new order
	//			For existing orders, returns index 1 past the matching order!!!
	recIndex = DmFindSortPosition(MmOrdersDB, &Prefs.curOrder, NULL,
										(DmComparF *)MmCompareDB, NULL);

	//	If it was a FindOnly, point to the found item
	if(!newDishCounter)
		recIndex--;							//	recIndex points to just past our record
	
	recHandle = DmQueryRecord(MmOrdersDB, recIndex);
	if(recHandle)
	{
		recPtr = MemHandleLock(recHandle);
		
		if(recPtr->tableNumber	== Prefs.curOrder.tableNumber		&&
			recPtr->seatNumber	== Prefs.curOrder.seatNumber		&&
			recPtr->menuType		== Prefs.curOrder.menuType			&&
			recPtr->courseNumber	== Prefs.curOrder.courseNumber	&&
			recPtr->itemNumber	== Prefs.curOrder.itemNumber		&&
			recPtr->itemCounter	== Prefs.curOrder.itemCounter			)
		{
			wasFound = true;

			if(setPriceAdjust)
			{
//				Prefs.curOrder.priceAdj = recPtr->priceAdj;
			}
		}

		MemPtrUnlock(recPtr);
	}

	//	Call specified that the itemCounter should be set up in Prefs.curOrder
	if(newDishCounter)
	{
		//	Check previous record for same dish type
		recHandle = DmQueryRecord(MmOrdersDB, recIndex - 1);
		if(recHandle)
		{
			recPtr = MemHandleLock(recHandle);
	
			//	If previous record is same dish type, make itemCounter 1 greater than its value
			if(recPtr->tableNumber	== Prefs.curOrder.tableNumber		&&
				recPtr->seatNumber	== Prefs.curOrder.seatNumber		&&
				recPtr->menuType		== Prefs.curOrder.menuType			&&
				recPtr->courseNumber	== Prefs.curOrder.courseNumber	&&
				recPtr->itemNumber	== Prefs.curOrder.itemNumber)
			{
				Prefs.curOrder.itemCounter = recPtr->itemCounter + 1;
			}
			else
			{
				Prefs.curOrder.itemCounter = 0;
			}

			MemPtrUnlock(recPtr);
		}
		else	//	No record was found
		{
			Prefs.curOrder.itemCounter = 0;
		}
	}

	if(wasFound)
	{
		Prefs.current.recIndex = recIndex;			//	This is what we came for

		//	This is our exact order so return index to it
		*recIndexPtr = recIndex;
		return true;
	}
	else
	{
		//	We didn't find it, but a new record should go here anyway
		*recIndexPtr = recIndex;
		return false;
	}
}





/***********************************************************************
 *
 * FUNCTION:    MmFindDishOrDrinkIndex
 *
 * DESCRIPTION: Use the passed course and dish numbers to find the index.
 *
 * PARAMETERS:  Course, Dish, DB
 *
 * RETURNED:    Index to Dish/Drink
 *
 ***********************************************************************/
UInt16 MmFindDishOrDrinkIndex(DmOpenRef *DBName, UInt16 courseNumber, UInt16 itemNumber)
{
	UInt16					recMenuIndex;
	MenuType					recMenuTemp	=	{0,0,0,0};	//	Temporary struct for DB search


	recMenuTemp.courseNumber	= courseNumber;
	recMenuTemp.itemNumber		= itemNumber;

	recMenuIndex = DmFindSortPosition(DBName, &recMenuTemp, NULL,
								(DmComparF *)MmCompareCourseDish, NULL) - 1;
	return recMenuIndex;
}



/***********************************************************************
 *
 * FUNCTION:    MmFindCourseOptionIndex
 *
 * DESCRIPTION: Use the passed course and option numbers to find the index.
 *
 * PARAMETERS:  Course, Option, DB
 *
 * RETURNED:    Index to Option
 *
 ***********************************************************************/
UInt16 MmFindCourseOptionIndex(DmOpenRef *DBName, UInt16 courseNumber, UInt16 optionNumber)
{
	UInt16					recOptionIndex;
	OptionsType				recOptionTemp	=	{0,0};	//	Temporary struct for DB search


	recOptionTemp.courseNumber	= courseNumber;
	recOptionTemp.courseOptionNumber	= optionNumber;

	recOptionIndex = DmFindSortPosition(DBName, &recOptionTemp, NULL,
								(DmComparF *)MmCompareCourseOption, NULL) - 1;
	return recOptionIndex;
}




/***********************************************************************
 *
 * FUNCTION:		MmZapAllDBs
 *
 * DESCRIPTION:	Nuke All Databases without asking.
 *
 * PARAMETERS:		Nothing.
 *
 * RETURNED:		
 *
 ***********************************************************************/
void MmZapAllDBs(UInt16 initialiseMode)
{
	MmZapDB(&MmOrdersDB, 				MmOrdersDBName,	 				NoRecreateDB, &DBDeleteBitMask, MmOrdersMaskBit);
	
	if(!(initialiseMode & InitZapAllDBsNotInfo))
		MmZapDB(&MmInfoDB, 					MmInfoDBName, 						NoRecreateDB, &DBDeleteBitMask, MmInfoMaskBit);
	
	MmZapDB(&MmMiscDataDB,				MmMiscDataDBName,					NoRecreateDB, &DBDeleteBitMask, MmMiscDataMaskBit);
	
	MmZapDB(&MmDishesDB, 				MmDishesDBName, 					NoRecreateDB, &DBDeleteBitMask, MmDishesMaskBit);
	
	MmZapDB(&MmDishesSizesDB, 			MmDishesSizesDBName, 			NoRecreateDB, &DBDeleteBitMask, MmDishesSizesMaskBit);
	
	MmZapDB(&MmDishesOptionsDB,		MmDishesOptionsDBName,		 	NoRecreateDB, &DBDeleteBitMask, MmDishesOptionsMaskBit);
	
	MmZapDB(&MmDishesForcedSidesDB,	MmDishesForcedSidesDBName, 	NoRecreateDB, &DBDeleteBitMask, MmDishesForcedSidesMaskBit);

	MmZapDB(&MmDrinksDB, 				MmDrinksDBName,					NoRecreateDB, &DBDeleteBitMask, MmDrinksMaskBit);
	
	MmZapDB(&MmDrinksSizesDB,	 		MmDrinksSizesDBName, 			NoRecreateDB, &DBDeleteBitMask, MmDrinksSizesMaskBit);
	
	MmZapDB(&MmDrinksOptionsDB,		MmDrinksOptionsDBName,		 	NoRecreateDB, &DBDeleteBitMask, MmDrinksOptionsMaskBit);
	
	MmZapDB(&MmDrinksForcedSidesDB,	MmDrinksForcedSidesDBName, 	NoRecreateDB, &DBDeleteBitMask, MmDrinksForcedSidesMaskBit);
	
	MmZapDB(&MmServingCourseDB,		MmServingCourseDBName, 			NoRecreateDB, &DBDeleteBitMask, MmServingCourseMaskBit);

	MmZapDB(&MmTablenameDB, 			MmTablenameDBName,		 		NoRecreateDB, &DBDeleteBitMask, MmTablenameMaskBit);
	
	MmZapDB(&MmSeatnameDB, 				MmSeatnameDBName, 				NoRecreateDB, &DBDeleteBitMask, MmSeatnameMaskBit);
}
	
	
