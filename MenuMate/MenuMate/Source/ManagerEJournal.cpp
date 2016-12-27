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

void TManagerEJournal::TriggerEJournal()
{
    TForm* currentForm = Screen->ActiveForm;
    std::auto_ptr <TfrmEJournal> (frmEJournal)(TfrmEJournal::Create <TfrmEJournal> (currentForm));
    frmEJournal->Execute();
}
//---------------------------------------------------------------------------

