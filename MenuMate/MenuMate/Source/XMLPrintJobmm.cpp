
// ********************************************************************************************** //
//                                                                                              
//                                       XML Data Binding                                       
//                                                                                              
//         Generated on: 8/21/2013 5:17:57 PM                                                   
//       Generated from: C:\Code\MMDev\Static Files\XML Examples\Master Printing Template.xml   
//                                                                                              
// ********************************************************************************************** //

#include <vcl.h>
#pragma hdrstop

#include "XMLPrintJobmm.h"

namespace XMLPrintJob
{

// Global Functions 

_di_IXMLPRINTJOBType __fastcall GetPRINTJOB(_di_IXMLDocument Doc)
{
  return (_di_IXMLPRINTJOBType) Doc->GetDocBinding("PRINTJOB", __classid(TXMLPRINTJOBType), TargetNamespace);
};

_di_IXMLPRINTJOBType __fastcall GetPRINTJOB(TXMLDocument *Doc)
{
  _di_IXMLDocument DocIntf;
  Doc->GetInterface(DocIntf);
  return GetPRINTJOB(DocIntf);
};

_di_IXMLPRINTJOBType __fastcall LoadPRINTJOB(const UnicodeString& FileName)
{
  return (_di_IXMLPRINTJOBType) LoadXMLDocument(FileName)->GetDocBinding("PRINTJOB", __classid(TXMLPRINTJOBType), TargetNamespace);
};

_di_IXMLPRINTJOBType __fastcall  NewPRINTJOB()
{
  return (_di_IXMLPRINTJOBType) NewXMLDocument()->GetDocBinding("PRINTJOB", __classid(TXMLPRINTJOBType), TargetNamespace);
};

// TXMLPRINTJOBType 

void __fastcall TXMLPRINTJOBType::AfterConstruction(void)
{
  ItemTag = "OrderGUID";
  ItemInterface = __uuidof(IXMLNode);
  TXMLNodeCollection::AfterConstruction();
};

UnicodeString __fastcall TXMLPRINTJOBType::Get_NAME()
{
  return TXMLNode::AttributeNodes->Nodes[UnicodeString("NAME")]->Text;
};

void __fastcall TXMLPRINTJOBType::Set_NAME(UnicodeString Value)
{
  TXMLNode::SetAttribute(UnicodeString("NAME"), Value);
};

UnicodeString __fastcall TXMLPRINTJOBType::Get_DEVICENAME()
{
  return TXMLNode::AttributeNodes->Nodes[UnicodeString("DEVICENAME")]->Text;
};

void __fastcall TXMLPRINTJOBType::Set_DEVICENAME(UnicodeString Value)
{
  TXMLNode::SetAttribute(UnicodeString("DEVICENAME"), Value);
};

UnicodeString __fastcall TXMLPRINTJOBType::Get_DEVICEGUID()
{
  return TXMLNode::AttributeNodes->Nodes[UnicodeString("DEVICEGUID")]->Text;
};

void __fastcall TXMLPRINTJOBType::Set_DEVICEGUID(UnicodeString Value)
{
  TXMLNode::SetAttribute(UnicodeString("DEVICEGUID"), Value);
};

UnicodeString __fastcall TXMLPRINTJOBType::Get_TRANSNO()
{
  return TXMLNode::AttributeNodes->Nodes[UnicodeString("TRANSNO")]->Text;
};

void __fastcall TXMLPRINTJOBType::Set_TRANSNO(UnicodeString Value)
{
  TXMLNode::SetAttribute(UnicodeString("TRANSNO"), Value);
};

UnicodeString __fastcall TXMLPRINTJOBType::Get_STARTTIME()
{
  return TXMLNode::AttributeNodes->Nodes[UnicodeString("STARTTIME")]->Text;
};

void __fastcall TXMLPRINTJOBType::Set_STARTTIME(UnicodeString Value)
{
  TXMLNode::SetAttribute(UnicodeString("STARTTIME"), Value);
};

UnicodeString __fastcall TXMLPRINTJOBType::Get_OrderGUID(int Index)
{
  return TXMLNodeCollection::List->Nodes[Index]->Text;
};

_di_IXMLNode __fastcall TXMLPRINTJOBType::Add(const UnicodeString OrderGUID)
{
  _di_IXMLNode item = AddItem(-1);
  item->NodeValue = OrderGUID;
  return item;
};

_di_IXMLNode __fastcall TXMLPRINTJOBType::Insert(const int Index, const UnicodeString OrderGUID)
{
  _di_IXMLNode item = AddItem(Index);
  item->NodeValue = OrderGUID;
  return item;
};
}
