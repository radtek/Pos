//---------------------------------------------------------------------------

#pragma hdrstop

#include "StringTools.h"

#include <ctype.h>

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

// Singleton Object Instance set to NULL.
TStringTools* TStringTools::_stringTools = NULL;

//---------------------------------------------------------------------------

TStringTools* TStringTools::Instance()
{
	if( !_stringTools )
	{
		_stringTools = new TStringTools();
	}

	return _stringTools;
}
// ---------------------------------------------------------------------------
TStringTools::TStringTools()
{
}
// ---------------------------------------------------------------------------
TStringTools::~TStringTools()
{
}
// ---------------------------------------------------------------------------
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC METHODS.
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
size_t TStringTools::EqualStrings(
						std::string inStr1,
						std::string inStr2 )
{
	return inStr1.compare( inStr2 ) == 0;
}
// ---------------------------------------------------------------------------
std::string TStringTools::UpperCaseString( std::string inStr )
{
	std::string result;

	//:::::::::::::::::::::::::::::::::::::::::::::

	for( int pos = 0; inStr[pos] != '\0'; ++pos )
	{
		result.push_back( toupper( inStr[pos] ) );
	}

	//:::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
// ---------------------------------------------------------------------------
UnicodeString TStringTools::StripSpecialChars_UC( UnicodeString inStr )
{
    inStr = StringReplace(inStr,UnicodeString("\r"),"", TReplaceFlags() << rfReplaceAll << rfIgnoreCase);
	inStr = StringReplace(inStr,UnicodeString("\n"),"", TReplaceFlags() << rfReplaceAll << rfIgnoreCase);

    return inStr;
}
// ---------------------------------------------------------------------------
AnsiString TStringTools::StripSpecialChars_ANSI( AnsiString inStr )
{
    inStr = AnsiReplaceStr(inStr,"\r","");
    inStr = AnsiReplaceStr(inStr,"\n","");

    return inStr;
}
// ---------------------------------------------------------------------------
AnsiString TStringTools::UpperCaseWithNoSpace(AnsiString string)
{
    AnsiString retVal = "";
    bool active = true;
    const char* line = string.c_str();
    for(int i = 0; line[i] != '\0'; i++)
    {
        if(line[i] != ' ')
        {
            retVal += (char)toupper(line[i]);
        }
    }
    return retVal;
}

//Method to check that payment have requested Properties
//param - propertyString -> propertystring saved in DB
//param - allProperties -> all properties need to be checked,
//should be separeted by comman and ended by comma e.g to check 1 and 2 string should be "1,2,"
bool TStringTools::HasAllProperties(AnsiString propertyString,AnsiString allProperties)
{
  bool retVal = false;
  AnsiString searchString = "";
  const char* line = allProperties.c_str();
  for(int i = 0 ; i < line[i] != '\0' ; i++)
  {
     if(line[i] == ',')
     {
         searchString = "-" + searchString + "-";
         retVal = propertyString.AnsiPos(searchString) > 0;
         if(!retVal)
           break;
         searchString = "";
     }
     else
     {
        searchString += line[i];
     }
  }
  return retVal;
}

