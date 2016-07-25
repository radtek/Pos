//---------------------------------------------------------------------------
#pragma hdrstop

#include "EftposSyncroPinPadCTRL.h"
//---------------------------------------------------------------------------
#include "MMMessageBox.h"
#include "DeviceRealTerminal.h"
#include <winerror.h>
#include "Processing.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------

TEftposSyncroPinPadCTRL::TEftposSyncroPinPadCTRL() : PinPadCTRL(NULL)
{
   EventCompleted = false;
   Enabled = false;   
}


TEftposSyncroPinPadCTRL::~TEftposSyncroPinPadCTRL()
{
   PinPadCTRL->RequestCancel();
   do
   {
      PinPadCTRL->ReleaseLock();
   }
   while(PinPadCTRL->ResultCode != NOT_LOCKED && PinPadCTRL->ResultCode != PINPAD_BUSY);
}

void TEftposSyncroPinPadCTRL::Initialise()
{
	try
	{
		if(TGlobalSettings::Instance().EnableAARewards)
		{
			Enabled = true;
         PinPadCTRL.reset(new TForegroundPinPadControl2(NULL));
			PinPadCTRL->OnPINPadData = ForegroundPinPadControl21PINPadData;
		}
		else
		{
			Enabled = false;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EFTPOSLOG,"Unable to create TForegroundPinPadControl2" + E.Message);
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to create TForegroundPinPadControl2"  + E.Message);
		Enabled = false;
	}
}

bool TEftposSyncroPinPadCTRL::RetrieveCard(TMagneticCardTrackData &TrackData)
{
   bool RetVal = false;
   TForegroundPinPadControl2 *MyPinPadCTRL = PinPadCTRL.get();
	if(Enabled)
	{
      // Cant tell if this succeeded or failed stupid BCC Builder Type Lib Importer.
      MyPinPadCTRL->RequestLock();
      if(MyPinPadCTRL->ResultCode != PINPAD_BUSY)
      {
         EventCompleted = false;
         ErrorMessage = "";
         try
         {
               WideString Temp = "Swipe AA Rewards Card";
               MyPinPadCTRL->RequestCardSwipe( Temp.c_bstr(),TRACK2);

               // A full cards Track 2 looks like 3083261538427975=10010000000000000000
               /*Card Alogrithm check bunch of cards from AA rewards.
               Yeap this should be in a unit test of some kind.*/
               /*
               std::string Cards[] = {
                  "3083261594812301",
                  "3083261313139384",
                  "3083261635371966",
                  "3083261404493345",
                  "3083261538747102",
                  "3083261531527853",
                  "3083261322550606",
                  "3083261500574985",
                  "3083261518744385",
                  "" };

               for (int i = 0; Cards[i] != "" ; i++)
               {
                  if(!CheckCard(AnsiString(Cards[i].c_str())))
                  {
                     RetVal = false;
                  }
               } */

               if(WaitOnEvent())
               {
                  TrackData.Track1 = MyPinPadCTRL->Track1Data;
                  TrackData.Track2 = MyPinPadCTRL->Track2Data;
                  TrackData.Track3 = MyPinPadCTRL->Track3Data;
                  RetVal = true;
                  if(!CheckCard(TrackData.Track2.SubString(1,16)))
                  {
                     RetVal = false;
                  }
               }

               if(ErrorMessage == "")
               {
                  ErrorMessage = MyPinPadCTRL->ResultText;
               }
         }
         __finally
         {
            MyPinPadCTRL->RequestCancel();
            MyPinPadCTRL->ReleaseLock();
         }
      }
	}
   return RetVal;
}

void __fastcall TEftposSyncroPinPadCTRL::ForegroundPinPadControl21PINPadData(
      TObject *Sender, long lEventType)
{
   switch(lEventType)
   {
      case CANCEL_PRESSED :
      case CARD_SWIPED :
      case CARD_ERROR :
      case CLEAR_PRESSED :
      case ENTER_PRESSED :
      case RESPONSE_TIMEOUT :
         EventCompleted = true;
      break;
   }
/*
   lEventType's
	UNKNOWN
	CLEAR_PRESSED
	CANCEL_PRESSED
	ENTER_PRESSED
	PIN_ENTERED
	ACCOUNT_SELECTED
	CUSTOMER_DATA_ENTERED
	CARD_SWIPED
*/
}

bool TEftposSyncroPinPadCTRL::WaitOnEvent()
{
	EventCompleted = false;
	if(Enabled)
	{
      const int EFT_AAREWARDS_OCX_TIMEOUT = 1000 * 20; // 20 Seconds.
		bool Abort = false;
		bool AllowTimeOut = true;

      TMMProcessingState State(TDeviceRealTerminal::Instance().ProcessingController.GetTopOwner(),"Please Swipe AA Rewards Card...","Swipe Card.", true,true,EFT_AAREWARDS_OCX_TIMEOUT,0,0);
      TDeviceRealTerminal::Instance().ProcessingController.Push(State);

		__int64 StartTime = ::GetTickCount();
		__int64 TickCount = StartTime + (EFT_AAREWARDS_OCX_TIMEOUT);
		while (!EventCompleted && !Abort)
		{
			while( ( (AllowTimeOut && ::GetTickCount() < TickCount) || (!AllowTimeOut) ) &&
						!EventCompleted &&
						!TDeviceRealTerminal::Instance().ProcessingController.Cancelled()
				  )
			{
				Application->ProcessMessages();
				TDeviceRealTerminal::Instance().ProcessingController.SetPosition(::GetTickCount() - StartTime);
			}

			if(!EventCompleted)
			{
            if(TDeviceRealTerminal::Instance().ProcessingController.Cancelled())
            {
					Abort = true;
               ErrorMessage = "Cancelled By Operator.";
            }
				else if (MessageBox("The PIN Pad Manager has failed to respond within the default time out period.\rDo you wish to continue to wait?\rSelecting No will cancel this PIN Pad Transaction.", "Warning", MB_YESNO + MB_ICONQUESTION) == IDNO)
				{
					Abort = true;
				}
				else
				{
					StartTime = ::GetTickCount();
					TickCount = StartTime + EFT_AAREWARDS_OCX_TIMEOUT;
               TDeviceRealTerminal::Instance().ProcessingController.ResetCancelled();
				}
			}
		}
		TDeviceRealTerminal::Instance().ProcessingController.Pop();
	}
	return EventCompleted;
}

bool TEftposSyncroPinPadCTRL::CheckCard(AnsiString CardNumber)
{
/*
 * Checks whether a string of digits is a valid credit card number according to
 * the Luhn algorithm.
 *
 * 1. Starting with the second to last digit and moving left, double the value
 *    of all the alternating digits. For any digits that thus become 10 or more,
 *    add their digits together. For example, 1111 becomes 2121, while 8763
 *    becomes 7733 (from (1+6)7(1+2)3).
 *
 * 2. Add all these digits together. For example, 1111 becomes 2121, then
 *    2+1+2+1 is 6; while 8763 becomes 7733, then 7+7+3+3 is 20.
 *
 * 3. If the total ends in 0 (put another way, if the total modulus 10 is 0),
 *    then the number is valid according to the Luhn formula, else it is not
 *    valid. So, 1111 is not valid (as shown above, it comes out to 6), while
 *    8763 is valid (as shown above, it comes out to 20).


Check Digit processing." Module 10 Double add Double" routine.         
The second digit and each alternate digit from that point is doubled   
(Where the result of doubling a digit is a 2 digit number, the 2 digits 
of the number are added to each other).                                 
A sum is created by adding all of the resultant digits.                 
The check digit is found by subtracting the sum from the next highest   
multiple of 10.                                                         
                                                                        
e.g. Card number   707189910008301                                     
                   |||||||||||||||                                     
                   |||||||||||||||-----------------------1             
                   ||||||||||||||------0*2 = 0           0             
                   |||||||||||||-------------------------3             
                   ||||||||||||--------8*2 = 16    1+6=  7             
                   |||||||||||---------------------------0             
                   ||||||||||----------0*2 = 0           0             
                   |||||||||-----------------------------0             
                   ||||||||------------1*2 = 2           2             
                   |||||||-------------------------------9             
                   ||||||--------------9*2 = 18    1+8=  9 
                   |||||---------------------------------8 
                   ||||----------------1*2 = 2           2 
                   |||-----------------------------------7 
                   ||------------------0*2 = 0           0 
                   |-------------------------------------7 
                                                      ====== 
                                                         55 
                                          60 MINUS 55 = 5   

                    Therefore the digit check is 5.
*/
    bool RetVal = false;
    int n, i, alternate, sum;

    if (CardNumber == "")
        return false;

    n = CardNumber.Length();

    if (n < 13 || n > 19)
        return false;


   if(CardNumber.SubString(1,6) != "308326")
   {
      return false;
   }

    for (alternate = 0, sum = 0, i = n - 1; i > 0; --i)
    {
        if (!isdigit(CardNumber[i]))
            return false;

        n = CardNumber[i] - '0';

        if (alternate)
        {
            n *= 2;
            if (n > 9)
                n = (n % 10) + 1;
        }
        alternate = !alternate;

        sum += n;
    }

    RetVal = ((10 - (sum % 10)) == CardNumber[CardNumber.Length()] - '0' );
    return RetVal;
}
