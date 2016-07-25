//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SelectRMSRoom.h"
#include "MMMessageBox.h"
#include "MMLogging.h"
#include "CSV.h"
  #include "GlobalSettings.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchKeyboard"
#pragma link "TouchNumpad"
#pragma link "TouchPages"
#pragma resource "*.dfm"

char MemButtonChar[24][2] =
{
   {
	  '~', '`'
   },
   {
	  '!', '1'
   },
   {
	  '@', '2'
   },
   {
	  '#', '3'
   },
   {
	  '$', '4'
   },
   {
	  '%', '5'
   },
   {
	  '^', '6'
   },
   {
	  '&', '7'
   },
   {
	  '*', '8'
   },
   {
	  '(', '9'
   },
   {
	  ')', '0'
   },
   {
	  '_', '-'
   },
   {
	  '+', '='
   },
   {
	  '{', '['
   },
   {
	  '}', ']'
   },
   {
	  '|', '\\'
   },
   {
	  ':', ';'
   },
   {
	  '"', '\''
   },
   {
	  '<', ','
   },
   {
	  '>', '.'
   },
   {
	  '?', '/'
   }
};

// ---------------------------------------------------------------------------
__fastcall TfrmRMSRoom::TfrmRMSRoom(TComponent* Owner) : TZForm(Owner)
{
}

// ---------------------------------------------------------------------------
void __fastcall TfrmRMSRoom::FormCreate(TObject *Sender)
{
   StartWithShiftDown = false;
   pnlList->Height = Panel4->Height;
   IncrementalSearch = false;
   LastSearchIndex = 0;
   DefaultInterfaceDisplay = eMemKeypad;

}
// ---------------------------------------------------------------------------

void __fastcall TfrmRMSRoom::FormShow(TObject *Sender)
{
   FormResize(this);

   SelectedRoom.Clear();
   NewBookSelectedRoom.Clear();
   tbtnOk->Enabled = false;
   tbtnCancel->Enabled = true;
   tbtnCancel->Visible = true;
   tlistRooms->Visible = true;
   LastSearchIndex = 0;
   edSearch->Text = SearchString;
   SelectionVisible = false;

   if (SearchType == eRoomSearchByNumber)
   {
	  tbtnSearchByNumberClick(Sender);
   }
   else
   {
	  tbtnSearchByNameClick(Sender);
   }
   edSearch->SetFocus();



}

// ---------------------------------------------------------------------------

void __fastcall TfrmRMSRoom::FormClose(TObject *Sender, TCloseAction &Action)
{
   StartWithShiftDown = false;
   MustHaveValidResult = false;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmRMSRoom::FormResize(TObject *Sender)
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

   Panel4->Height = pnlKeyboard->Top;
   Panel8->Height = Panel3->Height;
   pnlList->Width = Panel4->Left -this->BorderWidth;
   pnlList->Left = this->BorderWidth;
   pnlList->Top = this->BorderWidth;

   Left = (Screen->Width - Width) / 2;
   Top = (Screen->Height - Height) / 2;

   // tlistRooms->ItemHeight = btnSearch->Height;
   // TDeviceRealTerminal::Instance().ManagerMembership->DefaultInterfaceDisplay == eMemKeypad
   // Always show Keypad first.
   if (DefaultInterfaceDisplay == eMemKeypad)
   {
	  pnlList->Height = Panel4->Height;
   }
   else
   {
	  pnlList->Height = Panel4->Height + pnlKeyboard->Height;
   }
   Realign();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmRMSRoom::WMDisplayChange(TWMDisplayChange& Message)
{
   FormResize(this);
}

// ---------------------------------------------------------------------------
void __fastcall TfrmRMSRoom::btnOkClick(TObject *Sender)
{
   ModalResult = mrOk;
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------

void __fastcall TfrmRMSRoom::FormCloseQuery(TObject *Sender, bool &CanClose)
{
   if ((TGlobalSettings::Instance().NewBook!=2)&&(MustHaveValidResult && SelectedRoom.AccountNo != -1))
   {
	  MessageBox("A Valid Room must be selected", "Please Select a Room", MB_OK + MB_ICONINFORMATION);
	  CanClose = false;
   }

}
// ---------------------------------------------------------------------------

void __fastcall TfrmRMSRoom::tbtnCancelClick(TObject *Sender)
{
   ModalResult = mrCancel;
}
// ---------------------------------------------------------------------------

void __fastcall TfrmRMSRoom::tbtnClearClick(TObject *Sender)
{
   lbMemberText->Clear();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmRMSRoom::edSearchKeyPress(TObject *Sender, char &Key)
{
   if (Key == VK_RETURN)
   {
	  QuickSearch(edSearch->Text);
	  edSearch->SelectAll();
   if(TGlobalSettings::Instance().NewBook==2)
     {DisplayNewBookRoomData();}
	 else{ DisplayData();  }

      //trip
   }
   else
   {
	  LastSearchIndex = 0;
   }
}
// ---------------------------------------------------------------------------

void TfrmRMSRoom::QuickSearch(AnsiString SearchString)
{
   SearchString = edSearch->Text;
   bool ItemFound = false;
   for (int i = LastSearchIndex; (i < tlistRooms->Count) && !ItemFound; i++)
   {
	  if (UpperCase(tlistRooms->Items->Strings[i]).Pos(UpperCase(SearchString)) != 0)
	  {
		 tlistRooms->Selected[i] = true;
		 // tgridMembers->SetVisibleRow(tgridMembers->Row(tgridMembers->Buttons[i][0]));
		 LastSearchIndex = i + 1;
		 ItemFound = true;

          if(TGlobalSettings::Instance().NewBook==2)
     {
     SetSelectedNewBookRoom(int(tlistRooms->Items->Objects[i]), false);
     }
	 else{ SetSelectedRoom(int(tlistRooms->Items->Objects[i]), false); }


		 //SetSelectedRoom(int(tlistRooms->Items->Objects[i]), false);
	  }
   }

   if (!ItemFound && LastSearchIndex > 0)
   {
	  LastSearchIndex = 0;
	  for (int i = LastSearchIndex; (i < tlistRooms->Items->Count) && !ItemFound; i++)
	  {
		 if (UpperCase(tlistRooms->Items->Strings[i]).Pos(UpperCase(SearchString)) != 0)
		 {
			tlistRooms->Selected[i] = true;
			// tgridMembers->Buttons[i][0]->Latched = true;
			// tgridMembers->SetVisibleRow(tgridMembers->Row(tgridMembers->Buttons[i][0]));
			LastSearchIndex = i + 1;
			ItemFound = true;
		  //	SetSelectedRoom(int(tlistRooms->Items->Objects[i]), false);


     if(TGlobalSettings::Instance().NewBook==2)
     {
       SetSelectedNewBookRoom(int(tlistRooms->Items->Objects[i]), false);
     }
	 else
     {
     SetSelectedRoom(int(tlistRooms->Items->Objects[i]), false);
     }




		 }
	  }
   }


}

void __fastcall TfrmRMSRoom::tbtnSearchByNameClick(TObject *Sender)
{
   SearchType = eRoomSearchByName;
   tbtnSearchByName->ButtonColor = clGreen;
   tbtnSearchByNumber->ButtonColor = clMaroon;
//   TDeviceRealTerminal::Instance().ManagerMembership->DefaultSearchType = SearchType;
   //RefreshDisplay();
        if(TGlobalSettings::Instance().NewBook==2)
     { RefreshNewBookDisplay();}
	 else{  RefreshDisplay(); }

   //trip
   edSearch->SetFocus();
//  TDeviceRealTerminal::Instance().ManagerMembership->DefaultInterfaceDisplay = eMemKeypad;
   pnlList->Height = Panel4->Height;
   UpdateInterface();

}
// ---------------------------------------------------------------------------

void __fastcall TfrmRMSRoom::tbtnSearchByNumberClick(TObject *Sender)
{
   SearchType = eRoomSearchByNumber;
   tbtnSearchByName->ButtonColor = clMaroon;
   tbtnSearchByNumber->ButtonColor = clGreen;
//   tbtnSearchByGuest->ButtonColor = clMaroon;
//   TDeviceRealTerminal::Instance().ManagerMembership->DefaultSearchType = SearchType;

   //trip
     if(TGlobalSettings::Instance().NewBook==2)
     { RefreshNewBookDisplay();}
	 else{  RefreshDisplay(); }

   edSearch->SetFocus();
//   TDeviceRealTerminal::Instance().ManagerMembership->DefaultInterfaceDisplay = eMemNumPad;
   pnlList->Height = Panel4->Height + pnlKeyboard->Height;
   UpdateInterface();

}
// ---------------------------------------------------------------------------


void __fastcall TfrmRMSRoom::CardSwipe(TMessage& Message)
{
/*   AnsiString Data = *((AnsiString*)Message.WParam);

   Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
   TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
   DBTransaction.StartTransaction();

   TMMContactInfo Info;
   Info.CardStr = Data.SubString(1, 80);
   int SearchKey = TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetContactByCard(DBTransaction, Info);
   if (SearchKey != 0)
   {
	  TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetContactDetails(DBTransaction, SearchKey, Info);
	  if (TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->ContactType == eERSMember)
	  {
		 SearchKey = Info.ExternalKey;
	  }
   }
   else
   {
	  int SearchKey = TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetContactByProx(DBTransaction, Data.SubString(1, 80));
	  if (SearchKey != 0)
	  {
		 TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetContactDetails(DBTransaction, SearchKey, Info);
		 if (TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->ContactType == eERSMember)
		 {
			SearchKey = Info.ExternalKey;
		 }
	  }
   }
   DBTransaction.Commit();

   if (SearchKey != 0)
   {
	  SetSelectedMember(SearchKey, false);
	  bool ItemFound = false;
	  for (int i = LastSearchIndex; (i < tlistRooms->Count) && !ItemFound; i++)
	  {
		 if (int(tlistRooms->Items->Objects[i]) == SearchKey)
		 {
			tlistRooms->Selected[i] = true;
			// tgridMembers->Buttons[i][0]->Latched = true;
			// tgridMembers->SetVisibleRow(tgridMembers->Row(tgridMembers->Buttons[i][0]));
			ItemFound = true;
		 }
	  }
   } */
}

void TfrmRMSRoom::RefreshDisplay()
{
	unsigned int sel_mbr = -1;
	tlistRooms->Items->BeginUpdate();
	tlistRooms->Clear();
	for(int i = 0; i < RoomsList.size(); i++)
	{
		AnsiString RoomLine = RoomsList[i].RoomNo + " " + RoomsList[i].Name;
		if(SearchType == eRoomSearchByNumber)
		{
			RoomLine = IntToStr(RoomsList[i].AccountNo) + " " + RoomsList[i].Name;
		}
		unsigned int ukey = RoomsList[i].AccountNo;
		tlistRooms->AddItem(RoomLine,reinterpret_cast<TObject *>(ukey));
	}

	tlistRooms->Items->EndUpdate();
	tlistRooms->Refresh();
	tlistRooms->Realign();
	tlistRooms->SetFocus();

	SetSelectedRoom(sel_mbr, 0);
}


void TfrmRMSRoom::RefreshNewBookDisplay()
{
	unsigned int sel_mbr = -1;
	tlistRooms->Items->BeginUpdate();
	tlistRooms->Clear();
	for(int i = 0; i < NewBookRoomsList.size(); i++)
	{
		AnsiString RoomLine ="Room"+ NewBookRoomsList[i].RoomNo ;
		if(SearchType == eRoomSearchByNumber)
		{
			RoomLine ="Room" +  NewBookRoomsList[i].RoomCount ;
		}
    	unsigned int ukey = NewBookRoomsList[i].RoomCount;
		tlistRooms->AddItem(RoomLine,reinterpret_cast<TObject *>(ukey));
	}

	tlistRooms->Items->EndUpdate();
	tlistRooms->Refresh();
	tlistRooms->Realign();
	tlistRooms->SetFocus();

	SetSelectedRoom(sel_mbr, 0);
}

void TfrmRMSRoom::DisplayData()
{
	if(SelectedRoom.AccountNo != -1)
   {
	  lbMemberText->Clear();
	  lbMemberText->Items->Add("Room Number : " + SelectedRoom.RoomNo);
	  lbMemberText->Items->Add("Room Name   : " + SelectedRoom.Name);
	  lbMemberText->Items->Add("Account Number : " + IntToStr(SelectedRoom.AccountNo));
   }
   else
   {
	  lbMemberText->Clear();
   }
}


void TfrmRMSRoom::DisplayNewBookRoomData()
{
	if(NewBookSelectedRoom.RoomNo != "")
   {
     UnicodeString status;
   if(NewBookSelectedRoom.Allow_Charge==false)
   {
   status="Vacant";
   }
   else
     {
     status="Closed";
     }
        lbMemberText->Clear();
        lbMemberText->Items->Add("Room Number : " + NewBookSelectedRoom.RoomNo);
        lbMemberText->Items->Add("Room Name   : Room" + NewBookSelectedRoom.RoomNo);
        lbMemberText->Items->Add("Guest Name   : " + NewBookSelectedRoom.Guest_Name);
        lbMemberText->Items->Add("Status   : " + status);
       TCustomer Customer(StrToInt(NewBookSelectedRoom.RoomNo), 0,  NewBookSelectedRoom.Guest_Name);

   }
   else
   {
	  lbMemberText->Clear();
   }
}

void __fastcall TfrmRMSRoom::tbtnFullSearchClick(TObject *Sender)
{
   /*
   SearchString = edSearch->Text;
   bool ItemFound = false;
   for (int i = LastSearchIndex; (i < List->RowCount) && !ItemFound; i++)
   {
   if(UpperCase(List->Cells[0][i]).Pos(UpperCase(SearchString)) != 0)
   {
   List->Row = i;
   LastSearchIndex = i+1;
   ItemFound = true;
   }
   }
   edSearch->SelectAll();
   Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
   TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
   DBTransaction.StartTransaction();
   DisplayData(DBTransaction);
   DBTransaction.Commit();


   for (int i=0; i<pnlLastPicked->ControlCount; i++)
   {
   TTouchButton *UnSelected = (TTouchButton * )pnlLastPicked->Controls[i];
   UnSelected->Color = clMaroon;
   } */
}
// ---------------------------------------------------------------------------

int TfrmRMSRoom::SetSelectedRoom(int inAccountNumber, bool filter)
{
	if (inAccountNumber != 0)
	{
		SelectedRoom.Clear();
		SelectedRoom.AccountNo = inAccountNumber;
		tpDetails->ActivePage = tsDetails;

		std::vector<TRMSRoom>::iterator  i = std::find(RoomsList.begin(), RoomsList.end(), SelectedRoom);
		if( i != RoomsList.end())
		{
			SelectedRoom = *i;
		   //	DisplayData();    trip
           if(TGlobalSettings::Instance().NewBook==2)
     {DisplayNewBookRoomData();}
	 else{ DisplayData();  }


			tbtnOk->Enabled = true;
			tbtnOk->Caption = "Select\r" + SelectedRoom.RoomNo;
		}
		SelectionVisible = true;
	}
	else
	{
		tbtnOk->Caption = "Ok";
		tbtnOk->Enabled = false;
		lbMemberText->Clear();
	}

   return SelectedRoom.AccountNo;
}

int TfrmRMSRoom::SetSelectedNewBookRoom(int inRoomNumber, bool filter)
{
	if (inRoomNumber != 0)
	{
		NewBookSelectedRoom.Clear();
		NewBookSelectedRoom.RoomCount = inRoomNumber;
		tpDetails->ActivePage = tsDetails;

		std::vector<TNewBookRoom>::iterator  i = std::find(NewBookRoomsList.begin(), NewBookRoomsList.end(), NewBookSelectedRoom);
		if( i != NewBookRoomsList.end())
		{
			NewBookSelectedRoom = *i;
		   //	DisplayData();    trip
           if(TGlobalSettings::Instance().NewBook==2)
     {DisplayNewBookRoomData();}
	 else{ DisplayData();  }


			tbtnOk->Enabled = true;
			tbtnOk->Caption = "Select\r" + NewBookSelectedRoom.RoomNo;
		}
		SelectionVisible = true;
	}
	else
	{
		tbtnOk->Caption = "Ok";
		tbtnOk->Enabled = false;
		lbMemberText->Clear();
	}

   return NewBookSelectedRoom.RoomCount;
}

void TfrmRMSRoom::UpdateInterface()
{
//   if (TDeviceRealTerminal::Instance().ManagerMembership->DefaultInterfaceDisplay == eMemKeypad)
   if (DefaultInterfaceDisplay == eMemKeypad)
   {
	  tbtnInterface->Caption = "NumPad";
	  TouchNumpad1->Visible = false;
	  TouchKeyboard1->Visible = true;

   }
   else
   {
	  tbtnInterface->Caption = "Keyboard";
	  TouchNumpad1->Visible = true;
	  TouchKeyboard1->Visible = false;
   }
}

void __fastcall TfrmRMSRoom::edSearchChange(TObject *Sender)
{
   if (IncrementalSearch)
   {
	  LastSearchIndex = 0;
	  QuickSearch(edSearch->Text);
	 // DisplayData(); //trip
     if(TGlobalSettings::Instance().NewBook==2)
     {DisplayNewBookRoomData();}
	 else{ DisplayData();  }


   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmRMSRoom::TouchKeyboard1Click(TObject *Sender, char Char)
{
   PostMessage(edSearch->Handle, WM_CHAR, Char, 0);
   edSearch->SetFocus();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmRMSRoom::btnSearchMouseClick(TObject *Sender)
{
   PostMessage(edSearch->Handle, WM_CHAR, '\r', 0);
   edSearch->SetFocus();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmRMSRoom::TouchNumpad2Click(TObject *Sender, TNumpadKey Key)
{
   if (Key <= nk9)
   {
	  PostMessage(edSearch->Handle, WM_CHAR, char(Key + 0x30), 0);
   }
   else if (Key == nkCustom)
   {
	  edSearch->Text = "";
   }
   else // nkClear
   {
	  edSearch->Text = "";
   }
   edSearch->SetFocus();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmRMSRoom::tbtnInterfaceMouseClick(TObject *Sender)
{
   if (DefaultInterfaceDisplay == eMemKeypad)
   {
	  DefaultInterfaceDisplay = eMemNumPad;
	  pnlList->Height = Panel4->Height + pnlKeyboard->Height;
   }
   else
   {
	  DefaultInterfaceDisplay = eMemKeypad;
	  pnlList->Height = Panel4->Height;
   }
   Realign();
   UpdateInterface();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmRMSRoom::btnClearMouseClick(TObject *Sender)
{
   edSearch->Text = "";
   edSearch->SetFocus();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmRMSRoom::tgridHistoryMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
   int Key = GridButton->Tag;

   bool ItemFound = false;
   for (int i = 0; i < tlistRooms->Items->Count; i++)
   {
	  if (int(tlistRooms->Items->Objects[i]) == Key)
	  {
		 LastSearchIndex = i + 1;
		 ItemFound = true;
		 tlistRooms->Selected[i] = true;
		 // tgridMembers->SetVisibleRow(i);
		 // tgridMembers->Buttons[i][0]->Latched = true;
	  }
   }

   if (ItemFound)
   {

      if(TGlobalSettings::Instance().NewBook==2)
     {
       SetSelectedNewBookRoom(Key, false);
     }
	 else
     {
     SetSelectedRoom(Key, false);
     }


	  //SetSelectedRoom(Key, false);
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmRMSRoom::tlistRoomsClick(TObject *Sender)
{
   if (!IncrementalSearch)
   {
	  edSearch->SelectAll();
   }

   //	SetSelectedRoom(int(tlistRooms->Items->Objects[tlistRooms->ItemIndex]), false);
   if(TGlobalSettings::Instance().NewBook==2)
     {
     SetSelectedNewBookRoom(int(tlistRooms->Items->Objects[tlistRooms->ItemIndex]), false);
     }
	 else{ SetSelectedRoom(int(tlistRooms->Items->Objects[tlistRooms->ItemIndex]), false); }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmRMSRoom::TouchBtn2AutoRepeat(TObject *Sender)
{
   __int64 TickCount = ::GetTickCount();
   int ScrollCount = 0;
   // while (ScrollCount < List->VisibleRowCount-1)
   while (ScrollCount < (int(tlistRooms->Height / tlistRooms->ItemHeight) - 1))
   {
	  if (::GetTickCount() > TickCount + 20)
	  {
		 TickCount = ::GetTickCount();
		 SendMessage(tlistRooms->Handle, WM_VSCROLL, SB_LINEUP, 0L);
		 ScrollCount++;
		 if (tlistRooms->TopIndex == 0)
		 {
			break;
		 }
	  }
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmRMSRoom::TouchBtn1AutoRepeat(TObject *Sender)
{
   __int64 TickCount = ::GetTickCount();
   int ScrollCount = 0;
   while (ScrollCount < (int(tlistRooms->Height / tlistRooms->ItemHeight) - 1))
   {
	  if (::GetTickCount() > TickCount + 20)
	  {
		 TickCount = ::GetTickCount();
		 SendMessage(tlistRooms->Handle, WM_VSCROLL, SB_LINEDOWN, 0L);
		 ScrollCount++;
	  }
   }
}
// ---------------------------------------------------------------------------


void __fastcall TfrmRMSRoom::Panel8Resize(TObject *Sender)
{
   TouchBtn1->Width = (Panel8->Width / 2) - (Panel6->Width / 2);
}
//----------------------------------------------------------------------------

void TfrmRMSRoom::SetFilterlist(bool temp)
{
	Filterlist = temp;
}
//----------------------------------------------------------------------------


int TfrmRMSRoom::SelectRoom(UnicodeString CsvFilePath)
{
   int RetVal = 0;
   try
   {

       LoadRooms(CsvFilePath);
	   RetVal = this->ShowModal();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
   }
   return RetVal;
}

int TfrmRMSRoom::SelectNewBookRoom(UnicodeString CsvFilePath)
{
   int RetVal = 0;
   try
   {

       LoadNewBookRooms(CsvFilePath);//trip

	   RetVal = this->ShowModal();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
   }
   return RetVal;
}

void TfrmRMSRoom::LoadRooms(UnicodeString inCsvFilePath)
{
	RoomsList.clear();
	CsvFilePath = inCsvFilePath;
	UnicodeString RoomsFileName = "\\RMS" + Now().FormatString("ddmmyy") +".csv";
	UnicodeString FullFileName =  inCsvFilePath + RoomsFileName;
	if(FileExists(FullFileName))
	{
		TCsv Csv;
		Csv.LoadFromFile(FullFileName);

		const int ROOM_NUMBER = 0;
		const int ACCOUNT_NUMBER = 1;
		const int SURNAME = 2;
		const int NAME = 3;
		const int ACTIVE = 4;

		for (int i = 0; i < Csv.RowCount; i++)
		{
			try
			{
				// Only Load the Active Rooms.
				if(Csv.Cells[ACTIVE][i].Trim() == "0")
				{
					TRMSRoom Room;
					Room.Clear();
					Room.RoomNo = Csv.Cells[ROOM_NUMBER][i];
					Room.AccountNo = StrToIntDef(Csv.Cells[ACCOUNT_NUMBER][i],-1);
					Room.Name = Csv.Cells[SURNAME][i] + ", " + Csv.Cells[NAME][i];
					Room.Active = (Csv.Cells[ACTIVE][i].Trim() == "0");
					RoomsList.push_back(Room);
				}
			}
			catch(Exception & E)
			{
				TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, ("Rooms Failed To Import : Line Number " + IntToStr(i) + " Error Msg : " + E.Message));
			}
		}
	}
	else
	{
		MessageBox("Unable to load room information from \n" + FullFileName, "Error Loading Rooms", MB_OK + MB_ICONINFORMATION);
	}
}


void TfrmRMSRoom::LoadNewBookRooms(UnicodeString inCsvFilePath)
{
	NewBookRoomsList.clear();
	CsvFilePath = inCsvFilePath;
	UnicodeString RoomsFileName = "\\ROOMLIST.csv";
	UnicodeString FullFileName =  inCsvFilePath + RoomsFileName;
	if(FileExists(FullFileName))
	{
		TCsv Csv;
		Csv.LoadFromFile(FullFileName);

		const int ROOM_NUMBER = 0;
		const int Guest_Name = 1;
		const int Allow_Charge = 2;

		for (int i = 0; i < Csv.RowCount; i++)
		{
			try
			{
               if(AnsiUpperCase(Csv.Cells[Allow_Charge][i]) == AnsiUpperCase("Yes"))
              {
                TNewBookRoom Room;
                Room.Clear()  ;
                Room.RoomCount=i+1;
                Room.RoomNo = Csv.Cells[ROOM_NUMBER][i];
                Room.Guest_Name = Csv.Cells[Guest_Name][i];
                Room.Allow_Charge =(AnsiUpperCase(Csv.Cells[Allow_Charge][i]) == AnsiUpperCase("No")?true:false);
                NewBookRoomsList.push_back(Room);
               }

			}
			catch(Exception & E)
			{
				TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, ("Rooms Failed To Import : Line Number " + IntToStr(i) + " Error Msg : " + E.Message));
			}
		}
	}
	else
	{
	MessageBox("No Rooms available for billing " , "No Rooms available", MB_OK + MB_ICONINFORMATION);
	}

          if( FileExists( FullFileName ) )
	{
   //	DeleteFile( FullFileName );
	}
}
