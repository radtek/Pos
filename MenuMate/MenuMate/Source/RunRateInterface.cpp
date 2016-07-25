//---------------------------------------------------------------------------

#pragma hdrstop

#include "RunRateInterface.h"

#include "ChefmateSettings.h"

#include "MMMessageBox.h"

// This is used until the DLL is fixed
#include "TcpStreamSenderSettings.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TChefmateSettings
//::::::::::::::::::::::::::::::::::::::::::::::::::::::

TRunRateSettings::TRunRateSettings()
{
	FActive  = _DEFAULT_CM_ACTIVE;
	FIPAddr  = _DEFAULT_CM_IP_ADDR;
	FIPPort  = _DEFAULT_CM_IP_PORT;
	FDynamic = _DEFAULT_CM_DYNAMIC;
}
//---------------------------------------------------------------------------

TRunRateSettings::~TRunRateSettings()
{
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC FUNCTIONS
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void TRunRateSettings::SetDatabase( TIBDatabase *inIBDatabase )
{
	ibDatabase = inIBDatabase;
	Database::TDBTransaction dbTransaction( ibDatabase );

	loadSettingsFromDB( &dbTransaction, FActive, FIPAddr, FIPPort, FDynamic );
}
//---------------------------------------------------------------------------

void TRunRateSettings::Edit( TForm* inForm )
{
    TfrmChefmateSettings *frmChefmateSettings = new TfrmChefmateSettings(inForm);
	frmChefmateSettings->Set( FActive, FIPAddr, FIPPort, FDynamic );
	if( frmChefmateSettings->ShowModal() == mrOk )
	{
		Active  = frmChefmateSettings->Active;
		IPAddr  = frmChefmateSettings->IPAddr;
		IPPort  = frmChefmateSettings->IPPort;
		Dynamic = frmChefmateSettings->Dynamic;
	}
    delete frmChefmateSettings;
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PROTECTED FUNCTIONS
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void TRunRateSettings::_setActive( bool inActive)
{
	FActive = inActive;

	Database::TDBTransaction dbTransaction( ibDatabase );
	dbTransaction.StartTransaction();

	saveActiveToDB( &dbTransaction, FActive );

	dbTransaction.Commit();
}
//---------------------------------------------------------------------------

void TRunRateSettings::_setIPAddr( UnicodeString inIPAddr  )
{
	FIPAddr = inIPAddr;

	Database::TDBTransaction dbTransaction( ibDatabase );
	dbTransaction.StartTransaction();

	saveIPAddrToDB( &dbTransaction, FIPAddr );

	dbTransaction.Commit();
}
//---------------------------------------------------------------------------

void TRunRateSettings::_setIPPort( unsigned int inIPPort  )
{
	FIPPort = inIPPort;

	Database::TDBTransaction dbTransaction( ibDatabase );
	dbTransaction.StartTransaction();

	saveIPPortToDB( &dbTransaction, FIPPort );

	dbTransaction.Commit();
}
//---------------------------------------------------------------------------

void TRunRateSettings::_setDynamic( bool inDynamic )
{
	FDynamic = inDynamic;

	Database::TDBTransaction dbTransaction( ibDatabase );
	dbTransaction.StartTransaction();

	saveDynamicToDB( &dbTransaction, FDynamic );

	dbTransaction.Commit();
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PRIVATE FUNCTIONS
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void TRunRateSettings::loadSettingsFromDB(
							Database::TDBTransaction* inDBTransaction,
							bool&          			  outActive,
							UnicodeString& 			  outIPAddr,
							unsigned int&       	  outIPPort,
							bool&          			  outDynamic )
{
	UnicodeString boolStr = _DEFAULT_CM_ACTIVE ? "TRUE" : "FALSE";
	initSetting( inDBTransaction,
				_ACTIVE_ID,
				"Chefmate Active",
				"Chefmate Active",
				boolStr );

	initSetting( inDBTransaction,
				_IP_ADDR_ID,
				"Chefmate IP address",
				"Chefmate IP address",
				_DEFAULT_CM_IP_ADDR );

	initSetting( inDBTransaction,
				 _IP_PORT_ID,
				 "Chefmate IP port",
				 "Chefmate IP port",
				 _DEFAULT_CM_IP_PORT );

	boolStr = _DEFAULT_CM_DYNAMIC ? "TRUE" : "FALSE";
	initSetting( inDBTransaction,
				 _DYNAMIC_ID,
				 "Chefmate Dynamic Orders",
				 "Chefmate Dynamic Orders",
				 boolStr );

	// This was for Single Chefmate
	//outActive  =           readVarValueStr( inDBTransaction, _ACTIVE_ID  ).UpperCase() == "TRUE";
	//outIPAddr  =           readVarValueStr( inDBTransaction, _IP_ADDR_ID );
	//outIPPort  = StrToInt( readVarValueStr( inDBTransaction, _IP_PORT_ID ) );
	//outDynamic =           readVarValueStr( inDBTransaction, _DYNAMIC_ID ).UpperCase() == "TRUE";

	// This is for Multi Chefmate
	outActive  = _DEFAULT_CM_ACTIVE;
	outIPAddr  = _DEFAULT_CM_IP_ADDR;
	outIPPort  = _DEFAULT_CM_IP_PORT;
	outDynamic = _DEFAULT_CM_DYNAMIC;
}
//---------------------------------------------------------------------------

void TRunRateSettings::initSetting(
							Database::TDBTransaction* inDBTransaction,
							__int32        			  inSettingID,
							UnicodeString          	  inName,
							UnicodeString          	  inDescription,
							UnicodeString          	  inDefaultValue )
{
	if( !variableHasBeenSet( inDBTransaction, inSettingID ) )
	{
		inDBTransaction->StartTransaction();

		setVarStr(
			inDBTransaction,
			inSettingID,
			inName,
			inDescription,
			1,
			inDefaultValue );

		inDBTransaction->Commit();
	}
}
//---------------------------------------------------------------------------

void TRunRateSettings::saveActiveToDB(
							Database::TDBTransaction* inDBTransaction,
							bool inActive )
{
	try
	{
		UnicodeString valueStr = inActive ? "true" : "false";
		updateVarValueStr( inDBTransaction, _ACTIVE_ID, valueStr );
	}
	catch( ... )
	{
	}
}
//---------------------------------------------------------------------------

void TRunRateSettings::saveIPAddrToDB(
							Database::TDBTransaction* inDBTransaction,
							UnicodeString inIPAddr )
{
	try
	{
		updateVarValueStr( inDBTransaction, _IP_ADDR_ID, inIPAddr );
	}
	catch( ... )
	{
	}
}
//---------------------------------------------------------------------------

void TRunRateSettings::saveIPPortToDB(
							Database::TDBTransaction* inDBTransaction,
							unsigned int              inIPPort )
{
	try
	{
		UnicodeString valueStr = StrToInt( inIPPort );
		updateVarValueStr( inDBTransaction, _IP_PORT_ID, valueStr );
	}
	catch( ... )
	{
	}
}
//---------------------------------------------------------------------------

void TRunRateSettings::saveDynamicToDB(
							Database::TDBTransaction* inDBTransaction,
							bool inDynamic )
{
	try
	{
		UnicodeString valueStr = inDynamic ? "true" : "false";
		updateVarValueStr( inDBTransaction, _DYNAMIC_ID, valueStr );
	}
	catch( ... )
	{
	}
}
//---------------------------------------------------------------------------

bool TRunRateSettings::variableHasBeenSet(
							Database::TDBTransaction* inDBTransaction,
							__int32                   inVariableID )
{
	bool result = false;

	//:::::::::::::::::::::::::::::::::::::

	inDBTransaction->StartTransaction();

	//.....................................

	TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
	qr->SQL->Text = "SELECT COUNT( Variables_Key ) as VarCount "
					"FROM VARIABLES "
					"WHERE Variables_Key = :key;";

	qr->ParamByName( "key" )->AsInteger = inVariableID;

	qr->ExecQuery();

	//.....................................

	result = qr->FieldByName("VarCount")->AsInteger > 0;

	inDBTransaction->Commit();

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

void TRunRateSettings::setVarStr(
							Database::TDBTransaction* inDBTransaction,
							__int32                   inVariableID,
							UnicodeString             inName,
							UnicodeString             inDiscription,
							__int32                   inGroup,
							UnicodeString             inValue)
{
	inDBTransaction->StartTransaction();

	//.....................................

	TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
	qr->SQL->Text = "INSERT INTO VARIABLES ( "
						"Variables_Key, "
						"Variable_Name, "
						"Variable_Description, "
						"Variable_Group, "
						"VarChar_Val ) "
					"VALUES ( "
						":key, "
						":name, "
						":description, "
						":group, "
						":varcharVal ); ";

	qr->ParamByName( "key"         )->AsInteger = inVariableID;
	qr->ParamByName( "name"        )->AsString  = inName;
	qr->ParamByName( "description" )->AsString  = inDiscription;
	qr->ParamByName( "group"       )->AsInteger = inGroup;
	qr->ParamByName( "varcharVal"  )->AsString  = inValue;

	qr->ExecQuery();

	//.....................................

	inDBTransaction->Commit();
}
//---------------------------------------------------------------------------

void TRunRateSettings::updateVarValueStr(
							Database::TDBTransaction* inDBTransaction,
							__int32                   inVariableID,
							UnicodeString             inValue )
{
	inDBTransaction->StartTransaction();

	//.....................................

	TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
	qr->SQL->Text = "UPDATE "
						"VARIABLES "
					"SET "
						"Varchar_Val = :varcharVal "
					"WHERE "
						"VARIABLES_KEY = :key";


	qr->ParamByName( "key"         )->AsInteger = inVariableID;
	qr->ParamByName( "varcharVal"  )->AsString  = inValue;

	qr->ExecQuery();

	//.....................................

	inDBTransaction->Commit();
}
//---------------------------------------------------------------------------

void TRunRateSettings::updateVarStr(
							Database::TDBTransaction* inDBTransaction,
							__int32                   inVariableID,
							UnicodeString             inName,
							UnicodeString             inDiscription,
							__int32                   inGroup,
							UnicodeString             inValue)
{
	inDBTransaction->StartTransaction();

	//.....................................

	TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
	qr->SQL->Text = "UPDATE "
						"VARIABLES "
					"SET "
						"Variable_Name        = :name, "
						"Variable_Description = :description, "
						"Variable_Group       = :group, "
						"Varchar_Val          = :varcharVal "
					"WHERE "
						"VARIABLES_KEY = :key";


	qr->ParamByName( "key"         )->AsInteger = inVariableID;
	qr->ParamByName( "name"        )->AsString  = inName;
	qr->ParamByName( "description" )->AsString  = inDiscription;
	qr->ParamByName( "group"       )->AsInteger = inGroup;
	qr->ParamByName( "varcharVal"  )->AsString  = inValue;

	qr->ExecQuery();

	//.....................................

	inDBTransaction->Commit();
}
//---------------------------------------------------------------------------

UnicodeString TRunRateSettings::readVarValueStr(
									Database::TDBTransaction* inDBTransaction,
									__int32                   inVariableID )
{
	UnicodeString result = "";

	//:::::::::::::::::::::::::::::::::::::

	inDBTransaction->StartTransaction();

	//.....................................

	TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
	qr->SQL->Text = "SELECT "
						"Varchar_Val as varcharVal "
					"FROM VARIABLES "
					"WHERE Variables_Key = :key;";

	qr->ParamByName( "key" )->AsInteger = inVariableID;

	qr->ExecQuery();

	if( qr->RecordCount > 0 )
	{
		result = qr->FieldByName( "varcharVal"  )->AsString;
	}

	//.....................................

	inDBTransaction->Commit();

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

void TRunRateSettings::readVarStr(
							Database::TDBTransaction* inDBTransaction,
							__int32                   inVariableID,
							UnicodeString&            outName,
							UnicodeString&            outDiscription,
							__int32&                  outGroup,
							UnicodeString&            outValue)
{
	inDBTransaction->StartTransaction();

	//.....................................

	TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
	qr->SQL->Text = "SELECT "
						"Variable_Name        as name, "
						"Variable_Description as description, "
						"Variable_Group       as group, "
						"Varchar_Val          as varcharVal "
					"FROM VARIABLES "
					"WHERE Variables_Key = :key;";

	qr->ParamByName( "key" )->AsInteger = inVariableID;

	qr->ExecQuery();

	if( qr->RecordCount > 0 )
	{
		outName        = qr->FieldByName( "name"        )->AsString;
		outDiscription = qr->FieldByName( "description" )->AsString;
		outGroup       = qr->FieldByName( "group"       )->AsInteger;
		outValue       = qr->FieldByName( "varcharVal"  )->AsString;
	}

	//.....................................

	inDBTransaction->Commit();
}
//---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TChefmateInterface
//::::::::::::::::::::::::::::::::::::::::::::::::::::::

TRunRateInterface::TRunRateInterface()
{
	//_cmGeneratorH = 0;
    //_orderDocH = 0;
    //_cmGeneratorOpen = false;
}
//---------------------------------------------------------------------------

TRunRateInterface::~TRunRateInterface()
{
   //delete _xmlOrderManager;
}
//---------------------------------------------------------------------------

void TRunRateInterface::SetDatabase( TIBDatabase *inIBDatabase )
{
	ibDatabase = inIBDatabase;

    Settings.SetDatabase(inIBDatabase);

	ChefmateServingCourseMap.SetDatabase(  inIBDatabase );
    ChefmateCourseMap.SetDatabase( inIBDatabase );
	ChefmatePrintingFormatMap.SetDatabase( inIBDatabase );
}
//---------------------------------------------------------------------------

bool TRunRateInterface::Available()
{
	bool result = false;
	try
	{
      result = _chefmateActive();
	}
	catch( ... )
	{
	}
	return result;
}
//---------------------------------------------------------------------------

bool TRunRateInterface::Init()
{
	bool result = false;
	if( _chefmateActive() )
	{
		try
		{
			if( !_cmGeneratorOpen )
			{
				_orderDocH = 0;

				OERROR error     = openOrderGenerator();
				_cmGeneratorOpen = ( error == OERROR_SUCCESSFUL );
			}

			if( _cmGeneratorOpen )
			{
				result = true;
			}
		}
		catch( ... )
		{
		}
	}
	return result;
}
//---------------------------------------------------------------------------

void TRunRateInterface::Done()
{
	if( _cmGeneratorOpen )
	{
		try
		{
			closeOrderGenerator();

			_cmGeneratorOpen = false;
		}
		catch( ... )
		{
		}
	}
}
//---------------------------------------------------------------------------

void TRunRateInterface::Clear()
{
	if( _cmGeneratorOpen && ( _orderDocH > 0 ) )
	{
		try
		{
			_xmlOrderManager->DeleteOrder( _orderDocH );
		}
		catch( ... )
		{
		}
	}
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

// If inCaceled is false then inQty is ignored

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

//OERROR TRunRateInterface::CommitOrder()
//{
//	OERROR result = OERROR_FAILED_GENERATOR_NOT_ACTIVE;
//
//    // This is used until the DLL is fixed
//
//	TSaveRunRateXMLOrder* xmlOrderDoc = new TSaveRunRateXMLOrder();
//
//
//	if( _cmGeneratorOpen )
//	{
//		try
//		{
//			result = _xmlOrderManager->CommitOrder( _orderDocH );
//		}
//		catch( ... )
//		{
//			result = OERROR_FAILED_GENERATOR_NOT_AVAILABLE;
//		}
//	}
//
//    result = xmlOrderDoc->CommitOrder( hostnameLIST );
//
//    return result;
//}
//---------------------------------------------------------------------------


OERROR TRunRateInterface::DeleteOrder()
{
	OERROR result = OERROR_FAILED_GENERATOR_NOT_ACTIVE;
	if( _cmGeneratorOpen )
	{
		try
		{
			_xmlOrderManager->DeleteOrder( _orderDocH );
		}
		catch( ... )
		{
			result = OERROR_FAILED_GENERATOR_NOT_AVAILABLE;
		}

		_orderDocH = 0;
	}
	return result;
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  PROTECTED FUNCTIONS
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

bool TRunRateInterface::_chefmateActive()
{
	return Settings.Active;
}
//---------------------------------------------------------------------------

bool TRunRateInterface::_getOpen()
{
	return _cmGeneratorOpen;
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  PRIVATE FUNCTIONS
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

OERROR TRunRateInterface::openOrderGenerator()
{
	OERROR result = OERROR_SUCCESSFUL;
	_xmlOrderManager = createXMLOrderManager();

	return result;
}
//---------------------------------------------------------------------------

void TRunRateInterface::closeOrderGenerator()
{
	if( _cmGeneratorOpen )
	{
		try
		{
			delete _xmlOrderManager;
		}
		catch( ... )
		{
		}
	}
}
//---------------------------------------------------------------------------


// This is used until the DLL is fixed
TXMLOrderSmartManager* TRunRateInterface::createXMLOrderManager()
{
	return new TXMLOrderSmartManager();
}
//---------------------------------------------------------------------------

// NOT USED!!! This is for the DLL
void TRunRateInterface::loadFunctions()
{
	_openOrderGenerator =
		( _OpenOrderGenerator )GetProcAddress( _cmGeneratorH, "OpenOrderGenerator" );

	_closeOrderGenerator =
		( _CloseOrderGenerator )GetProcAddress( _cmGeneratorH, "CloseOrderGenerator" );

	_configureChefMate =
		( _ConfigureChafMate )GetProcAddress( _cmGeneratorH, "ConfigureChafMate" );

	_openCompleteOrder =
		( _OpenCompleteOrder )GetProcAddress( _cmGeneratorH, "OpenCompleteOrder" );

	_openIncompleteOrder =
		( _OpenIncompleteOrder )GetProcAddress( _cmGeneratorH, "OpenIncompleteOrder" );

	_openCreditOrder =
		( _OpenCreditOrder )GetProcAddress( _cmGeneratorH, "OpenCreditOrder" );

	_openCancelOrder =
		( _OpenCancelOrder )GetProcAddress( _cmGeneratorH, "OpenCancelOrder" );

	_openTransferOrder =
		( _OpenTransferOrder )GetProcAddress( _cmGeneratorH, "OpenTransferOrder" );

	_addServingCourse =
		( _AddServingCourse )GetProcAddress( _cmGeneratorH, "AddServingCourse" );

	_addCourse =
		( _AddCourse )GetProcAddress( _cmGeneratorH, "AddCourse" );

	_addItem =
		( _AddItem )GetProcAddress( _cmGeneratorH, "AddItem" );

	_addItemPFormat =
		( _AddItemPrintingFormat )GetProcAddress( _cmGeneratorH, "AddItemPrintingFormat" );

	_addItemOption =
		( _AddItemOption )GetProcAddress( _cmGeneratorH, "AddItemOption" );

	_addItemOptionPFormat =
		( _AddItemOptionPrintingFormat )GetProcAddress( _cmGeneratorH, "AddItemOptionPrintingFormat" );

	_addItemSide =
		( _AddItemSide )GetProcAddress( _cmGeneratorH, "AddItemSide" );

	_addSidePFormat =
		( _AddSidePrintingFormat )GetProcAddress( _cmGeneratorH, "AddSidePrintingFormat" );

	_addSideOption =
		( _AddSideOption )GetProcAddress( _cmGeneratorH, "AddSideOption" );

	_addSideOptionPFormat =
		( _AddSideOptionPrintingFormat )GetProcAddress( _cmGeneratorH, "AddSideOptionPrintingFormat" );

	_commitOrder =
		( _CommitOrder )GetProcAddress( _cmGeneratorH, "CommitOrder" );

	_deleteOrder =
		( _DeleteOrder )GetProcAddress( _cmGeneratorH, "DeletetOrder" );

	_getLastOError =
		( _GetLastOError )GetProcAddress( _cmGeneratorH, "GetLastOError" );
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

// NOT USED!!! .... addServingCourseWithKey is used instead

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


