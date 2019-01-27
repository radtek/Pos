//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerSyndCode.h"
#include "GlobalSettings.h"
#include <iterator>
#include "blowfish.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)


TSyndCode::TSyndCode(int inSyndCodeKey,  AnsiString inName, AnsiString inSyndCode, bool inEnabled,
                     bool inEncryptCode,AnsiString inOriginalSyndCode,bool inUseForCom ) :
      SyndCodeKey(inSyndCodeKey),
      Name(inName) ,
      DecryptedSyndCode(inSyndCode),
      Enabled(inEnabled),
      DefaultEncryptCode(inEncryptCode),
      OriginalSyndCode(inOriginalSyndCode),
      UseForCom(inUseForCom)
{
}

TSyndCode::TSyndCode()
{
   SyndCodeKey = 0;
   Enabled = false;
   DefaultEncryptCode = false;
   UseForCom = false;
}

bool TSyndCode::Valid()
{
   return (SyndCodeKey > 0) && (DecryptedSyndCode != "") && (OriginalSyndCode != "");
}

AnsiString TSyndCode::GetSyndCode()
{
   AnsiString retVal =  OriginalSyndCode;
   if(retVal == "" || retVal == NULL)
     retVal =  DecryptedSyndCode;
  return retVal;
}

TManagerSyndCode::TManagerSyndCode()
{
	CurrentKey = 0;
}

void TManagerSyndCode::Initialise(Database::TDBTransaction &DBTransaction)
{
   CurrentKey = 0;
   SyndCodes.clear();
   LoadCodes(DBTransaction);
   First();
}

void TManagerSyndCode::AddCode(Database::TDBTransaction &DBTransaction,TSyndCode inSyndCode)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_SYNDCODES, 1) FROM RDB$DATABASE";
   IBInternalQuery->ExecQuery();
   inSyndCode.SyndCodeKey = IBInternalQuery->Fields[0]->AsInteger;

   SyndCodes[inSyndCode.SyndCodeKey] = inSyndCode;
   UpdateEncryptCode(DBTransaction,inSyndCode);

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Clear();
   IBInternalQuery->SQL->Text =
      "INSERT INTO SYNDCODES ("
         "SYNDCODES_KEY, "
         "SYNDCODE, "
         "REPLACEMENTCODE, "
         "ENABLED, "
         "ENCRYPT_CODE, "
         "USE_FOR_LM_CLOUD, "
         "NAME) "
      "VALUES ( "
         ":SYNDCODES_KEY, "
         ":SYNDCODE, "
         ":REPLACEMENTCODE, "
         ":ENABLED, "
         ":ENCRYPT_CODE, "
         ":USE_FOR_LM_CLOUD, "
         ":NAME);";
   IBInternalQuery->ParamByName("SYNDCODES_KEY")->AsInteger	   = inSyndCode.SyndCodeKey;
   IBInternalQuery->ParamByName("SYNDCODE")->AsString		      = Encrypt(inSyndCode.OriginalSyndCode);
   IBInternalQuery->ParamByName("REPLACEMENTCODE")->AsString	= inSyndCode.OriginalSyndCode;
   IBInternalQuery->ParamByName("ENABLED")->AsString		      = inSyndCode.Enabled ? "T" : "F";
   IBInternalQuery->ParamByName("ENCRYPT_CODE")->AsString		= inSyndCode.DefaultEncryptCode ? "T" : "F";
   IBInternalQuery->ParamByName("NAME")->AsString		         = inSyndCode.Name;
   IBInternalQuery->ParamByName("USE_FOR_LM_CLOUD")->AsString		= inSyndCode.UseForCom ? "T" : "F";
   IBInternalQuery->ExecQuery();
}

void TManagerSyndCode::UpdateCode(Database::TDBTransaction &DBTransaction,TSyndCode inSyndCode)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   SyndCodes[inSyndCode.SyndCodeKey] = inSyndCode;
   UpdateEncryptCode(DBTransaction,inSyndCode);
   SyndCodes[inSyndCode.SyndCodeKey].UseForCom = inSyndCode.UseForCom;
   IBInternalQuery->SQL->Clear();
   IBInternalQuery->SQL->Text =
      "UPDATE SYNDCODES SET "
         "NAME             = :NAME, "
         "SYNDCODE         = :SYNDCODE, "
         "ENCRYPT_CODE     = :ENCRYPT_CODE, "
         "ENABLED          = :ENABLED, "
         "USE_FOR_LM_CLOUD = :USE_FOR_LM_CLOUD, "
         "REPLACEMENTCODE  = :REPLACEMENTCODE "
         " WHERE SYNDCODES_KEY = :SYNDCODES_KEY";
   IBInternalQuery->ParamByName("SYNDCODES_KEY")->AsInteger    = inSyndCode.SyndCodeKey;
   IBInternalQuery->ParamByName("NAME")->AsString	            = inSyndCode.Name;
   IBInternalQuery->ParamByName("SYNDCODE")->AsString		      = Encrypt(inSyndCode.OriginalSyndCode);
   IBInternalQuery->ParamByName("ENCRYPT_CODE")->AsString      = inSyndCode.DefaultEncryptCode ? "T" : "F";
   IBInternalQuery->ParamByName("ENABLED")->AsString		      = inSyndCode.Enabled ? "T" : "F";
   IBInternalQuery->ParamByName("REPLACEMENTCODE")->AsString	= inSyndCode.OriginalSyndCode;
   IBInternalQuery->ParamByName("USE_FOR_LM_CLOUD")->AsString		= inSyndCode.UseForCom ? "T" : "F";
   IBInternalQuery->ExecQuery();
}

void TManagerSyndCode::UpdateEncryptCode(Database::TDBTransaction &DBTransaction,TSyndCode inSyndCode)
{
   if(SyndCodes[inSyndCode.SyndCodeKey].DefaultEncryptCode != inSyndCode.DefaultEncryptCode)
   { // Default Code has changed.

      // Fix up and items in DB that's ENCRYPT_CODE may be set that need unsetting.
      if(inSyndCode.DefaultEncryptCode)
      {
	      TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

         IBInternalQuery->Close();
         IBInternalQuery->SQL->Text =
            "UPDATE SYNDCODES "
            "SET "
               "ENCRYPT_CODE = :ENCRYPT_CODE "
            "WHERE "
               "SYNDCODES_KEY	!= :SYNDCODES_KEY";
         IBInternalQuery->ParamByName("SYNDCODES_KEY")->AsInteger	= inSyndCode.SyndCodeKey;
         IBInternalQuery->ParamByName("ENCRYPT_CODE")->AsString	= "F";
         IBInternalQuery->ExecQuery();

         // Fix up CACHE of Keys.
         for (First();!Eof() ;Next())
         {
            if(SyndCode().SyndCodeKey != inSyndCode.SyndCodeKey)
            {
               SyndCode().DefaultEncryptCode = false;
            }
         }
      } // It was True but now its false, so we no longer have an encrypt Code.
      //Figure out how to let the other tills know to update their settings / load on use?
   }
}

AnsiString TManagerSyndCode::Encrypt(AnsiString Data)
{
   AnsiString RetVal(Data.c_str());
   // Space must be a multiple of 8 for Blowfish encryption.
   if(RetVal.Length() % 8 != 0)
   {
      int BlockSize = 0;
      if( (RetVal.Length() - (RetVal.Length() % 8)) %8 == 0 )
      {
         BlockSize = 8 - (RetVal.Length() % 8);
      }
      else
      {
         BlockSize = (RetVal.Length() % 8);
      }

      AnsiString ptrBuffer = AnsiString::StringOfChar(0x00, BlockSize);
      RetVal = RetVal + ptrBuffer;
   }

   CBlowFish BF;
   // Have to pass though the NULL on the end of the string because this old format
   // has it inherently ( yea you cant see it but its there ).
   BYTE Passwd[] = {"InTheBeginningGodCreatedTheHeavensAndTheEarthMenuMateCameLater"};
   BF.Initialize(Passwd,sizeof(Passwd));
   BF.Encode((BYTE *)Data.c_str(),(BYTE *)RetVal.c_str(),RetVal.Length());
   return RetVal;
}

AnsiString TManagerSyndCode::Decrypt(AnsiString Data)
{
   AnsiString RetVal(Data.c_str());
   CBlowFish BF;
   // Have to pass though the NULL on the end of the string because this old format
   // has it inherently ( yea you cant see it but its there ).
   BYTE Passwd[] = {"InTheBeginningGodCreatedTheHeavensAndTheEarthMenuMateCameLater"};
   BF.Initialize(Passwd,sizeof(Passwd));
   BF.Decode((BYTE *)Data.c_str(),(BYTE *)RetVal.c_str(),Data.Length());
   return RetVal.Trim();
}

void TManagerSyndCode::LoadCodes(Database::TDBTransaction &DBTransaction)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "Select * From SYNDCODES;";
	IBInternalQuery->ExecQuery();

	while(!IBInternalQuery->Eof)
	{
      TSyndCode inSyndCode(IBInternalQuery->FieldByName("SYNDCODES_KEY")->AsInteger,
                         IBInternalQuery->FieldByName("NAME")->AsString,
                         Decrypt(AnsiString(IBInternalQuery->FieldByName("SYNDCODE")->AsString)),
                         IBInternalQuery->FieldByName("ENABLED")->AsString == "T",
                         IBInternalQuery->FieldByName("ENCRYPT_CODE")->AsString == "T",
                         IBInternalQuery->FieldByName("REPLACEMENTCODE")->AsString,
                         IBInternalQuery->FieldByName("USE_FOR_LM_CLOUD")->AsString == "T"
                         );
      SyndCodes[inSyndCode.SyndCodeKey] = inSyndCode;
	  IBInternalQuery->Next();
	}
}

void TManagerSyndCode::RemoveCode(Database::TDBTransaction &DBTransaction,int ItemKey)
{
   SyndCodes.erase(ItemKey);
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
   IBInternalQuery->SQL->Clear();
   IBInternalQuery->SQL->Text =
      "DELETE FROM SYNDCODES WHERE SYNDCODES_KEY = :SYNDCODES_KEY";
   IBInternalQuery->ParamByName("SYNDCODES_KEY")->AsInteger	= ItemKey;
   IBInternalQuery->ExecQuery();
}

bool TManagerSyndCode::ValidateSyndCodes(AnsiString& errorMessage)
{
   if(!TGlobalSettings::Instance().LoyaltyMateEnabled)
     return true;
   int comSyndCount = 0;
   for (First(false);!Eof();Next(false))
   {
      TSyndCode CurrentSyndCode = SyndCode();
      if(CurrentSyndCode.UseForCom)
      {
         comSyndCount++;
      }
   }
   if(comSyndCount == 1)
   {
      return true;
   }
   else if(comSyndCount == 0)
   {
      errorMessage = "There is no Syndicate Code configured for communication with Menumate Cloud. Please configure it or disable Loyaltymate if you do not wish to configure it.";
      return false;
   }
   else
   {
      errorMessage = "There are multiple Syndicate Code configured. Please setup a Syndicate Code for communication with Menumate Cloud. It should be the one configured for your company at Menumate Cloud.";;
      return false;
   }
}

bool TManagerSyndCode::CanUseForCommunication(int syndCodeKey)
{
   if(!TGlobalSettings::Instance().LoyaltyMateEnabled)
     return true;

   bool retVal = true;
   for (First(false);!Eof();Next(false))
   {
      TSyndCode CurrentSyndCode = SyndCode();
      if(CurrentSyndCode.UseForCom && CurrentSyndCode.SyndCodeKey != syndCodeKey)
      {
         retVal = false;
         break;
      }
   }
   return retVal;
}


void TManagerSyndCode::First(bool EnabledCodesOnly)
{
   ptrSyndCodes = SyndCodes.begin();
   //Add code to First, Next, etc to step around Disabled Code is EnabledCodesOnly is set.
   if(EnabledCodesOnly)
   {
      if(!ptrSyndCodes->second.Enabled)
      {
         Next();
      }
      else
      {
         CurrentKey = ptrSyndCodes->first;
      }
   }
   else
   {
    	CurrentKey = ptrSyndCodes->first;
   }
}

void TManagerSyndCode::Next(bool EnabledCodesOnly)
{
   advance(ptrSyndCodes,1);
   if(ptrSyndCodes != SyndCodes.end())
   {
      if(EnabledCodesOnly)
      {
         do
         {
            if(!ptrSyndCodes->second.Enabled)
            {
               advance(ptrSyndCodes,1);
            }
         }
         while(ptrSyndCodes != SyndCodes.end() && !ptrSyndCodes->second.Enabled);
         if(ptrSyndCodes != SyndCodes.end())
         {
            CurrentKey = ptrSyndCodes->first;
         }
      }
      else
      {
         CurrentKey = ptrSyndCodes->first;
      }
   }
}

bool TManagerSyndCode::Eof()
{
	return  (ptrSyndCodes == SyndCodes.end());
}

bool TManagerSyndCode::Bof()
{
	return  ptrSyndCodes == SyndCodes.begin();
}

TSyndCode &TManagerSyndCode::SyndCode()
{
   return SyndCodes[CurrentKey];
}

TSyndCode &TManagerSyndCode::SyndCodeByKey(int SyndKey)
{
   return SyndCodes[SyndKey];
}

bool TManagerSyndCode::Empty()
{
   return SyndCodes.empty();
}

int TManagerSyndCode::Size()
{
   return SyndCodes.size();
}

TSyndCode TManagerSyndCode::GetDefaultSyndCode()
{
   TSyndCode retval;
   for (First(false);!Eof();Next(false))
   {
      TSyndCode CurrentSyndCode = SyndCode();
      if(CurrentSyndCode.DefaultEncryptCode)
      {
         retval = CurrentSyndCode;
      }
   }
   return retval;
}

TSyndCode TManagerSyndCode::GetCommunicationSyndCode()
{
   TSyndCode retval;
   for (First(false);!Eof();Next(false))
   {
      TSyndCode CurrentSyndCode = SyndCode();
      if(CurrentSyndCode.UseForCom)
      {
         retval = CurrentSyndCode;
         break;
      }
   }
   return retval;
}

AnsiString TManagerSyndCode::GetCommunicationSyndCodeString()
{
   AnsiString retval = "";
   for (First(false);!Eof();Next(false))
   {
      TSyndCode CurrentSyndCode = SyndCode();
      if(CurrentSyndCode.UseForCom)
      {
         retval = CurrentSyndCode.OriginalSyndCode;
         break;
      }
   }
   return retval;
}

std::map <int, TSyndCode> TManagerSyndCode::GetSyndCodes(bool EnabledCodesOnly)
{
	std::map <int, TSyndCode> Codes;
   for (First(EnabledCodesOnly);!Eof();Next(EnabledCodesOnly))
   {
      TSyndCode CurrentSyndCode = SyndCode();
   	  Codes[CurrentSyndCode.SyndCodeKey] = CurrentSyndCode;
   }
   return Codes;
}

void TManagerSyndCode::ImportFromFile(Database::TDBTransaction &DBTransaction,int Key ,AnsiString FileName)
{
	std::auto_ptr<TMemoryStream> SyndStream(new TMemoryStream);
	SyndStream->LoadFromFile(FileName);
	SyndStream->Position = 0;
    AnsiString Data((char *)SyndStream->Memory,SyndStream->Size);

    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "UPDATE SYNDCODES SET SYNDCODE = :SYNDCODE WHERE SYNDCODES_KEY	= :SYNDCODES_KEY";
    IBInternalQuery->ParamByName("SYNDCODES_KEY")->AsInteger = Key;
    IBInternalQuery->ParamByName("SYNDCODE")->AsString = Data;
    IBInternalQuery->ExecQuery();
}

void TManagerSyndCode::ExportToFile(Database::TDBTransaction &DBTransaction,int Key ,AnsiString FileName)
{
	std::auto_ptr<TMemoryStream> SyndStream(new TMemoryStream);
	SyndStream->Position = 0;

    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "SELECT SYNDCODE FROM SYNDCODES WHERE SYNDCODES_KEY = :SYNDCODES_KEY";
    IBInternalQuery->ParamByName("SYNDCODES_KEY")->AsInteger = Key;
    IBInternalQuery->ExecQuery();
    AnsiString Data = IBInternalQuery->FieldByName("SYNDCODE")->AsString;
    SyndStream->Write(Data.c_str(),Data.Length());
	SyndStream->Position = 0;
    SyndStream->SaveToFile(FileName);
}
//----------------------------------------------------------------------
bool TManagerSyndCode::CheckIfSynCodeEnabled(int syndCodeKey)
{
   bool retVal = false;
   for (First(false);!Eof();Next(false))
   {
      TSyndCode CurrentSyndCode = SyndCode();
      if(CurrentSyndCode.SyndCodeKey == syndCodeKey && CurrentSyndCode.Enabled == true)
      {
         retVal = true;
         break;
      }
   }
   return retVal;
}
//----------------------------------------------------------------------
bool TManagerSyndCode::CheckIfAnySynCodeEnabled()
{
   bool retVal = true;
   for (First(false);!Eof();Next(false))
   {
      TSyndCode CurrentSyndCode = SyndCode();
      if(CurrentSyndCode.Enabled == true)
      {
         retVal = false;
         break;
      }
   }
   return retVal;
}

