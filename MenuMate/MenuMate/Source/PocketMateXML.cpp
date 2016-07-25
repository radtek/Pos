//---------------------------------------------------------------------------


#pragma hdrstop

#include "PocketMateXML.h"
#include <SysUtils.hpp>
#include <tchar.h>
#include "MD5Checksum.h"
#include <XMLDoc.hpp>
//---------------------------------------------------------------------------

#pragma package(smart_init)

TXMLBase::TXMLBase(AnsiString inName)
{
   Name = inName;
   ID = Name + Now().FormatString(" yyyy-mmm-dd hh-nn-ss");
   Cache = true;

   Doc = NewXMLDocument();
   ResultDoc = NewXMLDocument();
}

TXMLBase::~TXMLBase()
{
//	delete Doc;
   Doc = NULL;
//	delete ResultDoc;
   ResultDoc = NULL;
}

void TXMLBase::SaveToFile()
{
//	Doc->SaveToFile(AnsiString("XML Sent " + Name + ".xml").t_str());
//	ResultDoc->SaveToFile(AnsiString("XML Received " + Name + ".xml").t_str());
}

void TXMLBase::LoadFromFile(AnsiString FileName)
{
//	Doc->LoadFromFile(FileName.t_str());
//   Doc->Active = true;
//	ID = Doc->DocumentElement->GetAttribute(ID_ELEMENT);
}


AnsiString TXMLBase::SerializeOut()
{
   UnicodeString RetVal = "";
   Doc->SaveToXML(RetVal);
   return RetVal;
}

void TXMLBase::SerializeIn(AnsiString Data)
{
   //load it into the Doc
   try
   {
		ResultDoc->LoadFromXML(Data);
   }
	catch(Exception &E)
   {
      Result = eXMLFailed;
      ResultText = E.Message;
   }
}

//---------------------------------------------------------------------------
void TXMLBase::Clear()
{
   Doc->Active = false;
   Doc->FileName = "";
   Doc->XML->Clear();
   ResultDoc->Active = false;
   ResultDoc->FileName = "";
   ResultDoc->XML->Clear();
   Result = eXMLFailed;
}

void TXMLBase::Reset(AnsiString inName, AnsiString inID)
{
   Name = inName;
   if(inID == "")
   {
      ID = Now().FormatString(Name + " yyyy-mmm-dd hh-nn-ss");
   }
   Clear();
}

//---------------------------------------------------------------------------

TXMLVoucher::TXMLVoucher(AnsiString TransID) : TXMLBase("Status")
{
   Cache = false;
   ID_ELEMENT = _T("Transaction_ID");
   ID = TransID;
}

void TXMLVoucher::Parse()
{
	if(ResultDoc->Active && ResultDoc->DocumentElement != NULL)
   {
      try
      {
         if(ResultDoc->DocumentElement->LocalName == WideString("MM_PV_Transaction_Response"))
         {
//            AnsiString TransID 	= ResultDoc->DocumentElement->GetChildValue(ID_ELEMENT.t_str());
            UnicodeString datastring = ResultDoc->XML->CommaText;


            AnsiString TransID 	= "";
            AnsiString PVTransID 	= "";
            AnsiString Status 	= "";
            AnsiString Message1 	= "";
            AnsiString Message2 	= "";
            AnsiString Message3 	= "";

            _di_IXMLNodeList GetChildNodes = ResultDoc->DocumentElement->GetChildNodes();
            int Index = GetChildNodes->IndexOf("Transaction_ID");
            if(Index != -1)
            {
               PVTransID 	= GetChildNodes->GetNode(Index)->Text;
            }

            Index = GetChildNodes->IndexOf("Menumate_Transaction");
            if(Index != -1)
            {
            	TransID 	= GetChildNodes->GetNode(Index)->Text;
            }
           Index = GetChildNodes->IndexOf("Status");
            if(Index != -1)
            {
            	Status 	= GetChildNodes->GetNode(Index)->Text;
            }
            Index = GetChildNodes->IndexOf("Message_1");


            if(Index != -1)
            {
            	Message1 	= GetChildNodes->GetNode(Index)->Text;
                Message1 =  UpperCase(Message1);
                AnsiString searchMessage =  "SUCCESS";
                if(strstr(Message1.c_str(), searchMessage.c_str()))
                {
                    Message1 = "SUCCESS- Voucher Accepted";
                }
            }
            Index = GetChildNodes->IndexOf("Message_2");
            if(Index != -1)
            {
            	Message2 	= GetChildNodes->GetNode(Index)->Text;
            }
            Index = GetChildNodes->IndexOf("Message_3");
            if(Index != -1)
            {
            	Message3 	= GetChildNodes->GetNode(Index)->Text;
            }

            if(UpperCase(Status) == "0")
            {
               if(TransID != ID)
               {
                  Result = eXMLFailed;
                  ResultText = "Voucher ID Mismatch";
               }
               else
               {
			         Result = eXMLAccepted;
                  fPVID = PVTransID;
         	      ResultText = Message1 + "\r" + Message2 + "\r" + Message3;

               }
            }
            else
            {
               Result = eXMLFailed;
               fPVID = PVTransID;
               ResultText = Message1 + "\r" + Message2 + "\r" + Message3;
               GetContactNumber(ResultText);
            }
         }
         else
         {
            Result = eXMLFailed;
            ResultText = "Incorrect XML format. Root Node is not MM_PV_Transaction_Response.";
         }
      }
      catch(Exception &E)
      {
         Result = eXMLFailed;
         ResultText = E.Message;
      }
   }
   else
   {
   	Result = eXMLFailed;
      ResultText = "No Result Received";
   }
}

void TXMLVoucher::BuildXML()
{
	try
   {
      //Doc->XML->Clear();
      Doc->Active = false;
      Doc->FileName = "";
      Doc->Active = true;

      _di_IXMLNode RootNode = Doc->AddChild("MM_PV_Transaction");
      _di_IXMLNode TransTypeNode = RootNode->AddChild("Transaction_Type");
      TransTypeNode->Text = fType;
      _di_IXMLNode VoucherNode = RootNode->AddChild("PV_Code");
      VoucherNode->Text = fVoucherCode;
      _di_IXMLNode TenderNode = RootNode->AddChild("Tender_Amount");
      TenderNode->Text = fAmountTender;
      _di_IXMLNode SaleNode = RootNode->AddChild("Sale_Amount");
      SaleNode->Text = fAmountSale;
      _di_IXMLNode MerchantNode = RootNode->AddChild("Merchant_ID");
      MerchantNode->Text = fMerchantID;
      _di_IXMLNode TransactionIDNode = RootNode->AddChild("Menumate_Transaction");
      TransactionIDNode->Text = ID;
      _di_IXMLNode LoyaltyNode = RootNode->AddChild("Loyalty_ID");
      LoyaltyNode->Text = fLoyaltyID;
      _di_IXMLNode KeyNode = RootNode->AddChild("Key");
      std::auto_ptr<CMD5Checksum> lCMD5Checksum(new CMD5Checksum);
      AnsiString MD5MessesUpTheDatapassedin = AnsiString("123") +  fVoucherCode;
      KeyNode->Text = lCMD5Checksum->GetMD5(MD5MessesUpTheDatapassedin);
   }
	catch (Exception &E)
	{
   	Result = eXMLFailed;
      ResultText = E.Message;
	}
}

void TXMLVoucher::SetTransactionType(AnsiString Type)
{
	fType = Type;
	BuildXML();
}
void TXMLVoucher::SetVoucherCode(AnsiString VoucherCode)
{
	fVoucherCode = VoucherCode;
  	BuildXML();
}
void TXMLVoucher::SetTenderAmount(AnsiString TenderAmount)
{
	fAmountTender = TenderAmount;
  	BuildXML();
}
void TXMLVoucher::SetSaleAmount(AnsiString SaleAmount)
{
	fAmountSale = SaleAmount;
  	BuildXML();
}
void TXMLVoucher::SetMerchantID(AnsiString VoucherMerchantID)
{
	fMerchantID = VoucherMerchantID;
  	BuildXML();
}
void TXMLVoucher::SetLoyaltyID(AnsiString MembershipNumber)
{
	fLoyaltyID = MembershipNumber;
  	BuildXML();
}

AnsiString TXMLVoucher::GetPVTransCode()
{
   return fPVID;
}

void TXMLVoucher::GetContactNumber(AnsiString &ResultTxt)
{
 int breakpos= ResultTxt.Pos(".");

 if(breakpos <= 0)
    breakpos= ResultTxt.Pos("-");

 int indcal= ResultTxt.Pos("0");
 if(indcal > 0)
 {
    ContactNumber = "(+61) "+ResultTxt.SubString(indcal,12);
    if(breakpos>0)
  ResultTxt= ResultTxt.SubString(0,breakpos-1)+".";
 }
// if(breakpos>0)
 // ResultTxt= ResultTxt.SubString(0,breakpos-1)+".";
}



//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


