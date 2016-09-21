namespace ChefMate.Database.Parser
{
    public class ChefmateParserVersion1_0 : VersionParser
    {
        private const string Version = "1.0";

        public override void RunParser()
        {
            if (!IsVersionExist(Version))
            {
                CreateGenerators();
                CreateTables();
                UpdateVersionInDatabase(Version);
            }
        }
        private void CreateGenerators()
        {
            CheckAndCreateGenerator("GEN_GROUPS");
            CheckAndCreateGenerator("GEN_ITEM");
            CheckAndCreateGenerator("GEN_OPTION");
            CheckAndCreateGenerator("GEN_ORDERGROUP");
            CheckAndCreateGenerator("GEN_ORDERITEM");
            CheckAndCreateGenerator("GEN_ORDERITEMOPTION");
            CheckAndCreateGenerator("GEN_ORDERITEMSIDE");
            CheckAndCreateGenerator("GEN_ORDERITEMTIME");
            CheckAndCreateGenerator("GEN_ORDERS");
            CheckAndCreateGenerator("GEN_SETTINGS");
            CheckAndCreateGenerator("GEN_SIDE");
            CheckAndCreateGenerator("GEN_TERMINAL");
            CheckAndCreateGenerator("GEN_TERMINALNAMES");
            CheckAndCreateGenerator("GEN_VERSION");
        }
        private void CreateTables()
        {
            CreateVersionTable();
            CreateTerminalTable();
            CreateTerminalSettingsTable();
            CreateGroupsTable();
            CreateItemsTable();
            CreateSidesTable();
            CreateOptionsTable();
            CreateOrdersTable();
            CreateOrderGroupsTable();
            CreateOrderItemsTable();
            CreateOrderSidesTable();
            CreateOrderOptionsTable();
        }
        private void CreateVersionTable()
        {
            var query = @"CREATE TABLE VERSIONS
                            (
                              VERSION_KEY Integer NOT NULL,
                              VERSION_NUMBER Varchar(7) NOT NULL,
                              VERSION_TIME_STAMP Timestamp NOT NULL,
                              PRIMARY KEY (VERSION_KEY),
                              UNIQUE (VERSION_NUMBER)
                            )";
            CheckAndCreateTable("VERSIONS", query);
        }
        private void CreateTerminalTable()
        {
            var query = @"CREATE TABLE TERMINALS
                            (
                              TERMINALID Integer NOT NULL,
                              TERMINALNAME Varchar(25),
                              TERMINALIPADDRESS Varchar(20),
                              DISPLAY_NAME Varchar(30),
                              TERMINAL_TYPE Integer,
                              CONSTRAINT PK_TERMINALS_1 PRIMARY KEY (TERMINALID),
                              CONSTRAINT UNQ_TERMINALS_1 UNIQUE (TERMINALNAME,TERMINALIPADDRESS)
                            );";
            CheckAndCreateTable("TERMINALS", query);
        }
        private void CreateTerminalSettingsTable()
        {
            var query = @"CREATE TABLE TERMINALSETTINGS
                        (
                            TERMINALKEY Integer,
                            SETTINGNAME Varchar(50),
                            SETTINGVALUE Varchar(150)
                        );";
            CheckAndCreateTable("TERMINALSETTINGS", query);
        }
        private void CreateGroupsTable()
        {
            var query = @"CREATE TABLE GROUPS
                            (
                              GROUP_KEY Integer NOT NULL,
                              GROUP_NAME Varchar(30),
                              GROUP_TYPE Integer,
                              GROUP_POS_KEY Integer,
                              GROUP_DISPLAY_ORDER Integer DEFAULT -1 NOT NULL,
                              PRIMARY KEY (GROUP_KEY),
                              CONSTRAINT GROUP_TYPE_POS_KEY UNIQUE (GROUP_TYPE,GROUP_POS_KEY)
                            );";
            CheckAndCreateTable("GROUPS", query);
        }
        private void CreateItemsTable()
        {
            var query = @"CREATE TABLE ITEMS
                            (
                              ITEM_KEY Integer NOT NULL,
                              ITEM_NAME Varchar(100),
                              ITEM_POS_KEY Integer,
                              PRIMARY KEY (ITEM_KEY),
                              UNIQUE (ITEM_POS_KEY)
                            )";
            CheckAndCreateTable("ITEMS", query);
        }
        private void CreateSidesTable()
        {
            var query = @"CREATE TABLE SIDES
                            (
                              SIDE_KEY Integer NOT NULL,
                              SIDE_NAME Varchar(50),
                              PRIMARY KEY (SIDE_KEY)
                            );";
            CheckAndCreateTable("SIDES", query);
        }
        private void CreateOptionsTable()
        {
            var query = @"CREATE TABLE OPTIONS
                            (
                              OPTION_KEY Integer NOT NULL,
                              OPTION_NAME Varchar(40),
                              PRIMARY KEY (OPTION_KEY)
                            );";
            CheckAndCreateTable("OPTIONS", query);
        }
        private void CreateOrdersTable()
        {
            var query = @"CREATE TABLE ORDERS
                            (
                              ORDER_KEY Integer NOT NULL,
                              ORDER_NUMBER Integer,
                              ORDER_POS_KEY Integer,
                              PATRON_COUNT Integer,
                              CHIT_VALUE Varchar(50),
                              TABLE_TAB_NAME Varchar(50),
                              SOURCE_TABLE_TAB_NAME Varchar(50),
                              PARTY_NAME Varchar(45),
                              SERVER_NAME Varchar(30),
                              CUSTOMER_NAME Varchar(50),
                              ORDER_TYPE Varchar(50),
                              ORDER_STATUS Integer,
                              ORDER_SALE_START_TIME Timestamp DEFAULT TIMESTAMP 'NOW',
                              ORDER_SALE_FINISH_TIME Timestamp DEFAULT TIMESTAMP 'NOW',
                              DELIVERY_TIME Timestamp,
                              IS_BUMPED Integer,
                              ARRIVAL_TIME Timestamp,
                              BUMP_TIME Timestamp,
                              CUSTOMER_PHONE Varchar(25),
                              CUSTOMER_EMAIL Varchar(50),
                              CUSTOMER_ADDRESS Varchar(250),
                              PAYMENT_STATUS Varchar(10),
                              ORDER_ACTION Varchar(20),
                              PRIMARY KEY (ORDER_KEY)
                            );";
            CheckAndCreateTable("ORDERS", query);
        }
        private void CreateOrderGroupsTable()
        {
            var query = @"CREATE TABLE ORDERGROUPS
                            (
                              ORDERGROUP_KEY Integer NOT NULL,
                              ORDER_KEY Integer NOT NULL,
                              GROUP_KEY Integer NOT NULL,
                              ORDERGROUP_CALLED_AWAY Integer DEFAULT 0,
                              PRIMARY KEY (ORDERGROUP_KEY)
                            );";
            CheckAndCreateTable("ORDERGROUPS", query);
        }
        private void CreateOrderItemsTable()
        {
            var query = @"CREATE TABLE ORDERITEMS
                            (
                              ORDERITEM_KEY Integer NOT NULL,
                              ITEM_KEY Integer NOT NULL,
                              COURSE_KEY Integer,
                              SERVINGCOURSE_KEY Integer,
                              TERMINAL_KEY Integer,
                              ORDERITEM_POS_KEY Integer,
                              ORDER_ITEM_STATUS Integer,
                              ORDERITEM_NOTE Varchar(80),
                              ARRIVAL_TIME Timestamp,
                              BUMP_TIME Timestamp,
                              PRIMARY KEY (ORDERITEM_KEY)
                            );";
            CheckAndCreateTable("ORDERITEMS", query);
        }
        private void CreateOrderSidesTable()
        {
            var query = @"CREATE TABLE ORDERITEMSIDES
                            (
                              ORDERITEMSIDE_KEY Integer NOT NULL,
                              ORDERITEM_KEY Integer,
                              SIDE_KEY Integer,
                              ORDERITEMSIDE_POS_KEY Integer,
                              IS_CANCELED Integer DEFAULT 0,
                              PRIMARY KEY (ORDERITEMSIDE_KEY)
                            );";
            CheckAndCreateTable("ORDERITEMSIDES", query);
        }
        private void CreateOrderOptionsTable()
        {
            var query = @"CREATE TABLE ORDERITEMOPTIONS
                            (
                              ORDERITEMOPTION_KEY Integer NOT NULL,
                              ORDERITEM_KEY Integer NOT NULL,
                              OPTION_KEY Integer NOT NULL,
                              IS_PLUS Integer DEFAULT 0,
                              PRIMARY KEY (ORDERITEMOPTION_KEY)
                            );";
            CheckAndCreateTable("ORDERITEMOPTIONS", query);
        }
    }
}