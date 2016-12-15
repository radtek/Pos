// ---------------------------------------------------------------------------

#pragma hdrstop

#include "MMSettings.h"
#include "GlobalSettings.h"
#include "ManagerVariable.h"
// ---------------------------------------------------------------------------

#pragma package(smart_init)

TMMSettings::TMMSettings()
{
}

TMMSettings::~TMMSettings()
{
}

void TMMSettings::Initialise(Database::TDBTransaction &DBTransaction)
{
        TGlobalSettings::Instance().InfoSync = TManagerVariable::Instance().GetInt(DBTransaction, vmInfoSync, 1);
        if (TGlobalSettings::Instance().InfoSync == 0)
        {
        TGlobalSettings::Instance().InfoSync++;
        TManagerVariable::Instance().SetDeviceInt(DBTransaction, vmInfoSync, TGlobalSettings::Instance().InfoSync);
        }

        TGlobalSettings::Instance().AutoMateIP = TManagerVariable::Instance().GetStr(DBTransaction, vmAutoMateIP, "");
        TGlobalSettings::Instance().StockInterbaseIP = TManagerVariable::Instance().GetStr(DBTransaction, vmStockInterbaseIP, "");
        TGlobalSettings::Instance().StockDatabasePath = TManagerVariable::Instance().GetStr(DBTransaction, vmStockDatabasePath, "");
        TGlobalSettings::Instance().SntpIP = TManagerVariable::Instance().GetStr(DBTransaction, vmSntpIP, "");
        TGlobalSettings::Instance().MembershipDatabaseIP = TManagerVariable::Instance().GetStr(DBTransaction, vmMembershipDatabaseIP, "");
        TGlobalSettings::Instance().MembershipDatabasePath = TManagerVariable::Instance().GetStr(DBTransaction, vmMembershipDatabasePath, "");
        TGlobalSettings::Instance().MembershipDatabasePort = TManagerVariable::Instance().GetInt(DBTransaction, vmMembershipDatabasePort, 10061);
        TGlobalSettings::Instance().MembershipTillID = TManagerVariable::Instance().GetInt(DBTransaction, vmMembershipTillID, 1);
        TGlobalSettings::Instance().MaxGamingID = TManagerVariable::Instance().GetInt(DBTransaction, vmMaxGamingID, 1);
        TGlobalSettings::Instance().TabsEnabled = TManagerVariable::Instance().GetBool(DBTransaction, vmTabsEnabled, true);
        TGlobalSettings::Instance().TablesEnabled = TManagerVariable::Instance().GetBool(DBTransaction, vmTablesEnabled, true);
        TGlobalSettings::Instance().PoleDisplayTopLine = TManagerVariable::Instance().GetStr(DBTransaction, vmPoleDisplayTopLine, "MenuMate");
        TGlobalSettings::Instance().PoleDisplayBottomLine = TManagerVariable::Instance().GetStr(DBTransaction, vmPoleDisplayBottomLine, "MenuMate P.O.S");
        TGlobalSettings::Instance().AlwaysForceLogin = TManagerVariable::Instance().GetBool(DBTransaction, vmAlwaysForceLogin, false);
        TGlobalSettings::Instance().AutoLogoutPOS = TManagerVariable::Instance().GetBool(DBTransaction, vmAutoLogoutPOS, false);
        TGlobalSettings::Instance().AutoLogoutSeconds = TManagerVariable::Instance().GetInt(DBTransaction, vmAutoLogoutSeconds, 0);
        TGlobalSettings::Instance().ForcedReasons = TManagerVariable::Instance().GetBool(DBTransaction, vmForcedReasons, false);
        TGlobalSettings::Instance().RevertToFirstCourse = TManagerVariable::Instance().GetBool(DBTransaction, vmRevertToFirstCourse, true);
        TGlobalSettings::Instance().ForcedSidesFirst = TManagerVariable::Instance().GetBool(DBTransaction, vmForcedSidesFirst, false);
        TGlobalSettings::Instance().NonServicePeriodMinutes = TManagerVariable::Instance().GetInt(DBTransaction, vmNonServicePeriod, 30);
        TGlobalSettings::Instance().ClearLogsEveryXDays = TManagerVariable::Instance().GetInt(DBTransaction, vmClearLogsDays, 0);
        TGlobalSettings::Instance().HappyHourExempt = TManagerVariable::Instance().GetBool(DBTransaction, vmHappyHourExempt, false);
        TGlobalSettings::Instance().HappyHourMembersOnly = TManagerVariable::Instance().GetBool(DBTransaction, vmHappyHourMembersOnly, false);
        TGlobalSettings::Instance().MinRedemptionPoint = TManagerVariable::Instance().GetInt(DBTransaction, vmMinRedemptionPoint, 0);
        //Added a new filed for receipt number label.
        TGlobalSettings::Instance().ReceiptNumberLabel  =   TManagerVariable::Instance().GetStr(DBTransaction,vmReceiptNumberLabel,"");
        TGlobalSettings::Instance().RefundReferenceLabel  =   TManagerVariable::Instance().GetStr(DBTransaction,vmRefundReferenceLabel,"");
        //Added a new filed for reprint receipt label.
        TGlobalSettings::Instance().ReprintReceiptLabel  =   TManagerVariable::Instance().GetStr(DBTransaction,vmReprintReceiptLabel,"");
        TGlobalSettings::Instance().PrintStockWeborders =
        TManagerVariable::Instance().GetBool(DBTransaction, vmPrintStockWeborders, false);
        int DefaultMembershipType = MembershipTypeMenuMate;
        if (TGlobalSettings::Instance().MembershipDatabaseIP != "")
        DefaultMembershipType = MembershipTypeERS;
        TGlobalSettings::Instance().MembershipType = TManagerVariable::Instance().GetInt(DBTransaction, vmMembershipType, DefaultMembershipType);
        TGlobalSettings::Instance().EnableEftPosSyncro = TManagerVariable::Instance().GetBool(DBTransaction, vmEnableEftPosSyncro, false);
        TGlobalSettings::Instance().EnableEftPosANZ = TManagerVariable::Instance().GetBool(DBTransaction, vmEnableEftPosANZ, false);
        TGlobalSettings::Instance().EnableEftPosIngenico = TManagerVariable::Instance().GetBool(DBTransaction, vmEnableEftPosIngenico, false);
        TGlobalSettings::Instance().EnableEftPosCadmus = TManagerVariable::Instance().GetBool(DBTransaction, vmEnableEftPosCadmus, false);
        TGlobalSettings::Instance().EnableEftPosCadmusCronos = TManagerVariable::Instance().GetBool(DBTransaction, vmEnableEftPosCadmusCronos, false);
        TGlobalSettings::Instance().EnableEftPosIceLink = TManagerVariable::Instance().GetBool(DBTransaction, vmEnableEftPosIceLink, false);
        TGlobalSettings::Instance().EnableEftPosDPS = TManagerVariable::Instance().GetBool(DBTransaction, vmEnableEftPosDPS, false);
        TGlobalSettings::Instance().EnableDPSTipping = TManagerVariable::Instance().GetBool(DBTransaction, vmEnableDPSTipping, false);
        TGlobalSettings::Instance().EnableDisplayTax = TManagerVariable::Instance().GetBool(DBTransaction, vmEnableDisplayTax, false);
        TGlobalSettings::Instance().EnableDisplayServiceCharge = TManagerVariable::Instance().GetBool(DBTransaction, vmEnableDisplayServiceCharge, false);
        TGlobalSettings::Instance().EnableNmiDisplay = TManagerVariable::Instance().GetBool(DBTransaction, vmEnableNmiDisplay, false);
        TGlobalSettings::Instance().IncludeServiceChargeTaxInTaxValue = TManagerVariable::Instance().GetBool(DBTransaction, vmInclServiceChargeTaxInTaxVal, false);
        TGlobalSettings::Instance().DuplicateEftPosReceipt = TManagerVariable::Instance().GetBool(DBTransaction, vmDuplicateEftPosReceipt, true);
        TGlobalSettings::Instance().PrintSignatureReceiptsTwice = TManagerVariable::Instance().GetBool(DBTransaction, vmDuplicateSignatureReceipts, false);
        TGlobalSettings::Instance().SaveAndPrintPrintsPartialOrder = TManagerVariable::Instance().GetBool(DBTransaction, vmSaveAndPrintPrintsPartialOrder, false);
        TGlobalSettings::Instance().PointsPaymentGroupNumber = TManagerVariable::Instance().GetInt(DBTransaction, vmPointsPaymentGroupNumber, 0);
        TGlobalSettings::Instance().CreditPaymentGroupNumber = TManagerVariable::Instance().GetInt(DBTransaction, vmCreditPaymentGroupNumber, 0);
        TGlobalSettings::Instance().HappyHourStart = TManagerVariable::Instance().GetTime(DBTransaction, vmHappyHourStart, 0);
        TGlobalSettings::Instance().HappyHourFinish = TManagerVariable::Instance().GetTime(DBTransaction, vmHappyHourFinish, 0);
        TGlobalSettings::Instance().HappyHourDays[eSunday] = TManagerVariable::Instance().GetBool(DBTransaction, vmHHSunday, false);
        TGlobalSettings::Instance().HappyHourDays[eMonday] = TManagerVariable::Instance().GetBool(DBTransaction, vmHHMonday, false);
        TGlobalSettings::Instance().HappyHourDays[eTuesday] = TManagerVariable::Instance().GetBool(DBTransaction, vmHHTuesday, false);
        TGlobalSettings::Instance().HappyHourDays[eWednesday] = TManagerVariable::Instance().GetBool(DBTransaction, vmHHWednesday, false);
        TGlobalSettings::Instance().HappyHourDays[eThursday] = TManagerVariable::Instance().GetBool(DBTransaction, vmHHThursday, false);
        TGlobalSettings::Instance().HappyHourDays[eFriday] = TManagerVariable::Instance().GetBool(DBTransaction, vmHHFridays, false);
        TGlobalSettings::Instance().HappyHourDays[eSaturday] = TManagerVariable::Instance().GetBool(DBTransaction, vmHHSaturday, false);
        TGlobalSettings::Instance().ShowPointsOnZed = TManagerVariable::Instance().GetBool(DBTransaction, vmShowPointsOnZed, true);
        TGlobalSettings::Instance().ShowInvoiceInfoOnZed = TManagerVariable::Instance().GetBool(DBTransaction, vmShowInvoiceInfoOnZed, true);
        TGlobalSettings::Instance().ShowSessionDateInZed = TManagerVariable::Instance().GetBool(DBTransaction, vmShowSessionDateInZed, true);
        TGlobalSettings::Instance().HideServingCourseLabels = TManagerVariable::Instance().GetBool(DBTransaction, vmHideServingCourseLabels, false);
        TGlobalSettings::Instance().RememberLastServingCourse = TManagerVariable::Instance().GetBool(DBTransaction, vmRememberLastServingCourse, false);
        TGlobalSettings::Instance().PromptForPatronCount = TManagerVariable::Instance().GetBool(DBTransaction, vmPromptForPatronCount, false);
        TGlobalSettings::Instance().LuckyMemberPeriod = TManagerVariable::Instance().GetInt(DBTransaction, vmLuckyMemberPeriod, 24);
        TGlobalSettings::Instance().LuckyDrawByLocationOnly = TManagerVariable::Instance().GetBool(DBTransaction, vmLuckyMemberDrawByLocationOnly, false);
        TGlobalSettings::Instance().DuplicateReceipts = TManagerVariable::Instance().GetBool(DBTransaction, vmDuplicateReceipts, false);
        TGlobalSettings::Instance().AutoPrintTabReceipts = TManagerVariable::Instance().GetBool(DBTransaction, vmAutoPrintTabReceipts, false);
        TGlobalSettings::Instance().AutoPrintStaffReceipts = TManagerVariable::Instance().GetBool(DBTransaction, vmAutoPrintStaffReceipts, false);
        TGlobalSettings::Instance().AutoPrintMemberReceipts = TManagerVariable::Instance().GetBool(DBTransaction, vmAutoPrintMemberReceipts, false);
        TGlobalSettings::Instance().AutoPrintTableReceipts = TManagerVariable::Instance().GetBool(DBTransaction, vmAutoPrintTableReceipts, false);
        TGlobalSettings::Instance().AutoPrintCashReceipts = TManagerVariable::Instance().GetBool(DBTransaction, vmAutoPrintCashReceipts, false);
        TGlobalSettings::Instance().AutoPrintRoomReceipts = TManagerVariable::Instance().GetBool(DBTransaction, vmAutoPrintRoomReceipts, false);
        TGlobalSettings::Instance().AutoPrintInvoiceReceipts = TManagerVariable::Instance().GetBool(DBTransaction, vmAutoPrintInvoiceReceipts, false);
        TGlobalSettings::Instance().AutoPrintWebReceipts = TManagerVariable::Instance().GetBool(DBTransaction, vmAutoPrintWebReceipts, false);
        TGlobalSettings::Instance().DefaultToBevMenu = TManagerVariable::Instance().GetBool(DBTransaction, vmPOSDefaultsToBeverages, false);
        TGlobalSettings::Instance().SummariseDiscountOnZed = TManagerVariable::Instance().GetBool(DBTransaction, vmSummariseDiscountOnZed, false);
        TGlobalSettings::Instance().AutoConvertScalesWeights = TManagerVariable::Instance().GetBool(DBTransaction, vmAutoConvertScalesWeights, false);
        TGlobalSettings::Instance().AllowToEnterPrice = TManagerVariable::Instance().GetBool(DBTransaction, vmAllowEnterPrice, false);
        TGlobalSettings::Instance().AllowToEnterWeight = TManagerVariable::Instance().GetBool(DBTransaction, vmAllowEnterWeight, false);
        TGlobalSettings::Instance().EnableAARewards = TManagerVariable::Instance().GetBool(DBTransaction, vmEnableAARewards, false);
        TGlobalSettings::Instance().EnableAARewardsReminder = TManagerVariable::Instance().GetBool(DBTransaction, vmEnableAARewardsReminder, false);
        TGlobalSettings::Instance().DisableInternalInvoicing = TManagerVariable::Instance().GetBool(DBTransaction, vmDisableInternalInvoicing, false);
        TGlobalSettings::Instance().EnableBlindBalances = TManagerVariable::Instance().GetBool(DBTransaction, vmEnableBlindBalances, false);
        TGlobalSettings::Instance().EndOfDay = TManagerVariable::Instance().GetInt(DBTransaction, vmEndOfDay, 5);
        TGlobalSettings::Instance().ExtendCurrencyDisplay = TManagerVariable::Instance().GetBool(DBTransaction, vmExtendCurrencyDisplay, false);
        TGlobalSettings::Instance().DisableConfirmationOnSave = TManagerVariable::Instance().GetBool(DBTransaction, vmDisableConfirmationOnSave, false);
        TGlobalSettings::Instance().DisableReceiptOnConfirmation = TManagerVariable::Instance().GetBool(DBTransaction, vmDisableReceiptOnConfirmation, false);
        TGlobalSettings::Instance().EnableBirthdayNotifications = TManagerVariable::Instance().GetBool(DBTransaction, vmEnableBirthdayNotifications, false);
        TGlobalSettings::Instance().AcknowledgeFirstVisit = TManagerVariable::Instance().GetBool(DBTransaction, vmAcknowledgeFirstVisit, false);
        TGlobalSettings::Instance().EnableMemberSpendChits = TManagerVariable::Instance().GetBool(DBTransaction, vmEnableMemberSpendChits, false);
        TGlobalSettings::Instance().MemberSpendChitsAmount = StrToCurr(TManagerVariable::Instance().GetStr(DBTransaction, vmMemberSpendChitsAmount, "5.00"));
        TGlobalSettings::Instance().AutoAddWebMembers = TManagerVariable::Instance().GetBool(DBTransaction, vmAutoAddWebMembers, false);
        TGlobalSettings::Instance().EnableStaffHours = TManagerVariable::Instance().GetBool(DBTransaction, vmEnableStaffHours, false);
        TGlobalSettings::Instance().EnableCommission = TManagerVariable::Instance().GetBool(DBTransaction, vmEnableCommission, false);
        TGlobalSettings::Instance().EnablePrinterCounts = TManagerVariable::Instance().GetBool(DBTransaction, vmEnablePrinterCounts, false);
        TGlobalSettings::Instance().EnablePaxCount = TManagerVariable::Instance().GetBool(DBTransaction, vmEnablePaxCount, false);
        TGlobalSettings::Instance().EnableHideCredsCancels = TManagerVariable::Instance().GetBool(DBTransaction, vmHideCredsCancels, false);
        TGlobalSettings::Instance().EnableDontClearZedData = TManagerVariable::Instance().GetBool(DBTransaction, vmDontClearZedData, false);
        TGlobalSettings::Instance().CaptureCustomerName = TManagerVariable::Instance().GetBool(DBTransaction, vmCaptureCustomer, false);
        TGlobalSettings::Instance().EnableHoldSend = TManagerVariable::Instance().GetBool(DBTransaction, vmHoldSend, false);
        TGlobalSettings::Instance().EnableMenuPatronCount = TManagerVariable::Instance().GetBool(DBTransaction, vmMenuPatronCount, false);
        TGlobalSettings::Instance().SpendPtsOnlyFullProduct = TManagerVariable::Instance().GetBool(DBTransaction, vmSpendPtsOnlyFullProduct, false);
        TGlobalSettings::Instance().SpendPtsOnlySelectItems = TManagerVariable::Instance().GetBool(DBTransaction, vmSpendPtsOnlySelectItems, false);
        TGlobalSettings::Instance().EarntPointsOnWeight = TManagerVariable::Instance().GetBool(DBTransaction, vmEarntPointsOnWeight, false);
        TGlobalSettings::Instance().HideFreeItemsAtBilling = TManagerVariable::Instance().GetBool(DBTransaction, vmHideFreeItemsAtBilling, false);
        TGlobalSettings::Instance().AllowRefundPoints = TManagerVariable::Instance().GetBool(DBTransaction, vmAllowRefundPoints, false);
        TGlobalSettings::Instance().MaximumChitNumber = TManagerVariable::Instance().GetInt(DBTransaction, vmMaximumChitNumber, 99);
        bool enableWaiterStation = false;
        TManagerVariable::Instance().GetProfileBool(DBTransaction,TManagerVariable::Instance().DeviceProfileKey,vmEnableWaiterStation,enableWaiterStation);
        TGlobalSettings::Instance().EnableWaiterStation = enableWaiterStation;
        TGlobalSettings::Instance().NoPtsEarnedDuringHappyHour = TManagerVariable::Instance().GetBool(DBTransaction, vmNoPtsEarnedDuringHappyHour, false);
        int g = 0;
        TManagerVariable::Instance().GetProfileIntDB(DBTransaction, TManagerVariable::Instance().DeviceProfileKey,vmDepositBagNum, g);
        TGlobalSettings::Instance().EnableDepositBagNum = g;
        TGlobalSettings::Instance().EnableSeperateEarntPts = TManagerVariable::Instance().GetBool(DBTransaction, vmEnableSeperateEarntPts, false);
        TGlobalSettings::Instance().ShowPointsOnDisplay = TManagerVariable::Instance().GetBool(DBTransaction, vmPoleDisplayShowPoints, false);
        if (TManagerVariable::Instance().GetBool(DBTransaction, vmEarnsPointsWhileRedeeming))
        {
        TGlobalSettings::Instance().SystemRules << eprEarnsPointsWhileRedeemingPoints;
        }
        if (TManagerVariable::Instance().GetBool(DBTransaction, vmOnlyEarnsPointsWhileRedeeming))
        {
        TGlobalSettings::Instance().SystemRules << eprOnlyEarnsPointsWhileRedeemingPoints;
        }

        TGlobalSettings::Instance().IntaMateIPAddress = TManagerVariable::Instance().GetStr(DBTransaction, vmIntaMateIPAddress, ""); ;
        TGlobalSettings::Instance().IntaMatePort = TManagerVariable::Instance().GetInt(DBTransaction, vmIntaMatePort, 0);
        TGlobalSettings::Instance().IntaMateTCPTimeOut_ms = TManagerVariable::Instance().GetInt(DBTransaction, vmIntaMateIPTimeOut, 60000);
        TGlobalSettings::Instance().IntaMateTerminalID = TManagerVariable::Instance().GetInt(DBTransaction, vmIntaMateTerminalID, 0);
        TGlobalSettings::Instance().WebMatePort = TManagerVariable::Instance().GetInt(DBTransaction, vmWebMatePort, 65006);
        TGlobalSettings::Instance().WebMateEnabled = TManagerVariable::Instance().GetBool(DBTransaction, vmWebMateEnabled, false);
        TGlobalSettings::Instance().AutoAcceptWebOrders = TManagerVariable::Instance().GetBool(DBTransaction, vmAutoAcceptWebOrders, false);
        TGlobalSettings::Instance().UnRavelKitchenPrinting = TManagerVariable::Instance().GetBool(DBTransaction, vmUnRavelKitchenPrinting, false);
        TGlobalSettings::Instance().SeatLabel = TManagerVariable::Instance().GetStr(DBTransaction, vmSeatLabel, "Guest");
        TGlobalSettings::Instance().ReservationsAddress	= TManagerVariable::Instance().GetStr(DBTransaction,vmReservationsAddress,"");
        TGlobalSettings::Instance().ReservationsEnabled	= TManagerVariable::Instance().GetBool(DBTransaction,vmReservationsEnabled,false);
        TGlobalSettings::Instance().FloorPlanTransparentTables 	= TManagerVariable::Instance().GetBool(DBTransaction,vmFloorPlanTransparentTables,false);
        TGlobalSettings::Instance().FloorPlanTableColour = TManagerVariable::Instance().GetInt(DBTransaction,vmFloorPlanTableColour,clBlack);
        TGlobalSettings::Instance().FloorPlanTextColour  = TManagerVariable::Instance().GetInt(DBTransaction,vmFloorPlanTextColour,clWhite);
        TGlobalSettings::Instance().LastSelectedFloorPlanLocationID = TManagerVariable::Instance().GetInt(DBTransaction,vmLastSelectedFloorPlanLocationID,-1);
        if ((TGlobalSettings::Instance().QMSIsEnabled = TManagerVariable::Instance().GetBool(DBTransaction,vmQuickMemberSetupEnabled)))
        {
        TGlobalSettings::Instance().QMSPhoneNumberIsMandatory = TManagerVariable::Instance().GetBool(DBTransaction, vmQuickMemberSetupPhoneIsMandatory);
        TGlobalSettings::Instance().QMSEmailAddressIsMandatory = TManagerVariable::Instance().GetBool(DBTransaction, vmQuickMemberSetupEmailIsMandatory);
        TGlobalSettings::Instance().QMSDateOfBirthIsMandatory = TManagerVariable::Instance().GetBool(DBTransaction, vmQuickMemberSetupDateOfBirthIsMandatory);
        }
        TGlobalSettings::Instance().NagUserToSelectChit = TManagerVariable::Instance().GetBool(DBTransaction, vmNagUserToSelectChit);
        TGlobalSettings::Instance().WarnIfOpenTableAndTabs = TManagerVariable::Instance().GetBool(DBTransaction, vmWarnIfOpenTableAndTabs, true);
        TGlobalSettings::Instance().StopIfOpenTableAndTabs = TManagerVariable::Instance().GetBool(DBTransaction, vmStopIfOpenTableAndTabs, false);
        // Globals.
        int GlobalProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction, eSystemProfiles, "Globals");
        if (GlobalProfileKey != 0)
        {
			TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmCombineServiceChargeTaxWithServiceCharge, TGlobalSettings::Instance().ShowServiceChargeTaxWithServiceCharge);
			TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmCombineServiceChargeTaxWithSalesTax, TGlobalSettings::Instance().ShowServiceChargeTaxWithSalesTax);
			TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmShowTransactionSummaryGroups, TGlobalSettings::Instance().ShowTransactionSummaryGroups);
			TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmShowBilledSalesTotals, TGlobalSettings::Instance().ShowBilledSalesTotals);
			TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmShowComplimentarySalesTotals, TGlobalSettings::Instance().ShowComplimentarySalesTotals);
			TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmShowChargedSalesTotals, TGlobalSettings::Instance().ShowChargedSalesTotals);
			TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmShowTotals, TGlobalSettings::Instance().ShowTotals);
			TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmShowBreakdownCategories, TGlobalSettings::Instance().ShowBreakdownCategories);
			TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmShowDiscountReport, TGlobalSettings::Instance().ShowDiscountReport);
			TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmShowPointsReport, TGlobalSettings::Instance().ShowPointsReport);
			TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmShowPatronAverages, TGlobalSettings::Instance().ShowPatronAverages);
			TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmShowProductionInfo, TGlobalSettings::Instance().ShowProductionInfo);
			TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmShowAccountPurchases, TGlobalSettings::Instance().ShowAccountPurchases);
			TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmShowAccountBalancesTabs, TGlobalSettings::Instance().ShowAccountBalancesTabs);
			TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmShowAccountBalances, TGlobalSettings::Instance().ShowAccountBalances);
			TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmShowHourlySales, TGlobalSettings::Instance().ShowHourlySales);
			TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmRevenueFiguresAreTaxAndServiceChargeInclusive, TGlobalSettings::Instance().RevenueFiguresAreTaxAndServiceChargeInclusive);
            TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmMidPointRoundsDown, TGlobalSettings::Instance().MidPointRoundsDown);
            TManagerVariable::Instance().GetProfileInt(DBTransaction,  GlobalProfileKey, vmSiteID, TGlobalSettings::Instance().SiteID);
            TManagerVariable::Instance().GetProfileInt(DBTransaction,  GlobalProfileKey, vmRoundingTime, TGlobalSettings::Instance().RoundingTime);
            TManagerVariable::Instance().GetProfileInt(DBTransaction, GlobalProfileKey, vmPosGuiDimensionsColumns, TGlobalSettings::Instance().DefaultColumns);
            TManagerVariable::Instance().GetProfileInt(DBTransaction, GlobalProfileKey, vmPosGuiDimensionsRows, TGlobalSettings::Instance().DefaultRows);
            TManagerVariable::Instance().GetProfileInt(DBTransaction, GlobalProfileKey, vmPosGridFontSize, reinterpret_cast<int &>(TGlobalSettings::Instance().posButtonFontSize));
            TManagerVariable::Instance().GetProfileInt(DBTransaction, GlobalProfileKey, vmChefMateColour, TGlobalSettings::Instance().KitchenColour);
            TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmBarExchangeServerGlobalEnabled, TGlobalSettings::Instance().BarExchangeServerGlobalEnabled);
            // Tax Settings
            TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmItemPriceIncludeTax,                TGlobalSettings::Instance().ItemPriceIncludeTax);
            TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmItemPriceIncludeServiceCharge,      TGlobalSettings::Instance().ItemPriceIncludeServiceCharge);
            TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmCalculateTaxPostDiscount,           TGlobalSettings::Instance().ReCalculateTaxPostDiscount);
            TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmCalculateServiceChargePostDiscount, TGlobalSettings::Instance().ReCalculateServiceChargePostDiscount);
            TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmUsingServiceCharge,                 TGlobalSettings::Instance().UsingServiceCharge);
            TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmApplyServiceChargeTax,              TGlobalSettings::Instance().ApplyServiceChargeTax);
            TManagerVariable::Instance().GetProfileInt( DBTransaction, GlobalProfileKey, vmServiceChargeTaxProfileKey,         TGlobalSettings::Instance().ServiceChargeTaxProfileKey);
            TManagerVariable::Instance().GetProfileNum( DBTransaction, GlobalProfileKey, vmServiceChargeTaxRate,               TGlobalSettings::Instance().ServiceChargeTaxRate);
            TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmIsCloudSynced, TGlobalSettings::Instance().IsCloudSyncedForDiscount);
            InitializeMallExportConfig(DBTransaction);

        }

        TGlobalSettings::Instance().XeroMachineName	= TManagerVariable::Instance().GetStr(DBTransaction, vmXeroMachineName,"");
        TGlobalSettings::Instance().XeroUserName	= TManagerVariable::Instance().GetStr(DBTransaction, vmXeroUserName, "");
        TGlobalSettings::Instance().XeroPassword	= TManagerVariable::Instance().GetStr(DBTransaction, vmXeroPassword, "");
        TGlobalSettings::Instance().XeroFolderPath	= TManagerVariable::Instance().GetStr(DBTransaction, vmXeroFolderPath, "");
        TGlobalSettings::Instance().MYOBMachineName	= TManagerVariable::Instance().GetStr(DBTransaction, vmMYOBMachineName,"");
        TGlobalSettings::Instance().MYOBUserName	= TManagerVariable::Instance().GetStr(DBTransaction, vmMYOBUserName, "");
        TGlobalSettings::Instance().MYOBPassword	= TManagerVariable::Instance().GetStr(DBTransaction, vmMYOBPassword, "");
        TGlobalSettings::Instance().MYOBFolderPath	= TManagerVariable::Instance().GetStr(DBTransaction, vmMYOBFolderPath, "");
        TGlobalSettings::Instance().EnablePhoneOrders = TManagerVariable::Instance().GetBool(DBTransaction, vmEnablePhoneOrders, false);
        TGlobalSettings::Instance().LoyaltyMateEnabled = TManagerVariable::Instance().GetBool(DBTransaction, vmLoyaltyMateEnabled, false);
        TGlobalSettings::Instance().ServiceURL = TManagerVariable::Instance().GetStr(DBTransaction, vmServiceURL, "");

        TGlobalSettings::Instance().MemberActivationURL = TManagerVariable::Instance().GetStr(DBTransaction, vmMemberActivationURL, "");
        TGlobalSettings::Instance().EnableActivationTokenPrint = TManagerVariable::Instance().GetBool(DBTransaction, vmEnableActivationTokenPrint, false);
        TGlobalSettings::Instance().ActivationTokenGreeting = TManagerVariable::Instance().GetStr(DBTransaction, vmActivationTokenGreeting, "");
        TGlobalSettings::Instance().ShowCustomerDisplay = TManagerVariable::Instance().GetBool(DBTransaction,vmShowCustomerDisplay,false);
        TGlobalSettings::Instance().EnableParkedSales = TManagerVariable::Instance().GetBool(DBTransaction,vmEnableParkedSales,false);
        TGlobalSettings::Instance().EnableActivationTokenPrint = TManagerVariable::Instance().GetBool(DBTransaction, vmEnableActivationTokenPrint, false);
        TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmShowAccumulatedZed, TGlobalSettings::Instance().ShowAccumulatedZeds);
        TGlobalSettings::Instance().PrintZeroTaxes =       TManagerVariable::Instance().GetBool(DBTransaction, vmPrintZeroTaxes, false);
        TGlobalSettings::Instance().EnableDisplayTaxRemoval = TManagerVariable::Instance().GetBool(DBTransaction, vmEnableDisplayTaxRemoval, false);
        TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmShowTaxSummary, TGlobalSettings::Instance().ShowTaxSummary);
        TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmShowServiceChargeSummary, TGlobalSettings::Instance().ShowServiceChargeSummary);
        TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmEnableClientDetails, TGlobalSettings::Instance().EnableClientDetails);
        TGlobalSettings::Instance().BarcodeFormat = TManagerVariable::Instance().GetInt(DBTransaction,vmBarcodeFormat,0);
        TManagerVariable::Instance().GetProfileBool(DBTransaction, TManagerVariable::Instance().DeviceProfileKey,
        vmEnableTableDisplayMode, TGlobalSettings::Instance().EnableTableDisplayMode );
        TGlobalSettings::Instance().UseTierLevels = TManagerVariable::Instance().GetBool(DBTransaction, vmUseTierLevels, false);
        TGlobalSettings::Instance().BirthdayBuffer = TManagerVariable::Instance().GetInt(DBTransaction, vmBirthdayBuffer, 1);
        TGlobalSettings::Instance().FirstVisitPoint = TManagerVariable::Instance().GetInt(DBTransaction, vmFirstVisitPoint, 1);
        TGlobalSettings::Instance().AllowMemberDetailscreen = TManagerVariable::Instance().GetBool(DBTransaction, vmAllowMemberDetailscreen, false);

        TGlobalSettings::Instance().AllowPointPaymentByWeight = TManagerVariable::Instance().GetBool(DBTransaction, vmAllowPointPaymentByWeight, false);
        TGlobalSettings::Instance().AllowPointPaymentByValue = TManagerVariable::Instance().GetBool(DBTransaction, vmAllowPointPaymentByValue, true);
        TGlobalSettings::Instance().PrintNoticeOnTransfer = TManagerVariable::Instance().GetBool(DBTransaction, vmPrintNoticeOnTransfer, false);
        TGlobalSettings::Instance().UpdateTableGUIOnOrderStatus = TManagerVariable::Instance().GetBool(DBTransaction, vmUpdateTableGUIOnOrderStatus, true);
        TGlobalSettings::Instance().ShowCurrencySymbol = TManagerVariable::Instance().GetBool(DBTransaction, vmShowCurrencySymbol, true);


        bool enableEmailSettings = false;
        TManagerVariable::Instance().GetProfileBool(DBTransaction,TManagerVariable::Instance().DeviceProfileKey,vmEmailZedReport,enableEmailSettings);
        TGlobalSettings::Instance().EmailZedReports = enableEmailSettings;
        TGlobalSettings::Instance().SaveEmailId = TManagerVariable::Instance().GetStr(DBTransaction, vmSaveEmailId, "Email_Id");
        TGlobalSettings::Instance().BarExchangeSelect = TManagerVariable::Instance().GetStr(DBTransaction, vmBarExchangeSelect, "None");
        TGlobalSettings::Instance().BarExchangePath = TManagerVariable::Instance().GetStr(DBTransaction, vmBarExchangePath, "");

        TGlobalSettings::Instance().BarExchangeEnabled = TManagerVariable::Instance().GetBool(DBTransaction, vmBarExchangeEnabled, false);
        TGlobalSettings::Instance().BarExchangeServerEnabled = TManagerVariable::Instance().GetBool(DBTransaction, vmBarExchangeServerEnabled, false);
        TGlobalSettings::Instance().SkipConfirmationOnFastTender = TManagerVariable::Instance().GetBool(DBTransaction, vmSkipConfirmationOnFastTender, false);
        TGlobalSettings::Instance().TransferTableOnPrintPrelim = TManagerVariable::Instance().GetBool(DBTransaction, vmTransferTableOnPrintPrelim, false);


        TGlobalSettings::Instance().EmailPassword = TManagerVariable::Instance().GetStr(DBTransaction, vmEmailPassword, "Email_Password");
        TGlobalSettings::Instance().EmailHost = TManagerVariable::Instance().GetStr(DBTransaction, vmEmailHost, "Email_Host");
        TGlobalSettings::Instance().EmailId = TManagerVariable::Instance().GetStr(DBTransaction, vmEmailId, "Email_Id");
        TGlobalSettings::Instance().EmailHostPort = TManagerVariable::Instance().GetInt(DBTransaction, vmEmailHostPort, 0);
        TGlobalSettings::Instance().RoundOnBilling = TManagerVariable::Instance().GetBool(DBTransaction, vmRoundOnBilling, false);
        TGlobalSettings::Instance().RoundOnBillingAmount = TManagerVariable::Instance().GetNum(DBTransaction, vmRoundOnBillingAmount, 0.01);


        //MM-4579
        TGlobalSettings::Instance().PointPurchased = TManagerVariable::Instance().GetBool(DBTransaction, vmPointPurchased, false);
        TGlobalSettings::Instance().PointRedeem = TManagerVariable::Instance().GetBool(DBTransaction, vmPointRedeemed, false);
        TGlobalSettings::Instance().PointEarned =TManagerVariable::Instance().GetBool(DBTransaction, vmPointEarned, false);
        TGlobalSettings::Instance().ResetDay = TManagerVariable::Instance().GetInt(DBTransaction, vmResetDay, 1);

        TGlobalSettings::Instance().IsDrinkCommandEnabled = TManagerVariable::Instance().GetBool(DBTransaction, vmIsDrinkCommandEnabled, false);
        TGlobalSettings::Instance().DrinkCommandServerPath = TManagerVariable::Instance().GetStr(DBTransaction, vmDrinkCommandServerPath, "localhost");
        TGlobalSettings::Instance().DrinkCommandServerPort = TManagerVariable::Instance().GetInt(DBTransaction, vmDrinkCommandServerPort, 15090);
        TManagerVariable::Instance().GetProfileBool(DBTransaction, GlobalProfileKey, vmShowRemovalReport, TGlobalSettings::Instance().ShowRemovalReport);

        TGlobalSettings::Instance().EnableCancelCheckRemoval = TManagerVariable::Instance().GetBool(DBTransaction, vmEnableCancelCheckRemoval, true);
        TGlobalSettings::Instance().RevenueFiguresAreDiscountInclusive = TManagerVariable::Instance().GetBool(DBTransaction, vmRevenueFiguresAreDiscountInclusive, true);
        TGlobalSettings::Instance().ShowPriceAdjustment = TManagerVariable::Instance().GetBool(DBTransaction, vmShowPriceAdjustment, true);
        TGlobalSettings::Instance().MandatoryMembershipCard = TManagerVariable::Instance().GetBool(DBTransaction, vmMandatoryMembershipCard, false);
        TManagerVariable::Instance().GetProfileBool(DBTransaction, TManagerVariable::Instance().DeviceProfileKey,vmPointOnly, TGlobalSettings::Instance().PointOnly );    //MM 4775
        TGlobalSettings::Instance().EnableApprovedLogin = TManagerVariable::Instance().GetBool(DBTransaction, vmEnableApprovedLogin, false);
        TGlobalSettings::Instance().ClockInOutApprovedLogin = TManagerVariable::Instance().GetBool(DBTransaction, vmClockInOutApprovedLogin, false);
        TGlobalSettings::Instance().PromptPatronCountOnTableSales = TManagerVariable::Instance().GetBool(DBTransaction, vmPromptPatronCountOnTableSales, false);
        TGlobalSettings::Instance().PostZToAccountingSystem = TManagerVariable::Instance().GetBool(DBTransaction, vmPostZToAccountingSystem, false);

        //MM 5610
        TGlobalSettings::Instance().ConfirmResetPoint = TManagerVariable::Instance().GetBool(DBTransaction, vmConfirmResetPoint, false);
        TGlobalSettings::Instance().EnableEftPosSmartPay = TManagerVariable::Instance().GetBool(DBTransaction, vmEnableEftPosSmartPay, false);
        TGlobalSettings::Instance().EftPosSmartPayIp = TManagerVariable::Instance().GetStr(DBTransaction, vmEftPosSmartPayIp, "127.0.0.1");

        TGlobalSettings::Instance().NewBook = TManagerVariable::Instance().GetInt(DBTransaction, vmNewBook, 0);
        TGlobalSettings::Instance().PointsPurchasedGLCode	= TManagerVariable::Instance().GetStr(DBTransaction, vmPointsPurchasedGLCode, "300");
        TGlobalSettings::Instance().PointsSpentGLCode	= TManagerVariable::Instance().GetStr(DBTransaction, vmPointsSpentGLCode, "301");
        TGlobalSettings::Instance().VoucherPurchasedGLCode	= TManagerVariable::Instance().GetStr(DBTransaction, vmVoucherPurchasedGLCode, "302");
        TGlobalSettings::Instance().ServiceChargeGLCode	= TManagerVariable::Instance().GetStr(DBTransaction, vmServiceChargeGLCode, "400");
        TGlobalSettings::Instance().ServiceChargeTaxGLCode	= TManagerVariable::Instance().GetStr(DBTransaction, vmServiceChargeTaxGLCode, "401");
        TGlobalSettings::Instance().RoundingGLCode	= TManagerVariable::Instance().GetStr(DBTransaction, vmRoundingGLCode, "860");
        TGlobalSettings::Instance().GiftCardOnly = TManagerVariable::Instance().GetBool(DBTransaction, vmGiftCardOnly, false);
        TGlobalSettings::Instance().IsClippIntegrationEnabled = TManagerVariable::Instance().GetBool(DBTransaction, vmIsClippIntegrationEnabled, false);//4894
        TGlobalSettings::Instance().IsMemberSalesOnlyEnabled = TManagerVariable::Instance().GetBool(DBTransaction, vmIsMemberSalesOnlyEnabled, false);
        TGlobalSettings::Instance().ClippTabStartIndex = TManagerVariable::Instance().GetInt(DBTransaction, vmClippTabStartIndex, 800);
        TGlobalSettings::Instance().ClippTabEndIndex = TManagerVariable::Instance().GetInt(DBTransaction, vmClippTabEndIndex, 990);
        TGlobalSettings::Instance().DeleteItemSizeAfterSale = TManagerVariable::Instance().GetBool(DBTransaction, vmDeleteItemSizeAfterSale, false);
        TGlobalSettings::Instance().ShowItemSizesAsList = TManagerVariable::Instance().GetBool(DBTransaction, vmShowItemSizesAsList, false);
        TGlobalSettings::Instance().ShowItemPriceInTransferScreen = TManagerVariable::Instance().GetBool(DBTransaction, vmShowItemPriceInTransferScreen, false);
        TGlobalSettings::Instance().NoOfPriceLevels = TManagerVariable::Instance().GetInt(DBTransaction, vmNoOfPriceLevels, 2);
        TGlobalSettings::Instance().TerminalExemptFromHappyHour = TManagerVariable::Instance().GetBool(DBTransaction, vmTerminalExemptFromHappyHour, false);
        TGlobalSettings::Instance().NotifyForManuallyEnteredWeight = TManagerVariable::Instance().GetBool(DBTransaction, vmNotifyForManuallyEnteredWeight, false);


        //MM 6148 Thorlink
        TGlobalSettings::Instance().IsThorlinkEnabled = TManagerVariable::Instance().GetBool(DBTransaction, vmIsThorlinkEnabled, false);
        TGlobalSettings::Instance().ThorlinkAppKey = TManagerVariable::Instance().GetStr(DBTransaction, vmThorlinkAppKey, "AppKey");
        TGlobalSettings::Instance().ThorlinkSiteNo = TManagerVariable::Instance().GetStr(DBTransaction, vmThorlinkSiteNo,"SiteNo");
        TGlobalSettings::Instance().ThorlinkMerchantCode = TManagerVariable::Instance().GetStr(DBTransaction, vmThorlinkMerchantCode, "Merchant Code");
        TGlobalSettings::Instance().ThorlinkDeviceCode = TManagerVariable::Instance().GetStr(DBTransaction, vmThorlinkDeviceCode, "Device Code");
        TGlobalSettings::Instance().AwaitingPaymentInXero = TManagerVariable::Instance().GetBool(DBTransaction, vmAwaitingPaymentInXero, false);
        TGlobalSettings::Instance().PostMoneyToGlAccounts  = TManagerVariable::Instance().GetBool(DBTransaction, vmPostMoneyToGlAccounts, false);
        TGlobalSettings::Instance().PostMoneyAsPayment  = TManagerVariable::Instance().GetBool(DBTransaction, vmPostMoneyAsPayment, false);
        TGlobalSettings::Instance().TabDepositCreditReceivedGLCode	= TManagerVariable::Instance().GetStr(DBTransaction, vmTabDepositCreditReceivedGLCode, "200");
        TGlobalSettings::Instance().TabDepositCreditRefundedGLCode	= TManagerVariable::Instance().GetStr(DBTransaction, vmTabDepositCreditRefundedGLCode, "200");
        TGlobalSettings::Instance().FloatGLCode	= TManagerVariable::Instance().GetStr(DBTransaction, vmFloatGLCode, "");
        TGlobalSettings::Instance().SetTextFontSizeOnCustomerDisplay = TManagerVariable::Instance().GetBool(DBTransaction, vmSetTextFontSize, false);
        TGlobalSettings::Instance().CustomerDisplayFontSize = TManagerVariable::Instance().GetStr(DBTransaction, vmSetCustomerDisplayFontSize, "");
        TGlobalSettings::Instance().WeightLimit = TManagerVariable::Instance().GetNum(DBTransaction, vmWeightLimit, 0.000);
        //add
        TGlobalSettings::Instance().ShowScreenToSelectItemForPoint = TManagerVariable::Instance().GetBool(DBTransaction, vmShowScreenToSelectItemForPoint, false);
        TGlobalSettings::Instance().PontsSpentCountedAsRevenue = TManagerVariable::Instance().GetBool(DBTransaction, vmPontsSpentCountedAsRevenue, false);
        TGlobalSettings::Instance().RunRateBoardIP = TManagerVariable::Instance().GetStr(DBTransaction, vmRunRateBoardIP, "");
        TGlobalSettings::Instance().IsRunRateBoardEnabled = TManagerVariable::Instance().GetBool(DBTransaction, vmRunRateBoardEnabled, false);
        TGlobalSettings::Instance().ShowLargeFonts = TManagerVariable::Instance().GetBool(DBTransaction, vmShowLargeFonts, false);
        TGlobalSettings::Instance().ItemSearch = TManagerVariable::Instance().GetBool(DBTransaction, vmItemSearch, false);
        TGlobalSettings::Instance().ShowDarkBackground = TManagerVariable::Instance().GetBool(DBTransaction, vmShowDarkBackgroundInPOS, false);        
	    TGlobalSettings::Instance().IsXeroEnabled = TManagerVariable::Instance().GetBool(DBTransaction, vmIsXeroEnabled, false);
        TGlobalSettings::Instance().IsMYOBEnabled = TManagerVariable::Instance().GetBool(DBTransaction, vmIsMYOBEnabled, false);
        TGlobalSettings::Instance().OpenCashDrawer = TManagerVariable::Instance().GetBool(DBTransaction, vmOpenCashDrawer, false);
        TGlobalSettings::Instance().HideReceiptNumberForRefundItem = TManagerVariable::Instance().GetBool(DBTransaction, vmHideReceiptNumberForRefundItem, false);
        TGlobalSettings::Instance().MergeSimilarItem = TManagerVariable::Instance().GetBool(DBTransaction, vmMergeSimilarItem, false);

        TGlobalSettings::Instance().CaptureRefundRefNo = TManagerVariable::Instance().GetBool(DBTransaction, vmCaptureRefundRefNo, false);
        TGlobalSettings::Instance().HideTaxInvoice = TManagerVariable::Instance().GetBool(DBTransaction, vmHideTaxInvoice, false);
        TGlobalSettings::Instance().ExportReprintReceipt = TManagerVariable::Instance().GetBool(DBTransaction, vmExportReprintReceipt, false);
        TGlobalSettings::Instance().SetVoidFooter = TManagerVariable::Instance().GetBool(DBTransaction, vmSetVoidFooter, false);
        TGlobalSettings::Instance().SetSubHeader = TManagerVariable::Instance().GetBool(DBTransaction, vmSetSubHeader, false);
        TGlobalSettings::Instance().ReceiptDigits = TManagerVariable::Instance().GetStr(DBTransaction, vmReceiptDigits, "0");
        TGlobalSettings::Instance().ShowVoidOrRefund = TManagerVariable::Instance().GetBool(DBTransaction, vmShowVoidOrRefund, false);
        TGlobalSettings::Instance().ShowVoidNumber = TManagerVariable::Instance().GetBool(DBTransaction, vmShowVoidNumber, false);
        TGlobalSettings::Instance().EftPosTipGLCode	= TManagerVariable::Instance().GetStr(DBTransaction, vmEftPosTipGLCode, "860");
        TGlobalSettings::Instance().GiftCardValidation = TManagerVariable::Instance().GetInt(DBTransaction, vmGiftCardValidation, 0);
        TGlobalSettings::Instance().EftPosTipGLCode	= TManagerVariable::Instance().GetStr(DBTransaction, vmEftPosTipGLCode, "860");
        TGlobalSettings::Instance().ExcludeReceipt	= TManagerVariable::Instance().GetBool(DBTransaction, vmExcludeReceipt, false);
        TGlobalSettings::Instance().ExcludeXReport	= TManagerVariable::Instance().GetBool(DBTransaction, vmExcludeXReport, false);
        TGlobalSettings::Instance().EftPosTipGLCode	= TManagerVariable::Instance().GetStr(DBTransaction, vmEftPosTipGLCode, "860");
        TGlobalSettings::Instance().UseBIRFormatInXZReport = TManagerVariable::Instance().GetBool(DBTransaction, vmUseBIRFormatInXZReport, false);
        TGlobalSettings::Instance().EftPosTipGLCode	= TManagerVariable::Instance().GetStr(DBTransaction, vmEftPosTipGLCode, "860");
        TGlobalSettings::Instance().HideRoundingOnReceipt = TManagerVariable::Instance().GetBool(DBTransaction, vmHideRoundingOnReceipt, false);
		TGlobalSettings::Instance().CashDenominationEntry = TManagerVariable::Instance().GetBool(DBTransaction, vmCashDenominationEntry, false);
        TGlobalSettings::Instance().MembershipPaid = TManagerVariable::Instance().GetBool(DBTransaction, vmMembershipPaid, false);
}

void TMMSettings::InitializeMallExportConfig(Database::TDBTransaction &DBTransaction)
{
	int GlobalProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction, eSystemProfiles, "Globals");
	if (GlobalProfileKey != 0)
	{
        //Mall Export                                  vmPPlantPrevTime
        TManagerVariable::Instance().GetProfileInt(DBTransaction,GlobalProfileKey,vmBatchNo, TGlobalSettings::Instance().BatchNo);
        TManagerVariable::Instance().GetProfileInt(DBTransaction,GlobalProfileKey,vmZCount, TGlobalSettings::Instance().ZCount);
        TManagerVariable::Instance().GetProfileInt(DBTransaction,GlobalProfileKey,vmPPlantCount, TGlobalSettings::Instance().PPlantCount);
        TManagerVariable::Instance().GetProfileInt(DBTransaction,GlobalProfileKey,vmPPlantPrevTime, TGlobalSettings::Instance().PPlantPrevTime);
        TManagerVariable::Instance().GetProfileInt(DBTransaction,GlobalProfileKey,vmPPlantHourly, TGlobalSettings::Instance().PPlantHourly);
        TManagerVariable::Instance().GetProfileInt(DBTransaction,GlobalProfileKey,vmPPlantFlag, TGlobalSettings::Instance().PPlantFlag);
        TManagerVariable::Instance().GetProfileStr(DBTransaction,GlobalProfileKey,vmFirstMallDate, TGlobalSettings::Instance().FirstMallDate);
        TManagerVariable::Instance().GetProfileStr(DBTransaction,GlobalProfileKey,vmLastTransactionDate, TGlobalSettings::Instance().LastTransactionDate);
        TManagerVariable::Instance().GetProfileBool(DBTransaction,GlobalProfileKey,vmFirstMallSet, TGlobalSettings::Instance().FirstMallSet);
        TManagerVariable::Instance().GetProfileInt(DBTransaction,GlobalProfileKey,vmCMFileSerialNumber, TGlobalSettings::Instance().CMFileSerialNumber);
        TManagerVariable::Instance().GetProfileStr(DBTransaction,GlobalProfileKey,vmLastZedDate, TGlobalSettings::Instance().LastZedDate);
        TManagerVariable::Instance().GetProfileStr(DBTransaction,GlobalProfileKey,vmLastZedTime, TGlobalSettings::Instance().LastZedTime);
        TManagerVariable::Instance().GetProfileStr(DBTransaction,GlobalProfileKey,vmAmountValue, TGlobalSettings::Instance().AmountValue);
    }

    TManagerVariable::Instance().GetProfileInt(DBTransaction,TManagerVariable::Instance().DeviceProfileKey,vmMallIndex, TGlobalSettings::Instance().MallIndex);
    TManagerVariable::Instance().GetProfileStr(DBTransaction,TManagerVariable::Instance().DeviceProfileKey,vmClassCode, TGlobalSettings::Instance().ClassCode);
    TManagerVariable::Instance().GetProfileStr(DBTransaction,TManagerVariable::Instance().DeviceProfileKey,vmOutletCode, TGlobalSettings::Instance().OutletCode);
    TManagerVariable::Instance().GetProfileStr(DBTransaction,TManagerVariable::Instance().DeviceProfileKey,vmMallPath, TGlobalSettings::Instance().MallPath);
    TManagerVariable::Instance().GetProfileStr(DBTransaction,TManagerVariable::Instance().DeviceProfileKey,vmTenantNo, TGlobalSettings::Instance().TenantNo);
    TManagerVariable::Instance().GetProfileStr(DBTransaction,TManagerVariable::Instance().DeviceProfileKey,vmTradeCode, TGlobalSettings::Instance().TradeCode);
    TManagerVariable::Instance().GetProfileStr(DBTransaction,TManagerVariable::Instance().DeviceProfileKey,vmBranchCode, TGlobalSettings::Instance().BranchCode);
    TManagerVariable::Instance().GetProfileStr(DBTransaction,TManagerVariable::Instance().DeviceProfileKey,vmSerialNo, TGlobalSettings::Instance().SerialNo);
    TManagerVariable::Instance().GetProfileStr(DBTransaction,TManagerVariable::Instance().DeviceProfileKey,vmTerminalNo, TGlobalSettings::Instance().TerminalNo);

    TManagerVariable::Instance().GetProfileStr(DBTransaction,TManagerVariable::Instance().DeviceProfileKey,vmFTPServer, TGlobalSettings::Instance().FTPServer);
    TManagerVariable::Instance().GetProfileStr(DBTransaction,TManagerVariable::Instance().DeviceProfileKey,vmFTPPath, TGlobalSettings::Instance().FTPPath);
    TManagerVariable::Instance().GetProfileStr(DBTransaction,TManagerVariable::Instance().DeviceProfileKey,vmFTPUserName, TGlobalSettings::Instance().FTPUserName);
    TManagerVariable::Instance().GetProfileStr(DBTransaction,TManagerVariable::Instance().DeviceProfileKey,vmFTPPassword, TGlobalSettings::Instance().FTPPassword);
    TManagerVariable::Instance().GetProfileStr(DBTransaction,TManagerVariable::Instance().DeviceProfileKey,vmDLFMallFileName, TGlobalSettings::Instance().DLFMallFileName);
    TManagerVariable::Instance().GetProfileStr(DBTransaction,TManagerVariable::Instance().DeviceProfileKey,vmConsolidateReportPaths, TGlobalSettings::Instance().ConsolidateReportPaths);
    TManagerVariable::Instance().GetProfileBool(DBTransaction,TManagerVariable::Instance().DeviceProfileKey,vmEnableConsolidatedReport, TGlobalSettings::Instance().EnableConsolidatedReport);
    }

