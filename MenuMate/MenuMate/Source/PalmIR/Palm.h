//---------------------------------------------------------------------------
#ifndef PalmH
#define PalmH

#include "enum.h"       // Messaging Types.
#include <set>
#include <vector>

#ifdef PalmMate

#include "PalmMenus.h"
#define PalmNotFoundID 255

class TIRTable;
class TIROrder;
class TIRPacket;  // Packet sent from palms.
class TPalm;
class TCallAwayComplete;

enum PalmFeatures {PriceAdjust,Notes,Billing,DefaultLogin,AutoPopup,LargeFont};

typedef std::vector<long> TSetMenuList; // PalmKey(POS in List) / DB Key
typedef std::map<long,TSetMenuList> TTabsSetMenuList; // TabKey / SetMenus Group No.

//---------------------------------------------------------------------------
// Holds the list of the Palm objects we need to send information to.
// A Plam object will contain a Palm ID and Palm Data to be sent.
class TDeviceRealPalms : public TDeviceRealControl
{
public:
   // returns a New ID for a Palm.
   int RetrieveNewPalmID();
   TPalm * PalmNotFound;
   TPalm * PalmRegister;
   TList *PalmList;

   bool Registering;
   bool Reregistering;
   int  ReregisteringOldPalmID;
   int RegisteringTimer;
   AnsiString NewPalmRegisterName;
	__fastcall TDeviceRealPalms();
   __fastcall ~TDeviceRealPalms();
 	// retrives a transmission for a Palm if any (else returns Null).
   TPalm * FindTPalm(unsigned char ID);
   TPalmMenus Menus;
   void FreePalm(AnsiString Name);
   void SavePalms();
   void LoadPalms();
   void SendMessage(AnsiString From, AnsiString Msg);
   void UpdateCurrentRecipes(AnsiString Code,AnsiString Location,
	     AnsiString Unit, AnsiString Desc, Currency Cost,double CostGSTPercent);
   void __fastcall BuildInfoSyncPacket(TMemoryStream *Process_Outbound);

   TTabsSetMenuList TabsSetMenuList;
   void CheckTabsSetMenuList();
	void Initialise();
};


//---------------------------------------------------------------------------

class TPalmSeatName
{
public:
   int TableNumber;
   int SeatNumber;
   bool DeleteMe;
   AnsiString SeatName;
	__fastcall TPalmSeatName(  int inTableNumber,
                              int inSeatNumber,
                              AnsiString inSeatName);
   __fastcall ~TPalmSeatName();
};

//---------------------------------------------------------------------------

class TPalm : public TDeviceReal
{
   long fNonFatialMenuEnableDisableNumber;
   long fNonFatialDrinksEnableDisableNumber;
public:
	void DumpStatus(TStringList *Debug);
   MESSAGE_TYPE LastTransmissionType;
   EProcessingErrorType LastErrorNumber;

	__fastcall TPalm();
   __fastcall ~TPalm();

   TList *SeatNames;

   // Stores inbound IR Packet for this palm.
   unsigned char *ptrRawData;
   TIRPacket *Inbound;
   // Stores outbound IR packet for this palm.
   TMemoryStream *Outbound;

   int  LastConnectVar;

   // Called in message by Primary thread.
   void UpdateTransactionNumber();
   void ClearInbound();
	// Unique Palm Identifer.
   AnsiString From; // Message to be sent to palm.
   AnsiString Message; // Message to be sent to palm.

   bool getMenuEnabledStateChange();
   void setMenuEnableState();
   void resetMenuEnableState();
   std::set<long> CurrentTables;
   unsigned char CurrentTable;
   unsigned char TransNo;
   int LogoffTime;
   DownloadStatus DownloadOrders; // ThisCurrentTable,ActiveTables,Full
   bool AutoPopup;
   bool LargeFont;
   bool AdvanceUser;
   // Set to true when Palm device is removed in Maintainence form.
   // The secondary thread will remove the palm when it is ready
   // because it could be taking to it when it is removed from
   // Maintainence.
   bool RemoveFromPalmList;
	void BuildPacket(TMemoryStream *Process_Outbound,MESSAGE_TYPE MsgType = NA);

   TPalmSeatName* GetSeatName(int inTableNumber, int inSeatNumber);
   void TPalm::ClearSeatNames();
   void UpdateNames();
   void RemoveSeatName(TPalmSeatName * ptrName);
   void LogHexMessage(AnsiString MsgType, char * ptrMessage, int PacketSize);
//---------------------------

	void __fastcall NewOrders();
   void __fastcall RepackageOrders(TMemoryStream *Process_Outbound, int LastConnectVar);
   bool __fastcall CheckCRC(unsigned short int CRC);
   void __fastcall TPalm::Login();
   bool CheckUser();
   bool __fastcall CheckSyncNo();
   void InfoSync();
	void Register();
    bool getDrinksEnabledStateChange();
   TItemComplete *BuildOrderProper(TIROrder * TempOrder,TDateTime OrderTimeStamp);
   TCallAwayComplete *BuildCallAwayProper(TIRTable *TempTable);   
   TReturnItemComplete * BuildReturnProper(TIBDataSet *OrderTable,TIBDataSet *OptionsTable,TIBDataSet *SubOrdersTable);
   void ProcessTransmission();
   void CallAways();
	void BillSeats();   
};

//------------------------------------------------------------------------------
//------------------------ Menu Order Storage classes that conatian Orders -----
//------------------------------------------------------------------------------
class TIRPacket
{
   private:
   TPalm *Owner;
   public:
   __fastcall TIRPacket(TPalm *InOwner);
   __fastcall ~TIRPacket();

   struct PktHeader
   {
   public:
      MESSAGE_TYPE  Command;
      unsigned char TransNo;
      unsigned char InfoSyncNo,MenuSync1No,MenuSync2No,StaffIDNo;
      unsigned char ErrorNo;
      unsigned char HighPktSize, LowPktSize;
   	unsigned char SectionFlag;			//	Bit 7 = Last flags byte
	   											//	Bit 6 = NA
		   										//	Bit 5 = NA
			   									//	Bit 4 = NA
                                       // Bit 3 = Contains Order Data.
                                       // Bit 2 = NA
                                       // Bit 1 = Contains Billing Data.
                                       // Bit 0 = NA
      void Clear();
   }Header;

   struct PktLogin
   {
   public:
      unsigned char NewStaffID;
      // Contains the Users Pin in the Case of a Login.
      AnsiString UsersPIN;
      // Contains Name (Unique Palm Identifier in case of an InfoSync or Login)
      AnsiString DeviceName;
      void Clear();
   }Login;

   bool Valid; // Contains Valid Data. Used by Palm->Process.

   enum BillingSection {bsOrders,bsBilling,bsCallAways};
   BillingSection Section;
   class PktData
   {
   public:
      PktData();
      ~PktData();

      // List that Holds tables when required.
      TList *Tables;
      // Application Layer High Level variables used for data decodeing of a packet.
      void Clear();
   }Orders,Billing,CallAways;

   bool LastItem;
   bool LastTable;
   bool LastSeat;

   ePaymentType FinalBill;

	// pointers used in "Helix order" sorting.
	unsigned char *ptrBuffer;
	unsigned char *ptrReadBuffer;
   unsigned char PktStart;
   int PktSize;
   long OrderCount;
	void Process(unsigned char* &ptrStream);
   void ReadFileBuf(void *Dest,int inbytes);
   void Clear();
};

//------------------------------------------------------------------------------

class TIRTable
{
   public:
   __fastcall TIRTable(TIRPacket *Packet,unsigned char Command);
   __fastcall ~TIRTable();
   unsigned char Table;
   std::set<long> SelectedCourses;
   TList *Seats;
};

//------------------------------------------------------------------------------

class TIRSeat
{
   public:
   __fastcall TIRSeat(TIRPacket *Packet);
   __fastcall ~TIRSeat();
   unsigned char Seat;
   unsigned short int PalmTime;
   unsigned char Flags;
   unsigned char SeatFlag;
   AnsiString Name;
   bool NameChange;
   TList *Orders;
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


class TIROrder
{
   public:
   __fastcall TIROrder(TIRPacket *Packet, unsigned char Flags);
   __fastcall ~TIROrder();
   unsigned char MasterContainer;
   double PriceAdjust;
   // Set when customer's having an entire dish credited.
   OrderStatus Status;
   // Is this order a Drink.
   bool Drink;
   AnsiString Note;

   std::vector<long> Options;
   std::vector<long> Items;
   std::vector<long> Container;

   bool DrinkIDCount;
   bool Mixer;
   bool PromoItem;
   bool SetMenuGroupIsIndex; // One of My Numbers
   bool IsPriceAdjust;
   unsigned char SetMenuGroup;
   unsigned char SplitCount;
   long PalmSplitLK;
   long OrderLK; // Used in New Order().
};


#endif
#endif
