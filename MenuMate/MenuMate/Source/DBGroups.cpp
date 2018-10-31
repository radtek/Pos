// ---------------------------------------------------------------------------

#pragma hdrstop

#include "DBGroups.h"
#include "MMLogging.h"
#include "GlobalSettings.h"

#include <map>
#include <memory>
#include <tinyxml.h>

// ---------------------------------------------------------------------------

#pragma package(smart_init)

int TDBGroups::SetContactGroup(Database::TDBTransaction &DBTransaction, int inContactKey, UnicodeString inGroupName)
{
	int RetVal = 0;
	try
	{
		int GroupKey = GetOrCreateGroup(DBTransaction, inGroupName);
		RetVal = SetContactGroup(DBTransaction, inContactKey, GroupKey);
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
	return RetVal;
}

int TDBGroups::SetContactGroup(Database::TDBTransaction &DBTransaction, int inContactKey, int inGroupKey)
{
	int RetVal = 0;

	try
	{
		if (!ContactGroupExists(DBTransaction, inContactKey, inGroupKey))
		{
			if (GroupExists(DBTransaction, inGroupKey))
			{
				TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
				IBInternalQuery->Close();
				IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_CONTACTGROUPS_KEY, 1) FROM RDB$DATABASE";
				IBInternalQuery->ExecQuery();
				RetVal = IBInternalQuery->Fields[0]->AsInteger;

				IBInternalQuery->Close();
				IBInternalQuery->ParamCheck = true;
				IBInternalQuery->SQL->Text =
					 "INSERT INTO CONTACTGROUPS (CONTACTGROUPS_KEY, CONTACTS_KEY, GROUPS_KEY, SUMMA_GROUP)" " VALUES (:CONTACTGROUPS_KEY, :CONTACTS_KEY,:GROUPS_KEY, 0);";
				IBInternalQuery->ParamByName("CONTACTGROUPS_KEY")->AsInteger = RetVal;
				IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
				IBInternalQuery->ParamByName("GROUPS_KEY")->AsInteger = inGroupKey;
				IBInternalQuery->ExecQuery();

			}
			else
			{
				throw Exception("Unable to Set Contact Group, Group Not Found");
			}
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}

	return RetVal;
}

bool TDBGroups::ContactGroupExists(Database::TDBTransaction &DBTransaction, int inContactKey, int inGroupKey)
{
	bool RetVal = false;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT CONTACTS_KEY FROM CONTACTGROUPS WHERE CONTACTS_KEY = :CONTACTS_KEY AND GROUPS_KEY = :GROUPS_KEY";
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
		IBInternalQuery->ParamByName("GROUPS_KEY")->AsInteger = inGroupKey;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			RetVal = true;
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
	return RetVal;
}

// -------------------------------------------------------------------------

void TDBGroups::GetContacts_GroupsKeys(Database::TDBTransaction &DBTransaction, int inContactKey, std::vector <ContactGroup> &GroupKeys)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->SQL->Text = "select GROUPS_KEY from CONTACTGROUPS WHERE CONTACTS_KEY = :CONTACTS_KEY";

	IBInternalQuery->Close();
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
	IBInternalQuery->ExecQuery();

	while (!IBInternalQuery->Eof)
	{
		ContactGroup GroupData;
		GroupData.Key = IBInternalQuery->FieldByName("GROUPS_KEY")->AsInteger;
		GroupKeys.push_back(GroupData);
		IBInternalQuery->Next();
	}
}

// --------------------------------------------------------------------------

void TDBGroups::GetContacts_ContactsKeys(Database::TDBTransaction &DBTransaction, int inGroupKey, std::vector <ContactGroup> &ContactKeys)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->SQL->Text = "select CONTACTS_KEY from CONTACTGROUPS WHERE GROUPS_KEY = :GROUPS_KEY";

	IBInternalQuery->Close();
	IBInternalQuery->ParamByName("GROUPS_KEY")->AsInteger = inGroupKey;
	IBInternalQuery->ExecQuery();

	while (!IBInternalQuery->Eof)
	{
		ContactGroup GroupData;
		GroupData.Key = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
		ContactKeys.push_back(GroupData);
		IBInternalQuery->Next();
	}
}

void TDBGroups::DeleteContactGroup(Database::TDBTransaction &DBTransaction, int GroupKey, int ContactKey)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->SQL->Text = "Delete from CONTACTGROUPS where GROUPS_KEY = :GROUPS_KEY and CONTACTS_KEY = :CONTACTS_KEY";
	IBInternalQuery->ParamByName("GROUPS_KEY")->AsInteger = GroupKey;
	IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactKey;
	IBInternalQuery->ExecQuery();
}

void TDBGroups::SetSummaGroup(Database::TDBTransaction &DBTransaction, int GroupKey, int ContactKey)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->SQL->Text = "UPDATE CONTACTGROUPS SET SUMMA_GROUP = 1 WHERE GROUPS_KEY = :GROUPS_KEY and CONTACTS_KEY = :CONTACTS_KEY";
	IBInternalQuery->ParamByName("GROUPS_KEY")->AsInteger = GroupKey;
	IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactKey;
	IBInternalQuery->ExecQuery();
}

bool TDBGroups::GetSummaGroup(Database::TDBTransaction &DBTransaction, int GroupKey, int ContactKey)
{
	bool RetVal = false;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
		 "SELECT SUMMA_GROUP FROM CONTACTGROUPS WHERE GROUPS_KEY = :GROUPS_KEY and CONTACTS_KEY = :CONTACTS_KEY and SUMMA_GROUP = :SUMMA_GROUP";
	IBInternalQuery->ParamByName("GROUPS_KEY")->AsInteger = GroupKey;
	IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactKey;
	IBInternalQuery->ParamByName("SUMMA_GROUP")->AsInteger = 1;
	IBInternalQuery->ExecQuery();
	if (IBInternalQuery->RecordCount)
	{
		RetVal = true;
	}
	return RetVal;
}

bool TDBGroups::SummaGroupExists(Database::TDBTransaction &DBTransaction, int ContactKey)
{
	bool RetVal = false;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "SELECT SUMMA_GROUP FROM CONTACTGROUPS WHERE CONTACTS_KEY = :CONTACTS_KEY and SUMMA_GROUP = :SUMMA_GROUP";
	IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactKey;
	IBInternalQuery->ParamByName("SUMMA_GROUP")->AsInteger = 1;
	IBInternalQuery->ExecQuery();

	if (IBInternalQuery->RecordCount)
	{
		RetVal = true;
	}

	return RetVal;
}

void TDBGroups::RemoveSummaGroup(Database::TDBTransaction &DBTransaction, int GroupKey, int ContactKey)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->SQL->Text = "UPDATE CONTACTGROUPS SET SUMMA_GROUP = 0 WHERE GROUPS_KEY = :GROUPS_KEY and CONTACTS_KEY = :CONTACTS_KEY";
	IBInternalQuery->ParamByName("GROUPS_KEY")->AsInteger = GroupKey;
	IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactKey;
	IBInternalQuery->ExecQuery();
}

void TDBGroups::RemoveSummaGroup(Database::TDBTransaction &DBTransaction, int ContactKey)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->SQL->Text =
		 "UPDATE CONTACTGROUPS SET SUMMA_GROUP = 0 WHERE SUMMA_GROUP = :SUMMA_GROUP and CONTACTS_KEY = :CONTACTS_KEY";
	IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactKey;
	IBInternalQuery->ParamByName("SUMMA_GROUP")->AsInteger = 1;
	IBInternalQuery->ExecQuery();
}

int TDBGroups::GetOrCreateGroup(Database::TDBTransaction &DBTransaction, UnicodeString inGroupName)
{
	return(GetOrCreateGroup(DBTransaction, inGroupName, 1));
}

int TDBGroups::GetOrCreateGroup(Database::TDBTransaction &DBTransaction, UnicodeString inGroupName, int inGroupNumber)
{
	int RetVal = FindGroup(DBTransaction, inGroupName);
	if (RetVal == 0)
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_GROUPS_KEY, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		RetVal = IBInternalQuery->Fields[0]->AsInteger;

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			 "INSERT INTO GROUPS (" "GROUPS_KEY," "NAME," "TYPE," "STATUS )" " VALUES (" ":GROUPS_KEY," ":NAME," ":TYPE," ":STATUS);";

		IBInternalQuery->ParamByName("GROUPS_KEY")->AsInteger = RetVal;
		IBInternalQuery->ParamByName("NAME")->AsString = inGroupName;
		IBInternalQuery->ParamByName("TYPE")->AsInteger = inGroupNumber;
		IBInternalQuery->ParamByName("STATUS")->AsInteger = 0;
		IBInternalQuery->ExecQuery();
	}
	return RetVal;
}


int TDBGroups::FindGroup(Database::TDBTransaction &DBTransaction, UnicodeString inGroupName)
{
	int RetVal = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GROUPS_KEY FROM GROUPS WHERE NAME = :NAME";
		IBInternalQuery->ParamByName("NAME")->AsString = inGroupName;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("GROUPS_KEY")->AsInteger;
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
	return RetVal;
}

bool TDBGroups::GroupExists(Database::TDBTransaction &DBTransaction, int inGroupKey)
{
	bool RetVal = false;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GROUPS_KEY FROM GROUPS WHERE GROUPS_KEY = :GROUPS_KEY";
		IBInternalQuery->ParamByName("GROUPS_KEY")->AsInteger = inGroupKey;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			RetVal = true;
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
	return RetVal;
}

bool TDBGroups::GroupExists(Database::TDBTransaction &DBTransaction, UnicodeString inGroupName)
{
	bool RetVal = false;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GROUPS_KEY FROM GROUPS WHERE NAME = :NAME";
		IBInternalQuery->ParamByName("NAME")->AsString = inGroupName;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			RetVal = true;
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
	return RetVal;
}
// -------------------------------------------------------------------------

void TDBGroups::GetGroup(Database::TDBTransaction &DBTransaction, ContactGroup &myData)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->SQL->Text = "select * from GROUPS where GROUPS_KEY = :GROUPS_KEY";
	IBInternalQuery->ParamByName("GROUPS_KEY")->AsInteger = myData.Key;
	IBInternalQuery->ExecQuery();

	myData.Name = IBInternalQuery->FieldByName("NAME")->AsString;
	myData.Type = IBInternalQuery->FieldByName("TYPE")->AsInteger;

	// return IBInternalQuery->FieldByName("NAME")->AsString;

}
// -------------------------------------------------------------------------

void TDBGroups::SetGroup(Database::TDBTransaction &DBTransaction, int &GroupKey, UnicodeString GroupName, int GroupType)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	if (GroupKey == 0)
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_GROUPS, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		GroupKey = IBInternalQuery->Fields[0]->AsInteger;

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			 "insert into \"GROUPS\" " "  (\"GROUPS\".\"GROUPS_KEY\", \"GROUPS\".\"NAME\", \"GROUPS\".\"TYPE\") " " values " " (:\"GROUPS_KEY\", :\"NAME\", :\"TYPE\") ";
		IBInternalQuery->ParamByName("GROUPS_KEY")->AsInteger = GroupKey;
		IBInternalQuery->ParamByName("NAME")->AsString = GroupName;
		IBInternalQuery->ParamByName("TYPE")->AsInteger = GroupType;
		IBInternalQuery->ExecQuery();
	}
	else
	{
		IBInternalQuery->SQL->Text =
			 " update \"GROUPS\" " " set " " \"GROUPS\".\"NAME\" = :\"NAME\", " " \"GROUPS\".\"TYPE\" = :\"TYPE\" "
			 "where " "  \"GROUPS\".\"GROUPS_KEY\" = :\"GROUPS_KEY\" ";
		IBInternalQuery->ParamByName("GROUPS_KEY")->AsInteger = GroupKey;
		IBInternalQuery->ParamByName("NAME")->AsString = GroupName;
		IBInternalQuery->ParamByName("TYPE")->AsInteger = GroupType;
		IBInternalQuery->ExecQuery();
	}
}
// -------------------------------------------------------------------------

void TDBGroups::GetGroupsList(Database::TDBTransaction &DBTransaction, TStrings *GroupName, TStrings *GroupType)
{

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->SQL->Text =
	"select * from GROUPS WHERE STATUS = :STATUS order by TYPE, NAME";
	IBInternalQuery->ParamByName("STATUS")->AsInteger = 0;
	IBInternalQuery->ExecQuery();

	for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	{
		GroupName->AddObject(UnicodeString(IBInternalQuery->FieldByName("NAME")->AsString),
			(TObject*)IBInternalQuery->FieldByName("GROUPS_KEY")->AsInteger);

		// GroupType[i] = IBInternalQuery->FieldByName("TYPE")->AsInteger;
		GroupType->AddObject(UnicodeString(IBInternalQuery->FieldByName("TYPE")->AsInteger),
			(TObject*)IBInternalQuery->FieldByName("GROUPS_KEY")->AsInteger);

	}
}
// -------------------------------------------------------------------------

void TDBGroups::DeleteGroup(Database::TDBTransaction &DBTransaction, int GroupKey)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->SQL->Text = " delete from \"GROUPS\" " " where " "  \"GROUPS\".\"GROUPS_KEY\" = :\"GROUPS_KEY\" ";
	IBInternalQuery->ParamByName("GROUPS_KEY")->AsInteger = GroupKey;
	IBInternalQuery->ExecQuery();
}
// -------------------------------------------------------------------------

std::vector <ContactGroup> TDBGroups::SortGroups(Database::TDBTransaction &DBTransaction, std::vector <ContactGroup> temp)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->SQL->Text = "select * from GROUPS order by TYPE, NAME";
	/* " select * from \"GROUPS\" "
	" order by "
	" \"TYPE\", \"NAME\" "; */
	IBInternalQuery->ExecQuery();

	for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	{
		ContactGroup GroupData;
		GroupData.Name = IBInternalQuery->FieldByName("NAME")->AsString;
		GroupData.Type = IBInternalQuery->FieldByName("TYPE")->AsInteger;
		GroupData.Key = IBInternalQuery->FieldByName("GROUPS_KEY")->AsInteger;
		temp.push_back(GroupData);
	}
	/*
	" delete from \"GROUPS\" "
	" where "
	"  \"GROUPS\".\"GROUPS_KEY\" = :\"GROUPS_KEY\" ";
	IBInternalQuery->ParamByName("GROUPS_KEY")->AsInteger = GroupKey;
	IBInternalQuery->ExecQuery();

	 */
}
// -------------------------------------------------------------------------

void TDBGroups::UpdateGroup(Database::TDBTransaction &DBTransaction, ContactGroup myData)
{

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->SQL->Text =
		 " update \"GROUPS\" " " set " " \"GROUPS\".\"NAME\" = :\"NAME\", " " \"GROUPS\".\"TYPE\" = :\"TYPE\" "
		 "where " "  \"GROUPS\".\"GROUPS_KEY\" = :\"GROUPS_KEY\" ";
	IBInternalQuery->ParamByName("GROUPS_KEY")->AsInteger = myData.Key;
	IBInternalQuery->ParamByName("NAME")->AsString = myData.Name;
	IBInternalQuery->ParamByName("TYPE")->AsInteger = myData.Type;
	IBInternalQuery->ExecQuery();

}
// -------------------------------------------------------------------------

void TDBGroups::GetGroupsList(Database::TDBTransaction &DBTransaction, std::vector <ContactGroup> &DataVector)
{
	// std::vector<ContactGroup> Datavector(10);

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->SQL->Text =
	"select * from GROUPS WHERE STATUS =:STATUS order by TYPE, NAME";
	IBInternalQuery->ParamByName("STATUS")->AsInteger = 0;
	IBInternalQuery->ExecQuery();

	for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	{
		ContactGroup GroupData;
		GroupData.Name = IBInternalQuery->FieldByName("NAME")->AsString;
		GroupData.Type = IBInternalQuery->FieldByName("TYPE")->AsInteger;
		GroupData.Key = IBInternalQuery->FieldByName("GROUPS_KEY")->AsInteger;
		DataVector.push_back(GroupData);
	}

	/*
	GroupName->AddObject(UnicodeString(IBInternalQuery->FieldByName("NAME")->AsString),
	(TObject *)IBInternalQuery->FieldByName("GROUPS_KEY")->AsInteger);

	//          GroupType[i] = IBInternalQuery->FieldByName("TYPE")->AsInteger;
	GroupType->AddObject(UnicodeString(IBInternalQuery->FieldByName("TYPE")->AsInteger),
	(TObject *)IBInternalQuery->FieldByName("GROUPS_KEY")->AsInteger);

	 */

}
// -------------------------------------------------------------------------

int TDBGroups::GetGroupType(Database::TDBTransaction &DBTransaction, UnicodeString inGroupName)
{

	int RetVal;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT TYPE FROM GROUPS WHERE NAME = :NAME";
		IBInternalQuery->ParamByName("NAME")->AsString = inGroupName;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("TYPE")->AsInteger;
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
	return RetVal;
}
// ------------------------------------------------------------------------

void TDBGroups::GetGroupsfromType(Database::TDBTransaction &DBTransaction, std::vector <ContactGroup> &DataVector, int inType)
{

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->SQL->Text = "select * from GROUPS where TYPE = :TYPE";
	IBInternalQuery->ParamByName("TYPE")->AsInteger = inType;
	IBInternalQuery->ExecQuery();

	for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	{
		ContactGroup GroupData;
		GroupData.Name = IBInternalQuery->FieldByName("NAME")->AsString;
		GroupData.Type = IBInternalQuery->FieldByName("TYPE")->AsInteger;
		GroupData.Key = IBInternalQuery->FieldByName("GROUPS_KEY")->AsInteger;
		DataVector.push_back(GroupData);
	}
}
void TDBGroups::CreateGroup(Database::TDBTransaction &DBTransaction, ContactGroup inGroup)
{
	int RetVal = FindGroup(DBTransaction, inGroup.Name);
	if (RetVal == 0)
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_GROUPS_KEY, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		RetVal = IBInternalQuery->Fields[0]->AsInteger;

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			 "INSERT INTO GROUPS (" "GROUPS_KEY," "NAME," "TYPE," "STATUS )" " VALUES (" ":GROUPS_KEY," ":NAME," ":TYPE," ":STATUS);";

		IBInternalQuery->ParamByName("GROUPS_KEY")->AsInteger = RetVal;
		IBInternalQuery->ParamByName("NAME")->AsString = inGroup.Name;
		IBInternalQuery->ParamByName("TYPE")->AsInteger = inGroup.Type;
		IBInternalQuery->ParamByName("STATUS")->AsInteger = 0;
		IBInternalQuery->ExecQuery();
	}
}





int TDBGroups::SetDefaultGroup(Database::TDBTransaction &DBTransaction)
{
		int Key;
	  Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_GROUPS_KEY, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		Key = IBInternalQuery->Fields[0]->AsInteger;

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			 "INSERT INTO GROUPS ("
		  "GROUPS_KEY,"
          "NAME,"
          "TYPE,"
          "STATUS )"
          " VALUES ("
			 ":GROUPS_KEY,"
          ":NAME,"
		  ":TYPE,"
		  ":STATUS);";

		IBInternalQuery->ParamByName("GROUPS_KEY")->AsInteger = Key;
		IBInternalQuery->ParamByName("NAME")->AsString = "No Contacts Group";
		IBInternalQuery->ParamByName("TYPE")->AsInteger = 0;
		IBInternalQuery->ParamByName("STATUS")->AsInteger = 1;
		IBInternalQuery->ExecQuery();
		return Key;

}
