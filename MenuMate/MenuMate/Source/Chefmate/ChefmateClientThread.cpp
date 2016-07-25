//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ChefmateClientThread.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall ChefmateClientThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

/****************************************************************************/
// Earlier Definition for class ChefmateClientThread was extending TThread
// now threading has been removed as per the discussion with Ravish Sharma and
// Arpit Tripathi as there are "For loops" which were creating problem when
// used with threading.
/****************************************************************************/

__fastcall TChefmateClientThread::TChefmateClientThread()
{
    CompleteOrderTransaction = NULL;
    CreditOrdersList = NULL;
    CancelOrdersList = NULL;
    CallAwayOrder = NULL;
    ReplacementOrderTransaction = NULL;
}
//---------------------------------------------------------------------------

void TChefmateClientThread::SendCompleteOrder()
{
    executeType = etSendCompleteOrder;
    ExecuteProcess();
}
//---------------------------------------------------------------------------
void TChefmateClientThread::SendCreditOrder()
{
    executeType = etSendCreditOrder;
    ExecuteProcess();
}
//---------------------------------------------------------------------------

void TChefmateClientThread::SendCancelOrder()
{
    executeType = etSendCancelOrder;
    ExecuteProcess();
}
//---------------------------------------------------------------------------

void TChefmateClientThread::SendTransferOrder()
{
    executeType = etSendTransferOrder;
    ExecuteProcess();
}
//---------------------------------------------------------------------------

void TChefmateClientThread::SendCallAwayOrder()
{
    executeType = etSendCallAwayOrder;
    ExecuteProcess();
}
//---------------------------------------------------------------------------

void TChefmateClientThread::SendReplacementOrder()
{
    executeType = etSendReplacementOrder;
    ExecuteProcess();
}
//---------------------------------------------------------------------------

void TChefmateClientThread::ExecuteProcess()
{
    switch( executeType )
    {
        case etSendCompleteOrder    : sendCompleteOrderWithTransaction( CompleteOrderTransaction ); break;
        case etSendCreditOrder      : sendCreditOrder( CreditOrdersList ); break;
        case etSendCancelOrder      : sendCancelOrder( CancelOrdersList ); break;
        case etSendTransferOrder      : sendTransferOrder( OrderList ); break;
        case etSendCallAwayOrder    : sendCallAwayOrder( CallAwayOrder ); break;
        case etSendReplacementOrder : sendReplacementOrderWithTransaction( ReplacementOrderTransaction ); break;

    }
}
//---------------------------------------------------------------------------

void TChefmateClientThread::sendCompleteOrderWithTransaction( TPaymentTransaction* inTransaction)
{
    if( inTransaction != NULL )
    {
         //Chefmate Logging
        TStrings * List = new TStringList();
        if (FileExists(ExtractFilePath(Application->ExeName) + "chefmateLog.txt"))
        {
        List->LoadFromFile(ExtractFilePath(Application->ExeName) + "chefmateLog.txt");
        }
        List->Add("Chefmate Client Thread -- new Order");
        List->Add("Chefmate Client Thread -- order item count is " + IntToStr(inTransaction->Orders->Count));
        List->SaveToFile(ExtractFilePath(Application->ExeName) + "chefmateLog.txt");
        delete List;
        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        TChefmateClient *cmClient = new TChefmateClient();

        initChefMate( cmClient, &inTransaction->ChitNumber );
        cmClient->SendCompleteOrder( inTransaction );
        doneChefMate( cmClient );

        delete cmClient;
    }
}

//---------------------------------------------------------------------------

void TChefmateClientThread::sendCreditOrder(TList* creditOrderList)
{
    if( creditOrderList != NULL )
    {

        //Chefmate Logging
        TStrings * List = new TStringList();
        if (FileExists(ExtractFilePath(Application->ExeName) + "chefmateLog.txt"))
        {
        List->LoadFromFile(ExtractFilePath(Application->ExeName) + "chefmateLog.txt");
        }
        List->Add("Chefmate Client Thread -- Credit Order");
        List->Add("Chefmate Client Thread -- order item count is " + IntToStr(creditOrderList->Count));
        List->SaveToFile(ExtractFilePath(Application->ExeName) + "chefmateLog.txt");
        delete List;
        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        TChefmateClient *cmClient = new TChefmateClient();

        initChefMate( cmClient, NULL );
        cmClient->SendCreditOrder( creditOrderList );
        doneChefMate( cmClient );

        delete cmClient;
    }
}
//---------------------------------------------------------------------------

void TChefmateClientThread::sendCancelOrder(TList* cancelOrderList)
{
    if( cancelOrderList != NULL )
    {
        //Chefmate Logging
        TStrings * List = new TStringList();
        if (FileExists(ExtractFilePath(Application->ExeName) + "chefmateLog.txt"))
        {
        List->LoadFromFile(ExtractFilePath(Application->ExeName) + "chefmateLog.txt");
        }
        List->Add("Chefmate Client Thread -- Refund Order");
        List->Add("Chefmate Client Thread -- order item count is " + IntToStr(cancelOrderList->Count));
        List->SaveToFile(ExtractFilePath(Application->ExeName) + "chefmateLog.txt");
        delete List;
        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        TChefmateClient *cmClient = new TChefmateClient();

        initChefMate( cmClient, NULL );
        cmClient->SendCancelOrder( cancelOrderList );
        doneChefMate( cmClient );

        delete cmClient;
    }
}
//---------------------------------------------------------------------------

void TChefmateClientThread::sendTransferOrder(TList* orderList)
{
    if( orderList != NULL )
    {
        //Chefmate Logging
        TItemComplete *order = ( TItemComplete* )orderList->Items[0];
        TStrings * List = new TStringList();
        if (FileExists(ExtractFilePath(Application->ExeName) + "chefmateLog.txt"))
        {
        List->LoadFromFile(ExtractFilePath(Application->ExeName) + "chefmateLog.txt");
        }
        List->Add("Chefmate Client Thread -- Transfer Order");
        List->Add("Chefmate Client Thread -- order item count is " + IntToStr(orderList->Count));
        List->Add("Chefmate Client Thread -- order Transfeered from " + SourceTableName);
        List->Add("Chefmate Client Thread -- order Transfeered to " + order->TabContainerName);
        List->SaveToFile(ExtractFilePath(Application->ExeName) + "chefmateLog.txt");
        delete List;
        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        TChefmateClient *cmClient = new TChefmateClient();

        initChefMate( cmClient, NULL );
        cmClient->SendTransferOrder( orderList, SourceTableName );
        doneChefMate( cmClient );

        delete cmClient;
    }
}
//---------------------------------------------------------------------------

void TChefmateClientThread::sendCallAwayOrder(TCallAwayComplete* callAwayOrder)
{
    if( callAwayOrder != NULL )
    {
        TChefmateClient *cmClient = new TChefmateClient();

        initChefMate( cmClient, NULL );
        cmClient->SendCallAwayOrder( callAwayOrder );
        doneChefMate( cmClient );

        delete cmClient;
    }
}
//---------------------------------------------------------------------------

void TChefmateClientThread::sendReplacementOrderWithTransaction( TPaymentTransaction* inTransaction)
{
    if( inTransaction != NULL )
    {
        TChefmateClient *cmClient = new TChefmateClient();

        initChefMate( cmClient, &inTransaction->ChitNumber );
        cmClient->SendReplacementOrder( inTransaction );
        doneChefMate( cmClient );

        delete cmClient;
    }
}
//---------------------------------------------------------------------------

void TChefmateClientThread::initChefMate( TChefmateClient *inClient, TChitNumber* inChitNumber )
{
    if( !inClient->Open( TDeviceRealTerminal::Instance().DBControl, inChitNumber ) )
    {
        //MessageBox( "Menumate failed to connect to Chefmate", "Chefmate", MB_OK + MB_ICONWARNING);
    }
}
//---------------------------------------------------------------------------

void TChefmateClientThread::doneChefMate( TChefmateClient *inClient )
{
	inClient->Close();
}
//---------------------------------------------------------------------------

