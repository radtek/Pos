//---------------------------------------------------------------------------

#ifndef OracleDataBuilderH
#define OracleDataBuilderH
#include "PaymentTransaction.h"
#include "DeviceRealTerminal.h"
#include "OracleDataObjects.h"
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
        TPostRequest CreatePost(TPaymentTransaction &paymentTransaction,double portion,int paymentIndex, double tip);
        TLinkDescription CreateLinkDescription();
        TRoomInquiryResult createXMLInquiryDoc();
        TiXmlDocument CreatePostXML(TPostRequest &postRequest);
        TiXmlDocument CreateRoomInquiryXML(TPostRoomInquiry &postRoomInquiry);
        TiXmlDocument CreateLinkDescriptionXML(TLinkDescription linkDescription);
        AnsiString SerializeOut(TiXmlDocument doc);
        bool DeserializeGetLinkStatus(AnsiString inData);
        bool DeserializeInquiryData(AnsiString inData, TRoomInquiryResult &_roomResult);
        bool DeserializeData(AnsiString inData, TPostRequestAnswer &_postResult);

    private:
        TiXmlElement *_rootElem;
        TiXmlElement *_itemsElem;
        __int32 ChildCount( TiXmlElement* inElem );
        TiXmlElement* loaditemsElem(TiXmlElement* _rootElem);
        void SetNodeAttr(TiXmlElement *inNode, AnsiString inAttrName, AnsiString inAttrValue);
        void LoadCustomerDetails(int _index,TRoomInquiryResult &roomInquiryResult);
        void ReadXML(TiXmlDocument *result,TRoomInquiryResult &roomInquiryResult);
        void ReadXML(TiXmlDocument *result,TPostRequestAnswer &_postResult);
        void AddInvoiceAttrs(TiXmlElement *rootNode,TLinkDescription linkDescription);
        void AddInvoiceAttrs(TiXmlElement *rootNode,TPostRequest &postRequest);
        void AddInvoiceAttrs(TiXmlElement *rootNode,TPostRoomInquiry &postRoomInquiry);
        void ExtractDiscount(std::map<int, double> &discMap, TItemComplete *itemComplete,double portion);
        void ExtractSubTotal(std::map<int,double> &subtotals, std::map<int, double> &discMap,
                             std::map<int,double> &taxMap, std::map<int, double> &serviceChargeMap,
                             TItemComplete *itemComplete,double portion);
        void ExtractTaxes(TItemComplete *itemComplete,std::vector<TTax> &taxVector,double portion);
        void ExtractServiceCharges(std::vector<TServiceCharge> serviceChargeVector, TItemComplete *itemComplete);
        double CalculateTotal(std::map<int,double> subtotals,std::map<int, double> discMap,
                              std::map<int,double> &taxMap, std::map<int, double> &serviceChargeMap );
        void ExtractServiceCharge();
        TiXmlDocument PrepareXMLDocument();
        void GetPaymentAttributes(Database::TDBTransaction &DBTransaction,int PaymentKey,TPayment &Payment);
        std::vector<TPayment> PaymentLoad(Database::TDBTransaction &DBTransaction);
        double SUrcharge;
};
#endif
