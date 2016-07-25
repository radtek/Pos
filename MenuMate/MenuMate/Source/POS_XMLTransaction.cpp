// ---------------------------------------------------------------------------

#pragma hdrstop

#include "POS_XMLTransaction.h"
#include "DeviceRealTerminal.h"
#include "ManagerPatron.h"
// ---------------------------------------------------------------------------

#pragma package(smart_init)

// ---------------------------------------------------------------------------

TPOS_XMLTransaction::TPOS_XMLTransaction(AnsiString TransID) : TPOS_XMLBase(TransID)
{
	Cache = true;
}

void TPOS_XMLTransaction::Build(TPaymentTransaction &PaymentTransaction)
{
	// Update the IntaMate ID with the Invoice Number.
	IntaMateID = PaymentTransaction.InvoiceNumber + FormatFloat("0000", TGlobalSettings::Instance().SiteID);
	Doc.Clear();
	TiXmlDeclaration * decl = new TiXmlDeclaration(_T("1.0"), _T("UTF-8"), _T(""));
	Doc.LinkEndChild(decl);
	// Insert DOCTYPE definiation here.
	TiXmlElement * Transaction = new TiXmlElement(_T("Transaction"));
	Transaction->SetAttribute(xmlAttrID, AnsiString(IntaMateID).c_str());
	Doc.LinkEndChild(Transaction);

	BuildTransaction(PaymentTransaction, Transaction);
}

void TPOS_XMLTransaction::BuildTransaction(TPaymentTransaction &PaymentTransaction, TiXmlElement *Transaction)
{
	TiXmlElement *XMLVersion = new TiXmlElement(xmlEleXMLVersion);
	XMLVersion->SetAttribute(xmlAttrVERSION, _T("1.0"));
	Transaction->LinkEndChild(XMLVersion);

	TiXmlElement *ProductVersion = new TiXmlElement(xmlEleProductVersion);
	ProductVersion->SetAttribute(xmlAttrPRODUCT, TDeviceRealTerminal::Instance().ID.Product.t_str());
	ProductVersion->SetAttribute(xmlAttrVERSION, TDeviceRealTerminal::Instance().OS.MMSoftwareVersion.t_str());
	Transaction->LinkEndChild(ProductVersion);

	TiXmlElement *Terminal = new TiXmlElement(xmlEleTerminal);
	Terminal->SetAttribute(xmlAttrTerminalID, TDeviceRealTerminal::Instance().IMManager->POSID);
	Terminal->SetAttribute(xmlAttrTerminalName, TDeviceRealTerminal::Instance().ID.ComputerName.t_str());
	Terminal->SetAttribute(xmlAttrLocation, TDeviceRealTerminal::Instance().ID.Location.t_str());
	Terminal->SetAttribute(xmlAttrSiteID, TGlobalSettings::Instance().SiteID);
	Transaction->LinkEndChild(Terminal);

	TiXmlElement *Sender = new TiXmlElement(xmlEleSender);
	Sender->SetAttribute(xmlAttrID, TDeviceRealTerminal::Instance().User.ContactID);
	Sender->SetAttribute(xmlAttrXmlID, TDeviceRealTerminal::Instance().User.ContactKey);
	Sender->SetAttribute(xmlAttrName, TDeviceRealTerminal::Instance().User.Name.t_str());
	Sender->SetAttribute(xmlAttrPayrollID, TDeviceRealTerminal::Instance().User.PayRollID.t_str());
	Sender->SetAttribute(xmlAttrInitials, TDeviceRealTerminal::Instance().User.Initials.t_str());
	Transaction->LinkEndChild(Sender);

	TiXmlElement *Destination = new TiXmlElement(xmlEleDestination);
	Destination->SetAttribute(xmlAttrName, _T("Unknown"));
	Destination->SetAttribute(xmlAttrTo, _T("Unknown"));
	Destination->SetAttribute(xmlAttrFolio, _T("Unknown"));
	Transaction->LinkEndChild(Destination);

	BuildReceipt(PaymentTransaction, Transaction);

}

void TPOS_XMLTransaction::BuildReceipt(TPaymentTransaction &PaymentTransaction, TiXmlElement *Transaction)
{
	TiXmlElement *Receipt = new TiXmlElement(xmlEleReceipt);
	Receipt->SetAttribute(xmlAttrReceiptNo, PaymentTransaction.InvoiceNumber.t_str());
	Receipt->SetAttribute(xmlAttrTransactionType, PaymentTransaction.CreditTransaction ? _T("Refund") : _T("Purchase"));
	Receipt->SetAttribute(xmlAttrDate, Now().FormatString("dd/mm/yyyy").t_str());
	Receipt->SetAttribute(xmlAttrTime, Now().FormatString("hh:nn:ss").t_str());
   Receipt->SetAttribute(xmlAttrTotalTendered, FormatFloat("0.00", PaymentTransaction.Money.TotalTendered).t_str());
	Receipt->SetAttribute(xmlAttrRounding, FormatFloat("0.00", PaymentTransaction.Money.TotalRounding).t_str());

   Receipt->SetAttribute(xmlAttrTotal, FormatFloat("0.00", PaymentTransaction.Money.GrandTotal).t_str());
   Receipt->SetAttribute(xmlAttrTotalXTax, FormatFloat("0.00", PaymentTransaction.Money.GrandTotalExclGST).t_str());
   Receipt->SetAttribute(xmlAttrTotalTax, FormatFloat("0.00", PaymentTransaction.Money.GrandTotalGSTContent).t_str());

   Receipt->SetAttribute(xmlAttrAdjustment, FormatFloat("0.00", PaymentTransaction.Money.TotalAdjustment).t_str());
   Receipt->SetAttribute(xmlAttrAdjustmentXTax, FormatFloat("0.00", PaymentTransaction.Money.TotalAdjustmentExcGST).t_str());
   Receipt->SetAttribute(xmlAttrAdjustmentTax, FormatFloat("0.00", PaymentTransaction.Money.TotalAdjustmentGSTContent).t_str());
   Receipt->SetAttribute(xmlAttrTotalDiscount, FormatFloat("0.00", PaymentTransaction.Money.Discount).t_str());
   Receipt->SetAttribute(xmlAttrTotalSurcharge, FormatFloat("0.00", PaymentTransaction.Money.Surcharge).t_str());
   Receipt->SetAttribute(xmlAttrTotalSurchargeXTax, FormatFloat("0.00", PaymentTransaction.Money.SurchargeExcGST).t_str());
   Receipt->SetAttribute(xmlAttrTotalSurchargeTax, FormatFloat("0.00", PaymentTransaction.Money.SurchargeGSTContent).t_str());
   Receipt->SetAttribute(xmlAttrSummaGross, FormatFloat("0.00", PaymentTransaction.Money.Total + PaymentTransaction.Money.Surcharge).t_str());
   Receipt->SetAttribute(xmlAttrSummaNet, FormatFloat("0.00",  PaymentTransaction.Money.Total + PaymentTransaction.Money.Surcharge + PaymentTransaction.Money.Discount).t_str());
   Receipt->SetAttribute(xmlAttrSummaNetXTax, FormatFloat("0.00", PaymentTransaction.Money.GrandTotalExclGST).t_str());
   Receipt->SetAttribute(xmlAttrSummaNetTax, FormatFloat("0.00", PaymentTransaction.Money.GrandTotalGSTContent).t_str());

	Transaction->LinkEndChild(Receipt);

	BuildMembership(PaymentTransaction, Receipt);

	for (int i = 0; i < PaymentTransaction.PaymentsCount(); i++)
	{
		TPayment *Payment = PaymentTransaction.PaymentGet(i);
		if (Payment->GetPayCashOutTotal() != 0 || (Payment->GetAdjustment() != 0))
		{
			BuildPayment(PaymentTransaction, Payment, Receipt);
		}
	}

	for (int i = 0; i < PaymentTransaction.Orders->Count; i++)
	{
		TItemComplete *Order = (TItemComplete*)PaymentTransaction.Orders->Items[i];
		BuildProduct(PaymentTransaction, Order, Receipt);
      for (int i = 0; i < Order->SubOrders->Count; i++)
      {
         TItemCompleteSub *SubOrderImage = Order->SubOrders->SubOrderGet(i);
	      BuildProductSide(PaymentTransaction,Order, SubOrderImage, Receipt);
      }
	}
}

void TPOS_XMLTransaction::BuildMembership(TPaymentTransaction &PaymentTransaction, TiXmlElement *Receipt)
{
	PaymentTransaction.Membership.Member;
	TiXmlElement *EleMember = new TiXmlElement(xmlEleMember);
	EleMember->SetAttribute(xmlAttrXmlID, PaymentTransaction.Membership.Member.ContactKey);
	EleMember->SetAttribute(xmlAttrID, PaymentTransaction.Membership.Member.ContactID);
	EleMember->SetAttribute(xmlAttrName, PaymentTransaction.Membership.Member.Name.t_str());
	EleMember->SetAttribute(xmlAttrMemNumber, PaymentTransaction.Membership.Member.MembershipNumber.t_str());
	EleMember->SetAttribute(xmlAttrSiteID, PaymentTransaction.Membership.Member.SiteID);
	EleMember->SetAttribute(xmlAttrKnownAs, PaymentTransaction.Membership.Member.Alias.t_str());
	//EleMember->SetAttribute(xmlAttrProxCard, PaymentTransaction.Membership.Member.ProxStr.t_str());
	EleMember->SetAttribute(xmlAttrLastMod, PaymentTransaction.Membership.Member.LastModified.FormatString("YYYYMMDDHHMMSS").t_str());
	EleMember->SetAttribute(xmlAttrLastVisit, PaymentTransaction.Membership.Member.LastVisit.FormatString("YYYYMMDDHHMMSS").t_str());

	TiXmlElement *MagCards = new TiXmlElement(xmlEleMagCards);
	for (std::set <UnicodeString> ::iterator itCard = PaymentTransaction.Membership.Member.Cards.begin();
		itCard != PaymentTransaction.Membership.Member.Cards.end(); advance(itCard, 1))
	{
		TiXmlElement *MagCard = new TiXmlElement(xmlEleMagCard);
		UnicodeString Card = *itCard;
		MagCard->SetAttribute(xmlAttrCard, Card.t_str());
		MagCards->LinkEndChild(MagCard);
	}
	EleMember->LinkEndChild(MagCards);

	Receipt->LinkEndChild(EleMember);

	if (PaymentTransaction.Membership.Member.ContactKey != 0)
	{
		TPOS_XMLBase POSXML("Update Member Export");
		TDeviceRealTerminal::Instance().ManagerMembership->BuildXMLMemberUpdate(PaymentTransaction.DBTransaction,
			PaymentTransaction.Membership.Member, POSXML);
		TDeviceRealTerminal::Instance().IMManager->Export(POSXML);
		POSXML.SaveToFile();
	}
}

void TPOS_XMLTransaction::BuildPayment(TPaymentTransaction &PaymentTransaction, TPayment *inPayment, TiXmlElement *Receipt)
{
	TiXmlElement *ElePaymnet = new TiXmlElement(xmlElePayment);
	ElePaymnet->SetAttribute(xmlAttrID, _T(""));
	ElePaymnet->SetAttribute(xmlAttrName, inPayment->Name.t_str());
  	ElePaymnet->SetAttribute(xmlAttrCode, inPayment->PaymentThirdPartyID.t_str());

	if(inPayment->Properties & ePayTypePoints && inPayment->GetAdjustment() != 0)
   {
		ElePaymnet->SetAttribute(xmlAttrTotal, FormatFloat("-0.00", inPayment->GetAdjustment()).t_str());
		ElePaymnet->SetAttribute(xmlAttrAmountTendered, FormatFloat("-0.00", inPayment->GetAdjustment()).t_str());
   }
   else
   {
		ElePaymnet->SetAttribute(xmlAttrTotal, FormatFloat("0.00", inPayment->GetPayCashOutTotal()).t_str());
		ElePaymnet->SetAttribute(xmlAttrAmountTendered, FormatFloat("0.00", inPayment->GetPayTendered()).t_str());
   }
	ElePaymnet->SetAttribute(xmlAttrAmountPay, FormatFloat("0.00", inPayment->GetPayTotal()).t_str());
	ElePaymnet->SetAttribute(xmlAttrAmountChange, FormatFloat("0.00", inPayment->GetChange()).t_str());
	ElePaymnet->SetAttribute(xmlAttrAmountCashOut, FormatFloat("0.00", inPayment->GetCashOutTotal()).t_str());
   ElePaymnet->SetAttribute(xmlAttrAdjustment, FormatFloat("0.00", inPayment->GetAdjustment()).t_str());
   ElePaymnet->SetAttribute(xmlAttrTotalSurcharge, FormatFloat("0.00", inPayment->GetSurchargeTotal()).t_str());
   ElePaymnet->SetAttribute(xmlAttrTotalDiscount, FormatFloat("0.00", inPayment->GetDiscountTotal()).t_str());
	ElePaymnet->SetAttribute(xmlAttrRounding, FormatFloat("0.00", inPayment->GetRoundingTotal()).t_str());
	Receipt->LinkEndChild(ElePaymnet);
}

void TPOS_XMLTransaction::BuildProduct(TPaymentTransaction &PaymentTransaction, TItemComplete *Order, TiXmlElement *Receipt)
{
	TiXmlElement *EleProduct = new TiXmlElement(xmlEleProduct);
	EleProduct->SetAttribute(xmlAttrID, _T(""));
	EleProduct->SetAttribute(xmlAttrMenu, Order->MenuName.t_str());
	// EleProduct->SetAttribute(xmlAttrCourse,         UnicodeToUTF8AnsiString(Order->Course).c_str() );
	EleProduct->SetAttribute(xmlAttrCourse, Order->Course.t_str());
	EleProduct->SetAttribute(xmlAttrItem, Order->Item.t_str());
	EleProduct->SetAttribute(xmlAttrSize, Order->Size.t_str());
	EleProduct->SetAttribute(xmlAttrPLU, Order->PLU);
	EleProduct->SetAttribute(xmlAttrQty, FormatFloat("0.00", Order->GetQty()).t_str());
  	EleProduct->SetAttribute(xmlTransNo, Order->TransNo.t_str());

	// EleProduct->SetAttribute(xmlAttrServingCourse,  UnicodeToUTF8AnsiString(Order->ServingCourse.Name).c_str() );
	EleProduct->SetAttribute(xmlAttrServingCourse, Order->ServingCourse.Name.t_str());
	EleProduct->SetAttribute(xmlAttrCode, Order->ThirdPartyCode.t_str());
	EleProduct->SetAttribute(xmlEleMemberXmlID, Order->Loyalty_Key);

	TMMContactInfo Member;
	if (Order->Loyalty_Key != 0)
	{
		eMemberSource MemberSource;
		Member.ContactKey = Order->Loyalty_Key;
		TDeviceRealTerminal::Instance().ManagerMembership->GetMember(PaymentTransaction.DBTransaction, Member, MemberSource);
	}

	EleProduct->SetAttribute(xmlAttrMemNumber, Member.MembershipNumber.t_str());
	EleProduct->SetAttribute(xmlEleMemberSiteID, Member.SiteID);

	Receipt->LinkEndChild(EleProduct);

	BuildDiscount(PaymentTransaction, Order, EleProduct);
	BuildMetrics(PaymentTransaction, Order, EleProduct);
	BuildPricing(PaymentTransaction, Order, EleProduct);
	BuildSource(PaymentTransaction, Order, EleProduct);
}

void TPOS_XMLTransaction::BuildProductSide(TPaymentTransaction &PaymentTransaction,TItemComplete *ParentOrder,TItemMinorComplete *Order,TiXmlElement *Receipt)
{
   TiXmlElement *EleProduct = new TiXmlElement(xmlEleProduct);
   EleProduct->SetAttribute(xmlAttrID, _T(""));
   EleProduct->SetAttribute(xmlAttrMenu, Order->MenuName.t_str());
   // EleProduct->SetAttribute(xmlAttrCourse,         UnicodeToUTF8AnsiString(Order->Course).c_str() );
   EleProduct->SetAttribute(xmlAttrCourse, Order->Course.t_str());
   EleProduct->SetAttribute(xmlAttrItem, Order->Item.t_str());
   EleProduct->SetAttribute(xmlAttrSize, Order->Size.t_str());
   EleProduct->SetAttribute(xmlAttrQty, FormatFloat("0.00", Order->GetQty()).t_str());
  	EleProduct->SetAttribute(xmlTransNo, Order->TransNo.t_str());
   // EleProduct->SetAttribute(xmlAttrServingCourse,  UnicodeToUTF8AnsiString(Order->ServingCourse.Name).c_str() );
   EleProduct->SetAttribute(xmlAttrServingCourse, Order->ServingCourse.Name.t_str());
   EleProduct->SetAttribute(xmlAttrCode, Order->ThirdPartyCode.t_str());
   EleProduct->SetAttribute(xmlEleMemberXmlID, Order->Loyalty_Key);

   TMMContactInfo Member;
   if (Order->Loyalty_Key != 0)
   {
      eMemberSource MemberSource;
      Member.ContactKey = Order->Loyalty_Key;
      TDeviceRealTerminal::Instance().ManagerMembership->GetMember(PaymentTransaction.DBTransaction, Member, MemberSource);
   }

   EleProduct->SetAttribute(xmlAttrMemNumber, Member.MembershipNumber.t_str());
   EleProduct->SetAttribute(xmlEleMemberSiteID, Member.SiteID);

   Receipt->LinkEndChild(EleProduct);

   BuildDiscount(PaymentTransaction, Order, EleProduct);
   BuildMetrics(PaymentTransaction, ParentOrder, EleProduct);
   BuildPricing(PaymentTransaction, Order, EleProduct);
   BuildSource(PaymentTransaction, ParentOrder, EleProduct);
}

void TPOS_XMLTransaction::BuildDiscount(TPaymentTransaction &PaymentTransaction, TItemMinorComplete *Order, TiXmlElement *Product)
{
	TiXmlElement *ElePriceChanges = new TiXmlElement(xmlElePriceChanges);
	for (std::vector <TDiscount> ::iterator ptrDiscounts = Order->DiscountsBegin(); ptrDiscounts != Order->DiscountsEnd();
		std::advance(ptrDiscounts, 1))
	{
		TiXmlElement *ElePriceChange = new TiXmlElement(xmlElePriceChange);
		ElePriceChange->SetAttribute(xmlAttrName, ptrDiscounts->Name.t_str());
		ElePriceChange->SetAttribute(xmlAttrDiscription, ptrDiscounts->Description.t_str());
		ElePriceChange->SetAttribute(xmlAttrID, ptrDiscounts->ID);
		ElePriceChange->SetAttribute(xmlAttrXmlID, ptrDiscounts->DiscountKey);
		ElePriceChange->SetAttribute(xmlAttrReason, Order->DiscountReason.t_str());

		ElePriceChange->SetAttribute(xmlAttrType, ptrDiscounts->Type);
		ElePriceChange->SetAttribute(xmlAttrMode, ptrDiscounts->Mode);
		ElePriceChange->SetAttribute(xmlAttrPriority, ptrDiscounts->Priority);
		ElePriceChange->SetAttribute(xmlAttrSource, ptrDiscounts->Source);
		ElePriceChange->SetAttribute(xmlAttrApperanceOrder, ptrDiscounts->AppearanceOrder);
		ElePriceChange->SetAttribute(xmlAttrMenu, "");

		ElePriceChange->SetAttribute(xmlAttrPercent, FormatFloat("0.00", ptrDiscounts->PercentAmount).t_str());
		ElePriceChange->SetAttribute(xmlAttrAmount, FormatFloat("0.00", ptrDiscounts->Amount).t_str());
		ElePriceChange->SetAttribute(xmlAttrRounding, FormatFloat("0.00", ptrDiscounts->Rounding).t_str());
		ElePriceChange->SetAttribute(xmlAttrTotal, FormatFloat("0.00", Order->DiscountValue_BillCalc(ptrDiscounts)).t_str());
		ElePriceChanges->LinkEndChild(ElePriceChange);
	}
	Product->LinkEndChild(ElePriceChanges);
}

void TPOS_XMLTransaction::BuildMetrics(TPaymentTransaction &PaymentTransaction, TItemComplete *Order, TiXmlElement *Product)
{
	TiXmlElement *EleMetrics = new TiXmlElement(xmlEleMetrics);
	EleMetrics->SetAttribute(xmlAttrDate, Order->TimeStamp.FormatString("dd/mm/yyyy").t_str());
	EleMetrics->SetAttribute(xmlAttrTime, Order->TimeStamp.FormatString("hh:nn:ss").t_str());
	EleMetrics->SetAttribute(xmlAttrGroupName, Order->Categories->FinancialCategoryGroup.t_str());
	EleMetrics->SetAttribute(xmlAttrCategoryName, Order->Categories->FinancialCategory.t_str());
	Product->LinkEndChild(EleMetrics);
}

void TPOS_XMLTransaction::BuildPricing(TPaymentTransaction &PaymentTransaction, TItemMinorComplete *Order, TiXmlElement *Product)
{
   TiXmlElement *ElePricing = new TiXmlElement(xmlElePricing);
   ElePricing->SetAttribute(xmlAttrOrderType, Order->OrderType);
   ElePricing->SetAttribute(xmlAttrHappyHour, Order->HappyHour);
   ElePricing->SetAttribute(xmlAttrTotal, FormatFloat("0.00", Order->GrandTotal()).t_str());
   ElePricing->SetAttribute(xmlAttrTotalExcGST, FormatFloat("0.00", Order->GrandTotalExclGST()).t_str());
	ElePricing->SetAttribute(xmlAttrGSTContent, FormatFloat("0.00", Order->GrandTotalGSTContent()).t_str());
   ElePricing->SetAttribute(xmlAttrAdjustmentTotal, FormatFloat("0.00", Order->TotalAdjustment()).t_str());
   ElePricing->SetAttribute(xmlAttrAdjustmentTotalExcGST, FormatFloat("0.00", Order->TotalAdjustmentExcGST()).t_str());
   ElePricing->SetAttribute(xmlAttrAdjustmentGSTTotal, FormatFloat("0.00", Order->TotalAdjustmentGSTContent()).t_str());
   ElePricing->SetAttribute(xmlAttrSummaGross, FormatFloat("0.00", Order->Price() + Order->TotalSurcharge()).t_str());
   ElePricing->SetAttribute(xmlAttrSummaNet, FormatFloat("0.00", Order->GrandTotal()).t_str());
   ElePricing->SetAttribute(xmlAttrSummaNetXTax, FormatFloat("0.00",Order->GrandTotalExclGST()).t_str());
   ElePricing->SetAttribute(xmlAttrSummaNetTax, FormatFloat("0.00", Order->GrandTotalGSTContent()).t_str());

   ElePricing->SetAttribute(xmlAttrTotalDiscount, FormatFloat("0.00", Order->TotalDiscount()).t_str());
   ElePricing->SetAttribute(xmlAttrTotalSurcharge, FormatFloat("0.00", Order->TotalSurcharge()).t_str());
   ElePricing->SetAttribute(xmlAttrTotalSurchargeXTax, FormatFloat("0.00", Order->TotalSurchargeExcGST()).t_str());
   ElePricing->SetAttribute(xmlAttrTotalSurchargeTax, FormatFloat("0.00", Order->TotalSurchargeGSTContent()).t_str());

   ElePricing->SetAttribute(xmlAttrPriceLevel0, FormatFloat("0.00", Order->PriceLevel0).t_str());
   ElePricing->SetAttribute(xmlAttrPriceLevel1, FormatFloat("0.00", Order->PriceLevel1).t_str());
//	ElePricing->SetAttribute(xmlAttrGSTPercent, FormatFloat("0.00", Order->GSTPercent).t_str());
   ElePricing->SetAttribute(xmlAttrPointsRedeemed, FormatFloat("0.00", Order->Redeemed).t_str());
   ElePricing->SetAttribute(xmlAttrPointsPercent, FormatFloat("0.00", Order->PointsPercent).t_str());
   ElePricing->SetAttribute(xmlAttrPointsEarned, FormatFloat("0.00", Order->PointsEarned).t_str());
   Product->LinkEndChild(ElePricing);
}

void TPOS_XMLTransaction::BuildSource(TPaymentTransaction &PaymentTransaction, TItemComplete *Order, TiXmlElement *Product)
{
	TiXmlElement *EleSource = new TiXmlElement(xmlEleSource);
	EleSource->SetAttribute(xmlAttrTable, IntToStr(Order->TableNo).t_str());
	EleSource->SetAttribute(xmlAttrSeat, IntToStr(Order->SeatNo).t_str());
	EleSource->SetAttribute(xmlAttrRoom, IntToStr(Order->RoomNo).t_str());
	EleSource->SetAttribute(xmlAttrTab, Order->TabName.t_str());
	EleSource->SetAttribute(xmlAttrPax, UnicodeString(TManagerPatron::Instance().GetTotalPatrons(PaymentTransaction.Patrons)).t_str());
	Product->LinkEndChild(EleSource);
}

void TPOS_XMLTransaction::Parse()
{
	TiXmlHandle hDoc(&ResultDoc);
	TiXmlElement* pElement;
	pElement = hDoc.FirstChild(_T("Response")).ToElement();
	if (pElement)
	{
		AnsiString TransID = AnsiString(pElement->Attribute(_T("id")));
		AnsiString Status = AnsiString(pElement->Attribute(_T("Status")));
		AnsiString Message = AnsiString(pElement->Attribute(_T("Message")));

		if (TransID != IntaMateID)
		{
			Result = eIMCache;
			ResultText = "Transaction ID Mismatch";
		}
		else if (UpperCase(Status) == "NAK")
		{
			Result = eIMFailed;
			ResultText = Message;
		}
		else if (UpperCase(Status) == "ACK")
		{
			Result = eIMAccepted;
			ResultText = Message;
		}
		else
		{
			Result = eIMFailed;
			ResultText = "Incorrect Responce format";
		}
	}
	else
	{
		Result = eIMFailed;
		ResultText = "Incorrect XML format";
	}
}

