//---------------------------------------------------------------------------


#pragma hdrstop

#include "Eftpos.h"
#include "Enum.h"

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif

#include "MMMessageBox.h"
#include "MMLogging.h"
#include "DBOrder.h"
#include "Processing.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


TEftPos *EftPos;

TEftPos::TEftPos(): LastEftPosReceipt(new TStringList)
{
	EftPosTransContainer					= new TListEftPosTransContainer;
	Enabled = false;
	EFTPOSType = eTEftPosUnknown;
	ChequeAccountMaxLength = 10;
	ChequeBranchMaxLength = 8;
	ChequeSerialMaxLength = 6;

	EnquiryEnabled = true;
	AllowTimeOut = false;   
	LastEftPosReceipt->Clear();

	OverLimitAmount = 0;
	AllowsTipping = false;
	TippableCardTypes.clear();
}


TEftPos::~TEftPos()
{
	delete EftPosTransContainer;
}

void TEftPos::Initialise()
{
}

bool __fastcall TEftPos::GetEnabled()
{
   return fEnabled;
}

void __fastcall TEftPos::SetEnabled(bool value)
{
   if (fEnabled != value)
	{
      if(TDeviceRealTerminal::Instance().PaymentSystem->EftPosEnabled && value)
      {
         fEnabled = value;
      }
      else
      {
         fEnabled = false;
      }
   }
}

bool TEftPos::CheckOverLimitExceeded(Currency Total)
{
   bool LimitExceeded = false;
   if(OverLimitAmount != 0)
   {
      if(Total > OverLimitAmount)
      {
         LimitExceeded = true;
      }
   }
   return LimitExceeded;
}




void TEftPos::SetTransactionEvent(AnsiString inID,eEFTTransactionType inTransType)
{
	TEftPosTransaction *EftTrans = new TEftPosTransaction(inID,inTransType);
	EftPosTransContainer->Add(EftTrans);
}

TEftPosTransaction *TEftPos::GetTransactionEvent(AnsiString inID)
{
	TEftPosTransaction *Event = EftPosTransContainer->GetByID(inID);
	if(Event == NULL)
	{
		// MessageBox("Eftpos Event " + inID + " not found", "Error",MB_OK + MB_ICONWARNING);
	}
	return Event;
}

TEftPosTransaction *TEftPos::GetTransactionEvent(eEFTTransactionType inTransType)
{
	TEftPosTransaction *Event = EftPosTransContainer->Get(inTransType);
	if(Event == NULL)
	{
		// MessageBox("Eftpos Event " + inID + " not found", "Error",MB_OK + MB_ICONWARNING);
	}
	return Event;
}

TEftPosTransaction *TEftPos::GetTransactionEventInWaiting()
{
	TEftPosTransaction *Event = EftPosTransContainer->GetByID(WaitingOnReferenceNumber);
	if(Event == NULL)
	{
		// MessageBox("Eftpos Event " + inID + " not found", "Error",MB_OK + MB_ICONWARNING);
	}
	return Event;
}

TEftPosTransaction *TEftPos::GetTransactionEvent()
{
	return EftPosTransContainer->Get(0);
}

void TEftPos::DelTransactionEvent(AnsiString inID)
{
	return EftPosTransContainer->Delete(inID);
}

//---------------------------------------------------------------------------
bool TEftPos::WaitOnEftPosEvent(AnsiString ReferenceNumber)
{
	bool EventCompleted = false;
	if(Enabled)
	{
		WaitingOnReferenceNumber = ReferenceNumber;

		bool Abort = false;

      TMMProcessingState State(Screen->ActiveForm,"Processing EFTPOS Please Wait...","Processing EFTPOS",AllowTimeOut,false,1000 * EFT_OCX_TIMEOUT);
      TDeviceRealTerminal::Instance().ProcessingController.Push(State);

		__int64 StartTime = ::GetTickCount();
		__int64 TickCount = StartTime + (1000 * EFT_OCX_TIMEOUT);
		while (!EventCompleted && !Abort)
		{

			while( ( (AllowTimeOut && ::GetTickCount() < TickCount) || (!AllowTimeOut) ) &&
						!EventCompleted &&
						!TDeviceRealTerminal::Instance().ProcessingController.Cancelled()
				  )
			{
				Application->ProcessMessages();
				TDeviceRealTerminal::Instance().ProcessingController.SetPosition(::GetTickCount() - StartTime);
				TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(ReferenceNumber);
				if(EftTrans != NULL)
				{
					if(EftTrans->EventCompleted)
					{
						 EventCompleted = true;
					}
				}
				else
				{
					 EventCompleted = true;
				}
			}

			if(!EventCompleted)
			{
				if (MessageBox("The EFTPOS Manager has failed to respond within the default time out period.\rDo you wish to continue to wait?\rSelecting No will cancel this EFTPOS Transaction.", "Warning", MB_YESNO + MB_ICONQUESTION) == IDNO)
				{
					Abort = true;
				}
				else
				{
					StartTime = ::GetTickCount();
					TickCount = StartTime + (1000 * EFT_OCX_TIMEOUT);
					TDeviceRealTerminal::Instance().ProcessingController.ResetCancelled();
				}
			}
		}
      TDeviceRealTerminal::Instance().ProcessingController.Pop();
	}
	return EventCompleted;
}


TEftPosTransaction::TEftPosTransaction(AnsiString inID,eEFTTransactionType inTransType)
: UniqueID(inID), TransType(inTransType)
{
	Result = eFailed;
	ResultText = "";
	SuppressReceipt = false;
	EventCompleted = false;
	CardType = "";
	EftposTransactionID = "";
    TimeOut = false;
}

__fastcall TListEftPosTransContainer::~TListEftPosTransContainer()
{
	Clear();
}
//---------------------------------------------------------------------------
int __fastcall TListEftPosTransContainer::Add(TEftPosTransaction *inEftPosTrans)
{
	return TList::Add(inEftPosTrans);
}
//---------------------------------------------------------------------------
void __fastcall TListEftPosTransContainer::Delete(AnsiString UniqueID)
{
	Delete(GetByID(UniqueID));
}

void __fastcall TListEftPosTransContainer::Delete(TEftPosTransaction *in)
{
	if(in != NULL)
   {
      if(IndexOf(in) != -1)
		{
			TList::Remove(in);
			delete (TEftPosTransaction *)in;
         in = NULL;
      }
   }
}

//---------------------------------------------------------------------------
void __fastcall TListEftPosTransContainer::Clear()
{
   while(Count > 0)
   {
		Delete(Get(0));
   }
}
//---------------------------------------------------------------------------
TEftPosTransaction *TListEftPosTransContainer::Get(int Index)
{
	if(Count > Index)
	{
		return (TEftPosTransaction *)Items[Index];
	}
	return NULL;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TEftPosTransaction *TListEftPosTransContainer::GetByID(AnsiString UniqueID)
{
	for (int i = 0 ; i < Count ; i++)
	{
		TEftPosTransaction *EftPosTransaction = Get(i);
		if(EftPosTransaction->UniqueID == UniqueID)
		{
			return EftPosTransaction;
		}
	}
	return NULL;
}
//---------------------------------------------------------------------------
TEftPosTransaction *TListEftPosTransContainer::Get(eEFTTransactionType inTransType)
{
   for (int i = 0 ; i < Count ; i++)
	{
		TEftPosTransaction *EftPosTransaction = Get(i);
		if(EftPosTransaction->TransType == inTransType)
		{
			return EftPosTransaction;
		}
	}
	return NULL;
}

//--------------------------------------------------------------------------
bool TEftPos::AllowsTipsOnTransactions()
{
	return AllowsTipping == true;
}
//--------------------------------------------------------------------------
void TEftPos::ProcessTip(WideString OriginalDpsTxnRef, Currency OriginalAmount, Currency TipAmount, UnicodeString MerchantRef)
{
    // does nothing here. this method needs to be overwritten by childs who implements tipping
}

std::vector<AnsiString> TEftPos::GetTippableCardTypes()
{
    return TippableCardTypes;
}
