//---------------------------------------------------------------------------

#pragma hdrstop

#include "ChefmateInterface.h"

#include "ChefmateSettings.h"

#include "MMMessageBox.h"
#include "MMContactInfo.h"

// This is used until the DLL is fixed
#include "TcpStreamSenderSettings.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TChefmateSettings
//::::::::::::::::::::::::::::::::::::::::::::::::::::::

TChefmateSettings::TChefmateSettings()
{
	FActive  = _DEFAULT_CM_ACTIVE;
	FIPAddr  = _DEFAULT_CM_IP_ADDR;
	FIPPort  = _DEFAULT_CM_IP_PORT;
	FDynamic = _DEFAULT_CM_DYNAMIC;
}
//---------------------------------------------------------------------------

TChefmateSettings::~TChefmateSettings()
{
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC FUNCTIONS
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void TChefmateSettings::SetDatabase( TIBDatabase *inIBDatabase )
{
	ibDatabase = inIBDatabase;
	Database::TDBTransaction dbTransaction( ibDatabase );

	loadSettingsFromDB( &dbTransaction, FActive, FIPAddr, FIPPort, FDynamic );
}
//---------------------------------------------------------------------------

void TChefmateSettings::Edit( TForm* inForm )
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

void TChefmateSettings::_setActive( bool inActive)
{
	FActive = inActive;

	Database::TDBTransaction dbTransaction( ibDatabase );
	dbTransaction.StartTransaction();

	saveActiveToDB( &dbTransaction, FActive );

	dbTransaction.Commit();
}
//---------------------------------------------------------------------------

void TChefmateSettings::_setIPAddr( UnicodeString inIPAddr  )
{
	FIPAddr = inIPAddr;

	Database::TDBTransaction dbTransaction( ibDatabase );
	dbTransaction.StartTransaction();

	saveIPAddrToDB( &dbTransaction, FIPAddr );

	dbTransaction.Commit();
}
//---------------------------------------------------------------------------

void TChefmateSettings::_setIPPort( unsigned int inIPPort  )
{
	FIPPort = inIPPort;

	Database::TDBTransaction dbTransaction( ibDatabase );
	dbTransaction.StartTransaction();

	saveIPPortToDB( &dbTransaction, FIPPort );

	dbTransaction.Commit();
}
//---------------------------------------------------------------------------

void TChefmateSettings::_setDynamic( bool inDynamic )
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

void TChefmateSettings::loadSettingsFromDB(
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

void TChefmateSettings::initSetting(
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

void TChefmateSettings::saveActiveToDB(
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

void TChefmateSettings::saveIPAddrToDB(
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

void TChefmateSettings::saveIPPortToDB(
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

void TChefmateSettings::saveDynamicToDB(
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

bool TChefmateSettings::variableHasBeenSet(
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

void TChefmateSettings::setVarStr(
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

void TChefmateSettings::updateVarValueStr(
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

void TChefmateSettings::updateVarStr(
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

UnicodeString TChefmateSettings::readVarValueStr(
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

void TChefmateSettings::readVarStr(
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

TChefmateInterface::TChefmateInterface()
{
	_cmGeneratorH = 0;
    _orderDocH = 0;
    _cmGeneratorOpen = false;
}
//---------------------------------------------------------------------------

TChefmateInterface::~TChefmateInterface()
{
   //delete _xmlOrderManager;
}
//---------------------------------------------------------------------------

void TChefmateInterface::SetDatabase( TIBDatabase *inIBDatabase )
{
	ibDatabase = inIBDatabase;

    Settings.SetDatabase(inIBDatabase);

	ChefmateServingCourseMap.SetDatabase(  inIBDatabase );
    ChefmateCourseMap.SetDatabase( inIBDatabase );
	ChefmatePrintingFormatMap.SetDatabase( inIBDatabase );
}
//---------------------------------------------------------------------------

bool TChefmateInterface::Available()
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

bool TChefmateInterface::Init()
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

void TChefmateInterface::Done()
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

void TChefmateInterface::Clear()
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

bool TChefmateInterface::OpenIncompleteOrder( __int32       inOrderDBKey,
											  UnicodeString inServerName,
											  __int32       inOrderNumber,
											  UnicodeString inChitValue,
											  UnicodeString inTableTabName,
											  UnicodeString inOrderType,
											  UnicodeString inCustomerName,
											  UnicodeString inPartyName )
{
	bool result = false;
	if( _cmGeneratorOpen )
	{
		try
		{
			if( _orderDocH > 0 )
			{
				Clear();
			}

			// This is used until the DLL is fixed
			_orderDocH = _xmlOrderManager->OpenIncompleteOrder(
					inOrderDBKey,
					inServerName.t_str(),
					inOrderNumber,
					inChitValue.t_str(),
					inTableTabName.t_str(),
					inOrderType.t_str(),
					inCustomerName.t_str(),
					inPartyName.t_str() );

			result = true;
		}
		catch( ... )
		{
		}
	}

	//::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

bool TChefmateInterface::OpenCompleteOrder( __int32       inOrderDBKey,
											UnicodeString inServerName,
											__int32       inOrderNumber,
											UnicodeString inChitValue,
											UnicodeString inTableTabName,
											UnicodeString inOrderType,
											UnicodeString inCustomerName,
											UnicodeString inPartyName,
											UnicodeString inPatronCount,
											UnicodeString inSaleStartTime,
                                            UnicodeString inDeliveryTime)
{
	bool result = false;

    if( _cmGeneratorOpen )
	{
		try
		{
			if( _orderDocH > 0 )
			{
				Clear();
			}

			// This is used until the DLL is fixed
			_orderDocH = _xmlOrderManager->OpenCompleteOrder(
					inOrderDBKey,
					inServerName.t_str(),
					inOrderNumber,
					inChitValue.t_str(),
					inTableTabName.t_str(),
					inOrderType.t_str(),
					inCustomerName.t_str(),
					inPartyName.t_str(),
					inPatronCount.t_str(),
					inSaleStartTime.t_str(),
                    inDeliveryTime.t_str(),
                    "",
                    "",
                    "",
                    "" );

			result = true;
		}
		catch( ... )
		{
		}
	}
	return result;
}
//---------------------------------------------------------------------------

bool TChefmateInterface::OpenTransferOrder( __int32       inOrderDBKey,
											__int32       inOrderNumber,
											UnicodeString inTableTabName,
                                            UnicodeString inServerName,
			                                UnicodeString inPatronCount,
			                                UnicodeString inSaleStartTime,
                                            UnicodeString inSourceTableName   )
{
	bool result = false;
	if( _cmGeneratorOpen )
	{
		try
		{
			if( _orderDocH > 0 )
			{
				Clear();
			}

            _orderDocH = _xmlOrderManager->OpenTransferOrder(
                    inOrderDBKey,
                    inOrderNumber,
                    inTableTabName.t_str(),
                    inServerName.t_str(),
                    inPatronCount.t_str(),
                    inSaleStartTime.t_str(),
                    inSourceTableName.t_str() );

			result = true;
		}
		catch( ... )
		{
		}
	}
	return result;
}
//---------------------------------------------------------------------------

HORDERDOC TChefmateInterface::OpenCreditOrder()
{
	try
	{
        if (!_cmGeneratorOpen)
            return false;
        else if (_orderDocH > 0)
            Clear();
        _orderDocH = _xmlOrderManager->OpenCreditOrder();
        return true;
	}
	catch (...)
	{
        return false;
    }
}
//---------------------------------------------------------------------------

HORDERDOC TChefmateInterface::OpenCancelOrder(
								__int32 inDBKey,
								__int32 inOrderNumber,
                               UnicodeString inTableTabName)
{
	bool result = false;

	//:::::::::::::::::::::::::::::::::::::::

	if( _cmGeneratorOpen )
	{
		try
		{
			if( _orderDocH > 0 )
			{
				Clear();
			}

			// This is used until the DLL is fixed
			_orderDocH = _xmlOrderManager->OpenCancelOrder(
							inDBKey,
							inOrderNumber,
                            inTableTabName);

			result = true;
		}
		catch( ... )
		{
		}
	}

	//::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

HORDERDOC TChefmateInterface::OpenReplacementOrder(
								__int32 	  inDBKey,
								__int32 	  inOrderNumber,
								UnicodeString inTableTabName )
{
	bool result = false;
	if( _cmGeneratorOpen )
	{
		try
		{
			if( _orderDocH > 0 )
			{
				Clear();
			}

			// This is used until the DLL is fixed
			_orderDocH = _xmlOrderManager->OpenReplacementOrder(
							inDBKey,
							inOrderNumber,
							inTableTabName );

			result = true;
		}
		catch( ... )
		{
		}
	}

	//::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

HORDERDOC TChefmateInterface::OpenCallAwayOrder(
								__int32 	   inOrderNumber,
								UnicodeString inTableTabName )
{
	bool result = false;
	if( _cmGeneratorOpen )
	{
		try
		{
			if( _orderDocH > 0 )
			{
				Clear();
			}

			// This is used until the DLL is fixed
			_orderDocH = _xmlOrderManager->OpenCallAwayOrder(
							inOrderNumber,
							inTableTabName );

			result = true;
		}
		catch( ... )
		{
		}
	}

	//::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

HITEM TChefmateInterface::AddItem(
							  TCMHostname   inCMHostname,
							  __int32       inItemSizeKey,
							  __int32       inOrderItemKey,
							  UnicodeString inItemName,
							  UnicodeString inItemKitchenName,
							  __int32       inCourseKey,
							  __int32       inServingCourseKey,
							  UnicodeString inNote )
{
	HITEM result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	if( _cmGeneratorOpen )
	{
		addServingCourseWithKey(inCMHostname, inServingCourseKey );

		addCourseWithKey(inCMHostname, inCourseKey );

	   	try
	   {
            result = _xmlOrderManager->AddItem(
						_orderDocH,
						inCMHostname,
						inItemSizeKey,
						inOrderItemKey,
						inItemName.t_str(),
						inItemKitchenName.t_str(),
						inServingCourseKey,
						inCourseKey,
						inNote );
	   	}
	   	catch( ... )
	   	{
	   	}

		addItemPrintingFormatWithKey(inCMHostname, inItemSizeKey, result);
	}
	return result;
}
//---------------------------------------------------------------------------

// If inCaceled is false then inQty is ignored
HITEM TChefmateInterface::AddItemToCancel(
							  TCMHostname   inCMHostname,
							  __int32       inOrderItemKey,
							  UnicodeString inItemName,
							  bool          inCanceled,
							  __int32       inQty )
{
	HITEM result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	if( _cmGeneratorOpen )
	{
		try
		{
			// This is used until the DLL is fixed
			result = _xmlOrderManager->AddItemToCancel(
						_orderDocH,
						inCMHostname,
						inOrderItemKey,
						inItemName,
						inCanceled,
						inQty );
		}
		catch( ... )
		{
		}
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

__int32 TChefmateInterface::AddCallAwayServingCourse(
								TCMHostname   inCMHostname,
								UnicodeString inServingCourseName )
{
	__int32 result = 0;
	if( _cmGeneratorOpen )
	{
		result = addServingCourseWithName(
					inCMHostname,
					inServingCourseName );
	}
	return result;
}
//---------------------------------------------------------------------------

HSIDE TChefmateInterface::AddItemSide(
								TCMHostname   inCMHostname,
								HITEM         inItemHandle,
								__int32       inOrderSideKey,
								UnicodeString inSideName,
								UnicodeString inSideKitchenName
							   )
{
	HSIDE result = 0;

if( _cmGeneratorOpen )
	{
		try
		{
			// This is used until the DLL is fixed
			result = _xmlOrderManager->AddItemSide(
						_orderDocH,
						inCMHostname,
						inItemHandle,
						inOrderSideKey,
						inSideName.t_str(),
						inSideKitchenName.t_str()
					);

			addItemSidePrintingFormatWithKey(
				inCMHostname,
				inOrderSideKey,
				result );
		}
		catch( ... )
		{
		}
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

HSIDE TChefmateInterface::AddItemSideToCancel(
								TCMHostname   inCMHostname,
								HITEM         inItemHandle,
								__int32       inOrderSideKey,
								UnicodeString inSideName )
{
	HSIDE result = 0;

	//::::::::::::::::::::::::::::::::::::::::::::::::

	if( _cmGeneratorOpen )
	{
		try
		{
			// This is used until the DLL is fixed
			result = _xmlOrderManager->AddItemSideToCancel(
						_orderDocH,
						inCMHostname,
						inItemHandle,
						inOrderSideKey,
						inSideName );
		}
		catch( ... )
		{
		}
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

void TChefmateInterface::AddItemOption(
								TCMHostname   inCMHostname,
								HITEM         inItemHandle,
								__int32       inOptionKey,
								UnicodeString inOptionName,
								UnicodeString inOptionKitchenName,
								bool          inIsPlusPlus
							)
{
	if( _cmGeneratorOpen )
	{
		try
		{
			HOPTION optionH = _xmlOrderManager->AddItemOption(
									_orderDocH,
									inCMHostname,
									inItemHandle,
									inOptionKey,
									inOptionName.t_str(),
									inOptionKitchenName.t_str(),
									inIsPlusPlus
								);

			addItemOptionPrintingFormatWithKey(
				inCMHostname,
				inOptionKey,
				optionH );
		}
		catch( ... )
		{
		}
	}
}
//---------------------------------------------------------------------------

void TChefmateInterface::AddSideOption(
								TCMHostname   inCMHostname,
								HSIDE         inSideHandle,
								__int32       inOptionKey,
								UnicodeString inOptionName,
								UnicodeString inOptionKitchenName,
								bool          inIsPlusPlus
							)
{
	if( _cmGeneratorOpen )
	{
		try
		{
			HOPTION optionH = _xmlOrderManager->AddSideOption(
									_orderDocH,
									inCMHostname,
									inSideHandle,
									inOptionKey,
									inOptionName.t_str(),
									inOptionKitchenName.t_str(),
									inIsPlusPlus
								);

			addSideOptionPrintingFormatWithKey(
				inCMHostname,
				inOptionKey,
				optionH );
		}
		catch( ... )
		{
		}
	}
}
//---------------------------------------------------------------------------

OERROR TChefmateInterface::CommitOrder()
{
	OERROR result = OERROR_FAILED_GENERATOR_NOT_ACTIVE;

	if( _cmGeneratorOpen )
	{
		try
		{
			result = _xmlOrderManager->CommitOrder( _orderDocH );
		}
		catch( ... )
		{
			result = OERROR_FAILED_GENERATOR_NOT_AVAILABLE;
		}
	}
	return result;
}
//---------------------------------------------------------------------------


OERROR TChefmateInterface::DeleteOrder()
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

bool TChefmateInterface::_chefmateActive()
{
	return Settings.Active;
}
//---------------------------------------------------------------------------

bool TChefmateInterface::_getOpen()
{
	return _cmGeneratorOpen;
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  PRIVATE FUNCTIONS
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

OERROR TChefmateInterface::openOrderGenerator()
{
	OERROR result = OERROR_SUCCESSFUL;
	_xmlOrderManager = createXMLOrderManager();
	return result;
}
//---------------------------------------------------------------------------

void TChefmateInterface::closeOrderGenerator()
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
TXMLOrderSmartManager* TChefmateInterface::createXMLOrderManager()
{
	return new TXMLOrderSmartManager();
}
//---------------------------------------------------------------------------

// NOT USED!!! This is for the DLL
void TChefmateInterface::loadFunctions()
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

void TChefmateInterface::addServingCourseWithKey(
								TCMHostname inCMHostname,
							    __int32     inServingCourseKey )
{
	try
	{
		TChefmateServingCourse scourse =
			ChefmateServingCourseMap.ServingCourseWithKey( inServingCourseKey );
		_xmlOrderManager->AddServingCourse(
			_orderDocH,
			inCMHostname,
			scourse.key,
			scourse.Name.t_str(),
			scourse.KitchenName.t_str(),
			scourse.DisplayOrder
		);
	}
	catch( ... )
	{
	}
}
//---------------------------------------------------------------------------

// NOT USED!!! .... addServingCourseWithKey is used instead
__int32 TChefmateInterface::addServingCourseWithCourseKey(
								TCMHostname inCMHostname,
								__int32     inCourseKey )
{
	__int32 result = 0;

	try
	{
		TChefmateServingCourse scourse =
			ChefmateServingCourseMap.ServingCourseWithCourseKey( inCourseKey );
		_xmlOrderManager->AddServingCourse(
			_orderDocH,
			inCMHostname,
			scourse.key,
			scourse.Name.t_str(),
			scourse.KitchenName.t_str(),
			scourse.DisplayOrder
		);

		result = scourse.key;
	}
	catch( ... )
	{
	}

	return result;
}
//---------------------------------------------------------------------------

__int32 TChefmateInterface::addServingCourseWithName(
								TCMHostname 	inCMHostname,
								UnicodeString   inServingCourseName )
{
	__int32 result = 0;

	try
	{
		TChefmateServingCourse scourse =
			ChefmateServingCourseMap.ServingCourseWithName( inServingCourseName );

		_xmlOrderManager->AddServingCourse(
			_orderDocH,
			inCMHostname,
			scourse.key,
			scourse.Name.t_str(),
			scourse.KitchenName.t_str(),
			scourse.DisplayOrder
		);

		result = scourse.key;
	}
	catch( ... )
	{
	}

	return result;
}
//---------------------------------------------------------------------------

void TChefmateInterface::addCourseWithKey(
								TCMHostname inCMHostname,
								__int32 inCourseKey )
{
	try
	{
		TChefmateCourse course =
			ChefmateCourseMap.CourseWithKey( inCourseKey );
		_xmlOrderManager->AddCourse(
			_orderDocH,
			inCMHostname,
			inCourseKey,
			course.Name.t_str(),
			course.KitchenName.t_str(),
			course.ServingCourseKey,
			course.DisplayOrder
		);
	}
	catch( ... )
	{
	}
}
//---------------------------------------------------------------------------

void  TChefmateInterface::addItemPrintingFormatWithKey(
								TCMHostname inCMHostname,
								__int32 inItemSizeKey,
								HITEM inItemH )
{
	try
	{
		TChefmatePrintingFormat itemPFormat =
			ChefmatePrintingFormatMap.ItemPFormatWithKey( inItemSizeKey );

		_xmlOrderManager->AddItemPrintingFormat(
			_orderDocH,
			inCMHostname,
			inItemH,
			0,
			itemPFormat.Underline,
			itemPFormat.Bold,
			( COLOR_INT )itemPFormat.Color,
			itemPFormat.LargeFont,
			itemPFormat.DoubleWidth,
			itemPFormat.DoubleHeight
		);
	}
	catch( ... )
	{
	}
}
//---------------------------------------------------------------------------

void  TChefmateInterface::addItemOptionPrintingFormatWithKey(
								TCMHostname inCMHostname,
								__int32     inItemOptionKey,
								HOPTION     inOptionH )
{
	try
	{
		TChefmatePrintingFormat itemOptionPFormat =
			ChefmatePrintingFormatMap.ItemOptionPFormatWithKey( inItemOptionKey );
		_xmlOrderManager->AddItemOptionPrintingFormat(
			_orderDocH,
			inCMHostname,
			inOptionH,
			0,
			itemOptionPFormat.Underline,
			itemOptionPFormat.Bold,
			( COLOR_INT )itemOptionPFormat.Color,
			itemOptionPFormat.LargeFont,
			itemOptionPFormat.DoubleWidth,
			itemOptionPFormat.DoubleHeight
		);
	}
	catch( ... )
	{
	}
}
//---------------------------------------------------------------------------

void  TChefmateInterface::addItemSidePrintingFormatWithKey(
								TCMHostname inCMHostname,
								__int32     inItemSideKey,
								HSIDE       inSideH )
{
	try
	{
		TChefmatePrintingFormat itemSidePFormat =
			ChefmatePrintingFormatMap.ItemSidePFormatWithKey( inItemSideKey );

		_xmlOrderManager->AddSidePrintingFormat(
			_orderDocH,
			inCMHostname,
			inSideH,
			0,
			itemSidePFormat.Underline,
			itemSidePFormat.Bold,
			( COLOR_INT )itemSidePFormat.Color,
			itemSidePFormat.LargeFont,
			itemSidePFormat.DoubleWidth,
			itemSidePFormat.DoubleHeight
		);
	}
	catch( ... )
	{
	}
}
//---------------------------------------------------------------------------

void  TChefmateInterface::addSideOptionPrintingFormatWithKey(
								TCMHostname inCMHostname,
								__int32     inSideOptionKey,
								HOPTION     inOptionH )
{
	try
	{
		TChefmatePrintingFormat itemOptionPFormat =
			ChefmatePrintingFormatMap.SideOptionPFormatWithKey( inSideOptionKey );


		_xmlOrderManager->AddSideOptionPrintingFormat(
			_orderDocH,
			inCMHostname,
			inOptionH,
			0,
			itemOptionPFormat.Underline,
			itemOptionPFormat.Bold,
			( COLOR_INT )itemOptionPFormat.Color,
			itemOptionPFormat.LargeFont,
			itemOptionPFormat.DoubleWidth,
			itemOptionPFormat.DoubleHeight
		);
	}
	catch( ... )
	{
	}
}
//---------------------------------------------------------------------------
 bool TChefmateInterface::OpenWebOrder( __int32       inOrderDBKey,
											UnicodeString inServerName,
											__int32       inOrderNumber,
											UnicodeString inChitValue,
											UnicodeString inTableTabName,
											UnicodeString inOrderType,
											TMMContactInfo inCustomerInfo,
											UnicodeString inPartyName,
											UnicodeString inPatronCount,
											UnicodeString inSaleStartTime,
                                            UnicodeString inDeliveryTime,
                                            UnicodeString inPaymentStatus )
{
	bool result = false;

    if( _cmGeneratorOpen )
	{
		try
		{
			if( _orderDocH > 0 )
			{
				Clear();
			}
			// This is used until the DLL is fixed
		   	_orderDocH = _xmlOrderManager->OpenCompleteOrder(
					inOrderDBKey,
					inServerName.t_str(),
					inOrderNumber,
					inChitValue.t_str(),
					inTableTabName.t_str(),
					inOrderType.t_str(),
					inCustomerInfo.Name.t_str(),//"rupendra",
					inPartyName.t_str(),
					inPatronCount.t_str(),
					inSaleStartTime.t_str(),
                    inDeliveryTime.t_str(),
                    inCustomerInfo.Phone.t_str(),//"05661694658",
                    inCustomerInfo.EMail.t_str(),//"rupendra.pandey@menumate.com",
                    inCustomerInfo.MailingAddress.t_str(),
                    inPaymentStatus.t_str());

			result = true;
		}
		catch( ... )
		{
		}
	}
	return result;
}

