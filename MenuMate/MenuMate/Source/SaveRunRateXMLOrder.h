//---------------------------------------------------------------------------

#ifndef SaveRunRateXMLOrderH
#define SaveRunRateXMLOrderH

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

class TSaveRunRateXMLOrder : public ISaveOrder
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
		 TSaveRunRateXMLOrder();
		~TSaveRunRateXMLOrder();

		__property bool AllowServingCourseDuplicate =
				{ read = FAllowServingCourseDuplicate, write = _setAllowServingCourseDuplicate };

		__property bool AllowCourseDuplicate =
				{ read = FAllowCourseDuplicate, write = _setAllowCourseDuplicate };

		// Open a Complete Order
		// Order's components must be added afterwards
		// Once all the components SaveOrder() must be called


		// Opens an Incomplete Order
		// Order's components must be added afterwards
		// Once all the components SaveOrder() must be called

		// Opens a Cancel Order
		// The items to be canceled must be added afterwards
		// Once all the items are added SaveOrder() must be called


		// Opens a Credit Order
		// The items to be canceled must be added afterwards
		// Once all the items are added SaveOrder() must be called

		// Open a Replacement Order
		// Order's components must be added afterwards
		// Once all the components SaveOrder() must be called

		// Open a Call Away Order
		// Order's components must be added afterwards
		// Once all the components SaveOrder() must be called


		// Opens a Transfer Order
		// SaveOrder() must be called straight away

		// Adds a ServingCourse to an Order

		// Adds a Course to an Order

		// Adds a Item to an Order

		// Adds a Item to cancel to an Order
		// THIS FUNCTION IS TO BE USED FOR CANCEL ORDERS ONLY
		// If inCaceled is false then inQty is ignored


		// Adds a Item's Print Format to an Order


		// Adds a Item's Option to an Order


		// Adds a Item's Option Print Format to an Order


		// Adds a Item's Side to an Order

		// Adds a Item's Side to cancel to an Order


		// Adds a Option's Print Format to an Order


		// Adds a Side's Option to an Order


		// Adds a Option's Print Format to an Order


		// Saves an Order

	private:
		TiXmlDocument* createXMLOrderDoc();

		void addMainElements();
		void addElement(  TiXmlElement* inParentElem, AnsiString inName, TiXmlElement* &inElem );
		void setNodeAttr( TiXmlElement* inElem, AnsiString inAttrName, AnsiString inAttrValue );

		//.............................................

		//.............................................

		// Adds a Print Format to an Order
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


