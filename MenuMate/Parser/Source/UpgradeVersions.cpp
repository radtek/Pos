//---------------------------------------------------------------------------

#pragma hdrstop

#include "UpgradeVersions.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

// Singleton Object Instance set to NULL.
TVersionParser* TVersionParser::_parser = NULL;

//---------------------------------------------------------------------------

TVersionParser* TVersionParser::Instance()
{
	if( !_parser )
	{
		_parser = new TVersionParser();
	}

	return _parser;
}
// ---------------------------------------------------------------------------
TVersionParser::TVersionParser()
{
	_stringTools = TStringTools::Instance();

	initParserEngines();
	setDefaultParserEngine();
}
// ---------------------------------------------------------------------------
TVersionParser::~TVersionParser()
{
	closeParserEngines();
}
// ---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC METHODS.
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
PARSER_ERROR TVersionParser::SetParser( std::string inParserAlias )
{
	// The default Parser Engine is Menumate's.
	try
	{
		return setParserEngine( inParserAlias );
	}
	catch( EParserEngineUnknown& exc )
	{
		return PE_UNKNOWN_PARSER;
	}
}
// ---------------------------------------------------------------------------
PARSER_ERROR TVersionParser::SetDBControl( Database::TDBControl* inDBControl )
{
	try
	{
		return getParserEngine( _parserEngineAlias )->SetDBControl( inDBControl );
	}
	catch( EParserEngineUnknown& exc )
	{
		return PE_UNKNOWN_PARSER;
	}
}
// ---------------------------------------------------------------------------
PARSER_ERROR TVersionParser::SetDBControl(
										 std::string  inParserAlias,
								Database::TDBControl* inDBControl )
{
	try
	{
		return getParserEngine( inParserAlias )->SetDBControl( inDBControl );
	}
	catch( EParserEngineUnknown& exc )
	{
		return PE_UNKNOWN_PARSER;
	}
}
// ---------------------------------------------------------------------------
PARSER_ERROR TVersionParser::SetVersionParserInProgressEvent( VERSION_PARSER_EVENT inEvent )
{
	try
	{
		getParserEngine( _parserEngineAlias )->SetVersionParserInProgressEvent( inEvent );
		return PE_EVENT_SET;
	}
	catch( EParserEngineUnknown& exc )
	{
		return PE_UNKNOWN_PARSER;
	}
}
// ---------------------------------------------------------------------------
PARSER_ERROR TVersionParser::SetVersionParserInProgressEvent(
                                 std::string inParserAlias,
						VERSION_PARSER_EVENT inEvent )
{
	try
	{
		getParserEngine( inParserAlias )->SetVersionParserInProgressEvent( inEvent );
		return PE_EVENT_SET;
	}
	catch( EParserEngineUnknown& exc )
	{
		return PE_UNKNOWN_PARSER;
	}
}
// ---------------------------------------------------------------------------
PARSER_ERROR TVersionParser::SetVersionParserSucceedEvent( VERSION_PARSER_EVENT inEvent )
{
	try
	{
		getParserEngine( _parserEngineAlias )->SetVersionParserSucceedEvent( inEvent );
		return PE_EVENT_SET;
	}
	catch( EParserEngineUnknown& exc )
	{
		return PE_UNKNOWN_PARSER;
	}
}
// ---------------------------------------------------------------------------
PARSER_ERROR TVersionParser::SetVersionParserSucceedEvent(
                                 std::string inParserAlias,
						VERSION_PARSER_EVENT inEvent )
{
	try
	{
		getParserEngine( inParserAlias )->SetVersionParserSucceedEvent( inEvent );
		return PE_EVENT_SET;
	}
	catch( EParserEngineUnknown& exc )
	{
		return PE_UNKNOWN_PARSER;
	}
}
// ---------------------------------------------------------------------------
PARSER_ERROR TVersionParser::SetVersionParserFailedEvent( VERSION_PARSER_EVENT inEvent )
{
	try
	{
		getParserEngine( _parserEngineAlias )->SetVersionParserFailedEvent( inEvent );
		return PE_EVENT_SET;
	}
	catch( EParserEngineUnknown& exc )
	{
		return PE_UNKNOWN_PARSER;
	}
}
// ---------------------------------------------------------------------------
PARSER_ERROR TVersionParser::SetVersionParserFailedEvent(
                                  std::string inParserAlias,
						 VERSION_PARSER_EVENT inEvent )
{
	try
	{
		getParserEngine( inParserAlias )->SetVersionParserFailedEvent( inEvent );
		return PE_EVENT_SET;
	}
	catch( EParserEngineUnknown& exc )
	{
		return PE_UNKNOWN_PARSER;
	}
}
// ---------------------------------------------------------------------------
int TVersionParser::VersionCount()
{
	return getParserEngine( _parserEngineAlias )->VersionCount();
}
// ---------------------------------------------------------------------------
PARSER_ERROR TVersionParser::Execute()
{
	return getParserEngine( _parserEngineAlias )->Execute();
}
// ---------------------------------------------------------------------------
PARSER_ERROR TVersionParser::ExecuteParser( std::string inParserAlias )
{
	try
	{
		return getParserEngine( inParserAlias )->Execute();
	}
	catch( EParserEngineUnknown& exc )
	{
		return PE_UNKNOWN_PARSER;
	}
}
// ---------------------------------------------------------------------------
PARSER_ERROR TVersionParser::ExecuteParserAtVersion(
					std::string inParserAlias,
					std::string inVersionAlias )
{
	try
	{
		return getParserEngine( inParserAlias )->ExecuteAtVersion( inVersionAlias );
	}
	catch( EParserEngineUnknown& exc )
	{
		return PE_UNKNOWN_PARSER;
	}
	catch( EParserVersionUnknown& exc )
	{
		return PE_UNKNOWN_VERSION;
	}
}
// ---------------------------------------------------------------------------
PARSER_ERROR TVersionParser::ExecuteParserFromVersion(
					std::string inParserAlias,
					std::string inVersionAlias )
{
	try
	{
		return getParserEngine( inParserAlias )->ExecuteFromVersion( inVersionAlias );
	}
	catch( EParserEngineUnknown& exc )
	{
		return PE_UNKNOWN_PARSER;
	}
	catch( EParserVersionUnknown& exc )
	{
		return PE_UNKNOWN_VERSION;
	}
}
// ---------------------------------------------------------------------------
std::string TVersionParser::ErrorMessage()
{
	return getParserEngine( _parserEngineAlias )->ErrorMessage();
}
// ---------------------------------------------------------------------------
std::string TVersionParser::ErrorMessageWithParser( std::string inParserAlias )
{
	try
	{
		return getParserEngine( inParserAlias )->ErrorMessage();
	}
	catch( EParserEngineUnknown& exc )
	{
		std::string result;
		return result;
	}
}
// ---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PRIVATE METHODS
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void TVersionParser::initParserEngines()
{
	populateParserEngineList( _parserEngines );
}
// ---------------------------------------------------------------------------
void TVersionParser::closeParserEngines()
{
	for( _parserEnginesIT  = _parserEngines.begin();
		 _parserEnginesIT != _parserEngines.end();
		 _parserEnginesIT++ )
	{
		delete _parserEnginesIT->second;
	}

	_parserEngines.clear();
}
// ---------------------------------------------------------------------------
void TVersionParser::setDefaultParserEngine()
{
	_parserEngineAlias = "MENUMATE";
}
// ---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  Methods that have to be modified when adding new ParserEngines.
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void TVersionParser::populateParserEngineList( PARSER_ENGINE_LIST& outParserEngines )
{
	outParserEngines["MENUMATE"] = new TParserEngineMenumate();
}
// ---------------------------------------------------------------------------
PARSER_ERROR TVersionParser::setParserEngine( std::string inParserAlias )
{
	std::string upperCaseAlias = upperCaseString( inParserAlias );

	if( equalStrings( upperCaseAlias, "MENUMATE" )  )
	{
		_parserEngineAlias = upperCaseAlias;

		return PE_PARSER_SET;
	}
	else
	{
		throw new EParserEngineUnknown( UnicodeString( inParserAlias.c_str() ) );
	}
}
// ---------------------------------------------------------------------------
TParserEngine* TVersionParser::getParserEngine( std::string inParserAlias )
{
	std::string upperCaseAlias = upperCaseString( inParserAlias );

	if( equalStrings( upperCaseAlias, "MENUMATE" )  )
	{
		return _parserEngines["MENUMATE"];
	}
	else
	{
		throw new EParserEngineUnknown( UnicodeString( inParserAlias.c_str() ) );
	}
}
// ---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  Methods that use TStringTools.
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
int TVersionParser::equalStrings(
						std::string inStr1,
						std::string inStr2 )
{
	return _stringTools->EqualStrings( inStr1, inStr2 );
}
// ---------------------------------------------------------------------------
std::string TVersionParser::upperCaseString( std::string inStr )
{
	return _stringTools->UpperCaseString( inStr );
}
// ---------------------------------------------------------------------------

