// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdMessageCoderMIME.pas' rev: 6.00

#ifndef IdMessageCoderMIMEHPP
#define IdMessageCoderMIMEHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <IdComponent.hpp>	// Pascal unit
#include <IdMessage.hpp>	// Pascal unit
#include <IdMessageCoder.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Idmessagecodermime
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TIdMessageDecoderMIME;
class PASCALIMPLEMENTATION TIdMessageDecoderMIME : public Idmessagecoder::TIdMessageDecoder 
{
	typedef Idmessagecoder::TIdMessageDecoder inherited;
	
protected:
	AnsiString FFirstLine;
	bool FBodyEncoded;
	AnsiString FMIMEBoundary;
	
public:
	__fastcall TIdMessageDecoderMIME(Classes::TComponent* AOwner)/* overload */;
	__fastcall TIdMessageDecoderMIME(Classes::TComponent* AOwner, AnsiString ALine)/* overload */;
	virtual Idmessagecoder::TIdMessageDecoder* __fastcall ReadBody(Classes::TStream* ADestStream, bool &VMsgEnd);
	virtual void __fastcall ReadHeader(void);
	__property AnsiString MIMEBoundary = {read=FMIMEBoundary, write=FMIMEBoundary};
	__property bool BodyEncoded = {read=FBodyEncoded, write=FBodyEncoded, nodefault};
public:
	#pragma option push -w-inl
	/* TIdMessageDecoder.Destroy */ inline __fastcall virtual ~TIdMessageDecoderMIME(void) { }
	#pragma option pop
	
};


class DELPHICLASS TIdMessageDecoderInfoMIME;
class PASCALIMPLEMENTATION TIdMessageDecoderInfoMIME : public Idmessagecoder::TIdMessageDecoderInfo 
{
	typedef Idmessagecoder::TIdMessageDecoderInfo inherited;
	
public:
	virtual Idmessagecoder::TIdMessageDecoder* __fastcall CheckForStart(Idmessage::TIdMessage* ASender, AnsiString ALine);
public:
	#pragma option push -w-inl
	/* TIdMessageDecoderInfo.Create */ inline __fastcall virtual TIdMessageDecoderInfoMIME(void) : Idmessagecoder::TIdMessageDecoderInfo() { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TIdMessageDecoderInfoMIME(void) { }
	#pragma option pop
	
};


class DELPHICLASS TIdMessageEncoderMIME;
class PASCALIMPLEMENTATION TIdMessageEncoderMIME : public Idmessagecoder::TIdMessageEncoder 
{
	typedef Idmessagecoder::TIdMessageEncoder inherited;
	
public:
	virtual void __fastcall Encode(Classes::TStream* ASrc, Classes::TStream* ADest)/* overload */;
public:
	#pragma option push -w-inl
	/* TIdMessageEncoder.Create */ inline __fastcall virtual TIdMessageEncoderMIME(Classes::TComponent* AOwner) : Idmessagecoder::TIdMessageEncoder(AOwner) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TIdComponent.Destroy */ inline __fastcall virtual ~TIdMessageEncoderMIME(void) { }
	#pragma option pop
	
	
/* Hoisted overloads: */
	
public:
	inline void __fastcall  Encode(const AnsiString AFilename, Classes::TStream* ADest){ TIdMessageEncoder::Encode(AFilename, ADest); }
	
};


class DELPHICLASS TIdMessageEncoderInfoMIME;
class PASCALIMPLEMENTATION TIdMessageEncoderInfoMIME : public Idmessagecoder::TIdMessageEncoderInfo 
{
	typedef Idmessagecoder::TIdMessageEncoderInfo inherited;
	
public:
	__fastcall virtual TIdMessageEncoderInfoMIME(void);
	virtual void __fastcall InitializeHeaders(Idmessage::TIdMessage* AMsg);
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TIdMessageEncoderInfoMIME(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
#define IndyMIMEBoundary "=_MoreStuf_2zzz1234sadvnqw3nerasdf"
#define IndyMultiPartAlternativeBoundary "=_MoreStuf_2altzzz1234sadvnqw3nerasdf"
#define IndyMultiPartRelatedBoundary "=_MoreStuf_2relzzzsadvnq1234w3nerasdf"
#define MIMEGenericText "text/"
#define MIMEGenericMultiPart "multipart/"
#define MIME7Bit "7bit"

}	/* namespace Idmessagecodermime */
using namespace Idmessagecodermime;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdMessageCoderMIME
