//---------------------------------------------------------------------------

#ifndef POS_XMLTransactionH
#define POS_XMLTransactionH
//---------------------------------------------------------------------------
#include "ItemComplete.h"
#include "Payment.h"

class TPOS_XMLTransaction : public TPOS_XMLBase
{
   private:
      void BuildTransaction(TPaymentTransaction &PaymentTransaction,TiXmlElement *Transaction);
      void BuildReceipt(TPaymentTransaction &PaymentTransaction,TiXmlElement *Transaction);
      void BuildDiscount(TPaymentTransaction &PaymentTransaction,TItemMinorComplete *Order,TiXmlElement *Product);
      void BuildMetrics(TPaymentTransaction &PaymentTransaction,TItemComplete *Order,TiXmlElement *Product);
      void BuildPricing(TPaymentTransaction &PaymentTransaction,TItemMinorComplete *Order,TiXmlElement *Product);
      void BuildSource(TPaymentTransaction &PaymentTransaction,TItemComplete *Order,TiXmlElement *Product);
      void BuildPayment(TPaymentTransaction &PaymentTransaction,TPayment *inPayment,TiXmlElement *Receipt);
      void BuildProduct(TPaymentTransaction &PaymentTransaction,TItemComplete *Order,TiXmlElement *Receipt);
      void BuildProductSide(TPaymentTransaction &PaymentTransaction,TItemComplete *ParentOrder,TItemMinorComplete *Order,TiXmlElement *Receipt);
      void BuildMembership(TPaymentTransaction &PaymentTransaction,TiXmlElement *Receipt);      
   public:
      TPOS_XMLTransaction(AnsiString TransID);
      void Build(TPaymentTransaction &PaymentTransaction);
      void Parse();      
};
#endif
