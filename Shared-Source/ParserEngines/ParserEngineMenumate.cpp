//---------------------------------------------------------------------------

#pragma hdrstop

#include "ParserEngineMenumate.h"

#include "MenumateVersionParser.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

PARSER_ERROR apply5_18( TDBControl* inDBControl );
PARSER_ERROR apply5_19( TDBControl* inDBControl );
PARSER_ERROR apply5_20( TDBControl* inDBControl );
PARSER_ERROR apply5_21( TDBControl* inDBControl );
PARSER_ERROR apply5_22( TDBControl* inDBControl );
PARSER_ERROR apply5_23( TDBControl* inDBControl );
PARSER_ERROR apply5_24( TDBControl* inDBControl );
PARSER_ERROR apply5_25( TDBControl* inDBControl );
PARSER_ERROR apply5_26( TDBControl* inDBControl );
PARSER_ERROR apply5_27( TDBControl* inDBControl );
PARSER_ERROR apply5_28( TDBControl* inDBControl );
PARSER_ERROR apply5_29( TDBControl* inDBControl );
PARSER_ERROR apply5_30( TDBControl* inDBControl );
PARSER_ERROR apply5_31( TDBControl* inDBControl );
PARSER_ERROR apply5_32( TDBControl* inDBControl );
PARSER_ERROR apply5_33( TDBControl* inDBControl );
PARSER_ERROR apply5_34( TDBControl* inDBControl );
PARSER_ERROR apply5_35( TDBControl* inDBControl );
PARSER_ERROR apply5_36( TDBControl* inDBControl );
PARSER_ERROR apply5_37( TDBControl* inDBControl );
PARSER_ERROR apply5_38( TDBControl* inDBControl );

// Version 6.x
PARSER_ERROR apply6_00( TDBControl* inDBControl );
PARSER_ERROR apply6_01( TDBControl* inDBControl );
PARSER_ERROR apply6_02( TDBControl* inDBControl );
PARSER_ERROR apply6_03( TDBControl* inDBControl );
PARSER_ERROR apply6_04( TDBControl* inDBControl );
PARSER_ERROR apply6_05( TDBControl* inDBControl );
PARSER_ERROR apply6_06( TDBControl* inDBControl );
PARSER_ERROR apply6_07( TDBControl* inDBControl );
PARSER_ERROR apply6_08( TDBControl* inDBControl );
PARSER_ERROR apply6_09( TDBControl* inDBControl );
PARSER_ERROR apply6_10( TDBControl* inDBControl );
PARSER_ERROR apply6_11( TDBControl* inDBControl );
PARSER_ERROR apply6_12( TDBControl* inDBControl );
PARSER_ERROR apply6_13( TDBControl* inDBControl );
PARSER_ERROR apply6_14( TDBControl* inDBControl );
PARSER_ERROR apply6_15( TDBControl* inDBControl );
PARSER_ERROR apply6_16( TDBControl* inDBControl );
PARSER_ERROR apply6_17( TDBControl* inDBControl );
PARSER_ERROR apply6_18( TDBControl* inDBControl );
PARSER_ERROR apply6_19( TDBControl* inDBControl );
PARSER_ERROR apply6_20( TDBControl* inDBControl );
PARSER_ERROR apply6_21( TDBControl* inDBControl );
PARSER_ERROR apply6_22( TDBControl* inDBControl );
PARSER_ERROR apply6_23( TDBControl* inDBControl );
PARSER_ERROR apply6_24( TDBControl* inDBControl );
PARSER_ERROR apply6_25( TDBControl* inDBControl );
PARSER_ERROR apply6_26( TDBControl* inDBControl );
PARSER_ERROR apply6_27( TDBControl* inDBControl );
PARSER_ERROR apply6_28( TDBControl* inDBControl );
PARSER_ERROR apply6_29( TDBControl* inDBControl );
PARSER_ERROR apply6_30( TDBControl* inDBControl );
PARSER_ERROR apply6_31( TDBControl* inDBControl );
PARSER_ERROR apply6_32( TDBControl* inDBControl );
PARSER_ERROR apply6_33( TDBControl* inDBControl );
PARSER_ERROR apply6_34( TDBControl* inDBControl );
PARSER_ERROR apply6_35( TDBControl* inDBControl );
PARSER_ERROR apply6_36( TDBControl* inDBControl );
PARSER_ERROR apply6_37( TDBControl* inDBControl );
//::::::::::::::::::::::::::::::::::::::::::::::::::
//  TParserEngineMenumate
//::::::::::::::::::::::::::::::::::::::::::::::::::
TParserEngineMenumate::TParserEngineMenumate() : TParserEngine()
{
	initVersions();
}
// ---------------------------------------------------------------------------
TParserEngineMenumate::~TParserEngineMenumate()
{
}
// ---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC METHODS.
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PRIVATE METHODS.
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  Members and Methods that have to be modified when adding new versions.
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void TParserEngineMenumate::populateVersionList( PARSER_FUNCTION_MAP& outVersions )
{
    outVersions["5.18"] = apply5_18;
    outVersions["5.19"] = apply5_19;
    outVersions["5.20"] = apply5_20;
    outVersions["5.21"] = apply5_21;
    outVersions["5.22"] = apply5_22;
    outVersions["5.23"] = apply5_23;
    outVersions["5.24"] = apply5_24;
    outVersions["5.24"] = apply5_24;
    outVersions["5.25"] = apply5_25;
    outVersions["5.26"] = apply5_26;
    outVersions["5.27"] = apply5_27;
    outVersions["5.28"] = apply5_28;
    outVersions["5.29"] = apply5_29;
	outVersions["5.30"] = apply5_30;
	outVersions["5.31"] = apply5_31;
	outVersions["5.32"] = apply5_32;
	outVersions["5.33"] = apply5_33;
	outVersions["5.34"] = apply5_34;
	outVersions["5.35"] = apply5_35;
	outVersions["5.36"] = apply5_36;
	outVersions["5.37"] = apply5_37;
	outVersions["5.38"] = apply5_38;
	outVersions["6.00"] = apply6_00;
	outVersions["6.01"] = apply6_01;
	outVersions["6.02"] = apply6_02;
	outVersions["6.03"] = apply6_03;
	outVersions["6.04"] = apply6_04;
	outVersions["6.05"] = apply6_05;
	outVersions["6.06"] = apply6_06;
	outVersions["6.07"] = apply6_07;
    outVersions["6.08"] = apply6_08;
    outVersions["6.09"] = apply6_09;
    outVersions["6.10"] = apply6_10;
    outVersions["6.11"] = apply6_11;
    outVersions["6.12"] = apply6_12;
    outVersions["6.13"] = apply6_13;
    outVersions["6.14"] = apply6_14;
    outVersions["6.15"] = apply6_15;
    outVersions["6.16"] = apply6_16;
    outVersions["6.17"] = apply6_17;
    outVersions["6.18"] = apply6_18;
    outVersions["6.19"] = apply6_19;
    outVersions["6.20"] = apply6_20;
    outVersions["6.21"] = apply6_21;
    outVersions["6.22"] = apply6_22;
    outVersions["6.23"] = apply6_23;
	outVersions["6.24"] = apply6_24;
    outVersions["6.25"] = apply6_25;
    outVersions["6.26"] = apply6_26;
    outVersions["6.27"] = apply6_27;
    outVersions["6.28"] = apply6_28;
    outVersions["6.29"] = apply6_29;
    outVersions["6.30"] = apply6_30;
    outVersions["6.31"] = apply6_31;
    outVersions["6.32"] = apply6_32;
    outVersions["6.33"] = apply6_33;
    outVersions["6.34"] = apply6_34;
    outVersions["6.35"] = apply6_35;
    outVersions["6.36"] = apply6_36;
    outVersions["6.37"] = apply6_37;
}
// ---------------------------------------------------------------------------
int TParserEngineMenumate::VersionCount()
{
    return _versions.size();
}
// ---------------------------------------------------------------------------



using MenumateVersionParser::TApplyParser;

PARSER_ERROR apply5_18( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv5_18,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply5_19( TDBControl* inDBControl )
{
    return TApplyParser::Instance()->Apply( MenumateVersionParser::pv5_19,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply5_20( TDBControl* inDBControl )
{
    return TApplyParser::Instance()->Apply( MenumateVersionParser::pv5_20,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply5_21( TDBControl* inDBControl )
{
    return TApplyParser::Instance()->Apply( MenumateVersionParser::pv5_21,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply5_22( TDBControl* inDBControl )
{
    return TApplyParser::Instance()->Apply( MenumateVersionParser::pv5_22,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply5_23( TDBControl* inDBControl )
{
    return TApplyParser::Instance()->Apply( MenumateVersionParser::pv5_23,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply5_24( TDBControl* inDBControl )
{
    return TApplyParser::Instance()->Apply( MenumateVersionParser::pv5_24,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply5_25( TDBControl* inDBControl )
{
    return TApplyParser::Instance()->Apply( MenumateVersionParser::pv5_25,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply5_26( TDBControl* inDBControl )
{
    return TApplyParser::Instance()->Apply( MenumateVersionParser::pv5_26,  inDBControl );
}

// ---------------------------------------------------------------------------
PARSER_ERROR apply5_27( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv5_27,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply5_28( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv5_28,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply5_29( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv5_29,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply5_30( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv5_30,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply5_31( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv5_31,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply5_32( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv5_32,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply5_33( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv5_33,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply5_34( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv5_34,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply5_35( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv5_35,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply5_36( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv5_36,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply5_37( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv5_37,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply5_38( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv5_38,  inDBControl );
}
PARSER_ERROR apply6_00( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_00,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply6_01( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_01,  inDBControl );
}
PARSER_ERROR apply6_02( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_02,  inDBControl );
}

PARSER_ERROR apply6_03( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_03,  inDBControl );
}

PARSER_ERROR apply6_04( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_04,  inDBControl );
}

PARSER_ERROR apply6_05( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_05,  inDBControl );
}

// ---------------------------------------------------------------------------
PARSER_ERROR apply6_06( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_06,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply6_07( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_07,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply6_08( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_08,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply6_09( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_09,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply6_10( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_10,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply6_11( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_11,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply6_12( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_12,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply6_13( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_13,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply6_14( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_14,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply6_15( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_15,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply6_16( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_16,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply6_17( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_17,  inDBControl );
}

PARSER_ERROR apply6_18( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_18,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply6_19( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_19,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply6_20( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_20,  inDBControl );
}
// ---------------------------------------------------------------------------
PARSER_ERROR apply6_21( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_21,  inDBControl );
}
//---------------------------------
PARSER_ERROR apply6_22( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_22,  inDBControl );
}
//---------------------------------
PARSER_ERROR apply6_23( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_23,  inDBControl );
}
//---------------------------------
PARSER_ERROR apply6_24( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_24,  inDBControl );
}
PARSER_ERROR apply6_25( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_25,  inDBControl );
}
PARSER_ERROR apply6_26( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_26,  inDBControl );
}
//---------------------------------
PARSER_ERROR apply6_27( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_27,  inDBControl );
}
//---------------------------------
PARSER_ERROR apply6_28( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_28,  inDBControl );
}
//---------------------------------
PARSER_ERROR apply6_29( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_29,  inDBControl );
}
//---------------------------------
PARSER_ERROR apply6_30( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_30,  inDBControl );
}
//---------------------------------
PARSER_ERROR apply6_31( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_31,  inDBControl );
}
//---------------------------------
PARSER_ERROR apply6_32( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_32,  inDBControl );
}
//---------------------------------
PARSER_ERROR apply6_33( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_33,  inDBControl );
}
//---------------------------------
PARSER_ERROR apply6_34( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_34,  inDBControl );
}
//---------------------------------
PARSER_ERROR apply6_35( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_35,  inDBControl );
}
//---------------------------------
PARSER_ERROR apply6_36( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_36,  inDBControl );
}
//---------------------------------
PARSER_ERROR apply6_37( TDBControl* inDBControl )
{
	return TApplyParser::Instance()->Apply( MenumateVersionParser::pv6_37,  inDBControl );
}
//---------------------------------
