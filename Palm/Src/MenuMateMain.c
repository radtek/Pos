/***********************************************************************
 *
 *	Copyright © IQWorks 1998-2004 -- All Rights Reserved
 *
 * PROJECT:			MenuMate Restaurant Menu Application
 *
 * FILE:				MenuMate.c
 *
 *		For best formatting set tab size to 3.
 *
 * DESCRIPTION:	Application developed using C.
 *						
 *						
 *						
 *	  
 *
 * REVISION HISTORY:
 * 	See file MenuMate.h
 *		
 *
 **********************************************************************/

#include "MenuMate.h"


/***********************************************************************
 *
 * FUNCTION:     MmInitialiseAll
 *
 * DESCRIPTION:  This routine sets up the initial state of the application.
 *               It opens the application's database and sets up global variables. 
 *
 * PARAMETERS:   
 *
 * RETURNED:     true if error (database couldn't be created)
 *
 ***********************************************************************/
Boolean MmInitialiseAll(UInt16 initialiseMode)
{
	UInt16						prefSize;
	Int16							prefReturn;

	Char							verNumStr[VersionNumberStringLength + 1] = VersionNumberString;
	Boolean						isNewVersion = false;

	Err							err;

	UInt32						value;
	



	if(initialiseMode & InitLoadSavedPrefs)
	{
		// Read the preferences / saved-state information.  If the preferences
		// were written with a different MmVersionNum they will be ignored.  If
		// MmPreferenceType changes then MmVersionNum should be incremented
		// to avoid using outdated versions of the preferences.
		prefSize = sizeof(Prefs);

		prefReturn = PrefGetAppPreferences(MmAppType, MmPrefID, &Prefs, &prefSize, true);
		if(prefReturn == noPreferenceFound)
		{
			initialiseMode &= ~InitLoadSavedPrefs;	//	Nothing there, so start from scratch
			initialiseMode |= InitResetAllPrefs;
			initialiseMode &= ~InitKeepRegistration;
			initialiseMode &= ~InitKeepBluetoothNames;
			ReDiscoverBt = true;
		}
	}

	if(StrCompare(Prefs.verNumStr, verNumStr))
	{
		isNewVersion = true;
	}


	//	If its a different version, give them the opportunity to bail out
	if(isNewVersion	&&		prefReturn != noPreferenceFound)
	{
		if(FrmCustomAlert(WarnVersionUpdateAlert, (const char *) Prefs.verNumStr,
										(const char *) verNumStr, "") != WarnVersionUpdateProceed)
		{
			return true;
		}
	}




	BtLibRefNum							=	0;
	BtPortId								=	0;


	MmDoInitialiseCode(&initialiseMode, isNewVersion, &BtLibRefNum, &Prefs, &ReDiscoverBt, &RegTemp);



	MmSavePrefs();



	
	NumOldUnsentOrders				=	NoValue16;

	RegTemp.regSuccessful			=	false;
	RegTemp.forceInfoSync			=	false;
	RegTemp.palmListChanged			=	false;
	RegTemp.uniqueID					=	0;
	RegTemp.flags						=	0;
	RegTemp.syncPrintingToID		=	0;
	StrCopy(RegTemp.pIN, "");
	StrCopy(RegTemp.newName, "");
	StrCopy(RegTemp.syncPrintingToName, "");

	AutoTablePageShortSecs			=	NoValue32;
	AutoTablePageLongSecs			=	NoValue32;

	UnsentOrdersCheckSecs			=	1;	//	Force immediate check
		
	SerialPortID						=	NULL;
	SendIndex							=	0;
	InHappyHour							=	false;
	LeftArrowSelected					=	false;
	RightArrowSelected				=	false;

	LastItemOrdered					=	0;
	
	MainviewTopRowNum					=	0;

	OverviewTopRowNum					=	0;
	OverviewExpandedStatus			=	false;
	OverviewPageType					=	OverviewTakeOrders;
	OverviewCheckboxType				=	OverviewCheckboxesNone;
	
//	ServingCourseDefault				=	NoValue16;
	
	RepeatOrderNum						=	0;

	RecentNotesThis					=	0;

	DBDeleteBitMask					=	0;

#ifdef ForceMenus
	GenTableOrders						=	ForceTableNos;	//	Debug Force: Number to generate
	GenSeatOrders						=	ForceSeatNos;
	GenOrdersPerSeat					=	ForceOrderNos;
	GenSeatnames						=	ForceSeatnames;
#endif

	// Get SoundLevel for the sound code.  If sounds are desired use
	// the default sound else set the volume to zero to turn it off.
	SoundLevel = PrefGetPreference(prefAlarmSoundVolume);

	// Check if the new Serial Manager is available, otherwise use the old one.
	err = FtrGet(sysFileCSerialMgr, sysFtrNewSerialPresent, &value);
	NewSerialManagerPresent = (value && !err);

	OldCurrentSeat = Prefs.current.seatNo;

/******************************/


//	if(MmOpenOrCreateDB(MemoDB, MemoDBName))
//		return true;												//	Crash out on error


//	MmCreateMemoRecord(&MemoDB, Prefs.current.memoRec);



/******************************/
	



	if((initialiseMode & InitZapAllDatabases)		||		(initialiseMode & InitZapAllDBsNotInfo))
		MmZapAllDBs(initialiseMode);


	if(MmOpenOrCreateAllDBs(initialiseMode))
		return true;


	if(initialiseMode & InitLoadSavedPrefs)
	{
		if(StrLen(Prefs.palmRegistrationName) == 0	||
			StrCompare(Prefs.palmRegistrationName, "Unregistered") == 0)
		{
			RegTemp.autoReRegister = true;
		}
	}




/*

	MmOpenBluetoothCachedDeviceListDB(&MmBluetoothDevicesCacheDB, MmBtDevicesCacheName);

ShowDebug("Total = %d", BtLibSecurityNumTrustedDeviceRecords((UInt16)MmBluetoothDevicesCacheDB, false));
ShowDebug("Trusted = %d", BtLibSecurityNumTrustedDeviceRecords((UInt16)MmBluetoothDevicesCacheDB, true));

	DmCloseDatabase(MmBluetoothDevicesCacheDB);


	//	Open the library
	err = BtOpenCloseLib(&BtLibRefNum, BtOpenOnly);
	
ShowDebug("Total = %d", BtLibSecurityNumTrustedDeviceRecords(BtLibRefNum, false));
ShowDebug("Trusted = %d", BtLibSecurityNumTrustedDeviceRecords(BtLibRefNum, true));
	
	//	Close the library
	err = BtOpenCloseLib(&BtLibRefNum, BtCloseOnly);

*/


	return false;
}


/***********************************************************************
 *
 * FUNCTION:     StartApplication
 *
 * DESCRIPTION:  This routine sets up the initial state of the application.
 *               It opens the application's database and sets up global variables. 
 *
 * PARAMETERS:   None.
 *
 * RETURNED:     true if error (database couldn't be created)
 *
 ***********************************************************************/
static Boolean MmStartApplication(void)
{
	Boolean		 			recordFound = false;	

//	UInt16 			attr;							// P15. record attributes
//	UInt32 			uniqueID;					// P15. record's unique id


//	HostTraceInit();


#ifdef AllowDebugLogDatabaseButton
DlkSetLogEntry("", 0, false);		//	Clear HotSync Log
#endif

#ifndef ReleaseVersion
DlkSetLogEntry("", 0, false);		//	Clear HotSync Log
#endif



	//	Initialise the Globals, open the DBs etc.
	if(MmInitialiseAll(InitLoadSavedPrefs))
	{
		return true;
	}




//	If no errors in DB creation ...


	
	// P4. Set current view so the first form to display is the main form.
//	CurrentView = MainForm;
	

//DlkSetLogEntry("", 0, false);				//	Temporary fudge to clear HotSync Log
					
	AutoOffTime = SysSetAutoOffTime(0);		//	Get the current value
	SysSetAutoOffTime(AutoOffTime);			//	Set it back to what it was
	


	if(Prefs.userSettings.screenLockOn)
	{
		ScreenLockStruct.action		=	awaitingTimeSecs;	//	Await next AutoScreenLockxxxxTimeSecs timeout
		ScreenLockStruct.timeSecs = TimGetSeconds() + AutoScreenLockLongTimeSecs;
	}
	else
	{
		ScreenLockStruct.action		=	disabled;
		ScreenLockStruct.timeSecs	=	NoValue32;	//	Disable for now
	}
#ifndef ReleaseVersion
	ScreenLockStruct.action		=	disabled;
	ScreenLockStruct.timeSecs	=	NoValue32;	//	Force it OFF during playtime
#endif


	//	Always ensure the Palm turns off again when its in the cradle
	PrefSetPreference(prefStayOnWhenPluggedIn, false);
	PrefSetPreference(prefStayLitWhenPluggedIn, false);


	PreviousForm = OverviewForm;
	FrmGotoForm(TableForm);



	return false;
}


/***********************************************************************
 *
 * FUNCTION:    RomVersionCompatible
 *
 * DESCRIPTION: P4. Check that the ROM version meets your
 *              minimum requirement.  Warn if the app was switched to.
 *
 * PARAMETERS:  requiredVersion - minimum rom version required
 *                                (see sysFtrNumROMVersion in SystemMgr.h 
 *                                for format)
 *              launchFlags     - flags indicating how the application was
 *											 launched.  A warning is displayed only if
 *                                these flags indicate that the app is 
 *											 launched normally.
 *
 * RETURNED:    zero if rom is compatible else an error code
 *                             
 ***********************************************************************/
static Err MmRomVersionCompatible (UInt16 launchFlags)
{
	UInt32			romVersion;
	Err				err;
	
	
	// See if we're on in minimum required version of the ROM or later.
	// The system records the version number in a feature.  A feature is a
	// piece of information which can be looked up by a creator and feature
	// number.
	err = FtrGet(sysFtrCreator, sysFtrNumROMVersion, &romVersion);

	if(romVersion < version35)
	{
		// If the user launched the app from the launcher, explain
		// why the app shouldn't run.  If the app was contacted for something
		// else, like it was asked to find a string by the system find, then
		// don't bother the user with a warning dialog.  These flags tell how
		// the app was launched to decided if a warning should be displayed.
		if((launchFlags & (sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp)) ==
			(sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp))
		{
			FrmAlert(ErrRomIncompatibleAlert);
		
			// Pilot 1.0 will continuously relaunch this app unless we switch to 
			// another safe one.  The sysFileCDefaultApp is considered "safe".
			if(romVersion < version35)
			{
				AppLaunchWithCommand(sysFileCDefaultApp, sysAppLaunchCmdNormalLaunch, NULL);
			}
		}
		
		return(sysErrRomIncompatible);
	}

	return 0;
}


/***********************************************************************
 *
 * FUNCTION:    MmStopApplication
 *
 * DESCRIPTION: P16. This routine closes the application's database
 *              and saves the current state of the application.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
static void MmStopApplication(void)
{
	//	 If the port is already open, close it first
	if(BtPortId != NULL)
	{
		MmCloseBtPort(&BtPortId);
	}

	// Close the application's database.
	DmCloseDatabase(MmOrdersDB);
	DmCloseDatabase(MmInfoDB);
	DmCloseDatabase(MmMiscDataDB);
	DmCloseDatabase(MmDishesDB);
	DmCloseDatabase(MmDishesSizesDB);
	DmCloseDatabase(MmDishesOptionsDB);
	DmCloseDatabase(MmDishesForcedSidesDB);
	DmCloseDatabase(MmDrinksDB);
	DmCloseDatabase(MmDrinksSizesDB);
	DmCloseDatabase(MmDrinksOptionsDB);
	DmCloseDatabase(MmDrinksForcedSidesDB);
	DmCloseDatabase(MmServingCourseDB);
	DmCloseDatabase(MmTablenameDB);
	DmCloseDatabase(MmSeatnameDB);
//	DmCloseDatabase(MemoDB);

	MmSavePrefs();

	// Close all open forms to allow their frmCloseEvent handlers
	// to execute.  An appStopEvent doesn't send frmCloseEvents.
	// FrmCloseAllForms will send all opened forms a frmCloseEvent.

	FrmCloseAllForms();
}


/***********************************************************************
 *
 * FUNCTION:    GetObjectPtr
 *
 * DESCRIPTION: P10. This routine returns a pointer to an object in the active form.
 *
 * PARAMETERS:  objectID - id of the object
 *
 * RETURNED:    pointer to the object's data structure
 *
 ***********************************************************************/
MemPtr MmGetObjectPtr(Int16 objectID)
{
	FormPtr		 frm;
	
	frm = FrmGetActiveForm();
	return(FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, objectID)));
}

/***********************************************************************
 *
 * FUNCTION:    MmCtlShowControl
 *
 * DESCRIPTION: Force show. Overcomes CtlShowControl() 'Bus Error' problem
 *						in emulator
 *
 * PARAMETERS:  pointer to the object's data structure
 *
 * RETURNED:    
 *
 ***********************************************************************/
/*
void MmCtlShowControl(ControlType * controlPtr)
{
	controlPtr->attr.usable = true;
	controlPtr->attr.visible = true;
	CtlShowControl(controlPtr);
}
*/


/***********************************************************************
 *
 * FUNCTION:    GetFocusObjectPtr
 *
 * DESCRIPTION: P4. This routine returns a pointer to the field object, in 
 *              the current form, that has the focus.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    pointer to a field object or NULL if there is no focus
 *
 ***********************************************************************/
/*static FieldPtr MmGetFocusObjectPtr(void)
{
	FormPtr frm;
	Int16 focus;
	
	// Get a pointer to tha active form and the index of the form object with focus.
	frm = FrmGetActiveForm();
	focus = FrmGetFocus(frm);
	
	// If no object has the focus return NULL pointer.
	if (focus == noFocus)
		return(NULL);
		
	// Return a pointer to the object with focus.
	return(FrmGetObjectPtr(frm, focus));
}
*/



/***********************************************************************
 *
 * FUNCTION:    MmLoadList
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
UInt16 MmLoadList(UInt16 listType, UInt16* offset)
{
	UInt16					items;					//	Counter to add stuff to the list

	UInt16					thisTable = 0;
	UInt16					thisSeat = 0;
	DmOpenRef				menuDB;
	DmOpenRef				menuSizesDB;
	DmOpenRef				optionsDB;
	DmOpenRef				forcedSidesDB;
	
	UInt16					btDevice;

	Boolean					nextButton = true;
	UInt16					maxSelect = NoValue16;


	
	if(Prefs.current.mMenu == DishesMenu)
	{
		menuDB = MmDishesDB;
		menuSizesDB = MmDishesSizesDB;
		optionsDB = MmDishesOptionsDB;
		forcedSidesDB = MmDishesForcedSidesDB;
	}
	else
	{
		menuDB = MmDrinksDB;
		menuSizesDB = MmDrinksSizesDB;
		optionsDB = MmDrinksOptionsDB;
		forcedSidesDB = MmDrinksForcedSidesDB;
	}


	//	For CallAwayList we always access the Dishes DB, even if we are on Drinks menu
	if(listType == CallAwayList)
	{
		menuDB = MmDishesDB;
	}
	
	switch(listType)
	{
		case BtDevicesList:
			for(items=0, btDevice = 0;
								items < MaxListItems && btDevice < Prefs.btNumDevices; btDevice++)
			{		
				//	Point to the item's text string in the database
				if(btDevice == Prefs.btDevice				&&
					Prefs.btSelectedDeviceName[0] != 0		)
				{
					ListItems[items++] = (Char *)Prefs.btSelectedDeviceName;
				}
				else
				{
					ListItems[items++] = (Char *)Prefs.btName[btDevice].name;
				}
			}
			break;



		case RecentNotesList:
			for(items = 0; items < MaxRecentNotesListItems; items++)
			{	
				if(StrLen(Prefs.recentNotes[items]) == 0)
					break;
				ListItems[items] = Prefs.recentNotes[items];
			}
			break;

		default:
			items = MmLoadSelectList(&DBs, &Prefs, listType, offset, &nextButton, &maxSelect,
										&ListItems[0], &SizeListItems[0], &ListItemsStruct[0], &SizeListIndices[0]);
	}



	//	If this list has no members display "None Available"
	if(items == 0		&&		listType != CourseForcedOptions)
	{
		switch(listType)
		{
			//	For these list types the Item Index has been stored
			case SizeList:
			case SetMenuSizeList:
			case AllSizeList:
				SizeListItems[0] = "None Available";
				break;

			case CourseOptionsPlus:
			case CourseOptionsMinus:
			case CallAwayList:
			case PalmsList:
				ListItems[0] = "None Available";
				ListItemsStruct[0].recIndex = NoValue16;
				break;
				
			default:
				ListItems[0] = "None Available";
				break;
		}
		items = 1;
	}


	return items;
}



 /***********************************************************************
 *
 * FUNCTION:    MmPopupList
 *
 * DESCRIPTION: Loads the database records into the list by
 *              writing pointers to strings into the list pointer array.
 *						Note: Offset allows the index to start further down the list
 *						Eg. If the Staff come after the Table names then offset steps
 *						over the Tables.
 *
 * PARAMETERS:  List type: Either Available or Selected options
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
UInt16 MmPopupList(ListPtr listPtr, UInt16 listType, UInt16* offset, UInt16 preSelect)
{
	MemHandle				recHandle;
	UInt16					recIndex = 0;
	UInt16					recSeatnameIndex = 0;
	SeatnameTypePtr		rectablenamePtr;
	SeatnameTypePtr		recSeatnamePtr;
	UInt16					numRecords;

	UInt16					items = 0;
	UInt16					itemSelected;
	

	items = MmLoadList(listType, offset);

	//	For Forced Options, no point showing the list if theres no options for this course
	if(listType == CourseForcedOptions && items == 0)
		return 0;
	
	MmLstSetFont(listPtr, &Prefs);

	if((listType == SizeList) || (listType == SetMenuSizeList) || (listType == AllSizeList))
		LstSetListChoices(listPtr, (char**)SizeListItems, items);
	else
		LstSetListChoices(listPtr, ListItems, items);

	//	Display the possible items list
	if(Prefs.current.fFontID == stdFont)
	{
		if(items > DishListSizeStdFont -1)
			items = DishListSizeStdFont -1;		//	Don't let list get too large
	}
	else
	{
		if(items > DishListSizeLargeFont -1)
			items = DishListSizeLargeFont -1;	//	Don't let list get too large
	}
	LstSetHeight(listPtr, items);					//	Set the vertical size of the list

	if(items < 5)
		LstSetPosition(listPtr, 5, 50-items*2);//	Position above centre
	else
		LstSetPosition(listPtr, 5, 20-items*2);//	Position above centre


	LstSetSelection(listPtr, preSelect);		//	Select item


	
	//	Then get the option selected
	itemSelected = LstPopupList(listPtr);


	if(itemSelected != NoValue16)
	{
		switch(listType)
		{
			//	For these list types the Item Index has been stored
			case SizeList:
			case AllSizeList:
			case SetMenuSizeList:
				if(StrCaselessCompare((const Char*)SizeListItems[0], "None Available") == 0)
					itemSelected = NoValue16;
				break;

/*			case StaffList:
				if(itemSelected == 0)		//	Refresh List has same effect as None Available
					itemSelected = NoValue16;
				break;
*/
			case PalmsList:
				break;

			default:
				if(StrCaselessCompare((const Char*)ListItems[0], "None Available") == 0)
					itemSelected = NoValue16;
			
		}

		if(itemSelected != NoValue16)
		{
			switch(listType)
			{
				//	For these list types the Item Index has been stored
				case ItemsList:
				case ItemsListNotSetMenyKey:
				case SetMenuItemsList:

				case CourseOptionsPlus:
				case CourseOptionsMinus:
				case CourseForcedOptions:

				case ActiveTablesList:
				case ActiveSeatsList:

					itemSelected = ListItemsStruct[itemSelected].recIndex;
					break;

				case SeatsList:
					recSeatnameIndex = ListItemsStruct[itemSelected].recIndex;
					recHandle = DmQueryRecord(MmSeatnameDB, recSeatnameIndex);
					if (recHandle)
					{
						recSeatnamePtr = MemHandleLock(recHandle);

						itemSelected = recSeatnamePtr->seatNumber;

						MemPtrUnlock(recSeatnamePtr);
					}
					break;

				case TablesList:
					//	If listType is 'Tables' list then look in DB to find actual Table number
					numRecords = DmNumRecords(MmTablenameDB);
	//				recIndex = *offset + itemSelected;	//	Point to start of Tables + itemSelected
					recIndex = itemSelected+1;

					recHandle = DmQueryRecord(MmTablenameDB, recIndex);
					if(recHandle)
					{
						rectablenamePtr = MemHandleLock(recHandle);

						itemSelected = rectablenamePtr->tableNumber;

						MemPtrUnlock(rectablenamePtr);
					}
					break;
			}
		}
	}

	return itemSelected;
}



/***********************************************************************
 *
 * FUNCTION:    DetailsOptionsLoadList
 *
 * DESCRIPTION: Loads the database records into the Options list by
 *              writing pointers to strings into the list pointer array.
 *
 * PARAMETERS:  List type: Either Available or Selected options
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
static UInt16 MmDetailsOptionsLoadList(/*ListPtr listPtr, */RectangleType bounds, UInt16 listType,
																UInt16 offset, UInt16 currentRecordIndex)
{
	UInt16					recIndex = 0;
	UInt16					recOptionIndex;
	MemHandle				recHandle;
	DmOpenRef				optionsDB;
	DmOpenRef				menuDB;
	OptionsTypePtr			recOptionsPtr;
	OrderTypePtr			recPtr;
	MenuTypePtr				recMenuPtr;

	UInt8*					optionsPtr;
	
	UInt16					optionsInCourse = 0;
	UInt16					item = 0;				//	Counter to step through list items or Options
	UInt16					i = 0;					//	Local Counter

	Int16						listWidth;
	Int16						textLen;
	Boolean					fits = true;
//	Char						tempStr[MaxLinkedItems+1];



	if(Prefs.current.mMenu == DishesMenu)
	{
		optionsDB = MmDishesOptionsDB;
		menuDB = MmDishesDB;
	}
	else
	{
		optionsDB = MmDrinksOptionsDB;
		menuDB = MmDrinksDB;
	}

	// For each item in the list, store the record number in the 
	// list item that will display the record.

	//	Display the Options chosen so far
	if(listType == SelectedOptions)
	{
		// Get the record.
		recHandle = DmQueryRecord(MmOrdersDB, currentRecordIndex);
		
		// If record is found, store the pointer to the Option string in the list array.
		if(recHandle)
		{
			recPtr = MemHandleLock(recHandle);

			if(recPtr->noteLength > 1)						//	Point to start of Options
				optionsPtr = (&recPtr->options + recPtr->noteLength -1);	//	Point to first selected Option
			else
				optionsPtr = &recPtr->options;			//	Point to first selected Option

			do
			{
				if(*optionsPtr == 0x80)						//	Is this just the placeholder?
					break;

				//	Point to the item's text string in the database
				recOptionIndex = MmFindOptionIndex(optionsDB, recPtr->courseNumber, (*optionsPtr) & 0x3F);
				recHandle = DmQueryRecord(optionsDB, recOptionIndex);
				
				// If record is found, store the pointer to the Option string in the list array.
				if(recHandle)
				{
					recOptionsPtr = MemHandleLock(recHandle);

					if(*optionsPtr & OptionsMinusBit)
					{
						//	Point to the item's text string in the database
						ListOptionsItems[item + offset] = (Char*)recOptionsPtr->optionMinusNameFit;
					}
					else
					{
						//	Point to the item's text string in the database
						ListOptionsItems[item + offset] = (Char*)recOptionsPtr->optionNameFit;
					}
					item++;
					optionsInCourse++;

					MemPtrUnlock(recOptionsPtr);
				}
				else
				{
					ErrFatalDisplayIf(1, "Could not open Options record");
				}
			}while(*optionsPtr++ < 0x80);					//	Quit when last Option is done

			MemPtrUnlock(recPtr);
		}
		else
		{
			ErrFatalDisplayIf(1, "Could not open Order record");
		}

		return optionsInCourse + offset;				//	Number in list including previous
	}
	
	
	//	Display the DrinkMix items chosen so far
	else if(listType == LinkedItems)
	{
//		MmLstSetFont(listPtr);
		
		// Get the record.
		recHandle = DmQueryRecord (MmOrdersDB, currentRecordIndex);
		
		// If record is found, store the pointer to the DrinkMix string in the list array.
		if(recHandle)
		{
			recPtr = MemHandleLock(recHandle);

			//	Step through and load all the stored Indices
			for(item=0, i=0; recPtr->menuIndex[i].index != NoValue16 &&
																		i < MaxLinkedItems; i++)
			{
				recHandle = DmQueryRecord(menuDB, recPtr->menuIndex[i].index & IsNotDrinkMask);
				if(recHandle)
				{
					recMenuPtr = MemHandleLock(recHandle);

					//	Show the base item for drinks, clear it for food
					if(Prefs.current.mMenu == DishesMenu && i == 0)
					{
						ListItemsStruct[0].trimName[0] = 0;				//	Null old string
						ListItemsStruct[0].recIndex = NoValue16;
					}
					else
					{
						StrCopy(ListItemsStruct[item].trimName, recMenuPtr->itemName);

						// Determine the length of text that will fit within the bounds of the list.
						textLen = StrLen(ListItemsStruct[item].trimName);	//	Untrimmed length
						listWidth = bounds.extent.x -10;		//	Space for scroll arrows
						
						FntCharsInWidth(ListItemsStruct[item].trimName, &listWidth, &textLen, &fits);
						ListItemsStruct[item].trimName[textLen] = 0;	//	Terminate string to listWidth.

						//	Point to the item's text string in the database
						ListOptionsItems[item + offset] = (Char*)ListItemsStruct[item].trimName;

						//	Save the Index so we can find this record again later.
						ListItemsStruct[item].recIndex = recPtr->menuIndex[item].index;
						item++;
 					}

					MemPtrUnlock(recMenuPtr);
				}
				else
				{
					ErrFatalDisplayIf(1, "Could not open DrinkMix Menu record");
				}

			}
			MemPtrUnlock(recPtr);
		}
		else
		{
			ErrFatalDisplayIf(1, "Could not open Linked Item Order record");
		}

		return item + offset;						//	Number in list including previous
	}
	else
	{
		ErrFatalDisplayIf(1, "Invalid listType in function MmDetailsOptionsLoadList");
		return NoValue16;
	}
}


/***********************************************************************
 *
 * FUNCTION:    DetailsStoreOptionsOrNote
 *
 * DESCRIPTION: Records the Option that has been selected by the user into.
 *						the Orders database
 *
 * PARAMETERS:  Option Selected, if this call is to store Options
 *					 notePtr, the string to be stored or NULL
 *
 * RETURNED:    true if appropriate
 *
 ***********************************************************************/
Boolean MmDetailsStoreOptionsOrNote(UInt16 optionSelected, UInt8* notePtr)
{
	MemHandle 			recHandle;
	OrderTypePtr 		recPtr;

//	MemHandle 			recHandle2;
	UInt16				recIndex2 = 0;
//	OptionsTypePtr		recPtr2;
	
	UInt16				optionFound = 0;
	UInt8					*optionsPtr;
	UInt16				offset = 0;

	UInt8					*destPtr;						//	Temporary pointers for string copy
	UInt8					*sourcePtr;

	UInt16				recLength=1;
	UInt16				i;								//	Counter
	UInt16				noteLength=0;
	UInt16				recordsDeleted;

	recHandle = DmQueryRecord(MmOrdersDB, Prefs.current.recIndex);
	recPtr = MemHandleLock(recHandle);			// Lock down the block containing the Order record.

	//	Copy all fields before the NoteLength to Prefs.curOrder
	for(destPtr = (UInt8 *)&Prefs.curOrder, sourcePtr = (UInt8 *)&recPtr->tableNumber;
					sourcePtr < (UInt8 *)&recPtr->noteLength;  )
		*destPtr++ = *sourcePtr++;						//	Copy all this stuff straight across

	//	Find if a note is being passed or already exists and work out how long it is.
	if(notePtr)											//	If a note has been passed to us ...
	{
		sourcePtr = notePtr;							//	... then use it instead.
		noteLength = StrLen((Char*)notePtr);
		destPtr++;										//	Do the noteLength later
	}
	else if(recPtr->noteLength != 0)				//	Else, if the record already has a note
	{
		noteLength = recPtr->noteLength;
	}
	else
		destPtr++;										//	Do the noteLength later
	
	//	Now copy the NoteLength and Note
	if(recPtr->noteLength || noteLength)		//	If there is a note ...
	{
		for(i=0; i<=noteLength; i++)
			*destPtr++ = *sourcePtr++;				//	Copy the note straight across
		Prefs.curOrder.noteLength = i-1;
	}
	else
		Prefs.curOrder.noteLength = 0;				//	Ensure Note field is blanked

	//	Also copy the Options
	if(recPtr->noteLength > 1)						//	Point to start of Options
		sourcePtr = ((UInt8*)&recPtr->options + recPtr->noteLength -1);
	else
		sourcePtr = ((UInt8*)&recPtr->options);//	No note attached

	destPtr = (UInt8*)&Prefs.curOrder.options;
	do
	{
		*destPtr = *sourcePtr;						//	Copy the Options
		destPtr++;
//		recLength++;
	}while(*sourcePtr++ < 0x80);					//	Until we hit the 'last' one

	MemPtrUnlock(recPtr);

	if(!notePtr)										//	No note => Option change only
	{
		//	Point to the existing Options in the Order
		optionsPtr = (UInt8*)&Prefs.curOrder.options;

		//	Find the last Option and then add the new one
		while(*optionsPtr < 0x80)				//	Find the last Option (MSB = 1)
			optionsPtr++;
		if(*optionsPtr != 0x80)					//	If its not just the marker Option ...
			*optionsPtr++ &= 0x7F;				//	... then strip off the MSB
		*optionsPtr = optionSelected | 0x80;//	Copy in the Options DB index with MSB = 1

	}
	
	if(MmDeleteCurrentRecord(MmOrdersDB, NotMarker, Prefs.current.recIndex, &Prefs.numUnsentOrders,
										&recordsDeleted))//	Kill copy in DB, but not the Marker record
		ErrFatalDisplayIf (true, "Could not Delete Record");
	Prefs.curOrder.timeTaken = TimGetSeconds();	//	Record the time stamp
	MmCreateOrdersRecord(&Prefs, &DBs, &LastItemOrdered);							//	Go make an updated copy of the record
		
	return true;										//	Record NOT updated with Option or Note
}

/***********************************************************************
 *
 * FUNCTION:    DetailsDeleteOption
 *
 * DESCRIPTION: Deletes an Option that has been selected by the user
 *						in the Selected Options list, from the Orders database
 *
 * PARAMETERS:  Option Selected
 *
 * RETURNED:    true if appropriate
 *
 ***********************************************************************/
static Boolean MmDetailsDeleteOption(UInt16 optionSelected)
{
	MemHandle 			recHandle;
	OrderTypePtr 		recPtr;

	UInt16				recIndex2 = 0;
	
	UInt16				optionsCount = 0;
	UInt16				offset = 0;

	UInt8					*destPtr;						//	Temporary pointers for string copy
	UInt8					*sourcePtr;

	UInt16				recLength=1;
	UInt16				i;								//	Counter
	Boolean				optionDeleted = false;
	UInt16				recordsDeleted;

	recHandle = DmQueryRecord(MmOrdersDB, Prefs.current.recIndex);
	recPtr = MemHandleLock(recHandle);			// Lock down the block containing the Order record.

	//	Copy all fields up to and including the noteLength to Prefs.curOrder
	for(destPtr = (UInt8*)&Prefs.curOrder, sourcePtr = (UInt8*)&recPtr->tableNumber;
					sourcePtr <= (UInt8*)&recPtr->noteLength;)
		*destPtr++ = *sourcePtr++;						//	Copy all this stuff straight across

	if(recPtr->noteLength)							//	If there is a note ...
	{
		for(i=0; i<=recPtr->noteLength; i++)
			*destPtr++ = *sourcePtr++;				//	Copy the note straight across
		Prefs.curOrder.noteLength = i-1;
	}
	else
		Prefs.curOrder.noteLength = 0;				//	Ensure Note field is blanked

	//	Also copy the Options
	if(recPtr->noteLength > 1)						//	Point to start of Options
		sourcePtr = ((UInt8*)&recPtr->options + recPtr->noteLength -1);
	else
		sourcePtr = ((UInt8*)&recPtr->options);//	No note attached
		
	destPtr = (UInt8*)&Prefs.curOrder.options;

	//	Copy all Options, but blitz the matched Option if found
	do
	{
		if(((*sourcePtr & 0x7F) == optionSelected) &&
					(optionDeleted == false))		//	Is this our option?
		{
			optionDeleted = true;					//	Only blitz one per delete
			if(*sourcePtr > 0x80)					//	If its the last one
			{
				if(optionsCount)
				{
					destPtr--;
					*destPtr |= 0x80;					//	Set end flag of previous item
				}
				else
				{
					*destPtr = 0x80;					//	There was only one, now there's none
				}
			}
		}
		else
		{
			*destPtr++ = *sourcePtr;				//	Copy the Option
		}
		optionsCount++;
	}while(*sourcePtr++ < 0x80);					//	Quit when last Option is done

	MemPtrUnlock(recPtr);

	if(MmDeleteCurrentRecord(MmOrdersDB, NotMarker, Prefs.current.recIndex, &Prefs.numUnsentOrders,
										&recordsDeleted))//	Kill copy in the DB, but not the Marker record
		ErrFatalDisplayIf (true, "Could not Delete Record");
	Prefs.curOrder.timeTaken = TimGetSeconds();	//	Record the time stamp
	MmCreateOrdersRecord(&Prefs, &DBs, &LastItemOrdered);	//	Go make an updated copy of the record

	return true;										//	Record NOT updated with Option or Note
}

/***********************************************************************
 *
 * FUNCTION:    DetailsEditNote
 *
 * DESCRIPTION: Records the Option that has been selected by the user into.
 *						the Orders database
 *
 * PARAMETERS:  Option Selected
 *
 * RETURNED:    Nil
 *
 ***********************************************************************/
/*static Boolean MmDetailsEditNote(FormPtr frm)
{
	FormPtr 				prvFormPtr;
	MemHandle	 		recHandle;
	OrderTypePtr	 	recPtr;
	MemHandle			textHandle;
	UInt8*				textPtr;
	UInt16				i;
	
	UInt16				fieldIndex;
	FieldPtr				fieldPtr;

	UInt16				buttonHit;

	//	First get MemHandle and MemPtr to the Order record
	recHandle = DmQueryRecord(MmOrdersDB, Prefs.current.record);
	recPtr = MemHandleLock(recHandle);	// Lock down the block containing the Order record.

	// Now Display a dialog asking for their note.
	prvFormPtr = FrmGetActiveForm();
	frm = FrmInitForm(EditLineForm);
	
	textHandle = MemHandleNew(MaxNoteLength + 1);
	textPtr = MemHandleLock(textHandle);
	
	//	Try and get existing note if possible
	if(recPtr->noteLength)
	{
		UInt16			i;
		UInt8*		sourcePtr;
		UInt8*		destPtr;

		sourcePtr = (Char*)&recPtr->note;
		destPtr = textPtr;
		for(i=0; i < recPtr->noteLength; i++)
			*destPtr++ = *sourcePtr++;
		*destPtr = 0;
	}
	else
		StrCopy(textPtr, "");

	// Set the field to edit the note.
	fieldIndex = FrmGetObjectIndex(frm, EditLineNoteField);
	fieldPtr = FrmGetObjectPtr(frm, fieldIndex);
	FldSetTextHandle(fieldPtr, textHandle);
	FldDrawField(fieldPtr);
	FrmSetFocus(frm, fieldIndex);			// Set the insertion point blinking in the note field
	
	//	Allow a longer logoff time in case they are slow at 'typing'
	if(Prefs.userLogOffMins < LogOffMinimumTime && Prefs.userLogOffMins != 0)
		MmRefreshAutoLogout(true, LogOffMinimumTime);	//	Update the Prefs.autoLogoutSecs global

	// Allow the user to type in a note.  Wait until a button is pushed. OK is 
	// the default button so if the app is switched the note is still entered.
	// The user must press cancel to not record the note.
	buttonHit = FrmDoDialog(frm);
//	SysKeyboardDialog(kbdAlpha);			//	Why doesn't this work???
	
	MmRefreshAutoLogout(true, 0);					//	Update the Prefs.autoLogoutSecs global

	// Truncate the string to ensure it's not too long
	textPtr[MaxNoteLength] = '\0';

	//	Save the text if the Okay button was pressed. Otherwise trash it.
	if(buttonHit == EditLineOkayButton)
	{
		for(i=0; textPtr[i] != 0; i++)
		{
			if((textPtr[i] < 0x20) || (textPtr[i] > 0x7A))
			{
				textPtr[i] = ' ';
			}
		}
		MmDetailsStoreOptionsOrNote(0, (UInt8*)textPtr);
	}

//	MemPtrUnlock(recPtr);
	MemPtrUnlock(textPtr);

	FrmDeleteForm(frm);						// Delete the field's new text from memory.
	FrmSetActiveForm(prvFormPtr);

	return true;								//	Record updated with Option
}
*/

/*	for(index = firstRenameableLabel; index <= lastRenameableLabel; index++)
	{
		fld = FrmGetObjectPtr (frm, objNumber++);
		length = StrLen(fieldLabels[index]);
		if (length > 0)
		{
			length += 1;         // include space for a null terminator
			textH = MemHandleNew(length);
         if (textH)
			{
				textP = MemHandleLock(textH);
				MemMove(textP, fieldLabels[index], length);
				FldSetTextHandle (fld, textH);
				MemHandleUnlock(textH);
			}
		}
	}
      
	MemPtrUnlock(appInfoPtr);




   // Get a pointer to the memo field.
   fld = GetObjectPtr (NoteField);
   
   // Set the font used in the memo field.
   FldSetFont (fld, NoteFont);
   
   AddrGetRecord (AddrDB, Prefs.current.record, &record, &recordH);
   
   // CreateNote will have been called before the NoteView was switched
   // to.  It will have insured that a note field exists.
   
   // Find out where the note field is to edit it
   ptr = MemDeref (recordH);
   offset = record.fields[note] - ptr;
   FldSetText (fld, recordH, offset, StrLen(record.fields[note])+1);
   
   MemHandleUnlock(recordH);





   // If the field wasn't modified then don't do anything
   if (FldDirty (fld))
      {      
      // Release any free space in the note field.
      FldCompactText (fld);

      DirtyRecord (Prefs.current.record);
      }
   
	textLength = FldGetTextLength(fld);

   // Clear the handle value in the field, otherwise the handle
   // will be free when the form is disposed of,  this call also unlocks
   // the handle that contains the note string.
   FldSetTextHandle (fld, 0);
   
   // Empty fields are not allowed because they cause problems
   if (textLength == 0)
      DeleteNote();



//=======================================================================
// SetFieldTextFromStr
//
// given a formPtr and fieldID, set the text of the field from the string
// on strP (a copy is made). the active form is used if formP is NULL. works
// even if the field is not editable.
//
// return a pointer to the field.
// 
//=======================================================================

FieldPtr SetFieldTextFromStr( FormPtr formP, UInt16 fieldID, UInt8* strP )
{
	MemHandle		oldTxtH;
	MemHandle		txtH;
	UInt8*		txtP;

	FieldPtr		fldP;
	Boolean		fieldWasEditable;
	

	// get some space in which to stash the string.
	txtH	= MemHandleNew(  StrLen( strP ) + 1 );
	txtP	= MemHandleLock( txtH );

	// copy the string.
	StrCopy( txtP, strP );

	// unlock the string handle.
	MemHandleUnlock( txtH );
	txtP = 0;

	// get the field and the field's current text handle.
	fldP		= GetObjectPtr( formP, fieldID );	
	oldTxtH	= FldGetTextHandle( fldP );
	
	// set the field's text to the new text.
	fieldWasEditable		= fldP->attr.editable;
	fldP->attr.editable	= true;
	FldSetTextHandle( fldP, txtH );
	FldDrawField( fldP );
	fldP->attr.editable	= fieldWasEditable;

	// free the handle AFTER we call FldSetTextHandle().
	if ( oldTxtH != NULL ) MemHandleFree( oldTxtH );
	
	return fldP;

}	// SetFieldTextFromStr
*/



/***********************************************************************
 *
 * FUNCTION:    MmDetailsReDrawOptionsList
 *
 * DESCRIPTION: ReDraw Options/DrinkMix List of selected items.
 *
 * PARAMETERS:  ListPtr listPtr
 *
 * RETURNED:    Number of Food or Drink items (not Options).
 *
 ***********************************************************************/
UInt16 MmDetailsReDrawOptionsList(FormPtr frm, ListPtr listPtr, UInt16 currentRecordIndex)
{
	UInt16 					optionsInCourse = 0;
	UInt16					drinkItemsInList = 0;
	UInt16					i;								//	Local Counter
	ControlType				*buttonPtr;
	RectangleType			bounds;

	buttonPtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, DetailsLinkedItemsButton));
	
	FrmGetObjectBounds(frm, FrmGetObjectIndex(frm, DetailsOptionsSelectedList), &bounds);

	MmCourseOptionsTrimList(&DBs, &Prefs, bounds);		//	Trim Options to fit list width
	//	Call MmDetailsOptionsLoadList twice. First for the Linked Items, then for the Options
	drinkItemsInList = MmDetailsOptionsLoadList(/*listPtr, */bounds, LinkedItems,
																						0, currentRecordIndex);
	optionsInCourse = MmDetailsOptionsLoadList(/*listPtr, */bounds, SelectedOptions,
																	drinkItemsInList, currentRecordIndex);
	//	Display the list and then get the selection
	if(Prefs.current.fFontID == stdFont)
		LstSetHeight(listPtr, 6);						//	Fill the entire list area
	else
		LstSetHeight(listPtr, 5);						//	Fill the entire list area

	LstSetPosition(listPtr, 46, 53);					//	Position list above centre
	LstSetSelection(listPtr, NoValue16);	//	Ensure no items are selected

	LstSetListChoices(listPtr, (Char**)ListOptionsItems, optionsInCourse);

	LstDrawList(listPtr);

	if(Prefs.current.mMenu == DishesMenu)
	{
		CtlSetLabel(buttonPtr, "Sides");
	}
	else
			//	Count the number of Nips of the base Drink so we can set the Nips display
	{
		i = 1;
		while(i < MaxLinkedItems)
		{
			if(Prefs.curOrder.menuIndex[i].index != Prefs.curOrder.menuIndex[0].index)
				break;
			else
				i++;
		}

		FrmSetControlGroupSelection (frm, DetailsGroupID, DetailsNipsLabel + i);

		if(Prefs.curOrder.menuIndex[1].index == NoValue16)
		{
			CtlSetLabel(buttonPtr, "Mix");

			FrmHideObject(frm, FrmGetObjectIndex(frm, DetailsNipsLabel));
			FrmHideObject(frm, FrmGetObjectIndex(frm, DetailsNips1PushButton));
			FrmHideObject(frm, FrmGetObjectIndex(frm, DetailsNips2PushButton));
			FrmHideObject(frm, FrmGetObjectIndex(frm, DetailsNips3PushButton));
			FrmHideObject(frm, FrmGetObjectIndex(frm, DetailsNips4PushButton));
		}
		else
		{
			CtlSetLabel(buttonPtr, "Drinks");

			FrmShowObject(frm, FrmGetObjectIndex(frm, DetailsNipsLabel));
			FrmShowObject(frm, FrmGetObjectIndex(frm, DetailsNips1PushButton));
			FrmShowObject(frm, FrmGetObjectIndex(frm, DetailsNips2PushButton));
			FrmShowObject(frm, FrmGetObjectIndex(frm, DetailsNips3PushButton));
			FrmShowObject(frm, FrmGetObjectIndex(frm, DetailsNips4PushButton));
		}
	}
	return drinkItemsInList;
}



/***********************************************************************
 *
 * FUNCTION:    DetailsFormHandleEvent
 *
 * DESCRIPTION: P10. Handles processing of events for the ÒdetailsÓ form.
 *
 * PARAMETERS:  event - the most recent event.
 *
 * RETURNED:    True if the event is handled, false otherwise.
 *
 ***********************************************************************/
static Boolean MmDetailsFormHandleEvent(EventPtr event)
{
	FormPtr						frm;
	Boolean						handled = false;
	UInt32						timeNow;

	Int16 						updateCode = frmRedrawUpdateCode;

	ControlType					*buttonPtr;
	
	ListPtr						listPtr;							//	Selected Options list
	ListPtr						listOptsPtr;					//	Options to select from list
	ListPtr						listSizePtr;
	ListPtr						listMenuPtr;					//	Coursees/Drinks list
	ListPtr						listServePtr;
	UInt16						itemSelected;
	UInt16						optionSelected;
	UInt16						recOptionIndex;
	UInt16						recMenuIndex;
//	UInt16						recServeIndex;
	UInt8*						optionStrPtr;
	UInt16						offset;
	UInt16						itemCount = 0;
	
	UInt16						currentCourse;				//	Preserve Prefs.current.course
	UInt16						currentMenuIndex;
	
	MemHandle 					recHandle;
	UInt16						recIndex;
	OptionsTypePtr				recOptionsPtr;
	OrderTypePtr				recPtr;
	MenuTypePtr					recMenuPtr;
	SizeTypePtr					recSizePtr;
//	Err							err;
	UInt16						numRecords;
	Int16							i, j, k;							//	Local Counters
	Int16							nips;
	
	Boolean						detailsDisabled = false;
//	Boolean						containsSetMenuKey;
	
//	UInt16						status;
//	Int16							priceAdj;
	UInt16						recordsDeleted;
//	UInt16						numSizeTypes;
	UInt16						numLinkedItems = 0;
	
	RectangleType				bounds;

	
	DmOpenRef					menuDB;
	DmOpenRef					menuSizesDB;
	DmOpenRef					optionsDB;

	

	frm = FrmGetActiveForm();

	timeNow = MmProcessEventIncludingNilEvent(frm, event, 0, 0);



	if(Prefs.current.mMenu == DishesMenu)
	{
		menuDB = MmDishesDB;
		menuSizesDB = MmDishesSizesDB;
		optionsDB = MmDishesOptionsDB;
	}
	else
	{
		menuDB = MmDrinksDB;
		menuSizesDB = MmDrinksSizesDB;
		optionsDB = MmDrinksOptionsDB;
	}


	frm = FrmGetActiveForm();




	if(event->eType == ctlSelectEvent ||
		event->eType == lstSelectEvent	)
	{
		//	Test once to see if editing allowed and save in detailsDisabled,
		//	rather than testing for each keypress type below
		recHandle = DmQueryRecord(MmOrdersDB, Prefs.current.recIndex);
		if(recHandle)
		{
			recPtr = MemHandleLock(recHandle);
			if((recPtr->status & StatusAsSent) ||
				(recPtr->status & StatusAsCreditDish)			)
			{
				detailsDisabled = true;
			}

			MemPtrUnlock(recPtr);
		}
	}
	


	if(event->eType == ctlSelectEvent)   	// A control button was pressed and released.
	{
		//	Do Okay and Delete buttons separately. Need to MmDisplayPopNameLabel() all others
		if(event->data.ctlSelect.controlID == DetailsOkayButton)
		{
   		// The ok button was pressed so apply the changes.

			//	Update the size and price for the order
			MmCopyCurrentOrderIndicesToRecord(MmOrdersDB, &Prefs);

			// Remove the details form and display the Previous form.
			if(Prefs.userSettings.classicMode	||	PreviousForm == MainForm)
				FrmGotoForm(MainForm);
			else
				FrmGotoForm(OverviewForm);
			
			handled = true;
		}



		//	Copy Button
		else if(event->data.ctlSelect.controlID == DetailsCopyButton)
		{
			Keypad.mode = KeypadCopyOrder;
			Keypad.value = 0;
			Keypad.maxValue = 99;
			StrNCopy(Keypad.frmTitleLabel, "New Copies", 15);
			StrNCopy(Keypad.cancelLabel, "Cancel", 15);

			FrmPopupForm(KeypadForm);

			handled = true;
		}



		//	Delete Button
		else if(event->data.ctlSelect.controlID == DetailsDeleteButton)
		{
   		// The delete button was pressed.  Delete the current record.
/*				
			if(detailsDisabled)
			{
				//	Record has been sent => Deletion is to give credit
				if(FrmAlert(ConfCreditThisDishAlert) == ConfCreditThisDishOK)
				{
					recHandle = DmGetRecord(MmOrdersDB, Prefs.current.record);
					if(recHandle)
					{
						recPtr = MemHandleLock(recHandle);

						//	'Unsend' the order so it gets sent again
						status = recPtr->status & StatusAsUnsentUnconfirmed;
						
						//	Set the conditions for Credit: PriceAdj bit set, PriceAdj = 0
						status |= StatusAsCreditDish;
						err = DmWrite(recPtr, (Char*)&recPtr->status - (Char*)&recPtr->tableNumber,
												&status, sizeof(recPtr->status));
						ErrFatalDisplayIf(err, "Could not write 'StatusAsCreditDish' to record.");

						priceAdj = 0;
						err = DmWrite(recPtr,
												(UInt8*)&recPtr->priceAdj - (UInt8*)&recPtr->tableNumber,
												&priceAdj, sizeof(priceAdj));
						ErrFatalDisplayIf(err, "Could not write 'priceAdj' to record.");

						//	Flag Prefs.hLStatus as 'To send'
						Prefs.hLStatus.HLSendOrdersStatus = HLStatusToSend;

						MemPtrUnlock(recPtr);
						DmReleaseRecord(MmOrdersDB, Prefs.current.record, true);
					}
				}
			}
*/
			if(detailsDisabled)
			{
				FrmAlert(WarnCantAlterSentOrderAlert);
			}
			else
			{
				if(FrmAlert(ConfDeleteThisDishAlert) == ConfDeleteThisDishOK)
				{
					if(MmDeleteCurrentRecord(MmOrdersDB, WithMarker, Prefs.current.recIndex,
														&Prefs.numUnsentOrders, &recordsDeleted))
						ErrFatalDisplayIf (true, "Could not Delete Record");
				}
			}



			if(Prefs.userSettings.classicMode	||	PreviousForm == MainForm)
				FrmGotoForm(MainForm);
			else
				FrmGotoForm(OverviewForm);

			handled = true;
		}
		else
		{
			//	Do all these controls together. Need to do MmDisplayPopNameLabel()
			switch (event->data.ctlSelect.controlID)
			{
				case DetailsAttachNoteButton:
		   		// The note button was pressed, so go to edit mode.
					if(detailsDisabled)
					{
						MmShowNoteInButton(frm, DetailsAttachNoteButton, Prefs.curOrder.note, &Prefs);
						FrmAlert(WarnCantAlterSentOrderAlert);
					}
					else
					{
						MmShowNoteInButton(frm, DetailsAttachNoteButton, Prefs.curOrder.note, &Prefs);
						FrmPopupForm(EditLineForm);
					}

					handled = true;
					break;
					
					
					
					
					
				case DetailsServeButton:
		   		// The Serve button was pressed, so go to select mode.
					if(detailsDisabled)
					{
						FrmAlert(WarnCantAlterSentOrderAlert);
					}
					else
					{
						listServePtr = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, DetailsOptionsList));

						//	Get the ServingCourse selected
						offset = 0;

						//	Redraw to prevent corruption of button label while the list is displayed
						MmShowServingCourse(frm, DetailsServeButton, &DBs, NoValue16, &Prefs);

						itemSelected = MmPopupList(listServePtr, ServingCourseAllList, &offset, 0);
						if(itemSelected != NoValue16)
						{						
							itemSelected = ListItemsStruct[itemSelected].recIndex;
							
//							MmGetServingCourseIndex(MmServingCourseDB, NoValue16,//Prefs.current.dishesServingCourseDefault,
//													&itemSelected, &servingCourseColor, &selectable);

							//	Redraw with the new item
							MmShowServingCourse(frm, DetailsServeButton, &DBs, itemSelected, &Prefs);
						}
					}

					handled = true;
					break;





					
				case DetailsPriceAdjButton:
		   		// The Price Adjust button was pressed so allow the changes.
					if(	  recPtr->setMenuMask	&&		//	Cannot price adjust a slave SetMenu item
							!(recPtr->setMenuMask & SetMenuKey))
					{
						FrmAlert(WarnCantPriceAdjustSetMenuAlert);
					}
					else if(detailsDisabled)
					{
						FrmAlert(WarnCantAlterSentOrderAlert);
					}
					else if(Prefs.userSettings.priceAdjust == false)
					{
						FrmAlert(WarnNoPriceAdjustRightsAlert);
					}
					else
					{
						Keypad.mode = KeypadPriceAdjust;
						Keypad.value = Prefs.curOrder.price;
						Keypad.maxValue = NoValue32Signed;
						StrNCopy(Keypad.frmTitleLabel, "Enter Price", 15);
						StrNCopy(Keypad.cancelLabel, "Standard Price", 15);

						FrmPopupForm(KeypadForm);
					}

					handled = true;
					break;

				case DetailsNips1PushButton:
				case DetailsNips2PushButton:
				case DetailsNips3PushButton:
				case DetailsNips4PushButton:

					if(detailsDisabled)
					{
						FrmAlert(WarnCantAlterSentOrderAlert);
					}
					else
					{
						//	We assume that the Button IDs have been set up in order above DetailsNipsLabel
						nips = event->data.ctlEnter.controlID - DetailsNipsLabel;
						
						// First count how many nips there are currently
						currentCourse = Prefs.curOrder.menuIndex[0].index;

						i=1;
						while(i < MaxLinkedItems)
						{
							if(Prefs.curOrder.menuIndex[i].index != Prefs.curOrder.menuIndex[0].index)
								break;
							else
								i++;
						}

						//	The current number of nips can be greater than, less than, or equal to requested
						//	They want more Nips ...
						if(nips > i)
						{
							//	Count the number of DrinkMixes already selected and warn if too many
							for(k=0; k < MaxLinkedItems; k++)
							{
								if(Prefs.curOrder.menuIndex[k].index == NoValue16)
									break;
							}
							
							if(k-i+nips <= MaxLinkedItems)
							{
								for(j=4; j > 0; j--)				//	Shove them all up by the difference
								{
									if(j+i-nips > 0)
									{
										Prefs.curOrder.menuIndex[j].index =
																			Prefs.curOrder.menuIndex[j+i-nips].index;
										Prefs.curOrder.menuIndex[j].size =
																			Prefs.curOrder.menuIndex[j+i-nips].size;
									}
									else
									{
										Prefs.curOrder.menuIndex[j].index = Prefs.curOrder.menuIndex[0].index;
										Prefs.curOrder.menuIndex[j].size = Prefs.curOrder.menuIndex[0].size;
									}
								}
							}
							else
							{
								FrmAlert(WarnMaxDrinkMixItemsAlert);
							}
						}
						else if(nips < i)						//	They want fewer
						{
							for(j=0; j < MaxLinkedItems; j++)	//	Shove them all down by the difference
							{
								if(j+i-nips < MaxLinkedItems)
								{
									Prefs.curOrder.menuIndex[j].index = Prefs.curOrder.menuIndex[j+i-nips].index;
									Prefs.curOrder.menuIndex[j].size = Prefs.curOrder.menuIndex[j+i-nips].size;
								}
								else
								{
									Prefs.curOrder.menuIndex[j].index = NoValue16;
									Prefs.curOrder.menuIndex[j].size = (UInt8)(NoValue16);
								}
							}
						}
						else
						{
						}

						MmCopyCurrentOrderIndicesToRecord(MmOrdersDB, &Prefs);
						if(Prefs.curOrder.menuIndex[1].index == NoValue16)
						{
							Prefs.curOrder.size = Prefs.curOrder.menuIndex[0].size;
//							MmDisplayPopNameLabel(frm, menuSizesDB, Prefs.curOrder.size,
//																				DetailsSizePopTrigger, true);
						}

//						MmGetPrice(Prefs.current.mMenu);
//						MmShowPrice(frm, DetailsPriceLabel, 0);

						//	Set up list pointers for the selected DrinkMix/Options list and
						//	the list of Coursees/Drinks to select from
						listPtr = FrmGetObjectPtr(frm, 
												FrmGetObjectIndex(frm, DetailsOptionsSelectedList));

						MmLstSetFont(listPtr, &Prefs);

						FrmShowObject(frm, FrmGetObjectIndex(frm, DetailsOptionsSelectedList));
						MmDetailsReDrawOptionsList(frm, listPtr, Prefs.current.recIndex);
					}
					handled = true;
					break;


				case DetailsLinkedItemsButton:
		   		// The Drinks button was pressed so show the list of courses.
					if(detailsDisabled)		//	Cant mess with sent orders
					{
						FrmAlert(WarnCantAlterSentOrderAlert);
					}
					else
					{
						buttonPtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, DetailsLinkedItemsButton));
		
						//	If its a drink, first tap of button just shows Nips & changes button name
						if(StrCaselessCompare((const char *)CtlGetLabel(buttonPtr), "Mix") == 0)
						{
							CtlSetLabel(buttonPtr, "Drinks");

							FrmShowObject(frm, FrmGetObjectIndex(frm, DetailsNipsLabel));
							FrmShowObject(frm, FrmGetObjectIndex(frm, DetailsNips1PushButton));
							FrmShowObject(frm, FrmGetObjectIndex(frm, DetailsNips2PushButton));
							FrmShowObject(frm, FrmGetObjectIndex(frm, DetailsNips3PushButton));
							FrmShowObject(frm, FrmGetObjectIndex(frm, DetailsNips4PushButton));
						}
						else
						{
							//	They want to add a linked item. Show a list
							//	Count the number of DrinkMixes already selected and warn if too many
							for(i=0; i < MaxLinkedItems; i++)
							{
								if(Prefs.curOrder.menuIndex[i].index == NoValue16)
									break;
							}
							
							if(i >= MaxLinkedItems)
							{
								FrmAlert(WarnMaxDrinkMixItemsAlert);
							}
							else
							{
								currentCourse = Prefs.current.course;				//	Preserve
								currentMenuIndex = Prefs.current.recMenuIndex;	//	Preserve
								
								//	Set up list pointers for the selected DrinkMix/Options list and
								//	the list of Courses/Drinks to select from
								listPtr = FrmGetObjectPtr(frm, 
														FrmGetObjectIndex(frm, DetailsOptionsSelectedList));

								listMenuPtr = FrmGetObjectPtr(frm,
														FrmGetObjectIndex(frm, DetailsOptionsList));

								listSizePtr = FrmGetObjectPtr(frm,
														FrmGetObjectIndex(frm, DetailsOptionsList));

								MmLstSetFont(listPtr, &Prefs);
								MmLstSetFont(listMenuPtr, &Prefs);
								MmLstSetFont(listSizePtr, &Prefs);

								//	Get the Course selected for Side
								offset = 0;

								//	Get the Course selected
								itemSelected = MmPopupList(listMenuPtr, CoursesList, &offset, 0);

								if(itemSelected != NoValue16)
								{
//recMenuHandle = DmQueryRecord (menuDB, listItemsStruct[itemSelected].recIndex);

									Prefs.current.course = itemSelected + 1;	//	itemSelected is zero based
									
									//	Get the Item selected for Side
									recMenuIndex = MmPopupList(listMenuPtr, ItemsListNotSetMenyKey, &offset, 0);

									if(recMenuIndex != NoValue16)
									{
										recHandle = DmQueryRecord(menuDB, recMenuIndex);
										
										// If record is found, add this Side or Mix item to this order.
										if(recHandle)
										{
											recMenuPtr = MemHandleLock(recHandle);
											
											if(!(recMenuPtr->enabled & StatusAsEnabled))
											{
												FrmAlert(WarnMenuDishDisabledAlert);
											}

											//	Good Side selected. Now ask for any available Size, or use default
											else
											{
												UInt16	orderSizeIndex;
												
												Prefs.current.recMenuIndex = recMenuIndex;
//												Prefs.curOrder.menuIndex[0].index = recMenuIndex;

												//	Ask for the size if applicable
												if(MmAskUserForSize(&DBs, &Prefs, SizeForNewSide, recMenuPtr, 
																			listSizePtr, &SizeListIndices[0], &orderSizeIndex))
												{
													MmAddSideToRecord(&DBs, &Prefs, orderSizeIndex);
												}
											}
											MemPtrUnlock(recMenuPtr);
										}
									}
								}
								
								Prefs.current.course = currentCourse;				//	Restore
								Prefs.current.recMenuIndex = currentMenuIndex;	//	Restore
//									MmGetPrice(Prefs.current.mMenu);
//									MmShowPrice(frm, DetailsPriceLabel, 0);
								FrmShowObject(frm, FrmGetObjectIndex(frm, DetailsOptionsSelectedList));
								MmDetailsReDrawOptionsList(frm, listPtr, Prefs.current.recIndex);
							}
						}
//							MmDisplayPopNameLabel(frm, menuSizesDB, Prefs.curOrder.size,
//																			DetailsSizePopTrigger, true);
					}
					handled = true;
					break;


				case DetailsSizePopTrigger:
					//	The Dish Options pick list was selected
					if(detailsDisabled)
					{
						FrmAlert(WarnCantAlterSentOrderAlert);
					}
					else
					{
						//	Repaint name to avoid corruption while pick list enabled
//						MmDisplayPopNameLabel(frm, menuSizesDB, Prefs.curOrder.size,
//																				DetailsSizePopTrigger, true);

						listSizePtr = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, DetailsOptionsList));

						//	Get the Size selected
						offset = 0;

						//	Show only the sizes for the base dish if its a food item
						//	but show all size types if more than one drink item, because its a DrinkMix
						if(Prefs.curOrder.setMenuMask)
						{
							itemSelected = MmPopupList(listSizePtr, SetMenuSizeList, &offset, 0);
						}
						else if(	Prefs.current.mMenu == DishesMenu	||
									Prefs.curOrder.menuIndex[1].index == NoValue16)
						{
							itemSelected = MmPopupList(listSizePtr, SizeList, &offset, 0);
						}
						else
						{
							itemSelected = MmPopupList(listSizePtr, AllSizeList, &offset, 0);
						}
						
						if(itemSelected != NoValue16)
						{
							recHandle = DmQueryRecord(menuSizesDB, SizeListIndices[itemSelected]);

							// If record is found, store the pointer to the item string in the list array.
							if(recHandle)
							{
								recSizePtr = MemHandleLock(recHandle);

								//	Get the size type selected
								Prefs.curOrder.size = recSizePtr->size;

								//	If its not (or no longer) a DrinkMix, make size type change price
								if(Prefs.current.mMenu == DishesMenu	||
									Prefs.curOrder.menuIndex[1].index == NoValue16)
								{
									Prefs.curOrder.menuIndex[0].size = recSizePtr->size;
								}
								MemPtrUnlock(recSizePtr);
							}
		
//							MmDisplayPopNameLabel(frm, menuSizesDB, SizeListIndices[itemSelected],
//																					DetailsSizePopTrigger, true);
//							MmGetPrice(Prefs.current.mMenu);

							//	Update the size and price for the order
							MmCopyCurrentOrderIndicesToRecord(MmOrdersDB, &Prefs);

//							MmShowPrice(frm, DetailsPriceLabel, 0);
						}
					}

					handled = true;
					break;


				case DetailsOptionsPlusButton:
				case DetailsOptionsMinusButton:
					//	The Dish Options pick list was selected
					if(detailsDisabled)
					{
						FrmAlert(WarnCantAlterSentOrderAlert);
					}
					else
					{
						listPtr = FrmGetObjectPtr(frm, 
												FrmGetObjectIndex(frm, DetailsOptionsSelectedList));
						listOptsPtr = FrmGetObjectPtr(frm,
												FrmGetObjectIndex(frm, DetailsOptionsList));

						MmLstSetFont(listPtr, &Prefs);
						MmLstSetFont(listOptsPtr, &Prefs);
						FrmGetObjectBounds(frm, FrmGetObjectIndex(frm, DetailsOptionsList), &bounds);

						//	Load the list pointer array with the string pointers

						MmCourseOptionsTrimList(&DBs, &Prefs, bounds);//	Trim Options to fit list width
						offset = 0;
						
						if(event->data.ctlSelect.controlID == DetailsOptionsPlusButton)
						{
							recIndex = MmPopupList(listOptsPtr, CourseOptionsPlus, &offset, 0);
						}
						else if(event->data.ctlSelect.controlID == DetailsOptionsMinusButton)
						{
							recIndex = MmPopupList(listOptsPtr, CourseOptionsMinus, &offset, 0);
						}
						
						if(StrCaselessCompare((const char *)ListItems[0], "None Available") == 0)
							recIndex = NoValue16;
						
						if(recIndex != NoValue16)
						{
							recHandle = DmQueryRecord(optionsDB, recIndex);

							// If record is found, store the pointer to the item string in the list array.
							if(recHandle)
							{
								recOptionsPtr = MemHandleLock(recHandle);

								if(event->data.ctlSelect.controlID == DetailsOptionsPlusButton)
								{
									MmDetailsStoreOptionsOrNote(recOptionsPtr->courseOptionNumber, 0);
								}
								else if(event->data.ctlSelect.controlID == DetailsOptionsMinusButton)
								{
									MmDetailsStoreOptionsOrNote(
												recOptionsPtr->courseOptionNumber | OptionsMinusBit, 0);
								}
								
								MemPtrUnlock(recOptionsPtr);
							}

							FrmShowObject(frm, FrmGetObjectIndex(frm, DetailsOptionsSelectedList));
							MmDetailsReDrawOptionsList(frm, listPtr, Prefs.current.recIndex);
						}
					}

					handled = true;
					break;
			}
			MmDisplayPopNameLabel(frm, &DBs, menuSizesDB,
										Prefs.curOrder.size, DetailsSizePopTrigger, true);
		}
	}

	else if (event->eType == lstSelectEvent)
	{
		if(detailsDisabled)
		{
			FrmAlert(WarnCantAlterSentOrderAlert);
		}
		else
		{
			listPtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, DetailsOptionsSelectedList));
			MmLstSetFont(listPtr, &Prefs);

			FrmShowObject(frm, FrmGetObjectIndex(frm, DetailsOptionsSelectedList));
			numLinkedItems = MmDetailsReDrawOptionsList(frm, listPtr, Prefs.current.recIndex);

			//	Get the string: It could be a Linked Item, or an Option
			optionStrPtr = (unsigned char *)ListOptionsItems[event->data.lstSelect.selection];
			


			//	Check if its a Linked Item or an Option
			if(event->data.lstSelect.selection < numLinkedItems)
			{
/*				recHandle = DmQueryRecord(menuSizesDB, SizeListIndices[event->data.lstSelect.selection]);

				// If record is found, store the pointer to the item string in the list array.
				if(recHandle)
				{
					recSizePtr = MemHandleLock(recHandle);

					//	Get the size type selected
					Prefs.curOrder.size = recSizePtr->size;

					//	If its not (or no longer) a DrinkMix, make size type change price
					if(Prefs.current.mMenu == DishesMenu	||
						Prefs.curOrder.menuIndex[1].index == NoValue16)
					{
						Prefs.curOrder.menuIndex[0].size = recSizePtr->size;
					}
					MemPtrUnlock(recSizePtr);
				}
*/
				if(FrmCustomAlert(ConfDeleteSideOrOptionAlert, "Side?         ", "", (const char *) optionStrPtr) == ConfDeleteSideOrOptionDelete)
				{
					//	Must be a Linked item (because Options come second)
					//	Never blitz last one for Drinks!
					if(Prefs.current.mMenu == DishesMenu	||
						event->data.lstSelect.selection != 0)
					{
						j = event->data.lstSelect.selection;
						
						if(Prefs.current.mMenu == DishesMenu)				//	Dishes list is offset up by 1
							j++;

						for(; j < MaxLinkedItems; j++)			//	Shove them all up by one
						{
							if(j+1 < MaxLinkedItems)
							{
								Prefs.curOrder.menuIndex[j].index = Prefs.curOrder.menuIndex[j+1].index;
								Prefs.curOrder.menuIndex[j].size = Prefs.curOrder.menuIndex[j+1].size;
							}
							else
							{
								Prefs.curOrder.menuIndex[j].index = NoValue16;
								Prefs.curOrder.menuIndex[j].size = (UInt8)(NoValue16);
							}
						}
					}
					MmCopyCurrentOrderIndicesToRecord(MmOrdersDB, &Prefs);
				}
			}
			else	//	Must be an Option
			{
				if(FrmCustomAlert(ConfDeleteSideOrOptionAlert, "Option?       ", "", (const char *) optionStrPtr) == ConfDeleteSideOrOptionDelete)
				{
					numRecords = DmNumRecords(optionsDB);

					//	Find the first Option in the Course
					recOptionIndex = MmFindCourseOptionIndex(optionsDB,
																				Prefs.curOrder.courseNumber, 1);

					//	Find this Option string in the Options DB
					for(; recOptionIndex <= numRecords; recOptionIndex++)
					{
						recHandle = DmQueryRecord(optionsDB, recOptionIndex);
						// If record is found, compare with Option selected.
						if(recHandle)
						{
							recOptionsPtr = MemHandleLock(recHandle);

							if(StrCaselessCompare((const char *)optionStrPtr, recOptionsPtr->optionNameFit) == 0)
							{
								optionSelected = recOptionsPtr->courseOptionNumber;

								MemPtrUnlock(recOptionsPtr);

								//	We know which Option index so now find and remove it from the order
								MmDetailsDeleteOption(optionSelected);

								break;
							}
							else if(StrCaselessCompare((const char *)optionStrPtr, recOptionsPtr->optionMinusNameFit) == 0)
							{
								optionSelected = recOptionsPtr->courseOptionNumber | OptionsMinusBit;

								MemPtrUnlock(recOptionsPtr);

								//	We know which Option index so now find and remove it from the order
								MmDetailsDeleteOption(optionSelected);

								break;
							}
							else if(recOptionsPtr->courseNumber > Prefs.curOrder.courseNumber)
							{
								MemPtrUnlock(recOptionsPtr);
								break;
							}
							else
							{
								MemPtrUnlock(recOptionsPtr);
							}
						}
					}
				}
			}
			
			
			//	Force the size to base type and make the Size select PopLabel show that
			if(Prefs.curOrder.menuIndex[1].index == NoValue16)
			{
				Prefs.curOrder.size = Prefs.curOrder.menuIndex[0].size;
//				MmDisplayPopNameLabel(frm, menuSizesDB, Prefs.curOrder.size,
//																		 DetailsSizePopTrigger, false);
			}

//			MmGetPrice(Prefs.current.mMenu);
//			MmShowPrice(frm, DetailsPriceLabel, 0);

			FrmShowObject(frm, FrmGetObjectIndex(frm, DetailsOptionsSelectedList));
			MmDetailsReDrawOptionsList(frm, listPtr, Prefs.current.recIndex);
		}

		handled = true;
	}
	



	else if(event->eType == frmOpenEvent)
	{
		FrmDrawForm(frm);

		MmDetailsFormRedraw(frm, &DBs, &Prefs);

		handled = true;
	}



	//	Refresh all the form's buttons to avoid corruption
	if(event->eType == keyDownEvent)
	{
		if(IsFiveWayNavEvent(event))
		{
			MmDetailsFormRedraw(frm, &DBs, &Prefs);
		}
	}










#ifdef CheckSizeIndices
	recHandle = DmQueryRecord(MmOrdersDB, Prefs.current.record);
	if(recHandle)
	{
		recPtr = MemHandleLock(recHandle);
		i=0;
		
		while(i < MaxLinkedItems)
		{
			if(recPtr->menuType != DishesMenu)
			{
				//	If an index has been assigned, there had better also be a size
				if(Prefs.curOrder.menuIndex[i].index != NoValue16)
				{
					if(Prefs.curOrder.menuIndex[i].size == (UInt8)(NoValue16))
					{
						FrmCustomAlert(InfSizeIndexLostAlert, "1", "", "");
					}
					if(Prefs.curOrder.menuIndex[i].size == 0)
					{
						FrmCustomAlert(InfSizeIndexLostAlert, "2", "", "");
					}
				}
				if(recPtr->menuIndex[i].index != NoValue16)
				{
					if(recPtr->menuIndex[i].size == (UInt8)(NoValue16))
					{
						FrmCustomAlert(InfSizeIndexLostAlert, "3", "", "");
					}
					if(recPtr->menuIndex[i].size == 0)
					{
						FrmCustomAlert(InfSizeIndexLostAlert, "4", "", "");
					}
				}
				if(Prefs.curOrder.size == (UInt8)(NoValue16))
				{
					FrmCustomAlert(InfSizeIndexLostAlert, "5", "", "");
				}
				if(Prefs.curOrder.size == 0)
				{
					FrmCustomAlert(InfSizeIndexLostAlert, "6", "", "");
				}
				if(recPtr->size == (UInt8)(NoValue16))
				{
					FrmCustomAlert(InfSizeIndexLostAlert, "7", "", "");
				}
				if(recPtr->size == 0)
				{
					FrmCustomAlert(InfSizeIndexLostAlert, "8", "", "");
				}
			}
			i++;
		}
		MemPtrUnlock(recPtr);
	}
#endif

	return(handled);
}


/***********************************************************************
 *
 * FUNCTION:    MainviewUpdateScrollers
 *
 * DESCRIPTION: P13. This routine draws or erases the list view scroll arrow
 *              buttons.
 *
 * PARAMETERS:  frm          -  pointer to the to do list form
 *              bottomRecord -  record index of the last visible record
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
/*static void MainviewUpdateScrollers (FormPtr frm, Int16 bottomRecord)
{
	Int16 upIndex;
	Int16 downIndex;
	UInt16 recNumber;
	Boolean scrollableUp;
	Boolean scrollableDown;
		
	// If the first record displayed is not the fist record in the category,
	// enable the up scroller.
	recNumber = TopVisibleRecord;
	scrollableUp = !DmSeekRecordInCategory (MmDishesDB, &recNumber, 1, 
		dmSeekBackward, Prefs.current.course);


	// If the last record displayed is not the last record in the category,
	// enable the down scroller.
	recNumber = bottomRecord;
	scrollableDown = !DmSeekRecordInCategory (MmDishesDB, &recNumber, 1, 
		dmSeekForward, Prefs.current.course); 


	// Update the scroll button.
	upIndex = FrmGetObjectIndex (frm, MainScrollUpRepeating);
	downIndex = FrmGetObjectIndex (frm, MainScrollDownRepeating);
	FrmUpdateScrollers (frm, upIndex, downIndex, scrollableUp, scrollableDown);
}
*/

/***********************************************************************
 *
 * FUNCTION:    MmEditLineHandleEvent
 *
 * DESCRIPTION: .
 *
 * PARAMETERS:  event - the most recent event.
 *
 * RETURNED:    True if the event is handled, false otherwise.
 *
 ***********************************************************************/
static Boolean MmEditLineFormHandleEvent(EventPtr event)
{
	FormType						*frm;
	Boolean						handled = false;
	UInt32						timeNow;

//	FormPtr		 				prvFormPtr;
	MemHandle 					recHandle;
	OrderTypePtr 				recPtr;

	MemHandle					oldHandle;
	MemHandle					textHandle;
	Char							*textPtr;
	UInt16						i;
	
	Char							*sourcePtr;
	Char							*destPtr;

	UInt16						fieldIndex;
	FieldType					*fieldPtr;

//	UInt16						buttonHit;
	Boolean						matchFound;

	ListPtr						listRecentPtr;							//	Recent Notes list

	UInt16						offset;
	UInt16						recMenuIndex = 0;
	UInt16						itemSelected;



	frm = FrmGetActiveForm();

	timeNow = MmProcessEventIncludingNilEvent(frm, event, 0, 0);


	fieldIndex = FrmGetObjectIndex(frm, EditLineNoteField);
	fieldPtr = (FieldType *) FrmGetObjectPtr(frm, fieldIndex);

	if(FrmGetFocus(frm) == noFocus)
		FrmSetFocus(frm, fieldIndex);			// Set the insertion point blinking in the note field

	switch(event->eType)
	{
		case ctlSelectEvent:  // A control button was pressed and released.

			if(event->data.ctlSelect.controlID == EditLineOkayButton)
			{
				textPtr = FldGetTextPtr(fieldPtr);
				
				// Truncate the string to ensure it's not too long
//				textPtr[MaxNoteLength] = '\0';

				for(i=0; i < StrLen(textPtr); i++)
				{
					if((textPtr[i] < 0x20) || (textPtr[i] > 0x7A))
					{
						textPtr[i] = ' ';
					}
				}

				//	Check if this note already exists in the recent list. If so, don't record it
				matchFound = false;
				for(i=0; i < MaxRecentNotesListItems; i++)
				{
					if(StrCaselessCompare((const char *)textPtr, Prefs.recentNotes[i]) == 0)
					{
						matchFound = true;
						break;
					}
				}

				//	Note isn't in the list, so add it
				if(!matchFound)
				{
					for(i=0; i < StrLen(textPtr); i++)
					{
						Prefs.recentNotes[RecentNotesThis][i] = textPtr[i];
					}
					Prefs.recentNotes[RecentNotesThis][i] = 0;	//	Terminate the string
					RecentNotesThis++;
					if(RecentNotesThis >= MaxRecentNotesListItems)
						RecentNotesThis = 0;
				}
				MmDetailsStoreOptionsOrNote(0, (UInt8*)textPtr);


				FrmReturnToForm(DetailsForm);
				frm = FrmGetActiveForm();

				MmShowNoteInButton(frm, DetailsAttachNoteButton, Prefs.curOrder.note, &Prefs);
			}


			else if(event->data.ctlSelect.controlID == EditLineCancelButton)
			{
				FrmReturnToForm(DetailsForm);
				frm = FrmGetActiveForm();

				MmShowNoteInButton(frm, DetailsAttachNoteButton, Prefs.curOrder.note, &Prefs);
			}


			else if(event->data.ctlSelect.controlID == EditLineRecentButton)
			{
				//	The Recent Notes pick list was selected
				listRecentPtr = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, EditLineRecentList));

				//	Then get the Note selected
				offset = 0;
				itemSelected = MmPopupList(listRecentPtr, RecentNotesList, &offset, 0);

				if(itemSelected != NoValue16)
				{
					textHandle = MemHandleNew(MaxNoteLength + 1);
					textPtr = MemHandleLock(textHandle);
					
					StrCopy(textPtr, Prefs.recentNotes[itemSelected]);

					MemHandleResize(textHandle, StrLen(textPtr)+1);
					MemHandleUnlock(textHandle);

					// Set the field to edit the note.
					oldHandle = FldGetTextHandle(fieldPtr);
					FldSetTextHandle(fieldPtr, textHandle);
					if (oldHandle)
						MemHandleFree(oldHandle);

					FldDrawField(fieldPtr);
					FrmSetFocus(frm, fieldIndex);	// Set insertion point blinking in note field
				}
			}

			handled = true;

			break;



		case frmOpenEvent:

			FrmDrawForm(frm);

			//	First get MemHandle and MemPtr to the Order record
			recHandle = DmQueryRecord(MmOrdersDB, Prefs.current.recIndex);
			recPtr = MemHandleLock(recHandle);	// Lock down the block containing the Order record.

			textHandle = MemHandleNew(MaxNoteLength + 1);
			textPtr = MemHandleLock(textHandle);
			
			//	Try and get existing note if possible
			if(recPtr->noteLength)
			{
				sourcePtr = (Char*)&recPtr->note;
				destPtr = textPtr;
				for(i=0; i < recPtr->noteLength; i++)
					*destPtr++ = *sourcePtr++;
				*destPtr = 0;
			}
			else
				StrCopy(textPtr, "");

			MemHandleResize(textHandle, StrLen(textPtr)+1);
			MemHandleUnlock(textHandle);

			oldHandle = FldGetTextHandle(fieldPtr);
			FldSetTextHandle(fieldPtr, textHandle);
			if (oldHandle)
				MemHandleFree(oldHandle);

//			FldDrawField(fieldPtr);
			FrmSetFocus(frm, fieldIndex);			// Set the insertion point blinking in the note field
			FldDrawField(fieldPtr);

			handled = true;

			break;



/*		case frmObjectFocusTakeEvent:

//ObjectFocusTakeEvent
			handled = true;

			break;
*/
	}
	return(handled);
}



/*
	FldGetTextHandle or FldGetTextPtr, and perhaps FldCompactText 



	//To initialize the text in a field use the following:

		MemHandle h, oldHandle;
		char *p;
		FieldType* fldP;
		FormType* frmP;

		frmP = FrmGetActiveForm();
		h = MemHandleNew(50);
		p = (char *) MemHandleLock(h);
		StrCopy(p, "George Washington");  // you'd copy from saved data, presumably
		MemHandleResize(h, StrLen(p)+1);
		MemHandleUnlock(h);

		fldP = (FieldType *) FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, MainNameField));
		oldHandle = FldGetTextHandle(fldP);
		FldSetTextHandle(fldP, h);
		if (oldHandle)
			MemHandleFree(oldHandle); // or do something else with it first
*/	  
	/*  
	If you don't want the field to free the handle when the form closes, then in your
	frmCloseEvent get the handle with FldGetTextHandle() and then set the field to use
	a zero handle with FldSetTextHandle(fldP, 0). Then you are the "owner" of the handle
	containing the user's input, and can use it as you wish, remembering to free it when
	you are done.
	*/


/*
			while(buttonHit != EditLineOkayButton && buttonHit != EditLineCancelButton)
			{
				buttonHit = FrmDoDialog(frm);
		//		SysKeyboardDialog(kbdAlpha);			//	Why doesn't this work???

				// Truncate the string to ensure it's not too long
				textPtr[MaxNoteLength] = '\0';

				//	Save the text if the Okay button was pressed. Otherwise trash it.
				if(buttonHit == EditLineOkayButton)
				{
					for(i=0; textPtr[i] != 0; i++)
					{
						if((textPtr[i] < 0x20) || (textPtr[i] > 0x7A))
						{
							textPtr[i] = ' ';
						}
					}
				}
				else if(buttonHit == EditLineRecentButton)
				{
					//	The Recent Notes pick list was selected
					listRecentPtr = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, EditLineRecentList));

					//	Then get the Note selected
					offset = 0;
					itemSelected = MmPopupList(listRecentPtr, RecentNotesList, &offset);

					if(itemSelected != NoValue16)
					{
						StrCopy(textPtr, Prefs.recentNotes[itemSelected]);
						FldDrawField(fieldPtr);
						FrmSetFocus(frm, fieldIndex);		// Set insertion point blinking in note field
						FldSetTextHandle(fieldPtr, textHandle);
					}
				}
				else if(buttonHit == EditLineCancelButton)
				{
				}
				else
				{
					MemPtrUnlock(recPtr);
					MemPtrUnlock(textPtr);
					handled = true;

					return(handled);
				}
			}

			if(buttonHit == EditLineOkayButton || buttonHit == EditLineRecentButton)
			{
				//	Check if this note already exists in the recent list. If so, don't record it
				matchFound = false;
				for(i=0; i < MaxRecentNotesListItems; i++)
				{
					if(StrCaselessCompare((const char *)textPtr, Prefs.recentNotes[i]) == 0)
					{
						matchFound = true;
						break;
					}
				}

				//	Note isn't in the list, so add it
				if(!matchFound)
				{
					for(i=0; textPtr[i] != 0; i++)
					{
						Prefs.recentNotes[RecentNotesThis][i] = textPtr[i];
					}
					Prefs.recentNotes[RecentNotesThis][i] = 0;	//	Terminate the string
					RecentNotesThis++;
					if(RecentNotesThis >= MaxRecentNotesListItems)
						RecentNotesThis = 0;
				}
				MmDetailsStoreOptionsOrNote(0, (UInt8*)textPtr);
			}
			MmRefreshAutoLogout(true, 0);				//	Update the Prefs.autoLogoutSecs global


		//	MemPtrUnlock(recPtr);
			MemPtrUnlock(textPtr);

			FrmDeleteForm(frm);							// Delete the field's new text from memory.
			FrmSetActiveForm(prvFormPtr);

			FrmReturnToForm(DetailsForm);				//	Refresh table and redraw Main form


			frm = FrmGetActiveForm();
			MmShowNoteInButton(frm, DetailsAttachNoteButton, Prefs.curOrder.note, Prefs.current.fFontID);

			handled = true;
		}
*/





/***********************************************************************
 *
 * FUNCTION:		MainviewDrawRecordInBounds
 *
 * DESCRIPTION:	P14. Draw an item within a passed bounds
 *
 * PARAMETERS:		MmRecP - the record to draw
 *						bounds - bounds in which to draw the record
 *
 * RETURNED:		Nothing.
 *
 ***********************************************************************/
/*static void MmMainviewDrawRecordInBounds(UInt8* dishPtr, RectanglePtr bounds,
														UInt8 itemNumber,UInt16 status)
{
	UInt16						i,j,k=50;
	RectangleType			space = {0, 34, 160, 125};

	char						bulletChar[2];	// = "\x95";


	//	Set the Rectangle area to be cleared before the new bullet is drawn.
	RctSetRectangle(&space, 0, 34, 160, 125);
	WinEraseRectangle(&space, 0);
	
	for(i=0x82,j=3; i<0xC0; i++,j+=12)
	{
		if(j > 148)
		{
			j = 5;
			k += 15;
		}
		bulletChar[0] = i;
		WinDrawChars(bulletChar, 1, j, k);
	}


/*	for(i=0xC0,j=3; i<0x100; i++,j+=12)
	{
		if(j > 148)
		{
			j = 5;
			k += 15;
		}
		bulletChar[0] = i;
		WinDrawChars(bulletChar, 1, j, k);
	}


	for(i=3,j=5; i<15; i++,j+=15)
	{
			FntSetFont(symbolFont);
			bulletChar[0] = i;
			WinDrawChars(bulletChar, 1, j, 50);
			FntSetFont(Prefs.current.fFontID);
	}

	for(j=5; i<23; i++,j+=15)
	{
			FntSetFont(symbolFont);
			bulletChar[0] = i;
			WinDrawChars(bulletChar, 1, j, 65);
			FntSetFont(Prefs.current.fFontID);
	}



	for(i=1,j=5; i<5; i++,j+=15)
	{
			FntSetFont(symbol7Font);
			bulletChar[0] = i;
			WinDrawChars(bulletChar, 1, j, 80);
			FntSetFont(Prefs.current.fFontID);
	}



	for(i=0,j=5; i<4; i++,j+=15)
	{
			FntSetFont(symbol11Font);
			bulletChar[0] = i;
			WinDrawChars(bulletChar, 1, j, 95);
			FntSetFont(Prefs.current.fFontID);
	}

}
*/


/***********************************************************************
 *
 * FUNCTION:		MainviewDrawRecord
 *
 * DESCRIPTION:	P12. Draw an item in the main form's table.  This 
 *						routine is called from the table object and must match
 *						the parameters that the table object passes.  The routine
 *						MainviewLoadRecords sets the table object to call this
 *						routine.  The table object calls it once for every table 
 *						cell needing drawing.
 *
 * PARAMETERS:		tableP - table in which to draw the record
 *						row - row of the record to change
 *						column - column of the record to change
 *						bounds - bounds in which to draw the record
 *
 * RETURNED:		Nothing.
 *
 ***********************************************************************/
static void MmMainviewDrawRecord(MemPtr tableP, Int16 tableRow, Int16 column, RectanglePtr bounds)
{
	MemHandle				recHandle;
	OrderTypePtr			recOrderPtr;
	MenuTypePtr				recMenuPtr;
	SizeTypePtr				recSizePtr;
	UInt16					recMenuIndex;
	UInt16					recOrderIndex;
	UInt16					servingCourseIndex;
	UInt32					servingCourseColor = 0;
	Boolean					selectable;
	UInt16					status;
	
	UInt16					row;								//	MainviewTopRowNum + tableRow
	
	Char						tempStr[MaxMenuItemSize + MaxInfoNameSize];	//	Allow for Size and Drink
	
	Char						tempStr2[10];

	DmOpenRef				menuDB;
	DmOpenRef				menuSizesDB;



	tableP = NULL;				//	Stop compiler grizzle

	row = MainviewTopRowNum + tableRow;


	if(Prefs.current.mMenu == DishesMenu)
	{
		menuDB = MmDishesDB;
		menuSizesDB = MmDishesSizesDB;
	}
	else
	{
		menuDB = MmDrinksDB;
		menuSizesDB = MmDrinksSizesDB;
	}

	StrCopy(tempStr, "");

	// Get the record number that corresponds to the table item to draw.
	// The record number is stored in the "intValue" field of the item.
	recMenuIndex = TableDisplayRows[row].dishIndex;
	recOrderIndex = TableDisplayRows[row].recIndex;
	status = TableDisplayRows[row].status;

	recHandle = DmQueryRecord(menuDB, recMenuIndex);
	if(recHandle)
	{
		recMenuPtr = MemHandleLock(recHandle);

		recHandle = DmQueryRecord(MmOrdersDB, recOrderIndex);
		if(recHandle)
		{
			recOrderPtr = MemHandleLock(recHandle);
			
			if(Prefs.current.mMenu == DishesMenu)
			{
				if(status & StatusHasLinkedItem)
				{
					SidesIndexNum++;				//	Count Sides so we know which size to show
					StrCopy(tempStr, DisplaySidesIcon);
					StrCat(tempStr, " ");
				}
				else if((recOrderPtr->setMenuMask)/*	&&
					!(recOrderPtr->setMenuMask & SetMenuKey)*/	)
				{
					SidesIndexNum = 0;

					StrIToA(tempStr2, recOrderPtr->setMenuGroup & ResetMSB16);
					StrCopy(tempStr, "[");
					StrCat(tempStr, tempStr2);
					StrCat(tempStr, "]");
					StrCat(tempStr, " ");
//					StrCopy(tempStr, DisplaySetMenuItemIcon);
//					StrCat(tempStr, " ");
				}
				else
				{
					SidesIndexNum = 0;
				}

				recHandle = DmQueryRecord(menuSizesDB,
												recOrderPtr->menuIndex[SidesIndexNum].size);
				if(recHandle)
				{
					recSizePtr = MemHandleLock(recHandle);
					if(StrCaselessCompare((const char *)recSizePtr->sizeName, "Default"))
					{
						StrCat(tempStr, recSizePtr->sizeName);
						StrCat(tempStr, " ");
					}
					MemPtrUnlock(recSizePtr);
				}
			}
			else		//	Must be a Drink
			{
				//	Unless its a Course number, prepend the size type to the name
				if(recMenuPtr->itemNumber != 0)
				{
					//	If it hasn't been sent, display size type
					if(status & StatusAsSentConfirmed)
					{
						if(status & StatusHasLinkedItem)
						{
							StrCopy(tempStr, DisplayDrinkMixIcon);
							StrCat(tempStr, " ");
						}
					}
					else
					{
						recHandle = DmQueryRecord(menuSizesDB, recOrderPtr->size);
						if(recHandle)
						{
							recSizePtr = MemHandleLock(recHandle);
							if(StrCaselessCompare((const char *)recSizePtr->sizeName, "Default"))
							{
								StrCopy(tempStr, recSizePtr->sizeName);
								StrCat(tempStr, " ");
							}
							MemPtrUnlock(recSizePtr);
						}
					}
				}
			}
			MmGetServingCourseIndex(MmServingCourseDB, recOrderPtr->servingCourseID,
												&servingCourseIndex, &servingCourseColor, &selectable);

			MemPtrUnlock(recOrderPtr);
		}
	

		if(column == 0)
		{
			if(recMenuPtr->enabled & StatusAsEnabled)
				StrCat(tempStr, recMenuPtr->itemName);
			else				
				StrCat(tempStr, recMenuPtr->disabledItemName);

			MmMainviewDrawRecordInBounds(tempStr, bounds, recMenuPtr->itemNumber, status, 
																	servingCourseColor, &Prefs);
		}
		MemPtrUnlock(recMenuPtr);
	}
	else
	{
		MmMainviewDrawRecordInBounds(tempStr, bounds, 0, status, 0, &Prefs);
	}
}



/***********************************************************************
 *
 * FUNCTION:    MainviewScroll
 *
 * DESCRIPTION: P13. This routine scrolls the list of Menu titles
 *              in the direction specified.
 *
 *					 Scrolling up stops at the first record visible.  
 *					 Because of categories and private records the first
 *					 record visible isn't neccessarily record 0.
 *
 *					 Scrolling down stops when less than a full table of
 *					 records can be displayed.  To enfore this when the 
 *					 table is scrolled down, we check if at the new 
 *					 position there are enough records visible to fill
 *					 the table.  If not we find the last records visible
 *					 by working backwards from the end.
 *
 * PARAMETERS:  direction - up or dowm
 *					 oneLine - true scrolls one line, false scrolls one page
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
/*static void MainviewScroll (DirectionType direction, Boolean oneLine)
{
	TablePtr tableP;
	Int16 		rowsInTable;
	UInt16		newTopVisibleRecord;
	FormPtr	frmP;

	tableP = GetObjectPtr (MainOrderTable);
	rowsInTable = TblGetNumberOfRows (tableP);
	newTopVisibleRecord = TopVisibleRecord;
	Prefs.current.record = noRecordSelected;

	// Scroll the table down.
	if (direction == down)
	{
		// Scroll down a single line.
		if (oneLine)
		{
			DmSeekRecordInCategory (MmDishesDB, &newTopVisibleRecord, 1, 
				dmSeekForward, Prefs.current.course);
		}

		// Scroll down a page (less one row).
		else
		{
			// Try going forward one page
			if (DmSeekRecordInCategory (MmDishesDB, &newTopVisibleRecord, 
					rowsInTable - 1, dmSeekForward, Prefs.current.course))
			{
				// Try going backwards one page from the last record
				newTopVisibleRecord = dmMaxRecordIndex;
				DmSeekRecordInCategory (MmDishesDB, &newTopVisibleRecord, 
					rowsInTable - 1, dmSeekBackward, Prefs.current.course);
			}
		}
	}



	// Scroll the table up.
	else
	{
		// Scroll up a single line
		if (oneLine)
		{
			DmSeekRecordInCategory (MmDishesDB, &newTopVisibleRecord, 1, 
				dmSeekBackward, Prefs.current.course);
		}

		// Scroll up a page (less one row).
		else
		{
			if (DmSeekRecordInCategory (MmDishesDB, &newTopVisibleRecord, 
				rowsInTable - 1, dmSeekBackward, Prefs.current.course))
			{
				// Not enough records to fill one page.  Start with the first record
				newTopVisibleRecord = 0;
				DmSeekRecordInCategory (MmDishesDB, &newTopVisibleRecord, 0, 
					dmSeekForward, Prefs.current.course);
			}
		}
	}


	// Avoid redraw if no change
	if (TopVisibleRecord != newTopVisibleRecord)
	{
		// The table should be at a different position.  Load the
		// table with new records and redraw it.
		TopVisibleRecord = newTopVisibleRecord;
		frmP = FrmGetActiveForm ();
		MainviewLoadRecords (frmP);
		TblRedrawTable(tableP);
	}
}

*/
/***********************************************************************
 *
 * FUNCTION:		MmZapOrdersDB
 *
 * DESCRIPTION:	Zap the Databases without asking.
 *
 * PARAMETERS:		Nothing.
 *
 * RETURNED:		true if successful.
 *
 ***********************************************************************/
/*Boolean MmZapOrdersDB(void)
{
	Int16		error;						// P5. error code
	UInt16		cardNo;						// P5. card containing the application database
	LocalID	dbID;							// P5. handle for application database
	UInt16		dbAttrs;						// P5. database attributes
	UInt16		mode;							// P5. permissions when opening the database
//	SystemPreferencesType sysPrefs;	// P10. The system's preferences
	Boolean 	recordFound = false;		// P15. whether we use the record used before
												// This will be false if the record is now hidden.


	mode = dmModeReadWrite;

	//	Delete and Recreate the Orders database

	//	Delete the Orders database.
//	if(error = DmResetRecordStates(MmOrdersDB))
//		ErrFatalDisplayIf(error, "Could not Reset MmOrdersDB.");

	DmOpenDatabaseInfo(MmOrdersDB, &dbID, NULL, NULL, &cardNo, NULL);
	DmCloseDatabase(MmOrdersDB);
	error = DmDeleteDatabase(cardNo, dbID);
	if(error)
	{
//		PrvSyncLogTrace("Error is: %x\n", error);
		return false;
		ErrFatalDisplayIf(error, "Could not delete MmOrdersDB.");
	}

	// Create a new, empty one.
	error = DmCreateDatabase(0, MmOrdersDBName, MmAppType, MmDBType, false);
	ErrFatalDisplayIf(error, "Could not Create MmOrdersDB.");

	// Check for error.  If the database couldn't be created report it.
//	if (error)
//		return true;
	
	// Find the new database.
	dbID = DmFindDatabase(0, MmOrdersDBName);
	MmOrdersDB = DmOpenDatabase(0, dbID, mode);

	// Get info about the database
	error = DmOpenDatabaseInfo(MmOrdersDB, &dbID, NULL, NULL, &cardNo, NULL);
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

	MmCreateFinalRecord(&Prefs, &DBs, LastItemOrdered);

	return true;	
}
*/

/***********************************************************************
 *
 * FUNCTION:		MmZapInfoDB
 *
 * DESCRIPTION:	Zap the Database without asking.
 *
 * PARAMETERS:		Nothing.
 *
 * RETURNED:		true if successful.
 *
 ***********************************************************************/
/*Boolean MmZapInfoDB(void)
{
	Int16		error;						// P5. error code
	UInt16		cardNo;						// P5. card containing the application database
	LocalID	dbID;							// P5. handle for application database
	UInt16		dbAttrs;						// P5. database attributes
	UInt16		mode;							// P5. permissions when opening the database
//	SystemPreferencesType sysPrefs;	// P10. The system's preferences
	Boolean 	recordFound = false;		// P15. whether we use the record used before
												// This will be false if the record is now hidden.

	mode = dmModeReadWrite;

	//	Delete and Recreate the Info database

	DmOpenDatabaseInfo(MmInfoDB, &dbID, NULL, NULL, &cardNo, NULL);
	DmCloseDatabase(MmInfoDB);
	error = DmDeleteDatabase(cardNo, dbID);
	if(error)
	{
//		PrvSyncLogTrace("Error is: %x\n", error);
		return false;
		ErrFatalDisplayIf(error, "Could not delete MmInfoDB.");
	}

	// Create a new, empty one.
	error = DmCreateDatabase(0, MmInfoDBName, MmAppType, MmDBType, false);
	ErrFatalDisplayIf(error, "Could not Create MmInfoDB.");

	// Check for error.  If the database couldn't be created report it.
//	if (error)
//		return true;
	
	// Find the new database.
	dbID = DmFindDatabase(0, MmInfoDBName);
	MmInfoDB = DmOpenDatabase(0, dbID, mode);

	// Get info about the database
	error = DmOpenDatabaseInfo(MmInfoDB, &dbID, NULL, NULL, &cardNo, NULL);
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

	return true;	
}
*/
/***********************************************************************
 *
 * FUNCTION:		MmNukeOrdersDB
 *
 * DESCRIPTION:	Delete the entire orders DB.
 *
 * PARAMETERS:		.
 *
 * RETURNED:		True if the successful, false otherwise.
 *
 ***********************************************************************/
/*static Boolean MmNukeOrdersDB(void)
{
	UInt16				unSentCounter = 0;
	Char				tempStr[10];

	//	Check through all the records in case some are not sent
	unSentCounter = MmNumUnsentOrdersDB(&MmOrdersDB);

	if(unSentCounter != 0)
	{
		StrIToA(tempStr, unSentCounter);				// Convert count to string
		if(FrmCustomAlert(ConfDeleteUnbilledAlert, tempStr, "", "") != ConfDeleteUnbilledOK)
		{
			// The delete was not confirmed.
			return false;
		}
	}
	
	MmZapDB(&MmOrdersDB, MmOrdersDBName, RecreateDB);

	return true;
}
*/




/***********************************************************************
 *
 * FUNCTION:    MmSystemInfoFormHandleEvent
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  event - the most recent event.
 *
 * RETURNED:    True if the event is handled, false otherwise.
 *
 ***********************************************************************/
static Boolean MmSystemInfoFormHandleEvent(EventPtr event)
{
	FormPtr						frm;
	Boolean						handled = false;

	LocalID						dbID;									//	Used for DmDatabaseSize() below
	UInt32						numberOfRecords;
	UInt32						totalBytes;
	UInt32						dataBytes;
	UInt32						bytesFree;
	UInt32						timeNow;

	Char							tempStr[maxStrIToALen];
	Char							nameStr[20];
	Char							displayStr[50];

	UInt16						recIndex;
	OrderTypePtr				recPtr;
	MemHandle 					recHandle;
	UInt16						numRecords;
	
	UInt16						totalOrders = 0;
	UInt16						totalTables = 0;
	UInt16						totalSeats = 0;
	UInt16						thisTable = 0;
	UInt16						thisSeat = 0;
	
	Err							err;
	
	UInt16						i;

	

	frm = FrmGetActiveForm();

	timeNow = MmProcessEventIncludingNilEvent(frm, event, 0, 0);


	if(event->eType == ctlSelectEvent)
	{
   	// A control button was pressed and released.	
		switch (event->data.ctlSelect.controlID)
		{
			//	This 'Secret' button allows access to the Display Data and also Test Menus
			//	Security should really be enhanced, perhaps by requiring simultaneaous
			//	pressing of a hard key to allow access, or entry of a PIN(?)
	   	case SystemInfoDebugDataButton:

//				FrmPopupForm(DebugDataForm);
				FrmGotoForm(DebugDataForm);

				handled = true;
				break;


			case SystemInfoOkayButton:
	   		// The Done button was pressed so apply the changes.
				
				// Remove the details form and display the details form.
//				frm = FrmGetFormPtr(SystemInfoForm);
//				FrmEraseForm(frm);					// Clear Main form avoids trash on screen
				PreviousForm = OverviewForm;
				FrmGotoForm(TableForm);
//				FrmGotoForm(MainForm);

				handled = true;
				break;
		}
	}


	if(event->eType == frmOpenEvent)
	{
		dbID = DmFindDatabase(0, MmOrdersDBName);
		DmDatabaseSize(0, dbID, &numberOfRecords, &totalBytes, &dataBytes);
		MemCardInfo(0, 0, 0, 0, 0, 0, 0, &bytesFree);

		numRecords = DmNumRecords(MmOrdersDB);
	
		for(recIndex=0; recIndex < numRecords; recIndex++)
		{
			recHandle = DmQueryRecord(MmOrdersDB, recIndex);
			if(recHandle)
			{
				recPtr = MemHandleLock(recHandle);

				if(recPtr->tableNumber != thisTable)
				{
					totalTables++;
					thisTable = recPtr->tableNumber;
					thisSeat = 0;
				}

				if(recPtr->seatNumber != thisSeat)
				{
					totalSeats++;
					thisSeat = recPtr->seatNumber;
				}

				if(recPtr->courseNumber != 0)
				{
					totalOrders++;
				}

				MemPtrUnlock(recPtr);
			}
		}
		
		if(totalTables > 0)
			totalTables--;
		
		if(totalSeats > 0)
			totalSeats--;
		
		FrmCopyLabel(frm, SystemInfoPalmNameLabel, Prefs.palmRegistrationName);
#ifndef ReleaseVersion
		FrmCopyLabel(frm, SystemInfoPalmNameLabel, "Palm 1");
#endif

		FrmCopyLabel(frm, SystemInfoDishesMenuLabel, Prefs.dishesMenuName);
		FrmCopyLabel(frm, SystemInfoDrinksMenuLabel, Prefs.drinksMenuName);
		
		StrIToA(tempStr, totalOrders);
		FrmCopyLabel(frm, SystemInfoNumOrdersLabel, tempStr);
		
		StrIToA(tempStr, Prefs.numUnsentOrders);
		FrmCopyLabel(frm, SystemInfoNumUnsentLabel, tempStr);
		
		StrIToA(tempStr, totalTables);
		FrmCopyLabel(frm, SystemInfoNumTablesLabel, tempStr);
		
		StrIToA(tempStr, totalSeats);
		FrmCopyLabel(frm, SystemInfoNumSeatsLabel, tempStr);

		StrIToA(tempStr, bytesFree);
		FrmCopyLabel(frm, SystemInfoMemoryLabel, tempStr);

		StrIToA(tempStr, bytesFree / (sizeof(OrderType)+10));
		//	10 is fudge factor to allow for notes, options etc.
		FrmCopyLabel(frm, SystemInfoFreeOrdersLabel, tempStr);



		//	Bluetooth
		if(Prefs.btDevice != NoValue8)
		{
			for(i=0; Prefs.btNamesBuffer[Prefs.btDevice][i] && i < 19; i++)
			{
				displayStr[i] = Prefs.btNamesBuffer[Prefs.btDevice][i];
			}
			displayStr[i] = 0;
		}
		else
		{
			StrCopy(displayStr, "Not Discovered");
#ifndef ReleaseVersion
			StrCopy(displayStr, "POS 1");
#endif
		}
		FrmCopyLabel(frm, SystemInfoBtNameLabel, displayStr);


/*Parameters 	-> btLibRefNum Reference number for the Bluetooth library.
				-> btDevP Address of a Bluetooth device. This parameter must not be NULL.
				<- spaceP Pointer to a buffer to store the ASCII formatted Bluetooth devices address upon return. This parameter must not be NULL.
				-> spaceSize Size of the spaceP buffer, in bytes. Must be at least 18.
*/

		if(BtLibRefNum && BtLibRefNum != NoValue16)
		{
			err = BtLibAddrBtdToA(BtLibRefNum, &Prefs.btDevices[Prefs.btDevice],
																				nameStr, sizeof(nameStr));
			if(err == btLibErrNoError)
			{
				StrCopy(displayStr, "= ");
				StrCat(displayStr, nameStr);
				FrmCopyLabel(frm, SystemInfoBtAddressLabel, displayStr);
#ifndef ReleaseVersion
				FrmCopyLabel(frm, SystemInfoBtAddressLabel, "00:80:C8:26:F8:1C");
#endif
			}
			else
			{
				FrmCopyLabel(frm, SystemInfoBtAddressLabel, "");
			}

			if(StrLen(Prefs.palmUniqueName))
			{
				StrCopy(displayStr, "= ");
				StrCat(displayStr, Prefs.palmUniqueName);
				FrmCopyLabel(frm, SystemInfoBtPalmAddressLabel, displayStr);
#ifndef ReleaseVersion
				FrmCopyLabel(frm, SystemInfoBtPalmAddressLabel, "00:07:E0:4E:63:92");
#endif
			}
			else
			{
				FrmCopyLabel(frm, SystemInfoBtPalmAddressLabel, "");
			}
		}
		else
		{
			FrmCopyLabel(frm, SystemInfoBtPalmAddressLabel, "no BT");
		}

		// Draw the details form and the Selected Options list.
		FrmDrawForm(frm);
		
		handled = true;
	}
	
	
	return(handled);
}



/***********************************************************************
 *
 * FUNCTION:    MmSystemInfoFormHandleEvent
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  event - the most recent event.
 *
 * RETURNED:    True if the event is handled, false otherwise.
 *
 ***********************************************************************/
static Boolean MmScreenLockFormHandleEvent(EventPtr event)
{
	FormPtr						frm;
	Boolean						handled = false;
	UInt32						timeNow;

	// The form was told to open.  It has already been loaded and activated.
	frm = FrmGetFormPtr(ScreenLockForm);

	timeNow = MmProcessEventIncludingNilEvent(frm, event, 0, 0);

	if(timeNow >= ScreenLockStruct.unlockSecs)
	{
		ScreenLockStruct.unlockSecs = NoValue32;
		FrmCopyLabel(frm, ScreenLockPress1then2Label, "<    Press to Unlock");

		FrmHideObject(frm, FrmGetObjectIndex(frm, ScreenLock2Button));

		FrmShowObject(frm, FrmGetObjectIndex(frm, ScreenLock1Button));
		FrmSetFocus (frm, FrmGetObjectIndex(frm, ScreenLock1Button));
		FrmUpdateForm (FrmGetActiveFormID(), frmRedrawUpdateCode);
	}

	switch(event->eType)
	{
		case ctlSelectEvent:
		case keyDownEvent:

			if(		(event->data.ctlSelect.controlID == ScreenLock1Button)	||
						(NavDirectionPressed(event, Left))
																										)
			{
				MmScreenLockFunctions(ScreenLock1Button, frm, PreviousForm, timeNow, &ScreenLockStruct);
				handled = true;
			}
			else if(	(event->data.ctlSelect.controlID == ScreenLock2Button)	||
						(NavDirectionPressed(event, Right))
																										)
			{
				MmScreenLockFunctions(ScreenLock2Button, frm, PreviousForm, timeNow, &ScreenLockStruct);
				handled = true;
			}
			else if(	(event->data.ctlSelect.controlID == vchrHardRockerCenter)||
						(NavSelectPressed(event))
																										)
			{
				MmScreenLockFunctions(vchrRockerCenter, frm, PreviousForm, timeNow, &ScreenLockStruct);
				handled = true;
			}

			break;


		case frmOpenEvent:
			// Draw the details form and the Selected Options list.
			FrmDrawForm(frm);
			
			FrmHideObject(frm, FrmGetObjectIndex(frm, ScreenLock2Button));

			FrmCopyLabel(frm, ScreenLockPress1then2Label, "<    Press to Unlock");
			ScreenLockStruct.unlockSecs = NoValue32;
			
			handled = true;
			break;

	}
	
	
	return(handled);
}




/***********************************************************************
 *
 * FUNCTION:    MmDebugDataFormHandleEvent
 *
 * DESCRIPTION: P10. Handles processing of events for the ÒdetailsÓ form.
 *
 * PARAMETERS:  event - the most recent event.
 *
 * RETURNED:    True if the event is handled, false otherwise.
 *
 ***********************************************************************/
static Boolean MmDebugDataFormHandleEvent(EventPtr event)
{
	FormPtr					frm;
	Boolean					handled = false;
	UInt32					timeNow;

	LocalID					dbID;									//	Used for DmDatabaseSize() below
	UInt32					numberOfRecords;
	UInt32					totalBytes;
	UInt32					dataBytes;
	UInt32					bytesFree;

	Char						tempStr[10];

	MemHandle 				recHandle;
	UInt16					recIndex;
	OrderTypePtr			recPtr;
	UInt16					numRecords;
	
	UInt16					totalOrders;
	UInt16					totalTables;
	UInt16					totalSeats;
	UInt16					thisTable;
	UInt16					thisSeat;
	

#ifdef TestMenus
	Err						err;
	UInt16					status;
#endif


	frm = FrmGetActiveForm();

	timeNow = MmProcessEventIncludingNilEvent(frm, event, 0, 0);

//	if(timeNow != LastTimeNowSecs)
	{
//		LastTimeNowSecs = timeNow;
	
		if(timeNow <= Prefs.autoLogoutSecs)
		{
			StrIToA(tempStr, (Prefs.autoLogoutSecs - timeNow) / 60);
			FrmHideObject(frm, FrmGetObjectIndex(frm, DebugDataLogOffMinsLabel));
			FrmCopyLabel(frm, DebugDataLogOffMinsLabel, tempStr);
			FrmShowObject(frm, FrmGetObjectIndex(frm, DebugDataLogOffMinsLabel));

			StrIToA(tempStr, (Prefs.autoLogoutSecs - timeNow) % 60);
			FrmHideObject(frm, FrmGetObjectIndex(frm, DebugDataLogOffSecsLabel));
			FrmCopyLabel(frm, DebugDataLogOffSecsLabel, tempStr);
			FrmShowObject(frm, FrmGetObjectIndex(frm, DebugDataLogOffSecsLabel));
		}

		if(Prefs.loggedIn)
			StrCopy(tempStr, "Logged In");
		else
			StrCopy(tempStr, "LoggedOff");

		FrmHideObject(frm, FrmGetObjectIndex(frm, DebugDataLoginStatusLabel));
		FrmCopyLabel(frm, DebugDataLoginStatusLabel, tempStr);
		FrmShowObject(frm, FrmGetObjectIndex(frm, DebugDataLoginStatusLabel));
	}

	if(event->eType == ctlSelectEvent)
	{
   	// A control button was pressed and released.	
		switch (event->data.ctlSelect.controlID)
		{
			case DebugDataOkayButton:
	   		// The Done button was pressed so apply the changes.
				
				// Remove the Debug form and display the SystemInfoForm form.
				FrmGotoForm(SystemInfoForm);
				handled = true;
				break;

#ifdef AllowDebugLogDatabaseButton
			case DebugDataLogDBButton:
				//	Step through the DB, writing all records to Infosync log
				MmLogDatabase(&MmOrdersDB);

				handled = true;
				break;
#endif
		}
	}

	else if(event->eType == menuEvent)
	{
		// A menu item was selected.
		
		// First clear the menu status from the display.
		MenuEraseStatus(0);
		
		// Do the appropriate action for the menu command selected.
		switch (event->data.menu.itemID)
		{
			case BlueToothSetFirstByteTOut:

				ShowMessage("Set First byte Timeout in SysTicks\n\nOnly alter this time under direction from IQWorks!");
				NumericalEntryItem = BlueToothSetFirstByteTOut;
				FrmPopupForm(NumberEntryFieldForm);
				break;
				
			case BlueToothSetInterByteTOut:

				ShowMessage("Set Interbyte Timeout in SysTicks\n\nOnly alter this time under direction from IQWorks!");
				NumericalEntryItem = BlueToothSetInterByteTOut;
				FrmPopupForm(NumberEntryFieldForm);

				break;
				
			case BlueToothSetSendOrdersTOut:

				ShowMessage("Set SendOrders Timeout in ms per Order\n\nOnly alter this time under direction from IQWorks!");
				NumericalEntryItem = BlueToothSetSendOrdersTOut;
				FrmPopupForm(NumberEntryFieldForm);
				break;
				
			case BlueToothSetGetAllOrdersTOut:

				ShowMessage("Set GetAllOrders Total Timeout in SysTicks\n\nOnly alter this time under direction from IQWorks!");
				NumericalEntryItem = BlueToothSetGetAllOrdersTOut;
				FrmPopupForm(NumberEntryFieldForm);
				break;
				



#ifdef ForceMenus

			case ForceFlattenBattery:

				ErrFatalDisplay("Force Error to flatten battery");
				break;



			case ForceDeregisterPalm:						// Force a Deregistered state

				Prefs.palmUniqueID = 0;
				break;



			case ForceForceLogin:						// Force a logged in state

				Prefs.loggedIn = true;
				Prefs.autoLogoutSecs = TimGetSeconds() + 2;
				MmRefreshAutoLogout(true, 0);
				break;



			case ForceNumberOfTables:								//	What it says

				ShowMessage("Set Tables, Seats, Orders per seat.\n\nNumber of Tables");
				NumericalEntryItem = ForceNumberOfTables;
				FrmPopupForm(NumberEntryFieldForm);
				break;


			case ForceNumberOfSeats:								//	What it says

				ShowMessage("Number of Seats");
				NumericalEntryItem = ForceNumberOfSeats;
				FrmPopupForm(NumberEntryFieldForm);
				break;


			case ForceNumberOfOrdersPerSeat:						//	What it says

				ShowMessage("Number of Orders per Seat");
				NumericalEntryItem = ForceNumberOfOrdersPerSeat;
				FrmPopupForm(NumberEntryFieldForm);
				break;


			case ForceNumberOfSeatNames:							//	What it says

				ShowMessage("Number of Seatnames");
				NumericalEntryItem = ForceNumberOfSeatNames;
				FrmPopupForm(NumberEntryFieldForm);
				break;


			case ForceGenerateFixedOrders:								//	What it says

				MmGenerateOrders(&DBs, &Prefs, false, 
								&GenTableOrders, &GenSeatOrders, &GenOrdersPerSeat, &GenSeatnames, &LastItemOrdered);
				ShowMessage("%u Unsent Orders", Prefs.numUnsentOrders);
				Prefs.hLStatus.HLSendOrdersStatus	= HLStatusToSend;
				break;



			case ForceGenerateRandomOrders:								//	What it says

				MmGenerateOrders(&DBs, &Prefs, true, 
								&GenTableOrders, &GenSeatOrders, &GenOrdersPerSeat, &GenSeatnames, &LastItemOrdered);
				ShowMessage("%u Unsent Orders", Prefs.numUnsentOrders);
				Prefs.hLStatus.HLSendOrdersStatus	= HLStatusToSend;
				break;

#endif

#ifdef TestMenus
/*				
			case TestLogDatabaseNow:				// Log all current orders in database

				//	Step through the DB, writing all records to Infosync log
				MmLogDatabase(&MmOrdersDB);
				break;
*/				
/*			case TestEnableDatabaseLog:			// Enable logging of all orders sent

				Prefs.sendLogEnabled = true;
				break;
				
			case TestDisableDatabaseLog:			// Disable logging of all orders sent

				Prefs.sendLogEnabled = false;
				break;
*/				
			case TestClearHotSyncLog:				// Clear log of all orders sent

				DlkSetLogEntry("", 0, false);		//	Clear HotSync Log
				break;

			case TestEnableMessageLog:				// Enable logging of all messages sent

				Prefs.msgLogEnabled = true;
				break;
				
			case TestDisableMessageLog:			// Disable logging of all messages sent

				Prefs.msgLogEnabled = false;
				break;
				
			case TestMarkAllOrdersAsSent:					//	'Send' all orders

				//	Step through the DB, setting 'Confirmed' status for all records
				numberOfRecords = DmNumRecords(MmOrdersDB);
				for(recIndex=0; recIndex < numberOfRecords; recIndex++)
				{
					recHandle = DmGetRecord(MmOrdersDB, recIndex);
										
					// If the record was found, check for unConfirmed sent order
					if(recHandle)
					{
						recPtr = MemHandleLock(recHandle);
						if(recPtr->courseNumber != 0)			//	Don't do the Marker records
						{
							status = recPtr->status;
							//	Mark this record as sent
							status |= StatusAsSentConfirmed;	//	Reset the Confirmed status bit
							status |= StatusAsSent;				//	Reset the Sent status bit
							err = DmWrite(recPtr, (Char*)&recPtr->status - (Char*)&recPtr->tableNumber,
													&status, sizeof(recPtr->status));
							ErrFatalDisplayIf(err, "Could not write 'UnConfirmed' status to record.");
						}
						MemPtrUnlock(recPtr);
						err = DmReleaseRecord(MmOrdersDB, recIndex, true);
						ErrFatalDisplayIf(err, "Could not release record after marking as UnConfirmed.");
					}
				}
				Prefs.numUnsentOrders = MmNumUnsentOrdersDB(MmOrdersDB);

//				MmMainviewRedraw(frm, NoScroll);
				break;

			case TestUnsendAllOrders:							//	'Unsend' all orders

				MmProcessAllOrdersSentStatus(UnsendSentAndConfirmed, MmOrdersDB, &Prefs);
				Prefs.hLStatus.HLSendOrdersStatus	= HLStatusToSend;
//				MmMainviewRedraw(frm, NoScroll);
				break;



			case TestLoadDummyMenu:								// What it says

//				MmLoadDummyMenu();
				Prefs.hLStatus.HLMenuSyncStatus = HLStatusAllDone;
//				MmMainviewRedraw(frm, NoScroll);
				Prefs.palmUniqueID = 1;
				break;
#endif					
		}
	}

	else if(event->eType == frmOpenEvent)
	{
		totalOrders = 0;
		totalTables = 0;
		totalSeats = 0;

		dbID = DmFindDatabase(0, MmOrdersDBName);
		DmDatabaseSize(0, dbID, &numberOfRecords, &totalBytes, &dataBytes);
		MemCardInfo(0, 0, 0, 0, 0, 0, 0, &bytesFree);

		StrIToA(tempStr, numberOfRecords);	// Convert value to string
		FrmCopyLabel(frm, DebugDataNumOrderRecordsLabel, tempStr);
		
		numberOfRecords = DmNumRecords(MmSeatnameDB);

		StrIToA(tempStr, numberOfRecords);
		FrmCopyLabel(frm, DebugDataNumSeatRecordsLabel, tempStr);
		
		StrIToA(tempStr, dataBytes);
		FrmCopyLabel(frm, DebugDataDataLabel, tempStr);
		
		StrIToA(tempStr, totalBytes);
		FrmCopyLabel(frm, DebugDataTotalLabel, tempStr);
		
		StrIToA(tempStr, bytesFree);
		FrmCopyLabel(frm, DebugDataMemoryLabel, tempStr);
		


		numRecords = DmNumRecords(MmOrdersDB);
	
		for(recIndex=0; recIndex < numRecords; recIndex++)
		{
			recHandle = DmQueryRecord(MmOrdersDB, recIndex);
			if(recHandle)
			{
				recPtr = MemHandleLock(recHandle);

				if(recPtr->tableNumber != thisTable)
				{
					totalTables++;
					thisTable = recPtr->tableNumber;
					thisSeat = 0;
				}

				if(recPtr->seatNumber != thisSeat)
				{
					totalSeats++;
					thisSeat = recPtr->seatNumber;
				}

				if(recPtr->courseNumber != 0)
				{
					totalOrders++;
				}

				MemPtrUnlock(recPtr);
			}
		}

		if(totalTables > 0)
			totalTables--;
		
		if(totalSeats > 0)
			totalSeats--;
		
		StrIToA(tempStr, totalOrders);
		FrmCopyLabel(frm, DebugDataNumOrdersLabel, tempStr);
		
		StrIToA(tempStr, totalTables);
		FrmCopyLabel(frm, DebugDataNumTablesLabel, tempStr);
		
		StrIToA(tempStr, totalSeats);
		FrmCopyLabel(frm, DebugDataNumSeatsLabel, tempStr);
		


		StrIToA(tempStr, Prefs.staffIDNo);
		FrmCopyLabel(frm, DebugDataStaffIDNumberLabel, tempStr);
		
		StrIToA(tempStr, Prefs.palmUniqueID);
		FrmCopyLabel(frm, DebugDataPalmIDNumberLabel, tempStr);
		
//		StrIToA(tempStr, (Prefs.autoLogoutSecs - TimGetSeconds())/* / 60*/);
//		FrmCopyLabel(frm, DebugDataLogOffMinsLabel, tempStr);
		
		StrIToA(tempStr, Prefs.infoSyncNo);
		FrmCopyLabel(frm, DebugDataInfoSyncNumberLabel, tempStr);
		
		StrIToA(tempStr, Prefs.menuSyncNo);
		FrmCopyLabel(frm, DebugDataMenuSyncNumberLabel, tempStr);
		
		StrIToA(tempStr, Prefs.drinkSyncNo);
		FrmCopyLabel(frm, DebugDataDrinkSyncNumberLabel, tempStr);
		
		StrIToA(tempStr, Prefs.transactionNo);
		FrmCopyLabel(frm, DebugDataTransactionNumberLabel, tempStr);
		
		StrIToA(tempStr, Prefs.btTimeoutSendOrders);
		FrmCopyLabel(frm, DebugDataBtTxTicksLabel, tempStr);
		
		StrIToA(tempStr, Prefs.btTimeoutGetAllOrders);
		FrmCopyLabel(frm, DebugDataBtRxTicksLabel, tempStr);
		
		// Draw the details form and the Selected Options list.
		FrmDrawForm(frm);
		
		handled = true;
	}
	
	
	return(handled);
}


/***********************************************************************
 *
 * FUNCTION:		MmPINEntryFormHandleEvent
 *
 * DESCRIPTION:	Handles processing of events for the Prefs.pIN Entry form.
 *
 * PARAMETERS:		event	- the most recent event.
 *
 * RETURNED:		True if the event is handled, false otherwise.
 *
 ***********************************************************************/
Boolean MmEnterPINFormHandleEvent(EventPtr event)
{
	static UInt16			index = 0;

	FormPtr					frm;
	Boolean					handled = false;
	UInt32					timeNow;
	
	UInt16					staffIDNumber;
	UInt16					i;
	
	ControlType				*buttonPtr;


	frm = FrmGetActiveForm();

	timeNow = MmProcessEventIncludingNilEvent(frm, event, 0, 0);


	frm = FrmGetActiveForm();

	buttonPtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, EnterPINDisplayButton));

	switch (event->eType)
	{
   	case ctlSelectEvent:  // A control button was pressed and released.
			
	   	// If the done button is pressed, go back to the main form.
	   	if(event->data.ctlEnter.controlID == EnterPINOkayButton)
	   	{
				if(index <= MaxPINSize)
				{
					//	Doing Registration
					if(PreviousForm == RegisterForm)
					{
			   		StrCopy(RegTemp.pIN, Prefs.pIN);
			   		PreviousForm = OverviewForm;
			   		FrmGotoForm(RegisterForm);
					}
					
					//	Doing Login
					else if(PreviousForm != RegisterForm)
					{
						Prefs.hLStatus.HLLoginStatus = HLStatusToSend;	//	Ensure Login message is sent
						staffIDNumber = Prefs.staffIDNo;
						MmGetStaffID(StaffType, &Prefs.staffIDNo, Prefs.staffIDIndex, MmInfoDB);
						
						//	Clear out the Recent Notes list if someone else logs in
						if(staffIDNumber != Prefs.staffIDNo)
						{
							for(i=0; i<MaxRecentNotesListItems; i++)
								Prefs.recentNotes[i][0] = 0;	//	Kill the note
						}

						#ifdef DebugFakeMenu
							//	This is for real now
							Prefs.loggedIn = true;
							Prefs.hLStatus.HLLoginStatus = HLStatusAllDone;
							MmGetStaffID(StaffType, &Prefs.staffIDNo, Prefs.staffIDIndex, MmInfoDB);
							StrCopy(Prefs.pIN, "PIN#");		//	Blitz Prefs.pIN for next time
							MmSavePrefs();							//	Save this now in case we break
						#endif

						//	If new orders are taken, force count to check whether old unsent orders exist
						Prefs.logoutTimeSecs = TimGetSeconds();
						NumOldUnsentOrders = NoValue16;

						PreviousForm = TableForm;

						FrmGotoForm(BeamingForm);
			   	}
			   }
		   	else
		   	{
					FrmAlert(WarnPINMustBe10CharsAlert);
		   	}
			}

	   	// If the Log Off button is pressed, go back to the main form.
			else if(event->data.ctlEnter.controlID == EnterPINLogOffButton)
			{
				Prefs.loggedIn = false;

				//	If new orders are taken, force count to check whether old unsent orders exist
				Prefs.logoutTimeSecs = TimGetSeconds();
				NumOldUnsentOrders = NoValue16;

				StrCopy(Prefs.pIN, "PIN#");				//	Not a valid Prefs.pIN
	   		FrmGotoForm(PreviousForm);
			}

			else if(event->data.ctlEnter.controlID == EnterPINClearButton)
			{
				index = 0;
				StrCopy(TempStr, "PIN#");
				StrCopy(Prefs.pIN, "PIN#");				//	Not a valid Prefs.pIN
				CtlSetLabel(buttonPtr, TempStr);
			}

			else if(	event->data.ctlEnter.controlID >= EnterPIN0Button &&
						event->data.ctlEnter.controlID <= EnterPIN9Button)
			{
				if(index < MaxPINSize)
				{
					Prefs.pIN[index] = (event->data.ctlEnter.controlID - EnterPIN0Button) + 0x30;
					
					TempStr[index++] = '*';
					CtlSetLabel(buttonPtr, TempStr);
				}
				else
				{
					FrmAlert(WarnPINMustBe10CharsAlert);
				}
			}

			handled = true;
			break;
			

  		case frmOpenEvent:	// The form was told to open.

			FrmDrawForm(frm);
			
			for(i=0; i < MaxPINSize + 1; i++)
			{
				TempStr[i] = 0;
				Prefs.pIN[i] = 0;
			}
			
			StrCopy(TempStr, "PIN#");
			CtlSetLabel(buttonPtr, TempStr);

			if(PreviousForm == RegisterForm)
			{
				FrmHideObject(frm, FrmGetObjectIndex(frm, EnterPINLogOffButton));
			}
			else
			{
				FrmShowObject(frm, FrmGetObjectIndex(frm, EnterPINLogOffButton));
			}
			index = 0;

			handled = true;
			break;
	}
	return(handled);
}


/***********************************************************************
 *
 * FUNCTION:		MmPINEntryFormHandleEvent
 *
 * DESCRIPTION:	Handles processing of events for the Prefs.pIN Entry form.
 *
 * PARAMETERS:		event	- the most recent event.
 *
 * RETURNED:		True if the event is handled, false otherwise.
 *
 ***********************************************************************/
/*Boolean MmEnterPINFormHandleEvent(EventPtr event)
{
//	static Char				Pin[5];
	static UInt16			index = 0;
	FormPtr					frm;
	Boolean					handled = false;
	
	UInt16					staffIDNumber;
	UInt16					i;
	
	Char						tempStr[5] = "";
	Char						*labelPtrL;

	RectangleType			space;
	ControlType *			buttonPtr;
//	FormLabelType			*labelPtr;

	if(event->eType != nilEvent)
		MmRefreshAutoLogout(true, 0);								//	Update the Prefs.autoLogoutSecs global


	frm = FrmGetActiveForm();

	buttonPtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, EnterPINDisplayButton));
	labelPtrL = (char *)CtlGetLabel(buttonPtr);				//	Point to Display button text
//	labelPtr = FrmGetLabel(frm, buttonPtr);				//	Point to Display button text
	RctSetRectangle(	&space,
							buttonPtr->bounds.topLeft.x+4, buttonPtr->bounds.topLeft.y,
							buttonPtr->bounds.extent.x-9, buttonPtr->bounds.extent.y-2);

	switch (event->eType)
	{
   	case ctlSelectEvent:  // A control button was pressed and released.
			
	   	// If the done button is pressed, go back to the main form.
	   	if(event->data.ctlEnter.controlID == EnterPINOkayButton)
	   	{
				if(index == 4)
				{
					Prefs.hLStatus.HLLoginStatus = HLStatusToSend;	//	Ensure Login message is sent
					staffIDNumber = Prefs.staffIDNo;
					MmGetStaffID(&Prefs.staffIDNo, Prefs.staffIDIndex, MmInfoDB);
					
					//	Clear out the Recent Notes list if someone else logs in
					if(staffIDNumber != Prefs.staffIDNo)
					{
						for(i=0; i<MaxRecentNotesListItems; i++)
							Prefs.recentNotes[i][0] = 0;	//	Kill the note
					}

					#ifdef DebugFakeMenu
						Prefs.loggedIn = true;
						Prefs.hLStatus.HLLoginStatus				= HLStatusAllDone;
						MmGetStaffID(&Prefs.staffIDNo, Prefs.staffIDIndex, MmInfoDB);	//	This is for real now
						MmSavePrefs();							//	Save this now in case we break
						StrCopy(Prefs.pIN, "PIN#");		//	Blitz Prefs.pIN for next time
					#else
						if(Prefs.userSettings.advancedUser == false)
							FrmAlert(InfLoginBeamNowAlert);
					#endif

					//	If new orders are taken, check whether old unsent orders exist
					Prefs.logoutTimeSecs = TimGetSeconds();
					NumOldUnsentOrders = NoValue16;

		   		FrmGotoForm(MainForm);
		   	}
		   	else
		   	{
					FrmAlert(WarnPINMustBe4CharsAlert);
		   	}
			}

	   	// If the Log Off button is pressed, go back to the main form.
			else if(event->data.ctlEnter.controlID == EnterPINLogOffButton)
			{
				Prefs.loggedIn = false;

				//	If new orders are taken, check whether old unsent orders exist
				Prefs.logoutTimeSecs = TimGetSeconds();
				NumOldUnsentOrders = NoValue16;

				StrCopy(Prefs.pIN, "PIN#");				//	Not a valid Prefs.pIN
	   		FrmGotoForm(MainForm);
			}

			else if(event->data.ctlEnter.controlID == EnterPINClearButton)
			{
				WinEraseRectangle(&space, 0);
				index = 0;
				StrCopy(labelPtrL, "PIN#");
				CtlDrawControl(buttonPtr);
			}

			else if(event->data.ctlEnter.controlID >= EnterPIN0Button &&
						event->data.ctlEnter.controlID <= EnterPIN9Button)
			{
				if(index > 3)
				{
					FrmAlert(WarnPINMustBe4CharsAlert);

					WinEraseRectangle(&space, 0);
					index = 0;
					StrCopy(labelPtrL, "PIN#");
					CtlDrawControl(buttonPtr);
				}
				else
				{
					Prefs.pIN[index] = (event->data.ctlEnter.controlID - EnterPIN0Button) + 0x30;
					
					WinEraseRectangle(&space, 0);
			
					*(labelPtrL + index++) = '*';
					CtlDrawControl(buttonPtr);
				}
			}

			handled = true;
			break;
			

  		case frmOpenEvent:	// The form was told to open.

			FrmDrawForm(frm);
			
			WinEraseRectangle(&space, 0);
			index = 0;
			Prefs.pIN[0] = 0;
			StrCopy(labelPtrL, "PIN#");
			CtlDrawControl(buttonPtr);

			handled = true;
			break;
	}
	return(handled);
}

*/
/***********************************************************************
 *
 * FUNCTION:		MainviewInit
 *
 * DESCRIPTION:	P7. Initializes the main form and the list object.
 *
 * PARAMETERS:		frm - pointer to the main form.
 *
 * RETURNED:		Nothing.
 *
 ***********************************************************************/
void MmMainviewInit(void)
{
	FormPtr				frm;
	TablePtr 			tablePtr;
	UInt16				displayRow = 0;
	UInt16				numRows;
	
	numRows = MainTableSizeStandardFont;


	// Get a pointer to the main form.
	frm = FrmGetActiveForm();
	
	// Get the table and it's number of rows
	tablePtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, MainOrderTable));
	

	//	Initialise any rows that haven't already been set up
	while(displayRow < numRows)
	{
		TblSetItemStyle(tablePtr, displayRow, Column0, numericTableItem);
		TblSetItemInt(tablePtr, displayRow, Column0, 0x20);
		TblSetRowUsable(tablePtr, displayRow, false);
		displayRow++;
	}


	if(MainviewTopRowNum == 0)
		MmMainviewRedraw(frm, ScrollTop, DoRewriteArray);
	else
		MmMainviewRedraw(frm, NoScroll, DoRewriteArray);

	// Set the callback routine that will draw the records.
	TblSetCustomDrawProcedure(tablePtr, Column0, MmMainviewDrawRecord);

	// Set the column usable so that it draws and accepts user input.
	TblSetColumnUsable(tablePtr, Column0, true);	

	MmShowSeatPrice(frm, MainPriceLabel, MmOrdersDB, &Prefs);
	MmDisplayStaffNameLabel(frm, MmInfoDB, Prefs.staffIDIndex, MainStaffPopTrigger, 
										OverviewPageType, Prefs.loggedIn);

	// Draw the form.
	FrmDrawForm(frm);
	MmDrawTableSeatBoxes();
}


/***********************************************************************
 *
 * FUNCTION:    MmMainviewRedraw
 *
 * DESCRIPTION: Reload the main table, redraw the screen and redisplay 
 *              the total price.
 *
 * PARAMETERS:  frm - form containing list to load
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
void MmMainviewRedraw(FormPtr frm, UInt16 ScrollType, UInt16 redraw)
{
	UInt16				recOrderIndex;
	UInt16				rowsInMainTable;
	
	if(MainviewTopRowNum < 0)
		MainviewTopRowNum = 0;
	
	if(ScrollType == ScrollTop)
	{
		MainviewTopRowNum = 0;
	}
	if(ScrollType == CanScroll)
	{
	}
	else	//	ScrollType == NoScroll
	{	
		LastItemOrdered = NoValue16;
	}

	if(redraw == DoRewriteArray)
	{
		recOrderIndex = MmFindSortTableSeat(Prefs.current.tableNo, Prefs.current.seatNo);


		if(Prefs.current.fFontID == stdFont)
			rowsInMainTable = MainTableSizeStandardFont;
		else
			rowsInMainTable = MainTableSizeLargeFont;

		
		if(Prefs.current.mMenu == DishesMenu)
		{
			MmMainviewLoadTableArrayLoop(&MmDishesDB, &MmOrdersDB, MainviewTablePage,
													TableDisplayRows, Prefs.current.mMenu, recOrderIndex,
														Prefs.current.tableNo, Prefs.current.seatNo,
															&LastItemOrdered, &MainviewTopRowNum, rowsInMainTable);
		}
		else
		{
			MmMainviewLoadTableArrayLoop(&MmDrinksDB, &MmOrdersDB, MainviewTablePage,
													TableDisplayRows, Prefs.current.mMenu, recOrderIndex,
														Prefs.current.tableNo, Prefs.current.seatNo,
																&LastItemOrdered, &MainviewTopRowNum, rowsInMainTable);
		}
		MmMainviewLoadTable(frm);
	}
	else	//	redraw == NoRewriteArray
	{
		MmMainviewLoadTable(frm);
	}
	
	MmShowSeatPrice(frm, MainPriceLabel, MmOrdersDB, &Prefs);
	MmDisplayStaffNameLabel(frm, MmInfoDB, Prefs.staffIDIndex, MainStaffPopTrigger, 
										OverviewPageType, Prefs.loggedIn);
	MmShowServingCourse(frm, MainForm, &DBs, NoValue16, &Prefs);
}


/***********************************************************************
 *
 * FUNCTION:		MainviewHandleEvent
 *
 * DESCRIPTION:	Handles processing of events for the main form.
 *
 * PARAMETERS:		event	- the most recent event.
 *
 * RETURNED:		True if the event is handled, false otherwise.
 *
 ***********************************************************************/
static Boolean MmMainviewHandleEvent(EventPtr event)
{
	FormPtr						frm;
	Int16							formId;
	ListPtr						listPtr;
	ListPtr						listServePtr;

	UInt16						offset;
	UInt16						itemNumber = 0;
	UInt16						itemSelected;

	Boolean						handled = false;

	UInt32						timeNow;

#ifdef DebugTiming1
	UInt32						startTick, endTick;
	UInt16						diffTicks;
#endif



	frm = FrmGetActiveForm();
	formId = FrmGetFormId(frm);

	timeNow = MmProcessEventIncludingNilEvent(frm, event, 0, MainStaffPopTrigger);

/*
	if(event->eType == nilEvent)
	{
		//	Only do it once per second
		if(timeNow != LastTimeNowSecs)
		{
			LastTimeNowSecs = timeNow;
		
			if(timeNow > Prefs.autoLogoutSecs)
			{
				MmRefreshAutoLogout(true, 0);				//	Update the Prefs.autoLogoutSecs global
			}
			if(!Prefs.loggedIn)
			{
				MmDisplayStaffNameLabel(frm, MmInfoDB, 0, MainStaffPopTrigger, OverviewPageType, Prefs.loggedIn);
			}
		}
	}
*/
/*	else	//	This is a real event, so sort out the auto timeout
	{
		MmRefreshAutoLogout(true, 0);						//	Update the Prefs.autoLogoutSecs global
	}
*/

/*
static void BmpChangeBitmap (FormBitmapType *bmp, Int16 rsrcID)
{
  MemHandle h;
  BitmapPtr b;


  h = DmGetResource(bitmapRsc, rsrcID);
  if (h)
  {
    b = MemHandleLock(h);
    WinDrawBitmap(b, bmp->pos.x, bmp->pos.y);
    MemHandleUnlock(h);
    DmReleaseResource(h);
  }
}
*/
			
//	initialiseMode = 0;

	switch(event->eType)
	{
		case ctlSelectEvent:  // A control button was pressed and released.

			if(event->data.ctlEnter.controlID == MainDrinksButton)
	  		{
				//	Swap icons, set Prefs.current.mMenu type
				if(Prefs.current.mMenu == DishesMenu)
				{
					Prefs.current.mMenu = DrinksMenu;
					FrmHideObject(frm, FrmGetObjectIndex(frm, MainMealsBitMap));
					FrmShowObject(frm, FrmGetObjectIndex(frm, MainDrinksBitMap));
				}
				else
				{
					Prefs.current.mMenu = DishesMenu;
					FrmHideObject(frm, FrmGetObjectIndex(frm, MainDrinksBitMap));
					FrmShowObject(frm, FrmGetObjectIndex(frm, MainMealsBitMap));
				}
				MmMainviewRedraw(frm, ScrollTop, DoRewriteArray);
			}



			else if(event->data.ctlEnter.controlID == MainOverviewButton)
	  		{
				//	If we change seat while in Overview we'll need to set MainviewTopRowNum to 1
				OldCurrentSeat = Prefs.current.seatNo;
				
				if(Prefs.userSettings.classicMode)
				{
					OverviewPageType = OverviewOrdersOnlyPage;
					PreviousForm = MainForm;
				}
				else
					OverviewPageType = OverviewTakeOrders;
				
				FrmGotoForm(OverviewForm);
			}
			

			else if(event->data.ctlEnter.controlID == MainTableButton)
	  		{
				//	Check for unsent orders, Login etc. Boolean is checkIfUnsentOrders
				if(MmBeamOrdersWithOptionalCheckForUnsent(formId, &Prefs, true, &UnsentOrdersCheckSecs))
				{
					PreviousForm = MainForm;
					FrmGotoForm(TableForm);
				}

				OverviewTopRowNum = 0;							//	Always start at Row 0
			}
			


	   	else if(event->data.ctlEnter.controlID == MainSeatButton)
	   	{
				FrmGotoForm(SeatForm);
				OverviewTopRowNum = 0;							//	Always start at Row 0
			}
			


			else if(event->data.ctlEnter.controlID == MainHappyHourButton)
	  		{
				ShowMessage("Happy Hour pricing applies");
			}
			


	   	// If the Staff trigger is pressed, pop up the Staff list.
	   	else if(event->data.ctlEnter.controlID == MainStaffPopTrigger)
	   	{
				//	Check if any unsent orders exist. false -> check for OLD and NEW unsent orders
				if(MmWarnIfUnsentOrders(MmOrdersDB, &Prefs, false, &NumOldUnsentOrders))
				{
					//	Log them out so they can't send orders through on previous account
					Prefs.loggedIn = false;

					OverviewPageType = OverviewUnsentPage;
					OverviewCheckboxType = OverviewCheckboxesUnsent;
					FrmGotoForm(OverviewForm);
				}
				else
				{
					listPtr = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, MainDishesList));

					//	Then get the Staff member selected
					itemSelected = MmPopupList(listPtr, StaffList, &offset, 0);

					//	Aborted Login
					if(itemSelected == NoValue16)
					{
						MmDisplayStaffNameLabel(frm, MmInfoDB, Prefs.staffIDIndex, 
															MainStaffPopTrigger, OverviewPageType, Prefs.loggedIn);
					}

					//	Selected 'Refresh List'
					else if(itemSelected == 0)
					{
						itemSelected = NoValue16;
						#ifndef StandAlonePalm
							Prefs.loggedIn = false;
						#endif
						Prefs.hLStatus.HLInfoSyncStatus = HLStatusToSend;	//	Force an InfoSync

						FrmGotoForm(BeamingForm);
					}

					//	Selected a User
					else
					{
						itemSelected += offset -1;					//	Is non zero if list starts higher
						#ifndef StandAlonePalm
							Prefs.loggedIn = false;
						#endif
						Prefs.staffIDIndex = itemSelected;
			   		PreviousForm = MainForm;
						FrmGotoForm(EnterPINForm);					//	Get their PIN number
					}
				}

				handled = true;
			}



	   	else if(event->data.ctlEnter.controlID == MainTableupButton)
	   	{

				//	Check for unsent orders, Login etc. Boolean is checkIfUnsentOrders
				if(MmBeamOrdersWithOptionalCheckForUnsent(formId, &Prefs, true, &UnsentOrdersCheckSecs))
				{
		   		if(Prefs.current.tableNo < LastTable)
		   		{
						Prefs.current.tableNo++;
						Prefs.current.seatNo = 1;
						OverviewTopRowNum = 0;						//	Always start at Row 0
					}
					MmDisplayTableSeatLabel(frm, Prefs.current.tableNo, MainTablenoLabel, &Prefs,
																MmTablenameDB, MmSeatnameDB);

					MmDisplayTableSeatLabel(frm, Prefs.current.seatNo, MainSeatnoLabel, &Prefs,
																MmTablenameDB, MmSeatnameDB);

					MmMainviewRedraw(frm, ScrollTop, DoRewriteArray);
				}
			}


	   	else if(event->data.ctlEnter.controlID == MainTabledownButton)
	   	{
				//	Check for unsent orders, Login etc. Boolean is checkIfUnsentOrders
				if(MmBeamOrdersWithOptionalCheckForUnsent(formId, &Prefs, true, &UnsentOrdersCheckSecs))
				{
		   		if(Prefs.current.tableNo > 1)			//	1 is lowest number permitted
		   		{
						Prefs.current.tableNo--;
						Prefs.current.seatNo = 1;
						OverviewTopRowNum = 0;				//	Always start at Row 0
					}
					MmDisplayTableSeatLabel(frm, Prefs.current.tableNo, MainTablenoLabel, &Prefs,
																MmTablenameDB, MmSeatnameDB);

					MmDisplayTableSeatLabel(frm, Prefs.current.seatNo, MainSeatnoLabel, &Prefs,
																MmTablenameDB, MmSeatnameDB);

					MmMainviewRedraw(frm, ScrollTop, DoRewriteArray);
				}
			}
			

	   	else if(event->data.ctlEnter.controlID == MainSeatupButton)
	   	{
	   		if(Prefs.current.seatNo < LastSeat)
					Prefs.current.seatNo++;

				MmDisplayTableSeatLabel(frm, Prefs.current.seatNo, MainSeatnoLabel, &Prefs,
															MmTablenameDB, MmSeatnameDB);

				OverviewTopRowNum = 0;							//	Always start at Row 0
				MmMainviewRedraw(frm, ScrollTop, DoRewriteArray);
			}
			


	   	else if(event->data.ctlEnter.controlID == MainSeatdownButton)
	   	{
	   		if(Prefs.current.seatNo > 1)
					Prefs.current.seatNo--;

				MmDisplayTableSeatLabel(frm, Prefs.current.seatNo, MainSeatnoLabel, &Prefs,
															MmTablenameDB, MmSeatnameDB);

				OverviewTopRowNum = 0;							//	Always start at Row 0
				MmMainviewRedraw(frm, ScrollTop, DoRewriteArray);
			}


			handled = true;



			break;
  
  
   	case ctlExitEvent: 	 // A control button was pressed and released OUTSIDE of the bounds.
			
			if(event->data.ctlExit.controlID == MainStaffPopTrigger)
			{
				MmDisplayStaffNameLabel(frm, MmInfoDB, Prefs.staffIDIndex, MainStaffPopTrigger, 
															OverviewPageType, Prefs.loggedIn);
			}
			
			handled = true;
			break;

			


		case ctlRepeatEvent:		// An onscreen scroll button is pressed
	   	if (event->data.ctlRepeat.controlID == MainScrollDownRepeating)
		   {
				MmMainScrollDown(frm, &Prefs, &MainviewTopRowNum, MainviewCanScrollDown);

			   // Repeating controls don't repeat if handled is set true.
			   handled = true;
		   }



		   else if (event->data.ctlRepeat.controlID == MainScrollUpRepeating)
		   {
				MmMainScrollUp(frm, &Prefs, &MainviewTopRowNum);

			   // Repeating controls don't repeat if handled is set true.
			   handled = true;
		   }
	   	

			break;



		case keyDownEvent:		// MemHandle the physical buttons

	   	if(event->data.keyDown.chr == vchrFind)
	   	{
				//	false -> Force Beaming (unless we're not logged in. ie No check for Unsent Orders)
				MmBeamOrdersWithOptionalCheckForUnsent(formId, &Prefs, false, &UnsentOrdersCheckSecs);
				OverviewExpandedStatus = StatusIsContracted;

				handled = true;
			}



	   	if(NavSelectPressed(event))
	   	{
				if(Prefs.userSettings.classicMode)
				{
					//	false -> Force Beaming (unless we're not logged in. ie No check for Unsent Orders)
					MmBeamOrdersWithOptionalCheckForUnsent(formId, &Prefs, false, &UnsentOrdersCheckSecs);
				}
				else
				{
					//	We just got here by selecting ClassicView, not in Classic Mode
					//	If we change seat while in Mainview we'll need to follow suit in Overview
					OldCurrentSeat = Prefs.current.seatNo;
					OverviewPageType = OverviewOrdersOnlyPage;
					FrmGotoForm(OverviewForm);
				}

				handled = true;
			}



			else if(NavDirectionPressed(event, Left))
	   	{
//ShowDebug("Left");
				//	If we change seat while in Mainview we'll need to follow suit in Overview
				OldCurrentSeat = Prefs.current.seatNo;
				OverviewPageType = OverviewOrdersOnlyPage;
				FrmGotoForm(OverviewForm);

				handled = true;
			}



			else if(NavDirectionPressed(event, Right))
	   	{
//ShowDebug("Right");
				//	Swap icons, set Prefs.current.mMenu type
				if(Prefs.current.mMenu == DishesMenu)
				{
					Prefs.current.mMenu = DrinksMenu;
					FrmHideObject(frm, FrmGetObjectIndex(frm, MainMealsBitMap));
					FrmShowObject(frm, FrmGetObjectIndex(frm, MainDrinksBitMap));
				}
				else
				{
					Prefs.current.mMenu = DishesMenu;
					FrmHideObject(frm, FrmGetObjectIndex(frm, MainDrinksBitMap));
					FrmShowObject(frm, FrmGetObjectIndex(frm, MainMealsBitMap));
				}
				MmMainviewRedraw(frm, ScrollTop, DoRewriteArray);

			   handled = true;
			}



			else if(NavDirectionPressed(event, Up)	||
						(event->data.keyDown.chr == vchrPageUp)	)
	   	{
				MmMainScrollUp(frm, &Prefs, &MainviewTopRowNum);

			   handled = true;
	   	}


			else if(NavDirectionPressed(event, Down)	||
						(event->data.keyDown.chr == vchrPageDown)	)
	   	{
				MmMainScrollDown(frm, &Prefs, &MainviewTopRowNum, MainviewCanScrollDown);

			   handled = true;
	   	}



/*
	The 5-Way Navigator buttons (except up and down) generate a vchrNavChange when pressed,
	and the keyCode field tells you both the button states (which ones are pressed now)
	and the recent transition (which were just pressed or released). You can do complicated
	things with this information if you want, but most code will just do the following:

	if((EvtKeydownIsVirtual(eventP) && 
		(eventP->data.keyDown.chr == vchrNavChange) && 
		((eventP->data.keyDown.keyCode & (navBitsAll | navChangeBitsAll)) == navChangeSelect))
	{
		DoMySelectAction();
	}
*/
/*
	This will trigger when the select button is pressed briefly and released. Remember to
	let the vchrNavChange event pass through to the system, so that you don't prevent
	the automatic "go to the launcher" functionality of holding down the select button.
*/
/*
I see vchrHard5 but what about the Pref Set and Pref Get for the Hard5
button?

PrefSetPreference( prefHard5CharAppCreator, creator );
creator = PrefGetPreference( prefHard5CharAppCreator);

prefHard5CharAppCreator??  I don't have that.

When I programatically searched with PrefGet looking for my target, I
couldn't find the SystemPrefencesChoice

extern void PrefSetPreference(SystemPreferencesChoice choice, UInt32 value)
  SYS_TRAP(sysTrapPrefSetPreference);

*/






			break;


/*		case penDownEvent:
	   	if(event->screenX < OverviewCol0Width)
	   	{
				IconSelected = true;
				IconSelectedTime = TimGetTicks();
	   	}
			break;



*/
		case tblEnterEvent:	// An item has been selected in the table.

			{
				Boolean screenRedrawn = false;
						
				PreviousForm = MainForm;
				handled = MmProcessOrder(frm, &TableDisplayRows[MainviewTopRowNum + event->data.tblEnter.row], 
													&DBs, &Prefs, &ListItems[0], &SizeListItems[0],
														&ListItemsStruct[0], &SizeListIndices[0],
															/*&IconSelected, &IconSelectedTime, */&LastItemOrdered,
																&OverviewPageType, &OverviewCheckboxType,
																	&NumOldUnsentOrders, &screenRedrawn		);
							

			}

			//	Repaint the Table title with new defaultServingCourse
			MmShowServingCourse(frm, NoValue16, &DBs, NoValue16, &Prefs);

			handled = true;
			break;
			
 


		case frmTitleEnterEvent:

			listServePtr = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, MainDishesList));

			//	Get the ServingCourse selected
			offset = 0;

			//	Redraw so Title show Server while list is displayed
//			Prefs.current.servingCourseDefault = NoValue16;

//			MmShowServingCourse(frm, MainForm, &DBs, NoValue16, &Prefs);

			if(Prefs.current.mMenu == DishesMenu)
			{
				itemSelected = MmPopupList(listServePtr, ServingCourseDishesList, &offset, 0);
				if(itemSelected != NoValue16)
				{
					itemSelected = ListItemsStruct[itemSelected].recIndex;
					Prefs.current.dishesServingCourseDefault = MmGetServingCourseID(MmServingCourseDB, itemSelected);
				}
			}
			else
			{
				itemSelected = MmPopupList(listServePtr, ServingCourseDrinksList, &offset, 0);
				if(itemSelected != NoValue16)
				{
					itemSelected = ListItemsStruct[itemSelected].recIndex;
					Prefs.current.drinksServingCourseDefault = MmGetServingCourseID(MmServingCourseDB, itemSelected);
				}
			}

			//	Redraw with the new item
			MmShowServingCourse(frm, MainForm, &DBs, NoValue16, &Prefs);

			handled = true;
			break;



		case menuEvent:		// A menu item was selected.
			
			MmHandleMenuEvent(frm, event);

			handled = true;
			break;



		case frmOpenEvent:	// P3. The form was told to open.
  			// P7. Initialize the main form.

			MmMainviewInit();

			frm = FrmGetActiveForm();
			
			// Get Prefs.current.tableNo and Prefs.current.seatNo and display
			if((Prefs.current.tableNo < 1) || (Prefs.current.tableNo > CashTable))
				Prefs.current.tableNo = 1;

			if((Prefs.current.seatNo < 1) || (Prefs.current.seatNo > LastSeat))
				Prefs.current.seatNo = 1;
			MmDisplayTableSeatLabel(frm, Prefs.current.tableNo, MainTablenoLabel, &Prefs,
															MmTablenameDB, MmSeatnameDB);
			MmDisplayTableSeatLabel(frm, Prefs.current.seatNo, MainSeatnoLabel, &Prefs,
															MmTablenameDB, MmSeatnameDB);

			//	If we're in the Drinks Menu, Swap icons
			if(Prefs.current.mMenu == DrinksMenu)
			{
				FrmHideObject(frm, FrmGetObjectIndex(frm, MainMealsBitMap));
				FrmShowObject(frm, FrmGetObjectIndex(frm, MainDrinksBitMap));
			}

			if(InHappyHour)
				FrmShowObject(frm, FrmGetObjectIndex(frm, MainHappyHourButton));
			else
				FrmHideObject(frm, FrmGetObjectIndex(frm, MainHappyHourButton));


			MmShowServingCourse(frm, MainForm, &DBs, NoValue16, &Prefs);


			PreviousForm = MainForm;

			handled = true;
			break;
			

	}
	return(handled);
}


/***********************************************************************
 *
 * FUNCTION:    ApplicationHandleEvent
 *
 * DESCRIPTION: P3. This routine loads a form resource and sets the
 *              event handler for the form.
 *
 * PARAMETERS:  event - a pointer to an EventType structure
 *
 * RETURNED:    True if the event has been handled and should not be
 *	             passed to a higher level handler.
 *
 ***********************************************************************/
static Boolean MmApplicationHandleEvent(EventPtr event)
{
	FormPtr			frm;
	Int16				formId;
	Boolean			handled = false;

	if(event->eType == frmLoadEvent)
	{
		// Load the form resource specified in the event then activate the form.
		formId = event->data.frmLoad.formID;
		frm = FrmInitForm(formId);
		FrmSetActiveForm(frm);

		// Set the event handler for the form.  The handler of the currently 
		// active form is called by FrmDispatchEvent each time it receives an event.
		switch (formId)
		{
			case MainForm:
				FrmSetEventHandler(frm, MmMainviewHandleEvent);
				break;
		
			case OverviewForm:
				FrmSetEventHandler(frm, MmOverviewHandleEvent);
				break;

			case RegisterForm:
				FrmSetEventHandler(frm, MmRegisterHandleEvent);
				break;
		
//			case SplitBillForm:
//				FrmSetEventHandler(frm, MmSplitBillHandleEvent);
//				break;
		
			case TableForm:
				FrmSetEventHandler(frm, MmTableOrSeatHandleEvent);
				break;
		
			case SeatForm:
				FrmSetEventHandler(frm, MmTableOrSeatHandleEvent);
				break;
		
			case DetailsForm:
				FrmSetEventHandler(frm, MmDetailsFormHandleEvent);
				break;
				
			case CallAwayForm:
				FrmSetEventHandler(frm, MmCallAwayHandleEvent);
				break;

			case UserSettingsForm:
				FrmSetEventHandler(frm, MmUserSettingsHandleEvent);
				break;

			case NumberEntryFieldForm:
				FrmSetEventHandler(frm, MmNumberEntryFieldEvent);
				break;

			case KeypadForm:
				FrmSetEventHandler(frm, MmKeypadHandleEvent);
				break;

			case EditLineForm:
				FrmSetEventHandler(frm, MmEditLineFormHandleEvent);
				break;
		
			case BeamingForm:
				FrmSetEventHandler(frm, MmBeamingFormHandleEvent);
				break;

			case EnterPINForm:
				FrmSetEventHandler(frm, MmEnterPINFormHandleEvent);
				break;

			case SystemInfoForm:
				FrmSetEventHandler(frm, MmSystemInfoFormHandleEvent);
				break;

			case ScreenLockForm:
				FrmSetEventHandler(frm, MmScreenLockFormHandleEvent);
				break;

			case DebugDataForm:
				FrmSetEventHandler(frm, MmDebugDataFormHandleEvent);
				break;
		}
		handled = true;
	}
	
	return handled;
}


/***********************************************************************
 *
 * FUNCTION:		TimeUntilNextSerialRead
 *
 * DESCRIPTION:	Returns time in ticks until next serial read should occur
 *						
 *						Returns evtWaitForever unless SerialOpened != 0
 *
 * PARAMETERS:		None.
 *
 * RETURNED:		Returns time in ticks or evtWaitForever.
 *
 ***********************************************************************/
/*static Int32 MmTimeUntilNextSerialEvent(void)
{
	if(!SerialOpened)
		return (evtWaitForever);
	else
	{
		Int32 timeRemaining;
		
		timeRemaining = NextSerialEventTime - TimGetTicks();
		if(timeRemaining < 0)
			timeRemaining = 0;
		return timeRemaining;
	}
}
*/



/***********************************************************************
 *
 * FUNCTION:		MmHandleNotification
 *
 * DESCRIPTION:	
 *						
 *
 * PARAMETERS:		None.
 *
 * RETURNED:		
 *
 ***********************************************************************/
static void MmHandleNotification(SysNotifyParamType *notifyPtr)
{
//	UInt32 timeAsleep;

/*	if(notifyPtr->notifyType == sysNotifySleepNotifyEvent)
	{

		FtrSet(appFileCreator, sysFtrNumROMVersion, 0x05003000);//0xMMmfsbbb

//		FtrSet(appFileCreator, FtrBtPortID, 0);
	}
	else*/ if(notifyPtr->notifyType == sysNotifyLateWakeupEvent)
	{
//		MmRefreshAutoLogout(true, 0);						//	Update the Prefs.autoLogoutSecs global
//		FtrGet(appFileCreator, 1, &timeAsleep);
//		FtrSet(appFileCreator, FtrBtPortID, 1);
	}
}


/*
Sending Notifications 
It is easy to broadcast a notification to inform other applications (or your own application) that something noteworthy has occurred. If no applications have registered for the notification that you are sending, your call to the SysNotifyBroadcast function will return quickly, though it does obviously take a little time for the system to make that determination.

You can send a notification immediately by using SysNotifyBroadcast. In this case, you'll wait until every recipient processes the notification. 
Or else you can send a deferred notification by using SysNotifyBroadcastDeferred. In this case, the notification will be delivered later (currently, "later" means the next time EvtGetEvent is called). 
The following code shows how to send a notification.

   // must be a registered creator ID; use your
    // application's or else register a new one
    // at http://www.palmos.com/dev/creatorid/
	#define myInterestingEvent 'MyEv'

 
   SysNotifyParamType notify;
 
   notify.notifyType     = myInterestingEvent;
   // say who is sending the event (i.e. you.)
   notify.broadcaster    = appFileCreator;
   notify.handled        = false;
   // or whatever additional information you need to send
   notify.notifyDetailsP = NULL;
 
	// or possibly use SysNotifyBroadcastDeferred
   SysNotifyBroadcast(notify);   
If you call SysNotifyBroadcastDeferred and have a structure that notifyDetailsP points to, you can simplify your memory management by choosing to have SysNotifyBroadcastDeferred make a copy of your structure so that you don't need to worry about knowing when to free it. To do this, pass a nonzero value as the second parameter to SysNotifyBroadcastDeferred. Then it will allocate a new buffer with the specified size, copy that many bytes from wherever notifyDetailsP points to, and change the notifyDetailsP element to point to its own buffer. After all recipients have handled the notification, it will free that buffer. Using this method, you can immediately free the buffer you specified in notifyDetailsP (or even have it point to a local variable) without worrying about making sure that the pointer stays valid until everyone has been notified, or leaking memory.
*/




/***********************************************************************
 *
 * FUNCTION:		RegisterForNotifications
 *
 * DESCRIPTION:	
 *						
 *
 * PARAMETERS:		None.
 *
 * RETURNED:		
 *
 ***********************************************************************/
static void MmRegisterForNotifications()
{
	UInt16 cardNo;
	LocalID dbID;

	if(0 != SysCurAppDatabase(&cardNo, &dbID))
		return; // shouldn't ever fail, but just in case.

	// Tell the system we want to know when the device sleeps and wakes.
	//	With this RegisterForNotification function added, the HandleNotification
	//	function will be called upon every sleep and wake. Note that the fourth
	//	parameter to the SysNotifyRegister function allows you to pass a pointer
	//	to a callback function that will handle the notification. Passing NULL
	//	as the pointer to the callback function causes the sysAppLaunchCmdNotify
	//	launch code to be sent to your application (we had you add code to your
	//	PilotMain function to handle this above). In general, it is simpler to
	//	use launch codes, but if you know you are the active application or are
	//	writing a shared library you may want to use a callback function.

	SysNotifyRegister(cardNo, dbID, sysNotifySleepNotifyEvent, NULL,
																sysNotifyNormalPriority, 0);
	SysNotifyRegister(cardNo, dbID, sysNotifyLateWakeupEvent, NULL,
																sysNotifyNormalPriority, 0);
}



/***********************************************************************
 *
 * FUNCTION:		EventLoop
 *
 * DESCRIPTION:	A simple loop that obtains events from the Event
 *						Manager and passes them on to various applications and
 *						system event handlers before passing them on to
 *						FrmDispatchEvent for default processing.
 *
 * PARAMETERS:		None.
 *
 * RETURNED:		Nothing.
 *
 ***********************************************************************/
static void MmEventLoop(void)
{
	EventType		event;
	UInt16			error;
	
	do
	{
//		EvtGetEvent(&event, SysTicksPerSecond());

		// Get the next available event or generate nilEvent every second 
		//	(or 10ms during return from ScreenLock).
		if(	ScreenLockStruct.action == unlockOnNilEvent
			||	
				ScreenLockStruct.action == lockOnNilEvent)
		{
			EvtGetEvent(&event, 10);
		}
		else
		{
			EvtGetEvent(&event, 50);	//	SysTicksPerSecond()/2
		}
		
		// Intercept the hard keys to prevent them from switching apps
		if(event.eType == keyDownEvent)
		{
			// Swallow events notifying us of key presses.
			//	Allow Power events to get through
			if(event.data.keyDown.chr == vchrLateWakeup)
			{
//				MmRefreshAutoLogout(true, 0);	//	Caution!!!  This breaks the ScreenLock
			}
			else if(	event.data.keyDown.chr == vchrPowerOff	||		//autoOffChr or hardPowerChr)
						event.data.keyDown.chr == vchrAutoOff	||		//autoOffChr
						event.data.keyDown.chr == vchrHardPower	)	//hardPowerChr)
			{

				if(ScreenLockStruct.action == isActive)
				{
					ScreenLockStruct.action = isActivePowerOff;	//	Must not be isActive when it wakes up
				}

				MmRefreshAutoLogout(true, 0);				//	Update the Prefs.autoLogoutSecs global
			}
			//	Let MenuMate look at it, but don't let the OS see it
			else if(	ScreenLockStruct.action == isActive	||
						event.data.keyDown.chr == vchrHard1	||
						event.data.keyDown.chr == vchrHard2	||
						event.data.keyDown.chr == vchrHard3	||
						event.data.keyDown.chr == vchrHard4	||
						event.data.keyDown.chr == vchrFind		)
			{
				MmRefreshAutoLogout(true, 0);				//	Update the Prefs.autoLogoutSecs global
				FrmDispatchEvent(&event);
				continue;
			}
		}

		// Give the system a chance to handle the event.
		if (! SysHandleEvent(&event))

			// Give the menu bar a chance to update and handle the event.	
			if (! MenuHandleEvent(0, &event, &error))
			{
				// Give the application a chance to handle the event.
				if (! MmApplicationHandleEvent(&event))

					// Let the form object provide default handling of the event.
					FrmDispatchEvent(&event);
			}
	}while (event.eType != appStopEvent);
}
/*
This code will mask out the hard button and silked buttons. Unfortunately 
it doesn't work if the user presses one of these button while in a dialog. 
You can easily change this code so instead of masking out the keys, it checks 
to see if they are hit and runs some function. The key (no pun intended) is 
to never let the key presses get to the normal event loop code 
(SysHandleEvent), because then the app will close.

//event loop 
KeySetMask( ~(keyBitHard1 | keyBitHard2 | keyBitHard3 | keyBitHard4) ); 
do
{ 
	EvtGetEvent(&event, evtWaitForever); 
	if(event.eType == keyDownEvent) 
		if(event.data.keyDown.chr == 264 || event.data.keyDown.chr == 266 || 
						event.data.keyDown.chr == 267) 
		continue; 
	if(! SysHandleEvent(&event)) 
		if(! MenuHandleEvent(NULL, &event, &error)) 
			if(! AppHandleEvent(&event)) 
				FrmDispatchEvent(&event); 
}while (event.eType != appStopEvent); 

KeySetMask(keyBitsAll);

*/



/***********************************************************************
 *
 * FUNCTION:		PilotMain
 *
 * DESCRIPTION:	This function is the equivalent of a main() function
 *						under standard ÒCÓ.  It is called by the Emulator to begin
 *						execution of this application.
 *
 * PARAMETERS:		cmd - command specifying how to launch the application.
 *						cmdPBP - parameter block for the command.
 *						launchFlags - flags used to configure the launch.			
 *
 * RETURNED:		Any applicable error codes.
 *
 ***********************************************************************/
UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
	Err					error;
	LocalID				dbID;
	UInt16				cardNo;
	DmSearchStateType	theSearchState;
	
	error = MmRomVersionCompatible(launchFlags);
	if(error)
		return error;



	switch(cmd)
	{
		case sysAppLaunchCmdNormalLaunch:

			if(! MmStartApplication())
			{
				MmEventLoop();
				
				MmStopApplication();
			}
			break;
		
//	   case sysAppLaunchCmdSystemReset:
//	   case sysAppLaunchCmdSyncNotify:  // sent when our app is installed
//	      MmRegisterForNotifications();
//	      break;
	 
	   case sysAppLaunchCmdNotify:
	      MmHandleNotification((SysNotifyParamType *)cmdPBP);
	      break;


			// P17. The database has been created but there isn't an app info block.
			// Either the HotSync app or another application which uses this application's
			// data may request that the application info block be created by this app
			// (because it's the creator of the database).
		case sysAppLaunchCmdInitDatabase:
			MmInitAppInfo (((SysAppLaunchCmdInitDatabaseType*)cmdPBP)->dbP);
	      break;




		case sysAppLaunchCmdSystemReset:
			// Soft Reset
			// Grab the id of the application
			DmGetNextDatabaseByTypeCreator(true, &theSearchState,
										sysFileTApplication, appFileCreator, true, &cardNo, &dbID);
			// Set Alarm for 5 seconds time
			AlmSetAlarm(cardNo,dbID,1,5,true);
			break;

		case sysAppLaunchCmdSyncNotify:
	      MmRegisterForNotifications();

			// HotSync
			// Grab the id of the application
			DmGetNextDatabaseByTypeCreator(true, &theSearchState,
										sysFileTApplication, appFileCreator, true, &cardNo, &dbID);
			// Set Alarm for 5 seconds time
			AlmSetAlarm(cardNo,dbID,1,5,true);
			break;

		case sysAppLaunchCmdDisplayAlarm:
			// Alarm from above was triggered
			// Launch the new app.
			DmGetNextDatabaseByTypeCreator(true, &theSearchState,
										sysFileTApplication, appFileCreator, true, &cardNo, &dbID);
			SysUIAppSwitch(cardNo, dbID, sysAppLaunchCmdNormalLaunch, NULL);
			break;








			// P14. The system find command uses this to search this application.
/*		case sysAppLaunchCmdFind:
			Search ((FindParamsPtr)cmdPBP);
	      break;
	
	
			// P14. If the user wants to goto a record listed in the find dialog
			// it sends the following notification
		case sysAppLaunchCmdGoTo:
			// Check if new global variables are set up.  This occurs when the 
			// system intends for the app to start.
			if (launchFlags & sysAppLaunchFlagNewGlobals) 
			{
				// This occurs when the user wants to see a record in this
				// application's database.  The app must be started and 
				// a view to display the data must be opened.
				if (!StartApplication ())
				{
					GoToRecord ((GoToParamsPtr) cmdPBP, true);
					
					EventLoop ();
					StopApplication ();
				}
			}
			else
			{
				// The app was already running.  Open a form to display the record.
				GoToRecord ((GoToParamsPtr) cmdPBP, false);
			}
			break;

			// P14. Launch code sent to running app to save all data.
			// This is neccessary before a find for some applications because
			// some apps do not edit in place and the user might search for
			// data not saved.  Or saving a record might reorder the database.
			// Saving before a find avoids finding a record, saving, resorting,
			// and then displaying a record moved to the position of the found
			// record.
		case sysAppLaunchCmdSaveData:
			FrmSaveAllForms ();
			break;
*/	

	}

	return errNone;
}

/*	//	Go to end of database
//	recIndex = DmNumRecords(MmOrdersDB);

	// Find first record for this Table/Seat
	recIndex = DmFindSortPosition(MmOrdersDB, newRecordPtr, NULL,
								(DmComparF *)MmCompareTableSeat, 0) - 1;
	//	Have a look at the record
	recHandle = DmQueryRecord(MmOrdersDB, recIndex)
	if(recHandle)
	{
		recPtr = MemHandleLock(recHandle);
		//	Check if a marker record exists. If not, create one.
		if(recPtr->courseNumber != 0 && recPtr->itemNumber != 0)
		{
			MemPtrUnlock(recPtr);
			DmNewRecord(MmOrdersDB, )
			DmQuickSort(
		}
	}

	Sample out of DateDB.C
	newIndex = ApptFindSortPosition (dbP, dst);
	DmMoveRecord (dbP, *index, newIndex);
	if (newIndex > *index) newIndex--;
	*index = newIndex;						// return new position
*/
