//---------------------------------------------------------------------------

#ifndef XMLOrderSmartManagerH
#define XMLOrderSmartManagerH

//---------------------------------------------------------------------------

#include "map.h"
#include "vector.h"

#include "SaveXMLOrderBuilder.h"
#include "SaveXMLOrder.h"

//---------------------------------------------------------------------------

class TSmartOrderDocHeader
{
	protected:
		AnsiString FAction;
		AnsiString FType;
		__int32    FDBKey;
		AnsiString FServerName;
		__int32    FOrderNumber;
		AnsiString FChitValue;
		AnsiString FTableName;
		AnsiString FOrderType;
		AnsiString FCustomerName;
		AnsiString FPartyName;
		AnsiString FPatronCount;
		AnsiString FSaleStartTime;
        AnsiString FDeliveryTime;
        AnsiString FSourceTableName;

	public:
		 TSmartOrderDocHeader(
			AnsiString inAction,
			AnsiString inType,
			__int32    inDBKey,
			AnsiString inServerName,
			__int32    inOrderNumber,
			AnsiString inChitValue,
			AnsiString inTableName,
			AnsiString inOrderType,
			AnsiString inCustomerName,
			AnsiString inPartyName,
			AnsiString inPatronCount,
			AnsiString inSaleStartTime,
            AnsiString inDeliveryTime,
            AnsiString inSourceTableName = "");

		~TSmartOrderDocHeader();

		__property AnsiString Action        = { read = FAction       };
		__property AnsiString Type          = { read = FType         };
		__property __int32    DBKey         = { read = FDBKey        };
		__property AnsiString ServerName    = { read = FServerName   };
		__property __int32    OrderNumber   = { read = FOrderNumber  };
		__property AnsiString ChitValue     = { read = FChitValue    };
		__property AnsiString TableName     = { read = FTableName    };
		__property AnsiString OrderType     = { read = FOrderType    };
		__property AnsiString CustomerName  = { read = FCustomerName };
		__property AnsiString PartyName     = { read = FPartyName    };
		__property AnsiString PatronCount   = { read = FPatronCount  };
		__property AnsiString SaleStartTime = { read = FSaleStartTime  };
                __property AnsiString DeliveryTime = { read = FDeliveryTime  };
       __property AnsiString SourceTableName = { read = FSourceTableName  };
};

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

class TSmartOrderDoc
{
	protected:
		TSmartOrderDocHeader* FHeader;

		TSaveXMLOrder* _createOrderXMLDoc();

	public:
		 TSmartOrderDoc( TSmartOrderDocHeader* inHeader );
		~TSmartOrderDoc();

		__property TSmartOrderDocHeader* Header = { read = FHeader };

		TSaveXMLOrder* XmlDocWithHostname( TCMHostname inHostName );
		      __int32  GetOrderNumber();
				 void  HostnameList( std::vector<TCMHostname>& outList );

	private:
		// MAP = [Hostname / IP Address] : [XML Order]
		std::map<TCMHostname, TSaveXMLOrder> _xmlOrderMap;

		TSmartOrderDocHeader* createHeader( TSmartOrderDocHeader* inHeader );
		void openOrder( TSaveXMLOrder* inOrderDoc );
		void openCompleteOrder( TSaveXMLOrder* inOrderDoc );

};

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

class TXMLOrderSmartManager
{
	public:
		 TXMLOrderSmartManager();
		~TXMLOrderSmartManager();
		// Open a Complete Order
		// Order's components must be added afterwards
		// Once all the components are added SaveOrder() must be called
		HORDERDOC OpenCompleteOrder(
					__int32    inDBKey,
					AnsiString inServerName,
					__int32    inOrderNumber,
					AnsiString inChitValue,
					AnsiString inTableTabName,
					AnsiString inOrderType,
					AnsiString inCustomerName,
				    AnsiString inPartyName,
					AnsiString inPatronCount,
					AnsiString inSaleStartTime,
                                        AnsiString inDeliveryTime );

		// Opens an Incomplete Order
		// Order's components must be added afterwards
		// Once all the components are added SaveOrder() must be called
		HORDERDOC OpenIncompleteOrder(
					__int32    inDBKey,
					AnsiString inServerName,
					__int32    inOrderNumber,
					AnsiString inChitValue,
					AnsiString inTableTabName,
					AnsiString inOrderType,
					AnsiString inCustomerName,
					AnsiString inPartyName );

		// Opens a Cancel Order
		// The items to be canceled must be added afterwards
		// Once all the items are added SaveOrder() must be called
		HORDERDOC OpenCancelOrder(
					__int32 inDBKey,
					__int32 inOrderNumber,
                      AnsiString inTableTabName);

		// Opens a Credit Order
		// The items to be canceled must be added afterwards
		// Once all the items are added SaveOrder() must be called
		HORDERDOC OpenCreditOrder();

		// Open a Replacement Order
		// Order's components must be added afterwards
		// Once all the components are added SaveOrder() must be called
		HORDER OpenReplacementOrder( __int32    inDBKey,
									 __int32    inOrderNumber,
									 AnsiString inTableTabName );

		// Open a Call Away Order
		// Order's serving courses must be added afterwards
		// Once all the serving courses are added SaveOrder() must be called
		HORDER OpenCallAwayOrder( __int32    inOrderNumber,
								  AnsiString inTableTabName );

		// Opens a Transfer Order
		// SaveOrder() must be called straight away
		HORDERDOC OpenTransferOrder(
					__int32     inDBKey,
					__int32     inOrderNumber,
					AnsiString  inTableTabName,
                    AnsiString  inServerName,
			        AnsiString inPatronCount,
			        AnsiString inSaleStartTime,
                    AnsiString inSourceTableName );

		// Adds a ServingCourse to an Order
		HSCOURSE AddServingCourse(
					HORDERDOC   inOrderDocHandle,
					TCMHostname inHostname,
					__int32     inDBKey,
					AnsiString  inName,
					AnsiString  inKitchenName,
					__int32 	inDisplayOrder	);

		// Adds a Course to an Order
		HCOURSE AddCourse(
					HORDERDOC   inOrderDocHandle,
					TCMHostname inHostname,
					__int32     inDBKey,
					AnsiString  inName,
					AnsiString  inKitchenName,
					__int32     inServingCourseKey,
					__int32 	inDisplayOrder );

		// Adds a Item to an Order
		HITEM AddItem(
					HORDERDOC     inOrderDocHandle,
					TCMHostname   inHostname,
					__int32       inDBKey,
					__int32       inOrderItemDBKey,
					AnsiString    inName,
					AnsiString    inKitchenName,
					__int32       inServingCourseKey,
					__int32       inCourseKey,
					UnicodeString inNote );

		// Adds a Item to cancel to an Order
		// THIS FUNCTION IS TO BE USED FOR CANCEL ORDERS ONLY
		// If inCaceled is false then inQty is ignored
		HITEM AddItemToCancel(
				  HORDERDOC   inOrderDocHandle,
				  TCMHostname inHostname,
				  __int32     inOrderItemKey,
				  AnsiString  inItemName,
				  bool        inCanceled = true,
				  __int32     inQty      = 1 );

		// Adds a Item's Print Format to an Order
		HPFORMAT AddItemPrintingFormat(
					HORDERDOC   inOrderDocHandle,
					TCMHostname inHostname,
					HITEM       inItemHandle,
					__int32     inDBKey,
					bool	    inUnderline,
					bool	    inBold,
					TColor	    inColor,
					bool	    inLargeFont,
					bool	    inDoubleWidth,
					bool	    inDoubleHeight );

		// Adds a Item's Option to an Order
		HOPTION AddItemOption(
					HORDERDOC   inOrderDocHandle,
					TCMHostname inHostname,
					HITEM       inItemHandle,
					__int32     inDBKey,
					AnsiString  inName,
					AnsiString  inKitchenName,
					bool	    inIsPlus );

		// Adds a Item's Option Print Format to an Order
		HPFORMAT AddItemOptionPrintingFormat(
					HORDERDOC   inOrderDocHandle,
					TCMHostname inHostname,
					HOPTION     inOptionHandle,
					__int32     inDBKey,
					bool	    inUnderline,
					bool	    inBold,
					TColor      inColor,
					bool	    inLargeFont,
					bool	    inDoubleWidth,
					bool	    inDoubleHeight );

		// Adds a Item's Side to an Order
		HSIDE AddItemSide(
					HORDERDOC   inOrderDocHandle,
					TCMHostname inHostname,
					HITEM       inItemHandle,
					__int32     inOrderSideKey,
					AnsiString  inName,
					AnsiString  inKitchenName );

		// Adds a Item's Side to cancel to an Order
		HSIDE AddItemSideToCancel(
					HORDERDOC   inOrderDocHandle,
					TCMHostname inHostname,
					HITEM       inItemHandle,
					__int32     inOrderSideKey,
					AnsiString  inSideName );

		// Adds a Option's Print Format to an Order
		HPFORMAT AddSidePrintingFormat(
					HORDERDOC   inOrderDocHandle,
					TCMHostname inHostname,
					HSIDE       inSideHandle,
					__int32     inDBKey,
					bool	    inUnderline,
					bool	    inBold,
					TColor	    inColor,
					bool	    inLargeFont,
					bool	    inDoubleWidth,
					bool	    inDoubleHeight );

		// Adds a Side's Option to an Order
		HOPTION AddSideOption(
					HORDERDOC   inOrderDocHandle,
					TCMHostname inHostname,
					HSIDE       inSideHandle,
					__int32     inDBKey,
					AnsiString  inName,
					AnsiString  inKitchenName,
					bool	    inIsPlus );

		// Adds a Option's Print Format to an Order
		HPFORMAT AddSideOptionPrintingFormat(
					HORDERDOC   inOrderDocHandle,
					TCMHostname inHostname,
					HOPTION     inOptionHandle,
					__int32     inDBKey,
					bool	    inUnderline,
					bool	    inBold,
					TColor	    inColor,
					bool	    inLargeFont,
					bool	    inDoubleWidth,
					bool	    inDoubleHeight );

		// Commits an Order
		OERROR CommitOrder( HORDERDOC inOrderDocHandle );

		// Deletes an Order
		OERROR DeleteOrder( HORDERDOC inOrderDocHandle );

		OERROR GetLastOError();

	private:
		// List of Open XML Orders
		std::vector <TSmartOrderDoc> _xmlSmartOrderList;

		OERROR _lastOError;

		//::::::::::::::::::::::::::::::::::::::::::::::::::::

		bool orderOpen( __int32 inOrderNumber );

		HORDERDOC findOrder( __int32 inOrderNumber );

		TSmartOrderDoc* createXMLSmartOrderDoc( TSmartOrderDocHeader* inHeader );

		HORDERDOC addSmartOrderXMLDoc( TSmartOrderDoc* inSmartOrderDoc );

		TSaveXMLOrder* getXMLOrderDoc(
						HORDERDOC   inOrderDocHandle,
						TCMHostname inHostname );

		OERROR deleteOrderDoc( HORDERDOC inOrderDocHandle );

};

//---------------------------------------------------------------------------

#endif


