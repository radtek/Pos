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
void TApplyParser::upgrade6_30Tables()
{
    update6_30Tables();
}
//::::::::::::::::::::::::Version 6.20::::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_30Tables()
{
    UpdatePaymentTables6_30(_dbControl);
}

void TApplyParser::UpdatePaymentTables6_30( TDBControl* const inDBControl)
{
    if ( !fieldExists("DAYARCBILLPAY", "TIP_AMOUNT", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE DAYARCBILLPAY ADD TIP_AMOUNT NUMERIC(15,4) DEFAULT 0;",
		inDBControl );
	}
    if ( !fieldExists("ARCBILLPAY", "TIP_AMOUNT", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE ARCBILLPAY ADD TIP_AMOUNT NUMERIC(15,4) DEFAULT 0;",
		inDBControl );
	}
}

}
