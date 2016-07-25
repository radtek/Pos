//---------------------------------------------------------------------------
#ifndef PalmH
#define PalmH

#include <set>
#include <vector>

#include "enum.h"       // Messaging Types.

#ifdef PalmMate

#include "PalmMenus.h"
#include "bluetooth.h"
#include "DevicePalm.h"
#include "DeviceRealControl.h"

// See __fastcall TPalms::TPalms() for missing palm routines.
#define PalmNotFoundID 0

class TIRTable;
class TIROrder;
class TIRPacket;  // Packet sent from palms.
class TPalm;
class TCallAwayComplete;

enum PalmFeatures {PriceAdjust,Notes,Billing,DefaultLogin,AutoPopup,LargeFont};

typedef std::vector<long> TSetMenuList; // PalmKey(POS in List) / DB Key
typedef std::map<long,TSetMenuList> TTabsSetMenuList; // TabKey / SetMenus Group No.

//---------------------------------------------------------------------------
extern bool LogGetOrders; // Global (ick!) set from PalmSetup
//---------------------------------------------------------------------------
// Holds the list of the Palm objects we need to send information to.
// A Plam object will contain a Palm ID and Palm Data to be sent.

class TDeviceRealPalms : public TDeviceRealControl
{
public:
   // returns a New ID for a Palm.
   int RetrieveNewPalmID();
   TBluetoothLayer *BlueTooth;
   TPalm * PalmNotFound;
	TList *PalmList;

	static TDeviceRealPalms& Instance() {
        static TDeviceRealPalms singleton;
        return singleton;
    }

	__fastcall TDeviceRealPalms();
   __fastcall ~TDeviceRealPalms();
 	// retrives a transmission for a Palm if any (else returns Null).
	TPalm* FindTPalm(Database::TDBTransaction &DBTransaction,unsigned short ID);
	TPalm* FindTPalm(Database::TDBTransaction &DBTransaction,UnicodeString DeviceIP);
	TPalm* FindTPalmByName(Database::TDBTransaction &DBTransaction,UnicodeString DeviceName);

	__property TManagerMenusPalm *Menus = { read=GetMenuManager };

   UnicodeString RegistrationPIN;
   void FreePalm(UnicodeString Name);
   void SavePalms(Database::TDBTransaction &DBTransaction);
   void LoadPalms(Database::TDBTransaction &DBTransaction);
   void UpdateCurrentRecipes(UnicodeString Code,UnicodeString Location,Currency Cost,double CostGSTPercent);
	void UpdateMessage(TNetMessageMsg * Request);		  
   void __fastcall BuildInfoSyncPacket(Database::TDBTransaction &DBTransaction,TMemoryStream *Process_Outbound);

   TTabsSetMenuList TabsSetMenuList;
   void CheckTabsSetMenuList(Database::TDBTransaction &DBTransaction);
	void Initialise(Database::TDBTransaction &DBTransaction);
	void SyncPrintingTo(Database::TDBTransaction &DBTransaction,TPalm * PalmSource,TPalm * PalmDest);
	TTimer *RegExpire;
	void __fastcall RegExpireTimer(TObject *Sender);
	int RegSyncNumber; // Updates PalmListDisplay.
	void CreateDevice(Database::TDBTransaction &DBTransaction);			
};

/* Stub Class for Palms */
class TDeviceRealTerminal
{
	public :
	static TDeviceRealPalms& Instance() {
        return TDeviceRealPalms::Instance();
    }
};

//---------------------------------------------------------------------------
/*struct TPalmSeatName
{
	TPalmSeatName(int inTableNumber,	int inSeatNumber,	UnicodeString inSeatName);
	~TPalmSeatName();

	int			TableNumber;
	int			SeatNumber;
	UnicodeString	SeatName;
//	bool	DeleteMe;
};*/
//---------------------------------------------------------------------------
struct TPalmSeatName
{
	TPalmSeatName() : TableNumber(0), SeatNumber(0) {}
	TPalmSeatName(int Table, int Seat, UnicodeString Name) : TableNumber(Table), SeatNumber(Seat), SeatName(Name) {}

	int			TableNumber;
	int			SeatNumber;
	UnicodeString	SeatName;

	bool operator < (const TPalmSeatName& PalmSeat) const
	{
		if (TableNumber < PalmSeat.TableNumber) return true;
		else if (TableNumber == PalmSeat.TableNumber) return (SeatNumber < PalmSeat.SeatNumber);
		else return false;
	}
};
//---------------------------------------------------------------------------
class TPalmMessage
{
public:
	UnicodeString From; // Message to be sent to palm.
	UnicodeString Message; // Message to be sent to palm.
	TDateTime Received;
	bool Sent;

	TPalmMessage(UnicodeString inFrom, UnicodeString inMessage, TDateTime inReceived);
};

typedef std::vector<TPalmMessage> TListPalmMessage;
//---------------------------------------------------------------------------
typedef unsigned char TTableNumber;
typedef unsigned char TSeatNumber;
//---------------------------------------------------------------------------
struct TPalmSeat
{
	TPalmSeat() : SeatNumber(0)	{}
	TPalmSeat(int Seat, UnicodeString Name) : SeatNumber(Seat), SeatName(Name) {}

	TSeatNumber		SeatNumber;
	UnicodeString		SeatName;
};
//---------------------------------------------------------------------------
class TPalmTable
{
public:
	TPalmTable() : TableNumber(0)						{ }
	TPalmTable(int Table) : TableNumber(Table)	{ }

	void	AddSeat(const TPalmSeat &PalmSeat)		{ fSeats.push_back(PalmSeat); }

	TTableNumber	TableNumber;
	int				SeatCount()							{ return fSeats.size(); }
	TPalmSeat		&Seat(int Index)					{ return fSeats[Index]; }

private:
//	friend class TPalmSeatNames;
	std::vector<TPalmSeat> fSeats;
};
//---------------------------------------------------------------------------
class TPalmSeatNames
{
public:
	void	Clear()											{ fTables.clear(); }
	void	Load(Database::TDBTransaction &DBTransaction,int TableNumber=-1);
	void	Save(Database::TDBTransaction &DBTransaction);

	void	AddTable(const TPalmTable &PalmTable)	{ fTables.push_back(PalmTable); }

//	int	NamesCount();
	int	TableCount()									{ return fTables.size(); }
	TPalmTable &Table(int Index)						{ return fTables[Index]; }

private:
	std::vector<TPalmTable>								fTables;
};
//---------------------------------------------------------------------------
class TPalm : public TDevicePalm
{
	long fNonFatialMenuEnableDisableNumber;
	long fNonFatialDrinksEnableDisableNumber;
	void CheckLocation(Database::TDBTransaction & DBTransaction){};
public:
//	void DumpStatus(TStringList *Debug);
	MESSAGE_TYPE LastTransmissionType;
	EProcessingErrorType LastErrorNumber;

	__fastcall TPalm();
	__fastcall ~TPalm();

//	TList *SeatNames;

//	TPalmSeatNames SeatNames;

	TListPalmMessage Messages;
	// Stores inbound IR Packet for this palm.
	unsigned char *ptrRawData;
	TIRPacket *Inbound;
	// Stores outbound IR packet for this palm.
	TMemoryStream *Outbound;

	int  LastConnectVar;

	// Called in message by Primary thread.
	void UpdateTransactionNumber(Database::TDBTransaction &DBTransaction);
	void ClearInbound();
	// Unique Palm Identifer.

   bool getMenuEnabledStateChange();
   void setMenuEnableState();
   void resetMenuEnableState();
//   std::set<long> CurrentTables;
   unsigned char CurrentTable;
   unsigned char TransNo;

//	DownloadStatus		DownloadOrders; // ThisCurrentTable,ActiveTables,Full - now in user

//	TPalmUserSettings	UserSettings;	// these are in User.UserSettings

//	int				LogoffTime; 		// this is in User
//	bool				AutoPopup;
//	bool				LargeFont;
//	bool				AdvanceUser;

	bool Delete;
   // Set to true when Palm device is removed in Maintainence form.
   // The secondary thread will remove the palm when it is ready
   // because it could be taking to it when it is removed from
   // Maintainence.
	void BuildPacket(TMemoryStream *Process_Outbound,MESSAGE_TYPE MsgType = NA);

//   bool GetSeatName(int TableNumber, int SeatNumber, TPalmSeatName &PalmSeatName);
//   void TPalm::ClearSeatNames();
//	void UpdateNames();
//   void RemoveSeatName(TPalmSeatName * ptrName);
	void LogHexMessage(UnicodeString MsgType, unsigned char *ptrMessage, int PacketSize);
//---------------------------

	void __fastcall NewOrders(Database::TDBTransaction &DBTransaction);
	void __fastcall RepackageOrders(Database::TDBTransaction &DBTransaction,TMemoryStream *Process_Outbound, int LastConnectVar);
	bool __fastcall CheckCRC(unsigned short int CRC);
	void __fastcall TPalm::Login(Database::TDBTransaction &DBTransaction);
	bool CheckUser();
	bool __fastcall CheckSyncNo();
	void InfoSync(Database::TDBTransaction &DBTransaction);
	void Register(Database::TDBTransaction &DBTransaction);
    bool getDrinksEnabledStateChange();
	TItemComplete *BuildOrderProper(TIROrder * TempOrder,TDateTime OrderTimeStamp);
	TCallAwayComplete *BuildCallAwayProper(Database::TDBTransaction &DBTransaction,TIRTable *TempTable);
	TReturnItemComplete * BuildReturnProper(TIBDataSet *OrderTable,TIBDataSet *OptionsTable,TIBDataSet *SubOrdersTable);
	void ProcessTransmission();
	void CallAways(Database::TDBTransaction &DBTransaction);
	void BillSeats(Database::TDBTransaction &DBTransaction);
	void ClearMessageQue();
	void ProcessKitchenModFinal(Database::TDBTransaction &DBTransaction,TList * OrdersList);

private:
	void fUpdateUserSettings(const TPalmUserSettings &newPalmUserSettings);

	void fReceiveSeatNames(Database::TDBTransaction &DBTransaction,TIRPacket *Packet);

	struct TSortPalmOrders
	{
		bool inline operator() (const TReturnItemComplete* lhs, const TReturnItemComplete* rhs) const
		{
			if (lhs->Palm_ID == rhs->Palm_ID)
			{
				return lhs->OrderKey < rhs->OrderKey;
			}
			else
			{
				return (lhs->Palm_ID < rhs->Palm_ID);
			}
		}
	};
//	template<class T>
/*	class TPalmItemsSet : public std::set<TReturnItemComplete *, TPalm::TSortPalmOrders()>
	{
		public: ~TPalmItemsSet()
		{
			for (TPalmItemsSet::const_iterator iOrders = begin(); iOrders != end(); ++iOrders) delete *iOrders;
		}
	};*/

	typedef std::set<TReturnItemComplete *, TPalm::TSortPalmOrders >	TPalmItemsSet;
	typedef std::pair<TPalmItemsSet, Currency>							TPalmSeatItems;
	typedef std::map<int, TPalmSeatItems>								TPalmSeats;
	typedef std::map<int, TPalmSeats>									_TPalmTables;

	class TPalmTables
	{
		public:

		typedef _TPalmTables::const_iterator	const_iterator;
		typedef _TPalmTables::size_type			size_type;

		const_iterator	begin()	const				{ return PalmTables.begin(); }
		const_iterator	end()		const				{ return PalmTables.end(); }
		size_type		size()	const				{ return PalmTables.size(); }
		TPalmSeats &	operator [](int Index)	{ return PalmTables[Index]; }

		~TPalmTables()
		{
			for (const_iterator iTable = PalmTables.begin(); iTable != PalmTables.end(); ++iTable)
			{
				TPalmSeats::const_iterator iSeat;
				for (iSeat = iTable->second.begin(); iSeat != iTable->second.end(); ++iSeat)
				{
					const TPalmItemsSet &PalmItemsSet = iSeat->second.first;

					TPalmItemsSet::const_iterator iOrders;

					for (iOrders = PalmItemsSet.begin(); iOrders != PalmItemsSet.end(); ++iOrders)
					{
						delete *iOrders;
					}
				}
			}
		}

		private:

		_TPalmTables PalmTables;
	};

	int fLoadDisabledItems(std::vector<unsigned short> &DisablesItemsList, unsigned char &SectionFlag);
	void fLoadPalmOrders(Database::TDBTransaction &DBTransaction,TPalm::TPalmTables &PalmTables);
	void fStreamOrders(const TPalm::TPalmTables &PalmTables, TMemoryStream *Stream, TStrings *Log, unsigned char &SectionFlag);

	inline void fWriteStreamChar(TStream *Stream, unsigned char Char, UnicodeString LogMessage, TStrings *Log, bool SetMSB=false)
	{
		if (SetMSB) Char |= 0x80;
		Stream->Write(&Char, 1);
//		#if (DEBUGLEVEL & debugExpandGetOrdes)
		if (Log) Log->Add(LogMessage + ":" + UnicodeString::StringOfChar(' ', 20 - LogMessage.Length()) + IntToHex(static_cast<__int64>(Char), 2));
//		#endif
	}
	inline void fWriteStreamShort(TStream *Stream, unsigned short Short, UnicodeString LogMessage, TStrings *Log, bool SetMSB=false)
	{
		if (SetMSB) Short |= 0x8000;
		unsigned char Byte;
		Byte = HIBYTE(Short);	Stream->Write(&Byte, 1);
		Byte = LOBYTE(Short);	Stream->Write(&Byte, 1);
//		#if (DEBUGLEVEL & debugExpandGetOrdes)
		if (Log) Log->Add(LogMessage + ":" + UnicodeString::StringOfChar(' ', 20 - LogMessage.Length()) + IntToHex(static_cast<__int64>(Short), 4));
//		#endif
	}
	inline void fWriteStreamLong(TStream *Stream, unsigned long Long, UnicodeString LogMessage, TStrings *Log, bool SetMSB=false)
	{
		if (SetMSB) Long |= 0x80000000;
		unsigned char Byte;
		Byte = HIBYTE(HIWORD(Long));	Stream->Write(&Byte, 1);
		Byte = LOBYTE(HIWORD(Long));	Stream->Write(&Byte, 1);
		Byte = HIBYTE(LOWORD(Long));	Stream->Write(&Byte, 1);
		Byte = LOBYTE(LOWORD(Long));	Stream->Write(&Byte, 1);
//		#if (DEBUGLEVEL & debugExpandGetOrdes)
		if (Log) Log->Add(LogMessage + ":" + UnicodeString::StringOfChar(' ', 20 - LogMessage.Length()) + IntToHex(static_cast<__int64>(Long), 8));
//		#endif
	}
	inline void fWriteStreamString(TStream *Stream, UnicodeString String, UnicodeString LogMessage, TStrings *Log)
	{
//		#if (DEBUGLEVEL & debugExpandGetOrdes)
		if (Log) Log->Add(LogMessage + ":" + UnicodeString::StringOfChar(' ', 20 - LogMessage.Length()) + String);
//		#endif

		unsigned char Byte;
		Stream->Write(String.t_str(), String.Length() - 1);
		Byte = String[String.Length()] | 0x80;
		Stream->Write(&Byte, 1);
	}
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
      unsigned char InfoSyncNo,MenuSync1No,MenuSync2No,HighStaffIDNo,LowStaffIDNo;
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
   unsigned short StaffIDNo;
   
   struct PktLogin
   {
   public:
		int NewStaffID;
      // Contains the Users Pin in the Case of a Login.
		UnicodeString UsersPIN;
      // Contains Name (Unique Palm Identifier in case of an InfoSync or Login)
      UnicodeString DeviceIP;
      void Clear();
   }Login;

   struct PktRegister
   {
   public:
      // Contains Name (Unique Palm Identifier in case of an InfoSync or Login)
      UnicodeString DeviceIP;
		unsigned short PalmUniqueID;
		unsigned short SyncPrintingToPalmKey;
      UnicodeString NewDeviceName;
      UnicodeString RegistrationPIN;
      char RegistrationFlag;
   }Register;

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
   std::set<long> SelectedCourses;	// Callaways serving course

	const std::auto_ptr<TList> Seats;

private:
	void CleanUp();
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
   UnicodeString Name;
   bool NameChange;

	const std::auto_ptr<TList> Orders;
private:
	void CleanUp();
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
   UnicodeString Note;

	std::vector<long> Options;
   std::vector<long> Items;
   std::vector<long> Container;

   bool DrinkIDCount;
   bool Mixer;
   bool PromoItem;
   bool SetMenuGroupIsIndex; // One of My Numbers
   bool IsPriceAdjust;
   unsigned short SetMenuGroup;
   unsigned char SplitCount;
   long PalmSplitLK;
	long OrderLK; // Used in New Order().

	USHORT ServingCourseID;
};


#endif
#endif
