// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdMessageCollection.pas' rev: 6.00

#ifndef IdMessageCollectionHPP
#define IdMessageCollectionHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <IdMessage.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Idmessagecollection
{
//-- type declarations -------------------------------------------------------
typedef TMetaClass*TIdMessageItems;

class DELPHICLASS TIdMessageItem;
class PASCALIMPLEMENTATION TIdMessageItem : public Classes::TCollectionItem 
{
	typedef Classes::TCollectionItem inherited;
	
protected:
	int FAttempt;
	bool FQueued;
	
public:
	Idmessage::TIdMessage* IdMessage;
	__property int Attempt = {read=FAttempt, write=FAttempt, nodefault};
	__property bool Queued = {read=FQueued, write=FQueued, nodefault};
	__fastcall virtual TIdMessageItem(Classes::TCollection* Collection);
	__fastcall virtual ~TIdMessageItem(void);
};


class DELPHICLASS TIdMessageCollection;
class PASCALIMPLEMENTATION TIdMessageCollection : public Classes::TCollection 
{
	typedef Classes::TCollection inherited;
	
public:
	Idmessage::TIdMessage* operator[](int index) { return Messages[index]; }
	
private:
	Idmessage::TIdMessage* __fastcall GetIdMessage(int index);
	void __fastcall SetIdMessage(int index, const Idmessage::TIdMessage* Value);
	
public:
	HIDESBASE TIdMessageItem* __fastcall Add(void);
	__property Idmessage::TIdMessage* Messages[int index] = {read=GetIdMessage, write=SetIdMessage/*, default*/};
public:
	#pragma option push -w-inl
	/* TCollection.Create */ inline __fastcall TIdMessageCollection(TMetaClass* ItemClass) : Classes::TCollection(ItemClass) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCollection.Destroy */ inline __fastcall virtual ~TIdMessageCollection(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Idmessagecollection */
using namespace Idmessagecollection;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdMessageCollection
