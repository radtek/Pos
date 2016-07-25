// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'OpMSO.pas' rev: 6.00

#ifndef OpMSOHPP
#define OpMSOHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <OpPptXP.hpp>	// Pascal unit
#include <OpXLXP.hpp>	// Pascal unit
#include <OpOfcXP.hpp>	// Pascal unit
#include <OpWrdXP.hpp>	// Pascal unit
#include <OpShared.hpp>	// Pascal unit
#include <OpConst.hpp>	// Pascal unit
#include <ActiveX.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Opmso
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TOpBalloon;
class PASCALIMPLEMENTATION TOpBalloon : public Classes::TPersistent 
{
	typedef Classes::TPersistent inherited;
	
private:
	Classes::TPersistent* FAssistant;
	Opofcxp::_di_Balloon FCOMBalloon;
	AnsiString FText;
	void __fastcall SetText(const AnsiString Value);
	
public:
	__fastcall TOpBalloon(Classes::TPersistent* AOwner);
	DYNAMIC Classes::TPersistent* __fastcall GetOwner(void);
	void __fastcall Show(void);
	void __fastcall SetAppAssistant(Opofcxp::_di_Assistant COMAssistant);
	
__published:
	__property AnsiString Text = {read=FText, write=SetText};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TOpBalloon(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpAssistant;
class PASCALIMPLEMENTATION TOpAssistant : public Opshared::TOpBaseComponent 
{
	typedef Opshared::TOpBaseComponent inherited;
	
private:
	bool FVisible;
	TOpBalloon* FBalloon;
	int FTop;
	int FLeft;
	Word FReduced;
	Word FAssistWithHelp;
	Word FAssistWithWizards;
	Word FAssistWithAlerts;
	Word FMoveWhenInTheWay;
	Word FSounds;
	Word FFeatureTips;
	Word FMouseTips;
	Word FKeyboardShortcutTips;
	Word FHighPriorityTips;
	Word FTipOfDay;
	Word FGuessHelp;
	Word FSearchWhenProgramming;
	AnsiString FFileName;
	Opshared::TOpOfficeComponent* FOfficeComponent;
	void __fastcall ConnectListener(Opshared::TOpOfficeComponent* Instance, bool Connect);
	void __fastcall SetVisible(bool Value);
	void __fastcall SetBalloon(TOpBalloon* Value);
	Word __fastcall GetAssistWithAlerts(void);
	Word __fastcall GetAssistWithHelp(void);
	Word __fastcall GetAssistWithWizards(void);
	Word __fastcall GetFeatureTips(void);
	AnsiString __fastcall GetFileName();
	Word __fastcall GetGuessHelp(void);
	Word __fastcall GetHighPriorityTips(void);
	Word __fastcall GetKeyboardShortcutTips(void);
	int __fastcall GetLeft(void);
	Word __fastcall GetMouseTips(void);
	Word __fastcall GetMoveWhenInTheWay(void);
	Word __fastcall GetReduced(void);
	Word __fastcall GetSearchWhenProgramming(void);
	Word __fastcall GetSounds(void);
	Word __fastcall GetTipOfDay(void);
	int __fastcall GetTop(void);
	void __fastcall SetAssistWithAlerts(const Word Value);
	void __fastcall SetAssistWithHelp(const Word Value);
	void __fastcall SetAssistWithWizards(const Word Value);
	void __fastcall SetFeatureTips(const Word Value);
	void __fastcall SetFileName(const AnsiString Value);
	void __fastcall SetGuessHelp(const Word Value);
	void __fastcall SetHighPriorityTips(const Word Value);
	void __fastcall SetKeyboardShortcutTips(const Word Value);
	void __fastcall SetLeft(const int Value);
	void __fastcall SetMouseTips(const Word Value);
	void __fastcall SetMoveWhenInTheWay(const Word Value);
	void __fastcall SetReduced(const Word Value);
	void __fastcall SetSearchWhenProgramming(const Word Value);
	void __fastcall SetSounds(const Word Value);
	void __fastcall SetTipOfDay(const Word Value);
	void __fastcall SetTop(const int Value);
	void __fastcall SetOfficeComponent(const Opshared::TOpOfficeComponent* Value);
	
protected:
	virtual void __fastcall Notification(Classes::TComponent* AComponent, Classes::TOperation Operation);
	
public:
	__fastcall virtual TOpAssistant(Classes::TComponent* AOwner);
	__fastcall virtual ~TOpAssistant(void);
	void __fastcall GetFileInfo(AnsiString &Filter, AnsiString &DefExt);
	bool __fastcall GetOfficeConnected(void);
	
__published:
	__property bool Visible = {read=FVisible, write=SetVisible, default=0};
	__property TOpBalloon* Balloon = {read=FBalloon, write=SetBalloon};
	__property int AssistantTop = {read=GetTop, write=SetTop, nodefault};
	__property int AssistantLeft = {read=GetLeft, write=SetLeft, nodefault};
	__property Word Reduced = {read=GetReduced, write=SetReduced, nodefault};
	__property Word AssistWithHelp = {read=GetAssistWithHelp, write=SetAssistWithHelp, nodefault};
	__property Word AssistWithWizards = {read=GetAssistWithWizards, write=SetAssistWithWizards, nodefault};
	__property Word AssistWithAlerts = {read=GetAssistWithAlerts, write=SetAssistWithAlerts, nodefault};
	__property Word MoveWhenInTheWay = {read=GetMoveWhenInTheWay, write=SetMoveWhenInTheWay, nodefault};
	__property Word Sounds = {read=GetSounds, write=SetSounds, nodefault};
	__property Word FeatureTips = {read=GetFeatureTips, write=SetFeatureTips, nodefault};
	__property Word MouseTips = {read=GetMouseTips, write=SetMouseTips, nodefault};
	__property Word KeyboardShortcutTips = {read=GetKeyboardShortcutTips, write=SetKeyboardShortcutTips, nodefault};
	__property Word HighPriorityTips = {read=GetHighPriorityTips, write=SetHighPriorityTips, nodefault};
	__property Word TipOfDay = {read=GetTipOfDay, write=SetTipOfDay, nodefault};
	__property Word GuessHelp = {read=GetGuessHelp, write=SetGuessHelp, nodefault};
	__property Word SearchWhenProgramming = {read=GetSearchWhenProgramming, write=SetSearchWhenProgramming, nodefault};
	__property AnsiString FileName = {read=GetFileName, write=SetFileName, stored=false};
	__property Opshared::TOpOfficeComponent* OfficeComponent = {read=FOfficeComponent, write=SetOfficeComponent};
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Opmso */
using namespace Opmso;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// OpMSO
