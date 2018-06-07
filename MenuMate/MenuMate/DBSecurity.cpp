//---------------------------------------------------------------------------


#pragma hdrstop

#include "DBSecurity.h"
#include "MMLogging.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

int TDBSecurity::GetNextSecurityRef(Database::TDBTransaction &inDBTransaction)
{
   int key = 0;
   try
   {
	  TIBSQL *IBInternalQuery = inDBTransaction.Query(inDBTransaction.AddQuery());
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_SECURITY_REF, 1) FROM RDB$DATABASE";
	  IBInternalQuery->ExecQuery();
	  key = IBInternalQuery->Fields[0]->AsInteger;
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return key;
}

void __fastcall TDBSecurity::ProcessSecurity(Database::TDBTransaction &DBTransaction, int SecRef, int UserKey, UnicodeString Event,
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


void __fastcall TDBSecurity::ProcessSecurityFill(Database::TDBTransaction &DBTransaction, int SecRef, int UserKey, UnicodeString Event,
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

void __fastcall TDBSecurity::ProcessSecurity(Database::TDBTransaction &DBTransaction, TListSecurityRefContainer *Security, bool isTransactionCommit)
{
   try
   {

   Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
   DBTransaction.RegisterQuery(IBInternalQuery);
      DBTransaction.StartTransaction();
   for (int i = 0; i < Security->Count; i++)
   {
   TSecurityReference *SecRef = Security->SecurityGet(i);
   if (SecRef && SecRef->UserKey)
   {
       // Check For Dupliacte Ref and Events
        IBInternalQuery->Close();
        if((SecRef->Event == "CancelY")|| (SecRef->Event =="Cancel"))
        {
           IBInternalQuery->SQL->Text = "SELECT SECURITY_REF FROM SECURITY WHERE ((SECURITY_REF = :SECURITY_REF AND SECURITY_EVENT = 'Cancel') or (SECURITY_REF = :SECURITY_REF AND SECURITY_EVENT = 'CancelY'))  ";
        }
        else if(SecRef->Event == SecurityTypes[secTransferredPrelim])
        {
           IBInternalQuery->SQL->Text =
            "SELECT SECURITY_REF FROM SECURITY WHERE SECURITY_REF = :SECURITY_REF AND SECURITY_EVENT = :SECURITY_EVENT AND TO_VAL = :TO_VAL";
            IBInternalQuery->ParamByName("TO_VAL")->AsString = SecRef->To.SubString(1, 50);
            IBInternalQuery->ParamByName("SECURITY_EVENT")->AsString = SecRef->Event.SubString(1, 50);
        }
        else
        {
           IBInternalQuery->SQL->Text =
            "SELECT SECURITY_REF, FROM_VAL, TO_VAL, TIME_STAMP FROM SECURITY WHERE SECURITY_REF = :SECURITY_REF AND SECURITY_EVENT = :SECURITY_EVENT  ";
            if(SecRef->Event == "Transferred")
            {
               IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + " and TIME_STAMP = :TIME_STAMP ";
               IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = SecRef->TimeStamp;
            }
            IBInternalQuery->ParamByName("SECURITY_EVENT")->AsString = SecRef->Event.SubString(1, 50);

        }

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
     if(!isTransactionCommit)
     {
         DBTransaction.Commit();
         DBTransaction.StartTransaction();
     }

 }
   catch(Exception & E)
   {
   TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
   throw;
   }
}
// ---------------------------------------------------------------------------

void __fastcall TDBSecurity::ProcessSecurity(TProcessSecurityQueries &Queries, TListSecurityRefContainer *Security)
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

// ---------------------------------------------------------------------------
void __fastcall TDBSecurity::ClearSecurity(Database::TDBTransaction &DBTransaction, TListSecurityRefContainer *Security,
   TSecurityTypes Type)
{
   try
   {

	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  TSecurityReference *OldSecRef = Security->SecurityGetType(Type);
	  if (OldSecRef && OldSecRef->UserKey && Security)
	  {
		 if (OldSecRef != NULL)
		 {
			// Check For Dupliacte Ref and Events
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
				"SELECT SECURITY_REF FROM SECURITY WHERE SECURITY_REF = :SECURITY_REF AND SECURITY_EVENT = :SECURITY_EVENT";
			IBInternalQuery->ParamByName("SECURITY_EVENT")->AsString = OldSecRef->Event.SubString(1, 50);
			IBInternalQuery->ParamByName("SECURITY_REF")->AsInteger = Security->GetSecurityRefNumber();
			IBInternalQuery->ExecQuery();
			if (IBInternalQuery->RecordCount)
			{
			   IBInternalQuery->Close();
			   IBInternalQuery->SQL->Clear();
			   IBInternalQuery->SQL->Text = "DELETE FROM SECURITY WHERE SECURITY_REF = :SECURITY_REF AND SECURITY_EVENT = :SECURITY_EVENT";
			   IBInternalQuery->ParamByName("SECURITY_EVENT")->AsString = OldSecRef->Event.SubString(1, 50);
			   IBInternalQuery->ParamByName("SECURITY_REF")->AsInteger = Security->GetSecurityRefNumber();
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
//----------------------------------------------------------------------------------------------------------------
void TDBSecurity::SavePMSGuestDetails(TPaymentTransaction &paymentTransaction, TItemMinorComplete *Order, int tableNo, int seatNo)
{
    try
    {
        TItemMinorComplete *ItemMinorComplete = (TItemMinorComplete *)Order;
        TIBSQL *IBInternalQuery = paymentTransaction.DBTransaction.Query(paymentTransaction.DBTransaction.AddQuery());
        bool isRecordAlresdyExist = CheckWhetherGuestRecordExists(paymentTransaction.DBTransaction, Order->Security->GetSecurityRefNumber(),
                                        Order->RoomNoStr, seatNo, Order->AccNo);
        if(!isRecordAlresdyExist)
        {
             for (int i = 0; i < Order->Security->Count; i++)
             {
                TSecurityReference *SecRef = Order->Security->SecurityGet(i);
                if (SecRef && SecRef->UserKey && SecRef->Event.SubString(1, 50).Pos("Ordered By"))
                {
                    IBInternalQuery->Close();
                    IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_PMSGUESTDETAIL, 1) FROM RDB$DATABASE";
                    IBInternalQuery->ExecQuery();
                    int guestDetailsKey = IBInternalQuery->Fields[0]->AsInteger;

                    IBInternalQuery->Close();
                    IBInternalQuery->ParamCheck = true;
                    IBInternalQuery->SQL->Clear();
                    IBInternalQuery->SQL->Text =

                    "INSERT INTO PMSGUESTDETAILS ("
                    "GUESTDETAILKEY,"
                    "SECURITYREF,"
                    "ACCOUNTNUBER,"
                    "ROOMNUMBER,"
                    "FIRSTNAME, "
                    "LASTNAME, "
                    "TABLENUMBER, "
                    "SEATNUMBER  "
                    ")"
                    " VALUES "
                    "("
                    ":GUESTDETAILKEY, "
                    ":SECURITYREF,"
                    ":ACCOUNTNUBER,"
                    ":ROOMNUMBER,"
                    ":FIRSTNAME, "
                    ":LASTNAME, "
                    ":TABLENUMBER, "
                    ":SEATNUMBER  "
                    ");";

                    IBInternalQuery->ParamByName("GUESTDETAILKEY")->AsInteger = guestDetailsKey;
                    IBInternalQuery->ParamByName("SECURITYREF")->AsInteger = Order->Security->GetSecurityRefNumber();

                    IBInternalQuery->ParamByName("ACCOUNTNUBER")->AsString = Order->AccNo.SubString(1, 20);//paymentTransaction.Phoenix.AccountNumber.SubString(1, 20);
                    IBInternalQuery->ParamByName("ROOMNUMBER")->AsString = Order->RoomNoStr;//paymentTransaction.Phoenix.RoomNumber;
                    IBInternalQuery->ParamByName("FIRSTNAME")->AsString = Order->FirstName.SubString(1, 50);//paymentTransaction.Phoenix.FirstName.SubString(1, 50);
                    IBInternalQuery->ParamByName("LASTNAME")->AsString = Order->LastName.SubString(1, 50);//paymentTransaction.Phoenix.LastName.SubString(1, 50);
                    IBInternalQuery->ParamByName("TABLENUMBER")->AsInteger = tableNo;
                    IBInternalQuery->ParamByName("SEATNUMBER")->AsInteger = seatNo;
                    IBInternalQuery->ExecQuery();
                }
            }
        }
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}
//----------------------------------------------------------------------------------------
bool TDBSecurity::CheckWhetherGuestRecordExists(Database::TDBTransaction &dBTransaction, int securityRef, UnicodeString roomNumber,
                                int seatNo, UnicodeString accNo)
{
    bool isRecordExist = false;
    try
    {
        TIBSQL *IBInternalQuery = dBTransaction.Query(dBTransaction.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT a.SECURITYREF FROM PMSGUESTDETAILS a "
                                     "WHERE a.SECURITYREF = :SECURITYREF ";//AND A.ROOMNUMBER = :ROOMNUMBER "
                                     //   "AND A.SEATNUMBER = :SEATNUMBER AND A.ACCOUNTNUBER = :ACCOUNTNUBER ";
        IBInternalQuery->ParamByName("SECURITYREF")->AsInteger = securityRef;
//        IBInternalQuery->ParamByName("ROOMNUMBER")->AsString = roomNumber;
//        IBInternalQuery->ParamByName("SEATNUMBER")->AsInteger = seatNo;
//        IBInternalQuery->ParamByName("ACCOUNTNUBER")->AsString = accNo.SubString(1, 20);
        IBInternalQuery->ExecQuery();

        if(IBInternalQuery->RecordCount)
            isRecordExist = true;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
	}
    return isRecordExist;
}
