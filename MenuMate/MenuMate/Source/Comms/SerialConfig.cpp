//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SerialConfig.h"
#include "MMTouchNumpad.h"
#include "VerticalSelect.h"
#include "MMMessageBox.h"
#include "MMLogging.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "touchbtn"
#pragma link "touchcontrols"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TfrmSerialConfig::TfrmSerialConfig(TComponent* Owner)
   : TZForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSerialConfig::DrawButtons()
{
   btnBaudRate->Caption = IntToStr(BaudRateToInt(BaudRate));
   btnDataBits->Caption = IntToStr(DataBitsToInt(DataBits));

   switch(ParityBits)
   {
      case prNone:
      {
         btnParity->Caption = "None";
      }
      break;
      case prOdd:
      {
         btnParity->Caption = "Odd";
      }
      break;
      case prEven:
      {
         btnParity->Caption = "Even";
      }
      break;
      case prMark:
      {
         btnParity->Caption = "Mark";
      }
      break;
      case prSpace:
      {
         btnParity->Caption = "Space";
      }
      break;
   }


   switch(StopBits)
   {
      case sbOneStopBit:
      {
         btnStopBits->Caption = "1 Stop Bit";
      }
      break;
      case sbOne5StopBits:
      {
         btnStopBits->Caption = "1.5 Stop Bits";
      }
      break;
      case sbTwoStopBits:
      {
         btnStopBits->Caption = "2 Stop Bits";
      }
      break;
   }


   if( RTSFlowControl == rtsEnable &&
       OutCTSFlow == false &&
       XonXoffOut == false &&
       XonXoffIn == false
   )
   {
      btnFlowControl->Caption = "None";
   }
   else
   if( RTSFlowControl == rtsHandshake &&
       OutCTSFlow == true &&
       XonXoffOut == false &&
       XonXoffIn == false
   )
   {
      btnFlowControl->Caption = "RTS/CTS (Hardware)";
   }
   else
   if( RTSFlowControl == rtsEnable &&
       OutCTSFlow == false &&
       XonXoffOut == true &&
       XonXoffIn == true
   )
   {
      btnFlowControl->Caption = "Xon/Xoff (Software)";
   }

   cbAsync->Checked = Async;
}

void __fastcall TfrmSerialConfig::LoadSettings(TComPort *Port,bool inAsync)
{
   BaudRate = Port->BaudRate;
   DataBits = Port->DataBits;
   StopBits = Port->StopBits;
   ParityBits = Port->Parity->Bits;

   RTSFlowControl = Port->FlowControl->ControlRTS;
   OutCTSFlow = Port->FlowControl->OutCTSFlow;
   XonXoffOut = Port->FlowControl->XonXoffOut;
   XonXoffIn = Port->FlowControl->XonXoffIn;

   Async     = inAsync;

   DrawButtons();

}

void __fastcall TfrmSerialConfig::AssignSettings(TComPort *Port, bool &inAsync)
{
   if(Port != NULL)
   {
      Port->BaudRate = BaudRate;
      Port->DataBits = DataBits;
      Port->StopBits = StopBits;
      Port->Parity->Bits = ParityBits;

      Port->FlowControl->ControlRTS = RTSFlowControl;
      Port->FlowControl->OutCTSFlow = OutCTSFlow;
      Port->FlowControl->XonXoffOut = XonXoffOut;
      Port->FlowControl->XonXoffIn = XonXoffIn;
      inAsync = Async;
   }
}

int TfrmSerialConfig::BaudRateToInt(TBaudRate Baud)
{
   int RetVal = 19200;
   switch(Baud)
   {
         case br110 :
         RetVal = 110;
         break;
         case br300 :
         RetVal = 300;
         break;
         case br600 :
         RetVal = 600;
         break;
         case br1200 :
         RetVal = 1200;
         break;
         case br2400 :
         RetVal = 2400;
         break;
         case br4800 :
         RetVal = 4800;
         break;
         case br9600 :
         RetVal = 9600;
         break;
         case br14400 :
         RetVal = 14400;
         break;
         case br19200 :
         RetVal = 19200;
         break;
         case br38400 :
         RetVal = 38400;
         break;
         case br56000 :
         RetVal = 56000;
         break;
         case br57600 :
         RetVal = 57600;
         break;
         case br115200 :
         RetVal = 115200;
         break;
         default :
         throw Exception("Invalid Baud Rate");
   }
   return RetVal;
}

TBaudRate TfrmSerialConfig::IntToBaudRate(int Baud)
{
   TBaudRate RetVal = br9600;
   switch(Baud)
   {
         case 110 :
         RetVal = br110;
         break;
         case 300:
         RetVal = br300;
         break;
         case 600 :
         RetVal = br600;
         break;
         case 1200 :
         RetVal = br1200 ;
         break;
         case 2400 :
         RetVal = br2400;
         break;
         case 4800 :
         RetVal = br4800;
         break;
         case 9600 :
         RetVal = br9600;
         break;
         case 14400:
         RetVal = br14400;
         break;
         case 19200:
         RetVal = br19200;
         break;
         case 38400 :
         RetVal = br38400;
         break;
         case 56000 :
         RetVal = br56000;
         break;
         case 57600 :
         RetVal = br57600;
         break;
         case 115200 :
         RetVal = br115200;
         break;
         default :
         throw Exception("Invalid Baud Rate");
   }
   return RetVal;
}

int TfrmSerialConfig::DataBitsToInt(TDataBits DataBits)
{
   int RetVal = 8;
   switch(DataBits)
   {
         case dbFive :
         RetVal = 5;
         break;
         case dbSix :
         RetVal = 6;
         break;
         case dbSeven :
         RetVal = 7;
         break;
         case dbEight :
         RetVal = 8;
         break;
         default :
         throw Exception("Invalid Number of Data Bits 5 -> 8");
   }
   return RetVal;
}

TDataBits TfrmSerialConfig::IntToDataBits(int DataBits)
{
   TDataBits RetVal = br9600;
   switch(DataBits)
   {
         case 5 :
         RetVal = dbFive;
         break;
         case 6:
         RetVal = dbSix;
         break;
         case 7 :
         RetVal = dbSeven;
         break;
         case 8 :
         RetVal = dbEight ;
         break;
         default :
         throw Exception("Invalid Number of Data Bits 5 -> 8");
   }
   return RetVal;
}


void __fastcall TfrmSerialConfig::btnBaudRateClick(TObject *Sender)
{
   try
   {
      std::auto_ptr<TfrmTouchNumpad> Numpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
      Numpad->Caption = "Enter Baud Rate";
      Numpad->btnSurcharge->Caption = "Ok";
      Numpad->btnDiscount->Visible = false;
      Numpad->btnSurcharge->Visible = true;
      Numpad->Mode = pmNumber;
      Numpad->INTInitial = BaudRateToInt(BaudRate);
      if (Numpad->ShowModal() == mrOk)
      {
         BaudRate = IntToBaudRate(Numpad->INTResult);
      }
      DrawButtons();
   }
	catch(Exception &E)
	{
		MessageBox(E.Message, "Error",MB_OK + MB_ICONERROR);
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmSerialConfig::btnDataBitsClick(TObject *Sender)
{
   try
   {
      std::auto_ptr<TfrmTouchNumpad> Numpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
      Numpad->Caption = "Enter Number Of Data Bits 5 -> 8";
      Numpad->btnSurcharge->Caption = "Ok";
      Numpad->btnDiscount->Visible = false;
      Numpad->btnSurcharge->Visible = true;
      Numpad->Mode = pmNumber;
      Numpad->INTInitial = DataBitsToInt(DataBits);
      if (Numpad->ShowModal() == mrOk)
      {
         DataBits = IntToDataBits(Numpad->INTResult);

      }
      DrawButtons();
   }
	catch(Exception &E)
	{
		MessageBox(E.Message, "Error",MB_OK + MB_ICONERROR);
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSerialConfig::btnParityClick(TObject *Sender)
{
   try
   {
      // Display Reports List
      std::auto_ptr<TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

      TVerticalSelection Item;
      Item.Title = "Cancel";
      Item.Properties["Color"] = IntToStr(clMaroon);
      Item.CloseSelection = true;
      SelectionForm->Items.push_back(Item);

      TVerticalSelection Item1;
      Item1.Title = "No parity";
      Item1.Properties["Action"] = IntToStr(1);
      Item1.Properties["Color"] = IntToStr(clNavy);
      Item1.CloseSelection = true;
      SelectionForm->Items.push_back(Item1);

      TVerticalSelection Item3;
      Item3.Title = "Odd";
      Item3.Properties["Action"] = IntToStr(2);
      Item3.Properties["Color"] = IntToStr(clNavy);
      Item3.CloseSelection = true;
      SelectionForm->Items.push_back(Item3);

      TVerticalSelection Item6;
      Item6.Title = "Even";
      Item6.Properties["Action"] = IntToStr(3);
      Item6.Properties["Color"] = IntToStr(clNavy);
      Item6.CloseSelection = true;
      SelectionForm->Items.push_back(Item6);

      TVerticalSelection Item4;
      Item4.Title = "Mark";
      Item4.Properties["Action"] = IntToStr(4);
      Item4.Properties["Color"] = IntToStr(clNavy);
      Item4.CloseSelection = true;
      SelectionForm->Items.push_back(Item4);

      TVerticalSelection Item2;
      Item2.Title = "Space";
      Item2.Properties["Action"] = IntToStr(5);
      Item2.Properties["Color"] = IntToStr(clNavy);
      Item2.CloseSelection = true;
      SelectionForm->Items.push_back(Item2);

      SelectionForm->ShowModal();
      TVerticalSelection SelectedItem;
      if(SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel" )
      {
         int Action = StrToIntDef(SelectedItem.Properties["Action"],0);
         switch(Action)
         {
            case 1:
            {
               ParityBits = prNone;
            }
            break;
            case 2:
            {
               ParityBits = prOdd;
            }
            break;
            case 3:
            {
               ParityBits = prEven;
            }
            break;
            case 4:
            {
               ParityBits = prMark;
            }
            break;
            case 5:
            {
               ParityBits = prSpace;
            }
            break;
         }
      }
      DrawButtons();
   }
	catch(Exception &E)
	{
		MessageBox(E.Message, "Error",MB_OK + MB_ICONERROR);
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}

//---------------------------------------------------------------------------
void __fastcall TfrmSerialConfig::btnStopBitsClick(TObject *Sender)
{
   try
   {
      // Display Reports List
      std::auto_ptr<TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

      TVerticalSelection Item;
      Item.Title = "Cancel";
      Item.Properties["Color"] = IntToStr(clMaroon);
      Item.CloseSelection = true;
      SelectionForm->Items.push_back(Item);

      TVerticalSelection Item1;
      Item1.Title = "1 Stop Bit";
      Item1.Properties["Action"] = IntToStr(1);
      Item1.Properties["Color"] = IntToStr(clNavy);
      Item1.CloseSelection = true;
      SelectionForm->Items.push_back(Item1);

      TVerticalSelection Item3;
      Item3.Title = "1.5 Stop Bits";
      Item3.Properties["Action"] = IntToStr(2);
      Item3.Properties["Color"] = IntToStr(clNavy);
      Item3.CloseSelection = true;
      SelectionForm->Items.push_back(Item3);

      TVerticalSelection Item6;
      Item6.Title = "2 Stop Bits";
      Item6.Properties["Action"] = IntToStr(3);
      Item6.Properties["Color"] = IntToStr(clNavy);
      Item6.CloseSelection = true;
      SelectionForm->Items.push_back(Item6);

      SelectionForm->ShowModal();
      TVerticalSelection SelectedItem;
      if(SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel" )
      {
         int Action = StrToIntDef(SelectedItem.Properties["Action"],0);
         switch(Action)
         {
            case 1:
            {
               StopBits = sbOneStopBit;
            }
            break;
            case 2:
            {
               StopBits = sbOne5StopBits;
            }
            break;
            case 3:
            {
               StopBits = sbTwoStopBits;
            }
            break;
         }
      }
      DrawButtons();
   }
	catch(Exception &E)
	{
		MessageBox(E.Message, "Error",MB_OK + MB_ICONERROR);
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSerialConfig::btnFlowControlClick(TObject *Sender)
{
   try
   {
      // Display Reports List
      std::auto_ptr<TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

      TVerticalSelection Item;
      Item.Title = "Cancel";
      Item.Properties["Color"] = IntToStr(clMaroon);
      Item.CloseSelection = true;
      SelectionForm->Items.push_back(Item);

      TVerticalSelection Item1;
      Item1.Title = "None";
      Item1.Properties["Action"] = IntToStr(1);
      Item1.Properties["Color"] = IntToStr(clNavy);
      Item1.CloseSelection = true;
      SelectionForm->Items.push_back(Item1);

      TVerticalSelection Item3;
      Item3.Title = "RTS/CTS (Hardware)";
      Item3.Properties["Action"] = IntToStr(2);
      Item3.Properties["Color"] = IntToStr(clNavy);
      Item3.CloseSelection = true;
      SelectionForm->Items.push_back(Item3);

      TVerticalSelection Item6;
      Item6.Title = "Xon/Xoff (Software)";
      Item6.Properties["Action"] = IntToStr(3);
      Item6.Properties["Color"] = IntToStr(clNavy);
      Item6.CloseSelection = true;
      SelectionForm->Items.push_back(Item6);

      SelectionForm->ShowModal();
      TVerticalSelection SelectedItem;
      if(SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel" )
      {
         int Action = StrToIntDef(SelectedItem.Properties["Action"],0);
         switch(Action)
         {
            case 1:
            {
               RTSFlowControl = rtsEnable;
               OutCTSFlow = false;
               XonXoffOut = false;
               XonXoffIn = false;
            }
            break;
            case 2:
            {
               RTSFlowControl = rtsHandshake;
               OutCTSFlow = true;
               XonXoffOut = false;
               XonXoffIn = false;
            }
            break;
            case 3:
            {
               RTSFlowControl = rtsEnable;
               OutCTSFlow = false;
               XonXoffOut = true;
               XonXoffIn = true;
            }
            break;
         }
      }
      DrawButtons();
   }
	catch(Exception &E)
	{
		MessageBox(E.Message, "Error",MB_OK + MB_ICONERROR);
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSerialConfig::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
   {
      int Temp = Tag;
		Tag = Screen->Width;
      		if((double)Screen->Width / Screen->Height < 1.4)
                {
   			ScaleBy(Screen->Width, Temp);
                }
   }
	ClientWidth = Panel2->Left + Panel2->Width + Panel1->Left;
	ClientHeight = Panel1->Height + (Panel1->Top * 2);
   Left = (Screen->Width - Width) / 2;
	Top = (Screen->Height - Height) / 2;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSerialConfig::TouchBtn1MouseClick(TObject *Sender)
{
	ModalResult = mrOk;      
}
//---------------------------------------------------------------------------

void __fastcall TfrmSerialConfig::TouchBtn2MouseClick(TObject *Sender)
{
	ModalResult = mrCancel;   
}
//---------------------------------------------------------------------------

void __fastcall TfrmSerialConfig::cbAsyncClick(TObject *Sender)
{
   Async = cbAsync->Checked;   
}
//---------------------------------------------------------------------------

