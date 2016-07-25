object frmMain: TfrmMain
  Left = 1318
  Top = 28
  Width = 725
  Height = 398
  BorderIcons = []
  Caption = 'DB Parser'
  Color = 14215660
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object lbeProgress: TLabel
    Left = 8
    Top = 323
    Width = 89
    Height = 20
    Caption = 'Processing'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Memo: TMemo
    Left = 5
    Top = 0
    Width = 484
    Height = 321
    Color = clBlack
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -16
    Font.Name = 'Times New Roman'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 0
  end
  object memMMCreateSQL: TMemo
    Left = 8
    Top = 24
    Width = 481
    Height = 25
    Lines.Strings = (
      
        'CREATE DOMAIN T_TRUEFALSE AS CHAR(1) DEFAULT '#39'F'#39' CHECK((VALUE IS' +
        ' NULL) OR '
      '(VALUE IN ('#39'F'#39','#39'T'#39')));'
      ''
      'CREATE TABLE "ARCBILL" ('
      '  "ARCBILL_KEY" INTEGER NOT NULL PRIMARY KEY,'
      '  "TERMINAL_NAME" VARCHAR(22),'
      '  "STAFF_NAME" VARCHAR(30),'
      '  "TIME_STAMP" TIMESTAMP,'
      '  "TOTAL" NUMERIC(15,4),'
      '  "DISCOUNT" NUMERIC(15,4)'
      ');'
      ''
      'CREATE TABLE "DAYARCBILL" ('
      '  "ARCBILL_KEY" INTEGER NOT NULL PRIMARY KEY,'
      '  "TERMINAL_NAME" VARCHAR(22),'
      '  "STAFF_NAME" VARCHAR(30),'
      '  "TIME_STAMP" TIMESTAMP,'
      '  "TOTAL" NUMERIC(15,4),'
      '  "DISCOUNT" NUMERIC(15,4)'
      ');'
      ''
      'CREATE TABLE "ARCBILLPAY" ('
      '  "ARCBILLPAY_KEY" INTEGER NOT NULL PRIMARY KEY ,'
      '  "ARCBILL_KEY" INTEGER,'
      
        '  FOREIGN KEY ("ARCBILL_KEY") REFERENCES "ARCBILL" ("ARCBILL_KEY' +
        '") ON UPDATE '
      'CASCADE ON DELETE '
      'CASCADE,'
      '  "PAY_TYPE" VARCHAR(50),'
      '  "SUBTOTAL" NUMERIC(15,4)'
      ');'
      ''
      'CREATE INDEX "ARCBILLFKINDEX" ON "ARCBILLPAY" "ARCBILL_KEY";'
      ''
      'CREATE TABLE "DAYARCBILLPAY" ('
      '  "DAYARCBILLPAY_KEY" INTEGER NOT NULL PRIMARY KEY,'
      '  "ARCBILL_KEY" INTEGER,'
      
        '  FOREIGN KEY ("ARCBILL_KEY") REFERENCES "DAYARCBILL" ("ARCBILL_' +
        'KEY") ON '
      'UPDATE CASCADE ON DELETE '
      'CASCADE,'
      '  "PAY_TYPE" VARCHAR(50),'
      '  "SUBTOTAL" NUMERIC(15,4)'
      ');'
      ''
      
        'CREATE INDEX "DAYARCBILLKEYINDEX" ON "DAYARCBILLPAY" "ARCBILL_KE' +
        'Y";'
      ''
      'CREATE TABLE "ARCHIVE" ('
      '  "ARCHIVE_KEY" INTEGER NOT NULL PRIMARY KEY,'
      '  "ARCBILL_KEY" INTEGER,'
      
        '  FOREIGN KEY ("ARCBILL_KEY") REFERENCES "ARCBILL" ("ARCBILL_KEY' +
        '") ON UPDATE '
      'CASCADE ON DELETE '
      'CASCADE,'
      '  "TERMINAL_NAME" VARCHAR(22),'
      '  "MENU_NAME" VARCHAR(15),'
      '  "COURSE_NAME" VARCHAR(25),'
      '  "ITEM_NAME" VARCHAR(40),'
      '  "ITEM_SHORT_NAME" VARCHAR(20),'
      '  "ITEM_ID" INTEGER,'
      '   "ITEM_CATEGORY" VARCHAR(40),'
      '  "SIZE_NAME" VARCHAR(25),'
      '  "TABLE_NUMBER" INTEGER,'
      '  "TABLE_NAME" VARCHAR(25),'
      '  "SEAT_NUMBER" INTEGER,'
      '  "SERVER_NAME" VARCHAR(30),'
      '  "TAB_NAME" VARCHAR(32),'
      '  "LOYALTY_NAME" VARCHAR(30),'
      '  "ORDER_TYPE" INTEGER,'
      '  "TIME_STAMP" TIMESTAMP,'
      '  "TIME_STAMP_BILLED" TIMESTAMP,'
      '  "ORDER_LOCATION" VARCHAR(10),'
      '  "PRICE" NUMERIC(15,4),'
      '  "PRICE_ADJUST" NUMERIC(15,4),'
      '  "COST" NUMERIC(15,4),'
      '  "HAPPY_HOUR" T_TRUEFALSE,'
      '  "NOTE" VARCHAR(80)'
      ');'
      ''
      '  CREATE INDEX "ARCHIVEINDEX" ON "ARCHIVE" '
      '"TIME_STAMP","SERVER_NAME","ITEM_NAME";'
      '  CREATE INDEX "TIMEINDEX" ON "ARCHIVE" "TIME_STAMP";'
      '  CREATE INDEX "COURSENAMEINDEX" ON "ARCHIVE" "COURSE_NAME";'
      '  CREATE INDEX "TABLEINDEX" ON "ARCHIVE" "TABLE_NUMBER";'
      '  CREATE INDEX "DISHNAMEINDEX" ON "ARCHIVE" "ITEM_NAME";'
      '  CREATE INDEX "SEATINDEX" ON "ARCHIVE" "SEAT_NUMBER";'
      
        '  CREATE INDEX "SHORTDISHNAMEINDEX" ON "ARCHIVE" "ITEM_SHORT_NAM' +
        'E";'
      '  CREATE INDEX "SERVERNAMEINDEX" ON "ARCHIVE" "SERVER_NAME";'
      '  CREATE INDEX "TABLENAMEINDEX" ON "ARCHIVE" "TABLE_NAME";'
      '  CREATE INDEX "BILLINGINDEX" ON "ARCHIVE" "ARCBILL_KEY";'
      ''
      'CREATE TABLE "DAYARCHIVE" ('
      '  "ARCHIVE_KEY" INTEGER NOT NULL PRIMARY KEY,'
      '  "ARCBILL_KEY" INTEGER,'
      
        '  FOREIGN KEY ("ARCBILL_KEY") REFERENCES "DAYARCBILL" ("ARCBILL_' +
        'KEY") ON '
      'UPDATE CASCADE ON DELETE '
      'CASCADE,'
      '  "TERMINAL_NAME" VARCHAR(22),'
      '  "MENU_NAME" VARCHAR(15),'
      '  "COURSE_NAME" VARCHAR(25),'
      '  "ITEM_NAME" VARCHAR(40),'
      '   "ITEM_CATEGORY" VARCHAR(40),   '
      '  "ITEM_SHORT_NAME" VARCHAR(20),'
      '  "ITEM_ID" INTEGER,'
      '  "SIZE_NAME" VARCHAR(25),'
      '  "TABLE_NUMBER" INTEGER,'
      '  "TABLE_NAME" VARCHAR(25),'
      '  "SEAT_NUMBER" INTEGER,'
      '  "SERVER_NAME" VARCHAR(30),'
      '  "TAB_NAME" VARCHAR(32),'
      '  "LOYALTY_NAME" VARCHAR(30),'
      '  "ORDER_TYPE" INTEGER,'
      '  "TIME_STAMP" TIMESTAMP,'
      '  "TIME_STAMP_BILLED" TIMESTAMP,'
      '  "ORDER_LOCATION" VARCHAR(10),'
      '  "PRICE" NUMERIC(15,4),'
      '  "PRICE_ADJUST" NUMERIC(15,4),'
      '  "COST" NUMERIC(15,4),'
      '  "HAPPY_HOUR" T_TRUEFALSE,'
      '  "NOTE" VARCHAR(80)'
      ');'
      ''
      '  CREATE INDEX "TABLENAMEINDEX2" ON "DAYARCHIVE" "TABLE_NAME";'
      '  CREATE INDEX "SERVERNAMEINDEX2" ON "DAYARCHIVE" "SERVER_NAME";'
      
        '  CREATE INDEX "SHORTDISHNAMEINDEX2" ON "DAYARCHIVE" "ITEM_SHORT' +
        '_NAME";'
      '  CREATE INDEX "SEATINDEX2" ON "DAYARCHIVE" "SEAT_NUMBER";'
      '  CREATE INDEX "DISHNAMEINDEX2" ON "DAYARCHIVE" "ITEM_NAME";'
      '  CREATE INDEX "TABLEINDEX2" ON "DAYARCHIVE" "TABLE_NUMBER";'
      '  CREATE INDEX "COURSENAMEINDEX2" ON "DAYARCHIVE" "COURSE_NAME";'
      '  CREATE INDEX "TIME2" ON "DAYARCHIVE" "TIME_STAMP";'
      '  CREATE INDEX "ARCHIVEINDEX2" ON "DAYARCHIVE" '
      '"TIME_STAMP","SERVER_NAME","ITEM_NAME";'
      ''
      'CREATE TABLE "DEVICES" ('
      '  "DEVICE_KEY" INTEGER NOT NULL PRIMARY KEY,'
      '  "DEVICE_ID" INTEGER,'
      '  "DEVICE_NAME" VARCHAR(25),'
      '  "TRANSNO" INTEGER,'
      '  "DEVICE_TYPE" INTEGER'
      ');'
      ''
      'CREATE UNIQUE INDEX "DEV_INDEX" ON "DEVICES" "DEVICE_NAME";'
      'CREATE INDEX "IDINDEX1" ON "DEVICES" "DEVICE_ID";'
      ''
      'CREATE TABLE "CONNECTIONS" ('
      '  "CONNECT_KEY" INTEGER NOT NULL PRIMARY KEY,'
      '  "DEVICE_KEY" INTEGER,'
      
        '  FOREIGN KEY ("DEVICE_KEY") REFERENCES "DEVICES" ("DEVICE_KEY")' +
        ' ON UPDATE '
      'CASCADE ON DELETE '
      'CASCADE,'
      '  "CONNECT_NAME" VARCHAR(60),'
      '  "CONNECT_ID" INTEGER,'
      '  "CONNECT_TYPE" INTEGER,'
      '  "DISABLED" T_TRUEFALSE,'
      '  "PC_ACTIVATE_CASHDRAW" T_TRUEFALSE,'
      '  "PALM_ACTIVATE_CASHDRAW" T_TRUEFALSE,'
      '  "ORDER_ACTIVATE_BUZZER" T_TRUEFALSE'
      ');'
      ''
      
        '  CREATE INDEX "IDINDEX" ON "CONNECTIONS" "DEVICE_KEY","CONNECT_' +
        'ID";'
      '  CREATE INDEX "NAMEINDEX" ON "CONNECTIONS" '
      '"DEVICE_KEY","CONNECT_NAME","CONNECT_TYPE";'
      ''
      'CREATE TABLE "MENU" ('
      '  "MENU_KEY" INTEGER NOT NULL PRIMARY KEY,'
      '  "MENU_NAME" VARCHAR(15),'
      '  "MENU_TYPE" INTEGER,'
      '  "DESCRIPTION" VARCHAR(30)'
      ');'
      ''
      'CREATE UNIQUE INDEX "MENU_NAME_INDEX" ON "MENU" "MENU_NAME";'
      ''
      'CREATE TABLE "DEVICESMENUS" ('
      '  "DEVICE_KEY" INTEGER NOT NULL,'
      '  "MENU_NAME" VARCHAR(15),'
      
        '  FOREIGN KEY ("DEVICE_KEY") REFERENCES "DEVICES" ("DEVICE_KEY")' +
        ' ON UPDATE '
      'CASCADE ON DELETE '
      'CASCADE'
      ');'
      ''
      'CREATE UNIQUE INDEX "DEVMENU_INDEX" ON "DEVICESMENUS" '
      '("DEVICE_KEY","MENU_NAME");'
      ''
      'CREATE TABLE "COURSE" ('
      '  "COURSE_KEY" INTEGER NOT NULL PRIMARY KEY,'
      '  "MENU_KEY" INTEGER NOT NULL,'
      
        '  FOREIGN KEY ("MENU_KEY") REFERENCES "MENU" ("MENU_KEY") ON UPD' +
        'ATE CASCADE '
      'ON DELETE CASCADE,'
      '  "COURSE_NAME" VARCHAR(25),'
      '  "COURSE_ID" INTEGER,'
      '  "PALM_COURSE_ID" INTEGER,'
      '  "CAO" INTEGER,'
      '  "ENABLED" T_TRUEFALSE,'
      '  "ORDER_LOCATION" INTEGER'
      ');'
      ''
      'CREATE TABLE "OPTIONS" ('
      '  "OPTIONS_KEY" INTEGER NOT NULL PRIMARY KEY,'
      '  "COURSE_KEY" INTEGER NOT NULL,'
      
        '  FOREIGN KEY ("COURSE_KEY") REFERENCES "COURSE" ("COURSE_KEY") ' +
        'ON UPDATE '
      'CASCADE ON DELETE '
      'CASCADE,'
      '  "PALM_OPTION_ID" INTEGER,'
      '  "OPTION_ID" INTEGER,'
      '  "OPTION_NAME" VARCHAR(28),'
      '  "OPTION_SHORT_NAME" VARCHAR(10),'
      '  "OPTION_ORDER" INTEGER,'
      '  "REDIR_ID" INTEGER,'
      '  "ENABLED" T_TRUEFALSE'
      ');'
      ''
      'CREATE UNIQUE INDEX "OPTION_NAME_INDEX" ON "OPTIONS" '
      '"COURSE_KEY","OPTION_NAME";'
      
        'CREATE UNIQUE INDEX "ID_INDEX" ON "OPTIONS" "COURSE_KEY","OPTION' +
        '_ID";'
      
        'CREATE INDEX "PALM_ID_INDEX" ON "OPTIONS" "COURSE_KEY","PALM_OPT' +
        'ION_ID";'
      ''
      'CREATE TABLE "ITEM" ('
      '  "ITEM_KEY" INTEGER NOT NULL PRIMARY KEY,'
      '  "COURSE_KEY" INTEGER NOT NULL,'
      
        '  FOREIGN KEY ("COURSE_KEY") REFERENCES "COURSE" ("COURSE_KEY") ' +
        'ON UPDATE '
      'CASCADE ON DELETE '
      'CASCADE,'
      '  "ITEM_ID" INTEGER,'
      '  "PALM_ITEM_ID" INTEGER,'
      '  "ITEM_NAME" VARCHAR(50),'
      '  "ITEM_SHORT_NAME" VARCHAR(10),'
      '  "ITEM_CATEGORY" VARCHAR(40),  '
      '  "DISPLAY_SIZES" T_TRUEFALSE,'
      '  "ENABLED" T_TRUEFALSE,'
      '  "IAO" INTEGER,'
      '  "BUTTON_COLOUR" INTEGER,'
      '  "PRINT_CHIT" T_TRUEFALSE,'
      '  "FREE" T_TRUEFALSE'
      ');'
      ''
      'CREATE TABLE "SIZES" ('
      '  "SIZES_KEY" INTEGER NOT NULL PRIMARY KEY,'
      '  "MENU_KEY" INTEGER NOT NULL,'
      
        '  FOREIGN KEY ("MENU_KEY") REFERENCES "MENU" ("MENU_KEY") ON UPD' +
        'ATE CASCADE '
      'ON DELETE CASCADE,'
      '  "SIZE_NAME" VARCHAR(30),'
      '  "SIZE_ID" INTEGER,'
      '  "PALM_SIZE_ID" INTEGER,'
      '  "SAO" INTEGER'
      ');'
      ''
      ''
      'CREATE TABLE "ITEMSIZE" ('
      '  "ITEMSIZE_KEY" INTEGER NOT NULL PRIMARY KEY,'
      '  "ITEM_KEY" INTEGER NOT NULL,'
      
        '  FOREIGN KEY ("ITEM_KEY") REFERENCES "ITEM" ("ITEM_KEY") ON UPD' +
        'ATE CASCADE ON '
      'DELETE CASCADE,'
      '  "SIZES_KEY" INTEGER NOT NULL,'
      
        '  FOREIGN KEY ("SIZES_KEY") REFERENCES "SIZES" ("SIZES_KEY") ON ' +
        'UPDATE CASCADE '
      'ON DELETE CASCADE,'
      '  "ITEM_ID" INTEGER,'
      '  "SIZE_ID" INTEGER,'
      '  "PALM_ITEM_ID" INTEGER,'
      '  "PALM_SIZE_ID" INTEGER,'
      '  "SIZE_NAME" VARCHAR(30),'
      '  "PRICE" NUMERIC(15,4),'
      '  "SPECIAL_PRICE" NUMERIC(15,4),'
      '  "COST" NUMERIC(15,4),'
      '  "ISAO" INTEGER,'
      '  "FREE" T_TRUEFALSE,'
      '  "AVAILABLE_AS_STANDARD" T_TRUEFALSE,'
      '  "NO_RECIPE" T_TRUEFALSE,'
      '  "BARCODE" VARCHAR(80),'
      '  "SETMENU_MASK" INTEGER,'
      '  "ENABLED" T_TRUEFALSE'
      ');'
      ''
      
        'CREATE INDEX "PALM_INDEX" ON "ITEMSIZE" "PALM_ITEM_ID","PALM_SIZ' +
        'E_ID";'
      
        'CREATE UNIQUE INDEX "ITEM_INDEX" ON "ITEMSIZE" "ITEM_ID","SIZE_I' +
        'D";'
      
        'CREATE UNIQUE INDEX "ITEM_KEY_INDEX" ON "ITEMSIZE" "ITEM_KEY","S' +
        'IZES_KEY";'
      ''
      'CREATE TABLE "STOCKCOSTS" ('
      '  "STOCK_CODE" VARCHAR(25) NOT NULL PRIMARY KEY,'
      '  "UNIT" CHAR(16),'
      '  "STOCK_DESC" VARCHAR(50),'
      '  "COST" NUMERIC(15,4)'
      ');'
      ''
      'CREATE TABLE "RECIPE" ('
      '  "RECIPE_KEY" INTEGER NOT NULL PRIMARY KEY,'
      '  "ITEMSIZE_KEY" INTEGER NOT NULL,'
      
        '  FOREIGN KEY ("ITEMSIZE_KEY") REFERENCES "ITEMSIZE" ("ITEMSIZE_' +
        'KEY") ON UPDATE '
      'CASCADE ON DELETE '
      'CASCADE,'
      '  "STOCK_CODE" VARCHAR(20),'
      '  "QTY" NUMERIC(15,4),'
      '  "STOCK_LOCATION" VARCHAR(10)'
      ');'
      ''
      'CREATE TABLE "TAB" ('
      '  "TAB_KEY" INTEGER NOT NULL PRIMARY KEY ,'
      '  "ID_TYPE" VARCHAR(32),'
      '  "ID_NUMBER" VARCHAR(48),'
      '  "ID_EXPIRE" VARCHAR(10),'
      '  "COURSE_NO" INTEGER,'
      '  "TAB_NAME" VARCHAR(32),'
      '  "TAB_TYPE" INTEGER,'
      '  "CREDIT" NUMERIC(15,4),'
      '  "DIVISIONS_LEFT" INTEGER,'
      '  "LOCKED_BY" VARCHAR(22)'
      ');'
      ''
      '  CREATE DESCENDING INDEX "INVERSEKEYINDEX" ON "TAB" "TAB_KEY";'
      '  CREATE INDEX "NAMEINDEX2" ON "TAB" "TAB_NAME";'
      '  CREATE INDEX "IDNUMINDEX" ON "TAB" "ID_NUMBER";'
      ''
      'CREATE TABLE "ORDERS" ('
      '  "ORDER_KEY" INTEGER NOT NULL PRIMARY KEY,'
      '  "ORDERMOVE_KEY" INTEGER,'
      '  "TAB_KEY" INTEGER NOT NULL,'
      
        '  FOREIGN KEY ("TAB_KEY") REFERENCES "TAB" ("TAB_KEY") ON UPDATE' +
        ' CASCADE ON '
      'DELETE CASCADE,'
      '  "ITEM_NAME" VARCHAR(50),'
      '  "ITEM_ID" INTEGER,'
      '  "SIZE_NAME" VARCHAR(25),'
      '  "NOTE" VARCHAR(80),'
      '  "SERVER_NAME" VARCHAR(30),'
      '  "SERVER_INITIALS" CHAR(3),'
      '  "PARTY_NAME" VARCHAR(32),'
      '  "TABLE_NUMBER" INTEGER,'
      '  "TABLE_NAME" VARCHAR(25),'
      '  "SEATNO" INTEGER,'
      '  "PRICE" NUMERIC(15,4),'
      '  "PRICE_ADJUST" NUMERIC(15,4),'
      '  "PRINTED" T_TRUEFALSE,'
      '  "TRANSNO" INTEGER,'
      '  "ORDER_TYPE" INTEGER,'
      '  "TERMINAL_NAME" VARCHAR(22),'
      '  "MENU_NAME" VARCHAR(15),'
      '  "TAB_NAME" VARCHAR(32),'
      '  "COURSE_NAME" VARCHAR(25),'
      '  "HAPPYHOUR" T_TRUEFALSE,'
      '  "ORDER_LOCATION" VARCHAR(10),'
      '  "TAB_TYPE" INTEGER,'
      '  "TIME_STAMP" TIMESTAMP,'
      '  "COST" NUMERIC(15,4),'
      '  "LOYALTY_KEY" INTEGER,'
      '  "LOYALTY_NAME" VARCHAR(30),'
      '  "MASTER_CONTAINER" VARCHAR(25),'
      '  "SETMENU_MASK" INTEGER,'
      '  "SETMENU_GROUP" INTEGER,'
      '  "ITEM_CATEGORY" VARCHAR(40)'
      '/* PALM REQ FIELDS'
      '  "ITEM_ID" INTEGER,'
      '  "COURSE_ID" INTEGER,'
      '  "MASTER_CONTAINER_ID" INTEGER,'
      '  "SIZE_ID" INTEGER*/'
      ');'
      ''
      '  CREATE INDEX "ORDER_PRINTING_INDEX" ON "ORDERS" "COURSE_NAME";'
      '  CREATE INDEX "TRANSINDEX" ON "ORDERS" '
      '"TRANSNO","TERMINAL_NAME","ITEM_NAME","SIZE_NAME";'
      '  CREATE INDEX "CREDITINDEX" ON "ORDERS" '
      '"ITEM_NAME","TABLE_NUMBER","SEATNO","NOTE","ORDER_TYPE";'
      ''
      'CREATE TABLE "ORDERRECIPE" ('
      '  "ORDERRECIPE_KEY" INTEGER NOT NULL PRIMARY KEY,'
      '  "ORDER_KEY" INTEGER,'
      
        '  FOREIGN KEY ("ORDER_KEY") REFERENCES "ORDERS" ("ORDER_KEY") ON' +
        ' UPDATE '
      'CASCADE ON DELETE '
      'CASCADE,'
      '  "STOCK_CODE" VARCHAR(20),'
      '  "UNIT" CHAR(4),'
      '  "QTY" NUMERIC(15,4),'
      '  "COST" NUMERIC(15,4),'
      '  "STOCK_LOCATION" VARCHAR(10)'
      ');'
      ''
      'CREATE TABLE "ORDERMIX" ('
      '  "MIX_KEY" INTEGER NOT NULL PRIMARY KEY ,'
      '  "ORDER_KEY" INTEGER,'
      
        '  FOREIGN KEY ("ORDER_KEY") REFERENCES "ORDERS" ("ORDER_KEY") ON' +
        ' UPDATE '
      'CASCADE ON DELETE '
      'CASCADE,'
      '  "COURSE_NAME" VARCHAR(25),'
      '  "ITEM_NAME" VARCHAR(50),'
      '  "ITEM_ID" INTEGER,'
      '  "SIZE_NAME" VARCHAR(25),'
      '  "PRICE" NUMERIC(15,4),'
      '  "COST" NUMERIC(15,4),'
      '  "PRICE_ADJUST" NUMERIC(15,4)'
      ');'
      ''
      
        '  CREATE INDEX "ORDERMIX_PRIME" ON "ORDERMIX" "ORDER_KEY","MIX_K' +
        'EY";'
      
        '  CREATE INDEX "ORDERMIX_SIZE_ID" ON "ORDERMIX" "ORDER_KEY","SIZ' +
        'E_NAME";'
      
        '  CREATE INDEX "ORDERMIX_ITEM_ID" ON "ORDERMIX" "ORDER_KEY","ITE' +
        'M_NAME";'
      ''
      'CREATE TABLE "ORDEROPTION" ('
      '  "ORDEROPTION_KEY" INTEGER NOT NULL PRIMARY KEY ,'
      '  "ORDER_KEY" INTEGER,'
      
        '  FOREIGN KEY ("ORDER_KEY") REFERENCES "ORDERS" ("ORDER_KEY") ON' +
        ' UPDATE '
      'CASCADE ON DELETE '
      'CASCADE,'
      '  "OPTION_ID" INTEGER,'
      '  "OPTION_NAME" VARCHAR(28),'
      '  "OPTION_SHORT_NAME" VARCHAR(10),'
      '  "REDIR_ID" INTEGER'
      ');'
      ''
      '  CREATE INDEX "ORDEROPTION_INDEX" ON "ORDEROPTION" '
      '"ORDER_KEY","ORDEROPTION_KEY";'
      ''
      'CREATE TABLE "PRNORDER" ('
      '  "PRNORD_KEY" INTEGER NOT NULL PRIMARY KEY,'
      '  "CONNECT_KEY" INTEGER,'
      
        '  FOREIGN KEY ("CONNECT_KEY") REFERENCES "CONNECTIONS" ("CONNECT' +
        '_KEY") ON '
      'UPDATE CASCADE ON '
      'DELETE CASCADE,'
      '  "MENU_KEY" INTEGER,'
      
        '  FOREIGN KEY ("MENU_KEY") REFERENCES "MENU" ("MENU_KEY") ON UPD' +
        'ATE CASCADE '
      'ON DELETE CASCADE,'
      '  "COURSE_KEY" INTEGER,'
      
        '  FOREIGN KEY ("COURSE_KEY") REFERENCES "COURSE" ("COURSE_KEY") ' +
        'ON UPDATE '
      'CASCADE ON DELETE '
      'CASCADE,'
      '  "COURSE_NAME" VARCHAR(25)'
      ');'
      ''
      
        '  CREATE INDEX "COURSEINDEX1" ON "PRNORDER" "CONNECT_KEY","COURS' +
        'E_NAME";'
      
        '  CREATE INDEX "COURSEIDINDEX1" ON "PRNORDER" "CONNECT_KEY","COU' +
        'RSE_KEY";'
      ''
      'CREATE TABLE "STOCKTRACK" ('
      '  "STOCK_KEY" INTEGER NOT NULL PRIMARY KEY,'
      '  "STOCK_CODE" VARCHAR(25),'
      '  "STOCK_DESC" VARCHAR(50),'
      '  "STOCK_LOCATION" VARCHAR(25),'
      '  "UNIT" VARCHAR(16),'
      '  "QTY" NUMERIC(15,4),'
      '  "DEFAULT_LOCATION" VARCHAR(25),'
      '  "DOLLAR_VALUE" NUMERIC(15,4),'
      '  "COST" NUMERIC(15,4)'
      ');'
      ''
      '  CREATE INDEX "STOCKINDEX" ON "STOCKTRACK" "STOCK_CODE";'
      '  CREATE INDEX "DESCRIPTIONINDEX1" ON "STOCKTRACK" "STOCK_DESC";'
      '  CREATE INDEX "STOCKLOCATIONINDEX" ON "STOCKTRACK" '
      '"STOCK_CODE","STOCK_LOCATION";'
      ''
      'CREATE TABLE "ARCHIVESTOCKTRACK" ('
      '  "STOCK_KEY" INTEGER NOT NULL PRIMARY KEY,'
      '  "STOCK_CODE" VARCHAR(25),'
      '  "STOCK_DESC" VARCHAR(50),'
      '  "STOCK_LOCATION" VARCHAR(25),'
      '  "UNIT" VARCHAR(16),'
      '  "QTY" NUMERIC(15,4),'
      '  "DOLLAR_VALUE" NUMERIC(15,4),'
      '  "TIME_STAMP" TIMESTAMP,'
      '  "COST" NUMERIC(15,4),'
      '  "EXPORTED" T_TRUEFALSE,'
      '  "CONFIRMED" T_TRUEFALSE'
      ');'
      ''
      'CREATE TABLE "TABLES" ('
      '  "TABLE_KEY" INTEGER NOT NULL  PRIMARY KEY,'
      '  "TABLE_NUMBER" INTEGER,'
      '  "RELATIVE_X" INTEGER,'
      '  "RELATIVE_Y" INTEGER,'
      '  "RELATIVE_HEIGHT" INTEGER,'
      '  "RELATIVE_WIDTH" INTEGER,'
      '  "X" INTEGER,'
      '  "Y" INTEGER,'
      '  "CIRCLE" T_TRUEFALSE,'
      '  "HEIGHT" INTEGER,'
      '  "WIDTH" INTEGER,'
      '  "TABLE_NAME" VARCHAR(25),'
      '  "PARTY_NAME" VARCHAR(45),'
      '  "COLOUR" INTEGER,'
      '  "TEMPORARY" T_TRUEFALSE,'
      '  "SECTION" VARCHAR(25),'
      '  "DEFAULT_NUMBER_OF_SEATS" INTEGER,'
      '  "DEFAULT_LEFT" INTEGER,'
      '  "DEFAULT_TOP" INTEGER,'
      '  "DEFAULT_SEAT_COUNT" INTEGER'
      ');'
      ''
      
        '  CREATE DESCENDING INDEX "INVERSEKEYINDEX1" ON "TABLES" "TABLE_' +
        'KEY";'
      '  CREATE UNIQUE INDEX "TABLEINDEX3" ON "TABLES" "TABLE_NUMBER";'
      ''
      'CREATE TABLE "SEAT" ('
      '  "SEAT_KEY" INTEGER NOT NULL PRIMARY KEY,'
      '  "TABLE_KEY" INTEGER,'
      
        '  FOREIGN KEY ("TABLE_KEY") REFERENCES "TABLES" ("TABLE_KEY") ON' +
        ' UPDATE '
      'CASCADE ON DELETE '
      'CASCADE,'
      '  "SEATNO" INTEGER,'
      '  "RELATIVE_X" INTEGER,'
      '  "RELATIVE_Y" INTEGER,'
      '  "TEMP" T_TRUEFALSE,'
      '  "TAB_KEY" INTEGER,'
      
        '  FOREIGN KEY ("TAB_KEY") REFERENCES "TAB" ("TAB_KEY") ON UPDATE' +
        ' CASCADE ON '
      'DELETE SET NULL'
      ');'
      ''
      '  CREATE INDEX "TABLEKEY" ON "SEAT" "TABLE_KEY";'
      '  CREATE INDEX "TABINDEX" ON "SEAT" "TAB_KEY";'
      '  CREATE INDEX "SEATNOINDEX" ON "SEAT" "TABLE_KEY","SEATNO";'
      '  CREATE INDEX "SEATTAB_KEY" ON "SEAT" "TAB_KEY";'
      ''
      'CREATE TABLE "USERS" ('
      '  "USER_KEY" INTEGER NOT NULL PRIMARY KEY,'
      '  "USER_TYPE" INTEGER NOT NULL,'
      '  "NAME" VARCHAR(30),'
      '  "PHONE" VARCHAR(30),'
      '  "CELL_PHONE" VARCHAR(30),'
      '  "PHONE_EXT" VARCHAR(6),'
      '  "EMAIL" VARCHAR(30),'
      '  "ADD1" VARCHAR(30),'
      '  "ADD2" VARCHAR(30),'
      '  "ADD3" VARCHAR(30),'
      '  "INITIALS" CHAR(3),'
      '  "USER_ID" INTEGER,'
      '  "PALM_ID" INTEGER,'
      '  "PIN" INTEGER,'
      '  "ACCESS_LEVEL" INTEGER,'
      '  "AUTO_OPTIONS" T_TRUEFALSE,'
      '  "LARGE_FONT" T_TRUEFALSE,'
      '  "ADVANCED_USER" T_TRUEFALSE,'
      '  "DOWNLOADS" INTEGER,'
      '  "TIMEOUT" INTEGER,'
      '  "CARD" VARCHAR(80),'
      '  "CARD_NUMBER" VARCHAR(30),'
      '  "EXPIRY_DATE" DATE,'
      '  "TOTAL_SPENT" NUMERIC(15,4),'
      '  "PAYROLL_ID" VARCHAR(15),'
      '  "START_TIME1" TIMESTAMP,'
      '  "START_TIME2" TIMESTAMP,'
      '  "START_TIME3" TIMESTAMP,'
      '  "START_TIME4" TIMESTAMP,'
      '  "START_TIME5" TIMESTAMP,'
      '  "START_TIME6" TIMESTAMP'
      ');'
      'CREATE UNIQUE INDEX "USERINDEX" ON "USERS" "NAME","USER_TYPE";'
      'CREATE INDEX "USERIDINDEX" ON "USERS" "USER_ID";'
      'CREATE UNIQUE INDEX "USERKEYINDEX" ON "USERS" "USER_KEY";'
      'CREATE INDEX "STAFFCARDINDEX" ON "USERS" "CARD";'
      ''
      'CREATE TABLE "USERSTIME" ('
      '  "USERTIME_KEY" INTEGER NOT NULL PRIMARY KEY,'
      '  "USER_KEY" INTEGER,'
      
        '  FOREIGN KEY ("USER_KEY") REFERENCES "USERS" ("USER_KEY") ON UP' +
        'DATE CASCADE '
      'ON DELETE CASCADE,'
      '  "LOGIN_DATETIME" TIMESTAMP,'
      '  "LOGOUT_DATETIME" TIMESTAMP,'
      '  "ZONE" INTEGER,'
      '  "MODIFIED" T_TRUEFALSE,'
      '  "EXPORTED" T_TRUEFALSE  '
      ');'
      
        'CREATE INDEX "USERINDEX1" ON "USERSTIME" "USER_KEY","LOGIN_DATET' +
        'IME";'
      ''
      'CREATE TABLE "LOGS"'
      '('
      '  "LOGS_KEY" INTEGER NOT NULL PRIMARY KEY,'
      '  "TIME_STAMP"'#9'TIMESTAMP,'
      '  "TERMINAL_NAME"'#9'VARCHAR(22),'
      '  "ERROR_TYPE"'#9'VARCHAR(20),'
      '  "SEVERITY"'#9'VARCHAR(12),'
      '  "ERROR_MSG"'#9'VARCHAR(200),'
      '  "FUNCTION_NAME"'#9'VARCHAR(60)'
      ');'
      ''
      'CREATE TABLE "PALMLOGS"'
      '('
      '  "LOGS_KEY" INTEGER NOT NULL PRIMARY KEY,'
      '  "TIME_STAMP"'#9'TIMESTAMP,'
      '  "TERMINAL_NAME"'#9'VARCHAR(22),'
      '  "ERROR_TYPE"'#9'VARCHAR(20),'
      '  "SEVERITY"'#9'VARCHAR(12),'
      '  "ERROR_MSG"'#9'VARCHAR(200),'
      '  "FUNCTION_NAME"'#9'VARCHAR(60)'
      ');'
      ''
      'CREATE TABLE "MESSAGES" ('
      '  "MESSAGES_KEY" INTEGER NOT NULL PRIMARY KEY,'
      '  "APPERANCE_ORDER" INTEGER,'
      '  "TITLE" VARCHAR(15),'
      '  "MESSAGE_TEXT" VARCHAR(200),'
      '  "MESSAGE_TYPE" INTEGER'
      ');'
      ''
      'CREATE TABLE "VERSIONHISTORY"'
      '('
      '  "VERSION_NUMBER" VARCHAR(20) NOT NULL PRIMARY KEY,'
      '  "TIME_STAMP"'#9'TIMESTAMP,'
      '  "COMMENT"'#9'VARCHAR(60)'
      ');'
      ''
      'INSERT INTO VERSIONHISTORY ('
      '   VERSION_NUMBER,'
      '   TIME_STAMP,'
      '   COMMENT)'
      'VALUES ('
      '   '#39'2.1.18.0'#39','
      '   '#39'2002-01-22 12:00:00'#39','
      '   '#39'Initial Install'#39');'
      ''
      
        'COMMIT WORK;                                                    ' +
        '                                                  '
      ''
      '  CREATE GENERATOR "GEN_ARCBILL";'
      '  SET GENERATOR "GEN_ARCBILL" TO 0;'
      ''
      '  CREATE GENERATOR "GEN_ARCBILLPAY";'
      '  SET GENERATOR "GEN_ARCBILLPAY" TO 0;'
      ''
      '  CREATE GENERATOR "GEN_ARCHIVE";'
      '  SET GENERATOR "GEN_ARCHIVE" TO 0;'
      ''
      '  /* Remove this */'
      '  CREATE GENERATOR "GEN_CAPACITY";'
      '  SET GENERATOR "GEN_CAPACITY" TO 0;'
      ''
      '  CREATE GENERATOR "GEN_CONNECTIONS";'
      '  SET GENERATOR "GEN_CONNECTIONS" TO 0;'
      '   /* Remove this */'
      '  CREATE GENERATOR "GEN_CUSTOMER";'
      '  SET GENERATOR "GEN_CUSTOMER" TO 0;'
      ''
      '  CREATE GENERATOR "GEN_DAYARCBILL";'
      '  SET GENERATOR "GEN_DAYARCBILL" TO 0;'
      ''
      '  CREATE GENERATOR "GEN_DAYARCBILLPAY";'
      '  SET GENERATOR "GEN_DAYARCBILLPAY" TO 0;'
      ''
      '  CREATE GENERATOR "GEN_DAYARCHIVE";'
      '  SET GENERATOR "GEN_DAYARCHIVE" TO 0;'
      ''
      '  CREATE GENERATOR "GEN_DEVICES";'
      '  SET GENERATOR "GEN_DEVICES" TO 0;'
      ''
      '  CREATE GENERATOR "GEN_ITEM";'
      '  SET GENERATOR "GEN_ITEM" TO 0;'
      ''
      '  CREATE GENERATOR "GEN_COURSE";'
      '  SET GENERATOR "GEN_COURSE" TO 0;'
      ''
      '  CREATE GENERATOR "GEN_OPTION";'
      '  SET GENERATOR "GEN_OPTION" TO 0;'
      ''
      '  CREATE GENERATOR "GEN_RECIPE";'
      '  SET GENERATOR "GEN_RECIPE" TO 0;'
      ''
      '  CREATE GENERATOR "GEN_ITEMSIZE";'
      '  SET GENERATOR "GEN_ITEMSIZE" TO 0;'
      ''
      '  CREATE GENERATOR "GEN_SIZE";'
      '  SET GENERATOR "GEN_SIZE" TO 0;'
      ''
      '  CREATE GENERATOR "GEN_ORDEROPTION";'
      '  SET GENERATOR "GEN_ORDEROPTION" TO 0;'
      ''
      '  CREATE GENERATOR "GEN_MENU";'
      '  SET GENERATOR "GEN_MENU" TO 0;'
      ''
      '  CREATE GENERATOR "GEN_ORDERMIX";'
      '  SET GENERATOR "GEN_ORDERMIX" TO 0;'
      ''
      '  CREATE GENERATOR "GEN_ORDERS";'
      '  SET GENERATOR "GEN_ORDERS" TO 0;'
      ''
      '   /* Remove this */'
      '  CREATE GENERATOR "GEN_RESERVATIONS";'
      '  SET GENERATOR "GEN_RESERVATIONS" TO 0;'
      '   /* Remove this */'
      '  CREATE GENERATOR "GEN_RESERVETABLE";'
      '  SET GENERATOR "GEN_RESERVETABLE" TO 0;'
      ''
      '   /* Remove this */'
      '  CREATE GENERATOR "GEN_STOCK";'
      '  SET GENERATOR "GEN_STOCK" TO 0;'
      ''
      '  CREATE GENERATOR "GEN_STOCKTRACK";'
      '  SET GENERATOR "GEN_STOCKTRACK" TO 0;'
      ''
      '  CREATE GENERATOR "GEN_ARCHIVESTOCKTRACK";'
      '  SET GENERATOR "GEN_ARCHIVESTOCKTRACK" TO 0;'
      ''
      '  CREATE GENERATOR "GEN_TAB";'
      '  SET GENERATOR "GEN_TAB" TO 0;'
      ''
      '  CREATE GENERATOR "GEN_TABLES";'
      '  SET GENERATOR "GEN_TABLES" TO 0;'
      ''
      '  CREATE GENERATOR "GEN_USERS";'
      '  SET GENERATOR "GEN_USERS" TO 0;'
      ''
      '  CREATE GENERATOR "GEN_USERSTIME";'
      '  SET GENERATOR "GEN_USERSTIME" TO 0;'
      ''
      '  CREATE GENERATOR "GEN_SEAT";'
      '  SET GENERATOR "GEN_SEAT" TO 0;'
      ''
      '  CREATE GENERATOR "GEN_PRNORDER";'
      '  SET GENERATOR "GEN_PRNORDER" TO 0;'
      ''
      '  CREATE GENERATOR "GEN_PCINTERNALTRANSNUMBER";'
      '  SET GENERATOR "GEN_PCINTERNALTRANSNUMBER" TO 0;'
      ''
      '  CREATE GENERATOR "GEN_CHITNUMBER";'
      '  SET GENERATOR "GEN_CHITNUMBER" TO 0;'
      ''
      '  CREATE GENERATOR "GEN_MENUID";'
      '  SET GENERATOR "GEN_MENUID" TO 0;'
      ''
      '  CREATE GENERATOR "GEN_SETMENUGROUP";'
      '  SET GENERATOR "GEN_SETMENUGROUP" TO 0;'
      ''
      '  CREATE GENERATOR "GEN_LOGSKEY";'
      '  SET GENERATOR "GEN_LOGSKEY" TO 0;'
      ''
      '  CREATE GENERATOR "GEN_MESSAGES";'
      '  SET GENERATOR "GEN_MESSAGES" TO 0;')
    TabOrder = 1
    Visible = False
  end
  object barProgress: TProgressBar
    Left = 8
    Top = 344
    Width = 697
    Height = 16
    BorderWidth = 3
    Min = 0
    Max = 100
    Smooth = True
    Step = 1
    TabOrder = 2
  end
  object TouchBtn1: TTouchBtn
    Left = 504
    Top = 8
    Width = 201
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    ParentColor = True
    Caption = 'Begin Update'
    ButtonColor = clNavy
    LatchedColor = clBtnFace
    DisabledButtonColor = clBtnFace
    LatchingProperties = [lkLatchColor, lkStayDown]
    OnMouseClick = TouchBtn1MouseClick
  end
  object TouchBtn2: TTouchBtn
    Left = 504
    Top = 256
    Width = 201
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    ParentColor = True
    Caption = 'Close'
    ButtonColor = clMaroon
    LatchedColor = clBtnFace
    DisabledButtonColor = clBtnFace
    LatchingProperties = [lkLatchColor, lkStayDown]
    OnMouseClick = btnCloseClick
  end
  object diaOpen: TOpenDialog
    Left = 496
    Top = 200
  end
end
