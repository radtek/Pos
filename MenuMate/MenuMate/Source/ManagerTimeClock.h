//---------------------------------------------------------------------------

#ifndef ManagerTimeClockH
#define ManagerTimeClockH
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include "MM_DBCore.h"
#include "StaffHours.h"
#include "GlobalSettings.h"
//---------------------------------------------------------------------------

class TManagerTimeClock
{
   private :
      void SetClockInDept(Database::TDBTransaction &DBTransaction,AnsiString DeptName,int DeptCode);
      int GetClockInDept(Database::TDBTransaction &DBTransaction,AnsiString DeptName,int DeptCode);
   public :
   AnsiString GetClockInDeptName(Database::TDBTransaction &DBTransaction,int DeptKey);
   int GetClockInDeptCode(Database::TDBTransaction &DBTransaction,int DeptKey);
   void UpdateClockInDept(Database::TDBTransaction &DBTransaction,AnsiString DeptName,int DeptCode,int DeptKey);      

   void GetClockInDeptList(Database::TDBTransaction &DBTransaction,TStringList *DeptList);
   void AddClockInDept(Database::TDBTransaction &DBTransaction,AnsiString DeptName,int DeptCode);
   void DelClockInDept(Database::TDBTransaction &DBTransaction,int Key);
   void SetDefaultDept(Database::TDBTransaction &DBTransaction);

	int SetUserBreak(Database::TDBTransaction &DBTransaction, int &ContactTimeKey, TTime inBreak);
	TTime GetUserBreak(Database::TDBTransaction &DBTransaction,int inContactKey, int &ContactTimeKey);
   TDateTime ClockedInDateTime(Database::TDBTransaction &DBTransaction,int inContactKey);
	int ClockOut(Database::TDBTransaction &DBTransaction,int &inContactKey,TDateTime &LogInTime,TDateTime &LogOutTime);
   void ClockIn(Database::TDBTransaction &DBTransaction,int inContactKey,int DeptCode,TDateTime LogInTime);
   bool ClockedIn(Database::TDBTransaction &DBTransaction,int inContactKey);
   int ClockedCount(Database::TDBTransaction &DBTransaction,int inContactKey);
   void MoveDisplayOrderUp(Database::TDBTransaction &DBTransaction,int Key);
	void MoveDisplayOrderDown(Database::TDBTransaction &DBTransaction,int Key);
	TDateTime RoundTime(TDateTime inTime, int RoundTo);
   void ConvertTo(int &in);
	void GetLoggedInDetails(Database::TDBTransaction &DBTransaction,TDateTime LogInTime, std::vector<TStaffHours> &inStaffHours,UnicodeString inDevicename);
	void SetLoggedInOutDetails(Database::TDBTransaction &DBTransaction, TStaffHoursContainer::iterator inStaffHours);
    TDateTime GetRoundedLoginTime(Database::TDBTransaction &DBTransaction, TDateTime loginTime, int contact_time_key);
    TDateTime GetRoundedLogOutTime(Database::TDBTransaction &DBTransaction, TDateTime loginTime, int contact_time_key);
    void UpdateClockInOut(Database::TDBTransaction &DBTransaction, int contact_time_key, int contact_key);
    bool ClockedInDep(Database::TDBTransaction &DBTransaction,int inContactKey);
    void DelClockInDeptFromLoc(Database::TDBTransaction &DBTransaction,int Key);
    bool CheckClockedIn(Database::TDBTransaction &DBTransaction,int Key);
    void AddDepInLoc(Database::TDBTransaction &DBTransaction, AnsiString DeptName,int DeptCode);
};


#endif
 