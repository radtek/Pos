//---------------------------------------------------------------------------


#pragma hdrstop

#include "InitializeDCSession.h"
#include "Processing.h"
#include "DeviceRealTerminal.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
TInitializeDCSession::TInitializeDCSession()
{
}
//---------------------------------------------------------------------------

TInitializeDCSession::~TInitializeDCSession()
{
}
void TInitializeDCSession::StartSession(int memPoints,AnsiString memberPoints,AnsiString memNo,int contactKey)
{
          UnicodeString serverPath = TGlobalSettings::Instance().DrinkCommandServerPath;
          AnsiString path = AnsiString(serverPath);
          int serverPort = TGlobalSettings::Instance().DrinkCommandServerPort;
          std::auto_ptr<TDrinkCommandManager> dcManager(
                                                    new TDrinkCommandManager());

           // check if session exists or not
          AnsiString card_id = TDrinkCommandData::Instance().GetCardIdFromMemberNo(memNo);
          AnsiString session_uuid = TDrinkCommandData::Instance().CheckForOpenSession(card_id);
          int groupKey = TDrinkCommandData::Instance().GetGroupKey(contactKey);
          if (session_uuid.Length() == 0)
          {
             UnicodeString session_id = dcManager->StartSession(path , serverPort , card_id, groupKey) ;
             if(session_id.Length() != 0)
             {
                 Sleep(3000);
                 dcManager->SendCashInfo(path , serverPort , memberPoints ,session_id);
             }
          }
          else
          {
             Sleep(1000);
             dcManager->AskDCSummary(path , serverPort ,session_uuid) ;            //asks DC for itemised summary of the customer                                                 // Indicates that item has been fetched
          }
}