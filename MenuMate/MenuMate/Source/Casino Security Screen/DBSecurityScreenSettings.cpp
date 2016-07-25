//---------------------------------------------------------------------------

#pragma hdrstop

#include "DBSecurityScreenSettings.h"

#include "ManagerLocations.h"
#include "Devices.h"
#include "Contact.h"
#include "ListPaymentSystem.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

void TDBSecurityScreenSettings::GetLocations(
                                  Database::TDBTransaction& inDBTransaction,
                                  AUDIT_FILTER_LIST& outList )
{
    outList.clear();

    TManagerLocations locationManager;

    std::auto_ptr <TStringList> locationList( new TStringList );
    locationManager.GetLocationList( inDBTransaction, locationList.get() );

    for( int i = 0; i < locationList->Count; i++ )
    {
        TFilterInfo info( ( int )locationList->Objects[i], locationList->Strings[i] );
        outList.push_back( info );
    }
}
//---------------------------------------------------------------------------

void TDBSecurityScreenSettings::GetTerminals(
                                  Database::TDBTransaction& inDBTransaction,
                                  AUDIT_FILTER_LIST& outList )
{
    outList.clear();

    TManagerDevices deviceManager;

    std::auto_ptr <TStringList> deviceList( new TStringList );
    deviceManager.GetDeviceNameList( inDBTransaction, 1, deviceList.get() );

    for( int i = 0; i < deviceList->Count; i++ )
    {
        int         object = ( int )deviceList->Objects[i];
        UnicodeString name = deviceList->Strings[i];

        TFilterInfo info( object, name );
        outList.push_back( info );
    }
/*
    for( int i = 0; i < 30; i++ )
    {
        TFilterInfo info( i, "Terminal" );
        result.push_back( info );
    }
*/
}
//---------------------------------------------------------------------------

void TDBSecurityScreenSettings::GetCashiers(
                                  Database::TDBTransaction& inDBTransaction,
                                  AUDIT_FILTER_LIST& outList )
{
    outList.clear();

    std::auto_ptr<TContact> contacts( new TContact );
    contacts->Enabled = true;

    std::auto_ptr <TStringList> cashierList( new TStringList );
    contacts->GetContactList( inDBTransaction, cashierList.get() );

    for( int i = 0; i < cashierList->Count; i++ )
    {
        TFilterInfo info( ( int )cashierList->Objects[i], cashierList->Strings[i] );
        outList.push_back( info );
    }
}
//---------------------------------------------------------------------------

void TDBSecurityScreenSettings::GetPaymentTypes(
                                  Database::TDBTransaction& inDBTransaction,
                                  AUDIT_FILTER_LIST& outList )
{
    outList.clear();

    std::auto_ptr<TListPaymentSystem> paymentSystem( new TListPaymentSystem );

    std::auto_ptr <TStringList> paymentList( new TStringList );
    paymentSystem->PaymentLoad( inDBTransaction, paymentList.get() );

    for( int i = 0; i < paymentList->Count; i++ )
    {
        TFilterInfo info( ( int )paymentList->Objects[i], paymentList->Strings[i] );
        outList.push_back( info );
    }
    TFilterInfo info(paymentList->Count ,"Points" );
    TFilterInfo delayedPaymentinfo(paymentList->Count ,"Delayed Payment" );
    outList.push_back( info );
    outList.push_back( delayedPaymentinfo );
}
//---------------------------------------------------------------------------

