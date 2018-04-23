/*
*  IMPORTANT: When you add new enum variables to this page, always add them to the end of the list.
*  Do not change the order of existing enums then you will break some parts of the code and database stuff as well.
*/

// ---------------------------------------------------------------------------

#ifndef ManagerVariableH
#define ManagerVariableH

#include <map>
#include "MM_DBCore.h"

// ---------------------------------------------------------------------------
enum ProfileType
{
	eLocationProfiles, eTerminalProfiles, eEquipmentProfiles, eNetworkProfiles, eSystemProfiles
};

const char* const vmProfileTypes[] =
{
	"Location Profiles", "POS Terminal Profiles", "Equipment Profiles", "Network Profiles", "System Profiles", NULL
};

enum vmGroup
{
	vmgMenuMateModules, vmg3rdPartyInterface, vmgPOS, vmgSecurity, vmgPrinting, vmgMiscEquipment, vmgHappyHour, vmgNetwork, vmgLogging,
	vmgMisc, vmgPrinters, vmgSystem, vmgNone, vmgTax
};

const char* const vmGroups[] =
{
	// Terminal Profile & Location Profile Groups ----------------------------------
	"MenuMate Modules", "3rd Party Interfaces", "POS Behaviour", "Security Settings", "Printing", "Misc Equipment", "Happy Hour",
	"Networking", "Logging", "Misc",
	// Equipment Profile Groups ----------------------------------------------------
	"Printers", "Pole Displays", "Cash Drawers", "System", NULL
};

enum vmVariables
{
	// System 	  ------------------------------------------------------------------
	vmbtnDollar1, vmbtnDollar2, vmbtnDollar3, vmbtnDollar4, vmbtnDollar5, Deprecated_vmEnabledStateSync, vmInfoSync, vmVersionNumber,
	vmMidPointRoundsDown, vmSiteID, vmMMSubNet, vmRootAccount, vmRootAccountTimer,

   // Path Config------------------------------------------------------------------
   vmAutoMateIP = 1000, vm_PLACEHOLDER_ChefMateIP, vmStockInterbaseIP, vmStockDatabasePath, vmSntpIP, vmMembershipDatabaseIP,
   vmMembershipDatabasePath, vmMembershipDatabasePort,
   // Modules    ------------------------------------------------------------------
   vm_PLACEHOLDER_GuestMasterPath = 2000, vmMembershipType, vmTabsEnabled, vmTablesEnabled, vmChefMateColour, vmEnableEftPosSyncro,
   vmEnableEftPosANZ, vmEnableEftPosIngenico, vmTrackSaleAndMakeTimes, vmEftposMerchantNumber, vmEnableEftPosCadmus, vmEftposSerialPort,
   vmEnableEftPosIceLink, vmEnableEftPosDPS, vmSmartCardMembership, vmEnableAARewards, vmEnableEftPosCadmusCronos,
   vmEnableAARewardsReminder,vmAutoAddWebMembers,vmAutoAcceptWebOrders,vmReservationsAddress,vmReservationsEnabled,
   vmFloorPlanTransparentTables,vmFloorPlanTableColour,vmFloorPlanTextColour,vmLastSelectedFloorPlanLocationID,
   vmExternalMembershipDiscountCode,vmExternalMembershipPointsCode,vmExternalMembershipSystemCode, vmEnableDPSTipping,vmEnableEftPosSmartPay,vmEftPosSmartPayIp,
   vmEnableEftPosSmartConnect, vmSmartConnectPairingCode,
   // Connections  ----------------------------------------------------------------
   vmPMSIPAddress = 2100, vmPMSTCPPort, vmPMSPOSID, vmPMSPaymentCategory, vmPMSItemCategory, vmPMSPointsCategory, vmPMSCreditCategory,
   vmPMSDefaultAccount, vmPMSDefaultSurchargeAccount, vmPMSDefaultRoundingAccount, vmIntaMateIPAddress, vmIntaMatePort,
   vmIntaMateTerminalID, vmIntaMateIPTimeOut, vmPocketVoucherURL, vmMembershipTillID,
   vmWebMatePort, vmWebMateEnabled, vmMaxGamingID, vmIsClippIntegrationEnabled, vmClippTabStartIndex, vmClippTabEndIndex,vmPMSTipAccount,vmPMSExpensesAccount,vmPMSServiceChargeAccount,
   vmPMSRoundingAccountSiHot, vmSiHotRounding,vmSiHotDefaultTransaction,vmSiHotDefaultTransactionName, vmEnableCustomerJourney,vmRevenueCentre,
	// Stock Master ----------------------------------------------------------------
	vm_PLACEHOLDER_UseStockMaster = 2200, vmStockMasterExportPath,

	// Kiosk ----------------------------------------------------------------
	vmKioskDisplayMode = 2300,vmKioskMembershipMode = 2301,

	// Misc Equip   ----------------------------------------------------------------
	vmPoleDisplayTopLine = 3000, vmPoleDisplayBottomLine, vmPoleDisplayPort, vmPoleDisplayEmulationMode, vmSerialKickPort,
	vmSerialKickPortLength, vmSerialKickPortHighChars, vm_PLACEHOLDER_ProximityPort, vmBarCodeReaderPort, vmBluetoothPort,
	vmBluetoothSerialTimeOut, vmSerialPort, vmSerialBaudRate, vmSerialDataBits, vmSerialStopBits, vmSerialParityBits,
	vmSerialRTSFlowControl, vmSerialOutCTSFlow, vmSerialXonXoffOut, vmSerialXonXoffIn, vmSecuritySerialProfileName, vmSerialAsync,
	vmSmartCardReader, vmSmartCardCode, vmProxWiegandFormat, vmScalesProfile, vmAutoConvertScalesWeights, vmProxProfile,
	vmScalesType, vmBarcodeFormat,vmAllowEnterWeight, vmAllowEnterPrice, vmShowCustomerDisplay, vmWeightLimit,  vmIsMemberSalesOnlyEnabled,  vmRunRateBoardEnabled ,
    vmRunRateBoardIP , vmItemSearch,
	// POS        ------------------------------------------------------------------
	vmAlwaysForceLogin = 4000, vmAutoLogoutPOS, vmAutoLogoutSeconds, vmForcedReasons, vmRevertToFirstCourse,
	vm_PLACEHOLDER_OptionsMultiSelect, vmForcedSidesFirst, vmForceTender, vmFTBypassElecTranTyp, vmNonServicePeriod, vmClearLogsDays,
	vmMaxLogsRecords, vm_PLACEHOLDER_MemberNumericPannelFirst, vmStaffNumericPannelFirst, vmDisableManualLogin, vmMemberIncrementalSearch,
	vmMemberNameOnPoleDisplay, vmMemberLoyaltyRewardsRules, vmLocationLoyaltyRewardsRules, vmMemberQuickButtonCount,
	vm_PLACEHOLDER_DisplayServingCourseColours, vmHideServingCourseLabels, vm_PLACEHOLDER_ShowSingleServingCourses,
	vmRememberLastServingCourse, vmPromptForPatronCount, vm_PLACEHOLDER_DefaultItemQuickMode, vmPOSDefaultsToBeverages,
	vmStaffMessageOfTheDay, vmAllowSmartCardNotPresentMemberTransactions, vmDisableInternalInvoicing, vmEnableBlindBalances,
	vmDisableConfirmationOnSave, vmDisableReceiptOnConfirmation, vmEnableBirthdayNotifications, vmAcknowledgeFirstVisit,
	vmEnableMemberSpendChits, vmMemberSpendChitsAmount, vmShowVoucher,vmPosGuiDimensionsColumns,vmPosGuiDimensionsRows,
	vmAutoZProfileKey, vmAutoZCheck1Time, vmAutoZCheck2Time, vmAutoZCheck3Time,
	vmAutoZCheck4Time, vmAutoZCheck1Options, vmAutoZCheck2Options, vmAutoZCheck3Options, vmAutoZCheck4Options, vmAutoZAuthAs,
	vmPosGridFontSize, vmEnableStaffHours, vmEnableCommission, vmEnablePrinterCounts, vmEnablePaxCount, vmPrintStockWeborders,
    vmSalesforceUploadEnabled, vmSalesforceUsername, vmSalesforcePassword, vmSalesforceSecurityToken, vmHideCredsCancels,
    vmEnableSeperateEarntPts, vmQuickMemberSetupEnabled, vmQuickMemberSetupPhoneIsMandatory, vmQuickMemberSetupEmailIsMandatory,
	vmQuickMemberSetupDateOfBirthIsMandatory, vmSalesforceUploadStock, vmDepositBagNum, vmDontClearZedData,	vmCaptureCustomer,
	vmHoldSend, vmHideFreeItemsAtBilling, vmNagUserToSelectChit, vmRoundingTime, vmEnablePhoneOrders, vmMenuPatronCount,
    vmEnableWaiterStation, vmEnableDisplayTax, vmEnableParkedSales, vmEnableDisplayServiceCharge, vmShowAccumulatedZed,
    vmEndOfDay, vmShowTaxSummary, vmShowServiceChargeSummary, vmEnableClientDetails, vmEnableDisplayTaxRemoval, vmExtendCurrencyDisplay,
    vmEnableTableDisplayMode, vmShowServiceChargeTax, vmWarnIfOpenTableAndTabs, vmStopIfOpenTableAndTabs, vmEnableNmiDisplay, vmInclServiceChargeTaxInTaxVal,
    vmCombineServiceChargeTaxWithSalesTax, vmCombineServiceChargeTaxWithServiceCharge, vmUpdateTableGUIOnOrderStatus, vmShowCurrencySymbol,
    vmSkipConfirmationOnFastTender, vmTransferTableOnPrintPrelim, vmEmailZedReport, vmSaveEmailId, vmEmailId, vmEmailPassword, vmEmailHost,
    vmEmailHostPort, vmShowRemovalReport, vmEnableCancelCheckRemoval, vmEnableApprovedLogin, vmClockInOutApprovedLogin, vmRoundOnBilling,vmRoundOnBillingAmount,
    vmPointPurchased, vmPointRedeemed, vmPointEarned, vmResetDay, vmPointOnly,vmConfirmResetPoint, vmPromptPatronCountOnTableSales, vmPostZToAccountingSystem,vmDeleteItemSizeAfterSale,
    vmPostMoneyToGlAccounts, vmPostMoneyAsPayment,
    vmIsDrinkCommandEnabled,
	vmDrinkCommandServerPath,
	vmDrinkCommandServerPort,
    vmNewBook,
    vmShowItemSizesAsList,
    vmShowItemPriceInTransferScreen,
    vmGiftCardOnly,
    vmTerminalExemptFromHappyHour,
    vmSetTextFontSize,
    vmSetCustomerDisplayFontSize,
    vmNotifyForManuallyEnteredWeight,
    vmShowScreenToSelectItemForPoint,
    vmNotifyLastWebOrder,
    vmUpdateMenu,
    vmPontsSpentCountedAsRevenue,
    vmShowDarkBackgroundInPOS,
    vmIsCloudSynced,
    vmOpenCashDrawer,
    vmHideReceiptNumberForRefundItem,
    vmMergeSimilarItem,
    vmHideRoundingOnReceipt,
	vmCashDenominationEntry,
    vmReportExportPath,
    vmIsBillSplittedByMenuType,
    vmIsAutoLoggedOut,
    vmDiningBal,
   // Happy Hour ------------------------------------------------------------------
   vmHappyHourExempt = 5000, vmHappyHourStart, vmHappyHourFinish, vmHHSunday, vmHHMonday, vmHHTuesday, vmHHWednesday, vmHHThursday,
   vmHHFridays, vmHHSaturday, vmLuckyMemberPeriod, vmHappyHourMembersOnly, vmLuckyMemberDrawByLocationOnly, vmNoPtsEarnedDuringHappyHour, vmNoOfPriceLevels,
	// Points Config ---------------------------------------------------------------
   vmEarnsPointsWhileRedeeming = 5200, vmMinMemberNumber, vmRecycleMemberNumber, vmOnlyEarnsPointsWhileRedeeming, vmSpendPtsOnlyFullProduct, vmMinRedemptionPoint,
   vmSpendPtsOnlySelectItems, vmPoleDisplayShowPoints, vmEarntPointsOnWeight,vmAllowPointPaymentByWeight,vmAllowPointPaymentByValue, vmAllowRefundPoints,
   // POS Printing ----------------------------------------------------------------
   vmReceiptPrinter = 6000, vmPrintNoteWithDiscount, vm_PLACEHOLDER_LargeKitchenFont, vmDuplicateReceipts,
   vm_PLACEHOLDER_HeaderForEachCourse, vmDuplicateEftPosReceipt, vmDuplicateSignatureReceipts, vm_PLACEHOLDER_UseChitNumbers, vm_PLACEHOLDER_ChitLabel,
   vm_PLACEHOLDER_CustomChitLabel, vmSeatLabel, vmPointsPaymentGroupNumber, vm_PLACEHOLDER_CutType, vm_PLACEHOLDER_TipOnRecipt,
   vm_PLACEHOLDER_OptionsOnRecipt, vmSignatureOnRecipt, vmSumPricingChangesOnRecipt, vmDisplayBarCodesOnReceipt,
   vm_PLACEHOLDER_DisplayChitNumberOnReceipt, vmAlwaysPrintReceiptTenderedSales, vm_PLACEHOLDER_ConsolidateReceipt,
   vm_PLACEHOLDER_AlphaNumChitNumbers, vmShowPointsOnZed, vmShowInvoiceInfoOnZed, vmUnRavelKitchenPrinting,
   vmKitchenPerSeatMultiplerString, vmKitchenBulletSide, vmKitchenHeaderSide, vmKitchenBulletOpt, vmKitchenHeaderOpt, vmKitchenBulletNote,
   vmKitchenHeaderNote, vmKitchenBulletMix, vmKitchenHeaderMix, vmSaveAndPrintPrintsPartialOrder, vm_PLACEHOLDER_HideFreeItemsOnReceipt,
   vm_PLACEHOLDER_HideZeroDollarsOnReceipt, vm_PLACEHOLDER_HideCancelledItemsOnReceipts, vm_PLACEHOLDER_HidePrevPayemntsOnReceipts,
   vmAlwaysPrintReceiptCashSales, vm_PLACEHOLDER_RavellItemsWithPriceAdjusts, vm_PLACEHOLDER_PrintByServingCourse,
   vmKitchenBulletSetMenuItem, vmAutoPrintTabReceipts, vmAutoPrintStaffReceipts, vmAutoPrintMemberReceipts, vmAutoPrintTableReceipts,
   vmAutoPrintCashReceipts, vmAutoPrintRoomReceipts, vmAutoPrintInvoiceReceipts, vm_PLACEHOLDER_SumDiscountsOnRecipt,
   vmSummariseDiscountOnZed, vm_PLACEHOLDER_HideMembershipDetialsOnReceipts, vmCreditPaymentGroupNumber, vmMembersNameOnReceipt,
   vmAutoPrintWebReceipts,vmReceiptNumberLabel,vmReprintReceiptLabel, vmShowSessionDateInZed,vmPrintZeroTaxes,vmPrintNoticeOnTransfer, vmShowPriceAdjustment, vmRevenueFiguresAreDiscountInclusive,
   vmCaptureRefundRefNo, vmHideTaxInvoice, vmExportReprintReceipt, vmSetVoidFooter,
   vmReceiptDigits, vmShowVoidOrRefund, vmShowVoidNumber,vmRefundReferenceLabel,vmSetSubHeader,vmUseBIRFormatInXZReport,
   vmAlwaysPrintReceiptDiscountSales, vmPrintSignatureWithDiscountSales, vmPrintSignatureWithRoomSales,
   // Printers   ------------------------------------------------------------------
   vmPrinterModel = 7000, vmPrinterEmulationMode, vmNormalCPL, vmNormalBoldCPL, vmCutterGap, vmTopMargin, vmKitchenTopMargin,
   vmAlwaysUseFullCut, vmKanjiPrinter, vmSingleLineChar, vmDoubleLineChar, vmGraphicPrinter, vmGraphicFontName, vmGraphicFontSize,
   vmGraphicHeaderX, vmGraphicHeaderY, vmGraphicHeaderWidth, vmGraphicHeaderHeight, vmGraphicHeaderFile, vmGraphicReceiptX,
   vmGraphicReceiptY, vmGraphicReceiptMaxLines, vmGraphicBackGroundX, vmGraphicBackGroundY, vmGraphicBackGroundWidth,
   vmGraphicBackGroundHeight, vmGraphicBackGroundFile, vmGraphicPageCountX, vmGraphicPageCountY, vmPrinterCodePage,
   vmPrinterCodePageOn,vmPrinterCodePageOff,vmPrinterInitCmd,
   // Xero Integration ------------------------------------------------------------
   vmXeroMachineName, vmXeroFolderPath, vmXeroUserName, vmXeroPassword, vmAwaitingPaymentInXero,

   // LoyaltyMate ------------------------------------------------------------
   vmLoyaltyMateEnabled, vmServiceURL, vmMemberActivationURL, vmEnableActivationTokenPrint, vmActivationTokenGreeting,vmUseTierLevels,
   vmBirthdayBuffer,vmFirstVisitPoint, vmAllowMemberDetailscreen, vmMandatoryMembershipCard,vmGiftCardValidation,
   //GlCodes
   vmPointsPurchasedGLCode,vmPointsSpentGLCode,
   vmVoucherPurchasedGLCode, vmServiceChargeGLCode, vmServiceChargeTaxGLCode,vmRoundingGLCode, vmTabDepositCreditReceivedGLCode,
   vmTabDepositCreditRefundedGLCode, vmFloatGLCode,vmEftPosTipGLCode, vmCashWithdrawal, vmCashVariance,
    //Tax
    vmItemPriceIncludeTax = 8000, vmItemPriceIncludeServiceCharge, vmCalculateTaxPostDiscount, vmCalculateServiceChargePostDiscount,
    vmUsingServiceCharge, vmApplyServiceChargeTax, vmServiceChargeTaxProfileKey, vmServiceChargeTaxRate, vmUseItalyFiscalPrinter, vmFPPrinterType, vmFPPrinterLogicalName,

   // MallExport ------------------------------------------------------------
    vmMallPath = 9000, vmMallIndex, vmTenantNo, vmMallLocation, vmClassCode, vmTradeCode, vmOutletCode, vmBranchCode,
    vmTerminalNo, vmSerialNo, vmBatchNo, vmZCount, vmFirstMallSet, vmPPlantCount, vmPPlantPrevTime, vmPPlantHourly,
    vmPPlantFlag, vmFirstMallDate, vmLastTransactionDate, vmCMFileSerialNumber, vmFTPServer, vmFTPPath, vmFTPUserName,
    vmFTPPassword, vmLastZedDate, vmLastZedTime, vmAmountValue, vmShowTransactionSummaryGroups, vmShowBilledSalesTotals,
    vmShowComplimentarySalesTotals, vmShowChargedSalesTotals, vmShowTotals, vmShowBreakdownCategories, vmShowDiscountReport,
    vmShowPointsReport, vmShowPatronAverages, vmShowProductionInfo, vmShowAccountPurchases, vmShowAccountBalancesTabs,
    vmShowAccountBalances, vmShowHourlySales, vmRevenueFiguresAreTaxAndServiceChargeInclusive,  vmDLFMallFileName,

	// BarExchange ------------------------------------------------------------
	vmBarExchangeEnabled,
	vmBarExchangePath,
	vmBarExchangeSelect,
	vmBarExchangeServerEnabled,
	vmBarExchangeServerGlobalEnabled ,

	// Story MM4161
	vmMaximumChitNumber,

	//Thorlink Loyalty 6148
    vmIsThorlinkEnabled,    // enum value = 9049
    vmThorlinkAppKey,
    vmThorlinkSiteNo,
    vmThorlinkMerchantCode,
    vmThorlinkDeviceCode,
    //////////////////

    vmShowLargeFonts,
    // MYOB Integration 10224
    vmMYOBMachineName,  // enum value = 9055
    vmMYOBFolderPath,
    vmMYOBUserName,
    vmMYOBPassword,
    vmIsXeroEnabled,
    vmIsMYOBEnabled,
    vmConsolidateReportPaths,
    vmEnableConsolidatedReport,
    vmExcludeReceipt,
    vmExcludeXReport,
    vmMembershipPaid,
    vmUseMemberSubs,
    vmFloatWithdrawFromCash,
    ///PechTree CSV
    vmCSVPath = 9500, vmIsEnabledPeachTree, vmCSVExportIP, vmCustomerId,

     //Panasonic Integration
    vmIsPanasonicIntegrationEnabled = 9600, vmPanasonicServerIP ,
    vmPMSType, vmIsFiscalStorageEnabled, vmFiscalServerPortNumber,
    vmFiscalBaudRate, vmFiscalDataBits,vmFiscalStopBits,vmFiscalParityBits,
    vmFiscalRTSFlowControl,vmFiscalOutCTSFlow,vmFiscalXonXoffOut,vmFiscalXonXoffIn,vmFiscalAsync ,
    vmOrganizationNumber, vmShowCashDrawerOpeningsCount, vmShowReprintReceiptDetails,
    vmApplyRoundingTax = 9617, vmRoundingTaxProfileKey, vmRoundingTaxRate,
    vmIsSiHotPostInProgress,vmIsOraclePostInProgress,vmIsOraclePOSServer, vmOracleInterfaceIPAddress,
    vmOracleInterfacePortNumber
};

// ---------------------------------------------------------------------------
class TManagerVariable
{
	bool fEnabled;

	void __fastcall SetEnabled(bool value);
	bool __fastcall GetEnabled();


	void SetVarStr(Database::TDBTransaction &Transaction, vmVariables VariablesKey, UnicodeString Name, UnicodeString Discription,
	vmGroup Group, UnicodeString Value);
	void SetVarInt(Database::TDBTransaction &Transaction, vmVariables VariablesKey, UnicodeString Name, UnicodeString Discription,
	vmGroup Group, int Value);
	void SetVarNum(Database::TDBTransaction &Transaction, vmVariables VariablesKey, UnicodeString Name, UnicodeString Discription,
	vmGroup Group, double Value);
	void SetVarBool(Database::TDBTransaction &Transaction, vmVariables VariablesKey, UnicodeString Name, UnicodeString Discription,
	vmGroup Group, bool Value);
	void SetVarTime(Database::TDBTransaction &Transaction, vmVariables VariablesKey, UnicodeString Name, UnicodeString Discription,
	vmGroup Group, TDateTime Value);

	bool GetVarStr(Database::TDBTransaction &Transaction, vmVariables VariablesKey, UnicodeString &Value);
	bool GetVarInt(Database::TDBTransaction &Transaction, vmVariables VariablesKey, int &Value);
	bool GetVarNum(Database::TDBTransaction &Transaction, vmVariables VariablesKey, double &Value);
	bool GetVarBool(Database::TDBTransaction &Transaction, vmVariables VariablesKey, bool &Value);
	bool GetVarTime(Database::TDBTransaction &Transaction, vmVariables VariablesKey, TDateTime &Value);

	bool GetDeviceStr(Database::TDBTransaction &Transaction, vmVariables VariablesKey, UnicodeString &Value);
	bool GetDeviceInt(Database::TDBTransaction &Transaction, vmVariables VariablesKey, int &Value);
	bool GetDeviceNum(Database::TDBTransaction &Transaction, vmVariables VariablesKey, double &Value);
	// bool GetDeviceBool(Database::TDBTransaction &Transaction,vmVariables VariablesKey,bool &Value);
	bool GetDeviceTime(Database::TDBTransaction &Transaction, vmVariables VariablesKey, TDateTime &Value);

	bool DeleteProfileVariable(Database::TDBTransaction &Transaction, int ProfileKey, vmVariables VariablesKey);

	bool GetProfileNumDB(Database::TDBTransaction &Transaction, int ProfileKey, vmVariables VariablesKey, double &Value);
	bool GetProfileStrDB(Database::TDBTransaction &Transaction, int ProfileKey, vmVariables VariablesKey, UnicodeString &Value);
	bool GetProfileTimeDB(Database::TDBTransaction &Transaction, int ProfileKey, vmVariables VariablesKey, TDateTime &Value);
	int GetProfileDB(Database::TDBTransaction &Transaction, ProfileType Type, UnicodeString ProfileName);

	// Cache Access Structures and Funtions.
	std::map <ProfileType, std::map <UnicodeString, int> > CacheGetProfile;
	bool GetProfileCache(ProfileType Type, UnicodeString ProfileName, int &Value);

	std::map <int, std::map <vmVariables, int> > CacheGetProfileInt;
	bool GetProfileIntCache(int ProfileKey, vmVariables VariablesKey, int &Value);

	std::map <int, std::map <vmVariables, double> > CacheGetProfileNum;
	bool GetProfileNumCache(int ProfileKey, vmVariables VariablesKey, double &Value);

	std::map <int, std::map <vmVariables, UnicodeString> > CacheGetProfileStr;
	bool GetProfileStrCache(int ProfileKey, vmVariables VariablesKey, UnicodeString &Value);

	std::map <int, std::map <vmVariables, TDateTime> > CacheGetProfileTime;
	bool GetProfileTimeCache(int ProfileKey, vmVariables VariablesKey, TDateTime &Value);

public:
	__property bool Enabled =
	{
		read = GetEnabled, write = SetEnabled
	};


	static TManagerVariable& Instance() {
		static TManagerVariable singleton;
		return singleton;
	}

	int LocationProfileKey;
	int DeviceProfileKey;

	bool Delete(Database::TDBTransaction &Transaction, vmVariables VariablesKey);

	UnicodeString GetStr(Database::TDBTransaction &Transaction, vmVariables VariablesKey, UnicodeString Default = "");
	int GetInt(Database::TDBTransaction &Transaction, vmVariables VariablesKey, int Default = 0);
	double GetNum(Database::TDBTransaction &Transaction, vmVariables VariablesKey, double Default = 0);
	bool GetBool(Database::TDBTransaction &Transaction, vmVariables VariablesKey, bool Default = false);
	TDateTime GetTime(Database::TDBTransaction &Transaction, vmVariables VariablesKey, TDateTime Default = 0);

	void SetStr(Database::TDBTransaction &Transaction,vmVariables VariablesKey,UnicodeString Value);
	void SetDeviceStr(Database::TDBTransaction &Transaction, vmVariables VariablesKey, UnicodeString Value);
	void SetDeviceInt(Database::TDBTransaction &Transaction, vmVariables VariablesKey, int Value);
	void SetDeviceNum(Database::TDBTransaction &Transaction, vmVariables VariablesKey, double Value);
	void SetDeviceBool(Database::TDBTransaction &Transaction, vmVariables VariablesKey, bool Value);
	void SetDeviceTime(Database::TDBTransaction &Transaction, vmVariables VariablesKey, TDateTime Value);

	int GetProfile(Database::TDBTransaction &Transaction, ProfileType Type, UnicodeString ProfileName);
	int SetProfile(Database::TDBTransaction &Transaction, ProfileType Type, UnicodeString ProfileName);
	bool SetProfile(Database::TDBTransaction &Transaction, int ProfileKey, ProfileType Type, UnicodeString ProfileName);
	bool DelProfile(Database::TDBTransaction &Transaction, ProfileType Type, UnicodeString ProfileName);
	bool DelProfile(Database::TDBTransaction &Transaction, int ProfileKey);

	void SetProfileStr(Database::TDBTransaction &Transaction, int ProfileKey, vmVariables VariablesKey, UnicodeString Value);
	void SetProfileInt(Database::TDBTransaction &Transaction, int ProfileKey, vmVariables VariablesKey, int Value);
	void SetProfileInt(Database::TDBTransaction &Transaction, int ProfileKey, vmVariables VariablesKey, unsigned char Value);
	void SetProfileInt(Database::TDBTransaction &Transaction, int ProfileKey, vmVariables VariablesKey, char Value);
	void SetProfileInt(Database::TDBTransaction &Transaction, vmVariables VariablesKey, int Value);
	void SetProfileNum(Database::TDBTransaction &Transaction, int ProfileKey, vmVariables VariablesKey, double Value);
	void SetProfileBool(Database::TDBTransaction &Transaction, int ProfileKey, vmVariables VariablesKey, bool Value);
	void SetProfileTime(Database::TDBTransaction &Transaction, int ProfileKey, vmVariables VariablesKey, TDateTime Value);

	bool GetProfileIntDB(Database::TDBTransaction &Transaction, int ProfileKey, vmVariables VariablesKey, int &Value);

	bool GetProfileStr(Database::TDBTransaction &Transaction, int ProfileKey, vmVariables VariablesKey, UnicodeString &Value);
	bool GetProfileInt(Database::TDBTransaction &Transaction, int ProfileKey, vmVariables VariablesKey, int &Value);

	bool GetProfileInt(Database::TDBTransaction &Transaction, int ProfileKey, vmVariables VariablesKey, unsigned char &Value);
	bool GetProfileInt(Database::TDBTransaction &Transaction, int ProfileKey, vmVariables VariablesKey, char &Value);
	bool GetProfileNum(Database::TDBTransaction &Transaction, int ProfileKey, vmVariables VariablesKey, double &Value);
	bool GetProfileBool(Database::TDBTransaction &Transaction, int ProfileKey, vmVariables VariablesKey, bool &Value);
	bool GetProfileTime(Database::TDBTransaction &Transaction, int ProfileKey, vmVariables VariablesKey, TDateTime &Value);

	TManagerVariable();
	void InitialisePOSVars(Database::TDBTransaction &DBTransaction);
};

#endif


