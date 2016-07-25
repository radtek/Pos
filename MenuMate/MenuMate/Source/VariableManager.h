// ---------------------------------------------------------------------------

#ifndef VariableManagerH
#define VariableManagerH

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
   vmgMisc, vmgPrinters, vmgSystem, vmgNone
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
   vmEnableAARewardsReminder,
   // Connectiosn  ----------------------------------------------------------------
   vmPMSIPAddress = 2100, vmPMSTCPPort, vmPMSPOSID, vmPMSPaymentCategory, vmPMSItemCategory, vmPMSPointsCategory, vmPMSCreditCategory,
   vmPMSDefaultAccount, vmPMSDefaultSurchargeAccount, vmPMSDefaultRoundingAccount, vmIntaMateIPAddress, vmIntaMatePort,
   vmIntaMateTerminalID, vmIntaMateIPTimeOut, vmPocketVoucherURL, vmEBetID,
   vmWebMatePort, vmWebMateEnabled,

   // Stock Master ----------------------------------------------------------------
   vm_PLACEHOLDER_UseStockMaster = 2200, vmStockMasterExportPath,
   // Misc Equip   ----------------------------------------------------------------
   vmPoleDisplayTopLine = 3000, vmPoleDisplayBottomLine, vmPoleDisplayPort, vmPoleDisplayEmulationMode, vmSerialKickPort,
   vmSerialKickPortLength, vmSerialKickPortHighChars, vm_PLACEHOLDER_ProximityPort, vmBarCodeReaderPort, vmBluetoothPort,
   vmBluetoothSerialTimeOut, vmSerialPort, vmSerialBaudRate, vmSerialDataBits, vmSerialStopBits, vmSerialParityBits,
   vmSerialRTSFlowControl, vmSerialOutCTSFlow, vmSerialXonXoffOut, vmSerialXonXoffIn, vmSecuritySerialProfileName, vmSerialAsync,
   vmSmartCardReader, vmSmartCardCode, vmProxWiegandFormat, vmScalesProfile, vmAutoConvertScalesWeights, vmProxProfile,
   // POS        ------------------------------------------------------------------
   vmAlwaysForceLogin = 4000, vmAutoLogoutPOS, vmAutoLogoutSeconds, vmForcedReasons, vmRevertToFirstCourse,
   vm_PLACEHOLDER_OptionsMultiSelect, vmForcedSidesFirst, vmForceTender, vmFTBypassElecTranTyp, vmNonServicePeriod, vmClearLogsDays,
   vmMaxLogsRecords, vm_PLACEHOLDER_MemberNumericPannelFirst, vmStaffNumericPannelFirst, vmDisableManualLogin, vmMemberIncrementalSearch,
   vmMemberNameOnPoleDisplay, vmMemberLoyaltyRewardsRules, vmLocationLoyaltyRewardsRules, vmMemberQuickButtonCount,
   vm_PLACEHOLDER_DisplayServingCourseColours, vmHideServingCourseLabels, vm_PLACEHOLDER_ShowSingleServingCourses,
   vmRememberLastServingCourse, vmPromptForPatronCount, vm_PLACEHOLDER_DefaultItemQuickMode, vmPOSDefaultsToBeverages,
   vmStaffMessageOfTheDay, vmAllowSmartCardNotPresentMemberTransactions, vmDisableInternalInvoicing, vmEnableBlindBalances,
   vmDisableConfirmationOnSave, vmDisableReceiptOnConfirmation, vmEnableBirthdayNotifications, vmAcknowledgeFirstVisit,
   vmEnableMemberSpendChits, vmMemberSpendChitsAmount,
   // Happy Hour ------------------------------------------------------------------
   vmHappyHourExempt = 5000, vmHappyHourStart, vmHappyHourFinish, vmHHSunday, vmHHMonday, vmHHTuesday, vmHHWednesday, vmHHThursday,
   vmHHFridays, vmHHSaturday, vmLuckyMemberPeriod, vmHappyHourMembersOnly, vmLuckyMemberDrawByLocationOnly,
   // Points Config ---------------------------------------------------------------
   vmEarnsPointsWhileRedeeming = 5200, vmMinMemberNumber, vmRecycleMemberNumber, vmOnlyEarnsPointsWhileRedeeming,
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
   // Printers   ------------------------------------------------------------------
   vmPrinterModel = 7000, vmPrinterEmulationMode, vmNormalCPL, vmNormalBoldCPL, vmCutterGap, vmTopMargin, vmKitchenTopMargin,
   vmAlwaysUseFullCut, vmKanjiPrinter, vmSingleLineChar, vmDoubleLineChar, vmGraphicPrinter, vmGraphicFontName, vmGraphicFontSize,
   vmGraphicHeaderX, vmGraphicHeaderY, vmGraphicHeaderWidth, vmGraphicHeaderHeight, vmGraphicHeaderFile, vmGraphicReceiptX,
   vmGraphicReceiptY, vmGraphicReceiptMaxLines, vmGraphicBackGroundX, vmGraphicBackGroundY, vmGraphicBackGroundWidth,
   vmGraphicBackGroundHeight, vmGraphicBackGroundFile, vmGraphicPageCountX, vmGraphicPageCountY, vmPrinterCodePage,
   vmPrinterCodePageOn,vmPrinterCodePageOff,vmPrinterInitCmd
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

   bool GetProfileIntDB(Database::TDBTransaction &Transaction, int ProfileKey, vmVariables VariablesKey, int &Value);
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

   int LocationProfileKey;
   int DeviceProfileKey;

   bool Delete(Database::TDBTransaction &Transaction, vmVariables VariablesKey);

   UnicodeString GetStr(Database::TDBTransaction &Transaction, vmVariables VariablesKey, UnicodeString Default = "");
   int GetInt(Database::TDBTransaction &Transaction, vmVariables VariablesKey, int Default = 0);
   double GetNum(Database::TDBTransaction &Transaction, vmVariables VariablesKey, double Default = 0);
   bool GetBool(Database::TDBTransaction &Transaction, vmVariables VariablesKey, bool Default = false);
   TDateTime GetTime(Database::TDBTransaction &Transaction, vmVariables VariablesKey, TDateTime Default = 0);

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
   void SetProfileNum(Database::TDBTransaction &Transaction, int ProfileKey, vmVariables VariablesKey, double Value);
   void SetProfileBool(Database::TDBTransaction &Transaction, int ProfileKey, vmVariables VariablesKey, bool Value);
   void SetProfileTime(Database::TDBTransaction &Transaction, int ProfileKey, vmVariables VariablesKey, TDateTime Value);

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

