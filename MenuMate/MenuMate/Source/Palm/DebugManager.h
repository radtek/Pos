//---------------------------------------------------------------------------

#ifndef DebugManagerH
#define DebugManagerH

#include "MMLogging.h"
#include "Enum.h"
//---------------------------------------------------------------------------

class TDebugHelp : public TObject
{
	private :
	public:
	int MessageCount;
	int TriggerCount;
	EProcessingErrorType Action;
	MESSAGE_TYPE MessageType;
};

class TManagerDebug  : public TList
{
	private :
      bool fEnabled;
      void __fastcall SetEnabled(bool value);
		bool __fastcall GetEnabled();

	UnicodeString MessageTypeToStr(MESSAGE_TYPE MessageType);
	UnicodeString ErrorTypeToStr(EProcessingErrorType ErrorType);
	public:
	void BugsClear();
	void BugsDelete(TDebugHelp *in);
	int BugsAdd(TDebugHelp *inSize);
	TDebugHelp *BugsGet(int Index);
	TDebugHelp *BugsGet(MESSAGE_TYPE inMessageType);
	void TManagerDebug::BugsGet(TStrings *List);	
	TManagerDebug();
	__fastcall virtual ~TManagerDebug();

	__property bool Enabled  = { read=GetEnabled, write=SetEnabled };
	EProcessingErrorType GetErrorNumber(MESSAGE_TYPE MessageType);
};

extern TManagerDebug *ManagerDebug;
#endif
