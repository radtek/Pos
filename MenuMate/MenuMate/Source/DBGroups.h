// ---------------------------------------------------------------------------

#ifndef DBGroupsH
#define DBGroupsH
// ---------------------------------------------------------------------------
#include "MM_DBCore.h"
#include "ContactGroup.h"
#include "POS_XMLBase.h"
#include <vector>

class TDBGroups
{
public:

	static int SetContactGroup(Database::TDBTransaction &DBTransaction, int inContactKey, UnicodeString inGroupName);
	static int SetContactGroup(Database::TDBTransaction &DBTransaction, int inContactKey, int inGroupKey);
	static bool ContactGroupExists(Database::TDBTransaction &DBTransaction, int inContactKey, int inGroupKey);
	static void GetContacts_GroupsKeys(Database::TDBTransaction &DBTransaction, int inContactKey, std::vector <ContactGroup> &GroupKeys);
	static void DeleteContactGroup(Database::TDBTransaction &DBTransaction, int GroupKey, int ContactKey);
	static void GetContacts_ContactsKeys(Database::TDBTransaction &DBTransaction, int inGroupKey, std::vector <ContactGroup> &ContactKeys);
	static void SetSummaGroup(Database::TDBTransaction &DBTransaction, int GroupKey, int ContactKey);
	static bool GetSummaGroup(Database::TDBTransaction &DBTransaction, int GroupKey, int ContactKey);
	static bool SummaGroupExists(Database::TDBTransaction &DBTransaction, int ContactKey);
	static void RemoveSummaGroup(Database::TDBTransaction &DBTransaction, int GroupKey, int ContactKey);
	static void RemoveSummaGroup(Database::TDBTransaction &DBTransaction, int ContactKey);
	static void BuildXMLListGroups(Database::TDBTransaction &DBTransaction, TPOS_XMLBase &Data);
	static void BuildXMLListMemberGroups(Database::TDBTransaction &DBTransaction, TPOS_XMLBase &Data);

	static int GetOrCreateGroup(Database::TDBTransaction &DBTransaction, UnicodeString inGroupName, int inGroupNumber);
	static int GetOrCreateGroup(Database::TDBTransaction &DBTransaction, UnicodeString inGroupName);
	static void CreateGroup(Database::TDBTransaction &DBTransaction, ContactGroup inGroup);

   static bool GroupExists(Database::TDBTransaction &DBTransaction, int inGroupKey);
   static bool GroupExists(Database::TDBTransaction &DBTransaction, UnicodeString inGroupName);
   static int FindGroup(Database::TDBTransaction &DBTransaction,  UnicodeString inGroupName);
   static int GetGroupType(Database::TDBTransaction &DBTransaction, UnicodeString inGroupName);

   static void GetGroupsList(Database::TDBTransaction &DBTransaction,TStrings *GroupName, TStrings *GroupType);
   static void GetGroupsList(Database::TDBTransaction &DBTransaction, std::vector<ContactGroup> &DataVector);
   static void GetGroupsfromType(Database::TDBTransaction &DBTransaction, std::vector<ContactGroup> &DataVector, int inType);

	static void GetGroup(Database::TDBTransaction &DBTransaction, ContactGroup &myData);
	static void SetGroup(Database::TDBTransaction &DBTransaction,int &GroupKey,UnicodeString GroupName, int GroupType = 2);
   static int SetDefaultGroup(Database::TDBTransaction &DBTransaction);
   static void DeleteGroup(Database::TDBTransaction &DBTransaction,int GroupKey);
   static void UpdateGroup(Database::TDBTransaction &DBTransaction, ContactGroup myData);
	static std::vector<ContactGroup> SortGroups(Database::TDBTransaction &DBTransaction, std::vector<ContactGroup> temp);

};

#endif
