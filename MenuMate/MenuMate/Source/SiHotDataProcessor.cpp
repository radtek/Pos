//---------------------------------------------------------------------------


#pragma hdrstop

#include "SiHotDataProcessor.h"
#include "Math.h"

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
void TSiHotDataProcessor::CreateRoomChargePost(TPaymentTransaction _paymentTransaction, TRoomCharge &_roomCharge)
{
    bool AddDiscountPart = false;
    double discountValue = 0.0;
    _roomCharge.TransactionNumber = GetTransNumber();
    _roomCharge.AccountNumber = _paymentTransaction.Phoenix.AccountNumber;
    UnicodeString billNo = GetInvoiceNumber(_paymentTransaction);
    for(int i = 0; i < _paymentTransaction.Orders->Count; i++)
    {
        AddDiscountPart = false;
        discountValue = 0.0;
        TSiHotService siHotService;
        TItemComplete *itemComplete = ((TItemComplete*)_paymentTransaction.Orders->Items[i]);
        if(itemComplete->OrderType ==  CanceledOrder)
            continue;
        siHotService.SuperCategory = itemComplete->ItemType == eFoodItem ? "1" : "2";
        siHotService.SuperCategory_Desc = itemComplete->ItemType == eFoodItem ? "Food" : "Drinks";
        UnicodeString glCode = GetGLCode(TDBOrder::GetArchiveCategory(_paymentTransaction.DBTransaction, itemComplete->Categories->FinancialCategory));
        siHotService.MiddleCategory = glCode;
        siHotService.MiddleCategory_Desc = itemComplete->Categories->FinancialCategory;
        siHotService.ArticleCategory = glCode;
        siHotService.ArticleCategory_Desc = itemComplete->Categories->FinancialCategory;
        siHotService.ArticleNo = itemComplete->ThirdPartyCode;
        siHotService.ArticleNo_Desc = itemComplete->Item;
        if(TGlobalSettings::Instance().Instance().ReCalculateTaxPostDiscount)
        {
            siHotService.PricePerUnit = fabs((double)itemComplete->BillCalcResult.FinalPrice/itemComplete->GetQty() -
                                                 ((double)itemComplete->BillCalcResult.ServiceCharge.Value/itemComplete->GetQty()));
            siHotService.Amount = (double)itemComplete->GetQty();
            siHotService.PriceTotal = fabs((double)itemComplete->BillCalcResult.FinalPrice -
                                                    (double)itemComplete->BillCalcResult.ServiceCharge.Value);
        }
        else
        {
            siHotService.PricePerUnit = fabs((double)itemComplete->BillCalcResult.FinalPrice/itemComplete->GetQty() -
                                                 ((double)itemComplete->BillCalcResult.ServiceCharge.Value/itemComplete->GetQty())
                                                 -((double)itemComplete->BillCalcResult.TotalDiscount/itemComplete->GetQty()));
            siHotService.Amount = (double)itemComplete->GetQty();
            siHotService.PriceTotal = fabs((double)itemComplete->BillCalcResult.FinalPrice -
                                                    (double)itemComplete->BillCalcResult.ServiceCharge.Value
                                                    -((double)itemComplete->BillCalcResult.TotalDiscount));
            AddDiscountPart = (double)itemComplete->BillCalcResult.TotalDiscount != 0.0 ? true : false;
            discountValue = (double)itemComplete->BillCalcResult.TotalDiscount;
        }
        siHotService.VATPercentage = fabs(GetVATpercentage(itemComplete));
        siHotService.Billno = billNo;
        siHotService.Cashno = TDeviceRealTerminal::Instance().BasePMS->POSID;
        siHotService.Cashier = TDeviceRealTerminal::Instance().User.Name;
        siHotService.Source = "Guest";
        _roomCharge.SiHotServices.push_back(siHotService);
        if(AddDiscountPart)
            AddDiscountPartToService(itemComplete, _roomCharge, _paymentTransaction, billNo);
        AddDiscountPart = false;
        discountValue = 0.0;
        for(int j = 0; j < itemComplete->SubOrders->Count; j++)
        {
            TSiHotService siHotService;
            TItemComplete *subItemComplete = ((TItemComplete*)_paymentTransaction.Orders->Items[j]);
            siHotService.SuperCategory = subItemComplete->ItemType == eFoodItem ? "1" : "2";
            siHotService.SuperCategory_Desc = subItemComplete->ItemType == eFoodItem ? "Food" : "Drinks";
            UnicodeString glCode = GetGLCode(TDBOrder::GetArchiveCategory(_paymentTransaction.DBTransaction, subItemComplete->Categories->FinancialCategory));
            siHotService.MiddleCategory = glCode;
            siHotService.MiddleCategory_Desc = subItemComplete->Categories->FinancialCategory;
            siHotService.ArticleCategory = glCode;
            siHotService.ArticleCategory_Desc = subItemComplete->Categories->FinancialCategory;
            siHotService.ArticleNo = subItemComplete->ThirdPartyCode;
            siHotService.ArticleNo_Desc = subItemComplete->Item;
            if(TGlobalSettings::Instance().Instance().ReCalculateTaxPostDiscount)
            {
                siHotService.PricePerUnit = fabs((double)subItemComplete->BillCalcResult.FinalPrice/subItemComplete->GetQty()-
                                                 ((double)subItemComplete->BillCalcResult.ServiceCharge.Value/subItemComplete->GetQty()));
                siHotService.Amount = (double)subItemComplete->GetQty();
                siHotService.PriceTotal = fabs((double)subItemComplete->BillCalcResult.FinalPrice -
                                                        (double)subItemComplete->BillCalcResult.ServiceCharge.Value);
            }
            else
            {
                siHotService.PricePerUnit = fabs((double)subItemComplete->BillCalcResult.FinalPrice/subItemComplete->GetQty()-
                                                 ((double)subItemComplete->BillCalcResult.ServiceCharge.Value/subItemComplete->GetQty())
                                                 -((double)subItemComplete->BillCalcResult.TotalDiscount/subItemComplete->GetQty()));
                siHotService.Amount = (double)subItemComplete->GetQty();
                siHotService.PriceTotal = fabs((double)subItemComplete->BillCalcResult.FinalPrice -
                                                        (double)subItemComplete->BillCalcResult.ServiceCharge.Value
                                                        -((double)subItemComplete->BillCalcResult.TotalDiscount));
                AddDiscountPart = (double)subItemComplete->BillCalcResult.TotalDiscount != 0.0 ? true : false;
                discountValue = (double)subItemComplete->BillCalcResult.TotalDiscount;
            }
            siHotService.VATPercentage = fabs(GetVATpercentage(subItemComplete));
            siHotService.Billno = GetInvoiceNumber(_paymentTransaction);
            siHotService.Cashno = TDeviceRealTerminal::Instance().BasePMS->POSID;
            siHotService.Cashier = TDeviceRealTerminal::Instance().User.Name;
            siHotService.Source = "Guest";
            _roomCharge.SiHotServices.push_back(siHotService);
            if(AddDiscountPart)
                AddDiscountPartToService(subItemComplete, _roomCharge, _paymentTransaction, billNo);
            AddDiscountPart = false;
            discountValue = 0.0;
        }
    }

    // Add ServiceCharge as service to SiHot
    if(_paymentTransaction.Money.ServiceCharge != 0)
    {
        TSiHotService siHotService;
        siHotService.SuperCategory = TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount;
        siHotService.SuperCategory_Desc = "Service Charge";
        siHotService.MiddleCategory = TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount;
        siHotService.MiddleCategory_Desc = "Service Charge";
        siHotService.ArticleCategory = TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount;
        siHotService.ArticleCategory_Desc = "Service Charge";
        siHotService.ArticleNo = TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount;
        siHotService.ArticleNo_Desc = "Service Charge";
        siHotService.PricePerUnit = fabs((double)(_paymentTransaction.Money.ServiceCharge+_paymentTransaction.Money.ServiceChargeTax));
        siHotService.Amount = _paymentTransaction.Money.ServiceCharge < 0 ? "-1" : "1";
        siHotService.PriceTotal = fabs((double)(_paymentTransaction.Money.ServiceCharge+_paymentTransaction.Money.ServiceChargeTax));
        siHotService.VATPercentage = TGlobalSettings::Instance().ServiceChargeTaxRate;
        siHotService.Billno = billNo;
        siHotService.Cashno = TDeviceRealTerminal::Instance().BasePMS->POSID;
        siHotService.Cashier = TDeviceRealTerminal::Instance().User.Name;
        siHotService.Source = "Guest";
        _roomCharge.SiHotServices.push_back(siHotService);
    }
    // Add Rounding as service to SiHot
    if(RoundTo((double)(_paymentTransaction.Money.PaymentRounding),-2) != 0.00)
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
    // Adding payment types
    double surcharge = 0.0;
    int indexcash = 0;
    double cashOutStore = 0.0;
    for(int i = 0; i < _paymentTransaction.PaymentsCount(); i++)
    {
        TSiHotPayments siHotPayment;
        TPayment *payment = _paymentTransaction.PaymentGet(i);
        if(payment->GetPayTendered() != 0)
        {
            surcharge += (double)payment->GetSurchargeTotal();
        }
        if(((payment->GetPayTendered() != 0) || (payment->GetCashOut() != 0.0)) && !(payment->Properties & ePayTypeRoomInterface))
        {
            siHotPayment.Type = payment->PaymentThirdPartyID;
            if(payment->Properties & ePayTypePoints)
               siHotPayment.Type = TDeviceRealTerminal::Instance().BasePMS->PointsCategory;
            if(payment->Properties & ePayTypeCredit)
               siHotPayment.Type = TDeviceRealTerminal::Instance().BasePMS->CreditCategory;
            siHotPayment.Amount = -(payment->GetPayTendered() - payment->GetSurchargeTotal() - payment->TipAmount + payment->GetCashOut());
            siHotPayment.Description = payment->Name;
            siHotPayment.Billno = billNo;
            siHotPayment.Cashno = TDeviceRealTerminal::Instance().BasePMS->POSID;
            siHotPayment.Cashier = TDeviceRealTerminal::Instance().User.Name;
            siHotPayment.Source = "Guest";
            _roomCharge.SiHotPayments.push_back(siHotPayment);

            if(payment->Properties & ePayTypeCash)
                indexcash = i;
            cashOutStore += (double)payment->GetCashOut();
        }
        if(i + 1 == _paymentTransaction.PaymentsCount())
        {
            if(indexcash != 0 && cashOutStore != 0.0)
            {
                _roomCharge.SiHotPayments[indexcash].Amount += cashOutStore;
            }
            else if(indexcash == 0 && cashOutStore != 0.0)
            {
                TSiHotPayments siHotPaymentCash;
                for(int k = 0; k < _paymentTransaction.PaymentsCount(); k++)
                {
                    TPayment *payment1 = _paymentTransaction.PaymentGet(k);
                    if(!(payment1->Properties & ePayTypeCash))
                       continue;
                    siHotPaymentCash.Type = payment1->PaymentThirdPartyID;
                    siHotPaymentCash.Amount = cashOutStore;
                    siHotPaymentCash.Description = payment1->Name;
                    siHotPaymentCash.Billno = billNo;
                    siHotPaymentCash.Cashno = TDeviceRealTerminal::Instance().BasePMS->POSID;
                    siHotPaymentCash.Cashier = TDeviceRealTerminal::Instance().User.Name;
                    siHotPaymentCash.Source = "Guest";
                    _roomCharge.SiHotPayments.push_back(siHotPaymentCash);
                }
            }
        }
        if((payment->Properties & ePayTypeRoomInterface) && payment->GetCashOut() != 0)
        {
            AddExpensesToSiHotService(payment,_roomCharge,billNo);
        }
    }
    if(surcharge != 0)
    {
        AddSurchargeAndTip(_roomCharge,surcharge,_paymentTransaction,billNo);
    }
    _roomCharge.IPAddress = TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress;
    _roomCharge.PortNumber = TDeviceRealTerminal::Instance().BasePMS->TCPPort;
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
UnicodeString TSiHotDataProcessor::GetGLCode(int key)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *IBInternalQuery= DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text = "SELECT a.GL_CODE FROM ARCCATEGORIES a WHERE a.CATEGORY_KEY = :CATEGORY_KEY ";
    IBInternalQuery->ParamByName("CATEGORY_KEY")->AsInteger = key;
    IBInternalQuery->ExecQuery();
    UnicodeString glCode = IBInternalQuery->FieldByName("GL_CODE")->AsString;
    DBTransaction.Commit();
    return glCode;
}
//----------------------------------------------------------------------------
void TSiHotDataProcessor::AddDiscountPartToService(TItemComplete *itemComplete,TRoomCharge &_roomCharge, TPaymentTransaction _paymentTransaction
                                                   ,UnicodeString _billNo)
{
        TSiHotService siHotService;
        siHotService.SuperCategory = itemComplete->ItemType == eFoodItem ? "1" : "2";
        siHotService.SuperCategory_Desc = itemComplete->ItemType == eFoodItem ? "Food" : "Drinks";
        int key = TDBOrder::GetArchiveCategory(_paymentTransaction.DBTransaction, itemComplete->Categories->FinancialCategory);
        siHotService.MiddleCategory = key;
        siHotService.MiddleCategory_Desc = itemComplete->Categories->FinancialCategory;
        siHotService.ArticleCategory = key;
        siHotService.ArticleCategory_Desc = itemComplete->Categories->FinancialCategory;
        siHotService.ArticleNo = itemComplete->ThirdPartyCode;
        siHotService.ArticleNo_Desc = itemComplete->Item;
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
        _roomCharge.SiHotServices.push_back(siHotService);
}
//----------------------------------------------------------------------------
void TSiHotDataProcessor::AddSurchargeAndTip( TRoomCharge _roomCharge, double surcharge,
                                              TPaymentTransaction _paymentTransaction, UnicodeString _billNo)
{
    double tipAmount = (double)_paymentTransaction.Money.Surcharge - surcharge;
    // Add Tip as service to Sihot
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

    // Add surcharge as service to SiHot
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
//----------------------------------------------------------------------------
double TSiHotDataProcessor::GetVATpercentage(TItemComplete *itemComplete)
{
    double percentage = 0.0;
    for(std::vector<BillCalculator::TTaxResult>::iterator i = itemComplete->BillCalcResult.Tax.begin();
          i != itemComplete->BillCalcResult.Tax.end() ; ++i)
    {
        percentage = i->Percentage;
    }
    return percentage;
}
//----------------------------------------------------------------------------
UnicodeString TSiHotDataProcessor::GetInvoiceNumber(TPaymentTransaction _paymentTransaction)
{
    UnicodeString invoiceNumber = "";
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *IBInternalQueryGenerator= DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQueryGenerator->Close();
    switch(_paymentTransaction.TypeOfSale)
    {
       case 0:
       {
            try
            {
            IBInternalQueryGenerator->SQL->Text = "SELECT GEN_ID(GEN_INVOICENUMBER, 0) FROM RDB$DATABASE ";
            IBInternalQueryGenerator->ExecQuery();
            }
            catch(Exception &E)
            {
                TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
                throw;
            }
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
//----------------------------------------------------------------------------
int TSiHotDataProcessor::GetTransNumber()
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *IBInternalQueryGenerator= DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQueryGenerator->Close();
    IBInternalQueryGenerator->SQL->Text = "SELECT GEN_ID(GEN_SIHOTTRANSNUMBER, 1) FROM RDB$DATABASE ";
    IBInternalQueryGenerator->ExecQuery();
    int value = IBInternalQueryGenerator->Fields[0]->AsInteger;
    DBTransaction.Commit();
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
