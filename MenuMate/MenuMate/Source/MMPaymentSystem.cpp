//---------------------------------------------------------------------------


#pragma hdrstop

#include "MMPaymentSystem.h"
#include "DBOrder.h"
#include "DBTab.h"
#include "DBSecurity.h"
#include "DeviceRealTerminal.h"

//---------------------------------------------------------------------------

TMMPaymentSystem::TMMPaymentSystem()
{
    Security = new TListSecurityRefContainer;
//    frmPaymentType = NULL;
//    frmControlTransaction = NULL;
}
//---------------------------------------------------------------------------

TMMPaymentSystem::~TMMPaymentSystem()
{
//    cleanUpControlFormsFromMemory();
}
//---------------------------------------------------------------------------

int TMMPaymentSystem::getTabInvoiceKey( Database::TDBTransaction &dbTransaction, int tabKey )
{
	return TDBTab::GetTabInvoice(dbTransaction, tabKey);
}
//---------------------------------------------------------------------------

void TMMPaymentSystem::saveLeftOverOrders(  Database::TDBTransaction &dbTransaction, TList* ordersList )
{
	for (int i = 0; i < ordersList->Count; i++)
	{
		TItemComplete *Order = (TItemComplete*)ordersList->Items[i];
		Order->OrderKey = 0;
		Order->TransNo = TDBOrder::GetNextTransNumber(dbTransaction);
		if (Order->ServingCourse.ServingCourseKey < 1)
		{
			Order->ServingCourse = TDeviceRealTerminal::Instance().Menus->DefaultServingCourse;
			TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, "Order has no Serving Course");
		}

		TDBOrder::SetOrder(dbTransaction, Order);
		TDBSecurity::ProcessSecurity(dbTransaction, Order->Security);
		for (int i = 0; i < Order->SubOrders->Count; i++)
		{
			TItemCompleteSub *SubOrder = Order->SubOrders->SubOrderGet(i);
			if (SubOrder)
			{
				TDBSecurity::ProcessSecurity(dbTransaction, SubOrder->Security);
			}
		}
	}
}
//---------------------------------------------------------------------------

int TMMPaymentSystem::getSplitDivisionsByTab( Database::TDBTransaction &dbTransaction, int tabKey )
{
	return TDBTab::GetSplitDivisions( dbTransaction, tabKey);
}
//---------------------------------------------------------------------------

void TMMPaymentSystem::setSplitDivisionsForTab( Database::TDBTransaction &dbTransaction, int tabKey, int divisions )
{
	TDBTab::SetSplitDivisions(
						dbTransaction,
						tabKey,
						divisions);
}
//---------------------------------------------------------------------------

UnicodeString TMMPaymentSystem::getInvoiceNumberByInvoiceKey( Database::TDBTransaction &dbTransaction, int invoiceKey )
{
    UnicodeString value = "";

	try
	{
		TIBSQL *IBInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"select "
			"a.INVOICE_NUMBER "
		"from "
			"INVOICES a "
		"where "
			"a.INVOICE_KEY = :INVOICE_KEY";
		IBInternalQuery->ParamByName("INVOICE_KEY")->AsInteger = invoiceKey;
		IBInternalQuery->ExecQuery();

		if(IBInternalQuery->RecordCount)
		{
			value = IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsString;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

	return value;
}
//---------------------------------------------------------------------------

void TMMPaymentSystem::initPaymentControlForms( TPaymentTransaction &transactionToControl, bool isRecovering )
{

    frmPaymentType = new TfrmPaymentType(Screen->ActiveForm, transactionToControl);
    frmControlTransaction = new TfrmControlTransaction(Screen->ActiveForm, transactionToControl);
	//frmPaymentType.reset( TfrmPaymentType::Create(Screen->ActiveForm, transactionToControl) );
	//frmControlTransaction.reset( TfrmControlTransaction::Create(Screen->ActiveForm, transactionToControl) );

	frmPaymentType->QueryPatronCount = TGlobalSettings::Instance().PromptForPatronCount;
	frmPaymentType->Recovering = isRecovering;
}
//---------------------------------------------------------------------------

void TMMPaymentSystem::resetPaymentControlForms(  )
{
     delete frmPaymentType;
     delete frmControlTransaction;
    //frmPaymentType.reset();
    //frmControlTransaction.reset();
}
//---------------------------------------------------------------------------

void TMMPaymentSystem::removeOrdersFromMemory( TList* Orders )
{
	while (Orders->Count != 0)
	{
		delete(TItemComplete*)Orders->Items[0];
		Orders->Delete(0);
	}
}
//---------------------------------------------------------------------------

void TMMPaymentSystem::cleanUpControlFormsFromMemory()
{
    if( frmPaymentType != NULL )
        delete frmPaymentType;

    if( frmControlTransaction != NULL)
        delete frmControlTransaction;
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
