//---------------------------------------------------------------------------

#ifndef TaxProfileControllerH
#define TaxProfileControllerH

#include "I_TaxProfileGUI.h"
#include "TaxProfile.h"
#include "TaxProfileManager.h"


#include <memory>
#include <vector>

class I_TfrmTaxProfile;
class TaxProfileController
{
   public:
	  TaxProfileController(I_TfrmTaxProfile* gui);
	  TaxProfileController();
      ~TaxProfileController();
      void ShowView();
      void AddNewTaxProfile();
      void DeleteTaxProfile(TaxProfile* profile);
      void EditTaxProfile(TaxProfile* profile);
	  bool FindTaxProfile(int profileKey);
	  TaxProfile* GetTaxProfile(int profileKey);
      bool IsDuplicateTaxProfileExists(int currentTaxProfileKey, AnsiString taxProfileName, TaxType type);

      void UpdateTaxProfile(TaxProfile* guiTaxProfile);

      bool ProceedSaveTaxProfiles;
      bool SaveTaxProfiles();
	  void GetAllModifications(TaxProfileMap* destination);
	  std::map<int, TaxProfile*>  GetAllTaxProfiles();

   private:
      I_TfrmTaxProfile* taxGUI;
	  std::map<int,TaxProfile*>  taxProfiles;
      TaxProfile* selectedTaxProfile;
      int _newTaxProfileKey;

      void loadView();
      void loadTaxProfilesFromDB();
      void populateTaxProfileMap(TaxProfileMap* dbMap);
      int getNextNewTaxProfileKey();
      void insertTaxProfileToMap(TaxProfile *taxProfile);
};

#endif
