//---------------------------------------------------------------------------


#pragma hdrstop

#include "AdjustmentContact.h"

//---------------------------------------------------------------------------
TAdjustmentTransaction::TAdjustmentTransaction()
{
    Adjustment = 0;
    AdjustmentType = 0;
    AdjustmentSubType = 0;
    //TimeStamp = Now();
    InvoiceNumber = 0;
    ContactKey = 0;
}

TAdjustmentContact::TAdjustmentContact()
{
    ContactKey = 0;
    Name = "";
    MemberNumber = "";
    SiteId = -1;
    Adjustments.clear();
}

#pragma package(smart_init)
