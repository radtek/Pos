// ---------------------------------------------------------------------------

#ifndef KitchenModH
#define KitchenModH

#include "SimpleTCP.h"
#include <Grids.hpp>
#include <ExtCtrls.hpp>

#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdUDPBase.hpp>
#include <IdUDPClient.hpp>
#include <IdUDPServer.hpp>
#include <IdGlobal.hpp>

#include <ComCtrls.hpp>
#include <Dialogs.hpp>

// ---------------------------------------------------------------------------
#include "NetMessageChefMate.h"
#include "MM_DBCore.h"
#define CalculateHeight 0
#define DoDrawOrders 1

class TTill
{
public:
   TColor TillColour;
   TColor FontColour;
   UnicodeString TillName;
   UnicodeString UserName;

   __fastcall TTill();
   // void DeleteRequest(TNetMessageChefMate *Request);
};

class TListChefDisplay : public TList
{
public:
   void Assign(TListChefDisplay *inListChefDisplay);
   void ChefDisplayClear();
   void ChefDisplaySort();

   void ChefDisplayDelete(TNetMessageChefMate *in);
   int ChefDisplayAdd(TNetMessageChefMate *in);

   TNetMessageChefMate *ChefDisplayGet(int Index);
   TNetMessageChefMate *GetByOrderNumber(int OrderNumber, UnicodeString Device);
   TNetMessageChefMate *GetByTimeKey(int TimeKey);
   TNetMessageChefMate *GetChitNumber(UnicodeString ChitNumber);
   TNetMessageChefMate *GetByChitNumber(UnicodeString ChitNumber);
   __fastcall virtual ~TListChefDisplay();
};

class TDisplayOrderSet
{
   std::auto_ptr <TStringList> InternalDisplay;
   std::auto_ptr <TListChefDisplay> OrdersList;

   int GetMaxLineLength(TStringList * Data);

public:
   char NewOrderChar;
   UnicodeString FinalOrderTag;
   UnicodeString CurrentOrderTag;

   std::vector <TRichEdit*> Displays;

   int MaxDisplayLines;
   TDateTime WarningTime;

   void Draw();

   void Add(TNetMessageChefMate *Request);
   void Delete(TNetMessageChefMate *Request);
   void Update();
   int Size();
   TNetMessageChefMate *Get(int Index);
   TNetMessageChefMate *FindByOrderAndDevice(int OrderNumber, UnicodeString Device);
   TNetMessageChefMate *FindByLineNumber(int LineNumber);
   TNetMessageChefMate *FindByTimeKey(int TimeKey);
   TNetMessageChefMate *FindByChitNumber(UnicodeString ChitNumber);

   void RegisterDisplays(std::vector <TRichEdit *> &inDisplays, int MaxHeight);

   TDisplayOrderSet();
   TDisplayOrderSet(const TDisplayOrderSet &inDisplayOrderSet);
};

enum TDisplayType
{
   edtCurrent, edtAged
};

typedef std::map <TDisplayType, TDisplayOrderSet*> TDisplaySet;

class TKitchenMod : public TObject
{
private:
   TIdUDPServer *udpServer;
   TIdUDPClient *udpClient;

   TDisplaySet Displays;

   void __fastcall udpServerUDPRead(TIdUDPListenerThread *AThread, TBytes AData, TIdSocketHandle *ABinding);

   // Called By UDP packet Needs to be added to terminal and Display updated.
   void SetRequest(TNetMessageChefMate *Request);
   Database::TDBControl &DBControl;
   TDateTime GetOrderDuration(Database::TDBTransaction &DBTransaction, int TimeKey);
   bool GetOrderClosed(Database::TDBTransaction &DBTransaction, int TimeKey);

public:
   void SendRequest(TNetMessageChefMate *Request, UnicodeString DestinationIP);
   __fastcall TKitchenMod(Database::TDBControl &inDBControl);
   virtual __fastcall ~TKitchenMod();

   bool Enabled;
   bool AutoCloseWhenBumped;
   bool RemovedWhenScanned;
   bool RemovedWhenClosed;
   bool DisplayDynamicOrders;

   void RegisterDisplays(TDisplayType Type, std::vector <TRichEdit *> &inDisplays, int MaxHeight);
   void Initialise();
   void SetFontSize(TFontDialog *FontDialog);
   void UpdateOrderDurations(Database::TDBTransaction &DBTransaction);

   TDateTime WarningTime;

   TNetMessageChefMate * FindByLineNumber(TDisplayType Type, int LineNumber);
   TNetMessageChefMate * FindByOrderAndDevice(TDisplayType Type, int OrderNumber, UnicodeString Device);
   TNetMessageChefMate * FindByTimeKey(TDisplayType Type, int TimeKey);
   TNetMessageChefMate * FindByChitNumber(TDisplayType Type, UnicodeString ChitNumber);
   void Add(TDisplayType Type, TNetMessageChefMate *Request);
   void Delete( /* TDisplayType Type, */ TNetMessageChefMate *Request);
   void Update(TDisplayType Type);
   void SetMinutesWarning(TDateTime inWarningTime);
   void RemoveClosedOrders(Database::TDBTransaction &DBTransaction);
};

// ---------------------------------------------------------------------------
#endif
