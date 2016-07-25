//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#ifdef PalmMate
#include "Palm.h"
#endif

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif

#include "SelectItem.h"
#include "MMMessageBox.h"
#include "MMLogging.h"
#include "ManagerVariable.h"
#include "ItemSize.h"

#include <cctype>

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchControls"
#pragma link "TouchKeyboard"
#pragma link "TouchBtn"
#pragma link "TouchNumpad"
#pragma link "TouchGrid"
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

//---------------------------------------------------------------------------
__fastcall TfrmSelectItem::TfrmSelectItem(TComponent* Owner) : TZForm(Owner)
{
    SearchType = eSearchByItemName;
    CurrentVisibleInterface = eMemKeypad;
    ItemIncrementalSearch = false;
    SelectedItems.reset(new TList());
}

void __fastcall TfrmSelectItem::FormCreate(TObject *Sender)
{
    StartWithShiftDown = false;
    LastSearchIndex = 0;
    pnlList->Height = pnlSelectedItemDetails->Height;
}

void __fastcall TfrmSelectItem::FormShow(TObject *Sender)
{
    FormResize(this);

    tbtnOk->Enabled = false;
    tbtnCancel->Enabled = true;
    tbtnCancel->Visible = true;
    tListItems->Visible = true;

    tedtSearchText->Text = "";
    LastSearchIndex = 0;
    SelectionVisible = false;

    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    FetchMenuItems(DBTransaction);

    SearchType = eSearchByName;
    if (SearchType == eSearchByItemBarcode)
    {
        tbtnSearchByBarcodeClick(Sender);
    }
    else
    {
        tbtnSearchByNameClick(Sender);
    }

    tedtSearchText->SetFocus();
}

void __fastcall TfrmSelectItem::FormClose(TObject *Sender, TCloseAction &Action)
{
   StartWithShiftDown = true;
   MustHaveValidItem = false;
}

void __fastcall TfrmSelectItem::FormResize(TObject *Sender)
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

    pnlSelectedItemDetails->Height = pnlKeyboard->Top;
    pnlScrollButtons->Height = pnlSearchCriteria->Height;
    pnlList->Width = pnlSelectedItemDetails->Left -this->BorderWidth;
    pnlList->Left = this->BorderWidth;
    pnlList->Top = this->BorderWidth;

    Left = (Screen->Width - Width) / 2;
    Top = (Screen->Height - Height) / 2;

    if (TDeviceRealTerminal::Instance().ManagerMembership->DefaultInterfaceDisplay == eMemKeypad)
    {
      pnlList->Height = pnlSelectedItemDetails->Height;
    }
    else
    {
      pnlList->Height = pnlSelectedItemDetails->Height + pnlKeyboard->Height;
    }

    Realign();
}

void __fastcall TfrmSelectItem::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(this);
}

void __fastcall TfrmSelectItem::btnOkClick(TObject *Sender)
{
    LoadSelectedItemInformation();
    ModalResult = mrOk;
}

void __fastcall TfrmSelectItem::tbtnClearClick(TObject *Sender)
{
    lbItemText->Clear();
}

void __fastcall TfrmSelectItem::btnClearMouseClick(TObject *Sender)
{
    lbItemText->Clear();

    tedtSearchText->Clear();

    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    FetchMenuItems(DBTransaction);

    tedtSearchText->SetFocus();
}

void __fastcall TfrmSelectItem::btnSearchMouseClick(TObject *Sender)
{
    PostMessage(tedtSearchText->Handle, WM_CHAR, '\r', 0);
    tedtSearchText->SetFocus();
}

void __fastcall TfrmSelectItem::tpnlKeyboardClick(TObject *Sender, char Char)
{
   PostMessage(tedtSearchText->Handle, WM_CHAR, Char, 0);
   tedtSearchText->SetFocus();
}

void __fastcall TfrmSelectItem::tpnlNumKeypadClick(TObject *Sender, TNumpadKey Key)
{
    if (Key <= nk9)
    {
        PostMessage(tedtSearchText->Handle, WM_CHAR, char(Key + 0x30), 0);
    }
    else if (Key == nkCustom)
    {
        tedtSearchText->Text = "";
    }
    else
    {
        tedtSearchText->Text = "";
    }
    tedtSearchText->SetFocus();
}

void __fastcall TfrmSelectItem::tbtnInterfaceMouseClick(TObject *Sender)
{
    if (CurrentVisibleInterface == eMemKeypad)
    {
        CurrentVisibleInterface = eMemNumPad;
        pnlList->Height = pnlSelectedItemDetails->Height + tpnlKeyboard->Height;
    }
    else
    {
        CurrentVisibleInterface = eMemKeypad;
        pnlList->Height = pnlSelectedItemDetails->Height;
    }
    Realign();
    UpdateInterface();
}

void __fastcall TfrmSelectItem::tListItemsClick(TObject *Sender)
{
    if (!ItemIncrementalSearch)
    {
        tedtSearchText->SelectAll();
    }

    SetSelectedItem();
}

void __fastcall TfrmSelectItem::tbtnScrollUpAutoRepeat(TObject *Sender)
{
    __int64 TickCount = ::GetTickCount();
    int ScrollCount = 0;

    while (ScrollCount < (int(tListItems->Height / tListItems->ItemHeight) - 1))
    {
        if (::GetTickCount() > TickCount + 20)
        {
            TickCount = ::GetTickCount();
            SendMessage(tListItems->Handle, WM_VSCROLL, SB_LINEUP, 0L);
            ScrollCount++;
            if (tListItems->TopIndex == 0)
            {
                break;
            }
        }
    }
}

void __fastcall TfrmSelectItem::tbtnScrollDownAutoRepeat(TObject *Sender)
{
    __int64 TickCount = ::GetTickCount();
    int ScrollCount = 0;
    while (ScrollCount < (int(tListItems->Height / tListItems->ItemHeight) - 1))
    {
        if (::GetTickCount() > TickCount + 20)
        {
            TickCount = ::GetTickCount();
            SendMessage(tListItems->Handle, WM_VSCROLL, SB_LINEDOWN, 0L);
            ScrollCount++;
        }
    }
}

void __fastcall TfrmSelectItem::pnlScrollButtonsResize(TObject *Sender)
{
   tbtnScrollDown->Width = (pnlScrollButtons->Width / 2) - (pnlScrollButtonMargin->Width / 2);
}

void __fastcall TfrmSelectItem::tbtnSearchByNameClick(TObject *Sender)
{
    SearchType = eSearchByItemName;
    //tbtnSearchByName->ButtonColor = (TColor)RGB(0,74,0);
    tbtnSearchByName->ButtonColor = (TColor)RGB(0,157,191);
    //tbtnSearchByBarcode->ButtonColor = clGray;
    //MM1725
    tbtnSearchByBarcode->ButtonColor = (TColor)RGB(167,171,174);

    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    FetchMenuItems(DBTransaction);

    tedtSearchText->SetFocus();
    CurrentVisibleInterface = eMemKeypad;
    pnlList->Height = pnlSelectedItemDetails->Height;
    UpdateInterface();
}

void __fastcall TfrmSelectItem::tbtnSearchByBarcodeClick(TObject *Sender)
{
    SearchType = eSearchByItemBarcode;
    //tbtnSearchByName->ButtonColor = clGray;
    //MM1725
    tbtnSearchByName->ButtonColor = (TColor)RGB(167,171,174);
    //tbtnSearchByBarcode->ButtonColor = (TColor)RGB(0,74,0);
    tbtnSearchByBarcode->ButtonColor = (TColor)RGB(0,157,191);

    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    FetchMenuItems(DBTransaction);

    tedtSearchText->SetFocus();
    CurrentVisibleInterface = eMemNumPad;
    pnlList->Height = pnlSelectedItemDetails->Height + tpnlKeyboard->Height;
    UpdateInterface();
}

bool TfrmSelectItem::QuickSearch(AnsiString SearchString)
{
    SearchString = tedtSearchText->Text;
    bool itemFound = false;

    try
    {
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction.StartTransaction();

        if(SearchString == "")
        {
            LastSearchIndex = 0;
            FetchMenuItems(DBTransaction);
        }
        else
        {
            Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
            DBTransaction.RegisterQuery(IBInternalQuery);

            IBInternalQuery->Close();

            tListItems->Items->BeginUpdate();
            tListItems->Clear();

            AnsiString selectQuery[2] = { "SELECT IT.ITEM_KEY,  IT.ITEM_ID, IT.ITEM_NAME, IT.ITEM_NAME || ' - ' || ITS.SIZE_NAME AS NAME, ITS.ITEMSIZE_KEY AS KEY, ITS.SIZE_NAME FROM ITEM IT INNER JOIN ITEMSIZE ITS ON IT.ITEM_KEY = ITS.ITEM_KEY INNER JOIN COURSE C ON IT.COURSE_KEY = C.COURSE_KEY INNER JOIN MENU M ON C.MENU_KEY = M.MENU_KEY INNER JOIN DEVICESMENUS DM ON M.MENU_NAME = DM.MENU_NAME WHERE IT.ENABLED = 'T' AND IT.EXCLUSIVELY_AS_SIDE = 'F' AND IT.ITEM_NAME CONTAINING :searchString OR ITs.BARCODE CONTAINING :searchString group by IT.ITEM_KEY,  IT.ITEM_ID, IT.ITEM_NAME,ITS.SIZE_NAME ,M.MENU_NAME, ITS.ITEMSIZE_KEY",
                                    "SELECT IT.ITEM_KEY,  IT.ITEM_ID, IT.ITEM_NAME, IT.ITEM_NAME || ' - ' || ITS.BARCODE AS NAME, ITS.ITEMSIZE_KEY AS KEY, ITS.SIZE_NAME FROM ITEM IT INNER JOIN ITEMSIZE ITS ON IT.ITEM_KEY = ITS.ITEM_KEY INNER JOIN COURSE C ON IT.COURSE_KEY = C.COURSE_KEY INNER JOIN MENU M ON C.MENU_KEY = M.MENU_KEY INNER JOIN DEVICESMENUS DM ON M.MENU_NAME = DM.MENU_NAME WHERE IT.ENABLED = 'T' AND IT.EXCLUSIVELY_AS_SIDE = 'F' AND IT.ITEM_NAME CONTAINING :searchString OR ITs.BARCODE CONTAINING :searchString group by IT.ITEM_KEY,  IT.ITEM_ID, IT.ITEM_NAME,  NAME, ITS.ITEMSIZE_KEY, ITS.SIZE_NAME"};

            IBInternalQuery->SQL->Text = selectQuery[SearchType];
            IBInternalQuery->ParamByName("searchString")->AsString = SearchString;

            IBInternalQuery->ExecQuery();
            for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
            {
                UnicodeString itemName = IBInternalQuery->FieldByName("NAME")->AsString;
                int itemSizeId =  IBInternalQuery->FieldByName("KEY")->AsInteger;

                tListItems->AddItem(itemName, reinterpret_cast<TObject *>(itemSizeId));
            }

            tListItems->Items->EndUpdate();
            tListItems->Refresh();
            tListItems->Realign();
            tListItems->SetFocus();

            tListItems->ItemIndex = 0;
        }
    }
    catch(Exception & E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
    }

    return itemFound;
}

void TfrmSelectItem::UpdateInterface()
{
    if (CurrentVisibleInterface == eMemKeypad)
    {
        tbtnInterface->Caption = "NumPad";
        tpnlNumKeypad->Visible = false;
        tpnlKeyboard->Visible = true;
    }
    else
    {
        tbtnInterface->Caption = "Keyboard";
        tpnlNumKeypad->Visible = true;
        tpnlKeyboard->Visible = false;
    }
}

void __fastcall TfrmSelectItem::FormCloseQuery(TObject *Sender, bool &CanClose)
{
    if (MustHaveValidItem && SelectedItem && SelectedItem->ItemKey == 0)
    {
        MessageBox("A Valid Item must be selected", "Please Select an Item", MB_OK + MB_ICONINFORMATION);
        CanClose = false;
    }
}

void TfrmSelectItem::LoadSelectedItemInformation()
{
    if(tListItems->ItemIndex != -1)
    {
        pair<TItem*, TItemSize*> itemAndItemSize(NULL, NULL);
        int itemSizeKey = int(tListItems->Items->Objects[tListItems->ItemIndex]);

        bool ItemFound = false;
        for (int i = 0; i < TDeviceRealTerminal::Instance().Menus->Current->Count && !ItemFound; i++)
        {
            TListMenu *Menu = TDeviceRealTerminal::Instance().Menus->Current->MenuGet(i);
            for (int j = 0; j < Menu->Count && !ItemFound; j++)
            {
                TListCourse *Course = Menu->CourseGet(j);

                for (int i = 0; i < Course->Count && !ItemFound; i++)
                {
                    TItem *Item = Course->ItemGet(i);
                    TItemSize *ItemSize;

                    for (int j = 0; j < Item->Sizes->Count && !ItemFound; j++)
                    {
                        ItemSize = Item->Sizes->SizeGet(j);
                        if (ItemSize->ItemSizeKey == itemSizeKey)
                        {
                            ItemFound = true;
                            itemAndItemSize.first = Item;
                            itemAndItemSize.second = ItemSize;
                            SelectedItem = Item;
                            if (!Item->Enabled)
                            {
                                MessageBox("Cannot order the item " + Item->Item + "( " + ItemSize->Name + ") as it has been disabled.",
                                "Cannot order item",
                                MB_OK | MB_ICONEXCLAMATION);
                            }
                        }
                    }
                }
            }
        }
    ItemAndItemSize = itemAndItemSize;
    }
}

std::pair<TItem*, TItemSize*> TfrmSelectItem::GetSelectedItemAndItemSize()
{
    return ItemAndItemSize;
}

void TfrmSelectItem::SetSelectedItem()
{
    if (tListItems->ItemIndex != -1)
    {
        tpItemDetails->ActivePage = tsItemDetails;

        DisplayData();

        tbtnOk->Enabled = true;
        tbtnOk->Caption = "Select\r" + SelectedItem->Item;

        SelectionVisible = true;
    }
    else
    {
        tbtnOk->Caption = "Ok";
        tbtnOk->Enabled = false;
        lbItemText->Clear();
    }
}

void TfrmSelectItem::DisplayData()
{
    if (tListItems->ItemIndex != -1)
    {
        LoadSelectedItemInformation();
        lbItemText->Clear();
        lbItemText->Items->Add("Product Name   : " + TStringTools::Instance()->StripSpecialChars_UC(ItemAndItemSize.first->Item));
        lbItemText->Items->Add("Barcode        : " + TStringTools::Instance()->StripSpecialChars_UC(ItemAndItemSize.second->Barcode));
        lbItemText->Items->Add("Size           : " + TStringTools::Instance()->StripSpecialChars_UC(ItemAndItemSize.second->Name));
        lbItemText->Items->Add("Price          : " + TStringTools::Instance()->StripSpecialChars_UC(FormatFloat("0.00", ItemAndItemSize.second->Price)));
        lbItemText->Items->Add("Menu Name      : " + TStringTools::Instance()->StripSpecialChars_UC(ItemAndItemSize.first->MenuName));
        lbItemText->Items->Add("Course Name    : " + TStringTools::Instance()->StripSpecialChars_UC(ItemAndItemSize.first->Course));
    }
    else
    {
        lbItemText->Clear();
    }
}

void TfrmSelectItem::FetchMenuItems(Database::TDBTransaction &DBTransaction)
{
    try
    {
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        DBTransaction.RegisterQuery(IBInternalQuery);

        IBInternalQuery->Close();

        tListItems->Items->BeginUpdate();
	    tListItems->Clear();

        AnsiString selectQuery[2] = { "SELECT IT.ITEM_KEY,  IT.ITEM_ID, IT.ITEM_NAME, IT.ITEM_NAME || ' - ' || ITS.SIZE_NAME AS NAME, ITS.ITEMSIZE_KEY AS KEY, ITS.SIZE_NAME FROM ITEM IT INNER JOIN ITEMSIZE ITS ON IT.ITEM_KEY = ITS.ITEM_KEY INNER JOIN COURSE C ON IT.COURSE_KEY = C.COURSE_KEY INNER JOIN MENU M ON C.MENU_KEY = M.MENU_KEY INNER JOIN DEVICESMENUS DM ON M.MENU_NAME = DM.MENU_NAME WHERE IT.ENABLED = 'T' AND IT.EXCLUSIVELY_AS_SIDE = 'F' AND DM.DEVICE_KEY = :TERMINALKEY  GROUP BY IT.ITEM_KEY,  IT.ITEM_ID, IT.ITEM_NAME,ITS.SIZE_NAME ,M.MENU_NAME, ITS.ITEMSIZE_KEY",
                                "SELECT IT.ITEM_KEY,  IT.ITEM_ID, IT.ITEM_NAME, IT.ITEM_NAME || ' - ' || ITS.BARCODE AS NAME, ITS.ITEMSIZE_KEY AS KEY, ITS.SIZE_NAME FROM ITEM IT INNER JOIN ITEMSIZE ITS ON IT.ITEM_KEY = ITS.ITEM_KEY INNER JOIN COURSE C ON IT.COURSE_KEY = C.COURSE_KEY INNER JOIN MENU M ON C.MENU_KEY = M.MENU_KEY INNER JOIN DEVICESMENUS DM ON M.MENU_NAME = DM.MENU_NAME WHERE IT.ENABLED = 'T' AND IT.EXCLUSIVELY_AS_SIDE = 'F' AND DM.DEVICE_KEY = :TERMINALKEY GROUP BY IT.ITEM_KEY,  IT.ITEM_ID, IT.ITEM_NAME,  NAME, ITS.ITEMSIZE_KEY, ITS.SIZE_NAME"};

        IBInternalQuery->SQL->Text = selectQuery[SearchType];
        IBInternalQuery->ParamByName("TERMINALKEY")->AsInteger = TDeviceRealTerminal::Instance().ID.DeviceKey;
        IBInternalQuery->ExecQuery();
        for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            UnicodeString itemName = IBInternalQuery->FieldByName("NAME")->AsString;
            int itemSizeId =  IBInternalQuery->FieldByName("KEY")->AsInteger;
            tListItems->AddItem(itemName, reinterpret_cast<TObject *>(itemSizeId));
        }

        tListItems->Items->EndUpdate();
        tListItems->Refresh();
        tListItems->Realign();
        tListItems->SetFocus();
    }
    catch(Exception & E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
    }
}

int TfrmSelectItem::GetItemKeyByItemSize(int itemSizeKey)
{
    try
    {
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
        DBTransaction.StartTransaction();

        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        DBTransaction.RegisterQuery(IBInternalQuery);

        IBInternalQuery->Close();

        IBInternalQuery->SQL->Text = "SELECT ITS.ITEM_KEY AS KEY FROM ITEMSIZE ITS WHERE ITS.ITEMSIZE_KEY = :itemSizeKey";
        IBInternalQuery->ParamByName("itemSizeKey")->AsInteger = itemSizeKey;
        IBInternalQuery->ExecQuery();

        int itemSizeKey =  IBInternalQuery->FieldByName("KEY")->AsInteger;
        return itemSizeKey;
    }
    catch(Exception & E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
    }
}

void __fastcall TfrmSelectItem::tbtnCancelMouseClick(TObject *Sender)
{
    ModalResult = mrCancel;
    Close();
}

void __fastcall TfrmSelectItem::tedtSearchTextChange(TObject *Sender)
{
    if(ItemIncrementalSearch || tedtSearchText->Text == "")
    {
        LastSearchIndex = 0;
        tbtnOk->Caption = "Ok";
        tbtnOk->Enabled = false;
        bool isItemFound = QuickSearch(tedtSearchText->Text);

        if(isItemFound)
        {
            DisplayData();
        }
        else
        {
            tListItems->ItemIndex = -1;
            DisplayData();
        }
    }
}

void __fastcall TfrmSelectItem::tedtSearchTextKeyPress(TObject *Sender, wchar_t &Key)
{
    if (Key == VK_RETURN)
    {
        tbtnOk->Caption = "Ok";
        tbtnOk->Enabled = false;
        QuickSearch(tedtSearchText->Text);
        tedtSearchText->SelectAll();
        DisplayData();
    }
    else
    {
        LastSearchIndex = 0;
    }
}

void __fastcall TfrmSelectItem::CardSwipe(Messages::TMessage& Message)
{
	enum TCardType
	{
		eUnknown, eKitchenDocket, eStaffCard, eMemberCard, eTabCard, eDiscountCard, eEmbeddedPriceBarcode
	};
    TCardType DataType = eUnknown;
    AnsiString Data = *((AnsiString*)Message.WParam);
}

void __fastcall TfrmSelectItem::tListItemsDblClick(TObject *Sender)
{
    LoadSelectedItemInformation();
    ModalResult = mrOk;
}
//---------------------------------------------------------------------------

