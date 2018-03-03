// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "GUIDiscount.h"
#include "SelectDiscounts.h"
#include "MMTouchKeyboard.h"
#include "MMMessageBox.h"
#include "DeviceRealTerminal.h"
#include "ManagerDiscount.h"
#include "DBSecurity.h"
#include "SCDPWDChecker.h"
// ---------------------------------------------------------------------------
#include <mshtml.h>
// #include <oleacc.h>
#include "SHDocVw_OCX.h"
#include "OrderUtils.h"

#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma link "TouchNumpad"
#pragma link "SHDocVw_OCX"
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TfrmSelectDiscounts::TfrmSelectDiscounts(TComponent* Owner) : TZForm(Owner)
{
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectDiscounts::btnOkMouseClick(TObject *Sender)
{
   UpdateManualDiscount();

	/*
	 * Determine whether anything's been changed that should result in a
	 * "clone" order being placed. The flag altered here -
	 * alterations_have_been_made - is only used in the case that a stack of
	 * items created using the Quantity screen is having it's pricing stuff
	 * changed. I.e. discounts added, removed, whatever.
	 */
	alterations_have_been_made = 0;

	if (pnlItemDiscountGridContainer->Visible) {
		for (int i = 0, j = original_discount_states.size(); i < j; i++)
			alterations_have_been_made |=
      	  (original_discount_states[i]
			   != tgridItemPrice->Buttons[i][0]->Latched);
	}

	alterations_have_been_made |=
     (pnlMiscPrice->Visible
	   & original_price != NumpadDisplayItemPrice->Numeric());

   Close();
}

// ---------------------------------------------------------------------------
void TfrmSelectDiscounts::UpdateManualDiscount()
{
   if (SelectedItem != NULL)
   {
	  if (NumpadDisplayItemPrice->Numeric() != SelectedItem->PriceEach())
	  {
    	  double FromVal = SelectedItem->Price();
		 /* Price Adjusts as a Misc Price buried in the Item */
		 SelectedItem->SetPriceLevelCustom(NumpadDisplayItemPrice->Numeric());

		 SelectedItem->Security->SecurityDelete(SelectedItem->Security->SecurityGetType(secPriceAdjust));
		 TSecurityReference *SecRef = new TSecurityReference;
		 SecRef->UserKey = TDeviceRealTerminal::Instance().User.ContactKey;
		 SecRef->Event = SecurityTypes[secPriceAdjust];
		 SecRef->From = CurrToStrF(FromVal, ffCurrency, 2);
		 SecRef->To = CurrToStrF(NumpadDisplayItemPrice->Numeric(), ffCurrency, 2);
		 SecRef->Note = tbtnReason->Caption;
		 SecRef->TimeStamp = Now();
		 SelectedItem->Security->SecurityAdd(SecRef);
		 if (SelectedItem->Security->GetSecurityRefNumber() == 0)
		 {
            Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
            DBTransaction.StartTransaction();
				SelectedItem->Security->SetSecurityRefNumber(TDBSecurity::GetNextSecurityRef(DBTransaction));
            DBTransaction.Commit();
		 }
	  }


	  TSecurityReference *OldSecRef = SelectedItem->Security->SecurityGetType(secDiscountedBy);
      if(SelectedItem->Price() != SelectedItem->TotalPriceAdjustment())
      {
         if (OldSecRef == NULL)
         {
            // Add Security.
            TSecurityReference *SecRef = new TSecurityReference;
            SecRef->UserKey = TDeviceRealTerminal::Instance().User.ContactKey;
            SecRef->Event = SecurityTypes[secDiscountedBy];
            SecRef->From =  CurrToStrF( SelectedItem->Price(), ffCurrency, 2);
            SecRef->To = CurrToStrF(SelectedItem->TotalPriceAdjustment(), ffCurrency, 2);
            SecRef->Note = "";
            SecRef->TimeStamp = Now();
            SelectedItem->Security->SecurityAdd(SecRef);
         }
         else
         {
            SelectedItem->Security->SecurityDelete(SelectedItem->Security->SecurityGetType(secDiscountedBy));
            TSecurityReference *SecRef = new TSecurityReference;
            SecRef->UserKey = TDeviceRealTerminal::Instance().User.ContactKey;
            SecRef->Event = SecurityTypes[secDiscountedBy];
            SecRef->From =  CurrToStrF(SelectedItem->Price(), ffCurrency, 2);
            SecRef->To = CurrToStrF(SelectedItem->TotalPriceAdjustment(), ffCurrency, 2);
            SecRef->Note = "";
            SecRef->TimeStamp = Now();
            SelectedItem->Security->SecurityAdd(SecRef);
         }
      }
      else
      {
      	 SelectedItem->Security->SecurityDelete(SelectedItem->Security->SecurityGetType(secDiscountedBy));
      }
   }
}

void __fastcall TfrmSelectDiscounts::Execute(TItemMinorComplete *inSelectedItem, TItemRedirector *inRedirector, bool ShowDiscounts,
   bool ShowPriceAdjsut, TList *OrdersList)
{
   Orders = OrdersList;
   SelectedItem = inSelectedItem;
   Redirector = inRedirector;

   if (ShowDiscounts)
   {
	  pnlItemDiscountGridContainer->Visible = true;
	  Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	  DBTransaction.StartTransaction();
	  std::auto_ptr <TStringList> List(new TStringList);
      std::vector<eDiscountFilter> discountFilter;
      discountFilter.push_back(exclude_combos);
 	  ManagerDiscount->GetDiscountList(DBTransaction, List.get(), discountFilter);
	  DBTransaction.Commit();

	  tgridItemPrice->ColCount = 1;
	  tgridItemPrice->RowCount = List->Count;

      original_discount_states.insert(original_discount_states.end(), List->Count, false);

	  for (int i = 0; i < List->Count; i++)
	  {
		 tgridItemPrice->Buttons[i][0]->Caption = List->Strings[i];
		 tgridItemPrice->Buttons[i][0]->Color = clMaroon;
		 tgridItemPrice->Buttons[i][0]->FontColor = clWhite;
		 tgridItemPrice->Buttons[i][0]->Tag = (int)List->Objects[i];
		 tgridItemPrice->Buttons[i][0]->LatchedColor = clGreen;
		 tgridItemPrice->Buttons[i][0]->LatchedFontColor = clWhite;


         tgridItemPrice->Buttons[i][0]->Latched = false;
		 if (SelectedItem)
         {
			tgridItemPrice->Buttons[i][0]->Latched = original_discount_states[i] = SelectedItem->DiscountApplied((int)List->Objects[i]);
		 }
	  }
   }
   else
   {
	  pnlItemDiscountGridContainer->Visible = false;
   }

   if (SelectedItem)
   {
	  TSecurityReference *SecRef = SelectedItem->Security->SecurityGetType(secPriceAdjust);
	  if (SecRef != NULL)
	  {
		 tbtnReason->Caption = tbtnReason->Caption;
	  }
	  else
	  {
		 tbtnReason->Caption = "";
	  }

     original_price = SelectedItem->PriceEach();
	  NumpadDisplayItemPrice->SetNumeric(original_price);
   }

	pnlMiscPrice->Visible = ShowPriceAdjsut == true;

   UpdateDiscountDisplay();
   ShowModal();
}

void __fastcall TfrmSelectDiscounts::ItemPriceDescriptionClick(TObject *Sender)
{
   std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
   frmTouchKeyboard->MaxLength = 0;
   frmTouchKeyboard->AllowCarriageReturn = true;
   frmTouchKeyboard->CloseOnDoubleCarriageReturn = true;
   frmTouchKeyboard->KeyboardText = tbtnReason->Caption;
   frmTouchKeyboard->Caption = "Enter A Note";
   if (frmTouchKeyboard->ShowModal() == mrOk)
   {
	  tbtnReason->Caption = frmTouchKeyboard->memText->Text.Trim();
	  UpdateManualDiscount();
	  UpdateDiscountDisplay();
   }
}

// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDiscounts::FormResize(TObject *Sender)
{
  // if (Tag != Screen->Width)
 //  {
	  int Temp = Tag;
	  Tag = Screen->Width;
          if((double)Screen->Width / Screen->Height < 1.4)
          {
	  ScaleBy(Screen->Width, Temp);
          }
 //  }
   Left = (Screen->Width - Width) / 2;
   Top = (Screen->Height - Height) / 2;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDiscounts::tgridItemPriceMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift,
   TGridButton *GridButton)
{
    if((Member.ContactKey == 0) || Member.Points.PointsRulesSubs.Contains(eprAllowDiscounts))
    {
       Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
       DBTransaction.StartTransaction();
       DiscountSingleItem(DBTransaction, GridButton);
       DBTransaction.Commit();
    }
}

// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDiscounts::tbtnItemPriceDiscountClearMouseClick(TObject *Sender)
{
   if (SelectedItem != NULL)
   {
	  SelectedItem->DiscountsClear();
	  tbtnReason->Caption = "";
	  SelectedItem->Security->SecurityDelete(SelectedItem->Security->SecurityGetType(secPriceAdjust));

	  for (int i = 0; i < tgridItemPrice->RowCount; i++)
	  {
		 tgridItemPrice->Buttons[i][0]->Latched = false;
	  }
	  NumpadDisplayItemPrice->SetNumeric(SelectedItem->Price());
	  UpdateDiscountDisplay();
   }
}
// ---------------------------------------------------------------------------

AnsiString TfrmSelectDiscounts::DiscountSingleItem(Database::TDBTransaction &DBTransaction, TGridButton *GridButton)
{
   int DiscountKey = GridButton->Tag;
   AnsiString RetVal = "";
   if (SelectedItem != NULL)
   {
	  if (DiscountKey != 0)
	  {
		 std::auto_ptr <TList> ItemList(new TList);
		 ItemList->Add(SelectedItem);

		 TDiscount CurrentDiscount;
		 CurrentDiscount.DiscountKey = DiscountKey;
		 ManagerDiscount->GetDiscount(DBTransaction, DiscountKey, CurrentDiscount);

         TSCDPWDChecker SCDChecker;
         if(SCDChecker.SeniorCitizensCheckForItem(CurrentDiscount, SelectedItem) && SCDChecker.PWDCheck(CurrentDiscount, Orders)
             && SelectedItem->DiscountApplies(CurrentDiscount))
         {
             if (SelectedItem->DiscountApplied(DiscountKey))
             {
                SelectedItem->DiscountRemove(CurrentDiscount);
                GridButton->Latched = false;
                // Update the DB if required.
                if (Redirector->ItemType.Contains(itPrevItem))
                {
                   ManagerDiscount->SetDiscountAmountDB(DBTransaction, ItemList.get());
                }
             }
             else
             {
                ApplyDiscount(DBTransaction, DiscountKey, ItemList.get());
                RetVal = SelectedItem->DiscountReason;
                GridButton->Latched = true;

                // Update the DB if required.
                if (Redirector->ItemType.Contains(itPrevItem))
                {
                   ManagerDiscount->SetDiscountAmountDB(DBTransaction, ItemList.get());
                }
             }
         }
         else
         {
         	GridButton->Latched = false;
         }
	  }
	  else
	  {
		 SelectedItem->DiscountsClear();
		 GridButton->Latched = false;
	  }
	  UpdateDiscountDisplay();
   }
   return RetVal;
}

void __fastcall TfrmSelectDiscounts::ApplyDiscount(Database::TDBTransaction &DBTransaction, int DiscountKey, TList *Orders,
   TDiscountSource DiscountSource)
{
   if (Orders->Count > 0)
   {
	  bool ProcessDiscount = true;
	  TDiscount CurrentDiscount;

	  bool bailout = false;
	  CurrentDiscount.DiscountKey = DiscountKey;

	  if (ManagerDiscount->GetDiscount(DBTransaction, CurrentDiscount.DiscountKey, CurrentDiscount))
	  {
        if(CurrentDiscount.Source == dsMMUser)
        {
            CurrentDiscount.Source = DiscountSource;
        } // Id they selected members point discount but there is no member then bail out.
        else if(CurrentDiscount.Source == dsMMMebersPoints)
        {
            if(Member.Valid())
            {
                Currency ProductValue = TOrderUtils::TotalPriceSides(Orders);
                CurrentDiscount.MaximumValue = Member.Points.getPointsBalance(pasDatabase,ptstLoyalty);
                CurrentDiscount.Amount = ProductValue;
                ManagerDiscount->ClearDiscount(Orders, CurrentDiscount);
		 		ManagerDiscount->AddDiscount(Orders, CurrentDiscount);
            }
            ProcessDiscount = false;
        }

		 if (CurrentDiscount.Type == dtPromptDescription || CurrentDiscount.Type == dtPromptDescriptionAmount)
		 {
			std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
			do
			{
			   frmTouchKeyboard->MaxLength = 200;
			   frmTouchKeyboard->AllowCarriageReturn = false;
			   frmTouchKeyboard->StartWithShiftDown = true;
			   frmTouchKeyboard->MustHaveValue = true;
			   frmTouchKeyboard->KeyboardText = CurrentDiscount.Description;
			   frmTouchKeyboard->Caption = "Please enter a discount / surcharge Description";
			   if (frmTouchKeyboard->ShowModal() == mrOk)
			   {
				  CurrentDiscount.Description = frmTouchKeyboard->KeyboardText;
			   }
			   else
			   {
				  bailout = true;
			   }
			}
			while (frmTouchKeyboard->KeyboardText == "" && !bailout);

			if (bailout)
			{
			   ProcessDiscount = false;
			}
		 }
	  }
	  else
	  {
		 MessageBox("Member Discount not found in discount table.", "Error", MB_ICONWARNING + MB_OK);
		 ProcessDiscount = false;
	  }

	  if (ProcessDiscount && (CurrentDiscount.Type == dtPromptAmount || CurrentDiscount.Type == dtPromptDescriptionAmount))
	  {
		 std::auto_ptr <TfrmDiscount> frmDiscount(TfrmDiscount::Create <TfrmDiscount> (this));
		 frmDiscount->Mode = CurrentDiscount.Mode;
		 frmDiscount->CURInitial = CurrentDiscount.Amount;
		 frmDiscount->PERCInitial = CurrentDiscount.PercentAmount;

		 // Partial Payments.
		 // frmDiscount->TotalValue = InitialMoney.GrandTotal;

		 if (frmDiscount->ShowModal() == mrOk)
		 {
			CurrentDiscount.Mode = frmDiscount->Mode;
			if (frmDiscount->Mode == DiscModeCurrency || frmDiscount->Mode == DiscModePoints)
			{
			   CurrentDiscount.Amount = RoundToNearest(frmDiscount->CURResult, MIN_CURRENCY_VALUE, TGlobalSettings::Instance().MidPointRoundsDown);
               CurrentDiscount.OriginalAmount = CurrentDiscount.Amount;
			   if (CurrentDiscount.Amount != frmDiscount->CURResult)
			   {
				  MessageBox("The Discount has been rounded!.", "Warning", MB_ICONWARNING + MB_OK);
			   }
			}

            else if (frmDiscount->Mode == DiscModeItem)
			{
			   CurrentDiscount.Amount = RoundToNearest(frmDiscount->CURResult, MIN_CURRENCY_VALUE, TGlobalSettings::Instance().MidPointRoundsDown);
               CurrentDiscount.OriginalAmount = CurrentDiscount.Amount;

			   if (CurrentDiscount.Amount != frmDiscount->CURResult)
			   {
				  MessageBox("The Discount has been rounded!.", "Warning", MB_ICONWARNING + MB_OK);
			   }
			}

			else if (frmDiscount->Mode == DiscModeSetPrice || frmDiscount->Mode == DiscModeCombo)
			{
			   CurrentDiscount.Amount = RoundToNearest(frmDiscount->CURResult, MIN_CURRENCY_VALUE, TGlobalSettings::Instance().MidPointRoundsDown);
               CurrentDiscount.OriginalAmount = CurrentDiscount.Amount;
			   if (CurrentDiscount.Amount != frmDiscount->CURResult)
			   {
				  MessageBox("The Discount has been rounded!.", "Warning", MB_ICONWARNING + MB_OK);
			   }
			}
			else
			{
			   CurrentDiscount.PercentAmount = frmDiscount->PERCResult;
               if(frmDiscount->Mode == DiscModePercent)
               {
                  CurrentDiscount.OriginalAmount = CurrentDiscount.PercentAmount;
               }
			}
		 }
		 else
		 {
			ProcessDiscount = false;
		 }
	  }

	  if (ProcessDiscount)
	  {
         CurrentDiscount.DiscountAppliedTime = Now();
		 ManagerDiscount->ClearDiscount(Orders, CurrentDiscount);
		 ManagerDiscount->AddDiscount(Orders, CurrentDiscount);
	  }
   }
}

void __fastcall TfrmSelectDiscounts::UpdateDiscountDisplay()
{
   // Display the Item Details If any
   std::auto_ptr <TStringList> Report(new TStringList);
   ManagerDiscount->GetReportItemDiscountsHeader(Report.get());
   ManagerDiscount->GetReportItemDiscounts(SelectedItem, Report.get());
   ManagerDiscount->GetReportItemDiscountsFooter(Report.get());
   Navigate(webDisplay, Report.get());

}

void TfrmSelectDiscounts::Navigate(TWebBrowser *WebBrowser, TStringList *Html)
{
   std::auto_ptr <TMemoryStream> StreamRpt(new TMemoryStream);
   Html->SaveToStream(StreamRpt.get());
   IPersistStreamInit *psi;
   StreamRpt->Seek(0, 0);
   TStreamAdapter *sa = new TStreamAdapter(StreamRpt.get(), soReference);
   _di_IDispatch doc = WebBrowser->Document;
   if (doc == NULL)
   {
	  OleVariant url = "about:blank";
	  WebBrowser->Navigate2(url);
	  while (doc == NULL)
	  {
		 Application->ProcessMessages();
		 doc = WebBrowser->Document;
	  }
   }

   TComInterface <IHTMLDocument2> HTMLDocument;
   TComInterface <IHTMLWindow2> parentWindow;

   if (SUCCEEDED(WebBrowser->Document->QueryInterface(IID_IHTMLDocument2, (LPVOID*) & HTMLDocument)))
   {
	  IHTMLElement* pBodyElem = 0;
	  HRESULT hr = HTMLDocument->get_body(&pBodyElem);
	  if (SUCCEEDED(hr) && pBodyElem != NULL)
	  {
		 IHTMLBodyElement* pBody = 0;
		 hr = pBodyElem->QueryInterface(IID_IHTMLBodyElement, (void**) & pBody);
		 if (SUCCEEDED(hr) && pBody != NULL)
		 {
			// hide 3D border
			IHTMLStyle* pStyle = 0;
			hr = pBodyElem->get_style(&pStyle);
			if (SUCCEEDED(hr) && pStyle != NULL)
			{
			   pStyle->put_borderStyle(BSTR("none"));
			   pStyle->Release();
			}

			// hide scrollbars
			pBody->put_scroll(BSTR("no"));
		 }
		 pBodyElem->Release();
	  }
   }

   if (doc->QueryInterface(IID_IPersistStreamInit, (void**) & psi) == S_OK)
   {
	  if (psi)
		 psi->Load(*sa);
   }
}

void __fastcall TfrmSelectDiscounts::TouchNumpadItemPriceClick(TObject *Sender, TNumpadKey Key)
{
   // UpdateManualDiscount();
   UpdateDiscountDisplay();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmSelectDiscounts::FormShow(TObject *Sender)
{
   FormResize(Sender);
   SetGridColors(tgridItemPrice);
   SetNumpadColors(TouchNumpadItemPrice);
}
// ---------------------------------------------------------------------------

void TfrmSelectDiscounts::SetMember(TMMContactInfo inMember)
{
	Member = inMember;
}

