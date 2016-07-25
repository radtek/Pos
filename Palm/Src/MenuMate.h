//	Versioning: Comprises 4 parts separated by decimal points, defined by
//	the POS development team so they remain in sync

#define VersionNumberString			"4.1.0.1"
#define VersionNumberStringLength	15
#define TransmissionVersion			2		//	Byte incremented for each transmission version change

#define ReleaseVersion

#define ForceMenus					//	Enable the Force menus to be active
#define TestMenus						//	Enable the Test menus to be active


//	1st Digit = Major version
//	2nd Digit = Minor version: Implies rejection if not synched with POS
//	3rd Digit = Local feature upgrade. No effect on POS
//	4th Digit = Bug fix

//	Version History
//	2.4.0.0 Infrared Version
//	2.4.0.1 Bug fix
//	2.4.0.2 Bug fix
//	2.4.1.0 Added Forced Sides. Added PriceAdj Security. Removed NoValueAssigned.

//	2.7.1.5 Added -> 16 Sides (Side Orders)
//	2.7.1.6 Added multiple BT Retries
//	2.7.1.7 Bug: Fixed menu item appearing as forced options
//	2.7.1.8 Bug: Fixed forced options not appearing for more than 16 item in course
//	2.7.1.9 Bug: Fixed Wrap around of Int8 itemNumber. Changed to Int16
//	2.7.1.10 Added PriceAdj Security. 

//	3.0.0.0 Altered User ID to 16 bit. Fixed 1st Forced Option not working
//	3.0.0.1 Bug: Fixed Wrap around of Int8 itemNumber. Changed to Int16 (As above!)
//	3.0.0.2 Bug: Fixed crash on OverviewScrollUp/Down
//	3.0.0.3 Added PriceAdj Security. 
//	3.0.0.4 Added extra menu options eg Sound level.
//	3.0.0.5 Disable Forced Options if Forced Sides abandoned.
//	3.0.0.6 Tidied Warning handling. Added warning for user list changed.
//	3.0.0.7 Bug: Transaction number on Palm now only incremented on SendOrders.
//	3.0.0.8 Increased Sides count (to 31).
//	3.0.0.9 Added limit to time since orders taken to 0xFFFF. Multiple message gets if incoming message type not same as outgoing.
//	3.0.0.10 Fixed PalmT5 5-way Navigator

//	3.1.0.0 Unsent orders Overview page (Ver never released)
//	3.2.0.0 Variable PIN length, BT address as Palm reg ID (Ver never released)
//	3.3.0.0 Version No sent to PalmMate, Palms initiate reg process, Forced Sides.
//				Moved code to secondary segment. Added time taken max to 0xFFFF.
//				Fixed PalmT5 5-way Navigator
//				Added bailout if number of sizes exceed 16 per Item
//	3.3.0.1 Bug fix prevents T5s crashing on MenuSync. (MmOrdersDB zapped once only).
//	3.3.0.2 2nd attempt to fix bug that prevents T5s crashing on MenuSync. (MmOrdersDB zapped on GetAllOrders) 
//	3.3.0.3 Bug: Version number message didn't work
//	3.3.0.4 Final fix for T5 issues, Added Auto-re-registration (both ported from V3.4)
//	3.3.0.5 More fix attempts for T5 issues, and fix Seatnames splattering across seats if entered before orders
//	3.3.0.6 Fix auto-re-registration.
//	3.3.0.7 Bug fix. (UInt8) casts not parenthesised in some places

//	4.0.0.0 Serving Courses,
//				Overview/Orders screen revamp,
//				Seatname transfers fixed,
//				User settings added,
//				Added ScreenLock
//	4.0.0.1 Fixed bug with NumUnsentOrders not always tracking
//	4.0.0.2 Added setMenuGroup displayed in []
//	4.0.0.3 Added warning on Delete in Unsent view, Fixed bug where seatnames repeated for each table, Fixed Server Message wrap-around error, Converted to scroll 1 line rather than 1 page
//	4.0.0.4 Modified Overview UI to be Column based
//	4.0.0.5 Fixed Seat Up/Down icons on screen not always working.
//	4.0.0.6 Fixed lockup when exit to OS when Options list displayed
//	4.0.0.7 Fixed setMenuGroup above SetMenuGroupMaxValue bug. Added TransmissionVersion
//	4.0.0.8 Fixed SetMenu key item sometimes not displaying in TakeOrders view.
//	4.0.0.9 Added SendOrders timeout per order
//	4.0.0.10 - 14 Added Copy x n code. Generalised Keypad code for Price Adj and Copy. Made NoPrice value NoValue32Signed. Stored BundleGroups in dynamic storage.
//	4.0.0.15 Code brought forward into SVN system using BeyondCompare. Fixed ScreenLock on E2s.
//	4.0.0.16 Prevent order taking when logged out to avoid possible corruption issues (Table 127 etc!)
//	4.0.0.17 Fixed Table/Seat price displays as 0.-1 for value = 0
//	4.0.0.18 Fixed some courses not selectable (incorrect selection of course index) (Misceo) Fixed Forced Sides not working for Drinks (JDV At the Rocks)
//	4.0.0.19 Fixed When adding Forced Sides need available sizes displayed for each Forced Side. Reinstate 'Copy' when user taps to left of item bullet.
//	4.1.0.0	Added HLWarnCallAwayPrinterNotSetUp and HLWarnKitchenPrinterNotSetUp and stacking warning messages.
//	4.1.0.1	Fixed Fatal crash when adding options due to Chunk Overlocked


//	4.1.0.0 Added Payments from Palms

//	DB Scope V1 Key 8142771511
//	DB Scope V2 Key 6849387531


#ifndef __MENUMATE_H__			//	If not defined include this file (to #endif)
#define __MENUMATE_H__







//#define PalmZire


//#define DebugForceMessage	//	Force dummy message to be installed into serial buffer


#define IrDA						//	Directly drive IrDA port as a serial port


#define ForceTableNos	10		//	Number of Tables to Force
#define ForceSeatNos		20		//	Number of Seats to Force
#define ForceOrderNos	10		//	Number of Orders to Force per seat
#define ForceSeatnames	4		//	Number of Seatnames per table


//#define HideBilling			//	Turn off billing. (Also set Overview Col 0 width = 109 (else 143))

//	#define AllowDebugLogDatabaseButton

#ifdef ReleaseVersion
	#undef ERROR_CHECK_LEVEL
	#define ERROR_CHECK_LEVEL ERROR_CHECK_NONE

#else
	#undef ERROR_CHECK_LEVEL
	#define ERROR_CHECK_LEVEL ERROR_CHECK_FULL


#ifndef ForceMenus
	#define ForceMenus			//	Enable the Force menus to be active
#endif


#ifndef TestMenus
	#define TestMenus				//	Enable the Test menus to be active
#endif

//	#define StandAlonePalm		//	Disable Login requirements etc. Use with DebugFakeMenu
	#define DebugFakeMenu		//	Allow default menu to be installed into serial buffer

//	#define TestMessageLog
//	#define CountOrdersSent
//	#define CheckSizeIndices	//	Confirm that each drink index has corresponding size

//	#define DebugDLMike
//	#define DebugDL				//	Insert Datalink Layer (1) debug display code
//	#define DebugDL2				//	Insert Datalink Layer (2) debug display code
//	#define DebugDLLost			//	Insert Lost Datalink Layer packets debug logging code
//	#define DebugHL				//	Insert High level Layer (1) debug data logging
//	#define DebugHL2				//	Insert High level Layer (2) debug data logging
//	#define DebugHL3				//	Insert High level Layer (3) debug data logging
//	#define DebugHLFake			//	Insert High level Layer debug data logging
//	#define DebugTiming1			//	Show timing of Sort etc functions
//	#define DebugTiming2			//	Show timing of GetAllOrders functions
#endif

//#include <PalmTypes.h>		// 
#include <PalmOS.h>				// all the system toolbox headers
//#include <Bitmap.h>				// 
#include <ErrorMgr.h>			// For Error Displays
//#include <Chars.h>				// For symbol fonts
#include <PalmOneChars.h>		// For symbol fonts
#include <PalmResources.h>		// 
#include <PalmOneNavigator.h>	// 
//#include <PalmNavigator.h>	// 
//#include "Navigation.r"			// 
//#include "NavResourceDefs.r"	// 
#include <SysEvtMgr.h>			//	Needed for search for EvtSysEventAvail
#include <Preferences.h>
#include <FeatureMgr.h>			//	Needed to get the ROM version
#include <KeyMgr.h>
#include <Keyboard.h>
#include "MenuMateRsc.h"		// application resource defines
#include <SerialMgrOld.h>
#include <IrLib.h>
#include <ExgMgr.h>
#include <ExgLib.h>
//#include "Hardware.h"
#include <Table.h>
#include <Progress.h>
#include <DLServer.h>   		// for DlkSetLogEntry for HotSync log
#include "CRCTable.h"
#include "ComsLink.h"
#include <unix_stdarg.h>

#include <PalmOSGlue.h>			//	Glue functions to allow fonts to be set in lists
//NB. You need to add PalmOSGlue.lib to the project for this to work

#include <BtCommVdrv.h>
//#include "BTSerial.h"
//#include "Bluetooth.h"

//#include <hostcontrol.h>



//	Tidy up the inclusion of extern
#ifdef EXTERNAL
#define EXTERN  extern
#else
#define EXTERN

#endif








//////////////////////////////////////////////////////////////////////
// Using HotSync log for debug-time error logging on the handheld
//////////////////////////////////////////////////////////////////////

#ifndef ReleaseVersion


//#include <DLServer.h>   // for DlkSetLogEntry
#include <unix_stdarg.h>     // for va_arg and friends

static void PrvSyncLogTrace(Char* formatStrP, ...);
static void PrvSyncLogTrace0(Char* strP);
 
// Macros for tracing to HotSync log

#ifndef ReleaseVersion

	// MACRO:   HSLOGTRACE
	//
	// DESCRIPTION:  Outputs a debug message consisting of printf-style
	//           format string and multiple parameters into HotSync Log
	//
	// USAGE:   HSLOGTRACE(Char* formatStrP, ...)
	//
	// NOTES:
	//  LIMIT TOTAL RESULT STRING LENGTH TO 50 CHARACTERS
	//
	//  Currently, only %d, %i, %u, %x and %s are implemented
	//  and don't accept field length or format specifications
	//  except for the l (long) modifier.
	//
	//
	 
	#define HSLOGTRACE         PrvSyncLogTrace
	
	
	// MACRO:	HSLOGTRACE0
	//
	// DESCRIPTION:   Outputs a debug message consisting of a single
	//                text string into HotSync Log
	//
	// USAGE:   HSLOGTRACE(Char* strP)
	//
	//
	//
	
	#define HSLOGTRACE0(strP)   PrvSyncLogTrace0((strP))
	#define HSLOGTRACE0IF(cond, strP)  ((cond) ? PrvSyncLogTrace0((strP)) : (void)0)
	 
	// MACRO:   HSLOGCLEAR
	//
	// DESCRIPTION:   Clears all messages from HotSync log
	//
	// USAGE:   HSLOGCLEAR()
	//
	//
	
	#define HSLOGCLEAR()         DlkSetLogEntry("", 0, false)
#else
	#define HSLOGTRACE            1 ? (void)0 : PrvSyncLogTrace
	#define HSLOGTRACE0(strP)
	#define HSLOGTRACE0IF(cond, strP)
	#define HSLOGCLEAR()
#endif

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION:   PrvSyncLogTrace
//
// DESCRIPTION: Append a message consisting of printf-style parameters
//              to the HotSync log for debugging.
//
// PARAMETERS:   formatStrP   -- printf-style format string
//               ...          -- printf-style variable arguments
//
// RETURNED:	nothing
//
// NOTES:
//   Currently, only %d, %i, %u, %x and %s are implemented
//   and don't accept field length or format specifications
//   except for the l (long) modifier.
//
// REVISION HISTORY:
//   Name   Date   Description
//   ----   ----   -----------
//
///////////////////////////////////////////////////////////////////////////////

#define prvMaxLogTraceBufSize		1001//51

static void PrvSyncLogTrace(Char* formatStrP, ...)
{
#ifndef ReleaseVersion
//#if ERROR_CHECK_LEVEL == ERROR_CHECK_FULL
 
   Char   text[prvMaxLogTraceBufSize + 25];   // extra 25 bytes for safety margin
   va_list   args;
   Int16     result;

   // Check for err
   ErrFatalDisplayIf(formatStrP == NULL, "NULL string passed");
   ErrFatalDisplayIf(StrLen(formatStrP) >= prvMaxLogTraceBufSize,
		"PrvSyncLogTrace fmt str too big");
  
   va_start(args, formatStrP);
   result = StrVPrintF(text, formatStrP, args);
   va_end(args);
 
   // Make sure the result string did not overflow our text buffer
   ErrFatalDisplayIf(StrLen(text) >= sizeof(text), "PrvSyncLogTrace buf overflow");
 
   // Append the result string to the HotSync log
   DlkSetLogEntry(text, StrLen(text), true);
 
#endif	
}


///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION:   PrvSyncLogTrace0
//
// DESCRIPTION:   Append a message consisting of a single text string
//                to the HotSync log for debugging.
//
// PARAMETERS:   strP      -- text string
//
// RETURNED:   nothing
//
// REVISION HISTORY:
//      Name   Date      Description
//      ----   ----      -----------
//
///////////////////////////////////////////////////////////////////////////////

static void PrvSyncLogTrace0(Char* strP)
{
   DlkSetLogEntry(strP, StrLen(strP), true);
}

#endif


#define VerNumStrShortLength			6		// As sent to POS. Eg Ver = 3.2

//#define DisplayIconLength				5		//	Any Icon prefixes listed here must not exceed
#define DisplaySidesIcon				">"
#define DisplaySetMenuItemIcon		"[ ]"
#define DisplayDrinkMixIcon			"Mix"

#define LastTable							99		// Last usable table
#define LastSeat							99		// Last usable seat
#define CashTable							LastTable + 1	// Cash is handled like a table
#define LastTableRecord					CashTable + 1	// Fictitious table marks end of DB

#define MaxTables							LastTableRecord// Max number of list items allowed
#define MaxCourseItems					32		// Max number of courses allowed
#define MaxListItems						63		// Max number of items allowed in a list
#define MaxOptionsItems					63		// Max number of Options allowed
//#define MaxForcedSidesItems			63		//	Max number of Forced Sides allowed

#define MaxForcedOptionGroups			15		// Max number of Forced Option groups allowed
#define MaxForcedSidesGroups			8		// Max number of Forced Sides groups allowed
//#define MaxGroupItems					32		//	Max number of Billing Groups
#define MaxLinkedItems					63		//	Number of Linked Items (Sides)
#define MaxSizeTypes						16		//	Max number of Sizes per Class

#define MaxMenuItemSize					25		// Bytes allowed for Menu Item string
#define MaxOptionItemSize				25		// Bytes allowed for Option Item string
#define MaxInfoNameSize					25		// Bytes allowed for Info Name string
#define MaxMenuNameSize					25		// Bytes allowed for Menu Name string
#define MaxTableNameSize				25		// Bytes allowed for Table Name string
#define MaxTableLabelSize				10		// Bytes allowed for Table Name string in Button
#define MaxSeatNameSize					25		// Bytes allowed for Seat Name string
#define MaxSeatLabelSize				10		// Bytes allowed for Seat Name string in Button
#define MaxMessageNameSize				25		// Bytes allowed for Message Name string
#define MaxInfoInitialSize				3		// Bytes allowed for Initial string
#define MaxNoteLength					80		// Max number of characters allowed in a Note
#define MaxMessageFromServerLength	80
#define MaxRecentNotesListItems		5			
#define MaxNoteLabelLength				25			

#define MaxTableScrollPages			25
#define MaxTableDisplayRows			MaxTableScrollPages*13

#define BundleGroupsMax					128	//	Set static number of sizes to bundle

#define MaxPINSize						10		//	Max number of PIN characters

#define MaxBtTransceivers				5		//	Max number of Bluetooth devices allowed
#define BtAddrStringSize		   	18

#define LogOffMinimumTime				5
#define Hard2ReleaseDelay				1
#define AutoGotoTablePageShortSecs	10
#define AutoGotoTablePageLongSecs	120
#define AutoScreenLockShortTimeSecs	20
#define AutoScreenLockLongTimeSecs	60
#define ScreenLockButtonSecs			2
#define UnsentOrdersCheckTimeout		10

#define PalmUniqueNameLength			17		//	Total length of Unique Name fields
#define PalmPrefSpareLength			4		//	Spare space made available from Unique Name
#define PalmRegistrationNameLength	25		//	Total length of Friendly Name field

#define MainTableSizeStandardFont	11		// Number of lines in Main table
#define MainTableSizeLargeFont		9		// Number of lines in Main table

#define OvervwTableSizeStandardFont	13		// Number of lines in Overview table
#define OvervwTableSizeLargeFont		10		// Number of lines in Overview table

#define BevTableSizeStandardFont		10		// Number of lines in Main table
#define BevTableSizeLargeFont			7		// Number of lines in Main table

#define DishListSizeStdFont			16		// Number of lines in Main table
#define DishListSizeLargeFont			13		// Number of lines in Main table


#define NoScroll							0		//	Scroll to top when table reloaded
#define CanScroll							1		//	Can Scroll to new seat when table reloaded
#define ScrollTop							2		//	No scroll when main table reloaded

#define NoRewriteTable					0		//	Leave display Table unchanged, just refresh buttons
#define NoRewriteArray					1		//	Leave display array unchanged
#define DoRewriteArray					2		//	Rewrite the display array
#define DoRewriteExpanded				3		//	Rewrite the display array Expanded
#define DoRewriteContracted			4		//	Rewrite the display array Contracted

#define StatusNoChange					0		//	OverviewExpandedStatus: Leave Overview status Unchanged
#define StatusIsExpanded				1		//	OverviewExpandedStatus: Set Overview status Expanded
#define StatusIsContracted				2		//	OverviewExpandedStatus: Set Overview status Contracted


#define CopyPriceAdj						1		//	Copy recPtr->priceAdj to CurrentRecord

#define CourseOptionsPlus				1		//	Selection value for LoadList()
#define CourseOptionsMinus				2		//	As above
#define CourseForcedOptions			3		//	As above
#define SelectedOptions					4		//	As above
#define LinkedItems						5		//	As above
#define NoteString						6		//	As above
#define CoursesList						7		//	As above
#define ItemsList							8		//	As above
#define ItemsListNotSetMenyKey		9		//	As above
#define SetMenuItemsList				10		//	As above
#define ForcedSidesList					11		//	As above
#define ServingCourseAllList			12		//	As above
//#define ServingCourseAllSelctblList	13		//	As above
#define ServingCourseDishesList		14		//	As above
#define ServingCourseDrinksList		15		//	As above
#define SizeList							16		//	As above
#define SetMenuSizeList					17		//	As above
#define AllSizeList						18		//	As above
#define SeatsList							19		//	As above
#define ActiveSeatsList					20		//	As above
#define TablesList						21		//	As above
#define ActiveTablesList				22		//	As above
#define StaffList							23		//	As above
#define PalmsList							24		//	As above
#define CallAwayList						25		//	As above
#define RecentNotesList					26		//	As above
#define BtDevicesList					27		//	As above
#define DisplayViewList					28		//	As above



#define SetMenuKey						0x80000000	//	Master status for 'SetMenu' items


#define SizeForNewOrder					1		//	Selection value for MmAskUserForSize()
#define SizeForNewSetMenuItem			2		//	As above
#define SizeForNewSide					3		//	As above

#define AllAvailableSizes				1		//	Selection value for MmCountNumSizeTypes()
#define AllSizesNotSetMenuKey			2		//	As above
#define OnlySetMenuSizes				3		//	As above
#define AllSizesForSide					4		//	As above


#define UnsendSentAndConfirmed		1		//	If Sent/Confirmed orders, mark as Unsent
#define UnsendSentButNotConfirmed	2		//	If Sent/Unconfirmed orders, mark as Unsent
#define CheckUnsent						3		//	Returns true if any Unsent orders found

#define BtOpenOnly						1
#define BtCloseOnly						2
#define BtOpenAndClose					3

#define BtNewDiscoveryList				false
#define BtShowLastDiscoveryList		true

#define NoReset							0
#define Reset								1

/***********************************************************************
 * Structures for this module
 **********************************************************************/

// P11. An application info structure is intended to be stored in the
// database's app info block and to contain data relevant to the database.
// For now we store just category information.  Category information is 
// always placed in the beginning where the category.c code expects it.
typedef struct 
{
   UInt16	renamedCategories;		// bitfield of categories with a changed name
	char		categoryLabels[dmRecNumCategories][dmCategoryLength];
	UInt8		categoryUniqIDs[dmRecNumCategories];
	UInt8		lastUniqID;			// Uniq IDs for categories generated by the device 
									// are between 0 - 127.  Those from the PC are 128 - 255.
	UInt8		reserved1;			// from the compiler word aligning things
	UInt16	reserved2;
} MmAppInfoType;

typedef MmAppInfoType *MmAppInfoPtr;

//	Status is used for Sent(Bit 0), Confirmed as sent(Bit 1), and Note attached(Bit 3)
//	flags etc. and also to hold the itemCounter when multiple dishes are selected (upper
//	UInt8). Status is written into the Int16 field in the main table, column 2 (so it is
//	accessable from MmMainviewDrawRecordInBounds().
//	Bit 5 in a Marker record signals that a Seatname needs to be sent

//	Billing (in Marker records) holds the billing group number in its lower bits.
//	Bit 6 is used in the billing function to identify the current round of billing
//	operations.
//	Bit 7 is then written when the record is sent. If Bit 7 is set when the Billing
//	message is confirmed, then the record is deleted.

//	Options: The Option number stored in the Orders database is an index into the Options
//	database. This must be accessed to find the Option number relative to the Course,
//	as used in the messages sent to the server.
//	Option Bit 7 -> Last Option, Bit 6 -> Minus Option

//	OrderType is DishesMenu or DrinksMenu but DrinksItems may be modified with FlagsHasItemLinks

//	The Price field holds the price in cents. Marker records hold the price correction, sent 
//	from the server, for any items ordered on the server that are not available on the Palm.

typedef struct 
{
	UInt16		index;	//	Absolute Index into menu DB (Index[0] of Marker holds SeatnameDB index)
	UInt16		size;		//	Absolute Index into size DB.
} MixType;


typedef struct
{
	UInt16		tableNumber;
	UInt16		seatNumber;
	UInt8			menuType;				//	DishesMenu/DrinksMenu
	UInt8			courseNumber;
	UInt16		itemNumber;
	UInt8			itemInCourse;
	UInt8			itemCounter;			//	Used to identify multiple dishes c/w options
	UInt8			billing;					//	Billing action number. See note above
	UInt32		setMenuMask;			//	MSB==1 => Master SetMenu item. Non 0 => special group mask
	UInt16		setMenuGroup;			//	MSB==1 => Temp group no. 0 => Unique no. from server
												//	Marker record holds setMenuGroup to use for this order
	UInt16		size;						//	Normally menuIndex[0].size, may be other for Sides
	MixType		menuIndex[MaxLinkedItems];	//	Absolute Index into menu, and size DBs, for Linked items
												//	Index[0] of Marker holds SeatnameDB index
	UInt16		status;					//	See note above
	UInt16		servingCourseID;		//	Index in list. Could be Unique ID in future
	Int32			price;					//	Dish price in cents. Marker has Seat Correction
	UInt32		timeTaken;
	UInt8			noteLength;
	Char			note;
	UInt8			options;
} OrderType;

typedef OrderType *OrderTypePtr;


typedef struct 
{
	UInt16		tableNumber;
	UInt16		seatNumber;
	UInt8			menuType;				//	DishesMenu/DrinksMenu
	UInt8			courseNumber;
	UInt16		itemNumber;
	UInt8			itemInCourse;
	UInt8			itemCounter;			//	Used to identify multiple dishes c/w options
	UInt8			billing;					//	Billing action number. See note above
	UInt32		setMenuMask;			//	MSB==1 => Master SetMenu item. Non 0 => special group mask
	UInt16		setMenuGroup;			//	MSB==1 => Temp group no. 0 => Unique no. from server
												//	Marker record holds setMenuGroup to use for this order
	UInt16		size;						//	Normally menuIndex[0].size, may be other for DrinkMix
	MixType		menuIndex[MaxLinkedItems];	//	Absolute Index into menu, and size type.
												//	Index[0] of Marker also holds SeatnameDB index
	UInt16		status;					//	See note above
	UInt16		servingCourseID;		//	Index in list. Could be Unique ID in future
	Int32			price;					//	Dish price in cents. Marker has Seat Correction
	UInt32		timeTaken;
	UInt8			noteLength;
	Char			note[MaxNoteLength+1];
	UInt8			options[MaxOptionsItems+1];
} CurrentOrderType;

typedef CurrentOrderType *CurrentOrderTypePtr;



typedef struct 
{
	UInt16		size;
	Int32			price;
	UInt32		setMenuMask;			//	MSB==1 => Master SetMenu item. Non 0 => SetMenu mask
	Boolean		mayBeStdItem;			//	Set menu item also available as a standard item
	UInt16		servingCourseIndex;	//	Index of Serving Course in MmServingCourseDB
} MenuPriceType;

//	A MenuType record stores Dish or Drinks data in their respective databases


typedef struct
{
	UInt8				menuID;
	UInt8				courseNumber;
	UInt16			itemNumber;
	UInt8				itemInCourse;
	UInt8				enabled;
	Boolean			popupItemSizes;	//	If true, force popup of item sizes automatically
	Boolean			onlyAsSide;			//	If true, item is available only as a Side
//	UInt32			setMenuMask;		//	MSB==1 => Master SetMenu item. Non 0 => special group mask
	UInt16			uniqueID;			//	Unique ID supplied by server
	UInt16			courseIndex;		//	Index of Course record(ie itemNumber=0)
	UInt16			forcedSidesIndex;	//	Index of Forced Side in MmForcedSidesDB
	UInt16			servingCourseDefault;
	char				itemName[MaxMenuItemSize+1];
	char				disabledItemName[MaxMenuItemSize+3];
//	Int32				price;
	MenuPriceType	sizes[MaxSizeTypes];
} MenuType;

typedef MenuType *MenuTypePtr;


//	Forced Options Mask definition:
//
//		optionFlags
//			Bit 7		Next Button displayed (ie allow skip)
//			Bit 6		Available as minus option
//			Bit 5		Available as plus option
//			Bit 4		(Not) Multi-Select (ie 0 = multi select, 1 = select 1 only)
//			Bit 3		-
//			Bit 2		 [	Group number
//			Bit 1		 [		They may be prompted up to 16 times with lists of forced options
//			Bit 0		-
//
//		optionItemMask  -> Bit mask of items that will force select this option

#define OptionsNextButtonBit			0x80
#define OptionsMinusBit					0x40
#define OptionsPlusBit					0x20
#define OptionsNotMultiSelectBit		0x10


typedef struct 
{
	UInt8			courseNumber;			//	Course Number
	UInt8			courseOptionNumber;	//	Option index within THIS course
	UInt8			optionFlags;			//	Bit 6 = - option, 5 = +, Bits 3->0 = Group no.
	UInt32		optionItemMask;		//	Identifies items this option is forced for
	char			optionName[MaxOptionItemSize+1];
	char			optionNameFit[MaxOptionItemSize+1];	//	optionName, trimmed to fit list width
	char			optionMinusNameFit[MaxOptionItemSize+4];	//	Allow for '- OptionName'
	char			optionSelectedNameFit[MaxOptionItemSize+4];	//	Allow for '* OptionName'
} OptionsType;

typedef OptionsType *OptionsTypePtr;


typedef struct 
{
//	UInt16		menuID;				//	ID for both the masterItem and the Side's itemIndex
	UInt16		masterItem;			//	Menu item to which this Side is a slave
	UInt16		groupNumber;		//	Forced Sides are prompted for in groups (up to 8)
	Boolean		nextButton;			//	Next Button displayed
	UInt16		maxSelect;			//	Max sides user may select
	UInt16		itemIndex;			//	Absolute index of the Side in the Food or Drinks Menu
} ForcedSidesType;

typedef ForcedSidesType *ForcedSidesTypePtr;

/*
	No Longer Relevant!!!

//	Seatnames:

//	The Seatname data type is used for attaching Guest names to Seat numbers.
//	StatusAsHasSeatnameToSend bit of rec.status is set if seatname is modified.

//	Seatnames may be entered before or after an order exists for that seat.
//	The Marker record is used as a go between. If no order exists, the seatname record is 
//	created and its status marked as StatusAsHasSeatnameToSend. When an order is taken the
//	Marker is modified:
//		if((recSeatnameIndex = MmCheckIfCurrentOrderSeatHasName(&Prefs, SeatnameDB)) != NoValue16)
//		{
//			recMarker.status |= StatusAsHasSeatnameToSend;
//			recMarker.menuIndex[0].index = recSeatnameIndex;
//		}
//	
//	If an order does exist, its Marker record is modified similarly.
//
//	When orders are sent only seatnames with their status bit set are sent. The status
//	bits in the Marker and seatname records are reset during MmMarkSentAsConfirmed().
//
//	At end of a Beaming operation, all seatnames of length 0 are deleted.
//	Then the Orders DB Marker records have their Seatname indices re-aligned to the correct
//	SeatnameDB indices. ie. menuIndex[0].index
*/

typedef struct 
{
	UInt16			tableNumber;
	UInt16			seatNumber;
	UInt8				status;
	char				name[MaxSeatNameSize+1];
} SeatnameType;

typedef SeatnameType *SeatnameTypePtr;

//	The Size data type is used for Glass, Bottles, Cans etc, or Standard, Large, Small
typedef struct 
{
	UInt16			size;										//	size Type
	char				sizeName[MaxInfoNameSize+1];
} SizeType;

typedef SizeType *SizeTypePtr;


//	The Info data type is used for StaffType and PalmsType records.
typedef struct 
{
	UInt16			recordType;								//	Staff, Tablename
	UInt16			number;									//	Staff ID Number
	UInt16			uniqueID;
	char				name[MaxInfoNameSize+1];
//	char				nameFit[MaxInfoNameSize+1];		//	name, trimmed to fit list width
	char				initial[MaxInfoInitialSize+1];	//	Really only for staff records
} InfoType;

typedef InfoType *InfoTypePtr;



//	The MiscDataType data type is used for CallAwayType and MessageType records.
//	CallAway records are always at the start of the DB and are entered,
//	one record per Table/Course, in any order.
typedef struct 
{
	UInt16			recordType;								//	CallAway, Message records, 
	UInt16			number;									//	Table for CallAways
	UInt16			recIndex;								//	CallAways Serving Course record index
	UInt16			uniqueID;
	char				name[MaxMessageFromServerLength+1];
} MiscDataType;

typedef MiscDataType *MiscDataTypePtr;



//	Options / DrinkMix List structure
typedef struct 
{
	Char				trimName[MaxMenuItemSize+1];
	Boolean			selected;
	UInt16			recIndex;
	UInt16			sizeIndex;										//	Used for Forced Sides selection list
} ListStructType;

typedef ListStructType *ListStructTypePtr;





//	The ServingCourseType data type is used for Serving Course records in the DB.
typedef struct 
{
	UInt16			recordType;								//	Serving Course Type
	UInt16			menuType;								//	MenuType for servingCourses
	UInt16			uniqueID;
	Boolean			selectable;								//	Only display in selectors if selectable
	UInt32			color;
	char				standardName[MaxMessageFromServerLength+1];
	char				selectedName[MaxMessageFromServerLength+1];
} ServingCourseType;

typedef ServingCourseType *ServingCourseTypePtr;



//	Set up a 2D array to hold the indexes to Orders (rows) for each Serving Course (cols)
typedef struct 
{
//	UInt16					tableNumber;
//	UInt16					seatNumber;
	UInt16					index;
	UInt16					servingCourseID;
	Boolean					selectable;				//	May be displayed for selection. Only used in serveIndxsPtr[0]
	Boolean					isSetMenuKey;			//	Set Menu Key item. Only used in serveIndxsPtr[0]
} ServingCourseIndexType;

typedef ServingCourseIndexType *ServingCourseIndexTypePtr;


//	Stores general data for the Serving Courses (effectively column headers for above) 
typedef struct 
{
//	UInt16					seatNumber;
	UInt16					menuType;
	UInt16					sentTotal;
	UInt16					unsentTotal;
	UInt16					servingCourseID;
	Boolean					selectable;				//	May be displayed for selection. Used to mask non-selectable
	Boolean					showCollapsed;			//	collapsed = Sent Orders hidden, Right arrow displayed
	Boolean					setExpanded;
} ServeDataType;

typedef ServeDataType *ServeDataTypePtr;





//	Overview page configuration options
typedef struct 
{
	Boolean					servingCourses;
	Boolean					thisServingOnly;
	Boolean					seatNumbers;
	Boolean					orders;
	Boolean					ravelOrders;
	Boolean					unsentOnly;
	Boolean					wholeTable;
	Boolean					startThisSeat;
	Boolean					allTables;
} PageViewType;

typedef PageViewType *PageViewTypePtr;


typedef struct 
{
	UInt16		markerIndex;
	UInt16		currentIndex;
	Boolean		isDishesPass;
} OverviewMarkerType;



//	The Overview page is pre-loaded as an array of this type for all possible values.
//	The display scans this array for as many items as can fit on the display
//	UInt16 table; is loaded with the current table.
//	UInt16 seat; is loaded with the current seat. Seat == NoValue16 => end of data
typedef struct 
{
	UInt16		overviewPageType;			//	
	UInt16		rowDisplayType;			//	
	UInt16		tableNumber;				//	
	UInt16		seatNumber;					//	
	UInt16		dishIndex;					//	Column[0]
	UInt16		recIndex;					//	Column[1]
	UInt16		menuType;					//	Column[3]
	UInt16		status;						//	Column[2]
	UInt16		billing;						//	Column[4]
	Boolean		showCollapsed;
	Boolean		setExpanded;
	Boolean		drawLine;
	
	UInt16		column[5];
} TableDisplayType;


//	Set up an array table to hold data to bundle each dish/drink item and size etc
//	Allows display to show 3x Small Steak etc.
//	Separately stores the sent and unsent values of Count and Row for each size
typedef struct 
{
	UInt16							menuIndex;
	UInt16							servingCourseID;
	UInt16							setMenuGroup;
	UInt16							size;
	Int32								price;
	UInt16							sentCount;
	UInt16							unsentCount;
	UInt16							sentRow;
	UInt16							unsentRow;
	Boolean							isLinked;
} BundleGroupType;

typedef BundleGroupType 			*BundleGroupTypePtr;





typedef struct
{
	UInt8		HLRegisterStatus;			//	Non-zero if action required upon Beaming...
	UInt8		HLInfoSyncStatus;			//	... Values defined in IrLink.h
	UInt8		HLMenuSyncStatus;
	UInt8		HLDrinkSyncStatus;
	UInt8		HLLoginStatus;
	UInt8		HLSendSeatnamesStatus;
	UInt8		HLBtTimeoutStatus;
	UInt8		HLSendOrdersStatus;
	UInt8		HLCallAwayStatus;
	UInt8		HLBillSeatsStatus;
	UInt8		HLTransferTablesStatus;
	UInt8		HLGetAllOrdersStatus;
} HLStatusType;

typedef HLStatusType *HLStatusTypePtr;

typedef struct 
{
	UInt16		messageLength;
	UInt8			message[MyALReceiveBuffSize];
	UInt16		returnValue;
} HLCommandType;


//	Bit position multpliers for User Settings
typedef enum userSettingsBits
{
	advancedUserBit	= 0x0001,
	priceAdjustBit		= 0x0002,
	autoDetailsBit		= 0x0004,
	isLargeFontBit		= 0x0008,
	screenLockBit		= 0x0010,
	classicModeBit		= 0x0020,
	soundLevelBit0		= 0x0040,
	soundLevelBit1		= 0x0080,
	getAllTablesBit	= 0x0100
}UserSettingsBits;
/*
typedef enum userSoundLevel
{
	soundLevelOff		= 0x0001,
	soundLevelLow		= 0x0002,
	soundLevelMed		= 0x0004,
	soundLevelHigh		= 0x0008
}UserSoundLevel;
*/

typedef struct// userSettingsType
{
	Boolean				soundLevel0;
	Boolean				soundLevel1;
}UserSoundLevelType;



//	These are traded with the POS so settings are preserved across logins
typedef struct// userSettingsType
{
	Boolean					advancedUser;
	Boolean					priceAdjust;					//	Not settable from the Palm
	Boolean					autoDetails;
	Boolean					isLargeFont;
	Boolean					screenLockOn;
	Boolean					classicMode;
	UserSoundLevelType	soundLevel;
	Boolean					getAllTables;
}UserSettingsType;



typedef struct 
{
	Char						displayName[4][MaxMenuItemSize];
	UInt16					numChoices;
} MenuOptionType;

typedef MenuOptionType *MenuOptionTypePtr;





typedef enum billingActionOptions					//	MmWriteThisBillingAction() action options
{
	BillingActionFindFirstInTable = 1,
	BillingActionFindFirstInDB,
	BillingActionSelectThis,
	BillingActionUnselectThis,
	BillingActionSelectTable,
	BillingActionCancelTable,
	BillingActionConfirmAll
}BillingActionOptions;



//	Temporary Registration data used during registration process
typedef struct 
{
	Boolean						autoReRegister;
	Boolean						registering;
	Boolean						regSuccessful;
	Boolean						forceInfoSync;
	Boolean						palmListChanged;
	UInt16						uniqueID;
	UInt16						flags;
	UInt16						syncPrintingToID;
	Char							pIN[MaxPINSize+1];
	Char							newName[PalmRegistrationNameLength+1];
	Char							syncPrintingToName[PalmRegistrationNameLength+1];
} RegTempType;

typedef RegTempType *PalmRegTypePtr;




//	Temporary Keypad data used for Keypad form
typedef struct 
{
//	Boolean						;
	UInt16						mode;
	Int32							value;
	Int32							maxValue;					//	Lamits max value that may be entered
	Char							frmTitleLabel[15 + 1];
	Char							cancelLabel[15 + 1];
} KeypadType;

typedef KeypadType *KeypadTypePtr;




typedef struct  				//	Values being currently processed
{
	UInt8							
									mMenu,
									memoRec;
	UInt16
									tableNo,
									seatNo,
									course,
									recMenuIndex,
									recIndex,
									setMenuGroup,
									dishesServingCourseDefault,
									drinksServingCourseDefault;
								
	Char							
									noteButtonLabel[MaxNoteLabelLength + 1];
	Char							
									servingButtonLabel[MaxNoteLabelLength + 1];
	UInt32						
									setMenuMask;
	FontID						
									fFontID;
	
} CurrentPrefsType;

typedef CurrentPrefsType *CurrentPrefsTypePtr;




// The data saved between application executions.  This data
// is saved as a single resource in the System Preferences.
typedef struct 
{
//	!!! Warning !!!
//	Changing anything in this structure prior to the Bluetooth entries
//	will require a Palm hard reset to enable upgrades. The Bluetooth subsystem
//	expects this information to be fixed in its current position.

	Char							verNumStr[VersionNumberStringLength + 1];				//	15
	Char							palmUniqueName[PalmUniqueNameLength + 1];				//	17
	UInt8							palmPrefSpare[PalmPrefSpareLength];						//	4
	UInt16						palmUniqueID;													//	2


	BtLibDeviceAddressType	btDevices[MaxBtTransceivers];
	Char							btNamesBuffer[MaxBtTransceivers][btLibMaxDeviceNameLength];
	BtLibFriendlyNameType	btName[MaxBtTransceivers];
	Char							btSelectedDeviceName[btLibMaxDeviceNameLength+3];
	BtLibDeviceAddressType	btPalmAddress;

/*
// Represents a 48-bit Bluetooth device address.
typedef struct BtLibDeviceAddressType
{
	UInt8		address[btLibDeviceAddressSize];
} BtLibDeviceAddressType, * BtLibDeviceAddressTypePtr;
 
// The BtLibFriendlyNameType is used to get/set the device friendly name
typedef struct BtLibFriendlyNameType
{
	UInt8	*name;		// An array containing the device's friendly name (null terminated)
	UInt8	nameLength;	// Size of the friendly name, including the null terminator, max is btLibMaxDeviceNameLength
}BtLibFriendlyNameType, *BtLibFriendlyNameTypePtr;
*/
	UInt8							btNumDevices;	//	Number of devices selected
	UInt8							btDevice;		//	Index into currently selected device

	UInt16						btTimeoutInterByte;
	UInt16						btTimeoutFirstByte;
	UInt16						btTimeoutSendOrders;
	UInt16						btTimeoutGetAllOrders;

	Char							palmRegistrationName[PalmRegistrationNameLength];	//	25


#ifdef CountOrdersSent
	UInt16
									ordersCount;
#endif

	Boolean						canScrollUp,
									sendLogEnabled,
									msgLogEnabled,
									rememberLastServCourse,
									isCashMode;
	
	UInt16
									staffIDNo,
									staffIDIndex,
									icon2Position,
									numUnsentOrders;	
	UInt8							
									transactionNo,
									menuSyncNo,
									drinkSyncNo,
									infoSyncNo,

									loggedIn,
									userLogOffMins,
									roundingEnabled,
									billingIsPaid;

	UInt32
									autoLogoutSecs,
									logoutTimeSecs;		//	To check for unsent orders
							
	Char							dishesMenuName[MaxMenuNameSize];
	Char							drinksMenuName[MaxMenuNameSize];
	Char							defaultSeatname[MaxSeatNameSize + 1];
	Char							pIN[MaxPINSize + 1];

	Char							verNumStrShort[VerNumStrShortLength];

	char							recentNotes[MaxRecentNotesListItems][MaxNoteLength];

	HLStatusType				hLStatus;

	CurrentOrderType			curOrder;

	CurrentPrefsType			current;

	UserSettingsType			userSettings;

//	UInt8							lastItem;
} MmPreferenceType;

typedef MmPreferenceType *MmPreferenceTypePtr;





// List of possible ScreenLock actions
enum screenLock
{
	disabled = 1,
	lockOnNilEvent,
	awaitingTimeSecs,							//	Await next AutoScreenLockxxxxTimeSecs timeout
	isActive,
	isActivePowerOff,
	unlockOnNilEvent
};

typedef struct
{
	UInt16				action;
	UInt32				timeSecs;			//	Trigger time. NoValue32 = Disable for now
	UInt32				unlockSecs;			//	Time between Button 1 and Button 2 presses
}ScreenLockType;




// List of possible sounds
typedef enum 
{
	noSound, 
	errorSound,
	thinkingSound,
	finishedSound
//	soundTypeCount
} SoundType;


typedef struct
{
	Int32				frequency1;
	UInt16			duration1;
	Int32				frequency2;
	UInt16			duration2;
	Int32				frequency3;
	UInt16			duration3;
} SoundInfo;


//	Data Link Layer packet format
typedef struct
{
	UInt8				startFlag;
	UInt16			slaveAddress;
	UInt8 			controlChar;
	UInt8				dataCount;
	UInt8*			dataPtr;
	UInt16			CRC16;
} DllDataPacket;

typedef DllDataPacket * DllPacketPtr;


/***********************************************************************
 * Global defines for this module
 **********************************************************************/
#define version35						0x03503000//0x03503000	// PalmOS 3.5 version number
#define MmVersionNum					100						// Version of the software

#define appFileCreator				'Mnu1'

// Application database management defines.
#define MmAppType						'Menu'					// Type for application.  must be 4 chars, mixed case.
#define MmDBType						'Ordr'					// Type for application database.  must be 4 chars, mixed case.

#define MemoAppType					sysFileCMemo			// 
#define MemoDBType					'DATA'					// 
#define MemoDBName					"MemoDB"					// 

#define startBTEvent					sysEventFirstUserEvent

//	Feature location assignments
//	NB Do not use location 1. This is the ROM version number! Others?
//#define FtrBtPortID				1


//#define MmEditDBName			"MmEditDB"				// Name for application database.  up to 31 characters.


#define NewMemoSize  				64			// Initial size for new Memo record.
#define MemoCategory  				1			// Category to assign Memo Logs

#define MmPrefID						0			// preferences resource ID.

#define NoValue8						0xFF
#define NoValue16						0xFFFF
#define NoValue32						0xFFFFFFFF

#define NoValue32Signed				0x80000000	//	Max negative value for 32 bits

#define SetMSB8						0x80
#define SetMSB16						0x8000
#define SetMSB32						0x80000000

#define ResetMSB8						0x7F
#define ResetMSB16					0x7FFF
#define ResetMSB32					0x7FFFFFFF

#define NewMenuSize  				64			// P6. initial size for new database record.

#define SetMenuGroupMinValue		0x8000	//	MSB set, because its just my temp number
#define SetMenuGroupMaxValue		0xFFFF	//	setMenuGroup should never reach this

#define UpdateCategoryChanged		0x01		// P11. Indicates a category changed
#define LocalizedAppInfoStr		1000		// P11. default category strings ** tAIS **

#define TitleReplacementChar		'#'		// P16. Character replaced in edit title
#define DigitsForRecordPosition	6			// P16. digits in title replacements
#define EditFontGroup				1			// P16. group number of font pushbuttons

#define MarkerItem					0			//	Category number for a course menu item

#define InitLoadSavedPrefs			0x1			//	Used for MmInitialiseAll()
#define InitResetAllPrefs			0x2
#define InitZapAllDatabases		0x4
#define InitZapAllDBsNotInfo		0x8
#define InitKeepRegistration		0x10
#define InitKeepBluetoothNames	0x20
#define InitForceBtDiscovery		0x40

#define RegReplacePalm				0x1		//	Used for Registration Flag
#define RegSyncPrinting				0x2
#define RegFlag3						0x4
#define RegFlag4						0x8

#define NotMarker						0			//	DeleteCurrentRecord() option
#define WithMarker					1			//	Delete, including Marker record
#define SetMenuAndSlaves			2			//	

#define CountOldOrders				0			//	MmMarkOrDeleteUnsentOrders() option
#define DeleteMarkedOrders			1
#define MarkUnsentOrders			2
#define UnMarkUnsentOrders			3
#define OldOrdersDone				-2

#define MaxProcessErrorValues		6

#define FindOnly						0			//	Used for MmFindCurrentRecordIndex()
#define NewDishCounter				1			//	Specify set itemCounter in CurrentOrder
#define SetPriceAdjust				2			//	

#define IsPriceAdj					0			//	Used for MmFormatPriceString()
#define IsPrice						1			//	
#define LeftAlign						2			//	

#define NoRecreateDB					0			//	Don't recreate the database after Zapping it
#define RecreateDB					1			//	Recreate the database after Zapping it

#define DontMarkForDelete			0			//	Used with Seatname records
#define MarkForDelete				1			//	Used with Seatname records
#define ReplaceSeatname				2			//	Used with Seatname records

#define DontMarkAsSelected			0			//	Used with Overview Billing records
#define MarkAsSelected				1			//	Used with Overview Billing records
#define MarkAsUnSelected			2			//	Used with Overview Billing records

#define DontCheckList				0			//	Used with MmItemToString()
#define CheckList						1			//	Used with MmItemToString()

#define StaffType						0			//	Record type for Info records
#define PalmsType						1			//	Record type for Info records
#define PaymentTypesType			2			//	Record type for Info records

#define CallAwayType					0			//	Record type for MiscDataType records
#define MessageType					1			//	Record type for MiscDataType records

#define ServingType					2			//	Record type for ServingCourseType records

#define IsSendOrders					0			//	Flag that operation is SendOrders
#define IsBilling						1			//	Flag that operation is Billing

#define DishesMenu					0			//	Display shows Dishes not Drinks
#define DrinksMenu					1			//	Display shows Drinks not Dishes

#define KeypadPriceAdjust			1			//	Define possibilities for Keypad use
#define KeypadCopyOrder				2


//	Mainview page display types
#define MainviewTablePage				0

//	Overview page display types.
//	These are defined at start of MmOverviewHandleEvent()
#define OverviewTables					0
#define OverviewPrintReceiptsPage	1
#define OverviewBeam						2
#define OverviewTakeOrders				3
#define OverviewServingCourses		4
#define OverviewUnsentPage				5
#define OverviewOrdersOnlyPage		6
#define OverviewClassicView			7			//	Go to Classic 'Courses' screen

#define MaxOverviewDisplayViews		8


//	Overview rowDisplayType display types.
#define RowIsBlank						0
#define RowIsSeat							1
#define RowIsServingCourse				2
#define RowIsNoServingCourse			3
#define RowIsOrder						4
#define RowIsOrderSide					5
#define RowIsSetMenuKey					6


#define IDForNoServingCourse			1			//	Hard-wired at POS

#define OverviewCheckboxesNone		0
#define OverviewCheckboxesBilling	1
#define OverviewCheckboxesUnsent		2


//	Main and Overview Table Columns
#define Column0							0			//	Course/Dish descriptor. Int16 holds recMenuIndex
#define Column1							1			//	Int16 holds recOrderIndex
#define Column2							2			//	Int16 holds status field
#define Column3							3			//	Int16 holds Billing group
#define Column4							4			//	Int16 holds Billing checkboxes
#define BillingColumn					Column4

//	Overview Screen Layout
#define	OverviewScreenHeight			160
#define	OverviewScreenWidth			160
#define	OverviewUsableScreenWidth	158//142

#define	OverviewCol0Width				12		//	Distance Items are spaced off left edge
#define	OverviewCol1Width				OverviewUsableScreenWidth - OverviewCol0Width
#define	OverviewBillingColWidth		15

#define	OverviewArrowIconWidth		24
#define	OverviewArrowIconHeight		12

#define	OverviewBillingColPos		OverviewUsableScreenWidth - OverviewBillingColWidth

#define	OverviewScreenTopBorder		15


//#define	RepeatOrderTicks				30



#define DisplayRowOffset				3
/*
#define	M238BASE						0xFFFFF000

#define	M238_UARTMISC				(M238BASE+0x908)
#define	M238_PORTJ					(M238BASE+0x438)

#define	UART_IRDA_ENABLE			0x0020
#define	UART_IRDA_DISABLE			0xFFDF
#define	UART_POLARITY_INVERTED	0x000C
#define	UART_POLARITY_NORMAL		0xFFF3
*/

//	Status is used for Sent, Confirmed as sent, and Note attached flags and also
//	to hold the itemCounter in the main table when multiple dishes are selected
#define FlagsLastFlag					0x80

#define StatusAsSent		 				0x0001	//	Order has been sent, but not confirmed
#define StatusAsUnsent					0xFFFE
#define StatusAsSentConfirmed			0x0002	//	Order has been sent and confirmed
#define StatusAsSentUnconfirmed		0xFFFD
#define StatusAsUnsentUnconfirmed	0xFFFC
#define StatusAsEnabled					0x0004	//	Identify orders as enabled/disabled
#define StatusAsDisabled				0xFFFB	//	
#define StatusHasLinkedItem			0x0008	//	Order has another order linked to it
#define StatusAsCreditDish				0x0010	//	Order is to be credited
#define StatusAsHasSeatnameToSend	0x0020	//	Seat has a Seatname to send
#define StatusAsMarkedForDelete		0x0040	//	Mark this unsent order for possible deletion
#define StatusAsSetMenu					0x0080	//	This order is a SetMenu item

//	Used with Orders Billing. Defines top three bits as flags
#define BillingSeatHasPaid				0x20		//	Seat has paid, will be deleted
#define BillingSeatMarked				0x40		//	The selection checkbox has been ticked
#define BillingSeatBilled				0x80		//	Seat has been assigned a groupNumber
#define BillingHideFlags				0x3F

//	Used with global 'BillingIsPaid'. Identifies whether billed seats are deleted or not
#define BillingIsPaidOff				0x00		//	BillingIsPaid is not enabled
#define BillingIsPaidCurrent			0x01		//	BillingIsPaid is Current for this Billing only
#define BillingIsPaidLatched			0x02		//	BillingIsPaid is Latched by Menu command


//	Transmission Message Flags preceed each dish item etc.
//	One flags is always sent per order SendOrders()
//	Two flags are always received per order GetAllOrders()

//	Transmission Message First Flag
#define FlagsCreditDish					0x01		//	Used for Transmission Message Flags
#define FlagsHasDishOrDrink			0x02
#define FlagsHasSeatname				0x04
#define FlagsHasItemLinks				0x08		//	Mixed drink (split), or linked dish (Side)
#define FlagsHasNote 					0x10
#define FlagsHasPriceAdj				0x20
#define FlagsHasOption 					0x40
#define FlagsHasSetMenuMask			0x80

//	Transmission Message Second Flag
#define FlagsNA1							0x01		//	Used for Transmission Message Flags
#define FlagsNA2							0x02
#define FlagsNA3							0x04
#define FlagsNA4 							0x08
#define FlagsNA5 							0x10
#define FlagsHasPriceCorrection		0x20		//	Dollar value from POS for non Palm items

//	Section Transmission Flags: Positioned at the start of a message, these identify
//	which optional sections within the message are included in this message
#define SectionBillingIsPaid			0x01		//	Used for Transmission Section Flags
#define SectionIsInHappyHour 			0x02
#define SectionHasMessage				0x04
#define SectionHasGetOrSendOrders 	0x08
#define SectionHasSeatname 			0x10
#define SectionReEnableAllDishes		0x20
#define SectionReEnableAllDrinks	 	0x40
#define SectionHasDisableItems	 	0x80


//	Drinks Class Type
#define ClassNA1							0x01			//	Used for Drinks Class Type definitions
#define ClassNA2							0x02
#define ClassNA3							0x04
#define ClassNA4 							0x08
#define ClassNA5 							0x10
#define ClassNA6							0x20
#define ClassSpeedDrink					0x40

//	Drinks related masks
#define IsDrinkMask						0x4000
#define IsDrinkMask8						0x40
#define IsNotDrinkMask					0xBFFF

//	


//	Flags for HLStatus flag cancel operations
#define KeepNone							0x0000
#define KeepRegister						0x0001
#define KeepInfoSync						0x0002
#define KeepMenuSync						0x0004
#define KeepDrinkSync					0x0008
#define KeepLogin							0x0010
#define KeepSendSeatnames				0x0020
#define KeepBtTimeout					0x0040
#define KeepSendOrders					0x0080
#define KeepCallAway						0x0100
#define KeepBillSeats					0x0200
#define KeepTransferTables				0x0400
#define KeepGetAllOrders				0x0800

//#define SendFail		 				0x0E
//#define SendSyncFail	 				0x0F
//#define SendOK			 				0xF0

#define ThisTablesOrders				 1
#define ThisSeatsOrders					 2
#define FindSort							-1


/***********************************************************************
 * Global variables for this module
 **********************************************************************/

#define MmOrdersDBName					"MmOrdersDB"			// Name for application database.  up to 31 characters.
#define MmInfoDBName						"MmInfoDB"
#define MmMiscDataDBName				"MmMiscDataDB"
#define MmDishesDBName					"MmDishesMenuDB"
#define MmDishesSizesDBName			"MmDishesSizesDB"
#define MmDishesOptionsDBName			"MmDishesOptionsDB"
#define MmDishesForcedSidesDBName	"MmDishesForcedSidesDB"
#define MmDrinksDBName					"MmDrinksMenuDB"
#define MmDrinksSizesDBName			"MmDrinksSizesDB"
#define MmDrinksOptionsDBName			"MmDrinksOptionsDB"
#define MmDrinksForcedSidesDBName	"MmDrinksForcedSidesDB"
#define MmServingCourseDBName			"MmServingCourseDB"
#define MmTablenameDBName				"MmTablenameDB"
#define MmSeatnameDBName				"MmSeatnameDB"
#define MmBtDevicesCacheName			"Bluetooth Device Cache"

//	Warning !!! These must be kept in sync with the typedef below!!!!!!!!!!!!
EXTERN DmOpenRef		MmOrdersDB,							// MemHandle for Orders database
							MmInfoDB,							// MemHandle for Info database
							MmMiscDataDB,						// MemHandle for Misc Data database
							MmDishesDB,							// MemHandle for Menu database
							MmDishesSizesDB,					// MemHandle for Size Names database
							MmDishesOptionsDB,				// MemHandle for Options database
							MmDishesForcedSidesDB,			// MemHandle for Forced Sides database
							MmDrinksDB,							// MemHandle for Menu database
							MmDrinksSizesDB,					// MemHandle for Size Names database
							MmDrinksOptionsDB,				// MemHandle for Options database
							MmDrinksForcedSidesDB,			// MemHandle for Forced Sides database
							MmServingCourseDB,				// MemHandle for Serving Course database
							MmTablenameDB,						// MemHandle for Table Names database
							MmSeatnameDB,						// MemHandle for Seat Names database
							MmBluetoothDevicesCacheDB;
//							MemoDB;


//	Warning !!! These must be kept in sync with the typedef below!!!!!!!!!!!!
typedef struct
{
	DmOpenRef			mmOrdersDB;
	DmOpenRef			mmInfoDB;
	DmOpenRef			mmMiscDataDB;
	DmOpenRef			mmDishesDB;
	DmOpenRef			mmDishesSizesDB;
	DmOpenRef			mmDishesOptionsDB;
	DmOpenRef			mmDishesForcedSidesDB;
	DmOpenRef			mmDrinksDB;
	DmOpenRef			mmDrinksSizesDB;
	DmOpenRef			mmDrinksOptionsDB;
	DmOpenRef			mmDrinksForcedSidesDB;
	DmOpenRef			mmServingCourseDB;
	DmOpenRef			mmTablenameDB;
	DmOpenRef			mmSeatnameDB;
//	DmOpenRef			memoDB;
} DBType;

typedef DBType *DBTypePtr;

EXTERN DBType
							DBs;

EXTERN UInt16			DBDeleteBitMask;					// Identify which databases have been deleted

//	Bit position multpliers for User Settings
typedef enum dBDeleteMaskBits
{
	MmOrdersMaskBit				= 0x0001,
	MmInfoMaskBit					= 0x0002,
	MmMiscDataMaskBit				= 0x0004,
	MmDishesMaskBit				= 0x0008,
	MmDishesSizesMaskBit			= 0x0010,
	MmDishesOptionsMaskBit		= 0x0020,
	MmDishesForcedSidesMaskBit	= 0x0040,
	MmDrinksMaskBit				= 0x0080,
	MmDrinksSizesMaskBit			= 0x0100,
	MmDrinksOptionsMaskBit		= 0x0200,
	MmDrinksForcedSidesMaskBit	= 0x0400,
	MmServingCourseMaskBit		= 0x0800,
	MmTablenameMaskBit			= 0x1000,
	MmSeatnameMaskBit				= 0x2000
}DBDeleteMaskBits;




EXTERN Char*			ListItems[MaxTables];			//	Array of pointers to list text strings
EXTERN Char*			SizeListItems[MaxListItems];
EXTERN Char*			ListOptionsItems[MaxOptionsItems];

EXTERN ListStructType
							ListItemsStruct[MaxTables];	//[MaxOptionsItems];

EXTERN UInt16			SizeListIndices[MaxListItems];//	Array of Indices into DB for Sizes



// P11. This is a default MmAppInfoType structure.  When a new database
// is created and a default app info block is created, the following structure
// is copied into the block initializing it.
static MmAppInfoType DefaultMmAppInfo = 
{
 	// Renamed categories
 	0x0006,			// set the 2nd and 3rd bits indicating those categories as changed.

	"",	//"Unfiled",
	"",	//"Business",
	"",	//"Personal",
	"",
	"",	"",	"",	"",
	"",	"",	"",	"",
	"",	"",	"",	"",
	
	// Category Uniq IDs
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 
	
	// Last Uniq ID
	15,

	// Reserved
	0, 0
};






EXTERN Boolean
							NewSerialManagerPresent,
							MainviewCanScrollDown,
							OverviewCanScrollDown,
							LeftArrowSelected,
							RightArrowSelected,
							InHappyHour,
//							ForceRegistration,
							ReDiscoverBt;			//	Run once bit may be set to force BtDiscovery							
EXTERN Int16
							NumOldUnsentOrders,
							MainviewTopRowNum,
							OverviewTopRowNum;

EXTERN UInt16
							AutoOffTime,

							OldCurrentTable,
							OldCurrentSeat,
							TableNoTemp,
							SeatNoTemp,
							GuestsTemp,

							SendIndex,

							StaffIDIndex,
							SidesIndexNum,			//	Count sides, for MmMainviewDrawRecord
							
							TotalPacketsSent,
							TotalPacketsReceived,
							
							ALPacketDataSize,
							ALReceiveIndex,
							
							LastItemOrdered,		//	Stores menu Index of latest order taken
							OverviewSeatNum,
							OverviewPageType,		//	Tables, Seats, or Receipt
							OverviewExpandedStatus,
							OverviewCheckboxType,
							OverviewServingCourse,
//							ServingCourseDefault,
							PreviousForm,			//	Used for PIN form
							
							RepeatOrderNum,
							
							RecentNotesThis,

							SerialPortID,			// Could be the ID using the new serial API or the old Ref
							BtPortId,
							BtLibRefNum,

							NumericalEntryItem,	//	Select which Numeric Item to set in Debug form
							GenTableOrders,		//	Debug Force: Number to generate
							GenSeatOrders,			//	Debug Force: Number to generate
							GenOrdersPerSeat,		//	Debug Force: Number to generate
							GenSeatnames;			//	Debug Force: Number to generate
							

EXTERN TableDisplayType
							TableDisplayRows[MaxTableDisplayRows];
							
							
EXTERN RegTempType
							RegTemp;

EXTERN KeypadType
							Keypad;

							
EXTERN UInt8			*ALReceivePtr;

EXTERN UInt8			ReceiveBuff[MyReceiveBuffSize];		//	Raw incoming serial
//EXTERN UInt8			DLReceiveBuff[MyDLReceiveBuffSize];	//	Assembled DL packet

EXTERN UInt8			SendBuff[MySendBuffSize + MySendBuffMargin];	//	Additional safety margin
//EXTERN UInt8			DLSendBuff[MyDLSendBuffSize];
							
//EXTERN UInt8			GroupsSelected[MaxGroupItems];
//EXTERN UInt8			GroupsSelectedIndex;

//EXTERN MemHandle		ALReceiveHandle;

EXTERN Char				TempStr[MaxNoteLength];	//	Available as a G.P. string for labels etc
EXTERN Char				ServerMessageFrom[MaxMessageFromServerLength];
EXTERN Char				ServerMessageIs[MaxMessageFromServerLength];

EXTERN UInt32
//							LastTimeNowSecs,
							AutoTablePageShortSecs,
							AutoTablePageLongSecs,
							UnsentOrdersCheckSecs,
							Hard2ReleaseTime,			//	Release Hard2 button after Beaming
							IconSelectedTime;			//	Time that repeat order bullet was selected



EXTERN
		ScreenLockType	ScreenLockStruct;

EXTERN
	HLStatusType		BeamMessagesDone;

EXTERN
		 UInt16			
		 					SoundLevel;					// default sound amplitude
static SoundType		SoundToMake;				// 
//SByte					SoundPeriodsRemaining;	// times to repeat the sound

/*	
	Frequency 	(Hz)
	Duration		(ms)
*/
static SoundInfo 		Sound[/*soundTypeCount*/] = 
	{
		{	0,		0,		0,		0,		0,		0		},	//	no sound
		{	2400,	100,	1200,	100,	2400,	100	},	//	errorSound
		{	800,	100,	1200,	100,	1600,	200	},	//	thinkingSound
		{	1600,	100,	1200,	100,	800,	200	}	//	finishedSound
	};
	

EXTERN MmPreferenceType Prefs;






/***********************************************************************
 * Prototypes for internal functions
 **********************************************************************/

//EXTERN void MmCopyToPrefs(void);
EXTERN void MmSavePrefs(void);
EXTERN Boolean MmInitialiseAll(UInt16 initialiseMode);
EXTERN void MmDoInitialiseCode(UInt16 *initialiseMode, Boolean isNewVersion, UInt16 *btLibRefNum,
														MmPreferenceTypePtr prefs, Boolean *reDiscoverBt,
														RegTempType* regTemp);

//EXTERN MemPtr MmMemHandleLock(MemHandle handle);
//EXTERN Err MmMemPtrUnlock(MemPtr ptr);

EXTERN void MmPlaySounds(SoundType SoundToMake);
EXTERN void MmRefreshAutoLogout(Boolean doRefresh, UInt16 thisUserLogOffMins);
EXTERN UInt32 MmProcessEventIncludingNilEvent(FormPtr frm, EventPtr event,
											UInt16 thisUserLogOffMins, UInt16 popTriggerID);
EXTERN Boolean MmItemToString(UInt16 itemSelected, Char numberStr[4],
											Boolean checkList, Char* *listItems);
EXTERN void MmGetPrice(UInt16 currentMenu);
EXTERN void MmShowTablePrice(FormPtr frm, UInt16 labelID, DmOpenRef mmOrdersDB,
										MmPreferenceTypePtr prefs);
EXTERN void MmShowSeatPrice(FormPtr frm, UInt16 labelID, DmOpenRef mmOrdersDB,
										MmPreferenceTypePtr prefs);
EXTERN void MmFormatPriceString(Int32 price, UInt16 isPrice, Char* priceStr,
																			UInt16 offset);
EXTERN void MmShowPrice(FormPtr frm, UInt16 labelID, Int32 passedPrice, UInt16 labelsize);
EXTERN Boolean MmKeypadHandleEvent(EventPtr event);
EXTERN Boolean MmHandleMenuEvent(FormPtr frm, EventPtr event);

EXTERN Boolean MmRegisterHandleEvent(EventPtr event);
EXTERN Boolean MmCallAwayHandleEvent(EventPtr event);
EXTERN Boolean MmUserSettingsHandleEvent(EventPtr event);
EXTERN Boolean MmNumberEntryFieldEvent(EventPtr event);

EXTERN Boolean MmEnterPINFormHandleEvent(EventPtr event);

EXTERN void MmDeleteAllBilledSeats(void);
EXTERN Boolean MmConfirmBilling(void);
EXTERN UInt16 MmFindFirstBillingRecord(void);
EXTERN void MmWriteThisBillingAction(UInt16 recIndex, UInt16 action, DmOpenRef ordersDB);
EXTERN UInt16 MmBillingOperations(UInt16 action, DmOpenRef ordersDB, 
																	CurrentPrefsTypePtr prefs_current);
EXTERN void MmMainScrollDown(FormPtr frm, MmPreferenceTypePtr prefs, 
										Int16* mainviewTopRowNum, Boolean mainviewCanScrollDown);
EXTERN void MmDetailsFormRedraw(FormPtr frm, DBTypePtr dBs, MmPreferenceTypePtr prefs);

EXTERN void MmMainScrollUp(FormPtr frm, MmPreferenceTypePtr prefs, 
										Int16* mainviewTopRowNum);

EXTERN void MmMainviewDrawRecordInBounds(Char* dishPtr, RectanglePtr bounds, UInt8 itemNumber, 
												UInt16 status, UInt32 servingCourseColor, 
													MmPreferenceTypePtr prefs);

//EXTERN void MmOverviewDrawCheckboxRecordInBounds(RectanglePtr bounds, UInt16 status,
//										UInt8 billingAction, UInt8 courseNumber,
//										MmPreferenceTypePtr prefs, UInt16 overviewPageType);

//EXTERN void MmOverviewDrawRecordInBounds(Boolean menuType, Char *dishPtr,
//											RectanglePtr bounds, UInt16 itemNumber, UInt16 status,
//											UInt16 servingCourse, MmPreferenceTypePtr prefs);

EXTERN Boolean MmGetServingCourseIndex(DmOpenRef mmServingCourseDB, UInt16 servingCourseID,
											UInt16* recServingCourseIndex, UInt32* color, Boolean* selectable);
EXTERN UInt16 MmGetServingCourseID(DmOpenRef DBName, UInt16 recServingCourseIndex);


EXTERN UInt16 MmGetFirstServingCourseForMenu(DmOpenRef DBName, UInt8 menuType, UInt16* servingCourseID, 
																UInt16* recServingCourseIndex, UInt32* color);

EXTERN void MmGetStaffID(UInt16 type, UInt16* ptrID, UInt16 recIndex, DmOpenRef mmInfoDB);
EXTERN void MmDisplayStaffNameLabel(FormPtr frm, DmOpenRef mmInfoDB, UInt16 index, 
												UInt16 popTriggerID, UInt16 overviewPageType, Boolean loggedIn);

//EXTERN Int16 MmCompareDishesUniqueID(MenuTypePtr r1, MenuTypePtr r2);
//EXTERN Int16 MmCompareDrinksUniqueID(MenuTypePtr r1, MenuTypePtr r2);
//EXTERN Int16 MmCompareCourseDish(MenuTypePtr r1, MenuTypePtr r2);
//EXTERN Int16 MmCompareTableSeat(OrderTypePtr r1, OrderTypePtr r2);
EXTERN Int16 MmCompareDB(OrderTypePtr r1, OrderTypePtr r2);
EXTERN UInt16 MmFindSortItemsUniqueID(DmOpenRef *DBName, UInt16 uniqueID);
//EXTERN UInt16 MmFindSortDrinksUniqueID(UInt16 uniqueID);
EXTERN UInt16 MmFindSortTableSeat(UInt16 tableNumber, UInt16 seatNumber);
EXTERN UInt16 MmFindSortTableSeatExt(DmOpenRef DBName, UInt16 tableNumber, UInt16 seatNumber);
EXTERN UInt16 MmFindSortSeatname(UInt16 tableNumber, UInt16 seatNumber);
EXTERN UInt16 MmFindSortCallAway(UInt16 table, UInt16 course);

EXTERN void MmCreateRecordTemplate(MmPreferenceTypePtr prefs);

EXTERN void MmGenerateMenuRecord(DmOpenRef *DBName, MenuTypePtr newRecordPtr, UInt16 recIndex);
EXTERN void MmGenerateSizeMenuRecord(DmOpenRef *DBName, SizeTypePtr newRecordPtr, UInt16 recNumber);
EXTERN void MmGenerateSeatOrTableRecord(DmOpenRef *DBName, SeatnameTypePtr newRecordPtr, UInt16 recNumber);
EXTERN void MmGenerateOptionsRecord(DmOpenRef *DBName, OptionsTypePtr newRecordPtr, UInt16 recNumber);
EXTERN void MmGenerateForcedSideRecord(DmOpenRef *DBName, ForcedSidesTypePtr newRecordPtr, UInt16 recIndex);
EXTERN void MmGenerateInfoRecord(DmOpenRef *DBName, InfoTypePtr newRecordPtr, UInt16 recNumber);
EXTERN void MmGenerateMiscDataRecord(DmOpenRef *DBName, MiscDataTypePtr newRecordPtr, UInt16 recIndex);
EXTERN void MmGenerateServingCourseRecord(DmOpenRef *DBName, ServingCourseTypePtr newRecordPtr, UInt16 recIndex);

EXTERN Boolean MmMarkAllUnconfirmedAsUnsent(void);

EXTERN UInt16 MmNumUnsentOrdersDB(DmOpenRef DBName);

EXTERN Boolean MmZapDB(DmOpenRef *DBName, Char* ptrName, Boolean recreate, 
									UInt16 *dBDeleteBitMask, UInt16 dBToDelete);
EXTERN void MmZapAllDBs(UInt16 initialiseMode);
EXTERN void MmZapServingCourseRecordsForMenuDB(DmOpenRef DBName, UInt16 menuType);

EXTERN Boolean MmDeleteCurrentRecord(DmOpenRef DBName, UInt16 mode, UInt16 currentRecordIndex,
									UInt16* numUnsentOrders, UInt16* numberRemoved);
EXTERN UInt16 MmMarkOrDeleteUnsentOrders(DmOpenRef mmOrdersDB, MmPreferenceTypePtr prefs, 
																										UInt16 deleteType);
EXTERN void MmToggleMarkedForDeleteOrder(DmOpenRef mmOrdersDB, UInt16 recIndex);
EXTERN Boolean MmWarnIfUnsentOrders(DmOpenRef DBName, MmPreferenceTypePtr prefs, 
													Boolean oldOrdersOnly, Int16 *ptrNumOldUnsentOrders); 
EXTERN Boolean MmDeleteSetMenuKeyAndOrders(DmOpenRef DBName, MmPreferenceTypePtr prefs);													

EXTERN UInt16 MmNukeTableSeatOrders(DmOpenRef *DBName, UInt16 tableOrSeat, UInt16 recIndex,
												UInt16 currentTableNumber, UInt16 currentSeatNumber,
													UInt16* ptrNumUnsentOrders);

EXTERN UInt16 MmDeleteAllTypeRecordsForMiscDataType(DmOpenRef DBName, UInt16 recordType);
EXTERN UInt16 MmDeleteAllServingCourseRecordsForMenu(DmOpenRef DBName, UInt16 menuType);



//EXTERN Boolean MmFindCurrentRecord(Boolean copyPriceAdj);
EXTERN Boolean MmFindCurrentRecordIndex(UInt16* recIndexPtr, Boolean newDishCounter,
																					Boolean setPriceAdjust);
EXTERN UInt16 MmFindDishOrDrinkIndex(DmOpenRef *DBName, UInt16 courseNumber, UInt16 itemNumber);
EXTERN UInt16 MmFindCourseOptionIndex(DmOpenRef *DBName, UInt16 courseNumber, UInt16 optionNumber);
EXTERN UInt16 MmFindOptionIndex(DmOpenRef optionsDB, UInt8 courseNumber, UInt8 optionNumber);
//EXTERN Boolean MmDeleteCurrentRecord(UInt16 withMarker, UInt16* numberRemoved);
EXTERN UInt16 MmCallAwayRecordExists(DmOpenRef *DBName);
EXTERN UInt16 MmDeleteAllServingCourseRecordsForMenu(DmOpenRef DBName, UInt16 menuType);
EXTERN MemPtr MmGetObjectPtr(Int16 objectID);
EXTERN void MmMainviewRedraw(FormPtr frm, UInt16 ScrollType, UInt16 redraw);
EXTERN void MmOverviewRedraw(FormPtr frm, UInt16 ScrollType, UInt16 redraw);
//EXTERN UInt16 MmGetTableNumber(UInt16 controlID);
//EXTERN void MmSetCurrentTableNumber (FormPtr frm, Char* numberStrTable);
EXTERN void MmDisplayPopNameLabel(FormPtr frm, DBTypePtr dBs, DmOpenRef DBName,
										UInt16 index, UInt16 labelID, Boolean recExists);
EXTERN UInt16 MmGetSeatNumber(UInt16 controlID);
EXTERN Boolean MmTableOrSeatHandleEvent(EventPtr event);
EXTERN FontID MmLstSetFont(ListType *listPtr, MmPreferenceTypePtr prefs);
EXTERN UInt16 MmLoadList(UInt16 listType, UInt16* offset);
EXTERN UInt16 MmPopupList(ListPtr listPtr, UInt16 listType, UInt16* offset, UInt16 preSelect);
EXTERN Boolean MmDetailsEditNote(FormPtr frm);
EXTERN void MmMainviewLoadTable(FormPtr frm);
EXTERN void MmLoadDisplayTableArray(FormPtr frm, UInt16 pageType);
EXTERN void MmCourseOptionsTrimList(DBTypePtr dBs, MmPreferenceTypePtr prefs, RectangleType bounds);
EXTERN Boolean MmDetailsStoreOptionsOrNote(UInt16 optionSelected, UInt8* notePtr);
EXTERN UInt16 MmDetailsReDrawOptionsList(FormPtr frm, ListPtr listPtr, UInt16 currentRecordIndex);
EXTERN void MmMainviewInit(void);
EXTERN void MmOverviewInit(void);
EXTERN void MmDisplayTableSeatLabel(FormPtr frm, UInt16 number, UInt16 labelID,
												MmPreferenceTypePtr prefs,
												DmOpenRef mmTablenameDB, DmOpenRef mmSeatnameDB);
EXTERN Boolean MmEnterSeatName(FormPtr frm, UInt16 *ptrRecSeatnameIndex, Boolean recExists,
												MmPreferenceTypePtr prefs, DmOpenRef mmSeatnameDB);
EXTERN Boolean MmGetTableNameIndex(DmOpenRef mmInfoDB, Char* numberStrPtr,
																	UInt16 *recIndex);
EXTERN Boolean MmGetSeatnameIndexAndUpdate(DBTypePtr dBs, UInt16 tableNumber, UInt16 seatNumber,
									UInt16 *recIndex, Char* seatName, Boolean markForDeletion);

EXTERN void MmSetDefaultServingCourse(MmPreferenceTypePtr prefs, 
										MenuTypePtr recMenuPtr, UInt8* previousCourse, 
											UInt16* servingCourseID, UInt16* servingCourseDefault, 
												UInt16* numServingCourses, Boolean* showServingCourses);

EXTERN Boolean MmAskUserForSize(DBTypePtr dBs, MmPreferenceTypePtr prefs, UInt16 sizeMode, 
										MenuTypePtr recMenuPtr, 
											ListPtr listSizePtr, UInt16* sizeListIndices, 
												UInt16* defaultSizeIndexPtr);
EXTERN UInt16 MmCountNumSizeTypes(MmPreferenceTypePtr prefs, UInt16 countMode, 
										MenuTypePtr recMenuPtr, UInt16 *defaultSizeIndex, 
											Boolean *containsSetMenuKey);

EXTERN UInt16 MmFindIndexForNextSide(CurrentOrderTypePtr currentOrderPtr);
EXTERN void MmSetCurrentOrderSetMenuMask(DmOpenRef *menuDB, UInt16 recMenuIndex,
							CurrentOrderTypePtr currentOrderPtr, UInt32* currentSetMenuMask);
EXTERN void MmLogAndDisplayError(Char* errorMessagePtr, Boolean reset);
EXTERN void MmLogDatabase(DmOpenRef *DBName);

EXTERN UInt16 DisplayPacketCounts(Int16 packetsToWrite, Int16 packetsToRead);
EXTERN UInt16 DisplayRcvdPacket(UInt16 rcvdPackets, DLPacketPtr ptrThisPacket);
EXTERN UInt16 DisplayPacketCountSent(UInt16 sentPackets, DLPacketPtr ptrThisPacket);
EXTERN UInt16 DisplayPacketCountRcvd(UInt16 rcvdPackets, DLPacketPtr ptrThisPacket);
EXTERN UInt16 MmDLCalcCRC(DLPacketPtr ptrThisPacket);
EXTERN void MmDLFillPacketHeader(DataLinkHeaderPtr DLHeaderPtr, UInt16 slave, UInt16 reply,
										UInt16 sequence, UInt16 dataLinkControl, UInt8 dataCount);
EXTERN UInt16 MmDLValidatePacket(DLPacketPtr ptrPacket, LinkControl packetType);
EXTERN void MmDLPacketiseSingle(DLPacketPtr ptrWritePacket, UInt8* ptrWriteMessage,
							UInt16 writeMessageLength, UInt16* ptrMessageIndex, UInt16 ALCommand);
EXTERN UInt16 MmDLSendPacket(UInt16 serialRefNum,  Boolean UseNewSerialManager, 
						UInt8* dLSendBuff,	DLPacketPtr ptrThisPacket, Boolean dLEscapingEnabled);
EXTERN UInt16 MmDLReceivePacket(UInt16 serialRefNum,  Boolean UseNewSerialManager,
					UInt8* dLReceiveBuff, DLPacketPtr ptrThisPacket, UInt16 timeout);
EXTERN Boolean MmDLSingleWrite(DLPacketPtr ptrThisPacket);
EXTERN UInt16 MmDLSingleRead(DLPacketPtr ptrThisPacket);

EXTERN void MmParseInfoSyncMessage(UInt16 menuMessageLength);
//EXTERN void MmParseMenuSyncMessage(UInt16 menuMessageLength, UInt16 menuType);
EXTERN Boolean MmParseGetAllOrdersMessage(UInt16 menuMessageLength);

EXTERN void MmFillMessageHeader(UInt16 hLCommandType, UInt16* sendIndex, UInt16 statusError);
EXTERN void MmHLCalcCRC(UInt16 messageStart, UInt16 dataCount, UInt16* sendIndex);
EXTERN UInt16 MmHLConfirmCRC(UInt16 dataCount, UInt16 messageIndex);

EXTERN void MmAssemblePalmUniqueIDMessage(UInt16* ptrSendIndex, UInt8* buffer,
																	UInt16* dataCount);
EXTERN void MmAssembleBillingMessage(UInt16* ptrSendIndex, UInt16* dataCount);
EXTERN UInt16 MmAssembleOrdersMessage(UInt16* ptrSendIndex, UInt16* dataCount,
																							Boolean isBilling);

EXTERN Boolean MmBeamOrdersWithOptionalCheckForUnsent(Int16 formId, MmPreferenceTypePtr prefs, 
														Boolean checkIfUnsentOrders, UInt32* unsentOrdersCheckSecs);

EXTERN UInt16 MmHLValidateMessage(MmPreferenceTypePtr prefs, UInt8 *aLReceivePtr, UInt16* messageType,
										UInt16 messageIndex, UInt16* messageLength, UInt8* ptrErrorCode);
EXTERN UInt16 MmProcessAllOrdersSentStatus(UInt16 mode, DmOpenRef ordersDB,
																			MmPreferenceType *prefs);
EXTERN void MmMarkSentAsConfirmed(void);
EXTERN void MmResetHLStatusToAllDone(MmPreferenceTypePtr prefs, UInt16 flags);
EXTERN UInt16 MmRedrawList(ListPtr listPtr, UInt16 listType, UInt16 listHeight,
											Char* *listItems, MmPreferenceTypePtr prefs);

EXTERN Boolean MmBeamingFormHandleEvent(EventPtr event);
EXTERN Boolean MmOverviewHandleEvent(EventPtr event);
//EXTERN void MmTblSetItemInt(UInt16 row, UInt16 column, UInt16 value);
EXTERN void MmOverviewLoadTable(FormPtr frm);
EXTERN void MmOverviewDisplayTablenameInTitle(FormPtr frm,
										MmPreferenceTypePtr prefs, UInt16 overviewPageType,
										DmOpenRef mmTablenameDB);

EXTERN void MmGenerateOrders(DBTypePtr dBs, MmPreferenceTypePtr prefs, Boolean random,	
											UInt16 *genTableOrders, 
											UInt16 *genSeatOrders, 
											UInt16 *genOrdersPerSeat, 
											UInt16 *genSeatnames, 
											UInt16 *lastItemOrdered		);
EXTERN void MmScreenLockFunctions(UInt16 controlID, FormPtr frm, UInt16 previousFormID,
						UInt32 timeNow, ScreenLockType *screenLockStruct);
EXTERN Boolean MmCreateMemoRecord(DmOpenRef *DBName, UInt16 recIndex);

EXTERN Boolean MmProgressCallback (PrgCallbackData *callbackData);
EXTERN Boolean MmDLConnect(UInt16 serialRefNum, Boolean UseNewSerialManager, 
						UInt8* dLSendBuff,	UInt8* dLReceiveBuff,Boolean *dLSequenceNo,
							Boolean *dLEscapingEnabled);
EXTERN Boolean MmDLDisconnect(UInt16 serialRefNum, Boolean UseNewSerialManager, 
						UInt8* dLSendBuff, UInt8* dLReceiveBuff, Boolean *dLSequenceNo,
							Boolean *dLEscapingEnabled);
EXTERN void MmCloseIrdaPort(UInt16 serialRefNum, Boolean UseNewSerialManager);
EXTERN Err MmCloseBtPort(UInt16* btPortId);

EXTERN Int32 MmRoundPrice(Int32 price);

EXTERN void MmShowNoteInButton(FormPtr frm, UInt16 labelID, Char *sourceLabel, 
												MmPreferenceTypePtr prefs);
EXTERN void MmShowServingCourse(FormPtr frm, UInt16 labelID, DBTypePtr dBs, UInt16 itemSelected,
												MmPreferenceTypePtr prefs);

EXTERN void MmCtlShowControl(ControlType * controlPtr);

EXTERN void MmDrawOverviewLines(Boolean overviewShowBilling);
EXTERN void MmDrawTableSeatBoxes(void);

EXTERN void MmMainviewLoadTableArrayLoop(DmOpenRef *menuDB, DmOpenRef *ordersDB,
									UInt16 overviewPageType, TableDisplayType *tableDisplayRowsPtr, 
									UInt16 currentMenu, UInt16 recOrderIndex,
									UInt16 currentTableNumber, UInt16 currentSeatNumber,
									UInt16 *lastItemOrdered, Int16 *mainviewTopRowNum, 
									UInt16 rowsInMainTable		);
								
														
EXTERN void MmOverviewLoadTableArrayLoop(DBTypePtr dBs, MmPreferenceTypePtr prefs, 
									UInt16 overviewPageType, UInt16 overviewServingCourse, 
									TableDisplayType *TableDisplayPtr, UInt16 overviewExpandedStatus, 
									UInt16 recOrderIndex, 
									Int16 *overviewTopRowNum, UInt16 *lastItemOrdered
																													);
														

UInt16 MmOverviewGetNextRecIndex(Boolean *firstIndexPass, Boolean thisServingCourseOnly, 
									Boolean* showServingCourse, 
									UInt16* servingCourseIndex, UInt16* servingCourseID, 
									ServingCourseIndexType** serveIndxsPtr,
									ServeDataTypePtr serveDataPtr, 
									UInt16 numRecords, UInt16 numServingCourseRecords, 
									Boolean* found);


EXTERN void MmFillServingCourseArray(DBTypePtr dBs, MmPreferenceTypePtr prefs, 
									UInt16 overviewPageType, UInt16* recOrderIndex, 
									UInt16 currentTableNumber, UInt16 currentSeatNumber,

									UInt16* servingCourse, UInt16* servingCourseID, 
									ServingCourseIndexTypePtr serve2DArrayPtr,
									ServingCourseIndexTypePtr*	serveIndxsPtr,
									ServeDataTypePtr	serveDataPtr, 
									PageViewTypePtr show,
									TableDisplayType *tableDisplayRowsPtr, 
									UInt16 overviewExpandedStatus );


EXTERN Boolean MmOverviewWriteSeatOrders(DBTypePtr dBs, MmPreferenceTypePtr prefs, Int16 overviewPageType, 
									Int16 *overviewTopRowNum, UInt16 *lastItemOrdered,
									TableDisplayType *tableDisplayRowsPtr, Int16 *row,
									UInt16* tableNumber, UInt16* nextTableNumber,
									UInt16* seatNumber, UInt16* nextSeatNumber,
									UInt16* recOrderIndex, UInt8 menuType, 
									Boolean* firstPass, Boolean* wasMarker, UInt16* servingCourseID, 
//									PageViewTypePtr show, BundleGroupType bundleGroup[BundleGroupsMax], 
									PageViewTypePtr show, BundleGroupTypePtr bundleGroup, 
									UInt16* bundleGroupIndex_J, UInt16* bundleGroupIndex_K
																													);

EXTERN void MmSetDisplayModeBooleanFlags(PageViewTypePtr show, UInt16 overviewPageType);


EXTERN UInt16 MmFindFirstSeatOrder(DBTypePtr dBs, CurrentPrefsTypePtr prefs_current, 
									Boolean* orderFound, UInt16* tableNo, UInt16* seatNo);


EXTERN void MmOverviewDrawRecord2(DBTypePtr dBs, UInt16 overviewTopRowNum, Int16 tableRow, Int16 column, 
									RectanglePtr bounds, 
									UInt16 overviewPageType, UInt16 overviewCheckboxes, UInt16 overviewExpandedStatus, 
									TableDisplayType *displayRowsPtr, 
									UInt16 *sidesIndexNum, Char* *listItems,
									MmPreferenceTypePtr prefs);

EXTERN void MmOverviewInit(void);

EXTERN void MmDrawRectangle(UInt16 frameType, UInt16 topLeftX, UInt16 topLeftY,
													UInt16 extentX, UInt16 extentY);

//EXTERN void MmShowPrice(FormPtr frm, UInt16 labelID, Int32 passedPrice);
EXTERN UInt32 MmAToH(char *string);

EXTERN void MmCopyCurrentOrderIndicesToRecord(DmOpenRef ordersDB, MmPreferenceTypePtr prefs);
EXTERN Boolean MmAddSideToRecord(DBTypePtr dBs, MmPreferenceTypePtr prefs, 
												UInt16 sizeDBIndex);

EXTERN UInt16 MmLoadSelectList(DBTypePtr dBs, MmPreferenceTypePtr prefs, UInt16 listType,
											UInt16* offset, Boolean* nextButton, UInt16* maxSelect,
												Char* *listItems, Char* *sizeListItems,
													ListStructTypePtr listItemsStruct,
														UInt16* sizeListIndices);

EXTERN void MmUInt16ToUserSettings(UInt16 userSettingsUInt16, MmPreferenceTypePtr prefs);

EXTERN UInt16 MmUserSettingsToUInt16(MmPreferenceTypePtr prefs);

EXTERN void MmSetUserSettingsPopLabel(FormPtr frm, UInt16 controlID, char* currentSetting);
EXTERN void MmSetUserSettings(FormPtr frm, MmPreferenceTypePtr prefs, Char* *listItems, 
								ListPtr listPtr, UInt16 controlID, 
									UInt16* soundLevel, ScreenLockType* screenLockStruct 	);
								
EXTERN Boolean MmGetForcedOptions(DBTypePtr dBs, MmPreferenceTypePtr prefs, 
										ListPtr listPtr, Char* *listItems, Char* *sizeListItems,
											ListStructTypePtr listItemsStruct, UInt16* sizeListIndices);

EXTERN Boolean MmGetForcedSides(DBTypePtr dBs, MmPreferenceTypePtr prefs, 
										ListPtr listPtr, Char* *listItems, Char* *sizeListItems,
											ListStructTypePtr listItemsStruct, UInt16* sizeListIndices);

EXTERN void ShowDebug(const Char *format, ...);
EXTERN void ShowMessage(const Char *format, ...);
EXTERN void ShowError(const Char *format, ...);



EXTERN Boolean CreateEditRecord(UInt16 index);
EXTERN FieldPtr GetFocusObjectPtr(void);
EXTERN void EditDoMenuCommand(UInt16 command);
EXTERN void EditSaveData(FieldPtr fld);
EXTERN void EditSaveData(FieldPtr fld);
EXTERN void EditRetrieveData(FieldPtr fld);
EXTERN void EditViewUpdateScrollBar();
EXTERN void EditRetrieveData(FieldPtr fld);
EXTERN void EditViewPageScroll (WinDirectionType direction);
EXTERN void EditViewScroll (Int16 linesToScroll);
EXTERN Boolean MmEditPageFormHandleEvent(EventPtr event);

EXTERN void MmPrvSyncLogTraceUI(Char* messagePtr, UInt16 intValue);
EXTERN void MmPrvSyncLogTrace(Char* messagePtr);

EXTERN UInt16 MmBtReceiveMessage(UInt16 btPortId, MmPreferenceTypePtr prefs, 
												UInt16 *palmUniqueID, UInt8 *aLReceivePtr);
EXTERN UInt16 MmBtTalk(UInt16 btPortId, MmPreferenceTypePtr prefs, UInt8 *ptrWriteMessage, 
										UInt16 writeMessageLength, UInt8 *aLReceivePtr);


//	DBStuff
EXTERN Boolean MmOpenOrCreateAllDBs(UInt16 initialiseMode);
EXTERN void MmInitialiseNewlyCreatedDB(DmOpenRef DBName);
EXTERN Err MmInitAppInfo(DmOpenRef dbP);


//	DBStuff2
EXTERN Boolean MmOpenOrCreateDB(DmOpenRef *DBName, Char* ptrName, DmOpenRef *dBsThisOne);

EXTERN Boolean MmOpenBluetoothCachedDeviceListDB(DmOpenRef *DBName, Char* ptrName);

EXTERN UInt16 MmCheckIfCurrentOrderSeatHasName(MmPreferenceTypePtr prefs, DmOpenRef seatnameDB);
EXTERN Boolean MmCreateFinalRecord(MmPreferenceTypePtr prefs, DBTypePtr dBs, 
																				UInt16* lastItemOrdered);
EXTERN Boolean MmCreateOrdersRecord(MmPreferenceTypePtr prefs, DBTypePtr dBs,
																				UInt16* lastItemOrdered);

EXTERN void MmCopyOrderToCurrentRecord(MmPreferenceTypePtr prefs, DBTypePtr dBs);

EXTERN void MmCreatNewDuplicateOrder(MmPreferenceTypePtr prefs, DBTypePtr dBs, UInt16* lastItemOrdered);

EXTERN Boolean MmCreateMarkerRecord(MmPreferenceTypePtr prefs, DmOpenRef ordersDB,
														UInt16* recIndexPtr, Int32 priceCorrection);
EXTERN Boolean MmCreateGetAllOrdersRecord(MmPreferenceTypePtr prefs, DmOpenRef ordersDB,
																					UInt16* recIndexPtr);



//	Ordering
EXTERN Boolean MmProcessOrder(FormPtr frm, TableDisplayType *displayRowsPtr, 
								DBTypePtr dBs, MmPreferenceTypePtr prefs,

									Char* *listItems, Char* *sizeListItems,
										ListStructTypePtr listItemsStruct, UInt16* sizeListIndices,

								UInt16 *lastItemOrdered,

									UInt16 *overviewPageType, UInt16 *overviewCheckboxes,
										Int16 *numOldUnsentOrders, 
											Boolean *screenRedrawn
																															);



//	Bluetooth
EXTERN Err BtOpenCloseLib(UInt16 *btLibRefNum, UInt16 action);
EXTERN Err BtOpenSerial(UInt16 *p_btPortId, BtLibDeviceAddressType device);
EXTERN Err BtCloseSerial(UInt16 *p_btPortId);
EXTERN Err BtSendSerial(UInt16 btPortId, const void *data, UInt16 dataLength, UInt32 *p_bytesSent);
EXTERN Err BtSendSerialStr(UInt16 btPortId, const Char *data, UInt16 length, UInt32 *p_bytesSent);
EXTERN Err BtReadSerial(UInt16 btPortId, void *buffer, UInt32 buffLen, UInt32 timeout, UInt32 *bytesRead);
EXTERN Err BtReadSerialBuff(UInt16 btPortId, void *buffer, UInt32 buffLen, UInt32 *bytesRead);
EXTERN Err BtDiscoverDevice(UInt16 *p_btPortId, BtLibDeviceAddressType *p_device);
EXTERN Err BtDiscoverMultipleDevices(UInt16 *btLibRefNum, UInt16 *p_btPortId, Boolean showLastList);
EXTERN Err BtGetSelectedDevices(UInt16 *btLibRefNum, BtLibDeviceAddressType *p_device,
													BtLibFriendlyNameType *p_name, UInt8 *p_btNumDevices);
EXTERN void BtSelectDevice(FormPtr frm, BtLibFriendlyNameType *p_name,
													Char *btSelectedDeviceName, UInt8 *prefs_btDevice);
EXTERN void MmDiscoverBtDevices(FormPtr frm, Boolean showLastList, UInt16 *btLibRefNum,
									UInt16 *btPortId, MmPreferenceTypePtr prefs, Boolean *reDiscoverBt);

EXTERN void LogPortErr(Err err);


EXTERN Boolean MmProcessCallAwayRecords(DBTypePtr dBs, MmPreferenceTypePtr prefs, 
																UInt16 recServingCourseIndex);


#endif	//	 __MENUMATE_H__

