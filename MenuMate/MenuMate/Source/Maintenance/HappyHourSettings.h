//---------------------------------------------------------------------------

#ifndef HappyHourSettingsH
#define HappyHourSettingsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <CheckLst.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include "ZForm.h"
#include "DBHappyHour.h"
#include "GlobalSettings.h"
//---------------------------------------------------------------------------
class TfrmHappyHourSettings : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *Panel24;
	TLabel *Label12;
	TLabel *Label13;
	TLabel *Label4;
	TCheckListBox *lbDay;
	TPanel *Panel2;
    TTouchBtn *TouchBtn1;
    TTouchBtn *TouchBtn2;
    TTouchBtn *TouchBtn3;
    TTouchBtn *TouchBtn4;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label1;
    TLabel *Label8;
    TLabel *Label10;
    TLabel *Label11;
    TTouchBtn *BtnDate;
    TTouchBtn *BtnMonth;
    TTouchBtn *BtnYear;
    TTouchBtn *BtnStartHour;
    TTouchBtn *BtnStartMinute;
    TTouchBtn *BtnStartAMPM;
    TTouchBtn *BtnEndHour;
    TTouchBtn *BtnEndMinute;
    TTouchBtn *BtnEndAMPM;
    TComboBox *cbHappyHourProfile;
    TGroupBox *GroupBox1;
    TGroupBox *GroupBox2;
    TGroupBox *GroupBox3;
    TComboBox *csPriceLevel;
    TCheckBox *cbMembersOnly;
    TCheckBox *cbNoPointsEarnedDuring;
    TGroupBox *GroupBox4;
    TTouchBtn *BtnPriority;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);

	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);

	void __fastcall cbMembersOnlyClick(TObject *Sender);
	void __fastcall cbNoPointsEarnedDuringClick(TObject *Sender);
    void __fastcall cbHappyHourProfileChange(TObject *Sender);
    void __fastcall BtnDateMouseClick(TObject *Sender);
    void __fastcall BtnMonthMouseClick(TObject *Sender);
    void __fastcall BtnYearMouseClick(TObject *Sender);
    void __fastcall BtnStartHourMouseClick(TObject *Sender);
    void __fastcall BtnStartMinuteMouseClick(TObject *Sender);
    void __fastcall BtnStartAMPMMouseClick(TObject *Sender);
    void __fastcall BtnEndHourMouseClick(TObject *Sender);
    void __fastcall BtnEndMinuteMouseClick(TObject *Sender);
    void __fastcall BtnEndAMPMMouseClick(TObject *Sender);
    void __fastcall btnADDProfileClick(TObject *Sender);
    void __fastcall btnEDITProfileClick(TObject *Sender);
    void __fastcall btnDeleteProfileClick(TObject *Sender);
    void __fastcall lbDayClickCheck(TObject *Sender);
    void __fastcall BtnResetMouseClick(TObject *Sender);
    void __fastcall lbDayClick(TObject *Sender);
    void __fastcall cbHappyHourProfileDblClick(TObject *Sender);
    void __fastcall csPriceLevelChange(TObject *Sender);
    void __fastcall BtnPriorityMouseClick(TObject *Sender);


protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	 BEGIN_MESSAGE_MAP
			MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
	 END_MESSAGE_MAP(TForm)
private:	// User declarations
    TDateTime _currentTimeStamp;
     bool ValidateTime(bool validateFlag,bool validationType );
     bool EnableDate();
     void DayOfWeek();
     void DateBtnCheck();
     void DateAndDayReset();
     void ResetForm();
     bool ShowMessage(int dayindex);

public:		// User declarations
        TDateTime SelectedStartTime;
        TDateTime SelectedEndTime;
        TDateTime SelectedDateTime;
        bool EnableEdit;
        bool ProfileEdited;
     __fastcall TfrmHappyHourSettings(TComponent* Owner);

        std::vector<THappyHour> HappyHourProfiles;
        std::vector<THappyHour> DeletedHappyHourProfiles;
        static int CurrentSelectedProfileKey;
        static int MaxHHProfileKey;
        static int NewMaxNoHHProfileAdded;

        void UpdateEditedProfile(int currentSelectedProfileKey,std::vector<THappyHour> &happyHourProfiles);
        void ProfileEditAndSave(bool eventType);
        void ProfileInitialize(TDateTime startTime,TDateTime endTime);

        void AssignHHProfileDayValues(std::vector<int> HappyHourProfileDays );
        void AssignHHProfileDaySettings(std::vector<int> HappyHourProfileSettings);

        bool CheckNoOfHHProfile(int dayindex);
        TDateTime ProfileDateTime;
        void ResetHoursButtonValues(TDateTime currentTimeStamp);

        void AssignHHProfileValues();
        int  CalcHHProfilesinDate(TDateTime ProfileDate);

        std::map<AnsiString,int> ProfilePriceLevels;
        int FindPriceLevelIndex(int PriceLevel);

};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmHappyHourSettings *frmHappyHourSettings;
//---------------------------------------------------------------------------
#endif
