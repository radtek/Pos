//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerFloat.h"
#include "DeviceRealTerminal.h"
#include "MMLogging.h"
#include "MMTouchNumpad.h"
#include "MMMessageBox.h"
#include "DBSecurity.h"
#include "Message.h"
#include "Comms.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TManagerFloat::TManagerFloat(TForm * inOwner):Owner(inOwner)
{
}

void TManagerFloat::AlterFloat(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo)
{
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
   // Staff can set float once.
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
      "SELECT "
         "* FROM ZEDS "
      "WHERE "
         "TERMINAL_NAME = :TERMINAL_NAME AND "
         "TIME_STAMP IS NULL";
   IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
   IBInternalQuery->ExecQuery();
   if (IBInternalQuery->RecordCount)
   {
	  std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(Owner));
      frmTouchNumpad->Caption = "Enter the Deposit/Withdrawal amount";
      frmTouchNumpad->CURInitial = 0;
	  frmTouchNumpad->btnSurcharge->Caption = "Deposit";
	  frmTouchNumpad->btnDiscount->Caption = "Withdraw";
      frmTouchNumpad->btnDiscount->Visible = true;
	  frmTouchNumpad->btnSurcharge->Visible = true;
	  frmTouchNumpad->btnCancel->Visible = true;
	  frmTouchNumpad->Mode = pmCurrency;
      if (frmTouchNumpad->ShowModal() == mrOk)
	  {
         int CurrentSecurityRef     = 	IBInternalQuery->FieldByName("SECURITY_REF")->AsInteger;
		 Currency CurrentSkimsTotal   = 	IBInternalQuery->FieldByName("SKIMS_TOTAL")->AsCurrency;

		 TDBSecurity::ProcessSecurity(
						DBTransaction,
                        CurrentSecurityRef,
						UserInfo.ContactKey,
                        SecurityTypes[secFloatAdjust],FormatFloat("$0.00",CurrentSkimsTotal),
						FormatFloat("$0.00",CurrentSkimsTotal + frmTouchNumpad->CURResult),Now(),
                        TDeviceRealTerminal::Instance().ID.Name);

        if(frmTouchNumpad->CURResult > 0 && !TGlobalSettings::Instance().FloatWithdrawFromCash)
        {
             IBInternalQuery->Close();
             IBInternalQuery->SQL->Text =
                "UPDATE ZEDS "
                "SET "
                   "SKIMS_TOTAL	= :SKIMS_TOTAL "
                "WHERE "
                "TERMINAL_NAME = :TERMINAL_NAME AND "
                "TIME_STAMP IS NULL";
             IBInternalQuery->ParamByName("SKIMS_TOTAL")->AsCurrency = CurrentSkimsTotal + frmTouchNumpad->CURResult;
             IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
             IBInternalQuery->ExecQuery();
        }

		 int Zed_Key;
		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text =
			"SELECT "
			"Z_KEY FROM ZEDS "
			"WHERE "
			"TERMINAL_NAME = :TERMINAL_NAME AND "
			"TIME_STAMP IS NULL";

		 IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
		 IBInternalQuery->ExecQuery();

		 Zed_Key = IBInternalQuery->FieldByName("Z_KEY")->AsInteger;
		 FloatSkimTransaction temp;

		   if(frmTouchNumpad->CURResult < 0)
				temp = eSkim;
		   else if(frmTouchNumpad->CURResult == 0)
				temp = eNochange;
		   else
				temp = eRefloat;

			Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
			DBTransaction.StartTransaction();

		   if((double)frmTouchNumpad->CURResult)
		   {
				TFloatSkimData FloatSkimData(Owner, UserInfo, temp, frmTouchNumpad->CURResult, Zed_Key);
				FloatSkimData.runReason();
				FloatSkimData.InsertToDatabase(DBTransaction);
				DBTransaction.Commit();
				if (temp != eNochange)
					TComms::Instance().KickLocalDraw(DBTransaction);

				FloatSkimData.PrintDocket();
		   }
	  }
   }
   else
   {
	  MessageBox("You must Set the Float first.", "Error",	MB_OK + MB_ICONWARNING);
   }
}

void TManagerFloat::SetFloat(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo)
{
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
   // Staff can set float once.
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
	  "SELECT "
		 "* FROM ZEDS "
	  "WHERE "
		 "TERMINAL_NAME = :TERMINAL_NAME AND "
		 "TIME_STAMP IS NULL";
   IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
   IBInternalQuery->ExecQuery();

   std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(Owner));

   if (IBInternalQuery->RecordCount == 0)
   {

      frmTouchNumpad->Caption = "Enter the current float";
	  frmTouchNumpad->CURInitial = 0;
      frmTouchNumpad->btnSurcharge->Caption = "Ok";
	  frmTouchNumpad->btnDiscount->Visible = false;
      frmTouchNumpad->btnSurcharge->Visible = true;
      frmTouchNumpad->Mode = pmCurrency;
	  if (frmTouchNumpad->ShowModal() == mrOk)
      {
         int CurrentSecurityRef = TDBSecurity::GetNextSecurityRef(DBTransaction);
         TDBSecurity::ProcessSecurity(
                        DBTransaction,
                        CurrentSecurityRef,
                        UserInfo.ContactKey,
						SecurityTypes[secFloatSet],"0",
                        FormatFloat("$0.00",frmTouchNumpad->CURResult),Now(),
                        TDeviceRealTerminal::Instance().ID.Name);

         int key;
         IBInternalQuery->Close();
         IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ZED, 1) FROM RDB$DATABASE";
         IBInternalQuery->ExecQuery();
		 key = IBInternalQuery->Fields[0]->AsInteger;

         IBInternalQuery->Close();
         IBInternalQuery->SQL->Text =
            "INSERT INTO ZEDS ("
			   "Z_KEY,"
               "INITIAL_FLOAT,"
               "TERMINAL_NAME,"
               "SECURITY_REF) "
            "VALUES ("
               ":Z_KEY,"
               ":INITIAL_FLOAT,"
               ":TERMINAL_NAME,"
               ":SECURITY_REF);";
         IBInternalQuery->ParamByName("Z_KEY")->AsInteger		   = key;
         IBInternalQuery->ParamByName("INITIAL_FLOAT")->AsCurrency  = frmTouchNumpad->CURResult;
         IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString	= TDeviceRealTerminal::Instance().ID.Name;
         IBInternalQuery->ParamByName("SECURITY_REF")->AsInteger = CurrentSecurityRef;
         IBInternalQuery->ExecQuery();
      }
   }
   else
   {



	  IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
         "SELECT "
            "* FROM ZEDS "
         "WHERE "
            "TERMINAL_NAME = :TERMINAL_NAME AND "
            "TIME_STAMP IS NULL";
      IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
	  IBInternalQuery->ExecQuery();


	  if(IBInternalQuery->RecordCount)
	  {
		  MessageBox("The float has already been set, please Z-off to set the initial float again", "error", 0);
		  return;
      }


      int CurrentSecurityRef   = 	IBInternalQuery->FieldByName("SECURITY_REF")->AsInteger;
      Currency CurrentFloat      = 	IBInternalQuery->FieldByName("INITIAL_FLOAT")->AsCurrency;

	 // std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(Owner));
      frmTouchNumpad->Caption = "Re-enter the correct original float";
      frmTouchNumpad->CURInitial = CurrentFloat;
      frmTouchNumpad->btnSurcharge->Caption = "Ok";
      frmTouchNumpad->btnDiscount->Visible = false;
      frmTouchNumpad->btnSurcharge->Visible = true;
      frmTouchNumpad->Mode = pmCurrency;
      if (frmTouchNumpad->ShowModal() == mrOk)
	  {

         TDBSecurity::ProcessSecurity(
                        DBTransaction,
                        CurrentSecurityRef,
                        UserInfo.ContactKey,
                        SecurityTypes[secFloatAdjust],FormatFloat("$0.00",CurrentFloat),
                        FormatFloat("$0.00",frmTouchNumpad->CURResult),Now(),
						TDeviceRealTerminal::Instance().ID.Name);

         IBInternalQuery->Close();
         IBInternalQuery->SQL->Text =
            "UPDATE ZEDS "
            "SET "
               "INITIAL_FLOAT	= :INITIAL_FLOAT "
            "WHERE "
			"TERMINAL_NAME = :TERMINAL_NAME AND "
            "TIME_STAMP IS NULL";
         IBInternalQuery->ParamByName("INITIAL_FLOAT")->AsCurrency = frmTouchNumpad->CURResult;
         IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString	= TDeviceRealTerminal::Instance().ID.Name;
		 IBInternalQuery->ExecQuery();


      }
   }

		 int Zed_Key;
		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text =
			"SELECT "
			"Z_KEY FROM ZEDS "
			"WHERE "
			"TERMINAL_NAME = :TERMINAL_NAME AND "
			"TIME_STAMP IS NULL";

		 IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
		 IBInternalQuery->ExecQuery();

		 Zed_Key = IBInternalQuery->FieldByName("Z_KEY")->AsInteger;

		   TFloatSkimData FloatSkimData(Owner, UserInfo, eInitial, frmTouchNumpad->CURResult, Zed_Key);
			TComms::Instance().KickLocalDraw(DBTransaction);
	   //	   runReason(FloatSkimData);
		   FloatSkimData.InsertToDatabase(DBTransaction);
		   DBTransaction.Commit();
}











