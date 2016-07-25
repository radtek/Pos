//---------------------------------------------------------------------------

#pragma hdrstop

#include "MMSecurityScreenManager.h"

#include <algorithm>

#include "MMTimer.h"
#include "DeviceRealTerminal.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

TDateTime               TMMSecurityScreenManager::date_time = 0;
EVENT_PROVIDER_MAP      TMMSecurityScreenManager::event_provider_map;
bool                    TMMSecurityScreenManager::searching_now = false;
std::auto_ptr<TMMTimer> TMMSecurityScreenManager::search_timer;

_fastcall TSecurityEventProvider::TSecurityEventProvider()
{
}

//::::::::::::::::::::::::::
// PUBLIC
//::::::::::::::::::::::::::
TMMSecurityScreenManager::TMMSecurityScreenManager()
{
    TCriticalSection *csection = createCriticalSection();

    csection->Acquire(); // lockout threads
    try
    {
        if( search_timer.get() == NULL )
        {
            date_time = Now(); //0;

            searching_now = false;

            // Default timer interval: 1000 mSec = 1 sec
            search_timer.reset( new TMMTimer( NULL ) );
            search_timer->Enabled = false;
            search_timer->OnTimer = onSearchTimer;
        }
    }
    catch( Exception &e )
    {
        csection->Release();
        delete csection;

        throw;
    }

    csection->Release();
    delete csection;
}
//---------------------------------------------------------------------------

TMMSecurityScreenManager::~TMMSecurityScreenManager()
{
    deleteAllHandles();
}
//---------------------------------------------------------------------------

//void TMMSecurityScreenManager::SetDB( TIBDatabase* inIBDatabase, bool inForced )
//{
//    TCriticalSection *csection = createCriticalSection();
//
//    csection->Acquire(); // lockout threads
//    try
//    {
//        if( ( db_control == NULL ) || inForced )
//        {
//            db_control = inIBDatabase;
//        }
//    }
//    __finally
//    {
//        csection->Release();
//    }
//
//    delete csection;
//}
//---------------------------------------------------------------------------

TMMSecurityEventResponse TMMSecurityScreenManager::CreateSecurityEventProvider()
{
    TMMSecurityEventResponse result;

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::

    TCriticalSection *csection = createCriticalSection();

    csection->Acquire(); // lockout threads
    try
    {
        TSecurityEventHandle handle = addProvider( new TSecurityEventProvider() );
        result = createResponseWithHandle( handle );
    }
    catch( Exception &e )
    {
        result = createFailedResponseWithDescription( e.Message );
    }

    csection->Release();
    delete csection;

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::

    return result;
}
//---------------------------------------------------------------------------

TMMSecurityEventResponse TMMSecurityScreenManager::CloseSecurityEventProvider(
                                                     TSecurityEventHandle inHandle )
{
    TMMSecurityEventResponse result;

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::

    TCriticalSection *csection = createCriticalSection();

    csection->Acquire(); // lockout threads
    try
    {
        deleteHandle( inHandle );
        result = createResponseWithHandle( inHandle );
    }
    catch( Exception &e )
    {
        result = createFailedResponseWithDescription( e.Message );
    }

    csection->Release();
    delete csection;

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::

    return result;
}
//---------------------------------------------------------------------------

void TMMSecurityScreenManager::CloseAllSecurityEventProviders()
{
    TCriticalSection *csection = createCriticalSection();

    csection->Acquire(); // lockout threads
    try
    {
        deleteAllHandles();
    }
    __finally
    {
        csection->Release();
    }

    delete csection;
}
//---------------------------------------------------------------------------

TMMSecurityEventResponse TMMSecurityScreenManager::SetSecurityTransactionDelegate(
                                                     TSecurityEventHandle inHandle,
                                                     TNotifySecurityTransaction inEvent )
{
    TMMSecurityEventResponse result;

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::

    TCriticalSection *csection = createCriticalSection();

    csection->Acquire(); // lockout threads
    try
    {
        TSecurityEventProvider *provider = event_provider_map[inHandle];
        provider->OnTransactionDataReady = inEvent;

        result = createResponseWithHandle( inHandle );
    }
    catch( Exception &e )
    {
        result = createFailedResponseWithDescription( e.Message );
    }

    csection->Release();
    delete csection;

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::

    return result;
}
//---------------------------------------------------------------------------

void TMMSecurityScreenManager::ResetDatetime()
{
    date_time = Now();
}
//---------------------------------------------------------------------------

//::::::::::::::::::::::::::
// PROTECTED
//::::::::::::::::::::::::::
TSecurityEventHandle TMMSecurityScreenManager::addProvider(
                                                 TSecurityEventProvider* inProvider )
{
    TSecurityEventHandle handle = ( TSecurityEventHandle )inProvider;
    event_provider_map[handle]  = inProvider;

    handle_list.push_back( handle );

    search_timer->Enabled = true;

    return handle;
}
//---------------------------------------------------------------------------

void TMMSecurityScreenManager::deleteHandle( TSecurityEventHandle inHandle )
{
    try
    {
        event_provider_map.erase( inHandle );
    }
    catch( ... )
    {
    }

    //::::::::::::::::::::::::::::::::::::::::

    try
    {
        SHANDLE_LIST::iterator shIT = std::find(
                                         handle_list.begin(),
                                         handle_list.end(), inHandle );

        handle_list.erase( shIT );
    }
    catch( ... )
    {
    }

    search_timer->Enabled = event_provider_map.size() > 0;
}
//---------------------------------------------------------------------------
void TMMSecurityScreenManager::deleteAllHandles()
{
    while( handle_list.size() > 0 )
    {
        deleteHandle( handle_list[0] );
    }
}
//---------------------------------------------------------------------------

//::::::::::::::::::::::::::
// Event searching timer
void _fastcall TMMSecurityScreenManager::onSearchTimer( TObject* inSender )
{
    TCriticalSection *csection = createCriticalSection();

    csection->Acquire(); // lockout threads
    if( searching_now )
    {
    }
    else
    {
        launchEventSearch();
    }

    csection->Release();
    delete csection;
}
//---------------------------------------------------------------------------

//::::::::::::::::::::::::::
// Event searching thread
void TMMSecurityScreenManager::launchEventSearch()
{
    searching_now = true;

    TMMSecurityEventSearchThread *thread = new TMMSecurityEventSearchThread( TDeviceRealTerminal::Instance().DBControl );

    thread->DateTime        = date_time;
    thread->FreeOnTerminate = true;
    thread->OnSecurityTransactionFetch = securityTransactionFetched;
    thread->OnTerminate = securityEventSearchThreadTerminated;

    thread->Suspended = false;
}
//---------------------------------------------------------------------------
void _fastcall TMMSecurityScreenManager::securityTransactionFetched(
                                           TObject* inSender,
                                           TSecurityTransactionData* inData)
{
    TCriticalSection *csection = createCriticalSection();

    csection->Acquire(); // lockout threads
    try
    {
        EVENT_PROVIDER_MAP::iterator mapIT = event_provider_map.begin();

        for( ; mapIT != event_provider_map.end(); mapIT++ )
        {
            TSecurityEventProvider *provider = mapIT->second;
            if( provider->OnTransactionDataReady != NULL )
            {
                provider->OnTransactionDataReady( (TObject*)this, inData );
            }
            date_time = inData->DateTime;
        }
    }
    __finally
    {
        csection->Release();
    }

    delete csection;
}
//---------------------------------------------------------------------------
void _fastcall TMMSecurityScreenManager::securityEventSearchThreadTerminated( TObject* inSender )
{
    TCriticalSection *csection = createCriticalSection();

    csection->Acquire(); // lockout threads

    searching_now = false;

    csection->Release();
    delete csection;
}
//---------------------------------------------------------------------------

TCriticalSection* TMMSecurityScreenManager::createCriticalSection()
{
    return new  TCriticalSection();
}
//---------------------------------------------------------------------------

//::::::::::::::::::::::::::
// Responses
TMMSecurityEventResponse TMMSecurityScreenManager::createResponseWithHandle(
                                                     TSecurityEventHandle inHandle )
{
    TMMSecurityEventResponse result;

    //::::::::::::::::::::::::::::::::::::::::::::::::

    result.Successful  = true;
    result.Description = "";
    result.Handle      = inHandle;

    //::::::::::::::::::::::::::::::::::::::::::::::::

    return result;
}
//---------------------------------------------------------------------------

TMMSecurityEventResponse TMMSecurityScreenManager::createFailedResponseWithDescription(
                                                      UnicodeString inDescription )
{
    TMMSecurityEventResponse result;

    //::::::::::::::::::::::::::::::::::::::::::::::::

    result.Successful  = false;
    result.Description = inDescription;
    result.Handle      = 0;

    //::::::::::::::::::::::::::::::::::::::::::::::::

    return result;
}
//---------------------------------------------------------------------------



