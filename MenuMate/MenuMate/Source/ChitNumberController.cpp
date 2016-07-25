// ---------------------------------------------------------------------------

#pragma hdrstop

#include "ChitNumberController.h"
#include "ChitNumberGui.h"
#include "MMMessageBox.h"
#include "VerticalSelect.h"
#include "MMTouchNumpad.h"
#include "MMTouchKeyboard.h"
#include "ManagerVariable.h"
#include "TerminalChitConfig.h"
#include "VerticalSelectUtilities.h"
#include "ChitList.h"

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef  PalmMate
#include "Palm.h"
#endif

// ---------------------------------------------------------------------------

#pragma package(smart_init)

TChitNumberController::TChitNumberController(TForm *inDisplayOwner,Database::TDBTransaction &inDBTransaction) :
frmListManager(new TfrmListManager(inDisplayOwner)),
DBTransaction(inDBTransaction)
{
   Enabled = true;//TManagerVariable::Instance().GetBool(DBTransaction, vmUseChitNumbers, true);
   DisplayOwner = inDisplayOwner;
}

//change here for process web order..
TChitNumberController::TChitNumberController(Database::TDBTransaction &inDBTransaction) :
DBTransaction(inDBTransaction)
{

}


TChitNumberController::~TChitNumberController()
{
}

void TChitNumberController::Run()
{
   TManagerChitNumber::Instance().LoadAll(DBTransaction);
   frmListManager->Add.RegisterForEvent(OnAdd);
   frmListManager->Edit.RegisterForEvent(OnEdit);
   frmListManager->Delete.RegisterForEvent(OnDelete);
   frmListManager->Active.RegisterForEvent(OnActivate);
   frmListManager->Close.RegisterForEvent(OnClose);
   PopulateListManager();
   frmListManager->SetCaption("Chit Numbers");
   frmListManager->btnActive->Visible = true;
   frmListManager->btnActive->Enabled = true;
   frmListManager->btnActive->Caption = "Terminals";

    frmListManager->ShowModal();
}

void TChitNumberController::OnClose(int SyndKey, int ColIndex)
{
	frmListManager->ModalResult = mrOk;
}

void TChitNumberController::OnAdd(int SyndKey, int ColIndex)
{
    std::auto_ptr<TfrmChitNumberGui> dialog(
      new TfrmChitNumberGui(DisplayOwner, TChitNumber()));

    if (dialog->ShowModal() != mrOk)
        return;

    TManagerChitNumber::Instance().Add(DBTransaction,
                                       dialog->ChitNumber(),
                                       dialog->OptionsToBeAdded());
    PopulateListManager();
}

void TChitNumberController::OnEdit(int ChitNumber, int ColIndex)
{
    std::auto_ptr <TfrmChitNumberGui> dialog(
      new TfrmChitNumberGui(
            DisplayOwner,
            TManagerChitNumber::Instance().ChitNumberByKey(ChitNumber)));
    if (dialog->ShowModal() != mrOk)
        return;

    TManagerChitNumber::Instance().Update(DBTransaction,
                                          dialog->ChitNumber(),
                                          dialog->OptionsToBeAdded(),
                                          dialog->OptionsToBeDeleted(),
                                          dialog->OptionsToBeRenamed());
    PopulateListManager();
}

void TChitNumberController::OnDelete(int ChitNumberKey, int ColIndex)
{
    if (MessageBox("Are you sure you wish to remove this Chit Number?",
                  "Warning",
                  MB_OKCANCEL | MB_ICONQUESTION)
       == IDOK) {
	  TManagerChitNumber::Instance().Remove(DBTransaction, ChitNumberKey);
	  PopulateListManager();
                 }
}

void TChitNumberController::OnActivate(int ChitNumberKey, int ColIndex)
{
   std::auto_ptr <TfrmTerminalChitConfig> frmTerminalChitConfig(new TfrmTerminalChitConfig(DisplayOwner,DBTransaction));
   frmTerminalChitConfig->ShowModal();
   PopulateListManager();
}

void TChitNumberController::PopulateListManager()
{
   try
   {
	  frmListManager->sgDisplay->ColCount = 1;
	  frmListManager->sgDisplay->Cols[0]->Clear();
	  frmListManager->sgDisplay->RowCount = TManagerChitNumber::Instance().Size();
	  for (TManagerChitNumber::Instance().First(false); !TManagerChitNumber::Instance().Eof(); TManagerChitNumber::Instance().Next(false))
	  {
		 TChitNumber ChitNumber =
           TManagerChitNumber::Instance().ChitNumber();
		 frmListManager->sgDisplay->Cols[0]->AddObject(
            ChitNumber.Name,
            reinterpret_cast<TObject *>(ChitNumber.ChitNumberKey));
	  }

   }
   catch(Exception & E)
   {
	  ShowMessage(E.Message);
   }
}

ChitResult TChitNumberController::GetDefaultChitNumber(TChitNumber &ChitNumber)
{
   if (!Enabled)
   {
	  return ChitDisabled;
   }
   else
   {
	  ChitNumber = TManagerChitNumber::Instance().GetDefault();
	  if (ChitNumber.Valid())
	  {
		 return ChitOk;
	  }
	  else
	  {
		 return ChitCancelled;
	  }
   }
}

const chit_option_name_t
get_chit_option(const chit_key_t chit_key)
{
	TManagerChitNumber::Instance().SetLastAccessedChitKey(chit_key);

	std::auto_ptr<chit_option_name_vector_t> names(db_chit_t::option_names(chit_key));

	std::auto_ptr<TfrmChitList> menu(new TfrmChitList(0x0, *names));

	menu->Caption = "Select a chit option";

	if (menu->ShowModal() == mrCancel)
	{
		return L"";
	}

	UnicodeString selectedChitName = menu->SelectedChitName;

    return selectedChitName.c_str();
}

ChitResult TChitNumberController::GetChitNumber(bool Prompt, TChitNumber &ChitNumber)
{
   ChitResult ChitNumberReturned = ChitCancelled;
   if (!Enabled)
   {
	  return ChitDisabled;
   }
   else if (Prompt)
   {
	  // Toggle or prompt.
	  if (TManagerChitNumber::Instance().Size() == 1)
	  {
		 TManagerChitNumber::Instance().First(false);

		 ChitNumber = TManagerChitNumber::Instance().ChitNumber();
         ChitNumberReturned = ChitOk;

		 if (!ChitNumber.Assigned())
			ChitNumberReturned = GetNextChitNumber(ChitNumber);

         if (ChitNumber.Format == ectList)
            ChitNumber.ActiveOption(
			  get_chit_option(ChitNumber.ChitNumberKey).c_str());

         return ChitNumberReturned;
	  }
	  else
	  {
		 // Display Reports List
		 std::auto_ptr <TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create <TfrmVerticalSelect> (DisplayOwner));
         SelectionForm->Caption = "Select a chit";

		 TVerticalSelection Item;
		 Item.Title = "Cancel";
		 Item.Properties["Color"] = IntToStr(clMaroon);
		 Item.CloseSelection = true;
		 SelectionForm->Items.push_back(Item);

		 for (TManagerChitNumber::Instance().First(false); !TManagerChitNumber::Instance().Eof(); TManagerChitNumber::Instance().Next(false))
		 {
			TChitNumber ChitNumber = TManagerChitNumber::Instance().ChitNumber();

			Item.Title = ChitNumber.Name;
			Item.Properties["Action"] = IntToStr(ChitNumber.ChitNumberKey);
			Item.Properties["Color"] = IntToStr(clNavy);
			Item.CloseSelection = true;
			SelectionForm->Items.push_back(Item);
		 }

		 SelectionForm->ShowModal();
		 TVerticalSelection SelectedItem;
		 if (SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel")
		 {
			int Action = StrToIntDef(SelectedItem.Properties["Action"], 0);
			ChitNumber = TManagerChitNumber::Instance().ChitNumberByKey(Action); //Load chit taxes values...




			if (GetNextChitNumber(ChitNumber))
			   ChitNumberReturned = ChitOk;

            if (ChitNumber.Format == ectList)
                ChitNumber.ActiveOption(
				  get_chit_option(ChitNumber.ChitNumberKey).c_str());


		 }
	  }
   }
   else if (ChitNumber.Valid())
   {
	  if (!ChitNumber.Assigned())
	  {
		 ChitNumberReturned = GetNextChitNumber(ChitNumber);
	  }
	  else
	  {
		 ChitNumberReturned = ChitOk;
	  }
   }
   else
   {
	  ChitNumber = TManagerChitNumber::Instance().GetDefault();
	  if (ChitNumber.Valid())
	  {
		 ChitNumberReturned = GetNextChitNumber(ChitNumber);
	  }
	  else
	  {
		 ChitNumberReturned = ChitNone;
	  }
   }

   return ChitNumberReturned;
}

ChitResult TChitNumberController::GetNextChitNumber(TChitNumber &Chit)
{
    ChitResult ChitNumberReturned = ChitCancelled;
    if (Chit.Type == ectAuto || Chit.Format == ectList || Chit.OnlineDeliveryOrder)
    {
      TManagerChitNumber::Instance().GetNextChitNumber(DBTransaction, Chit);
      return ChitOk;
    }
    if(TGlobalSettings::Instance().AutoAcceptWebOrders)
    {
        if(Chit.OnlinePickupOrder)
        {
            TManagerChitNumber::Instance().GetNextChitNumber(DBTransaction, Chit);
            return ChitOk;
        }
    }

    if (Chit.Format == ectNumeric)
    {
         std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (DisplayOwner));
         frmTouchNumpad->Caption = "Enter the " + Chit.Name + " Number";
         frmTouchNumpad->btnSurcharge->Caption = "Ok";
         frmTouchNumpad->btnSurcharge->Visible = true;
         frmTouchNumpad->btnDiscount->Visible = false;
         frmTouchNumpad->Mode = pmNumber;
         frmTouchNumpad->INTInitial = 0;
         if (frmTouchNumpad->ShowModal() == mrOk)
         {
            Chit.ChitNumber = IntToStr(frmTouchNumpad->INTResult);
            ChitNumberReturned = ChitOk;
         }
    }
    else
    {
        std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (Screen->ActiveForm));
        frmTouchKeyboard->MaxLength = 15;
        frmTouchKeyboard->AllowCarriageReturn = false;
        frmTouchKeyboard->StartWithShiftDown = false;
        frmTouchKeyboard->KeyboardText = "";
        frmTouchKeyboard->Caption = "Enter the " + Chit.Name + " Text/Number";

        if (frmTouchKeyboard->ShowModal() == mrOk)
        {
            Chit.ChitNumber = frmTouchKeyboard->KeyboardText;
            ChitNumberReturned = ChitOk;
        }
    }
    return ChitNumberReturned;
}




