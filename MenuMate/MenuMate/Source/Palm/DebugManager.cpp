//---------------------------------------------------------------------------


#pragma hdrstop

#include "DebugManager.h"


//---------------------------------------------------------------------------

#pragma package(smart_init)

TManagerDebug *ManagerDebug;

TManagerDebug::TManagerDebug()
{
	//TODO: Add your source code here
}

__fastcall TManagerDebug::~TManagerDebug()
{
	BugsClear();
}

EProcessingErrorType TManagerDebug::GetErrorNumber(MESSAGE_TYPE inMessageType)
{
	EProcessingErrorType RetVal = proA_Ok;

	TDebugHelp *ItemBugs = BugsGet(inMessageType);
	if(ItemBugs != NULL)
	{
		ItemBugs->MessageCount++;
		if(ItemBugs->MessageCount >= ItemBugs->TriggerCount)
		{
			ItemBugs->MessageCount = 0;
			RetVal = ItemBugs->Action;
		}
	}
	return RetVal;
}

void __fastcall TManagerDebug::SetEnabled(bool value)
{
   if (fEnabled != value)
   {
      if(value)
      {
         fEnabled = value;
      }
      else
      {
         fEnabled = false;
      }
   }
}

bool __fastcall TManagerDebug::GetEnabled()
{
   return fEnabled;
}

void TManagerDebug::BugsClear()
{
   while(Count > 0)
   {
      BugsDelete(BugsGet(0));
   }
}

TDebugHelp *TManagerDebug::BugsGet(int Index)
{
   if(Count > Index)
   {
      return (TDebugHelp *)Items[Index];
   }
   return NULL;
}

TDebugHelp *TManagerDebug::BugsGet(MESSAGE_TYPE inMessageType)
{
	for(int i = 0; i < Count; i++ )
   {
		TDebugHelp *ItemBugs = (TDebugHelp *)Items[i];
		if(ItemBugs->MessageType == inMessageType)
      {
         return ItemBugs;
      }
   }
   return NULL;
}

void TManagerDebug::BugsGet(TStrings *List)
{
	List->Clear();
	for(int i = 0; i < Count; i++ )
	{
		TDebugHelp *ItemBugs = (TDebugHelp *)Items[i];
		List->AddObject(MessageTypeToStr(ItemBugs->MessageType) +
							 " Count :" + IntToStr(ItemBugs->MessageCount) +
							 "\\" + IntToStr(ItemBugs->TriggerCount) +
							 "Error :" + ErrorTypeToStr(ItemBugs->Action)
							 ,ItemBugs);
	}
}

int TManagerDebug::BugsAdd(TDebugHelp *in)
{
	return Add(in);
}

void TManagerDebug::BugsDelete(TDebugHelp *in)
{
	if(in != NULL)
	{
		if(IndexOf(in) != -1)
		{
			Remove(in);
			delete (TDebugHelp *)in;
			in = NULL;
		}
   }
}

UnicodeString TManagerDebug::MessageTypeToStr(MESSAGE_TYPE MessageType)
{
	UnicodeString msgType = "UNKNOWN";
	switch(MessageType)
   {
		case KEEPALIVE : msgType = "KEEPALIVE";
		break;
		case REGISTRATION : msgType = "REGISTRATION";
		break;
		case INFO_SYNC : msgType = "INFO_SYNC";
      break;
      case MENU_SYNC : msgType = "MENU_SYNC";
      break;
      case DRINK_SYNC : msgType = "DRINK_SYNC";
      break;
      case LOGIN : msgType = "LOGIN";
      break;
		case ORDERS : msgType = "ORDERS";
		break;
		case CALL_AWAY : msgType = "CALL_AWAY";
		break;
		case REQUEST_RECEIPT : msgType = "REQUEST_RECEIPT";
		break;
      case TRANSFER_TABLE_ORDERS : msgType = "TRANSFER_TABLE_ORDERS";
      break;
      case RETRIEVE_ORDERS : msgType = "RETRIEVE_ORDERS";
      break;
	}
	return msgType;
}

UnicodeString TManagerDebug::ErrorTypeToStr(EProcessingErrorType ErrorType)
{
	UnicodeString ErrType = "Unknown";
	switch(ErrorType)
	{
		case proA_Ok :	ErrType = "Msg_Ok";
		break;
		case Err_ErrorMsg : ErrType = "Err_ErrorMsg";
		break;
		case Err_Msg_CRC : ErrType = "Err_Msg_CRC";
		break;
		case Err_Currupt_Message : ErrType = "Err_Currupt_Message";
		break;
		case Err_Mismatched_Message_Data : ErrType = "Err_Mismatched_Message_Data";
		break;
		case Err_InfoOutOfSync : ErrType = "Err_InfoOutOfSync";
		break;
		case Err_MenuOutOfSync : ErrType = "Err_MenuOutOfSync";
		break;
		case Err_DrinkOutOfSync : ErrType = "Err_DrinkOutOfSync";
		break;
		case Err_Database : ErrType = "Err_Server_DB";
		break;
		case Err_Not_Logged_In : ErrType = "Err_Not_Logged_In";
		break;
		case Err_PIN_Incorrect : ErrType = "Err_PIN_Incorrect";
		break;
		case Err_User_Not_Found : ErrType = "Err_User_Not_Found";
		break;
		case Err_User_OverFlow : ErrType = "Err_User_OverFlow";
		break;
		case Err_DeviceKey_Not_Found : ErrType = "Err_DeviceKey_Not_Found";
		break;
		case Err_Reg_Password_Wrong : ErrType = "Err_Reg_Password_Wrong";
		break;
		case Err_Not_In_Reg_Mode : ErrType = "Err_Not_In_Reg_Mode";
		break;
		case Err_No_Printer_Found : ErrType = "Err_No_Printer_Found";
		break;
		case Err_Incorrect_Version : ErrType = "Err_Incorrect_Version";
		break;
		case Err_Destructor : ErrType = "Err_Destructor";
		break;
		case Wrn_NonFatalWarnings : ErrType = "Wrn_NonFatalWarnings";
		break;
		case Wrn_WarningMsg : ErrType = "Wrn_WarningMsg";
		break;
		case Wrn_No_Menu_Found : ErrType = "Wrn_No_Menu_Found";
		break;
		case Wrn_Printing_Redirected : ErrType = "Wrn_Printing_Redirected";
		break;
		case Wrn_Printer_Offline : ErrType = "Wrn_Printer_Offline";
		break;
		case Wrn_Duplicate_Order : ErrType = "Wrn_Duplicate_Order";
		break;
		case PalmToServerErrors : ErrType = "PalmToServerErrors";
		break;
		default : ErrType = "Unknown Error : " + IntToStr(ErrorType);
	}
   return ErrType;
}
