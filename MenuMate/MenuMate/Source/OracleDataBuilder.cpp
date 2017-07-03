//---------------------------------------------------------------------------


#pragma hdrstop

#include "OracleDataBuilder.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TOracleDataBuilder::TOracleDataBuilder()
{

}
//---------------------------------------------------------------------------
TOracleDataBuilder::~TOracleDataBuilder()
{
}
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
        postRoomInquiry.PaymentMethod = "";
        postRoomInquiry.Date = Now().FormatString( "YYMMDD");
        postRoomInquiry.Time = Now().FormatString( "HHMMSS");
        postRoomInquiry.RevenueCenter = TDeviceRealTerminal::Instance().BasePMS->RevenueCentre;
        postRoomInquiry.WaiterId = TDeviceRealTerminal::Instance().User.Name + " " +
                                   TDeviceRealTerminal::Instance().User.Surname;
        postRoomInquiry.WorkstationId = TDeviceRealTerminal::Instance().BasePMS->POSID;
        DBTransaction.Commit();
    }
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        DBTransaction.Rollback();
	}
}
void TOracleDataBuilder::CreatePost(TPaymentTransaction &paymentTransaction, TPostRequest &postRequest)
{
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
    try
    {
        Currency priceExclusive = 0;
        Currency tax = 0;
        Currency serviceCharge = 0;
        Currency discount = 0;
        std::map<int, double> discMap;
        discMap.clear();
        std::vector<TTax> taxVector;
        taxVector.clear();
        for(int i = 0; i < paymentTransaction.Orders->Count; i++)
        {
            TItemComplete *itemComplete = (TItemComplete*)paymentTransaction.Orders->Items[i];

            priceExclusive += itemComplete->BillCalcResult.FinalPrice -
                              itemComplete->BillCalcResult.ServiceCharge.Value -
                              itemComplete->BillCalcResult.TotalDiscount -
                              itemComplete->BillCalcResult.TotalTax;
//            tax += itemComplete->BillCalcResult.TotalTax;
            serviceCharge += itemComplete->BillCalcResult.ServiceCharge.Value;

//            discount += itemComplete->BillCalcResult.TotalDiscount;
            // calculation of taxes
            ExtractTaxes(itemComplete,taxVector);
            // calculation for discount
            ExtractDiscount(discMap,itemComplete);
        }

        Currency total = priceExclusive + tax + serviceCharge + discount;
        AnsiString paymentMethod = "";

        for(int i = 0; i < paymentTransaction.PaymentsCount(); i++)
        {
            TPayment *payment = paymentTransaction.PaymentGet(i);
            if(payment->GetPayTendered() + payment->GetCashOut() - payment->GetChange() != 0)
            {
               paymentMethod = payment->PaymentThirdPartyID;
               break;
            }
        }

        Currency tip = 0;
        for(int i = 0; i < paymentTransaction.PaymentsCount(); i++)
        {
            TPayment *payment = paymentTransaction.PaymentGet(i);
            if(payment->GetPaymentAttribute(ePayTypeCustomSurcharge))
               tip += payment->GetAdjustment();
            tip += payment->TipAmount;
        }

        Currency patronCount = 0;
        std::vector<TPatronType>::iterator itPatron = paymentTransaction.Patrons.begin();
        for(;itPatron != paymentTransaction.Patrons.end(); ++itPatron)
        {
            patronCount += itPatron->Count;
        }
        AnsiString mealName = "Default";
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
        std::auto_ptr<TOracleManagerDB> managerDB(new TOracleManagerDB());
        AnsiString checkNumber = managerDB->GetCheckNumber(DBTransaction);

        postRequest.RoomNumber = paymentTransaction.PMSClientDetails.RoomNumber;
        postRequest.ReservationId = paymentTransaction.PMSClientDetails.ReservationID;
        postRequest.ProfileId = paymentTransaction.PMSClientDetails.ProfileID;
        postRequest.LastName = paymentTransaction.PMSClientDetails.LastName;
        postRequest.HotelId = paymentTransaction.PMSClientDetails.HotelID;
        postRequest.RequestType = "4";
        postRequest.InquiryInformation = paymentTransaction.PMSClientDetails.RoomNumber;
        postRequest.MatchfromPostList = paymentTransaction.PMSClientDetails.MatchIdentifier;
        postRequest.SequenceNumber = paymentTransaction.PMSClientDetails.SequenceNumber;
        DBTransaction.Commit();
        double totalValue = RoundTo((double)total,-2);
        totalValue = totalValue * 100;
        postRequest.TotalAmount = totalValue;
        postRequest.CreditLimitOverride = "N";
        postRequest.PaymentMethod = paymentMethod;
        postRequest.Covers = CurrToStrF(patronCount,ffFixed,0);
        postRequest.RevenueCenter = TDeviceRealTerminal::Instance().BasePMS->RevenueCentre;
        postRequest.ServingTime = mealName;//"BreakFast";
        postRequest.CheckNumber = checkNumber;
        double data = RoundTo((double)priceExclusive, -2);
        data = data * 100;
        postRequest.Subtotal1.push_back(data);
//        data = RoundTo((double)discount, -2);
//        data = data * 100;
//        postRequest.Discount.push_back(data);
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
        data = RoundTo((double)tip, -2);
        data = data * 100;
        postRequest.Tip = data;
        data = RoundTo((double)serviceCharge, -2);
        data = data * 100;
        postRequest.ServiceCharge.push_back(data);
//        data = RoundTo((double)tax, -2);
//        data = data * 100;
////        postRequest.Tax.push_back(data);
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
        DBTransaction.Rollback();
	}
}
//----------------------------------------------------------------------------
TiXmlDocument TOracleDataBuilder::CreateRoomInquiryXML(TPostRoomInquiry &postRequest)
{
	TiXmlDocument doc;

	// add declaration
	TiXmlDeclaration * decl = new TiXmlDeclaration(_T("1.0"), _T("UTF-8"), _T(""));
	doc.LinkEndChild( decl );

	// add root node ( Oracle )
	TiXmlElement *rootNode = new TiXmlElement("PostRequest");
	doc.LinkEndChild( rootNode );
	AddInvoiceAttrs( rootNode,postRequest);
//
//	//........................
    try
    {
       AnsiString fileName = ExtractFilePath(Application->ExeName) +"\\"+ "Oracle Room Inquiry\\" + "RoomInquiry@ "+Now().FormatString("HHMMSS") + ".txt";
       bool result = doc.SaveFile( fileName.c_str() );
    }
    catch( Exception &exc )
    {
        MessageBox(exc.Message,"",MB_OK);
    }
    return doc;
}
//----------------------------------------------------------------------------
TiXmlDocument TOracleDataBuilder::CreatePostXML(TPostRequest &postRequest)
{
	TiXmlDocument doc;

	// add declaration
	TiXmlDeclaration * decl = new TiXmlDeclaration(_T("1.0"), _T("UTF-8"), _T(""));
	doc.LinkEndChild( decl );

	// add root node ( Oracle )
	TiXmlElement *rootNode = new TiXmlElement("PostRequest");
	doc.LinkEndChild( rootNode );
	AddInvoiceAttrs( rootNode,postRequest);

//	//........................
    try
    {
       AnsiString fileName = ExtractFilePath(Application->ExeName) + "\\" + "Oracle Room Post\\" + "Room Post@ "+Now().FormatString("HHMMSS") + ".txt";
       bool result = doc.SaveFile( fileName.c_str() );
//       if(result)
//         MessageBox("File exported","",MB_OK);
//       else
//         MessageBox("File not exported","",MB_OK);
    }
    catch( Exception &exc )
    {
        MessageBox(exc.Message,"",MB_OK);
    }
    return doc;
}
//----------------------------------------------------------------------------
void TOracleDataBuilder::AddInvoiceAttrs(TiXmlElement *rootNode,TPostRoomInquiry &postRequest)
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
//----------------------------------------------------------------------------
void TOracleDataBuilder::AddInvoiceAttrs(TiXmlElement *rootNode,TPostRequest &postRequest)
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
//        int j = 1;
//        for(; itsubtotal != postRequest.Subtotal1.end(); ++itsubtotal)
//        {
//            AnsiString nodeName = "Subtotal";
//            nodeName += j;
//            SetNodeAttr( rootNode, nodeName,             postRequest.Subtotal1.c_str() );
//            j += 1;
//        }
       SetNodeAttr( rootNode, "Subtotal1",             postRequest.Subtotal1[0].c_str() );
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
            SetNodeAttr( rootNode, nodeName,             postRequest.Discount[i] );
            j += 1;
            i = +1;
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
//        int j = 1;
//        for(; itservicecharge != postRequest.ServiceCharge.end(); ++itservicecharge)
//        {
//            AnsiString nodeName = "ServiceCharge";
//            nodeName += j;
//            SetNodeAttr( rootNode, nodeName,             postRequest.Tip[0] );
//            j += 1;
//        }
        SetNodeAttr( rootNode, "ServiceCharge1",             postRequest.ServiceCharge[0].c_str() );
    }
    std::vector<AnsiString>::iterator itTax =  postRequest.Tax.begin();
    if(postRequest.Tax.size() == 0)
    {
       SetNodeAttr( rootNode, "Tax2222",            0 );
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
//----------------------------------------------------------------------------
void TOracleDataBuilder::SetNodeAttr(TiXmlElement *inNode, AnsiString inAttrName, AnsiString inAttrValue)
{
    inNode->SetAttribute( inAttrName.c_str(), inAttrValue.c_str() );
}
//----------------------------------------------------------------------------
TRoomInquiryResult TOracleDataBuilder::createXMLInquiryDoc()
{
	TiXmlDocument* result = new TiXmlDocument();
    TiXmlDeclaration * decl = new TiXmlDeclaration(_T("1.0"), _T("UTF-8"), _T(""));
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
//        TiXmlElement *itPtr = _itemsElem->FirstChildElement();
//        for(int i = 1; i < countOfGuest; i++)
//        {
//            TRoomInquiryItem inquiryItems;
//            inquiryItems.RoomNumber = itPtr->Attribute("RoomNumber");
//            inquiryItems.ReservationId = itPtr->Attribute("ReservationId");
//            inquiryItems.LastName = itPtr->Attribute("LastName");
//            inquiryItems.FirstName = itPtr->Attribute("FirstName");
//            inquiryItems.Title = itPtr->Attribute("Title");
//            inquiryItems.NoPost = itPtr->Attribute("NoPost");
//            inquiryItems.CreditLimit = itPtr->Attribute("CreditLimit");
//            inquiryItems.ProfileId = itPtr->Attribute("ProfileId");
//            inquiryItems.HotelId = itPtr->Attribute("HotelId");
//            MessageBox(inquiryItems.FirstName,"Element",MB_OK);
//            roomInquiryResult->RoomInquiryItem.push_back(inquiryItems);
//            itPtr = itPtr->NextSiblingElement();
//        }
    }
    else if(value == "PostAnswer")
    {
        roomInquiryResult.IsSuccessful = false;
        roomInquiryResult.resultText = _rootElem->Attribute("ResponseText");
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
	catch( ... )
	{
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
	catch( ... )
	{
		//throw Exception( "Invalid menu version: " + VERSION + " Element not found: " + "root" );
	}

	//throw Exception( "Invalid menu version: " + VERSION + ". Element not found: " + inElemName );

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
	catch( ... )
	{
	}

	//::::::::::::::::::::::::::::::::

	return result;
}
//----------------------------------------------------------------------------
void TOracleDataBuilder::ExtractDiscount(std::map<int, double> &discMap, TItemComplete *itemComplete)
{
    for(int i = 0; i < itemComplete->BillCalcResult.Discount.size(); i++)
    {
        if(discMap.size() == 0)
        {
            discMap.insert(std::pair<int,double>(itemComplete->BillCalcResult.Discount[i].DiscountKey,
                                                 itemComplete->BillCalcResult.Discount[i].Value));
        }
        else
        {
            std::map<int,double>::iterator itDisc = discMap.find(itemComplete->BillCalcResult.Discount[i].DiscountKey);
            if(itDisc != discMap.end())
            {
                double newValue = itDisc->second + double(fabs(itemComplete->BillCalcResult.Discount[i].Value));
                discMap[itemComplete->BillCalcResult.Discount[i].DiscountKey] = newValue;
            }
            else
            {
                discMap.insert(std::pair<int,double>(itemComplete->BillCalcResult.Discount[i].DiscountKey,
                             fabs(itemComplete->BillCalcResult.Discount[i].Value)));
            }
        }
    }
}
//----------------------------------------------------------------------------
void TOracleDataBuilder::ExtractSubTotal()
{
}
//----------------------------------------------------------------------------
void TOracleDataBuilder::ExtractTaxes(TItemComplete *itemComplete,std::vector<TTax> &taxVector)
{
    for(int j = 0; j < itemComplete->BillCalcResult.Tax.size(); j++)
    {
        if(taxVector.size() == 0)
        {
            TTax tax;
            tax.Name = itemComplete->BillCalcResult.Tax[j].Name;
            tax.Type = (int)itemComplete->BillCalcResult.Tax[j].TaxType;
            tax.Percentage = itemComplete->BillCalcResult.Tax[j].Percentage;
            tax.Value = itemComplete->BillCalcResult.Tax[j].Value;
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
                    tax.Value = itemComplete->BillCalcResult.Tax[j].Value;
                }
                else
                {
                    existingTax = true;
                    double taxValue = taxVector[ptr].Value;
                    taxValue += (double)itemComplete->BillCalcResult.Tax[j].Value;
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
//    return taxVector;
}
//----------------------------------------------------------------------------
void TOracleDataBuilder::ExtractServiceCharge()
{
}
//----------------------------------------------------------------------------
