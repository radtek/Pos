//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SelectRoom.h"
#include "MMMessageBox.h"
#include "GUIScale.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchControls"
#pragma resource "*.dfm"

char RoomButtonChar[24][2] = { {'~', '`'}, {'!', '1'}, {'@', '2'}, {'#', '3'}, {'$', '4'}, {'%', '5'}, {'^', '6'},
	{'&', '7'}, {'*', '8'}, {'(', '9'}, {')', '0'}, {'_', '-'}, {'+', '='}, {'{', '['}, {'}', ']'}, {'|', '\\'},
	{':', ';'}, {'"', '\''}, {'<', ','}, {'>', '.'}, {'?', '/'} };

//---------------------------------------------------------------------------
__fastcall TfrmSelectRoom::TfrmSelectRoom(TComponent* Owner,Database::TDBTransaction &inDBTransaction)
	: TZForm(Owner), DBTransaction(inDBTransaction)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectRoom::FormCreate(TObject *Sender)
{
	RoomsList = new TListRoomContainer;
	StartWithShiftDown = false;
	SelectedRoom = new TRoomData;
	LastSearchIndex = 0;

	pnlRooms->Left = 0;
	for (int i=0; i<pnlRooms->ControlCount; i++)
	{
	  ((TTouchBtn *)pnlRooms->Controls[i])->Color = clMaroon;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectRoom::FormDestroy(TObject *Sender)
{
	delete RoomsList;
	delete SelectedRoom;
	for (int i=0; i<pnlRooms->ControlCount; i++)
	{
		delete pnlRooms->Controls[i];
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectRoom::FormShow(TObject *Sender)
{
	FormResize(this);
	ShiftDown = StartWithShiftDown;
	CapsDown = false;
	tbtnOk->Enabled = false;
	DrawUpDownPanel(ShiftDown, pnlShift1);
	DrawUpDownPanel(CapsDown, pnlCapsLock);
	DrawLetters();
	edSearch->Text = SearchString;
	edSearch->SetFocus();
	CurrentRoomKey = -1;
	LastSearchIndex = 0;
	SearchType = DefaultSeatchType;
	SelectionVisible = false;
	memText->Clear();

	RefreshDisplay();

	TRoomData *TempRoom = new TRoomData;
	int ButtonIndex = 0;
	TTouchBtn *RoomsButton = NULL;

	for (int i = 0; i < TRooms::Instance().LastRoomsAccessed.size() ; i++)
	{
		if(RoomsList->GetByRoomKey(TRooms::Instance().LastRoomsAccessed[i]))
		{
			if(TDBRooms::GetRoom(DBTransaction,TRooms::Instance().LastRoomsAccessed[i],TempRoom))
			{
				RoomsButton						= new TTouchBtn(this);
				RoomsButton->Color			= clMaroon;
				RoomsButton->Width 			= (pnlScrollRooms->ClientWidth - 6) / 4 - 6;
				RoomsButton->Height 			= pnlRooms->ClientHeight;
				RoomsButton->Top 				= 0;
				RoomsButton->Left 			= ((ButtonIndex) * (RoomsButton->Width + 6));
				RoomsButton->Tag 				= TempRoom->RoomKey;
				RoomsButton->Caption 		= TempRoom->Caption;
				RoomsButton->OnMouseClick	= QuickRoomClick;
				RoomsButton->Parent 			= pnlRooms;
				RoomsButton->ParentFont		= true;
				ButtonIndex++;
			}
		}
	}

	if(RoomsButton != NULL)
	{
		pnlRooms->Width = RoomsButton->Left + RoomsButton->Width + 6;
	}
	delete TempRoom;

}

//---------------------------------------------------------------------------

void __fastcall TfrmSelectRoom::FormClose(TObject *Sender,
		TCloseAction &Action)
{
	StartWithShiftDown = false;
	MustHaveValidRoom = false;
	Filter = eRoomFilterOccupied;
	for (int i=0; i<pnlRooms->ControlCount; i++)
	{
		delete pnlRooms->Controls[i];
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectRoom::ShiftClick(TObject *Sender)
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
void TfrmSelectRoom::DrawUpDownPanel(bool Down, TPanel *Panel)
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
void __fastcall TfrmSelectRoom::ButtonMouseUp(TObject *Sender,
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
				RoomButtonChar[((TComponent *)Sender)->Tag-1][ShiftDown?0:1], 0);
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

void __fastcall TfrmSelectRoom::ButtonMouseDown(TObject *Sender,
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
void __fastcall TfrmSelectRoom::CapsLockClick(TObject *Sender)
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
void TfrmSelectRoom::DrawLetters()
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
void __fastcall TfrmSelectRoom::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
   {
      int Temp = Tag;
		Tag = Screen->Width;
                if((double)Screen->Width / Screen->Height < 1.4)
                {
			ScaleBy(Screen->Width, Temp);
                }
                else
                {
                	GUIScale::ParentWidth(Panel16, 0.15);
                        GUIScale::ParentWidth(Panel2, 0.4);
                        GUIScale::ParentWidth(Panel22, 0.45);
                }

      GUIScale::ParentHeight(pnlKeys, 0.5);
      Top = 0;
      Left = 0;
      Width = Screen->Width;
      Height = Screen->Height;
      Align = alClient;

      int CurrentPanelNumber = 0;
      int Gap = 4 * Screen->Width / Temp;
      int PanelHeight,
      	 PanelWidth;
      int BackSpaceWidth,
			 TabWidth,
          CapsWidth,
          EnterWidth,
          LShiftWidth,
          RShiftWidth,
			 SpaceWidth;
      int LastWidth = 0;

      if (Screen->Width >= 1024)
      {
			PanelHeight = 65;
         PanelWidth = 65;

         BackSpaceWidth = 97;
         TabWidth = 97;
         CapsWidth = 125;
         EnterWidth = 105;
         LShiftWidth = 153;
         RShiftWidth = 145;
         SpaceWidth = 677;

         Gap = 4;
      }
		else if (Screen->Width >= 800)
      {
      	PanelHeight = 50;
         PanelWidth = 50;

			BackSpaceWidth = 75;
         TabWidth = 75;
         CapsWidth = 97;
         EnterWidth = 82;
         LShiftWidth = 119;
			RShiftWidth = 113;
         SpaceWidth = 528;

         Gap = 3;
      }
      else if (Screen->Width >= 640)
      {
      	PanelHeight = 40;
         PanelWidth = 40;

         BackSpaceWidth = 60;
         TabWidth = 60;
         CapsWidth = 78;
			EnterWidth = 65;
         LShiftWidth = 95;
         RShiftWidth = 90;
         SpaceWidth = 423;

			Gap = 2;
      }

      Panel8->Width = PanelWidth;
      Panel8->Height = PanelHeight;
		TPoint Origin(Panel8->Left & 0xFFFC, Panel8->Top & 0xFFFC);
      TPoint LastPanelPos(Panel8->Left & 0xFFFC, Panel8->Top & 0xFFFC);

      while (CurrentPanelNumber < 54)
      {
			for (int i=0; i<pnlKeys->ControlCount; i++)
         {
            TPanel *Panel = (TPanel *)pnlKeys->Controls[i];
            if (Panel->TabOrder == CurrentPanelNumber)
            {
               Panel->Height = PanelHeight;
               if (CurrentPanelNumber == 0)
               {
						Panel->Left = Origin.x;
                  Panel->Top = Origin.y;
               }
               else
               {
						Panel->Left = LastPanelPos.x + LastWidth + Gap;
                  Panel->Top = LastPanelPos.y;
               }
               switch (CurrentPanelNumber)
               {
						case 13:
                     LastPanelPos.x = Origin.x - PanelWidth - Gap;
                     LastPanelPos.y = Origin.y + PanelHeight + Gap;
               		Panel->Width = BackSpaceWidth;
							LastWidth = PanelWidth;
                     break;
                  case 14:
                     LastPanelPos.x = Panel->Left;
                     LastPanelPos.y = Panel->Top;
               		Panel->Width = TabWidth;
							LastWidth = Panel->Width;
                     break;
                  case 27:
							LastPanelPos.x = Origin.x - PanelWidth - Gap;
                     LastPanelPos.y = Origin.y + ((PanelHeight + Gap) * 2);
               		Panel->Width = PanelWidth;
							LastWidth = PanelWidth;
                     break;
						case 28:
                     LastPanelPos.x = Panel->Left;
                     LastPanelPos.y = Panel->Top;
               		Panel->Width = CapsWidth;
							LastWidth = Panel->Width;
							break;
                  case 40:
                     LastPanelPos.x = Origin.x - PanelWidth - Gap;
                     LastPanelPos.y = Origin.y + ((PanelHeight + Gap) * 3);
               		Panel->Width = EnterWidth;
							LastWidth = PanelWidth;
                     break;
                  case 41:
                     LastPanelPos.x = Panel->Left;
                     LastPanelPos.y = Panel->Top;
               		Panel->Width = LShiftWidth;
							LastWidth = Panel->Width;
                     break;
						case 52:
                     LastPanelPos.x = Origin.x;
                     LastPanelPos.y = Origin.y + ((PanelHeight + Gap) * 4);
               		Panel->Width = RShiftWidth;
							LastWidth = pnlShift1->Width;
							break;
                  case 53:
                     LastPanelPos.x = Panel->Left;
                     LastPanelPos.y = Panel->Top;
               		Panel->Width = SpaceWidth;
							LastWidth = Panel->Width;
                     break;
                  default:
                     LastPanelPos.x = Panel->Left;
                     LastPanelPos.y = Panel->Top;
               		Panel->Width = PanelWidth;
               		LastWidth = Panel->Width;
                     break;
               }
	            break;
            }
			}
         CurrentPanelNumber++;
		}

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

		pnlScrollRooms->Left = btnRoomsLeft->Left + btnRoomsLeft->Width + BorderWidth;
		pnlScrollRooms->Height = btnRoomsLeft->Height;
		pnlScrollRooms->Top = btnRoomsLeft->Top;
		pnlScrollRooms->Width = btnRoomsRight->Left - pnlScrollRooms->Left - BorderWidth;
		pnlRooms->Height = pnlScrollRooms->Height;
		pnlRooms->Top = 0;
       	}
}

void __fastcall TfrmSelectRoom::Panel1Resize(TObject *Sender)
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

		pnlScrollRooms->Left = btnRoomsLeft->Left + btnRoomsLeft->Width + BorderWidth;
		pnlScrollRooms->Height = btnRoomsLeft->Height;
		pnlScrollRooms->Top = btnRoomsLeft->Top;
		pnlScrollRooms->Width = btnRoomsRight->Left - pnlScrollRooms->Left - BorderWidth;
		pnlRooms->Height = pnlScrollRooms->Height;
		pnlRooms->Top = 0;

}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TfrmSelectRoom::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(this);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectRoom::btnOkClick(TObject *Sender)
{
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


void __fastcall TfrmSelectRoom::FormCloseQuery(TObject *Sender,
		bool &CanClose)
{
	if(MustHaveValidRoom && SelectedRoom->RoomKey == 0)
	{
		MessageBox("A Valid Room must be suppiled", "Please enter a value", MB_OK + MB_ICONINFORMATION);
		CanClose = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectRoom::tbtnCancelClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectRoom::tbtnClearClick(TObject *Sender)
{
	memText->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectRoom::edSearchKeyPress(TObject *Sender,
		char &Key)
{
	if (Key == VK_RETURN)
	{
		QuickSearch(edSearch->Text);
		edSearch->SelectAll();
		DisplayRoomData();		
	}
	else
	{
		LastSearchIndex = 0;
	}
}
//---------------------------------------------------------------------------


void TfrmSelectRoom::QuickSearch(AnsiString SearchString)
{
	
	SearchString = edSearch->Text;
	bool ItemFound = false;
	for (int i = LastSearchIndex; (i < RoomsList->Count) && !ItemFound; i++)
	{
		TRoomData *Room = RoomsList->Get(i);
		if(UpperCase(Room->Caption).Pos(UpperCase(SearchString)) != 0)
		{
			StringGrid1->Row = i;
			StringGrid1->TopRow = i; // Force refocus on the screen			
			LastSearchIndex = i+1;
			ItemFound = true;
		}
	}

	if(!ItemFound && LastSearchIndex > 0)
	{
		LastSearchIndex = 0;
		for (int i = LastSearchIndex; (i < RoomsList->Count) && !ItemFound; i++)
		{
			TRoomData *Room = RoomsList->Get(i);
			if(UpperCase(Room->Caption).Pos(UpperCase(SearchString)) != 0)
			{
				StringGrid1->Row = i;
				StringGrid1->TopRow = i; // Force refocus on the screen
				LastSearchIndex = i+1;
				ItemFound = true;
			}
		}
	}
	for (int i=0; i<pnlRooms->ControlCount; i++)
	{
		TTouchBtn *UnSelected = (TTouchBtn * )pnlRooms->Controls[i];
		UnSelected->Color = clMaroon;
	}
}

void __fastcall TfrmSelectRoom::tbtnSearchByNameClick(TObject *Sender)
{
	SearchType = eSeachRoomByName;
	tbtnSearchByName->Color = clGreen;
	tbtnSearchByNumber->Color = clMaroon;
	tbtnSearchByGuest->Color = clMaroon;
	DefaultSeatchType = SearchType;
	RefreshDisplay();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectRoom::tbtnSearchByNumberClick(TObject *Sender)
{
	SearchType = eSeachRoomByNumber;
	tbtnSearchByName->Color = clMaroon;
	tbtnSearchByNumber->Color = clGreen;
	tbtnSearchByGuest->Color = clMaroon;
	DefaultSeatchType = SearchType;
	RefreshDisplay();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectRoom::tbtnSearchByGuestClick(TObject *Sender)
{
	SearchType = eSeachRoomByGuest;
	tbtnSearchByName->Color = clMaroon;
	tbtnSearchByNumber->Color = clMaroon;
	tbtnSearchByGuest->Color = clGreen;
	DefaultSeatchType = SearchType;	
	RefreshDisplay();	
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectRoom::StringGrid1DrawCell(TObject *Sender,
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

	UnicodeString Caption = StringGrid1->Cells[ACol][ARow];
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
	DrawTextExW(pCanvas->Handle, Caption.c_str(), Caption.Length(), &CentredRect, DT_WORDBREAK|DT_NOPREFIX, NULL);
	pCanvas->Brush->Color = Color;
	pCanvas->Brush->Style = Style;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectRoom::CardSwipe(TMessage& Message)
{
	AnsiString Data = *((AnsiString *)Message.WParam);
}

void TfrmSelectRoom::RefreshDisplay()
{
	SearchString = "";
	StringGrid1->RowCount = 1;
	StringGrid1->Cells[0][0] = "";
	StringGrid1->Objects[0][0] = NULL;	

	int Row = 0;
	int SelectedRow = 0;

	if(SearchType == eSeachRoomByName)
	{
		TDBRooms::GetList(DBTransaction,RoomsList,Filter);
	}
	else if (SearchType == eSeachRoomByNumber)
	{
		TDBRooms::GetListNumeric(DBTransaction,RoomsList,Filter);
	}
	else if (SearchType == eSeachRoomByGuest)
	{
		TDBRooms::GetListGuest(DBTransaction,RoomsList,Filter);
	}


	for (int i = 0; i < RoomsList->Count; i++)
	{
		TRoomData *Room = RoomsList->Get(i);

		if (Room->RoomKey == LastRoomKey)
		{
			SelectedRow = Row;
		}
		StringGrid1->Objects[0][Row] = Room;
		StringGrid1->Cells[0][Row++] = Room->Caption;
		StringGrid1->RowCount++;
	}
	StringGrid1->RowCount--;
	StringGrid1->Row = SelectedRow;
	StringGrid1->SetFocus();
	SetSelectedRoom((TRoomData *)(StringGrid1->Objects[0][StringGrid1->Row]));	
}

void TfrmSelectRoom::DisplayRoomData()
{
	TRoomData *Room = SelectedRoom;
	if(Room->RoomNo != 0)
	{
		memText->Clear();
		memText->Lines->Add("Room Number : " + IntToStr(Room->RoomNo));
		memText->Lines->Add("Room Name   : " + Room->Name);
		memText->Lines->Add("Guest Name  : " + Room->PartyName);
		AnsiString Status;
		switch (Room->Status)
		{
			case eOccupied : Status = "Occupied";
			break;
			case eVacant : Status = "Vacant";
			break;
		}                   
		memText->Lines->Add("Status      : " + Status);

	}
	memText->SelStart = 0;
}


void __fastcall TfrmSelectRoom::tbtnFullSearchClick(TObject *Sender)
{
	
	SearchString = edSearch->Text;
	bool ItemFound = false;
	for (int i = LastSearchIndex; (i < RoomsList->Count) && !ItemFound; i++)
	{
		TRoomData *Room = RoomsList->Get(i);
		if(UpperCase(Room->Caption).Pos(UpperCase(SearchString)) != 0)
		{
			StringGrid1->Row = i;
			LastSearchIndex = i+1;
			ItemFound = true;
		}
	}

	// Have a look though the rest of the Accessable Data for a match.
	if(!ItemFound)
	{
		RoomsList->SortByGuest();
		for (int m = 0; (m < RoomsList->Count) && !ItemFound; m++)
		{
			TRoomData *Room = RoomsList->Get(m);
			if(UpperCase(Room->PartyName).Pos(UpperCase(SearchString)) != 0)
			{
				StringGrid1->Row = m;
				LastSearchIndex = m+1;
				ItemFound = true;
			}
		}

		if(ItemFound)
		{
			tbtnSearchByGuestClick(NULL);
			StringGrid1->Row = LastSearchIndex-1;			
		}
		else
		{
			RoomsList->SortByRoomName();
			for (int j = 0; (j < RoomsList->Count) && !ItemFound; j++)
			{
				TRoomData *Room = RoomsList->Get(j);
				if(UpperCase(Room->Name).Pos(UpperCase(SearchString)) != 0)
				{
					StringGrid1->Row = j;
					LastSearchIndex = j+1;
					ItemFound = true;
				}
			}

			if(ItemFound)
			{
				tbtnSearchByNameClick(NULL);
				StringGrid1->Row = LastSearchIndex-1;
			}
			else
			{
				RoomsList->SortByRoomNumber();
				for (int k = 0; (k < RoomsList->Count) && !ItemFound; k++)
				{
					TRoomData *Room = RoomsList->Get(k);
					if(UpperCase(IntToStr(Room->RoomNo)).Pos(UpperCase(SearchString)) != 0)
					{
						StringGrid1->Row = k;
						LastSearchIndex = k+1;
						ItemFound = true;
					}
				}

				if(ItemFound)
				{
					tbtnSearchByNumberClick(NULL);
					StringGrid1->Row = LastSearchIndex-1;
				}
			}
		}
	}
	edSearch->SelectAll();
	DisplayRoomData();

	for (int i=0; i<pnlRooms->ControlCount; i++)
	{
		TTouchBtn *UnSelected = (TTouchBtn * )pnlRooms->Controls[i];
		UnSelected->Color = clMaroon;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectRoom::StringGrid1Click(TObject *Sender)
{
	edSearch->SelectAll();
	SetSelectedRoom((TRoomData *)(StringGrid1->Objects[0][StringGrid1->Row]));
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectRoom::btnRoomsLeftMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (pnlRooms->Left < 0)
	{
		SlideRoomsRight();
	}
}

void __fastcall TfrmSelectRoom::btnRoomsRightMouseDown(TObject *Sender,
		TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (pnlRooms->Left > pnlScrollRooms->ClientWidth - pnlRooms->Width)
	{
		SlideRoomsLeft();
	}
}
//---------------------------------------------------------------------------
void TfrmSelectRoom::SlideRoomsLeft()
{
	const int SCROLL_TIME	= 400;

	__int64 StartTickCount		= ::GetTickCount();
	__int64 TargetTickCount	= StartTickCount + SCROLL_TIME;
	if (StartTickCount > TargetTickCount) return; //Time wrapped

	int StartPosition			= pnlRooms->Left;
	int TargetOffset			= pnlScrollRooms->Width;
	int LastOffset				= 0;

	__int64 CurrentTickCount	= ::GetTickCount();
	while (CurrentTickCount < TargetTickCount && CurrentTickCount >= StartTickCount)
	{
		int Offset = ((CurrentTickCount - StartTickCount) * TargetOffset) / SCROLL_TIME;
		if (Offset != LastOffset)
		{
			if (StartPosition - Offset < pnlScrollRooms->ClientWidth - pnlRooms->Width)
			{
				pnlRooms->Left = pnlScrollRooms->ClientWidth - pnlRooms->Width;
				break;
			}
			else
			{
				pnlRooms->Left = StartPosition - Offset;
			}

			pnlScrollRooms->Update();
			LastOffset = Offset;
		}
		CurrentTickCount = ::GetTickCount();
	}
}
//---------------------------------------------------------------------------
void TfrmSelectRoom::SlideRoomsRight()
{
	const int SCROLL_TIME	= 400;

	__int64 StartTickCount		= ::GetTickCount();
	__int64 TargetTickCount	= StartTickCount + SCROLL_TIME;
	if (StartTickCount > TargetTickCount) return; //Time wrapped

	int StartPosition			= pnlRooms->Left;
	int TargetOffset			= pnlScrollRooms->Width;
	int LastOffset				= 0;

	__int64 CurrentTickCount	= ::GetTickCount();
	while (CurrentTickCount < TargetTickCount && CurrentTickCount >= StartTickCount)
	{
		int Offset = ((CurrentTickCount - StartTickCount) * TargetOffset) / SCROLL_TIME;
		if (Offset != LastOffset)
		{
			if (StartPosition + Offset > 0)
			{
				pnlRooms->Left = 0;
				break;
			}
			else
			{
				pnlRooms->Left = StartPosition + Offset;
			}

			pnlScrollRooms->Update();
			LastOffset = Offset;
		}
		CurrentTickCount = ::GetTickCount();
	}
}

void __fastcall TfrmSelectRoom::QuickRoomClick(TObject *Sender)
{
	TTouchBtn *RoomsButton = (TTouchBtn * )Sender;
	int RoomKey = RoomsButton->Tag;
	RoomsButton->Color = clGreen;
	bool ItemFound = false;
	for (int i = 0; (i < RoomsList->Count) && !ItemFound; i++)
	{
		TRoomData *Room = RoomsList->Get(i);
		if(Room->RoomKey == RoomKey)
		{
			StringGrid1->Row = i;
			LastSearchIndex = i+1;
			ItemFound = true;
		}
	}

	for (int i=0; i<pnlRooms->ControlCount; i++)
	{
		if(pnlRooms->Controls[i] != Sender)
		{
			TTouchBtn *UnSelected = (TTouchBtn * )pnlRooms->Controls[i];
			UnSelected->Color = clMaroon;
		}
	}

	if(ItemFound)
	{
		SetSelectedRoom((TRoomData *)(StringGrid1->Objects[0][StringGrid1->Row]));
	}
}


//---------------------------------------------------------------------------

TRoomData * TfrmSelectRoom::SetSelectedRoom(TRoomData * inRoomSelected)
{
	if(inRoomSelected != NULL)
	{
		SelectedRoom->Assign(inRoomSelected);
		DisplayRoomData();
		tbtnOk->Enabled = true;
		tbtnOk->Caption = "Select\r" + SelectedRoom->Name;
		SelectionVisible = true;
		StringGrid1->Repaint();
	}
	return SelectedRoom;
}

void __fastcall TfrmSelectRoom::btnUserUpAutoRepeat(TObject *Sender)
{
	__int64 TickCount = ::GetTickCount();
   int ScrollCount = 0;
	while (ScrollCount < StringGrid1->VisibleRowCount-1)
   {
	  if (::GetTickCount() > TickCount + 20)
	  {
		 TickCount = ::GetTickCount();
		 SendMessage(StringGrid1->Handle, WM_VSCROLL, SB_LINEUP, 0L);
		 ScrollCount++;
		 if (StringGrid1->TopRow == 0)
		 {
			break;
		 }
	  }
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectRoom::btnUserDownAutoRepeat(TObject *Sender)
{
   __int64 TickCount = ::GetTickCount();
   int ScrollCount = 0;
   while (ScrollCount < StringGrid1->VisibleRowCount-1)
   {
		if (::GetTickCount() > TickCount + 20)
	  {
		 TickCount = ::GetTickCount();
		 SendMessage(StringGrid1->Handle, WM_VSCROLL, SB_LINEDOWN, 0L);
		 ScrollCount++;
		 if (StringGrid1->TopRow == StringGrid1->RowCount - StringGrid1->VisibleRowCount)
		 {
			break;
		 }
	  }
   }
}
//---------------------------------------------------------------------------

