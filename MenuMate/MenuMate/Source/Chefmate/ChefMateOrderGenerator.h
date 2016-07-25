//---------------------------------------------------------------------------

#ifndef ChefMateOrderGeneratorH
#define ChefMateOrderGeneratorH
//---------------------------------------------------------------------------

#include <vcl.h>
#include <vector>;
#include <map>;

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Order Handle: 1 = Succesful.
//               0 = Failed. For details you must call GetLastError()
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::
typedef __int32 HORDERDOC;

typedef __int32 HORDER;
typedef __int32 HSCOURSE;
typedef __int32 HCOURSE;
typedef __int32 HITEM;
typedef __int32 HOPTION;
typedef __int32 HSIDE;
typedef __int32 HPFORMAT;

typedef __int32 HPARENT;

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::

typedef __int32 COLOR_INT;

typedef __int32 OERROR;

const OERROR_NONE                         =  1;
const OERROR_SUCCESSFUL                   =  2;
const OERROR_CHEFMATE_SETTINGS_CHANGED    =  3;  // ChefMate's IP Address and IP Port have been changed
const OERROR_FAILED_CLOSING_GENERATOR     =  4;
const OERROR_FAILED_OPEN_COMPLETE_ORDER   =  5;
const OERROR_FAILED_OPEN_INCOMPLETE_ORDER =  6;
const OERROR_FAILED_OPEN_CANCEL_ORDER     =  7;
const OERROR_FAILED_OPEN_CREDIT_ORDER     =  8;
const OERROR_FAILED_OPEN_TRANSFER_ORDER   =  9;
const OERROR_FAILED_ADD_SERVING_COURSE    = 11;
const OERROR_FAILED_ADD_COURSE            = 12;
const OERROR_FAILED_ADD_ITEM              = 13;
const OERROR_FAILED_ADD_ITEMPFORMAT       = 14;
const OERROR_FAILED_ADD_ITEMOPTION        = 15;
const OERROR_FAILED_ADD_ITEMSIDE          = 16;
const OERROR_FAILED_ADD_SIDEOPTION        = 17;
const OERROR_FAILED_ADD_SIDEPFORMAT       = 18;
const OERROR_FAILED_COMMIT_ORDER          = 19;
const OERROR_FAILED_DELETE_ORDER          = 20;

const OERROR_FAILED_GENERATOR_NOT_ACTIVE    = 50;
const OERROR_FAILED_GENERATOR_NOT_AVAILABLE = 51;

const OERROR_FAILED_MANAGER_ORDER_ALREADY_OPEN      = 104;
const OERROR_FAILED_MANAGER_OPEN_COMPLETE_ORDER     = 105;
const OERROR_FAILED_MANAGER_OPEN_INCOMPLETE_ORDER   = 106;
const OERROR_FAILED_MANAGER_OPEN_CANCEL_ORDER       = 107;
const OERROR_FAILED_MANAGER_OPEN_CREDIT_ORDER       = 108;
const OERROR_FAILED_MANAGER_OPEN_TRANSFER_ORDER     = 109;
const OERROR_FAILED_MANAGER_ADD_SERVING_COURSE      = 110;
const OERROR_FAILED_MANAGER_ADD_COURSE              = 111;
const OERROR_FAILED_MANAGER_ADD_ITEM                = 111;
const OERROR_FAILED_MANAGER_ADD_ITEM_PFORMAT        = 113;
const OERROR_FAILED_MANAGER_ADD_ITEM_OPTION         = 114;
const OERROR_FAILED_MANAGER_ADD_ITEM_OPTION_PFORMAT = 115;
const OERROR_FAILED_MANAGER_ADD_SIDE                = 116;
const OERROR_FAILED_MANAGER_ADD_SIDE_PFORMAT        = 117;
const OERROR_FAILED_MANAGER_ADD_SIDE_OPTION         = 118;
const OERROR_FAILED_MANAGER_ADD_SIDE_OPTION_PFORMAT = 119;
const OERROR_FAILED_MANAGER_COMMIT_ORDER            = 120;
const OERROR_FAILED_MANAGER_DELETE_ORDER            = 121;
const OERROR_FAILED_MANAGER_UKNOWN_ORDER            = 122;
const OERROR_FAILED_MANAGER_OPEN_REPLACEMENT_ORDER  = 123;


const OERROR_FAILED_CLASS_OPEN_COMPLETE_ORDER     = 205;
const OERROR_FAILED_CLASS_OPEN_INCOMPLETE_ORDER   = 206;
const OERROR_FAILED_CLASS_OPEN_CANCEL_ORDER       = 207;
const OERROR_FAILED_CLASS_OPEN_CREDIT_ORDER       = 208;
const OERROR_FAILED_CLASS_OPEN_TRANSFER_ORDER     = 209;
const OERROR_FAILED_CLASS_ADD_SERVING_COURSE      = 210;
const OERROR_FAILED_CLASS_ADD_COURSE              = 211;
const OERROR_FAILED_CLASS_ADD_ITEM                = 212;
const OERROR_FAILED_CLASS_ADD_ITEM_PFORMAT        = 213;
const OERROR_FAILED_CLASS_ADD_ITEM_OPTION         = 214;
const OERROR_FAILED_CLASS_ADD_ITEM_OPTION_PFORMAT = 215;
const OERROR_FAILED_CLASS_ADD_ITEM_SIDE           = 216;
const OERROR_FAILED_CLASS_ADD_SIDE_PFORMAT        = 217;
const OERROR_FAILED_CLASS_ADD_SIDE_OPTION         = 218;
const OERROR_FAILED_CLASS_ADD_SIDE_OPTION_PFORMAT = 219;
const OERROR_FAILED_CLASS_COMMITTING_ORDER        = 220;
const OERROR_FAILED_CLASS_COMMIT_ORDER            = 221;
const OERROR_FAILED_CLASS_COMMIT_ORDER_INVALID_IPPORT          = 222;
const OERROR_FAILED_CLASS_COMMIT_ORDER_INVALID_RESPONSE_FORMAT = 223;

//---------------------------------------------------------------------------

typedef AnsiString TCMHostname; // Chefmate Hostname / IP address
typedef std::vector<AnsiString>                 HostnameLIST;
typedef std::vector<AnsiString>::const_iterator HostnameLIST_Iterator;

typedef char* OrderChar;

// Opens the Order Generator
typedef OERROR (*_OpenOrderGenerator)();

// Closes the Order Generator
typedef OERROR __stdcall (*_CloseOrderGenerator)();

// Update ChefMate's Settings
typedef OERROR __stdcall (*_ConfigureChafMate)(
							OrderChar, // IP Address,
							__int32 ); // IP Port

// Opens a Complete Order
// Order's components must be added afterwards
// Once all the components are added CommitOrder() must be called
typedef HORDERDOC __stdcall (*_OpenCompleteOrder)(
						__int32,     // DB Key,
						OrderChar,   // Server Name,
						__int32,     // Order Number,
						OrderChar,   // Chit Value,
						OrderChar,   // TableTab Name,
						OrderChar,   // Order Type,
						OrderChar,   // Customer Name
						OrderChar ); // Party Name

// Open an Incomplete Order
// Order's components must be added afterwards
// Once all the components CommitOrder() must be called
typedef HORDERDOC __stdcall (*_OpenIncompleteOrder)(
						__int32, // DB Key,
						OrderChar,   // Server Name,
						__int32, // Order Number,
						OrderChar,   // Chit Value,
						OrderChar,   // TableTab Name,
						OrderChar,   // OrderType,
						OrderChar,   // Customer Name
						OrderChar ); // Party Name

// Opens a Cancel Order
// The items to be canceled must be added afterwards
// Once all the items are added CommitOrder() must be called
typedef HORDERDOC __stdcall (*_OpenCancelOrder)(
						__int32,   // DB Key,
						__int32 ); // Order Number

// Opens a Credit Order
// Once all the items are added CommitOrder() must be called
typedef HORDERDOC __stdcall (*_OpenCreditOrder)();

// Opens a Transfer Order
// CommitOrder() must be called straight away
typedef HORDERDOC __stdcall (*_OpenTransferOrder)(
						__int32,      // DB Key
						__int32,      // Order Number
						OrderChar ); // TableTab Name )

// Adds a ServingCourse to an Order
typedef HSCOURSE __stdcall (*_AddServingCourse)(
						HORDERDOC, // OrderDocHandle
						__int32,   // DB Key
						OrderChar, // Name
						OrderChar ); // Kitchen Name

// Adds a Course to an Order
typedef HCOURSE __stdcall (*_AddCourse)(
						HORDERDOC, // OrderDocHandle
						__int32,   // DB Key
						OrderChar, // Name
						OrderChar, // Kitchen Name
						__int32 ); // Serving Course's Key

// Adds a Item to an Order
typedef HITEM __stdcall (*_AddItem)(
						HORDERDOC,  // OrderDocHandle
						__int32,    // DB Key
						__int32,    // Order's Item DB Key
						OrderChar,  // Name
						OrderChar,  // Kitchen Name
						__int32,    // Serving Course Key
						__int32 );  // Course Key

// Adds a Item's Print Format to an Order
typedef HPFORMAT __stdcall (*_AddItemPrintingFormat)(
						HORDERDOC,       // OrderDocHandle,
						HITEM,           // Item Handle,
						__int32,         // DB Key,
						unsigned char,	 // Underline,
						unsigned char,	 // Bold,
						COLOR_INT,	     // Color,
						unsigned char,	 // Large Font,
						unsigned char,	 // Double Width,
						unsigned char ); // Double Height

// Adds a Item's Option to an Order
typedef HOPTION __stdcall (*_AddItemOption)(
						HORDERDOC,       // OrderDocHandle
						HITEM,           // Item Handle
						__int32,         // DB Key
						OrderChar,           // Name
						OrderChar,           // Kitchen Name
						unsigned char ); // Is Plus

// Adds a Item Option's Print Format to an Order
typedef HPFORMAT __stdcall (*_AddItemOptionPrintingFormat)(
						HORDERDOC,        // OrderDocHandle,
						HOPTION,          // OptionHandle,
						__int32,          // DBKey,
						unsigned char,    // Underline,
						unsigned char,    // Bold,
						COLOR_INT,	      // Color,
						unsigned char,    // Large Font,
						unsigned char,    // Double Width,
						unsigned char );  // Double Height )

// Adds a Item's Side to an Order
typedef HSIDE __stdcall (*_AddItemSide)(
						HORDERDOC,    // OrderDocHandle,
						HITEM,        // ItemHandle,
						__int32,      // DBKey,
						OrderChar,   // Name,
						OrderChar ); // KitchenName

// Adds a Side's Print Format to an Order
typedef HPFORMAT __stdcall (*_AddSidePrintingFormat)(
						HORDERDOC,       // OrderDocHandle
						HSIDE,           // Side Handle
						__int32,         // DB Key
						unsigned char,   // Underline
						unsigned char,   // Bold
						COLOR_INT,       // Color
						unsigned char,   // Large Font
						unsigned char,   // Double Width
						unsigned char ); // Double Height

// Adds a Side's Option to an Order
typedef HOPTION __stdcall (*_AddSideOption)(
						HORDERDOC,       // OrderDocHandle
						HSIDE,           // Side Handle
						__int32,         // DB Key
						OrderChar,           // Name
						OrderChar,           // Kitchen Name
						unsigned char ); // IsPlus )

// Adds a Side Option's Print Format to an Order
typedef HPFORMAT __stdcall (*_AddSideOptionPrintingFormat)(
						HORDERDOC,       // OrderDocHandle,
						HOPTION,         // Option Handle,
						__int32,         // DB Key,
						unsigned char,   // Underline,
						unsigned char,   // Bold,
						COLOR_INT,       // Color,
						unsigned char,   // Large Font,
						unsigned char,   // Double Width,
						unsigned char ); // Double Height )

// Commit an Order
typedef OERROR __stdcall (*_CommitOrder)( ORDERDOC ); // OrderDocHandle

// Delete an Order
typedef OERROR __stdcall (*_DeleteOrder)( ORDERDOC ); // OrderDocHandle

// Returns the latest Error
typedef OERROR __stdcall (*_GetLastOError)();

//::::::::::::::::::::::::::::::::::::::::::::::::::::::


#endif


