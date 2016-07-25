//---------------------------------------------------------------------------
#pragma hdrstop
#include "ManagerPatron.h"
#include "MMLogging.h"
//---------------------------------------------------------------------------
#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef PalmMate
#include "Palm.h"
#endif

#pragma package(smart_init)

void TManagerPatron::GetPatronTypes(Database::TDBTransaction &DBTransaction, TStringList *List)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" SELECT PATRONTYPES_KEY,PATRON_TYPE,IS_DEFAULT"
	" FROM"
	"  PATRONTYPES"
	" ORDER BY PATRON_TYPE";
	IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount && List != NULL)
	{
		for (;!IBInternalQuery->Eof; IBInternalQuery->Next())
		{
			if(IBInternalQuery->FieldByName("IS_DEFAULT")->AsString.UpperCase() == "F" ? false : true)
			{
				 List->AddObject(IBInternalQuery->FieldByName("PATRON_TYPE")->AsString.Unique() + "(Default)",(TObject *)IBInternalQuery->FieldByName("PATRONTYPES_KEY")->AsInteger);
			}
			else
			{
				 List->AddObject(IBInternalQuery->FieldByName("PATRON_TYPE")->AsString.Unique(),(TObject *)IBInternalQuery->FieldByName("PATRONTYPES_KEY")->AsInteger);
			}
		}
	}
}

void TManagerPatron::BuildXMLListPatronCounts(Database::TDBTransaction &DBTransaction,TPOS_XMLBase &Data)
{
   try
	{
      // Update the IntaMate ID with the Invoice Number.
      Data.Doc.Clear();
      TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "UTF-8", "" );
      Data.Doc.LinkEndChild( decl );

      // Insert DOCTYPE definiation here.
      TiXmlElement * List = new TiXmlElement( xmlEleListPatron );
      List->SetAttribute(xmlAttrID, Data.IntaMateID.t_str());
      List->SetAttribute(xmlAttrSiteID, TGlobalSettings::Instance().SiteID);

      std::vector<TPatronType> Patrons;
      GetPatronTypes(DBTransaction, Patrons);

		for (std::vector<TPatronType>::iterator ptrPatron = Patrons.begin();
			  ptrPatron != Patrons.end();
			  ptrPatron++)
		{
         TiXmlElement *ElePatron = new TiXmlElement( xmlElePatronType );
         ElePatron->SetAttribute(xmlAttrID,         ptrPatron->Name.t_str() );
         ElePatron->SetAttribute(xmlAttrName,       ptrPatron->Name.t_str() );
         ElePatron->SetAttribute(xmlAttrDefault,    ptrPatron->Default ? "yes" : "no" );
         List->LinkEndChild( ElePatron );
      }
      Data.Doc.LinkEndChild( List );
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TManagerPatron::GetPatronTypes(Database::TDBTransaction &DBTransaction, std::vector<TPatronType> &PatronTypes)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" SELECT PATRONTYPES_KEY,PATRON_TYPE,IS_DEFAULT"
	" FROM"
	"  PATRONTYPES"
	" ORDER BY PATRON_TYPE";
	IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount)
	{
		for (;!IBInternalQuery->Eof; IBInternalQuery->Next())
		{
			TPatronType PatronType;
			PatronType.Name = IBInternalQuery->FieldByName("PATRON_TYPE")->AsString;
			PatronType.Default = IBInternalQuery->FieldByName("IS_DEFAULT")->AsString.UpperCase() == "F" ? false : true;
            PatronType.Count= 0;
			PatronTypes.push_back(PatronType);
		}
	}
}

int TManagerPatron::GetCount(Database::TDBTransaction &DBTransaction)
{
	int RetVal = 0;
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" SELECT COUNT(PATRONTYPES_KEY)"
	" FROM"
	"  PATRONTYPES";
	IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount)
	{
		RetVal = IBInternalQuery->FieldByName("COUNT")->AsInteger;
	}
	return RetVal;
}

TPatronType TManagerPatron::GetPatronTypes(Database::TDBTransaction &DBTransaction, int Key)
{
	TPatronType PatronType;
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" SELECT PATRONTYPES_KEY,PATRON_TYPE,IS_DEFAULT"
	" FROM"
	"  PATRONTYPES"
	" WHERE "
	" PATRONTYPES_KEY = :PATRONTYPES_KEY"
	" ORDER BY PATRON_TYPE";
	IBInternalQuery->ParamByName("PATRONTYPES_KEY")->AsInteger = Key;
	IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount)
	{
			PatronType.Name = IBInternalQuery->FieldByName("PATRON_TYPE")->AsString;
			PatronType.Default = IBInternalQuery->FieldByName("IS_DEFAULT")->AsString.UpperCase() == "F" ? false : true;
	}
	
	return PatronType;
}


void TManagerPatron::GetPatronTypes(Database::TDBTransaction &DBTransaction,TStringGrid * StringGrid)
{
	if(GetCount(DBTransaction) < 2)
	{
		StringGrid->RowCount = 2;
	}
	else
	{
		StringGrid->RowCount = GetCount(DBTransaction) + 1;
	}
	StringGrid->Cols[0]->Clear();
	StringGrid->Cols[1]->Clear();

	StringGrid->Cols[0]->Add("Patron Type");
	StringGrid->Cols[1]->Add("Default");

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" SELECT PATRONTYPES_KEY,PATRON_TYPE,IS_DEFAULT"
	" FROM"
	"  PATRONTYPES"
	" ORDER BY PATRON_TYPE";
	IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount)
	{
		for (;!IBInternalQuery->Eof; IBInternalQuery->Next())
		{
			if(IBInternalQuery->FieldByName("IS_DEFAULT")->AsString.UpperCase() == "F" ? false : true)
			{
				 StringGrid->Cols[0]->AddObject(UnicodeString(IBInternalQuery->FieldByName("PATRON_TYPE")->AsString),(TObject *)IBInternalQuery->FieldByName("PATRONTYPES_KEY")->AsInteger);
				 StringGrid->Cols[1]->AddObject("(Default)",(TObject *)IBInternalQuery->FieldByName("PATRONTYPES_KEY")->AsInteger);
			}
			else
			{
				 StringGrid->Cols[0]->AddObject(UnicodeString(IBInternalQuery->FieldByName("PATRON_TYPE")->AsString),(TObject *)IBInternalQuery->FieldByName("PATRONTYPES_KEY")->AsInteger);
				 StringGrid->Cols[1]->AddObject("",(TObject *)IBInternalQuery->FieldByName("PATRONTYPES_KEY")->AsInteger);
			}
		}
	}
	StringGrid->FixedRows = 1;
}

void TManagerPatron::SetPatronType(Database::TDBTransaction &DBTransaction,long Key,TPatronType PatronType)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	if(PatronType.Default)
	{
		ClearDefault(DBTransaction);
	}

	if(Key == 0)
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_PATRONTYPES, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		Key = IBInternalQuery->Fields[0]->AsInteger;

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"INSERT INTO PATRONTYPES ("
			"PATRONTYPES_KEY,"
			"PATRON_TYPE,"
			"IS_DEFAULT) "
		"VALUES ("
			":PATRONTYPES_KEY,"
			":PATRON_TYPE,"
			":IS_DEFAULT);";
		IBInternalQuery->ParamByName("PATRONTYPES_KEY")->AsInteger = Key;
		IBInternalQuery->ParamByName("PATRON_TYPE")->AsString = PatronType.Name;
		IBInternalQuery->ParamByName("IS_DEFAULT")->AsString = PatronType.Default ? "T" : "F";
		IBInternalQuery->ExecQuery();
	}
	else
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE "
		" PATRONTYPES"
		" SET"
		" PATRON_TYPE = :PATRON_TYPE,"
		" IS_DEFAULT = :IS_DEFAULT"
		" WHERE"
		" PATRONTYPES_KEY = :PATRONTYPES_KEY";
		IBInternalQuery->ParamByName("PATRONTYPES_KEY")->AsInteger = Key;
		IBInternalQuery->ParamByName("PATRON_TYPE")->AsString = PatronType.Name;
		IBInternalQuery->ParamByName("IS_DEFAULT")->AsString = PatronType.Default ? "T" : "F";
		IBInternalQuery->ExecQuery();
	}
}

void TManagerPatron::Delete(Database::TDBTransaction &DBTransaction,int Key)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	if(Key != 0)
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"DELETE FROM PATRONTYPES"
		" WHERE PATRONTYPES_KEY = :PATRONTYPES_KEY";
		IBInternalQuery->ParamByName("PATRONTYPES_KEY")->AsInteger = Key;
		IBInternalQuery->ExecQuery();
	}
}

void TManagerPatron::ClearDefault(Database::TDBTransaction &DBTransaction)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	"UPDATE "
	" PATRONTYPES"
	" SET"
	" IS_DEFAULT = :IS_DEFAULT";
	IBInternalQuery->ParamByName("IS_DEFAULT")->AsString = "F";
	IBInternalQuery->ExecQuery();
}

void TManagerPatron::SetDefaultPatrons(Database::TDBTransaction &DBTransaction, std::vector<TPatronType> &PatronTypes, int Count)
{
	TPatronType PatronType = GetDefaultPatron(DBTransaction);
	if(PatronType.Name != "")
	{
		if(PatronTypes.size() == 0)
		{
			PatronType.Count = Count;
			PatronTypes.push_back(PatronType);
		}
		else
		{
			std::vector<TPatronType>::iterator ptrPatronTypes = PatronTypes.begin();
			for (ptrPatronTypes = PatronTypes.begin();
				  ptrPatronTypes != PatronTypes.end() ; ptrPatronTypes++)
			{
				if(ptrPatronTypes->Name == PatronType.Name)
				{
					ptrPatronTypes->Count = Count;
				}
			}
		}
	}
}

int TManagerPatron::GetTotalPatrons(std::vector<TPatronType> &PatronTypes)
{
	int TotalCount = 0;
	std::vector<TPatronType>::iterator ptrPatronTypes;
	for (ptrPatronTypes = PatronTypes.begin();
		  ptrPatronTypes != PatronTypes.end() ; ptrPatronTypes++)
	{
		TotalCount += ptrPatronTypes->Count;
	}
	return TotalCount;
}


TPatronType TManagerPatron::GetDefaultPatron(Database::TDBTransaction &DBTransaction)
{
	TPatronType RetVal;
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" SELECT PATRONTYPES_KEY,PATRON_TYPE,IS_DEFAULT"
	" FROM"
	"  PATRONTYPES"
	" WHERE IS_DEFAULT = :IS_DEFAULT"
	" ORDER BY PATRON_TYPE";
	IBInternalQuery->ParamByName("IS_DEFAULT")->AsString = "T";
	IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount)
	{
		RetVal.Name = IBInternalQuery->FieldByName("PATRON_TYPE")->AsString;
		RetVal.Default = IBInternalQuery->FieldByName("IS_DEFAULT")->AsString.UpperCase() == "F" ? false : true;
	}

	return RetVal;
}
