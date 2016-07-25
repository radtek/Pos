#ifndef ManagerHappyHourH
#define ManagerHappyHourH
#include "DBHappyHour.h"
#include "Vector.h"

class TManagerHappyHour
{
public:

    //load profile information which are available ..
    std::vector<THappyHour> LoadHappyHourProfiles(bool allTerminalsHHProfile);

    void HappyHourProfileSaveAndUpdate(std::vector<THappyHour> happyHourProfiles);

    //save the new profile..
    void SaveHappyHourProfile(Database::TDBTransaction &DBTransaction,THappyHour  *happyHourProfile);

    //update existing profile..
    void UpdateHappyHourProfile(Database::TDBTransaction &DBTransaction,THappyHour  *happyHourProfile);

    //setflag when particular profile deleted..
    void DeleteHappyHourProfile(int profile_key);

	//check whether current Time exist undet happyhour
    bool IsCurrentTimeHappyHour(int deviceKey,int &priceLevel);


    //gets all the terminals key
    std::vector<int> getTerminalsKey();

    //delete  profiles terminal mapping
    void DeleteHHProfileTerminalMapping(int profileKey,int terminalKey);

    //set happyhour settings according to profiles
    void SetHappyHourDaysSettings(std::vector<int> HappyHourProfileSettings);


    int ValidationOfProfiles(std::vector<THappyHour> happyHourProfiles);

    THappyHour CopyProfile(std::vector<THappyHour> happyHourProfiles, int index);
    int CalcNoOfProfiles();

    void   ProfileBroadcast(std::vector<THappyHour> &HappyHourProfiles,int &MaxHHProfileKey,int &NewMaxNoHHProfileAdded,bool message_flag,std::vector<THappyHour> &DeletedHappyHourProfiles);

    std::map<AnsiString,int> loadAllPriceLevels();


    int CheckAllHHProfilesPriorities(std::vector<THappyHour> &HappyHourProfiles);


   bool CheckAllHHProfilesName(std::vector<THappyHour> HappyHourProfiles,AnsiString profileName,std::vector<int> &HHtoBeDeleted );



};
#endif
