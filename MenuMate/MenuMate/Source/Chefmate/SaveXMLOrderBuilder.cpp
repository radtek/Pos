//---------------------------------------------------------------------------


#pragma hdrstop

#include "SaveXMLOrderBuilder.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

TSaveXMLOrderBuilder::TSaveXMLOrderBuilder()
{
}
//......................................................

TSaveXMLOrderBuilder::~TSaveXMLOrderBuilder()
{
}
//......................................................

TSaveXMLOrder* TSaveXMLOrderBuilder::CreateSaveXMLOrderDoc()
{
	 return new TSaveXMLOrder();
}
//......................................................


