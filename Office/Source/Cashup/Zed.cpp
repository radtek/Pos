//---------------------------------------------------------------------------
#pragma hdrstop

#include "Zed.h"
#include "MMData.h"
#include "MM_DBCore.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
using namespace Cashup;
//---------------------------------------------------------------------------
Zed::Zed(const int id,
		const System::TDateTime& createdDateTime,
		const System::AnsiString& terminalName,
		const AnsiString& cashier,
		const bool reconciled,
		const Currency& totalVariance) :

	_id(id),
	_createdDateTime(createdDateTime),
	_previousZCreatedDateTime(0),
	_terminalName(terminalName),
	_cashier(cashier),
	_reconciled(reconciled),
	_totalVariance(totalVariance)

{
}
//---------------------------------------------------------------------------
bool Zed::operator == (const Zed& rhs) const
{
	return _id == rhs._id;
}
//---------------------------------------------------------------------------
bool Zed::operator < (const Zed& rhs) const
{
	return _createdDateTime < rhs._createdDateTime;
}
//---------------------------------------------------------------------------
int Zed::GetId() const
{
	return _id;
}
//---------------------------------------------------------------------------
const TDateTime& Zed::GetCreated() const
{
	return _createdDateTime;
}
//---------------------------------------------------------------------------
const AnsiString& Zed::GetTerminalName() const
{
	return _terminalName;
}
//---------------------------------------------------------------------------
const AnsiString& Zed::GetCashier() const
{
	return _cashier;
}
//---------------------------------------------------------------------------
bool Zed::HasBeenReconciled() const
{
	return _reconciled;
}
//---------------------------------------------------------------------------
const Currency& Zed::GetTotalVariance() const
{
	return _totalVariance;
}
//---------------------------------------------------------------------------
const AnsiString INSERT_BALANCE_SQL =
		"Insert Into BlindBalance ("
			"BlindBalance_Key,"
			"Z_Key,"
			"Payment,"
			"Payment_Group,"
			"Payment_Trans_Qty, "
            "blind_balance, "
			"System_Balance,"
			"Office_Balance "
		")"
		"Values ("
			"(Select Gen_id(Gen_BlindBalance_Key, 1) From rdb$database),"
			":Z_Key,"
			":Payment,"
			":Payment_Group,"
			":Payment_Trans_Qty,"
            "0, "
			":System_Balance,"
			":Office_Balance "
		")";
//---------------------------------------------------------------------------
std::set<Payment> Zed::GetPayments() const
{
	if (_previousZCreatedDateTime == TDateTime(0))
	{
		GetPreviousZCreatedDateTime();
	}

	std::set<Payment> payments;

	Database::TDBTransaction transaction(dmMMData->dbMenuMate);

	TIBSQL* paymentsQuery = transaction.Query(transaction.AddQuery());
	paymentsQuery->SQL->Text =
		"Select "
        "   case when (arcbillpay.pay_type = 'Eftpos' "
        "              and arcbillpay.cash_out = 'T') then "
        "             'Eftpos Cash Out' "
        "        else arcbillpay.pay_type "
        "   end Pay_Type, "
			"ArcBillPay.Group_Number,"
			"Sum (ArcBillPay.SubTotal) SubTotal,"
			"Count (ArcBillPay.ArcBillPay_Key) Trans_Count "
		"From "
			"(Security Left Outer Join ArcBill on "
				"Security.Security_Ref = ArcBill.Security_Ref) "
			"Left Outer Join ArcBillPay on "
				"ArcBill.ArcBill_Key = ArcBillPay.ArcBill_Key "
		"Where "
			"Security.Time_Stamp > :StartTime and "
			"Security.Time_Stamp <= :EndTime and "
			"ArcBillPay.Properties != 131072 and "
			"Security.Security_Event = 'Billed By' and "
			"Security.Terminal_Name = :Terminal "
		"Group By "
			"Security.Terminal_Name,"
			"ArcBillPay.Pay_Type,"
			"ArcBillPay.Group_Number,"
			"ArcBillPay.Properties, "
            "ArcBillPay.Cash_Out "
		"Having "
			"Count (ArcBillPay.ArcBillPay_Key) > 0 "
		"Order By "
			"Security.Terminal_Name Asc, "
			"ArcBillPay.Pay_Type Asc, "
            "ArcBillPay.Cash_Out ";

	TIBSQL* adjustmentsQuery = transaction.Query(transaction.AddQuery());

	adjustmentsQuery->SQL->Text =
		"Select "
			"BlindBalance_Key,"
			"Office_Balance "
		"From "
			"BlindBalance "
		"Where "
			"Z_Key = :Key and "
			"Payment = :PayType";

	transaction.Start();

	TIBSQL* insertQuery = transaction.Query(transaction.AddQuery());
	insertQuery->SQL->Text = INSERT_BALANCE_SQL;

	paymentsQuery->ParamByName("StartTime")->AsDateTime = _previousZCreatedDateTime;
	paymentsQuery->ParamByName("EndTime")->AsDateTime = _createdDateTime;
	paymentsQuery->ParamByName("Terminal")->AsString = _terminalName;

	for (paymentsQuery->ExecQuery(); !paymentsQuery->Eof; paymentsQuery->Next())
	{
		adjustmentsQuery->ParamByName("Key")->AsInteger = _id;
		adjustmentsQuery->ParamByName("PayType")->AsString =
			paymentsQuery->FieldByName("Pay_Type")->AsString;
        adjustmentsQuery->ExecQuery();

		if (adjustmentsQuery->Eof) {
            insertQuery->Close();
            insertQuery->ParamByName("z_key")->AsInteger = _id;
            insertQuery->ParamByName("payment")->AsString =
              paymentsQuery->FieldByName("pay_type")->AsString;
            insertQuery->ParamByName("payment_group")->AsInteger =
              paymentsQuery->FieldByName("group_number")->AsInteger;
            insertQuery->ParamByName("payment_trans_qty")->AsInteger =
              paymentsQuery->FieldByName("trans_count")->AsInteger;
            insertQuery->ParamByName("system_balance")->AsCurrency =
              paymentsQuery->FieldByName("subtotal")->AsCurrency;
            insertQuery->ParamByName("office_balance")->AsInteger = 0;
            insertQuery->ExecQuery();

            adjustmentsQuery->Close();
            adjustmentsQuery->ExecQuery();
        }

		Currency officeBalance =
          adjustmentsQuery->FieldByName("Office_Balance")->AsCurrency;

		payments.insert(Payment(
			adjustmentsQuery->FieldByName("BlindBalance_Key")->AsInteger,
			paymentsQuery->FieldByName("Pay_Type")->AsString,
			paymentsQuery->FieldByName("Group_Number")->AsInteger,
			paymentsQuery->FieldByName("Trans_Count")->AsInteger,
			paymentsQuery->FieldByName("SubTotal")->AsCurrency,
			officeBalance
		));

		/* dgw:201104191503: adjustmentsQuery is closed to release whatever
         * assets it has acquired on each iteration. But...
	     */
		adjustmentsQuery->Close();
	}

	transaction.Commit();

	/* dgw:201104191503: paymentsQuery is not? Seems like some kind of leak.
	 * Added the release at the time mentioned. On mysterious crashes,
	 * blame me :P
	 */
	paymentsQuery->Close();

	return payments;
}
//---------------------------------------------------------------------------
void Zed::SavePaymentAdjustments(const std::set<Payment>& payments)
{
	Database::TDBTransaction transaction(dmMMData->dbMenuMate);

	TIBSQL* updateQuery = transaction.Query(transaction.AddQuery());
	updateQuery->SQL->Text =
		"Update "
			"BlindBalance "
		"Set "
			"Office_Balance = :Office_Balance, "
            "System_Balance = :System_Balance "
		"Where "
			"BlindBalance_Key = :Key";

	TIBSQL* insertQuery = transaction.Query(transaction.AddQuery());
	insertQuery->SQL->Text = INSERT_BALANCE_SQL;

	transaction.Start();

	_totalVariance = 0;

	std::set<Payment>::const_iterator i;

	for (i=payments.begin(); i!=payments.end(); ++i)
	{
        if (!i->GetId()) {
			insertQuery->ParamByName("Z_Key")->AsInteger = _id;
			insertQuery->ParamByName("Payment")->AsString = i->GetType();
			insertQuery->ParamByName("Payment_Group")->AsInteger = i->GetGroup();
			insertQuery->ParamByName("Payment_Trans_Qty")->AsCurrency = i->GetQuantity();
			insertQuery->ParamByName("System_Balance")->AsCurrency = i->GetCalculatedValue();
			insertQuery->ParamByName("Office_Balance")->AsCurrency = i->GetCountedValue();
			insertQuery->ExecQuery();
        } else {
			updateQuery->ParamByName("Key")->AsInteger = i->GetId();
			updateQuery->ParamByName("Office_Balance")->AsCurrency = i->GetCountedValue();
			updateQuery->ParamByName("System_Balance")->AsCurrency = i->GetCalculatedValue();
			updateQuery->ExecQuery();
		}

		_totalVariance += i->GetVariance();
	}

	TIBSQL* updateZQuery = transaction.Query(transaction.AddQuery());
	updateZQuery->SQL->Text =
		"Update "
			"Zeds "
		"Set "
			"Adjustments = :Variance "
		"Where "
			"Z_Key = :Key";

	updateZQuery->ParamByName("Key")->AsInteger = _id;
	updateZQuery->ParamByName("Variance")->AsCurrency = _totalVariance;
	updateZQuery->ExecQuery();

	transaction.Commit();

	_reconciled = true;
}
//---------------------------------------------------------------------------
void Zed::GetPreviousZCreatedDateTime() const
{
	Database::TDBTransaction transaction(dmMMData->dbMenuMate);

	TIBSQL* query = transaction.Query(transaction.AddQuery());

	query->SQL->Text =
		"Select "
			"Zeds.Time_Stamp "
		"From "
			"Zeds "
		"Where "
			"Zeds.Time_Stamp < :StartTime and "
			"Terminal_Name = :Terminal "
		"Order By "
			"Zeds.Time_Stamp desc";

	transaction.Start();

	query->ParamByName("StartTime")->AsDateTime = _createdDateTime;
	query->ParamByName("Terminal")->AsString = _terminalName;
	query->ExecQuery();
	if (!query->Eof)
	{
		_previousZCreatedDateTime = query->FieldByName("Time_Stamp")->AsDateTime;
	}
	else
	{
		_previousZCreatedDateTime = TDateTime(0);
	}

	transaction.Commit();
}
//---------------------------------------------------------------------------
AnsiString Zed::GetOriginalReport() const
{
	Database::TDBTransaction transaction(dmMMData->dbMenuMate);

	TIBSQL* query = transaction.Query(transaction.AddQuery());

	query->SQL->Text =
		"Select "
			"Report "
		"From "
			"Zeds "
		"Where "
			"Zeds.Z_Key = :Key";

	transaction.Start();

	AnsiString s;

	query->ParamByName("Key")->AsInteger = _id;
	query->ExecQuery();
	if (!query->Eof)
	{
		s = query->FieldByName("Report")->AsString;
	}

	AnsiString stripped;
	for (int i=1; i<= s.Length(); ++i)
	{
		if (s[i] == 0x1B)
		{
			/*if (
				s[i] == CUT_MODE ||
				s[i] == */
			i++;
		}
		else if(s[i] == char(196))
		{
			stripped += "-";
		}
		else if(s[i] == char(205))
		{
			stripped += "=";
		}

		else if (isprint(s[i]) || s[i] == '\r' || s[i] == '\n')
		{
			stripped += s[i];
		}
	}

	transaction.Commit();

	return stripped;
}
//---------------------------------------------------------------------------

