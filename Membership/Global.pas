unit Global;

   { 65,97  :  // A         78,110 :  // N
     66,98  :  // B         79,111 :  // O
     67,99  :  // C         80,112 :  // P
     68,100 :  // D         81,113 :  // Q
     69,101 :  // E         82,114 :  // R
     70,102 :  // F         83,115 :  // S
     71,103 :  // G         84,116 :  // T
     72,104 :  // H         85,117 :  // U
     73,105 :  // I         86,118 :  // V
     74,106 :  // J         87,119 :  // W
     75,107 :  // K         88,120 :  // X
     76,108 :  // L         89,121 :  // Y
     77,109 :  // M         90,122 :  // Z

Pauanui Card Rankings
1 Standard
2 Golf
3 9 & 10 yr Standard
4 9 & 10 yr Golf
5 Sports Player
6 Sports Player Golf
   }
interface

uses
  Graphics,SysUtils,Windows, WinSpool,Classes,Controls,Dialogs,Messages,Forms,IBQuery,IBTable,DB
  ,Printers,Registry,dmMembers,IBServices,IBCustomDataSet,comctrls, StdForm,stdctrls,Wwdbcomb,
  MemberMenu,extctrls,ClipBrd,rIndividualMember,IBDataBase,Setup;

  { **************************************************************************** }
  type
    TBeforeEditPointsCredit = record
       beTotalEarnt      : Extended;
       beTotalRedeemed   : Extended;
       beBonusCredit     : Extended;
       beEarntCredit     : Extended;
       bePrePaidCredit   : Extended;
       bePointsAvailable : Extended;
  end;            
  TPointsCreditTypes = (pcAll,pcMaintenance,pcSetup,pcSubPaidOffice,pcCredit,pcLucky,pcSubPaidPOS,pcPointsEarnt,pcPointsSpent,pcReset,pcDeletedMember,pcCardLastUsed );

  var  PointsCreditTypes : TPointsCreditTypes;
  
  
  const
    // NB last 3 are not put in Points Log as Yet
    MaxPointsCreditTypes = 10; 
    PointsCreditNames : Array[0..MaxPointsCreditTypes] of String[37]
             = ( 'All Types','Maintenance Credit/Points (Office)','Bonus Points (Office)','Subs Paid (Office)','VIP Credit (POS)','Lucky Points (POS)','Subs Paid (POS)','Points Earnt (POS)','Points Spent (POS)','Reset Credit/Points(Office)','Deleted Member');
      
 { **************************************************************************** }
{ ****************************************************************************}
  type
  THeightWidth = record
    frmHeight : Integer;
    frmWidth  : Integer;
  end;
  { **************************************************************************** }
// Process types for the Process Log
type
   TProcessTypes = (ptAll,ptMembers,ptMembersLinks,ptMembersActive,ptRollover,ptMaintain,ptSystem,ptEmail,ptPOS);
const
   MaxProcessTypes = 8;
   ProcessTypes : Array[0..MaxProcessTypes] of String[14]
             = ( 'All','Members','Members Links','Members Active','Rollover','Maintain','System','Email','POS');
{ **************************************************************************** }
// see frmMembersEDIT - makes up the full_name from these fields
const
   MaxNameTypes = 3;
   NameTypes : Array[0..MaxNameTypes] of String[10]
             = ( 'Title','Initials','FirstName','Surname');
{ **************************************************************************** }
// General useage
const
   MaxFieldNames = 3;
   FieldNameArray : Array[0..MaxFieldNames] of String[12]
             = ( 'SUBSCRIPTION','SECTION','EVENT','GROUPS');
   FieldTableArray : Array[0..MaxFieldNames] of String[12]
              = ( 'M_SUBS','M_SECTIONS','M_EVENTS','M_GROUPS');
   FieldNamesArray : Array[0..MaxFieldNames] of String[13]
             = ( 'SUBSCRIPTIONS','SECTIONS','EVENTS','GROUPS');
   SearchDesc : Array[0..4] of string[12]
             = ('Member','Event','Group','Section','Subscription');
{ **************************************************************************** }
// Importing from AREV Overnight - tenders are CASH, CHEQUE  etc   frmMemberSYS
//type
//   TTenderImportTypes = (tiUnKnown,tiCash,tiCheque,tiEftpos);
//const
//   MaxTenderImportTypes = 3;
//   TenderImportTypes : Array[0..MaxTenderImportTypes] of String[14]
//             = ( 'UNKNOWN','CASH','CHEQUE','EFTPOS');

{ **************************************************************************** }
{ Importing from AREV Overnight - frmMemberSYS
   ERNT - points earnt
   CRED - credit to a member - ie big tab
   SPNT - points spent
   SUBS - pay by subs - need to know which one
   ZZZZ - last line
}
type
   TPrefixImportTypes = (piError,piPointsEarnt,piCredit,piPointsSpent,piSubsPaid,piSectionPaid,piJoinFeePaid,piBirthdayEvent,piLuckyMember,piTotal);
const
   MaxPrefixImportTypes = 9;
   //  invalid
   PrefixImportTypes : Array[0..MaxPrefixImportTypes] of String[4]
             = ( 'ERRR','ERNT','CRED','SPNT','SUBS','SECT','JOIN','EVNT','LUCK','ZZZZ');

{ **************************************************************************** }
// Subs Payments for Members SUB_PAID  values 0,1,2
// NB spPart and spFull are treated as Financial Members
type
   TSubPayTypes = (spZero,spPart,spFull);
{ **************************************************************************** }
type
   TYesNoTypes = (ynNo,ynPart,ynYes);  // same order as Sub_Paid
const
   YesNoTypesArray  : Array[0..2] of String[4]
                   = ( 'No','Part','Yes');
   FinancialArray  : Array[0..2] of String[14]
                   = ( 'Non Financial','Part Financial','Financial');

{ **************************************************************************** }
type
   TNumberTypes = (ntNextNumber,ntBlankNumber,ntNonActive,ntNonFinancial,ntNonFinancialNoPoints);
var
   TPaymentCardTypes    : (pcPrint,pcSave,pcBulk);
   TPaymentReceiptTypes : (prPrint,prSave,prBulk,prLetter);
   DefaultNumberTypes  : TNumberTypes;
   CurrentNumberTypes  : TNumberTypes;

{ **************************************************************************** }

// PAY_TYPES in PAYMENTS_DET db
type
   TPayTypes = (ptSubs,ptSections,ptJoinFee);
const
   PayTypesArray  : Array[0..2] of String[13]
                   = ( 'Subscriptions','Sections','Joining Fees');
{ **************************************************************************** }
// Exporting of Members Info to Arev
type
   TExportTypes = (etAdd,etChange,etDelete);
const
   ExportTypesArray  : Array[0..2] of String[3]
                   = ( 'ADD','CHG','DEL');
{ **************************************************************************** }
const
   MaxPrinterTypes = 3;
   PrinterTypes : Array[0..MaxPrinterTypes] of String[16]
             = ( 'Report Printer','Receipt Printer','Card Printer','Cash Drawer');
{ **************************************************************************** }
var
   CommPortsTypes : (cpNone,cpCom1,cpCom2,cpCom3,cpCom4,cpCom5,cpCom6);
//const
   //MaxCommPorts = 6;
   //CommPortsArray : Array[0..MaxCommPorts] of String[4]
   //          = ( 'None','Com1','Com2','Com3','Com4','Com5','Com6');
{ **************************************************************************** }
const
   MaxEmulations = 2;
   EmulationTypes : Array[0..MaxEmulations] of String[5]
             = ( 'None','Epson','Star');
{ **************************************************************************** }
const
   StateNames : Array[0..3] of String[8]
             = ( 'Inactive','Browse','Edit','Adding');
{ **************************************************************************** }
var

   PrintLogo       : string;

   // MemberShip only system - is set when the app starts
   MemberSystem   : Boolean;
   // Connected to the Test DB
   InTestMode     : Boolean;

   // read from registry -
   regServerName     : String = '';
   regDatabaseName   : String = '';
   regMenuMateServerName     : String = '';
   regMenuMateDatabaseName   : String = '';
   regLogOn          : Boolean = False;
   regLocalUserName  : string = 'DEFAULT';

   // user details - Name = Default, Password = User in a non-log-on system
   // are loaded when the app starts
   UserCode      : Integer = 0;
   UserName      : String = 'DEFAULT';
   UserPassword  : String = 'USER';
   HighestUserCode : SmallInt = 0;

   // site details - are loaded when the app starts
   CompName       : String;
   SiteLicense    : LongInt;
   MemberLicense  : string;
   PosLicense     : string;
   Address1       : string[30];
   Address2       : string[30];
   Address3       : string[30];
   PostCode       : string[8];
   Phone          : string[20];
   GSTNo          : string[10];
   GSTRate        : extended;
   HelpDesk       : string[20];
   Fax            : string[20];

   // Misc details - are loaded when the app starts
   MemberExpiry   : TDateTime;
   MemberPreExpiry : SmallInt;
   ExportSeconds,SwipeCardLength,MaxSub,MaxGroup,MinSub,MinGroup : Integer;
   KeepInactive,TopLineFeed,BottomLineFeed   : SmallInt;
   RevolveDays,SectionRevolveDays,MaxEmailMembers    : SmallInt;
   NameFields,CheckedFields,SwipeCardText     : string[45];
   ManBackupEnabled,PrintReceiptAsText,ClearInterestsFlag  : Boolean;
   AutoBackupEnabled : Boolean;
   AutoBackupDate ,ManLastBackup ,AutoBackupTime  : TDateTime;
   AutoBackupPCName,ControllerPCName,PHMControllerPCName,ArevImportFilePath,ArevSecondFilePath ,ManBackupPCName,PHMPath,VIPCreditName : String;
   EditCapitals,DisableServices,ExportSubs,CheckForSubs,CheckForGroups,MenuMateDiscountsInstalled : Boolean;
   SubsGroupDelete,ParentCodeAddressChanged,PayBeforeExpires,PointsPassword,PrintMemberPayment,MenuMateTriggerInstalled,MenuMateInstalled,PHMInstalled,SwipeCardOnInsert,KnownAsDefaulted  : Boolean;
   DefaultCardID    : ShortInt;
      //Discount2,Discount3 : Extended;
   PaymentGrids, CardDefault,ReceiptDefault,ReceiptType : String[3];
   ConfirmPayment,FirstPaymentNonActive,POSPaymentCardReqd : Boolean;
   CardEncodeMemberOnly : String[1];
   
   // Fixed or Revolving Member Expiry Dates - if no date entered in MEMBER_EXPIRY tbl MISC
   FixedExpirySystem : Boolean;
   arrayUserNames : array[0..30] of string[30];
   arrayUserCodes : array[0..30] of shortint;
   CountArrayUserNames : shortint = 0;

   // screen difference ratio
   DiffRatio  :  extended;
   // Color Palette 256 = 8, 65536 = 16, True = 32 ( dont show gradient on 256 color
   SysInfoColorPalette : ShortInt;
   // LastFileExported  from proc CreateAsciifile
   LastFileExported    : string;
   // for Card Printing
   ReceiptPrinterIndex : SmallInt;
   ReportPrinterIndex  : SmallInt;
   CardPrinterIndex    : SmallInt;
   CashDrawerPrinterIndex    : SmallInt;

   CardPrinterName,ReceiptPrinterName,CashDrawerPrinterName,CashDrawerStr  : ShortString;
   // for inserting new members - different starting points - tblNumbers
   NextMemberIndex  : shortint;
   ToolbarMoved     : Boolean;
   ToolbarSaveTop   : Smallint;
   CardFieldStr     : String;
   LastMemberInMaintenance : String[12]; // transfer of Maintenence to Payments
   // used to notify Stockmaster for a Reset Points/Credit if it was an Non Financial
   // or Non Active Member and they chose to reuse it
   ReUsingMember    : Boolean;
   
   { **************************************************************************** }
   varYes : string[1] = 'Y';
   varYesWord  : string[3] = 'YES';
   varNo  : string[1] = 'N';
   varNoWord  : string[2] = 'NO';
   { **************************************************************************** }
const
   { General const }
   //StockMaster      =  'StockMaster';
   Membership       =  'Membership';
   BusinessLogo     =  'Enterprise Retail Solutions';
{   Check Card Encoding and reading for City Club
   Member Information Report - Multi Select of Members States eg NonFinancial and Active - Pauanui
   Default Member Groups/Subs - cannot focus a disabled or invisible Window
   Refresh on Links - description not coming thru - frmMembersEDIT
   Subs Information Rpt - not passing thru Active/ Non active etc
   ***************************************************
   DATABASE CHANGE + grant
   alter table members   and members_def
   alter SPARE_INT to CLUBS,
   alter SPARE_CHAR_1 to ASSOCIATION
   CREATE TABLE "CLUBS"
   (
   "CLUB_ID"	SMALLINT NOT NULL,
   "DESCRIPTION"	VARCHAR(60),
    PRIMARY KEY ("CLUB_ID")
   );
   DECEASED set to 'N' on before POST - queries with 'and DECEASED <> 'Y' dont get null members
   ( apply query to update DECEASED to N where DECEASED is null )
   Members Information Report - code to 5 digits and Address to 8 font size
   Added in Home Club and Golf Association in frmMEMBERSMaint
   Added Report for Clubs
   Export Subs Due by Groups now Works
   Fix for Subs Due Letter - SUBS_PAID < 1 removed
   Fix CLUB instaed of CLUBS
   Changes made to the Clubs report and setp
   drop down to 1st record in clubs
   Removal of groups for GOLF result in the Home club being removed as well - notified to user
   Auto Backups date problem fixed
   Auto Backups delete of old revolving filenames fixed
   Payments - can key in a Points Value - Commerce Club
   Voting List - Page Numbers on the bottom
   Fix - Removal of groups for GOLF result in the Home club being removed as well - notified to user
   Upper Hutt - Birthday's not there in DB + event
   Section Information Report - Add Adresses and make landscape
   ***************************************************
   DATABASE CHANGE - Printers
   alter table printers    add EMULATION smallint
   alter table printers    alter  PRINTER_TYPE TO COMM_PORT
  ***** insert into printers    - new fields for CASH_DRAWER

   Points Used on the Receipt Report
   Payments Points Used > payment Amount fixed
   Fixed Access Violation bug in MembersLETTER - adding a Query
   Direct Debits Changes
   Proposer / Seconder check removed
   bug in printing cards
   Find on Clubs - Pauanui
   Members to Payments - when adding a Member - make this change in qryMembersLook and
      pass this tru to Payments for a quick entry
   Upper Hutt - Subs Paid not active or Financial on the First Member
   ***************************************************
   DATABASE CHANGE - CLUBS
   CREATE  INDEX DESC_INDEX ON CLUBS (DESCRIPTION);
   ***************************************************
   Index on CLUBS on Description - Pauanui
   debug fix - left in before M_GROUPS delete sm
   Auto Print after save from Direct Debits
   Add in of first_name_surname in Print Cards concat of fields / fix of first_name
   Warning Message on saving of Direct Debits
   Direct Dedit report alignments , add in Receipt
   ***************************************************
   DATABASE CHANGE - MISC
    alter table misc add POS_PAY_CARD_REQD char(1)   // Oxford

   Card is not required when making a payment thru the POS
   Oxford Changes
   Export Subs Due not Fixed
   Receipt Printer changes
   Card Changes - desc
   frmPayments.Create(self)  not nil
   trouble with windows menu having frmPayments already as a component
   not printing more than one payment in the new print from file Receipt
   ***************************************************
   DATABASE CHANGE - MISC
    alter table misc add BOTTOM_LINE_FEED smallint,
    alter table misc add TOP_LINE_FEED smallint;
       add CHECKED_FIELDS varchar(45);


    Members Brief Edit screen from Payments
    Field Watch for Payments
    Deactivate on forms - speeded up hopefully
    ****************************************************************************
   DATABASE CHANGE - MISC
   CARD TYPE to CARD_TYPE
   Changes to Club Lookup
   Changes to M_Interests Lookup
   Changes to going into Sytem and GetUserNAmes - dont make tsLogs as the first control
   Changes to Logon - dont see the tables
   Changes to frmMembersEdit - opentables done there now - Commit and StartTrans on form Act
   Allows you to back out of the ExportTimer while processing Customers
   No IBTransactionCommit anymore - commited with IBTransaction from frmMembers
      which is the most used IBTransaction form
   Cant add a Member which is Below 1;
   . on blanks for the Printer

   Print card to Yes - payments
    ****************************************************************************
   DATABASE CHANGE - MISC
   PAY_BEFORE_EXPIRES char
   CARD_DEFAULT  char
   RECEIPT_DEFAULT char

   Change of Subs
   F6 & F9 in Payments
   Default Card and Default Receipt into Misc  - need to fill in on site upgrades
   MembersMAINT find button
   Lookup - coloured line on grid
    ****************************************************************************
   DATABASE CHANGE - MISC
   EXPORT_SUBS char
   add DEFAULT_NUMBER smallint
   Fix on Next Member Number - doesnt update when it not used - user overrides the Number
   Cant put a Space in the description field of tblNumbers - loads as a Menu Name
   Checks to see if the Member is not there while inserting from dbeMember onkeypress
   Added Receipt Number and Fullname to the receipt
   Member Numbers - Default Number and overrides for Next Number,Non Fin, Non Active & Blank Number
   1.11.02 Receipt Report,Section Analysis,Group Analysys - Open on tblGroups/tblSections
   1.11.02 Locate on qryMembersLook frmActivate if there are many records it returns you to the place
           where it left off after the commit
   1.11.02 Test DataBase installed
   1.11.02 Voting List Including First Name
   1.11.02 Discount not Reset after making a Payment
   1.11.02 receipt Report - included the Discount Column
   1.11.02 Discount not clearing off payments form
   1.11.02 Capitals setup moved from stdform to membersEDIT
   1.11.02 Maximise enlarges the font and component size
   1.11.02 POINTS_AVAILABLE and VALUE_OF_CREDIT now as calc Fields
   ****************************************************************************
   DATABASE CHANGE - MISC - POINTS_PASSWORD
   ****************************************************************************
   1.12.00 Members EDIT - Password on Points if Points Password is True
   ****************************************************************************
   DATABASE CHANGE - New DB - POINTS_LOG
   POINTS and Credit Fields to DOUBLE PRECISION
   MEMBERS alter SPARE_VARCHAR_20 to REASON;
   ALTERED char to Varchar wher char > 5
   Dropped Constraints on MEMBERS - GROUPS and MAIL_Codes
   Used MEMBERS - GROUPS to numeric - renamed to PAYMENT_DISC
   Subscriptions - added TERM
   
   ****************************************************************************
   1.13.00 Points Passwords
   1.13.00 Points in Settings
   1.13.00 Points in Maintenance
   1.13.00 IBRollover is now Read Commited - Daily Jobs and Rollover was Table Stability
   1.13.00 Direct Debits removed Payment Day - used for part Pay
   1.13.00 Made Char's > 5 into a varchar in Members - easier editing
   1.13.00 Payments - Part Pay ,Direct Debit and Pro Rata, Discounts
   1.13.00 Member Information Report - male female totals are correct - Pauanui
   1.13.00 Maintenance - Points Reset
   1.13.00 Points Rollover
   1.13.00 Subscriptions - TERM eg 3 9 or 12 month - Woolston          
   1.13.00 Points from Daily Jobs + Subs Payments
   1.13.00 Fix Member Information Report male Female Totals initialized
   1.13.00 Member Ages Report - new one
   1.13.00 New Member by Joined date - new one
   1.13.00 Timaru only 11 digit Member card encoding
   1.13.01 Deleted members with poinst or credit go onto the points audit log
   1.13.01 Use un Financial Members with no points - Members Maintenance
   ****************************************************************************
   DATABASE CHANGE - MISC - added Mailing Label Margins
   ****************************************************************************
   1.14.00 Flexible mailing Labels
   1.14.01 Card field changes eg known_as_surname - 2 for Oxford
   1.14.01 Bulk Points fix
   1.14.02 Minor Changes for Richmond
   1.14.02 Hornby - Delete of Temp files upon entry
   1.14.02 papaunui - Email all Members a file 
   ****************************************************************************
   DATABASE CHANGE - MISC - added Font_Size  smallint
   ****************************************************************************
   1.15.00 added font size to mailing labels
   1.15.00 New Report Members List
   1.15.01 Report Members List fix on free
   1.15.01 Member Points Report now reflects financial status
   1.15.01 Export Timer Set to 1 minute from 15 seconds - doesnt clash on NT PC's
   1.15.01 RPNYC - Direct Debit minor Changes - finish
   ****************************************************************************
   DATABASE CHANGE - MISC - Payments - changed FullName to Varchar(40)
                           DROP TRIGGER "PAYMENTSTRIGGER1" 
   ****************************************************************************
   1.16.00 Hornby - Payments - GetReceiptNumber - get max Payment then tries to insert it
                    went away from a trigger/Generator as this doesnt rollback properly
   1.16.00 Export Timer Set to 1 minute to 15 seconds ( from Papanui )
   1.16.00 Papatoetoe - Allow_Credit Default is now Set in Maintenance
   1.16.00 Pauanui - rSubAnalysis - not reflecting 0.00 subs as Paid 
   1.16.00 Added new splash screen from Brad
   1.16.01 Added Parent Code Member Report - for Pauanui
   1.16.01 Export Subs Due - a delete then next problem fixed - RPNYC
   1.16.01 Payments - setfocus on Find
   1.16.02 Points Rollover
   1.16.03 Receipt Report - change to add on Tender Name - missed out Points Payments
   1.16.03 Events Report - added Financial
   1.16.03 Minimize Maximise fix
   1.16.03 Fix Note 1 - Members Rollover to get Financial/Part Financial Members
   1.16.03  Members Rollover updated the FEE in M_SUBS for Subs Analysis Rpt
   1.16.03  Payments - fixed up points payments - can pay more than what they have 
   1.16.03  Payments - Late Sub added and removed 
   ****************************************************************************
   DATABASE CHANGE - NEW Table - IMPORT_POINTS
   ****************************************************************************
   1.17.01  Richmond - export of code kown as etc fix
   1.17.01  Added SQL Paste from Clipboard
   1.17.01  Fix to Subs Due Letter - still problems with Rich Text fields \member\ 
   1.17.01  Adding a late Fee - does not get a card
   1.17.02  - Members < 4 digits must go out with leading zeros - Papatoetoe
   1.17.02   can delete DOB in M_EVENTS - export required M_EVENTS DOB not MEMBERS DOB - Howick
   1.17.02  Invoice with Comp Name on it except for Pauanui - for TePuke
   ****************************************************************************
   DATABASE CHANGE - 1.18.00
   ****************************************************************************
   1.18.00  Cards for Hornby Specifically from  1 to 9 go to 01 to 09
   1.18.00  Non Financial Seach included Normal and SUB_PAYMENT = Y - not a new member
   1.18.00  Send one/all members to EXPORT_AREV - frmMembersEdit
   1.18.01  Option for no Encoding on Cards - frmMembersSYS
   1.18.01  Implemented Points Audit Report
   1.18.01  Rollover - Members with Keep Expires Current get
    exported to keep Clubmaster uptotdate 
   1.18.01  Global - gridLoadPrinters - remove ON LPT 
   1.18.02  Bulk Delete of Non Financial Members
   1.18.02  Invoice Year Dates to 60 days from 30
   1.18.02  Group Analysis Report by Financial Members   
   1.18.02  Oamaru Fix for Financial
   1.18.03  Palmerston north has no Subs get the correct one for the Member - Daily Jobs
   1.18.04  fix for Subs paid at POS - PaymentDetailInsert
   1.18.04  fix top 50 member report
   1.18.04  fix auto backup enabled 
   1.18.04  fix add on Member to export subs file to Clubmaster
   1.18.04  fix add Sub with group already there
   1.18.04  add ExportSeconds for the Export Timer
   1.18.04  add Save on Site/Misc/Registry
   1.18.04  add Print of Member Details from Maintenance // Hornby
   1.18.04  fix System/Printers - change Printer save
   1.18.05  Cards and Export Member with member # prefilled with zeros
   ****************************************************************************
   DATABASE CHANGE - 1.19.00  in the New Release Folder
   ****************************************************************************
   1.19.00 POINTS_AVAILABLE now a back to a float field away from a calc field
   1.19.00 Maint - Bonus Points - Fields are now rounded
   1.19.00 Payments -  MemberDeleteReport after printing
   1.19.00 Check for DOB in Maintenance Screen
   1.19.01 Export Cards in Global
   1.19.02 MembersSYS removed Audit Report - if MemberPA <> ImportPA then 'CARD_LAST_USED' := Date;
   1.19.03 Global - Card Print RPNYC print of Life Members with no expires date
   1.19.06 Makerewa - hard code to add $10 worth of points if paid around subs time
   1.19.06 Fix - Closed TblSubs in Members Maint
   1.19.06 Fix - Empty Daily Import File - notify then delete it
   
   1.19.06 Fix - Receipt Report Upper Hutt - Range Check Error - to many Groups
   1.19.06 Fix - Receipt Report Upper Hutt - Range Check Error - to many Groups
   1.19.06 New Members POints Report - looks like Stockmaster one - Hornby
   
   1.19.07 Makerewa - hard code to add $10 worth of points - REMOVED
   1.19.07 Makerewa -  GetPointsFromString proc to read in points from the file
   1.19.07 removed sys calls from members system - left delete win file

   ****************************************************************************
   DATABASE CHANGE - 
   1.20.00 - Menu Mate added SWIPE_CARD into members and members_def
           - CREATE TABLE "KIOSK"
           - CREATE TABLE "KIOSK_HISTORY"
           - Settings Kiosk Message, and Kiosk 
           - Maintenance - options - member visits
   ****************************************************************************
   1.20.00 - added IBTransactionCommit again to commit after form deactivate
           -  and after adding/deleting a Member - stop locking for Kiosk/MenuMate
   1.20.01 - added Kiosk Report
   1.20.01 - added Members Proposer Seconder Report - for Upper Hutt      
   1.20.01 - modified Voting List - added Groups 
   1.20.02 - modified Current Points Report
   1.20.02 - Print Member Details on payment - only do new members - Hornby
   1.20.03 - ibdsMembers after post - removed IBTransactionCommit as it was not looking
             up the Subs
   1.20.03 - SWIPE_CARD 
   make up for Blenheim moved to ibdsMembers Before Post

   ****************************************************************************
   DATABASE CHANGE - 
   1.21.00 - added AREV_FILE_PATH"	VARCHAR(40);  for RPNYC second system
   ****************************************************************************
   1.21.00 - Export of Changes - dont send points if Restaurant to RPNYC - AvecSecondFilePAth
   1.21.01 - Maint - added Members by Group for a Card
   1.21.01 - Reports - removed Group Selection on Create Card File     
   1.21.01 - Edit - Debug fix - Swipe_Card was filled with Position     
   1.21.01 - Query - added Null and Not Null to 
   
   ****************************************************************************
   DATABASE CHANGE - 
   1.22.00 - added Points and Trigger for connection to Menu Mate
   NB only include trigger if menu mate is connected
   Splits the Points 
   NB watch out - any change  to Membership thru IBConsole etc
   will Generate into the Points Split file
   ****************************************************************************
   1.22.00 - PayBeforeExpires on Sections
   1.22.00 - Fix to Subs Analysis Report - PAID column

   ****************************************************************************
   DATABASE CHANGE - 
   1.23.00 - Expires Date on the Card if a Life/Honary Member 
   ****************************************************************************
   1.23.00 - Export Subs Due - increased remp table field length - RPNYC
   1.23.00 - Removed Trigger Active/Deactive for Tuskers
   1.23.00 - Backups not working on Masterkey
   1.23.01 - View of Points Allocation - removed members with no Donation Member

   ****************************************************************************
   DATABASE CHANGE - 
   1.24.00 - Menu Mate intalled - for Backups
   - must be SYSDBA and the Password ( master or masterkey )
   ****************************************************************************
   1.24.00 - Backups not working - cant be DEFAULT 
   1.24.00 - upgrade
   1.24.00 - Blenheim - swipe card makeup - used SWIPE Card in frmMembersEDIT
   1.24.01 - Pauanui - printing of address removed on the Invoice - only for Pauanui
   1.24.01 - Hornby - eport of First Name and Surname for STockmaster
   1.24.01 - Fix for table Sections not open when starting the form
   1.24.02 - Pauanui - card swipes
   1.24.02 - Pauanui - print Mailing labels from Create Card File
   1.24.03 - Pauanui - Mailing labels with no spaces
   ****************************************************************************
   DATABASE CHANGE - 
   1.25.00 - PHM Installed 
   ****************************************************************************
   * CheckSiteLicense removed to much hassle with sites
   * CheckLogInCount  removed to much hassle with menumate
   ****************************************************************************
   DATABASE CHANGE - 
   1.26.00 - KIOSK additional Lucky Draw and Birthday
             Menu Mate Swipe Card Makeup
   *****************************************************                                             
   1.26.00 Kiosk changes in Settings
   1.26.01 Edit Brief changes
   ****************************************************************************
   DATABASE CHANGE - 
   1.27.00 - KIOSK additional Lucky Draw End Times
   *****************************************************                                             
   ****************************************************************************
   DATABASE CHANGE - 
   1.28.00 - Misc table changes
   *****************************************************
   1.28.00 Known As , Address Changes and some minor bug fixes
   ****************************************************************************
   DATABASE CHANGE - 
   1.29.00 -  table Group changes
   *****************************************************
   1.29.00 Swipe Card Fixes for Howick
   1.29.00 Address change for Parent Codes for Howick
   1.29.02 Emails add to Clipboard
   ****************************************************************************
   DATABASE CHANGE - 
   1.30.00 -  Kiosk History
   *****************************************************
   1.30.00 
      - Bug in Stockmaster Points Report
   ****************************************************************************
   DATABASE CHANGE - 
   1.40.00 -  Preference Customer 3 tables
   DONT do - CREATE TABLE "PREF_MEMBERS" - done in SPLASH because of USER priveleges
   *****************************************************
   1.40.00 Pauanui - Multiple members in the Mailing Labels - because of multiple groups/subs etc
   1.40.00 Kelvin Hotel - Export to PHM - format of Points Available
   1.40.00 Export to Stockmaster RPNYC - second file path - tidy up
   1.40.01 Export Subs Due RPNYC - add in Financial/Non Financial
   1.40.02 Timaru Conversion to MenuMate - swipe card format
   1.40.03 Mailing Labels to Imclude Receive Email
   ****************************************************************************
   DATABASE CHANGE - 
   1.41.00 -  Misc 
   *****************************************************
   1.41.00 PrintReceiptAsText added to confuration 
           affects (Blenheim done),(Upper Hutt done),Richmond,(Papanui done)                                       
	1.41.00 SwipeCard added to confuration
			  affects (Blenheim done),Richmond,(Tuskers done),(Pauanui done),(Howick done),Timaru(Done) 
	1.41.01 PayReprint 
      -  Receipt made longer so it wasnt cutting off.
	1.41.02 Maintenance 
      - loading .JPG files - was looking for .jpg files ( case insensitive )
	1.41.02 Section Payments 
      - expires date set to the Section - so increase this first
	****************************************************************************
	DATABASE CHANGE -
	1.42.00 -  Sections  added MIN_FEE into table sections
	****************************************************************************
	1.42.00 
      - payments on Sections and Change Section in Payments
	1.42.01 
      - payments Deletion of Section
	1.42.01 
      - Card Print on Win98 = deleted on LPT but it leaves a space on the end = Eltron to Eltron_
   ****************************************************************************
   DATABASE CHANGE - 
   1.43.00 -  Points Log add Full Name for Deleted Members 
   ****************************************************************************
   1.43.00 
      - Points and Credits set to zero if null on Before Post on Members table
	****************************************************************************
	DATABASE CHANGE -
	1.44.00 -  Kiosk Changes, MenuMate Connected, Misc
	****************************************************************************
	1.44.00 
      - Kiosk Changes in System
	1.44.01 
      - MenuMate Login Changes
	****************************************************************************
	DATABASE CHANGE -
	1.45.00 -  Points_Audit Table
	****************************************************************************
	1.45.00
	  -Added 2 Reports for Points - Points Audit and Sponsorship Sections
	  -Modified Stockmaster Points - added in Group Select
	1.45.01
	  -Minor bug fixes
	  -Points Audit Report - format on Deleted field ( Summary )
	  -Points Log Report   - end date ( timestamp ) fix
     -Mailing Labels from Last Report - order of records/labels fixed
     -Added Phone to Members Information Report
     -Added Members with Many Subs Report
     -Added Members with Many Groups Report
	1.45.02
     -Section Information report - added Company and Barcode - Papanui Only
     -Mailing Labels with Group Selection and Parent Codes ( 2 Members in Separate Groups )
	****************************************************************************
	DATABASE CHANGE -
	1.46.00 -  Proposer and Seconder Names in Members
	****************************************************************************
	1.46.00
      - new MenuMate splash Screen
      - Added Proposer and Seconder name frmEdit.  ( Bays Club )
	1.46.01
      - Fixed Proposer and Seconder Report
	1.46.02
      - Non Financial Member delete - didnt have full name for the Points Log (Papanui)
	1.46.03
      -Member Points Audit - out in MenuMate Date From and MenuMate Date To GroupBox (Papanui)
      -Member Number on Mailing Label (Howick)
	1.46.04
      -Member Points Audit - Report Date range to incude MenuMate Date Range
      -Points Log Report   - Summary totals on the bottom of the report
	1.46.05
      -Bug - Mailing Labels - missed off the Post Code ( stored in Home_phone of M_Rollover )
      -Direct Debits fix
      - New Main Menu
	1.46.06
      - A Deleted Member when Exporting to Stockmaster - it resets the Points ('R' record ) first  
        ( from Non Financial Member Delete at Hornby )
	1.46.07
      - DD Print Cards then Preview the Report ( RPNYC ) 
      - Joining Fees not coming thru into Payments
      - Backups not to look for a local dir of dirBackup, if the DB is on another PC 
	1.46.08
      - Export Subs Due for Pauanui - by Parent Code and a sort order
	****************************************************************************
	DATABASE CHANGE -
	4.00.00 -  Members_Points table added - Check for Subs and Groups
	****************************************************************************
	4.00.00
      - Added Members_Points for better points coming from MenuMate
      - CheckGroupsSubs in Maintenance                                                              
	4.00.01
      - fix Email to Members from a Report in ( 1,2, etc ) cant use more then 1500
      - Export now has a Save Dialog box and this is reset each time
      - Top 50 Sales Report  ( papanui )
	****************************************************************************
	DATABASE CHANGE -
	4.01.00 -  Clear Interests Flag put in the Misc Table
	****************************************************************************
	4.01.00
      Dont Clear Section Interests after the Report - papanui Keep the as Profiles
      Section Report not broken into pages
	****************************************************************************
	DATABASE CHANGE -
	4.02.00 -  Discounts for MenuMate by Member - Finn McCoulls
	****************************************************************************
	4.02.00
      Papanui dont want District on Mailing Labels
      Discounts for MenuMate by Member - Finn McCoulls
	4.02.01
      Registry looking at IQWorks 
      Setup Screen for Registry and Interbase Install
      Registry fix for XP
      Update Expires from frmMemberSYS - for Ashburton
      Bulk Add/Delete Discounts from Maintenance
      Hide Donation Points in Maintenence
      POS_LICENSE filled in in Splash because MenuMate now relies on it 
	****************************************************************************
	DATABASE CHANGE -
	4.03.00 -  table MISC DISCOUNTS_INSTALLED
	****************************************************************************
	4.03.01 
      Hide Discounts for MenuMate - can run on any version of MenuMate
	4.03.02 
      Altererd Trigger for Tuskers
      Points Alocation alterered to add Bonus Credit and  Total Earnt
	4.03.03 
      Added Report Sponsorship by Member - Member Sales added to a Section - Timaru TCC
      Facility to edit the Kiosk Message from Settings Screen in Membership.
      Note - Kiosk screen upgraded
      Change - Top 50 Sales report changes for Upper Hutt so they can Mail Merge it
           

	MenuMate 3.35.12 needs Membership 4.00 minimum
	MenuMate 4.00.00 needs Membership 4.02 minimum

	Membership latest version can handle any version of MenuMate
	( if Discounts are turned off )

	}
	SysVersion       =  '4.03.03';
	//                  major/database/minor  release
	{
	 MenuMate StartDate Monday 11 october 2004
	 TeAnau Club - Membership Installation                                         13/10/2004
	 Oxford WMC - upgrade                                                          22/11/2004
	 Levin Membership Install                                                      20/11/2004
	 Papanui upgrade to MenuMate                                                   26/01/2005
	 Papatoetoe upgrade to MenuMate                                                01/03/2005
    Ashburton Club upgrade to MenuMate                                            22/02/2005
    Upper Hutt Menu Mate install                                                  25/02/2005
    Bays Club ( New Membership )                                                  28/04/2005 
    Rawhiti Golf Club ( New Membership )                                          18/07/2005 
    Martinborough ( New Membership )                                             19/07/2005

   
	Menu Mate Connection - Membership minimum version 1.20.00  menu mate ver min 3.2.0.22
   Dont forget any AREV updates
   Before 1.14.00 - Arev update Points Reset
   After  1.18.01 - Export Member Points Update  }                          

{alter table CARD_MEMBERS
  add "CARD_TYPE"	SMALLINT;

CREATE TABLE "M_PHOTOS" 
(
  "MEMBER"	INTEGER NOT NULL,
  "PHOTO"	BLOB SUB_TYPE 0 SEGMENT SIZE 80,
  "DRAWN"	CHAR(1),
 PRIMARY KEY ("MEMBER")
);
   grant all on M_PHOTOS to allusers;

	alter table members
	alter SPARE_INT to CLUBS,
   alter SPARE_CHAR_1 to ASSOCIATION;

   alter table members_def
   alter SPARE_INT to CLUBS,
   alter SPARE_CHAR_1 to ASSOCIATION;

   CREATE TABLE "CLUBS"
   (
   "CLUB_ID"	SMALLINT NOT NULL,
   "DESCRIPTION"	VARCHAR(60),
    PRIMARY KEY ("CLUB_ID")
   );
   grant all on CLUBS to allusers;

    alter table misc
       add  "PART_PAY_ADVANCE"	CHAR(1);
       
   alter table printers    add EMULATION smallint;
   alter table printers    alter  PRINTER_TYPE TO COMM_PORT;


    alter table misc
       add POS_PAY_CARD_REQD char(1);

   /*  1.08.00 */

    CREATE  INDEX DESC_INDEX ON CLUBS (DESCRIPTION);
    alter table misc
		 add BOTTOM_LINE_FEED smallint,
       add TOP_LINE_FEED smallint,
		 add CHECKED_FIELDS varchar(45);

	alter table CARD_MEMBERS
     drop "CARD TYPE",
     add "CARD_TYPE" smallint;

    alter table misc
     add PAY_BEFORE_EXPIRES char(1),
     add CARD_DEFAULT  char(1),
     add RECEIPT_DEFAULT char(1),
     add EXPORT_SUBS char(1),
     add DEFAULT_NUMBER smallint;

   /* Fill in Default Card and Default Receipt into Misc  - need to fill in on site upgrades */
   
   /*  1.12.00 */
    alter table misc
      add POINTS_PASSWORD char(1);

    update misc set CARD_DEFAULT = 'N',RECEIPT_DEFAULT = 'N';
    commit work;
     
    /* Points Version Below Here  */
    /* 1.13.00 */
    alter table MEMBERS
    drop POINTS_AVAILABLE,
    drop VALUE_OF_CREDIT,
    alter TOTAL_EARNT type DOUBLE PRECISION,
    alter TOTAL_REDEEMED type DOUBLE PRECISION,
    alter EARNT_CREDIT type DOUBLE PRECISION,
    alter BONUS_CREDIT type DOUBLE PRECISION,
	 alter PRE_PAID_CREDIT type DOUBLE PRECISION;
    commit work;

	 alter table MEMBERS_DEF
	 drop POINTS_AVAILABLE,
    drop VALUE_OF_CREDIT,
    alter TOTAL_EARNT type DOUBLE PRECISION,
    alter TOTAL_REDEEMED type DOUBLE PRECISION,
	 alter EARNT_CREDIT type DOUBLE PRECISION,
    alter BONUS_CREDIT type DOUBLE PRECISION,
    alter PRE_PAID_CREDIT type DOUBLE PRECISION;
    commit work;

    CREATE TABLE "POINTS_LOG" 
    (
    "POINTS_ID"	INTEGER NOT NULL,
    "MEMBER"	INTEGER,
    "USER_CODE"	INTEGER,
    "PROCESS"	SMALLINT,
    "REASON"	VARCHAR(35),
    "BEF_TOTAL_EARNT" DOUBLE PRECISION,
    "BEF_TOTAL_REDEEMED" DOUBLE PRECISION,
    "BEF_EARNT_CREDIT" DOUBLE PRECISION,
    "BEF_BONUS_CREDIT" DOUBLE PRECISION,
    "BEF_PRE_PAID_CREDIT" DOUBLE PRECISION,
    "TOTAL_EARNT" DOUBLE PRECISION,
    "TOTAL_REDEEMED" DOUBLE PRECISION,
    "EARNT_CREDIT" DOUBLE PRECISION,
    "BONUS_CREDIT" DOUBLE PRECISION,
    "PRE_PAID_CREDIT" DOUBLE PRECISION,
    "POINTS_VALUE"	FLOAT,
    "EDIT_DATE" TIMESTAMP,
    CONSTRAINT "PK_POINTS" PRIMARY KEY ("POINTS_ID")
	 );
    GRANT ALL on POINTS_LOG to ALLUSERS;
	 commit work;

	 CREATE GENERATOR "POINTS_LOG_GEN";
    SET TERM ^ ;
    CREATE TRIGGER "POINTS_LOG_TRIGGER" FOR "POINTS_LOG" 
    ACTIVE BEFORE INSERT POSITION 0
    AS
	 BEGIN
      NEW.POINTS_ID = GEN_ID (POINTS_LOG_GEN, 1);
    END
    ^
    COMMIT WORK ^
    SET TERM ;^

    /* deletes the Foreign key for Groups and mail Codes */
    delete from RDB$RELATION_CONSTRAINTS where RDB$RELATION_NAME = 'MEMBERS' and
    RDB$CONSTRAINT_TYPE = 'FOREIGN KEY';

    /*  for the above to take care of the Group alteration below */
    commit work;
 
    alter table MEMBERS
    alter SPARE_VARCHAR_20 to REASON,
    alter KNOWN_AS type varchar(15),
    alter WORK_PHONE type varchar(14),
    alter CELL_PHONE type varchar(14),
    alter FAX_NUMBER type varchar(14),
    alter POSITIONS type varchar(20),
    alter "GROUPS" type numeric(6,2),
    alter "GROUPS" to PAYMENT_DISC,
    alter INITIALS type varchar(6);    
	 commit work;
    
	 alter table MEMBERS_DEF
    alter SPARE_VARCHAR_20 to REASON,
	 alter KNOWN_AS type varchar(15),
    alter WORK_PHONE type varchar(14),
    alter CELL_PHONE type varchar(14),
    alter FAX_NUMBER type varchar(14),
    alter POSITIONS type varchar(15),
    alter "GROUPS" type numeric(6,2),
	 alter "GROUPS" to PAYMENT_DISC,
    alter INITIALS type varchar(6);    
    commit work;
    
   alter table "EXPORT_AREV"
     add RESET_POINTS char(1);
     
   alter table misc
     alter DISCOUNT_2 to EXPORT_TIMER;

   alter table misc
     alter EXPORT_TIMER type integer;

     
   alter table SUBSCRIPTIONS
      add TERM smallint;   

   update SUBSCRIPTIONS
      set TERM = 12;
      
   commit work;
      
   alter table members
		alter known_as type varchar(25),
      alter POSITIONS type varchar(25),
	  alter REASON type varchar(35);
                                      
	alter table members_def
     alter known_as type varchar(25),
     alter POSITIONS type varchar(25),
     alter REASON type varchar(35);
   commit work;
    
   /* 1.14.00 */
	alter table misc
    add NUMBER_COLUMNS smallint,
    add COLUMN_SPACE numeric(6,2),
    add LEFT_MARGIN  numeric(6,2),
    add RIGHT_MARGIN  numeric(6,2),
    add TOP_MARGIN  numeric(6,2),
    add BOTTOM_MARGIN  numeric(6,2),
    add BAND_HEIGHT numeric(6,2);
   commit work;
    
   /* 1.15.00   */
   alter table misc
    add FONT_SIZE smallint;
   commit work;
    
   /* 1.16.00   */
   alter table payments
     alter FULL_NAME type varchar(40);    
   commit work;
    
   DROP TRIGGER "PAYMENTSTRIGGER1";
   commit work;

   /* 1.17.00   */
	CREATE TABLE "IMPORT_POINTS"
   (
  "MEMBER"	INTEGER,
  "POINTS_AVAILABLE"	DOUBLE PRECISION,
  "TOTAL_REDEEMED"	DOUBLE PRECISION,
  "TOTAL_EARNT"	DOUBLE PRECISION,
  "PRE_PAID_CREDIT"	DOUBLE PRECISION,
  "BONUS_CREDIT"	DOUBLE PRECISION,
  "EARNT_CREDIT"	DOUBLE PRECISION
   );
	commit work;

   grant all on IMPORT_POINTS to allusers;
   commit work;
   
   /* 1.18.00   */
    ALTER TABLE "IMPORT_POINTS" 
    ALTER  "MEMBER"	type VARCHAR(20);
   commit work;
   
  /* apply query to update DECEASED to N where DECEASED is null - to be sure */
    UPDATE MEMBERS set DECEASED = 'N' where DECEASED is null;
    COMMIT WORK;

   /* 1.19.00   */
   alter table members_def
     add  "POINTS_AVAILABLE"	DOUBLE PRECISION;
   alter table members
     add  "POINTS_AVAILABLE"	DOUBLE PRECISION;
   alter table POINTS_LOG
      add "BEF_POINTS_AVAILABLE"	DOUBLE PRECISION,
		add "POINTS_AVAILABLE"	DOUBLE PRECISION;
    alter table misc add PRINT_MEMBER_PAYMENT char(1);
	 COMMIT WORK;
    UPDATE MEMBERS set POINTS_AVAILABLE = 0.00 where POINTS_AVAILABLE is null;
	 COMMIT WORK;

   /* 1.20.00   */
    alter table members_def
    add "SWIPE_CARD" VARCHAR(128) CHARACTER SET ISO8859_1 COLLATE EN_UK,
    add "VISITS" smallint,
    add "LAST_VISIT" Timestamp;
    commit work;
    
	 alter table members
    add "SWIPE_CARD" VARCHAR(128) CHARACTER SET ISO8859_1 COLLATE EN_UK,
    add "VISITS" smallint,
    add "LAST_VISIT" Timestamp;
    commit work;

    update MEMBERS set SWIPE_CARD = MEMBER;
    commit work;
    
   CREATE TABLE "KIOSK"
   (
     "KIOSK_ID"	SMALLINT NOT NULL,
     "MESSAGE"	VARCHAR(80),
     PRIMARY KEY ("KIOSK_ID")
   );
   commit work;
   grant all on KIOSK to allusers;
   commit work;

   alter table MISC
	add "KIOSK_MSG"	BLOB SUB_TYPE 0 SEGMENT SIZE 80;
   commit work;

   CREATE TABLE "KIOSK_HISTORY"
	(
  "MEMBER"	INTEGER NOT NULL,
  "KIOSK_ID"	SMALLINT NOT NULL,
  "DATE_TIME"   TIMESTAMP
   );
   commit work;

  grant all on "KIOSK_HISTORY" to allusers;
  commit work;


   /* 1.21.00   */
   Alter table misc              
   add "AREV_FILE_PATH"	VARCHAR(40);

   commit work;

   /* 1.22.00   */
   alter TABLE "SECTIONS"      
   add  "DONATION"  numeric ( 6,2);

   alter table "MISC"
    add TRIGGER_INSTALLED char(1);

   Alter TABLE "MEMBERS"
   add "DONATION" Numeric(6,2),
   add "DONATION_MEMBER" Integer;
   commit work;

	Alter TABLE "MEMBERS_DEF"
   add "DONATION" Numeric(6,2),
	add "DONATION_MEMBER" Integer;
   commit work;

   CREATE TABLE "POINTS_SPLIT" 
    (
   "POINTS_SPLIT_ID"	INTEGER NOT NULL,
   "DATE_TIME"	TIMESTAMP,
   "MEMBER"	INTEGER,
   "POINTS"	NUMERIC(6, 2),
   "DONATION_MEMBER"	INTEGER,
   "DONATION_POINTS"	NUMERIC(6, 2),
   "LAST_POINTS_AVAILABLE"	NUMERIC(6, 2),
   "LAST_DONATED_POINTS_AVAILABLE"	NUMERIC(6, 2),
	"UPDATED" char(1),
    PRIMARY KEY ("POINTS_SPLIT_ID")
   );
   commit work;
   grant all on POINTS_SPLIT to allusers;

   commit work;
   
   create GENERATOR POINTS_SPLIT_GEN;
   
   SET TERM ^ ;

   CREATE TRIGGER "POINTS_SPLITTRIGGER" FOR "POINTS_SPLIT" 
   ACTIVE BEFORE INSERT POSITION 0
   AS
   BEGIN
      NEW.POINTS_SPLIT_ID = GEN_ID (POINTS_SPLIT_GEN, 1);
   END
	^
   COMMIT WORK ^
	SET TERM ;^

	/* add POINTS_UPDATE_TRIGGER on MEMBERS for Menu Mate Sites */
   /* set MenuMate Trigger Installed to Y */

   /* 1.23.00   */
   alter table SUBSCRIPTIONS
    add EXPIRES_CARD char(1),
    add EXPIRES_DESC varchar(20);
   commit work;

   /* set MenuMate Installed to Y  for existing MENUMATE Sites - Backups */
   /* 1.24.00   */
   alter table misc      
	add MENUMATE_INSTALLED char(1);
   commit work;

   /* 1.25.00   */ 
   alter table MISC                               
   add "PHM_INSTALLED"	CHAR(1),                  
   add "PHM_PATH"	varCHAR(25),                    
   add "PHM_CONTROLLER"	VARCHAR(40);              
   commit work;

   CREATE TABLE "PHM_LOG"                         
   (                                              
  "MEMBER"	INTEGER NOT NULL,                     
  "POINTS"	VARCHAR(15),                          
  "DATE_TIME"	TIMESTAMP,                         
  "COMMENT"	VARCHAR(30),                          
  "BOOKING"	INTEGER                               
	);
   commit work;                                   
	grant all on PHM_LOG to allusers;
   commit work;                                   

    /* 1.26.00   */     
   alter TABLE "MISC"
   add "ONE_DIGIT" char(4),
   add "TWO_DIGIT" char(4),
   add "THREE_DIGIT" char(4),
   add "SWIPE_TEXT" VARCHAR(35),
   add "ON_INSERT" char(1),
   ADD "MONDAY_PRINT" CHAR(1),
   ADD "TUESDAY_PRINT" CHAR(1),
   ADD "WEDNESDAY_PRINT" CHAR(1),
   ADD "THURSDAY_PRINT" CHAR(1),
   ADD "FRIDAY_PRINT" CHAR(1),
	ADD "SATURDAY_PRINT" CHAR(1),
   ADD "SUNDAY_PRINT" CHAR(1),
   add "BIRTHDAY_TEXT" VARCHAR(35),
   add "LUCKY_DRAW_TEXT" VARCHAR(35),
   ADD "KIOSK_MEMBER_START" smallint,
   add "KIOSK_MEMBER_LENGHT"	SMALLINT,
   add "BIRTHDAY_PRINT" char(1);
   commit work;

   update misc set "MONDAY_PRINT" = 'F',"TUESDAY_PRINT" = 'F',"WEDNESDAY_PRINT" = 'F',
   "THURSDAY_PRINT" = 'F',"FRIDAY_PRINT" = 'F',"SATURDAY_PRINT" = 'F',"SUNDAY_PRINT" = 'F',
   "BIRTHDAY_PRINT" = 'F';
   commit work;
   
   
    /* 1.27.00   */
	alter table "MISC"
   add "VISIT_MSG" varchar(70),
	add "BIRTHDAY_MSG" varchar(70),
   add "LUCKY_DRAW_MSG" varchar(70),
	add  "MONDAY_END_TIME"	TIME,
   add  "TUESDAY_END_TIME"	TIME,
   add  "WEDNESDAY_END_TIME"	TIME,
   add  "THURSDAY_END_TIME"	TIME,
   add  "FRIDAY_END_TIME"      TIME,
   add  "SATURDAY_END_TIME"	TIME,
   add  "SUNDAY_END_TIME"      TIME;
   commit work;

    /* 1.28.00   */
   alter table "MISC"
   add "KNOWN_AS_DEFAULTED" char(1),
   add "PARENT_CODE_ADDRESS" char(1),
   add "GROUP_DELETE" char(1);
	commit work;

   update misc set "KNOWN_AS_DEFAULTED" = 'Y';
	commit work;

    /* 1.29.00   */
    alter table "GROUPS"
    add "IN_REPORT" char(1);                                                
   commit work;

	 /* 1.30.00   */
	ALTER TABLE "KIOSK_HISTORY"
	add "DRAWN" char(1),
	add "PRIZE" varchar(25);
	commit work;

	ALTER TABLE "MISC"
	add "MENUMATE_POINTS_DATE" DATE,
	add "KIOSK_DRAW_DATE" DATE,
	add "KIOSK_SUCCESS" char(1),
	add "KIOSK_ERROR_MSG" varchar(30),
	add "KIOSK_DRAWS" smallint;
	commit work;

	CREATE TABLE "KIOSK_DRAWS"
   (                                                          
  "KIOSK_ID"	smallint,                                      
  "KIOSK_DESC" varchar(25)                                    
   );
   grant all on "KIOSK_DRAWS" to allusers;                    
                                                              
   commit work;                                               
                                                              
   alter table "MEMBERS_DEF"                                  
	add "PREV_POINTS_AVAILABLE" DOUBLE PRECISION,
   add "TOTAL_VISITS" integer; 

   alter table "MEMBERS"                                      
   add "PREV_POINTS_AVAILABLE" DOUBLE PRECISION,              
   add "TOTAL_VISITS" integer;                                
   commit work;                                               
                                                              
   CREATE TABLE "MENUMATE_POINTS"                             
   (                                                          
  "MEMBER"	INTEGER NOT NULL,
  "POINTS_DIFFERENCE" DOUBLE PRECISION,
  "POINTS_AVAILABLE" DOUBLE PRECISION,                        
  "THE_DATE" DATE
	);
   commit work;
	grant all on "MENUMATE_POINTS" to allusers;
                                                              
	commit work;
                                                              
                                                              
                                                              
   CREATE TABLE "PREF_HEADING"                                
	(
     "HEADING_ID"	INTEGER NOT NULL,                           
     "HEADING_DESC"	VARCHAR(30),
   CONSTRAINT "PK_PREF_HEADING_ID" PRIMARY KEY ("HEADING_ID") 
   );
   commit work;
   grant all on "PREF_HEADING" to allusers;                   
   commit work;
                                                              
   create GENERATOR "PREF_HEADING_GEN";                       
                                                              
	SET TERM ^ ;

   CREATE TRIGGER "PREF_HEADING_TRIGGER" FOR "PREF_HEADING"   
   ACTIVE BEFORE INSERT POSITION 0                            
   AS
   BEGIN                                                      
      NEW.HEADING_ID = GEN_ID (PREF_HEADING_GEN, 1);          
   END
    ^                                                         
                                                              
   COMMIT WORK ^                                              
   SET TERM ;^                                                
                                                              
	CREATE TABLE "PREF_SUB_HEADING"
   (                                                          
	  "HEADING_ID"	INTEGER NOT NULL,
     "SUB_HEADING_DESC"	VARCHAR(30),
	  "DISPLAY_HEADING"	CHAR(1)
   );                                                         
   commit work;
   grant all on "PREF_SUB_HEADING" to allusers;               
   commit work;                                               

   CREATE TABLE "PREF_DETAIL"                                 
   (                                                          
     "HEADING_ID"	INTEGER NOT NULL,                           
     "DETAIL_DESC"	VARCHAR(30),                             
     "DETAIL_TYPE"	CHAR(1)
     
	);
   commit work;
   grant all on "PREF_DETAIL" to allusers;
   commit work;

	/* 1.40.00   */
	alter TABLE "PREF_DETAIL"
	alter "DETAIL_DESC" type varchar(60),
	add   "DISPLAY_DETAIL"	CHAR(1),
	add  "FIELD_DESC"	VARCHAR(60),
	add "DETAIL_ID"	INTEGER NOT NULL,
	add  "LINE_NUMBER" smallint NOT NULL;

	alter TABLE "PREF_SUB_HEADING"
	add "SUB_HEADING_ID"	INTEGER NOT NULL,
	alter "SUB_HEADING_DESC" type varchar(60);


	alter TABLE "PREF_HEADING"
	alter "HEADING_DESC" type varchar(60);
	commit work;

	/* 1.41.00   */
	alter table MISC
		add PRINT_RECEIPT_AS_TEXT char(1),
		add SWIPE_LENGTH smallint,
		alter SWIPE_TEXT type VARCHAR(60),
      drop ONE_DIGIT,
      drop TWO_DIGIT,
      drop THREE_DIGIT;
   commit work;   

   /* 1.42.00   */                                         
   alter TABLE "SECTIONS"
		add MIN_FEE numeric(6,2);
   commit work;   

   /* 1.43.00   */                                         
	alter TABLE "POINTS_LOG"
		add FULL_NAME varchar(35);
	commit work;

	/* 1.44.00   */
	alter TABLE "MISC"
		add POINTS_AS_DOLLARS char(1),
		add SHOW_DICE char(1),
		add CHECK_EXPIRES char(1),
		add VIP_CREDIT_NAME varchar(20),
		add KIOSK_TIMER smallint;
	commit work;

	CREATE TABLE "POINTS_AUDIT"
	(
	  "LOCATION"	VARCHAR(25),
	  "EDIT_DATE"	TIMESTAMP,
	  "POINTS_EARNT" NUMERIC(15,4),
	  "POINTS_SPENT" NUMERIC(15,4),
	  "BONUS_POINTS"    NUMERIC(15,4),
	  "VIP_CREDIT"      NUMERIC(15,4),
	  "ADJUSTED_POINTS" NUMERIC(15,4),
	  "BIRTHDAY_POINTS" NUMERIC(15,4),
	  "LUCKY_POINTS"    NUMERIC(15,4),
	  "SUBS_POINTS"     NUMERIC(15,4),
	  "DELETED_POINTS"  NUMERIC(15,4)
	);
	commit work;
	grant all on "POINTS_AUDIT" to allusers;
	commit work;

	/* 1.45.00   */
	create generator POINTS_AUDIT_GEN;

	alter TABLE "POINTS_AUDIT"
	add  "POINTS_AUDIT_ID"	INTEGER NOT NULL,
	add CONSTRAINT "PK_POINTS_AUDIT" PRIMARY KEY ("POINTS_AUDIT_ID");

	commit work;

	alter TABLE "POINTS_AUDIT"
		alter  "POINTS_AUDIT_ID"	position 0;

	commit work;

	SET TERM ^ ;
	CREATE TRIGGER "POINTS_AUDIT_TRIGGER" FOR "POINTS_AUDIT"
	ACTIVE BEFORE INSERT POSITION 0
	AS
		BEGIN
			NEW.POINTS_AUDIT_ID = GEN_ID (POINTS_AUDIT_GEN, 1);
		END
	^
	COMMIT WORK ^
	SET TERM ;^


	/* 1.46.00   */

	 alter table members
		add PROPOSER_NAME varchar(40),
		add SECONDER_NAME varchar(40);

	 alter table members_def
		add PROPOSER_NAME varchar(40),
		add SECONDER_NAME varchar(40);


	  update members a set a.proposer_name =
	  (select m.Full_Name from Members m where m.member = a.proposer);

	  update members a set a.seconder_name =
	  (select m.Full_Name from Members m where m.member = a.seconder);

	 commit work;

	 /* 1.47.00   */
	 alter table Misc
	 add CHECK_SUBS char(1),
	 add CHECK_GROUPS char(1),
	 add MAX_SUB smallint,
	 add MAX_GROUP smallint,
	 add MIN_SUB smallint,
	 add MIN_GROUP smallint;

	 commit work;

	 update Misc set CHECK_SUBS = 'N',CHECK_GROUPS = 'N';

	 commit work;

	/* 4.00.00   */
	CREATE TABLE "MEMBERS_POINTS"
	(
	 "MEMBERS_POINTS_KEY"	INTEGER NOT NULL,
	 "MEMBER"	INTEGER,
	 "EARNED"	NUMERIC(15, 4),
	 "REDEEMED"	NUMERIC(15, 4),
	 "ADJUSTMENT_TYPE"	INTEGER,
	 "POINTS_TYPE"	INTEGER,
	 "TIME_STAMP"	TIMESTAMP,
	 "INVOICE_NUMBER"	VARCHAR(50),
	 PRIMARY KEY ("MEMBERS_POINTS_KEY")
	);
	commit work;
	grant all on MEMBERS_POINTS to allusers;
	commit work;
	create generator GEN_MEMBERS_POINTS;
	commit work;
	update MEMBERS set POINTS_AVAILABLE = 0.00 where POINTS_AVAILABLE is null;
	commit work;


	/* 4.01.00   */
	alter TABLE MISC
	  add CLEAR_INTERESTS char(1);

	 commit work;


	 /* 4.02.00   */
	 CREATE TABLE "MEMBERS_DISCOUNTS"
	 (
		"MEMBER_DISCOUNTS_KEY"	INTEGER NOT NULL,
		"DISCOUNT_KEY"	INTEGER NOT NULL,
		"MEMBER" INTEGER NOT NULL,
		PRIMARY KEY ("DISCOUNT_KEY","MEMBER")
	 );

	 commit work;
	 grant all on MEMBERS_DISCOUNTS to allusers;
	 commit work;

	 /* 4.03.00   */
	 ALTER TABLE MISC
	 add DISCOUNTS_INSTALLED char(1);

	 commit work;

	 }


	{
	/* standard setup */
	update MISC set PRINT_RECEIPT_AS_TEXT = 'Y';

   /* MenuMate  Install Only */
   update MISC set MENUMATE_INSTALLED = 'Y';  
   update MISC set SWIPE_LENGTH = 4;  
   update MISC set SWIPE_TEXT   = ';member?';  
   update MISC set ON_INSERT    = 'Y';  
   update MEMBERS set ALLOW_CREDIT = 'Y', EARN_POINTS = 'Y';
	update MEMBERS_DEF set ALLOW_CREDIT = 'Y', EARN_POINTS = 'Y',POINTS_AVAILABLE = 0.00;
	update MEMBERS set POINTS_AVAILABLE = 0.00 where POINTS_AVAILABLE is null;
	commit work;
    /* affects Receipt Printing from Payments  ( note when each site has been done )
      ** (Blenheim done 20/1/05)
      ** (Upper Hutt)
      **Richmond
		** (Papanui done 19/1/05)
		update MISC set PRINT_RECEIPT_AS_TEXT = 'N';
	*/
	/* affects Swipe Card  ( note when each site has been done )
		get the code from each site and put in the update SQL
		** Howick   = 5  ;member?  'T'
		** (Blenheim = 4  %5member30064?;5member30064?  'T'  done 20/01/05)
		**Tuskers  = 3  ;member?  'T'  Brad said 0 member T
		**Richmond = 0  ;member?  'T'
      **Pauanui  = 4  %0member00598AD@@@@^]@@@@@@@@@@@@@@@@H@@@@@fullname?;member?  'T'
      **Timaru   = 11 ;member?  'T'
   */


   {* Ashburton       version 1.19.01        30/06/2003                               
   *  Ashburton       version 1.19.06        8/10/2003                               
   *  Ashburton       version 1.28.00        31/08/2004                               
	*  Ashburton       version 1.42.01        22/02/2005
   
	*  Bays Club       version 1.46.01        28/04/2005 ( install date )
                                             
   *  Blenheim        version 1.14.00        24/02/2003                               
   *  Blenheim        version 1.14.02        07/03/2003                               
   *  Blenheim        version 1.17.01        16/04/2003                               
   *  Blenheim        version 1.18.01        16/05/2003                               
   *  Blenheim        version 1.18.02        19/05/2003                               
   *  Blenheim        version 1.18.04        10/06/2003                               
   *  Blenheim        version 1.20.00        09/12/2003                               
   *  Blenheim        version 1.20.01        15/12/2003                               
   *  Blenheim        version 1.20.02        24/12/2003                               
	*  Blenheim        version 1.20.03        27/01/2003
   *  Blenheim        version 1.24.00        29/04/2004                               
	*  Blenheim        version 1.41.00        24/01/2005  + Kiosk Upgrade

   *  Buller          version 1.18.01        16/05/2003                               
   
   *  Cashmere        version 1.01.03        29/11/2001
   *  Cashmere        version 1.01.05        21/02/2002
   *  Cashmere        version 1.01.06        12/03/2002
	*  Cashmere        version 1.01.07        29/03/2002
   *  Cashmere        version 1.02.01        02/04/2002
   *  CASHMERE        version 1.07.00        31/07/2002
	*  CASHMERE        version 1.07.01        30/08/2002
   *  Cashmere        version 1.18.04        10/06/2003                               
   *  Cashmere        version 1.19.01        30/06/2003                            
   *  Cashmere        version 1.29.02        02/09/2004                            
	*  Cashmere        version 1.42.01        28/02/2005

   *  City Club       version 1.01.03        21/11/2001
   *  City Club       version 1.01.06        28/02/2002 
   *  City Club       version 1.01.07        22/03/2002
   *  City Club       version 1.02.03        09/04/2002
   *  City Club       version 1.04.01        24/05/2002
   *  City Club       version 1.04.02        18/06/2002
   *  City Club       version 1.11.02        20/11/2002
   *  City Club       version 1.18.00        14/05/2003                               


   *  COMMERCE CLUB   version 1.04.04        28/06/2002
   *  COMMERCE CLUB   version 1.05.01        02/07/2002
   *  COMMERCE CLUB   version 1.05.04        10/07/2002
   *  COMMERCE CLUB   version 1.05.05        11/07/2002
   *  Commerce Club   version 1.15.00        13/03/2003                               
	*  Commerce Club   version 1.18.01        16/05/2003
   *  Commerce        version 1.19.02        28/07/2003                            

   *  Hargens         version 1.45.00        11/07/2005
   
   *  Hornby          version 1.09.01        18/10/2002
	*  Hornby          version 1.09.02        05/11/2002
   *  Hornby          version 1.10.02        07/11/2002
   *  Hornby          version 1.11.01        13/11/2002
	*  Hornby          version 1.13.00        10/02/2003
   *  Hornby          version 1.14.02        07/03/2003                               
   *  Hornby          version 1.16.00        17/03/2003                               
   *  Hornby          version 1.16.02        02/04/2003                               
   *  Hornby          version 1.17.00        05/04/2003                               
   *  Hornby          version 1.17.01        16/04/2003                               
   *  Hornby          version 1.18.02        20/05/2003                               
   *  Hornby          version 1.18.03        28/05/2003                               
	*  Hornby          version 1.18.04        10/06/2003
   *  Hornby          version 1.19.00        25/06/2003                               
   *  Hornby          version 1.20.02        13/01/2004                               
   *  Hornby          version 1.20.03        19/01/2004                               
   *  Hornby          version 1.24.01        14/06/2004                            
   *  Hornby          version 1.40.01        23/11/2004                            
	*  Hornby          version 1.46.07        04/07/2005
	*  Hornby          version 4.03.00        07/09/2005
	*  Hornby          version 4.03.03        06/12/2005

	*  Howick          version 1.15.00        12/03/2003
	*  Howick          version 1.16.01        21/03/2003
	*  Howick          version 1.30.01        22/09/2004
	*  Howick          version 1.30.02        23/09/2004
	*  Howick          version 1.30.03        23/09/2004
	*  Howick          version 1.45.01        05/04/2005
	*  Howick          version 1.45.02        20/04/2005
	*  Howick          version 1.46.04        10/05/2005
	*  Howick          version 1.46.05        12/05/2005
	*  Howick          version 1.46.07        03/06/2005

	*  Kaipoi          version 1.24.00        23/06/2002?

	*  Kelvin Hotel    version 1.25.00        23/06/2004
   *  Kelvin Hotel    version 1.40.00        18/10/2004                            

	*  Levin           version 1.41.00        11/01/2005
	*  Levin           version 1.42.00        14/02/2005
	*  Levin           version 1.43.00        08/03/2005
   
   *  Makerewa        version 1.14.02        10/03/2003                               
   *  Makerewa        version 1.19.04        01/10/2003                               
   *  Makerewa        version 1.19.06        10/10/2003                               
   *  Makerewa        version 1.19.07        05/11/2003                               

	*  Nelson          version 1.14.01        25/02/2003

   *  Oamaru          version 1.18.04        10/06/2003                               
   *  Oamaru          version 1.18.02        27/05/2003                               
   *  Oamaru          version 1.18.03        28/05/2003                               
   *  Oamaru          version 1.19.04        22/09/2003                            
   *  Oamaru          version 1.19.01        30/06/2003                            
   *  Oamaru          version 1.24.00        29/04/2004                            

   *  OXFORD          version 1.08.01        14/10/2002
   *  OXFORD          version 1.40.01        22/11/2004
   *  OXFORD          version 1.45.01        15/04/2005
                                                          
   *  Papanui         version 1.09.02        05/11/2002
   *  Papanui         version 1.14.02        07/03/2003                               
   *  Papanui         version 1.15.01        14/03/2003                               
   *  Papanui         version 1.18.02        20/05/2003                               
   *  Papanui         version 1.18.04        10/06/2003                               
   *  Papanui         version 1.21.01        03/02/2004                            
   *  Papanui         version 1.22.00        27/02/2004                            
	*  Papanui         version 1.42.00        10/02/2005
   *  Papanui         version 1.43.00        03/03/2005  
	*  Papanui         version 1.45.00        01/04/2005
	*  Papanui         version 1.46.01        20/04/2005
	*  Papanui         version 1.46.02        04/05/2005
  	*  Papanui         version 1.46.04        11/05/2005
  	*  Papanui         version 1.47.00        07/07/2005
  	*  Papanui         version 4.00.01        02/08/2005
  	*  Papanui         version 4.02.01        02/08/2005
   
   *  Pauanui         version 1.01.02        14/11/2001
   *  Pauanui         version 1.01.04        14/12/2001   
   *  Pauanui         version 1.01.06        8/03/2002 
   *  Pauanui         version 1.01.07        18/03/2002
	*  Pauanui         version 1.02.03        12/04/2002
   *  Pauanui         version 1.03.00        19/04/2002
   *  Pauanui         version 1.03.00        06/05/2002
   *  Pauanui         version 1.03.00        06/05/2002
   *  Pauanui         version 1.04.00        13/05/2002
   *  Pauanui         version 1.04.03        13/06/2002
   *  PAUANUI         version 1.05.02        03/07/2002
   *  PAUANUI         version 1.05.03        04/07/2002
   *  PAUANUI         version 1.06.00        23/07/2002
   *  PAUANUI         version 1.06.03        30/07/2002
   *  Pauanui         version 1.09.01        31/10/2002
   *  Pauanui         version 1.11.02        20/11/2002
   *  Papanui         version 1.19.02        18/07/2003                            
   *  Pauanui         version 1.24.03        17/06/2004                            
   *  Pauanui         version 1.40.03        10/12/2004                            
   *  Pauanui         version 1.40.00        18/10/2004                            
   *  Pauanui         version 1.43.00        03/03/2005                                                                              
   *  Pauanui         version 1.46.01        29/04/2005                                                                              
   *  Pauanui         version 1.46.08        05/07/2005                                                                              
   *  Pauanui         version 4.03.02        05/10/2005                                                                              


   *  Palmerston      version 1.18.02        20/05/2003                               
	*  Palm North      version 1.18.03        28/05/2003
   *  Palm North      version 1.19.01        30/06/2003 
   *  Palm North      version 1.29.02        06/09/2004 

   *  Papatoetoe      version 1.15.00        13/03/2003                               
   *  Papatoetoe      version 1.17.00        08/04/2003                               
   *  Papatoetoe      version 1.17.02        17/04/2003                               
   *  Papatoetoe      version 1.19.04        07/10/2003                               
	*  Papatoetoe      version 1.28.00        07/10/2003
	*  Papatoetoe      version 1.42.01        28/02/2005

   *  Rawhiti Golf C  version 1.47.01         18/07/2005  ( Install Date )

	*  Richmond        version 1.16.00        17/03/2003
   *  Richmond        version 1.17.00        05/04/2003                               
   *  Richmond        version 1.17.01        08/04/2003                               
   *  Richmond        version 1.18.02        20/05/2003                               
   *  Richmond        version 1.19.02        30/07/2003                            
   *  Richmond        version 1.22.00        16/03/2004                            
   *  Richmond        version 1.23.00        19/03/2004                            
   *  Richmond        version 1.24.03        21/06/2004                            
   *  Richmond        version 1.26.01        08/07/2004  
   *  Richmond        version 1.27.00        13/07/2004  
   *  Richmond        version 1.40.03        22/12/2004  
   *  Richmond        version 4.03.03        12/12/2005  

   *  RPNYC           version 1.01.03        28/11/2001
   *  RPNYC           version 1.01.06        12/03/2002
   *  RPNYC           version 1.01.07        13/03/2002
   *  RPNYC           version 1.01.07        22/03/2002
   *  RPNYC           version 1.01.07        25/03/2002
   *  RPNYC           version 1.02.01        02/04/2002
   *  RPNYC           version 1.02.02        03/04/2002
	*  RPNYC           version 1.02.03        09/04/2002
   *  RPNYC           version 1.04.02        12/06/2002
   *  RPNYC           version 1.04.03        20/06/2002
   *  RPNYC           version 1.04.04        20/06/2002
   *  RPNYC           version 1.06.01        26/07/2002
   *  RPNYC           version 1.06.02        29/07/2002
   *  RPNYC           version 1.07.02        05/09/2002
   *  RPNYC           version 1.16.00        18/03/2003  
   *  RPNYC           version 1.16.01        20/03/2003   
   *  RPNYC           version 1.16.03        01/04/2003    
	*  RPNYC           version 1.18.04        13/06/2003                                                                                                                                   *  RPNYC      version 1.19.01   30/06/2003
   *  RPNYC           version 1.19.03        15/09/2003                            
   *  RPNYC           version 1.21.00        28/01/2004                            
	*  RPNYC           version 1.21.01        28/01/2004
   *  RPNYC           version 1.23.00        16/03/2004                            
   *  RPNYC           version 1.24.00        13/05/2004                            
   *  RPNYC           version 1.40.00        18/10/2004                            
   *  RPNYC           version 1.40.01        09/11/2004                            
   *  RPNYC           version 1.46.02        04/05/2005                            
   *  RPNYC           version 1.46.05        16/05/2005                            
   *  RPNYC           version 1.46.06        17/05/2005                            
	*  RPNYC           version 1.46.07        30/05/2005

   *  StAlbans        version 1.18.05        17/06/2003                            
   *  StAlbans        version 1.19.00        26/06/2003                            
      
   *  TE PUKE         version 1.08.01        14/10/2002
   *  TePuke          version 1.11.03        03/12/2002                               
   *  TePuke          version 1.18.00        08/05/2003                               

   *  Timaru          version 1.13.00        13/02/2003                               
   *  Timaru          version 1.40.02        01/12/2004                               
   *  Timaru          version 1.47.03        01/07/2005                               
	*  Timaru          version 4.03.03        21/11/2005

   *  Tuskers         version 1.23.00        17/03/2004                            
   *  Tuskers         version 1.23.01        06/04/2004                            
	*  Tuskers         version 1.41.02        01/02/2005
	*  Tuskers         version 1.43.02        21/09/2005

   *  UPPER HUTT      version 1.05.05        11/07/2002
   *  Upper Hutt      version 1.19.06        09/10/2003                               
   *  Upper Hutt      version 1.19.04        07/10/2003                               
   *  Upper Hutt      version 1.19.06        09/10/2003                               
   *  Upper Hutt      version 1.20.00        18/12/2003                               
   *  Upper Hutt      version 1.24.00        29/04/2004                               
	*  Upper Hutt      version 1.42.01        25/02/2005
	*  Upper Hutt      version 1.44.00        17/03/2005
	*  Upper Hutt      version 1.44.01        23/03/2005
	*  Upper Hutt      version 4.03.02        23/09/2005
	*  Upper Hutt      version 4.03.03        05/12/2005
                               
	*  Waimea          version 1.14.01        25/02/2003
   *  Waimea          version 1.19.02        28/07/2003                            
   *  Waimea          version 1.29.03        09/09/2004                        }    
    
   

   Member          = 'MEMBER';
   ReceiptFileName = 'RECEIPT.TXT';
   ExportFileName  = '\Member.csv';
   { **************************************************************************** }
   // passwords
   MasterUserName   = 'SYSDBA';
   MenuMateMasterPassword   = 'masterkey';
   MasterPassword   = 'master'; 
   MasterUserCode   =  999999;
	DefaultUserName  = 'DEFAULT';
   DefaultPassword  = 'password';

   { **************************************************************************** }
   // backups
   StockMasterBackupName = '\Stkmast.gbk';
   FilePrefix            = 'SM';
   FileSuffix            = '.GBK';
   ToolbarHeight         = 48;
   ToolbarSpace          = 5;
   { **************************************************************************** }
   // Receipts

   ReceiptLineLength     = 38;
   ReceiptDosFileName    = '\DosReceipt.txt';
   { **************************************************************************** }
   //  fields for SQL
   Male   = '''M''';
   Female = '''F''';
   Yes    = '''Y''';
   No     = '''N''';
   { **************************************************************************** }
   { Form Colors }
   fcStdForm   = clBtnFace;
   fcOther     = clTeal;
   fcForm      = clBlack;
   fcTestMode  = clYellow;
   fcGridEdit  = clInfoBk; //pay in Grids
   fcStatus    = clAqua;   //datasourse state edit / browse / insert
   PayColor    = clMaroon; //pay in Grids
   { **************************************************************************** }
   Date1899      = '12/30/1899';
   StrToDate1899 = '30/12/1899';
   // days difference between 12/30/1899( Delphi ) and 01/01/1968(AREV) for Julian Days
   ArevDateDiff = 24837;
   comma = ',';
   { **************************************************************************** }
   // Card Printing
   Esc = chr(27);
   Cr  = chr(13);
   GS  = chr(29);
   epsoncut = chr(86);
   partialcut = chr(49);

   { **************************************************************************** }
const 
    InsertPaymentsTend = 'INSERT INTO PAYMENTS_TEND (PAYMENT,TENDER_TYPE,AMOUNT,DETAILS,EX_RATE) ' +
                         'VALUES (%d,''%s'',%f,''%s'',%f)';    
   { **************************************************************************** }
   
   { Screen Codes and Screen Tabs for MemberShip}
   scMemberMenu                = 1010;

   scTenderPayments            = 2020;

   scMembersEDIT               = 3020;
   scMembersTabMaintenance     = 3021; stMembersTabMaintenance  = '  { Maintenance Tab }';
   scMembersTabProposer        = 3022; stMembersTabProposer     = '  { Proposer Tab }';
   scMembersTabLinks           = 3023; stMembersTabLinks        = '  { Links Tab }';
   scMembersTabOther           = 3024; stMembersTabOther        = '  { Other Tab }';
   scMembersTabGroupsSubs      = 3025; stMembersTabGroupsSubs     = '  { Groups/Subs Tab }';
   scMembersTabEventsSections  = 3026; stMembersTabEventsSections = '  { Events/Sections Tab }';
   scMembersTabDairy           = 3027; stMembersTabDairy        = '  { Dairy Tab }';
   scMembersTabInterests       = 3028; stMembersTabInterests    = '  { Interests Tab }';
   scMembersTabCreditPoints    = 3029; stMembersTabCreditPoints = '  { Credit/Points Tab }';

   scMembersEDIT_BRIEF         = 3030; 

   scMembersReport             = 4020;

   scMembersMaintenance        = 5020;
   scMaintenanceTabLinks       = 5021; stMaintenanceTabLinks       = '  { Links Tab }';
   scMaintenanceTabLucMem      = 5022; stMaintenanceTabLucMem      = '  { Lucky Member Tab }';
   scMaintenanceTabBulkSection = 5023; stMaintenanceTabBulkSection = '  { Bulk Section Tab }';
   scMaintenanceTabOther       = 5024; stMaintenanceTabOther       = '  { Other Tab }';
   scMaintenanceTabSetupPoints = 5025; stMaintenanceTabSetupPoints = '  { Setup Points }';
   scMaintenanceTabCards       = 5026; stMaintenanceTabCards       = '  { Cards }';
   scMaintenanceTabCardSetup   = 5027; stMaintenanceTabCardSetup   = '  { Card Setup }';
   scMaintenanceTabManualCard  = 5028; stMaintenanceTabManualCard  = '  { Manual Card }';

   scMembersPayments           = 6020;
   scPaymentsTabSubs           = 6021; stPaymentsTabSubs           =  '  { Pay by Member Tab }';
   scPaymentsTabSections       = 6022; stPaymentsTabSections       =  '  { Pay by Section Tab }';
   scPaymentsTabReprint        = 6023; stPaymentsTabReprint        =  '  { Reprint Tab }';
   scPaymentsTabBulkCardReceipt = 6024; stPaymentsTabBulkCardReceipt = '  { Bulk Card/Receipt Tab }';
   scPaymentsTabAutoPay         = 6025; stPaymentsTabAutoPay       = '  { Bulk Card/Receipt Tab }';

   
                               
   scMembersSystem             = 7020;
   scSystemTabSite             = 7021; stSystemTabSite         = '  { Site Tab }';
   scSystemTabOther            = 7022; stSystemTabOther        = '  { Others Tab }';
   scSystemTabUserSecurity     = 7023; stSystemTabUserSecurity = '  { Users/Security Tab }';
   scSystemTabLogs             = 7024; stSystemTabLogs         = '  { Logs Tab }';
   scSystemTabBackup           = 7025; stSystemTabBackup       = '  { Backup Tab }';
   scSystemTabDaily            = 7026; stSystemTabDaily        = '  { Daily Jobs Tab }';
   scSystemTabInterface        = 7027; stSystemTabInterface    = '  { Interface Tab }';
                               
   scMembersRoll               = 8020;
   scRollTabLucMem             = 8021; stRollTabLucMem         = '  { Lucky Member Rollover Tab }';
   scRollTabMember             = 8022; stRollTabMember         = '  { Member Rollover Tab }'; 
   scRollTabSection            = 8023; stRollTabSection        = '  { Sections Rollover Tab }'; 

   scMembersLook               = 9020;
   scMembersDetails            = 9030;
   scMembersLetter             = 9040;
   scLookup                    = 9060;
   scMembersEmail              = 9080;
   scMembersQuery              = 9090;

   scMainMenu                  = 10000;
   
   { **************************************************************************** }
   {const messages}
   UsersLoggedOnMsg      = 'There are other Users logged on - you may have a conflict when updating records';
   UsersMakingChangesMsg = 'There are other Users that are making changes to the Members Database ';
   //UsersMakingChangesMsg = 'There are other Users that are making changes to the Database';
   AreYouSureMsg         = 'Are you sure you want to do a ';
   PleaseWaitMsg         = ' Please Wait ...';
   PleaseTryAgainMsg     = ' has not worked, please try again';
   IncorrectDateMsg      = 'The Date is incorrect - cannot continue';
   FailedMsg             = ' has failed - changes have Not been accepted';
   RolledBackPaymentMsg  = 'The work has been rolled back and the Payment has NOT been accepted';
   NotAValidMemMsg       = 'Not a valid Member, please re-enter';
   MembersPostErrorMsg   = 'There has been a Members Post error - changes have NOT been accepted';
   DoesNotExistMsg       = ' does not exist, press the Cancel(X) button and retry';
   IsStillInUseMsg       = 'is still in use and cannot be deleted';
   CompletedMsg          = ' Completed'; 
   CantContinueMsg       = ' - cannot continue';
   DidntWorkMsg          = ' - did not work';
   HelpDeskMsg           = ' Contact the Enterprise Retail Systems HelpDesk on ';
   DirNotExistMsg        = 'Directory does not exist';
   ExtractRptMsg         = 'Extracting Data for the report ... ';
   ProtectedMsg          = 'Is Protected and cannot be deleted ... ';
   NoCardPrinterMsg      = 'No Card Printer Installed!, this can be setup in the Sytem Screen/Other tab';
   KeepExpiresCurrentMsg = 'You cannot have more than one Subscription where Keep Expires Current = Yes';
   CopyFilesMsg          = 'Copying files ...';
   CreatedFileMsg        = 'Created file successfully ';
   MemberCardMsg         = 'Member %d has already been added for a Card';
   FailedToLogExportMsg  = 'Failed to log in the Export file - changes have not been accepted';
   CancelClearMsg        = 'Cancel and Clear Changes?';
   SaveChangesMsg        = 'Save Changes?';
   DisabledServiceMsg    = 'This Service has been disabled - change in the Site tab';
   PrintDetailsMsg       =  'Print Details before deletion?';
   LockConflictMsg       = 'lock conflict on no wait transaction';
   ForeignKeyMsg         = 'violation of FOREIGN KEY constraint';
   PrimaryKeyMsg         = 'violation of PRIMARY or UNIQUE KEY constraint';
   NoRecordsMsg          = 'No records selected!';
   TestDBMsg             = '  - You are connected to the TEST DataBase';
{ **************************************************************************** }
   // Misc                 
   procedure ArrangePanel( thePAnel : TPanel ; x,y : SmallInt );
   procedure ResizeToolBar( theForm : TSMStdForm ; theWindowState : TWindowState ;
           theButtonWidth : SmallInt ; pnlWidth : SmallInt; thePanel : TPanel  );
   procedure ResizeToolBarNew( theForm : TSMStdForm ; theButtonWidth : SmallInt ;
      pnlWidth,pnlLeft : SmallInt; thePanel : TPanel );
   procedure BeforeEditPointsCreditSave(var BeforeEditPointsCredit :TBeforeEditPointsCredit;DataSet: TDataSet);


   function CalcPosition( NumComp, ThisComp, FormClientWidth, CompWidth : Integer ) : Integer;
  // procedure CheckToolbar;
   //procedure MoveToolbar(TTop,THeight : smallint);
   function ConvertToNumber( s : shortstring  ): shortstring ;
   function CreateFileAndClose( theFile : String ): Boolean ;
   function CreateFileLeaveOpen(  var F : TextFile ; theFile : String ): Boolean ;
   //function CheckCreateFile( var F : TextFile; theFile : String ): Boolean ;
   function CheckCreateFile( var F : TextFile; theFile,CopyToFile : String ): Boolean ;
   function CreateFileForExport( var F : TextFile ): Boolean ;
   procedure GetFiles(WhereFrom : String ; FileList : TStringList );
   function CopyFiles( FromStr, ToStr : String  ; bFailifExists : Boolean ): Boolean;
   procedure DeleteOldFiles( FilePath, FilePrefix,FileExt : shortstring; KeepDays:ShortInt );
   function MakeShortFileName( s : string ) : string;
   function DOSExec( ProgName : String ) : boolean ;
   function GetOpSystem : string ;
   function ReadComputerName : string;
   function ReadUserName     : string;
   function ReadReg : Boolean ;
   procedure EnsureSingleInstance( const ApplicationTitle : String );
   function CheckSiteLicense : Boolean;
   function AddOneYear( FromDate : TDateTime ) : TDateTime;
   procedure RemoveTab( tabName : TTabSheet );
   procedure MDlgI( const s : string );
   procedure MemberDeleteReport( Member : LongInt );
   function CentreText( str : String ; LineLength : smallint ) : String;
   function LeftAdjustRightSpaces(str : String; LineLength : smallint ) : String;
   function RightAdjustLeftSpaces(str : String; LineLength : smallint ) : String;
   procedure SetNumberType( i : integer );
   function WorkOutAge( DOB : TDateTime ) : smallint;
   function NextMonthExpires( mExpires : TDateTime ) : Boolean;
   function GetCommaDelimitedField(FieldNum : shortint ; s : String) : String ;
        
   procedure CreateAsciiFile( FName:string ; AddDate:Boolean ; FromQry:TIBQuery; Str:TStrings; ShowMess,CommaDel:Boolean);
   function PrintFile( Filename : String ) : Boolean;
   procedure PrintBlankLines(BlankLines : ShortInt );
   function PrintFileRichEdit( Filename : String ) : Boolean;
   procedure CardPrint( Member,Expires : ShortString ; NumberCards : ShortInt ; theTransaction : TIBTransaction);
   procedure CardEncode( Member : ShortString ; Expires : TDateTime);
   procedure CardEject;
   procedure CardReadTrack2;
   procedure ResetEncoder;

   function CardPrinterAvailable : Boolean;
   procedure ExportCards(ShowMess : Boolean);
   procedure GetCardsFromLinks( GroupsList,SectionsList,SubsList : TStringList; Member : LongInt );
   function GetCardType( MEMBER : LongInt) : ShortString;

   function GetMemberCount : Shortstring;

   //function OpenDOSFile( OutFile : TextFile ; Filename : String ) : Boolean;
   function RawDataToPrinter(szDocName,szPrinterName,DataStr : String;ReadPrinterData : Boolean) : Boolean;
   function ReadFromPrinter(szDocName,szPrinterName,DataStr : String; ReadPrinterData : Boolean) : Boolean;

   // Tables Queries
   procedure QryCloseIfActive( theQry : TIBQuery );
   procedure TblCloseIfActive( theTbl : TIBTable );
   procedure TblCloseIfActiveFiltered( theTbl : TIBTable );
   procedure TblActiveOrRefresh ( theTbl : TIBTable );
   procedure IBDSActiveOrRefresh ( theSQL : TIBDataSet );
   procedure QryActiveOrRefresh ( theQry : TIBQuery );
   procedure TblReActivate( theTbl : TIBTable );
   procedure QryReActivate( theQry : TIBQuery );
   procedure TblSetFiltered ( theTbl : TIBTable ; filterStr : String );
   

   //DAtes
   function CheckSystemDate ( Present : TDateTime ) : Boolean ;
   function AddDays( FromDate : TDateTime; ExpiryDate : TDateTime ) : TDateTime;
   function AlterExpireDateAbove(MExpires : TDateTime; GoodDate : TDateTime ): TDateTime;
   function AlterExpireDateBelow(MExpires : TDateTime; GoodDate : TDateTime ): TDateTime;
   function AddYears( FromDate : TDateTime; Yrs : ShortInt ) : TDateTime;
   function GetShortMonthDate ( theDate : TDateTime ) : String;
   function GetShortDate ( theDate : TDateTime ) : String;
   function GetShortDateTime ( theDateTime : TDateTime ) : String;
   function ConvertDateStrtoStr ( s : String ) : String;
   function GetShortMonthDateTime ( theDate : TDateTime ) : String;
   
      
   // Dialogs
   procedure SetupSaveDialog( SetupExt,SetupFilter,SetupTitle,SetupFileName,Dir : ShortString );
   procedure SetupOpenDialog(  SetupExt,SetupFilter,SetupTitle : ShortString );
   function IfMsgDlg( const s : string ): Boolean;
   procedure sm( s : String);
   procedure smi( s : Integer);
   procedure smf( s : extended);

   // hardware routines
   function GetHDSerialNumber : String ;
   procedure GridLoadPrinters (  cbx : TwwDBComboBox );
   procedure ReadPrinters ;
   procedure OpenCashDrawer;
   procedure OpenCashDrawerPrinter;
   procedure OpenCashDrawerComPort;
   procedure ResetPrinter;

   // string routines
   function RightSpaces( const len : integer ;s : shortstring ): shortstring ;
   function StrToBoolean ( value : string ) : Boolean;
   function BooleanStr ( value : Boolean ) : string;
   function AllDigits( const S : String ) : Boolean;
   function AllDigitsChar( const c : char ) : Boolean;
   function RemoveSignSpaceConvert(s : string) : extended;
   function RemoveSignSpace(s : string) : string;
   function DeleteLastComma(s : string) : string;
   procedure GetAddressList( Address,Address1,District,City,Country : ShortString ;AddressList : TStrings);
   procedure AddToClipBoard( Str : String );
   function GetAddressAsString(Address,District,Address1,City,Country : ShortString ): ShortString;
   function FirstLetterUpper ( str : string ) : String;

   // security
   function XorEncode(const Key, Source: string): string;
   function XorDecode(const Key, Source: string): string;
   function GetLicenseNumber ( s : string ) : string;

   // Floats Integer
   function RoundCurrency( const Value : Extended ) : Extended;

   // Enumerated
   function enmPrefixImportTypes( const str : ShortString ) : TPrefixImportTypes;
   function ATableField(  str : ShortString ) : Boolean;
   function GetFieldNamesArray(  str : ShortString ) : ShortInt;

implementation
{ **************************************************************************** }
procedure EnsureSingleInstance( const ApplicationTitle : String );
var
   Wnd : HWnd;
   WndClass, WndText : array[0..255] of Char;
begin
   if ( CreateSemaphore( nil, 0, 1, PChar( ExtractFileName( Application.ExeName ))) <> 0 ) and
      ( GetLastError = Error_Already_Exists ) then begin
      Wnd := GetWindow( Application.Handle, gw_HWndFirst );
      While ( Wnd <> 0 ) do begin
         if ( Wnd <> Application.Handle ) then begin
            GetClassName( Wnd, WndClass, Pred( SizeOf( WndClass ) ) );
            GetWindowText( Wnd, WndText, Succ( Length( ApplicationTitle ) ) );
            if ( StrPas( WndClass ) = Application.ClassName ) and
               ( StrPas( WndText  ) = ApplicationTitle ) then begin
               PostMessage( Wnd, wm_SysCommand, sc_Restore, 0 );
               SetForeGroundWindow( Wnd );
               Halt;
            end;
         end;
         Wnd := GetWindow( Wnd, gw_HWndNext );
      end;
   end;
end;
{ **************************************************************************** }
function IfMsgDlg( const s : string ): Boolean;
begin
    if MessageDlg ( s ,mtConfirmation,[mbYes,mbNo],0) = mrNo then
       result := False
    else   
       result := True;
end;    
{ **************************************************************************** }
procedure HelpDeskMessageDlg( const s : string );
begin
   MessageDlg ( s + #13#10 +
                'Please contact the ERS HelpDesk on ' + HelpDesk,mtError,[mbOK],0);
end;
{ **************************************************************************** }
procedure MDlgI( const s : string );
begin
     MessageDlg ( s ,mtInformation,[mbOK],0);
end;
{ **************************************************************************** }
procedure sm( s : String);
begin
     MessageDlg ( s,mtError,[mbOK],0);
end;
{ **************************************************************************** }
procedure smi( s : Integer);
begin
     showmessage( IntToStr(s) );
end;
{ **************************************************************************** }
procedure smf( s : extended);
begin
     showmessage( FormatFloat('#####.00',s) );
end;
{ **************************************************************************** }
function AllDigits( const S : String ) : Boolean;
{ Returns True if S contains only digits 0-9 }
type  TCharSet = set of Char;
const Digits : TCharSet = ['0'..'9'];
var
   i : Integer;
begin
   Result := False;
   if s = '' then exit;
   for i := 1 to Length( S ) do begin
      If not ( S[i] in Digits ) then
         Exit;
   end;
   Result := True;
end;
{ **************************************************************************** }
function AllDigitsChar( const c : char ) : Boolean;
type  TCharSet = set of Char;
const Digits   : TCharSet = ['0'..'9'];
begin
   Result := False;
   if ( c in Digits ) then Result := True;
end;
{ **************************************************************************** }
function GetHDSerialNumber : String ;
var
  VolumeSerialNumber :DWORD;
  MaxComponentLength :DWORD;
  FileSystemFlags :DWORD;
begin
    GetVolumeInformation('C:\',nil,0,@VolumeSerialNumber,MaxComponentLength,FileSystemFlags,nil,0);
    Result := IntToHex(HiWord (VolumeSerialNumber),4) + '-' +
                    IntToHex(LoWord (VolumeSerialNumber),4);
end;
{**************************************************************************}
function CheckSiteLicense : Boolean;
   { calculates registration number for SiteName and checks against SiteLicense }
const                
   XORMask = $AAf4;
var
   i : Integer;
   ActualSiteLicense : LongInt;
begin
   Result := False;
   ActualSiteLicense := 0;
   If CompName = '' then Exit;
   For i:= 1 to Length( CompName ) do
      ActualSiteLicense := ActualSiteLicense + ( Ord( CompName[i] ) xor XORMask )*i;
   //If Name = DefaultStoreName then RegNo := ActualRegNo;
   If ActualSiteLicense = SiteLicense then Result := True;

end;
{**************************************************************************}
function StrToBoolean ( value : string ) : Boolean;
begin
     result := True;
     if (value = '') or ( value[1] in ['F','f','0','N','n']) then
        result := False;
end;
{ **************************************************************************** }
function BooleanStr ( value : Boolean ) : string;
begin
     if value then result := 'Y'
        else result := 'N';
end;
{ **************************************************************************** }
function XorEncode(const Key, Source: string): string;
var
  I: Integer;
  C: Byte;
begin
  Result := '';
  for I := 1 to Length(Source) do begin
    if Length(Key) > 0 then
      C := Byte(Key[1 + ((I - 1) mod Length(Key))]) xor Byte(Source[I])
    else
      C := Byte(Source[I]);
    Result := Result + AnsiLowerCase(IntToHex(C, 2));
  end;
end;
{ **************************************************************************** }
function XorDecode(const Key, Source: string): string;
var
  I: Integer;
  C: Char;
begin
  Result := '';
  for I := 0 to Length(Source) div 2 - 1 do begin
    C := Chr(StrToIntDef('$' + Copy(Source, (I * 2) + 1, 2), Ord(' ')));
    if Length(Key) > 0 then
      C := Chr(Byte(Key[1 + (I mod Length(Key))]) xor Byte(C));
    Result := Result + C;
  end;
end;
{ **************************************************************************** }
function GetLicenseNumber ( s : string ) : string;
var i : integer;
begin
     result := '';
     for i := 1 to length(s) do
        if s[i] in ['0'..'9'] then result := result + s[i];
end;
{ **************************************************************************** }
function AddOneYear( FromDate : TDateTime ) : TDateTime;
var
   Yr, Mnth, Day: Word;
// add one year to FromDate - if next year is a leap year add 1 day if the 29/02
begin
     DecodeDate(FromDate, Yr, Mnth, Day);   // if date in MemberSYS is blank
     inc(Yr);  // add 1 to year
     if (IsLeapYear(Yr)) and (Mnth = 2) and (Day = 29) then begin
        Day  := 1;   // leap yr next year - add 1 day  to 1/3/20??
        Mnth := 3;
     end;
     Result := EncodeDate(Yr, Mnth, Day); 
end;
{ **************************************************************************** }
function AddYears( FromDate : TDateTime; Yrs : ShortInt ) : TDateTime;
var
   Yr, Mnth, Day: Word;
// add yrs (could be a negative value )to FromDate -
// if next year is a leap year add 1 day if the 29/02
begin
     DecodeDate(FromDate, Yr, Mnth, Day);   // if date in MemberSYS is blank
     Yr := Yr + Yrs;  // add years
     if (IsLeapYear(Yr)) and (Mnth = 2) and (Day = 29) then begin
        Day  := 1;   // leap yr next year - add 1 day  to 1/3/20??
        Mnth := 3;
     end;
     Result := EncodeDate(Yr, Mnth, Day);
end;
{ **************************************************************************** }
function AddDays( FromDate : TDateTime; ExpiryDate : TDateTime ) : TDateTime;
var
   FYr, FMnth, FDay: Word;
   EYr, EMnth, EDay: Word;
// takes from date and adds the appropriate # of days to it - keeps an eye on years
// ExpiryDate is always > FromDate
begin
     DecodeDate(FromDate, FYr, FMnth, FDay);
     DecodeDate(ExpiryDate, EYr, EMnth, EDay);
     if EYr <> Fyr then begin
        Eyr := Eyr + (Eyr-Fyr);
     end;
     if (IsLeapYear(FYr)) and (FMnth = 2) and (FDay = 29) then begin
        FDay  := 1;   // leap yr next year - add 1 day  to 1/3/20??
        FMnth := 3;
     end;
     Result := EncodeDate(EYr, FMnth, FDay); // ExpiryYear From Month and DAy
end;
{ **************************************************************************** }
function CheckSystemDate ( Present : TDateTime ) : Boolean ;
var
   Yr, Mnth, Day: Word;
begin
     Result := False;
     DecodeDate( Present, Yr, Mnth, Day);
     if (Yr  < 2000) or (Yr > 3000) then exit;
     if (Mnth < 01)  or (Mnth > 12 ) then exit;
     if Mnth = 02 then begin
        if (IsLeapYear(Yr)) and (Day < 01) or (Day > 29) then exit;
     end else begin
        if (Day < 01 )  or (Day > 31 ) then exit;
     end;
     Result := True;
end;
{ **************************************************************************** }
function RoundCurrency( const Value : Extended ) : Extended;
{ Returns Value rounded to two decimal places }
begin
   try
      Result := Round( Value * 100 ) / 100.0;
   except
      On EInvalidOp do
         Result := Value;
   end;
end;
{ **************************************************************************** }
function RemoveSignSpaceConvert(s : string) : extended;
var sCredit : Boolean;
begin
     sCredit := False;
     while POS('$',s) > 0 do // remove the dollar
        delete(s,POS('$',s),1);
     while POS(',',s) > 0 do // remove comma
        delete(s,POS(',',s),1);
     // remove any quotes in case it was a loke "$1,100" cww 03/12/04   
     while POS('"',s) > 0 do 
        delete(s,POS('"',s),1);

     s := trim(s);           
     //while POS(' ',s) > 0 do // remove the spaces
     //   delete(s,POS(' ',s),1);
     while POS('(',s) > 0 do begin// credit = (26.00)
        delete(s,POS('(',s),1);
        sCredit := True;
     end;   
     while POS(')',s) > 0 do // credit
        delete(s,POS(')',s),1);
     if s = '' then s := '0.00';
     if sCredit then s := '-' + s;
     try
        Result := StrToFloat(s);
   except
       On EInvalidOp do
          Result := -9999999.00;
   end;
end;
{ **************************************************************************** }
function RemoveSignSpace(s : string) : string;
begin
     while POS('$',s) > 0 do // remove the dollar
        delete(s,POS('$',s),1);
     while POS(' ',s) > 0 do // remove the spaces
        delete(s,POS(' ',s),1);
     // remove any quotes in case it was a loke "$1,100" cww 03/12/04   
     while POS('"',s) > 0 do 
        delete(s,POS('"',s),1);
     result := s;   
end;
{ **************************************************************************** }
function RightSpaces( const len : integer ; s : shortstring ): shortstring ;
var i : Integer;
begin
     Result := s;
     for i := (len + 1) to Length(Result) do
        Result[i] := ' ';
end;
{ **************************************************************************** }
function AlterExpireDateBelow(MExpires : TDateTime; GoodDate : TDateTime ): TDateTime;
var
   MYr, MMnth, MDay: Word;
   GYr, GMnth, GDay: Word;
   SMnth, SDay: Word;
begin
     try
        DecodeDate(GoodDate, GYr, GMnth, GDay);
        DecodeDate(MExpires, MYr, MMnth, MDay);
        // bring date up to the the latest Year but below GoodDate
        // eg MExpires = 01/05/1998  GoodDate = 07/07/2000 then Result = 01/05/2000
        SMnth := MMnth;
        SDay  := MDay;
        // put date Less than GoodDate
        if MExpires = GoodDate then
           exit
        else if MExpires > GoodDate then begin
           repeat
              dec(MYr);
              if (IsLeapYear(MYr)) and (MMnth = 2) and (MDay = 29) then begin
                 MDay  := 1;   // leap yr next year - add 1 day  to 1/3/20??
                 MMnth := 3;
              end;
              MExpires := EncodeDate(MYr, MMnth, MDay);
           until
              MExpires < GoodDate;
        end else begin
        // put date greater than GoodDate - then reduce by one year
           repeat
              inc(MYr);
              if (IsLeapYear(MYr)) and (MMnth = 2) and (MDay = 29) then begin
                 MDay  := 1;   // leap yr next year - add 1 day  to 1/3/20??
                 MMnth := 3;
              end;
              MExpires := EncodeDate(MYr, MMnth, MDay);
           until
              MExpires > GoodDate;
           dec(MYr);
        end;
        // Work off the Saved SMnth and SDay as these could have altered along the way
        if (IsLeapYear(MYr)) and (SMnth = 2) and (SDay = 29) then begin
           MDay  := 1;
           MMnth := 3;
        end;
     finally
        Result := EncodeDate(MYr, MMnth, MDay);
     end;
end;
{ **************************************************************************** }
function AlterExpireDateAbove(MExpires : TDateTime; GoodDate : TDateTime ): TDateTime;
var
   MYr, MMnth, MDay: Word;
   GYr, GMnth, GDay: Word;
   SMnth, SDay: Word;
begin
     try
        DecodeDate(GoodDate, GYr, GMnth, GDay);
        DecodeDate(MExpires, MYr, MMnth, MDay);
        // bring date up to the the latest Year but ABOVE GoodDate
        // eg MExpires = 01/05/1998  GoodDate = 07/07/2000 then Result = 01/05/2001
        SMnth := MMnth;
        SDay  := MDay;
        // put date Less than GoodDate then add 1 year
        if MExpires = GoodDate then
           inc(MYr)
        else if MExpires > GoodDate then begin
           repeat
              dec(MYr);
              if (IsLeapYear(MYr)) and (MMnth = 2) and (MDay = 29) then begin
                 MDay  := 1;
                 MMnth := 3;
              end;
              MExpires := EncodeDate(MYr, MMnth, MDay);
           until
              MExpires < GoodDate;
           inc(MYr);
        end else begin
        // put date greater than GoodDate
           repeat
              inc(MYr);
              if (IsLeapYear(MYr)) and (MMnth = 2) and (MDay = 29) then begin
                 MDay  := 1;   // leap yr next year - add 1 day  to 1/3/20??
                 MMnth := 3;
              end;
              MExpires := EncodeDate(MYr, MMnth, MDay);
           until
              MExpires > GoodDate;
        end;
        // Work off the Saved SMnth and SDay as these could have altered along the way
        if (IsLeapYear(MYr)) and (SMnth = 2) and (SDay = 29) then begin
           MDay  := 1;
           MMnth := 3;
        end;
     finally
        Result := EncodeDate(MYr, MMnth, MDay);
     end;
end;
{ **************************************************************************** }
procedure CreateAsciiFile( FName:string ; AddDate:Boolean ; FromQry:TIBQuery; Str:TStrings; ShowMess,CommaDel :Boolean );
const
   fileext    = '.TXT';
var
   Present: TDateTime;
   Year, Month, Day : Word;
   AsciiFile    : TextFile;
   i            : integer;
   DataTypeList : TStringList;
   s,asDataType : string;
   FieldType    : TFieldType;
   singlesep    : string[1]; // " or null
   sep          : string[3]; // , or ","
   
label            skipnext;
begin
   try
      try
         if AddDate then begin
            Present:= Now;
            DecodeDate(Present, Year, Month, Day);
            FName := FName + IntToStr(Day) + IntToStr(Month) + IntToStr(Year)+ Fileext;
         end;
       LastFileExported := FName;
   	 Assignfile ( AsciiFile ,  FName );
     	 Rewrite ( AsciiFile );
      except
         showmessage ( 'Cannot create file ' + FName);
         exit;
      end;
      // heading to the file if used 
      if Str <> nil then begin
         for i := 0 to Str.Count -1 do begin
            WriteLn(AsciiFile,sep + Str[i] + sep);
         end;
      end;
      if CommaDel then begin  // comma delimeted
         singlesep := '';
         sep       := ',';
      end else begin   
         singlesep := '"';
         sep       := '","';
      end;
      DataTypeList := TStringList.Create;
      // put the fields on the top line
      s := singlesep;
      for i := 0 to FromQry.FieldCount - 1 do begin
         s := s +  FromQry.Fields[i].DisplayName + sep;
      end;
      if CommaDel then   // comma delimeted
         delete(s,length(s) ,1 )  // remove last ,
      else   
         delete(s,length(s) -2 ,2 ); // remove last ,"
      WriteLn(AsciiFile,s);
      // build the list of datatypes for the field
   	for i := 0 to FromQry.FieldCount - 1 do begin
         Fieldtype := TFieldType(FromQry.Fields[i].DataType); {datatype from field}
         case FieldType of
            ftUnknown:   asDataType := 'asString';
            ftString:    asDataType := 'asString';
            ftSmallint:  asDataType := 'asInteger';     
            ftInteger:   asDataType := 'asInteger';
            ftWord:      asDataType := 'asInteger';
            ftBoolean:   asDataType := 'asBoolean';
            ftFloat:     asDataType := 'asFloat';
            ftCurrency:  asDataType := 'asFloat';
            ftBCD:       asDataType := 'asString';
            ftDate:      asDataType := 'asDateTime';
            ftTime:      asDataType := 'asDateTime';
            ftDateTime:  asDataType := 'asDateTime';
            ftBytes:     asDataType := 'asString';
            ftVarBytes:  asDataType := 'asString';
            ftBlob:      asDataType := 'asBlob';
            ftMemo:      asDataType := 'asBlob';
         else
            asDataType := 'asString';
         end; { end case }
         DataTypeList.Add(asDataType);
      end; {for}
      FromQry.First;
      while not FromQry.EOF do begin
         s := singlesep;
         for i := 0 to FromQry.FieldCount - 1 do begin
            if DataTypeList[i] = 'asString' then begin
               s := s + FromQry.Fields[i].asString + sep;
               goto skipnext;
            end;
            if DataTypeList[i] = 'asDateTime' then begin
               s := s + DateTimeToStr(FromQry.Fields[i].asDateTime) + sep;
               goto skipnext;
            end;
            if DataTypeList[i] = 'asInteger' then begin
               s := s + IntToStr(FromQry.Fields[i].asInteger) + sep;
               goto skipnext;
            end;
            if DataTypeList[i] = 'asFloat' then begin
               s := s + formatfloat('$###.00',FromQry.Fields[i].asFloat) + sep;
               goto skipnext;
            end;
            s := s + FromQry.Fields[i].asString + sep;
            skipnext:;
         end; {for}
         if CommaDel then   // comma delimeted
            delete(s,length(s) ,1 )  // remove last ,
         else   
            delete(s,length(s) -2 ,2 ); // remove last ,"
         WriteLn(AsciiFile,s);
         FromQry.next;
      end; { while }
      if showmess then
         MdlgI( CreatedFileMsg + FName);
   finally
      flush(AsciiFile);
      closefile(AsciiFile);
      DataTypeList.Free;
   end;
end;
{ **************************************************************************** }
function CentreText( str : String ; LineLength : smallint ) : String;
var i,Len : smallint;
begin
     Result := StringOfChar(' ', (LineLength - Length(str)) div 2) + str;
     len := length(Result) + 1;
     setlength( Result,LineLength );
     for i := len  to Length(Result) do
        Result[i] := ' ';
end;
{ **************************************************************************** }
function LeftAdjustRightSpaces(str : String; LineLength : smallint ) : String;
var i,Len : smallint;
begin
     len := length(Str) + 1;
     Result := str;
     setlength( Result,LineLength );
     if len  = Length(Result) then
       Result[Length(Result)] := ' ';
     for i := len  to Length(Result) do
        Result[i] := ' ';

end;
{ **************************************************************************** }
function RightAdjustLeftSpaces(str : String; LineLength : smallint ) : String;
var i,Len : smallint;
begin
     Len := LineLength - length(Str);
     setlength( Result,Len );
     for i := 1  to Len do
        Result[i] := ' ';
     Result := Result + Str;
end;
{ **************************************************************************** }

function PrintFileRichEdit( Filename : String ) : Boolean;
Var RichEdit : TRichEdit;
begin
     {RichEdit := TRichEdit.Create(nil);
     try
             RichEdit.Lines.Clear;
        RichEdit.PlainText := True;
        RichEdit.Lines.LoadFromFile(FileName);
        RichEdit.Print('Receipt');
     finally
        RichEdit.Free;
     end;     }
end;
{******************************************************************************}
procedure PrintBlankLines( BlankLines : ShortInt );
var P  : Text;
    i : shortint;
begin
   if BlankLines = 0 then exit;
   try
      AssignPrn( P );
      Rewrite( P );
      for i := 0 to BlankLines do
         WriteLn( P, '.' );
   finally
      // prints here
      System.CloseFile( P );
   end;
end;
{ **************************************************************************** }
function PrintFile( Filename : String ) : Boolean;
{  Prints file passed as parameter to default windows printer.
   Returns True if file printed
   Returns False if file not printed
   Note : Do not call again while printing in Progress
}
Var
   Line  : String;
   P     : Text;
   F     : Text;
Begin
   Result := False;
   If not FileExists( Filename ) then begin
      Exit;
   End; { if }

   { init printer }
   Printer.Orientation := poPortrait;
   AssignPrn( P );
   Rewrite( P );
   Printer.Canvas.Font.Name := 'Arial'; // not a true type font - proportional
   Printer.Canvas.Font.Size := 10;

   { Open file to print }
   AssignFile( F, Filename );
   Reset( F );

   { Build print job }
   Repeat
      ReadLn( F, Line );
      WriteLn( P, Line );
   Until EOF( F );

   CloseFile( F );
   { Print }
   System.CloseFile( P );

   Result := True;
end;
{ **************************************************************************** }
function ReadReg : Boolean ;
const regSection    = '\SOFTWARE\IQWORKS\MenuMate\Membership';
      OldregSection = '\Software\Membership';
      regIBSection  = '\Software\Borland\InterBase\CurrentVersion';
var   reg : TRegistry; 
      Oldreg : TRegistry; 
      regLogonStr : ShortString;
begin             
     Result := False;
     try
        try
           reg := TRegistry.Create;
           reg.RootKey  := HKEY_LOCAL_MACHINE;
           with reg do begin
              if OpenKey(regSection, False) then begin
                 regServerName            := trim(ReadString ( 'ServerName'));
                 regDatabaseName          := trim(ReadString ( 'DatabaseName'));
                 regMenuMateServerName    := trim(ReadString ( 'MenuMateServerName'));
                 regMenuMateDatabaseName  := trim(ReadString ( 'MenuMateDatabaseName' ));
                 regLogonStr              := ReadString      ( 'UserLogon' );
                 regLogon := False;
                 if (regLogonStr = '1') or (regLogonStr = 'Yes') then
                    regLogon := True;
                 regLocalUserName         := ReadString      ( 'LocalUserName');
                 Result := True;
              end else begin
                 // try copying accross from the Old Registry
                 Oldreg := TRegistry.Create;
                 Oldreg.RootKey  := HKEY_CURRENT_USER;
                 try
                    if Oldreg.OpenKey(OldregSection, False) then begin
                       regServerName            := trim(Oldreg.ReadString ( 'ServerName'));
                       regDatabaseName          := trim(Oldreg.ReadString ( 'DatabaseName'));
                       regMenuMateServerName    := trim(Oldreg.ReadString ( 'MenuMateServerName'));
                       regMenuMateDatabaseName  := trim(Oldreg.ReadString ( 'MenuMateDatabaseName' ));
                       regLogonStr              := Oldreg.ReadString      ( 'UserLogon' );
                       regLogon := True;
                       if (regLogonStr = '1') or (regLogonStr = 'Yes') then
                          regLogon := True;
                       regLocalUserName         := Oldreg.ReadString      ( 'LocalUserName');
                       // and write to the new program files
                       if reg.OpenKey(regSection, True) then begin
                          Reg.WriteString ('ServerName', regServerName);
                          Reg.WriteString ('DatabaseName',regDatabaseName);
                          Reg.WriteString ('MenuMateServerName', regMenuMateServerName );
                          Reg.WriteString ('MenuMateDatabaseName',regMenuMateDatabaseName);
                          Reg.WriteString ('UserLogon',  regLogonStr );
                          Reg.WriteString ('LocalUserName', regLocalUserName );
                          Result := True;
                       end;   
                    end;
                 finally
                    Oldreg.CloseKey;
                    Oldreg.Free;
                 end;
              end;   
           end; 
        except                              
           sm('Cannot read the registry - cannot continue');
        end;   
     finally
        Reg.CloseKey;
        reg.Free;
     end;   
     if Result = False then begin
        frmSetup := TfrmSetup.Create(nil);
        try
           frmSetup.ShowModal;
        finally   
           frmSetup.Free;
        end;   
     end;
end;
{ **************************************************************************** }
function ReadComputerName : string;              
// Get the Network Name of the Computer
//var
//   pCompName : Array[0..MAX_COMPUTERNAME_LENGTH+1] of Char;
//   pSize     : Dword;
begin
   result := dm.SysInfo.ComputerName;
  // pSize := MAX_COMPUTERNAME_LENGTH +1;           
  // if GetComputerName(pCompName,pSize) then
   //   result := strPas(pCompName)       
  // else
  //    result := '';
end;                   
{ **************************************************************************** }
function ReadUserName : string;
// Get the Network Name of the User
const
   MAX_USERNAME_LENGTH = 20;
var
   pUserName : Array[0..MAX_USERNAME_LENGTH+1] of char;
   pSize     : dWord;
begin
   pSize := MAX_USERNAME_LENGTH;
   if getusername(pUserName,pSize) then
       result := strPas(pUserName)
   else
       result := '';
end;
{ **************************************************************************** }
function GetOpSystem : string ;
var OSVersionInfo: TOSVersionInfo;
    WinVer : string;                    
begin
     OSVersionInfo.dwOSVersionInfoSize := SizeOf(OSVersionInfo);
     if GetVersionEx(OSVersionInfo) then begin
     with OSVersionInfo do begin
        WinVer := Format('%u.%.2u', [dwMajorVersion, dwMinorVersion] );
        case dwPlatformID of 
           0 : Result := '3.x';
           1 : Result := '95 or 98';  // no difference between them
           2 : Result := 'NT';
        else
           Result := 'Unknown';   
        end;    
        Result :=  Format( 'Windows %s Version %s %s', [ Result, WinVer,szCSDVersion ] ); 
      end;
  end;   
end;
{ **************************************************************************** }
function DOSExec( ProgName : String ) : boolean ;
var
   CmdLine     : String;
   CmdShow     : Integer;
   StartupInfo : TStartupInfo;
   ProcessInfo : TProcessInformation;
   ResultWait  : Integer;
   ErrorCode   : Integer;
begin                
   Result := False;
   CmdLine := ProgName;
   CmdShow := SW_NORMAL;
   FillChar( StartupInfo, sizeof( StartupInfo ), 0 );
   with StartupInfo do begin
      cb := sizeof( StartupInfo );
      dwFlags := STARTF_USESHOWWINDOW;
      wShowWindow := CmdShow;
   end;   		
   Result := CreateProcess( Nil, PChar( CmdLine ),Nil, Nil, False,
                            NORMAL_PRIORITY_CLASS, Nil, Nil,
                            StartupInfo, ProcessInfo );
   If not Result then begin
      ErrorCode := GetLastError;
      Exit;
   end;   
   Application.ProcessMessages;
   Repeat
         // Wait for External Process but give App Repaint time etc every 100ms
       ResultWait := WaitforSingleObject(ProcessInfo.hProcess,100);
       Application.ProcessMessages;
   Until not ( ResultWait = WAIT_TIMEOUT );   
   CloseHandle( ProcessInfo.hProcess );
   CloseHandle( ProcessInfo.hThread );
end;
{ **************************************************************************** }
function MakeShortFileName( s : string ) : string;
var s1 : shortstring;
begin
     Result := copy(s,1,pos('\',s));
     while POS('\',s) > 0 do begin
        system.delete(s,1,pos('\',s)); // delete up to next '\'
        if POS('\',s) > 0 then begin  // check for next '\' 
           s1 := copy(s,1,POS('\',s) -1); // read up to next '\' - remove it
           if length(s1) > 6 then 
              s1 := copy(s1,1,6) + '~1\' // make into short name 
           else
              s1 := s1 + '\';  // add '\' back on   
        end else begin  // no more \
           s1 := s;
           if length(s1) > 6 then 
              s1 := copy(s1,1,6) + '~1'; // make into short name 
           if POS('.',s) > 0 then    // keep extension
              s1 := s1 + copy(s,POS('.',s),length(s)); // read up to . and copy rest
        end;
        Result := Result + s1; 
     end;
end;
{ **************************************************************************** }
procedure DeleteOldFiles( FilePath, FilePrefix,FileExt : shortstring ; KeepDays : ShortInt ); 
const
   asterisk = '*';
var
   SearchRec : TSearchRec;
   stlFileNames : TStringList;
   i : Integer;
   strDate : string[12];
   DeleteFileDate : TDateTime;
begin
   stlFileNames := TStringList.Create;
   try
      If ( FindFirst( FilePath + '\' + FilePrefix + asterisk + FileExt , faAnyFile, SearchRec ) = 0 ) then begin
         Repeat
            stlFileNames.Add( SearchRec.Name );
         Until ( FindNext( SearchRec ) <> 0 );
         For i := 0 to ( stlFileNames.Count - 1 ) do begin
            try                            //smddmmyy.gbk
               strDate := copy(stlFileNames.strings[i],3,6);
               DeleteFileDate := StrToDateTime(copy(strDAte,1,2) + '/' + copy(strDAte,3,2) + '/' + copy(strDAte,5,4));
            except
               // forget about these files - pick them up next time - or not sm ones
               on EConvertError do
                  continue;
            end;
            if DeleteFileDate < (Date - KeepDays) then
               sysutils.deletefile( FilePath + '\' + stlFileNames.strings[i]);
         end;
      end;
   finally
      stlFileNames.Free; 
   end;
end;                 
{ **************************************************************************** }
procedure RemoveTab( tabName : TTabSheet );
begin
     tabName.TabVisible := False;
     //tabName.Enabled := False;
     //tabName.Visible := False; 
end;
{ **************************************************************************** }
function enmPrefixImportTypes( const str : ShortString ) : TPrefixImportTypes;
var i : ShortInt;
begin
     Result := piError;
     for i := 0 to MaxPrefixImportTypes do begin
      if ( str = PrefixImportTypes[i] ) then begin
         Result := TPrefixImportTypes(i);
         Break;
      end;          
   end;          
end;
{ **************************************************************************** }
function ATableField(  str : ShortString ) : Boolean;
var i : ShortInt;
begin
     Result := True;
     if str = 'CARD TYPE' then begin
        Result := False;
        exit;
     end;
     for i := 0 to MaxFieldNames do begin
      if ( str = FieldNamesArray[i] ) then begin
         Result := False;
         Break;
      end;
   end;
end;
{ **************************************************************************** }
function GetFieldNamesArray(  str : ShortString ) : ShortInt;
var i : ShortInt;    
begin
     Result := -1;
     if str = 'CARD TYPE' then begin
        Result := MaxFieldNames + 1;
        exit;
     end;          
     for i := 0 to MaxFieldNames do begin
        if ( str = FieldNamesArray[i] ) then begin
           Result := i;
           Break;
        end;          
     end;
end;
{ **************************************************************************** }
function GetShortDateTime ( theDateTime : TDateTime ) : String;
begin
     Result := formatdatetime('mm/dd/yyyy hh:mm:ss',theDateTime);
end;
{ **************************************************************************** }
function GetShortDate ( theDate : TDateTime ) : String;
begin
     try
        Result := formatdatetime('mm/dd/yyyy',theDate);
     except
        sm('proc GetShortDate format date and time error'); 
     end;   
end;
{ **************************************************************************** }
function GetShortMonthDate ( theDate : TDateTime ) : String;
begin
     Result := formatdatetime('dd mmm yyyy',theDate);
end;
{ **************************************************************************** }
function GetShortMonthDateTime ( theDate : TDateTime ) : String;
begin
     Result := formatdatetime('dd mmm yyyy hh:mm:ss',theDate);
end;
{ **************************************************************************** }

function ConvertDateStrtoStr ( s : String ) : String;
begin                              
     Result := copy(s,5,2) + '/' + copy(s,7,2) + '/' + copy(s,1,4) + ' ' + copy(s,10,5) + ':00';
end;
{ **************************************************************************** }
procedure ReadPrinters;
const SQL1 = 'SELECT * FROM PRINTERS where PC_NAME = ''%s''';
      sp = ' ';
var i : shortint;
begin
     CardPrinterIndex := -1;
     ReportPrinterIndex := -1;
     ReceiptPrinterIndex := -1;
     CashDrawerPrinterIndex := -1;
     CommPortsTypes := cpNone;
     CardPrinterName  := '';
     ReceiptPrinterName  := '';
     CashDrawerPrinterName  := '';
     CashDrawerStr := '';
     dm.sqlExecQuery(dm.qryGeneral,format(SQL1,[ReadComputerName]));
     with dm.qryGeneral do begin
        while not EOF do begin
           i := FieldByName('PRINTER_INDEX').AsInteger;
           if FieldByName('DESCRIPTION').AsString = PrinterTypes[0] then
              ReportPrinterIndex := i;
           if FieldByName('DESCRIPTION').AsString = PrinterTypes[1] then begin
              ReceiptPrinterIndex := i;
              ReceiptPrinterName  := FieldByName('PRINTER_NAME').AsString; // keep it for raw printing
           end;   
           if FieldByName('DESCRIPTION').AsString = PrinterTypes[2] then begin
              CardPrinterIndex := i;
              CardPrinterName  := FieldByName('PRINTER_NAME').AsString; // keep it for raw printing
           end;          
          // set up as Cash Drawer
           if FieldByName('DESCRIPTION').AsString = PrinterTypes[3] then begin
              //  thru Comm Kick
              if FieldByName('PRINTER_INDEX').IsNull then begin
                 //comm port
                 case FieldByName('COMM_PORT').AsInteger of
                    0 : CommPortsTypes := cpNone;
                    1 : CommPortsTypes := cpCom1;
                    2 : CommPortsTypes := cpCom2;
                    3 : CommPortsTypes := cpCom3;
                    4 : CommPortsTypes := cpCom4;
                    5 : CommPortsTypes := cpCom5;
                    6 : CommPortsTypes := cpCom6;
                 end;
              // Thru a nominated Receipt Printer and emulation Epson or Star
              end else begin
                 case FieldByName('EMULATION').AsInteger of
                    1 : CashDrawerStr := Esc + 'p055'  ; //for STP 131P
                    2 : CashDrawerStr := chr(28); // Star
                 end;
                 CashDrawerPrinterIndex := i;
                 CashDrawerPrinterName := FieldByName('PRINTER_NAME').AsString; // keep it for raw printing
              end;
           end;
           next;
        end;
     end;
end;
{ **************************************************************************** }
procedure GridLoadPrinters (  cbx : TwwDBComboBox );
var i : shortint;
    s : Shortstring;
const OnLPT = 'on LPT';    
      OnCapsLPT = 'ON LPT';    
      OnEltronLPT = 'on Eltron LPT';    
begin
     try
        cbx.Items.Clear;
        with Printer do begin
           for i := 0 to Printers.Count - 1 do begin
              s := printers[i];
              // on win 98 it adds on on LPT1: - needs to be deleted 
              if POS( OnLPT,s ) > 0 then
                 system.delete(s,POS( OnLPT,s),length(s));
              if POS( OnCapsLPT,s ) > 0 then
                 system.delete(s,POS( OnCapsLPT,s),length(s));
              // trim off the end
              if POS( OnEltronLPT,s) > 0 then
                 system.delete(s,POS( OnEltronLPT,s),length(s));
              // trim off space on END  
              s := trim(s); 
              if s[length(s)] = ' ' then
                 system.delete(s,length(s),1);
              cbx.Items.Add(s);
           end;
           //To select the default printer, set the value of PrinterIndex to -1.
           PrinterIndex := -1;
           cbx.ItemIndex := PrinterIndex;
        end;
     finally
     end;
end;
{ **************************************************************************** }
procedure GetCardsFromLinks( GroupsList,SectionsList,SubsList : TStringList; Member : LongInt );
const SQL1 = 'Select DESCRIPTION,CARD_DESC from SUBSCRIPTIONS s, M_SUBS ms ' +
             ' where ms.MEMBER = %d and s.SUBSCRIPTION = ms.SUBSCRIPTION ' +
             ' and s.PRINT_ON_CARD = ''Y'''; 
      SQL2 = 'Select DESCRIPTION,CARD_DESC from SECTIONS s, M_SECTIONS ms ' +
             ' where ms.MEMBER = %d and s.SECTION = ms.SECTION ' +
             ' and s.PRINT_ON_CARD = ''Y''';        
      SQL3 = 'Select DESCRIPTION,CARD_DESC from GROUPS g, M_GROUPS mg ' +
             ' where mg.MEMBER = %d and g.GROUPS = mg.GROUPS ' +
             ' and g.PRINT_ON_CARD = ''Y''';         
begin
     with dm.qryGeneral1 do begin
        if dm.sqlExecQueryCheckEOF(dm.qryGeneral1,format(SQL1,[Member])) then begin
           while not EOF do begin
              if FieldByName('CARD_DESC').ISNull then 
                 SubsList.Add(Trim(FieldByName('DESCRIPTION').AsString))
              else
                 SubsList.Add(Trim(FieldByName('CARD_DESC').AsString));
              Next;
           end;   
        end;
        if dm.sqlExecQueryCheckEOF(dm.qryGeneral1,format(SQL2,[Member])) then begin
           while not EOF do begin
              if FieldByName('CARD_DESC').ISNull then 
                 SectionsList.Add(Trim(FieldByName('DESCRIPTION').AsString))
              else 
                 SectionsList.Add(Trim(FieldByName('CARD_DESC').AsString));
              Next;
           end;
        end;
        if dm.sqlExecQueryCheckEOF(dm.qryGeneral1,format(SQL3,[Member])) then begin
           while not EOF do begin
              if FieldByName('CARD_DESC').ISNull then 
                 GroupsList.Add(Trim(FieldByName('DESCRIPTION').AsString))
              else 
                 GroupsList.Add(Trim(FieldByName('CARD_DESC').AsString));
              Next;
           end;
        end;
     end;                     
end;
{ **************************************************************************** }
function GetCardType( MEMBER : LongInt) : ShortString;
// NB CARD TYPE is stored in SPARE SMALLINT - 
const  SQL1 = 'Select max(s.SPARE_SMALLINT) as CARD_TYPE from SUBSCRIPTIONS s, m_SUBS ms where ' +
              's.SUBSCRIPTION = ms.SUBSCRIPTION and ms.MEMBER = %d';
begin 
     Result := '';
     if dm.sqlExecQueryCheckEOF(dm.qryGeneral1,format(SQL1,[Member])) then
        Result := dm.qryGeneral1.FieldByNAme('CARD_TYPE').AsString;
end;
{ **************************************************************************** }
procedure ExportCards(Showmess : Boolean);
var CardStr,HeaderStr,s : String;
    F : TextFile;
    strFieldList,GroupsList,SectionsList,SubsList : TStringList;
    i,GroupsCtr,SectionsCtr,SubsCtr : smallint;
    Idx : shortInt;
const
      WhichScreen = 'this can be done in the Members Settings Screen/Cards tab';
      SQL1 = 'Select MEMBER %s from MEMBERS where MEMBER = %s';
      // Others
      SQLOthers = 'Select c.MEMBER,c.NUMBER_CARDS,c.CARD_TYPE from CARD_MEMBERS c,MEMBERS m where ' +
             'm.MEMBER = c.MEMBER order by m.Member';
      // Ashburton
      SQLAshburton = 'Select c.MEMBER,c.NUMBER_CARDS,c.CARD_TYPE from CARD_MEMBERS c,MEMBERS m where ' +
             'm.MEMBER = c.MEMBER order by m.SURNAME,m.FIRST_NAME';
      // sort order for Pauanui  - Put Card Type in from SUBSCRIPTIONS
      SQLPauanuiCard  = 'UPDATE CARD_MEMBERS cm SET CARD_TYPE = ' +
              '( Select max(s.SPARE_SMALLINT) from SUBSCRIPTIONS s, m_SUBS ms where ' +
              's.SUBSCRIPTION = ms.SUBSCRIPTION and ms.MEMBER = cm.MEMBER)';
      // sort order for Pauanui 
      SQLPauanui = 'Select c.MEMBER,c.NUMBER_CARDS,c.CARD_TYPE from CARD_MEMBERS c,MEMBERS m where ' +
             'm.MEMBER = c.MEMBER order by c.CARD_TYPE,m.SPARE_VARCHAR_50';
{
 Cashmere Club   Number,Known As  ,Surname,Joined,Expire Date,Group1,Group2,Group3,Group4
                        FIRST_NAME                            JUNIOR, CASHMERE, SYDENHAM

 Pauanui     CardType,Number,Full Name,Expiry Date,Group1,Group2,Group3,Group4
             ????                     dd mmm yyyy
}

begin
    // filter to the DefaultCardID
    if DefaultCardID = -1 then begin
       sm('You need to create a Card!,' + WhichScreen);
       exit;
    end;
    CardStr := '';HeaderStr := '';CardFieldStr := '';
    strFieldList := TStringList.Create;
    with dm.tblCardDetail do begin
       if not Filtered then begin
          Filter := format('CARD_ID = %d',[DefaultCardID]);
          Filtered := True;
       end;
       // make up CardFieldStr from FIELD_NAME - each record
       //if CardFieldStr = '' then begin
          First;                    
          while not EOF do begin
             if FieldByName('FIELD_NAME').AsString <> '' then begin
                if FieldByName('FIELD_NAME').AsString = 'FIRST_NAME_SURNAME' then begin
                   CardFieldStr := CardFieldStr + ',' + 'FIRST_NAME || '' '' || SURNAME as FIRST_NAME_SURNAME';
                end else if FieldByName('FIELD_NAME').AsString = 'KNOWN_AS_SURNAME' then begin
                   CardFieldStr := CardFieldStr + ',' + 'KNOWN_AS || '' '' || SURNAME as KNOWN_AS_SURNAME';
                end else if FieldByName('FIELD_NAME').AsString = 'UPPER_FULL_NAME' then begin
                   CardFieldStr := CardFieldStr + ',' + 'UPPER(FULL_NAME) as UPPER_FULL_NAME';
                end else if FieldByName('FIELD_NAME').AsString = 'JOINED_YEAR' then begin
                   CardFieldStr := CardFieldStr + ',' + 'EXTRACT (YEAR from JOINED) as JOINED_YEAR';
                end else if FieldByName('FIELD_NAME').AsString <> '' then begin
                   if ATableField( FieldByName('FIELD_NAME').AsString ) then
                      CardFieldStr := CardFieldStr + ',' + FieldByName('FIELD_NAME').AsString;
                end;
                if not FieldByName('CARD_TEXT').IsNull then
                   HeaderStr := HeaderStr + FieldByName('CARD_TEXT').AsString + ',';
                strFieldList.Add(FieldByName('FIELD_NAME').AsString);
             end;   
             Next;
          end;
       //end;   
       try
          CreateFileForExport( F );
          // use CARD_TEXT for the Header String
          if length(HeaderStr) > 0 then begin
             // remove last comma
             system.delete(HeaderStr,length(HeaderStr),1);
             WriteLn(F,HeaderStr);
          end;
          application.processmessages;
          // apply CARD TYPE to CARD_MEMBERS
          if (POS('Pauanui',CompName) > 0) then 
             dm.sqlExecquery(dm.qryGeneral,SQLPauanuiCard);
          // get the member and the fields - best to do a sort on the way out
          application.processmessages;
          with dm.qryReport do begin
             close;
             if (POS('Pauanui',CompName) > 0) then 
                SQL.Text := SQLPauanui
             else if POS ('ASHBURTON', uppercase(CompName)) > 0 then 
                SQL.Text := SQLAshburton
             else
                SQL.Text := SQLOthers;  
             open;
             first;
             GroupsList   := TStringList.Create;
             SectionsList := TStringList.Create;
             SubsList     := TStringList.Create;
             GroupsCtr     := 0;
             SectionsCtr   := 0;
             SubsCtr       := 0;
             while not EOF do begin
                if not dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL1,[CardFieldStr,FieldByName('MEMBER').AsString])) then begin
                   sm(format('Member %s is not valid',[FieldByName('MEMBER').AsString]));
                   next;
                   continue;
                end;   
                s := '';
                GetCardsFromLinks( GroupsList,SectionsList,SubsList,dm.qryGeneral.FieldByName('MEMBER').AsInteger);
                for i := 0 to strFieldList.Count -1 do begin
                    if ATableField( strFieldList[i] ) then begin
                     // Data Fields   - cant use datatype with IBSQL
                      if strFieldList[i] = 'JOINED' then
                         s := s + uppercase(GetShortMonthDate(dm.qryGeneral.FieldByName(strFieldList[i]).AsDAteTime)) + ','
                      else if strFieldList[i] = 'EXPIRES' then begin
                         // fixed expiry and member expires < expires
                         // Cashmere for a new Member - expires is always 1 year less
                         // but they print cards then they pay once they get the cards
                         if FixedExpirySystem then begin
                            if dm.qryGeneral.FieldByName(strFieldList[i]).AsDAteTime < MemberExpiry then
                               s := s + uppercase(GetShortMonthDate(MemberExpiry)) + ','
                            else
                               s := s + uppercase(GetShortMonthDate(dm.qryGeneral.FieldByName(strFieldList[i]).AsDAteTime)) + ',';
                         end else begin   
                            // revolving
                            s := s + uppercase(GetShortMonthDate(dm.qryGeneral.FieldByName(strFieldList[i]).AsDAteTime)) + ','
                         end;   
                      {end else if strFieldList[i] = 'MEMBER' then begin
                         if (Global.CompName = 'Papatoetoe Cosmopolitan Club') or
                            (Global.CompName = 'St Albans Shirley Club') then begin
                            case Length(dm.qryGeneral.FieldByName(strFieldList[i]).AsString) of
                                 1 : s := s + '"' + 'm000' + Trim(dm.qryGeneral.FieldByName(strFieldList[i]).AsString) + '",';
                                 2 : s := s + '"' + 'm00'  + Trim(dm.qryGeneral.FieldByName(strFieldList[i]).AsString) + '",';
                                 3 : s := s + '"' + 'm0'   + Trim(dm.qryGeneral.FieldByName(strFieldList[i]).AsString) + '",';
                            end;
                         end else begin
                         
                         end;}
                      end else 
                         s := s + Trim(dm.qryGeneral.FieldByName(strFieldList[i]).AsString) + ',';
                    end else begin
                       // ( 'SUBSCRIPTIONS','SECTIONS','EVENTS','GROUPS'); 
                       Idx := GetFieldNamesArray(strFieldList[i]);
                       case Idx of
                         0 : begin
                                if SubsCtr <= SubsList.Count -1  then begin
                                   s := s + Trim(SubsList[SubsCtr]) + ',';
                                   inc(SubsCtr);
                                end else begin   
                                   s := s + ','
                                end;
                             end;  
                         1 : begin
                                if SectionsCtr <= SectionsList.Count -1  then begin
                                   s := s + Trim(SectionsList[SectionsCtr]) + ',';
                                   inc(SectionsCtr);
                                end else begin   
                                   s := s + ','
                                end;   
                             end;       
                         3 : begin
                                if GroupsCtr <= GroupsList.Count -1  then begin
                                   s := s + Trim(GroupsList[GroupsCtr]) + ',';
                                   inc(GroupsCtr);
                                end else begin   
                                   s := s + ','
                                end;   
                             end;   
                         4 : begin
                            // 'CARD TYPE' put into CARD_MEMBERS see SQL3
                                if strFieldList[i] = 'CARD TYPE' then
                                   s := s + FieldByName('CARD_TYPE').AsString + ',';
                                 //s := s + GetCardType( dm.qryGeneral.FieldByName('MEMBER').AsInteger) + ',';
                             end;   
                       end;      
                    end;
                end;
                system.delete(S,length(S),1);
                // one card
                if FieldByName('NUMBER_CARDS').IsNull then begin
                   WriteLn(F,s);
                end else begin   
                   if FieldByName('NUMBER_CARDS').AsInteger > 1 then begin
                      for i := 1 to FieldByName('NUMBER_CARDS').AsInteger do
                         WriteLn(F,s);
                   end else begin
                      WriteLn(F,s);
                   end;
                end;
                next;
             end;
          end;
          if Showmess then
             MdlgI( CreatedFileMsg + LastFileExported);
       finally
         if assigned(GroupsList)   then  GroupsList.Free;
         if assigned(SectionsList) then  SectionsList.Free;
         if assigned(SubsList)     then  SubsList.Free;
         CloseFile( F );
         if assigned(strFieldList) then strFieldList.Free;
       end;
    end;
end;
{ **************************************************************************** }
procedure CardEncode( Member : ShortString ; Expires : TDateTime);
var CardStr : String;
    ExpiryDateDays : LongInt;
begin
     if CardEncodeMemberOnly = 'X' then begin
        sm('No Encoding Selected');
        exit;
     end;
     if not CardPrinterAvailable then exit;
     // make up the CardStr
     CardStr := '';
     if CardEncodeMemberOnly = varYes then begin
        if CompName = 'Blenheim Working Mens Club' then begin
           case Length(Member) of
              1 : Member := '000' + Member;
              2 : Member := '00' + Member;
              3 : Member := '0' + Member;
            end;
            Member := '5' + Member + '30064';
            CardStr := CardStr + Format( '%s&E1 %s%s',[Esc,Member,Cr]); // Encode Tr1
            CardStr := CardStr + Format( '%s&E2 %s%s',[Esc,Member,Cr]); // Encode Tr2
       end else if POS('Hornby',CompName) > 0 then begin
           if Length(Member) = 1 then
              Member := '0' + Member;
           CardStr := CardStr + Format( '%s&E2 %s%s',[Esc,Member,Cr]);   // Encode
        end else begin
           CardStr := Format( '%s&E2 %s%s',[Esc,Member,Cr]);   // Encode
        end;
     end else begin   // varNo
        ExpiryDateDays := trunc(Expires);
        CardStr := Format( '%s&E2 %s%s',[Esc,Member + ':' + IntToStr(ExpiryDateDays - ArevDateDiff),Cr]);   // Encode
     end;
     // prepare to print
     if not RawDataToPrinter('Card Encode',CardPrinterName,CardStr,False) then
        sm(format('Card Encoding for Member %s has failed',[Member]));
// tried                   MIB         ME , MO , MC  ,MB
     //CardStr := Format( '%s# 1 !M%s',[ESC,Cr]);  
     //if not RawDataToPrinter('Return Card',CardPrinterName,CardStr,False) then
     //   sm(format('Card Encoding for Member %s has failed',[Member]));

     {CardStr := Format( '%sME',[ESC]);  
     if not RawDataToPrinter('Exit Card to Hopper',CardPrinterName,CardStr,False) then
        sm(format('Card Encoding for Member %s has failed',[Member]));

     CardStr := Format( '%sMO',[ESC]);  
     if not RawDataToPrinter('Exit Card to Hopper',CardPrinterName,CardStr,False) then
        sm(format('Card Encoding for Member %s has failed',[Member]));
     }   
end;
{ **************************************************************************** }
procedure CardReadTrack2;
var CardStr : String;
begin
     if not CardPrinterAvailable then exit;
     CardStr := '';
     CardStr := CardStr + Format( '%sMI%s',[Esc,Cr]);   // insert card to position
     //if not RawDataToPrinter('Read Encoding',CardPrinterName,pc,dw,True) then
     if not ReadFromPrinter('Read Encoding',CardPrinterName,CardStr,False) then
        sm( 'Card Read Encoding has failed' );
     CardStr := '';
     CardStr := CardStr + Format( '%s&L2%s',[Esc,Cr]);   // Read Encoding track 2
     //CardStr := CardStr + Format( '%s\M\1\MI[&L2[ME%s',[Esc,Cr]);   // Read Encoding track 2
     //CardStr := CardStr + Format( '%s&L12%s',[Esc,Cr]);   // Read Encoding track 2
     if not ReadFromPrinter('Read Encoding',CardPrinterName,CardStr,True) then
        sm( 'Card Read Encoding has failed' );
     CardEject;
end;
{ **************************************************************************** }
procedure ResetEncoder;
var CardStr : String;
begin
     if not CardPrinterAvailable then exit;
     CardStr := '';
     CardStr := CardStr + Format( '%s&R%s',[Esc,Cr]);   // Read Encoding track 2
     if not RawDataToPrinter('Reset Encoder',CardPrinterName,CardStr,True) then
        sm( 'Card Reset Encoder has failed' );
end;
{ **************************************************************************** }
procedure CardEject;
var CardStr : String;
begin
     if not CardPrinterAvailable then exit;
     CardStr := Format( '%s&T%s',[Esc,Cr]);   // Eject Card
     if not RawDataToPrinter('Eject Card',CardPrinterName,CardStr,False) then
        sm('Card Eject has failed');
end;
{ **************************************************************************** }
function CardPrinterAvailable : Boolean;
begin
     Result := True;
     if (Global.CardPrinterIndex = -1) or (Global.CardPrinterName = '') then begin
        Result := False;
        sm(NoCardPrinterMsg);
    end;
end;

{ **************************************************************************** }
procedure CardPrint( Member,Expires : ShortString ; NumberCards : ShortInt ; theTransaction : TIBTransaction);
const  SQL1 = 'Select MEMBER,EXPIRES %s from MEMBERS where MEMBER = %s';
       SQL2 = 'Select DESCRIPTION,CARD_DESC,EXPIRES_CARD,EXPIRES_DESC from SUBSCRIPTIONS s, M_SUBS ms ' +
             ' where ms.MEMBER = %d and s.SUBSCRIPTION = ms.SUBSCRIPTION ';
      //SQL2 = 'Select DESCRIPTION from SUBSCRIPTIONS s, M_SUBS ms ' +
      //       ' where ms.MEMBER = %s and s.SUBSCRIPTION = ms.SUBSCRIPTION';
      WhichScreen = 'this can be done in the Members Settings Screen/Cards tab';
      sp = ' ';
var CardStr,TempStr,FMember : String;
    strFieldList,GroupsList,SectionsList,SubsList : TStringList;
    GroupsCtr,SectionsCtr,SubsCtr,MemberLength  : smallint;
    Idx : shortInt;
    AddCardStr,RPNYC : Boolean;
    ExpiryDateDays : LongInt;
    CardText,ExpiresDesc : String[50];
    CardTextPos,ExpiresNotOnCard : String[1];
    CardHzt,CardVrt,CardRotation,CardSize,CardReduce,CardLength : SmallInt;
    i,CardStyle,CardsProp,CardGraphic,j: ShortInt;
{               Start Pos               0   1                   0,1 or 2
               Hzt   Vrt   Rotation   Nml/Bold   prop   size   Graphic Mode
     Text  T  150    300   0          1           0     60     1
 }
begin
     if not CardPrinterAvailable then exit;
    // filter to the DefaultCardID
    if DefaultCardID = -1 then begin
       sm('You need to create a Card!,' + WhichScreen);
       exit;
    end;
    try
       with dm.tblCardDetail do begin
          if not Filtered then begin
             Filter := format('CARD_ID = %d',[DefaultCardID]);
             Filtered := True;
          end;
          // make up CardFieldStr from FIELD_NAME - each record
          if CardFieldStr = '' then begin
             First;
             while not EOF do begin
                if FieldByName('FIELD_NAME').AsString = 'FIRST_NAME_SURNAME' then begin
                   CardFieldStr := CardFieldStr + ',' + 'FIRST_NAME || '' '' || SURNAME as FIRST_NAME_SURNAME';
                end else if FieldByName('FIELD_NAME').AsString = 'KNOWN_AS_SURNAME' then begin
                   CardFieldStr := CardFieldStr + ',' + 'KNOWN_AS || '' '' || SURNAME as KNOWN_AS_SURNAME';
                end else if FieldByName('FIELD_NAME').AsString = 'UPPER_FULL_NAME' then begin
                   CardFieldStr := CardFieldStr + ',' + 'UPPER(FULL_NAME) as UPPER_FULL_NAME';
                end else if FieldByName('FIELD_NAME').AsString = 'EXPIRES_YEAR' then begin
                   CardFieldStr := CardFieldStr + ',' + 'EXTRACT (YEAR from EXPIRES) as EXPIRES_YEAR';
                end else if FieldByName('FIELD_NAME').AsString <> '' then begin
                   if ATableField( FieldByName('FIELD_NAME').AsString ) then
                      CardFieldStr := CardFieldStr + ',' + FieldByName('FIELD_NAME').AsString;
                      
                end;
                Next;
             end;
             //if CardFieldStr = '') and (TextStr = '' )then begin
             //   sm('There are no Fields on this Card!, ' + WhichScreen );
             //   exit;
             //end;
          end;
          GroupsList   := TStringList.Create;
          SectionsList := TStringList.Create;
          SubsList     := TStringList.Create;
          // assign to the transaction because sometimes the dataset is uncommitted
         // dm.qryGeneral.Transaction := theTransaction;
          // get the member and the fields
          if not dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL1,[CardFieldStr,Member])) then begin
             sm(NotAValidMemMsg);
             exit;
          end;
          GroupsCtr     := 0;
          SectionsCtr   := 0;
          SubsCtr       := 0;
          GetCardsFromLinks( GroupsList,SectionsList,SubsList,dm.qryGeneral.FieldByName('MEMBER').AsInteger);
          // make up the CardStr
          CardStr := '';
          // Clear Monochrome Image Buffers
          CardStr := Format( '%sF%s',[Esc,Cr]);             
          // CardEncodeMemberOnly is now Y,N or X for no encoding
          if CardEncodeMemberOnly = varYes then begin
             { ****************************************************************************}
             // Blenheim Club encode both track 1 and track 2
             if CompName = 'Blenheim Working Mens Club' then begin
                case Length(Member) of
                  1 : FMember := '000' + Member;
                  2 : FMember := '00' + Member;
                  3 : FMember := '0' + Member;
                else
                   FMember := Member;
                end;
                FMember := '5' + FMember + '30064';
                CardStr := CardStr + Format( '%s&E1 %s%s',[Esc,Fmember,Cr]); // Encode Tr1
                CardStr := CardStr + Format( '%s&E2 %s%s',[Esc,Fmember,Cr]); // Encode Tr2
             { ****************************************************************************}
             // Oamaru cards 
             end else if pos('Oamaru',CompName) > 0 then begin
                case Length(Member) of
                  1 : FMember := '000' + Member;
                  2 : FMember := '00' + Member;
                  3 : FMember := '0' + Member;
                else
                   FMember := Member;
                end;
                FMember := '1009100000' + FMember;
                CardStr := CardStr + Format( '%s&E2 %s%s',[Esc,Fmember,Cr]); // Encode Tr2
             { ****************************************************************************}
             // Timaru Cards must begin with zeros 
             end else if POS('Timaru Town',CompName) > 0 then begin
                MemberLength := length(Member);
                SetLength(FMember,11);
                for j := 1 to (11 - MemberLength) do begin
                   FMember[j] := '0';
                end;
                SetLength(FMember,(11 - MemberLength));
                FMember := FMember + Member;
                CardStr := CardStr + Format( '%s&E2 %s%s',[Esc,Fmember,Cr]);   // Encode
             { ****************************************************************************}
             // Hornby Cards which are 1 to 9 change to 01 to 09 for Door Control
             end else if POS('Hornby',CompName) > 0 then begin
                if Length(Member) = 1 then
                   Member := '0' + Member;
                CardStr := CardStr + Format( '%s&E2 %s%s',[Esc,Member,Cr]);   // Encode
             { ****************************************************************************}
             // Standard format for everyone else
             end else begin               
                CardStr := CardStr + Format( '%s&E2 %s%s',[Esc,Member,Cr]);   // Encode
             end; 
          end else if CardEncodeMemberOnly = varNo then begin   // RPNYC  Expires dates on Card - not used now
             ExpiryDateDays := trunc(dm.qryGeneral.FieldByName('EXPIRES').AsDateTime);
             CardStr := CardStr + Format( '%s&E2 %s%s',[Esc,Member + ':' + IntToStr(ExpiryDateDays - ArevDateDiff),Cr]);   // Encode
          end;
          First;
          { ****************************************************************************}
          // Make up the Card Structure from the Field names
          while not EOF do begin
             AddCardStr := True;
             CardHzt      := FieldByName('HORIZONTAL').AsInteger;
             CardVrt      := FieldByName('VERTICAL').AsInteger;
             CardRotation := FieldByName('ROTATION').AsInteger;
             CardStyle    := FieldByName('STYLE').AsInteger;
             CardsProp    := FieldByName('PROPORTION').AsInteger;
             CardSize     := FieldByName('SIZE').AsInteger;
             CardReduce   := FieldByName('SIZE_REDUCE').AsInteger;
             CardGraphic  := FieldByName('GRAPHIC_MODE').AsInteger;
             CardText     := FieldByName('CARD_TEXT').AsString;
             CardTextPos  := FieldByName('TEXT_POSITION').AsString;
             CardLength   := FieldByName('TEXT_LENGTH').AsInteger;  // length before font reduction
             { ****************************************************************************}
             // format the date  JOINED
             if FieldByName('FIELD_NAME').AsString = 'JOINED' then begin
                if CardText <> '' then begin
                   // After
                   if CardTextPos = 'A' then begin
                      TempStr :=  GetShortMonthDate(dm.qryGeneral.FieldByName(FieldByName('FIELD_NAME').AsString).AsDateTime) + sp + CardText;
                   // Before or Null
                   end else begin
                      TempStr := CardText + sp + GetShortMonthDate(dm.qryGeneral.FieldByName(FieldByName('FIELD_NAME').AsString).AsDateTime);
                   end;
                // No CardText
                end else begin
                  TempStr :=  GetShortMonthDate(dm.qryGeneral.FieldByName(FieldByName('FIELD_NAME').AsString).AsDateTime);
                end;
             { ****************************************************************************}
             // format the date  EXPIRES Date
             end else if FieldByName('FIELD_NAME').AsString = 'EXPIRES' then begin
                // Get Expires from the DataBase ie it is not passed
                if Expires = '' then begin
                   ExpiresNotOnCard := '';
                   ExpiresDesc := '';
                   // get subs for member
                   dm.sqlExecQueryCheckEOF(dm.qryGeneral1,format(SQL2,[dm.qryGeneral.FieldByName('MEMBER').AsInteger]));
                   //on checks the first sub - most have only 1 sub
                   if not dm.qryGeneral1.EOF then begin
                      ExpiresNotOnCard := dm.qryGeneral1.FieldByName('EXPIRES_CARD').AsString;
                      ExpiresDesc      := dm.qryGeneral1.FieldByName('EXPIRES_DESC').AsString;
                   end;
                   // Dont put Expires Date on the card - Life , Honary etc
                   if ExpiresNotOnCard = 'Y' then begin 
                      TempStr :=  ' ';
                      AddCardStr := False;
                      // may choose to put 'Life Member' on Card instead of Expires
                      if ExpiresDesc <> '' then begin
                         TempStr := ExpiresDesc;
                         AddCardStr := True;
                      end;
                   // no Expires Date - dont print
                   end else if (dm.qryGeneral.FieldByName('EXPIRES').IsNull) or
                      (dm.qryGeneral.FieldByName('EXPIRES').AsDateTime <= StrToDate(StrToDate1899)) then begin
                      TempStr :=  ' ';
                      AddCardStr := False;
                   end else if CardText <> '' then begin
                      // After
                      if CardTextPos = 'A' then begin
                         TempStr :=  GetShortMonthDate(dm.qryGeneral.FieldByName(FieldByName('FIELD_NAME').AsString).AsDateTime) + sp + CardText;
                      // Before or Null
                      end else begin
                         TempStr := CardText + sp + GetShortMonthDate(dm.qryGeneral.FieldByName(FieldByName('FIELD_NAME').AsString).AsDateTime);
                      end;
                   // No CardText
                   end else begin
                      TempStr :=  GetShortMonthDate(dm.qryGeneral.FieldByName(FieldByName('FIELD_NAME').AsString).AsDateTime);
                   end;           
                // Expires has been passed from a worked out Date ( Direct Debits )
                end else begin
                   if CardText <> '' then begin
                      // After
                      if CardTextPos = 'A' then begin
                         TempStr :=  Expires + sp + CardText;
                      // Before or Null
                      end else begin
                         TempStr := CardText + sp + Expires;
                      end;
                   // No CardText
                   end else begin
                      TempStr :=  Expires;
                   end;           
                end;   
             { ****************************************************************************}
             // Subscriptions
             end else if FieldByName('FIELD_NAME').AsString = 'SUBSCRIPTIONS' then begin
                if SubsCtr <= SubsList.Count -1 then begin
                   if CardText <> '' then begin
                      if CardTextPos = 'A' then begin
                         TempStr :=  SubsList[SubsCtr] + sp + CardText;
                      end else begin
                         TempStr := CardText + sp + SubsList[SubsCtr];
                      end;
                   end else begin
                      TempStr := SubsList[SubsCtr];
                   end;
                   inc(SubsCtr);
                end else begin
                  // no subs left
                  next;
                  continue;
                end;
             { ****************************************************************************}
             // Sections
             end else if FieldByName('FIELD_NAME').AsString = 'SECTIONS' then begin
                if SectionsCtr <= SectionsList.Count -1 then begin
                   if CardText <> '' then begin
                      if CardTextPos = 'A' then begin
                         TempStr :=  SectionsList[SectionsCtr] + sp + CardText;
                      end else begin
                         TempStr := CardText + sp + SectionsList[SectionsCtr];
                      end;
                   end else begin
                      TempStr := SectionsList[SectionsCtr];
                   end;
                   inc(SectionsCtr);
                end else begin
                  // no sections left
                  next;
                  continue;
                end;
             { ****************************************************************************}
             // Groups
             end else if FieldByName('FIELD_NAME').AsString = 'GROUPS' then begin
                if GroupsCtr <= GroupsList.Count -1 then begin
                   if CardText <> '' then begin
                      if CardTextPos = 'A' then begin
                         TempStr :=  GroupsList[GroupsCtr] + sp + CardText;
                      end else begin
                         TempStr := CardText + sp + GroupsList[GroupsCtr];
                      end;
                   end else begin
                      TempStr := GroupsList[GroupsCtr];
                   end;
                   inc(GroupsCtr);
                end else begin
                  // no Groups left
                  next;
                  continue;
                end;
             { ****************************************************************************}
             // No Field Name
             end else if FieldByName('FIELD_NAME').AsString = '' then begin
                TempStr := CardText;
             { ****************************************************************************}
             // Standard Field Names
             end else begin
                if not dm.qryGeneral.FieldByName(FieldByName('FIELD_NAME').AsString).IsNull then begin
                   if CardText <> '' then begin
                      if CardTextPos = 'A' then begin
                         TempStr :=  dm.qryGeneral.FieldByName(FieldByName('FIELD_NAME').AsString).AsString + sp + CardText;
                      end else begin
                         TempStr := CardText + sp + dm.qryGeneral.FieldByName(FieldByName('FIELD_NAME').AsString).AsString;
                      end;
                   end else begin
                      TempStr := dm.qryGeneral.FieldByName(FieldByName('FIELD_NAME').AsString).AsString;
                   end;
                // dont write out this record if the field name is null
                end else begin
                   next;
                   continue;
                end;
             end;
             { ****************************************************************************}
             // reduce the size of the font by CardSize
             if CardLength > 0 then begin
                if (length(TempStr) >= CardLength) and (CardReduce > 0) then
                   CardSize := CardSize - CardReduce;
                                                                                 //Hzt    Vrt       Rotation   Nml/Bold   prop     size    Graphic Mode
             end;
             if AddCardStr then 
                CardStr := CardStr + Format( '%sT %d %d %d %d %d %d %d %s%s',[ESC,CardHzt,CardVrt,CardRotation,CardStyle,CardsProp,CardSize,CardGraphic,TempStr,Cr]);  // Expires
             next;
          end;
          CardStr := CardStr + Format( '%sI%s',[Esc,Cr]);
          // prepare to print
          
          for i := 1 to NumberCards do begin
             if not RawDataToPrinter('Card Print',CardPrinterName,CardStr,False) then
                sm(format('Card Printing for Member %s has failed',[Member]));
          end;
       end;
    finally
       //dm.QryGeneral.Transaction := dm.IBTransaction;
       if assigned(GroupsList)   then  GroupsList.Free;
       if assigned(SectionsList) then  SectionsList.Free;
       if assigned(SubsList)     then  SubsList.Free;
    end;
end;
{ **************************************************************************** }
function RawDataToPrinter(szDocName,szPrinterName,DataStr : String;ReadPrinterData : Boolean) : Boolean;
var
  hPrinter : THandle;
  DocInfo : TDocInfo1;
  dwJob,paCount : DWord;
  dwBytesWritten,pNoBytesRead : DWord;
  dwCount : DWord;
  lpData  : PChar;

 // s : string;
  //CardDate : TDateTime;
 // pbuf : array[1..100] of Char;
 // pc : Pchar;
 // i : smallint;
  //longboolint : LongBool;

begin
    // Need a handle to the printer
    lpData := PChar(DataStr);
    dwCount := Length(lpData);
    //dwCount := Sizeof(DataStr);
    try
      result := False;
      if not OpenPrinter(PChar(szPrinterName), hPrinter, nil) then begin
        sm('Cannot open Printer ' + szPrinterName); 
        Result := False;
        Exit;
      end;
      // Fill in the structure with info about this "document"
      DocInfo.pDocName := PChar(szDocName);
      DocInfo.pOutputFile := nil;
      DocInfo.pDatatype := 'RAW';
      // Inform the spooler the document is beginning
      dwJob := StartDocPrinter(hPrinter, 1, @DocInfo);
      if dwJob = 0 then begin
        ClosePrinter(hPrinter);
        Result := False;
        Exit;
      end;
      // Start a page
      if not StartPagePrinter(hPrinter) then begin
        EndDocPrinter(hPrinter);
        ClosePrinter(hPrinter);
        Result := False;
        Exit;
      end;
      // Send the data to the printer
      if not WritePrinter(hPrinter, lpData, dwCount, dwBytesWritten) then begin
        EndPagePrinter(hPrinter);
        EndDocPrinter(hPrinter);
        ClosePrinter(hPrinter);
        Result := False;
        Exit;
      end;
      // End the page
      if not EndPagePrinter(hPrinter) then begin
        EndDocPrinter(hPrinter);
        ClosePrinter(hPrinter);
        Result := False;
        Exit;
      end;
      // Inform the spooler that the document is ending
      if not EndDocPrinter(hPrinter) then  begin
        ClosePrinter( hPrinter );
        Result := False;
        Exit;
      end;
      // Check to see if correct number of bytes writen
      if dwBytesWritten <> dwCount Then
        result := False
      else
        Result := True;
    finally
      ClosePrinter( hPrinter );
    end;
end;
{ **************************************************************************** }
function ReadFromPrinter(szDocName,szPrinterName,DataStr : String; ReadPrinterData : Boolean) : Boolean;
{var
  hPrinter : THandle;
  DocInfo : TDocInfo1;
  dwJob,paCount : DWord;
  dwBytesWritten,pNoBytesRead : DWord;
  lpData : PChar;
  s : string;
  //CardDate : TDateTime;
  pbuf : array[1..100] of Char;
  pc : Pchar;
  i : smallint;
  longboolint : Integer;}

begin
   { // Need a handle to the printer
    lpData := PChar(DataStr);
    dwCount := Sizeof(DataStr);
    try
      result := False;
      if not OpenPrinter(PChar(szPrinterName), hPrinter, nil) then begin
        Result := False;
        Exit;
      end;
      // Fill in the structure with info about this "document"
      DocInfo.pDocName := PChar(szDocName);
      DocInfo.pOutputFile := nil;
      DocInfo.pDatatype := 'RAW';
      // Inform the spooler the document is beginning
      dwJob := StartDocPrinter(hPrinter, 1, @DocInfo);
      if dwJob = 0 then begin
        ClosePrinter(hPrinter);
        Result := False;
        Exit;
      end;
         //FillChar(pbuf, SizeOf(pbuf), Ord(' '));
         //pNoBytesRead := 0;
         //paCount := sizeof(pbuf);
         //pc := pChar(s);
         //s := pc;


      // Start a page
      if not StartPagePrinter(hPrinter) then begin
        EndDocPrinter(hPrinter);
        ClosePrinter(hPrinter);
        Result := False;
        Exit;
      end;
      s := '';
      if ReadPrinterData then begin
         if ReadPrinter(hPrinter,@pbuf,sizeof(pbuf),pNoBytesRead) then begin
            s := pbuf;
         end;
      end else begin
         // Send the data to the printer
         if not WritePrinter(hPrinter, lpData, dwCount, dwBytesWritten) then begin
           EndPagePrinter(hPrinter);
           EndDocPrinter(hPrinter);
           ClosePrinter(hPrinter);
           Result := False;
           Exit;
         end;
      end;
      // End the page
      if not EndPagePrinter(hPrinter) then begin
        EndDocPrinter(hPrinter);
        ClosePrinter(hPrinter);
        Result := False;
        Exit;
      end;
      // Inform the spooler that the document is ending
      if not EndDocPrinter(hPrinter) then  begin
        ClosePrinter( hPrinter );
        Result := False;
        Exit;
      end;
      // Check to see if correct number of bytes writen
      //if dwBytesWritten <> dwCount Then
      //  result := False
      //else
      //  Result := True;
       Result := True;

    finally
      // Tidy up the printer handle
      ClosePrinter( hPrinter );
      // if reading track 2
      if ReadPrinterData then begin
          if s <> '' then begin
             //Member := copy(s,1,pos(':',s) -1);
             //CardDate := StrToInt64(copy(s,pos(':',s) +1 , length(s)));
             // add on days that were taken off for Andrews System
             //CardDate := CardDate + ArevDateDiff;
             //mdlgI(format('Member %s Expires %s',[Member,GetShortMonthDate(CardDAte)] ));
             mdlgI(format('Card is encoded with %s',[s]));
          end;
       end;
    end;}
end;
{ **************************************************************************** }
procedure OpenCashDrawer;
begin
     // 3 states Not Setup, Comm Port or Printer
     if CashDrawerPrinterName <> '' then begin
        OpenCashDrawerPrinter;
        // seems to need a call to Reset the Printer after the open command
        ResetPrinter;
        exit;
     end;
     if CommPortsTypes <> cpNone then begin
        OpenCashDrawerComPort;
        exit;
     end;
     //sm('No Cash Drawer has been setup');
end;
{ **************************************************************************** }
procedure OpenCashDrawerPrinter;
begin
     // Cash Drawer String is set up in Read Printers
     if not RawDataToPrinter('Open Cash Drawer',CashDrawerPrinterName,CashDrawerStr,False) then
        sm( 'Cash Drawer open has failed' );
end;
{ **************************************************************************** }
procedure OpenCashDrawerComPort;
var s : pchar;
    hCommFile : THandle;
    res : cardinal;
    Len : Integer;
    CommPort : String;
 begin
      try
         CommPort := 'COM' + IntToStr(Ord(CommPortsTypes));
        {Open the comm port}
         hCommFile := CreateFile(Pchar(commport),GENERIC_WRITE,0,nil,
                                 OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
         if hCommFile = INVALID_HANDLE_VALUE then begin
           //ShowMessage('Unable to open  the Cash Drawer on Port ' + CommPort);
           exit;
         end;
         s := 'ffffff';
         Len := Length(s);
         WriteFile(hCommFile, S, Len, Res, nil);
      finally
         CloseHandle(hCommFile);
      end;
end;
{ **************************************************************************** }
procedure ResetPrinter;
var s : string;
begin
     s := Esc + chr(64);
     if not RawDataToPrinter('Reset Printer',CashDrawerPrinterName,s,False) then
        sm( 'Reset Printer has failed' );

end;
{ **************************************************************************** }
procedure QryCloseIfActive( theQry : TIBQuery );
begin
     if theQry.Active then theQry.Active := False;
end;
{ **************************************************************************** }
procedure TblCloseIfActive( theTbl : TIBTable );
begin
     if theTbl.Active then theTbl.Active := False;
end;
{ **************************************************************************** }
procedure TblCloseIfActiveFiltered( theTbl : TIBTable );
begin
     if theTbl.Active then begin
        theTbl.Filtered := False;
        theTbl.Active   := False;
     end;
end;
{ **************************************************************************** }
procedure TblActiveOrRefresh ( theTbl : TIBTable );
begin
     if theTbl.Active then 
        theTbl.Refresh
     else 
        theTbl.Active := True;
end;
{ **************************************************************************** }
procedure QryActiveOrRefresh ( theQry : TIBQuery );
begin
     if theQry.Active then begin
        theQry.Active := False;
        theQry.Active := True;
     end else 
        theQry.Active := True;
end;
{ **************************************************************************** }
procedure IBDSActiveOrRefresh ( theSQL : TIBDataSet );
begin
     if theSQL.Active then begin
        theSQL.Active := False;
        theSQL.Active := True;
     end else 
        theSQL.Active := True;
end;
{ **************************************************************************** }

procedure TblReActivate( theTbl : TIBTable );
begin
     with theTbl do begin
        Active := False;
        Active := True;
    end;
end;
{ **************************************************************************** }
procedure QryReActivate( theQry : TIBQuery );
begin
     with theQry do begin
        Active := False;
        Active := True;
    end;
end;
{ **************************************************************************** }
procedure TblSetFiltered ( theTbl : TIBTable ; filterStr : String );
begin
     with theTbl do begin
        Filtered := False;
        Filter   := filterStr;
        Filtered := True;
     end;   
end;
{ **************************************************************************** }
function CopyFiles( FromStr, ToStr : String ; bFailifExists : Boolean): Boolean;
var FromFile,ToFile : PChar;
// bFailifExists False = Overwrite file if it exists
//               True = if the file exists then fail
begin
     Result := False;
     FromFile := Pchar(FromStr);
     ToFile   := Pchar(ToStr);
     if not copyfile(FromFile,ToFile,bFailifExists) then begin
       // showmessage(Format('Cannot copy %s to %s',[FromStr,ToStr]));
        exit;                            
     end;
     Result := True;
end;         
{ ****************************************************************************}    
{ **************************************************************************** }
procedure GetFiles(WhereFrom : String ; FileList : TStringList );
var   SearchRec : TSearchRec;
begin
      If ( FindFirst( WhereFrom , faAnyFile, SearchRec ) = 0 ) then begin
         Repeat
            FileList.Add( SearchRec.Name );
         Until ( FindNext( SearchRec ) <> 0 );
      end;
end;
{ **************************************************************************** }
function CreateFileAndClose( theFile : String ): Boolean ;
var F : TextFile;
begin
     Result := False;
     AssignFile( F, theFile );
     Rewrite( F );
     CloseFile( F );
     Result := True;
end;
{ **************************************************************************** }
function CreateFileLeaveOpen( var F : TextFile; theFile : String ): Boolean ;
begin
     Result := False;
     try
        AssignFile( F, theFile );
        Rewrite( F );
        Result := True;

     except
        DeleteFile(PChar(theFile));
     end;
end;
{ **************************************************************************** }
function CreateFileForExport( var F : TextFile ): Boolean ;
var CurrDirAndFile : ShortString;
begin
     Result := False;
     try
        GetDir(0,CurrDirAndFile);  // get for email - needs path ext
        dm.SaveDialog.InitialDir := CurrDirAndFile;
        if dm.SaveDialog.Execute then begin
           //GetDir(0,CurrDirAndFile);  // get for email - needs path ext
           AssignFile( F, dm.SaveDialog.FileName );
           Rewrite( F );
           LastFileExported := dm.SaveDialog.FileName;
           Result := True;
        end;   
     except
        sm(format('Cannot create %s - %s %s',[dm.SaveDialog.FileName,HelpDeskMsg,HelpDesk]));
        DeleteFile(PChar(dm.SaveDialog.FileName));
     end;
end;
{ **************************************************************************** }
function CheckCreateFile( var F : TextFile; theFile,CopyToFile : String ): Boolean ;
begin
     Result := False;
     //try
     if FileExists(theFile) then begin
        // make a backup of the file before writing to it , overwrite if there is already one 
        if CopyToFile <> '' then begin
           copyfiles(theFile,CopyToFile,false);
        end;
        // open for appending
        AssignFile( F, theFile );
        Append( F );
        //inc(dm.WinMemberFileCtr);
     end else begin
        // file does not exist
        AssignFile( F, theFile );
        Rewrite( F );
       // dm.WinMemberFileCtr := 0;
     end;
     Result := True;
     //except
     //   Closefile(f);
     //   sm(format('Cannot create %s - %s %s',[theFile,HelpDeskMsg,HelpDesk]));
     //end;
end;
{ **************************************************************************** }
function ConvertToNumber( s : shortstring  ): shortstring ;
begin
     if s = 'Y' then begin
        Result := '1';
        exit;
     end;   
     if (s = 'N') or (s = '') then Result := '0';
end;
{ **************************************************************************** }
{procedure CheckToolbar;
const ToolbarBelowReport = 55;
begin
     {ToolbarSaveTop := 0;
     ToolbarMoved   := False;
     if frmMemberMenu.TOOLBAR.Floating then begin
        if (frmMemberMenu.TOOLBAR.Top < 20) and (frmMemberMenu.TOOLBAR.Left < 320) then begin
            ToolbarMoved  := True;
            ToolbarSaveTop := frmMemberMenu.TOOLBAR.Top;
            MoveToolbar(ToolbarBelowReport,ToolbarBelowReport + ToolbarHeight);
        end;
     end;}
//end;
{ **************************************************************************** }
{procedure MoveToolbar( TTop,THeight : smallint );
var Rect : TRect;
begin
     frmMemberMenu.Toolbar.AutoSize := True;
     Rect.Left   := 1;
     Rect.Top    := TTop;               
     Rect.Right  := frmMemberMenu.Toolbar.Width + 3;
//     Rect.Right  := 270; 
     Rect.Bottom := THeight;
     frmMemberMenu.Toolbar.ManualFloat(Rect);
end;
{ **************************************************************************** }
procedure SetupSaveDialog( SetupExt,SetupFilter,SetupTitle,SetupFileName,Dir : ShortString );
begin
     with dm.SaveDialog do begin
        DefaultExt := SetupExt;
        FileName   := SetupFileName;
        Filter     := SetupFilter;
        SetupTitle := SetupTitle;
        if Dir = '' then
           InitialDir := GetCurrentDir
        else   
           InitialDir := Dir;
     end;
end;
{ **************************************************************************** }
procedure SetupOpenDialog( SetupExt,SetupFilter,SetupTitle : ShortString );
begin
     with dm.OpenDialog do begin
        DefaultExt := SetupExt;
        Filter     := SetupFilter;
        SetupTitle := SetupTitle;
        InitialDir := GetCurrentDir;
     end;
end;
{ **************************************************************************** }
function CalcPosition( NumComp, ThisComp, FormClientWidth, CompWidth : Integer ) : Integer;
// Used to Position visual components on Form ( designed for buttons )
// NumComp = Number of Components to place in row
// ThisComp = Position No of this component
var
   W : Integer;
begin
   W := ( FormClientWidth - ( NumComp * CompWidth ) ) div ( NumComp + 1 );
   Result := ( W * ThisComp ) + ( CompWidth * ( ThisComp - 1 ) ); 
end;
{ **************************************************************************** }
procedure ResizeToolBar( theForm : TSMStdForm ; theWindowState : TWindowState ;
           theButtonWidth : SmallInt ; pnlWidth : SmallInt; thePanel : TPanel );
// called from Most forms - the state theWindowState is the State prior to resizing it
var SmallScreenDiff : smallint;
{
Toolbar must be AutoSize = True and Align = alClient
for the resizing to work correctly 
}
begin
     // 640 * 480 screens have touble seeing the exit button - make a small allowance
     if Screen.Height = 480 then
        SmallScreenDiff := 20
     else
        SmallScreenDiff := 0;

     with theForm do begin
        case theWindowState of
           wsNormal,wsMinimized  : begin
                           //sm('thePanel.Width ' + IntToStr(thePanel.Width));
                           ToolBar.Width  := ControlBar1.Width -1;
                           //sm('ToolBar.Width ' + IntToStr(ToolBar.Width) );
                           //sm('thePanel.Left ' + IntToStr(thePanel.Left) );
                           //sm('theButtonWidth'  + IntToStr(theButtonWidth) );
                           thePanel.Width := (ToolBar.Width  - thePanel.Left) - theButtonWidth - SmallScreenDiff;
                           //sm('thePanel.Width ' + IntToStr(thePanel.Width));
                         end;
           wsMaximized : begin
                           thePanel.Width := pnlWidth - SmallScreenDiff;
                           ToolBar.Width  := ControlBAr1.Width -1;
                         end;
        end;
     end;
end;
{ **************************************************************************** }
procedure ResizeToolBarNew( theForm : TSMStdForm ;
           theButtonWidth : SmallInt ; pnlWidth,pnlLeft : SmallInt; thePanel : TPanel  );
// called from Most forms - the state theWindowState is the State prior to resizing it
var OtherSizeDiff : smallint;
begin
     {if Trunc(DiffRatio) <> 0 then
        OtherSizeDiff := 50
     else
        OtherSizeDiff := 1;}

     with theForm do begin
        case WindowState of
           wsNormal,wsMinimized  : begin
                           //sm('thePanel.Width ' + IntToStr(thePanel.Width));
                           ToolBar.Width  := ControlBar1.Width -1;
                           //sm('ToolBar.Width ' + IntToStr(ToolBar.Width) );
                           //sm('thePanel.Left ' + IntToStr(pnlLeft) );
                           //sm('theButtonWidth'  + IntToStr(theButtonWidth) );
                           thePanel.Width := (ToolBar.Width  - pnlLeft) - theButtonWidth ;//- OtherSizeDiff;
                           //7sm('thePanel.Width ' + IntToStr(thePanel.Width));
                         end;
           wsMaximized : begin
                           thePanel.Width := pnlWidth;
                           ToolBar.Width  := ControlBAr1.Width -1;
                         end;
        end;
     end;
end;
{ **************************************************************************** }

procedure ArrangePanel( thePAnel : TPanel ; x,y : SmallInt );
begin
     thePAnel.Left := x - (thePanel.Width div 2);
     thePAnel.Top  := y - (thePanel.Height div 2);
end;
{ **************************************************************************** }
procedure GetAddressList( Address,Address1,District,City,Country : ShortString ;AddressList : TStrings); 
begin
     try
        //AddressList := TStringList.Create;
        if Address <> '' then AddressList.Add(Address);
        if Address1 <> '' then AddressList.Add(Address1);
        if District <> '' then AddressList.Add(District);
        if City <> '' then AddressList.Add(City);
        if Country <> '' then AddressList.Add(Country);
        //Result.Text := AddressList.Text;  
     finally
        //AddressList.Free;
     end;
end;                                 
{ **************************************************************************** }
procedure AddToClipBoard( Str : String );
begin
     ClipBoard.asText := Str;                
end;
{ **************************************************************************** }
function DeleteLastComma( s : String) : String;
begin
     //if s[length(s)] = ',' then
     //   sm(s[length(s)]);
     system.Delete(s,length(s),1);
     Result := s;
end;
{ **************************************************************************** }
function GetMemberCount : Shortstring;
const SQL1 = 'select count(member) from members';
begin
     result := '';
     if dm.sqlExecQueryCheckEOFCount(dm.qryGeneral,SQL1) then
        Result := IntToStr(dm.qryGeneral.fields[0].AsInteger);
end;
{ **************************************************************************** }
procedure MemberDeleteReport( Member : LongInt );
var   SavePrinterIndex    : SmallInt;
begin
     try
       frmIndividualMember := TfrmIndividualMember.Create(nil);
       with frmIndividualMember do begin
          Setup(Member);
          SavePrinterIndex := Printer.PrinterIndex;
          Printer.PrinterIndex := ReportPrinterIndex;
          qr.PrinterSettings.PrinterIndex := ReportPrinterIndex;
          lblReportTitle.Caption := 'Individual Member Report';
          // testing for Oamaru
          //if not dm.PrintDialog.Execute then exit;
          qr.Print;
       end;  
     finally
       dm.qryReport.Close;
       frmIndividualMember.Free;
       Printer.PrinterIndex := SavePrinterIndex;
     end;
end;
{ **************************************************************************** }
function GetAddressAsString(Address,District,Address1,City,Country : ShortString ): ShortString;
begin
     Result := '';
     if Address <> '' then
        Result := Result + TrimRight(Address) + comma; 
     if District <> '' then
        Result := Result + TrimRight(District) + comma;
     if Address1 <> '' then
        Result := Result + TrimRight(Address1) + comma;
     if City <> '' then
        Result := Result + TrimRight(City) + comma;
     if Country <> '' then
        Result := Result + TrimRight(Country) + comma;
     Result := DeleteLastComma( Result );                     
end;
{ **************************************************************************** }
function FirstLetterUpper ( str : string ) : String;

var MySet: set of 'a'..'z';
var i : integer;
    s : string;
    NextOne : Boolean;

begin
     if str = '' then begin
        result := '';
        exit;
     end;
     str := LowerCase(str);
     //if str[1] in MySet then
        str[1] := chr(ord(str[1]) -32);
     NextOne := False;
     if POS(' ',str) > 0 then begin
      for i := 1 to Length(str) do begin
         if str[i] = ' ' then begin
            NextOne := True;
            continue;
         end;
         if NextOne then begin
            //if str[1] in MySet then
               str[i] := chr(ord(str[i]) - 32);
            NextOne := False;
         end;
      end;
     end; 
     result := str;
end;
{ **************************************************************************** }
procedure SetNumberType( i : integer );
begin
     case i of
        0 : DefaultNumberTypes := ntNextNumber;
        1 : DefaultNumberTypes := ntBlankNumber;
        2 : DefaultNumberTypes := ntNonActive;
        3 : DefaultNumberTypes := ntNonFinancial;
        4 : DefaultNumberTypes := ntNonFinancialNoPoints;
     else
        DefaultNumberTypes := ntNextNumber;
     end;
end;

{******************************************************************************}
procedure BeforeEditPointsCreditSave(var BeforeEditPointsCredit :TBeforeEditPointsCredit;DataSet: TDataSet);
begin
      with BeforeEditPointsCredit,DataSet do begin
         beTotalEarnt      := FieldByName('TOTAL_EARNT').AsFloat;
         beTotalRedeemed   := FieldByName('TOTAL_REDEEMED').AsFloat;
         beEarntCredit     := FieldByName('EARNT_CREDIT').AsFloat;
         beBonusCredit     := FieldByName('BONUS_CREDIT').AsFloat;
         bePrePaidCredit   := FieldByName('PRE_PAID_CREDIT').AsFloat;
         bePointsAvailable := FieldByName('POINTS_AVAILABLE').AsFloat;
      end;  
end;
{ **************************************************************************** }
function NextMonthExpires( mExpires : TDateTime ) : Boolean;
var    Yr, Mnth, Day,ExpYr, ExpMnth, ExpDay: Word;
begin
     // check out the Year as well

     DecodeDate(mExpires, ExpYr, ExpMnth, ExpDay);
     DecodeDate(Date, Yr, Mnth, Day);
     //applies to the next month
     if Mnth = 12 then begin
        Mnth := 1;
        inc(Yr);
     end else
       inc(Mnth);
     if (ExpMnth = Mnth) and (ExpYr = Yr) then
        result := True
     else   
        result := False;
end;
{ ****************************************************************************}       
function WorkOutAge( DOB : TDateTime ) : smallint;
var
    bYear, bMonth, bDay : Word;
    Year, Month, Day : Word;
    CurrentDOB : TDateTime;
begin
     DecodeDate(DOB, bYear, bMonth, bDay);
     DecodeDate(Date, Year, Month, Day);
     Result := Year - bYear;
     // make DOB into current year 10-11-61 to 10-11-03
     try
        CurrentDOB := EncodeDate(Year, bMonth, bDay);
     except   
        if (bMonth = 2) and (bDay = 29) then begin
           dec(bDay); 
           CurrentDOB := EncodeDate(Year, bMonth, bDay);
        end else raise;   
        
     end;
     // havnt had there birthday this year
     if CurrentDOB > Date then 
        dec(Result);
end;
{ ****************************************************************************}
function GetCommaDelimitedField(FieldNum : shortint ; s : String) : String ;
const comma = ',';
var FieldStart,FieldEnd,FieldCount,i : smallint;
// 0 is the first field 
begin         
     result := '';
     FieldStart := 0;
     FieldEnd   := 0;
     FieldCount := 0;
     // move to the FieldNum comma to get StartField
     if FieldNum = 0 then
        FieldStart := 1
     else begin   
        for i := 1 to length(s) do begin
            if s[i] = comma then inc(FieldCount);
            if FieldCount = FieldNum then begin
               FieldStart := i;
               break;
            end;   
         end;   
     end;
     // trim off up to the first comma
     if FieldStart <> 1 then
        system.delete(s,1,FieldStart);
     FieldEnd := pos(comma,s);
     if FieldEnd = 0 then
        // must be the last field - no comma found
        FieldEnd := length(s)
     else 
        // place -1 off the comma
        FieldEnd := FieldEnd -1;
     // not data in between 
     if FieldEnd = 0 then begin
        result := '';
        exit;
     end;   
     Result := copy(s,1,FieldEnd);   
end;


Initialization

     ShortDateFormat := 'dd/mm/yyyy';
     DateSeparator	:= '/';
     LongDateFormat	:= 'dddd, d MMMM yyyy';
     CurrencyFormat	:= 0; // $1
     NegCurrFormat	:= 1; // -$1
     DecimalSeparator := '.';	
     CurrencyDecimals := 2;
     CardFieldStr     := '';
     
     {DecodeDate(now,Year, Month, Day);
     Month := mMonths + 1;
     Day := 31;
     // check for valid date
     for i := 0 to 3 do begin
        try
         EncodeDate(Year, Month, Day);
         break;
        except
          on EConvertError do dec(day);  // 31 downto 28
        end;
     end;
     //StartDate := formatdatetime('mm/dd/yyyy',EnCodeDate(Year,Month,01));
     //EndDate   := formatdatetime('mm/dd/yyyy',EnCodeDate(Year,Month,Day));
     }

end.

