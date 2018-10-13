//---------------------------------------------------------------------------

#ifndef MenumateVersionParserH
#define MenumateVersionParserH

//---------------------------------------------------------------------------

#include "VersionParser.h"
#include <map>
#define vPaymentAcess        0x40000000
#define vTaxRemoval          0x80000000
#define FinancialPaid        0x0000080
#define DiscountsAllowed     0x0000040
#define vReprintReceipt 	 0x020000000
//---------------------------------------------------------------------------

namespace MenumateVersionParser
{
    using Database::TDBControl;
    using Database::TDBTransaction;
    using Database::TDBUtilities;

    enum TArchiveType { ARCHIVE = 0, DAYARCHIVE };

    enum TParserVersions
    {
            pv5_18,pv5_19,pv5_20,pv5_21,pv5_22,pv5_23,pv5_24,pv5_25,pv5_26,pv5_27,pv5_28,pv5_29,pv5_30,
            pv5_31,pv5_32,pv5_33,pv5_34,pv5_35,pv5_36,pv5_37,pv5_38,

            //version 6
            pv6_00,pv6_01,pv6_02,pv6_03,pv6_04,pv6_05,pv6_06,pv6_07,pv6_08,pv6_09,pv6_10,
            pv6_11,pv6_12,pv6_13,pv6_14,pv6_15,pv6_16,pv6_17,pv6_18,pv6_19,pv6_20,
            pv6_21,pv6_22,pv6_23,pv6_24,pv6_25,pv6_26,pv6_27,pv6_28,pv6_29,pv6_30,
            pv6_31,pv6_32,pv6_33,pv6_34,pv6_35,pv6_36,pv6_37,pv6_38,pv6_39,pv6_40,pv6_41, pv6_42, pv6_43,pv6_44,
            pv6_45,pv6_46,pv6_47,pv6_48,pv6_49,pv6_50,pv6_51,pv6_52, pv6_53  };

    class TApplyParser
    {
    public:

        static TApplyParser* Instance();

        PARSER_ERROR Apply(
                          TParserVersions inVersion,
                        TDBControl* const inDBControl );

        std::string LastErrorMsg();

    private:
        TApplyParser();
        static TApplyParser* applyParser;

        std::string previous_version;
        std::string current_version;

        PARSER_ERROR apply5_18( TDBControl* const inDBControl );
        PARSER_ERROR apply5_19( TDBControl* const inDBControl );
        PARSER_ERROR apply5_20( TDBControl* const inDBControl );
        PARSER_ERROR apply5_21( TDBControl* const inDBControl );
        PARSER_ERROR apply5_22( TDBControl* const inDBControl );
        PARSER_ERROR apply5_23( TDBControl* const inDBControl );
        PARSER_ERROR apply5_24( TDBControl* const inDBControl );
        PARSER_ERROR apply5_25( TDBControl* const inDBControl );
        PARSER_ERROR apply5_26( TDBControl* const inDBControl );
        PARSER_ERROR apply5_27( TDBControl* const inDBControl );
        PARSER_ERROR apply5_28( TDBControl* const inDBControl );
        PARSER_ERROR apply5_29( TDBControl* const inDBControl );
        PARSER_ERROR apply5_30( TDBControl* const inDBControl );
        PARSER_ERROR apply5_31( TDBControl* const inDBControl );
        PARSER_ERROR apply5_32( TDBControl* const inDBControl );
		PARSER_ERROR apply5_33( TDBControl* const inDBControl );
		PARSER_ERROR apply5_34( TDBControl* const inDBControl );
		PARSER_ERROR apply5_35( TDBControl* const inDBControl );
        PARSER_ERROR apply5_36( TDBControl* const inDBControl );
        PARSER_ERROR apply5_37( TDBControl* const inDBControl );
        PARSER_ERROR apply5_38( TDBControl* const inDBControl );

        // Version 6.x
	    PARSER_ERROR apply6_00( TDBControl* const inDBControl );
	    PARSER_ERROR apply6_01( TDBControl* const inDBControl );
        PARSER_ERROR apply6_02( TDBControl* const inDBControl );
        PARSER_ERROR apply6_03( TDBControl* const inDBControl );
        PARSER_ERROR apply6_04( TDBControl* const inDBControl );
        PARSER_ERROR apply6_05( TDBControl* const inDBControl );
        PARSER_ERROR apply6_06( TDBControl* const inDBControl );
        PARSER_ERROR apply6_07( TDBControl* const inDBControl );
        PARSER_ERROR apply6_08( TDBControl* const inDBControl );
        PARSER_ERROR apply6_09( TDBControl* const inDBControl );
        PARSER_ERROR apply6_10( TDBControl* const inDBControl );
        PARSER_ERROR apply6_11( TDBControl* const inDBControl );
        PARSER_ERROR apply6_12( TDBControl* const inDBControl );
        PARSER_ERROR apply6_13( TDBControl* const inDBControl );
        PARSER_ERROR apply6_14( TDBControl* const inDBControl );
        PARSER_ERROR apply6_15( TDBControl* const inDBControl );
        PARSER_ERROR apply6_16( TDBControl* const inDBControl );
        PARSER_ERROR apply6_17( TDBControl* const inDBControl );
        PARSER_ERROR apply6_18( TDBControl* const inDBControl );
        PARSER_ERROR apply6_19( TDBControl* const inDBControl );
        PARSER_ERROR apply6_20( TDBControl* const inDBControl );
        PARSER_ERROR apply6_21( TDBControl* const inDBControl );
        PARSER_ERROR apply6_22( TDBControl* const inDBControl );
        PARSER_ERROR apply6_23( TDBControl* const inDBControl );
        PARSER_ERROR apply6_24( TDBControl* const inDBControl );
        PARSER_ERROR apply6_25( TDBControl* const inDBControl );
        PARSER_ERROR apply6_26( TDBControl* const inDBControl );
        PARSER_ERROR apply6_27( TDBControl* const inDBControl );
		PARSER_ERROR apply6_28( TDBControl* const inDBControl );
        PARSER_ERROR apply6_29( TDBControl* const inDBControl );
        PARSER_ERROR apply6_30( TDBControl* const inDBControl );
        PARSER_ERROR apply6_31( TDBControl* const inDBControl );
        PARSER_ERROR apply6_32( TDBControl* const inDBControl );
        PARSER_ERROR apply6_33( TDBControl* const inDBControl );
        PARSER_ERROR apply6_34( TDBControl* const inDBControl );
        PARSER_ERROR apply6_35( TDBControl* const inDBControl );
        PARSER_ERROR apply6_36( TDBControl* const inDBControl );
        PARSER_ERROR apply6_37( TDBControl* const inDBControl );
        PARSER_ERROR apply6_38( TDBControl* const inDBControl );
        PARSER_ERROR apply6_39( TDBControl* const inDBControl );
        PARSER_ERROR apply6_40( TDBControl* const inDBControl );
        PARSER_ERROR apply6_41( TDBControl* const inDBControl );
        PARSER_ERROR apply6_42( TDBControl* const inDBControl );
        PARSER_ERROR apply6_43( TDBControl* const inDBControl );
        PARSER_ERROR apply6_44( TDBControl* const inDBControl );
        PARSER_ERROR apply6_45( TDBControl* const inDBControl );
        PARSER_ERROR apply6_46( TDBControl* const inDBControl );
        PARSER_ERROR apply6_47( TDBControl* const inDBControl );
        PARSER_ERROR apply6_48( TDBControl* const inDBControl );
        PARSER_ERROR apply6_49( TDBControl* const inDBControl );
        PARSER_ERROR apply6_50( TDBControl* const inDBControl );
        PARSER_ERROR apply6_51( TDBControl* const inDBControl );
        PARSER_ERROR apply6_52( TDBControl* const inDBControl );
        PARSER_ERROR apply6_53( TDBControl* const inDBControl );
        TDBControl* _dbControl;

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Tool Queries
        std::string emptyTableQuery( std::string inName );

        std::string checkWhetherATableExistsQuery();
        std::string checkWhetherAGeneratorExistsQuery();
        std::string checkWhetherAnIndexExistsQuery();
        std::string checkWhetherAConstraintExistsQuery();
        std::string checkWhetherAFieldExistsQuery();
        std::string checkWhetherAProcedureExistsQuery();
        std::string checkWhetherATriggerExistsQuery();

        std::string activeIndexQuery(   std::string inIndexName );
        std::string inactiveIndexQuery( std::string inIndexName );

        std::string versionNumberQuery();
        std::string insertVersionNumberQuery( std::string inVersion );
        std::string updateVersionNumberQuery( std::string inVersion );

        std::string createGeneratorQuery(     std::string inName );
        std::string setGeneratorToZeroQuery(  std::string inName );

        std::string contactPointsQuery(            __int32 inContactKey );
        std::string contactTransactionPointsQuery( __int32 inContactKey );
        std::string adjustContactTransactionPointsQuery(
                        __int32 inKey,
                        __int32 inContactKey,
                        Currency inPoints );

        std::string readContactsQuery();
        std::string getForeignKeysListForTableQuery( std::string inTableName );
        std::string checkWhetherADomainExistsQuery();

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Tool Functions
        bool tableExists(
                std::string inTableName,
                TDBControl* inDBControl );

        bool generatorExists(
                std::string inGeneratorName,
                TDBControl* inDBControl );

        bool indexExists(
                std::string inIndexName,
                TDBControl* inDBControl );

        bool constraintExists(
                std::string inConstraintName,
                TDBControl* inDBControl );

        bool fieldExists(
                std::string inTableName,
                std::string inFieldName,
                TDBControl* inDBControl );

        bool procedureExists(
                         std::string  inProcedureName,
                Database::TDBControl* inDBControl );

        bool triggerExists(
                        std::string inTriggerName,
                        TDBControl* inDBControl );

        bool dbObjectExists(
                std::string inName,
                std::string inSQL,
                TDBControl* inDBControl );

        bool dbObjectExists(
                                std::string inNameOne,
                                std::string inNameTwo,
                                std::string inSQL,
                                TDBControl* inDBControl );

        bool DomainExists(std::string inDomainName, TDBControl* inDBControl );

        //.....................................................

        bool updateAlreadyApplied(
                        const std::string inVersion,
                        TDBControl* const inDBControl );

        bool versionNumberAlreadyUpdated(
                const std::string inVersion,
                TDBControl* const inDBControl );

        void updateVersionNumber(
                const std::string inVersion,
                TDBControl* const inDBControl );

        void executeQuery(
                          const std::string inSQL,
                Database::TDBControl* const inDBControl );

        void createTables( TDBControl* const inDBControl );
        void updateTables( TDBControl* const inDBControl );

        //.....................................................

        void readKeys(
                    const std::string inSQL,
                std::vector<__int32>& inKeyVector,
                          TDBControl* const inDBControl );

        void readContacts(
                std::vector<__int32>& inContactVector,
                    TDBControl* const inDBControl );

        void getForeignKeysListForTable(
                    const std::string inSQL,
                std::vector<UnicodeString>& inFKeysList,
                          TDBControl* const inDBControl );

        //.....................................................

        Currency getPoints(
                    const std::string inSQL,
                    TDBControl* const inDBControl );

        Currency getContactPoints(
                              __int32 inContactKey,
                    TDBControl* const inDBControl );

        Currency getContactTransactionPoints(
                              __int32 inContactKey,
                    TDBControl* const inDBControl );

        void adjustContactTransactionPoints(
                              __int32 inContactKey,
                              Currency inPoints,
                    TDBControl* const inDBControl );

        std::vector<TPointsTransaction> getHeldPointsTransactions(
                                                  __int32 inContactKey,
                                        TDBControl* const inDBControl );

        void reEncryptMiscDataForPointsTransactions(
                                        TDBControl* const inDBControl,
                                        std::vector<TPointsTransaction> transactionList);

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        //  5.18.
        void upgrade5_18Tables();
        void update5_18Tables();
        void updateOrdersTable5_18( TDBControl* const inDBControl );

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        //  5.19.
        struct TOptionForcedMask
        {
            __int32 optionKey;
            __int32 forcedMask;
        };

        void upgrade5_19Tables();
        void create5_19Tables();
        void createForcedOptionsTable(     TDBControl* const inDBControl );
        void createForcedOptionsGenerator( TDBControl* const inDBControl );
        void populateTableForcedOptions(   TDBControl* const inDBControl );
        void updateCourseIDsForcedOptions( TDBControl* const inDBControl );
        void updateItemIDsForcedOptions(   TDBControl* const inDBControl );
        void updateItemIDQuery(
				TDBTransaction* const inTransaction,
						UnicodeString inTableName,
							  __int32 inItemKey,
							  __int32 inItemID );
        void readCourses(
                TDBTransaction* const inTransaction,
                std::vector<__int32>& inCourses );
        void populateTableForcedOptionsWithCourse(
                              __int32 inCourse,
                TDBTransaction* const inTransaction );
        void readItems(
                TDBTransaction* const inTransaction,
                              __int32 inCourse,
                std::vector<__int32>& inItems );
        void readOptionForcedMasks(
                TDBTransaction* const inTransaction,
                              __int32 inCourse,
                std::vector<TOptionForcedMask>& inItems );
        void addFieldForcedOptionItems(
                    TOptionForcedMask inOptionForcedMask,
                 std::vector<__int32> inItems,
                TDBTransaction* const inTransaction );
        void addFieldForcedOptionItem(
                              __int32 inOption,
                              __int32 inItem,
                TDBTransaction* const inTransaction );

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        //  5.20.
        void upgrade5_20Tables();
        void update5_20Tables();
        void updatePointsTransactionTable5_20( TDBControl* const inDBControl );
        void syncPointsAdjustments5_20(        TDBControl* const inDBControl );
        void syncPointsAdjustment5_20(
                          __int32 inContactKey,
                TDBControl* const inDBControl );
        void fixEncryptionDataForHeldTransactions5_20(
                                  __int32 inContactKey,
                        TDBControl* const inDBControl );
        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        //  5.21.
        void upgrade5_21Tables();
        void update5_21Tables();
        void update5_21Tables( TDBControl* const inDBControl );
        void create_new_fields_if_not_present( TDBControl* const inDBControl);
        bool are_fields_present_5_21( TDBControl* const inDBControl );

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        //  5.22.
        void upgrade5_22Tables();
        void update5_22Tables();
        void updateItemSizeTable5_22( TDBControl* const inDBControl );
        void insert_new_fields(       TDBControl* const inDBControl );
        bool are_fields_present_5_22( TDBControl* const inDBControl );

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        //  5.23.
        void upgrade5_23Tables();
        void create5_23Tables();
        void update5_23Tables();
        void create5_23DiscountCategoriesTable( TDBControl* const inDBControl );
        void create5_23DiscountCategoriesGenerator( TDBControl* const inDBControl );
        void update5_23DiscountsTable( TDBControl* const inDBControl );

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        //  5.24.
        void upgrade5_24Tables();
        void update5_24Tables();
        void dropAndCreateForcedOptionsConstraints( TDBControl* const inDBControl );

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        //  5.25.
        void upgrade5_25Tables();
        void create5_25Tables();
        void create5_25Procedures();
        void create5_25Views();
        void create5_25RoundingTimesTable(     TDBControl* const inDBControl );
        void populate5_25RoundingTimesTable(   TDBControl* const inDBControl );
        void create5_25RoundTimeProcedure(     TDBControl* const inDBControl );
        void create5_25RoundedContactTimeView( TDBControl* const inDBControl );

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		//  5.26.
		void upgrade5_26Tables();
		void update5_26Tables();
		void updateOrdersTable5_26( TDBControl* const inDBControl );

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        //  5.27.
        void upgrade5_27Tables();
        void update5_27Tables();
        void create5_27Generators( TDBControl* const inDBControl );
        void create5_27Tables( TDBControl* const inDBControl );
		void alterColumnsForArchiveTables5_27( TDBControl* const inDBControl );
    	//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        //  5.28.
        void upgrade5_28Tables();
        void update5_28Tables(TDBControl* const inDBControl);

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        //  5.29.
        void upgrade5_29Tables();
        void update5_29Tables();
        void create5_29Generators( TDBControl* const inDBControl );
        void create5_29Tables( TDBControl* const inDBControl );
        void alterLoyaltyAttributesTable( TDBControl* const inDBControl );

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        //  5.30.
        void upgrade5_30Tables();
        void update5_30Tables();
        void alterRoundTimeProcedure( TDBControl* const inDBControl );

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		//  5.31.
        void upgrade5_31Tables();
        void update5_31Tables();
        void create5_31Generators( TDBControl* const inDBControl );
        void create5_31Tables( TDBControl* const inDBControl );

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		//  5.32.
        void upgrade5_32Tables();
        void update5_32Tables();
        void create5_32Generators( TDBControl* const inDBControl );
        void create5_32Tables( TDBControl* const inDBControl );

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		//  5.33.
        void upgrade5_33Tables();
        void update5_33Tables();
        void create5_33Generators( TDBControl* const inDBControl );
        void create5_33Tables( TDBControl* const inDBControl );

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        //  5.34.
        void upgrade5_34Tables();
        void alter5_34Procedures();
        void alter5_34RoundTimeProcedure( TDBControl* const inDBControl );

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        //  5.35.
        void upgrade5_35Tables();
        void update5_35Tables();
        void clearLoyaltyPendingTransactions5_35( TDBControl* const inDBControl );
        void updateLoyaltyPendingTransactions5_35( TDBControl* const inDBControl );
        void updateLoyaltyAttributes5_35( TDBControl* const inDBControl );

		//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		//  5.36.
        void upgrade5_36Tables();
        void update5_36Tables();
        void update5_36Indexs();
        void addColumnsToLoyaltyAttributes5_36( TDBControl* const inDBControl );
        void addIndexToItemSizePLU5_36( TDBControl* const inDBControl );

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		//  5.37.
        void upgrade5_37Tables();
        void update5_37Tables();
        void modifyContactFreebieColumns5_37( TDBControl* const inDBControl );

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        //  5.38.
        void upgrade5_38Tables();
        void update5_38Tables();
        void modifyContactsColumns5_38( TDBControl* const inDBControl );

        //  6.00.
        void upgrade6_00Tables();
        void update6_00Tables();

        //  6.01.
        void upgrade6_01Tables();
        void update6_01Tables();

        //6.02
        void upgrade6_02Tables();

        //6.03
        void upgrade6_03Tables();

        void create6_00Generators( TDBControl* const inDBControl );
        void create6_00Tables( TDBControl* const inDBControl );
        void create6_00TablesForTaxProfiles( TDBControl* const inDBControl );
        void create6_00TablesForPaymentGroups( TDBControl* const inDBControl );
        void create6_00TablesForDiscountGroups( TDBControl* const inDBControl );
        void create6_00TablesForTaxCalculator( TDBControl* const inDBControl );
        void create6_00TablesForMallExport( TDBControl* const inDBControl );
        void create6_00TableForTablePartronCount(TDBControl* const inDBControl );
        void create6_00MallViews( TDBControl* const inDBControl );
        void populateTablePaymentGroups6_00( TDBControl* const inDBControl );
        void populateTableMallExport6_00( TDBControl* const inDBControl );
        void populateTableDiscountGroups6_00( TDBControl* const inDBControl );
        void updateArcBillPayTable6_00( TDBControl* const inDBControl );
        void updateDayArcBillPayTable6_00( TDBControl* const inDBControl );
        void updateOrdersTable6_00( TDBControl* const inDBControl );
        void updateZedsTable6_00( TDBControl* const inDBControl );
        void updateDiscountGroups_DiscountTypesTable6_00( TDBControl* const inDBControl );
        void updatePaymentAccessToContacts6_00( TDBControl* const inDBControl );
        void updateChitTable6_00( TDBControl* const inDBControl );
        void updateDayArcDiscountTable6_00( TDBControl* const inDBControl );
        void updateArcDiscountTable6_00( TDBControl* const inDBControl );
        void updateChitTableData6_00(TDBControl * const inDBControl);
        void updateTaxRemovalToContacts6_00( TDBControl* const inDBControl );
        void update6_00TablesForDiscountGroups( TDBControl* const inDBControl );
        void UpdatePaymentAccessToContacts6_00( TDBControl* const indbControl );
        void updateCHITTable6_00( TDBControl* const indbControl );
        void UpdateTaxRemovalToContacts6_00(TDBControl* const indbControl);
        /*void CREATEDSRPIVOTProcedure6_00( TDBControl* const inDBControl );
        void POPULATEDSRPIVOTProcedure6_00( TDBControl* const inDBControl );*/ //Now moved to 6.37
        void create6_00TablesForMallExportHourly( TDBControl* const inDBControl );
        void migrateData6_00();
        std::map<Currency, int> migrateTaxProfiles6_00( TDBTransaction* const inDBTransaction );
        void migrateItemSizes6_00( TDBTransaction* const inDBTransaction, std::map<Currency, int> inTaxProfileKeys );
        void migrateOrders6_00( TDBTransaction* const inDBTransaction, std::map<Currency, int> inTaxProfileKeys );
        std::vector<int> orderKeysWithTaxPercentage(Currency inTaxPercentage, TDBTransaction* const inDBTransaction ) ;
        void saveKeysToTaxProfileOrdersTable(
        std::vector<int> inOrdersKeys,
        int inTaxProfilesKey,
        TDBTransaction* const inDBTransaction );
        void migrateArchiveTaxes6_00( TDBTransaction* const inDBTransaction, TArchiveType inArchiveType );
        std::vector<AnsiString> buildItemSizeMigrationQueries(  TDBTransaction* const inDBTransaction, std::map<Currency, int> &inTaxProfileKeys );
        void loadItemSizeTPMigrationQueriesForTaxVal(TDBTransaction* const inDBTransaction, std::vector<AnsiString> &isTPMigrationsList, Currency taxValue, int taxProfileKey );
        void updateDayArchiveTable6_00(TDBControl* const inDBControl);
        void updateArchiveTable6_00(TDBControl* const inDBControl);
        void updateOrdersTableForBasePrice6_00(TDBControl* const inDBControl);
        void PopulateDiscountWithOutTaxAndTaxOnDiscountColumn6_00(TDBControl* const inDBControl);
        void UpdateContactTable6_00(TDBControl* const inDBControl);
        void create6_00TablesForTierLevels(TDBControl* const inDBControl);
        void UpdateRecipeTable6_00();
        void CreateRoundTimeProcedure6_00(TDBControl* const inDBControl);

        void updateTableDiscountGroups6_01(TDBControl* const inDBControl);
        void PopulateMemberTypeForContacts6_01(TDBControl* const inDBControl);
        void UpdateTierLabelTable6_01(TDBControl* const inDBControl);
        void UpdateContactTable6_01(TDBControl* const inDBControl);
        void UpdateLoyaltPendingTransactionTable6_01(TDBControl* const inDBControl);

        void UpdateWriteOffTable6_03(TDBControl* const inDBControl);
        void CreateTableTablesStatus6_03(TDBControl* const inDBControl);
        void UpdateDayArcbillTable6_03(TDBControl* const inDBControl);
        void UpdateArcbillTable6_03(TDBControl* const inDBControl);

        //6.04
        void upgrade6_04Tables();
        void update6_04Tables();
        void ModifyContactsColumns6_04(TDBControl* const inDBControl);
        void UpdateLocationColumns6_04(TDBControl* const inDBControl);
        //Following two functions now moved to 6.37
        /*void CREATEDSR_PIVOT_BY_ITEMProcedure6_04( TDBControl* const inDBControl );
        void POPULATEDSR_PIVOT_BY_ITEMProcedure6_04( TDBControl* const inDBControl ); */

        //6.05
        void upgrade6_05Tables();
        void update6_05Tables();
        void UpdateTierLevelTable6_05(TDBControl* const inDBControl);
        void ModifyTierLevelTable6_05(TDBControl* const inDBControl);
        void UpdateLocationColumns6_05(TDBControl* const inDBControl);
        void create6_05TablesForMallExportTransaction(TDBControl* const inDBControl);

        //6.06
        void upgrade6_06Tables();
        void update6_06Tables();
        void UpdateOrdersTable6_06(TDBControl* const inDBControl);
        void ModifyCloseZedColumns6_06( TDBControl* const inDBControl );
        void PopulateEmailStatusForZeds6_06(TDBControl* const inDBControl);
        void CreateCancelItemsTable6_06(TDBControl* const inDBControl);
        void create6_06Generators( TDBControl* const inDBControl);

        //6.07
        void upgrade6_07Tables();
        void update6_07Tables();
        void UpdateChitNumberTable6_07(TDBControl* const inDBControl);
        void CreateChitTaxRelationTable6_07(TDBControl* const inDBControl);
        void UpdateItemSizeTable6_07(TDBControl* const inDBControl);
        void UpdateOrderTable6_07(TDBControl* const inDBControl);
        void ModifyPaymentTypesColumns6_07(TDBControl* const inDBControl);

        //6.08
        void upgrade6_08Tables();
        void update6_08Tables();
        void ModifyChitNumberTable6_08( TDBControl* const inDBControl );
        void PopulateAddMinutesForChitNumber6_08(TDBControl* const inDBControl);
        void CreateChitDiscountRelationTable6_08(TDBControl* const inDBControl);
        void UpdateTabHistoryTable6_08(TDBControl* const inDBControl);
        void UpdateTabTable6_08(TDBControl* const inDBControl);

        //6.09
        void upgrade6_09Tables();
        void update6_09Tables();
        void create6_09Generators(TDBControl* const inDBControl);
        bool UpdateOrdersTable6_09(TDBControl* const inDBControl);
        void PopulateOrdersTable6_09(TDBControl* const inDBControl);
        void UpdateArcbill6_09(TDBControl* const inDBControl);
        void UpdateDayArcbill6_09(TDBControl* const inDBControl);

        //6.10
        void upgrade6_10Tables();
        void update6_10Tables();
        void createGenerators6_10(TDBControl* const inDBControl);
        void CreateTabDiscountTable6_10(TDBControl* const inDBControl);
        void UpdateContacts_Access_Level6_10(TDBControl* const inDBControl);
        void ChangePaymentTypesFieldSize6_10( TDBControl* const inDBControl);
        void CreateResetPointsTable6_10(TDBControl* const inDBControl); //MM 4579

        //6.11
        void upgrade6_11Tables();
        void update6_11Tables();
        void UpdateHeldOrderItems6_11(TDBControl* const inDBControl);
        void UpdateHeldOrderView6_11(TDBControl* const inDBControl);
        void CreateReprintOrderTable6_11(TDBControl* const inDBControl);
        void createGenerators6_11( TDBControl* const inDBControl);
        void UpdateContacts_Access_Level6_11(TDBControl* const inDBControl);
        void create6_11ArcMallExport(TDBControl* const inDBControl);
        void create6_11ArcMallExportHourly(TDBControl* const inDBControl);
        void create6_11ArcMallExportTransaction(TDBControl* const inDBControl);
        void update6_11MallExportHourly( TDBControl* const inDBControl );

        //6.12
        void upgrade6_12Tables();
        void update6_12Tables();
        void UpdatePaymentTypes6_12(TDBControl* const inDBControl);
        void UpdateDayArcBillPay6_12(TDBControl* const inDBControl);
        void UpdateArcBillPay6_12(TDBControl* const inDBControl);
        void update6_12ArcCategories( TDBControl* const inDBControl);
        void upgrade6_13Tables();
        void update6_13Tables();
        void UpdateTabHistoryTable6_13(TDBControl* const inDBControl);

        //6.14
        void upgrade6_14Tables();
        void update6_14Tables();
        void UpdateContacts_Access_Level6_14(TDBControl* const inDBControl);

        //6.15
        void upgrade6_15Tables();
        void update6_15Tables();
        void UpdateOrdersTable6_15(TDBControl* const inDBControl);
        void ModifyStaffHourEnableZedColumns6_15( TDBControl* const inDBControl );
        void PopulateStaffHourEnableForZeds6_15( TDBControl* const inDBControl );

        //6.16
        void upgrade6_16Tables();
        void update6_16Tables();
        void CreateGenerators6_16(TDBControl* const inDBControl);
        void CreateClippTabIntegrationTable(TDBControl* const inDBControl);
        void CreateClippTabPaymentsTable(TDBControl* const inDBControl);
        void CreateClippTabIntegrationForeignKey(TDBControl* const inDBControl);
        void CreateClippTabPaymentsForeignKey(TDBControl* const inDBControl);
        void createDrinkCommandSessionTable(TDBControl* const inDBControl);
        void createSessionTableGenerators(TDBControl* const inDBControl);
        void changeThirdPartyCodeSize(TDBControl* const inDBControl);
        void UpdateChitNumberTableForSaveCustomerAndMemberDetails6_16( TDBControl* const inDBControl );
        void create6_16HappyHourProfiles(TDBControl* const inDBControl);
        void create6_16HappyHourSettings(TDBControl* const inDBControl);
        void create6_16HappyHourDays(TDBControl* const inDBControl);
        void create6_16TerminalHappyHourProfiles(TDBControl* const inDBControl);

        //6.17
        void upgrade6_17Tables();
        void update6_17Tables();
        void CreateGenerators6_17(TDBControl* const inDBControl);
        void create6_17TableForSalesTypeGroups(TDBControl* const inDBControl);
        void populateTableSalesTypeGroups6_17(TDBControl* const inDBControl );
        void updateTableMallExport6_17(TDBControl* const inDBControl);
        void update6_17MallExportHourly(TDBControl* const inDBControl);
        void update6_17ArcMallExport(TDBControl* const inDBControl);
        void update6_17ArcMallExportHourly(TDBControl* const inDBControl);
        void AlterItemSizeTable6_17(TDBControl* const  inDBControl);
        void UpdateItemSizeTable6_17( TDBControl* const  inDBControl);
        void UpdateSizesTable6_17(TDBControl* const inDBControl);
        void AlterSizesTable6_17(TDBControl* const inDBControl);

        //6.18
        void upgrade6_18Tables();
        void update6_18Tables();
        //6.19
        void upgrade6_19Tables();
        void update6_19Tables();
        void UpdateChitNumberTableForPickupAndDeliveryChit6_19( TDBControl* const inDBControl );
        void create6_19MallViews(TDBControl* const inDBControl);

        //6.20
        void upgrade6_20Tables();
        void update6_20Tables();
        void create6_20TableForThorVouchers(TDBControl* const inDBControl);
        void populateTableForThorVouchers6_20(TDBControl* const inDBControl);
        void create6_20TableForThorVoucher_Categories(TDBControl* const inDBControl);
        void create6_20ThorVoucherCategoriesGenerator(TDBControl* const inDBControl);
        void update6_20TablesForOrderDiscounts(TDBControl* const inDBControl);
        void Create6_20PriceLevels(TDBControl* const inDBControl);
        void Alter6_20HappyHour_Profile(TDBControl* const inDBControl);
        void Create6_20PriceLevelItemSize(TDBControl* const inDBControl);
        void Update6_20_New_Tables(TDBControl* const inDBControl);
        void Create6_20_HappyHour_Priority(TDBControl* const inDBControl);
        void Update6_20_HappyHour_Priority(TDBControl* const inDBControl);
        void Update6_20_HappyHour_Profiles(TDBControl* const inDBControl);

        //6.21
        void upgrade6_21Tables();
        void update6_21Tables();
        void Update6_21_OrderTable( TDBControl* const inDBControl );

        //6.22
        void upgrade6_22Tables();
        void update6_22Tables();
        void Insert6_22_PricelevelDefaultValueInVariableTable( TDBControl* const inDBControl );
        void AutoCorrectPricelevel( TDBControl* const inDBControl );
        void UpdateArchive6_22(TDBControl* const inDBControl);
        void UpdateDayArchive6_22(TDBControl* const inDBControl);
        void UpdateOrders6_22(TDBControl* const inDBControl);
        void ModifyItemSizesColumns6_22( TDBControl* const inDBControl );
        void Update6_22_Thorlink_Vouchers( TDBControl* const inDBControl );

        //6.23
        void upgrade6_23Tables();
        void update6_23Tables();
        void create6_23ThorVoucherKeysGenerator(TDBControl* const inDBControl);

		//6.24
		void upgrade6_24Tables();
        void update6_24Tables();
        void UpdateContactsTable6_24( TDBControl* const inDBControl );
        void UpdateLoyaltyTransactionTable6_24( TDBControl* const inDBControl );
        void UpdateTierLevelsTable6_24(TDBControl* const inDBControl);

        //6.25
		void upgrade6_25Tables();
        void update6_25Tables();
		void UpdateDiscountsTable6_25(TDBControl* const inDBControl);

        //6.26
		void upgrade6_26Tables();
        void update6_26Tables();
		void UpdateDiscountsTable6_26(TDBControl* const inDBControl);
        void UpdateLoyaltyTransactionTable6_26(TDBControl* const inDBControl);
        void UpdateArcCateoriesTable6_26(TDBControl* const inDBControl);

        //6.27
        void upgrade6_27Tables();
        void update6_27Tables();
        void UpdatePrnorderTable6_27(TDBControl* const inDBControl);

        //6.28
        void upgrade6_28Tables();
        void update6_28Tables();
		void UpdateItemSizeTable6_28(TDBControl* const inDBControl);
        void create6_28MallViews(TDBControl* const inDBControl);
        void update6_28MallExportHourly(TDBControl* const inDBControl);
        void update6_28ArcMallExportHourly(TDBControl* const inDBControl);
        void create6_28MallExportOtherDetails(TDBControl* const inDBControl);
        void create6_28ArcMallExportOtherDetails(TDBControl* const inDBControl);
        void CreateGenerators6_28(TDBControl* const inDBControl);
		void Create6_28MYOBInvoiceNumberGenerator(TDBControl* const inDBControl);

        //6.29
        void upgrade6_29Tables();
        void update6_29Tables();
        void UpdateSyndCodeTable6_29(TDBControl* const inDBControl);
        void UpdateSyndCodes6_29(TDBControl* const inDBControl);
        void UpdateContactCardsTable6_29(TDBControl* const inDBControl);

        //6.30
        void upgrade6_30Tables();
        void update6_30Tables();
        void Create6_30VoidInvoiceGenerator(TDBControl* const inDBControl);
        void UpdateDayArcBillTable6_30(TDBControl* const inDBControl);
        void UpdateArcBillTable6_30(TDBControl* const inDBControl);
        //6.30
        void upgrade6_31Tables();
        void update6_31Tables();
        void UpdatePaymentTables6_31(TDBControl* const inDBControl);
        void UpdateContactTable6_31(TDBControl* const inDBControl);
        //6.32
		void upgrade6_32Tables();
        void update6_32Tables();
		void UpdateDiscountsTable6_32(TDBControl* const inDBControl);
        //

        void upgrade6_33Tables();
        void update6_33Tables();
		void AlterRoundTimeProcedure6_33( TDBControl* const inDBControl );
        void PopulateZED_StatusForContactTime6_33(TDBControl* const inDBControl);
        void ModifyCloseZedColumns6_33( TDBControl* const inDBControl );
        void ReCreateRoundedContactTimeView6_33( TDBControl* const inDBControl );

        //6.34
        void upgrade6_34Tables();
        void update6_34Tables();
        void Create6_34GeneratorSCDPWDCustomerDetails(TDBControl* const inDBControl);
        void Create6_34TableSCDPWDCustomerDetails(TDBControl* const inDBControl);
        void UpdateZedTable6_34(TDBControl* const inDBControl);
        void Create6_34Malls(TDBControl* const inDBControl);
        void Create6_34MallExportSettings(TDBControl* const inDBControl);
        void Create6_34MallExportSettingsMapping(TDBControl* const inDBControl);
        void Create6_34MallExportSettingsMappingValues(TDBControl* const inDBControl);
        void Create6_34MallExportSales(TDBControl* const inDBControl);
        void Create6_34MallExportSettingValuesAttributes(TDBControl* const inDBControl);
        void Create6_34MallExportHeader(TDBControl* const inDBControl);
        void Create6_34GeneratorMallExportSaleKey(TDBControl* const inDBControl);
        void Insert6_34Malls(TDBControl* const inDBControl);
        void Insert6_34MallExport_Settings(TDBControl* const inDBControl);
        void Insert6_34MallExport_Settings_Mapping(TDBControl* const inDBControl);
        void Insert6_34MallExport_Settings_Values(TDBControl* const inDBControl);
        void Insert6_34Mall_ExportHeader(TDBControl* const inDBControl);
		void CreateGeneratorAndTableForCashDenominations6_34( TDBControl* const inDBControl );

        //6.35
        void upgrade6_35Tables();
        void update6_35Tables();
        void Create6_35MemberSubsGenerator(TDBControl* const inDBControl);
        void Create6_35MemberSubsDetails(TDBControl* const inDBControl);
        void Insert6_35MemberSubsDetails(TDBControl* const inDBControl);
        void UpdateMallExportSettingValuesTable6_35(TDBControl* const inDBControl);
        void Create6_35GeneratorMallExportSettingValues(TDBControl* const inDBControl);
        void UpdateContactIndex_6_35(TDBControl* const inDBControl);

        void upgrade6_36Tables();
        void update6_36Tables();
        void AlterTableRefloat_Skim6_36(TDBControl* const inDBControl);
        void Update6_36TableSCDPWDCustomerDetails(TDBControl* const inDBControl);
        bool CheckDataTypeOfColumn(TDBControl* const inDBControl);

        void upgrade6_37Tables();
        void update6_37Tables();
        void UpdateContacts6_37(TDBControl* const inDBControl);
//        void CREATEDSR_PIVOT_BY_ITEMProcedure6_37( TDBControl* const inDBControl );
//        void POPULATEDSR_PIVOT_BY_ITEMProcedure6_37( TDBControl* const inDBControl );
//        void CREATEDSRPIVOTProcedure6_37( TDBControl* const inDBControl );
//        void POPULATEDSRPIVOTProcedure6_37( TDBControl* const inDBControl );  //All of these now moved to 6.46
        void AlterTable6_37(TDBControl* const inDBControl);

        void upgrade6_38Tables();
        void update6_38Tables();
        void AlterTable_PaymentTypes(TDBControl* const inDBControl);
        void Updatetable_PaymentTypes(TDBControl* const inDBControl);

        void upgrade6_39Tables();
        void update6_39Tables();
        void CreateTable_PaymentAttributes(TDBControl* const inDBControl);
        void PopulateTable_PaymentAttributes(TDBControl* const inDBControl);
        void Updatetable_PaymentProperties(TDBControl* const inDBControl);
        void PopulatePaymentProperties(TDBControl* const inDBControl);
        void CreateTable_PaymentWalletAttributes(TDBControl* const inDBControl);
		void CreateGenerators6_39(TDBControl* const inDBControl);
        void CreateTable6_39MallSalesType(TDBControl* const inDBControl);
        void CreateTable6_39MallSalesTypeItemRelation(TDBControl* const inDBControl);
        void CreateTable6_39MallSalesBySalesType(TDBControl* const inDBControl);
        void Insert6_39Malls(TDBControl* const inDBControl, int mallKey, UnicodeString mallName, UnicodeString isActive);
        void InsertInTo_MallExport_Settings_Mapping(TDBControl* const inDBControl, int settingIds[], int arraySize, int mallIndex);
        int GetMallExportSettingsMappingIndex(TDBControl* const inDBControl);
		void Create6_39SiHotTransNumberGenerator(TDBControl* const inDBControl);

        //6.40
        void upgrade6_40Tables();
        void update6_40Tables();
        void UpdateChargeToAccount(TDBControl* const inDBControl);
        void UpdatePaymentTypesTable(TDBControl* const inDBControl, AnsiString newProperty, int chargeToAccount);
        void UpdatePaymentAttributesTable(TDBControl* const inDBControl, int key, int chargeToAccount, int invoiceInterace);
        void DelFromPaymentAttributesTable(TDBControl* const inDBControl, int property, int key);

        void upgrade6_41Tables();
        void update6_41Tables();
        void AlterTable6_41(TDBControl* const inDBControl);
        void UpdateMallSalesBySalesType(TDBControl* const inDBControl);
        void UpdateTablePatronCountTable(TDBControl* const inDBControl);

        //6.42
        void upgrade6_42Tables();
        void update6_42Tables();
        void Create6_42Generator(TDBControl* const inDBControl);

        void upgrade6_43Tables();
        void update6_43Tables();
        void CreateTable6_43(TDBControl* const inDBControl);
        void Create6_43Generator(TDBControl* const inDBControl);
        void AlterTableDiscount6_43(TDBControl* const inDBControl);
        void AlterTableOrders6_43(TDBControl* const inDBControl);
        void AlterTableTIMECLOCKLOCATIONS6_43(TDBControl* const inDBControl);
        void AlterTableCONTACTTIME6_43(TDBControl* const inDBControl);

        void upgrade6_44Tables();
        void update6_44Tables();
        void InsertIntoMallExportSettings6_44(TDBControl* const inDBControl, int settingKey, UnicodeString fiedlName, UnicodeString controlName,
                                            char isUIRequired);
        void CreateMezzanineAreaTable6_44(TDBControl* const inDBControl);
        void CreateMezzanineSalesTable6_44(TDBControl* const inDBControl);
        int GetMallExportSettingValueKey(TDBControl* const inDBControl);
        void InsertInTo_MallExport_Settings_Values6_44(TDBControl* const inDBControl, int settingId, int mallId);
        void UpdateReprintReceiptToContacts6_01(TDBControl* const inDBControl);
        void upgrade6_45Tables();
        void update6_45Tables();
        void AlterDayArcBillTable6_45(TDBControl* const inDBControl);
        void AlterArcBillTable6_45(TDBControl* const inDBControl);
        void upgrade6_46Tables();
        void update6_46Tables();
        void CREATEDSR_PIVOT_BY_ITEMProcedure6_46( TDBControl* const inDBControl );
        void POPULATEDSR_PIVOT_BY_ITEMProcedure6_46( TDBControl* const inDBControl );
        void CREATEDSRPIVOTProcedure6_46( TDBControl* const inDBControl );
        void POPULATEDSRPIVOTProcedure6_46( TDBControl* const inDBControl );
		void Create6_46Generators(TDBControl* const _dbControl);
        void AlterTablePaymentType6_46(TDBControl* const _dbControl);
        void Updatetable_PaymentTypes6_46(TDBControl* const _dbControl);
        void UpdateItemSize(TDBControl* const _dbControl);
        void UpdateRevenueCodes(TDBControl* const _dbControl);
        void UpdateServingTimes(TDBControl* const _dbControl);
        void upgrade6_47Tables();
        void update6_47Tables();
        void AlterTableDiscount6_47(TDBControl* const _dbControl);
        void upgrade6_48Tables();
        void update6_48Tables();
        void Create6_48Generator(TDBControl* const inDBControl);
        void CreateTabPatronCount6_48Table(TDBControl* const inDBControl);

        void upgrade6_49Tables();
        void update6_49Tables();
        void Create6_49Generator(TDBControl* const inDBControl);
        void Create6_49Tables(TDBControl* const inDBControl);
        void Create6_49_DomainNotNull(TDBControl* const inDBControl);
        void Alter6_49_Tables(TDBControl* const inDBControl);
        void UpdateTable6_49Orders(TDBControl* const inDBControl);
        void UpdateTable6_49DayArchive(TDBControl* const inDBControl);
        void UpdateTable6_49Archive(TDBControl* const inDBControl);
		void AlterTable6_49MallExportSales(TDBControl* const inDBControl);

        void upgrade6_50Tables();
        void update6_50Tables();
        void Create6_50Generator(TDBControl* const inDBControl);
        void Alter6_50Tables(TDBControl* const inDBControl);
        void Create6_50Table(TDBControl* const inDBControl);
        void ModifyElectronicsPayments(TDBControl* const inDBControl);
        void PopulateMMPaymentTypes(TDBControl* const inDBControl);
        void PopulateDefaultPaymentType(TDBControl* const inDBControl);

        void upgrade6_51Tables();
        void update6_51Tables();
        void Create6_51Generators(TDBControl* const inDBControl);
        void Create6_51Tables(TDBControl* const inDBControl);

        void upgrade6_52Tables();
        void update6_52Tables();
        void AlterTable6_52MallExport(TDBControl* const inDBControl);

        void upgrade6_53Tables();
        void update6_53Tables();
        void Create6_53Generators(TDBControl* const inDBControl);
        void AlterTableOrders6_53(TDBControl* const inDBControl);
        void AlterTableDayArchive6_53(TDBControl* const inDBControl);
        void AlterTableArchive6_53(TDBControl* const inDBControl);
        void AlterTableItemSizes6_53(TDBControl* const inDBControl);
        void UpdateOrders6_53(TDBControl* const inDBControl);
        void UpdateDayArchive6_53(TDBControl* const inDBControl);
        void UpdateArchive6_53(TDBControl* const inDBControl);
        void AlterTableArcBills6_53(TDBControl* const inDBControl);
        void UpdateTableArcBills6_53(TDBControl* const inDBControl);
        void AlterTableTab6_54(TDBControl* const inDBControl);
        void AlterTableOrders6_54(TDBControl* const inDBControl);
        void AlterTableArchives6_54(TDBControl* const inDBControl);
    };// class
} // namespace

#endif
