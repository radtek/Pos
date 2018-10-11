//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ConfirmOrd.h"
#include "MMTouchKeyboard.h"
#include "Rooms.h"
#include "TableManager.h"
#include "MMMessageBox.h"
#include "ContactStaff.h"
#include "GUIScale.h"
//#include "DBClippTab.h"
#include "DBTab.h"
#include "DeviceRealTerminal.h"
#include "DBTab.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "IdBaseComponent"
#pragma link "IdComponent"
#pragma link "IdHTTP"
#pragma link "IdTCPClient"
#pragma link "IdTCPConnection"
#pragma resource "*.dfm"

TForm * TfrmConfirmOrder::WinOwner = NULL;
//---------------------------------------------------------------------------
__fastcall TfrmConfirmOrder::TfrmConfirmOrder(TComponent* Owner,TSaveOrdersTo &inOrderContainer)
	: TForm(Owner), OrderContainer(inOrderContainer)
{
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TfrmConfirmOrder::CreateParams(Controls::TCreateParams &params)
{ 
	TForm::CreateParams( params );
	if( WinOwner )
	{
		params.WndParent = WinOwner->Handle;
		WinOwner = NULL;
	}
}
//---------------------------------------------------------------------------
TfrmConfirmOrder *TfrmConfirmOrder::Create(TForm* Owner,TSaveOrdersTo &inOrderContainer)
{
	WinOwner = Owner;
	return new TfrmConfirmOrder(Owner,inOrderContainer);
}
//---------------------------------------------------------------------------
void __fastcall TfrmConfirmOrder::tbCancelClick(TObject *Sender)
{
    DeleteFile(applicationDirectory + "\\" + "clippImage.jpg");
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TfrmConfirmOrder::FormClose(TObject *Sender,
		TCloseAction &Action)
{
	lbParty->Caption = "";
	lbTableNo->Caption = "";
	tbSavePrint->Visible = true;
	tbtnSave->Visible = true;
	btnSaveAndBill->Visible = true;
	ReceiptDisplay->Clear();
}
//---------------------------------------------------------------------------


void __fastcall TfrmConfirmOrder::FormShow(TObject *Sender)
{
	FormResize(Sender);

    //Register the database transaction..
//    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
//    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
//    dbTransaction.StartTransaction();
//
//    //Check selected tab is clipp tab
//    int tabType =  TDBClippTab::GetClippTabByTabKey(dbTransaction, OrderContainer.Location["TabKey"]);
//
//    if(tabType == 13)
//    {
//        //Get customerphotourl
//        UnicodeString customerPhotoUrl = TDBClippTab::GetClippCustomerPhotoURl(dbTransaction, OrderContainer.Location["TabKey"]);
//
//        //Get customer name by tab key
//         UnicodeString customerName = TDBClippTab::GetCustomerName(dbTransaction, OrderContainer.Location["TabKey"]);
//
//        //Get clipp tab Limit
//        Currency creditLimit = TDBTab::GetTabCreditLimit(dbTransaction, OrderContainer.Location["TabKey"]);
//
//        lbTabLimit->Caption = customerName + "'s Tab Limit: " ;
//        lbLimitBal->Caption = FloatToStrF(creditLimit, ffNumber, 15, 2);
//
//        if(customerPhotoUrl != "")
//        {
//            //loading clipp customer's image
//            TMemoryStream *PicStream = new TMemoryStream();
//
//            // Creating path for storing image
//            applicationDirectory = ExtractFilePath(Application->ExeName);
//
//            //Check if directory not exist than create it.
//            if (!DirectoryExists(applicationDirectory))
//            {
//                CreateDir(applicationDirectory);
//            }
//
//            //Name of Image will be  clippImage.jpg
//            UnicodeString filename = applicationDirectory + "\\" + "clippImage.jpg";
//            IdHTTP1->Get(customerPhotoUrl, PicStream );
//
//            //save image to specified path
//            PicStream->SaveToFile(filename);
//            //Loading image from file
//            Image1->Picture->LoadFromFile(filename);
//
//            delete PicStream;
//        }
//    }
//     // commit the transaction
//     dbTransaction.Commit();

    lbTabLimit->Visible = (lbTabLimit->Caption == "") ? false : true;
    lbLimitBal->Visible = (lbLimitBal->Caption == "") ? false : true;

	lbParty->Caption = OrderContainer.Location["PartyName"];
	lbParty->Visible = (lbParty->Caption == "") ? false : true;
    tbtnPartyName->Caption = "Set Party Name\r" + OrderContainer.Location["PartyName"];
    OrderContainer.Location["OriginalPartyName"] = OrderContainer.Location["PartyName"];

	Caption = "Save orders to " + OrderContainer.Location["ContainerName"];
	lbTableNo->Caption = OrderContainer.Location["ContainerName"];
	LabelSeat->Caption = OrderContainer.Location["TabName"];
	if(LabelSeat->Caption == lbTableNo->Caption)
	{
		LabelSeat->Visible = false;
	}
	else
	{
   	LabelSeat->Visible = true;
	}
	tbCancel->Caption = "Cancel";
	tbSavePrint->Visible = true;
	btnSaveAndBill->Visible = true;

	tntReceipt->Clear();
	for (int i = 0; i < ReceiptDisplay->Count ; i++)
	{
		tntReceipt->Lines->Add(ReceiptDisplay->Strings[i]);
	}

   tbtnPartyName->Enabled = OrderContainer.AllowPartyNameChanged;

   sgTotals->DefaultColWidth = pnlInfo->Width / 2;
   sgTotals->RowCount = OrderContainer.Values.size();

   sgTotals->ColWidths[0] = sgTotals->ClientWidth * 3/5;
   sgTotals->ColWidths[1] = sgTotals->ClientWidth - sgTotals->ColWidths[0];

   for (int i = 0; i < sgTotals->RowCount ; i++)
   {
		sgTotals->Cells[0][i] = "";
		sgTotals->Cells[1][i] = "";
	}

   std::vector<TSaveOrdersTo::StringValuePair>::iterator itValues;
   int i = 0;
   for (itValues = OrderContainer.Values.begin(); itValues != OrderContainer.Values.end();itValues++,i++)
   {

      sgTotals->Cells[0][i] = itValues->first;
      sgTotals->Cells[1][i] = CurrToStrF(itValues->second,ffNumber,2);

      if(TGlobalSettings::Instance().ShowCurrencySymbol)
      {
        if(sgTotals->Cells[0][i] == "Total")
        {
          sgTotals->Cells[0][i] = itValues->first + " (" + CurrencyString + ")";
        }
      }
   }


   //btnSaveAndBill->Enabled = !TGlobalSettings::Instance().EnableWaiterStation;
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfirmOrder::FormResize(TObject *Sender)
{
this->Height = Screen->Height;
this->Width = Screen->Width;
GUIScale::ParentWidth(pnlButtons, 0.2);
GUIScale::ParentWidth(pnlReceipt, 0.4);
GUIScale::ParentWidth(pnlInfo, 0.4);



/*
	if (Tag != Screen->Width)
   {
      int Temp = Tag;
		Tag = Screen->Width;
   //	ScaleBy(Screen->Width, Temp);
   this->Height = Screen->Height;
   this->Width = Screen->Width - 100;
	}

	if(ReceiptDisplay->Count == 0)
	{
		pnlReceipt->Visible = false;
		ClientWidth		= pnlInfo->Left + pnlInfo->Width + this->BorderWidth + pnlButtons->Width + this->BorderWidth;
	}
	else
	{
		pnlReceipt->Visible = true;
		pnlReceipt->Left = pnlInfo->Left + pnlInfo->Width + this->BorderWidth;
		ClientWidth		= pnlReceipt->Left + pnlReceipt->Width + this->BorderWidth + pnlButtons->Width;
	}

	Left				= (Screen->Width - Width) / 2;
	Top				= (Screen->Height - Height) / 2;
        */
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfirmOrder::btnBillDownMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	tntReceipt->Perform(WM_VSCROLL, SB_LINEDOWN, 0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfirmOrder::btnBillUpMouseDown(TObject *Sender,
		TMouseButton Button, TShiftState Shift, int X, int Y)
{
	tntReceipt->Perform(WM_VSCROLL, SB_LINEUP, 0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfirmOrder::FormCreate(TObject *Sender)
{
	ReceiptDisplay = new TStringList;
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfirmOrder::FormDestroy(TObject *Sender)
{
	delete ReceiptDisplay;	
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfirmOrder::btnSaveAndBillClick(TObject *Sender)
{
	OrderContainer.Location["PrintPreLimReceipt"] = false;
	OrderContainer.Location["BillOff"] = true;
	ModalResult = mrOk;
	tntReceipt->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TfrmConfirmOrder::tbSavePrintClick(TObject *Sender)
{
	OrderContainer.Location["PrintPreLimReceipt"] = true;
	OrderContainer.Location["BillOff"] = false;	
	ModalResult = mrOk;
	tntReceipt->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfirmOrder::btnSaveClick(TObject *Sender)
{
	OrderContainer.Location["PrintPreLimReceipt"] = false;
	OrderContainer.Location["BillOff"] = false;
    DeleteFile(applicationDirectory + "\\" + "clippImage.jpg");
	ModalResult = mrOk;
	tntReceipt->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TfrmConfirmOrder::tbtnPartyNameMouseClick(TObject *Sender)
{
  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
   frmTouchKeyboard->MaxLength = 25;
   frmTouchKeyboard->AllowCarriageReturn = false;
   frmTouchKeyboard->KeyboardText = OrderContainer.Location["PartyName"];
   frmTouchKeyboard->Caption = "Enter Party Name";
   if (frmTouchKeyboard->ShowModal() == mrOk)
   {
      OrderContainer.Location["PartyName"] = frmTouchKeyboard->KeyboardText;
      OrderContainer.PartyNameChanged = true;
      tbtnPartyName->Caption = "Set Party Name\r" + frmTouchKeyboard->KeyboardText;
   	lbParty->Caption = OrderContainer.Location["PartyName"];
   	lbParty->Visible = (lbParty->Caption == "") ? false : true;
   }
   else
   {
      OrderContainer.PartyNameChanged = false;
      tbtnPartyName->Caption = "Set Party Name";
      OrderContainer.Location["PartyName"] = OrderContainer.Location["OriginalPartyName"];
   	lbParty->Caption = OrderContainer.Location["PartyName"];
   	lbParty->Visible = (lbParty->Caption == "") ? false : true;
   }
}

//---------------------------------------------------------------------------

void __fastcall TfrmConfirmOrder::tchbtnScrollBillUpAutoRepeat(TObject *Sender)
{
   tntReceipt->Perform(WM_VSCROLL, SB_LINEUP, 0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfirmOrder::tchbtnScrollBillDownAutoRepeat(TObject *Sender)
{
   tntReceipt->Perform(WM_VSCROLL, SB_LINEDOWN, 0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfirmOrder::sgTotalsDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State)
{
      sgTotals->Canvas->Font->Color = clWindowText;
      sgTotals->Canvas->Font->Name="Tahoma";
      sgTotals->Canvas->Font->Size=24;
      sgTotals->Canvas->Brush->Color = RGB(218,218,218);
      sgTotals->Canvas->FillRect(Rect);
      if(ACol == 1)
        {
           UnicodeString CellContent = sgTotals->Cells[ACol][ARow];
           Word SavedAlign = SetTextAlign(sgTotals->Canvas->Handle,TA_RIGHT);
           sgTotals->Canvas->TextRect(Rect, Rect.Right - 5, Rect.Top, CellContent);
           SetTextAlign(sgTotals->Canvas->Handle, SavedAlign);
        }
       else
        {
            UnicodeString text = sgTotals->Cells[ACol][ARow];
            sgTotals->Canvas->TextRect(Rect, Rect.Left + 5 , Rect.Top, text);
        }
}
//---------------------------------------------------------------------------

