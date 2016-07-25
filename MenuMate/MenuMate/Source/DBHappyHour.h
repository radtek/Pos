#ifndef DBHappyHourH
#define DBHappyHourH
#include "GlobalSettings.h"
#include "Vector.h"
#include "MM_DBCore.h"



enum eHappyHourSettings {
                      eHappyHourIsForMembersOnly=1,eNoPointsEarnedDuringHappyHour
                     };

class THappyHour
{
 private:
    TDateTime _startTime;
    TDateTime _endTime;
    TDateTime _dateTime;
    AnsiString _profileName;
    int _profileKey;
    bool _isEditedProfile;
    std::vector<int> _happyHourProfileDays;
    std::vector<int> _happyHourProfileSettings;
    std::vector<int> _profileTerminalsKeys;
    int  _profilePricelevel;
    int _priority;
    int _terminalKey;

    void SetStartTime(TDateTime startTime);
    void SetEndTime(TDateTime endTime);
    void SetDateTime(TDateTime dateTime);
    void SetProfileName(AnsiString profileName);
    void SetProfileKey(int profileKey);
    void SetEditedProfile(bool isEditedProfile);
    void SetHappyHourProfileDays(std::vector<int> happyHourProfileDays);
    void SetHappyHourProfileSettings(std::vector<int> happyHourProfileSettings);
    void SetProfileTerminalKeys(std::vector<int> profileTerminalsKeys);
    void SetProfilePricelevel(int profilePricelevel);
    void SetProfilePriority(int Priority);
    void SetProfileTerminalKey(int TerminalKey);

public:
    __property TDateTime StartTime = {read = _startTime, write = SetStartTime};
    __property TDateTime EndTime = {read = _endTime, write = SetEndTime};
    __property TDateTime DateTime = {read = _dateTime, write = SetDateTime};
    __property AnsiString ProfileName = {read = _profileName, write = SetProfileName};
    __property int ProfileKey = {read = _profileKey, write = SetProfileKey};
    __property bool IsEditedProfile = {read = _isEditedProfile, write = SetEditedProfile};
    __property std::vector<int> HappyHourProfileDays = {read = _happyHourProfileDays, write = SetHappyHourProfileDays };
    __property std::vector<int> HappyHourProfileSettings = {read = _happyHourProfileSettings, write = SetHappyHourProfileSettings};
    __property std::vector<int> ProfileTerminalsKeys = {read = _profileTerminalsKeys, write = SetProfileTerminalKeys};
    __property int Pricelevel = {read = _profilePricelevel, write = SetProfilePricelevel};
    __property int Priority = {read = _priority, write = SetProfilePriority};
    __property int TerminalKey = {read = _terminalKey, write = SetProfileTerminalKey};

};

class TDBHappyHour
{
public:

    static std::vector<THappyHour> GetAvailableHappyHourProfile(Database::TDBTransaction &dBTransaction,bool allTerminalsHHProfile);
    static std::vector<THappyHour *> GetHappyHourTime(Database::TDBTransaction &dBTransaction, int terminalKey);
    static int CreateProfileKey(Database::TDBTransaction &dBTransaction );

    static void SaveProfile(Database::TDBTransaction &dBTransaction,THappyHour* happyHourProfiles, int profileKey);
    static void SaveProfileDays(Database::TDBTransaction &dBTransaction,THappyHour* happyHourProfiles, int profileKey);
    static void SaveProfileSettings(Database::TDBTransaction &dBTransaction,THappyHour* happyHourProfiles, int profileKey);
    static void SaveTerminalMapping(Database::TDBTransaction &dBTransaction,THappyHour* happyHourProfiles, int profileKey);
    static void SaveProfilePriority(Database::TDBTransaction &dBTransaction,THappyHour* happyHourProfiles, int profileKey);
    static void UpdateProfile(Database::TDBTransaction &dBTransaction, THappyHour* happyHourProfile);
    static void UpdateProfileDays(Database::TDBTransaction &dBTransaction, THappyHour* happyHourProfile);
    static void UpdateProfileSettings(Database::TDBTransaction &dBTransaction, THappyHour* happyHourProfile);
    static void UpdateTerminalMapping(Database::TDBTransaction &dBTransaction, THappyHour* happyHourProfile);
    static void UpdateProfilePriority(Database::TDBTransaction &dBTransaction, THappyHour* happyHourProfile);
    static void DeleteProfile(Database::TDBTransaction &dBTransaction, int profile_key);
    static void DeleteProfileSettings(Database::TDBTransaction &dBTransaction, int profile_key);
    static void DeleteProfileDays(Database::TDBTransaction &dBTransaction, int profile_key);
    static void DeleteTerminalMapping(Database::TDBTransaction &dBTransaction, int profile_key);
    static void DeleteprofilePriority(Database::TDBTransaction &dBTransaction, int profile_key);
    static bool IsHHProfilePresent(Database::TDBTransaction &dBTransaction, int profile_key);
    static std::vector<int> GetDevicesKeys(Database::TDBTransaction &dBTransaction);
    static void DeleteTerminalMapping(Database::TDBTransaction &dBTransaction,int profileKey, int terminalKey);
    static int MaxProfilesNo(Database::TDBTransaction &dbTransaction);
    static std::map<AnsiString,int> GetAllPriceLevels(Database::TDBTransaction &dBTransaction);


  
};
#endif

