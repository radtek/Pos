//---------------------------------------------------------------------------

#ifndef XMLOrderManagerH
#define XMLOrderManagerH

//---------------------------------------------------------------------------

#include "vector.h"

#include "SaveXMLOrderBuilder.h"
#include "SaveXMLOrder.h"

//---------------------------------------------------------------------------

class TXMLOrderManager
{
	public:
		 TXMLOrderManager();
		~TXMLOrderManager();
		// Open a Complete Order
		// Order's components must be added afterwards
		// Once all the components SaveOrder() must be called
		HORDERDOC OpenCompleteOrder(  __int32    inDBKey,
									  AnsiString inServerName,
									  __int32    inOrderNumber,
									  AnsiString inChitValue,
									  AnsiString inTableTabName,
									  AnsiString inOrderType,
									  AnsiString inCustomerName,
									  AnsiString inPartyName );

		// Opens an Incomplete Order
		// Order's components must be added afterwards
		// Once all the components SaveOrder() must be called
		HORDERDOC OpenIncompleteOrder(  __int32    inDBKey,
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
		HORDERDOC OpenCancelOrder( __int32 inDBKey,
								   __int32 inOrderNumber );

		// Opens a Credit Order
		// The items to be canceled must be added afterwards
		// Once all the items are added SaveOrder() must be called
		HORDERDOC OpenCreditOrder();

		// Opens a Transfer Order
		// SaveOrder() must be called straight away
		HORDERDOC OpenTransferOrder( __int32    inDBKey,
									 __int32    inOrderNumber,
								  	 AnsiString inTableTabName );

		// Adds a ServingCourse to an Order
		HSCOURSE AddServingCourse( HORDERDOC  inOrderDocHandle,
								   __int32    inDBKey,
								   AnsiString inName,
								   AnsiString inKitchenName	);

		// Adds a Course to an Order
		HCOURSE AddCourse( HORDERDOC  inOrderDocHandle,
						   __int32    inDBKey,
						   AnsiString inName,
						   AnsiString inKitchenName,
						   __int32    inServingCourseKey );

		// Adds a Item to an Order
		HITEM AddItem( HORDERDOC  inOrderDocHandle,
					   __int32    inDBKey,
					   __int32    inOrderItemDBKey,
					   AnsiString inName,
					   AnsiString inKitchenName,
					   __int32    inServingCourseKey,
					   __int32    inCourseKey,
					   bool       inCanceled );

		// Adds a Item to cancel to an Order
		HITEM AddItemToCancel( HORDERDOC   inOrderDocHandle,
							   TCMHostname inCMHostname,
							   __int32     inOrderSideKey,
							   AnsiString  inSideName,
							   bool        inCanceled = false,
							   __int32     inQty      = 1 );

		// Adds a Item's Print Format to an Order
		HPFORMAT AddItemPrintingFormat( HORDERDOC inOrderDocHandle,
										HITEM     inItemHandle,
										__int32   inDBKey,
										bool	  inUnderline,
										bool	  inBold,
										TColor	  inColor,
										bool	  inLargeFont,
										bool	  inDoubleWidth,
										bool	  inDoubleHeight );

		// Adds a Item's Option to an Order
		HOPTION AddItemOption( HORDERDOC  inOrderDocHandle,
							   HITEM      inItemHandle,
							   __int32    inDBKey,
							   AnsiString inName,
							   AnsiString inKitchenName,
							   bool	      inIsPlus );

		// Adds a Item's Option Print Format to an Order
		HPFORMAT AddItemOptionPrintingFormat( HORDERDOC inOrderDocHandle,
											  HOPTION   inOptionHandle,
											  __int32   inDBKey,
											  bool	    inUnderline,
											  bool	    inBold,
											  TColor  	inColor,
											  bool	    inLargeFont,
											  bool	    inDoubleWidth,
											  bool	    inDoubleHeight );

		// Adds a Item's Side to an Order
		HSIDE AddItemSide( HORDERDOC  inOrderDocHandle,
						   HITEM      inItemHandle,
						   __int32    inDBKey,
						   AnsiString inName,
						   AnsiString inKitchenName );

		// Adds a Option's Print Format to an Order
		HPFORMAT AddSidePrintingFormat( HORDERDOC  inOrderDocHandle,
										HSIDE      inSideHandle,
										__int32    inDBKey,
										bool	   inUnderline,
										bool	   inBold,
										TColor	   inColor,
										bool	   inLargeFont,
										bool	   inDoubleWidth,
										bool	   inDoubleHeight );

		// Adds a Side's Option to an Order
		HOPTION AddSideOption( HORDERDOC  inOrderDocHandle,
							   HSIDE      inSideHandle,
							   __int32    inDBKey,
							   AnsiString inName,
							   AnsiString inKitchenName,
							   bool	      inIsPlus );

		// Adds a Option's Print Format to an Order
		HPFORMAT AddSideOptionPrintingFormat( HORDERDOC  inOrderDocHandle,
											  HOPTION    inOptionHandle,
											  __int32    inDBKey,
											  bool	  	 inUnderline,
											  bool		 inBold,
											  TColor	 inColor,
											  bool	     inLargeFont,
											  bool		 inDoubleWidth,
											  bool		 inDoubleHeight );

		// Commits an Order
		OERROR CommitOrder(
				HORDERDOC    inOrderDocHandle,
				HostnameLIST inHostnameList );

		// Deletes an Order
		OERROR DeleteOrder( HORDERDOC inOrderDocHandle );

		OERROR GetLastOError();

	private:
		TSaveXMLOrderBuilder _saveXMLOrderBuilder;

		// List of Open XML Orders
		std::vector <TSaveXMLOrder> _xmlOrderDocs;

		OERROR _lastOError;

		//::::::::::::::::::::::::::::::::::::::::::::::::::::

		bool           orderOpen( __int32 inOrderNumber );
		TSaveXMLOrder* createXMLOrderDoc();
		HORDERDOC      addOrderXMLDoc( TSaveXMLOrder* inSaveXMLOrderDoc );
		HORDERDOC      findOrder( __int32 inOrderNumber );
		TSaveXMLOrder* getXMLOrderDoc( HORDERDOC inOrderDocHandle );
		OERROR         deleteOrderDoc( HORDERDOC inOrderDocHandle );

};

//---------------------------------------------------------------------------

#endif


