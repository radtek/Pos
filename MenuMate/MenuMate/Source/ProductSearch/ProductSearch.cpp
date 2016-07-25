//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "DeviceRealTerminal.h"
#include "MMMessageBox.h"
#include "MMLogging.h"
#include "ManagerVariable.h"
#include "ItemSize.h"
#include "ProductSearch.h"
#include "StringTools.h"

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

//---------------------------------------------------------------------------
__fastcall TfrmProductSearch::TfrmProductSearch(TComponent* Owner) : TZForm(Owner)
{
    SearchType = eNameSearch;
    ItemIncrementalSearch = false;
    SelectedItems.reset(new TList());

}

__fastcall TfrmProductSearch::~TfrmProductSearch()
{
//
}

void __fastcall TfrmProductSearch::FormCreate(TObject *Sender)
{
    StartWithShiftDown = false;
    LastSearchIndex = 0;
    pnlList->Height = pnlSelectedItemDetails->Height;
}

void __fastcall TfrmProductSearch::FormShow(TObject *Sender)
{
    FormResize(this);
    tbtnOk->Enabled = true;
    tbtnCancel->Enabled = true;
    tbtnCancel->Visible = true;
    tListItems->Visible = true;
    LastSearchIndex = 0;
    SelectionVisible = false;
    QuickSearch(SearchText);
    LoadSelectedItemInformation();
    DisplayData();
}

void __fastcall TfrmProductSearch::FormClose(TObject *Sender, TCloseAction &Action)
{
   StartWithShiftDown = true;
   MustHaveValidItem = false;
}

void __fastcall TfrmProductSearch::FormResize(TObject *Sender)
{
//    if (Tag != Screen->Width)
}

void __fastcall TfrmProductSearch::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(this);
}

void __fastcall TfrmProductSearch::btnOkClick(TObject *Sender)
{
    LoadSelectedItemInformation();
    ModalResult = mrOk;
}

void __fastcall TfrmProductSearch::tListItemsClick(TObject *Sender)
{
    SetSelectedItem();
}

void __fastcall TfrmProductSearch::tbtnScrollUpAutoRepeat(TObject *Sender)
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

void __fastcall TfrmProductSearch::tbtnScrollDownAutoRepeat(TObject *Sender)
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

void __fastcall TfrmProductSearch::pnlScrollButtonsResize(TObject *Sender)
{
   tbtnScrollDown->Width = (pnlScrollButtons->Width / 2) - (pnlScrollButtonMargin->Width / 2);
}

bool TfrmProductSearch::QuickSearch(AnsiString SearchString)
{
    SearchString = SearchText;
    bool itemFound = false;

    try
    {
        if(SearchString == "")
        {
            LastSearchIndex = 0;
        }
        else
        {
            Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
            DBTransaction.StartTransaction();
            Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
            DBTransaction.RegisterQuery(IBInternalQuery);

            IBInternalQuery->Close();

            tListItems->Items->BeginUpdate();
            tListItems->Clear();

            AnsiString selectQuery1 =
                                 
                                     "SELECT FIRST 1 ";
            AnsiString selectQuery2 =
									"IT.ITEM_KEY,  "
                                     "IT.ITEM_ID, "
                                     "IT.ITEM_NAME, "
                                     "IT.ITEM_NAME || ' - ' || ITS.SIZE_NAME AS NAME, "
                                     "ITS.ITEMSIZE_KEY AS KEY, "
                                     "ITS.SIZE_NAME "
                                 "FROM ITEM IT "
                                     "INNER JOIN ITEMSIZE ITS ON IT.ITEM_KEY = ITS.ITEM_KEY "
                                     "INNER JOIN COURSE C ON IT.COURSE_KEY = C.COURSE_KEY "
                                     "INNER JOIN MENU M ON C.MENU_KEY = M.MENU_KEY "
                                     "INNER JOIN DEVICESMENUS DM ON M.MENU_NAME = DM.MENU_NAME "
                                 "WHERE IT.ENABLED = 'T' and ITS.ISINVISIBLE = 'F' AND IT.EXCLUSIVELY_AS_SIDE = 'F' AND DM.DEVICE_KEY = :TERMINALKEY AND ITS.ENABLED = 'T' ";

            IBInternalQuery->SQL->Text = selectQuery1 + selectQuery2 +"AND ( LOWER(ITs.BARCODE) = LOWER(:searchString) )  "
                                            "GROUP BY IT.ITEM_KEY,  IT.ITEM_ID, IT.ITEM_NAME,ITS.SIZE_NAME ,M.MENU_NAME, ITS.ITEMSIZE_KEY ";

            IBInternalQuery->ParamByName("searchString")->AsString = SearchString ;
            IBInternalQuery->ParamByName("TERMINALKEY")->AsInteger = TDeviceRealTerminal::Instance().ID.DeviceKey;

            IBInternalQuery->ExecQuery();
            if(IBInternalQuery->RecordCount <= 0)
            {
                IBInternalQuery->Close();
                IBInternalQuery->SQL->Text = "Select " +selectQuery2  + "AND (LOWER(IT.ITEM_NAME) CONTAINING LOWER(:searchString) OR LOWER(ITs.BARCODE) CONTAINING LOWER(:searchString) )  "
                                            "GROUP BY IT.ITEM_KEY,  IT.ITEM_ID, IT.ITEM_NAME,ITS.SIZE_NAME ,M.MENU_NAME, ITS.ITEMSIZE_KEY ";
                IBInternalQuery->ParamByName("searchString")->AsString = SearchString ;
                IBInternalQuery->ParamByName("TERMINALKEY")->AsInteger = TDeviceRealTerminal::Instance().ID.DeviceKey;
                IBInternalQuery->ExecQuery();
            }

            for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
            {
                UnicodeString itemName = IBInternalQuery->FieldByName("NAME")->AsString;
                int itemSizeId =  IBInternalQuery->FieldByName("KEY")->AsInteger;

                tListItems->AddItem(itemName, reinterpret_cast<TObject *>(itemSizeId));
            }

            tListItems->Items->EndUpdate();
            tListItems->Refresh();
            tListItems->Realign();
            tListItems->ItemIndex = 0;
            DBTransaction.Commit();
        }
    }
    catch(Exception & E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
    }

    return itemFound;
}



void __fastcall TfrmProductSearch::FormCloseQuery(TObject *Sender, bool &CanClose)
{
    if (MustHaveValidItem && SelectedItem && SelectedItem->ItemKey == 0)
    {
        MessageBox("A Valid Item must be selected", "Please Select an Item", MB_OK + MB_ICONINFORMATION);
        CanClose = false;
    }
}

void TfrmProductSearch::LoadSelectedItemInformation()
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

std::pair<TItem*, TItemSize*> TfrmProductSearch::GetSelectedItemAndItemSize()
{
    return ItemAndItemSize;
}

void TfrmProductSearch::SetSelectedItem()
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

void TfrmProductSearch::DisplayData()
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

void __fastcall TfrmProductSearch::tbtnCancelMouseClick(TObject *Sender)
{
    ModalResult = mrCancel;
    Close();
}

void __fastcall TfrmProductSearch::CardSwipe(Messages::TMessage& Message)
{
	enum TCardType
	{
		eUnknown, eKitchenDocket, eStaffCard, eMemberCard, eTabCard, eDiscountCard, eEmbeddedPriceBarcode
	};
    TCardType DataType = eUnknown;
    AnsiString Data = *((AnsiString*)Message.WParam);
}

void __fastcall TfrmProductSearch::tListItemsDblClick(TObject *Sender)
{
    LoadSelectedItemInformation();
    ModalResult = mrOk;
}
//---------------------------------------------------------------------------
