//---------------------------------------------------------------------------

#ifndef ServingCourseH
#define ServingCourseH

#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TServingCourse
{
public:
	UnicodeString Name;
	UnicodeString KitchenName;
	TColor Colour;
	int ServingCourseKey;
	USHORT ServingCourseID;
	int SCOO;
	bool Deleted;
	bool Selectable;

	void Reset(TServingCourse ResetTo);
	void Clear();
	bool Valid();
	TServingCourse();
};
#endif
