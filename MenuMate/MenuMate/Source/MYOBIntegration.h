//---------------------------------------------------------------------------

#ifndef MYOBIntegrationH
#define MYOBIntegrationH

#include <vcl.h>
#include <iostream.h>

#include "tinyxml.h"

#include "FolderManager.h"

//---------------------------------------------------------------------------

const AnsiString MYOB_INTEGRATION_FOLDER = "Menumate Services\\MYOB Integration\\Cache";

const AnsiString XI_ERROR_NO_ERROR_MYOB                               = "No Error";
const AnsiString XI_ERROR_MESSAGE_MINTEGRATION_FOLDER_NOT_FOUND_MYOB = "MYOB Integration Folder not found";
const AnsiString XI_ERROR_MESSAGE_MINTEGRATION_NOT_RUNNING_MYOB      = "MYOB Integration Service is not running";

//---------------------------------------------------------------------------

class TMYOBContact;
class TMYOBInvoiceItem;

class TMYOBInvoice : TObject
{
	public:
	  __fastcall TMYOBInvoice();
	  __fastcall ~TMYOBInvoice();

    __property AnsiString        InvoiceStatus           = { read = FInvoiceStatus,     write = writeInvoiceStatus    };
	__property AnsiString        InvoiceNumber           = { read = FInvoiceNumber,     write = writeInvoiceNumber    };
	__property AnsiString        InvoiceReference        = { read = FInvoiceReference,  write = writeInvoiceReference };
    __property AnsiString        JobCode                 = { read = FJobCode,           write = writeJobCode          };
	__property TDateTime         InvoiceDate             = { read = FInvoiceDate,       write = writeInvoiceDate      };
	__property TDateTime         InvoiceDueDate          = { read = FInvoiceDueDate,    write = writeInvoiceDueDate   };
//    __property AnsiString        CashGLCode              = { read = FCashGLCode,         write = writeCashGLCode       };
	__property TMYOBContact*     InvoiceContact          = { read = FInvoiceContact };
	__property TMYOBInvoiceItem* InvoiceItems[int index] = { read = readInvoiceItem };
	__property int               InvoiceItemCount        = { read = readInvoiceItemCount };


	void AddItem( TMYOBInvoiceItem* inInvoiceItem );
	void AddItem( AnsiString    inglCode,
				  AnsiString    inDescription,
				  double        inUnitAmount,
				  double        inTaxAmount,
                  AnsiString    inTaxStatus );

	void AddPayment( TMYOBInvoiceItem* inInvoiceItem );
	void AddPayment( AnsiString    inglCode,
				     AnsiString    inDescription,
				     double        inUnitAmount,
                     double        inTaxAmount,
                     AnsiString    inTaxStatus);


	protected:
        AnsiString       FInvoiceStatus;
		AnsiString       FInvoiceNumber;
		AnsiString       FInvoiceReference;
        AnsiString       FJobCode;
		TDateTime        FInvoiceDate;
//        AnsiString       FCashGLCode;
		TDateTime        FInvoiceDueDate;
		TMYOBContact    *FInvoiceContact;
		TObjectList     *FInvoiceItems;
        void writeInvoiceStatus(    AnsiString       inInvoiceStatus    );
		void writeInvoiceNumber(    AnsiString       inInvoiceNumber    );
        void writeJobCode      (    AnsiString       inJobCode          );
		void writeInvoiceReference( AnsiString       inInvoiceReference );
		void writeInvoiceDate(      TDateTime        inInvoiceDate      );
		void writeInvoiceDueDate(   TDateTime        inInvoiceDueDate   );
//        void writeCashGLCode    (   TDateTime        inCashGLCode       );

		TMYOBInvoiceItem* readInvoiceItem( int inIndex );
		int               readInvoiceItemCount();
};

//---------------------------------------------------------------------------

class TMYOBContact : TObject
{
	public:
	  __fastcall TMYOBContact();
	  __fastcall TMYOBContact( AnsiString inContactName );
	  __fastcall ~TMYOBContact();

	__property AnsiString ContactName = { read = FCName, write = writeCName };

	protected:
		AnsiString FCName;

		void writeCName( AnsiString inContactName );
};

//---------------------------------------------------------------------------

class TMYOBInvoiceItem : TObject
{
	public:
	  __fastcall TMYOBInvoiceItem();
	  __fastcall TMYOBInvoiceItem( AnsiString inDescription );
	  __fastcall ~TMYOBInvoiceItem();

	__property AnsiString glCode      = { read = FGLCode,       write = writeGLCode     };
	__property AnsiString Description = { read = FDescription, write = writeDescription };
	__property double     UnitAmount  = { read = FUAmount,     write = writeUAmount     };
	__property AnsiString TaxStatus   = { read = FTaxStatus,   write = writeTaxStatus   };
    __property double TaxAmount       = { read = FTaxAmount,   write = writeTaxAmount   };

	protected:
		AnsiString FGLCode;
		AnsiString FDescription;
		double     FUAmount;
		AnsiString FTaxStatus;
        double     FTaxAmount;

		void writeGLCode(       AnsiString inAccountCode);
		void writeDescription( AnsiString inDescription );
		void writeUAmount(     double     inUnitAmount  );
		void writeTaxStatus(   AnsiString inTaxStatus   );
        void writeTaxAmount(     double inTaxAmount     );
};

//---------------------------------------------------------------------------

class TMYOBIntegration
{
	public:
		static TMYOBIntegration& Instance()
		{
			static TMYOBIntegration singleton;
			return singleton;
		}

        TMYOBIntegration();
		~TMYOBIntegration();

		void SetAndValidate( AnsiString inHostName, AnsiString inFolderName, AnsiString inUserName, AnsiString inPassword );
		void Validate();

		bool MYOBIntegrationIsInstalled();
		bool MYOBIntegrationIsRunning();
		bool MYOBFolderPathExists();
		bool SendInvoice( TMYOBInvoice *inMYOBInvoice );

		__property bool     FailedInvoicesInQueue        = { read = readFailedInvoicesInQueue };
		__property unsigned FailedActiveInterval         = { read = FFailedActiveInterval     };
		__property unsigned FailedPauseInterval   		 = { read = FFailedPauseInterval      };
		__property unsigned FailedFolderNotFoundInterval = { read = FFailedFolderNotFoundInterval    };
        		// Username and Password to the remote host where MYOB Integration service is running
		// They are ignored if the service is running localy

		__property AnsiString MYOBMachineName  = { read = readMYOBMachineName, write = writeMYOBMachineName };
		__property AnsiString MYOBFolderPath   = { read = readMYOBFolderPath,  write = writeMYOBFolderPath  };
		__property AnsiString MYOBUserName     = { read = readMYOBUserName,    write = writeMYOBUserName    };
		__property AnsiString MYOBPassword     = { read = readMYOBPassword,    write = writeMYOBPassword    };

		__property AnsiString MYOBErrorMessage = { read = FErrorMessage };

		unsigned SendNextFailedMYOBInvoice();

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

		AnsiString readMYOBMachineName();
		AnsiString readMYOBUserName();
		AnsiString readMYOBPassword();
		AnsiString readMYOBFolderPath();

		void writeMYOBMachineName( AnsiString inName );
		void writeMYOBUserName(    AnsiString inUserName );
		void writeMYOBPassword(    AnsiString inPassword );
		void writeMYOBFolderPath(  AnsiString inPath );

		bool readFailedInvoicesInQueue();
		bool CheckFailedInvoicesInQueue();

		void AddInvoiceAttrs( TiXmlElement *inInvoiceNode, TMYOBInvoice *inMYOBInvoice );
		void AddContactAttrs( TiXmlElement *inContactNode, TMYOBContact *inContact );
		void AddItems(        TiXmlElement *inItemsNode,   TMYOBInvoice *inMYOBInvoice );
		void AddItemAttrs(    TiXmlElement *inItemNode,    TMYOBInvoiceItem *inInvoiceItem );
		void SetNodeAttr(     TiXmlElement *inNode, AnsiString inAttrName, AnsiString inAttrValue );
        void AddPayment(        TiXmlElement *inItemsNode,   TMYOBInvoice *inMYOBInvoice );
        void AddPaymentAttrs(    TiXmlElement *inItemNode,    TMYOBInvoiceItem *inInvoiceItem );

		void CheckAndCreateFolder( AnsiString inFolderName );

		bool SaveMYOBInvoiceInPoll( AnsiString inInvoiceFilename, TiXmlDocument inDoc );
		bool SaveMYOBInvoiceToFile( AnsiString inInvoiceFilename, TiXmlDocument inDoc );

        AnsiString GetCashGLCode();
        AnsiString MYOBInvoiceFilename( AnsiString inInvoiceNumber   );
		AnsiString MYOBInvoiceFilePath( AnsiString inInvoiceFilename );
        AnsiString MYOBInvoiceFileTempPath( AnsiString inInvoiceFilename );
};
#endif
//---------------------------------------------------------------------------

