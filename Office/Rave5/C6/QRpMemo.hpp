// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRpMemo.pas' rev: 6.00

#ifndef QRpMemoHPP
#define QRpMemoHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QRpBase.hpp>	// Pascal unit
#include <QRpDefine.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <QExtCtrls.hpp>	// Pascal unit
#include <QComCtrls.hpp>	// Pascal unit
#include <QStdCtrls.hpp>	// Pascal unit
#include <QDialogs.hpp>	// Pascal unit
#include <QForms.hpp>	// Pascal unit
#include <QGraphics.hpp>	// Pascal unit
#include <QControls.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrpmemo
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TMemoBuf;
class PASCALIMPLEMENTATION TMemoBuf : public Qrpbase::TBaseMemoBuf 
{
	typedef Qrpbase::TBaseMemoBuf inherited;
	
protected:
	double FMaxHeight;
	int FBufferInc;
	bool FNoNewLine;
	char *FSaveBuffer;
	int FSaveSize;
	Classes::TList* FSaveRPTFList;
	char *FBuffer;
	int FSize;
	int FMaxSize;
	int FPos;
	bool FNewParagraph;
	bool NewLine;
	AnsiString FSearchText;
	int FSearchLen;
	bool FCaseMatters;
	Classes::TList* FRPTFList;
	bool FProcessRPTF;
	System::SmallString<60>  LastRPTF;
	bool ReadRTF;
	bool ReplaceRPTF;
	int NonSpacePos;
	double LeftIndent;
	double RightIndent;
	int SaveFPos;
	bool SaveNewParagraph;
	bool SaveNewLine;
	AnsiString __fastcall GetText();
	void __fastcall SetText(AnsiString Value);
	void __fastcall SetRTFText(AnsiString Value);
	void __fastcall SetPos(int Value);
	void __fastcall SetMaxSize(int Value);
	void __fastcall SetMemo(Qstdctrls::TMemo* Value);
	void __fastcall SetSize(int Value);
	void __fastcall FreeBuffer(void);
	void __fastcall ClearRPTFList(Classes::TList* List);
	void __fastcall CopyRPTFList(Classes::TList* FromList, Classes::TList* ToList);
	void __fastcall AddRPTFString(AnsiString RPTFStr, int BufPos);
	AnsiString __fastcall FillRPTF(AnsiString TextStr, int StartPos);
	Qrpdefine::TFormatState __fastcall GetStateAtPos(int Pos);
	AnsiString __fastcall GetLine(int Width, bool &Eol);
	
public:
	__fastcall TMemoBuf(void);
	__fastcall virtual ~TMemoBuf(void);
	bool __fastcall HasRPTF(void);
	void __fastcall Reset(void);
	void __fastcall SaveState(void);
	void __fastcall RestoreState(void);
	AnsiString __fastcall GetWord();
	void __fastcall SetData(void *Buffer, int BufSize);
	void __fastcall SetRTF(void *Buffer, int BufSize);
	void __fastcall SaveToStream(Classes::TStream* Stream);
	void __fastcall LoadFromStream(Classes::TStream* Stream, int BufSize);
	void __fastcall LoadFromFile(AnsiString FileName);
	void __fastcall RTFLoadFromStream(Classes::TStream* Stream, int BufSize);
	void __fastcall RTFLoadFromFile(AnsiString FileName);
	void __fastcall InsertMemoBuf(int BufPos, TMemoBuf* MemoBuf);
	void __fastcall AppendMemoBuf(TMemoBuf* MemoBuf);
	bool __fastcall Empty(void);
	void __fastcall Insert(int BufPos, AnsiString Text);
	void __fastcall Append(AnsiString Text);
	void __fastcall Delete(int BufPos, int DelLen);
	bool __fastcall SearchFirst(AnsiString SearchText, bool CaseMatters);
	bool __fastcall SearchNext(void);
	void __fastcall ReplaceAll(AnsiString SearchText, AnsiString ReplaceText, bool CaseMatters);
	void __fastcall SaveBuffer(void);
	void __fastcall RestoreBuffer(void);
	void __fastcall FreeSaved(void);
	virtual AnsiString __fastcall GetNextLine(bool &Eol);
	virtual int __fastcall MemoLinesLeft(void);
	virtual double __fastcall PrintLines(int Lines, bool PrintTabs);
	double __fastcall PrintHeight(double Height, bool PrintTabs);
	double __fastcall MemoHeightLeft(void);
	double __fastcall ConstrainHeightLeft(double Constraint);
	__property bool NewParagraph = {read=FNewParagraph, nodefault};
	__property AnsiString Text = {read=GetText, write=SetText};
	__property AnsiString RTFText = {write=SetRTFText};
	__property int Size = {read=FSize, write=SetSize, nodefault};
	__property int MaxSize = {read=FMaxSize, write=SetMaxSize, nodefault};
	__property int Pos = {read=FPos, write=SetPos, nodefault};
	__property Qrpdefine::PCharArray Buffer = {read=FBuffer};
	__property bool NoCRLF = {read=FNoNewLine, write=FNoNewLine, nodefault};
	__property bool NoNewLine = {read=FNoNewLine, write=FNoNewLine, nodefault};
	__property int BufferInc = {read=FBufferInc, write=FBufferInc, nodefault};
	__property Qstdctrls::TMemo* Memo = {write=SetMemo};
	__property bool ProcessRPTF = {read=FProcessRPTF, write=FProcessRPTF, nodefault};
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Qrpmemo */
using namespace Qrpmemo;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRpMemo
