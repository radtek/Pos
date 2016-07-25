//---------------------------------------------------------------------------

#ifndef STAFFHOURSH
#define STAFFHOURSH
//---------------------------------------------------------------------------
#include "ListManager.h"
#include "Timer.h"

enum eLogInOut{eLogIn, eLogOut};

class TStaffHours
{
	public :
	TStaffHours() :
						ContactName(""),
						ContactNumber(NULL),
						LoggedIn(NULL),
						LoggedOut(NULL),
						DeptNumber(NULL),
						DeptName(""),
						isLogOutNull(false),
						ContactTimeKey(NULL),
						HourlyRate(0),
						Break(0){}

	TStaffHours( AnsiString inContactName, int inContactNumber,
						TDateTime inLoggedIn, TDateTime inLoggedOut,
						int inDeptNumber, AnsiString inDeptName,
						int inContactTimeKey, double inHourlyRate,
						double inBreak) :
						ContactName(inContactName),
						ContactNumber(inContactNumber),
						LoggedIn(inLoggedIn),
						LoggedOut(inLoggedOut),
						DeptNumber(inDeptNumber),
						DeptName(inDeptName),
						isLogOutNull(false),
						ContactTimeKey(inContactTimeKey),
						HourlyRate(inHourlyRate),
						Break(inBreak){}

	AnsiString GetContactName(void){return ContactName;}
	int GetContactNumber(void){return ContactNumber;}
	TDateTime GetLoggedIn(void){return LoggedIn;}
	TDateTime GetLoggedOut(void){return LoggedOut;}
	int GetDeptNumber(void){return DeptNumber;}
	AnsiString GetDeptName(void){return DeptName;}
	bool GetLogOutNull(void){return isLogOutNull;}
	double GetHoursWorked(void);
	int GetContactTimeKey(void){return ContactTimeKey;}
	double GetHourlyRate(void){return HourlyRate;}
	int GetBreak(void){return Break;}

	void SetContactName(AnsiString inContactName){ContactName = inContactName;}
	void SetContactNumber(int inContactNumber){ContactNumber = inContactNumber;}
	void SetLoggedIn(TDateTime inLoggedIn){LoggedIn = inLoggedIn;}
	bool SetLoggedOut(TDateTime inDate);
	void SetDeptNumber(int inDeptNumber){DeptNumber = inDeptNumber;}
	void SetDeptName(AnsiString inDeptName){DeptName = inDeptName;}
	void SetLogOutNull(bool inLogOutNull){isLogOutNull = inLogOutNull;}
	void SetHoursWorked(void);
	void SetContactTimeKey(int inContactTimeKey){ContactTimeKey = inContactTimeKey;}
	void SetHourlyRate(double inHourlyRate){HourlyRate = inHourlyRate;}
	void SetBreak(int inBreak){Break = inBreak;}

	private :
	AnsiString ContactName;
	int ContactNumber;
	TDateTime LoggedIn;
	TDateTime LoggedOut;
	int DeptNumber;
	AnsiString DeptName;
	double HoursWorked;
	int ContactTimeKey;
	double HourlyRate;
	bool isLogOutNull;
	int Break;

};




typedef std::map< int, TStaffHours > TStaffHoursContainer;
class TStaffHoursInterface
{
	public:
		void InsertToContainer(int row, TStaffHours inStaffHours);
		TStaffHoursContainer::iterator begin(void);
		TStaffHoursContainer::iterator end(void);
		TStaffHours* GetStaff(int row);
		int size(void);
      bool Empty(void);
	private:
		TStaffHoursContainer StaffHoursContainer;
};






class TStaffHoursController
{
	private :
		std::auto_ptr<TfrmListManager> frmListManager;
      Database::TDBTransaction &DBTransaction;
		TForm *DisplayOwner;
		void PopulateListManager(void);
		void InitialiseHeader(void);
		void InitialiseListManager(void);
		void ChangeListManagerDimensions(void);
		TDateTime getCurrentTime(void);
		void OnClose(int ItemIndex, int ColIndex = -1);
		void OnSelect(int ItemIndex, int ColIndex = -1);
		double TotalHours;
		void UpdateDatabase(void);

	public :
		TStaffHoursController(TForm *inDisplayOwner,Database::TDBTransaction &inDBTransaction);
		void Run(void);
		TStaffHoursInterface Get(void);
		TStaffHoursInterface StaffHours;
};



#endif
