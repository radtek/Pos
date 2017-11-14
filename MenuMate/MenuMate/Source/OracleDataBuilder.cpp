//---------------------------------------------------------------------------


#pragma hdrstop

#include "OracleDataBuilder.h"
#include "OracleManagerDB.h"
#include "ManagerPMSCodes.h"
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
    try
    {
        std::auto_ptr<TOracleManagerDB> managerDB(new TOracleManagerDB());
        postRoomInquiry.SequenceNumber = managerDB->GetSequenceNumber(DBTransaction);
        postRoomInquiry.MaximumReturnedMatches = "16";
        postRoomInquiry.RequestType = "4";
        postRoomInquiry.PaymentMethod = "Room";
        postRoomInquiry.Date = Now().FormatString( "YYMMDD");
        postRoomInquiry.Time = Now().FormatString( "HHMMSS");
        postRoomInquiry.RevenueCenter = TDeviceRealTerminal::Instance().BasePMS->RevenueCentre;
        postRoomInquiry.WaiterId = TDeviceRealTerminal::Instance().User.Name + " " +
//                                   TDeviceRealTerminal::Instance().User.Surname;
        postRoomInquiry.WorkstationId = TDeviceRealTerminal::Instance().BasePMS->POSID;
        DBTransaction.Commit();
    }
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        DBTransaction.Rollback();
	}
}
//----------------------------------------------------------------------------
/* The method runs for one payment type and extracts the details depending upon the portion
of amount received through that particular payment type*/
//----------------------------------------------------------------------------
TPostRequest TOracleDataBuilder::CreatePost(TPaymentTransaction &paymentTransaction, double portion, int paymentIndex, double tip)
{
    TPostRequest postRequest;
    try
    {
        double serviceCharge = 0;
        Currency discount = 0;
        std::map<int, double> discMap;
        discMap.clear();
        std::vector<TTax> taxVector;
        std::map<int,double> subtotals;
        taxVector.clear();
        subtotals.clear();
        for(int i = 0; i < paymentTransaction.Orders->Count; i++)
        {
            TItemComplete *itemComplete = (TItemComplete*)paymentTransaction.Orders->Items[i];
            // calculation of subtotal
            ExtractSubTotal(subtotals,discMap, itemComplete, portion);
            // calculation of taxes
            if(!TGlobalSettings::Instance().ItemPriceIncludeTax)
                ExtractTaxes(itemComplete,taxVector,portion);
            // calculation of service Charge
            if(!TGlobalSettings::Instance().ItemPriceIncludeServiceCharge)
                serviceCharge += (double)itemComplete->BillCalcResult.ServiceCharge.Value;
            for(int k = 0; k < itemComplete->SubOrders->Count; k++)
            {
                TItemComplete *itemCompleteSub = (TItemComplete*)itemComplete->SubOrders->Items[k];
                // calculation of subtotal
                ExtractSubTotal(subtotals,discMap, itemCompleteSub, portion);
                // calculation of taxes
                if(!TGlobalSettings::Instance().ItemPriceIncludeTax)
                    ExtractTaxes(itemCompleteSub,taxVector,portion);
                // calculation of service Charge
                if(!TGlobalSettings::Instance().ItemPriceIncludeServiceCharge)
                    serviceCharge += (double)itemCompleteSub->BillCalcResult.ServiceCharge.Value;
            }
        }

        AnsiString paymentMethod = "";

        TPayment *payment = paymentTransaction.PaymentGet(paymentIndex);
        paymentMethod = payment->PaymentThirdPartyID;


        double total = CalculateTotal(subtotals,taxVector,discMap);
        total += serviceCharge;
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
        postRequest.SequenceNumber = paymentTransaction.PMSClientDetails.SequenceNumber;
        double totalValue = RoundTo((double)total,-2);
        totalValue = totalValue * 100;
        postRequest.TotalAmount = totalValue;
        postRequest.CreditLimitOverride = "N";
        postRequest.PaymentMethod = paymentMethod;
        postRequest.Covers = CurrToStrF(patronCount,ffFixed,0);
        postRequest.RevenueCenter = TDeviceRealTerminal::Instance().BasePMS->RevenueCentre;
        postRequest.ServingTime = mealName;//"BreakFast";
        tip = RoundTo((double)tip, -2);
        tip = tip * 100;
        postRequest.Tip = tip;
        double data = 0;

        std::map<int,double>::iterator itsubtotals =  subtotals.begin();
        for(;itsubtotals != subtotals.end(); advance(itsubtotals,1))
        {
           double value = RoundTo((double)itsubtotals->second, -2) * 100;
           AnsiString str = (AnsiString)value;
           if(str.Pos(".") != 0)
           {
              str = str.SubString(1,str.Pos(".")-1);
           }
           postRequest.Subtotal1.push_back(str);
        }

        std::map<int,double>::iterator itDisc =  discMap.begin();
        for(;itDisc != discMap.end(); advance(itDisc,1))
        {
           double value = RoundTo((double)itDisc->second, -2) * 100;
           AnsiString str = (AnsiString)value;
           if(str.Pos(".") != 0)
           {
              str = str.SubString(1,str.Pos(".")-1);
           }
           postRequest.Discount.push_back(str);
        }

        data = RoundTo((double)serviceCharge, -2);
        data = data * 100;
        postRequest.ServiceCharge.push_back(data);
        for(int i = 0; i < taxVector.size(); i++)
        {
            double value = RoundTo(taxVector[i].Value * 100,-2);
            AnsiString str = (AnsiString)value;
            if(str.Pos(".") != 0)
            {
                str = str.SubString(1,str.Pos(".")-1);
            }
            postRequest.Tax.push_back(str);
        }
        postRequest.Date = Now().FormatString( "YYMMDD");
        postRequest.Time = Now().FormatString( "HHMMSS");
        postRequest.WaiterId = TDeviceRealTerminal::Instance().User.Name + " " +
                               TDeviceRealTerminal::Instance().User.Surname;
        postRequest.WorkstationId = TDeviceRealTerminal::Instance().BasePMS->POSID;
    }
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        MessageBox(E.Message,"Exception in creating XML",MB_OK);
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
            for(; itsubtotal != postRequest.Subtotal1.end(); ++itsubtotal)
            {
                AnsiString nodeName = "Subtotal";
                nodeName += j;
                SetNodeAttr( rootNode, nodeName,             postRequest.Subtotal1[i].c_str() );
                i += 1;
                j += 1;
            }
//           SetNodeAttr( rootNode, "Subtotal1",             postRequest.Subtotal1[0].c_str() );
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
            for(; itdiscount != postRequest.Discount.end(); ++itdiscount)
            {
                AnsiString nodeName = "Discount";
                nodeName += j;
                SetNodeAttr( rootNode, nodeName,             postRequest.Discount[i].c_str() );
                j += 1;
                i += 1;
            }
    //        SetNodeAttr( rootNode, "Discount1",            postRequest.Discount[i].c_str() );
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
            for(; itservicecharge != postRequest.ServiceCharge.end(); ++itservicecharge)
            {
                AnsiString nodeName = "ServiceCharge";
                nodeName += j;
                SetNodeAttr( rootNode, nodeName,             postRequest.ServiceCharge[i].c_str() );
                j += 1;
            }
//            SetNodeAttr( rootNode, "ServiceCharge1",             postRequest.ServiceCharge[0].c_str() );
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
            for(; itTax != postRequest.Tax.end(); ++itTax)
            {
                AnsiString nodeName = "Tax";
                nodeName += j;
                SetNodeAttr( rootNode, nodeName,             postRequest.Tax[i].c_str());
                j += 1;
                i += 1;
            }
    //       SetNodeAttr( rootNode, "Tax1",            postRequest.Tax[0].c_str()  );
        }

        SetNodeAttr( rootNode, "Date",              	postRequest.Date.c_str() );
        SetNodeAttr( rootNode, "Time",              	postRequest.Time.c_str() );
        SetNodeAttr( rootNode, "WaiterId",             	postRequest.WaiterId.c_str() );
        SetNodeAttr( rootNode, "WorkstationId",         postRequest.WorkstationId.c_str() );
    }
    catch( Exception &E )
    {
        MessageBox(E.Message,"Exception in add attributes",MB_OK);
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
    //std::auto_ptr<TRoomInquiryResult> roomInquiryResult (new TRoomInquiryResult());
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
//        roomInquiryResult.PathId = _rootElem->Attribute("PathId");
//        roomInquiryResult.PostNum = _rootElem->Attribute("PostNum");
        roomInquiryResult.HotelId = _rootElem->Attribute("HotelId");
        roomInquiryResult.PaymentMethod = _rootElem->Attribute("PaymentMethod");
        roomInquiryResult.RevenueCenter = _rootElem->Attribute("PaymentMethod");
//        roomInquiryResult.SalesOutlet = _rootElem->Attribute("SalesOutlet");
        roomInquiryResult.WaiterId = _rootElem->Attribute("WaiterId");
//        roomInquiryResult.UserID = _rootElem->Attribute("UserID");
        roomInquiryResult.WorkstationId = _rootElem->Attribute("WorkstationId");
//        roomInquiryResult.WSNum = _rootElem->Attribute("WSNum");
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
//                    inquiryItems.GuestNum = itPtr->Attribute("GuestNum");
                    inquiryItems.LastName = itPtr->Attribute("LastName");
//                    inquiryItems.GuestName = itPtr->Attribute("GuestName");
                    inquiryItems.FirstName = itPtr->Attribute("FirstName");
//                    inquiryItems.GuestFirstname = itPtr->Attribute("GuestFirstname");
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
//    for(int i = 0; i < itemComplete->BillCalcResult.Discount.size(); i++)
//    {
//        if(discMap.size() == 0)
//        {
//            discMap.insert(std::pair<int,double>(itemComplete->BillCalcResult.Discount[i].DiscountKey,
//                                                 itemComplete->BillCalcResult.Discount[i].Value * portion));
//        }
//        else
//        {
//            std::map<int,double>::iterator itDisc = discMap.find(itemComplete->BillCalcResult.Discount[i].DiscountKey);
//            if(itDisc != discMap.end())
//            {
//                double newValue = itDisc->second + double(fabs(itemComplete->BillCalcResult.Discount[i].Value * portion));
//                discMap[itemComplete->BillCalcResult.Discount[i].DiscountKey] = newValue;
//            }
//            else
//            {
//                discMap.insert(std::pair<int,double>(itemComplete->BillCalcResult.Discount[i].DiscountKey,
//                             fabs(itemComplete->BillCalcResult.Discount[i].Value * portion)));
//            }
//        }
//    }
}
//----------------------------------------------------------------------------
void TOracleDataBuilder::ExtractSubTotal(std::map<int,double> &subtotals, std::map<int, double> &discMap,
                                         TItemComplete *itemComplete, double portion)
{
    double priceExclusive = 0;
//    priceExclusive = ((double)itemComplete->BillCalcResult.FinalPrice -
//                      (double)itemComplete->BillCalcResult.ServiceCharge.Value -
//                      (double)itemComplete->BillCalcResult.TotalDiscount -
//                      (double)itemComplete->BillCalcResult.TotalTax);
    priceExclusive = ((double)itemComplete->BillCalcResult.FinalPrice -
                      (double)itemComplete->BillCalcResult.TotalDiscount);
//    MessageBox(priceExclusive, "priceExclusive discount removed only",MB_OK);
    if(!TGlobalSettings::Instance().ItemPriceIncludeServiceCharge)
        priceExclusive = priceExclusive - (double)itemComplete->BillCalcResult.ServiceCharge.Value;
//    MessageBox(priceExclusive, "priceExclusive service charge removed only",MB_OK);
    if(!TGlobalSettings::Instance().ItemPriceIncludeTax)
        priceExclusive = priceExclusive - (double)itemComplete->BillCalcResult.TotalTax;
//    MessageBox(priceExclusive, "priceExclusive tax removed only",MB_OK);
    priceExclusive = priceExclusive * portion;

    if(subtotals.size() == 0)
    {
        subtotals.insert(std::pair<int,double>(itemComplete->RevenueCode,priceExclusive));
    }
    else
    {
        std::map<int,double>::iterator itsubtotals = subtotals.find(itemComplete->RevenueCode);
        if(itsubtotals != subtotals.end())
        {
            priceExclusive += itsubtotals->second;
            subtotals[itemComplete->RevenueCode] = priceExclusive;
        }
        else
        {
            subtotals.insert(std::pair<int,double>(itemComplete->RevenueCode,priceExclusive));
        }
    }

//    for(int i = 0; i < itemComplete->BillCalcResult.Discount.size(); i++)
//    {
        if(discMap.size() == 0)
        {
            discMap.insert(std::pair<int,double>(itemComplete->RevenueCode,
                                                 (double)(itemComplete->BillCalcResult.TotalDiscount * portion)));
        }
        else
        {
            std::map<int,double>::iterator itDisc = discMap.find(itemComplete->RevenueCode);
            if(itDisc != discMap.end())
            {
                double newValue = itDisc->second + double((itemComplete->BillCalcResult.TotalDiscount * portion));
                discMap[itemComplete->RevenueCode] = newValue;
            }
            else
            {
                discMap.insert(std::pair<int,double>(itemComplete->RevenueCode,
                             double(itemComplete->BillCalcResult.TotalDiscount * portion)));
            }
        }
//    }

//    MessageBox(subtotals.size(),"size of subtotals", MB_OK);
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
double TOracleDataBuilder::CalculateTotal(std::map<int,double> subtotals,std::vector<TTax> taxVector,
                                            std::map<int, double> discMap)
{
    double totalValue = 0;
    for(int i = 0; i < taxVector.size(); i++)
    {
        totalValue += taxVector[i].Value;
    }
    for(std::map<int,double>::iterator itdisc = discMap.begin(); itdisc != discMap.end();advance(itdisc,1))
    {
        totalValue += itdisc->second;
    }
    for(std::map<int,double>::iterator itsubtotals = subtotals.begin(); itsubtotals != subtotals.end();advance(itsubtotals,1))
    {
        totalValue += itsubtotals->second;
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

    if(inData.Length() == 0)
        MessageBox("Oracle PMS was not enabled because no response was received.","Error", MB_OK+MB_ICONERROR);
    else if(!retValue)
        MessageBox(inData,"Error", MB_OK+MB_ICONERROR);

    return (inData.Pos("LinkAlive") != 0);
}
//----------------------------------------------------------------------------
bool TOracleDataBuilder::DeserializeInquiryData(AnsiString inData, TRoomInquiryResult &_roomResult)
{
    TiXmlDocument* result = new TiXmlDocument();
    result->Parse(inData.c_str());
    ReadXML(result,_roomResult);
    return true;
}
//----------------------------------------------------------------------------
bool TOracleDataBuilder::DeserializeData(AnsiString inData, TPostRequestAnswer &_postResult)
{
    TiXmlDocument* result = new TiXmlDocument();
    result->Parse(inData.c_str());
    ReadXML(result,_postResult);
    if(_postResult.AnswerStatus.Pos("OK") != 0)
        return true;
    else
    {
        MessageBox(_postResult.resultText,"Error",MB_OK + MB_ICONERROR);
        return false;
    }
}
//----------------------------------------------------------------------------

