// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ProcessWebOrder.h"

#include "DBWebUtil.h"
#include "WebProcUtil.h"

#include "DeviceRealTerminal.h"
#include "DBOrder.h"
#include "DBTab.h"
#include "DBTables.h"
#include "Comms.h"
#include "MMMessageBox.h"


// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TfrmProcessWebOrder::TfrmProcessWebOrder(TComponent* Owner) : TForm(Owner)
{
    frmProcessing.reset(TfrmProcessing::Create<TfrmProcessing>(this));
}
// ---------------------------------------------------------------------------

void __fastcall TfrmProcessWebOrder::FormResize(TObject *Sender)
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

    tchbtnEditOrders->Visible = false;
    TouchButton1->Visible = false;
}
// ---------------------------------------------------------------------------

void __fastcall TfrmProcessWebOrder::FormShow(TObject *Sender)
{
	FormResize(this);
}
// ---------------------------------------------------------------------------

void TfrmProcessWebOrder::Execute()
{
    if(TGlobalSettings::Instance().AutoAcceptWebOrders)
    {
        autoAcceptAllWebOrders();
        return;
    }
    else
    {
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction.StartTransaction();

        TDBWebUtil::LoadWebOrders(DBTransaction, WebOrderContainer);
        WebOrderContainer.first();

        DBTransaction.Commit();
    }
	UpdateDisplay();
	ShowModal();
}

void __fastcall TfrmProcessWebOrder::WebOrder(TMessage& Message)
{
	UnicodeString OrderID = WebOrderContainer.Current.GUID;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	TDBWebUtil::LoadWebOrders(DBTransaction, WebOrderContainer);
	WebOrderContainer.find(OrderID);
	DBTransaction.Commit();
	UpdateDisplay();
}

void __fastcall TfrmProcessWebOrder::UpdateDisplay()
{

    if(!TGlobalSettings::Instance().AutoAcceptWebOrders)
    {
       if(WebOrderContainer.Current.WebKey > 0)
       {
          TDBWebUtil::InitializeChit(WebOrderContainer.Current.WebKey, WebOrderChitNumber); // initialize chit here..
       }
    }
	UpdateButtons();
	if (WebOrderContainer.empty())
	{
		memReceipt->Lines->Clear();
		memReceipt->Lines->Add("There are no more Web Orders pending.");
	}
	else
	{
		ShowReceipt();
	}
}

void TfrmProcessWebOrder::ShowReceipt()
{
	try
	{
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
		DBTransaction.StartTransaction();

		std::auto_ptr<TReqPrintJob>TempReceipt(new TReqPrintJob(&TDeviceRealTerminal::Instance()));
		TPaymentTransaction ReceiptTransaction(DBTransaction);
		TempReceipt->Transaction = &ReceiptTransaction;

		std::set<__int64>ItemsTabs;
		ItemsTabs.insert(WebOrderContainer.Current.TabKey);

        TempReceipt->TimeStamp = WebOrderContainer.Current.DateOrdered;

        TempReceipt->Transaction->ChitNumber.DeliveryTime = WebOrderContainer.Current.DateExpected;

        TDBOrder::GetOrdersIncludingSidesFromTabKeys(DBTransaction, ReceiptTransaction.Orders, ItemsTabs);

		ReceiptTransaction.Money.CreditAvailable = TDBOrder::LoadCreditFromOrders(DBTransaction, ReceiptTransaction.Orders);
		ReceiptTransaction.Money.Recalc(ReceiptTransaction);

		ReceiptTransaction.TabCredit.clear();
		for (std::set<__int64>::iterator itTabs = ItemsTabs.begin(); itTabs != ItemsTabs.end(); advance(itTabs, 1))
		{
			Currency TabCurrentCredit = TDBTab::GetTabCredit(ReceiptTransaction.DBTransaction, *itTabs);
			if (TabCurrentCredit != 0)
			{
				ReceiptTransaction.Money.CreditAvailable += TabCurrentCredit;
				TTabCredit Credit = ReceiptTransaction.TabCredit[*itTabs];
				Credit.CurrentCredit = TabCurrentCredit;
				ReceiptTransaction.TabCredit[*itTabs] = Credit;
			}
		}

		for (std::set<__int64>::iterator pTabKey = ItemsTabs.begin(); pTabKey != ItemsTabs.end(); advance(pTabKey, 1))
		{
			std::auto_ptr<TStringList>WebDetials(new TStringList);
            int WebKey = TDBWebUtil::GetWebOrderKeyByTabKey(DBTransaction, *pTabKey);
			TDBWebUtil::getWebOrderDetials(DBTransaction, WebKey, *WebDetials.get());
			TempReceipt->ExtraInfo->AddStrings(WebDetials.get());

			std::auto_ptr<TStringList>WebDeliveryDetials(new TStringList);
            std::auto_ptr<TStringList>WebComments(new TStringList);
            std::auto_ptr<TStringList>WebPaymentDetials(new TStringList);
            TDBWebUtil::getWebOrderData(DBTransaction, WebDeliveryDetials.get(), WebPaymentDetials.get(), WebComments.get(), WebKey);

			TempReceipt->PaymentInfo->AddStrings(WebPaymentDetials.get());
			TempReceipt->OrderComments->AddStrings(WebComments.get());

            TMMContactInfo webMemberInfo;
            eMemberSource MemberSource;

            if(TGlobalSettings::Instance().CaptureCustomerName)
            {
               webMemberInfo.Clear();
               if(!WebOrderChitNumber.Valid())
               {
                  WebDeliveryDetials->Clear();
               }
            }
            if(WebOrderChitNumber.Valid())
            {
               if(WebOrderChitNumber.IsCaptureCustomerDetails)
               {
                   webMemberInfo = TDBWebUtil::autoAddWebMembers(DBTransaction, WebDetials.get(), WebDeliveryDetials.get(), WebKey);
                   if(MemberInfo.ContactKey > 0)
                   {
                      webMemberInfo = TDBWebUtil::checkAppliedChit(WebOrderChitNumber, MemberInfo, WebDeliveryDetials.get());
                   }
                   else
                   {
                       webMemberInfo = TDBWebUtil::checkAppliedChit(WebOrderChitNumber, webMemberInfo, WebDeliveryDetials.get());
                   }
               }
               else
               {
                   webMemberInfo.Clear();
                   WebDeliveryDetials->Clear();
               }
               if(WebOrderChitNumber.PromptForPickUpDeliveryTime)
               {
                  if((double)WebOrderChitNumber.DeliveryTime > 0)
                  {
                     TempReceipt->Transaction->ChitNumber.DeliveryTime = WebOrderChitNumber.DeliveryTime;//.//FormatString("DD/MM/YYYY hh:nn am/pm");
                  }
               }
            }
            ReceiptTransaction.Membership.Assign(webMemberInfo, MemberSource);
            TempReceipt->DeliveryInfo->AddStrings(WebDeliveryDetials.get());
		}
        ReceiptTransaction.IgnoreLoyaltyKey = false;
		ReceiptTransaction.Recalc();

		TStringList *TabHistory = new TStringList;
        bool isTable = false;
        int tabKey = 0;
		for (std::set<__int64>::iterator itTabs = ItemsTabs.begin(); itTabs != ItemsTabs.end(); advance(itTabs, 1))
		{
            tabKey = *itTabs;
            isTable = false;
            TMMTabType TabType = TDBTab::GetTabType(DBTransaction, *itTabs);
            if (TabType == TabTableSeat)
               {
                  TTableSeat TableSeat;
                  if (TDBTables::GetTableSeat(DBTransaction, *itTabs, &TableSeat))
                  {
                     tabKey = TableSeat.TableNo;
                     isTable = true;
                  }
               }
		    TDBTab::GetPartialPaymentList(DBTransaction, tabKey, TabHistory,isTable);
            for(int x = 0 ; x < TabHistory->Count; x++)
            {
               if(TempReceipt->TabHistory->IndexOf(TabHistory->Strings[x]) == -1)
               {
                 TempReceipt->TabHistory->Add(TabHistory->Strings[x]);
               }
            }
		}
		delete TabHistory;

		TempReceipt->SenderType = devPC;
		TempReceipt->Waiter = TDeviceRealTerminal::Instance().User.Name;

		TempReceipt->PaymentType = ptPreliminary;

		TPrinterPhysical DefaultScreenPrinter;
		DefaultScreenPrinter.NormalCharPerLine = 40;
		DefaultScreenPrinter.BoldCharPerLine = 40;
		DefaultScreenPrinter.PhysicalPrinterKey = TComms::Instance().ReceiptPrinter.PhysicalPrinterKey;
		Receipt->GetPrintouts(DBTransaction, TempReceipt.get(), DefaultScreenPrinter, eDispBCOff);
		memReceipt->Lines->Clear();
		TempReceipt->Printouts->PrintToStrings(memReceipt->Lines);

		while (ReceiptTransaction.Orders->Count != 0)
		{
			delete(TItemComplete*)ReceiptTransaction.Orders->Items[0];
			ReceiptTransaction.Orders->Delete(0);
		}

		DBTransaction.Commit();
        TempReceipt->Transaction->ChitNumber.DeliveryTime = 0;
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		memReceipt->Lines->Clear();
		memReceipt->Lines->Add(E.Message);
		MessageBox("Unable to display receipt.\r" "Please report the following message to your service provider :\r\r" + E.Message, "Error", MB_OK + MB_ICONERROR);
	}

}

void __fastcall TfrmProcessWebOrder::btnCloseMouseClick(TObject *Sender)
{
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmProcessWebOrder::btnPrevMouseClick(TObject *Sender)
{
	WebOrderContainer.prev();
	UpdateDisplay();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmProcessWebOrder::btnNextMouseClick(TObject *Sender)
{
	WebOrderContainer.next();
	UpdateDisplay();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmProcessWebOrder::UpdateButtons()
{
	if (WebOrderContainer.hasprev())
	{
		btnPrev->Enabled = true;
		btnPrev->Color = clNavy;
	}
	else
	{
		btnPrev->Enabled = false;
		btnPrev->Color = clMaroon;

	}

	if (WebOrderContainer.hasnext())
	{
		btnNext->Enabled = true;
		btnNext->Color = clNavy;
	}
	else
	{
		btnNext->Enabled = false;
		btnNext->Color = clMaroon;
	}

}

void __fastcall TfrmProcessWebOrder::btnBillUpMouseClick(TObject *Sender)
{
	memReceipt->Perform(WM_VSCROLL, SB_LINEUP, 0);
}

// ---------------------------------------------------------------------------
void __fastcall TfrmProcessWebOrder::btnBillDownMouseClick(TObject *Sender)
{
	memReceipt->Perform(WM_VSCROLL, SB_LINEDOWN, 0);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmProcessWebOrder::tbtnAcceptOrderMouseClick(TObject *Sender)
{
    acceptWebOrder();
}

void TfrmProcessWebOrder::acceptWebOrder()
{
    frmProcessing->Message = "Posting Orders";
    frmProcessing->Show();

    startAcceptWebOrdersThread(false);
}

void TfrmProcessWebOrder::autoAcceptAllWebOrders()
{
    startAcceptWebOrdersThread(true);
}

void __fastcall TfrmProcessWebOrder::autoAcceptWebOrdersTheadTerminate( TObject* sender )
{
    autoAcceptingWebOrders = false;
    frmProcessing->Close();
    UpdateDisplay();
}

void TfrmProcessWebOrder::startAcceptWebOrdersThread(bool acceptAll)
{
    if(!autoAcceptingWebOrders)
    {
        if(acceptAll)
        {
            Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
            DBTransaction.StartTransaction();
            TDBWebUtil::LoadWebOrders(DBTransaction, WebOrderContainer);
            WebOrderContainer.first();
            DBTransaction.Commit();
        }
       func_ptr ptr = &acceptWebOrder;
       TAcceptWebOrdersThread *autoAcceptThread = new TAcceptWebOrdersThread(&WebOrderContainer, acceptAll);
       autoAcceptThread->OnTerminate = autoAcceptWebOrdersTheadTerminate;
       autoAcceptThread->ChitNumber = WebOrderChitNumber;
       autoAcceptThread->Start();
       autoAcceptingWebOrders = true;
    }
}

TAcceptWebOrdersThread::TAcceptWebOrdersThread(TWebOrderContainer* webOrderContainer, bool acceptAllWebOrders)
    :TThread(true)
{
    acceptAll = acceptAllWebOrders;
    container = webOrderContainer;
    FreeOnTerminate = true;
}

void __fastcall TAcceptWebOrdersThread::Execute()
{
    if( acceptAll )
        acceptAllWebOrders();
    else
        acceptWebOrder();
}

void TAcceptWebOrdersThread::acceptWebOrder()
{
	try
	{
		// Load the Order.
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
		DBTransaction.StartTransaction();
        if(TGlobalSettings::Instance().AutoAcceptWebOrders)
        {
           if(container->Current.WebKey > 0)
           {
              TDBWebUtil::InitializeChit(container->Current.WebKey, ChitNumber);//, ChitNumberController);
           }
        }
        if(WebOrderTabKey != container->Current.TabKey)
        {
           WebOrderTabKey = container->Current.TabKey;
		   TWebProcUtil::ProcessWebOrder(Screen->ActiveForm, DBTransaction, container->Current, ChitNumber);
        }
		DBTransaction.Commit();
		DBTransaction.StartTransaction();
        if(TGlobalSettings::Instance().AutoAcceptWebOrders)
         {
            if(container->hasprev())
            {
               container->prev();
            }
            else
            {
                container->clear();
            }
        }
        else
        {
            container->clear();
            TDBWebUtil::LoadWebOrders(DBTransaction, *container);
            container->first();
        }
		DBTransaction.Commit();
	}
	catch(EAbort & E)
	{
		//MessageBox(E.Message, "Abort", MB_OK + MB_ICONERROR);
	}
	catch(Exception & E)
	{
		//MessageBox("Unable to process this order.\r" "Please report the following message to your service provider :\r\r" + E.Message, "Error", MB_OK + MB_ICONERROR);
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
}

void TAcceptWebOrdersThread::acceptAllWebOrders()
{
    while(!container->empty())
    {
        acceptWebOrder();
    }
}

void __fastcall TfrmProcessWebOrder::FormClose(TObject *Sender, TCloseAction &Action)
{
   WebOrderChitNumber.Clear();
}
//---------------------------------------------------------------------------

