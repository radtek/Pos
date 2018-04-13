//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SelectPhoenixRoom.h"
#include "MMMessageBox.h"
#include "MMLogging.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

char MemButtonChar[24][2] = { {'~', '`'}, {'!', '1'}, {'@', '2'}, {'#', '3'}, {'$', '4'}, {'%', '5'}, {'^', '6'},
	{'&', '7'}, {'*', '8'}, {'(', '9'}, {')', '0'}, {'_', '-'}, {'+', '='}, {'{', '['}, {'}', ']'}, {'|', '\\'},
	{':', ';'}, {'"', '\''}, {'<', ','}, {'>', '.'}, {'?', '/'} };

//---------------------------------------------------------------------------
__fastcall TfrmPhoenixRoom::TfrmPhoenixRoom(TComponent* Owner)
	: TZForm(Owner)//, *roomResult(new TRoomInquiryResult())
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmPhoenixRoom::FormCreate(TObject *Sender)
{
	SelectedRoom.Clear();
	StartWithShiftDown 	= false;
	MaxRemembered 			= 20;
	DefaultInterfaceDisplay = eMemNumPad;

	pnlLastPicked->Left = 0;
	for (int i=0; i<pnlLastPicked->ControlCount; i++)
	{
	  ((TTouchBtn *)pnlLastPicked->Controls[i])->Color = clMaroon;
	}

}
//---------------------------------------------------------------------------
void __fastcall TfrmPhoenixRoom::FormDestroy(TObject *Sender)
{
	for (int i=0; i<pnlLastPicked->ControlCount; i++)
	{
		delete pnlLastPicked->Controls[i];
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmPhoenixRoom::FormShow(TObject *Sender)
{
	SelectedRoom.Clear();
	DisplayData();
	FormResize(this);
	ShiftDown = StartWithShiftDown;
	CapsDown = false;
	tbtnOk->Enabled = false;
	DrawUpDownPanel(ShiftDown, pnlShift1);
	DrawUpDownPanel(CapsDown, pnlCapsLock);
	DrawLetters();
	edSearch->SetFocus();
	SelectionVisible = false;

	UpdateInterface();	

	int ButtonIndex = 0;
	TTouchBtn *Button = NULL;
	for (int i = 0; i < LastAccessed.size() ; i++)
	{
		Button					= new TTouchBtn(this);
		Button->Color			= clMaroon;
		Button->Width 			= (pnlScroll->ClientWidth - 6) / 4 - 6;
		Button->Height			= pnlLastPicked->ClientHeight;
		Button->Top   			= 0;
		Button->Left 			= ((ButtonIndex) * (Button->Width + 6));
		Button->Tag   			= 0;
		Button->Caption 		= LastAccessed[i];
		Button->OnMouseClick 		= QuickClick;
		Button->Parent  		= pnlLastPicked;
		Button->ParentFont	= true;
		ButtonIndex++;
	}

	if(LastAccessed.size() > 0)
	{
		edSearch->Text = LastAccessed[0];
	}

	if(Button != NULL)
	{
		pnlLastPicked->Width = Button->Left + Button->Width + 6;
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmPhoenixRoom::FormClose(TObject *Sender,
		TCloseAction &Action)
{
	StartWithShiftDown = false;
	MustHaveValidRoom = false;
	for (int i=0; i<pnlLastPicked->ControlCount; i++)
	{
		delete pnlLastPicked->Controls[i];
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPhoenixRoom::ShiftClick(TObject *Sender)
{
	if (ShiftDown == true)
	{
		ShiftDown = false;
		DrawUpDownPanel(false, pnlShift1);
	}
	else
	{
		ShiftDown = true;
		DrawUpDownPanel(true, pnlShift1);
   }
   DrawLetters();
}
//---------------------------------------------------------------------------
void TfrmPhoenixRoom::DrawUpDownPanel(bool Down, TPanel *Panel)
{
	if (Down == true)
   {
      Panel->BevelInner = bvLowered;
      Panel->BevelOuter = bvLowered;
   }
   else
	{
		Panel->BevelInner = bvRaised;
      Panel->BevelOuter = bvRaised;
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmPhoenixRoom::ButtonMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	// Print Tabs
	if (((TPanel *)Sender)->Tag == 251)
   {
		PostMessage(edSearch->Handle, WM_CHAR, '\t', 0);
		edSearch->SetFocus();
	}
	// Print the backspace
	else if (((TPanel *)Sender)->Tag == 252)
   {
		PostMessage(edSearch->Handle, WM_CHAR, '\b', 0);
		edSearch->SetFocus();
   }
	// Print the Return
	else if (((TPanel *)Sender)->Tag == 253)
   {
		PostMessage(edSearch->Handle, WM_CHAR, '\r', 0);
      edSearch->SetFocus();		
	}
   // Print the space
	else if (((TPanel *)Sender)->Tag == 254)
   {
      PostMessage(edSearch->Handle, WM_CHAR, ' ', 0);
      edSearch->SetFocus();
   }
   // Print the character
	else if (((TPanel *)Sender)->Tag == 255)
   {
      Word Key;
		Key = ((TPanel *)Sender)->Caption.c_str()[0];
      PostMessage(edSearch->Handle, WM_CHAR, Key, 0);
      edSearch->SetFocus();
   }
   // Print the number / others
   else if (((TComponent *)Sender)->Tag != 0 && ((TComponent *)Sender)->Tag != 255)
   {
		PostMessage(edSearch->Handle, WM_CHAR,
				MemButtonChar[((TComponent *)Sender)->Tag-1][ShiftDown?0:1], 0);
   }

   if (String(Sender->ClassName()) == "TPanel")
   {
      TPanel *Panel = (TPanel *)Sender;

		Panel->BevelInner = bvRaised;
      Panel->BevelOuter = bvRaised;
   }
   else if (String(Sender->ClassName()) == "TLabel")
   {
      TPanel *Panel = (TPanel *)((TLabel *)Sender)->Parent;

      Panel->BevelInner = bvRaised;
		Panel->BevelOuter = bvRaised;
   }
   if (Sender != pnlShift1 && Sender != pnlEnter && Sender != pnlBackSpace)
   {
   	if (ShiftDown)
      {
      	ShiftDown = false;
			DrawUpDownPanel(false, pnlShift1);
			DrawLetters();
      }
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmPhoenixRoom::ButtonMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (String(Sender->ClassName()) == "TPanel")
	{
      TPanel *Panel = (TPanel *)Sender;

      Panel->BevelInner = bvLowered;
      Panel->BevelOuter = bvLowered;
	}
   else if (String(Sender->ClassName()) == "TLabel")
   {
      TPanel *Panel = (TPanel *)((TLabel *)Sender)->Parent;

      Panel->BevelInner = bvLowered;
      Panel->BevelOuter = bvLowered;
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmPhoenixRoom::CapsLockClick(TObject *Sender)
{
	if (CapsDown == true)
   {
   	CapsDown = false;
		DrawUpDownPanel(false, pnlCapsLock);
   }
   else
   {
   	CapsDown = true;
		DrawUpDownPanel(true, pnlCapsLock);
	}
   DrawLetters();
}
//---------------------------------------------------------------------------
void TfrmPhoenixRoom::DrawLetters()
{
   // Sets upper or lower case letters.
   
	for (int i=0; i<pnlKeys->ControlCount; i++)
   {
      if (String(pnlKeys->Controls[i]->ClassName()) == "TPanel")
      {
			TPanel *Panel = (TPanel *)pnlKeys->Controls[i];

         if (Panel->Tag == 255)
         {
            if (CapsDown && ShiftDown)
            {
               Panel->Caption = Panel->Caption.LowerCase();
            }
            else if (CapsDown || ShiftDown)
            {
               Panel->Caption = Panel->Caption.UpperCase();
            }
            else
				{
					Panel->Caption = Panel->Caption.LowerCase();
            }
         }
         else
         {
            for (int j=0; j<Panel->ControlCount; j++)
            {
               if (String(Panel->Controls[j]->ClassName()) == "TLabel")
               {
						TLabel *Label = (TLabel *)Panel->Controls[j];
                  if (Label->Top < Panel->ClientHeight / 3)
                  {
                  	Label->Enabled = ShiftDown;
                  }
                  else
                  {
                  	Label->Enabled = !ShiftDown;
                  }
               }
            }
         }
      }
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPhoenixRoom::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
   {
      int Temp = Tag;
		Tag = Screen->Width;
                if((double)Screen->Width/Screen->Height < 1.4)
                {
			ScaleBy(Screen->Width, Temp);
                }
      Top = 0;
      Left = 0;
      Width = Screen->Width;
      Height = Screen->Height;
      Align = alClient;

		int CurrentPanelNumber = 0;
		Panel21->Height = BorderWidth;
		Panel19->Width = BorderWidth;
		Panel2->Left = Panel22->Left + Panel22->Width + BorderWidth;
		Panel16->Left = Panel2->Left + Panel2->Width + BorderWidth;
		Panel16->Width = Panel1->Width - Panel16->Left;
		btnRoomsLeft->Left = Panel2->Left;
		btnRoomsLeft->Top = Panel2->Top + Panel2->Height + BorderWidth;
		btnRoomsLeft->Height = Panel1->Height - btnRoomsLeft->Top;
		btnRoomsRight->Top = btnRoomsLeft->Top;
		btnRoomsRight->Height = btnRoomsLeft->Height;
		btnRoomsRight->Left = Panel1->Width - btnRoomsRight->Width;

		pnlScroll->Left = btnRoomsLeft->Left + btnRoomsLeft->Width + BorderWidth;
		pnlScroll->Height = btnRoomsLeft->Height;
		pnlScroll->Top = btnRoomsLeft->Top;
		pnlScroll->Width = btnRoomsRight->Left - pnlScroll->Left - BorderWidth;
		pnlLastPicked->Height = pnlScroll->Height;
		pnlLastPicked->Top = 0;
	}
}

void __fastcall TfrmPhoenixRoom::Panel1Resize(TObject *Sender)
{
		Panel2->Left = Panel22->Left + Panel22->Width + BorderWidth;
		Panel16->Left = Panel2->Left + Panel2->Width + BorderWidth;
		Panel16->Width = Panel1->Width - Panel16->Left;
		btnRoomsLeft->Left = Panel2->Left;
		btnRoomsLeft->Top = Panel2->Top + Panel2->Height + BorderWidth;
		btnRoomsLeft->Height = Panel1->Height - btnRoomsLeft->Top;
		btnRoomsRight->Top = btnRoomsLeft->Top;
		btnRoomsRight->Height = btnRoomsLeft->Height;
		btnRoomsRight->Left = Panel1->Width - btnRoomsRight->Width;

		pnlScroll->Left = btnRoomsLeft->Left + btnRoomsLeft->Width + BorderWidth;
		pnlScroll->Height = btnRoomsLeft->Height;
		pnlScroll->Top = btnRoomsLeft->Top;
		pnlScroll->Width = btnRoomsRight->Left - pnlScroll->Left - BorderWidth;
		pnlLastPicked->Height = pnlScroll->Height;
		pnlLastPicked->Top = 0;

}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TfrmPhoenixRoom::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(this);
}
//---------------------------------------------------------------------------
void __fastcall TfrmPhoenixRoom::btnOkClick(TObject *Sender)
{
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------


void __fastcall TfrmPhoenixRoom::FormCloseQuery(TObject *Sender,
		bool &CanClose)
{
	if(MustHaveValidRoom && (SelectedRoom.AccountNumber == "" || SelectedRoom.FolderNumber == 0))
	{
		MessageBox("A Valid Room and Folder must be selected", "Please Select a Room and Folder", MB_OK + MB_ICONINFORMATION);
		CanClose = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmPhoenixRoom::tbtnCancelClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TfrmPhoenixRoom::tbtnClearClick(TObject *Sender)
{
	memText->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TfrmPhoenixRoom::edSearchKeyPress(TObject *Sender,
		char &Key)
{
	if (Key == VK_RETURN)
	{
		QuickSearch();
		edSearch->SelectAll();
	}
}
//---------------------------------------------------------------------------
void TfrmPhoenixRoom::QuickSearch()
{

	SelectedRoom.Clear();
	SelectedRoom.AccountNumber = edSearch->Text;
    if(TGlobalSettings::Instance().PMSType == Oracle)
    {
//        MessageBox(roomResult.RoomInquiryItem.size(),"size()",MB_OK);
        roomResult.RoomInquiryItem.clear();
        TDeviceRealTerminal::Instance().BasePMS->GetRoomStatus(edSearch->Text,roomResult);
    }
    else if(TGlobalSettings::Instance().PMSType == SiHot)
    {
        TSiHotAccounts siHotAccount;
        siHotAccount.AccountNumber = edSearch->Text;
        SiHotAccounts.push_back(siHotAccount);
        TDeviceRealTerminal::Instance().BasePMS->GetRoomStatus(SiHotAccounts,PMSIPAddress,PMSPort);
    }
    else
    {
    	TDeviceRealTerminal::Instance().BasePMS->GetRoomStatus(SelectedRoom,PMSIPAddress,PMSPort);
        if(SelectedRoom.Result != eAccepted)
        {
            MessageBox(SelectedRoom.ResultText,"PMS Hotel Error", MB_OK + MB_ICONERROR);
        }
    }
    ShowAccount();

    for (int i=0; i<pnlLastPicked->ControlCount; i++)
    {
        TTouchBtn *UnSelected = (TTouchBtn * )pnlLastPicked->Controls[i];
        UnSelected->Color = clMaroon;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmPhoenixRoom::tbtnSearchByNameClick(TObject *Sender)
{
	tbtnSearchByName->Color = clGreen;
	tbtnSearchByNumber->Color = clMaroon;
	tbtnSearchByGuest->Color = clMaroon;
}
//---------------------------------------------------------------------------

void __fastcall TfrmPhoenixRoom::tbtnSearchByNumberClick(TObject *Sender)
{
	tbtnSearchByName->Color = clMaroon;
	tbtnSearchByNumber->Color = clGreen;
	tbtnSearchByGuest->Color = clMaroon;
}
//---------------------------------------------------------------------------

void __fastcall TfrmPhoenixRoom::tbtnSearchByGuestClick(TObject *Sender)
{
	tbtnSearchByName->Color = clMaroon;
	tbtnSearchByNumber->Color = clMaroon;
	tbtnSearchByGuest->Color = clGreen;
}
//---------------------------------------------------------------------------

void __fastcall TfrmPhoenixRoom::ListDrawCell(TObject *Sender,
		int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
	TCanvas *pCanvas = ((TStringGrid *)Sender)->Canvas;
	pCanvas->Font = ((TStringGrid *)Sender)->Font;
	pCanvas->Brush->Color = ((TStringGrid *)Sender)->Color;

	TColor Color = pCanvas->Brush->Color;
	TBrushStyle Style = pCanvas->Brush->Style;

	if ((State.Contains(gdSelected	) || State.Contains(gdFocused)) && SelectionVisible)
	{
		pCanvas->Font->Color = clWhite;
		pCanvas->Font->Style = TFontStyles() << fsBold;
		pCanvas->Brush->Color = clNavy;
		pCanvas->Brush->Style = bsSolid;
		pCanvas->FillRect(Rect); // clear the rectangle
	}
	else
	{
		pCanvas->Font->Color = clBlack;
		pCanvas->Font->Style = TFontStyles();
		pCanvas->FillRect(Rect); // clear the rectangle
	}

	UnicodeString Caption = List->Cells[ACol][ARow];
	TRect DrawRegRect(Rect.Left+2, Rect.Top, Rect.Right-2, Rect.Bottom);

	int TextHeight = DrawTextExW(pCanvas->Handle,
										 Caption.w_str(),
                               Caption.Length(),
                               &DrawRegRect,
//                               &TRect(0, 0, ((TStringGrid *)Sender)->ColWidths[1], 0),
										 DT_CALCRECT|DT_WORDBREAK|DT_NOPREFIX,
                               NULL) + 4;

   TRect CentredRect = DrawRegRect;
   int RowHeight = Rect.Bottom - Rect.Top;
   if (RowHeight > TextHeight)
   {
   	    CentredRect.Top += (RowHeight - TextHeight) / 2;
		CentredRect.Bottom = CentredRect.Top + TextHeight;
   }
	DrawTextExW(pCanvas->Handle, Caption.w_str(), Caption.Length(), &CentredRect, DT_WORDBREAK|DT_NOPREFIX, NULL);
	pCanvas->Brush->Color = Color;
	pCanvas->Brush->Style = Style;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPhoenixRoom::CardSwipe(TMessage& Message)
{
/*	AnsiString Data = *((AnsiString *)Message.WParam);
	int SearchKey = Contacts->GetContactByCard(Data.SubString(1,80));
	if(SearchKey != 0)
	{
		TMMContactInfo Info;
		Contacts->GetContactDetails(SearchKey,Info);
		if(Contacts->ContactType == eERSMember)
		{
			SearchKey = Info.Contact3rdPartyKey;
		}
	}
	else
	{
		int SearchKey = Contacts->GetContactByProx(Data.SubString(1,80));
		if(SearchKey != 0)
		{
			TMMContactInfo Info;
			Contacts->GetContactDetails(SearchKey,Info);
			if(Contacts->ContactType == eERSMember)
			{
				SearchKey = Info.Contact3rdPartyKey;
			}
		}
	}

	if(SearchKey != 0)
	{
		SetSelectedMember(SearchKey);
		bool ItemFound = false;
		for (int i = LastSearchIndex; (i < List->RowCount) && !ItemFound; i++)
		{
			if(int(List->Objects[0][i]) == SearchKey)
			{
				List->Row = i;
				ItemFound = true;
			}
		}

	}*/
}

void TfrmPhoenixRoom::RefreshList()
{
    if(TGlobalSettings::Instance().PMSType == Oracle)
    {
        List->RowCount = roomResult.RoomInquiryItem.size();
        List->Cells[0][0] = "";
        bool CommitTransaction = false;
        int Row = 0;
        int SelectedRow = 0;
        List->Cols[0] = SelectedRoom.Folders;
        List->Row = SelectedRow;
        List->SetFocus();
    }
    else if(TGlobalSettings::Instance().PMSType != SiHot)
    {
        List->RowCount = SelectedRoom.Folders->Count;
        List->Cells[0][0] = "";

        bool CommitTransaction = false;
        int Row = 0;
        int SelectedRow = 0;

        List->Cols[0] = SelectedRoom.Folders;
        List->Row = SelectedRow;
        List->SetFocus();
    }
    else if(TGlobalSettings::Instance().PMSType == SiHot)
    {
        List->RowCount = SiHotAccounts.size();
        List->Cells[0][0] = "";
        bool CommitTransaction = false;
        int Row = 0;
        int SelectedRow = 0;
        List->Cols[0] = SelectedRoom.Folders;
        List->Row = SelectedRow;
        List->SetFocus();
    }
}

void TfrmPhoenixRoom::DisplayData()
{
    memText->Clear();
    if(roomResult.RoomInquiryItem.size() != 0 && TGlobalSettings::Instance().PMSType == Oracle)
    {
       AnsiString Balance = "0.0";
       SelectedRoom.Folders->Clear();
       memText->Lines->Add("Room Number : " + roomResult.RoomInquiryItem[0].RoomNumber);
       memText->Lines->Add("Balance   : " + Balance);
       memText->Lines->Add("Credit Limit : " + roomResult.RoomInquiryItem[0].CreditLimit);
       for(std::vector<TRoomInquiryItem>::iterator itRoomResult = roomResult.RoomInquiryItem.begin();
          itRoomResult != roomResult.RoomInquiryItem.end(); ++itRoomResult)
       {
           SelectedRoom.Folders->Add(itRoomResult->FirstName + " " + itRoomResult->LastName);
       }
       RefreshList();
    }
	else if(SelectedRoom.AccountNumber != 0 && TGlobalSettings::Instance().PMSType != SiHot)
	{
		AnsiString Balance = CurrToStrF(SelectedRoom.Balance, ffCurrency, 2);
		AnsiString CreditLimit = CurrToStrF(SelectedRoom.CreditLimit, ffCurrency, 2);
		memText->Lines->Add("Room Number : " + SelectedRoom.AccountNumber);
		memText->Lines->Add("Balance   : " + Balance);
		memText->Lines->Add("Credit Limit : " + CreditLimit);
		RefreshList();
	}
    else if(SiHotAccounts.size() != 0 && TGlobalSettings::Instance().PMSType == SiHot)
    {
          SelectedRoom.Folders->Clear();
          bool displayed = false;
          for(std::vector<TSiHotAccounts>::iterator i = SiHotAccounts.begin();
             i != SiHotAccounts.end() ; ++i)
          {
              for(std::vector<TAccountDetails>::iterator j = i->AccountDetails.begin();
               j != i->AccountDetails.end() ;)
              {
                AnsiString Balance = j->Balance;
                AnsiString CreditLimit = j->CreditLimit;
                AnsiString RoomNumber = edSearch->Text;
                SelectedRoom.Folders->Add(j->FirstName + " " + j->LastName);
                if(!displayed)
                {
                    LimitSiHot = (double)((StrToCurr)(SiHotAccounts[0].AccountDetails[0].CreditLimit));
                    memText->Lines->Add("Room Number : " + RoomNumber);
                    memText->Lines->Add("Balance   : " + Balance);
                    memText->Lines->Add("Credit Limit : " + CreditLimit);
                    displayed = true;
                }
                std::advance(j,1);
              }
          }
          RefreshList();
    }
	else
	{
		List->RowCount = 1;
		List->Cells[0][0] = "";
	}
	memText->SelStart = 0;
}

void __fastcall TfrmPhoenixRoom::btnUserUpMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	__int64 TickCount = ::GetTickCount();
   int ScrollCount = 0;
//	while (ScrollCount < List->VisibleRowCount-1)
	while (ScrollCount < (int(List->Height / List->DefaultRowHeight) -1))
   {
      if (::GetTickCount() > TickCount + 20)
      {
         TickCount = ::GetTickCount();
         SendMessage(List->Handle, WM_VSCROLL, SB_LINEUP, 0L);
         ScrollCount++;
         if (List->TopRow == 0)
         {
         	break;
         }
      }
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmPhoenixRoom::btnUserDownMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	__int64 TickCount = ::GetTickCount();
   int ScrollCount = 0;
	while (ScrollCount < (int(List->Height / List->DefaultRowHeight) -1))
   {
		if (::GetTickCount() > TickCount + 20)
      {
         TickCount = ::GetTickCount();
         SendMessage(List->Handle, WM_VSCROLL, SB_LINEDOWN, 0L);
         ScrollCount++;
         if (List->TopRow == List->RowCount - List->VisibleRowCount)
         {
         	break;
         }
      }
   }
}
//---------------------------------------------------------------------------


void __fastcall TfrmPhoenixRoom::ListClick(TObject *Sender)
{
	edSearch->SelectAll();
	SetSelectedFolder(List->Row+1);
}
//---------------------------------------------------------------------------
void __fastcall TfrmPhoenixRoom::btnRoomsLeftMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (pnlLastPicked->Left < 0)
	{
		SlideRight();
	}
}

void __fastcall TfrmPhoenixRoom::btnRoomsRightMouseDown(TObject *Sender,
		TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (pnlLastPicked->Left > pnlScroll->ClientWidth - pnlLastPicked->Width)
	{
		SlideLeft();
	}
}
//---------------------------------------------------------------------------
void TfrmPhoenixRoom::SlideLeft()
{
	const int SCROLL_TIME	= 400;

	__int64 StartTickCount		= ::GetTickCount();
	__int64 TargetTickCount	= StartTickCount + SCROLL_TIME;
	if (StartTickCount > TargetTickCount) return; //Time wrapped

	int StartPosition			= pnlLastPicked->Left;
	int TargetOffset			= pnlScroll->Width;
	int LastOffset				= 0;

	__int64 CurrentTickCount	= ::GetTickCount();
	while (CurrentTickCount < TargetTickCount && CurrentTickCount >= StartTickCount)
	{
		int Offset = ((CurrentTickCount - StartTickCount) * TargetOffset) / SCROLL_TIME;
		if (Offset != LastOffset)
		{
			if (StartPosition - Offset < pnlScroll->ClientWidth - pnlLastPicked->Width)
			{
				pnlLastPicked->Left = pnlScroll->ClientWidth - pnlLastPicked->Width;
				break;
			}
			else
			{
				pnlLastPicked->Left = StartPosition - Offset;
			}

			pnlScroll->Update();
			LastOffset = Offset;
		}
		CurrentTickCount = ::GetTickCount();
	}
}
//---------------------------------------------------------------------------
void TfrmPhoenixRoom::SlideRight()
{
	const int SCROLL_TIME	= 400;

	__int64 StartTickCount		= ::GetTickCount();
	__int64 TargetTickCount	= StartTickCount + SCROLL_TIME;
	if (StartTickCount > TargetTickCount) return; //Time wrapped

	int StartPosition			= pnlLastPicked->Left;
	int TargetOffset			= pnlScroll->Width;
	int LastOffset				= 0;

	__int64 CurrentTickCount	= ::GetTickCount();
	while (CurrentTickCount < TargetTickCount && CurrentTickCount >= StartTickCount)
	{
		int Offset = ((CurrentTickCount - StartTickCount) * TargetOffset) / SCROLL_TIME;
		if (Offset != LastOffset)
		{
			if (StartPosition + Offset > 0)
			{
				pnlLastPicked->Left = 0;
				break;
			}
			else
			{
				pnlLastPicked->Left = StartPosition + Offset;
			}

			pnlScroll->Update();
			LastOffset = Offset;
		}
		CurrentTickCount = ::GetTickCount();
	}
}

void __fastcall TfrmPhoenixRoom::QuickClick(TObject *Sender)
{
	TTouchBtn *Button = (TTouchBtn * )Sender;
	edSearch->Text = Button->Caption;
	QuickSearch();
}


//---------------------------------------------------------------------------

void TfrmPhoenixRoom::ShowAccount()
{
	DisplayData();
	SetSelectedFolder(-1);
}

int TfrmPhoenixRoom::SetSelectedFolder(int FolderNumber)
{
    if(FolderNumber != -1)
    {
        memText->Clear();
        AnsiString Balance = "0.0";
        if(TGlobalSettings::Instance().PMSType == Oracle)
        {
            LimitSiHot = (double)((StrToCurr)(roomResult.RoomInquiryItem[FolderNumber-1].CreditLimit));
            memText->Lines->Add("Room Number : " + roomResult.RoomInquiryItem[FolderNumber-1].RoomNumber);
            memText->Lines->Add("Balance   : " + Balance);
            memText->Lines->Add("Credit Limit : " + roomResult.RoomInquiryItem[FolderNumber-1].CreditLimit);
            SelectedRoom.SiHotRoom = "";
            SelectedRoom.FolderNumber = FolderNumber;
        }
        else if(TGlobalSettings::Instance().PMSType == SiHot)
        {
            SelectedRoom.AccountNumber = SiHotAccounts[FolderNumber-1].AccountNumber;
            SelectedRoom.FolderNumber = FolderNumber;
            SelectedRoom.SiHotRoom = SiHotAccounts[FolderNumber-1].AccountDetails[0].RoomBedNumber;//edSearch->Text;
            LimitSiHot = (double)((StrToCurr)(SiHotAccounts[FolderNumber-1].AccountDetails[0].CreditLimit));
            memText->Clear();
            memText->Lines->Add("Room Number : " + SiHotAccounts[FolderNumber-1].AccountDetails[0].RoomBedNumber);
            memText->Lines->Add("Balance   : " + Balance);
            memText->Lines->Add("Credit Limit : " + SiHotAccounts[FolderNumber-1].AccountDetails[0].CreditLimit);
            SelectionVisible = true;
        }
        else
        {
            SelectedRoom.FolderNumber = FolderNumber;
            SelectedRoom.SiHotRoom = "";
        }
            tbtnOk->Enabled = true;
            tbtnOk->Caption = "Select\r" + List->Cols[0]->Strings[FolderNumber-1];
            SelectionVisible = true;
    }
	else
	{
		tbtnOk->Caption = "Select\r";
		tbtnOk->Enabled = false;
		SelectionVisible = false;
	}
	List->Repaint();
	return SelectedRoom.FolderNumber;
}

int TfrmPhoenixRoom::SelectRoom(AnsiString inPMSIPAddress,int inPMSPort)
{
	int RetVal = 0;
	try
	{
        PMSIPAddress   = inPMSIPAddress;
        PMSPort        = inPMSPort;
        RetVal = ShowModal();
        if(RetVal == mrOk)
        {
            if(find(LastAccessed.begin(),LastAccessed.end(),SelectedRoom.AccountNumber) == LastAccessed.end())
            {
                LastAccessed.insert(LastAccessed.begin(),SelectedRoom.AccountNumber);
            }
            while(LastAccessed.size() > MaxRemembered)
            {
                LastAccessed.pop_back();
            }
        }
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
	return RetVal;
}

void __fastcall TfrmPhoenixRoom::btnNumPadClick(TObject *Sender)
{
	if(DefaultInterfaceDisplay == eMemKeypad)
	{
		DefaultInterfaceDisplay = eMemNumPad;
	}
	else
	{
		DefaultInterfaceDisplay = eMemKeypad;
	}
	UpdateInterface();
}
//---------------------------------------------------------------------------


void TfrmPhoenixRoom::UpdateInterface()
{
	if(DefaultInterfaceDisplay == eMemKeypad)
	{
		pnlKeys->Visible = true;
		pnlKeys->Align = alBottom;
		pnlNumPad->Visible = false;
		pnlNumPad->Align = alNone;
	}
	else
	{
		pnlKeys->Visible = false;
		pnlKeys->Align = alNone;
		pnlNumPad->Visible = true;
		pnlNumPad->Align = alBottom;
	}
}


