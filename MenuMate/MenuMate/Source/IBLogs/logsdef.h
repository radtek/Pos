//---------------------------------------------------------------------------
#ifndef LogsDefH
#define LogsDefH

#define Err_Panic       "Panic"
#define Err_Error       "Error"
#define Err_Warning     "Warning"
#define Err_Standard    "Standard"
#define Err_Info        "Info"

#define EXCEPTIONLOG 	"Exception"
#define ERRORLOG     	"Error"
#define DEBUGLOG     	"Debug"
#define MSGLOG       	"Message"
#define DLLNZALOG    	"NZA DLL"
#define DLLSTOCKLOG		"Stock DLL"
#define EFTPOSLOG  		"EFTPOS"
#define REGISTRATIONLOG "Registration"
#define UNUSEDLOG    	"Unused"
#define PALMBTLAYERLOG   	"BT Layer"
#define PALMPACKETLAYERLOG "Packet Layer"
#define PALMDECODEPACKETLOG "Decode Layer"
#define PHOENIXINTERFACELOG "Phoenix Interface"
#define INTAMATEINTERFACELOG "IntaMate Interface"
#define EBETINTERFACELOG "Ebet Interface"
#define XMLINTERFACELOG			"XML Interface"
#define SMARTCARDLOG "SmartCard Interface"
#define UPDATELOG "Updates"
#define VERSIONLOG "Version"
#define WEBMATELOG "WebMate"
#define MEMBERSHIPINTERFACELOG "External Membership"


enum EProcessingErrorType {   Err_ErrorMsg = 1,
                              Err_Msg_CRC,
                              Err_Currupt_Message,
                              Err_Mismatched_Message_Data,
                              Err_InfoOutOfSync,
                              Err_MenuOutOfSync,
                              Err_DrinkOutOfSync,
                              Err_Database,
                              Err_Not_Logged_In,
                              Err_PIN_Incorrect,
                              Err_User_Not_Found,
                              Err_User_OverFlow,
                              Err_DeviceKey_Not_Found,
                              Err_Reg_Password_Wrong,
										Err_Not_In_Reg_Mode,
                              Err_No_Printer_Found,
										Err_Incorrect_Version,
                              Err_Destructor,
                              Wrn_NonFatalWarnings = 128,
                              Wrn_WarningMsg,
                              Wrn_No_Menu_Found,
                              Wrn_Printing_Redirected,
                              Wrn_Printer_Offline,
                              Wrn_Duplicate_Order,
										Wrn_Callaway_Printer_Not_Setup,
										Wrn_Kitchen_Printer_Not_Setup, // Not yet implemented (v4.0.0.50)
										PalmToServerErrors = 200,
										proA_Ok = 255,
										MaxErrors = 1000};

#endif
