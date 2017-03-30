//---------------------------------------------------------------------------


#pragma hdrstop

#include "SiHotDataProcessor.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TRoomRequest TSiHotDataProcessor::CreateRoomRequest(TPhoenixRoomStatusExt status,AnsiString pmsIPAddress,int pmsPort)
{
    TRoomRequest roomRequest;
    roomRequest.TransactionNumber = GetTransNumber();
    roomRequest.RoomNumber = status.AccountNumber;
    roomRequest.IPAddress = pmsIPAddress;
    roomRequest.PortNumber = pmsPort;
    return roomRequest;
}
//----------------------------------------------------------------------------
void TSiHotDataProcessor::CreateRoomChargePost(TPaymentTransaction _paymentTransaction, TRoomCharge &_roomCharge)
{

}
//----------------------------------------------------------------------------
int TSiHotDataProcessor::GetTransNumber()
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *IBInternalQueryGenerator= DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQueryGenerator->SQL->Text = "SELECT GEN_ID(GEN_SIHOTTRANSNUMBER, 1) FROM RDB$DATABASE ";
    IBInternalQueryGenerator->Close();
    IBInternalQueryGenerator->ExecQuery();
    int value = IBInternalQueryGenerator->Fields[0]->AsInteger;
    return value;
    DBTransaction.Commit();
}
//----------------------------------------------------------------------------
void TSiHotDataProcessor::PrepareRoomStatus(TPhoenixRoomStatusExt &status,TRoomResponse roomResponse)
{
//    status.Balance =
//    status.CreditLimit =
//    status.FolderNumber =
//    status.Folders =
}
//----------------------------------------------------------------------------
