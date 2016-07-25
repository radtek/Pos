//---------------------------------------------------------------------------

#ifndef SyndCodeControllerH
#define SyndCodeControllerH
//---------------------------------------------------------------------------
#include <Memory>
#include "ListManager.h"
#include "ManagerSyndCode.h"

class TSyndCodeController
{
   private :
      std::auto_ptr<TfrmListManager> frmListManager;
      TManagerSyndCode &ManagerSyndCode;
      Database::TDBTransaction &DBTransaction;
      TForm *DisplayOwner;
		void OnAdd(int ItemIndex, int ColIndex = -1);
		void OnEdit(int ItemIndex, int ColIndex = -1);
		void OnDelete(int ItemIndex, int ColIndex = -1);
		void OnClose(int itemIndex, int ColIndex = -1);
      void PopulateListManager();
   public :
      TSyndCodeController(TForm *inDisplayOwner,Database::TDBTransaction &inDBTransaction,TManagerSyndCode &inManagerSyndCode);
      void Run();
};
#endif
