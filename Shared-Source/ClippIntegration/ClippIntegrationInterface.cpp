//---------------------------------------------------------------------------

#pragma hdrstop

#include "DeviceRealTerminal.h"
#include "ClippIntegrationInterface.h"
#include "Modules.h"
#include "LogsDB.h"
#include "DBClippTab.h"

//---------------------------------------------------------------------------

TClippIntegrationInterface::TClippIntegrationInterface()
{
    //Initiate the class and its member variables..
    _useWsdl = false;

    InitializeWebClient();
}

//---------------------------------------------------------------------------

TClippIntegrationInterface::~TClippIntegrationInterface()
{
    //Remove all references..
}

void TClippIntegrationInterface::InitializeWebClient()
{
    bool useWSDL = false;
    AnsiString clippIntegrationServicePath = AnsiString("http://localhost:8736/MenumateServices/ClippService/");

    _clippIntegrationWebClient = GetIClippIntergrationWebService(_useWsdl, clippIntegrationServicePath, NULL);
}

//---------------------------------------------------------------------------

CLIPP_MESSAGE_MAP TClippIntegrationInterface::GetClippMessages(MMClippMessageVersionType clippMessageType)
{
    CLIPP_MESSAGE_LIST messages;
    CLIPP_MESSAGE_MAP clippMessageMap;

    try
    {
        CoInitialize(NULL);

        switch(clippMessageType)
        {
            case Previous:
                //Invoke the web service client with the required parameters..
                messages = _clippIntegrationWebClient->GetPreviousMessages();
                break;
            default:
                //Invoke the web service client with the required parameters..
                messages = _clippIntegrationWebClient->GetOpenMessages();
                break;
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }

    if(messages.Length <= 0)
        return clippMessageMap;

    clippMessageMap = ProcessMessagesIntoMap(messages);
    return clippMessageMap;
}

//---------------------------------------------------------------------------

void TClippIntegrationInterface::SendTabDetails(MMClippTabDetail mmClippTabDetail)
{
    ClippTabDetailRequest* clippTabDetailRequest = CreateClippTabDetailRequest(mmClippTabDetail);

    try
    {
        CoInitialize(NULL);

        //Invoke the web service client with the required parameters..
        _clippIntegrationWebClient->UpdateTabDetails(clippTabDetailRequest);

    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

//---------------------------------------------------------------------------

void TClippIntegrationInterface::SendTabDetails(UnicodeString clippTabRef, UnicodeString recievedMessageId, UnicodeString errorCode,
                                                     UnicodeString errorDescription, Currency totalPaymentAmount)
{
    ClippErrorDetail* clippErrorDetail = CreateClippErrorDetail(clippTabRef, recievedMessageId, errorCode, errorDescription, totalPaymentAmount);

    try
    {
        CoInitialize(NULL);

        _clippIntegrationWebClient->UpdateTabDetailsOnError(clippErrorDetail);

    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

//---------------------------------------------------------------------------

MMClippMessageDetail TClippIntegrationInterface::CloseTab(MMClippTabDetail mmClippTabDetail)
{
    ClippTabDetailRequest* clippTabDetailRequest = CreateClippTabDetailRequest(mmClippTabDetail);

    try
    {
        CoInitialize(NULL);

        //Invoke the web service client with the required parameters..
        ClippMessageDetail* clippMessageDetail = _clippIntegrationWebClient->CloseTab(clippTabDetailRequest);

        //Since we have the wsdl type, we need to convert it back to mm type..
        MMClippMessageDetail mmClippMessageDetail = CreateMMClippMessageDetail(clippMessageDetail);
        return mmClippMessageDetail;
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

//---------------------------------------------------------------------------

void TClippIntegrationInterface::CloseTab(UnicodeString clippTabRef, UnicodeString recievedMessageId, UnicodeString errorCode,
                                                            UnicodeString errorDescription, Currency totalPaymentAmount)
{
    ClippErrorDetail* clippErrorDetail = CreateClippErrorDetail(clippTabRef, recievedMessageId, errorCode, errorDescription, totalPaymentAmount);

    try
    {
        CoInitialize(NULL);

        _clippIntegrationWebClient->CloseTabOnError(clippErrorDetail);

    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

//---------------------------------------------------------------------------

MMPaymentDetailResponse TClippIntegrationInterface::RequestTabPayment(MMPaymentDetailRequest mmPaymentDetailRequest)
{
    PaymentDetailRequest* paymentDetailRequest = CreatePaymentDetailRequest(mmPaymentDetailRequest);

    try
    {
        CoInitialize(NULL);

        //Invoke the web service client with the required parameters..
        PaymentDetailResponse* paymentDetailResponse = _clippIntegrationWebClient->RequestTabPayment(paymentDetailRequest);

        //Since we have the wsdl type, we need to convert it back to mm type..
        MMPaymentDetailResponse mmPaymentDetailResponse = CreateMMPaymentDetailResponse(paymentDetailResponse);
        return mmPaymentDetailResponse;
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

//---------------------------------------------------------------------------

bool TClippIntegrationInterface::ForceCloseAllTabs()
{
    try
    {
        CoInitialize(NULL);

        //Invoke the web service client with the required parameters..
        return _clippIntegrationWebClient->ForceCloseAllTabs();
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

//---------------------------------------------------------------------------

CLIPP_MESSAGE_MAP TClippIntegrationInterface::ProcessMessagesIntoMap(CLIPP_MESSAGE_LIST clippMessages)
{
    //Initialize the map, this will hold the messages with message types.
    CLIPP_MESSAGE_MAP clippMessageMap;

    for( int i = clippMessages.Low; i <= clippMessages.High; i++)
    {
        ClippMessage* clippMessage = clippMessages[i];

        MMClippMessage mmClippMessage = CreateMMClippMessage(clippMessage);

        //We need to do this since clipp app never sends this id, and we need this id to perform all processing on pos..
        mmClippMessage.PosTabRef = GetTabKeyByClippTabRef(mmClippMessage.ClippTabRef);

        switch(clippMessage->MessageTypeEnum)
        {
            case TabOpen:
            {
                clippMessageMap[TabOpen].push_back(mmClippMessage);
                break;
            }
            case TabUpdate:
            {
                clippMessageMap[TabUpdate].push_back(mmClippMessage);
                break;
            }
            case TabClose:
            {
                clippMessageMap[TabClose].push_back(mmClippMessage);
                break;
            }
            case TabPay:
            {
                clippMessageMap[TabPay].push_back(mmClippMessage);
                break;
            }
        }
    }

    return clippMessageMap;
}

//---------------------------------------------------------------------------

MMClippMessage TClippIntegrationInterface::CreateMMClippMessage(ClippMessage* clippMessage)
{
    /*Do all the conversions from WSDL types to c++ types this is very much required here because
    we do not want to clutter code with wsdl reference..*/
    TDateTime createdDate = clippMessage->CreatedDate->AsDateTime;
    Currency discountAmount = StrToCurr(clippMessage->Data->DiscountAmount->DecimalString);
    Currency processingFeeAmount = StrToCurr(clippMessage->Data->ProcessingFeeAmount->DecimalString);
    Currency tabLimit = StrToCurr(clippMessage->Data->TabLimit->DecimalString);
    Currency tipAmount = StrToCurr(clippMessage->Data->TipAmount->DecimalString);
    Currency totalPaymentAmount = StrToCurr(clippMessage->Data->TotalPaymentAmount->DecimalString);

    MMClippMessage mmClippMessage = MMClippMessage(createdDate, clippMessage->Id, clippMessage->MessageId,
                                                CreateMMClippMessageType(clippMessage->MessageTypeEnum),
                                                clippMessage->Data->ClippPaymentRef, clippMessage->Data->ClippTabRef,
                                                clippMessage->Data->ClippUserRef, discountAmount, clippMessage->Data->FirstName,
                                                clippMessage->Data->LastName, clippMessage->Data->PhotoUrl, clippMessage->Data->PosTabRef,
                                                processingFeeAmount, tabLimit, tipAmount, totalPaymentAmount);

    if(clippMessage->Data->Discounts.Length > 0)
    {
        for( int i = clippMessage->Data->Discounts.Low; i <= clippMessage->Data->Discounts.High; i++)
        {
            Discount* discount = clippMessage->Data->Discounts[i];
            MMClippDiscount mmDiscount = CreateMMClippDiscount(discount);
            mmClippMessage.Discounts.push_back(mmDiscount);
        }
    }

    return mmClippMessage;
}

//---------------------------------------------------------------------------

MMClippMessageType TClippIntegrationInterface::CreateMMClippMessageType(ClippMessageType messageType)
{
    switch(messageType)
    {
        case TabClose:
            return TabClose;
            break;
        case TabPay:
            return TabPay;
            break;
        case TabUpdate:
            return TabUpdate;
            break;
        case TabStatus:
            return TabStatus;
            break;
        default:
            return TabOpen;
            break;
    }
}

//---------------------------------------------------------------------------

MMClippMessageDetail TClippIntegrationInterface::CreateMMClippMessageDetail(ClippMessageDetail* clippMessageDetail)
{
    /*Do all the conversions from WSDL types to c++ types this is very much required here because
    we do not want to clutter code with wsdl reference..*/
    Currency discountAmount = StrToCurr(clippMessageDetail->DiscountAmount->DecimalString);
    Currency processingFeeAmount = StrToCurr(clippMessageDetail->ProcessingFeeAmount->DecimalString);
    Currency tabLimit = StrToCurr(clippMessageDetail->TabLimit->DecimalString);
    Currency tipAmount = StrToCurr(clippMessageDetail->TipAmount->DecimalString);
    Currency totalPaymentAmount = StrToCurr(clippMessageDetail->TotalPaymentAmount->DecimalString);

    MMClippMessageDetail mmClippMessageDetail = MMClippMessageDetail(clippMessageDetail->ClippPaymentRef, clippMessageDetail->ClippTabRef,
                                                clippMessageDetail->ClippUserRef, discountAmount, clippMessageDetail->FirstName,
                                                clippMessageDetail->LastName, clippMessageDetail->PhotoUrl, clippMessageDetail->PosTabRef,
                                                processingFeeAmount, tabLimit, tipAmount, totalPaymentAmount);

    if(clippMessageDetail->Discounts.Length > 0)
    {
        for( int i = clippMessageDetail->Discounts.Low; i <= clippMessageDetail->Discounts.High; i++)
        {
            Discount* discount = clippMessageDetail->Discounts[i];
            MMClippDiscount mmDiscount = CreateMMClippDiscount(discount);
            mmClippMessageDetail.Discounts.push_back(mmDiscount);
        }
    }

    return mmClippMessageDetail;
}

//---------------------------------------------------------------------------

MMClippTabDetail TClippIntegrationInterface::CreateMMClippTabDetail(ClippTabDetailRequest* clippTabDetailRequest)
{
    /*Do all the conversions from WSDL types to c++ types this is very much required here because
    we do not want to clutter code with wsdl reference..*/
    Currency includedTaxAmount = StrToCurr(clippTabDetailRequest->TabData->IncludedTaxAmount->DecimalString);
    Currency outstandingAmount = StrToCurr(clippTabDetailRequest->TabData->OutstandingAmount->DecimalString);
    Currency tabLimit = StrToCurr(clippTabDetailRequest->TabData->TabLimit->DecimalString);
    Currency totalAmount = StrToCurr(clippTabDetailRequest->TabData->TotalAmount->DecimalString);
    TDateTime recieptDate = clippTabDetailRequest->TabData->ReceiptDate->AsDateTime;

    MMClippTabDetail mmClippTabDetail = MMClippTabDetail(clippTabDetailRequest->ClippTabRef, clippTabDetailRequest->MessageId,
                                                includedTaxAmount, outstandingAmount, recieptDate, tabLimit, totalAmount,
                                                clippTabDetailRequest->TabData->VenueReceiptId,
                                                clippTabDetailRequest->TabData->VenueTabRef );

    if(clippTabDetailRequest->TabData->Items.Length > 0)
    {
        for( int i = clippTabDetailRequest->TabData->Items.Low; i <= clippTabDetailRequest->TabData->Items.High; i++)
        {
            Item* item = clippTabDetailRequest->TabData->Items[i];
            MMClippItem mmClippItem = CreateMMClippItem(item);
            mmClippTabDetail.Items.push_back(mmClippItem);
        }
    }

    if(clippTabDetailRequest->TabData->Payments.Length > 0)
    {
        for( int i = clippTabDetailRequest->TabData->Payments.Low; i <= clippTabDetailRequest->TabData->Payments.High; i++)
        {
            NS_ClippIntegrationWSDL::Payment* payment = clippTabDetailRequest->TabData->Payments[i];
            MMClippPayment mmClippPayment = CreateMMClippPayment(payment);
            mmClippTabDetail.Payments.push_back(mmClippPayment);
        }
    }

    if(clippTabDetailRequest->TabData->Sundries.Length > 0)
    {
        for( int i = clippTabDetailRequest->TabData->Sundries.Low; i <= clippTabDetailRequest->TabData->Sundries.High; i++)
        {
            Sundry* sundry = clippTabDetailRequest->TabData->Sundries[i];
            MMClippSundry mmClippSundry = CreateMMClippSundry(sundry);
            mmClippTabDetail.Sundries.push_back(mmClippSundry);
        }
    }

    return mmClippTabDetail;
}

//---------------------------------------------------------------------------

MMClippDiscount TClippIntegrationInterface::CreateMMClippDiscount(Discount* discount)
{
    /*Do all the conversions from WSDL types to c++ types this is very much required here because
    we do not want to clutter code with wsdl reference..*/
    Currency amount = StrToCurr(discount->Amount->DecimalString);

    return MMClippDiscount(amount, discount->Description, discount->DiscountType, discount->Reference);
}

//---------------------------------------------------------------------------

MMClippItem TClippIntegrationInterface::CreateMMClippItem(Item* item)
{
    /*Do all the conversions from WSDL types to c++ types this is very much required here because
    we do not want to clutter code with wsdl reference..*/
    Currency discountAmount = StrToCurr(item->DiscountAmount->DecimalString);
    Currency price = StrToCurr(item->Price->DecimalString);
    Currency totalAmount = StrToCurr(item->TotalAmount->DecimalString);
    Currency quantity = StrToCurr(item->Quantity->DecimalString);

    MMClippItem mmClippItem = MMClippItem(item->Category, item->Description, discountAmount, item->DiscountDescription.SubString(0,50), item->Group,
                                        price, quantity, item->IsTaxInclusive, totalAmount, item->VenueItemRef);

    if(item->SubItems.Length > 0)
    {
        for( int i = item->SubItems.Low; i <= item->SubItems.High; i++)
        {
            Item* item = item->SubItems[i];
            MMClippItem mmClippItem = CreateMMClippItem(item);
            mmClippItem.SubItems.push_back(mmClippItem);
        }
    }

    return mmClippItem;
}

//---------------------------------------------------------------------------

MMClippPayment TClippIntegrationInterface::CreateMMClippPayment(NS_ClippIntegrationWSDL::Payment* payment)
{
    /*Do all the conversions from WSDL types to c++ types this is very much required here because
    we do not want to clutter code with wsdl reference..*/
    Currency amount = StrToCurr(payment->Amount->DecimalString);

    return MMClippPayment(amount, payment->Description, payment->VenuePaymentRef);
}

//---------------------------------------------------------------------------

MMClippSundry TClippIntegrationInterface::CreateMMClippSundry(Sundry* sundry)
{
    /*Do all the conversions from WSDL types to c++ types this is very much required here because
    we do not want to clutter code with wsdl reference..*/
    Currency amount = StrToCurr(sundry->Amount->DecimalString);

    return MMClippSundry(amount, sundry->Description, sundry->VenueSundryRef);
}

//---------------------------------------------------------------------------

MMPaymentDetailResponse TClippIntegrationInterface::CreateMMPaymentDetailResponse(PaymentDetailResponse* paymentDetailResponse)
{
    /*Do all the conversions from WSDL types to c++ types this is very much required here because
    we do not want to clutter code with wsdl reference..*/
    Currency processingFeeAmount = StrToCurr(paymentDetailResponse->ProcessingFeeAmount->DecimalString);
    Currency totalPaymentAmount = StrToCurr(paymentDetailResponse->TotalPaymentAmount->DecimalString);
    UnicodeString posTabRef = GetTabKeyByClippTabRef(paymentDetailResponse->ClippTabRef);

    return MMPaymentDetailResponse(paymentDetailResponse->ClippPaymentRef, paymentDetailResponse->ClippTabRef, posTabRef,
                                        processingFeeAmount, totalPaymentAmount);
}

//---------------------------------------------------------------------------

ClippTabDetailRequest* TClippIntegrationInterface::CreateClippTabDetailRequest(MMClippTabDetail mmClippTabDetail)
{
    /*We will have to convert the mm types to wsdl in order to make network calls for services..*/
    TXSDecimal* amount = new TXSDecimal;
    amount->XSToNative(CurrToStr(mmClippTabDetail.Amount));

    TXSDecimal* includedTaxAmount = new TXSDecimal;
    includedTaxAmount->XSToNative(CurrToStr(mmClippTabDetail.IncludedTaxAmount));

    TXSDecimal* outstandingAmount = new TXSDecimal;
    outstandingAmount->XSToNative(CurrToStr(mmClippTabDetail.OutstandingAmount));

    TXSDateTime* recieptDate = new TXSDateTime;
    recieptDate->AsDateTime = mmClippTabDetail.RecieptDate;

    TXSDecimal* tabLimit = new TXSDecimal;
    tabLimit->XSToNative(CurrToStr(mmClippTabDetail.TabLimit));

    TXSDecimal* totalAmount = new TXSDecimal;
    totalAmount->XSToNative(CurrToStr(mmClippTabDetail.TotalAmount));

    ClippTabDetail* clippTabDetail = new ClippTabDetail;

    //No Constructor exists so we need to set all the values manually..
    clippTabDetail->IncludedTaxAmount = includedTaxAmount;
    clippTabDetail->OutstandingAmount = outstandingAmount;
    clippTabDetail->ReceiptDate = recieptDate;
    clippTabDetail->TabLimit = tabLimit;
    clippTabDetail->TotalAmount = totalAmount;
    clippTabDetail->VenueReceiptId = mmClippTabDetail.VenueRecieptId;
    clippTabDetail->VenueTabRef = mmClippTabDetail.VenueTabRef;

    if(!mmClippTabDetail.Items.empty())
    {
        ArrayOfItem itemArray;
        for (std::vector<MMClippItem>::iterator it = mmClippTabDetail.Items.begin(); it != mmClippTabDetail.Items.end(); ++it)
        {
            MMClippItem mmClippItem = *it;
            Item* item = CreateItem(mmClippItem);

            //Increase the length of the dynamic array..
            itemArray.Length = (itemArray.Length + 1);
            itemArray[(itemArray.Length - 1)] = item;
        }
        clippTabDetail->Items = itemArray;
    }

    if(!mmClippTabDetail.Payments.empty())
    {
        ArrayOfPayment paymentArray;
        for (std::vector<MMClippPayment>::iterator it = mmClippTabDetail.Payments.begin(); it != mmClippTabDetail.Payments.end(); ++it)
        {
            MMClippPayment mmClippPayment = *it;
            NS_ClippIntegrationWSDL::Payment* payment = CreatePayment(mmClippPayment);

            //Increase the length of the dynamic array..
            paymentArray.Length = (paymentArray.Length + 1);
            paymentArray[(paymentArray.Length - 1)] = payment;
        }
        clippTabDetail->Payments = paymentArray;
    }

    if(!mmClippTabDetail.Sundries.empty())
    {
        ArrayOfSundry sundryArray;
        for (std::vector<MMClippSundry>::iterator it = mmClippTabDetail.Sundries.begin(); it != mmClippTabDetail.Sundries.end(); ++it)
        {
            MMClippSundry mmClippSundry = *it;
            Sundry* sundry = CreateSundry(mmClippSundry);

            //Increase the length of the dynamic array..
            sundryArray.Length = (sundryArray.Length + 1);
            sundryArray[(sundryArray.Length - 1)] = sundry;
        }
        clippTabDetail->Sundries = sundryArray;
    }

    //Now we create the actual data carrier for the service..
    ClippTabDetailRequest* clippTabDetailRequest = new ClippTabDetailRequest;
    clippTabDetailRequest->ClippTabRef = mmClippTabDetail.ClippTabRef;
    clippTabDetailRequest->MessageId = mmClippTabDetail.MessageId;
    clippTabDetailRequest->Amount = amount;

    clippTabDetailRequest->TabData = clippTabDetail;

    return clippTabDetailRequest;
}

//---------------------------------------------------------------------------

ClippErrorDetail* TClippIntegrationInterface::CreateClippErrorDetail(UnicodeString clippTabRef, UnicodeString recievedMessageId,
                                                            UnicodeString errorCode, UnicodeString errorDescription,
                                                            Currency totalPaymentAmount)
{
    /*We will have to convert the mm types to wsdl in order to make network calls for services..*/
    TXSDecimal* totalAmount = new TXSDecimal;
    totalAmount->XSToNative(CurrToStr(totalPaymentAmount));

    ClippErrorDetail* clippErrorDetail = new ClippErrorDetail;

    //No Constructor exists so we need to set all the values manually..
    clippErrorDetail->TotalAmount = totalAmount;
    clippErrorDetail->ClippTabRef = clippTabRef;
    clippErrorDetail->MessageId = recievedMessageId;
    clippErrorDetail->ErrorCode = errorCode;
    clippErrorDetail->ErrorDescription = errorDescription;

    return clippErrorDetail;
}


//---------------------------------------------------------------------------

Discount* TClippIntegrationInterface::CreateDiscount(MMClippDiscount mmClippDiscount)
{
    /*We will have to convert the mm types to wsdl in order to make network calls for services..*/
    TXSDecimal* amount = new TXSDecimal;
    amount->XSToNative(CurrToStr(mmClippDiscount.Amount));

    Discount* discount = new Discount;

    //No Constructor exists so we need to set all the values manually..
    discount->Amount = amount;
    discount->Description = mmClippDiscount.Description;
    discount->DiscountType = mmClippDiscount.DiscountType;
    discount->Reference = mmClippDiscount.Reference;

    return discount;
}

//---------------------------------------------------------------------------

Item* TClippIntegrationInterface::CreateItem(MMClippItem mmClippItem)
{
    /*We will have to convert the mm types to wsdl in order to make network calls for services..*/
    TXSDecimal* discountAmount = new TXSDecimal;
    discountAmount->XSToNative(CurrToStr(mmClippItem.DiscountAmount));

    TXSDecimal* price = new TXSDecimal;
    price->XSToNative(CurrToStr(mmClippItem.Price));

    TXSDecimal* totalAmount = new TXSDecimal;
    totalAmount->XSToNative(CurrToStr(mmClippItem.TotalAmount));

    TXSDecimal* quantity = new TXSDecimal;
    quantity->XSToNative(CurrToStr(mmClippItem.Quantity));

    //Create a new copy of item to be copied to the collection...
    Item* item = new Item;

    //No Constructor exists so we need to set all the values manually..
    item->Category = mmClippItem.Category;
    item->Description = mmClippItem.Description;
    item->DiscountAmount = discountAmount;
    item->DiscountDescription = mmClippItem.DiscountDescription.SubString(0,50);
    item->Group = mmClippItem.Group;
    item->Price = price;
    item->Quantity = quantity;
    item->IsTaxInclusive = mmClippItem.IsTaxInclusive;
    item->TotalAmount = totalAmount;
    item->VenueItemRef = mmClippItem.VenueItemRef;

    if(!mmClippItem.SubItems.empty())
    {
        ArrayOfItem subItemArray;
        for (std::vector<MMClippItem>::iterator it = mmClippItem.SubItems.begin(); it != mmClippItem.SubItems.end(); ++it)
        {
            MMClippItem mmClippItem = *it;
            Item* subItem = CreateItem(mmClippItem);

            //Increase the length of the dynamic array..
            subItemArray.Length = (subItemArray.Length + 1);
            subItemArray[(subItemArray.Length - 1)] = subItem;
        }
        item->SubItems = subItemArray;
    }

    return item;
}

//---------------------------------------------------------------------------

NS_ClippIntegrationWSDL::Payment* TClippIntegrationInterface::CreatePayment(MMClippPayment mmClippPayment)
{
    /*We will have to convert the mm types to wsdl in order to make network calls for services..*/
    TXSDecimal* amount = new TXSDecimal;
    amount->XSToNative(CurrToStr(mmClippPayment.Amount));

    NS_ClippIntegrationWSDL::Payment* payment = new NS_ClippIntegrationWSDL::Payment;

    //No Constructor exists so we need to set all the values manually..
    payment->Amount = amount;
    payment->Description = mmClippPayment.Description;
    payment->VenuePaymentRef = mmClippPayment.VenuePaymentRef;

    return payment;
}

//---------------------------------------------------------------------------

Sundry* TClippIntegrationInterface::CreateSundry(MMClippSundry mmClippSundry)
{
    /*We will have to convert the mm types to wsdl in order to make network calls for services..*/
    TXSDecimal* amount = new TXSDecimal;
    amount->XSToNative(CurrToStr(mmClippSundry.Amount));

    Sundry* sundry = new Sundry;

    //No Constructor exists so we need to set all the values manually..
    sundry->Amount = amount;
    sundry->Description = mmClippSundry.Description;
    sundry->VenueSundryRef = mmClippSundry.VenueSundryRef;

    return sundry;
}

//---------------------------------------------------------------------------

ClippMessageDetail* TClippIntegrationInterface::CreateClippMessageDetail(MMClippMessageDetail mmClippMessageDetail)
{
    /*We will have to convert the mm types to wsdl in order to make network calls for services..*/
    TXSDecimal* discountAmount = new TXSDecimal;
    discountAmount->XSToNative(CurrToStr(mmClippMessageDetail.DiscountAmount));

    TXSDecimal* processingFeeAmount = new TXSDecimal;
    processingFeeAmount->XSToNative(CurrToStr(mmClippMessageDetail.ProcessingFeeAmount));

    TXSDecimal* tabLimit = new TXSDecimal;
    tabLimit->XSToNative(CurrToStr(mmClippMessageDetail.TabLimit));

    TXSDecimal* tipAmount = new TXSDecimal;
    tipAmount->XSToNative(CurrToStr(mmClippMessageDetail.TipAmount));

    TXSDecimal* totalPaymentAmount = new TXSDecimal;
    totalPaymentAmount->XSToNative(CurrToStr(mmClippMessageDetail.TotalPaymentAmount));

    ClippMessageDetail* clippMessageDetail = new ClippMessageDetail;

    //No Constructor exists so we need to set all the values manually..
    clippMessageDetail->DiscountAmount = discountAmount;
    clippMessageDetail->ProcessingFeeAmount = processingFeeAmount;
    clippMessageDetail->TabLimit = tabLimit;
    clippMessageDetail->TipAmount = tipAmount;
    clippMessageDetail->TotalPaymentAmount = totalPaymentAmount;
    clippMessageDetail->ClippPaymentRef = mmClippMessageDetail.ClippPaymentRef;
    clippMessageDetail->ClippTabRef = mmClippMessageDetail.ClippTabRef;
    clippMessageDetail->ClippUserRef = mmClippMessageDetail.ClippUserRef;
    clippMessageDetail->FirstName = mmClippMessageDetail.FirstName;
    clippMessageDetail->LastName = mmClippMessageDetail.LastName;
    clippMessageDetail->PhotoUrl = mmClippMessageDetail.PhotoUrl;
    clippMessageDetail->PosTabRef = mmClippMessageDetail.PosTabRef;

    if(!mmClippMessageDetail.Discounts.empty())
    {
        ArrayOfDiscount discountArray;
        for (std::vector<MMClippDiscount>::iterator it = mmClippMessageDetail.Discounts.begin(); it != mmClippMessageDetail.Discounts.end(); ++it)
        {
            MMClippDiscount mmClippDiscount = *it;
            Discount* discount = CreateDiscount(mmClippDiscount);

            //Increase the length of the dynamic array..
            discountArray.Length = (discountArray.Length + 1);
            discountArray[(discountArray.Length - 1)] = discount;
        }
        clippMessageDetail->Discounts = discountArray;
    }

    return clippMessageDetail;
}

//---------------------------------------------------------------------------

PaymentDetailRequest* TClippIntegrationInterface::CreatePaymentDetailRequest(MMPaymentDetailRequest mmPaymentDetailRequest)
{
    /*We will have to convert the mm types to wsdl in order to make network calls for services..*/
    TXSDecimal* amount = new TXSDecimal;
    amount->XSToNative(CurrToStr(mmPaymentDetailRequest.Amount));

    PaymentDetailRequest* paymentDetailRequest = new PaymentDetailRequest;

    //No Constructor exists so we need to set all the values manually..
    paymentDetailRequest->Amount = amount;
    paymentDetailRequest->ClippMessageId = mmPaymentDetailRequest.ClippMessageId;
    paymentDetailRequest->ClippTabRef = mmPaymentDetailRequest.ClippTabRef;
    paymentDetailRequest->ErrorData = mmPaymentDetailRequest.ErrorData;

    return paymentDetailRequest;
}

//---------------------------------------------------------------------------

UnicodeString TClippIntegrationInterface::GetTabKeyByClippTabRef(UnicodeString clippTabRef)
{
    try
    {
        //Register the database transaction..
        Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
        TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
        dbTransaction.StartTransaction();

        UnicodeString posTabRef = IntToStr(TDBClippTab::GetTabKeyByClippTabRef(dbTransaction, clippTabRef));

        //Commit the transaction as we have completed all the transactions
        dbTransaction.Commit();

        return posTabRef;
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

//---------------------------------------------------------------------------
