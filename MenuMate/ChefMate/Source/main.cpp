//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "main.h"
#include "Queries.h"
#include "SystemConfigLite.h"
#include "MMRegistry.h"
#include "frmCfg.h"
#include "enum.h"
#include "MMTouchKeyboard.h"
#include "MMMessageBox.h"
#include "MMTouchNumpad.h"
#include "ManagerVariable.h"
#include <DateUtils.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchControls"
#pragma link "TouchPages"
#pragma link "TouchBtn"
#pragma link "TouchGrid"
#pragma link "TouchNumpad"
#pragma resource "*.dfm"

TfrmMain *frmMain;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
	: TForm(Owner), Kitchen(new TKitchenMod(DBControl)), BarCodeReader(new TMMBarcodeReader)
{
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::FormCreate(TObject *Sender)
{
	VM = new TManagerVariable;

	Application->OnMessage = AppMessage;

	std::vector<TRichEdit *> Displays;
	Displays.push_back(Display1);
	Displays.push_back(Display2);
	Displays.push_back(Display3);
	Displays.push_back(Display4);
	Display1->Font = Fonts->Font;
	Display2->Font = Fonts->Font;
	Display3->Font = Fonts->Font;
	Display4->Font = Fonts->Font;
	int Height = abs(Display1->Font->Size * Display1->Font->PixelsPerInch / 72);
	int MaxHeight = (Display1->Height / Height) - 4;
	Kitchen->RegisterDisplays(edtCurrent,Displays,MaxHeight);
	Displays.clear();
	Displays.push_back(AgedDisplay1);
	Displays.push_back(AgedDisplay2);
	Displays.push_back(AgedDisplay3);
	AgedDisplay1->Font = Fonts->Font;
	AgedDisplay2->Font = Fonts->Font;
	AgedDisplay3->Font = Fonts->Font;
	Kitchen->RegisterDisplays(edtAged,Displays,MaxHeight);
	Kitchen->SetFontSize(Fonts);

	int MinutesWarning = 5;
	RegistryRead(MMBaseKey "Configuration","RedisplayWarningMinutes",MinutesWarning);

   Kitchen->SetMinutesWarning(RecodeMinute(TDateTime(), MinutesWarning));

	Displays.clear();
/*	Displays.push_back(DynamicDisplay1);
	DynamicDisplay1->Font = Fonts->Font;
	DynamicDisplay1->Width = ClientWidth;
	Kitchen->RegisterDisplays(edtDynamic,Displays,MaxHeight);*/

	Kitchen->Initialise();

	if(!Screen->Monitors[frmMain->Monitor->MonitorNum]->Primary)
	{
		frmMain->MakeFullyVisible(Screen->Monitors[0]);
	}
}


void __fastcall TfrmMain::FormDestroy(TObject *Sender)
{
   delete VM;
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
   {
      int Temp = Tag;
		Tag = Screen->Width;
		ScaleBy(Screen->Width, Temp);
	}

	Left = Screen->Monitors[frmMain->Monitor->MonitorNum]->Left;
	Width = Screen->Monitors[frmMain->Monitor->MonitorNum]->Width;
	Top = Screen->Monitors[frmMain->Monitor->MonitorNum]->Top;
	Height = Screen->Monitors[frmMain->Monitor->MonitorNum]->Height;
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::FormShow(TObject *Sender)
{
	FormResize(this);

	if(!DBControl.Connected())
	{
		if(!OpenDB())
		{
			Close();
			return;
		}
	}

	RefreshTimer->Enabled = true;

	int PortNumber = -1;
	RegistryRead(MMBaseKey "Comms","SerialBarcodePort",PortNumber);
	if(PortNumber != -1 && !BarCodeReader->Open(PortNumber))
	{
		MessageBox("Unable to Open Barcode Reader Comm port.\r" + BarCodeReader->LastException, "Barcode Reader Error",	MB_OK + MB_ICONWARNING);
	}

	String KeyName = MMBaseKey "Configuration";
	String KickKey = "AutoCloseWhenBumped";
	bool AutoCloseWhenBumped = false;
	RegistryRead(KeyName,KickKey,AutoCloseWhenBumped);
	Kitchen->AutoCloseWhenBumped = AutoCloseWhenBumped;

	KeyName = MMBaseKey "Configuration";
	KickKey = "RemovedWhenScanned";
	bool RemovedWhenScanned = false;
	RegistryRead(KeyName,KickKey,RemovedWhenScanned);
	Kitchen->RemovedWhenScanned = RemovedWhenScanned;

	KeyName = MMBaseKey "Configuration";
	KickKey = "RemovedWhenClosed";
	bool RemovedWhenClosed = false;
	RegistryRead(KeyName,KickKey,RemovedWhenClosed);
	Kitchen->RemovedWhenClosed = RemovedWhenClosed;



	KeyName = MMBaseKey "Configuration";
	KickKey = "DisplayDynamicOrders";
	bool DisplayDynamicOrders = false;
	RegistryRead(KeyName,KickKey,DisplayDynamicOrders);
	Kitchen->DisplayDynamicOrders = DisplayDynamicOrders;

	KeyName = MMBaseKey "Configuration";
	KickKey = "ConfirmBeforeBumping";
	ConfirmBeforeBumping = false;
	RegistryRead(KeyName,KickKey,ConfirmBeforeBumping);

	ConfigSplitters();

	int MinutesWarning = 5;
	RegistryRead(MMBaseKey "Configuration","RedisplayWarningMinutes",MinutesWarning);
   Kitchen->SetMinutesWarning(RecodeMinute(TDateTime(), MinutesWarning));

	tpDisplays->ActivePage = tsCurrentOrders;

	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
   VM->DeviceProfileKey = VM->SetProfile(DBTransaction,eTerminalProfiles,"ChefMate");
   // Get the Font Info.
   UnicodeString FontName = "Courier New";
   VM->GetProfileStr(DBTransaction,VM->DeviceProfileKey,vmGraphicFontName,FontName);
   int FontSize = 14;
   VM->GetProfileInt(DBTransaction,VM->DeviceProfileKey,vmGraphicFontSize,FontSize);
	DBTransaction.Commit();

   Fonts->Font->Name = FontName;
   Fonts->Font->Size = FontSize;
	Kitchen->SetFontSize(Fonts);
}

//---------------------------------------------------------------------------

bool TfrmMain::OpenDB(bool ShowConfig)
{
	bool RetVal = false;
	bool Abort = false;
	do
	{
		// Retrive MenuMate DB Settings.
		UnicodeString DatabasePath = "";
		RegistryRead(MMBaseKey "Database","DatabasePath",DatabasePath);

		UnicodeString InterbaseIP = "";
		RegistryRead(MMBaseKey "Database","InterbaseIP",InterbaseIP);

		try
		{
			if(InterbaseIP != "" && DatabasePath != "")
			{
				DBControl.Disconnect();
				DBControl.Init(Database::TDBSettings(InterbaseIP,DatabasePath,false));
				DBControl.Connect();
				Database::TDBUtilities Utils(DBControl);
				Utils.SetForcedWrites();
				RetVal = true;
			}
			Abort = false;
		}
		catch(Exception &Err)
		{
			CloseDB();
			Abort = false;
			RetVal = false;
		}

		if(RetVal == false && ShowConfig)
		{
			std::auto_ptr<TfrmSystemConfig> frmSystemConfig(new TfrmSystemConfig(NULL));
			if(frmSystemConfig->ShowModal() == mrOk)
			{
				Abort = false;
			}
			else if(frmSystemConfig->ModalResult == mrAbort)
			{
				Abort = true;
			}
		}
	}while(RetVal == false && Abort == false);

	return RetVal;
}

void TfrmMain::CloseDB()
{
	DBControl.Disconnect();
}

void __fastcall TfrmMain::AppMessage(tagMSG &Msg, bool &Handled)
{
	if (Msg.message == WM_CHAR)
	{
		if (!(Msg.lParam & 0x1000000)) // Is the CTRL or ALT key being held down i.e is it an Extended Key.
		{
			if (!Screen->ActiveForm->ClassNameIs("TfrmTouchKeyboard") && !Screen->ActiveForm->ClassNameIs("TfrmMessageBox"))
			{
				Handled = true;
				if (tiCardSwipe->Enabled)
				{
					tiCardSwipe->Enabled = false;
					if(!iscntrl((char)Msg.wParam))
               {
						CardSwipeString += (char)Msg.wParam;
               }
               tiCardSwipe->Enabled = true;
				}
				else
				{
               if(!iscntrl((char)Msg.wParam))
               {
						CardSwipeString = (char)Msg.wParam;
						tiCardSwipe->Enabled = true;
					}
				}
			}
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::tiCardSwipeTimer(TObject *Sender)
{
	tiCardSwipe->Enabled = false;
	if (CardSwipeString != "")
	{
		PostMessage(Screen->ActiveForm->Handle, UWM_CARDSWIPE, (UINT)&CardSwipeString, NULL);
	}
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::RefreshTimerTimer(TObject *Sender)
{
	if(OpenDB(false))
	{
		 Database::TDBTransaction DBTransaction(DBControl);
		 DBTransaction.StartTransaction();

		 tbtnConfig->Caption = "Order Count " + IntToStr(GetTotalDailyOrders(DBTransaction));

		 //Update all the Order durations.
		 if(Kitchen->RemovedWhenClosed)
		 {
			 Kitchen->RemoveClosedOrders(DBTransaction);
		 }

		 Kitchen->UpdateOrderDurations(DBTransaction);
		 Kitchen->Update(edtCurrent);

		 unsigned short hour = 0, min = 0, sec = 0, msec = 0;
		 TDateTime Total = GetSalesAverage(DBTransaction);
		 Total.DecodeTime(&hour,&min,&sec,&msec);
		 AvgSales->Caption = IntToStr(min) + ":" + IntToStr(sec);
		 Total = GetMakeAverage(DBTransaction);
		 Total.DecodeTime(&hour,&min,&sec,&msec);
		 AvgMake->Caption = IntToStr(min) + ":" + IntToStr(sec);
		 Total = GetMakeLongest(DBTransaction);
		 Total.DecodeTime(&hour,&min,&sec,&msec);
		 LongestMake->Caption = IntToStr(min) + ":" + IntToStr(sec);

		 if(double(Kitchen->WarningTime) != 0)
		 {
			 if(OutStandingOrders(DBTransaction,Kitchen->WarningTime))
			 {
				 std::auto_ptr<TListChefDisplay> OldOrdersList(new TListChefDisplay);
				 GetOutStandingOrder(DBTransaction,Kitchen->WarningTime,OldOrdersList.get());
				 if(OldOrdersList->Count > 0)
				 {
					 tsAgedOrders->TabVisible = true;
				 }
				 else
				 {
					 tsAgedOrders->TabVisible = false;
					 if(tpDisplays->ActivePage == tsAgedOrders)
					 {
						 tpDisplays->ActivePage = tsCurrentOrders;
					 }
				 }
				 for (int i = 0; i < OldOrdersList->Count ; i++)
				 {
					 TNetMessageChefMate *Request = OldOrdersList->ChefDisplayGet(i);
					 if(Request->Order->Count > 0)
					 {
						 if(!Kitchen->FindByOrderAndDevice(edtAged,Request->OrderNumber,Request->Device))
						 {
							 TNetMessageChefMate *InternalRequest = new TNetMessageChefMate;
							 InternalRequest->Assign(Request);
							 Kitchen->Add(edtAged,InternalRequest);
						 }
					 }
				 }
			 }
			 else
			 {
				 tsAgedOrders->TabVisible = false;
				 if(tpDisplays->ActivePage == tsAgedOrders)
				 {
					 tpDisplays->ActivePage = tsCurrentOrders;
				 }
			 }
		 }
		 DBTransaction.Commit();
		 tbtnConfig->ButtonColor = clNavy;
	}
	CloseDB();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::tbtnConfigClick(TObject *Sender)
{
	std::auto_ptr<TfrmConfig> frmConfig(new TfrmConfig(NULL,DBControl,*VM));
	frmConfig->ShowModal();
	ConfigSplitters();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
	if(Key == VK_MENU)
	{
      std::auto_ptr<TfrmConfig> frmConfig(new TfrmConfig(NULL,DBControl,*VM));
      frmConfig->ShowModal();
	}
	else if(Key == VK_F1 || Key == VK_ESCAPE)
	{
		tpDisplays->ActivePage = tsCurrentOrders;
	}
	else if(Key == VK_F2 || Key == VK_CONTROL)
	{
		tpDisplays->ActivePage = tsAgedOrders;
	}
	else if(Key == VK_RETURN || Key == VK_SPACE)
	{
      TNetMessageChefMate *Request = Kitchen->FindByLineNumber((TDisplayType)tpDisplays->ActivePageIndex,0);
      if(Request)
      {
         if(frmMain->ConfirmBeforeBumping == false || (frmMain->ConfirmBeforeBumping == true && MessageBox("Remove (bump) Order #" + Request->ChitNumber, "Remove Order",	MB_OKCANCEL + MB_ICONWARNING) == IDOK))
         {
            if(Request->TimeKey != 0 && Kitchen->AutoCloseWhenBumped)
            {
               Database::TDBTransaction DBTransaction(DBControl);
               DBTransaction.StartTransaction();
               CloseMakeStartTime(DBTransaction,Request->TimeKey);
               DBTransaction.Commit();
            }
            Kitchen->Delete(Request);
			}
		}
	}
   else if( (Key >= VK_NUMPAD0 && Key <= VK_NUMPAD9) || (char(Key) >= '0' && char(Key) <= '9') )
   {
      int KeyPressed = 0;
      if(Key >= VK_NUMPAD0 && Key <= VK_NUMPAD9)
      {
         KeyPressed = Key - VK_NUMPAD0;
      }
      else if (char(Key) >= '0' && char(Key) <= '9')
      {
         KeyPressed = char(Key) - 0x30;
      }

      std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
      frmTouchNumpad->Caption = "Enter the Chit Number";
      frmTouchNumpad->btnSurcharge->Caption = "Ok";
      frmTouchNumpad->btnSurcharge->Visible = true;
      frmTouchNumpad->btnDiscount->Visible = false;
      frmTouchNumpad->PreSelect = false;
      frmTouchNumpad->Mode = pmNumber;
      frmTouchNumpad->INTInitial = KeyPressed;
      if (frmTouchNumpad->ShowModal() == mrOk)
      {
         UnicodeString ChitNumber = IntToStr(frmTouchNumpad->INTResult);
         // Bring up the text box and stick the number in it.
         if(frmMain->ConfirmBeforeBumping == false || (frmMain->ConfirmBeforeBumping == true && MessageBox("Remove (bump) Order #" + ChitNumber, "Remove Order",	MB_OKCANCEL + MB_ICONWARNING) == IDOK))
         {
            TNetMessageChefMate *Request = Kitchen->FindByChitNumber((TDisplayType)tpDisplays->ActivePageIndex,ChitNumber);
            if(Request)
            {
               if(Request->TimeKey != 0 && Kitchen->AutoCloseWhenBumped)
               {
                  Database::TDBTransaction DBTransaction(DBControl);
                  DBTransaction.StartTransaction();
                  CloseMakeStartTime(DBTransaction,Request->TimeKey);
                  DBTransaction.Commit();
               }
               Kitchen->Delete(Request);
            }
         }
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::CardSwipe(TMessage& Message)
{
	tbtnConfig->ButtonColor = clBlue;
	enum TCardType {eUnknown,eKitchenDocket,eStaffCard,eMemberCard,eTabCard};
   TCardType DataType = eUnknown;
	int TimeKey = 0;
	AnsiString Data = *((AnsiString *)Message.WParam);

	if(Data.Pos("MMK") != 0) // Kitchen Docket.
	{
		int Index = Data.Pos("MMK");
		DataType = eKitchenDocket;
		TimeKey = StrToInt("0x" + Data.SubString(Index+3,Data.Length()));
	}

	if(DataType == eKitchenDocket) // Kitchen Docket.
	{
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		int RecordsUpdated = CloseMakeStartTime(DBTransaction,TimeKey);
		DBTransaction.Commit();
		if(RecordsUpdated > 0)
		{
			tbtnConfig->ButtonColor = clGreen;
		}
		else
		{
			tbtnConfig->ButtonColor = clRed;
		}

      if(Kitchen->RemovedWhenScanned)
      {
			TNetMessageChefMate *Request = Kitchen->FindByTimeKey(edtCurrent,TimeKey);
			if(Request)
			{
				Kitchen->Delete(Request);
			}

			Request = Kitchen->FindByTimeKey(edtAged,TimeKey);
			if(Request)
			{
				Kitchen->Delete(Request);
			}

      }
	}
	else if(DataType == eStaffCard)
	{
	}
	else if(DataType == eTabCard)
	{
	}
	else if(DataType == eMemberCard)
	{
	}
	else
	{
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::Display1MouseUp(TObject *Sender,
		TMouseButton Button, TShiftState Shift, int X, int Y)
{
   TRichEdit *Disp = (TRichEdit *)Sender;
   int LineNo = 0;
   if(Disp == Display2)
   {
      LineNo += Display1->Lines->Count;
   }
   else if(Disp == Display3)
   {
      LineNo += Display1->Lines->Count;
      LineNo += Display2->Lines->Count;
   }
   else if(Disp == AgedDisplay2)
   {
      LineNo += AgedDisplay1->Lines->Count;
   }
   else if(Disp == AgedDisplay3)
   {
      LineNo += AgedDisplay1->Lines->Count;
      LineNo += AgedDisplay2->Lines->Count;
   }


   int Height = abs(Disp->Font->Size * Disp->Font->PixelsPerInch / 72);
   LineNo += Y / Height;
   TNetMessageChefMate *Request = Kitchen->FindByLineNumber((TDisplayType)tpDisplays->ActivePageIndex,LineNo);
   if(Request != NULL)
   {
      if(MessageBox("Remove (bump) Order #" + Request->ChitNumber, "Remove Order",	MB_OKCANCEL + MB_ICONWARNING) == IDOK)
      {
         if(Request->TimeKey != 0 && Kitchen->AutoCloseWhenBumped)
         {
            Database::TDBTransaction DBTransaction(DBControl);
            DBTransaction.StartTransaction();
            CloseMakeStartTime(DBTransaction,Request->TimeKey);
            DBTransaction.Commit();
         }
         Kitchen->Delete(Request);
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::tpDisplaysChange(TObject *Sender)
{
	if(double(Kitchen->WarningTime) != 0 && tpDisplays->ActivePage == tsAgedOrders)
	{
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		if(OutStandingOrders(DBTransaction,Kitchen->WarningTime))
		{
			std::auto_ptr<TListChefDisplay> OldOrdersList(new TListChefDisplay);
			GetOutStandingOrder(DBTransaction,Kitchen->WarningTime,OldOrdersList.get());
			for (int i = 0; i < OldOrdersList->Count ; i++)
			{
				TNetMessageChefMate *Request = OldOrdersList->ChefDisplayGet(i);
				if(Request->Order->Count > 0)
				{
					if(!Kitchen->FindByOrderAndDevice(edtAged,Request->OrderNumber,Request->Device))
					{
						TNetMessageChefMate *InternalRequest = new TNetMessageChefMate;
						InternalRequest->Assign(Request);
						Kitchen->Add(edtAged,InternalRequest);
					}
				}
			}
		}
		DBTransaction.Commit();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Splitter1Moved(TObject *Sender)
{
	String KeyName = MMBaseKey "Configuration";
	String KickKey = "Splitter1Left";
	RegistryWrite(KeyName,KickKey,Splitter1->Left);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Splitter2Moved(TObject *Sender)
{
	String KeyName = MMBaseKey "Configuration";
	String KickKey = "Splitter2Left";
	RegistryWrite(KeyName,KickKey,Splitter2->Left);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Splitter3Moved(TObject *Sender)
{
	String KeyName = MMBaseKey "Configuration";
	String KickKey = "Splitter3Left";
	RegistryWrite(KeyName,KickKey,Splitter3->Left);
}
//---------------------------------------------------------------------------



void __fastcall TfrmMain::SplitterAged1Moved(TObject *Sender)
{
	String KeyName = MMBaseKey "Configuration";
	String KickKey = "SplitterAged1Left";
	RegistryWrite(KeyName,KickKey,SplitterAged1->Left);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::SplitterAged2Moved(TObject *Sender)
{
	String KeyName = MMBaseKey "Configuration";
	String KickKey = "SplitterAged2Left";
	RegistryWrite(KeyName,KickKey,SplitterAged2->Left);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::SplitterAged3Moved(TObject *Sender)
{
	String KeyName = MMBaseKey "Configuration";
	String KickKey = "SplitterAged3Left";
	RegistryWrite(KeyName,KickKey,SplitterAged3->Left);
}
//---------------------------------------------------------------------------

void TfrmMain::ConfigSplitters()
{
	String KeyName = MMBaseKey "Configuration";
	String KickKey = "Splitter1Left";
	int Splitter1Left = (ClientWidth/4);
	RegistryRead(KeyName,KickKey,Splitter1Left);

	KeyName = MMBaseKey "Configuration";
	KickKey = "Splitter2Left";
	int Splitter2Left = (ClientWidth/4) * 2;
	RegistryRead(KeyName,KickKey,Splitter2Left);

	KeyName = MMBaseKey "Configuration";
	KickKey = "Splitter3Left";
	int Splitter3Left = (ClientWidth/4) * 3;
	RegistryRead(KeyName,KickKey,Splitter3Left);

	KeyName = MMBaseKey "Configuration";
	KickKey = "SplitterAged1Left";
	int SplitterAged1Left = (ClientWidth/4);
	RegistryRead(KeyName,KickKey,SplitterAged1Left);

	KeyName = MMBaseKey "Configuration";
	KickKey = "SplitterAged2Left";
	int SplitterAged2Left = (ClientWidth/4) * 2;
	RegistryRead(KeyName,KickKey,SplitterAged2Left);

	KeyName = MMBaseKey "Configuration";
	KickKey = "Splitter3Left";
	int SplitterAged3Left = (ClientWidth/4) * 3;
	RegistryRead(KeyName,KickKey,SplitterAged3Left);

	Display1->Width = Splitter1Left;
	Display2->Width = Splitter2Left - Splitter1Left;
	Display3->Width = Splitter3Left - Splitter2Left;
	AgedDisplay1->Width = SplitterAged1Left;
	AgedDisplay2->Width = SplitterAged2Left - SplitterAged1Left;
	AgedDisplay3->Width = SplitterAged3Left - SplitterAged2Left;
}
