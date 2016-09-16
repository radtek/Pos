//---------------------------------------------------------------------------


#pragma hdrstop

#include "MYOBIntegration.h"
#include "Math.h"
#include "XeroServiceManager.h"
#include "DeviceRealTerminal.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

__fastcall TMYOBInvoice::TMYOBInvoice() : TObject()
{
    FInvoiceStatus    = "";
	FInvoiceNumber    = "";
	FInvoiceReference = "";
	FInvoiceDate      = Now();
	FInvoiceDueDate   = Now();
	FInvoiceContact   = new TMYOBContact();
	FInvoiceItems     = new TObjectList();
}
//---------------------------------------------------------------------------
__fastcall TMYOBInvoice::~TMYOBInvoice()
{
	FInvoiceItems->Clear();

	delete FInvoiceContact;
	delete FInvoiceItems;
}
//---------------------------------------------------------------------------
void TMYOBInvoice::AddItem( TMYOBInvoiceItem* inInvoiceItem )
{
	FInvoiceItems->Add( ( TObject *)inInvoiceItem );
}
//---------------------------------------------------------------------------
void TMYOBInvoice::AddItem( AnsiString inGLCode,
							AnsiString inDescription,
							double     inUnitAmount,
							double     inTaxAmount,
                            AnsiString inTaxStatus)
{
	TMYOBInvoiceItem *invoiceItem = new TMYOBInvoiceItem();

	invoiceItem->glCode = inGLCode;
	invoiceItem->Description = inDescription;
	invoiceItem->UnitAmount  = inUnitAmount;
	invoiceItem->TaxAmount   = inTaxAmount;
    invoiceItem->TaxStatus     = inTaxStatus;
	AddItem( invoiceItem ) ;
}

//---------------------------------------------------------------------------
void TMYOBInvoice::writeInvoiceStatus( AnsiString inInvoiceStatus )
{
	FInvoiceStatus = inInvoiceStatus;
}
//---------------------------------------------------------------------------
void TMYOBInvoice::writeInvoiceNumber( AnsiString inInvoiceNumber )
{
	FInvoiceNumber = inInvoiceNumber;
}
//---------------------------------------------------------------------------
void TMYOBInvoice::writeInvoiceReference( AnsiString inInvoiceReference )
{
	FInvoiceReference = inInvoiceReference;
}
//---------------------------------------------------------------------------
void TMYOBInvoice::writeJobCode( AnsiString inJobCode )
{
	FJobCode = inJobCode;
}
//---------------------------------------------------------------------------
void TMYOBInvoice::writeInvoiceDate( TDateTime inInvoiceDate )
{
	FInvoiceDate = inInvoiceDate;
}
//---------------------------------------------------------------------------
void TMYOBInvoice::writeInvoiceDueDate( TDateTime inInvoiceDueDate )
{
	FInvoiceDueDate = inInvoiceDueDate;
}
//---------------------------------------------------------------------------
TMYOBInvoiceItem* TMYOBInvoice::readInvoiceItem( int inIndex )
{
	return ( TMYOBInvoiceItem* )FInvoiceItems->Items[inIndex];
}
//---------------------------------------------------------------------------
int TMYOBInvoice::readInvoiceItemCount()
{
	return FInvoiceItems->Count;
}
//---------------------------------------------------------------------------
//...........................
//  TMYOBContact
//...........................
__fastcall TMYOBContact::TMYOBContact() : TObject()
{
	FCName = "";
}
//---------------------------------------------------------------------------
__fastcall TMYOBContact::TMYOBContact( AnsiString inContactName ) : TObject()
{
	writeCName( inContactName );
}
//---------------------------------------------------------------------------
__fastcall TMYOBContact::~TMYOBContact()
{
}
//---------------------------------------------------------------------------
void TMYOBContact::writeCName( AnsiString inContactName )
{
	FCName = inContactName;
}
//---------------------------------------------------------------------------

//...........................
//  TMYOBInvoiceItem
//...........................
__fastcall TMYOBInvoiceItem::TMYOBInvoiceItem() : TObject()
{
	FGLCode 		 = "";
	FDescription = "";
	FUAmount	 = 0.0;
    FTaxAmount   = 0.0;
	FTaxStatus   = "";
}
//---------------------------------------------------------------------------
__fastcall TMYOBInvoiceItem::TMYOBInvoiceItem( AnsiString inDescription ) : TObject()
{
	FGLCode 		 = "";
	FDescription = inDescription;
	FUAmount	 = 0.0;
	FTaxStatus   = "";
    FTaxAmount = 0.0;
}
//---------------------------------------------------------------------------
__fastcall TMYOBInvoiceItem::~TMYOBInvoiceItem()
{
}
//---------------------------------------------------------------------------
void TMYOBInvoiceItem::writeGLCode( AnsiString inGLCode )
{
	FGLCode = inGLCode;
}
//---------------------------------------------------------------------------
void TMYOBInvoiceItem::writeDescription( AnsiString inDescription )
{
	FDescription = inDescription;
}
//---------------------------------------------------------------------------
void TMYOBInvoiceItem::writeUAmount( double inUnitAmount )
{
	FUAmount = inUnitAmount;
}
//---------------------------------------------------------------------------
void TMYOBInvoiceItem::writeTaxAmount( double inTaxStatus )
{
	FTaxAmount = inTaxStatus;
}
//---------------------------------------------------------------------------
void TMYOBInvoiceItem::writeTaxStatus( AnsiString inTaxStatus )
{
	FTaxStatus = inTaxStatus;
}

//...........................
//  TMYOBIntegration
//...........................
TMYOBIntegration::TMYOBIntegration()
{
	FFolderManager = new TFolderManager();

	FErrorMessage = XI_ERROR_NO_ERROR_MYOB;

	FFailedActiveInterval         = 1000;         // 1 sec
	FFailedPauseInterval          =    5 * 60000; // 5 min
	FFailedFolderNotFoundInterval =   30 *  1000; // 30 sec
	FFailedInvoicesFolderPath = ".\\Temp\\MYOBInvoices";

	SetAndValidate( LocalHost(), ExtractFilePath( Application->ExeName ), "", "" );
	CheckAndCreateFolder( FFailedInvoicesFolderPath );
	FailedInvoiceList = new TStringList();
}
//---------------------------------------------------------------------------
TMYOBIntegration::~TMYOBIntegration()
{
	delete FailedInvoiceList;
	delete FFolderManager;
}
//---------------------------------------------------------------------------
void TMYOBIntegration::SetAndValidate( AnsiString inHostName, AnsiString inFolderName, AnsiString inUserName, AnsiString inPassword )
{
	FFolderManager->SetAndValidate( inHostName, inFolderName, inUserName, inPassword );
}
//---------------------------------------------------------------------------
void TMYOBIntegration::Validate()
{
	//TMYOBServiceManager::Instance().SetAndValidate( MYOBMachineName, MYOBUserName, MYOBPassword );
	FFolderManager->Validate();
}
//---------------------------------------------------------------------------
bool TMYOBIntegration::MYOBIntegrationIsInstalled()
{
	// TO DO: Work on OpenSCManager remotely on WinXP.
	return true;
	//return TMYOBServiceManager::Instance().IsServiceInstalled();
}
//----------------------------------------------------------------------------
bool TMYOBIntegration::MYOBIntegrationIsRunning()
{
	return TXeroServiceManager::Instance().IsServiceRunning();
}
//----------------------------------------------------------------------------
bool TMYOBIntegration::MYOBFolderPathExists()
{

	TFolderManager::Instance().SetAndValidate( MYOBMachineName, MYOBFolderPath, MYOBUserName, MYOBPassword );

	return TFolderManager::Instance().LastError == fmNO_ERROR;
}
//---------------------------------------------------------------------------
bool TMYOBIntegration::SendInvoice( TMYOBInvoice *inMYOBInvoice )
{
	// create xml document
	TiXmlDocument doc;

	// add declaration
	TiXmlDeclaration * decl = new TiXmlDeclaration(_T("1.0"), _T("UTF-8"), _T(""));
	doc.LinkEndChild( decl );

	// add root node ( MYOBInvoice )
	TiXmlElement *rootNode = new TiXmlElement("MYOBInvoice");
	doc.LinkEndChild( rootNode );
	AddInvoiceAttrs( rootNode, inMYOBInvoice );


	// add Contact node
	TiXmlElement *contactNode = new TiXmlElement("Contact");
	rootNode->LinkEndChild( contactNode );
	AddContactAttrs( contactNode, inMYOBInvoice->InvoiceContact );

	// add Items node
	TiXmlElement *itemsNode = new TiXmlElement("Items");
    rootNode->LinkEndChild( itemsNode );
	AddItems( itemsNode, inMYOBInvoice );

	//........................
	if( inMYOBInvoice->InvoiceItemCount > 0 )
	{
		try
		{

			return SaveMYOBInvoiceInPoll( MYOBInvoiceFilename( inMYOBInvoice->InvoiceNumber ), doc );
		}
		catch( Exception &exc )
		{
			FErrorMessage = exc.Message;
			return false;
		}
	}
	else
	{
    	FErrorMessage = "No payments to include in the invoice";
		return false;
    }
}
//---------------------------------------------------------------------------
unsigned TMYOBIntegration::SendNextFailedMYOBInvoice()
{
	if( FailedInvoiceList->Count > 0 )
	{
		AnsiString fileName      = PopFileNameFromList( FailedInvoiceList );
		AnsiString invoiceNumber = ExtractFileName( fileName );
		           invoiceNumber = invoiceNumber.SubString( 1, invoiceNumber.AnsiPos( "." ) - 1 );

		// create xml document
		TiXmlDocument doc;

		if( doc.LoadFile( fileName.c_str(), TIXML_ENCODING_UTF8 ) )
		{
			DeleteFile( fileName );
			SaveMYOBInvoiceInPoll( MYOBInvoiceFilename( invoiceNumber ), doc );
		}
	}

	return FailedInvoiceList->Count;
}
//---------------------------------------------------------------------------
AnsiString TMYOBIntegration::PopFileNameFromList( TStringList* inList )
{
	try
	{
		AnsiString result = inList->Strings[0];
		inList->Delete( 0 );

		return result;
	}
	catch( ... )
	{
		 throw;
    }
}
//---------------------------------------------------------------------------
AnsiString TMYOBIntegration::LocalHost()
{
	return "localhost";
}
//---------------------------------------------------------------------------
AnsiString TMYOBIntegration::readMYOBMachineName()
{
	return FFolderManager->HostName;
}
//---------------------------------------------------------------------------
AnsiString TMYOBIntegration::readMYOBUserName()
{
	return FFolderManager->UserName;
}
//---------------------------------------------------------------------------
AnsiString TMYOBIntegration::readMYOBPassword()
{
	return FFolderManager->Password;
}
//---------------------------------------------------------------------------
AnsiString TMYOBIntegration::readMYOBFolderPath()
{
	return FFolderManager->FolderName;
}
//---------------------------------------------------------------------------
void TMYOBIntegration::writeMYOBMachineName( AnsiString inName )
{
	FFolderManager->HostName = inName;
}
//---------------------------------------------------------------------------
void TMYOBIntegration::writeMYOBUserName( AnsiString inUserName )
{
	FFolderManager->UserName = inUserName;
}
//---------------------------------------------------------------------------
void TMYOBIntegration::writeMYOBPassword( AnsiString inPassword )
{
	FFolderManager->Password = inPassword;
}
//---------------------------------------------------------------------------
void TMYOBIntegration::writeMYOBFolderPath( AnsiString inPath )
{
	FFolderManager->FolderName = inPath;
}
//---------------------------------------------------------------------------
bool TMYOBIntegration::readFailedInvoicesInQueue()
{
   return CheckFailedInvoicesInQueue();
}
//...........................................................................
bool TMYOBIntegration::CheckFailedInvoicesInQueue()
{
	if( FailedInvoiceList->Count == 0 )
	{
		TSearchRec sr;
		int iAttributes = faAnyFile;

		if( FindFirst( FFailedInvoicesFolderPath + "\\*.xml", iAttributes, sr ) == 0 )
		{
			if( sr.Attr & iAttributes )
			{
				FailedInvoiceList->Add( FFailedInvoicesFolderPath + "\\" + sr.Name );
			}
			while( FindNext( sr ) == 0 )
			{
			   if( sr.Attr & iAttributes )
			   {
			   		FailedInvoiceList->Add( FFailedInvoicesFolderPath + "\\" + sr.Name );
			   }
            }
		}
		FindClose(sr);
	}

	return FailedInvoiceList->Count > 0;
}
//...........................................................................
void TMYOBIntegration::AddInvoiceAttrs( TiXmlElement *inInvoiceNode, TMYOBInvoice *inMYOBInvoice )
{
    SetNodeAttr( inInvoiceNode, "Status",         inMYOBInvoice->InvoiceStatus.c_str() );
	SetNodeAttr( inInvoiceNode, "Number",         inMYOBInvoice->InvoiceNumber.c_str() );
	SetNodeAttr( inInvoiceNode, "Date",           inMYOBInvoice->InvoiceDate.FormatString( "dd/mm/yyyy hh:mm:ss"));
    SetNodeAttr( inInvoiceNode, "Jobcode",        inMYOBInvoice->JobCode);
    SetNodeAttr( inInvoiceNode, "CashGLCode",     GetCashGLCode());
}
//----------------------------------------------------------------------------
AnsiString TMYOBIntegration::GetCashGLCode()
{
    AnsiString CashGlcode;
	Database::TDBTransaction DBTransaction(
	TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();

    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->SQL->Text = "SELECT a.GL_CODE FROM PAYMENTTYPES a where a.PAYMENT_NAME = :PAYMENT_NAME ";
    IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = "Cash";

    IBInternalQuery->ExecQuery();

   CashGlcode = IBInternalQuery->FieldByName("GL_CODE")->AsString;
   return CashGlcode;
}
//----------------------------------------------------------------------------
void TMYOBIntegration::AddContactAttrs( TiXmlElement *inContactNode, TMYOBContact *inContact )
{
	SetNodeAttr( inContactNode, "Name", inContact->ContactName );
}
//----------------------------------------------------------------------------
void TMYOBIntegration::AddItems( TiXmlElement *inItemsNode, TMYOBInvoice *inMYOBInvoice )
{
	// process line items
	for( int i = 0; i < inMYOBInvoice->InvoiceItemCount; i++ )
	{
		TiXmlElement *itemNode = new TiXmlElement("Item");
		inItemsNode->LinkEndChild( itemNode );
		AddItemAttrs( itemNode, inMYOBInvoice->InvoiceItems[i] );
	}
}
//----------------------------------------------------------------------------
void TMYOBIntegration::AddItemAttrs( TiXmlElement *inItemNode, TMYOBInvoiceItem *inInvoiceItem )
{
	AnsiString unitAmountStr = FormatFloat( "0.00", inInvoiceItem->UnitAmount );
    AnsiString taxAmountStr  = FormatFloat( "0.00", inInvoiceItem->TaxAmount );
	AnsiString taxStr        = inInvoiceItem->TaxStatus;

	SetNodeAttr( inItemNode, "GLCode", inInvoiceItem->glCode.c_str() );
	SetNodeAttr( inItemNode, "Description", inInvoiceItem->Description.c_str() );
	SetNodeAttr( inItemNode, "UnitAmount",  unitAmountStr.c_str() );
	SetNodeAttr( inItemNode, "TaxName",   taxStr.c_str() );
    SetNodeAttr( inItemNode, "TaxAmount",   inInvoiceItem->TaxAmount );
}
//----------------------------------------------------------------------------
void TMYOBIntegration::SetNodeAttr( TiXmlElement *inNode, AnsiString inAttrName, AnsiString inAttrValue )
{
    inNode->SetAttribute( inAttrName.c_str(), inAttrValue.c_str() );
}
//----------------------------------------------------------------------------
void TMYOBIntegration::CheckAndCreateFolder( AnsiString inFolderName )
{
	if(!DirectoryExists( ".\\Temp" ))
	{
		CreateDir( ".\\Temp" );
	}

	if(!DirectoryExists( inFolderName ))
	{
		CreateDir( inFolderName );
	}
}
//----------------------------------------------------------------------------
bool TMYOBIntegration::SaveMYOBInvoiceInPoll( AnsiString inInvoiceFilename, TiXmlDocument inDoc )
{
	bool result = false;

	//.........................................

	FErrorMessage = XI_ERROR_NO_ERROR_MYOB;

	if( MYOBIntegrationIsInstalled())
	{
		if( MYOBFolderPathExists() )
		{
			if( !SaveMYOBInvoiceToFile( MYOBInvoiceFilePath( inInvoiceFilename ), inDoc ) )
			{
				result = SaveMYOBInvoiceToFile( MYOBInvoiceFileTempPath( inInvoiceFilename ), inDoc );
            }
			else
			{
				result = true;
            }
		}
		else
		{
			result = SaveMYOBInvoiceToFile( MYOBInvoiceFileTempPath( inInvoiceFilename ), inDoc );
			FErrorMessage = TFolderManager::Instance().LastErrorMsg;
		}
	}
	else
	{
		result = SaveMYOBInvoiceToFile( MYOBInvoiceFileTempPath( inInvoiceFilename ), inDoc );
		FErrorMessage = XI_ERROR_MESSAGE_MINTEGRATION_NOT_RUNNING_MYOB;
	}

	//.........................................

	return result;
}
//---------------------------------------------------------------------------
bool TMYOBIntegration::SaveMYOBInvoiceToFile( AnsiString inInvoiceFilename, TiXmlDocument inDoc )
{
	bool result = true;

	//.........................................

	try
	{
		result = inDoc.SaveFile( inInvoiceFilename.c_str() );
	}
	catch( Exception &exc )
	{
		FErrorMessage = exc.Message;
		result = false;
	}

	//.........................................

	return result;
}
//----------------------------------------------------------------------------
AnsiString TMYOBIntegration::MYOBInvoiceFilename( AnsiString inInvoiceNumber )
{
	if( inInvoiceNumber != "" )
	{
        AnsiString invoiceName = TDeviceRealTerminal::Instance().ID.Name+"_"+
                                 "ZED"+"_"+inInvoiceNumber;
		return invoiceName + ".xml";
    }
    else
    {
        throw new Exception( "Cannot send an invoice without Invoice Number" );
    }
}
//----------------------------------------------------------------------------
AnsiString TMYOBIntegration::MYOBInvoiceFilePath( AnsiString inInvoiceFilename )
{
	return TFolderManager::Instance().FullPath + "\\" + inInvoiceFilename;
}
//----------------------------------------------------------------------------
AnsiString TMYOBIntegration::MYOBInvoiceFileTempPath( AnsiString inInvoiceFilename )
{
	return FFailedInvoicesFolderPath + "\\" + inInvoiceFilename;
}


