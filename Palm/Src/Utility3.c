#define EXTERNAL 1

#include "MenuMate.h"



/***********************************************************************
 *
 * FUNCTION:    MmProcessCallAwayRecords
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/
Boolean MmProcessCallAwayRecords(DBTypePtr dBs, MmPreferenceTypePtr prefs, 
														UInt16 recServingCourseIndex)
{
	MemHandle					recHandle;
	UInt16						recMiscDataIndex;
	MiscDataTypePtr			recMiscDataPtr;
	
	MiscDataType				callAwayRec;
	
	Boolean						createRecord = false;
	
	Err							err;



	//	Search for a record for this Course in the MiscData DB.
	//	If a match exists, delete it. Else create it.

	recMiscDataIndex = MmFindSortCallAway(prefs->current.tableNo, recServingCourseIndex);

	recHandle = DmQueryRecord(dBs->mmMiscDataDB, recMiscDataIndex);

	if(recHandle)
	{
		recMiscDataPtr = MemHandleLock(recHandle);

		if(recMiscDataPtr->recordType == CallAwayType			&&
			recMiscDataPtr->number == prefs->current.tableNo	&&
			recMiscDataPtr->recIndex == recServingCourseIndex		)
		{
			MemPtrUnlock(recMiscDataPtr);
			err = DmRemoveRecord(dBs->mmMiscDataDB, recMiscDataIndex);//	Nuke the record
			ErrFatalDisplayIf(err, "Could not remove CallAway record");
		}
		else	//	Generate a first new 'Selected' record at correct place
		{
			MemPtrUnlock(recMiscDataPtr);
			createRecord = true;
		}
	}
	else	//	Generate a new 'Selected' record at correct place
	{
		createRecord = true;
	}
	

	if(createRecord)
	{
		callAwayRec.recordType	= CallAwayType;
		callAwayRec.number		= prefs->current.tableNo;
		callAwayRec.recIndex		= recServingCourseIndex;
		callAwayRec.uniqueID		= MmGetServingCourseID(dBs->mmServingCourseDB, recServingCourseIndex);

		//	Insert the record at correct place
		MmGenerateMiscDataRecord(&dBs->mmMiscDataDB, &callAwayRec, recMiscDataIndex);
	}

	return createRecord;
}