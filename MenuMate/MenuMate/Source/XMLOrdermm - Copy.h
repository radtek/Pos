
// ********************************************************************************************************* //
//                                                                                                         
//                                            XML Data Binding                                             
//                                                                                                         
//         Generated on: 14/01/2011 9:14:21 a.m.                                                           
//       Generated from: C:\Users\michael\dev\5.1.Web\Static Files\XML Examples\XML Order Example-mm.xml   
//   Settings stored in: C:\Users\michael\dev\5.1.Web\Static Files\XML Examples\XML Order Example-mm.xdb   
//                                                                                                         
// ********************************************************************************************************* //

#ifndef   XMLOrdermmH
#define   XMLOrdermmH

#include <System.hpp>
#include <xmldom.hpp>
#include <XMLDoc.hpp>
#include <XMLIntf.hpp>
#include <XMLNodeImp.h>


// Forward Decls 

__interface IXMLORDERSType;
typedef System::DelphiInterface<IXMLORDERSType> _di_IXMLORDERSType;
__interface IXMLORDERType;
typedef System::DelphiInterface<IXMLORDERType> _di_IXMLORDERType;
__interface IXMLCUSTOMERType;
typedef System::DelphiInterface<IXMLCUSTOMERType> _di_IXMLCUSTOMERType;
__interface IXMLACCOUNTType;
typedef System::DelphiInterface<IXMLACCOUNTType> _di_IXMLACCOUNTType;
__interface IXMLACCOUNTIDENTType;
typedef System::DelphiInterface<IXMLACCOUNTIDENTType> _di_IXMLACCOUNTIDENTType;
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

// IXMLORDERSType 

__interface INTERFACE_UUID("{F02E1853-DDA3-41C4-8276-02D43D97EBC3}") IXMLORDERSType : public IXMLNodeCollection
{
public:
public:
  // Property Accessors 
  virtual UnicodeString __fastcall Get_ORDER_ID() = 0;
  virtual UnicodeString __fastcall Get_ORDER_NAME() = 0;
  virtual bool __fastcall Get_RESPOND() = 0;
  virtual UnicodeString __fastcall Get_STORE_NAME() = 0;
  virtual UnicodeString __fastcall Get_ORDER_DATE() = 0;
  virtual UnicodeString __fastcall Get_EXPECTED_DATE() = 0;
  virtual _di_IXMLORDERType __fastcall Get_ORDER(int Index) = 0;
  virtual void __fastcall Set_ORDER_ID(UnicodeString Value) = 0;
  virtual void __fastcall Set_ORDER_NAME(UnicodeString Value) = 0;
  virtual void __fastcall Set_RESPOND(bool Value) = 0;
  virtual void __fastcall Set_STORE_NAME(UnicodeString Value) = 0;
  virtual void __fastcall Set_ORDER_DATE(UnicodeString Value) = 0;
  virtual void __fastcall Set_EXPECTED_DATE(UnicodeString Value) = 0;
  // Methods & Properties 
  virtual _di_IXMLORDERType __fastcall Add() = 0;
  virtual _di_IXMLORDERType __fastcall Insert(const int Index) = 0;
  __property UnicodeString ORDER_ID = { read=Get_ORDER_ID, write=Set_ORDER_ID };
  __property UnicodeString ORDER_NAME = { read=Get_ORDER_NAME, write=Set_ORDER_NAME };
  __property bool RESPOND = { read=Get_RESPOND, write=Set_RESPOND };
  __property UnicodeString STORE_NAME = { read=Get_STORE_NAME, write=Set_STORE_NAME };
  __property UnicodeString ORDER_DATE = { read=Get_ORDER_DATE, write=Set_ORDER_DATE };
  __property UnicodeString EXPECTED_DATE = { read=Get_EXPECTED_DATE, write=Set_EXPECTED_DATE };
  __property _di_IXMLORDERType ORDER[int Index] = { read=Get_ORDER };/* default */
};

// IXMLORDERType 

__interface INTERFACE_UUID("{498F1679-5672-4C77-B396-E4F477CE0F38}") IXMLORDERType : public IXMLNode
{
public:
  // Property Accessors 
  virtual Integer __fastcall Get_PAYMENT_METHOD() = 0;
  virtual Integer __fastcall Get_SCHEDULED() = 0;
  virtual UnicodeString __fastcall Get_ORDER_TOTAL() = 0;
  virtual _di_IXMLCUSTOMERType __fastcall Get_CUSTOMER() = 0;
  virtual _di_IXMLACCOUNTType __fastcall Get_ACCOUNT() = 0;
  virtual _di_IXMLPRODUCTTypeList __fastcall Get_PRODUCT() = 0;
  virtual void __fastcall Set_PAYMENT_METHOD(Integer Value) = 0;
  virtual void __fastcall Set_SCHEDULED(Integer Value) = 0;
  virtual void __fastcall Set_ORDER_TOTAL(UnicodeString Value) = 0;
  // Methods & Properties 
  __property Integer PAYMENT_METHOD = { read=Get_PAYMENT_METHOD, write=Set_PAYMENT_METHOD };
  __property Integer SCHEDULED = { read=Get_SCHEDULED, write=Set_SCHEDULED };
  __property UnicodeString ORDER_TOTAL = { read=Get_ORDER_TOTAL, write=Set_ORDER_TOTAL };
  __property _di_IXMLCUSTOMERType CUSTOMER = { read=Get_CUSTOMER };
  __property _di_IXMLACCOUNTType ACCOUNT = { read=Get_ACCOUNT };
  __property _di_IXMLPRODUCTTypeList PRODUCT = { read=Get_PRODUCT };
};

// IXMLCUSTOMERType 

__interface INTERFACE_UUID("{B7C235A4-041C-4196-B4E5-57ADF2F5E69B}") IXMLCUSTOMERType : public IXMLNode
{
public:
  // Property Accessors 
  virtual UnicodeString __fastcall Get_ID() = 0;
  virtual UnicodeString __fastcall Get_FIRST_NAME() = 0;
  virtual UnicodeString __fastcall Get_LAST_NAME() = 0;
  virtual UnicodeString __fastcall Get_PHONE_NUMBER() = 0;
  virtual void __fastcall Set_ID(UnicodeString Value) = 0;
  virtual void __fastcall Set_FIRST_NAME(UnicodeString Value) = 0;
  virtual void __fastcall Set_LAST_NAME(UnicodeString Value) = 0;
  virtual void __fastcall Set_PHONE_NUMBER(UnicodeString Value) = 0;
  // Methods & Properties 
  __property UnicodeString ID = { read=Get_ID, write=Set_ID };
  __property UnicodeString FIRST_NAME = { read=Get_FIRST_NAME, write=Set_FIRST_NAME };
  __property UnicodeString LAST_NAME = { read=Get_LAST_NAME, write=Set_LAST_NAME };
  __property UnicodeString PHONE_NUMBER = { read=Get_PHONE_NUMBER, write=Set_PHONE_NUMBER };
};

// IXMLACCOUNTType 

__interface INTERFACE_UUID("{E08AC640-95A8-432A-A93C-2313855ADDBE}") IXMLACCOUNTType : public IXMLNode
{
public:
  // Property Accessors 
  virtual Integer __fastcall Get_KEY() = 0;
  virtual UnicodeString __fastcall Get_TYPE() = 0;
  virtual _di_IXMLACCOUNTIDENTType __fastcall Get_ACCOUNTIDENT() = 0;
  virtual void __fastcall Set_KEY(Integer Value) = 0;
  virtual void __fastcall Set_TYPE(UnicodeString Value) = 0;
  // Methods & Properties 
  __property Integer KEY = { read=Get_KEY, write=Set_KEY };
  __property UnicodeString TYPE = { read=Get_TYPE, write=Set_TYPE };
  __property _di_IXMLACCOUNTIDENTType ACCOUNTIDENT = { read=Get_ACCOUNTIDENT };
};

// IXMLACCOUNTIDENTType 

__interface INTERFACE_UUID("{0943DE9B-3B6C-42D8-96BB-B33355733020}") IXMLACCOUNTIDENTType : public IXMLNode
{
public:
  // Property Accessors 
  virtual Integer __fastcall Get_TYPE() = 0;
  virtual Integer __fastcall Get_TABLE() = 0;
  virtual Integer __fastcall Get_SEAT() = 0;
  virtual Integer __fastcall Get_TABKEY() = 0;
  virtual Integer __fastcall Get_ROOMNUMBER() = 0;
  virtual UnicodeString __fastcall Get_WEBID() = 0;

  virtual void __fastcall Set_TYPE(Integer Value) = 0;
  virtual void __fastcall Set_TABLE(Integer Value) = 0;
  virtual void __fastcall Set_SEAT(Integer Value) = 0;
  virtual void __fastcall Set_TABKEY(Integer Value) = 0;
  virtual void __fastcall Set_ROOMNUMBER(Integer Value) = 0;
  virtual void __fastcall Set_WEBID(UnicodeString Value) = 0;
  // Methods & Properties 
  __property Integer TYPE = { read=Get_TYPE, write=Set_TYPE };
  __property Integer TABLE = { read=Get_TABLE, write=Set_TABLE };
  __property Integer SEAT = { read=Get_SEAT, write=Set_SEAT };
  __property Integer TABKEY = { read=Get_TABKEY, write=Set_TABKEY };
  __property Integer ROOMNUMBER = { read=Get_ROOMNUMBER, write=Set_ROOMNUMBER };
  __property UnicodeString WEBID = { read=Get_WEBID, write=Set_WEBID };
};

// IXMLPRODUCTType 

__interface INTERFACE_UUID("{ECB6F02E-6075-4955-9BA0-4BA56973C03A}") IXMLPRODUCTType : public IXMLNode
{
public:
  // Property Accessors 
  virtual UnicodeString __fastcall Get_GUID() = 0;
  virtual UnicodeString __fastcall Get_SERVINGCOURSE() = 0;
  virtual UnicodeString __fastcall Get_PRODUCT_NAME() = 0;
  virtual UnicodeString __fastcall Get_SIZE() = 0;
  virtual UnicodeString __fastcall Get_PRODUCT_PLU() = 0;
  virtual UnicodeString __fastcall Get_PRODUCT_BASE_PRICE() = 0;
  virtual UnicodeString __fastcall Get_PRODUCT_CHARGE_PRICE() = 0;
  virtual Integer __fastcall Get_PRICELEVEL() = 0;
  virtual UnicodeString __fastcall Get_FOR() = 0;
  virtual UnicodeString __fastcall Get_PRODUCT_COMMENT() = 0;
  virtual Integer __fastcall Get_QTY() = 0;
  virtual _di_IXMLOPTIONTypeList __fastcall Get_OPTION() = 0;
  virtual _di_IXMLPRODUCTIDENTType __fastcall Get_PRODUCTIDENT() = 0;
  virtual _di_IXMLPRODUCTType __fastcall Get_PRODUCT() = 0;
  virtual void __fastcall Set_GUID(UnicodeString Value) = 0;
  virtual void __fastcall Set_SERVINGCOURSE(UnicodeString Value) = 0;
  virtual void __fastcall Set_PRODUCT_NAME(UnicodeString Value) = 0;
  virtual void __fastcall Set_SIZE(UnicodeString Value) = 0;
  virtual void __fastcall Set_PRODUCT_PLU(UnicodeString Value) = 0;
  virtual void __fastcall Set_PRODUCT_BASE_PRICE(UnicodeString Value) = 0;
  virtual void __fastcall Set_PRODUCT_CHARGE_PRICE(UnicodeString Value) = 0;
  virtual void __fastcall Set_PRICELEVEL(Integer Value) = 0;
  virtual void __fastcall Set_FOR(UnicodeString Value) = 0;
  virtual void __fastcall Set_PRODUCT_COMMENT(UnicodeString Value) = 0;
  virtual void __fastcall Set_QTY(Integer Value) = 0;
  // Methods & Properties 
  __property UnicodeString ORDER_GUID = { read=Get_GUID, write=Set_GUID };
  __property UnicodeString SERVINGCOURSE = { read=Get_SERVINGCOURSE, write=Set_SERVINGCOURSE };
  __property UnicodeString PRODUCT_NAME = { read=Get_PRODUCT_NAME, write=Set_PRODUCT_NAME };
  __property UnicodeString SIZE = { read=Get_SIZE, write=Set_SIZE };
  __property UnicodeString PRODUCT_PLU = { read=Get_PRODUCT_PLU, write=Set_PRODUCT_PLU };
  __property UnicodeString PRODUCT_BASE_PRICE = { read=Get_PRODUCT_BASE_PRICE, write=Set_PRODUCT_BASE_PRICE };
  __property UnicodeString PRODUCT_CHARGE_PRICE = { read=Get_PRODUCT_CHARGE_PRICE, write=Set_PRODUCT_CHARGE_PRICE };
  __property Integer PRICELEVEL = { read=Get_PRICELEVEL, write=Set_PRICELEVEL };
  __property UnicodeString FOR = { read=Get_FOR, write=Set_FOR };
  __property UnicodeString PRODUCT_COMMENT = { read=Get_PRODUCT_COMMENT, write=Set_PRODUCT_COMMENT };
  __property Integer QTY = { read=Get_QTY, write=Set_QTY };
  __property _di_IXMLOPTIONTypeList OPTION = { read=Get_OPTION };
  __property _di_IXMLPRODUCTIDENTType PRODUCTIDENT = { read=Get_PRODUCTIDENT };
  __property _di_IXMLPRODUCTType PRODUCT = { read=Get_PRODUCT };
};

// IXMLPRODUCTTypeList 

__interface INTERFACE_UUID("{5206C68A-BFCE-46CB-B358-E99D93BC4C83}") IXMLPRODUCTTypeList : public IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLPRODUCTType __fastcall Add() = 0;
  virtual _di_IXMLPRODUCTType __fastcall Insert(const int Index) = 0;
  virtual _di_IXMLPRODUCTType __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLPRODUCTType Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLOPTIONType 

__interface INTERFACE_UUID("{144EB630-A50F-41E5-98A2-1CE90FDF212B}") IXMLOPTIONType : public IXMLNode
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

__interface INTERFACE_UUID("{B95AD5C7-9E82-44BB-AF6C-BBF1E1811D85}") IXMLOPTIONTypeList : public IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLOPTIONType __fastcall Add() = 0;
  virtual _di_IXMLOPTIONType __fastcall Insert(const int Index) = 0;
  virtual _di_IXMLOPTIONType __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLOPTIONType Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLPRODUCTIDENTType 

__interface INTERFACE_UUID("{537E6DE1-3DC8-4B5D-8876-576C9A96CD43}") IXMLPRODUCTIDENTType : public IXMLNode
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

class TXMLORDERSType;
class TXMLORDERType;
class TXMLCUSTOMERType;
class TXMLACCOUNTType;
class TXMLACCOUNTIDENTType;
class TXMLPRODUCTType;
class TXMLPRODUCTTypeList;
class TXMLOPTIONType;
class TXMLOPTIONTypeList;
class TXMLPRODUCTIDENTType;

// TXMLORDERSType 

class TXMLORDERSType : public TXMLNodeCollection, public IXMLORDERSType
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLORDERSType 
  virtual UnicodeString __fastcall Get_ORDER_ID();
  virtual UnicodeString __fastcall Get_ORDER_NAME();
  virtual bool __fastcall Get_RESPOND();
  virtual UnicodeString __fastcall Get_STORE_NAME();
  virtual UnicodeString __fastcall Get_ORDER_DATE();
  virtual UnicodeString __fastcall Get_EXPECTED_DATE();
  virtual _di_IXMLORDERType __fastcall Get_ORDER(int Index);
  virtual void __fastcall Set_RESPOND(bool Value);
  virtual void __fastcall Set_ORDER_ID(UnicodeString Value);
  virtual void __fastcall Set_ORDER_NAME(UnicodeString Value);
  virtual void __fastcall Set_STORE_NAME(UnicodeString Value);
  virtual void __fastcall Set_ORDER_DATE(UnicodeString Value);
  virtual void __fastcall Set_EXPECTED_DATE(UnicodeString Value);
  virtual _di_IXMLORDERType __fastcall Add();
  virtual _di_IXMLORDERType __fastcall Insert(const int Index);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLORDERType 

class TXMLORDERType : public TXMLNode, public IXMLORDERType
{
  __IXMLNODE_IMPL__
private:
  _di_IXMLPRODUCTTypeList FPRODUCT;
protected:
  // IXMLORDERType 
  virtual Integer __fastcall Get_PAYMENT_METHOD();
  virtual Integer __fastcall Get_SCHEDULED();
  virtual UnicodeString __fastcall Get_ORDER_TOTAL();
  virtual _di_IXMLCUSTOMERType __fastcall Get_CUSTOMER();
  virtual _di_IXMLACCOUNTType __fastcall Get_ACCOUNT();
  virtual _di_IXMLPRODUCTTypeList __fastcall Get_PRODUCT();
  virtual void __fastcall Set_PAYMENT_METHOD(Integer Value);
  virtual void __fastcall Set_SCHEDULED(Integer Value);
  virtual void __fastcall Set_ORDER_TOTAL(UnicodeString Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLCUSTOMERType 

class TXMLCUSTOMERType : public TXMLNode, public IXMLCUSTOMERType
{
  __IXMLNODE_IMPL__
protected:
  // IXMLCUSTOMERType 
  virtual UnicodeString __fastcall Get_ID();
  virtual UnicodeString __fastcall Get_FIRST_NAME();
  virtual UnicodeString __fastcall Get_LAST_NAME();
  virtual UnicodeString __fastcall Get_PHONE_NUMBER();
  virtual void __fastcall Set_ID(UnicodeString Value);
  virtual void __fastcall Set_FIRST_NAME(UnicodeString Value);
  virtual void __fastcall Set_LAST_NAME(UnicodeString Value);
  virtual void __fastcall Set_PHONE_NUMBER(UnicodeString Value);
};

// TXMLACCOUNTType 

class TXMLACCOUNTType : public TXMLNode, public IXMLACCOUNTType
{
  __IXMLNODE_IMPL__
protected:
  // IXMLACCOUNTType 
  virtual Integer __fastcall Get_KEY();
  virtual UnicodeString __fastcall Get_TYPE();
  virtual _di_IXMLACCOUNTIDENTType __fastcall Get_ACCOUNTIDENT();
  virtual void __fastcall Set_KEY(Integer Value);
  virtual void __fastcall Set_TYPE(UnicodeString Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLACCOUNTIDENTType 

class TXMLACCOUNTIDENTType : public TXMLNode, public IXMLACCOUNTIDENTType
{
  __IXMLNODE_IMPL__
protected:
  // IXMLACCOUNTIDENTType 
  virtual Integer __fastcall Get_TYPE();
  virtual Integer __fastcall Get_TABLE();
  virtual Integer __fastcall Get_SEAT();
  virtual Integer __fastcall Get_TABKEY();
  virtual Integer __fastcall Get_ROOMNUMBER();
  virtual UnicodeString __fastcall Get_WEBID();

  virtual void __fastcall Set_TYPE(Integer Value);
  virtual void __fastcall Set_TABLE(Integer Value);
  virtual void __fastcall Set_SEAT(Integer Value);
  virtual void __fastcall Set_TABKEY(Integer Value);
  virtual void __fastcall Set_ROOMNUMBER(Integer Value);
  virtual void __fastcall Set_WEBID(UnicodeString Value);
};

// TXMLPRODUCTType 

class TXMLPRODUCTType : public TXMLNode, public IXMLPRODUCTType
{
  __IXMLNODE_IMPL__
private:
  _di_IXMLOPTIONTypeList FOPTION;
protected:
  // IXMLPRODUCTType 
  virtual UnicodeString __fastcall Get_GUID();
  virtual UnicodeString __fastcall Get_SERVINGCOURSE();
  virtual UnicodeString __fastcall Get_PRODUCT_NAME();
  virtual UnicodeString __fastcall Get_SIZE();
  virtual UnicodeString __fastcall Get_PRODUCT_PLU();
  virtual UnicodeString __fastcall Get_PRODUCT_BASE_PRICE();
  virtual UnicodeString __fastcall Get_PRODUCT_CHARGE_PRICE();
  virtual Integer __fastcall Get_PRICELEVEL();
  virtual UnicodeString __fastcall Get_FOR();
  virtual UnicodeString __fastcall Get_PRODUCT_COMMENT();
  virtual Integer __fastcall Get_QTY();
  virtual _di_IXMLOPTIONTypeList __fastcall Get_OPTION();
  virtual _di_IXMLPRODUCTIDENTType __fastcall Get_PRODUCTIDENT();
  virtual _di_IXMLPRODUCTType __fastcall Get_PRODUCT();
  virtual void __fastcall Set_GUID(UnicodeString Value);
  virtual void __fastcall Set_SERVINGCOURSE(UnicodeString Value);
  virtual void __fastcall Set_PRODUCT_NAME(UnicodeString Value);
  virtual void __fastcall Set_SIZE(UnicodeString Value);
  virtual void __fastcall Set_PRODUCT_PLU(UnicodeString Value);
  virtual void __fastcall Set_PRODUCT_BASE_PRICE(UnicodeString Value);
  virtual void __fastcall Set_PRODUCT_CHARGE_PRICE(UnicodeString Value);
  virtual void __fastcall Set_PRICELEVEL(Integer Value);
  virtual void __fastcall Set_FOR(UnicodeString Value);
  virtual void __fastcall Set_PRODUCT_COMMENT(UnicodeString Value);
  virtual void __fastcall Set_QTY(Integer Value);
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

_di_IXMLORDERSType __fastcall GetORDERS(_di_IXMLDocument Doc);
_di_IXMLORDERSType __fastcall GetORDERS(TXMLDocument *Doc);
_di_IXMLORDERSType __fastcall LoadORDERS(const UnicodeString& FileName);
_di_IXMLORDERSType __fastcall  NewORDERS();

#define TargetNamespace ""

#endif