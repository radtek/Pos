#include <vcl.h>
#pragma hdrstop

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef  PalmMate
#include "Palm.h"
#endif

#include "Requests.h"
#include "ListPaymentSystem.h"
#include "Printing.h"
#include "PrintOut.h"
#include "GetMoney.h"
#include "Paytype.h"
#include "AddTab.h"
#include "CSV.h"
#include "Tabs.h"
#include "DBSaleTimes.h"
#include "Invoice.h"
#include "MMMessageBox.h"
#include "Orders.h"
#include "Eftpos.h"

#include "EftPosDialogs.h"
//#include "EftposSyncroPinPadCTRL.h"

#include "CardSwipe.h"
#include "MMTouchKeyboard.h"
#include "MMTouchNumpad.h"
#include "EftChequeVerify.h"
#include "ManagerVariable.h"
#include "TableManager.h"
#include "Rooms.h"
#include "SplitPayment.h"
#include "ReceiptManager.h"
#include "ManagerThirdParty.h"
#include "PhoenixHotelSystem.h"
#include "FreebieManager.h"
#include "ManagerPatron.h"
#include "Membership.h"
#include "Processing.h"
#include "ProcessingController.h"
#include "IMManager.h"
#include "OrderUtils.h"
#include "Comms.h"
#include "ListSecurityRefContainer.h"
#include "ReqPrintJob.h"

HWND hEdit1 = NULL, hEdit2 = NULL, hEdit3 = NULL, hEdit4 = NULL;

BOOL CALLBACK EnumChildWindowsProc(HWND hwnd, LPARAM lParam)
{
   try
   {
	  char Buffer[32];
	  GetWindowText(hwnd, Buffer, 32);
	  AnsiString WindowName = Buffer;

	  char lpClassName[32];
	  if (GetClassName(hwnd, lpClassName, 32))
	  {
		 AnsiString ClassName(lpClassName);
		 if (ClassName == "Edit")
		 {
			if (IsWindowEnabled(hwnd))
			{
			   WINDOWPLACEMENT wndpl;
			   if (GetWindowPlacement(hwnd, &wndpl))
			   {
				  if (wndpl.rcNormalPosition.left == 195 && wndpl.rcNormalPosition.top == 171 && wndpl.rcNormalPosition.right == 267 &&
					 wndpl.rcNormalPosition.bottom == 195)
				  {
					 hEdit1 = hwnd;
				  }
				  if (wndpl.rcNormalPosition.left == 290 && wndpl.rcNormalPosition.top == 171 && wndpl.rcNormalPosition.right == 343 &&
					 wndpl.rcNormalPosition.bottom == 195)
				  {
					 hEdit2 = hwnd;
				  }
				  if (wndpl.rcNormalPosition.left == 195 && wndpl.rcNormalPosition.top == 210 && wndpl.rcNormalPosition.right == 267 &&
					 wndpl.rcNormalPosition.bottom == 234)
				  {
					 hEdit3 = hwnd;
				  }
				  if (wndpl.rcNormalPosition.left == 290 && wndpl.rcNormalPosition.top == 210 && wndpl.rcNormalPosition.right == 343 &&
					 wndpl.rcNormalPosition.bottom == 234)
				  {
					 hEdit4 = hwnd;
				  }
			   }
			}
		 }
	  }
	  return TRUE;
   }
   catch(Exception & E)
   {
	  ShowMessage(E.Message);
	  return FALSE;
   }
}
// ---------------------------------------------------------------------------

TListPaymentSystem::TListPaymentSystem() : ManagerReference(new TManagerReference)
{
   Busy = false;
   ForceReceiptWithCashContent = false;
   Security = new TListSecurityRefContainer;
   EftPosEnabled = true;
   LastReceipt = NULL;
}

__fastcall TListPaymentSystem::~TListPaymentSystem()
{
   delete Security;
   if (LastReceipt != NULL)
   {
	  delete LastReceipt;
   }
}

void TListPaymentSystem::PaymentsReload(TPaymentTransaction &PaymentTransaction)
{
   PaymentTransaction.PaymentsClear();
   PaymentsLoadTypes(PaymentTransaction);
}

void TListPaymentSystem::PaymentLoad(Database::TDBTransaction &DBTransaction, int PaymentKey, TPayment &Payment)
{
   try
   {
	  // Used by the Payment Editor see PaymentsLoadTypes for the Payment System Loader.
	  Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery);

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = " SELECT * FROM PAYMENTTYPES" " WHERE PAYMENT_KEY = :PAYMENT_KEY" " ORDER BY PAYMENTTYPES.DISPLAY_ORDER";
	  IBInternalQuery->ParamByName("PAYMENT_KEY")->AsInteger = PaymentKey;
	  IBInternalQuery->ExecQuery();
	  for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	  {
		 Payment.Name = IBInternalQuery->FieldByName("PAYMENT_NAME")->AsString;
		 Payment.Properties = IBInternalQuery->FieldByName("PROPERTIES")->AsInteger;
		 Payment.DisplayOrder = IBInternalQuery->FieldByName("DISPLAY_ORDER")->AsInteger;
		 Payment.Colour = IBInternalQuery->FieldByName("COLOUR")->AsInteger;
		 Payment.AmountAdjust = IBInternalQuery->FieldByName("AMOUNT_ADJUST")->AsFloat;
		 Payment.PercentAdjust = IBInternalQuery->FieldByName("PERCENT_ADJUST")->AsFloat;
		 Payment.RoundTo = IBInternalQuery->FieldByName("ROUNDTO")->AsCurrency;
		 Payment.TaxRate = IBInternalQuery->FieldByName("TAX_RATE")->AsFloat;
		 Payment.AdjustmentReason = IBInternalQuery->FieldByName("ADJUST_REASON")->AsString;
		 Payment.GroupNumber = IBInternalQuery->FieldByName("GROUP_NUMBER")->AsInteger;
		 Payment.SecondaryPMSIPAddress = IBInternalQuery->FieldByName("DEST_IP")->AsString;
		 Payment.SecondaryPMSPortNumber = IBInternalQuery->FieldByName("DEST_PORT")->AsInteger;
		 Payment.FixedVoucherCode = IBInternalQuery->FieldByName("PRE_VOUCHER_CODE")->AsString;
		 Payment.VoucherMerchantID = IBInternalQuery->FieldByName("VOUCHER_MERCHANT_ID")->AsString;

		 Payment.PaymentThirdPartyID = TDBThirdPartyCodes::GetThirdPartyCodeByKey(DBTransaction,
			IBInternalQuery->FieldByName("THIRDPARTYCODES_KEY")->AsInteger);
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

void TListPaymentSystem::PaymentLoad(Database::TDBTransaction &DBTransaction, TStringList *List)
{
   try
   {
	  // Used by the Payment Editor see PaymentsLoadTypes for the Payment System Loader.
	  Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery);

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = " SELECT PAYMENT_KEY,PAYMENT_NAME" " FROM PAYMENTTYPES ORDER BY PAYMENTTYPES.DISPLAY_ORDER";
	  IBInternalQuery->ExecQuery();
	  for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	  {
		 int Index = List->Add(IBInternalQuery->FieldByName("PAYMENT_NAME")->AsString);
		 // Little bit of pointer abuse but we just want to store the int somewhere.
		 List->Objects[Index] = (TObject*)IBInternalQuery->FieldByName("PAYMENT_KEY")->AsInteger;
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

void TListPaymentSystem::PaymentDelete(Database::TDBTransaction &DBTransaction, int PaymentKey)
{
   try
   {
	  // Used by the Payment Editor see PaymentsLoadTypes for the Payment System Loader.
	  Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery);

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "DELETE FROM PAYMENTTYPES WHERE PAYMENT_KEY = :PAYMENT_KEY";
	  IBInternalQuery->ParamByName("PAYMENT_KEY")->AsInteger = PaymentKey;
	  IBInternalQuery->ExecQuery();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

void TListPaymentSystem::PaymentSave(Database::TDBTransaction &DBTransaction, int &PaymentKey, TPayment &Payment)
{
   try
   {
	  // Used by the Payment Editor see PaymentsLoadTypes for the Payment System Loader.
	  Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery);

	  if (PaymentKey != 0)
	  {
		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = " UPDATE PAYMENTTYPES " " SET " " PAYMENT_NAME = :PAYMENT_NAME, " " PROPERTIES = :PROPERTIES, "
			 " EXCHANGE_RATE = :EXCHANGE_RATE, " " COLOUR = :COLOUR, " " DISPLAY_ORDER = :DISPLAY_ORDER, "
			 " PERCENT_ADJUST = :PERCENT_ADJUST, " " AMOUNT_ADJUST = :AMOUNT_ADJUST, " " ADJUST_REASON = :ADJUST_REASON, "
			 " ROUNDTO = :ROUNDTO, " " GROUP_NUMBER = :GROUP_NUMBER, " " THIRDPARTYCODES_KEY = :THIRDPARTYCODES_KEY, "
			 " TAX_RATE = :TAX_RATE, " " DEST_IP = :DEST_IP, " " DEST_PORT = :DEST_PORT, " " PRE_VOUCHER_CODE = :PRE_VOUCHER_CODE, "
			 " VOUCHER_MERCHANT_ID = :VOUCHER_MERCHANT_ID " " WHERE " "  PAYMENT_KEY = :PAYMENT_KEY ";
		 IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = Payment.Name;
		 IBInternalQuery->ParamByName("PROPERTIES")->AsInteger = Payment.Properties;
		 IBInternalQuery->ParamByName("EXCHANGE_RATE")->AsCurrency = 0.0;
		 IBInternalQuery->ParamByName("COLOUR")->AsInteger = Payment.Colour;
		 IBInternalQuery->ParamByName("DISPLAY_ORDER")->AsInteger = Payment.DisplayOrder;
		 IBInternalQuery->ParamByName("PERCENT_ADJUST")->AsCurrency = Payment.PercentAdjust;
		 IBInternalQuery->ParamByName("AMOUNT_ADJUST")->AsCurrency = Payment.AmountAdjust;
		 IBInternalQuery->ParamByName("ROUNDTO")->AsCurrency = Payment.RoundTo;
		 IBInternalQuery->ParamByName("ADJUST_REASON")->AsString = Payment.AdjustmentReason;
		 IBInternalQuery->ParamByName("GROUP_NUMBER")->AsInteger = Payment.GroupNumber;
		 IBInternalQuery->ParamByName("DEST_IP")->AsString = Payment.SecondaryPMSIPAddress;
		 IBInternalQuery->ParamByName("DEST_PORT")->AsInteger = Payment.SecondaryPMSPortNumber;
		 IBInternalQuery->ParamByName("PRE_VOUCHER_CODE")->AsString = Payment.FixedVoucherCode;
		 IBInternalQuery->ParamByName("VOUCHER_MERCHANT_ID")->AsString = Payment.VoucherMerchantID;

		 IBInternalQuery->ParamByName("TAX_RATE")->AsCurrency = Payment.TaxRate;
		 if (Payment.PaymentThirdPartyID != "")
		 {
			int ThirdPartyCodeKey = TDBThirdPartyCodes::SetThirdPartyCode(DBTransaction, Payment.PaymentThirdPartyID, "Payment Type Code",
			   tpPaymentType);
			IBInternalQuery->ParamByName("THIRDPARTYCODES_KEY")->AsInteger = ThirdPartyCodeKey;
		 }
		 else
		 {
			IBInternalQuery->ParamByName("THIRDPARTYCODES_KEY")->Clear();
		 }
		 IBInternalQuery->ParamByName("PAYMENT_KEY")->AsInteger = PaymentKey;
		 IBInternalQuery->ExecQuery();
	  }
	  else
	  {
		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_PAYMENTTYPES, 1) FROM RDB$DATABASE";
		 IBInternalQuery->ExecQuery();
		 PaymentKey = IBInternalQuery->Fields[0]->AsInteger;

		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text =
			 "INSERT INTO PAYMENTTYPES (" "PAYMENT_KEY, " "PAYMENT_NAME, " "PROPERTIES, " "EXCHANGE_RATE, " "COLOUR, "
			 "DISPLAY_ORDER, " "PERCENT_ADJUST, " "AMOUNT_ADJUST, " "ROUNDTO, " "ADJUST_REASON, " "GROUP_NUMBER, " "THIRDPARTYCODES_KEY, "
			 "DEST_IP," "DEST_PORT," "TAX_RATE," "PRE_VOUCHER_CODE," "VOUCHER_MERCHANT_ID) " "VALUES (" ":PAYMENT_KEY, " ":PAYMENT_NAME, "
			 ":PROPERTIES, " ":EXCHANGE_RATE, " ":COLOUR, " ":DISPLAY_ORDER, " ":PERCENT_ADJUST, " ":AMOUNT_ADJUST, " ":ROUNDTO, "
			 ":ADJUST_REASON, " ":GROUP_NUMBER, " ":THIRDPARTYCODES_KEY, " ":DEST_IP," ":DEST_PORT," ":TAX_RATE," ":PRE_VOUCHER_CODE,"
			 ":VOUCHER_MERCHANT_ID) ";

		 IBInternalQuery->ParamByName("PAYMENT_KEY")->AsInteger = PaymentKey;
		 IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = Payment.Name;
		 IBInternalQuery->ParamByName("PROPERTIES")->AsInteger = Payment.Properties;
		 IBInternalQuery->ParamByName("EXCHANGE_RATE")->AsCurrency = 0.0;
		 IBInternalQuery->ParamByName("COLOUR")->AsInteger = Payment.Colour;
		 IBInternalQuery->ParamByName("DISPLAY_ORDER")->AsInteger = Payment.DisplayOrder;
		 IBInternalQuery->ParamByName("PERCENT_ADJUST")->AsCurrency = Payment.PercentAdjust;
		 IBInternalQuery->ParamByName("AMOUNT_ADJUST")->AsCurrency = Payment.AmountAdjust;
		 IBInternalQuery->ParamByName("ROUNDTO")->AsCurrency = Payment.RoundTo;
		 IBInternalQuery->ParamByName("ADJUST_REASON")->AsString = Payment.AdjustmentReason;
		 IBInternalQuery->ParamByName("GROUP_NUMBER")->AsInteger = Payment.GroupNumber;
		 IBInternalQuery->ParamByName("DEST_IP")->AsString = Payment.SecondaryPMSIPAddress;
		 IBInternalQuery->ParamByName("DEST_PORT")->AsInteger = Payment.SecondaryPMSPortNumber;
		 IBInternalQuery->ParamByName("TAX_RATE")->AsCurrency = Payment.TaxRate;
		 IBInternalQuery->ParamByName("PRE_VOUCHER_CODE")->AsString = Payment.FixedVoucherCode;
		 IBInternalQuery->ParamByName("VOUCHER_MERCHANT_ID")->AsString = Payment.VoucherMerchantID;

		 if (Payment.PaymentThirdPartyID != "")
		 {
			int ThirdPartyCodeKey = TDBThirdPartyCodes::SetThirdPartyCode(DBTransaction, Payment.PaymentThirdPartyID, "Payment Type Code",
			   tpPaymentType);
			IBInternalQuery->ParamByName("THIRDPARTYCODES_KEY")->AsInteger = ThirdPartyCodeKey;
		 }
		 else
		 {
			IBInternalQuery->ParamByName("THIRDPARTYCODES_KEY")->Clear();
		 }
		 IBInternalQuery->ExecQuery();
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

void TListPaymentSystem::PaymentsLoadTypes(TPaymentTransaction &PaymentTransaction)
{
   Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
   PaymentTransaction.DBTransaction.RegisterQuery(IBInternalQuery);

   int CurrentDisplayOrder = 0;

   if (PaymentTransaction.Membership.Member.ContactKey != 0)
   { // You cant use points as a Credit Payment Type.
	  // Points will still be credited though.

	  TPayment *NewPayment = new TPayment;
	  NewPayment->Name = PaymentTransaction.Membership.Member.Name + "'s Points";
	  NewPayment->SysNameOveride = "Points";
	  NewPayment->Properties |= ePayTypePoints;
	  NewPayment->DisplayOrder = 1;
	  NewPayment->GroupNumber = TGlobalSettings::Instance().PointsPaymentGroupNumber;
	  NewPayment->Colour = clTeal;
	  PaymentTransaction.PaymentAdd(NewPayment);
   }

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = " SELECT * FROM PAYMENTTYPES ORDER BY PAYMENTTYPES.DISPLAY_ORDER";
   IBInternalQuery->ExecQuery();
   for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
   {
	  TPayment *NewPayment = new TPayment;
	  NewPayment->Name = IBInternalQuery->FieldByName("PAYMENT_NAME")->AsString;
	  NewPayment->Properties = IBInternalQuery->FieldByName("PROPERTIES")->AsInteger;
	  NewPayment->DisplayOrder = IBInternalQuery->FieldByName("DISPLAY_ORDER")->AsInteger;
	  NewPayment->Colour = IBInternalQuery->FieldByName("COLOUR")->AsInteger;
	  NewPayment->AmountAdjust = IBInternalQuery->FieldByName("AMOUNT_ADJUST")->AsFloat;
	  NewPayment->PercentAdjust = IBInternalQuery->FieldByName("PERCENT_ADJUST")->AsFloat;
	  NewPayment->RoundTo = IBInternalQuery->FieldByName("ROUNDTO")->AsCurrency;
	  NewPayment->TaxRate = IBInternalQuery->FieldByName("TAX_RATE")->AsFloat;
	  NewPayment->AdjustmentReason = IBInternalQuery->FieldByName("ADJUST_REASON")->AsString;
	  NewPayment->GroupNumber = IBInternalQuery->FieldByName("GROUP_NUMBER")->AsInteger;
	  NewPayment->SecondaryPMSIPAddress = IBInternalQuery->FieldByName("DEST_IP")->AsString;
	  NewPayment->SecondaryPMSPortNumber = IBInternalQuery->FieldByName("DEST_PORT")->AsInteger;
	  NewPayment->FixedVoucherCode = IBInternalQuery->FieldByName("PRE_VOUCHER_CODE")->AsString;
	  NewPayment->VoucherMerchantID = IBInternalQuery->FieldByName("VOUCHER_MERCHANT_ID")->AsString;
	  NewPayment->PaymentThirdPartyID = TDBThirdPartyCodes::GetThirdPartyCodeByKey(PaymentTransaction.DBTransaction,
		 IBInternalQuery->FieldByName("THIRDPARTYCODES_KEY")->AsInteger);
	  CurrentDisplayOrder = NewPayment->DisplayOrder;
	  PaymentTransaction.PaymentAdd(NewPayment);
   }

   TPayment *CashPayment = PaymentTransaction.PaymentFind(CASH);
   if (CashPayment == NULL)
   {
	  TPayment *NewPayment = new TPayment;
	  NewPayment->Name = CASH;
	  NewPayment->Properties |= ePayTypeOpensCashDrawer;
	  NewPayment->Properties |= ePayTypeCash;
	  NewPayment->DisplayOrder = -1;
	  NewPayment->Colour = clGreen;
	  PaymentTransaction.PaymentAdd(NewPayment);
   }

   PaymentTransaction.ProcessPoints();

   PaymentTransaction.Money.Recalc(PaymentTransaction);

   if (PaymentTransaction.Type == eTransCreditPurchase)
   {
	  for (std::map <long, TTabCredit> ::iterator itTabCredit = PaymentTransaction.TabCredit.begin();
		 itTabCredit != PaymentTransaction.TabCredit.end(); advance(itTabCredit, 1))
	  {
		 TPayment *NewPayment = new TPayment;
		 NewPayment->Name = TDBTab::GetTabName(PaymentTransaction.DBTransaction, itTabCredit->first).SubString(1, 15);
		 NewPayment->NameOveride = CREDIT;
		 NewPayment->Properties |= ePayTypeCredit;
		 NewPayment->DisplayOrder = -2;
		 NewPayment->Colour = clRed;
		 NewPayment->SetPay(itTabCredit->second.CreditRedeemed);
		 NewPayment->TabCreditKey = itTabCredit->first;
		 NewPayment->GroupNumber = itTabCredit->second.PaymentGroupNumber;
		 NewPayment->Visible = false;
		 PaymentTransaction.PaymentAdd(NewPayment);
	  }
   }
   else
   {
	  std::set <__int64> TabKeys; // Tabs from which the orders come from.
	  TDBOrder::GetTabKeysFromOrders(PaymentTransaction.Orders, TabKeys);
	  PaymentTransaction.TabCredit.clear();

	  for (std::set <__int64> ::iterator itTabs = TabKeys.begin(); itTabs != TabKeys.end(); advance(itTabs, 1))
	  {
		 Currency TabCurrentCredit = TDBTab::GetTabCredit(PaymentTransaction.DBTransaction, *itTabs);
		 if (TabCurrentCredit != 0)
		 {
			PaymentTransaction.Money.CreditAvailable += TabCurrentCredit;

			TTabCredit Credit = PaymentTransaction.TabCredit[*itTabs];
			Credit.CurrentCredit = TabCurrentCredit;
			PaymentTransaction.TabCredit[*itTabs] = Credit;

			TPayment *NewPayment = new TPayment;
			NewPayment->Name = TDBTab::GetTabName(PaymentTransaction.DBTransaction, *itTabs).SubString(1, 15);
			NewPayment->NameOveride = CREDIT;
			NewPayment->Properties |= ePayTypeCredit;
			NewPayment->DisplayOrder = -2;
			NewPayment->Colour = clRed;
			NewPayment->SetPay(Credit.CreditRedeemed);
			NewPayment->TabCreditKey = *itTabs;
			NewPayment->GroupNumber = Credit.PaymentGroupNumber;
			NewPayment->Visible = true;
			PaymentTransaction.PaymentAdd(NewPayment);
		 }
	  }
   }

   PaymentTransaction.Money.Recalc(PaymentTransaction);
}

bool TListPaymentSystem::ProcessTransaction(TPaymentTransaction &PaymentTransaction)
{
   bool PaymentComplete = false;
   try
   {
	  Busy = true;
	  bool PaymentAborted = false;
	  bool RequestEFTPOSReceipt = false;

	  std::auto_ptr <TfrmPaymentType> frmPaymentType(TfrmPaymentType::Create(Screen->ActiveForm, PaymentTransaction));
	  std::auto_ptr <TfrmControlTransaction> frmControlTransaction(TfrmControlTransaction::Create(Screen->ActiveForm, PaymentTransaction));

	  TMMProcessingState State(Screen->ActiveForm, "Processing Bill", "Processing Bill");

	  TMMContactInfo TempUserInfo;
	  TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->GetContactInfo(TempUserInfo);
	  if (TempUserInfo.Valid())
	  {
		 State.Message = "Do NOT remove smartcard. Please Wait.";
		 State.Title = "Do NOT remove smartcard.";
	  }

	  TForm *BackGround = Screen->ActiveForm;

	  if (PaymentTransaction.Type == eTransEFTPOSRecovery || PaymentTransaction.Type == eTransRewardsRecovery)
	  {
		 frmPaymentType->Recovering = true;
	  }
	  else if (PaymentTransaction.Type == eTransQuickSale)
	  {
		 Security->SecurityClear();
	  }
	  else
	  {
		 PaymentsReload(PaymentTransaction);
		 Security->SecurityClear();
	  }

	  if (PaymentTransaction.SalesType == eAccount)
	  {
		 PaymentTransaction.Money.InvoiceTotal = PaymentTransaction.Money.TotalOwing;
		 PaymentTransaction.References.push_back(RefRefType(Invoice->GetInvoiceNumber(PaymentTransaction.DBTransaction,
				  PaymentTransaction.InvoiceKey), ManagerReference->GetReferenceByType(PaymentTransaction.DBTransaction, REFTYPE_INVOICE))
			 );
	  }

	  PreparePayments(PaymentTransaction);
	  // Retrive this Receipts Security Ref.
	  Security->SetSecurityRefNumber(TDBSecurity::GetNextSecurityRef(PaymentTransaction.DBTransaction));

	  if (PaymentTransaction.Type == eTransOrderSet && PaymentTransaction.Orders != NULL)
	  {
		 std::set <__int64> TabKeys; // Tabs from which the orders come from.
		 TDBOrder::GetTabKeysFromOrders(PaymentTransaction.Orders, TabKeys);

		 PaymentTransaction.Money.Recalc(PaymentTransaction);

		 frmPaymentType->QueryPatronCount = TGlobalSettings::Instance().PromptForPatronCount;
		 while (!PaymentComplete && !PaymentAborted)
		 {
			if (frmPaymentType->Execute() == mrOk)
			{
			   if (frmControlTransaction->RetrieveUserOptions() != eBack)
			   {
				  TDeviceRealTerminal::Instance().ManagerMembership->BeginMemberTransaction();
				  PaymentTransaction.ProcessPoints();
				  TDeviceRealTerminal::Instance().ProcessingController.PushOnce(State);
				  PaymentComplete = ProcessThirdPartyModules(PaymentTransaction, RequestEFTPOSReceipt);

				  if (PaymentComplete)
				  {
					 if (BackGround != NULL)
						BackGround->Repaint();
					 TDeviceRealTerminal::Instance().ProcessingController.Repaint();

					 UpdateFreebieRewards(PaymentTransaction);
					 SetInvoiceNumber(PaymentTransaction);
					 ReceiptPrepare(PaymentTransaction, RequestEFTPOSReceipt);
					 StoreInfo(PaymentTransaction);
					 ProcessRewardSchemes(PaymentTransaction);
					 ArchiveTransaction(PaymentTransaction);
					 BuildXMLTransaction(PaymentTransaction);
					 OpenCashDrawer(PaymentTransaction);
					 RemoveOrders(PaymentTransaction);
					 AdjustCredit(PaymentTransaction);
				  }
			   }
			   else
			   {
				  PaymentComplete = false;
				  PaymentTransaction.PaymentsReset();
				  frmPaymentType->EnableElectronicPayments();
			   }
			}
			else
			{
			   ClearDiscounts(PaymentTransaction);
			   PaymentAborted = true;
			}
		 }
	  }
	  else if (PaymentTransaction.Type == eTransSplitPayment)
	  {
		 std::set <__int64> TabKeys; // Tabs from which the orders come from.
		 TDBOrder::GetTabKeysFromOrders(PaymentTransaction.Orders, TabKeys);
		 std::set <__int64> ::iterator itTabs = TabKeys.begin();
		 if (itTabs != TabKeys.end())
		 {
			PaymentTransaction.SplitMoney.DivisionsLeft = TDBTab::GetSplitDivisions(PaymentTransaction.DBTransaction, *itTabs);
		 }

		 std::auto_ptr <TList> ClonedOrdersList(new TList);
		 double SplitPercentage = 0;
		 if (SplitPayment(PaymentTransaction, &PaymentTransaction.SplitMoney, SplitPercentage))
		 {
			if (PaymentTransaction.SplitMoney.LeftOwing != 0)
			{
			   TDBOrder::CloneOrdersPartialPaymentDifference(PaymentTransaction.DBTransaction, PaymentTransaction.Orders,
				  ClonedOrdersList.get(), SplitPercentage, PaymentTransaction.SplitMoney.PaymentAmount);
			}

			PaymentTransaction.Money.Recalc(PaymentTransaction);

			TManagerPatron::Instance().SetDefaultPatrons(PaymentTransaction.DBTransaction, PaymentTransaction.Patrons,
			   PaymentTransaction.SplitMoney.NumberOfPayments);

			frmPaymentType->btnPrelim->Visible = false;

			// Load points.
			PaymentsReload(PaymentTransaction);

			frmPaymentType->QueryPatronCount = TGlobalSettings::Instance().PromptForPatronCount;
			while (!PaymentComplete && !PaymentAborted)
			{
			   if (frmPaymentType->Execute() == mrOk)
			   {
				  if (frmControlTransaction->RetrieveUserOptions() != eBack)
				  {
					 TDeviceRealTerminal::Instance().ManagerMembership->BeginMemberTransaction();
					 PaymentTransaction.ProcessPoints();
					 TDeviceRealTerminal::Instance().ProcessingController.PushOnce(State);
					 PaymentComplete = ProcessThirdPartyModules(PaymentTransaction, RequestEFTPOSReceipt);

					 if (PaymentComplete)
					 {
						if (BackGround != NULL)
						   BackGround->Repaint();
						TDeviceRealTerminal::Instance().ProcessingController.Repaint();

						UpdateFreebieRewards(PaymentTransaction);
						SetInvoiceNumber(PaymentTransaction);
						ReceiptPrepare(PaymentTransaction, RequestEFTPOSReceipt);
						StoreInfo(PaymentTransaction);
						ProcessRewardSchemes(PaymentTransaction);
						ArchiveTransaction(PaymentTransaction);
						BuildXMLTransaction(PaymentTransaction);
						OpenCashDrawer(PaymentTransaction);

						// Save off the cloned orders with whats left of the partial payment.
						for (int i = 0; i < ClonedOrdersList->Count; i++)
						{
						   TItemComplete *Order = (TItemComplete*)ClonedOrdersList->Items[i];
						   Order->OrderKey = 0;
						   Order->TransNo = TDBOrder::GetNextTransNumber(PaymentTransaction.DBTransaction);
							// Patch to fix missing Serving Course.
							if(Order->ServingCourse.ServingCourseKey < 1)
							{
								Order->ServingCourse = TDeviceRealTerminal::Instance().Menus->DefaultServingCourse;
								TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Order has no Serving Course" );
							}
						   TDBOrder::SetOrder(PaymentTransaction.DBTransaction, Order);
						   TDBSecurity::ProcessSecurity(PaymentTransaction.DBTransaction, Order->Security);
						   for (int i = 0; i < Order->SubOrders->Count; i++)
						   {
							  TItemCompleteSub *SubOrder = Order->SubOrders->SubOrderGet(i);
							  if (SubOrder)
							  {
								 TDBSecurity::ProcessSecurity(PaymentTransaction.DBTransaction, SubOrder->Security);
							  }
						   }
						}

						RemoveOrders(PaymentTransaction);
						AdjustCredit(PaymentTransaction);

						for (itTabs = TabKeys.begin(); itTabs != TabKeys.end(); advance(itTabs, 1))
						{
						   TDBTab::SetSplitDivisions(PaymentTransaction.DBTransaction, *itTabs, PaymentTransaction.SplitMoney.DivisionsLeft);
						}
					 }
				  }
				  else
				  {
					 PaymentComplete = false;
					 PaymentTransaction.PaymentsReset();
					 frmPaymentType->EnableElectronicPayments(); // clears all PaymentTypes and reloads them.
				  }
			   }
			   else
			   {
				  ClearDiscounts(PaymentTransaction);
				  frmSplitPayment->DivisionsLeft = 0;
				  PaymentAborted = true;
			   }
			}
		 }
		 else
		 {
			PaymentAborted = true;
		 }

		 while (ClonedOrdersList->Count != 0)
		 {
			delete(TItemComplete*)ClonedOrdersList->Items[0];
			ClonedOrdersList->Delete(0);
		 }

	  }
	  else if (PaymentTransaction.Type == eTransPartialPayment)
	  {
		 /* TODO :
		 Clone the orders and discount them down to the payment amount.
		 Add a discount tothe old order of this amount is it's successful */
		 std::auto_ptr <TList> ClonedOrdersList(new TList);
		 double SplitPercentage = 0;
		 Currency RequestedTotal;
		 int PaymentReturnVal = GetPartialPayment(PaymentTransaction, SplitPercentage, RequestedTotal);
		 if (PaymentReturnVal > 0)
		 {
			// They may be partially paying for the entire Bill in which case PaymentReturnVal == 2;
			if (PaymentReturnVal == 1)
			{
			   TDBOrder::CloneOrdersPartialPaymentDifference(PaymentTransaction.DBTransaction, PaymentTransaction.Orders,
				  ClonedOrdersList.get(), SplitPercentage, RequestedTotal);
			}

			PaymentsReload(PaymentTransaction);
			frmPaymentType->QueryPatronCount = TGlobalSettings::Instance().PromptForPatronCount;
			while (!PaymentComplete && !PaymentAborted)
			{
			   if (frmPaymentType->Execute() == mrOk)
			   {
				  if (frmControlTransaction->RetrieveUserOptions() != eBack)
				  {
					 TDeviceRealTerminal::Instance().ManagerMembership->BeginMemberTransaction();
					 PaymentTransaction.ProcessPoints();
					 TDeviceRealTerminal::Instance().ProcessingController.PushOnce(State);
					 PaymentComplete = ProcessThirdPartyModules(PaymentTransaction, RequestEFTPOSReceipt);

					 if (PaymentComplete)
					 {
						if (BackGround != NULL)
						   BackGround->Repaint();
						TDeviceRealTerminal::Instance().ProcessingController.Repaint();

						UpdateFreebieRewards(PaymentTransaction);
						SetInvoiceNumber(PaymentTransaction);
						ReceiptPrepare(PaymentTransaction, RequestEFTPOSReceipt);
						StoreInfo(PaymentTransaction);
						ProcessRewardSchemes(PaymentTransaction);
						ArchiveTransaction(PaymentTransaction);
						BuildXMLTransaction(PaymentTransaction);
						OpenCashDrawer(PaymentTransaction);

						// Save off the cloned orders with whats left of the partial payment.
						for (int i = 0; i < ClonedOrdersList->Count; i++)
						{
						   TItemComplete *Order = (TItemComplete*)ClonedOrdersList->Items[i];
						   Order->OrderKey = 0;
						   Order->TransNo = TDBOrder::GetNextTransNumber(PaymentTransaction.DBTransaction);
							if(Order->ServingCourse.ServingCourseKey < 1)
							{
								Order->ServingCourse = TDeviceRealTerminal::Instance().Menus->DefaultServingCourse;
								TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Order has no Serving Course" );
							}

						   TDBOrder::SetOrder(PaymentTransaction.DBTransaction, Order);
						   TDBSecurity::ProcessSecurity(PaymentTransaction.DBTransaction, Order->Security);
						   for (int i = 0; i < Order->SubOrders->Count; i++)
						   {
							  TItemCompleteSub *SubOrder = Order->SubOrders->SubOrderGet(i);
							  if (SubOrder)
							  {
								 TDBSecurity::ProcessSecurity(PaymentTransaction.DBTransaction, SubOrder->Security);
							  }
						   }
						}

						RemoveOrders(PaymentTransaction);
						AdjustCredit(PaymentTransaction);
					 }
				  }
				  else
				  {
					 PaymentComplete = false;
					 PaymentTransaction.PaymentsReset();
					 frmPaymentType->EnableElectronicPayments(); // clears all PaymentTypes and reloads them.
				  }
			   }
			   else
			   {
				  ClearDiscounts(PaymentTransaction);
				  PaymentAborted = true;
			   }
			}
		 }

		 while (ClonedOrdersList->Count != 0)
		 {
			delete(TItemComplete*)ClonedOrdersList->Items[0];
			ClonedOrdersList->Delete(0);
		 }

	  }
	  else if (PaymentTransaction.Type == eTransQuickSale)
	  {
		 if (PaymentTransaction.PaymentsCount() > 0)
		 {
			TDeviceRealTerminal::Instance().ManagerMembership->BeginMemberTransaction();
			PaymentTransaction.ProcessPoints();
			TDeviceRealTerminal::Instance().ProcessingController.PushOnce(State);
			PaymentComplete = ProcessThirdPartyModules(PaymentTransaction, RequestEFTPOSReceipt);
			if (PaymentComplete)
			{
			   if (BackGround != NULL)
				  BackGround->Repaint();
			   TDeviceRealTerminal::Instance().ProcessingController.Repaint();

			   UpdateFreebieRewards(PaymentTransaction);
			   SetInvoiceNumber(PaymentTransaction);
			   ReceiptPrepare(PaymentTransaction, RequestEFTPOSReceipt);
			   StoreInfo(PaymentTransaction);
			   ProcessRewardSchemes(PaymentTransaction);
			   ArchiveTransaction(PaymentTransaction);
			   BuildXMLTransaction(PaymentTransaction);
			   OpenCashDrawer(PaymentTransaction);
			   RemoveOrders(PaymentTransaction);
			}
		 }
	  }
	  else if (PaymentTransaction.Type == eTransCreditPurchase)
	  {
		 frmPaymentType->QueryPatronCount = TGlobalSettings::Instance().PromptForPatronCount;
		 while (!PaymentComplete && !PaymentAborted)
		 {
			if (frmPaymentType->Execute() == mrOk)
			{
			   if (frmControlTransaction->RetrieveUserOptions() != eBack)
			   {
				  TDeviceRealTerminal::Instance().ManagerMembership->BeginMemberTransaction();
				  PaymentTransaction.ProcessPoints();
				  TDeviceRealTerminal::Instance().ProcessingController.PushOnce(State);
				  PaymentComplete = ProcessThirdPartyModules(PaymentTransaction, RequestEFTPOSReceipt);

				  if (PaymentComplete)
				  {
					 if (BackGround != NULL)
						BackGround->Repaint();
					 TDeviceRealTerminal::Instance().ProcessingController.Repaint();

					 UpdateFreebieRewards(PaymentTransaction);
					 SetInvoiceNumber(PaymentTransaction);
					 ReceiptPrepare(PaymentTransaction, RequestEFTPOSReceipt);
					 StoreInfo(PaymentTransaction);
					 ProcessRewardSchemes(PaymentTransaction);
					 ArchiveTransaction(PaymentTransaction);
					 BuildXMLTransaction(PaymentTransaction);
					 OpenCashDrawer(PaymentTransaction);
					 RemoveOrders(PaymentTransaction);
					 AdjustCredit(PaymentTransaction);
				  }
			   }
			   else
			   {
				  PaymentComplete = false;
				  PaymentTransaction.PaymentsReset();
				  frmPaymentType->EnableElectronicPayments(); // clears all PaymentTypes and reloads them.
			   }
			}
			else
			{
			   ClearDiscounts(PaymentTransaction);
			   PaymentAborted = true;
			}
		 }
	  }
	  else if (PaymentTransaction.Type == eTransEFTPOSRecovery && PaymentTransaction.Orders != NULL)
	  {
		 PaymentTransaction.Money.Recalc(PaymentTransaction);

		 bool SkipUserOptions = true;
		 frmControlTransaction->UserOption = eCloseandPrint;
		 frmPaymentType->QueryPatronCount = TGlobalSettings::Instance().PromptForPatronCount;
		 while (!PaymentComplete && !PaymentAborted)
		 {
			if (frmPaymentType->Execute() == mrOk)
			{
			   if (SkipUserOptions || frmControlTransaction->RetrieveUserOptions() != eBack)
			   {
				  TDeviceRealTerminal::Instance().ManagerMembership->BeginMemberTransaction();
				  PaymentTransaction.ProcessPoints();
				  TDeviceRealTerminal::Instance().ProcessingController.PushOnce(State);
				  PaymentComplete = ProcessThirdPartyModules(PaymentTransaction, RequestEFTPOSReceipt);

				  if (PaymentComplete)
				  {
					 if (BackGround != NULL)
						BackGround->Repaint();
					 TDeviceRealTerminal::Instance().ProcessingController.Repaint();

					 UpdateFreebieRewards(PaymentTransaction);
					 SetInvoiceNumber(PaymentTransaction);
					 ReceiptPrepare(PaymentTransaction, RequestEFTPOSReceipt);
					 StoreInfo(PaymentTransaction);
					 ProcessRewardSchemes(PaymentTransaction);
					 ArchiveTransaction(PaymentTransaction);
					 BuildXMLTransaction(PaymentTransaction);
					 OpenCashDrawer(PaymentTransaction);
					 RemoveOrders(PaymentTransaction);
					 AdjustCredit(PaymentTransaction);
				  }
				  else
				  {
					 PaymentsReload(PaymentTransaction);
					 // We are no longer recovering. Recovery was declined.
					 // Enable EFTPos again for things other than Inquiryies.
					 PaymentTransaction.Type = eTransTabSet;
				  }
			   }
			   else
			   {
				  PaymentComplete = false;
				  PaymentTransaction.PaymentsReset();
				  frmPaymentType->EnableElectronicPayments(); // clears all PaymentTypes and reloads them.
			   }
			   SkipUserOptions = false;
			}
			else
			{
			   ClearDiscounts(PaymentTransaction);
			   PaymentAborted = true;
			}
		 }
	  }
	  else if (PaymentTransaction.Type == eTransRewardsRecovery && PaymentTransaction.Orders != NULL)
	  {
		 PaymentTransaction.Money.Recalc(PaymentTransaction);

		 bool SkipUserOptions = true;
		 frmControlTransaction->UserOption = eCloseandPrint;
		 frmPaymentType->QueryPatronCount = TGlobalSettings::Instance().PromptForPatronCount;
		 while (!PaymentComplete && !PaymentAborted)
		 {
			if (frmPaymentType->Execute() == mrOk)
			{
			   if (SkipUserOptions || frmControlTransaction->RetrieveUserOptions() != eBack)
			   {
				  TDeviceRealTerminal::Instance().ManagerMembership->BeginMemberTransaction();
				  PaymentTransaction.ProcessPoints();
				  TDeviceRealTerminal::Instance().ProcessingController.PushOnce(State);
				  PaymentComplete = ProcessThirdPartyModules(PaymentTransaction, RequestEFTPOSReceipt);

				  if (PaymentComplete)
				  {
					 if (BackGround != NULL)
						BackGround->Repaint();
					 TDeviceRealTerminal::Instance().ProcessingController.Repaint();

					 UpdateFreebieRewards(PaymentTransaction);
					 SetInvoiceNumber(PaymentTransaction);
					 ReceiptPrepare(PaymentTransaction, RequestEFTPOSReceipt);
					 StoreInfo(PaymentTransaction);
					 ProcessRewardSchemes(PaymentTransaction);
					 BuildXMLTransaction(PaymentTransaction);
					 ArchiveTransaction(PaymentTransaction);
					 OpenCashDrawer(PaymentTransaction);
					 RemoveOrders(PaymentTransaction);
					 AdjustCredit(PaymentTransaction);
				  }
				  else
				  {
					 PaymentsReload(PaymentTransaction);
					 // We are no longer recovering. Recovery was declined.
					 // Enable EFTPos again for things other than Inquiryies.
					 PaymentTransaction.Type = eTransTabSet;
				  }
			   }
			   else
			   {
				  PaymentComplete = false;
				  PaymentTransaction.PaymentsReset();
				  frmPaymentType->EnableElectronicPayments(); // clears all PaymentTypes and reloads them.
			   }
			   SkipUserOptions = false;
			}
			else
			{
			   ClearDiscounts(PaymentTransaction);
			   PaymentAborted = true;
			}
		 }
	  }
	  ClearRecoveryInfo();

	  if ((PaymentTransaction.Type == eTransEFTPOSRecovery) || (PaymentTransaction.Type == eTransRewardsRecovery))
	  {
		 EftPos->ReprintReceipt();
	  }

	  if (PaymentComplete)
	  {
		 ReceiptPrint(PaymentTransaction, RequestEFTPOSReceipt, frmControlTransaction->UserOption == eCloseandPrint);
		 FormatSpendChit(PaymentTransaction);
		 ProcessSecurity(PaymentTransaction);
		 SaveToFileCSV(PaymentTransaction);
		 RemoveTabs(PaymentTransaction); // Remove any empty Tabs.
	  }
	  else
	  {
		 // Last Receipt is a persistant variable.
		 // This payment Transaction isnt.
		 // Since the payment was never completed Receipt Prepare was never called.
		 // So this Payment Transation was never set inside last receipt and the old one
		 // is now invalid. So we remove last receipt.
		 /* TODO : It might be better to ensure Receipt Prepare is always called reguardless of transaction success. */
		 delete LastReceipt;
		 LastReceipt = NULL;
	  }
	  Reset(PaymentTransaction);
	  TDeviceRealTerminal::Instance().ProcessingController.Pop();
	  OnAfterTransactionComplete.Occured();
   }
   catch(Exception & E)
   {
	  TDeviceRealTerminal::Instance().ProcessingController.PopAll();
	  Busy = false;
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   Busy = false;
   return PaymentComplete;
}

void TListPaymentSystem::TransRetriveInvoiceResult(TPaymentTransaction &PaymentTransaction, TPayment *Payment)
{
   // Create an invoice for this bill, mark it as Closed but not exported.
   int InvoiceKey = Invoice->CreateInvoice(PaymentTransaction.DBTransaction, Payment->InvoiceContact.ContactKey,
	  PaymentTransaction.Money.GrandTotal - PaymentTransaction.Money.GSTContent, PaymentTransaction.Money.GrandTotal, "");
   Invoice->SetInvoiceClosed(PaymentTransaction.DBTransaction, InvoiceKey);
   PaymentTransaction.SalesType = eAccount;
   PaymentTransaction.InvoiceKey = InvoiceKey;
   PaymentTransaction.InvoiceNumber = Invoice->GetInvoiceNumber(PaymentTransaction.DBTransaction, InvoiceKey);

   PaymentTransaction.References.push_back(RefRefType(Invoice->GetInvoiceNumber(PaymentTransaction.DBTransaction,
			PaymentTransaction.InvoiceKey), ManagerReference->GetReferenceByType(PaymentTransaction.DBTransaction, REFTYPE_INVOICE)));

   // Update Orders
   for (int i = 0; i < PaymentTransaction.Orders->Count; i++)
   {
	  ((TItemComplete*)PaymentTransaction.Orders->Items[0])->ContainerTabType = TabInvoice;
	  ((TItemComplete*)PaymentTransaction.Orders->Items[0])->InvoiceNumber = PaymentTransaction.InvoiceNumber;
   }

   Payment->Result = eAccepted;
}

void TListPaymentSystem::TransRetriveElectronicResult(TPaymentTransaction &PaymentTransaction, TPayment *Payment)
{
   if (PaymentTransaction.Type == eTransRewardsRecovery)
   { // If we are doing a rewards recovery all EFTPOS has already been accepted.
	  Payment->Result = eAccepted;
   }
   else if (Payment->Properties & ePayTypeIntegratedEFTPOS)
   {
	  if (!EftPos->Enabled)
	  {
		 Application->MessageBox(UnicodeString("Payment type selected is Configured to use Intergrated EFTPOS."
			   " The Intergrated EFTPOS Software has not been installed or enabled correctly."
			   " Please contact your MenuMate support agent.").w_str(), UnicodeString("EFTPOS Configuration Error").w_str(),
			MB_OK + MB_ICONERROR);
		 Payment->Result = eDeclined;
	  }
	  else
	  {
		 EftPos->LastEftPosReceipt->Clear();
		 // Store All OrderKeys;
		 if (Payment->ReferenceNumber == "")
		 {
			Payment->ReferenceNumber = EftPos->GetRefNumber();
		 }

		 bool Abort = false;
		 ePANSource PanSource = PANSource_None; // " ";
		 AnsiString CardString = "";
		 int ExpiryMonth = -1;
		 int ExpiryYear = -1;
		 if ((Payment->Properties & ePayTypeAllowMANPAN) && (PaymentTransaction.Type != eTransEFTPOSRecovery))
		 {
			PanSource = PANSource_Manual; // "K";
			std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (Screen->ActiveForm));
			frmTouchKeyboard->MaxLength = 25;
			frmTouchKeyboard->AllowCarriageReturn = false;
			frmTouchKeyboard->KeyboardText = "";
			frmTouchKeyboard->Caption = "Enter Card Number";
			if (frmTouchKeyboard->ShowModal() == mrOk)
			{
			   CardString = frmTouchKeyboard->KeyboardText;
			   do
			   {
				  std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (Screen->ActiveForm));
				  frmTouchNumpad->Caption = "Enter Card Expiry Month e.g 06";
				  frmTouchNumpad->btnSurcharge->Caption = "Ok";
				  frmTouchNumpad->btnSurcharge->Visible = true;
				  frmTouchNumpad->btnDiscount->Visible = false;
				  frmTouchNumpad->Mode = pmNumber;
				  frmTouchNumpad->INTInitial = 0;
				  if (frmTouchNumpad->ShowModal() == mrOk)
				  {
					 ExpiryMonth = frmTouchNumpad->INTResult;
				  }
				  else
				  {
					 Abort = true; // abort
				  }
			   }
			   while (Abort == false && ExpiryMonth == -1 && ExpiryMonth < 100);

			   if (Abort == false)
			   {
				  do
				  {
					 std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (Screen->ActiveForm));
					 frmTouchNumpad->Caption = "Enter Card Expiry Year e.g 04";
					 frmTouchNumpad->btnSurcharge->Caption = "Ok";
					 frmTouchNumpad->btnSurcharge->Visible = true;
					 frmTouchNumpad->btnDiscount->Visible = false;
					 frmTouchNumpad->Mode = pmNumber;
					 frmTouchNumpad->INTInitial = 0;
					 if (frmTouchNumpad->ShowModal() == mrOk)
					 {
						ExpiryYear = frmTouchNumpad->INTResult;
					 }
					 else
					 {
						Abort = true; // abort
					 }
				  }
				  while (Abort == false && ExpiryYear == -1 && ExpiryYear < 100);
			   }
			}
			else
			{
			   Abort = true; // abort
			}
		 }

		 if (!(Payment->Properties & ePayTypeAllowReversal) && Payment->CreditTransaction)
		 {
			MessageBox("The Payment type selected is not configured to allow Reversals.", "EFTPOS Configuration Error",
			   MB_OK + MB_ICONERROR);
			Abort = true;
		 }

		 eEFTTransactionType TransType;
		 Currency Pay = 0;
		 Currency CashOut = 0;
		 if (!Abort)
		 {
			if (PaymentTransaction.Type == eTransEFTPOSRecovery)
			{
			   TransType = TransactionType_INQUIRY;
			   Pay = Payment->GetPayTendered();
			   CashOut = Payment->GetCashOutTotal();
			}
			else if (Payment->Properties & ePayTypeAllowReversal && Payment->CreditTransaction)
			{
			   TransType = TransactionType_REFUND;
			   Pay = -Payment->GetPayTendered();
			   CashOut = 0;
			}
			else
			{
			   if (Payment->GetPayTendered() > 0 && Payment->GetCashOutTotal() > 0)
			   {
				  TransType = TransactionType_PURCHASE_PLUS_CASH;
			   }
			   else if (Payment->GetPayTendered() == 0 && Payment->GetCashOutTotal() > 0)
			   {
				  TransType = TransactionType_CASH_ADVANCE;
			   }
			   else if (Payment->GetPayTendered() > 0 && Payment->GetCashOutTotal() == 0)
			   {
				  TransType = TransactionType_PURCHASE;
			   }
			   else if (Payment->GetPayTendered() == 0 && Payment->GetCashOutTotal() == 0)
			   {
				  TransType = TransactionType_NONE;
			   }
			   Pay = Payment->GetPayTendered();
			   CashOut = Payment->GetCashOutTotal();
			}

			try
			{
			   EftPos->SetTransactionEvent(Payment->ReferenceNumber, TransType);

			   SaveRecoveryInfo(PaymentTransaction);
			   EftPos->ProcessEftPos(TransType, Pay, CashOut, Payment->ReferenceNumber, PanSource, CardString, ExpiryMonth, ExpiryYear);

			   if (EftPos->WaitOnEftPosEvent(Payment->ReferenceNumber))
			   {
				  TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(Payment->ReferenceNumber);
				  if (EftTrans != NULL)
				  {
					 if (EftTrans->Result == eManualQuery)
					 {

                        std::auto_ptr<TfrmEftPos>frmEftPos(TfrmEftPos::Create<TfrmEftPos>(Screen->ActiveForm));
						if (frmEftPos->TransactionOk(Payment, EftTrans->ResultText) == mrNo)
						{
						   EftPos->CancelTransaction(Payment->ReferenceNumber);
						   Payment->Reset();
						   Payment->Result = eDeclined;
						   EftTrans->ResultText =
							   "Manually Declined \r\rRemember to ensure that the transaction is cancelled on the Eftpos Unit!";
						}
						else
						{
						   Payment->Result = eAccepted;
						}
					 }
					 else
					 {
						Payment->Result = EftTrans->Result;
					 }

					 if (Payment->Result != eAccepted)
					 {
						MessageBox(EftTrans->ResultText, "EFTPOS Response", MB_OK + MB_ICONINFORMATION);
					 }
				  }
				  else
				  {
					 Payment->Result = eFailed;
				  }
			   }
			   else
			   {
				  Payment->Result = eFailed;
			   }
			}
			__finally
			{
			   EftPos->DelTransactionEvent(Payment->ReferenceNumber);
			   if (Payment->Result != eAccepted)
			   {
				  Payment->Reset();
			   }
			}
		 }
	  }
   }
   else
   {
	  Payment->Result = eAccepted;
   }
}

bool TListPaymentSystem::TransRetrivePhoenixResult(TPaymentTransaction &PaymentTransaction)
{
   bool RetVal = true;

   if (!PhoenixHM->Enabled)
   {
	  Application->MessageBox(UnicodeString("Payment System is Configured to use the PMS Interface."
			" The PMS Interface Software has not been installed or enabled correctly." " Please contact your MenuMate support agent.").w_str
		 (), UnicodeString("PMS Interface Configuration Error").w_str(), MB_OK + MB_ICONERROR);
	  RetVal = false;
   }
   else
   {
	  if (!PhoenixHM->ExportData(PaymentTransaction, TDeviceRealTerminal::Instance().User.ContactKey))
	  {
		 RetVal = false;
		 for (int i = 0; i < PaymentTransaction.PaymentsCount(); i++)
		 {
			TPayment *Payment = PaymentTransaction.PaymentGet(i);
			if (Payment->Result != eAccepted)
			{
			   Payment->Reset();
			}
		 }
	  }
	  else
	  {
		 AnsiString TransactionRef = PhoenixHM->GetLastTransactionRef();
		 PaymentTransaction.References.push_back(RefRefType(TransactionRef,
			   ManagerReference->GetReferenceByType(PaymentTransaction.DBTransaction, REFTYPE_PMS)));
	  }
   }
   return RetVal;
}

bool TListPaymentSystem::BuildXMLTransaction(TPaymentTransaction &PaymentTransaction)
{
   TDeviceRealTerminal::Instance().IMManager->Export(PaymentTransaction);
}

void TListPaymentSystem::GetChequeVerifyResult(TPayment *Payment)
{
   if (Payment->Properties & ePayTypeChequeVerify)
   {
	  if (!EftPos->Enabled)
	  {
		 Application->MessageBox(UnicodeString("Payment type selected is Configured to use Intergrated EFTPOS Cheque Verify System."
			   " The Intergrated EFTPOS Software has not been installed or enabled correctly."
			   " Please contact your MenuMate support agent.").w_str(), UnicodeString("EFTPOS Configuration Error").w_str(),
			MB_OK + MB_ICONERROR);
		 Payment->Result = eDeclined;
	  }
	  else
	  {
		 bool Abort = false;
		 ePANSource PanSource = PANSource_Manual; // "K";
		 AnsiString ChequeSerial = "";
		 AnsiString ChequeBranch = "";
		 AnsiString ChequeAccount = "";

		 frmChequeVerfiy->ChequeAmount = Payment->GetPay();
		 if (frmChequeVerfiy->ShowModal() != mrOk)
		 {
			Abort = true;
		 }
		 else
		 {
			ChequeSerial = frmChequeVerfiy->edChqSerial->Text;
			ChequeBranch = frmChequeVerfiy->edChqBranch->Text;
			ChequeAccount = frmChequeVerfiy->edChqAccount->Text;
		 }

		 if (!Abort)
		 {
			try
			{
			   if (Payment->ReferenceNumber == "")
			   {
				  Payment->ReferenceNumber = EftPos->GetRefNumber();
			   }

			   EftPos->SetTransactionEvent(Payment->ReferenceNumber, TransactionType_NONE);

			   EftPos->ChequeVerify(Payment->GetPay(), ChequeSerial, ChequeBranch, ChequeAccount, PanSource, Payment->ReferenceNumber);

			   if (EftPos->WaitOnEftPosEvent(Payment->ReferenceNumber))
			   {
				  TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(Payment->ReferenceNumber);
				  if (EftTrans != NULL)
				  {
					 if (EftTrans->Result == eManualQuery)
					 {
                        std::auto_ptr<TfrmEftPos>frmEftPos(TfrmEftPos::Create<TfrmEftPos>(Screen->ActiveForm));
						if (frmEftPos->TransactionOk(Payment, EftTrans->ResultText) == mrNo)
						{
						   Payment->Reset();
						   Payment->Result = eDeclined;
						   EftTrans->ResultText =
							   "Manually Declined \r\rRemember to ensure that the transaction is cancelled on the Eftpos Unit!";
						}
						else
						{
						   Payment->Result = eAccepted;
						}
					 }
					 else
					 {
						Payment->Result = EftTrans->Result;
					 }

					 if (Payment->Result != eAccepted)
					 {
						if (MessageBox("Cheque was Declined.\r Result : " + EftTrans->ResultText +
							  "\rDo you wish to override this and accept the cheque anyway?", "Cheque Declined Confirmation",
							  MB_ACCEPTDECLINE + MB_ICONQUESTION) == IDOK)
						{
						   Payment->Result = eAccepted;
						}
						else
						{
						   Payment->Reset();
						   Payment->Result = eDeclined;
						}
					 }
					 else
					 {
						if (MessageBox("Cheque was Accepted.\r Result : " + EftTrans->ResultText +
							  "\rDo you wish to Accept the Cheque and Continue.", "Cheque Accepted Confirmation",
							  MB_ACCEPTDECLINE + MB_ICONQUESTION) == IDOK)
						{
						   Payment->Result = eAccepted;
						}
						else
						{
						   Payment->Reset();
						   Payment->Result = eDeclined;
						}
					 }
				  }
				  else
				  {
					 Payment->Reset();
					 Payment->Result = eFailed;
				  }
			   }
			   else
			   {
				  if (MessageBox("Cheque verifaction was canceled. Do you wish to accept this cheque anyway?",
						"Cheque Failed Confirmation", MB_YESNO + MB_ICONQUESTION) == IDYES)
				  {
					 Payment->Result = eAccepted;
				  }
				  else
				  {
					 Payment->Reset();
					 Payment->Result = eDeclined;
				  }
			   }
			}
			__finally
			{
			   EftPos->DelTransactionEvent(Payment->ReferenceNumber);
			}
		 }
	  }
   }
}

void TListPaymentSystem::ArchiveTransaction(TPaymentTransaction &PaymentTransaction)
{
   long ArcBillKey = ArchiveBill(PaymentTransaction);
   ArchivePatronCount(PaymentTransaction, ArcBillKey);
   ArchiveReferences(PaymentTransaction, ArcBillKey);
   ArchivePoints(PaymentTransaction);
   SetLastVisit(PaymentTransaction);
   ArchiveOrder(PaymentTransaction, ArcBillKey);
   ArchiveRewards(PaymentTransaction, ArcBillKey);
}

void TListPaymentSystem::SetLastVisit(TPaymentTransaction &PaymentTransaction)
{
   if (PaymentTransaction.Membership.Member.Valid())
   {
	  TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->SetLastVisit(PaymentTransaction.DBTransaction,
		 PaymentTransaction.Membership.Member);
   }
}

void TListPaymentSystem::ArchivePoints(TPaymentTransaction &PaymentTransaction)
{

   for (TPointsTransactionContainer::iterator ptrPointsTransaction = PaymentTransaction.Membership.Points.PointsTransactions.begin();
	  ptrPointsTransaction != PaymentTransaction.Membership.Points.PointsTransactions.end(); ptrPointsTransaction++)
   {
	  TDeviceRealTerminal::Instance().ManagerMembership->ArchivePoints(PaymentTransaction.DBTransaction, *ptrPointsTransaction);
   }
}

void TListPaymentSystem::ArchivePatronCount(TPaymentTransaction &PaymentTransaction, long ArcBillKey)
{
   try
   {
	  std::vector <TPatronType> ::iterator ptrPatronTypes;
	  for (ptrPatronTypes = PaymentTransaction.Patrons.begin(); ptrPatronTypes != PaymentTransaction.Patrons.end(); ptrPatronTypes++)
	  {
		 if (ptrPatronTypes->Count != 0)
		 {
			TIBSQL *IBInternalQuery = PaymentTransaction.DBTransaction.Query(PaymentTransaction.DBTransaction.AddQuery());

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_DAYPATRONCOUNT, 1) FROM RDB$DATABASE";
			IBInternalQuery->ExecQuery();
			int PatronKey = IBInternalQuery->Fields[0]->AsInteger;
			IBInternalQuery->Close();

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
				"INSERT INTO DAYPATRONCOUNT (" "PATRONCOUNT_KEY, " "ARCBILL_KEY, " "PATRON_TYPE, " "PATRON_COUNT) "
				"VALUES (" ":PATRONCOUNT_KEY, " ":ARCBILL_KEY, " ":PATRON_TYPE, " ":PATRON_COUNT) ";
			IBInternalQuery->ParamByName("PATRONCOUNT_KEY")->AsString = PatronKey;
			IBInternalQuery->ParamByName("ARCBILL_KEY")->AsString = ArcBillKey;
			IBInternalQuery->ParamByName("PATRON_TYPE")->AsString = ptrPatronTypes->Name;
			IBInternalQuery->ParamByName("PATRON_COUNT")->AsInteger = ptrPatronTypes->Count;
			IBInternalQuery->ExecQuery();
		 }
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

void TListPaymentSystem::ArchiveReferences(TPaymentTransaction &PaymentTransaction, long ArcBillKey)
{
   try
   {
	  std::vector <RefRefType> ::iterator ptrReferances;
	  for (ptrReferances = PaymentTransaction.References.begin(); ptrReferances != PaymentTransaction.References.end(); ptrReferances++)
	  {
		 TIBSQL *IBInternalQuery = PaymentTransaction.DBTransaction.Query(PaymentTransaction.DBTransaction.AddQuery());

		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_DAYARCREF, 1) FROM RDB$DATABASE";
		 IBInternalQuery->ExecQuery();
		 int RefKey = IBInternalQuery->Fields[0]->AsInteger;
		 IBInternalQuery->Close();

		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text =
			 "INSERT INTO DAYARCREF (" "DAYARCREF_KEY, " "ARCBILL_KEY, " "REFERENCE, " "REFS_KEY ) " "VALUES ("
			 ":DAYARCREF_KEY, " ":ARCBILL_KEY, " ":REFERENCE, " ":REFS_KEY ) ";
		 IBInternalQuery->ParamByName("DAYARCREF_KEY")->AsString = RefKey;
		 IBInternalQuery->ParamByName("ARCBILL_KEY")->AsString = ArcBillKey;
		 IBInternalQuery->ParamByName("REFS_KEY")->AsInteger = ptrReferances->second.Key;
		 UnicodeString Reference = ptrReferances->first; // This could be null so put it in an empty string.
		 IBInternalQuery->ParamByName("REFERENCE")->AsString = Reference.SubString(1, 40);
		 IBInternalQuery->ExecQuery();
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

void TListPaymentSystem::ArchiveRewards(TPaymentTransaction &PaymentTransaction, long ArcBillKey)
{
   try
   {
	  if (PaymentTransaction.RewardsTransaction.CardNumber != "")
	  {
		 TIBSQL *IBInternalQuery = PaymentTransaction.DBTransaction.Query(PaymentTransaction.DBTransaction.AddQuery());

		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_REWARDS, 1) FROM RDB$DATABASE";
		 IBInternalQuery->ExecQuery();
		 int RewardKey = IBInternalQuery->Fields[0]->AsInteger;
		 IBInternalQuery->Close();

		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text =
			 "INSERT INTO REWARDS (" "REWARDS_KEY, " "ARCBILL_KEY, " "CARD, " "REFERENCE, " "AMOUNT ) " "VALUES ("
			 ":REWARDS_KEY, " ":ARCBILL_KEY, " ":CARD, " ":REFERENCE, " ":AMOUNT ) ";
		 IBInternalQuery->ParamByName("REWARDS_KEY")->AsInteger = RewardKey;
		 IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = ArcBillKey;
		 IBInternalQuery->ParamByName("CARD")->AsString = PaymentTransaction.RewardsTransaction.CardNumber;
		 IBInternalQuery->ParamByName("REFERENCE")->AsString = PaymentTransaction.RewardsTransaction.EFTPOSRef;
		 IBInternalQuery->ParamByName("AMOUNT")->AsCurrency = PaymentTransaction.RewardsTransaction.Amount;
		 IBInternalQuery->ExecQuery();
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

long TListPaymentSystem::ArchiveBill(TPaymentTransaction &PaymentTransaction)
{
   long Retval;

   try
   {

	  Currency Total = PaymentTransaction.Money.GrandTotal;

	  TIBSQL *IBInternalQuery = PaymentTransaction.DBTransaction.Query(PaymentTransaction.DBTransaction.AddQuery());
	  TIBSQL *IBInternalQuery2 = PaymentTransaction.DBTransaction.Query(PaymentTransaction.DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_DAYARCBILL, 1) FROM RDB$DATABASE";
	  IBInternalQuery->ExecQuery();
	  Retval = IBInternalQuery->Fields[0]->AsInteger;
	  IBInternalQuery->Close();

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
		  "INSERT INTO DAYARCBILL (" "ARCBILL_KEY, " "TERMINAL_NAME, " "STAFF_NAME, " "TIME_STAMP, " "TOTAL, " "DISCOUNT, "
		  "PATRON_COUNT, " "RECEIPT, " "SECURITY_REF, " "BILLED_LOCATION, " "INVOICE_NUMBER, " "SALES_TYPE, " "INVOICE_KEY) " "VALUES ("
		  ":ARCBILL_KEY, " ":TERMINAL_NAME, " ":STAFF_NAME, " ":TIME_STAMP, " ":TOTAL, " ":DISCOUNT, " ":PATRON_COUNT, " ":RECEIPT, "
		  ":SECURITY_REF, " ":BILLED_LOCATION," ":INVOICE_NUMBER, " ":SALES_TYPE, " ":INVOICE_KEY) ";
	  IBInternalQuery->ParamByName("ARCBILL_KEY")->AsString = Retval;
	  IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
	  IBInternalQuery->ParamByName("STAFF_NAME")->AsString = TDeviceRealTerminal::Instance().User.Name;
	  IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = Now();
	  IBInternalQuery->ParamByName("TOTAL")->AsCurrency = Total;

	  int TotalCount = 0;
	  std::vector <TPatronType> ::iterator ptrPatronTypes;
	  for (ptrPatronTypes = PaymentTransaction.Patrons.begin(); ptrPatronTypes != PaymentTransaction.Patrons.end(); ptrPatronTypes++)
	  {
		 TotalCount += ptrPatronTypes->Count;
	  }
	  IBInternalQuery->ParamByName("PATRON_COUNT")->AsInteger = TotalCount;

	  IBInternalQuery->ParamByName("SECURITY_REF")->AsInteger = Security->GetSecurityRefNumber();
	  IBInternalQuery->ParamByName("INVOICE_NUMBER")->AsString = PaymentTransaction.InvoiceNumber;
	  IBInternalQuery->ParamByName("SALES_TYPE")->AsInteger = PaymentTransaction.SalesType;
	  IBInternalQuery->ParamByName("BILLED_LOCATION")->AsString = TDeviceRealTerminal::Instance().ID.Location;
	  IBInternalQuery->ParamByName("INVOICE_KEY")->AsInteger = PaymentTransaction.InvoiceKey;
	  ManagerReceipt->ReceiptToArchive->Position = 0;
	  IBInternalQuery->ParamByName("RECEIPT")->LoadFromStream(ManagerReceipt->ReceiptToArchive);

	  IBInternalQuery->ExecQuery();

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
		  "INSERT INTO DAYARCBILLPAY (" "DAYARCBILLPAY_KEY, " "ARCBILL_KEY, " "PAY_TYPE, " "VOUCHER_NUMBER, "
		  "SUBTOTAL, " "ROUNDING, " "CASH_OUT, " "TAX_FREE, " "NOTE, " "PROPERTIES, " "GROUP_NUMBER) " "VALUES ("
		  ":DAYARCBILLPAY_KEY, " ":ARCBILL_KEY, " ":PAY_TYPE, " ":VOUCHER_NUMBER, " ":SUBTOTAL, " ":ROUNDING, " ":CASH_OUT, " ":TAX_FREE, "
		  ":NOTE, " ":PROPERTIES, " ":GROUP_NUMBER) ";

	  for (int i = 0; i < PaymentTransaction.PaymentsCount(); i++)
	  {
		 TPayment *SubPayment = PaymentTransaction.PaymentGet(i);
		 if (SubPayment->GetPay() != 0)
		 {
			// Get New Key
			IBInternalQuery2->Close();
			IBInternalQuery2->SQL->Text = "SELECT GEN_ID(GEN_DAYARCBILLPAY, 1) FROM RDB$DATABASE";
			IBInternalQuery2->ExecQuery();
			int PaymentKey = IBInternalQuery2->Fields[0]->AsInteger;

			IBInternalQuery->Close();
			IBInternalQuery->ParamByName("DAYARCBILLPAY_KEY")->AsInteger = PaymentKey;
			IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = Retval;

			if (SubPayment->SysNameOveride != "")
			{
			   IBInternalQuery->ParamByName("PAY_TYPE")->AsString = SubPayment->SysNameOveride;
			}
			else
			{
			   if (SubPayment->NameOveride == "")
			   {
				  IBInternalQuery->ParamByName("PAY_TYPE")->AsString = SubPayment->Name;
			   }
			   else
			   {
				  IBInternalQuery->ParamByName("PAY_TYPE")->AsString = SubPayment->NameOveride;
			   }
			}

			IBInternalQuery->ParamByName("VOUCHER_NUMBER")->AsString = SubPayment->ReferenceNumber;

			if (!PaymentTransaction.CreditTransaction)
			{
			   IBInternalQuery->ParamByName("SUBTOTAL")->AsCurrency = SubPayment->GetPayTendered();
			}
			else
			{
			   IBInternalQuery->ParamByName("SUBTOTAL")->AsCurrency = SubPayment->GetPayTendered();
			}

			IBInternalQuery->ParamByName("ROUNDING")->AsCurrency = SubPayment->GetPayRounding();
			IBInternalQuery->ParamByName("CASH_OUT")->AsString = "F";
			if (SubPayment->Properties & ePayTypeTaxFree)
			{
			   IBInternalQuery->ParamByName("TAX_FREE")->AsString = "T";
			}
			else
			{
			   IBInternalQuery->ParamByName("TAX_FREE")->AsString = "F";
			}

			IBInternalQuery->ParamByName("NOTE")->AsString = SubPayment->Note.SubString(1, 50);
			IBInternalQuery->ParamByName("PROPERTIES")->AsInteger = SubPayment->Properties;
			IBInternalQuery->ParamByName("GROUP_NUMBER")->AsInteger = SubPayment->GroupNumber;
			IBInternalQuery->ExecQuery();
		 }
	  }

	  for (int i = 0; i < PaymentTransaction.PaymentsCount(); i++)
	  {
		 TPayment *SubPayment = PaymentTransaction.PaymentGet(i);
		 if (SubPayment->GetCashOut() != 0)
		 {
			// Get New Key
			IBInternalQuery2->Close();
			IBInternalQuery2->SQL->Text = "SELECT GEN_ID(GEN_DAYARCBILLPAY, 1) FROM RDB$DATABASE";
			IBInternalQuery2->ExecQuery();
			int PaymentKey = IBInternalQuery2->Fields[0]->AsInteger;

			IBInternalQuery->Close();
			IBInternalQuery->ParamByName("DAYARCBILLPAY_KEY")->AsInteger = PaymentKey;
			IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = Retval;
			IBInternalQuery->ParamByName("PAY_TYPE")->AsString = SubPayment->Name;
			IBInternalQuery->ParamByName("VOUCHER_NUMBER")->AsString = SubPayment->ReferenceNumber;
			IBInternalQuery->ParamByName("SUBTOTAL")->AsCurrency = SubPayment->GetCashOut();
			IBInternalQuery->ParamByName("ROUNDING")->AsCurrency = SubPayment->GetCashOutRounding();
			IBInternalQuery->ParamByName("CASH_OUT")->AsString = "T";
			if (SubPayment->Properties & ePayTypeTaxFree)
			{
			   IBInternalQuery->ParamByName("TAX_FREE")->AsString = "T";
			}
			else
			{
			   IBInternalQuery->ParamByName("TAX_FREE")->AsString = "F";
			}

			IBInternalQuery->ParamByName("NOTE")->AsString = SubPayment->Note.SubString(1, 80);
			IBInternalQuery->ParamByName("PROPERTIES")->AsInteger = SubPayment->Properties;
			IBInternalQuery->ParamByName("GROUP_NUMBER")->AsInteger = SubPayment->GroupNumber;
			IBInternalQuery->ExecQuery();
		 }
	  }

	  // Insert a cash adjustment record for Change and Cash Out.
	  TPayment *CashPayment = PaymentTransaction.PaymentFind(CASH);
	  IBInternalQuery2->Close();
	  IBInternalQuery2->SQL->Text = "SELECT GEN_ID(GEN_DAYARCBILLPAY, 1) FROM RDB$DATABASE";
	  IBInternalQuery2->ExecQuery();
	  int PaymentKey = IBInternalQuery2->Fields[0]->AsInteger;

	  IBInternalQuery->Close();
	  IBInternalQuery->ParamByName("DAYARCBILLPAY_KEY")->AsInteger = PaymentKey;
	  IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = Retval;
	  IBInternalQuery->ParamByName("PAY_TYPE")->AsString = CASH;
	  IBInternalQuery->ParamByName("VOUCHER_NUMBER")->AsString = "";
	  IBInternalQuery->ParamByName("ROUNDING")->AsCurrency = 0;
	  IBInternalQuery->ParamByName("CASH_OUT")->AsString = "F";
	  IBInternalQuery->ParamByName("TAX_FREE")->AsString = "F";
	  IBInternalQuery->ParamByName("NOTE")->AsString = "Total Change.";
	  if (CashPayment)
	  {
		 IBInternalQuery->ParamByName("SUBTOTAL")->AsCurrency = -CashPayment->GetChange();
		 IBInternalQuery->ParamByName("PROPERTIES")->AsInteger = CashPayment->Properties;
		 IBInternalQuery->ParamByName("GROUP_NUMBER")->AsInteger = CashPayment->GroupNumber;
	  }
	  else
	  {
		 IBInternalQuery->ParamByName("SUBTOTAL")->AsCurrency = -PaymentTransaction.Money.Change;
		 IBInternalQuery->ParamByName("PROPERTIES")->AsInteger = 0;
		 IBInternalQuery->ParamByName("GROUP_NUMBER")->AsInteger = 0;
	  }
	  IBInternalQuery->ExecQuery();

	  for (int i = 0; i < PaymentTransaction.PaymentsCount(); i++)
	  {
		 TPayment *SubPayment = PaymentTransaction.PaymentGet(i);
		 if (SubPayment->GetAdjustment() != 0)
		 {
			// Get New Key
			IBInternalQuery2->Close();
			IBInternalQuery2->SQL->Text = "SELECT GEN_ID(GEN_DAYARCSURCHARGE, 1) FROM RDB$DATABASE";
			IBInternalQuery2->ExecQuery();
			int SurchargeKey = IBInternalQuery2->Fields[0]->AsInteger;

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
				"INSERT INTO DAYARCSURCHARGE (" "SURCHARGE_KEY, " "ARCBILL_KEY, " "PAY_TYPE, " "PAY_TYPE_DETAILS, "
				"SUBTOTAL, " "ROUNDING, " "TAX_FREE, " "NOTE, " "PROPERTIES," "GROUP_NUMBER) " "VALUES (" ":SURCHARGE_KEY, "
				":ARCBILL_KEY, " ":PAY_TYPE, " ":PAY_TYPE_DETAILS, " ":SUBTOTAL, " ":ROUNDING, " ":TAX_FREE, " ":NOTE, " ":PROPERTIES,"
				":GROUP_NUMBER) ";

			IBInternalQuery->ParamByName("SURCHARGE_KEY")->AsInteger = SurchargeKey;
			IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = Retval;
			IBInternalQuery->ParamByName("PAY_TYPE_DETAILS")->AsString = SubPayment->ReferenceNumber;
			IBInternalQuery->ParamByName("SUBTOTAL")->AsCurrency = SubPayment->GetAdjustment();
			IBInternalQuery->ParamByName("ROUNDING")->AsCurrency = SubPayment->GetAdjustmentRounding();
			if (SubPayment->Properties & ePayTypeTaxFree)
			{
			   IBInternalQuery->ParamByName("TAX_FREE")->AsString = "T";
			}
			else
			{
			   IBInternalQuery->ParamByName("TAX_FREE")->AsString = "F";
			}

			if (SubPayment->SysNameOveride != "")
			{
			   IBInternalQuery->ParamByName("PAY_TYPE")->AsString = SubPayment->SysNameOveride;
			}
			else
			{
			   if (SubPayment->NameOveride == "")
			   {
				  IBInternalQuery->ParamByName("PAY_TYPE")->AsString = SubPayment->Name;
			   }
			   else
			   {
				  IBInternalQuery->ParamByName("PAY_TYPE")->AsString = SubPayment->NameOveride;
			   }
			}

			IBInternalQuery->ParamByName("NOTE")->AsString = SubPayment->Note.SubString(1, 50);
			IBInternalQuery->ParamByName("PROPERTIES")->AsInteger = SubPayment->Properties;
			IBInternalQuery->ParamByName("GROUP_NUMBER")->AsInteger = SubPayment->GroupNumber;
			IBInternalQuery->ExecQuery();
		 }
	  }

   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return Retval;
}

void TListPaymentSystem::ArchiveOrder(TPaymentTransaction &PaymentTransaction, long ArcBillLK)
{
   if (PaymentTransaction.SalesType != eCreditPurchase)
   {
	  for (int CurrentIndex = 0; CurrentIndex < PaymentTransaction.Orders->Count; CurrentIndex++)
	  {
		 TItemComplete *Order = (TItemComplete*)(PaymentTransaction.Orders->Items[CurrentIndex]);

		 // Patch to fix missing Serving Course.
		 if (Order->ServingCourse.ServingCourseKey < 1)
		 {
			Order->ServingCourse = TDeviceRealTerminal::Instance().Menus->DefaultServingCourse;
			TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, "Order has no Serving Course");
		 }

		 try
		 {
			int PrimaryArcCatkey = TDBOrder::GetArchiveCategory(PaymentTransaction.DBTransaction, Order->Categories->FinancialCategory);

			TIBSQL *IBInternalQuery = PaymentTransaction.DBTransaction.Query(PaymentTransaction.DBTransaction.AddQuery());

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_DAYARCHIVE, 1) FROM RDB$DATABASE";
			IBInternalQuery->ExecQuery();
			int MasterArchiveKey = IBInternalQuery->Fields[0]->AsInteger;

			IBInternalQuery->Close();
			IBInternalQuery->ParamCheck = true;
			IBInternalQuery->SQL->Clear();
			IBInternalQuery->SQL->Text =
				"INSERT INTO DAYARCHIVE (" "ARCHIVE_KEY," "ARCBILL_KEY," "TERMINAL_NAME," "MENU_NAME," "COURSE_NAME,"
				"ITEM_NAME," "ITEM_CATEGORY," "ITEM_SHORT_NAME," "ITEM_ID," "SIZE_NAME," "TABLE_NUMBER," "TABLE_NAME," "SEAT_NUMBER,"
				"SERVER_NAME," "TAB_NAME," "LOYALTY_NAME," "ORDER_TYPE," "TIME_STAMP," "TIME_STAMP_BILLED," "ORDER_LOCATION," "PRICE,"
				"COST," "HAPPY_HOUR," "NOTE," "SECURITY_REF," "TIME_KEY," "GST_PERCENT," "COST_GST_PERCENT," "QTY,"
				"DISCOUNT," "REDEEMED," "POINTS_PERCENT," "POINTS_EARNED," "LOYALTY_KEY," "THIRDPARTYCODES_KEY," "CATEGORY_KEY,"
				"DISCOUNT_REASON," "PRICE_LEVEL0," "PRICE_LEVEL1," "SERVINGCOURSES_KEY)" " VALUES (" ":ARCHIVE_KEY," ":ARCBILL_KEY,"
				":TERMINAL_NAME," ":MENU_NAME," ":COURSE_NAME," ":ITEM_NAME," ":ITEM_CATEGORY," ":ITEM_SHORT_NAME," ":ITEM_ID,"
				":SIZE_NAME," ":TABLE_NUMBER," ":TABLE_NAME," ":SEAT_NUMBER," ":SERVER_NAME," ":TAB_NAME," ":LOYALTY_NAME," ":ORDER_TYPE,"
				":TIME_STAMP," ":TIME_STAMP_BILLED," ":ORDER_LOCATION," ":PRICE," ":COST," ":HAPPY_HOUR," ":NOTE,"
				":SECURITY_REF," ":TIME_KEY," ":GST_PERCENT," ":COST_GST_PERCENT," ":QTY," ":DISCOUNT," ":REDEEMED," ":POINTS_PERCENT,"
				":POINTS_EARNED," ":LOYALTY_KEY," ":THIRDPARTYCODES_KEY," ":CATEGORY_KEY," ":DISCOUNT_REASON," ":PRICE_LEVEL0,"
				":PRICE_LEVEL1," ":SERVINGCOURSES_KEY);";

			IBInternalQuery->ParamByName("ARCHIVE_KEY")->AsInteger = MasterArchiveKey;
			IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = ArcBillLK;
			IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
			IBInternalQuery->ParamByName("MENU_NAME")->AsString = Order->MenuName;
			IBInternalQuery->ParamByName("TAB_NAME")->AsString = Order->TabName;
			switch(Order->TabType)
			{
			case TabNormal:
			case TabStaff:
			case TabMember:
			case TabCashAccount:
			case TabNone:
			case TabTableSeat:
         case TabWeb:
			   IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = Order->TableNo;
			   IBInternalQuery->ParamByName("TABLE_NAME")->AsString = Order->TabContainerName;
			   break;
			case TabRoom:
			   IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = Order->RoomNo;
			   IBInternalQuery->ParamByName("TABLE_NAME")->AsString = Order->TabContainerName;
			   break;
			}
			IBInternalQuery->ParamByName("SEAT_NUMBER")->AsInteger = Order->SeatNo;
			IBInternalQuery->ParamByName("COURSE_NAME")->AsString = Order->Course;
			IBInternalQuery->ParamByName("SIZE_NAME")->AsString = Order->Size;
			IBInternalQuery->ParamByName("ITEM_NAME")->AsString = Order->Item;
			IBInternalQuery->ParamByName("ITEM_ID")->AsString = Order->Item_ID;
			IBInternalQuery->ParamByName("ORDER_TYPE")->AsInteger = Order->OrderType;
			IBInternalQuery->ParamByName("QTY")->AsFloat = double(Order->GetQty());
			IBInternalQuery->ParamByName("PRICE")->AsCurrency = Order->Price();
			IBInternalQuery->ParamByName("GST_PERCENT")->AsFloat = double(Order->GSTPercent);
			IBInternalQuery->ParamByName("COST")->AsCurrency = Order->Cost;
			IBInternalQuery->ParamByName("COST_GST_PERCENT")->AsFloat = double(Order->CostGSTPercent);
			IBInternalQuery->ParamByName("DISCOUNT")->AsCurrency = Order->TotalDiscount();
			IBInternalQuery->ParamByName("DISCOUNT_REASON")->AsString = Order->DiscountReason.SubString(1, 40);
			IBInternalQuery->ParamByName("REDEEMED")->AsCurrency = Order->Redeemed;
			IBInternalQuery->ParamByName("POINTS_PERCENT")->AsFloat = Order->PointsPercent;
			IBInternalQuery->ParamByName("ORDER_LOCATION")->AsString = Order->OrderedLocation;
			IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = Order->TimeStamp;
			IBInternalQuery->ParamByName("TIME_STAMP_BILLED")->AsDateTime = Now();
			IBInternalQuery->ParamByName("HAPPY_HOUR")->AsString = Order->HappyHour ? "T" : "F";
			IBInternalQuery->ParamByName("NOTE")->AsString = Order->Note.SubString(1, 80);
			IBInternalQuery->ParamByName("TIME_KEY")->AsInteger = Order->TimeKey;
			IBInternalQuery->ParamByName("SECURITY_REF")->AsInteger = Order->Security->GetSecurityRefNumber();
			IBInternalQuery->ParamByName("POINTS_PERCENT")->AsFloat = Order->PointsPercent;
			IBInternalQuery->ParamByName("POINTS_EARNED")->AsFloat = Order->PointsEarned;
			IBInternalQuery->ParamByName("LOYALTY_KEY")->AsInteger = PaymentTransaction.Membership.Member.ContactKey;
			IBInternalQuery->ParamByName("CATEGORY_KEY")->AsInteger = PrimaryArcCatkey;
			IBInternalQuery->ParamByName("THIRDPARTYCODES_KEY")->AsInteger = Order->ThirdPartyKey;
			IBInternalQuery->ParamByName("PRICE_LEVEL0")->AsCurrency = Order->PriceLevel0;
			IBInternalQuery->ParamByName("PRICE_LEVEL1")->AsCurrency = Order->PriceLevel1;

			if (Order->ServingCourse.ServingCourseKey == 0)
			{
			   IBInternalQuery->ParamByName("SERVINGCOURSES_KEY")->Clear();
			}
			else
			{
			   IBInternalQuery->ParamByName("SERVINGCOURSES_KEY")->AsInteger = Order->ServingCourse.ServingCourseKey;
			}

			IBInternalQuery->ExecQuery();

			ArchiveOrderDiscounts(PaymentTransaction.DBTransaction, MasterArchiveKey, Order);

			for (int q = 0; q < Order->Categories->Count; q++)
			{
			   TItemSizeCategory *CurrentCat = Order->Categories->CategoryGet(q);

			   CurrentCat->CategoryKey = TDBOrder::GetArchiveCategory(PaymentTransaction.DBTransaction, CurrentCat->Category);

			   IBInternalQuery->Close();
			   IBInternalQuery->SQL->Text =
				   "INSERT INTO DAYARCCATEGORY (" "ARCHIVE_KEY," "CATEGORY_KEY) " "VALUES (" ":ARCHIVE_KEY,"
				   ":CATEGORY_KEY);";
			   IBInternalQuery->ParamByName("ARCHIVE_KEY")->AsInteger = MasterArchiveKey;
			   IBInternalQuery->ParamByName("CATEGORY_KEY")->AsInteger = CurrentCat->CategoryKey;
			   IBInternalQuery->ExecQuery();
			}

			for (int i = 0; i < Order->SubOrders->Count; i++)
			{
			   TItemCompleteSub *CurrentSubOrder = (TItemCompleteSub*)Order->SubOrders->Items[i];

			   int SubPrimaryArcCatkey = TDBOrder::GetArchiveCategory(PaymentTransaction.DBTransaction,
				  CurrentSubOrder->Categories->FinancialCategory);

			   IBInternalQuery->Close();
			   IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_DAYARCHIVE, 1) FROM RDB$DATABASE";
			   IBInternalQuery->ExecQuery();
			   int SubArchiveKey = IBInternalQuery->Fields[0]->AsInteger;

			   IBInternalQuery->Close();
			   IBInternalQuery->ParamCheck = true;
			   IBInternalQuery->SQL->Clear();
			   IBInternalQuery->SQL->Text =
				   "INSERT INTO DAYARCHIVE (" "ARCHIVE_KEY," "ARCBILL_KEY," "TERMINAL_NAME," "MENU_NAME," "COURSE_NAME,"
				   "ITEM_NAME," "ITEM_CATEGORY," "ITEM_SHORT_NAME," "ITEM_ID," "SIZE_NAME," "TABLE_NUMBER," "TABLE_NAME," "SEAT_NUMBER,"
				   "SERVER_NAME," "TAB_NAME," "LOYALTY_NAME," "ORDER_TYPE," "TIME_STAMP," "TIME_STAMP_BILLED," "ORDER_LOCATION," "PRICE,"
				   "COST," "HAPPY_HOUR," "NOTE," "SECURITY_REF," "TIME_KEY," "GST_PERCENT," "COST_GST_PERCENT," "QTY,"
				   "DISCOUNT," "REDEEMED," "POINTS_PERCENT," "POINTS_EARNED," "LOYALTY_KEY," "THIRDPARTYCODES_KEY," "CATEGORY_KEY,"
				   "DISCOUNT_REASON," "PRICE_LEVEL0," "PRICE_LEVEL1," "SERVINGCOURSES_KEY)" " VALUES (" ":ARCHIVE_KEY," ":ARCBILL_KEY,"
				   ":TERMINAL_NAME," ":MENU_NAME," ":COURSE_NAME," ":ITEM_NAME," ":ITEM_CATEGORY," ":ITEM_SHORT_NAME," ":ITEM_ID,"
				   ":SIZE_NAME," ":TABLE_NUMBER," ":TABLE_NAME," ":SEAT_NUMBER," ":SERVER_NAME," ":TAB_NAME," ":LOYALTY_NAME,"
				   ":ORDER_TYPE," ":TIME_STAMP," ":TIME_STAMP_BILLED," ":ORDER_LOCATION," ":PRICE," ":COST,"
				   ":HAPPY_HOUR," ":NOTE," ":SECURITY_REF," ":TIME_KEY," ":GST_PERCENT," ":COST_GST_PERCENT," ":QTY,"
				   ":DISCOUNT," ":REDEEMED," ":POINTS_PERCENT," ":POINTS_EARNED," ":LOYALTY_KEY," ":THIRDPARTYCODES_KEY," ":CATEGORY_KEY,"
				   ":DISCOUNT_REASON," ":PRICE_LEVEL0," ":PRICE_LEVEL1," ":SERVINGCOURSES_KEY);";

			   IBInternalQuery->ParamByName("ARCHIVE_KEY")->AsInteger = SubArchiveKey;
			   IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = ArcBillLK;

			   IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
			   // TODO 2 -o Michael -c Descrespancy: This Menuwill be inaccurate
			   IBInternalQuery->ParamByName("MENU_NAME")->AsString = Order->MenuName;
			   IBInternalQuery->ParamByName("TAB_NAME")->AsString = Order->TabName;
			   switch(Order->TabType)
			   {
			   case TabNormal:
			   case TabStaff:
			   case TabMember:
			   case TabCashAccount:
			   case TabNone:
			   case TabTableSeat:
            case TabWeb:
				  IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = Order->TableNo;
				  IBInternalQuery->ParamByName("TABLE_NAME")->AsString = Order->TabContainerName;
				  break;
			   case TabRoom:
				  IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = Order->RoomNo;
				  IBInternalQuery->ParamByName("TABLE_NAME")->AsString = Order->TabContainerName;
				  break;
			   }
			   IBInternalQuery->ParamByName("SEAT_NUMBER")->AsInteger = Order->SeatNo;
			   IBInternalQuery->ParamByName("COURSE_NAME")->AsString = CurrentSubOrder->Course;
			   IBInternalQuery->ParamByName("SIZE_NAME")->AsString = CurrentSubOrder->Size;
			   IBInternalQuery->ParamByName("ITEM_NAME")->AsString = CurrentSubOrder->Item;
			   IBInternalQuery->ParamByName("ITEM_ID")->AsInteger = CurrentSubOrder->Item_ID;
			   IBInternalQuery->ParamByName("ORDER_TYPE")->AsInteger = Order->OrderType;
			   IBInternalQuery->ParamByName("QTY")->AsFloat = double(CurrentSubOrder->GetQty());
			   IBInternalQuery->ParamByName("PRICE")->AsCurrency = CurrentSubOrder->Price();
			   IBInternalQuery->ParamByName("GST_PERCENT")->AsFloat = double(CurrentSubOrder->GSTPercent);
			   IBInternalQuery->ParamByName("ORDER_LOCATION")->AsString = Order->OrderedLocation;
			   IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = Order->TimeStamp;
			   IBInternalQuery->ParamByName("TIME_STAMP_BILLED")->AsDateTime = Now();
			   IBInternalQuery->ParamByName("COST")->AsCurrency = CurrentSubOrder->Cost;
			   IBInternalQuery->ParamByName("DISCOUNT")->AsCurrency = CurrentSubOrder->TotalDiscount();
			   IBInternalQuery->ParamByName("DISCOUNT_REASON")->AsString = CurrentSubOrder->DiscountReason.SubString(1, 40);
			   IBInternalQuery->ParamByName("REDEEMED")->AsCurrency = CurrentSubOrder->Redeemed;
			   IBInternalQuery->ParamByName("COST_GST_PERCENT")->AsFloat = double(CurrentSubOrder->CostGSTPercent);
			   IBInternalQuery->ParamByName("HAPPY_HOUR")->AsString = Order->HappyHour ? "T" : "F";
			   IBInternalQuery->ParamByName("NOTE")->AsString = Order->Note.SubString(1, 80);
			   IBInternalQuery->ParamByName("TIME_KEY")->AsInteger = Order->TimeKey;
			   IBInternalQuery->ParamByName("SECURITY_REF")->AsInteger = CurrentSubOrder->Security->GetSecurityRefNumber();
			   IBInternalQuery->ParamByName("POINTS_PERCENT")->AsFloat = CurrentSubOrder->PointsPercent;
			   IBInternalQuery->ParamByName("POINTS_EARNED")->AsFloat = CurrentSubOrder->PointsEarned;
			   IBInternalQuery->ParamByName("LOYALTY_KEY")->AsInteger = PaymentTransaction.Membership.Member.ContactKey;
			   IBInternalQuery->ParamByName("CATEGORY_KEY")->AsInteger = SubPrimaryArcCatkey;
			   IBInternalQuery->ParamByName("PRICE_LEVEL0")->AsCurrency = CurrentSubOrder->PriceLevel0;
			   IBInternalQuery->ParamByName("PRICE_LEVEL1")->AsCurrency = CurrentSubOrder->PriceLevel1;
			   IBInternalQuery->ParamByName("THIRDPARTYCODES_KEY")->AsInteger = CurrentSubOrder->ThirdPartyKey;

			   if (CurrentSubOrder->ServingCourse.ServingCourseKey == 0)
			   {
				  IBInternalQuery->ParamByName("SERVINGCOURSES_KEY")->Clear();
			   }
			   else
			   {
				  IBInternalQuery->ParamByName("SERVINGCOURSES_KEY")->AsInteger = CurrentSubOrder->ServingCourse.ServingCourseKey;
			   }

			   IBInternalQuery->ExecQuery();

			   ArchiveOrderDiscounts(PaymentTransaction.DBTransaction, SubArchiveKey, CurrentSubOrder);

			   for (int q = 0; q < ((TItemCompleteSub*)Order->SubOrders->Items[i])->Categories->Count; q++)
			   {
				  TItemSizeCategory *CurrentCat = ((TItemCompleteSub*)Order->SubOrders->Items[i])->Categories->CategoryGet(q);

				  CurrentCat->CategoryKey = TDBOrder::GetArchiveCategory(PaymentTransaction.DBTransaction, CurrentCat->Category);

				  IBInternalQuery->Close();
				  IBInternalQuery->SQL->Text =
					  "INSERT INTO DAYARCCATEGORY (" "ARCHIVE_KEY," "CATEGORY_KEY) " "VALUES ("
					  ":ARCHIVE_KEY," ":CATEGORY_KEY);";
				  IBInternalQuery->ParamByName("ARCHIVE_KEY")->AsInteger = SubArchiveKey;
				  IBInternalQuery->ParamByName("CATEGORY_KEY")->AsInteger = CurrentCat->CategoryKey;
				  IBInternalQuery->ExecQuery();
			   }
			}
		 }
		 catch(Exception & E)
		 {
			TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message + " " + Order->Item);
			throw;
		 }
	  }
   }

}

void TListPaymentSystem::ArchiveOrderDiscounts(Database::TDBTransaction &DBTransaction, int ArchiveKey, TItemMinorComplete *Order)
{
   Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
   DBTransaction.RegisterQuery(IBInternalQuery);

   for (std::vector <TDiscount> ::const_iterator ptrDiscounts = Order->DiscountsBegin(); ptrDiscounts != Order->DiscountsEnd();
	  std::advance(ptrDiscounts, 1))
   {
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_DAYARCORDERDISCOUNTS, 1) FROM RDB$DATABASE";
	  IBInternalQuery->ExecQuery();
	  int OrderDiscountKey = IBInternalQuery->Fields[0]->AsInteger;

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
		  "INSERT INTO DAYARCORDERDISCOUNTS (" "ARCORDERDISCOUNTS_KEY," "ARCHIVE_KEY," "DISCOUNT_KEY," "NAME," "DESCRIPTION,"
		  "PERCENTAGE," "AMOUNT," "DISCOUNTED_VALUE," "DISCOUNT_TYPE," "DISCOUNT_MODE," "SOURCE," "ROUNDING," "MENU," "APPEARANCE_ORDER,"
		  "PRIORITY," "DISCOUNT_ID) " "VALUES (" ":ARCORDERDISCOUNTS_KEY," ":ARCHIVE_KEY," ":DISCOUNT_KEY," ":NAME," ":DESCRIPTION,"
		  ":PERCENTAGE," ":AMOUNT," ":DISCOUNTED_VALUE," ":DISCOUNT_TYPE," ":DISCOUNT_MODE," ":SOURCE," ":ROUNDING," ":MENU,"
		  ":APPEARANCE_ORDER," ":PRIORITY," ":DISCOUNT_ID);";
	  IBInternalQuery->ParamByName("ARCORDERDISCOUNTS_KEY")->AsInteger = OrderDiscountKey;
	  IBInternalQuery->ParamByName("ARCHIVE_KEY")->AsInteger = ArchiveKey;
	  IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = ptrDiscounts->DiscountKey;
	  IBInternalQuery->ParamByName("NAME")->AsString = ptrDiscounts->Name.SubString(1, 15);
	  if (ptrDiscounts->Description == "")
	  {
		 IBInternalQuery->ParamByName("DESCRIPTION")->AsString = ptrDiscounts->Name.SubString(1, 128);
	  }
	  else
	  {
		 IBInternalQuery->ParamByName("DESCRIPTION")->AsString = ptrDiscounts->Description.SubString(1, 128);
	  }
	  IBInternalQuery->ParamByName("PERCENTAGE")->AsFloat = ptrDiscounts->PercentAmount;
	  IBInternalQuery->ParamByName("AMOUNT")->AsCurrency = ptrDiscounts->Amount;
	  IBInternalQuery->ParamByName("DISCOUNTED_VALUE")->AsCurrency = Order->DiscountValue(ptrDiscounts);
	  IBInternalQuery->ParamByName("ROUNDING")->AsCurrency = ptrDiscounts->Rounding;
	  IBInternalQuery->ParamByName("DISCOUNT_TYPE")->AsInteger = ptrDiscounts->Type;
	  IBInternalQuery->ParamByName("DISCOUNT_MODE")->AsInteger = ptrDiscounts->Mode;
	  IBInternalQuery->ParamByName("SOURCE")->AsInteger = ptrDiscounts->Source;
	  IBInternalQuery->ParamByName("MENU")->AsString = ptrDiscounts->Menu;
	  IBInternalQuery->ParamByName("APPEARANCE_ORDER")->AsInteger = ptrDiscounts->AppearanceOrder;
	  IBInternalQuery->ParamByName("PRIORITY")->AsInteger = ptrDiscounts->Priority;
	  IBInternalQuery->ParamByName("DISCOUNT_ID")->AsInteger = ptrDiscounts->ID;
	  IBInternalQuery->ExecQuery();
   }

}
// ------------------------------------------------------------------------------

void TListPaymentSystem::SaveToFileCSV(TPaymentTransaction &PaymentTransaction)
{
   try
   {
	  for (int i = 0; i < PaymentTransaction.PaymentsCount(); i++)
	  {
		 TPayment *Payment = PaymentTransaction.PaymentGet(i);
		 if ((Payment->Properties & ePayTypeCSV) && ((Payment->GetPay() != 0) || (Payment->GetCashOut() != 0)))
		 {
			AnsiString File = ExtractFilePath(Application->ExeName) + Payment->Name + ".csv";

			HANDLE hFile = NULL;
			hFile = CreateFile((ExtractFilePath(Application->ExeName) + Payment->Name + ".lck").t_str(), GENERIC_WRITE, 0, NULL,
			   CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			try
			{
			   TCsv Csv;
			   if (FileExists(File))
			   {
				  Csv.LoadFromFile(File);
			   }

			   Csv.Add(IntToStr(Payment->CSVNumber) + "," + FormatDateTime("ddmmyyyy", Date()) + "," + FormatDateTime("hh:nn",
					 Now()) + "," + PaymentTransaction.InvoiceNumber + "," + FloatToStrF
				  (Payment->GetPay() + Payment->GetCashOut() + Payment->GetSurcharge(), ffCurrency, 15, 2));

			   Csv.SaveToFile(File);
			}
			__finally
			{
			   CloseHandle(hFile);
			   DeleteFile(ExtractFilePath(Application->ExeName) + Payment->Name + ".lck");
			}
		 }
	  }
   }
   catch(Exception & Err)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
   }
}

void TListPaymentSystem::Initialise(Database::TDBTransaction &DBTransaction)
{
   ForceTender = TManagerVariable::Instance().GetBool(DBTransaction, vmForceTender, false);
   FTBypassElecTranTyp = TManagerVariable::Instance().GetBool(DBTransaction, vmFTBypassElecTranTyp, true);
   ManagerReference->Initialise(DBTransaction);

   try
   {
	  RecoveryFilePaymentSystem = ExtractFilePath(Application->ExeName) + "EFTPOSPaymentSystem.bin";
	  RecoveryFileSecurity = ExtractFilePath(Application->ExeName) + "EFTPOSSecurity.bin";
	  RecoveryFilePayments = ExtractFilePath(Application->ExeName) + "EFTPOSPayments.bin";
	  RecoveryFileOrders = ExtractFilePath(Application->ExeName) + "EFTPOSOrders.bin";
	  RecoveryFileOrders = ExtractFilePath(Application->ExeName) + "EFTPOSOrderItems.bin";
	  CreateRecoveryInfo();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EFTPOSLOG, "Unable to Initialise EFTPOS Data Structures" + E.Message);
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, "Unable to Initialise EFTPOS Data Structures" + E.Message);
	  EftPosEnabled = false;
   }
}

void TListPaymentSystem::RemoveTabs(TPaymentTransaction &PaymentTransaction)
{
   std::set <__int64> TabKeys; // Tabs from which the orders come from.
   TDBOrder::GetTabKeysFromOrders(PaymentTransaction.Orders, TabKeys);
   for (std::set <__int64> ::iterator itTabs = TabKeys.begin(); itTabs != TabKeys.end(); advance(itTabs, 1))
   {
	  if (TDBTab::GetTabExists(PaymentTransaction.DBTransaction, *itTabs))
	  {
		 TMMTabType TabType = TDBTab::GetTabType(PaymentTransaction.DBTransaction, *itTabs);
		 if (TabType == TabTableSeat)
		 {
			TTableSeat TableSeat;
			if (TDBTables::GetTableSeat(PaymentTransaction.DBTransaction, *itTabs, &TableSeat))
			{
			   if (TDBTables::IsEmpty(PaymentTransaction.DBTransaction, TableSeat.TableNo))
			   {
				  TDBTables::SetPartyName(PaymentTransaction.DBTransaction, TableSeat.TableNo, "");
				  // Clear the Seat Names.
				  TDBTables::ClearSeatsName(PaymentTransaction.DBTransaction, TableSeat.TableNo);
			   }
			}

			if (TDBTab::GetTabRemovable(PaymentTransaction.DBTransaction, *itTabs))
			{
			   Invoice->SetInvoiceClosed(PaymentTransaction.DBTransaction, TDBTab::GetTabInvoice(PaymentTransaction.DBTransaction, *itTabs));
			}

			if (!TDBTab::CloseTab(PaymentTransaction.DBTransaction, *itTabs))
			{
			   if (TDBTab::GetTabExists(PaymentTransaction.DBTransaction, *itTabs))
			   {
				  TDBTab::SetPartialPayment(PaymentTransaction.DBTransaction, *itTabs, PaymentTransaction.InvoiceNumber);
			   }
			}

		 }
		 else if (TabType == TabRoom)
		 {
			int RoomNumber = TDBRooms::GetRoomNumber(PaymentTransaction.DBTransaction, *itTabs);
			if (TDBTab::GetTabRemovable(PaymentTransaction.DBTransaction, *itTabs))
			{
			   // THis Tab type can never be an invoice's Tab.
			   /* int InvoiceKey = TDBTab::GetTabInvoice(*itTabs);
			   -	if(InvoiceKey != 0)
			   {
			   Invoice->SetInvoiceClosed(PaymentTransaction.DBTransaction,InvoiceKey);
			   } */

			   // Since the tab is never removed dont record partial payment info against it.
			   /* if(TDBTab::GetTabExists(*itTabs))
			   {
			   TDBTab::SetPartialPayment(*itTabs, PaymentTransaction.InvoiceNumber);
			   } */
			   TDBRooms::SetPartyName(PaymentTransaction.DBTransaction, RoomNumber, "");
			   TDBRooms::SetRoomStatus(PaymentTransaction.DBTransaction, RoomNumber, eVacant);
			}
		 }
		 else
		 {
			if (TDBTab::GetTabRemovable(PaymentTransaction.DBTransaction, *itTabs))
			{
			   Invoice->SetInvoiceClosed(PaymentTransaction.DBTransaction, TDBTab::GetTabInvoice(PaymentTransaction.DBTransaction, *itTabs));
			}

			if (!TDBTab::CloseTab(PaymentTransaction.DBTransaction, *itTabs))
			{
			   if (TDBTab::GetTabExists(PaymentTransaction.DBTransaction, *itTabs))
			   {
				  TDBTab::SetPartialPayment(PaymentTransaction.DBTransaction, *itTabs, PaymentTransaction.InvoiceNumber);
			   }
			}
		 }
	  }
   }
}

void TListPaymentSystem::OpenCashDrawer(TPaymentTransaction &PaymentTransaction)
{
   if (PaymentTransaction.TransOpenCashDraw())
   {
	  TComms::Instance().KickLocalDraw(PaymentTransaction.DBTransaction);
   }
}

void TListPaymentSystem::ReceiptPrepare(TPaymentTransaction &PaymentTransaction, bool &RequestEFTPOSReceipt)
{
   if (LastReceipt != NULL)
   {
	  delete LastReceipt;
	  LastReceipt = new TReqPrintJob(&TDeviceRealTerminal::Instance());
   }
   else
   {
	  LastReceipt = new TReqPrintJob(&TDeviceRealTerminal::Instance());
   }

   LastReceipt->Transaction = &PaymentTransaction;
   LastReceipt->JobType = pjReceiptReceipt;
   LastReceipt->SenderType = devPC;
   LastReceipt->Waiter = TDeviceRealTerminal::Instance().User.Name;

   if (PaymentTransaction.Money.TotalDiscount != 0)
   {
	  PaymentTransaction.TotalDiscount = PaymentTransaction.Money.TotalDiscount;
	  PaymentTransaction.DiscountReason = PaymentTransaction.TotalDiscount < 0 ? "Discount " : "Surcharge";
   }

   std::set <__int64> TabKeys; // Tabs from which the orders come from.
   TDBOrder::GetTabKeysFromOrders(PaymentTransaction.Orders, TabKeys);

   std::auto_ptr <TStringList> TabHistory(new TStringList);
   for (std::set <__int64> ::iterator itTabs = TabKeys.begin(); itTabs != TabKeys.end(); advance(itTabs, 1))
   {
	  TDBTab::GetPartialPaymentList(PaymentTransaction.DBTransaction, *itTabs, TabHistory.get());
	  LastReceipt->TabHistory->AddStrings(TabHistory.get());
   }

   LastReceipt->PaymentType = ptFinal;

   LastReceipt->WaitTime = TDBSaleTimes::GetAverageWaitTimeMins(PaymentTransaction.DBTransaction);
   if (PaymentTransaction.InvoiceKey != 0)
   {
	  LastReceipt->AccountPayment = true;
	  LastReceipt->AccountInvoiceNumber = Invoice->GetInvoiceNumber(PaymentTransaction.DBTransaction, PaymentTransaction.InvoiceKey);

	  TMMContactInfo InvoiceOwnerInfo;
	  int ContactKey = Invoice->GetInvoiceOwner(PaymentTransaction.DBTransaction, PaymentTransaction.InvoiceKey);
	  TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetContactDetails(PaymentTransaction.DBTransaction, ContactKey, InvoiceOwnerInfo);
	  LastReceipt->ExtraInfo->Add("Name: " + InvoiceOwnerInfo.Name);
	  LastReceipt->ExtraInfo->Add("Member No. " + InvoiceOwnerInfo.MembershipNumber);
   }

   if (PaymentTransaction.SalesType == eRoomSale)
   {
	  if (PaymentTransaction.Phoenix.AccountNumber != 0)
	  {
		 LastReceipt->ExtraInfo->Add("Room Number # " + PaymentTransaction.Phoenix.AccountNumber);
		 LastReceipt->ExtraInfo->Add("Guest " + PaymentTransaction.Phoenix.AccountName);
		 LastReceipt->ExtraInfo->Add("PMS Reference : " + PaymentTransaction.Phoenix.ReferenceNumber);
	  }
	  else
	  {
		 LastReceipt->ExtraInfo->Add("Room Number # " + IntToStr(PaymentTransaction.RoomNumber));
		 LastReceipt->ExtraInfo->Add("Guest " + TDBRooms::GetPartyName(PaymentTransaction.DBTransaction, PaymentTransaction.RoomNumber));
		 LastReceipt->ExtraInfo->Add("PMS Reference : " + PaymentTransaction.Phoenix.ReferenceNumber);
	  }
   }

   if (PaymentTransaction.SalesType == eCreditPurchase)
   {
	  LastReceipt->ExtraInfo->Add("");
	  LastReceipt->ExtraInfo->Add("Credit Purchase For Tabs... ");
	  for (std::set <__int64> ::iterator itTabs = TabKeys.begin(); itTabs != TabKeys.end(); advance(itTabs, 1))
	  {
		 if (TDBTab::GetTabExists(PaymentTransaction.DBTransaction, *itTabs))
		 {
			LastReceipt->ExtraInfo->Add("Tab " + TDBTab::GetTabName(PaymentTransaction.DBTransaction, *itTabs));
			LastReceipt->ExtraInfo->Add("Credit Purchased " + FormatFloat("$0.00", PaymentTransaction.Money.GrandTotal));
			LastReceipt->ExtraInfo->Add("New Tab Balance " + FormatFloat("$0.00", TDBTab::GetTabBalance(PaymentTransaction.DBTransaction,
					 *itTabs) - PaymentTransaction.Money.GrandTotal));
		 }
	  }
   }

   LastReceipt->MiscData = PaymentTransaction.MiscPrintData;

   Receipt->GetPrintouts(PaymentTransaction.DBTransaction, LastReceipt, TComms::Instance().ReceiptPrinter);

   if (RequestEFTPOSReceipt && EftPos->LastEftPosReceipt->Count > 0)
   {
	  TPrintout *Printout = new TPrintout();
	  if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
	  {
		 TPrinterPhysical DefaultScreenPrinter;
		 DefaultScreenPrinter.NormalCharPerLine = 40;
		 DefaultScreenPrinter.BoldCharPerLine = 40;
		 Printout->Printer = DefaultScreenPrinter;
	  }
	  else
	  {
		 Printout->Printer = TComms::Instance().ReceiptPrinter;
	  }

	  Printout->PrintFormat->Line->FontInfo.Reset();
	  Printout->PrintFormat->Line->ColCount = 1;
	  Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;
	  Printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;
	  Printout->PrintFormat->Line->Columns[0]->Text = Printout->PrintFormat->DocumentName;
	  Printout->PrintFormat->AddLine();
	  Printout->PrintFormat->NewLine();

	  for (int i = 0; i < EftPos->LastEftPosReceipt->Count; i++)
	  {
		 Printout->PrintFormat->Line->Columns[0]->Text = EftPos->LastEftPosReceipt->Strings[i];
		 Printout->PrintFormat->AddLine();
	  }
	  Printout->PrintFormat->PartialCut();
	  // TODO : The last efpos receipt isnt cleared is the next transaction has no eftpos.
	  // so all following transactions ha eftpos receipt attached
	  LastReceipt->Printouts->Add(Printout);
   }

   std::auto_ptr <TStringList> StringReceipt(new TStringList);
   LastReceipt->Printouts->PrintToStrings(StringReceipt.get());

   ManagerReceipt->ReceiptToArchive->Clear();
   ManagerReceipt->ReceiptToArchive->Position = 0;
   StringReceipt->SaveToStream(ManagerReceipt->ReceiptToArchive);
   ManagerReceipt->ReceiptToArchive->Position = 0;

   for (int i = 0; i < StringReceipt->Count; i++)
   {
	  TDeviceRealTerminal::Instance().SecurityPort->SetData(StringReceipt->Strings[i]);
   }
}

void TListPaymentSystem::SetInvoiceNumber(TPaymentTransaction &PaymentTransaction)
{
   PaymentTransaction.InvoiceNumber = Invoice->GetNextInvoiceNumber(PaymentTransaction.DBTransaction);
   PaymentTransaction.Membership.Points.SetInvoiceNumber(PaymentTransaction.InvoiceNumber);
}

void TListPaymentSystem::StoreInfo(TPaymentTransaction &PaymentTransaction)
{
   LastTotal = PaymentTransaction.Money.GrandTotal;
   LastChange = PaymentTransaction.Money.Change;
}

void TListPaymentSystem::Reset(TPaymentTransaction &PaymentTransaction)
{
   if (PaymentTransaction.Type == eTransSplitPayment || PaymentTransaction.Type == eTransPartialPayment || PaymentTransaction.Type ==
	  eTransTabSet)
   {
	  while (PaymentTransaction.Orders->Count != 0)
	  {
		 delete(TItemComplete*)PaymentTransaction.Orders->Items[0];
		 PaymentTransaction.Orders->Delete(0);
	  }
   }
}

void TListPaymentSystem::PreparePayments(TPaymentTransaction &PaymentTransaction)
{
   TPayment *CashPayment = PaymentTransaction.PaymentFind(CASH);
   if (CashPayment == NULL)
   {
	  TPayment *NewPayment = new TPayment;
	  NewPayment->Name = CASH;
	  NewPayment->Properties |= ePayTypeOpensCashDrawer;
	  NewPayment->Properties |= ePayTypeCash;
	  NewPayment->DisplayOrder = -1;
	  NewPayment->Colour = clGreen;
	  PaymentTransaction.PaymentAdd(NewPayment);
   }
}

void TListPaymentSystem::FormatSpendChit(TPaymentTransaction &PaymentTransaction)
{
   try
   {
	  if (TGlobalSettings::Instance().EnableMemberSpendChits)
	  {
		 if (PaymentTransaction.Membership.Member.Valid())
		 {
			Currency Total = PaymentTransaction.Money.GrandTotal;
			if (Total > 0.0)
			{
			   int Chits = Total / TGlobalSettings::Instance().MemberSpendChitsAmount;
			   AnsiString index = ExtractFilePath(Application->ExeName) + "\\MemberDocket.txt";
			   if (FileExists(index))
			   {
				  for (int i = 0; i < Chits; i++)
				  {
					 std::auto_ptr <TStringList> MyDocket(new TStringList);
					 MyDocket->LoadFromFile(index);
					 MyDocket->Text = StringReplace(MyDocket->Text, "%TITLE%", PaymentTransaction.Membership.Member.Title,
						TReplaceFlags() << rfReplaceAll);
					 MyDocket->Text = StringReplace(MyDocket->Text, "%RECEIPT%", PaymentTransaction.InvoiceNumber,
						TReplaceFlags() << rfReplaceAll);
					 MyDocket->Text = StringReplace(MyDocket->Text, "%USER%", TDeviceRealTerminal::Instance().User.Name, TReplaceFlags() << rfReplaceAll);
					 MyDocket->Text = StringReplace(MyDocket->Text, "%POS%", TDeviceRealTerminal::Instance().ID.Name, TReplaceFlags() << rfReplaceAll);
					 MyDocket->Text = StringReplace(MyDocket->Text, "%NAME%", PaymentTransaction.Membership.Member.Name,
						TReplaceFlags() << rfReplaceAll);
					 MyDocket->Text = StringReplace(MyDocket->Text, "%ALIAS%", PaymentTransaction.Membership.Member.Alias,
						TReplaceFlags() << rfReplaceAll);
					 MyDocket->Text = StringReplace(MyDocket->Text, "%NUMBER%", PaymentTransaction.Membership.Member.MembershipNumber,
						TReplaceFlags() << rfReplaceAll);
					 MyDocket->Text = StringReplace(MyDocket->Text, "%VALUE%",
						"$" + CurrToStr(TGlobalSettings::Instance().MemberSpendChitsAmount), TReplaceFlags() << rfReplaceAll);
					 MyDocket->Text = StringReplace(MyDocket->Text, "%DATETIME%", Now().FormatString("dd/mm/yyyy hh:nn:ss am/pm"),
						TReplaceFlags() << rfReplaceAll);
					 PrintSpendChit(MyDocket.get());
				  }
			   }
			}
		 }
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
   }
}

void TListPaymentSystem::PrintSpendChit(TStringList *Docket)
{
   try
   {

	  if (Comms)
	  {
		 if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey != 0)
		 {
			std::auto_ptr <TPrintout> Printout(new TPrintout);
			Printout->Printer = TComms::Instance().ReceiptPrinter;
			Printout->PrintFormat->DocumentName = "Spend Chit Docket";

			std::auto_ptr <TStringList> MultiLine(new TStringList);
			for (int i = 0; i < Docket->Count; i++)
			{
			   MultiLine->Clear();
			   AnsiString Line = Docket->Strings[i];

			   Printout->PrintFormat->Line->ColCount = 1;
			   Printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;
			   Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;
			   Printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;

			   if (Line.Pos("?DH?"))
			   {
				  Printout->PrintFormat->Line->FontInfo.Height = fsDoubleSize;
				  Line = StringReplace(Line, "?DH?", "", TReplaceFlags() << rfReplaceAll);
			   }
			   else
			   {
				  Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
			   }

			   if (Line.Pos("?DW?"))
			   {
				  Printout->PrintFormat->Line->FontInfo.Width = fsDoubleSize;
				  Line = StringReplace(Line, "?DW?", "", TReplaceFlags() << rfReplaceAll);
			   }
			   else
			   {
				  Printout->PrintFormat->Line->FontInfo.Width = fsNormalSize;
			   }

			   if (Line.Pos("?DL?"))
			   {
				  Line = StringReplace(Line, "?DL?", "", TReplaceFlags() << rfReplaceAll);
				  Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;
				  Printout->PrintFormat->Line->Columns[0]->DoubleLine();
				  Printout->PrintFormat->AddLine();
			   }
			   else
			   {
				  Printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;
				  Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;
				  MultiLine->Text = StringReplace(Line, "<br>", "\r", TReplaceFlags() << rfReplaceAll);
				  for (int j = 0; j < MultiLine->Count; j++)
				  {
					 Printout->PrintFormat->Line->Columns[0]->Text = MultiLine->Strings[j];
					 Printout->PrintFormat->AddLine();
				  }
			   }
			}
			Printout->PrintFormat->Cut();
			Printout->PrintFormat->Print(Printout->Printer.UNCName(), "");
		 }
	  }

   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
   }
}

void TListPaymentSystem::ReceiptPrint(TPaymentTransaction &PaymentTransaction, bool RequestEFTPOSReceipt, bool CloseAndPrint)
{
   if (PaymentTransaction.Type == eTransQuickSale)
   {
	  if (Receipt->AlwaysPrintReceiptCashSales)
	  {
		 if (RequestEFTPOSReceipt && TGlobalSettings::Instance().DuplicateEftPosReceipt)
		 {
			// Print all the TPrintouts including the EFTPOS one.
			LastReceipt->Printouts->Print(TDeviceRealTerminal::Instance().ID.Type);
		 }
		 else
		 {
			// Only print the first TPrintout as the EFTPOS job does not need duplication.
			LastReceipt->Printouts->Print(0, TDeviceRealTerminal::Instance().ID.Type);
		 }

		 if (TGlobalSettings::Instance().DuplicateReceipts)
		 {
			if (RequestEFTPOSReceipt && TGlobalSettings::Instance().DuplicateEftPosReceipt)
			{
			   // Print all the TPrintouts including the EFTPOS one.
			   LastReceipt->Printouts->Print(TDeviceRealTerminal::Instance().ID.Type);
			}
			else
			{
			   // Only print the first TPrintout as the EFTPOS job does not need duplication.
			   LastReceipt->Printouts->Print(0, TDeviceRealTerminal::Instance().ID.Type);
			}
		 }
	  }
   }
   else
   {
	  if (Receipt->AlwaysPrintReceiptTenderedSales)
	  {
		 if (RequestEFTPOSReceipt && TGlobalSettings::Instance().DuplicateEftPosReceipt)
		 {
			// Print all the TPrintouts including the EFTPOS one.
			LastReceipt->Printouts->Print(TDeviceRealTerminal::Instance().ID.Type);
		 }
		 else
		 {
			// Only print the first TPrintout as the EFTPOS job does not need duplication.
			LastReceipt->Printouts->Print(0, TDeviceRealTerminal::Instance().ID.Type);
		 }

		 if (TGlobalSettings::Instance().DuplicateReceipts)
		 {
			if (RequestEFTPOSReceipt && TGlobalSettings::Instance().DuplicateEftPosReceipt)
			{
			   // Print all the TPrintouts including the EFTPOS one.
			   LastReceipt->Printouts->Print(TDeviceRealTerminal::Instance().ID.Type);
			}
			else
			{
			   // Only print the first TPrintout as the EFTPOS job does not need duplication.
			   LastReceipt->Printouts->Print(0, TDeviceRealTerminal::Instance().ID.Type);
			}
		 }
	  }
	  else
	  {
		 if (CloseAndPrint)
		 {
			if (RequestEFTPOSReceipt && TGlobalSettings::Instance().DuplicateEftPosReceipt)
			{
			   // Print all the TPrintouts including the EFTPOS one.
			   LastReceipt->Printouts->Print(TDeviceRealTerminal::Instance().ID.Type);
			}
			else
			{
			   // Only print the first TPrintout as the EFTPOS job does not need duplication.
			   LastReceipt->Printouts->Print(0, TDeviceRealTerminal::Instance().ID.Type);
			}

			if (TGlobalSettings::Instance().DuplicateReceipts)
			{
			   if (RequestEFTPOSReceipt && TGlobalSettings::Instance().DuplicateEftPosReceipt)
			   {
				  // Print all the TPrintouts including the EFTPOS one.
				  LastReceipt->Printouts->Print(TDeviceRealTerminal::Instance().ID.Type);
			   }
			   else
			   {
				  // Only print the first TPrintout as the EFTPOS job does not need duplication.
				  LastReceipt->Printouts->Print(0, TDeviceRealTerminal::Instance().ID.Type);
			   }
			}
		 }
		 else
		 {
			// Print Only the EFTPOS Receipt as the want duplication of only the EFTPOS dockets.
			if (RequestEFTPOSReceipt && TGlobalSettings::Instance().DuplicateEftPosReceipt)
			{
			   LastReceipt->Printouts->Print(1, TDeviceRealTerminal::Instance().ID.Type);
			}
		 }
	  }
   }
}

void TListPaymentSystem::RemoveOrders(TPaymentTransaction &PaymentTransaction)
{
   for (int CurrentIndex = 0; CurrentIndex < PaymentTransaction.Orders->Count; CurrentIndex++)
   {
	  TItemComplete *Order = (TItemComplete*)(PaymentTransaction.Orders->Items[CurrentIndex]);
	  for (int i = 0; i < Order->SubOrders->Count; i++)
	  {
		 TItemCompleteSub *CurrentSubOrder = (TItemCompleteSub*)Order->SubOrders->Items[i];
		 TDBOrder::DeleteOrder(PaymentTransaction.DBTransaction, CurrentSubOrder->OrderKey);
	  }
	  TDBOrder::DeleteOrder(PaymentTransaction.DBTransaction, Order->OrderKey);
   }
}

bool TListPaymentSystem::ProcessThirdPartyModules(TPaymentTransaction &PaymentTransaction, bool &RequestEFTPOSReceipt)
{
   bool RetVal = false;
   bool PointsOk = true;
   bool ChequesOk = true;
   bool EftPosOk = true;
   bool PhoenixHSOk = true;
   bool DialogsOk = true;
   bool InvoiceOk = true;
   bool XMLExportOK = true;
   bool PocketVoucher = true;

   if (PaymentTransaction.TransVerifyCheque())
   {
	  for (int i = 0; i < PaymentTransaction.PaymentsCount(); i++)
	  {
		 TPayment *Payment = PaymentTransaction.PaymentGet(i);
		 if ((Payment->GetCashOut() != 0 || Payment->GetPay() != 0) && (Payment->Result != eAccepted)
			&& Payment->Properties & ePayTypeChequeVerify)
		 {
			GetChequeVerifyResult(Payment);
			if (Payment->Result != eAccepted)
			{
			   Payment->Failed();
			   ChequesOk = false;
			}
		 }
	  }
   }

   if (!ChequesOk)
	  return RetVal;

   if (PaymentTransaction.TransElectronicPayment())
   {
	  if (PaymentTransaction.TransIntegratedEFTPOS())
	  {
		 RequestEFTPOSReceipt = true;
	  }

	  for (int i = 0; i < PaymentTransaction.PaymentsCount(); i++)
	  {
		 TPayment *Payment = PaymentTransaction.PaymentGet(i);
		 if ((Payment->Properties & ePayTypeElectronicTransaction) && (Payment->GetCashOut() != 0 || Payment->GetPay() != 0) &&
			(Payment->Result != eAccepted))
		 {
			TransRetriveElectronicResult(PaymentTransaction, Payment);
			if (Payment->Result != eAccepted)
			{
			   Payment->Failed();
			   EftPosOk = false;
			}
			else
			{
			   if (Payment->ReferenceNumber != "")
			   {
				  PaymentTransaction.References.push_back(RefRefType(Payment->ReferenceNumber,
						ManagerReference->GetReferenceByType(PaymentTransaction.DBTransaction, REFTYPE_EFTPOS)));
			   }
			}
			if (Payment->SuppressEftPosReceipt)
			{
			   RequestEFTPOSReceipt = false;
			}
		 }
	  }
   }

   if (!EftPosOk)
	  return RetVal;

   if (PaymentTransaction.TransInvoicePayment())
   {
	  for (int i = 0; i < PaymentTransaction.PaymentsCount(); i++)
	  {
		 TPayment *Payment = PaymentTransaction.PaymentGet(i);
		 if ((Payment->Properties & ePayTypeInvoiceExport) && (Payment->GetCashOut() != 0 || Payment->GetPay() != 0) &&
			(Payment->Result != eAccepted))
		 {
			TransRetriveInvoiceResult(PaymentTransaction, Payment);
			if (Payment->Result != eAccepted)
			{
			   Payment->Failed();
			   InvoiceOk = false;
			}
		 }
	  }
   }

   if (!InvoiceOk)
	  return RetVal;

   DialogsOk = ProcessComfirmationDialogs(PaymentTransaction);

   if (!DialogsOk)
	  return RetVal;

   if (PhoenixHM->Enabled)
   {
	  PhoenixHSOk = TransRetrivePhoenixResult(PaymentTransaction);
   }

   if (!PhoenixHSOk)
	  return RetVal;

   PointsOk = TDeviceRealTerminal::Instance().ManagerMembership->ProcessPoints(PaymentTransaction);

   for (int i = 0; i < PaymentTransaction.PaymentsCount(); i++)
   {
	  TPayment *Payment = PaymentTransaction.PaymentGet(i);
	  if ((Payment->Properties & ePayTypePocketVoucher) && (Payment->GetCashOut() != 0 || Payment->GetPay() != 0) &&
		 (Payment->Result != eAccepted))
	  {
		 Payment->ReferenceNumber = ManagerReference->BuildReference(TGlobalSettings::Instance().SiteID, TDeviceRealTerminal::Instance().ID.Name,
			REFTYPE_POCKETVOUCHER_CODE);
		 TDeviceRealTerminal::Instance().PocketVouchers->ProcessVoucher(PaymentTransaction, Payment);
		 if (Payment->Result != eAccepted)
		 {
			Payment->Failed();
			PocketVoucher = false;
		 }
		 else
		 {
			PaymentTransaction.References.push_back(RefRefType(Payment->ReferenceNumber,
				  ManagerReference->GetReferenceByType(PaymentTransaction.DBTransaction, REFTYPE_POCKETVOUCHER_MMTRANS)));
			PaymentTransaction.References.push_back(RefRefType(Payment->VoucherCode,
				  ManagerReference->GetReferenceByType(PaymentTransaction.DBTransaction, REFTYPE_POCKETVOUCHER_CODE)));
			PaymentTransaction.References.push_back(RefRefType(Payment->ExternalReferenceNumber,
				  ManagerReference->GetReferenceByType(PaymentTransaction.DBTransaction, REFTYPE_POCKETVOUCHER_PVTRANS)));
		 }
	  }
   }

   if (!PointsOk)
	  return RetVal;

   RetVal = ChequesOk && EftPosOk && PhoenixHSOk && DialogsOk && PointsOk && PocketVoucher;
   return RetVal;
}

void TListPaymentSystem::ClearDiscounts(TPaymentTransaction &PaymentTransaction)
{
   ManagerDiscount->ClearDiscounts(PaymentTransaction.Orders);
}

// bool TListPaymentSystem::SplitPayment(TPaymentTransaction &PaymentTransaction,TSplitMoney *SplitMoney, Currency &SplitPercentage)
bool TListPaymentSystem::SplitPayment(TPaymentTransaction &PaymentTransaction, TSplitMoney *SplitMoney, double &SplitPercentage)
{
   bool Retval = false;

   SplitMoney->TotalCost = 0;
   SplitMoney->TotalDiscount = 0;
   for (int i = 0; i < PaymentTransaction.Orders->Count; i++)
   {
	  TItemComplete *Order = (TItemComplete*)PaymentTransaction.Orders->Items[i];
	  SplitMoney->TotalCost += Order->TotalPriceDiscountSides();
	  SplitMoney->TotalDiscount += Order->TotalDiscountSides();
   }

   SplitMoney->LeftOwing = 0;

   frmSplitPayment->TotalDue = SplitMoney->TotalCost;
   frmSplitPayment->DivisionsLeft = SplitMoney->DivisionsLeft;
   if (frmSplitPayment->ShowModal() == mrOk)
   {
	  SplitMoney->DivisionsLeft = frmSplitPayment->DivisionsLeft;
	  SplitMoney->PaymentAmount = frmSplitPayment->PaymentAmount;
	  SplitMoney->NumberOfPayments = frmSplitPayment->NumberOfPayments;

	  if (SplitMoney->PaymentAmount == SplitMoney->TotalCost)
	  {
		 SplitMoney->LeftOwing = 0;
	  }
	  else
	  {
		 SplitMoney->LeftOwing = SplitMoney->TotalCost - SplitMoney->PaymentAmount;
	  }

	  SplitPercentage = (double)(SplitMoney->PaymentAmount) / (double)(SplitMoney->TotalCost);

	  SplitMoney->GSTContent = 0;
	  for (int i = 0; i < PaymentTransaction.Orders->Count; i++)
	  {
		 TItemComplete *Order = (TItemComplete*)PaymentTransaction.Orders->Items[i];
		 SplitMoney->GSTContent += Order->TotalGSTContentSides();
	  }
	  PaymentTransaction.Money.Recalc(PaymentTransaction);

	  Retval = true;
   }
   return Retval;
}

void TListPaymentSystem::AdjustCredit(TPaymentTransaction &PaymentTransaction)
{
   for (std::map <long, TTabCredit> ::iterator itCredit = PaymentTransaction.TabCredit.begin();
	  itCredit != PaymentTransaction.TabCredit.end(); advance(itCredit, 1))
   {
	  TDBTab::SetTabCredit(PaymentTransaction.DBTransaction, itCredit->first,
		 itCredit->second.CurrentCredit - itCredit->second.CreditRedeemed);
   }
}

void TListPaymentSystem::ProcessSecurity(TPaymentTransaction &PaymentTransaction)
{

   bool AddDiscountSecurity = false;
   AnsiString DiscountReason = "";
   if (PaymentTransaction.Orders != NULL)
   {
	  for (int i = 0; i < PaymentTransaction.Orders->Count; i++)
	  {
		 TItemComplete *Order = (TItemComplete*)PaymentTransaction.Orders->Items[i];
		 if (Order->TotalDiscount() != 0)
		 {
            AnsiString Reason = Order->DiscountReasons();
            if(DiscountReason.Pos(Reason) == 0)
            {
   				DiscountReason += " " + Reason;
            }
			AddDiscountSecurity = true;
		 }

		 for (int i = 0; i < Order->SubOrders->Count; i++)
		 {
			TItemCompleteSub *SubOrder = Order->SubOrders->SubOrderGet(i);
			if (SubOrder)
			{
			   if (SubOrder->TotalDiscount() != 0)
			   {
                  AnsiString Reason = SubOrder->DiscountReasons();
                  if(DiscountReason.Pos(Reason) == 0)
                  {
                     DiscountReason += " " + Reason;
                  }
				  AddDiscountSecurity = true;
			   }
			}
		 }
	  }
   }

   if (AddDiscountSecurity)
   {
      TDeviceRealTerminal::Instance().PaymentSystem->Security->SecurityDelete(TDeviceRealTerminal::Instance().PaymentSystem->Security->SecurityGetType(secDiscountedBy));
      // Add Security.
      TSecurityReference *SecRef = new TSecurityReference;
      SecRef->UserKey = TDeviceRealTerminal::Instance().User.ContactKey;
      SecRef->Event = SecurityTypes[secDiscountedBy];
      SecRef->From = DiscountReason.SubString(1,50);
      SecRef->To = CurrToStrF(PaymentTransaction.TotalDiscount, ffCurrency, 2);
      SecRef->Note = DiscountReason.SubString(1,50);
      SecRef->TimeStamp = Now();
      Security->SecurityAdd(SecRef);
   }

   TSecurityReference *SecRef = new TSecurityReference;
   SecRef->UserKey = TDeviceRealTerminal::Instance().User.ContactKey;
   SecRef->Event = SecurityTypes[secBilledBy];
   SecRef->From = TDeviceRealTerminal::Instance().User.Name;
   SecRef->To = TDeviceRealTerminal::Instance().User.Initials;
   SecRef->TimeStamp = Now();
   Security->SecurityAdd(SecRef);
   TDBSecurity::ProcessSecurity(PaymentTransaction.DBTransaction, Security);

   if (PaymentTransaction.Orders != NULL)
   {
	  for (int i = 0; i < PaymentTransaction.Orders->Count; i++)
	  {
		 TItemComplete *Order = (TItemComplete*)PaymentTransaction.Orders->Items[i];
		 TDBSecurity::ProcessSecurity(PaymentTransaction.DBTransaction, Order->Security);
		 for (int i = 0; i < Order->SubOrders->Count; i++)
		 {
			TItemCompleteSub *SubOrder = Order->SubOrders->SubOrderGet(i);
			if (SubOrder)
			{
			   TDBSecurity::ProcessSecurity(PaymentTransaction.DBTransaction, SubOrder->Security);
			}
		 }
	  }
   }
}

int TListPaymentSystem::GetPartialPayment(TPaymentTransaction &PaymentTransaction, double &SplitPercentage, Currency &RequestedTotal)
{
   int RetVal = 0;

   double TotalCost = TOrderUtils::TotalPriceDiscountSides(PaymentTransaction.Orders);

   std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (Screen->ActiveForm));
   frmTouchNumpad->Caption = "Enter the amount to pay off";
   frmTouchNumpad->CURInitial = TotalCost;
   frmTouchNumpad->btnSurcharge->Caption = "Ok";
   frmTouchNumpad->btnDiscount->Visible = false;
   frmTouchNumpad->btnSurcharge->Visible = true;
   frmTouchNumpad->Mode = pmCurrency;
   if (frmTouchNumpad->ShowModal() == mrOk)
   {
	  if (frmTouchNumpad->CURResult > TotalCost)
	  {
		 MessageBox("This amount is larger than what is owed.", "Error", MB_ICONERROR + MB_OK);
	  }
	  else if (frmTouchNumpad->CURResult != TotalCost)
	  {
		 SplitPercentage = (double)(frmTouchNumpad->CURResult) / (double)TotalCost;
		 TManagerPatron::Instance().SetDefaultPatrons(PaymentTransaction.DBTransaction, PaymentTransaction.Patrons, 1);
		 RequestedTotal = frmTouchNumpad->CURResult;
		 RetVal = 1;
	  }
	  else
	  { // They are partially paying for the whole thing....
		 RequestedTotal = frmTouchNumpad->CURResult;
		 RetVal = 2;
	  }
   }
   return RetVal;
}

bool TListPaymentSystem::ProcessComfirmationDialogs(TPaymentTransaction &PaymentTransaction)
{
   bool AllOk = true;

   for (int i = 0; i < PaymentTransaction.PaymentsCount(); i++)
   {
	  TPayment *Payment = PaymentTransaction.PaymentGet(i);
	  if (Payment->GetCashOut() != 0 || Payment->GetPay() != 0)
	  {
         std::auto_ptr<TfrmEftPos>frmEftPos(TfrmEftPos::Create<TfrmEftPos>(Screen->ActiveForm));
		 if ((Payment->Properties & ePayTypeCheckAccepted) && (frmEftPos->TransactionOk(Payment) == mrNo))
		 {
			Payment->Result = eDeclined;
			Payment->SetPay(0);
			Payment->SetCashOut(0);
			AllOk = false;
		 }
		 else if ((Payment->Properties & ePayTypeCheckSignature) && (frmEftPos->SignatureOk() == mrNo))
		 {
			Payment->Result = eDeclined;
			Payment->SetPay(0);
			Payment->SetCashOut(0);
			AllOk = false;
		 }
	  }
   }
   return AllOk;

}

void TListPaymentSystem::ClearRecoveryInfo()
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

void TListPaymentSystem::CreateRecoveryInfo()
{
}

bool TListPaymentSystem::LoadRecoveryInfo(TPaymentTransaction &RecoverTransaction)
{
   bool retval = false;
   try
   {
	  if (FileExists(RecoveryFilePayments) && FileExists(RecoveryFileOrders) && FileExists(RecoveryFilePaymentSystem) && FileExists
		 (RecoveryFileSecurity))
	  {
		 retval = true;

		 TListPaymentSystem * ptrPaySys = this;
		 RecoverTransaction.PaymentsClear();

		 TCsv PaymentCsv;
		 PaymentCsv.LoadFromFile(RecoveryFilePaymentSystem);

		 RecoverTransaction.SalesType = eTransactionType(StrToInt(PaymentCsv.Cells[0][0]));
		 RecoverTransaction.DiscountReason = PaymentCsv.Cells[1][0];
		 RecoverTransaction.ChitNumber.ChitNumber = PaymentCsv.Cells[2][0];
		 RecoverTransaction.ChitNumber.ChitNumberKey = StrToInt(PaymentCsv.Cells[3][0]);
		 RecoverTransaction.ChitNumber.Name = PaymentCsv.Cells[4][0];
		 RecoverTransaction.ChitNumber.Mask = PaymentCsv.Cells[5][0];
		 RecoverTransaction.ChitNumber.Type = StrToInt(PaymentCsv.Cells[6][0]);
		 RecoverTransaction.BookingID = StrToInt(PaymentCsv.Cells[7][0]);
		 TManagerPatron::Instance().SetDefaultPatrons(RecoverTransaction.DBTransaction, RecoverTransaction.Patrons, StrToInt(PaymentCsv.Cells[8][0]));
		 RecoverTransaction.Money.PaymentDue = StrToCurr(PaymentCsv.Cells[9][0]);
		 RecoverTransaction.Money.Change = StrToCurr(PaymentCsv.Cells[10][0]);
		 RecoverTransaction.Money.InvoiceTotal = StrToCurr(PaymentCsv.Cells[11][0]);
		 RecoverTransaction.Money.GrandTotal = StrToCurr(PaymentCsv.Cells[12][0]);
		 RecoverTransaction.Money.PaymentAmount = StrToCurr(PaymentCsv.Cells[13][0]);
		 RecoverTransaction.Money.TotalRounding = StrToCurr(PaymentCsv.Cells[14][0]);
		 RecoverTransaction.Money.GrandTotal = StrToCurr(PaymentCsv.Cells[15][0]);
		 RecoverTransaction.Money.CreditAvailable = StrToCurr(PaymentCsv.Cells[16][0]);
		 RecoverTransaction.Money.TotalDiscount = StrToCurr(PaymentCsv.Cells[17][0]);
		 int StaffKey = StrToInt(PaymentCsv.Cells[18][0]);
		 RecoverTransaction.RewardsTransaction.CardNumber = PaymentCsv.Cells[19][0];
		 RecoverTransaction.RewardsTransaction.EFTPOSRef = PaymentCsv.Cells[20][0];
		 RecoverTransaction.RewardsTransaction.Amount = StrToCurr(PaymentCsv.Cells[21][0]);

		 // Relog in user.

		 TMMContactInfo TempUserInfo;
		 std::auto_ptr <TContactStaff> Staff(new TContactStaff(RecoverTransaction.DBTransaction));
		 Staff->GetContactDetails(RecoverTransaction.DBTransaction, StaffKey, TempUserInfo);
		 TDeviceRealTerminal::Instance().User = TempUserInfo;

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
			ptrPaySys->Security->SecurityAdd(SecRef);
		 }

		 // Restore Payments.
		 TCsv PaymentTypesCsv;
		 PaymentTypesCsv.LoadFromFile(RecoveryFilePayments);

		 for (int i = 0; i < PaymentTypesCsv.RowCount; i++)
		 {
			TPayment *Payment = new TPayment;
			Payment->Name = PaymentTypesCsv.Cells[0][i];
			Payment->Properties = StrToInt(PaymentTypesCsv.Cells[1][i]);
			Payment->Colour = StrToInt(PaymentTypesCsv.Cells[2][i]);
			Payment->DisplayOrder = StrToInt(PaymentTypesCsv.Cells[3][i]);
			Payment->PercentAdjust = double(StrToFloat(PaymentTypesCsv.Cells[4][i]));
			Payment->AmountAdjust = double(StrToFloat(PaymentTypesCsv.Cells[5][i]));
			Payment->AdjustmentReason = PaymentTypesCsv.Cells[6][i];
			Payment->GroupNumber = StrToInt(PaymentTypesCsv.Cells[7][i]);
			Payment->SetPay(double(StrToFloat(PaymentTypesCsv.Cells[8][i])));
			Payment->SetCashOut(double(StrToFloat(PaymentTypesCsv.Cells[9][i])));
			Payment->SetSurcharge(double(StrToFloat(PaymentTypesCsv.Cells[10][i])));
			Payment->Visible = StrToInt(PaymentTypesCsv.Cells[11][i]);
			Payment->CreditTransaction = StrToInt(PaymentTypesCsv.Cells[12][i]);
			Payment->CSVNumber = StrToInt(PaymentTypesCsv.Cells[13][i]);
			Payment->NameOveride = PaymentTypesCsv.Cells[14][i];
			Payment->Note = PaymentTypesCsv.Cells[15][i];
			Payment->ReferenceNumber = PaymentTypesCsv.Cells[16][i];
			RecoverTransaction.PaymentAdd(Payment);
		 }

		 // Restore Orders.
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
		 TDBOrder::GetOrdersFromOrderKeys(RecoverTransaction.DBTransaction, RecoverTransaction.Orders, OrderKeys);

		 // Retrive Security Ref and discount values.
		 for (int i = 0; i < RecoverTransaction.Orders->Count; i++)
		 {
			TItemComplete *Order = (TItemComplete*)RecoverTransaction.Orders->Items[i];
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

void TListPaymentSystem::SaveRecoveryInfo(TPaymentTransaction &PaymentTransaction)
{
   try
   {
	  ClearRecoveryInfo();

	  TListPaymentSystem * ptrPaySys = this;
	  int TotalPatrons = TManagerPatron::Instance().GetTotalPatrons(PaymentTransaction.Patrons);

	  TCsv PaymentCsv;
	  PaymentCsv.Add(IntToStr(PaymentTransaction.SalesType) + "," + PaymentTransaction.DiscountReason + "," +
		 PaymentTransaction.ChitNumber.ChitNumber + "," + IntToStr(PaymentTransaction.ChitNumber.ChitNumberKey)
		 + "," + PaymentTransaction.ChitNumber.Name + "," + PaymentTransaction.ChitNumber.Mask + "," + IntToStr
		 (PaymentTransaction.ChitNumber.Type) + "," + IntToStr(PaymentTransaction.BookingID) + "," + IntToStr(TotalPatrons)
		 + "," + CurrToStr(PaymentTransaction.Money.PaymentDue) + "," + CurrToStr(PaymentTransaction.Money.Change) + "," + CurrToStr
		 (PaymentTransaction.Money.InvoiceTotal) + "," + CurrToStr(PaymentTransaction.Money.GrandTotal) + "," + CurrToStr
		 (PaymentTransaction.Money.PaymentAmount) + "," + CurrToStr(PaymentTransaction.Money.TotalRounding) + "," + CurrToStr
		 (PaymentTransaction.Money.GrandTotal) + "," + CurrToStr(PaymentTransaction.Money.CreditAvailable) + "," + CurrToStr
		 (PaymentTransaction.Money.TotalDiscount) + "," + IntToStr(TDeviceRealTerminal::Instance().User.ContactKey)
		 + "," + PaymentTransaction.RewardsTransaction.CardNumber + "," + PaymentTransaction.RewardsTransaction.EFTPOSRef + "," + CurrToStr
		 (PaymentTransaction.RewardsTransaction.Amount));

	  PaymentCsv.SaveToFile(RecoveryFilePaymentSystem);

	  TCsv PaySysSecurityCsv;
	  for (int j = 0; j < ptrPaySys->Security->Count; j++)
	  {
		 TSecurityReference *SecRef = ptrPaySys->Security->SecurityGet(j);
		 PaySysSecurityCsv.Add(
			// IntToStr(ptrPaySys->Security->GetSecurityRefNumber()) + ","+
			SecRef->Event + "," + SecRef->From + "," + SecRef->To + "," + SecRef->Note + "," + ::IntToStr(SecRef->UserKey)
			+ "," + FloatToStrF(double(SecRef->TimeStamp), ffGeneral, 15, 0));
	  }
	  PaySysSecurityCsv.SaveToFile(RecoveryFileSecurity);

	  TCsv PaymentTypesCsv;
	  for (int i = 0; i < PaymentTransaction.PaymentsCount(); i++)
	  {
		 TPayment *Payment = PaymentTransaction.PaymentGet(i);
		 PaymentTypesCsv.Add(Payment->Name + "," + IntToStr(Payment->Properties) + "," + IntToStr(Payment->Colour) + "," + IntToStr
			(Payment->DisplayOrder) + "," + FloatToStrF(Payment->PercentAdjust, ffGeneral, 15,
			   0) + "," + FloatToStrF(Payment->AmountAdjust, ffGeneral, 15, 0) + "," + Payment->AdjustmentReason + "," + IntToStr
			(Payment->GroupNumber) + "," + CurrToStr(Payment->GetPay()) + "," + CurrToStr(Payment->GetCashOut()) + "," + CurrToStr
			(Payment->GetSurcharge()) + "," + IntToStr((int)Payment->Visible) + "," + IntToStr((int)Payment->CreditTransaction)
			+ "," + IntToStr((int)Payment->CSVNumber) + "," + Payment->NameOveride + "," + Payment->Note + "," + Payment->ReferenceNumber);
	  }
	  PaymentTypesCsv.SaveToFile(RecoveryFilePayments);

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

	  __int64 EndTime = ::GetTickCount() + 15000;
	  bool Saved = false;
	  while (::GetTickCount() < EndTime && !Saved)
	  {
		 if (FileExists(RecoveryFilePaymentSystem) && FileExists(RecoveryFileSecurity) && FileExists(RecoveryFilePayments) && FileExists
			(RecoveryFileOrders))
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

void TListPaymentSystem::ProcessRecoveryInfo()
{
   if (EftPos->Enabled)
   {
	  try
	  {
		 Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		 TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
		 DBTransaction.StartTransaction();

		 TPaymentTransaction RecoverTransaction(DBTransaction);
		 try
		 {
			if (LoadRecoveryInfo(RecoverTransaction))
			{
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
			   ProcessTransaction(RecoverTransaction);
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

void TListPaymentSystem::UpdateFreebieRewards(TPaymentTransaction &PaymentTransaction)
{
   if (!PaymentTransaction.CreditTransaction)
   {
	  bool AddToMemberPurchaseCount = false;
	  bool AddToLocationPurchaseCount = false;

	  switch(ManagerFreebie->MemberLoyaltyRewardsRules)
	  {
	  case eFSRDisabled:
		 AddToMemberPurchaseCount = false;
		 break;
	  case eFSREnabledAll:
		 AddToMemberPurchaseCount = true;
		 break;
	  case eFSRCash:
		 switch(PaymentTransaction.SalesType)
		 {
		 case eCash:
			AddToMemberPurchaseCount = true;
			break;
		 default:
			AddToMemberPurchaseCount = false;
		 }
		 break;
	  case eFSRSaved:
		 switch(PaymentTransaction.SalesType)
		 {
		 case eTab:
		 case eTableSeat:
		 case eAccount:
		 case eRoomSale:
			AddToMemberPurchaseCount = true;
			break;
		 default:
			AddToMemberPurchaseCount = false;
		 }
		 break;
	  }

	  switch(ManagerFreebie->LocationLoyaltyRewardsRules)
	  {
	  case eFSRDisabled:
		 AddToLocationPurchaseCount = false;
		 break;
	  case eFSREnabledAll:
		 AddToLocationPurchaseCount = true;
		 break;
	  case eFSRCash:
		 switch(PaymentTransaction.SalesType)
		 {
		 case eCash:
			AddToLocationPurchaseCount = true;
			break;
		 default:
			AddToLocationPurchaseCount = false;
		 }
		 break;
	  case eFSRSaved:
		 switch(PaymentTransaction.SalesType)
		 {
		 case eTab:
		 case eTableSeat:
		 case eAccount:
		 case eRoomSale:
			AddToLocationPurchaseCount = true;
			break;
		 default:
			AddToLocationPurchaseCount = false;
		 }
		 break;
	  }

	  for (int i = 0; i < PaymentTransaction.Orders->Count; i++)
	  {
		 TItemComplete *Order = (TItemComplete*)PaymentTransaction.Orders->Items[i];
		 if (!Order->IsFreebe)
		 {
			if (PaymentTransaction.Membership.Member.ContactKey != 0 && AddToMemberPurchaseCount)
			{
			   ManagerFreebie->HasPurchased(PaymentTransaction.DBTransaction, &PaymentTransaction.Membership.Member, Order->Item,
				  Order->Size, 1, Order->MemberFreeSaleCount);
			}

			if (AddToLocationPurchaseCount)
			{
			   ManagerFreebie->HasPurchased(PaymentTransaction.DBTransaction, TDeviceRealTerminal::Instance().ID.LocationKey, Order->Item, Order->Size, 1,
				  Order->LocationFreeSaleCount);
			}
		 }

		 for (int j = 0; j < Order->SubOrders->Count; j++)
		 {
			TItemCompleteSub *SubOrder = (TItemCompleteSub*)Order->SubOrders->Items[j];
			if (!SubOrder->IsFreebe)
			{
			   if (PaymentTransaction.Membership.Member.ContactKey != 0 && AddToMemberPurchaseCount)
			   {
				  ManagerFreebie->HasPurchased(PaymentTransaction.DBTransaction, &PaymentTransaction.Membership.Member, SubOrder->Item,
					 SubOrder->Size, 1, SubOrder->MemberFreeSaleCount);
			   }

			   if (AddToLocationPurchaseCount)
			   {
				  ManagerFreebie->HasPurchased(PaymentTransaction.DBTransaction, TDeviceRealTerminal::Instance().ID.LocationKey, SubOrder->Item,
					 SubOrder->Size, 1, SubOrder->LocationFreeSaleCount);
			   }
			}
		 }
	  }
   }
}

void TListPaymentSystem::ProcessRewardSchemes(TPaymentTransaction & PaymentTransaction)
{
/*   if (TGlobalSettings::Instance().EnableAARewards)
   {
	  try
	  {
		 bool Proceed = false;
		 bool RetrieveCard = true;
		 ePANSource PanSource = PANSource_CardReader;

		 eEFTTransactionType TransType;
		 if (PaymentTransaction.Type == eTransRewardsRecovery)
		 {
			TransType = TransactionType_INQUIRY;
			Proceed = true;
			RetrieveCard = false;
		 }
		 else
		 {
			TransType = TransactionType_PURCHASE;
			if (MessageBox("Do you have an AA Rewards Card?.\r", "AA Rewards?", MB_YESNO + MB_ICONQUESTION) == IDYES)
			{
			   Proceed = true;
			   RetrieveCard = true;
			   PaymentTransaction.RewardsTransaction.Amount = PaymentTransaction.Money.ProductAmount;
			   PaymentTransaction.RewardsTransaction.EFTPOSRef = EftPos->GetRefNumber();
			}
			else
			{
			   Proceed = false;
			}

		 }

		 eTransactionResult Result = eProcessing;
		 Currency Pay = PaymentTransaction.RewardsTransaction.Amount;
		 Currency CashOut = 0;
		 int ExpiryMonth = -1;
		 int ExpiryYear = -1;

		 if (Proceed)
		 {
			do
			{
			   try
			   {
				  if (RetrieveCard)
				  {
					 TMagneticCardTrackData TrackData;
					 TEftposSyncroPinPadCTRL MyControl;
					 MyControl.Initialise();
					 TransType = TransactionType_PURCHASE;

					 enum eCardResult
					 {
						eCardAborted, eCardAccepted, eCardProcessing
					 }CardResult;

					 CardResult = eCardProcessing;
					 do
					 {
						if (!MyControl.RetrieveCard(TrackData))
						{
						   Proceed = false;
						   if (MessageBox("AA Rewards Swipe Card Failed\r" + MyControl.ErrorMessage + "\rDo you wish to try again?\r",
								 "AA Rewards Failed Try again?", MB_YESNO + MB_ICONQUESTION) == IDYES)
						   {
							  CardResult = eCardProcessing;
						   }
						   else
						   {
							  CardResult = eCardAborted;
						   }
						}
						else
						{
						   Proceed = true;
						   CardResult = eCardAccepted;
						}
					 }
					 while (CardResult == eCardProcessing);
					 PaymentTransaction.RewardsTransaction.CardNumber = TrackData.Track2;
				  }

				  if (Proceed)
				  {
					 EftPos->SetTransactionEvent(PaymentTransaction.RewardsTransaction.EFTPOSRef, TransType);
					 SaveRecoveryInfo(PaymentTransaction);
					 EftPos->ProcessEftPos(TransType, Pay, CashOut, PaymentTransaction.RewardsTransaction.EFTPOSRef, PanSource,
						PaymentTransaction.RewardsTransaction.CardNumber, ExpiryMonth, ExpiryYear);

					 if (EftPos->WaitOnEftPosEvent(PaymentTransaction.RewardsTransaction.EFTPOSRef))
					 {
						TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(PaymentTransaction.RewardsTransaction.EFTPOSRef);
						if (EftTrans != NULL)
						{
						   if (EftTrans->Result != eAccepted)
						   {
							  MessageBox(EftTrans->ResultText, "EFTPOS Response", MB_OK + MB_ICONINFORMATION);
						   }
						   Result = EftTrans->Result;
						}
						else
						{
						   Result = eFailed;
						}
					 }
					 else
					 {
						Result = eFailed;
					 }
				  }
				  else
				  {
					 Result = eFailed;
				  }
			   }
			   __finally
			   {
				  EftPos->DelTransactionEvent(PaymentTransaction.RewardsTransaction.EFTPOSRef);
			   }

			   if (Result != eAccepted && MessageBox("AA Rewards Transation Failed\rDo you wish to try again?.\r",
					 "AA Rewards Failed Try again?", MB_YESNO + MB_ICONQUESTION) == IDYES)
			   {
				  Result = eProcessing;
			   }
			}
			while (Result == eProcessing);
		 }

		 if (Result != eAccepted)
		 {
			PaymentTransaction.RewardsTransaction.CardNumber = "";
			PaymentTransaction.RewardsTransaction.EFTPOSRef = "";
			PaymentTransaction.RewardsTransaction.Amount = 0;
		 }
	  }
	  catch(Exception & E)
	  {
		 TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  }
   }
   else if (TGlobalSettings::Instance().EnableAARewardsReminder)
   {
	  MessageBox("Reminder.\rDo they have an AA Rewards Card?\r", "AA Rewards?", MB_OK + MB_ICONQUESTION);
   }*/
}
