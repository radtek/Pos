//---------------------------------------------------------------------------

#ifndef OracleDataBuilderH
#define OracleDataBuilderH
#include "OracleDataIntegration.h"
#include "ManagerPMSCodes.h"
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
        bool CreatePostRoomInquiry(TPaymentTransaction &paymentTransaction);
        bool CreatePost(TPaymentTransaction &paymentTransaction);
        void AddInvoiceAttrs(TiXmlElement *rootNode);
        void CreateXML();
        void SetNodeAttr(TiXmlElement *inNode, AnsiString inAttrName, AnsiString inAttrValue);
        //void AddItemToMYOBInvoice( TMYOBInvoice* inMYOBInvoice, TMYOBCategoryDetail& PayTypeDetail );
//		AnsiString CalcAsString(AnsiString inPrice, AnsiString inQty, AnsiString &inTax );
};
#endif
