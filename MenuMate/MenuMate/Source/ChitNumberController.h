// ---------------------------------------------------------------------------

#ifndef ChitNumberControllerH
#define ChitNumberControllerH
// ---------------------------------------------------------------------------
#include <memory>
#include "ListManager.h"
#include "ManagerChitNumber.h"
#include "ChitNumber.h"

enum ChitResult
{
   ChitDisabled, ChitOk, ChitCancelled, ChitNone
};

class TChitNumberController
{
private:
   bool Enabled;

   std::auto_ptr <TfrmListManager> frmListManager;
   Database::TDBTransaction &DBTransaction;

   TForm *DisplayOwner;
	void OnAdd(int ItemIndex, int ColIndex = -1);
	void OnEdit(int ItemIndex, int ColIndex = -1);
	void OnDelete(int ItemIndex, int ColIndex = -1);
	void OnActivate(int ItemIndex, int ColIndex = -1);
	void OnClose(int ItemIndex, int ColIndex = -1);
   void PopulateListManager();
   ChitResult GetNextChitNumber(TChitNumber &Chit, bool isWebOrder = true);

public:
      TChitNumberController(TForm *inDisplayOwner,Database::TDBTransaction &inDBTransaction);
      TChitNumberController(Database::TDBTransaction &inDBTransaction);
   ~TChitNumberController();
   void Run();
   ChitResult GetChitNumber(bool Prompt, TChitNumber &ChitNumber);
   ChitResult GetDefaultChitNumber(TChitNumber &ChitNumber);
   ChitResult GetChitNumber(TChitNumber &ChitNumber);
};

#endif
