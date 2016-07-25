//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerSecurity.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

void __fastcall ManagerSecurity::ProcessSecurity(Database::TDBTransaction &DBTransaction, int SecRef, int UserKey, UnicodeString Event,
   UnicodeString FromVal, UnicodeString ToVal, TDateTime TimeStamp, UnicodeString TerminalName, UnicodeString Note)
{

   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  int seckey;
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_SECURITY_KEY, 1) FROM RDB$DATABASE";
	  IBInternalQuery->ExecQuery();
	  seckey = IBInternalQuery->Fields[0]->AsInteger;

	  // Add new Reocrd.
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Clear();
	  IBInternalQuery->SQL->Text =
		  "INSERT INTO SECURITY (" "SECURITY_KEY, " "SECURITY_REF, " "SECURITY_EVENT, " "FROM_VAL, " "TO_VAL, " "NOTE, "
		  "TERMINAL_NAME, " "USER_KEY, " "TIME_STAMP) " "VALUES (" ":SECURITY_KEY, " ":SECURITY_REF, " ":SECURITY_EVENT, " ":FROM_VAL, "
		  ":TO_VAL, " ":NOTE, " ":TERMINAL_NAME, " ":USER_KEY, " ":TIME_STAMP);";
	  IBInternalQuery->ParamByName("SECURITY_KEY")->AsInteger = seckey;
	  IBInternalQuery->ParamByName("SECURITY_REF")->AsInteger = SecRef;
	  IBInternalQuery->ParamByName("USER_KEY")->AsInteger = UserKey;
	  IBInternalQuery->ParamByName("SECURITY_EVENT")->AsString = Event;
	  IBInternalQuery->ParamByName("FROM_VAL")->AsString = FromVal;
	  IBInternalQuery->ParamByName("TO_VAL")->AsString = ToVal;
	  IBInternalQuery->ParamByName("NOTE")->AsString = Note;
	  IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = TimeStamp;
	  IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TerminalName;
	  IBInternalQuery->ExecQuery();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
}

void __fastcall ManagerSecurity::ProcessSecurity(Database::TDBTransaction &DBTransaction, TListSecurityRefContainer *Security)
{
   try
   {
	  Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery);

	  for (int i = 0; i < Security->Count; i++)
	  {
		 TSecurityReference *SecRef = Security->SecurityGet(i);
		 if (SecRef && SecRef->UserKey)
		 {
			// Check For Dupliacte Ref and Events
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
				"SELECT SECURITY_REF FROM SECURITY WHERE SECURITY_REF = :SECURITY_REF AND SECURITY_EVENT = :SECURITY_EVENT";
			IBInternalQuery->ParamByName("SECURITY_EVENT")->AsString = SecRef->Event.SubString(1, 50);
			IBInternalQuery->ParamByName("SECURITY_REF")->AsInteger = Security->GetSecurityRefNumber();
			IBInternalQuery->ExecQuery();
			if (IBInternalQuery->RecordCount == 0)
			{
			   int seckey;
			   IBInternalQuery->Close();
			   IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_SECURITY_KEY, 1) FROM RDB$DATABASE";
			   IBInternalQuery->ExecQuery();
			   seckey = IBInternalQuery->Fields[0]->AsInteger;

			   IBInternalQuery->Close();
			   IBInternalQuery->SQL->Clear();
			   IBInternalQuery->SQL->Text =
				   "INSERT INTO SECURITY (" "SECURITY_KEY, " "SECURITY_REF, " "SECURITY_EVENT, " "FROM_VAL, " "TO_VAL, "
				   "NOTE, " "TERMINAL_NAME, " "USER_KEY, " "TIME_STAMP) " "VALUES (" ":SECURITY_KEY, " ":SECURITY_REF, "
				   ":SECURITY_EVENT, " ":FROM_VAL, " ":TO_VAL, " ":NOTE, " ":TERMINAL_NAME, " ":USER_KEY, "
				   ":TIME_STAMP);";
			   IBInternalQuery->ParamByName("SECURITY_KEY")->AsInteger = seckey;
			   IBInternalQuery->ParamByName("SECURITY_REF")->AsInteger = Security->GetSecurityRefNumber();
			   IBInternalQuery->ParamByName("USER_KEY")->AsInteger = SecRef->UserKey;
			   IBInternalQuery->ParamByName("SECURITY_EVENT")->AsString = SecRef->Event.SubString(1, 50);
			   IBInternalQuery->ParamByName("FROM_VAL")->AsString = SecRef->From.SubString(1, 50);
			   IBInternalQuery->ParamByName("TO_VAL")->AsString = SecRef->To.SubString(1, 50);
			   IBInternalQuery->ParamByName("NOTE")->AsString = SecRef->Note.SubString(1, 50);
			   IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = SecRef->TimeStamp;
			   IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = SecRef->TerminalName.SubString(1, 22);
			   IBInternalQuery->ExecQuery();
			}
		 }
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
}

// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------


void __fastcall ManagerSecurity::ProcessSecurity(TProcessSecurityQueries &Queries, TListSecurityRefContainer *Security)
{
   try
   {
	  for (int i = 0; i < Security->Count; i++)
	  {
		 TSecurityReference *SecRef = Security->SecurityGet(i);
		 if (SecRef && SecRef->UserKey)
		 {
			// Reuse security events if possible e.g. multiple items ordered at the same time.
			Queries.DuplicateCheck->Close();
			Queries.DuplicateCheck->ParamByName("SECURITY_EVENT")->AsString = SecRef->Event.SubString(1, 50);
			Queries.DuplicateCheck->ParamByName("SECURITY_REF")->AsInteger = Security->GetSecurityRefNumber();
			Queries.DuplicateCheck->ExecQuery();
			if (Queries.DuplicateCheck->RecordCount == 0)
			{
			   Queries.Insertor->Close();
			   Queries.Insertor->ParamByName("SECURITY_KEY")->AsInteger = Queries.GetKey();
			   Queries.Insertor->ParamByName("SECURITY_REF")->AsInteger = Security->GetSecurityRefNumber();
			   Queries.Insertor->ParamByName("USER_KEY")->AsInteger = SecRef->UserKey;
			   Queries.Insertor->ParamByName("SECURITY_EVENT")->AsString = SecRef->Event.SubString(1, 50);
			   Queries.Insertor->ParamByName("FROM_VAL")->AsString = SecRef->From.SubString(1, 50);
			   Queries.Insertor->ParamByName("TO_VAL")->AsString = SecRef->To.SubString(1, 50);
			   Queries.Insertor->ParamByName("NOTE")->AsString = SecRef->Note.SubString(1, 50);
			   Queries.Insertor->ParamByName("TIME_STAMP")->AsDateTime = SecRef->TimeStamp;
			   Queries.Insertor->ParamByName("TERMINAL_NAME")->AsString = SecRef->TerminalName.SubString(1, 22);
			   Queries.Insertor->ExecQuery();
			}
		 }
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
}
