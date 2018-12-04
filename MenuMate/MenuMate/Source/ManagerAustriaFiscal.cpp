//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerAustriaFiscal.h"
#include "DeviceRealTerminal.h"
#include "AustriaFiscalInterface.h"
#include "GeneratorManager.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TManagerAustriaFiscal::TManagerAustriaFiscal()
{
}
TManagerAustriaFiscal::~TManagerAustriaFiscal()
{
}
bool TManagerAustriaFiscal::GetEchoResponseFromConfig()
{
    bool retValue = false;
    UnsetPostingFlag();
    std::auto_ptr<TStringList> list(new TStringList);
    list->Add("Note- "+ (AnsiString)"Enabling Austria Fiscal as Selected from UI" +"\n");
    list->Add((AnsiString)"Time:-   " +Now().CurrentTime().FormatString("hh:nn:ss am/pm")+"\n");
    if(IsEchoSuccessful())
    {
        list->Add((AnsiString)"ECHO was successful" +"\n");
        if(!WasSignatureReceivedInLastSale())
        {
            list->Add((AnsiString)"Commisioning is required." +"\n");
            if(CommissionAustriaFiscal())
            {
                retValue = true;
                list->Add((AnsiString)"Commisioning is successful." +"\n");
            }
            else
            {
                retValue = false;
                list->Add((AnsiString)"Commisioning is unsuccessful." +"\n");
            }
        }
        else
        {
            retValue = true;
            list->Add((AnsiString)"Commisioning is not required." +"\n");
        }
    }
    LogFiscalAustriaEnabling(list,retValue);
    return retValue;
}
bool TManagerAustriaFiscal::GetEchoResponseFromMain()
{
    bool retValue = false;
    UnsetPostingFlag();
    std::auto_ptr<TStringList> list(new TStringList);
    list->Add("Note- "+ (AnsiString)"Enabling Austria Fiscal at start of Menumate" +"\n");
    list->Add((AnsiString)"Time:-   " +Now().CurrentTime().FormatString("hh:nn:ss am/pm")+"\n");
    // Call to Fiscal API using services
    retValue = IsEchoSuccessful();
    if(retValue)
        list->Add((AnsiString)"Echo was successful" +"\n");
    else
        list->Add((AnsiString)"Echo was unsuccessful" +"\n");
    LogFiscalAustriaEnabling(list,retValue);
    return retValue;
}
bool TManagerAustriaFiscal::IsZeroReceiptSuccessful()
{
    bool retValue = false;
    try
    {
        std::auto_ptr<TAustriaFiscalInterface> fiscalInterface(new TAustriaFiscalInterface());
        retValue = fiscalInterface->SendZeroReceipt(TGlobalSettings::Instance().AustriaFiscalUrl,
                                                        TGlobalSettings::Instance().AustriaFiscalCashBoxId);
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, ex.Message);
    }
    return retValue;
}
// Make Call to Fiscal API for commisioning
bool TManagerAustriaFiscal::CommissionAustriaFiscal()
{
    bool retValue = false;
    try
    {
        std::auto_ptr<TAustriaFiscalInterface> fiscalInterface(new TAustriaFiscalInterface());
        bool isSuccessful = fiscalInterface->CommissionAustriaFiscal(TGlobalSettings::Instance().AustriaFiscalUrl,
                                                        TGlobalSettings::Instance().AustriaFiscalCashBoxId);
        if(isSuccessful)
        {
            retValue = fiscalInterface->SendZeroReceipt(TGlobalSettings::Instance().AustriaFiscalUrl,
                                                        TGlobalSettings::Instance().AustriaFiscalCashBoxId);
        }
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, ex.Message);
    }
    return retValue;
}
bool TManagerAustriaFiscal::PostDataToAustriaFiscal()
{
    bool retValue = false;
    retValue = true;
    return retValue;
}
bool TManagerAustriaFiscal::ExportData(TPaymentTransaction &paymentTransaction)
{
    bool retValue = true;
   	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	DBTransaction.StartTransaction();
    try
    {
        WaitOrProceedWithPost();
        TReceiptRequestAustriaFiscal receiptAustria;
        receiptAustria = GetAustriaReceiptDetails(paymentTransaction);
        // Send to Interface and get details from API.
        std::auto_ptr<TAustriaFiscalInterface> austriaInterface(new TAustriaFiscalInterface());
        TReceiptResponseAustriaFiscal response = austriaInterface->PostDataToAustriaFiscal(receiptAustria);
        // Analyse details and store in DB
        if(response.UnableToConnectToMenumateService)
        {
            response.CashBoxID = receiptAustria.CashBoxID;
            StoreDataInDB(response,receiptAustria.ReceiptReference,DBTransaction);
            StoreInvoiceDetailsinDB(receiptAustria,DBTransaction);
        }
        else if(response.UnableToConnectToFiscalService)
        {
            response.CashBoxID = receiptAustria.CashBoxID;
            StoreDataInDB(response,receiptAustria.ReceiptReference,DBTransaction);
            StoreInvoiceDetailsinDB(receiptAustria,DBTransaction);
        }
        else
        {
            StoreDataInDB(response,receiptAustria.ReceiptReference,DBTransaction);
            StoreInvoiceDetailsinDB(receiptAustria,DBTransaction);
            TakeCorrectiveMeasures(DBTransaction);
        }
//        if(response.Signatures.size() == 0 /*|| response.State != 0x4154000000000000*/)
//        {
//
//        }
        DBTransaction.Commit();
    }
    catch(Exception &ex)
    {
       DBTransaction.Rollback();
       TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, ex.Message);
    }
    UnsetPostingFlag();
    return retValue;
}
//private
// Make Query to get status
bool TManagerAustriaFiscal::WasSignatureReceivedInLastSale()
{
    bool retValue = false;
   	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	DBTransaction.StartTransaction();
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->SQL->Text = "SELECT * FROM AUSTRIAFISCALRESPONSE ORDER BY RESPONSE_ID DESC";
        IBInternalQuery->ExecQuery();
        if(IBInternalQuery->RecordCount > 0)
        {
            if(IBInternalQuery->FieldByName("IS_SIGNED")->AsString == "T")
                retValue = true;
        }
        DBTransaction.Commit();
    }
    catch(Exception &Exc)
    {
        DBTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Exc.Message);
    }
    return retValue;
}
bool TManagerAustriaFiscal::IsEchoSuccessful()
{
    bool retValue = false;
    std::auto_ptr<TAustriaFiscalInterface> fiscalInterface(new TAustriaFiscalInterface());
    UnicodeString response = fiscalInterface->InitAustriaFiscal(TGlobalSettings::Instance().AustriaFiscalUrl,
                                                        TGlobalSettings::Instance().AustriaFiscalCashBoxId);
    if(response.Pos("Echo is Successful for details provided.") != 0)
        retValue = true;
    else
    {
        retValue = false;
        response += "\rAustria fiscal is not enabled.\r";
        if(response.Pos("Echo failed") != 0)
            response += "Please check the system is connected to network and verify the details are correct.";
        MessageBox(response,"Error",MB_OK+MB_ICONINFORMATION);
    }
    return retValue;
}
void TManagerAustriaFiscal::LogFiscalAustriaEnabling(std::auto_ptr<TStringList> list,bool isEnabled)
{
    try
    {
        AnsiString fileName = GetLogFileName();
        std::auto_ptr<TStringList> listLocal(new TStringList);
        if (FileExists(fileName) )
          listLocal->LoadFromFile(fileName);
        for(int i = 0; i < list->Count; i++)
        {
            AnsiString value = list->operator [](i);
            listLocal->Add(value);
        }
        if(isEnabled)
            listLocal->Add((AnsiString)"<<< Austria Fiscal is enabled. >>>"+"\n");
        else
            listLocal->Add((AnsiString)"<<< Austria Fiscal is not enabled. >>>"+"\n");
        listLocal->Add("=================================================================================");
        listLocal->SaveToFile(fileName );
    }
    catch(Exception &Exc)
    {
       TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
    }
}
AnsiString TManagerAustriaFiscal::GetLogFileName()
{
    AnsiString directoryName = ExtractFilePath(Application->ExeName) + "Menumate Services";
    if (!DirectoryExists(directoryName))
        CreateDir(directoryName);
    directoryName = directoryName + "\\logs";
    if (!DirectoryExists(directoryName))
        CreateDir(directoryName);
    directoryName = directoryName + "\\Austria Fiscal Post Logs";
    if (!DirectoryExists(directoryName))
        CreateDir(directoryName);
    AnsiString name = "AustriaFiscalPosts " + Now().CurrentDate().FormatString("DDMMMYYYY")+ ".txt";
    AnsiString fileName =  directoryName + "\\" + name;
    return fileName;
}
TReceiptRequestAustriaFiscal TManagerAustriaFiscal::GetAustriaReceiptDetails(TPaymentTransaction &paymentTransaction)
{
    TReceiptRequestAustriaFiscal receiptAustria;
    try
    {
        receiptAustria.CashBoxID = TGlobalSettings::Instance().AustriaFiscalCashBoxId;
        receiptAustria.TerminalID = TGlobalSettings::Instance().AustriaFiscalTerminalId;
        if(!paymentTransaction.CreditTransaction)
            receiptAustria.ReceiptCase = 0x4154000000000001;
        else
            receiptAustria.ReceiptCase = 0x4154000000040000;
        receiptAustria.ReceiptReference = GetInvoiceNumber(paymentTransaction);
        receiptAustria.ChargeItems = GetChargeItemsAustria(paymentTransaction);
        receiptAustria.PayItems = GetPayItemsAustria(paymentTransaction);
        receiptAustria.ReceiptMoment = Now();
    }
    catch(Exception &Exc)
    {
       TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
    }
    return receiptAustria;
}
UnicodeString TManagerAustriaFiscal::GetInvoiceNumber(TPaymentTransaction paymentTransaction)
{
    UnicodeString invoiceNumber = "";
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *IBInternalQueryGenerator= DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQueryGenerator->Close();
        bool IsCompDiscountApplied      = false;
        bool IsNCDiscountApplied        = false;
        bool IsNormalDiscountApplied    = false;
        for(int index = 0; index < paymentTransaction.Orders->Count; index++)
        {
            TItemComplete *itemcomplete = (TItemComplete*)paymentTransaction.Orders->Items[index];
            for(int discountIndex = 0; discountIndex < itemcomplete->Discounts.size(); discountIndex++)
            {
                if(itemcomplete->Discounts[discountIndex].IsComplimentaryDiscount())
                {
                    IsCompDiscountApplied = true;
                    break;
                }
                if(itemcomplete->Discounts[discountIndex].IsNonChargableDiscount())
                {
                    IsNCDiscountApplied = true;
                    break;
                }
            }
            if(IsCompDiscountApplied || IsNCDiscountApplied)
            {
                break;
            }
        }
        if(IsCompDiscountApplied)
        {
            IBInternalQueryGenerator->SQL->Text = "SELECT GEN_ID(GEN_INVOICENUMBERCOMP, 0) FROM RDB$DATABASE ";
            IBInternalQueryGenerator->ExecQuery();
            int number = IBInternalQueryGenerator->Fields[0]->AsInteger + 1;
            invoiceNumber = "Comp " + IntToStr(number);
        }
        else if(IsNCDiscountApplied)
        {
            IBInternalQueryGenerator->SQL->Text = "SELECT GEN_ID(GEN_INVOICENUMBERNC, 0) FROM RDB$DATABASE ";
            IBInternalQueryGenerator->ExecQuery();
            int number = IBInternalQueryGenerator->Fields[0]->AsInteger + 1;
            invoiceNumber = "NC "+ IntToStr(number);
        }
        else
        {
            IBInternalQueryGenerator->SQL->Text = "SELECT GEN_ID(GEN_INVOICENUMBER, 0) FROM RDB$DATABASE ";
            IBInternalQueryGenerator->ExecQuery();
            int number = IBInternalQueryGenerator->Fields[0]->AsInteger + 1;
            invoiceNumber = IntToStr(number);
        }
        DBTransaction.Commit();
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
        DBTransaction.Rollback();
    }
    return invoiceNumber;
}
std::vector<TChargeItemAustriaFiscal> TManagerAustriaFiscal::GetChargeItemsAustria(TPaymentTransaction paymentTransaction)
{
    std::vector<TChargeItemAustriaFiscal> chargeItems;
    chargeItems.clear();
    try
    {
        for(int index = 0; index < paymentTransaction.Orders->Count; index++)
        {
            bool addDiscountItem = false;
            TItemComplete* itemComplete = (TItemComplete*)paymentTransaction.Orders->Items[index];
            TChargeItemAustriaFiscal chargeItem;
            chargeItem = GetBillItemAustria(itemComplete,paymentTransaction,addDiscountItem);
            chargeItems.push_back(chargeItem);
            if((double)itemComplete->BillCalcResult.ServiceCharge.Value != 0)
            {
                TChargeItemAustriaFiscal chargeItemServiceCharge = GetBillItemServiceCharge(itemComplete,paymentTransaction.CreditTransaction);
                chargeItems.push_back(chargeItemServiceCharge);
            }
            if(addDiscountItem)
            {
                TChargeItemAustriaFiscal chargeItemDiscount;
                chargeItemDiscount = GetBillItemDiscountAustria(itemComplete,paymentTransaction);
                chargeItems.push_back(chargeItemDiscount);
            }
            for(int indexSub = 0; indexSub < itemComplete->SubOrders->Count; indexSub++)
            {
                bool addDiscountItemSub = false;
                TItemComplete *subItemComplete = ((TItemComplete*)itemComplete->SubOrders->Items[indexSub]);
                TChargeItemAustriaFiscal chargeItemSub;
                chargeItemSub = GetBillItemAustria(subItemComplete,paymentTransaction,addDiscountItemSub);
                chargeItems.push_back(chargeItemSub);
                if((double)subItemComplete->BillCalcResult.ServiceCharge.Value != 0)
                {
                    TChargeItemAustriaFiscal chargeItemSubServiceCharge = GetBillItemServiceCharge(subItemComplete,paymentTransaction.CreditTransaction);
                    chargeItems.push_back(chargeItemSubServiceCharge);
                }
                if(addDiscountItemSub)
                {
                    TChargeItemAustriaFiscal chargeItemSubDiscount;
                    chargeItemSubDiscount = GetBillItemDiscountAustria(subItemComplete,paymentTransaction);
                    chargeItems.push_back(chargeItemSubDiscount);
                }
            }
        }
        if(paymentTransaction.Membership.Member.Points.getCurrentPointsPurchased() != 0)
        {
            TChargeItemAustriaFiscal chargeItemPointsPurchased;

            if(!paymentTransaction.CreditTransaction)
                chargeItemPointsPurchased.ChargeItemCase = 0x4154000000000000;
            else
                chargeItemPointsPurchased.ChargeItemCase = 0x4154000000000006;
            chargeItemPointsPurchased.Description = "Points Purchased";
            chargeItemPointsPurchased.VATRate = 0;
            chargeItemPointsPurchased.Quantity = 1;
            chargeItemPointsPurchased.Amount = (double)paymentTransaction.Membership.Member.Points.getCurrentPointsPurchased();
            chargeItems.push_back(chargeItemPointsPurchased);
        }
        if(paymentTransaction.Money.TotalRounding != 0)
        {
            for(int index = 0; index < chargeItems.size(); index++)
            {
                chargeItems[index].Amount += (double)paymentTransaction.Money.TotalRounding;
                break;
            }
        }
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
    }
    return chargeItems;
}
std::vector<TPayItemAustriaFiscal> TManagerAustriaFiscal::GetPayItemsAustria(TPaymentTransaction paymentTransaction)
{
    std::vector<TPayItemAustriaFiscal> payItems;
    payItems.clear();
    for(int indexPayment = 0; indexPayment < paymentTransaction.PaymentsCount();indexPayment++)
    {
        TPayment* payment = paymentTransaction.PaymentGet(indexPayment);
        if(payment->GetPaymentAttribute(ePayTypeCustomSurcharge) && (double)payment->GetAdjustment() != 0)
        {
            TPayItemAustriaFiscal austriaCustomTips;
            austriaCustomTips.PayItemCase = 0x4154000000000000;
            austriaCustomTips.Amount = ((double)(payment->GetAdjustment()));
            if(!paymentTransaction.CreditTransaction)
                austriaCustomTips.Quantity = 1;
            else
                austriaCustomTips.Quantity = -1;
            austriaCustomTips.Description = "Tips";
            payItems.push_back(austriaCustomTips);
        }
        if(payment->GetPaymentAttribute(ePayTypeSurcharge) && (double)payment->GetAdjustment() != 0)
        {
            TPayItemAustriaFiscal austriaPaymentSurcharge;
            austriaPaymentSurcharge.PayItemCase = 0x4154000000000000;
            austriaPaymentSurcharge.Amount = ((double)(payment->GetAdjustment()));
            if(!paymentTransaction.CreditTransaction)
                austriaPaymentSurcharge.Quantity = 1;
            else
                austriaPaymentSurcharge.Quantity = -1;
            austriaPaymentSurcharge.Description = payment->Name + " Surcharge";
            payItems.push_back(austriaPaymentSurcharge);
        }
        if((double)payment->GetPayTendered() != 0)
        {
            TPayItemAustriaFiscal austriaPayment;
            austriaPayment.PayItemCase = 0x4154000000000000;
            austriaPayment.Amount = ((double)(payment->GetPayTendered() + payment->GetCashOut() - (payment->GetChange()) - (payment->GetAdjustment())));
            if(!paymentTransaction.CreditTransaction)
                austriaPayment.Quantity = 1;
            else
                austriaPayment.Quantity = -1;
            austriaPayment.Description = payment->Name;
            payItems.push_back(austriaPayment);
        }
        if(payment->GetCashOut() != 0)
        {
            TPayItemAustriaFiscal austriaPaymentCashOut;
            austriaPaymentCashOut.PayItemCase = 0x4154000000000000;
            austriaPaymentCashOut.Amount = ((double)payment->GetCashOut());
            if(!paymentTransaction.CreditTransaction)
                austriaPaymentCashOut.Quantity = 1;
            else
                austriaPaymentCashOut.Quantity = -1;
            austriaPaymentCashOut.Description = payment->Name + " Cash Out";
            payItems.push_back(austriaPaymentCashOut);
        }
        if(payment->TipAmount != 0)
        {
            TPayItemAustriaFiscal austriaPaymentTip;
            austriaPaymentTip.PayItemCase = 0x4154000000000012;
            austriaPaymentTip.Amount = ((double)payment->TipAmount);
            if(!paymentTransaction.CreditTransaction)
                austriaPaymentTip.Quantity = 1;
            else
                austriaPaymentTip.Quantity = -1;
            austriaPaymentTip.Description = payment->Name + "Tips";
            payItems.push_back(austriaPaymentTip);
        }
    }
    return payItems;
}
TChargeItemAustriaFiscal TManagerAustriaFiscal::GetBillItemAustria(TItemComplete* itemComplete, TPaymentTransaction paymentTransaction,bool &addDiscountItem)
{
    TChargeItemAustriaFiscal chargeItem;
    try
    {
        if(!paymentTransaction.CreditTransaction)
            chargeItem.ChargeItemCase = 0x4154000000000000;
        else
            chargeItem.ChargeItemCase = 0x4154000000000006;
        chargeItem.Description = itemComplete->Item + " (" + itemComplete->Size + ") ";
        chargeItem.VATRate = GetVatPercentage(itemComplete);
        chargeItem.Quantity = (double)itemComplete->GetQty();
        chargeItem.Amount = GetItemAmount(itemComplete);
        if(!TGlobalSettings::Instance().Instance().ReCalculateTaxPostDiscount && itemComplete->BillCalcResult.TotalDiscount != 0)
           addDiscountItem = true;
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
    }
    return chargeItem;
}
TChargeItemAustriaFiscal TManagerAustriaFiscal::GetBillItemDiscountAustria(TItemComplete* itemComplete, TPaymentTransaction paymentTransaction)
{
    TChargeItemAustriaFiscal chargeItem;
    try
    {
        chargeItem.ChargeItemCase = 0x4154000000000000;
        if(!paymentTransaction.CreditTransaction)
        {
            if(itemComplete->BillCalcResult.TotalDiscount < 0)
                chargeItem.Description = "Discount " + itemComplete->Item  + " (" + itemComplete->Size + ") ";
            else
                chargeItem.Description = "Surcharge " + itemComplete->Item  + " (" + itemComplete->Size + ") ";
        }
        else
        {
            if(itemComplete->BillCalcResult.TotalDiscount > 0)
                chargeItem.Description = "Discount " + itemComplete->Item  + " (" + itemComplete->Size + ") ";
            else
                chargeItem.Description = "Surcharge " + itemComplete->Item  + " (" + itemComplete->Size + ") ";
        }
        chargeItem.VATRate = 0;
        if(!paymentTransaction.CreditTransaction)
        {
            if(itemComplete->BillCalcResult.TotalDiscount < 0)
                chargeItem.Quantity = -1;
            else
                chargeItem.Quantity = 1;
        }
        else
        {
            if(itemComplete->BillCalcResult.TotalDiscount > 0)
                chargeItem.Quantity = 1;
            else
                chargeItem.Quantity = -1;
        }
        chargeItem.Amount = (double)itemComplete->BillCalcResult.TotalDiscount;
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
    }
    return chargeItem;
}
TChargeItemAustriaFiscal TManagerAustriaFiscal::GetBillItemServiceCharge(TItemComplete* itemComplete, bool isRefund)
{
    TChargeItemAustriaFiscal chargeItem;
    try
    {
        chargeItem.ChargeItemCase = 0x4154000000000000;
        chargeItem.Description = "Service Charge " + itemComplete->Item  + " (" + itemComplete->Size + ") ";
        chargeItem.VATRate = itemComplete->BillCalcResult.ServiceCharge.TaxPercentage;
        if(!isRefund)
            chargeItem.Quantity = 1;
        else
            chargeItem.Quantity = -1;
        chargeItem.Amount = itemComplete->BillCalcResult.ServiceCharge.Value;
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
    }
    return chargeItem;
}
double TManagerAustriaFiscal::GetVatPercentage(TItemComplete * itemComplete)
{
    double vatPercentage = 0;
    for(std::vector<BillCalculator::TTaxResult>::iterator tax = itemComplete->BillCalcResult.Tax.begin();
          tax != itemComplete->BillCalcResult.Tax.end() ; ++tax)
    {
        if(tax->Value != 0)
            vatPercentage += (double)tax->Percentage;
    }
    return vatPercentage;
}
double TManagerAustriaFiscal::GetItemAmount(TItemComplete * itemComplete)
{
    double amount = 0;
     if(TGlobalSettings::Instance().Instance().ReCalculateTaxPostDiscount)
     {
         amount = (double)itemComplete->BillCalcResult.FinalPrice -
                        (double)itemComplete->BillCalcResult.ServiceCharge.TaxValue -
                        (double)itemComplete->BillCalcResult.ServiceCharge.Value;
     }
     else
     {
         amount = (double)itemComplete->BillCalcResult.FinalPrice -
                        (double)itemComplete->BillCalcResult.ServiceCharge.Value -
                        (double)itemComplete->BillCalcResult.ServiceCharge.TaxValue -
                        (double)itemComplete->BillCalcResult.TotalDiscount;
     }
    return amount;
}
void TManagerAustriaFiscal::WaitOrProceedWithPost()
{
    bool isPosting = false;
    int global_profile_key;
    Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);
    std::auto_ptr<TStringList> waitLogs(new TStringList);
    try
    {
        Sleep(Random(21));
        Sleep(Random(30));
        Sleep(Random(50));
        TGlobalSettings  &gs = TGlobalSettings::Instance();
        TManagerVariable &mv = TManagerVariable::Instance();

        tr.StartTransaction();
        // This is used to retain the state of the checkbox if the POS is exited
        #pragma warn -pia
        if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
        global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
        #pragma warn .pia
        TManagerVariable::Instance().GetProfileBool(tr, global_profile_key, vmIsAustriaFiscalPostInProgress, isPosting);
        if(isPosting)
            waitLogs->Add("Entered queue at                          " + Now().FormatString("hh:mm:ss tt"));
        while(isPosting)
        {
            Sleep(400);
            TManagerVariable::Instance().GetProfileBool(tr, global_profile_key, vmIsAustriaFiscalPostInProgress, isPosting);
        }
        mv.SetProfileBool(tr,global_profile_key,
        vmIsAustriaFiscalPostInProgress,true);
        tr.Commit();
        //SetPostingFlag();
        if(waitLogs->Count > 0)
        {
            waitLogs->Add("Wait Over at                              " + Now().FormatString("hh:mm:ss tt"));
            waitLogs->Add("=================================================================================");
            LogWaitStatus(waitLogs);
        }
    }
    catch(Exception &Exc)
    {
        tr.Rollback();
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
    }
}
void TManagerAustriaFiscal::LogWaitStatus(std::auto_ptr<TStringList> waitLogs)
{
    try
    {
        AnsiString fileName = GetLogFileName();
        std::auto_ptr<TStringList> List(new TStringList);
        if (FileExists(fileName) )
          List->LoadFromFile(fileName);
        for(int index = 0; index < waitLogs->Count; index++)
        {
            AnsiString value = waitLogs->operator [](index);
            List->Add(value);
        }
        List->SaveToFile(fileName );
    }
    catch(Exception &Exc)
    {
       TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
    }
}
void TManagerAustriaFiscal::UnsetPostingFlag()
{
    Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);
    tr.StartTransaction();
    try
    {
        TGlobalSettings  &gs = TGlobalSettings::Instance();
        TManagerVariable &mv = TManagerVariable::Instance();

        int global_profile_key;

        // This is used to retain the state of the checkbox if the POS is exited
    #pragma warn -pia
        if (!(global_profile_key = mv.GetProfile(tr, eSystemProfiles, "Globals")))
        global_profile_key = mv.SetProfile(tr, eSystemProfiles, "Globals");
    #pragma warn .pia

        mv.SetProfileBool(tr,global_profile_key,
        vmIsAustriaFiscalPostInProgress,false);

        tr.Commit();
    }
    catch(Exception &Exc)
    {
        tr.Rollback();
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
    }
}
void TManagerAustriaFiscal::StoreDataInDB(TReceiptResponseAustriaFiscal response, UnicodeString invoiceNumber,Database::TDBTransaction &DBTransaction)
{
    try
    {
        int primaryKey = TGeneratorManager::GetNextGeneratorKey(DBTransaction,"GEN_AUSTRIAFISCALRESPONSE_ID");
        TIBSQL *IBInternalQuery= DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = " INSERT INTO AUSTRIAFISCALRESPONSE "
                 " ( RESPONSE_ID, MMINVOICENUMBER, CASHBOXID, QUEUEID, QUEUEITEMID, QUEUEROW, TERMINALID,"
                 " RECEIPTREFERENCE, CASHBOXIDENTIFICATION, RECEIPTIDENTIFICATION, RECEIPTMOMENT, STATE, STATEDATA,"
				 " IS_SIGNED ) VALUES"
                 " ( :RESPONSE_ID, :MMINVOICENUMBER, :CASHBOXID, :QUEUEID, :QUEUEITEMID, :QUEUEROW, :TERMINALID,"
                 " :RECEIPTREFERENCE, :CASHBOXIDENTIFICATION, :RECEIPTIDENTIFICATION, :RECEIPTMOMENT, :STATE, :STATEDATA,"
				 " :IS_SIGNED )";
        IBInternalQuery->ParamByName("RESPONSE_ID")->AsInteger = primaryKey;
        IBInternalQuery->ParamByName("MMINVOICENUMBER")->AsString = invoiceNumber;
        IBInternalQuery->ParamByName("CASHBOXID")->AsString = response.CashBoxID;
        IBInternalQuery->ParamByName("QUEUEID")->AsString = response.QueueID;
        IBInternalQuery->ParamByName("QUEUEITEMID")->AsString = response.QueueItemID;
        IBInternalQuery->ParamByName("QUEUEROW")->AsString = response.QueueRow;
        IBInternalQuery->ParamByName("TERMINALID")->AsString = response.TerminalID;
        IBInternalQuery->ParamByName("RECEIPTREFERENCE")->AsString = response.ReceiptReference;
        IBInternalQuery->ParamByName("CASHBOXIDENTIFICATION")->AsString = response.CashBoxIdentification;
        IBInternalQuery->ParamByName("RECEIPTIDENTIFICATION")->AsString = response.ReceiptIdentification;
        IBInternalQuery->ParamByName("RECEIPTMOMENT")->AsDateTime = response.ReceiptMoment;
        IBInternalQuery->ParamByName("STATE")->AsString = response.State;
        IBInternalQuery->ParamByName("STATEDATA")->AsString = response.StateData;
        bool isSigned = false;
        for(int indexSig = 0; indexSig < response.Signatures.size(); indexSig++)
        {
            if(response.Signatures[indexSig].Data != "")
            {
                isSigned = true;
                break;
            }
        }
        if(isSigned)
            IBInternalQuery->ParamByName("IS_SIGNED")->AsString = "T";
        else
            IBInternalQuery->ParamByName("IS_SIGNED")->AsString = "F";
        IBInternalQuery->ExecQuery();
        if(isSigned)
        {
            for(int i = 0; i < response.Signatures.size(); i++)
            {
                IBInternalQuery->Close();
                int primaryKeySignature = TGeneratorManager::GetNextGeneratorKey(DBTransaction,"GEN_AUSTRIAFISCALSIGNATURE_ID");
                IBInternalQuery->SQL->Text = " INSERT INTO AUSTRIAFISCALSIGNATURES "
                         " ( SIGNATURE_ID, RESPONSE_ID, SIGNATUREFORMAT, SIGNATURETYPE, CAPTION, DATA ) VALUES"
                         " ( :SIGNATURE_ID, :RESPONSE_ID, :SIGNATUREFORMAT, :SIGNATURETYPE, :CAPTION, :DATA )";
                IBInternalQuery->ParamByName("SIGNATURE_ID")->AsInteger = primaryKeySignature;
                IBInternalQuery->ParamByName("RESPONSE_ID")->AsInteger = primaryKey;
                IBInternalQuery->ParamByName("SIGNATUREFORMAT")->AsString = response.Signatures[i].SignatureFormat;
                IBInternalQuery->ParamByName("SIGNATURETYPE")->AsString = response.Signatures[i].SignatureType;
                IBInternalQuery->ParamByName("CAPTION")->AsString = response.Signatures[i].Caption;
                IBInternalQuery->ParamByName("DATA")->AsString = response.Signatures[i].Data;
                IBInternalQuery->ExecQuery();
            }
        }
        DBTransaction.Commit();
        DBTransaction.StartTransaction();
    }
    catch(Exception &Exc)
    {
        DBTransaction.Rollback();
        DBTransaction.StartTransaction();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Exc.Message);
    }
}
void TManagerAustriaFiscal::TakeCorrectiveMeasures(Database::TDBTransaction &DBTransaction)
{
    try
    {
        std::vector<TReceiptRequestAustriaFiscal> receiptsPending;
        receiptsPending.clear();
        GetOldInvoices(receiptsPending,DBTransaction);
        if(receiptsPending.size() > 0)
        {
            if(IsZeroReceiptSuccessful())
            {
                GetInvoiceDetails(receiptsPending,DBTransaction);
                SendOldInvoices(receiptsPending,DBTransaction);
            }
        }
    }
    catch(Exception &Exc)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Exc.Message);
    }
}
void TManagerAustriaFiscal::GetOldInvoices(std::vector<TReceiptRequestAustriaFiscal> &receiptsPending,Database::TDBTransaction &DBTransaction)
{
    try
    {
        TIBSQL *IBInternalQuery= DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->SQL->Text = "SELECT * FROM AUSTRIAFISCALRESPONSE WHERE IS_SIGNED = 'F'";
        IBInternalQuery->ExecQuery();
        for(;!IBInternalQuery->Eof;IBInternalQuery->Next())
        {
            TReceiptRequestAustriaFiscal receipt;
            receipt.CashBoxID = IBInternalQuery->FieldByName("CASHBOXID")->AsString;
            receipt.TerminalID = IBInternalQuery->FieldByName("TERMINALID")->AsString;
            receipt.ReceiptReference = IBInternalQuery->FieldByName("MMINVOICENUMBER")->AsString;
            receipt.ReceiptMoment = GetMomentForReceipt(receipt.ReceiptReference,DBTransaction);
            receiptsPending.push_back(receipt);
        }
        DBTransaction.Commit();
        DBTransaction.StartTransaction();
    }
    catch(Exception &ex)
    {
        DBTransaction.Rollback();
        DBTransaction.StartTransaction();
    }
}
void TManagerAustriaFiscal::GetInvoiceDetails(std::vector<TReceiptRequestAustriaFiscal> &receiptsPending,Database::TDBTransaction &DBTransaction)
{
    try
    {
        TIBSQL *IBInternalQuery= DBTransaction.Query(DBTransaction.AddQuery());
        for(int i = 0; i < receiptsPending.size(); i++)
        {
            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text = "SELECT * FROM AUSTRIAFISCALINVOICEDETAILS WHERE MMINVOICENUMBER = :MMINVOICENUMBER";
            IBInternalQuery->ParamByName("MMINVOICENUMBER")->AsString = receiptsPending[i].ReceiptReference;
            IBInternalQuery->ExecQuery();
            for(;!IBInternalQuery->Eof;IBInternalQuery->Next())
            {
                if(IBInternalQuery->FieldByName("IS_PAYMENT")->AsString == "T")
                {
                    TPayItemAustriaFiscal payItem;
                    payItem.Quantity = IBInternalQuery->FieldByName("QTY")->AsFloat;
                    payItem.Description = IBInternalQuery->FieldByName("DESCRIPTION")->AsString;
                    payItem.Amount = IBInternalQuery->FieldByName("AMOUNT")->AsFloat;
                    UnicodeString state = IBInternalQuery->FieldByName("ITEMCASE")->AsString;
                    __int64 itemCase = StrToInt64(state);
                    payItem.PayItemCase = itemCase;
                    receiptsPending[i].PayItems.push_back(payItem);
                }
                else
                {
                    TChargeItemAustriaFiscal chargeItem;
                    chargeItem.Quantity = IBInternalQuery->FieldByName("QTY")->AsFloat;
                    chargeItem.Description = IBInternalQuery->FieldByName("DESCRIPTION")->AsString;
                    chargeItem.Amount = IBInternalQuery->FieldByName("AMOUNT")->AsFloat;
                    UnicodeString state = IBInternalQuery->FieldByName("ITEMCASE")->AsString;
                    __int64 itemCase = StrToInt64(state);
                    chargeItem.ChargeItemCase = itemCase;
                    chargeItem.VATRate = IBInternalQuery->FieldByName("VATRATE")->AsFloat;
                    receiptsPending[i].ChargeItems.push_back(chargeItem);
                }
            }
        }
        DBTransaction.Commit();
        DBTransaction.StartTransaction();
    }
    catch(Exception &ex)
    {
        DBTransaction.Rollback();
        DBTransaction.StartTransaction();
    }
}
void TManagerAustriaFiscal::SendOldInvoices(std::vector<TReceiptRequestAustriaFiscal> &receiptsPending,Database::TDBTransaction &DBTransaction)
{
    try
    {
        for(int i = 0; i < receiptsPending.size(); i++)
        {
            std::auto_ptr<TAustriaFiscalInterface> austriaInterface(new TAustriaFiscalInterface());
            TReceiptResponseAustriaFiscal response = austriaInterface->PostDataToAustriaFiscal(receiptsPending[i]);
            bool isSigned = false;
            for(int indexSig = 0; indexSig < response.Signatures.size(); indexSig++)
            {
                if(response.Signatures[indexSig].Data != "")
                {
                    isSigned = true;
                    break;
                }
            }
            if(isSigned)
                UpdateInvoiceDetails(DBTransaction,receiptsPending[i],response);
        }
    }
    catch(Exception &Exc)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Exc.Message);
    }
}
void TManagerAustriaFiscal::StoreInvoiceDetailsinDB(TReceiptRequestAustriaFiscal receiptAustriaFiscal,Database::TDBTransaction &DBTransaction)
{
    try
    {
        TIBSQL *IBInternalQuery= DBTransaction.Query(DBTransaction.AddQuery());
        for(int i = 0; i < receiptAustriaFiscal.ChargeItems.size(); i++)
        {
            IBInternalQuery->Close();
            int primaryKey = TGeneratorManager::GetNextGeneratorKey(DBTransaction,"GEN_AUSTRIAFISCALDETAILS_ID");
            IBInternalQuery->SQL->Text = " INSERT INTO AUSTRIAFISCALINVOICEDETAILS "
                     " ( DETAILSID, MMINVOICENUMBER, DESCRIPTION, QTY, AMOUNT, ITEMCASE, VATRATE, IS_PAYMENT ) VALUES"
                     " ( :DETAILSID, :MMINVOICENUMBER, :DESCRIPTION, :QTY, :AMOUNT, :ITEMCASE, :VATRATE, :IS_PAYMENT )";
            IBInternalQuery->ParamByName("DETAILSID")->AsInteger = primaryKey;
            IBInternalQuery->ParamByName("MMINVOICENUMBER")->AsString = receiptAustriaFiscal.ReceiptReference;
            IBInternalQuery->ParamByName("DESCRIPTION")->AsString = receiptAustriaFiscal.ChargeItems[i].Description;
            IBInternalQuery->ParamByName("QTY")->AsFloat = receiptAustriaFiscal.ChargeItems[i].Quantity;
            IBInternalQuery->ParamByName("AMOUNT")->AsFloat = receiptAustriaFiscal.ChargeItems[i].Amount;
            UnicodeString itemCase = receiptAustriaFiscal.ChargeItems[i].ChargeItemCase;
            IBInternalQuery->ParamByName("ITEMCASE")->AsString = itemCase;
            IBInternalQuery->ParamByName("VATRATE")->AsFloat = receiptAustriaFiscal.ChargeItems[i].VATRate;
            IBInternalQuery->ParamByName("IS_PAYMENT")->AsString = "F";
            IBInternalQuery->ExecQuery();
        }
        for(int i = 0; i < receiptAustriaFiscal.PayItems.size(); i++)
        {
            IBInternalQuery->Close();
            int primaryKey = TGeneratorManager::GetNextGeneratorKey(DBTransaction,"GEN_AUSTRIAFISCALDETAILS_ID");
            IBInternalQuery->SQL->Text = " INSERT INTO AUSTRIAFISCALINVOICEDETAILS "
                     " ( DETAILSID, MMINVOICENUMBER, DESCRIPTION, QTY, AMOUNT, ITEMCASE, VATRATE, IS_PAYMENT ) VALUES"
                     " ( :DETAILSID, :MMINVOICENUMBER, :DESCRIPTION, :QTY, :AMOUNT, :ITEMCASE, :VATRATE, :IS_PAYMENT )";
            IBInternalQuery->ParamByName("DETAILSID")->AsInteger = primaryKey;
            IBInternalQuery->ParamByName("MMINVOICENUMBER")->AsString = receiptAustriaFiscal.ReceiptReference;
            IBInternalQuery->ParamByName("DESCRIPTION")->AsString = receiptAustriaFiscal.PayItems[i].Description;
            IBInternalQuery->ParamByName("QTY")->AsFloat = receiptAustriaFiscal.PayItems[i].Quantity;
            IBInternalQuery->ParamByName("AMOUNT")->AsFloat = receiptAustriaFiscal.PayItems[i].Amount;
            UnicodeString itemCase = receiptAustriaFiscal.PayItems[i].PayItemCase;
            IBInternalQuery->ParamByName("ITEMCASE")->AsString = itemCase;
            IBInternalQuery->ParamByName("VATRATE")->AsFloat = 0;
            IBInternalQuery->ParamByName("IS_PAYMENT")->AsString = "T";
            IBInternalQuery->ExecQuery();
        }
        DBTransaction.Commit();
        DBTransaction.StartTransaction();
    }
    catch(Exception &Exc)
    {
        DBTransaction.Rollback();
        DBTransaction.StartTransaction();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Exc.Message);
    }
}
void TManagerAustriaFiscal::UpdateInvoiceDetails(Database::TDBTransaction &DBTransaction,TReceiptRequestAustriaFiscal receipt,TReceiptResponseAustriaFiscal response)
{
    try
    {
        TIBSQL *IBInternalQuery= DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = " UPDATE AUSTRIAFISCALRESPONSE SET IS_SIGNED = 'T' WHERE MMINVOICENUMBER = :MMINVOICENUMBER";
        IBInternalQuery->ParamByName("MMINVOICENUMBER")->AsString = receipt.ReceiptReference;
        IBInternalQuery->ExecQuery();

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = " SELECT RESPONSE_ID FROM AUSTRIAFISCALRESPONSE WHERE MMINVOICENUMBER = :MMINVOICENUMBER";
        IBInternalQuery->ParamByName("MMINVOICENUMBER")->AsString = receipt.ReceiptReference;
        IBInternalQuery->ExecQuery();
        int responseId = IBInternalQuery->FieldByName("RESPONSE_ID")->AsInteger;
        bool isSigned = false;
        for(int indexSig = 0; indexSig < response.Signatures.size(); indexSig++)
        {
            if(response.Signatures[indexSig].Data != "")
            {
                isSigned = true;
                break;
            }
        }


        if(isSigned)
        {
            for(int i = 0; i < response.Signatures.size(); i++)
            {
                IBInternalQuery->Close();
                int primaryKeySignature = TGeneratorManager::GetNextGeneratorKey(DBTransaction,"GEN_AUSTRIAFISCALSIGNATURE_ID");
                IBInternalQuery->SQL->Text = " INSERT INTO AUSTRIAFISCALSIGNATURES "
                         " ( SIGNATURE_ID, RESPONSE_ID, SIGNATUREFORMAT, SIGNATURETYPE, CAPTION, DATA ) VALUES"
                         " ( :SIGNATURE_ID, :RESPONSE_ID, :SIGNATUREFORMAT, :SIGNATURETYPE, :CAPTION, :DATA )";
                IBInternalQuery->ParamByName("SIGNATURE_ID")->AsInteger = primaryKeySignature;
                IBInternalQuery->ParamByName("RESPONSE_ID")->AsInteger = responseId;
                IBInternalQuery->ParamByName("SIGNATUREFORMAT")->AsString = response.Signatures[i].SignatureFormat;
                IBInternalQuery->ParamByName("SIGNATURETYPE")->AsString = response.Signatures[i].SignatureType;
                IBInternalQuery->ParamByName("CAPTION")->AsString = response.Signatures[i].Caption;
                IBInternalQuery->ParamByName("DATA")->AsString = response.Signatures[i].Data;
                IBInternalQuery->ExecQuery();
            }
        }
        DBTransaction.Commit();
        DBTransaction.StartTransaction();
    }
    catch(Exception &ex)
    {
        DBTransaction.Rollback();
        DBTransaction.StartTransaction();
    }
}
TDateTime TManagerAustriaFiscal::GetMomentForReceipt(UnicodeString invoiceNumber,Database::TDBTransaction &DBTransaction)
{
    TDateTime dt = Now();
    try
    {
        TIBSQL *IBInternalQuery= DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = " SELECT TIME_STAMP FROM DAYARCBILL WHERE INVOICE_NUMBER = :INVOICE_NUMBER "
                                     " UNION ALL "
                                     " SELECT TIME_STAMP FROM ARCBILL WHERE INVOICE_NUMBER = :INVOICE_NUMBER ";
        IBInternalQuery->ParamByName("INVOICE_NUMBER")->AsString = invoiceNumber;
        IBInternalQuery->ExecQuery();
        dt = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
        DBTransaction.Commit();
        DBTransaction.StartTransaction();
    }
    catch(Exception &ex)
    {
        DBTransaction.Rollback();
        DBTransaction.StartTransaction();
    }
    return dt;
}

