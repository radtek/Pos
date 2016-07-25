//---------------------------------------------------------------------------

#pragma hdrstop

#include "ClippIntegrationWriteMessageThread.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

TClippIntegrationWriteMessageThread::TClippIntegrationWriteMessageThread() : TClippIntegrationThreadBase()
{
    _recievedMessageId = "";
    _tipAmount = 0.0;
    _clippProcessingFees = 0.0;
    _totalPaymentAmount = 0.0;
}

//---------------------------------------------------------------------------

void TClippIntegrationWriteMessageThread::SetTabKey(int tabKey)
{
    _tabKey = tabKey;
}

//---------------------------------------------------------------------------

void TClippIntegrationWriteMessageThread::SetRecievedMessageId(UnicodeString recievedMessageId)
{
    _recievedMessageId = recievedMessageId;
}

//---------------------------------------------------------------------------

void TClippIntegrationWriteMessageThread::SetTipAmount(Currency tipAmount)
{
    _tipAmount = tipAmount;
}

//---------------------------------------------------------------------------

void TClippIntegrationWriteMessageThread::SetClippProcessingFees(Currency clippProcessingFees)
{
    _clippProcessingFees = clippProcessingFees;
}

//---------------------------------------------------------------------------

void TClippIntegrationWriteMessageThread::SetTotalPaymentAmount(Currency totalPaymentAmount)
{
    _totalPaymentAmount = totalPaymentAmount;
}

//---------------------------------------------------------------------------

void TClippIntegrationWriteMessageThread::SetTotalClippDiscount(std::vector<MMClippDiscount> totalClippDiscounts)
{
    _totalClippDiscounts = totalClippDiscounts;
}

//---------------------------------------------------------------------------
