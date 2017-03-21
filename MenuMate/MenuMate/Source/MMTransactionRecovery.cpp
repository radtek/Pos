//---------------------------------------------------------------------------


#pragma hdrstop

#include "MMTransactionRecovery.h"
#include "Eftpos.h"
#include "DeviceRealTerminal.h"
#include "ManagerPatron.h"
#include "CSV.h"
#include "ContactStaff.h"
#include "DBOrder.h"
#include "ListPaymentSystem.h"
#include "MMInvoicePaymentSystem.h"

//---------------------------------------------------------------------------

TMMTransactionRecovery::TMMTransactionRecovery()
{
	RecoveryFilePaymentSystem	= ExtractFilePath(Application->ExeName) + "EFTPOSPaymentSystem.bin";
	RecoveryFileSecurity 		= ExtractFilePath(Application->ExeName) + "EFTPOSSecurity.bin";
	RecoveryFilePayments 		= ExtractFilePath(Application->ExeName) + "EFTPOSPayments.bin";
	RecoveryFileOrders 			= ExtractFilePath(Application->ExeName) + "EFTPOSOrders.bin";
	RecoveryFileOrders 			= ExtractFilePath(Application->ExeName) + "EFTPOSOrderItems.bin";
}
//---------------------------------------------------------------------------

void TMMTransactionRecovery::ProcessTransactionRecovery()
{
	if (EftPos->Enabled)
	{
		try
		{
			Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
			TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
			DBTransaction.StartTransaction();

			TPaymentTransaction RecoverTransaction(DBTransaction);
			std::auto_ptr<TMMPaymentSystem> paymentSystem;
            std::auto_ptr<TListSecurityRefContainer> securityContainer;

			try
			{
                securityContainer.reset( new TListSecurityRefContainer() );

				if (LoadRecoveryInformation(RecoverTransaction, securityContainer.get()))
				{
					bool isRecovery = true;

					if( isInvoicePaymentTransaction(RecoverTransaction) )
					{
						paymentSystem.reset( new TMMInvoicePaymentSystem() );
						paymentSystem->Security = securityContainer.get();

						// this will process as normal bill selected invoice transaction set
						RecoverTransaction.Type = eTransInvoicePayment;

						paymentSystem->ProcessTransaction( RecoverTransaction, isRecovery );
					}
					else
					{
						paymentSystem.reset( new TListPaymentSystem() );
						paymentSystem->Security = securityContainer.get();

						if (RecoverTransaction.RewardsTransaction.CardNumber != "")
						{
							RecoverTransaction.Type = eTransRewardsRecovery;
							MessageBox("Recovering Incomplete Rewards Transaction", "Rewards Recovery", MB_OK + MB_ICONINFORMATION);
						}
						else
						{
							RecoverTransaction.Type = eTransEFTPOSRecovery;
							MessageBox("Recovering Incomplete Transaction", "EFTPOS Recovery", MB_OK + MB_ICONINFORMATION);
						}

						paymentSystem->ProcessTransaction( RecoverTransaction, isRecovery );
					}

					TDeviceRealTerminal::Instance().User.Clear();
				}
			}
			__finally
			{
				while (RecoverTransaction.Orders->Count != 0)
				{
					TItemComplete *Order = (TItemComplete*)RecoverTransaction.Orders->Items[0];
					delete Order;
					RecoverTransaction.Orders->Delete(0);
				}
			}
			DBTransaction.Commit();
		}
		catch(Exception & E)
		{
			MessageBox("Unable to Process this Bill.\r" + E.Message + "\rPlease try again later.", "Error", MB_OK + MB_ICONERROR);
			TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		}
	}
}
//---------------------------------------------------------------------------

void TMMTransactionRecovery::SaveRecoveryInformation(TPaymentTransaction &PaymentTransaction, TListSecurityRefContainer* securityContainer )
{
	try
	{
		ClearRecoveryInfo();

        saveTransactionInfoToFile( PaymentTransaction );
        savePaymentTypesInfoToFile( PaymentTransaction );
        saveOrdersInfoToFile( PaymentTransaction );
        saveSecurtyInfoToFile( securityContainer );

		__int64 EndTime = ::GetTickCount() + 15000;
		bool Saved = false;
		while (::GetTickCount() < EndTime && !Saved)
		{
			if ( areAllRecoveryFilesPresent() )
			{
				Saved = true;
			}
		}

		if (!Saved)
		{
			MessageBox("Warning Unable to Save EFTPOS Recovery Info. Hard Disk Full?", "Error", MB_OK + MB_ICONWARNING);
		}
	}
	catch(Exception & E)
	{
		MessageBox("Unable to Save EFTPOS Recovery Info. " + E.Message, "Error", MB_OK + MB_ICONWARNING);
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		TManagerLogs::Instance().Add(__FUNC__, EFTPOSLOG, E.Message);
	}
}
//---------------------------------------------------------------------------

bool TMMTransactionRecovery::LoadRecoveryInformation(TPaymentTransaction &RecoverTransaction, TListSecurityRefContainer* securityContainer)
{
	bool retval = false;
	try
	{
        retval = areAllRecoveryFilesPresent();

		if( retval )
		{
            loadTransactionInfoFromFile( RecoverTransaction );
            loadPaymentTypesInfoFromFile( RecoverTransaction );
            loadOrdersInfoFromFile( RecoverTransaction );
            loadSecurtyInfoFromFile( securityContainer );

			// Relog in user.
            logUserFromTransactionFile( RecoverTransaction );
		}
		else
		{
			ClearRecoveryInfo();
		}
	}
	catch(Exception & E)
	{
		MessageBox("Unable to Load EFTPOS Recovery Info. The files are most likely currupt and will be removed. " + E.Message, "Error",
			MB_OK + MB_ICONWARNING);
		TManagerLogs::Instance().Add(__FUNC__, EFTPOSLOG, "Unable to Load Recovery Info" + E.Message);
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, "Unable to Load Recovery Info" + E.Message);
		ClearRecoveryInfo();

		retval = false;
	}
	return retval;
}
//---------------------------------------------------------------------------

void TMMTransactionRecovery::ClearRecoveryInfo()
{
	if (EftPos->Enabled)
	{
		// Remove File.
		if (FileExists(RecoveryFilePaymentSystem))
		{
			DeleteFile(RecoveryFilePaymentSystem);
		}
		if (FileExists(RecoveryFileSecurity))
		{
			DeleteFile(RecoveryFileSecurity);
		}
		if (FileExists(RecoveryFilePayments))
		{
			DeleteFile(RecoveryFilePayments);
		}
		if (FileExists(RecoveryFileOrders))
		{
			DeleteFile(RecoveryFileOrders);
		}
	}
}
//---------------------------------------------------------------------------

bool TMMTransactionRecovery::isInvoicePaymentTransaction( TPaymentTransaction &transaction )
{
	return transaction.SalesType == eAccount;
}
//---------------------------------------------------------------------------

void TMMTransactionRecovery::saveTransactionInfoToFile( TPaymentTransaction &PaymentTransaction )
{
    int TotalPatrons = TManagerPatron::Instance().GetTotalPatrons( PaymentTransaction.Patrons);

    TCsv PaymentCsv;
    PaymentCsv.Add(IntToStr(PaymentTransaction.SalesType) + "," + PaymentTransaction.DiscountReason + "," +
        PaymentTransaction.ChitNumber.ChitNumber + "," + IntToStr(PaymentTransaction.ChitNumber.ChitNumberKey)
        + "," + PaymentTransaction.ChitNumber.Name + "," + PaymentTransaction.ChitNumber.Mask + "," + IntToStr
        (PaymentTransaction.ChitNumber.Type) + "," + IntToStr(PaymentTransaction.BookingID) + "," + IntToStr(TotalPatrons)
        + "," + CurrToStr(PaymentTransaction.Money.PaymentDue) + "," + CurrToStr(PaymentTransaction.Money.Change) + "," + CurrToStr
        (PaymentTransaction.Money.InvoiceTotal) + "," + CurrToStr(PaymentTransaction.Money.GrandTotal) + "," + CurrToStr
        (PaymentTransaction.Money.PaymentAmount) + "," + CurrToStr(PaymentTransaction.Money.TotalRounding) + "," + CurrToStr
        (PaymentTransaction.Money.GrandTotal) + "," + CurrToStr(PaymentTransaction.Money.CreditAvailable) + "," + CurrToStr
        (PaymentTransaction.Money.TotalAdjustment) + "," + IntToStr(TDeviceRealTerminal::Instance().User.ContactKey)
        + "," + PaymentTransaction.RewardsTransaction.CardNumber + "," + PaymentTransaction.RewardsTransaction.EFTPOSRef + "," + CurrToStr
        (PaymentTransaction.RewardsTransaction.Amount));

    PaymentCsv.SaveToFile(RecoveryFilePaymentSystem);
}
//---------------------------------------------------------------------------

void TMMTransactionRecovery::savePaymentTypesInfoToFile( TPaymentTransaction &PaymentTransaction )
{
    TCsv PaymentTypesCsv;
    for (int i = 0; i < PaymentTransaction.PaymentsCount(); i++)
    {
         TPayment *Payment = PaymentTransaction.PaymentGet(i);
         PaymentTypesCsv.Add(Payment->Name + "," + AnsiString(Payment->GetPropertyString()) + "," + IntToStr(Payment->Colour) + "," + IntToStr
             (Payment->DisplayOrder) + "," + FloatToStrF(Payment->PercentAdjust, ffGeneral, 15,
                0) + "," + FloatToStrF(Payment->AmountAdjust, ffGeneral, 15, 0) + "," + Payment->AdjustmentReason + "," + IntToStr
             (Payment->GroupNumber) + "," + CurrToStr(Payment->GetPay()) + "," + CurrToStr(Payment->GetCashOut()) + "," + CurrToStr
             (Payment->GetAdjustment()) + "," + IntToStr((int)Payment->Visible) + "," + IntToStr((int)Payment->CreditTransaction)
             + "," + IntToStr((int)Payment->CSVNumber) + "," + Payment->NameOveride + "," + Payment->Note + "," + Payment->ReferenceNumber);
    }
    PaymentTypesCsv.SaveToFile(RecoveryFilePayments);
}
//---------------------------------------------------------------------------

void TMMTransactionRecovery::saveOrdersInfoToFile( TPaymentTransaction &PaymentTransaction )
{
    TCsv CDSOrdersCsv;
    TList *Orders = PaymentTransaction.Orders;
    for (int i = 0; i < Orders->Count; i++)
    {
        TItemComplete *Order = (TItemComplete*)Orders->Items[i];
        CDSOrdersCsv.Add("0," + IntToStr(Order->OrderKey) + "," + FloatToStrF(Order->TotalDiscount(), ffGeneral, 15,
                0) + "," + FloatToStrF(Order->Redeemed, ffGeneral, 15,
                0) + "," + IntToStr(Order->Security->GetSecurityRefNumber()));

        for (int j = 0; j < Order->SubOrders->Count; j++)
        {
            TItemCompleteSub *SubOrder = Order->SubOrders->SubOrderGet(j);
            if (SubOrder)
            {
                CDSOrdersCsv.Add("1," + IntToStr(SubOrder->OrderKey) + "," + FloatToStrF(SubOrder->TotalDiscount(), ffGeneral, 15,
                        0) + "," + FloatToStrF(SubOrder->Redeemed, ffGeneral, 15,
                        0) + "," + IntToStr(SubOrder->Security->GetSecurityRefNumber()));
            }
        }
    }

    CDSOrdersCsv.SaveToFile(RecoveryFileOrders);
}
//---------------------------------------------------------------------------

void TMMTransactionRecovery::saveSecurtyInfoToFile( TListSecurityRefContainer* securityContainer )
{
	TCsv PaySysSecurityCsv;
	for (int j = 0; j < securityContainer->Count; j++)
	{
		TSecurityReference *SecRef = securityContainer->SecurityGet(j);
		PaySysSecurityCsv.Add(
			SecRef->Event + "," + SecRef->From + "," + SecRef->To + "," + SecRef->Note + "," + ::IntToStr(SecRef->UserKey)
			+ "," + FloatToStrF(double(SecRef->TimeStamp), ffGeneral, 15, 0));
	}
	PaySysSecurityCsv.SaveToFile(RecoveryFileSecurity);
}
//---------------------------------------------------------------------------

void TMMTransactionRecovery::loadTransactionInfoFromFile( TPaymentTransaction &PaymentTransaction )
{
    TCsv PaymentCsv;
    PaymentCsv.LoadFromFile(RecoveryFilePaymentSystem);

    PaymentTransaction.SalesType = eTransactionType(StrToInt(PaymentCsv.Cells[0][0]));
    PaymentTransaction.DiscountReason = PaymentCsv.Cells[1][0];
    PaymentTransaction.ChitNumber.ChitNumber = PaymentCsv.Cells[2][0];
    PaymentTransaction.ChitNumber.ChitNumberKey = StrToInt(PaymentCsv.Cells[3][0]);
    PaymentTransaction.ChitNumber.Name = PaymentCsv.Cells[4][0];
    PaymentTransaction.ChitNumber.Mask = PaymentCsv.Cells[5][0];
    PaymentTransaction.ChitNumber.Type = StrToInt(PaymentCsv.Cells[6][0]);
    PaymentTransaction.BookingID = StrToInt(PaymentCsv.Cells[7][0]);
    TManagerPatron::Instance().SetDefaultPatrons(PaymentTransaction.DBTransaction, PaymentTransaction.Patrons, StrToInt(PaymentCsv.Cells[8][0]));
    PaymentTransaction.Money.PaymentDue = StrToCurr(PaymentCsv.Cells[9][0]);
    PaymentTransaction.Money.Change = StrToCurr(PaymentCsv.Cells[10][0]);
    PaymentTransaction.Money.InvoiceTotal = StrToCurr(PaymentCsv.Cells[11][0]);
    PaymentTransaction.Money.GrandTotal = StrToCurr(PaymentCsv.Cells[12][0]);
    PaymentTransaction.Money.PaymentAmount = StrToCurr(PaymentCsv.Cells[13][0]);
    PaymentTransaction.Money.TotalRounding = StrToCurr(PaymentCsv.Cells[14][0]);
    PaymentTransaction.Money.GrandTotal = StrToCurr(PaymentCsv.Cells[15][0]);
    PaymentTransaction.Money.CreditAvailable = StrToCurr(PaymentCsv.Cells[16][0]);
    PaymentTransaction.Money.TotalAdjustment = StrToCurr(PaymentCsv.Cells[17][0]);

    PaymentTransaction.RewardsTransaction.CardNumber = PaymentCsv.Cells[19][0];
    PaymentTransaction.RewardsTransaction.EFTPOSRef = PaymentCsv.Cells[20][0];
    PaymentTransaction.RewardsTransaction.Amount = StrToCurr(PaymentCsv.Cells[21][0]);
}
//---------------------------------------------------------------------------

void TMMTransactionRecovery::loadPaymentTypesInfoFromFile( TPaymentTransaction &PaymentTransaction )
{
    PaymentTransaction.PaymentsClear();

    // Restore Payments.
    TCsv PaymentTypesCsv;
    PaymentTypesCsv.LoadFromFile(RecoveryFilePayments);

    for (int i = 0; i < PaymentTypesCsv.RowCount; i++)
    {
        TPayment *Payment = new TPayment;
        Payment->Name = PaymentTypesCsv.Cells[0][i];
        //Payment->Properties = StrToInt(PaymentTypesCsv.Cells[1][i]);
        Payment->Colour = StrToInt(PaymentTypesCsv.Cells[2][i]);
        Payment->DisplayOrder = StrToInt(PaymentTypesCsv.Cells[3][i]);
        Payment->PercentAdjust = double(StrToFloat(PaymentTypesCsv.Cells[4][i]));
        Payment->AmountAdjust = double(StrToFloat(PaymentTypesCsv.Cells[5][i]));
        Payment->AdjustmentReason = PaymentTypesCsv.Cells[6][i];
        Payment->GroupNumber = StrToInt(PaymentTypesCsv.Cells[7][i]);
        Payment->SetPay(double(StrToFloat(PaymentTypesCsv.Cells[8][i])));
        Payment->SetCashOut(double(StrToFloat(PaymentTypesCsv.Cells[9][i])));
        Payment->SetAdjustment(double(StrToFloat(PaymentTypesCsv.Cells[10][i])));
        Payment->Visible = StrToInt(PaymentTypesCsv.Cells[11][i]);
        Payment->CreditTransaction = StrToInt(PaymentTypesCsv.Cells[12][i]);
        Payment->CSVNumber = StrToInt(PaymentTypesCsv.Cells[13][i]);
        Payment->NameOveride = PaymentTypesCsv.Cells[14][i];
        Payment->Note = PaymentTypesCsv.Cells[15][i];
        Payment->ReferenceNumber = PaymentTypesCsv.Cells[16][i];

        PaymentTransaction.PaymentAdd(Payment);
    }
}
//---------------------------------------------------------------------------

void TMMTransactionRecovery::loadOrdersInfoFromFile( TPaymentTransaction &PaymentTransaction )
{
    std::set <__int64> OrderKeys;

    TCsv CDSOrdersCsv;
    CDSOrdersCsv.LoadFromFile(RecoveryFileOrders);
    for (int i = 0; i < CDSOrdersCsv.RowCount; i++)
    {
        // Insert Orders Only ( not sides into set )
        if (StrToInt(CDSOrdersCsv.Cells[0][i]) == 0)
        {
            OrderKeys.insert(StrToInt(CDSOrdersCsv.Cells[1][i]));
        }
    }
    TDBOrder::GetOrdersFromOrderKeys(PaymentTransaction.DBTransaction, PaymentTransaction.Orders, OrderKeys);

    // Retrive Security Ref and discount values.
    for (int i = 0; i < PaymentTransaction.Orders->Count; i++)
    {
        TItemComplete *Order = (TItemComplete*)PaymentTransaction.Orders->Items[i];
        for (int j = 0; j < CDSOrdersCsv.RowCount; j++)
        {
            if (Order->OrderKey == StrToInt(CDSOrdersCsv.Cells[1][j]))
            {
                Order->Redeemed = double(StrToFloat(CDSOrdersCsv.Cells[3][j]));
                Order->Security->SetSecurityRefNumber(StrToInt(CDSOrdersCsv.Cells[4][j]));
            }
        }

        for (int k = 0; k < Order->SubOrders->Count; k++)
        {
            TItemCompleteSub *SubOrder = Order->SubOrders->SubOrderGet(k);
            if (SubOrder)
            {
                for (int n = 0; n < CDSOrdersCsv.RowCount; n++)
                {
                    if (SubOrder->OrderKey == StrToInt(CDSOrdersCsv.Cells[1][n]))
                    {
                        SubOrder->Redeemed = double(StrToFloat(CDSOrdersCsv.Cells[3][n]));
                        SubOrder->Security->SetSecurityRefNumber(StrToInt(CDSOrdersCsv.Cells[4][n]));
                    }
                }
            }
        }
    }
}
//---------------------------------------------------------------------------

void TMMTransactionRecovery::loadSecurtyInfoFromFile( TListSecurityRefContainer* securityContainer )
{
	TCsv PaySysSecurityCsv;
	PaySysSecurityCsv.LoadFromFile(RecoveryFileSecurity);

	for (int i = 0; i < PaySysSecurityCsv.RowCount; i++)
	{
		TSecurityReference *SecRef = new TSecurityReference;
		SecRef->Event = PaySysSecurityCsv.Cells[0][i];
		SecRef->From = PaySysSecurityCsv.Cells[1][i];
		SecRef->To = PaySysSecurityCsv.Cells[2][i];
		SecRef->Note = PaySysSecurityCsv.Cells[3][i];
		SecRef->UserKey = StrToInt(PaySysSecurityCsv.Cells[4][i]);
		SecRef->TimeStamp = double(StrToFloat(PaySysSecurityCsv.Cells[5][i]));
		securityContainer->SecurityAdd(SecRef);
	}
}
//---------------------------------------------------------------------------

void TMMTransactionRecovery::logUserFromTransactionFile( TPaymentTransaction &PaymentTransaction )
{
    TMMContactInfo TempUserInfo;
    int staffKey = getStaffKeyFromTransactionFile();

    std::auto_ptr <TContactStaff> Staff(new TContactStaff(PaymentTransaction.DBTransaction));
    Staff->GetContactDetails(PaymentTransaction.DBTransaction, staffKey, TempUserInfo);
    TDeviceRealTerminal::Instance().User = TempUserInfo;
}
//---------------------------------------------------------------------------

int TMMTransactionRecovery::getStaffKeyFromTransactionFile()
{
    int staffKey;
    TCsv paymentCsv;

    paymentCsv.LoadFromFile(RecoveryFilePaymentSystem);
    staffKey = StrToInt(paymentCsv.Cells[18][0]);

    return staffKey;
}
//---------------------------------------------------------------------------

bool TMMTransactionRecovery::areAllRecoveryFilesPresent()
{
    bool result = FileExists( RecoveryFilePaymentSystem )
                    && FileExists( RecoveryFileSecurity )
                    && FileExists( RecoveryFilePayments )
                    && FileExists( RecoveryFileOrders );

    return result;
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
