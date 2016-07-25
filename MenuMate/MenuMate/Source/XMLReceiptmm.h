
// ******************************************************************************************* //
//                                                                                           
//                                     XML Data Binding                                      
//                                                                                           
//         Generated on: 7/03/2012 10:05:27 a.m.                                             
//       Generated from: C:\Code\5.5\Static Files\XML Examples\Master Receipt Template.xml   
//                                                                                           
// ******************************************************************************************* //

#ifndef   XMLReceiptmmH
#define   XMLReceiptmmH

#include <System.hpp>
#include <xmldom.hpp>
#include <XMLDoc.hpp>
#include <XMLIntf.hpp>
#include <XMLNodeImp.h>

namespace XMLReceipt
{

// Forward Decls 

__interface IXMLRECEIPTType;
typedef System::DelphiInterface<IXMLRECEIPTType> _di_IXMLRECEIPTType;
__interface IXMLACCOUNTIDENTType;
typedef System::DelphiInterface<IXMLACCOUNTIDENTType> _di_IXMLACCOUNTIDENTType;

// IXMLRECEIPTType 

__interface INTERFACE_UUID("{B0A9E899-06D4-4295-B0BC-F6C189AE7A9D}") IXMLRECEIPTType : public IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLACCOUNTIDENTType __fastcall Get_ACCOUNTIDENT() = 0;
  // Methods & Properties 
  __property _di_IXMLACCOUNTIDENTType ACCOUNTIDENT = { read=Get_ACCOUNTIDENT };
};

// IXMLACCOUNTIDENTType 

__interface INTERFACE_UUID("{AAFF9660-7645-49E2-A3C4-63233761C9FD}") IXMLACCOUNTIDENTType : public IXMLNode
{
public:
  // Property Accessors 
  virtual Integer __fastcall Get_TYPE() = 0;
  virtual Integer __fastcall Get_TABLE() = 0;
  virtual Integer __fastcall Get_SEAT() = 0;
  virtual Integer __fastcall Get_ROOM() = 0;
  virtual Integer __fastcall Get_STAFF() = 0;
  virtual Integer __fastcall Get_WEBID() = 0;
  virtual Integer __fastcall Get_TAB() = 0;
  virtual Integer __fastcall Get_MEMBER() = 0;
  virtual void __fastcall Set_TYPE(Integer Value) = 0;
  virtual void __fastcall Set_TABLE(Integer Value) = 0;
  virtual void __fastcall Set_SEAT(Integer Value) = 0;
  virtual void __fastcall Set_ROOM(Integer Value) = 0;
  virtual void __fastcall Set_STAFF(Integer Value) = 0;
  virtual void __fastcall Set_WEBID(Integer Value) = 0;
  virtual void __fastcall Set_TAB(Integer Value) = 0;
  virtual void __fastcall Set_MEMBER(Integer Value) = 0;
  // Methods & Properties 
  __property Integer TYPE = { read=Get_TYPE, write=Set_TYPE };
  __property Integer TABLE = { read=Get_TABLE, write=Set_TABLE };
  __property Integer SEAT = { read=Get_SEAT, write=Set_SEAT };
  __property Integer ROOM = { read=Get_ROOM, write=Set_ROOM };
  __property Integer STAFF = { read=Get_STAFF, write=Set_STAFF };
  __property Integer WEBID = { read=Get_WEBID, write=Set_WEBID };
  __property Integer TAB = { read=Get_TAB, write=Set_TAB };
  __property Integer MEMBER = { read=Get_MEMBER, write=Set_MEMBER };
};

// Forward Decls 

class TXMLRECEIPTType;
class TXMLACCOUNTIDENTType;

// TXMLRECEIPTType 

class TXMLRECEIPTType : public TXMLNode, public IXMLRECEIPTType
{
  __IXMLNODE_IMPL__
protected:
  // IXMLRECEIPTType 
  virtual _di_IXMLACCOUNTIDENTType __fastcall Get_ACCOUNTIDENT();
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
  virtual Integer __fastcall Get_STAFF();
  virtual Integer __fastcall Get_WEBID();
  virtual Integer __fastcall Get_TAB();
  virtual Integer __fastcall Get_MEMBER();
  virtual void __fastcall Set_TYPE(Integer Value);
  virtual void __fastcall Set_TABLE(Integer Value);
  virtual void __fastcall Set_SEAT(Integer Value);
  virtual void __fastcall Set_ROOM(Integer Value);
  virtual void __fastcall Set_STAFF(Integer Value);
  virtual void __fastcall Set_WEBID(Integer Value);
  virtual void __fastcall Set_TAB(Integer Value);
  virtual void __fastcall Set_MEMBER(Integer Value);
};

// Global Functions 

_di_IXMLRECEIPTType __fastcall GetRECEIPT(_di_IXMLDocument Doc);
_di_IXMLRECEIPTType __fastcall GetRECEIPT(TXMLDocument *Doc);
_di_IXMLRECEIPTType __fastcall LoadRECEIPT(const UnicodeString& FileName);
_di_IXMLRECEIPTType __fastcall  NewRECEIPT();

#define TargetNamespace ""
}
#endif