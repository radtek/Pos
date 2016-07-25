//---------------------------------------------------------------------------

#ifndef UpdaterH
#define UpdaterH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>

#include "AdjustmentContact.h"
#include "tinyxml.h"
#include "Stream.h"
#include "IBSQL.hpp"
#include "TouchBtn.h"
#include "TouchControls.h"
#include "MM_DBCore.h"
#include "PointsTransaction.h"
#include <ExtCtrls.hpp>

//---------------------------------------------------------------------------
class TfrmUpdater : public TForm
{
__published:	// IDE-managed Components
    TLabel *Label1;
    TButton *btnBrowse;
    TLabel *lblBinFilePath;
    TButton *btnUpdateDB;
    TLabel *lblStatus;
    TOpenTextFileDialog *OpenBinFileDialog;
    TButton *btnSetDB;
    TLabel *lblDBPath;
    TOpenTextFileDialog *OpenDBDialog;
    TLabel *Label2;
    TEdit *txtServerName;
    void __fastcall btnBrowseClick(TObject *Sender);
    void __fastcall btnUpdateDBClick(TObject *Sender);
    void __fastcall btnSetDBClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
private:	// User declarations
    AnsiString binFilePath;
    UnicodeString dbLocation;
    UnicodeString serverName;
    Database::TDBControl DBControl;
    TiXmlDocument* xmlDocument;
    AnsiString encryptPassword;

    void initialize();
    bool isBinFileExists( AnsiString path );
    void loadStreamFromFile(
                    AnsiString filePath,
                    TMemoryStream* outStream);

    void decryptStream(
                TMemoryStream* inStream,
                AnsiString password);

    void loadXmlDocumentFromStream(
                        TMemoryStream* inStream,
                        TiXmlDocument* outXmlDocument);

    void updatePointsTransactionsFromXmlFile(
                            TiXmlDocument* inXmlDocument );

    int getNumberOfChildNodes( TiXmlElement* element );

    void loadAdjustmentContactFromXmlElement(
                                TiXmlElement* inElement,
                                TAdjustmentContact &contact );

    void loadAdjustmentTransactionFromXmlElement(
                                TiXmlElement* inElement,
                                TAdjustmentTransaction &transaction);

    AnsiString getDBLocation();

    AnsiString getServerName();

    void displayDBLocation();

    void setDBLocation();

    bool initializeDBConnection();

    void updateContactWithAdjustments(
                        Database::TDBTransaction &transaction,
                        TAdjustmentContact &contact );

    int getContactIdByMemberNumberSiteId(
                        Database::TDBTransaction &transaction,
                        AnsiString memberNumber,
                        int siteId );

    int getNextContactKey();

    int getNextPointsTransactionKey();

    void setContactDetails(
                Database::TDBTransaction &transaction,
                TAdjustmentContact &contact );

    void setAdjustmentForContact(
                Database::TDBTransaction &transaction,
                TAdjustmentTransaction &adjustmentTransaction );

    void insertPointsTransactionToDB(
                Database::TDBTransaction &transaction,
                TPointsTransaction pointsTransaction);

    UnicodeString getPointsTransactionInsertQuery();

    int getNextKeyFromGenerator(
                    AnsiString generatorName );

public:		// User declarations
    __fastcall TfrmUpdater(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmUpdater *frmUpdater;
//---------------------------------------------------------------------------
#endif
