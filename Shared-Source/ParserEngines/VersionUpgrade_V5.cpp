//---------------------------------------------------------------------------

#pragma hdrstop

#include "MenumateVersionParser.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

namespace MenumateVersionParser
{

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

}
