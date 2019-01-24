// ---------------------------------------------------------------------------

#ifndef ManagerSyndCodeH
#define ManagerSyndCodeH

#include <system.hpp>
#include <vector>
#include "MM_DBCore.h"
// ---------------------------------------------------------------------------

class TSyndCode
{
public:
   TSyndCode(int SyndCodeKey, AnsiString inName, AnsiString inSyndCode, bool inEnabled,
             bool inEncryptCode, AnsiString inOriginalSyndCode,bool inUseForCom);
   TSyndCode();

   int SyndCodeKey;
   AnsiString Name;
   AnsiString DecryptedSyndCode;
   AnsiString OriginalSyndCode;
   bool DefaultEncryptCode;
   bool Enabled;
   TDateTime ValidFrom;
   TDateTime ValidTo;
   AnsiString GetSyndCode();
   bool Valid();
   bool UseForCom;
};

class TManagerSyndCode
{
private:
   std::map <int, TSyndCode> SyndCodes; // Unscrabled SyndCodes.
   std::map <int, TSyndCode> ::iterator ptrSyndCodes;

   int CurrentKey;

   void LoadCodes(Database::TDBTransaction &DBTransaction);
   void UpdateEncryptCode(Database::TDBTransaction &DBTransaction, TSyndCode SyndCode);


public:
   TManagerSyndCode();
   AnsiString Decrypt(AnsiString Data);
   AnsiString Encrypt(AnsiString Data);
   void Initialise(Database::TDBTransaction &DBTransaction);
   void AddCode(Database::TDBTransaction &DBTransaction, TSyndCode SyndCode);
   void RemoveCode(Database::TDBTransaction &DBTransaction, int SyndKey);
   void UpdateCode(Database::TDBTransaction &DBTransaction, TSyndCode SyndCode);
   TSyndCode GetDefaultSyndCode();
   TSyndCode GetCommunicationSyndCode();
   std::map <int, TSyndCode> GetSyndCodes(bool EnabledCodesOnly);
   AnsiString FindCode(Database::TDBTransaction &DBTransaction, AnsiString Code); // Returns the Codes Name;
   void ExportToFile(Database::TDBTransaction &DBTransaction, int Key, AnsiString FileName);
   void ImportFromFile(Database::TDBTransaction &DBTransaction, int Key, AnsiString FileName);
   void First(bool EnabledCodesOnly = true);
   void Next(bool EnabledCodesOnly = true);
   bool Eof();
   bool Bof();
   bool Empty();
   int Size();
   TSyndCode &SyndCode();
   TSyndCode &SyndCodeByKey(int SyndKey);
   bool ValidateSyndCodes(AnsiString& errorMessage);
   bool CanUseForCommunication(int syndCodeKey);
   AnsiString GetCommunicationSyndCodeString();
   bool CheckIfAnySynCodeEnabled();
   bool CheckIfSynCodeEnabled(int key);
};

#endif
