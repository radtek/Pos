//---------------------------------------------------------------------------

#ifndef MMCustomerDisplayAdapterH
#define MMCustomerDisplayAdapterH

//---------------------------------------------------------------------------

#include <vcl.h>
#include "tinyxml.h"
#include "SeatOrders.h"

//---------------------------------------------------------------------------

 class TiXmlDocument;
 class TiXmlElement;
//:::::::::::::::::::::::::::
// TMMCustomerDisplayAdapter
//:::::::::::::::::::::::::::
class TMMCustomerDisplayAdapter
{
    public:

         TMMCustomerDisplayAdapter();
        ~TMMCustomerDisplayAdapter();
        TiXmlDocument* CreateOrderXML(
                           std::vector <TSeatOrders*> inPOSOrder,
                           TCustomerDisp  inChangeStr);

        TiXmlDocument* CreateOrderPaidXML(
                           Currency inLastTotal,
                           Currency inLastChange,
                           Currency inTotalServiceCharge,
                           Currency inTotalTax );

        UnicodeString isSameAddedItem;

    protected:
        TiXmlDocument *xml_order_doc;

        TiXmlElement *root_elem;
        TiXmlElement *scourses_elem;
        TiXmlElement *currencysym_elem;
        TiXmlElement *totals_elem;
          TiXmlElement *loyalty_elem;
        Currency total_discount;
        Currency total;
        Currency total_change;
        Currency total_scharge;
        Currency total_tax;

        TiXmlDocument* createXMLMenuDoc();

        UnicodeString createDefaultOrderHandle();
        UnicodeString createOrderHandle( std::vector <TSeatOrders*> inPOSOrder );

        void addMainElements();

        void populateOrderXmlDoc(
                 std::vector <TSeatOrders*> inPOSOrder,
                 TCustomerDisp  inChangeStr );

        void populateOrderPaidXmlDoc(
                 Currency inLastTotal,
                 Currency inLastChange,
                 Currency inTotalServiceCharge,
                 Currency inTotalTax );

        UnicodeString currencyRoundStr( Currency inValue );
             Currency currencyRound(    Currency inValue );

        TiXmlElement* getSCourseElement( TItemComplete* inOrderItem );

        void addItemElement( TiXmlElement*  inSCourseElem, TItemComplete* inOrderItem );

        TiXmlElement* findItemElement( UnicodeString inItemName, TiXmlElement *itemsElem );
		UnicodeString getItemDisplayName( TItemComplete* inOrderItem, Currency inQty );
		UnicodeString updateItemDisplayNameNMIFormat( TItemComplete* inOrderItem, Currency inQty );
        UnicodeString updateItemDisplayName( Currency inQty, UnicodeString inItemName );
        UnicodeString getItemName( TItemComplete* inOrderItem );

        void addOptions( TiXmlElement*  inItemElem, TItemComplete* inOrderItem );
        void addSides( TiXmlElement*  inItemElem, TItemComplete* inOrderItem );

        void addElement( TiXmlElement* inParentElem, AnsiString inName, TiXmlElement* &inElem );

        void setNodeAttr( TiXmlElement* inElem, AnsiString inAttrName, AnsiString inAttrValue );

        TiXmlElement* findChildElementByAttr(
                         TiXmlElement* inParentElem,
                         AnsiString inChildAttrName,
                         AnsiString inChildAttrValue );

		TiXmlElement* findChildElement( TiXmlElement* inParentElem, AnsiString inChildElementName );
        TiXmlElement *textFontSize;
	private:

		UnicodeString getCurrencySymbol();
        UnicodeString getCustomerFontSize();


};

//---------------------------------------------------------------------------

#endif
