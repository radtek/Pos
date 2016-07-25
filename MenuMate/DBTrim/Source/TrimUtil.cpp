//---------------------------------------------------------------------------


#pragma hdrstop

#include "TrimUtil.h"
#include "MMRegistry.h"
#include <Forms.hpp>

#define MMBaseKey "\\Software\\IQWorks\\MenuMate\\"
//---------------------------------------------------------------------------

#pragma package(smart_init)


TDBTrimUtil::TDBTrimUtil() : TThread(true)
{
	FreeOnTerminate	 = false;
	Priority           = tpLowest;
   TrimDate           = 0;
   MaxCount           = 1;
   Position           = 0;
}

void TDBTrimUtil::Start(TDateTime inTrimDate)
{
   TrimDate = inTrimDate;
	Resume();
}

void TDBTrimUtil::Cancel()
{
	Terminate();
}

void __fastcall TDBTrimUtil::Execute()
{
   try
   {
      // Connect to the DB
		UnicodeString DatabasePath = "";
      RegistryRead(MMBaseKey "Database","DatabasePath",DatabasePath);

      UnicodeString InterbaseIP = "";
      RegistryRead(MMBaseKey "Database","InterbaseIP",InterbaseIP);

      DBControl.Disconnect();
      DBControl.Init(Database::TDBSettings(InterbaseIP,DatabasePath,false));
      DBControl.Connect();

      // Select the Archives to be trashed.
      Database::TDBTransaction DBTransaction(DBControl);
      DBTransaction.StartTransaction();
      TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text = "SELECT COUNT(ARCBILL_KEY) FROM ARCBILL WHERE TIME_STAMP < :TIME_STAMP";
      IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = TrimDate;
      IBInternalQuery->ExecQuery();
      MaxCount = IBInternalQuery->FieldByName("COUNT")->AsInteger;
      Position = 0;

  		PostMessage(Application->Handle, STATUSCHANGED, (UINT)0, STATUSBEGIN);
      
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text = "SELECT ARCBILL_KEY,SECURITY_REF FROM ARCBILL WHERE TIME_STAMP < :TIME_STAMP";
      IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = TrimDate;
      IBInternalQuery->ExecQuery();

      Database::TDBTransaction DeletetionTransaction(DBControl);
      DeletetionTransaction.StartTransaction();
      TIBSQL *IBQuery1 = DeletetionTransaction.Query(DeletetionTransaction.AddQuery());
      TIBSQL *IBQuery2 = DeletetionTransaction.Query(DeletetionTransaction.AddQuery());
      TIBSQL *IBQuery3 = DeletetionTransaction.Query(DeletetionTransaction.AddQuery());
      TIBSQL *IBQuery4 = DeletetionTransaction.Query(DeletetionTransaction.AddQuery());

      // Remove the Zeds.
      IBQuery3->Close();
      IBQuery3->SQL->Text = "DELETE FROM ZEDS WHERE TIME_STAMP < :TIME_STAMP";
      IBQuery3->ParamByName("TIME_STAMP")->AsDateTime = TrimDate;
      IBQuery3->ExecQuery();

      // Remove the Zeds.
      IBQuery3->Close();
      IBQuery3->SQL->Text = "DELETE FROM KITCHENDOCKETS WHERE TIME_STAMP < :TIME_STAMP";
      IBQuery3->ParamByName("TIME_STAMP")->AsDateTime = TrimDate;
      IBQuery3->ExecQuery();

      for (int i = 0; !IBInternalQuery->Eof && !Terminated ;IBInternalQuery->Next(), i++)
      {
         // Remove the Security Records assiciated with the Bill.
         IBQuery1->Close();
         IBQuery1->SQL->Text = "DELETE FROM SECURITY WHERE SECURITY_REF = :SECURITY_REF";
         IBQuery1->ParamByName("SECURITY_REF")->AsInteger = IBInternalQuery->FieldByName("SECURITY_REF")->AsInteger;
         IBQuery1->ExecQuery();

         // Remove the security record associated with the orders.
         IBQuery2->Close();
         IBQuery2->SQL->Text = "SELECT SECURITY_REF FROM ARCHIVE WHERE ARCBILL_KEY = :ARCBILL_KEY";
         IBQuery2->ParamByName("ARCBILL_KEY")->AsInteger = IBInternalQuery->FieldByName("ARCBILL_KEY")->AsInteger;
         IBQuery2->ExecQuery();
         for (; !IBQuery2->Eof ;IBQuery2->Next())
         {
            IBQuery1->Close();
            IBQuery1->ParamByName("SECURITY_REF")->AsInteger = IBQuery2->FieldByName("SECURITY_REF")->AsInteger;
            IBQuery1->ExecQuery();
         }

         // Remove the Bill.
         IBQuery3->Close();
         IBQuery3->SQL->Text = "DELETE FROM ARCBILL WHERE ARCBILL_KEY = :ARCBILL_KEY";
         IBQuery3->ParamByName("ARCBILL_KEY")->AsInteger = IBInternalQuery->FieldByName("ARCBILL_KEY")->AsInteger;
         IBQuery3->ExecQuery();

         if(i > 1000)
         {
            DeletetionTransaction.Commit();
            DeletetionTransaction.StartTransaction();
            i = 0;
         }

         Position++;
      }

      DeletetionTransaction.Commit();
      DBTransaction.Commit();
   }
   catch(Exception &E)
   {
      LastError = E.Message;
   	PostMessage(Application->Handle, STATUSCHANGED, (UINT)0, STATUSERROR);      
   }
	PostMessage(Application->Handle, STATUSCHANGED, (UINT)0, STATUSCOMPLETE);
}
