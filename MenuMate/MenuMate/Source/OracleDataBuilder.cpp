//---------------------------------------------------------------------------


#pragma hdrstop

#include "OracleDataBuilder.h"
#include "OracleManagerDB.h"
#include "DBThirdPartyCodes.h"
#include "ManagerOraclePMS.h"
#include "GeneratorManager.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

TOracleDataBuilder::TOracleDataBuilder()
{

}
//---------------------------------------------------------------------------
TOracleDataBuilder::~TOracleDataBuilder()
{
}
//---------------------------------------------------------------------------
TLinkDescription TOracleDataBuilder::CreateLinkDescription()
{
    TLinkDescription linkDesc;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
    try
    {
        linkDesc.Date = Now().FormatString("YYMMDD");
        linkDesc.Time = Now().FormatString("HHMMSS");
        std::auto_ptr<TOracleManagerDB> managerDB(new TOracleManagerDB());
        linkDesc.VerNum = managerDB->GetVersionNumber(DBTransaction);
        DBTransaction.Commit();
    }
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        DBTransaction.Rollback();
	}
    return linkDesc;
}
//---------------------------------------------------------------------------
void TOracleDataBuilder::CreatePostRoomInquiry(TPostRoomInquiry &postRoomInquiry)
{
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
    std::map<int,TPMSPaymentType> paymentsMap;
    GetPMSPaymentType(paymentsMap);
    try
    {
        std::auto_ptr<TOracleManagerDB> managerDB(new TOracleManagerDB());
        postRoomInquiry.SequenceNumber = managerDB->GetSequenceNumber(DBTransaction);
        postRoomInquiry.MaximumReturnedMatches = "16";
        postRoomInquiry.RequestType = "4";
        postRoomInquiry.PaymentMethod = "Room";
        std::vector<TPayment> paymentsVector =  PaymentLoad(DBTransaction);
        for(int i = 0; i < paymentsVector.size(); i++)
        {
            TPayment payment = paymentsVector[i];
            if(payment.GetPaymentAttribute(ePayTypeRoomInterface))
            {
               postRoomInquiry.PaymentMethod = GetPMSPaymentCode(&payment,paymentsMap);//payment.PaymentThirdPartyID;
               break;
            }
        }
        postRoomInquiry.Date = Now().FormatString( "YYMMDD");
        postRoomInquiry.Time = Now().FormatString( "HHMMSS");
        postRoomInquiry.RevenueCenter = TDeviceRealTerminal::Instance().BasePMS->RevenueCentre;
        postRoomInquiry.WaiterId = TDeviceRealTerminal::Instance().User.Name + " " +
                                   TDeviceRealTerminal::Instance().User.Surname;
//        char* id = new char[20];
//        itoa(TDeviceRealTerminal::Instance().BasePMS->POSID , id , 10);
        postRoomInquiry.WorkstationId = TDeviceRealTerminal::Instance().ID.Name.SubString(0,20);
        DBTransaction.Commit();
    }
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        DBTransaction.Rollback();
	}
}
//----------------------------------------------------------------------------
void TOracleDataBuilder::GetPaymentAttributes(Database::TDBTransaction &DBTransaction,int PaymentKey,TPayment &Payment)
{
   TIBSQL *AttributeQuery    = DBTransaction.Query(DBTransaction.AddQuery());
   AttributeQuery->SQL->Text =  "SELECT * FROM PAYMENT_ATTRIBUTES WHERE PAYMENT_KEY = :PAYMENT_KEY ";
   AttributeQuery->ParamByName("PAYMENT_KEY")->AsInteger = PaymentKey;
   AttributeQuery->ExecQuery();
   for (; !AttributeQuery->Eof;)
   {
     Payment.SetPaymentAttribute(AttributeQuery->FieldByName("ATTRIBUTE_VALUE")->AsInteger);
     AttributeQuery->Next();
   }
}
//---------------------------------------------------------------------------
std::vector<TPayment> TOracleDataBuilder::PaymentLoad(Database::TDBTransaction &DBTransaction)
{
    std::vector<TPayment> paymentTypesVector;
    paymentTypesVector.clear();
	try
	{
		// Used by the Payment Editor see PaymentsLoadTypes for the Payment System Loader.
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = " SELECT * FROM PAYMENTTYPES "  " ORDER BY PAYMENTTYPES.DISPLAY_ORDER";
		IBInternalQuery->ExecQuery();
		for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
		{
            TPayment Payment;
            int key = IBInternalQuery->FieldByName("PAYMENT_KEY")->AsInteger;
			Payment.Name = IBInternalQuery->FieldByName("PAYMENT_NAME")->AsString;
			//Payment.Properties = IBInternalQuery->FieldByName("PROPERTIES")->AsInteger;
            GetPaymentAttributes(DBTransaction,key,Payment);
			Payment.DisplayOrder = IBInternalQuery->FieldByName("DISPLAY_ORDER")->AsInteger;
			Payment.Colour = IBInternalQuery->FieldByName("COLOUR")->AsInteger;
			Payment.AmountAdjust = IBInternalQuery->FieldByName("AMOUNT_ADJUST")->AsFloat;
			Payment.PercentAdjust = IBInternalQuery->FieldByName("PERCENT_ADJUST")->AsFloat;
			Payment.RoundTo = IBInternalQuery->FieldByName("ROUNDTO")->AsCurrency;
			Payment.TaxRate = IBInternalQuery->FieldByName("TAX_RATE")->AsFloat;
			Payment.AdjustmentReason = IBInternalQuery->FieldByName("ADJUST_REASON")->AsString;
			Payment.GroupNumber = IBInternalQuery->FieldByName("GROUP_NUMBER")->AsInteger;
			Payment.SecondaryPMSIPAddress = IBInternalQuery->FieldByName("DEST_IP")->AsString;
			Payment.SecondaryPMSPortNumber = IBInternalQuery->FieldByName("DEST_PORT")->AsInteger;
			Payment.Export = IBInternalQuery->FieldByName("INVOICE_EXPORT")->AsInteger;
			Payment.VoucherMerchantID = IBInternalQuery->FieldByName("VOUCHER_MERCHANT_ID")->AsString;
			Payment.PaymentThirdPartyID = TDBThirdPartyCodes::GetThirdPartyCodeByKey(DBTransaction,
			IBInternalQuery->FieldByName("THIRDPARTYCODES_KEY")->AsInteger);
			Payment.UniVoucherUser	=	IBInternalQuery->FieldByName("VOUCHER_USER")->AsString;
			Payment.UniVoucherPass	=	IBInternalQuery->FieldByName("VOUCHER_PASS")->AsString;
			Payment.CVSReadLocation	=	IBInternalQuery->FieldByName("CSV_READ_LOCATION")->AsString;
			Payment.CVSWriteLocation=	IBInternalQuery->FieldByName("CSV_WRITE_LOCATION")->AsString;
            Payment.TabKey =	IBInternalQuery->FieldByName("TabKey")->AsInteger;
            Payment.GLCode  =   IBInternalQuery->FieldByName("GL_CODE")->AsString;
            Payment.AutoPopulateBlindBalance =  IBInternalQuery->FieldByName("IS_AUTO_POPULATE_BLIND_BALANCE")->AsString == "T" ? true : false;
            paymentTypesVector.push_back(Payment);
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
    return paymentTypesVector;
}
//----------------------------------------------------------------------------
/* The method runs for one payment type and extracts the details depending upon the portion
of amount received through that particular payment type*/
//----------------------------------------------------------------------------
TPostRequest TOracleDataBuilder::CreatePost(TPaymentTransaction &paymentTransaction, double portion, int paymentIndex, double tip)
{
    TPostRequest postRequest;
    Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction1.StartTransaction();
    std::map<int,TPMSPaymentType> paymentsMap;
    GetPMSPaymentType(paymentsMap);
    try
    {
        double serviceCharge = 0;
        Currency discount = 0;
        std::map<int, double> discMap;
        discMap.clear();
        std::vector<TTax> taxVector;
        std::map<int,double> subtotals;
        std::map<int,double> taxMap;
        std::map<int,double> serviceChargeMap;
        taxMap.clear();
        serviceChargeMap.clear();
        taxVector.clear();
        subtotals.clear();
        SUrcharge = 0;
        for(int i = 0; i < paymentTransaction.Orders->Count; i++)
        {
            TItemComplete *itemComplete = (TItemComplete*)paymentTransaction.Orders->Items[i];
            // calculation of subtotal
            ExtractSubTotal(subtotals,discMap,taxMap,serviceChargeMap, itemComplete, portion);
            for(int k = 0; k < itemComplete->SubOrders->Count; k++)
            {
                TItemComplete *itemCompleteSub = (TItemComplete*)itemComplete->SubOrders->Items[k];
                // calculation of subtotal
                ExtractSubTotal(subtotals,discMap,taxMap, serviceChargeMap, itemCompleteSub, portion);
            }
        }

        AnsiString paymentMethod = "";
        bool isNotRoomPaymentType = false;
        TPayment *payment = paymentTransaction.PaymentGet(paymentIndex);
        paymentMethod = GetPMSPaymentCode(payment,paymentsMap);//payment->PaymentThirdPartyID;
        if(paymentMethod == "" && payment->GetPaymentAttribute(ePayTypeIntegratedEFTPOS) && payment->CardType != "")
        {
           paymentMethod = DoRequiredInsertion(payment, paymentsMap);
        }
        if(payment->GetPaymentAttribute(ePayTypePoints))
            paymentMethod = TDeviceRealTerminal::Instance().BasePMS->PointsCategory;
        if(payment->GetPaymentAttribute(ePayTypeCredit))
            paymentMethod = TDeviceRealTerminal::Instance().BasePMS->CreditCategory;
        if(paymentMethod.Trim() == "" || paymentMethod.Trim().Length() > 6)
            paymentMethod = GetPMSDefaultCode(paymentsMap);//TDeviceRealTerminal::Instance().BasePMS->DefaultPaymentCategory;
        if(!payment->GetPaymentAttribute(ePayTypeRoomInterface))
        {
            isNotRoomPaymentType = true;
        }

        //Add surcharge to first service charge entry
        std::map<int,double>::iterator itSubTotals =  subtotals.begin();
        for(;itSubTotals != subtotals.end(); advance(itSubTotals,1))
        {
           if(itSubTotals->second != 0)
           {
                double newValue = serviceChargeMap[itSubTotals->first] + SUrcharge;
                serviceChargeMap[itSubTotals->first] = newValue;
                break;
           }
        }
        double total = CalculateTotal(subtotals,discMap, taxMap, serviceChargeMap);
        total += tip;
        total += (double)payment->GetAdjustment();
        Currency patronCount = 0;
        std::vector<TPatronType>::iterator itPatron = paymentTransaction.Patrons.begin();
        for(;itPatron != paymentTransaction.Patrons.end(); ++itPatron)
        {
            patronCount += itPatron->Count;
        }
        // Identify Meal Code
        AnsiString mealName = "1";
        std::vector<TTimeSlots>::iterator it = TDeviceRealTerminal::Instance().BasePMS->Slots.begin();
        for(;it !=TDeviceRealTerminal::Instance().BasePMS->Slots.end(); ++it)
        {
            TDateTime date = EncodeDate(1899,12,30) + Now().CurrentTime();
            if(date >= it->StartTime &&
               date <= it->EndTime)
            {
                mealName = it->MealName;
                break;
            }
        }
        if(mealName == "1")
        {
            std::vector<TTimeSlots>::iterator itMeal = TDeviceRealTerminal::Instance().BasePMS->Slots.begin();
            for(;itMeal !=TDeviceRealTerminal::Instance().BasePMS->Slots.end(); ++itMeal)
            {
                if(itMeal->IsDefault)
                {
                    mealName = itMeal->MealName;
                    break;
                }
            }
        }
        postRequest.RoomNumber = paymentTransaction.PMSClientDetails.RoomNumber;
        postRequest.ReservationId = paymentTransaction.PMSClientDetails.ReservationID;
        postRequest.ProfileId = paymentTransaction.PMSClientDetails.ProfileID;
        postRequest.LastName = paymentTransaction.PMSClientDetails.LastName;
        postRequest.HotelId = paymentTransaction.PMSClientDetails.HotelID;
        if(payment->GetPaymentAttribute(ePayTypeRoomInterface))
            postRequest.RequestType = "4";
        else
            postRequest.RequestType = "1";
        postRequest.InquiryInformation = paymentTransaction.PMSClientDetails.RoomNumber;
        postRequest.MatchfromPostList = paymentTransaction.PMSClientDetails.MatchIdentifier;
        std::auto_ptr<TOracleManagerDB> managerDB(new TOracleManagerDB());
        postRequest.SequenceNumber = managerDB->GetSequenceNumber(DBTransaction1);
        DBTransaction1.Commit();
        double totalValue = (double)total;
        totalValue = totalValue * 100;
        postRequest.TotalAmount = totalValue;
        postRequest.CreditLimitOverride = "N";
        postRequest.PaymentMethod = paymentMethod;
        postRequest.Covers = CurrToStrF(patronCount,ffFixed,0);
        postRequest.RevenueCenter = TDeviceRealTerminal::Instance().BasePMS->RevenueCentre;
        postRequest.ServingTime = mealName;//"BreakFast";
        tip = tip * 100;
        postRequest.Tip = tip;
        double data = 0;

        std::map<int,double>::iterator itsubtotals =  subtotals.begin();
        for(int i = 0; i < 16; i++)
        {
           postRequest.Subtotal1.push_back("");
           postRequest.Discount.push_back("");
           postRequest.Tax.push_back("");
           postRequest.ServiceCharge.push_back("");
        }
        for(;itsubtotals != subtotals.end(); advance(itsubtotals,1))
        {
           double value = (double)itsubtotals->second * 100;
           AnsiString str = (AnsiString)value;
           if(str.Pos(".") != 0)
           {
              str = str.SubString(1,str.Pos(".")-1);
           }
           postRequest.Subtotal1[itsubtotals->first-1] = str;
        }

        std::map<int,double>::iterator itDisc =  discMap.begin();
        for(;itDisc != discMap.end(); advance(itDisc,1))
        {
           double value = (double)itDisc->second * 100;
           AnsiString str = (AnsiString)value;
           if(str.Pos(".") != 0)
           {
              str = str.SubString(1,str.Pos(".")-1);
           }
           postRequest.Discount[itDisc->first-1] = str;
        }
        std::map<int,double>::iterator itTaxMap =  taxMap.begin();
        for(;itTaxMap != taxMap.end(); advance(itTaxMap,1))
        {
           double value = (double)itTaxMap->second * 100;
           AnsiString str = (AnsiString)value;
           if(str.Pos(".") != 0)
           {
              str = str.SubString(1,str.Pos(".")-1);
           }
           postRequest.Tax[itTaxMap->first-1] = str;
        }

        std::map<int,double>::iterator itServiceChargeMap1 =  serviceChargeMap.begin();
        for(;itServiceChargeMap1 != serviceChargeMap.end(); advance(itServiceChargeMap1,1))
        {
           double value = (double)itServiceChargeMap1->second * 100;
           AnsiString str = (AnsiString)value;
           if(str.Pos(".") != 0)
           {
              str = str.SubString(1,str.Pos(".")-1);
           }
           postRequest.ServiceCharge[itServiceChargeMap1->first-1] = str;
        }
        postRequest.Date = Now().FormatString( "YYMMDD");
        postRequest.Time = Now().FormatString( "HHMMSS");
        postRequest.WaiterId = TDeviceRealTerminal::Instance().User.Name + " " +
                               TDeviceRealTerminal::Instance().User.Surname;
        postRequest.WorkstationId = TDeviceRealTerminal::Instance().ID.Name.SubString(0,20);
        if(isNotRoomPaymentType)
        {
           postRequest.RoomNumber = "";
           postRequest.ReservationId = "";
           postRequest.ProfileId = "";
           postRequest.LastName = "";
           postRequest.InquiryInformation = "";
           postRequest.RequestType = "1";
           postRequest.MatchfromPostList = "";
        }
    }
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        DBTransaction1.Rollback();
//        MessageBox(E.Message,"Exception in creating XML",MB_OK);
	}
    return postRequest;
}
//----------------------------------------------------------------------------
TiXmlDocument TOracleDataBuilder::CreateLinkDescriptionXML(TLinkDescription linkDescription)
{
	TiXmlDocument doc = PrepareXMLDocument();
    try
    {
        TiXmlElement *rootNode = new TiXmlElement("LinkDescription");
        doc.LinkEndChild( rootNode );
        AddInvoiceAttrs( rootNode,linkDescription);
    }
    catch( Exception &E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
    }
    return doc;
}
//----------------------------------------------------------------------------
TiXmlDocument TOracleDataBuilder::CreateRoomInquiryXML(TPostRoomInquiry &postRequest)
{
	TiXmlDocument doc = PrepareXMLDocument();
    try
    {
    	// add root node ( Oracle )
        TiXmlElement *rootNode = new TiXmlElement("PostInquiry");
        doc.LinkEndChild( rootNode );
        AddInvoiceAttrs( rootNode,postRequest);
    }
    catch( Exception &E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
    }
    return doc;
}
//----------------------------------------------------------------------------
TiXmlDocument TOracleDataBuilder::PrepareXMLDocument()
{
	TiXmlDocument doc;
    try
    {
    	TiXmlDeclaration * decl = new TiXmlDeclaration(_T("1.0"), _T(""), _T(""));
    	doc.LinkEndChild( decl );
    }
    catch (Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
    }
    return doc;
}
//----------------------------------------------------------------------------
void TOracleDataBuilder::AddInvoiceAttrs(TiXmlElement *rootNode,TLinkDescription linkDescription)
{
    try
    {
    SetNodeAttr( rootNode, "Date",      linkDescription.Date );
    SetNodeAttr( rootNode, "Time",  linkDescription.Time );
    SetNodeAttr( rootNode, "VerNum",  linkDescription.VerNum );
    }
    catch( Exception &E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
    }
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
TiXmlDocument TOracleDataBuilder::CreatePostXML(TPostRequest &postRequest)
{
    TiXmlDocument doc;
    try
    {
        doc = PrepareXMLDocument();
        // add root node ( Oracle )
        TiXmlElement *rootNode = new TiXmlElement("PostRequest");
        doc.LinkEndChild( rootNode );
        AddInvoiceAttrs( rootNode,postRequest);
    }
    catch( Exception &E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
    }
    return doc;
}
//----------------------------------------------------------------------------
void TOracleDataBuilder::AddInvoiceAttrs(TiXmlElement *rootNode,TPostRoomInquiry &postRequest)
{
    try
    {
         SetNodeAttr( rootNode, "InquiryInformation",      postRequest.InquiryInformation );
         SetNodeAttr( rootNode, "MaximumReturnedMatches",  postRequest.MaximumReturnedMatches );
         SetNodeAttr( rootNode, "SequenceNumber",          postRequest.SequenceNumber );
         SetNodeAttr( rootNode, "RequestType",             postRequest.RequestType );
         SetNodeAttr( rootNode, "PaymentMethod",           postRequest.PaymentMethod );
         SetNodeAttr( rootNode, "Date",                    postRequest.Date );
         SetNodeAttr( rootNode, "Time",                    postRequest.Time );
         SetNodeAttr( rootNode, "RevenueCenter",           postRequest.RevenueCenter );
         SetNodeAttr( rootNode, "WaiterId",                postRequest.WaiterId );
         SetNodeAttr( rootNode, "WorkstationId",           postRequest.WorkstationId );
    }
    catch( Exception &E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
    }
}
//----------------------------------------------------------------------------
void TOracleDataBuilder::AddInvoiceAttrs(TiXmlElement *rootNode,TPostRequest &postRequest)
{
    try
    {
        SetNodeAttr( rootNode, "RoomNumber",            postRequest.RoomNumber.c_str() );
        SetNodeAttr( rootNode, "ReservationId",         postRequest.ReservationId.c_str() );
        SetNodeAttr( rootNode, "ProfileId",             postRequest.ProfileId.c_str() );
        SetNodeAttr( rootNode, "LastName",             	postRequest.LastName.c_str() );
        SetNodeAttr( rootNode, "HotelId",             	postRequest.HotelId.c_str() );
        SetNodeAttr( rootNode, "RequestType",           postRequest.RequestType.c_str() );
        SetNodeAttr( rootNode, "InquiryInformation",    postRequest.InquiryInformation.c_str() );
        SetNodeAttr( rootNode, "MatchfromPostList",     postRequest.MatchfromPostList.c_str() );
        SetNodeAttr( rootNode, "SequenceNumber",        postRequest.SequenceNumber.c_str() );
        SetNodeAttr( rootNode, "TotalAmount",           postRequest.TotalAmount.c_str() );
        SetNodeAttr( rootNode, "CreditLimitOverride",   postRequest.CreditLimitOverride.c_str() );
        SetNodeAttr( rootNode, "PaymentMethod",         postRequest.PaymentMethod.c_str() );
        SetNodeAttr( rootNode, "Covers",             	postRequest.Covers.c_str() );
        SetNodeAttr( rootNode, "RevenueCenter",         postRequest.RevenueCenter.c_str() );
        SetNodeAttr( rootNode, "ServingTime",           postRequest.ServingTime.c_str() );
        SetNodeAttr( rootNode, "CheckNumber",           postRequest.CheckNumber.c_str() );

        std::vector<AnsiString>::iterator itsubtotal =  postRequest.Subtotal1.begin();
        if(postRequest.Subtotal1.size() == 0)
        {
           SetNodeAttr( rootNode, "Subtotal1",             0 );
        }
        else
        {
            int j = 1;
            int i = 0;
            bool isAdded = false;
            for(; itsubtotal != postRequest.Subtotal1.end(); ++itsubtotal)
            {
                AnsiString nodeName = "Subtotal";
                nodeName += j;
                if(postRequest.Subtotal1[i].Trim() != "" )
                {
                    SetNodeAttr( rootNode, nodeName,             postRequest.Subtotal1[i].c_str() );
                    isAdded = true;
                }
                i += 1;
                j += 1;
            }
            if(!isAdded)
              SetNodeAttr( rootNode, "Subtotal1",             0 );
        }
        std::vector<AnsiString>::iterator itdiscount =  postRequest.Discount.begin();
        if(postRequest.Discount.size() == 0)
        {
           SetNodeAttr( rootNode, "Discount1",             0 );
        }
        else
        {
            int j = 1;
            int i = 0;
            bool isAdded = false;
            for(; itdiscount != postRequest.Discount.end(); ++itdiscount)
            {
                AnsiString nodeName = "Discount";
                nodeName += j;
                if(postRequest.Discount[i].Trim() != "" )
                {
                    SetNodeAttr( rootNode, nodeName,             postRequest.Discount[i].c_str() );
                    isAdded = true;
                }
                j += 1;
                i += 1;
            }
            if(!isAdded)
                SetNodeAttr( rootNode, "Discount1",             0 );
        }

        SetNodeAttr( rootNode, "Tip",         			postRequest.Tip.c_str() );

        std::vector<AnsiString>::iterator itservicecharge =  postRequest.ServiceCharge.begin();
        if(postRequest.ServiceCharge.size() == 0)
        {
           SetNodeAttr( rootNode, "ServiceCharge1",             0 );
        }
        else
        {
            int j = 1;
            int i = 0;
            bool isAdded = false;
            for(; itservicecharge != postRequest.ServiceCharge.end(); ++itservicecharge)
            {
                AnsiString nodeName = "ServiceCharge";
                nodeName += j;
                if(postRequest.ServiceCharge[i].Trim() != "" )
                {
                    SetNodeAttr( rootNode, nodeName,             postRequest.ServiceCharge[i].c_str() );
                    isAdded = true;
                }
                j += 1;
                i += 1;
            }
            if(!isAdded)
               SetNodeAttr( rootNode, "ServiceCharge1",             0 );
        }
        std::vector<AnsiString>::iterator itTax =  postRequest.Tax.begin();
        if(postRequest.Tax.size() == 0)
        {
           SetNodeAttr( rootNode, "Tax1",            0 );
        }
        else
        {
            int j = 1;
            int i = 0;
            bool isAdded = false;
            for(; itTax != postRequest.Tax.end(); ++itTax)
            {
                AnsiString nodeName = "Tax";
                nodeName += j;
                if(postRequest.Tax[i].Trim() != "" )
                {
                    SetNodeAttr( rootNode, nodeName,             postRequest.Tax[i].c_str());
                    isAdded = true;
                }
                j += 1;
                i += 1;
            }
            if(!isAdded)
              SetNodeAttr( rootNode, "Tax1",            0 );
        }

        SetNodeAttr( rootNode, "Date",              	postRequest.Date.c_str() );
        SetNodeAttr( rootNode, "Time",              	postRequest.Time.c_str() );
        SetNodeAttr( rootNode, "WaiterId",             	postRequest.WaiterId.c_str() );
        SetNodeAttr( rootNode, "WorkstationId",         postRequest.WorkstationId.c_str() );
    }
    catch( Exception &E )
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
    }
}
//----------------------------------------------------------------------------
void TOracleDataBuilder::SetNodeAttr(TiXmlElement *inNode, AnsiString inAttrName, AnsiString inAttrValue)
{
    inNode->SetAttribute( inAttrName.c_str(), inAttrValue.c_str() );
}
//----------------------------------------------------------------------------
TRoomInquiryResult TOracleDataBuilder::createXMLInquiryDoc()
{
	TiXmlDocument* result = new TiXmlDocument();
    TiXmlDeclaration * decl = new TiXmlDeclaration(_T("1.0"), _T(""), _T(""));
	result->LinkEndChild( decl );
	//::::::::::::::::::::::::::::::
    TRoomInquiryResult roomInquiryResult;
	try
	{
		result->LoadFile( (ExtractFilePath(Application->ExeName) + "\\"+ "Oracle Room Inquiry\\" + "roomInquiry" + ".txt").t_str() );
        ReadXML(result,roomInquiryResult);
	}
	catch( ... )
	{
		delete result;
		result = NULL;
	}
	//::::::::::::::::::::::::::::::
	return roomInquiryResult;
}
//----------------------------------------------------------------------------
void TOracleDataBuilder::ReadXML(TiXmlDocument *result,TRoomInquiryResult &roomInquiryResult)
{
    _rootElem           = result->RootElement();
    AnsiString value = _rootElem->Value();
    if(value == "PostList")
    {
        roomInquiryResult.SequenceNumber = _rootElem->Attribute("SequenceNumber");
        roomInquiryResult.HotelId = _rootElem->Attribute("HotelId");
        roomInquiryResult.PaymentMethod = _rootElem->Attribute("PaymentMethod");
        roomInquiryResult.RevenueCenter = _rootElem->Attribute("PaymentMethod");
        roomInquiryResult.WaiterId = _rootElem->Attribute("WaiterId");
        roomInquiryResult.WorkstationId = _rootElem->Attribute("WorkstationId");
        roomInquiryResult.Date = _rootElem->Attribute("Date");
        roomInquiryResult.Time = _rootElem->Attribute("Time");
        int countOfGuest = ChildCount(_rootElem);
        if(countOfGuest > 0)
        {
            _itemsElem = loaditemsElem(_rootElem);
            for(int index = 0; index < countOfGuest; index++)
            {
                LoadCustomerDetails(index,roomInquiryResult);
                roomInquiryResult.IsSuccessful = true;
            }
        }
    }
    else if(value == "PostAnswer" || !(roomInquiryResult.RoomInquiryItem.size() > 0))
    {
        roomInquiryResult.IsSuccessful = false;
        roomInquiryResult.resultText = _rootElem->Attribute("ResponseText");
        if(roomInquiryResult.resultText.Length() == 0)
        {
            roomInquiryResult.resultText = "No details found for the room inquired.";
        }
    }
}
//----------------------------------------------------------------------------
void TOracleDataBuilder::ReadXML(TiXmlDocument *result,TPostRequestAnswer &_postResult)
{
    _rootElem           = result->RootElement();
    AnsiString value = _rootElem->Value();
    if(value == "PostAnswer")
    {
        _postResult.AnswerStatus = _rootElem->Attribute("AnswerStatus");
        _postResult.resultText   = _rootElem->Attribute("ResponseText");
    }
}
//----------------------------------------------------------------------------
void TOracleDataBuilder::LoadCustomerDetails(int _index,TRoomInquiryResult &roomInquiryResult)
{
    try
    {
        if( _index >= 0 )
        {
            int it = 0;
			TiXmlElement *itPtr = _rootElem->FirstChildElement();
			while( ( itPtr != NULL ) && ( it <= _index ) )
			{
				if( it == _index )
				{
                    TRoomInquiryItem inquiryItems;
                    inquiryItems.RoomNumber = itPtr->Attribute("RoomNumber");
                    inquiryItems.ReservationId = itPtr->Attribute("ReservationId");
                    inquiryItems.LastName = itPtr->Attribute("LastName");
                    inquiryItems.FirstName = itPtr->Attribute("FirstName");
                    inquiryItems.Title = itPtr->Attribute("Title");
                    inquiryItems.GuestTitle = itPtr->Attribute("GuestTitle");
                    inquiryItems.NoPost = itPtr->Attribute("NoPost");
                    inquiryItems.CreditLimit = itPtr->Attribute("CreditLimit");
                    inquiryItems.ProfileId = itPtr->Attribute("ProfileId");
                    inquiryItems.HotelId = itPtr->Attribute("HotelId");
                    roomInquiryResult.RoomInquiryItem.push_back(inquiryItems);
                    break;
                }
                itPtr = itPtr->NextSiblingElement();
                it++;
            }
        }
	}
	catch(Exception &E)
	{
       TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}
//----------------------------------------------------------------------------
TiXmlElement* TOracleDataBuilder::loaditemsElem(TiXmlElement* _rootElem)
{
	try
	{
		TiXmlElement *element = _rootElem->FirstChildElement();

		while( element != NULL )
		{
			if( AnsiString( element->Value() ) == "PostListItem" )
			{
				return element;
			}
			element = element->NextSiblingElement();
		}
	}
	catch(Exception &E)
	{
       TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
    return 0;
}
//----------------------------------------------------------------------------
__int32 TOracleDataBuilder::ChildCount( TiXmlElement* inElem )
{
	__int32 result = 0;

	//::::::::::::::::::::::::::::::::

	try
	{
		if(inElem)
		{
			TiXmlElement *child = inElem->FirstChildElement();
			while( child != NULL )
			{
				result++;
				child = child->NextSiblingElement();
			}
		}
	}
	catch(Exception &E)
	{
       TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}

	//::::::::::::::::::::::::::::::::

	return result;
}
//----------------------------------------------------------------------------
void TOracleDataBuilder::ExtractDiscount(std::map<int, double> &discMap, TItemComplete *itemComplete, double portion)
{
}
//----------------------------------------------------------------------------
void TOracleDataBuilder::ExtractSubTotal(std::map<int,double> &subtotals, std::map<int, double> &discMap,
                                         std::map<int,double> &taxMap, std::map<int, double> &serviceChargeMap,
                                         TItemComplete *itemComplete, double portion)
{
    double priceExclusive = 0;
    double taxValue = 0;
    double serviceCharge = 0;
    double finalPrice = 0;
    double discountTotal = 0;
    double surcharge = 0;
    double discountNew = 0;
    double discountSurcharge = 0;
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
    //*********************************************
    std::vector<TDiscount>::iterator itDiscount = itemComplete->Discounts.begin();
    if(itemComplete->GetQty() >= 0)
    {
        for(; itDiscount != itemComplete->Discounts.end(); advance(itDiscount,1))
        {
            Currency value = itemComplete->DiscountValue_BillCalc(itDiscount);
            double valueDouble = value;
            if(valueDouble >= 0.0)
                surcharge += valueDouble;
            else
                discountNew += valueDouble;
        }
    }
    else
    {
        for(; itDiscount != itemComplete->Discounts.end(); advance(itDiscount,1))
        {
            Currency value = itemComplete->DiscountValue_BillCalc(itDiscount);
            double valueDouble = value;
            if(valueDouble <= 0.0)
                surcharge += valueDouble;
            else
                discountNew += valueDouble;
        }
    }
    discountSurcharge = discountNew + surcharge;
    //*************Data Rounding to 2 decimal places*********//
    finalPrice      = RoundTo((double)itemComplete->BillCalcResult.FinalPrice * portion,-2);
//    discount        = RoundTo((double)itemComplete->BillCalcResult.TotalDiscount * portion,-2);
//    discountSurcharge = RoundTo((double)discountSurcharge * portion,-2);
    discountNew     = RoundTo((double)discountNew * portion,-2);
    surcharge       = RoundTo((double)surcharge * portion,-2);
    discountSurcharge = discountNew + surcharge;
//    discount        = RoundTo(discountNew * portion, -2);
    taxValue        = RoundTo((double)itemComplete->BillCalcResult.TotalTax * portion,-2);
    serviceCharge   = RoundTo((double)itemComplete->BillCalcResult.ServiceCharge.Value * portion,-2);
    //*******************************************************//
//    surcharge
    priceExclusive = finalPrice - discountSurcharge;
//    discountSurcharge = discountNew + surcharge;

    SUrcharge += surcharge;
    if(!TGlobalSettings::Instance().ItemPriceIncludeServiceCharge)
        priceExclusive = priceExclusive - serviceCharge;
    else
       serviceCharge = 0;

    if(!TGlobalSettings::Instance().ItemPriceIncludeTax)
        priceExclusive = priceExclusive - taxValue;
    else
        taxValue = 0;

    if(subtotals.size() == 0)
    {
        subtotals.insert(std::pair<int,double>(itemComplete->RevenueCode,priceExclusive));
        taxMap.insert(std::pair<int,double>(itemComplete->RevenueCode,taxValue));
        serviceChargeMap.insert(std::pair<int,double>(itemComplete->RevenueCode,serviceCharge));
    }
    else
    {
        std::map<int,double>::iterator itsubtotals = subtotals.find(itemComplete->RevenueCode);
        if(itsubtotals != subtotals.end())
        {
            priceExclusive += itsubtotals->second;
            subtotals[itemComplete->RevenueCode] = priceExclusive;
            std::map<int,double>::iterator itTaxMap = taxMap.find(itemComplete->RevenueCode);
            if(itTaxMap != taxMap.end())
            {
                taxValue += itTaxMap->second;
                taxMap[itemComplete->RevenueCode] = taxValue;
            }
            std::map<int,double>::iterator itServiceChargeMap = serviceChargeMap.find(itemComplete->RevenueCode);
            if(itServiceChargeMap != serviceChargeMap.end())
            {
                serviceCharge += itServiceChargeMap->second;
                serviceChargeMap[itemComplete->RevenueCode] = serviceCharge;
            }
        }
        else
        {
            subtotals.insert(std::pair<int,double>(itemComplete->RevenueCode,priceExclusive));
            taxMap.insert(std::pair<int,double>(itemComplete->RevenueCode,taxValue));
            serviceChargeMap.insert(std::pair<int,double>(itemComplete->RevenueCode,serviceCharge));
        }
    }
    if(discMap.size() == 0)
    {
        discMap.insert(std::pair<int,double>(itemComplete->RevenueCode,discountNew));
    }
    else
    {
        std::map<int,double>::iterator itDisc = discMap.find(itemComplete->RevenueCode);
        if(itDisc != discMap.end())
        {
            double newValue = itDisc->second + discountNew;
            discMap[itemComplete->RevenueCode] = newValue;
        }
        else
        {
            discMap.insert(std::pair<int,double>(itemComplete->RevenueCode,discountNew));
        }
    }
}
//----------------------------------------------------------------------------
void TOracleDataBuilder::ExtractTaxes(TItemComplete *itemComplete,
                                                   std::vector<TTax> &taxVector, double portion)
{
    for(int j = 0; j < itemComplete->BillCalcResult.Tax.size(); j++)
    {
        if(taxVector.size() == 0)
        {
            TTax tax;
            tax.Name = itemComplete->BillCalcResult.Tax[j].Name;
            tax.Type = (int)itemComplete->BillCalcResult.Tax[j].TaxType;
            tax.Percentage = itemComplete->BillCalcResult.Tax[j].Percentage;
            tax.Value = itemComplete->BillCalcResult.Tax[j].Value * portion;
            taxVector.push_back(tax);
        }
        else
        {
            std::vector<TTax>::iterator taxIT = taxVector.begin();
            int ptr = 0;
            bool existingTax = false;
            TTax tax;
            tax.Name = "";
            tax.Type = 0;
            tax.Percentage = 0;
            tax.Value = 0;
            for(;taxIT != taxVector.end(); advance(taxIT,1))
            {
                if(taxIT->Name != itemComplete->BillCalcResult.Tax[j].Name ||
                   taxIT->Percentage != itemComplete->BillCalcResult.Tax[j].Percentage ||
                   taxIT->Type != (int)itemComplete->BillCalcResult.Tax[j].TaxType)
                {
                    tax.Name = itemComplete->BillCalcResult.Tax[j].Name;
                    tax.Type = (int)itemComplete->BillCalcResult.Tax[j].TaxType;
                    tax.Percentage = itemComplete->BillCalcResult.Tax[j].Percentage;
                    tax.Value = itemComplete->BillCalcResult.Tax[j].Value * portion;
                }
                else
                {
                    existingTax = true;
                    double taxValue = taxVector[ptr].Value;
                    taxValue += (double)itemComplete->BillCalcResult.Tax[j].Value * portion;
                    taxVector[ptr].Value = taxValue;
                    break;
                }
                ptr++;
            }
            if(!existingTax)
            {
                taxVector.push_back(tax);
            }
        }
    }
}
//----------------------------------------------------------------------------
double TOracleDataBuilder::CalculateTotal(std::map<int,double> subtotals, std::map<int, double> discMap,
                                          std::map<int,double> &taxMap, std::map<int, double> &serviceChargeMap )
{
    double totalValue = 0;
    for(std::map<int,double>::iterator itdisc = discMap.begin(); itdisc != discMap.end();advance(itdisc,1))
    {
        totalValue += itdisc->second;
    }
    for(std::map<int,double>::iterator itsubtotals = subtotals.begin(); itsubtotals != subtotals.end();advance(itsubtotals,1))
    {
        totalValue += itsubtotals->second;
    }
    for(std::map<int,double>::iterator itTaxMap = taxMap.begin(); itTaxMap != taxMap.end();advance(itTaxMap,1))
    {
        totalValue += itTaxMap->second;
    }
    for(std::map<int,double>::iterator itserviceChargeMap = serviceChargeMap.begin();
        itserviceChargeMap != serviceChargeMap.end();advance(itserviceChargeMap,1))
    {
        totalValue += itserviceChargeMap->second;
    }
    return totalValue;
}
//----------------------------------------------------------------------------
void TOracleDataBuilder::ExtractServiceCharges(std::vector<TServiceCharge> serviceChargeVector,
                                                TItemComplete *itemComplete)
{
}
//---------------------------------------------------------------------------
AnsiString TOracleDataBuilder::SerializeOut(TiXmlDocument inDoc)
{
   AnsiString result = "";
   //::::::::::::::::::::::::::::::::::::::::::::

   TiXmlPrinter printer;

   printer.SetIndent( _T("\t") );
   inDoc.Accept( &printer );

   result = AnsiString( printer.CStr() );
   //::::::::::::::::::::::::::::::::::::::::::::
   result = result.SubString(23,result.Length() - 23);
   result = result.Trim();
   return result;
}
//----------------------------------------------------------------------------
bool TOracleDataBuilder::DeserializeGetLinkStatus(AnsiString inData)
{
    bool retValue = inData.Pos("LinkAlive") != 0;

    if(inData.Trim().Length() == 0)
        MessageBox("Oracle PMS was not enabled because no response was received.\nPlease check Oracle is up and working.","Error", MB_OK+MB_ICONERROR);
    if((inData.Trim().Length() != 0) && !retValue)
        MessageBox(inData,"Error", MB_OK+MB_ICONERROR);
    return (inData.Pos("LinkAlive") != 0);
}
//----------------------------------------------------------------------------
bool TOracleDataBuilder::DeserializeInquiryData(AnsiString inData, TRoomInquiryResult &_roomResult)
{
    bool retValue = false;
    try
    {
        TiXmlDocument* result = new TiXmlDocument();
        if(inData.Trim().Length() != 0)
        {
            result->Parse(inData.c_str());
            ReadXML(result,_roomResult);
            retValue = true;
        }
        else
        {
            retValue= false;
            _roomResult.IsSuccessful = false;
            if(TGlobalSettings::Instance().IsOraclePOSServer)
            {
                _roomResult.resultText = "No Response Received from Oracle.\nPlease ensure Oracle is up and working.\n";
            }
            else
            {
                _roomResult.resultText = "No Response Received from Oracle.\nPlease ensure Oracle is up and working.";
            }
        }
    }
    catch(Exception &Ex)
    {
        retValue= false;
        _roomResult.IsSuccessful = false;
        if(TGlobalSettings::Instance().IsOraclePOSServer)
        {
            _roomResult.resultText = "No Response Received from Oracle.\nPlease ensure Oracle is up and running.";
        }
        else
        {
            _roomResult.resultText = "No Response Received from Oracle.\nPlease ensure Oracle is up and running.";
        }
    }
    return retValue;
}
//----------------------------------------------------------------------------
bool TOracleDataBuilder::DeserializeData(AnsiString inData, TPostRequestAnswer &_postResult)
{
    bool retValue = false;
    try
    {
        TiXmlDocument* result = new TiXmlDocument();
        if(inData.Trim().Length() != 0)
        {
            result->Parse(inData.c_str());
            ReadXML(result,_postResult);
            if(_postResult.AnswerStatus.Pos("OK") != 0)
                retValue = true;
            else
            {
                MessageBox(_postResult.resultText,"Error",MB_OK + MB_ICONERROR);
                retValue = false;
            }
        }
        else
        {
            retValue= false;
            TDeviceRealTerminal::Instance().BasePMS->Enabled = false;
            if(TGlobalSettings::Instance().IsOraclePOSServer)
            {
                MessageBox("No Response Received from Oracle.\nPMS is disabled now.\nPlease ensure Oracle is up and working","Error",MB_OK + MB_ICONERROR);
                std::auto_ptr<TManagerOraclePMS> oraclePMS(new TManagerOraclePMS());
                oraclePMS->FindAndTerminateProcess();
            }
            else
            {
                MessageBox("No Response Received from Oracle.\nPMS is disabled now.\nPlease ensure POS Server and Oracle are up and working","Error",MB_OK + MB_ICONERROR);
            }
        }
    }
    catch(Exception &ex)
    {
        retValue= false;
        TDeviceRealTerminal::Instance().BasePMS->Enabled = false;
        if(TGlobalSettings::Instance().IsOraclePOSServer)
        {
            MessageBox("No Response Received from Oracle.\nPMS is disabled now\nPlease ensure Oracle is up and running","Error",MB_OK + MB_ICONERROR);
            std::auto_ptr<TManagerOraclePMS> oraclePMS(new TManagerOraclePMS());
            oraclePMS->FindAndTerminateProcess();
        }
        else
        {
            MessageBox("No Response Received from Oracle.\nPMS is disabled now.\nPlease ensure POS Server and Oracle are up and working","Error",MB_OK + MB_ICONERROR);
        }
    }
    return retValue;
}
//----------------------------------------------------------------------------
void TOracleDataBuilder::GetPMSPaymentType(std::map<int,TPMSPaymentType> &paymentMap)
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
AnsiString TOracleDataBuilder::GetPMSPaymentCode(TPayment *payment,std::map<int,TPMSPaymentType> paymentsMap)
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
AnsiString TOracleDataBuilder::GetPMSDefaultCode(std::map<int,TPMSPaymentType> paymentsMap)
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
AnsiString TOracleDataBuilder::DoRequiredInsertion(TPayment *payment, std::map<int,TPMSPaymentType> &paymentsMap)
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
void TOracleDataBuilder::AddPaymentToPMSPaymentTypes(TPayment *payment,AnsiString defaultCode)
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
