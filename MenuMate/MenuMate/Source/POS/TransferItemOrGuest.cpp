//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef  PalmMate
#include "Palm.h"
#endif

#include "TransferItemOrGuest.h"
#include "ZForm.h"
#include "EnableFloorPlan.h"
#include "VerticalSelect.h"
#include "MMTouchNumpad.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

TfrmTransferItemOrGuest *frmTransferItemOrGuest;
//---------------------------------------------------------------------------
__fastcall TfrmTransferItemOrGuest::TfrmTransferItemOrGuest(TComponent* Owner) : TZForm(Owner)
{

}
//---------------------------------------------------------------------------
__fastcall TfrmTransferItemOrGuest::~TfrmTransferItemOrGuest()
{
    delete SourceItems;
    delete DestItems;
}

void __fastcall TfrmTransferItemOrGuest::FormShow(TObject *Sender)
{
   if(SelectedItem)
   {
      ItmQty = 1;
      btnQuantity->Caption = IntToStr(ItmQty);
   }
   else
   {
      btnQuantity->Enabled = false;
   }
   btnDestination->Caption = DestItems->Strings[0];
   btnDestination->Tag = (int)DestItems->Objects[0];
   index_cnt = 1;
   GuestName ="";
}

void __fastcall TfrmTransferItemOrGuest::FormCreate(TObject *Sender)
{
   SourceItems = new TList;
   DestItems = new TStringList;
   SelectedItem = false;
}

void __fastcall TfrmTransferItemOrGuest::FormClose(TObject *Sender, TCloseAction &Action)
{
   Database::TDBTransaction DBTransferTransaction(TDeviceRealTerminal::Instance().DBControl);
   DBTransferTransaction.StartTransaction();

}

void __fastcall TfrmTransferItemOrGuest::btnOKMouseClick(TObject *Sender)
{
   dest_tab_key = btnDestination->Tag;
   ModalResult = mrOk;
}

void __fastcall TfrmTransferItemOrGuest::btnCloseMouseClick(TObject *Sender)
{
  ModalResult = mrCancel;
}

unsigned __int32 TfrmTransferItemOrGuest::getMaxSeatCount()
{
    std::auto_ptr<TEnableFloorPlan> floorPlan(new TEnableFloorPlan());
    unsigned __int32 value = floorPlan->GetMaxSeatCount();
    floorPlan.reset();
    return value;
//    return TEnableFloorPlan::Instance()->GetMaxSeatCount();
}

void __fastcall TfrmTransferItemOrGuest::btnDestinationClick(TObject *Sender)
{
    try
    {

        btnDestination->Caption = DestItems->Strings[0];
        std::auto_ptr<TfrmVerticalSelect>SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));
        TVerticalSelection Item;
        Item.Title = "Cancel";
        Item.Properties["Color"] = IntToStr(clMaroon);
        Item.CloseSelection = true;
        SelectionForm->Items.push_back(Item);
        unsigned __int32 maxSeatCount = getMaxSeatCount();
        for( __int64 i = 1; i <= maxSeatCount; i++ )
        {
            Item.Title = IntToStr(i) + ". " + TGlobalSettings::Instance().SeatLabel;
            Item.Properties["TabName"] = Item.Title;
            Item.Properties["TabKey"] = i;
            Item.Properties["Color"] = clInfoBk;
            Item.CloseSelection = true;
            SelectionForm->Items.push_back(Item);
        }
        SelectionForm->ShowModal();
        TVerticalSelection SelectedItem;
	    if(SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel" )
	    {
           btnDestination->Caption = SelectedItem.Properties["TabName"];;
           tab_key = SelectedItem.Properties["TabKey"];
           btnDestination->Tag = tab_key;
           GuestName = btnDestination->Caption;
        }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
   }
}

void __fastcall TfrmTransferItemOrGuest::btnQuantityClick(TObject *Sender)
{
    try
    {
        std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
        frmTouchNumpad->Mode = pmNumber;
        frmTouchNumpad->Caption = "Enter Quantity for Item ?";
        frmTouchNumpad->btnSurcharge->Caption = "Ok";
        frmTouchNumpad->btnDiscount->Visible = false;
        frmTouchNumpad->btnSurcharge->Visible = true;
        frmTouchNumpad->INTInitial = 1;
        if (frmTouchNumpad->ShowModal() == mrOk)
        {
           ItmQty = frmTouchNumpad->INTResult;
           if(GetItemQty >= ItmQty)
           {
              btnQuantity->Caption = IntToStr(ItmQty);
           }
           else
           {
              ItmQty = 1;
              MessageBox("Please Enter the Quantity less than or Equal to " + FloatToStr(GetItemQty) +"", "Warning", MB_OK + MB_ICONWARNING);
           }
        }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
   }
}
