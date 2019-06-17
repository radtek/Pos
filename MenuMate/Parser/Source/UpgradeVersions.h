//---------------------------------------------------------------------------

#ifndef UpgradeVersionsH
#define UpgradeVersionsH

//---------------------------------------------------------------------------

#include "ParserEngineMenumate.h"

//---------------------------------------------------------------------------

typedef Exception EParserEngineUnknown;

// Engine Map.
//		- engine alias: e.g "posabl".
//		- parser object's pointer.
typedef std::map<std::string, TParserEngine*> PARSER_ENGINE_LIST;

//---------------------------------------------------------------------------

class TVersionParser
{
   public:
   		~TVersionParser();

		//Singleton Instance Fetch Method.
		static TVersionParser* Instance();

		// Set parser: e.g. "menumate".
		PARSER_ERROR SetParser( std::string inParserAlias );

		// Set DBControl.
		PARSER_ERROR SetDBControl( Database::TDBControl* inDBControl );
		PARSER_ERROR SetDBControl(
						          std::string inParserAlias,
						Database::TDBControl* inDBControl );

        int VersionCount();

		// Set events.
        PARSER_ERROR SetVersionParserInProgressEvent( VERSION_PARSER_EVENT inEvent );
        PARSER_ERROR SetVersionParserInProgressEvent(
                                 std::string inParserAlias,
                        VERSION_PARSER_EVENT inEvent );

		PARSER_ERROR SetVersionParserSucceedEvent( VERSION_PARSER_EVENT inEvent );
		PARSER_ERROR SetVersionParserSucceedEvent(
                                 std::string inParserAlias,
						VERSION_PARSER_EVENT inEvent );

		PARSER_ERROR SetVersionParserFailedEvent( VERSION_PARSER_EVENT inEvent );
		PARSER_ERROR SetVersionParserFailedEvent(
                                  std::string inParserAlias,
						 VERSION_PARSER_EVENT inEvent );

		// Execute Parser.
		PARSER_ERROR Execute();
		PARSER_ERROR ExecuteParser(            std::string inParserAlias );
		PARSER_ERROR ExecuteParserAtVersion(   std::string inParserAlias, std::string inVersionAlias );
		PARSER_ERROR ExecuteParserFromVersion( std::string inParserAlias, std::string inVersionAlias );

		// Error message after updating a version.
		std::string ErrorMessage();
		std::string ErrorMessageWithParser( std::string inParserAlias );

   private:
		//Singleton Object Instance
		static TVersionParser* _parser;

		TVersionParser();

		std::string                        _parserEngineAlias;
		PARSER_ENGINE_LIST                 _parserEngines;
		PARSER_ENGINE_LIST::const_iterator _parserEnginesIT;

		TStringTools *_stringTools;

		void initParserEngines() ;
		void closeParserEngines();

		void setDefaultParserEngine();

		std::string errorMessage();

		//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		//  Methods that have to be modified when adding new ParserEngines.
		//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
				  void populateParserEngineList( PARSER_ENGINE_LIST& outParserEngines );
		  PARSER_ERROR setParserEngine( std::string inParserAlias );
		TParserEngine* getParserEngine( std::string inParserAlias );

		//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		//  Methods that use TStringTools.
		//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
				int equalStrings(
						std::string inStr1,
						std::string inStr2 );
		std::string upperCaseString( std::string inStr );
};

//---------------------------------------------------------------------------

#endif
