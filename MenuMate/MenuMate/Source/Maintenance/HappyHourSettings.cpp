//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "HappyHourSettings.h"
#include "MMMessageBox.h"
#include "ManagerVariable.h"
#include "enum.h"
#include "SelectDateAndTime.h"
#include "ManagerHappyHour.h"
#include "MMTouchKeyboard.h"
#include "Vector.h"
#include "GlobalSettings.h"
#include "MMTouchNumpad.h"

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif

#ifdef PalmMate
#include "Palm.h"
#endif

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
//TfrmHappyHourSettings *frmHappyHourSettings;
//---------------------------------------------------------------------------
__fastcall TfrmHappyHourSettings::TfrmHappyHourSettings(TComponent* Owner)
	: TZForm(Owner)
{
}



//---------------------------------------------------------------------------
//intializing static variables

 int TfrmHappyHourSettings::CurrentSelectedProfileKey=0;
 int TfrmHappyHourSettings::MaxHHProfileKey=0;
 int TfrmHappyHourSettings::NewMaxNoHHProfileAdded=0;

void __fastcall TfrmHappyHourSettings::FormShow(TObject *Sender)
{

    FormResize(NULL);

    BtnDate->Caption = "Day";
    BtnMonth->Caption = "Month";
    BtnYear->Caption = "Year";
    //reseting start and end time in 12 hour format according to current time
    ResetHoursButtonValues(Now());

     //get the available profiles  in combo box
    TManagerHappyHour *loadProfiles= new TManagerHappyHour();
    HappyHourProfiles = loadProfiles->LoadHappyHourProfiles(false);

    //loading the profiles name in combo box
    for(int i = 0; i < HappyHourProfiles.size(); i++)
    {
        cbHappyHourProfile->Items->Add(HappyHourProfiles[i].ProfileName);
    }


    //calculate no of profiles in database
    MaxHHProfileKey=loadProfiles->CalcNoOfProfiles();

    /// load all the price levels
    ProfilePriceLevels.clear();
    ProfilePriceLevels =loadProfiles->loadAllPriceLevels();
    std::map<AnsiString,int>::iterator pricelevel = ProfilePriceLevels.begin();


   for (pricelevel=ProfilePriceLevels.begin(); pricelevel!=ProfilePriceLevels.end(); ++pricelevel)
     {
       csPriceLevel->Items->Add(pricelevel->first);
     }
    csPriceLevel->ItemIndex=-1;
}
//---------------------------------------------------------------------------
void __fastcall TfrmHappyHourSettings::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmHappyHourSettings::FormResize(TObject *Sender)
{
  /*	if (Tag != Screen->Width)
   {
      int Temp = Tag;
		Tag = Screen->Width;

   	ScaleBy(Screen->Width, Temp);
   }
	ClientWidth = Panel2->Left + Panel2->Width + Panel24->Left;
	ClientHeight = Panel24->Height + (Panel24->Top * 2);
   Left = (Screen->Width - Width) / 2;
	Top = (Screen->Height - Height) / 2; */
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TfrmHappyHourSettings::btnOkClick(TObject *Sender)
{
   try
   {
      int itemIndex=0;
      bool message_flag=false;
      bool profileEdited=false;

    //check for profile combobox atleas one profile must be selected for pressing ok
    if(cbHappyHourProfile->ItemIndex==-1 && DeletedHappyHourProfiles.size()==0 )
    {
       MessageBox("Please select Happy Hour Profile first.", "Error", MB_OK + MB_ICONERROR);
       return;
    }

 if (HappyHourProfiles.size()>0 || DeletedHappyHourProfiles.size()>0)
 {
    for(int hhVal = 0; hhVal < HappyHourProfiles.size(); hhVal++)
    {
       if(HappyHourProfiles[hhVal].IsEditedProfile )profileEdited=true;
     }
    if(!profileEdited && DeletedHappyHourProfiles.size()==0)
    {
       Close();
       return;
    }


    TManagerHappyHour *profile = new TManagerHappyHour();
    itemIndex =profile->ValidationOfProfiles(HappyHourProfiles);
    if(itemIndex >=0)
    {
        EnableEdit=false;
        DateAndDayReset();
        EnableEdit=true;
        cbHappyHourProfile->ItemIndex=itemIndex;
        CurrentSelectedProfileKey=HappyHourProfiles[cbHappyHourProfile->ItemIndex].ProfileKey;
        //assign values of happy hour profile  to components of GUI
        AssignHHProfileValues();
        return;
    }

    if(!ValidateTime(true,false))
    {
       return;
    }

     //message which will ask for bradcast happyhour profiles to all terminals.
    if(MessageBox("Do you wish to change Happy Hour times for all MenuMate P.O.S terminals?\r(Happy Hour forces don't apply)",
    "Update all P.O.S Terminals",
    MB_YESNO + MB_ICONQUESTION) == IDYES)
    {
        message_flag=true;
        std::vector<THappyHour> allTerminalsHappyHourProfiles;
         std::vector<THappyHour> CopyHappyHourProfiles;

       CopyHappyHourProfiles= HappyHourProfiles;

       // load all the terminals happy hour profiles
        allTerminalsHappyHourProfiles=profile->LoadHappyHourProfiles(true);

       for(int profile=0; profile < allTerminalsHappyHourProfiles.size();profile++)
       {
           CopyHappyHourProfiles.push_back(allTerminalsHappyHourProfiles[profile]);
       }

     //     itemIndex = profile->CheckAllHHProfilesName(CopyHappyHourProfiles);

      if(itemIndex==-1)
         itemIndex = profile->CheckAllHHProfilesPriorities(CopyHappyHourProfiles);

    } else
    {
        itemIndex =    profile->CheckAllHHProfilesPriorities(HappyHourProfiles);
    }





  if(itemIndex >=0)
    {
        EnableEdit=false;
        DateAndDayReset();
        EnableEdit=true;
        cbHappyHourProfile->ItemIndex=itemIndex;
        CurrentSelectedProfileKey=HappyHourProfiles[cbHappyHourProfile->ItemIndex].ProfileKey;
        //assign values of happy hour profile  to components of GUI
        AssignHHProfileValues();
        return;
    }

    if(!ValidateTime(true,false))
    {
       return;
    }


    //Broadcasting
    profile->ProfileBroadcast(HappyHourProfiles,MaxHHProfileKey,NewMaxNoHHProfileAdded,message_flag, DeletedHappyHourProfiles);

     //  call ManagerHappyHour's  HappyHourProfileSaveAndUpdate function for saving and updating profiles
    if(HappyHourProfiles.size()>0)
     {
       profile->HappyHourProfileSaveAndUpdate(HappyHourProfiles);
     }


     Close();

    }

   }
   catch(Exception &Err)
  {
    TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
    throw;
  }

}
//---------------------------------------------------------------------------
void __fastcall TfrmHappyHourSettings::btnCancelClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void __fastcall TfrmHappyHourSettings::cbMembersOnlyClick(TObject *Sender)
{
   ProfileEdited =true;
    if(!ValidateTime(false,false))return;
   if (EnableEdit)
       UpdateEditedProfile(CurrentSelectedProfileKey,HappyHourProfiles);
}
//---------------------------------------------------------------------------

void __fastcall TfrmHappyHourSettings::cbNoPointsEarnedDuringClick(TObject *Sender)
{
    ProfileEdited =true;
     if(!ValidateTime(false,false))return;
    if (EnableEdit)
       UpdateEditedProfile(CurrentSelectedProfileKey,HappyHourProfiles);
}
//---------------------------------------------------------------------------
void __fastcall TfrmHappyHourSettings::cbHappyHourProfileChange(TObject *Sender)
{

   ProfileEditAndSave(true);

}
//---------------------------------------------------------------------------

void __fastcall TfrmHappyHourSettings::BtnDateMouseClick(TObject *Sender)
{
   if(EnableDate())
   {
     TfrmSelectDateTime *frmSelectDateTime = new TfrmSelectDateTime(this);
     BtnDate->Caption = frmSelectDateTime->PopUpDatePicker("Enter the Date", UnicodeString(BtnDate->Caption));
     delete frmSelectDateTime;
   }
   DateBtnCheck();

}
//---------------------------------------------------------------------------

void __fastcall TfrmHappyHourSettings::BtnMonthMouseClick(TObject *Sender)
{
  TfrmSelectDateTime *frmSelectDateTime = new TfrmSelectDateTime(this);
  if(EnableDate())
        BtnMonth->Caption = frmSelectDateTime->PopUpDatePicker("Enter the Month", UnicodeString(BtnMonth->Caption));
    DateBtnCheck();
}
//---------------------------------------------------------------------------

void __fastcall TfrmHappyHourSettings::BtnYearMouseClick(TObject *Sender)
{
     if(EnableDate())
     {
        TfrmSelectDateTime *frmSelectDateTime = new TfrmSelectDateTime(this);
        BtnYear->Caption = frmSelectDateTime->PopUpDatePicker("Enter the Year", UnicodeString (BtnYear->Caption));
        delete frmSelectDateTime;
     }
     DateBtnCheck();
}
//---------------------------------------------------------------------------



void __fastcall TfrmHappyHourSettings::BtnStartHourMouseClick(TObject *Sender)
{
     TfrmSelectDateTime *frmSelectDateTime = new TfrmSelectDateTime(this);
     BtnStartHour->Caption = frmSelectDateTime->PopUpDatePicker("Enter the Hour", UnicodeString (BtnStartHour->Caption));
    ProfileEdited =true;

     if(!ValidateTime(true,false))return;
     if (EnableEdit)
       UpdateEditedProfile(CurrentSelectedProfileKey,HappyHourProfiles);


}
//---------------------------------------------------------------------------


void __fastcall TfrmHappyHourSettings::BtnStartMinuteMouseClick(TObject *Sender)
{
    TfrmSelectDateTime *frmSelectDateTime = new TfrmSelectDateTime(this);
	BtnStartMinute->Caption = frmSelectDateTime->PopUpDatePicker("Enter the Minute", UnicodeString (BtnStartMinute->Caption));
    ProfileEdited =true;

    if(!ValidateTime(true,false))return;
    if (EnableEdit)
       UpdateEditedProfile(CurrentSelectedProfileKey,HappyHourProfiles);
}
//---------------------------------------------------------------------------

void __fastcall TfrmHappyHourSettings::BtnStartAMPMMouseClick(TObject *Sender)
{
    ProfileEdited =true;
     if(BtnStartAMPM->Caption == "PM")
    {
      BtnStartAMPM->Caption = "AM";
    }
    else
    {
      BtnStartAMPM->Caption = "PM";
    }


    if(!ValidateTime(true,false))return;
   if (EnableEdit)
       UpdateEditedProfile(CurrentSelectedProfileKey,HappyHourProfiles);
}
//---------------------------------------------------------------------------

void __fastcall TfrmHappyHourSettings::BtnEndHourMouseClick(TObject *Sender)
{
     TfrmSelectDateTime *frmSelectDateTime = new TfrmSelectDateTime(this);
     BtnEndHour->Caption = frmSelectDateTime->PopUpDatePicker("Enter the Hour", UnicodeString (BtnEndHour->Caption));
     ProfileEdited =true;

     if(!ValidateTime(true,false))return;

     if (EnableEdit)
       UpdateEditedProfile(CurrentSelectedProfileKey,HappyHourProfiles);
}
//---------------------------------------------------------------------------

void __fastcall TfrmHappyHourSettings::BtnEndMinuteMouseClick(TObject *Sender)
{
    TfrmSelectDateTime *frmSelectDateTime = new TfrmSelectDateTime(this);
 	BtnEndMinute->Caption = frmSelectDateTime->PopUpDatePicker("Enter the Minute", UnicodeString (BtnEndMinute->Caption));
    ProfileEdited =true;
    if(!ValidateTime(true,false))return;

    if (EnableEdit)
       UpdateEditedProfile(CurrentSelectedProfileKey,HappyHourProfiles);
}
//---------------------------------------------------------------------------

void __fastcall TfrmHappyHourSettings::BtnEndAMPMMouseClick(TObject *Sender)
{
    if(BtnEndAMPM->Caption == "PM")
    {
      BtnEndAMPM->Caption = "AM";
    }
    else
    {
      BtnEndAMPM->Caption = "PM";
    }

   ProfileEdited =true;
   if(!ValidateTime(true,false))return;
    if (EnableEdit)
       UpdateEditedProfile(CurrentSelectedProfileKey,HappyHourProfiles);
}
//---------------------------------------------------------------------------
void __fastcall TfrmHappyHourSettings::btnADDProfileClick(TObject *Sender)
{

    ResetForm();
    cbHappyHourProfile->ItemIndex=-1;
    ProfileEditAndSave(false);

}
//---------------------------------------------------------------------------

void __fastcall TfrmHappyHourSettings::btnEDITProfileClick(TObject *Sender)
{
    if(cbHappyHourProfile->ItemIndex!=-1 )
    {
        ProfileEditAndSave(false);
    }

}
//---------------------------------------------------------------------------

void __fastcall TfrmHappyHourSettings::btnDeleteProfileClick(TObject *Sender)
{

if ( HappyHourProfiles.size()>0)
{
    if (cbHappyHourProfile->ItemIndex == -1)
    {
       MessageBox("Please Select Happy Hour Profile First", "Error", MB_OK + MB_ICONERROR);
       return;
    }


    if (MessageBox("Are you sure you wish to remove this HappyHourProfile?",
      "Warning", MB_ICONWARNING + MB_OKCANCEL) == ID_OK)
    {
       // delete the profile for this terminal
       TManagerHappyHour *deleteHHProfile = new TManagerHappyHour();

       // delete all the info of happy hour
  //     deleteHHProfile->DeleteHappyHourProfile(HappyHourProfiles[cbHappyHourProfile->ItemIndex].ProfileKey);

       if(HappyHourProfiles[cbHappyHourProfile->ItemIndex].ProfileTerminalsKeys.size()==1 )
       { //if profile is for this terminal then delete the whole profile
         deleteHHProfile->DeleteHappyHourProfile(HappyHourProfiles[cbHappyHourProfile->ItemIndex].ProfileKey);
       }else
       {  // if profile is for multiple terminals then delete the mapping for this terminal
          deleteHHProfile->DeleteHHProfileTerminalMapping(HappyHourProfiles[cbHappyHourProfile->ItemIndex].ProfileKey, TDeviceRealTerminal::Instance().ID.DeviceKey);
       }
        DeletedHappyHourProfiles.push_back(HappyHourProfiles[cbHappyHourProfile->ItemIndex]);
     //deleting from vector
        HappyHourProfiles.erase(HappyHourProfiles.begin() + cbHappyHourProfile->ItemIndex);

        cbHappyHourProfile->Items->Delete(cbHappyHourProfile->ItemIndex);
        cbHappyHourProfile->ItemIndex=-1;
        if (cbHappyHourProfile->Items->Count ==0)cbHappyHourProfile->Text=" ";

        ResetForm();
    }

  }
}

//---------------------------------------------------------------------------
bool TfrmHappyHourSettings::ValidateTime(bool validateFlag,bool validationType)
{

    //validate Time
    int Day,Month,Year,profileDay,profileMonth,profileYear;

    TDateTime currentTimestamp = Dateutils::EncodeDateTime(YearOf(Now()),MonthOf(Now()),DayOf(Now()),
                                                            HourOfTheDay(Now()),MinuteOfTheHour(Now()),00,000);
    if(BtnDate->Caption == "Day" || (BtnMonth->Caption == "Month") || (BtnYear->Caption == "Year"))
    {
        profileDay = 30;
        profileMonth = 12;
        profileYear = 1899;
        Day = DayOf(currentTimestamp);
        Month = MonthOf(currentTimestamp);
        Year = YearOf(currentTimestamp);

      //  ProfileInitialize(Now(),Now());

    }
    else
    {
          Day = StrToInt(BtnDate->Caption);
         Month = StrToInt(BtnMonth->Caption);
         Year = StrToInt(BtnYear->Caption);
    }
    int StartHour = 0;
    int EndHour = 0;

    if(StrToInt(BtnStartHour->Caption) != 12)
    {
      StartHour = BtnStartAMPM->Caption == "PM" ?  StrToInt(BtnStartHour->Caption) + 12 : StrToInt(BtnStartHour->Caption);
    }
    else
    {
      if(BtnStartAMPM->Caption == "AM")
      {
        StartHour = 0;
      }
      else
      {
        StartHour = StrToInt(BtnStartHour->Caption);
      }
    }
    if(StartHour > 23)
    {
      StartHour = 0;
    }
    int StartMinute = StrToInt(BtnStartMinute->Caption);

    //validation for end date
    if(StrToInt(BtnEndHour->Caption) != 12)
    {
      EndHour = BtnEndAMPM->Caption == "PM" ?  StrToInt(BtnEndHour->Caption) + 12 : StrToInt(BtnEndHour->Caption);
    }
    else
    {
      if(BtnEndAMPM->Caption == "AM")
      {
        EndHour = 0;
      }
      else
      {
        EndHour = StrToInt(BtnEndHour->Caption);
      }
    }
    if(EndHour > 23)
    {
      EndHour = 0;
    }
    int EndMinute = StrToInt(BtnEndMinute->Caption);

     //encode date
    TDateTime encodedStartDate =Dateutils::EncodeDateTime(Year,Month,Day,StartHour,StartMinute,SecondOfTheMinute(currentTimestamp),MilliSecondOfTheSecond(currentTimestamp));

    TDateTime encodedEndDate =Dateutils::EncodeDateTime(Year,Month,Day,EndHour,EndMinute,SecondOfTheMinute(currentTimestamp),MilliSecondOfTheSecond(currentTimestamp));


     TDateTime encodedDateTime;


     if(profileYear == 1899)
     {
         encodedDateTime = Dateutils::EncodeDateTime(profileYear,profileMonth,profileDay,0,0,00,000);
      }
      else
      {
           encodedDateTime = Dateutils::EncodeDateTime(Year,Month,Day,HourOfTheDay(currentTimestamp),MinuteOfTheHour(currentTimestamp),SecondOfTheMinute(currentTimestamp),MilliSecondOfTheSecond(currentTimestamp));

      }

   TValueRelationship startResult  = Dateutils::CompareDateTime(encodedStartDate,currentTimestamp);
    TValueRelationship endResult  = Dateutils::CompareDateTime(encodedEndDate,currentTimestamp);

    //  if time entered is prior to current time
    if (validateFlag)
    {

       if (!validationType)
       {
            if( ( (startResult == LessThanValue)||(endResult == LessThanValue)) &&  (IsToday(encodedDateTime)) )
            {
                MessageBox("Time entered is prior to current time, please enter a valid time.", "Invalid Time", MB_OK + MB_ICONERROR);

                ResetHoursButtonValues(Now());
                return false ;

            }
       }


        if(profileYear != 1899)
        {

         if(validationType)
          {
        //    if( Dateutils::CompareDateTime(encodedEndDate,Now())== LessThanValue )

      if( Dateutils::CompareDateTime(Dateutils::EncodeDateTime(Year,Month,Day,HourOfTheDay(Now()),MinuteOfTheHour(Now()),SecondOfTheMinute(Now()),MilliSecondOfTheSecond(Now())),Now())== LessThanValue )
            {
                MessageBox("Date entered is prior to current date, please enter a valid date.", "Invalid Date", MB_OK + MB_ICONERROR);

                DateAndDayReset();
                profileDay = 30;
                profileMonth = 12;
                profileYear = 1899;


                return false ;
            }
          }
        }

    }

        TFormatSettings Settings;
        GetLocaleFormatSettings(10955, Settings);



        SelectedStartTime = encodedStartDate;
        SelectedEndTime   = encodedEndDate;
        if(profileYear != 1899)
         {
            SelectedDateTime   = encodedDateTime;
         }

        if (validateFlag)
        {
                if (SelectedStartTime >  SelectedEndTime)
                {
                   MessageBox("End Time is prior to Start Time, please enter a valid date.", "Invalid Time", MB_OK + MB_ICONERROR);
                   ResetHoursButtonValues(Now());
                   return false;
                }
        }

        return true;


}




void TfrmHappyHourSettings::UpdateEditedProfile(int CurrentSelectedProfileKey,std::vector<THappyHour> &happyHourProfiles)
{
    int DaySelectedNo=0;


    //update the profile
    if (happyHourProfiles.size()>0)
     {
        for(int hhVal = 0; hhVal < happyHourProfiles.size(); hhVal++)
        {

            if (happyHourProfiles.at(hhVal).ProfileKey== CurrentSelectedProfileKey  )
            {

                happyHourProfiles.at(hhVal).ProfileKey=CurrentSelectedProfileKey;
                happyHourProfiles.at(hhVal).ProfileName=cbHappyHourProfile->Text;
                happyHourProfiles.at(hhVal).StartTime=SelectedStartTime;
                happyHourProfiles.at(hhVal).EndTime=SelectedEndTime;
                 happyHourProfiles.at(hhVal).TerminalKey= TDeviceRealTerminal::Instance().ID.DeviceKey;

                if(BtnDate->Caption == "Day" || (BtnMonth->Caption == "Month") || (BtnYear->Caption == "Year"))
                {
                     happyHourProfiles.at(hhVal).DateTime = ProfileDateTime;
                }else
                {
                    happyHourProfiles.at(hhVal).DateTime=SelectedDateTime;
                }


                happyHourProfiles.at(hhVal).HappyHourProfileDays.clear();
                for(int day=eSunday; day<eEndOfDays; day++)
                {
                   if (lbDay->Checked[day])
                   {
                   happyHourProfiles.at(hhVal).HappyHourProfileDays.push_back(day);

                     }
                }


               happyHourProfiles.at(hhVal).HappyHourProfileSettings.clear();


                if (cbMembersOnly->Checked)
                {
                  happyHourProfiles.at(hhVal).HappyHourProfileSettings.push_back(eHappyHourIsForMembersOnly);
                }

                if (cbNoPointsEarnedDuring->Checked)
                {
                  happyHourProfiles.at(hhVal).HappyHourProfileSettings.push_back(eNoPointsEarnedDuringHappyHour);
                }

                 //insert the terminal keys

                bool terminalFound=false;
                for(std::vector<int>::iterator deviceKey = happyHourProfiles.at(hhVal).ProfileTerminalsKeys.begin(); deviceKey !=  happyHourProfiles.at(hhVal).ProfileTerminalsKeys.end(); deviceKey++)
                {
                      if((*deviceKey)==TDeviceRealTerminal::Instance().ID.DeviceKey  )
                      {
                         terminalFound=true;
                         break;
                      }

                }

               if(!terminalFound )
               {
                 happyHourProfiles.at(hhVal).ProfileTerminalsKeys.push_back(TDeviceRealTerminal::Instance().ID.DeviceKey );
               }


                 happyHourProfiles.at(hhVal).IsEditedProfile=ProfileEdited;

                 happyHourProfiles.at(hhVal).Pricelevel= ProfilePriceLevels[csPriceLevel->Text];

                if (BtnPriority->Caption=="Default")
                   happyHourProfiles.at(hhVal).Priority=0;
                else
                  happyHourProfiles.at(hhVal).Priority = StrToInt( BtnPriority->Caption);

              }
          }
     }
 }


//---------------------------------------------------------------------------

void __fastcall TfrmHappyHourSettings::BtnResetMouseClick(TObject *Sender)
{
    EnableEdit=false;
    DateAndDayReset();
    EnableEdit=true;

    if(!ValidateTime(false,false))return;

    if (EnableEdit)
       UpdateEditedProfile(CurrentSelectedProfileKey,HappyHourProfiles);
}

//---------------------------------------------------------------------------

void __fastcall TfrmHappyHourSettings::lbDayClickCheck(TObject *Sender)
{
   ProfileEdited =true;
   if(!ValidateTime(false,false))return;

   if (EnableEdit)
      UpdateEditedProfile(CurrentSelectedProfileKey,HappyHourProfiles);
}



void TfrmHappyHourSettings::DayOfWeek()
{
    int Day = StrToInt(BtnDate->Caption);
    int Month = StrToInt(BtnMonth->Caption);
    int Year = StrToInt(BtnYear->Caption);
    int DayValue ;
    int temp[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
    Year -= Month < 3;
    DayValue = ( Year + Year/4 - Year/100 + Year/400 + temp[Month-1] + Day) % 7;
    for(int i = 0 ; i<lbDay->Items->Count && i<eEndOfDays ; i++)
    {
        if( i == DayValue)
        {
            lbDay->Checked[i] = true;
        }
        else
        {
            lbDay->Checked[i] = false;
        }
        lbDay->ItemEnabled[i] = false;
        lbDay->Font->Color = 0x808080 ;
    }
}

//---------------------------------------------------------------------------------

bool TfrmHappyHourSettings::EnableDate()
{
    int count = 0;
    for(int index = 0 ; index<lbDay->Items->Count && index<eEndOfDays ; index++)
    {
        if( count>1 )
            return false;
        else
            if( lbDay->Checked[index] == true && count<2)
                count++;
    }
    return true;
}

//---------------------------------------------------------

 void TfrmHappyHourSettings::ProfileInitialize(TDateTime startTime,TDateTime endTime)
{
        int startHour, endHour;

        BtnStartHour->Caption = HourOf(startTime);
        BtnStartMinute->Caption = MinuteOf(startTime);
        BtnStartAMPM->Caption = "AM" ;

        BtnEndHour->Caption = HourOf(endTime);
        BtnEndMinute->Caption = MinuteOf(endTime);
        BtnEndAMPM->Caption = "AM" ;

        startHour = HourOf(startTime);
        endHour = HourOf(endTime);

        if(startHour >= 12)
        {
            BtnStartAMPM->Caption = "PM" ;
            if(startHour != 12)
            {
               BtnStartHour->Caption = IntToStr(startHour - 12);
            }
        }

        if(startHour == 0)
        {
           BtnStartHour->Caption = IntToStr(12);
        }

        if(endHour >= 12)
        {
            BtnEndAMPM->Caption = "PM" ;
            if(endHour != 12)
            {
               BtnEndHour->Caption = IntToStr(endHour - 12);
            }
        }

        if(endHour == 0)
        {
           BtnEndHour->Caption = IntToStr(12);
        }

}

  void TfrmHappyHourSettings::ProfileEditAndSave(bool editingOldProfileName)
  {
    bool hhHappyProfilePresent = false;
    THappyHour hhProfile;
    int comboIndex = -1;
    TDateTime startTime;
    TDateTime endTime;
    TDateTime dateTime;
    std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
    frmTouchKeyboard->MaxLength = 50;
    frmTouchKeyboard->AllowCarriageReturn = false;
    frmTouchKeyboard->StartWithShiftDown = true;
    frmTouchKeyboard->MustHaveValue = true;

    ResetForm();

    if(cbHappyHourProfile->ItemIndex==-1 )
    {
        frmTouchKeyboard->Caption = "Please enter Profile Name";


        if (frmTouchKeyboard->ShowModal() == mrOk)
        {
            for (int profile=0;profile < HappyHourProfiles.size();profile++)
            {
                if ( LowerCase(HappyHourProfiles[profile].ProfileName.Trim()) == LowerCase(frmTouchKeyboard->KeyboardText.Trim()) )
                {
                   MessageBox("Duplicate Name.Please Enter different name.", "Invalid Time", MB_OK + MB_ICONERROR);
                   return;
                }
            }
            if (frmTouchKeyboard->KeyboardText.Trim()=="")
            {
               MessageBox("Invalid  Name.Please Enter Valid name.", "Invalid Time", MB_OK + MB_ICONERROR);
               return;
            }

            cbHappyHourProfile->Items->Add(frmTouchKeyboard->KeyboardText);
            cbHappyHourProfile->ItemIndex=cbHappyHourProfile->Items->Count -1 ;
            NewMaxNoHHProfileAdded +=1; //static variable
            CurrentSelectedProfileKey =MaxHHProfileKey +NewMaxNoHHProfileAdded;
            hhHappyProfilePresent=false;


       }
        else
        {
         return;
        }

            // initializing the components   when new  profile is selected

                BtnDate->Caption = "Day";
                BtnMonth->Caption = "Month";
                BtnYear->Caption = "Year";

                ProfileInitialize( Now(), Now());

                for(int day=eSunday; day<lbDay->Items->Count && day<eEndOfDays; day++)
                {
                   lbDay->Checked[day] =false;
                }

                 cbMembersOnly->Checked=false;

                cbNoPointsEarnedDuring->Checked=false;

                csPriceLevel->ItemIndex=-1;
                BtnPriority->Caption="Default";

    }
 else
 {
         CurrentSelectedProfileKey=HappyHourProfiles[cbHappyHourProfile->ItemIndex].ProfileKey;

         //profile is in HappyHourProfiles vector
         hhHappyProfilePresent=true;

        if (!editingOldProfileName)
        {
            frmTouchKeyboard->KeyboardText =HappyHourProfiles[cbHappyHourProfile->ItemIndex].ProfileName;
            comboIndex=cbHappyHourProfile->ItemIndex;
            frmTouchKeyboard->Caption = "Please enter Profile Name";

            if (frmTouchKeyboard->ShowModal() == mrOk)
            {

                for (int profile=0;profile < HappyHourProfiles.size();profile++)
                {
                    if (LowerCase(HappyHourProfiles[profile].ProfileName.Trim()) == LowerCase(frmTouchKeyboard->KeyboardText.Trim()) )
                    {
                            MessageBox("Duplicate Name.Please Enter different name.", "Invalid Time", MB_OK + MB_ICONERROR);
                            return;
                    }
                }
                if (frmTouchKeyboard->KeyboardText.Trim()=="")
                {
                        MessageBox("Invalid  Name.Please Enter Valid name.", "Invalid Time", MB_OK + MB_ICONERROR);
                        return;
                }

                HappyHourProfiles[cbHappyHourProfile->ItemIndex].ProfileName=frmTouchKeyboard->KeyboardText;
                cbHappyHourProfile->Items->Delete(comboIndex);
                cbHappyHourProfile->Items->Insert(comboIndex,frmTouchKeyboard->KeyboardText);
                cbHappyHourProfile->ItemIndex=comboIndex;
                HappyHourProfiles[cbHappyHourProfile->ItemIndex].IsEditedProfile=true;
            }

         }


          //assign values of happy hour profile  to components of GUI
         AssignHHProfileValues();

 }
      // if it is a new profile add this to happy hour Profiles vector
    if(!hhHappyProfilePresent)
    {

    TDateTime profileDate;


    if(!ValidateTime(false,false))return;
    hhProfile.ProfileKey=CurrentSelectedProfileKey;
    hhProfile.ProfileName=cbHappyHourProfile->Text;

    hhProfile.StartTime=SelectedStartTime;
    hhProfile.EndTime=SelectedEndTime;
    hhProfile.DateTime = SelectedDateTime;
    hhProfile.IsEditedProfile=true;


    hhProfile.HappyHourProfileDays.clear();
    for(int day=eSunday; day<lbDay->Items->Count && day<eEndOfDays; day++)
    {

       if(lbDay->Checked[day])
       {
          hhProfile.HappyHourProfileDays.push_back(day);
       }

    }

    hhProfile.HappyHourProfileSettings.clear();
     if (cbMembersOnly->Checked)
    {
        hhProfile.HappyHourProfileSettings.push_back(eHappyHourIsForMembersOnly);
    }

    if (cbNoPointsEarnedDuring->Checked)
    {
        hhProfile.HappyHourProfileSettings.push_back(eNoPointsEarnedDuringHappyHour);
    }

    //insert the terminal keys

    hhProfile.ProfileTerminalsKeys.push_back(TDeviceRealTerminal::Instance().ID.DeviceKey);

    hhProfile.Pricelevel=0;// by default price level is base price
    hhProfile.Priority=0;

    //push the new profile in happy hour vector
    HappyHourProfiles.push_back(hhProfile);

    }

  }



//---------------------------------------------------------------------------

void __fastcall TfrmHappyHourSettings::lbDayClick(TObject *Sender)
{
  ProfileEdited =true;
  if(lbDay->Checked[lbDay->ItemIndex])
  {
    CheckNoOfHHProfile(lbDay->ItemIndex );
    lbDay->Font->Color = 0x000000 ;
  }
 if(!ValidateTime(false,false))return;
 if (EnableEdit)
       UpdateEditedProfile(CurrentSelectedProfileKey,HappyHourProfiles);


}
//---------------------------------------------------------------------------

void  TfrmHappyHourSettings::ResetForm()
 {
    EnableEdit=false;
    DateAndDayReset();
    ProfileInitialize(Now() , Now());
    cbMembersOnly->Checked = false;
    cbNoPointsEarnedDuring->Checked = false;
    EnableEdit=true;
}

//---------------------------------------------------------------------

void TfrmHappyHourSettings::DateAndDayReset()
{
    BtnDate->Caption = "Day";
    BtnMonth->Caption = "Month";
    BtnYear->Caption = "Year";
    for(int day=0; day<lbDay->Items->Count && day<eEndOfDays; day++)
    {
        lbDay->Checked[day] = false;
        lbDay->ItemEnabled[day]=true;
    }
    lbDay->Font->Color = 0x000000 ;
    lbDay->Enabled=true;
}
//---------------------------------------------------------------------------

 void  TfrmHappyHourSettings::DateBtnCheck()
{
    if(((BtnDate->Caption.Compare("Day") != 0 ) || (BtnMonth->Caption.Compare("Month") != 0 )  || (BtnYear->Caption.Compare("Year") != 0 )))
    {
        for(int i = 0 ; i<lbDay->Items->Count && i<eEndOfDays ; i++)
        {
            lbDay->ItemEnabled[i] = false;
            lbDay->Checked[i] = false;
        }
        lbDay->Font->Color = 0x808080 ;
        if(((BtnDate->Caption.Compare("Day") != 0 ) && (BtnMonth->Caption.Compare("Month") != 0 )  && (BtnYear->Caption.Compare("Year") != 0 )))
        {
            DayOfWeek();
        }
    }

 if(!ValidateTime(true,true))return;

 if(((BtnDate->Caption.Compare("Day") != 0 ) && (BtnMonth->Caption.Compare("Month") != 0 )  && (BtnYear->Caption.Compare("Year") != 0 )))
  {
   if( CalcHHProfilesinDate(SelectedDateTime)>=6)
    {
        MessageBox("You can add only 6 happy hours in a Day.", "Error", MB_OK + MB_ICONERROR);
        DateAndDayReset();
        return ;
    }
  }

   if (EnableEdit)
       UpdateEditedProfile(CurrentSelectedProfileKey,HappyHourProfiles);

   ProfileEdited=true;

}

//---------------------------------------------------------------------------

 void TfrmHappyHourSettings::AssignHHProfileDayValues(std::vector<int> HappyHourProfileDays )
 {
   int day;

  	for(std::vector<int>::iterator hhDayKey = HappyHourProfileDays.begin(); hhDayKey !=  HappyHourProfileDays.end(); hhDayKey++)
	  {
         day= *hhDayKey;

          switch(day)
          {
          	case 0:
                    lbDay->Checked[0]=true;
                    break;
            case 1:
                    lbDay->Checked[1]=true;
                    break;
            case 2:
                    lbDay->Checked[2]=true;
                    break;
            case 3:
                    lbDay->Checked[3]=true;
                    break;
            case 4:
                    lbDay->Checked[4]=true;
                    break;
            case 5:
                    lbDay->Checked[5]=true;
                    break;
            case 6:
                    lbDay->Checked[6]=true;
                    break;
          }
      }
 }
 //---------------------------------------------------------------------------

  void TfrmHappyHourSettings::AssignHHProfileDaySettings(std::vector<int> HappyHourProfileSettings)
  {
     int setting;

    for(std::vector<int>::iterator hhSettingKey = HappyHourProfileSettings.begin(); hhSettingKey !=  HappyHourProfileSettings.end(); hhSettingKey++)
	  {
          setting=*hhSettingKey;
          switch(setting)
           {
                case 1:
                        cbMembersOnly->Checked=true;
                        break;
                case 2:
                        cbNoPointsEarnedDuring->Checked =true;
                        break;
          }
      }
  }


bool TfrmHappyHourSettings::CheckNoOfHHProfile(int dayindex)
{
    int array[7]={0};
    int daycount;

    int day;
    for(int hhVal = 0; hhVal < HappyHourProfiles.size(); hhVal++)
    {
        for (std::vector<Integer>::iterator iter = HappyHourProfiles.at(hhVal).HappyHourProfileDays.begin() ; iter != HappyHourProfiles.at(hhVal).HappyHourProfileDays.end() ; iter++)
        {
             day=  *iter;

            switch(day)
            {
                case 0: //sunday
                         array[0]= array[0]+1;
                         break;
                case 1:
                         array[1]= array[1]+1;
                         break;
                case 2:
                         array[2]= array[2]+1;
                         break;
                case 3:
                          array[3]= array[3]+1;
                          break;
                case 4:
                          array[4]= array[4]+1;
                          break;
                case 5:
                           array[5]= array[5]+1;
                          break;
                case 6:  //saturday
                          array[6]= array[6]+1;
                          break;
              }

            }

           for(daycount =0; daycount<=6;daycount++)
           {
                if (array[daycount]>6 && (dayindex==daycount))
                {
                    return ShowMessage(dayindex);
                }
		   }
        }
   return true ;
 }

 bool TfrmHappyHourSettings::ShowMessage(int dayindex)
 {
       MessageBox("You can add only 6 happy hours in a Day.", "Error", MB_OK + MB_ICONERROR);
       lbDay->Checked[dayindex]=false;
       return false;
 }


  // below function is for reseting start and end time  values according to time stamp received and change time tn 12
  // hour format and set AM/PM accordingly.

  void TfrmHappyHourSettings::ResetHoursButtonValues(TDateTime currentTimeStamp)
  {
    BtnStartHour->Caption = HourOf(currentTimeStamp);
    BtnStartMinute->Caption = MinuteOf(currentTimeStamp);
    BtnStartAMPM->Caption = "AM" ;
    BtnEndHour->Caption = HourOf(currentTimeStamp);
    BtnEndMinute->Caption = MinuteOf(currentTimeStamp);
    BtnEndAMPM->Caption = "AM" ;
    int Hour = HourOf(currentTimeStamp);

    if(Hour >= 12)
    {
        BtnStartAMPM->Caption = "PM" ;
        BtnEndAMPM->Caption = "PM" ;
        if(Hour != 12)
        {
            BtnStartHour->Caption = IntToStr(Hour - 12);
            BtnEndHour->Caption = IntToStr(Hour - 12);
        }
    }
    if(Hour == 0)
    {
        BtnStartHour->Caption = IntToStr(12);
        BtnEndHour->Caption = IntToStr(12);
    }

  }


// Aswsigning the happy hour profies settings values to UI components
void TfrmHappyHourSettings::AssignHHProfileValues()
{
    TDateTime startTime ;
    TDateTime endTime ;
    TDateTime dateTime ;

    CurrentSelectedProfileKey=HappyHourProfiles[cbHappyHourProfile->ItemIndex].ProfileKey;
    startTime = HappyHourProfiles[cbHappyHourProfile->ItemIndex].StartTime;
    endTime = HappyHourProfiles[cbHappyHourProfile->ItemIndex].EndTime;
    dateTime = HappyHourProfiles[cbHappyHourProfile->ItemIndex].DateTime;

   EnableEdit=false;

    if(YearOf(dateTime)!= 1899)
    {
        BtnDate->Caption = DayOf(startTime);
        BtnMonth->Caption = MonthOf(startTime);
        BtnYear->Caption = YearOf(startTime);
        lbDay->Enabled =false;
        lbDay->Font->Color = 0x808080 ;
    }
    else
    {
        BtnDate->Caption = "Day";
        BtnMonth->Caption = "Month";
        BtnYear->Caption = "Year";
        lbDay->Enabled =true;
    }

    ProfileInitialize( startTime, endTime);  //call for AM or PM in buttons

    //assign day values
    AssignHHProfileDayValues(HappyHourProfiles[cbHappyHourProfile->ItemIndex].HappyHourProfileDays );


    //assign settings values
    AssignHHProfileDaySettings(HappyHourProfiles[cbHappyHourProfile->ItemIndex].HappyHourProfileSettings);


    csPriceLevel->ItemIndex= FindPriceLevelIndex( HappyHourProfiles[cbHappyHourProfile->ItemIndex].Pricelevel);

   // BtnPriority->Caption= HappyHourProfiles[cbHappyHourProfile->ItemIndex].Priority;
    if(HappyHourProfiles[cbHappyHourProfile->ItemIndex].Priority==0)
    {
       BtnPriority->Caption="Default";

    } else
    {
       BtnPriority->Caption= HappyHourProfiles[cbHappyHourProfile->ItemIndex].Priority;
    }

    EnableEdit=true;

    if(!ValidateTime(false,false))return;
    //if (EnableEdit)
   //    UpdateEditedProfile(CurrentSelectedProfileKey,HappyHourProfiles);



}

int TfrmHappyHourSettings::CalcHHProfilesinDate(TDateTime ProfileDate)
{

    int daycount=0;
    TValueRelationship  result;

    for(int hhVal = 0; hhVal < HappyHourProfiles.size(); hhVal++)
    {
       // matching the day when date is set
       if (YearOf(HappyHourProfiles.at(hhVal).DateTime)!=1899 )
       {

            if (DayOfTheWeek(ProfileDate) == DayOfTheWeek(HappyHourProfiles.at(hhVal).DateTime ))
            {
                  daycount++;
            }
         }
      else
       {
           // finding  the day in vector in when  date  is not set
          std::vector<int>::iterator dayindex;
          int selectedDay=DayOfTheWeek(ProfileDate);
          if (selectedDay==7)
          {
            selectedDay=0;
          }
          dayindex=  std::find(HappyHourProfiles.at(hhVal).HappyHourProfileDays.begin(),HappyHourProfiles.at(hhVal).HappyHourProfileDays.end(),selectedDay);

          if (dayindex != HappyHourProfiles.at(hhVal).HappyHourProfileDays.end())
          {
             daycount++;
          }
      }
   }
    return daycount;
}

//---------------------------------------------------------------------------
void __fastcall TfrmHappyHourSettings::cbHappyHourProfileDblClick(TObject *Sender)

{
//
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void __fastcall TfrmHappyHourSettings::csPriceLevelChange(TObject *Sender)
{
     if(!ValidateTime(false,false))return;
    ProfileEdited=true;

     if (EnableEdit)
       UpdateEditedProfile(CurrentSelectedProfileKey,HappyHourProfiles);

}



void __fastcall TfrmHappyHourSettings::BtnPriorityMouseClick(TObject *Sender)
{

        if (cbHappyHourProfile->ItemIndex == -1)
        {
           MessageBox("Please Select Happy Hour Profile First", "Error", MB_OK + MB_ICONERROR);
           return;
        }
        if (!( HappyHourProfiles[cbHappyHourProfile->ItemIndex].HappyHourProfileDays.size() >0))
        {
            MessageBox("Please select either date or atleast one day first", "Error", MB_OK + MB_ICONERROR);
            return;
        }


    std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
	frmTouchNumpad->Caption = "Enter Happy Hour Profile Priority";
	frmTouchNumpad->btnOk->Visible = true;
	frmTouchNumpad->Mode = pmNumber;
    if ( BtnPriority->Caption=="Default")
       frmTouchNumpad->INTInitial=0;
    else
      frmTouchNumpad->INTInitial = StrToInt(BtnPriority->Caption);

    if (frmTouchNumpad->ShowModal() == mrOk)
    {
      /* if (StrToInt(frmTouchNumpad->INTResult) ==0)
           {
             BtnPriority->Caption= "Default";
             return;
           }    */

        int selectedPriorityValue=StrToInt(frmTouchNumpad->INTResult);
        ProfileEdited=true;
        if (cbHappyHourProfile->ItemIndex>=0)
        {


            TDateTime selectedProfileDateTime = HappyHourProfiles[cbHappyHourProfile->ItemIndex].DateTime;
            TDateTime selectedProfileStartTime =HappyHourProfiles[cbHappyHourProfile->ItemIndex].StartTime;
            TDateTime selectedProfileEndTime =HappyHourProfiles[cbHappyHourProfile->ItemIndex].EndTime;

            int profileKey= HappyHourProfiles[cbHappyHourProfile->ItemIndex].ProfileKey;
            int currentProfileYear=YearOf(selectedProfileDateTime);

            int currentSelectedDay = DayOfTheWeek(selectedProfileStartTime);
            int currentProfileStartMinute=MinuteOfTheDay(selectedProfileStartTime);
            int currentProfileEndMinute=MinuteOfTheDay(selectedProfileEndTime);

            bool currentTimeSpanProfile;
            std::vector<int> currentTimeSpanProfiles;


            for (int profile =0;profile<HappyHourProfiles.size();profile++)
            {
             //excluding current selected profile
              if (profileKey !=  HappyHourProfiles[profile].ProfileKey)
              {

                currentTimeSpanProfile =false;
                // if date is set for the profile
                if(currentProfileYear!= 1899 && ( DateOf(selectedProfileEndTime)== DateOf(HappyHourProfiles[profile].EndTime) )  )
                {


                           if ( ( (double)selectedProfileStartTime >= (double) HappyHourProfiles[profile].StartTime )
                               && ((double)selectedProfileEndTime <= (double) HappyHourProfiles[profile].EndTime) )
                           {
                               currentTimeSpanProfile=true;

                           }
                           else if ( ( (double)selectedProfileStartTime >= (double) HappyHourProfiles[profile].StartTime )
                               && ((double)selectedProfileEndTime >= (double) HappyHourProfiles[profile].EndTime)
                               &&((double)selectedProfileStartTime <= (double) HappyHourProfiles[profile].EndTime) )
                           {
                             currentTimeSpanProfile=true;
                           }
                           else if ( ( (double)selectedProfileStartTime <= (double) HappyHourProfiles[profile].StartTime )
                               && ((double)selectedProfileEndTime >= (double) HappyHourProfiles[profile].StartTime) )
                           {
                             currentTimeSpanProfile=true;
                           }


                } else
                {
                  // int day =


                  for (int selectedDay=0;selectedDay< HappyHourProfiles[cbHappyHourProfile->ItemIndex].HappyHourProfileDays.size();selectedDay++)
                  {
                       currentSelectedDay = HappyHourProfiles[cbHappyHourProfile->ItemIndex].HappyHourProfileDays[selectedDay];

                       if(currentSelectedDay ==7)
                           currentSelectedDay =0;

                         //here HappyHourProfiles[cbHappyHourProfile->ItemIndex]
                         //and HappyHourProfiles[profile] are different
                          if ( HappyHourProfiles[profile].HappyHourProfileDays.size() >0 )
                          {
                               std::vector<int>::iterator pos;
                               pos= std::find(HappyHourProfiles[profile].HappyHourProfileDays.begin(),HappyHourProfiles[profile].HappyHourProfileDays.end(),currentSelectedDay);

                                if (pos!= HappyHourProfiles[profile].HappyHourProfileDays.end())
                                {

                                   if (( currentProfileStartMinute >= MinuteOfTheDay(HappyHourProfiles[profile].StartTime ) )
                                      && ( currentProfileEndMinute <= MinuteOfTheDay(HappyHourProfiles[profile].EndTime )  ) )
                                   {
                                         currentTimeSpanProfile=true;

                                   }else if (( currentProfileStartMinute >= MinuteOfTheDay(HappyHourProfiles[profile].StartTime ) )
                                      && ( currentProfileEndMinute >= MinuteOfTheDay(HappyHourProfiles[profile].EndTime )  )
                                      &&(currentProfileStartMinute <= MinuteOfTheDay(HappyHourProfiles[profile].EndTime )) )
                                   {
                                      currentTimeSpanProfile=true;
                                   }
                                   else if(( currentProfileStartMinute <= MinuteOfTheDay(HappyHourProfiles[profile].StartTime ) )
                                      && ( currentProfileEndMinute >= MinuteOfTheDay(HappyHourProfiles[profile].StartTime )  ) )
                                   {
                                      currentTimeSpanProfile=true;
                                   }

                                }
                          }

                  }
                }


                if (currentTimeSpanProfile)
                     currentTimeSpanProfiles.push_back(HappyHourProfiles[profile].Priority);
              }

            }

            std::vector<int>::iterator priorityPos;
            priorityPos=std::find(currentTimeSpanProfiles.begin(),currentTimeSpanProfiles.end(),selectedPriorityValue);

            if (priorityPos != currentTimeSpanProfiles.end() )
            {
                 MessageBox("Duplicate Priority Value.Please Enter Different Priority Value.", "Error", MB_OK + MB_ICONERROR);

                if (HappyHourProfiles[cbHappyHourProfile->ItemIndex].Priority == 0)
                    BtnPriority->Caption ="Default";
                else
                    BtnPriority->Caption= HappyHourProfiles[cbHappyHourProfile->ItemIndex].Priority;

            } else
            {
               BtnPriority->Caption = frmTouchNumpad->INTResult;
               BtnPriority->Caption =  StrToInt(BtnPriority->Caption);



            }

            if(!ValidateTime(false,false))return;

            UpdateEditedProfile(CurrentSelectedProfileKey,HappyHourProfiles);
        }


    }

}


 int TfrmHappyHourSettings::FindPriceLevelIndex(int PriceLevel)
 {
    std::map<AnsiString, int >::iterator priceval;
    for (priceval = ProfilePriceLevels.begin(); priceval != ProfilePriceLevels.end(); ++priceval)
     {
          if ( priceval->second ==  PriceLevel)
          {
            return csPriceLevel->Items->IndexOf(priceval->first);
          }
      }

    return 0;

 }


//---------------------------------------------------------------------------



