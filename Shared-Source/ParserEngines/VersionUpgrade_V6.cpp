//---------------------------------------------------------------------------

#pragma hdrstop

#include "MenumateVersionParser.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

namespace MenumateVersionParser
{
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
////::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
PARSER_ERROR TApplyParser::apply6_30( TDBControl* const inDBControl )
{
  	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.30";
	previous_version = "6.29";
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
		upgrade6_30Tables();
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
PARSER_ERROR TApplyParser::apply6_31( TDBControl* const inDBControl )
{
  	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.31";
	previous_version = "6.30";
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
		upgrade6_31Tables();
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
PARSER_ERROR TApplyParser::apply6_32( TDBControl* const inDBControl )
{
  	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.32";
	previous_version = "6.31";
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
		upgrade6_32Tables();
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
PARSER_ERROR TApplyParser::apply6_33( TDBControl* const inDBControl )
{
  	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.33";
	previous_version = "6.32";
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
		upgrade6_33Tables();
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
PARSER_ERROR TApplyParser::apply6_34( TDBControl* const inDBControl )
{
  	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.34";
	previous_version = "6.33";
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
		upgrade6_34Tables();
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
PARSER_ERROR TApplyParser::apply6_35( TDBControl* const inDBControl )
{
  	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.35";
	previous_version = "6.34";
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
		upgrade6_35Tables();
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
PARSER_ERROR TApplyParser::apply6_36( TDBControl* const inDBControl )
{
  	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.36";
	previous_version = "6.35";
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
		upgrade6_36Tables();
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
PARSER_ERROR TApplyParser::apply6_37( TDBControl* const inDBControl )
{
  	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.37";
	previous_version = "6.36";
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
		upgrade6_37Tables();
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
PARSER_ERROR TApplyParser::apply6_38( TDBControl* const inDBControl )
{
  	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.38";
	previous_version = "6.37";
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
		upgrade6_38Tables();
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
PARSER_ERROR TApplyParser::apply6_39( TDBControl* const inDBControl )
{
  	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.39";
	previous_version = "6.38";
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
		upgrade6_39Tables();
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
PARSER_ERROR TApplyParser::apply6_40( TDBControl* const inDBControl )
{
  	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.40";
	previous_version = "6.39";
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
		upgrade6_40Tables();
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
//-----------------------------------------------------------------------------------
PARSER_ERROR TApplyParser::apply6_41( TDBControl* const inDBControl )
{
  	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.41";
	previous_version = "6.40";
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
		upgrade6_41Tables();
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
PARSER_ERROR TApplyParser::apply6_42( TDBControl* const inDBControl )
{
  	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.42";
	previous_version = "6.41";
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
		upgrade6_42Tables();
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
//-----------------------------------------------------------------------------------
PARSER_ERROR TApplyParser::apply6_43( TDBControl* const inDBControl )
{
  	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.43";
	previous_version = "6.42";
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
		upgrade6_43Tables();
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
//-----------------------------------------------------------------------------------
PARSER_ERROR TApplyParser::apply6_44( TDBControl* const inDBControl )
{
  	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.44";
	previous_version = "6.43";
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
		upgrade6_44Tables();
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
//-----------------------------------------------------------------------------------
PARSER_ERROR TApplyParser::apply6_45( TDBControl* const inDBControl )
{
  	_dbControl = inDBControl;
	_errorMsg.clear();
	current_version  = "6.45";
	previous_version = "6.44";
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
		upgrade6_45Tables();
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
