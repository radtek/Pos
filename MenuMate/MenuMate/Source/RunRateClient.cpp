//---------------------------------------------------------------------------

#pragma hdrstop

#include "RunRateClient.h"

#include <map>

#include "SelectDish.h"

#include "DBOrder.h"
#include "ItemSize.h"
#include "DeviceRealTerminal.h"
#include "ReqPrintJob.h"
#include "DBSaleTimes.h"
#include "DocketManager.h"
#include "VirtualPrinterManager.h"
#include "ListSecurityRefContainer.h"
//--------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

TRunRateClient::~TRunRateClient()
{

}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC METHODS
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void TRunRateClient::SendPointDetailsToRunRate ( TPaymentTransaction &inTransaction )
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TManagerVariable::Instance().GetStr(DBTransaction, vmRunRateBoardIP, TGlobalSettings::Instance().RunRateBoardIP);
    DBTransaction.Commit();
    std::auto_ptr<TRunRateTcpStreamSender> streamSender(
                                        new TRunRateTcpStreamSender(
                                            TGlobalSettings::Instance().RunRateBoardIP,
                                            57001 ) );
    if(inTransaction.Membership.Member.Points.getCurrentPointsEarned() > 0)
    {
        UnicodeString points = FormatFloat("0.00", inTransaction.Membership.Member.Points.getCurrentPointsEarned());
        streamSender->Send(points);
    }

}
//---------------------------------------------------------------------------
