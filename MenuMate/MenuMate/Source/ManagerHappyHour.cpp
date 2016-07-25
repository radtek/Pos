#include "ManagerHappyHour.h"
#include "DBHappyHour.h"
#include "MM_DBCore.h"
#include "DeviceRealTerminal.h"
#include "DateUtils.hpp"

std::vector<THappyHour> TManagerHappyHour::LoadHappyHourProfiles(bool allTerminalsHHProfile)
{
        //Register the database transaction..
        Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
        TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
        dbTransaction.StartTransaction();

    try
    {
        //loading the combobox with all profiles.
        std::vector<THappyHour> happyHourProfiles;
        happyHourProfiles.clear();
        happyHourProfiles = TDBHappyHour::GetAvailableHappyHourProfile(dbTransaction,allTerminalsHHProfile);

        dbTransaction.Commit();

        return happyHourProfiles;
    }
    catch(Exception &Err)
    {
        dbTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
        throw;
    }
}


 std::map<AnsiString,int> TManagerHappyHour:: loadAllPriceLevels()
 {
      //Register the database transaction..
        Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
        TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
        dbTransaction.StartTransaction();

    try
    {
        //loading the combobox with all pricelevels.

        std::map<AnsiString,int> allPriceLevels;


       allPriceLevels = TDBHappyHour::GetAllPriceLevels(dbTransaction);

        dbTransaction.Commit();

        return allPriceLevels;
    }
    catch(Exception &Err)
    {
        dbTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
        throw;
    }
 }

//---------------------------------------------------------------------------
void TManagerHappyHour::HappyHourProfileSaveAndUpdate(std::vector<THappyHour> HappyHourProfiles)
{
     bool profilePresent=false;
     int DaySelectedNo=0;

    //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();

    try
    {
        for(int hhVal = 0; hhVal < HappyHourProfiles.size(); hhVal++)
        {
                // check if profile is there in database
                profilePresent= TDBHappyHour::IsHHProfilePresent(dbTransaction, HappyHourProfiles[hhVal].ProfileKey);

                THappyHour*  hhProfileInfo = new THappyHour();
                hhProfileInfo->StartTime=HappyHourProfiles[hhVal].StartTime;
                hhProfileInfo->EndTime=HappyHourProfiles[hhVal].EndTime;
                hhProfileInfo->DateTime=HappyHourProfiles[hhVal].DateTime;
                hhProfileInfo->ProfileName=HappyHourProfiles[hhVal].ProfileName;

                hhProfileInfo->ProfileKey=HappyHourProfiles[hhVal].ProfileKey;

               //saving the days
                for(std::vector<int>::iterator hhDayKey = HappyHourProfiles[hhVal].HappyHourProfileDays.begin(); hhDayKey !=  HappyHourProfiles[hhVal].HappyHourProfileDays.end(); hhDayKey++)
                {
                   hhProfileInfo->HappyHourProfileDays.push_back(*hhDayKey);
                }

                // saving the settings

               for(std::vector<int>::iterator hhSettingKey = HappyHourProfiles[hhVal].HappyHourProfileSettings.begin(); hhSettingKey !=  HappyHourProfiles[hhVal].HappyHourProfileSettings.end(); hhSettingKey++)
                {
                   hhProfileInfo->HappyHourProfileSettings.push_back(*hhSettingKey);
                }
                //saving the terminal key


                for(std::vector<int>::iterator iter = HappyHourProfiles[hhVal].ProfileTerminalsKeys.begin(); iter !=  HappyHourProfiles[hhVal].ProfileTerminalsKeys.end(); iter++)
                {
                  hhProfileInfo->ProfileTerminalsKeys.push_back(*iter);

                }


                hhProfileInfo->Pricelevel=  HappyHourProfiles[hhVal].Pricelevel;
                hhProfileInfo->Priority =  HappyHourProfiles[hhVal].Priority;



                if (profilePresent)
                {
                   UpdateHappyHourProfile(dbTransaction,hhProfileInfo);    //update if profile already present
                }
                else
                {
                   SaveHappyHourProfile(dbTransaction,hhProfileInfo);   //insert  if profile not  present
                }

                delete hhProfileInfo;

        }

      dbTransaction.Commit();

 }
 catch(Exception &Err)
 {
    dbTransaction.Rollback();
    TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
    throw;
  }

}
//---------------------------------------------------------------------------

void TManagerHappyHour::SaveHappyHourProfile(Database::TDBTransaction &dBTransaction,THappyHour* happyHourProfiles)
{
    //save New profile info
   // TDBHappyHour::SaveHappyHourInfo(dBTransaction, HappyHourProfiles);
      int profileKey = TDBHappyHour::CreateProfileKey(dBTransaction );
      //save profiles into DB
      TDBHappyHour::SaveProfile(dBTransaction, happyHourProfiles, profileKey);
      //savedays info into DB
      TDBHappyHour::SaveProfileDays(dBTransaction, happyHourProfiles, profileKey);
      // save settings info into DB
      TDBHappyHour::SaveProfileSettings(dBTransaction, happyHourProfiles, profileKey);
      // save terminal Mapping into DB
      TDBHappyHour::SaveTerminalMapping(dBTransaction, happyHourProfiles, profileKey);

      // save the profile priority
      TDBHappyHour::SaveProfilePriority(dBTransaction, happyHourProfiles, profileKey);


}
//---------------------------------------------------------------------------

void TManagerHappyHour::UpdateHappyHourProfile(Database::TDBTransaction &dbTransaction,THappyHour  *HappyHourProfile)
{
    //save exixting profile editing info
   TDBHappyHour::UpdateProfile(dbTransaction, HappyHourProfile);
   TDBHappyHour::UpdateProfileDays(dbTransaction, HappyHourProfile);
   TDBHappyHour::UpdateProfileSettings(dbTransaction, HappyHourProfile);
   TDBHappyHour::UpdateTerminalMapping(dbTransaction, HappyHourProfile);
   TDBHappyHour::UpdateProfilePriority(dbTransaction, HappyHourProfile);

}
//---------------------------------------------------------------------------

void TManagerHappyHour::DeleteHappyHourProfile(int profileKey)
{

     //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    try
    {
        dbTransaction.StartTransaction();

        //Delete the profile using delete button on UI
        TDBHappyHour::DeleteProfile(dbTransaction, profileKey);
        //delete the selected profile settings
        TDBHappyHour::DeleteProfileSettings(dbTransaction, profileKey);
        //delete selected profile days from table because it is mapping table
        TDBHappyHour::DeleteProfileDays(dbTransaction, profileKey);
        //delete terminal mapping of that profile
        TDBHappyHour::DeleteTerminalMapping(dbTransaction, profileKey);
        //delete haapy hour priority
        TDBHappyHour::DeleteprofilePriority(dbTransaction, profileKey);

        dbTransaction.Commit();
    }
    catch(Exception &Err)
    {
        dbTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
        throw;
    }
}
//---------------------------------------------------------------------------

//check whether current time fall ubnder any profile on that terminal

bool TManagerHappyHour::IsCurrentTimeHappyHour(int deviceKey,int &priceLevel)
{
    bool happyhour = false;

    //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();

    try
    {
        std::vector<THappyHour *> happyHourTimes;

        //get all profiles which fall under current current time
        happyHourTimes = TDBHappyHour::GetHappyHourTime(dbTransaction, deviceKey);
        dbTransaction.Commit();
        int index =0;


        if(happyHourTimes.size() > 0)
        {  int  priority =happyHourTimes[0]->Priority;

            for (int profile =0; profile< happyHourTimes.size(); profile++)
            {
               AnsiString name= happyHourTimes[profile]->ProfileName;

                if(happyHourTimes[profile]->Priority <= priority)
                {
                   priority = happyHourTimes[profile]->Priority;
                   index =profile;
                }
            }
            happyhour = true;

            //set the days setting values according to prfile
            SetHappyHourDaysSettings(happyHourTimes[index]->HappyHourProfileSettings);

            priceLevel= happyHourTimes[index]->Pricelevel;
        }
        return happyhour;
    }
    catch(Exception &Err)
    {
        dbTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
        throw;
    }
}


std::vector<int> TManagerHappyHour:: getTerminalsKey()
{
    std::vector<int> terminalsKeys;

    //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();

   try
    {
        //get all terminal keys
        terminalsKeys = TDBHappyHour::GetDevicesKeys(dbTransaction);
        dbTransaction.Commit();

        return terminalsKeys;
    }
    catch(Exception &Err)
    {
        dbTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
        throw;
    }
}


void TManagerHappyHour::DeleteHHProfileTerminalMapping(int profileKey, int terminalKey)
{
    //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();
    try
    {
        // Delete the  profiles mapping...
        TDBHappyHour::DeleteTerminalMapping(dbTransaction, profileKey, terminalKey);
        dbTransaction.Commit();

    }
    catch(Exception &Err)
    {
        dbTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
        throw;
    }
}

//------------------------------------------------------------------------------

///set the global settings according for all profiles which fall under current time

void TManagerHappyHour::SetHappyHourDaysSettings(std::vector<int> HappyHourProfileSettings)
{
    int settingVal;

  try
  {

        TGlobalSettings::Instance().HappyHourMembersOnly =  false;
        TGlobalSettings::Instance().NoPtsEarnedDuringHappyHour =  false;

        for(std::vector<int>::iterator hhSettingKey = HappyHourProfileSettings.begin(); hhSettingKey !=  HappyHourProfileSettings.end(); hhSettingKey++)
        {
          settingVal = *hhSettingKey;
          switch(settingVal)
           {

                case 1:
                        TGlobalSettings::Instance().HappyHourMembersOnly = true;
                        break;
                case 2:
                        TGlobalSettings::Instance().NoPtsEarnedDuringHappyHour = true;
                        break;
            }
        }

    }
 catch(Exception &Err)
 {
    TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
    throw;
  }

}

int  TManagerHappyHour::ValidationOfProfiles(std::vector<THappyHour> HappyHourProfiles)
 {
    //check before saving profile one must select eiether date or day/days. and return accordingly to that profile.
    for(int hhVal = 0; hhVal < HappyHourProfiles.size(); hhVal++)
    {
         if(HappyHourProfiles.at(hhVal).HappyHourProfileDays.size()==0)
         {
            MessageBox("You Must Select Atleast one Day Or Date.", "Error", MB_OK + MB_ICONERROR);
            return HappyHourProfiles.at(hhVal).ProfileKey;

         }else if (HappyHourProfiles.at(hhVal).ProfileName.Length()<= 0)
         {
           MessageBox("Please Enter Profile Name.", "Error", MB_OK + MB_ICONERROR);
           return HappyHourProfiles.at(hhVal).ProfileKey;
         }else if (HappyHourProfiles.at(hhVal).Pricelevel ==0)
         {
            MessageBox("Please Select Price Level Name.", "Error", MB_OK + MB_ICONERROR);
            return HappyHourProfiles.at(hhVal).ProfileKey;
         }else if((double)HappyHourProfiles.at(hhVal).StartTime == (double)HappyHourProfiles.at(hhVal).EndTime )
         {
           MessageBox("Profile Start Time And End Time Cannot be same.", "Error", MB_OK + MB_ICONERROR);
            return HappyHourProfiles.at(hhVal).ProfileKey;
         }

     }
     return -1;
 }

 //copy the profile to another profile
 THappyHour TManagerHappyHour::CopyProfile(std::vector<THappyHour> happyHourProfiles, int index)
 {
    THappyHour hhProfile;

    hhProfile.ProfileKey=happyHourProfiles.at(index).ProfileKey;
    hhProfile.ProfileName=happyHourProfiles.at(index).ProfileName;


    hhProfile.StartTime=happyHourProfiles.at(index).StartTime;
    hhProfile.EndTime=happyHourProfiles.at(index).EndTime;
    hhProfile.DateTime = happyHourProfiles.at(index).DateTime;


    hhProfile.HappyHourProfileDays.clear();


    std::vector<Integer>::iterator iter;
    for ( iter = happyHourProfiles.at(index).HappyHourProfileDays.begin() ; iter != happyHourProfiles.at(index).HappyHourProfileDays.end() ; iter++)
    {
      hhProfile.HappyHourProfileDays.push_back(*iter);
    }

    hhProfile.HappyHourProfileSettings.clear();
    for (iter = happyHourProfiles.at(index).HappyHourProfileSettings.begin() ; iter != happyHourProfiles.at(index).HappyHourProfileSettings.end() ; iter++)
    {
      hhProfile.HappyHourProfileSettings.push_back(*iter);
    }

    hhProfile.ProfileTerminalsKeys.clear();
    for (iter = happyHourProfiles.at(index).ProfileTerminalsKeys.begin() ; iter != happyHourProfiles.at(index).ProfileTerminalsKeys.end() ; iter++)
     {
       hhProfile.ProfileTerminalsKeys.push_back(*iter);

     }

     hhProfile.Pricelevel=  happyHourProfiles.at(index).Pricelevel;
     hhProfile.Priority=  happyHourProfiles.at(index).Priority;
     hhProfile.TerminalKey=  happyHourProfiles.at(index).TerminalKey;


    return  hhProfile;

 }

 //calculte no of profiles saved in database
 int TManagerHappyHour::CalcNoOfProfiles()
 {
    int profilesNumber=0;
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();
    try
    {

        profilesNumber=TDBHappyHour::MaxProfilesNo(dbTransaction);
        dbTransaction.Commit();


        return profilesNumber;

    }
    catch(Exception &Err)
    {
    TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
    throw;
    }


 }

//code for broadcasting
 void   TManagerHappyHour::ProfileBroadcast(std::vector<THappyHour> &HappyHourProfiles,int &MaxHHProfileKey,int &NewMaxNoHHProfileAdded,bool message_flag,std::vector<THappyHour> &DeletedHappyHourProfiles)
 {

    std::vector<int> Devices_key;
    std::vector<int> profileKeysToBeDeleted;
    std::vector<int> HHtoBeDeleted;
    std::vector<int> terminalKeys;

    int itemIndex=0;
    int noOfProfiles=0;
    std::vector<int> HappyHourProfileKeys;
    THappyHour hhProfile;

    bool duplicateNamePresent;

   if (message_flag)
   {
      /// delete the profiles for all terminals
      for(int hhVal = 0; hhVal < DeletedHappyHourProfiles.size(); hhVal++)
       {
          DeleteHappyHourProfile(DeletedHappyHourProfiles.at(hhVal).ProfileKey);
       }

   }


    //get all the pos terminals keys   in vector
    Devices_key= getTerminalsKey();
    noOfProfiles=HappyHourProfiles.size();

       //if want  happy hour to apply on all terminals message flag will be true
        if (message_flag)
        {
            for(int hhVal = 0; hhVal < noOfProfiles; hhVal++)
             {
            //     updateAll=false;
                 duplicateNamePresent=false;
               if(HappyHourProfiles.at(hhVal).IsEditedProfile)
               {
                    ///check for duplicate happy hour profile name in connected terminals
                  if(  CheckAllHHProfilesName(HappyHourProfiles,HappyHourProfiles.at(hhVal).ProfileName,HHtoBeDeleted ) )
                  {
                       duplicateNamePresent=true;
                      // delete all the happy hour which have same name
                      for(int profileKey = 0; profileKey < HHtoBeDeleted.size(); profileKey++)
                         {
                              DeleteHappyHourProfile(HHtoBeDeleted[profileKey]);
                         }
                  }

                    if(duplicateNamePresent)
                    {
                        HappyHourProfiles.at(hhVal).ProfileKey= (MaxHHProfileKey + NewMaxNoHHProfileAdded) + 1;
                        NewMaxNoHHProfileAdded+=1;  // static variable
                        HappyHourProfiles.at(hhVal).ProfileTerminalsKeys.clear();
                        Devices_key.clear();
                        Devices_key= getTerminalsKey();
                        HappyHourProfiles.at(hhVal).ProfileTerminalsKeys.clear();
                        for(std::vector<int>::iterator deviceKey = Devices_key.begin(); deviceKey !=  Devices_key.end(); deviceKey++)
                        {
                          HappyHourProfiles.at(hhVal).ProfileTerminalsKeys.push_back(*deviceKey);
                        }

                    }
                    else
                    {
                         HappyHourProfiles.at(hhVal).ProfileTerminalsKeys.clear();
                        for(std::vector<int>::iterator deviceKey = Devices_key.begin(); deviceKey !=  Devices_key.end(); deviceKey++)
                        {
                           HappyHourProfiles.at(hhVal).ProfileTerminalsKeys.push_back(*deviceKey);
                        }
                   }
               }
           }
       }
        else
        {
            // if no is pressed than prfile will be made only for that terminal
            for(int hhVal = 0; hhVal < noOfProfiles; hhVal++)
             {
                  if(HappyHourProfiles.at(hhVal).IsEditedProfile && (HappyHourProfiles.at(hhVal).ProfileKey <=MaxHHProfileKey))
                     {
                        // delete the terminal mapping from old   profile
                        DeleteHHProfileTerminalMapping(HappyHourProfiles.at(hhVal).ProfileKey, TDeviceRealTerminal::Instance().ID.DeviceKey);

                        // assign a new profile key .
                        HappyHourProfiles.at(hhVal).ProfileKey= (MaxHHProfileKey + NewMaxNoHHProfileAdded) + 1;
                        NewMaxNoHHProfileAdded+=1;  // static variable

                        HappyHourProfiles.at(hhVal).ProfileTerminalsKeys.clear();
                        HappyHourProfiles.at(hhVal).ProfileTerminalsKeys.push_back(TDeviceRealTerminal::Instance().ID.DeviceKey);
                  }
              }
       }
 }


int TManagerHappyHour::CheckAllHHProfilesPriorities(std::vector<THappyHour> &HappyHourProfiles)
  {

        // to check if same time span  profiles have same priority
    for(int hhVal = 0; hhVal < HappyHourProfiles.size(); hhVal++)
    {
            TDateTime selectedProfileDateTime = HappyHourProfiles.at(hhVal).DateTime;
            TDateTime selectedProfileStartTime =HappyHourProfiles.at(hhVal).StartTime;
            TDateTime selectedProfileEndTime =HappyHourProfiles.at(hhVal).EndTime;

            int profileKey= HappyHourProfiles.at(hhVal).ProfileKey;
            int currentProfileYear=YearOf(selectedProfileDateTime);

            int currentSelectedDay ;
            int currentProfileStartMinute=MinuteOfTheDay(selectedProfileStartTime);
            int currentProfileEndMinute=MinuteOfTheDay(selectedProfileEndTime);

            bool currentTimeSpanProfile;
            std::vector<int> currentTimeSpanProfiles;
            int  currentPriority=HappyHourProfiles.at(hhVal).Priority;


            for (int profile =0;profile<HappyHourProfiles.size();profile++)
            {
             //excluding current selected profile
              if (profileKey !=  HappyHourProfiles[profile].ProfileKey )
              {

                currentTimeSpanProfile =false;
                // if date is set for the profile
                if(currentProfileYear!= 1899 && ( DateOf(selectedProfileEndTime)== DateOf(HappyHourProfiles[profile].EndTime) ))
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

              //here HappyHourProfiles[cbHappyHourProfile->ItemIndex]
              //and HappyHourProfiles[profile] are different
                      if ( HappyHourProfiles[profile].HappyHourProfileDays.size() >0 )
                       {
                          for(int Day=0; Day< HappyHourProfiles.at(hhVal).HappyHourProfileDays.size(); Day++ )
                          {
                               currentSelectedDay= HappyHourProfiles.at(hhVal).HappyHourProfileDays[Day];
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
                    if (currentTimeSpanProfile )
                    // if (currentTimeSpanProfile )
                      {
                           if (currentPriority ==  HappyHourProfiles[profile].Priority )
                           {
                              //now when priority and time is common then check whether both profile have current terminal key mapping
                              //checking the source happy hour profile which is getting compared

                              bool terminalKeyinSource=false;
                              bool terminalKeyinDest=false;
                              for (int terminalKey=0; terminalKey< HappyHourProfiles.at(hhVal).ProfileTerminalsKeys.size(); terminalKey++)
                              {
                                  if(HappyHourProfiles.at(hhVal).ProfileTerminalsKeys[terminalKey]== TDeviceRealTerminal::Instance().ID.DeviceKey )
                                  {
                                    terminalKeyinSource=true;
                                    break;
                                  }
                              }
                              //checking the dest happy hour profile to which source happy hour is compared
                             for (int terminalKey=0; terminalKey< HappyHourProfiles[profile].ProfileTerminalsKeys.size(); terminalKey++)
                               {
                                  if(HappyHourProfiles[profile].ProfileTerminalsKeys[terminalKey]== TDeviceRealTerminal::Instance().ID.DeviceKey )
                                  {
                                    terminalKeyinDest=true;
                                    break;
                                  }
                               }

                              if(terminalKeyinSource && terminalKeyinDest )
                              {
                                   MessageBox("Duplicate Priority Value.Please Enter Different Priority Value.", "Error", MB_OK + MB_ICONERROR);
                                   return HappyHourProfiles.at(hhVal).ProfileKey;
                                   break;
                              }
                          }
                      }
              }

            }
     }

    return -1;
  }



 bool TManagerHappyHour::CheckAllHHProfilesName(std::vector<THappyHour> HappyHourProfiles,AnsiString profileName,std::vector<int> &HHtoBeDeleted )
  {
        bool duplicatePresent=false;
        std::vector<THappyHour> allTerminalsHappyHourProfiles;
        std::vector<THappyHour> CopyHappyHourProfiles;
        CopyHappyHourProfiles = HappyHourProfiles;

        // load all the terminals happy hour profiles
        allTerminalsHappyHourProfiles=LoadHappyHourProfiles(true);
       for(int profile=0; profile < allTerminalsHappyHourProfiles.size();profile++)
       {
           CopyHappyHourProfiles.push_back(allTerminalsHappyHourProfiles[profile]);
       }

         for(int profile=0; profile < allTerminalsHappyHourProfiles.size();profile++)
          {
                 if(LowerCase(profileName) == LowerCase(allTerminalsHappyHourProfiles[profile].ProfileName))
                   {  duplicatePresent=true;
                   }
          }

         if(duplicatePresent)
         {

            for(int profile=0; profile < CopyHappyHourProfiles.size();profile++)
             {
                 if(LowerCase(profileName) == LowerCase(CopyHappyHourProfiles[profile].ProfileName))
                   {
                    HHtoBeDeleted.push_back(CopyHappyHourProfiles[profile].ProfileKey);
                   }
             }
           return true;
         }

      return false;
 }





