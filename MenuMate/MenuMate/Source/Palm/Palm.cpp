//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <set>
#include <map>
#include <iterator>
#include <stdio.h>

#include "Palm.h"
#include "Printing.h"
#include "Comms.h"
#include "MMData.h"
#include "CrcTable.h"   // CRC Message checking.
#include "MMRegistry.h"
#include "bluetooth.h"

#include "ManagerStock.h"
#include "TableManager.h"
#include "DBTab.h"
#include "DBOrder.h"
#include "DBTables.h"
#include "DBSecurity.h"
#include "DebugManager.h"
#include "Money.h"
//#include "ListPaymentSystem.h"
#include "ManagerVariable.h"
#include "DocketManager.h"
#include "MM_DBCore.h"
#include "ContactStaff.h"
#include "ManagerDiscount.h"
#include "ReqPrintJob.h"
#include "ListSecurityRefContainer.h"
#include "enumContacts.h"
#include "ItemSize.h"
#include "ItemRecipe.h"
#include "ProcessSecurityQueries.h"
#include "PaymentTransaction.h"

//------------------------------------------------------------------------------
#if (DEBUGLEVEL & debugExpandGetOrdes)
#include "MMMessageBox.h"
#endif
//------------------------------------------------------------------------------
#pragma package(smart_init)

#define BYTESWAP(val) (unsigned short)(((val << 8) & 0xff00) + ((val >> 8) & 0x00ff))
//------------------------------------------------------------------------------
bool LogGetOrders = false; // Global (ick!) set from PalmSetup
//------------------------------------------------------------------------------
char const soh = '[';
char const eoh = ']';
char const sep = '|';
char const coma = ',';
char const soo = '{';
char const eoo = '}';
//------------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool IsLast(int InItemID);
bool IsLast(unsigned char InItemID);
bool BIT6SET(unsigned char Byte);
bool BIT5SET(unsigned char Byte);
bool BIT4SET(unsigned char Byte);
bool BIT3SET(unsigned char Byte);
bool BIT2SET(unsigned char Byte);
bool BIT1SET(unsigned char Byte);
bool BIT0SET(unsigned char Byte);
//------------------------------------------------------------------------------

bool BIT6SET(unsigned char Byte) //(7th/8 bit)
{
	if((Byte & 0x40) == 0x40)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//------------------------------------------------------------------------------

bool BIT5SET(unsigned char Byte) //(6th/8 bit)
{
	if((Byte & 0x20) == 0x20)
	{
		return true;
	}
	else
	{
      return false;
	}
}

//------------------------------------------------------------------------------

bool BIT4SET(unsigned char Byte) //(6th/8 bit)
{
   if((Byte & 0x10) == 0x10)
   {
      return true;
   }
   else
	{
      return false;
   }
}

//------------------------------------------------------------------------------

bool BIT3SET(unsigned char Byte) //(6th/8 bit)
{
   if((Byte & 0x08) == 0x08)
   {
      return true;
   }
   else
   {
      return false;
   }
}

//------------------------------------------------------------------------------

bool BIT2SET(unsigned char Byte) //(6th/8 bit)
{
   if((Byte & 0x04) == 0x04)
   {
      return true;
   }
   else
	{
      return false;
   }
}

//------------------------------------------------------------------------------

bool BIT1SET(unsigned char Byte) //(6th/8 bit)
{
   if((Byte & 0x02) == 0x02)
   {
      return true;
   }
   else
   {
      return false;
   }
}

//------------------------------------------------------------------------------

bool BIT0SET(unsigned char Byte) //(6th/8 bit)
{
   if((Byte & 0x01) == 0x01)
   {
		return true;
   }
   else
   {
      return false;
   }
}

//------------------------------------------------------------------------------
/*int __fastcall OrderSeatNames(void *Item1, void *Item2)
{
	TPalmSeatName *Seat1 = (TPalmSeatName *)Item1;
	TPalmSeatName *Seat2 = (TPalmSeatName *)Item2;

	if (Seat1->TableNumber > Seat2->TableNumber) return 1;
	else if (Seat1->TableNumber < Seat2->TableNumber) return -1;
	else
	{
		if (Seat1->SeatNumber > Seat2->SeatNumber) return 1;
		else if (Seat1->SeatNumber < Seat2->SeatNumber) return -1;
		else return 0;
	}
}*/
//------------------------------------------------------------------------------
int __fastcall  OrderListSort(void * inOrder1,void *inOrder2)
{
	TReturnItemComplete *ReturnOrder1 = (TReturnItemComplete *)inOrder1;
	TReturnItemComplete *ReturnOrder2 = (TReturnItemComplete *)inOrder2;
	if(ReturnOrder1->Palm_ID < ReturnOrder2->Palm_ID)
	{
		return -1;
	}
	else if(ReturnOrder1->Palm_ID == ReturnOrder2->Palm_ID)
	{
		if (ReturnOrder1->OrderKey < ReturnOrder2->OrderKey)
		{
			return -1;
		}
		else if (ReturnOrder1->OrderKey == ReturnOrder2->OrderKey)
		{
			return 0;
		}
		else
		{
			return 1;
		}
   }
   else
   {
      return 1;
   }
}

__fastcall TDeviceRealPalms::TDeviceRealPalms()
  : TDeviceRealControl(new TManagerMenusPalm)
{
   // The first connect made will generate the Transmission list for Plam Trans Objects..
	PalmList = new TList;
// Always contains a Palm Not found Error Message.
	PalmNotFound = new TPalm();
	PalmNotFound->ID.DeviceKey = PalmNotFoundID;
	PalmNotFound->ID.Name = "PalmNotFound";

   OSVERSIONINFO osvi;
   char szVersion [80];
   memset(&osvi, 0, sizeof(OSVERSIONINFO));
   osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
   GetVersionEx (&osvi);
	OS.dwPlatformId = osvi.dwPlatformId;

   Reboot = false;
   ShutDown = false;
   RegistrationPIN = "";
	BlueTooth = new TBluetoothLayer;
	RegExpire = new TTimer(NULL);
	RegExpire->OnTimer = RegExpireTimer;
	RegExpire->Enabled = false;
	RegExpire->Interval = (1000 * 60) * 5;
	RegSyncNumber = 0;
}

//------------------------------------------------------------------------------
void __fastcall TDeviceRealPalms::RegExpireTimer(TObject *Sender)
{
	RegistrationPIN = "";
	RegExpire->Enabled = false;
}

void __fastcall TDeviceRealPalms::BuildInfoSyncPacket(Database::TDBTransaction &DBTransaction,TMemoryStream *Process_Outbound)
{
   UnicodeString header				= "[Info]";
   UnicodeString DefaultSeatName	= TManagerVariable::Instance().GetStr(DBTransaction,vmSeatLabel, "");
   UnicodeString Tableheader		= "[Table]";
   UnicodeString Staffheader		= "[Staff]";
   UnicodeString Palmsheader		= "[Palms]";
   // UnicodeString Version = "XX.XXX";
   //	UnicodeString Version			= "1.7";
   int NewLineChar = 10;

   //TComms::Instance().Process_Outbound->Memory
   Process_Outbound->Write(header.t_str(),header.Length());
   Process_Outbound->Write(&NewLineChar,1); // newline character.
   // Versioning Info.
	Process_Outbound->Write(DefaultSeatName.t_str(), DefaultSeatName.Length());
   Process_Outbound->Write(&NewLineChar,1); // newline character.
   // Add Features here.
   UnicodeString Rounding = "1";
   Process_Outbound->Write(Rounding.t_str(),Rounding.Length());  // newline character.
   Process_Outbound->Write(&NewLineChar,1);  // newline character.
   Process_Outbound->Write(&NewLineChar,1);  // newline character.

   Process_Outbound->Write(Tableheader.t_str(),Tableheader.Length());  // Remove the NULL.
   Process_Outbound->Write(&NewLineChar,1);  // newline character.

   // Set the tables caption.
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
   " SELECT "
   " TABLE_KEY, TABLE_NUMBER, TABLE_NAME, PARTY_NAME "
   " FROM "
   "  TABLES "
   " WHERE "
   " TABLE_NAME IS NOT NULL AND TABLE_NAME != '' "
   " ORDER BY "
   "  TABLE_NUMBER ";
   IBInternalQuery->ExecQuery();
   for(;!IBInternalQuery->Eof; IBInternalQuery->Next())
   {
          if (IBInternalQuery->FieldByName("TABLE_NAME")->AsString != "")
          {
                   Process_Outbound->Write(IBInternalQuery->FieldByName("TABLE_NUMBER")->AsString.t_str(),
                   IBInternalQuery->FieldByName("TABLE_NUMBER")->AsString.Length());
                   Process_Outbound->Write(&sep,sizeof(sep));
                   Process_Outbound->Write(IBInternalQuery->FieldByName("TABLE_NAME")->AsString.t_str(),
                   IBInternalQuery->FieldByName("TABLE_NAME")->AsString.Length());
                   Process_Outbound->Write(&NewLineChar,1); // newline character.
          }
   }

   Process_Outbound->Write(&NewLineChar,1); // newline character.
   Process_Outbound->Write(Staffheader.t_str(),Staffheader.Length()); // Remove the NULL.
   Process_Outbound->Write(&NewLineChar,1); // newline character.

   // ok write out the servers.
   TStringList *List = new TStringList;
   try
   {
      std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
      Staff->GetContactList(DBTransaction,List);
      for (int i = 0; i < List->Count; i++)
      {
         int UserKey = int(List->Objects[i]);
         UnicodeString Name = List->Strings[i];
         UnicodeString Initails = Staff->GetContactInitials(DBTransaction,UserKey);

         Process_Outbound->Write(IntToStr(UserKey).t_str(),IntToStr(UserKey).Length());
         Process_Outbound->Write(&sep,sizeof(sep));
         Process_Outbound->Write(Name.t_str(),Name.Length());
         Process_Outbound->Write(&sep,sizeof(sep));
         Process_Outbound->Write(Initails.t_str(),Initails.Length());
         Process_Outbound->Write(&NewLineChar,1); // newline character.
      }
   }
   __finally
   {
   	delete List;
   }

   Process_Outbound->Write(&NewLineChar,1); // newline character.
   Process_Outbound->Write(Palmsheader.t_str(),Palmsheader.Length()); // Remove the NULL.
   Process_Outbound->Write(&NewLineChar,1); // newline character.

   for(int i = 0 ; i < PalmList->Count ; i++)
   {
      TPalm *Ptr = (TPalm *)PalmList->Items[i];
      Process_Outbound->Write(IntToStr(Ptr->ID.DeviceKey).t_str(),IntToStr(Ptr->ID.DeviceKey).Length());
      Process_Outbound->Write(&sep,sizeof(sep));
      Process_Outbound->Write(Ptr->ID.Name.t_str(),Ptr->ID.Name.Length());
      Process_Outbound->Write(&NewLineChar,1); // newline character.
   }
   Process_Outbound->Write(&NewLineChar,1); // newline character.
   Process_Outbound->SaveToFile("InfoSync.txt"); // newline character.
}

//------------------------------------------------------------------------------

__fastcall TDeviceRealPalms::~TDeviceRealPalms()
{
   // Save of all Palms that arnt in list all ready.
	if(DBControl.Connected())
	{
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		SavePalms(DBTransaction);
		DBTransaction.Commit();
	}

   TPalm *TempPalm;
	while(PalmList->Count != 0)
   {
		TempPalm = (TPalm *)PalmList->Items[0];
		delete TempPalm;
      PalmList->Delete(0);
   }
	delete PalmList;
	delete PalmNotFound;
	delete BlueTooth;
	delete RegExpire;
	delete Menus;
}

//------------------------------------------------------------------------------

// Removes a Palm form the Palm List and frees its memory.
// Will most likly be used when someone logs out.
void TDeviceRealPalms::FreePalm(UnicodeString Name)
{
	for(int i = 0 ; i < PalmList->Count ; i++)
   {
		TPalm *Ptr = (TPalm *)PalmList->Items[i];
		if(Ptr->ID.Name == Name)
      {
         delete Ptr;
      	PalmList->Delete(i);
      }
   }
}

//------------------------------------------------------------------------------

// Returns a Plam based on DeviceID.
// Sets the Palm InUse to True.
// This is the Only way to get a pointer to a Palm.
TPalm* TDeviceRealPalms::FindTPalm(Database::TDBTransaction &DBTransaction,unsigned short DeviceKey)
{
   PalmNotFound->ID.DeviceKey = PalmNotFoundID; // Reset Palm Not Found.

	TPalm *Palm = NULL;
	if(DeviceKey == 0)
   {
		Palm = PalmNotFound;
   }	// No palms in list request an InfoSync on Palm 0 (almost like a channel zero.)
   else if(PalmList->Count == 0)
   {
      // Check in Database.
      Palm = new TPalm();
		Palm->ID.DeviceKey = DeviceKey;
		Palm->Load(DBTransaction);
      if(Palm->Status.Error == Err_DeviceKey_Not_Found)
      {
         delete Palm;
         Palm = PalmNotFound;
      }
      else
      {
         PalmList->Add(Palm);
      }
   }
   else //Search using Palm ID.
   {
		Palm = PalmNotFound;
		for(int i = 0 ; i < PalmList->Count ; i++)
		{
			TPalm *Ptr = (TPalm *)PalmList->Items[i];
			if(Ptr->ID.DeviceKey == DeviceKey)
			{
				Palm = Ptr;
			}
		}
   }
   return Palm;
}
//------------------------------------------------------------------------------

TPalm* TDeviceRealPalms::FindTPalmByName(Database::TDBTransaction &DBTransaction,UnicodeString DeviceName)
{
   PalmNotFound->ID.DeviceKey = PalmNotFoundID; // Reset Palm Not Found.
   
	TPalm *Palm = NULL;
	if(DeviceName == "")
   {
		Palm = PalmNotFound;
   }	// No palms in list request an InfoSync on Palm 0 (almost like a channel zero.)
   else if(PalmList->Count == 0)
   {
      // Check in Database.
      Palm = new TPalm();
		Palm->ID.DeviceKey = NameToKey(DBTransaction,DeviceName);
      Palm->ID.Name = DeviceName;
      Palm->ID.IP = "";
      Palm->TransNo = 0;
      if(Palm->ID.DeviceKey != 0)
      {
			Palm->Load(DBTransaction);
			if(Palm->Status.Error == Err_DeviceKey_Not_Found)
	      {
	         delete Palm;         
         	Palm = PalmNotFound;
         }
         else
         {
         	PalmList->Add(Palm);
         }
      }
      else
      {
         delete Palm;
		   Palm = PalmNotFound;
      }
   }
   else //Search using Palm ID.
   {
		Palm = PalmNotFound;
		for(int i = 0 ; i < PalmList->Count ; i++)
		{
			TPalm *Ptr = (TPalm *)PalmList->Items[i];
			if(Ptr->ID.Name == DeviceName)
			{
				Palm = Ptr;
			}
		}
   }
   return Palm;
}

TPalm* TDeviceRealPalms::FindTPalm(Database::TDBTransaction &DBTransaction,UnicodeString DeviceIP)
{
   PalmNotFound->ID.DeviceKey = PalmNotFoundID; // Reset Palm Not Found.
   
	TPalm *Palm = NULL;
	if(DeviceIP == "")
   {
		Palm = PalmNotFound;
   }	// No palms in list request an InfoSync on Palm 0 (almost like a channel zero.)
   else //Search using Palm ID.
   {
      Palm = PalmNotFound;
      for(int i = 0 ; i < PalmList->Count ; i++)
      {
         TPalm *Ptr = (TPalm *)PalmList->Items[i];
         if(Ptr->ID.IP == DeviceIP)
         {
            Palm = Ptr;
         }
      }

		if(Palm == PalmNotFound) // Ok Check in DB
      {
         // Check in Database.
         TPalm *DBPalm = new TPalm();
			DBPalm->ID.DeviceKey = IPToKey(DBTransaction,DeviceIP);
         DBPalm->ID.Name = "";
         DBPalm->ID.IP = DeviceIP;
         DBPalm->TransNo = 0;
         if(DBPalm->ID.DeviceKey != 0)
         {
				DBPalm->Load(DBTransaction);
            if(DBPalm->Status.Error == Err_DeviceKey_Not_Found)
            {
               delete DBPalm;
               Palm = PalmNotFound;
            }
            else
            {
               PalmList->Add(DBPalm);
               Palm = DBPalm;
            }
         }
         else
         {
            delete DBPalm;
            Palm = PalmNotFound;
         }
      }
   }
   return Palm;
}
//------------------------------------------------------------------------------

void TDeviceRealPalms::UpdateCurrentRecipes(UnicodeString Code,UnicodeString Location,Currency Cost,double CostGSTPercent)
{
   for (int i = 0 ; i < Menus->Current->Count ; i++)
   {
		TListMenu *Menu = Menus->Current->MenuGet(i);

      for (int k = 0;k < Menu->Count; k++ )
      {
         TListCourse *Course = Menu->CourseGet(k);

			for(int i=0; i<Course->Count; i++)
			{
				TItem *Item = Course->ItemGet(i);

				for(int i=0; i<Item->Sizes->Count; i++)
				{
					TItemSize *ItemSize = Item->Sizes->SizeGet(i);

					for(int i=0; i<ItemSize->Recipes->Count; i++)
					{
						TItemRecipe *RecipeItem = ItemSize->Recipes->RecipeGet(i);
						if (RecipeItem->StockCode == Code)
						{
							RecipeItem->Cost			= Cost * RecipeItem->Qty;
                     RecipeItem->CostGSTPercent = CostGSTPercent;
						}
					}
				}
			}
		}
	}
}

//------------------------------------------------------------------------------
void TDeviceRealPalms::UpdateMessage(TNetMessageMsg * Request)
{
	TPalmMessage PalmMessage(Request->User,Request->Message,Now());
	for (int i = 0; i < PalmList->Count ; i++)
	{
		TPalm *CurrentPalm = (TPalm *)PalmList->Items[i];
		CurrentPalm->Messages.push_back(PalmMessage);
	}
}

__fastcall TPalm::TPalm()
{
   ID.Type = devPalm;
   LastConnectVar = 0;
//   SeatNames = new TList;
   Inbound = new TIRPacket(this);
   Outbound = new TMemoryStream;
   TransNo = 0;
	User.ContactID = 0;
   User.ContactKey = 0;
	Status.Active = false;
   Status.Error = proA_Ok;
//	DownloadOrders = ThisCurrentTable;
//   LogoffTime = 5;
   CurrentTable = 0;
//   AutoPopup = false;
//   LargeFont = false;
//   CurrentTables.clear();
	resetMenuEnableState(); // Will Force an update.
	Delete = false;
	Messages.clear();
}

//------------------------------------------------------------------------------

__fastcall TPalm::~TPalm()
{
   delete Inbound;
   delete Outbound;

/*   TPalmSeatName *SeatName;

	while(SeatNames->Count != 0)
	{
		SeatName = (TPalmSeatName *)SeatNames->Items[0];
		delete SeatName;
		SeatNames->Delete(0);
	}

	delete SeatNames; */
}
/*
void TPalm::DumpStatus(TStringList *Debug)
{
	Debug->Add("Memory Usage of Palm " + ID.Name);
	Debug->Add("Size of this Palm " + IntToStr(sizeof(TPalm)));
//   Debug->Add("Count of Palm SeatNames Pending " + IntToStr(SeatNames.size()));
	Debug->Add("Count of Palm SeatNames Pending " + IntToStr(SeatNames.NamesCount()));

   Debug->Add("Size of Palm Outbound Stream "+ IntToStr(Outbound->Size) );
   Debug->Add("Size of Palm Inbound Packet " + IntToStr(sizeof(TIRPacket)) );

	Debug->Add("Inbound Orders Tables Capacity " + IntToStr(Inbound->Orders.Tables->Capacity));
	Debug->Add("Inbound Orders Tables Size " + IntToStr(Inbound->Orders.Tables->Count));
	Debug->Add("Inbound Billing Tables Capacity " + IntToStr(Inbound->Billing.Tables->Capacity));
	Debug->Add("Inbound Billing Tables Size " + IntToStr(Inbound->Billing.Tables->Count));

}*/
//------------------------------------------------------------------------------

void TPalm::BuildPacket(TMemoryStream *Process_Outbound,MESSAGE_TYPE MsgType)
{
	#ifdef _DEBUG
	bool ScrewUpTheCRC = false;
	#endif

	EProcessingErrorType ErrorNum = this->Status.Error;
	//Thread Safe IRConnect Checks this before Talking to this palm.
   MESSAGE_TYPE Command;
   if(MsgType != NA) // Message type overriding.
	{
      Command = MsgType;
   }
   else
   {
      Command = this->Inbound->Header.Command;
   }

   unsigned char lPktSize = 0, hPktSize = 0;
   LastErrorNumber = ErrorNum;
   LastTransmissionType = Command;

   UnicodeString msgType = "";
	switch(Command)
	{
		case KEEPALIVE:					msgType = "KEEPALIVE";					break;
		case REGISTRATION:				msgType = "REGISTRATION";				break;
		case INFO_SYNC:					msgType = "INFO_SYNC";					break;
		case MENU_SYNC:					msgType = "MENU_SYNC";					break;
		case DRINK_SYNC:					msgType = "DRINK_SYNC";					break;
		case LOGIN:							msgType = "LOGIN";						break;
		case SEAT_NAMES:					msgType = "SEAT_NAMES";					break;
		case ORDERS:						msgType = "ORDERS";						break;
		case CALL_AWAY:					msgType = "CALL_AWAY";					break;
		case REQUEST_RECEIPT:			msgType = "REQUEST_RECEIPT";			break;
		case TRANSFER_TABLE_ORDERS:	msgType = "TRANSFER_TABLE_ORDERS";	break;
		case RETRIEVE_ORDERS:			msgType = "RETRIEVE_ORDERS";			break;
   }

	TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"Building Packet " + msgType,"Palm #" + IntToStr(this->ID.DeviceKey));
	UnicodeString ErrType = "Ok";
	if(ErrorNum != proA_Ok)
	{
      switch(ErrorNum)
      {
			case Err_ErrorMsg:						ErrType = "Err_ErrorMsg";
				Process_Outbound = TComms::Instance().Process_Outbound;
				Process_Outbound->Clear();
				Process_Outbound->Write(this->Status.ErrorMsg.t_str(),this->Status.ErrorMsg.Length());
			break;
			case Err_Msg_CRC:							ErrType = "Err_Msg_CRC";						break;
			case Err_Currupt_Message:				ErrType = "Err_Currupt_Message";				break;
			case Err_Mismatched_Message_Data:	ErrType = "Err_Mismatched_Message_Data";	break;
			case Err_InfoOutOfSync:					ErrType = "Err_InfoOutOfSync";				break;
			case Err_MenuOutOfSync:					ErrType = "Err_MenuOutOfSync";				break;
			case Err_DrinkOutOfSync:				ErrType = "Err_DrinkOutOfSync";				break;
			case Err_Database:						ErrType = "Err_Server_DB";						break;
			case Err_Not_Logged_In:					ErrType = "Err_Not_Logged_In";				break;
			case Err_PIN_Incorrect:					ErrType = "Err_PIN_Incorrect";				break;
			case Err_User_Not_Found:				ErrType = "Err_User_Not_Found";				break;
			case Err_User_OverFlow:					ErrType = "Err_User_OverFlow";				break;
			case Err_DeviceKey_Not_Found:			ErrType = "Err_DeviceKey_Not_Found";		break;
			case Err_No_Printer_Found:				ErrType = "Err_No_Printer_Found";			break;
			case Err_Reg_Password_Wrong:			ErrType = "Err_Reg_Password_Wrong";			break;
			case Wrn_No_Menu_Found:					ErrType = "Wrn_No_Menu_Found";				break;
			case Wrn_Printing_Redirected:			ErrType = "Wrn_Printing_Redirected";		break;
			case Wrn_Duplicate_Order:				ErrType = "Wrn_Duplicate_Order";				break;
			case Wrn_Printer_Offline:				ErrType = "Wrn_Printer_Offline";				break;
			default:										ErrType = "Unknown Error : " + IntToStr(ErrorNum);
		}
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Reporting Palm Status " + ErrType,"Palm #" + IntToStr(this->ID.DeviceKey));
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Error Message " + this->Status.ErrorMsg,"Palm #" + IntToStr(this->ID.DeviceKey));
	}
	#ifdef _DEBUG
	else
	{
		ErrorNum = ManagerDebug->GetErrorNumber(Command);
		if(ErrorNum == 256)
		{
      	throw Exception("Bailing on Debugger Request");
		}
		if(ErrorNum == 257)
		{
			ErrorNum = proA_Ok;
			ScrewUpTheCRC = true;
		}

	}
	#endif

	if(Process_Outbound)
	{
		hPktSize = HIBYTE(LOWORD(Process_Outbound->Size));
		lPktSize = LOBYTE(LOWORD(Process_Outbound->Size));

		// Clear the Working Outbound.
      Outbound->Clear();

      // Copy in Command Word.
      Outbound->Write(&Command, 1);
      // Copy in Transaction Number.
      unsigned char TransNumber = TransNo - 1;
      Outbound->Write(&TransNumber, 1);
      TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"PalmMates Current TransNumber : " + IntToStr(TransNumber));
      // InfoSync Number.
      Outbound->Write(&TGlobalSettings::Instance().InfoSync, 1);
      TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"PalmMates Current InfoSync : " + IntToStr(TGlobalSettings::Instance().InfoSync));

      // Menu Sync Number.
      TDeviceRealPalms::Instance().Menus->SelectMenuOfType(eFoodMenu);
      if(TDeviceRealPalms::Instance().Menus->MenuFood != NULL )
      {
         unsigned char FoodMenuSync = TDeviceRealPalms::Instance().Menus->MenuFood->MenuKey;
         Outbound->Write(&FoodMenuSync, 1);
         TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"PalmMates Current MenuSync : " + IntToStr(FoodMenuSync));
      }
      else
		{
         unsigned char FoodMenuSync = 0;
         Outbound->Write(&FoodMenuSync, 1);
			TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"PalmMates Current MenuSync : " + IntToStr(FoodMenuSync));
      }
      // Drink Sync Number.
      TDeviceRealPalms::Instance().Menus->SelectMenuOfType(eDrinksMenu);
      if(TDeviceRealPalms::Instance().Menus->MenuDrinks != NULL )
      {
         unsigned char DrinkMenuSync = TDeviceRealPalms::Instance().Menus->MenuDrinks->MenuKey;
         Outbound->Write(&DrinkMenuSync, 1);
         TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"PalmMates Current DrinkSync : " + IntToStr(DrinkMenuSync));
      }
      else
      {
         unsigned char DrinkMenuSync = 0;
         Outbound->Write(&DrinkMenuSync, 1);
         TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"PalmMates Current DrinkSync : " + IntToStr(DrinkMenuSync));
      }
      // Staff ID Number.
      unsigned char hContactKey = HIBYTE(LOWORD(User.ContactKey));
      unsigned char lContactKey = LOBYTE(LOWORD(User.ContactKey));

      Outbound->Write(&hContactKey, 1);
      Outbound->Write(&lContactKey, 1);
		TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"PalmMates Current UserID : " + IntToStr(User.ContactKey),"Palm #" + IntToStr(this->ID.DeviceKey));
		// Error Number.
		Outbound->Write(&ErrorNum, 1);

      // High Byte Count Number.
      Outbound->Write(&hPktSize, 1);
      // Low Byte Count Number.
      Outbound->Write(&lPktSize, 1);

      // Data.  TMemoryStream
      Outbound->CopyFrom(Process_Outbound,0);
      //Calculate CRC.

      unsigned char *ptrPacket = (unsigned char *)Outbound->Memory;
      unsigned short int CRC = 0;
      CRC =  StartCRC(ptrPacket[0]);
      for(int i = 1; i < Outbound->Size;i++) //Dont include CRC Section.
      {
         CRC = UpdateCRC(CRC,ptrPacket[i]);
		}
		unsigned char lCRC = 0, hCRC = 0;

		hCRC = HIBYTE(CRC);
		lCRC = LOBYTE(CRC);
		// Add Internal CRC.
		#ifdef _DEBUG
		if(ScrewUpTheCRC)
		{
      	hCRC++;
		}
		#endif

		Outbound->Write(&hCRC, 1);
      Outbound->Write(&lCRC, 1);
      // Process has been packed so clear it.
		Process_Outbound->Clear();
	}
   else // No Data to Return return a "NULL" Packet. Could Include an error number.
	{
      hPktSize = 0;
      lPktSize = 0;

   // Clear the Working Outbound Stream.
      Outbound->Clear();

      // Copy in Command Word.
      Outbound->Write(&Command, 1);
      // Copy in Transaction Number.
      unsigned char TransNumber = TransNo-1;
      Outbound->Write(&TransNumber, 1);
      TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"PalmMates Current TransNumber : " + IntToStr(TransNumber));
      // InfoSync Number.
      Outbound->Write(&TGlobalSettings::Instance().InfoSync, 1);
      TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"PalmMates Current InfoSync : " + IntToStr(TGlobalSettings::Instance().InfoSync));
      // Menu Sync Number.
      TDeviceRealPalms::Instance().Menus->SelectMenuOfType(eFoodMenu);
      if(TDeviceRealPalms::Instance().Menus->MenuFood != NULL )
      {
         unsigned char FoodMenuSync = TDeviceRealPalms::Instance().Menus->MenuFood->MenuKey;
         Outbound->Write(&FoodMenuSync, 1);
			TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"PalmMates Current MenuSync : " + IntToStr(FoodMenuSync));
      }
      else
      {
         unsigned char FoodMenuSync = 0;
         Outbound->Write(&FoodMenuSync, 1);
         TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"PalmMates Current MenuSync : " + IntToStr(FoodMenuSync));
      }
      // Drink Sync Number.
      TDeviceRealPalms::Instance().Menus->SelectMenuOfType(eDrinksMenu);
      if(TDeviceRealPalms::Instance().Menus->MenuDrinks != NULL )
      {
         unsigned char DrinkMenuSync = TDeviceRealPalms::Instance().Menus->MenuDrinks->MenuKey;
         Outbound->Write(&DrinkMenuSync, 1);
         TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"PalmMates Current DrinkSync : " + IntToStr(DrinkMenuSync));
      }
      else
      {
         unsigned char DrinkMenuSync = 0;
         Outbound->Write(&DrinkMenuSync, 1);
         TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"PalmMates Current DrinkSync : " + IntToStr(DrinkMenuSync));
      }
      // Staff ID Number.

      unsigned char hContactKey = HIBYTE(LOWORD(User.ContactKey));
      unsigned char lContactKey = LOBYTE(LOWORD(User.ContactKey));

      Outbound->Write(&hContactKey, 1);
      Outbound->Write(&lContactKey, 1);

		TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"PalmMates Current UserID : " + IntToStr(User.ContactKey),"Palm #" + IntToStr(this->ID.DeviceKey));
		// Error Number.
		Outbound->Write(&ErrorNum, 1);
		TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"Error Code : " + IntToStr(ErrorNum),"Palm #" + IntToStr(this->ID.DeviceKey));
// High Byte Count Number.
		Outbound->Write(&hPktSize, 1);
      // Low Byte Count Number.
      Outbound->Write(&lPktSize, 1);

      unsigned char *ptrPacket = (unsigned char *)Outbound->Memory;
      unsigned short int CRC = 0;
      CRC =  StartCRC(ptrPacket[0]);
      for(int i = 1; i < Outbound->Size;i++) //Dont include CRC Section.
      {
         CRC = UpdateCRC(CRC,ptrPacket[i]);
      }
   	unsigned char lCRC = 0, hCRC = 0;

      hCRC = HIBYTE(CRC);
      lCRC = LOBYTE(CRC);
      // Add Internal CRC.
      Outbound->Write(&hCRC, 1);
      Outbound->Write(&lCRC, 1);
   }
   Status.Active = true;
	Status.LastActive = ::GetTickCount();
}

//------------------------------------------------------------------------------

void TPalm::ClearInbound()
{
   Inbound->Clear();
}

//------------------------------------------------------------------------------

void TPalm::UpdateTransactionNumber(Database::TDBTransaction &DBTransaction)
{
	// Find the Palm in the Database.
	TIBSQL *IBSQL = DBTransaction.Query(DBTransaction.AddQuery());

	if (ID.Name != "")
	{
		IBSQL->SQL->Text =
			"Update DEVICES Set TRANSNO = :TRANSNO Where DEVICE_NAME = :DEVICE_NAME";
		IBSQL->ParamByName("DEVICE_NAME")->AsString = ID.Name;
		IBSQL->ParamByName("TRANSNO")->AsString = Inbound->Header.TransNo;
		IBSQL->ExecQuery();
		if (IBSQL->RowsAffected == 0)
		{
			Status.Error = Err_DeviceKey_Not_Found;
		}
	}
	else if (ID.DeviceKey != 0)
	{
		IBSQL->SQL->Text =
			"Update DEVICES Set TRANSNO = :TRANSNO Where DEVICE_KEY = :DEVICE_KEY";
		IBSQL->ParamByName("DEVICE_KEY")->AsInteger = ID.DeviceKey;
		IBSQL->ParamByName("TRANSNO")->AsString = Inbound->Header.TransNo;
		IBSQL->ExecQuery();
		if (IBSQL->RowsAffected == 0)
		{
			Status.Error = Err_DeviceKey_Not_Found;
		}
	}
	else
	{
		Status.Error = Err_DeviceKey_Not_Found;
	}
}

//------------------------------------------------------------------------------
/*bool TPalm::GetSeatName(int TableNumber, int SeatNumber, TPalmSeatName &PalmSeatName)
{
	TPalmSeatName *SeatName;
	for(unsigned int i=0; i<SeatNames.size(); i++)
	{
		if(SeatNames[i].TableNumber == TableNumber && SeatName->SeatNumber == SeatNumber)
		{
			PalmSeatName = SeatNames[i];
			return true;
		}
	}
	return false;
} */
//------------------------------------------------------------------------------
/*void TPalm::RemoveSeatName(TPalmSeatName *ptrName)
{
	 ptrName->DeleteMe = true;
}
//------------------------------------------------------------------------------
void TPalm::ClearSeatNames()
{
	TPalmSeatName *SeatName = NULL;
	for(int i = 0; i < SeatNames->Count;)
	{
		SeatName = (TPalmSeatName *)SeatNames->Items[i];
		if(SeatName->DeleteMe)
		{
			SeatNames->Delete(i);
         delete SeatName;
         i = 0;
      }
      else
      {
         i++;
      }
	}
}*/


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void TPalmSeatNames::Load(Database::TDBTransaction &DBTransaction,int TableNumber)
{
	fTables.clear();

	TIBSQL *Query = DBTransaction.Query(DBTransaction.AddQuery());

	// Load all seats out of DB
	Query->SQL->Text =
		"Select "
			"Tables.Table_Number, "
			"Seat.SeatNo, "
			"Tab.Tab_Name "
		"From "
			"Tables Inner Join Seat On "
				"Tables.Table_Key = Seat.Table_Key "
			"Inner Join Tab On "
				"Seat.Tab_Key = Tab.Tab_Key "
		"Where "
			"Tab.Tab_Name Is Not Null And Tab.Tab_Name <> '' ";
	// Filter on a specific table if required. (Palm set to 'Current Table' mode)
	if (TableNumber > -1)
	{
		Query->SQL->Text = Query->SQL->Text + "And Tables.Table_Number = :Table_Number ";
	}
	Query->SQL->Text = Query->SQL->Text +
		"Order By "
			"Tables.Table_Number, "
			"Seat.SeatNo";
	if (TableNumber > -1)
	{
		Query->ParamByName("Table_Number")->AsInteger = TableNumber;
	}
	UnicodeString DefaultSeatName = TManagerVariable::Instance().GetStr(DBTransaction,vmSeatLabel, "");
	int LastTable	= 0;
	for (Query->ExecQuery(); !Query->Eof; Query->Next())
	{
		int TableNumber	= Query->FieldByName("Table_Number")->AsInteger;
		int SeatNumber		= Query->FieldByName("SeatNo")->AsInteger;
		UnicodeString Name	= Query->FieldByName("Tab_Name")->AsString;

		// Don't sent the seat name if it matches the default
		if (DefaultSeatName == "" || Name != DefaultSeatName + " " + IntToStr(SeatNumber))
		{
			// Create the next table yet?
			if (LastTable != TableNumber)
			{
				fTables.push_back(TPalmTable(TableNumber));
				LastTable = TableNumber;
			}
			// Add the seat to the last table
			fTables[fTables.size()-1].AddSeat(TPalmSeat(SeatNumber, Name));
		}
	}
}
//------------------------------------------------------------------------------
void TPalmSeatNames::Save(Database::TDBTransaction &DBTransaction)
{
	for (int tableIndex=0; tableIndex<TableCount(); ++tableIndex)
	{
		TPalmTable &palmTable = Table(tableIndex);
		for (int seatIndex=0; seatIndex<palmTable.SeatCount(); ++seatIndex)
		{
			TPalmSeat &palmSeat	= palmTable.Seat(seatIndex);

			int seatKey				= TDBTables::GetOrCreateSeat(DBTransaction,palmTable.TableNumber, palmSeat.SeatNumber);
			int tabKey				= TDBTab::GetOrCreateTab(DBTransaction,TDBTables::GetTabKey(DBTransaction,seatKey));

			TDBTables::SetSeatTab(DBTransaction,seatKey, tabKey);
			TDBTab::SetTabType(DBTransaction,tabKey, TabTableSeat);
			TDBTab::SetTabName(DBTransaction,tabKey, palmSeat.SeatName);
			if (palmSeat.SeatName == "")
			{
				palmSeat.SeatName = TDBTables::GetNameMinor(DBTransaction,palmTable.TableNumber, palmSeat.SeatNumber);
				TDBTab::SetTabName(DBTransaction,tabKey, palmSeat.SeatName);
			}
		}
	}
}
//------------------------------------------------------------------------------
/*int TPalmSeatNames::NamesCount()
{
	int Count = 0;
	for (int i=0; i<TableCount(); i++)
	{
		Count += Table(i).SeatCount();
	}
	return Count;
} */
//------------------------------------------------------------------------------
/*
TPalmSeatName::TPalmSeatName(int inTableNumber, int inSeatNumber,UnicodeString inSeatName) :
	TableNumber	(inTableNumber),
	SeatNumber	(inSeatNumber),
	SeatName		(inSeatName)
//	DeleteMe = false;
{
}


TPalmSeatName::~TPalmSeatName()
{
}

void TPalm::UpdateNames()
{
	while(SeatNames->Count != 0)
	{
		delete (TPalmSeatName *)SeatNames->Items[0];
		SeatNames->Delete(0);
	}
	SeatNames.clear();
	// Find all table Seat Tabs with Names and create palm Seat Names for them.
   bool StartedTrans = false;
	if (!frmData->IBTransaction->InTransaction) { frmData->IBTransaction->StartTransaction(); StartedTrans = true;}
	try
	{
		IBInternalQuery->Close();
      frmData->IBSQL->SQL->Text =
		"SELECT "//DISTINCT "
			"TABLES.TABLE_NUMBER, "
			"SEAT.SEATNO, "
			"TAB.TAB_NAME "
		"FROM "
			"TABLES INNER JOIN SEAT ON TABLES.TABLE_KEY = SEAT.TABLE_KEY "
				"INNER JOIN TAB ON SEAT.TAB_KEY = TAB.TAB_KEY "
		"WHERE "
			"TAB.TAB_NAME IS NOT NULL AND TAB.TAB_NAME <> ''";
		for (frmData->IBSQL->ExecQuery(); !frmData->IBSQL->Eof; frmData->IBSQL->Next())
		{
			TPalmSeatName *ptrName = new TPalmSeatName(  frmData->IBSQL->FieldByName("TABLE_NUMBER")->AsInteger,
																		frmData->IBSQL->FieldByName("SEATNO")->AsInteger,
																		frmData->IBSQL->FieldByName("TAB_NAME")->AsString);
			SeatNames->Add(ptrName);
}
		if (frmData->IBTransaction->InTransaction && StartedTrans) { frmData->IBTransaction->Commit(); }
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG, "TPalm::UpdateNames " + E.Message);
		if (frmData->IBTransaction->InTransaction  && StartedTrans) { frmData->IBTransaction->Rollback(); }
	}
} */


bool TPalm::getMenuEnabledStateChange()
{
   if(TDeviceRealPalms::Instance().Menus->MenuFood == NULL)
   {
      return false;
   }
   if(TDeviceRealPalms::Instance().Menus->MenuFood->EnabledStateSync == fNonFatialMenuEnableDisableNumber)
   {
      return false; // No Changes needed.
   }
   else
   {
      return true;
   }
}

bool TPalm::getDrinksEnabledStateChange()
{
   if(TDeviceRealPalms::Instance().Menus->MenuDrinks == NULL)
   {
      return false;
   }

   if(TDeviceRealPalms::Instance().Menus->MenuDrinks->EnabledStateSync == fNonFatialDrinksEnableDisableNumber)
   {
      return false; // No Changes needed.
   }
   else
   {
      return true;
   }
}

//------------------------------------------------------------------------------

void TPalm::setMenuEnableState()
{
   if(TDeviceRealPalms::Instance().Menus->MenuFood != NULL)
   {
		fNonFatialMenuEnableDisableNumber = TDeviceRealPalms::Instance().Menus->MenuFood->EnabledStateSync;
	}
   else
   {
		fNonFatialMenuEnableDisableNumber = -1;
   }

   if(TDeviceRealPalms::Instance().Menus->MenuDrinks != NULL)
   {
   	fNonFatialDrinksEnableDisableNumber = TDeviceRealPalms::Instance().Menus->MenuDrinks->EnabledStateSync;
	}
   else
   {
	   fNonFatialDrinksEnableDisableNumber = -1;
   }
}

void TPalm::resetMenuEnableState()
{
	fNonFatialMenuEnableDisableNumber = -1;
   fNonFatialDrinksEnableDisableNumber = -1;
}

void TDeviceRealPalms::SavePalms(Database::TDBTransaction &DBTransaction)
{
   // Save of all Palms that arnt in list all ready.
	for(int i = 0 ; i < PalmList->Count ; i++)
   {
		TPalm *Ptr = (TPalm *)PalmList->Items[i];
		Ptr->Create(DBTransaction);
   }
}

void TDeviceRealPalms::LoadPalms(Database::TDBTransaction &DBTransaction)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT DEVICE_KEY FROM DEVICES WHERE DEVICES.DEVICE_TYPE = " + IntToStr(devPalm)+ " ORDER BY DEVICE_NAME";
		IBInternalQuery->ExecQuery();
		for(;!IBInternalQuery->Eof;IBInternalQuery->Next())// Removes all palms from database.
      {
         TPalm *Palm = new TPalm();
			Palm->ID.DeviceKey = IBInternalQuery->FieldByName("DEVICE_KEY")->AsInteger;
			Palm->Load(DBTransaction);
         PalmList->Add(Palm);
      }
	}
	catch(Exception &E)
	{
	}
}


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Creates and new tables that are not found in the database.
// the pointer to the packet is held in the LParam in the TMessage.
void TPalm::CallAways(Database::TDBTransaction &DBTransaction)
{
	TIRPacket *Packet = Inbound;
   try
   {
      // Order Processing Only process non splits..
      Packet->CallAways.Tables->Pack();

      while( Packet->CallAways.Tables->Count !=0)
      {
         TIRTable *TempTable = (TIRTable *)Packet->CallAways.Tables->Items[0];
			TCallAwayComplete *CallAway = BuildCallAwayProper(DBTransaction,TempTable);

			TReqPrintJob *NormalRequest = new TReqPrintJob(&TDeviceRealTerminal::Instance());
         NormalRequest->SenderType = devPalm;
         NormalRequest->TimeStamp = Now();
			NormalRequest->Waiter = this->User.Name;

			if (!Kitchen->GetPrintouts(DBTransaction,CallAway,NormalRequest))
			{
				Status.Error = Wrn_Callaway_Printer_Not_Setup;//Err_No_Printer_Found;
			}
			if (!NormalRequest->Printouts->Print(devPalm))
			{
				Status.Error = Wrn_Printer_Offline;
			}
			delete NormalRequest;
         NormalRequest = NULL;
			delete CallAway;
         CallAway = NULL;
         delete (TIRTable *)TempTable;
			Packet->CallAways.Tables->Delete(0);

			if (Status.Error != proA_Ok)
			{
				BuildPacket(NULL);
			}
		}
   }
   __finally
   {
		while( Packet->CallAways.Tables->Count !=0)
      {
         delete (TIRTable *)Packet->CallAways.Tables->Items[0];
         Packet->CallAways.Tables->Delete(0);
      }
   }
}

void __fastcall TPalm::NewOrders(Database::TDBTransaction &DBTransaction)
{
	TIRPacket *Packet = Inbound;
   Packet->OrderCount = 1; // Stops any chance of a devide error.
   // Look for any printer before processing the Job.
	// You have to do this here as a quik check so the
   // Job is not added to the Database because. Even if TKitchen::Prepare
   // detects that there is no printers and throws the error that warns the
   // user, the user can just beem the palm again which causes a Get_Orders
	// which will pull the order from the database and put a tick next to it.
   // By checking here we stop the order being added to the DB.
   TList *OrdersList;
	typedef std::map<long,long> TSetMenuGroupNoLookup; // Stephen "Random" Number / My SetMenuGroup Key.
   TSetMenuGroupNoLookup TempConsumedPalmGroupIDs;
   //-------- Strip Orders form IR packets into Orders Packet ----------------
   try
   {
      if(Packet->Header.Command == REQUEST_RECEIPT) // Billing section of New Orders.
      {
      }
		else
      {
			OrdersList  = new TList;
         // Order Processing Only process non splits..
         Packet->Orders.Tables->Pack();
			while( Packet->Orders.Tables->Count !=0)
         {
            // Check for Tables.
				TIRTable *TempTable = (TIRTable *)Packet->Orders.Tables->Items[0];
            TempTable->Seats->Pack();
            while( TempTable->Seats->Count != 0 )
            {
					int LastItemID = -1;
               TIRSeat *TempSeat  = ((TIRSeat *)TempTable->Seats->Items[0]);
               // Helps Uniquly ID two of the Same dish within the same course.
					int DishCount = 0;
               TempSeat->Orders->Pack();
               while(TempSeat->Orders->Count != 0)
               {
						TIROrder *TempOrder = (TIROrder *)TempSeat->Orders->Items[0];
						TItemComplete *Order =  this->BuildOrderProper(TempOrder,Now() - (double(TempSeat->PalmTime)/86400));
                  if(Order != NULL)
                  {
							Order->TableNo   = (WORD)TempTable->Table;
							Order->SeatNo         = TempSeat->Seat;
							Order->TabContainerName = TDBTables::GetTableName(DBTransaction,Order->TableNo);
                     ManagerDiscount->AddDiscountsByTime(DBTransaction,Order);
							// Sort out Set Menu Grouping Numbers.
							if((TempOrder->SetMenuGroup != 0) && TempOrder->SetMenuGroupIsIndex)
							{ //
								int SeatKey  = TDBTables::GetOrCreateSeat(DBTransaction,TempTable->Table,TempSeat->Seat);
								Order->TabKey = TDBTab::GetOrCreateTab(DBTransaction,TDBTables::GetTabKey(DBTransaction,SeatKey));
								TDBTables::SetSeatTab(DBTransaction,SeatKey,Order->TabKey);
								TDBTab::SetTabType(DBTransaction,Order->TabKey, TabTableSeat);
								TDBTab::SetTabName(DBTransaction,Order->TabKey,TempSeat->Name);
								Order->PartyName = TDBTables::GetPartyName(DBTransaction,Order->TableNo);

								TTabsSetMenuList::iterator CurrentTabsSetMenus = TDeviceRealPalms::Instance().TabsSetMenuList.find(Order->TabKey);
								if(CurrentTabsSetMenus != TDeviceRealPalms::Instance().TabsSetMenuList.end())
								{
								// Lookup the SetMenu Group No based on this Index.
									TSetMenuList::iterator SetMenuNo = CurrentTabsSetMenus->second.begin();
									std::advance(SetMenuNo,TempOrder->SetMenuGroup -1); // zero based.
									Order->SetMenuGroup = *SetMenuNo;
								}  // Tab should always be found if its a new Tab or Set Menu then
									// it would not be indexed based. (The rebuild function will pick
									// up any tabs that are not in TabsSetMenuList and add them whe
									// the orders are sent )
							}
							else if (TempOrder->SetMenuGroup != 0)
							{  // Number was generated from the palm.
								int SeatKey  = TDBTables::GetOrCreateSeat(DBTransaction,TempTable->Table,TempSeat->Seat);
								Order->TabKey = TDBTab::GetOrCreateTab(DBTransaction,TDBTables::GetTabKey(DBTransaction,SeatKey));
								TDBTables::SetSeatTab(DBTransaction,SeatKey,Order->TabKey);
								TDBTab::SetTabType(DBTransaction,Order->TabKey, TabTableSeat);
								TDBTab::SetTabName(DBTransaction,Order->TabKey,TempSeat->Name);
								Order->PartyName = TDBTables::GetPartyName(DBTransaction,Order->TableNo);

								TTabsSetMenuList::iterator CurrentTabsSetMenus = TDeviceRealPalms::Instance().TabsSetMenuList.find(Order->TabKey);
								if(CurrentTabsSetMenus != TDeviceRealPalms::Instance().TabsSetMenuList.end())
								{  // Tab already exists and is in List.
									// Have I may already Generated this translation for an eairlier order.
//									TSetMenuList::iterator SetMenuNumber = CurrentTabsSetMenus->second.begin();

									// Does the Used GroupNo Vector already contain this Group Number?
									TSetMenuGroupNoLookup::iterator LookupGroupNumber = TempConsumedPalmGroupIDs.find(TempOrder->SetMenuGroup);
									if(LookupGroupNumber != TempConsumedPalmGroupIDs.end())
									{
										Order->SetMenuGroup = LookupGroupNumber->second;
									}
									else
									{// Get the next Set Menu key from the Generator and use that.
										int NewSetMenuGroupingNumber = TDBOrder::GetSetMenuGroupNumber(DBTransaction);

										// Add the New Key to it.
										CurrentTabsSetMenus->second.push_back(NewSetMenuGroupingNumber);
										TempConsumedPalmGroupIDs[TempOrder->SetMenuGroup] = NewSetMenuGroupingNumber;
										Order->SetMenuGroup = NewSetMenuGroupingNumber;
									}
								}
								else
								{  // New table / seat -> tab with setmenu stuff.
									int NewSetMenuGroupingNumber = TDBOrder::GetSetMenuGroupNumber(DBTransaction);

									// Add Tab to List.
									TempConsumedPalmGroupIDs[TempOrder->SetMenuGroup] = NewSetMenuGroupingNumber;
									TDeviceRealPalms::Instance().TabsSetMenuList[Order->TabKey].push_back(NewSetMenuGroupingNumber);
									Order->SetMenuGroup = NewSetMenuGroupingNumber;
								}
							}

							if(Order->SetMenu && !Order->SetMenuMaster && Order->SetMenuGroup != 0)
							{
								Order->SetPriceLevelCustom(0);
							}


							if(Order->Palm_ID == LastItemID)
							{
								DishCount++;
							}
							else
							{
								DishCount = 0;
							}

							unsigned long TransTable = TempTable->Table & 0xFF;
							unsigned long TransSeat = TempSeat->Seat & 0xFF;
							unsigned long TransNumber = Packet->Header.TransNo & 0xFF;
							unsigned long TransDishCount = DishCount & 0xFF;
							Order->TransNo = TransDishCount + (TransNumber << 8)
												  + (TransSeat << 16) + (TransTable << 24);
							LastItemID = Order->Palm_ID;

							// --------- Store Order ----------
							OrdersList->Add(Order);
//                     CurrentTables.insert(Order->TableNo);
						}
						else
						{
							// Failed to Locate Order Remove all Orders and quit.
							this->Status.Error = Err_Database;
							while(TempSeat->Orders->Count != 0)
							{
								delete (TIROrder *)TempOrder;
								TempSeat->Orders->Delete(0);
							}
							while(OrdersList->Count != 0)
							{
								TItemComplete *Order = (TItemComplete *)OrdersList->Items[0];
								delete Order;
								OrdersList->Delete(0);
							}
						}

						if(TempSeat->Orders->Count > 0) // All order could have been removed due to error.
						{
							delete (TIROrder *)TempOrder;
							TempSeat->Orders->Delete(0);
						}
					}

					//-------------- Send Seat Names to other Palms--------------
					if(TempSeat->NameChange)
					{
						for(int i=0; i<TDeviceRealPalms::Instance().PalmList->Count; i++)
						{
							TPalm *Palm = (TPalm *)TDeviceRealPalms::Instance().PalmList->Items[i];

							if(Palm->ID.DeviceKey == ID.DeviceKey)
							{
							// Current palm downloaded a new name.Check if there is an old name to be uploaded.
/*								TPalmSeatName *ptrName = TempPalm->GetSeatName(TempTable->Table,TempSeat->Seat);
								if(ptrName)
								{
									// Update the name.
									ptrName->SeatName = TempSeat->Name;
								}*/
//								SeatNames.SetName(TempTable->Table, TempSeat->Seat, TempSeat->Name); // might not be necessary?
								//Alter the Name in the DB.
								//frmData->SetSeatName(TempSeat->Name, TempTable->Table, TempSeat->Seat); //Should use this
								int SeatKey  = TDBTables::GetOrCreateSeat(DBTransaction,TempTable->Table,TempSeat->Seat);
								int TabKey = TDBTab::GetOrCreateTab(DBTransaction,TDBTables::GetTabKey(DBTransaction,SeatKey));
								TDBTables::SetSeatTab(DBTransaction,SeatKey, TabKey);
								TDBTab::SetTabType(DBTransaction,TabKey, TabTableSeat);
								TDBTab::SetTabName(DBTransaction,TabKey, TempSeat->Name);
							}
						}
					}
/*					if(TempSeat->NameChange)
					{
						TPalm *TempPalm;
						for(int i = 0; i < TDeviceRealPalms::Instance().PalmList->Count; i++)
						{
							TempPalm = (TPalm *)TDeviceRealPalms::Instance().PalmList->Items[i];
							if(TempPalm->ID.DeviceKey != this->ID.DeviceKey)
							{
								if(TempPalm->Status.Active)
								{
									// Check a name for this table/Seat does not exist...
									TPalmSeatName *ptrName = TempPalm->GetSeatName(TempTable->Table,TempSeat->Seat);
									if(ptrName)
									{
										// Update the name.
										ptrName->SeatName = TempSeat->Name;
									}
									else
									{
										 ptrName = new TPalmSeatName(TempTable->Table,TempSeat->Seat,TempSeat->Name);
										 TempPalm->SeatNames->Add(ptrName);
                           }
                        }
                     }
							else  // Current palm downloaded a new name.Check if there is an old name to be uploaded.
                     {
                        TPalmSeatName *ptrName = TempPalm->GetSeatName(TempTable->Table,TempSeat->Seat);
                        if(ptrName)
								{
									// Update the name.
                           ptrName->SeatName = TempSeat->Name;
                        }
                        //Alter the Name in the DB.
								//frmData->SetSeatName(TempSeat->Name, TempTable->Table, TempSeat->Seat); //Should use this
								int SeatKey  = TDBTables::GetOrCreateSeat(TempTable->Table,TempSeat->Seat);
								int TabKey = TDBTab::GetOrCreateTab(TDBTables::GetTabKey(SeatKey));
								TDBTables::SetSeatTab(SeatKey,TabKey);
								TDBTab::SetTabType(TabKey, TabTableSeat);
								TDBTab::SetTabName(TabKey,TempSeat->Name);
							}
                  }
					}*/
					delete (TIRSeat *)TempSeat;
					TempTable->Seats->Delete(0);
				}
				delete (TIRTable *)TempTable;
				Packet->Orders.Tables->Delete(0);
			}

			if(this->Status.Error == proA_Ok)
			{
				// ------------- Process orders ----------------

            for(int o = 0; o < OrdersList->Count; o++)
            {
               TItemComplete *Order = (TItemComplete *)OrdersList->Items[o];
               if(Order->ServingCourse.ServingCourseKey < 1)
               {
                  Order->ServingCourse = TDeviceRealPalms::Instance().Menus->DefaultServingCourse;
                  TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Order has no Serving Course" );
               }
            }

				TDBOrder::ProcessOrders(DBTransaction,OrdersList);

				TProcessSecurityQueries queries(DBTransaction);

				for(int i=0; i<OrdersList->Count; i++)
				{
					TItemComplete *Order = (TItemComplete *)OrdersList->Items[i];
					TDBSecurity::ProcessSecurity(queries, Order->Security);
					for (int j=0; j<Order->SubOrders->Count; j++)
					{
						TItemCompleteSub *SubOrder = Order->SubOrders->SubOrderGet(j);
						if(SubOrder)
						{
							TDBSecurity::ProcessSecurity(queries, SubOrder->Security);
						}
					}
				}
				TDeviceRealPalms::Instance().ManagerStock->UpdateStock(DBTransaction,OrdersList);
			}

			if(this->Status.Error == proA_Ok)
			{
				// Print the Orders.
				TReqPrintJob *NormalRequest = new TReqPrintJob(&TDeviceRealPalms::Instance());
				TPaymentTransaction *NormalTransaction = new TPaymentTransaction(DBTransaction);
				NormalRequest->Transaction = NormalTransaction;
				NormalRequest->SenderType = devPalm;
				NormalRequest->TimeStamp = Now();
				TReqPrintJob *CreditRequest = new TReqPrintJob(&TDeviceRealPalms::Instance());
				TPaymentTransaction *CreditTransaction = new TPaymentTransaction(DBTransaction);
				CreditRequest->Transaction = CreditTransaction;

				CreditRequest->SenderType = devPalm;
				CreditRequest->TimeStamp = Now();
				for(int y = 0; y < OrdersList->Count; y++)
				{
					TItemComplete *Order = (TItemComplete *)OrdersList->Items[y];
					if(Order->OrderType != CanceledOrder && Order->DuplicateDetected == false)   // Credits are printed before they are removed.
					{
						NormalRequest->Waiter = this->User.Name;
						NormalTransaction->Orders->Add(Order);
						NormalRequest->OrderLocalKeys.insert(Order->OrderKey);
						if(Order->TimeStamp < NormalRequest->TimeStamp)
						{
							NormalRequest->TimeStamp = Order->TimeStamp;
						}
					}
					else if (Order->DuplicateDetected == false)
					{
						CreditRequest->Waiter = this->User.Name;
						CreditTransaction->Orders->Add(Order);
						CreditRequest->OrderLocalKeys.insert(Order->OrderKey);
						if(Order->TimeStamp < CreditRequest->TimeStamp)
						{
							CreditRequest->TimeStamp = Order->TimeStamp;
						}
					}
				}

				if(NormalTransaction->Orders->Count != 0)
				{
					Kitchen->GetPrintouts(DBTransaction,NormalRequest);
					if (!NormalRequest->Printouts->Print(devPalm))
					{
						Status.Error = Wrn_Printer_Offline;
					}
					ManagerDockets->Archive(NormalRequest);
										
					if(NormalRequest->Header.Error != proA_Ok)
					{
						this->Status.Error = NormalRequest->Header.Error;
					}
				}

				if(CreditTransaction->Orders->Count != 0)
				{
					Kitchen->GetPrintouts(DBTransaction,CreditRequest);
					if (!CreditRequest->Printouts->Print(devPalm))
					{
						Status.Error = Wrn_Printer_Offline;
					}
					if(CreditRequest->Header.Error != proA_Ok)
					{
						this->Status.Error = CreditRequest->Header.Error;
					}
				}

//				ProcessKitchenModFinal(Database::TDBTransaction &DBTransaction,OrdersList);

				delete NormalRequest;
				delete CreditRequest;
				delete NormalTransaction;
				delete CreditTransaction;
			}

			//------------- free order memory ---------------
			//Record the number of orders for packet times.
			Packet->OrderCount = OrdersList->Count;
			while(OrdersList->Count != 0)
			{
				TItemComplete *Order = (TItemComplete *)OrdersList->Items[0];
				delete Order;
				OrdersList->Delete(0);
			}
			delete OrdersList;
		}
	}
	catch(Exception &Err)
	{
		while(OrdersList->Count != 0)
		{
			TItemComplete *Order = (TItemComplete *)OrdersList->Items[0];
			delete Order;
			OrdersList->Delete(0);
		}
		delete OrdersList;
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"SyncThread::NewOrder " + Err.Message);
		throw;
	}
}

//DM_NEWORDER-------------------------------------------------------------------
void __fastcall TPalm::Login(Database::TDBTransaction &DBTransaction)
{
   // Retrieve Staff ID form Packet.
   // Search DB.
	int NewLineChar = 10;

	if(this->ID.IP != Inbound->Login.DeviceIP)
	{
		Status.Error = Err_DeviceKey_Not_Found;
	}
	else if(this->Locate(DBTransaction))
	{
		// Name was locateded ID matched or updated proceed.
		TMMContactInfo TempUserInfo;
		TempUserInfo.ContactKey = Inbound->Login.NewStaffID;
		TempUserInfo.PIN = Inbound->Login.UsersPIN;

		if(Inbound->Login.NewStaffID != 0)
		{
			TLoginSuccess Result = lsPINIncorrect;
			if (TempUserInfo.PIN != "")
			{
				std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
				Result = Staff->Login(NULL,DBTransaction,TempUserInfo, CheckPOS);
			}
         if (Result == lsAccepted)
         {
				User								= TempUserInfo;
//				DownloadOrders					= (DownloadStatus)TempUserInfo.PalmDownloadType;

//				LogoffTime						= TempUserInfo.PalmTimeOut;
//				UserSettings					= TempUserInfo.UserSettings;
//				UserSettings.AdvanceUser	= TempUserInfo.PalmAdvancedUser;
//				UserSettings.AutoPopup		= TempUserInfo.PalmAutoOptions;
//				UserSettings.LargeFont		= TempUserInfo.PalmLargeFont;

				// Compile Response.
            // Add it to the Palms Transmission list.
            TMemoryStream *UserInfo = new TMemoryStream;
            try
            {
					UserInfo->Clear();
					UnicodeString OutData;

               // Insert User Information.
					OutData = "[User]";
					UserInfo->Write(OutData.t_str(), OutData.Length());
               UserInfo->Write(&NewLineChar,1); // newline character.

					//TextVerion of Trans Number.
					OutData = IntToStr(Status.TransactionNumber);
					UserInfo->Write(OutData.t_str(), OutData.Length());
               UserInfo->Write(&NewLineChar,1); // newline character.

//					if (User.PalmTimeOut != 0)
//					{
						OutData = IntToStr(User.PalmTimeOut);
						UserInfo->Write(OutData.t_str(), OutData.Length());
						UserInfo->Write(&NewLineChar,1); // newline character.
//					}
/*               if(LogoffTime != 0)
					{
						Enabled = IntToStr(LogoffTime);
						UserInfo->Write(Enabled.c_str(),Enabled.Length()); // newline character.
						UserInfo->Write(&NewLineChar,1); // newline character.
					}
*/
					OutData = TGlobalSettings::Instance().RememberLastServingCourse?"1":"0";	// Default Serving Course selection mode: 0 = Force popup, 1 = As previous
					UserInfo->Write(OutData.t_str(), OutData.Length());
					UserInfo->Write(&NewLineChar,1); // newline character.

					OutData = IntToStr(User.PalmUserSettings);
					UserInfo->Write(OutData.t_str(), OutData.Length());
               UserInfo->Write(&NewLineChar, 1); // newline character.

/*               if(AdvanceUser)
					{
						Enabled = "1";
						UserInfo->Write(Enabled.c_str(),Enabled.Length()); // newline character.
						UserInfo->Write(&sep,sizeof(sep));
					}

					if(Staff->TestAccessLevel(TempUserInfo, CheckPriceAj))
               {
                  Enabled = "2";
						UserInfo->Write(Enabled.c_str(),Enabled.Length()); // newline character.
                  UserInfo->Write(&sep,sizeof(sep));
					}

               if(AutoPopup)
               {
                  Enabled = "3";
                  UserInfo->Write(Enabled.c_str(),Enabled.Length()); // newline character.
                  UserInfo->Write(&sep,sizeof(sep));
               }

               if(LargeFont)
               {
                  Enabled = "4";
                  UserInfo->Write(Enabled.c_str(),Enabled.Length()); // newline character.
                  UserInfo->Write(&sep,sizeof(sep));
					}*/
               UserInfo->Write(&NewLineChar, 1); // newline character.
               this->BuildPacket(UserInfo);
            }
            __finally
            {
               delete UserInfo;
            }

//            CurrentTables.clear();
//				SeatNames.Load();
//            this->UpdateNames();
			}
			else if(Result == lsUserNotFound)
			{
				TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,Now().FormatString("dd-mm-yy hh-mm") + " Login Failed Palm : " + IntToStr(ID.DeviceKey)
							+ " Current User ID: " + IntToStr(User.ContactID)
							+ " Requested User ID: " + IntToStr(Inbound->Login.NewStaffID) + " User Not Found");

				Status.Error = Err_User_Not_Found;
				Status.ErrorMsg = "User not found";
			}
			else if(Result == lsPINIncorrect)
			{
				TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,Now().FormatString("dd-mm-yy hh-mm") + " Login Failed Palm : " + IntToStr(ID.DeviceKey)
							+ " Current User ID: " + IntToStr(User.ContactID)
							+ " Requested User ID: " + IntToStr(Inbound->Login.NewStaffID) + " PIN Incorrect");

				Status.Error = Err_PIN_Incorrect;
				Status.ErrorMsg = "Pin Incorrect";
			}
		}
		else
		{
			TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,Now().FormatString("dd-mm-yy hh-mm") + " Login Failed Palm : " + IntToStr(ID.DeviceKey)
						+ " Current User ID: " + IntToStr(User.ContactID)
						+ " Requested User ID: " + IntToStr(Inbound->Login.NewStaffID) + " User Not Found");

			Status.Error = Err_User_Not_Found;
			Status.ErrorMsg = "User not found";
		}
	}
}


//------------------------------------------------------------------------------


void TPalm::BillSeats(Database::TDBTransaction &DBTransaction)
{
	TIRTable *Table;
	TIRSeat *Seat;
	TIRPacket *Packet = this->Inbound;
	try
   {
		Packet->Billing.Tables->Pack();
      for(int i = 0 ; (i < Packet->Billing.Tables->Count) && (this->Status.Error == proA_Ok); i++)
      {
         // Retrieve the Table Number.
			Table = (TIRTable *)Packet->Billing.Tables->Items[i];
			// Loop though the lists of seats pulling in the correct Tabs.
			Table->Seats->Pack();

			std::set<__int64> SelectedTabs;
			for(int j = 0 ;j < Table->Seats->Count; j++)
			{
            Seat = (TIRSeat *)Table->Seats->Items[j];
            TTableSeat TableSeat;
				TableSeat.TableNo = Table->Table;
				TableSeat.SeatNo = Seat->Seat;
				long TabKey = TDBTables::GetTabKey(DBTransaction,TableSeat.TableNo,TableSeat.SeatNo);
            if(TabKey != 0)
            {
               SelectedTabs.insert(TabKey);
            }
         }

			TReqPrintJob *Request = new TReqPrintJob(this);
			TPaymentTransaction NormalTransaction(DBTransaction);
			Request->Transaction = &NormalTransaction;
			try
         {
				TDBOrder::GetOrdersFromTabKeys(DBTransaction,NormalTransaction.Orders,SelectedTabs);
            NormalTransaction.Money.CreditAvailable = TDBOrder::LoadCreditFromOrders(DBTransaction,NormalTransaction.Orders);
        		NormalTransaction.Money.Recalc(NormalTransaction);

				Request->SenderType = devPalm;
				Request->PaymentType = ptPreliminary;//Packet->FinalBill;
				Request->Waiter = this->User.Name;

				if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
            {
					this->Status.Error = Wrn_Printer_Offline;
               break;
            }
            else
				{
					Receipt->GetPrintouts(DBTransaction,Request,TComms::Instance().ReceiptPrinter,eDispBCOff);
					if (!Request->Printouts->Print(devPalm))
					{
						Status.Error = Wrn_Printer_Offline;
					}
				}

				if(Request->Header.Error != proA_Ok)
            {
               this->Status.Error = Request->Header.Error;
				}
         }
         __finally
         {
				while(NormalTransaction.Orders->Count != 0)
            {
					delete (TItemComplete *)NormalTransaction.Orders->Items[0];
					NormalTransaction.Orders->Delete(0);
				}
            delete Request;
         }
      }
	}// End Try.
   catch(Exception &Err)
   {
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG, "SyncThread::BillSeats " + Err.Message);
      throw;
   }
}

//---------------------------------------------------------------------------

bool __fastcall TPalm::CheckCRC(unsigned short int CRC)
{
	TIRPacket * Packet = this->Inbound;
   // Cal what CRC should be.
   unsigned short oldCRC = 0;//(unsigned short int)
	oldCRC = (unsigned short) StartCRC(Packet->Header.Command);
	oldCRC = (unsigned short)UpdateCRC(oldCRC,Packet->Header.TransNo);
	oldCRC = (unsigned short)UpdateCRC(oldCRC,Packet->Header.InfoSyncNo);
	oldCRC = (unsigned short)UpdateCRC(oldCRC,Packet->Header.MenuSync1No);
	oldCRC = (unsigned short)UpdateCRC(oldCRC,Packet->Header.MenuSync2No);
	oldCRC = (unsigned short)UpdateCRC(oldCRC,Packet->Header.HighStaffIDNo);
	oldCRC = (unsigned short)UpdateCRC(oldCRC,Packet->Header.LowStaffIDNo);
	oldCRC = (unsigned short)UpdateCRC(oldCRC,Packet->Header.ErrorNo);
	oldCRC = (unsigned short)UpdateCRC(oldCRC,Packet->Header.HighPktSize);
	oldCRC = (unsigned short)UpdateCRC(oldCRC,Packet->Header.LowPktSize);
	for(int i=0;i<Packet->PktSize;i++)
	{
		oldCRC = (unsigned short)UpdateCRC(oldCRC,Packet->ptrBuffer[i]);
	}

	if(oldCRC != CRC)
	{
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Invalid CRC. Calculated : " + IntToStr(oldCRC) + " Sent : " + IntToStr(CRC));
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Invalid CRC. Calculated : " + IntToStr(oldCRC) + " Sent : " + IntToStr(CRC));
		this->Status.Error = Err_Msg_CRC;
	}
	else
	{
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------

bool __fastcall TPalm::CheckSyncNo()
{
   unsigned char FoodMenuSync, DrinkMenuSync;
   if(TDeviceRealPalms::Instance().Menus->MenuFood == NULL && this->Inbound->Header.MenuSync1No != 0 )
   {
      FoodMenuSync = 0;
		this->Status.Error = Err_MenuOutOfSync;
		TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"Menu Out Of Sync, Food Menu NULL While Palm Sync not 0");
   }
   else if(TDeviceRealPalms::Instance().Menus->MenuFood == NULL && this->Inbound->Header.MenuSync1No == 0)
   {
      FoodMenuSync = 0;
	}
   else
   {
      FoodMenuSync = TDeviceRealPalms::Instance().Menus->MenuFood->MenuKey;
   }

   if(TDeviceRealPalms::Instance().Menus->MenuDrinks == NULL && this->Inbound->Header.MenuSync2No != 0)
   {
		DrinkMenuSync = 0;
		this->Status.Error = Err_DrinkOutOfSync;
		TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"Menu Out Of Sync, Food Menu NULL While Palm Sync not 0");
   }
   else if(TDeviceRealPalms::Instance().Menus->MenuDrinks == NULL && this->Inbound->Header.MenuSync2No == 0)
   {
      DrinkMenuSync = 0;
   }
   else
   {
      DrinkMenuSync = TDeviceRealPalms::Instance().Menus->MenuDrinks->MenuKey;
   }

   if(this->Inbound->Header.MenuSync1No != FoodMenuSync && FoodMenuSync != 0)
   {
		TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"Menu Sync # Failure. Palm :" + IntToStr(this->Inbound->Header.MenuSync1No) + " PC :" + IntToStr(FoodMenuSync));
		this->Status.Error = Err_MenuOutOfSync;
   }
   else if(this->Inbound->Header.MenuSync2No != DrinkMenuSync && DrinkMenuSync != 0)
   {
		TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"Drink Sync # Failure. Palm :" + IntToStr(this->Inbound->Header.MenuSync2No) + " PC :" + IntToStr(DrinkMenuSync));
		this->Status.Error = Err_DrinkOutOfSync;
   }
   else if(this->Inbound->Header.InfoSyncNo != TGlobalSettings::Instance().InfoSync)
	{
		TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"Info Sync # Failure. Palm :" + IntToStr(this->Inbound->Header.InfoSyncNo) + " PC :" + IntToStr(TGlobalSettings::Instance().InfoSync));
		this->Status.Error = Err_InfoOutOfSync;
   }
   else
	{
		return true;
   }
   return false;
}
//------------------------------------------------------------------------------
void __fastcall TPalm::RepackageOrders(Database::TDBTransaction &DBTransaction,TMemoryStream *Process_Outbound, int LastConnectVar)
{
	TStringList *Log = NULL;
	std::auto_ptr<TStringList> LogAutoPtr (new TStringList);
	Log = LogAutoPtr.get();

	unsigned char	SectionFlag		= 0x00;
	int				PosSectionFlag	= Process_Outbound->Position;

	// Place holder for section flag
	Process_Outbound->Write(&SectionFlag, 1);

	// Look for disabled items
	std::vector<unsigned short> DisablesItemsList;
	int DisabledItemCount = fLoadDisabledItems(DisablesItemsList, SectionFlag);
	for (int i=0; i<DisabledItemCount; i++)
	{
		fWriteStreamShort(Process_Outbound, DisablesItemsList[i], "Disable Item ID", Log, i == DisabledItemCount-1);
	}
	// Load seat names
	TPalmSeatNames SeatNames;
	if (User.PalmUserSettings.GetAllTables)
	{
		SeatNames.Load(DBTransaction);
	}
	else
	{
		SeatNames.Load(DBTransaction, CurrentTable);
	}
	int tableCount	= SeatNames.TableCount();
	if (tableCount > 0)
	{
		SectionFlag |= ROSectionFlagContainsSeatName;

		for (int tableIndex=0; tableIndex<tableCount; ++tableIndex)
		{
			TTableNumber tableNumber = SeatNames.Table(tableIndex).TableNumber;
			fWriteStreamChar(Process_Outbound, tableNumber, "Table #", Log, tableIndex == tableCount-1);

			int seatCount = SeatNames.Table(tableIndex).SeatCount();
			for (int seatIndex=0; seatIndex<seatCount; ++seatIndex)
			{
				TSeatNumber		seatNumber		= SeatNames.Table(tableIndex).Seat(seatIndex).SeatNumber;
				UnicodeString		seatName			= SeatNames.Table(tableIndex).Seat(seatIndex).SeatName;

				fWriteStreamChar(Process_Outbound, seatNumber, "Seat #", Log, seatIndex == seatCount-1);
				fWriteStreamString(Process_Outbound, seatName, "Seat Name", Log);
			}
		}
	}
	if(TGlobalSettings::Instance().InHappyHour)
	{
		SectionFlag |= PalmSectionIsInHappyHour;
	}
	// Load orders
//	TMemoryStream *NewStream = Process_Outbound;
//		#if (DEBUGLEVEL & debugExpandGetOrdes)
//		std::auto_ptr<TMemoryStream> NewStreamAutoPtr (new TMemoryStream);
/*		NewStream = Process_Outbound;//NewStreamAutoPtr.get();
		NewStream->CopyFrom(Process_Outbound, 0); */
//		#endif

	TPalm::TPalmTables PalmTables;
	fLoadPalmOrders(DBTransaction, PalmTables);
	fStreamOrders(PalmTables, Process_Outbound, Log, SectionFlag);

	// Load messages
	if (!Messages.empty())
	{
		SectionFlag |= ROSectionFlagContainsMessage;
		// Set this Message as sent.
		TPalmMessage &CurrentMessage = Messages[0];
		CurrentMessage.Sent = true;

		if(CurrentMessage.From == "")
		{
			CurrentMessage.From == "Unknown";
		}
		fWriteStreamString(Process_Outbound, CurrentMessage.From, "Message From", Log);
		fWriteStreamString(Process_Outbound, CurrentMessage.Message, "Message", Log);
	}
	// Set the section flag
	static_cast<char *>(Process_Outbound->Memory)[PosSectionFlag] = SectionFlag;
	Log->Insert(0, "Section Flag:" + UnicodeString::StringOfChar(' ', 20 - 12) + IntToHex(static_cast<__int64>(SectionFlag), 2));

	// Write logs out if necessary
	if (LogGetOrders && Process_Outbound->Size > 0)
	{
		std::auto_ptr<TStringList> NewText (new TStringList);
		char *Memory = static_cast<char *>(Process_Outbound->Memory);
		for (int i=0; i<Process_Outbound->Size; i++)
		{
			NewText->Add(IntToHex(static_cast<__int64>(static_cast<unsigned char>(Memory[i])), 2));
		}
		UnicodeString DateString = Now().FormatString("dd-mm-yy hh-nn-ss");
		Log->SaveToFile("OrdersStream" + DateString + ".txt");
		NewText->SaveToFile("PalmGetOrders" + DateString + ".txt");
	}
}

//---------------------------------------------------------------------------
int TPalm::fLoadDisabledItems(std::vector<unsigned short> &DisablesItemsList, unsigned char &SectionFlag)
{
	if (getMenuEnabledStateChange())
	{
		SectionFlag |= ROSectionFlagEnableAllDishes; // State has chage set to refresh palm.
		for(int i=0; i<TDeviceRealPalms::Instance().Menus->MenuFood->Count; i++)
		{
			TListCourse *Course = TDeviceRealPalms::Instance().Menus->MenuFood->CourseGet(i);
			if(Course->ViewableLocations & LocPalm)
			{
				for(int i=0; i<Course->Count; i++)
				{
					TItem *Item = Course->ItemGet(i);
					if(Item->Enabled == false)
					{
						DisablesItemsList.push_back(Item->Palm_ID);
					}
				}
			}
		}
	}
	if (getDrinksEnabledStateChange())
	{
		SectionFlag |= ROSectionFlagEnableAllDrinks;
		for (int i=0; i<TDeviceRealPalms::Instance().Menus->MenuDrinks->Count; i++)
		{
			TListCourse *Course = TDeviceRealPalms::Instance().Menus->MenuDrinks->CourseGet(i);
			if(Course->ViewableLocations & LocPalm)
			{
				for(int i=0; i<Course->Count; i++)
				{
					TItem *Item = Course->ItemGet(i);
					if(Item->Enabled == false)
					{
						DisablesItemsList.push_back(Item->Palm_ID);
					}
				}
			}
		}
	}
	int Size = DisablesItemsList.size();
	if (Size > 0)
	{
		SectionFlag |= ROSectionFlagHasDisabledItems; // State has chage set to refresh palm.
	}
	return Size;
}
//---------------------------------------------------------------------------
void TPalm::fLoadPalmOrders(Database::TDBTransaction &DBTransaction,TPalm::TPalmTables &PalmTables)
{
	TIBSQL *qrOrders		= DBTransaction.Query(DBTransaction.AddQuery());
	TIBSQL *qrSubOrders	= DBTransaction.Query(DBTransaction.AddQuery());

	qrOrders->SQL->Text =
		"Select "
			"Tables.Table_Number,"
			"Seat.SeatNo,"
			"MasterOrder.Tab_Key,"

			"MasterOrder.Order_Key,"
			"MasterOrder.Menu_Name,"
			"MasterOrder.Course_Name,"
			"MasterOrder.Item_Name,"
			"MasterOrder.Size_Name,"
			"MasterOrder.Price,"
			"MasterOrder.Discount,"
			"MasterOrder.Cost,"
			"MasterOrder.HappyHour,"
			"MasterOrder.SetMenu_Group,"
			"MasterOrder.Note,"
			"MasterOrder.Master_Container,"
			"MasterOrder.ServingCourses_Key,"

			"OrderOption.OrderOption_Key,"
			"OrderOption.Option_Name,"
			"OrderOption.Plus "

		"From "
			"Orders MasterOrder Inner Join Tab On "
				"MasterOrder.Tab_Key = Tab.Tab_Key "
			"Inner Join Seat On "
				"Tab.Tab_Key = Seat.Tab_Key "
			"Inner Join Tables "
				"On Seat.Table_Key = Tables.Table_Key "
			"Left Join OrderOption On "
				"OrderOption.Order_Key = MasterOrder.Order_Key "
		"Where "
			"MasterOrder.Order_Type = :Order_Type "
			"And MasterOrder.Side_Order_Key Is Null ";
	if (User.PalmUserSettings.GetAllTables)
	{
		qrOrders->SQL->Text = qrOrders->SQL->Text +
			"And Tab.Tab_Type = :Tab_Type ";
	}
	else
	{
		qrOrders->SQL->Text = qrOrders->SQL->Text +
			"And Tables.Table_Number = :Table_Number ";
	}
	qrOrders->SQL->Text = qrOrders->SQL->Text +
		"Order By "
			"Tables.Table_Number,"
			"Seat.SeatNo,"
			"MasterOrder.Order_Key,"
			"OrderOption.OrderOption_Key";

	qrSubOrders->SQL->Text =
		"Select "
			"MasterOrder.Order_Key,"
			"SubOrder.Order_Key Sub_Order_Key,"
			"SubOrder.Course_Name Sub_Course_Name,"
			"SubOrder.Item_Name Sub_Item_Name,"
			"SubOrder.Size_Name Sub_Size_Name,"
			"SubOrder.Price Sub_Price,"
			"SubOrder.Discount Sub_Discount "
		"From "
			"Orders MasterOrder Inner Join Tab On "
				"MasterOrder.Tab_Key = Tab.Tab_Key "
			"Inner Join Seat On "
				"Tab.Tab_Key = Seat.Tab_Key "
			"Inner Join Tables "
				"On Seat.Table_Key = Tables.Table_Key "
			"Inner Join Orders SubOrder On "
				"SubOrder.Side_Order_Key = MasterOrder.Order_Key "
		"Where "
			"MasterOrder.Order_Type = :Order_Type "
			"And MasterOrder.Side_Order_Key Is Null ";
	if (User.PalmUserSettings.GetAllTables)
	{
		qrSubOrders->SQL->Text = qrSubOrders->SQL->Text +
			"And Tab.Tab_Type = :Tab_Type ";
	}
	else
	{
		qrSubOrders->SQL->Text = qrSubOrders->SQL->Text +
			"And Tables.Table_Number = :Table_Number ";
	}
	qrSubOrders->SQL->Text = qrSubOrders->SQL->Text +
		"Order By "
			"Tables.Table_Number,"
			"Seat.SeatNo,"
			"MasterOrder.Order_Key,"
			"SubOrder.Order_Key";
	if (User.PalmUserSettings.GetAllTables)
	{
		qrOrders->ParamByName("Tab_Type")->AsInteger				= TabTableSeat;
		qrSubOrders->ParamByName("Tab_Type")->AsInteger			= TabTableSeat;
	}
	else
	{
		qrOrders->ParamByName("Table_Number")->AsInteger		= CurrentTable;
		qrSubOrders->ParamByName("Table_Number")->AsInteger	= CurrentTable;
	}
	qrOrders->ParamByName("Order_Type")->AsInteger				= NormalOrder;
	qrSubOrders->ParamByName("Order_Type")->AsInteger			= NormalOrder;

	int CurrentTable						= 0;
	int CurrentSeat						= 0;
	int CurrentOrderKey					= 0;
	int CurrentOptionKey					= 0;

	Currency SeatTotalPrice				= 0;  // Total price sent to palm, including items not in current menu, for each seat.

	TReturnItemComplete *ReturnItem	= NULL;
	TItem *Item								= NULL;
	TListMenu *Menu						= NULL;

//	bool SeatChanged						= true;	// Let me know when to write the seat price

	qrSubOrders->ExecQuery();
	for (qrOrders->ExecQuery(); !qrOrders->Eof; qrOrders->Next())
	{
		if (CurrentTable != qrOrders->FieldByName("Table_Number")->AsInteger ||
			 CurrentSeat  != qrOrders->FieldByName("SeatNo")->AsInteger)
		{
//			SeatChanged			= true;
			CurrentOrderKey	= 0;
			SeatTotalPrice		= 0;
			CurrentTable		= qrOrders->FieldByName("Table_Number")->AsInteger;
			CurrentSeat			= qrOrders->FieldByName("SeatNo")->AsInteger;

			if (CurrentTable > 99 || CurrentSeat > 99)
			{
				TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, "Invalid table or seat - "
					"Table #: " + IntToStr(CurrentTable) +
					" Seat #: " + IntToStr(CurrentSeat));
			}
		}
		else
		{
//			SeatChanged			= false;
		}
		if (CurrentOrderKey != qrOrders->FieldByName("Order_Key")->AsInteger)
		{
			CurrentOrderKey		= qrOrders->FieldByName("Order_Key")->AsInteger;
			Item						= NULL;
			CurrentOptionKey		= 0;

			Menu = TDeviceRealPalms::Instance().Menus->IsACurrentMenu(qrOrders->FieldByName("Menu_Name")->AsString);
			if(Menu != NULL)
			{
				TListCourse *Course = Menu->FetchCourseByName(qrOrders->FieldByName("Course_Name")->AsString);
				if(Course != NULL)
				{
					Item = Course->FetchItemByName(qrOrders->FieldByName("Item_Name")->AsString);
					if(Item != NULL)
					{
						TItemSize *ItemsSize = Item->Sizes->SizeGet(qrOrders->FieldByName("Size_Name")->AsString);
						// We have Everything we need create and return a complete Item.
						if(ItemsSize != NULL)
						{
							ReturnItem								= new TReturnItemComplete;

							ReturnItem->OrderKey					= qrOrders->FieldByName("Order_Key")->AsInteger;
							ReturnItem->Palm_ID					= Item->Palm_ID;
							ReturnItem->Palm_Size_ID			= ItemsSize->Palm_ID;
							ReturnItem->OrderType				= NormalOrder;
							ReturnItem->SetMenuItem				= ItemsSize->SetMenuItem;
							ReturnItem->SetMenuMaster			= ItemsSize->SetMenuMaster;
							ReturnItem->SetMenuMask				= ItemsSize->SetMenuMask;
							ReturnItem->ViewableLocations		= Item->ViewableLocations;

							ReturnItem->ServingCourseID		= qrOrders->FieldByName("ServingCourses_Key")->AsInteger;

							ReturnItem->Cost						= qrOrders->FieldByName("Cost")->AsDouble;
							ReturnItem->HappyHour				= (qrOrders->FieldByName("HappyHour")->AsString == "T");
							ReturnItem->Price						= qrOrders->FieldByName("Price")->AsDouble;
							ReturnItem->Discount 				= qrOrders->FieldByName("Discount")->AsDouble;
							ReturnItem->SetMenuGroup			= 0;//qrOrders->FieldByName("SetMenu_Group")->AsInteger;
							ReturnItem->Note						= qrOrders->FieldByName("Note")->AsString;

							TItemSize *MasterSize = Menu->Sizes->SizeGet(qrOrders->FieldByName("MASTER_CONTAINER")->AsString);
							if(MasterSize != NULL)
							{
								ReturnItem->Palm_MasterContainer_ID = MasterSize->Palm_ID;
							}
							// Sort out Set Menu Grouping Numbers.
							if(qrOrders->FieldByName("SetMenu_Group")->AsInteger != 0)
							{
								// Look up number for this Tab.
								TTabsSetMenuList::iterator CurrentTabsSetMenus = TDeviceRealPalms::Instance().TabsSetMenuList.find(qrOrders->FieldByName("Tab_Key")->AsInteger);
								if(CurrentTabsSetMenus != TDeviceRealPalms::Instance().TabsSetMenuList.end())
								{  // Tab already exists and is in List.
									TSetMenuList::iterator SetMenuNumber = std::find(CurrentTabsSetMenus->second.begin(),CurrentTabsSetMenus->second.end(),qrOrders->FieldByName("SetMenu_Group")->AsInteger);
									if(SetMenuNumber != CurrentTabsSetMenus->second.end())
									{
										ReturnItem->SetMenuGroup = (SetMenuNumber - CurrentTabsSetMenus->second.begin()) +1 ; // Zero based vector.
									}
									else // Add Set Menu Number to the List.
									{
										CurrentTabsSetMenus->second.push_back(qrOrders->FieldByName("SetMenu_Group")->AsInteger);
										SetMenuNumber = std::find(CurrentTabsSetMenus->second.begin(),CurrentTabsSetMenus->second.end(), qrOrders->FieldByName("SetMenu_Group")->AsInteger);
										ReturnItem->SetMenuGroup = (SetMenuNumber - CurrentTabsSetMenus->second.begin())+1;
									}   
								}
								else
								{  // New table / seat -> tab with setmenu stuff.
									TDeviceRealPalms::Instance().TabsSetMenuList[qrOrders->FieldByName("Tab_Key")->AsInteger].push_back(qrOrders->FieldByName("SetMenu_Group")->AsInteger);
									TSetMenuList::iterator SetMenuNumber = std::find(  TDeviceRealPalms::Instance().TabsSetMenuList[qrOrders->FieldByName("Tab_Key")->AsInteger].begin(),
																										TDeviceRealPalms::Instance().TabsSetMenuList[qrOrders->FieldByName("Tab_Key")->AsInteger].end(),
																										qrOrders->FieldByName("SetMenu_Group")->AsInteger);
									ReturnItem->SetMenuGroup = (SetMenuNumber - TDeviceRealPalms::Instance().TabsSetMenuList[qrOrders->FieldByName("Tab_Key")->AsInteger].begin())+1;
								}
							}
							PalmTables[CurrentTable][CurrentSeat].first.insert(ReturnItem);
							PalmTables[CurrentTable][CurrentSeat].second = 0;	// Will get set at the end of the seat.
						}
					}
				}
			}
			SeatTotalPrice += qrOrders->FieldByName("Price")->AsCurrency + qrOrders->FieldByName("Discount")->AsCurrency;

			while (!qrSubOrders->Eof && qrSubOrders->FieldByName("Order_Key")->AsInteger == qrOrders->FieldByName("Order_Key")->AsInteger)
			{
				if (ReturnItem && Menu)
				{
					TListCourse *SubCourse = Menu->FetchCourseByName(qrSubOrders->FieldByName("Sub_Course_Name")->AsString);
					if(SubCourse != NULL)
					{
						TItem *SubItem = SubCourse->FetchItemByName(qrSubOrders->FieldByName("Sub_Item_Name")->AsString);
						if(SubItem != NULL)
						{
							TItemSize *SubSize = SubItem->Sizes->SizeGet(qrSubOrders->FieldByName("Sub_Size_Name")->AsString);
							if(SubSize != NULL)
							{
								TReturnItemCompleteSub SubOrder;
								SubOrder.Palm_ID			= SubItem->Palm_ID;
								SubOrder.Palm_Size_ID	= SubSize->Palm_ID;
								
								if (ReturnItem->SubOrders.size() < 31)
								{
									ReturnItem->SubOrders.push_back(SubOrder);
								}
							}
						}
					}
				}
				SeatTotalPrice += qrSubOrders->FieldByName("Sub_Price")->AsCurrency + qrSubOrders->FieldByName("Sub_Discount")->AsCurrency;

				qrSubOrders->Next();
			}
		}
		if (Item &&
          ReturnItem &&
          CurrentOptionKey != qrOrders->FieldByName("OrderOption_Key")->AsInteger &&
			 qrOrders->FieldByName("OrderOption_Key")->AsInteger != 0)
		{
			CurrentOptionKey = qrOrders->FieldByName("OrderOption_Key")->AsInteger;

			TItemOption *Option = Item->FetchOptionByName(qrOrders->FieldByName("Option_Name")->AsString);
			if(Option != NULL)
			{
				TReturnOption ReturnOption;
				ReturnOption.Palm_ID = Option->Palm_ID;
				ReturnOption.IsPlus = qrOrders->FieldByName("Plus")->AsString == "T";
				ReturnItem->OptionsSelected.push_back(ReturnOption);
			}
		}
//		if (SeatChanged)
		if (SeatTotalPrice != 0)
		{
			PalmTables[CurrentTable][CurrentSeat].second = SeatTotalPrice;
		}
	}

/*

Select
	Tables.Table_Number,
	Seat.SeatNo,
	MasterOrder.Tab_Key,

	MasterOrder.Order_Key,
	MasterOrder.Menu_Name,
	MasterOrder.Course_Name,
	MasterOrder.Item_Name,
	MasterOrder.Size_Name,
	MasterOrder.Price,
	MasterOrder.Cost,
	MasterOrder.HappyHour,
	MasterOrder.SetMenu_Group,
	MasterOrder.Note,
	MasterOrder.Master_Container,
	MasterOrder.ServingCourses_Key,

	MasterOrder.Side_Order_Key,
	SubOrder.Order_Key Sub_Order_Key,
	SubOrder.Course_Name Sub_Course_Name,
	SubOrder.Item_Name Sub_Item_Name,
	SubOrder.Size_Name Sub_Size_Name,
	SubOrder.Price Sub_Price,

	OrderOption.OrderOption_Key,
	OrderOption.Option_Name,
	OrderOption.Plus

From
	Orders MasterOrder Inner Join Tab On
		MasterOrder.Tab_Key = Tab.Tab_Key
	Inner Join Seat On
		Tab.Tab_Key = Seat.Tab_Key
	Inner Join Tables
		On Seat.Table_Key = Tables.Table_Key
	Left Join OrderOption On
		OrderOption.Order_Key = MasterOrder.Order_Key
	Left Join Orders SubOrder On
		SubOrder.Side_Order_Key = MasterOrder.Order_Key

Where
	MasterOrder.Order_Type = 0
	And MasterOrder.Side_Order_Key Is Null
	And Tab.Tab_Type = 3
Order By
	Tables.Table_Number,
	Seat.SeatNo,
	MasterOrder.Order_Key,
	SubOrder.Order_Key,
	OrderOption.OrderOption_Key

Select
	MasterOrder.Order_Key,
	SubOrder.Order_Key Sub_Order_Key,
	SubOrder.Course_Name Sub_Course_Name,
	SubOrder.Item_Name Sub_Item_Name,
	SubOrder.Size_Name Sub_Size_Name,
	SubOrder.Price Sub_Price,

From
	Orders MasterOrder Inner Join Tab On
		MasterOrder.Tab_Key = Tab.Tab_Key
	Inner Join Seat On
		Tab.Tab_Key = Seat.Tab_Key
	Inner Join Tables
		On Seat.Table_Key = Tables.Table_Key
	Inner Join Orders SubOrder On
		SubOrder.Side_Order_Key = MasterOrder.Order_Key

Where
	MasterOrder.Order_Type = 0
	And MasterOrder.Side_Order_Key Is Null
	And Tab.Tab_Type = 3
Order By
	Tables.Table_Number,
	Seat.SeatNo,
	MasterOrder.Order_Key,
	SubOrder.Order_Key
*/
}
//---------------------------------------------------------------------------
void TPalm::fStreamOrders(const TPalm::TPalmTables &PalmTables, TMemoryStream *Stream, TStrings *Log, unsigned char &SectionFlag)
{
	unsigned char StreamByte;		// Temp used for writing to the stream.
	int TableCount = PalmTables.size();
	int TableIndex = 0;

	TPalm::TPalmTables::const_iterator iTable;
	for (iTable = PalmTables.begin(); iTable != PalmTables.end(); ++iTable, ++TableIndex)
	{
		// Write out the table number
		fWriteStreamChar(Stream, iTable->first, "Table #", Log, TableIndex == TableCount - 1);

		int SeatCount = iTable->second.size();
		int SeatIndex = 0;

		TPalmSeats::const_iterator iSeat;
		for (iSeat = iTable->second.begin(); iSeat != iTable->second.end(); ++iSeat, ++SeatIndex)
		{
			// Write out the seat number
			fWriteStreamChar(Stream, iSeat->first, "Seat #", Log, SeatIndex == SeatCount-1);

			const TPalmItemsSet &PalmItemsSet	= iSeat->second.first;
			int SeatItemCount							= PalmItemsSet.size();
			int SeatItemIndex							= 0;
			bool FirstItem								= true;

			if (SeatItemCount == 0)	// No orders. Must be a price for different menu.
			{
				fWriteStreamChar(Stream, 0, "Flag 1", Log);
				fWriteStreamChar(Stream, 0x80 | RO2ContainsMiscPricing, "Flag 2", Log);
				unsigned long SeatTotalCents = static_cast<int>(iSeat->second.second * 100);
				fWriteStreamLong(Stream, SeatTotalCents, "Seat $", Log);
			}
			else
			{
				TPalmItemsSet::const_iterator iOrders;
				for (iOrders = PalmItemsSet.begin(); iOrders != PalmItemsSet.end(); ++iOrders, ++SeatItemIndex)
				{
					TReturnItemComplete *CurrentItem = *iOrders;
					if (CurrentItem->ViewableLocations & LocPalm)
					{
						// Contains orders so set the section flag.
						SectionFlag |= ROSectionFlagContainsOrders;

						// Get all the flags.
						unsigned char Flags = ROContainsOrders;
						if(CurrentItem->SetMenuMask != 0 && CurrentItem->SetMenuGroup != 0)
						{
							Flags |= ROContainsPromoMask;
						}
						if(CurrentItem->Note != "")
						{
							Flags |= ROContainsNote;
						}
						ReturnOptionList::iterator CurrentOption = CurrentItem->OptionsSelected.begin();
						if(CurrentOption != CurrentItem->OptionsSelected.end())
						{
							Flags |= ROContainsOptions; // Set flags as we have options.
						}
						int PalmSidesCount = CurrentItem->SubOrders.size();
						if (PalmSidesCount > 0)
						{
							Flags |= ROSubOrdersMix;
						}
						fWriteStreamChar(Stream, Flags, "Flag 1", Log);

						// Total seat price. Always write this if not $0 and first item.
						unsigned long SeatTotalCents = static_cast<int>(iSeat->second.second * 100);
						// And the second
						unsigned char Flags2 = 0x80;
						if (FirstItem && SeatTotalCents != 0)
						{
							Flags2 |= RO2ContainsMiscPricing;
						}
						fWriteStreamChar(Stream, Flags2, "Flag 2", Log);

						if (Flags2 & RO2ContainsMiscPricing)
						{
							fWriteStreamLong(Stream, SeatTotalCents, "Seat $", Log);
						}

						fWriteStreamShort(Stream, CurrentItem->Palm_ID, "Item ID", Log, (SeatItemIndex == SeatItemCount - 1));
						fWriteStreamChar(Stream, CurrentItem->Palm_Size_ID, "Item Size ID", Log);
						fWriteStreamShort(Stream, CurrentItem->ServingCourseID, "Serve ID", Log);

						if (Flags & ROSubOrdersMix)
						{
							int PalmSidesIndex	= 0;
							ReturnSubOrdersList::const_iterator CurrentSubOrder = CurrentItem->SubOrders.begin();
							for(; CurrentSubOrder != CurrentItem->SubOrders.end(); ++PalmSidesIndex, ++CurrentSubOrder)
							{
								fWriteStreamShort(Stream, CurrentSubOrder->Palm_ID, "Side ID", Log, PalmSidesIndex == PalmSidesCount - 1);
								fWriteStreamChar(Stream, CurrentSubOrder->Palm_Size_ID, "Side Size ID", Log);
							}
							fWriteStreamChar(Stream, CurrentItem->Palm_MasterContainer_ID, "Master Size ID", Log);
						}
						if(Flags & ROContainsPromoMask)
						{
							fWriteStreamShort(Stream, CurrentItem->SetMenuGroup, "Set Menu Group", Log);
						}
						//Options
						if (Flags & ROContainsOptions)
						{
							int OptionCount = CurrentItem->OptionsSelected.size();
							int OptionIndex = 0;
							for(; CurrentOption!=CurrentItem->OptionsSelected.end(); ++CurrentOption, ++OptionIndex)
							{
								unsigned char OptionID = CurrentOption->Palm_ID;
								if (!CurrentOption->IsPlus) OptionID |= 0x40;
								fWriteStreamChar(Stream, OptionID, "Option ID", Log, OptionIndex == OptionCount-1);
							}
						}
						// Note.
						if (Flags & ROContainsNote)
						{
							fWriteStreamString(Stream, CurrentItem->Note.SubString(1, 80), "Note", Log);
						}
						FirstItem = false;
					}
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
bool TPalm::CheckUser()
{
	// Check the User ID against that palm.
	int StaffIDNo = this->Inbound->StaffIDNo;
	if(this->User.ContactKey == 0)
	{
		TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"Check User Failed User Not Logged in \n Palm : " + IntToStr(this->ID.DeviceKey) + " User ID:" + IntToStr(this->User.ContactKey)
					 + " Staff No: " + IntToStr(StaffIDNo));
		this->Status.Error = Err_Not_Logged_In;
	}
   else if(StaffIDNo == 0)
   {
		TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"Check User Failed. Staff No: " + IntToStr(StaffIDNo));
      this->Status.Error = Err_Not_Logged_In;
   }
   else if(this->User.ContactKey == StaffIDNo)
	{
      return true;
   }
   return false;
}

//---------------------------------------------------------------------------
void TPalm::Register(Database::TDBTransaction &DBTransaction)
{
	if(TDeviceRealPalms::Instance().RegistrationPIN != "" && Inbound->Register.NewDeviceName != "")
	{
		if(Inbound->Register.RegistrationPIN == TDeviceRealPalms::Instance().RegistrationPIN)
		{
			//Reset Reg Timer.
			TDeviceRealPalms::Instance().RegExpire->Enabled = false;
			TDeviceRealPalms::Instance().RegExpire->Enabled = true;
			TDeviceRealPalms::Instance().RegSyncNumber++; // Update Display.

			if(BIT0SET(Inbound->Register.RegistrationFlag)) // Re-registraion Request (Editing)
			{ // Look up Palm based on Supplied ID and Sets its New Name and New BT Address.
				TPalm *Palm = TDeviceRealPalms::Instance().FindTPalm(DBTransaction,Inbound->Register.PalmUniqueID);
				if(Palm != TDeviceRealPalms::Instance().PalmNotFound)
				{
					/* Log out who ever is using this palm.
					This will force any other palm out there thats on the same ID
					as this palm to try and log back in sending through there BT
					ID and thus becomming not registered. This removes an expliot
					where 2 palms could be registered on the same Palm ID*/
					Palm->User.ContactKey = 0;

					//Set the Palms Name. (May have changed.)
					Palm->ID.Name = Inbound->Register.NewDeviceName;
					Palm->ID.IP	  = Inbound->Register.DeviceIP;
					Palm->Create(DBTransaction);
/*						for(int i = 0 ; i < TComms::Instance().ConnectLst->Count; i ++)
					{
						TConnect * TempCon = (TConnect *)TComms::Instance().ConnectLst->Items[i];
						if (TempCon->TRXType == Windows_Printer || TempCon->TRXType == Virtual_Printer)
						{
							TempCon->Verify(Palm->ID.DeviceKey,true);
						}
					}*/

					//Sync Printing Here.
					if(BIT1SET(Inbound->Register.RegistrationFlag))
					{
						TPalm *SyncToPalm = TDeviceRealPalms::Instance().FindTPalm(DBTransaction,Inbound->Register.SyncPrintingToPalmKey);
						if(SyncToPalm != NULL)
						{
							TDeviceRealPalms::Instance().SyncPrintingTo(DBTransaction,SyncToPalm,Palm);
						}
						else
						{
							Status.Error = Err_ErrorMsg;
							Status.ErrorMsg = "Could not sync printing to Palm # " + IntToStr(Inbound->Register.SyncPrintingToPalmKey);
						}
					}

					Status.Error = proA_Ok;
					unsigned short PalmKey = (unsigned short)Palm->ID.DeviceKey;
					TComms::Instance().Process_Outbound->Write(&PalmKey,sizeof(PalmKey));
					BuildPacket(TComms::Instance().Process_Outbound);
					// Registering this palm as something else so remove it.
					if(this != Palm && this != TDeviceRealPalms::Instance().PalmNotFound)
					{
						this->Delete = true;
					}
				}
				else
				{
					TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Palm not found during re-Registration");
					Status.Error = Err_InfoOutOfSync;
					BuildPacket(TComms::Instance().Process_Outbound);
				}
			}
			else // (Registration Insertion)
			{
				// Look up Palm Based on Blue Tooth Addess.
				int MaxPalmLicenses = TDeviceRealPalms::Instance().RegisteredPalmLicences();
				TPalm *Palm = TDeviceRealPalms::Instance().FindTPalm(DBTransaction,Inbound->Register.DeviceIP);
				if(Palm != TDeviceRealPalms::Instance().PalmNotFound) // Already Registered this palm.
				{
					//Is the palm name Uuique?
					TPalm *UniquePalm = TDeviceRealPalms::Instance().FindTPalmByName(DBTransaction,Inbound->Register.NewDeviceName);
					if(UniquePalm == TDeviceRealPalms::Instance().PalmNotFound)
					{
						/* Log out who ever is using this palm.
						This will force any other palm out there thats on the same ID
						as this palm to try and log back in sending through there BT
						ID and thus becomming not registered. This removes an expliot
						where 2 palms could be registered on the same Palm ID*/
						Palm->User.ContactKey = 0;

						Palm->ID.Name = Inbound->Register.NewDeviceName;
						Palm->Create(DBTransaction);
	 /*					for(int i = 0 ; i < TComms::Instance().ConnectLst->Count; i ++)
						{
							TConnect * TempCon = (TConnect *)TComms::Instance().ConnectLst->Items[i];
							if (TempCon->TRXType == Windows_Printer || TempCon->TRXType == Virtual_Printer)
							{
								TempCon->Verify(Palm->ID.DeviceKey,true);
							}
						}*/

						//Sync Printing Here.
						if(BIT1SET(Inbound->Register.RegistrationFlag))
						{
							TPalm *SyncToPalm = TDeviceRealPalms::Instance().FindTPalm(DBTransaction,Inbound->Register.SyncPrintingToPalmKey);
							if(SyncToPalm != NULL)
							{
								TDeviceRealPalms::Instance().SyncPrintingTo(DBTransaction,SyncToPalm,Palm);
							}
							else
							{
								Status.Error = Err_ErrorMsg;
								Status.ErrorMsg = "Could not sync printing to Palm # " + IntToStr(Inbound->Register.SyncPrintingToPalmKey);
							}
						}

						Status.Error = proA_Ok;
						// This Palm Object running this function could be Palm_Not_Found Morph it into
						// the palm with witch to respond with.
//						ID.DeviceKey = Palm->ID.DeviceKey;
						unsigned short PalmKey = (unsigned short)Palm->ID.DeviceKey;
						TComms::Instance().Process_Outbound->Write(&PalmKey,sizeof(PalmKey));
						BuildPacket(TComms::Instance().Process_Outbound);
					}
					else
					{
						Status.Error = Err_ErrorMsg;
						Status.ErrorMsg = "Palm name not unique";
					}
				}
				else if((TDeviceRealPalms::Instance().PalmList->Count < MaxPalmLicenses) ||
						  (TDeviceRealPalms::Instance().PalmList->Count <= MaxPalmLicenses && BIT0SET(Inbound->Register.RegistrationFlag)) )
				{
					TPalm *UniquePalm = TDeviceRealPalms::Instance().FindTPalmByName(DBTransaction,Inbound->Register.NewDeviceName);
					if(UniquePalm == TDeviceRealPalms::Instance().PalmNotFound)
					{
						Palm = new TPalm();
						Palm->ID.IP			 	= Inbound->Register.DeviceIP;
						Palm->ID.Name 			= Inbound->Register.NewDeviceName;
						Palm->ID.Product 		= TDeviceRealPalms::Instance().ID.Product;
						Palm->ID.LocationKey = TDeviceRealPalms::Instance().ID.LocationKey;
						Palm->ID.Location 	= TDeviceRealPalms::Instance().ID.Location;
						Palm->ID.DeviceKey = 0;
						Palm->TransNo = 0;
						Palm->Create(DBTransaction);
						TDeviceRealPalms::Instance().PalmList->Add(Palm);

/*							for(int i = 0 ; i < TComms::Instance().ConnectLst->Count; i ++)
						{
							TConnect * TempCon = (TConnect *)TComms::Instance().ConnectLst->Items[i];
							if (TempCon->TRXType == Windows_Printer || TempCon->TRXType == Virtual_Printer)
							{
								TempCon->Verify(Palm->ID.DeviceKey,true);
							}
						}*/

						//Sync Printing Here.
						if(BIT1SET(Inbound->Register.RegistrationFlag))
						{
							TPalm *SyncToPalm = TDeviceRealPalms::Instance().FindTPalm(DBTransaction,Inbound->Register.SyncPrintingToPalmKey);
							if(SyncToPalm != NULL)
							{
								TDeviceRealPalms::Instance().SyncPrintingTo(DBTransaction,SyncToPalm,Palm);
							}
							else
							{
								Status.Error = Err_ErrorMsg;
								Status.ErrorMsg = "Could not sync printing to Palm # " + IntToStr(Inbound->Register.SyncPrintingToPalmKey);
							}
						}

						Status.Error = proA_Ok;
						// This Palm Object running this function could be Palm_Not_Found Morph it into
						// the palm with witch to respond with.
						unsigned short PalmKey = (unsigned short)Palm->ID.DeviceKey;
						TComms::Instance().Process_Outbound->Write(&PalmKey,sizeof(PalmKey));
						BuildPacket(TComms::Instance().Process_Outbound);
					}
					else
					{
						Status.Error = Err_ErrorMsg;
						Status.ErrorMsg = "Palm name not unique";
					}
				}
				else
				{
					Status.Error = Err_User_OverFlow;
				}
			}
		}
		else
		{
			Status.Error = Err_Reg_Password_Wrong;
		}
	}
	else if (Inbound->Register.NewDeviceName == "")
	{
		TPalm *Palm = TDeviceRealPalms::Instance().FindTPalm(DBTransaction,Inbound->Register.DeviceIP);
		if(Palm != TDeviceRealPalms::Instance().PalmNotFound)
		{
			Status.Error = proA_Ok;

			unsigned short PalmKey = (unsigned short)Palm->ID.DeviceKey;
			TComms::Instance().Process_Outbound->Write(&PalmKey,sizeof(PalmKey));
			TComms::Instance().Process_Outbound->Write(Palm->ID.Name.t_str(), Palm->ID.Name.Length()+1);	// Includes NULL
			BuildPacket(TComms::Instance().Process_Outbound);
		}
		else
		{
			Status.Error = Err_Not_In_Reg_Mode;
		}
	}
	else
	{
		Status.Error = Err_Not_In_Reg_Mode;
	}
}
//---------------------------------------------------------------------------
void TPalm::InfoSync(Database::TDBTransaction &DBTransaction)
{
	// Build a packet with the palms real id in it.
	this->Status.TransactionNumber = 0;
	TComms::Instance().Process_Outbound->Clear();
	TDeviceRealPalms::Instance().BuildInfoSyncPacket(DBTransaction,TComms::Instance().Process_Outbound);
	BuildPacket(TComms::Instance().Process_Outbound);
}

//---------------------------------------------------------------------------
//------------------------Creators for order handling classes----------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//									Handles incomming packet.
//---------------------------------------------------------------------------

_fastcall TIRPacket::TIRPacket(TPalm *inOwner)
{
// List that Holds Tables when required.
   Owner = inOwner;
   Valid = false;

   LastTable = false;
   LastSeat = false;
   LastItem = false;
   FinalBill = ptPreliminary;
   OrderCount = 1;

   PktStart = NULL;
   ptrBuffer = NULL;
	ptrReadBuffer = NULL;
   Header.Clear();
   Billing.Clear();
   Orders.Clear();
   Login.Clear();
}

_fastcall TIRPacket::~TIRPacket()
{
   if(ptrBuffer)
		delete []ptrBuffer;
}

//---------------------------------------------------------------------------
// Command is the command under which this packet is built.
// I.E a the Plam has sent us an order...ORDERS 0xE4
_fastcall TIRTable::TIRTable(TIRPacket *Packet, unsigned char Command)
	: Seats(new TList()), Table(0)
{
//   TIRSeat *TempSeat;
//	Table = 0;
//   SelectedCourses.clear();
//	Seats = new TList();

	try
	{
		long inbytes=1;
		Packet->ReadFileBuf(&Table,inbytes);

		if(Table & cIsLast)
		{
				Packet->LastTable = true;
		}

		Table &= 0x7F;
		if(Table > 99)
		{
			throw EHelixError("Invalid Table Number Received:" + IntToStr(Table));
	//      TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Invalid Table Number Received:" + IntToStr(Table));
		}

		if(Command == CALL_AWAY)
		{
			bool LastCourse = false;
			do
			{
				unsigned char CourseLo = 0;
				unsigned char CourseHi = 0;
				//Retrieve Time Stamp.
				inbytes=1;
				Packet->ReadFileBuf(&CourseHi,inbytes);
				Packet->ReadFileBuf(&CourseLo,inbytes);
				unsigned short Course = (unsigned short)((CourseHi*256) + CourseLo);
				if(Course & wIsLast)
				{
					LastCourse = true;
					Course = Course & 0x7FFF;
				}
				SelectedCourses.insert(Course);
				#if (DEBUGLEVEL & debugExpandOrders )
								char out[10];
								ZeroMemory(out,10);
								UnicodeString TextOut,Fo0,Fo1;
								sprintf(out,"%02X ",CourseHi);
								Fo0 = out;
								Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
								TextOut += Fo1 + " <- Course ID "
													+ ( (Course & wIsLast) ? "LAST " : "");
								TManagerLogs::Instance().Add(__FUNC__,PALMDECODEPACKETLOG,TextOut);
								TextOut = "";
								Fo0 = "";
								Fo1 = "";

				#endif

				#if (DEBUGLEVEL & debugExpandOrders )
								ZeroMemory(out,10);
								sprintf(out,"%02X ",CourseLo);
								Fo0 = out;
								Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
								TextOut += Fo1 + " <- Course ID "
													+ ( (Course & wIsDrink) ? "DRINK COURSE" : "");
								TManagerLogs::Instance().Add(__FUNC__,PALMDECODEPACKETLOG,TextOut);
								TextOut = "";
								Fo0 = "";
								Fo1 = "";

				#endif
			}
			while(LastCourse == false);
		}
		else
		{
			#if (DEBUGLEVEL & debugExpandOrders )
							char out[10];
							ZeroMemory(out,10);
							UnicodeString TextOut,Fo0,Fo1;
							sprintf(out,"%02X ",Table);
							Fo0 = out;
							Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
							TextOut += Fo1 + (Packet->LastTable ? " <- Last Table Number ":" <- Table Number ") ;
							TManagerLogs::Instance().Add(__FUNC__,PALMDECODEPACKETLOG,TextOut);
			#endif

			Packet->LastSeat = false;
			do
			{
				//TIRSeat *TempSeat = new TIRSeat(Packet);
				Seats->Add(new TIRSeat(Packet));
				if(Packet->ptrReadBuffer > (Packet->ptrBuffer + Packet->PktSize))
				{
					throw EHelixError("Internal Packet Overrun. Adding Seats.");
				}
			}
			while(!Packet->LastSeat);
		}
	}
	catch (Exception &E)
	{
		CleanUp();
		throw;
	}
	catch (EHelixError &E)
	{
		CleanUp();
		throw;
	}
}
//---------------------------------------------------------------------------
_fastcall TIRTable::~TIRTable()
{
	CleanUp();
}
//---------------------------------------------------------------------------
void TIRTable::CleanUp()
{
  while(Seats->Count != 0)
  {
	 delete static_cast<TIRSeat *>(Seats->Items[0]);
    Seats->Delete(0);
  }
}
//---------------------------------------------------------------------------
_fastcall TIRSeat::TIRSeat(TIRPacket *Packet)
	: Orders(new TList()), Seat(0), PalmTime(0), Flags(0), SeatFlag(0), NameChange(false)
{
	long inbytes=1;

	try
	{
		// Retrieve the Seat Number the Seat Name is for.
		Packet->ReadFileBuf(&Seat,inbytes);
		if(Seat & cIsLast)
		{
			Packet->LastSeat = true;
		}
		Seat &= 0x7F;

		if(Seat > 99)
		{
			throw EHelixError("Invalid Seat Number Received:" + IntToStr(Seat));
		}

		if(Packet->Header.Command == ORDERS)
		{
			inbytes=1;
			Packet->ReadFileBuf(&SeatFlag,inbytes);
		}

		#if (DEBUGLEVEL & debugExpandOrders )
						char out[10];
						ZeroMemory(out,10);
						UnicodeString TextOut,Fo0,Fo1;
						sprintf(out,"%02X ",Seat);
						Fo0 = out;
						Fo1 = Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
						TextOut = Fo1 + (Packet->LastSeat ? " <- Last Seat Number ":" <- Seat Number ") ;
						TManagerLogs::Instance().Add(__FUNC__,PALMDECODEPACKETLOG,TextOut);

						if(Packet->Header.Command == ORDERS)
						{
							ZeroMemory(out,10);
							sprintf(out,"%02X ",SeatFlag);
							Fo0 = out;
							Fo1 = Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
							TextOut = Fo1 + " <- Seat Flag "
												+ ( (SeatFlag & ROSectionFlagHasDisabledItems)  ? "+NotUsed(0x80)" : "-NotUsed(0x80)")
												+ ( (SeatFlag & ROSectionFlagEnableAllDrinks)   ? " +NotUsed(0x40)" : " -NotUsed(0x40)")
												+ ( (SeatFlag & ROSectionFlagEnableAllDishes)   ? " +NotUsed(0x20)" : " -NotUsed(0x20)")
												+ ( (SeatFlag & ROSectionFlagContainsSeatName)  ? " +SeatName(0x10)" : " -SeatName(0x10)")
												+ ( (SeatFlag & ROSectionFlagContainsOrders)    ? " +Orders(0x08)" : " -Orders(0x08)")
												+ ( (SeatFlag & ROSectionFlagContainsMessage)   ? " +Message(0x04)" : " -Message(0x04)");
							TManagerLogs::Instance().Add(__FUNC__,PALMDECODEPACKETLOG,TextOut);
						}
		#endif

		Packet->LastItem = false;
		if(Packet->Header.Command == ORDERS)
		{
			// Loop for every used bit in the section flag.
	/*		if(SeatFlag & ROSectionFlagContainsSeatName)      // Get in Seat Name.
			{
				//Retrieve Flags.
				inbytes=1;
				Packet->ReadFileBuf(&Flags,inbytes);
				#if (DEBUGLEVEL & debugExpandOrders )
								char out[10];
								ZeroMemory(out,10);
								UnicodeString TextOut,Fo0,Fo1;
								sprintf(out,"%02X ",Flags);
								Fo0 = out;
								Fo1 = Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
								TextOut = Fo1 + " <- Seat Name Flag ("
													+ ( (Flags & ROContainsPromoMask) ? "+SETMENU(0x80)":"-SETMENU(0x80)")
													+ ( BIT6SET(Flags) ?                " +OPTIONS(0x40)":" -OPTIONS(0x40)")
													+ ( BIT5SET(Flags) ?                " +PRICEADJ(0x20)":" -PRICEADJ(0x20)")
													+ ( BIT4SET(Flags) ?                " +NOTE(0x10)":" -NOTE(0x10)")
													+ ( (Flags & ROContainsSeatName) ?  " +SEAT NAME(0x04)":" -SEAT NAME(0x04)")
													+ ( (Flags & ROContainsOrders) ?    " +ORDERS(0x02)":" -ORDERS(0x02)")
													+ ( (Flags & ROSubOrdersMix) ?      " +SUBITEMS(0x08)":" -SUBITEMS(0x08)")
													+ ( (Flags & ROCreditItem) ?        " +CREDIT(0x01)":" -CREDIT(0x01)")
													+ ")" ;
								TManagerLogs::Instance().Add(__FUNC__,PALMDECODEPACKETLOG,TextOut);
				#endif

				//Retrieve Seat Name.
				NameChange = true;
				unsigned char input = NULL;
				while(!(input & cIsLast))
				{
					inbytes=1;
					Packet->ReadFileBuf(&input,inbytes);

					if(input & cIsLast)
					{
						Name += (char)(input & 0x7F);
						#if (DEBUGLEVEL & debugExpandOrders )
										char out[10];
										ZeroMemory(out,10);
										UnicodeString TextOut,Fo0,Fo1;
										sprintf(out,"%02X ",input);
										Fo0 = out;
										Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
										TextOut += Fo1 + " <- Seat Name Char " + (char)(input & 0x7F) + " Last";
										TManagerLogs::Instance().Add(__FUNC__,PALMDECODEPACKETLOG,TextOut);
						#endif
					}
					else
					{
						Name += (char)input;
						#if (DEBUGLEVEL & debugExpandOrders )
										char out[10];
										ZeroMemory(out,10);
										UnicodeString TextOut,Fo0,Fo1;
										sprintf(out,"%02X ",input);
										Fo0 = out;
										Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
										TextOut += Fo1 + " <- Seat Name Char " + (char)(input);
										TManagerLogs::Instance().Add(__FUNC__,PALMDECODEPACKETLOG,TextOut);
						#endif
					}
					if(Packet->ptrReadBuffer > (Packet->ptrBuffer + Packet->PktSize))
					{
						throw EHelixError("Internal Packet Overrun. Reading Name.");
					}
				}
			}*/

			if(SeatFlag & ROSectionFlagContainsOrders)      // Get in Orders.
			{
				do
				{
					//Retrieve Flags.
					inbytes=1;
					Packet->ReadFileBuf(&Flags,inbytes);
					#if (DEBUGLEVEL & debugExpandOrders )
									char out[10];
									ZeroMemory(out,10);
									UnicodeString TextOut,Fo0,Fo1;
									sprintf(out,"%02X ",Flags);
									Fo0 = out;
									Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
									TextOut += Fo1 + " <- Orders Flag ("
																			+ ( (Flags & ROContainsPromoMask) ? "+SETMENU(0x80)":"-SETMENU(0x80)")
																			+ ( BIT6SET(Flags) ?                " +OPTIONS(0x40)":" -OPTIONS(0x40)")
																			+ ( BIT5SET(Flags) ?                " +PRICEADJ(0x20)":" -PRICEADJ(0x20)")
																			+ ( BIT4SET(Flags) ?                " +NOTE(0x10)":" -NOTE(0x10)")
																			+ ( (Flags & ROContainsSeatName) ?  " +SEAT NAME(0x04)":" -SEAT NAME(0x04)")
																			+ ( (Flags & ROContainsOrders) ?    " +ORDERS(0x02)":" -ORDERS(0x02)")
																			+ ( (Flags & ROSubOrdersMix) ?      " +SUBITEMS(0x08)":" -SUBITEMS(0x08)")
																			+ ( (Flags & ROCreditItem) ?        " +CREDIT(0x01)":" -CREDIT(0x01)")
																			+ ")" ;
									TManagerLogs::Instance().Add(__FUNC__,PALMDECODEPACKETLOG,TextOut);
					#endif

					TIROrder *TempOrder = new TIROrder(Packet,Flags);
					Orders->Add(TempOrder);
					if(Packet->ptrReadBuffer > (Packet->ptrBuffer + Packet->PktSize))
					{
						throw EHelixError("Internal Packet Overrun. Adding Orders.");
					}

					if(Packet->LastItem)
					{
						unsigned char ItemLo = 0;
						unsigned char ItemHi = 0;
						//Retrieve Time Stamp.
						Packet->ReadFileBuf(&ItemHi,inbytes);
							#if (DEBUGLEVEL & debugExpandOrders )
											char out[10];
											ZeroMemory(out,10);
											UnicodeString TextOut,Fo0,Fo1;
											sprintf(out,"%02X ",ItemHi);
											Fo0 = out;
											Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
											TextOut += Fo1 + " <- Time Byte";
											TManagerLogs::Instance().Add(__FUNC__,PALMDECODEPACKETLOG,TextOut);
											TextOut = "";
											Fo0 = "";
											Fo1 = "";

							#endif
						 Packet->ReadFileBuf(&ItemLo,inbytes);
							#if (DEBUGLEVEL & debugExpandOrders )
											ZeroMemory(out,10);
											sprintf(out,"%02X ",ItemLo);
											Fo0 = out;
											Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
											TextOut += Fo1 + " <- Time Byte Total Time In Sec :" + IntToStr( ((ItemHi*256) + ItemLo) );
											TManagerLogs::Instance().Add(__FUNC__,PALMDECODEPACKETLOG,TextOut);
							#endif
						PalmTime = ((ItemHi*256) + ItemLo);
					}
				}
				while(!Packet->LastItem);
			}
		}
	}
	catch (Exception &E)
	{
		CleanUp();
		throw;
	}
	catch (EHelixError &E)
	{
		CleanUp();
		throw;
	}
}

//---------------------------------------------------------------------------
_fastcall TIRSeat::~TIRSeat()
{
		CleanUp();
}
//---------------------------------------------------------------------------
void TIRSeat::CleanUp()
{
  while(Orders->Count != 0)
  {
	 delete static_cast<TIROrder *>(Orders->Items[0]);
    Orders->Delete(0);
  }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

_fastcall TIROrder::TIROrder(TIRPacket *Packet, unsigned char Flags)
{
   PriceAdjust = 0.0;
   DrinkIDCount = 1;
   unsigned long Cents = 0;

   unsigned char ItemLo = 0;
   unsigned char ItemHi = 0;

   long GlassType = 0;
	unsigned short Item = 0;
//	unsigned short ServingCouseID = 0;
   long  StoreItem = 0;
	SetMenuGroup = 0;
   SetMenuGroupIsIndex = false;

   Status = NormalOrder;
   Drink = false;
   Mixer = false;
	PromoItem = false;
   IsPriceAdjust = false;
   OrderLK = NULL;
   PalmSplitLK = NULL;

   long inbytes = 1;
   if((Flags & ROContainsPromoMask) == ROContainsPromoMask) // Is it a Promo Item?
   {
      PromoItem = true;
   }

   if((Flags & ROSubOrdersMix) == ROSubOrdersMix) // Is it a Mixer?
   {
      Mixer = true;
   }

	// Retrieve the ID.
	inbytes=1;
	Packet->ReadFileBuf(&ItemHi,inbytes);
	Packet->ReadFileBuf(&ItemLo,inbytes);
	Item = (unsigned short)((ItemHi*256) + ItemLo);
	#if (DEBUGLEVEL & debugExpandOrders )
					char out[10];
					ZeroMemory(out,10);
					UnicodeString TextOut,Fo0,Fo1;
					sprintf(out,"%02X ",ItemHi);
					Fo0 = out;
					Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
					TextOut += Fo1 + " <- Item ID "
										+ ( (Item & wIsLast) ? "LAST " : "");
					TManagerLogs::Instance().Add(__FUNC__,PALMDECODEPACKETLOG,TextOut);
					TextOut = "";
					Fo0 = "";
					Fo1 = "";

	#endif

   #if (DEBUGLEVEL & debugExpandOrders )
               ZeroMemory(out,10);
               sprintf(out,"%02X ",ItemLo);
               Fo0 = out;
               Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
               TextOut += Fo1 + " <- Item ID "
                              + ( (Item & wIsDrink) ? "DRINK " : "");
               TManagerLogs::Instance().Add(__FUNC__,PALMDECODEPACKETLOG,TextOut);
               TextOut = "";
               Fo0 = "";
               Fo1 = "";

   #endif

   if(Packet->Header.Command == REQUEST_RECEIPT)
   {
      //Retrieve Group Flags.
      inbytes=1;
      Packet->ReadFileBuf(&PalmSplitLK,inbytes);
   }

   if(Item & wIsLast)
   {
      Packet->LastItem = true;
   }

   StoreItem = Item & 0x7FFF;
   Items.push_back(StoreItem);
   inbytes=1;
   //Retrieve Glass Type.
   Packet->ReadFileBuf(&GlassType,inbytes);
   #if (DEBUGLEVEL & debugExpandOrders )
               ZeroMemory(out,10);
               sprintf(out,"%02X ",GlassType);
               Fo0 = out;
               Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
               TextOut += Fo1 + " <- Size ID ";
               TManagerLogs::Instance().Add(__FUNC__,PALMDECODEPACKETLOG,TextOut);
               TextOut = "";
               Fo0 = "";
               Fo1 = "";
   #endif

	Container.push_back(GlassType);

	// Retrieve the Serving Course ID.
	inbytes=1;
	Packet->ReadFileBuf(&ItemHi,inbytes);
	Packet->ReadFileBuf(&ItemLo,inbytes);
	ServingCourseID = (unsigned short)((ItemHi*256) + ItemLo);
	#if (DEBUGLEVEL & debugExpandOrders )
					ZeroMemory(out,10);
					sprintf(out,"%02X ",ServingCourseID);
					Fo0 = out;
					Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
					TextOut += Fo1 + " <- Serving Course ID ";
					TManagerLogs::Instance().Add(__FUNC__,PALMDECODEPACKETLOG,TextOut);
					TextOut = "";
					Fo0 = "";
					Fo1 = "";

	#endif

	// Retrieve any Sub Items Drink Combonation.

	if(PromoItem && !Mixer)
	{
		inbytes=1;
		Packet->ReadFileBuf(&ItemHi,inbytes);
		Packet->ReadFileBuf(&ItemLo,inbytes);
		SetMenuGroup = (unsigned short)((ItemHi*256) + ItemLo);
//		inbytes=2;
// THIS IS WRONG IT SOULD BE THE MSB OF THE FLAG.
//		Packet->ReadFileBuf(&SetMenuGroup, inbytes);
		if(SetMenuGroup & 0x8000)
		{
			SetMenuGroupIsIndex = false;
		}
		else
		{
			SetMenuGroupIsIndex = true;
		}
		#if (DEBUGLEVEL & debugExpandOrders )
						ZeroMemory(out,10);
						sprintf(out,"%02X ",SetMenuGroup);
						Fo0 = out;
						Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
						TextOut += Fo1 + " <- SetMenuGroup ID " + ((SetMenuGroup & 0x8000) ? "Palm Generated" : "PC Generated");
						TManagerLogs::Instance().Add(__FUNC__,PALMDECODEPACKETLOG,TextOut);
						TextOut = "";
						Fo0 = "";
						Fo1 = "";
		#endif
		SetMenuGroup &= 0x7FFF;
	}

	if(Mixer)
	{
		do
		{
			inbytes=1;
			Packet->ReadFileBuf(&ItemHi,inbytes);
			Packet->ReadFileBuf(&ItemLo,inbytes);
			Item = ((ItemHi*256) + ItemLo);

			StoreItem = Item & 0x7FFF;
			Items.push_back(StoreItem);
			Packet->ReadFileBuf(&GlassType,inbytes);
			#if (DEBUGLEVEL & debugExpandOrders )
							ZeroMemory(out,10);
							sprintf(out,"%02X ",ItemHi);
							Fo0 = out;
							Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
							TextOut += Fo1 + " <- Sub Item ID DRINK "
												+ ( (Item & wIsLast) ? "LAST " : "");
							TManagerLogs::Instance().Add(__FUNC__,PALMDECODEPACKETLOG,TextOut);
							TextOut = "";
							Fo0 = "";
							Fo1 = "";

							ZeroMemory(out,10);
							sprintf(out,"%02X ",ItemLo);
							Fo0 = out;
							Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
							TextOut += Fo1 + " <- Sub Item ID ";
							TManagerLogs::Instance().Add(__FUNC__,PALMDECODEPACKETLOG,TextOut);
							TextOut = "";
							Fo0 = "";
							Fo1 = "";

							ZeroMemory(out,10);
							sprintf(out,"%02X ",GlassType);
							Fo0 = out;
							Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
							TextOut += Fo1 + " <- Sub Size ID ";
							TManagerLogs::Instance().Add(__FUNC__,PALMDECODEPACKETLOG,TextOut);
							TextOut = "";
							Fo0 = "";
							Fo1 = "";
			#endif

			Container.push_back(GlassType);
         if(Packet->ptrReadBuffer > (Packet->ptrBuffer + Packet->PktSize))
         {
				throw EHelixError("Internal Packet Overrun. Adding Mixer.");
         }
		}
		while(!(Item & wIsLast));

		inbytes=1;
		//Retrieve Glass Type.
		Packet->ReadFileBuf(&MasterContainer,inbytes);
		#if (DEBUGLEVEL & debugExpandOrders )
						ZeroMemory(out,10);
						sprintf(out,"%02X ",MasterContainer);
						Fo0 = out;
						Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
						TextOut += Fo1 + " <- Master Size ID ";
						TManagerLogs::Instance().Add(__FUNC__,PALMDECODEPACKETLOG,TextOut);
						TextOut = "";
						Fo0 = "";
						Fo1 = "";
		#endif

		if(PromoItem)
		{
//			inbytes=2;
//			Packet->ReadFileBuf(&SetMenuGroup,inbytes);
			inbytes=1;
			Packet->ReadFileBuf(&ItemHi,inbytes);
			Packet->ReadFileBuf(&ItemLo,inbytes);
			SetMenuGroup = (unsigned short)((ItemHi*256) + ItemLo);
			if(SetMenuGroup & 0x8000)
			{
				SetMenuGroupIsIndex = false;
			}
			else
			{
				SetMenuGroupIsIndex = true;
			}
			SetMenuGroup &= 0x7FFF;
			#if (DEBUGLEVEL & debugExpandOrders )
							ZeroMemory(out,10);
							sprintf(out,"%02X ",SetMenuGroup);
							Fo0 = out;
							Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
							TextOut += Fo1 + " <- SetMenuGroup ID " + ((SetMenuGroup & 0x8000) ? "Palm Generated" : "PC Generated");
							TManagerLogs::Instance().Add(__FUNC__,PALMDECODEPACKETLOG,TextOut);
							TextOut = "";
							Fo0 = "";
							Fo1 = "";
			#endif
		}
	}

   // Retrieve Options (if any).
   if(BIT6SET(Flags))
   {
      unsigned char Option = NULL;
      bool LastOption = false;
      do
      {
         inbytes=1;
         Packet->ReadFileBuf(&Option,inbytes);
         #if (DEBUGLEVEL & debugExpandOrders )
                     char out[10];
                     UnicodeString TextOut,Fo0,Fo1;
                     ZeroMemory(out,10);
                     sprintf(out,"%02X ",Option);
                     Fo0 = out;
                     Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
                     TextOut += Fo1 + " <- Option ID "
                                    + ((IsLast(Option)) ? "LAST " : "");
                     TManagerLogs::Instance().Add(__FUNC__,PALMDECODEPACKETLOG,TextOut);
                     TextOut = "";
                     Fo0 = "";
                     Fo1 = "";
         #endif
         if(Option != 0)
         {
            Options.push_back(Option & 0x7F);
            if(IsLast(Option))
            {
               LastOption = true;
            }
         }
         if(Packet->ptrReadBuffer > (Packet->ptrBuffer + Packet->PktSize))
         {
            throw EHelixError("Internal Packet Overrun. Adding Options.");
         }
      }
      while(!LastOption);
   }

   // Retrieve the Price Adjustment.(if any)
   if(BIT5SET(Flags))
   {
      IsPriceAdjust = true;
	   inbytes=4;
      Packet->ReadFileBuf(&Cents,inbytes);
      #if (DEBUGLEVEL & debugExpandPalmComms )
                  char out[10];
                  UnicodeString TextOut,Fo0,Fo1;
                  ZeroMemory(out,10);
                  sprintf(out,"%02X ",LOBYTE(HIWORD(Cents)) );
                  Fo0 = out;
                  Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
                  TextOut += Fo1 + " <- Price Adjust ";
                  TManagerLogs::Instance().Add(__FUNC__,PALMDECODEPACKETLOG,TextOut);
                  TextOut = "";
                  Fo0 = "";
                  Fo1 = "";
                  ZeroMemory(out,10);
                  sprintf(out,"%02X ",HIBYTE(HIWORD(Cents)));
                  Fo0 = out;
                  Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
                  TextOut += Fo1 + " <- Price Adjust ";
                  TManagerLogs::Instance().Add(__FUNC__,PALMDECODEPACKETLOG,TextOut);
                  TextOut = "";
                  Fo0 = "";
                  Fo1 = "";
                  ZeroMemory(out,10);
                  sprintf(out,"%02X ",LOBYTE(LOWORD(Cents)));
                  Fo0 = out;
                  Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
                  TextOut += Fo1 + " <- Price Adjust ";
                  TManagerLogs::Instance().Add(__FUNC__,PALMDECODEPACKETLOG,TextOut);
                  TextOut = "";
                  Fo0 = "";
                  Fo1 = "";
                  ZeroMemory(out,10);
                  sprintf(out,"%02X ",HIBYTE(LOWORD(Cents)));
                  Fo0 = out;
                  Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
                  TextOut += Fo1 + " <- Price Adjust ";
                  TManagerLogs::Instance().Add(__FUNC__,PALMDECODEPACKETLOG,TextOut);
                  TextOut = "";
                  Fo0 = "";
                  Fo1 = "";
      #endif

      PriceAdjust = double(
                    (LOBYTE(LOWORD(Cents)) << 24) +
                    (HIBYTE(LOWORD(Cents)) << 16) +
                    (LOBYTE(HIWORD(Cents)) << 8) +
                    (HIBYTE(HIWORD(Cents)) )) / 100.0;
   }

   // Retrieve the Note if any...
   if(BIT4SET(Flags))
   {
      unsigned char input = NULL;
      while(!IsLast(input))
      {
         inbytes=1;
         Packet->ReadFileBuf(&input,inbytes);

         if(IsLast(input))
         {
            Note += (char)(input & 0x7F);
            #if (DEBUGLEVEL & debugExpandOrders )
                        char out[10];
                        UnicodeString TextOut,Fo0,Fo1;
                        ZeroMemory(out,10);
                        sprintf(out,"%02X ",input);
                        Fo0 = out;
                        Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
                        TextOut += Fo1 + " <- Note LAST ";
                        TManagerLogs::Instance().Add(__FUNC__,PALMDECODEPACKETLOG,TextOut);
                        TextOut = "";
                        Fo0 = "";
                        Fo1 = "";
            #endif

         }
         else
         {
            Note += (char)input;
            #if (DEBUGLEVEL & debugExpandOrders )
                        char out[10];
                        UnicodeString TextOut,Fo0,Fo1;
                        ZeroMemory(out,10);
                        sprintf(out,"%02X ",input);
                        Fo0 = out;
                        Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
                        TextOut += Fo1 + " <- Note ";
                        TManagerLogs::Instance().Add(__FUNC__,PALMDECODEPACKETLOG,TextOut);
                        TextOut = "";
                        Fo0 = "";
                        Fo1 = "";
            #endif
         }
         if(Packet->ptrReadBuffer > (Packet->ptrBuffer + Packet->PktSize))
         {
            throw EHelixError("Internal Packet Overrun. Reading Note.");
         }
      }
   }

   // Set the credit if any...
   if((Flags & ROCreditItem) == ROCreditItem)
   {
	   Status = CanceledOrder;
   }
}

//---------------------------------------------------------------------------

bool IsLast(int InItemID)
{
   if((InItemID & wIsLast) == wIsLast)
   {
      return true;
   }
   else
   {
      return false;
   }
}

bool IsLast(unsigned char InItemID)
{
   if((InItemID & cIsLast) == cIsLast)
   {
      return true;
   }
   else
   {
      return false;
   }
}


//------------------------------------------------------------------------------

_fastcall TIROrder::~TIROrder()
{
  // this will remove All allocated ports.
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//------------------------------------------------------------------------------

void TIRPacket::ReadFileBuf(void *Dest,int inbytes)
{
   for(int i=0;i<inbytes;i++)
   {
      ((char *)Dest)[i] = *(this->ptrReadBuffer);
      this->ptrReadBuffer++;
   }
}

void TIRPacket::Process(unsigned char* &ptrStream)
{
   this->PktSize = (this->Header.HighPktSize * 256) + this->Header.LowPktSize;
   this->StaffIDNo = (this->Header.HighStaffIDNo * 256) + this->Header.LowStaffIDNo;
   
   if(this->PktSize != 0)
   {
      this->ptrBuffer = new unsigned char[this->PktSize + 1]; // Needed for NULL in some cases.
      for(int i=0;i<this->PktSize;i++)
      {
         this->ptrBuffer[i] = *ptrStream++;
      }

      // Line up the read Pointer for Data processing.
      this->ptrReadBuffer = this->ptrBuffer;

      unsigned char HiCRC = *ptrStream++;
      unsigned char LowCRC = *ptrStream++;
      unsigned short int CRC;
      CRC = (unsigned short int)((HiCRC*256) + LowCRC);
		TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"CRC Size is " + IntToStr(CRC));
      Owner->CheckCRC(CRC);
      if(this->Header.Command != KEEPALIVE &&
         this->Header.Command != MENU_SYNC &&
			this->Header.Command != DRINK_SYNC &&
			this->Header.Command != INFO_SYNC &&
			this->Header.Command != REGISTRATION)
      {
         Owner->CheckSyncNo();
			if(this->Header.Command != LOGIN)
         {
         	Owner->CheckUser();
         }
      }
   }
   else
   {
      unsigned char HiCRC = *ptrStream++;
      unsigned char LowCRC = *ptrStream++;
      unsigned short int CRC;
      CRC = (unsigned short int)((HiCRC*256) + LowCRC);
		TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"CRC Size is " + IntToStr(CRC));
      Owner->CheckCRC(CRC);
      if(this->Header.Command != KEEPALIVE &&
         this->Header.Command != MENU_SYNC &&
      	this->Header.Command != DRINK_SYNC &&
			this->Header.Command != INFO_SYNC &&
         this->Header.Command != REGISTRATION)
      {
         Owner->CheckSyncNo();
         if(this->Header.Command != LOGIN)
         {
         	Owner->CheckUser();
         }
      }
		TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"Empty Order Recived.");
   }

   if(Header.Command < 0xE0 && Header.Command > 0xEA)
   {
		TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"Message Not Recognised. Bad Command :" + IntToStr(Header.Command));
		Owner->Status.ErrorMsg = "Message Not Recognised. Bad Command :" + IntToStr(Header.Command);
      Owner->Status.Error = Err_Currupt_Message;
      Owner->BuildPacket(NULL);
      Owner->ClearInbound();
   }
}

void TIRPacket::Clear()
{
   LastTable = false;
   LastSeat = false;
   LastItem = false;

   FinalBill = ptPreliminary;
   OrderCount = 1;

   Valid = false;
   StaffIDNo = 0;
   Header.Clear();
   Orders.Clear();
   Billing.Clear();
   CallAways.Clear();
   Login.Clear();
	Register.DeviceIP = "";

   PktStart = NULL;

   if(ptrBuffer != NULL)
		delete [] ptrBuffer;

   ptrBuffer = NULL;
	ptrReadBuffer = NULL;
}


//---------------------------------------------------------------------------
//------------------------------------------------------------------------------
//---------------------------------------------------------------------------
//------------------------------------------------------------------------------

TIRPacket::PktData::PktData()
{
   Tables = new TList();
//--------------------------------------
//--------------------------------------

}


TIRPacket::PktData::~PktData()
{
   TIRTable *TempTable;
   // this will remove All allocated ports.
   Tables->Pack();
   while(Tables->Count != 0)
   {
   	TempTable = (TIRTable *)Tables->Items[0];
   	delete (TIRTable *)TempTable;
   	Tables->Delete(0);
   }
   delete (TList *)Tables;
}


void TIRPacket::PktData::Clear()
{
//--------------------------------------
   TIRTable *TempTable;
   // this will remove All allocated Tables / Seats etc...
   Tables->Pack();
   while(Tables->Count != 0)
   {
      TempTable = (TIRTable *)Tables->Items[0];
      delete (TIRTable *)TempTable;
      Tables->Delete(0);
   }
}


void TIRPacket::PktHeader::Clear()
{
   Command =(MESSAGE_TYPE) NULL;
   TransNo = 0;
   InfoSyncNo = 0;
   MenuSync1No = 0;
   MenuSync2No = 0;
   HighStaffIDNo = 0;
   LowStaffIDNo = 0;
   ErrorNo = 0;
   HighPktSize = 0;
   LowPktSize = 0;
	SectionFlag = NULL;
}


void TIRPacket::PktLogin::Clear()
{
	NewStaffID = 0;
	UsersPIN = "";
   // Contains Name (Unique Palm Identifier in case of an InfoSync)
   DeviceIP = "";
}





//------------------------------------------------------------------------------
void TPalm::LogHexMessage(UnicodeString MsgType, unsigned char *ptrMessage, int PacketSize)
{
	UnicodeString msg = MsgType;
	for (int i=0; i<PacketSize; ++i)
	{
		msg += " " + IntToHex(int(ptrMessage[i]), 2);
	}
	int MessageIndex = 1;
	while (MessageIndex <= msg.Length())
	{
		UnicodeString subMsg = msg.SubString(MessageIndex, 180);
		MessageIndex += subMsg.Length();
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, subMsg, ID.Name);
	}
}
//------------------------------------------------------------------------------
TCallAwayComplete *TPalm::BuildCallAwayProper(Database::TDBTransaction &DBTransaction,TIRTable *TempTable)
{
	TCallAwayComplete *Return = NULL;
	try
	{
		Return = new TCallAwayComplete;
      Return->TableNo =  TempTable->Table;

		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
      // Retrive Table Data.
      IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT TABLE_NAME,PARTY_NAME FROM TABLES t "
      " WHERE "
      "  t.TABLE_NUMBER = :TABLE_NUMBER ";
		IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = TempTable->Table;
      IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount)
      {
			Return->TableName = IBInternalQuery->FieldByName("TABLE_NAME")->AsString;
			Return->PartyName = IBInternalQuery->FieldByName("PARTY_NAME")->AsString;
      }

      // Retrive course Data.
      for(std::set<long>::iterator CrntCourseID = TempTable->SelectedCourses.begin(); CrntCourseID != TempTable->SelectedCourses.end(); advance(CrntCourseID,1))
		{
			bool FoundCourse = true;
			TServingCourse ServingCourse;
			if (!TDeviceRealPalms::Instance().Menus->MenuFood->GetServingCourseByID(*CrntCourseID, ServingCourse))
			{
				if (!TDeviceRealPalms::Instance().Menus->MenuDrinks->GetServingCourseByID(*CrntCourseID, ServingCourse))
				{
					FoundCourse = false;
				}
			}
			if (FoundCourse)
			{
				Return->Courses->Add(ServingCourse.Name);
			}

/*			TListCourse *Course = TDeviceRealPalms::Instance().Menus->MenuFood->FetchCourseByID(*CrntCourseID);
         if(Course)
         {
				Return->Courses->Add(Course->Course_Name);
			}
         else
         {
				TListCourse *Course = TDeviceRealPalms::Instance().Menus->MenuDrinks->FetchCourseByID(*CrntCourseID);
            if(Course)
            {
               Return->Courses->Add(Course->Course_Name);
            }
			}*/
      }
	}
   catch(Exception &Err)
   {
   	delete Return;
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Err.Message);
      throw;
   }
   return Return;
}

TItemComplete *TPalm::BuildOrderProper(TIROrder * TempOrder,TDateTime OrderTimeStamp)
{
	TItemComplete *Order = NULL;
   try
   {
      Order = new TItemComplete;
      Order->Terminal       	= this->ID.Name;
      Order->TabType        	= TabTableSeat;
      Order->OrderKey      	= TempOrder->OrderLK;
      Order->OrderType      	= TempOrder->Status;
      Order->PrintChitNumber 	= false;
      Order->SetMenu			 	= TempOrder->PromoItem;
      Order->Note           	= TempOrder->Note;
      Order->HappyHour			= false;
      Order->TimeStamp 			= OrderTimeStamp;
		Order->OrderedLocation 	= TDeviceRealPalms::Instance().ID.Location;

      // Retrieve ITEM INFO.
      long ID              = TempOrder->Items[0];
		TItem * CurrentItem  = TDeviceRealPalms::Instance().Menus->FetchItemByID(ID);
		if(CurrentItem == NULL)
      {
         delete Order;
	      TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"Error Item not found by ID");
			return NULL;
      }
		Order->Palm_ID       = ID;
		bool ServingCourseFound = TDeviceRealPalms::Instance().Menus->FetchServingCourseByID(TempOrder->ServingCourseID, Order->ServingCourse);
      if(!ServingCourseFound)
      {
			TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Serving Course ID " + IntToStr(TempOrder->ServingCourseID) + "Not Found, assigning default.");
         Order->ServingCourse	= TDeviceRealPalms::Instance().Menus->DefaultServingCourse;
      }

		Order->Course        = CurrentItem->Course;
		Order->MenuName		= CurrentItem->MenuName;
		Order->MenuKey			= CurrentItem->MenuKey;
		Order->ItemType		= CurrentItem->ItemType;
		Order->Item		      = CurrentItem->Item;
		Order->Item_ID	      = CurrentItem->Item_ID;
		Order->ItemKitchenName = CurrentItem->ItemKitchenName;
		Order->CourseKitchenName = CurrentItem->CourseKitchenName;
		Order->FontInfo			 = CurrentItem->FontInfo;

      if(TempOrder->Container.size() > 0)
		{
			TItemSize *Size 		= CurrentItem->Sizes->SizeGetByPalmID(TempOrder->Container[0]);
         Order->PriceLevel0			= Size->Price;
         Order->PriceLevel1			= Size->HappyPrice;

			if(TGlobalSettings::Instance().InHappyHour)
			{
				Order->SetPriceLevel1();
			}
			else
			{
				Order->SetPriceLevel0();
			}
			Order->GSTPercent    = Size->GSTPercent;
			Order->Size          = Size->Name;
			Order->SetMenuMask	= Size->SetMenuMask;
         Order->SetMenu			= Size->SetMenuItem;
         Order->SetMenuMaster	= Size->SetMenuMaster;
			Order->PointsPercent = Size->PointsPercent;
			Order->ThirdPartyKey	= Size->ThirdPartyKey;
			Order->SizeKitchenName = Size->SizeKitchenName;
			// Sort Categories
         Order->Categories->CategoryCopyList(CurrentItem->Sizes->SizeGetByPalmID(TempOrder->Container[0])->Categories);

         if(Size->Recipes->Count != 0)
         {
            Order->SalesRecipesToApply->RecipeCopyList(Size->Recipes);
				double CostIncGst = 0;
				double OrderCost = 0;
            for(int i = 0; i < Size->Recipes->Count; i++)
            {
               OrderCost 	+= double(Size->Recipes->RecipeGet(i)->Cost);
					CostIncGst	+= double(Size->Recipes->RecipeGet(i)->Cost) *
									  double(Size->Recipes->RecipeGet(i)->CostGSTPercent + 100.0) / double(100.0);
            }
            Order->Cost = OrderCost;
            if(Order->Cost != 0)
            {
	         	Order->CostGSTPercent = double(((CostIncGst - OrderCost) * 100) / double(OrderCost));
            }
            else
            {
	         	Order->CostGSTPercent = 0;
            }
         }
         else
         {
            Order->Cost = CurrentItem->Sizes->SizeGetByPalmID(TempOrder->Container[0])->Cost;
	         Order->CostGSTPercent = CurrentItem->Sizes->SizeGetByPalmID(TempOrder->Container[0])->CostGSTPercent;
			}
		}
		else
		{
			delete Order;
			TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"Error Item has no sizes");
			return NULL;
		}

		// Palm now sends the New Price in the Price Adjust field.
		if (TempOrder->IsPriceAdjust)
		{
			TSecurityReference   *SecRef = new TSecurityReference;
			SecRef->TerminalName = this->ID.Name;
			SecRef->UserKey      = this->User.ContactKey;
			SecRef->Event        = SecurityTypes[secPriceAdjust];
			SecRef->From         = CurrToStrF(Order->Price(), ffCurrency, 2);
			Order->SetPriceLevelCustom(TempOrder->PriceAdjust);
			SecRef->To           = CurrToStrF(Order->Price(), ffCurrency, 2);
			SecRef->TimeStamp    = Order->TimeStamp;
			Order->Security->SecurityAdd(SecRef);
		}

	   Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
   	DBTransaction.StartTransaction();
   	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		Order->Security->SetSecurityRefNumber(TDBSecurity::GetNextSecurityRef(DBTransaction));
	   DBTransaction.Commit();


		TSecurityReference   *SecRef = new TSecurityReference;
		SecRef->TerminalName = this->ID.Name;
		SecRef->UserKey      = this->User.ContactKey;
		SecRef->Event        = SecurityTypes[secOrderedBy];
		SecRef->From         = this->User.Name;
		SecRef->To           = this->User.Initials;
		SecRef->TimeStamp    = Order->TimeStamp;
		Order->Security->SecurityAdd(SecRef);

		// sort sub orders.
		for(unsigned int i = 1;i < TempOrder->Items.size(); i++)
		{
			TItem * SubItem = TDeviceRealPalms::Instance().Menus->FetchItemByID(TempOrder->Items[i]);
			if(SubItem)
			{
				TItemSize *Size = SubItem->Sizes->SizeGetByPalmID(TempOrder->Container[i]);
				if(Size)
				{
					TItemCompleteSub *CurrentSubOrder = new TItemCompleteSub;
					CurrentSubOrder->Item 					= SubItem->Item;
					CurrentSubOrder->Item_ID 				= SubItem->Item_ID;
					CurrentSubOrder->Course 				= SubItem->Course;
					CurrentSubOrder->ItemKitchenName 	= SubItem->ItemKitchenName;
					CurrentSubOrder->CourseKitchenName 	= SubItem->CourseKitchenName;
					CurrentSubOrder->FontInfo		  		= SubItem->FontInfo;
               bool ServingCourseFound = TDeviceRealPalms::Instance().Menus->FetchServingCourseByID(TempOrder->ServingCourseID, CurrentSubOrder->ServingCourse);
               if(!ServingCourseFound)
               {
                  TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Serving Course ID " + IntToStr(TempOrder->ServingCourseID) + "Not Found, assigning default.");
                  CurrentSubOrder->ServingCourse	= TDeviceRealPalms::Instance().Menus->DefaultServingCourse;
               }


               Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
               DBTransaction.StartTransaction();
               TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
					CurrentSubOrder->Security->SetSecurityRefNumber(TDBSecurity::GetNextSecurityRef(DBTransaction));
               DBTransaction.Commit();

					TSecurityReference   *SecRef = new TSecurityReference;
					SecRef->TerminalName = this->ID.Name;
					SecRef->UserKey      = this->User.ContactKey;
					SecRef->Event        = SecurityTypes[secOrderedBy];
					SecRef->From         = this->User.Name;
					SecRef->To           = this->User.Initials;
					SecRef->TimeStamp    = Order->TimeStamp;
					CurrentSubOrder->Security->SecurityAdd(SecRef);
               CurrentSubOrder->PriceLevel0			= Size->Price;
               CurrentSubOrder->PriceLevel1			= Size->HappyPrice;

					if(TGlobalSettings::Instance().InHappyHour)
					{
      				CurrentSubOrder->SetPriceLevel1();
					}
					else
					{
      				CurrentSubOrder->SetPriceLevel0();
					}

					CurrentSubOrder->Size          = Size->Name;
               CurrentSubOrder->GSTPercent    = Size->GSTPercent;
					CurrentSubOrder->PointsPercent = Size->PointsPercent;
					CurrentSubOrder->ThirdPartyKey = Size->ThirdPartyKey;
					CurrentSubOrder->SizeKitchenName = Size->SizeKitchenName;
               // Sort Categories
               CurrentSubOrder->Categories->CategoryCopyList(Size->Categories);


               if(Size->Recipes->Count != 0)
               {
                  CurrentSubOrder->SalesRecipesToApply->RecipeCopyList(Size->Recipes);
						double CostIncGst = 0;
						double OrderCost = 0;
                  for (int j = 0; j < Size->Recipes->Count; j++)
                  {
                     OrderCost += double(Size->Recipes->RecipeGet(j)->Cost);
                     CostIncGst += double(Size->Recipes->RecipeGet(j)->Cost) *
                                   double(Size->Recipes->RecipeGet(j)->CostGSTPercent + 100.0) / double(100.0);
                  }
                  CurrentSubOrder->Cost = OrderCost;
                  if(CurrentSubOrder->Cost != 0)
                  {
				         CurrentSubOrder->CostGSTPercent = double(((CostIncGst - OrderCost) * 100) / double(OrderCost));
                  }
                  else
                  {
                     CurrentSubOrder->CostGSTPercent = 0;
                  }
               }
               else
               {
                  CurrentSubOrder->Cost = Size->Cost;
			         CurrentSubOrder->CostGSTPercent = Size->CostGSTPercent;
               }

               Order->SubOrders->SubOrderAdd(CurrentSubOrder);
               Order->MasterContainer = CurrentItem->Owner->Owner->Sizes->SizeGetByPalmID(TempOrder->MasterContainer)->Name;
            }
        	}
         else
         {
         	delete Order;
            return NULL;
         }
      }

      //Options
      for (std::vector<long>::iterator Option = TempOrder->Options.begin();
           Option != TempOrder->Options.end();Option++)
      {
         int PolarisedRawOptionID = (*Option);
         int RawOptionID = (PolarisedRawOptionID & 0x3F);
         bool IsNegativeOption = (PolarisedRawOptionID & 0x40);
         TItemOption *CurrentOption = CurrentItem->Options->OptionGet(RawOptionID-1);
         if(CurrentOption)
         {
            CurrentOption->IsPlus = !IsNegativeOption;
            Order->OptionsSelected->OptionAdd(CurrentOption);
         }
         else
         {
		      TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Option Not found " +
            			Order->Course + " " +
                     Order->Item + " " + "Option No." + IntToStr(RawOptionID));
         }
      }
	   return Order;
   }
   catch(Exception &Err)
   {
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"BuildOrderProper Excp Serving Course ID " + IntToStr(TempOrder->ServingCourseID));
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"BuildOrderProper Excp Serving Course Key " + IntToStr(Order->ServingCourse.ServingCourseKey));
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"BuildOrderProper Excp Serving Course Name " + Order->ServingCourse.Name);
   	delete Order;
      TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Err.Message);
      throw;
   }
}

void TDeviceRealPalms::CheckTabsSetMenuList(Database::TDBTransaction &DBTransaction)
{
try
   {
		std::set<long> TabsToRemove;

		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

      for(TTabsSetMenuList::iterator Tablist = TDeviceRealPalms::Instance().TabsSetMenuList.begin();
          Tablist != TDeviceRealPalms::Instance().TabsSetMenuList.end();
          advance(Tablist,1))
      {
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
			" SELECT TAB.TAB_KEY "
			" FROM TAB "
			" WHERE "
			"  TAB.TAB_KEY = :Loc_TAB_KEY";
			IBInternalQuery->ParamByName("Loc_TAB_KEY")->AsInteger = Tablist->first;
			IBInternalQuery->ExecQuery();
			if(!IBInternalQuery->RecordCount)
         { // Ok Remove this TAB.
            TabsToRemove.insert(Tablist->first);
         }
      }

      for (std::set<long>::iterator Removelist = TabsToRemove.begin();
           Removelist != TabsToRemove.end(); advance(Removelist,1))
      {
         TDeviceRealPalms::Instance().TabsSetMenuList.erase(*Removelist);
      }
	}
	catch(Exception &E)
	{
   }
}

void TPalm::ProcessTransmission()
{
	TIRPacket *Packet = Inbound;
   Packet->Clear();
   unsigned char* ptrStream = ptrRawData;

   Status.Error = proA_Ok;
   Status.ErrorMsg = "";

   static unsigned char LastCommand;
   MESSAGE_TYPE ExpectedCommand = (MESSAGE_TYPE)*ptrStream++;
   if(ExpectedCommand != Message_Packing)
   {
      Packet->Header.Command = ExpectedCommand;
      Packet->Header.TransNo = *ptrStream++;
      Packet->Header.InfoSyncNo = *ptrStream++;
      Packet->Header.MenuSync1No = *ptrStream++;
      Packet->Header.MenuSync2No = *ptrStream++;
      Packet->Header.HighStaffIDNo = *ptrStream++;
      Packet->Header.LowStaffIDNo = *ptrStream++;
      Packet->Header.ErrorNo = *ptrStream++;
      Packet->Header.HighPktSize = *ptrStream++;
      Packet->Header.LowPktSize = *ptrStream++;

		Packet->Process(ptrStream);

		if(this == TDeviceRealPalms::Instance().PalmNotFound &&
			ExpectedCommand != REGISTRATION && ExpectedCommand != INFO_SYNC)
   	{
	      Status.Error = Err_DeviceKey_Not_Found;
         BuildPacket(NULL);
         ClearInbound();
	   }
      else if(Status.Error != proA_Ok)
      {

         UnicodeString msgType = "";
         switch(Packet->Header.Command)
         {
				case KEEPALIVE:					msgType = "KEEPALIVE";					break;
				case INFO_SYNC:					msgType = "INFO_SYNC";					break;
				case MENU_SYNC:					msgType = "MENU_SYNC";					break;
				case DRINK_SYNC:					msgType = "DRINK_SYNC";					break;
				case LOGIN:							msgType = "LOGIN";						break;
				case SEAT_NAMES:					msgType = "SEAT_NAMES";					break;
				case ORDERS:						msgType = "ORDERS";						break;
				case REQUEST_RECEIPT:			msgType = "REQUEST_RECEIPT";			break;
				case TRANSFER_TABLE_ORDERS:	msgType = "TRANSFER_TABLE_ORDERS";	break;
				case RETRIEVE_ORDERS:			msgType = "RETRIEVE_ORDERS";			break;
				case REGISTRATION:				msgType = "REGISTRATION";				break;
				case CALL_AWAY:					msgType = "CALL AWAY";					break;
				default:								msgType = "UNKNOWN";
            break;
         }

			TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"-------------- FAILED PALM PACKET FAILED STATUS-------------","Palm #" + IntToStr(ID.DeviceKey));
         TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,msgType,"Palm #" + IntToStr(ID.DeviceKey));
         TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"CommandNo : " + IntToStr(Packet->Header.Command),"Palm #" + IntToStr(ID.DeviceKey));
         TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"TransNo : " + IntToStr(Packet->Header.TransNo),"Palm #" + IntToStr(ID.DeviceKey));
         TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"InfoSyncNo : " + IntToStr(Packet->Header.InfoSyncNo),"Palm #" + IntToStr(ID.DeviceKey));
         TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"MenuSync1No : " + IntToStr(Packet->Header.MenuSync1No),"Palm #" + IntToStr(ID.DeviceKey));
         TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"MenuSync2No : " + IntToStr(Packet->Header.MenuSync2No),"Palm #" + IntToStr(ID.DeviceKey));
         TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"Staff ID No : " + IntToStr(Packet->StaffIDNo),"Palm #" + IntToStr(ID.DeviceKey));
         TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"Packet Size " + IntToStr(Packet->PktSize),"Palm #" + IntToStr(ID.DeviceKey));
         TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"ERROR NO : " + IntToStr(Packet->Header.ErrorNo),"Palm #" + IntToStr(ID.DeviceKey));
         TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"------------------------------------------------------------","Palm #" + IntToStr(ID.DeviceKey));
         TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"Was Attempting : " + msgType + " # " + IntToStr(Packet->Header.Command),"Palm #" + IntToStr(ID.DeviceKey));
         TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"------------------------------------------------------------","Palm #" + IntToStr(ID.DeviceKey));
         TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"Returned empty packet with Error Message to palm.","Palm #" + IntToStr(ID.DeviceKey));

         BuildPacket(NULL);
         ClearInbound();

         TransNo = Packet->Header.TransNo;
      }
      else
      {
			UnicodeString msgType = "";
         switch(Packet->Header.Command)
         {
				case KEEPALIVE:					msgType = "KEEPALIVE";					break;
				case INFO_SYNC:					msgType = "INFO_SYNC";					break;
				case MENU_SYNC:					msgType = "MENU_SYNC";					break;
				case DRINK_SYNC:					msgType = "DRINK_SYNC";					break;
				case LOGIN:							msgType = "LOGIN";						break;
				case SEAT_NAMES:					msgType = "SEAT_NAMES";					break;
				case ORDERS:						msgType = "ORDERS";						break;
				case REQUEST_RECEIPT:			msgType = "REQUEST_RECEIPT";			break;
				case TRANSFER_TABLE_ORDERS:	msgType = "TRANSFER_TABLE_ORDERS";	break;
				case RETRIEVE_ORDERS:			msgType = "RETRIEVE_ORDERS";			break;
				case REGISTRATION:				msgType = "REGISTRATION";				break;
				case CALL_AWAY:					msgType = "CALL AWAY";					break;
				default:								msgType = "UNKNOWN";						break;
			}

			TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"-------------- PALM PACKET STATUS----------------","Palm #" + IntToStr(ID.DeviceKey));
         TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,msgType,"Palm #" + IntToStr(ID.DeviceKey));
         TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"CommandNo : " + IntToStr(Packet->Header.Command),"Palm #" + IntToStr(ID.DeviceKey));
         TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"TransNo : " + IntToStr(Packet->Header.TransNo),"Palm #" + IntToStr(ID.DeviceKey));
         TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"InfoSyncNo : " + IntToStr(Packet->Header.InfoSyncNo),"Palm #" + IntToStr(ID.DeviceKey));
         TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"MenuSync1No : " + IntToStr(Packet->Header.MenuSync1No),"Palm #" + IntToStr(ID.DeviceKey));
			TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"MenuSync2No : " + IntToStr(Packet->Header.MenuSync2No),"Palm #" + IntToStr(ID.DeviceKey));
			TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"Staff ID No : " + IntToStr(Packet->StaffIDNo),"Palm #" + IntToStr(ID.DeviceKey));
			TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"Error No : " + IntToStr(Packet->Header.ErrorNo),"Palm #" + IntToStr(ID.DeviceKey));
         TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"------------------------------------------------------------","Palm #" + IntToStr(ID.DeviceKey));
         TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"ATTEMPTING : " + msgType,"Palm #" + IntToStr(ID.DeviceKey));
			TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"------------------------------------------------------------","Palm #" + IntToStr(ID.DeviceKey));

         TransNo = Packet->Header.TransNo;

         switch(ExpectedCommand)
         {
            case KEEPALIVE :
            {
               //unsigned char HiCRC = *ptrStream++;
					ptrStream++;
					//unsigned char LowCRC = *ptrStream++;
					ptrStream++;
					CheckSyncNo();
					BuildPacket(NULL);
					ClearInbound();
					break;
				}
				case REGISTRATION :
				{
					try
					{
						if(Packet->PktSize != 0)
						{
							Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
							DBTransaction.StartTransaction();
							// Unique Name
							Packet->Register.DeviceIP = UnicodeString((char *)Packet->ptrReadBuffer);
							Packet->ptrReadBuffer += Packet->Register.DeviceIP.Length()+1; // Also Step over NULL
							// Palm ID of the palm to replace if any.
							unsigned short LowBytePalmID, HighBytePalmID;
							HighBytePalmID = *Packet->ptrReadBuffer++;
							LowBytePalmID = *Packet->ptrReadBuffer++;
							Packet->Register.PalmUniqueID = (HighBytePalmID * 256) + LowBytePalmID;
							// Palm Name
							Packet->Register.NewDeviceName = UnicodeString((char *)Packet->ptrReadBuffer);
							Packet->ptrReadBuffer += Packet->Register.NewDeviceName.Length()+1;  // Also Step over NULL
							// Registration PIN
							Packet->Register.RegistrationPIN = "";
							char PIN[PIN_Length+1];
							PIN[PIN_Length] = NULL;
							for(int i = 0 ; i < PIN_Length; i++)
							{
								 PIN[i]= (char)*Packet->ptrReadBuffer++;
							}
							Packet->Register.RegistrationPIN = UnicodeString(PIN);
							// Registration Flags
							Packet->Register.RegistrationFlag = *Packet->ptrReadBuffer++;
							HighBytePalmID = *Packet->ptrReadBuffer++;
							LowBytePalmID = *Packet->ptrReadBuffer++;
							Packet->Register.SyncPrintingToPalmKey = (HighBytePalmID * 256) + LowBytePalmID;

							Register(DBTransaction);
							DBTransaction.Commit();
						}
						else
                  {
							Status.Error = Err_Mismatched_Message_Data;
                     TManagerLogs::Instance().Add(__FUNC__,"REGISTRATION","Message has no Palm Unique ID");
                  }
               }
               __finally
               {
                  if(Status.Error != proA_Ok)
                  {
							BuildPacket(NULL);
                     ClearInbound();
                  }
               }
               break;
            }
				case SEAT_NAMES :
            {
					try
					{
						Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
						DBTransaction.StartTransaction();
						fReceiveSeatNames(DBTransaction,Packet);
						DBTransaction.Commit();
					}
					__finally
					{
						BuildPacket(NULL);
						ClearInbound();
					}
					break;
				}
				case ORDERS :
				{
					DWORD timerStart = ::GetTickCount();
					try
					{
						if(Packet->PktSize != 0)
						{
							TIRTable  *TempTable;
							try
							{
								#if (DEBUGLEVEL & debugExpandPalmComms )
									TManagerLogs::Instance().Add(__FUNC__,"NEW ORDERS","NEW ORDERS MESSAGE");
								#endif
								do     
								{
									//memset(Packet->ptrReadBuffer+2, 127, Packet->PktSize-2);

									TempTable = new TIRTable(Packet,Packet->Header.Command);
									Packet->Orders.Tables->Add(TempTable);
									if(Packet->ptrReadBuffer > (Packet->ptrBuffer + Packet->PktSize))
									{
										Status.ErrorMsg = "Error Decoding Message Helix TIRTable";
										Status.Error = Err_Currupt_Message;
										LogHexMessage("EHelixError",Packet->ptrBuffer,Packet->PktSize);
										throw EHelixError(Status.ErrorMsg);
									}
								}while(!Packet->LastTable);

								if(Status.Error == proA_Ok)
								{
									Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
									DBTransaction.StartTransaction();
									UpdateTransactionNumber(DBTransaction);
									NewOrders(DBTransaction);
									DBTransaction.Commit();
								}
							}
							catch(EHelixError &Err)
							{
								  Status.ErrorMsg = Err.ErrMsg + " ORDERS";
								  Status.Error = Err_Currupt_Message;
								  LogHexMessage("EHelixError",Packet->ptrBuffer,Packet->PktSize);
							}
							catch (Exception &E)
							{
								  Status.ErrorMsg = E.Message + " ORDERS";
								  Status.Error = Err_Database;
								  LogHexMessage(E.Message + " ORDERS", Packet->ptrBuffer, Packet->PktSize);
							}
						}
					}
					__finally
					{
						int orderCount = Packet->OrderCount;

						BuildPacket(NULL);
						ClearInbound();

						TDeviceRealPalms::Instance().BlueTooth->SetNewOrderTimeout(::GetTickCount() - timerStart, orderCount);

//						TDeviceRealPalms::Instance().BlueTooth->SetSetOrderProcDelay( (double(::GetTickCount() - DelayProc)) / double(Packet->OrderCount));
					}
               break;
				}
				case MENU_SYNC :
            {
               try
               {
                  if(Packet->PktSize == 0)
                  {
                     if(TDeviceRealPalms::Instance().Menus->MenuFood != NULL)
                     {
                        TComms::Instance().Process_Outbound->Clear();
                        TComms::Instance().Process_Outbound->CopyFrom(TDeviceRealPalms::Instance().Menus->MenuUploadFood,0);
                        // Add it to the Palms Transmission list.
                        BuildPacket(TComms::Instance().Process_Outbound);
//								SeatNames.Load();
//								UpdateNames();
                     }
                     else
                     {
                        Status.Error = Wrn_No_Menu_Found;
                        BuildPacket(NULL);
                     }
						}
                  else
                  {
                     // Add transmit falure to palm.
							Status.Error = Err_Mismatched_Message_Data;
							TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Err_Mismatched_Message_Data in MENU_SYNC");
                     BuildPacket(NULL);
                  }
               }
               __finally
               {
                  if(Status.Error != proA_Ok)
                  {
                     BuildPacket(NULL);
                  }
                  ClearInbound();
               }
               break;
            }
				case DRINK_SYNC :
            {
               try
               {
                  if(Packet->PktSize == 0)
                  {
                     if(TDeviceRealPalms::Instance().Menus->MenuDrinks != NULL)
							{
                        TComms::Instance().Process_Outbound->Clear();
                        TComms::Instance().Process_Outbound->CopyFrom(TDeviceRealPalms::Instance().Menus->MenuUploadDrinks,0);
                        // Add it to the Palms Transmission list.
                        BuildPacket(TComms::Instance().Process_Outbound);
							}
                     else
                     {
                        Status.Error = Wrn_No_Menu_Found;
                        BuildPacket(NULL);
                     }
                  }
                  else
                  {
								Status.Error = Err_Mismatched_Message_Data;
								TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Err_Mismatched_Message_Data in DRINK_SYNC");
	                     BuildPacket(NULL);
                  }
               }
               __finally
               {
                  if(Status.Error != proA_Ok)
                  {
                     BuildPacket(NULL);
                  }
                  ClearInbound();
					}
               break;
            }
				case LOGIN :
            {
					try
               {
						if(Packet->PktSize != 0)
						{
							Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
							DBTransaction.StartTransaction();

							//unsigned char PalmDeviceIPSize = (unsigned char)*Packet->ptrReadBuffer++;
							Packet->Login.DeviceIP = UnicodeString((char *)Packet->ptrReadBuffer);
							Packet->ptrReadBuffer += Packet->Login.DeviceIP.Length();
							Packet->ptrReadBuffer++; // Step over NULL.
							unsigned char LowByteID, HighByteID;

							HighByteID = *Packet->ptrReadBuffer++;
							LowByteID = *Packet->ptrReadBuffer++;

							Packet->Login.NewStaffID = (HighByteID * 256) + LowByteID;

							Packet->Login.UsersPIN = "";
							char PIN[PIN_Length+1];
							PIN[PIN_Length] = NULL;
							for(int i = 0 ; i < PIN_Length; i++)
							{
								 PIN[i]= (char)*Packet->ptrReadBuffer++;
							}
							Packet->Login.UsersPIN = UnicodeString(PIN);
							Login(DBTransaction);
							DBTransaction.Commit();
						}
						else
						{
							Status.Error = Err_Mismatched_Message_Data;
							TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Err_Mismatched_Message_Data in LOGIN");
                     BuildPacket(NULL);
                  }
               }
               __finally
               {
                  if(Status.Error != proA_Ok)
                  {
                     BuildPacket(NULL);
                  }
                  ClearInbound();
               }
               break;
            }
				case INFO_SYNC :
				{
               try
					{
						Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
						DBTransaction.StartTransaction();
						if(Packet->PktSize == 0)
						{
							InfoSync(DBTransaction);
                  }
                  else
						{
							Status.Error = Err_Mismatched_Message_Data;
							TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Err_Mismatched_Message_Data in INFO_SYNC");
                     BuildPacket(NULL);
						}
						DBTransaction.Commit();
               }
               __finally
               {
                  if(Status.Error != proA_Ok)
                  {
                     BuildPacket(NULL);
                  }
                  ClearInbound();
               }
               break;
				}
				case CALL_AWAY :
            {
               try
               {
                  try
                  {
                     if(Packet->PktSize != 0)
                     {
                        Packet->Section = TIRPacket::bsCallAways;
                        do
                        {
                           TIRTable *TempTable = new TIRTable(Packet,Packet->Header.Command);
                           Packet->CallAways.Tables->Add(TempTable);
                           if(Packet->ptrReadBuffer > (Packet->ptrBuffer + Packet->PktSize))
                           {
                              Status.ErrorMsg = "Error Decoding Message Helix CALL_AWAY Orders Section";
										Status.Error = Err_Currupt_Message;
                              LogHexMessage("Error Decoding Message Helix CALL_AWAY Orders Section",Packet->ptrBuffer,Packet->PktSize);
                              break;
                           }
                        }while(!Packet->LastTable);

                        if(Status.Error == proA_Ok)
								{
									Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
									DBTransaction.StartTransaction();
									CallAways(DBTransaction);
									DBTransaction.Commit();
								}
							}
                     else
                     {
								Status.Error = Err_Mismatched_Message_Data;
								TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Err_Mismatched_Message_Data in CALL_AWAY");
								BuildPacket(NULL);
                     }
                  }
                  catch(EHelixError &Err)
                  {
                       Status.ErrorMsg = Err.ErrMsg + " CALL_AWAY EHelixError";
							  Status.Error = Err_Currupt_Message;
                       LogHexMessage( Err.ErrMsg + " CALL_AWAY EHelixError",Packet->ptrBuffer,Packet->PktSize);
                       break;
                  }
               }
               __finally
               {
						if(Status.Error == proA_Ok)
                  {
                     BuildPacket(NULL);
                  }
                  ClearInbound();
               }

               break;
            }
				case REQUEST_RECEIPT :
            {
					try
					{
						if(Packet->PktSize != 0)
						{
							try
							{
								// Read in sections flag
								Packet->FinalBill = ptPartial;
								Packet->LastTable = false;
								Packet->LastSeat = false;
								Packet->LastItem = false;
								Packet->Section = TIRPacket::bsBilling;
								do
								{
									TIRTable *TempTable = new TIRTable(Packet,Packet->Header.Command);
									Packet->Billing.Tables->Add(TempTable);
									if(Packet->ptrReadBuffer > (Packet->ptrBuffer + Packet->PktSize))
									{
									  Status.ErrorMsg = "Error Decoding Message Helix REQUEST_RECEIPT Billing Section";
									  Status.Error = Err_Currupt_Message;
									  LogHexMessage("Error Decoding Message Helix REQUEST_RECEIPT Billing Section",Packet->ptrBuffer,Packet->PktSize);
									  throw EHelixError(Status.ErrorMsg);
									}
								}while(!Packet->LastTable);

								if(Status.Error == proA_Ok)
								{
									Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
									DBTransaction.StartTransaction();
									BillSeats(DBTransaction);
									DBTransaction.Commit();
								}
							}
							catch(EHelixError &Err)
							{
								  Status.ErrorMsg = Err.ErrMsg + " REQUEST_RECEIPT EHelixError";
								  Status.Error = Err_Currupt_Message;
								  LogHexMessage( Err.ErrMsg + " REQUEST_RECEIPT EHelixError",Packet->ptrBuffer,Packet->PktSize);
								  break;
							}
						}
						else
						{
							Status.Error = Err_Mismatched_Message_Data;
							TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Err_Mismatched_Message_Data in REQUEST_RECEIPT");
						}
               }
					__finally
					{
						BuildPacket(NULL);
						ClearInbound();
					}
					break;
				}
				case RETRIEVE_ORDERS :
				{
					long DelayProc = ::GetTickCount();
					try
					{
						Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
						DBTransaction.StartTransaction();
						if(Packet->PktSize != 0)
						{
//							DownloadOrders =(DownloadStatus)Packet->ptrReadBuffer[0];
							CurrentTable = Packet->ptrReadBuffer[0];

							TPalmUserSettings newPalmUserSettings = (Packet->ptrReadBuffer[1] << 8) + Packet->ptrReadBuffer[2];
							fUpdateUserSettings(newPalmUserSettings);
/*
							Database::TDBTransaction ContactTransaction(TDeviceRealPalms::Instance().DBControl);
							std::auto_ptr<TContactStaff> Staff(new TContactStaff(ContactTransaction));
							Staff->GetContactDetails(ContactTransaction, User.ContactKey, User);
							User.PalmUserSettings = (Packet->ptrReadBuffer[1] << 8) + Packet->ptrReadBuffer[2];
							Staff->GetContactDetails(ContactTransaction,User.ContactKey, User);
							ContactTransaction.Commit(); */
//							Staff->SetPalmUserSettings(User.ContactKey, (Packet->ptrReadBuffer[1] << 8) + Packet->ptrReadBuffer[2]);

//							ScreenLock	= UserSettingsFlag & 0x01;
//							ClassicMode	= UserSettingsFlag & 0x02;
//							SoundLevel	= ((UserSettingsFlag & 0x04)?1:0) + ((UserSettingsFlag & 0x08)?2:0);

							UpdateTransactionNumber(DBTransaction);
							TComms::Instance().Process_Outbound->Clear();
							RepackageOrders(DBTransaction,TComms::Instance().Process_Outbound,0);
							BuildPacket(TComms::Instance().Process_Outbound);
						}
						else
						{
							UpdateTransactionNumber(DBTransaction);
							TComms::Instance().Process_Outbound->Clear();
							RepackageOrders(DBTransaction,TComms::Instance().Process_Outbound,0);
							BuildPacket(TComms::Instance().Process_Outbound);
						}
						DBTransaction.Commit();
					}
               __finally
               {
                  if(Status.Error != proA_Ok)
                  {
							BuildPacket(NULL);
                  }
                  ClearInbound();
						long TickCount = ::GetTickCount();
                  TDeviceRealPalms::Instance().BlueTooth->SetGetOrderProcDelay(TickCount - DelayProc);
               }
               break;
            }
            default :
            {
               try
               {
                  TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Message Not Reconised. Bad Command :" + IntToStr(ExpectedCommand));
                  Status.ErrorMsg = "Message Not Reconised. Bad Command :" + IntToStr(ExpectedCommand);
                  Status.Error = Err_Currupt_Message;
                  LogHexMessage("Message Not Reconised. Bad Command :" + IntToStr(ExpectedCommand),Packet->ptrBuffer,Packet->PktSize);
               }
					__finally
					{
						BuildPacket(NULL);
						ClearInbound();
					}
				}
			}
		}
	}
	LastCommand = ExpectedCommand;
}
//------------------------------------------------------------------------------
void TPalm::fUpdateUserSettings(const TPalmUserSettings &newPalmUserSettings)
{
	Database::TDBTransaction dbTransaction(TDeviceRealPalms::Instance().DBControl);
	dbTransaction.StartTransaction();

	std::auto_ptr<TContactStaff> Staff(new TContactStaff(dbTransaction));

	Staff->GetContactDetails(dbTransaction, User.ContactKey, User);
	if (User.PalmUserSettings != newPalmUserSettings)
	{
		User.PalmUserSettings = newPalmUserSettings;
		Staff->SetContactDetails(dbTransaction,User.ContactKey, User);
	}
	dbTransaction.Commit();
}
//------------------------------------------------------------------------------
void TPalm::fReceiveSeatNames(Database::TDBTransaction &DBTransaction,TIRPacket *Packet)
{
	// Valid message?
	if (Packet->PktSize != 0)
	{
		try
		{
			// Container for seat names
			TPalmSeatNames	SeatNames;
			// Holders for reading message data into
			TTableNumber	TableNumber	= 0;
			TSeatNumber		SeatNumber	= 0;
			// Indicated by MSB
			bool				IsLastTable	= false;
			bool				IsLastSeat	= false;
			do
			{
				Packet->ReadFileBuf(&TableNumber, sizeof(TableNumber));
				IsLastTable = TableNumber & 0x80;
				SeatNames.AddTable(TPalmTable(TableNumber & 0x7F));
				do
				{
					Packet->ReadFileBuf(&SeatNumber, sizeof(SeatNumber));
					IsLastSeat = SeatNumber & 0x80;

					// Read in the seat name. Delimited by MSB
					UnicodeString		SeatName;
					unsigned char	Char = NULL;

					while (!(Char & 0x80))
					{
						Packet->ReadFileBuf(&Char, sizeof(Char));
						if (!(Char & 0x7F))
						{
							break;
						}
						SeatName += static_cast<char>(Char & 0x7F);
					}
					SeatNames.Table(SeatNames.TableCount()-1).AddSeat(TPalmSeat(SeatNumber & 0x7F, SeatName.Trim()));
				}
				while (!IsLastSeat);

				if(Packet->ptrReadBuffer > Packet->ptrBuffer + Packet->PktSize)
				{
				  Status.ErrorMsg = "Error Decoding Message Helix SEAT_NAMES";
				  Status.Error = Err_Currupt_Message;
				  LogHexMessage("Error Decoding Message Helix SEAT_NAMES", Packet->ptrBuffer, Packet->PktSize);
				  throw EHelixError(Status.ErrorMsg);
				}
			}
			while (!IsLastTable);

			if(Status.Error == proA_Ok)
			{
				SeatNames.Save(DBTransaction);
			}
		}
		catch(EHelixError &Err)
		{
			Status.ErrorMsg = Err.ErrMsg + " SEAT_NAMES EHelixError";
			Status.Error = Err_Currupt_Message;
			LogHexMessage( Err.ErrMsg + " SEAT_NAMES EHelixError", Packet->ptrBuffer, Packet->PktSize);
		}
	}
	else
	{
		Status.Error = Err_Mismatched_Message_Data;
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Err_Mismatched_Message_Data in SEAT_NAMES");
	}
}
//------------------------------------------------------------------------------
void TDeviceRealPalms::Initialise(Database::TDBTransaction &DBTransaction)
{
	TDeviceRealControl::Initialise(DBTransaction);
   Menus->Initialise(DBTransaction);
	BlueTooth->Initialise(DBTransaction);
}

void TDeviceRealPalms::CreateDevice(Database::TDBTransaction &DBTransaction)
{
	if(Locate(DBTransaction))
	{
		Load(DBTransaction);
		CheckLocation(DBTransaction);
	}
	else
	{
		CheckLocation(DBTransaction);
		Create(DBTransaction);
	}
}

void TDeviceRealPalms::SyncPrintingTo(Database::TDBTransaction &DBTransaction,TPalm * PalmSource,TPalm * PalmDest)
{
	if(PalmSource != PalmDest)
	{
		TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBQuery2 = DBTransaction.Query(DBTransaction.AddQuery());		
		try
		{
			IBQuery->Close();
			IBQuery->SQL->Text =
			" SELECT "
			" * "
			" FROM "
			"  CONNECTIONS "
			" WHERE "
			" DEVICE_KEY = :DEVICE_KEY";
			IBQuery->ParamByName("DEVICE_KEY")->AsInteger = PalmSource->ID.DeviceKey;
			IBQuery->ExecQuery();
			for(;!IBQuery->Eof; IBQuery->Next())
			{
				int NewConnectionKey = 0;
				IBQuery1->Close();
				IBQuery1->SQL->Text =
				"SELECT CONNECT_KEY "
				"FROM "
					"CONNECTIONS "
				"WHERE "
					"DEVICE_KEY = :DEVICE_KEY AND "
					"CONNECT_NAME = :CONNECT_NAME AND "
					"CONNECT_TYPE = :CONNECT_TYPE";
				IBQuery1->ParamByName("DEVICE_KEY")->AsString = PalmDest->ID.DeviceKey;
				IBQuery1->ParamByName("CONNECT_NAME")->AsString = IBQuery->FieldByName("CONNECT_NAME")->AsString;
				IBQuery1->ParamByName("CONNECT_TYPE")->AsInteger = IBQuery->FieldByName("CONNECT_TYPE")->AsInteger;
				IBQuery1->ExecQuery();
				if(!IBQuery1->RecordCount)
				{
					IBQuery1->Close();
					IBQuery1->SQL->Text = "SELECT GEN_ID(GEN_CONNECTIONS, 1) FROM RDB$DATABASE";
					IBQuery1->ExecQuery();
					NewConnectionKey = IBQuery1->Fields[0]->AsInteger;

					IBQuery1->Close();
					IBQuery1->SQL->Text =
					"INSERT INTO CONNECTIONS ("
						"CONNECT_KEY,"
						"DEVICE_KEY,"
						"CONNECT_NAME,"
						"CONNECT_ID,"
						"CONNECT_TYPE,"
						"DISABLED,"
						"PROFILE_KEY) "
					"VALUES ("
						":CONNECT_KEY,"
						":DEVICE_KEY,"
						":CONNECT_NAME,"
						":CONNECT_ID,"
						":CONNECT_TYPE,"
						":DISABLED,"
						":PROFILE_KEY);";
					IBQuery1->ParamByName("CONNECT_KEY")->AsInteger = NewConnectionKey;
					IBQuery1->ParamByName("DEVICE_KEY")->AsInteger = PalmDest->ID.DeviceKey;
					IBQuery1->ParamByName("CONNECT_NAME")->AsString = IBQuery1->FieldByName("CONNECT_NAME")->AsString;
					IBQuery1->ParamByName("CONNECT_ID")->AsInteger = IBQuery1->FieldByName("CONNECT_ID")->AsInteger;
					IBQuery1->ParamByName("CONNECT_TYPE")->AsInteger = IBQuery1->FieldByName("CONNECT_TYPE")->AsInteger;
					IBQuery1->ParamByName("PROFILE_KEY")->AsInteger = IBQuery1->FieldByName("PROFILE_KEY")->AsInteger;
					IBQuery1->ParamByName("DISABLED")->AsString = "F";
					IBQuery1->ExecQuery();
				}
				else
				{
					NewConnectionKey = IBQuery1->FieldByName("CONNECT_KEY")->AsInteger;
				}
				// Set the tables caption.
				IBQuery1->Close();
				IBQuery1->SQL->Text =
				" SELECT "
				" CONNECT_KEY, MENU_KEY, COURSE_KEY, COURSE_NAME "
				" FROM "
				"  PRNORDER "
				" WHERE "
				" CONNECT_KEY = :CONNECT_KEY";
				IBQuery1->ParamByName("CONNECT_KEY")->AsInteger =
					IBQuery->FieldByName("CONNECT_KEY")->AsInteger;
				IBQuery1->ExecQuery();
				for(;!IBQuery1->Eof; IBQuery1->Next())
				{
					IBQuery2->Close();
					IBQuery2->SQL->Text =
						" SELECT "
						" CONNECT_KEY, MENU_KEY, COURSE_KEY, COURSE_NAME "
						" FROM "
						"  PRNORDER "
						" WHERE "
						" CONNECT_KEY = :CONNECT_KEY AND "
						" MENU_KEY = :MENU_KEY AND "
						" COURSE_KEY = :COURSE_KEY AND "
						" COURSE_NAME = :COURSE_NAME";
					IBQuery2->ParamByName("CONNECT_KEY")->AsString = NewConnectionKey;
					IBQuery2->ParamByName("MENU_KEY")->AsString = IBQuery1->FieldByName("MENU_KEY")->AsString;
					IBQuery2->ParamByName("COURSE_KEY")->AsString = IBQuery1->FieldByName("COURSE_KEY")->AsString;
					IBQuery2->ParamByName("COURSE_NAME")->AsString = IBQuery1->FieldByName("COURSE_NAME")->AsString;
					IBQuery2->ExecQuery();
					if(!IBQuery2->RecordCount)
					{
						IBQuery2->Close();
						IBQuery2->SQL->Text = "SELECT GEN_ID(GEN_PRNORDER, 1) FROM RDB$DATABASE";
						IBQuery2->ExecQuery();
						int NewPrnOrderKey = IBQuery2->Fields[0]->AsInteger;

						IBQuery2->Close();
						IBQuery2->SQL->Text =
						"INSERT INTO PRNORDER ("
							"PRNORD_KEY,"
							"CONNECT_KEY,"
							"MENU_KEY,"
							"COURSE_KEY,"
							"COURSE_NAME) "
						"VALUES ("
							":PRNORD_KEY,"
							":CONNECT_KEY,"
							":MENU_KEY,"
							":COURSE_KEY,"
							":COURSE_NAME);";
						IBQuery2->ParamByName("PRNORD_KEY")->AsInteger = NewPrnOrderKey;
						IBQuery2->ParamByName("CONNECT_KEY")->AsString = NewConnectionKey;
						IBQuery2->ParamByName("MENU_KEY")->AsString = IBQuery1->FieldByName("MENU_KEY")->AsString;
						IBQuery2->ParamByName("COURSE_KEY")->AsString = IBQuery1->FieldByName("COURSE_KEY")->AsString;
						IBQuery2->ParamByName("COURSE_NAME")->AsString = IBQuery1->FieldByName("COURSE_NAME")->AsString;
						IBQuery2->ExecQuery();
					}
				}
			}
		}
		catch(Exception &E)
		{
			TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		}
	}
}

TPalmMessage::TPalmMessage(UnicodeString inFrom, UnicodeString inMessage, TDateTime inReceived)
{
	From = inFrom;
	Message = inMessage;
	Received = inReceived;
	Sent = false;
}

void TPalm::ClearMessageQue()
{
	for (TListPalmMessage::iterator RemoveMsg = Messages.begin();
		  RemoveMsg != Messages.end();)
	{
		if(RemoveMsg->Sent == true)
		{
			Messages.erase(RemoveMsg);
			RemoveMsg = Messages.begin();
		}
		else
		{
			std::advance(RemoveMsg,1);
		}
	}
}

void TPalm::ProcessKitchenModFinal(Database::TDBTransaction &DBTransaction,TList * OrdersList)
{
	try
	{
		if(TDeviceRealPalms::Instance().KitchenMod->Enabled)
		{
			TPaymentTransaction Transaction(DBTransaction);

			Transaction.Orders->Assign(OrdersList);

			if (Transaction.Orders->Count > 0)
			{
				TReqPrintJob *Request = new TReqPrintJob(&TDeviceRealPalms::Instance());
				Request->Transaction = &Transaction;
				Request->Waiter = this->User.Name;
				Request->SenderType = devPalm;
//				bool ReqPrintChitNumber = false;

				Kitchen->GetPrintouts(DBTransaction,Request,pfChefMate_Printer);
				Request->Printouts->FilterForChefMate();
				if (!Request->Printouts->Print(devPC))
				{
					Status.Error = Wrn_Printer_Offline;
				}
				delete Request;
			}
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}




