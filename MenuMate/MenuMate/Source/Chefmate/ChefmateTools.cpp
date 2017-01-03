//---------------------------------------------------------------------------


#pragma hdrstop

#include <cassert>
#include "ChefmateTools.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

//............................................................................

//::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TChefmateServingCourseMap
//::::::::::::::::::::::::::::::::::::::::::::::::::::::

TChefmateServingCourseMap::TChefmateServingCourseMap()
{
    TDeviceRealTerminal::Instance().Menus->OnNewMenus.RegisterForEvent(OnPOSMenuChanged);
}
//---------------------------------------------------------------------------

TChefmateServingCourseMap::~TChefmateServingCourseMap()
{
    TDeviceRealTerminal::Instance().Menus->OnNewMenus.DeregisterForEvent(OnPOSMenuChanged);
}
//---------------------------------------------------------------------------

void TChefmateServingCourseMap::SetDatabase( TIBDatabase *inIBDatabase )
{
	ibDatabase = inIBDatabase;
}
//---------------------------------------------------------------------------

// This function is not used since the ServingCourseKey field coming from
// the POS is always 0. addServingCourseWithCourseKey is used instead
TChefmateServingCourse TChefmateServingCourseMap::ServingCourseWithKey( __int32 inSCKey )
{
	TChefmateServingCourse result;

	//:::::::::::::::::::::::::::::::::::::::::::::::::

	map<__int32, TChefmateServingCourse>::const_iterator scIT =
		_scMap.find( inSCKey );

	if( scIT == _scMap.end() )  // SCKey not found!!!
	{
		_readServingCourseWithKey( inSCKey, result );
		_scMap[inSCKey] = result;
	}
	else
	{
		result = _scMap[inSCKey];
	}

	//:::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

TChefmateServingCourse TChefmateServingCourseMap::ServingCourseWithCourseKey( __int32 inCKey )
{
	TChefmateServingCourse result;

	//:::::::::::::::::::::::::::::::::::::::::::::::::

	map<__int32, TChefmateServingCourse>::const_iterator c_scIT =
		_c_scMap.find( inCKey );

	if( c_scIT == _c_scMap.end() )  // SCKey not found!!!
	{
		_readServingCourseWithCourseKey( inCKey, result );
		_c_scMap[inCKey] = result;
	}
	else
	{
		result = _c_scMap[inCKey];
	}

	//:::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

TChefmateServingCourse TChefmateServingCourseMap::ServingCourseWithName( UnicodeString inSCName )
{
	TChefmateServingCourse result;

	//:::::::::::::::::::::::::::::::::::::::::::::::::

	map<__int32, TChefmateServingCourse>::const_iterator scIT;

	bool servingCourseFound = false;

	for ( scIT = _scMap.begin(); scIT != _scMap.end() && !servingCourseFound; scIT++ )
	{
		if ( scIT->second.Name == inSCName )
		{
			servingCourseFound = true;
			result = scIT->second;
        }
    }

	if( !servingCourseFound )  // SCName not found!!!
	{
		_readServingCourseWithName( inSCName, result );
		_scMap[result.key] = result;
	}

	//:::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------
// clears out the map used to hold serving course orders
// this method will be called everytime a menu has been updated, so that the very next item will pickup the
// latest serving course order changes. This is done as part of the fix for case #27453
void TChefmateServingCourseMap::ClearChefmateServingCourseMap()
{
    if(_scMap.size() > 0)
    {
        _scMap.clear();
    }
}

//---------------------------------------------------------------------------

void TChefmateServingCourseMap::OnPOSMenuChanged(TSystemEvents *Sender)
{
    // menu change occurred. Clearout the serving course order structure
    ClearChefmateServingCourseMap();
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  PROTECTED FUNCTIONS
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// This function is not used since the ServingCourseKey field coming from
// the POS is always 0. _readServingCourseWithCourseKey is used instead
void TChefmateServingCourseMap::_readServingCourseWithKey(
									__int32                 inSCKey,
									TChefmateServingCourse& outSC )
{
	Database::TDBTransaction dbTransaction( ibDatabase );

	_readServingCourseFromDBWithKey( inSCKey, &dbTransaction, outSC );
}
//---------------------------------------------------------------------------

// This function is not used since the ServingCourseKey field coming from
// the POS is always 0. _readServingCourseFromDBWithCourseKey is used instead
void TChefmateServingCourseMap::_readServingCourseFromDBWithKey(
									__int32                   inSCKey,
									Database::TDBTransaction* inDBTransaction,
									TChefmateServingCourse&   outSC )
{
	inDBTransaction->StartTransaction();

	//:::::::::::::::::::::::::::::::::::::::

	TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
	qr->SQL->Text = "SELECT ServingCourse_Name, ServingCourse_Kitchen_Name, Display_Order "
					"FROM SERVINGCOURSES "
					"WHERE ServingCourses_Key = :key;";

	qr->ParamByName( "key" )->AsInteger = inSCKey;

	qr->ExecQuery();

	//.....................................

	outSC.key         = inSCKey;
	outSC.Name        = qr->Fields[0]->AsString;
	outSC.KitchenName = qr->Fields[1]->AsString;
	outSC.DisplayOrder = qr->Fields[2]->AsInteger;

	inDBTransaction->Commit();
}
//---------------------------------------------------------------------------

void TChefmateServingCourseMap::_readServingCourseWithName(
									UnicodeString           inSCName,
									TChefmateServingCourse& outSC )
{
	Database::TDBTransaction dbTransaction( ibDatabase );

	_readServingCourseFromDBWithName( inSCName, &dbTransaction, outSC );
}
//---------------------------------------------------------------------------

void TChefmateServingCourseMap::_readServingCourseFromDBWithName(
									UnicodeString             inSCName,
									Database::TDBTransaction* inDBTransaction,
									TChefmateServingCourse&   outSC )
{
	inDBTransaction->StartTransaction();

	//:::::::::::::::::::::::::::::::::::::::

	TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
	qr->SQL->Text = "SELECT sc.ServingCourses_key, sc.ServingCourse_Name, sc.ServingCourse_Kitchen_Name, sc.Display_Order "
					"FROM SERVINGCOURSES sc "
					"WHERE sc.SERVINGCOURSE_NAME = :name;";

	qr->ParamByName( "name" )->AsString = inSCName;

	qr->ExecQuery();

	//.....................................

	outSC.key         = qr->Fields[0]->AsInteger;
	outSC.Name        = qr->Fields[1]->AsString;
	outSC.KitchenName = qr->Fields[2]->AsString;
	outSC.DisplayOrder = qr->Fields[3]->AsInteger;

	inDBTransaction->Commit();
}
//---------------------------------------------------------------------------

void TChefmateServingCourseMap::_readServingCourseWithCourseKey(
									__int32                 inCKey,
									TChefmateServingCourse& outSC )
{
	Database::TDBTransaction dbTransaction( ibDatabase );

	_readServingCourseFromDBWithCourseKey( inCKey, &dbTransaction, outSC );
}
//---------------------------------------------------------------------------

void TChefmateServingCourseMap::_readServingCourseFromDBWithCourseKey(
									__int32                   inCKey,
									Database::TDBTransaction* inDBTransaction,
									TChefmateServingCourse&   outSC )
{
	inDBTransaction->StartTransaction();

	//:::::::::::::::::::::::::::::::::::::::

	TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
	qr->SQL->Text = "SELECT sc.ServingCourses_key, sc.ServingCourse_Name, sc.ServingCourse_Kitchen_Name, sc.Display_Order "
					"FROM SERVINGCOURSES sc INNER JOIN COURSE c "
					"		  ON sc.SERVINGCOURSES_KEY = c.SERVINGCOURSES_KEY "
					"WHERE c.COURSE_KEY = :key;";

	qr->ParamByName( "key" )->AsInteger = inCKey;

	qr->ExecQuery();

	//.....................................

	outSC.key         = qr->Fields[0]->AsInteger;
	outSC.Name        = qr->Fields[1]->AsString;
	outSC.KitchenName = qr->Fields[2]->AsString;
	outSC.DisplayOrder = qr->Fields[3]->AsInteger;

	inDBTransaction->Commit();
}
//---------------------------------------------------------------------------

//............................................................................

//::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TChefmateCourseMap
//::::::::::::::::::::::::::::::::::::::::::::::::::::::

TChefmateCourseMap::TChefmateCourseMap()
{
}
//---------------------------------------------------------------------------

TChefmateCourseMap::~TChefmateCourseMap()
{
}
//---------------------------------------------------------------------------

void TChefmateCourseMap::SetDatabase( TIBDatabase *inIBDatabase )
{
	ibDatabase = inIBDatabase;
}
//---------------------------------------------------------------------------

TChefmateCourse TChefmateCourseMap::CourseWithKey( __int32 inCKey )
{
	TChefmateCourse result;

	//:::::::::::::::::::::::::::::::::::::::::::::::::

	map<__int32, TChefmateCourse>::const_iterator cIT =
		_cMap.find( inCKey );

	if( cIT == _cMap.end() )  // CKey not found!!!
	{
		_readCourseWithKey( inCKey, result );
		_cMap[inCKey] = result;
	}
	else
	{
		result = _cMap[inCKey];
	}

	//:::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  PROTECTED FUNCTIONS
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void TChefmateCourseMap::_readCourseWithKey(
									__int32          inCKey,
									TChefmateCourse& outSC )
{
	Database::TDBTransaction dbTransaction( ibDatabase );

	_readCourseFromDBWithKey( inCKey, &dbTransaction, outSC );
}
//---------------------------------------------------------------------------

void TChefmateCourseMap::_readCourseFromDBWithKey(
							__int32                   inCKey,
							Database::TDBTransaction* inDBTransaction,
							TChefmateCourse&          outC )
{
	inDBTransaction->StartTransaction();

	//:::::::::::::::::::::::::::::::::::::::

	TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
	qr->SQL->Text = "SELECT Course_Name, Course_Kitchen_Name, ServingCourses_Key, CAO "
					"FROM COURSE "
					"WHERE Course_Key = :key;";

	qr->ParamByName( "key" )->AsInteger = inCKey;

	qr->ExecQuery();

	//.....................................

	outC.Name             = qr->Fields[0]->AsString;
	outC.KitchenName      = qr->Fields[1]->AsString;
	outC.ServingCourseKey = qr->Fields[2]->AsInteger;
	outC.DisplayOrder 	  = qr->Fields[3]->AsInteger;

	inDBTransaction->Commit();
}
//---------------------------------------------------------------------------

//............................................................................

//::::::::::::::::::::::::::::::::::::::::::::::::::::::
// TChefmatePrintingFormatMap
//::::::::::::::::::::::::::::::::::::::::::::::::::::::

TChefmatePrintingFormatMap::TChefmatePrintingFormatMap()
{
}
//---------------------------------------------------------------------------

TChefmatePrintingFormatMap::~TChefmatePrintingFormatMap()
{
}
//---------------------------------------------------------------------------

void TChefmatePrintingFormatMap::SetDatabase( TIBDatabase *inIBDatabase )
{
	ibDatabase = inIBDatabase;
}
//---------------------------------------------------------------------------

TChefmatePrintingFormat TChefmatePrintingFormatMap::ItemPFormatWithKey( __int32 inItemKey )
{
	TChefmatePrintingFormat result;

	//:::::::::::::::::::::::::::::::::::::::::::::::::

	map<__int32, TChefmatePrintingFormat>::const_iterator iPFIT =
		_iPFMap.find( inItemKey );

	if( iPFIT == _iPFMap.end() )  // Item's Key not found!!!
	{
		_readItemPFWithKey( inItemKey, result );
		_iPFMap[inItemKey] = result;
	}

	//:::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

TChefmatePrintingFormat TChefmatePrintingFormatMap::ItemOptionPFormatWithKey( __int32 inItemOptionKey )
{
	TChefmatePrintingFormat result;

	//:::::::::::::::::::::::::::::::::::::::::::::::::

	map<__int32, TChefmatePrintingFormat>::const_iterator iOPFIT =
		_iOPFMap.find( inItemOptionKey );

	if( iOPFIT == _iOPFMap.end() )  // Item Option's Key not found!!!
	{
		_readItemOptionPFWithKey( inItemOptionKey, result );
		_iOPFMap[inItemOptionKey] = result;
	}

	//:::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

TChefmatePrintingFormat TChefmatePrintingFormatMap::ItemSidePFormatWithKey( __int32 inItemSideKey )
{
	TChefmatePrintingFormat result;

	//:::::::::::::::::::::::::::::::::::::::::::::::::

	map<__int32, TChefmatePrintingFormat>::const_iterator iSPFIT =
		_iSPFMap.find( inItemSideKey );

	if( iSPFIT == _iSPFMap.end() )  // Item Side's Key not found!!!
	{
		_readItemSidePFWithKey( inItemSideKey, result );
		_iSPFMap[inItemSideKey] = result;
	}

	//:::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

TChefmatePrintingFormat TChefmatePrintingFormatMap::SideOptionPFormatWithKey(  __int32 inSideOptionKey )
{
	TChefmatePrintingFormat result;

	//:::::::::::::::::::::::::::::::::::::::::::::::::

	map<__int32, TChefmatePrintingFormat>::const_iterator sOPFIT =
		_sOPFMap.find( inSideOptionKey );

	if( sOPFIT == _sOPFMap.end() )  // Item Side's Key not found!!!
	{
		_readSideOptionPFWithKey( inSideOptionKey, result );
		_sOPFMap[inSideOptionKey] = result;
	}

	//:::::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  PROTECTED FUNCTIONS
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void TChefmatePrintingFormatMap::_readItemPFWithKey(
										__int32                  inItemKey,
										TChefmatePrintingFormat& outPF )
{
	Database::TDBTransaction dbTransaction( ibDatabase );
	dbTransaction.StartTransaction();

	_readItemPFFromDBWithKey( inItemKey, &dbTransaction, outPF );

	dbTransaction.Commit();
}
//---------------------------------------------------------------------------

void TChefmatePrintingFormatMap::_readItemPFFromDBWithKey(
									__int32                   inCKey,
									Database::TDBTransaction* inDBTransaction,
									TChefmatePrintingFormat&  outPF )
{
	outPF.Underline    = false;
	outPF.Bold         = false;
	outPF.Color        = clBlack;
	outPF.LargeFont    = false;
	outPF.DoubleWidth  = false;
	outPF.DoubleHeight = false;

/*
	inDBTransaction->StartTransaction();

	//:::::::::::::::::::::::::::::::::::::::

	TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
	qr->SQL->Text = "SELECT Course_Name, Course_Kitchen_Name, ServingCourses_Key "
					"FROM COURSE "
					"WHERE Course_Key = :key;";

	qr->ParamByName( "key" )->AsInteger = inCKey;

	qr->ExecQuery();

	//.....................................

	outPF.Underline    = qr->Fields[0]->AsString;
	outPF.Bold         = qr->Fields[1]->AsString;
	outPF.Color        = qr->Fields[2]->AsString;
	outPF.LargeFont    = qr->Fields[3]->AsString;
	outPF.DoubleWidth  = qr->Fields[4]->AsString;
	outPF.DoubleHeight = qr->Fields[5]->AsString;

	inDBTransaction->Commit();
*/
}
//---------------------------------------------------------------------------

void TChefmatePrintingFormatMap::_readItemOptionPFWithKey(
										__int32                  inItemOptionKey,
										TChefmatePrintingFormat& outPF )
{
	Database::TDBTransaction dbTransaction( ibDatabase );
	dbTransaction.StartTransaction();

	_readItemOptionPFFromDBWithKey( inItemOptionKey, &dbTransaction, outPF );

	dbTransaction.Commit();
}
//---------------------------------------------------------------------------

void TChefmatePrintingFormatMap::_readItemOptionPFFromDBWithKey(
									__int32                   inCKey,
									Database::TDBTransaction* inDBTransaction,
									TChefmatePrintingFormat&  outPF )
{
	outPF.Underline    = false;
	outPF.Bold         = false;
	outPF.Color        = clBlack;
	outPF.LargeFont    = false;
	outPF.DoubleWidth  = false;
	outPF.DoubleHeight = false;

/*
	inDBTransaction->StartTransaction();

	//:::::::::::::::::::::::::::::::::::::::

	TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
	qr->SQL->Text = "SELECT ( Course_Name, Course_Kitchen_Name, ServingCourses_Key ) "
					"FROM COURSE "
					"WHERE Course_Key = :key;";

	qr->ParamByName( "key" )->AsInteger = inCKey;

	qr->ExecQuery();

	//.....................................

	outPF.Underline    = qr->Fields[0]->AsString;
	outPF.Bold         = qr->Fields[1]->AsString;
	outPF.Color        = qr->Fields[2]->AsString;
	outPF.LargeFont    = qr->Fields[3]->AsString;
	outPF.DoubleWidth  = qr->Fields[4]->AsString;
	outPF.DoubleHeight = qr->Fields[5]->AsString;

	inDBTransaction->Commit();
*/
}
//---------------------------------------------------------------------------

void TChefmatePrintingFormatMap::_readItemSidePFWithKey(
										__int32                  inItemSideKey,
										TChefmatePrintingFormat& outPF )
{
	Database::TDBTransaction dbTransaction( ibDatabase );
	dbTransaction.StartTransaction();

	_readItemSidePFFromDBWithKey( inItemSideKey, &dbTransaction, outPF );

	dbTransaction.Commit();
}
//---------------------------------------------------------------------------

void TChefmatePrintingFormatMap::_readItemSidePFFromDBWithKey(
										__int32                   inItemSideKey,
										Database::TDBTransaction* inDBTransaction,
										TChefmatePrintingFormat&  outPF )
{
	outPF.Underline    = false;
	outPF.Bold         = false;
	outPF.Color        = clBlack;
	outPF.LargeFont    = false;
	outPF.DoubleWidth  = false;
	outPF.DoubleHeight = false;

/*
	inDBTransaction->StartTransaction();

	//:::::::::::::::::::::::::::::::::::::::

	TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
	qr->SQL->Text = "SELECT Course_Name, Course_Kitchen_Name, ServingCourses_Key "
					"FROM COURSE "
					"WHERE Course_Key = :key;";

	qr->ParamByName( "key" )->AsInteger = inCKey;

	qr->ExecQuery();

	//.....................................

	outPF.Underline    = qr->Fields[0]->AsString;
	outPF.Bold         = qr->Fields[1]->AsString;
	outPF.Color        = qr->Fields[2]->AsString;
	outPF.LargeFont    = qr->Fields[3]->AsString;
	outPF.DoubleWidth  = qr->Fields[4]->AsString;
	outPF.DoubleHeight = qr->Fields[5]->AsString;

	inDBTransaction->Commit();
*/
}
//---------------------------------------------------------------------------

void TChefmatePrintingFormatMap::_readSideOptionPFWithKey(
										__int32                  inSideOptionKey,
										TChefmatePrintingFormat& outPF )
{
	Database::TDBTransaction dbTransaction( ibDatabase );
	dbTransaction.StartTransaction();

	_readSideOptionPFFromDBWithKey( inSideOptionKey, &dbTransaction, outPF );

	dbTransaction.Commit();
}
//---------------------------------------------------------------------------

void TChefmatePrintingFormatMap::_readSideOptionPFFromDBWithKey(
									__int32                   inSideOptionKey,
									Database::TDBTransaction* inDBTransaction,
									TChefmatePrintingFormat&  outPF )
{
	outPF.Underline    = false;
	outPF.Bold         = false;
	outPF.Color        = clBlack;
	outPF.LargeFont    = false;
	outPF.DoubleWidth  = false;
	outPF.DoubleHeight = false;

/*
	inDBTransaction->StartTransaction();

	//:::::::::::::::::::::::::::::::::::::::

	TIBSQL *qr    = inDBTransaction->Query( inDBTransaction->AddQuery() );
	qr->SQL->Text = "SELECT ( Course_Name, Course_Kitchen_Name, ServingCourses_Key ) "
					"FROM COURSE "
					"WHERE Course_Key = :key;";

	qr->ParamByName( "key" )->AsInteger = inCKey;

	qr->ExecQuery();

	//.....................................

	outPF.Underline    = qr->Fields[0]->AsString;
	outPF.Bold         = qr->Fields[1]->AsString;
	outPF.Color        = qr->Fields[2]->AsString;
	outPF.LargeFont    = qr->Fields[3]->AsString;
	outPF.DoubleWidth  = qr->Fields[4]->AsString;
	outPF.DoubleHeight = qr->Fields[5]->AsString;

	inDBTransaction->Commit();
*/
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::
// TChefmateOrderNumberGenerator: This is a temporary solution for generating Order Numbers.
//:::::::::::::::::::::::::::::::::::::::::::::::::

TChefmateOrderNumberGenerator::TChefmateOrderNumberGenerator()
{
    std::vector<ip_utilities::adapter_address_info_t> addresses(
      ip_utilities::get_adapter_addresses());

    assert(addresses.size() != 0
           && "The system has no interfaces? Who's playing silly buggers?");

    terminal_ip_address_ = addresses.front().ip_address();
}
//---------------------------------------------------------------------------
TChefmateOrderNumberGenerator::~TChefmateOrderNumberGenerator()
{
}
//---------------------------------------------------------------------------
// PUBLIC METHODS
void TChefmateOrderNumberGenerator::InitOrderNumber( UnicodeString inTerminalName )
{
	_terminalName = inTerminalName;

	if( loadOrderNo( _orderNo ) )
	{
		saveOrderNo( ++_orderNo );
	}
	else
	{
		ResetOrderNumber();
	}
}
//---------------------------------------------------------------------------
void TChefmateOrderNumberGenerator::ResetOrderNumber()
{
	_orderNo = buildOrderNo( terminal_ip_address_,
                             strHashUINT( _terminalName ),
                             1 );
	saveOrderNo( _orderNo );
}
//---------------------------------------------------------------------------
void TChefmateOrderNumberGenerator::NextOrderNumber()
{
	_orderNo += 1;

	if( _orderNo == 0 )
	{
		ResetOrderNumber();
	}
	else
	{
		saveOrderNo( _orderNo );
	}
}
//---------------------------------------------------------------------------
unsigned __int32 TChefmateOrderNumberGenerator::CurrentOrderNumber()
{
	return _orderNo;
}
//---------------------------------------------------------------------------
// PRIVATE METHODS
bool TChefmateOrderNumberGenerator::loadOrderNo( unsigned __int32& outOrderNo )
{
	try
	{
		std::auto_ptr<TStringList> orderNoStr( new TStringList() );

		orderNoStr->LoadFromFile( _ORDER_NO_FILE_NAME );
		outOrderNo = StrToInt( orderNoStr->Text.Trim() );

		return true;
	}
	catch( ... )
	{
		return false;
	}
}
//---------------------------------------------------------------------------
void TChefmateOrderNumberGenerator::saveOrderNo( unsigned __int32 inOrderNo )
{
	UnicodeString str;

	try
	{
		str = StrToInt( inOrderNo );
	}
	catch( ... )
	{
		str = buildOrderNo( terminal_ip_address_,
                            strHashUINT( _terminalName ),
                            0 );
	}

	std::auto_ptr<TStringList> orderNoStr( new TStringList() );

	orderNoStr->Text = str;
	orderNoStr->SaveToFile( _ORDER_NO_FILE_NAME );
}
//---------------------------------------------------------------------------
unsigned __int32 TChefmateOrderNumberGenerator::buildOrderNo(
                    const ip_utilities::ip_address_t &inTerminalIPAddress,
					unsigned __int32 inOrderNoPrefix,
					unsigned __int32 inOrderNoCounter )
{
	return StrToInt( IntToStr( inTerminalIPAddress.octet_d() )
                     + IntToStr( ( __int32 )inOrderNoPrefix )
					 + IntToStr( ( __int32 )inOrderNoCounter ) );
}
//---------------------------------------------------------------------------
unsigned __int32 TChefmateOrderNumberGenerator::strHashUINT( UnicodeString inStr )
{
	unsigned __int32 hashVal = 0;
	char*            strPtr  = inStr.t_str();


	/* for each character, we multiply the old hash by 31 and add the current
     * character.  Remember that shifting a number left is equivalent to
	 * multiplying it by 2 raised to the number of places shifted.  So we
     * are in effect multiplying hashval by 32 and then subtracting hashval.
	 * Why do we do this?  Because shifting and subtraction are much more
     * efficient operations than multiplication.
	 */
	for( int i=0; i < inStr.Length(); i++ )
	{
		hashVal = strPtr[i] + ( ( hashVal << 5 ) - hashVal );
    }

    /* we then return the hash value mod the hashmap size so that it will
     * fit into the necessary range
     */
	hashVal = hashVal % ( sizeof( unsigned __int32 ) );

	return hashVal;
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::
// TDBChefmate
//:::::::::::::::::::::::::::::::::::::::::::::::::

int TDBChefmate::GetItemSizeKey( int inItemKey, UnicodeString inSizeName )
{
	int itemSizeKey = 0;

	Database::TDBTransaction DBTransaction( TDeviceRealControl::ActiveInstance().DBControl );
	DBTransaction.Start();
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"SELECT "
				"ITEMSIZE_KEY "
			"FROM "
				"ITEMSIZE "
			"WHERE "
				"ITEM_KEY = :ITEM_KEY "
			"AND "
				"SIZE_NAME = :SIZE_NAME";
		IBInternalQuery->ParamByName("ITEM_KEY")->AsInteger = inItemKey;
		IBInternalQuery->ParamByName("SIZE_NAME")->AsString = inSizeName;
		IBInternalQuery->ExecQuery();

		if(!IBInternalQuery->Eof)
		{
			itemSizeKey = IBInternalQuery->FieldByName("ITEMSIZE_KEY")->AsInteger;
        }
	}
	catch (Exception &E)
	{
		DBTransaction.Undo();
    }

	return itemSizeKey;
}
//---------------------------------------------------------------------------

long TDBChefmate::GetItemCourseKey( int inItemKey )
{
	long itemCourseKey = 0;

	Database::TDBTransaction DBTransaction( TDeviceRealControl::ActiveInstance().DBControl );
	DBTransaction.Start();
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"SELECT "
				"COURSE_KEY "
			"FROM "
				"ITEM "
			"WHERE "
				"ITEM_KEY = :ITEM_KEY ";
		IBInternalQuery->ParamByName("ITEM_KEY")->AsInteger = inItemKey;
		IBInternalQuery->ExecQuery();

		if(!IBInternalQuery->Eof)
		{
			itemCourseKey = IBInternalQuery->FieldByName("COURSE_KEY")->AsInteger;
        }
	}
	catch (Exception &E)
	{
		DBTransaction.Undo();
    }

	return itemCourseKey;
}
//---------------------------------------------------------------------------

UnicodeString TDBChefmate::GetItemName( int inItemKey )
{
	UnicodeString itemName = "";

	Database::TDBTransaction DBTransaction( TDeviceRealControl::ActiveInstance().DBControl );
	DBTransaction.Start();
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"SELECT "
				"ITEM_NAME "
			"FROM "
				"ITEM "
			"WHERE "
				"ITEM_KEY = :ITEM_KEY ";
		IBInternalQuery->ParamByName("ITEM_KEY")->AsInteger = inItemKey;
		IBInternalQuery->ExecQuery();

		if(!IBInternalQuery->Eof)
		{
			itemName = IBInternalQuery->FieldByName("ITEM_NAME")->AsString;
        }
	}
	catch (Exception &E)
	{
		DBTransaction.Undo();
    }

	return itemName;
}
//---------------------------------------------------------------------------

UnicodeString TDBChefmate::GetItemKitchenName( int inItemKey )
{
	UnicodeString itemKitchenName = "";

	Database::TDBTransaction DBTransaction( TDeviceRealControl::ActiveInstance().DBControl );
	DBTransaction.Start();
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"SELECT "
				"ITEM_NAME, "
				"ITEM_KITCHEN_NAME "
			"FROM "
				"ITEM "
			"WHERE "
				"ITEM_KEY = :ITEM_KEY ";
		IBInternalQuery->ParamByName("ITEM_KEY")->AsInteger = inItemKey;
		IBInternalQuery->ExecQuery();

		if(!IBInternalQuery->Eof)
		{
			itemKitchenName = IBInternalQuery->FieldByName("ITEM_KITCHEN_NAME")->AsString;
			if (itemKitchenName == "")
			{
				itemKitchenName = IBInternalQuery->FieldByName("ITEM_NAME")->AsString;
			}
        }
	}
	catch (Exception &E)
	{
		DBTransaction.Undo();
    }

	return itemKitchenName;
}
//---------------------------------------------------------------------------
