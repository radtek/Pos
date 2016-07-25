// ---------------------------------------------------------------------------
#pragma hdrstop

#include "KitchenMod.h"
#include "Stream.h"
#include "Dialogs.hpp"
#include "NetMessageChefMate.h"
#include <Memory>
#include <set>

#ifdef MenuMate
#include "DBSaleTimes.h"
#else
#include "Queries.h"
#endif

// ---------------------------------------------------------------------------

#pragma package(smart_init)

__fastcall TKitchenMod::TKitchenMod(Database::TDBControl &inDBControl) : DBControl(inDBControl)
{
	Displays[edtCurrent] = new TDisplayOrderSet;
	Displays[edtCurrent]->NewOrderChar = 0x00;
	Displays[edtCurrent]->FinalOrderTag = "";
	Displays[edtCurrent]->CurrentOrderTag = "|";
	Displays[edtAged] = new TDisplayOrderSet;
	Displays[edtAged]->NewOrderChar = 0x00;
	Displays[edtAged]->FinalOrderTag = "";
	Displays[edtAged]->CurrentOrderTag = "";
	WarningTime = 0;
	RemovedWhenClosed = false;
	RemovedWhenScanned = false;
}

// ---------------------------------------------------------------------------
__fastcall TKitchenMod::~TKitchenMod()
{
	for (TDisplaySet::iterator itDisplays = Displays.begin(); itDisplays != Displays.end(); advance(itDisplays, 1))
	{
		delete itDisplays->second;
	}
	delete udpClient;
	delete udpServer;
}
// ---------------------------------------------------------------------------

void TKitchenMod::Initialise()
{
	udpClient = new TIdUDPClient(NULL);
	udpClient->Port = 65004;
	udpClient->Active = true;

	udpServer = new TIdUDPServer(NULL);
#if defined (MenuMate)
	udpServer->DefaultPort = 0;
	udpServer->OnUDPRead = NULL;
#elif defined (PalmMate)
	udpServer->DefaultPort = 0;
	udpServer->OnUDPRead = NULL;
#else defined (ChefMate)
	udpServer->DefaultPort = 65004;
	udpServer->OnUDPRead = udpServerUDPRead;
	udpServer->Active = true;
#endif

	Enabled = false;
	AutoCloseWhenBumped = false;
	RemovedWhenScanned = false;
}

void TKitchenMod::SendRequest(TNetMessageChefMate *Request, UnicodeString DestinationIP)
{
	if (Enabled)
	{
		if ((Request != NULL) && (DestinationIP != ""))
		{
			std::auto_ptr <TMemoryStream> SentStream(new TMemoryStream);
			Request->Encode(SentStream.get());
			SentStream->Position = 0;
			udpClient->Host = DestinationIP;
			udpClient->Port = 65004;
			udpClient->SendBuffer(RawToBytes(SentStream->Memory, SentStream->Size));
		}
	}
}

void __fastcall TKitchenMod::udpServerUDPRead(TIdUDPListenerThread *AThread, TBytes AData, TIdSocketHandle *ABinding)
{
	try
	{
		if (AData.Length > 0)
		{
			std::auto_ptr <TMemoryStream> InboundStream(new TMemoryStream);
			WriteTIdBytesToStream(InboundStream.get(), AData);
			InboundStream->Position = 0;
			TNetMessage *Data = TNetMessageMaker::ConstructMessage(InboundStream.get());
			switch(Data->MessageType)
			{
			case udp_Unknown:
				break;
			case udp_ktichen:
				{
					SetRequest((TNetMessageChefMate*)Data);
				}break;
			}
		}
	}
	catch(Exception & Err)
	{
		ShowMessage(Err.Message + "IP : " + ABinding->PeerIP + " Byte Count " + IntToStr(AData.Length) + " Port " + IntToStr
			(ABinding->PeerPort));
	}
}

// ---------------------------------------------------------------------------
// Called By UPD packet Needs to be added to terminal and Display updated.

void TKitchenMod::SetRequest(TNetMessageChefMate *Request)
{
	if (Request->FinalOrder)
	{
		Request->tmPrinted = Now();
		Add(edtCurrent, Request);
		Beep();
	}
	else
	{
		// Order StartMake time was just scanned off and closed by a till.
		if (Request->OrderTimeScanned == true)
		{
			if (RemovedWhenScanned)
			{
				TNetMessageChefMate *OldRequest = FindByTimeKey(edtCurrent, Request->TimeKey);
				if (OldRequest)
				{
					Delete(OldRequest);
				}

				OldRequest = FindByTimeKey(edtAged, Request->TimeKey);
				if (OldRequest)
				{
					Delete(OldRequest);
				}
			}
			delete Request;
		}
		else if (Request->RemoveOrder == true)
		{
			TNetMessageChefMate *OldRequest = FindByOrderAndDevice(edtCurrent, Request->OrderNumber, Request->Device);
			if (OldRequest != NULL)
			{
				OldRequest->RemoveOrder = Request->RemoveOrder;
				OldRequest->TimeKey = Request->TimeKey;
				OldRequest->ChitNumber = Request->ChitNumber;
				OldRequest->tmPrinted = Now();
				Update(edtCurrent);
			}
			delete Request;
		}
		else if (DisplayDynamicOrders) // Update the order.
		{
			Add(edtCurrent, Request);
		}
		Beep();
	}
}

// ---------------------------------------------------------------------------
void TKitchenMod::SetMinutesWarning(TDateTime inWarningTime)
{
	WarningTime = inWarningTime;
	for (TDisplaySet::iterator itDisplays = Displays.begin(); itDisplays != Displays.end(); advance(itDisplays, 1))
	{
		itDisplays->second->WarningTime = inWarningTime;
	}
}

void TKitchenMod::Update(TDisplayType Type)
{
	if (Displays.find(Type) != Displays.end())
	{
		Displays[Type]->Update();
	}
}

void TKitchenMod::Add(TDisplayType Type, TNetMessageChefMate *Request)
{
	if (Displays.find(Type) != Displays.end())
	{
		Displays[Type]->Add(Request);
	}
}

void TKitchenMod::Delete( /* TDisplayType Type, */ TNetMessageChefMate *Request)
{
	/* Remove from the selected display
	Displays[Type]->Delete(Request);
	 */

	/* Remove from all displays */
	/* Assign Request to a local Variable so it not accessed once its Freed fromthe Main list. */
	TNetMessageChefMate LocalRequest;
	LocalRequest.Assign(Request);
	for (TDisplaySet::iterator disp = Displays.begin(); disp != Displays.end(); advance(disp, 1))
	{
		disp->second->Delete(&LocalRequest);
	}
}

void TKitchenMod::UpdateOrderDurations(Database::TDBTransaction &DBTransaction)
{
	for (TDisplaySet::iterator disp = Displays.begin(); disp != Displays.end(); advance(disp, 1))
	{
		TDisplayOrderSet *DisplayOrderSet = disp->second;
		for (int i = 0; i < DisplayOrderSet->Size(); i++)
		{
			TNetMessageChefMate *NetMessageChefMate = DisplayOrderSet->Get(i);
			NetMessageChefMate->Duration = GetOrderDuration(DBTransaction, NetMessageChefMate->TimeKey);
		}
	}
}

TDateTime TKitchenMod::GetOrderDuration(Database::TDBTransaction &DBTransaction, int TimeKey)
{
	TDateTime DateTime = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->SQL->Clear();
		IBInternalQuery->ParamCheck = true;
		IBInternalQuery->SQL->Text = "SELECT " " SALE_START_TIME " " FROM" " TURNAROUNDTIMES " "WHERE " "TIME_KEY = :TIME_KEY";
		IBInternalQuery->ParamByName("TIME_KEY")->AsInteger = TimeKey;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			DateTime = Now() - IBInternalQuery->FieldByName("SALE_START_TIME")->AsDateTime;
		}
	}
	catch(Exception & E)
	{
	}
	return DateTime;
}

bool TKitchenMod::GetOrderClosed(Database::TDBTransaction &DBTransaction, int TimeKey)
{
	bool retVal = false;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->SQL->Clear();
		IBInternalQuery->ParamCheck = true;
		IBInternalQuery->SQL->Text = "SELECT * FROM" " TURNAROUNDTIMES" " WHERE" " TIME_KEY = :TIME_KEY AND MAKE_END_TIME IS NOT NULL";
		IBInternalQuery->ParamByName("TIME_KEY")->AsInteger = TimeKey;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			retVal = true;
		}
	}
	catch(Exception & E)
	{
	}
	return retVal;
}

TNetMessageChefMate * TKitchenMod::FindByLineNumber(TDisplayType Type, int LineNumber)
{
	TNetMessageChefMate *RetVal = NULL;
	if (Displays.find(Type) != Displays.end())
	{
		RetVal = Displays[Type]->FindByLineNumber(LineNumber);
	}
	return RetVal;
}

TNetMessageChefMate * TKitchenMod::FindByOrderAndDevice(TDisplayType Type, int OrderNumber, UnicodeString Device)
{
	TNetMessageChefMate *RetVal = NULL;
	if (Displays.find(Type) != Displays.end())
	{
		RetVal = Displays[Type]->FindByOrderAndDevice(OrderNumber, Device);
	}
	return RetVal;
}

// ---------------------------------------------------------------------------
TNetMessageChefMate * TKitchenMod::FindByTimeKey(TDisplayType Type, int TimeKey)
{
	TNetMessageChefMate *RetVal = NULL;
	if (Displays.find(Type) != Displays.end())
	{
		RetVal = Displays[Type]->FindByTimeKey(TimeKey);
	}
	return RetVal;
}

TNetMessageChefMate * TKitchenMod::FindByChitNumber(TDisplayType Type, UnicodeString ChitNumber)
{
	TNetMessageChefMate *RetVal = NULL;
	if (Displays.find(Type) != Displays.end())
	{
		RetVal = Displays[Type]->FindByChitNumber(ChitNumber);
	}
	return RetVal;
}

__fastcall TListChefDisplay::~TListChefDisplay()
{
	ChefDisplayClear();
}

// ---------------------------------------------------------------------------
int TListChefDisplay::ChefDisplayAdd(TNetMessageChefMate *in)
{
	return Add(in);
}

void TListChefDisplay::Assign(TListChefDisplay *inListChefDisplay)
{
	for (int i = 0; i < inListChefDisplay->Count; i++)
	{
		TNetMessageChefMate *Source = inListChefDisplay->ChefDisplayGet(i);
		TNetMessageChefMate *Dest = new TNetMessageChefMate;
		*Dest = *Source;
		ChefDisplayAdd(Dest);
	}
}

// ---------------------------------------------------------------------------
void TListChefDisplay::ChefDisplayDelete(TNetMessageChefMate *in)
{
	if (in != NULL)
	{
		if (IndexOf(in) != -1)
		{
			Remove(in);
			delete(TNetMessageChefMate*)in;
			in = NULL;
		}
	}
}

// ---------------------------------------------------------------------------
int __fastcall SortFunc(void *Item1, void *Item2)
{
	TNetMessageChefMate *Order1 = (TNetMessageChefMate*)Item1;
	TNetMessageChefMate *Order2 = (TNetMessageChefMate*)Item2;

	if (Order1->RemoveOrder && !Order2->RemoveOrder)
	{
		return -1;
	}
	else if (Order1->RemoveOrder == Order2->RemoveOrder)
	{
		if (Order1->tmPrinted < Order2->tmPrinted)
		{
			return -1;
		}
		else if (Order1->tmPrinted == Order2->tmPrinted)
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

void TListChefDisplay::ChefDisplaySort()
{
	TListSortCompare ptrSortFunc = SortFunc;
	Sort(ptrSortFunc);
}

// ---------------------------------------------------------------------------
void TListChefDisplay::ChefDisplayClear()
{
	while (Count > 0)
	{
		ChefDisplayDelete(ChefDisplayGet(0));
	}
}

// ---------------------------------------------------------------------------
TNetMessageChefMate *TListChefDisplay::ChefDisplayGet(int Index)
{
	if (Count > Index)
	{
		return(TNetMessageChefMate*)Items[Index];
	}
	return NULL;
}

// ---------------------------------------------------------------------------

TNetMessageChefMate *TListChefDisplay::GetByOrderNumber(int OrderNumber, UnicodeString Device)
{
	for (int i = 0; i < Count; i++)
	{
		TNetMessageChefMate *Request = ChefDisplayGet(i);
		if (Request->OrderNumber == OrderNumber && Request->Device == Device)
		{
			return Request;
		}
	}
	return NULL;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
TNetMessageChefMate *TListChefDisplay::GetByTimeKey(int TimeKey)
{
	for (int i = 0; i < Count; i++)
	{
		TNetMessageChefMate *Request = ChefDisplayGet(i);
		if (Request->TimeKey == TimeKey)
		{
			return Request;
		}
	}
	return NULL;
}

// ---------------------------------------------------------------------------
TNetMessageChefMate *TListChefDisplay::GetByChitNumber(UnicodeString ChitNumber)
{
	for (int i = 0; i < Count; i++)
	{
		TNetMessageChefMate *Request = ChefDisplayGet(i);
		if (Request->ChitNumber == ChitNumber)
		{
			return Request;
		}
	}
	return NULL;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

__fastcall TTill::TTill()
{
	UserName = "";
	TillName = "";
}

void TKitchenMod::RegisterDisplays(TDisplayType Type, std::vector <TRichEdit *> &inDisplays, int MaxHeight)
{
	Displays[Type]->RegisterDisplays(inDisplays, MaxHeight);
	Displays[Type]->MaxDisplayLines = MaxHeight;
}

void TKitchenMod::SetFontSize(TFontDialog * FontDialog)
{
	if (FontDialog)
	{
		for (TDisplaySet::iterator itDisplays = Displays.begin(); itDisplays != Displays.end(); advance(itDisplays, 1))
		{
			for (int i = 0; i < itDisplays->second->Displays.size(); i++)
			{
				TRichEdit *Disp = itDisplays->second->Displays[i];
				Disp->SelectAll();
				Disp->SelAttributes->Name = FontDialog->Font->Name;
				Disp->SelAttributes->Size = FontDialog->Font->Size;
				Disp->SelAttributes->Height = FontDialog->Font->Height;
				Disp->SelAttributes->Style = FontDialog->Font->Style;
				Disp->Font = FontDialog->Font;
				Disp->SelLength = 0;

				RECT Rect;
				Disp->Perform(EM_GETRECT, 0, (int) & Rect);
				int EditHeight = Rect.bottom - Rect.top;

				std::auto_ptr <TControlCanvas> MemoCanvas(new TControlCanvas);
				MemoCanvas->Control = Disp;
				MemoCanvas->Font = Disp->Font;
				itDisplays->second->MaxDisplayLines = EditHeight / MemoCanvas->TextHeight("A");
			}
			itDisplays->second->Draw();
		}
	}
}

TDisplayOrderSet::TDisplayOrderSet() : OrdersList(new TListChefDisplay), InternalDisplay(new TStringList)
{
	MaxDisplayLines = 40;
	NewOrderChar = '*';
	FinalOrderTag = "";
	CurrentOrderTag = "";
}

TDisplayOrderSet::TDisplayOrderSet(const TDisplayOrderSet &inDisplayOrderSet) : OrdersList(new TListChefDisplay),
InternalDisplay(new TStringList)
{
	MaxDisplayLines = inDisplayOrderSet.MaxDisplayLines;
	NewOrderChar = inDisplayOrderSet.NewOrderChar;
	FinalOrderTag = inDisplayOrderSet.FinalOrderTag;
	CurrentOrderTag = inDisplayOrderSet.CurrentOrderTag;

	OrdersList->Assign(inDisplayOrderSet.OrdersList.get());
	InternalDisplay->Assign(InternalDisplay.get());
}

void TDisplayOrderSet::Draw()
{
	OrdersList->ChefDisplaySort();

	if (!Displays.empty())
	{
		for (int i = 0; i < Displays.size(); i++)
		{
			TRichEdit *Disp = Displays[i];
			Disp->Clear();
		}

		InternalDisplay->Clear();
		for (int i = 0; i < OrdersList->Count; i++)
		{
			TNetMessageChefMate *Request = OrdersList->ChefDisplayGet(i);
			int LineCount = InternalDisplay->Count;

			// Put the Age of the Order on top of the Docktet.
			if (double(Request->Duration) != 0)
			{
				UnicodeString Duration = "Time : " + Request->Duration.FormatString("hh:nn:ss");
				if (Request->Duration > WarningTime)
				{
					Duration = "*** " + Duration + " ***";
				}
				InternalDisplay->Add(Duration);
			}

			InternalDisplay->AddStrings(Request->Order);

			if (NewOrderChar != 0x00)
			{
				int Length = GetMaxLineLength(InternalDisplay.get());
				UnicodeString Orderbreak = Orderbreak.StringOfChar(NewOrderChar, Length);
				InternalDisplay->Add(Orderbreak);
			}

			for (int j = LineCount; j < InternalDisplay->Count; j++)
			{
				InternalDisplay->Objects[j] = (TObject*)Request;
			}
		}

		int TotalLinesApplied = 0;
		for (int i = 0; i < Displays.size() && (i < InternalDisplay->Count) && (TotalLinesApplied < InternalDisplay->Count); i++)
		{
			TRichEdit *Disp = Displays[i];
			int CurerntSelPos = Disp->SelStart;

			TNetMessageChefMate *LastRequest = (TNetMessageChefMate*)InternalDisplay->Objects[TotalLinesApplied];
			Disp->SelAttributes->Color = LastRequest->TillColour;
			if (LastRequest->Duration > WarningTime)
			{
				Disp->SelAttributes->Color = clRed;
			}

			for (int i = 0; (i < MaxDisplayLines) && (TotalLinesApplied < InternalDisplay->Count); i++, TotalLinesApplied++)
			{
				TNetMessageChefMate *CurrentRequest = (TNetMessageChefMate*)InternalDisplay->Objects[TotalLinesApplied];
				if (LastRequest != CurrentRequest)
				{
					LastRequest = CurrentRequest;
				}
				Disp->SelAttributes->Color = CurrentRequest->TillColour;
				if (CurrentRequest->Duration > WarningTime)
				{
					Disp->SelAttributes->Color = clRed;
				}

				UnicodeString Temp = InternalDisplay->Strings[TotalLinesApplied];
				UnicodeString Printable = "";
				if (Temp != "")
				{
					if (CurrentRequest->FinalOrder)
					{
						Printable += FinalOrderTag;
					}
					else
					{
						Printable += CurrentOrderTag;
					}
					for (int i = 1; i <= Temp.Length(); i++)
					{
						if (isprint(Temp[i]))
						{
							Printable += Temp[i];
						}
					}
				}
				Disp->Lines->Add(Printable);
			}
		}

		for (int i = 0; i < Displays.size(); i++)
		{
			TRichEdit *Disp = Displays[i];
			Disp->SelStart = 0;
		}
	}
}

void TDisplayOrderSet::Update()
{
	Draw();
}

void TDisplayOrderSet::Add(TNetMessageChefMate *Request)
{
	if (OrdersList->GetByOrderNumber(Request->OrderNumber, Request->Device) == NULL)
	{
		Request->Received = Now();
		Request->DisplayEnabled = false;
	}
	else
	{
		// You cannot have two orders with the same order number.
		Request->Received = FindByOrderAndDevice(Request->OrderNumber, Request->Device)->Received;
		Request->Duration = FindByOrderAndDevice(Request->OrderNumber, Request->Device)->Duration;
		Request->DisplayEnabled = FindByOrderAndDevice(Request->OrderNumber, Request->Device)->DisplayEnabled;
		OrdersList->ChefDisplayDelete(FindByOrderAndDevice(Request->OrderNumber, Request->Device));
	}
	OrdersList->ChefDisplayAdd(Request);
	Draw();
}

void TDisplayOrderSet::Delete(TNetMessageChefMate *Request)
{
	TNetMessageChefMate *OrderToRemove = FindByOrderAndDevice(Request->OrderNumber, Request->Device);
	if (OrderToRemove != NULL)
	{
		OrdersList->ChefDisplayDelete(OrderToRemove);
		Draw();
	}
}

TNetMessageChefMate * TDisplayOrderSet::Get(int Index)
{
	return OrdersList->ChefDisplayGet(Index);
}

int TDisplayOrderSet::Size()
{
	return OrdersList->Count;
}

TNetMessageChefMate *TDisplayOrderSet::FindByOrderAndDevice(int OrderNumber, UnicodeString Device)
{
	return OrdersList->GetByOrderNumber(OrderNumber, Device);
}

TNetMessageChefMate *TDisplayOrderSet::FindByTimeKey(int TimeKey)
{
	return OrdersList->GetByTimeKey(TimeKey);
}

TNetMessageChefMate *TDisplayOrderSet::FindByChitNumber(UnicodeString ChitNumber)
{
	return OrdersList->GetByChitNumber(ChitNumber);
}

TNetMessageChefMate *TDisplayOrderSet::FindByLineNumber(int LineNumber)
{
	TNetMessageChefMate * RetVal = NULL;
	if (InternalDisplay->Count > LineNumber)
	{
		RetVal = (TNetMessageChefMate*)(InternalDisplay->Objects[LineNumber]);
	}
	return RetVal;
}

void TDisplayOrderSet::RegisterDisplays(std::vector <TRichEdit *> &inDisplays, int MaxHeight)
{
	Displays = inDisplays;
	MaxDisplayLines = MaxHeight;
}

int TDisplayOrderSet::GetMaxLineLength(TStringList * Data)
{
	int RetVal = 0;
	for (int i = 0; i < Data->Count; i++)
	{
		if (Data->Strings[i].Length() > RetVal)
		{
			RetVal = Data->Strings[i].Length();
		}
	}
	return RetVal;
}

void TKitchenMod::RemoveClosedOrders(Database::TDBTransaction &DBTransaction)
{
	for (TDisplaySet::iterator disp = Displays.begin(); disp != Displays.end(); advance(disp, 1))
	{
		TDisplayOrderSet *DisplayOrderSet = disp->second;
		for (int i = 0; i < DisplayOrderSet->Size(); i++)
		{
			TNetMessageChefMate *NetMessageChefMate = DisplayOrderSet->Get(i);
			if (GetOrderClosed(DBTransaction, NetMessageChefMate->TimeKey))
			{
				Delete(NetMessageChefMate);
			}
		}
	}
}
