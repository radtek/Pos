//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop


#include "Data.h"       // So Plams can place orders.
#include "Palm.h"
#include "Comms.h"
#include "CrcTable.h"   // CRC Message checking.
#include <set>
#include <map>
#include <iterator>
#include "MMRegistry.h"
#include "ManagerStock.h"
#include "TableManager.h"
#include "Tabs.h"
#include "Orders.h"

#pragma package(smart_init)

#define BYTESWAP(val) (unsigned short)(((val << 8) & 0xff00) + ((val >> 8) & 0x00ff))
//------------------------------------------------------------------------------
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
int __fastcall OrderSeatNames(void *Item1, void *Item2)
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
}
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
      return 0;
   }
   else
   {
      return 1;
   }
}

__fastcall TDeviceRealPalms::TDeviceRealPalms()
{
   // The first connect made will generate the Transmission list for Plam Trans Objects..
	PalmList = new TList;
// Always contains a Palm Not found Error Message.
   PalmNotFound = new TPalm();
   PalmNotFound->ID.DeviceID = PalmNotFoundID;
   PalmNotFound->ID.Name = "000000000000PalmNotFound";
// Always contains a Info Sync Packet... (A freash one is built in the Retrieve Palm routuine.)
   PalmRegister = new TPalm();
   PalmRegister->ID.DeviceID = 0;
   PalmRegister->ID.Name = "000000000000Re-register";
   Registering = false;
   Reregistering = false;
   RegisteringTimer = 0;
   NewPalmRegisterName = "";

      OSVERSIONINFO osvi;
   char szVersion [80];
   memset(&osvi, 0, sizeof(OSVERSIONINFO));
   osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
   GetVersionEx (&osvi);
   OS.dwPlatformId = osvi.dwPlatformId;

   Reboot = false;
   ShutDown = false;
}

//------------------------------------------------------------------------------

void __fastcall TDeviceRealPalms::BuildInfoSyncPacket(TMemoryStream *Process_Outbound)
{
	if (!frmData->IBTransaction->InTransaction) { frmData->IBTransaction->StartTransaction(); }
   frmData->IBTDBTables::Open();

   AnsiString header = "[Info]";
   AnsiString Tableheader = "[Table]";
   AnsiString Staffheader = "[Staff]";
   // AnsiString Version = "XX.XXX";
   AnsiString Version = "1.6";
   int NewLineChar = 10;

   //TComms::Instance().Process_Outbound->Memory
   Process_Outbound->Write(header.c_str(),header.Length());
   Process_Outbound->Write(&NewLineChar,1); // newline character.
   // Versioning Info.
   Process_Outbound->Write(Version.c_str(),Version.Length());
   Process_Outbound->Write(&NewLineChar,1); // newline character.
   // Add Features here.
   AnsiString Rounding = "1";
   Process_Outbound->Write(Rounding.c_str(),Rounding.Length());  // newline character.
   Process_Outbound->Write(&NewLineChar,1);  // newline character.
   Process_Outbound->Write(&NewLineChar,1);  // newline character.

   Process_Outbound->Write(Tableheader.c_str(),Tableheader.Length());  // Remove the NULL.
   Process_Outbound->Write(&NewLineChar,1);  // newline character.

	// Set the tables caption.
	frmData->IBSQL->Close();
	frmData->IBSQL->SQL->Text =
	" SELECT "
	" TABLE_KEY, TABLE_NUMBER, TABLE_NAME, PARTY_NAME "
	" FROM "
	"  TABLES "
	" WHERE "
	" TABLE_NAME IS NOT NULL AND TABLE_NAME != '' ";
	frmData->IBSQL->ExecQuery();
	for(;!frmData->IBSQL->Eof; frmData->IBSQL->Next())
	{
		if (frmData->IBSQL->FieldByName("TABLE_NAME")->AsString != "")
		{
			 Process_Outbound->Write(frmData->IBSQL->FieldByName("TABLE_NUMBER")->AsString.c_str(),
			 frmData->IBSQL->FieldByName("TABLE_NUMBER")->AsString.Length());
			 Process_Outbound->Write(&sep,sizeof(sep));
			 Process_Outbound->Write(frmData->IBSQL->FieldByName("TABLE_NAME")->AsString.c_str(),
			 frmData->IBSQL->FieldByName("TABLE_NAME")->AsString.Length());
			 Process_Outbound->Write(&NewLineChar,1); // newline character.
		}
	}
   Process_Outbound->Write(&NewLineChar,1); // newline character.
   Process_Outbound->Write(Staffheader.c_str(),Staffheader.Length()); // Remove the NULL.
   Process_Outbound->Write(&NewLineChar,1); // newline character.
   // ok write out the servers.
	TStringList *List = new TStringList;
   try
   {
      Staff->GetContactList(List);
      for (int i = 0; i < List->Count; i++)
      {
			int UserKey = int(List->Objects[i]);
         AnsiString Name = List->Strings[i];
         AnsiString Initails = Staff->GetContactInitials(UserKey);

         Process_Outbound->Write(IntToStr(UserKey).c_str(),IntToStr(UserKey).Length());
         Process_Outbound->Write(&sep,sizeof(sep));
         Process_Outbound->Write(Name.c_str(),Name.Length());
         Process_Outbound->Write(&sep,sizeof(sep));
         Process_Outbound->Write(Initails.c_str(),Initails.Length());
         Process_Outbound->Write(&NewLineChar,1); // newline character.
      }
   }
   __finally
   {
   	delete List;
   }
   Process_Outbound->Write(&NewLineChar,1); // newline character.
   Process_Outbound->SaveToFile("InfoSync.doc"); // newline character.
	if (frmData->IBTransaction->InTransaction) { frmData->IBTransaction->Commit(); }
}

//------------------------------------------------------------------------------

__fastcall TDeviceRealPalms::~TDeviceRealPalms()
{
   // Save of all Palms that arnt in list all ready.
   SavePalms();

   TPalm *TempPalm;
	while(PalmList->Count != 0)
   {
		TempPalm = (TPalm *)PalmList->Items[0];
		delete TempPalm;
      PalmList->Delete(0);
   }
	delete PalmList;
	delete PalmNotFound;
 	delete PalmRegister;
}

//------------------------------------------------------------------------------

// Removes a Palm form the Palm List and frees its memory.
// Will most likly be used when someone logs out.
void TDeviceRealPalms::FreePalm(AnsiString Name)
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
TPalm* TDeviceRealPalms::FindTPalm(unsigned char ID)
{
	if(ID == 0)
   {
		return PalmRegister;
   }	// No palms in list request an InfoSync on Palm 0 (almost like a channel zero.)
   else if(PalmList->Count == 0)
   {
      // Check in Database.
      TPalm *Palm = new TPalm();
		Palm->ID.DeviceKey = IDToKey(ID);
      Palm->ID.DeviceID = ID;
      Palm->ID.Name ="";
      Palm->TransNo = 0;
      if(Palm->ID.DeviceKey != 0)
      {
			Palm->Load();
         PalmList->Add(Palm);
         return Palm;
      }
      else
      {
         delete Palm;
		   return PalmNotFound;
      }
   }
   else //Search using Palm ID.
   {
      for(int i = 0 ; i < PalmList->Count ; i++)
      {
         TPalm *Ptr = (TPalm *)PalmList->Items[i];
         if(Ptr->ID.DeviceID == ID)
         {
            return Ptr;
         }
      }
      return PalmNotFound;
   }
}
//------------------------------------------------------------------------------





//------------------------------------------------------------------------------
// All the palms in the DB must be loaded into the palm list in order for this function to
// work properly, If they arnt then it is very possable to assign a palm an ID that
// exists in the DB and when create is called it will find the DB version of the palm
// and load it. This will cause the palm serial numbers to be incorrect.
int TDeviceRealPalms::RetrieveNewPalmID()
{
	int ID = 0;
   if(PalmList->Count == 0)
   {
		ID++;
   }
   else
   {
		ID = 1;
      for(int i = 0 ; i < PalmList->Count;)
      {
         TPalm *Ptr = (TPalm *)PalmList->Items[i];
         if(Ptr->ID.DeviceID == ID)
         {
            ID++;
            i = 0;
         }
         else
         {
            i++;
         }
      }
   }
   return ID;
}

void TDeviceRealPalms::UpdateCurrentRecipes(AnsiString Code,AnsiString Location,
	AnsiString Unit, AnsiString Desc, Currency Cost,double CostGSTPercent)
{
   for (int i = 0 ; i < Menus.Current->Count ; i++)
   {
      TListMenu *Menu = Menus.Current->MenuGet(i);

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
							RecipeItem->Unit			= Unit;
							RecipeItem->Description	= Desc;
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

__fastcall TPalm::TPalm()
{
   ID.DeviceID = 0;
   ID.Type = devPalm;
   LastConnectVar = 0;
   SeatNames = new TList;
   Inbound = new TIRPacket(this);
   Outbound = new TMemoryStream;
   TransNo = 0;
   User.ContactID = 0;
   User.ContactKey = 0;
   Status.Active = false;
   Status.Error = proA_Ok;
   DownloadOrders = ThisCurrentTable;
   LogoffTime = 5;
   CurrentTable = 0;
   AutoPopup = false;
   LargeFont = false;
   RemoveFromPalmList = false;
   CurrentTables.clear();
   resetMenuEnableState(); // Will Force an update.
}

//------------------------------------------------------------------------------

__fastcall TPalm::~TPalm()
{
   delete Inbound;
   delete Outbound;

   TPalmSeatName *SeatName;

	while(SeatNames->Count != 0)
	{
		SeatName = (TPalmSeatName *)SeatNames->Items[0];
		delete SeatName;
		SeatNames->Delete(0);
	}

   delete SeatNames;
}

void TPalm::DumpStatus(TStringList *Debug)
{
	Debug->Add("Memory Usage of Palm " + ID.Name);
	Debug->Add("Size of this Palm " + IntToStr(sizeof(TPalm)));
   Debug->Add("Count of Palm SeatNames Pending " + IntToStr(SeatNames->Count) );

   Debug->Add("Size of Palm Outbound Stream "+ IntToStr(Outbound->Size) );
   Debug->Add("Size of Palm Inbound Packet " + IntToStr(sizeof(TIRPacket)) );

	Debug->Add("Inbound Orders Tables Capacity " + IntToStr(Inbound->Orders.TDBTables::Capacity));
	Debug->Add("Inbound Orders Tables Size " + IntToStr(Inbound->Orders.TDBTables::Count));
	Debug->Add("Inbound Billing Tables Capacity " + IntToStr(Inbound->Billing.TDBTables::Capacity));
	Debug->Add("Inbound Billing Tables Size " + IntToStr(Inbound->Billing.TDBTables::Count));

}
//------------------------------------------------------------------------------

void TPalm::BuildPacket(TMemoryStream *Process_Outbound,MESSAGE_TYPE MsgType)
{
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

   AnsiString msgType = "";
   switch(Command)
   {
      case KEEPALIVE : msgType = "KEEPALIVE";
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
      case REQUEST_RECEIPT : msgType = "REQUEST_RECEIPT";
      break;
      case TRANSFER_TABLE_ORDERS : msgType = "TRANSFER_TABLE_ORDERS";
      break;
      case RETRIEVE_ORDERS : msgType = "RETRIEVE_ORDERS";
      break;
   }

   TManagerLogs::Instance().Add(__FUNC__,"Packet Data","Building Packet " + msgType,"Palm #" + IntToStr(this->ID.DeviceID));
   AnsiString ErrType = "Ok";
   if(ErrorNum != proA_Ok)
   {
      switch(ErrorNum)
      {
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
         case Err_DeviceID_Not_Found : ErrType = "Err_DeviceID_Not_Found";
         break;
         case Err_No_Printer_Found : ErrType = "Err_No_Printer_Found";
         break;
         case Err_Not_In_Register_Mode : ErrType = "Err_Not_In_Register_Mode";
         break;
         case Wrn_No_Menu_Found : ErrType = "Wrn_No_Menu_Found";
         break;
         case Wrn_Printing_Redirected : ErrType = "Wrn_Printing_Redirected";
         break;
         case Wrn_Duplicate_Order : ErrType = "Wrn_Duplicate_Order";
         break;
         default : ErrType = "Unknown Error : " + IntToStr(ErrorNum);
      }
      TManagerLogs::Instance().Add(__FUNC__,"PALM ERROR","Reporting Palm Status " + ErrType,"Palm #" + IntToStr(this->ID.DeviceID));
      TManagerLogs::Instance().Add(__FUNC__,"PALM ERROR","Error Message " + this->Status.ErrorMsg,"Palm #" + IntToStr(this->ID.DeviceID));
   }
   if(Process_Outbound)
   {
      hPktSize = (unsigned char) (Process_Outbound->Size >> 8);
      lPktSize = (unsigned char) (Process_Outbound->Size % 256);

   // Clear the Working Outbound.
      Outbound->Clear();

      // Copy in Command Word.
      Outbound->Write(&Command, 1);
      // Copy in Transaction Number.
      unsigned char TransNumber = TransNo - 1;
      Outbound->Write(&TransNumber, 1);
      TManagerLogs::Instance().Add(__FUNC__,"Packet Data","PalmMates Current TransNumber : " + IntToStr(TransNumber));
      // InfoSync Number.
      Outbound->Write(&TGlobalSettings::Instance().InfoSync, 1);
      TManagerLogs::Instance().Add(__FUNC__,"Packet Data","PalmMates Current InfoSync : " + IntToStr(TGlobalSettings::Instance().InfoSync));

      // Menu Sync Number.
      TDeviceRealTerminal::Instance().Menus.SelectMenuOfType(eFoodMenu);
      if(TDeviceRealTerminal::Instance().Menus.MenuFood != NULL )
      {
         unsigned char FoodMenuSync = TDeviceRealTerminal::Instance().Menus.MenuFood->MenuKey;
         Outbound->Write(&FoodMenuSync, 1);
         TManagerLogs::Instance().Add(__FUNC__,"Packet Data","PalmMates Current MenuSync : " + IntToStr(FoodMenuSync));
      }
      else
      {
         unsigned char FoodMenuSync = 0;
         Outbound->Write(&FoodMenuSync, 1);
         TManagerLogs::Instance().Add(__FUNC__,"Packet Data","PalmMates Current MenuSync : " + IntToStr(FoodMenuSync));
      }
      // Drink Sync Number.
      TDeviceRealTerminal::Instance().Menus.SelectMenuOfType(eDrinksMenu);
      if(TDeviceRealTerminal::Instance().Menus.MenuDrinks != NULL )
      {
         unsigned char DrinkMenuSync = TDeviceRealTerminal::Instance().Menus.MenuDrinks->MenuKey;
         Outbound->Write(&DrinkMenuSync, 1);
         TManagerLogs::Instance().Add(__FUNC__,"Packet Data","PalmMates Current DrinkSync : " + IntToStr(DrinkMenuSync));
      }
      else
      {
         unsigned char DrinkMenuSync = 0;
         Outbound->Write(&DrinkMenuSync, 1);
         TManagerLogs::Instance().Add(__FUNC__,"Packet Data","PalmMates Current DrinkSync : " + IntToStr(DrinkMenuSync));
      }
      // Staff ID Number.
      Outbound->Write(&User.ContactKey, 1);
      TManagerLogs::Instance().Add(__FUNC__,"Packet Data","PalmMates Current User Key : " + IntToStr(User.ContactKey),"Palm #" + IntToStr(this->ID.DeviceID));
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

      hCRC = CRC >> 8;
      lCRC = CRC % 256;
      // Add Internal CRC.
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
      TManagerLogs::Instance().Add(__FUNC__,"Packet Data","PalmMates Current TransNumber : " + IntToStr(TransNumber));
      // InfoSync Number.
      Outbound->Write(&TGlobalSettings::Instance().InfoSync, 1);
      TManagerLogs::Instance().Add(__FUNC__,"Packet Data","PalmMates Current InfoSync : " + IntToStr(TGlobalSettings::Instance().InfoSync));
      // Menu Sync Number.
      TDeviceRealTerminal::Instance().Menus.SelectMenuOfType(eFoodMenu);
      if(TDeviceRealTerminal::Instance().Menus.MenuFood != NULL )
      {
         unsigned char FoodMenuSync = TDeviceRealTerminal::Instance().Menus.MenuFood->MenuKey;
         Outbound->Write(&FoodMenuSync, 1);
         TManagerLogs::Instance().Add(__FUNC__,"Packet Data","PalmMates Current MenuSync : " + IntToStr(FoodMenuSync));
      }
      else
      {
         unsigned char FoodMenuSync = 0;
         Outbound->Write(&FoodMenuSync, 1);
         TManagerLogs::Instance().Add(__FUNC__,"Packet Data","PalmMates Current MenuSync : " + IntToStr(FoodMenuSync));
      }
      // Drink Sync Number.
      TDeviceRealTerminal::Instance().Menus.SelectMenuOfType(eDrinksMenu);
      if(TDeviceRealTerminal::Instance().Menus.MenuDrinks != NULL )
      {
         unsigned char DrinkMenuSync = TDeviceRealTerminal::Instance().Menus.MenuDrinks->MenuKey;
         Outbound->Write(&DrinkMenuSync, 1);
         TManagerLogs::Instance().Add(__FUNC__,"Packet Data","PalmMates Current DrinkSync : " + IntToStr(DrinkMenuSync));
      }
      else
      {
         unsigned char DrinkMenuSync = 0;
         Outbound->Write(&DrinkMenuSync, 1);
         TManagerLogs::Instance().Add(__FUNC__,"Packet Data","PalmMates Current DrinkSync : " + IntToStr(DrinkMenuSync));
      }
      // Staff ID Number.
      Outbound->Write(&User.ContactKey, 1);
      TManagerLogs::Instance().Add(__FUNC__,"Packet Data","PalmMates Current User Key : " + IntToStr(User.ContactKey),"Palm #" + IntToStr(this->ID.DeviceID));
      // Error Number.
      Outbound->Write(&ErrorNum, 1);
      TManagerLogs::Instance().Add(__FUNC__,"Packet Data","Error Code : " + IntToStr(ErrorNum),"Palm #" + IntToStr(this->ID.DeviceID));
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

      hCRC = CRC >> 8;
      lCRC = CRC % 256;
      // Add Internal CRC.
      Outbound->Write(&hCRC, 1);
      Outbound->Write(&lCRC, 1);
   }
   Status.Active = true;
   Status.LastActive = GetTickCount();
}

//------------------------------------------------------------------------------

void TPalm::ClearInbound()
{
   Inbound->Clear();
}

//------------------------------------------------------------------------------

void TPalm::UpdateTransactionNumber()
{
	// Find the Palm in the Database.
	TIBSQL *IBSQL				= new TIBSQL(frmData);;
	try
	{
		IBSQL->Database		= frmData->IBDatabase;
		IBSQL->Transaction	= frmData->IBTransaction;

		if (!frmData->IBTransaction->InTransaction) frmData->IBTransaction->StartTransaction();

		if (ID.Name != "")
		{
			IBSQL->SQL->Text =
				"Update DEVICES Set TRANSNO = :TRANSNO Where DEVICE_NAME = :DEVICE_NAME";
			IBSQL->ParamByName("DEVICE_NAME")->AsString = ID.Name;
			IBSQL->ParamByName("TRANSNO")->AsString = Inbound->Header.TransNo;
			IBSQL->ExecQuery();
			if (IBSQL->RowsAffected == 0)
			{
				Status.Error = Err_DeviceID_Not_Found;
			}
		}
		else if (ID.DeviceID != 0)
		{
			IBSQL->SQL->Text =
				"Update DEVICES Set TRANSNO = :TRANSNO Where DEVICE_ID = :DEVICE_ID";
			IBSQL->ParamByName("DEVICE_ID")->AsInteger = ID.DeviceID;
			IBSQL->ParamByName("TRANSNO")->AsString = Inbound->Header.TransNo;
			IBSQL->ExecQuery();
			if (IBSQL->RowsAffected == 0)
			{
				Status.Error = Err_DeviceID_Not_Found;
			}
		}
		else
		{
			Status.Error = Err_DeviceID_Not_Found;
		}
		if (frmData->IBTransaction->InTransaction) frmData->IBTransaction->Commit();
	}
	__finally
	{
		delete IBSQL;
	}

/*   frmData->IBDevices->Open();
	if (this->ID.Name != "")
   {
		Variant locvalues = this->ID.Name;
		if (frmData->IBDevices->Locate("DEVICE_NAME", locvalues, TLocateOptions()))
      {
         return true;
      }
   }

	if (this->ID.DeviceID != 0)
   {
      Variant locvalues = this->ID.DeviceID;
		if (frmData->IBDevices->Locate("DEVICE_ID", locvalues, TLocateOptions()))
      {
         return true;
      }
   }

	this->Status.Error = Err_DeviceID_Not_Found;
	return false;


	if(this->Locate())
   {
      frmData->IBDevices->Edit();
		frmData->IBDevices->FieldByName("TRANSNO")->AsInteger = this->Inbound->Header.TransNo;
      frmData->IBDevices->Post();
	}
	if (frmData->IBTransaction->InTransaction) frmData->IBTransaction->Commit(); */
}

//------------------------------------------------------------------------------

TPalmSeatName* TPalm::GetSeatName(int inTableNumber, int inSeatNumber)
{
	TPalmSeatName *SeatName;
	for(int i = 0 ; i < SeatNames->Count; i++)
	{
		SeatName = (TPalmSeatName *)SeatNames->Items[i];
		if((SeatName->TableNumber == inTableNumber) && (SeatName->SeatNumber == inSeatNumber))
		{
			return SeatName;
		}
	}
	return NULL;
}


void TPalm::RemoveSeatName(TPalmSeatName *ptrName)
{
	 ptrName->DeleteMe = true;
}


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
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

__fastcall TPalmSeatName::TPalmSeatName(int inTableNumber, int inSeatNumber,AnsiString inSeatName)
{
   TableNumber = inTableNumber;
   SeatNumber = inSeatNumber;
   SeatName = inSeatName;
   DeleteMe = false;
}


__fastcall TPalmSeatName::~TPalmSeatName()
{
}

void TPalm::UpdateNames()
{
	while(SeatNames->Count != 0)
	{
		delete (TPalmSeatName *)SeatNames->Items[0];
		SeatNames->Delete(0);
	}
	// Find all table Seat Tabs with Names and create palm Seat Names for them.
   bool StartedTrans = false;
	if (!frmData->IBTransaction->InTransaction) { frmData->IBTransaction->StartTransaction(); StartedTrans = true;}
   try
   {
      frmData->IBSQL->Close();
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
      TManagerLogs::Instance().Add(__FUNC__,"Exception", "TPalm::UpdateNames " + E.Message);
      if (frmData->IBTransaction->InTransaction  && StartedTrans) { frmData->IBTransaction->Rollback(); }
   }
}


bool TPalm::getMenuEnabledStateChange()
{
   if(TDeviceRealTerminal::Instance().Menus.MenuFood == NULL)
   {
      return false;
   }
   if(TDeviceRealTerminal::Instance().Menus.MenuFood->EnabledStateSync == fNonFatialMenuEnableDisableNumber)
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
   if(TDeviceRealTerminal::Instance().Menus.MenuDrinks == NULL)
   {
      return false;
   }

   if(TDeviceRealTerminal::Instance().Menus.MenuDrinks->EnabledStateSync == fNonFatialDrinksEnableDisableNumber)
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
   if(TDeviceRealTerminal::Instance().Menus.MenuFood != NULL)
   {
   	fNonFatialMenuEnableDisableNumber = TDeviceRealTerminal::Instance().Menus.MenuFood->EnabledStateSync;
	}
   else
   {
		fNonFatialMenuEnableDisableNumber = -1;
   }

   if(TDeviceRealTerminal::Instance().Menus.MenuDrinks != NULL)
   {
   	fNonFatialDrinksEnableDisableNumber = TDeviceRealTerminal::Instance().Menus.MenuDrinks->EnabledStateSync;
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

void TDeviceRealPalms::SavePalms()
{
   // Save of all Palms that arnt in list all ready.
	for(int i = 0 ; i < PalmList->Count ; i++)
   {
		TPalm *Ptr = (TPalm *)PalmList->Items[i];
      Ptr->Create();
   }
}

void TDeviceRealPalms::LoadPalms()
{
   bool StartTrans = false;
   try
   {
      if (!IBTransaction->InTransaction) {IBTransaction->StartTransaction(); StartTrans = true; }

      IBQuery->Close();
      IBQuery->SQL->Text = "SELECT DEVICE_KEY FROM DEVICES WHERE DEVICES.DEVICE_TYPE = " + IntToStr(devPalm)+ " ORDER BY DEVICE_NAME";
      IBQuery->Open();
      for(IBQuery->First();!IBQuery->Eof;IBQuery->Next())// Removes all palms from database.
      {
         TPalm *Palm = new TPalm();
         Palm->ID.DeviceKey = IBQuery->FieldByName("DEVICE_KEY")->AsInteger;
         Palm->Load();
         PalmList->Add(Palm);
      }
      
      if (IBTransaction->InTransaction && StartTrans) { IBTransaction->Commit(); }
   }
   catch(Exception &E)
   {
      if (IBTransaction->InTransaction && StartTrans) { IBTransaction->Rollback(); }
   }

}


//---------------------------------------------------------------------------

void TDeviceRealPalms::SendMessage(AnsiString From, AnsiString Msg)
{
    // Send the message.
   for(int i=0; i<PalmList->Count; i++)
	{
      TPalm *Palm = (TPalm *)PalmList->Items[i];
      if(Palm != NULL)
      {
         Palm->From = From;
         Palm->Message = Msg;
      }
   }
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Creates and new tables that are not found in the database.
// the pointer to the packet is held in the LParam in the TMessage.
void TPalm::CallAways()
{
	TIRPacket *Packet = Inbound;
   try
   {
      // Order Processing Only process non splits..
      Packet->CallAways.TDBTables::Pack();

      while( Packet->CallAways.TDBTables::Count !=0)
      {
         TIRTable *TempTable = (TIRTable *)Packet->CallAways.TDBTables::Items[0];
         TCallAwayComplete *CallAway = BuildCallAwayProper(TempTable);

         TReqPrintJob *NormalRequest = new TReqPrintJob(this);
         NormalRequest->SenderType = devPalm;
         NormalRequest->TimeStamp = Now();
         NormalRequest->Waiter = this->User.Name;

         Kitchen->GetPrintouts(CallAway,NormalRequest);
         Kitchen->Print(NormalRequest);
         delete NormalRequest;
         NormalRequest = NULL;

         delete (TIRTable *)TempTable;
         Packet->CallAways.TDBTables::Delete(0);
      }
   }
   __finally
   {
      while( Packet->CallAways.TDBTables::Count !=0)
      {
         delete (TIRTable *)Packet->CallAways.TDBTables::Items[0];
         Packet->CallAways.TDBTables::Delete(0);
      }
   }
}

void __fastcall TPalm::NewOrders()
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
         Packet->Orders.TDBTables::Pack();
         while( Packet->Orders.TDBTables::Count !=0)
         {
            // Check for Tables.
				TIRTable *TempTable = (TIRTable *)Packet->Orders.TDBTables::Items[0];
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
							Order->OrderedLocation = TDeviceRealTerminal::Instance().ID.Location;

                     Order->Security->SetSecurityRefNumber(TDeviceRealTerminal::Instance().GetNextSecurityRef());

                     TSecurityReference   *SecRef = new TSecurityReference;
                     SecRef->UserKey      = this->User.ContactKey;
                     SecRef->Event        = SecurityTypes[secOrderedBy];
                     SecRef->From         = this->User.Name;
                     SecRef->To           = this->User.Initials;
                     SecRef->TimeStamp    = Order->TimeStamp;
                     Order->Security->SecurityAdd(SecRef);

                     if(Order->PriceAdjust != 0)
                     {
                        TSecurityReference   *SecRef = new TSecurityReference;
                        SecRef->UserKey      = this->User.ContactKey;
                        SecRef->Event        = SecurityTypes[secPriceAdjust];
                        SecRef->From         = CurrToStrF(Order->Price, ffCurrency, 2);
                        SecRef->To           = CurrToStrF(Order->Price + Order->PriceAdjust, ffCurrency, 2);
                        SecRef->TimeStamp    = Order->TimeStamp;
                        Order->Security->SecurityAdd(SecRef);
                     }

							// Sort out Set Menu Grouping Numbers.
							if((TempOrder->SetMenuGroup != 0) && TempOrder->SetMenuGroupIsIndex)
							{ //
                        int SeatKey  = TDBTables::GetOrCreateSeat(TempTable->Table,TempSeat->Seat);
                        Order->TabKey = TDBTab::GetOrCreateTab(TDBTables::GetTabKey(SeatKey));
                        TDBTables::SetSeatTab(SeatKey,Order->TabKey);
                        TDBTab::SetTabType(Order->TabKey, TabTableSeat);
                        TDBTab::SetTabName(Order->TabKey,TempSeat->Name);
                        Order->PartyName = TDBTables::GetPartyName(Order->TableNo);

								TTabsSetMenuList::iterator CurrentTabsSetMenus = TDeviceRealTerminal::Instance().TabsSetMenuList.find(Order->TabKey);
								if(CurrentTabsSetMenus != TDeviceRealTerminal::Instance().TabsSetMenuList.end())
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
                        int SeatKey  = TDBTables::GetOrCreateSeat(TempTable->Table,TempSeat->Seat);
                        Order->TabKey = TDBTab::GetOrCreateTab(TDBTables::GetTabKey(SeatKey));
                        TDBTables::SetSeatTab(SeatKey,Order->TabKey);
                        TDBTab::SetTabType(Order->TabKey, TabTableSeat);
                        TDBTab::SetTabName(Order->TabKey,TempSeat->Name);
                        Order->PartyName = TDBTables::GetPartyName(Order->TableNo);

								TTabsSetMenuList::iterator CurrentTabsSetMenus = TDeviceRealTerminal::Instance().TabsSetMenuList.find(Order->TabKey);
								if(CurrentTabsSetMenus != TDeviceRealTerminal::Instance().TabsSetMenuList.end())
								{  // Tab already exists and is in List.
									// Have I may already Generated this translation for an eairlier order.
									TSetMenuList::iterator SetMenuNumber = CurrentTabsSetMenus->second.begin();

									// Does the Used GroupNo Vector already contain this Group Number?
									TSetMenuGroupNoLookup::iterator LookupGroupNumber = TempConsumedPalmGroupIDs.find(TempOrder->SetMenuGroup);
									if(LookupGroupNumber != TempConsumedPalmGroupIDs.end())
									{
										Order->SetMenuGroup = LookupGroupNumber->second;
									}
									else
									{// Get the next Set Menu key from the Generator and use that.
										int NewSetMenuGroupingNumber = OrderManager->GetSetMenuGroupNumber();

                              // Add the New Key to it.
                              CurrentTabsSetMenus->second.push_back(NewSetMenuGroupingNumber);
                              TempConsumedPalmGroupIDs[TempOrder->SetMenuGroup] = NewSetMenuGroupingNumber;
                              Order->SetMenuGroup = NewSetMenuGroupingNumber;
                           }
                        }
                        else
								{  // New table / seat -> tab with setmenu stuff.
                           int NewSetMenuGroupingNumber = OrderManager->GetSetMenuGroupNumber();

                           // Add Tab to List.
                           TempConsumedPalmGroupIDs[TempOrder->SetMenuGroup] = NewSetMenuGroupingNumber;
                           TDeviceRealTerminal::Instance().TabsSetMenuList[Order->TabKey].push_back(NewSetMenuGroupingNumber);
                           Order->SetMenuGroup = NewSetMenuGroupingNumber;
                        }
                     }

                     if(Order->SetMenu && !Order->SetMenuMaster && Order->SetMenuGroup != 0)
                     {
                        Order->Price = 0;
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
                     CurrentTables.insert(Order->TableNo);
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
                  TPalm *TempPalm;
                  for(int i = 0; i < TDeviceRealTerminal::Instance().PalmList->Count; i++)
                  {
                     TempPalm = (TPalm *)TDeviceRealTerminal::Instance().PalmList->Items[i];
                     if(TempPalm->ID.DeviceID != this->ID.DeviceID)
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
               }
               delete (TIRSeat *)TempSeat;
               TempTable->Seats->Delete(0);
            }
            delete (TIRTable *)TempTable;
            Packet->Orders.TDBTables::Delete(0);
         }

			if(this->Status.Error == proA_Ok)
         {
				// ------------- Process orders ----------------
	         frmData->ProcessOrders(OrdersList);
            ManagerStock->UpdateStock(OrdersList);
         }

			if(this->Status.Error == proA_Ok)
         {
            // Print the Orders.
            TReqPrintJob *NormalRequest = new TReqPrintJob(this);
            NormalRequest->SenderType = devPalm;
            NormalRequest->TimeStamp = Now();
            TReqPrintJob *CreditRequest = new TReqPrintJob(this);
            CreditRequest->SenderType = devPalm;
            CreditRequest->TimeStamp = Now();
            for(int y = 0; y < OrdersList->Count; y++)
            {
               TItemComplete *Order = (TItemComplete *)OrdersList->Items[y];
               if(Order->OrderType != CanceledOrder && Order->DuplicateDetected == false)   // Credits are printed before they are removed.
               {
                  NormalRequest->Waiter = this->User.Name;
                  NormalRequest->Orders->Add(Order);
                  NormalRequest->OrderLocalKeys.insert(Order->Order_Key);
                  if(Order->TimeStamp < NormalRequest->TimeStamp)
                  {
                     NormalRequest->TimeStamp = Order->TimeStamp;
                  }
               }
               else if (Order->DuplicateDetected == false)
               {
                  CreditRequest->Waiter = this->User.Name;
                  CreditRequest->Orders->Add(Order);
                  CreditRequest->OrderLocalKeys.insert(Order->Order_Key);
                  if(Order->TimeStamp < CreditRequest->TimeStamp)
                  {
                     CreditRequest->TimeStamp = Order->TimeStamp;
                  }
               }
            }

            if(NormalRequest->Orders->Count != 0)
				{
					Kitchen->GetPrintouts(NormalRequest);
					Kitchen->Print(NormalRequest);
//               frmData->RequestKitchenOrder(NormalRequest);
//               TComms::Instance().Print(NormalRequest);
               if(NormalRequest->Header.Error != proA_Ok)
               {
						this->Status.Error = NormalRequest->Header.Error;
               }
            }

            if(CreditRequest->Orders->Count != 0)
            {
					Kitchen->GetPrintouts(CreditRequest);
					Kitchen->Print(CreditRequest);
//					frmData->RequestKitchenOrder(CreditRequest);
//               TComms::Instance().Print(CreditRequest);
               if(CreditRequest->Header.Error != proA_Ok)
               {
                  this->Status.Error = CreditRequest->Header.Error;
               }
            }

            delete NormalRequest;
            delete CreditRequest;
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
		TManagerLogs::Instance().Add(__FUNC__,"Exception","SyncThread::NewOrder " + Err.Message);
		throw;
	}
}
/*
void __fastcall TPalm::NewOrders()
{
	TIRPacket *Packet = Inbound;
	// Look for any printer before processing the Job.
	// You have to do this here as a quik check so the
	// Job is not added to the Database because. Even if TKitchen::Prepare
	// detects that there is no printers and throws the error that warns the
	// user, the user can just beem the palm again which causes a Get_Orders
	// which will pull the order from the database and put a tick next to it.
	// By checking here we stop the order being added to the DB.
	TList *OrdersList;
	TSetMenuList TempConsumedPalmGroupIDs;
//	typedef std::map<long,long> TSetMenuGroupNoLookup; // Stephen "Random" Number / My SetMenuGroup Key.
//   TSetMenuGroupNoLookup TempConsumedPalmGroupIDs;
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
			Packet->Orders.TDBTables::Pack();
			while( Packet->Orders.TDBTables::Count !=0)
			{
            // Check for Tables.
            TIRTable *TempTable = (TIRTable *)Packet->Orders.TDBTables::Items[0];
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
                  TItemComplete *Order =  this->BuildOrderProper(TempOrder);
                  if(Order != NULL)
                  {
                     Order->TableNo   = (WORD)TempTable->Table;
                     Order->SeatNo         = TempSeat->Seat;
                     Order->ServerName     = this->User.UserName;
                     Order->ServerInitials = this->User.UserInitials;
                     Order->ViewableLocations = "Palm";
                     // Sort out Set Menu Grouping Numbers.
                     if((TempOrder->SetMenuGroup != 0) && TempOrder->SetMenuGroupIsIndex)
                     { //
                        // Locate the Tab for this Table Seat (Create one if need be)
                        SZone Zone;
                        Zone.TableNo = TempTable->Table;
                        Zone.SeatNo = TempSeat->Seat;
                        frmData->dbTableSeatFindOrCreate(&Zone);
                        Zone.TabName = TempSeat->Name;
                        Zone.Tab_Key = 0;
                        Zone.TableName = "";
                        Zone.PartyName = "";
                        frmData->dbTabFindOrCreate(&Zone);
                        TTabsSetMenuList::iterator CurrentTabsSetMenus = TDeviceRealTerminal::Instance().TabsSetMenuList.find(Zone.Tab_Key);
                        if(CurrentTabsSetMenus != TDeviceRealTerminal::Instance().TabsSetMenuList.end())
                        {
                        // Lookup the SetMenu Group No based on this Index.
                           TSetMenuList::iterator SetMenuNo = CurrentTabsSetMenus->second.begin();

                           std::advance(SetMenuNo,TempOrder->SetMenuGroup);
                           Order->SetMenuGroup = *SetMenuNo;
                        }  // Tab should always be found if its a new Tab or Set Menu then
                           // it would not be indexed based. (The rebuild function will pick
                           // up any tabs that are not in TabsSetMenuList and add them whe
                           // the orders are sent )
                     }
                     else if (TempOrder->SetMenuGroup != 0)
                     {  // Number was generated from the palm.
                        SZone Zone;
                        Zone.TableNo = TempTable->Table;
                        Zone.SeatNo = TempSeat->Seat;
                        frmData->dbTableSeatFindOrCreate(&Zone);
                        Zone.TabName = TempSeat->Name;
                        Zone.Tab_Key = 0;
                        Zone.TableName = "";
                        Zone.PartyName = "";
                        frmData->dbTabFindOrCreate(&Zone);

                        TTabsSetMenuList::iterator CurrentTabsSetMenus = TDeviceRealTerminal::Instance().TabsSetMenuList.find(Zone.Tab_Key);
                        if(CurrentTabsSetMenus != TDeviceRealTerminal::Instance().TabsSetMenuList.end())
                        {  // Tab already exists and is in List.
                           // Have I may already Generated this translation for an eairlier order.
                           TSetMenuList::iterator SetMenuNumber = CurrentTabsSetMenus->second.begin();

                           // Does the Used GroupNo Vector already contain this Group Number?
// TSetMenuGroupNoLookup::iterator LookupGroupNumber = TempConsumedPalmGroupIDs.find(TempOrder->SetMenuGroup);
                           if(std::count( TempConsumedPalmGroupIDs.begin(), TempConsumedPalmGroupIDs.end(),TempOrder->SetMenuGroup) != 0)
                           {
	                           std::advance(SetMenuNumber,(TempOrder->SetMenuGroup-1)); // "0 Based Vector"
                              Order->SetMenuGroup = *SetMenuNumber;
                           }
                           else
                           {// Get the next Set Menu key from the Generator and use that.
                              int NewSetMenuGroupingNumber = 0;
                              frmData->IBSQL->Close();
                              frmData->IBSQL->SQL->Text = "SELECT GEN_ID(GEN_SETMENUGROUP, 1) FROM RDB$DATABASE";
                              frmData->IBSQL->ExecQuery();
                              NewSetMenuGroupingNumber = frmData->IBSQL->Fields[0]->AsInteger;

                              // Add the New Key to it.
                              CurrentTabsSetMenus->second.push_back(NewSetMenuGroupingNumber);
                              TempConsumedPalmGroupIDs.push_back(TempOrder->SetMenuGroup);
                              Order->SetMenuGroup = NewSetMenuGroupingNumber;
                           }
                        }
                        else
                        {  // New table / seat -> tab with setmenu stuff.
                           int NewSetMenuGroupingNumber = 0;
                           frmData->IBSQL->Close();
                           frmData->IBSQL->SQL->Text = "SELECT GEN_ID(GEN_SETMENUGROUP, 1) FROM RDB$DATABASE";
                           frmData->IBSQL->ExecQuery();
                           NewSetMenuGroupingNumber = frmData->IBSQL->Fields[0]->AsInteger;

                           // Add Tab to List.
                           TSetMenuList NewSetMenuList;
                           NewSetMenuList.push_back(NewSetMenuGroupingNumber);
                           TempConsumedPalmGroupIDs.push_back(TempOrder->SetMenuGroup);
                           TDeviceRealTerminal::Instance().TabsSetMenuList[Zone.Tab_Key] = NewSetMenuList;
                           Order->SetMenuGroup = NewSetMenuGroupingNumber;
                        }
                     }

                     if(Order->SetMenu && !Order->SetMenuMaster && Order->SetMenuGroup != 0)
                     {
                        Order->Price = 0;
                     }

                     Order->TimeStamp = Now() - (double(TempSeat->PalmTime)/86400);
                     if(Order->Palm_ID == LastItemID)
                     {
                        DishCount++;
                     }
                     else
                     {
                        DishCount = 0;
                     }
                     Order->TransNo = ((Packet->Header.TransNo *100) + DishCount);
                     LastItemID = Order->Palm_ID;

                     // --------- Store Order ----------
                     OrdersList->Add(Order);
                     CurrentTables.insert(Order->TableNo);
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
                  delete (TIROrder *)TempOrder;
                  TempSeat->Orders->Delete(0);
               }

               //-------------- Send Seat Names to other Palms--------------
               if(TempSeat->NameChange)
               {
                  TPalm *TempPalm;
                  for(int i = 0; i < TDeviceRealTerminal::Instance().PalmList->Count; i++)
                  {
                     TempPalm = (TPalm *)TDeviceRealTerminal::Instance().PalmList->Items[i];
                     if(TempPalm->ID.DeviceID != this->ID.DeviceID)
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
                               new TPalmSeatName(TempPalm->SeatNames,TempTable->Table,
                                                 TempSeat->Seat,TempSeat->Name);
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
                        SZone Zone;
                        Zone.TableNo = TempTable->Table;
                        Zone.SeatNo = TempSeat->Seat;
                        frmData->dbTableSeatFindOrCreate(&Zone);
                        Zone.TabName = TempSeat->Name;
                        Zone.Tab_Key = 0;
                        Zone.TableName = "";
                        Zone.PartyName = "";
                        if(frmData->dbTabFindOrCreate(&Zone))
                        {
                          frmData->SetTabName(TempSeat->Name,Zone.Tab_Key);
                        }
                        else // return 0 as Tab LK.
                        {
                          TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Palm NewOrders dbTabFindOrCreate Seat Name Failed");
                        }
                     }
                  }
               }
               delete (TIRSeat *)TempSeat;
               TempTable->Seats->Delete(0);
            }
            delete (TIRTable *)TempTable;
            Packet->Orders.TDBTables::Delete(0);
         }

			if(this->Status.Error == proA_Ok)
         {
				// ------------- Process orders ----------------
	         frmData->ProcessOrders(OrdersList);
         }

			if(this->Status.Error == proA_Ok)
         {
            // Print the Orders.
            TReqPrintJob *NormalRequest = new TReqPrintJob(this);
            NormalRequest->SenderType = devPalm;
            NormalRequest->TimeStamp = Now();
            TReqPrintJob *CreditRequest = new TReqPrintJob(this);
            CreditRequest->SenderType = devPalm;
            CreditRequest->TimeStamp = Now();
            for(int y = 0; y < OrdersList->Count; y++)
            {
               TItemComplete *Order = (TItemComplete *)OrdersList->Items[y];
               if(Order->OrderType != CreditOrder)   // Credits are printed before they are removed.
               {
                  NormalRequest->Waiter = Order->ServerName;
                  NormalRequest->Orders->Add(Order);
                  NormalRequest->OrderLocalKeys.insert(Order->Order_Key);
                  if(Order->TimeStamp < NormalRequest->TimeStamp)
                  {
                     NormalRequest->TimeStamp = Order->TimeStamp;
                  }
               }
               else
               {
                  CreditRequest->Waiter = Order->ServerName;
                  CreditRequest->Orders->Add(Order);
                  CreditRequest->OrderLocalKeys.insert(Order->Order_Key);
                  if(Order->TimeStamp < CreditRequest->TimeStamp)
                  {
                     CreditRequest->TimeStamp = Order->TimeStamp;
                  }
               }
            }

            if(NormalRequest->Orders->Count != 0)
            {
					frmData->RequestKitchenOrder(NormalRequest);
               TComms::Instance().Print(NormalRequest);
               if(NormalRequest->Header.Error != proA_Ok)
               {
                  this->Status.Error = NormalRequest->Header.Error;
               }
            }

            if(CreditRequest->Orders->Count != 0)
            {
					frmData->RequestKitchenOrder(CreditRequest);
               TComms::Instance().Print(CreditRequest);
               if(CreditRequest->Header.Error != proA_Ok)
               {
                  this->Status.Error = CreditRequest->Header.Error;
               }
            }

            delete NormalRequest;
            delete CreditRequest;
         }

         //------------- free order memory ---------------
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
      delete OrdersList;
      TManagerLogs::Instance().Add(__FUNC__,"Exception","SyncThread::NewOrder " + Err.Message);
      throw;
   }
}
*/

//DM_NEWORDER-------------------------------------------------------------------
void __fastcall TPalm::Login()
{
   // Retrieve Staff ID form Packet.
   // Search DB.
   int NewLineChar = 10;
   TManagerLogs::Instance().Add(__FUNC__,"Login","Verify sequence");
   TManagerLogs::Instance().Add(__FUNC__,"Login","Palm ID " + IntToStr(ID.DeviceID));
   TManagerLogs::Instance().Add(__FUNC__,"Login","Palm Name " + Inbound->Login.DeviceName);

   //update the palms Name from the one recived.
   if(this->ID.Name != Inbound->Login.DeviceName)
   {
      Status.Error = Err_DeviceID_Not_Found;
   }
   else if(this->Locate())
   {
      // Name was locateded ID matched or updated proceed.
      TManagerLogs::Instance().Add(__FUNC__,"Login","Current Palm ID " + IntToStr(ID.DeviceID));
      TManagerLogs::Instance().Add(__FUNC__,"Login","Current Staff ID " + IntToStr(User.ContactKey));

      TMMContactInfo TempUserInfo;
      TempUserInfo.ContactKey = Inbound->Login.NewStaffID;
		TempUserInfo.PIN = Inbound->Login.UsersPIN;

		if(Inbound->Login.NewStaffID != 0)
      {
         TLoginSuccess Result = Staff->Login(TempUserInfo, CheckPOS);
         if (Result == lsAccepted)
         {
            TManagerLogs::Instance().Add(__FUNC__,"Login","Login Request Granted");

            this->User = TempUserInfo;
            this->DownloadOrders = (DownloadStatus)TempUserInfo.PalmDownloadType;
            this->AdvanceUser = TempUserInfo.PalmAdvancedUser;
            this->AutoPopup = TempUserInfo.PalmAutoOptions;
            this->LargeFont = TempUserInfo.PalmLargeFont;
            this->LogoffTime = TempUserInfo.PalmTimeOut;

            // Compile Response.
            // Add it to the Palms Transmission list.
            TMemoryStream *UserInfo = new TMemoryStream;
            try
            {
               UserInfo->Clear();
               // Insert User Information.
               AnsiString header = "[User]";
               UserInfo->Write(header.c_str(),header.Length());
               UserInfo->Write(&NewLineChar,1); // newline character.

               //TextVerion of Trans Number.
               UserInfo->Write(IntToStr(this->Status.TransactionNumber).c_str(),IntToStr(this->Status.TransactionNumber).Length());
               UserInfo->Write(&NewLineChar,1); // newline character.

               AnsiString Enabled = "";
               if(LogoffTime != 0)
               {
                  Enabled = IntToStr(LogoffTime);
                  UserInfo->Write(Enabled.c_str(),Enabled.Length()); // newline character.
                  UserInfo->Write(&NewLineChar,1); // newline character.
               }

               if(AdvanceUser)
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
               }
               UserInfo->Write(&NewLineChar,1); // newline character.
               this->BuildPacket(UserInfo);
            }
            __finally
            {
               delete UserInfo;
            }

            CurrentTables.clear();
            this->UpdateNames();
         }
         else if(Result == lsUserNotFound)
         {
            TManagerLogs::Instance().Add(__FUNC__,"Login",Now().FormatString("dd-mm-yy hh-mm") + " Login Failed Palm : " + IntToStr(ID.DeviceID)
                     + " Current User ID: " + IntToStr(User.ContactID)
                     + " Requested User ID: " + IntToStr(Inbound->Login.NewStaffID) + " User Not Found");

            Status.Error = Err_User_Not_Found;
            Status.ErrorMsg = "User not found";
         }
         else if(Result == lsPINIncorrect)
         {
            TManagerLogs::Instance().Add(__FUNC__,"Login",Now().FormatString("dd-mm-yy hh-mm") + " Login Failed Palm : " + IntToStr(ID.DeviceID)
                     + " Current User ID: " + IntToStr(User.ContactID)
                     + " Requested User ID: " + IntToStr(Inbound->Login.NewStaffID) + " PIN Incorrect");

            Status.Error = Err_PIN_Incorrect;
            Status.ErrorMsg = "Pin Incorrect";
         }
      }
      else
      {
         TManagerLogs::Instance().Add(__FUNC__,"Login",Now().FormatString("dd-mm-yy hh-mm") + " Login Failed Palm : " + IntToStr(ID.DeviceID)
                  + " Current User ID: " + IntToStr(User.ContactID)
                  + " Requested User ID: " + IntToStr(Inbound->Login.NewStaffID) + " User Not Found");

         Status.Error = Err_User_Not_Found;
         Status.ErrorMsg = "User not found";
      }
   }
}


//------------------------------------------------------------------------------


void TPalm::BillSeats()
{
   TIRTable *Table;
   TIRSeat *Seat;
   TIRPacket *Packet = this->Inbound;
	try
   {
		this->Inbound->Billing.TDBTables::Pack();
      for(int i = 0 ; (i < this->Inbound->Billing.TDBTables::Count) && (this->Status.Error == proA_Ok); i++)
      {
         // Retrieve the Table Number.
         Table = (TIRTable *)this->Inbound->Billing.TDBTables::Items[i];
         TManagerLogs::Instance().Add(__FUNC__,PALMLOG,"Billing Table #" + IntToStr(Table->Table));
         // Loop though the lists of seats pulling in the correct Tabs.
         Table->Seats->Pack();
         std::set<long> SelectedTabs;
         for(int j = 0 ;j < Table->Seats->Count; j++)
         {
            Seat = (TIRSeat *)Table->Seats->Items[j];
            TTableSeat TableSeat;
            TableSeat.TableNo = Table->Table;
            TableSeat.SeatNo = Seat->Seat;
            long TabKey = TDBTables::GetTabKey(TableSeat.TableNo,TableSeat.SeatNo);
            if(TabKey != 0)
            {
               SelectedTabs.insert(TabKey);
            }
         }

         TReqPrintJob *Request = new TReqPrintJob(this);
         try
         {
            OrderManager->GetOrders(Request->Orders,SelectedTabs);
            TDeviceRealTerminal::Instance().PaymentSystem->InitialMoney.Reset();
            TDeviceRealTerminal::Instance().PaymentSystem->PaymentsLoadFromOrders(Request->Orders, SelectedTabs);
            Request->Money = TDeviceRealTerminal::Instance().PaymentSystem->InitialMoney;

            Request->SenderType = devPalm;
            Request->PaymentType = ptPreliminary;//this->Inbound->FinalBill;
            Request->Waiter = this->User.Name;

            if (TComms::Instance().ReceiptPrinter == NULL)
            {
               this->Status.Error = Err_No_Printer_Found;
               break;
            }
            else
            {
               TDeviceRealTerminal::Instance().PaymentSystem->ReceiptPrint(Request, true);
            }

            TManagerLogs::Instance().Add(__FUNC__,PALMLOG,"PaymentType on Receipt = " + IntToStr(Request->PaymentType));

            if(Request->Header.Error != proA_Ok)
            {
               this->Status.Error = Request->Header.Error;
            }
         }
         __finally
         {
            while(Request->Orders->Count != 0)
            {
               delete Request->Orders->Items[0];
               Request->Orders->Delete(0);
            }
            delete Request;
         }
      }
	}// End Try.
   catch(Exception &Err)
   {
      TManagerLogs::Instance().Add(__FUNC__,"Exception", "SyncThread::BillSeats " + Err.Message);
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
	oldCRC = (unsigned short)UpdateCRC(oldCRC,Packet->Header.StaffIDNo);
	oldCRC = (unsigned short)UpdateCRC(oldCRC,Packet->Header.ErrorNo);
	oldCRC = (unsigned short)UpdateCRC(oldCRC,Packet->Header.HighPktSize);
	oldCRC = (unsigned short)UpdateCRC(oldCRC,Packet->Header.LowPktSize);
	for(int i=0;i<Packet->PktSize;i++)
	{
		oldCRC = (unsigned short)UpdateCRC(oldCRC,Packet->ptrBuffer[i]);
	}

	if(oldCRC != CRC)
	{
		TManagerLogs::Instance().Add(__FUNC__,"Inbound Check","Invalid CRC. Calculated : " + IntToStr(oldCRC) + " Sent : " + IntToStr(CRC));
		TManagerLogs::Instance().Add(__FUNC__,"Inbound Check","Invalid CRC. Calculated : " + IntToStr(oldCRC) + " Sent : " + IntToStr(CRC));
		this->Status.Error = Err_Msg_CRC;
	}
	else
	{
		TManagerLogs::Instance().Add(__FUNC__,"Inbound Check","Valid CRC.");
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------

bool __fastcall TPalm::CheckSyncNo()
{
   unsigned char FoodMenuSync, DrinkMenuSync;
   if(TDeviceRealTerminal::Instance().Menus.MenuFood == NULL && this->Inbound->Header.MenuSync1No != 0 )
   {
      FoodMenuSync = 0;
		this->Status.Error = Err_MenuOutOfSync;
      TManagerLogs::Instance().Add(__FUNC__,"Inbound Check","Menu Out Of Sync, Food Menu NULL While Palm Sync not 0");
   }
   else if(TDeviceRealTerminal::Instance().Menus.MenuFood == NULL && this->Inbound->Header.MenuSync1No == 0)
   {
      FoodMenuSync = 0;
	}
   else
   {
      FoodMenuSync = TDeviceRealTerminal::Instance().Menus.MenuFood->MenuKey;
   }

   if(TDeviceRealTerminal::Instance().Menus.MenuDrinks == NULL && this->Inbound->Header.MenuSync2No != 0)
   {
		DrinkMenuSync = 0;
		this->Status.Error = Err_DrinkOutOfSync;
      TManagerLogs::Instance().Add(__FUNC__,"Inbound Check","Menu Out Of Sync, Food Menu NULL While Palm Sync not 0");
   }
   else if(TDeviceRealTerminal::Instance().Menus.MenuDrinks == NULL && this->Inbound->Header.MenuSync2No == 0)
   {
      DrinkMenuSync = 0;
   }
   else
   {
      DrinkMenuSync = TDeviceRealTerminal::Instance().Menus.MenuDrinks->MenuKey;
   }

   if(this->Inbound->Header.MenuSync1No != FoodMenuSync && FoodMenuSync != 0)
   {
		TManagerLogs::Instance().Add(__FUNC__,PALMLOG,"Menu Sync # Failure. Palm :" + IntToStr(this->Inbound->Header.MenuSync1No) + " PC :" + IntToStr(FoodMenuSync));
		this->Status.Error = Err_MenuOutOfSync;
   }
   else if(this->Inbound->Header.MenuSync2No != DrinkMenuSync && DrinkMenuSync != 0)
   {
      TManagerLogs::Instance().Add(__FUNC__,PALMLOG,"Drink Sync # Failure. Palm :" + IntToStr(this->Inbound->Header.MenuSync2No) + " PC :" + IntToStr(DrinkMenuSync));
		this->Status.Error = Err_DrinkOutOfSync;
   }
   else if(this->Inbound->Header.InfoSyncNo != TGlobalSettings::Instance().InfoSync)
   {
      TManagerLogs::Instance().Add(__FUNC__,PALMLOG,"Info Sync # Failure. Palm :" + IntToStr(this->Inbound->Header.InfoSyncNo) + " PC :" + IntToStr(TGlobalSettings::Instance().InfoSync));
		this->Status.Error = Err_InfoOutOfSync;
   }
   else
   {
      return true;
   }
   return false;
}
//------------------------------------------------------------------------------
void __fastcall TPalm::RepackageOrders(TMemoryStream *Process_Outbound, int LastConnectVar)
{
	// Loop though all tables.
	unsigned char  SectionFlag = 0x00;
	unsigned char  TableNo 	   = 0;
	unsigned char  SeatNo 	   = 0;
	unsigned short CourseID 	= 0;
	unsigned short DishID 	   = 0;
	unsigned char  ItemID      = 0; // Used for HI and LO byte swapping.
	unsigned char  OptionID    = 0;
	unsigned char  GlassID     = 0;
	unsigned char  Flags       = 0;
	unsigned char  Flags2      = 0;

	signed char 	hPrice 	= 0;
	unsigned char 	lPrice 	= 0;
	AnsiString Note;

	unsigned char  thisTableNo 	= 0;
	unsigned char  thisSeatNo 	   = 0;

	int PosSectionFlag = 0;

	int PosLastTable = -1;
	int PosLastSeat = 0;
	int PosLastDish = 0;
	int PosFlags = 0;
	int PosFlags2 = 0;
	int PosFirstOrdersFlag2 = 0;
	int PosLastItem = 0;
   int PosMiscPrice = 0;

   #if (DEBUGLEVEL & debugExpandGetOrdes )
		TStringList *ErrorsList = new TStringList;
      int debugPosSectionFlag = 0;

      int debugPosLastTable = -1;
      int debugPosLastSeat = 0;
		int debugPosLastDish = 0;
      int debugPosFlags = 0;
      int debugPosFlags2 = 0;
		int debugPosFirstOrdersFlag2 = 0;
      int debugPosLastItem = 0;
      int debugPosMiscPrice = 0;
		int debugPosLastSubDish = 0;
      ErrorsList->Add("NEW GET ORDERS");
   #endif

   unsigned char AutoIncSplitCounter = 1;
   std::map<long,long> GroupKey;

   // Write out the Section flag.
   // 1 byte	Sections Flag	     	//	Bit 7 = Last flags byte
												//	Bit 6 = Enable All Dishes/Drinks // No Data
												//	Bit 5 = Disabled Dishes/Drinks
												//	Bit 4 = Seat Names
                                    // Bit 3 = GetOrders/SendOrders
                                    // Bit 2 = Message
                                    // Bit 1 = NA
                                    // Bit 0 = NA

   // Write out the Section flag.
   // 1 byte	Dish Flag   	     	//	Bit 7 = Last flags byte
												//	Bit 6 = Enable All Dishes/Drinks // No Data
												//	Bit 5 = Disabled Dishes/Drinks
												//	Bit 4 = Seat Names
                                    // Bit 3 = GetOrders/SendOrders
                                    // Bit 2 = Message
                                    // Bit 1 = NA
                                    // Bit 0 = NA


   PosSectionFlag = Process_Outbound->Position;
	Process_Outbound->Write(&SectionFlag,1);
   #if (DEBUGLEVEL & debugExpandGetOrdes )
	   debugPosSectionFlag = ErrorsList->Count;
		ErrorsList->Add("Section Flag Place Holder");
	#endif

   // Loop though all courses dishes looking for disabled ones...

   if(this->getMenuEnabledStateChange())
   {
      SectionFlag |= ROSectionFlagEnableAllDishes; // State has chage set to refresh palm.
      int Count = 0;
      PosLastDish = 0;
      for(int i = 0; i < TDeviceRealTerminal::Instance().Menus.MenuFood->Count; i++)
		{
         TListCourse *Course = TDeviceRealTerminal::Instance().Menus.MenuFood->CourseGet(i);
         if(Course->ViewableLocations & LocPalm)
         {
// TODO 3 -o Michael -c Improvement : In order to re-enable this you will haveto add an enabled property to courses.
/*            if(frmData->ibwrkCourse->FieldByName("enabled")->AsBoolean == false)
            {
               // Pull the Course Information.
               CourseID = BYTESWAP(frmData->ibwrkCourse->FieldByName("palm_course_id")->AsInteger);
               PosLastItem = Process_Outbound->Position;
               Process_Outbound->Write(&CourseID,2);
               #if (DEBUGLEVEL & debugExpandGetOrdes )
                  debugPosLastItem = ErrorsList->Count;
                  char out[10];
                  AnsiString TextOut,Fo0,Fo1;
                  ZeroMemory(out,10);
                  sprintf(out,"%02X ",HIBYTE(CourseID));
                  Fo0 = out;
                  Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
                  TextOut += Fo1 + " <- This Course ID is Disabled (HIBYTE)";
                  ErrorsList->Add(TextOut);
						TextOut = "";
                  Fo0 = "";
                  Fo1 = "";

                  ZeroMemory(out,10);
                  sprintf(out,"%02X ",LOBYTE(CourseID));
                  Fo0 = out;
                  Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
                  TextOut += Fo1 + " <- This Course ID is Disabled (LOBYTE)";
                  ErrorsList->Add(TextOut);
                  TextOut = "";
                  Fo0 = "";
                  Fo1 = "";
               #endif
            }
				else // Check each dish in turn.*/
				{

               for(int i=0; i<Course->Count; i++)
               {
                  TItem *Item = Course->ItemGet(i);
						if(Item->Enabled == false)
                  {
                     // Pull the Course Information.
							DishID = BYTESWAP(Item->Palm_ID);
                     PosLastItem = Process_Outbound->Position;
                     Process_Outbound->Write(&DishID,2);
                     #if (DEBUGLEVEL & debugExpandGetOrdes )
                        debugPosLastItem = ErrorsList->Count;
                        char out[10];
                        AnsiString TextOut,Fo0,Fo1;
                        ZeroMemory(out,10);
                        sprintf(out,"%02X ",LOBYTE(DishID)); //Low byte DishID is Hibyte due to ByteSwap.
                        Fo0 = out;
                        Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
                        TextOut += Fo1 + " <- This Dish ID is Disabled (HIBYTE)";
								ErrorsList->Add(TextOut);
								TextOut = "";
								Fo0 = "";
								Fo1 = "";

                        ZeroMemory(out,10);
                        sprintf(out,"%02X ",HIBYTE(DishID)); //Hibyte DishID is Lobyte due to ByteSwap.
                        Fo0 = out;
                        Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
                        TextOut += Fo1 + " <- This Dish ID is Disabled (LOBYTE)";
                        ErrorsList->Add(TextOut);
                        TextOut = "";
                        Fo0 = "";
                        Fo1 = "";
                     #endif
						}
               }
            }
         }
      }
   }

   if(this->getDrinksEnabledStateChange())
   {
      SectionFlag |= ROSectionFlagEnableAllDrinks;
      // Reset used Variables.
      CourseID 	   = 0;
      DishID 	      = 0;
      int Count = 0;
      PosLastDish = 0;

      for(int i = 0; i < TDeviceRealTerminal::Instance().Menus.MenuDrinks->Count; i++)
		{
         TListCourse *Course = TDeviceRealTerminal::Instance().Menus.MenuDrinks->CourseGet(i);
         if(Course->ViewableLocations & LocPalm)
         {
// TODO 3 -o Michael -c Improvement : In order to re-enable this you will haveto add an enabled property to courses.
/*            if(frmData->ibwrkCourse->FieldByName("enabled")->AsBoolean == false)
				{
					// Pull the Course Information.
               CourseID = BYTESWAP(frmData->ibwrkCourse->FieldByName("palm_course_id")->AsInteger);
               PosLastItem = Process_Outbound->Position;
               Process_Outbound->Write(&CourseID,2);
               #if (DEBUGLEVEL & debugExpandGetOrdes )
                  debugPosLastItem = ErrorsList->Count;
                  char out[10];
                  AnsiString TextOut,Fo0,Fo1;
                  ZeroMemory(out,10);
                  sprintf(out,"%02X ",HIBYTE(CourseID));
                  Fo0 = out;
						Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
                  TextOut += Fo1 + " <- This Course ID is Disabled (HIBYTE)";
                  ErrorsList->Add(TextOut);
                  TextOut = "";
                  Fo0 = "";
                  Fo1 = "";

                  ZeroMemory(out,10);
                  sprintf(out,"%02X ",LOBYTE(CourseID));
                  Fo0 = out;
                  Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
                  TextOut += Fo1 + " <- This Course ID is Disabled (LOBYTE)";
                  ErrorsList->Add(TextOut);
                  TextOut = "";
                  Fo0 = "";
                  Fo1 = "";
               #endif
            }
				else // Check each dish in turn.*/
				{
               for(int i=0; i<Course->Count; i++)
               {
                  TItem *Item = Course->ItemGet(i);
						if(Item->Enabled == false)
                  {
                     // Pull the Course Information.
                     DishID = BYTESWAP(Item->Palm_ID);
                     PosLastItem = Process_Outbound->Position;
                     Process_Outbound->Write(&DishID,2);
                     #if (DEBUGLEVEL & debugExpandGetOrdes )
                        debugPosLastItem = ErrorsList->Count;
                        char out[10];
                        AnsiString TextOut,Fo0,Fo1;
                        ZeroMemory(out,10);
                        sprintf(out,"%02X ",LOBYTE(DishID));  //Low byte DishID is Hibyte due to ByteSwap.
								Fo0 = out;
                        Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
								TextOut += Fo1 + " <- This Drink ID is Disabled (HIBYTE)";
                        ErrorsList->Add(TextOut);
                        TextOut = "";
								Fo0 = "";
                        Fo1 = "";

                        ZeroMemory(out,10);
                        sprintf(out,"%02X ",HIBYTE(DishID));  //Hibyte DishID is Lobyte due to ByteSwap.
                        Fo0 = out;
                        Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
                        TextOut += Fo1 + " <- This Drink ID is Disabled (LOBYTE)";
                        ErrorsList->Add(TextOut);
                        TextOut = "";
                        Fo0 = "";
                        Fo1 = "";
                     #endif
                  }
               }
				}
			}
      }
	}

   if(PosLastItem != 0)
   {
      SectionFlag |= ROSectionFlagHasDisabledItems; // State has chage set to refresh palm.
      int cntPos = Process_Outbound->Position;
      Process_Outbound->Position = PosLastItem;
      Process_Outbound->Read(&OptionID,1);
      OptionID |= 0x80;
		Process_Outbound->Position = PosLastItem;
      Process_Outbound->Write(&OptionID,1);
      PosLastItem = 0;
		Process_Outbound->Position = cntPos;
      #if (DEBUGLEVEL & debugExpandGetOrdes )
         char out[10];
         AnsiString TextOut,Fo0,Fo1;
         ZeroMemory(out,10);
         sprintf(out,"%02X ",OptionID);
         Fo0 = out;
         Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
         TextOut += Fo1 + " <- Last Disable (HIBYTE)";
         ErrorsList->Strings[debugPosLastItem] = TextOut;
         TextOut = "";
         Fo0 = "";
         Fo1 = "";
      #endif
   }

   // Reset used Variables.
   PosLastDish    = 0;
	CourseID 	   = 0;
	DishID 	      = 0;
	OptionID       = 0;

	SeatNames->Sort(OrderSeatNames);
	for(int i=0; i<SeatNames->Count; i++)
	{
		TPalmSeatName *SeatName = (TPalmSeatName *)SeatNames->Items[i];
		if (SeatName)
		{
			TableNo = SeatName->TableNumber;
			SeatNo = SeatName->SeatNumber;
			// There are Seat Names so Set Bit 4 of section flag.
			SectionFlag |= ROSectionFlagContainsSeatName;
			// The follwing Ifs check that it hasnt been written before.
			if(thisTableNo != TableNo)
			{
				// Table Number hase changed. Set the MSB for the last Seat.
				if(PosLastSeat != 0)
				{
					unsigned char AlterMSB;
					int cntPos = Process_Outbound->Position;
					Process_Outbound->Position = PosLastSeat;
					Process_Outbound->Read(&AlterMSB,1);
					AlterMSB |= 0x80;
					Process_Outbound->Position = PosLastSeat;
					Process_Outbound->Write(&AlterMSB,1);
					PosLastSeat = 0;
					Process_Outbound->Position = cntPos;
					#if (DEBUGLEVEL & debugExpandGetOrdes )
						char out[10];
						AnsiString TextOut,Fo0,Fo1;
						ZeroMemory(out,10);
						sprintf(out,"%02X ",AlterMSB);
						Fo0 = out;
						Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
						TextOut += Fo1 + " <- Last Seat for Seat Names";
						ErrorsList->Strings[debugPosLastSeat] = TextOut;
						TextOut = "";
						Fo0 = "";
						Fo1 = "";
					#endif
				}
//          #if (DEBUGLEVEL & debugExpandGetOrdes )
//				else
//				{
//					debugPosLastTablesLastSeatToggle = false;
//				}
//				#endif
				thisTableNo = TableNo;
				// write the Table Number.
				PosLastTable = Process_Outbound->Position;
				Process_Outbound->Write(&TableNo,1);
				thisSeatNo = 0;
				#if (DEBUGLEVEL & debugExpandGetOrdes )
					debugPosLastTable = ErrorsList->Count;
					char out[10];
					AnsiString TextOut,Fo0,Fo1;
					ZeroMemory(out,10);
					sprintf(out,"%02X ",TableNo);
					Fo0 = out;
					Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
					TextOut += Fo1 + " <- Table Number";
					ErrorsList->Add(TextOut);
					TextOut = "";
					Fo0 = "";
					Fo1 = "";
				#endif
			}

			if(thisSeatNo != SeatNo)
			{
				thisSeatNo = SeatNo;
				// write the Seat Number.
				PosLastSeat = Process_Outbound->Position;
				Process_Outbound->Write(&SeatNo,1);
				#if (DEBUGLEVEL & debugExpandGetOrdes )
					//Only update this if the Table number has not changed.
					debugPosLastSeat = ErrorsList->Count;
					char out[10];
					AnsiString TextOut,Fo0,Fo1;
					ZeroMemory(out,10);
					sprintf(out,"%02X ",SeatNo);
					Fo0 = out;
					Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
					TextOut += Fo1 + " <- Seat Number";
					ErrorsList->Add(TextOut);
					TextOut = "";
					Fo0 = "";
					Fo1 = "";
				#endif
			}

		// write the Seat Name.
			if(SeatName->SeatName.Length() != 0)      // Seat Name is not empty
			{
				Process_Outbound->Write(SeatName->SeatName.c_str(),SeatName->SeatName.Length());
				Process_Outbound->Position -= 1;
				Process_Outbound->Read(&OptionID,1);   // Recycle Option ID.
				OptionID |= 0x80;
				Process_Outbound->Position -= 1;
				Process_Outbound->Write(&OptionID,1);
				OptionID = 0x00;
				#if (DEBUGLEVEL & debugExpandGetOrdes )
					for (int i = 1; i <= SeatName->SeatName.Length(); i++)
					{
						char out[10];
						AnsiString TextOut,Fo0,Fo1;
						ZeroMemory(out,10);
						if(i == SeatName->SeatName.Length())
						{
							sprintf(out,"%02X ",(SeatName->SeatName[i] | 0x80));
						}
						else
						{
							sprintf(out,"%02X ",SeatName->SeatName[i]);
						}
						Fo0 = out;
						Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
						if(i == SeatName->SeatName.Length())
						{
							TextOut += Fo1 + " <- Seat Name Last Char " + SeatName->SeatName[i];
						}
						else
						{
							TextOut += Fo1 + " <- Seat Name Char " + SeatName->SeatName[i];
						}
						ErrorsList->Add(TextOut);
						TextOut = "";
						Fo0 = "";
						Fo1 = "";
					}
				#endif
			}
			else // Seat Name is empty for deleation.
			{
				OptionID = 0x80;
				Process_Outbound->Write(&OptionID,1);
				#if (DEBUGLEVEL & debugExpandGetOrdes )
					debugPosLastSeat = ErrorsList->Count;
					char out[10];
					AnsiString TextOut,Fo0,Fo1;
					ZeroMemory(out,10);
					sprintf(out,"%02X ",OptionID);
					Fo0 = out;
					Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
					TextOut += Fo1 + " <- Please Remove Seat Name";
					ErrorsList->Add(TextOut);
					TextOut = "";
					Fo0 = "";
					Fo1 = "";
				#endif
				OptionID = 0x00;
			}
			// Set for deletion in beam is successful.
			SeatName->DeleteMe = true;
		}
	}

	// Correct the Table Number as Last Table.
	if(PosLastTable != -1)
	{
		// Set the MSB for the last Seat for the last table.
		if(PosLastSeat != 0)
      {
         unsigned char AlterMSB;
         int cntPos = Process_Outbound->Position;
         Process_Outbound->Position = PosLastSeat;
         Process_Outbound->Read(&AlterMSB,1);
         AlterMSB |= 0x80;
         Process_Outbound->Position = PosLastSeat;
         Process_Outbound->Write(&AlterMSB,1);
			PosLastSeat = 0;
         Process_Outbound->Position = cntPos;
         #if (DEBUGLEVEL & debugExpandGetOrdes )
            char out[10];
				AnsiString TextOut,Fo0,Fo1;
            ZeroMemory(out,10);
            sprintf(out,"%02X ",AlterMSB);
            Fo0 = out;
            Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
            TextOut += Fo1 + " <- Last Seat for Last Table for Seat Names";
            ErrorsList->Strings[debugPosLastSeat] = TextOut;
            TextOut = "";
            Fo0 = "";
            Fo1 = "";
         #endif
      }

      unsigned char AlterMSB;
      int cntPos = Process_Outbound->Position;
      Process_Outbound->Position = PosLastTable;
      Process_Outbound->Read(&AlterMSB,1);
      AlterMSB |= 0x80;
      Process_Outbound->Position = PosLastTable;
      Process_Outbound->Write(&AlterMSB,1);
      Process_Outbound->Position = cntPos;
      #if (DEBUGLEVEL & debugExpandGetOrdes )
         char out[10];
         AnsiString TextOut,Fo0,Fo1;
         ZeroMemory(out,10);
         sprintf(out,"%02X ",AlterMSB);
			Fo0 = out;
         Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
			TextOut += Fo1 + " <- Last Table for Seat Names";
         ErrorsList->Strings[debugPosLastTable] = TextOut;
         TextOut = "";
			Fo0 = "";
         Fo1 = "";
      #endif
   }


	TableNo 	   = 0;
	SeatNo 	   = 0;
	OptionID    = 0;
	thisTableNo = 0;
	thisSeatNo 	= 0;
	PosLastTable = -1;
	PosLastSeat = 0;
	Flags       = 0;
	Flags2      = 0x80;

	TList *OrdersList = new TList;

	TIBSQL *qrOrders		= NULL;
	TIBSQL *qrOrderMix	= NULL;
	TIBQuery *qrOptions		= NULL;
	try
	{
		qrOrders		= new TIBSQL(frmData);
		qrOrderMix	= new TIBSQL(frmData);
		qrOptions	= new TIBQuery(frmData);

		qrOrders->Database = frmData->IBDatabase;
		qrOrders->Transaction = frmData->IBTransaction;

		qrOrderMix->Database = frmData->IBDatabase;
		qrOrderMix->Transaction = frmData->IBTransaction;

		qrOptions->Database = frmData->IBDatabase;
		qrOptions->Transaction = frmData->IBTransaction;

		if (!qrOrders->Transaction->InTransaction) qrOrders->Transaction->StartTransaction();
		if (DownloadOrders == ThisCurrentTable)
		{
			qrOrders->SQL->Text =
            "SELECT SEAT.SEATNO,ORDER_KEY,MENU_NAME,COURSE_NAME,ITEM_NAME,SIZE_NAME,COST,HAPPYHOUR,PRICE, "
            "PRICE_ADJUST,SETMENU_GROUP,NOTE,TAB_KEY,MASTER_CONTAINER,TABLES.TABLE_NUMBER "
            "FROM ORDERS INNER JOIN TAB ON TAB.TAB_KEY = ORDERS.TAB_KEY INNER JOIN SEAT ON SEAT.TAB_KEY = TAB.TAB_KEY "
            "INNER JOIN TABLES ON TABLES.TABLE_KEY = SEAT.TABLE_KEY "
            "WHERE ORDER_TYPE = :ORDER_TYPE AND SIDE_ORDER_KEY IS NULL AND TABLES.TABLE_NUMBER = :TABLE_NUMBER "
            "ORDER BY TABLES.TABLE_NUMBER ASC, SEAT.SEATNO ASC";
			qrOrders->ParamByName("TABLE_NUMBER")->AsInteger = CurrentTable;
		}
		else if (DownloadOrders == ActiveTables)
		{
			AnsiString SQLText =
            "SELECT SEAT.SEATNO,ORDER_KEY,MENU_NAME,COURSE_NAME,ITEM_NAME,SIZE_NAME,COST,HAPPYHOUR,PRICE, "
            "PRICE_ADJUST,SETMENU_GROUP,NOTE,TAB_KEY,MASTER_CONTAINER,TABLES.TABLE_NUMBER "
            "FROM ORDERS INNER JOIN TAB ON TAB.TAB_KEY = ORDERS.TAB_KEY INNER JOIN SEAT ON SEAT.TAB_KEY = TAB.TAB_KEY "
            "INNER JOIN TABLES ON TABLES.TABLE_KEY = SEAT.TABLE_KEY "
            "WHERE ORDER_TYPE = :ORDER_TYPE AND SIDE_ORDER_KEY IS NULL AND TABLES.TABLE_NUMBER IN (";
			bool First = true;
			for (std::set<long>::iterator Temp = CurrentTables.begin();
				  Temp != CurrentTables.end();
				  advance(Temp, 1))
			{
				if (First)
				{
					SQLText += IntToStr(*Temp);
					First = false;
				}
				else
				{
					SQLText += "," + IntToStr(*Temp);
				}
			}
			SQLText +=
				")ORDER BY "
					"TABLES.TABLE_NUMBER ASC, SEAT.SEATNO ASC";
			qrOrders->SQL->Text = SQLText;
		}
		else
		{
	   		qrOrders->SQL->Text =
            "SELECT SEAT.SEATNO,ORDER_KEY,MENU_NAME,COURSE_NAME,ITEM_NAME,SIZE_NAME,COST,HAPPYHOUR,PRICE, "
            "PRICE_ADJUST,SETMENU_GROUP,NOTE,TAB_KEY,MASTER_CONTAINER,TABLES.TABLE_NUMBER "
            "FROM ORDERS INNER JOIN TAB ON TAB.TAB_KEY = ORDERS.TAB_KEY INNER JOIN SEAT ON SEAT.TAB_KEY = TAB.TAB_KEY "
            "INNER JOIN TABLES ON TABLES.TABLE_KEY = SEAT.TABLE_KEY "
            "WHERE ORDER_TYPE = :ORDER_TYPE AND TAB.TAB_TYPE = :TAB_TYPE AND SIDE_ORDER_KEY IS NULL "
				"ORDER BY "
				"TABLES.TABLE_NUMBER ASC, SEAT.SEATNO ASC";
      		qrOrders->ParamByName("TAB_TYPE")->AsInteger = TabTableSeat;
		}
		// Proboably no need to check that TABLE_NUMBER > 0. Could rely on TAB_TYPE.
		qrOrders->ParamByName("ORDER_TYPE")->AsInteger = NormalOrder;
		qrOrderMix->SQL->Text =
			"SELECT "
				"COURSE_NAME, "
				"ITEM_NAME, "
				"SIZE_NAME, "
				"PRICE, "
				"PRICE_ADJUST "
			"FROM "
				"ORDERS "
			"WHERE "
				"SIDE_ORDER_KEY = :ORDER_KEY";

		qrOptions->SQL->Text =
			"SELECT "
				"OPTION_NAME,PLUS "
			"FROM "
				"ORDEROPTION "
			"WHERE "
				"ORDER_KEY = :ORDER_KEY";

		qrOrders->ExecQuery();
		qrOrderMix->ParamByName("ORDER_KEY")->AsInteger = qrOrders->FieldByName("ORDER_KEY")->AsInteger;
		qrOrderMix->ExecQuery();

      Currency  MiscPrice = 0;  // Used for Price of Items sent to palm.
		while (!qrOrders->Eof)
		{
			// Load the record into memory then jump to next to detect next seat/table
			int LoopThisTableNumber		= qrOrders->FieldByName("TABLE_NUMBER")->AsInteger;
			int LoopThisSeatNumber		= qrOrders->FieldByName("SEATNO")->AsInteger;

			TableNo = (unsigned char)LoopThisTableNumber;

			PosFirstOrdersFlag2 = 0;
			unsigned char  Flags = 0;
			unsigned char  Flags2 = 0;

			SeatNo = (unsigned char)LoopThisSeatNumber;
			///////////////////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////////////////

			TReturnItemComplete *CurrentItem = NULL;

			TListMenu *Menu = TDeviceRealTerminal::Instance().Menus.IsACurrentMenu(qrOrders->FieldByName("MENU_NAME")->AsString);
			if(Menu != NULL)
			{
				TListCourse *Course = Menu->FetchCourseByName(qrOrders->FieldByName("COURSE_NAME")->AsString);
				if(Course != NULL)
				{
					TItem *Item = Course->FetchItemByName(qrOrders->FieldByName("ITEM_NAME")->AsString);
					if(Item != NULL)
					{
						TItemSize *ItemsSize = Item->Sizes->SizeGet(qrOrders->FieldByName("SIZE_NAME")->AsString);
						// We have Everything we need create and return a complete Item.
						if(ItemsSize != NULL)
						{
							TReturnItemComplete *ReturnItem	= new TReturnItemComplete;
							ReturnItem->Palm_ID					= Item->Palm_ID;
							ReturnItem->Palm_Size_ID			= ItemsSize->Palm_ID;
							ReturnItem->OrderType				= NormalOrder;
							ReturnItem->SetMenuItem				= ItemsSize->SetMenuItem;
							ReturnItem->SetMenuMaster			= ItemsSize->SetMenuMaster;
							ReturnItem->SetMenuMask				= ItemsSize->SetMenuMask;
							ReturnItem->ViewableLocations		= Item->ViewableLocations;

							ReturnItem->Cost						= qrOrders->FieldByName("COST")->AsDouble;
							ReturnItem->HappyHour				= (qrOrders->FieldByName("HAPPYHOUR")->AsString == "T");
							ReturnItem->Price						= qrOrders->FieldByName("PRICE")->AsDouble;
							ReturnItem->PriceAdjust				= qrOrders->FieldByName("PRICE_ADJUST")->AsDouble;
							ReturnItem->SetMenuGroup			= qrOrders->FieldByName("SETMENU_GROUP")->AsInteger;
							ReturnItem->Note						= qrOrders->FieldByName("NOTE")->AsString;

							// Sort out Set Menu Grouping Numbers.
							if(qrOrders->FieldByName("SETMENU_GROUP")->AsInteger != 0)
							{
								// Look up number for this Tab.
								TTabsSetMenuList::iterator CurrentTabsSetMenus = TDeviceRealTerminal::Instance().TabsSetMenuList.find(qrOrders->FieldByName("TAB_KEY")->AsInteger);
								if(CurrentTabsSetMenus != TDeviceRealTerminal::Instance().TabsSetMenuList.end())
								{  // Tab already exists and is in List.
									TSetMenuList::iterator SetMenuNumber = std::find(CurrentTabsSetMenus->second.begin(),CurrentTabsSetMenus->second.end(),qrOrders->FieldByName("SETMENU_GROUP")->AsInteger);
									if(SetMenuNumber != CurrentTabsSetMenus->second.end())
									{
										ReturnItem->SetMenuGroup = (SetMenuNumber - CurrentTabsSetMenus->second.begin()) +1 ; // Zero based vector.
									}
									else // Add Set Menu Number to the List.
									{
										CurrentTabsSetMenus->second.push_back(qrOrders->FieldByName("SETMENU_GROUP")->AsInteger);
										SetMenuNumber = std::find(CurrentTabsSetMenus->second.begin(),CurrentTabsSetMenus->second.end(), qrOrders->FieldByName("SETMENU_GROUP")->AsInteger);
										ReturnItem->SetMenuGroup = (SetMenuNumber - CurrentTabsSetMenus->second.begin())+1;
									}
								}
								else
								{  // New table / seat -> tab with setmenu stuff.
									TDeviceRealTerminal::Instance().TabsSetMenuList[qrOrders->FieldByName("TAB_KEY")->AsInteger].push_back(qrOrders->FieldByName("SETMENU_GROUP")->AsInteger);
									TSetMenuList::iterator SetMenuNumber = std::find(  TDeviceRealTerminal::Instance().TabsSetMenuList[qrOrders->FieldByName("TAB_KEY")->AsInteger].begin(),
																										TDeviceRealTerminal::Instance().TabsSetMenuList[qrOrders->FieldByName("TAB_KEY")->AsInteger].end(),
																										qrOrders->FieldByName("SETMENU_GROUP")->AsInteger);
									ReturnItem->SetMenuGroup = (SetMenuNumber - TDeviceRealTerminal::Instance().TabsSetMenuList[qrOrders->FieldByName("TAB_KEY")->AsInteger].begin())+1;
								 }
							}

							qrOptions->Close();
							qrOptions->ParamByName("ORDER_KEY")->AsInteger = qrOrders->FieldByName("ORDER_KEY")->AsInteger;
							for(qrOptions->Open(); !qrOptions->Eof; qrOptions->Next())
							{
								TItemOption *Option = Item->FetchOptionByName(qrOptions->FieldByName("OPTION_NAME")->AsString);
								if(Option != NULL)
								{
									TReturnOption ReturnOption;
									ReturnOption.Palm_ID = Option->Palm_ID;
                           ReturnOption.IsPlus = qrOptions->FieldByName("PLUS")->AsBoolean;
									ReturnItem->OptionsSelected.push_back(ReturnOption);
								}
							}

							qrOrderMix->Close();
							qrOrderMix->ParamByName("ORDER_KEY")->AsInteger = qrOrders->FieldByName("ORDER_KEY")->AsInteger;
							for(qrOrderMix->ExecQuery(); !qrOrderMix->Eof; qrOrderMix->Next())
							{
								// TODO 2 -o Michael -c Assumption Sub Orders : This makes the huge
								//assumption that the Suborder was ordered from the same Menu as the
								//master item.
								TListCourse *SubCourse = Menu->FetchCourseByName(qrOrderMix->FieldByName("COURSE_NAME")->AsString);
								if(SubCourse != NULL)
								{
									TItem *SubItem = SubCourse->FetchItemByName(qrOrderMix->FieldByName("ITEM_NAME")->AsString);
									if(SubItem != NULL)
									{
										TItemSize *SubSize = SubItem->Sizes->SizeGet(qrOrderMix->FieldByName("SIZE_NAME")->AsString);
										if(SubSize != NULL)
										{
											TReturnItemCompleteSub SubOrder;
											SubOrder.Palm_ID = SubItem->Palm_ID;
											SubOrder.Palm_Size_ID = SubSize->Palm_ID;
											ReturnItem->SubOrders.push_back(SubOrder);
										}
									}
								}
							}

							TItemSize *MasterSize = Menu->Sizes->SizeGet(qrOrders->FieldByName("MASTER_CONTAINER")->AsString);
							if(MasterSize != NULL)
							{
								ReturnItem->Palm_MasterContainer_ID = MasterSize->Palm_ID;
							}
							CurrentItem = ReturnItem;
						}
					}
				}
			}

			if(CurrentItem != NULL)
			{
				OrdersList->Add(CurrentItem);
			}

			///////////////////////////////////////////////////////////////////////////
         MiscPrice += qrOrders->FieldByName("PRICE")->AsCurrency  +
                      qrOrders->FieldByName("PRICE_ADJUST")->AsCurrency;

         qrOrderMix->Close();
         qrOrderMix->ParamByName("ORDER_KEY")->AsInteger = qrOrders->FieldByName("ORDER_KEY")->AsInteger;
         for(qrOrderMix->ExecQuery(); !qrOrderMix->Eof; qrOrderMix->Next())
         {
            MiscPrice += qrOrderMix->FieldByName("PRICE")->AsCurrency  +
                         qrOrderMix->FieldByName("PRICE_ADJUST")->AsCurrency;
         }
			///////////////////////////////////////////////////////////////////////////

			qrOrders->Next();

			qrOrderMix->Close();
			qrOrderMix->ParamByName("ORDER_KEY")->AsInteger = qrOrders->FieldByName("ORDER_KEY")->AsInteger;
			qrOrderMix->ExecQuery();

			if ((qrOrders->FieldByName("SEATNO")->AsInteger != LoopThisSeatNumber) ||
				 (qrOrders->FieldByName("TABLE_NUMBER")->AsInteger != LoopThisTableNumber) ||
				  qrOrders->Eof)
			{
				OrdersList->Sort(OrderListSort);
				for(int i = 0 ; i < OrdersList->Count; i++)
				{
					TReturnItemComplete *CurrentItem = (TReturnItemComplete *)OrdersList->Items[i];
					if (CurrentItem->ViewableLocations & LocPalm)
					{
						// Contains orders so set the section flag.
						SectionFlag |= ROSectionFlagContainsOrders;
						// Pull the Order Information. (From SQL table)
						// Is it a Mixed Order (i.e a Mixed Drink)
						if(thisTableNo != TableNo)
						{
							thisTableNo = TableNo;
							// Store the Table Number.
							PosLastTable = Process_Outbound->Position;
							Process_Outbound->Write(&TableNo,1);
							thisSeatNo 		= 0;
							#if (DEBUGLEVEL & debugExpandGetOrdes )
								debugPosLastTable = ErrorsList->Count;
								char out[10];
								AnsiString TextOut,Fo0,Fo1;
								ZeroMemory(out,10);
								sprintf(out,"%02X ",TableNo);
								Fo0 = out;
								Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
								TextOut += Fo1 + " <- Table Number (Orders Section)";
								ErrorsList->Add(TextOut);
								TextOut = "";
								Fo0 = "";
								Fo1 = "";
							#endif
						}

						bool firstOrder = false;
						if(thisSeatNo != SeatNo)
						{
							thisSeatNo = SeatNo;
							// Store the Seat Number.
							PosLastSeat = Process_Outbound->Position;
							Process_Outbound->Write(&SeatNo,1);
							firstOrder = true;
							#if (DEBUGLEVEL & debugExpandGetOrdes )
								debugPosLastSeat = ErrorsList->Count;
								char out[10];
								AnsiString TextOut,Fo0,Fo1;
								ZeroMemory(out,10);
								sprintf(out,"%02X ",SeatNo);
								Fo0 = out;
								Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
								TextOut += Fo1 + " <- Seat Number";
								ErrorsList->Add(TextOut);
								TextOut = "";
								Fo0 = "";
								Fo1 = "";
							#endif
						}

						// Yes there is a Order for this table seat combination.
						// Adjust the Seat name Flag to State that there are orders.
						// Store the Flags ID.
						// Modify Flag.
						Flags = (0x00 | ROContainsOrders);
						unsigned short SetMenuMask = CurrentItem->SetMenuMask;
						if(SetMenuMask != 0 && CurrentItem->SetMenuGroup != 0)
						{
							Flags |= ROContainsPromoMask;
						}
						PosFlags = Process_Outbound->Position;
						Process_Outbound->Write(&Flags,1);
						#if (DEBUGLEVEL & debugExpandGetOrdes )
							debugPosFlags = ErrorsList->Count;
							ErrorsList->Add("First Flags Place Holder");
						#endif

						Flags2 = (0x80);
						PosFlags2 = Process_Outbound->Position;
						if(firstOrder)
						{
							PosFirstOrdersFlag2 = Process_Outbound->Position;
							#if (DEBUGLEVEL & debugExpandGetOrdes )
								debugPosFirstOrdersFlag2 = ErrorsList->Count;
							#endif
						}
						Process_Outbound->Write(&Flags2,1);
						#if (DEBUGLEVEL & debugExpandGetOrdes )
							debugPosFlags2 = ErrorsList->Count;
							ErrorsList->Add("Second Flags Place Holder");
						#endif

						if(firstOrder)
						{
							PosMiscPrice = Process_Outbound->Position;
							#if (DEBUGLEVEL & debugExpandGetOrdes )
								debugPosMiscPrice = ErrorsList->Count;
							#endif
						}

						unsigned char MasterContainerID = (unsigned char)CurrentItem->Palm_MasterContainer_ID;
						// You are going have to look up the plam dish ID for this Item.
						DishID = (unsigned short)CurrentItem->Palm_ID;
// TODO 5 -o Michael -c Palm Repackage Orders: This from of getting the Size_ID is not the best, and should be re thought out.
						GlassID = (unsigned short)CurrentItem->Palm_Size_ID;

						PosLastDish = Process_Outbound->Position;
						ItemID = HIBYTE(DishID);
						Process_Outbound->Write(&ItemID,1);
						ItemID = LOBYTE(DishID);
						Process_Outbound->Write(&ItemID,1);

						#if (DEBUGLEVEL & debugExpandGetOrdes )
							debugPosLastItem = ErrorsList->Count;
							char out[10];
							AnsiString TextOut,Fo0,Fo1;
							ZeroMemory(out,10);
							sprintf(out,"%02X ",HIBYTE(DishID));
							Fo0 = out;
							Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
							TextOut += Fo1 + " <- Item ID (HIBYTE)";
							ErrorsList->Add(TextOut);
							TextOut = "";
							Fo0 = "";
							Fo1 = "";

							ZeroMemory(out,10);
							sprintf(out,"%02X ",LOBYTE(DishID));
							Fo0 = out;
							Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
							TextOut += Fo1 + " <- Item ID (LOBYTE)";
							ErrorsList->Add(TextOut);
							TextOut = "";
							Fo0 = "";
							Fo1 = "";
						#endif

						// Maybe Write Size Type.
						Process_Outbound->Write(&GlassID,1);
						#if (DEBUGLEVEL & debugExpandGetOrdes )
							ZeroMemory(out,10);
							sprintf(out,"%02X ",GlassID);
							Fo0 = out;
							Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
							TextOut += Fo1 + " <- Size ID";
							ErrorsList->Add(TextOut);
							TextOut = "";
							Fo0 = "";
							Fo1 = "";
						#endif

						if(( (Flags & ROContainsPromoMask) == ROContainsPromoMask) && (CurrentItem->SubOrders.size() == 0))
						{
							Process_Outbound->Write(&CurrentItem->SetMenuGroup,1);
							#if (DEBUGLEVEL & debugExpandGetOrdes )
								ZeroMemory(out,10);
								sprintf(out,"%02X ",CurrentItem->SetMenuGroup);
								Fo0 = out;
								Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
								TextOut += Fo1 + " <- Set Menu Group ID";
								ErrorsList->Add(TextOut);
								TextOut = "";
								Fo0 = "";
								Fo1 = "";
							#endif
						}

						int PosLastSubDish = 0;
						int MaxPalmSides = 0; //
						ReturnSubOrdersList::iterator CurrentSubOrder;
						for(CurrentSubOrder = CurrentItem->SubOrders.begin();
							 CurrentSubOrder != CurrentItem->SubOrders.end(); MaxPalmSides++, CurrentSubOrder++)
						{
							if(MaxPalmSides < 4)
							{
								Flags |= ROSubOrdersMix;
								unsigned short LinkedDishID = (unsigned short)CurrentSubOrder->Palm_ID;
								unsigned short LinkedGlassID = (unsigned short)CurrentSubOrder->Palm_Size_ID;

								PosLastSubDish = Process_Outbound->Position;
								ItemID = HIBYTE(LinkedDishID);
								Process_Outbound->Write(&ItemID,1);
								ItemID = LOBYTE(LinkedDishID);
								Process_Outbound->Write(&ItemID,1);
								#if (DEBUGLEVEL & debugExpandGetOrdes )
									debugPosLastSubDish = ErrorsList->Count;
									char out[10];
									AnsiString TextOut,Fo0,Fo1;
									ZeroMemory(out,10);
									sprintf(out,"%02X ",HIBYTE(LinkedDishID));
									Fo0 = out;
									Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
									TextOut += Fo1 + " <- Sub Item ID (HIBYTE)";
									ErrorsList->Add(TextOut);
									TextOut = "";
									Fo0 = "";
									Fo1 = "";

									ZeroMemory(out,10);
									sprintf(out,"%02X ",LOBYTE(LinkedDishID));
									Fo0 = out;
									Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
									TextOut += Fo1 + " <- Sub Item ID (LOBYTE)";
									ErrorsList->Add(TextOut);
									TextOut = "";
									Fo0 = "";
									Fo1 = "";
								#endif
								// Write Container Type.
								Process_Outbound->Write(&LinkedGlassID,1);
								#if (DEBUGLEVEL & debugExpandGetOrdes )
									ZeroMemory(out,10);
									sprintf(out,"%02X ",LinkedGlassID);
									Fo0 = out;
									Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
									TextOut += Fo1 + " <- Sub Size ID";
									ErrorsList->Add(TextOut);
									TextOut = "";
									Fo0 = "";
									Fo1 = "";
								#endif
							}
						}

						if(PosLastSubDish != 0)
						{
							int cntPos = Process_Outbound->Position;
							Process_Outbound->Position = PosLastSubDish;
							Process_Outbound->Read(&ItemID,1);
							ItemID |= 0x80;
							Process_Outbound->Position = PosLastSubDish;
							Process_Outbound->Write(&ItemID,1);
							PosLastSubDish = 0;
							Process_Outbound->Position = cntPos;
							#if (DEBUGLEVEL & debugExpandGetOrdes )
								char out[10];
								AnsiString TextOut,Fo0,Fo1;
								ZeroMemory(out,10);
								sprintf(out,"%02X ",ItemID);
								Fo0 = out;
								Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
								TextOut += Fo1 + " <- Last Sub Item (HIBYTE)";
								ErrorsList->Strings[debugPosLastSubDish] = TextOut;
								TextOut = "";
								Fo0 = "";
								Fo1 = "";
							#endif
						}

						// Maybe Write Master Container Type.
						if(Flags & ROSubOrdersMix)
						{
							Process_Outbound->Write(&MasterContainerID,1);
							#if (DEBUGLEVEL & debugExpandGetOrdes )
								char out[10];
								AnsiString TextOut,Fo0,Fo1;
								ZeroMemory(out,10);
								sprintf(out,"%02X ",MasterContainerID);
								Fo0 = out;
								Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
								TextOut += Fo1 + " <- Master Size Type";
								ErrorsList->Add(TextOut);
								TextOut = "";
								Fo0 = "";
								Fo1 = "";
							#endif
							if( (Flags & ROContainsPromoMask) == ROContainsPromoMask)
							{
								Process_Outbound->Write(&CurrentItem->SetMenuGroup,1);
								#if (DEBUGLEVEL & debugExpandGetOrdes )
									ZeroMemory(out,10);
									sprintf(out,"%02X ",CurrentItem->SetMenuGroup);
									Fo0 = out;
									Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
									TextOut += Fo1 + " <- Set Menu Group ID";
									ErrorsList->Add(TextOut);
									TextOut = "";
									Fo0 = "";
									Fo1 = "";
								#endif
							}
						}

						//Options.
						ReturnOptionList::iterator CurrentOption = CurrentItem->OptionsSelected.begin();
						if(CurrentOption !=  CurrentItem->OptionsSelected.end())
						{
							Flags |= ROContainsOptions; // Set flags as we have options.
							OptionID = 0x00;
							// Write Every Option.
							for(;CurrentOption != CurrentItem->OptionsSelected.end(); CurrentOption++)
							{
								// Is it the last Option?
								if(CurrentOption == (CurrentItem->OptionsSelected.end()-1))
								{
									OptionID = (unsigned char)CurrentOption->Palm_ID;
                           if(!CurrentOption->IsPlus)
                           {
                              OptionID |= 0x40;
                           }
									OptionID |= 0x80;
									Process_Outbound->Write(&OptionID,1);
									#if (DEBUGLEVEL & debugExpandGetOrdes )
										char out[10];
										AnsiString TextOut,Fo0,Fo1;
										ZeroMemory(out,10);
										sprintf(out,"%02X ",OptionID);
										Fo0 = out;
										Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
										TextOut += Fo1 + " <- Last Option ID ";
										ErrorsList->Add(TextOut);
										TextOut = "";
										Fo0 = "";
										Fo1 = "";
									#endif
								}
								else
								{
									OptionID = (unsigned char)CurrentOption->Palm_ID;
                           if(!CurrentOption->IsPlus)
                           {
                              OptionID |= 0x40;
                           }
									Process_Outbound->Write(&OptionID,1);
									#if (DEBUGLEVEL & debugExpandGetOrdes )
										char out[10];
										AnsiString TextOut,Fo0,Fo1;
										ZeroMemory(out,10);
										sprintf(out,"%02X ",OptionID);
										Fo0 = out;
										Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
										TextOut += Fo1 + " <- Option ID ";
										ErrorsList->Add(TextOut);
										TextOut = "";
										Fo0 = "";
										Fo1 = "";
									#endif
								}
							}
						}

						// Note.
						if(CurrentItem->Note != "")
						{
							// Modify Flag.
							Flags |= ROContainsNote; // Set flags as we have options.
							// Proceed to add note here.
							Note = CurrentItem->Note;
							// Set the MSB of the last character of the Note.
							// Trim the notes to 80 characters (Max on palm).
							if(Note.Length() > 80)
							{
								Note.SetLength(80); // <- Lies does increase the length of the str
							}
							Process_Outbound->Write(Note.c_str(),Note.Length());
							Process_Outbound->Position -= 1;
							Process_Outbound->Read(&OptionID,1);   // Recycle Option ID.
							OptionID |= 0x80;
							Process_Outbound->Position -= 1;
							Process_Outbound->Write(&OptionID,1);
							OptionID = 0x00;
							#if (DEBUGLEVEL & debugExpandGetOrdes )
								for (int i = 1; i <= Note.Length(); i++)
								{
									debugPosLastSeat = ErrorsList->Count;
									char out[10];
									AnsiString TextOut,Fo0,Fo1;
									ZeroMemory(out,10);
									if(i == Note.Length())
									{
										sprintf(out,"%02X ",(Note[i] | 0x80));
									}
									else
									{
										sprintf(out,"%02X ",Note[i]);
									}
									Fo0 = out;
									Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
									if(i == Note.Length())
									{
										TextOut += Fo1 + " <- Note Char LAST";
									}
									else
									{
										TextOut += Fo1 + " <- Note Char";
									}
									ErrorsList->Add(TextOut);
									TextOut = "";
									Fo0 = "";
									Fo1 = "";
								}
							#endif
						}

// Flags dont need to be set here... an enhance ment could be to move them
//out of this inner loop?
						// Modify Flag.
						int cntPos = Process_Outbound->Position;
						Process_Outbound->Position = PosFlags;
						Process_Outbound->Write(&Flags,1);
						PosFlags = 0;
						Process_Outbound->Position = cntPos;
						#if (DEBUGLEVEL & debugExpandGetOrdes )
							ZeroMemory(out,10);
							sprintf(out,"%02X ",Flags);
							Fo0 = out;
							Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
							TextOut += Fo1 + " <- First Flag "
												+ ( (Flags & ROContainsOrders) ? 		" +Orders" 		: " -Orders")
												+ ( (Flags & ROContainsPromoMask) ? 	" +SetMenu " 	: " -SetMenu")
												+ ( (Flags & ROSubOrdersMix) ? 			" +SubItems" 	: " -SubItems")
												+ ( (Flags & ROContainsOptions) ? 		" +Options" 	: " -Options")
												+ ( (Flags & ROContainsPriceAdjust) ? 	" +PriceAdj " 	: " -PriceAdj")
												+ ( (Flags & ROContainsNote) ? 			" +Note" 		: " -Note");
							ErrorsList->Strings[debugPosFlags] = TextOut;
							TextOut = "";
							Fo0 = "";
							Fo1 = "";
						#endif

						// Modify 2nd Flag2.
						cntPos = Process_Outbound->Position;
						Process_Outbound->Position = PosFlags2;
						Process_Outbound->Write(&Flags2,1);
						PosFlags2 = 0;
						Process_Outbound->Position = cntPos;
						#if (DEBUGLEVEL & debugExpandGetOrdes )
							ZeroMemory(out,10);
							sprintf(out,"%02X ",Flags2);
							Fo0 = out;
							Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
							TextOut += Fo1 + " <- Second Flag "
												+ ( (Flags2 & RO2ContainsSplitGroupNumberAndPrice) ? 	" +Contains Splits GroupNo & Price " : " -Contains Splits GroupNo & Price ")
												+ ( (Flags2 & RO2ContainsMiscPricing) ? 					" +MiscPricing " 			: " -MiscPricing ");
							ErrorsList->Strings[debugPosFlags2] = TextOut;
							TextOut = "";
							Fo0 = "";
							Fo1 = "";
						#endif
					}
				}
				// We have run though all the courses so fix up the last Dish for this seat.
				if(PosLastDish != 0)
				{
					int cntPos = Process_Outbound->Position;
					Process_Outbound->Position = PosLastDish;
					Process_Outbound->Read(&ItemID,1);
					ItemID |= 0x80;
					Process_Outbound->Position = PosLastDish;
					Process_Outbound->Write(&ItemID,1);
					PosLastDish = 0;
					Process_Outbound->Position = cntPos;
					#if (DEBUGLEVEL & debugExpandGetOrdes )
						char out[10];
						AnsiString TextOut,Fo0,Fo1;
						ZeroMemory(out,10);
						sprintf(out,"%02X ",ItemID);
						Fo0 = out;
						Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
						TextOut += Fo1 + " <- Last Item ID (HIBYTE)";
						ErrorsList->Strings[debugPosLastItem] = TextOut;
						TextOut = "";
						Fo0 = "";
						Fo1 = "";
					#endif
				}

				if(MiscPrice != 0)
				{
					// We now need to insert the Misc Price after the seat No.
					// Shuffle everything along 4 bytes till we hit the Misc Price Pos...
					//if genuine orders then modify the first orders flag2.
					// Contains orders so set the section flag.
					SectionFlag |= ROSectionFlagContainsOrders;

					if(PosFirstOrdersFlag2 != 0)
					{
						int cntPos = Process_Outbound->Position;
						Process_Outbound->Position = PosFirstOrdersFlag2;
						Process_Outbound->Read(&Flags2,1);
						Flags2 |= RO2ContainsMiscPricing;
						Process_Outbound->Position = PosFirstOrdersFlag2;
						Process_Outbound->Write(&Flags2,1);
						Process_Outbound->Position = cntPos;
						#if (DEBUGLEVEL & debugExpandGetOrdes )
							char out[10];
							AnsiString TextOut,Fo0,Fo1;
							ZeroMemory(out,10);
							sprintf(out,"%02X ",Flags2);
							Fo0 = out;
							Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
							TextOut += Fo1 + " <- Second Flag Contains Misc Pricing";
							ErrorsList->Strings[debugPosFirstOrdersFlag2] = TextOut;
							TextOut = "";
							Fo0 = "";
							Fo1 = "";
						#endif
					}
					else //wirte out the table / seat / flags / misc price  packet
					{
						if(thisTableNo != TableNo)
						{
							thisTableNo = TableNo;
							// Store the Table Number.
							PosLastTable = Process_Outbound->Position;
							Process_Outbound->Write(&TableNo,1);
							thisSeatNo 		= 0;
							#if (DEBUGLEVEL & debugExpandGetOrdes )
								debugPosLastTable = ErrorsList->Count;
								char out[10];
								AnsiString TextOut,Fo0,Fo1;
								ZeroMemory(out,10);
								sprintf(out,"%02X ",TableNo);
								Fo0 = out;
								Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
								TextOut += Fo1 + " <- Table Number (Added By Misc Price part of Orders Section. No Orders found)";
								ErrorsList->Add(TextOut);
								TextOut = "";
								Fo0 = "";
								Fo1 = "";
							#endif

						}

						if(thisSeatNo != SeatNo)
						{
							thisSeatNo = SeatNo;
							// Store the Seat Number.
							PosLastSeat = Process_Outbound->Position;
							Process_Outbound->Write(&SeatNo,1);
							#if (DEBUGLEVEL & debugExpandGetOrdes )
								debugPosLastSeat = ErrorsList->Count;
								char out[10];
								AnsiString TextOut,Fo0,Fo1;
								ZeroMemory(out,10);
								sprintf(out,"%02X ",SeatNo);
								Fo0 = out;
								Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
								TextOut += Fo1 + " <- Seat Number (Added by Misc Price part of Orders Section. No Orders found)";
								ErrorsList->Add(TextOut);
								TextOut = "";
								Fo0 = "";
								Fo1 = "";
							#endif

						}

						// Yes there are no Orders for this table seat combination.
						// Adjust the Seat name Flag to State that there are no orders.
						// Store the Flags ID.
						// Modify Flag.
						Flags = (0x00);
						PosFlags = Process_Outbound->Position;
						Process_Outbound->Write(&Flags,1);
						#if (DEBUGLEVEL & debugExpandGetOrdes )
							char out[10];
							AnsiString TextOut,Fo0,Fo1;
							ZeroMemory(out,10);
							sprintf(out,"%02X ",Flags);
							Fo0 = out;
							Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
							TextOut += Fo1 + " <- First Flag (Added By Misc Price) "
												+ ( (Flags & ROContainsOrders) ? "+Orders  This Bit will be redundant see Section Flag " : "-Orders ")
												+ ( (Flags & ROContainsPromoMask) ? "+SetMenu " : "-SetMenu ")
												+ ( (Flags & ROSubOrdersMix) ? "+SubItems " : "-SubItems ")
												+ ( (Flags & ROContainsOptions) ? "+Options " : "-Options ")
												+ ( (Flags & ROContainsPriceAdjust) ? "+PriceAdj " : "-PriceAdj ")
												+ ( (Flags & ROContainsNote) ? "+Note " : "-Note ");
							ErrorsList->Add(TextOut);
							TextOut = "";
							Fo0 = "";
							Fo1 = "";
						#endif

						Flags2 = (0x80 | RO2ContainsMiscPricing);
						PosFlags2 = Process_Outbound->Position;
						Process_Outbound->Write(&Flags2,1);
						PosMiscPrice = Process_Outbound->Position;
						#if (DEBUGLEVEL & debugExpandGetOrdes )
							ZeroMemory(out,10);
							sprintf(out,"%02X ",Flags2);
							Fo0 = out;
							Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
							TextOut += Fo1 + " <- Second Flag (Added By Misc Price) "
												+ ( (Flags2 & RO2ContainsSplitGroupNumberAndPrice) ? "+GroupNoSplitsPrice " : "-GroupNoSplitsPrice ")
												+ ( (Flags2 & RO2ContainsMiscPricing) ? "+MiscPricing " : "-MiscPricing ");
							ErrorsList->Add(TextOut);
							TextOut = "";
							Fo0 = "";
							Fo1 = "";

							// Line up the MiscPrice Pos for the inserts.
							debugPosMiscPrice = ErrorsList->Count;
						#endif
					}

					Process_Outbound->Position = Process_Outbound->Size - 1; // Point to end of Stream Zero based.
					Process_Outbound->SetSize(int(Process_Outbound->Size + 4)); //Add 4 bytes.
					// Set Destination Ptr to the end of the now extended Memory Stream.
					char *Memory = (char *)Process_Outbound->Memory; // ptr to start of Memory Stream.
					char *Dest = &Memory[Process_Outbound->Size -1]; // Zero based array
					while(Process_Outbound->Position != PosMiscPrice-1)
					{
					  char ByteToBeMoved = NULL;
					  Process_Outbound->Read(&ByteToBeMoved,1);
					  Process_Outbound->Position--; // undo move forward due to read.
					  *Dest = ByteToBeMoved;
					  Process_Outbound->Position--;
					  Dest--;
					}
					// Write out the 4 Bytes for the Misc Price.
					long ValueOut = double(MiscPrice) * 100;
					Process_Outbound->Position = PosMiscPrice;
					unsigned char MiscOut = HIBYTE(HIWORD(ValueOut));
					Process_Outbound->Write(&MiscOut,1);
					MiscOut = LOBYTE(HIWORD(ValueOut));
					Process_Outbound->Write(&MiscOut,1);
					MiscOut = HIBYTE(LOWORD(ValueOut));
					Process_Outbound->Write(&MiscOut,1);
					MiscOut = LOBYTE(LOWORD(ValueOut));
					Process_Outbound->Write(&MiscOut,1);
					Process_Outbound->Position = Process_Outbound->Size;
					#if (DEBUGLEVEL & debugExpandGetOrdes )
						char out[10];
						AnsiString TextOut,Fo0,Fo1;
						ZeroMemory(out,10);
						sprintf(out,"%02X ",HIBYTE(HIWORD(ValueOut)));
						Fo0 = out;
						Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
						TextOut += Fo1 + " <- Misc Pricing HIBYTE-HIWORD";
						ErrorsList->Insert(debugPosMiscPrice,TextOut);
						TextOut = "";
						Fo0 = "";
						Fo1 = "";

						ZeroMemory(out,10);
						sprintf(out,"%02X ",LOBYTE(HIWORD(ValueOut)));
						Fo0 = out;
						Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
						TextOut += Fo1 + " <- Misc Pricing LOBYTE-HIWORD";
						ErrorsList->Insert(debugPosMiscPrice+1,TextOut);
						TextOut = "";
						Fo0 = "";
						Fo1 = "";

						ZeroMemory(out,10);
						sprintf(out,"%02X ",HIBYTE(LOWORD(ValueOut)));
						Fo0 = out;
						Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
						TextOut += Fo1 + " <- Misc Pricing HIBYTE-LOWORD";
						ErrorsList->Insert(debugPosMiscPrice+2,TextOut);
						TextOut = "";
						Fo0 = "";
						Fo1 = "";

						ZeroMemory(out,10);
						sprintf(out,"%02X ",LOBYTE(LOWORD(ValueOut)));
						Fo0 = out;
						Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
						TextOut += Fo1 + " <- Misc Pricing LOBYTE-LOWORD";
						ErrorsList->Insert(debugPosMiscPrice+3,TextOut);
						TextOut = "";
						Fo0 = "";
						Fo1 = "";
					#endif
               MiscPrice = 0;
				}
				// removed the orders for this tabe seat from the list.
				while(OrdersList->Count != 0)
				{
					TReturnItemComplete *CurrentItem = (TReturnItemComplete*) OrdersList->Items[0];
					delete CurrentItem;
					OrdersList->Delete(0);
				}
			}

			if (qrOrders->FieldByName("TABLE_NUMBER")->AsInteger != LoopThisTableNumber || qrOrders->Eof)
			{
				if(PosLastSeat != 0)
				{
					int cntPos = Process_Outbound->Position;
					Process_Outbound->Position = PosLastSeat;
					Process_Outbound->Read(&SeatNo,1);
					SeatNo |= 0x80;
					Process_Outbound->Position = PosLastSeat;
					Process_Outbound->Write(&SeatNo,1);
					PosLastSeat = 0;
					Process_Outbound->Position = cntPos;
					#if (DEBUGLEVEL & debugExpandGetOrdes )
						char out[10];
						AnsiString TextOut,Fo0,Fo1;
						ZeroMemory(out,10);
						sprintf(out,"%02X ",SeatNo);
						Fo0 = out;
						Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
						TextOut += Fo1 + " <- Last Seat for Orders Section";
						ErrorsList->Strings[debugPosLastSeat] = TextOut;
						TextOut = "";
						Fo0 = "";
						Fo1 = "";
					#endif

				}
			}
		}
	}
	__finally
	{
		if (!qrOrders->Transaction->InTransaction) qrOrders->Transaction->Commit();
		delete qrOrders;
		delete qrOrderMix;
		delete qrOptions;
	}
	// Correct the Table Number as Last Table.
	if(PosLastTable != -1)
	{
		int cntPos = Process_Outbound->Position;
		Process_Outbound->Position = PosLastTable;
		Process_Outbound->Read(&TableNo,1);
		TableNo |= 0x80;
		Process_Outbound->Position = PosLastTable;
		Process_Outbound->Write(&TableNo,1);
		Process_Outbound->Position = cntPos;
		#if (DEBUGLEVEL & debugExpandGetOrdes )
			char out[10];
         AnsiString TextOut,Fo0,Fo1;
         ZeroMemory(out,10);
         sprintf(out,"%02X ",TableNo);
         Fo0 = out;
         Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
         TextOut += Fo1 + " <- Last Table for Orders Section";
         ErrorsList->Strings[debugPosLastTable] = TextOut;
         TextOut = "";
         Fo0 = "";
         Fo1 = "";
      #endif

   }

   delete OrdersList;

   if(this->Message != "")
   {
      SectionFlag |= ROSectionFlagContainsMessage;
      if(this->From != "")
      {
         Process_Outbound->Write(this->From.c_str(),this->From.Length());
         Process_Outbound->Position -= 1;
         Process_Outbound->Read(&OptionID,1);   // Recycle Option ID.
         OptionID |= 0x80;
         Process_Outbound->Position -= 1;
         Process_Outbound->Write(&OptionID,1);
         OptionID = 0x00;
      }
      else
      {
         Process_Outbound->Write(this->User.Name.c_str(),this->User.Name.Length());
         Process_Outbound->Position -= 1;
         Process_Outbound->Read(&OptionID,1);   // Recycle Option ID.
         OptionID |= 0x80;
         Process_Outbound->Position -= 1;
			Process_Outbound->Write(&OptionID,1);
         OptionID = 0x00;
      }
      Process_Outbound->Write(this->Message.c_str(),this->Message.Length());
      Process_Outbound->Position -= 1;
      Process_Outbound->Read(&OptionID,1);   // Recycle Option ID.
      OptionID |= 0x80;
      Process_Outbound->Position -= 1;
      Process_Outbound->Write(&OptionID,1);
      OptionID = 0x00;
      this->Message = "";
      this->From = "";
   }

   // Edit and Adjust the Sections flag.
      int cntPos = Process_Outbound->Position;
      Process_Outbound->Position = PosSectionFlag;
      Process_Outbound->Write(&SectionFlag,1);
      Process_Outbound->Position = cntPos;
      #if (DEBUGLEVEL & debugExpandGetOrdes )
         char out[10];
         AnsiString TextOut,Fo0,Fo1;
         ZeroMemory(out,10);
         sprintf(out,"%02X ",SectionFlag);
         Fo0 = out;
         Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
         TextOut += Fo1 + " <- Section Flag "
								+ ( (SectionFlag & ROSectionFlagHasDisabledItems)  ? "+HasDisabledItems " : "-HasDisabledItems ")
								+ ( (SectionFlag & ROSectionFlagEnableAllDishes)   ? "+EnableAllDishes " : "-EnableAllDishes ")
								+ ( (SectionFlag & ROSectionFlagEnableAllDrinks)   ? "+EnableAllDrinks " : "-EnableAllDrinks ")
								+ ( (SectionFlag & ROSectionFlagContainsSeatName)  ? "+SeatName " : "-SeatName ")
								+ ( (SectionFlag & ROSectionFlagContainsOrders)    ? "+Orders " : "-Orders ")
								+ ( (SectionFlag & ROSectionFlagContainsMessage)   ? "+Message " : "-Message ");
         ErrorsList->Strings[debugPosSectionFlag] = TextOut;
         TextOut = "";
         Fo0 = "";
         Fo1 = "";
      #endif


   #if (DEBUGLEVEL & debugExpandGetOrdes )
      ErrorsList->Add("END GET ORDERS");
      for (int i = 0; i < ErrorsList->Count; i++)
      {
         TManagerLogs::Instance().Add(__FUNC__,"GET ORDERS",ErrorsList->Strings[i]);
      }
      delete ErrorsList;
   #endif

}

//---------------------------------------------------------------------------

bool TPalm::CheckUser()
{
   // Check the User ID against that palm.
   int StaffIDNo = this->Inbound->Header.StaffIDNo;
   if(this->User.ContactKey == 0)
   {
      TManagerLogs::Instance().Add(__FUNC__,"Inbound Check","Check User Failed User Not Logged in \n Palm : " + IntToStr(this->ID.DeviceID) + " User ID:" + IntToStr(this->User.ContactKey)
                        + " Staff No: " + IntToStr(StaffIDNo));
      this->Status.Error = Err_Not_Logged_In;
   }
   else if(StaffIDNo == 0)
   {
      TManagerLogs::Instance().Add(__FUNC__,"Inbound Check","Check User Failed. Staff No: " + IntToStr(StaffIDNo));
      this->Status.Error = Err_Not_Logged_In;
   }
   else if(this->User.ContactKey == StaffIDNo)
   {
      return true;
   }
   return false;
}

//---------------------------------------------------------------------------
void TPalm::Register()
{
	try
   {
      if(TDeviceRealTerminal::Instance().Registering)
      {
        int MaxPalmLicenses = TDeviceRealTerminal::Instance().RegisteredPalmLicences();

        if((TDeviceRealTerminal::Instance().PalmList->Count < MaxPalmLicenses) || (TDeviceRealTerminal::Instance().PalmList->Count <= MaxPalmLicenses && TDeviceRealTerminal::Instance().Reregistering))
        {
            // retrieve this palms ID from the database...
            AnsiString UniqueName;
            unsigned char NewID = TDeviceRealTerminal::Instance().RetrieveNewPalmID();
            if(TDeviceRealTerminal::Instance().Reregistering)
            {
               UniqueName = TDeviceRealTerminal::Instance().NewPalmRegisterName;
            }
            else
            {
               AnsiString TimeID = Now().FormatString("mmddhhmmss");
               UniqueName = TimeID + TDeviceRealTerminal::Instance().NewPalmRegisterName;
            }

            TComms::Instance().Process_Outbound->Clear();
            TComms::Instance().Process_Outbound->Write(&NewID,sizeof(NewID));
            TComms::Instance().Process_Outbound->Write(UniqueName.c_str(),UniqueName.Length());

            TPalm *Palm;
            if(TDeviceRealTerminal::Instance().Reregistering)
				{
// TODO 3 -o Michael -c Registration : Possible that PalmNotFound is returned here.
               Palm = TDeviceRealTerminal::Instance().FindTPalm(TDeviceRealTerminal::Instance().ReregisteringOldPalmID);
            }
            else
            {
               Palm = new TPalm();
               Palm->ID.DeviceID = NewID;
               Palm->ID.Name = UniqueName;
               Palm->ID.DisplayName = TDeviceRealTerminal::Instance().NewPalmRegisterName;
					Palm->ID.Product = TDeviceRealTerminal::Instance().ID.Product;
					Palm->ID.LocationKey = TDeviceRealTerminal::Instance().ID.LocationKey;
					Palm->ID.Location = TDeviceRealTerminal::Instance().ID.Location;
               Palm->TransNo = 0;
               TDeviceRealTerminal::Instance().PalmList->Add(Palm);
            }

            if(Palm != NULL)
            {
               if(Palm != TDeviceRealTerminal::Instance().PalmNotFound)
               {
                  if (!frmData->IBTransaction->InTransaction) { frmData->IBTransaction->StartTransaction(); }
                  Palm->ID.DeviceID = NewID;
                  Palm->Create();
						if(Palm->Locate())
                  {
                     for(int i = 0 ; i < TComms::Instance().ConnectLst->Count; i ++)
                     {
                        TConnect * TempCon = (TConnect *)TComms::Instance().ConnectLst->Items[i];
                        if (TempCon->TRXType == Windows_Printer)
                        {
                           TempCon->Verify(Palm->ID.DeviceKey,true);
                        }
                     }
                  }
                  if (frmData->IBTransaction->InTransaction) { frmData->IBTransaction->Commit(); }

                  this->Status.Error = proA_Ok;
                  BuildPacket(TComms::Instance().Process_Outbound);
               }
               else
               {
                  this->Status.Error = Err_DeviceID_Not_Found;
                  BuildPacket(NULL);
               }
            }
            else
            {
               TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Palm not found during Registration");
               this->Status.Error = Err_DeviceID_Not_Found;
               BuildPacket(NULL);
            }
         }
         else
         {
            this->Status.Error = Err_User_OverFlow;
         }
      }
      else
      {
         this->Status.Error = Err_Not_In_Register_Mode;
      }
   }
   __finally
   {
   }
}



void TPalm::InfoSync()
{
   // Build a packet with the palms real id in it.
   this->Status.TransactionNumber = 0;
   TComms::Instance().Process_Outbound->Clear();
   TDeviceRealTerminal::Instance().BuildInfoSyncPacket(TComms::Instance().Process_Outbound);
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
{
   TIRSeat *TempSeat;
   Table = 0;
   SelectedCourses.clear();
   Seats = new TList();

   long inbytes=1;
	Packet->ReadFileBuf(&Table,inbytes);

   if(Table & cIsLast)
   {
         Packet->LastTable = true;
   }

   Table &= 0x7F;
   if(Table > 99)
   {
      TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Invalid Table Number Received:" + IntToStr(Table));
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
                     AnsiString TextOut,Fo0,Fo1;
                     sprintf(out,"%02X ",CourseHi);
                     Fo0 = out;
                     Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
                     TextOut += Fo1 + " <- Course ID "
                                    + ( (Course & wIsLast) ? "LAST " : "");
                     TManagerLogs::Instance().Add(__FUNC__,"IR PACKET",TextOut);
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
                     TManagerLogs::Instance().Add(__FUNC__,"IR PACKET",TextOut);
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
                  AnsiString TextOut,Fo0,Fo1;
                  sprintf(out,"%02X ",Table);
                  Fo0 = out;
                  Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
                  TextOut += Fo1 + (Packet->LastTable ? " <- Last Table Number ":" <- Table Number ") ;
                  TManagerLogs::Instance().Add(__FUNC__,"IR PACKET",TextOut);
      #endif

      Packet->LastSeat = false;
      do
      {
         TempSeat = new TIRSeat(Packet);
         Seats->Add(TempSeat);
         if(Packet->ptrReadBuffer > (Packet->ptrBuffer + Packet->PktSize))
         {
            throw EHelixError("Internal Packet Overrun. Adding Seats.");
         }
      }
      while(!Packet->LastSeat);
   }
}

//---------------------------------------------------------------------------

_fastcall TIRTable::~TIRTable()
{
  TIRSeat *TempSeat;

  // this will remove All allocated ports.
  Seats->Pack();
  while(Seats->Count != 0)
  {
    TempSeat = (TIRSeat *)Seats->Items[0];
    delete (TIRSeat *)TempSeat;
    Seats->Delete(0);
  }
  delete (TList *)Seats;
}

//---------------------------------------------------------------------------

_fastcall TIRSeat::TIRSeat(TIRPacket *Packet)
{
   Seat = 0;
   long inbytes=1;
   Name = "";
   PalmTime = 0;
   Flags = NULL;
   SeatFlag = NULL;
   NameChange = false;

   Orders = new TList();

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
               AnsiString TextOut,Fo0,Fo1;
               sprintf(out,"%02X ",Seat);
               Fo0 = out;
               Fo1 = Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
               TextOut = Fo1 + (Packet->LastSeat ? " <- Last Seat Number ":" <- Seat Number ") ;
			      TManagerLogs::Instance().Add(__FUNC__,"IR PACKET",TextOut);

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
                  TManagerLogs::Instance().Add(__FUNC__,"IR PACKET",TextOut);
               }
   #endif

	Packet->LastItem = false;
   if(Packet->Header.Command == ORDERS)
   {
      // Loop for every used bit in the section flag.
      if(SeatFlag & ROSectionFlagContainsSeatName)      // Get in Seat Name.
      {
         //Retrieve Flags.
         inbytes=1;
         Packet->ReadFileBuf(&Flags,inbytes);
         #if (DEBUGLEVEL & debugExpandOrders )
                     char out[10];
                     ZeroMemory(out,10);
                     AnsiString TextOut,Fo0,Fo1;
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
                     TManagerLogs::Instance().Add(__FUNC__,"IR PACKET",TextOut);
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
                           AnsiString TextOut,Fo0,Fo1;
                           sprintf(out,"%02X ",input);
                           Fo0 = out;
                           Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
                           TextOut += Fo1 + " <- Seat Name Char " + (char)(input & 0x7F) + " Last";
                           TManagerLogs::Instance().Add(__FUNC__,"IR PACKET",TextOut);
               #endif
            }
            else
            {
               Name += (char)input;
               #if (DEBUGLEVEL & debugExpandOrders )
                           char out[10];
                           ZeroMemory(out,10);
                           AnsiString TextOut,Fo0,Fo1;
                           sprintf(out,"%02X ",input);
                           Fo0 = out;
                           Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
                           TextOut += Fo1 + " <- Seat Name Char " + (char)(input);
                           TManagerLogs::Instance().Add(__FUNC__,"IR PACKET",TextOut);
               #endif
            }
            if(Packet->ptrReadBuffer > (Packet->ptrBuffer + Packet->PktSize))
            {
               throw EHelixError("Internal Packet Overrun. Reading Name.");
            }
         }
      }

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
                        AnsiString TextOut,Fo0,Fo1;
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
                        TManagerLogs::Instance().Add(__FUNC__,"IR PACKET",TextOut);
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
                              AnsiString TextOut,Fo0,Fo1;
                              sprintf(out,"%02X ",ItemHi);
                              Fo0 = out;
                              Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
                              TextOut += Fo1 + " <- Time Byte";
                              TManagerLogs::Instance().Add(__FUNC__,"IR PACKET",TextOut);
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
                              TManagerLogs::Instance().Add(__FUNC__,"IR PACKET",TextOut);
                  #endif
               PalmTime = ((ItemHi*256) + ItemLo);
            }
         }
         while(!Packet->LastItem);
      }
   }
}

//---------------------------------------------------------------------------

_fastcall TIRSeat::~TIRSeat()
{
  TIROrder *CntOrder;
  // this will remove All allocated ports.
  Orders->Pack();
  while(Orders->Count != 0)
  {
    CntOrder = (TIROrder *)Orders->Items[0];
    delete (TIROrder *)CntOrder;
    Orders->Delete(0);
  }
  delete (TList *)Orders;
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
               AnsiString TextOut,Fo0,Fo1;
               sprintf(out,"%02X ",ItemHi);
               Fo0 = out;
               Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
               TextOut += Fo1 + " <- Item ID "
										+ ( (Item & wIsLast) ? "LAST " : "");
               TManagerLogs::Instance().Add(__FUNC__,"IR PACKET",TextOut);
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
               TManagerLogs::Instance().Add(__FUNC__,"IR PACKET",TextOut);
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
               TManagerLogs::Instance().Add(__FUNC__,"IR PACKET",TextOut);
               TextOut = "";
               Fo0 = "";
               Fo1 = "";
   #endif

   Container.push_back(GlassType);
   // Retrieve any Sub Items Drink Combonation.

   if(PromoItem && !Mixer)
   {
      inbytes=1;
// THIS IS WRONG IT SOULD BE THE MSB OF THE FLAG.
      Packet->ReadFileBuf(&SetMenuGroup,inbytes);
      if(SetMenuGroup & 0x80)
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
                  TextOut += Fo1 + " <- SetMenuGroup ID " + ((SetMenuGroup & 0x80) ? "Palm Generated" : "PC Generated");
                  TManagerLogs::Instance().Add(__FUNC__,"IR PACKET",TextOut);
                  TextOut = "";
                  Fo0 = "";
                  Fo1 = "";
      #endif
      SetMenuGroup &= 0x7F;
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
                     TManagerLogs::Instance().Add(__FUNC__,"IR PACKET",TextOut);
                     TextOut = "";
                     Fo0 = "";
                     Fo1 = "";

                     ZeroMemory(out,10);
                     sprintf(out,"%02X ",ItemLo);
                     Fo0 = out;
                     Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
                     TextOut += Fo1 + " <- Sub Item ID ";
                     TManagerLogs::Instance().Add(__FUNC__,"IR PACKET",TextOut);
                     TextOut = "";
                     Fo0 = "";
                     Fo1 = "";

                     ZeroMemory(out,10);
                     sprintf(out,"%02X ",GlassType);
                     Fo0 = out;
                     Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
                     TextOut += Fo1 + " <- Sub Size ID ";
                     TManagerLogs::Instance().Add(__FUNC__,"IR PACKET",TextOut);
                     TextOut = "";
                     Fo0 = "";
                     Fo1 = "";
         #endif

         Container.push_back(GlassType);
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
                  TManagerLogs::Instance().Add(__FUNC__,"IR PACKET",TextOut);
                  TextOut = "";
                  Fo0 = "";
                  Fo1 = "";
      #endif

      if(PromoItem)
      {
         inbytes=1;
         Packet->ReadFileBuf(&SetMenuGroup,inbytes);
         if(SetMenuGroup & 0x80)
         {
            SetMenuGroupIsIndex = false;
         }
         else
         {
            SetMenuGroupIsIndex = true;
         }
         SetMenuGroup &= 0x7F;
         #if (DEBUGLEVEL & debugExpandOrders )
                     ZeroMemory(out,10);
                     sprintf(out,"%02X ",SetMenuGroup);
                     Fo0 = out;
                     Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
                     TextOut += Fo1 + " <- SetMenuGroup ID " + ((SetMenuGroup & 0x80) ? "Palm Generated" : "PC Generated");
                     TManagerLogs::Instance().Add(__FUNC__,"IR PACKET",TextOut);
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
                     AnsiString TextOut,Fo0,Fo1;
                     ZeroMemory(out,10);
                     sprintf(out,"%02X ",Option);
                     Fo0 = out;
                     Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
                     TextOut += Fo1 + " <- Option ID "
                                    + ((IsLast(Option)) ? "LAST " : "");
                     TManagerLogs::Instance().Add(__FUNC__,"IR PACKET",TextOut);
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
                  AnsiString TextOut,Fo0,Fo1;
                  ZeroMemory(out,10);
                  sprintf(out,"%02X ",LOBYTE(HIWORD(Cents)) );
                  Fo0 = out;
                  Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
                  TextOut += Fo1 + " <- Price Adjust ";
                  TManagerLogs::Instance().Add(__FUNC__,"IR PACKET",TextOut);
                  TextOut = "";
                  Fo0 = "";
                  Fo1 = "";
                  ZeroMemory(out,10);
                  sprintf(out,"%02X ",HIBYTE(HIWORD(Cents)));
                  Fo0 = out;
                  Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
                  TextOut += Fo1 + " <- Price Adjust ";
                  TManagerLogs::Instance().Add(__FUNC__,"IR PACKET",TextOut);
                  TextOut = "";
                  Fo0 = "";
                  Fo1 = "";
                  ZeroMemory(out,10);
                  sprintf(out,"%02X ",LOBYTE(LOWORD(Cents)));
                  Fo0 = out;
                  Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
                  TextOut += Fo1 + " <- Price Adjust ";
                  TManagerLogs::Instance().Add(__FUNC__,"IR PACKET",TextOut);
                  TextOut = "";
                  Fo0 = "";
                  Fo1 = "";
                  ZeroMemory(out,10);
                  sprintf(out,"%02X ",HIBYTE(LOWORD(Cents)));
                  Fo0 = out;
                  Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
                  TextOut += Fo1 + " <- Price Adjust ";
                  TManagerLogs::Instance().Add(__FUNC__,"IR PACKET",TextOut);
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
                        AnsiString TextOut,Fo0,Fo1;
                        ZeroMemory(out,10);
                        sprintf(out,"%02X ",input);
                        Fo0 = out;
                        Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
                        TextOut += Fo1 + " <- Note LAST ";
                        TManagerLogs::Instance().Add(__FUNC__,"IR PACKET",TextOut);
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
                        AnsiString TextOut,Fo0,Fo1;
                        ZeroMemory(out,10);
                        sprintf(out,"%02X ",input);
                        Fo0 = out;
                        Fo1 += Fo0[Fo0.Length()-2];Fo1 += Fo0[Fo0.Length()-1];Fo1 += Fo0[Fo0.Length()];
                        TextOut += Fo1 + " <- Note ";
                        TManagerLogs::Instance().Add(__FUNC__,"IR PACKET",TextOut);
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
		TManagerLogs::Instance().Add(__FUNC__,"Inbound Header","CRC Size is " + IntToStr(CRC));
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
		TManagerLogs::Instance().Add(__FUNC__,"Inbound Header","CRC Size is " + IntToStr(CRC));
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
      TManagerLogs::Instance().Add(__FUNC__,"Inbound Header","Empty Order Recived.");
   }

   if(Header.Command < 0xE0 && Header.Command > 0xEA)
   {
      TManagerLogs::Instance().Add(__FUNC__,"Inbound Header","Message Not Reconised. Bad Command :" + IntToStr(Header.Command));
      Owner->Status.ErrorMsg = "Message Not Reconised. Bad Command :" + IntToStr(Header.Command);
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
   Header.Clear();
   Orders.Clear();
   Billing.Clear();
   CallAways.Clear();
   Login.Clear();

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
   TDBTables::Pack();
   while(TDBTables::Count != 0)
   {
    TempTable = (TIRTable *)TDBTables::Items[0];
    delete (TIRTable *)TempTable;
    TDBTables::Delete(0);
   }
   delete (TList *)Tables;
}


void TIRPacket::PktData::Clear()
{
//--------------------------------------
   TIRTable *TempTable;
   // this will remove All allocated Tables / Seats etc...
   TDBTables::Pack();
   while(TDBTables::Count != 0)
   {
      TempTable = (TIRTable *)TDBTables::Items[0];
      delete (TIRTable *)TempTable;
      TDBTables::Delete(0);
   }
}


void TIRPacket::PktHeader::Clear()
{
   Command =(MESSAGE_TYPE) NULL;
   TransNo = 0;
   InfoSyncNo = 0;
   MenuSync1No = 0;
   MenuSync2No = 0;
   StaffIDNo = 0;
   ErrorNo = 0;
   HighPktSize = 0;
   LowPktSize = 0;
   SectionFlag = NULL;
}


void TIRPacket::PktLogin::Clear()
{
   NewStaffID = 0;
   // Contains the Users Pin in the Case of a Login.
   UsersPIN = "";
   // Contains Name (Unique Palm Identifier in case of an InfoSync)
   DeviceName = "";
}





void TPalm::LogHexMessage(AnsiString MsgType ,char * ptrMessage, int PacketSize)
{
   char out[10];
   char *SentData = ptrMessage;
//         AnsiString Formatter = "%2.2x ";
   AnsiString TextOut,Fo0,Fo1;
   char arg;
   for (int i = 0; i < PacketSize; i++)
   {
      for(int j=0;j<10;j++)
      {
         out[j] = NULL;
      }
      arg = (char)(*SentData++);
      sprintf(out,"%02X ",arg);
      Fo0 = out;
      Fo1 += Fo0[Fo0.Length()-2];
      Fo1 += Fo0[Fo0.Length()-1];
      Fo1 += Fo0[Fo0.Length()];
      TextOut += Fo1;//Format(Formatter, arg,1);
      Fo1 = "";
   }
   TextOut = MsgType +" "+ TextOut;
   #if (DEBUGLEVEL & debugExpandOrders )
      TManagerLogs::Instance().Add(__FUNC__,"IR Hex",TextOut);
   #endif
   TManagerLogs::Instance().Add(__FUNC__,PALMLOG,TextOut);
}

TCallAwayComplete *TPalm::BuildCallAwayProper(TIRTable *TempTable)
{
	TCallAwayComplete *Return = NULL;
   try
   {
      Return = new TCallAwayComplete;
      Return->TableNo =  TempTable->Table;
      if (!frmData->IBTransaction->InTransaction) { frmData->IBTransaction->StartTransaction(); }
      // Retrive Table Data.
      frmData->IBQuery->Close();
      frmData->IBQuery->Params->Clear();
      frmData->IBQuery->ParamCheck = true;
      frmData->IBQuery->SQL->Text =
      " SELECT TABLE_NAME,PARTY_NAME FROM TABLES t "
      " WHERE "
      "  t.TABLE_NUMBER = :TABLE_NUMBER ";
      frmData->IBQuery->Params->ParamByName("TABLE_NUMBER")->AsInteger = TempTable->Table;
      frmData->IBQuery->Open();
      if(!frmData->IBQuery->IsEmpty())
      {
         Return->TableName = frmData->IBQuery->FieldByName("TABLE_NAME")->AsString;
         Return->PartyName = frmData->IBQuery->FieldByName("PARTY_NAME")->AsString;
      }

      // Retrive course Data.
      for(std::set<long>::iterator CrntCourseID = TempTable->SelectedCourses.begin(); CrntCourseID != TempTable->SelectedCourses.end(); advance(CrntCourseID,1))
      {
         TListCourse *Course = TDeviceRealTerminal::Instance().Menus.MenuFood->FetchCourseByID(*CrntCourseID);
         if(Course)
         {
            Return->Courses->Add(Course->Course_Name);
         }
         else
         {
            TListCourse *Course = TDeviceRealTerminal::Instance().Menus.MenuDrinks->FetchCourseByID(*CrntCourseID);
            if(Course)
            {
               Return->Courses->Add(Course->Course_Name);
            }
         }
      }
      if (frmData->IBTransaction->InTransaction) { frmData->IBTransaction->Commit(); }
   }
   catch(Exception &Err)
   {
   	delete Return;
      TManagerLogs::Instance().Add(__FUNC__,"Exception",Err.Message);
      throw;
   }
   return Return;
}

TItemComplete *TPalm::BuildOrderProper(TIROrder * TempOrder,TDateTime OrderTimeStamp)
{
	TItemComplete *Order = NULL;
   try
   {
      static long LastItemID;
      Order = new TItemComplete;
      Order->Terminal       = this->ID.Name;
      Order->TabType        = TabTableSeat;
      Order->Order_Key      = TempOrder->OrderLK;
      Order->OrderType      = TempOrder->Status;
      Order->PrintChitNumber = false;
      Order->SetMenu			 = TempOrder->PromoItem;
      Order->Note           = TempOrder->Note;
      Order->HappyHour		= false;
      Order->TimeStamp 			= OrderTimeStamp;

      // Retrieve ITEM INFO.
      long ID              = TempOrder->Items[0];
      TItem * CurrentItem  = TDeviceRealTerminal::Instance().Menus.FetchItemByID(ID);
      if(CurrentItem == NULL)
      {
         delete Order;
	      TManagerLogs::Instance().Add(__FUNC__,"Exception","Error Item not found by ID");
			return NULL;
      }
      Order->Palm_ID       = ID;
      Order->Course        = CurrentItem->Course;
      Order->MenuName		= CurrentItem->MenuName;
      Order->MenuKey		= CurrentItem->MenuKey;
      Order->MenuType		= CurrentItem->MenuType;
      Order->Item		      = CurrentItem->Item;
      Order->Item_ID	      = CurrentItem->Item_ID;

      if(TempOrder->Container.size() > 0)
      {
         TItemSize *Size = CurrentItem->Sizes->SizeGetByPalmID(TempOrder->Container[0]);
         Order->Price         = Size->Price;
         Order->GSTPercent    = Size->GSTPercent;
         Order->Size          = Size->Name;
         Order->SetMenuMask	= Size->SetMenuMask;
         Order->SetMenu			= Size->SetMenuItem;
         Order->SetMenuMaster	= Size->SetMenuMaster;
         Order->PointsPercent = Size->PointsPercent;
			Order->ThirdPartyKey	= Size->ThirdPartyKey;

         // Sort Categories
         Order->Categories->CategoryCopyList(CurrentItem->Sizes->SizeGetByPalmID(TempOrder->Container[0])->Categories);

         if(Size->Recipes->Count != 0)
         {
            Order->SalesRecipesToApply->RecipeCopyList(Size->Recipes);
				double CostIncGst = 0;
				double OrderCost = 0;
            for(int i = 0; i < Size->Recipes->Count; i++)
            {
               OrderCost += double(Size->Recipes->RecipeGet(i)->Cost);
					CostIncGst += double(Size->Recipes->RecipeGet(i)->Cost) *
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
	      TManagerLogs::Instance().Add(__FUNC__,"Exception","Error Item has no sizes");
			return NULL;
      }

      // Palm now sends the New Price in the Price Adjust field.
      if (TempOrder->IsPriceAdjust)
      {
         Order->PriceAdjust = TempOrder->PriceAdjust - Order->Price;
      }

      // sort sub orders.
      for(unsigned int i = 1;i < TempOrder->Items.size(); i++)
      {
         TItem * SubItem = TDeviceRealTerminal::Instance().Menus.FetchItemByID(TempOrder->Items[i]);
			if(SubItem)
         {
            TItemSize *Size = SubItem->Sizes->SizeGetByPalmID(TempOrder->Container[i]);
            if(Size)
            {
               TItemCompleteSub *CurrentSubOrder = new TItemCompleteSub;
               CurrentSubOrder->PriceAdjust = 0;
               CurrentSubOrder->Item = SubItem->Item;
               CurrentSubOrder->Item_ID = SubItem->Item_ID;
               CurrentSubOrder->Course = SubItem->Course;

               CurrentSubOrder->Security->SetSecurityRefNumber(TDeviceRealTerminal::Instance().GetNextSecurityRef());

               TSecurityReference   *SecRef = new TSecurityReference;
               SecRef->UserKey      = this->User.ContactKey;
               SecRef->Event        = SecurityTypes[secOrderedBy];
               SecRef->From         = this->User.Name;
               SecRef->To           = this->User.Initials;
               SecRef->TimeStamp    = Order->TimeStamp;
               CurrentSubOrder->Security->SecurityAdd(SecRef);

               CurrentSubOrder->Price         = Size->Price;
               CurrentSubOrder->Size          = Size->Name;
               CurrentSubOrder->GSTPercent    = Size->GSTPercent;
		         CurrentSubOrder->PointsPercent = Size->PointsPercent;
					CurrentSubOrder->ThirdPartyKey = Size->ThirdPartyKey;
               // Sort Categories
               CurrentSubOrder->Categories->CategoryCopyList(Size->Categories);


               if(Size->Recipes->Count != 0)
               {
                  Order->SalesRecipesToApply->RecipeCopyList(Size->Recipes);
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
   	delete Order;
      TManagerLogs::Instance().Add(__FUNC__,"Exception",Err.Message);
      throw;
   }
}

void TDeviceRealPalms::CheckTabsSetMenuList()
{
   bool StartTrans = false;
   try
   {
      if (!IBTransaction->InTransaction) {IBTransaction->StartTransaction(); StartTrans = true; }


		std::set<long> TabsToRemove;

      for(TTabsSetMenuList::iterator Tablist = TDeviceRealTerminal::Instance().TabsSetMenuList.begin();
          Tablist != TDeviceRealTerminal::Instance().TabsSetMenuList.end();
          advance(Tablist,1))
      {
         IBQuery->Close();
         IBQuery->SQL->Text =
         " SELECT TAB.TAB_KEY "
         " FROM TAB "
         " WHERE "
         "  TAB.TAB_KEY = :Loc_TAB_KEY";
         IBQuery->Params->ParamByName("Loc_TAB_KEY")->AsInteger = Tablist->first;
         IBQuery->Open();
         if(IBQuery->IsEmpty())
         { // Ok Remove this TAB.
            TabsToRemove.insert(Tablist->first);
         }
      }

      for (std::set<long>::iterator Removelist = TabsToRemove.begin();
           Removelist != TabsToRemove.end(); advance(Removelist,1))
      {
         TDeviceRealTerminal::Instance().TabsSetMenuList.erase(*Removelist);
      }

      if (IBTransaction->InTransaction && StartTrans) { IBTransaction->Commit(); }
   }
   catch(Exception &E)
   {
      if (IBTransaction->InTransaction && StartTrans) { IBTransaction->Rollback(); }
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
      Packet->Header.StaffIDNo = *ptrStream++;
      Packet->Header.ErrorNo = *ptrStream++;
      Packet->Header.HighPktSize = *ptrStream++;
      Packet->Header.LowPktSize = *ptrStream++;

      Packet->Process(ptrStream);

      if(Status.Error != proA_Ok)
      {

         AnsiString msgType = "";
         switch(Packet->Header.Command)
         {
            case KEEPALIVE : msgType = "KEEPALIVE";
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
            case REQUEST_RECEIPT : msgType = "REQUEST_RECEIPT";
            break;
            case TRANSFER_TABLE_ORDERS : msgType = "TRANSFER_TABLE_ORDERS";
            break;
            case RETRIEVE_ORDERS : msgType = "RETRIEVE_ORDERS";
            break;
            case REGISTRATION :  msgType = "REGISTRATION";
            break;
            case CALL_AWAY :  msgType = "CALL AWAY";
            break;
            default : msgType = "UNKNOWN";
            break;
         }

         TManagerLogs::Instance().Add(__FUNC__,"Inbound Header","-------------- FAILED PALM PACKET FAILED STATUS-------------","Palm #" + IntToStr(ID.DeviceID));
         TManagerLogs::Instance().Add(__FUNC__,"Inbound Header",msgType,"Palm #" + IntToStr(ID.DeviceID));
         TManagerLogs::Instance().Add(__FUNC__,"Inbound Header","CommandNo : " + IntToStr(Packet->Header.Command),"Palm #" + IntToStr(ID.DeviceID));
         TManagerLogs::Instance().Add(__FUNC__,"Inbound Header","TransNo : " + IntToStr(Packet->Header.TransNo),"Palm #" + IntToStr(ID.DeviceID));
         TManagerLogs::Instance().Add(__FUNC__,"Inbound Header","InfoSyncNo : " + IntToStr(Packet->Header.InfoSyncNo),"Palm #" + IntToStr(ID.DeviceID));
         TManagerLogs::Instance().Add(__FUNC__,"Inbound Header","MenuSync1No : " + IntToStr(Packet->Header.MenuSync1No),"Palm #" + IntToStr(ID.DeviceID));
         TManagerLogs::Instance().Add(__FUNC__,"Inbound Header","MenuSync2No : " + IntToStr(Packet->Header.MenuSync2No),"Palm #" + IntToStr(ID.DeviceID));
         TManagerLogs::Instance().Add(__FUNC__,"Inbound Header","Staff ID No : " + IntToStr(Packet->Header.StaffIDNo),"Palm #" + IntToStr(ID.DeviceID));
         TManagerLogs::Instance().Add(__FUNC__,"Inbound Header","Packet Size " + IntToStr(Packet->PktSize),"Palm #" + IntToStr(ID.DeviceID));
         TManagerLogs::Instance().Add(__FUNC__,"Inbound Header","ERROR NO : " + IntToStr(Packet->Header.ErrorNo),"Palm #" + IntToStr(ID.DeviceID));
         TManagerLogs::Instance().Add(__FUNC__,"Inbound Header","------------------------------------------------------------","Palm #" + IntToStr(ID.DeviceID));
         TManagerLogs::Instance().Add(__FUNC__,"Inbound Header","Was Attempting : " + msgType + " # " + IntToStr(Packet->Header.Command),"Palm #" + IntToStr(ID.DeviceID));
         TManagerLogs::Instance().Add(__FUNC__,"Inbound Header","------------------------------------------------------------","Palm #" + IntToStr(ID.DeviceID));
         TManagerLogs::Instance().Add(__FUNC__,"Inbound Header","Returned empty packet with Error Message to palm.","Palm #" + IntToStr(ID.DeviceID));
         BuildPacket(NULL);
         ClearInbound();

         TransNo = Packet->Header.TransNo;
      }
      else
      {
         AnsiString msgType = "";
         switch(Packet->Header.Command)
         {
            case KEEPALIVE : msgType = "KEEPALIVE";
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
            case REQUEST_RECEIPT : msgType = "REQUEST_RECEIPT";
            break;
            case TRANSFER_TABLE_ORDERS : msgType = "TRANSFER_TABLE_ORDERS";
            break;
            case RETRIEVE_ORDERS : msgType = "RETRIEVE_ORDERS";
            break;
            case REGISTRATION :  msgType = "REGISTRATION";
            break;
            case CALL_AWAY :  msgType = "CALL AWAY";
            break;
            default : msgType = "UNKNOWN";
            break;
         }

         TManagerLogs::Instance().Add(__FUNC__,"Inbound Header","-------------- PALM PACKET STATUS----------------","Palm #" + IntToStr(ID.DeviceID));
         TManagerLogs::Instance().Add(__FUNC__,"Inbound Header",msgType,"Palm #" + IntToStr(ID.DeviceID));
         TManagerLogs::Instance().Add(__FUNC__,"Inbound Header","CommandNo : " + IntToStr(Packet->Header.Command),"Palm #" + IntToStr(ID.DeviceID));
         TManagerLogs::Instance().Add(__FUNC__,"Inbound Header","TransNo : " + IntToStr(Packet->Header.TransNo),"Palm #" + IntToStr(ID.DeviceID));
         TManagerLogs::Instance().Add(__FUNC__,"Inbound Header","InfoSyncNo : " + IntToStr(Packet->Header.InfoSyncNo),"Palm #" + IntToStr(ID.DeviceID));
         TManagerLogs::Instance().Add(__FUNC__,"Inbound Header","MenuSync1No : " + IntToStr(Packet->Header.MenuSync1No),"Palm #" + IntToStr(ID.DeviceID));
         TManagerLogs::Instance().Add(__FUNC__,"Inbound Header","MenuSync2No : " + IntToStr(Packet->Header.MenuSync2No),"Palm #" + IntToStr(ID.DeviceID));
         TManagerLogs::Instance().Add(__FUNC__,"Inbound Header","Staff ID No : " + IntToStr(Packet->Header.StaffIDNo),"Palm #" + IntToStr(ID.DeviceID));
         TManagerLogs::Instance().Add(__FUNC__,"Inbound Header","Error No : " + IntToStr(Packet->Header.ErrorNo),"Palm #" + IntToStr(ID.DeviceID));
         TManagerLogs::Instance().Add(__FUNC__,"Inbound Header","------------------------------------------------------------","Palm #" + IntToStr(ID.DeviceID));
         TManagerLogs::Instance().Add(__FUNC__,"Inbound Header","ATTEMPTING : " + msgType,"Palm #" + IntToStr(ID.DeviceID));
         TManagerLogs::Instance().Add(__FUNC__,"Inbound Header","------------------------------------------------------------","Palm #" + IntToStr(ID.DeviceID));

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
                  Register();
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
            case ORDERS :
            {
               double DelayProc = GetTickCount();
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
                           TempTable = new TIRTable(Packet,Packet->Header.Command);
                           Packet->Orders.TDBTables::Add(TempTable);
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
                           UpdateTransactionNumber();
                           NewOrders();
                        }
                     }
                     catch(EHelixError &Err)
                     {
                          Status.ErrorMsg = Err.ErrMsg + " ORDERS";
                          Status.Error = Err_Currupt_Message;
                          LogHexMessage("EHelixError",Packet->ptrBuffer,Packet->PktSize);
                     }
                  }
               }
               __finally
               {
                  BuildPacket(NULL);
                  ClearInbound();
               }
               break;
            }
            case MENU_SYNC :
            {
               try
               {
                  if(Packet->PktSize == 0)
                  {
                     if(TDeviceRealTerminal::Instance().Menus.MenuFood != NULL)
                     {
                        TComms::Instance().Process_Outbound->Clear();
                        TComms::Instance().Process_Outbound->CopyFrom(TDeviceRealTerminal::Instance().Menus.MenuUploadFood,0);
                        // Add it to the Palms Transmission list.
                        BuildPacket(TComms::Instance().Process_Outbound);
                        UpdateNames();
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
                     if(TDeviceRealTerminal::Instance().Menus.MenuDrinks != NULL)
                     {
                        TComms::Instance().Process_Outbound->Clear();
                        TComms::Instance().Process_Outbound->CopyFrom(TDeviceRealTerminal::Instance().Menus.MenuUploadDrinks,0);
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
                     unsigned char PalmDeviceNameSize = (unsigned char)*Packet->ptrReadBuffer++;
                     Packet->Login.DeviceName = AnsiString((char *)Packet->ptrReadBuffer,PalmDeviceNameSize);
                     Packet->ptrReadBuffer += PalmDeviceNameSize;

                     Packet->Login.NewStaffID = *Packet->ptrReadBuffer++;

                     Packet->Login.UsersPIN = "";
                     char PIN[PIN_Length+1];
                     PIN[PIN_Length] = NULL;
                     for(int i = 0 ; i < PIN_Length; i++)
                     {
                         PIN[i]= (char)*Packet->ptrReadBuffer++;
                     }
                     Packet->Login.UsersPIN = PIN;
                     Login();
                  }
                  else
                  {
                     Status.Error = Err_Mismatched_Message_Data;
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
                  if(Packet->PktSize == 0)
                  {
                     InfoSync();
                  }
                  else
                  {
                     Status.Error = Err_Mismatched_Message_Data;
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
                           Packet->CallAways.TDBTables::Add(TempTable);
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
                           CallAways();
                        }
                     }
                     else
                     {
                        Status.Error = Err_Mismatched_Message_Data;
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
                     #if (DEBUGLEVEL & debugExpandPalmComms )
                        TManagerLogs::Instance().Add(__FUNC__,"REQUEST_RECEIPT","REQUEST_RECEIPT MESSAGE");
                     #endif
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
                           Packet->Billing.TDBTables::Add(TempTable);
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
                           BillSeats();
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
                     BuildPacket(NULL);                     
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
               long DelayProc = GetTickCount();
               try
               {
                  if(Packet->PktSize != 0)
                  {
                     DownloadOrders =(DownloadStatus)Packet->ptrReadBuffer[0];
                     CurrentTable = Packet->ptrReadBuffer[1];
                     // Packet was empty so check the checksum.
                     UpdateTransactionNumber();
                     TComms::Instance().Process_Outbound->Clear();
                     RepackageOrders(TComms::Instance().Process_Outbound,0);
                     BuildPacket(TComms::Instance().Process_Outbound);
                  }
                  else
                  {
                     UpdateTransactionNumber();
                     TComms::Instance().Process_Outbound->Clear();
                     RepackageOrders(TComms::Instance().Process_Outbound,0);
                     BuildPacket(TComms::Instance().Process_Outbound);
                  }
               }
               __finally
               {
                  if(Status.Error != proA_Ok)
                  {
                     BuildPacket(NULL);
                  }
                  ClearInbound();
                  long TickCount = GetTickCount();
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


void TDeviceRealPalms::Initialise()
{
	TDeviceRealControl::Initialise();
}
