#include "XTransactionSummaryGroupDetailsReportSection.h"
#include "DeviceRealTerminal.h"
#include "SecurityReference.h"
#include "ReportUtilities.h"
#include "Analysis.h"
#include "Analysis.cpp"

XTransactionSummaryGroupDetailsReportSection::XTransactionSummaryGroupDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmTransactionSummaryGroupDetailsSection, dbTransaction, globalSettings)
{
    dataFormatUtilities = new DataFormatUtilities;
    dataCalculationUtilities = new DataCalculationUtilities;
}

XTransactionSummaryGroupDetailsReportSection::XTransactionSummaryGroupDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime)
	:BaseReportSection(mmConsolidatedZReport, mmTransactionSummaryGroupDetailsSection, dbTransaction, globalSettings, startTime, endTime)
{
    dataFormatUtilities = new DataFormatUtilities;
    dataCalculationUtilities = new DataCalculationUtilities;
}



XTransactionSummaryGroupDetailsReportSection::~XTransactionSummaryGroupDetailsReportSection()
{
    delete dataFormatUtilities;
    delete dataCalculationUtilities;
}

void XTransactionSummaryGroupDetailsReportSection::GetOutput(TPrintout* printOut)
{

   DisplayBankingSection(printOut);
}

void XTransactionSummaryGroupDetailsReportSection::DisplayBankingSection(TPrintout* printOut)
{

    IReportSectionDisplayTraits* reportSectionDisplayTraits = GetTextFormatDisplayTrait();

    AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;

     _memberShip = TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem.get();

    SkimCalculations skimCalculations;

    TTransactionInfo transactionInfo;
    if(IsConsolidatedZed)
    {
        skimCalculations.CalculateSkims(*_dbTransaction, deviceName, *_startTime, *_endTime);
        if(TGlobalSettings::Instance().UseBIRFormatInXZReport)
        {
           TTransactionInfoProcessor::Instance().RemoveEntryFromMap(deviceName);
        }
        transactionInfo = TTransactionInfoProcessor::Instance().GetTransactionInfoForConsolidatedZed(*_dbTransaction, deviceName, *_startTime, *_endTime, true);
    }
    else
    {
        skimCalculations.CalculateSkims(*_dbTransaction, deviceName);
        if(TGlobalSettings::Instance().UseBIRFormatInXZReport)
        {
           TTransactionInfoProcessor::Instance().RemoveEntryFromMap(deviceName);
        }
        transactionInfo = TTransactionInfoProcessor::Instance().GetTransactionInfo(*_dbTransaction, deviceName, true);
    }



    transactionInfo.CalculatedTotals[eStrCalculatedTotals[etcTotalCashDrawerOpens]] = dataCalculationUtilities->GetCashDrawerOpenTotals(*_dbTransaction, _globalSettings, deviceName);

    std::map <int, std::map <UnicodeString, TSumPayments> > ::iterator itPayments;
    Currency groupGrandTotal;
    std::map <UnicodeString, TSumPayments> PaymentValues;
    Currency roundingTotal = 0;
    int total_payment = 0;

    for (itPayments = transactionInfo.Payments.begin(); itPayments != transactionInfo.Payments.end(); itPayments++)
    {
            if(!TGlobalSettings::Instance().UseBIRFormatInXZReport)
            {
                AddTitle(printOut, "Banking                  Group " + IntToStr(itPayments->first));
                printOut->PrintFormat->NewLine();
            }
            int value = itPayments->first;
            PaymentValues = itPayments->second;

            groupGrandTotal = 0.0;

            Currency cashValue = 0;
            Currency changeValue = 0;

                if(reportSectionDisplayTraits)
                {
                    reportSectionDisplayTraits->ApplyTraits(printOut);
                }
                if (value == 0)
                {
                    if(!TGlobalSettings::Instance().UseBIRFormatInXZReport)
                    {

                        groupGrandTotal += skimCalculations.CurrentFloat;

                        printOut->PrintFormat->Line->Columns[0]->Text = eStrCalculatedTotals[ectInitialFloat];
                        printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency(skimCalculations.CurrentFloat);
                        printOut->PrintFormat->AddLine();

                        TCalculatedTotals InitialFloatTotal(ectInitialFloat, skimCalculations.CurrentFloat, 0, 0, 0);
                        transactionInfo.CalculatedTotals[eStrCalculatedTotals[ectInitialFloat]] = InitialFloatTotal;

                        groupGrandTotal += skimCalculations.CurrentSkimsTotal;
                        printOut->PrintFormat->Line->Columns[0]->Text = eStrCalculatedTotals[ectFloatAdjustments];
                        printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency(skimCalculations.CurrentSkimsTotal);
                        printOut->PrintFormat->AddLine();

                     }

                        TCalculatedTotals FloatAdjustmentsTotal(ectFloatAdjustments, skimCalculations.CurrentSkimsTotal, 0, 0, 0);
                        transactionInfo.CalculatedTotals[eStrCalculatedTotals[ectFloatAdjustments]] = FloatAdjustmentsTotal;
                    //}

                    std::map <UnicodeString, TSumPayments> ::iterator itCurrentPayment = PaymentValues.find(CASH);

                    itCurrentPayment = PaymentValues.find(CASH);
                    if (itCurrentPayment != PaymentValues.end())
                    {
                        groupGrandTotal += itCurrentPayment->second.Total;
                        roundingTotal += itCurrentPayment->second.Rounding;
                        groupGrandTotal -= itCurrentPayment->second.CashOut;
                        cashValue = itCurrentPayment->second.Total;

                        TCalculatedTotals CASHTotal(ectCASH, itCurrentPayment->second.Total,0,0, 0);
                        transactionInfo.CalculatedTotals[eStrCalculatedTotals[ectCASH]] = CASHTotal;
                    }

                    // Find the Change Payment Type.
                    itCurrentPayment = PaymentValues.find(CHANGE);
                    if (itCurrentPayment != PaymentValues.end())
                    {
                        groupGrandTotal += itCurrentPayment->second.Total; // This will be a negitive number for change.
                        changeValue += itCurrentPayment->second.Total; // This will be a negitive number for change.
                        roundingTotal += itCurrentPayment->second.Rounding;

                        TCalculatedTotals ChangeTotal(ectChange, itCurrentPayment->second.Total,0,0, 0);
                        transactionInfo.CalculatedTotals[eStrCalculatedTotals[ectChange]] = ChangeTotal;
                    }

                    if(!TGlobalSettings::Instance().UseBIRFormatInXZReport)
                    {
                        printOut->PrintFormat->Line->Columns[0]->Text = "Cash";
                        printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency( cashValue + changeValue + skimCalculations.CashWithdrawl);
                        printOut->PrintFormat->AddLine();

                        printOut->PrintFormat->Line->Columns[0]->Text = "";
                        printOut->PrintFormat->Line->Columns[1]->Line();
                        printOut->PrintFormat->AddLine();
                        printOut->PrintFormat->Add( "Cash In Drawer Total|" + dataFormatUtilities->FormatMMReportCurrency( groupGrandTotal + skimCalculations.CashWithdrawl) );
                        printOut->PrintFormat->NewLine();
                    }
                }
            //}

        //}
        if(reportSectionDisplayTraits)
        {
            reportSectionDisplayTraits->ApplyTraits(printOut);
        }
        if(TGlobalSettings::Instance().UseBIRFormatInXZReport)
        {

            dataCalculationUtilities->PrinterFormatinThreeSections(printOut);
            printOut->PrintFormat->Line->Columns[1]->Alignment = taLeftJustify;
            printOut->PrintFormat->Line->Columns[1]->Text = "Payments";
            printOut->PrintFormat->AddLine();

        }

        std::map <UnicodeString, TSumPayments> ::iterator itCurrentPayment;
        for (itCurrentPayment = PaymentValues.begin(); itCurrentPayment != PaymentValues.end(); itCurrentPayment++)
        {
            if (itCurrentPayment->second.Name.UpperCase() != UpperCase(CHANGE)
                && itCurrentPayment->second.Name.UpperCase() != UpperCase(CREDIT)
                && !(itCurrentPayment->second.Properties & ePayTypeCredit)
                || itCurrentPayment->second.IsLoyaltyVoucher())
            {
                std::map <UnicodeString, TTransactionCount> TransactionsCount = transactionInfo.TransactionsCountGroups[itPayments->first];
                TTransactionCount ThisTransaction = TransactionsCount[itCurrentPayment->second.Name];

                // Payment Might only have surcharges so dont show those here.
                if (itCurrentPayment->second.Total != 0)
                {

                    if(TGlobalSettings::Instance().UseBIRFormatInXZReport)
                    {
                         printOut->PrintFormat->Line->Columns[1]->Text = itCurrentPayment->second.Name;
                         printOut->PrintFormat->Line->Columns[2]->Text = IntToStr(ThisTransaction.Count);
                         if((itCurrentPayment->second.Total - itCurrentPayment->second.TipAmount) < 0)
                         {
                            printOut->PrintFormat->Line->Columns[3]->Text = "(" + CurrToStrF(fabs(itCurrentPayment->second.Total - itCurrentPayment->second.TipAmount), ffNumber, CurrencyDecimals) + ")";
                         }
                         else
                         {
                            printOut->PrintFormat->Line->Columns[3]->Text = CurrToStrF((itCurrentPayment->second.Total - itCurrentPayment->second.TipAmount), ffNumber, CurrencyDecimals);//dataFormatUtilities->FormatMMReportCurrency( itCurrentPayment->second.Total - itCurrentPayment->second.TipAmount);
                         }
                         printOut->PrintFormat->AddLine();
                         total_payment += ThisTransaction.Count;
                    }
                    else
                    {
                        printOut->PrintFormat->Line->Columns[0]->Text = itCurrentPayment->second.Name +
                                        " (" + IntToStr(ThisTransaction.Count) + ")";
                        printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency( itCurrentPayment->second.Total - itCurrentPayment->second.TipAmount);
                        printOut->PrintFormat->AddLine();
                    }
                    TCalculatedTotals Total(itCurrentPayment->second.Name, itCurrentPayment->second.Total,0,0, ThisTransaction.Count);
                    transactionInfo.CalculatedTotals[itCurrentPayment->second.Name] = Total;
                }

                if(itCurrentPayment->second.Name == "Cash" && skimCalculations.CashWithdrawl != 0.00)
                {
                    groupGrandTotal += skimCalculations.CashWithdrawl;
                    DisplayCashWithdrawlSection(printOut, skimCalculations, itCurrentPayment->second.Total - itCurrentPayment->second.TipAmount);
                }

                if (itCurrentPayment->second.TipAmount != 0)
                {
                    if(TGlobalSettings::Instance().UseBIRFormatInXZReport)
                    {
                        printOut->PrintFormat->Line->Columns[1]->Text = itCurrentPayment->second.Name + " Tips";
                        printOut->PrintFormat->Line->Columns[2]->Text = IntToStr(itCurrentPayment->second.TipQty);
                        printOut->PrintFormat->Line->Columns[3]->Text = dataFormatUtilities->FormatMMReportCurrency( itCurrentPayment->second.TipAmount );
                        printOut->PrintFormat->AddLine();
                        total_payment += itCurrentPayment->second.TipQty;
                    }
                    else
                    {
                        printOut->PrintFormat->Line->Columns[0]->Text = itCurrentPayment->second.Name + " Tips" +
                                        " (" + IntToStr(itCurrentPayment->second.TipQty) + ")";
                        printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency( itCurrentPayment->second.TipAmount );
                        printOut->PrintFormat->AddLine();
                    }
                }
                if (itCurrentPayment->second.CashOut != 0)
                {
                    if(TGlobalSettings::Instance().UseBIRFormatInXZReport)
                    {
                        AnsiString cashOut = " CashOut ";
                        printOut->PrintFormat->Line->Columns[1]->Width += 2;//cashOut.Length();
                        printOut->PrintFormat->Line->Columns[2]->Width -= 3;//cashOut.Length();
                        printOut->PrintFormat->Line->Columns[3]->Width +=1;
                        //printOut->PrintFormat->Line->Columns[3]->Width -= cashOut.Length();
                        printOut->PrintFormat->Line->Columns[1]->Text = itCurrentPayment->second.Name + cashOut;
                        printOut->PrintFormat->Line->Columns[2]->Text = IntToStr(ThisTransaction.CashOutCount);
                        printOut->PrintFormat->Line->Columns[3]->Text = dataFormatUtilities->FormatMMReportCurrency( itCurrentPayment->second.CashOut );
                        printOut->PrintFormat->AddLine();
                        total_payment += ThisTransaction.CashOutCount;
                    }
                    else
                    {
                        printOut->PrintFormat->Line->Columns[0]->Text = itCurrentPayment->second.Name + " Cash Out " + " ("
                                        + IntToStr(ThisTransaction.CashOutCount) + ")";
                        printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency( itCurrentPayment->second.CashOut );
                        printOut->PrintFormat->AddLine();
                    }
                    TCalculatedTotals Total(itCurrentPayment->second.Name + " Cash Out", itCurrentPayment->second.CashOut,0,0,
                    ThisTransaction.CashOutCount);
                    transactionInfo.CalculatedTotals[itCurrentPayment->second.Name + " Cash Out"] = Total;
                }

                if (itCurrentPayment->second.Name.UpperCase() == UpperCase(CASH))
                    continue;

                groupGrandTotal += itCurrentPayment->second.CashOut;
                groupGrandTotal += itCurrentPayment->second.Total;
            }
        }

        if (groupGrandTotal != 0)
        {

            if(TGlobalSettings::Instance().UseBIRFormatInXZReport)
            {
                dataCalculationUtilities->PrinterFormatinThreeSections(printOut);
                printOut->PrintFormat->Line->Columns[1]->Text = "-----------------------------------------------------------";
                printOut->PrintFormat->Line->Columns[2]->Text = "-----------------------------------------------------------";
                printOut->PrintFormat->Line->Columns[3]->Text = "-----------------------------------------------------------";
                printOut->PrintFormat->AddLine();

                printOut->PrintFormat->Line->Columns[1]->Text = "Total";
                printOut->PrintFormat->Line->Columns[2]->Text = IntToStr(total_payment);//, ffNumber, CurrencyDecimals);
                if(groupGrandTotal < 0)
                {
                   printOut->PrintFormat->Line->Columns[3]->Text = "(" + CurrToStrF(fabs(groupGrandTotal), ffNumber, CurrencyDecimals) + ")";
                }
                else
                {
                    printOut->PrintFormat->Line->Columns[3]->Text = CurrToStrF(groupGrandTotal, ffNumber, CurrencyDecimals);
                }
                printOut->PrintFormat->AddLine();
                SetSingleColumnPrinterFormat(printOut);
                printOut->PrintFormat->Line->Columns[0]->Text = "";
                printOut->PrintFormat->AddLine();
                printOut->PrintFormat->AddLine();
            }
            else
            {

                if(reportSectionDisplayTraits)
                {
                    reportSectionDisplayTraits->ApplyTraits(printOut);
                }
                printOut->PrintFormat->Line->Columns[0]->Text = "";
                printOut->PrintFormat->Line->Columns[1]->DoubleLine();
                printOut->PrintFormat->AddLine();

                if (itPayments->first == 0)
                {
                    printOut->PrintFormat->Line->Columns[0]->Text = "Subtotal";
                    printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency( groupGrandTotal );
                    printOut->PrintFormat->AddLine();

                    groupGrandTotal -= (skimCalculations.CurrentFloat + skimCalculations.CurrentSkimsTotal);
                    printOut->PrintFormat->Line->Columns[0]->Text = "Less Float & Skims";
                    printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency( -(skimCalculations.CurrentFloat + skimCalculations.CurrentSkimsTotal) );
                    printOut->PrintFormat->AddLine();

                    printOut->PrintFormat->Line->Columns[0]->Text = "";
                    printOut->PrintFormat->Line->Columns[1]->DoubleLine();
                    printOut->PrintFormat->AddLine();
                }

                printOut->PrintFormat->Add("Total |" + dataFormatUtilities->FormatMMReportCurrency( groupGrandTotal));
                printOut->PrintFormat->Line->Columns[0]->Text = "";
                printOut->PrintFormat->Line->Columns[1]->DoubleLine();
                printOut->PrintFormat->AddLine();

                TCalculatedTotals GroupTotal("Group " + IntToStr(itPayments->first), groupGrandTotal,0,0, 0);
                transactionInfo.CalculatedTotals["Group " + IntToStr(itPayments->first)] = GroupTotal;

                printOut->PrintFormat->Add("Rounding (+ = Loss)|" + dataFormatUtilities->FormatMMReportCurrency( roundingTotal ));

                printOut->PrintFormat->Line->Columns[0]->Text = "";
                printOut->PrintFormat->Line->Columns[1]->Line();
                printOut->PrintFormat->AddLine();
            }
        }
        if(!TGlobalSettings::Instance().UseBIRFormatInXZReport)
        {
            Currency VoucherPurchased = 0;
            Currency VoucherCredited = 0;
            Currency VoucherRedeemed = 0;
            Currency PointsIn = 0;
            Currency PointsOut = 0;
            Currency PointsHeld = 0;
            Currency CreditPurchased = 0;
            Currency CreditRedeemed = 0;
            Currency TotalSurcharges = 0;
            Currency EarntPoints = 0;
            Currency PuchasedPoints = 0;
            Currency LoyaltyPoints = 0;
            Currency SpentLoyaltyPoints = 0;
            Currency SpentEarntPoints = 0;
            Currency RefundedPoints = 0;

            int SurchargeQty = 0;

            PointsHeld = _memberShip->GetPointsHeldTotal(*_dbTransaction);

            for (itCurrentPayment = PaymentValues.begin(); itCurrentPayment != PaymentValues.end(); itCurrentPayment++)
            {
                if (itCurrentPayment->second.Properties & ePayTypeGetVoucherDetails &&
                    !itCurrentPayment->second.IsLoyaltyVoucher())
                {
                    if (itCurrentPayment->second.Surcharge == 0)
                    {
                        VoucherRedeemed += itCurrentPayment->second.Total;
                    }
                    else
                    {
                        if (itCurrentPayment->second.Surcharge > 0)
                        {
                            VoucherPurchased += itCurrentPayment->second.Surcharge;
                        }
                        else
                        {
                            VoucherCredited += itCurrentPayment->second.Surcharge;
                        }
                    }
                }

                if (itCurrentPayment->second.Properties & ePayTypeCredit)
                {
                    if (itCurrentPayment->second.Total < 0)
                    {
                        CreditPurchased -= itCurrentPayment->second.Total;
                    }
                    else
                    {
                        CreditRedeemed += itCurrentPayment->second.Total;
                    }
                }

                if (itCurrentPayment->second.Properties & ePayTypePoints)
                {
                    PointsOut += itCurrentPayment->second.Total;
                    PointsIn += itCurrentPayment->second.Surcharge;
                }

                if (itCurrentPayment->second.Surcharge != 0)
                {
                    TotalSurcharges += itCurrentPayment->second.Surcharge;
                    SurchargeQty++;
                }

                std::map<int, Currency> iPoints = itCurrentPayment->second.Points;
                EarntPoints += (iPoints[3] + iPoints[9]);
                PuchasedPoints += iPoints[1];
                SpentEarntPoints += iPoints[5];
                LoyaltyPoints += iPoints[2];
                SpentLoyaltyPoints += iPoints[6];
                RefundedPoints += (iPoints[13] + iPoints[14]);
            }

            PointsOut = (SpentLoyaltyPoints + SpentEarntPoints + RefundedPoints) * -1;
            PointsIn =  LoyaltyPoints + PuchasedPoints;

            if(reportSectionDisplayTraits)
            {
                reportSectionDisplayTraits->ApplyTraits(printOut);
            }

            if (VoucherPurchased != 0 || VoucherRedeemed != 0 || VoucherCredited != 0 || PointsIn != 0 || PointsOut != 0
                || CreditPurchased != 0 || CreditRedeemed != 0 || EarntPoints != 0 || LoyaltyPoints != 0 || SpentLoyaltyPoints != 0
                || SpentEarntPoints != 0 || PuchasedPoints != 0 || RefundedPoints != 0)
            {
                AddSubTitle(printOut, "Other Totals ");

                if(reportSectionDisplayTraits)
                {
                    reportSectionDisplayTraits->ApplyTraits(printOut);
                }
            }

            if(_globalSettings->EnableSeperateEarntPts)
            {
                if (LoyaltyPoints != 0)
                {
                    printOut->PrintFormat->Line->Columns[0]->Text = "Money Pre Loaded ";
                    printOut->PrintFormat->Line->Columns[1]->Text = FormatFloat("0.00", LoyaltyPoints);
                    printOut->PrintFormat->AddLine();
                }

                if (SpentLoyaltyPoints != 0)
                {
                    printOut->PrintFormat->Line->Columns[0]->Text = "Pre Loaded Spent";
                    printOut->PrintFormat->Line->Columns[1]->Text = FormatFloat("0.00", SpentLoyaltyPoints);
                    printOut->PrintFormat->AddLine();
                }

                if (EarntPoints != 0)
                {
                    printOut->PrintFormat->Line->Columns[0]->Text = "Points Earnt";
                    printOut->PrintFormat->Line->Columns[1]->Text = FormatFloat("0.00", EarntPoints);
                    printOut->PrintFormat->AddLine();
                }

                if (PuchasedPoints != 0)
                {
                    printOut->PrintFormat->Line->Columns[0]->Text = "Points Purchased";
                    printOut->PrintFormat->Line->Columns[1]->Text = FormatFloat("0.00", PuchasedPoints);
                    printOut->PrintFormat->AddLine();
                }

                if (SpentEarntPoints != 0)
                {
                    printOut->PrintFormat->Line->Columns[0]->Text = "Points Spent";
                    printOut->PrintFormat->Line->Columns[1]->Text = FormatFloat("0.00", SpentEarntPoints);
                    printOut->PrintFormat->AddLine();
                }

                if (RefundedPoints != 0)
                {
                    printOut->PrintFormat->Line->Columns[0]->Text = "Points Refunded";
                    printOut->PrintFormat->Line->Columns[1]->Text = FormatFloat("0.00", RefundedPoints);
                    printOut->PrintFormat->AddLine();
                }

                Currency Sum = LoyaltyPoints + SpentLoyaltyPoints + EarntPoints + SpentEarntPoints + PuchasedPoints + RefundedPoints;
                if (Sum != 0)
                {
                    printOut->PrintFormat->Line->Columns[0]->Text = "Points Movement (+ = Gain)";
                    printOut->PrintFormat->Line->Columns[1]->Text = FormatFloat("0.00", Sum);
                    printOut->PrintFormat->AddLine();
                    printOut->PrintFormat->Line->Columns[0]->Text = "";
                    printOut->PrintFormat->Line->Columns[1]->Line();
                    printOut->PrintFormat->AddLine();

                    TCalculatedTotals PointsMovementTotal(etcPointsMovement, Sum, 0, 0, 0);
                    transactionInfo.CalculatedTotals[eStrCalculatedTotals[etcPointsMovement]] = PointsMovementTotal;
                }
            }
            else
            {
                if ((LoyaltyPoints + PuchasedPoints) != 0)
                {
                    printOut->PrintFormat->Line->Columns[0]->Text = "Points Purchased";
                    printOut->PrintFormat->Line->Columns[1]->Text = FormatFloat("0.00", (LoyaltyPoints + PuchasedPoints));
                    printOut->PrintFormat->AddLine();
                }

                if (EarntPoints != 0)
                {
                    printOut->PrintFormat->Line->Columns[0]->Text = "Points Earnt";
                    printOut->PrintFormat->Line->Columns[1]->Text = FormatFloat("0.00", EarntPoints);
                    printOut->PrintFormat->AddLine();
                }

                if ((SpentLoyaltyPoints + SpentEarntPoints) != 0)
                {
                    printOut->PrintFormat->Line->Columns[0]->Text = "Points Redeemed";
                    printOut->PrintFormat->Line->Columns[1]->Text = FormatFloat("0.00", (SpentLoyaltyPoints + SpentEarntPoints));
                    printOut->PrintFormat->AddLine();
                }

                if (RefundedPoints != 0)
                {
                    printOut->PrintFormat->Line->Columns[0]->Text = "Points Refunded";
                    printOut->PrintFormat->Line->Columns[1]->Text = FormatFloat("0.00", RefundedPoints);
                    printOut->PrintFormat->AddLine();
                }

                Currency Sum = LoyaltyPoints + SpentLoyaltyPoints + EarntPoints + SpentEarntPoints + PuchasedPoints + RefundedPoints;
                if (Sum != 0)
                {
                    printOut->PrintFormat->Line->Columns[0]->Text = "Points Movement (+ = Gain)";
                    printOut->PrintFormat->Line->Columns[1]->Text = FormatFloat("0.00", Sum);
                    printOut->PrintFormat->AddLine();
                    printOut->PrintFormat->Line->Columns[0]->Text = "";
                    printOut->PrintFormat->Line->Columns[1]->Line();
                    printOut->PrintFormat->AddLine();

                    TCalculatedTotals PointsMovementTotal(etcPointsMovement, Sum, 0, 0, 0);
                    transactionInfo.CalculatedTotals[eStrCalculatedTotals[etcPointsMovement]] = PointsMovementTotal;
                }
                /*if (PointsIn != 0)
                {
                    printOut->PrintFormat->Line->Columns[0]->Text = "Points Sold ";
                    printOut->PrintFormat->Line->Columns[1]->Text = FormatFloat("0.00", PointsIn);
                    printOut->PrintFormat->AddLine();

                    TCalculatedTotals PointsInTotal(etcPointsSold, PointsIn, 0, 0, 0);
                    transactionInfo.CalculatedTotals[eStrCalculatedTotals[etcPointsSold]] = PointsInTotal;
                }

                if (PointsOut != 0)
                {
                    printOut->PrintFormat->Line->Columns[0]->Text = "Points Redeemed ";
                    printOut->PrintFormat->Line->Columns[1]->Text = FormatFloat("(-) 0.00", PointsOut);
                    printOut->PrintFormat->AddLine();
                    TCalculatedTotals PointsOutTotal(etcPointsRedeemed, PointsOut, 0, 0, 0);
                    transactionInfo.CalculatedTotals[eStrCalculatedTotals[etcPointsRedeemed]] = PointsOutTotal;
                }

                if (PointsIn - PointsOut != 0)
                {
                    printOut->PrintFormat->Line->Columns[0]->Text = "Points Movement (+ = Gain)";
                    printOut->PrintFormat->Line->Columns[1]->Text = FormatFloat("0.00", PointsIn - PointsOut);
                    printOut->PrintFormat->AddLine();
                    printOut->PrintFormat->Line->Columns[0]->Text = "";
                    printOut->PrintFormat->Line->Columns[1]->Line();
                    printOut->PrintFormat->AddLine();

                    TCalculatedTotals PointsMovementTotal(etcPointsMovement, PointsIn - PointsOut, 0, 0, 0);
                    transactionInfo.CalculatedTotals[eStrCalculatedTotals[etcPointsMovement]] = PointsMovementTotal;
                }*/
            }

            if (PointsHeld != 0)
            {
                printOut->PrintFormat->Line->Columns[0]->Text = "Points Held in Store";
                printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportPoints( PointsHeld );
                printOut->PrintFormat->AddLine();
                printOut->PrintFormat->Line->Columns[0]->Text = "";
                printOut->PrintFormat->Line->Columns[1]->Line();
                printOut->PrintFormat->AddLine();

                TCalculatedTotals PointsHeldTotal(etcPointsHeld, PointsHeld, 0, 0, 0);
                transactionInfo.CalculatedTotals[eStrCalculatedTotals[etcPointsHeld]] = PointsHeldTotal;
            }

            if (CreditPurchased != 0)
            {
                printOut->PrintFormat->Line->Columns[0]->Text = "Tabs Credit Sold ";
                printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency( CreditPurchased );
                printOut->PrintFormat->AddLine();

                TCalculatedTotals CreditPurchasedTotal(etcTabCreditSold, CreditPurchased, 0, 0, 0);
                transactionInfo.CalculatedTotals[eStrCalculatedTotals[etcTabCreditSold]] = CreditPurchasedTotal;
            }

            if (CreditRedeemed != 0)
            {
                printOut->PrintFormat->Line->Columns[0]->Text = "Tabs Credit Redeemed ";
                printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportRedeemCredit( CreditRedeemed );
                printOut->PrintFormat->AddLine();

                TCalculatedTotals CreditRedeemedTotal(etcTabCreditRedeemed, CreditRedeemed, 0, 0, 0);
                transactionInfo.CalculatedTotals[eStrCalculatedTotals[etcTabCreditRedeemed]] = CreditRedeemedTotal;
            }

            if (CreditPurchased + CreditRedeemed != 0)
            {
                printOut->PrintFormat->Line->Columns[0]->Text = "Credit Movement (+ = Gain)";
                printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency( CreditPurchased - CreditRedeemed );
                printOut->PrintFormat->AddLine();
                printOut->PrintFormat->Line->Columns[0]->Text = "";
                printOut->PrintFormat->Line->Columns[1]->Line();
                printOut->PrintFormat->AddLine();

                TCalculatedTotals TabCreditMovementTotal(etcTabCreditMovement, CreditPurchased - CreditRedeemed, 0, 0, 0);
                transactionInfo.CalculatedTotals[eStrCalculatedTotals[etcTabCreditMovement]] = TabCreditMovementTotal;
            }

            if (VoucherPurchased != 0)
            {
                printOut->PrintFormat->Line->Columns[0]->Text = "Vouchers Sold ";
                printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency( VoucherPurchased );
                printOut->PrintFormat->AddLine();

                TCalculatedTotals VouchersSoldTotal(etcVouchersSold, VoucherPurchased + VoucherCredited, 0, 0, 0);
                transactionInfo.CalculatedTotals[eStrCalculatedTotals[etcVouchersSold]] = VouchersSoldTotal;
            }

            if (VoucherCredited != 0)
            {
                printOut->PrintFormat->Line->Columns[0]->Text = "Vouchers Credited ";
                printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency( VoucherCredited );
                printOut->PrintFormat->AddLine();
            }

            if (VoucherRedeemed != 0)
            {
                printOut->PrintFormat->Line->Columns[0]->Text = "Vouchers Redeemed ";
                printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportRedeemCredit( VoucherRedeemed );
                printOut->PrintFormat->AddLine();

                TCalculatedTotals VoucherRedeemedTotal(etcVouchersRedeemed, VoucherRedeemed, 0, 0, 0);
                transactionInfo.CalculatedTotals[eStrCalculatedTotals[etcVouchersRedeemed]] = VoucherRedeemedTotal;
            }

            if (VoucherPurchased - VoucherRedeemed != 0)
            {
                printOut->PrintFormat->Line->Columns[0]->Text = "Voucher Movement (+ = Gain)";
                printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency( (VoucherPurchased + VoucherCredited) - VoucherRedeemed );
                printOut->PrintFormat->AddLine();
                printOut->PrintFormat->Line->Columns[0]->Text = "";
                printOut->PrintFormat->Line->Columns[1]->Line();
                printOut->PrintFormat->AddLine();

                TCalculatedTotals VoucherMovementTotal(etcVoucherMovement, VoucherPurchased - VoucherRedeemed, 0, 0, 0);
                transactionInfo.CalculatedTotals[eStrCalculatedTotals[etcVoucherMovement]] = VoucherMovementTotal;
            }

            // Print out all Custom surcharge types here.

            if (TotalSurcharges != 0)
            {
                AddSubTitle(printOut, "Surcharges ");

                if(reportSectionDisplayTraits)
                {
                    reportSectionDisplayTraits->ApplyTraits(printOut);
                }

                for (itCurrentPayment = PaymentValues.begin(); itCurrentPayment != PaymentValues.end(); itCurrentPayment++)
                {
                    if (itCurrentPayment->second.Surcharge != 0)
                    {
                        printOut->PrintFormat->Line->Columns[0]->Text = itCurrentPayment->second.Name + " Surcharges ";
                        printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency( itCurrentPayment->second.Surcharge );
                        printOut->PrintFormat->AddLine();

                        TCalculatedTotals Total(itCurrentPayment->second.Name, itCurrentPayment->second.Surcharge,0, 0, 0);
                        transactionInfo.CalculatedTotals[itCurrentPayment->second.Name + " Surcharges"] = Total;
                    }
                }
                printOut->PrintFormat->Line->Columns[0]->Text = "";
                printOut->PrintFormat->Line->Columns[1]->Line();
                printOut->PrintFormat->AddLine();
                printOut->PrintFormat->Line->Columns[0]->Text = "Total Surcharges";
                printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency( TotalSurcharges );
                printOut->PrintFormat->AddLine();
            }

            TCalculatedTotals SurchargesTotal(etcTotalSurcharges, TotalSurcharges, 0, 0, SurchargeQty);
            transactionInfo.CalculatedTotals[eStrCalculatedTotals[etcTotalSurcharges]] = SurchargesTotal;
        }
    }

    // If there are no Purchases still show Initail Float and Skims.
    if (transactionInfo.Payments.size() == 0)
    {

        if(!TGlobalSettings::Instance().UseBIRFormatInXZReport)
        {
            printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
            printOut->PrintFormat->Line->ColCount = 1;
            printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
            printOut->PrintFormat->Line->Columns[0]->Alignment = taCenter;
            printOut->PrintFormat->Line->Columns[0]->DoubleLine();
            printOut->PrintFormat->NewLine();
            printOut->PrintFormat->AddLine();

            printOut->PrintFormat->NewLine();

            if(reportSectionDisplayTraits)
            {
                reportSectionDisplayTraits->ApplyTraits(printOut);
            }

            printOut->PrintFormat->Line->Columns[0]->Text = eStrCalculatedTotals[ectInitialFloat];
            printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency(skimCalculations.CurrentFloat);
            printOut->PrintFormat->AddLine();

            printOut->PrintFormat->Line->Columns[0]->Text = "Float Deposits";
            printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency( skimCalculations.Refloats );
            printOut->PrintFormat->AddLine();

            printOut->PrintFormat->Line->Columns[0]->Text = "Float Withdrawals";
            printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency( skimCalculations.Skims );
            printOut->PrintFormat->AddLine();

            printOut->PrintFormat->Line->Columns[0]->Text = eStrCalculatedTotals[ectFloatAdjustments];
            printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency( skimCalculations.CurrentSkimsTotal );
            printOut->PrintFormat->AddLine();

            if(skimCalculations.CashWithdrawl != 0.00)
            {
                printOut->PrintFormat->Line->Columns[0]->Text = "Cash Withdrawls";
                printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency( skimCalculations.CashWithdrawl );
                printOut->PrintFormat->AddLine();
            }

            printOut->PrintFormat->Line->Columns[0]->Text = "";
            printOut->PrintFormat->Line->Columns[1]->Line();
            printOut->PrintFormat->AddLine();

            printOut->PrintFormat->Line->Columns[0]->Text = "Float Balance";
            printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency( skimCalculations.CurrentFloat + skimCalculations.Refloats + skimCalculations.Skims );
            printOut->PrintFormat->AddLine();
        }
        else
        {
            dataCalculationUtilities->PrinterFormatinThreeSections(printOut);
            printOut->PrintFormat->Line->Columns[1]->Text = "Payments";
            printOut->PrintFormat->AddLine();
            printOut->PrintFormat->Line->Columns[1]->Text = "-----------------------------------------------------------";
            printOut->PrintFormat->Line->Columns[2]->Text = "-----------------------------------------------------------";
            printOut->PrintFormat->Line->Columns[3]->Text = "-----------------------------------------------------------";
            printOut->PrintFormat->AddLine();
            printOut->PrintFormat->Line->Columns[1]->Text = "Total";
            printOut->PrintFormat->Line->Columns[2]->Text = IntToStr(total_payment);
            printOut->PrintFormat->Line->Columns[3]->Text = CurrToStrF(fabs(groupGrandTotal), ffNumber, CurrencyDecimals);
            printOut->PrintFormat->AddLine();
            SetSingleColumnPrinterFormat(printOut);
            printOut->PrintFormat->Line->Columns[0]->Text = "";
            printOut->PrintFormat->AddLine();
            printOut->PrintFormat->AddLine();
        }
    }
}


void XTransactionSummaryGroupDetailsReportSection::SetSingleColumnPrinterFormat(TPrintout* printOut)
{
    printOut->PrintFormat->Line->ColCount = 1;
    printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
    printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
}
//-------------------------------------------------------------------------------------------
void XTransactionSummaryGroupDetailsReportSection::DisplayCashWithdrawlSection(TPrintout* printOut, SkimCalculations &skimCalculations, Currency cashTotal)
{
    if(!TGlobalSettings::Instance().UseBIRFormatInXZReport)
    {
        printOut->PrintFormat->Line->Columns[0]->Text = "Cash Withdrawal(" + IntToStr(skimCalculations.CashWithdrawlCount) + ")";
        printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency(skimCalculations.CashWithdrawl);
        printOut->PrintFormat->AddLine();
        printOut->PrintFormat->Line->Columns[0]->Text = "Net Cash";
        printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency(cashTotal + skimCalculations.CashWithdrawl);
        printOut->PrintFormat->AddLine();
    }
    else
    {
        Currency total = cashTotal + skimCalculations.CashWithdrawl;
        printOut->PrintFormat->Line->Columns[1]->Text = "Cash Withdrawal";
        printOut->PrintFormat->Line->Columns[2]->Text = IntToStr(skimCalculations.CashWithdrawlCount);
        printOut->PrintFormat->Line->Columns[3]->Text =  "(" + CurrToStrF(fabs(skimCalculations.CashWithdrawl), ffNumber, CurrencyDecimals) + ")";
        printOut->PrintFormat->AddLine();
        printOut->PrintFormat->Line->Columns[1]->Text = "Net Cash";
        printOut->PrintFormat->Line->Columns[2]->Text = "";
        printOut->PrintFormat->Line->Columns[3]->Text =  total < 0.00 ? "(" + CurrToStrF(fabs(total), ffNumber, CurrencyDecimals) + ")" : dataFormatUtilities->FormatMMReportCurrency(total);
        printOut->PrintFormat->AddLine();
    }
}
