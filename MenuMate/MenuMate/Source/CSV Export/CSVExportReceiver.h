// ---------------------------------------------------------------------------

#ifndef CSVExportReceiverH
#define CSVExportReceiverH

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

enum XMLStatus {xmlResultEr,xmlResultWar,xmlResultO};
//enum WebMsgType {WebMsgUnknown,WebMsgOrder,WebMsgCallAway,WebMsgReceipt,WebMsgPrintJob};


class TCSVExportReceiver
{
private:
   std::auto_ptr<TIdTCPServer>	fTCPServer;
   void __fastcall OnExecute(TIdContext *AContext);
   void __fastcall OnConnect(TIdContext *AContext);
   void __fastcall OnDisconnect(TIdContext *AContext);
public:

   void Initialise(UnicodeString DBIP , UnicodeString DBPath, int inWebPort );

	static TCSVExportReceiver& Instance() {
        static TCSVExportReceiver singleton;
        return singleton;
    }
   UnicodeString CSVDir;

   bool Registered;

   TCSVExportReceiver();
   ~TCSVExportReceiver();
};

#endif
