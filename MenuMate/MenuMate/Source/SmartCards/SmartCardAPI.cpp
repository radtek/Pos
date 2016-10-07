// ---------------------------------------------------------------------------

#pragma hdrstop
// ---------------------------------------------------------------------------

#pragma hdrstop

#include "cascerr.h"
#include "casmcard.h"
#include "Stream.h"

#include "SmartCardAPI.h"

#include "MMLogging.h"
#include <Forms.hpp>
#include "CrcTable.h"   // CRC Message checking.
#include "blowfish.h"
#include "DBContacts.h"

// ---------------------------------------------------------------------------

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef PalmMate
#include "Palm.h"
#endif

#pragma package(smart_init)

TManagerSmartCard::TManagerSmartCard() : SmartCard(NULL)
{
   TransactionCardGuid.Clear();
   hCardContext = NULL;
   hCard = NULL;
   fMonitoringThread = NULL;
   VerificationCode[0] = 'C';
   VerificationCode[1] = 'f';
   VerificationCode[2] = 'C';
   Enabled = false;
   CardReaderEventsActive = true;
   SyndCodes.clear();
   Reset();
}

TManagerSmartCard::~TManagerSmartCard()
{
   if (hCardContext)
	  CasReleaseContext(hCardContext);
   StopMonitoringThread();
}

void TManagerSmartCard::Initialise(bool inUserEnabled, std::map <int, TSyndCode> inSyndCodes)
{
   try
   {
      SyndCodes.clear();
      SyndCodes = inSyndCodes;
	  Enabled = false;
	  if (inUserEnabled)
	  {
		 // CardReader	= TManagerVariable::Instance().GetStr(DBTransaction,vmSmartCardReader,"");
		 LONG lReturn = SCARD_S_SUCCESS;
		 lReturn = CasEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &hCardContext);

		 if (SyndCodes.size() == 0)
		 {
			throw TSCException(tsceConnectionFailed, "Smart Cards Disabled. No Syndicate Code.");
		 }

		 if (fCardReader == "")
		 {
			std::auto_ptr <TStringList> ReaderList(new TStringList);
			GetReaders(ReaderList.get());
			if (ReaderList->Count > 0)
			{
				SetReader(ReaderList->Strings[0]);
			}
		 }

		 if (fCardReader != "")
		 {
			// Establish a smart card resource context.
			if (lReturn == SCARD_S_SUCCESS)
			{
				TManagerLogs::Instance().Add(__FUNC__, SMARTCARDLOG, "Smart Card Connection Established.");
				StartMonitoringThread();
			}
			else
			{
				hCardContext = NULL;
				throw TSCException(tsceConnectionFailed, "Smart Card Reader Connection Failed.");
			}
			// Get the Readers and if theres no MenuMate Reader then Introduce it.
		 }
		 Enabled = true;
	  }
	}
	catch(Exception & E)
	{
	  Enabled = false;
	  StopMonitoringThread();
	  TManagerLogs::Instance().Add(__FUNC__, SMARTCARDLOG, "Smart Card Reader Initialise Failed.");
	  TManagerLogs::Instance().Add(__FUNC__, SMARTCARDLOG, E.Message);
   }
}

TSyndCode TManagerSmartCard::GetDefaultSyndCode()
{
   TSyndCode Default;
   std::map <int, TSyndCode> ::iterator ptrSyndCodes = SyndCodes.begin();
   for (ptrSyndCodes = SyndCodes.begin();ptrSyndCodes != SyndCodes.end(); advance(ptrSyndCodes,1))
   {
      if(ptrSyndCodes->second.DefaultEncryptCode)
      {
         Default = ptrSyndCodes->second;
      }
   }
   return Default;
}

void TManagerSmartCard::StartMonitoringThread()
{
   StopMonitoringThread();

   fMonitoringThread = new TManagerSmartCard::TMonitoringThread(hCardContext, fCardReader);
   fMonitoringThread->FreeOnTerminate = true;
   fMonitoringThread->Priority = tpLowest;
   fMonitoringThread->Resume();
}

void TManagerSmartCard::StopMonitoringThread()
{
   try
   {
	  if (fMonitoringThread)
	  {
		 fMonitoringThread->Terminate();
		 ::Sleep(100);
		 fMonitoringThread = NULL;
	  }
   }
   catch(Exception & E)
   {
	  Enabled = false;
	  TManagerLogs::Instance().Add(__FUNC__, SMARTCARDLOG, "Smart Card Reader Thread Stop Failed.");
	  TManagerLogs::Instance().Add(__FUNC__, SMARTCARDLOG, E.Message);
   }
}

// ---------------------------------------------------------------------------
void TManagerSmartCard::FormatCardToNewUser()
{
   try
   {
	  if (CardInserted == false)
	  {
		 throw TSCException(tsceNoCard, "No Card in Reader");
	  }

	  LONG lReturn = SCARD_S_SUCCESS;
	  CasBeginTransaction(hCard);
	  BYTE PSC[3] =
	  {
		 0xFF, 0xFF, 0xFF
	  };
	  lReturn = SLE4442_Update_Security_Memory(hCard, PSC, 0x01, sizeof(PSC));
	  CasEndTransaction(hCard, SCARD_LEAVE_CARD);
	  // if(lReturn != SCARD_S_SUCCESS && lReturn != SCARD_M_CHECK_ERROR)
	  if (lReturn != SCARD_S_SUCCESS)
	  {
		 TManagerLogs::Instance().Add(__FUNC__, SMARTCARDLOG, "Format Card To Blank Failed. : " + UnicodeString(IntToHex(int(lReturn), 2)));
	  }
	  else
	  {
		 CardBlank = true;
         TSmartCardBlock CardBlockData;
		 CardBlockData.ClearData();
		 CardBlockData.Version = SMART_CARD_CURRENT_VERSION;
		 SmartCard.reset(new SMART_CARD_CURRENT_VERSION_CLASS(hCard, CardBlockData, GetDefaultSyndCode()));

		 std::auto_ptr <TMemoryStream> Stream(new TMemoryStream);

		 int BlockLength = CARD_TOTAL_MEMORY_LENGTH - V2_CARD_GUID_START - V2_CARD_GUID_LENGTH;
		 std::vector <BYTE> recvbuffer(BlockLength, 0xFF);
		 Stream->Write(&recvbuffer[0], recvbuffer.size());
		 CasBeginTransaction(hCard);
		 lReturn = SLE4442_Update_Main_MemoryA(hCard, (unsigned char*)Stream->Memory, V2_CARD_GUID_START + V2_CARD_GUID_LENGTH,
			Stream->Size);
		 CasEndTransaction(hCard, SCARD_LEAVE_CARD);
		 if (lReturn != SCARD_S_SUCCESS && lReturn != SCARD_M_CHECK_ERROR)
		 {
			TManagerLogs::Instance().Add(__FUNC__, SMARTCARDLOG, "Format Card To Blank Failed. : " + UnicodeString(IntToHex(int(lReturn), 2)));
		 }

	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, SMARTCARDLOG, E.Message);
	  throw;
   }
}

// ---------------------------------------------------------------------------
bool TManagerSmartCard::FormatCardToFactory()
{
   try
   {
	  if (CardInserted == false)
	  {
		 throw TSCException(tsceNoCard, "No Card in Reader");
	  }

	  LONG lReturn = SCARD_S_SUCCESS;
	  CasBeginTransaction(hCard);
	  BYTE PSC[3] =
	  {
		 0xFF, 0xFF, 0xFF
	  };
	  lReturn = SLE4442_Update_Security_Memory(hCard, PSC, 0x01, sizeof(PSC));
	  if (lReturn != SCARD_S_SUCCESS)
	  {
		 TManagerLogs::Instance().Add(__FUNC__, SMARTCARDLOG,
			"Format Card To Blank Failed. Unable to reset Security Bytes: " + UnicodeString(IntToHex(int(lReturn), 2)));
     	return false;
	  }

	  CasEndTransaction(hCard, SCARD_LEAVE_CARD);

	  if (lReturn == SCARD_S_SUCCESS)
	  {
		 CardBlank = true;
         TSmartCardBlock CardBlockData;
		 CardBlockData.ClearAll();
		 CardBlockData.Version = SMART_CARD_CURRENT_VERSION;
		 SmartCard.reset(new SMART_CARD_CURRENT_VERSION_CLASS(hCard, CardBlockData, GetDefaultSyndCode()));

		 std::auto_ptr <TMemoryStream> Stream(new TMemoryStream);

		 int BlockLength = CARD_TOTAL_MEMORY_LENGTH - V2_CARD_GUID_START;
		 std::vector <BYTE> recvbuffer(BlockLength, 0xFF);

		 Stream->Write(&recvbuffer[0], recvbuffer.size());
		 CasBeginTransaction(hCard);
		 lReturn = SLE4442_Update_Main_MemoryA(hCard, (unsigned char*)Stream->Memory, V2_CARD_GUID_START, Stream->Size);
		 CasEndTransaction(hCard, SCARD_LEAVE_CARD);
		 if (lReturn != SCARD_S_SUCCESS && lReturn != SCARD_M_CHECK_ERROR)
		 {
			TManagerLogs::Instance().Add(__FUNC__, SMARTCARDLOG, "Format Card To Blank Failed. : " + UnicodeString(IntToHex(int(lReturn), 2)));
         return false;
		 }

	 return true;
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, SMARTCARDLOG, E.Message);
	  throw;
   }
}

void TManagerSmartCard::GetReaders(TStringList *ReaderList)
{
   try
   {
	  LPGUID szProvider;
	  LONG lReturn = SCARD_S_SUCCESS;

	  DWORD ReadersSize = 0;
	  lReturn = CasListReaders(hCardContext, NULL, NULL, &ReadersSize);
	  if (lReturn != SCARD_S_SUCCESS && lReturn != SCARD_M_CHECK_ERROR)
	  {
		 throw TSCException(tsceNoReader, "Retriving Reader Failed. Error Code : " + UnicodeString(IntToHex(int(lReturn), 2)));
	  }

	  LPTSTR mszReaders = new char[ReadersSize];
	  lReturn = CasListReaders(hCardContext, NULL, mszReaders, &ReadersSize);
	  if (lReturn == SCARD_S_SUCCESS)
	  {
		 TManagerLogs::Instance().Add(__FUNC__, SMARTCARDLOG, "Reader(s) found : " + UnicodeString(mszReaders));
		 ReaderList->Clear();
		 ReaderList->Add(UnicodeString(mszReaders));
	  }
	  else
	  {
		 delete[]mszReaders;
		 throw TSCException(tsceNoReader, "Listing Readers Failed. Error Code : " + UnicodeString(IntToHex(int(lReturn), 2)));
	  }
	  delete[]mszReaders;
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, SMARTCARDLOG, E.Message);
	  throw;
   }
}

void TManagerSmartCard::SetReader(UnicodeString inReaderName)
{
   //TManagerVariable::Instance().SetDeviceStr(DBTransaction, vmSmartCardReader, inReaderName);
   fCardReader = inReaderName;
}

void TManagerSmartCard::SmartCardRemoved()
{
   if (Enabled && CardReaderEventsActive)
   {
	  hCard = NULL;
	  Reset();
	  if (Enabled)
	  {
		 OnCardRemoved.Occured();
	  }
   }
}

void TManagerSmartCard::SmartCardInserted()
{
   if (Enabled)
   {
	  CardUnreadable = false;
	  if (CardConnect() == SCARD_S_SUCCESS)
	  {
		 if (CardReaderEventsActive)
		 {
			Reset();
		 }
		 else
		 {
			if (SmartCard.get() != NULL)
			{
			   SmartCard->ReassignHandle(hCard);
			}
		 }

		 try
		 {
			if (VerifySyndicateCard())
			{
			   CardSyndicate = true;
			}
			else if (VerifyFactoryCard())
			{
			   CardFactory = true;
			}
			else
			{
			   CardUnknown = true;
			}
		 }
		 catch(TSCException & E)
		 {
			if (E.Type == tsceReadFailed)
			{
			   CardCorrupt = true;
			}
			else
			{
			   throw;
			}
		 }

		 if (CardReaderEventsActive)
		 {
			CardInserted = true;
			try
			{
				TSmartCardBlock CardBlockData;
               LoadCardBlock(CardBlockData);
			   if (CardSyndicate)
			   {
				  try
				  {
					 if (CardBlockData.Empty())
					 {
						CardOk = true;
						CardBlank = true;
						CardBlockData.ClearData();
						CardBlockData.Version = SMART_CARD_CURRENT_VERSION;
						SmartCard.reset(new SMART_CARD_CURRENT_VERSION_CLASS(hCard, CardBlockData,GetDefaultSyndCode()));
						OnNewCardInserted.Occured();
					 }
					 else
					 {
						switch(CardBlockData.Version)
						{
						case SMART_CARD_VERSION_UNK:
						   {
							  SmartCard.reset(new TSmartCardVer1(
                                                        hCard,
                                                        CardBlockData,
                                                        GetDefaultSyndCode()));
						   }break;
						case SMART_CARD_VERSION_ONE:
						   {
							  SmartCard.reset(new TSmartCardVer1(
                                                        hCard,
                                                        CardBlockData,
                                                        GetDefaultSyndCode()));
						   }break;
						case SMART_CARD_VERSION_TWO:
						   {
							  SmartCard.reset(new TSmartCardVer2(
                                                        hCard,
                                                        CardBlockData,
                                                        GetDefaultSyndCode()));
							}break;
						case SMART_CARD_VERSION_THREE:
							{
                              SmartCard.reset(
                                    new TSmartCardVer3(
                                                    hCard,
                                                    CardBlockData,
                                                    GetDefaultSyndCode()));

							}break;
                        case SMART_CARD_VERSION_FOUR:
                            {
                              SmartCard.reset(
                                new TSmartCardVer4(hCard,
                                                   CardBlockData,
                                                   GetDefaultSyndCode()));
                            }break;
						case SMART_CARD_VERSION_FIVE: {
							  SmartCard.reset(
                                new TSmartCardVer5(hCard,
                                                   CardBlockData,
                                                   GetDefaultSyndCode()));

							break;
                        }
                        case SMART_CARD_VERSION_SIX: {
                              // smart card version 6 is created for the new loyalty mate cloud services
                              // in this version we store the unique id retrieved from the cloud along with other information
							  SmartCard.reset(
                                new TSmartCardVer6(hCard,
                                                   CardBlockData,
                                                   GetDefaultSyndCode()));

							break;
                        }
						default:
							throw TSCException(tsceUnknownVersion, "Unknown Card Version");
						}

                        // upgrade smart card to latest version. modify this method accordingly to newer smart card versions
                        UpgradeCardToVersion(
                                    SmartCard->GetBlockDataRef().Version,
                                    SMART_CARD_CURRENT_VERSION_NUMBER,
                                    SyndCodes);

						CardOk = true;
						OnCardInserted.Occured();
					 }
				  }
				  catch(TSCException & E)
				  {
					 if (E.Type == tsceInvalidCRC)
					 {
						CardCorrupt = true;
						// Create a Blank Card.
						CardBlockData.ClearData();
						CardBlockData.Version = SMART_CARD_CURRENT_VERSION_NUMBER; //3;
						SmartCard.reset(new SMART_CARD_CURRENT_VERSION_CLASS(hCard, CardBlockData,GetDefaultSyndCode()));
						OnCardCorrupt.Occured();
					 }
					 else if (E.Type == tsceInvalidSyndCRC || E.Type == tsceUnknownVersion)
					 {
						CardUnknown = true;
						OnUnableToDecodeCard.Occured();
					 }
					 else if (E.Type == tsceToMuchCardData)
					 {
						UnicodeString message = "Too much space being used on card.";

						Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
						DBTransaction.StartTransaction();

						TMMContactInfo info = SmartCard->GetContactInfo();
						info.ContactKey = TDBContacts::GetContactByMemberNumberSiteID(DBTransaction, info.MembershipNumber,info.SiteID);
						if (info.ContactKey == 0)
						{
							info.ContactKey = TDBContacts::GetOrCreateContact(DBTransaction, info.ContactKey, eMember, info);
							TDBContacts::SetContactDetails(DBTransaction, info.ContactKey, eMember, info);
						}
						//eMemberSource MemberSource;
						//TDeviceRealTerminal::Instance().ManagerMembership->GetMember(,info,MemberSource);

                        //TODO: Following code removes both alias and mailing address but the alias should be removed only after mailing address, not before.. so modify that

						info.Alias = "";
						info.MailingAddress = "";
						SmartCard->SetContactInfo(info);

						std::auto_ptr <TMemoryStream> stream(new TMemoryStream);
						info.SaveToStream(stream.get());
						unsigned short CalculatedCRC = 0xFF;

						bool memberUpdated = true;

						if ((stream->Size +sizeof(CalculatedCRC)) > (SMART_CARD_CURRENT_VERSION_CARD_MEMBER_DATA_LENGTH))
						{
							message += " \nPlease reduce the amount of space being used. \nFor reference on space being used please check bottom left corner of next screen.";
							MessageBox(message, "Too Much Space Being Used on Card", MB_OK);
							DBTransaction.CommitTransaction();
							DBTransaction.StartTransaction();
							memberUpdated = TDeviceRealTerminal::Instance().ManagerMembership->EditMember(DBTransaction, info) == mrOk;
						}

						DBTransaction.CommitTransaction();
						std::auto_ptr <TfrmProcessing> (frmProcessing)(TfrmProcessing::Create <TfrmProcessing> (Screen->ActiveForm));
						frmProcessing->Message = "Do NOT remove smartcard. Please Wait.";
						frmProcessing->Title = "Do NOT remove smartcard.";
						frmProcessing->Show();
						frmProcessing->Repaint();

						UpgradeCardToVersion(
									SmartCard->GetBlockDataRef().Version,
									SMART_CARD_CURRENT_VERSION_NUMBER,
									SyndCodes,
									true);
						CardOk = true;
						OnCardInserted.Occured();
                     }
					 else
					 {
						throw;
					 }
				  }
			   }
			   else if (CardFactory)
			   {
				  CardOk = true;
				  CardBlank = true;
				  CardBlockData.ClearData();
				  CardBlockData.Version = SMART_CARD_CURRENT_VERSION_NUMBER; //6;
				  SmartCard.reset(new SMART_CARD_CURRENT_VERSION_CLASS(hCard, CardBlockData,GetDefaultSyndCode()));
				  OnNewCardInserted.Occured();
			   }
			   else if (CardCorrupt)
			   {
				  OnCardCorrupt.Occured();
			   }
			   else
			   {
				  CardUnknown = true;
				  OnUnknownCardInserted.Occured();
			   }
			}
			catch(TSCException & E)
			{
			   if (E.Type == tsceReadFailed)
			   {
				  CardUnreadable = true;
				  OnCardUnreadable.Occured();
			   }
			   else
			   {
				  throw;
			   }
			}
		 }
	  }
	  else
	  {
		 CardUnreadable = true;
		 OnCardUnreadable.Occured();
	  }
   }
}

LONG TManagerSmartCard::CardConnect()
{
   DWORD pdwActiveProtocol = SCARD_PROTOCOL_UNDEFINED;

   LONG lReturn = CasConnectA(hCardContext, fCardReader.c_str(), SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, &hCard,
	  &pdwActiveProtocol);

   if (lReturn != SCARD_S_SUCCESS)
   {
	  TManagerLogs::Instance().Add(__FUNC__, SMARTCARDLOG, "Unable to Connect to Card : " + UnicodeString(IntToHex(int(lReturn), 2)));
	  switch(lReturn)
	  {
	  case SCARD_W_UNRESPONSIVE_CARD:
		 TManagerLogs::Instance().Add(__FUNC__, SMARTCARDLOG, "Casmcard Error : The smart card is not responding to a reset");
		 break;
	  case SCARD_W_SECURITY_VIOLATION:
		 TManagerLogs::Instance().Add(__FUNC__, SMARTCARDLOG, "Casmcard Error : Access was denied because of a security violation.");
		 break;
	  case SCARD_W_CHV_BLOCKED:
		 TManagerLogs::Instance().Add(__FUNC__, SMARTCARDLOG,
			"Casmcard Error : The card cannot be accessed because the maximum number of PIN entry attempts has been reached.");
		 break;
	  case SCARD_W_UNSUPPORTED_CARD:
		 TManagerLogs::Instance().Add(__FUNC__, SMARTCARDLOG,
			"Casmcard Error : The reader cannot communicate with the smart card, due to ATR configuration conflicts.");
		 break;
	  }
   }
   return lReturn;
}

void TManagerSmartCard::CardDisconnect()
{
   CasDisconnect(hCard, SCARD_LEAVE_CARD);
   hCard = NULL;
}

bool TManagerSmartCard::VerifyFactoryCard()
{
   bool FactoryCard = false;
   LONG lReturn = SCARD_S_SUCCESS;
   BYTE PSC[3] =
   {
	  0xFF, 0xFF, 0xFF
   };
   lReturn = SLE4442_Compare_Verification_Data(hCard, PSC[0], PSC[1], PSC[2]);
   if (SCARD_S_SUCCESS == lReturn)
   {
	  FactoryCard = true;
   }
   else
   {
	  TManagerLogs::Instance().Add(__FUNC__, SMARTCARDLOG,
		 "Unable to verify Factory Card PSC Data Conformation Failed. : " + UnicodeString(IntToHex(int(lReturn), 2)));
   }
   return FactoryCard;
}

bool TManagerSmartCard::VerifySyndicateCard()
{
   bool SyndicateCard = false;
   LONG lReturn = SCARD_S_SUCCESS;

   lReturn = SLE4442_Compare_Verification_Data(hCard, VerificationCode[0], VerificationCode[1], VerificationCode[2]);
   if (SCARD_S_SUCCESS == lReturn)
   {
	  SyndicateCard = true;
	  // Reset Error Counter on the card.
   }
   else
   {
	  TManagerLogs::Instance().Add(__FUNC__, SMARTCARDLOG,
		 "Unable to verify Syndicate Card PSC Data Confirmation Failed. : " + UnicodeString(IntToHex(int(lReturn), 2)));
   }

   return SyndicateCard;
}

TManagerSmartCard::TMonitoringThread::TMonitoringThread(SCARDCONTEXT &inhCardContext, UnicodeString infCardReader) : TThread(true)
{
   hCardContext = inhCardContext;
   fCardReader = infCardReader;
   fReaderState.szReader = fCardReader.t_str();
   fReaderState.dwCurrentState = SCARD_W_REMOVED_CARD;
}

void TManagerSmartCard::TMonitoringThread::Cancel()
{
   CasCancel(hCardContext);
   Terminate();
}

void __fastcall TManagerSmartCard::TMonitoringThread::Execute()
{
   try
   {
	  try
	  {
		 LONG lReturn = SCARD_S_SUCCESS;
		 fReaderState.dwCurrentState = SCARD_STATE_CHANGED;
		 lReturn = CasGetStatusChange(hCardContext, INFINITE, &fReaderState, 1);
		 fReaderState.dwCurrentState = fReaderState.dwEventState;
		 // If there is a card in the reader on Boot then let everyone know.
		 if (fReaderState.dwEventState & SCARD_STATE_PRESENT)
		 {
			PostMessage(Application->Handle, SMARTCARD_CARDSTATUSCHANGED, (UINT) & fReaderState, SMARTCARD_CARDINSERTED);
		 }

		 ::Sleep(100);
		 do
		 {
			::Sleep(0);
			lReturn = CasGetStatusChange(hCardContext, INFINITE, &fReaderState, 1);
			fReaderState.dwCurrentState = fReaderState.dwEventState;
			if (!Terminated)
			{
			   SetStatus();
			}
		 }
		 while (!Terminated);
	  }
	  __finally
	  {
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, SMARTCARDLOG, E.Message);
   }
}

void __fastcall TManagerSmartCard::TMonitoringThread::SetStatus()
{
   if (!(fReaderState.dwEventState & SCARD_STATE_INUSE))
   {
	  // Ignore in-use events as they are triggered by verifying cards etc.
	  // We have to post messages though to trigger these call back other wise the reader stat never gets updataed.
	  if (fReaderState.dwEventState & SCARD_STATE_CHANGED)
	  {
		 if (Application != NULL)
		 {
			if (fReaderState.dwEventState & SCARD_STATE_EMPTY)
			{
			   PostMessage(Application->Handle, SMARTCARD_CARDSTATUSCHANGED, (UINT) & fReaderState, SMARTCARD_CARDREMOVED);
			}

			if (fReaderState.dwEventState & SCARD_STATE_PRESENT)
			{
			   PostMessage(Application->Handle, SMARTCARD_CARDSTATUSCHANGED, (UINT) & fReaderState, SMARTCARD_CARDINSERTED);
			}
		 }
	  }
   }
}

void TManagerSmartCard::SaveContactInfo(TMMContactInfo &inContactInfo)
{
   try
   {
#ifdef _DEBUG
	  __int64 start = ::GetTickCount();
#endif

	  if (Enabled)
	  {
		 if (CardInserted)
		 {
			if (CardBlank)
			{
			   FormatCard();
			   // Write out the Contact Info.
			   SmartCard->SetVersionInfo();
			   SmartCard->SetContactInfo(inContactInfo);
				TContactPoints PointsInfo;
			   SmartCard->SetPointsInfo(PointsInfo);
			   SmartCard->Save();
			   CardBlank = false;
			}
			else
			{
			   if (CheckCorrectCard(TransactionCardGuid))
				{
				  SmartCard->SetContactInfo(inContactInfo);
				  SmartCard->Save();
			   }
			   else
			   {
				  throw TSCException(tsceIncorrectCard, "Incorrect Smart Card in Reader.");
			   }
			}
            TSmartCardBlock ValCardData;
			LoadCardBlock(ValCardData);
			SmartCard->Validate(ValCardData);
			OnCardUpdated.Occured();
		 }
		 else
		 {
			throw TSCException(tsceNoCard, "No Smart Card in Reader.");
		 }
	  }

#ifdef _DEBUG
	  int Time = ((::GetTickCount() - start));
	  TManagerLogs::Instance().Add(__FUNC__, DEBUGLOG, "--------------------------------");
	  TManagerLogs::Instance().Add(__FUNC__, DEBUGLOG, "SmartCard Save Time :" + IntToStr(Time));
#endif

   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, SMARTCARDLOG, "Error Saving Contact.");
	  TManagerLogs::Instance().Add(__FUNC__, SMARTCARDLOG, E.Message);
	  throw;
   }
}

void TManagerSmartCard::SaveContactPoints(TContactPoints Points)
{
   try
   {
#ifdef _DEBUG
	  __int64 start = ::GetTickCount();
#endif

	  if (Enabled)
	  {
		 if (CardInserted)
		 {
			if (CardBlank)
			{
			   throw TSCException(tsceIncorrectCard, "Unable to save points to a blank card.");
			}
			else
			{
			   if (CheckCorrectCard(TransactionCardGuid))
			   {
				  SmartCard->SetPointsInfo(Points);
				  SmartCard->Save();
			   }
			   else
			   {
				  throw TSCException(tsceIncorrectCard, "Incorrect Smart Card in Reader.");
			   }
                           TSmartCardBlock ValCardData;
			   LoadCardBlock(ValCardData);
			   SmartCard->Validate(ValCardData);

			   // Once the points are saved to the dabase, we will re-read them from the in memory DataBlock structure so the card and in-memory
			   // structure will be synced
			   SmartCard->ReloadCardDetails();
			   //Updates are now triggerd at the end of a process transaction.
			   // See void TListPaymentSystem::Reset(TPaymentTransaction &PaymentTransaction)
			   //OnCardUpdated.Occured();
			}
		 }
		 else
		 {
			throw TSCException(tsceNoCard, "No Smart Card in Reader.");
		 }
	  }

#ifdef _DEBUG
	  __int64 Time = ((::GetTickCount() - start));
	  TManagerLogs::Instance().Add(__FUNC__, DEBUGLOG, "--------------------------------");
	  TManagerLogs::Instance().Add(__FUNC__, DEBUGLOG, "SmartCard Save Time :" + IntToStr(Time));
#endif

   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, SMARTCARDLOG, "Error Saving Contact.");
	  TManagerLogs::Instance().Add(__FUNC__, SMARTCARDLOG, E.Message);
	  throw;
   }
}

void TManagerSmartCard::SaveCardGUID(TSmartCardGUID &CardGuid)
{
   try
   {
#ifdef _DEBUG
	  __int64 start = ::GetTickCount();
#endif

	  if (Enabled)
	  {
		 if (CardInserted)
		 {
			if (CardBlank)
			{
               // The GUID Contains the member number, you should have a member stamped to the card first.
			   throw TSCException(tsceIncorrectCard, "Unable to save GUID to a blank card.");
			}
			else
			{
			   SmartCard->SetGUIDInfo(CardGuid);
			   SmartCard->Save();
               TSmartCardBlock ValCardData;
			   LoadCardBlock(ValCardData);
			   SmartCard->Validate(ValCardData);
			}
		 }
		 else
		 {
			throw TSCException(tsceNoCard, "No Smart Card in Reader.");
		 }
	  }

#ifdef _DEBUG
	  __int64 Time = ((::GetTickCount() - start));
	  TManagerLogs::Instance().Add(__FUNC__, DEBUGLOG, "--------------------------------");
	  TManagerLogs::Instance().Add(__FUNC__, DEBUGLOG, "SmartCard Save Time :" + IntToStr(Time));
#endif

   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, SMARTCARDLOG, "Error Saving Contact.");
	  TManagerLogs::Instance().Add(__FUNC__, SMARTCARDLOG, E.Message);
	  throw;
   }
}

void TManagerSmartCard::FormatCard()
{
   LONG lReturn = SCARD_S_SUCCESS;
   CasBeginTransaction(hCard);
   lReturn = SLE4442_Update_Security_Memory(hCard, VerificationCode, 0x01, sizeof(VerificationCode));
   CasEndTransaction(hCard, SCARD_LEAVE_CARD);
   if (lReturn != SCARD_S_SUCCESS)
   {
	  TManagerLogs::Instance().Add(__FUNC__, SMARTCARDLOG, "Format Card Failed. : " + UnicodeString(IntToHex(int(lReturn), 2)));
   }
}

void TManagerSmartCard::RestoreToRestorePoint(TSmartCardBlock &RestorePoint)
{
   if (Enabled)
   {
	  if (CardInserted)
	  {
		 FormatCard();
		 SmartCard->Restore(RestorePoint);
	  }
   }
}

const TSmartCardGUID *TManagerSmartCard::GetCardGUID()
{
   // Create Card Restore Point.
   TSmartCardGUID *RetVal = NULL;
   if (CardInserted)
   {
	  RetVal = SmartCard->GetBlockData()->GetGUID();
   }
   return RetVal;
}

TSmartCardBlock *TManagerSmartCard::GetRestorePoint()
{
   // Create Card Restore Point.
   TSmartCardBlock *RetVal = NULL;
   if (CardInserted && CardOk)
   {
	  RetVal = SmartCard->GetBlockData();
   }
   return RetVal;
}

void TManagerSmartCard::GetSyndCodeInfo(TSyndCode &SyndCode)
{
   // Create Card Restore Point.
   if (CardInserted && SmartCard.get() != NULL)
   {
	  SyndCode = SmartCard->SyndCode;
   }
}

bool TManagerSmartCard::GetSyndCodeValid()
{
   // Create Card Restore Point.
   bool RetVal = false;
   if (CardInserted && SmartCard.get() != NULL)
   {
	  RetVal = SmartCard->SyndCode.Valid();
   }
   return RetVal;
}

void TManagerSmartCard::StreamGetVersion(TSmartCardBlock &CardBlock, TMemoryStream &Stream)
{
   Stream.Position = 0;
   CardBlock.BlockInfoRead(V2_CARD_VERSION_DATA_START, V2_CARD_VERSION_DATA_LENGTH, Stream);
   Stream.Position = 0;
}

UnicodeString TManagerSmartCard::LoadCardGUID()
{
   std::auto_ptr <TMemoryStream> GUIDStream(new TMemoryStream);
   GUIDStream->Clear();

   LONG lReturn = SCARD_F_UNKNOWN_ERROR;
   lReturn = CardReadMainMemory(V2_CARD_GUID_START, V2_CARD_GUID_LENGTH, *GUIDStream.get());
   if (lReturn != SCARD_S_SUCCESS)
   {
	  throw TSCException(tsceReadFailed, "Unable to load card GUID");
   }
   std::vector <char> recvbuffer((V2_CARD_GUID_LENGTH * 2) + 1);
   ZeroMemory(&recvbuffer[0], recvbuffer.size());

   BinToHex((void*)GUIDStream->Memory, &recvbuffer[0], (int)GUIDStream->Size);
   return UnicodeString((char*)(&recvbuffer[0]), recvbuffer.size());
}

void TManagerSmartCard::LoadCardBlock(TSmartCardBlock &CardBlock)
{
   if (fMonitoringThread)
   {
	  LONG lReturn = SCARD_F_UNKNOWN_ERROR;
	  CardBlock.ClearAll();
	  lReturn = CardReadMainMemory(CARD_BLOCK_START, CARD_TOTAL_MEMORY_LENGTH, CardBlock.GetStreamRef());
	  if (lReturn != SCARD_S_SUCCESS)
	  {
		 throw TSCException(tsceReadFailed, "Unable to load card data");
	  }
	  CardBlock.LoadGUID();
	  CardBlock.Version = GetCardBlockVersion(CardBlock);
   }
}

int TManagerSmartCard::GetCardBlockVersion(TSmartCardBlock &CardBlock)
{
   std::auto_ptr <TMemoryStream> Stream(new TMemoryStream);
   StreamGetVersion(CardBlock, *Stream.get());

   Stream->Position = 0;
   BYTE CardVersion = 0;
   StreamRead(Stream.get(), CardVersion);

   if (CardVersion < SMART_CARD_VERSION_TWO || CardVersion > SMART_CARD_CURRENT_VERSION)
   { // Use the Contact Version.

	  // ok if its not a version 2+ card then its really gota be a version 1 card.
	  // We may have to readdress this section of code.
	  bool SyndCodeValidated = false;
      std::map <int, TSyndCode> ::iterator ptrSyndCodes = SyndCodes.begin();
      for (ptrSyndCodes = SyndCodes.begin();ptrSyndCodes != SyndCodes.end() && !SyndCodeValidated; advance(ptrSyndCodes,1))
      {
		 Stream->Clear();
		 Stream->Position = 0;
		 CardBlock.BlockInfoRead(V1_CARD_MEMBER_DATA_START, 8, *Stream.get());
		 Decrypt(*Stream.get(), ptrSyndCodes->second.DecryptedSyndCode);
		 Stream->Position = 0;
		 CardVersion = SMART_CARD_VERSION_UNK;
		 StreamRead(Stream.get(), CardVersion);
		 if (CardVersion == 0x01)
		 {
			SyndCodeValidated = true;
		 }
	  }
   }
   return CardVersion;
}

bool TManagerSmartCard::CheckStreamCRC(TMemoryStream *Stream)
{
   bool RetVal = false;
   unsigned short CalculatedCRC = 0;
   unsigned short CardCRC = 0;
   int DataSize = Stream->Position;
   StreamRead(Stream, CardCRC);

   unsigned char *ptrData = (unsigned char*)Stream->Memory;
   CalculatedCRC = (unsigned short)StartCRC(ptrData[0]);
   for (int i = 1; i < DataSize; i++)
   {
	  CalculatedCRC = (unsigned short)UpdateCRC(CalculatedCRC, ptrData[i]);
   }

   if (CalculatedCRC == CardCRC)
   {
	  RetVal = true;
   }
   return RetVal;
}

LONG TManagerSmartCard::CardReadMainMemory(int BlockStart, int BlockLength, TMemoryStream &Stream)
{
   DWORD length = BlockLength;
   std::vector <BYTE> recvbuffer(length);
   ZeroMemory(&recvbuffer[0], length);

   LONG lReturn = SCARD_S_SUCCESS;
   CasBeginTransaction(hCard);
   lReturn = SLE4442_Read_Main_Memory(hCard, &recvbuffer[0], BlockStart, &length);
   CasEndTransaction(hCard, SCARD_LEAVE_CARD);

   Stream.Write(&recvbuffer[0], recvbuffer.size());

   if (lReturn != SCARD_S_SUCCESS)
   {
	  TManagerLogs::Instance().Add(__FUNC__, SMARTCARDLOG, "Reading Data Failed. : " + UnicodeString(IntToHex(int(lReturn), 2)));
   }
   return lReturn;
}

LONG TManagerSmartCard::CardReadProtectedMemory(TMemoryStream &Stream)
{
   DWORD length = 4;
   std::vector <BYTE> recvbuffer(length);
   ZeroMemory(&recvbuffer[0], length);

   LONG lReturn = SCARD_S_SUCCESS;
   CasBeginTransaction(hCard);
   lReturn = SLE4442_Read_Protection_Memory(hCard, &recvbuffer[0]);
   CasEndTransaction(hCard, SCARD_LEAVE_CARD);

   Stream.Write(&recvbuffer[0], recvbuffer.size());

   if (lReturn != SCARD_S_SUCCESS)
   {
	  TManagerLogs::Instance().Add(__FUNC__, SMARTCARDLOG, "Reading Data Failed. : " + UnicodeString(IntToHex(int(lReturn), 2)));
   }
   return lReturn;
}

LONG TManagerSmartCard::CardReadSecurityMemory(TMemoryStream &Stream)
{
   DWORD length = 4;
   std::vector <BYTE> recvbuffer(length);
   ZeroMemory(&recvbuffer[0], length);

   LONG lReturn = SCARD_S_SUCCESS;
   CasBeginTransaction(hCard);
   lReturn = SLE4442_Read_Security_Memory(hCard, &recvbuffer[0]);
   CasEndTransaction(hCard, SCARD_LEAVE_CARD);

   Stream.Write(&recvbuffer[0], recvbuffer.size());

   if (lReturn != SCARD_S_SUCCESS)
   {
	  TManagerLogs::Instance().Add(__FUNC__, SMARTCARDLOG, "Reading Data Failed. : " + UnicodeString(IntToHex(int(lReturn), 2)));
   }
   return lReturn;
}

bool TManagerSmartCard::CheckCorrectCard(TSmartCardGUID &SmartCardGUID)
{
   bool CorrectCard = false;
   if (fMonitoringThread)
   {
	  UnicodeString CurrentGUID = LoadCardGUID();
	  if (SmartCardGUID.Compare(CurrentGUID))
	  {
		 CorrectCard = true;
	  }
   }
   return CorrectCard;
}

bool TManagerSmartCard::ContactInfoMatches(TMMContactInfo &inContactInfo)
{
   bool ContactInfoMatches = false;
   if (CardInserted)
   {
	  if (inContactInfo.SiteID == SmartCard->GetContactInfo().SiteID)
	  {
		 if (inContactInfo.MembershipNumber == SmartCard->GetContactInfo().MembershipNumber)
		 {
			ContactInfoMatches = true;
		 }
	  }
   }
   return ContactInfoMatches;
}

void TManagerSmartCard::Decrypt(TMemoryStream &Data, AnsiString SyndicateCode)
{
   CBlowFish BF;
   // Have to pass though the NULL on the end of the string because this old format
   // has it inherently ( yea you cant see it but its there ).
   // BYTE Passwd[] = {"0123456789abcdefghijklmnopqrstuvwxyz0123456789"};
   // BF.Initialize(Passwd,sizeof(Passwd));
   BF.Initialize(SyndicateCode.c_str(), SyndicateCode.Length() + 1);
   BF.Decode((BYTE*)Data.Memory, (BYTE*)Data.Memory, Data.Size);
}

/* Used be the Calling programs to "lock in a card".
If the card changes between transaction calls any writes to the card fails.
 */
void TManagerSmartCard::BeginTransaction()
{
   if (fMonitoringThread && CardInserted)
   {
	  TransactionCardGuid.Assign(SmartCard->GetBlockData()->GetGUID());
   }
}

void TManagerSmartCard::EndTransaction()
{
   TransactionCardGuid.Clear();
}

TManagerSmartCard::GetContactInfo(TMMContactInfo & ContactInfo)
{
   if (CardInserted && SmartCard.get() != NULL)
   {
	  ContactInfo = SmartCard->GetContactInfo();
   }
}

void TManagerSmartCard::Reset()
{
   SmartCard.reset(NULL);

   CardSyndicate = false;
   CardFactory = false;
   CardInserted = false;
   CardUnknown = false;
   CardCorrupt = false;
   CardBlank = false;
   CardOk = false;
}

void TManagerSmartCard::CardReaderEventsOn()
{
   CardReaderEventsActive = true;
}

void TManagerSmartCard::CardReaderEventsOff()
{
   CardReaderEventsActive = false;
}

void TManagerSmartCard::UpdateViewers()
{
	OnCardUpdated.Occured();
}

// gradually upgrades an smart card to a given version
// add more case blocks for newer card upgrages ( Ex if the latest version is n our last case should be n-1 )
bool TManagerSmartCard::UpgradeCardToVersion(int version, int newVersion, std::map <int, TSyndCode> SyndCodes, bool cardUnlocked)
{
    bool result = false;

	if (!cardUnlocked)
	{
		// decrypt and read card information
		SmartCard->UnlockCard(SyndCodes);
	}

    while(version < newVersion)
    {
        switch(version)
        {
            case SMART_CARD_VERSION_ONE:
            {
                TSmartCardVer1 SmartCardVer1(
                                            hCard,
                                            SmartCard->GetBlockDataRef(),
                                            GetDefaultSyndCode());
                SmartCard.reset(new TSmartCardVer2(SmartCardVer1));
                result = true;
                break;
            }
            case SMART_CARD_VERSION_TWO:
            {
                TSmartCardVer2 SmartCardVer2(
                                            hCard,
                                            SmartCard->GetBlockDataRef(),
                                            GetDefaultSyndCode());
                SmartCard.reset(new TSmartCardVer3(SmartCardVer2));
                result = true;
                break;
            }
            case SMART_CARD_VERSION_THREE:
            {
                // Note: upgrading to version 5 as version 4 is buggy
                SmartCard.reset(new TSmartCardVer5(*SmartCard));
                result = true;
                break;
            }
            case SMART_CARD_VERSION_FOUR:
            {
                SmartCard.reset(new TSmartCardVer5(*SmartCard));
                result = true;
                break;
            }
            case SMART_CARD_VERSION_FIVE:
            {
                SmartCard.reset(new TSmartCardVer6(*SmartCard));
                result = true;
                break;
            }
            case SMART_CARD_VERSION_SIX:
            {
                // currently doing nothing here as this is the latest version
                result = true;
                break;
            }
        }

        version++;
    }

    // write the information in the latest format to the card
    if(result)
    {
      SmartCard->Upgrade(SyndCodes);
      SmartCard->GetBlockDataRef().Version = newVersion; // set the card block data version here
    }

    return result
            || (version == newVersion); // this is here to indicate success if the version we upgrade from is the same as version we upgrade to
}

int TManagerSmartCard::GetCurrentCardVersion()
{
    return SmartCard->GetBlockData()->Version;
}

TSyndCode TManagerSmartCard::GetCurrentSyndicateCode()
{
    return SmartCard->SyndCode;
}

