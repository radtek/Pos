#define EXTERNAL 1

#include "MenuMate.h"


/***********************************************************************
 *
 * FUNCTION:    CreateEditRecord
 *
 * DESCRIPTION: P6. This routine creates a new memo record.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    true if the record was sucessfully created.
 *
 ***********************************************************************/
Boolean CreateEditRecord(UInt index)
{
	Ptr		p;
	VoidHand memoRec;
//	UInt		index = 0;
	Err		error;
	Char		zero = 0;

	// Create a new first record in the database
	memoRec = DmNewRecord(MmEditDB, &index, 64);

	// Lock down the block containing the new record.
	p = MemHandleLock(memoRec);

	// Write a zero to the first byte of the record to null terminate the new memo string.
	error = DmWrite(p, 0, &zero, sizeof(Char));
	
	// Check for fatal error.
	ErrFatalDisplayIf(error, "Could not write to new record.");
	
	// Unlock the block of the new record.
	MemPtrUnlock(p);

	// Release the record to the database manager.  The true value indicates that
	// the record contains "dirty" data.  Release Record will set the record's 
	// dirty flag and update the database modification count.
	DmReleaseRecord(MmEditDB, index, true);

	// Remember the index of the current record.
	return(true);	
}


/***********************************************************************
 *
 * FUNCTION:    GetFocusObjectPtr
 *
 * DESCRIPTION: P4. This routine returns a pointer to the field object, in 
 *              the current form, that has the focus.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    pointer to a field object or NULL of there is no focus
 *
 ***********************************************************************/
static FieldPtr GetFocusObjectPtr(void)
{
	FormPtr frm;
	Word focus;
	
	// Get a pointer to tha active form and the index of the form object with focus.
	frm = FrmGetActiveForm();
	focus = FrmGetFocus(frm);
	
	// If no object has the focus return NULL pointer.
	if (focus == noFocus)
		return (NULL);
		
	// Return a pointer to the object with focus.
	return (FrmGetObjectPtr(frm, focus));
}


/***********************************************************************
 *
 * FUNCTION:    EditDoMenuCommand
 *
 * DESCRIPTION: P4. This routine performs the menu command specified.
 *
 * PARAMETERS:  command - menu item id
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
void EditDoMenuCommand(Word command)
{
	FieldPtr	fld;
//	FormPtr	frm;

	// Do the appropriate action for the menu command selected.
	switch (command)
	{
/*		case EditOptionsGetInfo:		// Display the info form.
			// Load the info form, then display it.
			frm = FrmInitForm(MemoBeamInfoForm);
			FrmDoDialog(frm);
			
			// Delete the info form.
	 		FrmDeleteForm(frm);
			break;
*/		
		case EditOptionsGototopofpage:		// Go to the top of the field.	
			fld = GetFocusObjectPtr();
			if (fld)
				FldSetInsPtPosition(fld, 0);
			break;
		
		case EditOptionsGotobottomofpage:	// Go to the bottom of the field.
			fld = GetFocusObjectPtr();
			if (fld)
				FldSetInsPtPosition(fld, FldGetTextLength(fld));
			break;
		
		case EditEditCut:			// Do a cut to the clipboard if text is highlighted.
			fld = GetFocusObjectPtr();
			if (fld)
				FldCut(fld);
			break;
		
		case EditEditCopy:			// Copy the highlighted text to the clipboard.
			fld = GetFocusObjectPtr();
			if (fld)
				FldCopy(fld);	
			break;
		
		case EditEditPaste:			// Do a paste from the clipboard.
			fld = GetFocusObjectPtr();
			if (fld)
				FldPaste(fld);		
			break;
			
		case EditEditUndo:			// Undo the last text change.
			fld = GetFocusObjectPtr();
			if (fld)
				FldUndo(fld);
			break;
		
		case EditEditSelectAll:	// Select all the existing text.
			fld = GetFocusObjectPtr();
			if (fld)
				FldSetSelection(fld, 0, FldGetTextLength(fld));
			break;
			
		case EditEditKeyboard:		// Display the on screen keyboard.
			SysKeyboardDialog(kbdAlpha);
			break;

		case EditEditGraffiti:		// Display the graffiti reference screen
			SysGraffitiReferenceDialog (referenceDefault);
			break;
			
	}	
}


/***********************************************************************
 *
 * FUNCTION:		EditSaveData
 *
 * DESCRIPTION:	P5. Saves the data in the field as the first record in 
 *                the application's database.
 *
 * PARAMETERS:		fld - the field that contains the text.
 *
 * RETURNED:		Nothing.
 *
 ***********************************************************************/
/*void EditSaveData(FieldPtr fld)
{
	CharPtr	text;
	UInt		index = 0;		// Index for the first record.
	Handle	textHandle;
	
	// Get a pointer to the field's text.
	text = FldGetTextPtr(fld);

	// P6. Clear the handle value in the field, otherwise the handle
	// will be freed when the form is closed.
	// This will cause problems since the handle is to the data
	// IN the database record, not a separate copy.
	FldSetTextHandle(fld, 0);

	#ifdef saveForReference
	// Is there data in the field?
	if (text != NULL && *text != 0)
	{
		// Create a new first record in the database and get a handle to the record.
		textHandle = DmNewRecord(MemoBeamDB, &index, strlen(text) + 1);

		// Lock down the handle and get a pointer to the new record.
		textPtr = MemHandleLock(textHandle);
	
		// Write the field's text to the database record.
		DmWrite(textPtr, 0, text, strlen(text) + 1);

		// Unlock the new record.
		MemHandleUnlock(textHandle);
	}
	#endif
	
	// Release the record to the database manager.  The true value indicates that
	// the record contains "dirty" data.  Release Record will set the record's 
	// dirty flag and update the database modification count.
	DmReleaseRecord(MemoBeamDB, index, true);
}
*/

/***********************************************************************
 *
 * FUNCTION:		EditSaveData P8
 *
 * DESCRIPTION:	P5. Saves the data in the field as the first record in 
 *                the application's database.
 *
 * PARAMETERS:		fld - the field that contains the text.
 *
 * RETURNED:		Nothing.
 *
 ***********************************************************************/
void EditSaveData(FieldPtr fld)
{
	CharPtr	textPtr;
	CharPtr	recPtr;
	Handle	textHandle;
//	Int		length;
	
	// Get a pointer to the field's text.
	textPtr = FldGetTextPtr(fld);

	// P6. Clear the handle value in the field, otherwise the handle
	// will be freed when the form is closed.
	// This will cause problems since the handle is to the data
	// IN the database record, not a separate copy.
	FldSetTextHandle(fld, 0);

	// Create a new first record in the database and get a handle to the record.
	textHandle = DmQueryRecord(MmEditDB, CurrentEditRecord);

	// Lock down the handle and get a pointer to the new record.
	recPtr = MemHandleLock(textHandle);

	// Write the field's text to the database record.
	DmWrite(recPtr, 0, textPtr, StrLen(textPtr) + 1);

	// Unlock the new record.
	MemHandleUnlock(textHandle);
	// Release the record to the database manager.  The true value indicates that
	// the record contains "dirty" data.  Release Record will set the record's 
	// dirty flag and update the database modification count.
	DmReleaseRecord(MmEditDB, CurrentEditRecord, true);
}


/***********************************************************************
 *
 * FUNCTION:		EditRetrieveData
 *
 * DESCRIPTION:	P5. Retrieves the data from the first record of a database 
 *						and places it in a field.
 *
 * PARAMETERS:		fld - the field to contain the text.
 *
 * RETURNED:		Nothing.
 *
 ***********************************************************************/
/*void EditRetrieveData(FieldPtr fld)
{
	VoidHand	recHandle;
	
	// Check to make sure there is a record to retrieve.
	if (CurrentEditRecord != noRecordSelected && DmNumRecords(MemoBeamDB) > 0)
	{
		// Get a handle for the first record.
		recHandle = DmGetRecord(MemoBeamDB, CurrentEditRecord);
		
		// P6. Set the field's text to the data in the record.
		FldSetTextHandle(fld, recHandle);
		
		#ifdef saveForReference
		// Lock down the handle and get a pointer to the record data.
		recText = MemHandleLock(recHandle);
		
		// Allocate a new memory chunk that will contain a copy of the data.
		newHandle = MemHandleNew(StrLen(recText) + 1);
		
		// Lock down the handle and get a pointer to the memory chunk.
		newText = MemHandleLock(newHandle);
		
		// Copy the data from the record to the new memory chunk.
		StrCopy(newText, recText);
	
		// Unlock the new memory chunk.
		MemHandleUnlock(newHandle);
		
		// Set the field's text to the data in the new memory chunk.
		FldSetTextHandle(fld, newHandle);
	
		// Unlock the database record.
		MemHandleUnlock(recHandle);
		
		// Release the record to the database system.
		// (The zero means that the record data hasn't been changed.)
		DmReleaseRecord(MemoBeamDB, CurrentEditRecord, 0);
		#endif
	}
}
*/

/***********************************************************************
 *
 * FUNCTION:    EditViewUpdateScrollBar
 *
 * DESCRIPTION: P13. Visually updates the scroll bar.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
void EditViewUpdateScrollBar()
{
	Word scrollPos;
	Word textHeight;
	Word fieldHeight;
	Short maxValue;
	FieldPtr fld;
	ScrollBarPtr bar;

	fld = MmGetObjectPtr (EditPageEditFieldField);
	bar = MmGetObjectPtr (EditPageScrollbarScrollBar);
	
	// get the values necessary to update the scroll bar.
	FldGetScrollValues (fld, &scrollPos, &textHeight,  &fieldHeight);

	if (textHeight > fieldHeight)
		maxValue = textHeight - fieldHeight;
	else if (scrollPos)
		maxValue = scrollPos;
	else
		maxValue = 0;

	SclSetScrollBar (bar, scrollPos, 0, maxValue, fieldHeight-1);
}


/***********************************************************************
 *
 * FUNCTION:		EditRetrieveData P8
 *
 * DESCRIPTION:	P5. Retrieves the data from the first record of a database 
 *						and places it in a field.
 *
 * PARAMETERS:		fld - the field to contain the text.
 *
 * RETURNED:		Nothing.
 *
 ***********************************************************************/
void EditRetrieveData(FieldPtr fld)
{
	VoidHand	recHandle;
	
	// Get a handle for the first record.
	recHandle = DmGetRecord(MmEditDB, CurrentEditRecord);
		
	// P6. Set the field's text to the data in the record.
	FldSetTextHandle(fld, recHandle);
	EditViewUpdateScrollBar();
}


/***********************************************************************
 *
 * FUNCTION:    EditViewPageScroll
 *
 * DESCRIPTION: P13. Scrolls the edit view a page at a time.
 *
 * PARAMETERS:   direction - up or down
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
void EditViewPageScroll (DirectionType direction)
{
	Short value;
	Short min;
	Short max;
	Short pageSize;
	Word linesToScroll;
	FieldPtr fld;
	ScrollBarPtr bar;

	fld = MmGetObjectPtr (EditPageEditFieldField);
	
	if (FldScrollable (fld, direction))
	{
		linesToScroll = FldGetVisibleLines (fld) - 1;
		FldScrollField (fld, linesToScroll, direction);

		// Update the scroll bar.
		bar = MmGetObjectPtr (EditPageScrollbarScrollBar);
		SclGetScrollBar (bar, &value, &min, &max, &pageSize);

		if (direction == up)
			value -= linesToScroll;
		else
			value += linesToScroll;
		
		SclSetScrollBar (bar, value, min, max, pageSize);
	}
}


/***********************************************************************
 *
 * FUNCTION:    EditViewScroll
 *
 * DESCRIPTION: P13.  Scrolls the edit view a line at a time.
 *
 * PARAMETERS:  linesToScroll - number of lines to scroll, 
 *                              negative numbers are used to scroll up
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
void EditViewScroll (Short linesToScroll)
{
	Word				blankLines;
	Short				min;
	Short				max;
	Short				value;
	Short				pageSize;
	FieldPtr			fld;
	ScrollBarPtr	bar;
	
	fld = MmGetObjectPtr (EditPageEditFieldField);

	if (linesToScroll < 0)
		{
		blankLines = FldGetNumberOfBlankLines (fld);
		FldScrollField (fld, -linesToScroll, up);
		
		// If there were blank lines visible at the end of the field
		// then we need to update the scroll bar.
		if (blankLines)
			{
			// Update the scroll bar.
			bar = MmGetObjectPtr (EditPageScrollbarScrollBar);
			SclGetScrollBar (bar, &value, &min, &max, &pageSize);
			if (blankLines > -linesToScroll)
				max += linesToScroll;
			else
				max -= blankLines;
			SclSetScrollBar (bar, value, min, max, pageSize);
			}
		}

	else if (linesToScroll > 0)
		FldScrollField (fld, linesToScroll, down);
}


/***********************************************************************
 *
 * FUNCTION:		MmEditPageFormHandleEvent
 *
 * DESCRIPTION:	P3. Handles processing of events for the 'edit' form.
 *
 * PARAMETERS:		event	- the most recent event.
 *
 * RETURNED:		True if the event is handled, false otherwise.
 *
 ***********************************************************************/
Boolean MmEditPageFormHandleEvent(EventPtr event)
{
	FormPtr	frm;
	Word		fldIndex;
	Boolean	handled = false;

	switch (event->eType)
	{
   	case ctlSelectEvent:  // A control button was pressed and released.
			
	   	// If the done button is pressed, go back to the main form.
	   	if (event->data.ctlEnter.controlID == EditPageDoneButton)
   		{
				// Remove the edit form and display the main form.
	   		FrmReturnToForm(/*BeamingForm*/0);
				handled = true;
			}
			break;
  	
  	  	case keyDownEvent:  // P13. User used Graffiti or a physical button
   		if (event->data.keyDown.chr == pageUpChr)
			{
				EditViewPageScroll(up);
				handled = true;
			}
	
			else if (event->data.keyDown.chr == pageDownChr)
			{
				EditViewPageScroll(down);
				handled = true;
			}
	
			else
			{
				// An ordinary ASCII character was entered.  Have the form
				// give the field the character.  Then check to see if the 
				// scrolling changed.
				frm = FrmGetActiveForm ();
				FrmHandleEvent (frm, event);
				EditViewUpdateScrollBar ();
				handled = true;
			}
			break;
   			
		case sclRepeatEvent:  // P13. the scroll bar was pressed
			EditViewScroll (event->data.sclRepeat.newValue - 
				event->data.sclRepeat.value);

	   	// Repeating controls don't repeat if handled is set true.
			break;
			
			
  		case frmOpenEvent:	// The form was told to open.
  			// It has already been loaded and activated.
  			
  			// P5. Get a pointer to the form and the index of the text field.
			frm = FrmGetActiveForm();
			fldIndex = FrmGetObjectIndex(frm, EditPageEditFieldField);

   		// P5. Get the data for the field from the application's database.
   		EditRetrieveData(FrmGetObjectPtr(frm, fldIndex));
			
  			// Draw the form with the text field data in place.
			FrmDrawForm(frm);
			
			// P4. Set the focus, and the insertion point, to the text edit field.
			FrmSetFocus(frm, fldIndex);
			
			handled = true;
			break;
			
		case menuEvent:		// P4. A menu item was selected.
			// First clear the menu status from the display.
			MenuEraseStatus(0);
			
			// Process menu commands for the edit form.
			EditDoMenuCommand(event->data.menu.itemID);
			handled = true;
			break;

  		case frmCloseEvent:
  			// P5. Get a pointer to the form and the index of the text field.
   		frm = FrmGetActiveForm();
   		fldIndex = FrmGetObjectIndex(frm, EditPageEditFieldField);
   		
   		// P5. Save the data from the field to the application's database.
   		EditSaveData(FrmGetObjectPtr(frm, fldIndex));
			break;
	}
	return(handled);
}


