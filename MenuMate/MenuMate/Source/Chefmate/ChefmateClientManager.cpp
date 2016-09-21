//---------------------------------------------------------------------------


#pragma hdrstop

#include "ChefmateClientManager.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//:::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC
//::::::::::::::::::::::::::::::::::::::::::::::

TChefmateClientManager::TChefmateClientManager()
{
}
//---------------------------------------------------------------------------
TChefmateClientManager::~TChefmateClientManager()
{
}
//---------------------------------------------------------------------------
bool TChefmateClientManager::ChefMateEnabled()
{
	return chefMateEnabled();
}
//---------------------------------------------------------------------------
CMC_ERROR TChefmateClientManager::SendCompleteOrder( TPaymentTransaction* inTransaction )
{
	CMC_ERROR result = CMC_ERROR_NOT_AVAILABLE;
    if( chefMateEnabled() )
    {
        TChefmateClient *cmClient = new TChefmateClient();
        initChefMate( cmClient, &inTransaction->ChitNumber );
        cmClient->SendCompleteOrder( inTransaction );
        doneChefMate( cmClient );
        result = CMC_ERROR_SUCCESSFUL;
        delete cmClient;
    }
	return result;
}
//---------------------------------------------------------------------------
CMC_ERROR TChefmateClientManager::SendTransferOrder( TList* orderList,UnicodeString inSourceTableName)
{
	CMC_ERROR result = CMC_ERROR_NOT_AVAILABLE;

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    if( chefMateEnabled() )
    {
        TChefmateClient *cmClient = new TChefmateClient();
        initChefMate( cmClient, NULL );
        cmClient->SendTransferOrder( orderList, inSourceTableName );
        doneChefMate( cmClient );
        result = CMC_ERROR_SUCCESSFUL;
        delete cmClient;
    }
	return result;
}
//---------------------------------------------------------------------------
CMC_ERROR TChefmateClientManager::SendCreditOrder( TContainerOrders* inOrders )
{
	CMC_ERROR result = CMC_ERROR_NOT_AVAILABLE;
    if( chefMateEnabled() )
    {
        // copy orders to a new list
        std::auto_ptr<TList>CreditOrdersList (new TList);
		for (int i = 0; i < inOrders->Count; ++i)
		{
			TItemComplete *item = reinterpret_cast<TItemComplete *>( inOrders->Items[i] );
            TItemComplete *copiedItem = new TItemComplete();
            item->Assign(copiedItem);
            CreditOrdersList->Add(copiedItem);
		}

        TChefmateClient *cmClient = new TChefmateClient();
        initChefMate( cmClient, NULL );
        cmClient->SendCreditOrder( CreditOrdersList.get());
        doneChefMate( cmClient );
        result = CMC_ERROR_SUCCESSFUL;
        delete cmClient;
    }
    return result;
}
//---------------------------------------------------------------------------
CMC_ERROR TChefmateClientManager::SendCancelOrder( TList* inElementsToCancel,AnsiString inTabTableName )
{
	CMC_ERROR result = CMC_ERROR_NOT_AVAILABLE;
    if( chefMateEnabled() )
    {
        TChefmateClient *cmClient = new TChefmateClient();
        initChefMate( cmClient, NULL );
        cmClient->SendCancelOrder( inElementsToCancel,inTabTableName );
        doneChefMate( cmClient );
        result = CMC_ERROR_SUCCESSFUL;
        delete cmClient;
    }
	return result;
}
//---------------------------------------------------------------------------
CMC_ERROR TChefmateClientManager::SendCallAwayOrder( TCallAwayComplete* inCallAway )
{
	CMC_ERROR result = CMC_ERROR_NOT_AVAILABLE;
    if( chefMateEnabled() && inCallAway != NULL)
    {
        TChefmateClient *cmClient = new TChefmateClient();
        initChefMate( cmClient, NULL );
        cmClient->SendCallAwayOrder( inCallAway );
        doneChefMate( cmClient );
        result = CMC_ERROR_SUCCESSFUL;
        delete cmClient;
    }
	return result;
}
//---------------------------------------------------------------------------
CMC_ERROR TChefmateClientManager::SendReplacementOrder( TPaymentTransaction* inTransaction )
{
	CMC_ERROR result = CMC_ERROR_NOT_AVAILABLE;
    if( chefMateEnabled() )
    {
        TChefmateClient *cmClient = new TChefmateClient();
        initChefMate( cmClient, &inTransaction->ChitNumber );
        cmClient->SendReplacementOrder( inTransaction );
        doneChefMate( cmClient );
        result = CMC_ERROR_SUCCESSFUL;
        delete cmClient;
    }
	return result;
}

//:::::::::::::::::::::::::::::::::::::::::::::::
// PRIVATE
//::::::::::::::::::::::::::::::::::::::::::::::

bool TChefmateClientManager::chefMateEnabled()
{
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	TManagerPhysicalPrinter printerManager;
 	std::auto_ptr<TStringList>serverNameList(new TStringList);
	printerManager.GetPrinterServerList(DBTransaction, serverNameList.get(), ptChefMate_Printer);
    DBTransaction.Commit();
	return serverNameList->Count > 0;
}
//---------------------------------------------------------------------------

void TChefmateClientManager::initChefMate( TChefmateClient *inClient, TChitNumber* inChitNumber )
{
    if( !inClient->Open( TDeviceRealTerminal::Instance().DBControl, inChitNumber ) )
    {
        //MessageBox( "Menumate failed to connect to Chefmate", "Chefmate", MB_OK + MB_ICONWARNING);
    }
}
//---------------------------------------------------------------------------

void TChefmateClientManager::doneChefMate( TChefmateClient *inClient )
{
	inClient->Close();
}
//----------------------------------------------------------------------------------------------------
CMC_ERROR TChefmateClientManager::SendWebOrder( TPaymentTransaction* inTransaction, UnicodeString paymentStatus )
{
	CMC_ERROR result = CMC_ERROR_NOT_AVAILABLE;
    if( chefMateEnabled() )
    {
        TChefmateClient *cmClient = new TChefmateClient();
        initChefMate( cmClient, &inTransaction->ChitNumber );
        cmClient->SendCompleteOrder( inTransaction, "WebOrder", paymentStatus );
        doneChefMate( cmClient );
        result = CMC_ERROR_SUCCESSFUL;
        delete cmClient;
    }
	return result;
}
