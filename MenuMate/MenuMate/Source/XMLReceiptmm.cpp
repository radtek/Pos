
// ******************************************************************************************* //
//                                                                                           
//                                     XML Data Binding                                      
//                                                                                           
//         Generated on: 7/03/2012 10:05:27 a.m.                                             
//       Generated from: C:\Code\5.5\Static Files\XML Examples\Master Receipt Template.xml   
//                                                                                           
// ******************************************************************************************* //

#include <vcl.h>
#pragma hdrstop

#include "XMLReceiptmm.h"

namespace XMLReceipt
{

// Global Functions 

_di_IXMLRECEIPTType __fastcall GetRECEIPT(_di_IXMLDocument Doc)
{
  return (_di_IXMLRECEIPTType) Doc->GetDocBinding("RECEIPT", __classid(TXMLRECEIPTType), TargetNamespace);
};

_di_IXMLRECEIPTType __fastcall GetRECEIPT(TXMLDocument *Doc)
{
  _di_IXMLDocument DocIntf;
  Doc->GetInterface(DocIntf);
  return GetRECEIPT(DocIntf);
};

_di_IXMLRECEIPTType __fastcall LoadRECEIPT(const UnicodeString& FileName)
{
  return (_di_IXMLRECEIPTType) LoadXMLDocument(FileName)->GetDocBinding("RECEIPT", __classid(TXMLRECEIPTType), TargetNamespace);
};

_di_IXMLRECEIPTType __fastcall  NewRECEIPT()
{
  return (_di_IXMLRECEIPTType) NewXMLDocument()->GetDocBinding("RECEIPT", __classid(TXMLRECEIPTType), TargetNamespace);
};

// TXMLRECEIPTType 

void __fastcall TXMLRECEIPTType::AfterConstruction(void)
{
  RegisterChildNode(UnicodeString("ACCOUNTIDENT"), __classid(TXMLACCOUNTIDENTType));
  TXMLNode::AfterConstruction();
};

_di_IXMLACCOUNTIDENTType __fastcall TXMLRECEIPTType::Get_ACCOUNTIDENT()
{
  return (_di_IXMLACCOUNTIDENTType) TXMLNode::ChildNodes->Nodes[UnicodeString("ACCOUNTIDENT")];
};

// TXMLACCOUNTIDENTType 

Integer __fastcall TXMLACCOUNTIDENTType::Get_TYPE()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("TYPE")]->NodeValue;
};

void __fastcall TXMLACCOUNTIDENTType::Set_TYPE(Integer Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("TYPE")]->NodeValue = Value;
};

Integer __fastcall TXMLACCOUNTIDENTType::Get_TABLE()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("TABLE")]->NodeValue;
};

void __fastcall TXMLACCOUNTIDENTType::Set_TABLE(Integer Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("TABLE")]->NodeValue = Value;
};

Integer __fastcall TXMLACCOUNTIDENTType::Get_SEAT()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("SEAT")]->NodeValue;
};

void __fastcall TXMLACCOUNTIDENTType::Set_SEAT(Integer Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("SEAT")]->NodeValue = Value;
};

Integer __fastcall TXMLACCOUNTIDENTType::Get_ROOM()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("ROOM")]->NodeValue;
};

void __fastcall TXMLACCOUNTIDENTType::Set_ROOM(Integer Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("ROOM")]->NodeValue = Value;
};

Integer __fastcall TXMLACCOUNTIDENTType::Get_STAFF()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("STAFF")]->NodeValue;
};

void __fastcall TXMLACCOUNTIDENTType::Set_STAFF(Integer Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("STAFF")]->NodeValue = Value;
};

Integer __fastcall TXMLACCOUNTIDENTType::Get_WEBID()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("WEBID")]->NodeValue;
};

void __fastcall TXMLACCOUNTIDENTType::Set_WEBID(Integer Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("WEBID")]->NodeValue = Value;
};

Integer __fastcall TXMLACCOUNTIDENTType::Get_TAB()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("TAB")]->NodeValue;
};

void __fastcall TXMLACCOUNTIDENTType::Set_TAB(Integer Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("TAB")]->NodeValue = Value;
};

Integer __fastcall TXMLACCOUNTIDENTType::Get_MEMBER()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("MEMBER")]->NodeValue;
};

void __fastcall TXMLACCOUNTIDENTType::Set_MEMBER(Integer Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("MEMBER")]->NodeValue = Value;
};
}