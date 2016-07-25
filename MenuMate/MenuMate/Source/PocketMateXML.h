//---------------------------------------------------------------------------

#ifndef PocketMateXMLH
#define PocketMateXMLH
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

#include <system.hpp>
#include <memory>
#include "XMLConst.h"
#include "XMLDoc.hpp"

//---------------------------------------------------------------------------
enum eXMLActionResult {eXMLFailed,eXMLAccepted};

class TXMLBase
{
	protected :
	   AnsiString ID_ELEMENT;
   public :
   TXMLBase(AnsiString FileName);
   ~TXMLBase();
   AnsiString ID;

	_di_IXMLDocument Doc;
	_di_IXMLDocument ResultDoc;

	eXMLActionResult Result;
	AnsiString ResultText;
    AnsiString ContactNumber;
   AnsiString Name;
   bool Cache;

   void Clear();
   void Reset(AnsiString FileName, AnsiString inID = "");
   void SaveToFile();
   void LoadFromFile(AnsiString FileName);
	AnsiString SerializeOut();
	void SerializeIn(AnsiString);
   virtual void Parse() = 0;
};

class TXMLVoucher : public TXMLBase
{
   private:
   	AnsiString fType;
   	AnsiString fVoucherCode;
   	AnsiString fAmountTender;
   	AnsiString fAmountSale;
   	AnsiString fMerchantID;
   	AnsiString fLoyaltyID;
   	AnsiString fPVID;

/*Work suspended to work on some other shit (again),
so Pocket vouchers go however the fPVID (there pocket voucher ref)
which is sent in there PV response needs to go into our reference list and be
searchable. Also reversal of vouchers needs to be tested and stepped though.*/

   public:
      TXMLVoucher(AnsiString TransID);
      void BuildXML();
      void Parse();
      void GetContactNumber(AnsiString &ResultTxt);

   void SetTransactionType(AnsiString Type);
   void SetVoucherCode(AnsiString VoucherCode);
   void SetTenderAmount(AnsiString TenderAmount);
   void SetSaleAmount(AnsiString SaleAmount);
   void SetMerchantID(AnsiString VoucherMerchantID);
   void SetLoyaltyID(AnsiString MembershipNumber);
   AnsiString GetPVTransCode();

};


#endif

