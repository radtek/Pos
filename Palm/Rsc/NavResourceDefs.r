#define EXTERNAL 1

#include "MenuMateRsc.h"
#include "Navigation.r"

//	Example of how you can use this template to define a form navigation resource.
//	Controls are in left-right navigation order, as well as specifying up-down navigation.
//		unsigned integer;								//	ID of object
//		unsigned integer frmNavObjectFlags;		//	Object flags
//		unsigned integer;								//	ID of above object
//		unsigned integer;								//	ID of below object

resource 'fnav' (EditLineForm)
{
	objectFocusStartState,		//	Flags
	EditLineNoteField,			//	Initial focus
//	EditLineCancelButton,		//	Object to jump to
//	EditLineOkayButton,			//	Bottom left object
	{
		EditLineNoteField,		0,	EditLineRecentButton,	EditLineOkayButton,
		EditLineOkayButton,		0,	EditLineNoteField,		EditLineCancelButton,
		EditLineCancelButton,	0,	EditLineOkayButton,		EditLineRecentButton,
		EditLineRecentButton,	0,	EditLineCancelButton,	EditLineNoteField,
	}
};

