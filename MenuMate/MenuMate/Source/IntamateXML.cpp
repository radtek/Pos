//---------------------------------------------------------------------------


#pragma hdrstop

#include "POS_XMLBase.h"
#include <SysUtils.hpp>
#include <tchar.h>
//---------------------------------------------------------------------------

#pragma package(smart_init)

TPOS_XMLBase::TPOS_XMLBase(UnicodeString inName)
{
   Name = inName;
   IntaMateID = Name + Now().FormatString(" yyyy-mmm-dd hh-nn-ss");
   Cache = true;
}

void TPOS_XMLBase::SaveToFile()
{
	Doc.SaveFile(UnicodeString(_T("IM Sent ") + Name + _T(".xml")).c_str());
	ResultDoc.SaveFile(UnicodeString("IM Received " + Name + ".xml").c_str());
}

void TPOS_XMLBase::LoadFromFile(UnicodeString FileName)
{
	Doc.LoadFile(FileName.t_str());
	TiXmlElement* pElement = NULL;
   pElement = Doc.RootElement();
   if (pElement)
   {
		IntaMateID = UnicodeString(pElement->Attribute(_T("id")));
	}
}


UnicodeString TPOS_XMLBase::SerializeOut()
{
   UnicodeString RetVal = "";
   TiXmlPrinter printer;
	printer.SetIndent( _T("\t") );
	Doc.Accept( &printer );
   RetVal = printer.CStr();
   return RetVal;
}

void TPOS_XMLBase::SerializeIn(UnicodeString Data)
{
   //load it into the Doc
	ResultDoc.Parse(Data.t_str());
   if(ResultDoc.Error())
   {
      Result = eIMFailed;
      ResultText = ResultDoc.ErrorDesc();
   }
}

//---------------------------------------------------------------------------
void TPOS_XMLBase::Clear()
{
   Doc.Clear();
   ResultDoc.Clear();
   Result = eIMFailed;
}

void TPOS_XMLBase::Reset(UnicodeString inName, UnicodeString inID)
{
   Name = inName;
   if(inID == "")
   {
      IntaMateID = Name + Now().FormatString(" yyyy-mmm-dd hh-nn-ss");
   }
   Doc.Clear();
   ResultDoc.Clear();
   Result = eIMFailed;
}

//---------------------------------------------------------------------------
void TPOS_XMLBase::Parse()
{
   TiXmlHandle hDoc(&ResultDoc);
   TiXmlElement* pElement = NULL;
	pElement = hDoc.FirstChild(_T("Response")).ToElement();
	if (pElement)
   {
		UnicodeString TransID = UnicodeString(pElement->Attribute(_T("id")));
		UnicodeString Status = UnicodeString(pElement->Attribute(_T("Status")));
		UnicodeString Message = UnicodeString(pElement->Attribute(_T("Message")));

      if(TransID != IntaMateID)
      {
      	Result = eIMFailed;
         ResultText = "Transaction ID Mismatch";
      }
      else if(UpperCase(Status) == "NAK")
      {
      	Result = eIMFailed;
         ResultText = Message;
      }
      else if(UpperCase(Status) == "ACK")
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

//---------------------------------------------------------------------------

TPOS_XMLStatus::TPOS_XMLStatus() : TPOS_XMLBase("Status")
{
   Cache = false;
}

void TPOS_XMLStatus::Parse()
{
   TiXmlHandle hDoc(&ResultDoc);
   TiXmlElement* pElement = NULL;
	pElement = hDoc.FirstChild(_T("Response")).ToElement();
   if (pElement)
   {
		UnicodeString TransID = UnicodeString(pElement->Attribute(_T("id")));
		UnicodeString Status = UnicodeString(pElement->Attribute(_T("Status")));
	  UnicodeString Message = UnicodeString(pElement->Attribute(_T("Message")));

      if(TransID != IntaMateID)
      {
      	Result = eIMFailed;
         ResultText = "Transaction ID Mismatch";
      }
      else if(UpperCase(Status) == "NAK")
      {
      	Result = eIMFailed;
         ResultText = Message;
      }
      else if(UpperCase(Status) == "ACK")
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

void TPOS_XMLStatus::Build()
{
      // Update the IntaMate ID with the Invoice Number.
      IntaMateID = IntaMateID + " StatusCheck";

      Doc.Clear();
		TiXmlDeclaration * decl = new TiXmlDeclaration( _T("1.0"), _T("UTF-8"), _T("") );
      Doc.LinkEndChild( decl );
// Insert DOCTYPE definiation here.
      TiXmlElement * Transaction = new TiXmlElement( xmlEleRequest );
      Transaction->SetAttribute(xmlAttrID, AnsiString(IntaMateID).c_str());
      Transaction->SetAttribute(xmlRequestType, xmlTypeStatus);
      Doc.LinkEndChild( Transaction );
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

