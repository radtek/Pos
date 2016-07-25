//---------------------------------------------------------------------------


#pragma hdrstop

#include "ServingCourse.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TServingCourse::TServingCourse()
{
   Clear();
}

void TServingCourse::Reset(TServingCourse ResetTo)
{
   *this = ResetTo;
}

void TServingCourse::Clear()
{
   Name = "";
   KitchenName = "";
	ServingCourseKey = 0;
   SCOO = 0;
   ServingCourseID = 0;
   Colour = clMaroon;
   Deleted = true;
   Selectable = true;
}

bool TServingCourse::Valid()
{
	bool retval = false;
	if (ServingCourseKey > 0)
	  retval = true;
	return retval;
}


