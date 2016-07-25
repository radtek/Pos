//---------------------------------------------------------------------------

#ifndef EftposSyncroPinPadCTRLH
#define EftposSyncroPinPadCTRLH

#include "PINPADCONTROL2Lib_OCX.h"
#include "MagneticCard.h"

#include <memory>

//---------------------------------------------------------------------------
const int  PINPAD_BUSY = 0x80040601;
const int  NOT_LOCKED = 0x80040602;
const int  NO_DEVICE = 0x80040603;
const int  RESPONSE_ERROR = 0x80040604;
const int  INVALID_PARAMETER = 0x80040613;
const int  PINPAD_PARAMERROR = 0x80040613;
const int  UNEXPECTED = 0x800406FF;

const int  ACCOUNT_SELECTED = 325;
const int  CARD_SWIPED = 326;
const int  CARD_ERROR = 327;
const int  PIN_ENTERED = 328;
const int  CUSTOMER_DATA_ENTERED = 329;
const int  CLEAR_PRESSED = 330;
const int  CANCEL_PRESSED = 331;
const int  ENTER_PRESSED = 332;
const int  RESPONSE_TIMEOUT = 333;
const int  RESPONSE_UNEXPECTED = 335;

const int  ACCOUNT__NONE = 0;
const int  ACCOUNT__CHEQUE = 1;
const int  ACCOUNT__CREDIT = 2;
const int  ACCOUNT__SAVINGS = 3;

const int   TRACK1 = 0x01;
const int   TRACK2 = 0x02;
const int   TRACK3 = 0x04;



class TEftposSyncroPinPadCTRL
{
   protected :
		std::auto_ptr<TForegroundPinPadControl2> PinPadCTRL;
      void __fastcall ForegroundPinPadControl21PINPadData(TObject *Sender, long lEventType);
      bool WaitOnEvent();
      bool CheckCard(AnsiString CardNumber);
      bool EventCompleted;
      bool Enabled;
	public:
      TEftposSyncroPinPadCTRL();
		~TEftposSyncroPinPadCTRL();
		void Initialise();
		bool RetrieveCard(TMagneticCardTrackData &TrackData);
      AnsiString ErrorMessage;
};

#endif
