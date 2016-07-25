//---------------------------------------------------------------------------

#ifndef SaveXMLOrderBuilderH
#define SaveXMLOrderBuilderH

//---------------------------------------------------------------------------

#include "SaveXMLOrder.h"

class TSaveXMLOrderBuilder
{
	public:
/*
		Removed Not Thread Save.
		static TSaveXMLOrderBuilder& Instance()
		{
			static TSaveXMLOrderBuilder singleton;
			return singleton;
		}*/

		TSaveXMLOrderBuilder();
		~TSaveXMLOrderBuilder();

		static TSaveXMLOrder* CreateSaveXMLOrderDoc();
};

//---------------------------------------------------------------------------

#endif
