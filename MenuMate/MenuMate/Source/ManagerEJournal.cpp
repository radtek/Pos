//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerEJournal.h"
#include "ShowEJournal.h"
#include "GlobalSettings.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
TManagerEJournal::TManagerEJournal()
{
}
//---------------------------------------------------------------------------

TManagerEJournal::~TManagerEJournal()
{
}
//---------------------------------------------------------------------------

void TManagerEJournal::TriggerEJournal(bool isConsolidatedZed)
{
    TForm* currentForm = Screen->ActiveForm;
    std::auto_ptr <TfrmEJournal> (frmEJournal)(TfrmEJournal::Create <TfrmEJournal> (currentForm));
    frmEJournal->IsConsolidatedZed = isConsolidatedZed;
    frmEJournal->Execute();

}
//---------------------------------------------------------------------------

