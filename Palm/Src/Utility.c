#define EXTERNAL 1

#include "MenuMate.h"

#ifndef ReleaseVersion

/***********************************************************************
 *
 * FUNCTION:	  MmPrvSyncLogTrace
 *
 * DESCRIPTION:  
 *
 * PARAMETERS:	
 *
 * RETURNED:	  
 *
 ***********************************************************************/
void MmPrvSyncLogTrace(Char* messagePtr)
{
	PrvSyncLogTrace(messagePtr);

/*	UInt8*					recPtr;
	MemHandle					textHandle;
//	Int16						length;
	
	// Create a new first record in the database and get a handle to the record.
	textHandle = DmQueryRecord(MemoDB, Prefs.current.memoRec);

	// Lock down the handle and get a pointer to the new record.
	recPtr = MemHandleLock(textHandle);
	
	// Write the field's text to the database record.
	DmWrite(recPtr, StrLen(recPtr), messagePtr, StrLen(messagePtr) + 1);
//	DmWrite(recPtr, 0, messagePtr, StrLen(messagePtr) + 1);

	// Unlock the new record.
	MemHandleUnlock(textHandle);
	// Release the record to the database manager.  The true value indicates that
	// the record contains "dirty" data.  Release Record will set the record's 
	// dirty flag and update the database modification count.
	DmReleaseRecord(MemoDB, Prefs.current.memoRec, true);
*/}

/***********************************************************************
 *
 * FUNCTION:	  MmPrvSyncLogTraceUI
 *
 * DESCRIPTION:  
 *
 * PARAMETERS:	
 *
 * RETURNED:	  
 *
 ***********************************************************************/
void MmPrvSyncLogTraceUI(Char* messagePtr, UInt16 intValue)
{
	PrvSyncLogTrace(messagePtr, intValue);

/*	UInt8*					recPtr;
	Char						tempStr[80];
	MemHandle					textHandle;
//	Int16						length;
	
	// Create a new first record in the database and get a handle to the record.
	textHandle = DmQueryRecord(MemoDB, Prefs.current.memoRec);

	// Lock down the handle and get a pointer to the new record.
	recPtr = MemHandleLock(textHandle);
	
	StrVPrintF(tempStr, messagePtr, &intValue);

	// Write the field's text to the database record.
	DmWrite(recPtr, StrLen(recPtr), tempStr, StrLen(tempStr) + 1);

	// Unlock the new record.
	MemHandleUnlock(textHandle);
	// Release the record to the database manager.  The true value indicates that
	// the record contains "dirty" data.  Release Record will set the record's 
	// dirty flag and update the database modification count.
	DmReleaseRecord(MemoDB, Prefs.current.memoRec, true);
*/}
#endif


/***********************************************************************
 *
 * FUNCTION:		MmProcessEventIncludingNilEvent
 *
 * DESCRIPTION:	
 *
 * PARAMETERS:		Nil
 *
 * RETURNED:		
 *
 ***********************************************************************/
UInt32 MmProcessEventIncludingNilEvent(FormPtr frm, EventPtr event, UInt16 thisUserLogOffMins,
									UInt16 popTriggerID)
{
	UInt32						timeNow;
	Int16							formID;
//	MenuBarType					activeMenu;

//	char					tempStr[MaxInfoNameSize+1];
	

	timeNow = TimGetSeconds();
	formID = FrmGetActiveFormID();

/*	
if(event.eType == winExitEvent)
{
	if(event.data.winExit.exitWindow == (WinHandle) FrmGetFormPtr(MainView))
	{
		// stop drawing.
	}
}


User Interface Menus
else if(event.eType == winEnterEvent)
{
	if(event.data.winEnter.enterWindow == (WinHandle) FrmGetFormPtr(MainView) &&
		event.data.winEnter.enterWindow == (WinHandle) FrmGetFirstForm ())
	{
		// start drawing
	}
}
*/

	//	Set in event loop to only generate a nilEvent twice per second, ...
	//	... except during return from Beaming before ScreenLock action
	if(event->eType == nilEvent)
	{
		if(timeNow > Prefs.autoLogoutSecs)
		{
			MmRefreshAutoLogout(false, thisUserLogOffMins);	//	Force Log Out
		}

		//	Redisplay 'Login' or initial, unless a Menu is up
		if(MenuGetActiveMenu() == NULL)
			MmDisplayStaffNameLabel(frm, MmInfoDB, 0, popTriggerID, OverviewPageType, Prefs.loggedIn);


/*		
		//	Debug: Display the time remaining until Logout
		else
		{
			if(staffLabelID == TableStaffLabel)
			{
				StrIToA(tempStr, Prefs.autoLogoutSecs - timeNow);

				FrmHideObject(frm, FrmGetObjectIndex(frm, staffLabelID));
				FrmCopyLabel(frm, staffLabelID, tempStr);
				FrmShowObject(frm, FrmGetObjectIndex(frm, staffLabelID));
			}
		}
*/

		//	NilEvent, so it should be safe to turn this back on!
		if(ScreenLockStruct.action == isActivePowerOff)
		{
			ScreenLockStruct.action = isActive;
		}



		//	Finish the ScreenLock and set a new one
		if(ScreenLockStruct.action == unlockOnNilEvent)
		{
			if(formID == PreviousForm	&&	PreviousForm != OverviewForm)
				PreviousForm = OverviewForm;

			ScreenLockStruct.action = awaitingTimeSecs;	//	Await next AutoScreenLockxxxxTimeSecs timeout
			if(Prefs.numUnsentOrders)
				ScreenLockStruct.timeSecs = timeNow + AutoScreenLockLongTimeSecs;	//	Extend the time
			else
				ScreenLockStruct.timeSecs = timeNow + AutoScreenLockShortTimeSecs;//	Extend the time
		}





		//	Trigger a ScreenLock
		else if(		ScreenLockStruct.action == unlockOnNilEvent
					||
						(		ScreenLockStruct.action == lockOnNilEvent
							&&
								Prefs.userSettings.screenLockOn
						)
					||
						(
								ScreenLockStruct.action == awaitingTimeSecs		//	Await next AutoScreenLockxxxxTimeSecs timeout
							&&
								Prefs.userSettings.screenLockOn
							&&
								timeNow > ScreenLockStruct.timeSecs
						)																	)
		{
			PreviousForm = formID;

			ScreenLockStruct.action = isActive;
			FrmPopupForm(ScreenLockForm);


			//	Also force new login if one was pending but not actioned
			Prefs.hLStatus.HLLoginStatus = HLStatusAllDone;
		}




		else if(	timeNow > AutoTablePageLongSecs	||
					timeNow > AutoTablePageShortSecs	)	//	Go to Table Screen if ScreenLock not active
		{
			MmRefreshAutoLogout(false, thisUserLogOffMins);
		}
	}
	
	
	
	


	//	This is a real event, so extend the ScreenLock timeout, and cancel the return to Table Form
	else
	{
		if(ScreenLockStruct.action == awaitingTimeSecs)			//	Await next AutoScreenLockxxxxTimeSecs timeout
		{
			if(Prefs.numUnsentOrders)
				ScreenLockStruct.timeSecs = timeNow + AutoScreenLockLongTimeSecs;	//	Extend the time
			else
				ScreenLockStruct.timeSecs = timeNow + AutoScreenLockShortTimeSecs;//	Extend the time
		}


		AutoTablePageLongSecs = timeNow + AutoGotoTablePageLongSecs;



		//	They pressed a button etc.
		if(event->eType <= keyDownEvent	&&	formID != ScreenLockForm)
		{
			AutoTablePageShortSecs = NoValue32;	//	Cancel the return to Table Form
		}


		MmRefreshAutoLogout(true, thisUserLogOffMins);

	}

	return timeNow;
}



/***********************************************************************
 *
 * FUNCTION:		MmRefreshAutoLogout
 *
 * DESCRIPTION:	Refreshes the AutoLogout time by updating Prefs.autoLogoutSecs
 *
 * PARAMETERS:		Nil
 *
 * RETURNED:		
 *
 ***********************************************************************/
void MmRefreshAutoLogout(Boolean doRefresh, UInt16 thisUserLogOffMins)
																	//	thisUserLogOffMins normally = 0
{																	//	may be set to LogOffMinimumTime
	UInt32						timeNow;
	
	Int16							formID;
//	UInt16 timeTest;
#ifdef TestMessageLog
	char							tempStr[20];
#endif
	
	timeNow = TimGetSeconds();


	//	Log them out if they've gone over time. (Prefs.userLogOffMins = 0 => forever)
	if((timeNow > Prefs.autoLogoutSecs) && (Prefs.userLogOffMins != 0))
	{
		//	User logged out, so set trigger to prompt the user to delete old orders next time
		//	Only want to prompt the user once.
		if(Prefs.loggedIn == true)
		{
			NumOldUnsentOrders = NoValue16;				//	Force count
			Prefs.loggedIn = false;							//	Log them out
			Prefs.logoutTimeSecs = timeNow;				//	To check for unsent orders
		}
	}
	
	//	If they're still logged in, restart their time counting again
	if(Prefs.loggedIn		&&		doRefresh)
	{
		if(Prefs.userLogOffMins == 0)
		{
			Prefs.autoLogoutSecs = NoValue32;	//	Forever
		}
		else
		{
			if(thisUserLogOffMins == 0)	//	If 0, use the normal Prefs.userLogOffMins
				Prefs.autoLogoutSecs = timeNow + (Prefs.userLogOffMins * 60);
			else
				Prefs.autoLogoutSecs = timeNow + (thisUserLogOffMins * 60);
		}
	}


	
	
	//	Go to Table form if either AutoTablePageLongSecs or AutoTablePageShortSecs time out
	if(	Prefs.userSettings.screenLockOn == false
			||
			(	Prefs.userSettings.screenLockOn == true
				&&
				(	ScreenLockStruct.action == awaitingTimeSecs	//	Await next AutoScreenLockxxxxTimeSecs timeout
					||
					ScreenLockStruct.action == disabled
				)
			)
		)
	{
		formID = FrmGetActiveFormID();

		if(timeNow > AutoTablePageShortSecs	||	timeNow > AutoTablePageLongSecs)
		{
			if(formID != TableForm)
			{
				if(timeNow > AutoTablePageShortSecs)
					AutoTablePageShortSecs = NoValue32;
				
				//	Also force new login if one was pending but not actioned
				if(timeNow > AutoTablePageShortSecs)
					Prefs.hLStatus.HLLoginStatus = HLStatusAllDone;

				AutoTablePageLongSecs = NoValue32;
				PreviousForm = formID;
				OverviewPageType = OverviewTakeOrders;	//	For whenever they get back to the Overview
				FrmGotoForm(TableForm);						//	Refresh table and redraw form
			}
		}
	}
}





/***********************************************************************
 *
 * FUNCTION:		MmKeypadHandleEvent
 *
 * DESCRIPTION:	Handles processing of events for the Keypad form.
 *
 * PARAMETERS:		event	- the most recent event.
 *						Fill out the Keypad global before entering this form
 *
 * RETURNED:		True if the event is handled, false otherwise.
 *						Values are returned in the Keypad global on leaving this form
 *
 ***********************************************************************/
Boolean MmKeypadHandleEvent(EventPtr event)
{
	FormPtr						frm;
	Boolean						handled = false;
	UInt32						timeNow;

	MemHandle 					recHandle;
	OrderTypePtr				recPtr;
	Err							err;

	ControlType *				buttonPtr;
	
	UInt16						buttonValue = NoValue16;
	UInt16						multiplier = 10;
	Int16							copiesToMake;

	Char							tempStr[15];



	frm = FrmGetActiveForm();

	timeNow = MmProcessEventIncludingNilEvent(frm, event, 0, 0);


	switch (event->eType)
	{
   	case ctlSelectEvent:  // A control button was pressed and released.
			
	   	// If the done or StandardPrice button is pressed, go back to the Details form.
	   	if(event->data.ctlEnter.controlID == KeypadOkayButton		||
	   		event->data.ctlEnter.controlID == KeypadCancelButton		)
	   	{
	   		FrmReturnToForm(DetailsForm);

	  			// Get a pointer to the form.
	   		frm = FrmGetFormPtr(DetailsForm);
	   		
		   	if(event->data.ctlEnter.controlID == KeypadCancelButton)
		   	{
					if(Keypad.mode == KeypadPriceAdjust)
						Keypad.value = NoValue32Signed;
					else if(Keypad.mode == KeypadCopyOrder)
						Keypad.value = NoValue32;
			   }
		   	

			   switch(Keypad.mode)
			   {
					case KeypadPriceAdjust:

					 	Prefs.curOrder.price = Keypad.value;
			
						//	Update the priceAdj for the order
						recHandle = DmGetRecord(MmOrdersDB, Prefs.current.recIndex);
						if(recHandle)
						{
							recPtr = MemHandleLock(recHandle);
							err = DmWrite(recPtr, (UInt8*)&recPtr->price - (UInt8*)&recPtr->tableNumber,
															&(Prefs.curOrder.price), sizeof(Prefs.curOrder.price));
							ErrFatalDisplayIf(err, "Could not write priceAdj to record.");
							MemPtrUnlock(recPtr);
							DmReleaseRecord(MmOrdersDB, Prefs.current.recIndex, true);
						}

						buttonPtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, DetailsPriceAdjButton));

						if(Prefs.curOrder.price == NoValue32Signed)
						{
							CtlSetLabel(buttonPtr, "Price");
							FrmShowObject(frm, FrmGetObjectIndex(frm, DetailsPriceAdjButton));
						}
						else
						{
							CtlSetLabel(buttonPtr, "$ $ $");
							FrmShowObject(frm, FrmGetObjectIndex(frm, DetailsPriceAdjButton));
						}
						break;


					case KeypadCopyOrder:

						//	Duplicate this order
						if(	(Keypad.mode == KeypadPriceAdjust	&&	Keypad.value != NoValue32Signed)
								||	
								(Keypad.mode == KeypadCopyOrder		&&	Keypad.value != NoValue32)
							)
						{
							UInt16	i;
							
							//	Update in case they made any changes before hitting Copy button
							MmCopyCurrentOrderIndicesToRecord(MmOrdersDB, &Prefs);

//							if(Prefs.curOrder.status & StatusAsSentConfirmed)
								copiesToMake = Keypad.value;
//							else
//								copiesToMake = Keypad.value -1;	//	Asked for 3 copies, give them 2 more
							
							if(copiesToMake > 0)
							{
								for(i=0; i<copiesToMake; i++)
								{
									MmCreatNewDuplicateOrder(&Prefs, &DBs, &LastItemOrdered);
								}
							}
						}

				   	if(event->data.ctlEnter.controlID != KeypadCancelButton)
				   	{
							// Remove the details form and display the Previous form.
							if(Prefs.userSettings.classicMode	||	PreviousForm == MainForm)
								FrmGotoForm(MainForm);
							else
								FrmGotoForm(OverviewForm);
						}
						
						break;
				}

				handled = true;
			}


	   	// If the Clear button is pressed, clear the display.
	   	else if(event->data.ctlEnter.controlID == KeypadClearButton)
	   	{
				if(Keypad.mode == KeypadPriceAdjust)
				{
			   	Keypad.value = NoValue32Signed;
					MmShowPrice(frm, KeypadAmountButton, Keypad.value, 18);
				}
				else
				{
			   	Keypad.value = 0;
					FrmCopyLabel(frm, KeypadAmountButton, StrIToA(tempStr, Keypad.value));
				}

				FrmHideObject(frm, FrmGetObjectIndex(frm, KeypadOkayButton));

				handled = true;
			}

			else
			{
				if(Keypad.maxValue == NoValue32Signed)
					Keypad.maxValue = 2147483647L;	//	= 0x7FFFFFFF
				
				switch(event->data.ctlEnter.controlID)
				{
			   	case KeypadOneButton:
			   		buttonValue = 1;
						break;
	
			   	case KeypadTwoButton:
			   		buttonValue = 2;
						break;
	
			   	case KeypadThreeButton:
			   		buttonValue = 3;
						break;
	
			   	case KeypadFourButton:
			   		buttonValue = 4;
						break;
	
			   	case KeypadFiveButton:
			   		buttonValue = 5;
						break;
	
			   	case KeypadSixButton:
			   		buttonValue = 6;
						break;
	
			   	case KeypadSevenButton:
			   		buttonValue = 7;
						break;
	
			   	case KeypadEightButton:
			   		buttonValue = 8;
						break;
	
			   	case KeypadNineButton:
			   		buttonValue = 9;
						break;

			   	case KeypadZeroZeroButton:
						multiplier = 100;
			   	case KeypadZeroButton:
			   		buttonValue = 0;
						break;
				}

				if(buttonValue != NoValue16)
				{
					if(	(Keypad.mode == KeypadPriceAdjust	&&	Keypad.value == NoValue32Signed)
							||	
							(Keypad.mode == KeypadCopyOrder		&&	Keypad.value == NoValue32)
						)
					{
						Keypad.value = 0;
					}
					
					//	Avoid potential overrun of Int32 Price (in cents) = $21,474,836.47
					if(Keypad.value <= ((Keypad.maxValue - buttonValue) / multiplier))
					{
						Keypad.value *= multiplier;
						Keypad.value += buttonValue;
					}

					//	Display what's there so far
					if(Keypad.mode == KeypadPriceAdjust)
						MmShowPrice(frm, KeypadAmountButton, Keypad.value, 18);
					else
						FrmCopyLabel(frm, KeypadAmountButton, StrIToA(tempStr, Keypad.value));

					FrmShowObject(frm, FrmGetObjectIndex(frm, KeypadOkayButton));
		   	}
			}

			handled = true;
			break;
			

  		case frmOpenEvent:	// The form was told to open.

  			// Draw the form with the text field data in place.
			FrmDrawForm(frm);
			
			//	Display what's there so far
			if(Keypad.mode == KeypadPriceAdjust)
				MmShowPrice(frm, KeypadAmountButton, Keypad.value, 18);
			else
				FrmCopyLabel(frm, KeypadAmountButton, StrIToA(tempStr, Keypad.value));
			


			if(	(Keypad.value == 0)
				|| 
					(Keypad.mode == KeypadPriceAdjust	&&	Keypad.value == NoValue32Signed)
					||	
					(Keypad.mode == KeypadCopyOrder		&&	Keypad.value == NoValue32)
				)
			{
				FrmHideObject(frm, FrmGetObjectIndex(frm, KeypadOkayButton));
			}
			else
			{
				FrmShowObject(frm, FrmGetObjectIndex(frm, KeypadOkayButton));
			}

			CtlSetLabel(FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, KeypadCancelButton)), 
																									Keypad.cancelLabel);
			FrmCopyTitle(frm, Keypad.frmTitleLabel);

			handled = true;
			break;
	}
	return(handled);
}


/***********************************************************************
 *
 * FUNCTION:     MmPlaySounds
 *
 * DESCRIPTION:  Play a sound.
 *
 * PARAMETERS:   nothing
 *
 * RETURNED:     nothing
 *
 ***********************************************************************/
void MmPlaySounds(SoundType SoundToMake)
{
	SndCommandType		sndCmd;

	if(SoundToMake != noSound) 
	{
		sndCmd.cmd 		= sndCmdFreqDurationAmp/*sndCmdFrqOn*/;
		sndCmd.param1 	= Sound[SoundToMake].frequency1;
		sndCmd.param2 	= Sound[SoundToMake].duration1;
		sndCmd.param3 	= SoundLevel;

		SndDoCmd( 0, &sndCmd, 1/*true noWait*/ );

		sndCmd.cmd 		= sndCmdFreqDurationAmp;
		sndCmd.param1 	= Sound[SoundToMake].frequency2;
		sndCmd.param2 	= Sound[SoundToMake].duration2;
		sndCmd.param3 	= SoundLevel;

		SndDoCmd( 0, &sndCmd, 1/*true noWait*/ );

		sndCmd.cmd 		= sndCmdFreqDurationAmp;
		sndCmd.param1 	= Sound[SoundToMake].frequency3;
		sndCmd.param2 	= Sound[SoundToMake].duration3;
		sndCmd.param3 	= SoundLevel;

		SndDoCmd( 0, &sndCmd, 1/*true noWait*/ );
	}
}


/***********************************************************************
 *
 * FUNCTION:		MmDeleteAllBilledSeats
 *
 * DESCRIPTION:	Goes through DB deleting ALL seats marked as Billed
 *
 * PARAMETERS:		
 *
 * RETURNED:		
 *
 ***********************************************************************/
/*void MmDeleteAllBilledSeats(void)
{
	UInt16					recIndex;
	MemHandle				recHandle;
	OrderTypePtr		recPtr;
	UInt16					numberOfRecs;
	UInt16					currentSeatNumber;
	
	currentSeatNumber = Prefs.current.seatNo;
	
	numberOfRecs = DmNumRecords(MmOrdersDB);

	for(recIndex=0; recIndex < numberOfRecs;)
	{
		//	Scoot through all the 'Marker' records
		recHandle = DmQueryNextInCategory (MmOrdersDB, &recIndex, MarkerItem);
		if(recHandle)
		{
			recPtr = MemHandleLock(recHandle);
			if(recPtr->billing & BillingSeatBilled)
			{
				Prefs.current.tableNo = recPtr->tableNumber;
				Prefs.current.seatNo = recPtr->seatNumber;
				MemPtrUnlock(recPtr);
				MmNukeTableSeatOrders(&MmOrdersDB, ThisSeatsOrders, recIndex, Prefs.current.tableNo, 
																	Prefs.current.seatNo, &Prefs.numUnsentOrders);
				numberOfRecs = DmNumRecords(MmOrdersDB);
			}
			else
			{
				MemPtrUnlock(recPtr);
				recIndex++;
			}
		}
		else
		{
			return;
		}
	}
	Prefs.current.seatNo = currentSeatNumber;
}
*/

/***********************************************************************
 *
 * FUNCTION:		MmDoesSeatHaveOrder
 *
 * DESCRIPTION:	Returns Boolean to indicate whether Seat passed has order.
 *						If true, returns billingGroup number by pointer
 *
 * PARAMETERS:		
 *
 * RETURNED:		
 *
 ***********************************************************************/
/*static Boolean MmDoesSeatHaveOrder(UInt16 thisSeat, UInt8* billingGroup, Boolean markItem)
{
	UInt16					recIndex;
	MemHandle				recHandle;
	OrderTypePtr		recPtr;
	UInt16					numberOfRecs;
	UInt8					thisBillingAction;
	Err					error;
	
	numberOfRecs = DmNumRecords(MmOrdersDB);

	recIndex = MmFindSortTableSeat(Prefs.current.tableNo, thisSeat);	//	Find Marker record for Table/Seat

	recHandle = DmQueryRecord (MmOrdersDB, recIndex);
	// If the record was found
	if(recHandle)
	{
		recHandle = DmGetRecord (MmOrdersDB, recIndex);
		recPtr = MemHandleLock(recHandle);
		if(recPtr->tableNumber	== Prefs.current.tableNo &&
			recPtr->seatNumber	== Prefs.current.seatNo)
		{
			thisBillingAction = recPtr->billing;

			//	Write the billing Group
			if(markItem)
			{
				if(markItem == MarkAsSelected)
					thisBillingAction |= BillingSeatMarked;
				else if(markItem == MarkAsUnSelected)
					thisBillingAction &= ~BillingSeatMarked;

				error = DmWrite(recPtr,
						(UInt8*)&recPtr->billing - (UInt8*)&recPtr->tableNumber,
						&thisBillingAction, sizeof(recPtr->billing));
				ErrFatalDisplayIf(error, "Could not write Billing to record.");
			}

			*billingGroup = recPtr->billing;
			Prefs.current.seatNo = currentSeatNumber;	//	Restore Prefs.current.seatNo
			DmReleaseRecord(MmOrdersDB, recIndex, true);
			MemPtrUnlock(recPtr);
			return true;
		}
		DmReleaseRecord(MmOrdersDB, recIndex, true);
		MemPtrUnlock(recPtr);
	}
	return false;
}
*/



/***********************************************************************
 *
 * FUNCTION:		MmConfirmBilling
 *
 * DESCRIPTION:	Skips through Marker records for each seat of Current Table
 *						resetting Bit 6 (Current billing flag (used for Cancel)) and
 *						setting Bit 7.
 *
 * PARAMETERS:		
 *
 * RETURNED:		
 *
 ***********************************************************************/
/*static Boolean MmConfirmBilling(void)
{
	UInt16					recIndex;
	MemHandle				recHandle;
	OrderTypePtr		recPtr;
	UInt16					numberOfRecs;
	
	UInt8					billingGroup;
	
	Err					error;
	
	numberOfRecs = DmNumRecords(MmOrdersDB);
	
	Prefs.current.seatNo = 1;
	recIndex = MmFindSortTableSeat();			//	Find first Marker record

	//	Go through all the records for this Table/Seat to check if a record exists
	while(recIndex < numberOfRecs)
	{
		// Go to the start of each seat (next Marker record).
		recHandle = DmQueryNextInCategory (MmOrdersDB, &recIndex, MarkerItem);
		recHandle = DmGetRecord(MmOrdersDB, recIndex);
		if(recHandle)
		{
			recPtr = MemHandleLock(recHandle);
			if(recPtr->tableNumber	== Prefs.current.tableNo)
			{
				billingGroup = 0;
				if(!(recPtr->billing & BillingSeatBilled))	//	If Bit 7 not set (not been Billed)
				{
					if(recPtr->billing & BillingSeatMarked)		//	If Bit 6 is set
					{
						billingGroup = recPtr->billing & ~BillingSeatMarked;//	Reset Bit 6
						billingGroup |= BillingSeatBilled;						//	And set Bit 7
					}
					//	Write the billing Group
					error = DmWrite(recPtr,
							(UInt8*)&recPtr->billing - (UInt8*)&recPtr->tableNumber,
							&billingGroup, sizeof(recPtr->billing));
					ErrFatalDisplayIf(error, "Could not write Billing to record.");
				}
				DmReleaseRecord(MmOrdersDB, recIndex, true);
				recIndex++;
			}
			else
			{
				DmReleaseRecord(MmOrdersDB, recIndex, true);
				recIndex = numberOfRecs;				//	Time to quit
			}
			MemPtrUnlock(recPtr);
		}
	}
	return true;
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
/*void EditViewUpdateScrollBar ()
{
	UInt16 scrollPos;
	UInt16 textHeight;
	UInt16 fieldHeight;
	Int16 maxValue;
	FieldPtr fld;
	ScrollBarPtr bar;

	fld = GetObjectPtr (EditEditFieldField);
	bar = GetObjectPtr (EditScrollbarScrollBar);
	
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

*/




/***********************************************************************
 *
 * FUNCTION:		MmRegisterHandleEvent
 *
 * DESCRIPTION:	Handles processing of events for the Register form.
 *
 * PARAMETERS:		event	- the most recent event.
 *
 * RETURNED:		True if the event is handled, false otherwise.
 *
 ***********************************************************************/
Boolean MmRegisterHandleEvent(EventPtr event)
{
	FormPtr						frm;
	UInt32						timeNow;
	Boolean						handled = false;
	
	MemHandle					fieldHandle, oldHandle;
	char							*textStr;
//	UInt16						fieldIndex;
	FieldType					*fieldPtr;

	UInt16						recIndex = 0;
	
	ListPtr						listPtr;
//	ControlType					*buttonPtr;
	UInt16						itemSelected;
	UInt16						offset;
	



	frm = FrmGetActiveForm();

	timeNow = MmProcessEventIncludingNilEvent(frm, event, 0, 0);


	listPtr = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, RegisterRegisteredPalmsList));
//	buttonPtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, RegisterRegisteredPalmsButton));
	fieldPtr = (FieldType *)FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, RegisterNewNameField));

	switch (event->eType)
	{
   	case ctlSelectEvent:  // A control button was pressed and released.
			
	   	// If the done button is pressed, go back to the Table form.
	   	if(event->data.ctlEnter.controlID == RegisterOkayButton)
	   	{
				StrCopy(RegTemp.newName, FldGetTextPtr(fieldPtr));
				//	Ensure its terminated at max length
				RegTemp.newName[PalmRegistrationNameLength] = 0;
				if(StrLen(RegTemp.newName) == 0)
				{
					RegTemp.flags &= ~RegReplacePalm;
					RegTemp.uniqueID = 0;
				}
				
//ShowMessage("Palm = %s\nID = %d\nFlags = %d", RegTemp.syncPrintingToName, RegTemp.syncPrintingToID, RegTemp.flags);
				if(RegTemp.syncPrintingToID == 0)
				{
					RegTemp.flags &= ~RegSyncPrinting;
				}
//ShowMessage("Palm = %s\nID = %d\nFlags = %d", RegTemp.syncPrintingToName, RegTemp.syncPrintingToID, RegTemp.flags);


				//	If HLInfoSyncStatus bit has been set, we want a Palm list, not Registration
				if(RegTemp.forceInfoSync)
				{
					Prefs.hLStatus.HLRegisterStatus	= HLStatusAllDone;
					Prefs.hLStatus.HLInfoSyncStatus	= HLStatusToSend;	//	Refresh InfoSync list
				}
				else
				{
					Prefs.hLStatus.HLRegisterStatus	= HLStatusToSend;
				}


				//	If there is no RegTemp name, then bail out
				if(StrLen(RegTemp.newName) == 0)
				{
					ShowMessage("No Palm Registration Name has been entered");
				}
				else
				{
					FrmGotoForm(BeamingForm);
				}
	   	}

	   	
	   	
	   	// If the Replace Existing Palm checkbox is pressed
	   	if(event->data.ctlEnter.controlID == RegisterReplaceCheckbox)
	   	{
				//	Checkbox has just been checked
				if(FrmGetControlValue(frm, FrmGetObjectIndex (frm, RegisterReplaceCheckbox)))
				{
					//	Get the Palm selected
					itemSelected = MmPopupList(listPtr, PalmsList, &offset, 0);

					if(itemSelected == 0)
					{
						RegTemp.forceInfoSync = true;
						MmResetHLStatusToAllDone(&Prefs, KeepNone);	//	Reset the lot
						Prefs.hLStatus.HLInfoSyncStatus = HLStatusToSend;	//	Force an InfoSync

						StrCopy(RegTemp.newName, FldGetTextPtr(fieldPtr));
						//	Ensure its terminated at max length
						RegTemp.newName[PalmRegistrationNameLength] = 0;

						FrmGotoForm(BeamingForm);
					}

					else if(itemSelected == NoValue16)
					{
						FrmSetControlValue(frm, FrmGetObjectIndex (frm, RegisterReplaceCheckbox), 0);
					}

					else
					{
						RegTemp.flags |= RegReplacePalm;

						// Get the handle for the string and unlock it by removing it from the field.
						fieldHandle = FldGetTextHandle(fieldPtr);
						FldSetTextHandle(fieldPtr, NULL);
						 
						//	Resize the chunk if necessary, lock the chunk, write to it, unlock it
						MemHandleResize(fieldHandle, StrLen(ListItems[itemSelected])+1);

						textStr = (char *) MemHandleLock(fieldHandle);
						StrCopy(textStr, ListItems[itemSelected]);
						MemHandleUnlock(fieldHandle);
						 
						// Update the text in the field.
						FldSetTextHandle (fieldPtr, fieldHandle);
						FldDrawField(fieldPtr);

						//	Save the Name and ID into the RegTemp struct
						StrCopy(RegTemp.newName, FldGetTextPtr(fieldPtr));
						itemSelected += offset -1;					//	Is non zero if list starts higher
						MmGetStaffID(PalmsType, &RegTemp.uniqueID, itemSelected, MmInfoDB);
					}
				}
				//	Checkbox has just been UN-checked
				else
				{
					RegTemp.flags &= ~RegReplacePalm;

					// Get the handle for the string and unlock it by removing it from the field.
					fieldHandle = FldGetTextHandle(fieldPtr);
					FldSetTextHandle(fieldPtr, NULL);
					 
					//	Resize the chunk if necessary, lock the chunk, write to it, unlock it
					MemHandleResize(fieldHandle, PalmRegistrationNameLength+1);

					textStr = (char *) MemHandleLock(fieldHandle);
					StrCopy(textStr, "");
					MemHandleUnlock(fieldHandle);
					 
					// Update the text in the field.
					FldSetTextHandle (fieldPtr, fieldHandle);
					FldDrawField(fieldPtr);

					RegTemp.uniqueID = 0;
				}
			}

	   	


	   	// If the Sync Printing to: checkbox is pressed
	   	if(event->data.ctlEnter.controlID == RegisterSyncPrintCheckbox)
	   	{
				//	Checkbox has just been checked
				if(FrmGetControlValue(frm, FrmGetObjectIndex (frm, RegisterSyncPrintCheckbox)))
				{
					//	Get the Palm selected
					itemSelected = MmPopupList(listPtr, PalmsList, &offset, 0);

					if(itemSelected == 0)
					{
						RegTemp.forceInfoSync = true;
						MmResetHLStatusToAllDone(&Prefs, KeepNone);	//	Reset the lot
						Prefs.hLStatus.HLInfoSyncStatus = HLStatusToSend;	//	Force an InfoSync

						StrCopy(RegTemp.newName, FldGetTextPtr(fieldPtr));
						//	Ensure its terminated at max length
						RegTemp.newName[PalmRegistrationNameLength] = 0;

						FrmGotoForm(BeamingForm);
					}

					else if(itemSelected == NoValue16)
					{
						FrmSetControlValue(frm, FrmGetObjectIndex (frm, RegisterSyncPrintCheckbox), 0);
					}

					//	Valid Palm has been selected from the list
					else
					{
						RegTemp.flags |= RegSyncPrinting;

						FrmHideObject(frm, FrmGetObjectIndex(frm, RegisterSyncPrintLabel));
						FrmCopyLabel(frm, RegisterSyncPrintLabel, ListItems[itemSelected]);
						FrmShowObject(frm, FrmGetObjectIndex(frm, RegisterSyncPrintLabel));

						//	Save the Name and ID into the RegTemp struct
						StrCopy(RegTemp.syncPrintingToName, ListItems[itemSelected]);
						itemSelected += offset -1;					//	Is non zero if list starts higher
						MmGetStaffID(PalmsType, &RegTemp.syncPrintingToID, itemSelected, MmInfoDB);
//ShowMessage("Palm = %s\nID = %d\nFlags = %d", RegTemp.syncPrintingToName, RegTemp.syncPrintingToID, RegTemp.flags);
					}
				}
				//	Checkbox has just been UN-checked
				else
				{
					RegTemp.flags &= ~RegSyncPrinting;

					//	Clear the Name label
					FrmHideObject(frm, FrmGetObjectIndex(frm, RegisterSyncPrintLabel));
					FrmCopyLabel(frm, RegisterSyncPrintLabel, "");
					FrmShowObject(frm, FrmGetObjectIndex(frm, RegisterSyncPrintLabel));
					
					StrCopy(RegTemp.syncPrintingToName, "");
					RegTemp.syncPrintingToID = 0;
				}
			}

	   	


	   	// If the Cancel button is pressed, go back to the Table form.
	   	if(event->data.ctlEnter.controlID == RegisterCancelButton)
	   	{
				StrCopy(RegTemp.newName, FldGetTextPtr(fieldPtr));
				//	Ensure its terminated at max length
				RegTemp.newName[PalmRegistrationNameLength] = 0;

				Prefs.hLStatus.HLRegisterStatus	= HLStatusAllDone;
				RegTemp.registering	= false;	//	All over; Go back to normal form

				PreviousForm = OverviewForm;
	   		FrmGotoForm(TableForm);
	   	}

	   	


	   	// If the PIN button is pressed, go back to the PIN form.
	   	if(event->data.ctlEnter.controlID == RegisterPINButton)
	   	{
				StrCopy(RegTemp.newName, FldGetTextPtr(fieldPtr));
				//	Ensure its terminated at max length
				RegTemp.newName[PalmRegistrationNameLength] = 0;

				Prefs.hLStatus.HLRegisterStatus	= HLStatusAllDone;
				RegTemp.registering	= false;	//	All over; Go back to normal form

	   		PreviousForm = RegisterForm;	//	Goto RegisterForm after PIN entry
				FrmGotoForm(EnterPINForm);
	   	}


			handled = true;
			break;




  		case frmOpenEvent:	// The form was told to open.

  			// Draw the form with the text field data in place.
			FrmDrawForm(frm);

			//	Use this test to decide if they are part way through doing a registration
			if(StrLen(RegTemp.newName))
			{
				if(RegTemp.flags & RegReplacePalm)
					FrmSetControlValue(frm, FrmGetObjectIndex (frm, RegisterReplaceCheckbox), 1);

				if(RegTemp.flags & RegSyncPrinting)
					FrmSetControlValue(frm, FrmGetObjectIndex (frm, RegisterSyncPrintCheckbox), 1);

				FrmCopyLabel(frm, RegisterSyncPrintLabel, RegTemp.syncPrintingToName);
			}
			else
			{
				if(StrLen(Prefs.palmRegistrationName) &&
						StrCaselessCompare((const char *)Prefs.palmRegistrationName, "Unregistered")	)
				{
					StrCopy(RegTemp.newName, Prefs.palmRegistrationName);
				}
				RegTemp.forceInfoSync		= false;
				RegTemp.palmListChanged		= false;
				RegTemp.uniqueID				= 0;
				RegTemp.flags					= 0;
				RegTemp.syncPrintingToID	= 0;
				StrCopy(RegTemp.syncPrintingToName, "");
			}



			fieldHandle = MemHandleNew(PalmRegistrationNameLength+1);
			textStr = (char *) MemHandleLock(fieldHandle);

			MemHandleResize(fieldHandle, StrLen(RegTemp.newName)+1);

			StrCopy(textStr, RegTemp.newName);

			MemHandleUnlock(fieldHandle);

			oldHandle = FldGetTextHandle(fieldPtr);
			FldSetTextHandle(fieldPtr, fieldHandle);
			if(oldHandle)
			{
				MemHandleFree(oldHandle);
			}

//			FrmSetFocus(frm, fieldIndex);			// Set the insertion point blinking in the note field
			FldDrawField(fieldPtr);

			RegTemp.registering = true;			//	Keep coming back here until finished

			handled = true;
			break;
	}
	return(handled);
}



/***********************************************************************
 *
 * FUNCTION:		MmCallAwayHandleEvent
 *
 * DESCRIPTION:	Handles processing of events for the CallAway form.
 *
 * PARAMETERS:		event	- the most recent event.
 *
 * RETURNED:		True if the event is handled, false otherwise.
 *
 ***********************************************************************/
Boolean MmCallAwayHandleEvent(EventPtr event)
{
	FormPtr						frm;
	UInt32						timeNow;
	Boolean						handled = false;
	
	Boolean						recFound = false;

	ListPtr						listPtr;
	UInt16						itemSelected;
	UInt16						offset = 0;
	
	UInt16						optionsInCourse = 0;
	UInt16						item = 0;				//	Counter to step through list items or Options
	UInt16						i = 0;					//	Local Counter

	Boolean						fits = true;



	frm = FrmGetActiveForm();

	timeNow = MmProcessEventIncludingNilEvent(frm, event, 0, 0);


	listPtr = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, CallAwayChoicesList));


	switch (event->eType)
	{
   	case ctlSelectEvent:  // A control button was pressed and released.
			
	   	// If the Beam button is pressed, go to the Beaming form.
	   	if(event->data.ctlEnter.controlID == CallAwayBeamButton)
	   	{
				if(Prefs.loggedIn)
				{
					PreviousForm = OverviewForm;
					FrmGotoForm(BeamingForm);
				}

				//	Not logged in
				else
				{
					FrmAlert(WarnLoginNotLoggedInAlert);
				}
	   	}

	   	// If the Cancel button is pressed, go back to the Table form.
	   	else if(event->data.ctlEnter.controlID == CallAwayCancelButton)
	   	{
				MmDeleteAllTypeRecordsForMiscDataType(MmMiscDataDB, CallAwayType);

				PreviousForm = OverviewForm;
	   		FrmGotoForm(TableForm);
	   	}

			handled = true;
			break;


		case lstSelectEvent:

			itemSelected = LstGetSelection(listPtr);

			if(StrCaselessCompare((const char *)ListItemsStruct[0].trimName, "None Available") == 0)
			{
				itemSelected = NoValue16;
			}

			if(itemSelected != NoValue16)
			{
				MmProcessCallAwayRecords(&DBs, &Prefs, ListItemsStruct[itemSelected].recIndex);
			}

			
			//	Count CallAway records to see if any exist, and set the Button label accordingly
			if(MmCallAwayRecordExists(&MmMiscDataDB) != NoValue16)
				FrmShowObject(frm, FrmGetObjectIndex(frm, CallAwayBeamButton));
			else
				FrmHideObject(frm, FrmGetObjectIndex(frm, CallAwayBeamButton));



			MmRedrawList(listPtr, CallAwayList, 0, &ListItems[0], &Prefs);

			handled = true;
			break;



  		case frmOpenEvent:	// The form was told to open.

  			// Draw the form with the text field data in place.
			FrmDrawForm(frm);

			
			//	Count CallAway records to see if any exist, and set the Button label accordingly
			if(MmCallAwayRecordExists(&MmMiscDataDB) != NoValue16)
				FrmShowObject(frm, FrmGetObjectIndex(frm, CallAwayBeamButton));
			else
				FrmHideObject(frm, FrmGetObjectIndex(frm, CallAwayBeamButton));


			MmRedrawList(listPtr, CallAwayList, 0, &ListItems[0], &Prefs);

			handled = true;
			break;
	}
	return(handled);
}



/***********************************************************************
 *
 * FUNCTION:		MmUserSettingsHandleEvent
 *
 * DESCRIPTION:	Handles processing of events for the UserSettings form.
 *
 * PARAMETERS:		event	- the most recent event.
 *
 * RETURNED:		True if the event is handled, false otherwise.
 *
 ***********************************************************************/
Boolean MmUserSettingsHandleEvent(EventPtr event)
{
	FormPtr						frm;

	UInt32						timeNow;
	Boolean						handled = false;
	
	ListPtr						listPtr;
	UInt16						offset = 0;
	
//	Err							err;
		
	UInt16						item = 0;				//	Counter to step through list items or Options
	UInt16						i = 0;					//	Local Counter

//	Int16							listWidth;
//	Int16							textLen;
	Boolean						fits = true;
//	Char							tempStr[MaxLinkedItems+1];



	frm = FrmGetActiveForm();
//	formId = FrmGetFormId(frm);

	timeNow = MmProcessEventIncludingNilEvent(frm, event, 0, 0);

	listPtr = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, UserSettingsChoicesList));


	switch (event->eType)
	{
   	case ctlSelectEvent:  // A control button was pressed and released.
			
	   	// If the done button is pressed, go back to the Table form.
	   	if(event->data.ctlEnter.controlID == UserSettingsOkayButton)
	   	{
				//	If we're in Classic Display Mode
				if(Prefs.userSettings.classicMode)
				{
					OverviewPageType = OverviewOrdersOnlyPage;
					PreviousForm = OverviewForm;
		   		FrmGotoForm(MainForm);
		   	}
		   	
				//	We're in Modern Display (Serving Course) Mode
		   	else
		   	{
					if(PreviousForm == MainForm)
						PreviousForm = OverviewForm;
		   		FrmGotoForm(PreviousForm);
		   	}


	   	}
	   	
	   	//	All other buttons are Pop Triggers
	   	else
	   	{
				MmSetUserSettings(frm, &Prefs, ListItems, listPtr, event->data.ctlEnter.controlID, 
											&SoundLevel, &ScreenLockStruct);
			}

			handled = true;
			break;


 

  		case keyDownEvent:

			handled = true;
			break;




  		case frmOpenEvent:	// The form was told to open.

  			// Draw the form with the text field data in place.
			FrmDrawForm(frm);

			MmSetUserSettings(frm, &Prefs, ListItems, listPtr, NoValue16, 
											&SoundLevel, &ScreenLockStruct);

			if(!Prefs.loggedIn)
				ShowMessage("Warning.\n\nUser settings will be lost unless you log in first.");
			
			handled = true;
			break;
	}
	return(handled);
}



/***********************************************************************
 *
 * FUNCTION:    MmSetCurrentTableNumber
 *
 * DESCRIPTION: Set Current Table Number from user dependant upon push buttons
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/
/*static void MmSetCurrentTableNumber (FormPtr frm, Char* numberStrTable)
{
	StrCopy(numberStrTable, FrmGetLabel(frm,TableTablenoLabel));

	if(numberStrTable[0] == ' ')
		Prefs.current.tableNo = StrAToI(numberStrTable+1);	// Avoid space char
	else
		Prefs.current.tableNo = StrAToI(numberStrTable);		// Save for later

	if(Prefs.current.tableNo == 0)
		Prefs.current.tableNo = 1;
}
*/

/***********************************************************************
 *
 * FUNCTION:    MmSetCurrentSeatNumber
 *
 * DESCRIPTION: Set Current Table Number from user dependant upon push buttons
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/
/*static void MmSetCurrentSeatNumber (FormPtr frm, Char* numberStrSeat)
{
	StrCopy(numberStrSeat, FrmGetLabel(frm,SeatSeatnoLabel));
	if(numberStrSeat[0] == ' ')
		Prefs.current.seatNo = StrAToI(numberStrSeat+1);	// Avoid space char
	else
		Prefs.current.seatNo = StrAToI(numberStrSeat);		// Save for later
	if(Prefs.current.seatNo == 0)
		Prefs.current.seatNo = 1;
}
*/




/***********************************************************************
 *
 * FUNCTION:		MmHandleMenuEvent
 *
 * DESCRIPTION:	.
 *
 * PARAMETERS:		event	- the most recent event.
 *
 * RETURNED:		.
 *
 ***********************************************************************/
Boolean MmHandleMenuEvent(FormPtr frm, EventPtr event)
{
	Int16							formID;

	Boolean						handled = false;

//	UInt16						itemSelected;
	
	UInt16						initialiseMode = 0;

//	ListPtr						listPtr;							//	Selected Options list
	
	Char							verNumStr[VersionNumberStringLength + 1] = VersionNumberString;

	formID = FrmGetFormId(frm);


	// First clear the menu status from the display.
	MenuEraseStatus(0);



	// Do the appropriate action for the menu command selected.
	switch (event->data.menu.itemID)
	{
		case MainOptionsBluetoothDevice:
		case TableOptionsBluetoothDevice:
		case OverviewOptionsBluetoothDevice:

			BtSelectDevice(frm, Prefs.btName, Prefs.btSelectedDeviceName, &Prefs.btDevice);
			break;




		case MainOptionsUserSettings:
		case TableOptionsUserSettings:
		case OverviewOptionsUserSettings:

			PreviousForm = FrmGetFormId(frm);
			FrmGotoForm(UserSettingsForm);
			break;





		case MainSetupRegisterPalm:
		case TableSetupRegisterPalm:
		case OverviewSetupRegisterPalm:

			if(FrmAlert(WarnWillNowRegisterPalmAlert) == WarnWillNowRegisterPalmOK)
			{
//						Prefs.palmUniqueID = 0;
//						Prefs.hLStatus.HLRegisterStatus = HLStatusToSend;
	   		PreviousForm = RegisterForm;	//	Goto RegisterForm after PIN entry
				FrmGotoForm(EnterPINForm);
			}

			break;
			

		case MainSetupDeleteDBAllSettings:
		case TableSetupDeleteDBAllSettings:
		case OverviewSetupDeleteDBAllSettings:
		
			if(FrmAlert(ConfDeleteDBAndSettingsAlert) == ConfDeleteDBAndSettingsOK)
			{
				initialiseMode = InitResetAllPrefs;
				initialiseMode = initialiseMode | InitKeepBluetoothNames;
			}
		case MainSetupDeleteDatabase:
		case TableSetupDeleteDatabase:
		case OverviewSetupDeleteDatabase:

			if(FrmAlert(ConfDeleteDBAlert) == ConfDeleteDBOK)
			{
				if(initialiseMode == 0)		//	Don't do if InitResetAllPrefs
				{
					initialiseMode = initialiseMode | InitKeepRegistration;
					initialiseMode = initialiseMode | InitKeepBluetoothNames;
				}
				initialiseMode = initialiseMode | InitZapAllDatabases;
				MmInitialiseAll(initialiseMode);
				
				if(formID == MainForm)
				{
					MmDisplayTableSeatLabel(frm, Prefs.current.tableNo, MainTablenoLabel, &Prefs,
															MmTablenameDB, MmSeatnameDB);
					MmDisplayTableSeatLabel(frm, Prefs.current.seatNo, MainSeatnoLabel, &Prefs,
															MmTablenameDB, MmSeatnameDB);
					MmMainviewRedraw(frm, ScrollTop, DoRewriteArray);
				}
				else if(formID == OverviewForm)
				{
					MmOverviewRedraw(frm, ScrollTop, DoRewriteArray);
				}
			}

			break;
			

		case MainSetupFindAllBluetoothDevices:
		case TableSetupFindAllBluetoothDevices:
		case OverviewSetupFindAllBluetoothDevices:

			MmDiscoverBtDevices(frm, BtNewDiscoveryList, &BtLibRefNum,
																	&BtPortId, &Prefs, &ReDiscoverBt);
			break;


		case MainSetupFindMoreBluetoothDevices:
		case TableSetupFindMoreBluetoothDevices:
		case OverviewSetupFindMoreBluetoothDevices:

			MmDiscoverBtDevices(frm, BtShowLastDiscoveryList, &BtLibRefNum,
																	&BtPortId, &Prefs, &ReDiscoverBt);
			break;





		case MainAboutSystemInfo:
		case TableAboutSystemInfo:
		case OverviewAboutSystemInfo:

			OverviewPageType = OverviewTakeOrders;
			FrmGotoForm(SystemInfoForm);

			break;


		case MainAboutAboutMenuMate:
		case TableAboutAboutMenuMate:
		case OverviewAboutAboutMenuMate:
			// Load the info form, then display it.
			frm = FrmInitForm(AboutMenuMateForm);
			
			FrmCopyLabel(frm, AboutMenuMateVersionNumLabel, verNumStr);

			// If the Okay button was pressed
			FrmDoDialog(frm);
			
			// Delete the info form.
	 		FrmDeleteForm(frm);
			break;

	}
	return(handled);
}


/***********************************************************************
 *
 * FUNCTION:		MmNumberEntryFieldEvent
 *
 * DESCRIPTION:	Handles processing of events for the NumberEntryField form.
 *
 * PARAMETERS:		event	- the most recent event.
 *
 * RETURNED:		True if the event is handled, false otherwise.
 *
 ***********************************************************************/
Boolean MmNumberEntryFieldEvent(EventPtr event)
{
	FormPtr						frm;
	UInt32						timeNow;
	Boolean						handled = false;

	MemHandle					fieldHandle, oldHandle;
	char							*textStr;
	UInt16						fieldIndex;
	FieldType					*fieldPtr;

	UInt32						number;




/*
If you add no further code at all, the field will accept user input, and will allocate
and resize memory as appropriate to hold the user's input, and will free that memory
when the form is closed - in other words, by default you need to do nothing but read
the user's input at the appropriate time by calling FldGetTextHandle or FldGetTextPtr,
and perhaps FldCompactText first. (Make sure to copy out the text, though, since the
pointer might well change if the user enters more text.)

Also, do not modify the data in the string directly, unless you again call
FldSetTextHandle or FldSetTextPtr immediately after doing so. Fields keep some
internal information about the string, such as its length, where line breaks are,
and so forth, so if you change the text without telling the field you may well get
bad results or error messages when the field notices that something is wrong.

To initialize the text in a field use the following:
*/
 

	frm = FrmGetActiveForm();

	timeNow = MmProcessEventIncludingNilEvent(frm, event, 0, 0);


	fieldIndex = FrmGetObjectIndex(frm, NumberEntryFieldNumberEntryField);
	fieldPtr = (FieldType *)FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, NumberEntryFieldNumberEntryField));


	switch (event->eType)
	{
		case ctlSelectEvent:  // A control button was pressed and released.
			
	   	// If the done button is pressed, go back to the Table form.
	   	if(event->data.ctlEnter.controlID == NumberEntryFieldOkayButton)
	   	{
				textStr = FldGetTextPtr(fieldPtr);
				
				number = StrAToI(textStr);
				if(number > 65535)
					number = 65535;

				switch(NumericalEntryItem)
				{
					case BlueToothSetFirstByteTOut:
						Prefs.btTimeoutFirstByte = number;
						break;
					case BlueToothSetInterByteTOut:
						Prefs.btTimeoutInterByte = number;
						break;
					case BlueToothSetSendOrdersTOut:
						Prefs.btTimeoutSendOrders = number;
						break;
					case BlueToothSetGetAllOrdersTOut:
						Prefs.btTimeoutGetAllOrders = number;
						break;

					case ForceNumberOfTables:
						GenTableOrders = number;
						break;
					case ForceNumberOfSeats:
						GenSeatOrders = number;
						break;
					case ForceNumberOfOrdersPerSeat:
						GenOrdersPerSeat = number;
						break;
					case ForceNumberOfSeatNames:
						GenSeatnames = number;
						break;
				}

				FrmReturnToForm(DebugDataForm);
	   	}
			handled = true;
			break;



		case frmOpenEvent:	// The form was told to open.

  			// Draw the form with the text field data in place.
			FrmDrawForm(frm);

			fieldHandle = MemHandleNew(50);
			textStr = (char *) MemHandleLock(fieldHandle);


			switch(NumericalEntryItem)
			{
				case BlueToothSetFirstByteTOut:
					StrIToA(textStr, Prefs.btTimeoutFirstByte);
					break;
				case BlueToothSetInterByteTOut:
					StrIToA(textStr, Prefs.btTimeoutInterByte);
					break;
				case BlueToothSetSendOrdersTOut:
					StrIToA(textStr, Prefs.btTimeoutSendOrders);
					break;
				case BlueToothSetGetAllOrdersTOut:
					StrIToA(textStr, Prefs.btTimeoutGetAllOrders);
					break;

				case ForceNumberOfTables:
					StrIToA(textStr, GenTableOrders);
					break;
				case ForceNumberOfSeats:
					StrIToA(textStr, GenSeatOrders);
					break;
				case ForceNumberOfOrdersPerSeat:
					StrIToA(textStr, GenOrdersPerSeat);
					break;
				case ForceNumberOfSeatNames:
					StrIToA(textStr, GenSeatnames);
					break;
			}

			MemHandleResize(fieldHandle, StrLen(textStr)+1);
			MemHandleUnlock(fieldHandle);

			oldHandle = FldGetTextHandle(fieldPtr);
			FldSetTextHandle(fieldPtr, fieldHandle);
			if(oldHandle)
			{
				MemHandleFree(oldHandle); 			// or do something else with it first
			}

			FrmSetFocus(frm, fieldIndex);			// Set the insertion point blinking in the note field
			FldDrawField(fieldPtr);

			handled = true;
			break;
	}
	return(handled);
}





/***********************************************************************
 *
 * FUNCTION:		MmSetTableLabels
 *
 * DESCRIPTION:	.
 *
 * PARAMETERS:		.
 *
 * RETURNED:		
 *
 ***********************************************************************/
static void MmSetTableLabels(FormPtr frm, Char numberStrTable[4])
{
	UInt16					recIndex;
	
	Boolean					found;
	
	Char						tableTitleStr[MaxTableLabelSize + 1] = "Table ";
	Char						tableNoStr[MaxTableLabelSize + 5];

	ControlType				*buttonPtr;

	//	Copy the table Number
	buttonPtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, TableTableNoPushButton));

	StrCopy(tableNoStr, "Table No.");

	StrCat(tableNoStr, numberStrTable);
	CtlSetLabel(buttonPtr, tableNoStr);

//	MmSetCurrentTableNumber(frm, numberStrTable);
	if(TableNoTemp == 0)
		TableNoTemp = NoValue16;

	if(TableNoTemp != NoValue16)
	{
		Prefs.current.tableNo = TableNoTemp;
	}

	//	Copy the table Name
	found = MmGetTableNameIndex(MmTablenameDB, numberStrTable, &recIndex);
	MmDisplayPopNameLabel(frm, &DBs, MmTablenameDB, 
										recIndex, TableNamePopTrigger, found);
	MmShowTablePrice(frm, TablePriceLabel, MmOrdersDB, &Prefs);

	StrCat(tableTitleStr, numberStrTable);
	FrmCopyTitle(frm, tableTitleStr);
}


/***********************************************************************
 *
 * FUNCTION:		MmSetSeatLabels
 *
 * DESCRIPTION:	.
 *
 * PARAMETERS:		.
 *
 * RETURNED:		
 *
 ***********************************************************************/
static void MmSetSeatLabels(FormPtr frm, Char numberStrSeat[4])
{
	UInt16					recIndex;
	
	Boolean					found;
	
	Char						tableTitleStr[11] = "Table ";
	Char						numberStrTable[4];
	Char						seatNoStr[MaxSeatLabelSize + 4];

	ControlType				*buttonPtr;

	//	Copy the seat Number
	buttonPtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, SeatSeatNoPushButton));

	StrCopy(seatNoStr, "Seat No.");

	if(StrLen(Prefs.defaultSeatname))
		StrNCopy(seatNoStr, Prefs.defaultSeatname, MaxSeatLabelSize);	//	Limit size to button width

	StrCat(seatNoStr, numberStrSeat);
	CtlSetLabel(buttonPtr, seatNoStr);

//	MmSetCurrentTableNumber(frm, numberStrTable);
	if(SeatNoTemp == 0)
		SeatNoTemp = NoValue16;

	if(SeatNoTemp != NoValue16)
	{
		Prefs.current.seatNo = SeatNoTemp;
	}

	MmShowSeatPrice(frm, SeatPriceLabel, MmOrdersDB, &Prefs);

	//	Copy the seat Name
	found = MmGetSeatnameIndexAndUpdate(&DBs, Prefs.current.tableNo,
										Prefs.current.seatNo, &recIndex, "", DontMarkForDelete);

	MmDisplayPopNameLabel(frm, &DBs, MmSeatnameDB, 
//										MmDrinksSizesDB, MmDishesSizesDB,
//										MmTablenameDB, MmSeatnameDB,
										recIndex, SeatNamePopTrigger, found);

	//	Update the New/Edit button label
	buttonPtr = FrmGetObjectPtr(frm, FrmGetObjectIndex (frm, SeatNewNameButton));
	if(found)
		CtlSetLabel(buttonPtr, "Edit");
	else
		CtlSetLabel(buttonPtr, "New");


	StrIToA(numberStrTable, Prefs.current.tableNo);
	StrCat(tableTitleStr, numberStrTable);
	FrmCopyTitle(frm, tableTitleStr);
}





/***********************************************************************
 *
 * FUNCTION:		MmTableOrSeatHandleEvent
 *
 * DESCRIPTION:	Handles processing of events for the Seat form.
 *
 * PARAMETERS:		event	- the most recent event.
 *
 * RETURNED:		True if the event is handled, false otherwise.
 *
 ***********************************************************************/
Boolean MmTableOrSeatHandleEvent(EventPtr event)
{
	FormPtr						frm;
	UInt16						formID;
	
	Boolean						handled = false;
	Boolean						found;
	Boolean						isTableButton;
	Boolean						isSeatButton;

	Char							numberStrTable[4];
	Char							numberStrSeat[4];
//	Char							spaces[] = "     ";
	Char							tempStr[] = "Tables";
	Char							tableNoStr[MaxTableLabelSize + 5];
	Char							seatNoStr[MaxSeatLabelSize + 5];

	UInt16						sendIndex = 0;

	ListPtr						listPtr;
	UInt16						itemSelected;
	UInt16						offset;
	
	UInt16						recIndex;
	
	UInt16						buttonValue = NoValue16;
	
	ControlType					*buttonPtr;

	UInt32						timeNow;
	

	frm = FrmGetActiveForm();

	formID = FrmGetActiveFormID();

	if(formID == TableForm)
		timeNow = MmProcessEventIncludingNilEvent(frm, event, 0, TableStaffPopTrigger);
	else
		timeNow = MmProcessEventIncludingNilEvent(frm, event, 0, 0);


	numberStrTable[2] = 0;								// Terminate string



	switch(event->eType)
	{
   	case ctlSelectEvent:  // A control button was pressed and released.
			
	   	// If the done button is pressed, go back to the main form.
	   	if(event->data.ctlEnter.controlID == TableOkayButton		||
	   		event->data.ctlEnter.controlID == TableMenuMateButton		)
	   	{
				// If they change table go to Seat 1.
				if(Prefs.current.tableNo != OldCurrentTable)
				{
					Prefs.current.seatNo = 1;
				}


	   		if(PreviousForm == OverviewForm)
					OverviewPageType = OverviewTakeOrders;
	   			
	   		//	Avoid getting stuck in Table/Seat ping pong if Palm goes to sleep in the Seats form
	   		if(PreviousForm == SeatForm)
	   			PreviousForm = OverviewForm;
	   		
				if(formID == TableForm && Prefs.userSettings.classicMode)
				{
		   		PreviousForm = MainForm;
				}

	   		FrmGotoForm(PreviousForm);
	   		PreviousForm = TableForm;
				handled = true;
			}


	   	else if(event->data.ctlEnter.controlID == SeatOkayButton)
	   	{
	   		FrmGotoForm(PreviousForm);
	   		PreviousForm = SeatForm;
				handled = true;
			}



/*			//	Go direct to Overview page from the Tables or Seat page
			else if(	event->data.ctlEnter.controlID == TableOverviewButton)
	  		{
				PreviousForm = TableForm;
				OverviewPageType = OverviewTakeOrders;
				FrmGotoForm(OverviewForm);

				handled = true;
			}
*/	







	   	// If the Staff trigger is pressed, pop up the Staff list.
	   	else if(event->data.ctlEnter.controlID == TableStaffPopTrigger)
	   	{
				//	Check if any unsent orders exist. false -> check for OLD and NEW unsent orders
				if(MmWarnIfUnsentOrders(MmOrdersDB, &Prefs, false, &NumOldUnsentOrders))
				{
					//	Log them out so they can't send orders through on previous account
					Prefs.loggedIn = false;

					OverviewPageType = OverviewUnsentPage;
					OverviewCheckboxType = OverviewCheckboxesUnsent;
		   		PreviousForm = TableForm;
					FrmGotoForm(OverviewForm);
				}
				else
				{
			   	TableNoTemp = NoValue16;

					listPtr = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, TableNamesList));

					//	Then get the Staff member selected
					itemSelected = MmPopupList(listPtr, StaffList, &offset, 0);

					//	Aborted Login
					if(itemSelected == NoValue16)
					{
						MmDisplayStaffNameLabel(frm, MmInfoDB, Prefs.staffIDIndex, TableStaffPopTrigger, 
															OverviewPageType, Prefs.loggedIn);
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
			   		PreviousForm = TableForm;
						FrmGotoForm(EnterPINForm);					//	Get their PIN number
					}
				}
				handled = true;
			}





			else if(event->data.ctlEnter.controlID == TableTableNoPushButton)
	  		{
		   	TableNoTemp = NoValue16;

				FrmSetControlGroupSelection(frm, TableGroupID, TableTableNoPushButton);

				// Display the table number
				MmItemToString(Prefs.current.tableNo, numberStrTable, DontCheckList, &ListItems[0]);
				MmSetTableLabels(frm, numberStrTable);
			}
			
			else if(event->data.ctlEnter.controlID == TableGuestsNoPushButton)
	  		{
		   	TableNoTemp = NoValue16;

				FrmSetControlGroupSelection(frm, TableGroupID, TableGuestsNoPushButton);

//!!!!!!!!!!!!!!!!!!!!
				// Display the table number
				MmItemToString(Prefs.current.tableNo, numberStrTable, DontCheckList, &ListItems[0]);
				MmSetTableLabels(frm, numberStrTable);
			}





			else if(event->data.ctlEnter.controlID == SeatSeatNoPushButton)
	  		{
				// Re-Display the seat number
				MmItemToString(Prefs.current.seatNo, numberStrSeat, DontCheckList, &ListItems[0]);
				MmSetSeatLabels(frm, numberStrSeat);
			}
			
			





			else if(event->data.ctlEnter.controlID == TableCashButton)
			{
				Prefs.isCashMode = true;
				
				Prefs.current.tableNo = CashTable;
				Prefs.current.seatNo = 1;

				// Remove the Table form and display the Main form.
				FrmGotoForm(PreviousForm);
	   		PreviousForm = TableForm;


				handled = true;
			}




			else if(event->data.ctlEnter.controlID == TableCallAwayButton)
			{
				FrmGotoForm(CallAwayForm);

				handled = true;
			}
			


			else if(event->data.ctlEnter.controlID == TableHappyHourButton)
	  		{
				ShowMessage("Happy Hour pricing applies");
			}
				




			else if(event->data.ctlEnter.controlID == TableActiveButton)
			{
		   	TableNoTemp = NoValue16;

				listPtr = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, TableNamesList));
	
				itemSelected = MmPopupList(listPtr, ActiveTablesList, &offset, 0);
				
				if(itemSelected != NoValue16)
				{
					Prefs.current.tableNo = itemSelected;
						
					if(MmItemToString(Prefs.current.tableNo, numberStrTable, CheckList, &ListItems[0]))
					{
						MmSetTableLabels(frm, numberStrTable);

						// Remove the Table form and display the Main or Orders form.
						if(OldCurrentTable != Prefs.current.tableNo)
						{
							Prefs.current.seatNo = 1;
						}
		
			   		if(PreviousForm == OverviewForm)
							OverviewPageType = OverviewTakeOrders;
	   			
			   		FrmGotoForm(PreviousForm);
			   		PreviousForm = TableForm;
					}
				}
				handled = true;
			}

				
			else if(event->data.ctlEnter.controlID == SeatActiveButton)
			{
		   	SeatNoTemp = NoValue16;

				listPtr = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, SeatNamesList));
	
				itemSelected = MmPopupList(listPtr, ActiveSeatsList, &offset, 0);
				
				if(itemSelected != NoValue16)
				{
					Prefs.current.seatNo = itemSelected;
				
					if(MmItemToString(Prefs.current.seatNo, numberStrSeat, CheckList, &ListItems[0]))
					{
						MmSetSeatLabels(frm, numberStrSeat);

						// Remove the Seat form and display the Main form.
			   		FrmGotoForm(PreviousForm);
			   		PreviousForm = SeatForm;
					}
				}

				handled = true;
			}
				



			else if(event->data.ctlEnter.controlID == SeatNewNameButton)
			{
		   	SeatNoTemp = NoValue16;

				found = MmGetSeatnameIndexAndUpdate(&DBs, Prefs.current.tableNo,
										Prefs.current.seatNo, &recIndex, "", DontMarkForDelete);
				MmEnterSeatName(frm, &recIndex, found, &Prefs, MmSeatnameDB);

				MmItemToString(Prefs.current.seatNo, numberStrSeat, DontCheckList, &ListItems[0]);
				MmSetSeatLabels(frm, numberStrSeat);

				handled = true;
			}




			else if(event->data.ctlSelect.controlID == TableNamePopTrigger)
			{
				TableNoTemp = NoValue16;

				listPtr = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, TableNamesList));
				//	The Table Name pick list was selected
				itemSelected = MmPopupList(listPtr, TablesList, &offset, 0);
				
				if(itemSelected != NoValue16)
				{
					Prefs.current.tableNo = itemSelected;
					MmItemToString(itemSelected, numberStrTable, CheckList, &ListItems[0]);
					MmSetTableLabels(frm, numberStrTable);
	
					// Remove the Table form and display the Main or Orders form.
					if(OldCurrentTable != Prefs.current.tableNo)
					{
						Prefs.current.seatNo = 1;
					}
			
		   		if(PreviousForm == OverviewForm)
						OverviewPageType = OverviewTakeOrders;
		   			
		   		FrmGotoForm(PreviousForm);
		   		PreviousForm = TableForm;
				}
				else
				{
					MmItemToString(Prefs.current.tableNo, numberStrTable, DontCheckList, &ListItems[0]);
					MmSetTableLabels(frm, numberStrTable);
				}

				handled = true;
			}
			




			else if(event->data.ctlEnter.controlID == SeatNamePopTrigger)
			{
				SeatNoTemp = NoValue16;

				listPtr = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, SeatNamesList));

				//	The Seat Name pick list was selected
				itemSelected = MmPopupList(listPtr, SeatsList, &offset, 0);

				if(itemSelected != NoValue16)
				{
					Prefs.current.seatNo = itemSelected;

					MmItemToString(itemSelected, numberStrSeat, CheckList, &ListItems[0]);
					MmSetSeatLabels(frm, numberStrSeat);

					// Remove the Seat form and display the Main form.
					FrmGotoForm(PreviousForm);
		   		PreviousForm = SeatForm;
				}
				else
				{
					MmItemToString(Prefs.current.seatNo, numberStrSeat, DontCheckList, &ListItems[0]);
					MmSetSeatLabels(frm, numberStrSeat);
				}


				handled = true;
			}





	   	// If the Clear button is pressed, clear the display.
	   	else if(event->data.ctlEnter.controlID == TableClearButton)
	   	{
		   	TableNoTemp = NoValue16;

				//	Copy Table without the Number
				StrCopy(tableNoStr, "Table No.");

				buttonPtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, TableTableNoPushButton));
				CtlSetLabel(buttonPtr, tableNoStr);

				handled = true;
			}

	   	// If the Clear button is pressed, clear the display.
	   	else if(event->data.ctlEnter.controlID == SeatClearButton)
	   	{
		   	SeatNoTemp = NoValue16;

				//	Copy Seat without the Number
				StrCopy(seatNoStr, "Seat ");

				if(StrLen(Prefs.defaultSeatname))
				{
					StrNCopy(seatNoStr, Prefs.defaultSeatname, MaxSeatLabelSize);	//	Limit size to button width
					StrCat(seatNoStr, " ");
				}
				buttonPtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, SeatSeatNoPushButton));
				CtlSetLabel(buttonPtr, seatNoStr);

				handled = true;
			}





	   	// A Table or Seat number button has been pressed.
			else
			{	
				isTableButton = false;
				isSeatButton = false;


				switch(event->data.ctlEnter.controlID)
				{
			   	case TableOneButton:
			   	case TableTwoButton:
			   	case TableThreeButton:
			   	case TableFourButton:
			   	case TableFiveButton:
			   	case TableSixButton:
			   	case TableSevenButton:
			   	case TableEightButton:
			   	case TableNineButton:
			   	case TableZeroButton:
						isTableButton = true;
						break;
				
			   	case SeatOneButton:
			   	case SeatTwoButton:
			   	case SeatThreeButton:
			   	case SeatFourButton:
			   	case SeatFiveButton:
			   	case SeatSixButton:
			   	case SeatSevenButton:
			   	case SeatEightButton:
			   	case SeatNineButton:
			   	case SeatZeroButton:

						isSeatButton = true;
						break;
				}


				switch(event->data.ctlEnter.controlID)
				{
			   	case TableOneButton:
			   	case SeatOneButton:
			   		buttonValue = 1;
						break;
	
			   	case TableTwoButton:
			   	case SeatTwoButton:
			   		buttonValue = 2;
						break;
	
			   	case TableThreeButton:
			   	case SeatThreeButton:
			   		buttonValue = 3;
						break;
	
			   	case TableFourButton:
			   	case SeatFourButton:
			   		buttonValue = 4;
						break;
	
			   	case TableFiveButton:
			   	case SeatFiveButton:
			   		buttonValue = 5;
						break;
	
			   	case TableSixButton:
			   	case SeatSixButton:
			   		buttonValue = 6;
						break;
	
			   	case TableSevenButton:
			   	case SeatSevenButton:
			   		buttonValue = 7;
						break;
	
			   	case TableEightButton:
			   	case SeatEightButton:
			   		buttonValue = 8;
						break;
	
			   	case TableNineButton:
			   	case SeatNineButton:
			   		buttonValue = 9;
						break;

			   	case TableZeroButton:
			   	case SeatZeroButton:
			   		buttonValue = 0;
						break;
				
					default:
						isTableButton = false;
						isSeatButton = false;
				}

				if(buttonValue != NoValue16)
				{
					if(isTableButton)
					{
						if(TableNoTemp == NoValue16)
							TableNoTemp = 0;
						
						//	Avoid potential overrun of permissible Tables
						if(TableNoTemp <= ((LastTable - buttonValue) / 10))
						{
							TableNoTemp *= 10;
							TableNoTemp += buttonValue;
						}

						// Display the table number
						MmItemToString(TableNoTemp, numberStrTable, DontCheckList, &ListItems[0]);
						MmSetTableLabels(frm, numberStrTable);
			   	}
					if(isSeatButton)
					{
						if(SeatNoTemp == NoValue16)
							SeatNoTemp = 0;
						
						//	Avoid potential overrun of permissible Tables
						if(SeatNoTemp <= ((LastSeat - buttonValue) / 10))
						{
							SeatNoTemp *= 10;
							SeatNoTemp += buttonValue;
						}
						// Display the seat number
						MmItemToString(SeatNoTemp, numberStrSeat, DontCheckList, &ListItems[0]);
						MmSetSeatLabels(frm, numberStrSeat);
			   	}
				}

				handled = true;
			}



			break;


   	case ctlExitEvent: 	 // A control button was pressed and released OUTSIDE of the bounds.
			
			if(event->data.ctlExit.controlID == TableStaffPopTrigger)
			{
				MmDisplayStaffNameLabel(frm, MmInfoDB, Prefs.staffIDIndex, TableStaffPopTrigger, 
															OverviewPageType, Prefs.loggedIn);
			}
			
			handled = true;
			break;

			


		case keyDownEvent:		// MemHandle the physical buttons


	   	// If the 5 way Select button is pressed, go back to the Ordering form.
			if(NavSelectPressed(event))
	   	{
	   		if(formID == TableForm)
	   		{
					// If they change table go to Seat 1.
					if(Prefs.current.tableNo != OldCurrentTable)
					{
						Prefs.current.seatNo = 1;
					}


		   		if(PreviousForm == OverviewForm)
						OverviewPageType = OverviewTakeOrders;
		   			
		   		//	Avoid getting stuck in Table/Seat ping pong if Palm goes to sleep in the Seats form
		   		if(PreviousForm == SeatForm)
		   			PreviousForm = OverviewForm;
		   		
					if(Prefs.userSettings.classicMode)
			   		PreviousForm = MainForm;

		   		FrmGotoForm(PreviousForm);
		   		PreviousForm = TableForm;
				}
				//	formID == SeatForm
				else
				{
		   		FrmGotoForm(PreviousForm);
		   		PreviousForm = SeatForm;
				}
			   handled = true;
			}



			//	For 5 Way Nav Left Change Table/Seat
			else if(NavDirectionPressed(event, Left))
	   	{
				if(formID == TableForm)
				{
					if(Prefs.current.tableNo > 1)
					{
						TableNoTemp = NoValue16;
						OverviewTopRowNum = 0;					//	Always start at Row 0
						Prefs.current.seatNo = 1;

						Prefs.current.tableNo--;
						MmItemToString(Prefs.current.tableNo, numberStrTable, DontCheckList, &ListItems[0]);
						MmSetTableLabels(frm, numberStrTable);
					}
				}
				else
				{
					if(Prefs.current.seatNo > 1)
					{
						Prefs.current.seatNo--;
						MmItemToString(Prefs.current.seatNo, numberStrSeat, DontCheckList, &ListItems[0]);
						MmSetSeatLabels(frm, numberStrSeat);
					}
				}
			   handled = true;
			}




			//	For 5 Way Nav Right Change Table/Seat
			else if(NavDirectionPressed(event, Right))
	   	{
				if(formID == TableForm)
				{
					if(Prefs.current.tableNo < LastTable)
					{
						TableNoTemp = NoValue16;
						OverviewTopRowNum = 0;					//	Always start at Row 0
						Prefs.current.seatNo = 1;

						Prefs.current.tableNo++;
						MmItemToString(Prefs.current.tableNo, numberStrTable, DontCheckList, &ListItems[0]);
						MmSetTableLabels(frm, numberStrTable);
					}
				}
				else
				{
					if(Prefs.current.seatNo < LastSeat)
					{
						Prefs.current.seatNo++;
						MmItemToString(Prefs.current.seatNo, numberStrSeat, DontCheckList, &ListItems[0]);
						MmSetSeatLabels(frm, numberStrSeat);
					}
				}
			   handled = true;
			}



			//	For 5 Way Nav Up show Display Views list
			else if(NavDirectionPressed(event, Up)	||
						(event->data.keyDown.chr == vchrPageUp)	)
	   	{
				if(formID == TableForm)
				{
					EventType *eventToAdd = (EventType *)MemPtrNew(sizeof(EventType));

					//	Generate events to 'click' the Display Views select button
					MemSet(eventToAdd, sizeof(EventType), 0);
					eventToAdd->eType = ctlSelectEvent;	//keyDownEvent
					eventToAdd->data.ctlEnter.controlID = TableOverviewButton;
					EvtAddUniqueEventToQueue(eventToAdd, 0, true);
				}
			   handled = true;
			}



			//	For 5 Way Nav Down Change Table/Seat
//			else if(event->data.keyDown.chr == pageDownChr)
			else if(NavDirectionPressed(event, Down)	||
						(event->data.keyDown.chr == vchrPageDown)	)
	   	{
			   handled = true;
			}





/*				if(formID == TableForm)
				{
					if(Prefs.current.tableNo != OldCurrentTable)
					{
						Prefs.current.seatNo = 1;
					}


		   		if(PreviousForm == OverviewForm)
						OverviewPageType = OverviewTakeOrders;
		   			
		   		//	Avoid getting stuck in Table/Seat ping pong if Palm goes to sleep in the Seats form
		   		if(PreviousForm == SeatForm)
		   			PreviousForm = OverviewForm;
		   		
					if(Prefs.userSettings.classicMode)
			   		PreviousForm = MainForm;

		   		FrmGotoForm(PreviousForm);
		   		PreviousForm = TableForm;
				}
*/


/*			//	For 5 Way Nav Up or Left Change Table/Seat
			else if(	event->data.keyDown.chr == pageDownChr		||
						event->data.keyDown.chr == vchrRockerLeft	||
						(EvtKeydownIsVirtual(event) && 
						(event->data.keyDown.chr == vchrNavChange) && 
						((event->data.keyDown.keyCode & (navBitsAll | navChangeBitsAll))
																						== navChangeLeft)))
	   	{
				if(formID == TableForm)
				{
					if(Prefs.current.tableNo > 1)
					{
						TableNoTemp = NoValue16;
						OverviewTopRowNum = 0;					//	Always start at Row 0
						Prefs.current.seatNo = 1;

						Prefs.current.tableNo--;
						MmItemToString(Prefs.current.tableNo, numberStrTable, DontCheckList, &ListItems[0]);
						MmSetTableLabels(frm, numberStrTable);
					}
				}
				else
				{
					if(Prefs.current.seatNo > 1)
					{
						Prefs.current.seatNo--;
						MmItemToString(Prefs.current.seatNo, numberStrSeat, DontCheckList, &ListItems[0]);
						MmSetSeatLabels(frm, numberStrSeat);
					}
				}
			   // Repeating controls don't repeat if handled is set true.
			   handled = true;
			}
*/


/*				if(formID == TableForm)
				{
					if(Prefs.current.tableNo < LastTable)
					{
						TableNoTemp = NoValue16;
						OverviewTopRowNum = 0;					//	Always start at Row 0
						Prefs.current.seatNo = 1;

						Prefs.current.tableNo++;
						MmItemToString(Prefs.current.tableNo, numberStrTable, DontCheckList, &ListItems[0]);
						MmSetTableLabels(frm, numberStrTable);
					}
				}
				else
				{
					if(Prefs.current.seatNo < LastSeat)
					{
						Prefs.current.seatNo++;
						MmItemToString(Prefs.current.seatNo, numberStrSeat, DontCheckList, &ListItems[0]);
						MmSetSeatLabels(frm, numberStrSeat);
					}
				}
*/


/*	   	else if(	event->data.keyDown.chr == pageUpChr ||
	   				event->data.keyDown.chr == pageDownChr	)
	   	{
	  			// If this is the Table (not Seat) Form.
				if(FrmGetActiveFormID() == TableForm)
				{
					TableNoTemp = NoValue16;
					OverviewTopRowNum = 0;					//	Always start at Row 0
					Prefs.current.seatNo = 1;

		   		if(event->data.keyDown.chr == pageUpChr && Prefs.current.tableNo < LastTable)
		   		{
						Prefs.current.tableNo++;
					}
		   		if(event->data.keyDown.chr == pageDownChr	&& Prefs.current.tableNo > 1)
		   		{
						Prefs.current.tableNo--;
					}
					MmItemToString(Prefs.current.tableNo, numberStrTable, DontCheckList, &ListItems[0]);
					MmSetTableLabels(frm, numberStrTable);
				}
				else	// == SeatForm
				{
					SeatNoTemp = NoValue16;
					OverviewTopRowNum = 0;					//	Always start at Row 0

		   		if(event->data.keyDown.chr == pageUpChr && Prefs.current.seatNo < LastSeat)
		   		{
						Prefs.current.seatNo++;
					}
		   		if(event->data.keyDown.chr == pageDownChr	&& Prefs.current.seatNo > 1)
		   		{
						Prefs.current.seatNo--;
					}
					MmItemToString(Prefs.current.seatNo, numberStrSeat, DontCheckList, &ListItems[0]);
					MmSetSeatLabels(frm, numberStrSeat);
				}
*/



/*


	   	if(event->data.keyDown.chr == hard2Chr	||
	   		event->data.keyDown.chr == findChr		)
	   	{
				//	Bail out straight away if we're not logged in
				if(Prefs.loggedIn													||
					Prefs.hLStatus.HLLoginStatus == HLStatusToSend		||
					Prefs.hLStatus.HLInfoSyncStatus == HLStatusToSend	||
					Prefs.palmUniqueID == 0												)
				{
					FrmGotoForm(BeamingForm);
				}
				else
				{
 					FrmAlert(WarnLoginNotLoggedInAlert);
				}
				handled = true;
			}
*/


			   // Repeating controls don't repeat if handled is set true.
//			   handled = true;
			break;



		case frmTitleEnterEvent:

			if(FrmGetActiveFormID() == SeatForm)
			{
				if(MmBeamOrdersWithOptionalCheckForUnsent(formID, &Prefs, true, &UnsentOrdersCheckSecs))
				{
					FrmGotoForm(TableForm);
				}
				handled = true;
			}
			break;



		case menuEvent:		// A menu item was selected.
			
			MmHandleMenuEvent(frm, event);

			handled = true;
			break;


		case frmOpenEvent:

			FrmDrawForm(frm);

  			// If this is the Table (not Seat) Form.
			if(FrmGetActiveFormID() == TableForm)
			{
		   	TableNoTemp = NoValue16;
		   	GuestsTemp = NoValue16;

				FrmSetControlGroupSelection(frm, TableGroupID, TableTableNoPushButton);

				// Display the table number
				MmItemToString(Prefs.current.tableNo, numberStrTable, DontCheckList, &ListItems[0]);
				MmSetTableLabels(frm, numberStrTable);

				MmDisplayStaffNameLabel(frm, MmInfoDB, Prefs.staffIDIndex, TableStaffPopTrigger, 
													OverviewPageType, Prefs.loggedIn);
				//	Draw shadow box frame around table keypad
				MmDrawRectangle(5, 67, 20, 88, 136);

				//	Draw shadow box frame around MenuMate label
				MmDrawRectangle(5, 2, 20, 59, 34);

				if(InHappyHour)
					FrmShowObject(frm, FrmGetObjectIndex(frm, TableHappyHourButton));
				else
					FrmHideObject(frm, FrmGetObjectIndex(frm, TableHappyHourButton));

				if(PreviousForm == TableForm)
					PreviousForm = OverviewForm;
			}
			
			
			else	// If this is the Seat Form.
			{
		   	SeatNoTemp = NoValue16;
		   	
				FrmSetControlGroupSelection(frm, SeatGroupID, SeatSeatNoPushButton);

				// Display the seat number
				MmItemToString(Prefs.current.seatNo, numberStrSeat, DontCheckList, &ListItems[0]);
				MmSetSeatLabels(frm, numberStrSeat);

				//	Draw shadow box frame around seat keypad
				MmDrawRectangle(5, 67, 42, 88, 111);

				FrmHideObject(frm, FrmGetObjectIndex(frm, SeatOverviewButton));
				FrmHideObject(frm, FrmGetObjectIndex(frm, SeatToggleviewBitMap));
			}


//			if(Prefs.btNumDevices == 0)
			if(ReDiscoverBt)
			{
				MmDiscoverBtDevices(frm, false, &BtLibRefNum, &BtPortId, &Prefs, &ReDiscoverBt);
				ReDiscoverBt = false;
			}

			
			OldCurrentTable = Prefs.current.tableNo;


			handled = true;
			break;
	}



	return(handled);
}


/***********************************************************************
 *
 * FUNCTION:    MmTblSetItemInt
 *
 * DESCRIPTION: Simply fill the array (of arrays) with table entries
 *
 * PARAMETERS:  UInt16 row, UInt16 column, UInt16 value
 *
 * RETURNED:    
 *
 ***********************************************************************/
/*void MmTblSetItemInt(UInt16 row, UInt16 column, UInt16 value)
{
	TableDisplayRows[row].column[column] = value;
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
 *
 * PARAMETERS:  frm - form containing list to load
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
/*static Int16 ListItemsCompare(UInt8* item1, UInt8* item2)
{
	return StrCompare(item1, item2);
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
 *
 * PARAMETERS:  frm - form containing list to load
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
/*static UInt16 MmLoadBillingList (ListPtr listPtr)
{
	UInt16					items;					//	Counter to add stuff to the list
	UInt16					i;
	MemHandle				recHandle;
	OrderTypePtr		recPtr;
	UInt16					recIndex;				//	Index through the database
	UInt16					numRecords;
	UInt16					groupNumbers[MaxGroupItems];
	UInt8					thisGroup;

	
	listPtr->font = Prefs.current.fFontID;

	// For each item in the list, store the record number in the list item that will
	//	display the record. Get the next record in the current category (ie Course).
	numRecords = DmNumRecords(MmOrdersDB);

//***???	Do MmFindSortTableSeat() and Quit out at end of this Table !!!

	recIndex=0;
	items = 0;

	for(i=0; i<MaxGroupItems; i++)
	{
		groupNumbers[i] = 0;
	}

	while(items < MaxListItems && recIndex < numRecords)
	{		
		recHandle = DmQueryNextInCategory (MmOrdersDB, &recIndex, MarkerItem);
		// If record is found, store the pointer to the item string in the list array.
		if (recHandle)
		{
			recPtr = MemHandleLock(recHandle);

			//	If the item has been assigned a Billing Group Number, add it to the list
			thisGroup = recPtr->billing & ~BillingSeatMarked;
			if(thisGroup != 0)
			{
				//	Strip out any multiple entries of a group number
				for(i=0; i < items; i++)
				{
					if(thisGroup == groupNumbers[i])
						break;
				}
				if(thisGroup != groupNumbers[i])
					groupNumbers[items++] = thisGroup;	//	Add it to the list
			}
			MemPtrUnlock(recPtr);
		}
		recIndex++;
	}

	for(i=0; i<items; i++)
	{
		StrIToA(BillingGroupItems[i], groupNumbers[i]);
	}

	SysQSort(BillingGroupItems, items, sizeof(BillingGroupItems[0]),
																		(CmpFuncPtr)ListItemsCompare, 0);
	for(i=0; i<items; i++)
	{
		ListItems[i+1] = BillingGroupItems[i];
	}

	items++;
	ListItems[0] = "Table";

	return items;
}
*/

/***********************************************************************
 *
 * FUNCTION:    MmPopupBillingList
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
/*static UInt16 MmPopupBillingList(ListPtr listPtr)
{
//	MemHandle				recHandle;
	UInt16					recIndex = 0;
//	InfoTypePtr				recInfoPtr;
//	UInt16					numRecords;

	UInt16					items = 0;
	UInt16					itemSelected;


	//	Load the list pointer array with the string pointers
//	MmCourseOptionsTrimList(listPtr);		//	Trim Options to fit list width
	items = MmLoadBillingList(listPtr);

	LstSetListChoices(listPtr, ListItems, items);
	
	listPtr->font = Prefs.current.fFontID;			//	Set font for this list
	//	Display the possible items list
	if(Prefs.current.fFontID == stdFont)
	{
		if(items > DishListSizeStdFont -1)
			items = DishListSizeStdFont -1;	//	Don't let list get too large
	}
	else
	{
		if(items > DishListSizeLargeFont -1)
			items = DishListSizeLargeFont -1;//	Don't let list get too large
	}
	LstSetHeight(listPtr, items);				//	Set the vertical size of the list
	LstSetPosition(listPtr, 40, 20-items*2);//	Position above centre
	LstSetSelection(listPtr, NoValue16);//	Ensure no items are selected

	//	Then get the option selected
	itemSelected = LstPopupList(listPtr);
	


	if(StrCaselessCompare((const char *)ListItems[0], "None Available") == 0)
		itemSelected = NoValue16;

	return itemSelected;
}
*/

/***********************************************************************
 *
 * FUNCTION:		MmSplitBillDrawRecordInBounds
 *
 * DESCRIPTION:	Draw an item within a passed bounds
 *
 * PARAMETERS:		
 *
 * RETURNED:		Nothing.
 *
 ***********************************************************************/
/*static void MmSplitBillDrawRecordInBounds(RectanglePtr bounds, UInt8 groupNumber)
{
	char						bulletChar[] = "\x95";
	char						tempStr[4];
//	RectangleType			space = {0,0,16,16};
//	enum symbolChars		memoChar = symbolMemo;

	//	Set the Rectangle area to be cleared before the new bullet is drawn.
//	RctSetRectangle(&space, bounds->topLeft.x, bounds->topLeft.y, 16, 16);
	
	//	If this is the seat name/number (not a dish) then always show the checkbox
	if(Prefs.current.fFontID == stdFont)
		FntSetFont(boldFont);
	else
		FntSetFont(largeBoldFont);

	if(groupNumber)
	{
		StrIToA(tempStr, groupNumber);
		if(StrLen(tempStr) < 2)
		{
			tempStr[2] = 0;
			tempStr[1] = tempStr[0];
			tempStr[0] = ' ';
		}

		//	Display the Group Number
		WinDrawChars(tempStr, StrLen(tempStr), bounds->topLeft.x, bounds->topLeft.y);
	}
	

	FntSetFont(Prefs.current.fFontID);
}
*/

/***********************************************************************
 *
 * FUNCTION:		MmSplitBillDrawRecord
 *
 * DESCRIPTION:	Draw an item in the Orders form's table.  This 
 *						routine is called from the table object and must match
 *						the parameters that the table object passes.  The routine
 *						OrdersViewLoadRecords sets the table object to call this
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
/*static void MmSplitBillDrawRecord(MemPtr tableP, UInt16 row, UInt16 column, 
																			RectanglePtr bounds)
{
	UInt16					groupNumber;

	column = 0;							//	Stop compiler warning
	// Get the record number that corresponds to the table item to draw.
	// The record number is stored in the "intValue" field of the item.
	groupNumber = TblGetItemInt(tableP, row, Column0);

	MmSplitBillDrawRecordInBounds(bounds, groupNumber);
}
*/

/***********************************************************************
 *
 * FUNCTION:    MmSplitBillLoadTable
 *
 * DESCRIPTION: Loads the database records into the Split Bill table
 *
 * PARAMETERS:  frm - form containing table to load
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
/*static void MmSplitBillLoadTable(FormPtr frm)
{
	Int16				row = 0;
	Int16				numRows;
	TablePtr 		tablePtr;

//	UInt16				itemCounter = 0;
	
	UInt16				rowHeight;
	
	Boolean			splitBillCanScrollUp;
	
	
	tablePtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, SplitBillGroupsTable));

	//	Configure table dependant upon current font size
	if(Prefs.current.fFontID == stdFont)
	{
		rowHeight = 126 / MainTableSizeStandardFont;	//	126 is main table height in pixels
		tablePtr->numRows = MainTableSizeStandardFont+2;
	}
	else
	{
		rowHeight = 126 / MainTableSizeLargeFont;
		tablePtr->numRows = MainTableSizeLargeFont+1;
	}
	numRows = TblGetNumberOfRows(tablePtr);

	for(row=0; row < numRows; row++)
	{
		TblSetRowHeight(tablePtr, row, rowHeight);
		tablePtr->items->fontID = Prefs.current.fFontID;
	}

	row = 0;
	splitBillCanScrollUp = true;

	while(row < numRows && GroupsSelected[row] != 0)
	{
		//	Start displaying from the index stored for the start of this page
		TblSetItemStyle(tablePtr, row, Column0, customTableItem);

		TblSetItemInt(tablePtr, row, Column0, GroupsSelected[row]);

		TblSetRowUsable(tablePtr, row, true);
//		itemCounter++;
		row++;
	}

	while(row < numRows)
	{
		TblSetRowUsable(tablePtr, row, false);
		row++;
		splitBillCanScrollUp = false;
	}

//	if(itemCounter < numRows)
//		OverviewScrollPages[OverviewPageNum+1].currentIndex =
//				OverviewScrollPages[OverviewPageNum+1].markerIndex;	//	Use same as before
//	else
//		OverviewScrollPages[OverviewPageNum+1].currentIndex = recOrderIndex - 1;



	CtlHideControl(MmGetObjectPtr(SplitBillScrollUpRepeating));
	CtlHideControl(MmGetObjectPtr(SplitBillScrollDownRepeating));

	//	More items are available lower down the table
	if(OverviewPageNum)
	{
		CtlShowControl(MmGetObjectPtr(SplitBillScrollUpRepeating));
	}
	else
	{
		CtlHideControl(MmGetObjectPtr(SplitBillScrollUpRepeating));
	}

	//	More items are available higher up the table
	if(splitBillCanScrollUp)
	{
		CtlShowControl(MmGetObjectPtr(SplitBillScrollDownRepeating));
		OverviewCanScrollDown = true;
	}
	else
	{
		CtlHideControl(MmGetObjectPtr(SplitBillScrollDownRepeating));
		OverviewCanScrollDown = false;
	}


	TblMarkTableInvalid(tablePtr);
	TblRedrawTable(tablePtr);

}

*/
/***********************************************************************
 *
 * FUNCTION:		MmSplitBillHandleEvent
 *
 * DESCRIPTION:	Handles processing of events for the Orders form.
 *
 * PARAMETERS:		event	- the most recent event.
 *
 * RETURNED:		True if the event is handled, false otherwise.
 *
 ***********************************************************************/
/*Boolean MmSplitBillHandleEvent(EventPtr event)
{
	FormPtr				frm;
	TablePtr				tablePtr;
	ListPtr				listPtr;
	
//	UInt16					recIndex;
//	MemHandle				recHandle;
//	OrderTypePtr		recPtr;

//	UInt16					controlID;
//	ControlType *			buttonPtr;
	UInt16					i;
	
//	UInt8					thisBillingAction;
	UInt16					groupSelected;

	Boolean				handled = false;


	
	frm = FrmGetActiveForm();

	switch (event->eType)
	{
   	case ctlSelectEvent:  // A control button was pressed and released.
			
	   	// If the done button is pressed, go back to the main form.
	   	if(event->data.ctlEnter.controlID == SplitBillOkayButton)
	   	{

				// Remove the Table form and display the main form.
	   		FrmGotoForm(OverviewForm);
			}

	   	// If the done button is pressed, go back to the main form.
	   	if(event->data.ctlEnter.controlID == SplitBillSplitWithButton)
	   	{
				listPtr = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, SplitBillGroupItemsList));
				groupSelected = MmPopupBillingList(listPtr);
				if(groupSelected != NoValue16)
				{
					GroupsSelected[GroupsSelectedIndex++] = groupSelected;
				}

				MmSplitBillLoadTable(frm);
			}


		case ctlRepeatEvent:		// An onscreen scroll button is pressed
	   	if (event->data.ctlRepeat.controlID == SplitBillScrollDownRepeating)
		   {
				if(OverviewPageNum < MaxOverviewScrollPages)
					OverviewPageNum++;
				MmOverviewLoadTable(frm);

			   // Repeating controls don't repeat if handled is set true.
			   handled = true;
		   }

		   else if (event->data.ctlRepeat.controlID == SplitBillScrollUpRepeating)
		   {
				if(OverviewPageNum > 0)
					OverviewPageNum--;

				MmOverviewLoadTable(frm);

			   // Repeating controls don't repeat if handled is set true.
			   handled = true;
		   }
			break;


		case keyDownEvent:		// MemHandle the physical buttons
	   	if (event->data.keyDown.chr == pageUpChr)
	   	{
				if(OverviewPageNum > 0)
					OverviewPageNum--;

				MmOverviewLoadTable(frm);

			   // Repeating controls don't repeat if handled is set true.
			   handled = true;
	   	}

	   	else if (event->data.keyDown.chr == pageDownChr)
	   	{
				if(OverviewPageNum < MaxOverviewScrollPages && OverviewCanScrollDown)
					OverviewPageNum++;
				MmOverviewLoadTable(frm);

			   // Repeating controls don't repeat if handled is set true.
			   handled = true;
	   	}
			break;


		case tblSelectEvent:	// An item has been selected in the table.

			tablePtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, SplitBillGroupsTable));

			handled = true;
			break;


 		case frmOpenEvent:

			// Draw the form.
			FrmDrawForm(frm);
			
			for(i=0; i < MaxGroupItems; i++)
			{
				GroupsSelected[i] = 0;
			}
			GroupsSelectedIndex = 0;

			// Get the table and it's number of rows
			tablePtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, SplitBillGroupsTable));
//			rowsInTable = TblGetNumberOfRows (tablePtr);
		
			TblSetCustomDrawProcedure (tablePtr, Column0, MmSplitBillDrawRecord);
			
			TblSetColumnUsable (tablePtr, Column0, true);

			// Load table right from first (0th) record
			
			MmSplitBillLoadTable(frm);

			handled = true;
			break;
	}
	
	return(handled);
}
*/

/***********************************************************************
 *
 * FUNCTION:    MmMainviewLoadTable
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
void MmMainviewLoadTable(FormPtr frm)
{
	UInt16				displayRow;
	UInt16				numRows;
	UInt16				column;
	UInt16				numColumns;
	TablePtr 			tablePtr;
	UInt16				arrayRow;
//	UInt16				i;
	UInt16				rowHeight;
	
	
	
	tablePtr = FrmGetObjectPtr(frm, FrmGetObjectIndex (frm, MainOrderTable));

	//	Configure main table dependant upon current font size
	if(Prefs.current.fFontID == stdFont)
	{
		rowHeight = 126 / MainTableSizeStandardFont;	//	126 is main table height in pixels
		numRows = MainTableSizeStandardFont;
	}
	else
	{
		rowHeight = 126 / MainTableSizeLargeFont;
		numRows = MainTableSizeLargeFont;
	}
	numColumns = TblGetNumberOfColumns(tablePtr);

	for(displayRow=0; displayRow < numRows; displayRow++)
	{
		for(column=0; column < numColumns; column++)
		{
			TblSetRowHeight(tablePtr, displayRow, rowHeight);
			TblSetItemFont(tablePtr, displayRow, column, Prefs.current.fFontID);
		}
	}



	// Count the number of items yet to display
	//	If there's nothing to display on this page, pop back up to the previous page
	for(displayRow=0, arrayRow = MainviewTopRowNum;
				displayRow < numRows && TableDisplayRows[arrayRow].seatNumber != NoValue16;
						displayRow++, arrayRow++)
	{
	}
	if(displayRow == 0)
	{
  		MainviewTopRowNum -= numRows;
  		if(MainviewTopRowNum < 0)
	  		MainviewTopRowNum = 0;
  	}


	arrayRow = MainviewTopRowNum;


	// For each row in the table, store the record number and pointer in the
	// table item that will display the record.  


	// For each row and column in the table, store the data into the table item
	for(displayRow=0;	displayRow < numRows &&
							TableDisplayRows[arrayRow].seatNumber != NoValue16;
																				displayRow++, arrayRow++)
	{
		TblSetItemStyle(tablePtr, displayRow, Column0, customTableItem);
			
/*		for(i=0; i<=Column2; i++)
		{
			TblSetItemInt(tablePtr, displayRow, i, TableDisplayRows[arrayRow].column[i]);
		}
*/
		TblSetRowUsable(tablePtr, displayRow, true);
	}

	//	Display scroll arrow if appropriate
	if((displayRow < numRows)	||
		((displayRow == numRows)	&&	TableDisplayRows[arrayRow].seatNumber == NoValue16))
		MainviewCanScrollDown = false;
	else
		MainviewCanScrollDown = true;


	//	Initialise any rows that haven't already been set up
	while(displayRow < numRows)
	{
		TblSetItemStyle(tablePtr, displayRow, Column0, customTableItem);
			
		TblSetItemInt(tablePtr, displayRow, Column0, 0x20);

		TblSetRowUsable(tablePtr, displayRow, false);
		displayRow++;
	}

	FrmHideObject(frm, FrmGetObjectIndex(frm, MainScrollUpRepeating));
	FrmHideObject(frm, FrmGetObjectIndex(frm, MainScrollDownRepeating));

	//	More items are available lower down the table
	if(MainviewTopRowNum)
	{
		FrmShowObject(frm, FrmGetObjectIndex(frm, MainScrollUpRepeating));
	}
	else
	{
		FrmHideObject(frm, FrmGetObjectIndex(frm, MainScrollUpRepeating));
	}

	//	More items are available higher up the table
	if(MainviewCanScrollDown)
	{
		FrmShowObject(frm, FrmGetObjectIndex(frm, MainScrollDownRepeating));
	}
	else
	{
		FrmHideObject(frm, FrmGetObjectIndex(frm, MainScrollDownRepeating));
	}

	TblMarkTableInvalid(tablePtr);
	TblRedrawTable(tablePtr);

	SidesIndexNum = 0;
}




/***********************************************************************
 *
 * FUNCTION:		MmOverviewDrawRecord
 *
 * DESCRIPTION:	Draw an item in the Orders form's table.  This 
 *						routine is called from the table object and must match
 *						the parameters that the table object passes.  The routine
 *						OrdersViewLoadRecords sets the table object to call this
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
static void MmOverviewDrawRecord(MemPtr tableP, Int16 tableRow, Int16 column, 
																			RectanglePtr bounds)
{
	tableP = NULL;				//	Stop compiler grizzle
	
	MmOverviewDrawRecord2(&DBs, OverviewTopRowNum, tableRow, column, bounds, 
									OverviewPageType, OverviewCheckboxType, OverviewExpandedStatus,
									&TableDisplayRows[0], &SidesIndexNum, ListItems, &Prefs);
}




/***********************************************************************
 *
 * FUNCTION:    MmOverviewRedraw
 *
 * DESCRIPTION: Reload the Overview table, redraw the screen
 *
 * PARAMETERS:  frm - form containing list to load
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
void MmOverviewRedraw(FormPtr frm, UInt16 scrollType, UInt16 redraw)
{
	TablePtr					tablePtr;
	ControlType				*buttonPtr;

	UInt16					recOrderIndex;
	Int16						overviewTopRowNum;
	UInt16					rowCount;
	Int16						formID;
	
	char						bulletExpandedChar[2]	= "";
	char						bulletUpChar[2]			= "";
	char						bulletDownChar[2]			= "";



	formID = FrmGetActiveFormID();
	
	tablePtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, OverviewOverviewTable));
	buttonPtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, OverviewSelectButton));

	
	SidesIndexNum = 0;					//	Count sides, for MmMainviewDrawRecord

	//	Show/Hide icons as necessary
	switch(OverviewPageType)
	{
		case OverviewPrintReceiptsPage:
		
//			if(MmBillingOperations(BillingActionFindFirstInTable, MmOrdersDB, &Prefs.current)
//																							 == NoValue16)
//			{
//				OverviewCheckboxType = OverviewCheckboxesNone;
//			}
//			else
//			{
				OverviewCheckboxType = OverviewCheckboxesBilling;
//			}
			FrmHideObject(frm, FrmGetObjectIndex(frm, OverviewUnsentBitMap));
//			FrmShowObject(frm, FrmGetObjectIndex(frm, OverviewPaidBitMap));
			break;

		case OverviewUnsentPage:
		
			OverviewCheckboxType = OverviewCheckboxesUnsent;
			FrmHideObject(frm, FrmGetObjectIndex(frm, OverviewPaidBitMap));
//			FrmShowObject(frm, FrmGetObjectIndex(frm, OverviewUnsentBitMap));
			break;

		default:
			OverviewCheckboxType = OverviewCheckboxesNone;
	}


	//	Set column widths as necessary
	switch(OverviewCheckboxType)
	{
		case OverviewCheckboxesNone:
		
			TblSetColumnWidth(tablePtr, Column1, OverviewCol1Width);
			break;

		case OverviewCheckboxesBilling:
		case OverviewCheckboxesUnsent:
		
			TblSetColumnWidth(tablePtr, Column1, OverviewCol1Width - OverviewBillingColWidth);
			break;

//		default:
	}



	//	Hide Buttons as necessary
	switch(OverviewCheckboxType)
	{
		case OverviewCheckboxesNone:
		
			FrmHideObject(frm, FrmGetObjectIndex(frm, OverviewSelectAllBitMap));
			FrmHideObject(frm, FrmGetObjectIndex(frm, OverviewDeselectAllBitMap));

			FrmHideObject(frm, FrmGetObjectIndex(frm, OverviewSelectAllButton));
			FrmHideObject(frm, FrmGetObjectIndex(frm, OverviewDeselectAllButton));

			CtlHideControl(buttonPtr);
			break;


//		default:
	}






//	FrmHideObject(frm, FrmGetObjectIndex(frm, OverviewTableBitMap));
//	FrmHideObject(frm, FrmGetObjectIndex(frm, OverviewSeatsBitMap));

//	FrmHideObject(frm, FrmGetObjectIndex(frm, OverviewPaidBitMap));
//	FrmHideObject(frm, FrmGetObjectIndex(frm, OverviewUnsentBitMap));
//	FrmShowObject(frm, FrmGetObjectIndex(frm, OverviewPaidBitMap));





	if(OverviewTopRowNum < 0)
		OverviewTopRowNum = 0;
	
	//	Preload this in case it isn't set during the MmOverviewLoadTableArrayLoop
	overviewTopRowNum = OverviewTopRowNum;




	if(redraw != NoRewriteTable)	//	Prevent screen flicker during re-write
	{
		//	Can be commanded to set OverviewExpandedStatus to Expanded, Contracted, or NoChange
		//	Leave it set as commanded until further notice.
		switch(redraw)
		{
			case DoRewriteExpanded:
				OverviewExpandedStatus = StatusIsExpanded;
				break;

			case DoRewriteContracted:
				OverviewExpandedStatus = StatusIsContracted;
				break;

	//		default:
	//			OverviewExpandedStatus = StatusNoChange;
		}




		if(OverviewPageType == OverviewTakeOrders)
		{
			if(OverviewExpandedStatus == StatusIsExpanded)
			{
				for(rowCount = 0; rowCount < MaxTableDisplayRows; rowCount++)
					TableDisplayRows[rowCount].setExpanded = true;
				redraw = DoRewriteArray;
			}
			else if(OverviewExpandedStatus == StatusIsContracted)
			{
				for(rowCount = 0; rowCount < MaxTableDisplayRows; rowCount++)
					TableDisplayRows[rowCount].setExpanded = false;
				redraw = DoRewriteArray;
			}
		}
	}


	if(redraw == DoRewriteArray)
	{
		//	Find start of current table
		recOrderIndex = MmFindSortTableSeat(Prefs.current.tableNo, 1);
	
		MmOverviewLoadTableArrayLoop(&DBs, &Prefs, OverviewPageType, OverviewServingCourse, 
												&TableDisplayRows[0], OverviewExpandedStatus, recOrderIndex,
													&overviewTopRowNum, &LastItemOrdered	);
		if(scrollType == ScrollTop)
			OverviewTopRowNum = 0;
		else if(scrollType == CanScroll)
			OverviewTopRowNum = overviewTopRowNum;	//	Display last of previous seat

		if(OverviewTopRowNum < 0)
			OverviewTopRowNum = 0;
	
		MmOverviewLoadTable(frm);
	}
	else	//	redraw == NoRewriteArray
	{
		if(redraw != NoRewriteTable)	//	Prevent screen flicker during re-write
		{
			MmOverviewLoadTable(frm);
		}
	}

	MmDrawOverviewLines(OverviewCheckboxType);
	MmOverviewDisplayTablenameInTitle(frm, &Prefs, OverviewPageType, MmTablenameDB);




	//	Show/Hide Buttons as necessary
	switch(OverviewCheckboxType)
	{
		case OverviewCheckboxesBilling:
		
			FrmShowObject(frm, FrmGetObjectIndex(frm, OverviewSelectAllBitMap));
			FrmShowObject(frm, FrmGetObjectIndex(frm, OverviewDeselectAllBitMap));

			FrmShowObject(frm, FrmGetObjectIndex(frm, OverviewSelectAllButton));
			FrmShowObject(frm, FrmGetObjectIndex(frm, OverviewDeselectAllButton));

			CtlHideControl(buttonPtr);
			break;

		case OverviewCheckboxesUnsent:
		
			FrmShowObject(frm, FrmGetObjectIndex(frm, OverviewSelectAllBitMap));
			FrmShowObject(frm, FrmGetObjectIndex(frm, OverviewDeselectAllBitMap));

			FrmShowObject(frm, FrmGetObjectIndex(frm, OverviewSelectAllButton));
			FrmShowObject(frm, FrmGetObjectIndex(frm, OverviewDeselectAllButton));

			CtlSetLabel(buttonPtr, " Delete");
			CtlShowControl(buttonPtr);
			break;

//		default:
	}





	//	Show/Hide the Login as necessary
	switch(OverviewPageType)
	{
		case OverviewTakeOrders:
		case OverviewOrdersOnlyPage:
		case OverviewServingCourses:
		
			CtlHideControl(buttonPtr);

			MmDisplayStaffNameLabel(frm, MmInfoDB, Prefs.staffIDIndex, OverviewStaffPopTrigger, 
												OverviewPageType, Prefs.loggedIn);
			break;

//		default:
	}




	//	Show or hide the Up/Down scroll arrowhead buttons

	//	More items are available lower down the table
	if(OverviewTopRowNum)
		bulletUpChar[0] = symbol7ScrollUp;
	else
		bulletUpChar[0] = symbol7ScrollUpDisabled;

	buttonPtr = FrmGetObjectPtr(frm, FrmGetObjectIndex (frm, OverviewScrollUpRepeating));
	CtlSetLabel(buttonPtr, bulletUpChar);





	//	More items are available higher up the table
	if(OverviewCanScrollDown)
		bulletDownChar[0] = symbol7ScrollDown;
	else
		bulletDownChar[0] = symbol7ScrollDownDisabled;

	buttonPtr = FrmGetObjectPtr(frm, FrmGetObjectIndex (frm, OverviewScrollDownRepeating));
	CtlSetLabel(buttonPtr, bulletDownChar);





	if(InHappyHour)
		FrmShowObject(frm, FrmGetObjectIndex(frm, OverviewHappyHourButton));
	else
		FrmHideObject(frm, FrmGetObjectIndex(frm, OverviewHappyHourButton));

//	PreviousForm = OverviewForm;
}



/***********************************************************************
 *
 * FUNCTION:		OverviewInit
 *
 * DESCRIPTION:	P7. Initializes the Overview form and the list object.
 *
 * PARAMETERS:		frm - pointer to the Overview form.
 *
 * RETURNED:		Nothing.
 *
 ***********************************************************************/
void MmOverviewInit(void)
{
	FormPtr					frm;
	TablePtr 				tablePtr;
	UInt16					displayRow;
	UInt16					column;
	UInt16					numRows;
	UInt16					numCols;


	// Get a pointer to the main form.
	frm = FrmGetActiveForm();
	
	FrmDrawForm(frm);
	
	// Get the table and it's number of rows
	tablePtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, OverviewOverviewTable));
	
	numRows = TblGetNumberOfRows(tablePtr);
	numCols = TblGetNumberOfColumns(tablePtr);
	
	//	Temporarily initialise Column0 for all the rows
	for(displayRow = 0; displayRow < numRows; displayRow++)
	{
		for(column = 0; column <= numCols; column++)
		{
			TblSetItemStyle(tablePtr, displayRow, column, customTableItem);
		}
		TblSetRowUsable(tablePtr, displayRow, false);
	}

	// Set the callback routine that will draw the records.
	TblSetCustomDrawProcedure(tablePtr, Column0, MmOverviewDrawRecord);
	TblSetCustomDrawProcedure(tablePtr, Column1, MmOverviewDrawRecord);
//	TblSetCustomDrawProcedure(tablePtr, BillingColumn, MmOverviewDrawRecord);


	// Set the column usable so that it draws and accepts user input.
	TblSetColumnUsable(tablePtr, Column0, true);
	TblSetColumnUsable(tablePtr, Column1, true);
//	TblSetColumnUsable(tablePtr, Column3, true);
//	TblSetColumnUsable(tablePtr, Column4, true);

//	MmOverviewRedraw(frm, CanScroll, DoRewriteArray);
}


/***********************************************************************
 *
 * FUNCTION:    MmOrdersviewLoadTable
 *
 * DESCRIPTION: Loads the database records into the Overview table
 *
 * PARAMETERS:  frm - form containing table to load
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
static void MmOverviewLoadTable(FormPtr frm)
{
	UInt16						displayRow;
	UInt16						numRows;
	UInt16						column;
	UInt16						numColumns;
	UInt16						arrayRow;
	TablePtr 					tablePtr;
//	UInt16						i;
	UInt16						rowHeight;
	

	tablePtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, OverviewOverviewTable));


	//	Configure main table dependant upon current font size
	if(Prefs.current.fFontID == stdFont)
	{
		rowHeight = 126 / MainTableSizeStandardFont;	//	126 is main table height in pixels
		numRows = MainTableSizeStandardFont+2;
	}
	else
	{
		rowHeight = 126 / MainTableSizeLargeFont;
		numRows = MainTableSizeLargeFont+1;
	}

	arrayRow = OverviewTopRowNum;			//	Start display at start of seat

	numColumns = TblGetNumberOfColumns(tablePtr);

	for(displayRow=0; displayRow < numRows; displayRow++)
	{
		for(column=0; column < numColumns; column++)
		{
			TblSetRowHeight(tablePtr, displayRow, rowHeight);
			TblSetItemFont(tablePtr, displayRow, column, Prefs.current.fFontID);
		}
	}



	// For each row that has valid data in the table, set it usable
	for(displayRow=0;		displayRow < numRows &&
								TableDisplayRows[arrayRow].seatNumber != NoValue16;
																				displayRow++, arrayRow++)
	{
		TblSetRowUsable(tablePtr, displayRow, true);
	}

	//	Disable the rest of the table
	while(displayRow < numRows)
	{
		TblSetRowUsable(tablePtr, displayRow, false);
		displayRow++;
	}




	TblMarkTableInvalid(tablePtr);
	TblRedrawTable(tablePtr);




	//	Work out what scrolling is allowed
	if((displayRow < numRows)	||
		((displayRow == numRows)	&&	TableDisplayRows[arrayRow].seatNumber == NoValue16))
		OverviewCanScrollDown = false;
	else
		OverviewCanScrollDown = true;

	SidesIndexNum = 0;
}



/***********************************************************************
 *
 * FUNCTION:    MmOverviewScrollUp
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
static void MmOverviewScrollUp(FormPtr frm)
{
	if(OverviewTopRowNum <= 0)
	{
		OverviewTopRowNum = 0;
		MmOverviewRedraw(frm, NoScroll, NoRewriteTable);
	}
	else
	{
   	OverviewTopRowNum --;

		//	Keep Prefs.current.seatNo pointing to current seat in case we go back to Main form
		Prefs.current.tableNo = TableDisplayRows[OverviewTopRowNum].tableNumber;
		Prefs.current.seatNo = TableDisplayRows[OverviewTopRowNum].seatNumber;

		MmOverviewRedraw(frm, CanScroll, NoRewriteArray);
   }
}



/***********************************************************************
 *
 * FUNCTION:    MmOverviewScrollDown
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
static void MmOverviewScrollDown(FormPtr frm)
{
	Boolean		doRedraw = false;

	if(OverviewCanScrollDown)
	{
	   doRedraw = true;
	   OverviewTopRowNum++;
	}

	//	Keep Prefs.current.seatNo pointing to current seat in case we go back to Main form
	Prefs.current.tableNo = TableDisplayRows[OverviewTopRowNum].tableNumber;
	Prefs.current.seatNo = TableDisplayRows[OverviewTopRowNum].seatNumber;


	if(doRedraw)
		MmOverviewRedraw(frm, CanScroll, NoRewriteArray);
	else
		MmOverviewRedraw(frm, NoScroll, NoRewriteTable);
}



/***********************************************************************
 *
 * FUNCTION:		MmOverviewHandleEvent
 *
 * DESCRIPTION:	Handles processing of events for the Orders form.
 *
 * PARAMETERS:		event	- the most recent event.
 *
 * RETURNED:		True if the event is handled, false otherwise.
 *
 ***********************************************************************/
Boolean MmOverviewHandleEvent(EventPtr event)
{
	FormPtr							frm;
	Int16								formId;
	Boolean							handled = false;
	UInt32							timeNow;

	TablePtr							tablePtr;
	ListPtr							listPtr;
	
	UInt16							recIndex;
	MemHandle						recHandle;
	OrderTypePtr					recPtr;
	
	UInt16							items;
	UInt16							itemSelected;
	UInt16							offset = 0;
	
	UInt16							row;



//	!!!!!!! Warning: These MUST be the same as the #defines in MenuMate.h
/*
#define OverviewTables					0
#define OverviewPrintReceiptsPage	1
#define OverviewBeam						2
#define OverviewTakeOrders				3
#define OverviewServingCourses		4
#define OverviewUnsentPage				5
#define OverviewOrdersOnlyPage		6
#define OverviewClassicView			7			//	Go to Classic 'Courses' screen

#define MaxOverviewDisplayViews		8
*/
	Char		displayName[][MaxInfoNameSize] = {	"Tables",
																"Print Receipts", 
																"Beam", 
																"Take Orders", 
																"By Serving Course", 
																"Unsent, All Tables", 
																"View Orders Only", 
																"Classic View"
															};


	frm = FrmGetActiveForm();
	formId = FrmGetFormId(frm);

	timeNow = MmProcessEventIncludingNilEvent(frm, event, 0, OverviewStaffPopTrigger);


	tablePtr = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, OverviewOverviewTable));


	switch (event->eType)
	{
   	case ctlSelectEvent:  // A control button was pressed and released.

	   	// If the OverviewSelectViewButton button is pressed, offer view list to choose from.
	   	if(event->data.ctlEnter.controlID == OverviewSelectViewButton)
	   	{
				//	Now 'point' the ListItems to the start of each of the Display Views
				//	followed by each of the Serving Course names
				for(items=0; items < MaxOverviewDisplayViews; items++)
					ListItems[items] = displayName[items];

				listPtr = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, OverviewDishesList));
				
				offset = items;
				
				
				
				if(Prefs.userSettings.classicMode)
				{
					//	Preselect the OverviewClassicView for single handed navigator use
					itemSelected = MmPopupList(listPtr, DisplayViewList, &offset, OverviewClassicView);

					//	Preset to avoid getting stuck in 
					PreviousForm = OverviewForm;
				}
				else
				{
					//	Preselect the OverviewBeam for single handed navigator use
					itemSelected = MmPopupList(listPtr, DisplayViewList, &offset, OverviewBeam);
				}
				
				if(itemSelected != NoValue16)
				{
					OverviewExpandedStatus = StatusIsContracted;

					switch(itemSelected)
					{
						case OverviewTables:

							if(MmBeamOrdersWithOptionalCheckForUnsent(formId, &Prefs, true, &UnsentOrdersCheckSecs))
							{
								PreviousForm = OverviewForm;
								FrmGotoForm(TableForm);

								MainviewTopRowNum = 0;							//	Scrolling offset for main table
								OverviewTopRowNum = 0;							//	Always start at row 0
							}
							
							break;


						case OverviewPrintReceiptsPage:
							OverviewPageType = OverviewPrintReceiptsPage;//	Show only active seats
							OverviewCheckboxType = OverviewCheckboxesNone;
							MmOverviewRedraw(frm, ScrollTop, DoRewriteArray);
							break;


						case OverviewBeam:

							//	false -> Force Beaming (unless we're not logged in)
							if(MmBeamOrdersWithOptionalCheckForUnsent(formId, &Prefs, false, &UnsentOrdersCheckSecs))
							{
								OverviewPageType = OverviewTakeOrders;
								PreviousForm = OverviewForm;
								FrmGotoForm(TableForm);
							}
							
							handled = true;
							break;


						case OverviewTakeOrders:

							OverviewPageType = OverviewTakeOrders;			//	Show the orders for this Seat by SC
							MmOverviewRedraw(frm, ScrollTop, DoRewriteArray);
							break;


						case OverviewServingCourses:

							offset = 0;
							itemSelected = MmPopupList(listPtr, ServingCourseAllList, &offset, 0);
							
							if(itemSelected != NoValue16)
							{
								OverviewServingCourse = ListItemsStruct[itemSelected].recIndex;

								OverviewPageType = OverviewServingCourses;	//	Show the orders by individual SC
								MmOverviewRedraw(frm, ScrollTop, DoRewriteArray);
							}
							break;


						case OverviewUnsentPage:
							OverviewPageType = OverviewUnsentPage;			//	Show all Unsent orders
							OverviewCheckboxType = OverviewCheckboxesNone;
							MmOverviewRedraw(frm, ScrollTop, DoRewriteArray);
							break;


						case OverviewOrdersOnlyPage:
							OverviewPageType = OverviewOrdersOnlyPage;	//	Show the orders only for this Seat
							MmOverviewRedraw(frm, CanScroll, DoRewriteArray);
							break;


						case OverviewClassicView:
							OverviewPageType = OverviewOrdersOnlyPage;	//	Show Classic Courses View
							MainviewTopRowNum = 0;

				   		FrmGotoForm(MainForm);
							break;


						default:														//	Should never happen!
							
							break;
					}
				}
			}
			



	   	// If the Staff trigger is pressed, pop up the Staff list.
	   	else if(event->data.ctlEnter.controlID == OverviewStaffPopTrigger)
	   	{
				//	Check if any unsent orders exist. false -> check for OLD and NEW unsent orders
				if(//	OverviewPageType != OverviewUnsentPage
					//&&
						MmWarnIfUnsentOrders(MmOrdersDB, &Prefs, false, &NumOldUnsentOrders))
				{
					//	Log them out so they can't send orders through on previous account
					Prefs.loggedIn = false;

					OverviewPageType = OverviewUnsentPage;
					OverviewCheckboxType = OverviewCheckboxesUnsent;
					MmOverviewRedraw(frm, ScrollTop, DoRewriteArray);
				}
				else
				{
					listPtr = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, OverviewDishesList));

					//	Then get the Staff member selected
					itemSelected = MmPopupList(listPtr, StaffList, &offset, 0);

					//	Aborted Login
					if(itemSelected == NoValue16)
					{
						MmDisplayStaffNameLabel(frm, MmInfoDB, Prefs.staffIDIndex, OverviewStaffPopTrigger, 
															OverviewPageType, Prefs.loggedIn);
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
			   		PreviousForm = OverviewForm;
						FrmGotoForm(EnterPINForm);					//	Get their PIN number
					}
				}
				handled = true;
			}





			//	Happy Hour Icon
			else if(event->data.ctlEnter.controlID == OverviewHappyHourButton)
	  		{
				ShowMessage("Happy Hour pricing applies");
			}




			//	Overview 'Select' Button
	   	else if(event->data.ctlEnter.controlID == OverviewSelectButton)
	   	{
				OverviewExpandedStatus = StatusIsContracted;

/*				if(OverviewPageType == OverviewTakeOrders)
				{
					if(OverviewCheckboxType)
					{
						if(MmBillingOperations(BillingActionFindFirstInTable, MmOrdersDB, &Prefs.current)
																										 == NoValue16)
							ShowMessage("No Seats are Selected");
						else
							ShowMessage("Receipts will be Printed when you Beam");
						
//						OverviewCheckboxType = OverviewCheckboxesNone;
					}
					else
					{
						OverviewCheckboxType = OverviewCheckboxesBilling;
					}
				}

				else 
*/
				if(OverviewPageType == OverviewUnsentPage)
				{
					if(OverviewCheckboxType)
					{
						if(FrmAlert(ConfDeleteAllUnsentAlert) == ConfDeleteAllUnsentOK)
						{
							MmMarkOrDeleteUnsentOrders(MmOrdersDB, &Prefs, DeleteMarkedOrders);
							
//							NumOldUnsentOrders = NoValue16;			//	Force a recount
							
							if(Prefs.numUnsentOrders == 0)
							{
								OverviewCheckboxType = OverviewCheckboxesNone;
							}
						}
					}
					else
					{
						OverviewCheckboxType = OverviewCheckboxesUnsent;
					}
				}
				MmOverviewRedraw(frm, ScrollTop, DoRewriteArray);
			}




			else
			{
				//	Select All or Cancel All tapped for Billing Page
				if(OverviewCheckboxType == OverviewCheckboxesBilling)
		   	{
			   	// Select Entire Table Button
			   	if(event->data.ctlEnter.controlID == OverviewSelectAllButton)
			   	{
						MmBillingOperations(BillingActionSelectTable, MmOrdersDB, &Prefs.current);
		//				Prefs.hLStatus.HLBillSeatsStatus = HLStatusToSend;

						MmOverviewRedraw(frm, NoScroll, DoRewriteArray);
					}


			   	// Cancel All Billing Button
			   	else if(event->data.ctlEnter.controlID == OverviewDeselectAllButton)
			   	{
					  	//	Its the normal Table/Seat Overview page
						MmBillingOperations(BillingActionCancelTable, MmOrdersDB, &Prefs.current);

		//				Prefs.hLStatus.HLBillSeatsStatus = HLStatusAllDone;
						MmOverviewRedraw(frm, NoScroll, DoRewriteArray);
					}

/*
			   	// If the Paid button is pressed
			   	else if(event->data.ctlEnter.controlID == OverviewPaidButton)
			   	{
						//	Don't mess with the Latch, only the Current status
						if(Prefs.billingIsPaid & BillingIsPaidCurrent)
							Prefs.billingIsPaid &= ~BillingIsPaidCurrent;
						else
							Prefs.billingIsPaid |= BillingIsPaidCurrent;

						MmOverviewRedraw(frm, NoScroll, DoRewriteArray);
					}
*/				}




				//	Select All or Cancel All tapped for Unsent Page
				else if(OverviewCheckboxType == OverviewCheckboxesUnsent)
		   	{
			   	// Select Entire Table Button
			   	if(event->data.ctlEnter.controlID == OverviewSelectAllButton)
			   	{
						MmMarkOrDeleteUnsentOrders(MmOrdersDB, &Prefs, MarkUnsentOrders);

						MmOverviewRedraw(frm, NoScroll, DoRewriteArray);
					}


			   	// Cancel All Billing Button
			   	else if(event->data.ctlEnter.controlID == OverviewDeselectAllButton)
			   	{
						MmMarkOrDeleteUnsentOrders(MmOrdersDB, &Prefs, UnMarkUnsentOrders);

						MmOverviewRedraw(frm, NoScroll, DoRewriteArray);
					}
				}




			}

			handled = true;
			break;



   	case ctlExitEvent: 	 // A control button was pressed and released OUTSIDE of the bounds.
			
			if(event->data.ctlExit.controlID == OverviewStaffPopTrigger)
			{
				MmDisplayStaffNameLabel(frm, MmInfoDB, Prefs.staffIDIndex, OverviewStaffPopTrigger, 
															OverviewPageType, Prefs.loggedIn);
			}
			
			handled = true;
			break;

			


		case ctlRepeatEvent:		// An onscreen scroll button is pressed

		   if(event->data.ctlRepeat.controlID == OverviewScrollUpRepeating)
		   {
				MmOverviewScrollUp(frm);
		   }
	   	else if(event->data.ctlRepeat.controlID == OverviewScrollDownRepeating)
		   {
				MmOverviewScrollDown(frm);
		   }


//			handled = true;
			break;

/*
if(Prefs.current.seatNo < LastSeat)
{
	Prefs.current.seatNo++;
	MmOverviewRedraw(frm, ScrollTop, DoRewriteArray);
}
*/
/*
			else if(event->data.keyDown.chr == vchrRockerLeft
						||
						(EvtKeydownIsVirtual(event) && 
					(event->data.keyDown.chr == vchrNavChange) && 
					((event->data.keyDown.keyCode & (navBitsAll | navChangeBitsAll))
																						== navChangeLeft))
																													)
	   	{
				
				if(Prefs.current.seatNo > 1)
				{
					Prefs.current.seatNo--;
					MmOverviewRedraw(frm, ScrollTop, DoRewriteArray);
				}
			}




			else if(event->data.keyDown.chr == vchrRockerRight
						||
						(EvtKeydownIsVirtual(event) && 
					(event->data.keyDown.chr == vchrNavChange) && 
					((event->data.keyDown.keyCode & (navBitsAll | navChangeBitsAll))
																						== navChangeRight))
																													)
	   	{
				if(	!Prefs.userSettings.classicMode
					||
						(	Prefs.userSettings.classicMode
						 &&
						 	OverviewPageType == OverviewOrdersOnlyPage))
				{
					if(Prefs.current.seatNo < LastSeat)
					{
						Prefs.current.seatNo++;
						MmOverviewRedraw(frm, ScrollTop, DoRewriteArray);
					}
				}
			}
*/




/*
Boolean MyFormHandleEvent(EventType* evtP)
{
	if (	(evtP->eType == keyDownEvent) &&
			(EvtKeydownIsVirtual(evtP)) &&
			(evtP->data.keyDown.chr == vchrNavChange) &&
			((evtP->data.keyDown.keyCode & navChangeBitsAll) == navChangeSelect) &&
			((evtP->data.keyDown.keyCode & navBitsAll) == navBitSelect))
	{
		// The patch won’t get this event and long press is now disabled
		return true;
	}
	return false;
}

	   	if(event->data.keyDown.chr == vchrRockerCenter	||
				((EvtKeydownIsVirtual(event) &&
				(event->data.keyDown.chr == vchrNavChange) && 
				((event->data.keyDown.keyCode & (navBitsAll | navChangeBitsAll))== navChangeSelect)))	)

			else if(event->data.keyDown.chr == vchrRockerLeft	||
						(EvtKeydownIsVirtual(event) && 
					(event->data.keyDown.chr == vchrNavChange) && 
					((event->data.keyDown.keyCode & (navBitsAll | navChangeBitsAll))
																						== navChangeLeft)))



For press, hold, and release of the select key, application will see the following events:
	{vchrNavChange, navBitSelect | navChangeSelect, commandKeyMask}
	{vchrNavChange, navBitSelect, commandKeyMask | autoRepeatKeyMask}
	...
	{vchrNavChange, navChangeSelect, commandKeyMask}
For rotating the 5-way controller starting from left, application will see the following events:
	{vchrNavChange, navBitLeft | navChangeLeft, commandKeyMask}
	{vchrPageUp, navBitUp | navChangeLeft | navChangeUp, commandKeyMask | autoRepeatKeyMask}
	{vchrNavChange, navBitRight | navChangeUp | navChangeRight, commandKeyMask | autoRepeatKeyMask }
	{vchrPageDown, navBitDown | navChangeRight | navChangeDown, commandKeyMask | autoRepeatKeyMask }
	{vchrNavChange, navBitLeft | navChangeDown | navChangeLeft, commandKeyMask | autoRepeatKeyMask }
	...
	{vchrNavChange, navChangeLeft, commandKeyMask}
*/






//(e.data.keyDown.modifiers & autoRepeatKeyMask)




		case keyDownEvent:		// MemHandle the physical buttons


	   	//	Legacy Beam
	   	if(event->data.keyDown.chr == vchrFind)
	   	{
				//	false -> Force Beaming (unless we're not logged in)
				if(MmBeamOrdersWithOptionalCheckForUnsent(formId, &Prefs, false, &UnsentOrdersCheckSecs))
				{
					OverviewPageType = OverviewTakeOrders;
					PreviousForm = OverviewForm;
					FrmGotoForm(TableForm);
				}

				OverviewExpandedStatus = StatusIsContracted;
			}

			
	   	else if(NavSelectPressed(event))
	   	{
				//	Generate events to 'click' the Overview view select button
				EventType *eventToAdd = (EventType *)MemPtrNew(sizeof(EventType));

				MemSet(eventToAdd, sizeof(EventType), 0);
				eventToAdd->eType = ctlSelectEvent;	//keyDownEvent
				eventToAdd->data.ctlEnter.controlID = OverviewSelectViewButton;
				EvtAddUniqueEventToQueue(eventToAdd, 0, true);

				OverviewExpandedStatus = StatusIsContracted;
			}

			
			else if(NavDirectionPressed(event, Left))
	   	{
				if(OverviewPageType == OverviewServingCourses)
				{
					if(OverviewServingCourse > 1)
						OverviewServingCourse--;
					MmOverviewRedraw(frm, ScrollTop, DoRewriteArray);
				}
				else
				{
					if(Prefs.current.seatNo > 1)
					{
						Prefs.current.seatNo--;
						MmOverviewRedraw(frm, ScrollTop, DoRewriteArray);
					}
				}

			   handled = true;
			}


			else if(NavDirectionPressed(event, Right))
	   	{
				if(OverviewPageType == OverviewServingCourses)
				{
					if(OverviewServingCourse < DmNumRecords(MmServingCourseDB)-1)
						OverviewServingCourse++;
					MmOverviewRedraw(frm, ScrollTop, DoRewriteArray);
				}
				else
				{
					if(Prefs.current.seatNo < LastSeat)
					{
						Prefs.current.seatNo++;
						MmOverviewRedraw(frm, ScrollTop, DoRewriteArray);
					}
				}

			   handled = true;
			}



			else if(NavDirectionPressed(event, Up)	||
						(event->data.keyDown.chr == vchrPageUp)	)
	   	{
				if(OverviewPageType == OverviewTakeOrders)
				{
					if(	!(event->data.keyDown.modifiers & autoRepeatKeyMask)
							&&
							OverviewTopRowNum == 0
							&&
							OverviewExpandedStatus != StatusIsContracted)
					{
						MmOverviewRedraw(frm, ScrollTop, DoRewriteContracted);	//	Sets OverviewExpandedStatus
					}
					else
					{
						MmOverviewScrollUp(frm);
					}
				}
				else
				{
					MmOverviewScrollUp(frm);
				}

			   handled = true;
	   	}


			else if(NavDirectionPressed(event, Down)	||
						(event->data.keyDown.chr == vchrPageDown)	)
	   	{
				if(OverviewPageType == OverviewTakeOrders)
				{
					if(	!(event->data.keyDown.modifiers & autoRepeatKeyMask)
							&&
							!OverviewCanScrollDown
							&&
							OverviewExpandedStatus != StatusIsExpanded
																						)
					{
						MmOverviewRedraw(frm, ScrollTop, DoRewriteExpanded);	//	Sets OverviewExpandedStatus
					}
					else
					{
						MmOverviewScrollDown(frm);
					}
				}
				else
				{
					MmOverviewScrollDown(frm);
				}

			   handled = true;
	   	}

			break;

/*			else if(NavDirectionPressed(event, Up)	||
						(event->data.keyDown.chr == vchrPageUp)	)
	   	{
				if(OverviewPageType == OverviewTakeOrders
					&&
					OverviewTopRowNum == 0
					&&
					OverviewExpandedStatus != StatusIsContracted)
				{
					MmOverviewRedraw(frm, ScrollTop, DoRewriteContracted);	//	Sets OverviewExpandedStatus
				}
				else
				{
					MmOverviewScrollUp(frm);
				}
			   handled = true;
	   	}


			else if(NavDirectionPressed(event, Down)	||
						(event->data.keyDown.chr == vchrPageDown)	)
	   	{
				if(OverviewPageType == OverviewTakeOrders
					&&
					!OverviewCanScrollDown
					&&
					OverviewExpandedStatus != StatusIsExpanded
																					)
				{
					MmOverviewRedraw(frm, ScrollTop, DoRewriteExpanded);	//	Sets OverviewExpandedStatus
				}
				else
				{
					MmOverviewScrollDown(frm);
				}
			   handled = true;
	   	}

			break;
*/
/*
					//	Generate events to 'click' the Overview view select button
					MemSet(eventToAdd, sizeof(EventType), 0);
					eventToAdd->eType = penDownEvent;
					eventToAdd->tapCount = 1;
					eventToAdd->penDown = true;
					eventToAdd->screenX = 75;	//	Replace with Bounds of button!!!
					eventToAdd->screenY = 5;
					EvtAddEventToQueue(eventToAdd);

					MemSet(eventToAdd, sizeof(EventType), 0);
					eventToAdd->eType = penUpEvent;
					eventToAdd->tapCount = 1;
					eventToAdd->penDown = false;
					eventToAdd->screenX = 75;
					eventToAdd->screenY = 5;
					EvtAddEventToQueue(eventToAdd);

*/
/*
					//	If we're in Classic Display Mode
					if(Prefs.userSettings.classicMode)
					{
						OverviewPageType = OverviewOrdersOnlyPage;
						PreviousForm = OverviewForm;
			   		FrmGotoForm(MainForm);
			   	}
			   	
					//	We're in Modern Display (Serving Course) Mode
			   	else
			   	{
						if(OverviewPageType == OverviewOrdersOnlyPage)
							OverviewPageType = OverviewTakeOrders;
						else
							OverviewPageType = OverviewOrdersOnlyPage;

						MmOverviewRedraw(frm, ScrollTop, DoRewriteArray);
			   	}
*/






		case penDownEvent:
			if(event->screenY >= OverviewScreenTopBorder	&&
		   	event->screenY <= OverviewScreenTopBorder + OverviewArrowIconHeight)
			{
				if(event->screenX > OverviewCol0Width	&&
		   		event->screenX < OverviewCol0Width + OverviewArrowIconWidth)
		   	{
					LeftArrowSelected = true;
		   	}
		   	else if(	event->screenX > OverviewUsableScreenWidth - OverviewArrowIconWidth	&&
		   				event->screenX < OverviewUsableScreenWidth)
		   	{
					RightArrowSelected = true;
		   	}
		   }
			break;




		case tblSelectEvent:	// An item has been selected in the table.

			//	If its a Serving Course step through ordering process,
			//	else might be checkbox being pressed,
			//	else might be a seat, so go to Seats page,
			//	else might be an existing order so show details etc.

			TblUnhighlightSelection(tablePtr);
			
			row = OverviewTopRowNum + event->data.tblSelect.row;
			
			



			//	Column0 has the Expand icons, Checkboxes etc.
			if(event->data.tblSelect.column == Column0)
			{
				//	If its a Serving Course ...
				if(TableDisplayRows[row].dishIndex == 0)
				{
					//If its NOT the Serving Course page ...
					if(OverviewPageType != OverviewServingCourses)
					{
						//	Serving Course selected: If its collapsed, expand it, and vice versa
						if(TableDisplayRows[row].setExpanded)
							TableDisplayRows[row].setExpanded = false;
						else
							TableDisplayRows[row].setExpanded = true;
						
						OverviewExpandedStatus = StatusNoChange;	//	Don't force expanded or contracted

						MmOverviewRedraw(frm, CanScroll, DoRewriteArray);
					}
				}



				//	Its Column 0, but there are no checkboxes.
				// We want to Copy the order (sent or not).
				else if(	TableDisplayRows[row].rowDisplayType == RowIsOrder
							&&
							(	OverviewPageType == OverviewTakeOrders
								||
								OverviewPageType == OverviewServingCourses
								||
								OverviewPageType == OverviewOrdersOnlyPage	)	)
				{
					Prefs.current.recIndex = TableDisplayRows[row].recIndex;

					MmCreatNewDuplicateOrder(&Prefs, &DBs, &LastItemOrdered);

					MmOverviewRedraw(frm, NoScroll, DoRewriteArray);
				}


				//	else might be a Seat for Billing checkboxes, or might be marking Unsent for Delete
				else
				{
					UInt8				courseNumber;
					UInt8				thisBillingAction;

					recIndex = TableDisplayRows[row].recIndex;
					recHandle = DmQueryRecord (MmOrdersDB, recIndex);
					if(recHandle)
					{
						recPtr = MemHandleLock(recHandle);

						courseNumber		= recPtr->courseNumber;
						thisBillingAction	= recPtr->billing;
						

						MemPtrUnlock(recPtr);
					}



					//	Seat, so either do an Expand All, or deal with Billing checkboxes
					if(	TableDisplayRows[row].dishIndex == NoValue16	)
					{
						//	If display mode is OverviewPrintReceiptsPage or OverviewUnsentPage
						if(		OverviewPageType != OverviewPrintReceiptsPage
									&&
									OverviewPageType != OverviewUnsentPage)
						{
							if(OverviewExpandedStatus == StatusIsExpanded)
					   	{
								MmOverviewRedraw(frm, ScrollTop, DoRewriteContracted);//	Sets OverviewExpandedStatus
							}
							else
							{
								MmOverviewRedraw(frm, ScrollTop, DoRewriteExpanded);	//	Sets OverviewExpandedStatus
					   	}
						}

						//	Selection checkbox tapped: Toggle its status if Marker (Seat) record
						else if(courseNumber == 0)
						{
							if(thisBillingAction & BillingSeatMarked)
							{
								MmWriteThisBillingAction(recIndex, BillingActionUnselectThis, MmOrdersDB);
							}
							else
							{
								MmWriteThisBillingAction(recIndex, BillingActionSelectThis, MmOrdersDB);
							}
			
							MmOverviewRedraw(frm, NoScroll, DoRewriteArray);
						}
					}
					

					//	Unsent: Deal with Checkboxes
					else
					{
						if(recHandle)
						{
							//	Selection checkbox tapped: Toggle status if Item is to be marked for delete
							if(OverviewPageType == OverviewUnsentPage)
							{
								MmToggleMarkedForDeleteOrder(MmOrdersDB, recIndex);
							}
			
							MmOverviewRedraw(frm, NoScroll, DoRewriteArray);
						}
					}
				}
			}







			else	//	Column1  ie. !(	if(event->data.tblSelect.column == Column0)	)
			{
				//	If its a Serving Course step through ordering process
				if(TableDisplayRows[row].dishIndex == 0)
				{
					//If its the Serving Course page, let them choose a new one
					if(OverviewPageType == OverviewServingCourses)
					{
						UInt16 offset = 0;

						listPtr = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, OverviewDishesList));

						itemSelected = MmPopupList(listPtr, ServingCourseAllList, &offset, 0);
						
						if(itemSelected != NoValue16)
						{
							OverviewServingCourse = ListItemsStruct[itemSelected].recIndex;

							OverviewPageType = OverviewServingCourses;	//	Show the orders by individual SC
							MmOverviewRedraw(frm, ScrollTop, DoRewriteArray);
						}
					}
					else
					{
						//	Serving Course Name selected, step through ordering process
						Boolean screenRedrawn = false;
						
						PreviousForm = OverviewForm;

						MmProcessOrder(frm,	&TableDisplayRows[row], &DBs, &Prefs, &ListItems[0], &SizeListItems[0],
													&ListItemsStruct[0], &SizeListIndices[0],
														/*&IconSelected, &IconSelectedTime, */&LastItemOrdered,
															&OverviewPageType, &OverviewCheckboxType,
																&NumOldUnsentOrders, &screenRedrawn	);

						if(!screenRedrawn)
							MmOverviewRedraw(frm, CanScroll, DoRewriteArray);
					}
				}






				//	else might be a Seat
				else if(	TableDisplayRows[row].dishIndex == NoValue16)
				{
					if(	Prefs.userSettings.classicMode
						&&	
							(	OverviewPageType != OverviewTakeOrders
							 &&
							 	OverviewPageType != OverviewOrdersOnlyPage)	)
					{
						Prefs.current.tableNo = TableDisplayRows[OverviewTopRowNum +
																					event->data.tblSelect.row].tableNumber;
						Prefs.current.seatNo = TableDisplayRows[OverviewTopRowNum +
																					event->data.tblSelect.row].seatNumber;
						Prefs.current.mMenu = TableDisplayRows[OverviewTopRowNum +
																					event->data.tblSelect.row].menuType;
						MainviewTopRowNum = 0;

			   		PreviousForm = OverviewForm;
			   		FrmGotoForm(MainForm);
			   	}

					
					//	If any display modes except OverviewTakeOrders and OverviewOrdersOnlyPage
					else if(		OverviewPageType != OverviewTakeOrders
								&&
									OverviewPageType != OverviewOrdersOnlyPage)
					{
						Prefs.current.tableNo = TableDisplayRows[OverviewTopRowNum +
																					event->data.tblSelect.row].tableNumber;
						Prefs.current.seatNo = TableDisplayRows[OverviewTopRowNum +
																					event->data.tblSelect.row].seatNumber;
						Prefs.current.mMenu = TableDisplayRows[OverviewTopRowNum +
																					event->data.tblSelect.row].menuType;

						OverviewPageType = OverviewTakeOrders;
						MmOverviewRedraw(frm, ScrollTop, DoRewriteArray);
					}
					
					
					//	OverviewPageType == OverviewTakeOrders or OverviewOrdersOnlyPage, play with arrows
					else
					{
						//	Seat item selected: Might be Left Arrow
						if(LeftArrowSelected)
						{
							LeftArrowSelected = false;	//	Left hand arrow (seat decrement)
							
							if(Prefs.current.seatNo > 1)
							{
								Prefs.current.seatNo--;
							}

							MmOverviewRedraw(frm, CanScroll, DoRewriteArray);
						}
						
						//	Seat item selected: Might be Right Arrow
						else if(RightArrowSelected)
						{
							RightArrowSelected = false;	//	Right hand arrow (seat increment)

							if(Prefs.current.seatNo < LastSeat)
							{
								if(OverviewPageType == OverviewOrdersOnlyPage)
								{
									UInt16	tableNo = 0;
									UInt16	seatNo;
									Boolean	orderFound;
									
									
									MmFindFirstSeatOrder(&DBs, &Prefs.current, &orderFound, &tableNo, &seatNo);
									
									if(orderFound	&&	seatNo > Prefs.current.seatNo)
										Prefs.current.seatNo = seatNo;
									else
										Prefs.current.seatNo++;
									
								}
								else
								{
									Prefs.current.seatNo++;
								}
							}

							MmOverviewRedraw(frm, CanScroll, DoRewriteArray);
						}
						
						//	Seat item selected: Must be the Seat itself
						else
						{
							Prefs.current.tableNo = TableDisplayRows[OverviewTopRowNum +
																						event->data.tblSelect.row].tableNumber;
							Prefs.current.seatNo = TableDisplayRows[OverviewTopRowNum +
																						event->data.tblSelect.row].seatNumber;
							Prefs.current.mMenu = TableDisplayRows[OverviewTopRowNum +
																						event->data.tblSelect.row].menuType;
							MainviewTopRowNum = 0;

							PreviousForm = OverviewForm;
							if(Prefs.userSettings.classicMode)
					   		FrmGotoForm(MainForm);
					   	else
								FrmGotoForm(SeatForm);
						}
					}
				}



				//	else might be an existing order so show details etc.
				else
				{
					recIndex = TableDisplayRows[row].recIndex;
					recHandle = DmQueryRecord (MmOrdersDB, recIndex);
					if(recHandle)
					{
						UInt16			mMenu;
						UInt16			tableNo;
						UInt16			seatNo;
						UInt8				courseNumber;
						Boolean			screenRedrawn = false;
						Boolean			forceLogin = false;

						recPtr = MemHandleLock(recHandle);

						tableNo				= recPtr->tableNumber;
						seatNo				= recPtr->seatNumber;
						mMenu					= recPtr->menuType;
						courseNumber		= recPtr->courseNumber;

						MemPtrUnlock(recPtr);
						

						//	Must be an existing item, so show them the Details Form,
						Prefs.current.mMenu		= mMenu;
						Prefs.current.tableNo	= tableNo;
						Prefs.current.seatNo		= seatNo;
						Prefs.current.course		= courseNumber;

						PreviousForm = OverviewForm;
						
						MmProcessOrder(frm,	&TableDisplayRows[row], 
												&DBs, &Prefs, &ListItems[0], &SizeListItems[0],
													&ListItemsStruct[0], &SizeListIndices[0],
														/*&IconSelected, &IconSelectedTime, */&LastItemOrdered,
															&OverviewPageType, &OverviewCheckboxType,
																&NumOldUnsentOrders, &screenRedrawn	);

						if(!screenRedrawn)
							MmOverviewRedraw(frm, CanScroll, DoRewriteArray);
					}
				}
			}
			
			handled = true;
			break;



		case frmTitleEnterEvent:

			if(MmBeamOrdersWithOptionalCheckForUnsent(formId, &Prefs, true, &UnsentOrdersCheckSecs))
			{
				PreviousForm = OverviewForm;
				FrmGotoForm(TableForm);

				MainviewTopRowNum = 0;							//	Scrolling offset for main table
				OverviewTopRowNum = 0;							//	Always start at row 0
			}
			
			handled = true;
			break;



			
		case menuEvent:		// A menu item was selected.

			MmHandleMenuEvent(frm, event);
			
			handled = true;
			break;




		case frmOpenEvent:

			MmOverviewInit();

			if(Prefs.userSettings.classicMode
				&&
				PreviousForm == MainForm
				&&
				OverviewPageType != OverviewUnsentPage)
			{
				OverviewPageType = OverviewOrdersOnlyPage;
			}


			MmOverviewRedraw(frm, CanScroll, DoRewriteArray);

			if(OverviewExpandedStatus == StatusIsExpanded)
				MmOverviewRedraw(frm, CanScroll, DoRewriteExpanded);
			else
				MmOverviewRedraw(frm, CanScroll, DoRewriteContracted);

			handled = true;
			break;

	}
	return(handled);
}

