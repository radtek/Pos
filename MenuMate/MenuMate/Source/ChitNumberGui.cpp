#include <vcl.h>
#pragma hdrstop

#include "DeviceRealControl.h"
#include "ChitNumberGui.h"
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "ZForm.h"
#include "VerticalSelect.h"
#include "MMTouchKeyboard.h"
#include "DBChit.h"
#include "VerticalSelectUtilities.h"
#include "ChitOptionsPrompt.h"
#include "MMTouchNumpad.h"
#include "DeviceRealTerminal.h"
#include <map>
#include "TaxRemoval.h"
#include "ManagerDiscount.h"
#include "ManagerChitNumber.h"

#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

using namespace vertical_selection_utilities;
using Database::TDBTransaction;

__fastcall
TfrmChitNumberGui::TfrmChitNumberGui(TComponent *owner, const TChitNumber &chit)
   : TForm(Owner),
     chit_options_prompt_(new chit_options_prompt_t(chit.ChitNumberKey)),
     keyboard_(TZForm::Create<TfrmTouchKeyboard>(this)),
     format_popup_menu_(0x0),
     type_popup_menu_(0x0),
     menu_popup_menu_(0x0),
     chit_(chit)
{
    TManagerMenus &ref_mm = *TDeviceRealControl::ActiveInstance().GetMenuManager();
    keyboard_->AllowCarriageReturn = false;
    keyboard_->MustHaveValue = true;
    if (!(default_menu_name_ = chit_.DefaultMenu()).Length())
        default_menu_name_ = L"None";
    CreatePopupMenus();
    Redraw();
    int maxChitNumber = TManagerChitNumber::Instance().GetMaxChitNumber();

    tbtnMaxChitNo->Caption = "Max Chit Number : " + IntToStr(maxChitNumber);
}

void __fastcall
TfrmChitNumberGui::OkClicked(TObject *)
{
    if(chit_.OnlinePickupOrder || chit_.OnlineDeliveryOrder)
    {
       if(!(CheckMenuExistsOrNot(default_menu_name_)))
       {
           MessageBox("Please Select a Preferred Menu to Process Your Online Order", "Error", MB_OK + MB_ICONERROR);
       }
       else
       {
          ModalResult = mrOk;
       }
    }
    else
    {
       ModalResult = mrOk;
    }
}

void __fastcall
TfrmChitNumberGui::CancelClicked(TObject *)
{
    ModalResult = mrCancel;
}

void __fastcall
TfrmChitNumberGui::NameClicked(TObject *)
{
    GetInput("Name", chit_.Name);
    Redraw();
}

void __fastcall
TfrmChitNumberGui::MaskClicked(TObject *)
{
    GetInput("Mask (e.g. Printing %)", chit_.Mask);
    Redraw();
}

void __fastcall
TfrmChitNumberGui::TypeClicked(TObject *)
{
    TVerticalSelection selection;

    type_popup_menu_->ShowModal();
    type_popup_menu_->GetFirstSelectedItem(selection);
    if (selection.Title == "Cancel")
        return;

    chit_.Type =
      static_cast<EChitPromptType>(
        static_cast<int>(selection.Properties["Type"]));

    Redraw();
}


void __fastcall
TfrmChitNumberGui::FormatClicked(TObject *)
{
    TVerticalSelection selection;

    format_popup_menu_->ShowModal();
    format_popup_menu_->GetFirstSelectedItem(selection);
    if (selection.Title == "Cancel")
        return;

    chit_.Format = static_cast<EChitFormatType>(
                     static_cast<int>(selection.Properties["Type"]));
    if (chit_.Format == ectList) {
        chit_options_prompt_->prompt();
        ;
    }

    Redraw();
}

void __fastcall
TfrmChitNumberGui::DefaultMenuClicked(TObject *)
{
    TVerticalSelection selection;

    menu_popup_menu_->ShowModal();
    menu_popup_menu_->GetFirstSelectedItem(selection);
    if (selection.Title == "Cancel")
        return;

    default_menu_name_ = selection.Title;
    chit_.DefaultMenu(default_menu_name_);

    Redraw();
}

void
TfrmChitNumberGui::CreateFormatPopupMenu()
{
    format_popup_menu_ =
      create_vertical_selection(
        this,
        (item_arglist_t()
         << item_t("Cancel").normal_bg(clMaroon)
         << (item_t(ChitFormatTypeStrings[ectNumeric])
             .normal_bg(clNavy)
             .property("Type", ectNumeric))
         << (item_t(ChitFormatTypeStrings[ectAlphaNumeric])
             .normal_bg(clNavy)
             .property("Type", ectAlphaNumeric))
         << (item_t(ChitFormatTypeStrings[ectList])
             .normal_bg(clNavy)
             .property("Type", ectList))));
}

void
TfrmChitNumberGui::CreateMenuPopupMenu()
{
    item_arglist_t items;
    std::vector<std::pair<UnicodeString, int> > menus;

    TDeviceRealControl::ActiveInstance().GetMenuManager()->GetMenuList(menus);

    items << item_t("Cancel").normal_bg(clMaroon)
          << item_t("None").normal_bg(clGreen);
    for (std::vector<std::pair<UnicodeString, int> >::const_iterator i =
           menus.begin(); i != menus.end(); ++i)
        items << item_t(i->first).normal_bg(clNavy);

    menu_popup_menu_ = create_vertical_selection(this, items);
}

void
TfrmChitNumberGui::CreatePopupMenus()
{
    CreateFormatPopupMenu();
    CreateMenuPopupMenu();
    CreateTypePopupMenu();
}

void
TfrmChitNumberGui::CreateTypePopupMenu()
{
    type_popup_menu_ =
      create_vertical_selection(
        this,
        (item_arglist_t()
         << item_t("Cancel").normal_bg(clMaroon)
         << (item_t(ChitPromptTypeStrings[ectOff])
             .normal_bg(clNavy)
             .property("Type", ectNumeric))
         << (item_t(ChitPromptTypeStrings[ectAuto])
             .normal_bg(clNavy)
             .property("Type", ectAuto))
         << (item_t(ChitPromptTypeStrings[ectPrompt])
             .normal_bg(clNavy)
             .property("Type", ectPrompt))));
}

void
TfrmChitNumberGui::GetInput(const UnicodeString &prompt,
                            UnicodeString &destination)
{
    keyboard_->Caption = prompt;
    keyboard_->KeyboardText = destination;

    if (keyboard_->ShowModal() == mrOk)
        destination = keyboard_->KeyboardText;
}

void
TfrmChitNumberGui::Redraw()
{
    tbtnName->Caption = "Name : " + chit_.Name;
    tbtnMask->Caption = "Mask : " + chit_.Mask;
    tbtnType->Caption = "Type : " + ChitPromptTypeStrings[chit_.Type];
    tbtnFormat->Caption = "Format : " + ChitFormatTypeStrings[chit_.Format];
    tbtnDefaultMenu->Caption ="Default Menu : " + default_menu_name_;
    cbDisableHoldAndSend->Checked = chit_.IsHoldAndSendDisabled;
    //MM-1647 Assigning the value to the check-box
    cbEnableTableDisplay->Checked = chit_.IsDisplayTablesEnabled;
    cbNonChargeable->Checked = chit_.IsNonChargableChit;
    cbCaptureCustomerNameAndPhone->Checked = chit_.IsCaptureCustomerNameAndPhone;
    cbCaptureCustomerDetails->Checked = chit_.IsCaptureCustomerDetails;
    cbAutoSaveOnTab->Checked = chit_.IsAutoSaveOnTab;
    cbPromptDeliveryTime->Checked = chit_.PromptForPickUpDeliveryTime;
    if((cbCaptureCustomerDetails->Checked==true)||(cbCaptureCustomerNameAndPhone->Checked==true))
    {
      cbAutoSaveOnTab->Enabled = true;
    }
    else
    {
      cbAutoSaveOnTab->Enabled = false;
    }
    //
    if(!chit_.PromptForPickUpDeliveryTime)
    {
       tbtnAddMinutes->Visible = false;
    }
    else
    {
       tbtnAddMinutes->Visible = true;
       tbtnAddMinutes->Caption = "+" + IntToStr(chit_.AddMinutes) + " minutes";
    }

    cbSaveCustomerName->Enabled = false;
    cbSaveMemberName->Enabled = false;
    cbSaveMemberAddress->Enabled = false;

    cbSaveCustomerName->Checked = chit_.SaveCustomerName;
    cbSaveMemberName->Checked = chit_.SaveMemberName;
    cbSaveMemberAddress->Checked = chit_.SaveMemberAddress;

    if(cbCaptureCustomerDetails->Checked)
    {
       cbSaveMemberName->Enabled = true;
       cbSaveMemberAddress->Enabled = true;
    }

   if(cbSaveCustomerName->Checked || cbCaptureCustomerNameAndPhone->Checked)
   {
      cbSaveCustomerName->Enabled = true;
      cbSaveMemberName->Checked = false;
      cbSaveMemberAddress->Checked = false;
      cbSaveMemberName->Enabled = false;
      cbSaveMemberAddress->Enabled = false;
   }
  if(cbSaveMemberAddress->Checked)
  {
     cbSaveMemberAddress->Enabled = true;
     cbSaveMemberName->Checked = false;
     cbSaveCustomerName->Checked = false;
     cbSaveCustomerName->Enabled = false;
     cbSaveMemberName->Enabled = false;
  }
  if(cbSaveMemberName->Checked)
  {
     cbSaveMemberName->Enabled = true;
     cbSaveMemberAddress->Enabled = false;
     cbSaveMemberAddress->Checked = false;
     cbSaveCustomerName->Checked = false;
     cbSaveCustomerName->Enabled = false;
  }
  //online pickup and delivery order
  cbOnlinePickupOrder->Enabled = true;
  cbOnlineDeliveryOrder->Enabled = true;
  cbOnlinePickupOrder->Checked = chit_.OnlinePickupOrder;
  cbOnlineDeliveryOrder->Checked = chit_.OnlineDeliveryOrder;
  if(cbOnlinePickupOrder->Checked)
  {
     cbOnlinePickupOrder->Enabled = true;
     cbOnlineDeliveryOrder->Checked = false;
     cbOnlineDeliveryOrder->Enabled = false;
  }
  if(cbOnlineDeliveryOrder->Checked)
  {
     cbOnlineDeliveryOrder->Enabled = true;
     cbOnlinePickupOrder->Checked = false;
     cbOnlinePickupOrder->Enabled = false;
  }

}

const TChitNumber &
TfrmChitNumberGui::ChitNumber()
const
{
    return chit_;
}

TChitNumber &
TfrmChitNumberGui::ChitNumber()
{
    return const_cast<TChitNumber &>(
             static_cast<const TfrmChitNumberGui *>(this)->ChitNumber());
}

const std::set<chit_name_t> &
TfrmChitNumberGui::OptionsToBeAdded()
const
{
    return chit_options_prompt_->to_be_added();
}

const std::set<chit_name_t> &
TfrmChitNumberGui::OptionsToBeDeleted()
const
{
    return chit_options_prompt_->to_be_deleted();
}

const std::map<chit_name_t, chit_name_t> &
TfrmChitNumberGui::OptionsToBeRenamed()
const
{
    return chit_options_prompt_->to_be_renamed();
}

void __fastcall TfrmChitNumberGui::ToggleHoldAndSendForThisChit(TObject *Sender)
{
	chit_.IsHoldAndSendDisabled = cbDisableHoldAndSend->Checked;
}
//---------------------------------------------------------------------------

//MM-1647 Assigning the value from the check-box to the variable
void __fastcall TfrmChitNumberGui::cbEnableTableDisplayClick(TObject *Sender)
{
    chit_.IsDisplayTablesEnabled = cbEnableTableDisplay->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmChitNumberGui::cbNonChargeableClick(TObject *Sender)
{
  chit_.IsNonChargableChit = cbNonChargeable->Checked;
}
//---------------------------------------------------------------------------

//MM2035
void __fastcall TfrmChitNumberGui::cbCaptureCustomerNameAndPhoneClick(TObject *Sender)
{
  cbAutoSaveOnTab->Checked = false;
  cbSaveCustomerName->Checked = false;
  cbAutoSaveOnTab->Enabled = cbCaptureCustomerNameAndPhone->Checked;
  cbCaptureCustomerDetails->Enabled = !cbCaptureCustomerNameAndPhone->Checked;
  chit_.IsCaptureCustomerNameAndPhone = cbCaptureCustomerNameAndPhone->Checked;
  cbSaveCustomerName->Enabled = cbCaptureCustomerNameAndPhone->Checked;
}
//---------------------------------------------------------------------------

//MM2036
void __fastcall TfrmChitNumberGui::cbAutoSaveOnTabClick(TObject *Sender)
{
  chit_.IsAutoSaveOnTab = cbAutoSaveOnTab->Checked;
}
//---------------------------------------------------------------------------
 //MM2063
void __fastcall TfrmChitNumberGui::cbCaptureCustomerDetailsClick(TObject *Sender)
{
  cbAutoSaveOnTab->Checked = false;
  cbSaveMemberName->Checked = false;
  cbSaveMemberAddress->Checked = false;
  cbAutoSaveOnTab->Enabled = cbCaptureCustomerDetails->Checked;
  cbCaptureCustomerNameAndPhone->Enabled = !cbCaptureCustomerDetails->Checked;
  chit_.IsCaptureCustomerDetails = cbCaptureCustomerDetails->Checked;
  cbSaveMemberName->Enabled = cbCaptureCustomerDetails->Checked;
  cbSaveMemberAddress->Enabled = cbCaptureCustomerDetails->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmChitNumberGui::tbtnMaxChitNoMouseClick(TObject *Sender)
{
	std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
	frmTouchNumpad->Caption = "Enter maximum chit number";
	frmTouchNumpad->btnSurcharge->Caption = "Ok";
	frmTouchNumpad->btnDiscount->Visible = false;
	frmTouchNumpad->btnSurcharge->Visible = true;
	frmTouchNumpad->Mode = pmNumber;
	if (frmTouchNumpad->ShowModal() == mrOk)
	{
		tbtnMaxChitNo->Caption = "Max Chit Number : " + IntToStr(frmTouchNumpad->INTResult);
		TGlobalSettings::Instance().MaximumChitNumber = frmTouchNumpad->INTResult;
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction.StartTransaction();
        int GlobalProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction,eSystemProfiles,"Globals");
        if(GlobalProfileKey == 0)
        {
            GlobalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction,eSystemProfiles,"Globals");
        }
        TManagerVariable::Instance().SetProfileInt(DBTransaction,vmMaximumChitNumber, frmTouchNumpad->INTResult);
        DBTransaction.Commit();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmChitNumberGui::tbtnRemoveTaxMouseClick(TObject *Sender)
{
    TStringList *taxList = new TStringList;
    TStringList *removedTaxList = new TStringList;
    TStringList *selectTaxList = new TStringList;
    GetTaxes(taxList,0);
    GetTaxes(taxList,4);
    for(int i =0; i < chit_.RemovedTaxList->Count; i++)
    {
      removedTaxList->AddObject(chit_.RemovedTaxList->Strings[i], (TObject *)chit_.RemovedTaxList->Objects[i]);
    }

    if(ShowTaxRemovalForm("Select Tax", taxList, selectTaxList,  removedTaxList))
    {
       chit_.RemovedTaxList = selectTaxList;
    }
}

void __fastcall TfrmChitNumberGui::tbtnRemoveServiceChargeMouseClick(TObject *Sender)
{
    TStringList *seviceChargeList = new TStringList;
    TStringList *removedTaxList = new TStringList;
    TStringList *selectTaxList = new TStringList;
    GetTaxes(seviceChargeList,2);
    for(int i =0; i < chit_.RemovedServiceChargeList->Count; i++)
    {
      removedTaxList->AddObject(chit_.RemovedServiceChargeList->Strings[i], (TObject *)chit_.RemovedServiceChargeList->Objects[i]);
    }
    if(ShowTaxRemovalForm("Select Service Charge", seviceChargeList, selectTaxList, removedTaxList))
    {
      chit_.RemovedServiceChargeList = selectTaxList;
    }
}

bool TfrmChitNumberGui::ShowTaxRemovalForm(AnsiString caption, TStringList* taxList,TStringList* selectTaxList, TStringList* lastTaxList)
{
    bool retval = false;
    std::auto_ptr<TfrmTaxRemoval> frmTaxRemoval(TfrmTaxRemoval::Create<TfrmTaxRemoval>(this,TDeviceRealTerminal::Instance().DBControl));
    frmTaxRemoval->Caption = caption;
    frmTaxRemoval->TaxList = taxList;
    frmTaxRemoval->RemovedTaxes = lastTaxList;
    if(frmTaxRemoval->ShowModal() == mrOk)
    {
      for(int i =0; i < lastTaxList->Count; i++)
      {
        selectTaxList->AddObject(lastTaxList->Strings[i], (TObject *)lastTaxList->Objects[i]);
      }
      retval = true;
    }
    return retval;
}

void TfrmChitNumberGui::GetTaxes(TStringList* taxList, int type)
{
   Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
   DBTransaction.StartTransaction();
   std::vector<TTaxProfile> gettaxProfiles = TDBTax::GetAllProfilesOfType(DBTransaction, type);
   DBTransaction.Commit();
   for(int i = 0; i < gettaxProfiles.size(); i++)
   {
       UnicodeString taxName = gettaxProfiles[i].Name;
       int key = gettaxProfiles[i].ProfileKey;
       taxList->AddObject(taxName, (TObject *)key);
   }
}
void __fastcall TfrmChitNumberGui::cbPromptDeliveryTimeClick(TObject *Sender)
{
  chit_.PromptForPickUpDeliveryTime = cbPromptDeliveryTime->Checked;
  //
  if(chit_.PromptForPickUpDeliveryTime)
  {
     tbtnAddMinutes->Visible = true;
     tbtnAddMinutes->Caption = "+" + IntToStr(chit_.AddMinutes) + " minutes";
  }
  else
  {
    tbtnAddMinutes->Visible = false;
  }

}
//---------------------------------------------------------------------------


void __fastcall TfrmChitNumberGui::tbtnSelectDiscountMouseClick(TObject *Sender)
{
  TStringList *discountsList = new TStringList;
  TStringList *applieddiscountsList = new TStringList;
  TStringList *selectdiscountsList = new TStringList;
  GetDiscounts(discountsList);
  for(int i =0; i < chit_.ApplyDiscountsList->Count; i++)
  {
     applieddiscountsList->AddObject(chit_.ApplyDiscountsList->Strings[i], (TObject *)chit_.ApplyDiscountsList->Objects[i]);
  }
  if(ShowTaxRemovalForm("Select Discounts", discountsList, selectdiscountsList, applieddiscountsList))
  {
      chit_.ApplyDiscountsList = selectdiscountsList;
  }
}

void TfrmChitNumberGui::GetDiscounts(TStringList* discountsList)
{
  ShowPointsAsDiscount = true;
  bool allow_combo = false;

  Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
  DBTransaction.StartTransaction();

  std::vector<eDiscountFilter> discountFilter;
  if(allow_combo)
     discountFilter.push_back(only_combos);
  else
     discountFilter.push_back(exclude_combos);

  ManagerDiscount->GetDiscountList(DBTransaction,discountsList,discountFilter,ShowPointsAsDiscount);
  DBTransaction.Commit();
}

void __fastcall TfrmChitNumberGui::tbtnAddMinutesMouseClick(TObject *Sender)
{
  ShowNumPad("Add Minute");
}

void TfrmChitNumberGui::ShowNumPad(AnsiString caption)
{
    std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
	frmTouchNumpad->Caption = caption;
	frmTouchNumpad->btnSurcharge->Caption = "Ok";
	frmTouchNumpad->btnSurcharge->Visible = true;
    frmTouchNumpad->btnCancel->Visible = true;
	frmTouchNumpad->Mode = pmNumber;
	if (frmTouchNumpad->ShowModal() == mrOk)
	{
        ProcessData(frmTouchNumpad->INTResult);
	}
}

void TfrmChitNumberGui::ProcessData(int min)
{
    if(ValidateMinutes(min))
    {
       tbtnAddMinutes->Caption = "+" + IntToStr(min) + " minutes";
       chit_.AddMinutes = min;
    }
    else
    {
      MessageBox("Please Enter Minute between 0 to 999", "Error", MB_OK + MB_ICONERROR);
    }
}


bool TfrmChitNumberGui::ValidateMinutes(int min)
{
  bool retval = false;

  if(min < 1000)
  {
    retval = true;
  }
  return retval;
}

void __fastcall TfrmChitNumberGui::cbSaveCustomerNameClick(TObject *Sender)
{
   chit_.SaveCustomerName = cbSaveCustomerName->Checked;
   if(cbSaveCustomerName->Checked)
   {
      cbSaveMemberName->Checked = false;
      cbSaveMemberAddress->Checked = false;
      cbSaveMemberName->Enabled = false;
      cbSaveMemberAddress->Enabled = false;
   }
}

void __fastcall TfrmChitNumberGui::cbSaveMemberNameClick(TObject *Sender)
{
  chit_.SaveMemberName = cbSaveMemberName->Checked;
  if(cbSaveMemberName->Checked)
  {
     cbSaveMemberAddress->Enabled = false;
     cbSaveMemberAddress->Checked = false;
     cbSaveCustomerName->Checked = false;
     cbSaveCustomerName->Enabled = false;
  }
  else
  {
     cbSaveMemberAddress->Enabled = true;
  }
}

void __fastcall TfrmChitNumberGui::cbSaveMemberAddressClick(TObject *Sender)
{
  chit_.SaveMemberAddress = cbSaveMemberAddress->Checked;
  if(cbSaveMemberAddress->Checked)
  {
     cbSaveMemberName->Checked = false;
     cbSaveCustomerName->Checked = false;
     cbSaveCustomerName->Enabled = false;
     cbSaveMemberName->Enabled = false;
  }
  else
  {
     cbSaveMemberName->Enabled = true;
  }
}

void __fastcall TfrmChitNumberGui::cbOnlinePickupOrderClick(TObject *Sender)
{
   chit_.OnlinePickupOrder = cbOnlinePickupOrder->Checked;
   bool checkValue = false;
   if(cbOnlinePickupOrder->Checked)
   {
       cbOnlineDeliveryOrder->Checked = false;
       cbOnlineDeliveryOrder->Enabled = false;
       checkValue = CheckPickupOrDelivery("ONLINE_PICKUP_ORDER");
       if(checkValue)
       {
          cbOnlinePickupOrder->Checked = false;
          cbOnlineDeliveryOrder->Enabled = true;
          MessageBox("Online Order Pickup Chit is Already Existed", "Information", MB_OK + MB_ICONINFORMATION);
       }
   }
   else
   {
      cbOnlineDeliveryOrder->Enabled = true;
   }

}

void __fastcall TfrmChitNumberGui::cbOnlineDeliveryOrderClick(TObject *Sender)
{

   chit_.OnlineDeliveryOrder = cbOnlineDeliveryOrder->Checked;
   bool checkValue = false;

   if(cbOnlineDeliveryOrder->Checked)
   {
       cbOnlinePickupOrder->Checked = false;
       cbOnlinePickupOrder->Enabled = false;
       checkValue = CheckPickupOrDelivery("ONLINE_DELIVERY_ORDER");
       if(checkValue)
       {
          cbOnlineDeliveryOrder->Checked = false;
          cbOnlinePickupOrder->Enabled = true;
          MessageBox("Online Order Delivery Chit is Already Existed", "Information", MB_OK + MB_ICONINFORMATION);
       }

   }
   else
   {
      cbOnlinePickupOrder->Enabled = true;
   }
}

bool TfrmChitNumberGui::CheckPickupOrDelivery(AnsiString caption)
{
   bool retVal = false;
   Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
   DBTransaction.StartTransaction();
   std::auto_ptr<TStringList>getList(new TStringList);
   TManagerChitNumber::Instance().CheckIsOnLinePickupOrDeliveryChecked(DBTransaction, caption, getList.get());

   for(int i = 0; i < getList->Count; i++)
   {
      int key = (int)getList->Objects[i];
      if(key != chit_.ChitNumberKey)
      {
         retVal = true;
      }
   }
   DBTransaction.Commit();
   return retVal;
}

bool TfrmChitNumberGui::CheckMenuExistsOrNot(UnicodeString default_menu_name_)
{
    bool retVal = false;
    std::vector<std::pair<UnicodeString, int> > menus;
    TDeviceRealControl::ActiveInstance().GetMenuManager()->GetMenuList(menus);

    for (std::vector<std::pair<UnicodeString, int> >::const_iterator i =
           menus.begin(); i != menus.end(); ++i)
           {
                if(default_menu_name_ == i->first)
                {
                   retVal = true;
                }
            }
     return retVal;
}
