
// ********************************************************************************************** //
//                                                                                              
//                                       XML Data Binding                                       
//                                                                                              
//         Generated on: 8/21/2013 5:17:57 PM                                                   
//       Generated from: C:\Code\MMDev\Static Files\XML Examples\Master Printing Template.xml   
//                                                                                              
// ********************************************************************************************** //

#ifndef   XMLPrintJobmmH
#define   XMLPrintJobmmH

#include <System.hpp>
#include <xmldom.hpp>
#include <XMLDoc.hpp>
#include <XMLIntf.hpp>
#include <XMLNodeImp.h>

namespace XMLPrintJob
{
// Forward Decls 

__interface IXMLPRINTJOBType;
typedef System::DelphiInterface<IXMLPRINTJOBType> _di_IXMLPRINTJOBType;

// IXMLPRINTJOBType 

__interface INTERFACE_UUID("{6DA50915-815D-46B2-88FD-F38F0E732F5A}") IXMLPRINTJOBType : public IXMLNodeCollection
{
public:
public:
  // Property Accessors 
  virtual UnicodeString __fastcall Get_NAME() = 0;
  virtual UnicodeString __fastcall Get_DEVICENAME() = 0;
  virtual UnicodeString __fastcall Get_DEVICEGUID() = 0;
  virtual UnicodeString __fastcall Get_TRANSNO() = 0;
  virtual UnicodeString __fastcall Get_STARTTIME() = 0;
  virtual UnicodeString __fastcall Get_OrderGUID(int Index) = 0;
  virtual void __fastcall Set_NAME(UnicodeString Value) = 0;
  virtual void __fastcall Set_DEVICENAME(UnicodeString Value) = 0;
  virtual void __fastcall Set_DEVICEGUID(UnicodeString Value) = 0;
  virtual void __fastcall Set_TRANSNO(UnicodeString Value) = 0;
  virtual void __fastcall Set_STARTTIME(UnicodeString Value) = 0;
  // Methods & Properties 
  virtual _di_IXMLNode __fastcall Add(const UnicodeString OrderGUID) = 0;
  virtual _di_IXMLNode __fastcall Insert(const int Index, const UnicodeString OrderGUID) = 0;
  __property UnicodeString NAME = { read=Get_NAME, write=Set_NAME };
  __property UnicodeString DEVICENAME = { read=Get_DEVICENAME, write=Set_DEVICENAME };
  __property UnicodeString DEVICEGUID = { read=Get_DEVICEGUID, write=Set_DEVICEGUID };
  __property UnicodeString TRANSNO = { read=Get_TRANSNO, write=Set_TRANSNO };
  __property UnicodeString STARTTIME = { read=Get_STARTTIME, write=Set_STARTTIME };
  __property UnicodeString OrderGUID[int Index] = { read=Get_OrderGUID };/* default */
};

// Forward Decls 

class TXMLPRINTJOBType;

// TXMLPRINTJOBType 

class TXMLPRINTJOBType : public TXMLNodeCollection, public IXMLPRINTJOBType
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLPRINTJOBType 
  virtual UnicodeString __fastcall Get_NAME();
  virtual UnicodeString __fastcall Get_DEVICENAME();
  virtual UnicodeString __fastcall Get_DEVICEGUID();
  virtual UnicodeString __fastcall Get_TRANSNO();
  virtual UnicodeString __fastcall Get_STARTTIME();
  virtual UnicodeString __fastcall Get_OrderGUID(int Index);
  virtual void __fastcall Set_NAME(UnicodeString Value);
  virtual void __fastcall Set_DEVICENAME(UnicodeString Value);
  virtual void __fastcall Set_DEVICEGUID(UnicodeString Value);
  virtual void __fastcall Set_TRANSNO(UnicodeString Value);
  virtual void __fastcall Set_STARTTIME(UnicodeString Value);
  virtual _di_IXMLNode __fastcall Add(const UnicodeString OrderGUID);
  virtual _di_IXMLNode __fastcall Insert(const int Index, const UnicodeString OrderGUID);
public:
  virtual void __fastcall AfterConstruction(void);
};

// Global Functions 

_di_IXMLPRINTJOBType __fastcall GetPRINTJOB(_di_IXMLDocument Doc);
_di_IXMLPRINTJOBType __fastcall GetPRINTJOB(TXMLDocument *Doc);
_di_IXMLPRINTJOBType __fastcall LoadPRINTJOB(const UnicodeString& FileName);
_di_IXMLPRINTJOBType __fastcall  NewPRINTJOB();

#define TargetNamespace ""
}
#endif