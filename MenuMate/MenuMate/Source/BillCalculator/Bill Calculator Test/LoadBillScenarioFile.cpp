//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "LoadBillScenarioFile.h"
#include "MMBillCalcResultDoc.h"
#include "tinyxml.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLoadBillScenarioFileForm *LoadBillScenarioFileForm;
//---------------------------------------------------------------------------

const AnsiString SCENARIOS_URL    = "..\results";
const AnsiString CONFIG_FILE_NAME = "BillCalcTest.config";

__fastcall TLoadBillScenarioFileForm::TLoadBillScenarioFileForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TLoadBillScenarioFileForm::FormCreate(TObject *Sender)
{
    _xmlConfigDoc = createConfigXMLDoc();
    loadConfigElements( _xmlConfigDoc );

    //::::::::::::::::::::::::::::::::::::::::::::::::

    readAllScenarios( billScenarios );
    showAllScenarios( billScenarios );
    selectLastSeenScenario();
}
//---------------------------------------------------------------------------
void __fastcall TLoadBillScenarioFileForm::lvScenariosDblClick(TObject *Sender)
{
    showScenarioContentAtItem( lvScenarios.Selected )
}
//---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::
// PRIVATE SECTION
//::::::::::::::::::::::::::::::::::::::::::::::

void TLoadBillScenarioFileForm::readAllScenarios( BILL_SCENARIOS_LIST* const inBillScenarios )
{
   inBillScenarios->clear();

   std::set<AnsiString>           fileURIs;
   std::set<AnsiString>::iterator fuIT;

   readAllScenarioURIs( fileURIs );

   using BillCalculator

   for( fuIT = fileURIs.begin(); fuIT != fileURIs.end(); fuIT++ )
   {
     AnsiString         scenarioURI = *fuIT;
     TBillCalcInfo billScenarioInfo = TMMBillCalcResultDoc::Instance()->BillCalcInfoWithURI( scenarioURI );

     inBillScenarios->push_back( billScenarioInfo );
   }
}
//---------------------------------------------------------------------------
void TLoadBillScenarioFileForm::readAllScenarioURIs( std::set<AnsiString>& inFileURIs )
{
    SCENARIOS_URL

    TSearchRec sr;
    int iAttributes = faArchive;
    AnsiString path = SCENARIOS_URL;

    if( FindFirst( path, iAttributes, sr ) == 0 )
    {
        do
        {
          inFileURIs.push_back( path + "\\" + sr.Name );
        }
        while( FindNext( sr ) == 0 );

        FindClose(sr);
    }
}
//---------------------------------------------------------------------------
void TLoadBillScenarioFileForm::showAllScenarios( BILL_SCENARIOS_LIST* inBillScenarios )
{
  BILL_SCENARIOS_LIST::iterator bsIT;

  for( bsIT = inBillScenarios.begin(); bsIT != inBillScenarios.end(); bsIT++ )
  {
    TListItem *item = lvTableList->Items->Add();

    item->Caption = bsIT->DateStr;
    item->SubItems->Add( bsIT->Name );

    item->Data = new AnsiString( bsIT->URI );
  }
}
//---------------------------------------------------------------------------
void TLoadBillScenarioFileForm::selectLastSeenScenario()
{
    AnsiString billScenarioURI = lastSeenScenarioURL();

    if( billScenarioURI = "" )
    {
      selectScenarioAtIndex( 0 );
    }
    else
    {
      selectScenarioWithURI( billScenarioURI );
    }
}
//---------------------------------------------------------------------------
AnsiString TLoadBillScenarioFileForm::lastSeenScenarioURL()
{
   return AnsiString( _lastSeenElement->Attribute( "uri" ) );
}
//---------------------------------------------------------------------------
void TLoadBillScenarioFileForm::saveLastSeenScenario( TBillScenarioInfo* inBillScenario )
{
   setNodeAttr( _lastSeenElement, "uri", inBillScenario->URI );
   saveConfigFile();
}
//---------------------------------------------------------------------------
void TLoadBillScenarioFileForm::selectScenarioWithURI( AnsiString inURI )
{
  BILL_SCENARIOS_LIST::iterator bsIT;

  __int32 i = ;

  for( bsIT = inBillScenarios.begin(); bsIT != inBillScenarios.end(); bsIT++ )
  {
    if( bsIT->URI.UpperCase().Trim() == inURI.UpperCase().Trim() )
    {
       selectScenarioAtIndex( i );
       return;
    }

    Application->MessageBox(
                    L"Failed to select scenario with URI: <" + inURI + "> at index: " + IntToStr( i ),
                    exc.Message,
                    MB_OK );
  }
}
//---------------------------------------------------------------------------
void TLoadBillScenarioFileForm::selectScenarioAtIndex( __int32 inIndex )
{
    try
    {
        TListItem *item = lvTableList->Items->Item[inIndex];

        item->Selected = true;
    }
    catch( Exception &exc )
    {
        Application->MessageBox(
                        L"Failed to select scenario at index: " + IntToStr( inIndex ),
                        exc.Message,
                        MB_OK );
    }
}
//---------------------------------------------------------------------------
void TLoadBillScenarioFileForm::showScenarioContentAtItem( TListItem* inItem )
{
    AnsiString fileURI = ( *( AnsiString* )inItem->Data );

    try
    {
        reSelectedScenarioContent.Lines->LoadFile( fileURI );
    }
    catch( Exception &exc )
    {
        Application->MessageBox(
                        L"Failed to open bill scenarion.",
                        exc.Message,
                        MB_OK );
    }
}
//---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::
// SAVE CONFIG
//::::::::::::::::::::::::::::::::::::::::::
TiXmlDocument* TLoadBillScenarioFileForm::createConfigXMLDoc()
{


    TiXmlDocument* result = new TiXmlDocument();

	//::::::::::::::::::::::::::::::

    if( FileExists( fileName ) )
    {
        result->LoadFile( fileName.c_str() );
    }
    else
    {
        // add declaration
        TiXmlDeclaration *decl = new TiXmlDeclaration(_T("1.0"), _T("UTF-8"), _T(""));
        result->LinkEndChild( decl );

        addConfigElements();
        saveConfigFile();
    }

    //::::::::::::::::::::::::::::::

    return result;
}
//---------------------------------------------------------------------------
void TLoadBillScenarioFileForm::saveConfigFile()
{
	try
	{
		_xmlConfigDoc->SaveFile( CONFIG_FILE_NAME.c_str() );
	}
	catch( Exception &exc )
	{
		result = false;
	}
}
//---------------------------------------------------------------------------
void TLoadBillScenarioFileForm::addConfigElements()
{
    addElement( ( TiXmlElement* )_xmlMenuDoc, "BillCalcTestConfig",  _rootElem );
    addElement( _rootElem,   "LastSeenScenario", _lastSeenElement );
}
//---------------------------------------------------------------------------
void TLoadBillScenarioFileForm::addElement( TiXmlElement* inParentElem, AnsiString inName, TiXmlElement* &inElem )
{
	inElem = new TiXmlElement(  inName.c_str() );
	inParentElem->LinkEndChild( inElem );
}
//----------------------------------------------------------------------------
void TLoadBillScenarioFileForm::setNodeAttr( TiXmlElement* inElem, AnsiString inAttrName, AnsiString inAttrValue )
{
    inElem->SetAttribute( inAttrName.c_str(), inAttrValue.c_str() );
}
//----------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::
// LOAD CONFIG
//::::::::::::::::::::::::::::::::::::::::::
void TLoadBillScenarioFileForm::loadConfigElements( TiXmlDocument *inXmlConfigDoc )
{
    try
    {
       _rootElem     = loadRootElement(  inXmlConfigDoc );
       _lastSeenElem = loadLastSeenElement( _rootElem );
    }
    catch( Exception &e )
    {
      throw ExceptionLoadConfigFile( e.Message, lmElementNotFound );
    }
}
//---------------------------------------------------------------------------
TiXmlElement* TLoadBillScenarioFileForm::loadRootElement( TiXmlDocument* inXMLMenuDoc )
{
    return inXMLMenuDoc->RootElement();
}
//---------------------------------------------------------------------------
TiXmlElement* TLoadBillScenarioFileForm::loadLastSeenElement( TiXmlElement* inRootElement )
{
    return loadElement( "LastSeenScenario", inRootElement );
}
//---------------------------------------------------------------------------
TiXmlElement* TLoadBillScenarioFileForm::loadElement( AnsiString inElemName, TiXmlElement* inParentElement )
{
    try
    {
       TiXmlElement *element = inParentElement->FirstChildElement();

       while( element != NULL )
       {
         if( AnsiString( element->Value() ) == inElemName )
         {
            return element;
         }
         element = element->NextSiblingElement();
       }
    }
    catch( ... )
    {
      throw Exception( "Invalid config file. Element not found: " + "root" );
    }

    throw Exception( "Invalid config file. Element not found: " + inElemName );
}
//---------------------------------------------------------------------------


