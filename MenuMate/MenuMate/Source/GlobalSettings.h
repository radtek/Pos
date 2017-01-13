// ---------------------------------------------------------------------------

#ifndef GlobalSettingsH
#define GlobalSettingsH

#include "PointsRulesSet.h"
#include "Mall.h"
#include <memory>
#include <Classes.hpp>
#include <Graphics.hpp>
// ---------------------------------------------------------------------------
enum eDayTimes {
	eSunday = 0, eMonday, eTuesday, eWednesday, eThursday, eFriday, eSaturday,
	eEndOfDays = 7
};

enum eMembershipType {
	MembershipTypeMenuMate, MembershipTypeERS, MembershipTypeEBet,
	MembershipTypeExternal , MembershipTypeThor
};

enum eSetFontSize {eSeven , eEight, eNine, eTenth, eEleven };

class TGlobalSettings {
private:
	TGlobalSettings();

public:

	static TGlobalSettings& Instance() {
		static TGlobalSettings singleton;
		return singleton;
	}

	std::auto_ptr<TStringList>Header;
	std::auto_ptr<TStringList>PHeader;
	std::auto_ptr<TStringList>Footer;

	unsigned char InfoSync;
	bool HappyHourDays[eEndOfDays];
	bool HappyHourExempt;
	bool HappyHourMembersOnly;
	bool InHappyHour;
	bool ForceHappyHour;
	bool AlwaysForceLogin;
	bool RevertToFirstCourse;
	bool ForcedReasons;
	bool ForcedSidesFirst;
	bool PrintSignatureReceiptsTwice;
	bool TablesEnabled;
	bool TabsEnabled;
	bool DuplicateEftPosReceipt;
	bool DuplicateReceipts;
	bool EnableEftPosSyncro;
	bool EnableEftPosANZ;
	bool EnableEftPosIngenico;
	bool EnableEftPosCadmus;
	bool EnableEftPosCadmusCronos;
	bool EnableEftPosIceLink;
	bool EnableEftPosDPS;
	bool EnableDPSTipping;
	bool ShowPointsOnZed;
	bool ShowInvoiceInfoOnZed;
	bool SaveAndPrintPrintsPartialOrder;
	bool HideServingCourseLabels;
	bool RememberLastServingCourse;
	bool PromptForPatronCount;
	bool DefaultToBevMenu;
	bool SummariseDiscountOnZed;
	bool DisableInternalInvoicing;
	bool DisableConfirmationOnSave;
	bool DisableReceiptOnConfirmation;
	bool ShowVoucherDetails;
	bool EnablePhoneOrders;
	bool EnableBirthdayNotifications;
	bool AcknowledgeFirstVisit;
	bool EnableMemberSpendChits;
	Currency MemberSpendChitsAmount;
	bool CaptureCustomerName;
	bool HideFreeItemsAtBilling;
	bool EnableWaiterStation;

	//warn user when there is open tables and open tabs
	bool WarnIfOpenTableAndTabs;
	//stop user when there is open tables and open tabs
	bool StopIfOpenTableAndTabs;

	//story MM1837
	bool ShowTransactionSummaryGroups;
	bool ShowBilledSalesTotals;
	bool ShowComplimentarySalesTotals;
	bool ShowChargedSalesTotals;
	bool ShowTotals;
	bool ShowBreakdownCategories;
	bool ShowDiscountReport;
	bool ShowPointsReport;
	bool ShowPatronAverages;
	bool ShowProductionInfo;
	bool ShowAccountPurchases;
	bool ShowAccountBalancesTabs;
	bool ShowAccountBalances;
	bool ShowHourlySales;
	bool RevenueFiguresAreTaxAndServiceChargeInclusive;


	bool AutoPrintTabReceipts;
	bool AutoPrintStaffReceipts;
	bool AutoPrintMemberReceipts;
	bool AutoPrintTableReceipts;
	bool AutoPrintCashReceipts;
	bool AutoPrintRoomReceipts;
	bool AutoPrintInvoiceReceipts;
	bool AutoPrintWebReceipts;
	bool EnableBlindBalances;
	bool EnableStaffHours;
	bool EnableCommission;
	bool EnablePrinterCounts;
	bool EnablePaxCount;
	bool EnableHideCredsCancels;
	bool EnableDepositBagNum;
	bool EnableDontClearZedData;
	bool EnableHoldSend;
	bool EnableMenuPatronCount;

	bool EnableSeperateEarntPts;

	bool EnableDisplayTax;
	bool EnableDisplayServiceCharge;
	bool IncludeServiceChargeTaxInTaxValue;
	bool EnableNmiDisplay;
	bool SpendPtsOnlyFullProduct;
	bool SpendPtsOnlySelectItems;
	bool EarntPointsOnWeight;

	bool ShowPointsOnDisplay;

	bool NoPtsEarnedDuringHappyHour;

	bool MidPointRoundsDown;

	bool AutoConvertScalesWeights;
	bool AllowToEnterPrice;
	bool AllowToEnterWeight;
	bool EnableAARewards;
	bool EnableAARewardsReminder;
	bool UnRavelKitchenPrinting;
	bool AutoLogoutPOS;
	bool LuckyDrawByLocationOnly;

	bool AutoAddWebMembers;
	bool AutoAcceptWebOrders;

	int SiteID;
	int AutoLogoutSeconds;
	int NonServicePeriodMinutes;
	int KitchenOrderNumber;
	int ClearLogsEveryXDays;
	int LuckyMemberPeriod;
	int PointsPaymentGroupNumber;
	int CreditPaymentGroupNumber;

	bool WebMateEnabled;
	int WebMatePort;

	TPointsRulesSet SystemRules;

	TDateTime HappyHourStart;
	TDateTime HappyHourFinish;

	UnicodeString AutoMateIP;
	UnicodeString InterbaseIP;
	UnicodeString SntpIP;
	UnicodeString CurrentIPAddress;
	UnicodeString StockInterbaseIP;
	UnicodeString StockDatabasePath;
	UnicodeString SeatLabel;

	eMembershipType MembershipType;
	UnicodeString MembershipDatabaseIP;
	UnicodeString MembershipDatabasePath;
	int MembershipDatabasePort;

	int KitchenColour;
	UnicodeString DatabasePath;
	UnicodeString ReceiptNumberLabel;
    UnicodeString RefundReferenceLabel;
	UnicodeString ReprintReceiptLabel;
	UnicodeString PoleDisplayTopLine;
	UnicodeString PoleDisplayBottomLine;

	UnicodeString IntaMateIPAddress;
	int IntaMatePort;
	int IntaMateTCPTimeOut_ms;
	int IntaMateTerminalID;

	//Variables for Tax Maintance purpose
	bool ItemPriceIncludeTax;
	bool ItemPriceIncludeServiceCharge;
	bool ReCalculateTaxPostDiscount;
	bool ReCalculateServiceChargePostDiscount;
	bool UsingServiceCharge;
	bool ApplyServiceChargeTax;
	int ServiceChargeTaxProfileKey;
	double ServiceChargeTaxRate;

	//For Mall Export
	int ZCount;
	int BatchNo;
	int MallIndex;
	int PPlantCount;
	int PPlantPrevTime;
	int PPlantHourly;
	int PPlantFlag;
	int CMFileSerialNumber;
	bool FirstMallSet;
    bool EnableConsolidatedReport;
	UnicodeString FirstMallDate;
	UnicodeString ClassCode;
	UnicodeString OutletCode;
	UnicodeString MallPath;
	UnicodeString TenantNo;
	UnicodeString TradeCode;
	UnicodeString BranchCode;
	UnicodeString TerminalNo;
	UnicodeString SerialNo;
	UnicodeString LastTransactionDate;
	UnicodeString LastZedDate;
	UnicodeString LastZedTime;
	UnicodeString FTPServer;
	UnicodeString FTPPath;
	UnicodeString FTPUserName;
	UnicodeString FTPPassword;
	UnicodeString AmountValue;
    UnicodeString DLFMallFileName;
    UnicodeString ConsolidateReportPaths;

	/* Unsigned so to use the most significant bit as a flag specifying
	* whether the default font size has been overriden by the User.
	*/
	unsigned int posButtonFontSize;

	int MembershipTillID;

	int MaxGamingID;

	int DefaultColumns;
	int DefaultRows;
	bool ReservationsEnabled;
	UnicodeString ReservationsAddress;
	bool FloorPlanTransparentTables;
	int FloorPlanTableColour;
	int FloorPlanTextColour;
	int LastSelectedFloorPlanLocationID;

	int RoundingTime;

	bool PrintStockWeborders;

	//story MM1837 remove
	/*
	bool SalesforceUploadEnabled;
	bool SalesforceUploadStock;
	UnicodeString SalesforceUsername;
	UnicodeString SalesforcePassword;
	UnicodeString SalesforceSecurityToken;
	*/

	bool QMSIsEnabled;
	bool QMSDateOfBirthIsMandatory;
	bool QMSEmailAddressIsMandatory;
	bool QMSPhoneNumberIsMandatory;

	UnicodeString XeroMachineName;
	UnicodeString XeroUserName;
	UnicodeString XeroPassword;
	UnicodeString XeroFolderPath;

  	UnicodeString MYOBMachineName;
	UnicodeString MYOBUserName;
	UnicodeString MYOBPassword;
	UnicodeString MYOBFolderPath;

	bool NagUserToSelectChit;

	/* LoyaltyMate settings */
	bool LoyaltyMateEnabled;
	UnicodeString ServiceURL;
	UnicodeString MemberActivationURL;
	bool EnableActivationTokenPrint;
	UnicodeString ActivationTokenGreeting;

	// Accumulated Zed
	bool ShowAccumulatedZeds;

	// Customer Display
	bool ShowCustomerDisplay;

	// Eparked Sales
	bool EnableParkedSales;

	// SessionDate
	short EndOfDay;
	bool ShowSessionDateInZed;
	bool ExtendCurrencyDisplay;
	bool EnableDisplayTaxRemoval;
	// Tax Summary
	bool ShowTaxSummary;
	// Service Charge Summary
	bool ShowServiceChargeSummary;
	bool ShowServiceChargeTaxWithServiceCharge;
	bool ShowServiceChargeTaxWithSalesTax;
	// Enable Client Details
	bool EnableClientDetails;
	// Zed Header
	std::auto_ptr<TStringList> ZedHeader;
    bool PrintZeroTaxes;
    // Receipt Docket Settings
    //bool HideSalesTaxWhenValueIsZero;
    int BarcodeFormat;
    bool EnableTableDisplayMode;
    bool UseTierLevels;
    int BirthdayBuffer;
    int FirstVisitPoint;
    int MinRedemptionPoint;
    bool AllowMemberDetailscreen;
    bool MandatoryMembershipCard;

    bool IsPOSOffline;
    int TierLevelChange;
    //-----------------------------------------------------------------------
    //MM2038
    UnicodeString TabPrintName;
    UnicodeString TabPrintPhone;
    bool AllowPointPaymentByWeight;
    bool AllowPointPaymentByValue;
    bool PrintNoticeOnTransfer;
	bool UpdateTableGUIOnOrderStatus;
    bool AllowRefundPoints;   //mm-3395


	/* BarExchange settings */
	bool BarExchangeEnabled;
	UnicodeString BarExchangePath;
    UnicodeString RunRateBoardPath;
	UnicodeString BarExchangeSelect;
	bool BarExchangeServerEnabled;
	bool BarExchangeServerGlobalEnabled;

    bool ShowCurrencySymbol;
    bool EmailZedReports;
    UnicodeString SaveEmailId;

    UnicodeString EmailId;
    UnicodeString EmailPassword;
    UnicodeString EmailHost;
    int EmailHostPort;

	// MM4161
	int MaximumChitNumber;
	bool SkipConfirmationOnFastTender;
	bool TransferTableOnPrintPrelim;

    //Drink Command Settings..
    bool IsDrinkCommandEnabled;
    UnicodeString DrinkCommandServerPath;
    int DrinkCommandServerPort;

    //MM4579
    bool PointPurchased;
    bool PointEarned;
    bool PointRedeem;
    int ResetDay;

    bool ShowRemovalReport;

    bool EnableCancelCheckRemoval;
    bool RevenueFiguresAreDiscountInclusive;
    bool ShowPriceAdjustment;
    bool EnableApprovedLogin;
    bool ClockInOutApprovedLogin;
    bool RoundOnBilling;
    double RoundOnBillingAmount;

    //MM-4775
    bool PointOnly;
    bool PromptPatronCountOnTableSales; //MM-5085
    bool PostZToAccountingSystem;

    int NewBook;
    //MM 5610
    bool ConfirmResetPoint;
    bool EnableEftPosSmartPay;
    UnicodeString EftPosSmartPayIp;
    UnicodeString PointsPurchasedGLCode;
    UnicodeString PointsSpentGLCode;
    UnicodeString VoucherPurchasedGLCode;
    UnicodeString ServiceChargeGLCode;
    UnicodeString ServiceChargeTaxGLCode;
    UnicodeString RoundingGLCode;
    bool RefundingItems;
    bool GiftCardOnly;

    bool IsClippIntegrationEnabled;
    bool IsMemberSalesOnlyEnabled;
    int ClippTabStartIndex;
    int ClippTabEndIndex;

    bool DeleteItemSizeAfterSale;

    bool ShowItemSizesAsList;
    bool ShowItemPriceInTransferScreen;
    int NoOfPriceLevels;
    bool TerminalExemptFromHappyHour;
    bool NotifyForManuallyEnteredWeight;


    bool IsThorVoucherSelected;
    // Thorlink 6148
    bool IsThorlinkEnabled;
    bool IsThorlinkSelected;
    bool IsDiscountSelected;
    bool IsProcessThorVoucher;
    bool IsThorlinkTender;
    bool IsThorPay;
    bool IsThorBillSelected;
    bool IsXeroEnabled;
    bool IsMYOBEnabled;
    UnicodeString ThorlinkAppKey;
    UnicodeString ThorlinkSiteNo;
    UnicodeString ThorlinkMerchantCode;
    UnicodeString ThorlinkDeviceCode;
    bool SetTextFontSizeOnCustomerDisplay;
    UnicodeString CustomerDisplayFontSize;
    eSetFontSize SetFontSize;
    Currency WeightLimit;

    bool AwaitingPaymentInXero;
    bool PostMoneyToGlAccounts;
    bool PostMoneyAsPayment;
    UnicodeString TabDepositCreditReceivedGLCode;
    UnicodeString TabDepositCreditRefundedGLCode;
    UnicodeString FloatGLCode;
    bool ShowScreenToSelectItemForPoint;
    bool NotifyPOSForLastWebOrder;
    bool UpdateMenu;
    bool PontsSpentCountedAsRevenue;
    bool IsRunRateBoardEnabled;
    UnicodeString RunRateBoardIP;
    bool ShowLargeFonts;
    bool ItemSearch;
    bool IsCloudSyncedForDiscount;
    bool ShowDarkBackground;
    bool OpenCashDrawer;
    bool HideReceiptNumberForRefundItem;
    bool MergeSimilarItem;

    bool HideTaxInvoice;
    bool ExportReprintReceipt;
    bool CaptureRefundRefNo;
    bool SetVoidFooter;
    bool SetSubHeader;
    UnicodeString ReceiptDigits;
    std::auto_ptr<TStringList>VoidFooter;
    std::auto_ptr<TStringList>SubHeader;
    bool ShowVoidOrRefund;
    bool ShowVoidNumber;
    UnicodeString EftPosTipGLCode;
    int GiftCardValidation;
    bool ExcludeReceipt;
    bool ExcludeXReport;
    bool UseBIRFormatInXZReport;
    bool HideRoundingOnReceipt;
	bool CashDenominationEntry;
    UnicodeString CSVPath;
    bool IsEnabledPeachTree;
    UnicodeString CSVExportIP;
    bool MembershipPaid;
    bool UseMemberSubs;
    TMall mallInfo;
    bool FloatWithdrawFromCash;
    bool IsPanasonicIntegrationEnabled;
    UnicodeString PanasonicServerIP;
};

#endif

