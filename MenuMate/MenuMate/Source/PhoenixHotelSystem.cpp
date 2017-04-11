//---------------------------------------------------------------------------
#pragma hdrstop

#include "PhoenixHotelSystem.h"
#include "MMMessageBox.h"
#include "SelectPhoenixRoom.h"
#include "ManagerVariable.h"
#include "ManagerThirdParty.h"
#include "MMLogging.h"
#include "DBThirdPartyCodes.h"
#include "ManagerPatron.h"

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef  PalmMate
#include "Palm.h"
#endif
//---------------------------------------------------------------------------

#pragma package(smart_init)

TPhoenixHM *PhoenixHM;

TPhoenixHM::TPhoenixHM(Database::TDBControl &inDBControl) : DBControl(inDBControl), fPhoenixNet(new TPhoenixNetTCPManager)
{
	FTCPPort = 4444;
	FTCPIPAddress = "";
	FPOSID = 0;
	DefaultPaymentCategory = "";
	DefaultItemCategory = "";
	PointsCategory = "";
	CreditCategory = "";
	LastTransactionRef = "";
	RoundingCategory = "";
	Enabled = false;
	Registered = false;
}

void __fastcall TPhoenixHM::SetTCPPort(int value)
{
	if(FTCPPort != value)
	{
		FTCPPort = value;
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmPMSTCPPort,FTCPPort);
		DBTransaction.Commit();
	}
}
int __fastcall TPhoenixHM::GetTCPPort()
{
	return FTCPPort;
}

void __fastcall TPhoenixHM::SetTCPIPAddress(AnsiString value)
{
	if(FTCPIPAddress != value) {
		FTCPIPAddress = value;
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmPMSIPAddress,FTCPIPAddress);
		DBTransaction.Commit();
	}
}
AnsiString __fastcall TPhoenixHM::GetTCPIPAddress()
{
	return FTCPIPAddress;
}

void __fastcall TPhoenixHM::SetPOSID(int value)
{
	if(FPOSID != value) {
		FPOSID = value;
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceInt(DBTransaction,vmPMSPOSID,FPOSID);
		DBTransaction.Commit();
	}
}
int __fastcall TPhoenixHM::GetPOSID()
{
	return FPOSID;
}

void __fastcall TPhoenixHM::SetDefaultPaymentCategory(AnsiString value)
{
	if(FDefaultPaymentCategory != value)
	{
		if(TestCode(value))
		{
			FDefaultPaymentCategory = value;
			Database::TDBTransaction DBTransaction(DBControl);
			DBTransaction.StartTransaction();
			TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmPMSPaymentCategory,FDefaultPaymentCategory);
			DBTransaction.Commit();
		}
      ClearCodesTestedOk();
	}
}
AnsiString __fastcall TPhoenixHM::GetDefaultPaymentCategory()
{
	return FDefaultPaymentCategory;
}

void __fastcall TPhoenixHM::SetDefaultItemCategory(AnsiString value)
{
	if(FDefaultItemCategory != value)
	{
		if(TestCode(value))
		{
			FDefaultItemCategory = value;
			Database::TDBTransaction DBTransaction(DBControl);
			DBTransaction.StartTransaction();
			TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmPMSItemCategory,FDefaultItemCategory);
			DBTransaction.Commit();
		}
      ClearCodesTestedOk();
	}
}
AnsiString __fastcall TPhoenixHM::GetDefaultItemCategory()
{
	return FDefaultItemCategory;
}

void __fastcall TPhoenixHM::SetCreditCategory(AnsiString value)
{
	if(FCreditCategory != value)
	{
		if(TestCode(value))
		{
			FCreditCategory = value;
			Database::TDBTransaction DBTransaction(DBControl);
			DBTransaction.StartTransaction();
			TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmPMSCreditCategory,FCreditCategory);
			DBTransaction.Commit();
		}
      ClearCodesTestedOk();
	}
}

void __fastcall TPhoenixHM::SetRoundingCategory(AnsiString value)
{
	if(FRoundingCategory != value) {
		FRoundingCategory = value;
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmPMSDefaultRoundingAccount,FRoundingCategory);
		DBTransaction.Commit();
	}
}

AnsiString __fastcall TPhoenixHM::GetRoundingCategory()
{
	return FRoundingCategory;
}

AnsiString __fastcall TPhoenixHM::GetCreditCategory()
{
	return FCreditCategory;
}

void __fastcall TPhoenixHM::SetPointsCategory(AnsiString value)
{
	if(FPointsCategory != value)
	{
		if(TestCode(value))
		{
			FPointsCategory = value;
			Database::TDBTransaction DBTransaction(DBControl);
			DBTransaction.StartTransaction();
			TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmPMSPointsCategory,FPointsCategory);
			DBTransaction.Commit();
		}
      ClearCodesTestedOk();
	}
}
AnsiString __fastcall TPhoenixHM::GetPointsCategory()
{
	return FPointsCategory;
}

void __fastcall TPhoenixHM::SetDefaultTransactionAccount(AnsiString value)
{
	if(FDefaultTransactionAccount != value) {
		FDefaultTransactionAccount = value;
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmPMSDefaultAccount,FDefaultTransactionAccount);
		DBTransaction.Commit();
	}
}
AnsiString __fastcall TPhoenixHM::GetDefaultTransactionAccount()
{
	return FDefaultTransactionAccount;
}

void __fastcall TPhoenixHM::SetDefaultSurchargeAccount(AnsiString value)
{
	if(FDefaultSurchargeAccount != value) {
		FDefaultSurchargeAccount = value;
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmPMSDefaultSurchargeAccount,FDefaultSurchargeAccount);
		DBTransaction.Commit();
	}
}
AnsiString __fastcall TPhoenixHM::GetDefaultSurchargeAccount()
{
	return FDefaultSurchargeAccount;
}

bool TPhoenixHM::ExportData(TPaymentTransaction &PaymentTransaction, int StaffID)
{
	bool RetVal = false;

   AnsiString PMSIPAddress = FTCPIPAddress;
   int PMSPort = FTCPPort;

	TPhoenixRoomCharge RoomCharge;
	RoomCharge.AccountNumber = PaymentTransaction.Phoenix.AccountNumber;
	RoomCharge.FolderNumber = PaymentTransaction.Phoenix.FolderNumber;
	RoomCharge.StaffID = StaffID;
	if(RoomCharge.AccountNumber == "" )
	{
		RoomCharge.AccountNumber = DefaultTransactionAccount;
	}

	Currency Total = 0;
	Currency TotalRounding = 0;

	for(int i = 0; i < PaymentTransaction.Orders->Count; i++)
	{
		TItemComplete *Order = (TItemComplete *)PaymentTransaction.Orders->Items[i];
		AnsiString ThirdPartyCode = TDBThirdPartyCodes::GetThirdPartyCodeByKey(PaymentTransaction.DBTransaction,Order->ThirdPartyKey);
		if(ThirdPartyCode == "")
		{
			ThirdPartyCode = DefaultItemCategory;
			TManagerLogs::Instance().Add(__FUNC__,PHOENIXINTERFACELOG,Order->Item + " " + Order->Size + " Has No 3rdParty Code");
		}
		RoomCharge.Categories[ThirdPartyCode] += Order->TotalPriceAdjustment();
		TManagerLogs::Instance().Add(__FUNC__,PHOENIXINTERFACELOG,"Item : " + Order->Item + " " +
		ThirdPartyCode +
		" Item Purchase " + CurrToStrF(Order->TotalPriceAdjustment(), ffCurrency, 2));

		for(int i = 0; i < Order->SubOrders->Count; i++)
		{
			TItemCompleteSub *SubOrder = (TItemCompleteSub *)Order->SubOrders->SubOrderGet(i);
			AnsiString SubOrderThirdPartyCode = TDBThirdPartyCodes::GetThirdPartyCodeByKey(PaymentTransaction.DBTransaction,SubOrder->ThirdPartyKey);
			if(SubOrderThirdPartyCode == "")
			{
				SubOrderThirdPartyCode = DefaultItemCategory;
				TManagerLogs::Instance().Add(__FUNC__,PHOENIXINTERFACELOG,SubOrder->Item + " " + SubOrder->Size + " Has No 3rdParty Code");
			}
			RoomCharge.Categories[SubOrderThirdPartyCode] += SubOrder->TotalPriceAdjustment();
			TManagerLogs::Instance().Add(__FUNC__,PHOENIXINTERFACELOG,"Sub Item : " + SubOrder->Item + " " +
			SubOrderThirdPartyCode +
			" Item Purchase " + CurrToStrF(SubOrder->TotalPriceAdjustment(), ffCurrency, 2));

			Total += SubOrder->TotalPriceAdjustment();
		}

		Total += Order->TotalPriceAdjustment();
	}

	for ( int i = 0 ; i <  PaymentTransaction.PaymentsCount(); i++ )
	{
		TPayment *Payment = PaymentTransaction.PaymentGet(i);

		if(Payment->GetPay() != 0 || Payment->GetAdjustment() != 0)
		{
			if(Payment->PaymentThirdPartyID == "")
			{
				Payment->PaymentThirdPartyID = DefaultPaymentCategory;
				TManagerLogs::Instance().Add(__FUNC__,PHOENIXINTERFACELOG,Payment->Name + " Has No 3rdParty ID");
			}
			if(Payment->GetPaymentAttribute(ePayTypeCredit))
			{
				RoomCharge.Categories[CreditCategory] += -Payment->GetPayTendered();
				Total += -Payment->GetPayTendered();
				TManagerLogs::Instance().Add(__FUNC__,PHOENIXINTERFACELOG,"Credit : " + Payment->Name + " " +
				CreditCategory +
				" Pay " + CurrToStrF(-Payment->GetPayTendered(), ffCurrency, 2));
			}
			else if(Payment->GetPaymentAttribute(ePayTypePoints))
			{
				RoomCharge.Categories[PointsCategory] += -Payment->GetPayTendered();
				Total += -Payment->GetPayTendered();
				TManagerLogs::Instance().Add(__FUNC__,PHOENIXINTERFACELOG,"Points : " + Payment->Name + " " +
				PointsCategory +
				" Pay " + CurrToStrF(-Payment->GetPayTendered(), ffCurrency, 2));

			}
			else if(Payment->GetPaymentAttribute(ePayTypeCustomSurcharge))
			{
				RoomCharge.Categories[Payment->PaymentThirdPartyID] += Payment->GetSurcharge();
				Total += Payment->GetAdjustment();
				TManagerLogs::Instance().Add(__FUNC__,PHOENIXINTERFACELOG,"Tip : " + Payment->Name + " " +
				Payment->PaymentThirdPartyID +
				" Custom Surcharge " + CurrToStrF(Payment->GetAdjustment(), ffCurrency, 2));
			}
			else if(Payment->GetPaymentAttribute(ePayTypeRoomInterface))
			{
				// Dont adjust total as its being charged to his room.
			}
			else if(Payment->GetPaymentAttribute(ePayTypeGetVoucherDetails))
			{
				// If Voucher Purchase the amount is storedin the Surchage.
				RoomCharge.Categories[Payment->PaymentThirdPartyID] += -Payment->GetPayTendered();
				Total += -Payment->GetPayTendered();
				TManagerLogs::Instance().Add(__FUNC__,PHOENIXINTERFACELOG,"Payment : " + Payment->Name + " " +
				Payment->PaymentThirdPartyID +
				" Pay " + CurrToStrF(-Payment->GetPayTendered(), ffCurrency, 2));
				RoomCharge.Categories[Payment->PaymentThirdPartyID] += Payment->GetSurcharge();
				Total += Payment->GetAdjustment();
				TManagerLogs::Instance().Add(__FUNC__,PHOENIXINTERFACELOG,"Payment : " + Payment->Name + " " +
				Payment->PaymentThirdPartyID +
				" Surchaarge " + CurrToStrF(Payment->GetAdjustment(), ffCurrency, 2));
			}
			else
			{
				RoomCharge.Categories[Payment->PaymentThirdPartyID] += -Payment->GetPayTendered();
				Total += -Payment->GetPayTendered();
				TManagerLogs::Instance().Add(__FUNC__,PHOENIXINTERFACELOG,"Payment : " + Payment->Name + " " +
				Payment->PaymentThirdPartyID +
				" Pay " + CurrToStrF(-Payment->GetPayTendered(), ffCurrency, 2));
			}

			if(Payment->GetPaymentAttribute(ePayTypeSurcharge))
			{
				RoomCharge.Categories[DefaultSurchargeAccount] += Payment->GetAdjustment(); // GetAdjustment() is positive, when a surcharge is present
				Total += Payment->GetAdjustment();
				TManagerLogs::Instance().Add(__FUNC__,PHOENIXINTERFACELOG,"Surcharge : " + Payment->Name + " " +
				Payment->PaymentThirdPartyID +
				" Surcharge " + CurrToStrF(Payment->GetAdjustment(), ffCurrency, 2));

				// Cat portion of Surcharge is included in Payment->Pay
				//Total += -Payment->Surcharge;
			}

			if(Payment->GetPaymentAttribute(ePayTypeSecondaryPMSExport))
			{
                // Override the ipaddress and port.
                PMSIPAddress = Payment->SecondaryPMSIPAddress;
                PMSPort = Payment->SecondaryPMSPortNumber;
			}
			TotalRounding += Payment->GetRoundingTotal();
		}
	}

	if(TotalRounding != 0)
	{
		RoomCharge.Categories[RoundingCategory] += TotalRounding;
		Total += TotalRounding;
	}

	TPayment *PaymentCashAcc = PaymentTransaction.PaymentFind(CASH);
	AnsiString CashAccountNumber = DefaultPaymentCategory;
	if(PaymentCashAcc != NULL)
	{
		CashAccountNumber = PaymentCashAcc->PaymentThirdPartyID;
		if(CashAccountNumber == "")
		{
			CashAccountNumber = DefaultPaymentCategory;
			TManagerLogs::Instance().Add(__FUNC__,PHOENIXINTERFACELOG,PaymentCashAcc->Name + " Cash Acc Has No 3rdParty ID");
		}
	}
	else
	{
		TManagerLogs::Instance().Add(__FUNC__,PHOENIXINTERFACELOG,"No Cash Account for Change 3rdParty ID");
	}

	/* Write out the Change.
      If its a credit transaction involving at least some cash the 'change'
      is refunded to the customer, this amount has already been calculated into
      the CASH payment type.*/

	if(PaymentTransaction.Money.Change != 0 && !PaymentTransaction.CreditTransaction)
	{
		RoomCharge.Categories[CashAccountNumber] += PaymentTransaction.Money.Change;
		Total += PaymentTransaction.Money.Change;
	}

	// Write out Payments to cover the "Cash Out" portion of the
	// change given above.
	for ( int i = 0 ; i <  PaymentTransaction.PaymentsCount(); i++ )
	{
		TPayment *Payment = PaymentTransaction.PaymentGet(i);
		if(Payment->GetCashOutTotal() != 0)
		{
			//RoomCharge.Categories[CashAccountNumber] += Payment->CashOut;
			//Total += Payment->CashOut;

			TManagerLogs::Instance().Add(__FUNC__,PHOENIXINTERFACELOG,"CashOut : " + Payment->Name + " " +
			CashAccountNumber +
			" Change " + CurrToStrF(Payment->GetCashOutTotal(), ffCurrency, 2));

			// Fake an 'EFTPOS' Payement of the same amout as cash out to cover the cash out 'purchase'
			if(Payment != PaymentCashAcc)
			{
				if(Payment->PaymentThirdPartyID == "")
				{
					Payment->PaymentThirdPartyID = DefaultPaymentCategory;
					TManagerLogs::Instance().Add(__FUNC__,PHOENIXINTERFACELOG,Payment->Name + " Has No 3rdParty ID");
				}
				RoomCharge.Categories[Payment->PaymentThirdPartyID] += -Payment->GetCashOutTotal();
				Total += -Payment->GetCashOutTotal();
				TManagerLogs::Instance().Add(__FUNC__,PHOENIXINTERFACELOG,"CashOut Payment : " + Payment->Name + " " +
				Payment->PaymentThirdPartyID +
				" Cash Out Purchase " + CurrToStrF(-Payment->GetCashOutTotal(), ffCurrency, 2));
			}
		}
	}

	RoomCharge.Total = Total;

	if(RoomCharge.AccountNumber != DefaultTransactionAccount)
	{
		CheckCreditLimit(RoomCharge,PMSIPAddress,PMSPort);
	}

	if(RoomCharge.Result == eAccepted)
	{
		RoomCharge.Covers = TManagerPatron::Instance().GetTotalPatrons(PaymentTransaction.Patrons);
		LastTransactionRef = GetRefNumber();
		RoomCharge.ReferenceNumber = LastTransactionRef;
		PaymentTransaction.Phoenix.ReferenceNumber = RoomCharge.ReferenceNumber;
		ChargeRoom(RoomCharge,PMSIPAddress, PMSPort);
		if(RoomCharge.Result == eAccepted)
		{
			RetVal = true;
		}
		else
		{
			TManagerLogs::Instance().Add(__FUNC__,PHOENIXINTERFACELOG,"PMS Export Failed.\r" + RoomCharge.ResultText);
			MessageBox("PMS Export Failed.\r" + RoomCharge.ResultText,
					  "PMS Hotel Export Error", MB_OK + MB_ICONERROR);
		}
	}
	else
	{
		TManagerLogs::Instance().Add(__FUNC__,PHOENIXINTERFACELOG,"PMS Export Failed.\r" + RoomCharge.ResultText);
		MessageBox("PMS Export Failed.\r" + RoomCharge.ResultText,
				  "PMS Hotel Export Error", MB_OK + MB_ICONERROR);
	}
	return RetVal;
}

void TPhoenixHM::CheckCreditLimit(TPhoenixRoomCharge &RoomCharge,AnsiString PMSIPAddress,int PMSPort)
{
	TPhoenixRoomStatusExt RoomStatus;
	RoomStatus.AccountNumber = RoomCharge.AccountNumber;
	RoomStatus.FolderNumber = RoomCharge.FolderNumber;
	RoomStatus.POSID = RoomCharge.POSID;		
	GetRoomStatus(RoomStatus,PMSIPAddress,PMSPort);
	if(RoomStatus.Result != eAccepted)
	{
		RoomCharge.Result = RoomStatus.Result;
		RoomCharge.ResultText = RoomStatus.ResultText;
	}
	else
	{
		RoomCharge.AccountNumber = RoomStatus.AccountNumber;
		RoomCharge.FolderNumber = RoomStatus.FolderNumber;		

		Currency AmountAvailable = RoomStatus.CreditLimit - RoomStatus.Balance;
		if(AmountAvailable < RoomCharge.Total)
		{
			AnsiString Balance = CurrToStrF(RoomStatus.Balance, ffCurrency, 2);
			AnsiString CreditLimit = CurrToStrF(RoomStatus.CreditLimit, ffCurrency, 2);
			RoomCharge.Result = eDeclined;
			RoomCharge.ResultText = "Credit Limit Exceeded\r Current Balance " + Balance + " Credit Limit " + CreditLimit;
		}
	}
}

void TPhoenixHM::GetRoomStatus(TPhoenixRoomStatusExt &Status,AnsiString PMSIPAddress,int PMSPort)
{
	// Select the room number.
	// Retrive the Room Folders.
	if(Status.AccountNumber != "")
	{
		Status.POSID = POSID;
		fPhoenixNet->SendAndFetch(Status,PMSIPAddress,PMSPort);
	}
	else
	{ // Canceled by User.
		Status.Result = eFailed;
		Status.ResultText = "Invalid Account Number";
		TManagerLogs::Instance().Add(__FUNC__,PHOENIXINTERFACELOG,"Invalid Account Number");
	}
}

AnsiString TPhoenixHM::GetRefNumber()
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_PHOENIX_REF, 1) FROM RDB$DATABASE";
	IBInternalQuery->ExecQuery();
	int PhoenixRef = IBInternalQuery->Fields[0]->AsInteger;
	DBTransaction.Commit();
	
	return "PMS" + IntToStr(PhoenixRef);
}

AnsiString TPhoenixHM::GetLastTransactionRef()
{
	return LastTransactionRef;
}


void TPhoenixHM::ChargeRoom(TPhoenixRoomCharge &RoomCharge,AnsiString PMSIPAddress,int PMSPort)
{
	// Select the room number.
	// Retrive the Room Folders.
	if(RoomCharge.AccountNumber != "" && FTCPIPAddress != "")
	{
		RoomCharge.POSID = POSID;
		fPhoenixNet->SendAndFetch(RoomCharge,PMSIPAddress,PMSPort);
	}
	else
	{ // Canceled by User.
		RoomCharge.Result = eFailed;
		RoomCharge.ResultText = "Invalid Account Number";
		TManagerLogs::Instance().Add(__FUNC__,PHOENIXINTERFACELOG,"Invalid Account Number");		
	}
}

void TPhoenixHM::Initialise()
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	FTCPPort = TManagerVariable::Instance().GetInt(DBTransaction,vmPMSTCPPort);
	FTCPIPAddress = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSIPAddress);
	FPOSID = TManagerVariable::Instance().GetInt(DBTransaction,vmPMSPOSID);

	FDefaultPaymentCategory = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSPaymentCategory);
	FDefaultItemCategory = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSItemCategory);
	FCreditCategory = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSCreditCategory);
	FPointsCategory = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSPointsCategory);
	FDefaultTransactionAccount = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSDefaultAccount);
	FDefaultSurchargeAccount = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSDefaultSurchargeAccount);
	FRoundingCategory = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSDefaultRoundingAccount);

	DBTransaction.Commit();
	if(Registered && FTCPIPAddress != "")
	{
		Enabled = true;
		Enabled = TestDefaultCodes();
	}
	else
	{
		Enabled = false;
	}
}

bool TPhoenixHM::TestDefaultCodes()
{
	bool Success = true;
	Success = TestCode(DefaultPaymentCategory);
	if(Success)
		Success = TestCode(DefaultItemCategory);
	if(Success)
		Success = TestCode(CreditCategory);
	if(Success)
		Success = TestCode(PointsCategory);
	if(Success)
		Success = TestCode(RoundingCategory);
	if(Success)
		Success = TestCode(DefaultSurchargeAccount);

   ClearCodesTestedOk();
	return Success;
}

void TPhoenixHM::ClearCodesTestedOk()
{
   CodesTestedOk.clear();
}

bool TPhoenixHM::TestCode(AnsiString CodeToTest)
{
	bool RetVal = true;
	try
	{
		if(Enabled && CodeToTest != "")
		{
         if(CodesTestedOk.find(CodeToTest) == CodesTestedOk.end())
         {
            TPhoenixRoomCharge RoomCharge;
            RoomCharge.AccountNumber = DefaultTransactionAccount;
            RoomCharge.FolderNumber = 1;
            RoomCharge.StaffID = 0;
            RoomCharge.Categories[CodeToTest] = 0;

            ChargeRoom(RoomCharge,FTCPIPAddress,FTCPPort);
            if(RoomCharge.Result != eAccepted)
            {
               RetVal = false;
               TManagerLogs::Instance().Add(__FUNC__,PHOENIXINTERFACELOG,"PMS Test of Category " + CodeToTest + " Failed.\r" + RoomCharge.ResultText);
               MessageBox("PMS Test of Code " + CodeToTest + " Failed.\r" + RoomCharge.ResultText,
                       "PMS Hotel Category Error", MB_OK + MB_ICONERROR);
            }
            else
            {
               CodesTestedOk.insert(CodeToTest);
            }
         }
		}
	}
	catch(Exception &E)
	{
		RetVal = false;
			MessageBox("Pheonix Test of Code " + CodeToTest + " Failed.\r" + E.Message,
					  "PMS Hotel Category Error", MB_OK + MB_ICONERROR);
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG, E.Message);
	}
	return RetVal;
}



void TPhoenixHM::FindAndReplaceCode(AnsiString SourceCode, AnsiString DestCode)
{
   
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
   int ThirdPartyKey = TDBThirdPartyCodes::GetThirdPartyKeyByCode(DBTransaction,SourceCode,tpItemSize);
   if(ThirdPartyKey)
   {
      TThirdPartyCodeType Type = TDBThirdPartyCodes::GetThirdPartyTypeByKey(DBTransaction,ThirdPartyKey);
      AnsiString Description = TDBThirdPartyCodes::GetThirdPartyDescriptionByKey(DBTransaction,ThirdPartyKey);
      TDBThirdPartyCodes::SetThirdPartyCodeByKey(DBTransaction,ThirdPartyKey,DestCode,Description,Type);
   }
	DBTransaction.Commit();
}
