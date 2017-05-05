#include <vcl.h>
#pragma hdrstop

#ifdef  PalmMate
#include "Palm.h"
#endif
#include <Sysutils.hpp>

#include "ManagerLoyaltyVoucher.h"
#include "SelectDish.h"
#include "Requests.h"
#include "ListPaymentSystem.h"
#include "Printing.h"
#include "PrintOut.h"
#include "AddTab.h"
#include "CSV.h"
#include "DBSaleTimes.h"
#include "Invoice.h"
#include "MMMessageBox.h"
#include "Eftpos.h"
#include "EftPosDialogs.h"
#include "Paytype.h"
#include "CardSwipe.h"
#include "MMTouchKeyboard.h"
#include "MMTouchNumpad.h"
#include "EftChequeVerify.h"
#include "ManagerVariable.h"
#include "TableManager.h"
#include "Rooms.h"
#include "SplitPayment.h"
#include "ReceiptManager.h"
#include "PhoenixHotelSystem.h"
#include "FreebieManager.h"
#include "ManagerPatron.h"
#include "Membership.h"
#include "IMManager.h"
#include "OrderUtils.h"
#include "Comms.h"
#include "ListSecurityRefContainer.h"
#include "ReqPrintJob.h"
#include "DBThirdPartyCodes.h"
#include "DBTab.h"
#include "DBOrder.h"
#include "DBSecurity.h"
#include "ItemSizeCategory.h"
#include "DBTables.h"
#include "ManagerDiscount.h"
#include "DBWebUtil.h"
#include <vector>
#include "IBillCalculator.h"
#include "MallExportUpdateAdaptor.h"
#include "PaymentTypeGroupsManager.h"
#include "MallExportManager.h"
#include "MallExportHourlyUpdate.h"
#include "MallExportTransactionUpdate.h"
#include "MallExportDefines.h"
#include "MallExportOtherDetailsUpdate.h"
#include "DBTierLevel.h"
#include "ManagerDelayedPayment.h"
#include "DrinkCommandManager.h"
#include "InitializeDCSession.h"
#include "MallExportRegenerateReport.h"
#include "LoyaltyMateUtilities.h"
#include "ReceiptUtility.h"
#include "StringTools.h"
#include "PointsRulesSetUtils.h"
#include "MallFactory.h"
#include "ManagerPanasonic.h"
#include "WalletPaymentsInterface.h"


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
	EftPosEnabled = true;
	LastReceipt = NULL;
    isSCDOrPWDApplied = false;
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
			//Payment.Properties = IBInternalQuery->FieldByName("PROPERTIES")->AsInteger;
            GetPaymentAttributes(DBTransaction,PaymentKey,Payment);
            GetPaymentWalletAttributes(DBTransaction,PaymentKey,Payment);
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
			Payment.Export = IBInternalQuery->FieldByName("INVOICE_EXPORT")->AsInteger;
			Payment.VoucherMerchantID = IBInternalQuery->FieldByName("VOUCHER_MERCHANT_ID")->AsString;
			Payment.PaymentThirdPartyID = TDBThirdPartyCodes::GetThirdPartyCodeByKey(DBTransaction,
			IBInternalQuery->FieldByName("THIRDPARTYCODES_KEY")->AsInteger);
			Payment.UniVoucherUser	=	IBInternalQuery->FieldByName("VOUCHER_USER")->AsString;
			Payment.UniVoucherPass	=	IBInternalQuery->FieldByName("VOUCHER_PASS")->AsString;
			Payment.CVSReadLocation	=	IBInternalQuery->FieldByName("CSV_READ_LOCATION")->AsString;
			Payment.CVSWriteLocation=	IBInternalQuery->FieldByName("CSV_WRITE_LOCATION")->AsString;
            Payment.TabKey =	IBInternalQuery->FieldByName("TabKey")->AsInteger;
            Payment.GLCode  =   IBInternalQuery->FieldByName("GL_CODE")->AsString;
            Payment.AutoPopulateBlindBalance =  IBInternalQuery->FieldByName("IS_AUTO_POPULATE_BLIND_BALANCE")->AsString == "T" ? true : false;
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

void TListPaymentSystem::GetPaymentAttributes(Database::TDBTransaction &DBTransaction,int PaymentKey,TPayment &Payment)
{
   TIBSQL *AttributeQuery    = DBTransaction.Query(DBTransaction.AddQuery());
   AttributeQuery->SQL->Text =  "SELECT * FROM PAYMENT_ATTRIBUTES WHERE PAYMENT_KEY = :PAYMENT_KEY ";
   AttributeQuery->ParamByName("PAYMENT_KEY")->AsInteger = PaymentKey;
   AttributeQuery->ExecQuery();
   for (; !AttributeQuery->Eof;)
   {
     Payment.SetPaymentAttribute(AttributeQuery->FieldByName("ATTRIBUTE_VALUE")->AsInteger);
     AttributeQuery->Next();
   }
}

void TListPaymentSystem::SetPaymentAttributes(Database::TDBTransaction &DBTransaction,int PaymentKey,TPayment &Payment)
{
   DeletePaymentAttribute(DBTransaction,PaymentKey);
   for(std::set<int>::iterator it = Payment.Properties.begin() ; it != Payment.Properties.end() ;advance(it,1))
   {
        int attribute = *it;
        SetPaymentAttribute(DBTransaction,PaymentKey,attribute);
   }
}

void TListPaymentSystem::SetPaymentAttribute(Database::TDBTransaction &DBTransaction,int PaymentKey,ePaymentAttribute inPaymentAttribute)
{
    TIBSQL *AttributeQuery    = DBTransaction.Query(DBTransaction.AddQuery());
    AttributeQuery->SQL->Text = "INSERT INTO PAYMENT_ATTRIBUTES (PAYMENT_KEY, ATTRIBUTE_VALUE) "
                                "VALUES (:PAYMENT_KEY, :ATTRIBUTE_VALUE)";
    AttributeQuery->ParamByName("PAYMENT_KEY")->AsInteger = PaymentKey;
    AttributeQuery->ParamByName("ATTRIBUTE_VALUE")->AsInteger = inPaymentAttribute;
    AttributeQuery->ExecQuery();
}

void TListPaymentSystem::DeletePaymentAttribute(Database::TDBTransaction &DBTransaction,int PaymentKey)
{
   TIBSQL *AttributeQuery    = DBTransaction.Query(DBTransaction.AddQuery());
   AttributeQuery->SQL->Text =  "DELETE FROM PAYMENT_ATTRIBUTES WHERE PAYMENT_KEY = :PAYMENT_KEY ";
   AttributeQuery->ParamByName("PAYMENT_KEY")->AsInteger = PaymentKey;
   AttributeQuery->ExecQuery();
}

void TListPaymentSystem::DeletePaymentAttribute(Database::TDBTransaction &DBTransaction,int PaymentKey,ePaymentAttribute inPaymentAttribute)
{
   TIBSQL *AttributeQuery    = DBTransaction.Query(DBTransaction.AddQuery());
   AttributeQuery->SQL->Text =  "DELETE FROM PAYMENT_ATTRIBUTES WHERE PAYMENT_KEY = :PAYMENT_KEY AND ATTRIBUTE_VALUE = :ATTRIBUTE_VALUE ";
   AttributeQuery->ParamByName("PAYMENT_KEY")->AsInteger = PaymentKey;
   AttributeQuery->ParamByName("ATTRIBUTE_VALUE")->AsInteger = inPaymentAttribute;
   AttributeQuery->ExecQuery();
}

void TListPaymentSystem::GetPaymentWalletAttributes(Database::TDBTransaction &DBTransaction,int PaymentKey,TPayment &Payment)
{
   TIBSQL *AttributeQuery    = DBTransaction.Query(DBTransaction.AddQuery());
   AttributeQuery->SQL->Text =  "SELECT * FROM PAYMENT_WALLET_ATTRIBUTES WHERE PAYMENT_KEY = :PAYMENT_KEY ";
   AttributeQuery->ParamByName("PAYMENT_KEY")->AsInteger = PaymentKey;
   AttributeQuery->ExecQuery();
   if (!AttributeQuery->Eof)
   {
        Payment.WalletType = AttributeQuery->FieldByName("WALLET_TYPE")->AsInteger;
        Payment.MerchentId = AttributeQuery->FieldByName("MERCHENT_ID")->AsString;
        Payment.TerminalId = AttributeQuery->FieldByName("TERMINAL_ID")->AsString;
        Payment.WalletUserName = AttributeQuery->FieldByName("USER_NAME")->AsString;
        Payment.WalletPassword = AttributeQuery->FieldByName("WALLET_PASSWORD")->AsString;
        Payment.WalletSecurityToken = AttributeQuery->FieldByName("SECURITY_TOKEN")->AsString;
   }
}

void TListPaymentSystem::SetPaymentWalletAttributes(Database::TDBTransaction &DBTransaction,int PaymentKey,TPayment &Payment)
{
   DeletePaymentWalletAttribute(DBTransaction,PaymentKey);
   TIBSQL *AttributeQuery    = DBTransaction.Query(DBTransaction.AddQuery());
   AttributeQuery->SQL->Text = "INSERT INTO PAYMENT_WALLET_ATTRIBUTES (PAYMENT_KEY,WALLET_TYPE, MERCHENT_ID,TERMINAL_ID,USER_NAME,WALLET_PASSWORD,SECURITY_TOKEN) "
                                "VALUES (:PAYMENT_KEY,:WALLET_TYPE,:MERCHENT_ID,:TERMINAL_ID,:USER_NAME,:WALLET_PASSWORD,:SECURITY_TOKEN)";
   AttributeQuery->ParamByName("PAYMENT_KEY")->AsInteger = PaymentKey;
   AttributeQuery->ParamByName("WALLET_TYPE")->AsInteger = Payment.WalletType;
   AttributeQuery->ParamByName("MERCHENT_ID")->AsString = Payment.MerchentId;
   AttributeQuery->ParamByName("TERMINAL_ID")->AsString = Payment.TerminalId;
   AttributeQuery->ParamByName("USER_NAME")->AsString = Payment.WalletUserName;
   AttributeQuery->ParamByName("WALLET_PASSWORD")->AsString = Payment.WalletPassword;
   AttributeQuery->ParamByName("SECURITY_TOKEN")->AsString = Payment.WalletSecurityToken;
   AttributeQuery->ExecQuery();
}

void TListPaymentSystem::DeletePaymentWalletAttribute(Database::TDBTransaction &DBTransaction,int PaymentKey)
{
   TIBSQL *AttributeQuery    = DBTransaction.Query(DBTransaction.AddQuery());
   AttributeQuery->SQL->Text =  "DELETE FROM PAYMENT_WALLET_ATTRIBUTES WHERE PAYMENT_KEY = :PAYMENT_KEY ";
   AttributeQuery->ParamByName("PAYMENT_KEY")->AsInteger = PaymentKey;
   AttributeQuery->ExecQuery();
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
            IBInternalQuery->SQL->Text =  "SELECT PAYMENT_KEY FROM PAYMENTTYPES WHERE PAYMENT_KEY <> :PAYMENT_KEY "
                                          " AND (UPPER(PAYMENT_NAME) = :PAYMENT_NAME "
                                          " OR PAYMENT_NAME = :MODIFIEDPAYMENTNAME ) ";
            IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = Payment.Name.UpperCase();
            IBInternalQuery->ParamByName("MODIFIEDPAYMENTNAME")->AsString = TStringTools::Instance()->UpperCaseWithNoSpace(Payment.Name);
            IBInternalQuery->ParamByName("PAYMENT_KEY")->AsInteger = PaymentKey;
            IBInternalQuery->ExecQuery();
            if(!IBInternalQuery->Eof)
            {
               throw new Exception("A Payment type of with name "+ Payment.Name +" already exists.\rPlease enter a different Payment name.");
            }

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = " UPDATE PAYMENTTYPES " " SET " " PAYMENT_NAME = :PAYMENT_NAME, " " PROPERTIES = :PROPERTIES, "
			" EXCHANGE_RATE = :EXCHANGE_RATE, " " COLOUR = :COLOUR, " " DISPLAY_ORDER = :DISPLAY_ORDER, "
			" PERCENT_ADJUST = :PERCENT_ADJUST, " " AMOUNT_ADJUST = :AMOUNT_ADJUST, " " ADJUST_REASON = :ADJUST_REASON, "
			" ROUNDTO = :ROUNDTO, " " GROUP_NUMBER = :GROUP_NUMBER, " " THIRDPARTYCODES_KEY = :THIRDPARTYCODES_KEY, "
			" TAX_RATE = :TAX_RATE, " " DEST_IP = :DEST_IP, " " DEST_PORT = :DEST_PORT, " " PRE_VOUCHER_CODE = :PRE_VOUCHER_CODE, "
			" VOUCHER_MERCHANT_ID = :VOUCHER_MERCHANT_ID, INVOICE_EXPORT = :INVOICE_EXPORT, VOUCHER_URL = :VOUCHER_URL, "
			" VOUCHER_USER = :VOUCHER_USER, VOUCHER_PASS = :VOUCHER_PASS, CSV_READ_LOCATION = :CSV_READ_LOCATION, "
			" CSV_WRITE_LOCATION = :CSV_WRITE_LOCATION ,TABKEY = :TABKEY, GL_CODE = :GL_CODE, IS_AUTO_POPULATE_BLIND_BALANCE = :IS_AUTO_POPULATE_BLIND_BALANCE "
            " WHERE  PAYMENT_KEY = :PAYMENT_KEY  " ;
            if(Payment.GetPaymentAttribute(ePayTypeElectronicTransaction))
            {
               IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = TStringTools::Instance()->UpperCaseWithNoSpace(Payment.Name);
            }
            else
            {
			  IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = Payment.Name;
            }
			IBInternalQuery->ParamByName("PROPERTIES")->AsString = Payment.GetPropertyString();
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
			IBInternalQuery->ParamByName("VOUCHER_MERCHANT_ID")->AsString = Payment.VoucherMerchantID;
			IBInternalQuery->ParamByName("INVOICE_EXPORT")->AsInteger = Payment.Export;
			IBInternalQuery->ParamByName("VOUCHER_USER")->AsString = Payment.UniVoucherUser;
			IBInternalQuery->ParamByName("VOUCHER_PASS")->AsString = Payment.UniVoucherPass;
			IBInternalQuery->ParamByName("CSV_READ_LOCATION")->AsString = Payment.CVSReadLocation;
			IBInternalQuery->ParamByName("CSV_WRITE_LOCATION")->AsString = Payment.CVSWriteLocation;
            IBInternalQuery->ParamByName("TABKEY")->AsInteger = Payment.TabKey;
            IBInternalQuery->ParamByName("GL_CODE")->AsString = Payment.GLCode;
			IBInternalQuery->ParamByName("TAX_RATE")->AsCurrency = Payment.TaxRate;
            IBInternalQuery->ParamByName("IS_AUTO_POPULATE_BLIND_BALANCE")->AsString = Payment.AutoPopulateBlindBalance == true ? "T" : "F";

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
            SetPaymentAttributes(DBTransaction,PaymentKey,Payment);
            SetPaymentWalletAttributes(DBTransaction,PaymentKey,Payment);
		}
		else
		{
            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text =  "SELECT PAYMENT_KEY FROM PAYMENTTYPES WHERE UPPER(PAYMENT_NAME) = :PAYMENT_NAME "
                                           " OR PAYMENT_NAME = :MODIFIEDPAYMENTNAME ";
            IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = Payment.Name.UpperCase();
            IBInternalQuery->ParamByName("MODIFIEDPAYMENTNAME")->AsString = TStringTools::Instance()->UpperCaseWithNoSpace(Payment.Name);
            IBInternalQuery->ExecQuery();
            if(!IBInternalQuery->Eof)
            {
               throw new Exception("A Payment type of with name "+ Payment.Name +" already exists.\rPlease enter a different Payment name.");
            }
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_PAYMENTTYPES, 1) FROM RDB$DATABASE";
			IBInternalQuery->ExecQuery();
			PaymentKey = IBInternalQuery->Fields[0]->AsInteger;

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
			"INSERT INTO PAYMENTTYPES (" "PAYMENT_KEY, " "PAYMENT_NAME, " "PROPERTIES, " "EXCHANGE_RATE, " "COLOUR, "
			"DISPLAY_ORDER, " "PERCENT_ADJUST, " "AMOUNT_ADJUST, " "ROUNDTO, " "ADJUST_REASON, " "GROUP_NUMBER, " "THIRDPARTYCODES_KEY, "
			"DEST_IP," "DEST_PORT," "TAX_RATE," "PRE_VOUCHER_CODE," "VOUCHER_MERCHANT_ID," "INVOICE_EXPORT,VOUCHER_URL,VOUCHER_USER, "
			"VOUCHER_PASS," "CSV_READ_LOCATION," "CSV_WRITE_LOCATION," "TABKEY," "GL_CODE, " "IS_AUTO_POPULATE_BLIND_BALANCE " ") " "VALUES (" ":PAYMENT_KEY, " ":PAYMENT_NAME, "
			":PROPERTIES, " ":EXCHANGE_RATE, " ":COLOUR, " ":DISPLAY_ORDER, " ":PERCENT_ADJUST, " ":AMOUNT_ADJUST, " ":ROUNDTO, "
			":ADJUST_REASON, " ":GROUP_NUMBER, " ":THIRDPARTYCODES_KEY, " ":DEST_IP," ":DEST_PORT," ":TAX_RATE," ":PRE_VOUCHER_CODE,"
			":VOUCHER_MERCHANT_ID, " ":INVOICE_EXPORT,:VOUCHER_URL,:VOUCHER_USER,:VOUCHER_PASS,"
			":CSV_READ_LOCATION,:CSV_WRITE_LOCATION,:TABKEY,:GL_CODE, :IS_AUTO_POPULATE_BLIND_BALANCE  ) ";

			IBInternalQuery->ParamByName("PAYMENT_KEY")->AsInteger = PaymentKey;
            if(Payment.GetPaymentAttribute(ePayTypeElectronicTransaction))
            {
               IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = TStringTools::Instance()->UpperCaseWithNoSpace(Payment.Name);
            }
            else
            {
			  IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = Payment.Name;
            }
			IBInternalQuery->ParamByName("PROPERTIES")->AsString = Payment.GetPropertyString();
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
			IBInternalQuery->ParamByName("VOUCHER_MERCHANT_ID")->AsString = Payment.VoucherMerchantID;
			IBInternalQuery->ParamByName("INVOICE_EXPORT")->AsInteger = Payment.Export;
 			IBInternalQuery->ParamByName("VOUCHER_USER")->AsString = Payment.UniVoucherUser.SubString(1,50);
			IBInternalQuery->ParamByName("VOUCHER_PASS")->AsString = Payment.UniVoucherPass.SubString(1,20);
			IBInternalQuery->ParamByName("CSV_READ_LOCATION")->AsString = Payment.CVSReadLocation.SubString(1,255);
			IBInternalQuery->ParamByName("CSV_WRITE_LOCATION")->AsString = Payment.CVSWriteLocation.SubString(1,255);
            IBInternalQuery->ParamByName("TABKEY")->AsInteger = Payment.TabKey;
            IBInternalQuery->ParamByName("GL_CODE")->AsString = Payment.GLCode;
            IBInternalQuery->ParamByName("IS_AUTO_POPULATE_BLIND_BALANCE")->AsString = Payment.AutoPopulateBlindBalance == true ? "T" : "F";
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
            SetPaymentAttributes(DBTransaction,PaymentKey,Payment);
            SetPaymentWalletAttributes(DBTransaction,PaymentKey,Payment);
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
    TIBSQL *IBInternalQuery = PaymentTransaction.DBTransaction.Query(PaymentTransaction.DBTransaction.AddQuery());
	int CurrentDisplayOrder = 0;
	if (PaymentTransaction.Membership.Member.ContactKey != 0)
	{
          LoadMemberPaymentTypes(PaymentTransaction);
	}
    if(TGlobalSettings::Instance().IsClippIntegrationEnabled)
    {
        LoadClippPaymentTypes(PaymentTransaction);
    }

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = " SELECT * FROM PAYMENTTYPES ORDER BY PAYMENTTYPES.DISPLAY_ORDER";
	IBInternalQuery->ExecQuery();
	for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	{
		TPayment *NewPayment = new TPayment;
		NewPayment->Name = IBInternalQuery->FieldByName("PAYMENT_NAME")->AsString;
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
		NewPayment->Export = IBInternalQuery->FieldByName("INVOICE_EXPORT")->AsInteger;
		NewPayment->VoucherMerchantID = IBInternalQuery->FieldByName("VOUCHER_MERCHANT_ID")->AsString;
		NewPayment->PaymentThirdPartyID = TDBThirdPartyCodes::GetThirdPartyCodeByKey(PaymentTransaction.DBTransaction,
		IBInternalQuery->FieldByName("THIRDPARTYCODES_KEY")->AsInteger);
		NewPayment->UniVoucherUser	=	IBInternalQuery->FieldByName("VOUCHER_USER")->AsString;
		NewPayment->UniVoucherPass	=	IBInternalQuery->FieldByName("VOUCHER_PASS")->AsString;
		NewPayment->CVSReadLocation	=	IBInternalQuery->FieldByName("CSV_READ_LOCATION")->AsString;
		NewPayment->CVSWriteLocation	=	IBInternalQuery->FieldByName("CSV_WRITE_LOCATION")->AsString;
        NewPayment->TabKey  =	IBInternalQuery->FieldByName("TabKey")->AsInteger;
        NewPayment->GLCode  =   IBInternalQuery->FieldByName("GL_CODE")->AsString;
        NewPayment->AutoPopulateBlindBalance = IBInternalQuery->FieldByName("IS_AUTO_POPULATE_BLIND_BALANCE")->AsString == "T" ? true : false;
		CurrentDisplayOrder = NewPayment->DisplayOrder;
        //NewPayment->Properties = IBInternalQuery->FieldByName("PROPERTIES")->AsInteger;
        GetPaymentAttributes(PaymentTransaction.DBTransaction,IBInternalQuery->FieldByName("PAYMENT_KEY")->AsInteger,*NewPayment);
        GetPaymentWalletAttributes(PaymentTransaction.DBTransaction,IBInternalQuery->FieldByName("PAYMENT_KEY")->AsInteger,*NewPayment);
	    loadPaymentTypeGroupsForPaymentType(IBInternalQuery->FieldByName("PAYMENT_KEY")->AsInteger,*NewPayment );
	    PaymentTransaction.PaymentAdd(NewPayment);
    }



    TPayment *CashPayment = PaymentTransaction.PaymentFind(CASH);
    if (CashPayment == NULL)
    {
        TPayment *NewPayment = new TPayment;
        NewPayment->Name = CASH;
        NewPayment->SetPaymentAttribute(ePayTypeOpensCashDrawer,true);
        NewPayment->SetPaymentAttribute(ePayTypeCash,true);
        NewPayment->DisplayOrder = -1;
        NewPayment->Colour = clGreen;
        // load up the default cash payment group
        NewPayment->SetAssignedGroups(getDefaultCashPaymentGroup());
        PaymentTransaction.PaymentAdd(NewPayment);
    }

	PaymentTransaction.ProcessPoints();
	if (PaymentTransaction.Type == eTransCreditPurchase)
	{
		for (std::map <long, TTabCredit> ::iterator itTabCredit = PaymentTransaction.TabCredit.begin();
		itTabCredit != PaymentTransaction.TabCredit.end(); advance(itTabCredit, 1))
		{
			TPayment *NewPayment = new TPayment;
			NewPayment->Name = TDBTab::GetTabName(PaymentTransaction.DBTransaction, itTabCredit->first).SubString(1, 15);
			NewPayment->NameOveride = CREDIT;
			NewPayment->SetPaymentAttribute(ePayTypeCredit,true);
			NewPayment->DisplayOrder = -2;
			NewPayment->Colour = clRed;
			NewPayment->SetPay(itTabCredit->second.CreditRedeemed);
			NewPayment->TabCreditKey = itTabCredit->first;
			NewPayment->GroupNumber = itTabCredit->second.PaymentGroupNumber;
			NewPayment->PaymentThirdPartyID = "800100"; // Credit Payment Type for Summa BI
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
				NewPayment->SetPaymentAttribute(ePayTypeCredit,true);
				NewPayment->DisplayOrder = -2;
				NewPayment->Colour = clRed;
				NewPayment->SetPay(Credit.CreditRedeemed);
				NewPayment->TabCreditKey = *itTabs;
				NewPayment->GroupNumber = Credit.PaymentGroupNumber;
				NewPayment->PaymentThirdPartyID = "800100"; // Credit Payment Type for Summa BI
				NewPayment->Visible = true;
				PaymentTransaction.PaymentAdd(NewPayment);
			}
		}
	}
	PaymentTransaction.Money.Recalc(PaymentTransaction);
}

void TListPaymentSystem::LoadMemberPaymentTypes(TPaymentTransaction &PaymentTransaction)
{
    TPayment *NewPayment = new TPayment;
    NewPayment->Name = PaymentTransaction.Membership.Member.Name + "'s Points";
    NewPayment->SysNameOveride = "Points";
    NewPayment->SetPaymentAttribute(ePayTypePoints);
    NewPayment->DisplayOrder = 1;
    NewPayment->GroupNumber = TGlobalSettings::Instance().PointsPaymentGroupNumber;
    NewPayment->Colour = clTeal;
    NewPayment->PaymentThirdPartyID = "10007242";
    PaymentTransaction.PaymentAdd(NewPayment);
    if(TGlobalSettings::Instance().UseTierLevels && TGlobalSettings::Instance().AllowPointPaymentByWeight)
    {
        TPayment *NewPayment = new TPayment;
        NewPayment->Name = PaymentTransaction.Membership.Member.Name + "'s Grams";
        NewPayment->SysNameOveride = "Points";
        NewPayment->SetPaymentAttribute(ePayTypePoints);
        NewPayment->DisplayOrder = 1;
        NewPayment->GroupNumber = TGlobalSettings::Instance().PointsPaymentGroupNumber;
        NewPayment->Colour = clTeal;
        NewPayment->PaymentThirdPartyID = "10007242";
        PaymentTransaction.PaymentAdd(NewPayment);
    }

    if (PaymentTransaction.Membership.Member.ContactKey != 0 && !TGlobalSettings::Instance().UseTierLevels)
    {
        TPayment *NewPayment = new TPayment;
        NewPayment->Name = "Refund Points";
        NewPayment->SysNameOveride = "Points";
        NewPayment->SetPaymentAttribute(ePayTypePoints);
        NewPayment->RefundPoints = true;
        NewPayment->DisplayOrder = 1;
        NewPayment->GroupNumber = TGlobalSettings::Instance().PointsPaymentGroupNumber;
        NewPayment->Colour = clTeal;
        NewPayment->PaymentThirdPartyID = "10007242";
        PaymentTransaction.PaymentAdd(NewPayment);
    }

    if(TGlobalSettings::Instance().MembershipType == MembershipTypeExternal)
    {
        TPayment *NewPayment = new TPayment;
        NewPayment->Name = "Comp";
        NewPayment->SysNameOveride = "Points";
        NewPayment->SetPaymentAttribute(ePayTypePoints);
        NewPayment->DisplayOrder = 1;
        NewPayment->GroupNumber = TGlobalSettings::Instance().PointsPaymentGroupNumber;
        NewPayment->Colour = clTeal;
        NewPayment->PaymentThirdPartyID = "10007242";
        PaymentTransaction.PaymentAdd(NewPayment);

        NewPayment = new TPayment;
        NewPayment->Name = "Dining";
        NewPayment->SysNameOveride = "Dining";
        NewPayment->SetPaymentAttribute(ePayTypePoints);
        NewPayment->DisplayOrder = 1;
        NewPayment->GroupNumber = TGlobalSettings::Instance().PointsPaymentGroupNumber;
        NewPayment->Colour = clTeal;
        NewPayment->PaymentThirdPartyID = "10007242";
        PaymentTransaction.PaymentAdd(NewPayment);
    }


}

void TListPaymentSystem::PaymentsLoadTypes(Database::TDBTransaction &DBTransaction,std::vector<TPayment> &Payments)
{
    bool CashFound = false;
    //if(!TGlobalSettings::Instance().PointOnly) //mm 4775
    //{
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	    DBTransaction.RegisterQuery(IBInternalQuery);
    	IBInternalQuery->Close();
	    IBInternalQuery->SQL->Text = " SELECT * FROM PAYMENTTYPES ORDER BY PAYMENTTYPES.DISPLAY_ORDER";
    	IBInternalQuery->ExecQuery();
	    for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
    	{
	    	TPayment NewPayment;
		    NewPayment.Name = IBInternalQuery->FieldByName("PAYMENT_NAME")->AsString;
	    	NewPayment.DisplayOrder = IBInternalQuery->FieldByName("DISPLAY_ORDER")->AsInteger;
		    NewPayment.Colour = IBInternalQuery->FieldByName("COLOUR")->AsInteger;
    		NewPayment.AmountAdjust = IBInternalQuery->FieldByName("AMOUNT_ADJUST")->AsFloat;
	    	NewPayment.PercentAdjust = IBInternalQuery->FieldByName("PERCENT_ADJUST")->AsFloat;
		    NewPayment.RoundTo = IBInternalQuery->FieldByName("ROUNDTO")->AsCurrency;
    		NewPayment.TaxRate = IBInternalQuery->FieldByName("TAX_RATE")->AsFloat;
	    	NewPayment.AdjustmentReason = IBInternalQuery->FieldByName("ADJUST_REASON")->AsString;
		    NewPayment.GroupNumber = IBInternalQuery->FieldByName("GROUP_NUMBER")->AsInteger;
    		NewPayment.SecondaryPMSIPAddress = IBInternalQuery->FieldByName("DEST_IP")->AsString;
	    	NewPayment.SecondaryPMSPortNumber = IBInternalQuery->FieldByName("DEST_PORT")->AsInteger;
		    NewPayment.Export = IBInternalQuery->FieldByName("INVOICE_EXPORT")->AsInteger;
	    	NewPayment.VoucherMerchantID = IBInternalQuery->FieldByName("VOUCHER_MERCHANT_ID")->AsString;
		    NewPayment.PaymentThirdPartyID = TDBThirdPartyCodes::GetThirdPartyCodeByKey(DBTransaction,
    		IBInternalQuery->FieldByName("THIRDPARTYCODES_KEY")->AsInteger);
		    NewPayment.UniVoucherUser	=	IBInternalQuery->FieldByName("VOUCHER_USER")->AsString;
    		NewPayment.UniVoucherPass	=	IBInternalQuery->FieldByName("VOUCHER_PASS")->AsString;
	    	NewPayment.CVSReadLocation	=	IBInternalQuery->FieldByName("CSV_READ_LOCATION")->AsString;
		    NewPayment.CVSWriteLocation	=	IBInternalQuery->FieldByName("CSV_WRITE_LOCATION")->AsString;
            NewPayment.TabKey  =	IBInternalQuery->FieldByName("TabKey")->AsInteger;
            NewPayment.GLCode  =   IBInternalQuery->FieldByName("GL_CODE")->AsString;
            NewPayment.AutoPopulateBlindBalance = IBInternalQuery->FieldByName("IS_AUTO_POPULATE_BLIND_BALANCE")->AsString == "T" ? true : false;
            //NewPayment.Properties = IBInternalQuery->FieldByName("PROPERTIES")->AsInteger;
            GetPaymentAttributes(DBTransaction,IBInternalQuery->FieldByName("PAYMENT_KEY")->AsInteger,NewPayment);
    		Payments.push_back(NewPayment);
            if(NewPayment.Name == CASH)
              CashFound = true;
	    }


	    if(!CashFound)
    	{
	    	TPayment NewPayment;
		    NewPayment.Name = CASH;
    		NewPayment.SetPaymentAttribute(ePayTypeOpensCashDrawer);
	    	NewPayment.SetPaymentAttribute(ePayTypeCash);
  		    NewPayment.DisplayOrder = -1;
    		NewPayment.Colour = clGreen;
	    	Payments.push_back(NewPayment);
        }
}

void TListPaymentSystem::ReturnOrderedItemsToStock( TPaymentTransaction &transaction )
{
	TList &orders = *transaction.Orders;

	for (int i = 0; i < orders.Count; ++i) {
		TItemMinorComplete &m =
		*reinterpret_cast<TItemMinorComplete *>(orders[i]);
		m.ReturnToAvailability(
		std::fabs(m.GetQty()) * (1 + m.IsBeingReturnedToStock()));
	}
}

bool TListPaymentSystem::ProcessTransaction(TPaymentTransaction &PaymentTransaction, bool isRecovery, bool isClippPayment )
{
	PaymentComplete = false;

	try
	{
        CheckForCard(PaymentTransaction);

		Busy = true;
		PaymentAborted = false;
		RequestEFTPOSReceipt = false;
		bool reprintEftposReceipt = false;
		bool isRecovering = false;
		State = _createProcessingStateMessage();

		if( PaymentTransaction.Type == eTransEFTPOSRecovery || PaymentTransaction.Type == eTransRewardsRecovery )
		{
			reprintEftposReceipt = true;
			isRecovering = true;
		}

        if( ( PaymentTransaction.Type == eTransQuickSale ) && ( PaymentTransaction.SalesType == eTab ) )
        {
			Security->SecurityClear();
        }
        else if( PaymentTransaction.Type == eTransQuickSale )
        {
            initPaymentControlForms( PaymentTransaction, isRecovery );
            Security->SecurityClear();
        }
        else
        {
           initPaymentControlForms( PaymentTransaction, isRecovery );
        }

		if( PaymentTransaction.Type != eTransQuickSale
                && PaymentTransaction.Type != eTransEFTPOSRecovery
				&& PaymentTransaction.Type != eTransRewardsRecovery )
		{
			PaymentsReload(PaymentTransaction);
			Security->SecurityClear();
		}

		PreparePayments(PaymentTransaction);

		// Retrive this Receipts Security Ref.
		Security->SetSecurityRefNumber(TDBSecurity::GetNextSecurityRef(PaymentTransaction.DBTransaction));

		switch (PaymentTransaction.Type)
		{
		case eTransOrderSet:
			_processOrderSetTransaction( PaymentTransaction );
			break;
		case eTransSplitPayment:
			_processSplitPaymentTransaction( PaymentTransaction );
			break;
		case eTransPartialPayment:
			_processPartialPaymentTransaction( PaymentTransaction );
			break;
		case eTransQuickSale:
			_processQuickTransaction( PaymentTransaction );
			break;
		case eTransCreditPurchase:
			_processCreditTransaction( PaymentTransaction );
			break;
		case eTransEFTPOSRecovery:
			_processEftposRecoveryTransaction( PaymentTransaction ); 	// similar to eTransRewardsRecovery ?
			break;
		case eTransRewardsRecovery:
			_processRewardsRecoveryTransaction( PaymentTransaction ); 	// similar to eTransEftposRecovery ?
			break;
		default:
			break;
		}

		transactionRecovery.ClearRecoveryInfo();

		if ( reprintEftposReceipt )
		EftPos->ReprintReceipt();
        bool earnpoints = TGlobalSettings::Instance().SystemRules.Contains(eprEarnsPointsWhileRedeemingPoints);
        bool onlyearns = TGlobalSettings::Instance().SystemRules.Contains(eprOnlyEarnsPointsWhileRedeemingPoints);
		if (PaymentComplete)
		{
            if(TGlobalSettings::Instance().IsDrinkCommandEnabled)
             {
                PaymentTransaction.Membership.Member.Points.getPointsBalance();
                PaymentTransaction.Membership.Member.Points.getCurrentPointsPurchased();


                TPayment *paymentType =  PaymentTransaction.PaymentGet(0);
                BillPts =  PaymentTransaction.Money.TotalProduct;
                Pts = TDrinkCommandData::Instance().GetPoints(PaymentTransaction);                                         //Current points of member
                MemberNo = PaymentTransaction.Membership.Member.MembershipNumber;
                AnsiString Card_Id =  TDrinkCommandData::Instance().GetCardIdFromMemberNo(MemberNo);
                Points = PaymentTransaction.Money.TotalAdjustment;
                PointsBuy = CurrToStrF(Points,ffNumber,2);
                PointsBuy = PointsBuy * 100;
                if(/*(PaymentTransaction.Membership.Member.Points.getCurrentPointsRedeemed()>0)*/
                   PaymentTransaction.Money.TotalProduct > 0||
                  PaymentTransaction.Money.TotalProduct - PaymentTransaction.Membership.Member.Points.getCurrentPointsPurchased() != 0)
                {
                    UnicodeString serverPath = TGlobalSettings::Instance().DrinkCommandServerPath;
                    AnsiString path = AnsiString(serverPath);
                    int serverPort = TGlobalSettings::Instance().DrinkCommandServerPort;
                    std::auto_ptr<TDrinkCommandManager> dcCommand (new TDrinkCommandManager());
                    AnsiString session_uuid = TDrinkCommandData::Instance().CheckForOpenSession(Card_Id);
                    if (session_uuid.Length() != 0)
                    {
//                    if( (Pts >= BillPts))                                                                                //Closing session after consuming points
//                    {
                        UnicodeString serverPath = TGlobalSettings::Instance().DrinkCommandServerPath;
                        AnsiString path = AnsiString(serverPath);
                        int serverPort = TGlobalSettings::Instance().DrinkCommandServerPort;
                        std::auto_ptr<TDrinkCommandManager> dcCommand (new TDrinkCommandManager());
                        AnsiString session_uuid = TDrinkCommandData::Instance().CheckForOpenSession(Card_Id);
                        if (session_uuid.Length() != 0)
                        {
                            dcCommand->CloseSession(path , serverPort , session_uuid);
                        }
                    }
                }
                else
                {
                     TDrinkCommandData::Instance().UpdateTimeStampToNull(Card_Id);
                }
                // Case of complete refund closes the session

                if(((double(Points.Val) + double(Pts.Val) )== 0))
                {
                    UnicodeString serverPath = TGlobalSettings::Instance().DrinkCommandServerPath;
                    AnsiString path = AnsiString(serverPath);
                    int serverPort = TGlobalSettings::Instance().DrinkCommandServerPort;
                    std::auto_ptr<TDrinkCommandManager> dcCommand (new TDrinkCommandManager());
                    AnsiString session_uuid = TDrinkCommandData::Instance().CheckForOpenSession(Card_Id) ;
                    if (session_uuid.Length() != 0)
                    {
                        dcCommand->CloseSession(path , serverPort , session_uuid);
                    }
                }
                // Adds cash in case of session exists or make a new session because points are purchased
                if((PaymentTransaction.Membership.Member.Points.getCurrentPointsPurchased() > 0)
                   ||(PaymentTransaction.Membership.Member.Points.getCurrentPointsEarned() > 0 ))
                {

                   Currency morePoints = PaymentTransaction.Membership.Member.Points.getCurrentPointsPurchased() +
                                         PaymentTransaction.Membership.Member.Points.getCurrentPointsEarned();

                    AnsiString pts = CurrToStrF(morePoints,ffNumber,2);
                    pts = pts * 100;

                    UnicodeString serverPath = TGlobalSettings::Instance().DrinkCommandServerPath;
                    AnsiString path = AnsiString(serverPath);
                    int serverPort = TGlobalSettings::Instance().DrinkCommandServerPort;
                    std::auto_ptr<TDrinkCommandManager> dcCommand (new TDrinkCommandManager());
                    AnsiString session_uuid = TDrinkCommandData::Instance().CheckForOpenSession(Card_Id);
                    AnsiString session_id = session_uuid;
                    if (session_uuid.Length() != 0)
                    {
                        dcCommand->SendCashInfo(path , serverPort ,PointsBuy ,session_id);
                    }
                    else
                    {
                        int key = PaymentTransaction.Membership.Member.ContactKey;
                        AnsiString memberNo = PaymentTransaction.Membership.Member.MembershipNumber;
                        AnsiString points = PaymentTransaction.Membership.Member.Points.getPointsBalance();
                        points = points * 100;
                        int memPoints = atoi(points.c_str());
                        std::auto_ptr<TInitializeDCSession> im(
                                                                new TInitializeDCSession());
                        im->StartSession(memPoints,points,memberNo,key);
                        sessionStartedAlready = true;
                        if (PaymentTransaction.Membership.Member.Points.getCurrentPointsPurchased() ==
                            PaymentTransaction.Membership.Member.Points.getPointsBalance())
                         {
                            skipPaymentFormDeletion = true;
                         }
                   }
                }
            }
            UpdateCachedRewardPoints(PaymentTransaction);
            PerformPostTransactionOperations( PaymentTransaction );

            if(TGlobalSettings::Instance().MallIndex == FEDERALLANDMALL)
            {
                TMallExportOtherDetailsUpdate exportOtherDetailsUpdate;
                TExportUpdateResponse responseotherdetails = exportOtherDetailsUpdate.UpdateOtherDetailsExportTablesOnTransaction( &PaymentTransaction );
            }
                                                           //Current points of member
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
        if(!isClippPayment)
		{
            // in case of directly redirecting to Payment screen
            if(!skipPaymentFormDeletion)
            {
               resetPaymentControlForms();
            }
        }
		Reset(PaymentTransaction);
        if(!IsClippSale)
        {
		    TDeviceRealTerminal::Instance().ProcessingController.Pop();
        }
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

    if(TGlobalSettings::Instance().IsPanasonicIntegrationEnabled)
    {
        TManagerPanasonic::Instance()->TriggerTransactionSync();
    }
	return PaymentComplete;
}

bool TListPaymentSystem::CheckForCard(TPaymentTransaction &PaymentTransaction)
{
    bool retVal = true;
    try
    {
        if(PaymentTransaction.Membership.Applied())
         {
            if(TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->Enabled)
             {
                 if (TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->CardOk)
                  {
                      UnicodeString UUID = PaymentTransaction.Membership.Member.CloudUUID;
                      retVal = UUID.Compare(TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->LoadCardGUID());
                  }

             }
         }
     }
    catch(Exception & E)
	{
        retVal = false;
	}

    if(!retVal)
      throw Exception("Failed To Read Card. Either Re-insert card or remove member.");
    return retVal;
}

void TListPaymentSystem::PerformPostTransactionOperations( TPaymentTransaction &PaymentTransaction )
{
	if (PaymentTransaction.Type == eTransQuickSale && PaymentTransaction.SalesType == eTab)
    {
         ReceiptPrint(PaymentTransaction, RequestEFTPOSReceipt, false);
    }
    else
    {
        ReceiptPrint(PaymentTransaction, RequestEFTPOSReceipt, frmControlTransaction->UserOption == eCloseandPrint);
    }

	FormatSpendChit(PaymentTransaction);
	ProcessSecurity(PaymentTransaction);
	SaveToFileCSV(PaymentTransaction);
	RemoveTabs(PaymentTransaction); // Remove any empty Tabs.

	if (PaymentTransaction.Membership.Applied())
	{
		TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->TransactionClosed(
		PaymentTransaction.DBTransaction,
		PaymentTransaction.Membership.Member);

        if((PaymentTransaction.Membership.Member.Points.getPointsBalance() > 0)
          && TGlobalSettings::Instance().IsDrinkCommandEnabled)
        {
              int key = PaymentTransaction.Membership.Member.ContactKey;
              AnsiString memberNo = PaymentTransaction.Membership.Member.MembershipNumber;
              AnsiString points = PaymentTransaction.Membership.Member.Points.getPointsBalance();
              points = points * 100;
              int memPoints = atoi(points.c_str());
               if((PaymentTransaction.Membership.Member.Points.getCurrentPointsRedeemed()>0)||(!sessionStartedAlready))
               {
                    std::auto_ptr<TInitializeDCSession> im(
                                                            new TInitializeDCSession());
                    im->StartSession(memPoints,points,memberNo,key);
               }
        }
	}
}

void TListPaymentSystem::TransRetriveInvoiceResult(TPaymentTransaction &PaymentTransaction, TPayment *Payment)
{

	// Calculate the ratio of the payment to cauculate it proportion of GST content.
	double Ratio =  0;
	if(PaymentTransaction.Money.GrandTotal != 0)
	{
		Ratio =  Payment->GetPayTotal() /  PaymentTransaction.Money.GrandTotal;
	}

	Currency GSTContent = PaymentTransaction.Money.GrandTotalGSTContent * Ratio;
	// Create an invoice for this bill, mark it as Closed but not exported.
	int InvoiceKey = Invoice->CreateInvoice(PaymentTransaction.DBTransaction, Payment->InvoiceContact.ContactKey,
	Payment->GetPayTotal() - GSTContent, Payment->GetPayTotal(), "");


	Invoice->SetInvoiceClosed(PaymentTransaction.DBTransaction, InvoiceKey);

	PaymentTransaction.SalesType = eAccount;
	PaymentTransaction.InvoiceKey = InvoiceKey;
    if(!TManagerDelayedPayment::Instance().IsDelayedPayment(PaymentTransaction))
    {
      if(PaymentTransaction.InvoiceNumber == "" || PaymentTransaction.InvoiceNumber == "Undefined")
         PaymentTransaction.InvoiceNumber = Invoice->GetInvoiceNumber(PaymentTransaction.DBTransaction, InvoiceKey);
    }
   else
   {
       TItemComplete *Order = (TItemComplete*)PaymentTransaction.Orders->Items[0];
       PaymentTransaction.InvoiceNumber = Order->DelayedInvoiceNumber;
   }

	Payment->ReferenceNumber = PaymentTransaction.InvoiceNumber;

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
	else if (Payment->GetPaymentAttribute(ePayTypeIntegratedEFTPOS))
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
			if (Payment->GetPaymentAttribute(ePayTypeAllowMANPAN) && PaymentTransaction.Type != eTransEFTPOSRecovery)
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

			if (!Payment->GetPaymentAttribute(ePayTypeAllowReversal) && Payment->CreditTransaction)
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
				else if (Payment->GetPaymentAttribute(ePayTypeAllowReversal) && Payment->CreditTransaction)
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

					// inspect payment property and set the TransType if this allows tips. also check whether eftpos allows tips
					if(Payment->GetPaymentAttribute(ePayTypeAllowTips))
					{
						if(EftPos->AllowsTipsOnTransactions())
						{
							TransType = TransactionType_TIP;
						}
					}

					Pay = Payment->GetPayTendered();
					CashOut = Payment->GetCashOutTotal();
				}

				try
				{
					EftPos->SetTransactionEvent(Payment->ReferenceNumber, TransType);

					// set recovery information for current transaction
					transactionRecovery.SaveRecoveryInformation( PaymentTransaction, Security );

					EftPos->ProcessEftPos(TransType, Pay, CashOut, Payment->ReferenceNumber, PanSource, CardString, ExpiryMonth, ExpiryYear);

					if (EftPos->WaitOnEftPosEvent(Payment->ReferenceNumber))
					{
						TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(Payment->ReferenceNumber);
						if (EftTrans != NULL)
						{
							if (EftTrans->Result == eManualQuery)
							{

								std::auto_ptr <TfrmEftPos> frmEftPos(TfrmEftPos::Create <TfrmEftPos> (Screen->ActiveForm));
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
                                AnsiString cardtype = EftTrans->CardType;
								Payment->CardType = TStringTools::Instance()->UpperCaseWithNoSpace(cardtype); // set the card type returned from eftpos transaction for future reference (tips)
								Payment->EftposTransactionID = EftTrans->EftposTransactionID; // eftpos transaction id

                                if(EftTrans->FinalAmount != "")
                                {
                                   Currency FinalAmount = StrToCurr(EftTrans->FinalAmount);
                                   if(FinalAmount != (Pay + CashOut))
                                   {
                                       Payment->TipAmount = FinalAmount - (Pay + CashOut);
                                       Payment->SetPay(FinalAmount);
                                   }
                                }
							}

                            if (Payment->Result != eAccepted)
                            {
                                 Payment->Result = eFailed;
                                 transactionRecovery.ClearRecoveryInfo();
                                 AnsiString unhandledState = "HANDLE IS IN THE WRONG";
                                 if((EftTrans->ResultText.UpperCase().Pos(unhandledState) != 0)
                                    ||  EftTrans->TimeOut)
                                 {
                                    if(MessageBox("Transaction Cancelled/Timed-Out.", "EFTPOS Response",MB_RETRYCANCEL) == IDRETRY)
                                       TransRetriveElectronicResult(PaymentTransaction, Payment);
                                 }
                                 else
                                 {
                                    if(EftTrans->ResultText == "")
                                        EftTrans->ResultText = "Transaction Failed.";
                                    MessageBox(EftTrans->ResultText, "EFTPOS Response", MB_OK + MB_ICONINFORMATION);
                                 }
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
					if (Payment->Result != eAccepted || Payment->Result == eFailed)
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

	if (!TDeviceRealTerminal::Instance().BasePMS->Enabled)
	{
		Application->MessageBox(UnicodeString("Payment System is Configured to use the PMS Interface."
		" The PMS Interface Software has not been installed or enabled correctly." " Please contact your MenuMate support agent.").w_str
		(), UnicodeString("PMS Interface Configuration Error").w_str(), MB_OK + MB_ICONERROR);
		RetVal = false;
	}
	else
	{
		if (!TDeviceRealTerminal::Instance().BasePMS->ExportData(PaymentTransaction, TDeviceRealTerminal::Instance().User.ContactKey))
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
			AnsiString TransactionRef = TDeviceRealTerminal::Instance().BasePMS->GetLastTransactionRef();
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
	if (Payment->GetPaymentAttribute(ePayTypeChequeVerify))
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

            TfrmChequeVerfiy *frmChequeVerfiy = new TfrmChequeVerfiy(NULL);
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
            delete frmChequeVerfiy;
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
								std::auto_ptr <TfrmEftPos> frmEftPos(TfrmEftPos::Create <TfrmEftPos> (Screen->ActiveForm));
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
    MakePatronCountZero = false;
    if(TGlobalSettings::Instance().PromptPatronCountOnTableSales)
    {
        CheckPatronByOrderIdentification(PaymentTransaction);
    }
	long ArcBillKey = ArchiveBill(PaymentTransaction);
	ArchivePatronCount(PaymentTransaction, ArcBillKey);
	ArchiveReferences(PaymentTransaction, ArcBillKey);
	ArchivePoints(PaymentTransaction);
	SetLastVisit(PaymentTransaction);
	ArchiveOrder(PaymentTransaction, ArcBillKey);
	ArchiveRewards(PaymentTransaction, ArcBillKey);
	ArchiveWebOrders(PaymentTransaction, ArcBillKey);
    TDeviceRealTerminal::Instance().ManagerMembership->SyncBarcodeMemberDetailWithCloud(PaymentTransaction.Membership.Member);

    if(isSCDOrPWDApplied)
        PrepareSCDOrPWDCustomerDetails(PaymentTransaction, ArcBillKey);

    if(TGlobalSettings::Instance().mallInfo.MallId && PaymentTransaction.Orders->Count)
    {
        //Instantiation is happenning in a factory based on the active mall in database
        TMallExport* mall = TMallFactory::GetMallType();
        mall->PushToDatabase(PaymentTransaction, ArcBillKey);
        delete mall;
    }
}

void TListPaymentSystem::CheckPatronByOrderIdentification(TPaymentTransaction &PaymentTransaction)
{
    try
    {
        int identificationNumber = 0;

        if(PaymentTransaction.Orders->Count > 0)
        {
           TItemComplete *item = (TItemComplete*)(PaymentTransaction.Orders->Items[0]);
           identificationNumber = item->OrderIdentificationNo;
        }

		TIBSQL *IBInternalQueryFirst = PaymentTransaction.DBTransaction.Query(PaymentTransaction.DBTransaction.AddQuery());
		IBInternalQueryFirst->Close();
		IBInternalQueryFirst->SQL->Text =
			" SELECT "
			" PATRON_COUNT "
			" FROM "
			"  DAYARCBILL "
			" WHERE "
			"  ORDER_IDENTIFICATION_NUMBER = :ORDER_IDENTIFICATION_NUMBER "

            " UNION ALL "

			" SELECT "
			" PATRON_COUNT "
			" FROM "
			"  ARCBILL "
			" WHERE "
			"  ORDER_IDENTIFICATION_NUMBER = :ORDER_IDENTIFICATION_NUMBER ";
        IBInternalQueryFirst->ParamByName("ORDER_IDENTIFICATION_NUMBER")->AsInteger = identificationNumber;
        IBInternalQueryFirst->ExecQuery();

        if(IBInternalQueryFirst->RecordCount > 0)
        {
           MakePatronCountZero = true;
        }
        IBInternalQueryFirst->Close();
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}

void TListPaymentSystem::ArchiveWebOrders(TPaymentTransaction &PaymentTransaction, long ArcBillKey)
{
	try
	{
		TIBSQL *IBInternalQuery = PaymentTransaction.DBTransaction.Query(PaymentTransaction.DBTransaction.AddQuery());
		std::set<__int64> ProcessedWebOrders;
		for (int CurrentIndex = 0; CurrentIndex < PaymentTransaction.Orders->Count; CurrentIndex++)
		{
			TItemComplete *Order = (TItemComplete*)(PaymentTransaction.Orders->Items[CurrentIndex]);

			if(Order->WebKey != 0)
			{
				IBInternalQuery->Close();
				IBInternalQuery->SQL->Text =
				"INSERT INTO DAYARCWEB (WEBORDER_KEY,ARCBILL_KEY) "
				"VALUES (:WEBORDER_KEY, :ARCBILL_KEY) ";
				IBInternalQuery->ParamByName("WEBORDER_KEY")->AsInteger = Order->WebKey;
				IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = ArcBillKey;
				IBInternalQuery->ExecQuery();

				if (ProcessedWebOrders.find(Order->WebKey) == ProcessedWebOrders.end())
				{
					TDBWebUtil::SetWebOrderStatus(PaymentTransaction.DBTransaction,Order->WebKey,ewosBilled);
					ProcessedWebOrders.insert(Order->WebKey);
				}
			}
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}

void TListPaymentSystem::SetLastVisit(TPaymentTransaction &PaymentTransaction)
{
	//if (PaymentTransaction.Membership.Member.Valid())
	{
		TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->SetLastVisit(PaymentTransaction.DBTransaction,
		PaymentTransaction.Membership.Member);
	}
}

void TListPaymentSystem::UpdateCachedRewardPoints(TPaymentTransaction &PaymentTransaction)
{
  if(TGlobalSettings::Instance().LoyaltyMateEnabled)
   {
      TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->AvailableBDPoint -=
                (double)PaymentTransaction.Membership.Member.Points.getBirthDayRewardRedeemedPoints();
      TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->AvailableFVPoint  -=
                (double)PaymentTransaction.Membership.Member.Points.getFirstVisitRedeemedPoints();
   }
}

void TListPaymentSystem::ArchivePoints(TPaymentTransaction &PaymentTransaction)
{

    if(PaymentTransaction.Membership.Member.ContactKey)
    {
        TPointsStore PointsToArchive = PaymentTransaction.Membership.Member.Points.getExcludingSource(pasDatabase);
        for (TPointsStore::iterator ptrPointsTransaction = PointsToArchive.begin();
        ptrPointsTransaction != PointsToArchive.end(); ptrPointsTransaction++)
        {
            if(ptrPointsTransaction->second !=0)
            {
                TPointsTransaction Transaction;
                Transaction.Build(PaymentTransaction.Membership.Member.ContactKey,
                *ptrPointsTransaction,
                PaymentTransaction.Membership.Member.Points.ExportStatus,
                PaymentTransaction.InvoiceNumber,
                Now(),
                Now(),
                TGlobalSettings::Instance().RefundingItems);
                TDeviceRealTerminal::Instance().ManagerMembership->ArchivePoints(PaymentTransaction.DBTransaction,Transaction);
            }
        }
    }

}

void TListPaymentSystem::CalculateTierLevel(TPaymentTransaction &PaymentTransaction)
{
   if(PaymentTransaction.Membership.Member.ContactKey == 0)
     return;

	try
	{

		if(!TGlobalSettings::Instance().UseTierLevels || PaymentTransaction.Membership.Member.MemberType != 1 ||
				(TGlobalSettings::Instance().LoyaltyMateEnabled && TGlobalSettings::Instance().IsPOSOffline))
		{
           if(TGlobalSettings::Instance().LoyaltyMateEnabled && TGlobalSettings::Instance().IsPOSOffline)
           {
             MessageBox( "Unable to communicate with Cloud. Tier level will be updated on next visit.", "Message", MB_ICONINFORMATION + MB_OK);
           }
           else if(!TGlobalSettings::Instance().UseTierLevels)
           {
              double currentPoint = PaymentTransaction.Membership.Member.Points.getCurrentPointsEarned();
              if(TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->CurrentYearPoints == 0)
              {
                 TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->CurrentYearPoints =
                 TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetEarnedPointsForCurrentYear(PaymentTransaction.DBTransaction, PaymentTransaction.Membership.Member);
                 TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->CurrentYearPoints += currentPoint;
              }
              else
              {
                 TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->CurrentYearPoints += currentPoint;
              }
           }
           return;

		}
		TDBTierLevel::GetTierLevelOfMember(PaymentTransaction.DBTransaction,PaymentTransaction.Membership.Member);
		TDateTime activationDate =  PaymentTransaction.Membership.Member.ActivationDate;
		TDateTime currentDate = Now();
		double earnedPoints  = 0;
		int lastYearTierLevel = 0;
		int currentYearTierLevel = 0;
		int tierLevel = 0;
		//replace activation date's year part
		TDateTime refDate = RecodeYear(activationDate, YearOf(Now()));
		//if timespan between current date and activation date is more than one year then calculate tier level
		//on the basis of points earned in previous year
		//then calculate tier level for the points earned in current year
		// use higher tier level
		int result =  Dateutils::CompareDate(refDate,currentDate);
		if(result == 1)
		{
			refDate = RecodeYear(activationDate, YearOf(Now()) - 1) ;
		}
		if(YearSpan(activationDate,currentDate) >= 1)
		{

			TDateTime startDate =  RecodeYear(refDate, YearOf(refDate) - 1);
			if(TGlobalSettings::Instance().LoyaltyMateEnabled)
			{
                           earnedPoints =  TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->PreviousYearPoints;
			}
			else
			{
				earnedPoints =  TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetEarnedPointsInDateRange(
                                                                            PaymentTransaction.DBTransaction,
			                                                    PaymentTransaction.Membership.Member,
                                                                            startDate,
                                                                            IncDay(refDate,-1));
			}
			lastYearTierLevel = TDBTierLevel::GetTierLevelAsPerEarnedPoints(PaymentTransaction.DBTransaction,earnedPoints);
		}

		if(TGlobalSettings::Instance().LoyaltyMateEnabled)
		{
			earnedPoints = TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->CurrentYearPoints;
		}
		else
		{
			earnedPoints =  TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetEarnedPointsInDateRange(PaymentTransaction.DBTransaction,
			PaymentTransaction.Membership.Member,refDate,currentDate);
		}
                double currentPoint = PaymentTransaction.Membership.Member.Points.getCurrentPointsEarned();
		earnedPoints += currentPoint;
                TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->CurrentYearPoints += currentPoint;
		currentYearTierLevel = TDBTierLevel::GetTierLevelAsPerEarnedPoints(PaymentTransaction.DBTransaction,earnedPoints);


		if(currentYearTierLevel < lastYearTierLevel)
		{
			tierLevel = lastYearTierLevel ;
		}
		else
		{
			tierLevel = currentYearTierLevel;
		}

		bool IsTierLevelChanged = PaymentTransaction.Membership.Member.TierLevel != tierLevel;
		if(IsTierLevelChanged)
		{
			TGlobalSettings::Instance().TierLevelChange   = tierLevel;
			MessageBox(PaymentTransaction.Membership.Member.Name + "'s Tier Level Changed To " +
			TDBTierLevel::GetTierLevelName(PaymentTransaction.DBTransaction,tierLevel), "Tier Level", MB_OK + MB_ICONINFORMATION);
		}

		PaymentTransaction.Membership.Member.TierLevel = tierLevel;
		TDBTierLevel::UpdateMemberTierLevel(PaymentTransaction.DBTransaction,PaymentTransaction.Membership.Member);
    }
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
}

void TListPaymentSystem::ArchivePatronCount(TPaymentTransaction &PaymentTransaction, long ArcBillKey)
{
	try
	{
        int zeroCount = 0;
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
                if(!MakePatronCountZero)
                {
    				IBInternalQuery->ParamByName("PATRON_COUNT")->AsInteger = ptrPatronTypes->Count;
                }
                else
                {
    				IBInternalQuery->ParamByName("PATRON_COUNT")->AsInteger = zeroCount;
                }
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
			IBInternalQuery->ParamByName("AMOUNT")->AsCurrency = RoundToNearest(
			PaymentTransaction.RewardsTransaction.Amount,
			0.01,
			TGlobalSettings::Instance().MidPointRoundsDown );
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
    bool isClippGroup = false;
    try
    {
        int identificationNumber = 0;

        if(PaymentTransaction.Orders->Count > 0)
        {
           TItemComplete *item = (TItemComplete*)(PaymentTransaction.Orders->Items[0]);
           identificationNumber = item->OrderIdentificationNo;
        }
		Currency Total = PaymentTransaction.Money.RoundedGrandTotal;
		Currency Discount = PaymentTransaction.Money.TotalAdjustment;
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
		"PATRON_COUNT, " "RECEIPT, " "SECURITY_REF, " "BILLED_LOCATION, " "INVOICE_NUMBER, " "SALES_TYPE, " "INVOICE_KEY,"
        "ROUNDING_ADJUSTMENT," "ORDER_IDENTIFICATION_NUMBER, " "REFUND_REFRECEIPT ) " "VALUES ("
		":ARCBILL_KEY, " ":TERMINAL_NAME, " ":STAFF_NAME, " ":TIME_STAMP, " ":TOTAL, " ":DISCOUNT, " ":PATRON_COUNT, " ":RECEIPT, "
		":SECURITY_REF, " ":BILLED_LOCATION," ":INVOICE_NUMBER, " ":SALES_TYPE, " ":INVOICE_KEY, "
        ":ROUNDING_ADJUSTMENT," ":ORDER_IDENTIFICATION_NUMBER, " ":REFUND_REFRECEIPT ) ";
		IBInternalQuery->ParamByName("ARCBILL_KEY")->AsString = Retval;
		IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
		IBInternalQuery->ParamByName("STAFF_NAME")->AsString = TDeviceRealTerminal::Instance().User.Name;
		IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = Now();
		IBInternalQuery->ParamByName("TOTAL")->AsCurrency = Total;
		IBInternalQuery->ParamByName("DISCOUNT")->AsCurrency = Discount;
        IBInternalQuery->ParamByName("ROUNDING_ADJUSTMENT")->AsCurrency = RoundToNearest(
					PaymentTransaction.Money.RoundingAdjustment,
					0.01,
					TGlobalSettings::Instance().MidPointRoundsDown);
		int TotalCount = 0;
		std::vector <TPatronType> ::iterator ptrPatronTypes;
        if(!MakePatronCountZero)
        {
            for (ptrPatronTypes = PaymentTransaction.Patrons.begin(); ptrPatronTypes != PaymentTransaction.Patrons.end(); ptrPatronTypes++)
            {
                TotalCount += ptrPatronTypes->Count;
            }
        }
        else
        {
                TotalCount = 0;
        }
		IBInternalQuery->ParamByName("PATRON_COUNT")->AsInteger = TotalCount;
        IBInternalQuery->ParamByName("ORDER_IDENTIFICATION_NUMBER")->AsInteger = identificationNumber;
		IBInternalQuery->ParamByName("SECURITY_REF")->AsInteger = Security->GetSecurityRefNumber();
		IBInternalQuery->ParamByName("INVOICE_NUMBER")->AsString = PaymentTransaction.InvoiceNumber;
		IBInternalQuery->ParamByName("SALES_TYPE")->AsInteger = PaymentTransaction.SalesType;
		IBInternalQuery->ParamByName("BILLED_LOCATION")->AsString = TDeviceRealTerminal::Instance().ID.Location;
		IBInternalQuery->ParamByName("INVOICE_KEY")->AsInteger = PaymentTransaction.InvoiceKey;
		IBInternalQuery->ParamByName("REFUND_REFRECEIPT")->AsString = PaymentTransaction.RefundRefReceipt;

		// set the receipt information if available, else insert null

		if( ManagerReceipt->ReceiptToArchive->Size > 0 )
		{
			ManagerReceipt->ReceiptToArchive->Position = 0;
			IBInternalQuery->ParamByName("RECEIPT")->LoadFromStream(ManagerReceipt->ReceiptToArchive);
		}
		else
		{
			IBInternalQuery->ParamByName("RECEIPT")->Clear();
		}

		IBInternalQuery->ExecQuery();

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"INSERT INTO DAYARCBILLPAY (" "DAYARCBILLPAY_KEY, " "ARCBILL_KEY, " "PAY_TYPE, " "VOUCHER_NUMBER, "
		"SUBTOTAL, " "ROUNDING, " "CASH_OUT, " "TAX_FREE, " "NOTE, PAY_TYPE_DETAILS," "PROPERTIES, "
        "GROUP_NUMBER, PAYMENT_CARD_TYPE, PAY_GROUP,CHARGED_TO_XERO,TIP_AMOUNT) " "VALUES ("
		":DAYARCBILLPAY_KEY, " ":ARCBILL_KEY, " ":PAY_TYPE, " ":VOUCHER_NUMBER, " ":SUBTOTAL, " ":ROUNDING, " ":CASH_OUT, " ":TAX_FREE, "
		":NOTE, :PAY_TYPE_DETAILS," ":PROPERTIES, " ":GROUP_NUMBER , :PAYMENT_CARD_TYPE, :PAY_GROUP,:CHARGED_TO_XERO,:TIP_AMOUNT) ";

        int paymentCounter = 0;
        bool changeIsChargerToXero = false;
        for (int i = 0; i < PaymentTransaction.PaymentsCount(); i++)
		{
			TPayment *SubPayment = PaymentTransaction.PaymentGet(i);
			if (SubPayment->GetPay() != 0)
			{
                paymentCounter++;
                if(!changeIsChargerToXero)
                  changeIsChargerToXero =  SubPayment->GetPaymentAttribute(ePayTypeChargeToXero);

                IBInternalQuery2->Close();
				IBInternalQuery2->SQL->Text = "SELECT GEN_ID(GEN_DAYARCBILLPAY, 1) FROM RDB$DATABASE";
				IBInternalQuery2->ExecQuery();
				int PaymentKey = IBInternalQuery2->Fields[0]->AsInteger;

				IBInternalQuery->Close();
				IBInternalQuery->ParamByName("DAYARCBILLPAY_KEY")->AsInteger = PaymentKey;
				IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = Retval;
                AnsiString payTypeName = "";
                AnsiString cardType = SubPayment->CardType;
                if( cardType != "" && cardType != NULL)
                {
                  payTypeName =  cardType;
                }
                else
                {
                    if (SubPayment->SysNameOveride != "")
                    {
                        payTypeName = SubPayment->SysNameOveride;
                    }
                    else
                    {
                        if (SubPayment->NameOveride == "")
                        {
                            payTypeName = SubPayment->Name;
                        }
                        else
                        {
                            payTypeName = SubPayment->NameOveride;
                        }
                    }
                }

                IBInternalQuery->ParamByName("PAY_TYPE")->AsString = payTypeName;

				if (SubPayment->GetPaymentAttribute(ePayTypeGetVoucherDetails))
				{
					IBInternalQuery->ParamByName("VOUCHER_NUMBER")->AsString = SubPayment->ReferenceNumber;
				}

                //checking clipp paytype
                if( payTypeName == "Clipp")
                    {
                         isClippGroup = true;
                    }
             

                if (!PaymentTransaction.CreditTransaction)
                {
                    IBInternalQuery->ParamByName("SUBTOTAL")->AsCurrency = RoundToNearest(
                    SubPayment->GetPayTendered(),
                    0.01,
                    TGlobalSettings::Instance().MidPointRoundsDown);
                }
                else
                {
                    IBInternalQuery->ParamByName("SUBTOTAL")->AsCurrency = RoundToNearest(
                    SubPayment->GetPayTendered(),
                    0.01,
                    TGlobalSettings::Instance().MidPointRoundsDown);
                }

                IBInternalQuery->ParamByName("TIP_AMOUNT")->AsCurrency = SubPayment->TipAmount;
				IBInternalQuery->ParamByName("ROUNDING")->AsCurrency = SubPayment->GetPayRounding();
				IBInternalQuery->ParamByName("CASH_OUT")->AsString = "F";
				if (SubPayment->GetPaymentAttribute(ePayTypeTaxFree))
				{
					IBInternalQuery->ParamByName("TAX_FREE")->AsString = "T";
				}
				else
				{
					IBInternalQuery->ParamByName("TAX_FREE")->AsString = "F";
				}

				IBInternalQuery->ParamByName("NOTE")->AsString = SubPayment->Note.SubString(1, 50);
				IBInternalQuery->ParamByName("PROPERTIES")->AsString = SubPayment->GetPropertyString();
				IBInternalQuery->ParamByName("GROUP_NUMBER")->AsInteger = SubPayment->GroupNumber;
				IBInternalQuery->ParamByName("PAY_TYPE_DETAILS")->AsString = SubPayment->EftposTransactionID;
				IBInternalQuery->ParamByName("PAYMENT_CARD_TYPE")->AsString = (UnicodeString)SubPayment->CardType;
                IBInternalQuery->ParamByName("CHARGED_TO_XERO")->AsString = changeIsChargerToXero ? "T" :"F";
                if(SubPayment->Name == PaymentTransaction.Membership.Member.Name + "'s Points")
                {
                   IBInternalQuery->ParamByName("PAY_GROUP")->AsString = "Redeem Points";
                }
                else
                {
                   IBInternalQuery->ParamByName("PAY_GROUP")->AsString = SubPayment->GetFirstAssignedGroup().Name;
                }

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
				if (SubPayment->GetPaymentAttribute(ePayTypeGetVoucherDetails))
				{
					IBInternalQuery->ParamByName("VOUCHER_NUMBER")->AsString = SubPayment->ReferenceNumber;
				}
				IBInternalQuery->ParamByName("SUBTOTAL")->AsCurrency = RoundToNearest(
				SubPayment->GetCashOut(),
				0.01,
				TGlobalSettings::Instance().MidPointRoundsDown);
                IBInternalQuery->ParamByName("TIP_AMOUNT")->AsCurrency = 0;
				IBInternalQuery->ParamByName("ROUNDING")->AsCurrency = SubPayment->GetCashOutRounding();
				IBInternalQuery->ParamByName("PAY_TYPE_DETAILS")->AsString = SubPayment->ReferenceNumber;
				IBInternalQuery->ParamByName("CASH_OUT")->AsString = "T";
				if (SubPayment->GetPaymentAttribute(ePayTypeTaxFree))
				{
					IBInternalQuery->ParamByName("TAX_FREE")->AsString = "T";
				}
				else
				{
					IBInternalQuery->ParamByName("TAX_FREE")->AsString = "F";
				}

				IBInternalQuery->ParamByName("NOTE")->AsString = SubPayment->Note.SubString(1, 80);
				IBInternalQuery->ParamByName("PROPERTIES")->AsString = SubPayment->GetPropertyString();
				IBInternalQuery->ParamByName("GROUP_NUMBER")->AsInteger = SubPayment->GroupNumber;
                IBInternalQuery->ParamByName("CHARGED_TO_XERO")->AsString = (SubPayment->GetPaymentAttribute(ePayTypeChargeToXero)) ? "T" :"F";
				IBInternalQuery->ExecQuery();
			}
		}


        if(paymentCounter > 1)
         {
            changeIsChargerToXero = false;
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
		IBInternalQuery->ParamByName("PAY_TYPE_DETAILS")->AsString = "";
         IBInternalQuery->ParamByName("TIP_AMOUNT")->AsCurrency = 0;
		IBInternalQuery->ParamByName("ROUNDING")->AsCurrency = 0;
		IBInternalQuery->ParamByName("CASH_OUT")->AsString = "F";
		IBInternalQuery->ParamByName("TAX_FREE")->AsString = "F";
		IBInternalQuery->ParamByName("NOTE")->AsString = "Total Change.";
        IBInternalQuery->ParamByName("CHARGED_TO_XERO")->AsString = changeIsChargerToXero ? "T" :"F";;
		if (CashPayment)
		{
			IBInternalQuery->ParamByName("SUBTOTAL")->AsCurrency = -RoundToNearest(CashPayment->GetChange(),0.01,TGlobalSettings::Instance().MidPointRoundsDown);
			IBInternalQuery->ParamByName("PROPERTIES")->AsString = CashPayment->GetPropertyString();
			IBInternalQuery->ParamByName("GROUP_NUMBER")->AsInteger = CashPayment->GroupNumber;
		}
		else
		{
			IBInternalQuery->ParamByName("SUBTOTAL")->AsCurrency = -RoundToNearest(PaymentTransaction.Money.Change,0.01,TGlobalSettings::Instance().MidPointRoundsDown);
			IBInternalQuery->ParamByName("PROPERTIES")->AsString = "";
			IBInternalQuery->ParamByName("GROUP_NUMBER")->AsInteger = 0;
		}

		// set the cash payment group for this adjustment
		IBInternalQuery->ParamByName("PAY_GROUP")->AsString = getDefaultCashPaymentGroup().Name;

		IBInternalQuery->ExecQuery();

		for (int i = 0; i < PaymentTransaction.PaymentsCount(); i++)
		{
			TPayment *SubPayment = PaymentTransaction.PaymentGet(i);
			Currency Value = 0;
			Currency ValueRnd = 0;
			if(PaymentTransaction.CreditTransaction)
			{
				Value = SubPayment->GetDiscount();
				ValueRnd = SubPayment->GetDiscountRounding();
			}
			else
			{
				Value = SubPayment->GetSurcharge();
				ValueRnd = SubPayment->GetSurchargeRounding();
			}

			if (Value != 0)
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
				IBInternalQuery->ParamByName("SUBTOTAL")->AsCurrency = RoundToNearest(Value,0.01,TGlobalSettings::Instance().MidPointRoundsDown);
				IBInternalQuery->ParamByName("ROUNDING")->AsCurrency = ValueRnd;
				if (SubPayment->GetPaymentAttribute(ePayTypeTaxFree))
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
				IBInternalQuery->ParamByName("PROPERTIES")->AsString = SubPayment->GetPropertyString();
                if(isClippGroup)
                {
                     IBInternalQuery->ParamByName("GROUP_NUMBER")->AsInteger = -999;
                }
                else
                {
				    IBInternalQuery->ParamByName("GROUP_NUMBER")->AsInteger = SubPayment->GroupNumber;
                }
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
    /**********************DLF MALL START****************************************/
    if(TGlobalSettings::Instance().MallIndex == DLFMALL )
    {
     GetDLFMallCMDCodeFirst(PaymentTransaction.InvoiceNumber,"OPENED");   //dlf mall    //DLF REQUIRED TO BE CHANGED LOCATION
     GetDLFMallCMDCodeSec(PaymentTransaction) ;
    }
  /**********************DLF MALL END****************************************/

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
				"INSERT INTO DAYARCHIVE ("
				"ARCHIVE_KEY,"
				"ARCBILL_KEY,"
				"TERMINAL_NAME,"
				"MENU_NAME,"
				"COURSE_NAME,"
				"ITEM_NAME,"
				"ITEM_CATEGORY,"
				"ITEM_SHORT_NAME,"
				"ITEM_ID,"
				"SIZE_NAME,"
				"TABLE_NUMBER,"
				"TABLE_NAME,"
				"SEAT_NUMBER,"
				"SERVER_NAME,"
				"TAB_NAME,"
				"LOYALTY_NAME,"
				"ORDER_TYPE,"
				"TIME_STAMP,"
				"TIME_STAMP_BILLED,"
				"ORDER_LOCATION,"
				"PRICE,"
				"COST,"
				"HAPPY_HOUR,"
				"NOTE,"
				"SECURITY_REF,"
				"TIME_KEY,"
				"GST_PERCENT,"
				"COST_GST_PERCENT,"
				"QTY,"
				"DISCOUNT,"
				"REDEEMED,"
				"POINTS_PERCENT,"
				"POINTS_EARNED,"
				"LOYALTY_KEY,"
				"THIRDPARTYCODES_KEY,"
				"CATEGORY_KEY,"
				"DISCOUNT_REASON,"
				"PRICE_LEVEL0,"
				"PRICE_LEVEL1,"
				"SERVINGCOURSES_KEY,"
				"CHIT_NAME,"
				"CHIT_OPTION,"
				"BASE_PRICE,"
				"DISCOUNT_WITHOUT_TAX,"
				"TAX_ON_DISCOUNT,"
                "PRICE_INCL, "
                "PRICE_ADJUST "
				")"
				" VALUES "
				"("
				":ARCHIVE_KEY,"
				":ARCBILL_KEY,"
				":TERMINAL_NAME,"
				":MENU_NAME,"
				":COURSE_NAME,"
				":ITEM_NAME,"
				":ITEM_CATEGORY,"
				":ITEM_SHORT_NAME,"
				":ITEM_ID,"
				":SIZE_NAME,"
				":TABLE_NUMBER,"
				":TABLE_NAME,"
				":SEAT_NUMBER,"
				":SERVER_NAME,"
				":TAB_NAME,"
				":LOYALTY_NAME,"
				":ORDER_TYPE,"
				":TIME_STAMP,"
				":TIME_STAMP_BILLED,"
				":ORDER_LOCATION,"
				":PRICE,"
				":COST,"
				":HAPPY_HOUR,"
				":NOTE,"
				":SECURITY_REF,"
				":TIME_KEY,"
				":GST_PERCENT,"
				":COST_GST_PERCENT,"
				":QTY,"
				":DISCOUNT,"
				":REDEEMED,"
				":POINTS_PERCENT,"
				":POINTS_EARNED,"
				":LOYALTY_KEY,"
				":THIRDPARTYCODES_KEY,"
				":CATEGORY_KEY,"
				":DISCOUNT_REASON,"
				":PRICE_LEVEL0,"
				":PRICE_LEVEL1,"
				":SERVINGCOURSES_KEY,"
				":CHIT_NAME,"
				":CHIT_OPTION,"
				":BASE_PRICE,"
				":DISCOUNT_WITHOUT_TAX,"
				":TAX_ON_DISCOUNT,"
                ":PRICE_INCL, "
                ":PRICE_ADJUST "
				");";

                IBInternalQuery->ParamByName("CHIT_NAME")->AsString = Order->ChitNumber.Name;
                IBInternalQuery->ParamByName("CHIT_NAME")->IsNull = Order->ChitNumber.ChitNumberKey == 0 || Order->ChitNumber.Name.Length() == 0;
                IBInternalQuery->ParamByName("CHIT_OPTION")->AsString = Order->ChitNumber.ActiveOption();
                IBInternalQuery->ParamByName("CHIT_OPTION")->IsNull = Order->ChitNumber.HasAnOption() == false;

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
				case TabHandheldCashAccount:
				case TabCashAccount:
				case TabNone:
                case TabClipp:
				case TabTableSeat:
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
                IBInternalQuery->ParamByName("PRICE")->AsCurrency = Order->PriceEach_BillCalc();
				IBInternalQuery->ParamByName("COST")->AsCurrency = RoundToNearest(
				Order->Cost,
				0.01,
				TGlobalSettings::Instance().MidPointRoundsDown);
				IBInternalQuery->ParamByName("COST_GST_PERCENT")->AsFloat = double(Order->CostGSTPercent);
                IBInternalQuery->ParamByName("DISCOUNT")->AsCurrency = Order->TotalAdjustment();
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
                if(Order->GetQty()!=0)
                  IBInternalQuery->ParamByName("BASE_PRICE")->AsCurrency = Order->BillCalcResult.BasePrice;///Order->GetQty();


            if(Order->TotalAdjustment()>0&&Order->BillCalcResult.DiscountWithoutTax<0||Order->TotalAdjustment()<0&&Order->BillCalcResult.DiscountWithoutTax>0)
            {
               IBInternalQuery->ParamByName("DISCOUNT_WITHOUT_TAX")->AsCurrency =-1*Order->BillCalcResult.DiscountWithoutTax;
             }
            else
            {
               IBInternalQuery->ParamByName("DISCOUNT_WITHOUT_TAX")->AsCurrency = Order->BillCalcResult.DiscountWithoutTax;
            }

            if(Order->TotalAdjustment()>0&&Order->BillCalcResult.TaxOnDiscount<0||Order->TotalAdjustment()<0&&Order->BillCalcResult.TaxOnDiscount>0)
            {
              IBInternalQuery->ParamByName("TAX_ON_DISCOUNT")->AsCurrency =-1* Order->BillCalcResult.TaxOnDiscount;
            }
            else
            {
              IBInternalQuery->ParamByName("TAX_ON_DISCOUNT")->AsCurrency = Order->BillCalcResult.TaxOnDiscount;
            }

            IBInternalQuery->ParamByName("PRICE_INCL")->AsCurrency = Order->BillCalcResult.PriceIncl;
            IBInternalQuery->ParamByName("PRICE_ADJUST")->AsCurrency = Order->PriceLevelCustom;


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
				ArchiveOrderTaxes(PaymentTransaction.DBTransaction, MasterArchiveKey, Order);

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

                    /**********************DLF MALL START****************************************/
                int catkey = TDBOrder::GetArchiveCategory(PaymentTransaction.DBTransaction, Order->Categories->FinancialCategory);
                 if(TGlobalSettings::Instance().MallIndex == DLFMALL )
                GetDLFMallCMDCodeThird(Order,  IntToStr(catkey), Order->TotalAdjustment());
               /**********************DLF MALL END****************************************/
             	for (int i = 0; i < Order->SubOrders->Count; i++)
				{
					TItemCompleteSub *CurrentSubOrder = (TItemCompleteSub*)Order->SubOrders->Items[i];

					int SubPrimaryArcCatkey = TDBOrder::GetArchiveCategory(PaymentTransaction.DBTransaction,
					CurrentSubOrder->Categories->FinancialCategory);
                     /**********************DLF MALL START****************************************/
				int catkey = TDBOrder::GetArchiveCategory(PaymentTransaction.DBTransaction, CurrentSubOrder->Categories->FinancialCategory);
				if(TGlobalSettings::Instance().MallIndex == DLFMALL )
				GetDLFMallCMDCodeSubOrderThird(CurrentSubOrder,  IntToStr(catkey), CurrentSubOrder->TotalAdjustment());
				/**********************DLF MALL END****************************************/

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
						 "COST," "HAPPY_HOUR," "NOTE," "SECURITY_REF," "TIME_KEY," "GST_PERCENT," "COST_GST_PERCENT," "QTY," "DISCOUNT,"
						 "REDEEMED," "POINTS_PERCENT," "POINTS_EARNED," "LOYALTY_KEY," "THIRDPARTYCODES_KEY," "CATEGORY_KEY," "DISCOUNT_REASON,"
						 "PRICE_LEVEL0," "PRICE_LEVEL1," "SERVINGCOURSES_KEY, CHIT_NAME, CHIT_OPTION,"
                         "BASE_PRICE,"
                         "DISCOUNT_WITHOUT_TAX,"
                         "TAX_ON_DISCOUNT, PRICE_INCL, PRICE_ADJUST )" " VALUES (" ":ARCHIVE_KEY," ":ARCBILL_KEY," ":TERMINAL_NAME,"
						 ":MENU_NAME," ":COURSE_NAME," ":ITEM_NAME," ":ITEM_CATEGORY," ":ITEM_SHORT_NAME," ":ITEM_ID," ":SIZE_NAME,"
						 ":TABLE_NUMBER," ":TABLE_NAME," ":SEAT_NUMBER," ":SERVER_NAME," ":TAB_NAME," ":LOYALTY_NAME," ":ORDER_TYPE,"
						 ":TIME_STAMP," ":TIME_STAMP_BILLED," ":ORDER_LOCATION," ":PRICE," ":COST," ":HAPPY_HOUR," ":NOTE," ":SECURITY_REF,"
						 ":TIME_KEY," ":GST_PERCENT," ":COST_GST_PERCENT," ":QTY," ":DISCOUNT," ":REDEEMED," ":POINTS_PERCENT," ":POINTS_EARNED,"
						 ":LOYALTY_KEY," ":THIRDPARTYCODES_KEY," ":CATEGORY_KEY," ":DISCOUNT_REASON," ":PRICE_LEVEL0," ":PRICE_LEVEL1,"
						 ":SERVINGCOURSES_KEY, :CHIT_NAME, :CHIT_OPTION, "
                         ":BASE_PRICE,"
                         ":DISCOUNT_WITHOUT_TAX,"
                         ":TAX_ON_DISCOUNT, :PRICE_INCL, :PRICE_ADJUST );";

					IBInternalQuery->ParamByName("CHIT_NAME")->AsString =
					Order->ChitNumber.Name;
					IBInternalQuery->ParamByName("CHIT_NAME")->IsNull =
					Order->ChitNumber.ChitNumberKey == 0;
					IBInternalQuery->ParamByName("CHIT_OPTION")->AsString =
					Order->ChitNumber.ActiveOption();
					IBInternalQuery->ParamByName("CHIT_OPTION")->IsNull =
					Order->ChitNumber.HasAnOption() == false;

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
					case TabHandheldCashAccount:
					case TabCashAccount:
					case TabNone:
                    case TabClipp:
					case TabTableSeat:
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
                    IBInternalQuery->ParamByName("PRICE")->AsCurrency = CurrentSubOrder->PriceEach_BillCalc();
					//IBInternalQuery->ParamByName("PRICE")->AsCurrency = fabs(RoundToNearest(CurrentSubOrder->PriceEach_BillCalc(),0.01,
					//TGlobalSettings::Instance().MidPointRoundsDown)); // always store price as positive figure, as for refund orders the calculation with qty will set the final result to negative
					//IBInternalQuery->ParamByName("GST_PERCENT")->AsFloat = double(CurrentSubOrder->GSTPercent);
					IBInternalQuery->ParamByName("ORDER_LOCATION")->AsString = Order->OrderedLocation;
					IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = Order->TimeStamp;
					IBInternalQuery->ParamByName("TIME_STAMP_BILLED")->AsDateTime = Now();
					IBInternalQuery->ParamByName("COST")->AsCurrency = RoundToNearest(
					CurrentSubOrder->Cost,
					0.01,
					TGlobalSettings::Instance().MidPointRoundsDown);
                                        IBInternalQuery->ParamByName("DISCOUNT")->AsCurrency = CurrentSubOrder->TotalAdjustment();
					//IBInternalQuery->ParamByName("DISCOUNT")->AsCurrency = RoundToNearest(CurrentSubOrder->TotalAdjustment(),0.01,
					//TGlobalSettings::Instance().MidPointRoundsDown );
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
                    IBInternalQuery->ParamByName("BASE_PRICE")->AsCurrency = CurrentSubOrder->BillCalcResult.BasePrice;//Order->GetQty();

                     if(CurrentSubOrder->TotalAdjustment()>0&&CurrentSubOrder->BillCalcResult.DiscountWithoutTax<0||CurrentSubOrder->TotalAdjustment()<0&&CurrentSubOrder->BillCalcResult.DiscountWithoutTax>0)
            {
               IBInternalQuery->ParamByName("DISCOUNT_WITHOUT_TAX")->AsCurrency =-1*CurrentSubOrder->BillCalcResult.DiscountWithoutTax;
             }
            else
            {
               IBInternalQuery->ParamByName("DISCOUNT_WITHOUT_TAX")->AsCurrency = CurrentSubOrder->BillCalcResult.DiscountWithoutTax;
            }

            if(CurrentSubOrder->TotalAdjustment()>0&&CurrentSubOrder->BillCalcResult.TaxOnDiscount<0||CurrentSubOrder->TotalAdjustment()<0&&CurrentSubOrder->BillCalcResult.TaxOnDiscount>0)
            {
              IBInternalQuery->ParamByName("TAX_ON_DISCOUNT")->AsCurrency =-1* CurrentSubOrder->BillCalcResult.TaxOnDiscount;
            }
            else
            {
              IBInternalQuery->ParamByName("TAX_ON_DISCOUNT")->AsCurrency = CurrentSubOrder->BillCalcResult.TaxOnDiscount;
            }

            IBInternalQuery->ParamByName("PRICE_INCL")->AsCurrency = CurrentSubOrder->BillCalcResult.PriceIncl;
            IBInternalQuery->ParamByName("PRICE_ADJUST")->AsCurrency = CurrentSubOrder->PriceLevelCustom;

                  //  IBInternalQuery->ParamByName("DISCOUNT_WITHOUT_TAX")->AsCurrency = CurrentSubOrder->BillCalcResult.DiscountWithoutTax;
                  //  IBInternalQuery->ParamByName("TAX_ON_DISCOUNT")->AsCurrency = CurrentSubOrder->BillCalcResult.TaxOnDiscount;
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
					ArchiveOrderTaxes(PaymentTransaction.DBTransaction, SubArchiveKey, CurrentSubOrder);

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
        /**********************DLF MALL START****************************************/
        if(TGlobalSettings::Instance().MallIndex == DLFMALL )
        {
            GetDLFMallCMDCodeForth(PaymentTransaction) ;
            GetDLFMallCMDCodeFifth(PaymentTransaction) ;
        }
        /**********************DLF MALL END****************************************/

}

void TListPaymentSystem::ArchiveOrderDiscounts(Database::TDBTransaction &DBTransaction, int ArchiveKey, TItemMinorComplete *Order)
{
	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	DBTransaction.RegisterQuery(IBInternalQuery);

	for (std::vector <TDiscount> ::const_iterator ptrDiscounts = Order->Discounts.begin(); ptrDiscounts != Order->Discounts.end();
	std::advance(ptrDiscounts, 1))
	{
        if(Order->DiscountValue_BillCalc(ptrDiscounts) == 0)
            continue;

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_DAYARCORDERDISCOUNTS, 1) FROM RDB$DATABASE";
        IBInternalQuery->ExecQuery();
        int OrderDiscountKey = IBInternalQuery->Fields[0]->AsInteger;

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
        "INSERT INTO DAYARCORDERDISCOUNTS (" "ARCORDERDISCOUNTS_KEY," "ARCHIVE_KEY," "DISCOUNT_KEY," "NAME," "DESCRIPTION,"
        "PERCENTAGE," "AMOUNT," "DISCOUNTED_VALUE," "DISCOUNT_TYPE," "DISCOUNT_MODE," "SOURCE," "ROUNDING," "MENU," "APPEARANCE_ORDER,"
        "PRIORITY," "DISCOUNT_ID, MEMBERS_ONLY, MEMBERS_EXEMPT,DISCOUNT_GROUPNAME ) " "VALUES (" ":ARCORDERDISCOUNTS_KEY," ":ARCHIVE_KEY," ":DISCOUNT_KEY," ":NAME," ":DESCRIPTION,"
        ":PERCENTAGE," ":AMOUNT," ":DISCOUNTED_VALUE," ":DISCOUNT_TYPE," ":DISCOUNT_MODE," ":SOURCE," ":ROUNDING," ":MENU,"
        ":APPEARANCE_ORDER," ":PRIORITY," ":DISCOUNT_ID, :MEMBERS_ONLY, :MEMBERS_EXEMPT, :DISCOUNT_GROUPNAME);";
        IBInternalQuery->ParamByName("ARCORDERDISCOUNTS_KEY")->AsInteger = OrderDiscountKey;
        IBInternalQuery->ParamByName("ARCHIVE_KEY")->AsInteger = ArchiveKey;
        IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = ptrDiscounts->DiscountKey;
        IBInternalQuery->ParamByName("NAME")->AsString = ptrDiscounts->Name.SubString(1, 15);
        IBInternalQuery->ParamByName("DISCOUNT_GROUPNAME")->AsString =ptrDiscounts->DiscountGroupList.size() > 0 ? ptrDiscounts->DiscountGroupList[0].Name : UnicodeString::UnicodeString("");
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
        //IBInternalQuery->ParamByName("AMOUNT")->AsCurrency = RoundToNearest(ptrDiscounts->Amount,0.01,TGlobalSettings::Instance().MidPointRoundsDown);
//		IBInternalQuery->ParamByName("DISCOUNTED_VALUE")->AsCurrency = Order->DiscountValue(ptrDiscounts);
        IBInternalQuery->ParamByName("DISCOUNTED_VALUE")->AsCurrency = RoundToNearest(Order->DiscountValue_BillCalc(ptrDiscounts),0.01,TGlobalSettings::Instance().MidPointRoundsDown);
        IBInternalQuery->ParamByName("ROUNDING")->AsCurrency = ptrDiscounts->Rounding;
        IBInternalQuery->ParamByName("DISCOUNT_TYPE")->AsInteger = ptrDiscounts->Type;
        IBInternalQuery->ParamByName("DISCOUNT_MODE")->AsInteger = ptrDiscounts->Mode;
        IBInternalQuery->ParamByName("SOURCE")->AsInteger = ptrDiscounts->Source;
        IBInternalQuery->ParamByName("APPEARANCE_ORDER")->AsInteger = ptrDiscounts->AppearanceOrder;
        IBInternalQuery->ParamByName("PRIORITY")->AsInteger = ptrDiscounts->Priority;
        IBInternalQuery->ParamByName("DISCOUNT_ID")->AsString = ptrDiscounts->DiscountCode;
        IBInternalQuery->ParamByName("MEMBERS_ONLY")->AsString = (ptrDiscounts->MembersOnly == true) ? "T" : "F";
        IBInternalQuery->ParamByName("MEMBERS_EXEMPT")->AsString = (ptrDiscounts->MembersExempt == true) ? "T" : "F";
        IBInternalQuery->ExecQuery();
	}

}

void TListPaymentSystem::ArchiveOrderTaxes(Database::TDBTransaction &DBTransaction, int ArchiveKey, TItemMinorComplete *Order)
{
	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	DBTransaction.RegisterQuery(IBInternalQuery);

	std::vector<BillCalculator::TTaxResult> taxInfomation = Order->BillCalcResult.Tax;

	UnicodeString query =
	"INSERT INTO "
	"  DAYARCORDERTAXES "
	"("
	"   ARCORDERTAXES_KEY,"
	"   ARCHIVE_KEY,"
	"   TAX_NAME,"
	"   TAX_VALUE, "
	"   TAX_TYPE "
	") "
	"VALUES "
	"("
	"   :ARCORDERTAXES_KEY,"
	"   :ARCHIVE_KEY,"
	"   :TAX_NAME,"
	"   :TAX_VALUE, "
	"   :TAX_TYPE "
	");";

	for (std::vector<BillCalculator::TTaxResult>::iterator itTaxes = taxInfomation.begin(); itTaxes != taxInfomation.end(); itTaxes++)
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_DAYARCORDERTAXES, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		int OrderTaxKey = IBInternalQuery->Fields[0]->AsInteger;

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = query;
		IBInternalQuery->ParamByName("ARCORDERTAXES_KEY")->AsInteger = OrderTaxKey;
		IBInternalQuery->ParamByName("ARCHIVE_KEY")->AsInteger = ArchiveKey;
		IBInternalQuery->ParamByName("TAX_NAME")->AsString = itTaxes->Name.SubString(1, 50);
		IBInternalQuery->ParamByName("TAX_VALUE")->AsCurrency = RoundToNearest(itTaxes->Value,0.01,TGlobalSettings::Instance().MidPointRoundsDown);
		switch( itTaxes->TaxType )
		{
		case TTaxType::ttSale:
			IBInternalQuery->ParamByName("TAX_TYPE")->AsInteger = 0;
			break;
		case TTaxType::ttLocal:
			IBInternalQuery->ParamByName("TAX_TYPE")->AsInteger = 4;
			break;
        case TTaxType::ttProfit:
			IBInternalQuery->ParamByName("TAX_TYPE")->AsInteger = 5;
			break;
		default:
			IBInternalQuery->ParamByName("TAX_TYPE")->AsInteger = 99;
			break;
		}
		IBInternalQuery->ExecQuery();
	}

	if (Order->BillCalcResult.ServiceCharge.Percentage != 0)
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_DAYARCORDERTAXES, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		int OrderTaxKey = IBInternalQuery->Fields[0]->AsInteger;

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = query;
		IBInternalQuery->ParamByName("ARCORDERTAXES_KEY")->AsInteger = OrderTaxKey;
		IBInternalQuery->ParamByName("ARCHIVE_KEY")->AsInteger = ArchiveKey;
		IBInternalQuery->ParamByName("TAX_NAME")->AsString = "Service Charge";
		IBInternalQuery->ParamByName("TAX_VALUE")->AsCurrency = RoundToNearest(Order->BillCalcResult.ServiceCharge.Value,0.01,
		                                                        TGlobalSettings::Instance().MidPointRoundsDown );
		IBInternalQuery->ParamByName("TAX_TYPE")->AsInteger = 2;
		IBInternalQuery->ExecQuery();

		if (Order->BillCalcResult.ServiceCharge.TaxPercentage != 0)
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_DAYARCORDERTAXES, 1) FROM RDB$DATABASE";
			IBInternalQuery->ExecQuery();
			int OrderTaxKey = IBInternalQuery->Fields[0]->AsInteger;

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = query;
			IBInternalQuery->ParamByName("ARCORDERTAXES_KEY")->AsInteger = OrderTaxKey;
			IBInternalQuery->ParamByName("ARCHIVE_KEY")->AsInteger = ArchiveKey;
			IBInternalQuery->ParamByName("TAX_NAME")->AsString = "Service Charge Tax";
			IBInternalQuery->ParamByName("TAX_VALUE")->AsCurrency = RoundToNearest(Order->BillCalcResult.ServiceCharge.TaxValue,0.01,
			                                                        TGlobalSettings::Instance().MidPointRoundsDown );
			IBInternalQuery->ParamByName("TAX_TYPE")->AsInteger = 3;
			IBInternalQuery->ExecQuery();
		}
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
			if ((Payment->GetPaymentAttribute(ePayTypeCSV)) && ((Payment->GetPay() != 0) || (Payment->GetCashOut() != 0)))
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
					(Payment->GetPay() + Payment->GetCashOut() + Payment->GetAdjustment(), ffCurrency, 15, 2));

					Csv.SaveToFile(File);
				}
				__finally
				{
					CloseHandle(hFile);
                    hFile = NULL;
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
						TDBTables::ClearPatronCount(PaymentTransaction.DBTransaction, TableSeat.TableNo);
					}
				}

				if (TDBTab::GetTabRemovable(PaymentTransaction.DBTransaction, *itTabs))
				{
					Invoice->SetInvoiceClosed(PaymentTransaction.DBTransaction, TDBTab::GetTabInvoice(PaymentTransaction.DBTransaction,
					*itTabs));
				}
                TDBTab::CloseTab(PaymentTransaction.DBTransaction, *itTabs);

				/*if (!TDBTab::CloseTab(PaymentTransaction.DBTransaction, *itTabs))
				{
					if (TDBTab::GetTabExists(PaymentTransaction.DBTransaction, *itTabs))
					{
						TDBTab::SetPartialPayment(PaymentTransaction.DBTransaction, *itTabs, PaymentTransaction.InvoiceNumber);
					}
				}*/
              if(!TDBTables::IsEmpty(PaymentTransaction.DBTransaction,TableSeat.TableNo))
              {
                 if (TDBTab::GetTabExists(PaymentTransaction.DBTransaction, TableSeat.TableNo))
					{
						TDBTab::SetPartialPayment(PaymentTransaction.DBTransaction, TableSeat.TableNo, PaymentTransaction.InvoiceNumber,true);
					}
              }
              else
              {
                TDBTab::ClearTabHistory(PaymentTransaction.DBTransaction, TableSeat.TableNo, true);
              }


			}
			else if (TabType == TabRoom)
			{
				int RoomNumber = TDBRooms::GetRoomNumber(PaymentTransaction.DBTransaction, *itTabs);
				if (TDBTab::GetTabRemovable(PaymentTransaction.DBTransaction, *itTabs))
				{
					TDBRooms::SetPartyName(PaymentTransaction.DBTransaction, RoomNumber, "");
					TDBRooms::SetRoomStatus(PaymentTransaction.DBTransaction, RoomNumber, eVacant);
				}
			}
			else
			{
				if (TDBTab::GetTabRemovable(PaymentTransaction.DBTransaction, *itTabs))
				{
					Invoice->SetInvoiceClosed(PaymentTransaction.DBTransaction, TDBTab::GetTabInvoice(PaymentTransaction.DBTransaction,
					*itTabs));
				}

				if (!TDBTab::CloseTab(PaymentTransaction.DBTransaction, *itTabs))
				{
					if (TDBTab::GetTabExists(PaymentTransaction.DBTransaction, *itTabs))
					{
						TDBTab::SetPartialPayment(PaymentTransaction.DBTransaction, *itTabs, PaymentTransaction.InvoiceNumber,false);
					}
				}
                else
                {
                     TDBTab::ClearTabHistory(PaymentTransaction.DBTransaction, *itTabs, false);
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

	if (PaymentTransaction.Money.TotalAdjustment != 0)
	{
		PaymentTransaction.TotalAdjustment = PaymentTransaction.Money.TotalAdjustment;
		PaymentTransaction.DiscountReason = PaymentTransaction.TotalAdjustment < 0 ? "Discount " : "Surcharge";
	}

	std::set <__int64> TabKeys; // Tabs from which the orders come from.
	TDBOrder::GetTabKeysFromOrders(PaymentTransaction.Orders, TabKeys);
    bool isTable = false;
    int tabKey = 0;
	std::auto_ptr <TStringList> TabHistory(new TStringList);
	for (std::set <__int64> ::iterator itTabs = TabKeys.begin(); itTabs != TabKeys.end(); advance(itTabs, 1))
	{
        tabKey = *itTabs;
        isTable = false;
        TMMTabType TabType = TDBTab::GetTabType(PaymentTransaction.DBTransaction, *itTabs);
		if (TabType == TabTableSeat)
		   {
              TTableSeat TableSeat;
			  if (TDBTables::GetTableSeat(PaymentTransaction.DBTransaction, *itTabs, &TableSeat))
              {
                 tabKey = TableSeat.TableNo;
                 isTable = true;
              }
           }
		TDBTab::GetPartialPaymentList(PaymentTransaction.DBTransaction, tabKey, TabHistory.get(),isTable);
        for(int x = 0 ; x < TabHistory->Count; x++)
        {
           if(LastReceipt->TabHistory->IndexOf(TabHistory->Strings[x]) == -1)
           {
             LastReceipt->TabHistory->Add(TabHistory->Strings[x]);
           }
        }
	}

	LastReceipt->PaymentType = ptFinal;

	LastReceipt->WaitTime = TDBSaleTimes::GetAverageWaitTimeMins(PaymentTransaction.DBTransaction);
	if (PaymentTransaction.InvoiceKey != 0)
	{
		LastReceipt->AccountPayment = true;
		LastReceipt->AccountInvoiceNumber = Invoice->GetInvoiceNumber(PaymentTransaction.DBTransaction, PaymentTransaction.InvoiceKey);

		TMMContactInfo InvoiceOwnerInfo;
		int ContactKey = Invoice->GetInvoiceOwner(PaymentTransaction.DBTransaction, PaymentTransaction.InvoiceKey);
		TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetContactDetails(PaymentTransaction.DBTransaction, ContactKey,
		InvoiceOwnerInfo);

		LastReceipt->ExtraInfo->Add("Account: " + InvoiceOwnerInfo.Name);
		//		LastReceipt->ExtraInfo->Add("Member No. " + InvoiceOwnerInfo.MembershipNumber);
		LastReceipt->ExtraInfo->Add("Invoice # " + LastReceipt->AccountInvoiceNumber);
		LastReceipt->ExtraInfo->Add("");
		LastReceipt->ExtraInfo->Add("Signature ____________________ ");
	}

	if (PaymentTransaction.SalesType == eRoomSale)
	{
		if (PaymentTransaction.Phoenix.AccountNumber != 0 &&PaymentTransaction.Phoenix.AccountNumber != "")
		{
            if(TGlobalSettings::Instance().PMSType != SiHot)
			    LastReceipt->ExtraInfo->Add("Room Number # " + PaymentTransaction.Phoenix.AccountNumber);
            else
                LastReceipt->ExtraInfo->Add("Room Number # " + PaymentTransaction.Phoenix.RoomNumber);
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
    if(PaymentTransaction.WebOrderKey > 0)
    {

        TDBWebUtil::getWebOrderExtraData(PaymentTransaction.DBTransaction, PaymentTransaction.WebOrderKey,"DELIVERY", LastReceipt->DeliveryInfo);
        TDBWebUtil::getWebOrderExtraData(PaymentTransaction.DBTransaction, PaymentTransaction.WebOrderKey,"PAYMENT_INFO", LastReceipt->PaymentInfo);
        TDBWebUtil::getWebOrderExtraData(PaymentTransaction.DBTransaction, PaymentTransaction.WebOrderKey, "COMMENTS", LastReceipt->OrderComments);

        if(TGlobalSettings::Instance().CaptureCustomerName)
        {
           if(!PaymentTransaction.ChitNumber.Valid())
           {
              LastReceipt->DeliveryInfo->Clear();
           }
        }
        if(PaymentTransaction.ChitNumber.Valid())
        {
           if(!PaymentTransaction.ChitNumber.IsCaptureCustomerDetails)
           {
              LastReceipt->DeliveryInfo->Clear();
           }
        }
    }

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
    if(TGlobalSettings::Instance().ExportReprintReceipt)
     ExportReceipt(StringReceipt.get(),PaymentTransaction);
	ManagerReceipt->ReceiptToArchive->Position = 0;

	for (int i = 0; i < StringReceipt->Count; i++)
	{
		TDeviceRealTerminal::Instance().SecurityPort->SetData(StringReceipt->Strings[i]);
	}
}

void TListPaymentSystem::ExportReceipt(TStringList *StringReceipt,TPaymentTransaction &PaymentTransaction)
{
    AnsiString fileName = ExtractFilePath(Application->ExeName) +"Exports\\" ;
    if(!DirectoryExists(fileName))
    {
        CreateDir(fileName);
        fileName += "Receipts Export\\";
        CreateDir(fileName);
    }
    else
    {
       fileName += "Receipts Export\\";
    }
    AnsiString date = Now().FormatString("yyyy-mm-dd - hh-mm-ss");
    fileName += PaymentTransaction.InvoiceNumber+" "+date+" "+".txt";
    StringReceipt->SaveToFile(fileName );
}

void TListPaymentSystem::SetInvoiceNumber(TPaymentTransaction &PaymentTransaction)
{
   if(!TManagerDelayedPayment::Instance().IsDelayedPayment(PaymentTransaction))
    {
      if(PaymentTransaction.InvoiceNumber == "" || PaymentTransaction.InvoiceNumber == "Undefined")
      {
            if(TReceiptUtility::CheckRefundCancelTransaction(PaymentTransaction) &&
               TGlobalSettings::Instance().CaptureRefundRefNo)
            {
                PaymentTransaction.InvoiceNumber = "RV " + Invoice->GetVoidInvoiceNumber(PaymentTransaction.DBTransaction);
            }
             else
             {
                    PaymentTransaction.InvoiceNumber = Invoice->GetNextInvoiceNumber(PaymentTransaction.DBTransaction,PaymentTransaction.TypeOfSale);
             }

      }
    }
   else
   {
       TItemComplete *Order = (TItemComplete*)PaymentTransaction.Orders->Items[0];
       PaymentTransaction.InvoiceNumber = Order->DelayedInvoiceNumber;
   }
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
	TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->UpdateViewers();
	// Trigger Smartcard Viewers to update.

}

void TListPaymentSystem::PreparePayments(TPaymentTransaction &PaymentTransaction)
{
         TPayment *CashPayment = PaymentTransaction.PaymentFind(CASH);
    	if (CashPayment == NULL)
    	{
	    	TPayment *NewPayment = new TPayment;
		    NewPayment->Name = CASH;
    		NewPayment->SetPaymentAttribute(ePayTypeOpensCashDrawer,true);
	    	NewPayment->SetPaymentAttribute(ePayTypeCash,true);
		    NewPayment->DisplayOrder = -1;
    		NewPayment->Colour = clGreen;
	    	PaymentTransaction.PaymentAdd(NewPayment);
    	}

        if( ( PaymentTransaction.Type == eTransQuickSale ) && ( PaymentTransaction.SalesType == eTab ) )
        {
            TPayment *ClippPayment = PaymentTransaction.PaymentFind(CLIPP);
            if (ClippPayment == NULL)
            {
                TPayment *NewPayment = new TPayment;
                NewPayment->Name = CLIPP;
                NewPayment->SetPaymentAttribute(ePayTypeClipp,true);
                NewPayment->DisplayOrder = -1;
                NewPayment->Colour = clGreen;
                PaymentTransaction.PaymentAdd(NewPayment);
            }

            TPayment *TipPayment = PaymentTransaction.PaymentFind(TIPS);
            if (TipPayment == NULL)
            {
                TPayment *NewPayment = new TPayment;
                NewPayment->Name = TIPS;
                NewPayment->SetPaymentAttribute(ePayTypeAllowTips,true);
                NewPayment->DisplayOrder = -1;
                NewPayment->Colour = clGreen;
                PaymentTransaction.PaymentAdd(NewPayment);
            }
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
							MyDocket->Text = StringReplace(MyDocket->Text, "%USER%", TDeviceRealTerminal::Instance().User.Name,
							TReplaceFlags() << rfReplaceAll);
							MyDocket->Text = StringReplace(MyDocket->Text, "%POS%", TDeviceRealTerminal::Instance().ID.Name,
							TReplaceFlags() << rfReplaceAll);
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
	bool ChequesOk = true;
	bool EftPosOk = true;
	bool PhoenixHSOk = true;
	bool DialogsOk = true;
	bool InvoiceOk = true;
	bool PocketVoucher = true;
	bool GeneralLedgerMate = true;
	bool RMSCSVRoomExport = true;
    bool NewBookCSVRoomExport = true;
    bool LoyaltyVouchers = true;
    bool WalletTransaction = true;

    WalletTransaction = ProcessWalletTransaction(PaymentTransaction);
    if (!WalletTransaction)
	   return RetVal;

    ChequesOk = ProcessChequePayment(PaymentTransaction);
	if (!ChequesOk)
	   return RetVal;

    EftPosOk = ProcessEftPosPayment(PaymentTransaction,RequestEFTPOSReceipt);
	if (!EftPosOk)
	   return RetVal;

    InvoiceOk = ProcessInvoicePayment(PaymentTransaction);
	if (!InvoiceOk)
	   return RetVal;

	DialogsOk = ProcessComfirmationDialogs(PaymentTransaction);
	if(!DialogsOk)
	  return RetVal;

	if (TDeviceRealTerminal::Instance().BasePMS->Enabled)
	{
		PhoenixHSOk = TransRetrivePhoenixResult(PaymentTransaction);
	}
	if(!PhoenixHSOk)
	   return RetVal;


    PocketVoucher =  ProcessPocketVoucherPayment(PaymentTransaction);
    if(!PocketVoucher)
	   return RetVal;

	if(TDeviceRealTerminal::Instance().ManagerGeneralLedger->EnabledFor(PaymentTransaction))
	{
		GeneralLedgerMate = TDeviceRealTerminal::Instance().ManagerGeneralLedger->ProcessTransaction(PaymentTransaction);
		if(GeneralLedgerMate)
		{
			PaymentTransaction.References.push_back(
			RefRefType(IntToStr(TDeviceRealTerminal::Instance().ManagerGeneralLedger->LastTransactionRef),
			ManagerReference->GetReferenceByType(PaymentTransaction.DBTransaction, REFTYPE_GENERAL_LEDGER)));
		}
	}
	else
	{
		GeneralLedgerMate = true;
	}
    if(!GeneralLedgerMate)
	   return RetVal;


	if(ProcessCSVRoomExport(PaymentTransaction))
	{
		RMSCSVRoomExport = true;
	}
	else
	{
		RMSCSVRoomExport = false;
	}

    if(!RMSCSVRoomExport)
	   return RetVal;

    if(TGlobalSettings::Instance().NewBook == 2)
    {
        if(ProcessCSVNewBookExport(PaymentTransaction))
        {
            NewBookCSVRoomExport = true;
        }
        else
        {
            NewBookCSVRoomExport = false;
        }
    }
    if(!NewBookCSVRoomExport)
	   return RetVal;

    if(TGlobalSettings::Instance().LoyaltyMateEnabled)
       LoyaltyVouchers = ProcessLoyaltyVouchers(PaymentTransaction);

	RetVal = ChequesOk && EftPosOk && PhoenixHSOk && DialogsOk && PocketVoucher &&
             GeneralLedgerMate && RMSCSVRoomExport && NewBookCSVRoomExport && LoyaltyVouchers && WalletTransaction;
	return RetVal;
}
//------------------------------------------------------------------------------
bool TListPaymentSystem::ProcessChequePayment(TPaymentTransaction &PaymentTransaction)
{
   bool paymentComplete = true;
   if (PaymentTransaction.TransVerifyCheque())
	{
		for (int i = 0; i < PaymentTransaction.PaymentsCount(); i++)
		{
			TPayment *Payment = PaymentTransaction.PaymentGet(i);
			if ((Payment->GetCashOut() != 0 || Payment->GetPay() != 0) && (Payment->Result != eAccepted)
					&& Payment->GetPaymentAttribute(ePayTypeChequeVerify))
			{
				GetChequeVerifyResult(Payment);
				if (Payment->Result != eAccepted)
				{
					Payment->Failed();
					paymentComplete = false;
                    break;
				}
			}
		}
	}
   return paymentComplete;
}
//------------------------------------------------------------------------------
bool TListPaymentSystem::ProcessEftPosPayment(TPaymentTransaction &PaymentTransaction,bool &RequestEFTPOSReceipt)
{
   bool paymentComplete = true;
   if (PaymentTransaction.TransElectronicPayment())
	{
		if (PaymentTransaction.TransIntegratedEFTPOS())
		{
			RequestEFTPOSReceipt = true;
		}

		for (int i = 0; i < PaymentTransaction.PaymentsCount(); i++)
		{
			TPayment *Payment = PaymentTransaction.PaymentGet(i);
			if ((Payment->GetPaymentAttribute(ePayTypeElectronicTransaction)) && (Payment->GetCashOut() != 0 || Payment->GetPay() != 0) &&
					(Payment->Result != eAccepted))
			{
				TransRetriveElectronicResult(PaymentTransaction, Payment);
				if (Payment->Result != eAccepted)
				{
                    transactionRecovery.ClearRecoveryInfo();
					Payment->Failed();
					paymentComplete = false;
                    break;
				}
				else
				{
					if (Payment->ReferenceNumber != "")
					{
                        if(TGlobalSettings::Instance().EnableEftPosSmartPay &&
                           TDeviceRealTerminal::Instance().Modules.Status[eEFTPOS]["Registered"] &&
                           EftPos->AcquirerRefSmartPay.Length() != 0)
                           Payment->ReferenceNumber = EftPos->AcquirerRefSmartPay;
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
     return paymentComplete;
}
//------------------------------------------------------------------------------
bool TListPaymentSystem::ProcessPocketVoucherPayment(TPaymentTransaction &PaymentTransaction)
{
    bool paymentComplete = true;
    for (int i = 0; i < PaymentTransaction.PaymentsCount(); i++)
	{
		TPayment *Payment = PaymentTransaction.PaymentGet(i);
		if ( (Payment->GetPaymentAttribute(ePayTypePocketVoucher)) &&
				(Payment->GetCashOut() != 0 || Payment->GetPay() != 0) &&
				(Payment->Result != eAccepted))
		{
				Payment->ReferenceNumber = ManagerReference->BuildReference(TGlobalSettings::Instance().SiteID,
				TDeviceRealTerminal::Instance().ID.Name, REFTYPE_POCKETVOUCHER_CODE);
                TDeviceRealTerminal::Instance().PocketVouchers->ProcessVoucher(PaymentTransaction, Payment);
				if (Payment->Result != eAccepted)
				{
					Payment->Failed();
					paymentComplete = false;
                    break;
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
     return paymentComplete;
}
//------------------------------------------------------------------------------
bool TListPaymentSystem::ProcessInvoicePayment(TPaymentTransaction &PaymentTransaction)
{
    bool paymentComplete = true;
	if (PaymentTransaction.TransInvoicePayment())
	{
		for (int i = 0; i < PaymentTransaction.PaymentsCount(); i++)
		{
			TPayment *Payment = PaymentTransaction.PaymentGet(i);
			if (((Payment->GetPaymentAttribute(ePayTypeChargeToAccount)))
                 && (Payment->GetCashOut() != 0 || Payment->GetPay() != 0) && (Payment->Result != eAccepted))
			{
				TransRetriveInvoiceResult(PaymentTransaction, Payment);
				if (Payment->Result != eAccepted)
				{
					Payment->Failed();
					paymentComplete = false;
                    break;
				}
				else
				{
					if( Payment->GetPaymentAttribute(ePayTypeChargeToXero))
					{
						// Creates and sends an invoice to Xero if there is a ChargeToXero Payment
						CreateXeroInvoiceAndSend( PaymentTransaction );
					}
				}
			}
		}
	}
    return paymentComplete;
}
//------------------------------------------------------------------------------
bool TListPaymentSystem::ProcessWalletTransaction(TPaymentTransaction &PaymentTransaction)
{
    bool paymentComplete = true;
    TWalletPaymentsInterface* WalletPaymentsInterface = new TWalletPaymentsInterface();
	for (int i = 0; i < PaymentTransaction.PaymentsCount(); i++)
	{
		TPayment *Payment = PaymentTransaction.PaymentGet(i);
		if (Payment->GetPaymentAttribute(ePayTypeWallet) && Payment->GetPay() != 0 && Payment->Result != eAccepted)
		{
           TWalletTransactionResponse response = WalletPaymentsInterface->DoTransaction(*Payment);
           if(!response.IsSuccessful)
           {
              MessageBox(response.ResponseMessage, "Error", MB_OK + MB_ICONINFORMATION);
              paymentComplete = false;
              break;
           }
           else
           {
              Payment->ReferenceNumber = response.OrderReference;
           }
		}
	}
    delete WalletPaymentsInterface;
    return paymentComplete;
}
//------------------------------------------------------------------------------
bool TListPaymentSystem::ProcessLoyaltyVouchers(TPaymentTransaction &PaymentTransaction)
{
  bool paymentComplete = true;
  bool isVoucherPresent = false;
  TManagerLoyaltyVoucher ManagerLoyaltyVoucher;
  TVoucherUsageDetail VoucherUsageDetail;
  TGUID transactionReference;
  CreateGUID(transactionReference);
  SetInvoiceNumber(PaymentTransaction);
  VoucherUsageDetail.ReferenceNumber =  Sysutils::GUIDToString(transactionReference);
  VoucherUsageDetail.InvoiceNumber =  PaymentTransaction.InvoiceNumber;
  VoucherUsageDetail.TotalSaleAmount = PaymentTransaction.Money.GrandTotal;
  if(PaymentTransaction.Membership.Applied())
  {
     VoucherUsageDetail.MemberUniqueId = TLoyaltyMateUtilities::GetMemberCloudId(PaymentTransaction.DBTransaction,
                                                       PaymentTransaction.Membership.Member.ContactKey);
  }

  //Loyalty Pocket Voucher
  if(PaymentTransaction.RedeemPocketVoucherInformation->VoucherNumber != NULL &&
     PaymentTransaction.RedeemPocketVoucherInformation->VoucherNumber !="")
  {
    isVoucherPresent = true;
    VoucherUsageDetail.PocketVoucherDiscountAmount = PaymentTransaction.RedeemPocketVoucherInformation->RedeemedAmount;
    VoucherUsageDetail.PocketVoucherNumber = PaymentTransaction.RedeemPocketVoucherInformation->VoucherNumber;
  }

  //Loyalty Gift Card redemtion
  if(paymentComplete && PaymentTransaction.RedeemGiftVoucherInformation->VoucherNumber != NULL &&
     PaymentTransaction.RedeemGiftVoucherInformation->VoucherNumber !="")
  {
    isVoucherPresent = true;
    VoucherUsageDetail.PointsRedeemed = PaymentTransaction.RedeemGiftVoucherInformation->RedeemedAmount;
    VoucherUsageDetail.GiftCardNumber = PaymentTransaction.RedeemGiftVoucherInformation->VoucherNumber;
  }

  //Loyalty gift card point purchase
  if(paymentComplete && PaymentTransaction.PurchasedGiftVoucherInformation->VoucherNumber != NULL &&
     PaymentTransaction.PurchasedGiftVoucherInformation->VoucherNumber !="")
  {
    isVoucherPresent = true;
    VoucherUsageDetail.PointsPurchased = PaymentTransaction.PurchasedGiftVoucherInformation->RedeemedAmount;
    VoucherUsageDetail.PurchasedGiftCardNumber = PaymentTransaction.PurchasedGiftVoucherInformation->VoucherNumber;
  }

  //Loyalty member voucher
   if(paymentComplete && TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->RedeemedVoucherName != NULL &&
     TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->RedeemedVoucherName !="")
  {
    isVoucherPresent = true;
    VoucherUsageDetail.MemberVoucherDiscountAmount = 0;
    VoucherUsageDetail.VoucherName = TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->RedeemedVoucherName;
  }
  //Loyalty members only discounts
  for (int i = 0; i < PaymentTransaction.Orders->Count; i++)
  {
    TItemComplete *Order = (TItemComplete*)(PaymentTransaction.Orders->Items[i]);
    DISCOUNT_RESULT_LIST::iterator it_discount = Order->BillCalcResult.Discount.begin();
    for( ; it_discount != Order->BillCalcResult.Discount.end(); it_discount++ )
    {
        TDiscount CurrentDiscount;
        ManagerDiscount->GetDiscount(PaymentTransaction.DBTransaction,(*it_discount).DiscountKey,CurrentDiscount);

        double discountAmount = (*it_discount).Value;
        if(it_discount->DiscountType == BillCalculator::dtDiscount)
           discountAmount = -1.0 * discountAmount;
        if(TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->RedeemedVoucherDiscount != ""
           && CurrentDiscount.DiscountCode  == TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->RedeemedVoucherDiscount)
        {
            VoucherUsageDetail.MemberVoucherDiscountAmount += discountAmount;
        }

        if(CurrentDiscount.IsCloudDiscount && CurrentDiscount.MembersOnly && CurrentDiscount.DailyUsageAllowedPerMember > 0)
        {
           isVoucherPresent = true;

           VoucherUsageDetail.DiscountUsage[(*it_discount).DiscountCode] += discountAmount;
        }
    }
  }

  if(isVoucherPresent)
  {
    if(!PaymentTransaction.IsVouchersProcessed)
    {
         paymentComplete = ManagerLoyaltyVoucher.ProcessVouchers(VoucherUsageDetail);
    }
    PaymentTransaction.IsVouchersProcessed = paymentComplete;
    if(paymentComplete)
    {

       PaymentTransaction.PurchasedGiftVoucherInformation->ExpiryDate = VoucherUsageDetail.GiftCardExpiryDate;
       TReleasedVoucherDetail ReleasedVoucherDetail;
       ReleasedVoucherDetail.GiftCardNumber =  PaymentTransaction.RedeemGiftVoucherInformation->VoucherNumber;
       ReleasedVoucherDetail.PocketVoucherNumber =  PaymentTransaction.RedeemPocketVoucherInformation->VoucherNumber;
       ReleasedVoucherDetail.VoucherName =  TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->RedeemedVoucherName;
       ReleasedVoucherDetail.ReferenceNumber = VoucherUsageDetail.ReferenceNumber;
       if(!VoucherUsageDetail.DiscountUsage.empty())
       {
           for(std::map<AnsiString,double>::iterator itDiscount = VoucherUsageDetail.DiscountUsage.begin();
            itDiscount != VoucherUsageDetail.DiscountUsage.end(); ++itDiscount)
            {
               ReleasedVoucherDetail.DiscountUsage.push_back(itDiscount->first);
            }
       }
       paymentComplete = ManagerLoyaltyVoucher.ReleaseVouchers(ReleasedVoucherDetail);
    }
  }
  return paymentComplete;
}
//------------------------------------------------------------------------------
void TListPaymentSystem::CreateXeroInvoiceAndSend( TPaymentTransaction &inPaymentTransaction )
{
	if( TXeroInvoiceBuilder::Instance().BuildXeroInvoice( inPaymentTransaction ) )
	{
		SendInvoiceToXero( TXeroInvoiceBuilder::Instance().XeroInvoice );
	}
}
//------------------------------------------------------------------------------
bool TListPaymentSystem::SendInvoiceToXero( TXeroInvoice* inXeroInvoice )
{
	return TXeroIntegration::Instance().SendInvoice( inXeroInvoice );
}

void TListPaymentSystem::ClearDiscounts(TPaymentTransaction &PaymentTransaction)
{
	ManagerDiscount->ClearDiscounts(PaymentTransaction.Orders);
}

bool TListPaymentSystem::SplitPayment(TPaymentTransaction &PaymentTransaction, TSplitMoney *SplitMoney,
                                      double &SplitPercentage,bool &DelayAll)
{
	bool Retval = false;

	SplitMoney->TotalCost = 0;
	SplitMoney->TotalAdjustment = 0;
	for (int i = 0; i < PaymentTransaction.Orders->Count; i++)
	{
		TItemComplete *Order = (TItemComplete*)PaymentTransaction.Orders->Items[i];
		SplitMoney->TotalCost += Order->BillCalcResult.FinalPrice;//TotalPriceAdjustmentSides();
		SplitMoney->TotalAdjustment += Order->TotalAdjustmentSides();
	}

	SplitMoney->LeftOwing = 0;

	frmSplitPayment->TotalDue = SplitMoney->TotalCost;
	frmSplitPayment->DivisionsLeft = SplitMoney->DivisionsLeft;
    frmSplitPayment->TotalDivisions =  SplitMoney->TotalDivisions;
    frmSplitPayment->IsDelayedPayment = TManagerDelayedPayment::Instance().IsDelayedPayment(PaymentTransaction);
	if (frmSplitPayment->ShowModal() == mrOk)
	{
        DelayAll = frmSplitPayment->DelayAll;
		SplitMoney->DivisionsLeft = frmSplitPayment->DivisionsLeft;
		SplitMoney->PaymentAmount = frmSplitPayment->PaymentAmount;
		SplitMoney->NumberOfPayments = frmSplitPayment->NumberOfPayments;
        SplitMoney->TotalDivisions =  frmSplitPayment->TotalDivisions;

        if(SplitMoney->DivisionsLeft >= 1)
        {
            splittedDivisionLeft = true;
        }
        else if(SplitMoney->DivisionsLeft == 0)
        {
            splittedDivisionLeft = false;
        }

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
			SplitMoney->GSTContent += Order->GrandTotalExclGSTSides();
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
			if (Order->TotalAdjustment() != 0)
			{
				AnsiString Reason = Order->DiscountReasons();
				if (DiscountReason.Pos(Reason) == 0)
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
					if (SubOrder->TotalAdjustment() != 0)
					{
						AnsiString Reason = SubOrder->DiscountReasons();
						if (DiscountReason.Pos(Reason) == 0)
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
		TDeviceRealTerminal::Instance().PaymentSystem->Security->SecurityDelete
		(TDeviceRealTerminal::Instance().PaymentSystem->Security->SecurityGetType(secDiscountedBy));
		// Add Security.
		TSecurityReference *SecRef = new TSecurityReference;
		SecRef->UserKey = TDeviceRealTerminal::Instance().User.ContactKey;
		SecRef->Event = SecurityTypes[secDiscountedBy];
		SecRef->From = DiscountReason.SubString(1, 50);
		SecRef->To = CurrToStrF(PaymentTransaction.TotalAdjustment, ffCurrency, 2);
		SecRef->Note = DiscountReason.SubString(1, 50);
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
            UnicodeString name = Order->Item;
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

	double TotalCost = TOrderUtils::FinalPriceAdjustmentSides(PaymentTransaction.Orders);

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
                        //SplitPercentage = RoundToNearest(SplitPercentage,0.01,TGlobalSettings::Instance().MidPointRoundsDown);
              // In case of Patron Count On Table only , Patron Count at Zed should be shown as
             // the original value (entered by user at the time of saving on table Story Ref:- #7099 & #6889)
            if(!TGlobalSettings::Instance().PromptPatronCountOnTableSales)
            {
			    TManagerPatron::Instance().SetDefaultPatrons(PaymentTransaction.DBTransaction, PaymentTransaction.Patrons, 1);
            }
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
			std::auto_ptr <TfrmEftPos> frmEftPos(TfrmEftPos::Create <TfrmEftPos> (Screen->ActiveForm));
			if ((Payment->GetPaymentAttribute(ePayTypeCheckAccepted)) && (frmEftPos->TransactionOk(Payment) == mrNo))
			{
				Payment->Result = eDeclined;
				Payment->SetPay(0);
				Payment->SetCashOut(0);
				AllOk = false;
			}
			else if ((Payment->GetPaymentAttribute(ePayTypeCheckSignature)) && (frmEftPos->SignatureOk() == mrNo))
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
					Order->Size, Order->GetQty(), Order->MemberFreeSaleCount);
				}

				if (AddToLocationPurchaseCount)
				{
					ManagerFreebie->HasPurchased(PaymentTransaction.DBTransaction, TDeviceRealTerminal::Instance().ID.LocationKey, Order->Item,
					Order->Size, Order->GetQty(), Order->LocationFreeSaleCount);
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
						SubOrder->Size, Order->GetQty(), SubOrder->MemberFreeSaleCount);
					}

					if (AddToLocationPurchaseCount)
					{
						ManagerFreebie->HasPurchased(PaymentTransaction.DBTransaction, TDeviceRealTerminal::Instance().ID.LocationKey,
						SubOrder->Item, SubOrder->Size, Order->GetQty(), SubOrder->LocationFreeSaleCount);
					}
				}
			}
		}
	}
}

void TListPaymentSystem::ProcessRewardSchemes(TPaymentTransaction & PaymentTransaction)
{
	//
}

bool TListPaymentSystem::ProcessCSVRoomExport( TPaymentTransaction &PaymentTransaction )
{
	bool RetVal = false;
	try
	{
		for (int i = 0; i < PaymentTransaction.PaymentsCount(); i++)
		{
			TPayment *Payment = PaymentTransaction.PaymentGet(i);
			if ((Payment->GetPaymentAttribute(ePayTypeRMSInterface)) && (Payment->RMSRoom.AccountNo != -1) &&
               ((Payment->GetPay() != 0) || (Payment->GetCashOut() != 0)))
			{
				AnsiString File = ExtractFilePath(Payment->CVSWriteLocation) + "SYSNET" + Now().FormatString("ddmmyy")+ ".csv";
				TCsv Csv;
				if (FileExists(File))
				{
					Csv.LoadFromFile(File);
				}

				Csv.Add(Payment->RMSRoom.RoomNo + "," +
				IntToStr(Payment->RMSRoom.AccountNo) + "," +
				Payment->PaymentThirdPartyID + "," +
				TDeviceRealTerminal::Instance().ID.Name + " Sale ," +
				FormatFloat("0.00", Payment->GetPay() + Payment->GetCashOut() + Payment->GetAdjustment()) + "," +
				FormatDateTime("dd/mm/yy", Date())  + "," +
				FormatDateTime("HH:nn:ss", Now()));
				Csv.SaveToFile(File);
			}
		}
		RetVal = true;
	}
	catch(Exception & Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		MessageBox(Err.Message, "RMS Room Payment processing Error", MB_OK + MB_ICONERROR);
	}
	return RetVal;
}

bool TListPaymentSystem::ProcessCSVNewBookExport( TPaymentTransaction &PaymentTransaction )
{
	bool RetVal = false;
	try
	{
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        PaymentTransaction.DBTransaction.RegisterQuery(IBInternalQuery);

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_INVOICENUMBER, 0) FROM RDB$DATABASE";
        IBInternalQuery->ExecQuery();
        int	InvoiceNumber = IBInternalQuery->Fields[0]->AsInteger;
        AnsiString File;
        AnsiString roomnumber,GuestName;
		for (int i = 0; i < PaymentTransaction.PaymentsCount(); i++)
		{
			TPayment *Payment = PaymentTransaction.PaymentGet(i);
			if ((Payment->GetPaymentAttribute(ePayTypeRMSInterface)) && ((Payment->GetPay() != 0) || (Payment->GetCashOut() != 0)))
            {
                ExtractFilePath(Payment->CVSWriteLocation);
                File = Payment->CVSWriteLocation + "\\ROOM-" + Payment->NewBookRoom.RoomNo + ".csv";
                roomnumber= Payment->NewBookRoom.RoomNo;
                GuestName=Payment->NewBookRoom.Guest_Name;

                for (int i = 0; i < PaymentTransaction.Orders->Count; i++)
                {
                    TItemComplete *Order = (TItemComplete*)PaymentTransaction.Orders->Items[i];
                    String s = FloatToStrF(Order->GetQty(), ffFixed, 15, 2) ;
                    for (int i = 0; i < Order->SubOrders->Count; i++)
                    {
                        TItemCompleteSub *SubOrder = Order->SubOrders->SubOrderGet(i);
                        if (SubOrder)
                        {
                            TCsv Csv;
                        if (FileExists(File))
                        {
                            Csv.LoadFromFile(File);
                        }
                        int itemNo =  Csv.RowCount+1;
                        Csv.Add(roomnumber + "," +
                        FormatDateTime("dd/mm/yy", Date())  + "," +
                        FormatDateTime("HH:nn:ss", Now())  + "," +
                        (InvoiceNumber+1) + "," +
                        TDeviceRealTerminal::Instance().ID.Name + "," +
                        TDeviceRealTerminal::Instance().ID.Location  + "," +
                        SubOrder->ThirdPartyCode + "," +
                        SubOrder->Item +" "+ SubOrder->Size + ","+
                        itemNo + "," +
                        FloatToStrF(SubOrder->GetQty(), ffFixed, 15, 2) + "," +
                        FormatFloat("0.00", SubOrder->FinalPrice_BillCalc())

                     );
                        Csv.SaveToFile(File);
                        }
                    }

                        TCsv Csv;
                        if (FileExists(File))
                        {
                            Csv.LoadFromFile(File);
                        }
                        int itemNo =  Csv.RowCount+1;
                        Csv.Add(roomnumber + "," +
                        FormatDateTime("dd/mm/yy", Date())  + "," +
                        FormatDateTime("HH:nn:ss", Now())  + "," +
                        (InvoiceNumber+1) + "," +
                        TDeviceRealTerminal::Instance().ID.Name + "," +
                        TDeviceRealTerminal::Instance().ID.Location  + "," +
                        Order->ThirdPartyCode + "," +
                        Order->Item + " " + Order->Size  + ","+
                        itemNo + "," +
                        FloatToStrF(Order->GetQty(), ffFixed, 15, 2)  + "," +
                        FormatFloat("0.00", Order->FinalPrice_BillCalc())

                     );
                        Csv.SaveToFile(File);
                }
                int invoiceno=InvoiceNumber+1;
                if(roomnumber!="")
                {
                    PaymentTransaction.Customer.RoomNumber=  StrToInt(roomnumber);
                    PaymentTransaction.Customer.Name= GuestName;
                    TCustomer Customer(StrToInt(roomnumber), invoiceno,  GuestName);
                    TDBContacts::SaveCustomerAndNumber( PaymentTransaction.DBTransaction, Customer );
                }
                //#include "MMContactInfo.h"
                RetVal = true;
            }
        }
	}
	catch(Exception & Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		MessageBox(Err.Message, "RMS Room Payment processing Error", MB_OK + MB_ICONERROR);
	}
	return RetVal;
}

bool TListPaymentSystem::AllowsTipsOnTransactions()
{
	return EftPos->AllowsTipsOnTransactions();
}

bool TListPaymentSystem::ProcessTipOnVisaTransaction(int arcBillKey, WideString paymentRefNumber, Currency OriginalAmount, Currency tipAmount)
{
	bool retVal = false;

	if (EftPos->Enabled)
	{
		eEFTTransactionType TransType = TransactionType_TIP;
		EftPos->SetTransactionEvent(paymentRefNumber,TransType );

		// TODO: set the recovery information

		EftPos->ProcessTip(paymentRefNumber, OriginalAmount, tipAmount, "");

		if (EftPos->WaitOnEftPosEvent(paymentRefNumber))
		{
			TEftPosTransaction *EftTrans = EftPos->GetTransactionEvent(paymentRefNumber);
			if (EftTrans != NULL && EftTrans->Result == eAccepted)
			{
				InsertOrUpdateTipTransactionRecordToDB(arcBillKey,tipAmount, paymentRefNumber);
				retVal = true;
			}
		}

		// TODO: clear recovery information
	}

	return retVal;
}

// adds a record to database for tip transaction, this can be used in reports
void TListPaymentSystem::InsertOrUpdateTipTransactionRecordToDB(int arcBillKey, Currency tipAmount, WideString originalPaymentRef)
{
	try
	{
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();

		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();

		IBInternalQuery->SQL->Text = "UPDATE DAYARCBILLPAY SET SUBTOTAL=:TIP_AMOUNT "
		"WHERE ARCBILL_KEY=:ARCBILL_KEY AND PAY_TYPE=:PAY_TYPE AND PAY_TYPE_DETAILS=:PAYMENT_REF";
		IBInternalQuery->ParamByName("PAYMENT_REF")->AsString = originalPaymentRef;
		IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = arcBillKey;
		IBInternalQuery->ParamByName("PAY_TYPE")->AsString = "Tip";
		IBInternalQuery->ParamByName("TIP_AMOUNT")->AsCurrency = tipAmount;
		IBInternalQuery->ExecQuery();

		if(IBInternalQuery->RowsAffected == 0)
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_DAYARCBILLPAY, 1) FROM RDB$DATABASE";
			IBInternalQuery->ExecQuery();
			int Retval = IBInternalQuery->Fields[0]->AsInteger;
			IBInternalQuery->Close();

			IBInternalQuery->SQL->Text = "INSERT INTO DAYARCBILLPAY (" "DAYARCBILLPAY_KEY, " "ARCBILL_KEY, " "PAY_TYPE, " "VOUCHER_NUMBER, "
			"SUBTOTAL, " "ROUNDING, " "CASH_OUT, " "TAX_FREE, " "NOTE, PAY_TYPE_DETAILS," "PROPERTIES, " "GROUP_NUMBER, PAYMENT_CARD_TYPE) " "VALUES ("
			":DAYARCBILLPAY_KEY, " ":ARCBILL_KEY, " ":PAY_TYPE, " ":VOUCHER_NUMBER, " ":SUBTOTAL, " ":ROUNDING, " ":CASH_OUT, " ":TAX_FREE, "
			":NOTE, :PAY_TYPE_DETAILS," ":PROPERTIES, " ":GROUP_NUMBER , :PAYMENT_CARD_TYPE) ";

			IBInternalQuery->ParamByName("DAYARCBILLPAY_KEY")->AsInteger = Retval;
			IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = arcBillKey;
			IBInternalQuery->ParamByName("PAY_TYPE")->AsString = "Tip";
			IBInternalQuery->ParamByName("VOUCHER_NUMBER")->AsString = "";
			IBInternalQuery->ParamByName("SUBTOTAL")->AsCurrency = tipAmount;
			IBInternalQuery->ParamByName("ROUNDING")->AsCurrency = 0.0;
			IBInternalQuery->ParamByName("CASH_OUT")->AsString = "F";
			IBInternalQuery->ParamByName("TAX_FREE")->AsString = "T";
			IBInternalQuery->ParamByName("NOTE")->AsString = "";
			IBInternalQuery->ParamByName("PROPERTIES")->AsInteger = 0;
			IBInternalQuery->ParamByName("GROUP_NUMBER")->AsInteger = 0;
			IBInternalQuery->ParamByName("PAY_TYPE_DETAILS")->AsString = originalPaymentRef;
			IBInternalQuery->ParamByName("PAYMENT_CARD_TYPE")->AsString = "";
			IBInternalQuery->ExecQuery();
		}
		DBTransaction.Commit();
	}
	catch(Exception &err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, err.Message);
		//MessageBox(Err.Message, "There was an error ", MB_OK + MB_ICONERROR);
	}
}

std::vector<AnsiString> TListPaymentSystem::GetTippableCardTypes()
{
	return EftPos->GetTippableCardTypes();
}

void TListPaymentSystem::_processOrderSetTransaction( TPaymentTransaction &PaymentTransaction)
{
	if(PaymentTransaction.Orders == NULL)
	return;

	PaymentTransaction.Money.Recalc(PaymentTransaction);

	//MM-1649, If the sale type is of table then the patron count has already been asked for while selecting the table.
	//Adding the condition for allowing this only if sale type is not table seat.
	frmPaymentType->QueryPatronCount = PaymentTransaction.SalesType != eTableSeat && TGlobalSettings::Instance().PromptForPatronCount;

        //In case of quich payment check only once
        int QuickTransactionCounter = 0;

	while (!PaymentComplete && !PaymentAborted &&
           (!PaymentTransaction.IsQuickPayTransaction || (PaymentTransaction.IsQuickPayTransaction && QuickTransactionCounter < 1)))
	{
                QuickTransactionCounter++;

		 if (frmPaymentType->Execute() == mrOk)
		{
            isPaymentProcessed = true;
             bool RetrieveUserOptions = false;
             if((TGlobalSettings::Instance().IsDrinkCommandEnabled) && (PaymentTransaction.Orders->Count >0 ) && ((TItemComplete*)PaymentTransaction.Orders->Items[0])->TabName == "DC_ITEMS_ZED")
             {
                 RetrieveUserOptions = true;
             }
             else
             {
                    RetrieveUserOptions = ((PaymentTransaction.IsQuickPayTransaction && TGlobalSettings::Instance().SkipConfirmationOnFastTender)||
                          frmControlTransaction->RetrieveUserOptions() != eBack);
             }

             //if RetrieveUserOptions is true then check whether transaction has SCD or PWD Discount
             if(RetrieveUserOptions)
             {
                 if(CaptureSCDOrPWDCustomerDetails(PaymentTransaction) == mrCancel)
                    RetrieveUserOptions = false;
             }

		     if (RetrieveUserOptions)
			{
				TDeviceRealTerminal::Instance().ManagerMembership->BeginMemberTransaction();
  			    PaymentTransaction.ProcessPoints();
				TDeviceRealTerminal::Instance().ProcessingController.PushOnce(State);
				PaymentComplete = ProcessThirdPartyModules(PaymentTransaction, RequestEFTPOSReceipt);
                /***Send Requests to Thor*****/
                /////////////////////////////////////////////////
                if(PaymentComplete && TGlobalSettings::Instance().IsThorlinkEnabled)
                {
                    PaymentComplete = PrepareThorRequest(PaymentTransaction);
                }

				if (PaymentComplete)
				{
					if (PaymentTransaction.CreditTransaction)
					ReturnOrderedItemsToStock(PaymentTransaction);

					if (Screen->ActiveForm != NULL)
					Screen->ActiveForm->Repaint();

					TDeviceRealTerminal::Instance().ProcessingController.Repaint();
                    if(TManagerDelayedPayment::Instance().IsDelayedPayment(PaymentTransaction) &&
                       !IsAllItemSelected(PaymentTransaction))
                       {
                         TManagerDelayedPayment::Instance().MoveOrderToTab(PaymentTransaction,false);
                       }
					UpdateFreebieRewards(PaymentTransaction);
					SetInvoiceNumber(PaymentTransaction);
					//MM-2277 Calculate tier level for user
					CalculateTierLevel(PaymentTransaction);
                    CheckSubscription(PaymentTransaction);
                    TDeviceRealTerminal::Instance().ManagerMembership->ProcessPoints(PaymentTransaction);
					ReceiptPrepare(PaymentTransaction, RequestEFTPOSReceipt);
					StoreInfo(PaymentTransaction);
					ProcessRewardSchemes(PaymentTransaction);
					ArchiveTransaction(PaymentTransaction);
					if(PaymentTransaction.TypeOfSale == RegularSale)
					{
						exportTransactionInformation( PaymentTransaction ); // update export tables on going through tender screen
					}

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
			PaymentAborted = true;
            isPaymentProcessed = false;
		}
    }
}
//------------------------------------------------------------------------------
void TListPaymentSystem::_processSplitPaymentTransaction( TPaymentTransaction &PaymentTransaction )
{
	// Tabs from which the orders come from
	std::set <__int64> TabKeys;
     std::vector<TPatronType>::iterator it = PaymentTransaction.Patrons.begin();
	TDBOrder::GetTabKeysFromOrders(PaymentTransaction.Orders, TabKeys);
	std::set <__int64> ::iterator itTabs = TabKeys.begin();
	if (itTabs != TabKeys.end())
	{
	    PaymentTransaction.SplitMoney.DivisionsLeft = TDBTab::GetSplitDivisions(PaymentTransaction.DBTransaction, *itTabs);
            PaymentTransaction.SplitMoney.TotalDivisions = TDBTab::GetTotalDivisions(PaymentTransaction.DBTransaction, *itTabs);
	}

        std::auto_ptr <TList> ClonedOrdersList(new TList);
        TItemComplete *SplittedItem = new TItemComplete();
        double SplitPercentage = 0;
        bool DelayAll = false;
        bool result = SplitPayment(PaymentTransaction, &PaymentTransaction.SplitMoney, SplitPercentage,DelayAll);

        if(DelayAll)
        {
           DelayAllPayments(PaymentTransaction);
        }
        else if (result)
	     {
            if (PaymentTransaction.SplitMoney.LeftOwing != 0)
            {
               TDBOrder::UpdateOrdersForPartialPayment(PaymentTransaction.DBTransaction, PaymentTransaction.Orders,
                    ClonedOrdersList.get(), SplittedItem, PaymentTransaction.SplitMoney.PaymentAmount);

                 PaymentTransaction.Orders->Clear();

                for(int i = 0; i < ClonedOrdersList->Count; i++)
                {
                    TItemComplete *Order = (TItemComplete *)ClonedOrdersList->Items[i];
                    PaymentTransaction.Orders->Add(Order);
                }
            }
            PaymentTransaction.RequestPartialPayment = PaymentTransaction.SplitMoney.PaymentAmount;
            PaymentTransaction.Money.Recalc(PaymentTransaction);


           if(it != PaymentTransaction.Patrons.end())
           {
               if(it->Count == 0)
               {
                  TManagerPatron::Instance().SetDefaultPatrons(PaymentTransaction.DBTransaction, PaymentTransaction.Patrons,
            PaymentTransaction.SplitMoney.NumberOfPayments);
               }
            }

            // Load points.
            PaymentsReload(PaymentTransaction);

            while (!PaymentComplete && !PaymentAborted)
             {
                if (frmPaymentType->Execute() == mrOk)
                {
                    isPaymentProcessed = true;
                    bool retrieveUserOptions = frmControlTransaction->RetrieveUserOptions() != eBack;

                    //if RetrieveUserOptions is true then check whether transaction has SCD or PWD Discount
                    if(retrieveUserOptions)
                    {
                        if(CaptureSCDOrPWDCustomerDetails(PaymentTransaction) == mrCancel)
                            retrieveUserOptions = false;
                    }

                    if (retrieveUserOptions)
                    {
                        TDeviceRealTerminal::Instance().ManagerMembership->BeginMemberTransaction();
                        PaymentTransaction.ProcessPoints();
                        TDeviceRealTerminal::Instance().ProcessingController.PushOnce(State);
                        PaymentComplete = ProcessThirdPartyModules(PaymentTransaction, RequestEFTPOSReceipt);
                        /////////////////////////////////////////////////
                        /***Send Requests to Thor*****/
                        /////////////////////////////////////////////////
                        if(PaymentComplete && TGlobalSettings::Instance().IsThorlinkEnabled)
                        {
                            PaymentComplete = PrepareThorRequest(PaymentTransaction);
                        }

                        if (PaymentComplete)
                        {
                            if (Screen->ActiveForm != NULL)
                                Screen->ActiveForm->Repaint();
                            TDeviceRealTerminal::Instance().ProcessingController.Repaint();
                            if(TManagerDelayedPayment::Instance().IsDelayedPayment(PaymentTransaction) && ((!IsAllItemSelected(PaymentTransaction)) ||
                                (IsAllItemSelected(PaymentTransaction) && SplittedItem->OrderKey > 0)))
                               {
                                 TManagerDelayedPayment::Instance().MoveOrderToTab(PaymentTransaction,false);
                               }
                            UpdateFreebieRewards(PaymentTransaction);
                            SetInvoiceNumber(PaymentTransaction);
                            //MM-2277 Calculate tier level for user
                            CalculateTierLevel(PaymentTransaction);
                            CheckSubscription(PaymentTransaction);
                            TDeviceRealTerminal::Instance().ManagerMembership->ProcessPoints(PaymentTransaction);
                            ReceiptPrepare(PaymentTransaction, RequestEFTPOSReceipt);
                            StoreInfo(PaymentTransaction);
                            ProcessRewardSchemes(PaymentTransaction);
                            ArchiveTransaction(PaymentTransaction);
                            if(PaymentTransaction.TypeOfSale == RegularSale)
                             {
                               exportTransactionInformation( PaymentTransaction ); // update export tables on going through tender screen
                             }
                            BuildXMLTransaction(PaymentTransaction);
                            OpenCashDrawer(PaymentTransaction);
                            if(SplittedItem->OrderKey > 0)
                            {
                              //Calculate DWT , Tax on discount on remaining quantities
                                TPaymentTransaction RemainingOrderTransaction(PaymentTransaction.DBTransaction);
                                RemainingOrderTransaction.Orders->Add(SplittedItem);
                                RemainingOrderTransaction.Recalc();

                                SplittedItem->OrderKey = 0;
                                SplittedItem->TransNo = TDBOrder::GetNextTransNumber(PaymentTransaction.DBTransaction);
                                if (SplittedItem->ServingCourse.ServingCourseKey < 1)
                                {
                                        SplittedItem->ServingCourse = TDeviceRealTerminal::Instance().Menus->DefaultServingCourse;
                                        TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, "Order has no Serving Course");
                                }

                                TDBOrder::SetOrder(PaymentTransaction.DBTransaction, SplittedItem);
                                PaymentTransaction.SplittedItemKey = SplittedItem->OrderKey;
                                TDBSecurity::ProcessSecurity(PaymentTransaction.DBTransaction, SplittedItem->Security);
                                for (int i = 0; i < SplittedItem->SubOrders->Count; i++)
                                {
                                        TItemCompleteSub *SubOrder = SplittedItem->SubOrders->SubOrderGet(i);
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
                                    TDBTab::SetSplitDivisions(PaymentTransaction.DBTransaction, *itTabs,
                                    PaymentTransaction.SplitMoney.DivisionsLeft);
                                    TDBTab::SetTotalDivisions(PaymentTransaction.DBTransaction, *itTabs,
                                    PaymentTransaction.SplitMoney.TotalDivisions);
                            }
                           TItemComplete *Order = (TItemComplete *)PaymentTransaction.Orders->Items[0];
                           if(Order && Order->TableNo > 0)
                           {
                             int patronCount = TDBTables::GetPatronNumbers(PaymentTransaction.DBTransaction,Order->TableNo);
                             patronCount = patronCount - PaymentTransaction.SplitMoney.NumberOfPayments;
                             if(patronCount <= 0)
                              patronCount = 1;
                             TDBTables::SetPatronNumbers(PaymentTransaction.DBTransaction,Order->TableNo,patronCount);
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
                        frmSplitPayment->DivisionsLeft = 0;
                        PaymentAborted = true;
                        isPaymentProcessed = false;
                }
            }
	}
	else
	{
		PaymentAborted = true;
	}
}
//------------------------------------------------------------------------------
void TListPaymentSystem::_processPartialPaymentTransaction( TPaymentTransaction &PaymentTransaction )
{
	/* TODO :
	Clone the orders and discount them down to the payment amount.
	Add a discount tothe old order of this amount is it's successful */
    std::auto_ptr <TList> OrdersList(new TList);
    TItemComplete *SplittedItem = new TItemComplete();
	double SplitPercentage = 0;
	Currency RequestedTotal;
	double PaymentReturnVal = GetPartialPayment(PaymentTransaction, SplitPercentage, RequestedTotal);
	if (PaymentReturnVal > 0)
	{

		// They may be partially paying for the entire Bill in which case PaymentReturnVal == 2;
		if (PaymentReturnVal == 1)
		{
            //Select on those items whose price falls in requested total
            TDBOrder::UpdateOrdersForPartialPayment(PaymentTransaction.DBTransaction, PaymentTransaction.Orders,
            OrdersList.get(), SplittedItem, RequestedTotal);
            PaymentTransaction.Orders->Clear();
            for(int i = 0; i < OrdersList->Count; i++)
             {
                TItemComplete *Order = (TItemComplete *)OrdersList->Items[i];
                PaymentTransaction.Orders->Add(Order);
             }
			PaymentTransaction.RequestPartialPayment = RequestedTotal;
		}
		PaymentsReload(PaymentTransaction);

		while (!PaymentComplete && !PaymentAborted)
		{
			if (frmPaymentType->Execute() == mrOk)
			{
                bool retrieveUserOptions = frmControlTransaction->RetrieveUserOptions() != eBack;

                //if RetrieveUserOptions is true then check whether transaction has SCD or PWD Discount
                if(retrieveUserOptions)
                {
                    if(CaptureSCDOrPWDCustomerDetails(PaymentTransaction) == mrCancel)
                        retrieveUserOptions = false;
                }

				if (retrieveUserOptions)
				{
                    isPaymentProcessed = true;
					TDeviceRealTerminal::Instance().ManagerMembership->BeginMemberTransaction();
					PaymentTransaction.ProcessPoints();
					TDeviceRealTerminal::Instance().ProcessingController.PushOnce(State);
					PaymentComplete = ProcessThirdPartyModules(PaymentTransaction, RequestEFTPOSReceipt);
                    /////////////////////////////////////////////////
                    /***Send Requests to Thor*****/
                    /////////////////////////////////////////////////
                    if(PaymentComplete && TGlobalSettings::Instance().IsThorlinkEnabled)
                    {
                        PaymentComplete = PrepareThorRequest(PaymentTransaction);
                    }

					if (PaymentComplete)
					{
						if (Screen->ActiveForm != NULL)
						   Screen->ActiveForm->Repaint();
						TDeviceRealTerminal::Instance().ProcessingController.Repaint();
                       if(TManagerDelayedPayment::Instance().IsDelayedPayment(PaymentTransaction) &&
                           ((!IsAllItemSelected(PaymentTransaction)) ||
                            (IsAllItemSelected(PaymentTransaction) && SplittedItem->OrderKey > 0)))
                           {
                             TManagerDelayedPayment::Instance().MoveOrderToTab(PaymentTransaction,false);
                           }
						UpdateFreebieRewards(PaymentTransaction);
						SetInvoiceNumber(PaymentTransaction);
						//MM-2277 Calculate tier level for user
						CalculateTierLevel(PaymentTransaction);
                        CheckSubscription(PaymentTransaction);
                        TDeviceRealTerminal::Instance().ManagerMembership->ProcessPoints(PaymentTransaction);
						ReceiptPrepare(PaymentTransaction, RequestEFTPOSReceipt);
						StoreInfo(PaymentTransaction);
						ProcessRewardSchemes(PaymentTransaction);
						ArchiveTransaction(PaymentTransaction);
                        if(PaymentTransaction.TypeOfSale == RegularSale)
                         {
						   exportTransactionInformation( PaymentTransaction ); // update export tables on going through tender screen
                         }
						BuildXMLTransaction(PaymentTransaction);
                        OpenCashDrawer(PaymentTransaction);

                        if(SplittedItem->OrderKey > 0)
                        {
                            TPaymentTransaction RemainingOrderTransaction(PaymentTransaction.DBTransaction);
                            RemainingOrderTransaction.Orders->Add(SplittedItem);
                            RemainingOrderTransaction.Recalc();
                            // Save off the cloned orders with whats left of the partial payment.
                            //insert only splitted order because it's quantity is changed
                            SplittedItem->OrderKey = 0;
                            SplittedItem->TransNo = TDBOrder::GetNextTransNumber(PaymentTransaction.DBTransaction);
                            if (SplittedItem->ServingCourse.ServingCourseKey < 1)
                            {
                                    SplittedItem->ServingCourse = TDeviceRealTerminal::Instance().Menus->DefaultServingCourse;
                                    TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, "Order has no Serving Course");
                            }

                            TDBOrder::SetOrder(PaymentTransaction.DBTransaction, SplittedItem);
                            TDBSecurity::ProcessSecurity(PaymentTransaction.DBTransaction, SplittedItem->Security);
                            for (int i = 0; i < SplittedItem->SubOrders->Count; i++)
                            {
                                    TItemCompleteSub *SubOrder = SplittedItem->SubOrders->SubOrderGet(i);
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
					PaymentTransaction.PaymentsReset();
					frmPaymentType->EnableElectronicPayments(); //clears all PaymentTypes and reloads them.
				}
			}
			else
			{
				//ClearDiscounts(PaymentTransaction);
                PaymentComplete = false;
				PaymentAborted = true;
                isPaymentProcessed = false;
			}
		}

        if( (!PaymentAborted) && PaymentReturnVal == 2)
        {
              isClippTabFullyPaid = true;
        }
	}
}
//------------------------------------------------------------------------------
void TListPaymentSystem::_processQuickTransaction( TPaymentTransaction &PaymentTransaction )
{
        if( PaymentTransaction.PaymentsCount() == 0 )
        return;

        PaymentTransaction.Money.Recalc(PaymentTransaction);
        TDeviceRealTerminal::Instance().ManagerMembership->BeginMemberTransaction();
        PaymentTransaction.ProcessPoints();
        if(!IsClippSale)
        {
            TDeviceRealTerminal::Instance().ProcessingController.PushOnce(State);
        }
        PaymentComplete = ProcessThirdPartyModules(PaymentTransaction, RequestEFTPOSReceipt);
        /////////////////////////////////////////////////
        /***Send Requests to Thor*****/
        /////////////////////////////////////////////////
        if(PaymentComplete && TGlobalSettings::Instance().IsThorlinkEnabled)
        {
            PaymentComplete = PrepareThorRequest(PaymentTransaction);
        }

         //if payment complete is true then check whether transaction has SCD or PWD Discount
        if(PaymentComplete)
        {
            if(CaptureSCDOrPWDCustomerDetails(PaymentTransaction) == mrCancel)
                        PaymentComplete = false;
        }

        if (PaymentComplete)
        {
            if (Screen->ActiveForm != NULL)
            Screen->ActiveForm->Repaint();

            TDeviceRealTerminal::Instance().ProcessingController.Repaint();

            UpdateFreebieRewards(PaymentTransaction);
            SetInvoiceNumber(PaymentTransaction);
            //MM-2277 Calculate tier level for user
            CalculateTierLevel(PaymentTransaction);
            CheckSubscription(PaymentTransaction);
            TDeviceRealTerminal::Instance().ManagerMembership->ProcessPoints(PaymentTransaction);
            ReceiptPrepare(PaymentTransaction, RequestEFTPOSReceipt);
            StoreInfo(PaymentTransaction);
            ProcessRewardSchemes(PaymentTransaction);
            ArchiveTransaction(PaymentTransaction);

            if(PaymentTransaction.TypeOfSale == RegularSale)
            {
                exportTransactionInformation( PaymentTransaction );     // update export tables on cash sales
            }

            BuildXMLTransaction(PaymentTransaction);
            OpenCashDrawer(PaymentTransaction);
            RemoveOrders(PaymentTransaction);
        }

}
//------------------------------------------------------------------------------
void TListPaymentSystem::_processCreditTransaction( TPaymentTransaction &PaymentTransaction )
{
	while (!PaymentComplete && !PaymentAborted)
	{
		if (frmPaymentType->Execute() == mrOk)
		{
            bool retrieveUserOptions = frmControlTransaction->RetrieveUserOptions() != eBack;

            //if RetrieveUserOptions is true then check whether transaction has SCD or PWD Discount
            if(retrieveUserOptions)
            {
                if(CaptureSCDOrPWDCustomerDetails(PaymentTransaction) == mrCancel)
                    retrieveUserOptions = false;
            }

            if (retrieveUserOptions)
            {
				TDeviceRealTerminal::Instance().ManagerMembership->BeginMemberTransaction();
				PaymentTransaction.ProcessPoints();
				TDeviceRealTerminal::Instance().ProcessingController.PushOnce(State);
				PaymentComplete = ProcessThirdPartyModules(PaymentTransaction, RequestEFTPOSReceipt);
                /////////////////////////////////////////////////
                /***Send Requests to Thor*****/
                /////////////////////////////////////////////////
                if(PaymentComplete && TGlobalSettings::Instance().IsThorlinkEnabled)
                {
                    PaymentComplete = PrepareThorRequest(PaymentTransaction);
                }

				if (PaymentComplete)
				{
					if (Screen->ActiveForm != NULL)
					Screen->ActiveForm->Repaint();

					TDeviceRealTerminal::Instance().ProcessingController.Repaint();

					UpdateFreebieRewards(PaymentTransaction);
					SetInvoiceNumber(PaymentTransaction);
					//MM-2277 Calculate tier level for user
			     	CalculateTierLevel(PaymentTransaction);
                    CheckSubscription(PaymentTransaction);
                    TDeviceRealTerminal::Instance().ManagerMembership->ProcessPoints(PaymentTransaction);
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
			//ClearDiscounts(PaymentTransaction);
			PaymentAborted = true;
		}
	}
}
//------------------------------------------------------------------------------
void TListPaymentSystem::_processEftposRecoveryTransaction( TPaymentTransaction &PaymentTransaction )
{
	if(PaymentTransaction.Orders == NULL)
	return;

	PaymentTransaction.Money.Recalc(PaymentTransaction);

	bool SkipUserOptions = true;
	frmControlTransaction->UserOption = eCloseandPrint;

	while (!PaymentComplete && !PaymentAborted)
	{
		if (frmPaymentType->Execute() == mrOk)
		{
            bool retrieveUserOptions = frmControlTransaction->RetrieveUserOptions() != eBack;

            //if RetrieveUserOptions is true then check whether transaction has SCD or PWD Discount
            if(retrieveUserOptions)
            {
                if(CaptureSCDOrPWDCustomerDetails(PaymentTransaction) == mrCancel)
                    retrieveUserOptions = false;
            }
            retrieveUserOptions = SkipUserOptions || retrieveUserOptions;
            if (retrieveUserOptions)
            {
				TDeviceRealTerminal::Instance().ManagerMembership->BeginMemberTransaction();
				PaymentTransaction.ProcessPoints();
				TDeviceRealTerminal::Instance().ProcessingController.PushOnce(State);
				PaymentComplete = ProcessThirdPartyModules(PaymentTransaction, RequestEFTPOSReceipt);
                /////////////////////////////////////////////////
                /***Send Requests to Thor*****/
                /////////////////////////////////////////////////
                if(PaymentComplete && TGlobalSettings::Instance().IsThorlinkEnabled)
                {
                    PaymentComplete = PrepareThorRequest(PaymentTransaction);
                }

				if (PaymentComplete)
				{
					if (Screen->ActiveForm != NULL)
					Screen->ActiveForm->Repaint();

					TDeviceRealTerminal::Instance().ProcessingController.Repaint();

					UpdateFreebieRewards(PaymentTransaction);
					SetInvoiceNumber(PaymentTransaction);
					//MM-2277 Calculate tier level for user
                    CalculateTierLevel(PaymentTransaction);
                    CheckSubscription(PaymentTransaction);
                    TDeviceRealTerminal::Instance().ManagerMembership->ProcessPoints(PaymentTransaction);
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
			//ClearDiscounts(PaymentTransaction);
			PaymentAborted = true;
		}
	}
}
//------------------------------------------------------------------------------
void TListPaymentSystem::_processRewardsRecoveryTransaction( TPaymentTransaction &PaymentTransaction )
{
	if(PaymentTransaction.Orders == NULL)
	return;

	PaymentTransaction.Money.Recalc(PaymentTransaction);

	bool SkipUserOptions = true;
	frmControlTransaction->UserOption = eCloseandPrint;

	while (!PaymentComplete && !PaymentAborted)
	{
		if (frmPaymentType->Execute() == mrOk)
		{
            bool retrieveUserOptions = frmControlTransaction->RetrieveUserOptions() != eBack;

            //if RetrieveUserOptions is true then check whether transaction has SCD or PWD Discount
            if(retrieveUserOptions)
            {
                if(CaptureSCDOrPWDCustomerDetails(PaymentTransaction) == mrCancel)
                retrieveUserOptions = false;
            }
            retrieveUserOptions = SkipUserOptions || retrieveUserOptions;

            if (retrieveUserOptions)
            {
				TDeviceRealTerminal::Instance().ManagerMembership->BeginMemberTransaction();
				PaymentTransaction.ProcessPoints();
				TDeviceRealTerminal::Instance().ProcessingController.PushOnce(State);
				PaymentComplete = ProcessThirdPartyModules(PaymentTransaction, RequestEFTPOSReceipt);
                /////////////////////////////////////////////////
                /***Send Requests to Thor*****/
                /////////////////////////////////////////////////
                if(PaymentComplete && TGlobalSettings::Instance().IsThorlinkEnabled)
                {
                    PaymentComplete = PrepareThorRequest(PaymentTransaction);
                }

				if (PaymentComplete)
				{
					if (Screen->ActiveForm != NULL)
					{
						Screen->ActiveForm->Repaint();
					}
					TDeviceRealTerminal::Instance().ProcessingController.Repaint();
					UpdateFreebieRewards(PaymentTransaction);
					SetInvoiceNumber(PaymentTransaction);
					//MM-2277 Calculate tier level for user
                    CalculateTierLevel(PaymentTransaction);
                    CheckSubscription(PaymentTransaction);
                    TDeviceRealTerminal::Instance().ManagerMembership->ProcessPoints(PaymentTransaction);
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
			//ClearDiscounts(PaymentTransaction);
			PaymentAborted = true;
		}
	}
}
//------------------------------------------------------------------------------
TMMProcessingState TListPaymentSystem::_createProcessingStateMessage()
{
	TMMProcessingState State(Screen->ActiveForm, "Processing Bill", "Processing Bill");

	if (_isSmartCardPresent())
	{
		State.Message = "Do NOT remove smartcard. Please Wait.";
		State.Title = "Do NOT remove smartcard.";
	}

	return State;
}
//------------------------------------------------------------------------------
bool TListPaymentSystem::_isSmartCardPresent()
{
	TMMContactInfo TempUserInfo;
	TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->GetContactInfo(TempUserInfo);
	return TempUserInfo.Valid();
}
//------------------------------------------------------------------------------
void TListPaymentSystem::loadPaymentTypeGroupsForPaymentType( int paymentDbKey, TPayment &payment )
{
	TPaymentTypeGroupsManager paymentGroupsManager;
	std::vector<TPaymentTypeGroup> assignedGroups;

	assignedGroups.clear();
	paymentGroupsManager.LoadAssignedGroupsByType( paymentDbKey, assignedGroups );

	payment.SetAssignedGroups( assignedGroups );
}
//------------------------------------------------------------------------------
TPaymentTypeGroup TListPaymentSystem::getDefaultCashPaymentGroup()
{
	TPaymentTypeGroup group(-1);

	TPaymentTypeGroupsManager paymentGroupsManager;
	group = paymentGroupsManager.GetPaymentGroupByName( CASH_PAYMENT_GROUP );

	return group;
}
//------------------------------------------------------------------------------
void TListPaymentSystem::exportTransactionInformation( TPaymentTransaction &paymentTransaction )
{
	updateMallExportTables( paymentTransaction );
	generateTransactionExportFile();
}
//------------------------------------------------------------------------------
void TListPaymentSystem::updateMallExportTables( TPaymentTransaction &paymentTransaction )
{
	TMallExportUpdateAdaptor exportUpdateAdaptor;
	TMallExportHourlyUpdate exportHourlyUpdate;
	TMallExportTransactionUpdate exportTransactionUpdate;
//    TMallExportOtherDetailsUpdate exportOtherDetailsUpdate;
	if(TGlobalSettings::Instance().MallIndex == POWERPLANTMALL)
	{
		int Time = Now().FormatString("hh").ToInt();
		int PreviousTime = TGlobalSettings::Instance().PPlantPrevTime;
		if((Time != PreviousTime) && (TGlobalSettings::Instance().PPlantHourly == 1)) {
			// MallExport Table needs to be reset because if the timing did not reset the table
			// the value that will be added in the text file in a new line will be accumulated
			// by the previous data
			exportUpdateAdaptor.ResetExportTables();
		}
	}

    // This code is used to make mall export Hourly file and Transaction file not working if the mall export is not
    // being set. This will prevent huge database file that is not needed.
	if(TGlobalSettings::Instance().MallIndex != 0&&TGlobalSettings::Instance().MallIndex != 9)
    {
     	TExportUpdateResponse response = exportUpdateAdaptor.UpdateExportTablesOnTransaction( &paymentTransaction );
    	TExportUpdateResponse responsehourly = exportHourlyUpdate.UpdateHourlyExportTablesOnTransaction( &paymentTransaction );

//        if(TGlobalSettings::Instance().MallIndex == FEDERALLANDMALL)
//        {
//            TExportUpdateResponse responseotherdetails = exportOtherDetailsUpdate.UpdateOtherDetailsExportTablesOnTransaction( &paymentTransaction );
//        }

//    	if(TGlobalSettings::Instance().MallIndex == ALPHALANDMALL || TGlobalSettings::Instance().MallIndex == FEDERALLANDMALL)
//        {
//            TExportUpdateResponse responsetransaction = exportTransactionUpdate.UpdateTransactionExportTablesOnTransaction( &paymentTransaction );
//        }
    	if(TGlobalSettings::Instance().MallIndex == ALPHALANDMALL)
        {
            TExportUpdateResponse responsetransaction = exportTransactionUpdate.UpdateTransactionExportTablesOnTransaction( &paymentTransaction );
        }

    	if(!response.Successful)
	    {
		    MessageBox(response.Description + " : " + response.Message, "MallExport Unsuccessful",
    		MB_OK + MB_ICONERROR);
	    }
    }
}
//------------------------------------------------------------------------------
void TListPaymentSystem::generateTransactionExportFile()
{
	if (TGlobalSettings::Instance().MallIndex != 0&&TGlobalSettings::Instance().MallIndex != 9)
	{
		std::auto_ptr<TMallExportManager> MEM(new TMallExportManager());
		MEM->IMallManager->TransactionExport();
	}
}

int TListPaymentSystem::GetPaymentTabName(Database::TDBTransaction &DBTransaction,AnsiString PAYMENT_NAME)
{
	int RetVal = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT  a.TABKEY "
		"FROM PAYMENTTYPES a "
		"where a.PAYMENT_NAME=:PAYMENT_NAME" ;

		IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = PAYMENT_NAME;
		IBInternalQuery->ExecQuery();

		if(IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("TABKEY")->AsInteger;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return RetVal;
}
//------------------------------------------------------------------------------
bool TListPaymentSystem::IsAllItemSelected(TPaymentTransaction &PaymentTransaction)
{
    bool AllSelected = true;
    if(PaymentTransaction.Orders->Count > 0)
    {
        TItemComplete *Order = (TItemComplete*)PaymentTransaction.Orders->Items[0];
        int tabKey = Order->TabKey;
        std::set <__int64> TabItems;
        std::set <__int64> BilledItems;
        TDBTab::GetOrderKeys(PaymentTransaction.DBTransaction, tabKey, TabItems);

        for(int i=0;i< PaymentTransaction.Orders->Count; i++)
        {
           TItemComplete *Item = (TItemComplete*)PaymentTransaction.Orders->Items[i];
           BilledItems.insert(Item->OrderKey);
        }

        for (std::set <__int64> ::iterator itItem = TabItems.begin(); itItem != TabItems.end(); advance(itItem, 1))
        {
            if (BilledItems.find(*itItem) == BilledItems.end())
            {
                AllSelected = false;
                break;
            }
        }
    }
    return AllSelected;
}
//------------------------------------------------------------------------------
void TListPaymentSystem::DelayAllPayments(TPaymentTransaction &PaymentTransaction)
{
  TItemComplete *Order = (TItemComplete *)PaymentTransaction.Orders->Items[0];
  std::set <__int64> TabKeys;
  TabKeys.insert(Order->TabKey);
  Currency SplitAmount = PaymentTransaction.SplitMoney.PaymentAmount/PaymentTransaction.SplitMoney.NumberOfPayments;
  for(int i = 0; i < PaymentTransaction.SplitMoney.DivisionsLeft - 1; i++)
  {
     PaymentTransaction.Orders->Clear();
     TDBOrder::GetOrdersFromTabKeys(PaymentTransaction.DBTransaction,PaymentTransaction.Orders,TabKeys);
     PaymentTransaction.Recalc();
     int count = PaymentTransaction.Orders->Count;
    TItemComplete *SplittedItem = new TItemComplete();
    std::auto_ptr <TList> ClonedOrdersList(new TList);
    TDBOrder::UpdateOrdersForPartialPayment(PaymentTransaction.DBTransaction, PaymentTransaction.Orders,
                    ClonedOrdersList.get(), SplittedItem,SplitAmount);
    TPaymentTransaction SplitTransaction(PaymentTransaction.DBTransaction);
    for(int i = 0; i < ClonedOrdersList->Count; i++)
    {
        TItemComplete *Order = (TItemComplete *)ClonedOrdersList->Items[i];
        SplitTransaction.Orders->Add(Order);
    }
   SplitTransaction.Recalc();
   TManagerDelayedPayment::Instance().MoveOrderToTab(SplitTransaction,false);
   if(SplittedItem->OrderKey > 0)
    {
      //Calculate DWT , Tax on discount on remaining quantities
        TPaymentTransaction RemainingOrderTransaction(PaymentTransaction.DBTransaction);
        RemainingOrderTransaction.Orders->Add(SplittedItem);
        RemainingOrderTransaction.Recalc();
        SplittedItem->OrderKey = 0;
        SplittedItem->TransNo = TDBOrder::GetNextTransNumber(PaymentTransaction.DBTransaction);
        if (SplittedItem->ServingCourse.ServingCourseKey < 1)
        {
            SplittedItem->ServingCourse = TDeviceRealTerminal::Instance().Menus->DefaultServingCourse;
            TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, "Order has no Serving Course");
        }

        TDBOrder::SetOrder(PaymentTransaction.DBTransaction, SplittedItem);
        TDBSecurity::ProcessSecurity(PaymentTransaction.DBTransaction, SplittedItem->Security);
        for (int i = 0; i < SplittedItem->SubOrders->Count; i++)
        {
                TItemCompleteSub *SubOrder = SplittedItem->SubOrders->SubOrderGet(i);
                if (SubOrder)
                {
                        TDBSecurity::ProcessSecurity(PaymentTransaction.DBTransaction, SubOrder->Security);
                }
        }
    }

  }
}

void TListPaymentSystem::LoadClippPaymentTypes(TPaymentTransaction &paymentTransaction)
{
    //Creating New Payment Type Clipp
    TPayment* clippPayment = new TPayment;
    clippPayment->Name = "Clipp";
    clippPayment->SysNameOveride = "Clipp";
    clippPayment->SetPaymentAttribute(ePayTypeClipp);
    clippPayment->DisplayOrder = 1;
    clippPayment->GroupNumber = -999;
    clippPayment->Colour = clTeal;
    clippPayment->PaymentThirdPartyID = "10007243";
    clippPayment->Visible = false;
    paymentTransaction.PaymentAdd(clippPayment);
}

bool TListPaymentSystem::PrepareThorRequest(TPaymentTransaction &paymentTransaction)
{
    bool retValue = false;
    if(paymentTransaction.Money.Total > 0)
        retValue = PrepareThorPurchaseRequest(paymentTransaction);
    else
        if(paymentTransaction.Membership.Member.Name.Length() != 0)
            retValue = PrepareThorRefundRequest(paymentTransaction);
        else
            retValue = true;
    return retValue;
}

bool TListPaymentSystem::PrepareThorPurchaseRequest(TPaymentTransaction &paymentTransaction)
{
    bool returnValue = true;
    TItemDetailsThor itemThor ;
    TTenderDetails tenderDetails;
    tenderDetails.sendTransactionValue = true;
    tenderDetailsList.erase(tenderDetailsList.begin(),tenderDetailsList.end());
    itemsList.erase(itemsList.begin(),itemsList.end());

    for(int i = 0 ; i < paymentTransaction.Orders->Count ; i++)
    {
        TItemComplete *itemComplete = (TItemComplete*)paymentTransaction.Orders->Items[i];
        itemThor.thirdPartyCode = itemComplete->ThirdPartyCode;
        double price = itemComplete->BillCalcResult.FinalPrice/itemComplete->GetQty();
        itemThor.unitPrice = price;
        itemThor.qty = itemComplete->GetQty();
        itemsList.push_back(itemThor);
        if(itemComplete->DiscountReason.Length() != 0)
        {
            tenderDetails.tenderType = eThorVoucher;
            tenderDetails.tenderIdentifier = "0";
            for(std::vector<TDiscount>::iterator i = itemComplete->Discounts.begin() ; i != itemComplete->Discounts.end() ; ++i)
            {
                if(i->VoucherCode.Length() != 0)
                {
                    tenderDetails.tenderIdentifier = i->VoucherCode;
                }
            }
            tenderDetails.cardNo = paymentTransaction.Membership.Member.CardStr;
            tenderDetails.tenderValue = 0;
        }
        if(itemComplete->SubOrders->Count > 0)
        {
            for(int subOrdersCount = 0 ; subOrdersCount < itemComplete->SubOrders->Count; subOrdersCount++)
            {
                TItemComplete *subItem = (TItemComplete*)itemComplete->SubOrders->Items[subOrdersCount];
                itemThor.thirdPartyCode = subItem->ThirdPartyCode;
                double price = subItem->BillCalcResult.FinalPrice/subItem->GetQty();
                itemThor.unitPrice = price;
                itemThor.qty = subItem->GetQty();
                itemsList.push_back(itemThor);
                if(subItem->DiscountReason.Length() != 0)
                {
                    tenderDetails.tenderType = eThorVoucher;
                    tenderDetails.tenderIdentifier = "0";
                    for(std::vector<TDiscount>::iterator i = subItem->Discounts.begin() ; i != subItem->Discounts.end() ; ++i)
                    {
                        if(i->VoucherCode.Length() != 0)
                        {
                              tenderDetails.tenderIdentifier = i->VoucherCode;
                        }
                    }
                    tenderDetails.cardNo = paymentTransaction.Membership.Member.CardStr;
                    tenderDetails.tenderValue = 0;
                }
            }
        }
    }
    if((tenderDetails.tenderType == eThorVoucher) && (tenderDetails.tenderIdentifier != "0"))
    {
        tenderDetailsList.push_back(tenderDetails);
    }
    for(int i = 0 ; i < paymentTransaction.PaymentsCount() ; i++)
    {
        TPayment *payment = paymentTransaction.PaymentGet(i);
        if((payment->GetPaymentAttribute(ePayTypeCash)) && ((payment->GetPayTendered()>0)))
        {
            tenderDetails.tenderValue =0;
            tenderDetails.tenderType = eThorCash;
            tenderDetails.tenderIdentifier = "0";
            tenderDetails.cardNo = paymentTransaction.Membership.Member.CardStr;
            double price = payment->GetPayTendered();
            tenderDetails.tenderValue = price;
            tenderDetailsList.push_back(tenderDetails);
        }
        if((payment->GetPaymentAttribute(ePayTypeElectronicTransaction)) && (payment->GetPayTendered()>0))
        {
            tenderDetails.tenderValue =0;
            tenderDetails.tenderType = eThorDebitCard;
            tenderDetails.tenderIdentifier = "0";
            tenderDetails.cardNo = paymentTransaction.Membership.Member.CardStr;
            double price = payment->GetPayTendered();
            tenderDetails.tenderValue = price;
            tenderDetailsList.push_back(tenderDetails);
        }
        if((payment->GetPaymentAttribute(ePayTypeChequeVerify)) && (payment->GetPayTendered()>0))
        {
            tenderDetails.tenderValue =0;
            tenderDetails.tenderType = eThorCheque;
            tenderDetails.tenderIdentifier = "0";
            tenderDetails.cardNo = paymentTransaction.Membership.Member.CardStr;
            double price = payment->GetPayTendered();
            tenderDetails.tenderValue = price;
            tenderDetailsList.push_back(tenderDetails);
        }
    }
    if(paymentTransaction.Membership.Member.Points.getCurrentPointsRedeemed() > 0)
    {
            tenderDetails.tenderValue = 0;
            tenderDetails.tenderType = eThorPoints;
            tenderDetails.tenderIdentifier = "0";
            tenderDetails.cardNo = paymentTransaction.Membership.Member.CardStr;
            double price = paymentTransaction.Membership.Member.Points.getCurrentPointsRedeemed();
            tenderDetails.tenderValue = price;
            if(!TGlobalSettings::Instance().SystemRules.Contains(eprEarnsPointsWhileRedeemingPoints))
            {
                tenderDetails.sendTransactionValue = false;
            }
            tenderDetailsList.push_back(tenderDetails);
    }
    if(tenderDetailsList.size() == 0)
    {
        tenderDetails.tenderValue =0;
        tenderDetails.tenderType = eThorCash;
        tenderDetails.tenderIdentifier = "0";
        tenderDetails.cardNo = paymentTransaction.Membership.Member.CardStr;
        tenderDetails.tenderValue = 0;
        tenderDetailsList.push_back(tenderDetails);
    }
    AnsiString message = TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->SendRequestForPay(tenderDetailsList,itemsList);
    if(message.Length() != 0)
    {
        MessageBox(message, "Transaction Error",MB_OK + MB_ICONERROR);
        returnValue = false;
    }
    return returnValue;
}

bool TListPaymentSystem::PrepareThorRefundRequest(TPaymentTransaction &paymentTransaction)
{
    bool retValue = true;
    TRefundTransaction refundTransaction;
    refundTransaction.cardNumber = paymentTransaction.Membership.Member.CardStr;
    refundTransaction.creditValue = 0.0;
    double transactionValue = 0;
    double loyaltyValue = 0;

    if(paymentTransaction.Money.Total > 0)
        transactionValue = paymentTransaction.Money.Total;
    else
        transactionValue = -paymentTransaction.Money.Total;

    if(paymentTransaction.Membership.Member.Points.getCurrentPointsRefunded() > 0)
        loyaltyValue = paymentTransaction.Membership.Member.Points.getCurrentPointsRefunded();
    else
        loyaltyValue = -paymentTransaction.Membership.Member.Points.getCurrentPointsRefunded();

    if(transactionValue != 0)
        refundTransaction.transactionValue = transactionValue;
    else
        refundTransaction.transactionValue = loyaltyValue;

    if(paymentTransaction.Orders->Count > 0)
        refundTransaction.loyaltyValue = loyaltyValue;
    else
    {
        refundTransaction.loyaltyValue = loyaltyValue;
        for(int i = 0 ; i < paymentTransaction.PaymentsCount() ; i++)
        {
            TPayment *payment = paymentTransaction.PaymentGet(i);
            if((payment->GetPaymentAttribute(ePayTypeCash)) && ((payment->GetPayTendered() != 0)))
            {
                refundTransaction.loyaltyValue = 0;
            }
        }
    }

    AnsiString errorMessage = TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->SendRequestForRefund(refundTransaction);
    if(errorMessage.Length() != 0)
    {
        MessageBox(errorMessage, "Transaction Error", MB_OK + MB_ICONERROR);
        retValue = false;
    }
    return retValue;
}
/**********************DLF MALL START****************************************/

/************************DLF_MALL_CMD_CODE=1**START********************************************/
void TListPaymentSystem::GetDLFMallCMDCodeFirst(AnsiString invoiceNumber,AnsiString fileStatus)     //CMD_CODE_101
{   try{

            Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
            DBTransaction.StartTransaction();

            Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
            DBTransaction.RegisterQuery(IBInternalQuery);

            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text = " SELECT a.INVOICE_NUMBER FROM ARCBILL a order by a.ARCBILL_KEY desc ";
            IBInternalQuery->ExecQuery();
            for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
            {
                invoiceNumber = IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsString;
                break;
            }

         	DBTransaction.Commit();

	AnsiString cmd_code= "1";
	AnsiString file_stat =fileStatus;                                        //length 6      //OPENED  OR CLOSED
	AnsiString tenant_No = TGlobalSettings::Instance().TenantNo;      //length 18
	AnsiString pos_No = TDeviceRealTerminal::Instance().ID.Name;      //length 6
	AnsiString Receipt_No=invoiceNumber;                                     //length 10
	int Tran_File_No=TGlobalSettings::Instance().BatchNo;      //length 4
	AnsiString date = Now().FormatString("yyyymmdd");               //length 8
	AnsiString time = Now().FormatString("hh:mm:ss");               //length 8
	AnsiString user_ID=TDeviceRealTerminal::Instance().User.Name;     //length 8
	AnsiString Sale_date=Now().FormatString("yyyymmdd");                                         //length 8

	AnsiString  finalValue= cmd_code+"|" +file_stat+"|"+tenant_No+"|"+pos_No+"|"+Receipt_No+"|"+ Tran_File_No+ "|" +
	date  +"|" +time+"|" +user_ID+"|" +Sale_date;
	if(TGlobalSettings::Instance().DLFMallFileName==""||TGlobalSettings::Instance().DLFMallFileName== "[null]" )
	{
		AnsiString fileName= CreateFilename(Now());
		AnsiString filePath=     CreateTextFile(fileName,finalValue);  //CREATE TEXT FILE METHOD NAME WILL BE CHANGED
	}
   }
	catch( Exception& exc )
	{

	}
}
/************************DLF_MALL_CMD_CODE=1**END********************************************/


/************************DLF_MALL_CMD_CODE=101**START********************************************/
void TListPaymentSystem::GetDLFMallCMDCodeSec(TPaymentTransaction &paymentTransaction)     //CMD_CODE_101
{
try{

     AnsiString cmd_code= "101";
	AnsiString receipt_No   =paymentTransaction.InvoiceNumber;          //length=10
	AnsiString shift_No="1";          //number of zed     //length=2
	AnsiString Date=Now().FormatString("yyyymmdd");                  //length=8
	AnsiString Time=Now().FormatString("hh:mm:ss");                         //length=8
	AnsiString user_id=TDeviceRealTerminal::Instance().User.Name;                //length=8
	AnsiString manual_Receipt="";         //length=10
    UnicodeString refund_Receipt=paymentTransaction.CreditTransaction?paymentTransaction.InvoiceNumber:UnicodeString("");
  	AnsiString reason_Code="";            //length=10
	AnsiString salesMan_Code=TDeviceRealTerminal::Instance().User.Name;          //length=8
	AnsiString table_No=TDeviceRealTerminal::Instance().ID.TerminalID;//paymentTransaction.TableNo;                 //length=10
	int cust_Count=TManagerPatron::Instance().GetTotalPatrons(paymentTransaction.Patrons);             //length=3
	AnsiString training="N";               //length=1
	AnsiString tran_status="SALE";            //length=8

	AnsiString  finalValue= cmd_code+"|" +receipt_No+"|"+shift_No+"|"+Date+"|"+Time+"|"+ user_id+ "|" +
	manual_Receipt  +"|" +refund_Receipt+"|" +reason_Code+"|" +salesMan_Code
	+"|" +table_No+"|" +cust_Count+"|" +training+"|" +tran_status;

	AnsiString fileName= CreateFilename(Now());
	AnsiString filePath=     CreateTextFile(fileName,finalValue);  //CREATE TEXT FILE METHOD NAME WILL BE CHANGED
    }
	catch( Exception& exc )
	{

	}


}
/************************DLF_MALL_CMD_CODE=101**END********************************************/

/************************DLF_MALL_CMD_CODE=111**START********************************************/
void TListPaymentSystem::GetDLFMallCMDCodeThird(TItemComplete *ItemComplete,AnsiString catkey,Currency discount)     //CMD_CODE_111
{
 //ItemComplete->Discounts.
	try
	{     Currency percentAmount =0;
    	for (std::vector <TDiscount> ::const_iterator ptrDiscounts = ItemComplete->Discounts.begin(); ptrDiscounts != ItemComplete->Discounts.end();
	    std::advance(ptrDiscounts, 1))
	{   //Currency ab=  ptrDiscounts->
        percentAmount =  percentAmount+ ptrDiscounts->PercentAmount ;
    }


		int qty= ItemComplete->GetQty();
		AnsiString cmd_code= "111";
		AnsiString  item_Code              =ItemComplete->Item_ID;             		 //length 16
		AnsiString  item_Qty               =ItemComplete->GetQty();            	 	 //length 8
		AnsiString  org_Price              =ItemComplete->PriceLevel0;            	  //length 8
		AnsiString  new_Price                =CurrToStr((ItemComplete->GetQty()*ItemComplete->PriceLevelCustom)>0?ItemComplete->PriceLevelCustom:ItemComplete->PriceLevel0);              //length 8
		AnsiString  item_Flag              ="";                           //"";//ItemComplete.Item.FinalPrice;              //length 1
		AnsiString  tax_Code               ="Tax";                        //ItemComplete.;              //length 6
		AnsiString  discount_Code          ="";                           //ItemComplete->*discount_Code;              //length 2
		AnsiString  discount_Amt            =CurrToStr(qty>0?(-1*ItemComplete->BillCalcResult.TotalDiscount):ItemComplete->BillCalcResult.TotalDiscount);
		         //length 8
		AnsiString  item_Dept              =ItemComplete->MenuName;              	//length 8
		AnsiString  item_Catg              =catkey ;          			  	//length 8
		AnsiString  label_Keys             ="";              				//length 8
		AnsiString  item_Comm              ="0";             				 //length 1
			AnsiString  item_Nsales            =CurrToStr(ItemComplete->PriceLevelCustom>0?(ItemComplete->GetQty()*ItemComplete->PriceLevelCustom+ItemComplete->BillCalcResult.TotalDiscount-ItemComplete->BillCalcResult.TotalTax):(ItemComplete->GetQty()*ItemComplete->PriceLevel0+ItemComplete->BillCalcResult.TotalDiscount-ItemComplete->BillCalcResult.TotalTax));//CurrentSubOrder->PriceEach_BillCalc();              //length 10

		AnsiString  discount_By            =CurrToStr(qty>0?(-1*ItemComplete->BillCalcResult.TotalDiscount):ItemComplete->BillCalcResult.TotalDiscount);//CurrToStr(percentAmount);              //length 7
		AnsiString  discount_Sign          ="$";              				//length 1
		AnsiString  item_Stax              =CurrToStr(ItemComplete->BillCalcResult.TotalTax);              //length 10
		AnsiString  plu_Code               = ItemComplete->PLU;


		AnsiString  finalValue= cmd_code+"|" +item_Code+"|"+item_Qty+"|"+org_Price+"|"+new_Price+"|"+ item_Flag+ "|" +
		tax_Code  +"|" +discount_Code+"|" +discount_Amt+"|" +item_Dept
		+"|" +item_Catg+"|" +label_Keys+"|" +item_Comm+"|" +item_Nsales+"|" +discount_By+"|" +discount_Sign+"|" +item_Stax+"|" +plu_Code;


		AnsiString fileName= CreateFilename(Now());
		AnsiString filePath=     CreateTextFile(fileName,finalValue);  //CREATE TEXT FILE METHOD NAME WILL BE CHANGED

	}
	catch( Exception& exc )
	{

	}

}
/************************DLF_MALL_CMD_CODE=111**END********************************************/


/************************DLF_MALL_CMD_CODE=111**START***Suborder*****************************************/
void TListPaymentSystem::GetDLFMallCMDCodeSubOrderThird(TItemCompleteSub *ItemComplete,AnsiString catkey,Currency discount)     //CMD_CODE_111
{
 //ItemComplete->Discounts.
	try
	{     Currency percentAmount =0;
    	for (std::vector <TDiscount> ::const_iterator ptrDiscounts = ItemComplete->Discounts.begin(); ptrDiscounts != ItemComplete->Discounts.end();
	std::advance(ptrDiscounts, 1))
	{   //Currency ab=  ptrDiscounts->
      percentAmount =  percentAmount+ ptrDiscounts->PercentAmount ;
    }


		int qty= ItemComplete->GetQty();
		AnsiString cmd_code= "111";
		AnsiString  item_Code              =ItemComplete->Item_ID;             		 //length 16
		AnsiString  item_Qty               =ItemComplete->GetQty();            	 	 //length 8
		AnsiString  org_Price              =ItemComplete->PriceLevel0;            	  //length 8
		AnsiString  new_Price                =CurrToStr((ItemComplete->GetQty()*ItemComplete->PriceLevelCustom)>0?ItemComplete->PriceLevelCustom:ItemComplete->PriceLevel0);              //length 8
		AnsiString  item_Flag              ="";                           //"";//ItemComplete.Item.FinalPrice;              //length 1
		AnsiString  tax_Code               ="Tax";                        //ItemComplete.;              //length 6
		AnsiString  discount_Code          ="";                           //ItemComplete->*discount_Code;              //length 2
		AnsiString  discount_Amt            =CurrToStr(qty>0?(-1*ItemComplete->BillCalcResult.TotalDiscount):ItemComplete->BillCalcResult.TotalDiscount);
		         //length 8
		AnsiString  item_Dept              =ItemComplete->MenuName;              	//length 8
		AnsiString  item_Catg              =catkey ;          			  	//length 8
		AnsiString  label_Keys             ="";              				//length 8
		AnsiString  item_Comm              ="0";             				 //length 1
		AnsiString  item_Nsales            =CurrToStr(ItemComplete->PriceLevelCustom>0?(ItemComplete->GetQty()*ItemComplete->PriceLevelCustom+ItemComplete->BillCalcResult.TotalDiscount-ItemComplete->BillCalcResult.TotalTax):(ItemComplete->GetQty()*ItemComplete->PriceLevel0+ItemComplete->BillCalcResult.TotalDiscount-ItemComplete->BillCalcResult.TotalTax));//CurrentSubOrder->PriceEach_BillCalc();              //length 10

		AnsiString  discount_By            =CurrToStr(qty>0?(-1*ItemComplete->BillCalcResult.TotalDiscount):ItemComplete->BillCalcResult.TotalDiscount);//CurrToStr(percentAmount);              //length 7
		AnsiString  discount_Sign          ="$";              				//length 1
		AnsiString  item_Stax              =CurrToStr(ItemComplete->BillCalcResult.TotalTax);              //length 10
		AnsiString  plu_Code               = ItemComplete->PLU;


		AnsiString  finalValue= cmd_code+"|" +item_Code+"|"+item_Qty+"|"+org_Price+"|"+new_Price+"|"+ item_Flag+ "|" +
		tax_Code  +"|" +discount_Code+"|" +discount_Amt+"|" +item_Dept
		+"|" +item_Catg+"|" +label_Keys+"|" +item_Comm+"|" +item_Nsales+"|" +discount_By+"|" +discount_Sign+"|" +item_Stax+"|" +plu_Code;


		AnsiString fileName= CreateFilename(Now());
		AnsiString filePath=     CreateTextFile(fileName,finalValue);  //CREATE TEXT FILE METHOD NAME WILL BE CHANGED

	}
	catch( Exception& exc )
	{

	}

}
/************************DLF_MALL_CMD_CODE=111**END********************************************/

/************************DLF_MALL_CMD_CODE=121**START********************************************/

void TListPaymentSystem::GetDLFMallCMDCodeForth(TPaymentTransaction &paymentTransaction)     //CMD_CODE_121  and 131
{
    try
    {
        Currency sd= paymentTransaction.Money.TotalGSTContent  ;
        AnsiString cmd_code= "121";
        AnsiString  sales         =paymentTransaction.Money.ProductAmount; ;              //length 11
        AnsiString  discount      =-1*paymentTransaction.Money.ProductDiscount;              //length 11
        AnsiString  cess          ="";              //length 11
        AnsiString  charges       = paymentTransaction.Money.ServiceCharge;
        AnsiString  tax           =paymentTransaction.Money.TotalGSTContent;              //length 11
        AnsiString  tax_Type      ="";
        if(paymentTransaction.Money.ProductAmount==paymentTransaction.Money.GrandTotal)        //length 11
        {
            tax_Type="I";
        }
        else
        {
            tax_Type="E";
        }
        // AnsiString  tax_Type      ="";              //length 1   ProductAmount
        AnsiString  exempt_Gst    ="Y";              //length 1
        AnsiString  discount_Code ="";              //length 2
        AnsiString  other_chg     ="";              //length 7
        AnsiString  discount_Per  =-1*paymentTransaction.Money.ProductDiscount;              //length 6
        AnsiString  rounding_Amt  =paymentTransaction.Money.PaymentRounding;              //length 7

        AnsiString  finalValue= cmd_code+"|" +sales+"|"+discount+"|"+cess+"|"+charges+"|"+ tax+ "|" +
        tax_Type  +"|" +exempt_Gst+"|" +discount_Code+"|" +other_chg
        +"|" +discount_Per+"|" +rounding_Amt;
        AnsiString fileName= CreateFilename(Now());
        AnsiString filePath=     CreateTextFile(fileName,finalValue);  //CREATE TEXT FILE METHOD NAME WILL BE CHANGED
    }
	catch( Exception& exc )
	{

	}

}

void TListPaymentSystem::GetDLFMallCMDCodeFifth(TPaymentTransaction &PaymentTransaction)     //CMD_CODE_121  and 131
{
    try
    {
        for (int i = 0; i < PaymentTransaction.PaymentsCount(); i++)
        {
            TPayment *SubPayment = PaymentTransaction.PaymentGet(i);
            if(SubPayment->GetPayTendered()>0)
            {
                AnsiString cmd_code= "131";
                AnsiString  type          = "T";              //length 1
                AnsiString  payment_Name  =SubPayment->Name;              //length 20
                AnsiString  curr_Code     ="$";              //length 4
                AnsiString  buy_Rate      ="1";              //length 12
                AnsiString  amount        =  RoundToNearest(
                SubPayment->GetPayTendered(),
                0.01,
                TGlobalSettings::Instance().MidPointRoundsDown);              //length 10
                AnsiString  remarks1      ="";              //length 24
                AnsiString  remarks2      ="";              //length 25
                AnsiString  base_Amount   =PaymentTransaction.Money.GrandTotal;              //length 10

                AnsiString  finalValue= cmd_code+"|" +type+"|"+payment_Name+"|"+curr_Code+"|"+buy_Rate+"|"+ amount+ "|" +
                remarks1  +"|" +remarks2+"|" +base_Amount;
                AnsiString fileName= CreateFilename(Now());
                AnsiString filePath=     CreateTextFile(fileName,finalValue);  //CREATE TEXT FILE METHOD NAME WILL BE CHANGED
            }
	    }
     }
	catch( Exception& exc )
	{

	}
}


/************************DLF_MALL_CMD_CODE=1**END********************************************/
// This function is for Creating Filename for DLF Mall
AnsiString TListPaymentSystem::CreateFilename(TDateTime date)
{
	try
	{
		AnsiString preFix ="t";
		AnsiString tenantName = TGlobalSettings::Instance().TenantNo;
		AnsiString posName = TDeviceRealTerminal::Instance().ID.Name;;
		int transactionFileNumber = TGlobalSettings::Instance().BatchNo;  //required global variable starting 1 and max 9999
		SaveIntVariable(vmBatchNo, TGlobalSettings::Instance().BatchNo+1);
        AnsiString datetime = date.FormatString("yymmddhhmm");
    
		AnsiString FileExt = ".txt";
		AnsiString mallPath = TGlobalSettings::Instance().MallPath;
		AnsiString FileName = preFix + tenantName + "_" + posName +  "_" + transactionFileNumber+ "_"  + datetime  + FileExt;

		if(TGlobalSettings::Instance().DLFMallFileName==""||TGlobalSettings::Instance().DLFMallFileName== "[null]" )
		{
			TGlobalSettings::Instance().DLFMallFileName = FileName;
			SaveCompValueinDBStrUnique(vmDLFMallFileName, TGlobalSettings::Instance().DLFMallFileName); // See Function Description
		}
		else{ FileName=  TGlobalSettings::Instance().DLFMallFileName ;
		}

		AnsiString LocalPath = mallPath + "\\DLFMall\\" ;
		if(!DirectoryExists(LocalPath))
		{
			CreateDir(LocalPath);
		}
		LocalPath = LocalPath+FileName;
		return LocalPath;
	}
	catch( Exception& exc )
	{

	}

	return "";

}


// This function is for checking if the specified directory path is existing,
// if not, it will create local path called MallExport then create DLF MALL

UnicodeString TListPaymentSystem::CreateTextFile(AnsiString LocalPath,AnsiString value)  //CREATE TEXT FILE METHOD NAME WILL BE CHANGED
{
     try
     {
        TStrings * List = new TStringList();
        if (FileExists(LocalPath ))
        {
            List->LoadFromFile(LocalPath );
        }

        List->Add(value);
        List->SaveToFile(LocalPath );
        delete List;
        return LocalPath;
    }
    catch( Exception& exc )
    {

    }
    return "";
}

// This function is for saving the integer global variable
void TListPaymentSystem::SaveIntVariable(vmVariables vmVar, int CompName)
{
    try
    {
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction.StartTransaction();
        int GlobalProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction,eSystemProfiles,"Globals");
        if(GlobalProfileKey == 0)
        {
            GlobalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction,eSystemProfiles,"Globals");
        }
        TManagerVariable::Instance().SetProfileInt(DBTransaction, GlobalProfileKey, vmVar, CompName);
        DBTransaction.Commit();
    }
	catch( Exception& exc )
	{

	}
}

void TListPaymentSystem::SaveCompValueinDBStrUnique(vmVariables vmVar, UnicodeString CompName)
{
    try
    {
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction.StartTransaction();

        TManagerVariable::Instance().SetProfileStr(DBTransaction, TManagerVariable::Instance().DeviceProfileKey, vmVar, CompName);
        DBTransaction.Commit();
     }
	catch( Exception& exc )
	{

	}
}
//---------------------------------------------------------------------------
/**************************DLF MALL END********************************************/

TModalResult TListPaymentSystem::CaptureSCDOrPWDCustomerDetails(TPaymentTransaction &paymentTransaction)
{
    modalResult = mrNone;
    //Check SCD Applied on Bill
    isSCDOrPWDApplied = IsSCDOrPWDApplied(paymentTransaction);

    if(isSCDOrPWDApplied)
    {
        std::auto_ptr <TfrmCaptureCustomerDetails> frmCaptureCustomerDetails(TfrmCaptureCustomerDetails::Create <TfrmCaptureCustomerDetails> (Screen->ActiveForm));
        if(frmCaptureCustomerDetails->ShowModal() == mrOk)
        {
            paymentTransaction.customerDetails = frmCaptureCustomerDetails->customerDetails;
            modalResult = mrOk;
        }
        else
        {
            modalResult = mrCancel;
        }
    }
    return modalResult;
}
//-------------------------------------------------------------------------------------------------------------
bool TListPaymentSystem::IsSCDOrPWDApplied(TPaymentTransaction &paymentTransaction)
{
    bool isSCDOrPWDApplied = false;
    for(int i = 0 ; i < paymentTransaction.Orders->Count ; i++)
    {
        TItemComplete *itemComplete = (TItemComplete*)paymentTransaction.Orders->Items[i];

        BillCalculator::DISCOUNT_RESULT_LIST::iterator drIT = itemComplete->BillCalcResult.Discount.begin();

        for( ; drIT != itemComplete->BillCalcResult.Discount.end(); drIT++ )
        {
            if(drIT->Name.UpperCase() != "DIPLOMAT" && drIT->Value != 0)
            {
                std::vector<UnicodeString> discountGroupList = drIT->DiscountGroupList;
                std::vector<UnicodeString>::iterator gIT = discountGroupList.begin();

                for( ; gIT != discountGroupList.end(); gIT++ )
                {
                    if( *gIT == SCD_DISCOUNT_GROUP ||  *gIT == PWD_DISCOUNT_GROUP)
                    {
                        isSCDOrPWDApplied = true;
                    }
                }
            }
        }
    }
	return isSCDOrPWDApplied;
}
//-----------------------------------------------------------------------------------------------------
void TListPaymentSystem::PrepareSCDOrPWDCustomerDetails(TPaymentTransaction &paymentTransaction, long arcbillKey)
{
    TIBSQL *IBInternalQuery = paymentTransaction.DBTransaction.Query(paymentTransaction.DBTransaction.AddQuery());

    InsertSCDOrPWDCustomerDetails(IBInternalQuery, arcbillKey, "Customer Name", paymentTransaction.customerDetails.CustomerName);
    InsertSCDOrPWDCustomerDetails(IBInternalQuery, arcbillKey, "Address", paymentTransaction.customerDetails.Address);
    InsertSCDOrPWDCustomerDetails(IBInternalQuery, arcbillKey, "Tin", paymentTransaction.customerDetails.TinNo);
    InsertSCDOrPWDCustomerDetails(IBInternalQuery, arcbillKey, "Business Style", paymentTransaction.customerDetails.BusinessStyle);
    InsertSCDOrPWDCustomerDetails(IBInternalQuery, arcbillKey, "SC/PWD ID#", paymentTransaction.customerDetails.SC_PWD_ID);
}
//----------------------------------------------------------------------------------------------------------------------
void TListPaymentSystem::InsertSCDOrPWDCustomerDetails(TIBSQL *IBInternalQuery, long arcbillKey, UnicodeString header, UnicodeString value)
{
    try
    {
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_CUSTOMER_DETAILS_KEY, 1) FROM RDB$DATABASE";
        IBInternalQuery->ExecQuery();
        int customerDetailKey = IBInternalQuery->Fields[0]->AsInteger;

        IBInternalQuery->Close();
        IBInternalQuery->ParamCheck = true;
        IBInternalQuery->SQL->Clear();
        IBInternalQuery->SQL->Text =
        "INSERT INTO SCD_PWD_CUSTOMER_DETAILS ("
        "CUSTOMER_DETAILS_KEY,"
        "ARCBILL_KEY,"
        "FIELD_HEADER,"
        "FIELD_VALUE,"
        "DATA_TYPE "
        ")"
        " VALUES "
        "("
        ":CUSTOMER_DETAILS_KEY,"
        ":ARCBILL_KEY,"
        ":FIELD_HEADER,"
        ":FIELD_VALUE,"
        ":DATA_TYPE "
        ");";

        IBInternalQuery->ParamByName("CUSTOMER_DETAILS_KEY")->AsInteger = customerDetailKey;
        IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = arcbillKey;
        IBInternalQuery->ParamByName("FIELD_HEADER")->AsString = header;
        IBInternalQuery->ParamByName("FIELD_VALUE")->AsString = value;
        IBInternalQuery->ParamByName("DATA_TYPE")->AsString = "UnicodeString";
        IBInternalQuery->ExecQuery();
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//-----------------------------------------------------------------------------
void TListPaymentSystem::UpdateSubscriptionDetails( TPaymentTransaction &PaymentTransaction,double amount )
{
    try
    {
        TIBSQL *IBInternalQueryFirst = PaymentTransaction.DBTransaction.Query(PaymentTransaction.DBTransaction.AddQuery());
        IBInternalQueryFirst->Close();
        IBInternalQueryFirst->SQL->Text =
            " UPDATE MEMBERSHIP_SUBS_DETAILS SET"
            " POINTS_RULES_SUBS = :POINTS_RULES_SUBS, "
            " SUBS_PAID = :SUBS_PAID, "
            " SUBS_TYPE = :SUBS_TYPE,"
            " SUBS_PAID_DATE = :SUBS_PAID_DATE,"
            " SUBS_PAID_AMOUNT = :SUBS_PAID_AMOUNT,"
            " SUBS_PAID_RECEIPT_NO = :SUBS_PAID_RECEIPT_NO,"
            " ISLOCAL_MEMBER = :ISLOCAL_MEMBER "
            " WHERE CONTACTS_KEY = :CONTACTS_KEY AND"
            " SUBS_PAID = :SUBS_PAID1 AND ISLOCAL_MEMBER = :ISLOCAL_MEMBER";
        IBInternalQueryFirst->ParamByName("POINTS_RULES_SUBS" )->AsInteger = TPointsRulesSetUtils().CompressSubs(PaymentTransaction.Membership.Member.Points.PointsRulesSubs);
        IBInternalQueryFirst->ParamByName("CONTACTS_KEY" )->AsInteger = PaymentTransaction.Membership.Member.ContactKey;
        IBInternalQueryFirst->ParamByName("SUBS_PAID_DATE" )->AsDateTime = Now();
        IBInternalQueryFirst->ParamByName("SUBS_PAID_AMOUNT" )->AsDouble = amount;
        IBInternalQueryFirst->ParamByName("SUBS_PAID_RECEIPT_NO" )->AsInteger = atoi(PaymentTransaction.InvoiceNumber.t_str());
        IBInternalQueryFirst->ParamByName("SUBS_PAID" )->AsString = "T";
        IBInternalQueryFirst->ParamByName("SUBS_TYPE" )->AsString = "Pay Subs";
        IBInternalQueryFirst->ParamByName("SUBS_PAID1" )->AsString = "F";
        IBInternalQueryFirst->ParamByName("ISLOCAL_MEMBER" )->AsString = "T";
        IBInternalQueryFirst->ExecQuery();
        IBInternalQueryFirst->Close();
        IBInternalQueryFirst->SQL->Text = " UPDATE CONTACTS SET"
            " POINTS_RULES = :POINTS_RULES, "
            " LAST_MODIFIED = :LAST_MODIFIED "
            " WHERE CONTACTS_KEY = :CONTACTS_KEY ";
        IBInternalQueryFirst->ParamByName("POINTS_RULES" )->AsInteger = TPointsRulesSetUtils().Compress(PaymentTransaction.Membership.Member.Points.PointsRules);
        TDeviceRealTerminal::Instance().ManagerMembership->SaveContactInfoEditedToSmartCard(PaymentTransaction.Membership.Member);
        TDateTime datetime = Now();
        PaymentTransaction.Membership.Member.LastModified = datetime;
        IBInternalQueryFirst->ParamByName("LAST_MODIFIED" )->AsDateTime = datetime;
        IBInternalQueryFirst->ParamByName("CONTACTS_KEY" )->AsInteger = PaymentTransaction.Membership.Member.ContactKey;
        IBInternalQueryFirst->ExecQuery();
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}
//------------------------------------------------------------------------------
void TListPaymentSystem::CheckSubscription( TPaymentTransaction &PaymentTransaction )
{
     if(PaymentTransaction.Membership.Member.ContactKey != 0 && TGlobalSettings::Instance().UseMemberSubs &&
        !PaymentTransaction.Membership.Member.Points.PointsRulesSubs.Contains(eprFinancial))
     {
        double amount = 0.0;
        if(TPaySubsUtility::IsPaySubsEligible(PaymentTransaction, amount))
        {
           UpdateSubscriptionDetails(PaymentTransaction,amount);
        }
    }
}
//-------------------------------------------------------------------------------------
UnicodeString TListPaymentSystem::PrepareLastReceiptDataForPanasonic(TStringList *_receipt)
{
    UnicodeString _lastreceipt = "";
    for(int i = 0; i < _receipt->Count; i++)
    {
       _lastreceipt += _receipt->Strings[i] + '\n';
    }
    return _lastreceipt;
}
