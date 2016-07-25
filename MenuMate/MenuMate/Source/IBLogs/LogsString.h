
#ifndef LogsStringH
#define LogsStringH
//---------------------------------------------------------------------------

#include "logsdef.h"
#include <System.hpp>
#include <Generics.Collections.hpp>
#include <memory>
#include <System.hpp>
#include <Classes.hpp>


class TManagerLogs
{
private:
	bool HaltProcessSome;
	bool Enabled;
   UnicodeString LogFile;
public:
	__fastcall TManagerLogs();
	~TManagerLogs();
   std::auto_ptr<TList> EventList;
   std::auto_ptr<TStringList> ErrorsToLog;
	bool OnLineLogging;


    static TManagerLogs& Instance() {
        static TManagerLogs singleton;
        return singleton;
    }


   class TLogRecord
   {
	public:
		TLogRecord(UnicodeString inhost_name, UnicodeString inFunction, UnicodeString inType, UnicodeString inMsg, TDateTime InTimeStamp);
		UnicodeString	host_name;
		UnicodeString	Function;
		UnicodeString	Type;
		UnicodeString	Msg;
		TDateTime	TimeStamp;
		int			Key;
   };

   void Add(UnicodeString Function,UnicodeString Type, UnicodeString Msg,UnicodeString Terminal = "");
   void AddLastError(UnicodeString Type,UnicodeString Func = "NA");
   void ClearAll();
   void ClearAllDB();
   void Process();
   void RecordToStrings(TLogRecord * CurrentRecord,TStrings * List);
	void Purge();
   bool ProcessingEnabled;
   UnicodeString TerminalName;
	void Initialise(UnicodeString inTerminalName);
};

#endif
