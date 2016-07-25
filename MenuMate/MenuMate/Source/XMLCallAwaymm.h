
// *************************************************************************************************************** //
//                                                                                                               
//                                               XML Data Binding                                                
//                                                                                                               
//         Generated on: 24/06/2011 9:16:40 p.m.                                                                 
//       Generated from: C:\Users\michael\dev\5.2.trunk\Static Files\XML Examples\Master CallAway Template.xml   
//                                                                                                               
// *************************************************************************************************************** //

#ifndef   XMLCallAwaymmH
#define   XMLCallAwaymmH

#include <System.hpp>
#include <xmldom.hpp>
#include <XMLDoc.hpp>
#include <XMLIntf.hpp>
#include <XMLNodeImp.h>

namespace XMLCallAways
{

// Forward Decls 

__interface IXMLCALLAWAYType;
typedef System::DelphiInterface<IXMLCALLAWAYType> _di_IXMLCALLAWAYType;
__interface IXMLACCOUNTType;
typedef System::DelphiInterface<IXMLACCOUNTType> _di_IXMLACCOUNTType;
__interface IXMLACCOUNTIDENTType;
typedef System::DelphiInterface<IXMLACCOUNTIDENTType> _di_IXMLACCOUNTIDENTType;
__interface IXMLSERVINGCOURSEType;
typedef System::DelphiInterface<IXMLSERVINGCOURSEType> _di_IXMLSERVINGCOURSEType;
__interface IXMLSERVINGCOURSETypeList;
typedef System::DelphiInterface<IXMLSERVINGCOURSETypeList> _di_IXMLSERVINGCOURSETypeList;

// IXMLCALLAWAYType 

__interface INTERFACE_UUID("{8F56C744-5E81-4E06-986B-61E4B5CF9659}") IXMLCALLAWAYType : public IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLACCOUNTType __fastcall Get_ACCOUNT() = 0;
  // Methods & Properties 
  __property _di_IXMLACCOUNTType ACCOUNT = { read=Get_ACCOUNT };
};

// IXMLACCOUNTType 

__interface INTERFACE_UUID("{3E248226-87F3-4B5A-9D85-F00B98E2586C}") IXMLACCOUNTType : public IXMLNode
{
public:
  // Property Accessors 
  virtual UnicodeString __fastcall Get_NAME() = 0;
  virtual Integer __fastcall Get_KEY() = 0;
  virtual Integer __fastcall Get_COVERS() = 0;
  virtual UnicodeString __fastcall Get_TYPE() = 0;
  virtual _di_IXMLACCOUNTIDENTType __fastcall Get_ACCOUNTIDENT() = 0;
  virtual _di_IXMLSERVINGCOURSETypeList __fastcall Get_SERVINGCOURSE() = 0;
  virtual void __fastcall Set_NAME(UnicodeString Value) = 0;
  virtual void __fastcall Set_KEY(Integer Value) = 0;
  virtual void __fastcall Set_COVERS(Integer Value) = 0;
  virtual void __fastcall Set_TYPE(UnicodeString Value) = 0;
  // Methods & Properties 
  __property UnicodeString NAME = { read=Get_NAME, write=Set_NAME };
  __property Integer KEY = { read=Get_KEY, write=Set_KEY };
  __property Integer COVERS = { read=Get_COVERS, write=Set_COVERS };
  __property UnicodeString TYPE = { read=Get_TYPE, write=Set_TYPE };
  __property _di_IXMLACCOUNTIDENTType ACCOUNTIDENT = { read=Get_ACCOUNTIDENT };
  __property _di_IXMLSERVINGCOURSETypeList SERVINGCOURSE = { read=Get_SERVINGCOURSE };
};

// IXMLACCOUNTIDENTType 

__interface INTERFACE_UUID("{CFCAD168-D89B-432D-AD9E-172C4E3B8D83}") IXMLACCOUNTIDENTType : public IXMLNode
{
public:
  // Property Accessors 
  virtual Integer __fastcall Get_TYPE() = 0;
  virtual Integer __fastcall Get_TABLE() = 0;
  virtual Integer __fastcall Get_SEAT() = 0;
  virtual Integer __fastcall Get_ROOM() = 0;
  virtual Integer __fastcall Get_WEBID() = 0;
  virtual Integer __fastcall Get_TAB() = 0;
  virtual Integer __fastcall Get_MEMBER() = 0;
  virtual void __fastcall Set_TYPE(Integer Value) = 0;
  virtual void __fastcall Set_TABLE(Integer Value) = 0;
  virtual void __fastcall Set_SEAT(Integer Value) = 0;
  virtual void __fastcall Set_ROOM(Integer Value) = 0;
  virtual void __fastcall Set_WEBID(Integer Value) = 0;
  virtual void __fastcall Set_TAB(Integer Value) = 0;
  virtual void __fastcall Set_MEMBER(Integer Value) = 0;
  // Methods & Properties 
  __property Integer TYPE = { read=Get_TYPE, write=Set_TYPE };
  __property Integer TABLE = { read=Get_TABLE, write=Set_TABLE };
  __property Integer SEAT = { read=Get_SEAT, write=Set_SEAT };
  __property Integer ROOM = { read=Get_ROOM, write=Set_ROOM };
  __property Integer WEBID = { read=Get_WEBID, write=Set_WEBID };
  __property Integer TAB = { read=Get_TAB, write=Set_TAB };
  __property Integer MEMBER = { read=Get_MEMBER, write=Set_MEMBER };
};

// IXMLSERVINGCOURSEType 

__interface INTERFACE_UUID("{3FE6B180-5C4E-4E2A-9873-5D8C22A4C7DC}") IXMLSERVINGCOURSEType : public IXMLNode
{
public:
  // Property Accessors 
  virtual Integer __fastcall Get_KEY() = 0;
  virtual UnicodeString __fastcall Get_NAME() = 0;
  virtual void __fastcall Set_KEY(Integer Value) = 0;
  virtual void __fastcall Set_NAME(UnicodeString Value) = 0;
  // Methods & Properties 
  __property Integer KEY = { read=Get_KEY, write=Set_KEY };
  __property UnicodeString NAME = { read=Get_NAME, write=Set_NAME };
};

// IXMLSERVINGCOURSETypeList 

__interface INTERFACE_UUID("{2CACD9D5-59B5-421B-A5BB-AF643BD257E7}") IXMLSERVINGCOURSETypeList : public IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLSERVINGCOURSEType __fastcall Add() = 0;
  virtual _di_IXMLSERVINGCOURSEType __fastcall Insert(const int Index) = 0;
  virtual _di_IXMLSERVINGCOURSEType __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLSERVINGCOURSEType Items[int Index] = { read=Get_Item /* default */ };
};

// Forward Decls 

class TXMLCALLAWAYType;
class TXMLACCOUNTType;
class TXMLACCOUNTIDENTType;
class TXMLSERVINGCOURSEType;
class TXMLSERVINGCOURSETypeList;

// TXMLCALLAWAYType 

class TXMLCALLAWAYType : public TXMLNode, public IXMLCALLAWAYType
{
  __IXMLNODE_IMPL__
protected:
  // IXMLCALLAWAYType 
  virtual _di_IXMLACCOUNTType __fastcall Get_ACCOUNT();
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLACCOUNTType 

class TXMLACCOUNTType : public TXMLNode, public IXMLACCOUNTType
{
  __IXMLNODE_IMPL__
private:
  _di_IXMLSERVINGCOURSETypeList FSERVINGCOURSE;
protected:
  // IXMLACCOUNTType 
  virtual UnicodeString __fastcall Get_NAME();
  virtual Integer __fastcall Get_KEY();
  virtual Integer __fastcall Get_COVERS();
  virtual UnicodeString __fastcall Get_TYPE();
  virtual _di_IXMLACCOUNTIDENTType __fastcall Get_ACCOUNTIDENT();
  virtual _di_IXMLSERVINGCOURSETypeList __fastcall Get_SERVINGCOURSE();
  virtual void __fastcall Set_NAME(UnicodeString Value);
  virtual void __fastcall Set_KEY(Integer Value);
  virtual void __fastcall Set_COVERS(Integer Value);
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
  virtual Integer __fastcall Get_ROOM();
  virtual Integer __fastcall Get_WEBID();
  virtual Integer __fastcall Get_TAB();
  virtual Integer __fastcall Get_MEMBER();
  virtual void __fastcall Set_TYPE(Integer Value);
  virtual void __fastcall Set_TABLE(Integer Value);
  virtual void __fastcall Set_SEAT(Integer Value);
  virtual void __fastcall Set_ROOM(Integer Value);
  virtual void __fastcall Set_WEBID(Integer Value);
  virtual void __fastcall Set_TAB(Integer Value);
  virtual void __fastcall Set_MEMBER(Integer Value);
};

// TXMLSERVINGCOURSEType 

class TXMLSERVINGCOURSEType : public TXMLNode, public IXMLSERVINGCOURSEType
{
  __IXMLNODE_IMPL__
protected:
  // IXMLSERVINGCOURSEType 
  virtual Integer __fastcall Get_KEY();
  virtual UnicodeString __fastcall Get_NAME();
  virtual void __fastcall Set_KEY(Integer Value);
  virtual void __fastcall Set_NAME(UnicodeString Value);
};

// TXMLSERVINGCOURSETypeList 

class TXMLSERVINGCOURSETypeList : public TXMLNodeCollection, public IXMLSERVINGCOURSETypeList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLSERVINGCOURSETypeList 
  virtual _di_IXMLSERVINGCOURSEType __fastcall Add();
  virtual _di_IXMLSERVINGCOURSEType __fastcall Insert(const int Index);
  virtual _di_IXMLSERVINGCOURSEType __fastcall Get_Item(int Index);
};

// Global Functions 

_di_IXMLCALLAWAYType __fastcall GetCALLAWAY(_di_IXMLDocument Doc);
_di_IXMLCALLAWAYType __fastcall GetCALLAWAY(TXMLDocument *Doc);
_di_IXMLCALLAWAYType __fastcall LoadCALLAWAY(const UnicodeString& FileName);
_di_IXMLCALLAWAYType __fastcall  NewCALLAWAY();

#define TargetNamespace ""
}

#endif
