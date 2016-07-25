//---------------------------------------------------------------------------

#pragma hdrstop

#include "ParserEngine.h"
#include <vcl.h>

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

std::string _errorMsg;

//---------------------------------------------------------------------------

EParserVersionUnknown::EParserVersionUnknown( UnicodeString inMessage ) :
						Exception( inMessage )
{

}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

TParserEngine::TParserEngine()
{
}
// ---------------------------------------------------------------------------
TParserEngine::~TParserEngine()
{
	closeVersions();
}
// ---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC METHODS.
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
PARSER_ERROR TParserEngine::SetDBControl( Database::TDBControl* const inDBControl )
{
	_dbControl = inDBControl;

	return PE_VERSION_DB_SET;
}
// ---------------------------------------------------------------------------
void TParserEngine::SetVersionParserInProgressEvent( VERSION_PARSER_EVENT inEvent )
{
	_onParserInProgress = inEvent;
}
// ---------------------------------------------------------------------------
void TParserEngine::SetVersionParserSucceedEvent( VERSION_PARSER_EVENT inEvent )
{
	_onParserSucceed = inEvent;
}
// ---------------------------------------------------------------------------
void TParserEngine::SetVersionParserFailedEvent( VERSION_PARSER_EVENT  inEvent )
{
	_onParserFailed = inEvent;
}
// ---------------------------------------------------------------------------
int TParserEngine::VersionCount()
{
    return 0;
}
// ---------------------------------------------------------------------------
PARSER_ERROR TParserEngine::ExecuteThread()
{
	PARSER_ERROR result;

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    //TParserThreadExecution::Instance()->SetExecuteFunction( ( PARSER_EXECUTE )TParserEngine::executeParserVersion );

    _threadExecutionCanceled = true;
    _versionsIT = _versions.begin();

    while( !_threadExecutionCanceled  &&
           (  _versionsIT != _versions.end() ) )
    {
        if( TParserThreadExecution::Instance()->Idle() )
        {
            TParserThreadExecution::Instance()->Start( _versionsIT );
        }

        Application->ProcessMessages();
        _versionsIT++;
    }

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
void TParserEngine::CancelThreadExecution()
{
    _threadExecutionCanceled = true;

    if( _onParserCanceling != NULL )
    {
        _onParserCanceling(
            _versionsIT->first,                    // version.
            "Canceling Updates. "
            "This might take a few seconds ..." ); // message.
    }
}
// ---------------------------------------------------------------------------
void TParserEngine::NotifyThreadExecutionCanceled()
{
    if( _onParserCanceled != NULL )
    {
        _onParserCanceled(
            _versionsIT->first,    // version.
            "Updates canceled." ); // message.
    }
}
// ---------------------------------------------------------------------------

PARSER_ERROR TParserEngine::Execute()
{
	PARSER_ERROR result;

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	for( _versionsIT  = _versions.begin();
		 _versionsIT != _versions.end();
		 _versionsIT++ )
	{
		if( !executeParserVersion( _versionsIT ) )
        {
            break;
        }
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
PARSER_ERROR TParserEngine::ExecuteAtVersion( std::string inVersionAlias )
{
	PARSER_ERROR result;

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		_versionsIT = iteratorAtVersion( inVersionAlias );

		executeParserVersion( _versionsIT );
	}
	catch( EParserVersionUnknown& exc )
	{
		setErrorMsg( exc.Message );
		result = PE_VERSION_UPGRADE_FAILED;
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
PARSER_ERROR TParserEngine::ExecuteFromVersion( std::string inVersionAlias )
{
	PARSER_ERROR result;

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		for( _versionsIT  = iteratorAtVersion( inVersionAlias );
			 _versionsIT != _versions.end();
			 _versionsIT++ )
		{
            if( !executeParserVersion( _versionsIT ) )
            {
                break;
            }
		}
	}
	catch( EParserVersionUnknown& exc )
	{
		setErrorMsg( exc.Message );
		result = PE_VERSION_UPGRADE_FAILED;
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
std::string TParserEngine::ErrorMessage()
{
	return _errorMsg;
}
// ---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PRIVATE METHODS.
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void TParserEngine::initVersions()
{
	populateVersionList( _versions );
}
// ---------------------------------------------------------------------------
void TParserEngine::closeVersions()
{
	_versions.clear();
}
// ---------------------------------------------------------------------------
void TParserEngine::populateVersionList( PARSER_FUNCTION_MAP& outVersions )
{
	// Must be redefined by derived class.
}
// ---------------------------------------------------------------------------
PARSER_FUNCTION_MAP::const_iterator TParserEngine::iteratorAtVersion( std::string inVersionAlias )
{
	std::string upperCaseAlias = upperCaseString( inVersionAlias );

	PARSER_FUNCTION_MAP::const_iterator result = _versions.find( upperCaseAlias );

	if( result == _versions.end() )
	{
		throw new EParserVersionUnknown( UnicodeString( inVersionAlias.c_str() ) );
	}

	return result;
}
// ---------------------------------------------------------------------------
bool __fastcall TParserEngine::executeParserVersion( PARSER_FUNCTION_MAP::const_iterator inVersionsIT )
{
    bool result = true;

    //:::::::::::::::::::::::::::::::::::::::

    VERSION_PARSER versionParser = inVersionsIT->second;

    try
    {
        if( _onParserInProgress != NULL )
        {
            _onParserInProgress(
                _versionsIT->first,                    // version.
                "Applying Updates. "
                "This might take a few minutes ..." ); // message.
        }

        result = versionParser( _dbControl );

        if( result == PE_VERSION_UPGRADE_FAILED )
        {
            if( _onParserFailed != NULL )
            {
                _onParserFailed(
                    _versionsIT->first, // version.
                    ErrorMessage() );   // error message.
            }
            result = false;
        }
        else
        {
            if( _onParserSucceed != NULL )
            {
                _onParserSucceed(
                    _versionsIT->first, // version.
                    ErrorMessage() );   // message.
            }
        }
    }
    catch( Exception& exc )
    {
        setErrorMsg( exc.Message );
        result = PE_VERSION_UPGRADE_FAILED;

        result = false;
    }

    //:::::::::::::::::::::::::::::::::::::::

    return result;
}
// ---------------------------------------------------------------------------
void TParserEngine::setErrorMsg( UnicodeString inErrorMsg )
{
	_errorMsg.clear();
	_errorMsg.append( inErrorMsg.t_str() );
}
// ---------------------------------------------------------------------------
std::string TParserEngine::upperCaseString( std::string inVersionAlias )
{
	return TStringTools::Instance()->UpperCaseString( inVersionAlias );
}
// ---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TParserThread
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
__fastcall TParserThread::TParserThread() : TThread( true )
{
    FreeOnTerminate = true;
    // setup other thread parameters as needed...
}
// ---------------------------------------------------------------------------

void __fastcall TParserThread::Execute()
{
    // do work here ...
    // if you need to access the UI controls,
    // use the TThread::Synchornize() method for that

    if( ParserExecute != NULL )
    {
        Synchronize( synchronizeExecute );
    }
}
// ---------------------------------------------------------------------------

void __fastcall TParserThread::synchronizeExecute()
{
    ParserExecute( VersionsIT );
}
// ---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TParserThreadExecution
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//---------------------------------------------------------------------------

// Singleton Object Instance set to NULL.
TParserThreadExecution* TParserThreadExecution::_executionThread = NULL;

//---------------------------------------------------------------------------

TParserThreadExecution* TParserThreadExecution::Instance()
{
	if( !_executionThread )
	{
		_executionThread = new TParserThreadExecution();
	}

	return _executionThread;
}
// ---------------------------------------------------------------------------
TParserThreadExecution::TParserThreadExecution()
{
    _idle   = true;
}
// ---------------------------------------------------------------------------
TParserThreadExecution::~TParserThreadExecution()
{
}
// ---------------------------------------------------------------------------
void TParserThreadExecution::SetExecuteFunction( PARSER_EXECUTE inExecute )
{
    if( _thread == NULL )
    {
        _thread = new TParserThread();
    }
    _thread->ParserExecute = inExecute;
}
// ---------------------------------------------------------------------------
bool TParserThreadExecution::Start( PARSER_FUNCTION_MAP::const_iterator inVersionsIT )
{
    bool result = false;

    //........................................................

    if( _thread != NULL )
    {
        _thread->VersionsIT = inVersionsIT;
        _thread->Start();
        _idle = false;

        result = true;
    }

    //........................................................

    return result;
}
// ---------------------------------------------------------------------------
bool TParserThreadExecution::Idle()
{
    return _idle;
}
// ---------------------------------------------------------------------------

