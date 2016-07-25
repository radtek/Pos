
// *************************************************************************************************************** //
//                                                                                                               
//                                               XML Data Binding                                                
//                                                                                                               
//         Generated on: 24/06/2011 9:16:40 p.m.                                                                 
//       Generated from: C:\Users\michael\dev\5.2.trunk\Static Files\XML Examples\Master CallAway Template.xml   
//                                                                                                               
// *************************************************************************************************************** //

#include <vcl.h>
#pragma hdrstop

#include "XMLCallAwaymm.h"

namespace XMLCallAways
{


// Global Functions 

_di_IXMLCALLAWAYType __fastcall GetCALLAWAY(_di_IXMLDocument Doc)
{
  return (_di_IXMLCALLAWAYType) Doc->GetDocBinding("CALLAWAY", __classid(TXMLCALLAWAYType), TargetNamespace);
};

_di_IXMLCALLAWAYType __fastcall GetCALLAWAY(TXMLDocument *Doc)
{
  _di_IXMLDocument DocIntf;
  Doc->GetInterface(DocIntf);
  return GetCALLAWAY(DocIntf);
};

_di_IXMLCALLAWAYType __fastcall LoadCALLAWAY(const UnicodeString& FileName)
{
  return (_di_IXMLCALLAWAYType) LoadXMLDocument(FileName)->GetDocBinding("CALLAWAY", __classid(TXMLCALLAWAYType), TargetNamespace);
};

_di_IXMLCALLAWAYType __fastcall  NewCALLAWAY()
{
  return (_di_IXMLCALLAWAYType) NewXMLDocument()->GetDocBinding("CALLAWAY", __classid(TXMLCALLAWAYType), TargetNamespace);
};

// TXMLCALLAWAYType 

void __fastcall TXMLCALLAWAYType::AfterConstruction(void)
{
  RegisterChildNode(UnicodeString("ACCOUNT"), __classid(TXMLACCOUNTType));
  TXMLNode::AfterConstruction();
};

_di_IXMLACCOUNTType __fastcall TXMLCALLAWAYType::Get_ACCOUNT()
{
  return (_di_IXMLACCOUNTType) TXMLNode::ChildNodes->Nodes[UnicodeString("ACCOUNT")];
};

// TXMLACCOUNTType 

void __fastcall TXMLACCOUNTType::AfterConstruction(void)
{
  RegisterChildNode(UnicodeString("ACCOUNTIDENT"), __classid(TXMLACCOUNTIDENTType));
  RegisterChildNode(UnicodeString("SERVINGCOURSE"), __classid(TXMLSERVINGCOURSEType));
  CollectionCreater<_di_IXMLSERVINGCOURSETypeList, TXMLSERVINGCOURSETypeList, IXMLSERVINGCOURSEType>::DoCreate(this, FSERVINGCOURSE, "SERVINGCOURSE");
  TXMLNode::AfterConstruction();
};

UnicodeString __fastcall TXMLACCOUNTType::Get_NAME()
{
  return TXMLNode::AttributeNodes->Nodes[UnicodeString("NAME")]->Text;
};

void __fastcall TXMLACCOUNTType::Set_NAME(UnicodeString Value)
{
  TXMLNode::SetAttribute(UnicodeString("NAME"), Value);
};

Integer __fastcall TXMLACCOUNTType::Get_KEY()
{
  return TXMLNode::AttributeNodes->Nodes[UnicodeString("KEY")]->NodeValue;
};

void __fastcall TXMLACCOUNTType::Set_KEY(Integer Value)
{
  TXMLNode::SetAttribute(UnicodeString("KEY"), Value);
};

Integer __fastcall TXMLACCOUNTType::Get_COVERS()
{
  return TXMLNode::AttributeNodes->Nodes[UnicodeString("COVERS")]->NodeValue;
};

void __fastcall TXMLACCOUNTType::Set_COVERS(Integer Value)
{
  TXMLNode::SetAttribute(UnicodeString("COVERS"), Value);
};

UnicodeString __fastcall TXMLACCOUNTType::Get_TYPE()
{
  return TXMLNode::AttributeNodes->Nodes[UnicodeString("TYPE")]->Text;
};

void __fastcall TXMLACCOUNTType::Set_TYPE(UnicodeString Value)
{
  TXMLNode::SetAttribute(UnicodeString("TYPE"), Value);
};

_di_IXMLACCOUNTIDENTType __fastcall TXMLACCOUNTType::Get_ACCOUNTIDENT()
{
  return (_di_IXMLACCOUNTIDENTType) TXMLNode::ChildNodes->Nodes[UnicodeString("ACCOUNTIDENT")];
};

_di_IXMLSERVINGCOURSETypeList __fastcall TXMLACCOUNTType::Get_SERVINGCOURSE()
{
  return (_di_IXMLSERVINGCOURSETypeList) FSERVINGCOURSE;
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

// TXMLSERVINGCOURSEType 

Integer __fastcall TXMLSERVINGCOURSEType::Get_KEY()
{
  return TXMLNode::AttributeNodes->Nodes[UnicodeString("KEY")]->NodeValue;
};

void __fastcall TXMLSERVINGCOURSEType::Set_KEY(Integer Value)
{
  TXMLNode::SetAttribute(UnicodeString("KEY"), Value);
};

UnicodeString __fastcall TXMLSERVINGCOURSEType::Get_NAME()
{
  return TXMLNode::AttributeNodes->Nodes[UnicodeString("NAME")]->Text;
};

void __fastcall TXMLSERVINGCOURSEType::Set_NAME(UnicodeString Value)
{
  TXMLNode::SetAttribute(UnicodeString("NAME"), Value);
};

// TXMLSERVINGCOURSETypeList 

_di_IXMLSERVINGCOURSEType __fastcall TXMLSERVINGCOURSETypeList::Add()
{
  return (_di_IXMLSERVINGCOURSEType) AddItem(-1);
};

_di_IXMLSERVINGCOURSEType __fastcall TXMLSERVINGCOURSETypeList::Insert(const int Index)
{
  return (_di_IXMLSERVINGCOURSEType) AddItem(Index);
};
_di_IXMLSERVINGCOURSEType __fastcall TXMLSERVINGCOURSETypeList::Get_Item(int Index)
{
  return (_di_IXMLSERVINGCOURSEType) TXMLNodeCollection::List->Nodes[Index];
};

}
