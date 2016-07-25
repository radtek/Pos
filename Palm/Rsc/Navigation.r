#define EXTERNAL 1


/********************************
 * Navigation.r
 *
 * Rez template for defining Palm OS 'fnav' resources for specifying the order of navigation
 * within a form. For each form, define a 'fnav' resource with the same ID listing all of the
 * controls in left-right navigation order, as well as specifying up-down navigation.
 ********************************/

#ifndef _NAVIGATION_R_
#define _NAVIGATION_R_


#define frmNavFlags \
	defaultStartStateValue	= 0x00000000, \
	objectFocusStartState	= 0x00000001, \
	appFocusStartState		= 0x00000002

#define frmNavObjectFlags \
	none							= 0x0000, \
	skip							= 0x0001, \
	forceInteraction			= 0x0002, \
	bigButton					= 0x8000

type 'fnav' {
    unsigned integer = 1;      						// resource version
    unsigned integer = $$Countof(ObjectArray);
    unsigned integer = 20;								//	size of header
    unsigned integer = 8;								//	size of list elements
    unsigned longint frmNavFlags;					//	navigation flags
    unsigned integer;									//	ID of initial focus object
    unsigned integer = 0;								//	hint ID of jump to object
    unsigned integer = 0;								//	hint ID of bottom left object
    fill word;

    array ObjectArray
    {
        unsigned integer;								//	ID of object
        unsigned integer frmNavObjectFlags;		//	object flags
        unsigned integer;								//	ID of above object
        unsigned integer;								//	ID of below object
    };
};

#endif

/********************************

example of how you can use this template to define a
form navigation resource.  The IDs are defined by a header
file output by Constructor.
 
resource 'fnav' ( MainForm )
{
    objectFocusStartState,
    MainFileTestButton,
    {
        MainFileTestButton, 0, 0, MainInputTestButton,
        MainInputTestButton, 0, MainFileTestButton, MainLoopTestButton,
        MainLoopTestButton, 0, MainInputTestButton, MainStopButton,
        MainStopButton, 0, MainLoopTestButton, 0
    }
};

*********************************/