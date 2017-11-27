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
        case pv6_30 : return apply6_30( inDBControl );
        case pv6_31 : return apply6_31( inDBControl );
        case pv6_32 : return apply6_32( inDBControl );
        case pv6_33 : return apply6_33( inDBControl );
        case pv6_34 : return apply6_34( inDBControl );
        case pv6_35 : return apply6_35( inDBControl );
        case pv6_36 : return apply6_36( inDBControl );
        case pv6_37 : return apply6_37( inDBControl );
        case pv6_38 : return apply6_38( inDBControl );
        case pv6_39 : return apply6_39( inDBControl );
        case pv6_40 : return apply6_40( inDBControl );
        case pv6_41 : return apply6_41( inDBControl );
        case pv6_42 : return apply6_42( inDBControl );
        case pv6_43 : return apply6_43( inDBControl );
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


}
////::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

