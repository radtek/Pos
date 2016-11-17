//---------------------------------------------------------------------------

#ifndef StringToolsH
#define StringToolsH

//---------------------------------------------------------------------------

#include <StrUtils.hpp>
#include <string>

//---------------------------------------------------------------------------

class TStringTools
{
   public:
		//Singleton Instance Fetch Method.
		static TStringTools* Instance();

		size_t EqualStrings(
				std::string inStr1,
				std::string inStr2 );

		std::string UpperCaseString( std::string inStr );

        UnicodeString StripSpecialChars_UC( UnicodeString inStr );

        AnsiString StripSpecialChars_ANSI( AnsiString inStr );

        AnsiString UpperCaseWithNoSpace(AnsiString string);

   private:
		//Singleton Object Instance
		static TStringTools* _stringTools;

		TStringTools();
		~TStringTools();
};

//---------------------------------------------------------------------------

#endif
