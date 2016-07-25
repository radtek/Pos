//---------------------------------------------------------------------------

#ifndef MMSecurityScreenManagerH
#define MMSecurityScreenManagerH

#include "MMSecurityEventSearchThread.h"
#include <map>
#include <vector>
#include <vcl.h>

//---------------------------------------------------------------------------

typedef unsigned __int32 TSecurityEventHandle;

//.......................................................

class TSecurityEventProvider : public TObject
{
private:
protected:
    TNotifySecurityTransaction FOnTransactionDataReady;

public:
    __fastcall TSecurityEventProvider();

	__property TNotifySecurityTransaction OnTransactionDataReady = { read=FOnTransactionDataReady, write=FOnTransactionDataReady };
};

//.......................................................

typedef std::map<TSecurityEventHandle, TSecurityEventProvider*> EVENT_PROVIDER_MAP;

struct TMMSecurityEventResponse
{
    bool                 Successful;
    UnicodeString        Description;
    TSecurityEventHandle Handle;
};

//.......................................................

class TMMTimer;

typedef std::vector<TSecurityEventHandle> SHANDLE_LIST;

class TMMSecurityScreenManager
{
    public:
               TMMSecurityScreenManager();
      virtual ~TMMSecurityScreenManager();

      TMMSecurityEventResponse CreateSecurityEventProvider();
      TMMSecurityEventResponse CloseSecurityEventProvider( TSecurityEventHandle inHandle );
      void CloseAllSecurityEventProviders();

      TMMSecurityEventResponse SetSecurityTransactionDelegate(
                                 TSecurityEventHandle inHandle,
                                 TNotifySecurityTransaction inEvent );

       void ResetDatetime();

     protected:
       static TDateTime               date_time;
       static EVENT_PROVIDER_MAP      event_provider_map;
       static bool                    searching_now;
       static std::auto_ptr<TMMTimer> search_timer;

       SHANDLE_LIST handle_list;

       TSecurityEventHandle addProvider( TSecurityEventProvider* inProvider );

       void deleteHandle( TSecurityEventHandle inHandle );
       void deleteAllHandles();

       //::::::::::::::::::::::::::
       // Event searching timer
       void _fastcall onSearchTimer( TObject* inSender );

       //::::::::::::::::::::::::::
       // Event searching thread
       void launchEventSearch();

       void _fastcall securityTransactionFetched(
                         TObject* inSender,
                         TSecurityTransactionData* inData);
       void _fastcall securityEventSearchThreadTerminated( TObject* inSender );

       TCriticalSection* createCriticalSection();

       //::::::::::::::::::::::::::
       // Responses
       TMMSecurityEventResponse createResponseWithHandle( TSecurityEventHandle inHandle );
       TMMSecurityEventResponse createFailedResponseWithDescription( UnicodeString inDescription );
};

#endif
