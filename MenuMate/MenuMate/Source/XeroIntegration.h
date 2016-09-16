//---------------------------------------------------------------------------

#ifndef XeroIntegrationH
#define XeroIntegrationH

#include <vcl.h>
#include <iostream.h>

#include "tinyxml.h"

#include "FolderManager.h"

//---------------------------------------------------------------------------

const AnsiString XERO_INTEGRATION_FOLDER = "Menumate Services\\Xero Integration\\Cache";

const AnsiString XI_ERROR_NO_ERROR                              = "No Error";
const AnsiString XI_ERROR_MESSAGE_XINTEGRATION_FOLDER_NOT_FOUND = "Xero Integration Folder not found";
const AnsiString XI_ERROR_MESSAGE_XINTEGRATION_NOT_RUNNING      = "Xero Integration Service is not running";

//---------------------------------------------------------------------------

class TXeroContact;
class TXeroInvoiceItem;

enum TXeroInvoiceType { xitACCREC, xitACCPAY, xitACCRECCREDIT, xitACCPAYCREDIT };

class TXeroInvoice : TObject
{
	public:
	  __fastcall TXeroInvoice();
	  __fastcall TXeroInvoice( TXeroInvoiceType inInvoiceType );
	  __fastcall ~TXeroInvoice();

	__property TXeroInvoiceType  InvoiceType             = { read = FInvoiceType,       write = writeInvoiceType      };
    __property AnsiString        InvoiceStatus           = { read = FInvoiceStatus,     write = writeInvoiceStatus    };
	__property AnsiString        InvoiceNumber           = { read = FInvoiceNumber,     write = writeInvoiceNumber    };
	__property AnsiString        InvoiceReference        = { read = FInvoiceReference,  write = writeInvoiceReference };
	__property TDateTime         InvoiceDate             = { read = FInvoiceDate,       write = writeInvoiceDate      };
	__property TDateTime         InvoiceDueDate          = { read = FInvoiceDueDate,    write = writeInvoiceDueDate   };
	__property TXeroContact*     InvoiceContact          = { read = FInvoiceContact };
	__property TXeroInvoiceItem* InvoiceItems[int index] = { read = readInvoiceItem };
	__property int               InvoiceItemCount        = { read = readInvoiceItemCount };

    __property TXeroInvoiceItem* InvoicePayment[int index] = { read = readInvoicePayment };
   	__property int               InvoicePaymentCount        = { read = readInvoicePaymentCount };

	void AddItem( TXeroInvoiceItem* inInvoiceItem );
	void AddItem( AnsiString    inAccountCode,
				  AnsiString    inDescription,
				  double        inUnitAmount,
				  double        inTaxAmount,
				  double        inQuantity );

	void AddPayment( TXeroInvoiceItem* inInvoiceItem );
	void AddPayment( AnsiString    inAccountCode,
				  AnsiString    inDescription,
				  double        inUnitAmount,
				  double        inTaxAmount);


	protected:
		TXeroInvoiceType FInvoiceType;
        AnsiString       FInvoiceStatus;
		AnsiString       FInvoiceNumber;
		AnsiString       FInvoiceReference;
		TDateTime        FInvoiceDate;
		TDateTime        FInvoiceDueDate;
		TXeroContact    *FInvoiceContact;
		TObjectList     *FInvoiceItems;
        TObjectList     *FInvoicePayments;


		void writeInvoiceType(      TXeroInvoiceType inInvoiceType      );
        void writeInvoiceStatus(    AnsiString       inInvoiceStatus    );
		void writeInvoiceNumber(    AnsiString       inInvoiceNumber    );
		void writeInvoiceReference( AnsiString       inInvoiceReference );
		void writeInvoiceDate(      TDateTime        inInvoiceDate      );
		void writeInvoiceDueDate(   TDateTime        inInvoiceDueDate   );

		TXeroInvoiceItem* readInvoiceItem( int inIndex );
		int               readInvoiceItemCount();
        int         readInvoicePaymentCount();

        TXeroInvoiceItem* readInvoicePayment( int inIndex );


};

//---------------------------------------------------------------------------

class TXeroContact : TObject
{
	public:
	  __fastcall TXeroContact();
	  __fastcall TXeroContact( AnsiString inContactName );
	  __fastcall ~TXeroContact();

	__property AnsiString ContactName = { read = FCName, write = writeCName };

	protected:
		AnsiString FCName;

		void writeCName( AnsiString inContactName );
};

//---------------------------------------------------------------------------

class TXeroInvoiceItem : TObject
{
	public:
	  __fastcall TXeroInvoiceItem();
	  __fastcall TXeroInvoiceItem( AnsiString inDescription );
	  __fastcall ~TXeroInvoiceItem();

	__property AnsiString AccountCode = { read = FACode,       write = writeACode       };
	__property AnsiString Description = { read = FDescription, write = writeDescription };
	__property double     UnitAmount  = { read = FUAmount,     write = writeUAmount     };
	__property double     TaxAmount   = { read = FTaxAmount,   write = writeTAmount     };
	__property double     Quantity    = { read = FQuantity,    write = writeQty         };

	protected:
		AnsiString FACode;
		AnsiString FDescription;
		double     FUAmount;
		double     FTaxAmount;
		double     FQuantity;

		void writeACode(       AnsiString inAccountCode );
		void writeDescription( AnsiString inDescription );
		void writeUAmount(     double     inUnitAmount  );
		void writeTAmount(     double     inTaxAmount   );
		void writeQty(         double     inQuantity    );
};

//---------------------------------------------------------------------------

class TXeroIntegration
{
	public:
		static TXeroIntegration& Instance()
		{
			static TXeroIntegration singleton;
			return singleton;
		}

        TXeroIntegration();
		~TXeroIntegration();

		void SetAndValidate( AnsiString inHostName, AnsiString inFolderName, AnsiString inUserName, AnsiString inPassword );
		void Validate();

		bool XeroIntegrationIsInstalled();
		bool XeroIntegrationIsRunning();
		bool XeroFolderPathExists();
		bool SendInvoice( TXeroInvoice *inXeroInvoice );

		__property bool     FailedInvoicesInQueue        = { read = readFailedInvoicesInQueue };
		__property unsigned FailedActiveInterval         = { read = FFailedActiveInterval     };
		__property unsigned FailedPauseInterval   		 = { read = FFailedPauseInterval      };
		__property unsigned FailedFolderNotFoundInterval = { read = FFailedFolderNotFoundInterval    };
        		// Username and Password to the remote host where Xero Integration service is running
		// They are ignored if the service is running localy

		__property AnsiString XeroMachineName  = { read = readXeroMachineName, write = writeXeroMachineName };
		__property AnsiString XeroFolderPath   = { read = readXeroFolderPath,  write = writeXeroFolderPath  };
		__property AnsiString XeroUserName     = { read = readXeroUserName,    write = writeXeroUserName    };
		__property AnsiString XeroPassword     = { read = readXeroPassword,    write = writeXeroPassword    };

		__property AnsiString XeroErrorMessage = { read = FErrorMessage };

		unsigned SendNextFailedXeroInvoice();

	protected:
		TFolderManager *FFolderManager;

		AnsiString FFailedInvoicesFolderPath;
		unsigned   FFailedActiveInterval;
		unsigned   FFailedPauseInterval;
		unsigned   FFailedFolderNotFoundInterval;

		AnsiString FErrorMessage;

		TStringList *FailedInvoiceList;

		AnsiString PopFileNameFromList( TStringList* inList );

		AnsiString LocalHost();

		AnsiString readXeroMachineName();
		AnsiString readXeroUserName();
		AnsiString readXeroPassword();
		AnsiString readXeroFolderPath();

		void writeXeroMachineName( AnsiString inName );
		void writeXeroUserName(    AnsiString inUserName );
		void writeXeroPassword(    AnsiString inPassword );
		void writeXeroFolderPath(  AnsiString inPath );

		bool readFailedInvoicesInQueue();
		bool CheckFailedInvoicesInQueue();

		void AddInvoiceAttrs( TiXmlElement *inInvoiceNode, TXeroInvoice *inXeroInvoice );
		void AddContactAttrs( TiXmlElement *inContactNode, TXeroContact *inContact );
		void AddItems(        TiXmlElement *inItemsNode,   TXeroInvoice *inXeroInvoice );
		void AddItemAttrs(    TiXmlElement *inItemNode,    TXeroInvoiceItem *inInvoiceItem );
		void SetNodeAttr(     TiXmlElement *inNode, AnsiString inAttrName, AnsiString inAttrValue );
        void AddPayment(        TiXmlElement *inItemsNode,   TXeroInvoice *inXeroInvoice );
        void AddPaymentAttrs(    TiXmlElement *inItemNode,    TXeroInvoiceItem *inInvoiceItem );

		void CheckAndCreateFolder( AnsiString inFolderName );

		bool SaveXeroInvoiceInPoll( AnsiString inInvoiceFilename, TiXmlDocument inDoc );
		bool SaveXeroInvoiceToFile( AnsiString inInvoiceFilename, TiXmlDocument inDoc );

        AnsiString XeroInvoiceFilename( AnsiString inInvoiceNumber   );
		AnsiString XeroInvoiceFilePath( AnsiString inInvoiceFilename );
        AnsiString XeroInvoiceFileTempPath( AnsiString inInvoiceFilename );

		AnsiString InvoiceTypeToStr( TXeroInvoiceType inInvoiceType );
};
#endif
//---------------------------------------------------------------------------

