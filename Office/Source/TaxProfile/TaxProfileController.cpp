//---------------------------------------------------------------------------


#pragma hdrstop

#include "TaxProfileController.h"

//---------------------------------------------------------------------------


TaxProfileController::TaxProfileController(I_TfrmTaxProfile* gui):
   taxGUI(gui), ProceedSaveTaxProfiles(false), _newTaxProfileKey(-1)
{
   loadTaxProfilesFromDB();
   taxGUI->SetController(this);  
}

TaxProfileController::TaxProfileController():ProceedSaveTaxProfiles(false), _newTaxProfileKey(-1)
{
   loadTaxProfilesFromDB();  
}

TaxProfileController::~TaxProfileController()
{
   taxProfiles.clear();
}

void TaxProfileController::ShowView()
{
   loadView();
   taxGUI->Display();
}

void TaxProfileController::AddNewTaxProfile()
{
   // create new tax profile with the next key
   TaxProfile* profile = new TaxProfile(getNextNewTaxProfileKey());

   insertTaxProfileToMap( profile );
   taxGUI->AddTaxProfileToGrid(*profile);
}

void TaxProfileController::DeleteTaxProfile(TaxProfile* profile)
{
   if( FindTaxProfile(profile->GetTaxProfileDBKey()))
   {
      selectedTaxProfile->taxSelectable = false;
   }

   loadView();
}

bool TaxProfileController::FindTaxProfile(int profileKey)
{
   bool found = false;
   std::map<int,TaxProfile*>::iterator it = taxProfiles.find(profileKey);

   if(it != taxProfiles.end())
   {
	  found = true;
	  selectedTaxProfile = it->second;
   }

   return found;
}

TaxProfile* TaxProfileController::GetTaxProfile(int profileKey)
{
	bool found = false;
	std::map<int, TaxProfile*>::iterator it = taxProfiles.find(profileKey);

	if(it != taxProfiles.end())
	{
		found = true;
		selectedTaxProfile = it->second;
	}
	return found ? selectedTaxProfile : NULL;
}

bool TaxProfileController::IsDuplicateTaxProfileExists(int currentTaxProfileKey, AnsiString taxProfileName, TaxType type)
{
   bool status = false;
   std::map<int,TaxProfile*>::iterator it = taxProfiles.begin();

   for( ; it != taxProfiles.end(); it++ )
   {
      if( it->second->GetTaxProfileDBKey() != currentTaxProfileKey
            && it->second->taxProfileName == taxProfileName
            && it->second->taxProfileType == type )
      {
         status = true;
         break;
      }
   }

   return status;
}

void TaxProfileController::UpdateTaxProfile(TaxProfile* guiTaxProfile)
{
   if( FindTaxProfile( guiTaxProfile->GetTaxProfileDBKey()))
   {
      selectedTaxProfile->taxProfileName  = guiTaxProfile->taxProfileName;
      selectedTaxProfile->taxPercentage   = guiTaxProfile->taxPercentage;
      selectedTaxProfile->taxProfileType  = guiTaxProfile->taxProfileType;
      selectedTaxProfile->taxCode         = guiTaxProfile->taxCode;
      selectedTaxProfile->taxPriority     = guiTaxProfile->taxPriority;
   }
}

bool TaxProfileController::SaveTaxProfiles()
{
   // load all the current changes into a map
   TaxProfileMap taxMap;
   GetAllModifications(&taxMap);

   // we have finished doing changes to tax profiles. save or update them as necessory here
   TTaxProfileManager taxProfileManager;
   taxProfileManager.SaveTaxProfiles(taxMap);
}

void TaxProfileController::GetAllModifications(TaxProfileMap* destination)
{
   destination->clear();
   std::map<int,TaxProfile*>::iterator it = taxProfiles.begin();
   TaxProfileKey key;

   for( ; it != taxProfiles.end(); it++ )
   {
      key = std::make_pair( it->second->taxProfileName, it->second->taxProfileType);
      destination->insert( std::make_pair(key, it->second));
   }
}

void TaxProfileController::loadView()
{
   taxGUI->ClearGrid();
   std::map<int, TaxProfile*>::iterator it = taxProfiles.begin();
   for( ; it != taxProfiles.end(); it++ )
   {
      if(it->second->GetTaxProfileStatus() != Deleted)
         taxGUI->AddTaxProfileToGrid(*it->second);
   }

   taxGUI->Refresh();
}

void TaxProfileController::loadTaxProfilesFromDB()
{
   TTaxProfileManager taxProfileManager;
   TaxProfileMap taxProfileMap = taxProfileManager.LoadTaxProfiles();

   populateTaxProfileMap(&taxProfileMap);
}

std::map<int, TaxProfile*> TaxProfileController::GetAllTaxProfiles()
{
	return taxProfiles;
}

void TaxProfileController::populateTaxProfileMap(TaxProfileMap* dbMap)
{
   taxProfiles.clear();
   TaxProfileMap::iterator it = dbMap->begin();
   for( ; it!= dbMap->end(); it++ )
   {
	  insertTaxProfileToMap(it->second);
   }
}

int TaxProfileController::getNextNewTaxProfileKey()
{
   return _newTaxProfileKey -= 1;
}

void TaxProfileController::insertTaxProfileToMap(TaxProfile *taxProfile)
{
   if(taxProfiles.find(taxProfile->GetTaxProfileDBKey()) == taxProfiles.end())
   {
      taxProfiles.insert(std::make_pair(taxProfile->GetTaxProfileDBKey(), taxProfile));
   }
}

#pragma package(smart_init)


