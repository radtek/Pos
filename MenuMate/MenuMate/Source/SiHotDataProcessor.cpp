//---------------------------------------------------------------------------


#pragma hdrstop

#include "SiHotDataProcessor.h"
#include "Math.h"
#include "MMMessageBox.h"
#include "GeneratorManager.h"

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
    roomRequest.IPAddress      = pmsIPAddress;
    roomRequest.PortNumber     = pmsPort;
    return roomRequest;
}
//----------------------------------------------------------------------------
void TSiHotDataProcessor::CreateRoomChargePost(TPaymentTransaction &_paymentTransaction, TRoomCharge &_roomCharge)
{
    bool AddDiscountPart          = false;
    totalBill                     = 0.0;
    double discountValue          = 0.0;
    _roomCharge.TransactionNumber = GetTransNumber();
    _roomCharge.AccountNumber     = _paymentTransaction.Phoenix.AccountNumber;
    _roomCharge.RoomNumber        = _paymentTransaction.Phoenix.RoomNumber;

    if(_roomCharge.AccountNumber == "" || _roomCharge.AccountNumber == TDeviceRealTerminal::Instance().BasePMS->DefaultAccountNumber)
    {
        _roomCharge.AccountNumber                 = TDeviceRealTerminal::Instance().BasePMS->DefaultAccountNumber;
        _paymentTransaction.Phoenix.AccountName   = TManagerVariable::Instance().GetStr(_paymentTransaction.DBTransaction,vmSiHotDefaultTransactionName);
        _paymentTransaction.Phoenix.RoomNumber    = TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount;
        _paymentTransaction.Phoenix.AccountNumber = _roomCharge.AccountNumber;
        _paymentTransaction.SalesType             = eRoomSale;
        _roomCharge.RoomNumber                    = TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount;

        for (int i = 0; i < _paymentTransaction.Orders->Count; i++)
        {
            TItemComplete *Order    = (TItemComplete*)_paymentTransaction.Orders->Items[i];
            if(Order->TabType != TabNone && Order->TabType != TabCashAccount)
                break;
            if(Order->TabType != TabCashAccount)
            {
                Order->TabType          = TabRoom;
                Order->TabContainerName = _paymentTransaction.Phoenix.RoomNumber;
                Order->TabName = _paymentTransaction.Phoenix.RoomNumber;
                Order->RoomNoStr        = _paymentTransaction.Phoenix.AccountNumber;
            }
        }
    }

    UnicodeString billNo = GetInvoiceNumber(_paymentTransaction);
    // Iterate pyamentTransaction orders loop and identify the same III party codes
    // with same tax percentage over them to club item prices and their quantities.
    for(int i = 0; i < _paymentTransaction.Orders->Count; i++)
    {

        TItemComplete *itemComplete = ((TItemComplete*)_paymentTransaction.Orders->Items[i]);
//        double currentVAT = GetVATpercentage(itemComplete);

        // Cancelled orders should not get posted
        if(itemComplete->OrderType == CanceledOrder)
            continue;

        bool AddDiscountPart        = AddItemToSiHotService(itemComplete,billNo,_roomCharge);
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

    // Adding payment types
    AddPaymentMethods(_roomCharge, billNo, _paymentTransaction);
    for(std::map <UnicodeString,TSiHotPayments>::iterator paymentIt = paymentSiHot.begin(); paymentIt != paymentSiHot.end(); ++paymentIt)
    {
        _roomCharge.SiHotPayments.push_back(paymentIt->second);
    }

    _roomCharge.IPAddress  = TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress;
    _roomCharge.PortNumber = TDeviceRealTerminal::Instance().BasePMS->TCPPort;
    _roomCharge.Total      = totalBill;
}
//----------------------------------------------------------------------------
bool TSiHotDataProcessor::AddItemToSiHotService(TItemComplete *itemComplete,UnicodeString billNo,
                                                TRoomCharge &_roomCharge)
{
    double discountValue = 0.0;
    bool AddDiscountPart = false;
    double taxPercentage = 0;
    if(!TGlobalSettings::Instance().SendNoTaxToSiHot)
        taxPercentage = GetVATpercentage(itemComplete);

    if(itemComplete->RevenueCode == 0)
    {
        for(std::map<int,TRevenueCodeDetails>::iterator itRev = TDeviceRealTerminal::Instance().BasePMS->RevenueCodesMap.begin();
            itRev!= TDeviceRealTerminal::Instance().BasePMS->RevenueCodesMap.end(); advance(itRev,1))
        {
            if(itRev->second.IsDefault)
            {
               itemComplete->RevenueCode = itRev->first;
               break;
            }
        }
    }

    UnicodeString categoryCode        = itemComplete->RevenueCode;

    if(categoryCode == "")
        categoryCode = TDeviceRealTerminal::Instance().BasePMS->DefaultItemCategory;
    TSiHotService siHotService;
    siHotService.SuperCategory        = categoryCode;
    siHotService.SuperCategory_Desc   = "";
    siHotService.MiddleCategory       = categoryCode;
    siHotService.MiddleCategory_Desc  = itemComplete->MenuName;
    if(TGlobalSettings::Instance().EnableItemDetailsPosting)
    {
        int alphaNumericCheck = 0;
        if(itemComplete->ItemSizeIdentifierKey > 0 && itemComplete->ItemSizeIdentifierKey <= 999 && itemComplete->ItemSizeIdentifierKey != NULL  && TryStrToInt(itemComplete->ItemSizeIdentifierKey,alphaNumericCheck))
        {
            siHotService.ArticleCategory      = IntToStr(itemComplete->ItemSizeIdentifierKey);
        }
        else
        {
            siHotService.ArticleCategory      = "999";
        }
        siHotService.ArticleCategory_Desc = itemComplete->Item + " (" + itemComplete->Size + ")";
    }
    else
    {
        siHotService.ArticleCategory      = categoryCode;
        siHotService.ArticleCategory_Desc = "";
    }
    siHotService.ArticleNo            = categoryCode;
    siHotService.ArticleNo_Desc       = "";
    if(TGlobalSettings::Instance().Instance().ReCalculateTaxPostDiscount)
    {
        totalBill += GetPriceTotal(itemComplete, true);
        double ppu = GetPriceTotal(itemComplete, true)/fabs((double)itemComplete->GetQty());
        ppu = RoundTo(ppu,-2);
        siHotService.PricePerUnit     = ppu;
        double amountValue1           = (double)itemComplete->GetQty();
        amountValue1                  = RoundTo(amountValue1,-2);
        siHotService.Amount           = amountValue1;//(double)itemComplete->GetQty();
        siHotService.PriceTotal       = GetPriceTotal(itemComplete, true);
    }
    else
    {
        totalBill += GetPriceTotal(itemComplete, false);
        double ppu = GetPriceTotal(itemComplete, false)/fabs((double)itemComplete->GetQty());
        ppu = RoundTo(ppu,-2);
        siHotService.PricePerUnit     = ppu;
        double amountValue1           = RoundTo((double)itemComplete->GetQty(),-2);
        amountValue1                  = RoundTo(amountValue1 , -2);
        siHotService.Amount           = amountValue1;//RoundTo((double)itemComplete->GetQty(),-2);
        siHotService.PriceTotal       = GetPriceTotal(itemComplete, false);
        AddDiscountPart               = (double)itemComplete->BillCalcResult.TotalDiscount != 0.0 ? true : false;
        discountValue =  RoundTo((double)itemComplete->BillCalcResult.TotalDiscount,-2);
    }
    siHotService.VATPercentage        = fabs(taxPercentage);
    siHotService.Billno               = billNo;
    siHotService.Cashno               = TDeviceRealTerminal::Instance().BasePMS->POSID;
    siHotService.Cashier              = TDeviceRealTerminal::Instance().User.Name;
    siHotService.Source               = "Guest";
    _roomCharge.SiHotServices.push_back(siHotService);
    return AddDiscountPart;
}
//----------------------------------------------------------------------------
double TSiHotDataProcessor::GetPriceTotal(TItemComplete* itemComplete, bool recalculateTax)
{
     double price = 0;
     if(!TGlobalSettings::Instance().SendNoTaxToSiHot)
     {
         if(recalculateTax)
         {
             price = fabs((double)itemComplete->BillCalcResult.FinalPrice);
         }
         else
         {
             price = fabs((double)itemComplete->BillCalcResult.FinalPrice
                     -(double)itemComplete->BillCalcResult.TotalDiscount);
         }
         price = RoundTo(price,-2);
     }
     else
     {
         if(recalculateTax)
         {
             price = fabs((double)itemComplete->BillCalcResult.FinalPrice -
                            (double)itemComplete->BillCalcResult.TotalTax -
                            (double)itemComplete->BillCalcResult.ServiceCharge.Value);
         }
         else
         {
             price = fabs((double)itemComplete->BillCalcResult.FinalPrice -
                            (double)itemComplete->BillCalcResult.TotalTax -
                            (double)itemComplete->BillCalcResult.ServiceCharge.Value -
                            (double)itemComplete->BillCalcResult.TotalDiscount);
         }
         price = RoundTo(price,-2);
     }
     return price;
}
//----------------------------------------------------------------------------
void TSiHotDataProcessor::AddExpensesToSiHotService(TPayment* payment, TRoomCharge &_roomCharge,UnicodeString billno)
{
    TSiHotService siHotService;
    siHotService.SuperCategory        = TDeviceRealTerminal::Instance().BasePMS->ExpensesAccount;
    siHotService.SuperCategory_Desc   = "Expenses";
    siHotService.MiddleCategory       = TDeviceRealTerminal::Instance().BasePMS->ExpensesAccount;
    siHotService.MiddleCategory_Desc  = "Expenses";
    siHotService.ArticleCategory      = TDeviceRealTerminal::Instance().BasePMS->ExpensesAccount;
    siHotService.ArticleCategory_Desc = "Expenses";
    siHotService.ArticleNo            = TDeviceRealTerminal::Instance().BasePMS->ExpensesAccount;
    siHotService.ArticleNo_Desc       = "Expenses";
    double pricePerUnit               = 0.0;
    pricePerUnit                      = fabs((double)payment->GetCashOut());
    pricePerUnit                      = RoundTo(pricePerUnit,-2);
    siHotService.PricePerUnit         = pricePerUnit;//fabs((double)payment->GetCashOut());
    siHotService.Amount               = 1;
    double priceTotal                 = 0.0;
    priceTotal                        = fabs((double)payment->GetCashOut());
    priceTotal                        = RoundTo(priceTotal,-2);
    siHotService.PriceTotal           = priceTotal;//fabs((double)payment->GetCashOut());
    siHotService.VATPercentage        = 0;
    siHotService.Billno               = billno;
    siHotService.Cashno               = TDeviceRealTerminal::Instance().BasePMS->POSID;
    siHotService.Cashier              = TDeviceRealTerminal::Instance().User.Name;
    siHotService.Source               = "Guest";
    _roomCharge.SiHotServices.push_back(siHotService);
}
//----------------------------------------------------------------------------
void TSiHotDataProcessor::AddDiscountPartToService(TItemComplete *itemComplete,TRoomCharge &_roomCharge, TPaymentTransaction &_paymentTransaction
                                                   ,UnicodeString _billNo)
{
    TSiHotService siHotService;
    UnicodeString categoryCode = "";
    if(TGlobalSettings::Instance().SendNoTaxToSiHot)
    {
       categoryCode = TGlobalSettings::Instance().RevenueCodeDiscountPart;
    }
    else
    {
       categoryCode = itemComplete->RevenueCode;
    }

    if(categoryCode == 0 || categoryCode == "")
    {
        for(std::map<int,TRevenueCodeDetails>::iterator itRev = TDeviceRealTerminal::Instance().BasePMS->RevenueCodesMap.begin();
            itRev!= TDeviceRealTerminal::Instance().BasePMS->RevenueCodesMap.end(); advance(itRev,1))
        {
            if(itRev->second.IsDefault)
            {
               categoryCode = itRev->first;
               break;
            }
        }
    }

    std::map<UnicodeString,TSiHotService>::iterator it;

    it = serviceSiHotDiscPart.find(categoryCode);
    if(it == serviceSiHotDiscPart.end())
    {
        // if value not found
        siHotService.SuperCategory         = categoryCode;
        siHotService.SuperCategory_Desc    = "";
        siHotService.MiddleCategory        = categoryCode;
//        MessageBox(siHotService.MiddleCategory,"AddDiscountPartToService3",MB_OK);
        siHotService.MiddleCategory_Desc   = "Discount";
        if(TGlobalSettings::Instance().EnableItemDetailsPosting)
        {
            int alphaNumericCheck = 0;
            if(itemComplete->ItemSizeIdentifierKey > 0 && itemComplete->ItemSizeIdentifierKey <= 999 && itemComplete->ItemSizeIdentifierKey != NULL  && TryStrToInt(itemComplete->ItemSizeIdentifierKey,alphaNumericCheck))
            {
                siHotService.ArticleCategory      = IntToStr(itemComplete->ItemSizeIdentifierKey);
            }
            else
            {
                siHotService.ArticleCategory      = "999";
            }
            siHotService.ArticleCategory_Desc = itemComplete->Item + " (" + itemComplete->Size + ")";
        }
        else
        {
            siHotService.ArticleCategory      = categoryCode;
            siHotService.ArticleCategory_Desc = "";
        }
        siHotService.ArticleNo             = categoryCode;
        siHotService.ArticleNo_Desc        = "";
        double pricePerUnit                = fabs(itemComplete->BillCalcResult.TotalDiscount);
        pricePerUnit                       = RoundTo(pricePerUnit,-2);
        siHotService.PricePerUnit          = pricePerUnit;//fabs(itemComplete->BillCalcResult.TotalDiscount);
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
        double priceTotal                  =  fabs(itemComplete->BillCalcResult.TotalDiscount);
        priceTotal = RoundTo(priceTotal,-2);
        siHotService.PriceTotal            = priceTotal;//fabs(itemComplete->BillCalcResult.TotalDiscount);
        siHotService.VATPercentage         = 0;
        siHotService.Billno                = _billNo;
        siHotService.Cashno                = TDeviceRealTerminal::Instance().BasePMS->POSID;
        siHotService.Cashier               = TDeviceRealTerminal::Instance().User.Name;
        siHotService.Source                = "Guest";
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
    tipAmount        = RoundTo(tipAmount,-2);
    surcharge        = RoundTo(surcharge,-2);
    // Add Tip as service to Sihot
    if(tipAmount != 0)
    {
        TSiHotService siHotService;
        siHotService.SuperCategory          = TDeviceRealTerminal::Instance().BasePMS->TipAccount;
        siHotService.SuperCategory_Desc     = "Tip";
        siHotService.MiddleCategory         = TDeviceRealTerminal::Instance().BasePMS->TipAccount;
        siHotService.MiddleCategory_Desc    = "Tip";
        siHotService.ArticleCategory        = TDeviceRealTerminal::Instance().BasePMS->TipAccount;
        siHotService.ArticleCategory_Desc   = "Tip";
        siHotService.ArticleNo              = TDeviceRealTerminal::Instance().BasePMS->TipAccount;
        siHotService.ArticleNo_Desc         = "Tip";
        siHotService.PricePerUnit           = fabs((double)(tipAmount));
        siHotService.Amount                 = tipAmount < 0 ? "-1" : "1";
        siHotService.PriceTotal             = fabs((double)(tipAmount));
        siHotService.VATPercentage          = 0;
        siHotService.Billno                 = _billNo;
        siHotService.Cashno                 = TDeviceRealTerminal::Instance().BasePMS->POSID;
        siHotService.Cashier                = TDeviceRealTerminal::Instance().User.Name;
        siHotService.Source                 = "Guest";
        _roomCharge.SiHotServices.push_back(siHotService);
    }

    // Add surcharge as service to SiHot
    if(surcharge)
    {
        TSiHotService siHotServiceSurcharge;
        siHotServiceSurcharge.SuperCategory         = TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount;
        siHotServiceSurcharge.SuperCategory_Desc    = "SurCharge";
        siHotServiceSurcharge.MiddleCategory        = TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount;
        siHotServiceSurcharge.MiddleCategory_Desc   = "Surcharge";
        siHotServiceSurcharge.ArticleCategory       = TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount;
        siHotServiceSurcharge.ArticleCategory_Desc  = "Surcharge";
        siHotServiceSurcharge.ArticleNo             = TDeviceRealTerminal::Instance().BasePMS->DefaultSurchargeAccount;
        siHotServiceSurcharge.ArticleNo_Desc        = "Surcharge";
        siHotServiceSurcharge.PricePerUnit          = fabs(surcharge);
        siHotServiceSurcharge.Amount                = surcharge < 0 ? "-1" : "1";
        siHotServiceSurcharge.PriceTotal            = fabs(surcharge);
        siHotServiceSurcharge.VATPercentage         = 0;
        siHotServiceSurcharge.Billno                = _billNo;
        siHotServiceSurcharge.Cashno                = TDeviceRealTerminal::Instance().BasePMS->POSID;
        siHotServiceSurcharge.Cashier               = TDeviceRealTerminal::Instance().User.Name;
        siHotServiceSurcharge.Source                = "Guest";
        _roomCharge.SiHotServices.push_back(siHotServiceSurcharge);
    }
}
//----------------------------------------------------------------------------
double TSiHotDataProcessor::GetVATpercentage(TItemComplete *itemComplete)
{
    double taxPercentage = 0.0;
    int taxIndex = 0;
    if(itemComplete->BillCalcResult.FinalPrice == 0)
    {
        for(int taxIndex = 0; taxIndex < itemComplete->TaxProfiles.size(); taxIndex++)
        {
             TaxProfile tp = itemComplete->TaxProfiles[taxIndex];
             if(itemComplete->RemovedTaxes->IndexOf(tp.taxProfileName) != 0)
             {
                 taxPercentage += (double)tp.taxPercentage;
             }
        }
    }
    else
    {
        for(std::vector<BillCalculator::TTaxResult>::iterator tax = itemComplete->BillCalcResult.Tax.begin();
              tax != itemComplete->BillCalcResult.Tax.end() ; ++tax)
        {
            if(tax->Value != 0)
                taxPercentage += (double)tax->Percentage;
        }
        if(itemComplete->BillCalcResult.ServiceCharge.Value != 0.0)
        {
            for(std::vector<TaxProfile>::iterator serviceCharge = itemComplete->TaxProfiles.begin();
            serviceCharge != itemComplete->TaxProfiles.end(); ++serviceCharge)
            {
                taxIndex = 0;
                if(((serviceCharge->taxProfileType == ServiceCharge))
                   && (!itemComplete->RemovedTaxes->Find(serviceCharge->taxProfileName,taxIndex)))
                {
                    taxPercentage += (double)serviceCharge->taxPercentage;
                }
            }
        }
    }
    taxPercentage = RoundTo(taxPercentage,-2);
    return taxPercentage;
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
        bool IsCompDiscountApplied      = false;
        bool IsNCDiscountApplied        = false;
        bool IsNormalDiscountApplied    = false;
        for(int index = 0; index < _paymentTransaction.Orders->Count; index++)
        {
            TItemComplete *itemcomplete = (TItemComplete*)_paymentTransaction.Orders->Items[index];
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
            siHotAccount.AccountNumber      = i->AccountNumber;
            siHotAccountDetail.RoomNumber   = i->RoomNumber;
            siHotAccountDetail.Balance      = 0;
            siHotAccountDetail.CreditLimit  = i->Limit;
            siHotAccountDetail.FirstName    = i->FirstName;
            siHotAccountDetail.LastName     = i->LastName;
            siHotAccountDetail.RoomBedNumber    = i->RoomBedNumber;
            siHotAccount.AccountDetails.push_back(siHotAccountDetail);
            siHotAccounts.push_back(siHotAccount);
        }
    }
}
//----------------------------------------------------------------------------
void TSiHotDataProcessor::AddServiceChargeAsService(TRoomCharge &_roomCharge, UnicodeString billNo, TPaymentTransaction &_paymentTransaction)
{
    TSiHotService siHotService;
    siHotService.SuperCategory          = TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount;
    siHotService.SuperCategory_Desc     = "Service Charge";
    siHotService.MiddleCategory         = TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount;
    siHotService.MiddleCategory_Desc    = "Service Charge";
    siHotService.ArticleCategory        = TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount;
    siHotService.ArticleCategory_Desc   = "Service Charge";
    siHotService.ArticleNo              = TDeviceRealTerminal::Instance().BasePMS->ServiceChargeAccount;
    siHotService.ArticleNo_Desc         = "Service Charge";
    double pricePerUnit                 = fabs((double)(_paymentTransaction.Money.ServiceCharge+_paymentTransaction.Money.ServiceChargeTax));
    pricePerUnit = RoundTo(pricePerUnit,-2);
    siHotService.PricePerUnit           = pricePerUnit;//fabs((double)(_paymentTransaction.Money.ServiceCharge+_paymentTransaction.Money.ServiceChargeTax));
    siHotService.Amount                 = _paymentTransaction.Money.ServiceCharge < 0 ? "-1" : "1";
    double priceTotal                   = fabs((double)(_paymentTransaction.Money.ServiceCharge+_paymentTransaction.Money.ServiceChargeTax));
    priceTotal                          = RoundTo(priceTotal,-2);
    siHotService.PriceTotal             = priceTotal;//fabs((double)(_paymentTransaction.Money.ServiceCharge+_paymentTransaction.Money.ServiceChargeTax));
    if(TGlobalSettings::Instance().ApplyServiceChargeTax)
        siHotService.VATPercentage      = TGlobalSettings::Instance().ServiceChargeTaxRate;
    else
        siHotService.VATPercentage      = 0;
    siHotService.Billno                 = billNo;
    siHotService.Cashno                 = TDeviceRealTerminal::Instance().BasePMS->POSID;
    siHotService.Cashier                = TDeviceRealTerminal::Instance().User.Name;
    siHotService.Source                 = "Guest";
    _roomCharge.SiHotServices.push_back(siHotService);
}
//----------------------------------------------------------------------------
void TSiHotDataProcessor::AddRoundingAsService(TRoomCharge &_roomCharge, UnicodeString billNo, TPaymentTransaction &_paymentTransaction)
{
    ReadCurrentRoundingSettingForVat();
    TSiHotService siHotService;
    siHotService.SuperCategory          = TDeviceRealTerminal::Instance().BasePMS->RoundingCategory;
    siHotService.SuperCategory_Desc     = "";
    siHotService.MiddleCategory         = TDeviceRealTerminal::Instance().BasePMS->RoundingCategory;
    siHotService.MiddleCategory_Desc    = "Rounding Differences";
    siHotService.ArticleCategory        = TDeviceRealTerminal::Instance().BasePMS->RoundingCategory;
    siHotService.ArticleCategory_Desc   = "";
    siHotService.ArticleNo              = TDeviceRealTerminal::Instance().BasePMS->RoundingCategory;
    siHotService.ArticleNo_Desc         = "";
    double pricePerUnit                 = fabs((double)(_paymentTransaction.Money.TotalRounding));
    pricePerUnit                        = RoundTo(pricePerUnit,-2);
    siHotService.PricePerUnit           = pricePerUnit;//fabs((double)(_paymentTransaction.Money.PaymentRounding));
    siHotService.Amount                 = _paymentTransaction.Money.TotalRounding < 0 ? "-1" : "1";
    siHotService.PriceTotal             = pricePerUnit;
    if(TGlobalSettings::Instance().ApplyRoundingTax)
    {
        double vatPercentage            = RoundTo(TGlobalSettings::Instance().RoundingTaxRate,-2);
        siHotService.VATPercentage = vatPercentage;
    }
    else
        siHotService.VATPercentage      = 0;
    siHotService.Billno                 = billNo;
    siHotService.Cashno                 = TDeviceRealTerminal::Instance().BasePMS->POSID;
    siHotService.Cashier                = TDeviceRealTerminal::Instance().User.Name;
    siHotService.Source                 = "Guest";
    _roomCharge.SiHotServices.push_back(siHotService);
}
//----------------------------------------------------------------------------
void TSiHotDataProcessor::ReadCurrentRoundingSettingForVat()
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        int GlobalProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction, eSystemProfiles, "Globals");
        if(GlobalProfileKey != 0)
        {

            TIBSQL *IBInternalQuery= DBTransaction.Query(DBTransaction.AddQuery());
            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text =
            	"SELECT "
                "NUMERIC_VAL "
                "FROM "
                "VARSPROFILE "
                "WHERE "
                "VARIABLES_KEY = :VARIABLES_KEY "
                "AND PROFILE_KEY = :PROFILE_KEY ";

            IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = vmRoundingTaxRate;
            IBInternalQuery->ParamByName("PROFILE_KEY")->AsInteger = GlobalProfileKey;
            IBInternalQuery->ExecQuery();
            TGlobalSettings::Instance().RoundingTaxRate = IBInternalQuery->FieldByName("NUMERIC_VAL")->AsFloat;

            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text =
            	"SELECT "
                "INTEGER_VAL "
                "FROM "
                "VARSPROFILE "
                "WHERE "
                "VARIABLES_KEY = :VARIABLES_KEY "
                "AND PROFILE_KEY = :PROFILE_KEY ";
            IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = vmApplyRoundingTax;
            IBInternalQuery->ParamByName("PROFILE_KEY")->AsInteger = GlobalProfileKey;
            IBInternalQuery->ExecQuery();
            TGlobalSettings::Instance().ApplyRoundingTax = IBInternalQuery->FieldByName("INTEGER_VAL")->AsInteger;
        }
        DBTransaction.Commit();
    }
    catch(Exception &Exc)
    {
        DBTransaction.Rollback();
    }
}
//----------------------------------------------------------------------------
void TSiHotDataProcessor::GetPMSPaymentType(std::map<int,TPMSPaymentType> &paymentMap)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    paymentMap.clear();
    try
    {
        TIBSQL *SelectQuery= DBTransaction.Query(DBTransaction.AddQuery());
        SelectQuery->Close();
        SelectQuery->SQL->Text = "SELECT * FROM PMSPAYMENTSCONFIG";
        SelectQuery->ExecQuery();
        for(;!SelectQuery->Eof;SelectQuery->Next())
        {
           TPMSPaymentType pmsPayment;
           pmsPayment.PMSPayTypeID                  = SelectQuery->FieldByName("PMS_PAYTYPE_ID")->AsInteger;
           pmsPayment.PMSPayTypeName                = SelectQuery->FieldByName("PMS_PAYTYPE_NAME")->AsString;
           pmsPayment.PMSPayTypeCode                = SelectQuery->FieldByName("PMS_PAYTYPE_CODE")->AsString;
           pmsPayment.PMSPayTypeCategory            = SelectQuery->FieldByName("PMS_PAYTYPE_CATEGORY")->AsInteger;
           pmsPayment.PMSMMPayTypeLink              = SelectQuery->FieldByName("PMS_MM_PAYTYPELINK")->AsInteger == NULL ? 0 : SelectQuery->FieldByName("PMS_MM_PAYTYPELINK")->AsInteger;
           pmsPayment.isElectronicPayment           = SelectQuery->FieldByName("IS_ELECTRONICPAYMENT")->AsString == "T" ? true : false;
           paymentMap[pmsPayment.PMSPayTypeID]
                                                    = pmsPayment;
        }
        DBTransaction.Commit();
    }
    catch(Exception &Exc)
    {
        DBTransaction.Rollback();
    }
}
//----------------------------------------------------------------------------
AnsiString TSiHotDataProcessor::GetPMSPaymentCode(TPayment *payment,std::map<int,TPMSPaymentType> paymentsMap)
{
    AnsiString value = "";
    std::map<int,TPMSPaymentType>::iterator it = paymentsMap.begin();

    for(;it != paymentsMap.end(); ++it)
    {
        if(!payment->GetPaymentAttribute(ePayTypeIntegratedEFTPOS))
        {
            if(it->second.PMSPayTypeName == payment->Name)
            {
                value = it->second.PMSPayTypeCode;
                break;
            }
        }
        else
        {
            if(payment->CardType != "")
            {
                if(it->second.PMSPayTypeName == payment->CardType)
                {
                    value = it->second.PMSPayTypeCode;
                    break;
                }
            }
            else
            {
                if(it->second.PMSPayTypeName == payment->Name)
                {
                    value = it->second.PMSPayTypeCode;
                    break;
                }
            }
        }
    }
    return value;
}
//----------------------------------------------------------------------------
AnsiString TSiHotDataProcessor::GetPMSDefaultCode(std::map<int,TPMSPaymentType> paymentsMap)
{
    AnsiString value = "";
    std::map<int,TPMSPaymentType>::iterator it = paymentsMap.begin();

    for(;it != paymentsMap.end(); ++it)
    {
        if(it->second.PMSPayTypeCategory == eDefaultCategory)
        {
            value = it->second.PMSPayTypeCode;
            break;
        }
    }
    return value;
}
//----------------------------------------------------------------------------
void TSiHotDataProcessor::AddPaymentMethods(TRoomCharge &_roomCharge, UnicodeString billNo, TPaymentTransaction &_paymentTransaction)
{
    double surcharge = 0.0;
    UnicodeString indexcash = "";
    double cashOutStore = 0.0;
    double tip = 0.0;
    UnicodeString cashType = "";
    std::map<int,TPMSPaymentType> paymentsMap;
    GetPMSPaymentType(paymentsMap);
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
            cashType = GetPMSPaymentCode(payment,paymentsMap);//payment->PaymentThirdPartyID;
        }

        if(((payment->GetPayTendered() != 0) || (payment->GetCashOut() != 0.0)) && !(payment->GetPaymentAttribute(ePayTypeRoomInterface)))
        {
            tip += (double)payment->TipAmount;
            siHotPayment.Type       = GetPMSPaymentCode(payment,paymentsMap);//payment->PaymentThirdPartyID;
            if(siHotPayment.Type == "" && payment->GetPaymentAttribute(ePayTypeIntegratedEFTPOS) && payment->CardType != "")
            {
               siHotPayment.Type = DoRequiredInsertion(payment, paymentsMap);
            }
            if(siHotPayment.Type    == "")
            {
                siHotPayment.Type   = GetPMSDefaultCode(paymentsMap);//TDeviceRealTerminal::Instance().BasePMS->DefaultPaymentCategory;
            }
            if(payment->GetPaymentAttribute(ePayTypePoints))
               siHotPayment.Type    = TDeviceRealTerminal::Instance().BasePMS->PointsCategory;
            if(payment->GetPaymentAttribute(ePayTypeCredit) && (double)payment->GetPayTendered() < 0.0)
               continue;
            if(payment->GetPaymentAttribute(ePayTypeCredit))
               siHotPayment.Type    = TDeviceRealTerminal::Instance().BasePMS->CreditCategory;
            std::map<UnicodeString, TSiHotPayments>::iterator iter;
            iter                    = paymentSiHot.find(siHotPayment.Type);
            if(iter == paymentSiHot.end())
            {
                double amountV              = ((double)(payment->GetPayTendered() - payment->GetChange()));
                amountV                     = RoundTo(amountV,-2);
                siHotPayment.Amount         = amountV;//(payment->GetPayTendered() + payment->GetCashOut() - payment->GetChange());
                siHotPayment.Description    = payment->Name;
                siHotPayment.Billno         = billNo;
                siHotPayment.Cashno         = TDeviceRealTerminal::Instance().BasePMS->POSID;
                siHotPayment.Cashier        = TDeviceRealTerminal::Instance().User.Name;
                siHotPayment.Source         = "Guest";
                paymentSiHot[siHotPayment.Type] = siHotPayment;
            }
            else
            {
                double amount               = StrToCurr(iter->second.Amount) + ((payment->GetPayTendered() - payment->GetChange()));
                iter->second.Amount         = amount;
            }
            if(payment->GetPaymentAttribute(ePayTypeCash))
            {
                indexcash                   = "Cash";
            }
            cashOutStore                    += (double)payment->GetCashOut();
        }
//        if(i + 1 == _paymentTransaction.PaymentsCount())
//        {
//            if(indexcash == "" && cashOutStore != 0.0)
//            {
//                TSiHotPayments siHotPaymentCash;
////                if(cashType == NULL || cashType == "")
////                {
////                    cashType = GetPMSDefaultCode(paymentsMap);
////                }
//                siHotPaymentCash.Type           = cashType;
//                double amountValue              = RoundTo((double)cashOutStore,-2);
//                siHotPaymentCash.Amount         = amountValue;//RoundTo((double)cashOutStore,-2);
//                siHotPaymentCash.Description    = "Cash";
//                siHotPaymentCash.Billno         = billNo;
//                siHotPaymentCash.Cashno         = TDeviceRealTerminal::Instance().BasePMS->POSID;
//                siHotPaymentCash.Cashier        = TDeviceRealTerminal::Instance().User.Name;
//                siHotPaymentCash.Source         = "Guest";
//                paymentSiHot[cashType]          = siHotPaymentCash;
//            }
//        }
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
    if(_paymentTransaction.Money.TotalRounding != 0)
    {
        AddRoundingAsService(_roomCharge, billNo, _paymentTransaction);
    }
}
//----------------------------------------------------------------------------
bool TSiHotDataProcessor::GetDefaultAccount(AnsiString tcpIPAddress,AnsiString tcpPort, AnsiString apiKey)
{
    bool retValue = false;
    std::auto_ptr<TSiHotInterface> siHotInterface (new TSiHotInterface());
    TRoomRequest roomRequest;
    roomRequest.IPAddress = tcpIPAddress;
    roomRequest.PortNumber = atoi(tcpPort.c_str());
    roomRequest.TransactionNumber = GetTransNumber();
    roomRequest.RoomNumber = TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount;
    TRoomResponse roomresponse;
    roomresponse = siHotInterface->SendRoomRequest(roomRequest,TGlobalSettings::Instance().PMSTimeOut,apiKey) ;
    if(roomresponse.GuestsInformation.size() != 0)
    {
        if(roomresponse.GuestsInformation[0].AccountNumber != "")
        {
            Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
            DBTransaction.StartTransaction();
            try
            {
                TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmSiHotDefaultTransaction,roomresponse.GuestsInformation[0].AccountNumber);
                TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmSiHotDefaultTransactionName,roomresponse.GuestsInformation[0].FirstName + " " +
                                                          roomresponse.GuestsInformation[0].LastName);

                DBTransaction.Commit();
                DBTransaction.Start();
                TDeviceRealTerminal::Instance().BasePMS->DefaultAccountNumber =
                               TManagerVariable::Instance().GetStr(DBTransaction,vmSiHotDefaultTransaction);
                TDeviceRealTerminal::Instance().BasePMS->Enabled = true;
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
bool TSiHotDataProcessor::GetRoundingAccounting(AnsiString tcpIPAddress,AnsiString tcpPort,AnsiString apiKey)
{
    bool retValue = false;
    std::auto_ptr<TSiHotInterface> siHotInterface (new TSiHotInterface());
    TRoomRequest roomRequest;
    roomRequest.IPAddress = tcpIPAddress;
    roomRequest.PortNumber = atoi(tcpPort.c_str());
    roomRequest.TransactionNumber = GetTransNumber();
    roomRequest.RoomNumber = TDeviceRealTerminal::Instance().BasePMS->RoundingAccountSiHot;
    TRoomResponse roomresponse;
    roomresponse = siHotInterface->SendRoomRequest(roomRequest,TGlobalSettings::Instance().PMSTimeOut,apiKey);
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
AnsiString TSiHotDataProcessor::DoRequiredInsertion(TPayment *payment, std::map<int,TPMSPaymentType> &paymentsMap)
{
   AnsiString retValue = "";
   try
   {
       AnsiString defaultPayCode = GetPMSDefaultCode(paymentsMap);
       AddPaymentToPMSPaymentTypes(payment,defaultPayCode);
       GetPMSPaymentType(paymentsMap);
       retValue = GetPMSPaymentCode(payment,paymentsMap);
   }
   catch(Exception &Exc)
   {
       TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
   }
   return retValue;
}
//----------------------------------------------------------------------------
void TSiHotDataProcessor::AddPaymentToPMSPaymentTypes(TPayment *payment,AnsiString defaultCode)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *InsertQuery= DBTransaction.Query(DBTransaction.AddQuery());
        InsertQuery->Close();
        InsertQuery->SQL->Text = " INSERT INTO PMSPAYMENTSCONFIG "
                 " (PMS_PAYTYPE_ID, PMS_PAYTYPE_NAME, PMS_PAYTYPE_CODE,"
                 " PMS_PAYTYPE_CATEGORY ,IS_ELECTRONICPAYMENT) VALUES"
                 " (:PMS_PAYTYPE_ID, :PMS_PAYTYPE_NAME, :PMS_PAYTYPE_CODE,"
                 " :PMS_PAYTYPE_CATEGORY ,:IS_ELECTRONICPAYMENT)";
        InsertQuery->ParamByName("PMS_PAYTYPE_ID")->AsInteger =
                                 TGeneratorManager::GetNextGeneratorKey(DBTransaction,"GEN_PMSPAYTYPEID");
        InsertQuery->ParamByName("PMS_PAYTYPE_NAME")->AsString = payment->CardType;
        InsertQuery->ParamByName("PMS_PAYTYPE_CODE")->AsString = defaultCode;
        InsertQuery->ParamByName("PMS_PAYTYPE_CATEGORY")->AsInteger = 2;
        InsertQuery->ParamByName("IS_ELECTRONICPAYMENT")->AsString = "T";
        InsertQuery->ExecQuery();
        DBTransaction.Commit();
    }
    catch(Exception &Exc)
    {
        DBTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
    }
}
//----------------------------------------------------------------------------
void TSiHotDataProcessor::CreateStoreTicketPost(UnicodeString invoiceNumber, TStoreTicket &_storeTicket, AnsiString receiptData)
{
    try
    {
    _storeTicket.TransNo       =     GetTransNumber();
    _storeTicket.StoreTicket   =     "";
    _storeTicket.Billno        =     invoiceNumber;
    _storeTicket.Cashno        =     TDeviceRealTerminal::Instance().BasePMS->POSID;
    _storeTicket.Signature     =     "";
    _storeTicket.Type          =     "TXT";
    _storeTicket.Document      =     receiptData;
    _storeTicket.IPAddress     =     TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress;
    _storeTicket.PortNumber    =     TDeviceRealTerminal::Instance().BasePMS->TCPPort;
    }
    catch(Exception &Exc)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
    }
}
//----------------------------------------------------------------------------

