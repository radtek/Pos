//---------------------------------------------------------------------------


#pragma hdrstop

#include "XeroIntegration.h"
#include "Math.h"

#include "XeroServiceManager.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

//...........................
//  TXeroInvoice
//...........................
__fastcall TXeroInvoice::TXeroInvoice() : TObject()
{
	FInvoiceType      = xitACCREC;
	FInvoiceNumber    = "";
	FInvoiceReference = "";
	FInvoiceDate      = Now();
	FInvoiceDueDate   = Now();
	FInvoiceContact   = new TXeroContact();
	FInvoiceItems     = new TObjectList();
}
//---------------------------------------------------------------------------
__fastcall TXeroInvoice::TXeroInvoice( TXeroInvoiceType inInvoiceType ) : TObject()
{
	FInvoiceType      = inInvoiceType;
	FInvoiceNumber    = "";
	FInvoiceReference = "";
	FInvoiceDate      = Now();
	FInvoiceDueDate   = Now();
	FInvoiceContact   = new TXeroContact();
	FInvoiceItems     = new TObjectList();
}
//---------------------------------------------------------------------------
__fastcall TXeroInvoice::~TXeroInvoice()
{
	FInvoiceItems->Clear();

	delete FInvoiceContact;
	delete FInvoiceItems;
}
//---------------------------------------------------------------------------
void TXeroInvoice::AddItem( TXeroInvoiceItem* inInvoiceItem )
{
	FInvoiceItems->Add( ( TObject *)inInvoiceItem );
}
//---------------------------------------------------------------------------
void TXeroInvoice::AddItem( AnsiString inAccountCode,
							AnsiString inDescription,
							double     inUnitAmount,
							double     inTaxAmount,
							double     inQuantity )
{
	TXeroInvoiceItem *invoiceItem = new TXeroInvoiceItem();

	invoiceItem->AccountCode = inAccountCode;
	invoiceItem->Description = inDescription;
	invoiceItem->UnitAmount  = inUnitAmount;
	invoiceItem->TaxAmount   = inTaxAmount;
	invoiceItem->Quantity    = inQuantity;

	AddItem( invoiceItem ) ;
}
//---------------------------------------------------------------------------
void TXeroInvoice::writeInvoiceType( TXeroInvoiceType inInvoiceType )
{
   FInvoiceType = inInvoiceType;
}
//---------------------------------------------------------------------------
void TXeroInvoice::writeInvoiceNumber( AnsiString inInvoiceNumber )
{
	FInvoiceNumber = inInvoiceNumber;
}
//---------------------------------------------------------------------------
void TXeroInvoice::writeInvoiceReference( AnsiString inInvoiceReference )
{
	FInvoiceReference = inInvoiceReference;
}
//---------------------------------------------------------------------------
void TXeroInvoice::writeInvoiceDate( TDateTime inInvoiceDate )
{
	FInvoiceDate = inInvoiceDate;
}
//---------------------------------------------------------------------------
void TXeroInvoice::writeInvoiceDueDate( TDateTime inInvoiceDueDate )
{
	FInvoiceDueDate = inInvoiceDueDate;
}
//---------------------------------------------------------------------------
TXeroInvoiceItem* TXeroInvoice::readInvoiceItem( int inIndex )
{
	return ( TXeroInvoiceItem* )FInvoiceItems->Items[inIndex];
}
//---------------------------------------------------------------------------
int TXeroInvoice::readInvoiceItemCount()
{
	return FInvoiceItems->Count;
}
//---------------------------------------------------------------------------

//...........................
//  TXeroContact
//...........................
__fastcall TXeroContact::TXeroContact() : TObject()
{
	FCName = "";
}
//---------------------------------------------------------------------------
__fastcall TXeroContact::TXeroContact( AnsiString inContactName ) : TObject()
{
	writeCName( inContactName );
}
//---------------------------------------------------------------------------
__fastcall TXeroContact::~TXeroContact()
{
}
//---------------------------------------------------------------------------
void TXeroContact::writeCName( AnsiString inContactName )
{
	FCName = inContactName;
}
//---------------------------------------------------------------------------

//...........................
//  TXeroInvoiceItem
//...........................
__fastcall TXeroInvoiceItem::TXeroInvoiceItem() : TObject()
{
	FACode 		 = "";
	FDescription = "";
	FUAmount	 = 0.0;
	FTaxAmount   = 0.0;
	FQuantity	 = 0.0;
}
//---------------------------------------------------------------------------
__fastcall TXeroInvoiceItem::TXeroInvoiceItem( AnsiString inDescription ) : TObject()
{
	FACode 		 = "";
	FDescription = inDescription;
	FUAmount	 = 0.0;
	FTaxAmount   = 0.0;
	FQuantity	 = 0.0;
}
//---------------------------------------------------------------------------
__fastcall TXeroInvoiceItem::~TXeroInvoiceItem()
{
}
//---------------------------------------------------------------------------
void TXeroInvoiceItem::writeACode( AnsiString inAccountCode )
{
	FACode = inAccountCode;
}
//---------------------------------------------------------------------------
void TXeroInvoiceItem::writeDescription( AnsiString inDescription )
{
	FDescription = inDescription;
}
//---------------------------------------------------------------------------
void TXeroInvoiceItem::writeUAmount( double inUnitAmount )
{
	FUAmount = inUnitAmount;
}
//---------------------------------------------------------------------------
void TXeroInvoiceItem::writeTAmount( double inTaxAmount )
{
	FTaxAmount = inTaxAmount;
}
//---------------------------------------------------------------------------
void TXeroInvoiceItem::writeQty( double inQuantity )
{
	FQuantity = inQuantity;
}
//---------------------------------------------------------------------------

//...........................
//  TXeroIntegration
//...........................
TXeroIntegration::TXeroIntegration()
{
	FFolderManager = new TFolderManager();

	FErrorMessage = XI_ERROR_NO_ERROR;

	FFailedActiveInterval         = 1000;         // 1 sec
	FFailedPauseInterval          =    5 * 60000; // 5 min
	FFailedFolderNotFoundInterval =   30 *  1000; // 30 sec
	FFailedInvoicesFolderPath = ".\\Temp\\XeroInvoices";
	//FFailedInvoicesFolderPath = ExtractFilePath( Application->ExeName ) + "Temp\\XeroInvoices";

	SetAndValidate( LocalHost(), ExtractFilePath( Application->ExeName ), "", "" );
	CheckAndCreateFolder( FFailedInvoicesFolderPath );
	FailedInvoiceList = new TStringList();
}
//---------------------------------------------------------------------------
TXeroIntegration::~TXeroIntegration()
{
	delete FailedInvoiceList;
	delete FFolderManager;
}
//---------------------------------------------------------------------------
void TXeroIntegration::SetAndValidate( AnsiString inHostName, AnsiString inFolderName, AnsiString inUserName, AnsiString inPassword )
{
	FFolderManager->SetAndValidate( inHostName, inFolderName, inUserName, inPassword );
}
//---------------------------------------------------------------------------
void TXeroIntegration::Validate()
{
	//TXeroServiceManager::Instance().SetAndValidate( XeroMachineName, XeroUserName, XeroPassword );
	FFolderManager->Validate();
}
//---------------------------------------------------------------------------
bool TXeroIntegration::XeroIntegrationIsInstalled()
{
	// TO DO: Work on OpenSCManager remotely on WinXP.
	return true;
	//return TXeroServiceManager::Instance().IsServiceInstalled();
}
//----------------------------------------------------------------------------
bool TXeroIntegration::XeroIntegrationIsRunning()
{
	return TXeroServiceManager::Instance().IsServiceRunning();
}
//----------------------------------------------------------------------------
bool TXeroIntegration::XeroFolderPathExists()
{
	TFolderManager::Instance().SetAndValidate( XeroMachineName, XeroFolderPath, XeroUserName, XeroPassword );

	return TFolderManager::Instance().LastError == fmNO_ERROR;
}
//---------------------------------------------------------------------------
bool TXeroIntegration::SendInvoice( TXeroInvoice *inXeroInvoice )
{
	// create xml document
	TiXmlDocument doc;

	// add declaration
	TiXmlDeclaration * decl = new TiXmlDeclaration(_T("1.0"), _T("UTF-8"), _T(""));
	doc.LinkEndChild( decl );

	// add root node ( XeroInvoice )
	TiXmlElement *rootNode = new TiXmlElement("XeroInvoice");
	doc.LinkEndChild( rootNode );
	AddInvoiceAttrs( rootNode, inXeroInvoice );

	// add Contact node
	TiXmlElement *contactNode = new TiXmlElement("Contact");
	rootNode->LinkEndChild( contactNode );
	AddContactAttrs( contactNode, inXeroInvoice->InvoiceContact );

	// add Items node
	TiXmlElement *itemsNode = new TiXmlElement("Items");
    rootNode->LinkEndChild( itemsNode );
	AddItems( itemsNode, inXeroInvoice );

	//........................

	if( inXeroInvoice->InvoiceItemCount > 0 )
	{
		try
		{
			return SaveXeroInvoiceInPoll( XeroInvoiceFilename( inXeroInvoice->InvoiceNumber ), doc );
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
unsigned TXeroIntegration::SendNextFailedXeroInvoice()
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
			SaveXeroInvoiceInPoll( XeroInvoiceFilename( invoiceNumber ), doc );
		}
	}

	return FailedInvoiceList->Count;
}
//---------------------------------------------------------------------------
AnsiString TXeroIntegration::PopFileNameFromList( TStringList* inList )
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
AnsiString TXeroIntegration::LocalHost()
{
	return "localhost";
}
//---------------------------------------------------------------------------
AnsiString TXeroIntegration::readXeroMachineName()
{
	return FFolderManager->HostName;
}
//---------------------------------------------------------------------------
AnsiString TXeroIntegration::readXeroUserName()
{
	return FFolderManager->UserName;
}
//---------------------------------------------------------------------------
AnsiString TXeroIntegration::readXeroPassword()
{
	return FFolderManager->Password;
}
//---------------------------------------------------------------------------
AnsiString TXeroIntegration::readXeroFolderPath()
{
	return FFolderManager->FolderName;
}
//---------------------------------------------------------------------------
void TXeroIntegration::writeXeroMachineName( AnsiString inName )
{
	FFolderManager->HostName = inName;
}
//---------------------------------------------------------------------------
void TXeroIntegration::writeXeroUserName( AnsiString inUserName )
{
	FFolderManager->UserName = inUserName;
}
//---------------------------------------------------------------------------
void TXeroIntegration::writeXeroPassword( AnsiString inPassword )
{
	FFolderManager->Password = inPassword;
}
//---------------------------------------------------------------------------
void TXeroIntegration::writeXeroFolderPath( AnsiString inPath )
{
	FFolderManager->FolderName = inPath;
}
//---------------------------------------------------------------------------
bool TXeroIntegration::readFailedInvoicesInQueue()
{
   return CheckFailedInvoicesInQueue();
}
//...........................................................................
bool TXeroIntegration::CheckFailedInvoicesInQueue()
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
void TXeroIntegration::AddInvoiceAttrs( TiXmlElement *inInvoiceNode, TXeroInvoice *inXeroInvoice )
{
	SetNodeAttr( inInvoiceNode, "type",           InvoiceTypeToStr( inXeroInvoice->InvoiceType ).c_str() );
	SetNodeAttr( inInvoiceNode, "number",         inXeroInvoice->InvoiceNumber.c_str() );
	SetNodeAttr( inInvoiceNode, "reference",      inXeroInvoice->InvoiceReference.c_str() );
	SetNodeAttr( inInvoiceNode, "lineAmountType", "0" );   //Tax Exclusive
	SetNodeAttr( inInvoiceNode, "date",           inXeroInvoice->InvoiceDate.FormatString( "dd/mm/yyyy hh:mm" ) );
	SetNodeAttr( inInvoiceNode, "dueDate",        inXeroInvoice->InvoiceDueDate.FormatString( "dd/mm/yyyy hh:mm" ) );
}
//----------------------------------------------------------------------------
void TXeroIntegration::AddContactAttrs( TiXmlElement *inContactNode, TXeroContact *inContact )
{
	SetNodeAttr( inContactNode, "name", inContact->ContactName );
}
//----------------------------------------------------------------------------
void TXeroIntegration::AddItems( TiXmlElement *inItemsNode, TXeroInvoice *inXeroInvoice )
{
	// process line items
	for( int i = 0; i < inXeroInvoice->InvoiceItemCount; i++ )
	{
		TiXmlElement *itemNode = new TiXmlElement("Item");
		inItemsNode->LinkEndChild( itemNode );
		AddItemAttrs( itemNode, inXeroInvoice->InvoiceItems[i] );
	}
}
//----------------------------------------------------------------------------
void TXeroIntegration::AddItemAttrs( TiXmlElement *inItemNode, TXeroInvoiceItem *inInvoiceItem )
{
	AnsiString unitAmountStr = FormatFloat( "0.00", inInvoiceItem->UnitAmount );
	AnsiString qtyStr        = FormatFloat( "0.00", inInvoiceItem->Quantity   );
	AnsiString taxStr        = FormatFloat( "0.00", inInvoiceItem->TaxAmount  );

	SetNodeAttr( inItemNode, "accountCode", inInvoiceItem->AccountCode.c_str() );
	SetNodeAttr( inItemNode, "description", inInvoiceItem->Description.c_str() );
	SetNodeAttr( inItemNode, "unitAmount",  unitAmountStr.c_str() );
	SetNodeAttr( inItemNode, "taxAmount",   taxStr.c_str() );
	SetNodeAttr( inItemNode, "qty",         qtyStr.c_str() );
}
//----------------------------------------------------------------------------
void TXeroIntegration::SetNodeAttr( TiXmlElement *inNode, AnsiString inAttrName, AnsiString inAttrValue )
{
    inNode->SetAttribute( inAttrName.c_str(), inAttrValue.c_str() );
}
//----------------------------------------------------------------------------
void TXeroIntegration::CheckAndCreateFolder( AnsiString inFolderName )
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
bool TXeroIntegration::SaveXeroInvoiceInPoll( AnsiString inInvoiceFilename, TiXmlDocument inDoc )
{
	bool result = false;

	//.........................................

	FErrorMessage = XI_ERROR_NO_ERROR;

	if( XeroIntegrationIsInstalled() )
	{
		if( XeroFolderPathExists() )
		{
			if( !SaveXeroInvoiceToFile( XeroInvoiceFilePath( inInvoiceFilename ), inDoc ) )
			{
				result = SaveXeroInvoiceToFile( XeroInvoiceFileTempPath( inInvoiceFilename ), inDoc );
            }
			else
			{
				result = true;
            }
		}
		else
		{
			result = SaveXeroInvoiceToFile( XeroInvoiceFileTempPath( inInvoiceFilename ), inDoc );
			FErrorMessage = TFolderManager::Instance().LastErrorMsg;
		}
	}
	else
	{
		result = SaveXeroInvoiceToFile( XeroInvoiceFileTempPath( inInvoiceFilename ), inDoc );
		FErrorMessage = XI_ERROR_MESSAGE_XINTEGRATION_NOT_RUNNING;
	}

	//.........................................

	return result;
}
//---------------------------------------------------------------------------
bool TXeroIntegration::SaveXeroInvoiceToFile( AnsiString inInvoiceFilename, TiXmlDocument inDoc )
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
AnsiString TXeroIntegration::XeroInvoiceFilename( AnsiString inInvoiceNumber )
{
	if( inInvoiceNumber != "" )
	{
		return inInvoiceNumber + ".xml";
    }
    else
    {
        throw new Exception( "Cannot send an invoice without Invoice Number" );
    }
}
//----------------------------------------------------------------------------
AnsiString TXeroIntegration::XeroInvoiceFilePath( AnsiString inInvoiceFilename )
{
	return TFolderManager::Instance().FullPath + "\\" + inInvoiceFilename;
}
//----------------------------------------------------------------------------
AnsiString TXeroIntegration::XeroInvoiceFileTempPath( AnsiString inInvoiceFilename )
{
	return FFailedInvoicesFolderPath + "\\" + inInvoiceFilename;
}
//----------------------------------------------------------------------------
AnsiString TXeroIntegration::InvoiceTypeToStr( TXeroInvoiceType inInvoiceType )
{
	switch( inInvoiceType )
	{
		 case xitACCREC: return "ACCREC";
		 case xitACCPAY: return "ACCPAY";
		 default       : return "ACCREC";
	}
}
