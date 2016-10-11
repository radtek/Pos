//---------------------------------------------------------------------------

#ifndef ChefMateInterfaceH
#define ChefMateInterfaceH

//---------------------------------------------------------------------------

#include <vcl.h>
#include <windows.h>
#include <IBDatabase.hpp>

#include "MM_DBCore.h"

#include "ChefMateOrderGenerator.h"
#include "ChefmateTools.h"

// This is used until the DLL is fixed
#include "XMLOrderSmartManager.h"
//#include "XMLOrderManager.h"


//---------------------------------------------------------------------------

#define _ACTIVE_ID  10001
#define _IP_ADDR_ID 10002
#define _IP_PORT_ID 10003
#define _DYNAMIC_ID 10004

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  TChefmateSettings
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

class TChefmateSettings
{
protected:
	TIBDatabase *ibDatabase;

	bool          FActive;
	UnicodeString FIPAddr;
	unsigned int  FIPPort;
	bool          FDynamic;

	void _setActive(  bool          inActive  );
	void _setIPAddr(  UnicodeString inIPAddr  );
	void _setIPPort(  unsigned int  inIPPort  );
	void _setDynamic( bool          inDynamic );

public:
/*	static TChefmateSettings& Instance()
	{
		static TChefmateSettings singleton;
		return singleton;
	}*/
	TChefmateSettings();
	~TChefmateSettings();

	void SetDatabase( TIBDatabase *inIBDatabase );
	void Edit( TForm* inForm );

	__property bool          Active  = { read = FActive,  write = _setActive };
	__property UnicodeString IPAddr  = { read = FIPAddr,  write = _setIPAddr };
	__property unsigned int  IPPort  = { read = FIPPort,  write = _setIPPort };
	__property bool          Dynamic = { read = FDynamic, write = _setDynamic };

private:

	void loadSettingsFromDB(
			Database::TDBTransaction* inDBTransaction,
			bool&          outActive,
			UnicodeString& outIPAddr,
			unsigned int&  outIPPort,
			bool&          outDynamic );

	void initSetting(
				Database::TDBTransaction* inDBTransaction,
				__int32        			  inSettingID,
				UnicodeString          	  inName,
				UnicodeString          	  inDescription,
				UnicodeString          	  inDefaultValue );

	void saveActiveToDB(
			Database::TDBTransaction* inDBTransaction,
			bool inActive );
	void saveIPAddrToDB(
			Database::TDBTransaction* inDBTransaction,
			UnicodeString inIPAddr );
	void saveIPPortToDB(
			Database::TDBTransaction* inDBTransaction,
			unsigned int              inIPPort );
	void saveDynamicToDB(
			Database::TDBTransaction* inDBTransaction,
			bool 					  inDynamic );

	bool variableHasBeenSet(
			Database::TDBTransaction* inDBTransaction,
			__int32                   inVariableID );

	void setVarStr(
			Database::TDBTransaction* inDBTransaction,
			__int32                   inVariableID,
			UnicodeString             inName,
			UnicodeString             inDiscription,
			__int32                   inGroup,
			UnicodeString             inValue );
	void updateVarValueStr(
			Database::TDBTransaction* inDBTransaction,
			__int32                   inVariableID,
			UnicodeString             inValue );
	void updateVarStr(
			Database::TDBTransaction* inDBTransaction,
			__int32                   inVariableID,
			UnicodeString             inName,
			UnicodeString             inDiscription,
			__int32                   inGroup,
			UnicodeString             inValue );
	UnicodeString readVarValueStr(
			Database::TDBTransaction* inDBTransaction,
			__int32                   inVariableID );
	void readVarStr(
			Database::TDBTransaction* inDBTransaction,
			__int32                   inVariableID,
			UnicodeString&            outName,
			UnicodeString&            outDiscription,
			__int32&                  outGroup,
			UnicodeString&            outValue );

};

//---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//   TChefmateInterface
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

class TChefmateInterface
{
protected:
	bool              _chefmateActive();
	bool              _getOpen();

public:
  	TChefmateInterface();
	~TChefmateInterface();

	bool Available();
	bool Init();
	void Done();

	void SetDatabase( TIBDatabase *inIBDatabase );

	void Clear();

	bool OpenIncompleteOrder(
			__int32       inOrderDBKey,
			UnicodeString inServerName,
			__int32       inOrderNumber,
			UnicodeString inChitValue,
			UnicodeString inTableTabName,
			UnicodeString inOrderType,
			UnicodeString inCustomerName,
			UnicodeString inPartyName );

	bool OpenCompleteOrder(
			__int32       inOrderDBKey,
			UnicodeString inServerName,
			__int32       inOrderNumber,
			UnicodeString inChitValue,
			UnicodeString inTableTabName,
			UnicodeString inOrderType,
			UnicodeString inCustomerName,
			UnicodeString inPartyName,
			UnicodeString inPatronCount,
			UnicodeString inSaleStartTime,
            UnicodeString inDeliveryTime,
            UnicodeString inCustomerPhone,
            UnicodeString inCustomerEmail,
            UnicodeString inCustomerAddress );

    bool OpenTransferOrder(
			__int32       inOrderDBKey,
			__int32       inOrderNumber,
			UnicodeString inTableTabName,
            UnicodeString inServerName,
			UnicodeString inPatronCount,
			UnicodeString inSaleStartTime,
            UnicodeString inSourceTableName);

	HORDERDOC OpenCancelOrder( __int32 inDBKey,
							   __int32 inOrderNumber,
                               UnicodeString inTableTabName);
	HORDERDOC OpenCreditOrder();

	HORDERDOC OpenReplacementOrder( __int32 	  inDBKey,
									__int32 	  inOrderNumber,
									UnicodeString inTableTabName );

	HORDERDOC OpenCallAwayOrder( __int32 	   inOrderNumber,
								 UnicodeString inTableTabName );

	HITEM AddItem(
			  TCMHostname   inCMHostname,
			  __int32       inItemSizeKey,
			  __int32       inOrderItemKey,
			  UnicodeString inItemName,
			  UnicodeString inItemKitchenName,
			  __int32       inCourseKey,
			  __int32       inServingCourseKey,
			  UnicodeString inNote );

	// If inCaceled is false then inQty is ignored
	HITEM AddItemToCancel(
			  TCMHostname   inCMHostname,
			  __int32       inOrderItemKey,
			  UnicodeString inItemName,
			  bool          inCanceled = true,
			  __int32       inQty      = 1 );

	__int32 AddCallAwayServingCourse(
			  TCMHostname   inCMHostname,
			  UnicodeString inServingCourseName );

	HSIDE AddItemSide(
			TCMHostname inCMHostname,
			HITEM         inItemHandle,
			__int32       inOrderSideKey,
			UnicodeString inSideName,
			UnicodeString inSideKitchenName );

	HSIDE AddItemSideToCancel(
			TCMHostname inCMHostname,
			HITEM         inItemHandle,
			__int32       inOrderSideKey,
			UnicodeString inSideName );

	void AddItemOption(
			TCMHostname inCMHostname,
			HITEM         inItemHandle,
			__int32       inOptionKey,
			UnicodeString inOptionName,
			UnicodeString inOptionKitchenName,
			bool          inIsPlusPlus );

	void AddSideOption(
			TCMHostname   inCMHostname,
			HSIDE         inSideHandle,
			__int32       inOptionKey,
			UnicodeString inOptionName,
			UnicodeString inOptionKitchenName,
			bool          inIsPlusPlus );

    bool OpenWebOrder(
			__int32       inOrderDBKey,
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
			UnicodeString inPaymentStatus = "" );

	OERROR CommitOrder();

    OERROR DeleteOrder();

	__property bool Open = { read = _getOpen };

    TChefmateSettings Settings;
    TChefmateServingCourseMap ChefmateServingCourseMap;
    TChefmateCourseMap ChefmateCourseMap;
    TChefmatePrintingFormatMap ChefmatePrintingFormatMap;

private:

	HINSTANCE _cmGeneratorH;
	bool      _cmGeneratorOpen;

	TIBDatabase *ibDatabase;

	// NOT USED!!! This is for the DLL
	// Function Pointers
	_OpenOrderGenerator          _openOrderGenerator;
	_CloseOrderGenerator         _closeOrderGenerator;
	_ConfigureChafMate           _configureChefMate;
	_OpenCompleteOrder           _openCompleteOrder;
	_OpenTransferOrder           _openTransferOrder;
	_OpenIncompleteOrder         _openIncompleteOrder;
	_OpenCancelOrder             _openCancelOrder;
	_OpenCreditOrder             _openCreditOrder;
	_AddServingCourse            _addServingCourse;
	_AddCourse                   _addCourse;
	_AddItem                     _addItem;
	_AddItemPrintingFormat       _addItemPFormat;
	_AddItemOption               _addItemOption;
	_AddItemOptionPrintingFormat _addItemOptionPFormat;
	_AddItemSide 				 _addItemSide;
	_AddSidePrintingFormat       _addSidePFormat;
	_AddSideOption               _addSideOption;
	_AddSideOptionPrintingFormat _addSideOptionPFormat;
	_CommitOrder                 _commitOrder;
	_DeleteOrder                 _deleteOrder;
	_GetLastOError               _getLastOError;
	//.............................................................

	// This is used until the DLL is fixed
	TXMLOrderSmartManager *_xmlOrderManager;
	//.............................................................

	HORDERDOC _orderDocH;

	OERROR openOrderGenerator();
	void   closeOrderGenerator();

	TXMLOrderSmartManager* createXMLOrderManager();

	void loadFunctions();

	void    addServingCourseWithKey(
				TCMHostname inCMHostname,
				__int32     inServingCourseKey );

    // NOT USED!!! .... addServingCourseWithKey is used instead
	__int32 addServingCourseWithCourseKey(
				TCMHostname inCMHostname,
				__int32     inCourseKey );

	__int32 addServingCourseWithName(
				TCMHostname 	inCMHostname,
				UnicodeString   inServingCourseName );

	void    addCourseWithKey(
				TCMHostname inCMHostname,
				__int32     inCourseKey );

	void addItemPrintingFormatWithKey(
				TCMHostname inCMHostname,
				__int32     inItemSizeKey,
				HITEM       inItemH );
	void addItemOptionPrintingFormatWithKey(
				TCMHostname inCMHostname,
				__int32     inItemOptionKey,
				HOPTION     inOptionH );
	void addItemSidePrintingFormatWithKey(
				TCMHostname inCMHostname,
				__int32     inItemSideKey,
				HSIDE       inSideH );
	void addSideOptionPrintingFormatWithKey(
				TCMHostname inCMHostname,
				__int32     inSideOptionKey,
				HOPTION     inOptionH );

};

//---------------------------------------------------------------------------

#endif
