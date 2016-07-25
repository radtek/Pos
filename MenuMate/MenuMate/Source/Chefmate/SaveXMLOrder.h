//---------------------------------------------------------------------------

#ifndef SaveXMLOrderH
#define SaveXMLOrderH

//---------------------------------------------------------------------------

#include <vector>
#include "tinyxml.h"

#include "ChefMateOrderGenerator.h"
#include "ChefmateTools.h"
#include "ISaveOrder.h"

//---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::::::

class ESaveXMLOrder : public Exception
{
public:
	ESaveXMLOrder( UnicodeString inMsg, OERROR inError );

	__property OERROR Error = { read = FError };

protected:
	OERROR FError;
};

//::::::::::::::::::::::::::::::::::::::::::::::::::::::

class ECommitOrderInvalidResponseFormat : public Exception
{
	public:
		ECommitOrderInvalidResponseFormat( UnicodeString inMsg );
};

//:::::::::::::::::::::::::::::::::::::::::::::::::::::

class TSaveXMLOrder : public ISaveOrder
{
protected:
    TiXmlDocument *_xmlOrderDoc;

	TiXmlElement *_rootElem;   // Order's Element
	TiXmlElement *_servingCoursesElem;
	TiXmlElement *_coursesElem;
	TiXmlElement *_itemsElem;

	bool FAllowServingCourseDuplicate;
	bool FAllowCourseDuplicate;

	void _setAllowServingCourseDuplicate( bool inAllow );
	void _setAllowCourseDuplicate(        bool inAllow );

	public:
		 TSaveXMLOrder();
		~TSaveXMLOrder();

		__property bool AllowServingCourseDuplicate =
				{ read = FAllowServingCourseDuplicate, write = _setAllowServingCourseDuplicate };

		__property bool AllowCourseDuplicate =
				{ read = FAllowCourseDuplicate, write = _setAllowCourseDuplicate };

		// Open a Complete Order
		// Order's components must be added afterwards
		// Once all the components SaveOrder() must be called
		HORDER OpenCompleteOrder( __int32    inDBKey,
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
		// Once all the components SaveOrder() must be called
		HORDER OpenIncompleteOrder( __int32    inDBKey,
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
		HORDER OpenCancelOrder( __int32 inDBKey,
								__int32 inOrderNumber,
                                AnsiString inTableTabName );

		// Opens a Credit Order
		// The items to be canceled must be added afterwards
		// Once all the items are added SaveOrder() must be called
		HORDER OpenCreditOrder();

		// Open a Replacement Order
		// Order's components must be added afterwards
		// Once all the components SaveOrder() must be called
		HORDER OpenReplacementOrder( __int32    inDBKey,
									 __int32    inOrderNumber,
									 AnsiString inTableTabName );

		// Open a Call Away Order
		// Order's components must be added afterwards
		// Once all the components SaveOrder() must be called
		HORDER OpenCallAwayOrder( __int32    inOrderNumber,
								  AnsiString inTableTabName );

		// Opens a Transfer Order
		// SaveOrder() must be called straight away
		HORDER OpenTransferOrder( __int32    inDBKey,
								  __int32    inOrderNumber,
								 AnsiString inTableTabName,
                                 AnsiString ServerName,
                                 AnsiString inPatronCount,
								 AnsiString inSaleStartTime,
                                 AnsiString inSourceTableName );

		// Adds a ServingCourse to an Order
		HSCOURSE AddServingCourse( __int32    inDBKey,
								   AnsiString inName,
								   AnsiString inKitchenName,
								   __int32 	  inDisplayOrder );

		// Adds a Course to an Order
		HCOURSE AddCourse( __int32    inDBKey,
						   AnsiString inName,
						   AnsiString inKitchenName,
						   __int32    inServingCourseKey,
						   __int32 	  inDisplayOrder );

		// Adds a Item to an Order
		HITEM AddItem( __int32    inDBKey,
					   __int32    inOrderItemDBKey,
					   AnsiString inName,
					   AnsiString inKitchenName,
					   __int32    inServingCourseKey,
					   __int32    inCourseKey,
					   AnsiString inNote );

		// Adds a Item to cancel to an Order
		// THIS FUNCTION IS TO BE USED FOR CANCEL ORDERS ONLY
		// If inCaceled is false then inQty is ignored
		HITEM AddItemToCancel( __int32    inOrderItemKey,
							   AnsiString inItemName,
							   bool       inCanceled = true,
							   __int32    inQty      = 1 );

		// Adds a Item's Print Format to an Order
		HPFORMAT AddItemPrintingFormat( HITEM   inItemHandle,
										__int32 inDBKey,
										bool	inUnderline,
										bool	inBold,
										TColor	inColor,
										bool	inLargeFont,
										bool	inDoubleWidth,
										bool	inDoubleHeight );

		// Adds a Item's Option to an Order
		HOPTION AddItemOption( HITEM      inItemHandle,
							   __int32    inDBKey,
							   AnsiString inName,
							   AnsiString inKitchenName,
							   bool	      inIsPlus );

		// Adds a Item's Option Print Format to an Order
		HPFORMAT AddItemOptionPrintingFormat( HOPTION inOptionHandle,
											  __int32 inDBKey,
											  bool    inUnderline,
											  bool    inBold,
											  TColor  inColor,
											  bool    inLargeFont,
											  bool    inDoubleWidth,
											  bool    inDoubleHeight );

		// Adds a Item's Side to an Order
		HSIDE AddItemSide( HITEM      inItemHandle,
						   __int32    inOrderSideKey,
						   AnsiString inSideName,
						   AnsiString inKitchenName );

		// Adds a Item's Side to cancel to an Order
		HSIDE AddItemSideToCancel( HITEM      inItemHandle,
								   __int32    inOrderSideKey,
								   AnsiString inSideName );

		// Adds a Option's Print Format to an Order
		HPFORMAT AddSidePrintingFormat( HSIDE      inSideHandle,
										__int32    inDBKey,
										bool	   inUnderline,
										bool	   inBold,
										TColor	   inColor,
										bool	   inLargeFont,
										bool	   inDoubleWidth,
										bool	   inDoubleHeight );

		// Adds a Side's Option to an Order
		HOPTION AddSideOption( HSIDE      inSideHandle,
							   __int32    inDBKey,
							   AnsiString inName,
							   AnsiString inKitchenName,
							   bool	     inIsPlus );

		// Adds a Option's Print Format to an Order
		HPFORMAT AddSideOptionPrintingFormat( HOPTION    inOptionHandle,
											  __int32    inDBKey,
											  bool	  	 inUnderline,
											  bool		 inBold,
											  TColor	 inColor,
											  bool	     inLargeFont,
											  bool		 inDoubleWidth,
											  bool		 inDoubleHeight );

		// Saves an Order
		OERROR CommitOrder( HostnameLIST inHostNameList );

		__int32 GetOrderNumber();

	private:
		TiXmlDocument* createXMLOrderDoc();

		void addMainElements();
		void addElement(  TiXmlElement* inParentElem, AnsiString inName, TiXmlElement* &inElem );
		void setNodeAttr( TiXmlElement* inElem, AnsiString inAttrName, AnsiString inAttrValue );

		//.............................................

		TiXmlElement *servingCourseWithKey( __int32 inDBKey );
		TiXmlElement *courseWithKey(        __int32 inDBKey );

		//.............................................

		// Adds a Print Format to an Order
		HPFORMAT addPrintingFormat( HPARENT inParentHandle,
									__int32 inDBKey,
									bool	inUnderline,
									bool	inBold,
									TColor  inColor,
									bool	inLargeFont,
									bool	inDoubleWidth,
									bool	inDoubleHeight );
		// Adds an Option to an Order
		HOPTION addOption( HPARENT    inParentHandle,
						   __int32    inDBKey,
						   AnsiString inName,
						   AnsiString inKitchenName,
						   bool	      inIsPlus );

		// Sends an Order to ChefMate
		OERROR sendOrderToCheftMate(
							TiXmlDocument* inXMLOrderDoc,
							HostnameLIST   inHostNameList );
		OERROR responseToErrorResult( TiXmlDocument *inXMLResponseDoc );

		AnsiString orderFileName( HORDER inOrderHandle );
		AnsiString xmlDateToStr( TDateTime inDateTime );
};

//---------------------------------------------------------------------------

#endif


