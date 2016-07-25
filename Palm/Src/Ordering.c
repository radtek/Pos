#define EXTERNAL 1

#include "MenuMate.h"



/***********************************************************************
 *
 * FUNCTION:   	MmProcessNewOrder
 *
 * DESCRIPTION:	
 *						
 *
 * PARAMETERS:  
 *
 * RETURNED:   	nothing
 *
 ***********************************************************************/
static Boolean MmProcessNewOrder(FormPtr frm, DBTypePtr dBs, MmPreferenceTypePtr prefs,
									Char* *listItems, Char* *sizeListItems,
										ListStructTypePtr listItemsStruct, UInt16* sizeListIndices,
											UInt16* lastItemOrdered, UInt16 recMenuIndex, UInt8 enabled, 
												UInt16 servingCourseID, UInt16 *servingCourseDefault,
													Boolean showServingCourses, Boolean showCourses, 
														Boolean isSetMenuItem, Boolean *screenRedrawn		)
{
	Boolean					handled = false;
	
	TablePtr					tablePtr;
	ListPtr					listPtr;
	ListPtr					listSizePtr;

	UInt16					tableType;
	UInt16					listType;

	UInt16					offset;
	UInt16					itemSelected;

	DmOpenRef				menuDB;
	DmOpenRef				menuSizesDB;
	DmOpenRef				optionsDB;
	DmOpenRef				forcedSidesDB;

	MemHandle				recMenuHandle;
	MenuTypePtr				recMenuPtr;
	Boolean					orderGood;
	
	RectangleType			bounds;




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

	if(FrmGetFormId(frm) == MainForm)
	{
		tableType	= MainOrderTable;
		listType		= MainDishesList;
	}
	else
	{
		tableType	= OverviewOverviewTable;
		listType		= OverviewDishesList;
	}

	tablePtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, tableType));
	listPtr = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, listType));
	MmLstSetFont(listPtr, prefs);
	FrmGetObjectBounds(frm, FrmGetObjectIndex(frm, listType), &bounds);






	//	If this is a Course offer the Dishes for selection
	orderGood = true;



	//	Only necessary if this was ordered through a Serving Course
	if(showCourses)
	{
		//	Get the Course selected
		itemSelected = MmPopupList(listPtr, CoursesList, &offset, 0);

		if(itemSelected != NoValue16)
		{
			recMenuHandle = DmQueryRecord (menuDB, listItemsStruct[itemSelected].recIndex);

			if(recMenuHandle)
			{
				recMenuPtr = MemHandleLock(recMenuHandle);

				if((recMenuPtr->enabled & StatusAsEnabled) != StatusAsEnabled)
				{
					FrmAlert(WarnMenuDishDisabledAlert);//	Course is not enabled
					orderGood = false;
				}
				else
				{
					prefs->current.course = itemSelected + 1;	//	itemSelected is zero based
					prefs->curOrder.courseNumber	= prefs->current.course;
					enabled = recMenuPtr->enabled;
				}
				MemPtrUnlock(recMenuPtr);
			}
			else
			{
				orderGood = false;
			}
		}
		else
		{
			orderGood = false;
		}
	}	





	if(orderGood)
	{
		if(!isSetMenuItem)
		{
			recMenuIndex = MmPopupList(listPtr, ItemsList, &offset, 0);
		}

		if(recMenuIndex == NoValue16)		//	If invalid selection made
		{
			orderGood = false;
		}
		else
		{
			//	Must be either a Dish or Drink
			recMenuHandle = DmQueryRecord (menuDB, recMenuIndex);

			// If the record was found, correctly set up the Prefs.curOrder, etc.
			if(recMenuHandle)
			{
				recMenuPtr = MemHandleLock(recMenuHandle);

				if(!(recMenuPtr->enabled & StatusAsEnabled))
				{
					FrmAlert(WarnMenuDishDisabledAlert);//	Dish/Drink item is not enabled
					orderGood = false;
				}
				
				else	//	This Course is enabled, so do it
				{
					UInt16 orderSizeIndex;
				
					if(!showServingCourses)
					{
						//	If the servingCourseDefault is not selectable, eg NoServingCourse
						//	then just use it. 
						if(recMenuPtr->servingCourseDefault != NoValue16)
						{
							UInt16		index;
							UInt32		servingCourseColor;
							Boolean		selectable;
							
							MmGetServingCourseIndex(dBs->mmServingCourseDB, recMenuPtr->servingCourseDefault,
																&index, &servingCourseColor, &selectable);
							
							if(!selectable)
								servingCourseID = recMenuPtr->servingCourseDefault;
						}
						else
						{
							*servingCourseDefault = servingCourseID;
						}
					}

					else if(isSetMenuItem)
					{
						//	If there is a servingCourseDefault then just use it. 
						if(recMenuPtr->servingCourseDefault != NoValue16)
						{
							servingCourseID = recMenuPtr->servingCourseDefault;
							showServingCourses = false;
						}
						else
						{
							servingCourseID = NoValue16;
							*servingCourseDefault = NoValue16;
							showServingCourses = true;
						}
					}

					prefs->curOrder.servingCourseID = servingCourseID;

					prefs->curOrder.itemNumber = recMenuPtr->itemNumber;
					prefs->curOrder.itemInCourse = recMenuPtr->itemInCourse;
					prefs->curOrder.timeTaken = TimGetSeconds();	//	Record the time stamp
					prefs->curOrder.menuIndex[0].index = recMenuIndex;
					prefs->current.recMenuIndex = recMenuIndex;

					if(!isSetMenuItem)
					{
						prefs->curOrder.setMenuMask = 0;
					}
				

					//	Set size defaults
					prefs->curOrder.menuIndex[0].size = recMenuPtr->sizes[0].size;
					prefs->curOrder.size = recMenuPtr->sizes[0].size;

					//	Ask for the size if applicable
					listSizePtr = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, listType));

					if(isSetMenuItem)
					{
						if(!MmAskUserForSize(dBs, prefs, SizeForNewSetMenuItem, recMenuPtr, 
													listSizePtr, sizeListIndices, &orderSizeIndex))
						{
							orderGood = false;
						}
					}
					else
					{
						if(!MmAskUserForSize(dBs, prefs, SizeForNewOrder, recMenuPtr, 
													listSizePtr, sizeListIndices, &orderSizeIndex))
						{
							orderGood = false;
						}
					}


					if(orderGood)
					{
						//	Get and apply any Forced Sides and Forced Options
						prefs->current.course = prefs->curOrder.courseNumber;

						if(!(prefs->curOrder.setMenuMask & SetMenuKey))
						{
							//	Trim Options for this course to fit list width
							MmCourseOptionsTrimList(dBs, prefs, bounds);

							if(MmGetForcedOptions(dBs, prefs, listPtr,
															&listItems[0], &sizeListItems[0], 
																&listItemsStruct[0], &sizeListIndices[0]))
							{
								orderGood = false;
							}
							else
							{
								if(MmGetForcedSides(dBs, prefs, listPtr,
															&listItems[0], &sizeListItems[0], 
																&listItemsStruct[0], &sizeListIndices[0]))
								{
									orderGood = false;
								}
							}
						}
					}
				}
				MemPtrUnlock(recMenuPtr);
			}
		}

		//	SetMenuKey items mustn't be assigned a value that would make it look like it was already sent
		if((prefs->curOrder.setMenuMask & SetMenuKey)	 &&
			 (	prefs->current.setMenuGroup == SetMenuGroupMaxValue || 
			 	prefs->current.setMenuGroup < SetMenuGroupMinValue		)	)
		{
			orderGood = false;
			FrmAlert(WarnMaxSetMenuGroupsAlert);
//ShowDebug("SetMenuGroup = %x\nPlease write this down and report to IQWorks Ltd.", prefs->current.setMenuGroup);
		}




		if(orderGood)
		{
			//	If this new order is a SetMenu key item, set the setMenuGroup number
			//	and force the serving course to No Serving Course
			if(prefs->curOrder.setMenuMask & SetMenuKey)
			{
				prefs->curOrder.setMenuGroup = prefs->current.setMenuGroup;
				prefs->current.setMenuGroup++;	//	Ready for next one

				servingCourseID = IDForNoServingCourse;		//	Hard-wired at POS
				prefs->curOrder.servingCourseID = IDForNoServingCourse;
			}


			//	Get Serving Course if there is no default, and Prefs.rememberLastServCourse == false
			else if(showServingCourses)
			{
				//	Redraw so Title shows Server while list is displayed
				*servingCourseDefault = NoValue16;

				MmShowServingCourse(frm, NoValue16, dBs, NoValue16, prefs);

				//	Get the Serving Course selected
				offset = 0;
				itemSelected = MmPopupList(listPtr, ServingCourseAllList, &offset, 0);
				
				if(itemSelected != NoValue16)
				{
					itemSelected = listItemsStruct[itemSelected].recIndex;
					servingCourseID = MmGetServingCourseID(dBs->mmServingCourseDB, itemSelected);
					prefs->curOrder.servingCourseID = servingCourseID;
				}
				else
				{
					orderGood = false;
				}
			}	
			*servingCourseDefault = servingCourseID;
		}









		if(orderGood)
		{
			//	Make sure that the new record is created right at the end
			prefs->curOrder.itemCounter = NoValue16;



			MmCreateOrdersRecord(prefs, dBs, lastItemOrdered);	//	Now create the new record

			prefs->hLStatus.HLSendOrdersStatus = HLStatusToSend;//	Flag Prefs.hLStatus as 'To send'

			prefs->current.setMenuMask = 0;

			if(FrmGetFormId(frm) == MainForm)
			{
				MmMainviewRedraw(frm, CanScroll, DoRewriteArray);
			}
			else
			{
				MmOverviewRedraw(frm, CanScroll, DoRewriteArray);
				*screenRedrawn = true;
			}


			if(prefs->current.mMenu == DishesMenu && prefs->userSettings.autoDetails)
			{
				FrmGotoForm(DetailsForm);		//	Automatically bring up the Details
			}
		}
	}
	handled = true;

	return true;
}
 






/***********************************************************************
 *
 * FUNCTION:   	MmProcessExistingOrder
 *
 * DESCRIPTION:	
 *						
 *
 * PARAMETERS:  
 *
 * RETURNED:   	nothing
 *
 ***********************************************************************/
static Boolean MmProcessExistingOrder(FormPtr frm, 	//event->data.tblEnter.row
								DBTypePtr dBs, MmPreferenceTypePtr prefs,
									Char* *listItems, Char* *sizeListItems,
										ListStructTypePtr listItemsStruct, UInt16* sizeListIndices,
											UInt16 *lastItemOrdered,
												UInt16 recIndex, UInt16 recMenuIndex,
													UInt32 orderSetMenuMask, UInt16 setMenuGroup, //UInt16 setMenuDisplayID,
														UInt16 itemNumber, UInt8 itemInCourse, 
															UInt16 recIndexSave, UInt16 *servingCourseDefault, 
																Boolean *screenRedrawn
																																)
{
	Boolean					handled = false;
	
	TablePtr					tablePtr;
	ListPtr					listPtr;
//	ListPtr					listSizePtr;

	UInt16					tableType;
	UInt16					listType;

	UInt16					offset;
//	UInt16					itemSelected;
	UInt16					currentRecordIndex;

	DmOpenRef				menuDB;
	DmOpenRef				menuSizesDB;
	DmOpenRef				optionsDB;
	DmOpenRef				forcedSidesDB;

	MemHandle				recHandle;

	OrderTypePtr			recPtr;
	MenuTypePtr				recMenuPtr;
//	SizeTypePtr				recSizePtr;

//	UInt16					numSizeTypes;
	UInt16					status;
	UInt16					servingCourseID = 0;
	
//	Boolean					containsSetMenuKey;
	Boolean					orderGood;
	
	RectangleType			bounds;




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

	if(FrmGetFormId(frm) == MainForm)
	{
		tableType	= MainOrderTable;
		listType		= MainDishesList;
	}
	else
	{
		tableType	= OverviewOverviewTable;
		listType		= OverviewDishesList;
	}


	tablePtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, tableType));
	
	listPtr = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, listType));
	MmLstSetFont(listPtr, prefs);
	FrmGetObjectBounds(frm, FrmGetObjectIndex(frm, listType), &bounds);







	//	If its not a Course it must be a Dish so offer Details facilities
	//	Might be a SetMenu Key item
	//	If its a SetMenu Key item, display Slaves. We stored orderSetMenuMask above
	if(orderSetMenuMask & SetMenuKey)
	{
		prefs->current.setMenuMask = orderSetMenuMask;

		recMenuIndex = MmPopupList(listPtr, SetMenuItemsList, &offset, 0);

		if(recMenuIndex != NoValue16)		//	If valid selection made
		{
			//	If its the 'Delete' item, do that if its not yet sent
			if(recMenuIndex == 0)
			{
				recHandle = DmQueryRecord(dBs->mmOrdersDB, recIndex);

				if(recHandle)
				{
					recPtr = MemHandleLock(recHandle);
					prefs->curOrder.itemCounter = recPtr->itemCounter;
					status = recPtr->status;
					MemPtrUnlock(recPtr);

					if((status & StatusAsSent) ||
						(status & StatusAsCreditDish)			)
					{
						FrmAlert(WarnCantAlterSentOrderAlert);
					}
					else
					{
						//	Get confirmation, then delete if OK
						if(FrmAlert(ConfDeleteSetMenuMasterAlert) == ConfDeleteSetMenuMasterOK)
						{
							currentRecordIndex = prefs->current.recIndex;	//	Preserve
							prefs->curOrder.itemNumber = itemNumber;
							prefs->curOrder.itemInCourse = itemInCourse;
							MmFindCurrentRecordIndex(&recIndex, FindOnly, FindOnly);

							//	Scan through the orders looking for any attached slaves
							if(MmDeleteSetMenuKeyAndOrders(dBs->mmOrdersDB, prefs))
								ErrFatalDisplayIf(true, "Could not Delete Record");

							prefs->current.recIndex = currentRecordIndex;	//	Restore

							prefs->current.setMenuMask = 0;


							if(FrmGetFormId(frm) == MainForm)
							{
								MmMainviewRedraw(frm, ScrollTop, DoRewriteArray);
							}
							else
							{
								MmOverviewRedraw(frm, ScrollTop, DoRewriteArray);
								*screenRedrawn = true;
							}
						}
					}
				}
			}

			//	Its not the 'Delete' item, so a SetMenu item has been selected from the list.
			else
			{
				recHandle = DmQueryRecord(menuDB, recMenuIndex);

				// If record is found, set up Prefs.curOrder
				if(recHandle)
				{
//					UInt16		showServingCourses;
					UInt16		sideIndex = 0;
					
					
					orderGood = true;
					


					recMenuPtr = MemHandleLock(recHandle);
					

					//	Make sure that the new record is created right at the end
					prefs->curOrder.itemCounter = NoValue16;

					prefs->curOrder.courseNumber = recMenuPtr->courseNumber;
					prefs->current.course = recMenuPtr->courseNumber;

					prefs->curOrder.setMenuMask = orderSetMenuMask & ~SetMenuKey;
					prefs->curOrder.setMenuGroup = setMenuGroup;
					prefs->curOrder.price = NoValue32Signed;//	This is a set menu item


//					if(recMenuPtr->servingCourseDefault == NoValue16)
//						showServingCourses = true;
//					else
//						showServingCourses = false;
					

/*					MmProcessNewOrder(frm, dBs, prefs,
									Char* *listItems, Char* *sizeListItems,
										ListStructTypePtr listItemsStruct, UInt16* sizeListIndices,
											UInt16* lastItemOrdered, UInt16 recMenuIndex, UInt8 enabled, 
												UInt16 servingCourseID, UInt16 *servingCourseDefault,
													Boolean showServingCourses, Boolean showCourses, 
														Boolean isSetMenuItem, Boolean *screenRedrawn)
*/
					MmProcessNewOrder(frm, dBs, prefs, 
											listItems, sizeListItems, listItemsStruct, sizeListIndices,
												lastItemOrdered, recMenuIndex, recMenuPtr->enabled, 
													servingCourseID, servingCourseDefault,
														true, false, true, screenRedrawn);



					MemPtrUnlock(recMenuPtr);
				}
			}
		}
	}
	
	//	It must be a normal Dish, so offer normal Details facilities
	else
	{
		prefs->current.recIndex = recIndexSave;
//		prefs->current.recMenuIndex = recMenuIndex;
		prefs->current.setMenuMask = orderSetMenuMask;

		MmCopyOrderToCurrentRecord(prefs, dBs);

		FrmGotoForm(DetailsForm);
	}
	return true;
}			
 



/***********************************************************************
 *
 * FUNCTION:   	MmProcessOrder
 *
 * DESCRIPTION:	
 *						
 *
 * PARAMETERS:  
 *
 * RETURNED:   	nothing
 *
 ***********************************************************************/
Boolean MmProcessOrder(FormPtr frm, TableDisplayType *displayRowsPtr, 
								DBTypePtr dBs, MmPreferenceTypePtr prefs,

									Char* *listItems, Char* *sizeListItems,
										ListStructTypePtr listItemsStruct, UInt16* sizeListIndices,

										UInt16 *lastItemOrdered,

											UInt16 *overviewPageType, UInt16 *overviewCheckboxes,
												Int16 *numOldUnsentOrders, Boolean *screenRedrawn
																																)
{											//	prefs->rememberLastServCourse
	Boolean					handled = false;
	
	TablePtr					tablePtr;

	UInt16					tableType;
	UInt16					listType;

	UInt16					itemNumber = 0;
	UInt8						itemInCourse;
	UInt8						previousCourse;
	

	DmOpenRef				menuDB;
	DmOpenRef				menuSizesDB;
	DmOpenRef				optionsDB;
	DmOpenRef				forcedSidesDB;

	UInt16					recMenuIndex;
	MemHandle				recMenuHandle;
	MenuTypePtr				recMenuPtr;

	UInt16					recIndex;
	UInt16					recIndexSave;
	MemHandle				recHandle;
	OrderTypePtr			recPtr;

	UInt16					setMenuGroup;			//	Derived from the current order
	UInt32					orderSetMenuMask;		//	Derived from the current order
	UInt8						enabled;

	UInt16					servingCourseDefault;
	UInt16					numServingCourses = 0;
	UInt16					servingCourseID = NoValue16;
	UInt16					recServingCourseIndex;
	UInt32					color;
	Boolean					selectable;

	UInt8						mMenu = DishesMenu;
	
	Boolean					showCourses	= false;
	Boolean					showServingCourses	= false;


	
	mMenu = prefs->current.mMenu;					//	Preserve

	if(FrmGetFormId(frm) == OverviewForm)
	{
		tableType	= OverviewOverviewTable;
		listType		= OverviewDishesList;

		tablePtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, tableType));

		//	If its a Serving Course picker, check which menu to order from
		if(displayRowsPtr->dishIndex == 0)
		{
			//	Must be a Serving Course, so offer courses for selection
			servingCourseID = displayRowsPtr->recIndex;
			mMenu = displayRowsPtr->menuType;

			MmGetServingCourseIndex(dBs->mmServingCourseDB, servingCourseID,
													&recServingCourseIndex, &color, &selectable);
			
			showCourses	= true;
			prefs->current.mMenu = mMenu;
		}
	}
	
	
	else	//	MainForm
	{
		tableType	= MainOrderTable;
		listType		= MainDishesList;

		numServingCourses = MmGetFirstServingCourseForMenu(dBs->mmServingCourseDB, mMenu, 
													&servingCourseID, &recServingCourseIndex, &color);
		
		tablePtr		= FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, tableType));
	}




	switch(mMenu)
	{
		case DrinksMenu:	//	!! Drinks !!
			menuDB					= dBs->mmDrinksDB;
			menuSizesDB				= dBs->mmDrinksSizesDB;
			optionsDB				= dBs->mmDrinksOptionsDB;
			forcedSidesDB			= dBs->mmDrinksForcedSidesDB;

			servingCourseDefault	= prefs->current.drinksServingCourseDefault;
			break;

		case DishesMenu:	//	!! Dishes !!
		default:
			menuDB					= dBs->mmDishesDB;
			menuSizesDB				= dBs->mmDishesSizesDB;
			optionsDB				= dBs->mmDishesOptionsDB;
			forcedSidesDB			= dBs->mmDishesForcedSidesDB;

			servingCourseDefault	= prefs->current.dishesServingCourseDefault;
			break;
	}




#ifndef DebugFakeMenu
	if(prefs->hLStatus.HLMenuSyncStatus != HLStatusAllDone)			//	Database Not Sync
	{
#ifndef ReleaseVersion
		MmPrvSyncLogTraceUI("HLMenuSyncStatus %d", prefs->hLStatus.HLMenuSyncStatus);
#endif
		FrmAlert(WarnDishesDBNotSyncAlert);
		handled = true;
		return handled;
	}
	else if(prefs->hLStatus.HLDrinkSyncStatus != HLStatusAllDone)	//	Database Not Sync
	{
#ifndef ReleaseVersion
		MmPrvSyncLogTraceUI("HLDrinkSyncStatus %d", prefs->hLStatus.HLDrinkSyncStatus);
#endif
		FrmAlert(WarnDrinksDBNotSyncAlert);
		handled = true;
		return handled;
	}
	else if(prefs->palmUniqueID == 0)
	{
		FrmAlert(WarnPalmNotRegisteredAlert);
		handled = true;
		return handled;
	}

#endif


	//	Check if (old) unsent orders exist. true -> check for OLD unsent orders only
	if(MmWarnIfUnsentOrders(dBs->mmOrdersDB, prefs, true,  numOldUnsentOrders))
	{
		*overviewPageType = OverviewUnsentPage;
		*overviewCheckboxes = OverviewCheckboxesUnsent;
		if(FrmGetFormId(frm) == MainForm)
			FrmGotoForm(OverviewForm);
		else
		{
			MmOverviewRedraw(frm, ScrollTop, DoRewriteArray);
			*screenRedrawn = true;
		}

		handled = true;
		return handled;
	}






	//	The integer value of the table cell Column1 stores the MmOrdersDB index value
	recIndex = displayRowsPtr->recIndex;

	recIndexSave = recIndex;
	recMenuIndex = NoValue16;	//	Just for starters






	//	Handle Differently between Mainview and Overview screens
	
	//	Mainview
	if(FrmGetFormId(frm) == MainForm)
	{
		recHandle = DmQueryRecord (dBs->mmOrdersDB, recIndex);

		//	First find the Order
		if(recHandle)
		{
			recPtr = MemHandleLock(recHandle);

	#ifndef DebugFakeMenu
			// Don't mess with it if it has been billed but not beamed
			if(recPtr->billing)								//	If its been billed
			{
				FrmAlert(WarnSeatBilledNotBeamedAlert);
				MemPtrUnlock(recPtr);
				handled = true;
				return handled;
			}
			else
	#endif
			{
				//	If this is a Linked Item, we actually need its parent
				if(recPtr->itemNumber != 0 		&&
					recPtr->menuIndex[1].index != NoValue16)
				{
					recMenuIndex = recPtr->menuIndex[0].index;
				}
				
				orderSetMenuMask = recPtr->setMenuMask;
				setMenuGroup = recPtr->setMenuGroup;
			
				MemPtrUnlock(recPtr);
			}
		}

		//	The integer value of the table cell Column0 stores the MmDishesDB index value
		if(recMenuIndex == NoValue16)
			recMenuIndex = displayRowsPtr->dishIndex;

		recMenuIndex = recMenuIndex & ~IsDrinkMask;
		recMenuHandle = DmQueryRecord (menuDB, recMenuIndex);

		// If the record was found, correctly set up the Prefs.current.course, etc.
		if(recMenuHandle)
		{
			recMenuPtr = MemHandleLock(recMenuHandle);

			previousCourse = prefs->current.course;			//	Use to detect whether we have changed course
			prefs->current.course = recMenuPtr->courseNumber;

			itemNumber = recMenuPtr->itemNumber;
			itemInCourse = recMenuPtr->itemInCourse;
			enabled = recMenuPtr->enabled;


			
			
//			MmAskUserForServingCourse()
			MmSetDefaultServingCourse(prefs, recMenuPtr, &previousCourse, 
												&servingCourseID, &servingCourseDefault, 
													&numServingCourses, &showServingCourses);




			MemPtrUnlock(recMenuPtr);
		}

		if(!(enabled & StatusAsEnabled))
		{
			FrmAlert(WarnMenuDishDisabledAlert);
			return handled;
		}
	}






	else	//	This is the Overview screen
	{
		//	If its a Serving Course step through ordering process,
		//	else might be an existing order so show details etc.
		if(displayRowsPtr->dishIndex == 0)
		{
			//	Must be a Serving Course, so offer courses for selection
			servingCourseID = displayRowsPtr->recIndex;
		}


		else	//	Must be an existing Order
		{
			recHandle = DmQueryRecord (dBs->mmOrdersDB, recIndex);

			//	First find the Order
			if(recHandle)
			{
				recPtr = MemHandleLock(recHandle);

#ifndef DebugFakeMenu
				// Don't mess with it if it has been billed but not beamed
				if(recPtr->billing)								//	If its been billed
				{
					FrmAlert(WarnSeatBilledNotBeamedAlert);
					MemPtrUnlock(recPtr);
					handled = true;
					return handled;
				}
				else
#endif
				{
					//	If this is a Linked Item, we actually need its parent
					if(recPtr->itemNumber != 0 		&&
						recPtr->menuIndex[1].index != NoValue16)
					{
						recMenuIndex = recPtr->menuIndex[0].index;
					}
					
					orderSetMenuMask = recPtr->setMenuMask;
					setMenuGroup = recPtr->setMenuGroup;
				
					MemPtrUnlock(recPtr);
				}
			}

			//	The integer value of the table cell Column0 stores the MmDishesDB index value
			if(recMenuIndex == NoValue16)
				recMenuIndex = displayRowsPtr->dishIndex;

			recMenuIndex = recMenuIndex & ~IsDrinkMask;
			recMenuHandle = DmQueryRecord (menuDB, recMenuIndex);

			// If the record was found, correctly set up the Prefs.current.course, etc.
			if(recMenuHandle)
			{
				recMenuPtr = MemHandleLock(recMenuHandle);

				prefs->current.course = recMenuPtr->courseNumber;
				itemNumber = recMenuPtr->itemNumber;
				itemInCourse = recMenuPtr->itemInCourse;
				enabled = recMenuPtr->enabled;

				MemPtrUnlock(recMenuPtr);
			}
		}
	}







	MmCreateRecordTemplate(prefs);							//	Fill in Prefs.curOrder defaults

	prefs->curOrder.tableNumber 	= prefs->current.tableNo;
	prefs->curOrder.seatNumber 	= prefs->current.seatNo;
	prefs->curOrder.menuType		= prefs->current.mMenu;
	prefs->curOrder.courseNumber	= prefs->current.course;





	//	If this is a Course offer the Dishes for selection
	if(itemNumber == 0)
	{
#ifdef ReleaseVersion
		if(!prefs->loggedIn)
		{
			ShowMessage("Please log in first");

			handled = true;
			return handled;
		}
#endif


		//	Display warning if the number of orders becomes rather high
		if(prefs->numUnsentOrders > 100			&&
			(prefs->numUnsentOrders % 10 == 0)		)	//	Rather arbitrary numbers
		{
			ShowMessage("You have very many unsent orders\n\nPlease beam these orders soon");
		}

		MmProcessNewOrder(frm, dBs, prefs, listItems, sizeListItems, listItemsStruct, sizeListIndices,
									lastItemOrdered, recMenuIndex, enabled, 
										servingCourseID, &servingCourseDefault,
											showServingCourses, showCourses, false, screenRedrawn
																																			);
	}


	//	If its not a Course it must be a Dish so offer Details facilities
	//	Might be a SetMenu Key item
	else
	{
		MmProcessExistingOrder(frm, dBs, prefs,
									listItems, sizeListItems, listItemsStruct, sizeListIndices,
										/*iconSelected, iconSelectedTime, */lastItemOrdered,
											recIndex, recMenuIndex, orderSetMenuMask, setMenuGroup, //setMenuDisplayID,
												itemNumber, itemInCourse, recIndexSave,
													 &servingCourseDefault, screenRedrawn
																																);
	}

	if(mMenu == DishesMenu)
		prefs->current.dishesServingCourseDefault = servingCourseDefault;
	else
		prefs->current.drinksServingCourseDefault = servingCourseDefault;
	
	prefs->current.mMenu = mMenu;					//	Restore
	
	return handled;
}			

