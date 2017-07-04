//---------------------------------------------------------------------------

#ifndef OracleDataBuilderH
#define OracleDataBuilderH
#include "PaymentTransaction.h"
#include "DeviceRealTerminal.h"
#include "OracleDataObjects.h"
#include "ManagerPMSCodes.h"
#include "OracleManagerDB.h"
#include <vcl.h>
#include <iostream.h>

#include "tinyxml.h"
//---------------------------------------------------------------------------

class TOracleDataBuilder
{
	public:

		TOracleDataBuilder();
		~TOracleDataBuilder();

		TPostRoomInquiry PostRoomInquiry;
        TPostRequest PostRequest;
		void ClearPostRoomInquiry(TPaymentTransaction &paymentTransaction);
        void CreatePostRoomInquiry(TPostRoomInquiry &postRoomInquiry);
        void CreatePost(TPaymentTransaction &paymentTransaction, TPostRequest &postRequest);
        TiXmlDocument CreatePostXML(TPostRequest &postRequest);
        TiXmlDocument CreateRoomInquiryXML(TPostRoomInquiry &postRoomInquiry);
        TRoomInquiryResult createXMLInquiryDoc();
        TLinkDescription CreateLinkDescription();

    private:
        TiXmlElement *_rootElem;
        TiXmlElement *_itemsElem;
        __int32 ChildCount( TiXmlElement* inElem );
        TiXmlElement* loaditemsElem(TiXmlElement* _rootElem);
        void SetNodeAttr(TiXmlElement *inNode, AnsiString inAttrName, AnsiString inAttrValue);
        void LoadCustomerDetails(int _index,TRoomInquiryResult &roomInquiryResult);
        void ReadXML(TiXmlDocument *result,TRoomInquiryResult &roomInquiryResult);
        void AddInvoiceAttrs(TiXmlElement *rootNode,TPostRequest &postRequest);
        void AddInvoiceAttrs(TiXmlElement *rootNode,TPostRoomInquiry &postRoomInquiry);
        void ExtractDiscount(std::map<int, double> &discMap, TItemComplete *itemComplete);
        void ExtractSubTotal();
        void ExtractTaxes(TItemComplete *itemComplete,std::vector<TTax> &taxVector);
        void ExtractServiceCharge();
};
#endif
