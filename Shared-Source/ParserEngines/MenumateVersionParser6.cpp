//---------------------------------------------------------------------------
#pragma hdrstop
#include "MenumateVersionParser.h"
#include <sstream>
#include <Predicates.h>
#include <DateUtils.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//Macro to convert a int to a string
#ifndef SSTR( x )
#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
( std::ostringstream() << std::dec << x ) ).str()
#endif

//---------------------------------------------------------------------------
// MENUMATE VERSION 6's PARSER.
namespace MenumateVersionParser
{
//  6.00.
void TApplyParser::upgrade6_00Tables()
{
    update6_00Tables();
	migrateData6_00();
}
// 6.01.
void TApplyParser::upgrade6_01Tables()
{
    update6_01Tables();
}
//  6.02.
void TApplyParser::upgrade6_02Tables()
{
}
//  6.03.
void TApplyParser::upgrade6_03Tables()
{
    UpdateWriteOffTable6_03(_dbControl);
	CreateTableTablesStatus6_03(_dbControl);
	UpdateDayArcbillTable6_03(_dbControl);
	UpdateArcbillTable6_03(_dbControl);
}
//  6.04.
void TApplyParser::upgrade6_04Tables()
{
    update6_04Tables();
}
//6.05
void TApplyParser::upgrade6_05Tables()
{
	update6_05Tables();
}
//6.06
void TApplyParser::upgrade6_06Tables()
{
	update6_06Tables();
}
//6.07
void TApplyParser::upgrade6_07Tables()
{
	update6_07Tables();
}
//6.08
void TApplyParser::upgrade6_08Tables()
{
	update6_08Tables();
}
//6.09
void TApplyParser::upgrade6_09Tables()
{
	update6_09Tables();
}

//::::::::::::::::::::::::Version 6.00::::::::::::::::::::::::::::::::::::::::::
///-------------------------------------------------------------------
void TApplyParser::update6_00Tables()
{
    create6_00Generators( _dbControl );
	create6_00Tables( _dbControl );
	create6_00MallViews( _dbControl );
	updateArcBillPayTable6_00( _dbControl );
	updateDayArcBillPayTable6_00( _dbControl );
	updateOrdersTable6_00( _dbControl );
	updateZedsTable6_00( _dbControl );
	update6_00TablesForDiscountGroups( _dbControl );
	populateTablePaymentGroups6_00( _dbControl );
	populateTableMallExport6_00( _dbControl );
	populateTableDiscountGroups6_00( _dbControl );
	updateDiscountGroups_DiscountTypesTable6_00( _dbControl );
	updatePaymentAccessToContacts6_00( _dbControl );
	updateTaxRemovalToContacts6_00( _dbControl );
	updateChitTable6_00( _dbControl );
	updateDayArcDiscountTable6_00( _dbControl );
	updateArcDiscountTable6_00( _dbControl );
	updateDayArchiveTable6_00(_dbControl);
	updateArchiveTable6_00(_dbControl);
	updateOrdersTableForBasePrice6_00(_dbControl);
	PopulateDiscountWithOutTaxAndTaxOnDiscountColumn6_00(_dbControl);
	UpdateContactTable6_00(_dbControl);
	UpdateRecipeTable6_00();
	CreateRoundTimeProcedure6_00(_dbControl);
}
//---------------------------------------------------------------------------
void TApplyParser::create6_00Generators( TDBControl* const inDBControl)
{
if( !generatorExists("GEN_TAXPROFILES", _dbControl) )
	{
		executeQuery(
		"CREATE GENERATOR GEN_TAXPROFILES;",
		inDBControl);
		executeQuery(
		"SET GENERATOR GEN_TAXPROFILES TO 0;",
		inDBControl );
	}
	if( !generatorExists("GEN_TAXPROFILES_ITEMSIZE", _dbControl) )
	{
		executeQuery(
		"CREATE GENERATOR GEN_TAXPROFILES_ITEMSIZE;",
		inDBControl);
		executeQuery(
		"SET GENERATOR GEN_TAXPROFILES_ITEMSIZE TO 0;",
		inDBControl );
	}
	if( !generatorExists("GEN_TAXPROFILES_ORDERS", _dbControl) )
	{
		executeQuery(
		"CREATE GENERATOR GEN_TAXPROFILES_ORDERS;",
		inDBControl);
		executeQuery(
		"SET GENERATOR GEN_TAXPROFILES_ORDERS TO 0;",
		inDBControl );
	}
	if( !generatorExists("GEN_DAYARCORDERTAXES", _dbControl) )
	{
		executeQuery(
		"CREATE GENERATOR GEN_DAYARCORDERTAXES;",
		inDBControl);
		executeQuery(
		"SET GENERATOR GEN_DAYARCORDERTAXES TO 0;",
		inDBControl );
	}
	if( !generatorExists("GEN_ARCORDERTAXES", _dbControl) )
	{
		executeQuery(
		"CREATE GENERATOR GEN_ARCORDERTAXES;",
		inDBControl);
		executeQuery(
		"SET GENERATOR GEN_ARCORDERTAXES TO 0;",
		inDBControl );
	}
	if( !generatorExists("GEN_PAYMENTGROUPS", _dbControl) )
	{
		executeQuery(
		"CREATE GENERATOR GEN_PAYMENTGROUPS;",
		inDBControl);
		executeQuery(
		"SET GENERATOR GEN_PAYMENTGROUPS TO 0;",
		inDBControl );
	}
	if( !generatorExists("GEN_PAYMENTGROUPS_PAYMENTTYPES", _dbControl) )
	{
		executeQuery(
		"CREATE GENERATOR GEN_PAYMENTGROUPS_PAYMENTTYPES;",
		inDBControl);
		executeQuery(
		"SET GENERATOR GEN_PAYMENTGROUPS_PAYMENTTYPES TO 0;",
		inDBControl );
	}
	//Making the generator for discount groups. Not connected to discout table.
	if(!generatorExists("GEN_DISCOUNT_GROUPS",_dbControl))
	{
		executeQuery(
		"CREATE GENERATOR GEN_DISCOUNT_GROUPS;",inDBControl);
		executeQuery(
		"SET GENERATOR GEN_DISCOUNT_GROUPS TO 0;",inDBControl);
	};
	// Added based on MM-60 on JIRA
	if(!generatorExists("GEN_DISCGROUPS_DISCTYPES", _dbControl))
	{
		executeQuery(
		"CREATE GENERATOR GEN_DISCGROUPS_DISCTYPES;",inDBControl);
		executeQuery(
		"SET GENERATOR GEN_DISCGROUPS_DISCTYPES TO 0;",inDBControl);
	}
	//Making the generator to connect discount groups to discount table.
	if(!generatorExists("GEN_DISCGROUP_DISCOUNT_RELATION",_dbControl))
	{
		executeQuery(
		"CREATE GENERATOR GEN_DISCGROUP_DISCOUNT_RELATION;",inDBControl
		);
		executeQuery(
		"SET GENERATOR GEN_DISCGROUP_DISCOUNT_RELATION TO 0;",inDBControl
		);
	}
	//Generators for redesigned Tax Calculation Setup
	if(!generatorExists("GEN_ORDERSTRANSCNO", _dbControl))
	{
		executeQuery(
		"CREATE GENERATOR GEN_ORDERSTRANSCNO;", inDBControl
		);
		executeQuery(
		"SET GENERATOR GEN_ORDERSTRANSCNO TO 0;", inDBControl
		);
	}
	//Generator for Complimentary invoice Series
	if(!generatorExists("GEN_INVOICENUMBERCOMP", _dbControl))
	{
		executeQuery(
		"CREATE GENERATOR GEN_INVOICENUMBERCOMP;", inDBControl
		);
		executeQuery(
		"SET GENERATOR GEN_INVOICENUMBERCOMP TO 0;", inDBControl
		);
	}
	//Generator for Nonchargable invoice Series
	if(!generatorExists("GEN_INVOICENUMBERNC", _dbControl))
	{
		executeQuery(
		"CREATE GENERATOR GEN_INVOICENUMBERNC;", inDBControl
		);
		executeQuery(
		"SET GENERATOR GEN_INVOICENUMBERNC TO 0;", inDBControl
		);
	}
	//Generators for table patron type.
	if(!generatorExists("GEN_TABLEPATRONCOUNT", _dbControl))
	{
		executeQuery("CREATE GENERATOR GEN_TABLEPATRONCOUNT;", inDBControl);
		executeQuery("SET GENERATOR GEN_TABLEPATRONCOUNT TO 0;", inDBControl);
	}
	if(!generatorExists("GEN_MALLEXPORT_HOURLY_KEY", _dbControl))
	{
		executeQuery(
		"CREATE GENERATOR GEN_MALLEXPORT_HOURLY_KEY;", inDBControl
		);
		executeQuery(
		"SET GENERATOR GEN_MALLEXPORT_HOURLY_KEY TO 0;", inDBControl
		);
	}
	if(!generatorExists("GEN_TIER_LEVELS_ID", _dbControl))
	{
		executeQuery(
		"CREATE GENERATOR GEN_TIER_LEVELS_ID;", inDBControl
		);
		executeQuery(
		"SET GENERATOR GEN_TIER_LEVELS_ID TO 0;", inDBControl
		);
	}
	if( !generatorExists("GEN_LOYALTYMEMBERSHIPID", _dbControl) )
	{
		executeQuery(
		"CREATE GENERATOR GEN_LOYALTYMEMBERSHIPID",
		inDBControl);
		executeQuery(
		"SET GENERATOR GEN_LOYALTYMEMBERSHIPID TO 0",
		inDBControl );
	}
    if(!generatorExists("GEN_MALLEXPORT_TRANSACTION_KEY", _dbControl))
    {
        executeQuery(
            "CREATE GENERATOR GEN_MALLEXPORT_TRANSACTION_KEY;", inDBControl
        );

        executeQuery(
            "SET GENERATOR GEN_MALLEXPORT_TRANSACTION_KEY TO 0;", inDBControl
        );
    }
}
//---------------------------------------------------------------------------
void TApplyParser::create6_00Tables( TDBControl* const inDBControl )
{
    create6_00TablesForTaxProfiles(inDBControl);
	create6_00TablesForPaymentGroups(inDBControl);
	create6_00TablesForDiscountGroups(inDBControl);
	create6_00TablesForTaxCalculator(inDBControl);
	create6_00TablesForMallExport(inDBControl);
	create6_00TableForTablePartronCount(inDBControl);
	create6_00TablesForMallExportHourly(inDBControl);
	create6_00TablesForTierLevels(inDBControl);
}
//---------------------------------------------------------------------------
void TApplyParser::create6_00TablesForTaxProfiles( TDBControl* const inDBControl )
{
	if ( !tableExists( "TAXPROFILES", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE TAXPROFILES "
		"( "
		"   PROFILE_KEY int primary key,"
		"   NAME varchar(50),"
		"   RATE numeric(15,4),"
		"   TYPE int,"
		"   PRIORITY int,"
		"   SELECTABLE char(1)"
		");",
		inDBControl );
	}
	if ( !tableExists( "TAXPROFILES_ITEMSIZE", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE TAXPROFILES_ITEMSIZE "
		"( "
		"   TPIS_KEY int primary key,"
		"   PROFILE_KEY int,"
		"   ITEMSIZE_KEY int,"
		"   foreign key(PROFILE_KEY) references TAXPROFILES(PROFILE_KEY) ON DELETE cascade,"
		"   foreign key(ITEMSIZE_KEY) references ITEMSIZE(ITEMSIZE_KEY) ON DELETE cascade"
		");",
		inDBControl );
	}
	if ( !tableExists( "TAXPROFILES_ORDERS", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE TAXPROFILES_ORDERS "
		"( "
		"   TPO_KEY int primary key,"
		"   PROFILE_KEY int,"
		"   ORDER_KEY int,"
		"   foreign key(PROFILE_KEY) references TAXPROFILES(PROFILE_KEY),"
		"   foreign key(ORDER_KEY) references ORDERS(ORDER_KEY) ON DELETE cascade"
		");",
		inDBControl );
	}
	if ( !tableExists( "DAYARCORDERTAXES", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE DAYARCORDERTAXES "
		"( "
		"   ARCORDERTAXES_KEY int primary key,"
		"   ARCHIVE_KEY int,"
		"   TAX_NAME varchar(50),"
		"   TAX_VALUE numeric(15,4),"
		"   TAX_TYPE int,"
		"   foreign key(ARCHIVE_KEY) references DAYARCHIVE(ARCHIVE_KEY) ON DELETE cascade"
		");",
		inDBControl );
	}
	if ( !tableExists( "ARCORDERTAXES", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE ARCORDERTAXES "
		"( "
		"   ARCORDERTAXES_KEY int primary key,"
		"   ARCHIVE_KEY int,"
		"   TAX_NAME varchar(50),"
		"   TAX_VALUE numeric(15,4),"
		"   TAX_TYPE int,"
		"   foreign key(ARCHIVE_KEY) references ARCHIVE(ARCHIVE_KEY) ON DELETE cascade"
		");",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::create6_00TablesForPaymentGroups( TDBControl* const inDBControl )
{    if ( !tableExists( "PAYMENTGROUPS", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE PAYMENTGROUPS "
		"( "
		"   PAYMENTGROUPS_KEY int primary key,"
		"   PAYMENTGROUP_NAME varchar(25) not null,"
		"   DEFAULT_GROUP T_TRUEFALSE default 'F'"
		");",
		inDBControl );
	}
	if ( !tableExists( "PAYMENTGROUPS_PAYMENTTYPES", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE PAYMENTGROUPS_PAYMENTTYPES "
		"( "
		"   PGPT_KEY int primary key,"
		"   PAYMENTGROUPS_KEY int,"
		"   PAYMENT_KEY int,"
		"   foreign key(PAYMENTGROUPS_KEY) references PAYMENTGROUPS(PAYMENTGROUPS_KEY) ON DELETE cascade,"
		"   foreign key(PAYMENT_KEY) references PAYMENTTYPES(PAYMENT_KEY) ON DELETE cascade"
		");",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::create6_00TablesForDiscountGroups( TDBControl* const inDBControl )
{    // Modified based on MM-60 on JIRA
	if ( !tableExists( "DISCOUNT_GROUPS", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE DISCOUNT_GROUPS "
		"( "
		"   DISCOUNTGROUPS_KEY int primary key,"
		"   DISCOUNTGROUP_NAME varchar(25),"
		"   DEFAULT_GROUP T_TRUEFALSE default 'F'"
		");",
		inDBControl );
	}
	// Added based on MM-60 on JIRA
	if( !tableExists( "DISCOUNTGROUPS_DISCOUNTTYPES", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE DISCOUNTGROUPS_DISCOUNTTYPES "
		"( "
		"   DGDT_KEY int primary key,"
		"   DISCOUNTTYPE_KEY int,"
		"   DISCOUNTGROUPS_KEY int,"
		"   foreign key(DISCOUNTTYPE_KEY) references DISCOUNTS(DISCOUNT_KEY) ON DELETE CASCADE,"
		"   foreign key(DISCOUNTGROUPS_KEY) references DISCOUNT_GROUPS(DISCOUNTGROUPS_KEY) ON DELETE CASCADE"
		");",
		inDBControl );
	}
	// If this is the same as the DISCOUNTGROUPS_DISCOUNTTYPES, this should be removed
	// since MM-60 on JIRA is updated
	if ( !tableExists( "DISCGROUP_DISCOUNT_RELATION", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE DISCGROUP_DISCOUNT_RELATION "
		"( "
		"   DDG_KEY int primary key,"
		"   DISCOUNT_KEY int,"
		"   DISCOUNTGROUPS_KEY int,"
		"   foreign key(DISCOUNT_KEY) references DISCOUNTS(DISCOUNT_KEY),"
		"   foreign key(DISCOUNTGROUPS_KEY) references DISCOUNT_GROUPS(DISCOUNTGROUPS_KEY)"
		");",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::create6_00TablesForTaxCalculator( TDBControl* const inDBControl )
{    if(!tableExists("ORDERSTRANSC" , _dbControl))
	{
		executeQuery(
		"CREATE TABLE ORDERSTRANSC "
		"( "
		"   p_key       INTEGER NOT NULL PRIMARY KEY, "
		"   transc_no   INTEGER NOT NULL,  "
		"   date_time   TIMESTAMP NOT NULL "
		"); ",
		inDBControl);
	}
	if(!tableExists("PAYMENTTYPETRANSC", _dbControl))
	{
		executeQuery(
		"CREATE TABLE PAYMENTTYPETRANSC "
		"( "
		"   p_key           INTEGER NOT NULL PRIMARY KEY, "
		"   payment_name    VARCHAR(30) NOT NULL, "
		"   properties      NUMERIC(15,4) NOT NULL, "
		"   exchange_rate   NUMERIC(15,4) NOT NULL, "
		"   display_order   INTEGER NOT NULL, "
		"   percent_adjust  NUMERIC(15,4) NOT NULL, "
		"   amount_adjust   NUMERIC(15,4) NOT NULL, "
		"   adjust_reason   VARCHAR(30) NOT NULL, "
		"   group_number    INTEGER NOT NULL, "
		"   taxt_rate       NUMERIC(15,4) NOT NULL, "
		"   round_to        NUMERIC(15,4) NOT NULL "
		"); ",
		inDBControl);
		executeQuery(
		"CREATE UNIQUE INDEX PAYMENT_TYPE_TRANSC_GROUP_INDEX ON PAYMENTTYPETRANSC (group_number);",
		inDBControl);
	}
	if(!tableExists("ORDERPAYMENTTYPETRANSC", _dbControl))
	{
		executeQuery(
		"CREATE TABLE ORDERPAYMENTTYPETRANSC "
		"( "
		"   p_key                   INTEGER NOT NULL PRIMARY KEY, "
		"   order_payment_type_fk   INTEGER NOT NULL, "
		"   orders_fk               INTEGER NOT NULL, "
		"   FOREIGN KEY (order_payment_type_fk) REFERENCES PAYMENTTYPETRANSC(p_key) ON DELETE CASCADE ON UPDATE CASCADE, "
		"   FOREIGN KEY (orders_fk)             REFERENCES ORDERSTRANSC(p_key) ON DELETE CASCADE ON UPDATE CASCADE "
		"); ",
		inDBControl);
	}
	if(!tableExists("ORDERITEMSTRANSC", _dbControl))
	{
		executeQuery(
		"CREATE TABLE ORDERITEMSTRANSC "
		"("
		"   p_key               INTEGER NOT NULL PRIMARY KEY, "
		"   menu_price          NUMERIC(15,4) NOT NULL, "
		"   sale_price          NUMERIC(15,4) NOT NULL, "
		"   tax_inclusive       Char(1) DEFAULT 'F', "
		"   tax                 NUMERIC(15,4) NOT NULL, "
		"   tax_exempt          Char(1) DEFAULT 'F', "
		"   zero_tax_rated      Char(1) DEFAULT 'F', "
		"   service_charge      NUMERIC(15,4) NOT NULL, "
		"   service_charge_tax  NUMERIC(15,4) NOT NULL, "
		"   is_free             Char(1) DEFAULT 'F', "
		"   orders_fk           INTEGER NOT NULL, "
		"   FOREIGN KEY (orders_fk) REFERENCES ORDERSTRANSC(p_key) ON DELETE CASCADE ON UPDATE CASCADE "
		");",
		inDBControl);
	}
	if(!tableExists("ORDERSIDESTRANSC", _dbControl))
	{
		executeQuery(
		"CREATE TABLE ORDERSIDESTRANSC "
		"( "
		"   p_key               INTEGER NOT NULL PRIMARY KEY, "
		"   menu_price          NUMERIC(15,4) NOT NULL, "
		"   sale_price          NUMERIC(15,4) NOT NULL, "
		"   tax_inclusive       Char(1)  DEFAULT 'F', "
		"   tax                 NUMERIC(15,4) NOT NULL, "
		"   tax_exempt          Char(1) DEFAULT 'F', "
		"   zero_tax_rated      Char(1) DEFAULT 'F', "
		"   service_charge      NUMERIC(15,4) NOT NULL, "
		"   service_charge_tax  NUMERIC(15,4) NOT NULL, "
		"   is_free             Char(1) DEFAULT 'F', "
		"   order_items_fk      INTEGER NOT NULL, "
		"   FOREIGN KEY (order_items_fk) REFERENCES ORDERITEMSTRANSC(p_key) ON DELETE CASCADE ON UPDATE CASCADE "
		"); ",
		inDBControl);
	}
	if(!tableExists("ORDERITEMSTAXTRANSC", _dbControl))
	{
		executeQuery(
		"CREATE TABLE ORDERITEMSTAXTRANSC "
		"( "
		"   p_key           INTEGER NOT NULL PRIMARY KEY, "
		"   order_tax_fk    INTEGER NOT NULL, "
		"   order_items_fk  INTEGER NOT NULL, "
		"   FOREIGN KEY (order_tax_fk) REFERENCES TAXPROFILES(profile_key) ON DELETE CASCADE ON UPDATE CASCADE, "
		"   FOREIGN KEY (order_items_fk) REFERENCES ORDERITEMSTRANSC(p_key) ON DELETE CASCADE ON UPDATE CASCADE "
		"); ",
		inDBControl);
	}
	if(!tableExists("ORDERSIDESTAXTRANSC", _dbControl))
	{
		executeQuery(
		"CREATE TABLE ORDERSIDESTAXTRANSC "
		"( "
		"   p_key           INTEGER NOT NULL PRIMARY KEY, "
		"   order_tax_fk    INTEGER NOT NULL, "
		"   order_sides_fk  INTEGER NOT NULL, "
		"   FOREIGN KEY (order_tax_fk)   REFERENCES TAXPROFILES(profile_key)    ON DELETE CASCADE ON UPDATE CASCADE, "
		"   FOREIGN KEY (order_sides_fk) REFERENCES ORDERSIDESTRANSC(p_key) ON DELETE CASCADE ON UPDATE CASCADE "
		");",
		inDBControl);
	}
	if(!tableExists("DISCOUNTTRANSC", _dbControl))
	{
		executeQuery(
		"CREATE TABLE DISCOUNTTRANSC "
		"( "
		"   p_key               INTEGER NOT NULL PRIMARY KEY, "
		"   percentage          NUMERIC(15,4) NOT NULL, "
		"   amount              NUMERIC(15,4) NOT NULL, "
		"   discount_type       INTEGER           NOT NULL, "
		"   rounding            NUMERIC(15,4) NOT NULL, "
		"   category_key        INTEGER           NOT NULL, "
		"   properties          NUMERIC(15,4) NOT NULL, "
		"   priority            INTEGER          NOT NULL, "
		"   discount_id         INTEGER          NOT NULL, "
		"   appeareance_order   INTEGER          NOT NULL, "
		"   members_only        Char(1)           DEFAULT 'F', "
		"   discount_group      INTEGER          NOT NULL "
		");",
		inDBControl);
	}
	if(!tableExists("ORDERITEMSDISCOUNTTRANSC", _dbControl))
	{
		executeQuery(
		"CREATE TABLE ORDERITEMSDISCOUNTTRANSC "
		"( "
		"   p_key               INTEGER NOT NULL PRIMARY KEY, "
		"   order_discount_fk   INTEGER NOT NULL, "
		"   order_items_fk      INTEGER NOT NULL, "
		"   FOREIGN KEY (order_discount_fk) REFERENCES DISCOUNTTRANSC(p_key) ON DELETE CASCADE ON UPDATE CASCADE, "
		"   FOREIGN KEY (order_items_fk)     REFERENCES ORDERITEMSTRANSC(p_key) ON DELETE CASCADE ON UPDATE CASCADE "
		");",
		inDBControl);
	}
	if(!tableExists("ORDERSIDESDISCOUNTTRANSC", _dbControl))
	{
		executeQuery(
		"CREATE TABLE ORDERSIDESDISCOUNTTRANSC "
		"("
		"   p_key               INTEGER NOT NULL PRIMARY KEY, "
		"   order_discount_fk   INTEGER NOT NULL, "
		"   order_sides_fk      INTEGER NOT NULL, "
		"   FOREIGN KEY (order_discount_fk) REFERENCES DISCOUNTTRANSC(p_key) ON DELETE CASCADE ON UPDATE CASCADE, "
		"   FOREIGN KEY (order_sides_fk) REFERENCES ORDERSIDESTRANSC(p_key) ON DELETE CASCADE ON UPDATE CASCADE "
		");",
		inDBControl);
	}
}
//---------------------------------------------------------------------------
void TApplyParser::create6_00TablesForMallExport( TDBControl* const inDBControl )
{    if ( !tableExists( "MALLEXPORT", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE MALLEXPORT "
		"( "
		"   MALLEXPORT_KEY INTEGER NOT NULL PRIMARY KEY, "
		"   FIELD_NAME VARCHAR(50) NOT NULL, "
		"   STRING_VALUE VARCHAR(200), "
		"   INTEGER_VALUE INTEGER, "
		"   CURRENCY_VALUE NUMERIC(15,4), "
		"   TIMESTAMP_VALUE TIMESTAMP "
		");",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::create6_00MallViews( TDBControl* const inDBControl )
{    if ( !tableExists( "SM_MALLVIEW", _dbControl ) )
	{
		executeQuery(
		"CREATE VIEW SM_MALLVIEW "
		"("
		"   MALLEXPORT_KEY, "
		"   FIELD_NAME, "
		"   STRING_VALUE, "
		"   INTEGER_VALUE, "
		"   CURRENCY_VALUE, "
		"   TIMESTAMP_VALUE "
		") "
		"AS "
		"SELECT "
		"   MALLEXPORT_KEY, "
		"   FIELD_NAME, "
		"   STRING_VALUE, "
		"   INTEGER_VALUE, "
		"   CURRENCY_VALUE, "
		"   TIMESTAMP_VALUE "
		"FROM "
		"   MALLEXPORT "
		"WHERE "
		"   MALLEXPORT_KEY in ( 01, " // Tenant Number
		"                       02, " // Terminal Number
		"                       04, " // Total Sale Tax
		"                       05, " // Total Cancelled
		"                       07, " // Total Regular Discount
		"                       09, " // Total Refund
		"                       11, " // Total Senior Citizen Discount
		"                       13, " // Total Service Charge
		"                       15, " // Grand Total Old
		"                       16, " // Total End of day
		"                       17, " // Grand Total
		"                       22, " // Charge Credit Sales
		"                       24, " // Tax Exempt Sales
		"                       27, " // Disability Discount
		"                       31, " // Mall Code
		"                       32, " // Class Code
		"                       33, " // Trade Code
		"                       34, " // Outlet Number
		"                       35, " // Sales Type
		"                       36, " // Employee Discount
		"                       37, " // VIP Discount
		"                       38, " // GPC Discount
		"                       39, " // DiscountG1
		"                       40, " // DiscountG2
		"                       41, " // DiscountG3
		"                       42, " // DiscountG4
		"                       43, " // DiscountG5
		"                       44, " // DiscountG6
		"                       45, " // Other Tax
		"                       46, " // Adjustment
		"                       47, " // Positive Adjustment
		"                       48, " // Negative Adjustment
		"                       49, " // Positive Adjustment Non Tax
		"                       50, " // Negative Adjustment Non Tax
		"                       51, " // Daily Sales
		"                       52, " // Tax Inclusive Sales
		"                       53, " // Cash Sales
		"                       54, " // Gift Cheque Sales
		"                       55, " // Debit Card Sales
		"                       56, " // Other Tender
		"                       57, " // Total Master Card
		"                       58, " // Total Visa
		"                       59, " // Total American Express
		"                       60, " // Total Diners
		"                       61, " // Total JCB
		"                       62, " // Total Other Cards
		"                       63, " // Other Charges
		"                       64, " // First Transaction
		"                       65, " // Last Transaction
		"                       66, " // Transaction Count
		"                       67, " // Beginning Invoice Number
		"                       68, " // Ending Invoice Number
		"                       69, " // Cash Transaction
		"                       70, " // Gift Card Checque Transactions
		"                       71, " // Debit Card Transactions
		"                       72, " // Other Tender Transactions
		"                       73, " // Master Card Transactions
		"                       74, " // Visa Transactions
		"                       75, " // American Express Transactions
		"                       76, " // Diners Transactions
		"                       77, " // JCB Transactions
		"                       78, " // Other Card Transactions
		"                       79, " // Serial Number
		"                       80, " // Transaction Time
		"                       81, " // Transaction Date
		"                       89 )", // Department Sum
		inDBControl );
	}
	if ( !tableExists( "ROBINSONS_MALLVIEW", _dbControl ) )
	{
		executeQuery(
		"CREATE VIEW ROBINSONS_MALLVIEW "
		"("
		"   MALLEXPORT_KEY, "
		"   FIELD_NAME, "
		"   STRING_VALUE, "
		"   INTEGER_VALUE, "
		"   CURRENCY_VALUE, "
		"   TIMESTAMP_VALUE "
		") "
		"AS "
		"SELECT "
		"   MALLEXPORT_KEY, "
		"   FIELD_NAME, "
		"   STRING_VALUE, "
		"   INTEGER_VALUE, "
		"   CURRENCY_VALUE, "
		"   TIMESTAMP_VALUE "
		"FROM "
		"   MALLEXPORT "
		"WHERE "
		"   MALLEXPORT_KEY in ( 01, " // Tenant Number
		"                       02, " // Terminal Number
		"                       03, " // Total Gross Sales
		"                       04, " // Total Sale Tax
		"                       05, " // Total Cancelled
		"                       06, " // Total Cancelled Count
		"                       07, " // Total Regular Discount
		"                       08, " // TOtal Regular Discount Count
		"                       09, " // Total Refund
		"                       10, " // Total Refund Count
		"                       11, " // Total Senior Citizen Discount
		"                       12, " // Total Senior Citizen Discount Count
		"                       13, " // Total Service Charge
		"                       14, " // Total End of Day Old
		"                       15, " // Grand Total Old
		"                       16, " // Total End of Day
		"                       17, " // Grand Total
		"                       18, " // Date
		"                       19, " // Novelty Sales
		"                       20, " // Miscellaneus Sales
		"                       21, " // Local Tax
		"                       22, " // Charge Credit Sales
		"                       23, " // Charge Credit Sales Tax
		"                       24, " // Tax Exempt Sales
		"                       25, " // Pharma Sales
		"                       26, " // Non Pharma Sales
		"                       27, " // Disability Discount
		"                       28, " // Sales Not Subjected to rent
		"                       29, " // Total Reprinted Transaction
		"                       30 )", // Total Reprinted Transaction Count
		inDBControl );
	}
	if ( !tableExists( "AYALA_HOURLY_MALLVIEW", _dbControl ) )
	{
		executeQuery(
		"CREATE VIEW AYALA_HOURLY_MALLVIEW "
		"("
		"   MALLEXPORT_KEY, "
		"   FIELD_NAME, "
		"   STRING_VALUE, "
		"   INTEGER_VALUE, "
		"   CURRENCY_VALUE, "
		"   TIMESTAMP_VALUE "
		") "
		"AS "
		"SELECT "
		"   MALLEXPORT_KEY, "
		"   FIELD_NAME, "
		"   STRING_VALUE, "
		"   INTEGER_VALUE, "
		"   CURRENCY_VALUE, "
		"   TIMESTAMP_VALUE "
		"FROM "
		"   MALLEXPORT "
		"WHERE "
		"   MALLEXPORT_KEY in ( 02, " // Terminal Number
		"                       31, " // Mall Code
		"                       51, " // Daily Sales
		"                       66, " // Transaction Count
		"                       81, " // Transaction Date
		"                       82 )", // Hour
		inDBControl );
	}
	if ( !tableExists( "AYALA_ZED_MALLVIEW", _dbControl ) )
	{
		executeQuery(
		"CREATE VIEW AYALA_ZED_MALLVIEW "
		"("
		"   MALLEXPORT_KEY, "
		"   FIELD_NAME, "
		"   STRING_VALUE, "
		"   INTEGER_VALUE, "
		"   CURRENCY_VALUE, "
		"   TIMESTAMP_VALUE "
		") "
		"AS "
		"SELECT "
		"   MALLEXPORT_KEY, "
		"   FIELD_NAME, "
		"   STRING_VALUE, "
		"   INTEGER_VALUE, "
		"   CURRENCY_VALUE, "
		"   TIMESTAMP_VALUE "
		"FROM "
		"   MALLEXPORT "
		"WHERE "
		"   MALLEXPORT_KEY in ( 02, " // Termimnal Number
		"                       03, " // Total Gross Sales
		"                       04, " // Total Sale Tax
		"                       05, " // Total Cancelled
		"                       09, " // Total Refund
		"                       13, " // Total Service Charge
		"                       15, " // Grand Total Old
		"                       17, " // Grand Total
		"                       21, " // Local Taxes
		"                       24, " // Tax Exempt Sales
		"                       31, " // Mall Code
		"                       51, " // Daily Sales
		"                       66, " // Transaction Count
		"                       67, " // Beginning Invoice Number
		"                       68, " // Ending Invoice Number
		"                       81, " // Transaction Date
		"                       83, " // Total Discount
		"                       84, " // Beginning OR Number
		"                       85, " // Ending OR Number
		"                       86, " // Local Tax Exempt Daily Sales
		"                       87 )", // Fine Dining Customer Count
		inDBControl );
	}
	if ( !tableExists( "POWERPLANT_ZED_MALLVIEW", _dbControl ) )
	{
		executeQuery(
		"CREATE VIEW POWERPLANT_ZED_MALLVIEW "
		"("
		"   MALLEXPORT_KEY, "
		"   FIELD_NAME, "
		"   STRING_VALUE, "
		"   INTEGER_VALUE, "
		"   CURRENCY_VALUE, "
		"   TIMESTAMP_VALUE "
		") "
		"AS "
		"SELECT "
		"   MALLEXPORT_KEY, "
		"   FIELD_NAME, "
		"   STRING_VALUE, "
		"   INTEGER_VALUE, "
		"   CURRENCY_VALUE, "
		"   TIMESTAMP_VALUE "
		"FROM "
		"   MALLEXPORT "
		"WHERE "
		"   MALLEXPORT_KEY in ( 01, " // Tenant Number
		"                       03, " // Total Gross Sale
		"                       04, " // Total Sale Tax
		"                       05, " // Total Cancelled
		"                       06, " // Total Cancelled Count
		"                       09, " // Total Refund
		"                       10, " // Total Refund Count
		"                       11, " // Total Senior Citizen Discount
		"                       13, " // Total Service Charge
		"                       31, " // Mall Code
		"                       66, " // Transaction Count
		"                       81, " // Transaction Date
		"                       83 )", // Total Discount
		inDBControl );
	}
	if ( !tableExists( "POWERPLANT_GENERIC_MALLVIEW", _dbControl ) )
	{
		executeQuery(
		"CREATE VIEW POWERPLANT_GENERIC_MALLVIEW "
		"("
		"   MALLEXPORT_KEY, "
		"   FIELD_NAME, "
		"   STRING_VALUE, "
		"   INTEGER_VALUE, "
		"   CURRENCY_VALUE, "
		"   TIMESTAMP_VALUE "
		") "
		"AS "
		"SELECT "
		"   MALLEXPORT_KEY, "
		"   FIELD_NAME, "
		"   STRING_VALUE, "
		"   INTEGER_VALUE, "
		"   CURRENCY_VALUE, "
		"   TIMESTAMP_VALUE "
		"FROM "
		"   MALLEXPORT "
		"WHERE "
		"   MALLEXPORT_KEY in ( 01, " // Tenant Number
		"                       03, " // Total Gross Sale
		"                       04, " // Total Sale Tax
		"                       05, " // Total Cancelled
		"                       06, " // Total Cancelled Count
		"                       09, " // Total Refund
		"                       10, " // Total Refund Count
		"                       11, " // Total Senior Citizen Discount
		"                       13, " // Total Service Charge
		"                       31, " // Mall Code
		"                       66, " // Transaction Count
		"                       80, " // Transaction Time
		"                       81, " // Transaction Date
		"                       83 )", // Total Discount
		inDBControl );
	}
	if ( !tableExists( "POWERPLANT_GT_MALLVIEW", _dbControl ) )
	{
		executeQuery(
		"CREATE VIEW POWERPLANT_GT_MALLVIEW "
		"("
		"   MALLEXPORT_KEY, "
		"   FIELD_NAME, "
		"   STRING_VALUE, "
		"   INTEGER_VALUE, "
		"   CURRENCY_VALUE, "
		"   TIMESTAMP_VALUE "
		") "
		"AS "
		"SELECT "
		"   MALLEXPORT_KEY, "
		"   FIELD_NAME, "
		"   STRING_VALUE, "
		"   INTEGER_VALUE, "
		"   CURRENCY_VALUE, "
		"   TIMESTAMP_VALUE "
		"FROM "
		"   MALLEXPORT "
		"WHERE "
		"   MALLEXPORT_KEY in ( 01, " // Tenant Number
		"                       15, " // Grand Total Old
		"                       17, " // Grand Total
		"                       81 )", // Transaction Date
		inDBControl );
	}
	if ( !tableExists( "CAPITALAND_MALLVIEW", _dbControl ) )
	{
		executeQuery(
		"CREATE VIEW CAPITALAND_MALLVIEW "
		"("
		"   MALLEXPORT_KEY, "
		"   FIELD_NAME, "
		"   STRING_VALUE, "
		"   INTEGER_VALUE, "
		"   CURRENCY_VALUE, "
		"   TIMESTAMP_VALUE "
		") "
		"AS "
		"SELECT "
		"   MALLEXPORT_KEY, "
		"   FIELD_NAME, "
		"   STRING_VALUE, "
		"   INTEGER_VALUE, "
		"   CURRENCY_VALUE, "
		"   TIMESTAMP_VALUE "
		"FROM "
		"   MALLEXPORT "
		"WHERE "
		"   MALLEXPORT_KEY in ( 03, " // Total Gross Sale
		"                       31, " // Mall Code/Branch Code
		"                       81 )", // Transaction Date
		inDBControl );
	}
    if ( !tableExists( "ALPHALAND_ZED_MALLVIEW", _dbControl ) )
	{
		executeQuery(
			"CREATE VIEW ALPHALAND_ZED_MALLVIEW "
			"("
            "   MALLEXPORT_KEY, "
            "   FIELD_NAME, "
            "   STRING_VALUE, "
            "   INTEGER_VALUE, "
            "   CURRENCY_VALUE, "
            "   TIMESTAMP_VALUE "
			") "
			"AS "
			"SELECT "
            "   MALLEXPORT_KEY, "
            "   FIELD_NAME, "
            "   STRING_VALUE, "
            "   INTEGER_VALUE, "
            "   CURRENCY_VALUE, "
            "   TIMESTAMP_VALUE "
			"FROM "
            "   MALLEXPORT "
            "WHERE "
            "   MALLEXPORT_KEY in ( 01, " // Tenant Number
            "                       02, " // Terminal Number
            "                       81, " // Transaction Date
            "                       67, " // Beginning Invoice Number
            "                       68, " // Ending Invoice Number
            "                       03, " // Gross Sale
            "                       04, " // Total Sale Tax (VAT Sales)
            "                       90, " // Total Sale Tax (Non-VAT Sales)
            "                       24, " // Tax Exempt Sales
            "                       05, " // Total Cancelled
            "                       06, " // Total Cancelled Count
            "                       83, " // Total Discounts
            "                       96, " // Total Discounts Count
            "                       09, " // Total Refund
            "                       10, " // Total Refund Count
            "                       11, " // Total Senior Citizen Discount
            "                       12, " // Total Senior Citizen Discount Count
            "                       13, " // Total Service Charge
            "                       97, " // Total Card Sales
            "                       53, " // Total Cash Sales
            "                       91, " // Total Check Sales
            "                       54, " // Total Gift Cheque Sales
            "                       92, " // Total EPay Sales
            "                       98, " // Total Other Sales
            "                       93, " // Total No Sales
            "                       94, " // Previous EOD Counter
            "                       15, " // Grand Total Old
            "                       95, " // EOD Counter
            "                       17, " // Grand Total
            "                       66 )", // Transaction Count
			inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::create6_00TableForTablePartronCount(TDBControl* const inDBControl )
{    if ( !tableExists( "TABLEPATRONCOUNT", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE TABLEPATRONCOUNT "
		"( "
		"PATRONCOUNT_KEY Integer NOT NULL, "
		"TABLE_KEY Integer, "
		"PATRON_TYPE Varchar(40), "
		"PATRON_COUNT Integer, "
		"PRIMARY KEY (PATRONCOUNT_KEY) "
		");", inDBControl);
		executeQuery(
		"ALTER TABLE TABLEPATRONCOUNT ADD CONSTRAINT TABLEPATRONCOUNT_TABLE_KEY "
		"FOREIGN KEY (TABLE_KEY) REFERENCES TABLES (TABLE_KEY) ON UPDATE CASCADE ON DELETE CASCADE;", inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::update6_00TablesForDiscountGroups( TDBControl* const inDBControl )
{   if ( !fieldExists("DISCOUNT_GROUPS", "DISCOUNTGROUP_TYPE", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE DISCOUNT_GROUPS ADD DISCOUNTGROUP_TYPE INTEGER default 0;",
		inDBControl );
	}
	if ( !fieldExists("DISCOUNT_GROUPS", "TAX_EXEMPT", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE DISCOUNT_GROUPS ADD TAX_EXEMPT T_TRUEFALSE default 'F';",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::updateArcBillPayTable6_00( TDBControl* const inDBControl )
{    if ( !fieldExists("ARCBILLPAY", "PAY_GROUP", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE ARCBILLPAY ADD PAY_GROUP varchar(25) default null;",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::updateDayArcBillPayTable6_00( TDBControl* const inDBControl )
{    if ( !fieldExists("DAYARCBILLPAY", "PAY_GROUP", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE DAYARCBILLPAY ADD PAY_GROUP varchar(25) default null;",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::updateOrdersTable6_00( TDBControl* const inDBControl )
{    if ( !fieldExists("ORDERS", "ZED_PRICE", inDBControl )
			&& !fieldExists("ORDERS", "CANCEL_BASE_PRICE", inDBControl )
			&& !fieldExists("ORDERS", "CANCEL_GROSS_PRICE", inDBControl )
			&& !fieldExists("ORDERS", "CANCEL_FINAL_PRICE", inDBControl )
			&& !fieldExists("ORDERS", "CANCEL_TOTAL_TAX", inDBControl )
			&& !fieldExists("ORDERS", "CANCEL_TOTAL_DISCOUNT", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE ORDERS "
		"ADD ZED_PRICE             NUMERIC(15,4) default 0.000,"
		"ADD CANCEL_BASE_PRICE     NUMERIC(15,4) default 0.000,"
		"ADD CANCEL_GROSS_PRICE    NUMERIC(15,4) default 0.000,"
		"ADD CANCEL_FINAL_PRICE    NUMERIC(15,4) default 0.000,"
		"ADD CANCEL_TOTAL_TAX      NUMERIC(15,4) default 0.000,"
		"ADD CANCEL_TOTAL_DISCOUNT NUMERIC(15,4) default 0.000;",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::updateZedsTable6_00( TDBControl* const inDBControl )
{    if ( !fieldExists("ZEDS", "ZED_TOTAL", inDBControl )
			&& !fieldExists("ZEDS", "TERMINAL_EARNINGS", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE ZEDS "
		"ADD TERMINAL_EARNINGS NUMERIC(15,4) default null,"
		"ADD ZED_TOTAL         NUMERIC(15,4) default null;",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::populateTablePaymentGroups6_00( TDBControl* const inDBControl )
{    TDBTransaction transaction( *inDBControl );
    const int NUMBER_OF_FIELDS = 13;

    UnicodeString paymentGroups[NUMBER_OF_FIELDS] = { "AmEx", "VISA", "Master Card", "Diners", "JCB", "Other Card", "Cash Sales", "Gift Card", "Debit Card", "Check Sales", "Card Sales", "EPay Sales", "Other Sales" };

    try
	{
		transaction.StartTransaction();

        TIBSQL *fieldCountQuery = transaction.Query( transaction.AddQuery() );

        fieldCountQuery->SQL->Text = "SELECT COUNT(PAYMENTGROUPS_KEY) FIELD_COUNT FROM PAYMENTGROUPS;";
        fieldCountQuery->ExecQuery();

        for (int i = fieldCountQuery->FieldByName("FIELD_COUNT")->AsInteger; i < NUMBER_OF_FIELDS; i++)
        {
            TIBSQL *keyGeneratorQuery    = transaction.Query( transaction.AddQuery() );

            keyGeneratorQuery->SQL->Text = "SELECT GEN_ID(GEN_PAYMENTGROUPS, 1) FROM RDB$DATABASE";
            keyGeneratorQuery->ExecQuery();

            int key = keyGeneratorQuery->Fields[0]->AsInteger;

            //:::::::::::::::::::::::::::::::::::::::

            TIBSQL *insertQuery    = transaction.Query( transaction.AddQuery() );
            insertQuery->SQL->Text = "INSERT INTO PAYMENTGROUPS "
                                     "( "
                                     "   PAYMENTGROUPS_KEY,"
                                     "   PAYMENTGROUP_NAME,"
                                     "   DEFAULT_GROUP"
                                     ") "
                                     "VALUES "
                                     "( "
                                     "   :PAYMENTGROUPS_KEY,"
                                     "   :PAYMENTGROUP_NAME,"
                                     "   :DEFAULT_GROUP"
                                     ");";

            insertQuery->ParamByName("PAYMENTGROUPS_KEY")->AsInteger = key;
            insertQuery->ParamByName("PAYMENTGROUP_NAME")->AsString  = paymentGroups[i];
            insertQuery->ParamByName("DEFAULT_GROUP"    )->AsString  = "T";

            insertQuery->ExecQuery();
        }

		transaction.Commit();
	}
	catch( ... )
	{
        transaction.Rollback();
        throw;
	}
}
//---------------------------------------------------------------------------
void TApplyParser::populateTableDiscountGroups6_00( TDBControl* const inDBControl )
{    TDBTransaction transaction( *inDBControl );
	const int NUMBER_OF_FIELDS = 13;
	UnicodeString discountGroups[NUMBER_OF_FIELDS] = { "Senior Citizen", "Person with Disability", "VIP", "Employee",
		"GPC", "Discount 1", "Discount 2", "Discount 3", "Discount 4",
		"Discount 5", "Discount 6","Complimentary","Non-Chargeable"};
	int discountGroupTypes[NUMBER_OF_FIELDS] = {0,0,0,0,0,0,0,0,0,0,0,1,2};
	UnicodeString taxExempt[NUMBER_OF_FIELDS] = { "T","F","F","F","F","F","F","F","F","F","F","T","T"};
	try
	{
		transaction.StartTransaction();
		TIBSQL *fieldCountQuery = transaction.Query( transaction.AddQuery() );
		fieldCountQuery->SQL->Text = "SELECT COUNT(DISCOUNTGROUPS_KEY) FIELD_COUNT FROM DISCOUNT_GROUPS;";
		fieldCountQuery->ExecQuery();
		for (int i = fieldCountQuery->FieldByName("FIELD_COUNT")->AsInteger; i < NUMBER_OF_FIELDS; i++)
		{
			TIBSQL *keyGeneratorQuery    = transaction.Query( transaction.AddQuery() );
			keyGeneratorQuery->SQL->Text = "SELECT GEN_ID(GEN_DISCOUNT_GROUPS, 1) FROM RDB$DATABASE";
			keyGeneratorQuery->ExecQuery();
			int key = keyGeneratorQuery->Fields[0]->AsInteger;
			//:::::::::::::::::::::::::::::::::::::::
			TIBSQL *insertQuery    = transaction.Query( transaction.AddQuery() );
			insertQuery->SQL->Text = "INSERT INTO DISCOUNT_GROUPS "
			"( "
			"   DISCOUNTGROUPS_KEY,"
			"   DISCOUNTGROUP_NAME,"
			"   DEFAULT_GROUP,"
			"   DISCOUNTGROUP_TYPE,"
			"   TAX_EXEMPT  "
			") "
			"VALUES "
			"( "
			"   :DISCOUNTGROUPS_KEY,"
			"   :DISCOUNTGROUP_NAME,"
			"   :DEFAULT_GROUP,"
			"   :DISCOUNTGROUP_TYPE,"
			"   :TAX_EXEMPT "
			");";
			insertQuery->ParamByName("DISCOUNTGROUPS_KEY")->AsInteger = key;
			insertQuery->ParamByName("DISCOUNTGROUP_NAME")->AsString  = discountGroups[i];
			insertQuery->ParamByName("DEFAULT_GROUP"    )->AsString  = "T";
			insertQuery->ParamByName("DISCOUNTGROUP_TYPE"    )->AsInteger  = discountGroupTypes[i];
			insertQuery->ParamByName("TAX_EXEMPT"    )->AsString  = taxExempt[i];
			insertQuery->ExecQuery();
		}
		transaction.Commit();
	}
	catch( ... )
	{
		transaction.Rollback();
		throw;
	}
};
//---------------------------------------------------------------------------
void TApplyParser::populateTableMallExport6_00( TDBControl* const inDBControl )
{    enum ExportValueType {STRING, INTEGER, CURRENCY, TIMESTAMP};
    const int NUMBER_OF_FIELDS = 98;

    TDBTransaction transaction( *inDBControl );

    UnicodeString fieldNames[NUMBER_OF_FIELDS] =
        {
            "TenantNo",
            "TerminalNo",
            "TotalGrossSale",
            "TotalSaleTax",
            "TotalCancelled",
            "TotalCancelledCount",
            "TotalRegularDiscount",
            "TotalRegularDiscountCount",
            "TotalRefund",
            "TotalRefundCount",
            "TotalSeniorCitizenDiscount",
            "TotalSeniorCitizenDiscountCount",
            "TotalServiceCharge",
            "TotalEndOfDayOld",
            "GrandTotalOld",
            "TotalEndOfDay",
            "GrandTotal",
            "Date",
            "NoveltySales",
            "MiscellaneusSales",
            "LocalTaxes",
            "ChargeCreditSales",
            "ChargeCreditSalesTax",
            "TaxExemptSales",
            "PharmaSales",
            "NonPharmaSales",
            "DisabilityDiscount",
            "SalesNotSubjectToRent",
            "TotalReprintedTransc",
            "TotalReprintedTranscCount",
            "MallCode",
            "ClassCode",
            "TradeCode",
            "OutletNumber",
            "SalesType",
            "EmployeeDiscount",
            "VIPDiscount",
            "GPCDiscount",
            "DiscountG1",
            "DiscountG2",
            "DiscountG3",
            "DiscountG4",
            "DiscountG5",
            "DiscountG6",
            "OtherTax",
            "Adjustments",
            "PositiveAdjustments",
            "NegativeAdjustments",
            "PositiveAdjustmentsNonTax",
            "NegativeAdjustmentsNonTax",
            "DailySales",
            "TaxInclusiveSales",
            "CashSales",
            "GiftChequeSales",
            "DebitCardSales",
            "OtherTender",
            "TotalMastercard",
            "TotalVisa",
            "TotalAmericanExpress",
            "TotallDiners",
            "TotalJCB",
            "TotalOtherCards",
            "OtherCharges",
            "FirstTransaction",
            "LastTransaction",
            "TransactionCount",
            "BeginningInvoiceNo",
            "EndingInvoiveNo",
            "CashTransactions",
            "GiftCardChequeTransactions",
            "DebitCardTransactions",
            "OtherTenderTransctions",
            "MasterCardTransactions",
            "VisaTransactions",
            "AmericanExpressTransactions",
            "DinersTransactions",
            "JCBTransactions",
            "OtherCardTransactions",
            "SerialNumber",
            "TransactionTime",
            "TransactionDate",
            "Hour",
            "TotalDiscount",
            "BeginningORNo",
            "EndingORNo",
            "LocalTaxExemptDailySales",
            "FineDiningCustomerCount",
            "TenderSurcharges",
            "DepartmentSum",
            "NonVatSales",
            "CheckSales",
            "EPaySales",
            "NoSales",
            "PrevEODCounter",
            "CurrentEODCounter",
            "DiscountCount",
            "CardSales",
            "OtherSales"
        };

    ExportValueType fieldTypes[NUMBER_OF_FIELDS] =
        {
            STRING,
            STRING,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            INTEGER,
            CURRENCY,
            INTEGER,
            CURRENCY,
            INTEGER,
            CURRENCY,
            INTEGER,
            CURRENCY,
            INTEGER,
            CURRENCY,
            INTEGER,
            CURRENCY,
            TIMESTAMP,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            INTEGER,
            STRING,
            STRING,
            STRING,
            STRING,
            STRING,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            INTEGER,
            INTEGER,
            INTEGER,
            STRING,
            STRING,
            INTEGER,   // number of cash transactions
            INTEGER,
            INTEGER,
            INTEGER,
            INTEGER,
            INTEGER,
            INTEGER,
            INTEGER,
            INTEGER,
            INTEGER,   // number of other card transactions
            STRING,
            TIMESTAMP,
            TIMESTAMP,
            STRING,
            CURRENCY,
            STRING,
            STRING,
            CURRENCY,
            INTEGER,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            CURRENCY,
            INTEGER,
            INTEGER,
            INTEGER,
            CURRENCY,
            CURRENCY
        };

    try
	{
		transaction.StartTransaction();

        TIBSQL *fieldCountQuery = transaction.Query( transaction.AddQuery() );

        fieldCountQuery->SQL->Text = "SELECT COUNT(MALLEXPORT_KEY) FIELD_COUNT FROM MALLEXPORT;";
        fieldCountQuery->ExecQuery();

        for (int i = fieldCountQuery->FieldByName("FIELD_COUNT")->AsInteger; i < NUMBER_OF_FIELDS; i++)
        {
            TIBSQL *insertQuery    = transaction.Query( transaction.AddQuery() );
            UnicodeString valueType = "";

            switch (fieldTypes[i])
            {
                case STRING:
                    valueType = "STRING_VALUE";
                    break;
                case INTEGER:
                    valueType = "INTEGER_VALUE";
                    break;
                case CURRENCY:
                    valueType = "CURRENCY_VALUE";
                    break;
                case TIMESTAMP:
                    valueType = "TIMESTAMP_VALUE";
                    break;
                default:
                    break;
            }

            insertQuery->SQL->Text = "INSERT INTO MALLEXPORT "
                                     "( "
                                     "   MALLEXPORT_KEY,"
                                     "   FIELD_NAME,"
                                     "   " + valueType + ""
                                     ") "
                                     "VALUES "
                                     "( "
                                     "   :MALLEXPORT_KEY,"
                                     "   :FIELD_NAME,"
                                     "   :" + valueType + ""
                                     ");";

            insertQuery->ParamByName("MALLEXPORT_KEY")->AsInteger = (i + 1);
            insertQuery->ParamByName("FIELD_NAME"    )->AsString  = fieldNames[i];

            switch (fieldTypes[i])
            {
                case STRING:
                    insertQuery->ParamByName(valueType)->AsString = "";
                    break;
                case INTEGER:
                    insertQuery->ParamByName(valueType)->AsInteger = 0;
                    break;
                case CURRENCY:
                    insertQuery->ParamByName(valueType)->AsCurrency = 0;
                    break;
                case TIMESTAMP:
                    insertQuery->ParamByName(valueType)->AsDateTime = 0;
                    break;
                default:
                    break;
            }

            insertQuery->ExecQuery();
        }

		transaction.Commit();
	}
	catch( ... )
	{
        transaction.Rollback();
        throw;
	}
}
//---------------------------------------------------------------------------
void TApplyParser::updatePaymentAccessToContacts6_00(TDBControl* const inDBControl)
{    TDBTransaction transaction( *inDBControl );
	try
	{
		transaction.StartTransaction();
		TIBSQL *contactsAccessLevelQuery   = transaction.Query(transaction.AddQuery());
		contactsAccessLevelQuery->SQL->Text = "SELECT C.CONTACTS_KEY, C.ACCESS_LEVEL FROM CONTACTS C";
		contactsAccessLevelQuery->ExecQuery();
		for (; !contactsAccessLevelQuery->Eof; contactsAccessLevelQuery->Next())
		{
			int contactKey = contactsAccessLevelQuery->FieldByName("CONTACTS_KEY")->AsInteger;
			int contactAccessLevel =  contactsAccessLevelQuery->FieldByName("ACCESS_LEVEL")->AsInteger;
			unsigned int isPaymentAccessAllowed =  (contactAccessLevel & vPaymentAcess) ? 1 : 0;
			if(isPaymentAccessAllowed != 1)
			{
				TIBSQL *contactsAccessLevelUpdateQuery   = transaction.Query(transaction.AddQuery());
				contactsAccessLevelUpdateQuery->SQL->Text = "UPDATE CONTACTS SET ACCESS_LEVEL = :ACCESS_LEVEL WHERE CONTACTS_KEY = :CONTACTS_KEY";
				contactsAccessLevelUpdateQuery->ParamByName("ACCESS_LEVEL")->AsInteger = (contactAccessLevel + vPaymentAcess);
				contactsAccessLevelUpdateQuery->ParamByName("CONTACTS_KEY")->AsInteger = contactKey;
				contactsAccessLevelUpdateQuery->ExecQuery();
			}
		}
		transaction.Commit();
	}
	catch( ... )
	{
		transaction.Rollback();
		throw;
	}
}
//---------------------------------------------------------------------------
void TApplyParser::updateDiscountGroups_DiscountTypesTable6_00( TDBControl* const inDBControl )
{    std::vector<UnicodeString> foreignKeys;
	getForeignKeysListForTable( getForeignKeysListForTableQuery("DISCOUNTGROUPS_DISCOUNTTYPES"), foreignKeys, inDBControl );
	std::vector<UnicodeString>::iterator it = foreignKeys.begin();
	if ( fieldExists("DISCOUNTGROUPS_DISCOUNTTYPES", "DISCOUNTTYPE_KEY", inDBControl )
			&& fieldExists("DISCOUNTGROUPS_DISCOUNTTYPES", "DISCOUNTGROUPS_KEY", inDBControl ) )
	{
		TDBTransaction DBTransaction( *inDBControl );
		TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
		DBTransaction.StartTransaction();
		for( ; it != foreignKeys.end(); it++ )
		{
			UnicodeString ConstraintName = *it;
			IBQuery->Close();
			IBQuery->SQL->Text = "ALTER TABLE DISCOUNTGROUPS_DISCOUNTTYPES DROP CONSTRAINT " + ConstraintName;
			IBQuery->ExecQuery();
		}
		DBTransaction.Commit();
		executeQuery(
		"ALTER TABLE DISCOUNTGROUPS_DISCOUNTTYPES "
		"ADD foreign key(DISCOUNTTYPE_KEY) references DISCOUNTS(DISCOUNT_KEY) ON DELETE cascade,"
		"ADD foreign key(DISCOUNTGROUPS_KEY) references DISCOUNT_GROUPS(DISCOUNTGROUPS_KEY) ON DELETE cascade;",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::updateTaxRemovalToContacts6_00(TDBControl* const inDBControl)
{    TDBTransaction transaction( *inDBControl );
	try
	{
		transaction.StartTransaction();
		TIBSQL *contactsAccessLevelQuery   = transaction.Query(transaction.AddQuery());
		contactsAccessLevelQuery->SQL->Text = "SELECT C.CONTACTS_KEY, C.ACCESS_LEVEL FROM CONTACTS C";
		contactsAccessLevelQuery->ExecQuery();
		for (; !contactsAccessLevelQuery->Eof; contactsAccessLevelQuery->Next())
		{
			int contactKey = contactsAccessLevelQuery->FieldByName("CONTACTS_KEY")->AsInteger;
			int contactAccessLevel =  contactsAccessLevelQuery->FieldByName("ACCESS_LEVEL")->AsInteger;
			unsigned int isTaxRemovalAllowed =  (contactAccessLevel & vTaxRemoval) ? 1 : 0;
			if(isTaxRemovalAllowed != 1)
			{
				TIBSQL *contactsAccessLevelUpdateQuery   = transaction.Query(transaction.AddQuery());
				contactsAccessLevelUpdateQuery->SQL->Text = "UPDATE CONTACTS SET ACCESS_LEVEL = :ACCESS_LEVEL WHERE CONTACTS_KEY = :CONTACTS_KEY";
				contactsAccessLevelUpdateQuery->ParamByName("ACCESS_LEVEL")->AsInteger = (contactAccessLevel + vTaxRemoval);
				contactsAccessLevelUpdateQuery->ParamByName("CONTACTS_KEY")->AsInteger = contactKey;
				contactsAccessLevelUpdateQuery->ExecQuery();
			}
		}
		transaction.Commit();
	}
	catch( ... )
	{
		transaction.Rollback();
		throw;
	}
}
//---------------------------------------------------------------------------
void TApplyParser::updateChitTable6_00( TDBControl* const inDBControl )
{
   if ( !fieldExists("CHITNUMBER", "IS_NONCHARGABLECHIT", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE CHITNUMBER "
		"ADD IS_NONCHARGABLECHIT CHAR(1) default 'F';",
		inDBControl );
		executeQuery(
		"UPDATE CHITNUMBER "
		"SET IS_NONCHARGABLECHIT = 'F';",
		inDBControl );
	}
	//MM-1647 Add a column to CHITNUMBER table for persisting the new setting.
	if ( !fieldExists("CHITNUMBER", "ENABLE_DISPLAY_TABLES", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE CHITNUMBER "
		"ADD ENABLE_DISPLAY_TABLES CHAR(1) default 'F';",
		inDBControl );
		executeQuery(
		"UPDATE CHITNUMBER "
		"SET ENABLE_DISPLAY_TABLES = 'F';",
		inDBControl );
	}
	//MM-2035 Add a column to CHITNUMBER table for persisting the new setting.
	if ( !fieldExists("CHITNUMBER", "ENABLE_NAMEANDPHONENUMBER", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE CHITNUMBER "
		"ADD ENABLE_NAMEANDPHONENUMBER CHAR(1) default 'F';",
		inDBControl );
		executeQuery(
		"UPDATE CHITNUMBER "
		"SET ENABLE_NAMEANDPHONENUMBER = 'F';",
		inDBControl );
	}
	//MM-2036 Add a column to CHITNUMBER table for persisting the new setting.
	if ( !fieldExists("CHITNUMBER", "ENABLE_AUTOSAVETAB", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE CHITNUMBER "
		"ADD ENABLE_AUTOSAVETAB CHAR(1) default 'F';",
		inDBControl );
		executeQuery(
		"UPDATE CHITNUMBER "
		"SET ENABLE_AUTOSAVETAB = 'F';",
		inDBControl );
	}
	//MM-2063 Add a column to CHITNUMBER table for persisting the new setting.
	if ( !fieldExists("CHITNUMBER", "ENABLE_CUSTOMERDETAILS", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE CHITNUMBER "
		"ADD ENABLE_CUSTOMERDETAILS CHAR(1) default 'F';",
		inDBControl );
		executeQuery(
		"UPDATE CHITNUMBER "
		"SET ENABLE_CUSTOMERDETAILS = 'F';",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::updateDayArcDiscountTable6_00( TDBControl* const inDBControl )
{  if ( !fieldExists("DAYARCORDERDISCOUNTS", "DISCOUNT_GROUPNAME", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE DAYARCORDERDISCOUNTS "
		"ADD DISCOUNT_GROUPNAME VARCHAR(50) default null;",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::updateArcDiscountTable6_00( TDBControl* const inDBControl )
{  if ( !fieldExists("ARCORDERDISCOUNTS", "DISCOUNT_GROUPNAME", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE ARCORDERDISCOUNTS "
		"ADD DISCOUNT_GROUPNAME VARCHAR(50) default null;",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::updateChitTableData6_00(TDBControl * const inDBControl)
{ TDBTransaction transaction( *inDBControl );
	try
	{
		transaction.StartTransaction();
		TIBSQL *resultQuery    = transaction.Query( transaction.AddQuery() );
		resultQuery->SQL->Text = "SELECT * FROM CHITNUMBER WHERE IS_NONCHARGABLECHIT = 'T'";
		resultQuery->ExecQuery();
		if(resultQuery->Eof)
		{
			TIBSQL *keyGeneratorQuery    = transaction.Query( transaction.AddQuery() );
			keyGeneratorQuery->SQL->Text = "SELECT GEN_ID(GEN_CHITNUMBER_KEY, 1) FROM RDB$DATABASE";
			keyGeneratorQuery->ExecQuery();
			int chitKey = keyGeneratorQuery->Fields[0]->AsInteger;
			//:::::::::::::::::::::::::::::::::::::::
			TIBSQL *chitInsertQuery    = transaction.Query( transaction.AddQuery() );
			chitInsertQuery->SQL->Text = "INSERT INTO CHITNUMBER "
			"( "
			"   CHITNUMBER_KEY, "
			"   NAME, "
			"   MASK,"
			"   CHITTYPE, "
			"   FORMAT, "
			"   IS_DEFAULT, "
			"   DISABLE_HOLD_AND_SEND,  "
			"   IS_NONCHARGABLECHIT"
			") "
			"VALUES "
			"( "
			"   :CHITNUMBER_KEY, "
			"   :NAME, "
			"   :MASK, "
			"   :CHITTYPE, "
			"   :FORMAT, "
			"   :IS_DEFAULT, "
			"   :DISABLE_HOLD_AND_SEND, "
			"   :IS_NONCHARGABLECHIT"
			");";
			chitInsertQuery->ParamByName("CHITNUMBER_KEY")->AsInteger = chitKey;
			chitInsertQuery->ParamByName("NAME")->AsString  = "NonChargable";
			chitInsertQuery->ParamByName("MASK"    )->AsString  = "NC";
			chitInsertQuery->ParamByName("CHITTYPE")->AsInteger = 1;
			chitInsertQuery->ParamByName("FORMAT")->AsInteger  = 0;
			chitInsertQuery->ParamByName("IS_DEFAULT")->AsString = 'F';
			chitInsertQuery->ParamByName("DISABLE_HOLD_AND_SEND")->AsString  = 'F';
			chitInsertQuery->ParamByName("IS_NONCHARGABLECHIT"    )->AsString  = 'T';
			chitInsertQuery->ExecQuery();
		}
		transaction.Commit();
	}
	catch(Exception &exception)
	{
		transaction.Rollback();
		throw;
	}
}
//---------------------------------------------------------------------------
void TApplyParser::updateDayArchiveTable6_00( TDBControl* const inDBControl )
{  if ( !fieldExists("DAYARCHIVE", "BASE_PRICE", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE DAYARCHIVE "
		"ADD BASE_PRICE NUMERIC(15,4) default 0;",
		inDBControl );
	}
	if ( !fieldExists("DAYARCHIVE", "DISCOUNT_WITHOUT_TAX", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE DAYARCHIVE "
		"ADD DISCOUNT_WITHOUT_TAX NUMERIC(15,4) default 0;",
		inDBControl );
	}
	if ( !fieldExists("DAYARCHIVE", "TAX_ON_DISCOUNT", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE DAYARCHIVE "
		"ADD TAX_ON_DISCOUNT NUMERIC(15,4) default 0;",
		inDBControl );
	}
}//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TApplyParser::create6_00TablesForMallExportHourly( TDBControl* const inDBControl )
{    if ( !tableExists( "MALLEXPORT_HOURLY", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE MALLEXPORT_HOURLY "
		"( "
		"   ME_HOURLY_KEY INTEGER NOT NULL PRIMARY KEY, "
		"   TERMINAL_NAME VARCHAR(50), "
		"   TENANT_NAME VARCHAR(50), "
		"   DATE_VALUE VARCHAR(50), "
		"   TIME_VALUE VARCHAR(50), "
		"   AMOUNT_VALUE NUMERIC(15,4), "
		"   TRANSACTION_COUNT INTEGER "
		");",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::updateArchiveTable6_00( TDBControl* const inDBControl )
{  if ( !fieldExists("ARCHIVE", "BASE_PRICE", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE ARCHIVE "
		"ADD BASE_PRICE NUMERIC(15,4) default 0;",
		inDBControl );
	}
	if ( !fieldExists("ARCHIVE", "DISCOUNT_WITHOUT_TAX", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE ARCHIVE "
		"ADD DISCOUNT_WITHOUT_TAX NUMERIC(15,4) default 0;",
		inDBControl );
	}
	if ( !fieldExists("ARCHIVE", "TAX_ON_DISCOUNT", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE ARCHIVE "
		"ADD TAX_ON_DISCOUNT NUMERIC(15,4) default 0;",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::updateOrdersTableForBasePrice6_00( TDBControl* const inDBControl )
{  if ( !fieldExists("ORDERS", "BASE_PRICE", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE ORDERS "
		"ADD BASE_PRICE NUMERIC(15,4) default 0;",
		inDBControl );
	}
	if ( !fieldExists("ORDERS", "DISCOUNT_WITHOUT_TAX", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE ORDERS "
		"ADD DISCOUNT_WITHOUT_TAX NUMERIC(15,4) default 0;",
		inDBControl );
	}
	if ( !fieldExists("ORDERS", "TAX_ON_DISCOUNT", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE ORDERS "
		"ADD TAX_ON_DISCOUNT NUMERIC(15,4) default 0;",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::PopulateDiscountWithOutTaxAndTaxOnDiscountColumn6_00(TDBControl* const inDBControl )
{      executeQuery("Update Archive a set a.DISCOUNT_WITHOUT_TAX = a.Discount, a.TAX_ON_DISCOUNT = 0",inDBControl);
	executeQuery("Update DayArchive a set a.DISCOUNT_WITHOUT_TAX = a.Discount, a.TAX_ON_DISCOUNT = 0",inDBControl);
	executeQuery("Update Orders a set a.DISCOUNT_WITHOUT_TAX = a.Discount, a.TAX_ON_DISCOUNT = 0",inDBControl);
}
//---------------------------------------------------------------------------
void TApplyParser::migrateData6_00()
{    TDBTransaction transaction( *_dbControl );
	try
	{
		transaction.StartTransaction();
		std::map<Currency, int> taxProfileKeys = migrateTaxProfiles6_00( &transaction );
		if( !taxProfileKeys.empty() )
		{
			migrateItemSizes6_00(    &transaction, taxProfileKeys );
			migrateOrders6_00(       &transaction, taxProfileKeys );
			migrateArchiveTaxes6_00( &transaction, ARCHIVE        );
			migrateArchiveTaxes6_00( &transaction, DAYARCHIVE     );
		}
		transaction.Commit();
	}
	catch( Exception &E )
	{
		transaction.Rollback();
		throw;
	}
}
//---------------------------------------------------------------------------
std::map<Currency, int> TApplyParser::migrateTaxProfiles6_00( TDBTransaction* const inDBTransaction )
{    std::map<Currency, int> taxProfileKeys = std::map<Currency, int>();
	TIBSQL *taxVaulesQuery    = inDBTransaction->Query( inDBTransaction->AddQuery() );
	TIBSQL *taxProfilesQuery  = inDBTransaction->Query( inDBTransaction->AddQuery() );
	TIBSQL *keyGeneratorQuery = inDBTransaction->Query( inDBTransaction->AddQuery() );
	taxVaulesQuery->SQL->Text =
	"SELECT "
	"    GST_PERCENT "
	"FROM "
	"    ITEMSIZE "
	"UNION "
	"SELECT "
	"    GST_PERCENT "
	"FROM "
	"    ORDERS ";
	taxVaulesQuery->ExecQuery();
	for (; !taxVaulesQuery->Eof; taxVaulesQuery->Next())
	{
		Currency taxRate  = taxVaulesQuery->FieldByName( "GST_PERCENT" )->AsCurrency;
		taxProfilesQuery->Close();
		taxProfilesQuery->SQL->Text =
		"SELECT "
		"    PROFILE_KEY "
		"FROM "
		"    TAXPROFILES "
		"WHERE "
		"    RATE = :RATE "
		"AND "
		"    TYPE = :TYPE ";
		taxProfilesQuery->ParamByName( "RATE" )->AsCurrency = taxRate;
		taxProfilesQuery->ParamByName( "TYPE" )->AsInteger  = 0;
		taxProfilesQuery->ExecQuery();
		if( taxProfilesQuery->RecordCount == 0 )
		{
			keyGeneratorQuery->Close();
			keyGeneratorQuery->SQL->Text = "SELECT GEN_ID(GEN_TAXPROFILES, 1) FROM RDB$DATABASE";
			keyGeneratorQuery->ExecQuery();
			int taxProfileKey = keyGeneratorQuery->Fields[0]->AsInteger;
			taxProfilesQuery->Close();
			taxProfilesQuery->SQL->Text =
			"INSERT INTO TAXPROFILES "
			"("
			"    PROFILE_KEY,"
			"    NAME,"
			"    RATE,"
			"    TYPE,"
			"    PRIORITY,"
			"    SELECTABLE"
			") "
			"VALUES "
			"("
			"    :PROFILE_KEY,"
			"    :NAME,"
			"    :RATE,"
			"    :TYPE,"
			"    :PRIORITY,"
			"    :SELECTABLE"
			")";
			taxProfilesQuery->ParamByName( "PROFILE_KEY" )->AsInteger  = taxProfileKey;
			taxProfilesQuery->ParamByName( "NAME"        )->AsString   = UnicodeString::CurrToStrF(taxRate, UnicodeString::sffFixed, 2) + "% GST";
			taxProfilesQuery->ParamByName( "RATE"        )->AsCurrency = taxRate;
			taxProfilesQuery->ParamByName( "TYPE"        )->AsInteger  = 0;
			taxProfilesQuery->ParamByName( "PRIORITY"    )->AsInteger  = taxProfileKey;
			taxProfilesQuery->ParamByName( "SELECTABLE"  )->AsString   = "T";
			taxProfilesQuery->ExecQuery();
			taxProfileKeys[taxRate] = taxProfileKey;
		}
	}
	return taxProfileKeys;
}
//---------------------------------------------------------------------------
void TApplyParser::migrateItemSizes6_00( TDBTransaction* const inDBTransaction, std::map<Currency, int> inTaxProfileKeys )
{	try
	{
		std::vector<AnsiString> migrationQueriesList  = buildItemSizeMigrationQueries( inDBTransaction, inTaxProfileKeys );
		std::vector<AnsiString>::iterator itQueryList = migrationQueriesList.begin();
		TIBSQL* itemSizeMigrationQuery = inDBTransaction->Query( inDBTransaction->AddQuery() );
		for( ; itQueryList != migrationQueriesList.end(); itQueryList++ )
		{
			itemSizeMigrationQuery->Close();
			itemSizeMigrationQuery->SQL->Text = *itQueryList;
			itemSizeMigrationQuery->ExecQuery();
		}
		itemSizeMigrationQuery->Close();
	}
	catch( Exception &ex )
	{
		throw ex;
	}
}//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
std::vector<AnsiString> TApplyParser::buildItemSizeMigrationQueries(  TDBTransaction* const inDBTransaction, std::map<Currency, int> &inTaxProfileKeys )
{	std::vector<AnsiString> isTPMigrationsList;	// collection of migration query string
	std::map<Currency, int>::iterator itTaxProfileKeys = inTaxProfileKeys.begin();
	isTPMigrationsList.clear();
	for( ; itTaxProfileKeys != inTaxProfileKeys.end(); itTaxProfileKeys++ )
	{
		// load migration queries for each tax value
		loadItemSizeTPMigrationQueriesForTaxVal(
		inDBTransaction,
		isTPMigrationsList,
		itTaxProfileKeys->first,
		itTaxProfileKeys->second );
	}
	return isTPMigrationsList;
}//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TApplyParser::loadItemSizeTPMigrationQueriesForTaxVal(
TDBTransaction* const inDBTransaction,
std::vector<AnsiString> &isTPMigrationsList,
Currency taxValue,
int taxProfileKey )
{	AnsiString migrationQueryForTaxValue = "";
	TIBSQL* itemSizeKeysQuery 	  = inDBTransaction->Query( inDBTransaction->AddQuery() );
	TIBSQL* itemSizeTPKeyGenQuery = inDBTransaction->Query( inDBTransaction->AddQuery() );
	int counter 				  = 0;
	// query to get item size keys for a tax value
	itemSizeKeysQuery->SQL->Text =
	"Select "
	"	A.ITEMSIZE_KEY "
	"From "
	"	ITEMSIZE A "
	"Where "
	"	A.GST_PERCENT = :GST_PERCENT " ;
	itemSizeKeysQuery->ParamByName("GST_PERCENT")->AsCurrency = taxValue;
	itemSizeKeysQuery->ExecQuery();
	while( !itemSizeKeysQuery->Eof )
	{
		itemSizeTPKeyGenQuery->Close();
		itemSizeTPKeyGenQuery->SQL->Text = "SELECT GEN_ID(GEN_TAXPROFILES_ITEMSIZE, 1) FROM RDB$DATABASE"; 	// query for generating primary keys
		itemSizeTPKeyGenQuery->ExecQuery();																	// generating keys for each migration row
		AnsiString migrationRow = " select first 1 '%d','%d','%d' from ITEMSIZE ";
		TVarRec rowData[3] = {
			itemSizeTPKeyGenQuery->Fields[0]->AsInteger,   // primary key for the table
			taxProfileKey,                                 // tax profile foreign key
			itemSizeKeysQuery->Fields[0]->AsInteger		   // item size foreign key
		};
		migrationRow = AnsiString::Format( migrationRow, rowData, ARRAYSIZE(rowData) - 1 );
		migrationQueryForTaxValue += migrationRow;
		itemSizeKeysQuery->Next();
		// firebird has a limit on the number of select + union statements of 255.
		// As we have both select and union operators, setting counter to 125 makes us
		// reach the number of selects + unions to 250. just under the limit.
		if( counter < 125
				&& !itemSizeKeysQuery->Eof )
		{
			migrationQueryForTaxValue += " union ";
			counter++;
		}
		else
		{
			AnsiString queryStr		 	= " insert into TAXPROFILES_ITEMSIZE " + migrationQueryForTaxValue;
			isTPMigrationsList.push_back( queryStr );
			migrationQueryForTaxValue 	= "";
			counter 					= 0;
		}
	}
	itemSizeTPKeyGenQuery->Close();
	itemSizeKeysQuery->Close();
}//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TApplyParser::migrateOrders6_00(
TDBTransaction* const inDBTransaction,
std::map<Currency, int> inTaxProfileKeys )
{	try
	{
		std::map<Currency, int>::iterator tpkIT = inTaxProfileKeys.begin();
		for( ; tpkIT != inTaxProfileKeys.end(); tpkIT++ )
		{
			int  taxProfilePercentage = tpkIT->first;
			int  taxProfileKey        = tpkIT->second;
			std::vector<int> orderKeys = orderKeysWithTaxPercentage(
			taxProfilePercentage,
			inDBTransaction );
			saveKeysToTaxProfileOrdersTable(
			orderKeys,
			taxProfileKey,
			inDBTransaction );
			orderKeys.clear();
		}
	}
	catch( Exception &ex )
	{
		throw ex;
	}
}//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
std::vector<int> TApplyParser::orderKeysWithTaxPercentage(
Currency inTaxPercentage,
TDBTransaction* const inDBTransaction )
{    std::vector<int> result;
	//::::::::::::::::::::::::::::::::::::::::::::::
	TIBSQL* orderKeysQuery = inDBTransaction->Query( inDBTransaction->AddQuery() );
	// query to get order keys for a tax percentage
	orderKeysQuery->SQL->Text =
	"Select "
	"	A.ORDER_KEY "
	"From "
	"	ORDERS A "
	"Where "
	"	A.GST_PERCENT = :GST_PERCENT " ;
	orderKeysQuery->ParamByName( "GST_PERCENT" )->AsCurrency = inTaxPercentage;
	orderKeysQuery->ExecQuery();
	while( !orderKeysQuery->Eof )
	{
		result.push_back( orderKeysQuery->Fields[0]->AsInteger );
		orderKeysQuery->Next();
	}
	orderKeysQuery->Close();
	//::::::::::::::::::::::::::::::::::::::::::::::
	return result;
}//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TApplyParser::saveKeysToTaxProfileOrdersTable(
std::vector<int> inOrdersKeys,
int inTaxProfilesKey,
TDBTransaction* const inDBTransaction )
{    TIBSQL *keyGeneratorQuery      = inDBTransaction->Query( inDBTransaction->AddQuery() );
	TIBSQL *taxProfilesOrdersQuery = inDBTransaction->Query( inDBTransaction->AddQuery() );
	std::vector<int>::iterator ordersIT = inOrdersKeys.begin();
	for( ; ordersIT != inOrdersKeys.end(); ordersIT++ )
	{
		keyGeneratorQuery->Close();
		keyGeneratorQuery->SQL->Text = "SELECT GEN_ID( GEN_TAXPROFILES_ORDERS, 1 ) FROM RDB$DATABASE;";
		keyGeneratorQuery->ExecQuery();
		int taxProfilesOrdersKey = keyGeneratorQuery->Fields[0]->AsInteger;
		//..............................................
		taxProfilesOrdersQuery->Close();
		taxProfilesOrdersQuery->SQL->Text =
		"INSERT INTO TAXPROFILES_ORDERS "
		"("
		"    TPO_KEY,"
		"    PROFILE_KEY,"
		"    ORDER_KEY"
		") "
		"VALUES "
		"("
		"    :TPO_KEY,"
		"    :PROFILE_KEY,"
		"    :ORDER_KEY"
		")";
		taxProfilesOrdersQuery->ParamByName( "TPO_KEY"     )->AsInteger = taxProfilesOrdersKey;
		taxProfilesOrdersQuery->ParamByName( "PROFILE_KEY" )->AsInteger = inTaxProfilesKey;
		taxProfilesOrdersQuery->ParamByName( "ORDER_KEY"   )->AsInteger = *ordersIT;
		taxProfilesOrdersQuery->ExecQuery();
	}
}
//---------------------------------------------------------------------------
void TApplyParser::migrateArchiveTaxes6_00( TDBTransaction* const inDBTransaction, TArchiveType inArchiveType )
{    UnicodeString archiveTable    = ( inArchiveType == ARCHIVE ? "ARCHIVE"       : "DAYARCHIVE"       );
	UnicodeString taxArchiveTable = ( inArchiveType == ARCHIVE ? "ARCORDERTAXES" : "DAYARCORDERTAXES" );
	TIBSQL *archiveQuery      = inDBTransaction->Query( inDBTransaction->AddQuery() );
	TIBSQL *taxArchiveQuery   = inDBTransaction->Query( inDBTransaction->AddQuery() );
	TIBSQL *keyGeneratorQuery = inDBTransaction->Query( inDBTransaction->AddQuery() );
	archiveQuery->Close();
	archiveQuery->SQL->Text =
	"SELECT "
	"    ARCHIVE_KEY, "
	"    PRICE, "
	"    DISCOUNT, "
	"    GST_PERCENT "
	"FROM "
	+ archiveTable;
	archiveQuery->ExecQuery();
	for (; !archiveQuery->Eof; archiveQuery->Next())
	{
		keyGeneratorQuery->Close();
		keyGeneratorQuery->SQL->Text = "SELECT GEN_ID(GEN_" + taxArchiveTable + ", 1) FROM RDB$DATABASE";
		keyGeneratorQuery->ExecQuery();
		int taxArchiveKey  = keyGeneratorQuery->Fields[0]->AsInteger;
		int archiveKey     = archiveQuery->FieldByName( "ARCHIVE_KEY" )->AsInteger;
		Currency price     = archiveQuery->FieldByName( "PRICE"       )->AsCurrency;
		Currency discount  = archiveQuery->FieldByName( "DISCOUNT"    )->AsCurrency;
		Currency taxRate   = archiveQuery->FieldByName( "GST_PERCENT" )->AsCurrency;
		Currency taxValue = ( ( price + discount ) / ( Currency(100.00) + taxRate ) ) * taxRate;
		taxArchiveQuery->Close();
		taxArchiveQuery->SQL->Text =
		"INSERT INTO " + taxArchiveTable + " "
		"("
		"    ARCORDERTAXES_KEY,"
		"    ARCHIVE_KEY,"
		"    TAX_NAME,"
		"    TAX_VALUE,"
		"    TAX_TYPE"
		") "
		"VALUES "
		"("
		"    :ARCORDERTAXES_KEY,"
		"    :ARCHIVE_KEY,"
		"    :TAX_NAME,"
		"    :TAX_VALUE,"
		"    :TAX_TYPE"
		")";
		taxArchiveQuery->ParamByName( "ARCORDERTAXES_KEY" )->AsInteger  = taxArchiveKey;
		taxArchiveQuery->ParamByName( "ARCHIVE_KEY"       )->AsInteger  = archiveKey;
		taxArchiveQuery->ParamByName( "TAX_NAME"          )->AsString   = UnicodeString::CurrToStrF(taxRate, UnicodeString::sffFixed, 2) + "% GST";
		taxArchiveQuery->ParamByName( "TAX_VALUE"         )->AsCurrency = taxValue;
		taxArchiveQuery->ParamByName( "TAX_TYPE"          )->AsInteger  = 0;
		taxArchiveQuery->ExecQuery();
	}
}//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TApplyParser::UpdateContactTable6_00(TDBControl* const inDBControl)
{  if ( !fieldExists("CONTACTS", "LAST_BIRTHDAY_PROCESSED", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE CONTACTS "
		"ADD LAST_BIRTHDAY_PROCESSED Timestamp ;",
		inDBControl );
	}
	if ( !fieldExists("CONTACTS", "TIER_LEVEL", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE CONTACTS "
		"ADD TIER_LEVEL Integer default 0;",
		inDBControl );
	}
	if ( !fieldExists("CONTACTS", "ACTIVATION_DATE", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE CONTACTS "
		"ADD ACTIVATION_DATE Timestamp;",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::create6_00TablesForTierLevels(TDBControl* const inDBControl)
{	if ( !tableExists( "TIER_LEVELS", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE TIER_LEVELS( "
		"  TIER_LEVEL Integer, "
		"  TIER_NAME Varchar(50), "
		"  POINTS_REQUIRED Integer, "
		"  BIRTHDAY_BONUS Integer, "
		"  PRICED_REDEMPTION_RATE Numeric(15,2), "
		"  WEIGHED_REDEMPTION_WEIGHT Numeric(15,2), "
		"  WEIGHED_REDEMPTION_POINTS Numeric(15,2), "
		"  TIER_ID Integer NOT NULL, "
		"  CONSTRAINT PK_TIER_LEVELS_1 PRIMARY KEY (TIER_ID)); ",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateRecipeTable6_00()
{   TDBTransaction transaction( *_dbControl );
	transaction.StartTransaction();
	try
	{
		if ( !fieldExists("RECIPE", "ORGINAL_STOCK_CODE", _dbControl ) )
		{
			TIBSQL *recipeQuery    = transaction.Query( transaction.AddQuery() );
			recipeQuery->Close();
			recipeQuery->SQL->Text = "ALTER TABLE RECIPE ADD ORGINAL_STOCK_CODE Varchar(25)";
			recipeQuery->ExecQuery();
			transaction.Commit();
			transaction.StartTransaction();
			recipeQuery->Close();
			recipeQuery->SQL->Text = "UPDATE RECIPE SET ORGINAL_STOCK_CODE = STOCK_CODE";
			recipeQuery->ExecQuery();
			recipeQuery->Close();
			transaction.Commit();
		}
	}
	catch( Exception &E )
	{
		transaction.Rollback();
		throw;
	}
}
//---------------------------------------------------------------------------
void TApplyParser::CreateRoundTimeProcedure6_00( TDBControl* const inDBControl )
{    bool procedureExistsPredicate = procedureExists( "ROUNDTIME", _dbControl);
	std::string profileKey = "(SELECT PROFILE_KEY FROM PROFILE WHERE PROFILE_TYPE = 4 AND NAME = 'Globals')";
	std::string roundingTimesKey = "(SELECT "
	"VARSPROFILE.INTEGER_VAL "
	"FROM VARSPROFILE "
	"INNER JOIN VARIABLES ON VARSPROFILE.VARIABLES_KEY = VARIABLES.VARIABLES_KEY "
	"WHERE VARIABLES.VARIABLE_NAME LIKE '%Rounded to time%' AND PROFILE_KEY = " + profileKey + " )";
	std::string dmlStatement = procedureExistsPredicate ? "ALTER" : "CREATE";
	executeQuery(
	dmlStatement +" PROCEDURE ROUNDTIME ( inUnroundedTime Timestamp ) "
	"RETURNS ( outRoundedTime Timestamp ) "
	"AS "
	"DECLARE VARIABLE timeStampMinutes INT; "
	"DECLARE VARIABLE timeStampHours INT; "
	"DECLARE VARIABLE timeString CHAR(8); "
	"DECLARE VARIABLE minutesToRoundTo INT; "
	"BEGIN "
	"SELECT ROUNDINGTIMES_VALUE FROM ROUNDINGTIMES WHERE ROUNDINGTIMES_KEY = " + roundingTimesKey + " INTO minutesToRoundTo; "
	"IF ( minutesToRoundTo = 0 ) THEN "
	"outRoundedTime = InUnroundedTime; "
	"ELSE "
	"BEGIN "
	"timeStampMinutes = EXTRACT ( MINUTE FROM InUnroundedTime ); "
	"timeStampMinutes = ROUND ( CAST ( timeStampMinutes as DECIMAL(18,4) ) / CAST ( minutesToRoundTo as DECIMAL(18,4) ) ) * minutesToRoundTo; "
	"timeStampHours = EXTRACT ( HOUR FROM InUnroundedTime ); "
	"IF ( timeStampMinutes >= 60 ) THEN "
	"BEGIN "
	"timeStampMinutes = MOD ( timeStampMinutes, 60 ); "
	"timeStampHours = timeStampHours + 1; "
	"END "
	"timeString = LPAD ( TRIM ( CAST ( timeStampHours as CHAR(2) ) ), 2, '0' ) || ':' || LPAD ( TRIM ( CAST ( timeStampMinutes as CHAR(2) ) ), 2, '0' ) || ':00'; "
	"outRoundedTime = DATEADD(SECOND, DATEDIFF(SECOND, CAST ( InUnroundedTime AS TIME ), CAST ( timeString as TIME ) ), InUnroundedTime); "
	"END "
	"SUSPEND; "
	"END ",
	inDBControl );
}//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

//::::::::::::::::::::::::Version 6.01::::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_01Tables()
{
    updateTableDiscountGroups6_01( _dbControl );
	UpdateContactTable6_01(_dbControl);
	UpdateTierLabelTable6_01(_dbControl);
	UpdateLoyaltPendingTransactionTable6_01(_dbControl);
	PopulateMemberTypeForContacts6_01(_dbControl);
}
//---------------------------------------------------------------------------
void TApplyParser::updateTableDiscountGroups6_01(  TDBControl* const inDBControl  )
{    TDBTransaction transaction( *inDBControl );
    const int NUMBER_OF_FIELDS = 3;

    UnicodeString discountGroups[NUMBER_OF_FIELDS] = { "Corporate Discount", "Promo Discount", "Other Discount"};
    int discountGroupTypes[NUMBER_OF_FIELDS] = {0,0,0};

    UnicodeString taxExempt[NUMBER_OF_FIELDS] = { "F","F","F"};

    try
	{
		transaction.StartTransaction();

        TIBSQL *fieldCountQuery = transaction.Query( transaction.AddQuery() );

        fieldCountQuery->SQL->Text = "SELECT COUNT(DISCOUNTGROUPS_KEY) FIELD_COUNT FROM DISCOUNT_GROUPS;";
        fieldCountQuery->ExecQuery();

        for (int i = 0; i < NUMBER_OF_FIELDS; i++)
        {
            TIBSQL *keyGeneratorQuery    = transaction.Query( transaction.AddQuery() );

            keyGeneratorQuery->SQL->Text = "SELECT GEN_ID(GEN_DISCOUNT_GROUPS, 1) FROM RDB$DATABASE";
            keyGeneratorQuery->ExecQuery();

            int key = keyGeneratorQuery->Fields[0]->AsInteger;

            //:::::::::::::::::::::::::::::::::::::::

            TIBSQL *insertQuery    = transaction.Query( transaction.AddQuery() );
            insertQuery->SQL->Text = "INSERT INTO DISCOUNT_GROUPS "
                                     "( "
                                     "   DISCOUNTGROUPS_KEY,"
                                     "   DISCOUNTGROUP_NAME,"
                                     "   DEFAULT_GROUP,"
                                     "   DISCOUNTGROUP_TYPE,"
                                     "   TAX_EXEMPT  "
                                     ") "
                                     "VALUES "
                                     "( "
                                     "   :DISCOUNTGROUPS_KEY,"
                                     "   :DISCOUNTGROUP_NAME,"
                                     "   :DEFAULT_GROUP,"
                                     "   :DISCOUNTGROUP_TYPE,"
                                     "   :TAX_EXEMPT "
                                     ");";

            insertQuery->ParamByName("DISCOUNTGROUPS_KEY")->AsInteger = key;
            insertQuery->ParamByName("DISCOUNTGROUP_NAME")->AsString  = discountGroups[i];
            insertQuery->ParamByName("DEFAULT_GROUP"    )->AsString  = "T";
            insertQuery->ParamByName("DISCOUNTGROUP_TYPE"    )->AsInteger  = discountGroupTypes[i];
            insertQuery->ParamByName("TAX_EXEMPT"    )->AsString  = taxExempt[i];
            insertQuery->ExecQuery();
        }

		transaction.Commit();
	}
	catch( ... )
	{
        transaction.Rollback();
        throw;
	}
}//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TApplyParser::PopulateMemberTypeForContacts6_01(TDBControl* const inDBControl)
{   TDBTransaction transaction( *_dbControl );
	transaction.StartTransaction();
	try
	{
		TIBSQL *query    = transaction.Query( transaction.AddQuery() );
		transaction.StartTransaction();
		query->Close();
		query->SQL->Text = "UPDATE CONTACTS SET MEMBER_TYPE = 1";
		query->ExecQuery();
		query->Close();
		transaction.Commit();
	}
	catch( Exception &E )
	{
		transaction.Rollback();
		throw;
	}
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateTierLabelTable6_01(TDBControl* const inDBControl)
{  if ( !fieldExists("TIER_LEVELS", "CLOUD_ID", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE TIER_LEVELS "
		"ADD CLOUD_ID Integer ;",
		inDBControl );
	}
	if ( !fieldExists("TIER_LEVELS", "SEND_MAIL_TO_HO", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE TIER_LEVELS "
		"ADD SEND_MAIL_TO_HO Char(1) ;",
		inDBControl );
	}
	if ( !fieldExists("TIER_LEVELS", "SEND_MAIL_TO_MEMBER", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE TIER_LEVELS "
		"ADD SEND_MAIL_TO_MEMBER Char(1) ;",
		inDBControl );
	}
	if ( !fieldExists("TIER_LEVELS", "CHANGE_CARD", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE TIER_LEVELS "
		"ADD CHANGE_CARD Char(1) ;",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateContactTable6_01(TDBControl* const inDBControl)
{    if ( !fieldExists("CONTACTS", "MEMBER_TYPE", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE CONTACTS "
		"ADD MEMBER_TYPE Integer;",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateLoyaltPendingTransactionTable6_01(TDBControl* const inDBControl)
{  if ( !fieldExists("LOYALTYPENDINGTRANSACTIONS", "POINTS_TYPE", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE LOYALTYPENDINGTRANSACTIONS "
		"ADD POINTS_TYPE Integer ;",
		inDBControl );
	}
}

//::::::::::::::::::::::::Version 6.03::::::::::::::::::::::::::::::::::::::::::
void TApplyParser::UpdateWriteOffTable6_03(TDBControl* const inDBControl)
{  if ( !fieldExists("WRITEOFF", "COST", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE WRITEOFF "
		"ADD COST Numeric(15,4) ;",
		inDBControl );
	}
	if ( !fieldExists("WRITEOFF", "QTY", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE WRITEOFF "
		"ADD QTY Integer ;",
		inDBControl );
	}
	if ( !fieldExists("WRITEOFF", "THIRD_PARTY_CODE", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE WRITEOFF "
		"ADD THIRD_PARTY_CODE Varchar(50) ;",
		inDBControl );
	}
	if ( !fieldExists("WRITEOFF", "ORDER_LOCATION", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE WRITEOFF "
		"ADD ORDER_LOCATION Varchar(50) ;",
		inDBControl );
	}
	if ( !fieldExists("WRITEOFF", "COURSE_NAME", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE WRITEOFF "
		"ADD COURSE_NAME Varchar(50) ;",
		inDBControl );
	}
	if ( !fieldExists("WRITEOFF", "MENU_NAME", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE WRITEOFF "
		"ADD MENU_NAME Varchar(50) ;",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::CreateTableTablesStatus6_03(TDBControl* const inDBControl)
{    if (tableExists( "TABLESSTATUS", _dbControl ) )
	return;
	TDBTransaction transaction( *inDBControl );
	try
	{
		transaction.StartTransaction();
		TIBSQL *Query    = transaction.Query( transaction.AddQuery() );
		Query->SQL->Text = "CREATE TABLE TABLESSTATUS "
		"(  TABLE_NUMBER Integer primary key, "
		"   TABLE_STATUS Integer );";
		Query->ExecQuery();
		Query->Close();
		transaction.Commit();
		transaction.StartTransaction();
		Query->SQL->Text = "SELECT distinct a.TABLE_NUMBER   "
		" FROM ORDERS a; ";
		Query->ExecQuery();
		TIBSQL *InsertQuery    = transaction.Query( transaction.AddQuery() );
		InsertQuery->SQL->Text = "INSERT INTO TABLESSTATUS "
		"( "
		"   TABLE_NUMBER,"
		"   TABLE_STATUS "
		") "
		"VALUES "
		"( "
		"   :TABLE_NUMBER,"
		"   :TABLE_STATUS "
		");";
		for (; !Query->Eof; Query->Next())
		{
			InsertQuery->Close();
			InsertQuery->ParamByName("TABLE_NUMBER")->AsInteger = Query->FieldByName("TABLE_NUMBER")->AsInteger;
			InsertQuery->ParamByName("TABLE_STATUS")->AsInteger = 2;
			InsertQuery->ExecQuery();
		}
		transaction.Commit();
	}
	catch( ... )
	{
		transaction.Rollback();
		throw;
	}
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateDayArcbillTable6_03(TDBControl* const inDBControl)
{
	if ( !fieldExists("DAYARCBILL", "ROUNDING_ADJUSTMENT", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE DAYARCBILL "
		"ADD ROUNDING_ADJUSTMENT Numeric(15,4);",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateArcbillTable6_03(TDBControl* const inDBControl)
{
	if ( !fieldExists("ARCBILL", "ROUNDING_ADJUSTMENT", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE ARCBILL "
		"ADD ROUNDING_ADJUSTMENT Numeric(15,4);",
		inDBControl );
	}
}

//::::::::::::::::::::::::Version 6.04::::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_04Tables()
{
	ModifyContactsColumns6_04( _dbControl );
	UpdateLocationColumns6_04( _dbControl );
}
//---------------------------------------------------------------------------
void TApplyParser::ModifyContactsColumns6_04( TDBControl* const inDBControl )
{
	if ( !fieldExists( "CONTACTS", "ACCESS_LEVEL", _dbControl ) )
	{
		executeQuery (
		"ALTER TABLE CONTACTS "
		"ALTER COLUMN ACCESS_LEVEL TYPE BIGINT",
		inDBControl);
	}
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateLocationColumns6_04(TDBControl* const inDBControl)
{
	if ( !fieldExists("LOCATIONS", "CATEGORY_TYPE", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE LOCATIONS "
		"ADD CATEGORY_TYPE numeric(15,4)  default 99 NOT NULL;",
		inDBControl );
	}
}

//::::::::::::::::::::::::Version 6.05::::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_05Tables()
{
    UpdateTierLevelTable6_05(_dbControl);
    ModifyTierLevelTable6_05(_dbControl);
    UpdateLocationColumns6_05( _dbControl );
    create6_05TablesForMallExportTransaction(_dbControl);
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateTierLevelTable6_05(TDBControl* const inDBControl)
{
	if ( !fieldExists("TIER_LEVELS", "ALLOW_EARNT_REDEMPTION", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE TIER_LEVELS "
		"ADD ALLOW_EARNT_REDEMPTION Char(1) ;",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::ModifyTierLevelTable6_05(TDBControl* const inDBControl)
{
	if ( fieldExists("TIER_LEVELS", "ALLOW_EARNT_REDEMPTION", inDBControl ) )
	{
		executeQuery("UPDATE TIER_LEVELS SET ALLOW_EARNT_REDEMPTION = 'T' ; ",
		inDBControl );
	}


}
//---------------------------------------------------------------------------
void TApplyParser::create6_05TablesForMallExportTransaction(TDBControl* const inDBControl)
{
    if ( !tableExists( "MALLEXPORT_TRANSACTION", inDBControl ) )
	{
		executeQuery(
                "CREATE TABLE MALLEXPORT_TRANSACTION "
                "( "
                "   ME_TRANSACTION_KEY INTEGER NOT NULL PRIMARY KEY, "
                "   DATE_VALUE VARCHAR(50), "
                "   TENANT_NAME VARCHAR(50), "
                "   CURRENT_DATE_VALUE VARCHAR(50), "
                "   TIME_VALUE VARCHAR(50), "
                "   TERMINAL_NAME VARCHAR(50), "
                "   INVOICE_NUMBER VARCHAR(50), "
                "   GROSS_SALES NUMERIC(17,4), "
                "   VAT_SALES NUMERIC(17,4), "
                "   NONVAT_SALES NUMERIC(17,4), "
                "   VATEXEMPT_SALES NUMERIC(17,4), "
                "   SCDISCOUNT_AMOUNT NUMERIC(17,4), "
                "   EMPDISCOUNT_AMOUNT NUMERIC(17,4), "
                "   PRODISCOUNT_AMOUNT NUMERIC(17,4), "
                "   OTHERDISCOUNT_AMOUNT NUMERIC(17,4), "
                "   REFUND_AMOUNT NUMERIC(17,4), "
                "   SCHARGE_AMOUNT VARCHAR(50), "
                "   OTHER_SURCHARGE NUMERIC(17,4), "
                "   NET_SALES NUMERIC(17,4), "
                "   OLD_GRANDTOTAL NUMERIC(17,4), "
                "   NEW_GRANDTOTAL NUMERIC(17,4), "
                "   ZCOUNT NUMERIC(17,4), "
                "   CASH_SALES_AMOUNT NUMERIC(17,4), "
                "   CARD_SALES_AMOUNT NUMERIC(17,4), "
                "   CHECK_SALES_AMOUNT NUMERIC(17,4), "
                "   GC_SALES_AMOUNT NUMERIC(17,4), "
                "   EPAY_SALES_AMOUNT NUMERIC(17,4), "
                "   OTHER_SALES_AMOUNT NUMERIC(17,4), "
                "   VOID_AMOUNT NUMERIC(17,4), "
                "   DEPOSIT_AMOUNT NUMERIC(17,4), "
                "   APPLIED_DEPOSIT_AMOUNT NUMERIC(17,4), "
                "   VAT_SALES_COUNT NUMERIC(17,4), "
                "   NONVAT_SALES_COUNT NUMERIC(17,4), "
                "   VATEXEMPT_SALES_COUNT NUMERIC(17,4), "
                "   SCDISCOUNT_COUNT NUMERIC(17,4), "
                "   EMPDISCOUNT_COUNT NUMERIC(17,4), "
                "   PRODISCOUNT_COUNT NUMERIC(17,4), "
                "   OTHERDISCOUNT_COUNT NUMERIC(17,4), "
                "   REFUND_COUNT NUMERIC(17,4), "
                "   SCHARGE_COUNT NUMERIC(17,4), "
                "   OTHER_SURCHARGE_COUNT NUMERIC(17,4), "
                "   CASH_COUNT NUMERIC(17,4), "
                "   CARD_COUNT NUMERIC(17,4), "
                "   CHECK_COUNT NUMERIC(17,4), "
                "   GC_COUNT NUMERIC(17,4), "
                "   EPAY_COUNT NUMERIC(17,4), "
                "   OTHER_COUNT NUMERIC(17,4), "
                "   VOID_COUNT NUMERIC(17,4), "
                "   TRANS_VOID_COUNT NUMERIC(17,4), "
                "   DEPOSIT_COUNT NUMERIC(17,4), "
                "   APPLIED_DEPOSIT_COUNT NUMERIC(17,4), "
                "   PATRON_COUNT NUMERIC(17,4), "
                "   TRANSACTION_COUNT NUMERIC(17,4), "
                "   NOSALE_COUNT NUMERIC(17,4), "
                "   SALES_FLAG VARCHAR(50), "
                "   VAT_PERCENTAGE NUMERIC(17,4) "
                ");",
			inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateLocationColumns6_05(TDBControl* const inDBControl)
{
	if ( !fieldExists("LOCATIONS", "STOCKREQUESTAUTO_KEY", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE LOCATIONS "
		"ADD STOCKREQUESTAUTO_KEY numeric(15,4)  default 0 NOT NULL;",
		inDBControl );
	}
}

//::::::::::::::::::::::::Version 6.06::::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_06Tables()
{
    UpdateOrdersTable6_06(_dbControl);
    ModifyCloseZedColumns6_06( _dbControl );
    PopulateEmailStatusForZeds6_06(_dbControl);
    CreateCancelItemsTable6_06(_dbControl);
    create6_06Generators(_dbControl);
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateOrdersTable6_06(TDBControl* const inDBControl)
{
	if ( !fieldExists("ORDERS", "DELAYED_INVOICE_NUMBER", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE ORDERS "
		"ADD DELAYED_INVOICE_NUMBER varchar(50);",
		inDBControl );
	}

        if ( !fieldExists("ORDERS", "DELAYED_PAYMENT_PRICE", inDBControl ) )
	{
		executeQuery(
		"ALTER TABLE ORDERS "
		"ADD DELAYED_PAYMENT_PRICE numeric(15,4)  default 0;",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::create6_06Generators( TDBControl* const inDBControl)
{	if( !generatorExists("GEN_CANCELITEMS", _dbControl) )
	{
		executeQuery(
		"CREATE GENERATOR GEN_CANCELITEMS;",
		inDBControl);
		executeQuery(
		"SET GENERATOR GEN_CANCELITEMS TO 0; ",
		inDBControl );
	}
}
//MM-4327
void TApplyParser::CreateCancelItemsTable6_06(TDBControl* const inDBControl)
{
    if ( !tableExists( "CANCELITEMS", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE CANCELITEMS "
		"( "
		"   CANCELITEMS_KEY INTEGER NOT NULL PRIMARY KEY , "
		"   ITEM_NAME VARCHAR(50) , "
		"   QTY NUMERIC(15,4) , "
		"   SIZE_NAME Varchar(30) , "
        "   MENU_NAME Varchar(30) , "
        "   PRICE NUMERIC(15,4) ,  "
        "   TIME_STAMP Timestamp , "
        "   TERMINAL_NAME Varchar(22) , "
        "   ORDER_LOCATION Varchar(25) , "
        "   SECURITY_REF Integer, "
        "   ZED_PRINT SMALLINT,  "
        "   Remove_User Varchar(50)  "
		"); ",
		inDBControl );
	}
}

void TApplyParser::PopulateEmailStatusForZeds6_06(TDBControl* const inDBControl)
{
    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    try
    {
        TIBSQL *query    = transaction.Query( transaction.AddQuery() );
        transaction.StartTransaction();
        query->Close();
        query->SQL->Text = "UPDATE ZEDS SET EMAIL_STATUS = 1";
        query->ExecQuery();
        query->Close();
        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
        throw;
    }
}
//---------------------------------------------------------------------------
void TApplyParser::ModifyCloseZedColumns6_06( TDBControl* const inDBControl )
{
    if ( !fieldExists( "ZEDS", "EMAIL_STATUS", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE ZEDS "
        "ADD EMAIL_STATUS Integer ",
        inDBControl);
    }
}

//::::::::::::::::::::::::Version 6.07::::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_07Tables()
{
	UpdateChitNumberTable6_07(_dbControl);
	CreateChitTaxRelationTable6_07(_dbControl);
	UpdateItemSizeTable6_07(_dbControl);
	UpdateOrderTable6_07(_dbControl);
	ModifyPaymentTypesColumns6_07(_dbControl);
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateChitNumberTable6_07( TDBControl* const inDBControl )
{
    if ( !fieldExists( "CHITNUMBER", "PROMPT_FOR_PICKUP_DELIVERY_TIME", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE CHITNUMBER "
        "ADD PROMPT_FOR_PICKUP_DELIVERY_TIME CHAR(1) default 'F';",
        inDBControl);
    }
}
//---------------------------------------------------------------------------
void TApplyParser::CreateChitTaxRelationTable6_07(TDBControl* const inDBControl)
{
  if ( !tableExists( "CHITTAXRELATION", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE CHITTAXRELATION "
		"( "
		"   CHITNUMBER_KEY INTEGER , "
		"   TAX_PROFILE_KEY INTEGER   "
		"); ",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateItemSizeTable6_07(TDBControl* const inDBControl)
{
    if ( !fieldExists("ITEMSIZE", "MAXRETAILPRICE", inDBControl ) )
    {
        executeQuery(
        "ALTER TABLE ITEMSIZE "
        "ADD MAXRETAILPRICE Numeric(15,4);",
        inDBControl );
    }
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateOrderTable6_07(TDBControl* const inDBControl)
{
    if ( !fieldExists("ORDERS", "MAXRETAILPRICE", inDBControl ) )
    {
        executeQuery(
        "ALTER TABLE ORDERS "
        "ADD MAXRETAILPRICE Numeric(15,4);",
        inDBControl );
    }
}
//---------------------------------------------------------------------------
void TApplyParser::ModifyPaymentTypesColumns6_07( TDBControl* const inDBControl )
{
    if ( !fieldExists( "PAYMENTTYPES", "TABKEY", _dbControl ) )
    {
        executeQuery (
        "ALTER  TABLE PAYMENTTYPES "
        "ADD TABKEY Integer ",
        inDBControl);
    }
}

//::::::::::::::::::::::::Version 6.08::::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_08Tables()
{    //changes in 6.8
	ModifyChitNumberTable6_08(_dbControl);
	PopulateAddMinutesForChitNumber6_08(_dbControl);
	CreateChitDiscountRelationTable6_08(_dbControl);
	UpdateTabHistoryTable6_08(_dbControl);
	UpdateTabTable6_08(_dbControl);
}
//---------------------------------------------------------------------------
void TApplyParser::ModifyChitNumberTable6_08( TDBControl* const inDBControl )
{
    if ( !fieldExists( "CHITNUMBER", "ADDMINUTES", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE CHITNUMBER "
        "ADD ADDMINUTES Integer ",
        inDBControl);
    }
}
//---------------------------------------------------------------------------
void TApplyParser::PopulateAddMinutesForChitNumber6_08(TDBControl* const inDBControl)
{
    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    try
    {
        TIBSQL *query    = transaction.Query( transaction.AddQuery() );
        transaction.StartTransaction();
        query->Close();
        query->SQL->Text = "UPDATE CHITNUMBER SET ADDMINUTES = 0";
        query->ExecQuery();
        query->Close();
        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
        throw;
    }
}
void TApplyParser::CreateChitDiscountRelationTable6_08(TDBControl* const inDBControl)
{
  if ( !tableExists( "CHITDISCOUNTRELATION", _dbControl ) )
	{
		executeQuery(
		"CREATE TABLE CHITDISCOUNTRELATION "
		"( "
		"   CHITNUMBER_KEY INTEGER , "
		"   DISCOUNT_KEY INTEGER   "
		"); ",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateTabTable6_08(TDBControl* const inDBControl)
{
 if ( !fieldExists( "TAB", "TOTAL_DIVISIONS", _dbControl ) )
    {
        executeQuery (
        "ALTER TABLE TAB "
        "ADD TOTAL_DIVISIONS Integer ",
        inDBControl);
    }
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateTabHistoryTable6_08(TDBControl* const inDBControl)
{
     if ( !fieldExists( "TABHISTORY", "IS_TABLE", _dbControl ) )
        {
            executeQuery (
            "ALTER TABLE TABHISTORY "
            "ADD IS_TABLE Char(1) ",
            inDBControl);
        }

    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    try
    {
        TIBSQL *query    = transaction.Query( transaction.AddQuery() );
        transaction.StartTransaction();
        query->Close();
        query->SQL->Text = "ALTER TABLE TABHISTORY DROP CONSTRAINT TABHISTORY_TAB_KEY ";
        query->ExecQuery();
        query->Close();
        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }
}

//::::::::::::::::::::::::Version 6.09::::::::::::::::::::::::::::::::::::::::::
void TApplyParser::update6_09Tables()
{
  create6_09Generators(_dbControl);
  if(UpdateOrdersTable6_09(_dbControl))
  {
     PopulateOrdersTable6_09(_dbControl);
  }
  UpdateArcbill6_09(_dbControl);
  UpdateDayArcbill6_09(_dbControl);
}
//---------------------------------------------------------------------------
void TApplyParser::create6_09Generators( TDBControl* const inDBControl )
{
  if( !generatorExists("GEN_ORDER_ID_NO", _dbControl) )
	{
		executeQuery(
		"CREATE GENERATOR GEN_ORDER_ID_NO;",
		inDBControl);
		executeQuery(
		"SET GENERATOR GEN_ORDER_ID_NO TO 0;",
		inDBControl );
	}
}
//---------------------------------------------------------------------------
bool TApplyParser::UpdateOrdersTable6_09(TDBControl* const inDBControl)
{
    if ( !fieldExists("ORDERS", "ORDER_IDENTIFICATION_NUMBER", inDBControl ) )
    {
        executeQuery(
        "ALTER TABLE ORDERS "
        "ADD ORDER_IDENTIFICATION_NUMBER Integer;",
        inDBControl );
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
void TApplyParser::PopulateOrdersTable6_09(TDBControl* const inDBControl)
{
    TDBTransaction transaction( *_dbControl );
    transaction.StartTransaction();
    try
    {
        int identificatinNo = 0;
        TIBSQL *SelectQuery    = transaction.Query( transaction.AddQuery() );
        TIBSQL *UpdateQuery    = transaction.Query( transaction.AddQuery() );
        TIBSQL *GeneratorQuery    = transaction.Query( transaction.AddQuery() );
        transaction.StartTransaction();
        SelectQuery->Close();
        SelectQuery->SQL->Text = "SELECT distinct a.TABLE_NAME FROM ORDERS a WHERE A.TABLE_NUMBER > 0";
        SelectQuery->ExecQuery();
        for(; !SelectQuery->Eof; SelectQuery->Next())
        {
           AnsiString TableName = SelectQuery->FieldByName("TABLE_NAME")->AsString;
           GeneratorQuery->Close();
           GeneratorQuery->SQL->Text = "SELECT GEN_ID(GEN_ORDER_ID_NO, 1) FROM RDB$DATABASE";
           GeneratorQuery->ExecQuery();
           identificatinNo = GeneratorQuery->Fields[0]->AsInteger;
           UpdateQuery->Close();
           UpdateQuery->SQL->Text = "UPDATE ORDERS SET ORDER_IDENTIFICATION_NUMBER = :ORDER_IDENTIFICATION_NUMBER "
                                    "WHERE TABLE_NAME = :TABLE_NAME ";
           UpdateQuery->ParamByName("ORDER_IDENTIFICATION_NUMBER")->AsInteger = identificatinNo;
           UpdateQuery->ParamByName("TABLE_NAME")->AsString = TableName;
           UpdateQuery->ExecQuery();
        }


        SelectQuery->Close();
        SelectQuery->SQL->Text = "SELECT distinct a.TAB_NAME FROM ORDERS a WHERE A.TABLE_NUMBER = 0";
        SelectQuery->ExecQuery();
        for(; !SelectQuery->Eof; SelectQuery->Next())
        {
           AnsiString TabName = SelectQuery->FieldByName("TAB_NAME")->AsString;
           GeneratorQuery->Close();
           GeneratorQuery->SQL->Text = "SELECT GEN_ID(GEN_ORDER_ID_NO, 1) FROM RDB$DATABASE";
           GeneratorQuery->ExecQuery();
           identificatinNo = GeneratorQuery->Fields[0]->AsInteger;
           UpdateQuery->Close();
           UpdateQuery->SQL->Text = "UPDATE ORDERS SET ORDER_IDENTIFICATION_NUMBER = :ORDER_IDENTIFICATION_NUMBER "
                                    "WHERE TAB_NAME = :TAB_NAME ";
           UpdateQuery->ParamByName("ORDER_IDENTIFICATION_NUMBER")->AsInteger = identificatinNo;
           UpdateQuery->ParamByName("TAB_NAME")->AsString = TabName;
           UpdateQuery->ExecQuery();
        }

        SelectQuery->Close();
        transaction.Commit();
    }
    catch( Exception &E )
    {
        transaction.Rollback();
    }

}
//---------------------------------------------------------------------------
void TApplyParser::UpdateArcbill6_09(TDBControl* const inDBControl)
{
    if ( !fieldExists("ARCBILL", "ORDER_IDENTIFICATION_NUMBER", inDBControl ) )
    {
        executeQuery(
        "ALTER TABLE ARCBILL "
        "ADD ORDER_IDENTIFICATION_NUMBER Integer;",
        inDBControl );
    }
}
//---------------------------------------------------------------------------
void TApplyParser::UpdateDayArcbill6_09(TDBControl* const inDBControl)
{
    if ( !fieldExists("DAYARCBILL", "ORDER_IDENTIFICATION_NUMBER", inDBControl ) )
    {
        executeQuery(
        "ALTER TABLE DAYARCBILL "
        "ADD ORDER_IDENTIFICATION_NUMBER Integer;",
        inDBControl );
    }
}
}
