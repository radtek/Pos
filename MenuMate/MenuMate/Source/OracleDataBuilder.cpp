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
bool TOracleDataBuilder::CreatePostRoomInquiry(TPaymentTransaction &paymentTransaction)
{
    PostRoomInquiry.InquiryInformation = "123";
    PostRoomInquiry.MaximumReturnedMatches = "16";
    PostRoomInquiry.SequenceNumber = "1";
    PostRoomInquiry.RequestType = "";
    PostRoomInquiry.PaymentMethod = "123";
    PostRoomInquiry.Date = Now().FormatString( "YYMMDD");
    PostRoomInquiry.Time = Now().FormatString( "HHMMSS");
    PostRoomInquiry.RevenueCenter = TDeviceRealTerminal::Instance().BasePMS->POSID;
    PostRoomInquiry.WaiterId = paymentTransaction.StaffMember.Name;
    PostRoomInquiry.WorkstationId = TDeviceRealTerminal::Instance().ID.ComputerName;
}
bool TOracleDataBuilder::CreatePost(TPaymentTransaction &paymentTransaction)
{
    Currency priceExclusive = 0;
    Currency tax = 0;
    Currency serviceCharge = 0;
    Currency discount = 0;

    for(int i = 0; i < paymentTransaction.Orders->Count; i++)
    {
        TItemComplete *itemComplete = (TItemComplete*)paymentTransaction.Orders->Items[i];
        priceExclusive += itemComplete->BillCalcResult.FinalPrice -
                         itemComplete->BillCalcResult.ServiceCharge.Value -
                         itemComplete->BillCalcResult.TotalDiscount -
                         itemComplete->BillCalcResult.TotalTax;
        tax += itemComplete->BillCalcResult.TotalTax;
        serviceCharge += itemComplete->BillCalcResult.ServiceCharge.Value;
        discount += itemComplete->BillCalcResult.TotalDiscount;
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
    PostRequest.RoomNumber = "123";
    PostRequest.ReservationId = 101;
    PostRequest.ProfileId = "121";
    PostRequest.LastName = "Sharma";
    PostRequest.HotelId = "1";
    PostRequest.RequestType = "2";
    PostRequest.InquiryInformation = "test";
    PostRequest.MatchfromPostList = "1";
    PostRequest.SequenceNumber = "1";
    double totalValue = RoundTo((double)total,-2);
    totalValue = totalValue * 100;
    PostRequest.TotalAmount = totalValue;
    PostRequest.CreditLimitOverride = "N";
    PostRequest.PaymentMethod = paymentMethod;
    PostRequest.Covers = CurrToStrF(patronCount,ffFixed,0);
    PostRequest.RevenueCenter = TDeviceRealTerminal::Instance().BasePMS->POSID;
    PostRequest.ServingTime = mealName;//"BreakFast";
    PostRequest.CheckNumber = 3;
    double data = RoundTo((double)priceExclusive, -2);
    data = data * 100;
    PostRequest.Subtotal1.push_back(data);
    data = RoundTo((double)discount, -2);
    data = data * 100;
    PostRequest.Discount.push_back(data);
    data = RoundTo((double)tip, -2);
    data = data * 100;
	PostRequest.Tip = data;
    data = RoundTo((double)serviceCharge, -2);
    data = data * 100;
    PostRequest.ServiceCharge.push_back(data);
    data = RoundTo((double)tax, -2);
    data = data * 100;
	PostRequest.Tax.push_back(data);
    PostRequest.Date = Now().FormatString( "YYMMDD");
    PostRequest.Time = Now().FormatString( "HHMMSS");
    PostRequest.WaiterId = TDeviceRealTerminal::Instance().User.Name + " " +
                           TDeviceRealTerminal::Instance().User.Surname;
    PostRequest.WorkstationId = TDeviceRealTerminal::Instance().ID.ComputerName;

    CreateXML();
    return true;
}
//---------------------------------------------------------------------------------
void TOracleDataBuilder::CreateXML()
{
//	TiXmlDocument doc;
//
//	// add declaration
//	TiXmlDeclaration * decl = new TiXmlDeclaration(_T("1.0"), _T("UTF-8"), _T(""));
//	doc.LinkEndChild( decl );
//
//	// add root node ( Oracle )
//	TiXmlElement *rootNode = new TiXmlElement("PostRequest");
//	doc.LinkEndChild( rootNode );
//	AddInvoiceAttrs( rootNode);
//
//	//........................
//	if( true)
//	{
//		try
//		{
//		   AnsiString fileName = ExtractFilePath(Application->ExeName) + Now().FormatString("HHMMSS") + ".txt";
//           result = doc.SaveFile( fileName.c_str() );
//           if(result)
//             MessageBox("File exported","",MB_OK);
//           else
//             MessageBox("File not exported","",MB_OK);
//		}
//		catch( Exception &exc )
//		{
//            MessageBox(exc.Message,"",MB_OK);
//		}
//	}
//	else
//	{
//    }
}
//----------------------------------------------------------------------------
void TOracleDataBuilder::AddInvoiceAttrs(TiXmlElement *rootNode)
{
    SetNodeAttr( rootNode, "RoomNumber",            PostRequest.RoomNumber.c_str() );
    SetNodeAttr( rootNode, "ReservationId",         PostRequest.ReservationId.c_str() );
    SetNodeAttr( rootNode, "ProfileId",             PostRequest.ProfileId.c_str() );
    SetNodeAttr( rootNode, "LastName",             	PostRequest.LastName.c_str() );
    SetNodeAttr( rootNode, "HotelId",             	PostRequest.HotelId.c_str() );
    SetNodeAttr( rootNode, "RequestType",           PostRequest.RequestType.c_str() );
    SetNodeAttr( rootNode, "InquiryInformation",    PostRequest.InquiryInformation.c_str() );
    SetNodeAttr( rootNode, "MatchfromPostList",     PostRequest.MatchfromPostList.c_str() );
    SetNodeAttr( rootNode, "SequenceNumber",        PostRequest.SequenceNumber.c_str() );
    SetNodeAttr( rootNode, "TotalAmount",           PostRequest.TotalAmount.c_str() );
    SetNodeAttr( rootNode, "CreditLimitOverride",   PostRequest.CreditLimitOverride.c_str() );
    SetNodeAttr( rootNode, "PaymentMethod",         PostRequest.PaymentMethod.c_str() );
    SetNodeAttr( rootNode, "Covers",             	PostRequest.Covers.c_str() );
    SetNodeAttr( rootNode, "RevenueCenter",         PostRequest.RevenueCenter.c_str() );
    SetNodeAttr( rootNode, "ServingTime",           PostRequest.ServingTime.c_str() );
    SetNodeAttr( rootNode, "CheckNumber",           PostRequest.CheckNumber.c_str() );

    std::vector<AnsiString>::iterator itsubtotal =  PostRequest.Subtotal1.begin();
    if(PostRequest.Subtotal1.size() == 0)
    {
       SetNodeAttr( rootNode, "Subtotal1",             0 );
    }
    else
    {
//        int j = 1;
//        for(; itsubtotal != PostRequest.Subtotal1.end(); ++itsubtotal)
//        {
//            AnsiString nodeName = "Subtotal";
//            nodeName += j;
//            SetNodeAttr( rootNode, nodeName,             PostRequest.Subtotal1.c_str() );
//            j += 1;
//        }
       SetNodeAttr( rootNode, "Subtotal1",             PostRequest.Subtotal1[0].c_str() );
    }
    std::vector<AnsiString>::iterator itdiscount =  PostRequest.Discount.begin();
    if(PostRequest.Discount.size() == 0)
    {
       SetNodeAttr( rootNode, "Discount1",             0 );
    }
    else
    {
//        int j = 1;
//        for(; itdiscount != PostRequest.Discount.end(); ++itdiscount)
//        {
//            AnsiString nodeName = "Discount";
//            nodeName += j;
//            SetNodeAttr( rootNode, nodeName,             PostRequest.Discount[0] );
//            j += 1;
//        }
        SetNodeAttr( rootNode, "Discount1",            PostRequest.Discount[0].c_str() );
    }

    SetNodeAttr( rootNode, "Tip",         			PostRequest.Tip.c_str() );

    std::vector<AnsiString>::iterator itservicecharge =  PostRequest.ServiceCharge.begin();
    if(PostRequest.ServiceCharge.size() == 0)
    {
       SetNodeAttr( rootNode, "ServiceCharge1",             0 );
    }
    else
    {
//        int j = 1;
//        for(; itservicecharge != PostRequest.ServiceCharge.end(); ++itservicecharge)
//        {
//            AnsiString nodeName = "ServiceCharge";
//            nodeName += j;
//            SetNodeAttr( rootNode, nodeName,             PostRequest.Tip[0] );
//            j += 1;
//        }
        SetNodeAttr( rootNode, "ServiceCharge1",             PostRequest.ServiceCharge[0].c_str() );
    }

    std::vector<AnsiString>::iterator itTax =  PostRequest.Tax.begin();
    if(PostRequest.Tax.size() == 0)
    {
       SetNodeAttr( rootNode, "Tax1",            0 );
    }
    else
    {
//        int j = 1;
//        for(; itTax != PostRequest.Tax.end(); ++itTax)
//        {
//            AnsiString nodeName = "Tax";
//            nodeName += j;
//            SetNodeAttr( rootNode, nodeName,             PostRequest.Tax[0] );
//            j += 1;
//        }
       SetNodeAttr( rootNode, "Tax1",            PostRequest.Tax[0].c_str()  );
    }

    SetNodeAttr( rootNode, "Date",              	PostRequest.Date.c_str() );
    SetNodeAttr( rootNode, "Time",              	PostRequest.Time.c_str() );
    SetNodeAttr( rootNode, "WaiterId",             	PostRequest.WaiterId.c_str() );
    SetNodeAttr( rootNode, "WorkstationId",         PostRequest.WorkstationId.c_str() );
}
//----------------------------------------------------------------------------
void TOracleDataBuilder::SetNodeAttr(TiXmlElement *inNode, AnsiString inAttrName, AnsiString inAttrValue)
{
    inNode->SetAttribute( inAttrName.c_str(), inAttrValue.c_str() );
}
