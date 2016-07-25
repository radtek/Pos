//---------------------------------------------------------------------------
#ifndef RequestsH
#define RequestsH

#include "Membership.h"

#include <ComCtrls.hpp>
#include <CPort.hpp>
#include <set>
#include <map>
#include <memory>
#include <list>

#include "enum.h"
#include "MMLogging.h"
#include "Contact.h"


#include "ManagerNet.h"
#include "NetMessageMsg.h"
#include "NetMessageHappyHour.h"
#include "NetMessageInfoSync.h"
#include "NetMessageSeatName.h"

#include "KitchenMod.h"
#include "MM_DBCore.h"
#include "Modules.h"
#include "ManagerStock.h"
#include "MMSettings.h"
#include "DevicePC.h"
#include "Request.h"
#include "SecurityReference.h"

//---------------------------------------------------------------------------
class TAccess;
class TReqLocationCheck;
class TReqPrintJob;
class TItemComplete;
class TListCategories;
class TListItems;
class TItem;
class TItemSize;
class TItemRecipe;
class TListPaymentSystem;



//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

class TReqLogin : public TRequest
{
   public :
   TReqLogin(TDeviceImage *Requester);

   int UserID;
   UnicodeString UsersPIN;

   EProcessingErrorType Error;
   UnicodeString ErrorMsg;
   void virtual udpEncode(TMemoryStream *Stream){}; // Writes it'self to Stream Supplied.
   void virtual udpDecode(TMemoryStream *Stream){};

};

class TMenus : public TObject
{
public:
	long MenuLK;
};





#endif


