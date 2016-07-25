//---------------------------------------------------------------------------

#ifndef ManagerTimeClockH
#define ManagerTimeClockH
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include <IBDatabase.hpp>
#include <IBQuery.hpp>

#include "MM_DBCore.h"
#include "StaffHours.h"
#include "POS_XMLBase.h"
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
	void BuildXMLTotalsTimeClock(TPOS_XMLBase &Data,int SiteID,int ContactKey, TDateTime &Login, TDateTime &LogOut);
	TDateTime RoundTime(TDateTime inTime, int RoundTo);
   void ConvertTo(int &in);
	void GetLoggedInDetails(Database::TDBTransaction &DBTransaction,TDateTime LogInTime, std::vector<TStaffHours> &inStaffHours,UnicodeString inDevicename);
	void SetLoggedInOutDetails(Database::TDBTransaction &DBTransaction, TStaffHoursContainer::iterator inStaffHours);
};


#endif
 