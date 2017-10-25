//---------------------------------------------------------------------------


#pragma hdrstop

#include "BasePMS.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//TBasePMS *BasePMS;
TBasePMS::TBasePMS() :fPhoenixNet(new TPhoenixNetTCPManager)
{
}
//---------------------------------------------------------------------------
//TBasePMS::TBasePMS()
//{
//}

//---------------------------------------------------------------------------
TBasePMS::~TBasePMS()
{
}
//---------------------------------------------------------------------------
void TBasePMS::Initialise()
{
}
//---------------------------------------------------------------------------
void TBasePMS::GetRoomStatus(TPhoenixRoomStatusExt &RoomStatus,AnsiString
                                                       PMSIPAddress,int PMSPort)
{
}
//---------------------------------------------------------------------------
void TBasePMS::GetRoomStatus(std::vector<TSiHotAccounts> &siHotAccounts,
                                           AnsiString pmsIPAddress,int pmsPort)
{
}
//---------------------------------------------------------------------------
bool TBasePMS::ExportData(TPaymentTransaction &PaymentTransaction, int StaffID)
{
}
//---------------------------------------------------------------------------
AnsiString TBasePMS::GetLastTransactionRef()
{
}
//---------------------------------------------------------------------------
bool TBasePMS::TestCode(AnsiString CodeToTest)
{
}
//---------------------------------------------------------------------------
void TBasePMS::ClearCodesTestedOk()
{
}
//---------------------------------------------------------------------------
bool TBasePMS::TestDefaultCodes()
{
}
//---------------------------------------------------------------------------
void TBasePMS::FindAndReplaceCode(AnsiString SourceCode, AnsiString DestCode)
{
}
//---------------------------------------------------------------------------
void TBasePMS::CheckCreditLimit(TPhoenixRoomCharge &RoomCharge,AnsiString
                                                       PMSIPAddress,int PMSPort)
{
}
//---------------------------------------------------------------------------
void TBasePMS::ChargeRoom(TPhoenixRoomCharge &RoomCharge,AnsiString
                                                       PMSIPAddress,int PMSPort)
{
}
//---------------------------------------------------------------------------
void TBasePMS::GetRoomStatus(AnsiString _roomNumber, TRoomInquiryResult &_roomResult)//std::auto_ptr<TRoomInquiryResult> _roomResult)
{
}
//---------------------------------------------------------------------------

