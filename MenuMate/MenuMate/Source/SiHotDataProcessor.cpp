//---------------------------------------------------------------------------


#pragma hdrstop

#include "SiHotDataProcessor.h"
#include "Math.h"
#include "MMMessageBox.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TRoomRequest TSiHotDataProcessor::CreateRoomRequest(std::vector<TSiHotAccounts> &siHotAccounts,AnsiString pmsIPAddress,int pmsPort)
{
    TRoomRequest roomRequest;
    roomRequest.TransactionNumber = GetTransNumber();
    for(std::vector<TSiHotAccounts>::iterator i = siHotAccounts.begin();
          i != siHotAccounts.end() ; ++i)
    {
        roomRequest.RoomNumber = i->AccountNumber;
    }
    roomRequest.IPAddress = pmsIPAddress;
    roomRequest.PortNumber = pmsPort;
    return roomRequest;
}
//----------------------------------------------------------------------------
void TSiHotDataProcessor::CreateRoomChargePost(TPaymentTransaction &_paymentTransaction, TRoomCharge &_roomCharge)
{
    bool AddDiscountPart = false;
    double discountValue = 0.0;
    _roomCharge.TransactionNumber = GetTransNumber();
    _roomCharge.AccountNumber = _paymentTransaction.Phoenix.AccountNumber;
    if(_roomCharge.AccountNumber == "")
        _roomCharge.AccountNumber = TDeviceRealTerminal::Instance().BasePMS->DefaultAccountNumber;
    UnicodeString billNo = GetInvoiceNumber(_paymentTransaction);

    // Iterate pyamentTransaction orders loop and identify the same III party codes
    // with same tax percentage over them to club item prices and their quantities.
    for(int i = 0; i < _paymentTransaction.Orders->Count; i++)
    {

        TItemComplete *itemComplete = ((TItemComplete*)_paymentTransaction.Orders->Items[i]);
        double currentVAT = GetVATpercentage(itemComplete);

        // Cancelled orders should not get posted
        if(itemComplete->OrderType == CanceledOrder)
            continue;

        bool AddDiscountPart = AddItemToSiHotService(itemComplete,billNo,_roomCharge);
        if(AddDiscountPart)
           AddDiscountPartToService(itemComplete,_roomCharge, _paymentTransaction, billNo);

        // loop for subitems
        for(int j = 0; j < itemComplete->SubOrders->Count; j++)
        {
            TSiHotService siHotService;
            TItemComplete *subItemComplete = ((TItemComplete*)itemComplete->SubOrders->Items[j]);
            // Cancelled orders should not get posted
            if(subItemComplete->OrderType ==  CanceledOrder)
               continue;

            bool AddDiscountPart = AddItemToSiHotService(subItemComplete,billNo,_roomCharge);
            if(AddDiscountPart)
               AddDiscountPartToService(subItemComplete,_roomCharge, _paymentTransaction, billNo);
        }
    }
    // Populate Discount Details in case of Tax on prediscounted price of items
    for(std::map <UnicodeString,TSiHotService>::iterator serviceItDisc = serviceSiHotDiscPart.begin(); serviceItDisc != serviceSiHotDiscPart.end(); ++serviceItDisc)
    {
       _roomCharge.SiHotServices.push_back(serviceItDisc->second);
    }

    // Add ServiceCharge as service to SiHot
    if(_paymentTransaction.Money.ServiceCharge != 0)
    {
        AddServiceChargeAsService(_roomCharge, billNo, _paymentTransaction);
    }

    // Add Rounding as service to SiHot
//    if(RoundTo((double)(_paymentTransaction.Money.PaymentRounding),-2) != 0.00)
//    {
//        AddRoundingAsService(_roomCharge, billNo, _paymentTransaction);
//    }

    // Adding payment types
    AddPaymentMethods(_roomCharge, billNo, _paymentTransaction);
    for(std::map <UnicodeString,TSiHotPayments>::iterator paymentIt = paymentSiHot.begin(); paymentIt != paymentSiHot.end(); ++paymentIt)
    {
        _roomCharge.SiHotPayments.push_back(paymentIt->second);
    }

    _roomCharge.IPAddress = TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress;
    _roomCharge.PortNumber = TDeviceRealTerminal::Instance().BasePMS->TCPPort;
}
//----------------------------------------------------------------------------
bool TSiHotDataProcessor::AddItemToSiHotService(TItemComplete *itemComplete,UnicodeString billNo,
                                                TRoomCharge &_roomCharge)
{
    double discountValue = 0.0;
    bool AddDiscountPart = false;
    double taxPercentage = GetVATpercentage(itemComplete);
    UnicodeString categoryCode = itemComplete->ThirdPartyCode;
    if(categoryCode == "")
        categoryCode = TDeviceRealTerminal::Instance().BasePMS->DefaultItemCategory;
    TSiHotService siHotService;
    siHotService.SuperCategory = categoryCode;
    siHotService.SuperCategory_Desc = "";
    siHotService.MiddleCategory = categoryCode;
    siHotService.MiddleCategory_Desc = "";
    siHotService.ArticleCategory = categoryCode;
    siHotService.ArticleCategory_Desc = "";
    siHotService.ArticleNo = categoryCode;
    siHotService.ArticleNo_Desc = "";
    if(TGlobalSettings::Instance().Instance().ReCalculateTaxPostDiscount)
    {

        siHotService.PricePerUnit = GetPriceTotal(itemComplete, true)/(double)itemComplete->GetQty();
        siHotService.Amount = (double)itemComplete->GetQty();
        siHotService.PriceTotal = GetPriceTotal(itemComplete, true);
    }
    else
    {
        siHotService.PricePerUnit = GetPriceTotal(itemComplete, false)/(double)itemComplete->GetQty();
        siHotService.Amount = (double)itemComplete->GetQty();
        siHotService.PriceTotal = GetPriceTotal(itemComplete, false);
        AddDiscountPart = (double)itemComplete->BillCalcResult.TotalDiscount != 0.0 ? true : false;
        discountValue = (double)itemComplete->BillCalcResult.TotalDiscount;
    }
    siHotService.VATPercentage = fabs(taxPercentage);
    siHotService.Billno = billNo;
    siHotService.Cashno = TDeviceRealTerminal::Instance().BasePMS->POSID;
    siHotService.Cashier = TDeviceRealTerminal::Instance().User.Name;
    siHotService.Source = "Guest";
    _roomCharge.SiHotServices.push_back(siHotService);
    return AddDiscountPart;
}
//----------------------------------------------------------------------------
double TSiHotDataProcessor::GetPriceTotal(TItemComplete* itemComplete, bool recalculateTax)
{
     Currency price = 0;
     if(recalculateTax)
     {
         price = fabs((double)itemComplete->BillCalcResult.FinalPrice
                -((double)itemComplete->BillCalcResult.ServiceCharge.Value)
                -(double)(itemComplete->BillCalcResult.ServiceCharge.TaxValue));
     }
     else
     {
         price = fabs((double)itemComplete->BillCalcResult.FinalPrice
                 -((double)itemComplete->BillCalcResult.ServiceCharge.Value)
                 -((double)itemComplete->BillCalcResult.TotalDiscount)
                 -(double)itemComplete->BillCalcResult.ServiceCharge.TaxValue);
     }
     return price;
}
//----------------------------------------------------------------------------
void TSiHotDataProcessor::AddExpensesToSiHotService(TPayment* payment, TRoomCharge &_roomCharge,UnicodeString billno)
{
    TSiHotService siHotService;
    siHotService.SuperCategory = TDeviceRealTerminal::Instance().BasePMS->ExpensesAccount;
    siHotService.SuperCategory_Desc = "Expenses";
    siHotService.MiddleCategory = TDeviceRealTerminal::Instance().BasePMS->ExpensesAccount;
    siHotService.MiddleCategory_Desc = "Expenses";
    siHotService.ArticleCategory = TDeviceRealTerminal::Instance().BasePMS->ExpensesAccount;
    siHotService.ArticleCategory_Desc = "Expenses";
    siHotService.ArticleNo = TDeviceRealTerminal::Instance().BasePMS->ExpensesAccount;
    siHotService.ArticleNo_Desc = "Expenses";
    siHotService.PricePerUnit = fabs((double)payment->GetCashOut());
    siHotService.Amount = 1;
    siHotService.PriceTotal = fabs((double)payment->GetCashOut());
    siHotService.VATPercentage = 0;;
    siHotService.Billno = billno;
    siHotService.Cashno = TDeviceRealTerminal::Instance().BasePMS->POSID;
    siHotService.Cashier = TDeviceRealTerminal::Instance().User.Name;
    siHotService.Source = "Guest";
    _roomCharge.SiHotServices.push_back(siHotService);
}
//----------------------------------------------------------------------------
void TSiHotDataProcessor::AddDiscountPartToService(TItemComplete *itemComplete,TRoomCharge &_roomCharge, TPaymentTransaction &_paymentTransaction
                                                   ,UnicodeString _billNo)
{
    TSiHotService siHotService;
    UnicodeString categoryCode = itemComplete->ThirdPartyCode;
    if(categoryCode == "")
        categoryCode = TDeviceRealTerminal::Instance().BasePMS->DefaultItemCategory;

    std::map<UnicodeString,TSiHotService>::iterator it;

    it = serviceSiHotDiscPart.find(categoryCode);
    if(it == serviceSiHotDiscPart.end())
    {
        // if value not found
        siHotService.SuperCategory = categoryCode;
        siHotService.SuperCategory_Desc = "";
        siHotService.MiddleCategory = categoryCode;
        siHotService.MiddleCategory_Desc = "";
        siHotService.ArticleCategory = categoryCode;
        siHotService.ArticleCategory_Desc = "";
        siHotService.ArticleNo = categoryCode;
        siHotService.ArticleNo_Desc = "";
        siHotService.PricePerUnit = fabs(itemComplete->BillCalcResult.TotalDiscount);
        if(itemComplete->GetQty() > 0.0)
        {
           if(itemComplete->BillCalcResult.TotalDiscount < 0.0)
              siHotService.Amount = -1;
           else
              siHotService.Amount = 1;
        }
        else
        {
           if(itemComplete->BillCalcResult.TotalDiscount < 0.0)
              siHotService.Amount = 1;
           else
              siHotService.Amount = -1;
        }
        siHotService.PriceTotal = fabs(itemComplete->BillCalcResult.TotalDiscount);
        siHotService.VATPercentage = 0;
        siHotService.Billno = _billNo;
        siHotService.Cashno = TDeviceRealTerminal::Instance().BasePMS->POSID;
        siHotService.Cashier = TDeviceRealTerminal::Instance().User.Name;
        siHotService.Source = "Guest";
        serviceSiHotDiscPart[categoryCode] = siHotService;
    }
    else
    {
       it->second.PricePerUnit = StrToCurr(it->second.PricePerUnit) -(double)(itemComplete->BillCalcResult.TotalDiscount);
       it->second.PriceTotal = StrToCurr(it->second.PriceTotal) - (double)(itemComplete->BillCalcResult.TotalDiscount);
    }
}
//----------------------------------------------------------------------------
void TSiHotDataProcessor::AddSurchargeAndTip( TRoomCharge &_roomCharge, double surcharge,
                          UnicodeString _billNo,double tip)
{
    double tipAmount = tip;
    // Add Tip as service to Sihot
    if(tipAmount != 0)
    {
        TSiHotService siHotService;
        siHotService.SuperCategory = TDeviceRealTerminal::Instance().BasePMS->TipAccount;
        siHotService.SuperCategory_Desc = "Tip";
        siHotService.MiddleCategory = TDeviceRealTerminal::Instance().BasePMS->TipAccount;
        siHotService.MiddleCategory_Desc = "Tip";
        siHotService.ArticleCategory = TDeviceRealTerminal::Instance().BasePMS->TipAccount;
        siHotService.ArticleCategory_Desc = "Tip";
        siHotService.ArticleNo = TDeviceRealTerminal::Instance().BasePMS->TipAccount;
        siHotService.ArticleNo_Desc = "Tip";
        siHotService.PricePerUnit = fabs((double)(tipAmount));
        siHotService.Amount = tipAmount < 0 ? "-1" : "1";
        siHotService.PriceTotal = fabs((double)(tipAmount));
        siHotService.VATPercentage = 0;
        siHotService.Billno = _billNo;
        siHotService.Cashno = TDeviceRealTerminal::Instance().BasePMS->POSID;
        siHotService.Cashier = TDeviceRealTerminal::Instance().User.Name;
        siHotService.Source = "Guest";
        _roomCharge.SiHotServices.push_back(siHotService);
    }

    // Add surcharge as service to SiHot
    if(surcharge)
    {
        TSiHotService siHotServiceSurcharge;
        siHotServiceSurcharge.SuperCategory = TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount;
        siHotServiceSurcharge.SuperCategory_Desc = "SurCharge";
        siHotServiceSurcharge.MiddleCategory = TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount;
        siHotServiceSurcharge.MiddleCategory_Desc = "Surcharge";
        siHotServiceSurcharge.ArticleCategory = TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount;
        siHotServiceSurcharge.ArticleCategory_Desc = "Surcharge";
        siHotServiceSurcharge.ArticleNo = TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount;
        siHotServiceSurcharge.ArticleNo_Desc = "Surcharge";
        siHotServiceSurcharge.PricePerUnit = fabs(surcharge);
        siHotServiceSurcharge.Amount = surcharge < 0 ? "-1" : "1";
        siHotServiceSurcharge.PriceTotal = fabs(surcharge);
        siHotServiceSurcharge.VATPercentage = 0;
        siHotServiceSurcharge.Billno = _billNo;
        siHotServiceSurcharge.Cashno = TDeviceRealTerminal::Instance().BasePMS->POSID;
        siHotServiceSurcharge.Cashier = TDeviceRealTerminal::Instance().User.Name;
        siHotServiceSurcharge.Source = "Guest";
        _roomCharge.SiHotServices.push_back(siHotServiceSurcharge);
    }
}
//----------------------------------------------------------------------------
double TSiHotDataProcessor::GetVATpercentage(TItemComplete *itemComplete)
{
    double percentage = 0.0;
    for(std::vector<BillCalculator::TTaxResult>::iterator i = itemComplete->BillCalcResult.Tax.begin();
          i != itemComplete->BillCalcResult.Tax.end() ; ++i)
    {
        percentage += (double)i->Percentage;
    }
    return percentage;
}
//----------------------------------------------------------------------------
UnicodeString TSiHotDataProcessor::GetInvoiceNumber(TPaymentTransaction _paymentTransaction)
{
    UnicodeString invoiceNumber = "";
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *IBInternalQueryGenerator= DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQueryGenerator->Close();
        switch(_paymentTransaction.TypeOfSale)
        {
           case 0:
           {
                IBInternalQueryGenerator->SQL->Text = "SELECT GEN_ID(GEN_INVOICENUMBER, 0) FROM RDB$DATABASE ";
                IBInternalQueryGenerator->ExecQuery();
                int number = IBInternalQueryGenerator->Fields[0]->AsInteger + 1;
                invoiceNumber = IntToStr(number);
                break;
           }
           case 1:
           {
                IBInternalQueryGenerator->SQL->Text = "SELECT GEN_ID(GEN_INVOICENUMBERCOMP, 0) FROM RDB$DATABASE ";
                IBInternalQueryGenerator->ExecQuery();
                int number = IBInternalQueryGenerator->Fields[0]->AsInteger + 1;
                invoiceNumber = "Comp " + IntToStr(number);
                break;
           }
           case 2:
           {
                IBInternalQueryGenerator->SQL->Text = "SELECT GEN_ID(GEN_INVOICENUMBERNC, 0) FROM RDB$DATABASE ";
                IBInternalQueryGenerator->ExecQuery();
                int number = IBInternalQueryGenerator->Fields[0]->AsInteger + 1;
                invoiceNumber = "NC "+ IntToStr(number);
                break;
           }
        }
        DBTransaction.Commit();
        return invoiceNumber;
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
        DBTransaction.Rollback();
    }
    return invoiceNumber;
}
//----------------------------------------------------------------------------
int TSiHotDataProcessor::GetTransNumber()
{
    int value = 0;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try{
        TIBSQL *IBInternalQueryGenerator= DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQueryGenerator->Close();
        IBInternalQueryGenerator->SQL->Text = "SELECT GEN_ID(GEN_SIHOTTRANSNUMBER, 1) FROM RDB$DATABASE ";
        IBInternalQueryGenerator->ExecQuery();
        int value = IBInternalQueryGenerator->Fields[0]->AsInteger;
        DBTransaction.Commit();
        return value;
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
        DBTransaction.Rollback();
    }
    return value;
}
//----------------------------------------------------------------------------
void TSiHotDataProcessor::PrepareRoomStatus(std::vector<TSiHotAccounts> &siHotAccounts,TRoomResponse roomResponse)
{
    siHotAccounts.clear();
    for(std::vector<TGuestDetails>::iterator i = roomResponse.GuestsInformation.begin();
                                       i != roomResponse.GuestsInformation.end() ; ++i)
    {
        TAccountDetails siHotAccountDetail;
        TSiHotAccounts siHotAccount;
        if(i->AccountActive == "1")
        {
            siHotAccount.AccountNumber = i->AccountNumber;
            siHotAccountDetail.RoomNumber = i->RoomNumber;
            siHotAccountDetail.Balance = 0;
            siHotAccountDetail.CreditLimit = i->Limit;
            siHotAccountDetail.FirstName = i->FirstName;
            siHotAccountDetail.LastName = i->LastName;
            siHotAccount.AccountDetails.push_back(siHotAccountDetail);
            siHotAccounts.push_back(siHotAccount);
        }
    }
}
//----------------------------------------------------------------------------
void TSiHotDataProcessor::AddServiceChargeAsService(TRoomCharge &_roomCharge, UnicodeString billNo, TPaymentTransaction &_paymentTransaction)
{
    TSiHotService siHotService;
    siHotService.SuperCategory = TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount;
    siHotService.SuperCategory_Desc = "Service Charge";
    siHotService.MiddleCategory = TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount;
    siHotService.MiddleCategory_Desc = "Service Charge";
    siHotService.ArticleCategory = TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount;
    siHotService.ArticleCategory_Desc = "Service Charge";
    siHotService.ArticleNo = TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount;
    siHotService.ArticleNo_Desc = "Service Charge";
    siHotService.PricePerUnit = fabs((double)(_paymentTransaction.Money.ServiceCharge+_paymentTransaction.Money.ServiceChargeTax));
    siHotService.Amount = _paymentTransaction.Money.ServiceCharge < 0 ? "-1" : "1";
    siHotService.PriceTotal = fabs((double)(_paymentTransaction.Money.ServiceCharge+_paymentTransaction.Money.ServiceChargeTax));
    if(TGlobalSettings::Instance().ApplyServiceChargeTax)
        siHotService.VATPercentage = TGlobalSettings::Instance().ServiceChargeTaxRate;
    else
        siHotService.VATPercentage = 0;
    siHotService.Billno = billNo;
    siHotService.Cashno = TDeviceRealTerminal::Instance().BasePMS->POSID;
    siHotService.Cashier = TDeviceRealTerminal::Instance().User.Name;
    siHotService.Source = "Guest";
    _roomCharge.SiHotServices.push_back(siHotService);
}
//----------------------------------------------------------------------------
void TSiHotDataProcessor::AddRoundingAsService(TRoomCharge &_roomCharge, UnicodeString billNo, TPaymentTransaction &_paymentTransaction)
{
    TSiHotService siHotService;
    siHotService.SuperCategory = TDeviceRealTerminal::Instance().BasePMS->RoundingCategory;
    siHotService.SuperCategory_Desc = "Rounding";
    siHotService.MiddleCategory = TDeviceRealTerminal::Instance().BasePMS->RoundingCategory;
    siHotService.MiddleCategory_Desc = "Rounding";
    siHotService.ArticleCategory = TDeviceRealTerminal::Instance().BasePMS->RoundingCategory;
    siHotService.ArticleCategory_Desc = "Rounding";
    siHotService.ArticleNo = TDeviceRealTerminal::Instance().BasePMS->RoundingCategory;
    siHotService.ArticleNo_Desc = "Rounding";
    siHotService.PricePerUnit = fabs((double)(_paymentTransaction.Money.PaymentRounding));
    siHotService.Amount = _paymentTransaction.Money.RoundingAdjustment < 0 ? "-1" : "1";
    siHotService.PriceTotal = fabs((double)(_paymentTransaction.Money.PaymentRounding));
    siHotService.VATPercentage = 0;
    siHotService.Billno = billNo;
    siHotService.Cashno = TDeviceRealTerminal::Instance().BasePMS->POSID;
    siHotService.Cashier = TDeviceRealTerminal::Instance().User.Name;
    siHotService.Source = "Guest";
    _roomCharge.SiHotServices.push_back(siHotService);
}
//----------------------------------------------------------------------------
void TSiHotDataProcessor::AddPaymentMethods(TRoomCharge &_roomCharge, UnicodeString billNo, TPaymentTransaction &_paymentTransaction)
{
    double surcharge = 0.0;
    UnicodeString indexcash = "";
    double cashOutStore = 0.0;
    double tip = 0.0;
    UnicodeString cashType = "";
    for(int i = 0; i < _paymentTransaction.PaymentsCount(); i++)
    {
        TSiHotPayments siHotPayment;
        TPayment *payment = _paymentTransaction.PaymentGet(i);
        if(payment->GetPaymentAttribute(ePayTypeCustomSurcharge))
        {
            tip += (double)payment->GetAdjustment();
        }
        if(payment->GetPaymentAttribute(ePayTypeSurcharge))
        {
            surcharge += (double)payment->GetAdjustment();
        }
        if(payment->GetPaymentAttribute(ePayTypeCash))
        {
            cashType = payment->PaymentThirdPartyID;
        }

        if(((payment->GetPayTendered() != 0) || (payment->GetCashOut() != 0.0)) && !(payment->GetPaymentAttribute(ePayTypeRoomInterface)))
        {
            tip += (double)payment->TipAmount;
            siHotPayment.Type = payment->PaymentThirdPartyID;
            if(siHotPayment.Type == "")
                siHotPayment.Type = TDeviceRealTerminal::Instance().BasePMS->DefaultPaymentCategory;
            if(payment->GetPaymentAttribute(ePayTypePoints))
               siHotPayment.Type = TDeviceRealTerminal::Instance().BasePMS->PointsCategory;
            if(payment->GetPaymentAttribute(ePayTypeCredit))
               siHotPayment.Type = TDeviceRealTerminal::Instance().BasePMS->CreditCategory;
            std::map<UnicodeString, TSiHotPayments>::iterator iter;
            iter = paymentSiHot.find(siHotPayment.Type);
            if(iter == paymentSiHot.end())
            {
                siHotPayment.Amount = -(payment->GetPayTendered() + payment->GetCashOut() - payment->GetChange());
                siHotPayment.Description = payment->Name;
                siHotPayment.Billno = billNo;
                siHotPayment.Cashno = TDeviceRealTerminal::Instance().BasePMS->POSID;
                siHotPayment.Cashier = TDeviceRealTerminal::Instance().User.Name;
                siHotPayment.Source = "Guest";
                paymentSiHot[siHotPayment.Type] = siHotPayment;
            }
            else
            {
                double amount = StrToCurr(iter->second.Amount) + (-(payment->GetPayTendered() + payment->GetCashOut()- payment->GetChange()));
                iter->second.Amount = amount;
            }
            if(payment->GetPaymentAttribute(ePayTypeCash))
            {
                indexcash = "Cash";
            }
            cashOutStore += (double)payment->GetCashOut();
        }
        if(i + 1 == _paymentTransaction.PaymentsCount())
        {
            if(indexcash == "" && cashOutStore != 0.0)
            {
                TSiHotPayments siHotPaymentCash;
                siHotPaymentCash.Type = cashType;
                siHotPaymentCash.Amount = (double)cashOutStore;
                siHotPaymentCash.Description = "Cash";
                siHotPaymentCash.Billno = billNo;
                siHotPaymentCash.Cashno = TDeviceRealTerminal::Instance().BasePMS->POSID;
                siHotPaymentCash.Cashier = TDeviceRealTerminal::Instance().User.Name;
                siHotPaymentCash.Source = "Guest";
                paymentSiHot[cashType] = siHotPaymentCash;
            }
        }
        if((payment->GetPaymentAttribute(ePayTypeRoomInterface)) && payment->GetCashOut() != 0)
        {
            AddExpensesToSiHotService(payment,_roomCharge,billNo);
        }
    }
//    if(_paymentTransaction.Money.)
    if((surcharge != 0) /*|| (_paymentTransaction.Money.ProductSurcharge != 0)*/ || tip != 0)
    {
        AddSurchargeAndTip(_roomCharge,surcharge,billNo,tip);
    }
}
//----------------------------------------------------------------------------
bool TSiHotDataProcessor::GetDefaultAccount(AnsiString tcpIPAddress,AnsiString tcpPort)
{
    bool retValue = false;
    std::auto_ptr<TSiHotInterface> siHotInterface (new TSiHotInterface());
    TRoomRequest roomRequest;
    roomRequest.IPAddress = tcpIPAddress;
    roomRequest.PortNumber = atoi(tcpPort.c_str());
    roomRequest.TransactionNumber = GetTransNumber();
    roomRequest.RoomNumber = TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount;
    TRoomResponse roomresponse;
    roomresponse = siHotInterface->SendRoomRequest(roomRequest) ;
    if(roomresponse.GuestsInformation.size() != 0)
    {
        if(roomresponse.GuestsInformation[0].AccountNumber != "")
        {
            Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
            DBTransaction.StartTransaction();
            try
            {
                TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmSiHotDefaultTransaction,roomresponse.GuestsInformation[0].AccountNumber);
                DBTransaction.Commit();
                return true;
            }
            catch(Exception &ex)
            {
                TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
                DBTransaction.Rollback();
            }
        }
    }
    return retValue;
}
//----------------------------------------------------------------------------
bool TSiHotDataProcessor::GetRoundingAccounting(AnsiString tcpIPAddress,AnsiString tcpPort)
{
    bool retValue = false;
    std::auto_ptr<TSiHotInterface> siHotInterface (new TSiHotInterface());
    TRoomRequest roomRequest;
    roomRequest.IPAddress = tcpIPAddress;
    roomRequest.PortNumber = atoi(tcpPort.c_str());
    roomRequest.TransactionNumber = GetTransNumber();
    roomRequest.RoomNumber = TDeviceRealTerminal::Instance().BasePMS->RoundingAccountSiHot;
    TRoomResponse roomresponse;
    roomresponse = siHotInterface->SendRoomRequest(roomRequest);
    if(roomresponse.GuestsInformation.size() != 0)
    {
        if(roomresponse.GuestsInformation[0].AccountNumber != "")
        {
            Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
            DBTransaction.StartTransaction();
            try
            {
                TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmSiHotRounding,roomresponse.GuestsInformation[0].AccountNumber);
                DBTransaction.Commit();
                return true;
            }
            catch(Exception &ex)
            {
                TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
                DBTransaction.Rollback();
            }
        }
    }
    return retValue;
}
//----------------------------------------------------------------------------
