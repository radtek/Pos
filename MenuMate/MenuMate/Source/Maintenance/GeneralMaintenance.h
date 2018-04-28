//---------------------------------------------------------------------------

#ifndef GeneralMaintenanceH
#define GeneralMaintenanceH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <CheckLst.hpp>
#include "MM_DBCore.h"
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"
#include "TouchGrid.h"
#include <Dialogs.hpp>
#include "TouchPages.h"

//---------------------------------------------------------------------------



class TfrmGeneralMaintenance : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
        TPanel *pnlButtons;
        TPageControl *Pages;
        TTabSheet *tsPointOfSale;
        TTabSheet *tsStaffOptions;
        TTabSheet *tsMembershipOptions;
        TTabSheet *tsEftPos;
        TPanel *Panel1;
        TPanel *Panel4;
        TPanel *Panel5;
        TPanel *Panel6;
        TGroupBox *GroupBox10;
        TLabel *Label4;
        TCheckBox *cbRevertCourse;
        TCheckBox *cbEnableTableMode;
        TCheckBox *cbForceTender;
        TCheckBox *cbFTEFTSales;
        TTouchBtn *btnNonService;
        TCheckBox *cbForcedReasons;
        TGroupBox *GroupBox11;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label7;
        TCheckBox *chbUseAutoLogout;
        TTouchBtn *btnAutoLogoutTime;
        TCheckBox *cbForcedSidesFirst;
        TGroupBox *GroupBox8;
        TCheckBox *cbStaffDisableManualLogin;
        TGroupBox *GroupBox1;
        TCheckBox *cbIncrementalSearch;
        TGroupBox *GroupBox7;
        TLabel *Label3;
        TTouchBtn *tbtnPointsGroup;
        TCheckBox *cbPointsOnZed;
        TCheckBox *cbShowInvoiceInfo;
        TCheckBox *cbEarnPoints;
        TGroupBox *GroupBox9;
        TCheckBox *cbIntegratedEftposANZ;
        TCheckBox *cbIntegratedEftposSyncro;
        TCheckBox *cbIntegratedEftposIngenico;
        TCheckBox *cbIntegratedEftposSmartPay;
        TGroupBox *GroupBox5;
        TCheckBox *cbSaleAndMakeTimes;
        TGroupBox *GroupBox6;
        TLabel *lbeSeatLabel;
        TEdit *edSeatLabel;
        TCheckBox *cbTableSeats;
        TCheckBox *cbTabsInterface;
        TTabSheet *tsLogging;
        TPanel *Panel7;
        TGroupBox *GroupBox4;
        TLabel *Label11;
        TEdit *edClearLogsXDays;
        TUpDown *udClearLogsXDays;
        TLabel *Label16;
        TPanel *Panel2;
        TPanel *Panel9;
        TPanel *Panel10;
        TPanel *Panel11;
        TPanel *Panel12;
        TGroupBox *gbMemberPoleDisplay;
        TRadioButton *rbNoGreeting;
        TRadioButton *rbShowAlias;
        TRadioButton *rbShowName;
        TMemo *MemLogs;
        TCheckBox *cbOnLineLogging;
        TRadioGroup *rgMemberFreeSalesRules;
        TRadioGroup *rgLocationFreeSalesRules;
        TLabel *Label1;
        TTouchBtn *tbMemberQuickButtons;
        TGroupBox *GroupBox12;
        TGroupBox *GroupBox13;
        TCheckBox *cbHideServingCourseLabels;
        TCheckBox *cbIntegratedEftposCadmus;
        TComboBox *cbEftposCadmusSerialPort;
        TLabel *Label20;
        TTouchBtn *tbCadmusMerchantNumber;
        TLabel *Label8;
        TLabel *Label9;
        TCheckBox *cbDuplicateEftPos;
        TCheckBox *cbPromptPatronCount;
        TGroupBox *GroupBox14;
        TLabel *Label10;
        TTouchBtn *tbtnLuckMemberPeriod;
        TTouchBtn *tbtnLuckMemberPeriodDay;
        TTouchBtn *tbtnLuckMemberPeriodWeek;
        TTouchBtn *tbtnLuckMemberPeriodMonth;
        TTouchBtn *TouchBtn1;
        TTouchBtn *TouchBtn2;
        TTouchBtn *TouchBtn3;
        TTouchBtn *TouchBtn4;
        TTouchBtn *TouchBtn5;
        TTouchBtn *TouchBtn6;
        TTouchBtn *TouchBtn7;
        TCheckBox *cbICELink;
        TCheckBox *cbRememberLastServingCourse;
        TCheckBox *cbDefaultToBevMenu;
        TCheckBox *cbRounding;
        TGroupBox *GroupBox2;
        TTouchBtn *tbtnSyncroniseMembersip;
        TLabel *Label12;
        TGroupBox *GroupBox15;
        TLabel *Label14;
        TTouchBtn *tbMinMembershipNumber;
        TCheckBox *cbRecycleMembershipNumbers;
        TTouchBtn *tbSmartCard;
        TCheckBox *cbOnlyEarnPoints;
        TCheckBox *cbIntegratedEftposDPS;
        TCheckBox *cbEnableDPSTipping;
        TGroupBox *GroupBox16;
        TCheckBox *cbEnableMembershipSmartCards;
        TLabel *lbeEntireSiteID;
        TTouchBtn *tbtnMMSubNet;
        TLabel *Label13;
        TGroupBox *GroupBox17;
        TMemo *memStaffMessage;
        TGroupBox *GroupBox18;
        TTouchBtn *tbtnAddDept;
        TTouchGrid *tbgTimeCodes;
        TTouchBtn *tbPOS;
        TTouchBtn *tbtnDelDept;
        TTouchBtn *tbtnAddDefaultDept;
        TTouchBtn *tbtnStaffOptions;
        TTouchBtn *tbtnMembershipOpts;
        TTouchBtn *tbtnEftposOpts;
        TTouchBtn *tbtnLoggingOpts;
        TTouchBtn *TouchBtn17;
        TTouchBtn *tbtnDeptDown;
        TTouchBtn *tbtnDeptUp;
        TTouchBtn *tbtnDeptEdit;
        TLabel *Label15;
        TTouchBtn *tbtnTabCreditGroup;
        TTouchBtn *tbMinRedemptionPoint;
        TCheckBox *cbvmAllowSmartCardNotPresentMemberTransactions;
        TCheckBox * cbvmAllowMemberDetailscreen  ;
		TCheckBox * cbvmMandatoryMembershipCard  ;
        TCheckBox *cbvmEnableQuickMemberCreation;
        TCheckBox *cbDepositBagNum;
        TCheckBox *cbDontClearDataOnZed;
        TGroupBox *GroupBox19;
        TCheckBox *cbSacleAutoConvertWeights;
        TRadioGroup *rgMemberNameOnRecipt;
        TRadioGroup *rgRoundingTimes;
        TCheckBox *cbCadmusCronos;
        TGroupBox *GroupBox21;
        TCheckBox *cbDisableInternalInvoicing;
        TGroupBox *GroupBox20;
        TGroupBox *GroupBox30;
        TCheckBox *cbEnableAARewards;
        TCheckBox *cbAARewardsReminder;
        TTouchBtn *TouchBtn19;
        TCheckBox *cbSummariseDiscountsonZed;
        TCheckBox *cbDisableSaveConfirmation;
        TCheckBox *cbDisableReceiptOnConfirmation;
        TTouchBtn *TouchBtn20;
        TGroupBox *GroupBox3;
        TCheckBox *cbNotifyBirthday;
        TCheckBox *cbAcknowledgeFirstVisit;
        TTouchBtn *tbRestLastVisit;
        TCheckBox *cbEnableMemberSpendChits;
        TTouchBtn *tbtnMemberSpendChitValue;
        TCheckBox *cbDrawByLocation;
        TCheckBox *cbAutoAddWebMembers;
        TCheckBox *cbShowVoucherDetails;
        TCheckBox *cbEnablePhoneOrders;
        TRadioGroup *rgPOSBtnSize;
        TTouchBtn *btnSetPOSFontSize;
        TTouchBtn *tbEndOfDay;
        TTabSheet *tsEndOfDayOptions;
        TPanel *Panel3;
        TPanel *Panel8;
        TCheckBox *cbDEnableBlindBalances;
        TCheckBox *cbEnableStaffHours;
        TCheckBox *cbEnableCommission;
        TCheckBox *cbEnablePrinterCounts;
        TCheckBox *cbEnablePaxCount;
        TCheckBox *cbDisplayStockPurchaseWeborders;
        TCheckBox *cbEnableHideCredCancels;
        TCheckBox *cbCaptureCustomerName;
        TCheckBox *cbSeparatePts;
        TCheckBox *cbHoldSend;
        TCheckBox *cbOnlyFullProducts;
        TCheckBox *cbRedeemOnSelectedItems;
        TCheckBox *cbEarnPointsOnWeight;
        TCheckBox *cbHideFreeItemsAtBilling;
        TCheckBox *cbEnableWaiterStation;
        TCheckBox *cbPoleDisplayShowPoints;
        TCheckBox *cbNagUserToSelectChit;
        TCheckBox *cbUseMenuDefaultPatronCount;
        TCheckBox *cbEnableExtendedDisplay;
        TCheckBox *cbDisplayServiceCharge;
        TCheckBox *cbEnableNmiDisplay;
        TCheckBox *cbDisplayTaxExempt;
        TCheckBox *cbDisplayTax;
        TCheckBox *cbShowSessionDate;
        TCheckBox *cbScaleEnterWeight;
        TCheckBox *cbScaleEnterPrice;
        TComboBox *cbEndOfDay;
        TCheckBox *cbInclServiceChargeTaxInTaxVal;
        TCheckBox *cbEnableParkedSales;
        TCheckBox *cbShowAccumulatedTotal;
        TCheckBox *cbShowTaxSummary;
        TCheckBox *cbShowServiceChargeSummary;
        TCheckBox *cbCombineServiceChargeAndServiceChargeTax;
        TCheckBox *cbCombineServiceChargeTaxAndSalesTax;
        TCheckBox *cbEnableClientDetails;
        TTouchPages *tpZedInfo;
        TTouchSheet *tszedHeader;
        TRichEdit *tbzedHeaderText;
        TCheckBox *cbWarnIfOpenTableAndTabs;
        TCheckBox *cbStopIfOpenTableAndTabs;
        TCheckBox *cbShowTransactionSummaryGroups;
        TCheckBox *cbShowBilledSalesTotals;
        TCheckBox *cbShowComplimentarySalesTotals;
        TCheckBox *cbShowChargedSalesTotals;
        TCheckBox *cbShowTotals;
        TCheckBox *cbShowBreakdownCategories;
        TCheckBox *cbShowDiscountReport;
        TCheckBox *cbShowPointsReport;
        TCheckBox *cbShowPatronAverages;
        TCheckBox *cbShowProductionInfo;
        TCheckBox *cbShowAccountPurchases;
        TCheckBox *cbShowAccountBalancesTabs;
        TCheckBox *cbShowAccountBalances;
        TCheckBox *cbShowHourlySales;
        TCheckBox *cbRevenueFiguresAreTaxAndServiceChargeInclusive;
        TCheckBox *cbCountPointSpentInRevenue;
        //add by frn
        TTabSheet *tsPointOfSaleDisplay;
        TPanel *Panel14;
        TPanel *Panel15;
        TTouchBtn *tbCustomerDisplay;
        TTouchBtn *tbPOSDisplay;
        TTabSheet *tsCustomerDisplay;
        TPanel *Panel16;
        TPanel *Panel17;
        TGroupBox *grpTierLevel;
        TCheckBox *cbEnableTierLevels;
        TLabel *lbeBirthdayBuffer;
        TTouchBtn *tbBirthdayBuffer;
        TTouchBtn *tbFirstVisitPoint;
        TLabel *lbeFirstVisitPoint;
        TCheckBox *cbAllowPointPaymentByWeight;
        TCheckBox *cbAllowPointPaymentByValue;
        TCheckBox *cbUpdateTableGUIOnOrderStatus;
        TCheckBox *cbRefundPoints;    //MM-3395
        TCheckBox *cbShowCurrencySymbol;
        TCheckBox *cbSkipConfirmationOnFastTender;
        TCheckBox *cbTransferTableOnPrintPrelim;
        TCheckBox *cbEmailZedClosingTill;
        TTouchBtn *tbtnConfigureEmail;
        TCheckBox *cbRoundOnBilling;
        TTouchBtn *tbRoundingOnBilling;
        TCheckBox *cbShowRemovalReport;
        TCheckBox *cbEnableCancelCheckRemoval;
        TCheckBox *cbRevenueFiguresAreDiscountInclusive;
        TCheckBox *cbShowPriceAdjustment;
        TCheckBox *cbResetPointBalancedPointRedeemed;
        TCheckBox *cbResetPointBalancedPointEarned;
        TCheckBox *cbResetPointBalancedPointPurchased;
        TEdit *edDay;
        TCheckBox *cbConfirmResetPoint;
        TCheckBox *cbPointOnly;
        TCheckBox *cbPromptPatronCountOnTableSales; //MM-5085
        TGroupBox *smartLinkGroupBox;
        TTouchBtn *tbtnSmartLinkIp;
        TCheckBox *cbPostToXero;
        TCheckBox *cbGiftCardOnly;
        TCheckBox *cbDisplayItemSizesAsList;
        TCheckBox *cbShowItemPricesInTransferScreen;
        TCheckBox *cbPostMoneyToGLAccounts;
        TCheckBox *cbPostMoneyAsPayments;
        TCheckBox *cbTerminalExemptFromHappyHour;
        TTouchBtn *tbEnterWeight;
        TCheckBox *cbNotifyForManuallyEnteredWeight;
        TCheckBox *cbShowScreenToSelectItems;
        TGroupBox *groupBoxStaffSettings;
        TCheckBox *cbEnableApprovedLogin;
        TCheckBox *cbLinkClockInOutApprovedLogin;
        TGroupBox *GroupBox22;
        TCheckBox *cbDeleteItemSizeAfterSale;
        TGroupBox *nMIStandardGroupBox;
        TCheckBox *cbUseNMIStandardFontSize;
        TCheckBox *cbShowLargeFonts;
        TGroupBox *GroupBox23;
        TGroupBox *GroupBoxEJournal;
        TCheckBox *cbItemSearch;
        TCheckBox *cbShowDarkBackgroundInPOS;
        TCheckBox *cbOpenCashDrawer;
        TGroupBox *gbItemBilling;
        TCheckBox *cbMergeSimilarItem;
        //TCheckBox *cbShowLocalTaxandProfitTax;
        TCheckBox *cbUseBIRFormatInXZReport;
        TCheckBox *cbExcludeReceipt;
        TCheckBox *cbExcludeXReport;
        TCheckBox *cbHideRoundingOnReceipt;
		TCheckBox *cbCashDenominationEntry;
        TGroupBox *grpMemberSubs;
        TCheckBox *cbUseMemberSubs;
        TCheckBox *cbFloatWithdrawFromCash;
        TGroupBox *GroupBox25;
        TGroupBox *GroupBox26;
        TLabel *lblEndOfDay;
        TCheckBox *cbSplitBillByMenuType;
        TCheckBox *cbShowCashDrawerCount;
        TCheckBox *cbShowReprintDetails;

        TCheckBox *cbIntegratedEftposSmartConnect;
        //**********
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall btnAutoLogoutTimeClick(TObject *Sender);
        void __fastcall lbeUseAutoLogoutClick(TObject *Sender);
        void __fastcall chbUseAutoLogoutClick(TObject *Sender);
        void __fastcall cbRevertCourseClick(TObject *Sender);
        void __fastcall edClearLogsXDaysClick(TObject *Sender);
        void __fastcall btnOkClick(TObject *Sender);
        void __fastcall btnCancelClick(TObject *Sender);
        void __fastcall cbForceTenderClick(TObject *Sender);
        void __fastcall cbFTEFTSalesClick(TObject *Sender);
        void __fastcall btnNonServiceClick(TObject *Sender);
        void __fastcall cbForcedReasonsClick(TObject *Sender);
        void __fastcall edSeatLabelMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall cbEnableTableModeClick(TObject *Sender);
        void __fastcall cbSaleAndMakeTimesClick(TObject *Sender);
        void __fastcall cbTableSeatsClick(TObject *Sender);
        void __fastcall cbTabsInterfaceClick(TObject *Sender);
        void __fastcall cbForcedSidesFirstClick(TObject *Sender);
        void __fastcall tbtnPointsGroupClick(TObject *Sender);
        void __fastcall cbDuplicateEftPosClick(TObject *Sender);
        void __fastcall cbIntegratedEftposANZClick(TObject *Sender);
        void __fastcall cbIntegratedEftposSyncroClick(TObject *Sender);
        void __fastcall cbPointsOnZedClick(TObject *Sender);
        void __fastcall cbShowInvoiceInfoClick(TObject *Sender);
        void __fastcall cbStaffDisableManualLoginClick(TObject *Sender);
        void __fastcall cbIntegratedEftposIngenicoClick(TObject *Sender);
        void __fastcall cbIntegratedEftposSmartPayClick(TObject *Sender);
        void __fastcall cbEarnPointsClick(TObject *Sender);
        void __fastcall cbIncrementalSearchClick(TObject *Sender);
        void __fastcall tbPOSClick(TObject *Sender);
        void __fastcall tbStaffClick(TObject *Sender);
        void __fastcall tbMembershipClick(TObject *Sender);
        void __fastcall tbEftposClick(TObject *Sender);
        void __fastcall tbLoggingClick(TObject *Sender);
        void __fastcall rgMemberPoleDisplayClick(TObject *Sender);
        void __fastcall tbLogsResetClick(TObject *Sender);
        void __fastcall tbLogDebugInfoClick(TObject *Sender);
        void __fastcall tbLogPhoenixInfoClick(TObject *Sender);
        void __fastcall tbLogBTInfoClick(TObject *Sender);
        void __fastcall tbLogBTPacketInfoClick(TObject *Sender);
        void __fastcall tbLogBTDecodeInfoClick(TObject *Sender);
        void __fastcall cbOnLineLoggingClick(TObject *Sender);
        void __fastcall rgMemberFreeSalesRulesClick(TObject *Sender);
        void __fastcall rgLocationFreeSalesRulesClick(TObject *Sender);
        void __fastcall tbMemberQuickButtonsClick(TObject *Sender);
        void __fastcall cbHideServingCourseLabelsClick(TObject *Sender);
        void __fastcall cbRememberLastServingCourseClick(TObject *Sender);
        void __fastcall cbIntegratedEftposCadmusClick(TObject *Sender);
        void __fastcall cbEftposCadmusSerialPortChange(TObject *Sender);
        void __fastcall tbCadmusMerchantNumberClick(TObject *Sender);
        void __fastcall cbPromptPatronCountClick(TObject *Sender);
        void __fastcall tbtnLuckMemberPeriodClick(TObject *Sender);
        void __fastcall tbtnLuckMemberPeriodDayClick(TObject *Sender);
        void __fastcall tbtnLuckMemberPeriodWeekClick(TObject *Sender);
        void __fastcall tbtnLuckMemberPeriodMonthClick(TObject *Sender);
        void __fastcall TouchBtn1MouseClick(TObject *Sender);
        void __fastcall cbICELinkClick(TObject *Sender);
        void __fastcall cbDefaultToBevMenuClick(TObject *Sender);
        void __fastcall cbSummariseDiscountsonZedClick(TObject *Sender);
        void __fastcall cbRoundingClick(TObject *Sender);
        void __fastcall tbtnSyncroniseMembersipMouseClick(TObject *Sender);
        void __fastcall tbMinMembershipNumberClick(TObject *Sender);
        void __fastcall cbRecycleMembershipNumbersClick(TObject *Sender);
        void __fastcall tbSmartCardMouseClick(TObject *Sender);
        void __fastcall cbOnlyEarnPointsClick(TObject *Sender);
        void __fastcall cbIntegratedEftposDPSClick(TObject *Sender);
        void __fastcall cbEnableDPSTippingClick(TObject *Sender);
        void __fastcall cbEnableMembershipSmartCardsClick(TObject *Sender);
        void __fastcall memStaffMessageClick(TObject *Sender);
        void __fastcall tbtnMMSubNetClick(TObject *Sender);
        void __fastcall tbtnAddDefaultDeptMouseClick(TObject *Sender);
        void __fastcall tbtnDelDeptMouseClick(TObject *Sender);
        void __fastcall tbtnAddDeptMouseClick(TObject *Sender);
        void __fastcall tbtnDeptUpMouseClick(TObject *Sender);
        void __fastcall tbtnDeptDownMouseClick(TObject *Sender);
        void __fastcall tbtnDeptEditMouseClick(TObject *Sender);
        void __fastcall tbtnTabCreditGroupClick(TObject *Sender);
        void __fastcall cbvmAllowSmartCardNotPresentMemberTransactionsClick(TObject *Sender);
        void __fastcall cbSacleAutoConvertWeightsClick(TObject *Sender);
        void __fastcall cbScaleEnterWeightClick(TObject *Sender);
        void __fastcall cbScaleEnterPriceClick(TObject *Sender);
        void __fastcall cbEnableAARewardsClick(TObject *Sender);
        void __fastcall cbCadmusCronosClick(TObject *Sender);
        void __fastcall rgMemberNameOnReciptClick(TObject *Sender);
        void __fastcall cbDisableInternalInvoicingClick(TObject *Sender);
        void __fastcall cbAARewardsReminderClick(TObject *Sender);
        void __fastcall cbDEnableBlindBalancesClick(TObject *Sender);
        void __fastcall TouchBtn19MouseClick(TObject *Sender);
        void __fastcall cbDisableSaveConfirmationClick(TObject *Sender);
        void __fastcall cbEnableShowVoucherDetails(TObject *Sender);
        void __fastcall cbDisableReceiptOnConfirmationClick(TObject *Sender);
        void __fastcall lbeEntireSiteIDMouseUp(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall TouchBtn20MouseClick(TObject *Sender);
        void __fastcall cbNotifyBirthdayClick(TObject *Sender);
        void __fastcall cbAcknowledgeFirstVisitClick(TObject *Sender);
        void __fastcall tbRestLastVisitClick(TObject *Sender);
        void __fastcall cbEnableMemberSpendChitsClick(TObject *Sender);
        void __fastcall tbtnMemberSpendChitValueClick(TObject *Sender);
        void __fastcall cbDrawByLocationClick(TObject *Sender);
        void __fastcall cbAutoAddWebMembersClick(TObject *Sender);
        void __fastcall rgPOSBtnSizeClick(TObject *Sender);
        void __fastcall btnSetPOSFontSizeMouseClick(TObject *Sender);
        void __fastcall tbEndOfDayClick(TObject *Sender);
        void __fastcall bankingMouseClick(TObject *Sender);
        void __fastcall EnableStaffHoursOnClick(TObject *Sender);
        void __fastcall cbEnableCommissionClick(TObject *Sender);
        void __fastcall cbEnablePrinterCountsClick(TObject *Sender);
        void __fastcall cbEnablePaxCountClick(TObject *Sender);
        void __fastcall cbDisplayStockPurchaseWebordersClick(TObject *Sender);
        void __fastcall cbHideCredsCancelsClick(TObject *Sender);
        void __fastcall cbDepositBagNumClick(TObject *Sender);
        void __fastcall cbDontClearDataOnZedClick(TObject *Sender);
        void __fastcall cbSeparatePtsClick(TObject *Sender);
        void __fastcall cbCaptureCustomerNameClick(TObject *Sender);
        void __fastcall cbHoldSendClick(TObject *Sender);
        void __fastcall cbOnlyFullProductsClick(TObject *Sender);
        void __fastcall cbRedeemOnSelectedItemsClick(TObject *Sender);
        void __fastcall cbEarnPointsOnWeightClick(TObject *Sender);
        void __fastcall cbHideFreeItemsAtBillingClick(TObject *Sender);
        void __fastcall cbEnableWaiterStationClick(TObject *Sender);
        void __fastcall cbPoleDisplayShowPointsClick(TObject *Sender);
        void __fastcall rbNoGreetingClick(TObject *Sender);
        void __fastcall rbShowAliasClick(TObject *Sender);
        void __fastcall rbShowNameClick(TObject *Sender);
        void __fastcall rgRoundingTimesClick(TObject *Sender);
        void __fastcall cbNagUserToSelectChitClicked(TObject *);
        void __fastcall cbUseMenuDefaultPatronCountClicked(TObject *Sender);
        void __fastcall cbEnableExtendedDisplayClicked(TObject *Sender);
        void __fastcall cbDisplayTaxClick(TObject *Sender);
        void __fastcall cbDisplayServiceChargeClick(TObject *Sender);
        void __fastcall cbEnableNmiDisplayClick(TObject *Sender);
        void __fastcall cbDisplayTaxExemptClick(TObject *Sender);
        void __fastcall cbEndOfDayChange(TObject *Sender);
        void __fastcall cbEndOfDaySelect(TObject *Sender);
        void __fastcall cbInclServiceChargeTaxInTaxValClick(TObject *Sender);
        void __fastcall tbEnterWeightClick(TObject *Sender);
        bool GetInputViaTouchKeyboard(UnicodeString prompt, UnicodeString &destination, int max_length, bool allow_cr, bool start_with_caps);
        void __fastcall quick_member_setup_clicked(TObject *sender);
        void __fastcall cbEnablePhoneOrdersMouseClick(TObject *sender);
        void __fastcall cbEnableParkedSalesClicked(TObject *sender);
        // For Accumulated Zed Total
        void __fastcall cbShowAccumulatedTotalClick(TObject *Sender);
        void __fastcall cbShowSessionDateClick(TObject *Sender);
        void __fastcall cbShowTaxSummaryClick(TObject *Sender);
        void __fastcall cbShowServiceChargeSummaryClick(TObject *Sender);
        void __fastcall cbCombineServiceChargeAndServiceChargeTaxClick(TObject *Sender);
        void __fastcall cbCombineServiceChargeTaxAndSalesTaxClick(TObject *Sender);
        void __fastcall cbEnableClientDetailsClick(TObject *Sender);
        void __fastcall tbzedHeaderTextClick(TObject *Sender);
        void __fastcall cbWarnIfOpenTableAndTabsClick(TObject *Sender);
        void __fastcall cbStopIfOpenTableAndTabsClick(TObject *Sender);
        //story MM1837
        void __fastcall cbShowTransactionSummaryGroupsClick(TObject *Sender);
        void __fastcall cbShowBilledSalesTotalsClick(TObject *Sender);
        void __fastcall cbShowComplimentarySalesTotalsClick(TObject *Sender);
        void __fastcall cbShowChargedSalesTotalsClick(TObject *Sender);
        void __fastcall cbShowTotalsClick(TObject *Sender);
        void __fastcall cbShowBreakdownCategoriesClick(TObject *Sender);
        void __fastcall cbShowDiscountReportClick(TObject *Sender);
        void __fastcall cbShowPointsReportClick(TObject *Sender);
        void __fastcall cbShowPatronAveragesClick(TObject *Sender);
        void __fastcall cbShowProductionInfoClick(TObject *Sender);
        void __fastcall cbShowAccountPurchasesClick(TObject *Sender);
        void __fastcall cbShowAccountBalancesTabsClick(TObject *Sender);
        void __fastcall cbShowAccountBalancesClick(TObject *Sender);
        void __fastcall cbShowHourlySalesClick(TObject *Sender);
        void __fastcall cbRevenueFiguresAreTaxAndServiceChargeInclusiveClick(TObject *Sender);
        void __fastcall cbEnableTierLevelsClick(TObject *Sender);
        //add by frn
        void __fastcall tbCustomerDisplayClick(TObject *Sender);
        void __fastcall tbPOSDisplayClick(TObject *Sender);
        void __fastcall tbBirthdayBufferClick(TObject *Sender);
        void __fastcall tbFirstVisitPointClick(TObject *Sender);
        void __fastcall tbMinRedemptionPointClick(TObject *Sender);
        void __fastcall cbvmAllowMemberDetailscreenClick(TObject *Sender);
        void __fastcall cbAllowPointPaymentByWeightClick(TObject *Sender);
        void __fastcall cbAllowPointPaymentByValueClick(TObject *Sender);
        void __fastcall cbUpdateTableGUIOnOrderStatusClick(TObject *Sender);
        void __fastcall cbRefundPointsClick(TObject *Sender);   //mm-3395
        void __fastcall cbShowCurrencySymbolClick(TObject *Sender);
        void __fastcall cbSkipConfirmationOnFastTenderClick(TObject *Sender);
        void __fastcall cbTransferTableOnPrintPrelimClick(TObject *Sender);
        void __fastcall cbEmailZedClosingTillClick(TObject *Sender);
        void __fastcall tbtnConfigureEmailClick(TObject *Sender);
        void __fastcall ValidateEmailId(UnicodeString emailidslist);
        void __fastcall cbShowRemovalReportClick(TObject *Sender);
        void __fastcall cbEnableCancelCheckRemovalClick(TObject *Sender);
        void __fastcall cbRevenueFiguresAreDiscountInclusiveClick(TObject *Sender);
        void __fastcall cbRoundOnBillingClick(TObject *Sender);
        void __fastcall tbRoundingOnBillingClick(TObject *Sender);
        void __fastcall cbShowPriceAdjustmentClick(TObject *Sender);
        void __fastcall cbvmMandatoryMembershipCardClick(TObject *Sender);
        void __fastcall cbPointOnlyClick(TObject *Sender);    //MM-4775
        void __fastcall cbEnableApprovedLoginClick(TObject *Sender);
        void __fastcall cbLinkClockInOutApprovedLoginClick(TObject *Sender);
        void _fastcall cbResetMemberPointPurchasedClick(TObject *Sender);
        void _fastcall cbResetMemberPointRedeemedClick(TObject *Sender);
        void _fastcall cbResetMemberPointEarnedClick(TObject *Sender);
        void _fastcall edDayClick(TObject *Sender);
        void _fastcall cbPromptPatronCountOnTableSalesClick(TObject *Sender);// MM-5085
        void _fastcall cbPostToXeroClick(TObject *Sender);
        void _fastcall cbConfirmResetPointClick(TObject *Sender);
        void _fastcall cbGiftCardOnlyClick(TObject *Sender);
        void _fastcall tbtnSmartLinkIpClick(TObject *Sender);
        void _fastcall cbDeleteItemSizeAfterSaleClick(TObject *Sender);
        void __fastcall cbDisplayItemSizesAsListClick(TObject *Sender);
        void __fastcall cbShowItemPricesInTransferScreenClick(TObject *Sender);
        void _fastcall cbPostMoneyAsPaymentsClick(TObject *Sender);
        void _fastcall cbPostMoneyToGLAccountsClick(TObject *Sender);
        void __fastcall cbTerminalExemptFromHappyHourClick(TObject *Sender);
        void __fastcall cbNotifyForManuallyEnteredWeightClicked(TObject *Sender);
        void __fastcall cbUseNMIStandardFontSizeClick(TObject *Sender);
        void __fastcall cbShowScreenToSelectItemsClick(TObject *Sender);
        void __fastcall cbCountPointSpentInRevenueClick(TObject *Sender);
        void __fastcall cbShowLargeFontsClick(TObject *Sender);
        void __fastcall cbItemSearchClick(TObject *Sender);   //cbShowDarkBackgroundInPOS
        void __fastcall cbShowDarkBackgroundInPOSClick(TObject *Sender);
        void __fastcall cbOpenCashDrawerClick(TObject *Sender);
        void __fastcall cbMergeSimilarItemClick(TObject *Sender);
        //void __fastcall cbShowLocalTaxandProfitTaxClick(TObject *Sender);
        void __fastcall cbUseBIRFormatInXZReportClick(TObject *Sender);
        //void __fastcall cbUseBIRFormatInXZReportOnEnter(TObject *Sender);
        void __fastcall cbUseBIRFormatInXZReportMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);

        void __fastcall cbExcludeReceiptClick(TObject *Sender);
        void __fastcall cbExcludeXReportClick(TObject *Sender);
        void __fastcall cbHideRoundingOnReceiptClick(TObject *Sender);
		void __fastcall cbCashDenominationEntryClick(TObject *Sender);
        void __fastcall cbUseMemberSubsClick(TObject *Sender);
        void __fastcall cbFloatWithdrawFromCashClick(TObject *Sender);
        void __fastcall cbSplitBillByMenuTypeClick(TObject *Sender);
        void __fastcall cbShowCashDrawerCountClick(TObject *Sender);
        void __fastcall cbShowReprintDetailsClick(TObject *Sender);
        void __fastcall cbIntegratedEftposSmartConnectClick(TObject *Sender);
protected:
        void __fastcall WMDisplayChange(TWMDisplayChange& Message);
         BEGIN_MESSAGE_MAP
                MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
         END_MESSAGE_MAP(TForm)
private:
        void CommitSalesforceEndOfDayOptions(Database::TDBTransaction &);
        __fastcall TfrmGeneralMaintenance(TComponent* Owner,Database::TDBControl &inDBControl);
        void RedrawButtons(TObject * Sender);	// User declarations
        Database::TDBControl &DBControl;
        void UpdateTimeClockGrid(bool GridEnabled);
        void DetermineAndSetDefaultFontSize(Database::TDBTransaction &, int);
        void SetPosFontSize(Database::TDBTransaction &, int);
        void CallBlindBalance(void);
        void SeperatePoints(void);
        void ClearAllParkedSales( Database::TDBTransaction &DBTransaction );
        bool invalid_email_id;
        void DisplayTextOnWeightLimit();
        void EnableDisableSectionsForBIRFormat();
        bool isBIRSettingTicked;
        void CheckSettingsOfZed();
        bool IsEligibleForTrue();
public:		// User declarations
        TTouchBtn *CurrentButton;
        void __fastcall ReDrawLogs();


};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmGeneralMaintenance *frmGeneralMaintenance;
//---------------------------------------------------------------------------
#endif
