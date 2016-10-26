#ifndef ENUMH
#define ENUMH

#include <values.h>
#include <System.hpp>


#ifdef  _DEBUG
#define DEBUGLEVEL 0xFF
#else
#define DEBUGLEVEL 0x00
#endif

#define debugExpandPalmComms 0x01
#define debugExpandSerialComms 0x02
#define debugExpandLowLevelSerialComms 0x04
#define debugExpandGetOrdes 0x08
#define debugExpandOrders 0x10

#define EFT_OCX_TIMEOUT 120

#define DESIGN_SCREEN_SIZE 1024

#ifdef  IR_Palm
#define PIN_Length 4
#endif

#define Max_Course_Records 14
#define Max_Class_Records 14
#define Max_Class_Glasses 5
#define Max_Dish_Records 31
#define Max_Dish_Options 15
#define Max_Course_Length 25
#define Max_Dish_Length 25
#define Max_Table_Name_Length 10
#define Max_Price 650
#define Max_User_Initials_Length 3

#define wIsLast   0x8000
#define cIsLast   0x80           // Word Defines.
#ifdef  PalmMate
   #define wIsDrink  0x4000         // Word Defines.
#endif
#define cIsSplit  0x08           // Word Defines.

// PALM Flags
// 1 byte  Palm Sections Flag	  	//	Bit 7 = Last flags byte
                                 //	Bit 6 = Enable All Dishes/Drinks (Not used By Palm)
											//	Bit 5 = Disabled Dishes/Drinks (Not used By Palm)
                                 //	Bit 4 = Has Seat Name (Not used by Palm at this level)
                                 // Bit 3 = Contains Orders to Get.
                                 // Bit 2 = Has Message (Not used by Palm)
                                 // Bit 1 = Billing Data
											// Bit 0 = NA

#define PalmSectionContainsOrders            0x08 // Contains Orders.
#define PalmSectionIsInHappyHour		         0x02 // Contains Billing.
#define PalmSectionFinalBill                 0x01 // Finial Bill.
// Repackage Orders Section Flags
// 1 byte	Sections Flag	     	//	Bit 7 = Last flags byte
                                 //	Bit 6 = Enable All Dishes/Drinks // No Data
                                 //	Bit 5 = Disabled Dishes/Drinks
                                 //	Bit 4 = Seat Names
                                 // Bit 3 = GetOrders/SendOrders
                                 // Bit 2 = Message
                                 // Bit 1 = NA
                                 // Bit 0 = NA


// Receive Orders & Repackage Orders.
#define ROSectionFlagHasDisabledItems        0x80 // Repackage Orders (Not used By Palm)
#define ROSectionFlagEnableDisableNoChange   0x9F // Repackage Orders (Not used By Palm)
#define ROSectionFlagEnableAllDrinks         0x40 // Repackage Orders (Not used By Palm)
#define ROSectionFlagEnableAllDishes         0x20 // Repackage Orders (Not used By Palm)
#define ROSectionFlagContainsSeatName        0x10 // Get.
#define ROSectionFlagContainsOrders          0x08 // Get.
#define ROSectionFlagContainsMessage         0x04 // Get.

#define ROCreditItem            0x01 // Get. (Not currently used)
#define ROContainsOrders        0x02 // Set.
#define ROContainsSeatName      0x04 // Set.
#define ROSubOrdersMix          0x08 // Set.
#define ROContainsNote          0x10 // Set.
#define ROContainsPriceAdjust   0x20 // Set. // No longer Used/
#define ROContainsOptions       0x40 // Set.
#define ROContainsPromoMask     0x80 // Set.
// 1 byte	1st Dish Flag 	     	//	Bit 7 = A Promot mask follows.
                                 //	Bit 6 = Has Option.
                                 //	Bit 5 = Has Price Adjust.
                                 //	Bit 4 = Has Note
                                 // Bit 3 = Is Drink Mix
                                 // Bit 2 = Has SeatName
                                 // Bit 1 = Dish/Drink
                                 // Bit 0 = Credit Dish

#define RO2ContainsSplitGroupNumberAndPrice  0x40 // Get (Second Flag).
#define RO2ContainsMiscPricing              0x20 // Get (Second Flag). Always Used

// 1 byte	2nd Dish Flag 	    //	Bit 7 = Last Flag.
                                 //	Bit 6 = It Split (Group number and Price to follow))
											//	Bit 5 = Contains pricing for courses that arnt
                                 //            sent to the palm so the plam total
											//            will be correct.
                                 //	Bit 4 = NA
                                 // Bit 3 = NA
                                 // Bit 2 = NA
                                 // Bit 1 = NA
											// Bit 0 = NA




// Printer.
#define OffLineFlag   0x08
#define MediaOut   0x04
#define CashDrawStatus   0x01

#define Max_Table_Number 99

#define SEAT_IMAGE_INDEX		0
#define SEAT_SEL_IMAGE_INDEX	1
#define SEAT_MT_IMAGE_INDEX	2
#define QICKPICK_DOWN_IMAGE	3
#define QICKPICK_UP_IMAGE		4

#define TABLE_SELECTED_COLOUR	clRed
#define TABLE_TEMP_COLOUR	clBlue
#define TABLE_UNSELECTED_COLOUR clBlack

#define IMAGE_INDEX_SELECT 0
#define IMAGE_INDEX_MENU 1
#define IMAGE_INDEX_OPTIONS 2
#define IMAGE_INDEX_OPTION 2
#define IMAGE_INDEX_DISH 3
#define IMAGE_INDEX_COURSE 4
#define IMAGE_INDEX_DRINK 5
#define IMAGE_INDEX_CLASS 5
#define IMAGE_INDEX_TABLE 6
#define IMAGE_INDEX_SEAT 7
#define IMAGE_INDEX_TOTAL 9
#define IMAGE_INDEX_MT_SEAT 10
#define IMAGE_INDEX_DISFOOD 11


#define Tree_Level_Menu 0
#define Tree_Level_Course_Bevs 1
#define Tree_Level_Dish_Opts_Bev 2
#define Tree_Level_Option 3

#define SEAT_HEIGHT 15
#define SEAT_WIDTH  15
#define TABLE_FONT_SIZE 8

//---------------------------------------------------------------------------

#define MSG_NEW_MESSAGE       (WM_USER + 400) // This Message is Used for New Messages from Palm.
#define MSG_POSTION_INFO      (WM_USER + 401) // This Message is Used for Debug Messages.
#define MSG_ERR_MESSAGE       (WM_USER + 402) // This Message is Used for Error Messages.
#define MSG_PRINTER_STATUS    (WM_USER + 403) // This Message is Used for updating the printer status.

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Error handling.
// Call to New failed in table construction.
#define ENum_TableCreateFailed 0001
#define EscapeCharaterEnableFlag 0x01
#define EscapeCharaterDisableFlag 0x00

#define MaxPacketSize 32

#define CommandPktSize 1

// The number of times to ping a module that is supposed to have a Palm on the end of it.
#define PALMECHO 20


#define Menu_Popup_Location_X 560
#define Menu_Popup_Location_Y 75


#define A715CPL 44 // Characters Per Line.
#define A714CPL 44 // Characters Per Line.




#define Message_Packing 0x20




#define Adjust "Adjustment"

/* NOT YET USED
#define VISA   "Visa"
#define BANKCD "BankCard"
#define MASTER "MasterCard"
*/




//---------------------------------- FILE INFO ---------------------------------
//GOLBAL
#define LOGFILE      ".log"
#define CSVFILE      ".csv"
// Debug
#define PALMDISHMENU 	"Palm Dish Menu"
#define PALMBEVMENU  	"Palm Bev Menu"
#define NZASTOCK     	"Stock"
#define LOWLEVELSERIAL	"LowLevelSerial"


// Staff
#define RECEIPT_HEADER  "Receipt.hdr"
#define RECEIPT_PRELIM_HEADER  "ReceiptP.hdr"
#define RECEIPT_FOOTER "Receipt.ftr"
#define RECEIPT_ZEDHEADER "Zed.hdr"
#define RECEIPT_VOID_FOOTER "Void.hdr"
#define RECEIPT_SUBHEADER "SubHeader.hdr"
// Paths
#define ZDIR   "ZDir"
#define LOGDIR "Logs"
// System Accounts
#define CASH_ACCOUNT " Sale"


#ifdef MenuMate
   #define MMBaseKey "\\Software\\IQWorks\\MenuMate\\"
#endif

#ifdef ImptMate
	#define MMBaseKey "\\Software\\IQWorks\\MenuMate\\"
#endif

#ifdef PalmMate
   #define MMBaseKey "\\Software\\IQWorks\\MenuMate\\PalmMate\\"
   #define MenuMateKey "\\Software\\IQWorks\\MenuMate\\"
#endif

#ifdef ChefMate
	#define  MMBaseKey "\\Software\\IQWorks\\ChefMate\\"
#endif

#ifdef Kiosk
   #define MMBaseKey "\\Software\\IQWorks\\MenuMate\\Kiosk\\"
#endif


#define mmSales	vPOS        +   \
    vPINChange              +   \
    vAccountCreation        +   \
    vPaymentAccess          +   \
    vReprintReceipt         +   \
    vAllowReprintOrders

#define mmPowerUser	vPOS    +   \
    vDisableFoodDrink       +   \
    vCredit                 +   \
    vPriceAj                +   \
    vDiscountBill           +   \
    vPrintZeds              +   \
    vMaintenance            +   \
    vMenuEditor             +   \
    vQuit                   +   \
    vPINChange              +   \
    vAdjustFloat            +   \
    vSetFloat               +   \
    vOpenDrawer             +   \
    vAccountCreation        +   \
    vWriteOff               +   \
    vReprintReceipt         +   \
    vPaymentAccess          +   \
    vAllowReprintOrders     +   \
    vAllowForcedHappyHour

#define mmAdmin	vPOS        +   \
    vDisableFoodDrink       +   \
    vViewMembersDetials     +   \
    vCredit                 +   \
    vPriceAj                +   \
    vDiscountBill           +	\
    vPrintZeds              +   \
    vMaintenance            +   \
    vMenuEditor             +   \
    vUsers                  +   \
    vPrinters               +   \
    vSetup                  +   \
    vQuit                   +   \
    vPINChange              +   \
    vPaymentTypes           +   \
    vAdjustFloat            +   \
    vSetFloat               +   \
    vTabPINOverride         +   \
    vAccountManager         +   \
    vOpenDrawer             +   \
    vChargeToInvoice        +   \
    vPayInvoice             +   \
    vAccountCreation        +   \
    vWriteOff               +   \
    vReprintReceipt         +   \
    vPaymentAccess          +   \
    vTransactionAudit       +   \
    vAllowReprintOrders     +   \
    vAllowForcedHappyHour

#define mmWaiter  vPOS      +   \
    vPINChange              +   \
    vAccountCreation        +   \
    vReprintReceipt         +   \
    vAllowReprintOrders


#define MSGEVENT_MENUINSERT         (WM_APP + 451)
#define MSGEVENT_MENUDELETE         (WM_APP + 452)
#define UWM_CARDSWIPE               (WM_APP + 453)
#define UWM_INACTIVE				(WM_APP + 454)
#define UWM_SHOWZONESELECT          (WM_APP + 455)
#define SMARTCARD_CARDSTATUSCHANGED (WM_APP + 456)
#define WEIGHSCALES_STATUSCHANGED   (WM_APP + 457)
//#define WEBORDER_STATUSCHANGED (WM_APP + 458) See WebMate.h
#define LOYALTYCLOUD_MEMBER_OPERATION_SUCCESS (WM_APP + 459)

#define MENU_IMPORT_VERSION "6.00"
//#define MENU_IMPORT_VERSION "5.00"
//#define MENU_IMPORT_VERSION "Version 4.90"


enum TLoginSuccess {lsAccepted, lsDenied, lsPINIncorrect, lsCancel, lsUserNotFound, lsAccountBlocked};

enum TOrderMode {omRestaurant, omBar, omUnknown };

enum ElectronicTransType {TransNONE,TransEFTPOS,TransCREDITCARD,TransAMEX,TransDINERS};

enum eLocation {LocNone=0,LocBar=1,LocRestaurant=2,LocBottleStore=4,LocPalm=8};



//enum DownloadStatus {ThisCurrentTable,ActiveTables,Full};


#ifdef IR_Palm
enum MESSAGE_TYPE{NA = 0x00,
                  KEEPALIVE = 0xE0,
                  REGISTRATION,
                  INFO_SYNC,
                  MENU_SYNC,
						DRINK_SYNC,
                  LOGIN,
                  ORDERS,
                  CALL_AWAY,
                  REQUEST_RECEIPT,
                  TRANSFER_TABLE_ORDERS,
                  RETRIEVE_ORDERS};
#else
enum MESSAGE_TYPE{NA = 0x00,
                  REGISTRATION = 0xE0,
                  INFO_SYNC,
						MENU_SYNC,
                  DRINK_SYNC,
						LOGIN,
						SEAT_NAMES,
						KEEPALIVE,
						ORDERS,
						CALL_AWAY,
						REQUEST_RECEIPT,
                  TRANSFER_TABLE_ORDERS,
                  RETRIEVE_ORDERS};
#endif


enum eDisplayMode {eNoDisplayMode, eTabs,eTables,eRooms, eInvoices};
typedef int TMMDisplayMode;

enum eVisibleInterface {eMemKeypad, eMemNumPad};

enum ePaymentType { ptPreliminary,ptPartial,ptFinal};


enum eUserOptions{eNone,eClose,eCloseandPrint,eBack};

// VACANT CLEAN,VACANT DIRTY,STAY OVER, ARRIVING, DEPARTING,MAINTANCE.
enum eRoomStatus {eVacantClean,eVacantDirty,eStayover,eArriving,eDeparting,eMaintance};

enum emPoleDisplyEmulation {ePoleDispNone,ePoleDispCD5220,ePoleDispEscPOS,
									 ePoleDispDSPUTC,ePoleDispDSPEMaxAdex,ePoleDispToshiba};


enum PC_COMMANDS {SLAVE_TYPE = 0, SOFTWARE_VERSION, HARDWARE_VERSION,SERIAL_NUMBER,SLAVE_ADDRESS,INTERFACE_VERSION,
						GREEN_LIGHT = 10,PC_TRX_REQ = 50, PC_TRX_DATA = 51, PC_TRX_NO_DATA = 52,PC_DISC_REQ = 53, PC_TRX_ACK = 54,
                  PRN_DATA = 200, PRN_GET_STATUS = 201}; // PRN_SET_DATA = 200 , PRN_GET_DATA = 200.

enum TablePos  {FirstTable, Table, LastTable};
enum SeatPos   {FirstSeat, Seat, LastSeat};
enum CoursePos {FirstCourse, Course, LastCourse};
enum DishPos   {FirstDish, Dish, LastDish};
enum OptionPos {FirstOprion, Option, LastOption};


const enum NodeTypes 	{	NO_NODE, MENU_NODE, COURSE_NODE, DISH_NODE, OPTIONS_NODE, OPTION_NODE,
									PRINTER_NODE, PRINTERGUID_NODE, PRINTMENU_NODE, PRINTCOURSE_NODE, PRINTCLASS_NODE, PRINTBEV_NODE,
                           TABLE_NODE, SEAT_NODE, NOTE_NODE,
                           CLASSES_NODE, CLASS_NODE, GLASSES_NODE, GLASS_NODE, DRINK_NODE, DRINKCOST_NODE, DRINKOPTIONS_NODE, DRINKOPTION_NODE};

const MAX_OPTIONS = 127;
const MAX_DISHES = MAXSHORT / 4 - 1;
const MAX_COURSES = 14;
const MAX_GLASSES = 5;

const MAX_DESCRIPTION_LENGTH	= 25;

const MENU_PIC 			= 0;
const CLOSECOURSE_PIC 	= 1;
const OPENCOURSE_PIC 	= 2;
const DISH_PIC 			= 3;
const CLOSEOPTION_PIC 	= 4;
const OPENOPTION_PIC 	= 5;
const OPTION_PIC 			= 6;

const PRINTER_PIC			= 7;
const CLASS_PIC			= 4;
const BOTTLE_PIC			= 8;
const GLASS_PIC			= 9;

const DISABLED_PIC		= 10;

const SEAT_PIC				= 11;
const TABLE_PIC			= 12;
const NOTE_PIC				= 0;
const SAVE_PIC				= 13;
const DOLLARS_PIC			= 14;
const CREDIT_PIC      	= 15;
const PALM_PIC         = 16;
const PC_PIC           = 17;

const NIP_PIC        	= 2;
const SPLASH_PIC        = 3;
const DISABLEDSTATE_PIC = 4;

#endif
