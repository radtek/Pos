#define EXTERNAL 1

#include "MenuMate.h"



/***********************************************************************
 *
 * FUNCTION:   	MmSetDisplayModeBooleanFlags
 *
 * DESCRIPTION:	Set Display Config Booleans for each display mode
 *
 * PARAMETERS:  
 *
 * RETURNED:   	nothing
 *
 ***********************************************************************/
void MmSetDisplayModeBooleanFlags(PageViewTypePtr show, UInt16 overviewPageType)
{
	switch(overviewPageType)
	{
		case OverviewTakeOrders:
			show->servingCourses		=	true;
			show->thisServingOnly	=	false;
			show->seatNumbers			=	true;
			show->orders				=	true;
			show->ravelOrders			=	true;
			show->unsentOnly			=	false;
			show->wholeTable			=	false;
			show->startThisSeat		=	false;
			show->allTables			=	false;
			
			break;
			
		case OverviewServingCourses:
			show->servingCourses		=	false;
			show->thisServingOnly	=	true;
			show->seatNumbers			=	true;
			show->orders				=	true;
			show->ravelOrders			=	true;
			show->unsentOnly			=	false;
			show->wholeTable			=	true;
			show->startThisSeat		=	false;
			show->allTables			=	false;
			
			break;
			
		case OverviewOrdersOnlyPage:
			show->servingCourses		=	false;
			show->thisServingOnly	=	false;
			show->seatNumbers			=	true;
			show->orders				=	true;
			show->ravelOrders			=	false;
			show->unsentOnly			=	false;
			show->wholeTable			=	false;
			show->startThisSeat		=	true;
			show->allTables			=	false;
			
			break;
			
		case OverviewUnsentPage:
			show->servingCourses		=	false;
			show->thisServingOnly	=	false;
			show->seatNumbers			=	true;
			show->orders				=	true;
			show->ravelOrders			=	false;
			show->unsentOnly			=	true;
			show->wholeTable			=	true;
			show->startThisSeat		=	false;
			show->allTables			=	true;
			break;
			
		case OverviewPrintReceiptsPage:
			show->servingCourses		=	false;
			show->thisServingOnly	=	false;
			show->seatNumbers			=	true;
			show->orders				=	false;
			show->ravelOrders			=	false;
			show->unsentOnly			=	false;
			show->wholeTable			=	true;
			show->startThisSeat		=	false;
			show->allTables			=	false;
			
			break;
			
		case OverviewClassicView:
			show->servingCourses		=	true;
			show->thisServingOnly	=	false;
			show->seatNumbers			=	false;
			show->orders				=	true;
			show->ravelOrders			=	true;
			show->unsentOnly			=	false;
			show->wholeTable			=	true;
			show->startThisSeat		=	true;
			show->allTables			=	false;
			break;
	}
}




/***********************************************************************
 *
 * FUNCTION:   	MmSetSeatToFirstSeatOrder
 *
 * DESCRIPTION:	
 *
 * PARAMETERS:  
 *
 * RETURNED:   	nothing
 *
 ***********************************************************************/
UInt16 MmFindFirstSeatOrder(DBTypePtr dBs, CurrentPrefsTypePtr prefs_current,
											Boolean* orderFound, UInt16* tableNo, UInt16* seatNo)
{
	MemHandle				recHandle;
	UInt16					recOrderIndex;
	OrderTypePtr			recOrderPtr;


	
	if(*tableNo == NoValue16)
	{
		recOrderIndex = 0;									//	Find very first record
	}
	else
	{
		*tableNo = prefs_current->tableNo;
		*seatNo = prefs_current->seatNo;					//	Preserve
		recOrderIndex = MmFindSortTableSeat(prefs_current->tableNo, 1);	//	Find first record for CurrentTableNumber
	}


	recHandle = DmQueryRecord(dBs->mmOrdersDB, recOrderIndex);
	if(recHandle)
	{
		recOrderPtr = MemHandleLock(recHandle);
		
		//	Only show the current Table
		if(recOrderPtr->tableNumber == prefs_current->tableNo)
		{
			*tableNo = recOrderPtr->tableNumber;
			*seatNo = recOrderPtr->seatNumber;

			MemPtrUnlock(recOrderPtr);
			*orderFound = true;
			return recOrderIndex;
		}

		MemPtrUnlock(recOrderPtr);
	}
	*orderFound = false;
	return NoValue16;											//	No orders found
}




/***********************************************************************
 *
 * FUNCTION:   	MmOverviewGetNextRecIndex
 *
 * DESCRIPTION:	Index through the 2D array returning successive indexes to Orders.
 *						Set the showServingCourse boolean for each new Serving Course
 *
 * PARAMETERS:  
 *
 * RETURNED:   	nothing
 *
 ***********************************************************************/
UInt16 MmOverviewGetNextRecIndex(Boolean *firstIndexPass, Boolean thisServingCourseOnly, 
												Boolean* showServingCourse, 
													UInt16* servingCourseIndex, UInt16* servingCourseID, 
														ServingCourseIndexType** serveIndxsPtr,
															ServeDataTypePtr serveDataPtr, 
																UInt16 numRecords, UInt16 numServingCourseRecords, 
																	Boolean* found)
{
	UInt16						recOrderIndex;
	static UInt16				i, j, prevServingCourse;

	
	*found = false;

	if(*firstIndexPass)
	{
		*firstIndexPass = false;
		i=0;
		j=0;
		prevServingCourse=0;
	}

	for(;i < numServingCourseRecords; i++, j=0)
	{
		if(thisServingCourseOnly && i != *servingCourseIndex)
		{
			continue;
		}
		
		if(prevServingCourse != i)
		{
			prevServingCourse = i;
			*servingCourseIndex = i;

			//	Normally show a SC header, but suppress if Non-selectable and no Unsent orders
			*showServingCourse = true;
			if(!serveDataPtr[i].selectable)
			{
				if(serveDataPtr[i].sentTotal == 0	&&
					serveDataPtr[i].unsentTotal == 0		)
				{
					*showServingCourse = false;
				}
			}

			*found = true;
			return NoValue16;
		}
		
		for(;j < numRecords; j++)
		{
			recOrderIndex = serveIndxsPtr[i][j].index;
			if(recOrderIndex != NoValue16)
			{
				*servingCourseID = serveIndxsPtr[i][j].servingCourseID;

				j++;
				*found = true;
				return recOrderIndex;
			}
		}
	}

	return NoValue16;
}



/***********************************************************************
 *
 * FUNCTION:   	MmFillServingCourseArray
 *
 * DESCRIPTION:	Loads the Serving Course ordered array
 *
 * PARAMETERS:  
 *
 * RETURNED:   	nothing
 *
 ***********************************************************************/
void MmFillServingCourseArray(DBTypePtr dBs, MmPreferenceTypePtr prefs, 
											UInt16 overviewPageType, UInt16* recOrderIndex, 
												UInt16 currentTableNumber, UInt16 currentSeatNumber,
													UInt16* servingCourse, UInt16* servingCourseID, 
														ServingCourseIndexTypePtr serve2DArrayPtr,
															ServingCourseIndexTypePtr*	serveIndxsPtr,
																ServeDataTypePtr	serveDataPtr, 
																	PageViewTypePtr show,
																		TableDisplayType *tableDisplayRowsPtr, 
																			UInt16 overviewExpandedStatus
																																)
{
	UInt16							numRecords;
	UInt16							numServingCourseRecords;

	MemHandle						recHandle;
	OrderTypePtr					recOrderPtr;
	ServingCourseTypePtr			recServingCoursePtr;

	UInt16							recIndex = 0;
	UInt16							recMenuIndex = 0;
	UInt16							i;
	
	UInt16							tableNo = 0;
	UInt16							seatNo;
	UInt32							servingCourseColor;
	Boolean							selectable;
		
	UInt8								menuType;

	Boolean							orderFound;




	numRecords = DmNumRecords(dBs->mmOrdersDB);
	numServingCourseRecords = DmNumRecords(dBs->mmServingCourseDB);


	//	If they want to see items for a serving course only, get the servingCourseID they want
	if(overviewPageType == OverviewServingCourses)
	{
		recHandle = DmQueryRecord(dBs->mmServingCourseDB, *servingCourse);

		if(recHandle)
		{
			recServingCoursePtr = MemHandleLock(recHandle);

			*servingCourseID = recServingCoursePtr->uniqueID;

			MemPtrUnlock(recServingCoursePtr);
		}
	}




	//	recOrderIndex was pre-loaded with start of current Table
	if(show->allTables)
	{
		tableNo = NoValue16;
		*recOrderIndex = MmFindFirstSeatOrder(dBs, &prefs->current, &orderFound, &tableNo, &seatNo);	//	Find start of this Seat
	}
	else if(show->wholeTable)
	{
		*recOrderIndex = MmFindFirstSeatOrder(dBs, &prefs->current, &orderFound, &tableNo, &seatNo);	//	Find start of this Seat
	}

	else if(currentSeatNumber != NoValue16)
		*recOrderIndex = MmFindSortTableSeat(prefs->current.tableNo, prefs->current.seatNo);	//	Find start of this Seat

	if(*recOrderIndex == NoValue16)
		*recOrderIndex = 0;								//	Start right at the beginning






	//	Next, go through all orders identifying and indexing their status.
	//	We need one array for each Serving Course, with indexes to each Order that applies

	for(recIndex = *recOrderIndex; recIndex<numRecords; recIndex++)
	{
		recHandle = DmQueryRecord(dBs->mmOrdersDB, recIndex);
		if(recHandle)
		{
			recOrderPtr = MemHandleLock(recHandle);

			if(recOrderPtr->tableNumber == LastTableRecord)
			{
				MemPtrUnlock(recOrderPtr);
				break;
			}
			
			//	Quit as soon as we're off this table (or all tables)
			if(	(!show->allTables	&&
						recOrderPtr->tableNumber != currentTableNumber)
				||
					(!show->wholeTable	&&
					  (recOrderPtr->tableNumber != currentTableNumber	||
						recOrderPtr->seatNumber != currentSeatNumber)		)	)
			{
				MemPtrUnlock(recOrderPtr);
				break;
			}
			

			//	Store the Order index into the appropriate serving course array,
			//	then ready the pointer for the next one
			menuType = NoValue8;

			MmGetServingCourseIndex(dBs->mmServingCourseDB, recOrderPtr->servingCourseID,
												&i, &servingCourseColor, &selectable);
		
			if((i < NoValue16)	||	(recOrderPtr->setMenuMask & SetMenuKey))
			{
				//	We use the 0th element to save set menu Key items so they display separately
				if(recOrderPtr->setMenuMask & SetMenuKey)
				{
					serveIndxsPtr[0]->selectable = selectable;
					serveIndxsPtr[0]->isSetMenuKey = true;		//	This really is a SC key item
					serveIndxsPtr[0]->index = recIndex;
					serveIndxsPtr[0]->servingCourseID = recOrderPtr->servingCourseID;

					(serveIndxsPtr[0])++;					//	Get pointer ready for next

					if(recOrderPtr->status & StatusAsSentConfirmed)
					{
						serveDataPtr[0].sentTotal++;		//	Keep Sent Total for this Serving Course
					}
					else
					{
						serveDataPtr[0].unsentTotal++;	//	Keep Unsent Total for this Serving Course
					}
				}
				else
				{
					//	Save the Index etc into the array
					serveIndxsPtr[i]->selectable = selectable;
					serveIndxsPtr[i]->isSetMenuKey = false;
					serveIndxsPtr[i]->index = recIndex;
					serveIndxsPtr[i]->servingCourseID = recOrderPtr->servingCourseID;

					(serveIndxsPtr[i])++;					//	Get pointer ready for next

					if(recOrderPtr->status & StatusAsSentConfirmed)
					{
						serveDataPtr[i].sentTotal++;		//	Keep Sent Total for this Serving Course
					}
					else
					{
						serveDataPtr[i].unsentTotal++;	//	Keep Unsent Total for this Serving Course
					}
				}
			}

			MemPtrUnlock(recOrderPtr);
		}
	}







	//	Next, go through all Serving Course Records and get the uniqueID.
	// Save the ID into the Data column for this serving course for easy access later
	//	and save key data relating to the Serving Course for easy access later
	for(recIndex=0; recIndex < numServingCourseRecords; recIndex++)
	{
		recHandle = DmQueryRecord(dBs->mmServingCourseDB, recIndex);
		if(recHandle)
		{
			recServingCoursePtr = MemHandleLock(recHandle);
			
		
			serveDataPtr[recIndex].menuType = recServingCoursePtr->menuType;
			serveDataPtr[recIndex].servingCourseID = recServingCoursePtr->uniqueID;

			if(show->thisServingOnly)
				serveDataPtr[recIndex].selectable = true;			//	Force to display
			else
				serveDataPtr[recIndex].selectable = recServingCoursePtr->selectable;

			if(serveDataPtr[recIndex].sentTotal	&&					//	Have got Sent orders ...
				overviewExpandedStatus != StatusIsExpanded		)
				serveDataPtr[recIndex].showCollapsed = true;		//	Set a default
			else
				serveDataPtr[recIndex].showCollapsed = false;
			

			//	Also go through the display table to find this Serving Course and find if its collapsed

			for(i=0; tableDisplayRowsPtr[i].seatNumber < NoValue16		&&	i < MaxTableDisplayRows; i++)
			{
				//	If its a Serving Course Name and its the correct ID,
				//	save the collapsed status before we start overwriting the table with new data
				if(	tableDisplayRowsPtr[i].overviewPageType == overviewPageType
					&&
						tableDisplayRowsPtr[i].seatNumber == currentSeatNumber
					&&
						tableDisplayRowsPtr[i].dishIndex == 0
					&&
						tableDisplayRowsPtr[i].recIndex == recServingCoursePtr->uniqueID
																												)
				{
					//	Show Collapsed if there are Sent Orders for this Serving Course,
					//	but expand if they asked for it.
					if(serveDataPtr[recIndex].sentTotal)					//	Have got Sent orders ...
					{
						if(tableDisplayRowsPtr[i].setExpanded == true	||	//	They want it expanded
							overviewExpandedStatus == StatusIsExpanded		)
						{
							serveDataPtr[recIndex].showCollapsed = false;//	... so don't show collapsed
							serveDataPtr[recIndex].setExpanded = true;	//	Make available for later

						}
					}
					tableDisplayRowsPtr[i].setExpanded = false;	//	Reset it
					break;	//	Found it
				}
			}

			MemPtrUnlock(recServingCoursePtr);
		}
	}





	//	Now 'point' the pointers back to the start of each of the Serving Course arrays (rows)
	for(i=0; i < numServingCourseRecords; i++)
	{
		serveIndxsPtr[i] = serve2DArrayPtr + (i * numRecords);
	}



/*
StrCopy(tempStr, "");

for(i=0; i < numServingCourseRecords; i++)
{
	for(j=0; j < numRecords; j++)
	{
		StrCat(tempStr, StrIToA(tempIntStr, serveIndxsPtr[i][j].index));
		StrCat(tempStr, " ");
	}
}
StrCat(tempStr, "\n");
for(i=0; i < numServingCourseRecords; i++)
{
	StrCat(tempStr, StrIToA(tempIntStr, serveDataPtr[i].total));
	StrCat(tempStr, " ");
}

ShowMessage("%s", tempStr);
*/



}






/***********************************************************************
 *
 * FUNCTION:   	MmOverviewWriteSeatOrders
 *
 * DESCRIPTION:	Loads the database records into the Overview TableDisplayRows array.
 *						Sets OverviewTopRowNum and Prefs.current.seatNo
 *						Do single scans only for the Serving Course displays,
 *						otherwise loop through Orders until end of seat, table or DB.
 *
 * PARAMETERS:  
 *
 * RETURNED:   	nothing
 *
 ***********************************************************************/
Boolean MmOverviewWriteSeatOrders(DBTypePtr dBs, MmPreferenceTypePtr prefs, Int16 overviewPageType, 
									Int16* overviewTopRowNum, UInt16* lastItemOrdered, 
										TableDisplayType* tableDisplayRowsPtr, Int16* row,
											UInt16* tableNumber, UInt16* nextTableNumber,
												UInt16* seatNumber, UInt16* nextSeatNumber,
													UInt16* recOrderIndex, UInt8 menuType, Boolean* firstPass, 
														Boolean* wasMarker, UInt16* servingCourseID, 
																PageViewTypePtr show, BundleGroupTypePtr bundleGroup, 
																	UInt16* bundleGroupIndex_J, UInt16* bundleGroupIndex_K
																																			)
{
	UInt16							thisRow;
	Int16								unsentItemCounter = 0;
	UInt16							rowsInTable;

	MemHandle						recHandle;
	OrderTypePtr					recOrderPtr;

	MemHandle						recMarkerHandle;
	OrderTypePtr					recMarkerPtr;

	UInt16							recIndex = 0;
	UInt16							recMenuIndex = 0;
	UInt16							recMarkerIndex = 0;

	UInt16							numRecords;

	Boolean							sizeFound			= false;

	UInt16							i, j, k;
	
	UInt16							status;





	numRecords = DmNumRecords(dBs->mmOrdersDB);

	if(prefs->current.fFontID == stdFont)
		rowsInTable = OvervwTableSizeStandardFont;
	else
		rowsInTable = OvervwTableSizeLargeFont;


	//	Main Loop. Load the Table etc.
	while(*recOrderIndex < numRecords		&&		*row < MaxTableDisplayRows)
	{
		recHandle = DmQueryRecord(dBs->mmOrdersDB, *recOrderIndex);
		if(recHandle)
		{
			recOrderPtr = MemHandleLock(recHandle);
			

			//	Only show the current Table, or Seats as appropriate
			//	Return True if all display conditions have been met,
			//	or False to go back and do next seat etc.
			//	Note that returning True for Dishes doesn't stop it repeating for Drinks

			//	Either way, we've reached the end
			if(recOrderPtr->tableNumber == LastTableRecord)
			{
				*nextTableNumber = recOrderPtr->tableNumber;
				MemPtrUnlock(recOrderPtr);
				return true;
			}
			


			//	Showing All Tables
			else if(show->allTables)
			{
				if(recOrderPtr->tableNumber != *tableNumber)
				{
					*nextTableNumber = recOrderPtr->tableNumber;
					*nextSeatNumber = recOrderPtr->seatNumber;
					MemPtrUnlock(recOrderPtr);
					(*recOrderIndex)++;
					return true;
				}

				else if(recOrderPtr->seatNumber != *seatNumber	&&	!show->thisServingOnly)
				{
					*nextSeatNumber = recOrderPtr->seatNumber;
					MemPtrUnlock(recOrderPtr);
					(*recOrderIndex)++;
					return false;
				}
			}


			

			//	Showing Whole CurrentTableNumber
			else if(show->wholeTable)
			{
				if(recOrderPtr->tableNumber != *tableNumber)
				{
					MemPtrUnlock(recOrderPtr);
					(*recOrderIndex)++;
					return true;
				}

				else if(recOrderPtr->seatNumber != *seatNumber	&&	!show->thisServingOnly)
				{
					*nextSeatNumber = recOrderPtr->seatNumber;
					MemPtrUnlock(recOrderPtr);
					(*recOrderIndex)++;
					return false;
				}
			}


			

			//	Showing just This Seat
			else if(		show->allTables == false
						&&
							show->wholeTable == false)
			{
				if(	recOrderPtr->tableNumber != *tableNumber
					||
						recOrderPtr->seatNumber != *seatNumber				)
				{
					MemPtrUnlock(recOrderPtr);
					(*recOrderIndex)++;
					return true;
				}
			}





			//	Display a Seat number because its different to the last one
			if(	(*firstPass && !show->servingCourses)
				||
					recOrderPtr->tableNumber != *tableNumber
				||
					recOrderPtr->seatNumber != *seatNumber
																					)
			{
				*firstPass = false;
				
				if(*wasMarker && show->orders)
				{
					(*row)--;
				}

				recMarkerIndex = MmFindSortTableSeat(recOrderPtr->tableNumber, recOrderPtr->seatNumber);
				
				
				recMarkerHandle = DmQueryRecord(dBs->mmOrdersDB, recMarkerIndex);
				if(recMarkerHandle)
				{
					recMarkerPtr = MemHandleLock(recMarkerHandle);

					//	Write the table and seat number to the display image
					tableDisplayRowsPtr[*row].overviewPageType = overviewPageType;
					tableDisplayRowsPtr[*row].rowDisplayType = RowIsSeat;
					tableDisplayRowsPtr[*row].tableNumber = recMarkerPtr->tableNumber;
					tableDisplayRowsPtr[*row].seatNumber = recMarkerPtr->seatNumber;


					tableDisplayRowsPtr[*row].dishIndex = NoValue16;
					tableDisplayRowsPtr[*row].recIndex = recMarkerIndex;
					tableDisplayRowsPtr[*row].menuType = recMarkerPtr->menuType;
					tableDisplayRowsPtr[*row].status = recMarkerPtr->status;
					tableDisplayRowsPtr[*row].billing = recMarkerPtr->billing;
					tableDisplayRowsPtr[*row].showCollapsed = false;
					tableDisplayRowsPtr[*row].setExpanded = false;
					tableDisplayRowsPtr[*row].drawLine = false;

					MemPtrUnlock(recMarkerPtr);
				}


				*tableNumber = recOrderPtr->tableNumber;
				*seatNumber = recOrderPtr->seatNumber;

				if(recOrderPtr->seatNumber == prefs->current.seatNo
					&&
					overviewPageType == OverviewUnsentPage				)
				{
					*overviewTopRowNum = *row;
				}
				*wasMarker = true;
				(*row)++;
			}




			//	Reset stuff if we're onto a new item
			if((	show->ravelOrders	&&	recOrderPtr->itemCounter == 0)	||
				( !show->ravelOrders	&&	unsentItemCounter < 1)					)
			{
				//	Clear the entire array of bundleGroups for next seat
				for(j=0; j < BundleGroupsMax; j++)
				{
					bundleGroup[j].menuIndex			= NoValue16;
					bundleGroup[j].servingCourseID	= NoValue16;
					bundleGroup[j].setMenuGroup		= NoValue16;
					bundleGroup[j].size					= NoValue16;
					bundleGroup[j].price					= NoValue32Signed;
					bundleGroup[j].sentCount			= 0;
					bundleGroup[j].unsentCount			= 0;
					bundleGroup[j].sentRow				= NoValue16;
					bundleGroup[j].unsentRow			= NoValue16;
					bundleGroup[j].isLinked				= false;
				}

				*bundleGroupIndex_J = 0;
			}





			//	Only write at the right times
			if(show->orders
				&&
					(	(show->servingCourses	||	show->thisServingOnly)
					||
						(!show->servingCourses	&&	!show->thisServingOnly)
						&&
						(	(menuType == DishesMenu) && (recOrderPtr->menuType == DishesMenu)	||
							(menuType == DrinksMenu) && (recOrderPtr->menuType == DrinksMenu)		)	)
				&&
					(recOrderPtr->courseNumber != 0)
				&&
					(!(recOrderPtr->setMenuMask & SetMenuKey)	||	overviewPageType == OverviewUnsentPage)
				&&
					(show->unsentOnly == false	||
					(show->unsentOnly == true && !(recOrderPtr->status & StatusAsSentConfirmed)))
																															)
			{
				*wasMarker = false;


				if(	(!show->servingCourses	&&	!show->thisServingOnly)
					||
						((show->servingCourses	||	show->thisServingOnly)	&&
						(recOrderPtr->servingCourseID == *servingCourseID)		)		)
				{
					//	Preset in case we don't find a size
					thisRow = *row;
					sizeFound = false;
					
					
					if(show->ravelOrders)
					{
						//	Search for this size in case it is already known
						//	If found, set up the row number and increment the count of that size type
						for(k=0; (bundleGroup[k].size != NoValue16) 	&& (k < BundleGroupsMax);	k++)
						{
							if((recOrderPtr->menuIndex[0].index == bundleGroup[k].menuIndex)		&&
								(recOrderPtr->menuIndex[1].index == NoValue16)							&&
								(recOrderPtr->servingCourseID == bundleGroup[k].servingCourseID)	&&
								(recOrderPtr->setMenuGroup == bundleGroup[k].setMenuGroup)			&&
								(recOrderPtr->size == bundleGroup[k].size)								&&
								(recOrderPtr->price == bundleGroup[k].price)								&&
								(bundleGroup[k].isLinked == false)											
																														)
							{
								//	This is a size we've had before, so we may just add another to it.
								//	Its possible that the previous one had a linked item, 
								//	or a different servingCourseID or setMenuGroup
								//	in which case we need to force it onto a new line anyway
								if(recOrderPtr->status & StatusAsSentConfirmed)
								{
									//	This is already ticked off, so use sentRow
									if(bundleGroup[k].sentRow != NoValue16)
									{
										thisRow = bundleGroup[k].sentRow;
										sizeFound = true;
										*bundleGroupIndex_K = k;
										break;
									}
								}
								else
								{
									//	Hasn't been sent yet so use unsentRow
									if(bundleGroup[k].unsentRow != NoValue16)
									{
										if(recOrderPtr->menuIndex[1].index == NoValue16)
											thisRow = bundleGroup[k].unsentRow;
										else
											thisRow = *row;
										
										sizeFound = true;
										*bundleGroupIndex_K = k;
										break;
									}
								}
							}
						}
					}
					if(sizeFound == false)
					{
						//	This size is unknown, so add it and note the row that it references
						bundleGroup[*bundleGroupIndex_J].menuIndex = recOrderPtr->menuIndex[0].index;
						bundleGroup[*bundleGroupIndex_J].servingCourseID = recOrderPtr->servingCourseID;
						bundleGroup[*bundleGroupIndex_J].setMenuGroup = recOrderPtr->setMenuGroup;
						bundleGroup[*bundleGroupIndex_J].size = recOrderPtr->size;
						bundleGroup[*bundleGroupIndex_J].price = recOrderPtr->price;
						if(recOrderPtr->status & StatusAsSentConfirmed)
						{
							bundleGroup[*bundleGroupIndex_J].sentRow = *row;
							bundleGroup[*bundleGroupIndex_J].sentCount = 0;
						}
						else
						{
							bundleGroup[*bundleGroupIndex_J].unsentRow = *row;
							bundleGroup[*bundleGroupIndex_J].unsentCount = 0;
						}
						*bundleGroupIndex_K = *bundleGroupIndex_J;
						(*bundleGroupIndex_J)++;
					}


					//	Write the table and seat number to the display image
					tableDisplayRowsPtr[*row].overviewPageType = overviewPageType;
					tableDisplayRowsPtr[*row].rowDisplayType = RowIsOrder;
					tableDisplayRowsPtr[*row].tableNumber = recOrderPtr->tableNumber;
					tableDisplayRowsPtr[*row].seatNumber = recOrderPtr->seatNumber;


					//	Display this item if it has not been shown before or if it has sides
					if(thisRow == *row)
					{
						if(*lastItemOrdered != NoValue16	&&
							recOrderPtr->menuIndex[0].index == *lastItemOrdered	&&
							((*row > *overviewTopRowNum + rowsInTable) ||
							 (*row < *overviewTopRowNum)))
						{
							*overviewTopRowNum = *row - DisplayRowOffset;
							if(*overviewTopRowNum < 0)
								*overviewTopRowNum = 0;
						}


						//	Preserve the Index into the Menu (indicating Dish/Drink)
						if(recOrderPtr->menuType == DishesMenu)
							tableDisplayRowsPtr[*row].dishIndex = recOrderPtr->menuIndex[0].index;
						else
							tableDisplayRowsPtr[*row].dishIndex = recOrderPtr->menuIndex[0].index
																									 | IsDrinkMask;

						//	Preserve the Index to the Order
						tableDisplayRowsPtr[*row].recIndex = *recOrderIndex;

						//	Preserve the menuType
						tableDisplayRowsPtr[*row].menuType = recOrderPtr->menuType;

						//	Preserve the status
						status = recOrderPtr->status;					//	Start with this record's status
						status &= 0x00FF;									//	Zero upper UInt8

						if(recOrderPtr->menuType == DrinksMenu)
						{
							if(recOrderPtr->menuIndex[1].index != NoValue16)
								status |= StatusHasLinkedItem;		//	Set bit 3 to indicate a Link
						}

						if(recOrderPtr->setMenuMask & SetMenuKey)
						{
							status |= StatusAsSetMenu;
							tableDisplayRowsPtr[*row].rowDisplayType = RowIsSetMenuKey;
						}
						tableDisplayRowsPtr[*row].status = status;

						//	Set the Marked status for Billing
						tableDisplayRowsPtr[*row].billing = recOrderPtr->billing;

						tableDisplayRowsPtr[*row].showCollapsed = false;
						tableDisplayRowsPtr[*row].setExpanded = false;
						tableDisplayRowsPtr[*row].drawLine = false;

						(*row)++;

					
						if(recOrderPtr->menuType == DishesMenu)
						{
							//	If this item has sides, display those too
							for(i=1; recOrderPtr->menuIndex[i].index != NoValue16 &&
														i < MaxLinkedItems &&
														*row < MaxTableDisplayRows;		 i++)
							{
								//	If this is a linked item, record to force new row for next item
								bundleGroup[*bundleGroupIndex_K].isLinked = true;
								
								//	Write the table and seat number to the display image
								tableDisplayRowsPtr[*row].overviewPageType = overviewPageType;
								tableDisplayRowsPtr[*row].rowDisplayType = RowIsOrderSide;
								tableDisplayRowsPtr[*row].tableNumber = recOrderPtr->tableNumber;
								tableDisplayRowsPtr[*row].seatNumber = recOrderPtr->seatNumber;
								
								//	Preserve the Index into the Menu (indicating Dish/Drink)
								if(recOrderPtr->menuType == DishesMenu)
									tableDisplayRowsPtr[*row].dishIndex = recOrderPtr->menuIndex[i].index;
								else
									tableDisplayRowsPtr[*row].dishIndex = recOrderPtr->menuIndex[i].index
																									 | IsDrinkMask;
								//	Preserve the Index to the Order
								tableDisplayRowsPtr[*row].recIndex = *recOrderIndex;

								//	Preserve the menuType
								tableDisplayRowsPtr[*row].menuType = recOrderPtr->menuType;

								//	Preserve the status
								status = 0;
								status |= StatusHasLinkedItem;			//	Set bit 3 to indicate a Link
		
								if(recOrderPtr->status & StatusAsSentConfirmed)
									status |= StatusAsSentConfirmed;
		
								tableDisplayRowsPtr[*row].status = status;

								//	These never have relevance for linked items
								tableDisplayRowsPtr[*row].billing = 0;
								
								tableDisplayRowsPtr[*row].showCollapsed = false;
								tableDisplayRowsPtr[*row].setExpanded = false;
								tableDisplayRowsPtr[*row].drawLine = false;

								(*row)++;
							}
						}
						else	//	Is a Drink item
						{
							//	If this was a drink mix, force a new line for next
							if(recOrderPtr->menuIndex[1].index != NoValue16)
							{
								bundleGroup[*bundleGroupIndex_K].isLinked = true;
							}
						}
					}
					else	//	We've had this dish before, so just update the count
					{
						status = recOrderPtr->status;		//	Start with this record's status
						status &= 0x00FF;						//	Zero upper UInt8

						if(recOrderPtr->status & StatusAsSentConfirmed)
						{
							bundleGroup[*bundleGroupIndex_K].sentCount++;
							status |= bundleGroup[*bundleGroupIndex_K].sentCount << 8;		//	itemCounter in upper UInt8
							tableDisplayRowsPtr[bundleGroup[*bundleGroupIndex_K].sentRow].status = status;
						}
						else
						{
							bundleGroup[*bundleGroupIndex_K].unsentCount++;
							status |= bundleGroup[*bundleGroupIndex_K].unsentCount << 8;	//	itemCounter in upper UInt8
							tableDisplayRowsPtr[bundleGroup[*bundleGroupIndex_K].unsentRow].status = status;
						}
					}
				}
			}

			MemPtrUnlock(recOrderPtr);
		}
		(*recOrderIndex)++;
		
		//	Do single scans only for the Serving Course displays
		if(show->servingCourses	||	show->thisServingOnly)
		{
			return false;
		}
	}
	return false;
}