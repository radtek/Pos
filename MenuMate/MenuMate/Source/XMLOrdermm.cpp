
// ***************************************************************************************** //
//                                                                                         
//                                    XML Data Binding                                     
//                                                                                         
//         Generated on: 20/03/2012 4:50:18 p.m.                                           
//       Generated from: C:\Code\5.5\Static Files\XML Examples\Master Order Template.xml   
//   Settings stored in: C:\Code\5.5\Static Files\XML Examples\Master Order Template.xdb   
//                                                                                         
// ***************************************************************************************** //

#include <vcl.h>
#pragma hdrstop

#include "XMLOrdermm.h"


// Global Functions 

_di_IXMLORDERType __fastcall GetORDER(_di_IXMLDocument Doc)
{
  return (_di_IXMLORDERType) Doc->GetDocBinding("ORDER", __classid(TXMLORDERType), TargetNamespace);
};

_di_IXMLORDERType __fastcall GetORDER(TXMLDocument *Doc)
{
  _di_IXMLDocument DocIntf;
  Doc->GetInterface(DocIntf);
  return GetORDER(DocIntf);
};

_di_IXMLORDERType __fastcall LoadORDER(const UnicodeString& FileName)
{
  return (_di_IXMLORDERType) LoadXMLDocument(FileName)->GetDocBinding("ORDER", __classid(TXMLORDERType), TargetNamespace);
};

_di_IXMLORDERType __fastcall  NewORDER()
{
  return (_di_IXMLORDERType) NewXMLDocument()->GetDocBinding("ORDER", __classid(TXMLORDERType), TargetNamespace);
};

// TXMLORDERType 

void __fastcall TXMLORDERType::AfterConstruction(void)
{
  RegisterChildNode(UnicodeString("FROM"), __classid(TXMLFROMType));
  RegisterChildNode(UnicodeString("ACCOUNT"), __classid(TXMLACCOUNTType));
  TXMLNode::AfterConstruction();
};

UnicodeString __fastcall TXMLORDERType::Get_GUID()
{
  return TXMLNode::AttributeNodes->Nodes[UnicodeString("GUID")]->Text;
};

void __fastcall TXMLORDERType::Set_GUID(UnicodeString Value)
{
  TXMLNode::SetAttribute(UnicodeString("GUID"), Value);
};

UnicodeString __fastcall TXMLORDERType::Get_ORDER_NAME()
{
  return TXMLNode::AttributeNodes->Nodes[UnicodeString("ORDER_NAME")]->Text;
};

void __fastcall TXMLORDERType::Set_ORDER_NAME(UnicodeString Value)
{
  TXMLNode::SetAttribute(UnicodeString("ORDER_NAME"), Value);
};

UnicodeString __fastcall TXMLORDERType::Get_STORE_NAME()
{
  return TXMLNode::AttributeNodes->Nodes[UnicodeString("STORE_NAME")]->Text;
};

void __fastcall TXMLORDERType::Set_STORE_NAME(UnicodeString Value)
{
  TXMLNode::SetAttribute(UnicodeString("STORE_NAME"), Value);
};

UnicodeString __fastcall TXMLORDERType::Get_RESPOND()
{
  return TXMLNode::AttributeNodes->Nodes[UnicodeString("RESPOND")]->Text;
};

void __fastcall TXMLORDERType::Set_RESPOND(UnicodeString Value)
{
  TXMLNode::SetAttribute(UnicodeString("RESPOND"), Value);
};

UnicodeString __fastcall TXMLORDERType::Get_ORDER_DATE()
{
  return TXMLNode::AttributeNodes->Nodes[UnicodeString("ORDER_DATE")]->Text;
};

void __fastcall TXMLORDERType::Set_ORDER_DATE(UnicodeString Value)
{
  TXMLNode::SetAttribute(UnicodeString("ORDER_DATE"), Value);
};

UnicodeString __fastcall TXMLORDERType::Get_EXPECTED_DATE()
{
  return TXMLNode::AttributeNodes->Nodes[UnicodeString("EXPECTED_DATE")]->Text;
};

void __fastcall TXMLORDERType::Set_EXPECTED_DATE(UnicodeString Value)
{
  TXMLNode::SetAttribute(UnicodeString("EXPECTED_DATE"), Value);
};

Integer __fastcall TXMLORDERType::Get_PAYMENT_METHOD()
{
  return TXMLNode::AttributeNodes->Nodes[UnicodeString("PAYMENT_METHOD")]->NodeValue;
};

void __fastcall TXMLORDERType::Set_PAYMENT_METHOD(Integer Value)
{
  TXMLNode::SetAttribute(UnicodeString("PAYMENT_METHOD"), Value);
};

Integer __fastcall TXMLORDERType::Get_SCHEDULED()
{
  return TXMLNode::AttributeNodes->Nodes[UnicodeString("SCHEDULED")]->NodeValue;
};

void __fastcall TXMLORDERType::Set_SCHEDULED(Integer Value)
{
  TXMLNode::SetAttribute(UnicodeString("SCHEDULED"), Value);
};

UnicodeString __fastcall TXMLORDERType::Get_ORDER_TOTAL()
{
  return TXMLNode::AttributeNodes->Nodes[UnicodeString("ORDER_TOTAL")]->Text;
};

void __fastcall TXMLORDERType::Set_ORDER_TOTAL(UnicodeString Value)
{
  TXMLNode::SetAttribute(UnicodeString("ORDER_TOTAL"), Value);
};

_di_IXMLFROMType __fastcall TXMLORDERType::Get_FROM()
{
  return (_di_IXMLFROMType) TXMLNode::ChildNodes->Nodes[UnicodeString("FROM")];
};

_di_IXMLACCOUNTType __fastcall TXMLORDERType::Get_ACCOUNT()
{
  return (_di_IXMLACCOUNTType) TXMLNode::ChildNodes->Nodes[UnicodeString("ACCOUNT")];
};

// TXMLFROMType 

UnicodeString __fastcall TXMLFROMType::Get_ID()
{
  return TXMLNode::AttributeNodes->Nodes[UnicodeString("ID")]->Text;
};

void __fastcall TXMLFROMType::Set_ID(UnicodeString Value)
{
  TXMLNode::SetAttribute(UnicodeString("ID"), Value);
};

UnicodeString __fastcall TXMLFROMType::Get_NAME()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("NAME")]->Text;
};

void __fastcall TXMLFROMType::Set_NAME(UnicodeString Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("NAME")]->NodeValue = Value;
};

UnicodeString __fastcall TXMLFROMType::Get_AUTH_NAME()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("AUTH_NAME")]->Text;
};

void __fastcall TXMLFROMType::Set_AUTH_NAME(UnicodeString Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("AUTH_NAME")]->NodeValue = Value;
};

UnicodeString __fastcall TXMLFROMType::Get_AUTH_PASS()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("AUTH_PASS")]->Text;
};

void __fastcall TXMLFROMType::Set_AUTH_PASS(UnicodeString Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("AUTH_PASS")]->NodeValue = Value;
};

UnicodeString __fastcall TXMLFROMType::Get_PHONE_NUMBER()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("PHONE_NUMBER")]->Text;
};

void __fastcall TXMLFROMType::Set_PHONE_NUMBER(UnicodeString Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("PHONE_NUMBER")]->NodeValue = Value;
};

UnicodeString __fastcall TXMLFROMType::Get_STAFF_NAME()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("STAFF_NAME")]->Text;
};

void __fastcall TXMLFROMType::Set_STAFF_NAME(UnicodeString Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("STAFF_NAME")]->NodeValue = Value;
};

UnicodeString __fastcall TXMLFROMType::Get_STAFF_ID()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("STAFF_ID")]->Text;
};

void __fastcall TXMLFROMType::Set_STAFF_ID(UnicodeString Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("STAFF_ID")]->NodeValue = Value;
};

UnicodeString __fastcall TXMLFROMType::Get_STAFF_PASS()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("STAFF_PASS")]->Text;
};

void __fastcall TXMLFROMType::Set_STAFF_PASS(UnicodeString Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("STAFF_PASS")]->NodeValue = Value;
};

// TXMLACCOUNTType 

void __fastcall TXMLACCOUNTType::AfterConstruction(void)
{
  RegisterChildNode(UnicodeString("PAYMENTS"), __classid(TXMLPAYMENTSType));
  RegisterChildNode(UnicodeString("DELIVERY"), __classid(TXMLDELEVERYType));
  RegisterChildNode(UnicodeString("COMMENTS"), __classid(TXMLCOMMENTSType));
  RegisterChildNode(UnicodeString("PRODUCT"), __classid(TXMLPRODUCTType));
  CollectionCreater<_di_IXMLPRODUCTTypeList, TXMLPRODUCTTypeList, IXMLPRODUCTType>::DoCreate(this, FPRODUCT, "PRODUCT");
  TXMLNode::AfterConstruction();
};

UnicodeString __fastcall TXMLACCOUNTType::Get_MEMBER_NUMBER()
{
  return TXMLNode::AttributeNodes->Nodes[UnicodeString("MEMBER_NUMBER")]->Text;
};

void __fastcall TXMLACCOUNTType::Set_MEMBER_NUMBER(UnicodeString Value)
{
  TXMLNode::SetAttribute(UnicodeString("MEMBER_NUMBER"), Value);
};

UnicodeString __fastcall TXMLACCOUNTType::Get_NAME()
{
  return TXMLNode::AttributeNodes->Nodes[UnicodeString("NAME")]->Text;
};

void __fastcall TXMLACCOUNTType::Set_NAME(UnicodeString Value)
{
  TXMLNode::SetAttribute(UnicodeString("NAME"), Value);
};

Integer __fastcall TXMLACCOUNTType::Get_COVERS()
{
  return TXMLNode::AttributeNodes->Nodes[UnicodeString("COVERS")]->NodeValue;
};

void __fastcall TXMLACCOUNTType::Set_COVERS(Integer Value)
{
  TXMLNode::SetAttribute(UnicodeString("COVERS"), Value);
};

_di_IXMLPAYMENTSType __fastcall TXMLACCOUNTType::Get_PAYMENTS()
{
  return (_di_IXMLPAYMENTSType) TXMLNode::ChildNodes->Nodes[UnicodeString("PAYMENTS")];
};

_di_IXMLDELEVERYType __fastcall TXMLACCOUNTType::Get_DELEVERY()
{
  return (_di_IXMLDELEVERYType) TXMLNode::ChildNodes->Nodes[UnicodeString("DELIVERY")];
};

_di_IXMLCOMMENTSType __fastcall TXMLACCOUNTType::Get_COMMENTS()
{
  return (_di_IXMLCOMMENTSType) TXMLNode::ChildNodes->Nodes[UnicodeString("COMMENTS")];
};

_di_IXMLPRODUCTTypeList __fastcall TXMLACCOUNTType::Get_PRODUCT()
{
  return (_di_IXMLPRODUCTTypeList) FPRODUCT;
};

// TXMLPAYMENTSType 

void __fastcall TXMLPAYMENTSType::AfterConstruction(void)
{
  RegisterChildNode(UnicodeString("PAYMENT_INFO"), __classid(TXMLPAYMENT_INFOType));
  RegisterChildNode(UnicodeString("PAYMENT"), __classid(TXMLPAYMENTType));
  CollectionCreater<_di_IXMLPAYMENTTypeList, TXMLPAYMENTTypeList, IXMLPAYMENTType>::DoCreate(this, FPAYMENT, "PAYMENT");
  TXMLNode::AfterConstruction();
};

_di_IXMLPAYMENT_INFOType __fastcall TXMLPAYMENTSType::Get_PAYMENT_INFO()
{
  return (_di_IXMLPAYMENT_INFOType) TXMLNode::ChildNodes->Nodes[UnicodeString("PAYMENT_INFO")];
};

_di_IXMLPAYMENTTypeList __fastcall TXMLPAYMENTSType::Get_PAYMENT()
{
  return (_di_IXMLPAYMENTTypeList) FPAYMENT;
};

// TXMLPAYMENT_INFOType 

void __fastcall TXMLPAYMENT_INFOType::AfterConstruction(void)
{
  ItemTag = "INFO";
  ItemInterface = __uuidof(IXMLNode);
  TXMLNodeCollection::AfterConstruction();
};

UnicodeString __fastcall TXMLPAYMENT_INFOType::Get_INFO(int Index)
{
  return TXMLNodeCollection::List->Nodes[Index]->Text;
};

_di_IXMLNode __fastcall TXMLPAYMENT_INFOType::Add(const UnicodeString INFO)
{
  _di_IXMLNode item = AddItem(-1);
  item->NodeValue = INFO;
  return item;
};

_di_IXMLNode __fastcall TXMLPAYMENT_INFOType::Insert(const int Index, const UnicodeString INFO)
{
  _di_IXMLNode item = AddItem(Index);
  item->NodeValue = INFO;
  return item;
};

// TXMLPAYMENTType 

UnicodeString __fastcall TXMLPAYMENTType::Get_NAME()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("NAME")]->Text;
};

void __fastcall TXMLPAYMENTType::Set_NAME(UnicodeString Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("NAME")]->NodeValue = Value;
};

UnicodeString __fastcall TXMLPAYMENTType::Get_VALUE()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("VALUE")]->Text;
};

void __fastcall TXMLPAYMENTType::Set_VALUE(UnicodeString Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("VALUE")]->NodeValue = Value;
};

// TXMLPAYMENTTypeList 

_di_IXMLPAYMENTType __fastcall TXMLPAYMENTTypeList::Add()
{
  return (_di_IXMLPAYMENTType) AddItem(-1);
};

_di_IXMLPAYMENTType __fastcall TXMLPAYMENTTypeList::Insert(const int Index)
{
  return (_di_IXMLPAYMENTType) AddItem(Index);
};
_di_IXMLPAYMENTType __fastcall TXMLPAYMENTTypeList::Get_Item(int Index)
{
  return (_di_IXMLPAYMENTType) TXMLNodeCollection::List->Nodes[Index];
};

// TXMLDELEVERYType 

UnicodeString __fastcall TXMLDELEVERYType::Get_TO()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("TO")]->Text;
};

void __fastcall TXMLDELEVERYType::Set_TO(UnicodeString Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("TO")]->NodeValue = Value;
};

UnicodeString __fastcall TXMLDELEVERYType::Get_PHONE()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("PHONE")]->Text;
};

void __fastcall TXMLDELEVERYType::Set_PHONE(UnicodeString Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("PHONE")]->NodeValue = Value;
};

UnicodeString __fastcall TXMLDELEVERYType::Get_STREET_NO()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("STREET_NO")]->Text;
};

void __fastcall TXMLDELEVERYType::Set_STREET_NO(UnicodeString Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("STREET_NO")]->NodeValue = Value;
};

UnicodeString __fastcall TXMLDELEVERYType::Get_STREET_NAME()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("STREET_NAME")]->Text;
};

void __fastcall TXMLDELEVERYType::Set_STREET_NAME(UnicodeString Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("STREET_NAME")]->NodeValue = Value;
};

UnicodeString __fastcall TXMLDELEVERYType::Get_SUBURB()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("SUBURB")]->Text;
};

void __fastcall TXMLDELEVERYType::Set_SUBURB(UnicodeString Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("SUBURB")]->NodeValue = Value;
};

UnicodeString __fastcall TXMLDELEVERYType::Get_POST_CODE()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("POST_CODE")]->Text;
};

void __fastcall TXMLDELEVERYType::Set_POST_CODE(UnicodeString Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("POST_CODE")]->NodeValue = Value;
};

UnicodeString __fastcall TXMLDELEVERYType::Get_CITY()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("CITY")]->Text;
};

void __fastcall TXMLDELEVERYType::Set_CITY(UnicodeString Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("CITY")]->NodeValue = Value;
};

UnicodeString __fastcall TXMLDELEVERYType::Get_COUNTRY()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("COUNTRY")]->Text;
};

void __fastcall TXMLDELEVERYType::Set_COUNTRY(UnicodeString Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("COUNTRY")]->NodeValue = Value;
};

UnicodeString __fastcall TXMLDELEVERYType::Get_NOTE()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("NOTE")]->Text;
};

void __fastcall TXMLDELEVERYType::Set_NOTE(UnicodeString Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("NOTE")]->NodeValue = Value;
};

// TXMLCOMMENTSType 

void __fastcall TXMLCOMMENTSType::AfterConstruction(void)
{
  ItemTag = "COMMENT";
  ItemInterface = __uuidof(IXMLNode);
  TXMLNodeCollection::AfterConstruction();
};

UnicodeString __fastcall TXMLCOMMENTSType::Get_COMMENT(int Index)
{
  return TXMLNodeCollection::List->Nodes[Index]->Text;
};

_di_IXMLNode __fastcall TXMLCOMMENTSType::Add(const UnicodeString COMMENT)
{
  _di_IXMLNode item = AddItem(-1);
  item->NodeValue = COMMENT;
  return item;
};

_di_IXMLNode __fastcall TXMLCOMMENTSType::Insert(const int Index, const UnicodeString COMMENT)
{
  _di_IXMLNode item = AddItem(Index);
  item->NodeValue = COMMENT;
  return item;
};

// TXMLPRODUCTType 

void __fastcall TXMLPRODUCTType::AfterConstruction(void)
{
  RegisterChildNode(UnicodeString("OPTION"), __classid(TXMLOPTIONType));
  RegisterChildNode(UnicodeString("PRODUCTIDENT"), __classid(TXMLPRODUCTIDENTType));
  RegisterChildNode(UnicodeString("PRODUCT"), __classid(TXMLPRODUCTType));
  CollectionCreater<_di_IXMLOPTIONTypeList, TXMLOPTIONTypeList, IXMLOPTIONType>::DoCreate(this, FOPTION, "OPTION");
  CollectionCreater<_di_IXMLPRODUCTTypeList, TXMLPRODUCTTypeList, IXMLPRODUCTType>::DoCreate(this, FPRODUCT, "PRODUCT");
  TXMLNode::AfterConstruction();
};

UnicodeString __fastcall TXMLPRODUCTType::Get_GUID()
{
  return TXMLNode::AttributeNodes->Nodes[UnicodeString("GUID")]->Text;
};

void __fastcall TXMLPRODUCTType::Set_GUID(UnicodeString Value)
{
  TXMLNode::SetAttribute(UnicodeString("GUID"), Value);
};

UnicodeString __fastcall TXMLPRODUCTType::Get_SERVINGCOURSE()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("SERVINGCOURSE")]->Text;
};

void __fastcall TXMLPRODUCTType::Set_SERVINGCOURSE(UnicodeString Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("SERVINGCOURSE")]->NodeValue = Value;
};

UnicodeString __fastcall TXMLPRODUCTType::Get_PRODUCT_NAME()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("PRODUCT_NAME")]->Text;
};

void __fastcall TXMLPRODUCTType::Set_PRODUCT_NAME(UnicodeString Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("PRODUCT_NAME")]->NodeValue = Value;
};

UnicodeString __fastcall TXMLPRODUCTType::Get_SIZE()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("SIZE")]->Text;
};

void __fastcall TXMLPRODUCTType::Set_SIZE(UnicodeString Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("SIZE")]->NodeValue = Value;
};

UnicodeString __fastcall TXMLPRODUCTType::Get_TYPE()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("TYPE")]->Text;
};

void __fastcall TXMLPRODUCTType::Set_TYPE(UnicodeString Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("TYPE")]->NodeValue = Value;
};

UnicodeString __fastcall TXMLPRODUCTType::Get_PRODUCT_PLU()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("PRODUCT_PLU")]->Text;
};

void __fastcall TXMLPRODUCTType::Set_PRODUCT_PLU(UnicodeString Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("PRODUCT_PLU")]->NodeValue = Value;
};

UnicodeString __fastcall TXMLPRODUCTType::Get_PRODUCT_BASE_PRICE()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("PRODUCT_BASE_PRICE")]->Text;
};

void __fastcall TXMLPRODUCTType::Set_PRODUCT_BASE_PRICE(UnicodeString Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("PRODUCT_BASE_PRICE")]->NodeValue = Value;
};

UnicodeString __fastcall TXMLPRODUCTType::Get_PRODUCT_CHARGE_PRICE()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("PRODUCT_CHARGE_PRICE")]->Text;
};

void __fastcall TXMLPRODUCTType::Set_PRODUCT_CHARGE_PRICE(UnicodeString Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("PRODUCT_CHARGE_PRICE")]->NodeValue = Value;
};

Integer __fastcall TXMLPRODUCTType::Get_PRICELEVEL()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("PRICELEVEL")]->NodeValue;
};

void __fastcall TXMLPRODUCTType::Set_PRICELEVEL(Integer Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("PRICELEVEL")]->NodeValue = Value;
};

Integer __fastcall TXMLPRODUCTType::Get_QTY()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("QTY")]->NodeValue;
};

void __fastcall TXMLPRODUCTType::Set_QTY(Integer Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("QTY")]->NodeValue = Value;
};

UnicodeString __fastcall TXMLPRODUCTType::Get_PRODUCT_COMMENT()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("PRODUCT_COMMENT")]->Text;
};

void __fastcall TXMLPRODUCTType::Set_PRODUCT_COMMENT(UnicodeString Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("PRODUCT_COMMENT")]->NodeValue = Value;
};

_di_IXMLOPTIONTypeList __fastcall TXMLPRODUCTType::Get_OPTION()
{
  return (_di_IXMLOPTIONTypeList) FOPTION;
};

_di_IXMLPRODUCTIDENTType __fastcall TXMLPRODUCTType::Get_PRODUCTIDENT()
{
  return (_di_IXMLPRODUCTIDENTType) TXMLNode::ChildNodes->Nodes[UnicodeString("PRODUCTIDENT")];
};

_di_IXMLPRODUCTTypeList __fastcall TXMLPRODUCTType::Get_PRODUCT()
{
  return (_di_IXMLPRODUCTTypeList) FPRODUCT;
};

// TXMLPRODUCTTypeList 

_di_IXMLPRODUCTType __fastcall TXMLPRODUCTTypeList::Add()
{
  return (_di_IXMLPRODUCTType) AddItem(-1);
};

_di_IXMLPRODUCTType __fastcall TXMLPRODUCTTypeList::Insert(const int Index)
{
  return (_di_IXMLPRODUCTType) AddItem(Index);
};
_di_IXMLPRODUCTType __fastcall TXMLPRODUCTTypeList::Get_Item(int Index)
{
  return (_di_IXMLPRODUCTType) TXMLNodeCollection::List->Nodes[Index];
};

// TXMLOPTIONType 

Integer __fastcall TXMLOPTIONType::Get_Key()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("KEY")]->NodeValue;
};

void __fastcall TXMLOPTIONType::Set_Key(Integer Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("KEY")]->NodeValue = Value;
};

UnicodeString __fastcall TXMLOPTIONType::Get_Name()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("NAME")]->Text;
};

void __fastcall TXMLOPTIONType::Set_Name(UnicodeString Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("NAME")]->NodeValue = Value;
};

// TXMLOPTIONTypeList 

_di_IXMLOPTIONType __fastcall TXMLOPTIONTypeList::Add()
{
  return (_di_IXMLOPTIONType) AddItem(-1);
};

_di_IXMLOPTIONType __fastcall TXMLOPTIONTypeList::Insert(const int Index)
{
  return (_di_IXMLOPTIONType) AddItem(Index);
};
_di_IXMLOPTIONType __fastcall TXMLOPTIONTypeList::Get_Item(int Index)
{
  return (_di_IXMLOPTIONType) TXMLNodeCollection::List->Nodes[Index];
};

// TXMLPRODUCTIDENTType 

Integer __fastcall TXMLPRODUCTIDENTType::Get_MODULEID()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("MODULEID")]->NodeValue;
};

void __fastcall TXMLPRODUCTIDENTType::Set_MODULEID(Integer Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("MODULEID")]->NodeValue = Value;
};

Integer __fastcall TXMLPRODUCTIDENTType::Get_MENUKEY()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("MENUKEY")]->NodeValue;
};

void __fastcall TXMLPRODUCTIDENTType::Set_MENUKEY(Integer Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("MENUKEY")]->NodeValue = Value;
};

Integer __fastcall TXMLPRODUCTIDENTType::Get_COURSEKEY()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("COURSEKEY")]->NodeValue;
};

void __fastcall TXMLPRODUCTIDENTType::Set_COURSEKEY(Integer Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("COURSEKEY")]->NodeValue = Value;
};

Integer __fastcall TXMLPRODUCTIDENTType::Get_ITEMKEY()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("ITEMKEY")]->NodeValue;
};

void __fastcall TXMLPRODUCTIDENTType::Set_ITEMKEY(Integer Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("ITEMKEY")]->NodeValue = Value;
};

Integer __fastcall TXMLPRODUCTIDENTType::Get_SIZEKEY()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("SIZEKEY")]->NodeValue;
};

void __fastcall TXMLPRODUCTIDENTType::Set_SIZEKEY(Integer Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("SIZEKEY")]->NodeValue = Value;
};

Integer __fastcall TXMLPRODUCTIDENTType::Get_SERVINGCOURSEKEY()
{
  return TXMLNode::ChildNodes->Nodes[UnicodeString("SERVINGCOURSEKEY")]->NodeValue;
};

void __fastcall TXMLPRODUCTIDENTType::Set_SERVINGCOURSEKEY(Integer Value)
{
  TXMLNode::ChildNodes->Nodes[UnicodeString("SERVINGCOURSEKEY")]->NodeValue = Value;
};
