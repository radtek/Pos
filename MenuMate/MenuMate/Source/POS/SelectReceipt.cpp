//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SelectReceipt.h"
#include "MMMessageBox.h"
#include "MMTouchNumpad.h"
#include "ReceiptManager.h"
#include "VerticalSelect.h"
#include "ReferenceManager.h"
#include "DeviceRealTerminal.h"
#include "MMTouchKeyboard.h"
#include "DBAdyen.h"


#define RECEIPT_DELIMITER "#####"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
TfrmSelectReceipt *frmSelectReceipt;
//---------------------------------------------------------------------------


void __fastcall TfrmSelectReceipt::FormCreate(TObject *Sender)
{
	FormResize(this);
}

//---------------------------------------------------------------------------

__fastcall TfrmSelectReceipt::TfrmSelectReceipt(TComponent* Owner)
	: TZForm(Owner)
{
	ManagerReceipt->AllTerminals = true;
	ManagerReceipt->Date = Date();
   calReceipt->Date = ManagerReceipt->Date;
}

__fastcall TfrmSelectReceipt::~TfrmSelectReceipt()
{
	ManagerReceipt->Close();
}
//---------------------------------------------------------------------------
void TfrmSelectReceipt::Execute()
{
	if (!ManagerReceipt->ReceiptsExist())
	{
		MessageBox("There are no receipts to print for this terminal.",
										"Warning", MB_ICONWARNING + MB_OK);

	}
   CurrentDate = Date();
	ShowModal();
}
//---------------------------------------------------------------------------
void TfrmSelectReceipt::Execute(AnsiString ReceiptNumber)
{
   CurrentDate = Date();
   if(ManagerReceipt->Find(ReceiptNumber))
   {
      ShowReceipt();
	   ShowModal();
   }
}
//---------------------------------------------------------------------------

void TfrmSelectReceipt::ShowReceipt()
{
	TStringList *Lines = new TStringList;
	try
	{
		ManagerReceipt->Get(Lines);
//        MessageBox(Lines->Count,"Count",MB_OK);
//        for(int i = 0; i <Lines->Count; i++)
//        {
//            MessageBox(Lines->Strings[i],"",MB_OK);
//        }
//        MessageBox(ManagerReceipt->InvoiceNumber,"InvoiceNumber",MB_OK);
		memReceipt->Lines->Assign(Lines);
		toggleAvailabilityOfTippingButton();
	}
	__finally
	{
		delete Lines;
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmSelectReceipt::btnPrintClick(TObject *Sender)
{
    if(TGlobalSettings::Instance().EnableCompanyDetailOnReprintReceipt)
    {
        std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
        frmTouchKeyboard->AllowCarriageReturn = true;
        frmTouchKeyboard->CloseOnDoubleCarriageReturn = false;
        frmTouchKeyboard->StartWithShiftDown = false;
        frmTouchKeyboard->MaxLength = 300;
        frmTouchKeyboard->Caption = "Enter the Company Details.";
        if (frmTouchKeyboard->ShowModal() == mrOk)
        {
          TGlobalSettings::Instance().CompanydetailsSaved = frmTouchKeyboard->KeyboardText.Trim() ;
           ManagerReceipt->Print();
        }
        TGlobalSettings::Instance().CompanydetailsSaved ="";
    }
    else
    {
      ManagerReceipt->Print();
    }


}
//---------------------------------------------------------------------------
/*
void TfrmSelectReceipt::PrintReceipt(TStringList *Receipt)
{
	if (TComms::Instance().ReceiptPrinter == NULL)			throw Exception("No receipt printer selected.");

	TPrintFormat *pPrinter							= new TPrintFormat;
	try
	{
		pPrinter->PrintRaw(ManagerReceipt->Receipt, TComms::Instance().ReceiptPrinter);
	}
	__finally
	{
		delete pPrinter;
	}
}
*/
//---------------------------------------------------------------------------
void __fastcall TfrmSelectReceipt::btnCloseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectReceipt::calReceiptClick(TObject *Sender)
{
	CurrentDate = calReceipt->Date;
}
//---------------------------------------------------------------------------


void TfrmSelectReceipt::SortTables()
{
}

void __fastcall TfrmSelectReceipt::sbAllClick(TObject *Sender)
{
	ManagerReceipt->AllTerminals = true;
	ManagerReceipt->Open();
	ShowReceipt();

   btnNext->Enabled = false;
   btnNext->Color = clMaroon;

	if(!ManagerReceipt->IsEmpty())
   {
      btnPrev->Enabled = true;
      btnPrev->Color = clNavy;
      ShowReceipt();
   }
   else
   {
		btnPrev->Enabled = false;
      btnPrev->Color = clMaroon;
   }
}
//---------------------------------------------------------------------------




void __fastcall TfrmSelectReceipt::FormResize(TObject *Sender)
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
	ClientWidth = Panel1->Width;
	ClientHeight = Panel1->Height;
	Left = (Screen->Width - Width) / 2;
	Top = (Screen->Height - Height) / 2;

}
//---------------------------------------------------------------------------


void __fastcall TfrmSelectReceipt::FormShow(TObject *Sender)
{
	FormResize(this);
	SetWindowPos(Handle, HWND_TOP	, Left, Top, Width, Height, 0);
}
//---------------------------------------------------------------------------



void TfrmSelectReceipt::SetCurrentDate(TDateTime SelectedDate)
{
	ManagerReceipt->Date = SelectedDate;
	btnNext->Enabled = false;
	btnNext->Color = clMaroon;

	if(!ManagerReceipt->IsEmpty())
	{
		btnPrev->Enabled = true;
		btnPrev->Color = clNavy;
	}
	else
	{
		btnPrev->Enabled = false;
		btnPrev->Color = clMaroon;
	}
	ShowReceipt();	
}




void __fastcall TfrmSelectReceipt::sbThisTerminalClick(TObject *Sender)
{
	ManagerReceipt->AllTerminals = false;
	ManagerReceipt->Open();
	ShowReceipt();

	btnNext->Enabled = false;
	btnNext->Color = clMaroon;

	if(!ManagerReceipt->IsEmpty())
	{
		btnPrev->Enabled = true;
		btnPrev->Color = clNavy;
		ShowReceipt();
	}
	else
	{
		btnPrev->Enabled = false;
		btnPrev->Color = clMaroon;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectReceipt::btnNextMouseClick(TObject *Sender)
{
	ManagerReceipt->Prev();
	if(!ManagerReceipt->Bof())
	{
		btnPrev->Enabled = true;
		btnPrev->Color = clNavy;
	}
	else
	{
		btnNext->Enabled = false;
		btnNext->Color = clMaroon;
	}
	ShowReceipt();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectReceipt::btnPrevMouseClick(TObject *Sender)
{
	ManagerReceipt->Next();
	if(!ManagerReceipt->Eof())
	{
		btnNext->Enabled = true;
		btnNext->Color = clNavy;
	}
	else
	{
		btnPrev->Enabled = false;
		btnPrev->Color = clMaroon;
	}
	ShowReceipt();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectReceipt::btnSearchMouseClick(TObject *Sender)
{
	std::auto_ptr<TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));
	std::auto_ptr<TStringList> List(new TStringList);

	TVerticalSelection Item;
	Item.Title = "Cancel";	Item.Properties["Color"] = clMaroon; Item.CloseSelection = true;
	SelectionForm->Items.push_back(Item);

	Item.Title = "Receipt No.";	Item.Properties["RefKey"] = 0; Item.Properties["Color"] = clInfoBk; Item.CloseSelection = true;
	SelectionForm->Items.push_back(Item);

	if(TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Enabled"])
	{
		Item.Title = "Member Receipts";	Item.Properties["RefKey"] = -1; Item.Properties["Color"] = clInfoBk; Item.CloseSelection = true;
		SelectionForm->Items.push_back(Item);
	}

	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();

	TManagerReference ManagerReference;
	ManagerReference.GetReferences(DBTransaction,List.get());

	for (int i = 0; i < List->Count ; i++)
	{
		Item.Title = List->Strings[i];
		Item.Properties["RefKey"] = (int)List->Objects[i];
		Item.Properties["Color"] = clInfoBk;
		Item.CloseSelection = true;
		SelectionForm->Items.push_back(Item);
	}


	SelectionForm->ShowModal();
	TVerticalSelection SelectedItem;
	if(SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel" )
	{
		int RefKey = SelectedItem.Properties["RefKey"];
		if(RefKey == 0)
		{
			std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
			frmTouchNumpad->Caption = "Enter the Receipt Number";
			frmTouchNumpad->btnSurcharge->Caption = "Ok";
			frmTouchNumpad->btnSurcharge->Visible = true;
			frmTouchNumpad->btnDiscount->Visible = false;
			frmTouchNumpad->Mode = pmNumber;
			frmTouchNumpad->INTInitial = 0;

			if (frmTouchNumpad->ShowModal() == mrOk)
			{
				SearchUsingTransactionNumber(IntToStr(frmTouchNumpad->INTResult));
				AnsiString Invoice = IntToStr(frmTouchNumpad->INTResult);
				if(ManagerReceipt->Find(Invoice))
				{
					ShowReceipt();
				}
				else
				{
					MessageBox("Receipt Number not found.",
					"Receipt Not Found", MB_ICONINFORMATION + MB_OK);
				}
			}
		}
		else if(RefKey == -1)
		{
			TMMContactInfo TempUserInfo;
			Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
			TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
			DBTransaction.StartTransaction();
			eMemberSource MemberSource;
			TLoginSuccess Result = TDeviceRealTerminal::Instance().ManagerMembership->GetMember(DBTransaction,TempUserInfo,MemberSource);
			if(Result == lsAccepted || Result == lsAccountBlocked )
			{
				if(ManagerReceipt->FindMember(TempUserInfo.ContactKey))
				{
					ShowReceipt();
				}
				else
				{
					MessageBox("Receipt Number not found.",
					"Receipt Not Found", MB_ICONINFORMATION + MB_OK);
				}
			}
		}
		else
		{
			TReference SelectedRef = ManagerReference.GetReference(DBTransaction,RefKey);
			std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
			frmTouchKeyboard->MaxLength = 25;
			frmTouchKeyboard->AllowCarriageReturn = false;
			frmTouchKeyboard->StartWithShiftDown = false;
			if(SelectedRef.Type == REFTYPE_PMS)
			{
				frmTouchKeyboard->KeyboardText = "PMS";
			}
			else
			{
				frmTouchKeyboard->KeyboardText = "";
			}
			frmTouchKeyboard->Caption = "Enter The " + SelectedRef.Name + " Number";
			if (frmTouchKeyboard->ShowModal() == mrOk)
			{
				if(ManagerReceipt->FindRef(frmTouchKeyboard->KeyboardText))
				{
					ShowReceipt();
				}
				else
				{
					MessageBox("Receipt Reference not found.",
					"Reference Not Found", MB_ICONINFORMATION + MB_OK);
				}
			}
		}
	}
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------



void __fastcall TfrmSelectReceipt::btnPrevAutoRepeat(TObject *Sender)
{
	ManagerReceipt->Next();
	if(!ManagerReceipt->Eof())
	{
		btnNext->Enabled = true;
		btnNext->Color = clNavy;
	}
	else
	{
		btnPrev->Enabled = false;
		btnPrev->Color = clMaroon;
		btnNext->Enabled = true;
		btnNext->Color = clNavy;
	}
	ShowReceipt();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectReceipt::btnNextAutoRepeat(TObject *Sender)
{
	ManagerReceipt->Prev();
	if(!ManagerReceipt->Bof())
	{
		btnPrev->Enabled = true;
		btnPrev->Color = clNavy;
	}
	else
	{
		btnNext->Enabled = false;
		btnNext->Color = clMaroon;
		btnPrev->Enabled = true;
		btnPrev->Color = clNavy;
	}
	ShowReceipt();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectReceipt::btnBillDownMouseClick(TObject *Sender)
{
	memReceipt->Perform(WM_VSCROLL, SB_LINEDOWN, 0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectReceipt::btnBillUpMouseClick(TObject *Sender)
{
	memReceipt->Perform(WM_VSCROLL, SB_LINEUP, 0);
}
//---------------------------------------------------------------------------

void TfrmSelectReceipt::toggleAvailabilityOfTippingButton()
{
	WideString paymentRefNumber = "";
	Currency originalVisaPaymentAmount;
	int arcBillKey = -1;
    bool canTipBeApplied = ManagerReceipt->CanApplyTipOnThisReceiptsTransaction(paymentRefNumber,originalVisaPaymentAmount,arcBillKey);

	if(((TGlobalSettings::Instance().EnableEftPosAdyen && !TDBAdyen::IsTipFromReceiptAlreadyAdded(arcBillKey)) ||
            (!TGlobalSettings::Instance().EnableEftPosAdyen && TDeviceRealTerminal::Instance().PaymentSystem->AllowsTipsOnTransactions())) && canTipBeApplied)
	{
		btnAddTip->Enabled = true;
	}
	else
		btnAddTip->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectReceipt::btnAddTipMouseClick(TObject *Sender)
{
	WideString paymentRefNumber = "";
	Currency originalVisaPaymentAmount;
	int arcBillKey = -1;

	bool isMasterOrVisaCard = ManagerReceipt->CanApplyTipOnThisReceiptsTransaction(paymentRefNumber,originalVisaPaymentAmount,arcBillKey);

	if(paymentRefNumber.Length() == 0 )
	{
		MessageBox("Cannot add a tip on this transaction. The payment reference number is missing",
						"Cannot add a tip", MB_ICONINFORMATION + MB_OK);
		return;
	}

    if (MessageBox("Are you sure you wish to add a tip on this transaction ?" ,"Add a tip to transaction", MB_OKCANCEL + MB_ICONQUESTION) == IDOK)
	{
        std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (Screen->ActiveForm));
        frmTouchNumpad->Caption = "Enter Tip Amount";
        frmTouchNumpad->CURInitial = 0.00;
        frmTouchNumpad->btnSurcharge->Caption = "Ok";
        frmTouchNumpad->btnDiscount->Visible = false;
        frmTouchNumpad->btnSurcharge->Visible = true;
        frmTouchNumpad->Mode = pmCurrency;
        if (frmTouchNumpad->ShowModal() == mrOk)
        {
            Currency tipAmount;
            tipAmount = frmTouchNumpad->CURResult;
            if(tipAmount.Val > 0)
            {
                double percentageOfIncrement = (tipAmount / originalVisaPaymentAmount) * 100;
                int maxPercentageOfIncrement = 50;
                bool isAnyTipLimitExist = true;

                if(TGlobalSettings::Instance().EnableEftPosAdyen)
                {
                    if(TGlobalSettings::Instance().EnableAdjustAuthorisationOnCards || isMasterOrVisaCard)
                        isAnyTipLimitExist = false;
                    else
                        maxPercentageOfIncrement = 100;
                }

                if(percentageOfIncrement > maxPercentageOfIncrement && isAnyTipLimitExist)
                {
                    MessageBox("You cannot add a tip more than " + IntToStr(maxPercentageOfIncrement) + "% of original payment.",
                                    "Failed to add a tip", MB_ICONERROR + MB_OK);
                }
                else if( !TDeviceRealTerminal::Instance().PaymentSystem->ProcessTipOnVisaTransaction( arcBillKey, paymentRefNumber, originalVisaPaymentAmount, tipAmount ))
                {
                    MessageBox("Failed to add a tip on this transaction", "Failed to add a tip", MB_ICONERROR + MB_OK);
                }
                else
                    MessageBox("Tip applied successfully", "Tip Applied", MB_ICONINFORMATION + MB_OK);
                    if(TGlobalSettings::Instance().EnableEftPosAdyen && TGlobalSettings::Instance().EnableEftPosPreAuthorisation)
                      ManagerReceipt->PrintDocketForTips(arcBillKey,tipAmount);

            }
        }
        toggleAvailabilityOfTippingButton();
	}
}
//---------------------------------------------------------------------------
void TfrmSelectReceipt::SearchUsingTransactionNumber(UnicodeString inTransactionNumber)
{
    if(ManagerReceipt->Find(inTransactionNumber))
    {
        ShowReceipt();
    }
    else
    {
       MessageBox("Receipt Number not found.", "Receipt Not Found", MB_ICONINFORMATION + MB_OK);
    }
}
//----------------------------------------------------------------------------------------------

