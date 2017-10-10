// ---------------------------------------------------------------------------

#pragma hdrstop

#include "GlobalSettings.h"
#include "ManagerVariable.h"
// ---------------------------------------------------------------------------

#pragma package(smart_init)

using Database::TDBTransaction;

TGlobalSettings::TGlobalSettings()
	: Header(new TStringList),PHeader(new TStringList),Footer(new TStringList),ZedHeader(new TStringList), VoidFooter(new TStringList),SubHeader(new TStringList)
{
    InfoSync = 1;
    KitchenOrderNumber = 0;
    ForceHappyHour = false;
    InHappyHour = false;
    AutoMateIP = "";
    StockInterbaseIP = "";
    StockDatabasePath = " ";
    SntpIP = "";
    MembershipDatabaseIP = "";
    MembershipDatabasePath = "";
    MembershipDatabasePort = 10061;
    MembershipTillID = 1;
    TabsEnabled = true;
    TablesEnabled = true;
    KitchenColour = clRed;
    PoleDisplayTopLine = "MenuMate";
    PoleDisplayBottomLine = "MenuMate P.O.S";
    AlwaysForceLogin = false;
    AutoLogoutPOS = false;
    AutoLogoutSeconds = 0;
    ForcedReasons = false;
    RevertToFirstCourse = true;
    ForcedSidesFirst = false;
    NonServicePeriodMinutes = 30;
    ClearLogsEveryXDays = 0;
    HappyHourExempt = false;
    HappyHourMembersOnly = false;
    MembershipType = MembershipTypeMenuMate;
    ShowServiceChargeTaxWithSalesTax = false;
    PrintStockWeborders = false;
    EnableEftPosSyncro = false;
    EnableEftPosANZ = false;
    EnableEftPosIngenico = false;
    EnableEftPosCadmus = false;
    EnableEftPosCadmusCronos = false;
    EnableEftPosIceLink = false;
    EnableEftPosDPS = false;
    EnableDPSTipping = false;
    EnableDisplayTax = false;
    EnableDisplayServiceCharge = false;
    IncludeServiceChargeTaxInTaxValue = false;
    EnableNmiDisplay = false;
    AutoAddWebMembers = true;
    CaptureCustomerName = false;
    HideFreeItemsAtBilling = false;
    EnableWaiterStation = false;
    DuplicateEftPosReceipt = true;
    PrintSignatureReceiptsTwice = false;
    SaveAndPrintPrintsPartialOrder = false;
    PointsPaymentGroupNumber = 0;
    CreditPaymentGroupNumber = 0;
    RoundingTime = 0;
    HappyHourStart = 0;
    HappyHourFinish = 0;
    HappyHourDays[eSunday] = false;
    HappyHourDays[eMonday] = false;
    HappyHourDays[eTuesday] = false;
    HappyHourDays[eWednesday] = false;
    HappyHourDays[eThursday] = false;
    HappyHourDays[eFriday] = false;
    HappyHourDays[eSaturday] = false;
    EndOfDay = 5;
    ShowSessionDateInZed = false;
    ShowPointsOnZed = true;
    ShowInvoiceInfoOnZed = true;
    HideServingCourseLabels = false;
    RememberLastServingCourse = false;
    PromptForPatronCount = false;
    LuckyMemberPeriod = 24;
    LuckyDrawByLocationOnly = false;
    DuplicateReceipts = false;
    AutoPrintTabReceipts = false;
    AutoPrintStaffReceipts = false;
    AutoPrintMemberReceipts = false;
    AutoPrintTableReceipts = false;
    AutoPrintCashReceipts = false;
    AutoPrintRoomReceipts = false;
    AutoPrintInvoiceReceipts = false;
    AutoPrintWebReceipts = false;
    DefaultToBevMenu = false;
    SummariseDiscountOnZed = false;
    AutoConvertScalesWeights = false;
    EnableAARewards = false;
    EnableAARewardsReminder = false;
    DisableInternalInvoicing = false;
    EnableBlindBalances = false;
    EnableStaffHours = false;
    EnableCommission = false;
    EnablePrinterCounts = false;
    EnablePaxCount = false;
    DisableConfirmationOnSave = false;
    DisableReceiptOnConfirmation = false;
    ShowVoucherDetails = false;
    EnablePhoneOrders = false;
    EnableBirthdayNotifications = false;
    AcknowledgeFirstVisit = false;
    EnableMemberSpendChits = false;
    MemberSpendChitsAmount = 5.00;
    MidPointRoundsDown = true;
    SiteID = -1;
    IntaMateIPAddress = ""; ;
    IntaMatePort = 0;
    IntaMateTCPTimeOut_ms = 60000;
    IntaMateTerminalID = 0;
    WebMatePort = 65006;
    WebMateEnabled = false;
    AutoAcceptWebOrders = false;
    UnRavelKitchenPrinting = false;
    SeatLabel = "Guest";
    DefaultColumns = 4;
    DefaultRows = 8;
    posButtonFontSize = 0x8A;
    ReservationsEnabled = false;
    ReservationsAddress = "";
    FloorPlanTransparentTables = false;
    FloorPlanTableColour = clBlack;
    FloorPlanTextColour = clWhite;
    LastSelectedFloorPlanLocationID = -1;
    EnableHideCredsCancels = false;
    EnableDepositBagNum = false;
    EnableSeperateEarntPts = false;
    EnableDontClearZedData = false;
    SpendPtsOnlyFullProduct = false;
    SpendPtsOnlySelectItems = false;
    EarntPointsOnWeight = false;
    EnableHoldSend = false;
    EnableMenuPatronCount = false;
    ShowPointsOnDisplay = false;
    NoPtsEarnedDuringHappyHour = false;
    QMSIsEnabled = QMSDateOfBirthIsMandatory =
    QMSEmailAddressIsMandatory = QMSPhoneNumberIsMandatory = false;
    XeroMachineName = "";
    XeroUserName    = "";
    XeroPassword    = "";
    XeroFolderPath  = "";
    MYOBMachineName = "";
    MYOBUserName    = "";
    MYOBPassword    = "";
    MYOBFolderPath  = "";
    NagUserToSelectChit = false;
    LoyaltyMateEnabled = false;
    ServiceURL = "";
    MemberActivationURL = "";
    EnableActivationTokenPrint = false;
    ActivationTokenGreeting = "";
    ReceiptNumberLabel="";
    RefundReferenceLabel = "";
    ReprintReceiptLabel="";
    //Making the first setting for initialize tax setting
    ItemPriceIncludeTax = true;
    ItemPriceIncludeServiceCharge = false;
    ReCalculateTaxPostDiscount = true;
    ReCalculateServiceChargePostDiscount = true;
    UsingServiceCharge = true;
    ApplyServiceChargeTax = false;
    ServiceChargeTaxProfileKey = 0;
    ServiceChargeTaxRate = 0;
    //Making the first setting for Mall Export
    MallIndex = 0;
    ClassCode = "";
    OutletCode = "";
    MallPath = "";

    TenantNo = "";
    TradeCode = "";
    BranchCode = "";
    TerminalNo = "";
    SerialNo = "";
    // For Zed report count flag
    ZCount = 0;
    // For Robinsons and PowerPlant batch no
    BatchNo = 1;
    // For Ayala, Ronbinson and PowerPlant
    FirstMallSet = false;
    FirstMallDate = "";
    // Power Plant
    PPlantCount = 0;
    PPlantPrevTime = 0;
    PPlantHourly = 0;
    PPlantFlag = 0;
    // For Robinsons last used date
    LastTransactionDate = "";
    LastZedDate = "";
    LastZedTime = "";
    // Capitaland Serial number
    CMFileSerialNumber = 0;
    // Amount Storage
    AmountValue = "0.00";
    // Accumulated Zed
    ShowAccumulatedZeds = true;
    ExtendCurrencyDisplay = false;
    EnableDisplayTaxRemoval = false;
    PrintZeroTaxes = false;
    BarcodeFormat =0;
    AllowToEnterPrice = false;
    AllowToEnterWeight = false;
    // Tax Summary
    ShowTaxSummary = false;
    // Service Charge Summary
    ShowServiceChargeSummary = false;
    ShowServiceChargeTaxWithServiceCharge = false;
    // Enable Client Details
    EnableClientDetails = false;
    EnableTableDisplayMode = false;
    //warn user when there is open tables and open tabs
    WarnIfOpenTableAndTabs = true;
    //stop user when there is open tables and open tabs
    StopIfOpenTableAndTabs = false;
    ShowTransactionSummaryGroups = true;
    ShowBilledSalesTotals = true;
    ShowComplimentarySalesTotals = false;
    ShowChargedSalesTotals = true;
    ShowTotals = true;
    ShowBreakdownCategories = true;
    ShowDiscountReport = true;
    ShowPointsReport = true;
    ShowPatronAverages = true;
    ShowProductionInfo = true;
    ShowAccountPurchases = true;
    ShowAccountBalancesTabs = true;
    ShowAccountBalances = true;
    ShowHourlySales = false;
    RevenueFiguresAreTaxAndServiceChargeInclusive = true;
    ShowCustomerDisplay = false;
    UseTierLevels = false;
    BirthdayBuffer = 0;
    FirstVisitPoint=0;
    MinRedemptionPoint=0;
    EnableParkedSales = true;
    // FTP Exoport Settings
    FTPServer = "";
    FTPPath = "";
    FTPUserName = "";
    FTPPassword = "";
    AllowMemberDetailscreen=false;
    IsPOSOffline = false;
    //MM2038
    TabPrintName = "";
    TabPrintPhone = "";
    AllowPointPaymentByWeight = false;
    AllowPointPaymentByValue = true;
    PrintNoticeOnTransfer = false;
    UpdateTableGUIOnOrderStatus = false;
    //MM-3395
    AllowRefundPoints = false;
    /* BarExchange settings */
    BarExchangeEnabled= false;
    BarExchangePath="";
    RunRateBoardPath = "";
    BarExchangeSelect="None";
    BarExchangeServerEnabled = false;
    BarExchangeServerGlobalEnabled= false;
    ShowCurrencySymbol = false;

    //Drink Command Server Settings..
    IsDrinkCommandEnabled = false;
    DrinkCommandServerPath = "";
    DrinkCommandServerPort = 15090;

    //MM-2827
    ShowCurrencySymbol = false;
    //MM-4104
    EmailZedReports = false;
    SaveEmailId = "";
    EmailId = "";
    EmailPassword = "";
    EmailHost = "";
    EmailHostPort = 0;
    //MM4161
    MaximumChitNumber = 99;
    SkipConfirmationOnFastTender = false;
    RoundOnBilling = false;
    RoundOnBillingAmount= 0.01;
    TransferTableOnPrintPrelim = false;
    //MM-4579
    PointPurchased = false;
    PointEarned = false;
    PointRedeem=false;
    ResetDay = 1;

    ShowRemovalReport = true;
    EnableCancelCheckRemoval = false;
    RevenueFiguresAreDiscountInclusive = true;
    ShowPriceAdjustment = true;

    EnableApprovedLogin = false;
    ClockInOutApprovedLogin = false;
    PointOnly = false;
    PromptPatronCountOnTableSales = false;
    PostZToAccountingSystem = false;

    //MM 5610
    ConfirmResetPoint = false;
    EnableEftPosSmartPay = false;
    EftPosSmartPayIp = "";
    NewBook=0;
    PointsPurchasedGLCode= "";
    PointsSpentGLCode= "";
    VoucherPurchasedGLCode= "";
    ServiceChargeGLCode= "";
    ServiceChargeTaxGLCode= "";
    RoundingGLCode = "";
    RefundingItems = false;
    GiftCardOnly = false;
    IsClippIntegrationEnabled = false;
    IsMemberSalesOnlyEnabled = false;
    ClippTabStartIndex = 800;
    ClippTabEndIndex = 990;
    DeleteItemSizeAfterSale = false;
    ShowItemSizesAsList = false;
    ShowItemPriceInTransferScreen = false;
    NoOfPriceLevels = 2;
    TerminalExemptFromHappyHour = false;
    NotifyForManuallyEnteredWeight = false;
    //Thorlink Settings
    IsThorlinkEnabled = false;
    IsThorlinkTender = false;
    IsThorlinkSelected = false;
    IsThorVoucherSelected = false;
    IsProcessThorVoucher = false;
    IsDiscountSelected = false;
    IsThorPay = false;
    IsThorBillSelected = false;
    ThorlinkAppKey = "";
    ThorlinkSiteNo ="";
    ThorlinkMerchantCode = "";
    ThorlinkDeviceCode = "";  
    WeightLimit = 0.00;
    AwaitingPaymentInXero = false;
    PostMoneyToGlAccounts = false;
    PostMoneyAsPayment = false;
    TabDepositCreditReceivedGLCode = "";
    TabDepositCreditRefundedGLCode = "";
    FloatGLCode = "";
    SetTextFontSizeOnCustomerDisplay = false;
    CustomerDisplayFontSize = "";
    ShowScreenToSelectItemForPoint = false;
    DLFMallFileName = "";
    NotifyPOSForLastWebOrder = false;
    UpdateMenu = false;
    PontsSpentCountedAsRevenue = false;
    IsRunRateBoardEnabled = false;
    RunRateBoardIP = "";
    ShowLargeFonts = false;
    ItemSearch = false;
    IsXeroEnabled = false;
    IsMYOBEnabled = false;

    // MallExport Additional Settings
    ConsolidateReportPaths = "";
    EnableConsolidatedReport = false;
    ShowDarkBackground = false;
    OpenCashDrawer = false;
    CashWithdrawalGLCode = "";
    MergeSimilarItem = true;
    HideTaxInvoice = false;
    ExportReprintReceipt = false;
    CaptureRefundRefNo = false;
    SetVoidFooter = false;
    SetSubHeader = false;
    ReceiptDigits = "";
    ShowVoidOrRefund = false;
    ShowVoidNumber = false;
    EftPosTipGLCode = "";
    GiftCardValidation = 0;
    ExcludeReceipt = false;
    ExcludeXReport = false;
    UseBIRFormatInXZReport = false;
    HideRoundingOnReceipt = false;
	CashDenominationEntry = false;
    CSVPath = "";
    IsEnabledPeachTree = false;
    CSVExportIP = "";
    MembershipPaid = false;
    UseMemberSubs = false;
    FloatWithdrawFromCash = false;
    IsPanasonicIntegrationEnabled = false;
    PanasonicServerIP = "";
    CashVarianceGLCode = "";
    ReportExportPath = "";
    MembershipType = 0;
    IsBillSplittedByMenuType = false;
    CustomerId = "";
    EnableEftPosSmartConnect = false;
}

