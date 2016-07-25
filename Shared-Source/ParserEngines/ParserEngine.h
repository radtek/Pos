//---------------------------------------------------------------------------

#ifndef ParserEngineH
#define ParserEngineH

//---------------------------------------------------------------------------

#include "VersionParser.h"

//---------------------------------------------------------------------------

class EParserVersionUnknown : public Exception
{
	public:
		EParserVersionUnknown( UnicodeString inMessage );
};

//---------------------------------------------------------------------------

class TParserThread : public TThread
{
protected:
    virtual void __fastcall Execute();

    void __fastcall synchronizeExecute();

public:
    __fastcall TParserThread();

    PARSER_EXECUTE ParserExecute;
    PARSER_FUNCTION_MAP::const_iterator VersionsIT;
};

//---------------------------------------------------------------------------

class TParserThreadExecution
{
public:
    ~TParserThreadExecution();

    //Singleton Instance Fetch Method.
    static TParserThreadExecution* Instance();

    void SetExecuteFunction( PARSER_EXECUTE inExecute );
    bool Start( PARSER_FUNCTION_MAP::const_iterator inVersionsIT );
    bool Idle();

private:
    //Singleton Object Instance
    static TParserThreadExecution* _executionThread;

    TParserThreadExecution();

    TParserThread *_thread;
    bool _idle;
};

//---------------------------------------------------------------------------

class TParserEngine
{
   public:
		TParserEngine();
		virtual ~TParserEngine();

		// Set DBControl.
		PARSER_ERROR SetDBControl( Database::TDBControl* const inDBControl );

		// Set Events.
        void SetVersionParserInProgressEvent( VERSION_PARSER_EVENT inEvent );
		void SetVersionParserSucceedEvent(    VERSION_PARSER_EVENT inEvent );
		void SetVersionParserFailedEvent(     VERSION_PARSER_EVENT inEvent );

        virtual int VersionCount();

        // Execute Parser in a different thread.
        PARSER_ERROR ExecuteThread();
        void CancelThreadExecution();
        void NotifyThreadExecutionCanceled();

		// Execute Parser.
		virtual PARSER_ERROR Execute();
		virtual PARSER_ERROR ExecuteAtVersion(   std::string inVersionAlias );
		virtual PARSER_ERROR ExecuteFromVersion( std::string inVersionAlias );

		// Error message after updating a version.
		std::string ErrorMessage();

   protected:
		Database::TDBControl* _dbControl;

        // Version Map.
        //		- version alias: e.g "5.18".
        //		- version parser function.
		PARSER_FUNCTION_MAP                 _versions;
		PARSER_FUNCTION_MAP::const_iterator _versionsIT;

        bool _threadExecutionCanceled;

        VERSION_PARSER_EVENT _onParserInProgress;
		VERSION_PARSER_EVENT _onParserSucceed;
		VERSION_PARSER_EVENT _onParserFailed;
        VERSION_PARSER_EVENT _onParserCanceling;
        VERSION_PARSER_EVENT _onParserCanceled;

		void initVersions();
		void closeVersions();

		virtual void populateVersionList( PARSER_FUNCTION_MAP& outVersions );

		PARSER_FUNCTION_MAP::const_iterator iteratorAtVersion( std::string inVersionAlias );

        bool __fastcall executeParserVersion( PARSER_FUNCTION_MAP::const_iterator inVersionsIT );

		void setErrorMsg( UnicodeString inErrorMsg );

		std::string upperCaseString( std::string inVersionAlias );
};

//---------------------------------------------------------------------------

#endif
