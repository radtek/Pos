// ---------------------------------------------------------------------------

#pragma hdrstop

#include "MembershipManagerSmartCards.h"
#include "MMMessageBox.h"
#include "ContactStaff.h"
#include "MMLogging.h"
#include "StringTableRes.h"
#include "ReportDisplayNav.h"
#include "SmartCardException.h"
#include "ManagerDiscount.h"
#include "MMTouchKeyboard.h"
#include "PointsRulesSetUtils.h"
#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef  PalmMate
#include "Palm.h"
#endif

#include "SmartcardPreloader.h"
#include "ManagerLoyaltyMate.h"
#include "MMTouchKeyboard.h"
#include "LoyaltyMateOperationDialogBox.h"
#include "DBTab.h"
#include "EditCustomer.h"
#include "DBGroups.h"

// ---------------------------------------------------------------------------

#pragma package(smart_init)

TManagerMembershipSmartCards::TManagerMembershipSmartCards(Database::TDBControl &inDBControl, TModules &inModules)
: TManagerMembership(inDBControl, inModules),ManagerSmartCards(new TManagerSmartCard())
{
	CardIssueInProgress = false;
	AllowSmartCardNotPresentMemberTransactions = false;
}

TManagerMembershipSmartCards::~TManagerMembershipSmartCards()
{
	ManagerSmartCards->OnCardInserted.DeregisterForEvent(OnCardInserted);
	ManagerSmartCards->OnCardInserted.DeregisterForEvent(OnUpDateRestorePoint);
	ManagerSmartCards->OnCardUpdated.DeregisterForEvent(OnUpDateRestorePoint);
	ManagerSmartCards->OnUnknownCardInserted.DeregisterForEvent(OnUnknownCardInserted);
	ManagerSmartCards->OnCardCorrupt.DeregisterForEvent(OnCardCorrupt);
	ManagerSmartCards->OnUnableToDecodeCard.DeregisterForEvent(OnUnableToDecodeCard);
	ManagerSmartCards->OnNewCardInserted.DeregisterForEvent(OnBlankCardInserted);
	ManagerSmartCards->OnCardUnreadable.DeregisterForEvent(OnCardUnreadable);
}

void TManagerMembershipSmartCards::Initialise(Database::TDBTransaction &DBTransaction)
{
	ManagerSmartCards->OnCardInserted.RegisterForEvent(OnCardInserted);
	ManagerSmartCards->OnCardInserted.RegisterForEvent(OnUpDateRestorePoint);
	ManagerSmartCards->OnCardUpdated.RegisterForEvent(OnUpDateRestorePoint);
	ManagerSmartCards->OnUnknownCardInserted.RegisterForEvent(OnUnknownCardInserted);
	ManagerSmartCards->OnCardCorrupt.RegisterForEvent(OnCardCorrupt);
	ManagerSmartCards->OnUnableToDecodeCard.RegisterForEvent(OnUnableToDecodeCard);
	ManagerSmartCards->OnNewCardInserted.RegisterForEvent(OnBlankCardInserted);
	ManagerSmartCards->OnCardUnreadable.RegisterForEvent(OnCardUnreadable);

	AllowSmartCardNotPresentMemberTransactions = TManagerVariable::Instance().GetBool(DBTransaction, vmAllowSmartCardNotPresentMemberTransactions,
	false);

	ManagerSyndicateCode.Initialise(DBTransaction);

	if (Modules.Status[eSmartCardSystem]["Registered"])
	{
		ManagerSmartCards->Registered = true;
		bool UserEnabled = TManagerVariable::Instance().GetBool(DBTransaction, vmSmartCardMembership, false);
		ManagerSmartCards->Initialise(UserEnabled, ManagerSyndicateCode.GetSyndCodes(true));
	}

	Modules.Status[eSmartCardSystem]["Enabled"] = ManagerSmartCards->Enabled;

	// start loyaltymate manager instance
	TManagerLoyaltyMate::Instance();
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//Smart Card Related Code
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void TManagerMembershipSmartCards::BeginMemberTransaction()
{
	if (ManagerSmartCards->Enabled)
	{
		ManagerSmartCards->BeginTransaction();
	}
}

void TManagerMembershipSmartCards::EndMemberTransaction()
{
	ManagerSmartCards->EndTransaction();
}

void TManagerMembershipSmartCards::OnBlankCardInserted(TSystemEvents *Sender)
{
    MembershipSystem->ResetPoints();
	if (!CardIssueInProgress)
	{
		try
		{
			CardIssueInProgress = true;

			if (!Modules.Status[eRegMembers]["Enabled"])
			{
				MessageBox("Membership is disabled.", "Warning", MB_ICONWARNING + MB_OK);
			}
			else
			{
				if (ManagerSmartCards->GetSyndCodeValid())
				{
                    bool canAddMember = false;
                    if(TGlobalSettings::Instance().GiftCardOnly)
                     {
                         canAddMember = true;
                     }
                     else
                     {
                        TMMContactInfo TempUserInfo;
                        Database::TDBTransaction DBTransaction(DBControl);
                        DBTransaction.StartTransaction();
                        std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
                        TLoginSuccess Result = Staff->Login(Screen->ActiveForm, DBTransaction, TempUserInfo, CheckAccountManager);
                        DBTransaction.Commit();
                        if (Result == lsAccepted)
                        {
                           canAddMember = true;
                        }
                        else if (Result == lsDenied)
                        {
                            MessageBox("You do not have access to Membership.", "Error", MB_OK + MB_ICONERROR);
                        }
                        else if (Result == lsPINIncorrect)
                        {
                            MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
                        }
                     }

                     if(canAddMember)
                     {
                            Database::TDBTransaction DBTransaction(DBControl);
                            RegisterTransaction(DBTransaction);
                            TMMContactInfo Info;
                            AddMember(Info);
                     }
				}
				else
				{
					MessageBox("There is no default syndicate code set. Please set one before trying to add a Smart Card.",
					"Failed to Save to Smart Card.", MB_OK + MB_ICONERROR);
				}
			}
		}
		__finally
		{
			CardIssueInProgress = false;
		}
	}
}

void TManagerMembershipSmartCards::GetReportMemberCardsInfo(Database::TDBTransaction &DBTransaction, TMMContactInfo &Member,
TStringList *Report)
{
	// Start the Member Table.
	AnsiString Temp = LoadStr(HTML_START);
	Temp = AnsiReplaceStr(Temp, "%TITLE%", "Card Report");
	Report->Add(Temp);

	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	DBTransaction.RegisterQuery(IBInternalQuery);

	// Find his Active Cards (Card) and display.
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "select * from SMARTCARDS where CONTACTS_KEY = :CONTACTS_KEY "
	"and CARD_STATUS = 1 order by SMARTCARDS.BACKUP_TIME_STAMP desc";
	IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = Member.ContactKey;
	IBInternalQuery->ExecQuery();
	for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	{
		TSmartCardBlock RestorePoint;
		int SMARTCARDS_KEY = IBInternalQuery->FieldByName("SMARTCARDS_KEY")->AsInteger;
		IBInternalQuery->FieldByName("BACKUP")->SaveToStream(RestorePoint.GetStream());
		GetReportCardInfo(DBTransaction,SMARTCARDS_KEY, &RestorePoint, Report);
	}

	// Find and Display all blocked and backups.
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "select * from SMARTCARDS where CONTACTS_KEY = :CONTACTS_KEY "
	"and CARD_STATUS != 1 order by SMARTCARDS.BACKUP_TIME_STAMP desc";
	IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = Member.ContactKey;
	IBInternalQuery->ExecQuery();

	for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	{
		TSmartCardBlock RestorePoint;
		int SMARTCARDS_KEY = IBInternalQuery->FieldByName("SMARTCARDS_KEY")->AsInteger;
		IBInternalQuery->FieldByName("BACKUP")->SaveToStream(RestorePoint.GetStream());
		GetReportCardInfo(DBTransaction,SMARTCARDS_KEY, &RestorePoint, Report);
	}

}

void TManagerMembershipSmartCards::GetReportCardInfo(Database::TDBTransaction &DBTransaction,int inSmartCardKey, TSmartCardBlock *SmartCardBlock,
TStringList *Report)
{
	// Start the Member Table.
	UnicodeString Temp = LoadStr(HTML_START);
	Temp = AnsiReplaceStr(Temp, "%TITLE%", "Card Report");
	Report->Add(Temp);

	std::auto_ptr <TSmartCard> SmartCard(NULL);
	try
	{
		SmartCardBlock->Version = ManagerSmartCards->GetCardBlockVersion(*SmartCardBlock);
		SmartCardBlock->LoadGUID();

		Report->Add("<span style=\"font-weight: bold; text-decoration: underline;\">SmartCard Details.<br></span>");
		Report->Add("<span style=\"font-weight: bold;\">GUID : </span> " + SmartCardBlock->CardGuid.GUID() + ".<br>");
		Report->Add("<span style=\"font-weight: bold;\">Card Version : </span> " + IntToStr(SmartCardBlock->Version) + ".<br>");
		Report->Add("<span style=\"font-weight: bold;\">Card Key : </span> " + IntToStr(inSmartCardKey) + ".<br>");

		switch(SmartCardBlock->Version)
		{
		case SMART_CARD_VERSION_UNK:
			{
				TSmartCardVer1 SmartCardVer1(NULL, *SmartCardBlock, ManagerSyndicateCode.GetDefaultSyndCode());
				SmartCard.reset(new TSmartCardVer2(SmartCardVer1));
				SmartCard->UnlockCard(ManagerSyndicateCode.GetSyndCodes(true));
			}break;
		case SMART_CARD_VERSION_ONE:
			{
				TSmartCardVer1 SmartCardVer1(NULL, *SmartCardBlock, ManagerSyndicateCode.GetDefaultSyndCode());
				TSmartCardVer2 SmartCardVer2(SmartCardVer1);
				SmartCard.reset(new TSmartCardVer3(SmartCardVer2));
				SmartCard->UnlockCard(ManagerSyndicateCode.GetSyndCodes(true));
			}break;
		case SMART_CARD_VERSION_TWO:
			{
				TSmartCardVer2 SmartCardVer2(NULL, *SmartCardBlock, ManagerSyndicateCode.GetDefaultSyndCode());
				SmartCard.reset(new TSmartCardVer3(SmartCardVer2));
				SmartCard->UnlockCard(ManagerSyndicateCode.GetSyndCodes(true));
			}break;
		case SMART_CARD_VERSION_THREE:
			{
				SmartCard.reset(new TSmartCardVer3(NULL, *SmartCardBlock, ManagerSyndicateCode.GetDefaultSyndCode()));
				SmartCard->UnlockCard(ManagerSyndicateCode.GetSyndCodes(true));
			}break;
		case SMART_CARD_VERSION_FOUR:
			{
				SmartCard.reset(new TSmartCardVer4(NULL, *SmartCardBlock, ManagerSyndicateCode.GetDefaultSyndCode()));
				SmartCard->UnlockCard(ManagerSyndicateCode.GetSyndCodes(true));
			}break;
		case SMART_CARD_VERSION_FIVE:
			{
				SmartCard.reset(new TSmartCardVer5(NULL, *SmartCardBlock, ManagerSyndicateCode.GetDefaultSyndCode()));
				SmartCard->UnlockCard(ManagerSyndicateCode.GetSyndCodes(true));
			}break;
		case SMART_CARD_VERSION_SIX:
			{
				SmartCard.reset(new TSmartCardVer6(NULL, *SmartCardBlock, ManagerSyndicateCode.GetDefaultSyndCode()));
				SmartCard->UnlockCard(ManagerSyndicateCode.GetSyndCodes(true));
			}break;
		default:
			throw TSCException(tsceUnknownVersion, "Unknown Card Version");
		}
		MembershipSystem->GetReportThisCardsInfo(DBTransaction, inSmartCardKey,SmartCardBlock->CardGuid.GUID(), Report);
		TMMContactInfo CardContactInfo = SmartCard->GetContactInfo();
		GetReportMemberInfo(CardContactInfo, SmartCard->SyndCode, "Member Info On Card", Report);
	}
	catch(TSCException & E)
	{
		Report->Add("<span style=\"font-weight: bold; text-decoration: underline;\">Error Reading Smart Card.<br></span>");
		Report->Add("Error in " __FUNC__ "<br>");
		Report->Add("Error Type. " + IntToStr(E.Type) + "<br>");
		Report->Add("Error :" + E.Message);
	}
}

void TManagerMembershipSmartCards::ReassignCard()
{
	// If there is no card inserted then prompt them to insert one.
	if (ManagerSmartCards->CardInserted)
	{
		Database::TDBTransaction DBTransaction(DBControl);
		RegisterTransaction(DBTransaction);
		DBTransaction.StartTransaction();
		try
		{
			TMMContactInfo Member;
			TLoginSuccess Result = MembershipSystem->Login(DBTransaction, Member);
			if (Result == lsAccepted || Result == lsAccountBlocked)
			{
				SaveContactInfoReassignedToSmartCard(Member);
				TDBContacts::SetCardCreationDate(DBTransaction, Member.ContactKey, Member.CardCreationDate);
			}
		}
		catch(Exception & E)
		{
			TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
			MessageBox(E.Message, "Failed to Restore Smart Card.", MB_OK + MB_ICONERROR);
		}
		DBTransaction.Commit();
	}
	else
	{
		MessageBox("Please Insert Card in reader and try again.", "No Card found.", MB_ICONERROR + MB_OK);
	}
	// Disable all Card insertaion Events??
	// Find that cards last active restore point based on ATR of card.
	// Write restore point back to card.
}

void TManagerMembershipSmartCards::RestoreCardWithPoints()
{
	// If there is no card inserted then prompt them to insert one.
	if (ManagerSmartCards->CardInserted)
	{
		if (ManagerSmartCards->CardBlank)
		{ // Select the Member.
			Database::TDBTransaction DBTransaction(DBControl);
			RegisterTransaction(DBTransaction);
			DBTransaction.StartTransaction();
			try
			{
				TMMContactInfo Member;
				TLoginSuccess Result = MembershipSystem->Login(DBTransaction, Member);
				if (Result == lsAccountBlocked)
				{
					MessageBox("Account Blocked " + Member.Name + " " + Member.AccountInfo, "Account Blocked", MB_OK + MB_ICONINFORMATION);
				}
				else if (Result == lsAccepted)
				{
					//Assign the Card a GUID.
					int MemberShipNumber = StrToIntDef(Member.MembershipNumber, 0);
					if (MemberShipNumber != 0)
					{
						//Save this Member to the card.
						SaveContactInfoAddedToSmartCard(Member);
						// TODO -c Improvement: Security hole.
						// There is a tiny chance to swap cards here the SaveContactInfoAddedToSmartCard
						// should be wrapped in the BeginMemberTransaction as well, at the moment
						// it has it's own.
						BeginMemberTransaction();
						ManagerSmartCards->SaveContactPoints(Member.Points);
						EndMemberTransaction();
					}
				}
			}
			catch(Exception & E)
			{
				TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
				MessageBox(E.Message, "Failed to Restore Smart Card.", MB_OK + MB_ICONERROR);
			}
			DBTransaction.Commit();
		}
		else
		{
			const TSmartCardGUID *CardGuid = ManagerSmartCards->GetCardGUID();
			if (CardGuid != NULL)
			{
				Database::TDBTransaction DBTransaction(DBControl);
				RegisterTransaction(DBTransaction);
				DBTransaction.StartTransaction();
				try
				{
					TSmartCardBlock RestorePoint;
					RestorePoint.CardGuid.Assign(*CardGuid);
					if (GetSmartCardRestorePointByCard(DBTransaction, RestorePoint))
					{
						if (CustomMessageBox("Restore point found.\rAre you sure you wish to restore this card.\r", "Restore Point Found.",
									MB_ICONQUESTION, "Restore Card", "Dont Update Card") == IDOK)
						{
							ManagerSmartCards->RestoreToRestorePoint(RestorePoint);
							MessageBox("Card Restored from image.", "Card Restored.", MB_ICONERROR + MB_OK);
						}
					}
					else
					{
						MessageBox("Unable to find a restore point for this card.", "No restore point found.", MB_ICONERROR + MB_OK);
					}
				}
				catch(Exception & E)
				{
					TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
					MessageBox(E.Message, "Failed to Restore Smart Card.", MB_OK + MB_ICONERROR);
				}
				DBTransaction.Commit();
			}
		}
	}
	else
	{
		MessageBox("Please Insert Card in reader and try again.", "No Card found.", MB_ICONERROR + MB_OK);
	}
	// Disable all Card insertaion Events??
	// Find that cards last active restore point based on ATR of card.
	// Write restore point back to card.
}

void TManagerMembershipSmartCards::RestoreCard()
{
	// If there is no card inserted then prompt them to insert one.
	if (ManagerSmartCards->CardInserted)
	{
		if (ManagerSmartCards->CardBlank)
		{ // Select the Member.
			Database::TDBTransaction DBTransaction(DBControl);
			RegisterTransaction(DBTransaction);
			DBTransaction.StartTransaction();
			try
			{
				TMMContactInfo Member;
				TLoginSuccess Result = MembershipSystem->Login(DBTransaction, Member);
				if (Result == lsAccountBlocked)
				{
					MessageBox("Account Blocked " + Member.Name + " " + Member.AccountInfo, "Account Blocked", MB_OK + MB_ICONINFORMATION);
				}
				else if (Result == lsAccepted)
				{
					SaveContactInfoAddedToSmartCard(Member);
				}
			}
			catch(Exception & E)
			{
				TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
				MessageBox(E.Message, "Failed to Restore Smart Card.", MB_OK + MB_ICONERROR);
			}
			DBTransaction.Commit();
		}
		else
		{
			const TSmartCardGUID *CardGuid = ManagerSmartCards->GetCardGUID();
			if (CardGuid != NULL)
			{
				Database::TDBTransaction DBTransaction(DBControl);
				RegisterTransaction(DBTransaction);
				DBTransaction.StartTransaction();
				try
				{
					TSmartCardBlock RestorePoint;
					RestorePoint.CardGuid.Assign(*CardGuid);
					if (GetSmartCardRestorePointByCard(DBTransaction, RestorePoint))
					{
						if (CustomMessageBox("Restore point found.\rAre you sure you wish to restore this card.\r", "Restore Point Found.",
									MB_ICONQUESTION, "Restore Card", "Dont Update Card") == IDOK)
						{
							ManagerSmartCards->RestoreToRestorePoint(RestorePoint);
							MessageBox("Card Restored from image.", "Card Restored.", MB_ICONERROR + MB_OK);
						}
					}
					else
					{
						MessageBox("Unable to find a restore point for this card.", "No restore point found.", MB_ICONERROR + MB_OK);
					}
				}
				catch(Exception & E)
				{
					TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
					MessageBox(E.Message, "Failed to Restore Smart Card.", MB_OK + MB_ICONERROR);
				}
				DBTransaction.Commit();
			}
		}
	}
	else
	{
		MessageBox("Please Insert Card in reader and try again.", "No Card found.", MB_ICONERROR + MB_OK);
	}
	// Disable all Card insertaion Events??
	// Find that cards last active restore point based on ATR of card.
	// Write restore point back to card.
}

int TManagerMembershipSmartCards::GetNextSmartCardNumber(Database::TDBTransaction &DBTransaction)
{
	int RetVal = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_SMARTCARD_NUMBER, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		RetVal = IBInternalQuery->Fields[0]->AsInteger;
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
	return RetVal;
}

void TManagerMembershipSmartCards::ExploreCard()
{
	std::auto_ptr <TStringList> Report(new TStringList);
	Database::TDBTransaction DBTransaction(DBControl);
	RegisterTransaction(DBTransaction);
	DBTransaction.StartTransaction();

	if (ManagerSmartCards->CardInserted)
	{
		try
		{
			TSmartCardBlock *RestorePoint = ManagerSmartCards->GetRestorePoint();
			if (RestorePoint != NULL)
			{
				GetReportCardInfo(DBTransaction,0, RestorePoint, Report.get());
			}
		}
		catch(Exception & E)
		{
			TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
			Report->Add("<span style=\"font-weight: bold; text-decoration: underline;\">Error Generating Report.<br></span>");
			Report->Add("Error in " __FUNC__ "<br>");
			Report->Add("Error :" + E.Message);
		}
	}
	else
	{
		try
		{
			TSyndCode DummySyndCode;
			DummySyndCode.Name = "No Card Inserted";
			TMMContactInfo Member;
			TLoginSuccess Result = MembershipSystem->Login(DBTransaction, Member);
			if (Result == lsAccepted)
			{
				GetReportMemberInfo(Member, DummySyndCode, "Member Info In Database :", Report.get());
				GetReportMemberCardsInfo(DBTransaction, Member, Report.get());
			}
		}
		catch(Exception & E)
		{
			TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
			MessageBox(E.Message, "Failed to Restore Smart Card.", MB_OK + MB_ICONERROR);
		}
	}

	// Start the Member Table.
	AnsiString Temp = LoadStr(HTML_BODY_STOP);
	Report->Add(Temp);

	// Pull the Info of the current card and format it into HTML, Add it to the report.
	DBTransaction.Commit();

	std::auto_ptr <TfrmReportDisplayNav> frmReportDisplay(TfrmReportDisplayNav::Create <TfrmReportDisplayNav> (Screen->ActiveForm));
	frmReportDisplay->Navigate(Report.get());
	frmReportDisplay->ShowModal();
}

void TManagerMembershipSmartCards::OnUnknownCardInserted(TSystemEvents *Sender)
{
	MessageBox("Security WARNING!\rMenuMate has determined that this smart card is not configured for this system."
	"\r If you continue to attempt to use it on this system it will be " " destroyed and any information on it will become unavailable.",
	"!DANGER!", MB_ICONERROR + MB_OK);
}

void TManagerMembershipSmartCards::OnCardCorrupt(TSystemEvents *Sender)
{
	MessageBox("Unable to decode card.\rThe Data appears corrupt.\rTry again however you may need to restore from backup.",
	"Card Read Error.", MB_ICONERROR + MB_OK);
}

void TManagerMembershipSmartCards::OnCardUnreadable(TSystemEvents *Sender)
{
	MessageBox("Unable to read Card.\r Check it is inserted the right way up.\r Check the cards contacts are clean.", "Card Read Error.",
	MB_ICONERROR + MB_OK);
}

void TManagerMembershipSmartCards::OnUnableToDecodeCard(TSystemEvents *Sender)
{
	MessageBox("Sorry.\rThis MenuMate card is not part of this SmartCard group.", "Incompatible Card", MB_ICONERROR + MB_OK);
}

void TManagerMembershipSmartCards::BlockAllOfContactsSmartCards(Database::TDBTransaction &DBTransaction, int contactKey)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		// Ban all other ATR's For this Contact.
		// You are only allowed one smart card.
		// Ban all Cards that were created before this card.
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE SMARTCARDS "
		"SET "
		"CARD_STATUS = :NEW_CARD_STATUS,"
		"BACKUP_TIME_STAMP = :BACKUP_TIME_STAMP "
		"WHERE "
		"CARD_STATUS = :OLD_CARD_STATUS AND "
		"CONTACTS_KEY = :CONTACTS_KEY";
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = contactKey;
		IBInternalQuery->ParamByName("BACKUP_TIME_STAMP")->AsDateTime = Now();
		IBInternalQuery->ParamByName("OLD_CARD_STATUS")->AsInteger = SMARTCARD_CARD_STATUS_ACTIVE;
		IBInternalQuery->ParamByName("NEW_CARD_STATUS")->AsInteger = SMARTCARD_CARD_STATUS_BANNED;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

bool TManagerMembershipSmartCards::CardCheckBannedList(Database::TDBTransaction &DBTransaction, int ContactKey, UnicodeString Atr)
{
	bool RetVal = false;
	try
	{
		if (CardBanned(DBTransaction, ContactKey, Atr))
		{
			RetVal = true;
		}
		else if (CardReplaced(DBTransaction, ContactKey, Atr))
		{
			RetVal = true;
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
	return RetVal;
}

bool TManagerMembershipSmartCards::GetSmartCardRestorePointByCard(Database::TDBTransaction &DBTransaction, TSmartCardBlock &RestorePoint)
{
	bool RetVal = false;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		// If there is no Restore Point Create one.
		// Otherwise update the image.
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = " SELECT " " BACKUP " " FROM " "  SMARTCARDS " " WHERE " "  ATR = :ATR AND CARD_STATUS = :CARD_STATUS";
		IBInternalQuery->ParamByName("CARD_STATUS")->AsInteger = SMARTCARD_CARD_STATUS_ACTIVE;
		IBInternalQuery->ParamByName("ATR")->AsString = RestorePoint.CardGuid.GUID();
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount != 0)
		{ // No Card Add it.
			RetVal = true;
			RestorePoint.ClearData();
			RestorePoint.GetStream()->Position = 0;
			IBInternalQuery->FieldByName("BACKUP")->SaveToStream(RestorePoint.GetStream());
			RestorePoint.GetStream()->Position = 0;
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
	return RetVal;
}

bool TManagerMembershipSmartCards::GetSmartCardRestorePointByContact(Database::TDBTransaction &DBTransaction, int ContactKey,
TSmartCardBlock &RestorePoint)
{
	bool RetVal = false;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		// If there is no Restore Point Create one.
		// Otherwise update the image.
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = " SELECT " " * " " FROM " "  SMARTCARDS " " WHERE "
		"  CONTACTS_KEY = :CONTACTS_KEY AND CARD_STATUS = :CARD_STATUS";
		IBInternalQuery->ParamByName("CARD_STATUS")->AsInteger = SMARTCARD_CARD_STATUS_ACTIVE;
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactKey;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount != 0)
		{ // No Card Add it.
			// RestorePoint.ATR = IBInternalQuery->FieldByName("ATR")->AsString;
			RetVal = true;
			IBInternalQuery->FieldByName("BACKUP")->SaveToStream(RestorePoint.GetStream());
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
	return RetVal;
}

bool TManagerMembershipSmartCards::GetSmartCardRestorePointBySmartCardKey(Database::TDBTransaction &DBTransaction, int SmartCardKey,
TSmartCardBlock &RestorePoint)
{
	bool RetVal = false;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		// If there is no Restore Point Create one.
		// Otherwise update the image.
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = " SELECT " " * " " FROM " "  SMARTCARDS " " WHERE " "  SMARTCARDS_KEY = :SMARTCARDS_KEY";
		IBInternalQuery->ParamByName("SMARTCARDS_KEY")->AsInteger = SmartCardKey;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount != 0)
		{
			RetVal = true;
			IBInternalQuery->FieldByName("BACKUP")->SaveToStream(RestorePoint.GetStream());
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
	return RetVal;
}

void TManagerMembershipSmartCards::OnUpDateRestorePoint(TSystemEvents *Sender)
{
	// Back up the Card, always.
	TSmartCardBlock *RestorePoint = ManagerSmartCards->GetRestorePoint();
	if (RestorePoint != NULL && !ManagerSmartCards->CardCorrupt)
	{
		try
		{

			Database::TDBTransaction DBTransaction(DBControl);
			RegisterTransaction(DBTransaction);
			DBTransaction.StartTransaction();
			if (!RestorePoint->CardGuid.Valid())
			{
				TMMContactInfo SmartCardContact;
				ManagerSmartCards->GetContactInfo(SmartCardContact);
				int MemberShipNumber = StrToIntDef(SmartCardContact.MembershipNumber, 0);
				if (MemberShipNumber != 0)
				{
					std::auto_ptr <TfrmProcessing> (frmProcessing)(TfrmProcessing::Create <TfrmProcessing> (Screen->ActiveForm));
					frmProcessing->Message = "Do NOT remove smartcard. Please Wait.";
					frmProcessing->Title = "Do NOT remove smartcard.";
					frmProcessing->Show();

					int CardNumber = GetNextSmartCardNumber(DBTransaction);
					TSmartCardGUID CardGuid;
					CardGuid.Assign(SmartCardContact.SiteID, MemberShipNumber, CardNumber);
					ManagerSmartCards->SaveCardGUID(CardGuid);
				}
				else
				{
					TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG,
					"Unable to Backup Smartcard. Membership Number is Invalid :" + SmartCardContact.MembershipNumber);
				}
			}

			bool isMemberInDatabase = false;

			// This has been switched around so that smart cards are backed up
			// when creating new cards or assigning a new card. Even if a new GUID
			// is required it should be created first.
			if (RestorePoint->CardGuid.Valid())
			{
				// Create a Restore Point for this card.
				TMMContactInfo SmartCardContact;
				ManagerSmartCards->GetContactInfo(SmartCardContact);

				TSyndCode SyndCode;
				ManagerSmartCards->GetSyndCodeInfo(SyndCode);

				SmartCardContact.ContactKey = TDBContacts::GetContactByMemberNumberSiteID(DBTransaction, SmartCardContact.MembershipNumber,SmartCardContact.SiteID);
				/* If its a new Member then while the member has been saved to the DB his transaction hasnt been commited yet.
			    prior to this even firing so no Contact key will be returned. */
				if (SmartCardContact.ContactKey != 0)
				{
					isMemberInDatabase = true;
					SetSmartCardRestorePoint(DBTransaction, SmartCardContact, RestorePoint, SyndCode);
				}
			}
			DBTransaction.Commit();

			if(isMemberInDatabase && TGlobalSettings::Instance().LoyaltyMateEnabled)
			{
				performLoyaltyMateOperations();
			}
		}
		catch(Exception & E)
		{
			TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		}
	}
}

void TManagerMembershipSmartCards::GetSmartCardReportForCardKey(Database::TDBTransaction &DBTransaction, int SmartCardKey,
TStringList *Report)
{
	try
	{
		TSmartCardBlock RestorePoint;
		if (GetSmartCardRestorePointBySmartCardKey(DBTransaction, SmartCardKey, RestorePoint))
		{
			if (!RestorePoint.Empty())
			{
				GetReportCardInfo(DBTransaction,SmartCardKey, &RestorePoint, Report);
			}
		}
		else
		{
			MessageBox("Unable to find a backup image for this card.", "No image point found.", MB_ICONERROR + MB_OK);
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		Report->Add("<span style=\"font-weight: bold; text-decoration: underline;\">Error Generating Report.<br></span>");
		Report->Add("Error in " __FUNC__ "<br>");
		Report->Add("Error :" + E.Message);
	}
}

Currency TManagerMembershipSmartCards::GetValue(Database::TDBTransaction &DBTransaction, int inContactKey)
{
	Currency RetVal = 0;
	try
	{
		if (ManagerSmartCards->Enabled)
		{
			TMMContactInfo ContactInfo;
			MembershipSystem->GetContactDetails(DBTransaction, inContactKey, ContactInfo);
			if (ManagerSmartCards->ContactInfoMatches(ContactInfo))
			{
				ManagerSmartCards->GetContactInfo(ContactInfo);
				RetVal = ContactInfo.Points.getPointsBalance();
			}
			else
			{
				throw TSCException(tsceIncorrectCard, "Incorrect Smart Card in Reader.");
			}
		}
		else
		{
			RetVal = MembershipSystem->LoyaltyGetValue(DBTransaction, inContactKey);
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		MessageBox("Unable to load points information from card.\r" + E.Message, "Error", MB_OK + MB_ICONERROR);
	}
	return RetVal;
}

int TManagerMembershipSmartCards::GetSmartCardStatus(Database::TDBTransaction &DBTransaction, int inContactKey, UnicodeString inAtr)
{
	int RetVal = SMARTCARD_CARD_STATUS_UNKNOWN;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT "
		"CARD_STATUS "
		"FROM "
		"SMARTCARDS "
		"WHERE "
		"ATR = :ATR "
		"AND "
		"CONTACTS_KEY = :CONTACTS_KEY;";
		IBInternalQuery->ParamByName("ATR")->AsString = inAtr;
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount != 0)
		{
			RetVal = IBInternalQuery->FieldByName("CARD_STATUS")->AsInteger;
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}

	return RetVal;
}

void TManagerMembershipSmartCards::BlockNewSmartCard(Database::TDBTransaction &DBTransaction, TMMContactInfo &MMContactInfo, TSyndCode &SyndCode)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_SMARTCARDS, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		int SmartCardKey = IBInternalQuery->Fields[0]->AsInteger;

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"INSERT INTO SMARTCARDS "
		"("
		"SMARTCARDS_KEY,"
		"CONTACTS_KEY,"
		"CARD_STATUS,"
		"CARD_CREATION_DATE,"
		"BACKUP_TIME_STAMP,"
		"BACKUP,"
		"ATR,"
		"SYNDCODES_KEY"
		")"
		" VALUES "
		"("
		":SMARTCARDS_KEY,"
		":CONTACTS_KEY,"
		":CARD_STATUS,"
		":CARD_CREATION_DATE,"
		":BACKUP_TIME_STAMP,"
		":BACKUP,"
		":ATR,"
		":SYNDCODES_KEY"
		");";
		IBInternalQuery->ParamByName("SMARTCARDS_KEY")->AsInteger = SmartCardKey;
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = MMContactInfo.ContactKey;
		IBInternalQuery->ParamByName("CARD_STATUS")->AsInteger = SMARTCARD_CARD_STATUS_BANNED;
		IBInternalQuery->ParamByName("BACKUP_TIME_STAMP")->AsDateTime = Now();
		IBInternalQuery->ParamByName("CARD_CREATION_DATE")->AsDateTime = MMContactInfo.CardCreationDate;
		if (SyndCode.SyndCodeKey <= 0)
		{
			IBInternalQuery->ParamByName("SYNDCODES_KEY")->Clear();
		}
		else
		{
			IBInternalQuery->ParamByName("SYNDCODES_KEY")->AsInteger = SyndCode.SyndCodeKey;
		}
		IBInternalQuery->ParamByName("BACKUP")->Clear();
		IBInternalQuery->ParamByName("ATR")->AsString = MMContactInfo.ATRStr;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

TDateTime TManagerMembershipSmartCards::GetCardCreationDateFromContactKey(Database::TDBTransaction &DBTransaction, int inContactKey)
{
	TDateTime RetVal;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT "
		"CARD_CREATION_DATE "
		"FROM "
		"SMARTCARDS "
		"WHERE "
		"CONTACTS_KEY = :CONTACTS_KEY "
		"AND "
		"CARD_STATUS = :CARD_STATUS;";
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
		IBInternalQuery->ParamByName("CARD_STATUS" )->AsInteger = SMARTCARD_CARD_STATUS_ACTIVE;;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount != 0)
		{
			RetVal = IBInternalQuery->FieldByName("CARD_CREATION_DATE")->AsDateTime;
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}

	return RetVal;
}

void TManagerMembershipSmartCards::OnCardInserted(TSystemEvents *Sender)
{
	try
	{
		ManagerSmartCards->OnCardUpdated.Sleep();
		try
		{
           if(TGlobalSettings::Instance().LoyaltyMateEnabled)
             LoyaltymateCardInsertedHandler(Sender);
           else
             LocalCardInsertedHandler(Sender);
		}
		__finally
		{
			ManagerSmartCards->OnCardUpdated.Wakeup();
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}	//
}

void TManagerMembershipSmartCards::LocalCardInsertedHandler(TSystemEvents *Sender)
{

    MembershipSystem->ResetPoints();
    Database::TDBTransaction DBTransaction(DBControl);
    RegisterTransaction(DBTransaction);
    DBTransaction.StartTransaction();
    TMMContactInfo SmartCardContact;
    ManagerSmartCards->GetContactInfo(SmartCardContact);
    if (TSmartcard_Preloader::is_preloaded_card(SmartCardContact))
    {
        DBTransaction.Commit();
        return;
    }
       // Hold the current smart card last modified time in case there
    // are held points to be saved to the card.
    TDateTime SmartCardLastModified = SmartCardContact.LastModified;
    SmartCardContact.ContactKey = TDBContacts::GetContactByMemberNumberSiteID(DBTransaction, SmartCardContact.MembershipNumber,SmartCardContact.SiteID);

    if (SmartCardContact.ContactKey == 0)
    {
        if(SmartCardContact.MemberType == 0)
        {
           if(SmartCardContact.EMail.Pos("GiftCard") == 1)
            {
              SmartCardContact.MemberType = 2;
            }
            else
            {
               SmartCardContact.MemberType = 1;
            }
        }
        MembershipSystem->SetContactDetails(DBTransaction, SmartCardContact.ContactKey, SmartCardContact);
        DBTransaction.Commit();
        DBTransaction.StartTransaction();
    }
    else
    {
       // Check for deleted status.
        bool CheckForCardInDB 	 = true;
        bool UpdateMemberDetails = false;
        bool CardNewToDB		 = false;

        if (MembershipSystem->GetContactType(DBTransaction, SmartCardContact.ContactKey) == eDeletedMember)
        {
            if (CustomMessageBox("This Member is Deleted.\rDo you wish to undelete them.", "Member is deleted.", MB_ICONQUESTION,
                        "Undelete", "Leave Deleted") == IDOK)
            {
                MembershipSystem->UndeleteContact(DBTransaction, SmartCardContact.ContactKey);
            }
            else
            {
                CheckForCardInDB = false;
            }
        }

        if (CheckForCardInDB)
        {
            switch(GetSmartCardStatus(DBTransaction, SmartCardContact.ContactKey, SmartCardContact.ATRStr))
            {
            case SMARTCARD_CARD_STATUS_BANNED:
                break;
            case SMARTCARD_CARD_STATUS_ACTIVE:
                UpdateMemberDetails = true;
                break;
            case SMARTCARD_CARD_STATUS_UNKNOWN:
            default:
                CardNewToDB = true;
                if (SmartCardContact.CardCreationDate < TDBContacts::GetCardCreationDate(DBTransaction, SmartCardContact.ContactKey))
                {
                    TSyndCode SyndCode;
                    ManagerSmartCards->GetSyndCodeInfo(SyndCode);
                    BlockNewSmartCard(DBTransaction, SmartCardContact, SyndCode);
                }
                else
                {
                    UpdateMemberDetails = true;
                }
                break;
            }
        }
        if (UpdateMemberDetails)
        {
            // Update the Card with Held Points Transactions.
            // This will trigger a card update when the card events system is woken up again.
            TPointsTransactionContainer PointsTransactions;
            GetValidPointsTransactions(DBTransaction, SmartCardContact.ContactKey, PointsTransactions);
            SmartCardContact.Points.Add(PointsTransactions);
            if (PointsTransactions.size() != 0 && SavePointsTransactionsToSmartCard(SmartCardContact.Points,""))
            {
                UpdatePointsTransactions(DBTransaction, PointsTransactions);
            }

            // DB Is More up to date.
            TMMContactInfo TempUserDataBaseInfo;
            // We need to get the Cloud GUID from the smart card in case it is zero and not stored in the database
            MembershipSystem->GetContactDetails(DBTransaction, SmartCardContact.ContactKey, TempUserDataBaseInfo);

            SmartCardContact.TierLevel = TempUserDataBaseInfo.TierLevel;
            SmartCardContact.MemberType = TempUserDataBaseInfo.MemberType;
            SmartCardContact.LastVisit =  TempUserDataBaseInfo.LastVisit;
            SmartCardContact.LastBirthdayProcessed = TempUserDataBaseInfo.LastBirthdayProcessed;
            SmartCardContact.DateOfBirth = TempUserDataBaseInfo.DateOfBirth;
            SmartCardContact.IsFirstVisitRewarded = TempUserDataBaseInfo.IsFirstVisitRewarded;
            MembershipSystem->SetContactDetails(DBTransaction, SmartCardContact.ContactKey, SmartCardContact);
            if (TempUserDataBaseInfo.LastModified > SmartCardLastModified && !CardNewToDB)
            {
                // Save any discounts applied in the DB to the cards.
                ManagerDiscount->DiscountKeyToCode(DBTransaction, SmartCardContact.AutoAppliedDiscountsID,
                TempUserDataBaseInfo.AutoAppliedDiscounts);
                SaveContactInfoEditedToSmartCard(TempUserDataBaseInfo);
                SaveContactInfoOnDownload(DBTransaction,SmartCardContact);
            }
            else if (TempUserDataBaseInfo.LastModified < SmartCardLastModified || CardNewToDB)
            {
                // Preserve and Discounts on the Card.
                ManagerDiscount->DiscountCodeToKey(DBTransaction, SmartCardContact.AutoAppliedDiscountsID,
                SmartCardContact.AutoAppliedDiscounts);
            }

        }
    }

    DBTransaction.Commit();
}

void TManagerMembershipSmartCards::LoyaltymateCardInsertedHandler(TSystemEvents *Sender)
{
    MembershipSystem->ResetPoints();
    bool addDefaultPoints = false;
    bool memberNotExist = false ;

    MemberMode memberMode = eSmartCardMode;
    Database::TDBTransaction DBTransaction(DBControl);
    RegisterTransaction(DBTransaction);
    DBTransaction.StartTransaction();
    TMMContactInfo SmartCardContact;
    ManagerSmartCards->GetContactInfo(SmartCardContact);
    if (TSmartcard_Preloader::is_preloaded_card(SmartCardContact))
    {
        DBTransaction.Commit();
        return;
    }
    TContactPoints pointsToSync = SmartCardContact.Points;
    TDateTime SmartCardLastModified = SmartCardContact.LastModified;

    SmartCardContact.ContactKey = TDBContacts::GetContactByMemberNumberSiteID(DBTransaction, SmartCardContact.MembershipNumber,SmartCardContact.SiteID);
    bool existInLocalDb = SmartCardContact.ContactKey != 0;
    bool smartCardHasUUID = TLoyaltyMateUtilities::IsLoyaltyMateEnabledGUID(SmartCardContact.CloudUUID);
    if (smartCardHasUUID)
    {
       GetMemberDetail(SmartCardContact);
       if(!existInLocalDb)
       {
          SmartCardContact.ContactKey =  TDBContacts::GetContactByEmail(DBTransaction,SmartCardContact.EMail);
          existInLocalDb = SmartCardContact.ContactKey != 0;;
       }
       else
       {
         ValidateCardExistance(DBTransaction,SmartCardContact);
       }
    }
    else
    {
       //check that card is used here or not
       if(!existInLocalDb)
       {
            if(SmartCardContact.EMail == "" || SmartCardContact.EMail == NULL)
            {
               SmartCardContact.EMail = GetActivationEmailFromUser();
            }
            if(SmartCardContact.EMail != "" && SmartCardContact.EMail != NULL)
            {
                int localEmailContactKey = TDBContacts::GetContactByEmail(DBTransaction,SmartCardContact.EMail);
                if(localEmailContactKey != 0)
                {
                    LinkSmartCard(DBTransaction,localEmailContactKey,SmartCardContact);
                    SmartCardContact.ContactKey = localEmailContactKey;
                    TDBContacts::GetContactDetails(DBTransaction,localEmailContactKey,SmartCardContact);
                    existInLocalDb = true;
                }
                memberNotExist = GetMemberDetailFromEmail(SmartCardContact);
            }
        }
        else
        {
          //check here that card has ever inserted in this site
          int registerdContactKey = ValidateCardExistanceUsingUUID(DBTransaction,SmartCardContact);
          if(registerdContactKey == 0)
          {
            if(SmartCardContact.EMail == "" || SmartCardContact.EMail == NULL)
            {
               SmartCardContact.EMail = GetActivationEmailFromUser();
            }
            if(SmartCardContact.EMail != "" && SmartCardContact.EMail != NULL)
            {
                int localEmailContactKey = TDBContacts::GetContactByEmail(DBTransaction,SmartCardContact.EMail,SmartCardContact.ContactKey);
                if(localEmailContactKey != 0)
                {
                    LinkSmartCard(DBTransaction,localEmailContactKey,SmartCardContact);
                    TDBContacts::GetContactDetails(DBTransaction,localEmailContactKey,SmartCardContact);
                }
               memberNotExist = GetMemberDetailFromEmail(SmartCardContact);
            }
          }
          else
          {
            LinkSmartCard(DBTransaction,registerdContactKey,SmartCardContact);
            TDBContacts::GetContactDetails(DBTransaction,registerdContactKey,SmartCardContact);
            GetMemberDetail(SmartCardContact);
          }
        }
    }

    if (!existInLocalDb)
    {
        bool immediatelyBlockCard = false;
        if (TLoyaltyMateUtilities::IsLoyaltyMateEnabledGUID(SmartCardContact.CloudUUID))
        {
            int existingContactKey;
            int existingSiteId;
            TLoyaltyMateUtilities::GetContactByCloudUUID(DBTransaction, SmartCardContact.CloudUUID, existingContactKey, existingSiteId);
            if (existingContactKey != 0)
            {
                TDateTime existingCardCreationDate = GetCardCreationDateFromContactKey(DBTransaction, existingContactKey);
                if (SmartCardContact.CardCreationDate >= existingCardCreationDate)
                {
                    TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->DeleteContact(DBTransaction, existingContactKey);
                    BlockAllOfContactsSmartCards(DBTransaction, existingContactKey);
                }
                else
                {
                    immediatelyBlockCard = true;
                }
            }
        }

        if (immediatelyBlockCard)
        {
            TSyndCode SyndCode;
            ManagerSmartCards->GetSyndCodeInfo(SyndCode);
            BlockNewSmartCard(DBTransaction, SmartCardContact, SyndCode);
        }
        else
        {
           if(SmartCardContact.MemberType == 0)
            {
               if(SmartCardContact.EMail.Pos("GiftCard") == 1)
                {
                  SmartCardContact.MemberType = 2;
                }
                else
                {
                   SmartCardContact.MemberType = 1;
                }
            }
            MembershipSystem->SetContactDetails(DBTransaction, SmartCardContact.ContactKey, SmartCardContact);
            DBTransaction.Commit();
            if (!TGlobalSettings::Instance().IsPOSOffline)
            {
               SavePointsTransactionsToSmartCard(SmartCardContact.Points,"",true);
               SaveContactInfoEditedToSmartCard(SmartCardContact);
            }
            DBTransaction.StartTransaction();
            if(TLoyaltyMateUtilities::IsLoyaltyMateEnabledGUID(SmartCardContact.CloudUUID))
               MembershipSystem->SetContactLoyaltyAttributes(DBTransaction, SmartCardContact.ContactKey, SmartCardContact);
        }
    }
    else
    {
       if (!TGlobalSettings::Instance().IsPOSOffline && !memberNotExist)
        {
           if(SmartCardContact.Surname != "")
           {
             MembershipSystem->SetContactDetails(DBTransaction, SmartCardContact.ContactKey, SmartCardContact);
             if(TLoyaltyMateUtilities::IsLoyaltyMateEnabledGUID(SmartCardContact.CloudUUID))
               MembershipSystem->SetContactLoyaltyAttributes(DBTransaction, SmartCardContact.ContactKey, SmartCardContact);
           }
           SavePointsTransactionsToSmartCard(SmartCardContact.Points,"",true);
        }
       // Check for deleted status.
        bool CheckForCardInDB 	 = true;
        bool UpdateMemberDetails = false;
        bool CardNewToDB		 = false;

        if (MembershipSystem->GetContactType(DBTransaction, SmartCardContact.ContactKey) == eDeletedMember)
        {
            if (CustomMessageBox("This Member is Deleted.\rDo you wish to undelete them.", "Member is deleted.", MB_ICONQUESTION,
                        "Undelete", "Leave Deleted") == IDOK)
            {
                MembershipSystem->UndeleteContact(DBTransaction, SmartCardContact.ContactKey);
            }
            else
            {
                CheckForCardInDB = false;
            }
        }

        if (CheckForCardInDB)
        {
            switch(GetSmartCardStatus(DBTransaction, SmartCardContact.ContactKey, SmartCardContact.ATRStr))
            {
            case SMARTCARD_CARD_STATUS_BANNED:
                break;
            case SMARTCARD_CARD_STATUS_ACTIVE:
                UpdateMemberDetails = true;
                break;
            case SMARTCARD_CARD_STATUS_UNKNOWN:
            default:
                CardNewToDB = true;
                if (SmartCardContact.CardCreationDate < TDBContacts::GetCardCreationDate(DBTransaction, SmartCardContact.ContactKey))
                {
                    TSyndCode SyndCode;
                    ManagerSmartCards->GetSyndCodeInfo(SyndCode);
                    BlockNewSmartCard(DBTransaction, SmartCardContact, SyndCode);
                }
                else
                {
                    UpdateMemberDetails = true;
                }
                break;
            }
        }
        if (UpdateMemberDetails)
        {
            // Update the Card with Held Points Transactions.
            // This will trigger a card update when the card events system is woken up again.
            TPointsTransactionContainer PointsTransactions;
            GetValidPointsTransactions(DBTransaction, SmartCardContact.ContactKey, PointsTransactions);
            SmartCardContact.Points.Add(PointsTransactions);
            if (PointsTransactions.size() != 0 && SavePointsTransactionsToSmartCard(SmartCardContact.Points,""))
            {
                UpdatePointsTransactions(DBTransaction, PointsTransactions);
            }

            // DB Is More up to date.
            TMMContactInfo TempUserDataBaseInfo;
            MembershipSystem->GetContactDetails(DBTransaction, SmartCardContact.ContactKey, TempUserDataBaseInfo);
            TempUserDataBaseInfo.CloudUUID = SmartCardContact.CloudUUID;

            //Assign the tier level to smart card, for safer side lets not do a blunder by letting it remain 0
            if(SmartCardContact.TierLevel < TempUserDataBaseInfo.TierLevel)
                   SmartCardContact.TierLevel = TempUserDataBaseInfo.TierLevel;

            if(SmartCardContact.LastVisit < TempUserDataBaseInfo.LastVisit)
                   SmartCardContact.LastVisit = TempUserDataBaseInfo.LastVisit;

            if(SmartCardContact.LastBirthdayProcessed < TempUserDataBaseInfo.LastBirthdayProcessed)
                   SmartCardContact.LastBirthdayProcessed = TempUserDataBaseInfo.LastBirthdayProcessed;

            // Save any discounts applied in the DB to the cards.
            SaveContactInfoEditedToSmartCard(TempUserDataBaseInfo);
            SaveContactInfoOnDownload(DBTransaction,SmartCardContact);
        }
    }

   if(!smartCardHasUUID && TLoyaltyMateUtilities::IsLoyaltyMateEnabledGUID(SmartCardContact.CloudUUID))
   {
     AddDefaultPoints(DBTransaction,pointsToSync,SmartCardContact.ContactKey);
      TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, "Added Default Entry --- CardInsertedhandler");
   }
   DBTransaction.Commit();
}

void TManagerMembershipSmartCards::performLoyaltyMateOperations()
{
	bool smartCardUpdateRequired = false;
	bool storeCloudUUIDInDB = false;
    bool memberCreationSuccess = false;
	TMMContactInfo SmartCardContact;
	ManagerSmartCards->GetContactInfo(SmartCardContact);
    TContactPoints Points =  SmartCardContact.Points;
	TSyndCode currentSyndicateCode = ManagerSyndicateCode.GetCommunicationSyndCode();

	Database::TDBTransaction DBTransaction(DBControl);
	RegisterTransaction(DBTransaction);
	DBTransaction.StartTransaction();

	SmartCardContact.ContactKey = TDBContacts::GetContactByMemberNumberSiteID(DBTransaction, SmartCardContact.MembershipNumber,SmartCardContact.SiteID);
    if(SmartCardContact.EMail != "" && SmartCardContact.EMail != NULL)
       SmartCardContact.ContactKey =  TDBContacts::GetContactByEmail(DBTransaction,SmartCardContact.EMail);
	AnsiString DbUUID = TLoyaltyMateUtilities::GetMemberCloudIdIfRegistered(DBTransaction, SmartCardContact.ContactKey, SmartCardContact.SiteID);
    TDBContacts::GetContactDetails(DBTransaction,SmartCardContact.ContactKey,SmartCardContact);
    SmartCardContact.Points = Points;

	if(SmartCardContact.CloudUUID.Length() == 0 && TGlobalSettings::Instance().LoyaltyMateEnabled)
	{
       AnsiString message = "For Loyaltymate, you will need to update your ";
       bool updateMember = false;
       if(SmartCardContact.ValidateMandatoryField(message))
       {
          updateMember = true;
       }
       else
       {
           MessageBox(message.SubString(1, message.Length()-1) + ".", "Message", MB_ICONINFORMATION + MB_OK);
           std::auto_ptr < TfrmEditCustomer >
           frmEditCustomer(TfrmEditCustomer::Create(Screen->ActiveForm));
           frmEditCustomer->Editing = true;
           frmEditCustomer->Info = SmartCardContact;
           frmEditCustomer->MemberType = SmartCardContact.MemberType;
           if(frmEditCustomer->ShowModal() == mrOk)
           {
              updateMember = true;
              TContactPoints points = SmartCardContact.Points;
              SmartCardContact = frmEditCustomer->Info;
              SmartCardContact.Points =  points;
              MembershipSystem->SetContactDetails(DBTransaction, SmartCardContact.ContactKey, SmartCardContact);
              DBTransaction.Commit();
              DBTransaction.StartTransaction();
           }
       }

       if(updateMember)
       {
           if(SmartCardContact.EMail.Pos("GiftCard") == 1)
            {
              SmartCardContact.MemberType = 2;
            }
            else
            {
               SmartCardContact.MemberType = 1;
            }

            memberCreationSuccess = createMemberOnLoyaltyMate(currentSyndicateCode,SmartCardContact);
            if(memberCreationSuccess && TLoyaltyMateUtilities::IsLoyaltyMateEnabledGUID(SmartCardContact.CloudUUID))
            {
                smartCardUpdateRequired = true;
                storeCloudUUIDInDB = true;
            }
       }
	}
    else
    {
        storeCloudUUIDInDB = true;
    }

	if(storeCloudUUIDInDB)
	{
		TLoyaltyMateUtilities::SetMemberCloudId(DBTransaction,SmartCardContact.ContactKey,SmartCardContact.SiteID,
		SmartCardContact.CloudUUID,SmartCardContact.ActivationToken);
	}

    if(memberCreationSuccess)
    {
       AddDefaultPoints(DBTransaction,Points,SmartCardContact.ContactKey);
        TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, "Added Default Entry --- Loyaltymate operation");
    }

	if(smartCardUpdateRequired)
	{
		// update the membership's contact details. performing the operation here
		// because as soon as we update the card it will throw events which prevents us from committing the transaction
		MembershipSystem->SetContactDetails(DBTransaction, SmartCardContact.ContactKey, SmartCardContact);
	}

	DBTransaction.Commit();

	if(smartCardUpdateRequired)
	  SaveContactInfoEditedToSmartCard(SmartCardContact);

	/* we execute this operation if we don't have a pending smart card update.
	* because as soon as we update the smart card it will fire events which brings us to this same piece of code twice.
	* to prevent calling the cloud two times, this condition need to be added
	*/
	if(!smartCardUpdateRequired && TLoyaltyMateUtilities::IsLoyaltyMateEnabledGUID(SmartCardContact.CloudUUID))
	{
		DBTransaction.StartTransaction();
		TDBContacts::GetContactDetails(DBTransaction,SmartCardContact.ContactKey,SmartCardContact);
		DBTransaction.Commit();
		// sync member information with cloud
		TManagerLoyaltyMate::Instance()->SyncMemberDetailsWithCloud(currentSyndicateCode,SmartCardContact);
	}
}

void TManagerMembershipSmartCards::SetSmartCardRestorePoint(Database::TDBTransaction &DBTransaction, TMMContactInfo &MMContactInfo,
TSmartCardBlock *RestorePoint, TSyndCode &SyndCode)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		// If there is no Restore Point Create one.
		// Then create a backup image of this card.
		// Otherwise update the image.
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = " SELECT SMARTCARDS_KEY FROM SMARTCARDS WHERE ATR = :ATR AND CONTACTS_KEY = :CONTACTS_KEY";
		IBInternalQuery->ParamByName("ATR")->AsString = RestorePoint->CardGuid.GUID();
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = MMContactInfo.ContactKey;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->FieldByName("SMARTCARDS_KEY")->AsInteger == 0)
		{ // No Card Add it.
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_SMARTCARDS, 1) FROM RDB$DATABASE";
			IBInternalQuery->ExecQuery();
			int SmartCardKey = IBInternalQuery->Fields[0]->AsInteger;

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
			"INSERT INTO SMARTCARDS (" "SMARTCARDS_KEY," "CONTACTS_KEY," "CARD_STATUS,"
			"CARD_CREATION_DATE," "BACKUP_TIME_STAMP," "BACKUP," "ATR," "SYNDCODES_KEY) " "VALUES ("
			":SMARTCARDS_KEY," ":CONTACTS_KEY," ":CARD_STATUS," ":CARD_CREATION_DATE," ":BACKUP_TIME_STAMP," ":BACKUP,"
			":ATR," ":SYNDCODES_KEY);";
			IBInternalQuery->ParamByName("SMARTCARDS_KEY")->AsInteger = SmartCardKey;
			IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = MMContactInfo.ContactKey;
			IBInternalQuery->ParamByName("CARD_STATUS")->AsInteger = SMARTCARD_CARD_STATUS_ACTIVE;
			IBInternalQuery->ParamByName("BACKUP_TIME_STAMP")->AsDateTime = Now();
			IBInternalQuery->ParamByName("CARD_CREATION_DATE")->AsDateTime = MMContactInfo.CardCreationDate;
			if (SyndCode.SyndCodeKey <= 0)
			{
				IBInternalQuery->ParamByName("SYNDCODES_KEY")->Clear();
			}
			else
			{
				IBInternalQuery->ParamByName("SYNDCODES_KEY")->AsInteger = SyndCode.SyndCodeKey;
			}
			RestorePoint->GetStream()->Position = 0;
			RestorePoint->GetStream()->SaveToFile("MMRestorePointSavedToDB.bin");
			RestorePoint->GetStream()->Position = 0;
			IBInternalQuery->ParamByName("BACKUP")->LoadFromStream(RestorePoint->GetStream());
			IBInternalQuery->ParamByName("ATR")->AsString = RestorePoint->CardGuid.GUID();
			IBInternalQuery->ExecQuery();

			// Ban all other ATR's For this Contact.
			// You are only allowed one smart card.
			// Ban all Cards that were created before this card.
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "UPDATE SMARTCARDS " "SET " "CARD_STATUS = :NEW_CARD_STATUS,"
			"BACKUP_TIME_STAMP = :BACKUP_TIME_STAMP " "WHERE " " ATR != :ATR AND " " CARD_STATUS = :OLD_CARD_STATUS AND "
			" CONTACTS_KEY = :CONTACTS_KEY AND " " CARD_CREATION_DATE < :CARD_CREATION_DATE";
			IBInternalQuery->ParamByName("ATR")->AsString = RestorePoint->CardGuid.GUID();
			IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = MMContactInfo.ContactKey;
			IBInternalQuery->ParamByName("BACKUP_TIME_STAMP")->AsDateTime = Now();
			IBInternalQuery->ParamByName("CARD_CREATION_DATE")->AsDateTime = MMContactInfo.CardCreationDate;
			IBInternalQuery->ParamByName("OLD_CARD_STATUS")->AsInteger = SMARTCARD_CARD_STATUS_ACTIVE;
			IBInternalQuery->ParamByName("NEW_CARD_STATUS")->AsInteger = SMARTCARD_CARD_STATUS_BANNED;
			IBInternalQuery->ExecQuery();
		}
		else
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "UPDATE SMARTCARDS SET SYNDCODES_KEY = :SYNDCODES_KEY,BACKUP = :BACKUP, "
			"BACKUP_TIME_STAMP = :BACKUP_TIME_STAMP WHERE ATR = :ATR AND CONTACTS_KEY = :CONTACTS_KEY";
			IBInternalQuery->ParamByName("ATR")->AsString = RestorePoint->CardGuid.GUID();
			IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = MMContactInfo.ContactKey;
			IBInternalQuery->ParamByName("BACKUP_TIME_STAMP")->AsDateTime = Now();
			if (SyndCode.SyndCodeKey <= 0)
			{
				IBInternalQuery->ParamByName("SYNDCODES_KEY")->Clear();
			}
			else
			{
				IBInternalQuery->ParamByName("SYNDCODES_KEY")->AsInteger = SyndCode.SyndCodeKey;
			}
			RestorePoint->GetStream()->Position = 0;
			RestorePoint->GetStream()->SaveToFile("MMRestorePointSavedToDB.bin");
			RestorePoint->GetStream()->Position = 0;
			IBInternalQuery->ParamByName("BACKUP")->LoadFromStream(RestorePoint->GetStream());
			IBInternalQuery->ExecQuery();
		}

		// Create Another Unique Backup of this card with a Backup ATR.
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_SMARTCARDS, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		int SmartCardKey = IBInternalQuery->Fields[0]->AsInteger;

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"INSERT INTO SMARTCARDS (SMARTCARDS_KEY,CONTACTS_KEY,CARD_STATUS,"
		"CARD_CREATION_DATE,BACKUP_TIME_STAMP,BACKUP,ATR,SYNDCODES_KEY) VALUES (" ":SMARTCARDS_KEY,:CONTACTS_KEY,:CARD_STATUS,:CARD_CREATION_DATE,:BACKUP_TIME_STAMP,:BACKUP," ":ATR,:SYNDCODES_KEY);";
		IBInternalQuery->ParamByName("SMARTCARDS_KEY")->AsInteger = SmartCardKey;
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = MMContactInfo.ContactKey;
		IBInternalQuery->ParamByName("CARD_STATUS")->AsInteger = SMARTCARD_CARD_STATUS_BACKUP;
		IBInternalQuery->ParamByName("BACKUP_TIME_STAMP")->AsDateTime = Now();
		IBInternalQuery->ParamByName("CARD_CREATION_DATE")->AsDateTime = MMContactInfo.CardCreationDate;
		if (SyndCode.SyndCodeKey <= 0)
		{
			IBInternalQuery->ParamByName("SYNDCODES_KEY")->Clear();
		}
		else
		{
			IBInternalQuery->ParamByName("SYNDCODES_KEY")->AsInteger = SyndCode.SyndCodeKey;
		}
		RestorePoint->GetStream()->Position = 0;
		RestorePoint->GetStream()->SaveToFile("MMRestorePointSavedToDB.bin");
		RestorePoint->GetStream()->Position = 0;
		IBInternalQuery->ParamByName("BACKUP")->LoadFromStream(RestorePoint->GetStream());
		IBInternalQuery->ParamByName("ATR")->AsString = RestorePoint->CardGuid.GUIDBackups();
		IBInternalQuery->ExecQuery();
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

bool TManagerMembershipSmartCards::SavePointsTransactionsToSmartCard(TContactPoints &Points,AnsiString inInvoiceNumber,bool PointsFromCloud)
{
	bool Complete = false;
	try
	{
		if (ManagerSmartCards->Enabled)
		{
			/* If they havnt got the card inserted,
		Dont mark these points as exported and export them next time
		*/
			if (ManagerSmartCards->CardOk && (Points.HasTransactions() || Points.HasHeldTransactions()))
			{
				bool Abort = false;

				// Dont trigger anything.
				ManagerSmartCards->CardReaderEventsOff();

				do
				{
					try
					{
						// prior to save this information on to the card add the transaction record to database for Loyaltymate uploader
						Database::TDBTransaction DBTransaction(DBControl);
						RegisterTransaction(DBTransaction);
						DBTransaction.StartTransaction();

						TMMContactInfo SmartCardContact;
						ManagerSmartCards->GetContactInfo(SmartCardContact);
						SmartCardContact.ContactKey = TDBContacts::GetContactByMemberNumberSiteID(DBTransaction, SmartCardContact.MembershipNumber,SmartCardContact.SiteID);
                        UnicodeString UUID = TDBContacts::GetMemberCloudId(DBTransaction,SmartCardContact.ContactKey);
                         if(TGlobalSettings::Instance().LoyaltyMateEnabled
                             && TLoyaltyMateUtilities::IsLoyaltyMateEnabledGUID(UUID)
                             && !PointsFromCloud)
                                {
                                    TLoyaltyMateUtilities::SetLoyaltymateTransactions(
                                    DBTransaction,
                                    SmartCardContact.ContactKey,
                                    ManagerSyndicateCode.GetCommunicationSyndCode(),
                                    Points,
                                    inInvoiceNumber);
                                    TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, "Added Default Entry --- SavePointsTransactionsToSmartCard");
                                }

						BeginMemberTransaction();
						ManagerSmartCards->SaveContactPoints(Points);
						EndMemberTransaction();

						DBTransaction.Commit(); // commiting the transaction here so the record will be saved in the database only if they are written on to the card

						Complete = true;
						Points.setExportStatus(pesExported);
						Points.TimeStampExported = Now();
					}
					catch(TSCException & E)
					{
						if (E.Type == tsceNoCard)
						{
							TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
							if (CustomMessageBox("Failed to Save to Smart Card.\r" + E.Message, "Failed to Save to Smart Card.", MB_ICONQUESTION,
										"Retry", "Dont Update Card") == IDOK)
							{
								Abort = false;
								Complete = false;
							}
							else
							{
								Abort = true;
							}
						}
						else if( E.Type == tsceToMuchCardData )
						{
							TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
							if (CustomMessageBox("Too much information on the Smart Card. \r Please reduce the length of fields on Smart Card.\r" + E.Message, "Failed to Save to Smart Card.", MB_ICONQUESTION,
										"Retry", "Dont update card") == IDOK)
							{
								Abort = false;
								Complete = false;
							}
							else
							{
								Abort = true;
							}
						}
						else
						{
							TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
							if (CustomMessageBox("Failed to Save to Smart Card.\r" + E.Message, "Failed to Save to Smart Card.", MB_ICONQUESTION,
										"Retry", "Dont update card") == IDOK)
							{
								Abort = false;
								Complete = false;
							}
							else
							{
								Abort = true;
							}
						}
					}
				}
				while (Complete == false && Abort == false);
				// Dont trigger anything.
				ManagerSmartCards->CardReaderEventsOn();
				if (Abort)
				{
					ManagerSmartCards->SmartCardRemoved();
				}
			}
			else
			{
				Points.setExportStatus(pesNone);
				Points.TimeStampExported = Now();
				Complete = true;
			}
		}
		else
		{ // We are not useing Smart cards so Export them.
			Points.setExportStatus(pesExported);
			Points.TimeStampExported = Now();
			Complete = true;
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		MessageBox("Unable to save points changes to card.\r" + E.Message, "Error", MB_OK + MB_ICONERROR);
	}
	return Complete;
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//Save Info to card
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void TManagerMembershipSmartCards::SaveContactInfoEditedToSmartCard(TMMContactInfo &inContactInfo)
{
	bool Abort = false;
	bool Complete = false;
	if (ManagerSmartCards->Enabled && ManagerSmartCards->CardInserted)
	{
		// Dont trigger anything.
		ManagerSmartCards->CardReaderEventsOff();
		do
		{
			try
			{
				if (ManagerSmartCards->ContactInfoMatches(inContactInfo) || ManagerSmartCards->CardBlank)
				{
					std::auto_ptr <TfrmProcessing> (frmProcessing)(TfrmProcessing::Create <TfrmProcessing> (Screen->ActiveForm));
					frmProcessing->Message = "Do NOT remove smartcard. Please Wait.";
					frmProcessing->Title = "Do NOT remove smartcard.";
					frmProcessing->Show();
					frmProcessing->Repaint();

					BeginMemberTransaction();
					ManagerSmartCards->SaveContactInfo(inContactInfo);

					EndMemberTransaction();
					Complete = true;
				}
				else
				{
					if (CustomMessageBox("Failed to Save to Smart Card.\rIncorrect Smart Card in Reader.", "Failed to Save to Smart Card.",
								MB_ICONQUESTION, "Retry", "Dont Issue Card") == IDOK)
					{
						Abort = false;
						Complete = false;
					}
					else
					{
						Abort = true;
						inContactInfo.CardCreationDate = 0;
					}
				}

			}
			catch(TSCException & E)
			{
				if (E.Type == tsceToMuchCardData)
				{
					MessageBox("The Smartcard cannot hold this much information.\rReduce the size of your fields.",
					"Failed to Save to Smart Card.", MB_OK + MB_ICONERROR);
					Abort = true;
					inContactInfo.CardCreationDate = 0;
				}
				else
				{
					TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
					if (CustomMessageBox("Failed to Save to Smart Card.\r" + E.Message, "Failed to Save to Smart Card.", MB_ICONQUESTION,
								"Retry", "Dont Issue Card") == IDOK)
					{
						Abort = false;
						Complete = false;
					}
					else
					{
						Abort = true;
						inContactInfo.CardCreationDate = 0;
					}
				}
			}
			catch(Exception & E)
			{
				TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
				if (CustomMessageBox("Failed to Save to Smart Card.\r" + E.Message, "Failed to Save to Smart Card.", MB_ICONQUESTION, "Retry",
							"Dont Issue Card") == IDOK)
				{
					Abort = false;
					Complete = false;
				}
				else
				{
					Abort = true;
				}
			}
		}
		while (Complete == false && Abort == false);

		ManagerSmartCards->CardReaderEventsOn();
		if (Abort)
		{
			ManagerSmartCards->SmartCardRemoved();
		}
	}
}

void TManagerMembershipSmartCards::SaveContactInfoAddedToSmartCard(
TMMContactInfo &inContactInfo,
bool can_overwrite_already_initialized_card)
{
	bool Abort = false;
	bool Complete = false;

	if (ManagerSmartCards->CardInserted)
	{
		// Dont trigger anything.
		ManagerSmartCards->CardReaderEventsOff();

		do
		{
			try
			{
				if (ManagerSmartCards->CardBlank
						| can_overwrite_already_initialized_card)
				{
					if (ManagerSmartCards->GetSyndCodeValid())
					{
						std::auto_ptr <TfrmProcessing> (frmProcessing)(TfrmProcessing::Create <TfrmProcessing> (Screen->ActiveForm));
						frmProcessing->Message = "Do NOT remove smartcard. Please Wait.";
						frmProcessing->Title = "Do NOT remove smartcard.";
						frmProcessing->Show();
						frmProcessing->Repaint();

						BeginMemberTransaction();
						ManagerSmartCards->SaveContactInfo(inContactInfo);
						EndMemberTransaction();
						Complete = true;
					}
					else
					{
						MessageBox("There is no default syndicate code set. Please set one before trying to save to the smart card.",
						"Failed to Save to Smart Card.", MB_OK + MB_ICONERROR);
						Abort = true;
						inContactInfo.CardCreationDate = 0;
					}
				}
				else
				{
					if (CustomMessageBox("Failed to Save to Smart Card.\rCard in Reader is not blank.", "Failed to Save to Smart Card.",
								MB_ICONQUESTION, "Retry", "Dont Issue Card") == IDOK)
					{
						Abort = false;
						Complete = false;
					}
					else
					{
						Abort = true;
						inContactInfo.CardCreationDate = 0;
					}
				}

			}
			catch(TSCException & E)
			{
				if (E.Type == tsceToMuchCardData)
				{
					MessageBox("The Smartcard cannot hold this much information.\rReduce the size of your fields.",
					"Failed to Save to Smart Card.", MB_OK + MB_ICONERROR);
					Abort = true;
					inContactInfo.CardCreationDate = 0;
				}
				else
				{
					TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
					if (CustomMessageBox("Failed to Save to Smart Card.\r" + E.Message, "Failed to Save to Smart Card.", MB_ICONQUESTION,
								"Retry", "Dont Issue Card") == IDOK)
					{
						Abort = false;
						Complete = false;
					}
					else
					{
						Abort = true;
						inContactInfo.CardCreationDate = 0;
					}
				}
			}
			catch(Exception & E)
			{
				TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
				if (CustomMessageBox("Failed to Save to Smart Card.\r" + E.Message, "Failed to Save to Smart Card.", MB_ICONQUESTION, "Retry",
							"Dont Issue Card") == IDOK)
				{
					Abort = false;
					Complete = false;
				}
				else
				{
					Abort = true;
				}
			}
		}
		while (Complete == false && Abort == false);

		ManagerSmartCards->CardReaderEventsOn();
		if (Abort)
		{
			ManagerSmartCards->SmartCardRemoved();
		}
	}
}

void TManagerMembershipSmartCards::SaveContactInfoReassignedToSmartCard(TMMContactInfo &inContactInfo)
{
	bool Abort = false;
	bool Complete = false;

	if (ManagerSmartCards->CardInserted)
	{
		// Dont trigger anything.
		ManagerSmartCards->CardReaderEventsOff();

		do
		{
			try
			{
				bool Proceed = true;
				if (!ManagerSmartCards->CardBlank)
				{
					TMMContactInfo SmartCardContact;
					ManagerSmartCards->GetContactInfo(SmartCardContact);
					if (CustomMessageBox("Are you sure you wish to replace " + SmartCardContact.Name + " (" +
								SmartCardContact.MembershipNumber + ") With " + inContactInfo.Name + " (" +
								inContactInfo.MembershipNumber + ")", "Are you sure?", MB_ICONQUESTION, "Replace", "Abort") != IDOK)
					{
						Proceed = false;
					}
				}

				if (Proceed)
				{
					if (ManagerSmartCards->GetSyndCodeValid())
					{
						std::auto_ptr <TfrmProcessing> (frmProcessing)(TfrmProcessing::Create <TfrmProcessing> (Screen->ActiveForm));
						frmProcessing->Message = "Do NOT remove smartcard. Please Wait.";
						frmProcessing->Title = "Do NOT remove smartcard.";
						frmProcessing->Show();
						frmProcessing->Repaint();

						BeginMemberTransaction();
						inContactInfo.CardCreationDate = Now();
						ManagerSmartCards->SaveContactInfo(inContactInfo);
						ManagerSmartCards->SaveContactPoints(inContactInfo.Points);
						EndMemberTransaction();
						Complete = true;
					}
					else
					{
						MessageBox("There is no default syndicate code set. Please set one before trying to save to the smart card.",
						"Failed to Save to Smart Card.", MB_OK + MB_ICONERROR);
						Abort = true;
					}
				}
				else
				{
					MessageBox("Operation Aborted.\r.", "Save to Smart Card Aborted.", MB_OK + MB_ICONERROR);
					Abort = true;
				}
			}
			catch(TSCException & E)
			{
				if (E.Type == tsceToMuchCardData)
				{
					MessageBox("The Smartcard cannot hold this much information.\rReduce the size of your fields.",
					"Failed to Save to Smart Card.", MB_OK + MB_ICONERROR);
					Abort = true;
					inContactInfo.CardCreationDate = 0;
				}
				else
				{
					TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
					if (CustomMessageBox("Failed to Save to Smart Card.\r" + E.Message, "Failed to Save to Smart Card.", MB_ICONQUESTION,
								"Retry", "Dont Issue Card") == IDOK)
					{
						Abort = false;
						Complete = false;
					}
					else
					{
						Abort = true;
						inContactInfo.CardCreationDate = 0;
					}
				}
			}
			catch(Exception & E)
			{
				TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
				if (CustomMessageBox("Failed to Save to Smart Card.\r" + E.Message, "Failed to Save to Smart Card.", MB_ICONQUESTION, "Retry",
							"Dont Issue Card") == IDOK)
				{
					Abort = false;
					Complete = false;
				}
				else
				{
					Abort = true;
				}
			}
		}
		while (Complete == false && Abort == false);

		ManagerSmartCards->CardReaderEventsOn();
		if (Abort)
		{
			ManagerSmartCards->SmartCardRemoved();
		}
	}
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//Member Operation
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

TManagerSyndCode& TManagerMembershipSmartCards::GetSyndicateCodeManager()
{
	return ManagerSyndicateCode;
}

TLoginSuccess TManagerMembershipSmartCards::GetMember(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo,
eMemberSource &MemberSource, bool AllowDB)
{
	TLoginSuccess LoginSuccess = lsUserNotFound;
	try
	{
        if(UserInfo.MemberCode != "" && UserInfo.ContactKey != 0)
        {
            MemberSource = emsBarcode;
			LoginSuccess = MembershipSystem->Login(DBTransaction, UserInfo);
        }
       else  if (ManagerSmartCards->Enabled)
		{
			if (ManagerSmartCards->CardInserted)
			{
				if (!ManagerSmartCards->CardBlank)
				{
					if (!ManagerSmartCards->CardUnknown)
					{
                        MemberSource = emsSmartCard;
                        LoginSuccess = MembershipSystem->Login(DBTransaction, UserInfo);
                    }
				}
			}
			else if (AllowSmartCardNotPresentMemberTransactions || AllowDB)
			{
				LoginSuccess = MembershipSystem->Login(DBTransaction, UserInfo);
				if (!UserInfo.Cards.size())
			    	UserInfo.Points.PointsRules << eprNoPointsRedemption;
				MemberSource = emsSmartCardOffline;
			}
		}
		else
		{
			MemberSource = emsManual;
			LoginSuccess = MembershipSystem->Login(DBTransaction, UserInfo);
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		MessageBox("Unable to load Member information.\r" + E.Message, "Error", MB_OK + MB_ICONERROR);
	}
	return LoginSuccess;
}

TLoginSuccess TManagerMembershipSmartCards::FindMember(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo)
{
	TLoginSuccess LoginSuccess = lsUserNotFound;
	try
	{
		if (ManagerSmartCards->Enabled)
		{
			if (ManagerSmartCards->CardInserted)
			{
				if (!ManagerSmartCards->CardBlank)
				{
					if (!ManagerSmartCards->CardUnknown)
					{
						TMMContactInfo SmartCardContact;
						ManagerSmartCards->GetContactInfo(SmartCardContact);
						UserInfo = SmartCardContact;
						LoginSuccess = MembershipSystem->Login(DBTransaction, UserInfo);
						// Override the points in the DB with whats on the smart card.
						UserInfo.Points = SmartCardContact.Points;
					}
				}
			}
			else if (AllowSmartCardNotPresentMemberTransactions)
			{
				LoginSuccess = MembershipSystem->FindMember(DBTransaction, UserInfo);
			}
		}
		else
		{
			LoginSuccess = TManagerMembership::FindMember(DBTransaction, UserInfo);
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		MessageBox("Unable to load Member information.\r" + E.Message, "Error", MB_OK + MB_ICONERROR);
	}
	return LoginSuccess;
}

AnsiString TManagerMembershipSmartCards::GetActivationEmailFromUser()
{
	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(Screen->ActiveForm));
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = "";
	frmTouchKeyboard->Caption = "Enter Registered Email For Loyaltymate";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
		UnicodeString activationEmail = frmTouchKeyboard->KeyboardText;
		return AnsiString(activationEmail);
	}

	return AnsiString("");
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//Cloud Related Code
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

bool TManagerMembershipSmartCards::runMemberDownloadThread(TSyndCode CurrentSyndicateCode,TMMContactInfo &SmartCardContact,
bool useUUID,bool useMemberCode, bool useEmail,bool &memberNotExist)
{
	bool replacePointsFromCloud = true;
	TLoyaltyMateDownloadMemberThread* loyaltyMemberDownloadThread = new TLoyaltyMateDownloadMemberThread(CurrentSyndicateCode,replacePointsFromCloud);
	loyaltyMemberDownloadThread->FreeOnTerminate = true;
	loyaltyMemberDownloadThread->UUID = SmartCardContact.CloudUUID;
	loyaltyMemberDownloadThread->DownLoadFromUUID = useUUID;
    loyaltyMemberDownloadThread->MemberCode = SmartCardContact.MemberCode;
	loyaltyMemberDownloadThread->DownLoadFromCode = useMemberCode;
    loyaltyMemberDownloadThread->MemberEmail = SmartCardContact.EMail;
	loyaltyMemberDownloadThread->DownLoadFromEmail = useEmail;
	//display the dialog box
	std::auto_ptr<TfrmLoyaltyMateOperationDialogBox> loyaltyMateOperationDialogBox = std::auto_ptr<TfrmLoyaltyMateOperationDialogBox>(TfrmLoyaltyMateOperationDialogBox::Create<TfrmLoyaltyMateOperationDialogBox>(Screen->ActiveForm));
	loyaltyMateOperationDialogBox->OperationDescription = "Downloading member information from LoyaltyMate... Please Wait.";
	loyaltyMateOperationDialogBox->OperationTitle 		= "LoyaltyMate Operation";
	loyaltyMateOperationDialogBox->DownloadThread 		= loyaltyMemberDownloadThread;
	loyaltyMateOperationDialogBox->Info			= SmartCardContact;
	loyaltyMemberDownloadThread->Start();
    bool dialogResultSuccessful = loyaltyMateOperationDialogBox->ShowModal() == mrOk;
	if(dialogResultSuccessful)
	{
        dialogResultSuccessful = true;
		//download complete, copy the values across and display the member information
		SmartCardContact.CloudUUID       = loyaltyMateOperationDialogBox->Info.CloudUUID;
		SmartCardContact.Phone          = loyaltyMateOperationDialogBox->Info.Phone;
		SmartCardContact.Mobile          = loyaltyMateOperationDialogBox->Info.Mobile;
		SmartCardContact.EMail           = loyaltyMateOperationDialogBox->Info.EMail;
		SmartCardContact.Name            = loyaltyMateOperationDialogBox->Info.Name;
		SmartCardContact.Surname         = loyaltyMateOperationDialogBox->Info.Surname;
		SmartCardContact.MailingAddress         = loyaltyMateOperationDialogBox->Info.MailingAddress;
        SmartCardContact.LocationAddress         = loyaltyMateOperationDialogBox->Info.LocationAddress;
		SmartCardContact.Title         = loyaltyMateOperationDialogBox->Info.Title;
		SmartCardContact.ActivationToken = loyaltyMateOperationDialogBox->Info.ActivationToken;
		SmartCardContact.DateOfBirth     = loyaltyMateOperationDialogBox->Info.DateOfBirth;
		SmartCardContact.LastVisit       = loyaltyMateOperationDialogBox->Info.LastVisit;
		SmartCardContact.TierLevel       = loyaltyMateOperationDialogBox->Info.TierLevel;
		SmartCardContact.LastBirthdayProcessed       = loyaltyMateOperationDialogBox->Info.LastBirthdayProcessed;
		SmartCardContact.MemberType       = loyaltyMateOperationDialogBox->Info.MemberType;
		SmartCardContact.ActivationDate       = loyaltyMateOperationDialogBox->Info.ActivationDate;
        SmartCardContact.PreviousYearPoint       = loyaltyMateOperationDialogBox->Info.PreviousYearPoint;
        SmartCardContact.CurrentYearPoint       = loyaltyMateOperationDialogBox->Info.CurrentYearPoint;
        SmartCardContact.AvailableBDPoint       = loyaltyMateOperationDialogBox->Info.AvailableBDPoint;
        SmartCardContact.AvailableFVPoint       = loyaltyMateOperationDialogBox->Info.AvailableFVPoint;
        SmartCardContact.MemberCode       = loyaltyMateOperationDialogBox->Info.MemberCode;
        if(SmartCardContact.MembershipNumber == NULL || SmartCardContact.MembershipNumber == "")
        {
           SmartCardContact.MembershipNumber  = loyaltyMateOperationDialogBox->Info.MembershipNumber;
        }
        if(SmartCardContact.SiteID == 0)
        {
           SmartCardContact.SiteID  = loyaltyMateOperationDialogBox->Info.SiteID;
        }
        SmartCardContact.LastModified  = loyaltyMateOperationDialogBox->Info.LastModified;
        SmartCardContact.IsFirstVisitRewarded  = loyaltyMateOperationDialogBox->Info.IsFirstVisitRewarded;
        SmartCardContact.MemberCode  = loyaltyMateOperationDialogBox->Info.MemberCode;
        SmartCardContact.Points = loyaltyMateOperationDialogBox->Info.Points;
        TPointsRulesSetUtils().Expand(loyaltyMateOperationDialogBox->Info.PointRule, SmartCardContact.Points.PointsRules);
        SmartCardContact.MemberVouchers = loyaltyMateOperationDialogBox->Info.MemberVouchers;
        SmartCardContact.HasTransactions = loyaltyMateOperationDialogBox->Info.HasTransactions;
	}
    else
    {
      memberNotExist = loyaltyMateOperationDialogBox->BarcodeMemberNotExist;
    }
	return dialogResultSuccessful;
}

bool TManagerMembershipSmartCards::createMemberOnLoyaltyMate(TSyndCode syndicateCode, TMMContactInfo &inContactInfo)
{
	bool result = false;
	bool running = false;

	// initiate loyaltymate member create thread and create member
	TLoyaltyMateCreateMemberThread* memberCreationThread = new TLoyaltyMateCreateMemberThread(
	syndicateCode,
	&inContactInfo);
	memberCreationThread->OnTerminate = loyaltyMateMemberCreationCompleted;
	memberCreationThread->Start();

	// display dialog box
	_lmOperationDialogBox = new TfrmLoyaltyMateOperationDialogBox(Screen->ActiveForm);

	_lmOperationDialogBox->OperationDescription = "Creating member information on LoyaltyMate...Please Wait.";
	_lmOperationDialogBox->OperationTitle = "LoyaltyMate Operation";
	_lmOperationDialogBox->PreventCancelOperation = true;
	_lmOperationDialogBox->ShowModal();

	result = memberCreationThread->OperationSuccessful;

	if(!result)
	  MessageBox(memberCreationThread->ErrorMessage,"Failed to create member", MB_ICONERROR + MB_OK);
    else
      MessageBox("Member created. Please re-insert card or scan member code to continue.","LoyaltyMate Operation", MB_ICONINFORMATION + MB_OK);

	// cleanup
	delete _lmOperationDialogBox;
	delete memberCreationThread;

	return result;
}

void __fastcall TManagerMembershipSmartCards::loyaltyMateMemberCreationCompleted(TObject* sender)
{
	_lmOperationDialogBox->Close();
}

void TManagerMembershipSmartCards::GetMemberDetail(TMMContactInfo &MMContactInfo)
{
   bool MemberNotExist = false;
   bool isCancel = false;
   TGlobalSettings::Instance().IsPOSOffline = !runMemberDownloadThread(ManagerSyndicateCode.GetCommunicationSyndCode(),MMContactInfo,
                                               true,false,false,MemberNotExist);
   if(TGlobalSettings::Instance().IsPOSOffline)
   {
     MessageBox( "Unable to read data from server. Members may not be able to spend their points for the time being.", "Message", MB_ICONINFORMATION + MB_OK);
   }
   MembershipSystem->CurrentYearPoints  = MMContactInfo.CurrentYearPoint;
   MembershipSystem->PreviousYearPoints = MMContactInfo.PreviousYearPoint;
   MembershipSystem->AvailableBDPoint  = MMContactInfo.AvailableBDPoint;
   MembershipSystem->AvailableFVPoint = MMContactInfo.AvailableFVPoint;
   MembershipSystem->MemberVouchers = MMContactInfo.MemberVouchers;
}

void TManagerMembershipSmartCards::SaveContactProfileOnDownload(Database::TDBTransaction &DBTransaction,TMMContactInfo &Info)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE "
		"CONTACTS "
		"SET "
        "NAME = :NAME, "
        "LAST_NAME = :LAST_NAME, "
        "TITLE = :TITLE, "
		"PHONE = :PHONE, "
		"MOBILE = :MOBILE, "
		"MAILING_ADDRESS = :MAILING_ADDRESS, "
        "DATEOFBIRTH = :DATEOFBIRTH "
		"WHERE "
		"CONTACTS_KEY = :CONTACTS_KEY";
		IBInternalQuery->ParamByName("NAME")->AsString = Info.Name;
		IBInternalQuery->ParamByName("LAST_NAME")->AsString = Info.Surname;
		IBInternalQuery->ParamByName("TITLE")->AsString = Info.Title;
		IBInternalQuery->ParamByName("PHONE")->AsString = Info.Phone;
		IBInternalQuery->ParamByName("MOBILE")->AsString = Info.Mobile;
		IBInternalQuery->ParamByName("MAILING_ADDRESS")->AsString = Info.MailingAddress;
        IBInternalQuery->ParamByName("DATEOFBIRTH")->AsDateTime = Info.DateOfBirth;
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = Info.ContactKey;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

void TManagerMembershipSmartCards::SaveContactInfoOnDownload(Database::TDBTransaction &DBTransaction,TMMContactInfo &Info)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE "
		"CONTACTS "
		"SET "
		"NAME = :NAME, "
		"LAST_NAME = :LAST_NAME, "
		"TITLE = :TITLE, "
		"PHONE = :PHONE, "
		"MOBILE = :MOBILE, "
		"MAILING_ADDRESS = :MAILING_ADDRESS, "
		"TIER_LEVEL = :TIER_LEVEL, "
        "IS_FIRSTVISIT_REWARDED = :IS_FIRSTVISIT_REWARDED "
		"WHERE "
		"CONTACTS_KEY = :CONTACTS_KEY";
		IBInternalQuery->ParamByName("NAME")->AsString = Info.Name;
		IBInternalQuery->ParamByName("LAST_NAME")->AsString = Info.Surname;
		IBInternalQuery->ParamByName("TITLE")->AsString = Info.Title;
		IBInternalQuery->ParamByName("PHONE")->AsString = Info.Phone;
		IBInternalQuery->ParamByName("MOBILE")->AsString = Info.Mobile;
		IBInternalQuery->ParamByName("TIER_LEVEL")->AsInteger = Info.TierLevel;
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = Info.ContactKey;
        IBInternalQuery->ParamByName("IS_FIRSTVISIT_REWARDED")->AsString = Info.IsFirstVisitRewarded ? 'T' : 'F';
		IBInternalQuery->ExecQuery();
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

bool TManagerMembershipSmartCards::HasCard(Database::TDBTransaction &DBTransaction,int contactKey)
{
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text =
    "SELECT CONTACTS_KEY FROM SMARTCARDS WHERE CONTACTS_KEY = :CONTACTS_KEY ";
    IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = contactKey;
    IBInternalQuery->ExecQuery();
    return !IBInternalQuery->Eof;
}

void TManagerMembershipSmartCards::ValidateCardExistance(Database::TDBTransaction &DBTransaction,TMMContactInfo &Info)
{
   try
   {
      int contactKeyToRetain = 0;
      int contactKeyToReplace = 0;

	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT CONTACTS_KEY FROM CONTACTS WHERE SITE_ID = :SITE_ID AND  MEMBER_NUMBER = :MEMBER_NUMBER ";
      IBInternalQuery->ParamByName("SITE_ID")->AsInteger = Info.SiteID;
      IBInternalQuery->ParamByName("MEMBER_NUMBER")->AsString = Info.MembershipNumber;
	  IBInternalQuery->ExecQuery();

      if(IBInternalQuery->RecordCount == 1)
      {
         contactKeyToReplace =  TDBContacts::GetContactByEmail(DBTransaction,Info.EMail,Info.ContactKey);
         if(contactKeyToReplace != 0)
         {
             ReverseLinkSmartCard(DBTransaction,contactKeyToReplace,Info);
         }
      }
      else
      {
          IBInternalQuery->Close();
          IBInternalQuery->SQL->Text = "SELECT CONTACTS_KEY FROM CONTACTS WHERE SITE_ID = :SITE_ID AND  MEMBER_NUMBER = :MEMBER_NUMBER "
                                       "AND  MEMBER_CARD_CODE IS NOT NULL AND MEMBER_CARD_CODE <> ''";
          IBInternalQuery->ParamByName("SITE_ID")->AsInteger = Info.SiteID;
          IBInternalQuery->ParamByName("MEMBER_NUMBER")->AsString = Info.MembershipNumber;
          IBInternalQuery->ExecQuery();
          contactKeyToReplace =  IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;

          IBInternalQuery->Close();
          IBInternalQuery->SQL->Text = "SELECT CONTACTS_KEY FROM CONTACTS WHERE SITE_ID = :SITE_ID AND  MEMBER_NUMBER = :MEMBER_NUMBER "
                                       "AND  ( MEMBER_CARD_CODE IS NULL OR MEMBER_CARD_CODE = '' )";
          IBInternalQuery->ParamByName("SITE_ID")->AsInteger = Info.SiteID;
          IBInternalQuery->ParamByName("MEMBER_NUMBER")->AsString = Info.MembershipNumber;
          IBInternalQuery->ExecQuery();
          contactKeyToRetain = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
          Info.ContactKey = contactKeyToRetain;
          ReverseLinkSmartCard(DBTransaction,contactKeyToReplace,Info);
      }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
   }
}

int TManagerMembershipSmartCards::ValidateCardExistanceUsingUUID(Database::TDBTransaction &DBTransaction,TMMContactInfo &Info)
{
   int retVal = 0;
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text ="SELECT a.CONTACTS_KEY  "
                                  "FROM LOYALTYATTRIBUTES A LEFT JOIN "
                                  "CONTACTS B ON A.CONTACTS_KEY = B.CONTACTS_KEY "
                                  "WHERE B.SITE_ID = :SITE_ID AND B.MEMBER_NUMBER = :MEMBER_NUMBER "
                                  "AND a.CONTACTS_KEY <> :CONTACTS_KEY";
      IBInternalQuery->ParamByName("SITE_ID")->AsInteger = Info.SiteID;
      IBInternalQuery->ParamByName("MEMBER_NUMBER")->AsString = Info.MembershipNumber;
      IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = Info.ContactKey;
	  IBInternalQuery->ExecQuery();
      if(!IBInternalQuery->Eof)
         retVal = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
   }
    return retVal;
}

bool TManagerMembershipSmartCards::LinkSmartCard(Database::TDBTransaction &DBTransaction,int contactKey,TMMContactInfo &SmartCardContact)
{
   try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        if(SmartCardContact.ContactKey != 0)
        {
            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text =
            "UPDATE POINTSTRANSACTIONS SET CONTACTS_KEY = :CONTACTS_KEY "
            "WHERE CONTACTS_KEY = :MODIFIED_CONTACTS_KEY";
            IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = contactKey;
            IBInternalQuery->ParamByName("MODIFIED_CONTACTS_KEY")->AsInteger = SmartCardContact.ContactKey;
            IBInternalQuery->ExecQuery();

            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text =
            "UPDATE SMARTCARDS SET CONTACTS_KEY = :CONTACTS_KEY "
            "WHERE CONTACTS_KEY = :MODIFIED_CONTACTS_KEY";
            IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = contactKey;
            IBInternalQuery->ParamByName("MODIFIED_CONTACTS_KEY")->AsInteger = SmartCardContact.ContactKey;
            IBInternalQuery->ExecQuery();

            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text =
            "DELETE FROM CONTACTS "
            "WHERE CONTACTS_KEY = :MODIFIED_CONTACTS_KEY";
            IBInternalQuery->ParamByName("MODIFIED_CONTACTS_KEY")->AsInteger = SmartCardContact.ContactKey;
            IBInternalQuery->ExecQuery();
        }
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
        "UPDATE CONTACTS SET SITE_ID = :SITE_ID, MEMBER_NUMBER=:MEMBER_NUMBER "
        "WHERE CONTACTS_KEY = :CONTACTS_KEY";
        IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = contactKey;
        IBInternalQuery->ParamByName("SITE_ID")->AsInteger = SmartCardContact.SiteID;
        IBInternalQuery->ParamByName("MEMBER_NUMBER")->AsString = SmartCardContact.MembershipNumber;
        IBInternalQuery->ExecQuery();


	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

bool TManagerMembershipSmartCards::ReverseLinkSmartCard(Database::TDBTransaction &DBTransaction,int contactKey,TMMContactInfo &SmartCardContact)
{
   try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE POINTSTRANSACTIONS SET CONTACTS_KEY = :CONTACTS_KEY "
		"WHERE CONTACTS_KEY = :MODIFIED_CONTACTS_KEY";
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = SmartCardContact.ContactKey;
        IBInternalQuery->ParamByName("MODIFIED_CONTACTS_KEY")->AsInteger = contactKey;
		IBInternalQuery->ExecQuery();


        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
        "UPDATE LOYALTYATTRIBUTES SET CONTACTS_KEY = :CONTACTS_KEY "
		"WHERE CONTACTS_KEY = :MODIFIED_CONTACTS_KEY";
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = SmartCardContact.ContactKey;
        IBInternalQuery->ParamByName("MODIFIED_CONTACTS_KEY")->AsInteger = contactKey;
        IBInternalQuery->ExecQuery();

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
        "DELETE FROM CONTACTS "
        "WHERE CONTACTS_KEY = :MODIFIED_CONTACTS_KEY";
        IBInternalQuery->ParamByName("MODIFIED_CONTACTS_KEY")->AsInteger = contactKey;
        IBInternalQuery->ExecQuery();

	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::      163
//Barcode
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

bool TManagerMembershipSmartCards::GetMemberDetailFromBarcode(TMMContactInfo &MMContactInfo,AnsiString memberCode)
{
   bool memberDownloadStatus = false;
   bool MemberNotExist = false;
   AnsiString oldCode = MMContactInfo.MemberCode;
   MMContactInfo.MemberCode = memberCode;
   if(ManagerSyndicateCode.GetCommunicationSyndCode().Valid())
   {
       memberDownloadStatus = runMemberDownloadThread(ManagerSyndicateCode.GetCommunicationSyndCode(),MMContactInfo,false,true,false,MemberNotExist);
       
       if(MemberNotExist)
        {
           TGlobalSettings::Instance().IsPOSOffline = false;
        }
       else
        {
           TGlobalSettings::Instance().IsPOSOffline = !memberDownloadStatus;
        }

       if(TGlobalSettings::Instance().IsPOSOffline)
       {
          MMContactInfo.MemberCode = oldCode;
         MessageBox( "Unable to read data from Cloud. Members may not be able to spend their points for the time being.", "Message", MB_ICONINFORMATION + MB_OK);
       }

       MembershipSystem->CurrentYearPoints  = MMContactInfo.CurrentYearPoint;
       MembershipSystem->PreviousYearPoints = MMContactInfo.PreviousYearPoint;
       MembershipSystem->AvailableBDPoint  = MMContactInfo.AvailableBDPoint;
       MembershipSystem->AvailableFVPoint = MMContactInfo.AvailableFVPoint;
       MembershipSystem->MemberVouchers = MMContactInfo.MemberVouchers;
       }
   else
   {
      MessageBox( "Syndicate Code is not valid.", "Message", MB_ICONINFORMATION + MB_OK);
   }
   return MemberNotExist;
}

bool TManagerMembershipSmartCards::GetMemberDetailFromEmail(TMMContactInfo &MMContactInfo)
{
   bool memberDownloadStatus = false;
   bool MemberNotExist = false;
   if(ManagerSyndicateCode.GetCommunicationSyndCode().Valid())
   {
       memberDownloadStatus = runMemberDownloadThread(ManagerSyndicateCode.GetCommunicationSyndCode(),MMContactInfo,false,false,true,MemberNotExist);

       if(MemberNotExist)
        {
           TGlobalSettings::Instance().IsPOSOffline = false;
        }
       else
        {
           TGlobalSettings::Instance().IsPOSOffline = !memberDownloadStatus;
        }
       if(TGlobalSettings::Instance().IsPOSOffline)
       {
         MessageBox( "Unable to read data from Cloud. Members may not be able to spend their points for the time being.", "Message", MB_ICONINFORMATION + MB_OK);
       }
       MembershipSystem->CurrentYearPoints  = MMContactInfo.CurrentYearPoint;
       MembershipSystem->PreviousYearPoints = MMContactInfo.PreviousYearPoint;
       MembershipSystem->AvailableBDPoint  = MMContactInfo.AvailableBDPoint;
       MembershipSystem->AvailableFVPoint = MMContactInfo.AvailableFVPoint;
       MembershipSystem->MemberVouchers = MMContactInfo.MemberVouchers;
       }
   else
   {
      MessageBox( "Syndicate Code is not valid.", "Message", MB_ICONINFORMATION + MB_OK);
   }
   return MemberNotExist;
}

bool TManagerMembershipSmartCards::MemberCodeScanned(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo,AnsiString memberCardCode)
{
   bool addDefaultPoints = false;
   bool isCancel = false;
   bool memberNotExist = false ;
   TMMContactInfo localContactInfo;
   TContactPoints pointsToSync;
   MemberMode memberMode = eInvalidMode;
   bool existInLocalDb = TDBContacts::GetContactDetailsByCode(DBTransaction,localContactInfo,memberCardCode,memberMode);
   UserInfo = localContactInfo;
   if(TLoyaltyMateUtilities::IsLoyaltyMateEnabledGUID(localContactInfo.CloudUUID))
   {
       GetMemberDetail(UserInfo);
   }
   else
   {
     if(existInLocalDb)
     {
       pointsToSync = localContactInfo.Points;
       //check for email
       if(UserInfo.EMail == "" || UserInfo.EMail == NULL)
       {
          UserInfo.EMail = GetActivationEmailFromUser();
          if(UserInfo.EMail != "" && UserInfo.EMail != NULL)
          {
              int localEmailContactKey = TDBContacts::GetContactByEmail(DBTransaction,UserInfo.EMail);
              if(localEmailContactKey != 0 && localEmailContactKey != UserInfo.ContactKey && !HasCard(DBTransaction,localEmailContactKey))
              {
                LinkMembers(DBTransaction, UserInfo.ContactKey, localEmailContactKey);
                TDBContacts::GetContactDetails(DBTransaction,localEmailContactKey,UserInfo);
                addDefaultPoints = true;
              }
              memberNotExist = GetMemberDetailFromEmail(UserInfo);
          }
          else
          {
             memberNotExist = GetMemberDetailFromBarcode(UserInfo,memberCardCode);
          }
       }
       else
       {
          memberNotExist = GetMemberDetailFromEmail(UserInfo);
       }
     }
     else
     {
         memberNotExist = GetMemberDetailFromBarcode(UserInfo,memberCardCode);
     }
   }

   if(memberNotExist && !existInLocalDb)
    {
       if(MessageBox("No Item/Member found, Press Ok to create a new member or Cancel to continue.","Member Not Exist", MB_OKCANCEL + MB_ICONQUESTION) == ID_CANCEL)
       {
          return false;
       }
    }

   if(!existInLocalDb)
   {
      if(memberNotExist)
      {
         bool canAddMember = false;
         if(TGlobalSettings::Instance().GiftCardOnly)
         {
            canAddMember = true;
         }
         else
         {
            if(!TGlobalSettings::Instance().EnablePhoneOrders)
            {
                    TMMContactInfo TempUserInfo;
                    std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
                    TLoginSuccess Result = Staff->Login(Screen->ActiveForm, DBTransaction, TempUserInfo, CheckAccountManager);
                    if (Result == lsAccepted)
                    {
                       canAddMember = true;
                    }
                    else if (Result == lsDenied)
                    {
                      MessageBox("You do not have access to Membership.", "Error", MB_OK + MB_ICONERROR);
                    }
                    else if (Result == lsPINIncorrect)
                    {
                      MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
                    }
            }
            else
            {
                    canAddMember = true;
            }
         }

         if(canAddMember)
         {
                 AddMember(UserInfo,true);
         }
      }
      else if(!TGlobalSettings::Instance().IsPOSOffline)
      {
         //check for linking here and load points in loyalty pending transaction table
         if(UserInfo.MemberType == 0)
         {
           if(UserInfo.EMail.Pos("GiftCard") == 1)
           {
                  UserInfo.MemberType = 2;
           }
           else
           {
                   UserInfo.MemberType = 1;
           }
         }
            //Member not exist in local DB and check that email member is not smartcard member
         int localEmailContactKey = TDBContacts::GetContactByEmail(DBTransaction,UserInfo.EMail);
         MembershipSystem->SetContactDetails(DBTransaction, UserInfo.ContactKey, UserInfo);
         if(localEmailContactKey != 0 && localEmailContactKey != UserInfo.ContactKey && !HasCard(DBTransaction,localEmailContactKey))
          {
            TDBContacts::GetPointsBalances(DBTransaction, localEmailContactKey, pointsToSync);
            LinkMembers(DBTransaction, localEmailContactKey, UserInfo.ContactKey);
            addDefaultPoints = true;
          }
         MembershipSystem->SetContactLoyaltyAttributes(DBTransaction, UserInfo.ContactKey, UserInfo);
      }
   }
   else if(!TGlobalSettings::Instance().IsPOSOffline)
   {
       if (memberNotExist)
       {
         AnsiString message = "For Loyaltymate, you will need to update your ";
         bool updateMember = false;
         if(UserInfo.ValidateMandatoryField(message))
         {
            pointsToSync = UserInfo.Points;
            updateMember = true;
         }
         else
         {
             MessageBox(message.SubString(1, message.Length()-1) + ".", "Message", MB_ICONINFORMATION + MB_OK);
             std::auto_ptr < TfrmEditCustomer >
             frmEditCustomer(TfrmEditCustomer::Create(Screen->ActiveForm));
             frmEditCustomer->Editing = true;
             frmEditCustomer->Info = UserInfo;
             frmEditCustomer->MemberType = UserInfo.MemberType;
             if(frmEditCustomer->ShowModal() == mrOk)
             {
                 updateMember = true;
                 pointsToSync = UserInfo.Points;
                 UserInfo = frmEditCustomer->Info;
                 UserInfo.Points =  pointsToSync;
                 MembershipSystem->SetContactDetails(DBTransaction, UserInfo.ContactKey, UserInfo);
             }
         }
         if(updateMember)
         {
            bool memberCreationSuccess = createMemberOnLoyaltyMate(ManagerSyndicateCode.GetCommunicationSyndCode(),UserInfo);
            addDefaultPoints = memberCreationSuccess;
         }
       }
       else if (!TGlobalSettings::Instance().IsPOSOffline && !memberNotExist)
       {
           MembershipSystem->SetContactDetails(DBTransaction, UserInfo.ContactKey, UserInfo);
           if(TLoyaltyMateUtilities::IsLoyaltyMateEnabledGUID(UserInfo.CloudUUID))
           {
           	 MembershipSystem->SetContactLoyaltyAttributes(DBTransaction, UserInfo.ContactKey, UserInfo);
           }
       }
   }

   if(addDefaultPoints && TLoyaltyMateUtilities::IsLoyaltyMateEnabledGUID(UserInfo.CloudUUID))
   {
     AddDefaultPoints(DBTransaction,pointsToSync,UserInfo.ContactKey);
     TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, "Added Default Entry --- MemberCodeScanned");
   }
    DBTransaction.Commit();
    DBTransaction.StartTransaction();

}

bool TManagerMembershipSmartCards::SavePointsTransactionsForBarcodeCard(TContactPoints &Points,TMMContactInfo &UserInfo,AnsiString inInvoiceNumber, bool PointsFromCloud)
{
    Database::TDBTransaction DBTransaction(DBControl);
    RegisterTransaction(DBTransaction);
    DBTransaction.StartTransaction();

    if(TGlobalSettings::Instance().LoyaltyMateEnabled
                && TLoyaltyMateUtilities::IsLoyaltyMateEnabledGUID(UserInfo.CloudUUID)
                && !PointsFromCloud)
        {
            TLoyaltyMateUtilities::SetLoyaltymateTransactions(
            DBTransaction,
            UserInfo.ContactKey,
            ManagerSyndicateCode.GetCommunicationSyndCode(),
            Points,
            inInvoiceNumber);
        }
    DBTransaction.Commit();
    Points.setExportStatus(pesExported);
	Points.TimeStampExported = Now();
    return true;
}

void TManagerMembershipSmartCards::SyncBarcodeMemberDetailWithCloud(TMMContactInfo &MMContactInfo)
{
     if(TGlobalSettings::Instance().LoyaltyMateEnabled
                && TLoyaltyMateUtilities::IsLoyaltyMateEnabledGUID(MMContactInfo.CloudUUID))
    {
        TManagerLoyaltyMate::Instance()->SyncMemberDetailsWithCloud(ManagerSyndicateCode.GetCommunicationSyndCode(),MMContactInfo);
    }
}

bool TManagerMembershipSmartCards::UpdateMemberCardCode(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo,AnsiString memberCardCode)
{
   	bool result = false;

	TLoyaltyMateUpdateCardThread* memberUpdationThread = new TLoyaltyMateUpdateCardThread(ManagerSyndicateCode.GetCommunicationSyndCode(),
                                                         UserInfo.CloudUUID,memberCardCode);
	memberUpdationThread->OnTerminate = loyaltyMateMemberCreationCompleted;
	memberUpdationThread->Start();

	// display dialog box
	_lmOperationDialogBox = new TfrmLoyaltyMateOperationDialogBox(Screen->ActiveForm);

	_lmOperationDialogBox->OperationDescription = "Updating member information on LoyaltyMate...Please Wait.";
	_lmOperationDialogBox->OperationTitle = "LoyaltyMate Operation";
	_lmOperationDialogBox->PreventCancelOperation = true;
	_lmOperationDialogBox->ShowModal();

	result = memberUpdationThread->OperationSuccessful;

	if(!result)
    	MessageBox(memberUpdationThread->ErrorMessage,"Failed to update member", MB_ICONERROR + MB_OK);
    else
       {
         UpdateMemberCardCodeToDB(DBTransaction,UserInfo,memberCardCode);
       }

	// cleanup
	delete _lmOperationDialogBox;
	delete memberUpdationThread;

	return result;
}

void TManagerMembershipSmartCards::UpdateMemberCardCodeToDB(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo,AnsiString memberCardCode)
{
  try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE "
		"CONTACTS "
		"SET "
        "MEMBER_CARD_CODE = :MEMBER_CARD_CODE "
		"WHERE "
		"CONTACTS_KEY = :CONTACTS_KEY";
		IBInternalQuery->ParamByName("MEMBER_CARD_CODE")->AsString = memberCardCode;
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = UserInfo.ContactKey;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

void TManagerMembershipSmartCards::LinkMembers(Database::TDBTransaction &DBTransaction, int contactToReplace, int contactKey)
{
   try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE POINTSTRANSACTIONS SET CONTACTS_KEY = :CONTACTS_KEY "
		"WHERE CONTACTS_KEY = :MODIFIED_CONTACTS_KEY";
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = contactKey;
        IBInternalQuery->ParamByName("MODIFIED_CONTACTS_KEY")->AsInteger = contactToReplace;
		IBInternalQuery->ExecQuery();

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
        "UPDATE CONTACTCARDS SET CONTACTS_KEY = :CONTACTS_KEY "
        "WHERE CONTACTS_KEY = :MODIFIED_CONTACTS_KEY";
        IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = contactKey;
        IBInternalQuery->ParamByName("MODIFIED_CONTACTS_KEY")->AsInteger = contactToReplace;
        IBInternalQuery->ExecQuery();


        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
        "UPDATE CONTACTS SET PROX_CARD = :PROX_CARD "
        "WHERE CONTACTS_KEY = :MODIFIED_CONTACTS_KEY";
        IBInternalQuery->ParamByName("PROX_CARD")->AsString = TDBContacts::GetContactProxCard(DBTransaction,contactToReplace);
        IBInternalQuery->ParamByName("MODIFIED_CONTACTS_KEY")->AsInteger = contactKey;
        IBInternalQuery->ExecQuery();

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
        "DELETE FROM CONTACTS "
        "WHERE CONTACTS_KEY = :MODIFIED_CONTACTS_KEY";
        IBInternalQuery->ParamByName("MODIFIED_CONTACTS_KEY")->AsInteger = contactToReplace;
        IBInternalQuery->ExecQuery();
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}




//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//Points Processing
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

bool TManagerMembershipSmartCards::ProcessPoints(TPaymentTransaction &PaymentTransaction)
{
    AnsiString invoiceNumber = PaymentTransaction.InvoiceNumber;
	bool Complete = false;
	if(PaymentTransaction.Membership.Member.MemberType == 1 )
	{
		RewardBirthdaybenefit(PaymentTransaction);
		RewardFirstVisitPoints(PaymentTransaction);
        SaveTransactionInvoiceDetail(PaymentTransaction);
	}
	if (PaymentTransaction.Membership.Member.ContactKey != 0 && PaymentTransaction.Membership.Member.Points.HasTransactions())
	{
        if(ManagerSmartCards->CardOk)
		// Update SmartCard system.
		  Complete = SavePointsTransactionsToSmartCard(PaymentTransaction.Membership.Member.Points,invoiceNumber);
        else
          Complete = SavePointsTransactionsForBarcodeCard(PaymentTransaction.Membership.Member.Points,PaymentTransaction.Membership.Member,invoiceNumber) ;
	}
	else
	{
		Complete = true;
	}
	return Complete;
}

void TManagerMembershipSmartCards::SaveTransactionInvoiceDetail(TPaymentTransaction &PaymentTransaction)
{
    if(TGlobalSettings::Instance().LoyaltyMateEnabled &&
        TLoyaltyMateUtilities::IsLoyaltyMateEnabledGUID(PaymentTransaction.Membership.Member.CloudUUID))
    {
        TLoyaltyMateTransaction transaction;
        transaction.ContactKey = PaymentTransaction.Membership.Member.ContactKey;
        transaction.SyndicateCode = ManagerSyndicateCode.GetCommunicationSyndCode();
        transaction.OccurredDate = Now();
        transaction.PointsDelta = PaymentTransaction.Money.GrandTotal;
        transaction.PointsType = 9;
        transaction.InvoiceNumber = PaymentTransaction.InvoiceNumber;
        TLoyaltyMateUtilities::SetTransaction(PaymentTransaction.DBTransaction,transaction);
    }
}

void TManagerMembershipSmartCards::AddDefaultPoints(Database::TDBTransaction &DBTransaction,TContactPoints &Points,int contactkey)
{
   if(Points.getPointsBalance(ptstLoyalty) > 0)
   {
        TLoyaltyMateTransaction transaction;
        transaction.ContactKey = contactkey;
        transaction.SyndicateCode = ManagerSyndicateCode.GetCommunicationSyndCode();
        transaction.OccurredDate = Now();
        transaction.PointsDelta = Points.getPointsBalance(ptstLoyalty);
        transaction.PointsType = 1;
        TLoyaltyMateUtilities::SetTransaction(DBTransaction,transaction);
   }

   if(Points.getPointsBalance(ptstAccount) > 0)
   {
        TLoyaltyMateTransaction transaction;
        transaction.ContactKey = contactkey;
        transaction.SyndicateCode = ManagerSyndicateCode.GetCommunicationSyndCode();
        transaction.OccurredDate = Now();
        transaction.PointsDelta = Points.getPointsBalance(ptstAccount);
        transaction.PointsType = 8;
        TLoyaltyMateUtilities::SetTransaction(DBTransaction,transaction);
   }
   TManagerLoyaltyMate::Instance()->TriggerPointSync();
   MessageBox("Points restored. Please re-insert card or scan member code to continue.","LoyaltyMate Operation", MB_ICONINFORMATION + MB_OK);
}

void TManagerMembershipSmartCards::RewardBirthdaybenefit(TPaymentTransaction &PaymentTransaction)
{
	bool canEarnPoints = TGlobalSettings::Instance().UseTierLevels;
	if( canEarnPoints && TGlobalSettings::Instance().LoyaltyMateEnabled)
	{
		canEarnPoints = !TGlobalSettings::Instance().IsPOSOffline &&
		TLoyaltyMateUtilities::IsLoyaltyMateEnabledGUID(PaymentTransaction.Membership.Member.CloudUUID);
	}
	//Add points for birth day
	if (canEarnPoints &&
			TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->CheckForBirthdayBenifit(PaymentTransaction.DBTransaction,PaymentTransaction.Membership.Member))
	{
        if(PaymentTransaction.Membership.Member.TierLevel != 0)
        {
            int birthdayBonus = TDBTierLevel::GetBirthdayBonusPoints(PaymentTransaction.DBTransaction, PaymentTransaction.Membership.Member.TierLevel);
            TPointsTypePair typepair(pttBirthdayBonus,ptstLoyalty);
            TPointsType type(pasUser, typepair,pesNone);
            PaymentTransaction.Membership.Member.Points.Load(type,birthdayBonus);
            TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->SetLastBirthdayProcessed(PaymentTransaction.DBTransaction, PaymentTransaction.Membership.Member);
            PaymentTransaction.IsBirtdayProcessed = true;
            PaymentTransaction.Birthdaypoints = birthdayBonus;
            TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->AvailableBDPoint  += birthdayBonus;
        }
	}

}

void TManagerMembershipSmartCards::RewardFirstVisitPoints(TPaymentTransaction &PaymentTransaction)
{
	bool canEarnPoints = TGlobalSettings::Instance().AcknowledgeFirstVisit;
	if( canEarnPoints && TGlobalSettings::Instance().LoyaltyMateEnabled)
	{
		canEarnPoints = !TGlobalSettings::Instance().IsPOSOffline &&
		TLoyaltyMateUtilities::IsLoyaltyMateEnabledGUID(PaymentTransaction.Membership.Member.CloudUUID);
	}

	//Add First Visit Points
	if (canEarnPoints && !TDBContacts::IsFirstVisitRewarded(PaymentTransaction.DBTransaction, PaymentTransaction.Membership.Member.ContactKey))
	{
		TPointsTypePair typepair(pttFirstVisit,ptstLoyalty);
		TPointsType type(pasUser, typepair,pesNone);
		PaymentTransaction.Membership.Member.Points.Load(type,TGlobalSettings::Instance().FirstVisitPoint);
		PaymentTransaction.IsFirstVisitProcessed = true;
        PaymentTransaction.Membership.Member.IsFirstVisitRewarded = true;
        TDBContacts::SetFirstVisitRewardStatus(PaymentTransaction.DBTransaction, PaymentTransaction.Membership.Member.ContactKey);
		TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->SetLastVisit(PaymentTransaction.DBTransaction, PaymentTransaction.Membership.Member);
        TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->AvailableFVPoint  += TGlobalSettings::Instance().FirstVisitPoint;
	}

}



