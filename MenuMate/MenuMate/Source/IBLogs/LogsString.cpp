#include <vcl.h>
//---------------------------------------------------------------------------
#pragma hdrstop

#include "LogsString.h"
#include <memory>

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------
#include "FileCtrl.hpp"
//---------------------------------------------------------------------
const int MaxFileLogRecords	= 1000;
const int EventsToLog		= 50;			// Log this many each loop
//---------------------------------------------------------------------

TManagerLogs *Logs;
//---------------------------------------------------------------------
__fastcall TManagerLogs::TManagerLogs() : EventList(new TList()),ErrorsToLog (new TStringList())
{
	Enabled = false;
   Logs = this;
	HaltProcessSome = false;
	ProcessingEnabled = true;
	ErrorsToLog->Add(EXCEPTIONLOG);
	ErrorsToLog->Add(ERRORLOG);
	ErrorsToLog->Add(VERSIONLOG);
	ErrorsToLog->Add(UPDATELOG);
	#ifdef _DEBUG
	ErrorsToLog->Add(DEBUGLOG);
	ErrorsToLog->Add(PHOENIXINTERFACELOG);
	ErrorsToLog->Add(PALMBTLAYERLOG);
	ErrorsToLog->Add(PALMPACKETLAYERLOG);
	ErrorsToLog->Add(PALMDECODEPACKETLOG);
	ErrorsToLog->Add(SMARTCARDLOG);
	ErrorsToLog->Add(XMLINTERFACELOG);
	ErrorsToLog->Add(DLLSTOCKLOG);
	ErrorsToLog->Add(EFTPOSLOG);
	ErrorsToLog->Add(DLLNZALOG);
  	ErrorsToLog->Add(REGISTRATIONLOG);
	ErrorsToLog->Add(WEBMATELOG);
	ErrorsToLog->Add(MEMBERSHIPINTERFACELOG);
   #endif
   LogFile = ExtractFilePath(Application->ExeName)+ ExtractFileName(Application->ExeName) + ".log";
}
//---------------------------------------------------------------------
TManagerLogs::~TManagerLogs()
{
	try
	{
		ClearAll();
		Enabled = false;
	}
	catch (Exception &E)
	{
		Enabled = false;
	}
}
//---------------------------------------------------------------------
void TManagerLogs::ClearAll()
{
	__int64 startTick = ::GetTickCount();
	while(EventList->Count > 0)
	{
		Process();
	}
	ErrorsToLog->Clear();
}
//---------------------------------------------------------------------
void TManagerLogs::Initialise(UnicodeString inTerminalName)
{
	TerminalName = inTerminalName;
   LogFile = ExtractFilePath(Application->ExeName)+ ExtractFileName(Application->ExeName) + ".log";
	Enabled = true;
   for(int i = 0 ; i < ErrorsToLog->Count ; i ++)
   {
   	TManagerLogs::Instance().Add(__FUNC__,DEBUGLOG, ErrorsToLog->Strings[i] );
   }
}
//---------------------------------------------------------------------
void TManagerLogs::Process()
{
	try
   {
      try
      {
         std::auto_ptr<TStringList> List(new TStringList);
         if(FileExists(LogFile))
         {
            List->LoadFromFile(LogFile);

            while (List->Count > MaxFileLogRecords)
            {
               List->Delete(0);
            }
         }

         while (EventList->Count != 0)
         {
            TLogRecord *CurrentRecord = (TLogRecord *) EventList->Items[0];
            try
            {
               RecordToStrings(CurrentRecord, List.get());
            }
            __finally
            {
               delete CurrentRecord;
               EventList->Delete(0);
            }
         }
         List->SaveToFile(LogFile);
      }
      catch (Exception &E)
      {
	   	TManagerLogs::Instance().Add(__FUNC__,DEBUGLOG, E.Message );
      }
   }
   __finally
   {
      HaltProcessSome = false;
   }
}
//---------------------------------------------------------------------
void TManagerLogs::Purge()
{
	if(!Enabled) return;
   if(FileExists(LogFile))
   {
      std::auto_ptr<TStringList> List(new TStringList);
      if(FileExists(LogFile))
      {
         List->LoadFromFile(LogFile);

         while (List->Count > MaxFileLogRecords)
         {
            List->Delete(0);
         }
      }
      List->SaveToFile(LogFile);
   }
}

void TManagerLogs::RecordToStrings(TLogRecord * CurrentRecord,TStrings * List)
{
   try
   {
      List->Add(CurrentRecord->TimeStamp.DateTimeString() + "\t\t\t" + CurrentRecord->Type.SubString(1,20) + "\t\t\t" + CurrentRecord->Msg.SubString(1,180) + "\t\t\t" + CurrentRecord->Function.SubString(1,60));
   }
	catch(Exception &E)
   {
      TStringList *List = new TStringList;
      if(FileExists(LogFile))
      {
         List->LoadFromFile(LogFile);
      }
      List->Add(E.Message);
      List->SaveToFile(LogFile);
      delete List;
   }
}

//---------------------------------------------------------------------
void TManagerLogs::Add(UnicodeString Function,UnicodeString Type, UnicodeString Msg, UnicodeString Terminal)
{
	try
	{
		if(ErrorsToLog->IndexOf(Type) != -1 && Enabled)
		{
			if(Terminal == "")
			{
				Terminal = TerminalName;
			}
			if(EventList->Count < 5000)
			{
				EventList->Add( new TLogRecord(Terminal,Function,Type,Msg,Now()));
			}
		}
	}
	catch (...)
	{
	}
}


void TManagerLogs::ClearAllDB()
{
	HaltProcessSome = true;
	try
	{
      DeleteFile(LogFile);
   }
   __finally
   {
		HaltProcessSome = false;
	}
}

TManagerLogs::TLogRecord::TLogRecord(UnicodeString inhost_name,
                                   UnicodeString inFunction,
                                   UnicodeString inType,
                                   UnicodeString inMsg, TDateTime InTimeStamp) : Key(0)
{
	host_name = inhost_name;
   Function = inFunction;
   Type = inType;
   Msg = inMsg;
	TimeStamp = InTimeStamp;
}
//---------------------------------------------------------------------------
void TManagerLogs::AddLastError(UnicodeString Type,UnicodeString Func)
{
	UnicodeString WinErrorMsg = SysErrorMessage(GetLastError());
	Add(Func,Type,WinErrorMsg);
}

//---------------------------------------------------------------------------
