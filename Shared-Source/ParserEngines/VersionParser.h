//---------------------------------------------------------------------------

#ifndef VersionParserH
#define VersionParserH

//---------------------------------------------------------------------------

#include <map>
#include <vector>

#include "MM_DBCore.h"
#include "MMLogging.h"

#include <math.h>
#include "StringTools.h"
#include "PointsTransaction.h"

//---------------------------------------------------------------------------

typedef __int32 PARSER_ERROR;

const PE_NO_ERROR        = 0x0001;
const PE_PARSER_SET      = 0x0002;
const PE_UNKNOWN_PARSER  = 0x0003;
const PE_UNKNOWN_VERSION = 0x0004;
const PE_EVENT_SET       = 0x0005;

const PE_VERSION_DB_SET               = 0x1001;
const PE_VERSION_ALREADY_APPLIED      = 0x1002;
const PE_VERSION_PREVIOUS_NOT_APPLIED = 0x1003;
const PE_VERSION_UPGRADE_SUCCEED      = 0x1004;
const PE_VERSION_UPGRADE_FAILED       = 0x1005;
typedef PARSER_ERROR (*VERSION_PARSER)(Database::TDBControl* const);
typedef         void (*VOID_VERSION_PARSER)(Database::TDBControl* const);

typedef void (*VERSION_PARSER_EVENT)(
					std::string version,
					std::string msg);

typedef std::map<std::string, VERSION_PARSER>      PARSER_FUNCTION_MAP;
typedef std::map<std::string, VOID_VERSION_PARSER> VOID_PARSER_FUNCTION_MAP;
typedef std::map<std::string, std::string>         TABLE_GENERATOR_MAP;

typedef PARSER_FUNCTION_MAP::const_iterator PARSER_MAP_IT;
typedef __fastcall bool (*PARSER_EXECUTE)(PARSER_MAP_IT);

extern std::string _errorMsg;

//---------------------------------------------------------------------------


#endif




