// ---------------------------------------------------------------------------

#ifndef SmartCardAPIH
#define SmartCardAPIH

#include "MM_DBCore.h"
#include "Contact.h"
#include "SystemEvents.h"
#include "ContactPoints.h"
#include "SmartCardDefs.h"
#include "ManagerSyndCode.h"
#include "SmartCardVer2API.h"
#include "SmartCardVer3API.h"
#include "SmartCardVer5API.h"
#include "SmartCardVer6API.h"
#include "SmartCardException.h"
#include "MMMessageBox.h"

/*
Packet Block on card.
1 ) A block Length includes the CRC for corruption.
2 ) Data in brackets ()is encrypted.
3 ) ... Is padding out to Block Length - 2.

.Card Ver Block.
[Card Ver]

.Customer Info Block.
([Customer Data][SYND CRC]...)[Coruption CRC]
.Customer Points.
([Points][SYND CRC]...)[Coruption CRC]

 */

// ---------------------------------------------------------------------------

class TManagerSmartCard
{
private:
   SCARDCONTEXT hCardContext;
   SCARDHANDLE hCard;
   BYTE VerificationCode[3];
   std::map <int, TSyndCode> SyndCodes;

   AnsiString fCardReader;
   UnicodeString LastErrorMessage;
   int LastErrorCode;
   TSmartCardGUID TransactionCardGuid;

   void StartMonitoringThread();
   void StopMonitoringThread();
   bool VerifyFactoryCard();
   bool VerifySyndicateCard();

   void FormatCard();
   LONG CardConnect();
   void CardDisconnect();

   LONG CardReadMainMemory(int BlockStart, int BlockEnd, TMemoryStream &Stream);
   LONG CardReadProtectedMemory(TMemoryStream &Stream);
   LONG CardReadSecurityMemory(TMemoryStream &Stream);

   bool CheckStreamCRC(TMemoryStream *Stream);
   // void Encrypt(TMemoryStream &InData);
   void Decrypt(TMemoryStream &InData, AnsiString SyndicateCode);

   void StreamGetVersion(TSmartCardBlock &CardBlock, TMemoryStream &Stream);
   void LoadCardBlock(TSmartCardBlock &CardBlock);


   std::auto_ptr <TSmartCard> SmartCard;

   bool CardReaderEventsActive;

   void Reset();

public:
   void Initialise(bool inUserEnabled , std::map <int, TSyndCode> inSyndCodes);
   UnicodeString LoadCardGUID();
   bool CheckCorrectCard(TSmartCardGUID &SmartCardGUID);
   bool ContactInfoMatches(TMMContactInfo &inContactInfo);
   void SaveContactInfo(TMMContactInfo &inContactInfo);
   void SaveContactPoints(TContactPoints Points);
   void SaveCardGUID(TSmartCardGUID &CardGuid);
   int GetCardBlockVersion(TSmartCardBlock &CardBlock);

   void GetReaders(TStringList *ReaderList);
   void SetReader(UnicodeString inReaderName);

   bool CardInserted;
   bool CardBlank;
   bool CardOk;
   bool CardUnknown;
   bool CardCorrupt;
   bool CardSyndicate;
   bool CardFactory;
   bool CardUnreadable;

   bool Registered;
   bool Enabled;

   void SmartCardRemoved();
   void SmartCardInserted();
   void FormatCardToNewUser();
   bool FormatCardToFactory();

   void CardReaderEventsOn();
   void CardReaderEventsOff();

   // Card Reader Events
   TSystemEvents OnCardInserted;
   TSystemEvents OnCardRemoved;

   // Card Events.
   TSystemEvents OnNewCardInserted;
   TSystemEvents OnUnknownCardInserted;
   TSystemEvents OnUnableToDecodeCard;
   TSystemEvents OnCardUpdated;
   TSystemEvents OnCardCorrupt;
   TSystemEvents OnCardUnreadable;

   TSmartCardBlock *GetRestorePoint();
   void GetSyndCodeInfo(TSyndCode &SyndCode);
   bool GetSyndCodeValid();
   TSyndCode GetDefaultSyndCode();
   const TSmartCardGUID *GetCardGUID();
   void RestoreToRestorePoint(TSmartCardBlock &RestorePoint);
   void UpdateViewers();



   TManagerSmartCard();
   virtual ~TManagerSmartCard();

   class TMonitoringThread : public TThread
   {
   public:
	  TMonitoringThread(SCARDCONTEXT &hCardContext, UnicodeString infCardReader /* , TCardStatusChangedEvent OnCardStatusChanged */ );

	  void Cancel();
	  // Call if terminating the application while running!
	  UnicodeString ErrorMsg;
   private:
	  SCARDCONTEXT hCardContext;
	  SCARD_READERSTATE fReaderState;
	  UnicodeString fCardReader;

	  void __fastcall Execute();
	  void __fastcall SetStatus();
   };

   TMonitoringThread *fMonitoringThread;

   /* Used be the Calling programs to "lock in a card".
   If the card changes between transaction calls any writes to the card fails.
	*/
   void BeginTransaction();
   void EndTransaction();
   GetContactInfo(TMMContactInfo & ContactInfo);
   // void __fastcall	fMonitoringThreadTerminate(TObject *Sender);

   bool UpgradeCardToVersion(int version, int newVersion, std::map <int, TSyndCode> SyndCodes, bool cardUnlocked = false);
   int GetCurrentCardVersion();
   TSyndCode GetCurrentSyndicateCode();
};

#endif
