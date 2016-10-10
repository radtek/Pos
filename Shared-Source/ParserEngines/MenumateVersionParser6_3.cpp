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
    //6.30
    void TApplyParser::upgrade6_30Tables()
    {
        update6_30Tables();
    }

    //::::::::::::::::::::::::Version 6.30::::::::::::::::::::::::::::::::::::::::::
    void TApplyParser::update6_30Tables()
    {
        UpdateArcBillTable6_30(_dbControl);
    }
    //-------------------------------------------------------------------------------------

    void TApplyParser::UpdateArcBillTable6_30( TDBControl* const inDBControl )
    {

        if ( !fieldExists("ARCBILL", "Z_KEY", inDBControl ) )
        {
            executeQuery("ALTER TABLE ARCBILL ADD Z_KEY INTEGER;", inDBControl );
        }
    }
}
