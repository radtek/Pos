//---------------------------------------------------------------------------

#ifndef LoadBillScenarioFileH
#define LoadBillScenarioFileH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
#include <vector>
//---------------------------------------------------------------------------

enum TLoadConfigFileResult
{
   lmSuccessful, lmInvalidConfigFile, lmElementNotFound
};

class ExceptionLoadConfigFile : public Exception
{
public:
   TLoadConfigFileResult Code;

   ExceptionLoadMenu( AnsiString inMsg, TLoadConfigFileResult inCode ) : Exception( inMsg ) { Code = inCode; }
};

typedef std::vector<TBillCalcInfo> BILL_SCENARIOS_LIST;

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

class TiXmlDocument;

class TLoadBillScenarioFileForm : public TForm
{
__published:	// IDE-managed Components
    TPanel *pnLeft;
    TPanel *pnRight;
    TStatusBar *sbFileURI;
    TPanel *pnTop;
    TPanel *pnBottom;
    TPanel *pnCenter;
    TPanel *pnScenarioNames;
    TGroupBox *gbScenarios;
    TPanel *pnInputPriceHeader;
    TListView *lvScenarios;
    TPanel *pnScenarioContent;
    TGroupBox *gbScenarioContent;
    TLabel *lbSelectedScenarioName;
    TPanel *Panel1;
    TRichEdit *reSelectedScenarioContent;
    TSplitter *Splitter1;
    TButton *btnOK;
    TButton *Button2;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall lvScenariosDblClick(TObject *Sender);

private:	// User declarations
    TiXmlDocument *_xmlConfigDoc;

    TiXmlElement* _rootElement;
    TiXmlElement* _lastSeenElement;

    void readAllScenarios( BILL_SCENARIOS_LIST* const inBillScenarios );
    void readAllScenarioURIs( std::set<AnsiString>& inFileURIs );
    void showAllScenarios( BILL_SCENARIOS_LIST* inBillScenarios );

    void selectLastSeenScenario();
    AnsiString lastSeenScenarioURL();
    void saveLastSeenScenario( TBillScenarioInfo* inBillScenario );

    void selectScenarioWithURI( AnsiString inURI );
    void selectScenarioAtIndex( __int32 inIndex );
    void showScenarioContentAtItem( TListItem* inItem ;)

    //::::::::::::::::::::::::::::::::::::::::::
    // SAVE CONFIG
    //::::::::::::::::::::::::::::::::::::::::::
    TiXmlDocument* createConfigXMLDoc();
    void saveConfigFile();
    void addConfigElements();
    void addElement( TiXmlElement* inParentElem, AnsiString inName, TiXmlElement* &inElem );
    void setNodeAttr( TiXmlElement* inElem, AnsiString inAttrName, AnsiString inAttrValue );

    //::::::::::::::::::::::::::::::::::::::::::
    // LOAD CONFIG
    //::::::::::::::::::::::::::::::::::::::::::
    void loadConfigElements( TiXmlDocument *inXmlConfigDoc );
    TiXmlElement* loadRootElement( TiXmlDocument* inXMLMenuDoc );
    TiXmlElement* loadLastSeenElement( TiXmlElement* inRootElement );
    TiXmlElement* loadElement( AnsiString inElemName, TiXmlElement* inParentElement );

public:		// User declarations
    __fastcall TLoadBillScenarioFileForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TLoadBillScenarioFileForm *LoadBillScenarioFileForm;
//---------------------------------------------------------------------------
#endif
