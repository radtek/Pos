#pragma hdrstop

#include "MembershipExternalXML.h"
#include <SysUtils.hpp>
#include <tchar.h>
#include <DateUtils.hpp>
#include <system.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------

#pragma package(smart_init)


TMSXMLBase *TMSXMLBase::SetTransNo(int data)
{
	TransID = data;
	return this;
}

UnicodeString TMSXMLBase::SerializeOut()
{
   UnicodeString RetVal = "";
   TiXmlPrinter printer;
	printer.SetIndent( _T("\t") );
	Doc.Accept( &printer );
   RetVal = printer.CStr();
	return RetVal;
}

void TMSXMLBase::SerializeIn(UnicodeString Data)
{
   //load it into the Doc
	ResultDoc.Parse(Data.t_str());
   if(ResultDoc.Error())
   {
	   Result = eMSFailed;
      ResultText = ResultDoc.ErrorDesc();
   }
}

//---------------------------------------------------------------------------
void TMSXMLBase::Clear()
{
   Doc.Clear();
   ResultDoc.Clear();
	Result = eMSFailed;
}

void TMSXMLBase::Reset()
{
	Doc.Clear();
   ResultDoc.Clear();
	Result = eMSFailed;
}

void TMSXMLBase::SaveToFile()
{
	Doc.SaveFile(UnicodeString(_T("MS Sent ") + IntToStr(TransID) + _T(".xml")).c_str());
	ResultDoc.SaveFile(UnicodeString("MS Received " + IntToStr(TransID) + ".xml").c_str());
}

//---------------------------------------------------------------------------
void TMSXMLBase::Parse()
{
	TiXmlHandle hDoc(&ResultDoc);
   TiXmlElement* pElement = NULL;
	pElement = hDoc.FirstChild(_T("Response")).ToElement();
	if (pElement)
   {
		UnicodeString TransactionID = UnicodeString(pElement->Attribute(_T("id")));
		UnicodeString TransmissionStatus = UnicodeString(pElement->Attribute(_T("TransmissionStatus")));
		UnicodeString Message = UnicodeString(pElement->Attribute(_T("Message")));

	   if(TransactionID != TransID)
      {
			Result = eMSFailed;
			ResultText = "Transaction ID Mismatch";
      }
	  else if(UpperCase(TransmissionStatus) == "NAK")
	   {
			Result = eMSFailed;
			ResultText = Message;
      }
	  else if(UpperCase(TransmissionStatus) == "ACK")
      {
			Result = eMSAccepted;
			ResultText = Message;
      }
      else
      {
	   	  Result = eMSFailed;
         ResultText = "Incorrect Responce Status : " + UpperCase(TransmissionStatus);
      }
   }
   else
   {
		Result = eMSFailed;
      ResultText = "Incorrect XML format ";
   }
}

TMSXMLBase *TMSXMLBase::SetRepeatTranaction(bool data)
{
	RepeatTransaction = data;
	return this;
}

//---------------------------------------------------------------------------

void TMSXMLStatus::Build()
{
	// Update the IntaMate ID with the Invoice Number.
	Doc.Clear();
	TiXmlDeclaration * decl = new TiXmlDeclaration( _T("1.0"), _T("UTF-8"), _T("") );
	Doc.LinkEndChild( decl );
	// Insert DOCTYPE definiation here.
	TiXmlElement * Transaction = new TiXmlElement( xmlEleRequest );
	Transaction->SetAttribute(xmlAttrID, AnsiString(TransID).c_str());
	Transaction->SetAttribute(xmlRequestType, xmlTypeStatus);
	Doc.LinkEndChild( Transaction );
}

void TMSXMLStatus::Parse()
{
	TMSXMLBase::Parse();
}

//---------------------------------------------------------------------------

TMSXMLEnquiry::TMSXMLEnquiry() : TMSXMLBase()
{
	   MemberName = "";
	   AccountNo = "";
	   PointsBalance = 0;
	   CompBalance = 0;
	   Authorised = 0;
       PromoBalance = 0;
}

void TMSXMLEnquiry::Parse()
{
    try
    {
       TiXmlHandle hDoc(&ResultDoc);
        TiXmlElement* pElement = NULL;
        pElement = hDoc.FirstChild(_T("Response")).ToElement();
        if (pElement)
        {
            UnicodeString TransactionID = UnicodeString(pElement->Attribute(_T("id")));
            UnicodeString TransmissionStatus = UnicodeString(pElement->Attribute(_T("TransmissionStatus")));
            Authorised = 0;
            ResultText = UnicodeString(pElement->Attribute(_T("Message")));

            if(TransactionID != TransID && TransactionID != "-1")
            {
                Result = eMSFailed;
                ResultText = "Transaction ID Mismatch";
            }
            else if(UpperCase(TransmissionStatus) == "NAK")
            {
                Result = eMSFailed;
            }
            else if(UpperCase(TransmissionStatus) == "ACK")
            {
                UnicodeString ResponseStatus = UnicodeString(pElement->Attribute(_T("ResponseStatus")));
                if(ResponseStatus == "ACK")
                {    
                    Result = eMSAccepted;
                    MemberName = UnicodeString(pElement->Attribute(_T(xmlEleMember)));
                    AccountNo = UnicodeString(pElement->Attribute(_T(xmlAttrMemNumber)));
                    Note = UnicodeString(pElement->Attribute(_T(xmlAttrNote)));
                    Type = UnicodeString(pElement->Attribute(_T(xmlAttrType)));
                    pElement->QueryIntAttribute(_T(xmlAttrGroupID),&MembersGroup);
                    pElement->QueryIntAttribute(_T(xmlAttrPoints),&PointsBalance);
                    pElement->QueryIntAttribute(_T(xmlAttrComp),&CompBalance);
                    pElement->QueryIntAttribute(_T(xmlAttrPromo2),&PromoBalance);
                    pElement->QueryIntAttribute(_T(xmlAttrAuth),&Authorised);
                    TiXmlDocument  *document = new TiXmlDocument();
                    document = pElement->GetDocument();
                }
                else
                {
                    Result = eMSFailed;
                }
            }
            else
            {
                Result = eMSFailed;
                ResultText = "Incorrect Responce Status : " + UpperCase(TransmissionStatus);
            }
        }
        else
        {
            Result = eMSFailed;
            ResultText = "Incorrect XML format ";
        }
    }
    catch(Exception &E)
    {
        //TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
    }
}

TMSXMLEnquiry *TMSXMLEnquiry::SetTerminalID(int data)
{
	TerminalID = data;
	return this;
}

TMSXMLEnquiry *TMSXMLEnquiry::SetTerminalName(AnsiString data)
{
	TerminalName = data;
	return this;
}

TMSXMLEnquiry *TMSXMLEnquiry::SetCard(AnsiString data)
{
	Card = data;
	return this;
}

TMSXMLEnquiry *TMSXMLEnquiry::SetPIN(AnsiString data)
{
	PIN = data;
	return this;
}



void TMSXMLEnquiry::Build()
{
	// Update the IntaMate ID with the Invoice Number.
	Doc.Clear();
	TiXmlDeclaration * decl = new TiXmlDeclaration( _T("1.0"), _T("UTF-8"), _T("") );
	Doc.LinkEndChild( decl );
	// Insert DOCTYPE definiation here.
	TiXmlElement * Transaction = new TiXmlElement( xmlEleRequest );
	Transaction->SetAttribute(xmlRequestType, xmlTypeEnquiry);
	//Transaction->SetAttribute(xmlAttrID, AnsiString(TransID).c_str());
	Transaction->SetAttribute(xmlAttrTerminalID, TerminalID);
	Transaction->SetAttribute(xmlAttrTerminalName, TerminalName.c_str());
	Transaction->SetAttribute(xmlAttrCard, Card.c_str());
	Transaction->SetAttribute(xmlAttrPIN, PIN.c_str());
	Transaction->SetAttribute(xmlRepeatTrans, RepeatTransaction);
	Doc.LinkEndChild( Transaction );
}

TMSXMLTransaction * TMSXMLTransaction::AddPayment(int ID,int value)
{
	CodedValues[ID] += value;
	return this;
}

void TMSXMLTransaction::Build()
{
	// Update the IntaMate ID with the Invoice Number.
	Doc.Clear();
	TiXmlDeclaration * decl = new TiXmlDeclaration( _T("1.0"), _T("UTF-8"), _T("") );
	Doc.LinkEndChild( decl );
	// Insert DOCTYPE definiation here.
	TiXmlElement * Transaction = new TiXmlElement( xmlEleRequest );
	Transaction->SetAttribute(xmlRequestType, xmlTypeTransaction);
	//Transaction->SetAttribute(xmlAttrID, AnsiString(TransID).c_str());
	Transaction->SetAttribute(xmlAttrTerminalID, TerminalID);
	Transaction->SetAttribute(xmlAttrTerminalName, TerminalName.c_str());
	Transaction->SetAttribute(xmlAttrCard, Card.c_str());
	Transaction->SetAttribute(xmlAttrPIN, PIN.c_str());
	Transaction->SetAttribute(xmlRepeatTrans, RepeatTransaction);
	TCodeCurrency::const_iterator Cats = CodedValues.begin();
	for (; Cats != CodedValues.end(); Cats++)
	{

		TiXmlElement *Payment = new TiXmlElement(xmlElePayment);
		Payment->SetAttribute(xmlAttrID, Cats->first);
		Payment->SetAttribute(xmlAttrTotal, Cats->second);
		Transaction->LinkEndChild(Payment);
	}

	Doc.LinkEndChild( Transaction );
}

void TMSXMLTransaction::Parse()
{  
	TiXmlHandle hDoc(&ResultDoc);
	TiXmlElement* pElement = NULL;
	pElement = hDoc.FirstChild(_T("Response")).ToElement();
	if (pElement)
	{
		UnicodeString TransactionID = UnicodeString(pElement->Attribute(_T("id")));
		UnicodeString TransmissionStatus = UnicodeString(pElement->Attribute(_T("TransmissionStatus")));

		ResultText = UnicodeString(pElement->Attribute(_T("Message")));

		if(TransactionID != TransID && TransactionID != "-1")
		{
			Result = eMSFailed;
			ResultText = "Transaction ID Mismatch";
		}
		else if(UpperCase(TransmissionStatus) == "NAK")
		{
			Result = eMSFailed;
		}
		else if(UpperCase(TransmissionStatus) == "ACK")
		{
			UnicodeString ResponseStatus = UnicodeString(pElement->Attribute(_T("ResponseStatus")));
			if(ResponseStatus == "ACK")
			{
				Result = eMSAccepted;
			}
			else
			{
				Result = eMSFailed;
			}
		}
		else
		{
			Result = eMSFailed;
			ResultText = "Incorrect Responce Status : " + UpperCase(TransmissionStatus);
		}
	}
	else
	{
		Result = eMSFailed;
		ResultText = "Incorrect XML format ";
	}
}
//------------------------------------------
