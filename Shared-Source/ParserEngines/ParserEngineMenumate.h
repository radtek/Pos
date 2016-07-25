//---------------------------------------------------------------------------

#ifndef ParserEngineMenumateH
#define ParserEngineMenumateH

//---------------------------------------------------------------------------

#include "ParserEngine.h"

//---------------------------------------------------------------------------

using Database::TDBControl;

class TParserEngineMenumate : public TParserEngine
{
   public:
		TParserEngineMenumate();
		virtual ~TParserEngineMenumate();

        int VersionCount();

   protected:
		//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		//  Members and Methods that have to be modified when adding new versions.
		//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		void populateVersionList( PARSER_FUNCTION_MAP& outVersions );
};

//---------------------------------------------------------------------------

#endif

