//---------------------------------------------------------------------------

#ifndef PoleDisplayH
#define PoleDisplayH
#include <CPort.hpp>
#include <list>
#include "MM_DBCore.h"
//---------------------------------------------------------------------------
class TPoleDisplay
{
   private:
   TComPort *PoleDisplayPort;
      bool Busy;
      bool CloseingPort;
      bool OpeningPort;
      bool OpeningFailed;
      std::list<PAsync> writeAsyncPtrList;
      void AsyncWrite(AnsiString Data);
	public:
      void __fastcall PortTxEmpty(TObject *Sender);

	  bool AsyncMode;
	  int EmulationMode;
      int PortNumber;
      TPoleDisplay();
      virtual ~TPoleDisplay();
      void UpdatePoleDisplay(AnsiString TopLineLeft,AnsiString TopLineRight,AnsiString BottomLineLeft,AnsiString BottomLineRight);
      void UpdatePoleDisplay(AnsiString TopLineLeft,AnsiString BottomLineLeft);
      void UpdatePoleDisplayDefault();
      bool Open(int PortNum);
      void Close();
      void ClearPoleDisplay();

      TComPort *GetTComPort();
      void Initialise(Database::TDBTransaction &DBTransaction);
      void SaveSettings(Database::TDBTransaction &DBTransaction);
};
#endif
 