//---------------------------------------------------------------------------


#pragma hdrstop

#include "EJournalEngine.h"
#include "GlobalSettings.h"
#include "RealDeviceterminal.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
TEJournalEngine::TEJournalEngine()
{
}
//---------------------------------------------------------------------------

TEJournalEngine::~TEJournalEngine()
{
}
//---------------------------------------------------------------------------

void TEJournalEngine::CategorizeEJournal(TDateTime fromSessionDate,TDateTime toSessionDate)
{
    // Receipts+X+Zed
    if(!TGlobalSettings::Instance().ExcludeReceipt && !TGlobalSettings::Instance().ExcludeXReport )
    {

    }
    // Receipts+Zed
    if(!TGlobalSettings::Instance().ExcludeReceipt && TGlobalSettings::Instance().ExcludeXReport)
    {

    }
    //only Zed
    if(TGlobalSettings::Instance().ExcludeReceipt && TGlobalSettings::Instance().ExcludeXReport)
    {

    }
    if(TGlobalSettings::Instance().ExcludeReceipt && !TGlobalSettings::Instance().ExcludeXReport)
    {

    }
}
//---------------------------------------------------------------------------
void TEJournalEngine::CheckDataExist(TDateTime fromSessionDate,TDateTime toSessionDate)
{
    CategorizeEJournal(fromSessionDate,toSessionDate);
}
