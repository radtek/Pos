#pragma hdrstop
#include "MenumateVersionParser.h"
#include <sstream>
#include <Predicates.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//Macro to convert a int to a string
#ifndef SSTR( x )
#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
( std::ostringstream() << std::dec << x ) ).str()
#endif
namespace MenumateVersionParser
{
//6.60
void TApplyParser::upgrade6_60Tables()
{
    update6_60Tables();
}

//::::::::::::::::::::::::Version 6.60:::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_60Tables()
{
    Alter6_60Tables(_dbControl);

}

//---------------------------------------------------------------------------
void TApplyParser::Alter6_60Tables(TDBControl* const inDBControl)
{
    if (!fieldExists( "TABLES", "ACCEPT_OO", _dbControl ) )
	{
        executeQuery ( "ALTER TABLE TABLES ADD ACCEPT_OO CHAR(1) DEFAULT 'F';", inDBControl);
	}
     if ( fieldExists( "TABLES", "ACCEPT_OO", _dbControl ) )
    {
        executeQuery ( "UPDATE TABLES SET ACCEPT_OO = 'F';", inDBControl);
    }
}
}