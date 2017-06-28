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
//		static TOracleDataBuilder& Instance()
//		{
//			static TOracleDataBuilder singleton;
//			return singleton;
//		}

		TOracleDataBuilder();
		~TOracleDataBuilder();

//		__property TMYOBInvoice* MYOBInvoice = { read = FMYOBInvoice };
//
//        bool BuildMYOBInvoice( TMYOBInvoiceDetail&  MYOBInvoiceDetails);
//        std::vector<TItemComplete *> itemComplete;
//
//	protected:
		TPostRoomInquiry PostRoomInquiry;
        TPostRequest PostRequest;
		void ClearPostRoomInquiry(TPaymentTransaction &paymentTransaction);
//
        void CreatePostRoomInquiry(TPostRoomInquiry &postRoomInquiry);
        void CreatePost(TPaymentTransaction &paymentTransaction, TPostRequest &postRequest);
        void AddInvoiceAttrs(TiXmlElement *rootNode,TPostRequest &postRequest);
        void AddInvoiceAttrs(TiXmlElement *rootNode,TPostRoomInquiry &postRoomInquiry);
        TiXmlDocument CreatePostXML(TPostRequest &postRequest);
        TiXmlDocument CreateRoomInquiryXML(TPostRoomInquiry &postRoomInquiry);
        void SetNodeAttr(TiXmlElement *inNode, AnsiString inAttrName, AnsiString inAttrValue);
        TRoomInquiryResult createXMLInquiryDoc();
//        void createXMLInquiryDoc(TiXmlDocument *result);
        TiXmlElement *_rootElem;
        TiXmlElement *_itemsElem;
        TiXmlElement* loaditemsElem(TiXmlElement* _rootElem);
        void ReadXML(TiXmlDocument *result,TRoomInquiryResult &roomInquiryResult);
        __int32 ChildCount( TiXmlElement* inElem );
        void LoadCustomerDetails(int _index,TRoomInquiryResult &roomInquiryResult);
    private:
        void ExtractDiscount(std::map<int, double> &discMap, TItemComplete *itemComplete);
        void ExtractSubTotal();
        void ExtractTaxes(std::vector<TTax> &taxVector, TItemComplete *itemComplete);
        void ExtractServiceCharge();
};
#endif
