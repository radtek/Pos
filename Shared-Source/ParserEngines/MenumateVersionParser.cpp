//---------------------------------------------------------------------------

#pragma hdrstop

#include "MenumateVersionParser.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

namespace MenumateVersionParser
{

//---------------------------------------------------------------------------

// Singleton Object Instance set to NULL.
TApplyParser* TApplyParser::applyParser = NULL;

//::::::::::::::::::::::::::::::::::::::
// TApplyParser: PUBLIC
//::::::::::::::::::::::::::::::::::::::
TApplyParser* TApplyParser::Instance()
{
    if( applyParser == NULL )
    {
        applyParser = new TApplyParser();
    }

    return applyParser;
}
//................................................................
PARSER_ERROR TApplyParser::Apply(
                                TParserVersions inVersion,
                              TDBControl* const inDBControl )
{
    switch( inVersion )
    {
        // Version 5.x
        case pv5_18 : return apply5_18( inDBControl );
        case pv5_19 : return apply5_19( inDBControl );
        case pv5_20 : return apply5_20( inDBControl );
        case pv5_21 : return apply5_21( inDBControl );
        case pv5_22 : return apply5_22( inDBControl );
        case pv5_23 : return apply5_23( inDBControl );
        case pv5_24 : return apply5_24( inDBControl );
        case pv5_25 : return apply5_25( inDBControl );
        case pv5_26 : return apply5_26( inDBControl );
        case pv5_27 : return apply5_27( inDBControl );
        case pv5_28 : return apply5_28( inDBControl );
        case pv5_29 : return apply5_29( inDBControl );
        case pv5_30 : return apply5_30( inDBControl );
        case pv5_31 : return apply5_31( inDBControl );
        case pv5_32 : return apply5_32( inDBControl );
        case pv5_33 : return apply5_33( inDBControl );
        case pv5_34 : return apply5_34( inDBControl );
        case pv5_35 : return apply5_35( inDBControl );
        case pv5_36 : return apply5_36( inDBControl );
        case pv5_37 : return apply5_37( inDBControl );
        case pv5_38 : return apply5_38( inDBControl );
        case pv6_00 : return apply6_00( inDBControl );
        case pv6_01 : return apply6_01( inDBControl );
        case pv6_02 : return apply6_02( inDBControl );
        case pv6_03 : return apply6_03( inDBControl );
        case pv6_04 : return apply6_04( inDBControl );
        case pv6_05 : return apply6_05( inDBControl );
        case pv6_06 : return apply6_06( inDBControl );
        case pv6_07 : return apply6_07( inDBControl );
        case pv6_08 : return apply6_08( inDBControl );
        case pv6_09 : return apply6_09( inDBControl );
        case pv6_10 : return apply6_10( inDBControl );
        case pv6_11 : return apply6_11( inDBControl );
        case pv6_12 : return apply6_12( inDBControl );
        case pv6_13 : return apply6_13( inDBControl );
        case pv6_14 : return apply6_14( inDBControl );
        case pv6_15 : return apply6_15( inDBControl );
        case pv6_16 : return apply6_16( inDBControl );
        case pv6_17 : return apply6_17( inDBControl );
        case pv6_18 : return apply6_18( inDBControl );
        case pv6_19 : return apply6_19( inDBControl );
        case pv6_20 : return apply6_20( inDBControl );
        case pv6_21 : return apply6_21( inDBControl );
        case pv6_22 : return apply6_22( inDBControl );
        case pv6_23 : return apply6_23( inDBControl );
        case pv6_24 : return apply6_24( inDBControl );
        case pv6_25 : return apply6_25( inDBControl );
        case pv6_26 : return apply6_26( inDBControl );
        case pv6_27 : return apply6_27( inDBControl );
        case pv6_28 : return apply6_28( inDBControl );
        case pv6_29 : return apply6_29( inDBControl );
	default: return PE_UNKNOWN_VERSION;
    }
}
//................................................................
std::string TApplyParser::LastErrorMsg()
{
	return _errorMsg;
}

//::::::::::::::::::::::::::::::::::::::
// TApplyParser: PRIVATE
//::::::::::::::::::::::::::::::::::::::

TApplyParser::TApplyParser()
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::
// Version 5.x
PARSER_ERROR TApplyParser::apply5_18( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;

	_errorMsg.clear();

    current_version  = "5.18";
    previous_version = "5.17";

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}

    //...............................................

	try
	{
		// There is no creating new tables in version 5.18 but altering existing ones.
        upgrade5_18Tables();
		updateVersionNumber( current_version, _dbControl );

        _errorMsg.append( "Updates have been succesfully applied." );
		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//................................................................
PARSER_ERROR TApplyParser::apply5_19( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;

	_errorMsg.clear();

    current_version  = "5.19";
    previous_version = "5.18";

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}

    //...............................................

	try
	{
        upgrade5_19Tables();
		updateVersionNumber( current_version, _dbControl );

        _errorMsg.append( "Updates have been succesfully applied." );
		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//................................................................
PARSER_ERROR TApplyParser::apply5_20( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;

	_errorMsg.clear();

    current_version  = "5.20";
    previous_version = "5.19";

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}

    //...............................................

	try
	{
        // There is no creating new tables in version 5.20 but updating existing ones.
		upgrade5_20Tables();
		updateVersionNumber( current_version, _dbControl );

        _errorMsg.append( "Updates have been succesfully applied." );
		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//................................................................
PARSER_ERROR TApplyParser::apply5_21( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;

	_errorMsg.clear();

    current_version  = "5.21";
    previous_version = "5.20";

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}

    //...............................................

	try
	{
        // There is no creating new tables in version 5.21 but updating existing ones.
		upgrade5_21Tables();
        updateVersionNumber( current_version, _dbControl );

        _errorMsg.append( "Updates have been succesfully applied." );
        return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//................................................................
PARSER_ERROR TApplyParser::apply5_22( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;

	_errorMsg.clear();

    current_version  = "5.22";
    previous_version = "5.21";

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}

    //...............................................

	try
	{
        // There is no creating new tables in version 5.22 but updating existing ones.
		upgrade5_22Tables();
        updateVersionNumber( current_version, _dbControl );

        _errorMsg.append( "Updates have been succesfully applied." );
		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//................................................................
PARSER_ERROR TApplyParser::apply5_23( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;

	_errorMsg.clear();

    current_version  = "5.23";
    previous_version = "5.22";

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}

    //...............................................

	try
	{
		upgrade5_23Tables();
		updateVersionNumber( current_version, _dbControl );

        _errorMsg.append( "Updates have been succesfully applied." );
		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//................................................................
PARSER_ERROR TApplyParser::apply5_24( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;

	_errorMsg.clear();

    current_version  = "5.24";
    previous_version = "5.23";

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}

    //...............................................

	try
	{
        upgrade5_24Tables();
		updateVersionNumber( current_version, _dbControl );

        _errorMsg.append( "Updates have been succesfully applied." );
		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//................................................................
PARSER_ERROR TApplyParser::apply5_25( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;

	_errorMsg.clear();

    current_version  = "5.25";
    previous_version = "5.24";

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}

    //...............................................

	try
	{
        upgrade5_25Tables();
		updateVersionNumber( current_version, _dbControl );

		_errorMsg.append( "Updates have been succesfully applied." );

		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//................................................................
PARSER_ERROR TApplyParser::apply5_26( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;

	_errorMsg.clear();

	current_version  = "5.26";
    previous_version = "5.25";

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}

    //...............................................

	try
	{
        upgrade5_26Tables();
		updateVersionNumber( current_version, _dbControl );

		_errorMsg.append( "Updates have been succesfully applied." );

		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//................................................................
PARSER_ERROR TApplyParser::apply5_27( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;

	_errorMsg.clear();

	current_version  = "5.27";
    previous_version = "5.26";

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}

    //...............................................

	try
	{
        upgrade5_27Tables();
		updateVersionNumber( current_version, _dbControl );

		_errorMsg.append( "Updates have been succesfully applied." );

		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//................................................................
PARSER_ERROR TApplyParser::apply5_28( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;

	_errorMsg.clear();

	current_version  = "5.28";
    previous_version = "5.27";

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}

    //...............................................

	try
	{
        upgrade5_28Tables();
		updateVersionNumber( current_version, _dbControl );

		_errorMsg.append( "Updates have been succesfully applied." );

		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//................................................................
PARSER_ERROR TApplyParser::apply5_29( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;

	_errorMsg.clear();

	current_version  = "5.29";
	previous_version = "5.28";

	//:::::::::::::::::::::::::::::::::::::::::::::::

	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}

	//:::::::::::::::::::::::::::::::::::::::::::::::

	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}

	//...............................................

	try
	{
		upgrade5_29Tables();
		updateVersionNumber( current_version, _dbControl );

		_errorMsg.append( "Updates have been succesfully applied." );

		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//................................................................
PARSER_ERROR TApplyParser::apply5_30( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;

	_errorMsg.clear();

	current_version  = "5.30";
	previous_version = "5.29";

	//:::::::::::::::::::::::::::::::::::::::::::::::

	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}

	//:::::::::::::::::::::::::::::::::::::::::::::::

	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}

	//...............................................

	try
	{
		upgrade5_30Tables();
		updateVersionNumber( current_version, _dbControl );

		_errorMsg.append( "Updates have been succesfully applied." );

		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//................................................................
PARSER_ERROR TApplyParser::apply5_31( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;

	_errorMsg.clear();

	current_version  = "5.31";
	previous_version = "5.30";

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}

    //...............................................

	try
	{
		upgrade5_31Tables();
		updateVersionNumber( current_version, _dbControl );

		_errorMsg.append( "Updates have been succesfully applied." );

		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//................................................................
PARSER_ERROR TApplyParser::apply5_32( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;

	_errorMsg.clear();

	current_version  = "5.32";
	previous_version = "5.31";

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}

	//:::::::::::::::::::::::::::::::::::::::::::::::

	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}

    //...............................................

	try
	{
		upgrade5_32Tables();
		updateVersionNumber( current_version, _dbControl );

		_errorMsg.append( "Updates have been succesfully applied." );

		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//................................................................
PARSER_ERROR TApplyParser::apply5_33( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;

	_errorMsg.clear();

	current_version  = "5.33";
	previous_version = "5.32";

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}

	//:::::::::::::::::::::::::::::::::::::::::::::::

	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}

    //...............................................

	try
	{
		upgrade5_33Tables();
		updateVersionNumber( current_version, _dbControl );

		_errorMsg.append( "Updates have been succesfully applied." );

		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//................................................................
PARSER_ERROR TApplyParser::apply5_34( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;

	_errorMsg.clear();

	current_version  = "5.34";
	previous_version = "5.33";

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}

	//:::::::::::::::::::::::::::::::::::::::::::::::

	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}

    //...............................................

	try
	{
		upgrade5_34Tables();
		updateVersionNumber( current_version, _dbControl );

		_errorMsg.append( "Updates have been succesfully applied." );

		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//................................................................
PARSER_ERROR TApplyParser::apply5_35( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;

	_errorMsg.clear();

	current_version  = "5.35";
	previous_version = "5.34";

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}

	//:::::::::::::::::::::::::::::::::::::::::::::::

	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}

    //...............................................

	try
	{
		upgrade5_35Tables();
		updateVersionNumber( current_version, _dbControl );

		_errorMsg.append( "Updates have been succesfully applied." );

		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//................................................................
PARSER_ERROR TApplyParser::apply5_36( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;

	_errorMsg.clear();

	current_version  = "5.36";
	previous_version = "5.35";

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}

	//:::::::::::::::::::::::::::::::::::::::::::::::

	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}

    //...............................................

	try
	{
		upgrade5_36Tables();
		updateVersionNumber( current_version, _dbControl );

		_errorMsg.append( "Updates have been succesfully applied." );

		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//................................................................
PARSER_ERROR TApplyParser::apply5_37( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;

	_errorMsg.clear();

	current_version  = "5.37";
	previous_version = "5.36";

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}

	//:::::::::::::::::::::::::::::::::::::::::::::::

	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}

    //...............................................

	try
	{
		upgrade5_37Tables();
		updateVersionNumber( current_version, _dbControl );

		_errorMsg.append( "Updates have been succesfully applied." );

		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//................................................................
PARSER_ERROR TApplyParser::apply5_38( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;

	_errorMsg.clear();

	current_version  = "5.38";
	previous_version = "5.37";

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}

	//:::::::::::::::::::::::::::::::::::::::::::::::

	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}

    //...............................................

	try
	{
		upgrade5_38Tables();
		updateVersionNumber( current_version, _dbControl );

		_errorMsg.append( "Updates have been succesfully applied." );

		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//................................................................
PARSER_ERROR TApplyParser::apply6_00( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;



	_errorMsg.clear();

	current_version  = "6.00";
	previous_version = "5.38";

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}

	//:::::::::::::::::::::::::::::::::::::::::::::::

	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}

    //...............................................

	try
	{
		upgrade6_00Tables();
		updateVersionNumber( current_version, _dbControl );

		_errorMsg.append( "Updates have been succesfully applied." );

		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//...............................................................
PARSER_ERROR TApplyParser::apply6_01( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;



	_errorMsg.clear();

	current_version  = "6.01";
	previous_version = "6.00";

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}

	//:::::::::::::::::::::::::::::::::::::::::::::::

	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}

    //...............................................

	try
	{
		upgrade6_01Tables();
		updateVersionNumber( current_version, _dbControl );

		_errorMsg.append( "Updates have been succesfully applied." );

		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//...............................................................
PARSER_ERROR TApplyParser::apply6_02( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;



	_errorMsg.clear();

	current_version  = "6.02";
	previous_version = "6.01";

    //:::::::::::::::::::::::::::::::::::::::::::::::

	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}

	//:::::::::::::::::::::::::::::::::::::::::::::::

	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}

    //...............................................

	try
	{
		upgrade6_02Tables();
		updateVersionNumber( current_version, _dbControl );

		_errorMsg.append( "Updates have been succesfully applied." );

		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//.....................................................................
PARSER_ERROR TApplyParser::apply6_03( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.03";
	previous_version = "6.02";
	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}
	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}
	try
	{
		upgrade6_03Tables();
		updateVersionNumber( current_version, _dbControl );
		_errorMsg.append( "Updates have been succesfully applied." );
		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//.....................................................................
PARSER_ERROR TApplyParser::apply6_04( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.04";
	previous_version = "6.03";
	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}
	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}
	try
	{
		upgrade6_04Tables();
		updateVersionNumber( current_version, _dbControl );
		_errorMsg.append( "Updates have been succesfully applied." );
		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//...............................................................
PARSER_ERROR TApplyParser::apply6_05( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.05";
	previous_version = "6.04";
	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}
	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}
	try
	{
		upgrade6_05Tables();
		updateVersionNumber( current_version, _dbControl );
		_errorMsg.append( "Updates have been succesfully applied." );
		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//...............................................................
PARSER_ERROR TApplyParser::apply6_06( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.06";
	previous_version = "6.05";
	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}
	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}
	try
	{
		upgrade6_06Tables();
		updateVersionNumber( current_version, _dbControl );
		_errorMsg.append( "Updates have been succesfully applied." );
		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//...............................................................
PARSER_ERROR TApplyParser::apply6_07( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.07";
	previous_version = "6.06";
	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}
	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}
	try
	{
		upgrade6_07Tables();
		updateVersionNumber( current_version, _dbControl );
		_errorMsg.append( "Updates have been succesfully applied." );
		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//.........................................................
PARSER_ERROR TApplyParser::apply6_08( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.08";
	previous_version = "6.07";
	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}
	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}
	try
	{
		upgrade6_08Tables();
		updateVersionNumber( current_version, _dbControl );
		_errorMsg.append( "Updates have been succesfully applied." );
		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//.........................................................
PARSER_ERROR TApplyParser::apply6_09( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.09";
	previous_version = "6.08";
	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}
	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}
	try
	{
		upgrade6_09Tables();
		updateVersionNumber( current_version, _dbControl );
		_errorMsg.append( "Updates have been succesfully applied." );
		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//.........................................................
PARSER_ERROR TApplyParser::apply6_10( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.10";
	previous_version = "6.09";
	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}
	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}
	try
	{
		upgrade6_10Tables();
		updateVersionNumber( current_version, _dbControl );
		_errorMsg.append( "Updates have been succesfully applied." );
		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//.........................................................
PARSER_ERROR TApplyParser::apply6_11( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.11";
	previous_version = "6.10";
	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}
	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}
	try
	{
		upgrade6_11Tables();
		updateVersionNumber( current_version, _dbControl );
		_errorMsg.append( "Updates have been succesfully applied." );
		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//.....................................................................
PARSER_ERROR TApplyParser::apply6_12( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;
	_errorMsg.clear();

	current_version  = "6.12";
	previous_version = "6.11";

	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}

	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}

	try
	{
		upgrade6_12Tables();
		updateVersionNumber( current_version, _dbControl );

		_errorMsg.append( "Updates have been succesfully applied." );

		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//.....................................................................
PARSER_ERROR TApplyParser::apply6_13( TDBControl* const inDBControl )
{
 _dbControl = inDBControl;
 _errorMsg.clear();
 current_version  = "6.13";
 previous_version = "6.12";
 // Previous Version.
 if( !updateAlreadyApplied( previous_version,  _dbControl ) )
 {
  _errorMsg.append( "Version " + previous_version + " required." );
  return PE_VERSION_PREVIOUS_NOT_APPLIED;
 }
 // Current Version.
 if( updateAlreadyApplied( current_version,  _dbControl ) )
 {
  _errorMsg.append( "Updates already succesfully applied." );
  return PE_VERSION_ALREADY_APPLIED;
 }
 try
 {
  upgrade6_13Tables();
  updateVersionNumber( current_version, _dbControl );
  _errorMsg.append( "Updates have been succesfully applied." );
  return PE_VERSION_UPGRADE_SUCCEED;
 }
 catch( Exception& exc )
 {
  _errorMsg.append( exc.Message.t_str() );
  return PE_VERSION_UPGRADE_FAILED;
 }
}
//.........................................................
PARSER_ERROR TApplyParser::apply6_14( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.14";
	previous_version = "6.13";

	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}

	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}

	try
	{
		upgrade6_14Tables();
		updateVersionNumber( current_version, _dbControl );

		_errorMsg.append( "Updates have been succesfully applied." );

		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::
PARSER_ERROR TApplyParser::apply6_15( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.15";
	previous_version = "6.14";
	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}
	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}
	try
	{
		upgrade6_15Tables();
		updateVersionNumber( current_version, _dbControl );
		_errorMsg.append( "Updates have been succesfully applied." );
		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::
PARSER_ERROR TApplyParser::apply6_16( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.16";
	previous_version = "6.15";
	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}
	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}
	try
	{
		upgrade6_16Tables();
		updateVersionNumber( current_version, _dbControl );
		_errorMsg.append( "Updates have been succesfully applied." );
		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::
PARSER_ERROR TApplyParser::apply6_17( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.17";
	previous_version = "6.16";
	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}
	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}
	try
	{
		upgrade6_17Tables();
		updateVersionNumber( current_version, _dbControl );
		_errorMsg.append( "Updates have been succesfully applied." );
		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
PARSER_ERROR TApplyParser::apply6_18( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.18";
	previous_version = "6.17";
	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}
	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}
	try
	{
		upgrade6_18Tables();
		updateVersionNumber( current_version, _dbControl );
		_errorMsg.append( "Updates have been succesfully applied." );
		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::
PARSER_ERROR TApplyParser::apply6_19( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.19";
	previous_version = "6.18";
	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}
	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}
	try
	{
		upgrade6_19Tables();
		updateVersionNumber( current_version, _dbControl );
		_errorMsg.append( "Updates have been succesfully applied." );
		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//:::::::::::::::::VoucherDiscount::::::::::::::::::::::::::::::::::::
PARSER_ERROR TApplyParser::apply6_20( TDBControl* const inDBControl )
{
	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.20";
	previous_version = "6.19";
	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}
	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}
	try
	{
		upgrade6_20Tables();
		updateVersionNumber( current_version, _dbControl );
		_errorMsg.append( "Updates have been succesfully applied." );
		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::
PARSER_ERROR TApplyParser::apply6_21( TDBControl* const inDBControl )
{
  	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.21";
	previous_version = "6.20";
	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}
	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}
	try
	{
		upgrade6_21Tables();
		updateVersionNumber( current_version, _dbControl );
		_errorMsg.append( "Updates have been succesfully applied." );
		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}

}
////::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
PARSER_ERROR TApplyParser::apply6_22( TDBControl* const inDBControl )
{
  	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.22";
	previous_version = "6.21";
	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}
	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}
	try
	{
		upgrade6_22Tables();
		updateVersionNumber( current_version, _dbControl );
		_errorMsg.append( "Updates have been succesfully applied." );
		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}

}
////::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
PARSER_ERROR TApplyParser::apply6_23( TDBControl* const inDBControl )
{
  	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.23";
	previous_version = "6.22";
	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}
	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}
	try
	{
		upgrade6_23Tables();
		updateVersionNumber( current_version, _dbControl );
		_errorMsg.append( "Updates have been succesfully applied." );
		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
////::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
PARSER_ERROR TApplyParser::apply6_24( TDBControl* const inDBControl )
{
  	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.24";
	previous_version = "6.23";
	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}
	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}
	try
	{
		upgrade6_24Tables();
		updateVersionNumber( current_version, _dbControl );
		_errorMsg.append( "Updates have been succesfully applied." );
		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
////::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
PARSER_ERROR TApplyParser::apply6_25( TDBControl* const inDBControl )
{
  	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.25";
	previous_version = "6.24";
	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}
	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}
	try
	{
		upgrade6_25Tables();
		updateVersionNumber( current_version, _dbControl );
		_errorMsg.append( "Updates have been succesfully applied." );
		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
////::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
PARSER_ERROR TApplyParser::apply6_26( TDBControl* const inDBControl )
{
  	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.26";
	previous_version = "6.25";
	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}
	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}
	try
	{
		upgrade6_26Tables();
		updateVersionNumber( current_version, _dbControl );
		_errorMsg.append( "Updates have been succesfully applied." );
		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
////::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
PARSER_ERROR TApplyParser::apply6_27( TDBControl* const inDBControl )
{
  	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.27";
	previous_version = "6.26";
	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}
	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}
	try
	{
		upgrade6_27Tables();
		updateVersionNumber( current_version, _dbControl );
		_errorMsg.append( "Updates have been succesfully applied." );
		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
////::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
PARSER_ERROR TApplyParser::apply6_28( TDBControl* const inDBControl )
{
  	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.28";
	previous_version = "6.27";
	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}
	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}
	try
	{
		upgrade6_28Tables();
		updateVersionNumber( current_version, _dbControl );
		_errorMsg.append( "Updates have been succesfully applied." );
		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
////::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
PARSER_ERROR TApplyParser::apply6_29( TDBControl* const inDBControl )
{
  	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.29";
	previous_version = "6.28";
	// Previous Version.
	if( !updateAlreadyApplied( previous_version,  _dbControl ) )
	{
		_errorMsg.append( "Version " + previous_version + " required." );
		return PE_VERSION_PREVIOUS_NOT_APPLIED;
	}
	// Current Version.
	if( updateAlreadyApplied( current_version,  _dbControl ) )
	{
		_errorMsg.append( "Updates already succesfully applied." );
		return PE_VERSION_ALREADY_APPLIED;
	}
	try
	{
		upgrade6_29Tables();
		updateVersionNumber( current_version, _dbControl );
		_errorMsg.append( "Updates have been succesfully applied." );
		return PE_VERSION_UPGRADE_SUCCEED;
	}
	catch( Exception& exc )
	{
		_errorMsg.append( exc.Message.t_str() );
		return PE_VERSION_UPGRADE_FAILED;
	}
}
}
////::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

