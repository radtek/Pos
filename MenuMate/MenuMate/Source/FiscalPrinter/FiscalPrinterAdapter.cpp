//---------------------------------------------------------------------------


#pragma hdrstop

#include "FiscalPrinterAdapter.h"
//#include "main.h"
#include <Math.hpp>
#include "MMMessageBox.h"
#include "FiscalIntegration_OCX.h"
#include "DeviceRealTerminal.h"
#include "SaveLogs.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TFiscalPrinterAdapter::TFiscalPrinterAdapter()
{
    //billDetails = new TFiscalBillDetails();
}
//---------------------------------------------------------------------------
UnicodeString TFiscalPrinterAdapter::ConvertInToFiscalData(TPaymentTransaction paymentTransaction)
{
    UnicodeString responseMessage = "";
    try
    {
        billDetails.InvoiceNumber = paymentTransaction.InvoiceNumber;
        billDetails.Date = Now().FormatString("dd/mm/yyyy");
        billDetails.Billno = paymentTransaction.InvoiceNumber;
        billDetails.Cashier = TDeviceRealTerminal::Instance().User.Name;
        billDetails.TerminalName = TDeviceRealTerminal::Instance().ID.Name;
        billDetails.Time = Now().FormatString("hh:nn");
        billDetails.TabCredit = "0";
        billDetails.SaleType = "1";
        billDetails.PointPurchased = paymentTransaction.Membership.Member.Points.getCurrentPointsPurchased();
        billDetails.OpenCashDrawer = false;
        if(paymentTransaction.Membership.Member.Points.getCurrentPointsRefunded() != 0)
        {
            billDetails.PointPurchased = paymentTransaction.Membership.Member.Points.getCurrentPointsRefunded();
            billDetails.SaleType = "0";
        }
        billDetails.PrinterType  = TGlobalSettings::Instance().PrinterType;
        billDetails.PrinterLogicalName = TGlobalSettings::Instance().PrinterlogicalName;
        PrepareItemInfo(paymentTransaction);
        PrepartePaymnetInfo(paymentTransaction);
        responseMessage = PrintFiscalReceipt(billDetails);
    }
    catch(Exception &ex)
    {
        std::auto_ptr <TStringList> logList(new TStringList);
        logList->Add("Exception occured in function ConvertInToFiscalData() and message is :  " + ex.Message);
        TSaveLogs::RecordFiscalLogs(logList.get());

    }
    return responseMessage;
}
//------------------------------------------------------------------------------
void TFiscalPrinterAdapter::PrepareItemInfo(TPaymentTransaction paymentTransaction)
{
    std::vector<TFiscalItemDetails> itemList;
    std::vector<TFiscalDiscountDetails> discountList;
    for (int CurrentIndex = 0; CurrentIndex < paymentTransaction.Orders->Count; CurrentIndex++)
    {
        TItemComplete *order = (TItemComplete*)(paymentTransaction.Orders->Items[CurrentIndex]);

        TFiscalItemDetails itemDetails;
        itemDetails.TableNo = order->TabContainerName;
        itemDetails.GuestName = order->TabContainerName;
        itemDetails.ChitNumber = order->ChitNumber.ChitNumber;
        itemDetails.MemberName = paymentTransaction.Membership.Member.Name;
        itemDetails.PartyName = order->PartyName;
        itemDetails.ItemDescription = order->Item;
        if (UpperCase(order->Size) != "DEFAULT")
                itemDetails.SizeName = order->Size;
        else
            itemDetails.SizeName = "";
        itemDetails.Quantity = Double(order->GetQty()); 
        itemDetails.ItemCategory = order->Categories->FinancialCategory;
        double priceTotal = 0;
        double ItemPrice = 0;
        ItemPrice = (double)RoundToNearest(order->PriceEach(), 0.01, TGlobalSettings::Instance().MidPointRoundsDown);
        priceTotal =  (double)RoundToNearest((order->PriceEach()*order->GetQty()), 0.01, TGlobalSettings::Instance().MidPointRoundsDown );
		itemDetails.PricePerUnit = ItemPrice;
        itemDetails.PriceTotal = priceTotal;
        itemDetails.VATPercentage;
        std::vector<BillCalculator::TTaxResult> taxInfomation = order->BillCalcResult.Tax;
        double taxPercentage = 0;
        for (std::vector<BillCalculator::TTaxResult>::iterator itTaxes = taxInfomation.begin(); itTaxes != taxInfomation.end(); itTaxes++)
	    {
            if(!itTaxes->TaxType)
                taxPercentage += (double)itTaxes->Percentage;
        }
        itemDetails.VATPercentage = taxPercentage;
        itemList.push_back(itemDetails);
        PrepareDiscountDetails(discountList, order);
        for (int i = 0; i < order->SubOrders->Count; i++)
		{
		    TItemMinorComplete *currentSubOrder = (TItemMinorComplete*)order->SubOrders->Items[i];
            TFiscalItemDetails itemDetails;
            itemDetails.TableNo = order->TabContainerName;
            itemDetails.GuestName = order->TabContainerName;
            itemDetails.ChitNumber = order->ChitNumber.ChitNumber;
            itemDetails.MemberName = paymentTransaction.Membership.Member.Name;
            itemDetails.PartyName = order->PartyName;
            itemDetails.ItemDescription = currentSubOrder->Item;
            if (UpperCase(currentSubOrder->Size) != "DEFAULT")
                itemDetails.SizeName = currentSubOrder->Size;
            else
                itemDetails.SizeName = "";
            itemDetails.Quantity = currentSubOrder->GetQty();
            itemDetails.ItemCategory = currentSubOrder->Categories->FinancialCategory;
            priceTotal = 0;
            ItemPrice = 0;
            ItemPrice = (double)RoundToNearest(currentSubOrder->TotalPriceSides()/currentSubOrder->GetQty(), 0.01, TGlobalSettings::Instance().MidPointRoundsDown);
            priceTotal =  (double)(currentSubOrder->TotalPriceSides());
            itemDetails.PricePerUnit = ItemPrice;
            itemDetails.PriceTotal = priceTotal;
            double taxPercentage = 0;
            std::vector<BillCalculator::TTaxResult> taxInfomation = currentSubOrder->BillCalcResult.Tax;
            for (std::vector<BillCalculator::TTaxResult>::iterator itTaxes = taxInfomation.begin(); itTaxes != taxInfomation.end(); itTaxes++)
            {
                if(!itTaxes->TaxType)
                    taxPercentage = taxPercentage + itTaxes->Percentage;
            }
            itemDetails.VATPercentage = taxPercentage;
            itemList.push_back(itemDetails);
            PrepareDiscountDetails(discountList, currentSubOrder);
        }
    }
    billDetails.ItemList = itemList;
    billDetails.DiscountList = discountList;
}
//-------------------------------------------------------------
void TFiscalPrinterAdapter::PrepartePaymnetInfo(TPaymentTransaction paymentTransaction)
{
    std::vector<TFiscalPaymentDetails> PaymentList;
    double tabCredit = 0, totalPaymentCollected = 0;
    bool isCashPaymentExecuted = false;

    for (int i = 0; i < paymentTransaction.PaymentsCount(); i++)
    {
        TPayment *SubPayment = paymentTransaction.PaymentGet(i);
        double tipAmount = 0, surcharge = 0, change = 0, subTotal = 0;
        bool AddedToList = false, IsTipAppliedFromPOS = false;

        if(SubPayment->GetPaymentAttribute(ePayTypeCredit) && (double)SubPayment->GetPayTendered() != 0.0 && !paymentTransaction.Orders->Count)
        {
            tabCredit = (double)(SubPayment->GetPayTendered());
            if(tabCredit < 0)
               continue;
            else
                billDetails.SaleType = "0";
        }

        if(SubPayment->GetPaymentAttribute(ePayTypeCustomSurcharge) && SubPayment->GetAdjustment() != 0)
        {
            tipAmount += (double)SubPayment->GetAdjustment();
            AddedToList = true;
            IsTipAppliedFromPOS = true;
        }
        if(SubPayment->GetPaymentAttribute(ePayTypeSurcharge) && SubPayment->GetAdjustment() != 0)
        {
            AddedToList = true;
            surcharge += (double)SubPayment->GetAdjustment();
        }
        if ((SubPayment->GetPay() != 0) || (SubPayment->GetCashOut() != 0.0))
        {
            AddedToList = true;
            tipAmount += (double)SubPayment->TipAmount;
        }

        if(AddedToList)
        {
            TFiscalPaymentDetails paymentDetails;
            double subTotal = 0;
            subTotal = (double)RoundToNearest((SubPayment->GetPayTendered()- SubPayment->GetChange() - SubPayment->GetCashOut()), 0.01, TGlobalSettings::Instance().MidPointRoundsDown);

            if(SubPayment->GetPaymentAttribute(ePayTypeCash))
            {
                TPayment* newPaymentType = paymentTransaction.PaymentFindByProperty(ePayTypeElectronicTransaction);
                if(!isCashPaymentExecuted && newPaymentType != NULL)
                {
                    subTotal = (double)RoundToNearest((SubPayment->GetPayTendered() - SubPayment->GetChange() + newPaymentType->GetCashOut()), 0.01, TGlobalSettings::Instance().MidPointRoundsDown);
                    isCashPaymentExecuted = true;
                }
                else
                {
                    subTotal = (double)RoundToNearest((SubPayment->GetPayTendered() - SubPayment->GetChange()), 0.01, TGlobalSettings::Instance().MidPointRoundsDown);
                }
            }
            else
            {
                subTotal = (double)RoundToNearest((SubPayment->GetPayTendered()- SubPayment->GetChange()), 0.01, TGlobalSettings::Instance().MidPointRoundsDown);
            }
            //MessageBox(subTotal,"subTotal",MB_OK);
            totalPaymentCollected += subTotal;
            paymentDetails.Amount = subTotal;
            paymentDetails.Description = SubPayment->Name;
            paymentDetails.Billno = paymentTransaction.InvoiceNumber;
            paymentDetails.TipAmount = tipAmount;
            paymentDetails.PaymentSurcharge = surcharge;
           // paymentDetails.Change = SubPayment->GetChange() - SubPayment->GetCashOut();
            if(IsTipAppliedFromPOS)
                paymentDetails.TipAppliedFromPOS = "1";
            else
                paymentDetails.TipAppliedFromPOS = "0";

            PaymentList.push_back(paymentDetails);
        }
    }
	if(paymentTransaction.TransOpenCashDraw())
		billDetails.OpenCashDrawer = true;

    if(totalPaymentCollected < 0)
        billDetails.SaleType = "0";

    billDetails.TabCredit = tabCredit;
    billDetails.PaymentList = PaymentList;
}
//------------------------------------------------------------------------------
void TFiscalPrinterAdapter::PrepareDiscountDetails(std::vector<TFiscalDiscountDetails> &discountList,TItemMinorComplete *order)
{
    for (std::vector <TDiscount> ::const_iterator ptrDiscounts = order->Discounts.begin(); ptrDiscounts != order->Discounts.end();
	std::advance(ptrDiscounts, 1))
	{
        if(order->DiscountValue_BillCalc(ptrDiscounts) == 0)
            continue;
        bool isDiscountAlreadyExists = false;
        double discountAmount = 0;
        discountAmount = (double)RoundToNearest(order->DiscountValue_BillCalc(ptrDiscounts),0.01,TGlobalSettings::Instance().MidPointRoundsDown);
        for (std::vector<TFiscalDiscountDetails> ::iterator ptrFiscalDiscounts = discountList.begin(); ptrFiscalDiscounts != discountList.end();
        std::advance(ptrFiscalDiscounts, 1))
        {
            if(ptrFiscalDiscounts->DiscountKey == ptrDiscounts->DiscountKey)
            {
                discountAmount = (double)(StrToCurr(ptrFiscalDiscounts->Amount) + discountAmount);
                ptrFiscalDiscounts->Amount = discountAmount;
                isDiscountAlreadyExists = true;
                break;
            }
        }

        if(!isDiscountAlreadyExists)
        {
            TFiscalDiscountDetails discountDetails;
            discountDetails.DiscountKey = ptrDiscounts->DiscountKey;
            discountDetails.Type = ptrDiscounts->Type;
            discountDetails.Amount = discountAmount;
            discountDetails.Description = ptrDiscounts->Name;
            discountDetails.DiscountGroup = ptrDiscounts->DiscountGroupList.size() > 0 ? ptrDiscounts->DiscountGroupList[0].Name : UnicodeString::UnicodeString("");
            discountDetails.DiscountMode = ptrDiscounts->Mode;
            discountList.push_back(discountDetails);
        }
    }
}
//-----------------------------------------------------------------------------------------------
UnicodeString TFiscalPrinterAdapter::FiscalZReportSettlement()
{
    UnicodeString response = "";
    TFiscalLibraryClass *fpclass = new TFiscalLibraryClass(frmMain);
    try
    {
        fpclass->PrinterType = WideString(TGlobalSettings::Instance().PrinterType).c_bstr();
        fpclass->PrinterLogicalName = WideString(TGlobalSettings::Instance().PrinterlogicalName).c_bstr();
        BSTR responseMessage = fpclass->PrintZReport();
        response = UnicodeString(responseMessage).t_str();
    }
    catch(Exception & E)
    {
       response = E.Message;
	}
    delete fpclass;
    fpclass = NULL;
    return response;
}
//----------------------------------------------------------------------------------------------------
UnicodeString TFiscalPrinterAdapter::PrintFiscalReceipt(TFiscalBillDetails receiptData)
{
    UnicodeString response = "";
    TFiscalLibraryClass *fpclass = new TFiscalLibraryClass(NULL);
    try
    {
        fpclass->Billno = WideString(receiptData.Billno).c_bstr();
        fpclass->Cashier = WideString(receiptData.Cashier).c_bstr();
        fpclass->Cashno = WideString(receiptData.Cashno).c_bstr();
        fpclass->Date = WideString(receiptData.Date).c_bstr();
        fpclass->Source = WideString(receiptData.Source).c_bstr();
        fpclass->Time = WideString(receiptData.Time).c_bstr();
        fpclass->InvoiceNumber = WideString(receiptData.InvoiceNumber).c_bstr();
        fpclass->TerminalName = WideString(receiptData.TerminalName).c_bstr();
        fpclass->PointPurchased = WideString(receiptData.PointPurchased).c_bstr();
        fpclass->PrinterType = WideString(receiptData.PrinterType).c_bstr();
        fpclass->PrinterLogicalName = WideString(receiptData.PrinterLogicalName).c_bstr();
        fpclass->TabCredit =  WideString(receiptData.TabCredit).c_bstr();
        fpclass->Saletype =  WideString(receiptData.SaleType).c_bstr();
        fpclass->OpenCD =  receiptData.OpenCashDrawer;

        for(std::vector<TFiscalItemDetails>::iterator i = receiptData.ItemList.begin(); i != receiptData.ItemList.end() ; ++i)
        {
            fpclass->InitializeItemProperties();
            fpclass->LoadReceiptItemInfo(0, WideString(i->ChitNumber).c_bstr());
            fpclass->LoadReceiptItemInfo(1, WideString(i->GuestName).c_bstr());
            fpclass->LoadReceiptItemInfo(2, WideString(i->ItemCategory).c_bstr());
            fpclass->LoadReceiptItemInfo(3, WideString(i->ItemDescription).c_bstr());
            fpclass->LoadReceiptItemInfo(4, WideString(i->MemberName).c_bstr());
            fpclass->LoadReceiptItemInfo(5, WideString(i->PartyName).c_bstr());
            fpclass->LoadReceiptItemInfo(6, WideString(i->PricePerUnit).c_bstr());
            fpclass->LoadReceiptItemInfo(7, WideString(i->PriceTotal).c_bstr());
            fpclass->LoadReceiptItemInfo(8, WideString(i->Quantity).c_bstr());
            fpclass->LoadReceiptItemInfo(9, WideString(i->SizeName).c_bstr());
            fpclass->LoadReceiptItemInfo(10, WideString(i->TableNo).c_bstr());
            fpclass->LoadReceiptItemInfo(11, WideString(i->VATPercentage).c_bstr());
            fpclass->AddItemToList();
        }

        for(std::vector<TFiscalPaymentDetails>::iterator i = receiptData.PaymentList.begin(); i != receiptData.PaymentList.end() ; ++i)
        {
            fpclass->InitializePaymentProperties();
            fpclass->LoadReceiptPaymentInfo(0, WideString(i->Type).c_bstr());
            fpclass->LoadReceiptPaymentInfo(1, WideString(i->Amount).c_bstr());
            fpclass->LoadReceiptPaymentInfo(2, WideString(i->Billno).c_bstr());
            fpclass->LoadReceiptPaymentInfo(3, WideString(i->Cashno).c_bstr());
            fpclass->LoadReceiptPaymentInfo(4, WideString(i->Cashier).c_bstr());
            fpclass->LoadReceiptPaymentInfo(5, WideString(i->Source).c_bstr());
            fpclass->LoadReceiptPaymentInfo(6, WideString(i->Description).c_bstr());
            fpclass->LoadReceiptPaymentInfo(7, WideString(i->TipAmount).c_bstr());
            fpclass->LoadReceiptPaymentInfo(8, WideString(i->PaymentSurcharge).c_bstr());
            fpclass->LoadReceiptPaymentInfo(9, WideString(i->Change).c_bstr());
            fpclass->LoadReceiptPaymentInfo(10, WideString(i->TipAppliedFromPOS).c_bstr());
            fpclass->AddPaymentInfoToList();
        }

        for(std::vector<TFiscalDiscountDetails>::iterator i = receiptData.DiscountList.begin(); i != receiptData.DiscountList.end() ; ++i)
        {
            fpclass->InitializeDiscountProperties();
            fpclass->LoadReceiptDiscountInfo(0, WideString(i->Amount).c_bstr());
            fpclass->LoadReceiptDiscountInfo(1, WideString(i->Description).c_bstr());
            fpclass->LoadReceiptDiscountInfo(2, WideString(i->DiscountGroup).c_bstr());
            fpclass->LoadReceiptDiscountInfo(3, WideString(i->DiscountMode).c_bstr());
            fpclass->LoadReceiptDiscountInfo(4, WideString(i->Type).c_bstr());
            fpclass->AddDiscountInfoToList();
        }
        TStringList* logList = new TStringList();
        logList->Add("Inside PrintFiscalReceipt() function.");
        std::auto_ptr<TSaveLogs> saveLogs(new TSaveLogs());
        AnsiString path = ExtractFilePath(Application->ExeName) + "/Fiscal POS Logs";
        AnsiString fileName = Now().CurrentDate().FormatString("DDMMYYYY")+ ".txt";
        TSaveLogs::RecordFiscalLogs(logList);

        BSTR responseMessage = fpclass->PrintReceipt();

        logList->Clear();
        logList->Add("After sending command to fiscal printer.");
        if(responseMessage == NULL || responseMessage == "")
        {
            if(responseMessage == NULL)
                logList->Add("Response received from fiscal printer is NULL  ");
            else
                logList->Add("Response received from fiscal printer is blank ");
            TSaveLogs::RecordFiscalLogs(logList);
            response = "OK";
        }
        else
        {
            response = UnicodeString(responseMessage).t_str();
            logList->Add("Response received from fiscal printer is not null and response is :  " + response);
            TSaveLogs::RecordFiscalLogs(logList);
        }
    }
    catch(Exception & E)
    {
        response = E.Message;
        std::auto_ptr <TStringList> logList(new TStringList);
        logList->Add("Exception occured in function PrintFiscalReceipt() and message is :  " + response);
        TSaveLogs::RecordFiscalLogs(logList.get());
	}
    delete fpclass;
    fpclass = NULL;
    return response;
}
//------------------------------------------------------------------------------------------
UnicodeString TFiscalPrinterAdapter::GetFiscalPrinterStatus()
{
    UnicodeString printerStatus = "";
    try
    {
        TFiscalLibraryClass *fpclass = new TFiscalLibraryClass(frmMain);
        fpclass->PrinterType = WideString(TGlobalSettings::Instance().PrinterType).c_bstr();
        fpclass->PrinterLogicalName = WideString(TGlobalSettings::Instance().PrinterlogicalName).c_bstr();
        printerStatus = UnicodeString(fpclass->CheckPrinterAvailable()).t_str();
    }
    catch(Exception & E)
    {
        printerStatus = E.Message;
    }
    return printerStatus;
}
//-----------------------------------------------------------------------------------------------
UnicodeString TFiscalPrinterAdapter::OpenCashDrawerForFiscalPrinter()
{
    UnicodeString openDrawerMessage = "";
    try
    {
        TFiscalLibraryClass *fpclass = new TFiscalLibraryClass(frmMain);
        BSTR responseMessage = fpclass->OpenCashDrawer();
        openDrawerMessage = UnicodeString(responseMessage).t_str();
    }
    catch(Exception & E)
    {
        openDrawerMessage = E.Message;
    }
    return openDrawerMessage;
}

