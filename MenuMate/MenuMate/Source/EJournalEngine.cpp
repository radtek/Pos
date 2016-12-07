//---------------------------------------------------------------------------


#pragma hdrstop

#include "EJournalEngine.h"
#include "GlobalSettings.h"
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
  if(!TGlobalSettings::Instance().ExcludeReceipt && !TGlobalSettings::Instance().ExcludeXReport)
  {
     int i = 0;
  }
  // Receipts+Zed
  else if(!TGlobalSettings::Instance().ExcludeReceipt && TGlobalSettings::Instance().ExcludeXReport)
  {
     int j = 0;
  }
  //only Zed
  else if(TGlobalSettings::Instance().ExcludeReceipt && TGlobalSettings::Instance().ExcludeXReport)
  {
     int k = 0;
  }
}
//---------------------------------------------------------------------------
