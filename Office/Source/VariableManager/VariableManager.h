//---------------------------------------------------------------------------

#ifndef VariableManagerH
#define VariableManagerH

#include "Manager.h"
#include <map>
#include "MM_DBCore.h"

//---------------------------------------------------------------------------
enum ProfileType{eLocationProfiles,eTerminalProfiles,eEquipmentProfiles,eNetworkProfiles,eSystemProfiles};

const char* const vmProfileTypes[]   = {"Location Profiles",
													"POS Terminal Profiles",
													"Equipment Profiles",
													"Network Profiles",
													"System Profiles",
                                       NULL};

enum vmGroup {vmgMenuMateModules,vmg3rdPartyInterface,vmgPOS,vmgSecurity,
					vmgPrinting,vmgMiscEquipment,vmgHappyHour,vmgNetwork,
					vmgLogging,vmgMisc, vmgPrinters, vmgSystem,
              	vmgNone};

const char* const vmGroups[] = 		  {
// Terminal Profile & Location Profile Groups ----------------------------------
													"MenuMate Modules",
                                       "3rd Party Interfaces",
                                       "POS Behaviour",
                                       "Security Settings",
													"Printing",
													"Misc Equipment",
													"Happy Hour",
                                       "Networking",
													"Logging",
													"Misc",
// Equipment Profile Groups ----------------------------------------------------
                                       "Printers",
                                       "Pole Displays",
                                       "Cash Drawers",
                                       "System",
                                       NULL};


enum vmVariables	{
// System 	  ------------------------------------------------------------------
							vmbtnDollar1 ,vmbtnDollar2,vmbtnDollar3,vmbtnDollar4,vmbtnDollar5,
							Deprecated_vmEnabledStateSync,vmInfoSync,vmVersionNumber,
                     vmMidPointRoundsDown,vmSiteID,

// Path Config------------------------------------------------------------------
							vmAutoMateIP = 1000, vmPLACE_HOLDER_FOR_OLD_ChefMateIP, vmStockInterbaseIP,
						 	vmStockDatabasePath, vmSntpIP,
                   	vmMembershipDatabaseIP, vmMembershipDatabasePath,
// Modules    ------------------------------------------------------------------
							vmGuestMasterPath = 2000,vmUseERSMemberShip,
							vmTabsEnabled,vmTablesEnabled,vmChefMateColour,
							vmEnableEftPosSyncro,vmEnableEftPosANZ,
							vmEnableEftPosIngenico,vmTrackSaleAndMakeTimes,
							vmEftposMerchantNumber,vmEnableEftPosCadmus,
							vmEftposSerialPort,vmEnableEftPosIceLink,vmEnableEftPosDPS,
                     vmSmartCardMembership,
// Phoenix      ----------------------------------------------------------------
							vmPhoenixIPAddress = 2100,vmPhoenixTCPPort,vmPhoenixPOSID,
							vmPhoenixPaymentCategory,vmPhoenixItemCategory,
							vmPhoenixPointsCategory,vmPhoenixCreditCategory,
							vmPhoenixDefaultAccount,vmPhoenixDefaultSurchargeAccount,
							vmPhoenixDefaultRoundingAccount,
// Stock Master ----------------------------------------------------------------
							vmUseStockMaster = 2200, vmStockMasterExportPath,
// Misc Equip   ----------------------------------------------------------------
							vmPoleDisplayTopLine = 3000,vmPoleDisplayBottomLine,vmPoleDisplayPort,
							vmPoleDisplayEmulationMode,
							vmSerialKickPort,vmSerialKickPortLength,vmSerialKickPortHighChars,
							vmProximityPort,vmBarCodeReaderPort,vmBluetoothPort,
							vmBluetoothSerialTimeOut,vmSerialPort,vmSerialBaudRate,
                     vmSerialDataBits,vmSerialStopBits,vmSerialParityBits,
                     vmSerialRTSFlowControl,vmSerialOutCTSFlow,
                     vmSerialXonXoffOut,vmSerialXonXoffIn,vmSecuritySerialProfileName,
                     vmSerialAsync,vmSmartCardReader,vmSmartCardCode,
// POS        ------------------------------------------------------------------
							vmAlwaysForceLogin = 4000, vmAutoLogoutPOS, vmAutoLogoutSeconds,
							vmForcedReasons, vmRevertToFirstCourse, vmPLACE_HOLDER_FOR_OLD_OptionsMultiSelect,
							vmForcedSidesFirst,vmForceTender,vmFTBypassElecTranTyp,
							vmNonServicePeriod, vmClearLogsDays,vmMaxLogsRecords,
							vmPLACE_HOLDER_FOR_OLD_MemberNumericPannelFirst,
							vmStaffNumericPannelFirst,vmDisableManualLogin,
							vmMemberIncrementalSearch,vmMemberNameOnPoleDisplay,
							vmMemberLoyaltyRewardsRules,vmLocationLoyaltyRewardsRules,
							vmMemberQuickButtonCount, vmDisplayServingCourseColours,vmHideServingCourseLabels,
							vmPLACE_HOLDER_FOR_OLD_ShowSingleServingCourses,vmRememberLastServingCourse,
							vmPromptForPatronCount,vmPLACE_HOLDER_FOR_OLD_DefaultItemQuickMode,vmPOSDefaultsToBeverages,
                            vmStaffMessageOfTheDay, vmAllowSmartCardNotPresentMemberTransactions, vmDisableInternalInvoicing, vmEnableBlindBalances,
                            vmDisableConfirmationOnSave, vmDisableReceiptOnConfirmation, vmEnableBirthdayNotifications, vmAcknowledgeFirstVisit,
                            vmEnableMemberSpendChits, vmMemberSpendChitsAmount, vmShowVoucher,vmPosGuiDimensionsColumns,vmPosGuiDimensionsRows,
                            vmAutoZProfileKey, vmAutoZCheck1Time, vmAutoZCheck2Time, vmAutoZCheck3Time,
                            vmAutoZCheck4Time, vmAutoZCheck1Options, vmAutoZCheck2Options, vmAutoZCheck3Options, vmAutoZCheck4Options, vmAutoZAuthAs,
                            vmPosGridFontSize, vmEnableStaffHours, vmEnableCommission, vmEnablePrinterCounts, vmEnablePaxCount, vmPrintStockWeborders,
                            vmSalesforceUploadEnabled, vmSalesforceUsername, vmSalesforcePassword, vmSalesforceSecurityToken, vmHideCredsCancels, vmEnableSeperateEarntPts,
                            vmQuickMemberSetupEnabled,
                            vmQuickMemberSetupPhoneIsMandatory,
                            vmQuickMemberSetupEmailIsMandatory,
                            vmQuickMemberSetupDateOfBirthIsMandatory,
                            vmSalesforceUploadStock,
// Happy Hour ------------------------------------------------------------------
							vmHappyHourExempt = 5000, vmHappyHourStart, vmHappyHourFinish,
							vmHHSunday, vmHHMonday, vmHHTuesday, vmHHWednesday, vmHHThursday,
							vmHHFridays, vmHHSaturday,vmLuckyMemberPeriod,vmHappyHourMembersOnly,
// Points Config ---------------------------------------------------------------
							vmEarnsPointsWhileRedeeming = 5200,vmMinMemberNumber,vmRecycleMemberNumber,
                     vmOnlyEarnsPointsWhileRedeeming,
// POS Printing ----------------------------------------------------------------
							vmReceiptPrinter = 6000, vmPrintNoteWithDiscount,vmPLACE_HOLDER_FOR_OLD_LargeKitchenFont,
							vmDuplicateReceipts,vmPLACE_HOLDER_FOR_OLD_HeaderForEachCourse,
							vmDuplicateEftPosReceipt, vmDuplicateSignatureReceipts,
							vmChitNumbers, vmChitLabel,vmCustomChitLabel,vmSeatLabel,
							vmPointsPaymentGroupNumber,vmPLACE_HOLDER_FOR_OLD_CutType,
							vmTipOnRecipt,vmOptionsOnRecipt,vmSignatureOnRecipt,vmSumPricingChangesOnRecipt,
							vmDisplayBarCodesOnReceipt,vmDisplayChitNumberOnReceipt,
							vmAlwaysPrintReceiptTenderedSales,vmConsolidateReceipt,vmAlphaNumChitNumbers,
							vmShowPointsOnZed,vmShowInvoiceInfoOnZed,vmUnRavelKitchenPrinting,
							vmKitchenPerSeatMultiplerString,vmKitchenBulletSide,vmKitchenHeaderSide,
							vmKitchenBulletOpt,vmKitchenHeaderOpt,vmKitchenBulletNote,
							vmKitchenHeaderNote,vmKitchenBulletMix,vmKitchenHeaderMix,
							vmSaveAndPrintPrintsPartialOrder,vmHideFreeItemsOnReceipt,
							vmHideZeroDollarsOnReceipt,vmHideCancelledItemsOnReceipts,
							vmHidePrevPayemntsOnReceipts,vmAlwaysPrintReceiptCashSales,
							vmRavellItemsWithPriceAdjusts,vmPLACE_HOLDER_FOR_OLD_PrintByServingCourse,
							vmKitchenBulletSetMenuItem,vmAutoPrintTabReceipts,
							vmAutoPrintStaffReceipts,vmAutoPrintMemberReceipts,
							vmAutoPrintTableReceipts,vmAutoPrintCashReceipts,
							vmAutoPrintRoomReceipts,vmAutoPrintInvoiceReceipts,
							vmSumDiscountsOnRecipt,vmSummariseDiscountOnZed,
                     vmHideMembershipDetialsOnReceipts,
// Printers   ------------------------------------------------------------------
							vmPrinterModel = 7000,vmPrinterEmulationMode,vmNormalCPL,
							vmNormalBoldCPL,vmCutterGap,
							vmTopMargin,vmKitchenTopMargin,vmAlwaysUseFullCut,
							vmKanjiPrinter,vmSingleLineChar,vmDoubleLineChar,
                     vmGraphicPrinter,vmGraphicFontName,vmGraphicFontSize,
                     vmGraphicHeaderX,vmGraphicHeaderY,vmGraphicHeaderWidth,
                     vmGraphicHeaderHeight,vmGraphicHeaderFile,
                     vmGraphicReceiptX,vmGraphicReceiptY,vmGraphicReceiptMaxLines,
                     vmGraphicBackGroundX,vmGraphicBackGroundY,vmGraphicBackGroundWidth,
                     vmGraphicBackGroundHeight,vmGraphicBackGroundFile,vmGraphicPageCountX,
                     vmGraphicPageCountY
						};

//---------------------------------------------------------------------------
class TManagerVariable : public TManager
{
	void SetVarStr(Database::TDBTransaction &Transaction,vmVariables VariablesKey,AnsiString Name,AnsiString Discription,vmGroup Group,AnsiString Value);
	void SetVarInt(Database::TDBTransaction &Transaction,vmVariables VariablesKey,AnsiString Name,AnsiString Discription,vmGroup Group,int Value);
	void SetVarNum(Database::TDBTransaction &Transaction,vmVariables VariablesKey,AnsiString Name,AnsiString Discription,vmGroup Group,double Value);
	void SetVarBool(Database::TDBTransaction &Transaction,vmVariables VariablesKey,AnsiString Name,AnsiString Discription,vmGroup Group,bool Value);
	void SetVarTime(Database::TDBTransaction &Transaction,vmVariables VariablesKey,AnsiString Name,AnsiString Discription,vmGroup Group,TDateTime Value);

	bool GetVarStr(Database::TDBTransaction &Transaction,vmVariables VariablesKey, AnsiString &Value);
	bool GetVarInt(Database::TDBTransaction &Transaction,vmVariables VariablesKey, int &Value);
	bool GetVarNum(Database::TDBTransaction &Transaction,vmVariables VariablesKey, double &Value);
	bool GetVarBool(Database::TDBTransaction &Transaction,vmVariables VariablesKey,bool &Value);
	bool GetVarTime(Database::TDBTransaction &Transaction,vmVariables VariablesKey,TDateTime &Value);

	bool GetDeviceStr(Database::TDBTransaction &Transaction,vmVariables VariablesKey, AnsiString &Value);
	bool GetDeviceInt(Database::TDBTransaction &Transaction,vmVariables VariablesKey, int &Value);
	bool GetDeviceNum(Database::TDBTransaction &Transaction,vmVariables VariablesKey, double &Value);
//	bool GetDeviceBool(Database::TDBTransaction &Transaction,vmVariables VariablesKey,bool &Value);
	bool GetDeviceTime(Database::TDBTransaction &Transaction,vmVariables VariablesKey,TDateTime &Value);

	bool DeleteProfileVariable(Database::TDBTransaction &Transaction,int ProfileKey,vmVariables VariablesKey);

	bool GetProfileIntDB(Database::TDBTransaction &Transaction,int ProfileKey, vmVariables VariablesKey, int &Value);
	bool GetProfileNumDB(Database::TDBTransaction &Transaction,int ProfileKey, vmVariables VariablesKey, double &Value);
	bool GetProfileStrDB(Database::TDBTransaction &Transaction,int ProfileKey, vmVariables VariablesKey, AnsiString &Value);
	bool GetProfileTimeDB(Database::TDBTransaction &Transaction,int ProfileKey, vmVariables VariablesKey, TDateTime &Value);
   int GetProfileDB(Database::TDBTransaction &Transaction,ProfileType Type,AnsiString ProfileName);

   // Cache Access Structures and Funtions.
   std::map< ProfileType , std::map<AnsiString,int> >CacheGetProfile;
   bool GetProfileCache(ProfileType Type,AnsiString ProfileName,int &Value);

   std::map< int , std::map<vmVariables,int> >CacheGetProfileInt;
	bool GetProfileIntCache(int ProfileKey, vmVariables VariablesKey, int &Value);

   std::map< int , std::map<vmVariables,double> >CacheGetProfileNum;
   bool GetProfileNumCache(int ProfileKey, vmVariables VariablesKey, double &Value);

   std::map< int , std::map<vmVariables,AnsiString> > CacheGetProfileStr;
	bool GetProfileStrCache(int ProfileKey, vmVariables VariablesKey, AnsiString &Value);

   std::map< int , std::map<vmVariables,TDateTime> > CacheGetProfileTime;
	bool GetProfileTimeCache(int ProfileKey, vmVariables VariablesKey, TDateTime &Value);

   public:

   int LocationProfileKey;
   int DeviceProfileKey;

	bool Delete(Database::TDBTransaction &Transaction,vmVariables VariablesKey);

	AnsiString 	GetStr(Database::TDBTransaction &Transaction,vmVariables VariablesKey,AnsiString Default = "");
	int 			GetInt(Database::TDBTransaction &Transaction,vmVariables VariablesKey,int Default = 0);
	double 		GetNum(Database::TDBTransaction &Transaction,vmVariables VariablesKey,double Default = 0);
	bool 			GetBool(Database::TDBTransaction &Transaction,vmVariables VariablesKey,bool Default = false);
	TDateTime 	GetTime(Database::TDBTransaction &Transaction,vmVariables VariablesKey,TDateTime Default = 0);

	void SetDeviceStr(Database::TDBTransaction &Transaction,vmVariables VariablesKey,AnsiString Value);
	void SetDeviceInt(Database::TDBTransaction &Transaction,vmVariables VariablesKey,int Value);
	void SetDeviceNum(Database::TDBTransaction &Transaction,vmVariables VariablesKey,double Value);
	void SetDeviceBool(Database::TDBTransaction &Transaction,vmVariables VariablesKey,bool Value);
	void SetDeviceTime(Database::TDBTransaction &Transaction,vmVariables VariablesKey,TDateTime Value);

	int GetProfile(Database::TDBTransaction &Transaction,ProfileType Type,AnsiString ProfileName);
	int SetProfile(Database::TDBTransaction &Transaction,ProfileType Type,AnsiString ProfileName);
	bool SetProfile(Database::TDBTransaction &Transaction,int ProfileKey,ProfileType Type,AnsiString ProfileName);
	bool DelProfile(Database::TDBTransaction &Transaction,ProfileType Type,AnsiString ProfileName);
	bool DelProfile(Database::TDBTransaction &Transaction,int ProfileKey);

	void SetProfileStr(Database::TDBTransaction &Transaction,int ProfileKey, vmVariables VariablesKey, AnsiString Value);
	void SetProfileInt(Database::TDBTransaction &Transaction,int ProfileKey, vmVariables VariablesKey, int Value);
	void SetProfileInt(Database::TDBTransaction &Transaction,int ProfileKey,vmVariables VariablesKey, unsigned char Value);
	void SetProfileInt(Database::TDBTransaction &Transaction,int ProfileKey,vmVariables VariablesKey, char Value);
	void SetProfileNum(Database::TDBTransaction &Transaction,int ProfileKey, vmVariables VariablesKey, double Value);
	void SetProfileBool(Database::TDBTransaction &Transaction,int ProfileKey, vmVariables VariablesKey, bool Value);
	void SetProfileTime(Database::TDBTransaction &Transaction,int ProfileKey, vmVariables VariablesKey, TDateTime Value);

	bool GetProfileStr(Database::TDBTransaction &Transaction,int ProfileKey, vmVariables VariablesKey, AnsiString &Value);
	bool GetProfileInt(Database::TDBTransaction &Transaction,int ProfileKey, vmVariables VariablesKey, int &Value);

	bool GetProfileInt(Database::TDBTransaction &Transaction,int ProfileKey, vmVariables VariablesKey, unsigned char &Value);
	bool GetProfileInt(Database::TDBTransaction &Transaction,int ProfileKey, vmVariables VariablesKey, char &Value);
	bool GetProfileNum(Database::TDBTransaction &Transaction,int ProfileKey, vmVariables VariablesKey, double &Value);
	bool GetProfileBool(Database::TDBTransaction &Transaction,int ProfileKey, vmVariables VariablesKey, bool &Value);
	bool GetProfileTime(Database::TDBTransaction &Transaction,int ProfileKey, vmVariables VariablesKey, TDateTime &Value);
   
	TManagerVariable();
	void Initialise(Database::TDBTransaction &DBTransaction);
};
extern TManagerVariable *VM;

#endif
