//---------------------------------------------------------------------------

#ifndef I_TaxProfileGUIH
#define I_TaxProfileGUIH

#include "TaxProfileController.h"
#include "TaxProfile.h"
#include <memory>
//---------------------------------------------------------------------------

class TaxProfileController;
class I_TfrmTaxProfile
{
   public:
      virtual void SetController(TaxProfileController* controller) = 0;
      virtual void AddTaxProfile() = 0;
      virtual void EditTaxProfile() = 0;
      virtual void DeleteTaxProfile() = 0;
      virtual void Display() = 0;
      virtual void ClearGrid() = 0;
      virtual void AddTaxProfileToGrid(TaxProfile &tp) = 0;
      virtual void Refresh() = 0;
      virtual void Message(AnsiString message, AnsiString header, int type=0) = 0;
};


#endif
