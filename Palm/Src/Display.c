#define EXTERNAL 1

#include "MenuMate.h"

/***********************************************************************
 *
 * FUNCTION:    ShowDebug
 *
 * DESCRIPTION: Same as ShowMessage() except easy to find for removal
 *
 * PARAMETERS:  same as StrPrintF
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
void ShowDebug(const Char *format, ...)
{
	va_list				args;
	Char					message[256];
	
	MemSet(message, 256, 0);
	// Format the message, if any, and append it to the field
	if(format)
	{
		// Format the message
		va_start(args, format);
		StrVPrintF(message, format, args);
		va_end(args);
	}
	FrmCustomAlert(InfMessageAlert, message, "", "");
}



/***********************************************************************
 *
 * FUNCTION:    ShowMessage
 *
 * DESCRIPTION: Shows a message as a popup. Max length 255 chars
 *
 * PARAMETERS:  same as StrPrintF
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
void ShowMessage(const Char *format, ...)
{
	va_list				args;
	Char					message[256];
	
	MemSet(message, 256, 0);
	// Format the message, if any, and append it to the field
	if(format)
	{
		// Format the message
		va_start(args, format);
		StrVPrintF(message, format, args);
		va_end(args);
	}
	FrmCustomAlert(InfMessageAlert, message, "", "");
}



/***********************************************************************
 *
 * FUNCTION:    ShowError
 *
 * DESCRIPTION: Shows an error message as a popup. Max length 255 chars
 *
 * PARAMETERS:  same as StrPrintF
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
void ShowError(const Char *format, ...)
{
	va_list				args;
	Char					message[256];
	
	MemSet(message, 256, 0);
	// Format the message, if any, and append it to the field
	if(format)
	{
		// Format the message
		va_start(args, format);
		StrVPrintF(message, format, args);
		va_end(args);
	}
	ErrAlertCustom(NULL, message, NULL, NULL);
}


/***********************************************************************
 *
 * FUNCTION:    LogPortErr
 *
 * DESCRIPTION: Prints the error type.
 *
 * PARAMETERS:
 *
 *	err
 *			error to print.
 *
 * RETURNED:    Nothing
 *
 ***********************************************************************/
void LogPortErr(Err err)
{
	switch(err)
	{
		case serErrBadParam:					ShowError("Bad Parameter"); break;
		case serErrBadPort:					ShowError("Bad Port"); break;
		case serErrNoMem:						ShowError("No Memory"); break;
		case serErrBadConnID:				ShowError("Bad Connection ID"); break;
		case serErrTimeOut:					ShowError("Timeout"); break;
		case serErrLineErr:					ShowError("Line Err"); break;
		case serErrAlreadyOpen:				ShowError("Already Open"); break;
		case serErrStillOpen:				ShowError("Still Open"); break;
		case serErrNotOpen:					ShowError("Not Open"); break;
		case serErrNotSupported:			ShowError("Not Supported"); break;
		case serErrNoDevicesAvail:			ShowError("No Devices Available"); break;
		case serErrConfigurationFailed:	ShowError("Configuration Failed"); break;

		case btLibErrCanceled:				ShowError("Cancelled"); break;

		case errNone:							/*ShowMessage("Successful");*/ break;
		default:									ShowError("Error %x", err); break;
	}
}


/***********************************************************************
 *
 * FUNCTION:	  MmLogAndDisplayError
 *
 * DESCRIPTION:  Write the passed error Message to the Log and display.
 *
 * PARAMETERS:	
 *
 * RETURNED:	  
 *
 ***********************************************************************/
void MmLogAndDisplayError(Char* errorMessagePtr, Boolean reset)
{
	Char					tempStr[300] = "Please write this down and report: ... \n";
	
#ifdef TestMessageLog
	PrvSyncLogTrace("\nHLErr: %s", errorMessagePtr);
#endif
	StrCat(tempStr, errorMessagePtr);
	ErrAlertCustom(NULL, tempStr, NULL, NULL);

	if(reset)
		SysReset();
}



#ifdef AllowDebugLogDatabaseButton
#ifdef TestMenus
/***********************************************************************
 *
 * FUNCTION:		MmLogDatabase
 *
 * DESCRIPTION:	Step through the DB, writing all records to Infosync log.
 *
 * PARAMETERS:		.
 *
 * RETURNED:		.
 *
 ***********************************************************************/
void MmLogDatabase(DmOpenRef *DBName)
{
	MemHandle				recHandle;
	UInt16 					recIndex;
	UInt16					numRecords;
	OrderTypePtr			recPtr;
	UInt8						*ptr;
	
	numRecords = DmNumRecords(*DBName);
	MmPrvSyncLogTraceUI("\nCurrent Orders = %d", numRecords);
	MmPrvSyncLogTrace("\nX O T S C D Cnt Ix0 Ix1 St Opts 123");
	for(recIndex=0; recIndex<=numRecords; recIndex++)
	{
		recHandle = DmQueryRecord(*DBName, recIndex);
		if(recHandle)
		{
			recPtr = MemHandleLock(recHandle);
			ptr = &recPtr->options + recPtr->noteLength;

			PrvSyncLogTrace("\n%d  %d %d %d %d   %d    %d    %d    %d    ",
				recIndex, recPtr->tableNumber, recPtr->seatNumber, recPtr->menuType, 
				recPtr->courseNumber, recPtr->itemNumber, recPtr->itemCounter,
				recPtr->menuIndex[0].index, recPtr->menuIndex[1].index, recPtr->status);

			//	Only show Options for Dishes/Drinks
			if(recPtr->courseNumber)
			{
				do
				{
					MmPrvSyncLogTraceUI("%d ", *ptr);
				}while(*(ptr++) < 128);
			}
			MemPtrUnlock(recPtr);
		}
	}
}
#endif
#endif



/***********************************************************************
 *
 * FUNCTION:		MmShowNoteInButton
 *
 * DESCRIPTION:	Shows the Note in the passed Label
 *
 * PARAMETERS:		Nil
 *
 * RETURNED:		Nil
 *
 ***********************************************************************/
void MmShowNoteInButton(FormPtr frm, UInt16 labelID, Char *sourceLabel, MmPreferenceTypePtr prefs)
{
	ControlType				*buttonPtr;
	Int16						labelIndex;
	Int16						buttonWidth;
	Int16						textLen;
	Boolean					fits = true;
	RectangleType			rect;


	labelIndex = FrmGetObjectIndex (frm, labelID);
	buttonPtr = FrmGetObjectPtr (frm, labelIndex);


	FntSetFont(boldFont);


	// Determine the length of text that will fit within the bounds of the button.
	textLen = StrLen(sourceLabel);							//	Untrimmed length
	FrmGetObjectBounds (frm, labelIndex, &rect);
	buttonWidth = rect.extent.x -10;							//	Allow space at sides
	
	FntCharsInWidth(sourceLabel, &buttonWidth, &textLen, &fits);

	if(labelID == DetailsAttachNoteButton)
	{
		if(StrLen(sourceLabel) == 0)
		{
			CtlSetLabel(buttonPtr, "Note");
		}
		else
		{
			StrNCopy(prefs->current.noteButtonLabel, sourceLabel, textLen);
			prefs->current.noteButtonLabel[textLen] = 0;
			CtlSetLabel(buttonPtr, prefs->current.noteButtonLabel);
		}
	}
	else if(labelID == DetailsServeButton)
	{
		if(StrLen(sourceLabel) == 0)
		{
			CtlSetLabel(buttonPtr, "No Serving");
		}
		else
		{
			StrNCopy(prefs->current.servingButtonLabel, sourceLabel, textLen);
			prefs->current.servingButtonLabel[textLen] = 0;
			CtlSetLabel(buttonPtr, prefs->current.servingButtonLabel);
		}
	}


	FntSetFont(prefs->current.fFontID);
}



/***********************************************************************
 *
 * FUNCTION:		MmShowServingCourse
 *
 * DESCRIPTION:	Shows the Serving Course in the passed Label
 *
 * PARAMETERS:		Nil
 *
 * RETURNED:		Nil
 *
 ***********************************************************************/
void MmShowServingCourse(FormPtr frm, UInt16 labelID, DBTypePtr dBs, 
												UInt16 itemSelected, MmPreferenceTypePtr prefs)
{
	MemHandle 					recHandle;
	ServingCourseTypePtr		recServePtr;
	UInt32						servingCourseColor;
	Boolean						selectable;
	char							tempStr[MaxMessageFromServerLength+1];


	if(itemSelected == NoValue16)
	{
		if(labelID == DetailsServeButton)
		{
			MmGetServingCourseIndex(dBs->mmServingCourseDB, prefs->curOrder.servingCourseID,
											&itemSelected, &servingCourseColor, &selectable);
		}
		else	//	MainForm Title
		{
			if(prefs->current.mMenu == DishesMenu)
				MmGetServingCourseIndex(dBs->mmServingCourseDB, prefs->current.dishesServingCourseDefault,
													&itemSelected, &servingCourseColor, &selectable);
			else
				MmGetServingCourseIndex(dBs->mmServingCourseDB, prefs->current.drinksServingCourseDefault,
													&itemSelected, &servingCourseColor, &selectable);
		}
		recHandle = DmQueryRecord(dBs->mmServingCourseDB, itemSelected);
	}
	else
	{
		recHandle = DmQueryRecord(dBs->mmServingCourseDB, itemSelected);
	}

	if(recHandle)
	{
		recServePtr = MemHandleLock(recHandle);

		if(labelID == DetailsServeButton)
		{
			//	Write the new selection into the record, then display it
			if(itemSelected != NoValue16)
			{
				prefs->curOrder.servingCourseID = recServePtr->uniqueID;
				MmCopyCurrentOrderIndicesToRecord(dBs->mmOrdersDB, prefs);
			}
			MmShowNoteInButton(frm, DetailsServeButton, recServePtr->standardName, prefs);
		}
		else	//	MainForm Title
		{
			StrCopy(tempStr, recServePtr->standardName);
			tempStr[10] = 0;					//	Long enough to fit 'No Serving'.
			FrmCopyTitle(frm, tempStr);
		}

		MemPtrUnlock(recServePtr);
	}
	else
	{
		if(labelID == DetailsServeButton)
		{
			MmShowNoteInButton(frm, DetailsServeButton, "", prefs);
		}
		else	//	MainForm Title
		{
			FrmCopyTitle(frm, "No Serving");
		}
	}
}




/***********************************************************************
 *
 * FUNCTION:		MmDrawOverviewLines
 *
 * DESCRIPTION:	Paints the outside edge of the form black.
 *
 * PARAMETERS:		
 *
 * RETURNED:		
 *
 ***********************************************************************/
void MmDrawOverviewLines(Boolean overviewCheckboxes)
{
	UInt16 rightLine;

	rightLine = OverviewBillingColPos; 

	if(overviewCheckboxes == OverviewCheckboxesBilling)
	{
		//	Draw vertical separator lines
//		WinDrawLine(OverviewBillingColPos, OverviewScreenTopBorder, OverviewBillingColPos, OverviewScreenHeight);
		WinDrawLine(rightLine, OverviewScreenTopBorder, rightLine, OverviewScreenHeight);
	}
	else if(overviewCheckboxes == OverviewCheckboxesUnsent)
	{
		//	Draw vertical separator lines
		WinDrawLine(rightLine, OverviewScreenTopBorder, rightLine, OverviewScreenHeight);
	}

	//	Blacken the edges
//	WinDrawLine(0,OverviewScreenTopBorder,0,OverviewScreenHeightFull);		//	Vertical left
//	WinDrawLine(1,OverviewScreenTopBorder,1,OverviewScreenHeightFull);

//	WinDrawLine(158,OverviewScreenTopBorder,158,OverviewScreenHeightFull);	//	Vertical right
//	WinDrawLine(159,OverviewScreenTopBorder,159,OverviewScreenHeightFull);

//	WinDrawLine(0,158,159,158);				//	Horizontal bottom
//	WinDrawLine(0,159,159,159);
}


/***********************************************************************
 *
 * FUNCTION:		MmDrawTableSeatBoxes
 *
 * DESCRIPTION:	Paints boxes around the Table and Seat boxes in Mainview.
 *
 * PARAMETERS:		
 *
 * RETURNED:		
 *
 ***********************************************************************/
void MmDrawTableSeatBoxes(void)
{
	RectangleType			box;

	//	Table: x, y, width, height
	RctSetRectangle(&box, 9, 17, 61, 15);
	WinDrawRectangleFrame(roundFrame, &box);
	//	Seat
	RctSetRectangle(&box, 91, 17, 61, 15);
	WinDrawRectangleFrame(roundFrame, &box);
}


/***********************************************************************
 *
 * FUNCTION:		MmDrawRectangle
 *
 * DESCRIPTION:	
 *
 * PARAMETERS:		frameType. (5 = shadow box)
 *
 * RETURNED:		
 *
 ***********************************************************************/
void MmDrawRectangle(UInt16 frameType, UInt16 topLeftX, UInt16 topLeftY,
													UInt16 extentX, UInt16 extentY)
{
	RectangleType			rect;
	
	rect.topLeft.x = topLeftX;
	rect.topLeft.y = topLeftY;
	rect.extent.x = extentX;
	rect.extent.y = extentY;

	WinDrawRectangleFrame(frameType, &rect);
}



/***********************************************************************
 *
 * FUNCTION:		MmItemToString
 *
 * DESCRIPTION:	Essentially does a right justified IToA function
 *
 * PARAMETERS:		
 *
 * RETURNED:		
 *
 ***********************************************************************/
Boolean MmItemToString(UInt16 itemSelected, Char numberStr[4],
											Boolean checkList, Char* *listItems)
{
	numberStr[2] = 0;										// Terminate string

	//	Don't always want to check the ListItems list for "None Available"
	if(checkList)
	{
		if(StrCaselessCompare((const char *)listItems[0], "None Available") == 0)
			itemSelected = NoValue16;
	}

	if(itemSelected != NoValue16)
	{
		if(itemSelected <= 9)
			numberStr[0] = ' ';							// Put 10s value
		else
			numberStr[0] = itemSelected/10+0x30;	// Put 10s value
		numberStr[1] = itemSelected%10+0x30;		// Put 1s value

		return true;
	}
	return false;
}



/***********************************************************************
 *
 * FUNCTION:		MmRoundPrice
 *
 * DESCRIPTION:	
 *						
 *
 * PARAMETERS:		
 *
 * RETURNED:		
 *
 ***********************************************************************/
Int32 MmRoundPrice(Int32 price)
{
	int 				oneCents;
	
	oneCents = price % 10;

	switch(oneCents)
	{
		case 0:	price -= 0;	break;
		case 1:	price -= 1;	break;
		case 2:	price -= 2;	break;
		case 3:	price += 2;	break;
		case 4:	price += 1;	break;
		case 5:	price -= 0;	break;
		case 6:	price -= 1;	break;
		case 7:	price -= 2;	break;
		case 8:	price += 2;	break;
		case 9:	price += 1;	break;
	}          
	return price;
}


/***********************************************************************
 *
 * FUNCTION:		MmShowPrice
 *
 * DESCRIPTION:	Shows the passed price in the passed Label
 *
 * PARAMETERS:		frm, passedPrice
 *
 * RETURNED:		Nil
 *
 ***********************************************************************/
void MmShowPrice(FormPtr frm, UInt16 labelID, Int32 passedPrice, UInt16 labelsize)
{
	Char					priceStr[] = "                   ";
	Char					tempStr[sizeof(priceStr)];
	Char					*sourcePtr;
	Char					*destPtr;
	UInt16				strLen;

	UInt16				objIndex;
//	FormLabelType 		*labelPtr;
	RectangleType		bounds;


	//	If no price exists, display $0.00
	if(passedPrice == NoValue32Signed)
		passedPrice = 0;
	
	//	Convert passedPrice to format $xxx.xx, inserting 0s as necessary 
	StrIToA(tempStr, passedPrice);					// Convert price to string

	strLen = StrLen(tempStr);
	sourcePtr = &tempStr[0] + strLen-1;				//	Start at end of string ...
	destPtr = &priceStr[0] + sizeof(priceStr)-2;	//	... and work towards the start

	if(passedPrice == 0)
		*destPtr-- = '0';
	else
		*destPtr-- = *sourcePtr--;

	if(strLen <= 1)
		*destPtr-- = '0';
	else
		*destPtr-- = *sourcePtr--;

	*destPtr-- = '.';

	if(strLen <= 2)
	{
		*destPtr-- = '0';
	}
	else
	{
		while(sourcePtr >= tempStr)
			*destPtr-- = *sourcePtr--;
	}

	*destPtr = '$';

	//	Now shuffle everything to the front of the price string
	sourcePtr = &priceStr[0];				//	Start of string ...
	destPtr = &priceStr[0];					//	Start of string ...

	while(*sourcePtr == ' ')				//	Find the start of the price text
		sourcePtr++;

	*destPtr++ = ' ';							//	Insert a space for separation
	*destPtr++ = ' ';							//	Insert a space for separation

	while(*sourcePtr)
	{
		*destPtr++ = *sourcePtr++;			//	Copy the Price text down
	}
	
	*destPtr = 0;								//	Add the Null

	
	priceStr[labelsize] = 0;				//	Ensure we never over-run the label size
	

	FrmHideObject(frm, FrmGetObjectIndex(frm, labelID));

	FrmCopyLabel(frm, labelID, priceStr);

	if(labelID == MainPriceLabel)
	{
		if(passedPrice != 0)
		{
			objIndex = FrmGetObjectIndex (frm, labelID);
			FrmGetObjectBounds(frm, objIndex, &bounds);
			FrmSetObjectPosition(frm, objIndex, 157-bounds.extent.x, 34);
			FrmShowObject(frm, FrmGetObjectIndex(frm, labelID));
		}
	}
	else
	{
		FrmShowObject(frm, FrmGetObjectIndex(frm, labelID));
	}
}


/***********************************************************************
 *
 * FUNCTION:		MmShowTablePrice
 *
 * DESCRIPTION:	Shows the total price in the Main window
 *
 * PARAMETERS:		Nil
 *
 * RETURNED:		Nil
 *
 ***********************************************************************/
void MmShowTablePrice(FormPtr frm, UInt16 labelID, DmOpenRef mmOrdersDB,
										MmPreferenceTypePtr prefs)
{
	UInt16					recIndex;
	MemHandle				recHandle;
	OrderTypePtr			recPtr;
	UInt16					numberOfRecs;
	
	Int32						totalPrice = 0;


	recIndex = MmFindSortTableSeat(prefs->current.tableNo, 1);	//	Find Marker record for Table/Seat

	//	Go through all the records for this Table to total the price
	numberOfRecs = DmNumRecords(mmOrdersDB);

	for(; recIndex <= numberOfRecs; recIndex++)
	{
		recHandle = DmQueryNextInCategory(mmOrdersDB, &recIndex, 0);
		// If the record was found, get the price,
		if(recHandle)
		{
			recPtr = MemHandleLock(recHandle);
			if(recPtr->tableNumber	!= prefs->current.tableNo)
			{
				MemPtrUnlock(recPtr);
				break;
			}
			if(recPtr->courseNumber == 0)
			{
				if(recPtr->price != NoValue32Signed)
				{
					totalPrice += recPtr->price;					//	Markers may have price correction
				}
			}
			MemPtrUnlock(recPtr);
		}
	}

	MmShowPrice(frm, labelID, totalPrice, 13);
}



/***********************************************************************
 *
 * FUNCTION:		MmShowSeatPrice
 *
 * DESCRIPTION:	Shows the total price in the Main window
 *
 * PARAMETERS:		Nil
 *
 * RETURNED:		Nil
 *
 ***********************************************************************/
void MmShowSeatPrice(FormPtr frm, UInt16 labelID, DmOpenRef mmOrdersDB,
										MmPreferenceTypePtr prefs)
{
	UInt16					recIndex;
	MemHandle				recHandle;
	OrderTypePtr			recPtr;
	
	Int32						totalPrice = 0;

	recIndex = MmFindSortTableSeat(prefs->current.tableNo, prefs->current.seatNo);//	Find Marker record for Table/Seat

	//	First get the seat price from the Marker record
	recHandle = DmQueryRecord (mmOrdersDB, recIndex);
	if(recHandle)
	{
		recPtr = MemHandleLock(recHandle);
		if(recPtr->seatNumber	== prefs->current.seatNo	&&
			recPtr->tableNumber	== prefs->current.tableNo		)
		{
			if(recPtr->price != NoValue32Signed)
			{
				totalPrice = recPtr->price;
			}
		}
		MemPtrUnlock(recPtr);
	}
	
	MmShowPrice(frm, labelID, totalPrice, 13);
}


/***********************************************************************
 *
 * FUNCTION:		MmFormatPriceString
 *
 * DESCRIPTION:	Shows the total price in the Main window
 *
 * PARAMETERS:		Nil
 *
 * RETURNED:		Nil
 *
 ***********************************************************************/
void MmFormatPriceString(Int32 price, UInt16 isPrice, Char* priceStr, UInt16 offset)
{
	Char						tempStr[20];
	Char*						sourcePtr;
	Char*						destPtr;
	UInt16					strLen;

	if((price < 0) && (isPrice == IsPriceAdj))//	If its negative
		StrIToA(tempStr, (~price)+1);				// Display 2's Complement; (make it +ve signed)
	else
		StrIToA(tempStr, price);					// Bang it out as it is

	//	If its 0, set it up to become 0.00
	if(!StrCompare(tempStr, "0"))
		StrCopy(tempStr, "000");
	
	strLen = StrLen(tempStr);
	
	//	If there's only units of cents, set up for 10s column
	if(strLen == 1)									//	If there's no 10s column, make one
	{
		tempStr[2] = 0;								//	Ensure null terminates the string
		tempStr[1] = tempStr[0];					//	Shove it up one
		tempStr[0] = '0';								//	10s column is 0
		strLen = 2;
	}
	
	//	Start at end of string ... and work towards the start
	sourcePtr = &tempStr[0] + strLen - 1;
	if(isPrice == LeftAlign)
	{
		destPtr = &priceStr[offset] + strLen + 3;	//	Allow for '+' or '-', '$' and '.' 
		*destPtr-- = 0;								//	Terminate the string
	}
	else if(offset == 1)
		destPtr = &priceStr[offset+8] + 7;		//	
	else
		destPtr = &priceStr[offset] + 7;			//	
	
	//	Copy the 2 decimal places, then add a decimal point
	*destPtr-- = *sourcePtr--;
	*destPtr-- = *sourcePtr--;
	*destPtr-- = '.';

	//	Now copy the rest and prefix a $
	while(sourcePtr >= tempStr)
		*destPtr-- = *sourcePtr--;
	*destPtr-- = '$';

	//	Add a plus or minus if an Offset was provided
	if((isPrice == LeftAlign) ||
		(offset !=0 && offset != 1 && price != 0))
	{
		if(price < 0)									//	If its negative
			*destPtr = '-';
		else
			*destPtr = '+';
	}
}




/***********************************************************************
 *
 * FUNCTION:		MmMainScrollDown
 *
 * DESCRIPTION:	
 *
 * PARAMETERS:		
 *						
 *
 * RETURNED:		Nothing.
 *
 ***********************************************************************/
void MmDetailsFormRedraw(FormPtr frm, DBTypePtr dBs, MmPreferenceTypePtr prefs)
{
	ControlType					*buttonPtr;
	ListPtr						listPtr;							//	Selected Options list
	MemHandle 					recHandle;
	UInt16						recIndex;

	MenuTypePtr					recMenuPtr;

	DmOpenRef					menuDB;
	DmOpenRef					menuSizesDB;




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



	// Draw the details form and the Selected Options list.
//	FrmDrawForm(frm);


	buttonPtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, DetailsPriceAdjButton));

	if(prefs->curOrder.price == NoValue32Signed)
	{
		CtlSetLabel(buttonPtr, "Price");
		CtlShowControl(buttonPtr);
	}
	else
	{
		CtlSetLabel(buttonPtr, "$ $ $");
		CtlShowControl(buttonPtr);
	}


	listPtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, DetailsOptionsSelectedList));
	buttonPtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, DetailsLinkedItemsButton));
	MmLstSetFont(listPtr, prefs);
	
	//	Get the Dish Name and display in the Title Bar
	if(prefs->current.mMenu == DishesMenu)
	{
		CtlSetLabel(buttonPtr, "Sides");
	}
	else
	{
		CtlSetLabel(buttonPtr, "Mix");
	}
	FrmShowObject(frm, FrmGetObjectIndex(frm, DetailsSizePopTrigger));

	recHandle = DmQueryRecord(menuDB, prefs->curOrder.menuIndex[0].index);
	if(recHandle)
	{
		recMenuPtr = MemHandleLock(recHandle);
		FrmCopyTitle(frm, recMenuPtr->itemName);
		MemPtrUnlock(recMenuPtr);
	}

	FrmShowObject(frm, FrmGetObjectIndex(frm, DetailsLinkedItemsButton));

	//	Set Prefs.current.record
	ErrFatalDisplayIf(!MmFindCurrentRecordIndex(&recIndex, FindOnly, SetPriceAdjust),
			"Could not find Details record");
	
	FrmShowObject(frm, FrmGetObjectIndex(frm, DetailsOptionsSelectedList));
	MmDetailsReDrawOptionsList(frm, listPtr, prefs->current.recIndex);

	MmShowNoteInButton(frm, DetailsAttachNoteButton, prefs->curOrder.note, prefs);
	MmShowServingCourse(frm, DetailsServeButton, dBs, NoValue16, prefs);

	MmDisplayPopNameLabel(frm, dBs, menuSizesDB,
									prefs->curOrder.size, DetailsSizePopTrigger, true);
}




/***********************************************************************
 *
 * FUNCTION:		MmMainScrollDown
 *
 * DESCRIPTION:	
 *
 * PARAMETERS:		
 *						
 *
 * RETURNED:		Nothing.
 *
 ***********************************************************************/
void MmMainScrollDown(FormPtr frm, MmPreferenceTypePtr prefs, 
									Int16* mainviewTopRowNum, Boolean mainviewCanScrollDown)
{
	if(!mainviewCanScrollDown)
		return;

	if(prefs->current.fFontID == stdFont)
	   *mainviewTopRowNum += MainTableSizeStandardFont;
	else
	   *mainviewTopRowNum += MainTableSizeLargeFont;

	MmMainviewRedraw(frm, NoScroll, NoRewriteArray);
}



/***********************************************************************
 *
 * FUNCTION:		MmMainScrollUp
 *
 * DESCRIPTION:	
 *
 * PARAMETERS:		
 *						
 *
 * RETURNED:		Nothing.
 *
 ***********************************************************************/
void MmMainScrollUp(FormPtr frm, MmPreferenceTypePtr prefs, 
									Int16* mainviewTopRowNum)
{
	if(prefs->current.fFontID == stdFont)
	   *mainviewTopRowNum -= MainTableSizeStandardFont;
	else
	   *mainviewTopRowNum -= MainTableSizeLargeFont;

	MmMainviewRedraw(frm, NoScroll, NoRewriteArray);
}



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
void MmMainviewDrawRecordInBounds(Char* dishPtr, RectanglePtr bounds, UInt8 itemNumber, 
												UInt16 status, UInt32 servingCourseColor, 
													MmPreferenceTypePtr prefs)
{
	Int16						textLen, width;
	UInt16					numOrdered;
	Boolean					fits = true;
	RectangleType			space = {0,0,10,10};

	char						tempStr[MaxInfoNameSize + MaxMenuNameSize +2];
	char						bulletChar[2];	// = "\x95";

/*	enum symbolChars		memoChar = symbolMemo;
	enum symbolChars		checkMark = symbolCheckMark;
	enum symbolChars		repeatSymbol = symbolRepeat;
*/
//	IndexedColorType		gColor, gBlack;//, gGray;
	RGBColorType			rgb;
 

	//	Force Display colour to black for Set Menu Key item
	if(status & StatusAsSetMenu)
		servingCourseColor = 0xAA0000;	//	Nice (arbitrary) BLUE colour!


	rgb.r = (UInt8)(servingCourseColor);
	rgb.g = (UInt8)(servingCourseColor>>8);
	rgb.b = (UInt8)(servingCourseColor>>16);

	WinSetTextColor(WinRGBToIndex(&rgb));



	numOrdered = (status >> 8) + 1;


	//	Show how many of this Dishname that have been ordered
	if((itemNumber != MarkerItem) 	&&			//	Is it a dish item?
		(numOrdered > 1)								)
	{
		StrIToA(tempStr, numOrdered);
		StrCat(tempStr, "x ");
		StrCat(tempStr, (const char*)dishPtr);
	}
	else
	{
		StrCopy(tempStr, (const char*)dishPtr);
	}


	// Determine the length of text that will fit within the bounds.
	width = bounds->extent.x - 12;
	textLen = StrLen(tempStr);
	FntCharsInWidth(tempStr, &width, &textLen, &fits);
	
	//	Set the Rectangle area to be cleared before the new bullet is drawn.
	RctSetRectangle(&space, bounds->topLeft.x + 1, bounds->topLeft.y, 10, 10);
	


	// Now draw the text from the record.
	if(itemNumber != MarkerItem)			//	Is it a dish item?
	{
		if(status & StatusAsSetMenu)
		{
			FntSetFont(symbolFont);
			WinEraseRectangle(&space, 0);
			bulletChar[0] = symbolMemo;
			WinDrawChars(bulletChar, 1, bounds->topLeft.x + 2, bounds->topLeft.y + 1);
			FntSetFont(prefs->current.fFontID);
		}
		else if(status & StatusAsSentConfirmed)
		{
			FntSetFont(symbolFont);
			WinEraseRectangle(&space, 0);
			bulletChar[0] = symbolCheckMark;
			WinDrawChars(bulletChar, 1, bounds->topLeft.x + 2, bounds->topLeft.y + 1);
			FntSetFont(prefs->current.fFontID);
		}
		else if(status & StatusHasLinkedItem)
		{
			if(prefs->current.mMenu == DrinksMenu)
			{
				FntSetFont(symbolFont);
				WinEraseRectangle(&space, 0);
				bulletChar[0] = symbolRepeat;
				WinDrawChars(bulletChar, 1, bounds->topLeft.x + 2, bounds->topLeft.y + 1);
				FntSetFont(prefs->current.fFontID);
			}
		}
		else
		{
			WinEraseRectangle(&space, 0);
			bulletChar[0] = 0x95;//bulletChr;			//	Defined in Chars.h
			WinDrawChars(bulletChar, 1, bounds->topLeft.x + 3, bounds->topLeft.y + 1);
		}
		FntSetFont(prefs->current.fFontID);

		//	Show the Dishname
		WinDrawChars(tempStr, textLen, bounds->topLeft.x + 12, bounds->topLeft.y);
	}
	else	//	This must be a Course item.
	{
		if(prefs->current.fFontID == stdFont)
		{
			FntSetFont(boldFont);
		}
		else
		{
			FntSetFont(largeBoldFont);
		}

		WinDrawChars(tempStr, textLen, bounds->topLeft.x + 1, bounds->topLeft.y);

		//	Only display the Course if it is enabled
//		if((status & StatusAsEnabled) == StatusAsEnabled)
//		{
//		}
//		else
//		{
//			FntSetFont(symbolFont);
//			bulletChar[0] = checkMark;
//			WinEraseRectangle(&space, 0);
//			WinDrawChars(bulletChar, 1, bounds->topLeft.x + 5, bounds->topLeft.y);
//			FntSetFont(prefs->current.fFontID);
//		}

	}

	// Restore current font.
	MemSet(&rgb, sizeof(rgb), 0);
	WinSetTextColor(WinRGBToIndex(&rgb));

	FntSetFont(prefs->current.fFontID);
}



/***********************************************************************
 *
 * FUNCTION:		MmOverviewDrawCheckboxInBounds
 *
 * DESCRIPTION:	Draw an item within a passed bounds
 *
 * PARAMETERS:		
 *
 * RETURNED:		Nothing.
 *
 ***********************************************************************/
static void MmOverviewDrawCheckboxInBounds(RectanglePtr bounds, UInt16 status,
														UInt8 billingAction, UInt8 courseNumber,
														MmPreferenceTypePtr prefs, UInt16 overviewPageType)
{
	char						bulletChar[] = "\x95";
	char						tempStr[4];
	RectangleType			space = {0,0,16,16};
//	enum symbolChars		memoChar = symbolMemo;
//	enum symbol11Chars	checkbox;


	//	Set the Rectangle area to be cleared before the new bullet is drawn.
	RctSetRectangle(&space, bounds->topLeft.x, bounds->topLeft.y, 16, 16);
	
	//	If this is the seat name/number (not a dish) then always show the checkbox
	if(overviewPageType == OverviewPrintReceiptsPage	&&	courseNumber == 0)
	{
		if(billingAction/* & BillingSeatMarked*/)
			bulletChar[0] = symbolCheckboxOn;
		else
			bulletChar[0] = symbolCheckboxOff;

		// Now draw the text from the record.
		FntSetFont(symbol11Font);

//		bulletChar[0] = checkbox;
		WinDrawChars(bulletChar, 1, bounds->topLeft.x, bounds->topLeft.y);
	}

	
	//	Show the checkbox if this order is marked for deletion
	if(overviewPageType == OverviewUnsentPage)
	{
		if(status & StatusAsMarkedForDelete)
			bulletChar[0] = symbolCheckboxOn;
		else
			bulletChar[0] = symbolCheckboxOff;

		// Now draw the text from the record.
		FntSetFont(symbol11Font);

//		bulletChar[0] = checkbox;
		WinDrawChars(bulletChar, 1, bounds->topLeft.x, bounds->topLeft.y);
	}


	//	NoValue8 is Flag to show that this is Billing Paid item
	else if(courseNumber == NoValue8)
	{
			if(prefs->current.fFontID == stdFont)
				FntSetFont(boldFont);
			else
				FntSetFont(largeBoldFont);
	
			tempStr[2] = 0;
			tempStr[1] = '$';
			tempStr[0] = ' ';

			//	Display the $ to show that items will be deleted after billing
			WinDrawChars(tempStr, StrLen(tempStr), bounds->topLeft.x, bounds->topLeft.y);
	}
/*
	//	courseNumber != 0
	else
	{
		if(prefs->current.fFontID == stdFont)
			FntSetFont(boldFont);
		else
			FntSetFont(largeBoldFont);

		billingAction &= ~BillingSeatBilled;					//	Ensure MSB is turned off
		if(billingAction)
		{
			StrIToA(tempStr, billingAction);

			if(StrLen(tempStr) < 2)
			{
				tempStr[2] = 0;
				tempStr[1] = tempStr[0];
				tempStr[0] = ' ';
			}

			//	Display the Group Number
			WinDrawChars(tempStr, StrLen(tempStr), bounds->topLeft.x, bounds->topLeft.y);
		}
	}
*/	
	FntSetFont(prefs->current.fFontID);
}


/***********************************************************************
 *
 * FUNCTION:		MmOverviewDrawRecordInBounds
 *
 * DESCRIPTION:	Draw an item within a passed bounds
 *
 * PARAMETERS:		
 *
 * RETURNED:		Nothing.
 *
 ***********************************************************************/
static void MmOverviewDrawRecordInBounds(DBTypePtr dBs, UInt8 menuType, Int16 tableRow, Char *dishPtr,
												RectanglePtr bounds, UInt16 itemNumber, UInt16 status, 
													UInt32 servingCourseColor, 
														MmPreferenceTypePtr prefs, UInt16 overviewPageType)
{
	UInt16					numOrdered;
	Int16						textLen, width;
	Boolean					fits = true;
	char						tempStr[MaxInfoNameSize + MaxMenuNameSize +2];
	char						bulletChar[2] = "";
	RectangleType			space = {0,0,10,10};
	RectangleType			trim = {0,0,4,10};
	RGBColorType			rgb;
	RGBColorType			rgbGrey;
 



	//	Force Display colour to black for Set Menu Key item
	if(status & StatusAsSetMenu)
		servingCourseColor = 0xAA0000;	//	Nice (arbitrary) BLUE colour!


	rgb.r = (UInt8)(servingCourseColor);
	rgb.g = (UInt8)(servingCourseColor>>8);
	rgb.b = (UInt8)(servingCourseColor>>16);

	WinSetTextColor(WinRGBToIndex(&rgb));

		

	numOrdered = (status >> 8) + 1;

	//	Show how many of this Dishname that have been ordered
	if((itemNumber != MarkerItem) 	&&			//	Is it a dish item?
		(numOrdered > 1)								)
	{
		StrIToA(tempStr, numOrdered);
		StrCat(tempStr, "x ");
		StrCat(tempStr, (const char *)dishPtr);
	}
	else
	{
		StrCopy(tempStr, (const char *)dishPtr);
	}

//ShowDebug("Item is %s", tempStr);						

	//	Set the Rectangle area to be cleared before the new bullet is drawn.
	RctSetRectangle(&space, bounds->topLeft.x + 5, bounds->topLeft.y, 10, 10);
	


	// Now draw the text from the record.
	if(itemNumber != MarkerItem	&& itemNumber != NoValue16)		//	Is it a dish item?
	{
		// Determine the length of text that will fit within the bounds, minus room for bullet.
		width = bounds->extent.x - 10;
		textLen = StrLen(tempStr);
		FntCharsInWidth(tempStr, &width, &textLen, &fits);

		
		//	First draw the appropriate bullet
		if(status & StatusAsSetMenu)
		{
//ShowDebug("StatusAsSetMenu");						
			if(menuType == DishesMenu)
			{
				FntSetFont(symbolFont);
				WinEraseRectangle(&space, 0);
				bulletChar[0] = symbolMemo;
				WinDrawChars(bulletChar, 1, bounds->topLeft.x + 1, bounds->topLeft.y + 1);
				FntSetFont(prefs->current.fFontID);
			}
		}
		else if(status & StatusAsSentConfirmed)
		{
			WinEraseRectangle(&space, 0);
			FntSetFont(symbolFont);
			bulletChar[0] = symbolCheckMark;
			WinDrawChars(bulletChar, 1, bounds->topLeft.x + 1, bounds->topLeft.y + 1);
			FntSetFont(prefs->current.fFontID);
		}
		else if(status & StatusHasLinkedItem)
		{
			if(menuType == DrinksMenu)
			{
				WinEraseRectangle(&space, 0);
				FntSetFont(symbolFont);
				bulletChar[0] = symbolRepeat;
				WinDrawChars(bulletChar, 1, bounds->topLeft.x + 1, bounds->topLeft.y + 1);
				FntSetFont(prefs->current.fFontID);
			}
		}
		else
		{
			WinEraseRectangle(&space, 0);
			bulletChar[0] = 0x95;//bulletChr;			//	Defined in Chars.h
			WinDrawChars(bulletChar, 1, bounds->topLeft.x + 2, bounds->topLeft.y + 1);
		}


		//	Draw the item name
		FntSetFont(prefs->current.fFontID);
		WinDrawChars(tempStr, textLen, bounds->topLeft.x + 10, bounds->topLeft.y);
	}





	//	This must be a Serving Course.
	else if(itemNumber == NoValue16)
	{
		if(prefs->current.fFontID == stdFont)
		{
			FntSetFont(boldFont);
		}
		else
		{
			FntSetFont(largeBoldFont);
		}

		// Determine the length of text that will fit within the bounds.
		width = bounds->extent.x - 4;
		textLen = StrLen(tempStr);
		FntCharsInWidth(tempStr, &width, &textLen, &fits);

		//	Display the Serving Course
		if(prefs->current.fFontID == stdFont)
			FntSetFont(boldFont);
		else
			FntSetFont(largeBoldFont);

		WinDrawChars(tempStr, textLen, bounds->topLeft.x + 2, bounds->topLeft.y);
	}






	//	This must be a Course item so show Seat and Number.
	else
	{
		if(prefs->current.fFontID == stdFont)
			FntSetFont(boldFont);
		else
			FntSetFont(largeBoldFont);



		//	Insert Left and/or Right Arrows as appropriate
		if(	tableRow == 0	
			&&
				(	overviewPageType == OverviewTakeOrders
				 ||
					overviewPageType == OverviewOrdersOnlyPage) )
		{
			UInt16		tableNo = 0;		//	Use CurrentTableNumber
			UInt16		seatNo;
//			UInt16		leftOffset = 0;
			UInt16		displayPosX;
			Boolean		orderFound;
			

			// Determine the length of text that will fit within the bounds.
			width = bounds->extent.x - 4;
			textLen = StrLen(tempStr);
//			if(width > OverviewCol1WidthChkBoxes - OverviewCol0Width - OverviewCol0Width)
//				width = OverviewCol1WidthChkBoxes - OverviewCol0Width - OverviewCol0Width;

			FntCharsInWidth(tempStr, &width, &textLen, &fits);

//			displayPosX = (OverviewCol1WidthChkBoxes/2) - (width/2);
			displayPosX = bounds->topLeft.x + (bounds->extent.x / 2) - (width/2);

			MmFindFirstSeatOrder(dBs, &prefs->current, &orderFound, &tableNo, &seatNo);

			//	Draw the Left Arrow
			if(	(prefs->current.seatNo > 1)
				||
					seatNo < prefs->current.seatNo
				||
					(orderFound == false && prefs->current.seatNo > 1)	)
			{
				FntSetFont(symbolFont);
				bulletChar[0] = symbolLeftArrow;
				WinDrawChars(bulletChar, 1, bounds->topLeft.x, bounds->topLeft.y + 1);
			}
			else
			{
				rgbGrey.r = (UInt8)(170);
				rgbGrey.g = (UInt8)(170);
				rgbGrey.b = (UInt8)(170);

				WinSetTextColor(WinRGBToIndex(&rgbGrey));

				FntSetFont(symbolFont);
				bulletChar[0] = symbolLeftArrow;
				WinDrawChars(bulletChar, 1, bounds->topLeft.x, bounds->topLeft.y + 1);

				// Restore current font.
				MemSet(&rgb, sizeof(rgb), 0);
				WinSetTextColor(WinRGBToIndex(&rgb));
			}

			

			//	Display the Seat Number and Name
			if(prefs->current.fFontID == stdFont)
				FntSetFont(boldFont);
			else
				FntSetFont(largeBoldFont);
			WinDrawChars(tempStr, textLen, displayPosX, bounds->topLeft.y);



			//	Draw the Right Arrow
//			prefs->icon2Position = OverviewCol1WidthChkBoxes - OverviewCol0Width;
			prefs->icon2Position = OverviewCol1Width;

			FntSetFont(symbolFont);
			bulletChar[0] = symbolRightArrow;
//			WinDrawChars(bulletChar, 1, bounds->topLeft.x + prefs->icon2Position, bounds->topLeft.y + 1);
			WinDrawChars(bulletChar, 1, OverviewBillingColPos, bounds->topLeft.y + 1);
		}
		else
		{
			// Determine the length of text that will fit within the bounds.
			width = bounds->extent.x - 4;
			textLen = StrLen(tempStr);
			FntCharsInWidth(tempStr, &width, &textLen, &fits);

			//	Display the Seat Number and Name
			WinDrawChars(tempStr, textLen, (bounds->topLeft.x + 2), bounds->topLeft.y);
		}
	}

	// Restore current font.
	MemSet(&rgb, sizeof(rgb), 0);
	WinSetTextColor(WinRGBToIndex(&rgb));

	FntSetFont(prefs->current.fFontID);
}




/***********************************************************************
 *
 * FUNCTION:		MmOverviewDrawIconInBounds
 *
 * DESCRIPTION:	Draw an item within a passed bounds
 *
 * PARAMETERS:		
 *
 * RETURNED:		Nothing.
 *
 ***********************************************************************/
static void MmOverviewDrawIconInBounds(RectanglePtr bounds, UInt16 itemNumber, UInt16 status, 
														Boolean collapsedIcon, UInt32 servingCourseColor, 
															MmPreferenceTypePtr prefs, UInt16 overviewPageType)
{
	char						bulletChar[2] = "";
	RectangleType			space = {0,0,10,10};
	RectangleType			trim = {0,0,4,10};
/*	enum symbol11Chars	leftArrow = symbol11LeftArrow;
	enum symbol11Chars	right11Arrow = symbol11RightArrow;

	enum symbolChars		leftArrow = symbolLeftArrow;
	enum symbolChars		rightArrow = symbolRightArrow;
	enum symbolChars		memoChar = symbolMemo;
	enum symbolChars		checkMark = symbolCheckMark;
	enum symbolChars		repeatSymbol = symbolRepeat;
*/
//	IndexedColorType		gColor, gBlack, gGrey;
	RGBColorType			rgb;
 



	//	Force Display colour to black for Set Menu Key item
	if(status & StatusAsSetMenu)
		servingCourseColor = 0xAA0000;	//	Nice (arbitrary) BLUE colour!


	rgb.r = (UInt8)(servingCourseColor);
	rgb.g = (UInt8)(servingCourseColor>>8);
	rgb.b = (UInt8)(servingCourseColor>>16);

	WinSetTextColor(WinRGBToIndex(&rgb));

		


	//	This must be a Serving Course.
	if(itemNumber == NoValue16)
	{
		//	Insert Right pointing Arrow as appropriate
		if(overviewPageType == OverviewTakeOrders)
		{
			if(collapsedIcon)
			{
				FntSetFont(symbol11Font);
				bulletChar[0] = symbol11RightArrow;
				WinDrawChars(bulletChar, 1, bounds->topLeft.x, bounds->topLeft.y + 1);

				//	Set the Rectangle area to trim the new bullet which is too large!
				RctSetRectangle(&space, bounds->topLeft.x, bounds->topLeft.y, 3, 12);
				WinEraseRectangle(&space, 0);
			}
		}
	}

	//	This must be a Course item (with Seat and Number).
	else
	{
		//	Insert Right pointing Arrow as appropriate
		if(overviewPageType == OverviewTakeOrders)
		{
			if(collapsedIcon)
			{
				FntSetFont(symbol11Font);
				bulletChar[0] = symbol11RightArrow;
				WinDrawChars(bulletChar, 1, bounds->topLeft.x + 1, bounds->topLeft.y + 1);
			}
			else
			{
				FntSetFont(symbol7Font);
				bulletChar[0] = symbol7ScrollDown;
				WinDrawChars(bulletChar, 1, bounds->topLeft.x + 1, bounds->topLeft.y + 4);

				//	Set the Rectangle area to trim the new bullet which is too large!
				RctSetRectangle(&space, bounds->topLeft.x, bounds->topLeft.y, OverviewCol0Width, 6);
				WinEraseRectangle(&space, 0);
			}
		}
	}




/*
	//	This must be a Course item so show Seat and Number.
	else
	{
		if(prefs->current.fFontID == stdFont)
			FntSetFont(boldFont);
		else
			FntSetFont(largeBoldFont);



		//	Insert Left and/or Right Arrows as appropriate
		if(	tableRow == 0	
			&&
				(	overviewPageType == OverviewTakeOrders
				 ||
					overviewPageType == OverviewOrdersOnlyPage) )
		{
			UInt16		tableNo = 0;		//	Use CurrentTableNumber
			UInt16		seatNo;
			UInt16		leftOffset = 0;
			UInt16		displayPosX;
			Boolean		orderFound;
			

			// Determine the length of text that will fit within the bounds.
			width = bounds->extent.x - OverviewCol0Width - 4;
			textLen = StrLen(tempStr);
			if(width > OverviewCol1WidthChkBoxes - OverviewCol0Width - OverviewCol0Width)
				width = OverviewCol1WidthChkBoxes - OverviewCol0Width - OverviewCol0Width;

			FntCharsInWidth(tempStr, &width, &textLen, &fits);

			displayPosX = (OverviewCol1WidthChkBoxes/2) - (width/2);

			MmFindFirstSeatOrder(dBs, &prefs->current, &orderFound, &tableNo, &seatNo);

			leftOffset = OverviewCol0Width;

			//	Draw the Left Arrow
			if(	(prefs->current.seatNo > 1)
				||
					seatNo < prefs->current.seatNo
				||
					(orderFound == false && prefs->current.seatNo > 1)	)
			{
				FntSetFont(symbolFont);
				bulletChar[0] = symbolLeftArrow;
				WinDrawChars(bulletChar, 1, bounds->topLeft.x + 1, bounds->topLeft.y + 1);
			}
			else
			{
				rgbGrey.r = (UInt8)170;
				rgbGrey.g = (UInt8)170;
				rgbGrey.b = (UInt8)170;

				WinSetTextColor(WinRGBToIndex(&rgbGrey));

				FntSetFont(symbolFont);
				bulletChar[0] = symbolLeftArrow;
				WinDrawChars(bulletChar, 1, bounds->topLeft.x + 1, bounds->topLeft.y + 1);

				// Restore current font.
				MemSet(&rgb, sizeof(rgb), 0);
				WinSetTextColor(WinRGBToIndex(&rgb));
			}

			

			//	Display the Seat Number and Name
			if(prefs->current.fFontID == stdFont)
				FntSetFont(boldFont);
			else
				FntSetFont(largeBoldFont);
//			WinDrawChars(tempStr, textLen, (bounds->topLeft.x + leftOffset + 2), bounds->topLeft.y);
			WinDrawChars(tempStr, textLen, displayPosX, bounds->topLeft.y);



			//	Draw the Right Arrow
//			prefs->icon2Position = bounds->topLeft.x + leftOffset + width + 4;
			prefs->icon2Position = OverviewCol1WidthChkBoxes - OverviewCol0Width;

			FntSetFont(symbolFont);
			bulletChar[0] = symbolRightArrow;
//			WinDrawChars(bulletChar, 1, bounds->topLeft.x + prefs->icon2Position, bounds->topLeft.y + 1);
			WinDrawChars(bulletChar, 1, OverviewCol1WidthChkBoxes - OverviewCol0Width, bounds->topLeft.y + 1);
		}
		else
		{
			// Determine the length of text that will fit within the bounds.
			width = bounds->extent.x - 4;
			textLen = StrLen(tempStr);
			FntCharsInWidth(tempStr, &width, &textLen, &fits);

			//	Display the Seat Number and Name
			WinDrawChars(tempStr, textLen, (bounds->topLeft.x + 2), bounds->topLeft.y);
		}
	}
*/
	// Restore current font.
	MemSet(&rgb, sizeof(rgb), 0);
	WinSetTextColor(WinRGBToIndex(&rgb));

	FntSetFont(prefs->current.fFontID);
}




/***********************************************************************
 *
 * FUNCTION:		MmOverviewDrawTableOrSeatnames
 *
 * DESCRIPTION:	Draw a Tablename or Seatname within a passed bounds
 *
 * PARAMETERS:		
 *
 * RETURNED:		Nothing.
 *
 ***********************************************************************/
static void MmOverviewDrawTableOrSeatnames(DBTypePtr dBs, UInt8 menuType, Int16 tableRow, 
											RectanglePtr bounds, UInt16 status,
												UInt16 tableNumber, UInt16 seatNumber, 
													Int32 pricePOS, UInt16 overviewPageType, 
														 Char* *listItems, MmPreferenceTypePtr prefs )
{
	MemHandle				recHandle;
	MemHandle				tableRecHandle;

	UInt16					recTablenameIndex;
	UInt16					recSeatnameIndex;
	SeatnameTypePtr		recSeatnamePtr;
	SeatnameTypePtr		recTablenamePtr;
	
	UInt32					servingCourseColor = 0;

	char						tempStr[MaxSeatNameSize+23];
	char						tempStr2[MaxSeatNameSize+23];
	char						numberStr[20];
	char						numberStrTable[MaxTableNameSize];


	StrCopy(tempStr, "");

	if(overviewPageType == OverviewUnsentPage)
	{
		StrIToA(numberStr, tableNumber);
		StrCopy(tempStr, numberStr);

		MmItemToString(tableNumber, numberStrTable, false, listItems);

		if(MmGetTableNameIndex(dBs->mmTablenameDB, numberStrTable, &recTablenameIndex))
		{
			tableRecHandle = DmQueryRecord(dBs->mmTablenameDB, recTablenameIndex);
			if(tableRecHandle)
			{
				recTablenamePtr = MemHandleLock(tableRecHandle);

				StrCat(tempStr, ". ");
				StrNCat(tempStr, recTablenamePtr->name, MaxTableLabelSize+4);	//	 Allow for "xx. "

				MemPtrUnlock(recTablenamePtr);
			}
		}
		StrCat(tempStr, ":  ");
	}

	//	If there's a seatname, use that
	if(MmGetSeatnameIndexAndUpdate(dBs, tableNumber/*prefs->current.tableNo*/,
			seatNumber, &recSeatnameIndex, numberStr, DontMarkForDelete))
	{
		StrIToA(numberStr, seatNumber);
		StrCopy(tempStr2, numberStr);
		StrCat(tempStr2, ". ");
		recHandle = DmQueryRecord(dBs->mmSeatnameDB, recSeatnameIndex);
		if(recHandle)
		{
			recSeatnamePtr = MemHandleLock(recHandle);
			StrCat(tempStr2, recSeatnamePtr->name);
			MemPtrUnlock(recSeatnamePtr);

		}
	}
	else	//	No Seatname for this seat, so use default. Failing that, show something anyway
	{
		if(StrLen(prefs->defaultSeatname))
		{
			StrNCopy(tempStr2, prefs->defaultSeatname, MaxSeatLabelSize);	//	Limit size to button width
		}
		else
		{
			StrCopy(tempStr2, "Seat");
		}

		StrCat(tempStr2, " ");
		StrIToA(numberStr, seatNumber);
		StrCat(tempStr2, numberStr);
	}

	
	StrCat(tempStr, tempStr2);

	//	Display any seat price correction from other menus on the POS
	if(pricePOS)
	{
		StrCat(tempStr, " (");
		MmFormatPriceString(pricePOS, LeftAlign, numberStr, 0);
		StrCat(tempStr, numberStr);
		StrCat(tempStr, ")");
	}

	MmOverviewDrawRecordInBounds(dBs, menuType, tableRow, tempStr, bounds, MarkerItem,
												status, servingCourseColor, prefs, overviewPageType);
}



/***********************************************************************
 *
 * FUNCTION:		MmOverviewDrawRecord2
 *
 * DESCRIPTION:	Draw an item in the Orders form's table.  This 
 *						routine is called from the table object and must match
 *						the parameters that the table object passes.  The routine
 *						OrdersViewLoadRecords sets the table object to call this
 *						routine.  The table object calls it once for every table 
 *						cell needing drawing.
 *
 * PARAMETERS:		tableP - table in which to draw the record
 *						tableRow - row of the record to change
 *						column - column of the record to change
 *						bounds - bounds in which to draw the record
 *
 * RETURNED:		Nothing.
 *
 ***********************************************************************/
void MmOverviewDrawRecord2(DBTypePtr dBs, UInt16 overviewTopRowNum, Int16 tableRow, Int16 column, 
									RectanglePtr bounds, 
									UInt16 overviewPageType, UInt16 overviewCheckboxes, UInt16 overviewExpandedStatus, 
									TableDisplayType *displayRowsPtr, 
									UInt16 *sidesIndexNum, Char* *listItems,
									MmPreferenceTypePtr prefs)
{
	MemHandle				recHandle;
	OrderTypePtr			recOrderPtr;
	MenuTypePtr				recMenuPtr;
	SizeTypePtr				recSizePtr;
	ServingCourseTypePtr	recServingCoursePtr;
	UInt16					recDishIndex;

	UInt16					recIndex;
	
	char						tempStr[MaxSeatNameSize+23];
	char						tempStr2[10];

	UInt8						billingAction;
	
	UInt16					row;								//	overviewTopRowNum + tableRow
	
	UInt16					tableNumber;
	UInt16					seatNumber;
	UInt16					status;
	UInt16					servingCourseIndex;
	UInt32					servingCourseColor;
	Boolean					selectable;
//	Boolean					showCollapsed;
	UInt32					setMenuMask;
	Int32						pricePOS;

	UInt8						menuType;

	DmOpenRef				menuDB;
	DmOpenRef				menuSizesDB;

	
	row = overviewTopRowNum + tableRow;
	



	//	If its the skinny icons column at left of screen
	if(column == Column0)
	{
		tempStr[0] = 0;
		
		// Get the data values that relate to the table item to draw.
		// The values are stored in the "intValue" field of the item.

		//	Order Menu Index.
		//	If 0 then recIndex = Serving Course Index
		//	If NoValue16 then recIndex = SeatNumber
		recDishIndex = displayRowsPtr[row].dishIndex;

		//	Order Index, Serving Course Index, or SeatNumber as above
		recIndex = displayRowsPtr[row].recIndex;

		status = displayRowsPtr[row].status;
		



		if(displayRowsPtr[row].rowDisplayType == RowIsServingCourse)
		{
			MmGetServingCourseIndex(dBs->mmServingCourseDB, recIndex,
												&servingCourseIndex, &servingCourseColor, &selectable);

			recHandle = DmQueryRecord(dBs->mmServingCourseDB, servingCourseIndex);
			if(recHandle)
			{
				recServingCoursePtr = MemHandleLock(recHandle);
				StrCopy(tempStr, recServingCoursePtr->standardName);		//	Show it
				MemPtrUnlock(recServingCoursePtr);
			}
			
			MmOverviewDrawIconInBounds(bounds, NoValue16, 0, displayRowsPtr[row].showCollapsed, 
													servingCourseColor, prefs, overviewPageType);
		}

		else if(	displayRowsPtr[row].rowDisplayType == RowIsSeat)
		{
			if(overviewExpandedStatus == StatusIsExpanded)
			{
				MmOverviewDrawIconInBounds(bounds, 0, 0, false, 0, prefs, overviewPageType);
			}
			else// if(overviewExpandedStatus == StatusIsExpanded)
			{
				MmOverviewDrawIconInBounds(bounds, 0, 0, true, 0, prefs, overviewPageType);
			}
		}
	}


	if(column == Column1)
	{
		tempStr[0] = 0;
		
		// Get the data values that relate to the table item to draw.
		// The values are stored in the "intValue" field of the item.

		//	Order Menu Index.
		//	If 0 then recIndex = Serving Course Index
		//	If NoValue16 then recIndex = SeatNumber
		recDishIndex = displayRowsPtr[row].dishIndex;

		//	Order Index, Serving Course Index, or SeatNumber as above
		recIndex = displayRowsPtr[row].recIndex;

		status = displayRowsPtr[row].status;
		



		if(displayRowsPtr[row].rowDisplayType == RowIsSeat)
		{
			//	Do Table/Seat stuff
			MmOverviewDrawTableOrSeatnames(dBs, DishesMenu, tableRow, bounds, status,
														displayRowsPtr[row].tableNumber, displayRowsPtr[row].seatNumber,
															0, overviewPageType, listItems, prefs);
		}
		


		
		
		else if(displayRowsPtr[row].rowDisplayType == RowIsServingCourse)
		{
			MmGetServingCourseIndex(dBs->mmServingCourseDB, recIndex,
												&servingCourseIndex, &servingCourseColor, &selectable);

			recHandle = DmQueryRecord(dBs->mmServingCourseDB, servingCourseIndex);
			if(recHandle)
			{
				recServingCoursePtr = MemHandleLock(recHandle);
				StrCopy(tempStr, recServingCoursePtr->standardName);		//	Show it
				MemPtrUnlock(recServingCoursePtr);
			}
			
			MmOverviewDrawRecordInBounds(dBs, DishesMenu, tableRow, tempStr, bounds, NoValue16,
														0/*, displayRowsPtr[row].showCollapsed*/, servingCourseColor, 
															prefs, overviewPageType);
			
			//	Identify where we are on the table so we can draw a line to show its start of new menu
			if(displayRowsPtr[row].drawLine	&&	overviewPageType == OverviewTakeOrders)
				WinDrawLine(0, bounds->topLeft.y, OverviewUsableScreenWidth, bounds->topLeft.y);
		}





		else	//	Must be an Order or Side or Set Menu
		{
//ShowDebug("Must be an Order or Side or Set Menu");						
			//	First get some basic data from the order
			recHandle = DmQueryRecord(dBs->mmOrdersDB, recIndex);
			if(recHandle)
			{
				recOrderPtr = MemHandleLock(recHandle);
				if(recOrderPtr->tableNumber == LastTableRecord)
				{
					menuType = DishesMenu;
					setMenuMask = 0;
					tableNumber = prefs->current.tableNo;
					seatNumber = prefs->current.seatNo;
					pricePOS = 0;
					servingCourseIndex = NoValue16;
				}
				else
				{
					menuType = recOrderPtr->menuType;
					setMenuMask = recOrderPtr->setMenuMask;
					tableNumber = recOrderPtr->tableNumber;
//						if(recDishIndex == NoValue16)
						seatNumber = displayRowsPtr[row].seatNumber;
//						else
//							seatNumber = recOrderPtr->seatNumber;
					
					if(recOrderPtr->price == 0 || recOrderPtr->price == NoValue32Signed)
					{
						pricePOS = 0;
					}
					else
					{
						pricePOS = recOrderPtr->price;
					}
					
					MmGetServingCourseIndex(dBs->mmServingCourseDB, recOrderPtr->servingCourseID,
														&servingCourseIndex, &servingCourseColor, &selectable);
				}


				
				
/*				
				//	Do Table/Seat stuff if its a Course
				if(recOrderPtr->itemNumber == 0)
				{
					MmOverviewDrawTableOrSeatnames(dBs, menuType, tableRow, bounds, status, 
																	tableNumber, seatNumber, pricePOS, 
																		overviewPageType, listItems, prefs);
				}
			
		
				//	Otherwise it must be a Dish/Drink
				else if(recDishIndex != NoValue16)
*/				{
					if(menuType == DishesMenu)
					{
						menuDB = dBs->mmDishesDB;
						menuSizesDB = dBs->mmDishesSizesDB;
					}
					else
					{
						menuDB = dBs->mmDrinksDB;
						menuSizesDB = dBs->mmDrinksSizesDB;
					}
		
			
					recHandle = DmQueryRecord(menuDB, recDishIndex & ~IsDrinkMask);
					if(recHandle)
					{
						recMenuPtr = MemHandleLock(recHandle);
			
						if(column == Column1 && recMenuPtr->itemNumber != 0)	//	If its a Dish
						{
							//	Its a Food item
							if(menuType == DishesMenu)
							{
								if(status & StatusHasLinkedItem)
								{
									(*sidesIndexNum)++;			//	Count Sides so we know which size to show
									StrCopy(tempStr, DisplaySidesIcon);
									StrCat(tempStr, " ");
								}
								else if(	(setMenuMask)	/*&&
											!(setMenuMask & SetMenuKey)*/	)
								{
									*sidesIndexNum = 0;
									StrIToA(tempStr2, recOrderPtr->setMenuGroup & ResetMSB16);
									StrCopy(tempStr, "[");
									StrCat(tempStr, tempStr2);
									StrCat(tempStr, "]");
									StrCat(tempStr, " ");
//ShowDebug("setMenuGroup %s", tempStr);						
								}
								else
								{
									*sidesIndexNum = 0;
								}
								recHandle = DmQueryRecord(menuSizesDB,
														recOrderPtr->menuIndex[*sidesIndexNum].size);
							}
							else	//	Must be a drink
							{
								//	If it has been sent, display Mix
								if(status & StatusAsSentConfirmed)
								{
									if(status & StatusHasLinkedItem)
									{
										StrCopy(tempStr, DisplayDrinkMixIcon);
										StrCat(tempStr, " ");
									}
								}
								recHandle = DmQueryRecord(menuSizesDB, recOrderPtr->size);
							}
				
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
		
							StrCat(tempStr, recMenuPtr->itemName);	//	Show it
						}
						MmOverviewDrawRecordInBounds(dBs, menuType, tableRow, tempStr, bounds, 
																recMenuPtr->itemNumber, status, servingCourseColor, 
																	prefs, overviewPageType);
			
						MemPtrUnlock(recMenuPtr);
					}
				}
				MemPtrUnlock(recOrderPtr);
			}
		}
	}


	//	Handle Checkboxes in Billing and Unsent pages
	else
	{
		if(overviewCheckboxes == OverviewCheckboxesBilling)
		{
			billingAction = displayRowsPtr[row].billing;

			recIndex = displayRowsPtr[row].recIndex;
		
			recHandle = DmQueryRecord(dBs->mmOrdersDB, recIndex);
			if(recHandle)
			{
				recOrderPtr = MemHandleLock(recHandle);
				
				MmOverviewDrawCheckboxInBounds(bounds, recOrderPtr->status, billingAction, 
															recOrderPtr->courseNumber, prefs, overviewPageType);
				
				MemPtrUnlock(recOrderPtr);
			}
		}
		else if(overviewCheckboxes == OverviewCheckboxesUnsent)
		{
			if(displayRowsPtr[row].dishIndex != NoValue16)
			{
				if(displayRowsPtr[row].rowDisplayType != RowIsSetMenuKey)
				{
					recIndex = displayRowsPtr[row].recIndex;
				
					recHandle = DmQueryRecord(dBs->mmOrdersDB, recIndex);
					if(recHandle)
					{
						recOrderPtr = MemHandleLock(recHandle);
						
						if(recOrderPtr->courseNumber != 0)
						{
							MmOverviewDrawCheckboxInBounds(bounds, recOrderPtr->status,
																		0, recOrderPtr->courseNumber,
																			prefs, overviewPageType);
						}
						MemPtrUnlock(recOrderPtr);
					}
				}
			}
		}
	}
}


/***********************************************************************
 *
 * FUNCTION:		MmOverviewDisplayTablenameInTitle
 *
 * DESCRIPTION:	Display Table name or number in Title of Overview form.
 *
 * PARAMETERS:		
 *
 * RETURNED:		
 *
 ***********************************************************************/
void MmOverviewDisplayTablenameInTitle(FormPtr frm,
										MmPreferenceTypePtr prefs, UInt16 overviewPageType,
										DmOpenRef mmTablenameDB)
{
	Char						numberStr[4];
	Char						tempStr[MaxSeatNameSize+1];

	SeatnameTypePtr		recPtr;
	UInt16					recIndex;
	MemHandle 				recHandle;
	UInt16					numRecords;

	Int16						width;
	Int16						textLen;
	Boolean					fits = true;
	FontID					currentFont;


	currentFont = FntGetFont();

	if(overviewPageType == OverviewUnsentPage)
	{
		StrCopy(tempStr, "All  Unsent");
	}
	else
	{
		StrCopy(tempStr, "Table ");

		StrIToA(numberStr, prefs->current.tableNo);
		StrCat(tempStr, numberStr);

		//	If a table name exists for this table write it into the label
		numRecords = DmNumRecords(mmTablenameDB);
		
		for(recIndex=0; recIndex < numRecords; recIndex++)
		{
			recHandle = DmQueryRecord(mmTablenameDB, recIndex);
			if(recHandle)										//	Not all tables have names
			{
				recPtr = MemHandleLock(recHandle);
				if(recPtr->tableNumber == prefs->current.tableNo)
				{
					StrCopy(tempStr, recPtr->name);
					MemPtrUnlock(recPtr);
					break;
				}
				else
				{
					MemPtrUnlock(recPtr);
				}
			}
		}
	}

	//	Scale to fit the max available Title space
	FntSetFont(largeFont);
	width = 68;
	textLen = StrLen(tempStr);						//	Untrimmed length
	FntCharsInWidth(tempStr, &width, &textLen, &fits);
	tempStr[textLen] = 0;							//	Terminate the string

	FrmCopyTitle(frm, tempStr);

	FntSetFont(currentFont);
}



/***********************************************************************
 *
 * FUNCTION:    MmMainviewLoadTableArrayLoop
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
void MmMainviewLoadTableArrayLoop(DmOpenRef *menuDB, DmOpenRef *ordersDB,
						UInt16 overviewPageType, TableDisplayType *tableDisplayRowsPtr, 
							UInt16 currentMenu, UInt16 recOrderIndex,
								UInt16 currentTableNumber, UInt16 currentSeatNumber,
									UInt16 *lastItemOrdered, Int16 *mainviewTopRowNum,
										UInt16 rowsInMainTable)
{
	Int16					row = 0;						//	Tallies the rows loaded for display
	Int16					entityCounter;				//	Counts through the Course, Dish & Side items
	Int16					countOffset = 0;			//	Counts the reduction in rows due to multiple items


	UInt16				thisRow;
	UInt16				thisDishCounter;
	Boolean				noFrills;
	UInt16				numRecords;
	UInt16				thisCourse;
	UInt16				firstDish = true;			//	First dish always displays
	UInt16				firstHadFrills = false;
	UInt16				status;
	
	UInt16				i;

	UInt16				recIndex = 0;
	MemHandle			recHandle;
	UInt16				currentCourse;

	UInt16				recMenuIndex = 0;
	MenuTypePtr			recMenuPtr;

	OrderTypePtr		recOrderPtr=0;
		
	UInt8					enabled;
	
	
	
	overviewPageType = 0;					//	Stop compiler grizzle!!!!!!!!!!!!!!!!!
	
	
	row = 0;
	entityCounter = 0;
	numRecords = DmNumRecords(*ordersDB);
	currentCourse = MarkerItem;
	
	
	while(recOrderIndex < numRecords && row < MaxTableDisplayRows)
	{
		if(currentCourse == MarkerItem)	//	Course item so do Course display
		{
			// Get the next record in the current category. (recNumber -> new record index).
			recHandle = DmQueryNextInCategory (*menuDB, &recMenuIndex, MarkerItem);

			//	Store the record number in the table item,
			if(recHandle)
			{
				recMenuPtr = MemHandleLock(recHandle);
				thisCourse = recMenuPtr->courseNumber;
				enabled = recMenuPtr->enabled;
				MemPtrUnlock(recMenuPtr);

				//	Previous page got countOffset items further down the list because
				//	of multiple items displayed per row
				if(entityCounter - countOffset >= 0)
				{
					//	Write the seat number to the display image
					tableDisplayRowsPtr[row].seatNumber = currentSeatNumber;
					
					//	Preserve the Index into the Menu
					tableDisplayRowsPtr[row].dishIndex = recMenuIndex;	//	Save recNum so we can find item

					//	Default this so it is identifiable as a Course
					tableDisplayRowsPtr[row].recIndex = NoValue16;

					tableDisplayRowsPtr[row].menuType = 0;

					//	Preserve the enabled status
					tableDisplayRowsPtr[row].status = enabled;

					tableDisplayRowsPtr[row].billing = 0;

//					displayRowsPtr++;
					row++;
				}
				currentCourse = NoValue16;	// Make sure we see the dishes
				recMenuIndex++;
				entityCounter++;
			}
			else	//	if(recHandle)
			{
				break;										//	We've reached the end of the Menu
			}
		}
	
		else	//	Its not a Course record but a Dish record
		{
			firstDish = true;

			//	This is a marker record (Course/Dish = 0)
			recHandle = DmQueryRecord(*ordersDB, recOrderIndex);
			if(recHandle)
			{
				recOrderPtr = MemHandleLock(recHandle);
				MemPtrUnlock(recOrderPtr);
				if(recOrderPtr->tableNumber != LastTableRecord)		//	Check its not the End Marker record.
				{
					recOrderIndex++;							//	Skip marker record, but display next.
					recHandle = DmQueryRecord(*ordersDB, recOrderIndex);


					//	Display all the records (Dishes or Drinks) of this course.
					while(recHandle)
					{
						recOrderPtr = MemHandleLock (recHandle);

						//	Don't display Dishes on Drinks page, or Drinks on Dishes page
						if((currentMenu==DishesMenu && recOrderPtr->menuType!=DishesMenu) ||
							(currentMenu!=DishesMenu && recOrderPtr->menuType==DishesMenu)	)
						{
							recOrderIndex++;					//	Try the next one
							recHandle = DmQueryRecord(*ordersDB, recOrderIndex);
							if(!recHandle)						//	Oops. Went past the end of the DB
								recOrderIndex--;
						}

						//	This is where it really happens
						else
						{
							if(recOrderPtr->tableNumber	== currentTableNumber	&&
								recOrderPtr->seatNumber		== currentSeatNumber		&&
								recOrderPtr->courseNumber	== thisCourse					)
							{
								if(recOrderPtr->itemCounter == 0)
								{
									thisDishCounter = 0;
									thisRow = 0;						//	==0 means no 'noFrills' orders yet
									firstDish = true;					//	Always display first one
									firstHadFrills = false;
								}

								//	Is this a plain vanilla order without options etc?
								if((recOrderPtr->options		==	0x80)	&&
									(recOrderPtr->price			==	0)		&&
									(recOrderPtr->noteLength	== 0)		&&
									(recOrderPtr->setMenuMask	== 0)		&&
									(recOrderPtr->menuIndex[1].index == NoValue16)	)
								{
									noFrills = true;
								}
								else
								{
									noFrills = false;
								}

								if( firstDish		||					//	Always display first one ...
									!noFrills		||					//	always display if Options etc ...
									firstHadFrills	||					//	force new row if first had noFrills
									!(recOrderPtr->status & StatusAsSentConfirmed)	)	//	and if not sent
								{
									//	If next item is no frills, it must still come through here
									if(firstDish && !noFrills)
										firstHadFrills = true;
									else
										firstHadFrills = false;
									
									firstDish = false;

									//	Previous page got countOffset items further down the list
									//	because of multiple items displayed per row

									//	If we're in correct page then load some stuff for display
									if(entityCounter - countOffset >= 0)
									{
										//	Write the seat number to the display image
										tableDisplayRowsPtr[row].seatNumber = currentSeatNumber;
										
										//	Preserve the Index into the Menu
										tableDisplayRowsPtr[row].dishIndex = recOrderPtr->menuIndex[0].index;

										//	Preserve the Index to the Order
										tableDisplayRowsPtr[row].recIndex = recOrderIndex;

										//	Set up 'status' flags to store in cell for call back function
										status = recOrderPtr->status;	//	Start with record's sent status
										status &= 0x00FF;					//	Zero upper UInt8
//										status |= recOrderPtr->itemCounter << 8;	//	itemCounter in upper UInt8

										if(currentMenu==DrinksMenu)
										{
											if(recOrderPtr->menuIndex[1].index != NoValue16)
												status |= StatusHasLinkedItem;	//	Set bit 3 to indicate Link
										}

										if(recOrderPtr->setMenuMask	&&
											recOrderPtr->setMenuMask & SetMenuKey)
											status |= StatusAsSetMenu;//	Set bit shows SetMenuKey

										//	Preserve the status
										tableDisplayRowsPtr[row].status = status;

										if(noFrills)
										{
											thisRow = row;
											thisDishCounter++;
										}
//										displayRowsPtr++;
										row++;
										
										if(*lastItemOrdered != NoValue16	&&
											recOrderPtr->menuIndex[0].index == *lastItemOrdered	&&
											((row > *mainviewTopRowNum + rowsInMainTable) ||
											 (row < *mainviewTopRowNum)))
										{
											*mainviewTopRowNum = row - DisplayRowOffset;
											if(*mainviewTopRowNum < 0)
												*mainviewTopRowNum = 0;
										}
									}

									//	Only show mixes for Food items
									if(currentMenu==DishesMenu)
									{
										//	If this item has sides, display those too
										for(i=1; recOrderPtr->menuIndex[i].index != NoValue16	&&
																i < MaxLinkedItems	&&
																row < MaxTableDisplayRows;		i++)
										{
											//	If we're in correct page then load some stuff for display
											if(entityCounter - countOffset >= 0)
											{
												//	Write the seat number to the display image
												tableDisplayRowsPtr[row].seatNumber = currentSeatNumber;
												
												//	Preserve the Index into the Menu (indicating Dish/Drink)
												if(recOrderPtr->menuType == DishesMenu)
													tableDisplayRowsPtr[row].dishIndex = recOrderPtr->menuIndex[i].index;
												else
													tableDisplayRowsPtr[row].dishIndex = recOrderPtr->menuIndex[i].index
																														 | IsDrinkMask;
												//	Preserve the Index to the Order
												tableDisplayRowsPtr[row].recIndex = recOrderIndex;

												status = 0;
												status |= StatusHasLinkedItem;			//	Set bit 3 to indicate a Link
						
												if(recOrderPtr->status & StatusAsSentConfirmed)
													status |= StatusAsSentConfirmed;

												tableDisplayRowsPtr[row].status = status;
									
//												displayRowsPtr++;
												row++;
										
												if(*lastItemOrdered != NoValue16	&&
													recOrderPtr->menuIndex[0].index == *lastItemOrdered	&&
													((row > *mainviewTopRowNum + rowsInMainTable) ||
													 (row < *mainviewTopRowNum)))
												{
													*mainviewTopRowNum = row - DisplayRowOffset;
													if(*mainviewTopRowNum < 0)
														*mainviewTopRowNum = 0;
												}
											}
											entityCounter++;
											firstDish = true;
										}
									}
								}
								else	//	We've had this dish before, so just update the count
								{
									firstDish = false;
									firstHadFrills = false;

									//	If we're in correct page then load some stuff for display
									if(entityCounter + countOffset >= 0)
									{
										status = recOrderPtr->status;	//	Start with record's sent status
										status &= 0x00FF;						//	Zero upper UInt8
										status |= thisDishCounter << 8;	//	itemCounter in upper UInt8
	
	
										tableDisplayRowsPtr[thisRow].status = status;


										thisDishCounter++;
										
										if(*lastItemOrdered != NoValue16	&&
											recOrderPtr->menuIndex[0].index == *lastItemOrdered	&&
											((row > *mainviewTopRowNum + rowsInMainTable) ||
											 (row < *mainviewTopRowNum)))
										{
											*mainviewTopRowNum = row - DisplayRowOffset;
											if(*mainviewTopRowNum < 0)
												*mainviewTopRowNum = 0;
										}
									}
									countOffset++;							//	Count the reduction in rows
								}
								entityCounter++;
								recOrderIndex++;
								recHandle = DmQueryRecord(*ordersDB, recOrderIndex);
							}
							else
							{
								recHandle = NULL;
								recOrderIndex--;		//	Return to Marker record
							}
						}
						MemPtrUnlock(recOrderPtr);
					}	//	while(). Do all items for this Course
				}
			}
			currentCourse = MarkerItem;	//	Look for next Course item
		}
	}

	tableDisplayRowsPtr[row].seatNumber = NoValue16;		//	Mark end of data

	if(row >= MaxTableDisplayRows)
	{
		FrmAlert(WarnMaxDisplayItemsAlert);
	}
}



/***********************************************************************
 *
 * FUNCTION:   	MmOrdersviewServeCourseLoadTableArrayLoop
 *
 * DESCRIPTION:	Loads the database records into the Overview Table array.
 *						Sets OverviewTopRowNum and Prefs.current.seatNo
 *
 * PARAMETERS:  
 *
 * RETURNED:   	nothing
 *
 ***********************************************************************/
void MmOverviewLoadTableArrayLoop(DBTypePtr dBs, MmPreferenceTypePtr prefs, 
												UInt16 overviewPageType, UInt16 overviewServingCourse, 
													TableDisplayType *tableDisplayRowsPtr, UInt16 overviewExpandedStatus, 
														UInt16 recOrderIndex,
															Int16 *overviewTopRowNum, UInt16 *lastItemOrdered
																															)
{
	Int16								row = 0;
	UInt16							unsentItemCounter = 0;

	MemHandle						recHandle;
	OrderTypePtr					recOrderPtr;

	UInt16							recIndex = 0;
	UInt16							recMenuIndex = 0;
	UInt16							recOrderIndexStart;
	
	UInt16							servingCourseIndex = 0;
	UInt16							servingCourseID = 0;
	UInt16							prevServingCourseID = 0;
	
	Int16								numRecords;
	Int16								numServingCourseRecords;
	
	Boolean							firstPass			= true;
	Boolean							firstIndexPass		= true;
	Boolean							showServingCourse	= false;

	UInt16							i, j;

	UInt16							tableNumber;
	UInt16							nextTableNumber;
	UInt16							seatNumber;
	UInt16							nextSeatNumber;
	
	Boolean							orderFound;
	Boolean							wasMarker			= false;
	

	MemHandle						serve2DArrayHandle;
	ServingCourseIndexTypePtr	serve2DArrayPtr;	//	Ptr to the entire 2D array

	MemHandle						serveIndxsHandle;
	ServingCourseIndexTypePtr*	serveIndxsPtr;		//	Ptr to each Serving Course array

	MemHandle						serveDataHandle;
	ServeDataTypePtr				serveDataPtr;		//	Start of each Serving Course array stores Total



	MemHandle						bundleSizeHandle;
	BundleGroupTypePtr			bundleGroup;		//	Ptr to bundling structures for display eg. 3xSoup

//	BundleGroupType 				bundleGroup[BundleGroupsMax];
	UInt16							bundleGroupIndex_J = 0;
	UInt16							bundleGroupIndex_K = 0;



//	Store page configuration options
	PageViewType					showType;
	PageViewTypePtr				show = &showType;
	



	numRecords = DmNumRecords(dBs->mmOrdersDB);
	numServingCourseRecords = DmNumRecords(dBs->mmServingCourseDB);



	// Get some space in which to stash the indexes. Allocates entire 2D array.
	serve2DArrayHandle = MemHandleNew((numRecords + 1) * numServingCourseRecords * sizeof(ServingCourseIndexType));
	serve2DArrayPtr = MemHandleLock(serve2DArrayHandle);

	// Allocate space for an array of pointers to each of the Serving Course arrays (rows).
	serveIndxsHandle = MemHandleNew(numServingCourseRecords * sizeof(ServingCourseIndexType*));
	serveIndxsPtr = MemHandleLock(serveIndxsHandle);

	// Also allocate space for pointers to the Serving Course arrays for Totals (element [0]).
	serveDataHandle = MemHandleNew(numServingCourseRecords * sizeof(ServeDataType));
	serveDataPtr = MemHandleLock(serveDataHandle);

	//	Now 'point' the pointers to the start of each of the Serving Course arrays (rows)
	for(i=0; i < numServingCourseRecords; i++)
		serveIndxsPtr[i] = serve2DArrayPtr + (i * numRecords);


	//	'Set' the entire 2D array. Zero the Totals
	MemSet(serve2DArrayPtr, (numRecords) * numServingCourseRecords * sizeof(ServingCourseIndexType), NoValue16);
	MemSet(serveDataPtr, numServingCourseRecords * sizeof(ServeDataType), 0);




	// Allocate space for array of item size bundles.
	bundleSizeHandle = MemHandleNew(BundleGroupsMax * sizeof(BundleGroupType));
	bundleGroup = MemHandleLock(bundleSizeHandle);

	
	//	Clear the entire array of bundleGroup for next seat
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


	tableNumber = prefs->current.tableNo;
	nextTableNumber = tableNumber;
	seatNumber = prefs->current.seatNo;
	nextSeatNumber = seatNumber;



	


	//	Set Display Config Booleans for each display mode 
	MmSetDisplayModeBooleanFlags(show, overviewPageType);



	if(overviewPageType == OverviewServingCourses)
		servingCourseIndex = overviewServingCourse;
	else
		servingCourseIndex = 0;
	
	
	if(show->servingCourses	||	show->thisServingOnly)
	{
		//	This array is only useful to structure the display data by Serving Course, rather than by Orders
		//	It is filled now for each Table/Seat/ServingCourse as desired, so that the display function just 
		//	needs to step through the array showing the data. 
		MmFillServingCourseArray(dBs, prefs, overviewPageType, &recOrderIndex, 
											tableNumber, seatNumber,
												&servingCourseIndex, &servingCourseID, 
													serve2DArrayPtr, serveIndxsPtr, serveDataPtr,
														show, tableDisplayRowsPtr, overviewExpandedStatus);
	}
	else
	{
		//	recOrderIndex was pre-loaded with start of current Table
		if(show->allTables)
		{
			nextTableNumber = NoValue16;
			recOrderIndex = MmFindFirstSeatOrder(dBs, &prefs->current, &orderFound, 
													&nextTableNumber, &nextSeatNumber);	//	Find First Table/Seat
		}
		else if(show->wholeTable/*	&&	!show->startThisSeat*/)
			recOrderIndex = MmFindFirstSeatOrder(dBs, &prefs->current, &orderFound, 
													&nextTableNumber, &nextSeatNumber);	//	Find start of First Seat

		else if(seatNumber != NoValue16)
			recOrderIndex = MmFindSortTableSeat(prefs->current.tableNo, prefs->current.seatNo);	//	Find start of this Seat


		if(recOrderIndex == NoValue16)
			recOrderIndex = 0;		//	Start right at the beginning
	}
	
	




	//	If its a Serving Coures Display, show a seat first, unless its a Serving Course only 
	if(show->servingCourses	&&	!show->thisServingOnly)
	{
		//	Write the table and seat number to the display image
		tableDisplayRowsPtr[row].overviewPageType = overviewPageType;
		tableDisplayRowsPtr[row].rowDisplayType = RowIsSeat;
		tableDisplayRowsPtr[row].tableNumber = tableNumber;
		tableDisplayRowsPtr[row].seatNumber = seatNumber;

		tableDisplayRowsPtr[row].dishIndex = NoValue16;	//	Identifies this as a defined Seat
		tableDisplayRowsPtr[row].recIndex = NoValue16;
		tableDisplayRowsPtr[row].menuType = 0;
		tableDisplayRowsPtr[row].status = 3;
		tableDisplayRowsPtr[row].billing = 0;
		tableDisplayRowsPtr[row].showCollapsed = false;
		tableDisplayRowsPtr[row].setExpanded = false;
		tableDisplayRowsPtr[row].drawLine = false;

		row++;
	}







	//	Main Loop. Load the Table etc.
	if(show->servingCourses	||	show->thisServingOnly)
	{
		UInt8			mMenu;
		UInt8			previousMenuType = NoValue8;
		UInt16		recElement0Index;
		UInt16		status;


		if(overviewPageType == OverviewTakeOrders)
		{
			//	First display any Set Menu Keys, or No Serving Course items stored in serveIndxsPtr[0][]
			recElement0Index = serveIndxsPtr[0][0].index;


			for(j=0; recElement0Index < NoValue16		&&		row < MaxTableDisplayRows;)
			{
				if(serveIndxsPtr[0][j].isSetMenuKey)
				{
					recHandle = DmQueryRecord(dBs->mmOrdersDB, recElement0Index);
					if(recHandle)
					{
						recOrderPtr = MemHandleLock(recHandle);
						
						//	Write the status
						status = recOrderPtr->status;					//	Start with this record's status
						status &= 0x00FF;									//	Zero upper UInt8 (item count)

						status |= StatusAsSetMenu;
						tableDisplayRowsPtr[row].rowDisplayType = RowIsSetMenuKey;

						tableDisplayRowsPtr[row].status = status;


						//	Write the table and seat number to the display image
						tableDisplayRowsPtr[row].overviewPageType = overviewPageType;
						tableDisplayRowsPtr[row].tableNumber = tableNumber;
						tableDisplayRowsPtr[row].seatNumber = seatNumber;

						if(recOrderPtr->menuType == DishesMenu)
							tableDisplayRowsPtr[row].dishIndex = recOrderPtr->menuIndex[0].index;
						else
							tableDisplayRowsPtr[row].dishIndex = recOrderPtr->menuIndex[0].index | IsDrinkMask;

						//	Write the Index to the Order
						tableDisplayRowsPtr[row].recIndex = recElement0Index;

						//	Write the menuType
						tableDisplayRowsPtr[row].menuType = recOrderPtr->menuType;

						tableDisplayRowsPtr[row].billing = 0;
						tableDisplayRowsPtr[row].showCollapsed = false;
						tableDisplayRowsPtr[row].setExpanded = false;
						tableDisplayRowsPtr[row].drawLine = false;

						MemPtrUnlock(recOrderPtr);
					}
					
					row++;
				}

				j++;
				recElement0Index = serveIndxsPtr[0][j].index;
			}
		}






		
		




		//	Now go through the rest of the orders
		while(recOrderIndex < numRecords		&&		row < MaxTableDisplayRows)
		{
			Boolean found;
			
			recIndex = MmOverviewGetNextRecIndex(&firstIndexPass, show->thisServingOnly,
																&showServingCourse, &servingCourseIndex, &servingCourseID, 
																	serveIndxsPtr, serveDataPtr, numRecords, 
																		numServingCourseRecords, &found);




			if(recIndex == NoValue16)	//	Show next Serving Course, or we're Finished
			{
				if(found)
				{
					//	showServingCourse boolean gets set if the Serving Course name should display
					if(showServingCourse)
					{
						//	Write the table and seat number to the display image
						tableDisplayRowsPtr[row].overviewPageType = overviewPageType;
						tableDisplayRowsPtr[row].rowDisplayType = RowIsServingCourse;
						tableDisplayRowsPtr[row].tableNumber = tableNumber;
						tableDisplayRowsPtr[row].seatNumber = seatNumber;

						tableDisplayRowsPtr[row].dishIndex = 0;
						tableDisplayRowsPtr[row].recIndex = serveDataPtr[servingCourseIndex].servingCourseID;

						tableDisplayRowsPtr[row].menuType = serveDataPtr[servingCourseIndex].menuType;
						tableDisplayRowsPtr[row].status = 0;

						tableDisplayRowsPtr[row].showCollapsed = serveDataPtr[servingCourseIndex].showCollapsed;
						tableDisplayRowsPtr[row].setExpanded = serveDataPtr[servingCourseIndex].setExpanded;
			
						if(serveDataPtr[servingCourseIndex].menuType != previousMenuType)
						{
							previousMenuType = serveDataPtr[servingCourseIndex].menuType;
							tableDisplayRowsPtr[row].drawLine = true;
						}
						else
							tableDisplayRowsPtr[row].drawLine = false;

						tableDisplayRowsPtr[row].billing = 0;


						recOrderIndex = 0;

						row++;

						continue;
					}
				}
				else
				{
					break;							//	Finished
				}
			}
			else
			{
				if(show->servingCourses)
				{
					if(serveDataPtr[servingCourseIndex].showCollapsed)
						show->unsentOnly = true;
					else
						show->unsentOnly = false;
				}

				recOrderIndex = recIndex;

				mMenu = serveDataPtr[servingCourseIndex].menuType;

				//	Do single scan only for the Serving Course displays
				MmOverviewWriteSeatOrders(dBs, prefs, overviewPageType, overviewTopRowNum, lastItemOrdered,
													tableDisplayRowsPtr, &row, &tableNumber, &nextTableNumber,
															&seatNumber, &nextSeatNumber,
																&recOrderIndex, mMenu, &firstPass, 
																	&wasMarker, &servingCourseID, show, 
																		bundleGroup, &bundleGroupIndex_J, &bundleGroupIndex_K);

				if(prevServingCourseID != servingCourseID)
				{
					prevServingCourseID = servingCourseID;
				}
			}
		}
	}








	else	//	ie !(		if(show->servingCourses	||	show->thisServingOnly)		)
	{
		UInt8			menuType;
		Int16			lastDishesRow;
		Boolean		allDone;
		Boolean		seatsDone;
		

		prevServingCourseID = servingCourseID;

		allDone = false;
		
		while(!allDone)
		{
			tableNumber = nextTableNumber;
			seatsDone = false;

			while(!seatsDone)
			{
				//	Do first for Dishes
				menuType = DishesMenu;
				recOrderIndexStart = recOrderIndex;
				firstPass = true;

				seatNumber = nextSeatNumber;



				//	Loop through Orders until end of seat, table or DB as appropriate
				MmOverviewWriteSeatOrders(dBs, prefs, overviewPageType, overviewTopRowNum, lastItemOrdered,
														tableDisplayRowsPtr, &row, &tableNumber, &nextTableNumber,
																&seatNumber, &nextSeatNumber,
																	&recOrderIndex, menuType, &firstPass, 
																		&wasMarker, &servingCourseID, show, 
																			bundleGroup, &bundleGroupIndex_J, &bundleGroupIndex_K);



				//	Now repeat for Drinks
				lastDishesRow = row;
				
				menuType = DrinksMenu;
				recOrderIndex = recOrderIndexStart;

				//	Loop through Orders until end of seat, table or DB as appropriate
				seatsDone = MmOverviewWriteSeatOrders(dBs, prefs, overviewPageType, overviewTopRowNum, lastItemOrdered, 
														tableDisplayRowsPtr, &row, &tableNumber, &nextTableNumber,
																&seatNumber, &nextSeatNumber,
																	&recOrderIndex, menuType, &firstPass, 
																		&wasMarker, &servingCourseID, show, 
																			bundleGroup, &bundleGroupIndex_J, &bundleGroupIndex_K);
																			
				if(nextTableNumber == LastTableRecord)
					allDone = true;
			}
		}




		//	If nothing's been shown before, at least show the current seat
		if(row == 0	&&	overviewPageType == OverviewOrdersOnlyPage)
		{
			//	Write the table and seat number to the display image
			tableDisplayRowsPtr[row].overviewPageType = overviewPageType;
			tableDisplayRowsPtr[row].rowDisplayType = RowIsSeat;
			tableDisplayRowsPtr[row].tableNumber = prefs->current.tableNo;
			tableDisplayRowsPtr[row].seatNumber = prefs->current.seatNo;

			tableDisplayRowsPtr[row].dishIndex = NoValue16;	//	Identifies this as a defined Seat
			tableDisplayRowsPtr[row].recIndex = NoValue16;
			tableDisplayRowsPtr[row].menuType = 0;
			tableDisplayRowsPtr[row].status = 3;
			tableDisplayRowsPtr[row].billing = 0;
			tableDisplayRowsPtr[row].showCollapsed = false;
			tableDisplayRowsPtr[row].setExpanded = false;
			tableDisplayRowsPtr[row].drawLine = false;

			row++;
		}
		//	No Unsent items exist, but a Sent item does, so a Table/Seat has been displayed
		else if(wasMarker	&&	overviewPageType == OverviewUnsentPage)
		{
			row--;
		}
	}



	tableDisplayRowsPtr[row].seatNumber = NoValue16;	//	Mark end of data


	MemHandleUnlock(serve2DArrayHandle);
	MemHandleFree(serve2DArrayHandle);

	MemHandleUnlock(serveIndxsHandle);
	MemHandleFree(serveIndxsHandle);

	MemHandleUnlock(serveDataHandle);
	MemHandleFree(serveDataHandle);

	MemHandleUnlock(bundleSizeHandle);
	MemHandleFree(bundleSizeHandle);


	*lastItemOrdered = 0;										//	Clear this so it doesn't keep going back
	
	if(row >= MaxTableDisplayRows)
	{
		FrmAlert(WarnMaxDisplayItemsAlert);
	}
}




