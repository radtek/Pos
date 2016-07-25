
// ***************************************************************************************** //
//                                                                                         
//                                    XML Data Binding                                     
//                                                                                         
//         Generated on: 20/03/2012 4:50:19 p.m.                                           
//       Generated from: C:\Code\5.5\Static Files\XML Examples\Master Order Template.xml   
//   Settings stored in: C:\Code\5.5\Static Files\XML Examples\Master Order Template.xdb   
//                                                                                         
// ***************************************************************************************** //

#ifndef   XMLOrdermmH
#define   XMLOrdermmH

#include <System.hpp>
#include <xmldom.hpp>
#include <XMLDoc.hpp>
#include <XMLIntf.hpp>
#include <XMLNodeImp.h>
#include "xmlInodeDec.h"


// Forward Decls 

__interface IXMLORDERType;
typedef System::DelphiInterface<IXMLORDERType> _di_IXMLORDERType;
__interface IXMLFROMType;
typedef System::DelphiInterface<IXMLFROMType> _di_IXMLFROMType;
__interface IXMLACCOUNTType;
typedef System::DelphiInterface<IXMLACCOUNTType> _di_IXMLACCOUNTType;
__interface IXMLPAYMENTSType;
typedef System::DelphiInterface<IXMLPAYMENTSType> _di_IXMLPAYMENTSType;
__interface IXMLPAYMENT_INFOType;
typedef System::DelphiInterface<IXMLPAYMENT_INFOType> _di_IXMLPAYMENT_INFOType;
__interface IXMLPAYMENTType;
typedef System::DelphiInterface<IXMLPAYMENTType> _di_IXMLPAYMENTType;
__interface IXMLPAYMENTTypeList;
typedef System::DelphiInterface<IXMLPAYMENTTypeList> _di_IXMLPAYMENTTypeList;
__interface IXMLDELEVERYType;
typedef System::DelphiInterface<IXMLDELEVERYType> _di_IXMLDELEVERYType;
__interface IXMLCOMMENTSType;
typedef System::DelphiInterface<IXMLCOMMENTSType> _di_IXMLCOMMENTSType;
__interface IXMLPRODUCTType;
typedef System::DelphiInterface<IXMLPRODUCTType> _di_IXMLPRODUCTType;
__interface IXMLPRODUCTTypeList;
typedef System::DelphiInterface<IXMLPRODUCTTypeList> _di_IXMLPRODUCTTypeList;
__interface IXMLOPTIONType;
typedef System::DelphiInterface<IXMLOPTIONType> _di_IXMLOPTIONType;
__interface IXMLOPTIONTypeList;
typedef System::DelphiInterface<IXMLOPTIONTypeList> _di_IXMLOPTIONTypeList;
__interface IXMLPRODUCTIDENTType;
typedef System::DelphiInterface<IXMLPRODUCTIDENTType> _di_IXMLPRODUCTIDENTType;

// IXMLORDERType 

__interface INTERFACE_UUID("{5A1C73BD-7875-419C-A5D7-F2147940CA57}") IXMLORDERType : public IXMLNode
{
public:
  // Property Accessors 
  virtual UnicodeString __fastcall Get_GUID() = 0;
  virtual UnicodeString __fastcall Get_ORDER_NAME() = 0;
  virtual UnicodeString __fastcall Get_STORE_NAME() = 0;
  virtual UnicodeString __fastcall Get_RESPOND() = 0;
  virtual UnicodeString __fastcall Get_ORDER_DATE() = 0;
  virtual UnicodeString __fastcall Get_EXPECTED_DATE() = 0;
  virtual Integer __fastcall Get_PAYMENT_METHOD() = 0;
  virtual Integer __fastcall Get_SCHEDULED() = 0;
  virtual UnicodeString __fastcall Get_ORDER_TOTAL() = 0;
  virtual _di_IXMLFROMType __fastcall Get_FROM() = 0;
  virtual _di_IXMLACCOUNTType __fastcall Get_ACCOUNT() = 0;
  virtual void __fastcall Set_GUID(UnicodeString Value) = 0;
  virtual void __fastcall Set_ORDER_NAME(UnicodeString Value) = 0;
  virtual void __fastcall Set_STORE_NAME(UnicodeString Value) = 0;
  virtual void __fastcall Set_RESPOND(UnicodeString Value) = 0;
  virtual void __fastcall Set_ORDER_DATE(UnicodeString Value) = 0;
  virtual void __fastcall Set_EXPECTED_DATE(UnicodeString Value) = 0;
  virtual void __fastcall Set_PAYMENT_METHOD(Integer Value) = 0;
  virtual void __fastcall Set_SCHEDULED(Integer Value) = 0;
  virtual void __fastcall Set_ORDER_TOTAL(UnicodeString Value) = 0;
  // Methods & Properties 
  __property UnicodeString GUID = { read=Get_GUID, write=Set_GUID };
  __property UnicodeString ORDER_NAME = { read=Get_ORDER_NAME, write=Set_ORDER_NAME };
  __property UnicodeString STORE_NAME = { read=Get_STORE_NAME, write=Set_STORE_NAME };
  __property UnicodeString RESPOND = { read=Get_RESPOND, write=Set_RESPOND };
  __property UnicodeString ORDER_DATE = { read=Get_ORDER_DATE, write=Set_ORDER_DATE };
  __property UnicodeString EXPECTED_DATE = { read=Get_EXPECTED_DATE, write=Set_EXPECTED_DATE };
  __property Integer PAYMENT_METHOD = { read=Get_PAYMENT_METHOD, write=Set_PAYMENT_METHOD };
  __property Integer SCHEDULED = { read=Get_SCHEDULED, write=Set_SCHEDULED };
  __property UnicodeString ORDER_TOTAL = { read=Get_ORDER_TOTAL, write=Set_ORDER_TOTAL };
  __property _di_IXMLFROMType FROM = { read=Get_FROM };
  __property _di_IXMLACCOUNTType ACCOUNT = { read=Get_ACCOUNT };
};

// IXMLFROMType 

__interface INTERFACE_UUID("{EF682BAF-C134-43AB-8609-E909154A0B28}") IXMLFROMType : public IXMLNode
{
public:
  // Property Accessors 
  virtual UnicodeString __fastcall Get_ID() = 0;
  virtual UnicodeString __fastcall Get_NAME() = 0;
  virtual UnicodeString __fastcall Get_AUTH_NAME() = 0;
  virtual UnicodeString __fastcall Get_AUTH_PASS() = 0;
  virtual UnicodeString __fastcall Get_PHONE_NUMBER() = 0;
  virtual UnicodeString __fastcall Get_STAFF_NAME() = 0;
  virtual UnicodeString __fastcall Get_STAFF_ID() = 0;
  virtual UnicodeString __fastcall Get_STAFF_PASS() = 0;
  virtual void __fastcall Set_ID(UnicodeString Value) = 0;
  virtual void __fastcall Set_NAME(UnicodeString Value) = 0;
  virtual void __fastcall Set_AUTH_NAME(UnicodeString Value) = 0;
  virtual void __fastcall Set_AUTH_PASS(UnicodeString Value) = 0;
  virtual void __fastcall Set_PHONE_NUMBER(UnicodeString Value) = 0;
  virtual void __fastcall Set_STAFF_NAME(UnicodeString Value) = 0;
  virtual void __fastcall Set_STAFF_ID(UnicodeString Value) = 0;
  virtual void __fastcall Set_STAFF_PASS(UnicodeString Value) = 0;
  // Methods & Properties 
  __property UnicodeString ID = { read=Get_ID, write=Set_ID };
  __property UnicodeString NAME = { read=Get_NAME, write=Set_NAME };
  __property UnicodeString AUTH_NAME = { read=Get_AUTH_NAME, write=Set_AUTH_NAME };
  __property UnicodeString AUTH_PASS = { read=Get_AUTH_PASS, write=Set_AUTH_PASS };
  __property UnicodeString PHONE_NUMBER = { read=Get_PHONE_NUMBER, write=Set_PHONE_NUMBER };
  __property UnicodeString STAFF_NAME = { read=Get_STAFF_NAME, write=Set_STAFF_NAME };
  __property UnicodeString STAFF_ID = { read=Get_STAFF_ID, write=Set_STAFF_ID };
  __property UnicodeString STAFF_PASS = { read=Get_STAFF_PASS, write=Set_STAFF_PASS };
};

// IXMLACCOUNTType 

__interface INTERFACE_UUID("{39AC5F35-E919-4EBE-BE2A-71AD0D3C838A}") IXMLACCOUNTType : public IXMLNode
{
public:
  // Property Accessors 
  virtual UnicodeString __fastcall Get_MEMBER_NUMBER() = 0;
  virtual UnicodeString __fastcall Get_NAME() = 0;
  virtual Integer __fastcall Get_COVERS() = 0;
  virtual _di_IXMLPAYMENTSType __fastcall Get_PAYMENTS() = 0;
  virtual _di_IXMLDELEVERYType __fastcall Get_DELEVERY() = 0;
  virtual _di_IXMLCOMMENTSType __fastcall Get_COMMENTS() = 0;
  virtual _di_IXMLPRODUCTTypeList __fastcall Get_PRODUCT() = 0;
  virtual void __fastcall Set_MEMBER_NUMBER(UnicodeString Value) = 0;
  virtual void __fastcall Set_NAME(UnicodeString Value) = 0;
  virtual void __fastcall Set_COVERS(Integer Value) = 0;
  // Methods & Properties 
  __property UnicodeString MEMBER_NUMBER = { read=Get_MEMBER_NUMBER, write=Set_MEMBER_NUMBER };
  __property UnicodeString NAME = { read=Get_NAME, write=Set_NAME };
  __property Integer COVERS = { read=Get_COVERS, write=Set_COVERS };
  __property _di_IXMLPAYMENTSType PAYMENTS = { read=Get_PAYMENTS };
  __property _di_IXMLDELEVERYType DELEVERY = { read=Get_DELEVERY };
  __property _di_IXMLCOMMENTSType COMMENTS = { read=Get_COMMENTS };
  __property _di_IXMLPRODUCTTypeList PRODUCT = { read=Get_PRODUCT };
};

// IXMLPAYMENTSType 

__interface INTERFACE_UUID("{604971B5-604B-49C4-A17E-6713CC88688F}") IXMLPAYMENTSType : public IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLPAYMENT_INFOType __fastcall Get_PAYMENT_INFO() = 0;
  virtual _di_IXMLPAYMENTTypeList __fastcall Get_PAYMENT() = 0;
  // Methods & Properties 
  __property _di_IXMLPAYMENT_INFOType PAYMENT_INFO = { read=Get_PAYMENT_INFO };
  __property _di_IXMLPAYMENTTypeList PAYMENT = { read=Get_PAYMENT };
};

// IXMLPAYMENT_INFOType 

__interface INTERFACE_UUID("{525469F3-7B57-457A-999C-BE7B6DCC9DE1}") IXMLPAYMENT_INFOType : public IXMLNodeCollection
{
public:
public:
  // Property Accessors 
  virtual UnicodeString __fastcall Get_INFO(int Index) = 0;
  // Methods & Properties 
  virtual _di_IXMLNode __fastcall Add(const UnicodeString INFO) = 0;
  virtual _di_IXMLNode __fastcall Insert(const int Index, const UnicodeString INFO) = 0;
  __property UnicodeString INFO[int Index] = { read=Get_INFO };/* default */
};

// IXMLPAYMENTType 

__interface INTERFACE_UUID("{5FC24DCC-74CE-4708-8906-C98D0ABB2739}") IXMLPAYMENTType : public IXMLNode
{
public:
  // Property Accessors 
  virtual UnicodeString __fastcall Get_NAME() = 0;
  virtual UnicodeString __fastcall Get_VALUE() = 0;
  virtual void __fastcall Set_NAME(UnicodeString Value) = 0;
  virtual void __fastcall Set_VALUE(UnicodeString Value) = 0;
  // Methods & Properties 
  __property UnicodeString NAME = { read=Get_NAME, write=Set_NAME };
  __property UnicodeString VALUE = { read=Get_VALUE, write=Set_VALUE };
};

// IXMLPAYMENTTypeList 

__interface INTERFACE_UUID("{ABCA9F86-3EB3-4023-80DE-6424E34C9E79}") IXMLPAYMENTTypeList : public IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLPAYMENTType __fastcall Add() = 0;
  virtual _di_IXMLPAYMENTType __fastcall Insert(const int Index) = 0;
  virtual _di_IXMLPAYMENTType __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLPAYMENTType Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLDELEVERYType 

__interface INTERFACE_UUID("{779ADF81-B542-4A62-9D1D-B9DF2A727129}") IXMLDELEVERYType : public IXMLNode
{
public:
  // Property Accessors 
  virtual UnicodeString __fastcall Get_TO() = 0;
  virtual UnicodeString __fastcall Get_PHONE() = 0;
  virtual UnicodeString __fastcall Get_STREET_NO() = 0;
  virtual UnicodeString __fastcall Get_STREET_NAME() = 0;
  virtual UnicodeString __fastcall Get_SUBURB() = 0;
  virtual UnicodeString __fastcall Get_POST_CODE() = 0;
  virtual UnicodeString __fastcall Get_CITY() = 0;
  virtual UnicodeString __fastcall Get_COUNTRY() = 0;
  virtual UnicodeString __fastcall Get_NOTE() = 0;
  virtual void __fastcall Set_TO(UnicodeString Value) = 0;
  virtual void __fastcall Set_PHONE(UnicodeString Value) = 0;
  virtual void __fastcall Set_STREET_NO(UnicodeString Value) = 0;
  virtual void __fastcall Set_STREET_NAME(UnicodeString Value) = 0;
  virtual void __fastcall Set_SUBURB(UnicodeString Value) = 0;
  virtual void __fastcall Set_POST_CODE(UnicodeString Value) = 0;
  virtual void __fastcall Set_CITY(UnicodeString Value) = 0;
  virtual void __fastcall Set_COUNTRY(UnicodeString Value) = 0;
  virtual void __fastcall Set_NOTE(UnicodeString Value) = 0;
  // Methods & Properties 
  __property UnicodeString TO = { read=Get_TO, write=Set_TO };
  __property UnicodeString PHONE = { read=Get_PHONE, write=Set_PHONE };
  __property UnicodeString STREET_NO = { read=Get_STREET_NO, write=Set_STREET_NO };
  __property UnicodeString STREET_NAME = { read=Get_STREET_NAME, write=Set_STREET_NAME };
  __property UnicodeString SUBURB = { read=Get_SUBURB, write=Set_SUBURB };
  __property UnicodeString POST_CODE = { read=Get_POST_CODE, write=Set_POST_CODE };
  __property UnicodeString CITY = { read=Get_CITY, write=Set_CITY };
  __property UnicodeString COUNTRY = { read=Get_COUNTRY, write=Set_COUNTRY };
  __property UnicodeString NOTE = { read=Get_NOTE, write=Set_NOTE };
};

// IXMLCOMMENTSType 

__interface INTERFACE_UUID("{60B4B941-3B4A-4F07-B3B8-FDE97104AD0E}") IXMLCOMMENTSType : public IXMLNodeCollection
{
public:
public:
  // Property Accessors 
  virtual UnicodeString __fastcall Get_COMMENT(int Index) = 0;
  // Methods & Properties 
  virtual _di_IXMLNode __fastcall Add(const UnicodeString COMMENT) = 0;
  virtual _di_IXMLNode __fastcall Insert(const int Index, const UnicodeString COMMENT) = 0;
  __property UnicodeString COMMENT[int Index] = { read=Get_COMMENT };/* default */
};

// IXMLPRODUCTType 

__interface INTERFACE_UUID("{FD60100E-D828-47D8-AC84-84E7726E0C76}") IXMLPRODUCTType : public IXMLNode
{
public:
  // Property Accessors 
  virtual UnicodeString __fastcall Get_GUID() = 0;
  virtual UnicodeString __fastcall Get_SERVINGCOURSE() = 0;
  virtual UnicodeString __fastcall Get_PRODUCT_NAME() = 0;
  virtual UnicodeString __fastcall Get_SIZE() = 0;
  virtual UnicodeString __fastcall Get_TYPE() = 0;
  virtual UnicodeString __fastcall Get_PRODUCT_PLU() = 0;
  virtual UnicodeString __fastcall Get_PRODUCT_BASE_PRICE() = 0;
  virtual UnicodeString __fastcall Get_PRODUCT_CHARGE_PRICE() = 0;
  virtual Integer __fastcall Get_PRICELEVEL() = 0;
  virtual Integer __fastcall Get_QTY() = 0;
  virtual UnicodeString __fastcall Get_PRODUCT_COMMENT() = 0;
  virtual _di_IXMLOPTIONTypeList __fastcall Get_OPTION() = 0;
  virtual _di_IXMLPRODUCTIDENTType __fastcall Get_PRODUCTIDENT() = 0;
  virtual _di_IXMLPRODUCTTypeList __fastcall Get_PRODUCT() = 0;
  virtual void __fastcall Set_GUID(UnicodeString Value) = 0;
  virtual void __fastcall Set_SERVINGCOURSE(UnicodeString Value) = 0;
  virtual void __fastcall Set_PRODUCT_NAME(UnicodeString Value) = 0;
  virtual void __fastcall Set_SIZE(UnicodeString Value) = 0;
  virtual void __fastcall Set_TYPE(UnicodeString Value) = 0;
  virtual void __fastcall Set_PRODUCT_PLU(UnicodeString Value) = 0;
  virtual void __fastcall Set_PRODUCT_BASE_PRICE(UnicodeString Value) = 0;
  virtual void __fastcall Set_PRODUCT_CHARGE_PRICE(UnicodeString Value) = 0;
  virtual void __fastcall Set_PRICELEVEL(Integer Value) = 0;
  virtual void __fastcall Set_QTY(Integer Value) = 0;
  virtual void __fastcall Set_PRODUCT_COMMENT(UnicodeString Value) = 0;
  // Methods & Properties 
  __property UnicodeString GUID = { read=Get_GUID, write=Set_GUID };
  __property UnicodeString SERVINGCOURSE = { read=Get_SERVINGCOURSE, write=Set_SERVINGCOURSE };
  __property UnicodeString PRODUCT_NAME = { read=Get_PRODUCT_NAME, write=Set_PRODUCT_NAME };
  __property UnicodeString SIZE = { read=Get_SIZE, write=Set_SIZE };
  __property UnicodeString TYPE = { read=Get_TYPE, write=Set_TYPE };
  __property UnicodeString PRODUCT_PLU = { read=Get_PRODUCT_PLU, write=Set_PRODUCT_PLU };
  __property UnicodeString PRODUCT_BASE_PRICE = { read=Get_PRODUCT_BASE_PRICE, write=Set_PRODUCT_BASE_PRICE };
  __property UnicodeString PRODUCT_CHARGE_PRICE = { read=Get_PRODUCT_CHARGE_PRICE, write=Set_PRODUCT_CHARGE_PRICE };
  __property Integer PRICELEVEL = { read=Get_PRICELEVEL, write=Set_PRICELEVEL };
  __property Integer QTY = { read=Get_QTY, write=Set_QTY };
  __property UnicodeString PRODUCT_COMMENT = { read=Get_PRODUCT_COMMENT, write=Set_PRODUCT_COMMENT };
  __property _di_IXMLOPTIONTypeList OPTION = { read=Get_OPTION };
  __property _di_IXMLPRODUCTIDENTType PRODUCTIDENT = { read=Get_PRODUCTIDENT };
  __property _di_IXMLPRODUCTTypeList PRODUCT = { read=Get_PRODUCT };
};

// IXMLPRODUCTTypeList 

__interface INTERFACE_UUID("{FC64A9A6-C7F1-443F-A7FE-8DA76CDA8A39}") IXMLPRODUCTTypeList : public IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLPRODUCTType __fastcall Add() = 0;
  virtual _di_IXMLPRODUCTType __fastcall Insert(const int Index) = 0;
  virtual _di_IXMLPRODUCTType __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLPRODUCTType Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLOPTIONType 

__interface INTERFACE_UUID("{0550AB63-A860-4D7A-930E-70A7C408D367}") IXMLOPTIONType : public IXMLNode
{
public:
  // Property Accessors 
  virtual Integer __fastcall Get_Key() = 0;
  virtual UnicodeString __fastcall Get_Name() = 0;
  virtual void __fastcall Set_Key(Integer Value) = 0;
  virtual void __fastcall Set_Name(UnicodeString Value) = 0;
  // Methods & Properties 
  __property Integer Key = { read=Get_Key, write=Set_Key };
  __property UnicodeString Name = { read=Get_Name, write=Set_Name };
};

// IXMLOPTIONTypeList 

__interface INTERFACE_UUID("{80A1EF00-3D76-432E-99C0-C09A950C733F}") IXMLOPTIONTypeList : public IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLOPTIONType __fastcall Add() = 0;
  virtual _di_IXMLOPTIONType __fastcall Insert(const int Index) = 0;
  virtual _di_IXMLOPTIONType __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLOPTIONType Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLPRODUCTIDENTType 

__interface INTERFACE_UUID("{7335E7EF-3388-431E-94E6-A58640037908}") IXMLPRODUCTIDENTType : public IXMLNode
{
public:
  // Property Accessors 
  virtual Integer __fastcall Get_MODULEID() = 0;
  virtual Integer __fastcall Get_MENUKEY() = 0;
  virtual Integer __fastcall Get_COURSEKEY() = 0;
  virtual Integer __fastcall Get_ITEMKEY() = 0;
  virtual Integer __fastcall Get_SIZEKEY() = 0;
  virtual Integer __fastcall Get_SERVINGCOURSEKEY() = 0;
  virtual void __fastcall Set_MODULEID(Integer Value) = 0;
  virtual void __fastcall Set_MENUKEY(Integer Value) = 0;
  virtual void __fastcall Set_COURSEKEY(Integer Value) = 0;
  virtual void __fastcall Set_ITEMKEY(Integer Value) = 0;
  virtual void __fastcall Set_SIZEKEY(Integer Value) = 0;
  virtual void __fastcall Set_SERVINGCOURSEKEY(Integer Value) = 0;
  // Methods & Properties 
  __property Integer MODULEID = { read=Get_MODULEID, write=Set_MODULEID };
  __property Integer MENUKEY = { read=Get_MENUKEY, write=Set_MENUKEY };
  __property Integer COURSEKEY = { read=Get_COURSEKEY, write=Set_COURSEKEY };
  __property Integer ITEMKEY = { read=Get_ITEMKEY, write=Set_ITEMKEY };
  __property Integer SIZEKEY = { read=Get_SIZEKEY, write=Set_SIZEKEY };
  __property Integer SERVINGCOURSEKEY = { read=Get_SERVINGCOURSEKEY, write=Set_SERVINGCOURSEKEY };
};

// Forward Decls 

class TXMLORDERType;
class TXMLFROMType;
class TXMLACCOUNTType;
class TXMLPAYMENTSType;
class TXMLPAYMENT_INFOType;
class TXMLPAYMENTType;
class TXMLPAYMENTTypeList;
class TXMLDELEVERYType;
class TXMLCOMMENTSType;
class TXMLPRODUCTType;
class TXMLPRODUCTTypeList;
class TXMLOPTIONType;
class TXMLOPTIONTypeList;
class TXMLPRODUCTIDENTType;

// TXMLORDERType 

class TXMLORDERType : public TXMLNode, public IXMLORDERType
{
  __IXMLNODE_IMPL2__
protected:
  // IXMLORDERType 
  virtual UnicodeString __fastcall Get_GUID();
  virtual UnicodeString __fastcall Get_ORDER_NAME();
  virtual UnicodeString __fastcall Get_STORE_NAME();
  virtual UnicodeString __fastcall Get_RESPOND();
  virtual UnicodeString __fastcall Get_ORDER_DATE();
  virtual UnicodeString __fastcall Get_EXPECTED_DATE();
  virtual Integer __fastcall Get_PAYMENT_METHOD();
  virtual Integer __fastcall Get_SCHEDULED();
  virtual UnicodeString __fastcall Get_ORDER_TOTAL();
  virtual _di_IXMLFROMType __fastcall Get_FROM();
  virtual _di_IXMLACCOUNTType __fastcall Get_ACCOUNT();
  virtual void __fastcall Set_GUID(UnicodeString Value);
  virtual void __fastcall Set_ORDER_NAME(UnicodeString Value);
  virtual void __fastcall Set_STORE_NAME(UnicodeString Value);
  virtual void __fastcall Set_RESPOND(UnicodeString Value);
  virtual void __fastcall Set_ORDER_DATE(UnicodeString Value);
  virtual void __fastcall Set_EXPECTED_DATE(UnicodeString Value);
  virtual void __fastcall Set_PAYMENT_METHOD(Integer Value);
  virtual void __fastcall Set_SCHEDULED(Integer Value);
  virtual void __fastcall Set_ORDER_TOTAL(UnicodeString Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLFROMType 

class TXMLFROMType : public TXMLNode, public IXMLFROMType
{
  __IXMLNODE_IMPL__
protected:
  // IXMLFROMType 
  virtual UnicodeString __fastcall Get_ID();
  virtual UnicodeString __fastcall Get_NAME();
  virtual UnicodeString __fastcall Get_AUTH_NAME();
  virtual UnicodeString __fastcall Get_AUTH_PASS();
  virtual UnicodeString __fastcall Get_PHONE_NUMBER();
  virtual UnicodeString __fastcall Get_STAFF_NAME();
  virtual UnicodeString __fastcall Get_STAFF_ID();
  virtual UnicodeString __fastcall Get_STAFF_PASS();
  virtual void __fastcall Set_ID(UnicodeString Value);
  virtual void __fastcall Set_NAME(UnicodeString Value);
  virtual void __fastcall Set_AUTH_NAME(UnicodeString Value);
  virtual void __fastcall Set_AUTH_PASS(UnicodeString Value);
  virtual void __fastcall Set_PHONE_NUMBER(UnicodeString Value);
  virtual void __fastcall Set_STAFF_NAME(UnicodeString Value);
  virtual void __fastcall Set_STAFF_ID(UnicodeString Value);
  virtual void __fastcall Set_STAFF_PASS(UnicodeString Value);
};

// TXMLACCOUNTType 

class TXMLACCOUNTType : public TXMLNode, public IXMLACCOUNTType
{
  __IXMLNODE_IMPL__
private:
  _di_IXMLPRODUCTTypeList FPRODUCT;
protected:
  // IXMLACCOUNTType 
  virtual UnicodeString __fastcall Get_MEMBER_NUMBER();
  virtual UnicodeString __fastcall Get_NAME();
  virtual Integer __fastcall Get_COVERS();
  virtual _di_IXMLPAYMENTSType __fastcall Get_PAYMENTS();
  virtual _di_IXMLDELEVERYType __fastcall Get_DELEVERY();
  virtual _di_IXMLCOMMENTSType __fastcall Get_COMMENTS();
  virtual _di_IXMLPRODUCTTypeList __fastcall Get_PRODUCT();
  virtual void __fastcall Set_MEMBER_NUMBER(UnicodeString Value);
  virtual void __fastcall Set_NAME(UnicodeString Value);
  virtual void __fastcall Set_COVERS(Integer Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLPAYMENTSType 

class TXMLPAYMENTSType : public TXMLNode, public IXMLPAYMENTSType
{
  __IXMLNODE_IMPL__
private:
  _di_IXMLPAYMENTTypeList FPAYMENT;
protected:
  // IXMLPAYMENTSType 
  virtual _di_IXMLPAYMENT_INFOType __fastcall Get_PAYMENT_INFO();
  virtual _di_IXMLPAYMENTTypeList __fastcall Get_PAYMENT();
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLPAYMENT_INFOType 

class TXMLPAYMENT_INFOType : public TXMLNodeCollection, public IXMLPAYMENT_INFOType
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLPAYMENT_INFOType 
  virtual UnicodeString __fastcall Get_INFO(int Index);
  virtual _di_IXMLNode __fastcall Add(const UnicodeString INFO);
  virtual _di_IXMLNode __fastcall Insert(const int Index, const UnicodeString INFO);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLPAYMENTType 

class TXMLPAYMENTType : public TXMLNode, public IXMLPAYMENTType
{
  __IXMLNODE_IMPL__
protected:
  // IXMLPAYMENTType 
  virtual UnicodeString __fastcall Get_NAME();
  virtual UnicodeString __fastcall Get_VALUE();
  virtual void __fastcall Set_NAME(UnicodeString Value);
  virtual void __fastcall Set_VALUE(UnicodeString Value);
};

// TXMLPAYMENTTypeList 

class TXMLPAYMENTTypeList : public TXMLNodeCollection, public IXMLPAYMENTTypeList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLPAYMENTTypeList 
  virtual _di_IXMLPAYMENTType __fastcall Add();
  virtual _di_IXMLPAYMENTType __fastcall Insert(const int Index);
  virtual _di_IXMLPAYMENTType __fastcall Get_Item(int Index);
};

// TXMLDELEVERYType 

class TXMLDELEVERYType : public TXMLNode, public IXMLDELEVERYType
{
  __IXMLNODE_IMPL__
protected:
  // IXMLDELEVERYType 
  virtual UnicodeString __fastcall Get_TO();
  virtual UnicodeString __fastcall Get_PHONE();
  virtual UnicodeString __fastcall Get_STREET_NO();
  virtual UnicodeString __fastcall Get_STREET_NAME();
  virtual UnicodeString __fastcall Get_SUBURB();
  virtual UnicodeString __fastcall Get_POST_CODE();
  virtual UnicodeString __fastcall Get_CITY();
  virtual UnicodeString __fastcall Get_COUNTRY();
  virtual UnicodeString __fastcall Get_NOTE();
  virtual void __fastcall Set_TO(UnicodeString Value);
  virtual void __fastcall Set_PHONE(UnicodeString Value);
  virtual void __fastcall Set_STREET_NO(UnicodeString Value);
  virtual void __fastcall Set_STREET_NAME(UnicodeString Value);
  virtual void __fastcall Set_SUBURB(UnicodeString Value);
  virtual void __fastcall Set_POST_CODE(UnicodeString Value);
  virtual void __fastcall Set_CITY(UnicodeString Value);
  virtual void __fastcall Set_COUNTRY(UnicodeString Value);
  virtual void __fastcall Set_NOTE(UnicodeString Value);
};

// TXMLCOMMENTSType 

class TXMLCOMMENTSType : public TXMLNodeCollection, public IXMLCOMMENTSType
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLCOMMENTSType 
  virtual UnicodeString __fastcall Get_COMMENT(int Index);
  virtual _di_IXMLNode __fastcall Add(const UnicodeString COMMENT);
  virtual _di_IXMLNode __fastcall Insert(const int Index, const UnicodeString COMMENT);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLPRODUCTType 

class TXMLPRODUCTType : public TXMLNode, public IXMLPRODUCTType
{
  __IXMLNODE_IMPL2__
private:
  _di_IXMLOPTIONTypeList FOPTION;
  _di_IXMLPRODUCTTypeList FPRODUCT;
protected:
  // IXMLPRODUCTType 
  virtual UnicodeString __fastcall Get_GUID();
  virtual UnicodeString __fastcall Get_SERVINGCOURSE();
  virtual UnicodeString __fastcall Get_PRODUCT_NAME();
  virtual UnicodeString __fastcall Get_SIZE();
  virtual UnicodeString __fastcall Get_TYPE();
  virtual UnicodeString __fastcall Get_PRODUCT_PLU();
  virtual UnicodeString __fastcall Get_PRODUCT_BASE_PRICE();
  virtual UnicodeString __fastcall Get_PRODUCT_CHARGE_PRICE();
  virtual Integer __fastcall Get_PRICELEVEL();
  virtual Integer __fastcall Get_QTY();
  virtual UnicodeString __fastcall Get_PRODUCT_COMMENT();
  virtual _di_IXMLOPTIONTypeList __fastcall Get_OPTION();
  virtual _di_IXMLPRODUCTIDENTType __fastcall Get_PRODUCTIDENT();
  virtual _di_IXMLPRODUCTTypeList __fastcall Get_PRODUCT();
  virtual void __fastcall Set_GUID(UnicodeString Value);
  virtual void __fastcall Set_SERVINGCOURSE(UnicodeString Value);
  virtual void __fastcall Set_PRODUCT_NAME(UnicodeString Value);
  virtual void __fastcall Set_SIZE(UnicodeString Value);
  virtual void __fastcall Set_TYPE(UnicodeString Value);
  virtual void __fastcall Set_PRODUCT_PLU(UnicodeString Value);
  virtual void __fastcall Set_PRODUCT_BASE_PRICE(UnicodeString Value);
  virtual void __fastcall Set_PRODUCT_CHARGE_PRICE(UnicodeString Value);
  virtual void __fastcall Set_PRICELEVEL(Integer Value);
  virtual void __fastcall Set_QTY(Integer Value);
  virtual void __fastcall Set_PRODUCT_COMMENT(UnicodeString Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLPRODUCTTypeList 

class TXMLPRODUCTTypeList : public TXMLNodeCollection, public IXMLPRODUCTTypeList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLPRODUCTTypeList 
  virtual _di_IXMLPRODUCTType __fastcall Add();
  virtual _di_IXMLPRODUCTType __fastcall Insert(const int Index);
  virtual _di_IXMLPRODUCTType __fastcall Get_Item(int Index);
};

// TXMLOPTIONType 

class TXMLOPTIONType : public TXMLNode, public IXMLOPTIONType
{
  __IXMLNODE_IMPL__
protected:
  // IXMLOPTIONType 
  virtual Integer __fastcall Get_Key();
  virtual UnicodeString __fastcall Get_Name();
  virtual void __fastcall Set_Key(Integer Value);
  virtual void __fastcall Set_Name(UnicodeString Value);
};

// TXMLOPTIONTypeList 

class TXMLOPTIONTypeList : public TXMLNodeCollection, public IXMLOPTIONTypeList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLOPTIONTypeList 
  virtual _di_IXMLOPTIONType __fastcall Add();
  virtual _di_IXMLOPTIONType __fastcall Insert(const int Index);
  virtual _di_IXMLOPTIONType __fastcall Get_Item(int Index);
};

// TXMLPRODUCTIDENTType 

class TXMLPRODUCTIDENTType : public TXMLNode, public IXMLPRODUCTIDENTType
{
  __IXMLNODE_IMPL__
protected:
  // IXMLPRODUCTIDENTType 
  virtual Integer __fastcall Get_MODULEID();
  virtual Integer __fastcall Get_MENUKEY();
  virtual Integer __fastcall Get_COURSEKEY();
  virtual Integer __fastcall Get_ITEMKEY();
  virtual Integer __fastcall Get_SIZEKEY();
  virtual Integer __fastcall Get_SERVINGCOURSEKEY();
  virtual void __fastcall Set_MODULEID(Integer Value);
  virtual void __fastcall Set_MENUKEY(Integer Value);
  virtual void __fastcall Set_COURSEKEY(Integer Value);
  virtual void __fastcall Set_ITEMKEY(Integer Value);
  virtual void __fastcall Set_SIZEKEY(Integer Value);
  virtual void __fastcall Set_SERVINGCOURSEKEY(Integer Value);
};

// Global Functions 

_di_IXMLORDERType __fastcall GetORDER(_di_IXMLDocument Doc);
_di_IXMLORDERType __fastcall GetORDER(TXMLDocument *Doc);
_di_IXMLORDERType __fastcall LoadORDER(const UnicodeString& FileName);
_di_IXMLORDERType __fastcall  NewORDER();

#define TargetNamespace ""

#endif