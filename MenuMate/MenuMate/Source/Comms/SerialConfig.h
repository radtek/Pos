//---------------------------------------------------------------------------

#ifndef SerialConfigH
#define SerialConfigH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
#include "CPortCtl.hpp"
#include "CPort.hpp"
#include "touchbtn.h"
#include "touchcontrols.h"
#include "ZForm.h"
//---------------------------------------------------------------------------


class TfrmSerialConfig : public TZForm
{
__published:	// IDE-managed Components
   TPanel *Panel2;
   TPanel *Panel1;
   TGroupBox *GroupBox2;
   TLabel *lbeFlowControl;
   TLabel *lbeStopBits;
   TLabel *lbeBaudRate;
   TLabel *lbeDataBits;
   TLabel *lbeParity;
   TTouchBtn *btnBaudRate;
   TTouchBtn *btnDataBits;
   TTouchBtn *btnParity;
   TTouchBtn *btnStopBits;
   TTouchBtn *btnFlowControl;
   TTouchBtn *TouchBtn1;
   TTouchBtn *TouchBtn2;
   TCheckBox *cbAsync;
   void __fastcall btnBaudRateClick(TObject *Sender);
   void __fastcall btnDataBitsClick(TObject *Sender);
   void __fastcall btnParityClick(TObject *Sender);
   void __fastcall btnStopBitsClick(TObject *Sender);
   void __fastcall btnFlowControlClick(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall TouchBtn1MouseClick(TObject *Sender);
   void __fastcall TouchBtn2MouseClick(TObject *Sender);
   void __fastcall cbAsyncClick(TObject *Sender);
private:	// User declarations
   int BaudRateToInt(TBaudRate Baud);
   TBaudRate IntToBaudRate(int Baud);
   int DataBitsToInt(TDataBits DataBits);
   TDataBits IntToDataBits(int DataBits);   
   void __fastcall TfrmSerialConfig::DrawButtons();
public:		// User declarations
   __fastcall TfrmSerialConfig(TComponent* Owner);

   void __fastcall AssignSettings(TComPort *Port, bool &inAsync);
   void __fastcall LoadSettings(TComPort *Port, bool inAsync);
         
   TBaudRate BaudRate;
   TDataBits DataBits;
   TStopBits StopBits;
   TParityBits ParityBits;
   TRTSFlowControl RTSFlowControl;
   bool OutCTSFlow;
   bool XonXoffOut;
   bool XonXoffIn;
   bool Async;
};
//---------------------------------------------------------------------------
#endif
