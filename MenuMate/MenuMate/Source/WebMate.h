// ---------------------------------------------------------------------------

#ifndef WebMateH
#define WebMateH

#include "XMLOrdermm.h"
#include "XMLCallAwaymm.h"
#include "XMLReceiptmm.h"
#include "MM_DBCore.h"
#include "SystemEvents.h"
#include "MMLogging.h"
#include "MMContactInfo.h"
#include "ItemComplete.h"
#include "ChitNumber.h"
//

#include "IdBaseComponent.hpp"
#include "IdComponent.hpp"
#include "IdContext.hpp"
#include "IdCustomTCPServer.hpp"
#include "IdTCPServer.hpp"

#include <queue>

// ---------------------------------------------------------------------------
#define WEBORDER_RECEIVED 0x01
#define WEBORDER_REJECTED 0x01
#define WEBORDER_STATUSCHANGED (WM_APP + 458)

enum XMLStatusCodes {xmlResultErr,xmlResultWarn,xmlResultOK};
enum WebMsgType {WebMsgUnknown,WebMsgOrder,WebMsgCallAway,WebMsgReceipt,WebMsgPrintJob};


class TWebMate
{
private:
   int LastErrorCode;
   std::auto_ptr<TIdTCPServer>	fTCPServer;
   void __fastcall OnExecute(TIdContext *AContext);
   void __fastcall OnConnect(TIdContext *AContext);
   void __fastcall OnDisconnect(TIdContext *AContext);
   UnicodeString DBIP;
   UnicodeString DBPath;
	XMLStatusCodes __fastcall ProcessWebOrders(Database::TDBControl &DBControl,_di_IXMLORDERType &WebOrder,AnsiString &ErrMsg);
   __fastcall ProcessMembership(Database::TDBTransaction &DBTransaction, TMMContactInfo &Member);
	Currency __fastcall ProcessPrePay(_di_IXMLORDERType &WebOrder);
	AnsiString __fastcall BuildXMLWebOrdersResponce(UnicodeString GUID,XMLStatusCodes Status,AnsiString ErrMsg);
	__fastcall AddProduct(Database::TDBTransaction &DBTransaction,int WebKey, UnicodeString WebGUID,
   									_di_IXMLACCOUNTType Account,
   									TList *OrderList, _di_IXMLPRODUCTType Product,TMMContactInfo Member, int SetMenuGroup = 0);

	__fastcall AddSides(Database::TDBTransaction &DBTransaction,
												_di_IXMLACCOUNTType Account,TItemComplete *MasterOrder,
												_di_IXMLPRODUCTType Product,TMMContactInfo Member);

    void __fastcall SetPrice(_di_IXMLPRODUCTType Product,TItemMinorComplete *Order );

    __fastcall ApplyMembership(Database::TDBTransaction &DBTransaction,TMMContactInfo Member,TItemComplete *Order);

	WebMsgType GetMsgType(AnsiString Data);
	void ProcessWebMsgOrder(TIdContext *AContext, UnicodeString FileName);
	void ProcessWebMsgCallAway(TIdContext *AContext, UnicodeString FileName);
  	void ProcessWebMsgReceipt(TIdContext *AContext, UnicodeString FileName);
	void ProcessWebMsgPrintJob(TIdContext *AContext, UnicodeString FileName);

    TChitNumber ChitNumber;

public:

   void Initialise(bool inEnabled, UnicodeString inBasePath, UnicodeString DBIP , UnicodeString DBPath, int inWebPort );

	static TWebMate& Instance() {
        static TWebMate singleton;
        return singleton;
    }
   UnicodeString BasePath;
   UnicodeString WebDir;
   UnicodeString CacheDir;
   UnicodeString FailedDir;
   UnicodeString ProcessedDir;

   bool Registered;
   bool Enabled;

   TWebMate();
   ~TWebMate();
};

#endif
