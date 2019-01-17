//---------------------------------------------------------------------------


#pragma hdrstop

#include "MMInvoicePaymentSystem.h"
#include "DeviceRealTerminal.h"
#include "DBOrder.h"
#include "DBSecurity.h"
#include "ManagerPatron.h"
#include "Invoice.h"
#include "ReceiptManager.h"
//#include "ManagerPanasonic.h"
#include "ManagerAustriaFiscal.h"

//---------------------------------------------------------------------------

TMMInvoicePaymentSystem::TMMInvoicePaymentSystem()
{
	PaymentComplete = false;
	PaymentAborted = false;
	RequestEFTPOSReceipt = false;
}
//---------------------------------------------------------------------------

bool TMMInvoicePaymentSystem::ProcessTransaction(TPaymentTransaction &MasterPaymentTransaction, bool isRecovery )
{
	if( MasterPaymentTransaction.HasOrders() )
	{
		// loads up payment types for the transaction
		_preparePaymentTypes( MasterPaymentTransaction );

		// initialize payment control forms
		initPaymentControlForms( MasterPaymentTransaction, isRecovery );

		// Set securiry ref for master transaction.
		Security->SecurityClear();
		Security->SetSecurityRefNumber(TDBSecurity::GetNextSecurityRef(MasterPaymentTransaction.DBTransaction));

		switch( MasterPaymentTransaction.Type )
		{
			case eTransInvoicePayment:
				_processMultipleInvoiceTransaction( MasterPaymentTransaction );
				break;
			case eTransSplitPayment:
				_processMultipleInvoiceSplitPayTransaction( MasterPaymentTransaction );
				break;
			case eTransPartialPayment:
				_processMultipleInvoicePartialPayTransaction( MasterPaymentTransaction );
				break;
			default:
				break;
		}

		transactionRecovery.ClearRecoveryInfo();
        //Calling Post Ticket method
        if(TGlobalSettings::Instance().EnableStoreTicketPosting && TGlobalSettings::Instance().PMSType == SiHot && TGlobalSettings::Instance().PMSPostSuccessful)
        {
            std::auto_ptr<TMemoryStream> receiptStream(new TMemoryStream);
            receiptStream->LoadFromStream(ManagerReceipt->ReceiptToArchive);
            receiptStream->Position = 0;
            AnsiString ReceiptData((char *)receiptStream->Memory,receiptStream->Size);
            TDeviceRealTerminal::Instance().BasePMS->StoreTicketPost(MasterPaymentTransaction.InvoiceNumber, ReceiptData);
        }
	}

	if (PaymentComplete)
	{
    	// once the receipt details are in the database, clear the archived receipt in memory
	    _clearArchivedReceipt();
		// all the payments are done. hence print the receipt and remove any empty tabs using MasterTransaction
		PerformPostTransactionOperations( MasterPaymentTransaction );
	}
	else
	{
		// refer to same section in ListPaymentSystem for description
		delete LastReceipt;
		LastReceipt = NULL;
	}

    resetPaymentControlForms();
	Reset(MasterPaymentTransaction);
	TDeviceRealTerminal::Instance().ProcessingController.Pop();
	OnAfterTransactionComplete.Occured();
    //Unsetting the Global settings used for Store Ticket Post
    TGlobalSettings::Instance().PMSPostSuccessful = false;
    if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot)
        TDeviceRealTerminal::Instance().BasePMS->UnsetPostingFlag();

    if(TGlobalSettings::Instance().IsAustriaFiscalStorageEnabled)
    {
        std::auto_ptr<TManagerAustriaFiscal> managerAustria(new TManagerAustriaFiscal());
        managerAustria->UnsetPostingFlag();
    }

//    if(TGlobalSettings::Instance().IsPanasonicIntegrationEnabled && PaymentComplete)
//    {
//        TManagerPanasonic::Instance()->TriggerTransactionSync();
//    }
}
//---------------------------------------------------------------------------

void TMMInvoicePaymentSystem::_processMultipleInvoiceTransaction( TPaymentTransaction &MasterPaymentTransaction)
{
	while (!PaymentComplete && !PaymentAborted)
	{
		if (frmPaymentType->Execute() == mrOk)
		{
             bool retrieveUserOptions = frmControlTransaction->RetrieveUserOptions() != eBack;

            //if RetrieveUserOptions is true then check whether transaction has SCD or PWD Discount
            if(retrieveUserOptions)
            {
                std::auto_ptr<TListPaymentSystem> paymentSystem( new TListPaymentSystem );
                if(paymentSystem->CaptureSCDOrPWDCustomerDetails(MasterPaymentTransaction) == mrCancel)
                    retrieveUserOptions = false;
            }

            if (retrieveUserOptions)
            {
				TDeviceRealTerminal::Instance().ManagerMembership->BeginMemberTransaction();
				MasterPaymentTransaction.ProcessPoints();
				TDeviceRealTerminal::Instance().ProcessingController.PushOnce(State);

				PaymentComplete = ProcessThirdPartyModules(MasterPaymentTransaction, RequestEFTPOSReceipt);

				if (PaymentComplete)
				{
					if (MasterPaymentTransaction.CreditTransaction)
						ReturnOrderedItemsToStock(MasterPaymentTransaction);

					if (Screen->ActiveForm != NULL)
						Screen->ActiveForm->Repaint();

					TDeviceRealTerminal::Instance().ProcessingController.Repaint();

                    // the payments are being done for the entire selection.
					// seperate invoices and set the archive records
					_performPostInvoiceTransactionSetOperations( MasterPaymentTransaction );
				}
			}
			else
			{
				PaymentComplete = false;
				MasterPaymentTransaction.PaymentsReset();
				frmPaymentType->EnableElectronicPayments();
			}
		}
		else
		{
			PaymentAborted = true;
		}
	}
}
//---------------------------------------------------------------------------

void TMMInvoicePaymentSystem::_processMultipleInvoicePartialPayTransaction( TPaymentTransaction &MasterPaymentTransaction )
{
	/* get the partial payment total from user
	 * seperate the master transaction to multiple invoice transactions
	 * iterate and inspect with the balances against requested total
	 * pay the invoices completely until cannot be
	 * pay the left amount of the next invoice
	 * keep the rest intact
	*/

	std::auto_ptr <TList> ClonedOrdersList(new TList);
    TItemComplete *SplittedItem = new TItemComplete();
	double splitPercentage = 0;
	Currency requestedTotal;
	double PaymentReturnVal = GetPartialPayment(MasterPaymentTransaction, splitPercentage, requestedTotal);

	if (PaymentReturnVal > 0)
	{
		bool ordersModified = false;

		// seperate the master transaction to individual invoice transactions
		std::vector<TPaymentTransaction> transactionSet;
		_buildInvoicePaymentTransactions( MasterPaymentTransaction, transactionSet );

		// They may be partially paying for the entire Bill in which case PaymentReturnVal == 2;
		if (PaymentReturnVal == 1)
		{
			// remove any invoices that cannot be payed, even partially.
			_removeInvoicesCannotBePayed( transactionSet, requestedTotal );

			// all the invoices except the last one can be paid fully.
			// re-initialise the requestedTotal and splitPercentage for the last invoice
			_reCalculatePaymentPercentageOnLastInvoice( transactionSet, splitPercentage, requestedTotal );
            //Select on those items whose price falls in requested total
            TDBOrder::UpdateOrdersForPartialPayment(transactionSet.back().DBTransaction, transactionSet.back().Orders,
            ClonedOrdersList.get(), SplittedItem, requestedTotal);
            transactionSet.back().Orders->Clear();
            for(int i = 0; i < ClonedOrdersList->Count; i++)
             {
                TItemComplete *Order = (TItemComplete *)ClonedOrdersList->Items[i];
                transactionSet.back().Orders->Add(Order);
             }
			// create clone orders for the last invoice. those will be inserted back to the db
//			TDBOrder::CloneOrdersPartialPaymentDifference(
//									transactionSet.back().DBTransaction,
//									transactionSet.back().Orders,
//									ClonedOrdersList.get(),
//									splitPercentage,
//									requestedTotal);
            transactionSet.back().RequestPartialPayment = requestedTotal ;
			ordersModified = true;
		}

		PaymentsReload(MasterPaymentTransaction);

		// if orders were modified due to partial payments, re-build ordes in master transaction from invoices
		// doing this step to make the receipt looks correct according to what we pay.
		if( ordersModified )
		{
			_populateOrdersFromInvoices( MasterPaymentTransaction, transactionSet );
            initPaymentControlForms( MasterPaymentTransaction, false );
		}

		while (!PaymentComplete && !PaymentAborted)
		{
			if (frmPaymentType->Execute() == mrOk)
			{
				 bool retrieveUserOptions = frmControlTransaction->RetrieveUserOptions() != eBack;

                //if RetrieveUserOptions is true then check whether transaction has SCD or PWD Discount
                if(retrieveUserOptions)
                {
                     std::auto_ptr<TListPaymentSystem> paymentSystem( new TListPaymentSystem );
                     if(paymentSystem->CaptureSCDOrPWDCustomerDetails(MasterPaymentTransaction) == mrCancel)
                        retrieveUserOptions = false;
                }

                if (retrieveUserOptions)
                {
					TDeviceRealTerminal::Instance().ManagerMembership->BeginMemberTransaction();
					MasterPaymentTransaction.ProcessPoints();
					TDeviceRealTerminal::Instance().ProcessingController.PushOnce(State);
					PaymentComplete = ProcessThirdPartyModules(MasterPaymentTransaction, RequestEFTPOSReceipt);

					if (PaymentComplete)
					{
						if (Screen->ActiveForm != NULL)
							Screen->ActiveForm->Repaint();

						TDeviceRealTerminal::Instance().ProcessingController.Repaint();

						_performPostInvoiceTransactionSetOperations( MasterPaymentTransaction );

						// Save off the cloned orders with whats left of the partial payment.
						//saveLeftOverOrders( MasterPaymentTransaction.DBTransaction, ClonedOrdersList.get() );

						// clean-up the cloned orders list
						//removeOrdersFromMemory( ClonedOrdersList.get() );
                        if(SplittedItem->OrderKey > 0)
                        {
                            TPaymentTransaction RemainingOrderTransaction(MasterPaymentTransaction.DBTransaction);
                            RemainingOrderTransaction.Orders->Add(SplittedItem);
                            RemainingOrderTransaction.IgnoreLoyaltyKey = false;
                            RemainingOrderTransaction.Recalc();
                            // Save off the cloned orders with whats left of the partial payment.
                            //insert only splitted order because it's quantity is changed
                            SplittedItem->OrderKey = 0;
                            SplittedItem->TransNo = TDBOrder::GetNextTransNumber(MasterPaymentTransaction.DBTransaction);
                            if (SplittedItem->ServingCourse.ServingCourseKey < 1)
                            {
                                    SplittedItem->ServingCourse = TDeviceRealTerminal::Instance().Menus->DefaultServingCourse;
                                    TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, "Order has no Serving Course");
                            }

                            TDBOrder::SetOrder(MasterPaymentTransaction.DBTransaction, SplittedItem);
                            TDBSecurity::ProcessSecurity(MasterPaymentTransaction.DBTransaction, SplittedItem->Security);
                            for (int i = 0; i < SplittedItem->SubOrders->Count; i++)
                            {
                                    TItemCompleteSub *SubOrder = SplittedItem->SubOrders->SubOrderGet(i);
                                    if (SubOrder)
                                    {
                                            TDBSecurity::ProcessSecurity(MasterPaymentTransaction.DBTransaction, SubOrder->Security);
                                    }
                            }
                        }
					}
				}
				else
				{
					PaymentComplete = false;
					MasterPaymentTransaction.PaymentsReset();
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
}
//---------------------------------------------------------------------------

void TMMInvoicePaymentSystem::_processMultipleInvoiceSplitPayTransaction( TPaymentTransaction &MasterPaymentTransaction )
{
	// load the divisions left on the tabs from db
	_loadDivisionsLeftFromTabs( MasterPaymentTransaction );

	std::auto_ptr <TList> ClonedOrdersList(new TList);
    TItemComplete *SplittedItem = new TItemComplete();
	double SplitPercentage = 0;



        bool DelayAll = false;
	if (SplitPayment(MasterPaymentTransaction, &MasterPaymentTransaction.SplitMoney, SplitPercentage,DelayAll))
	{
		if (MasterPaymentTransaction.SplitMoney.LeftOwing != 0)
		{

               TDBOrder::UpdateOrdersForPartialPayment(MasterPaymentTransaction.DBTransaction, MasterPaymentTransaction.Orders,
                    ClonedOrdersList.get(), SplittedItem, MasterPaymentTransaction.SplitMoney.PaymentAmount);

                 MasterPaymentTransaction.Orders->Clear();

                for(int i = 0; i < ClonedOrdersList->Count; i++)
                {
                    TItemComplete *Order = (TItemComplete *)ClonedOrdersList->Items[i];
                    MasterPaymentTransaction.Orders->Add(Order);
                }
         }

		TManagerPatron::Instance().SetDefaultPatrons(
								MasterPaymentTransaction.DBTransaction,
								MasterPaymentTransaction.Patrons,
								MasterPaymentTransaction.SplitMoney.NumberOfPayments);

		frmPaymentType->btnPrelim->Visible = false;

		// Load points.
		PaymentsReload(MasterPaymentTransaction);

		while (!PaymentComplete && !PaymentAborted)
		{
			if (frmPaymentType->Execute() == mrOk)
			{
				 bool retrieveUserOptions = frmControlTransaction->RetrieveUserOptions() != eBack;

                //if RetrieveUserOptions is true then check whether transaction has SCD or PWD Discount
                if(retrieveUserOptions)
                {
                    std::auto_ptr<TListPaymentSystem> paymentSystem( new TListPaymentSystem );
                    if(paymentSystem->CaptureSCDOrPWDCustomerDetails(MasterPaymentTransaction) == mrCancel)
                        retrieveUserOptions = false;
                }

                if (retrieveUserOptions)
                {
					TDeviceRealTerminal::Instance().ManagerMembership->BeginMemberTransaction();
					MasterPaymentTransaction.ProcessPoints();
					TDeviceRealTerminal::Instance().ProcessingController.PushOnce(State);
					PaymentComplete = ProcessThirdPartyModules(MasterPaymentTransaction, RequestEFTPOSReceipt);

					if (PaymentComplete)
					{
						if (Screen->ActiveForm != NULL)
							Screen->ActiveForm->Repaint();

						TDeviceRealTerminal::Instance().ProcessingController.Repaint();

						_performPostInvoiceTransactionSetOperations( MasterPaymentTransaction );

                        if(SplittedItem->OrderKey > 0)
                            {
                              //Calculate DWT , Tax on discount on remaining quantities
                                TPaymentTransaction RemainingOrderTransaction(MasterPaymentTransaction.DBTransaction);
                                RemainingOrderTransaction.Orders->Add(SplittedItem);
                                RemainingOrderTransaction.IgnoreLoyaltyKey = false;
                                RemainingOrderTransaction.Recalc();

                                SplittedItem->OrderKey = 0;
                                SplittedItem->TransNo = TDBOrder::GetNextTransNumber(MasterPaymentTransaction.DBTransaction);
                                if (SplittedItem->ServingCourse.ServingCourseKey < 1)
                                {
                                        SplittedItem->ServingCourse = TDeviceRealTerminal::Instance().Menus->DefaultServingCourse;
                                        TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, "Order has no Serving Course");
                                }

                                TDBOrder::SetOrder(MasterPaymentTransaction.DBTransaction, SplittedItem);
                                MasterPaymentTransaction.SplittedItemKey = SplittedItem->OrderKey;
                                TDBSecurity::ProcessSecurity(MasterPaymentTransaction.DBTransaction, SplittedItem->Security);
                                for (int i = 0; i < SplittedItem->SubOrders->Count; i++)
                                {
                                        TItemCompleteSub *SubOrder = SplittedItem->SubOrders->SubOrderGet(i);
                                        if (SubOrder)
                                        {
                                                TDBSecurity::ProcessSecurity(MasterPaymentTransaction.DBTransaction, SubOrder->Security);
                                        }
                                }
                            }

						// Save off the cloned orders with whats left of the partial payment.
						//saveLeftOverOrders( MasterPaymentTransaction.DBTransaction, ClonedOrdersList.get() );

						// clean-up the cloned orders list
					   //	removeOrdersFromMemory( ClonedOrdersList.get() );

						// save split divisions left
						_setDivisionsLeftForTabs( MasterPaymentTransaction );
					}
				}
				else
				{
					PaymentComplete = false;
					MasterPaymentTransaction.PaymentsReset();
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
	else
	{
		PaymentAborted = true;
	}
}






//---------------------------------------------------------------------------

void TMMInvoicePaymentSystem::_performPostInvoiceTransactionSetOperations( TPaymentTransaction &MasterPaymentTransaction )
{
    // set receipt number for the transaction
    SetInvoiceNumber(MasterPaymentTransaction);

	// preapare the receipt for MasterTransaction as it contains all the items and payment information
	ReceiptPrepare(MasterPaymentTransaction, RequestEFTPOSReceipt);

	// if required, open the cash drawer on Master transaction as all the payments are done
	OpenCashDrawer(MasterPaymentTransaction);

    // seperate invoices from the set
	std::vector<TPaymentTransaction> transactionSet;
	_buildInvoicePaymentTransactions( MasterPaymentTransaction, transactionSet );

    // assign payment information accordingly
    _assignPaymentsOnTransactionSet( MasterPaymentTransaction, transactionSet );

	std::vector<TPaymentTransaction>::iterator it = transactionSet.begin();

	for( ; it != transactionSet.end(); it++ )
	{
		TPaymentTransaction paymentTransaction = *it;
		_performPostInvoiceTransactionOperations( paymentTransaction );
	}
}
//---------------------------------------------------------------------------

void TMMInvoicePaymentSystem::_performPostInvoiceTransactionOperations( TPaymentTransaction &paymentTransaction )
{
	// mark any free items on the next purchase using location or member purchase counts
	UpdateFreebieRewards(paymentTransaction);

	// initialize LastTotal and LastChange properties
	StoreInfo(paymentTransaction);

	// unused at the moment - leaving it here in case it is modified in the future
	ProcessRewardSchemes(paymentTransaction);

	// store the transaction in database
	ArchiveTransaction(paymentTransaction);

	// clears orders in this transaction from memory
	RemoveOrders(paymentTransaction);

	// if there are any credits, set them here
	AdjustCredit(paymentTransaction);

	// once the receipt details are in the database, clear the archived receipt in memory
   //	_clearArchivedReceipt();
}
//---------------------------------------------------------------------------

void TMMInvoicePaymentSystem::_buildInvoicePaymentTransactions(
                                    TPaymentTransaction &MasterPaymentTransaction,
                                    std::vector<TPaymentTransaction> &invoicePaymentTransactions )
{
    invoicePaymentTransactions.clear();
	std::set <__int64> TabKeys;

    // find the tab keys contained in orders. if there are multiple keys, that means multiple invoices selected
	TDBOrder::GetTabKeysFromOrders(MasterPaymentTransaction.Orders, TabKeys);

    for( std::set<__int64>::iterator it = TabKeys.begin(); it != TabKeys.end(); it++ )
    {
        // assign the base transaction to this new one
        TPaymentTransaction transaction(MasterPaymentTransaction);

        // remove all orders which are not having the same tab key
		_extractOrdersByTabKey( MasterPaymentTransaction, *it, transaction.Orders );

		// recalculate transaction totals as the orders are now changed
		transaction.Money.Recalc(transaction);

		// set invoice key from tab
		transaction.InvoiceKey = getTabInvoiceKey(transaction.DBTransaction, *it);

		// set references for this transaction
		transaction.References.push_back(
									RefRefType(
										Invoice->GetInvoiceNumber(
												transaction.DBTransaction,
												transaction.InvoiceKey),
										ManagerReference->GetReferenceByType(
															transaction.DBTransaction,
															REFTYPE_INVOICE)));

        invoicePaymentTransactions.push_back( transaction );
    }
}
//---------------------------------------------------------------------------

void TMMInvoicePaymentSystem::_assignPaymentsOnTransactionSet(
                            TPaymentTransaction &MasterPaymentTransaction,
                            std::vector<TPaymentTransaction> &invoicePaymentTransactions )
{
    // get the used payments from master transaction
    std::vector<TPayment*> usedPayments;
    _extractUsedPaymentsFromTransaction( MasterPaymentTransaction, usedPayments );

    // clear out all the payments in sub invoices
    _clearPaymentsOnTransactionSet( invoicePaymentTransactions );

    // loop through the available used payments
    // assign them to individual invoices
    std::vector<TPayment*>::iterator it = usedPayments.begin();

    for( ; it != usedPayments.end(); it++ )
    {
        _distributePaymentOnTransactionSet( *it, invoicePaymentTransactions );
    }
}
//---------------------------------------------------------------------------

void TMMInvoicePaymentSystem::_extractOrdersByTabKey( TPaymentTransaction &transaction, __int64 tabKey, TList* destinationOrderList )
{
    destinationOrderList->Clear();

    // remove all orders does not match the tab key
    for(int i = 0; i < transaction.Orders->Count; i++)
    {
        TItemComplete *order = (TItemComplete *)transaction.Orders->Items[i];
        if( order->TabKey == tabKey )
            destinationOrderList->Add(order);
    }
}
//---------------------------------------------------------------------------

void TMMInvoicePaymentSystem::_removeInvoicesCannotBePayed( std::vector<TPaymentTransaction> &transactionSet, Currency requestedTotal )
{
	std::vector<TPaymentTransaction>::iterator it = transactionSet.begin();
	Currency balanceRemaining = requestedTotal;
	Currency currentInvoiceTotal = 0;

	for( ; it != transactionSet.end(); it++ )
	{
		currentInvoiceTotal = it->Money.GrandTotal;

		if( currentInvoiceTotal > balanceRemaining )
			break;
		else
			balanceRemaining = balanceRemaining - currentInvoiceTotal;
	}

	// the last invoice can be only paid partially. hence keep that and remove the rest that follow
	if( it != transactionSet.end() )
		transactionSet.erase( ++it, transactionSet.end());
}
//---------------------------------------------------------------------------

void TMMInvoicePaymentSystem::_reCalculatePaymentPercentageOnLastInvoice(
										std::vector<TPaymentTransaction> &transactionSet,
										double &splitPercentage,
										Currency &requestedTotal )
{
	std::vector<TPaymentTransaction>::iterator it = transactionSet.begin();
	Currency balanceRemaining = requestedTotal;
    Currency currentInvoiceTotal = 0;

	for( ; it != transactionSet.end(); it++ )
	{
		currentInvoiceTotal = it->Money.GrandTotal;

		if( currentInvoiceTotal > balanceRemaining )
		{
			splitPercentage = balanceRemaining / currentInvoiceTotal;
			requestedTotal = balanceRemaining;
			break;
		}
		else
			balanceRemaining = balanceRemaining - currentInvoiceTotal;
	}
}
//---------------------------------------------------------------------------

void TMMInvoicePaymentSystem::_populateOrdersFromInvoices(
								TPaymentTransaction &paymentTransaction,
								std::vector<TPaymentTransaction> &transactionSet )
{
	paymentTransaction.Orders->Clear();
	std::vector<TPaymentTransaction>::iterator it = transactionSet.begin();

	for( ; it != transactionSet.end(); it++)
	{
		for( int i=0; i< it->Orders->Count; i++ )
		{
			paymentTransaction.Orders->Add( it->Orders->Items[i] );
		}
	}
}
//---------------------------------------------------------------------------

void TMMInvoicePaymentSystem::_loadDivisionsLeftFromTabs( TPaymentTransaction &MasterPaymentTransaction )
{
	// Tabs from which the orders come from
	std::set <__int64> TabKeys;
	TDBOrder::GetTabKeysFromOrders(MasterPaymentTransaction.Orders, TabKeys);

	std::set <__int64> ::iterator itTabs = TabKeys.begin();
	if (itTabs != TabKeys.end())
	{
		//todo: fix this for issues, as this only supports equal splitting of all selected invoices, what happens if split one and select other ?
		MasterPaymentTransaction.SplitMoney.DivisionsLeft = getSplitDivisionsByTab(MasterPaymentTransaction.DBTransaction, *itTabs);
	}
}
//---------------------------------------------------------------------------

void TMMInvoicePaymentSystem::_setDivisionsLeftForTabs( TPaymentTransaction &MasterPaymentTransaction )
{
	// Tabs from which the orders come from
	std::set <__int64> TabKeys;
	TDBOrder::GetTabKeysFromOrders(MasterPaymentTransaction.Orders, TabKeys);

	// save back the split divisions left to db
	std::set<__int64>::iterator itTabs = TabKeys.begin();
	for ( ; itTabs != TabKeys.end(); advance(itTabs, 1))
	{
		setSplitDivisionsForTab(
				MasterPaymentTransaction.DBTransaction,
				*itTabs,
				MasterPaymentTransaction.SplitMoney.DivisionsLeft);
	}
}
//---------------------------------------------------------------------------

void TMMInvoicePaymentSystem::_preparePaymentTypes( TPaymentTransaction &paymentTransaction )
{
	PaymentsReload( paymentTransaction );
	PreparePayments( paymentTransaction );
}
//---------------------------------------------------------------------------

void TMMInvoicePaymentSystem::_clearPaymentsOnTransactionSet( std::vector<TPaymentTransaction> &transactionSet )
{
    std::vector<TPaymentTransaction>::iterator it = transactionSet.begin();

    for( ; it != transactionSet.end(); it++ )
    {
        it->PaymentsClear();
    }
}
//---------------------------------------------------------------------------

void TMMInvoicePaymentSystem::_extractUsedPaymentsFromTransaction( TPaymentTransaction &paymentTransaction, std::vector<TPayment*> &usedPayments )
{
	usedPayments.clear();

	paymentTransaction.ReCalculateAmounts();

    for (int i = 0; i < paymentTransaction.PaymentsCount(); i++)
    {
        TPayment *payment = paymentTransaction.PaymentGet(i);
		if( payment->GetPayTendered() != 0 )
        {
            usedPayments.push_back( payment );
        }
    }
}
//---------------------------------------------------------------------------

void TMMInvoicePaymentSystem::_distributePaymentOnTransactionSet( TPayment* payment, std::vector<TPaymentTransaction> &transactionSet )
{
    Currency balanceRemaining = payment->GetPayTendered();
    Currency invoiceAmount = 0;
    Currency payedAmount = 0;

    std::vector<TPaymentTransaction>::iterator it = transactionSet.begin();

    for( ; it != transactionSet.end(); it++ )
	{
		// calculate invoice amounts to get payment due value
		it->ReCalculateAmounts();

		invoiceAmount = it->Money.PaymentDue;

        if( invoiceAmount > 0 && balanceRemaining > 0 )
        {
			payedAmount = invoiceAmount > balanceRemaining ? balanceRemaining : invoiceAmount;
			balanceRemaining = balanceRemaining - payedAmount;

            // assign a payment to the invoice
            TPayment* subPayment = new TPayment( &(*it) );
            *subPayment = *payment;   // using the assignment operator overload on TPayment class
			subPayment->Reset();
			//subPayment->Properties = payment->Properties;
            subPayment->AssignPaymentAttribute(payment->Properties);
			subPayment->SetPay( payedAmount );

			// if the original payment had a cashout, add that as well, so it will adjust the change
			if( payment->GetCashOut() > 0 )
			{
				subPayment->SetCashOut( payment->GetCashOut() );
				payment->SetCashOut( 0 );
			}

			it->PaymentAdd( subPayment );
		}

		// re-calculate amounts, as payments might have changed
		it->ReCalculateAmounts();
    }
}
//---------------------------------------------------------------------------

void TMMInvoicePaymentSystem::_clearArchivedReceipt()
{
	ManagerReceipt->ReceiptToArchive->Clear();
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
