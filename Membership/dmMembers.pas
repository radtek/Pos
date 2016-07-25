
unit dmMembers;
interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  DBTables, Db, IBCustomDataSet, IBSQL, IBQuery, IBDatabase, IBTable,
  IBDatabaseInfo, IBUpdateSQL, IB, ImgList, ExtCtrls, Wwdatsrc, IBServices,
  OpShared, OpXL2K, OpExcel, OpModels, OpDbOfc,activeX, IBEvents,
  IBStoredProc,comctrls, lmdcompo, lmdclass, lmdnonvS, OpWrd2k, OpWord,FileCtrl,stdForm,
  Menus, DBClient, OpWrdXP, OpXLXP, wwDialog, wwidlg;

type
  Tdm = class(TDataModule)
    dsrMembers: TDataSource;
    dsrM_Events: TDataSource;
    dsrM_Subs: TDataSource;
    dsrM_Sections: TDataSource;
    IBDatabase: TIBDatabase;
    IBTransaction: TIBTransaction;
    tblTenders: TIBTable;
    dsrTblTenders: TDataSource;
    tblEvents: TIBTable;
    tblSubs: TIBTable;
    tblSections: TIBTable;
    tblGroups: TIBTable;
    dsrTblGroups: TDataSource;
    dsrMembersLook: TDataSource;
    qryMembersLook: TIBQuery;
    qryMembersLookSURNAME: TIBStringField;
    qryMembersLookMEMBER: TIntegerField;
    qryMembersLookFULL_NAME: TIBStringField;
    ibdsMembers: TIBDataSet;
    dsrTblEvents: TDataSource;
    dsrTblSections: TDataSource;
    dsrTblSubs: TDataSource;
    tblEventsEVENT: TIntegerField;
    tblEventsDESCRIPTION: TIBStringField;
    tblEventsBONUS_POINTS: TIntegerField;
    tblEventsCUST_DISPLAY: TIBStringField;
    qryGeneral: TIBSQL;
    qryMembersLookADDRESS: TIBStringField;
    ibdsM_Subs: TIBDataSet;
    ibdsM_Events: TIBDataSet;
    ibdsM_EventsEVENT: TIntegerField;
    ibdsM_EventsMEMBER: TIntegerField;
    ibdsM_Sections: TIBDataSet;
    ibdsM_SectionsMEMBER: TIntegerField;
    ibdsM_SectionsSECTION: TIntegerField;
    ibdsM_SectionsPAID: TIBStringField;
    ibdsM_SectionsRECEIPT: TIntegerField;
    ibdsM_SectionsSPONSORSHIP: TIBStringField;
    qryMembersLookFIRST_NAME: TIBStringField;
    qryPropSec: TIBQuery;
    dsrPropSec: TDataSource;
    qryPropSecMEMBER: TIntegerField;
    qryPropSecFULL_NAME: TIBStringField;
    qryPropSecSURNAME: TIBStringField;
    qryPropSecADDRESS: TIBStringField;
    qryPropSecFIRST_NAME: TIBStringField;
    qryReport1: TIBQuery;
    tblSite: TIBTable;
    tblMisc: TIBTable;
    dsrTblMisc: TDataSource;
    dsrTblSite: TDataSource;
    dsrBackupLog: TDataSource;
    dsrUserSecView: TDataSource;
    tblUserSecView: TIBTable;
    tblUserSecViewUSER_CODE: TIntegerField;
    tblUserSecViewSCREEN_CODE: TIntegerField;
    tblUserSecViewHAS_ACCESS: TIBStringField;
    qryMembersPAY: TIBQuery;
    dsrMembersPAY: TDataSource;
    qryReport: TIBQuery;
    dsrPayDet: TDataSource;
    dsrPayJoin: TDataSource;
    ibdsPayJoin: TIBDataSet;
    ibdsPayJoinMEMBER: TIntegerField;
    ibdsPayMembers: TIBDataSet;
    dsrPayMembers: TDataSource;
    ibdsPayMembersMEMBER: TIntegerField;
    ibdsPayMembersFULL_NAME: TIBStringField;
    ibdsPayMembersEDIT_DATE: TDateTimeField;
    qryMembersPAYSURNAME: TIBStringField;
    qryMembersPAYMEMBER: TIntegerField;
    qryMembersPAYFULL_NAME: TIBStringField;
    qryMembersPAYFIRST_NAME: TIBStringField;
    qryMembersPAYADDRESS: TIBStringField;
    ibdsPayDet: TIBDataSet;
    ibdsPayDetMEMBER: TIntegerField;
    ibdsPayDetSUBSCRIPTION: TIntegerField;
    ibdsPayDetDESCRIPTION: TIBStringField;
    ibdsPayDetEDIT_DATE: TDateTimeField;
    ibdsPayDetRECEIPT: TIntegerField;
    ibdsPayDetPAID: TIBStringField;
    IBTransPay: TIBTransaction;
    sqlTransPay: TIBSQL;
    ibdsPayMembersSUB_PAID: TSmallintField;
    ibdsPayMembersRECEIPT: TIntegerField;
    ibdsPayJoinRECEIPT: TIntegerField;
    dsrPayments: TDataSource;
    dsrPay_Det: TDataSource;
    ibdsPayJoinFULL_NAME: TIBStringField;
    ibdsPayDetFULL_NAME: TIBStringField;
    ibdsTenders: TIBDataSet;
    dsrTenders: TDataSource;
    ibdsTendersTENDER: TIntegerField;
    ibdsTendersTENDER_TYPE: TIBStringField;
    ibdsTendersDETAILS: TIBStringField;
    qryPayTend: TIBQuery;
    qryPayTendPAYMENT_TEND: TIntegerField;
    qryPayTendPAYMENT: TIntegerField;
    qryPayTendTENDER_TYPE: TIBStringField;
    qryPayTendDETAILS: TIBStringField;
    tblTendersTENDER_TYPE: TIBStringField;
    sqlRollover: TIBSQL;
    IBRollover: TIBTransaction;
    tblSiteCOMP_NAME: TIBStringField;
    tblSiteADDRESS1: TIBStringField;
    tblSiteADDRESS2: TIBStringField;
    tblSiteADDRESS3: TIBStringField;
    tblSitePOSTCODE: TIBStringField;
    tblSitePHONE: TIBStringField;
    tblSiteGST_NO: TIBStringField;
    tblSiteGST_RATE: TFloatField;
    tblSiteSITE_LICENSE: TIntegerField;
    tblSiteMEMBER_LICENSE: TIBStringField;
    tblSitePOS_LICENSE: TIBStringField;
    tblSiteHELP_DESK: TIBStringField;
    tblSiteFAX: TIBStringField;
    ibdsPayDetSUB_PAID: TSmallintField;
    qryReport2: TIBQuery;
    qryReport2MEMBER: TIntegerField;
    qryReport2FULL_NAME: TIBStringField;
    qryReport2ADDRESS: TIBStringField;
    qryReport2ADDRESS1: TIBStringField;
    qryReport2HOME_PHONE: TIBStringField;
    qryReport2FIRST_NAME: TIBStringField;
    qryReport2IS_ACTIVE: TIBStringField;
    dsrReport2: TwwDataSource;
    qryReport2KNOWN_AS: TIBStringField;
    qryReport2MALE_FEMALE: TIBStringField;
    qryReport2MAILING_LIST: TIBStringField;
    qryReport2MAILING_CODE: TIntegerField;
    qryReport2MAILING_LIST_CODE: TIntegerField;
    qryReport2NEWSLETTER: TIBStringField;
    qryReport2EMAIL: TIBStringField;
    qryReport2WORK_PHONE: TIBStringField;
    qryReport2CELL_PHONE: TIBStringField;
    qryReport2PROPOSER: TIntegerField;
    qryReport2SECONDER: TIntegerField;
    qryReport2SUB_PAID: TSmallintField;
    qryReport2RECEIPT: TIntegerField;
    qryReport2TRANS_FROM: TIBStringField;
    qryReport2ALLOW_CREDIT: TIBStringField;
    qryReport2PRICE_LEVEL: TIntegerField;
    qryReport2VIP_PAY_ONLY: TIBStringField;
    qryReport2CARD_TYPE: TIBStringField;
    qryReport2INCL_EOD: TIBStringField;
    qryReport2POSITIONS: TIBStringField;
    qryReport2CHARGE_CODE: TIntegerField;
    qryReport2DECEASED: TIBStringField;
    qryReport2SENIOR: TIBStringField;
    qryReport2FOUNDATION: TIBStringField;
    qryReport2DONATED: TIBStringField;
    qryReport2REFUNDED: TIBStringField;
    qryReport2DEBENTURE: TIntegerField;
    qryReport2SAVED: TIBStringField;
    qryReport2TITLE: TIBStringField;
    qryReport2INITIALS: TIBStringField;
    qryReport2SURNAME: TIBStringField;
    qryReport2ANALYSE: TIBStringField;
    qryReport2DISTRICT: TIBStringField;
    ibdsPaySections: TIBDataSet;
    dsrPaySections: TDataSource;
    ibdsPaySectionsMEMBER: TIntegerField;
    ibdsPaySectionsFULL_NAME: TIBStringField;
    ibdsPaySectionsSURNAME: TIBStringField;
    ibdsPaySectionsSECTION: TIntegerField;
    ibdsPaySectionsEDIT_DATE: TDateTimeField;
    ibdsPaySectionsRECEIPT: TIntegerField;
    ibdsPaySecMem: TIBDataSet;
    ibdsPaySecMemMEMBER: TIntegerField;
    ibdsPaySecMemEDIT_DATE: TDateTimeField;
    dsrPaySecMem: TDataSource;
    ibdsPaySectionsPAID: TIBStringField;
    ibdsPaySecMemFULL_NAME: TIBStringField;
    dsrProcess: TDataSource;
    qryProcess: TIBQuery;
    qryProcessMSG: TIBStringField;
    qryProcessEDIT_DATE: TDateTimeField;
    qryBackupLog: TIBQuery;
    qryReport2DAIRY_NOTES: TBlobField;
    qryReport2EDIT_DATE: TDateTimeField;
    ibdsLetters: TIBDataSet;
    ibdsLettersNAME: TIBStringField;
    dsrLetters: TDataSource;
    ibdsM_SectionsAMOUNT: TIBBCDField;
    ibdsM_SectionsAMOUNT_PAID: TIBBCDField;
    ibdsPayJoinJOINING_FEE: TIBBCDField;
    ibdsPayMembersDISCOUNT: TIBBCDField;
    ibdsPayDetAMOUNT_PAID: TIBBCDField;
    ibdsPayDetAMOUNT: TIBBCDField;
    ibdsTendersEX_RATE: TIBBCDField;
    ibdsTendersAMOUNT: TIBStringField;
    ibdsPaySectionsAMOUNT_PAID: TIBBCDField;
    ibdsPaySectionsAMOUNT: TIBBCDField;
    ibdsPaySectionsAMOUNT_DUE: TIBBCDField;
    qryPayTendEX_RATE: TIBBCDField;
    qryPayTendAMOUNT: TIBBCDField;
    tblTendersTENDER: TIntegerField;
    tblTendersEX_RATE: TIBBCDField;
    tblSectionsSECTION: TIntegerField;
    tblSectionsFEE: TIBBCDField;
    tblSectionsSAVED: TIBStringField;
    tblSectionsDAY_PAYMENTS: TIBStringField;
    tblSectionsDESCRIPTION: TIBStringField;
    tblSectionsLAST_ROLL: TDateField;
    tblSectionsEXPIRES: TDateField;
    ibdsM_SectionsEXPIRES: TDateField;
    ibdsM_SectionsEDIT_DATE: TDateTimeField;
    ibdsM_EventsDATE_LAST: TDateField;
    ibdsM_EventsEDIT_DATE: TDateTimeField;
    ibdsM_EventsDATES: TDateField;
    qryReport2DATE_OF_BIRTH: TDateField;
    qryReport2LUCKY_DATE: TDateField;
    qryReport2JOINED: TDateField;
    qryReport2APPROVED: TDateField;
    qryReport2EXPIRES: TDateField;
    qryReport2INACTIVE_DATE: TDateField;
    ibdsPayJoinEXPIRES: TDateField;
    ibdsPayMembersEXPIRES: TDateField;
    ibdsPayDetEXPIRES: TDateField;
    qryReport2JOINING_FEE: TIBBCDField;
    ibdsPaySectionsEXPIRES: TDateField;
    IBBackupService: TIBBackupService;
    IBDatabaseInfo: TIBDatabaseInfo;
    IBSecurityService: TIBSecurityService;
    tblUserSecViewSCREEN_DESC: TStringField;
    qryProcessUSER_CODE: TIntegerField;
    qryReport2AMOUNT_DONATED: TIBBCDField;
    qryReport2DISCOUNT: TIBBCDField;
    dsrPayTend: TDataSource;
    tblScreens: TIBTable;
    dsrScreens: TDataSource;
    tblScreensSCREEN_CODE: TIntegerField;
    tblScreensSCREEN_DESC: TIBStringField;
    tblScreensDEF_ACCESS: TIBStringField;
    ibdsPayJoinPAY: TIBBCDField;
    ibdsPayDetPAY: TIBBCDField;
    qryMembersPAYMAILING_LIST_CODE: TIntegerField;
    ibdsPayMemSec: TIBDataSet;
    dsrPayMemSec: TDataSource;
    ibdsPayMemSecMEMBER: TIntegerField;
    ibdsPayMemSecFULL_NAME: TIBStringField;
    ibdsPayMemSecDESCRIPTION: TIBStringField;
    ibdsPayMemSecAMOUNT_PAID: TIBBCDField;
    ibdsPayMemSecAMOUNT: TIBBCDField;
    ibdsPayMemSecPAY: TIBBCDField;
    ibdsPayMemSecEXPIRES: TDateField;
    ibdsPayMemSecEDIT_DATE: TDateTimeField;
    ibdsPayMemSecRECEIPT: TIntegerField;
    ibdsPayMemSecSECTION: TIntegerField;
    ibdsPayMemSecPAID: TIBStringField;
    ibdsM_EventsDESCRIPTION: TStringField;
    tblGroupsGROUPS: TIntegerField;
    tblGroupsDESCRIPTION: TIBStringField;
    tblGroupsVIP_POINTS: TIntegerField;
    tblSubsSUBSCRIPTION: TIntegerField;
    tblSubsDESCRIPTION: TIBStringField;
    tblSubsPOINTS_SPENT: TIntegerField;
    tblSubsCUST_DISPLAY: TIBStringField;
    tblSubsFEE: TIBBCDField;
    tblSubsSAVED: TIBStringField;
    ibdsM_SubsMEMBER: TIntegerField;
    ibdsM_SubsSUBSCRIPTION: TIntegerField;
    ibdsM_SubsAMOUNT_PAID: TIBBCDField;
    ibdsM_SubsAMOUNT: TIBBCDField;
    ibdsM_SubsPAID: TIBStringField;
    ibdsM_SubsEDIT_DATE: TDateTimeField;
    ibdsM_SubsRECEIPT: TIntegerField;
    ibdsM_SubsDESCRIPTION: TIBStringField;
    ibdsM_SectionsDESCRIPTION: TIBStringField;
    tblMailCodes: TIBTable;
    dsrTblMailCodes: TDataSource;
    tblMailCodesMAIL_CODE: TIntegerField;
    tblMailCodesDESCRIPTION: TIBStringField;
    tblSectionsGL_ACCOUNT: TIBStringField;
    qryReport2GL_ACCOUNT: TIBStringField;
    ibdsMembersDef: TIBDataSet;
    IntegerField1: TIntegerField;
    IBStringField1: TIBStringField;
    IBStringField2: TIBStringField;
    DateField1: TDateField;
    DateField2: TDateField;
    IBStringField3: TIBStringField;
    IBStringField4: TIBStringField;
    IBStringField5: TIBStringField;
    IntegerField2: TIntegerField;
    IntegerField3: TIntegerField;
    IBStringField6: TIBStringField;
    IBStringField7: TIBStringField;
    IBStringField8: TIBStringField;
    IBStringField9: TIBStringField;
    IBStringField10: TIBStringField;
    IntegerField4: TIntegerField;
    IntegerField5: TIntegerField;
    DateField3: TDateField;
    IBBCDField1: TIBBCDField;
    IntegerField6: TIntegerField;
    DateField4: TDateField;
    DateField5: TDateField;
    IBStringField11: TIBStringField;
    SmallintField1: TSmallintField;
    SmallintField2: TSmallintField;
    IBStringField12: TIBStringField;
    IBBCDField2: TIBBCDField;
    IBStringField13: TIBStringField;
    IBBCDField10: TIBBCDField;
    IBBCDField11: TIBBCDField;
    IntegerField8: TIntegerField;
    IBStringField14: TIBStringField;
    IBStringField15: TIBStringField;
    IBStringField16: TIBStringField;
    IBBCDField12: TIBBCDField;
    IBStringField17: TIBStringField;
    IBStringField18: TIBStringField;
    IntegerField9: TIntegerField;
    IBStringField19: TIBStringField;
    IBStringField20: TIBStringField;
    IBStringField21: TIBStringField;
    IBStringField22: TIBStringField;
    IBStringField23: TIBStringField;
    IntegerField10: TIntegerField;
    IBStringField24: TIBStringField;
    BlobField1: TBlobField;
    DateTimeField1: TDateTimeField;
    IBStringField25: TIBStringField;
    IBStringField26: TIBStringField;
    IBStringField27: TIBStringField;
    IBStringField28: TIBStringField;
    IBStringField29: TIBStringField;
    IBStringField30: TIBStringField;
    IBStringField31: TIBStringField;
    DateField6: TDateField;
    IBStringField32: TIBStringField;
    ibdsLettersPROTECT: TIBStringField;
    ibdsLettersLETTER: TBlobField;
    OpDataSetModel: TOpDataSetModel;
    OpExcel: TOpExcel;
    tblMiscMEMBER_EXPIRY: TDateField;
    tblMiscKEEP_INACTIVE: TIntegerField;
    tblMiscSUBS_ROLL: TDateField;
    tblMiscLUCKY_ROLL: TDateField;
    tblMiscREVOLVE_DAYS: TIntegerField;
    tblMiscNAME_FIELDS: TIBStringField;
    tblMiscM_BACKUP_ENABLED: TIBStringField;
    tblMiscM_BACKUP_DIR: TIBStringField;
    tblMiscM_BACKUP_PCNAME: TIBStringField;
    tblMiscM_BACKUP_DATE: TDateField;
    tblMiscM_BACKUP_TYPE: TSmallintField;
    tblMiscA_BACKUP_ENABLED: TIBStringField;
    tblMiscA_BACKUP_DIR: TIBStringField;
    tblMiscA_BACKUP_PCNAME: TIBStringField;
    tblMiscA_BACKUP_DATE: TDateField;
    tblMiscA_BACKUP_TIME: TTimeField;
    tblMiscA_BACKUP_TYPE: TSmallintField;
    tblMiscA_BACKUP_FILE_NAMES: TIBStringField;
    tblMiscMISC: TSmallintField;
    tblMiscA_BACKUP_KEEP_DAYS: TSmallintField;
    qryReport2CITY: TIBStringField;
    qryReport2COUNTRY: TIBStringField;
    ibdsMembersDefCITY: TIBStringField;
    ibdsMembersDefCOUNTRY: TIBStringField;
    IBEvents: TIBEvents;
    IBStoredProc: TIBStoredProc;
    tblMiscMSG: TIBStringField;
    tblMiscAREV_IMPORT_FILE_PATH: TIBStringField;
    tblNumbers: TIBTable;
    tblMiscMEMBER_PRE_EXPIRY: TSmallintField;
    dsrTblNumbers: TDataSource;
    tblNumbersNUMBER_ID: TIntegerField;
    tblNumbersDESCRIPTION: TIBStringField;
    tblNumbersNEXT_MEMBER: TIntegerField;
    dsrTblPrinters: TDataSource;
    tblPrinters: TIBTable;
    tblVersions: TIBTable;
    dsrTblVersions: TDataSource;
    tblVersionsPC_NAME: TIBStringField;
    tblVersionsINSTALL_DIR: TIBStringField;
    tblVersionsVERSION: TIBStringField;
    tblVersionsSHARE_NAME: TIBStringField;
    tblVersionsPROGRAM: TIBStringField;
    qryBackupLogBACKUP: TIntegerField;
    qryBackupLogNAME: TIBStringField;
    qryBackupLogDATE_TIME: TDateTimeField;
    qryBackupLogLOG: TMemoField;
    qryBackupLogSUCCESS: TIBStringField;
    qryBackupLogTYPE: TSmallintField;
    qryBackupLogMAN_AUTO: TIBStringField;
    qryBackupLogPCNAME: TIBStringField;
    qryBackupLogNOTIFIED: TIBStringField;
    ibdsExport: TIBDataSet;
    SysInfo: TLMDSysInfo;
    ExportTimer: TTimer;
    tblMiscCONTROLLER: TIBStringField;
    IBExport: TIBTransaction;
    sqlExport: TIBSQL;
    ibdsExportEXPORT_DATE: TDateTimeField;
    ibdsExportEXPORT_ID: TIntegerField;
    ibdsExportIN_TRANS: TIBStringField;
    ibdsExportMEMBER: TIntegerField;
    dsrExport: TDataSource;
    ibdsExportEXPORTED: TIBStringField;
    ImageListLarge: TImageList;
    IBUpdateSQL: TIBUpdateSQL;
    PrintDialog: TPrintDialog;
    tblMiscCAPITALS: TIBStringField;
    tblCardMaster: TIBTable;
    tblCardDetail: TIBTable;
    tblCardMasterCARD_ID: TIntegerField;
    tblCardMasterCARD_NAME: TIBStringField;
    dsrCardMaster: TDataSource;
    dsrCardDetail: TDataSource;
    tblMiscDEFAULT_CARDID: TSmallintField;
    tblCardDetailCARD_DETAIL_ID: TIntegerField;
    tblCardDetailCARD_ID: TIntegerField;
    tblCardDetailFIELD_NAME: TIBStringField;
    tblCardDetailHORIZONTAL: TSmallintField;
    tblCardDetailVERTICAL: TSmallintField;
    tblCardDetailROTATION: TSmallintField;
    tblCardDetailSTYLE: TSmallintField;
    tblCardDetailPROPORTION: TSmallintField;
    tblCardDetailSIZE: TSmallintField;
    tblCardDetailGRAPHIC_MODE: TSmallintField;
    qryQuery: TIBQuery;
    dsrQuery: TDataSource;
    OpenDialog: TOpenDialog;
    SaveDialog: TSaveDialog;
    ibdsMembersDefSWIPE_CARD_NUMBER: TIntegerField;
    ibdsMembersDefFAX_NUMBER: TIBStringField;
    ibdsMembersDefCOMPANY: TIBStringField;
    ibdsMembersDefOCCUPATION: TIBStringField;
    ibdsMembersDefBARCODE_NUMBER: TIntegerField;
    ibdsMembersDefCARD_LAST_USED: TDateField;
    ibdsMembersDefAUTO_PAYMENT: TIBStringField;
    ibdsMembersDefPAYMENT_PERIOD: TSmallintField;
    dsrAttachSubs: TwwDataSource;
    qryAttachSubs: TIBQuery;
    qryAttachSubsMEMBER: TIntegerField;
    qryAttachSubsSUBSCRIPTION: TIntegerField;
    qryAttachSubsAMOUNT_PAID: TIBBCDField;
    qryAttachSubsAMOUNT: TIBBCDField;
    qryAttachSubsPAID: TIBStringField;
    qryAttachSubsEDIT_DATE: TDateTimeField;
    qryAttachSubsRECEIPT: TIntegerField;
    qryAttachSubsDescription: TStringField;
    tblSubsKEEP_EXPIRES_CURRENT: TIBStringField;
    tblCardDetailTEXT_POSITION: TIBStringField;
    tblCardDetailCARD_TEXT: TIBStringField;
    tblCardDetailSIZE_REDUCE: TSmallintField;
    tblCardDetailTEXT_LENGTH: TSmallintField;
    qryGeneral1: TIBSQL;
    qryReport2FAX_NUMBER: TIBStringField;
    qryReport2SECTION_PAID: TSmallintField;
    qryReport2PAY: TIBBCDField;
    qryReport2SUB_PAYMENT: TIBStringField;
    qryReport2BARCODE_NUMBER: TIntegerField;
    qryReport2SWIPE_CARD_NUMBER: TIntegerField;
    qryReport2CARD_LAST_USED: TDateField;
    qryReport2AUTO_PAYMENT: TIBStringField;
    qryReport2RECEIVE_EMAIL: TIBStringField;
    qryReport2RESIDENTIAL: TIBStringField;
    qryReport2COMPANY: TIBStringField;
    qryReport2OCCUPATION: TIBStringField;
    qryReport2BOAT_LISTING: TIntegerField;
    qryReport2TIME: TTimeField;
    qryReport2PAYMENT_PERIOD: TSmallintField;
    ibdsMembersMEMBER: TIntegerField;
    ibdsMembersKNOWN_AS: TIBStringField;
    ibdsMembersMALE_FEMALE: TIBStringField;
    ibdsMembersDATE_OF_BIRTH: TDateField;
    ibdsMembersLUCKY_DATE: TDateField;
    ibdsMembersADDRESS: TIBStringField;
    ibdsMembersADDRESS1: TIBStringField;
    ibdsMembersMAILING_LIST: TIBStringField;
    ibdsMembersMAILING_CODE: TIntegerField;
    ibdsMembersMAILING_LIST_CODE: TIntegerField;
    ibdsMembersNEWSLETTER: TIBStringField;
    ibdsMembersEMAIL: TIBStringField;
    ibdsMembersHOME_PHONE: TIBStringField;
    ibdsMembersWORK_PHONE: TIBStringField;
    ibdsMembersCELL_PHONE: TIBStringField;
    ibdsMembersPROPOSER: TIntegerField;
    ibdsMembersSECONDER: TIntegerField;
    ibdsMembersJOINED: TDateField;
    ibdsMembersJOINING_FEE: TIBBCDField;
    ibdsMembersRECEIPT: TIntegerField;
    ibdsMembersAPPROVED: TDateField;
    ibdsMembersEXPIRES: TDateField;
    ibdsMembersTRANS_FROM: TIBStringField;
    ibdsMembersSUB_PAID: TSmallintField;
    SECTION_PAID: TSmallintField;
    ibdsMembersALLOW_CREDIT: TIBStringField;
    ibdsMembersCREDIT_LIMIT: TIBBCDField;
    ibdsMembersEARN_POINTS: TIBStringField;
    ibdsMembersPRODUCT_PURCHASE: TIBBCDField;
    ibdsMembersAMOUNT_DONATED: TIBBCDField;
    ibdsMembersPRICE_LEVEL: TIntegerField;
    ibdsMembersVIP_PAY_ONLY: TIBStringField;
    ibdsMembersCARD_TYPE: TIBStringField;
    ibdsMembersGL_ACCOUNT: TIBStringField;
    ibdsMembersDISCOUNT: TIBBCDField;
    ibdsMembersINCL_EOD: TIBStringField;
    ibdsMembersPOSITIONS: TIBStringField;
    ibdsMembersCHARGE_CODE: TIntegerField;
    ibdsMembersDECEASED: TIBStringField;
    ibdsMembersSENIOR: TIBStringField;
    ibdsMembersFOUNDATION: TIBStringField;
    ibdsMembersDONATED: TIBStringField;
    ibdsMembersREFUNDED: TIBStringField;
    ibdsMembersDEBENTURE: TIntegerField;
    ibdsMembersSAVED: TIBStringField;
    ibdsMembersDAIRY_NOTES: TBlobField;
    ibdsMembersEDIT_DATE: TDateTimeField;
    ibdsMembersTITLE: TIBStringField;
    ibdsMembersINITIALS: TIBStringField;
    ibdsMembersFIRST_NAME: TIBStringField;
    ibdsMembersSURNAME: TIBStringField;
    ibdsMembersFULL_NAME: TIBStringField;
    ibdsMembersIS_ACTIVE: TIBStringField;
    ibdsMembersANALYSE: TIBStringField;
    ibdsMembersINACTIVE_DATE: TDateField;
    ibdsMembersDISTRICT: TIBStringField;
    ibdsMembersCITY: TIBStringField;
    ibdsMembersCOUNTRY: TIBStringField;
    ibdsMembersSUB_PAYMENT: TIBStringField;
    ibdsMembersSWIPE_CARD_NUMBER: TIntegerField;
    ibdsMembersFAX_NUMBER: TIBStringField;
    ibdsMembersCOMPANY: TIBStringField;
    ibdsMembersOCCUPATION: TIBStringField;
    ibdsMembersBARCODE_NUMBER: TIntegerField;
    ibdsMembersCARD_LAST_USED: TDateField;
    ibdsMembersAUTO_PAYMENT: TIBStringField;
    ibdsMembersPAYMENT_PERIOD: TSmallintField;
    ibdsMembersINVOICE_NUMBER: TIBStringField;
    ibdsMembersPRINT_CARD: TIBStringField;
    ibdsMembersPRINT_RECEIPT: TIBStringField;
    ibdsMembersDefINVOICE_NUMBER: TIBStringField;
    ibdsMembersDefPRINT_CARD: TIBStringField;
    ibdsMembersDefPRINT_RECEIPT: TIBStringField;
    qryReport2INVOICE_NUMBER: TIBStringField;
    qryReport2PRINT_CARD: TIBStringField;
    qryReport2PRINT_RECEIPT: TIBStringField;
    ibdsBulkCardRec: TIBDataSet;
    dsrBuldCardRec: TDataSource;
    ibdsBulkCardRecAMOUNT_PAID: TIBBCDField;
    ibdsBulkCardRecDESCRIPTION: TIBStringField;
    ibdsBulkCardRecEXPIRES: TDateField;
    ibdsBulkCardRecFULL_NAME: TIBStringField;
    ibdsBulkCardRecINVOICE_NUMBER: TIBStringField;
    ibdsBulkCardRecMEMBER: TIntegerField;
    ibdsBulkCardRecPAYMENT: TIntegerField;
    ibdsBulkCardRecPAYMENT_PERIOD: TSmallintField;
    ibdsBulkCardRecAUTO_PAYMENT: TIBStringField;
    ibdsBulkCardRecBARCODE_NUMBER: TIntegerField;
    ibdsBulkCardRecPRINT_CARD: TIBStringField;
    ibdsBulkCardRecPRINT_RECEIPT: TIBStringField;
    ibdsMembersRECEIVE_EMAIL: TIBStringField;
    ibdsMembersDefRECEIVE_EMAIL: TIBStringField;
    ibdsBulkCardRecNUMBER_CARDS: TSmallintField;
    ibdsBulkCardRecFIRST_NAME: TIBStringField;
    ibdsBulkCardRecSURNAME: TIBStringField;
    ibdsBulkCardRecTENDER_TYPE: TIBStringField;
    ibdsBulkCardRecADDRESS: TIBStringField;
    ibdsBulkCardRecADDRESS1: TIBStringField;
    ibdsBulkCardRecDISTRICT: TIBStringField;
    ibdsBulkCardRecCITY: TIBStringField;
    ibdsBulkCardRecCOUNTRY: TIBStringField;
    tblSectionsPRINT_ON_CARD: TIBStringField;
    tblSectionsCARD_DESC: TIBStringField;
    tblSectionsSPARE_SMALLINT: TSmallintField;
    tblSectionsSPARE_CHAR: TIBStringField;
    tblSubsGL_ACCOUNT: TIBStringField;
    tblSubsPRO_RATA: TIBStringField;
    tblSubsDISCOUNTABLE: TIBStringField;
    tblSubsPRINT_ON_CARD: TIBStringField;
    tblSubsCARD_DESC: TIBStringField;
    tblSubsSPARE_CHAR: TIBStringField;
    tblSubsSPARE_SMALLINT: TSmallintField;
    qryReport2NUMBER_CARDS: TSmallintField;
    qryReport2CREDIT_LIMIT: TIBBCDField;
    qryReport2EARN_POINTS: TIBStringField;
    qryReport2PRODUCT_PURCHASE: TIBBCDField;
    tblCardMember: TIBTable;
    dsrCardMember: TDataSource;
    tblCardMemberMEMBER: TIntegerField;
    tblCardMemberNUMBER_CARDS: TSmallintField;
    tblGroupsCARD_DESC: TIBStringField;
    tblGroupsPRINT_ON_CARD: TIBStringField;
    tblCardDetailMULTI_GAP: TSmallintField;
    tblCardDetailEXPORT_LINES: TSmallintField;
    IBRunOnce: TIBTransaction;
    sqlRunOnce: TIBSQL;
    ImageList1: TImageList;
    ibdsExportPOINTS_USED: TIntegerField;
    ibdsBulkCardRecPAY_DATE: TDateField;
    ibdsAutoPay: TIBDataSet;
    dsrAutoPAy: TDataSource;
    ibdsAutoPayMEMBER: TIntegerField;
    ibdsAutoPayEXPIRES: TDateField;
    ibdsAutoPaySUBSCRIPTION: TIntegerField;
    ibdsAutoPayAMOUNT_PAID: TIBBCDField;
    ibdsAutoPayAMOUNT: TIBBCDField;
    ibdsAutoPaySUB_PAID: TSmallintField;
    ibdsAutoPayPAY: TIBBCDField;
    ibdsAutoPayPAYMENT_PERIOD: TSmallintField;
    ibdsAutoPayDESCRIPTION: TIBStringField;
    ibdsAutoPayEDIT_DATE: TDateTimeField;
    ibdsAutoPayRECEIPT: TIntegerField;
    ibdsAutoPayPAID: TIBStringField;
    ibdsAutoPayFIRST_NAME: TIBStringField;
    ibdsAutoPaySURNAME: TIBStringField;
    ibdsAutoPayFULL_NAME: TIBStringField;
    ibdsAutoPayAUTO_PAYMENT: TIBStringField;
    ibdsPoints: TIBDataSet;
    dsrPoints: TDataSource;
    ibdsPointsMEMBER: TIntegerField;
    ibdsPointsKNOWN_AS: TIBStringField;
    ibdsPointsFULL_NAME: TIBStringField;
    ibdsPointsPAY: TIBBCDField;
    ibdsM_Groups: TIBDataSet;
    dsrM_Groups: TDataSource;
    ibdsM_GroupsGROUPS: TSmallintField;
    ibdsM_GroupsMEMBER: TIntegerField;
    ibdsM_GroupsDESCRIPTION: TStringField;
    ibdsM_Lots: TIBDataSet;
    dsrM_Lots: TDataSource;
    ibdsM_LotsLOT_ID: TIntegerField;
    ibdsM_LotsMEMBER: TIntegerField;
    dsrM_Interests: TDataSource;
    ibdsM_Interests: TIBDataSet;
    ibdsM_InterestsCOMMENTS: TIBStringField;
    ibdsM_InterestsMEMBER: TIntegerField;
    ibdsM_InterestsSECTION: TIntegerField;
    ibdsM_InterestsDESCRIPTION: TStringField;
    ibdsM_LotsDESCRIPTION: TIBStringField;
    ibdsMembersNUMBER_CARDS: TSmallintField;
    ibdsM_LotsLOT_TYPE: TIBStringField;
    ibdsPayDetAUTO_PAYMENT: TIBStringField;
    ibdsPayDetPAYMENT_PERIOD: TSmallintField;
    ibdsPayDetPRO_RATA: TIBStringField;
    ibdsPayDetDISCOUNTABLE: TIBStringField;
    ibdsPayDetSUB_PAYMENT: TIBStringField;
    tblMiscSECTION_REVOLVE_DAYS: TSmallintField;
    tblMiscANNUAL_CARD: TIBStringField;
    tblSubsCODE: TIBStringField;
    tblSubsSUB_GROUPS: TIntegerField;
    tblSubsGROUP_DESC: TStringField;
    ibdsMembersDefPAYMENT_DAY: TSmallintField;
    ibdsMembersPAYMENT_DAY: TSmallintField;
    tblSubsAP_PREMIUM: TIBBCDField;
    ibdsPayDetMAILING_LIST_CODE: TIntegerField;
    qryMembersLookMAILING_LIST_CODE: TIntegerField;
    ibdsMembersSTREET_ADDRESS: TIBStringField;
    ibdsMembersSTREET_ADDRESS1: TIBStringField;
    ibdsMembersSTREET_CITY: TIBStringField;
    ibdsMembersSTREET_COUNTRY: TIBStringField;
    ibdsMembersSTREET_DISTRICT: TIBStringField;
    ibdsMembersDefSTREET_ADDRESS: TIBStringField;
    ibdsMembersDefSTREET_ADDRESS1: TIBStringField;
    ibdsMembersDefSTREET_CITY: TIBStringField;
    ibdsMembersDefSTREET_COUNTRY: TIBStringField;
    ibdsMembersDefSTREET_DISTRICT: TIBStringField;
    ibdsPayDetPRINT_CARD: TIBStringField;
    ibdsPayDetPRINT_RECEIPT: TIBStringField;
    ibdsPayDetINVOICED: TIBStringField;
    ibdsM_SubsSUB_GROUPS: TIntegerField;
    ibdsMembersDefNUMBER_CARDS: TSmallintField;
    ibdsPayDetSUB_GROUPS: TIntegerField;
    ibdsM_SubsAMOUNT_DUE: TIBBCDField;
    ibdsM_SubsCODE: TStringField;
    ibdsMembersFAX_WORK: TIBStringField;
    ibdsMembersDefFAX_WORK: TIBStringField;
    tblMiscDISABLE_SERVICES: TIBStringField;
    ibdsPay_Det: TIBDataSet;
    ibdsPAyments: TIBDataSet;
    ibdsPay_DetAMOUNT: TIBBCDField;
    ibdsPay_DetAMOUNT_PAID: TIBBCDField;
    ibdsPay_DetCODE: TIntegerField;
    ibdsPay_DetDESCRIPTION: TIBStringField;
    ibdsPay_DetDISC: TIBBCDField;
    ibdsPay_DetEXPIRES: TDateField;
    ibdsPay_DetFULL_NAME: TIBStringField;
    ibdsPay_DetINVOICE_NUMBER: TIBStringField;
    ibdsPay_DetMEMBER: TIntegerField;
    ibdsPay_DetPAID_BY: TIntegerField;
    ibdsPay_DetPAY_DATE: TDateField;
    ibdsPay_DetPAY_FULL: TSmallintField;
    ibdsPay_DetPAY_TYPE: TSmallintField;
    ibdsPay_DetPAYMENT: TIntegerField;
    ibdsPay_DetPAYMENT_DET: TIntegerField;
    ibdsPay_DetSPONSORSHIP: TIBStringField;
    ibdsPAymentsDISCOUNT: TIBBCDField;
    ibdsPAymentsEDIT_DATE: TDateTimeField;
    ibdsPAymentsFULL_NAME: TIBStringField;
    ibdsPAymentsINVOICE_NUMBER: TIBStringField;
    ibdsPAymentsPAID_BY: TIntegerField;
    ibdsPAymentsPAY_AMOUNT: TIBBCDField;
    ibdsPAymentsPAY_CREDIT: TIBBCDField;
    ibdsPAymentsPAYMENT: TIntegerField;
    ibdsPAymentsPOINTS_USED: TIBBCDField;
    ibdsPAymentsROUNDING: TIBBCDField;
    ibdsRollover: TIBDataSet;
    dsrRollover: TDataSource;
    ibdsRolloverACCEPT: TIBStringField;
    ibdsRolloverADDRESS: TIBStringField;
    ibdsRolloverADDRESS1: TIBStringField;
    ibdsRolloverANALYSE: TIBStringField;
    ibdsRolloverCITY: TIBStringField;
    ibdsRolloverCOUNTRY: TIBStringField;
    ibdsRolloverDISTRICT: TIBStringField;
    ibdsRolloverEXPIRES: TDateField;
    ibdsRolloverFIRST_NAME: TIBStringField;
    ibdsRolloverFULL_NAME: TIBStringField;
    ibdsRolloverHOME_PHONE: TIBStringField;
    ibdsRolloverIS_ACTIVE: TIBStringField;
    ibdsRolloverKEEP_EXPIRES_CURRENT: TIBStringField;
    ibdsRolloverMAILING_LIST_CODE: TIntegerField;
    ibdsRolloverMEMBER: TIntegerField;
    ibdsRolloverNEWSLETTER: TIBStringField;
    ibdsRolloverROLL_ID: TIntegerField;
    ibdsRolloverSUB_PAID: TSmallintField;
    ibdsRolloverSURNAME: TIBStringField;
    tblMiscPAYMENT_GRIDS: TIBStringField;
    tblMiscCONFIRM_PAYMENT: TIBStringField;
    tblMiscCARD_ENCODE_MEMBER: TIBStringField;
    qryMembersLookEXPIRES: TDateField;
    tblMiscPAYMENT_CARD: TSmallintField;
    tblMiscPAYMENT_RECEIPT: TSmallintField;
    ibdsQueries: TIBDataSet;
    dsrQueries: TDataSource;
    ibdsQueriesQUERIES_ID: TSmallintField;
    ibdsQueriesNAME: TIBStringField;
    ibdsQueriesCOMMENTS: TIBStringField;
    ibdsQueriesSQL: TBlobField;
    ibdsQueriesCALLER_ID: TSmallintField;
    OpWord: TOpWord;
    tblLuckyMember: TIBTable;
    tblLuckyMemberLUCKY_ID: TIntegerField;
    tblLuckyMemberMEMBER: TIntegerField;
    dsrLuckyMember: TDataSource;
    dsrReport: TDataSource;
    tblSubsON_INVOICE: TIBStringField;
    ibdsMembersSPARE_VARCHAR_50: TIBStringField;
    ibdsMembersDefSPARE_VARCHAR_50: TIBStringField;
    ibdsM_Photos: TIBDataSet;
    ibdsM_PhotosDRAWN: TIBStringField;
    ibdsM_PhotosMEMBER: TIntegerField;
    ibdsM_PhotosPHOTO: TBlobField;
    tblClubs: TIBTable;
    dsrClubs: TDataSource;
    tblClubsCLUB_ID: TSmallintField;
    tblClubsDESCRIPTION: TIBStringField;
    ibdsMembersCLUBS: TIntegerField;
    ibdsMembersASSOCIATION: TIBStringField;
    ibdsMembersDefCLUBS: TIntegerField;
    ibdsMembersDefASSOCIATION: TIBStringField;
    tblPrintersPRINTER_ID: TSmallintField;
    tblPrintersDESCRIPTION: TIBStringField;
    tblPrintersPRINTER_NAME: TIBStringField;
    tblPrintersPRINTER_INDEX: TSmallintField;
    tblPrintersPC_NAME: TIBStringField;
    tblPrintersEMULATION: TSmallintField;
    tblPrintersCOMM_PORT: TSmallintField;
    ibdsAutoPayPRINT_CARD: TIBStringField;
    tblMiscPART_PAY_ADVANCE: TIBStringField;
    tblMiscPOS_PAY_CARD_REQD: TIBStringField;
    tblMiscBOTTOM_LINE_FEED: TSmallintField;
    tblMiscTOP_LINE_FEED: TSmallintField;
    dsrMembersBrief: TDataSource;
    ibdsMembersBriefMEMBER: TIntegerField;
    ibdsMembersBriefADDRESS: TIBStringField;
    ibdsMembersBriefADDRESS1: TIBStringField;
    ibdsMembersBriefCITY: TIBStringField;
    ibdsMembersBriefCOUNTRY: TIBStringField;
    ibdsMembersBriefDATE_OF_BIRTH: TDateField;
    ibdsMembersBriefDISTRICT: TIBStringField;
    ibdsMembersBriefEMAIL: TIBStringField;
    ibdsMembersBriefEXPIRES: TDateField;
    ibdsMembersBriefFIRST_NAME: TIBStringField;
    ibdsMembersBriefFULL_NAME: TIBStringField;
    ibdsMembersBriefHOME_PHONE: TIBStringField;
    ibdsMembersBriefIS_ACTIVE: TIBStringField;
    ibdsMembersBriefJOINED: TDateField;
    ibdsMembersBriefSUB_PAID: TSmallintField;
    ibdsMembersBriefSURNAME: TIBStringField;
    ibdsMembersBriefWORK_PHONE: TIBStringField;
    tblCardMemberCARD_TYPE: TSmallintField;
    qryReport2CLUBS: TIntegerField;
    qryReport2ASSOCIATION: TIBStringField;
    qryReport2PAYMENT_DAY: TSmallintField;
    qryReport2FAX_WORK: TIBStringField;
    qryReport2STREET_ADDRESS: TIBStringField;
    qryReport2STREET_DISTRICT: TIBStringField;
    qryReport2STREET_ADDRESS1: TIBStringField;
    qryReport2STREET_CITY: TIBStringField;
    qryReport2STREET_COUNTRY: TIBStringField;
    tblMiscPAY_BEFORE_EXPIRES: TIBStringField;
    tblMiscCARD_DEFAULT: TIBStringField;
    tblMiscRECEIPT_DEFAULT: TIBStringField;
    tblMiscEXPORT_SUBS: TIBStringField;
    ibdsMembersVALUE_OF_CREDIT: TFloatField;
    ibdsPointsLog: TIBDataSet;
    dsrPointsLog: TDataSource;
    dsrMemberPoints: TDataSource;
    ibdsMemberPoints: TIBDataSet;
    ibdsMemberPointsMEMBER: TIntegerField;
    qryPointsLog: TIBQuery;
    dsrQryPointsLog: TDataSource;
    qryPointsLogPOINTS_ID: TIntegerField;
    qryPointsLogPROCESS: TSmallintField;
    qryPointsLogREASON: TIBStringField;
    qryPointsLogTOTAL_EARNT: TFloatField;
    qryPointsLogTOTAL_REDEEMED: TFloatField;
    qryPointsLogEARNT_CREDIT: TFloatField;
    qryPointsLogBONUS_CREDIT: TFloatField;
    qryPointsLogPRE_PAID_CREDIT: TFloatField;
    qryPointsLogPOINTS_VALUE: TFloatField;
    qryPointsLogMEMBER: TIntegerField;
    qryPointsLogEDIT_DATE: TDateTimeField;
    qryPointsLogBEF_TOTAL_EARNT: TFloatField;
    qryPointsLogBEF_TOTAL_REDEEMED: TFloatField;
    qryPointsLogBEF_EARNT_CREDIT: TFloatField;
    qryPointsLogBEF_BONUS_CREDIT: TFloatField;
    qryPointsLogBEF_PRE_PAID_CREDIT: TFloatField;
    qryPointsLogUSER_CODE: TIntegerField;
    qryPointsLogVALUE_OF_CREDIT: TFloatField;
    qryPointsLogBEF_VALUE_OF_CREDIT: TFloatField;
    ibdsPointsLogPOINTS_ID: TIntegerField;
    ibdsPointsLogPROCESS: TSmallintField;
    ibdsPointsLogREASON: TIBStringField;
    ibdsPointsLogTOTAL_EARNT: TFloatField;
    ibdsPointsLogTOTAL_REDEEMED: TFloatField;
    ibdsPointsLogEARNT_CREDIT: TFloatField;
    ibdsPointsLogBONUS_CREDIT: TFloatField;
    ibdsPointsLogPRE_PAID_CREDIT: TFloatField;
    ibdsPointsLogPOINTS_VALUE: TFloatField;
    ibdsPointsLogMEMBER: TIntegerField;
    ibdsPointsLogEDIT_DATE: TDateTimeField;
    ibdsPointsLogBEF_TOTAL_EARNT: TFloatField;
    ibdsPointsLogBEF_TOTAL_REDEEMED: TFloatField;
    ibdsPointsLogBEF_EARNT_CREDIT: TFloatField;
    ibdsPointsLogBEF_BONUS_CREDIT: TFloatField;
    ibdsPointsLogBEF_PRE_PAID_CREDIT: TFloatField;
    ibdsPointsLogUSER_CODE: TIntegerField;
    ibdsPointsVALUE_OF_CREDIT: TFloatField;
    MainMenu1: TMainMenu;
    ibdsPointsREASON: TIBStringField;
    ibdsExportRESET_POINTS: TIBStringField;
    ibdsPayMembersPOINTS_AVAILABLE: TFloatField;
    ibdsPayMembersTOTAL_EARNT: TFloatField;
    ibdsPayMembersBONUS_CREDIT: TFloatField;
    ibdsPayMembersPRE_PAID_CREDIT: TFloatField;
    ibdsPayMembersEARNT_CREDIT: TFloatField;
    ibdsPayMembersTOTAL_REDEEMED: TFloatField;
    ibdsMembersBONUS_CREDIT: TFloatField;
    ibdsMembersEARNT_CREDIT: TFloatField;
    ibdsMembersPRE_PAID_CREDIT: TFloatField;
    ibdsMembersTOTAL_EARNT: TFloatField;
    ibdsMembersTOTAL_REDEEMED: TFloatField;
    qryReport2PAYMENT_DISC: TIBBCDField;
    qryReport2BONUS_CREDIT: TFloatField;
    qryReport2EARNT_CREDIT: TFloatField;
    qryReport2PRE_PAID_CREDIT: TFloatField;
    qryReport2TOTAL_EARNT: TFloatField;
    qryReport2TOTAL_REDEEMED: TFloatField;
    qryReport2REASON: TIBStringField;
    ibdsPayDetDATE_OF_BIRTH: TDateField;
    ibdsPayDetADDRESS: TIBStringField;
    ibdsPayDetADDRESS1: TIBStringField;
    ibdsPayDetDISTRICT: TIBStringField;
    ibdsPayDetCITY: TIBStringField;
    ibdsPayDetCOUNTRY: TIBStringField;
    ibdsMembersDefBONUS_CREDIT: TFloatField;
    ibdsMembersDefEARNT_CREDIT: TFloatField;
    ibdsMembersDefPRE_PAID_CREDIT: TFloatField;
    ibdsMembersDefVALUE_OF_CREDIT: TFloatField;
    ibdsMembersDefPOINTS_AVAILABLE: TFloatField;
    ibdsMembersDefTOTAL_EARNT: TFloatField;
    ibdsMembersDefTOTAL_REDEEMED: TFloatField;
    ibdsPointsTOTAL_REDEEMED: TFloatField;
    ibdsPointsTOTAL_EARNT: TFloatField;
    ibdsPointsEARNT_CREDIT: TFloatField;
    ibdsPointsBONUS_CREDIT: TFloatField;
    ibdsPointsPRE_PAID_CREDIT: TFloatField;
    tblSubsTERM: TSmallintField;
    ibdsPayDetJOINED: TDateField;
    ibdsMemberPointsEARNT_CREDIT: TFloatField;
    ibdsMemberPointsBONUS_CREDIT: TFloatField;
    ibdsMemberPointsPOSITIONS: TIBStringField;
    ibdsMemberPointsTOTAL_EARNT: TFloatField;
    ibdsMemberPointsTOTAL_REDEEMED: TFloatField;
    ibdsMemberPointsPRE_PAID_CREDIT: TFloatField;
    ibdsMemberPointsCARD_LAST_USED: TDateField;
    ibdsMembersREASON: TIBStringField;
    ibdsMembersDefREASON: TIBStringField;
    ibdsMailingLabels: TIBDataSet;
    dsrMailingLabels: TDataSource;
    ibdsMailingLabelsRIGHT_MARGIN: TIBBCDField;
    ibdsMailingLabelsTOP_MARGIN: TIBBCDField;
    ibdsMailingLabelsBOTTOM_MARGIN: TIBBCDField;
    ibdsMailingLabelsCOLUMN_SPACE: TIBBCDField;
    ibdsMailingLabelsNUMBER_COLUMNS: TSmallintField;
    ibdsMailingLabelsMISC: TSmallintField;
    ibdsMailingLabelsLEFT_MARGIN: TIBBCDField;
    ibdsMailingLabelsBAND_HEIGHT: TIBBCDField;
    ibdsPayDetSPARE_CHAR: TIBStringField;
    cdsTemp: TClientDataSet;
    ibdsMemberDelete: TIBDataSet;
    tblMiscEXPORT_TIMER: TIntegerField;
    qryPointsLogBEF_POINTS_AVAILABLE: TFloatField;
    qryPointsLogPOINTS_AVAILABLE: TFloatField;
    ibdsPaySecMemPOINTS_AVAILABLE: TFloatField;
    ibdsPaySecMemTOTAL_REDEEMED: TFloatField;
    ibdsMembersPOINTS_AVAILABLE: TFloatField;
    ibdsPointsLogBEF_POINTS_AVAILABLE: TFloatField;
    ibdsPointsLogPOINTS_AVAILABLE: TFloatField;
    ibdsPointsPOINTS_AVAILABLE: TFloatField;
    ibdsMemberPointsPOINTS_AVAILABLE: TFloatField;
    tblMiscPRINT_MEMBER_PAYMENT: TIBStringField;
    ibdsMembersSWIPE_CARD: TIBStringField;
    ibdsMembersDefSWIPE_CARD: TIBStringField;
    tblKiosk: TIBTable;
    dsrKiosk: TDataSource;
    ibdsMembersVISITS: TSmallintField;
    ibdsMembersLAST_VISIT: TDateTimeField;
    ibdsMembersDefVISITS: TSmallintField;
    ibdsMembersDefLAST_VISIT: TDateTimeField;
    tblMiscAREV_FILE_PATH: TIBStringField;
    tblSectionsDONATION: TIBBCDField;
    ibdsMembersDONATION: TIBBCDField;
    ibdsMembersDONATION_MEMBER: TIntegerField;
    ibdsMembersDefDONATION: TIBBCDField;
    ibdsMembersDefDONATION_MEMBER: TIntegerField;
    ibdsPointsAllocation: TIBDataSet;
    dsrPointsAllocation: TDataSource;
    ibdsPointsAlloc: TIBDataSet;
    ibdsPointsAllocMEMBER: TIntegerField;
    ibdsPointsAllocPOINTS_AVAILABLE: TFloatField;
    ibdsPointsAllocationDATE_TIME: TDateTimeField;
    ibdsPointsAllocationDONATION_MEMBER: TIntegerField;
    ibdsPointsAllocationDONATION_POINTS: TIBBCDField;
    ibdsPointsAllocationLAST_DONATED_POINTS_AVAILABLE: TIBBCDField;
    ibdsPointsAllocationLAST_POINTS_AVAILABLE: TIBBCDField;
    ibdsPointsAllocationMEMBER: TIntegerField;
    ibdsPointsAllocationPOINTS: TIBBCDField;
    ibdsPointsAllocationPOINTS_SPLIT_ID: TIntegerField;
    ibdsPointsAllocationUPDATED: TIBStringField;
    ibdsPointsAllocationFULL_NAME: TIBStringField;
    tblSubsEXPIRES_CARD: TIBStringField;
    tblSubsEXPIRES_DESC: TIBStringField;
    tblMiscPHM_INSTALLED: TIBStringField;
    tblMiscPHM_PATH: TIBStringField;
    tblMiscPHM_CONTROLLER: TIBStringField;
    PHMExportTimer: TTimer;
    ibdsPHMPoints: TIBDataSet;
    ibdsPHMPointsMEMBER: TIntegerField;
    ibdsPHMPointsPOINTS_AVAILABLE: TFloatField;
    ibdsPHMPointsTOTAL_EARNT: TFloatField;
    ibdsPHMPointsTOTAL_REDEEMED: TFloatField;
    ibdsPHMPointsCARD_LAST_USED: TDateField;
    tblKioskKIOSK_ID: TSmallintField;
    tblKioskMESSAGE: TIBStringField;
    dsrMenuMate: TDataSource;
    tblMenuMate: TIBTable;
    tblMenuMateMENUMATE_INSTALLED: TIBStringField;
    tblMenuMateTRIGGER_INSTALLED: TIBStringField;
    tblMenuMateSWIPE_TEXT: TIBStringField;
    tblMenuMateON_INSERT: TIBStringField;
    tblMiscKIOSK_MSG: TBlobField;
    tblMiscMONDAY_PRINT: TIBStringField;
    tblMiscTUESDAY_PRINT: TIBStringField;
    tblMiscWEDNESDAY_PRINT: TIBStringField;
    tblMiscTHURSDAY_PRINT: TIBStringField;
    tblMiscFRIDAY_PRINT: TIBStringField;
    tblMiscSATURDAY_PRINT: TIBStringField;
    tblMiscSUNDAY_PRINT: TIBStringField;
    tblMiscBIRTHDAY_TEXT: TIBStringField;
    tblMiscLUCKY_DRAW_TEXT: TIBStringField;
    ibdsMembersBrief: TIBDataSet;
    tblMiscBIRTHDAY_PRINT: TIBStringField;
    ibdsMenuMate: TIBDataSet;
    ibdsMenuMateMEMBER: TIntegerField;
    ibdsMenuMateSWIPE_CARD: TIBStringField;
    ibdsMenuMateEARN_POINTS: TIBStringField;
    tblMiscMONDAY_END_TIME: TTimeField;
    tblMiscTUESDAY_END_TIME: TTimeField;
    tblMiscWEDNESDAY_END_TIME: TTimeField;
    tblMiscTHURSDAY_END_TIME: TTimeField;
    tblMiscFRIDAY_END_TIME: TTimeField;
    tblMiscSATURDAY_END_TIME: TTimeField;
    tblMiscSUNDAY_END_TIME: TTimeField;
    tblMiscVISIT_MSG: TIBStringField;
    tblMiscBIRTHDAY_MSG: TIBStringField;
    tblMiscLUCKY_DRAW_MSG: TIBStringField;
    ibdsPrefDetail: TIBDataSet;
    tblMiscPARENT_CODE_ADDRESS: TIBStringField;
    tblMiscGROUP_DELETE: TIBStringField;
    ibdsAddress: TIBDataSet;
    ibdsAddressADDRESS: TIBStringField;
    ibdsAddressADDRESS1: TIBStringField;
    ibdsAddressDISTRICT: TIBStringField;
    ibdsAddressCITY: TIBStringField;
    ibdsAddressCOUNTRY: TIBStringField;
    ibdsAddressMAILING_LIST_CODE: TIntegerField;
    ibdsAddressMEMBER: TIntegerField;
    tblMiscKNOWN_AS_DEFAULTED: TIBStringField;
    tblGroupsIN_REPORT: TIBStringField;
    ibdsAddressFULL_NAME: TIBStringField;
    tblKioskDraws: TIBTable;
    dsrKioskDraws: TDataSource;
    tblKioskDrawsKIOSK_ID: TSmallintField;
    tblKioskDrawsKIOSK_DESC: TIBStringField;
    tblMiscKIOSK_DRAWS: TSmallintField;
    tblMiscKIOSK_DRAW_DATE: TDateField;
    ibdsMenuMatePoints: TIBDataSet;
    ibdsMenuMatePointsMEMBER: TIntegerField;
    ibdsMenuMatePointsPOINTS_AVAILABLE: TFloatField;
    ibdsMenuMatePointsPOINTS_DIFFERENCE: TFloatField;
    ibdsMenuMatePointsTHE_DATE: TDateField;
    tblMiscKIOSK_SUCCESS: TIBStringField;
    tblMiscKIOSK_ERROR_MSG: TIBStringField;
    ibdsPrefHeading: TIBDataSet;
    ibdsPrefSubHeading: TIBDataSet;
    dsrPrefHeading: TDataSource;
    dsrPrefDetail: TDataSource;
    dsrPrefSubHeading: TDataSource;
    ibdsPrefSubHeadingDISPLAY_HEADING: TIBStringField;
    ibdsPrefSubHeadingHEADING_ID: TIntegerField;
    ibdsPrefSubHeadingSUB_HEADING_DESC: TIBStringField;
    ibdsPrefHeadingHEADING_DESC: TIBStringField;
    ibdsPrefHeadingHEADING_ID: TIntegerField;
    ibdsPrefDetailDETAIL_DESC: TIBStringField;
    ibdsPrefDetailDETAIL_TYPE: TIBStringField;
    ibdsPrefDetailHEADING_ID: TIntegerField;
    ibdsPrefDetailLINE_NUMBER: TSmallintField;
    ibdsPrefDetailFIELD_DESC: TIBStringField;
    tblPrefMembers: TIBTable;
    ibdsPrefDetailDETAIL_ID: TIntegerField;
    ibdsPrefSubHeadingSUB_HEADING_ID: TIntegerField;
    dsrPrefMembers: TDataSource;
    ibdsPrefDetailDISPLAY_DETAIL: TIBStringField;
    tblMiscPRINT_RECEIPT_AS_TEXT: TIBStringField;
    tblMenuMateSWIPE_LENGTH: TSmallintField;
    ibdsMenuMateFULL_NAME: TIBStringField;
    ibdsMenuMateALLOW_CREDIT: TIBStringField;
    ibdsPayMemSecSECTION_EXPIRES: TDateField;
    ibdsPayMemSecMIN_FEE: TIBBCDField;
    tblSectionsMIN_FEE: TIBBCDField;
    ibdsMemberPointsFULL_NAME: TIBStringField;
    ibdsPointsLogFULL_NAME: TIBStringField;
    dsrKioskSettings: TDataSource;
    tblKioskSettings: TIBTable;
    tblKioskSettingsSHOW_DICE: TIBStringField;
    tblKioskSettingsKIOSK_TIMER: TSmallintField;
    tblKioskSettingsCHECK_EXPIRES: TIBStringField;
    tblKioskSettingsPOINTS_AS_DOLLARS: TIBStringField;
    tblKioskSettingsKIOSK_MEMBER_START: TSmallintField;
    tblKioskSettingsKIOSK_MEMBER_LENGHT: TSmallintField;
    tblMiscVIP_CREDIT_NAME: TIBStringField;
    ibdsMemberDeleteMEMBER: TIntegerField;
    ibdsMemberDeletePOINTS_AVAILABLE: TFloatField;
    ibdsMemberDeleteEARNT_CREDIT: TFloatField;
    ibdsMemberDeleteTOTAL_EARNT: TFloatField;
    ibdsMemberDeleteTOTAL_REDEEMED: TFloatField;
    ibdsMemberDeletePRE_PAID_CREDIT: TFloatField;
    ibdsMemberDeleteBONUS_CREDIT: TFloatField;
    ibdsPointsAudit: TIBDataSet;
    tblMenuMateVIP_CREDIT_NAME: TIBStringField;
    ibdsMembersPROPOSER_NAME: TIBStringField;
    ibdsMembersSECONDER_NAME: TIBStringField;
    ibdsMembersDefPROPOSER_NAME: TIBStringField;
    ibdsMembersDefSECONDER_NAME: TIBStringField;
    ibdsMemberDeleteFULL_NAME: TIBStringField;
    ibdsAutoPayMAILING_LIST_CODE: TIntegerField;
    qryMenuMatePoints: TIBQuery;
    dsrMenuMatePoints: TDataSource;
    qryMenuMatePointsMEMBERS_POINTS_KEY: TIntegerField;
    qryMenuMatePointsMEMBER: TIntegerField;
    qryMenuMatePointsEARNED: TIBBCDField;
    qryMenuMatePointsREDEEMED: TIBBCDField;
    qryMenuMatePointsADJUSTMENT_TYPE: TIntegerField;
    qryMenuMatePointsPOINTS_TYPE: TIntegerField;
    qryMenuMatePointsTIME_STAMP: TDateTimeField;
    qryMenuMatePointsINVOICE_NUMBER: TIBStringField;
    qryMenuMatePointsFULL_NAME: TIBStringField;
    tblMiscCHECK_SUBS: TIBStringField;
    tblMiscCHECK_GROUPS: TIBStringField;
    tblMiscMAX_SUB: TSmallintField;
    tblMiscMAX_GROUP: TSmallintField;
    tblMiscMIN_SUB: TSmallintField;
    tblMiscMIN_GROUP: TSmallintField;
    tblMiscCLEAR_INTERESTS: TIBStringField;
    ibdsM_Discounts: TIBDataSet;
    dsrM_Discounts: TDataSource;
    ibdsM_DiscountsDISCOUNT_KEY: TIntegerField;
    ibdsM_DiscountsMEMBER: TIntegerField;
    ibdsM_DiscountsMEMBER_DISCOUNTS_KEY: TIntegerField;
    ibdsM_DiscountsNAME: TStringField;
    ibdsM_DiscountsDESCRIPTION: TStringField;
    ibdsM_DiscountsMENU: TStringField;
    ibdsM_DiscountsAMOUNT: TCurrencyField;
    ibdsM_DiscountsPERCENT: TFloatField;
    tblMenuMateDISCOUNTS_INSTALLED: TIBStringField;
    ibdsPointsAllocBONUS_CREDIT: TFloatField;
    ibdsPointsAllocTOTAL_EARNT: TFloatField;
    procedure DataModuleCreate(Sender: TObject);
    procedure dsrMembersStateChange(Sender: TObject);
    procedure ibdsMembersAfterInsert(DataSet: TDataSet);
    procedure tblGroupsAfterInsert(DataSet: TDataSet);
    procedure tblSubsAfterInsert(DataSet: TDataSet);
    procedure tblSectionsAfterInsert(DataSet: TDataSet);
    procedure tblEventsAfterInsert(DataSet: TDataSet);
    procedure tblSubsBeforeInsert(DataSet: TDataSet);
    procedure tblSubsBeforeDelete(DataSet: TDataSet);
    procedure tblSubsAfterPost(DataSet: TDataSet);
    procedure tblSectionsBeforeInsert(DataSet: TDataSet);
    procedure tblSectionsBeforeDelete(DataSet: TDataSet);
    procedure tblEventsBeforeInsert(DataSet: TDataSet);
    procedure tblEventsBeforeDelete(DataSet: TDataSet);
    procedure tblGroupsBeforeInsert(DataSet: TDataSet);
    procedure tblGroupsBeforeDelete(DataSet: TDataSet);
    procedure ibdsM_SectionsAfterInsert(DataSet: TDataSet);
    procedure ibdsM_SubsAfterInsert(DataSet: TDataSet);
    procedure ibdsM_EventsAfterInsert(DataSet: TDataSet);
    procedure ibdsMembersAfterCancel(DataSet: TDataSet);
    procedure dsrTblGroupsStateChange(Sender: TObject);
    procedure dsrTblSubsStateChange(Sender: TObject);
    procedure dsrTblSectionsStateChange(Sender: TObject);
    procedure tblGroupsPostError(DataSet: TDataSet; E: EDatabaseError; var Action: TDataAction);
    procedure tblSubsPostError(DataSet: TDataSet; E: EDatabaseError;   var Action: TDataAction);
    procedure tblSectionsPostError(DataSet: TDataSet; E: EDatabaseError; var Action: TDataAction);
    procedure tblEventsPostError(DataSet: TDataSet; E: EDatabaseError; var Action: TDataAction);
    procedure ibdsMembersBeforePost(DataSet: TDataSet);
    procedure PHMExportTimerTimer(Sender: TObject);
    procedure tblSubsBeforePost(DataSet: TDataSet);
    procedure ibdsPaySectionsBeforePost(DataSet: TDataSet);
    procedure ibdsPaySectionsAfterPost(DataSet: TDataSet);
    procedure ibdsPaySectionsAfterEdit(DataSet: TDataSet);
    procedure ibdsMembersBeforeDelete(DataSet: TDataSet);
    procedure ibdsM_EventsBeforePost(DataSet: TDataSet);
    procedure tblEventsBeforePost(DataSet: TDataSet);
    procedure MsgBeforePostTBL( FromTable : TIBTable ; tblMsg : string);
    procedure MsgBeforePostDS ( FromDS : TIBDataSet ; tblMsg : string);
    procedure ibdsMembersBeforeEdit(DataSet: TDataSet);
    procedure tblTendersAfterPost(DataSet: TDataSet);
    procedure dsrLettersStateChange(Sender: TObject);
    procedure ibdsLettersAfterPost(DataSet: TDataSet);
    procedure ibdsLettersBeforePost(DataSet: TDataSet);
    procedure ibdsM_SubsBeforeDelete(DataSet: TDataSet);
    procedure ibdsM_EventsBeforeDelete(DataSet: TDataSet);
    procedure IBDatabaseAfterConnect(Sender: TObject);
    function  GetUserNames : Boolean;
    procedure ibdsM_EventsBeforeEdit(DataSet: TDataSet);
    procedure ibdsMembersBeforeInsert(DataSet: TDataSet);
    function  GetCount ( tbl : shortstring ) : Integer;
    procedure SetMemberNumber( NextMember : LongInt);
    procedure ibdsMembersPostError(DataSet: TDataSet; E: EDatabaseError; var Action: TDataAction);
    procedure ibdsMembersAfterPost(DataSet: TDataSet);
    procedure ibdsM_SubsPostError(DataSet: TDataSet; E: EDatabaseError;  var Action: TDataAction);
    procedure ibdsM_SectionsPostError(DataSet: TDataSet; E: EDatabaseError; var Action: TDataAction);
    procedure ibdsM_EventsPostError(DataSet: TDataSet; E: EDatabaseError; var Action: TDataAction);
    procedure ResetMemberNumber;
    procedure ibdsM_SubsAfterPost(DataSet: TDataSet);
    procedure GetNextNumber;
    function GetBlankNumber : Boolean;
    //procedure MemberInsertChange;
    procedure dsrTblEventsStateChange(Sender: TObject);
    procedure dsrTblMailCodesStateChange(Sender: TObject);
    procedure tblMailCodesPostError(DataSet: TDataSet; E: EDatabaseError; var Action: TDataAction);
    procedure ibdsMembersDefBeforePost(DataSet: TDataSet);
    procedure ibdsMembersDefAfterPost(DataSet: TDataSet);
    procedure ibdsMembersDefBeforeDelete(DataSet: TDataSet);
    procedure ibdsMembersDefAfterCancel(DataSet: TDataSet);
    procedure ResetMemberDefault;
    procedure IBEventsEventAlert(Sender: TObject; EventName: String; EventCount: Integer; var CancelAlerts: Boolean);
    procedure SendEvent( strMsg : shortstring );
    procedure tblScreensAfterPost(DataSet: TDataSet);
    function sqlExecQuery( theSQL : TIBSql ; sqlString : String ) : Boolean;
    function sqlExecQueryCheckEOF( theSQL : TIBSql ; sqlString : String ) : Boolean;
    function sqlExecQueryCheckEOFCount( theSQL : TIBSql ; sqlString : String ) : Boolean;
    function qryOpen( theQry : TIBQuery ; sqlString : String ) : Boolean;
    function qryOpenCheckEOF( theQry : TIBQuery ; sqlString : String ) : Boolean;
    function ibdsOpenCheckEOF( theDataSet : TIBDataSet ; sqlString : String ) : Boolean;
    
    procedure tblNumbersAfterInsert(DataSet: TDataSet);
    procedure tblVersionsAfterPost(DataSet: TDataSet);
    procedure dsrTblTendersStateChange(Sender: TObject);
    procedure dsrTblNumbersStateChange(Sender: TObject);
    function LogExport ( theSQL : TIBSQL ; LogMember : LongInt ; PointsUsed : Integer; ResetPoints : Boolean ) : Boolean;
    procedure ExportTimerTimer(Sender: TObject);
    procedure tblCardMasterBeforeInsert(DataSet: TDataSet);
    procedure tblCardMasterAfterInsert(DataSet: TDataSet);
    procedure tblCardDetailAfterInsert(DataSet: TDataSet);
    procedure tblCardMasterBeforeDelete(DataSet: TDataSet);
    procedure tblCardMasterBeforePost(DataSet: TDataSet);
    procedure tblCardDetailBeforeDelete(DataSet: TDataSet);
    procedure DeleteReportTables;
    procedure ibdsM_SubsBeforeInsert(DataSet: TDataSet);
    procedure dsrCardDetailStateChange(Sender: TObject);
    procedure dsrCardMasterStateChange(Sender: TObject);
    procedure dsrCardMemberStateChange(Sender: TObject);
    procedure tblCardMemberPostError(DataSet: TDataSet; E: EDatabaseError;
      var Action: TDataAction);
    procedure ibdsM_GroupsAfterInsert(DataSet: TDataSet);
    procedure ibdsM_LotsAfterInsert(DataSet: TDataSet);
    procedure DataModuleDestroy(Sender: TObject);
    procedure ibdsM_InterestsAfterInsert(DataSet: TDataSet);
    procedure ibdsQueriesBeforeInsert(DataSet: TDataSet);
    procedure ibdsQueriesAfterInsert(DataSet: TDataSet);
    procedure dsrStateChange( theDataSource : TDataSource; theForm : TSMStdForm );
    procedure ibdsMembersAfterEdit(DataSet: TDataSet);
    procedure ibdsM_GroupsBeforeDelete(DataSet: TDataSet);
    procedure tblClubsAfterInsert(DataSet: TDataSet);
    procedure ibdsMembersBriefAfterPost(DataSet: TDataSet);
    procedure dsrMembersBriefStateChange(Sender: TObject);
    procedure tblCardMemberAfterInsert(DataSet: TDataSet);
    procedure ibdsMembersCalcFields(DataSet: TDataSet);
    procedure qryPointsLogCalcFields(DataSet: TDataSet);
    procedure ibdsPointsCalcFields(DataSet: TDataSet);
    procedure ibdsMailingLabelsAfterPost(DataSet: TDataSet);
    procedure ibdsPointsAllocationBeforeDelete(DataSet: TDataSet);
    procedure ibdsPointsAllocationBeforeInsert(DataSet: TDataSet);
    function TriggerState ( theState : Boolean ): Boolean;
    procedure ibdsMembersBeforeCancel(DataSet: TDataSet);
    procedure tblMiscBeforePost(DataSet: TDataSet);
    procedure tblKioskDrawsAfterInsert(DataSet: TDataSet);
    //procedure tmrPointsTimer(Sender: TObject);
    //procedure UpdateMenuMatePoints;
    procedure ibdsPrefHeadingAfterInsert(DataSet: TDataSet);
    procedure ibdsPrefSubHeadingAfterInsert(DataSet: TDataSet);
    procedure ibdsPrefSubHeadingAfterPost(DataSet: TDataSet);
    procedure ibdsPrefDetailBeforePost(DataSet: TDataSet);
    function CheckForStopFiles(const ArevStopFile : String; const ExportNumberOfFiles : ShortInt): Boolean;
    procedure DeleteExportFiles(const WinStopFile,WinMemberFile,Msg,TmpWinMemberFile : String);
    function PreFillMember(var Member : String) : String;
    function InsertMemberFullName(theMember,FullName : String) : String;
    procedure ibdsPayMembersAfterOpen(DataSet: TDataSet);
    function CheckGroupsSubs : Boolean;
    procedure CheckSubs( var ResultGroupsSubs : Boolean );
    procedure CheckGroups( var ResultGroupsSubs : Boolean );
    procedure ibdsM_DiscountsAfterInsert(DataSet: TDataSet);
              
    
  private
    { Private declarations }
    FVariant,FVariant1  : Variant; // used to pass integers for the Locate function in IBTables
    FSubscription,FSection,FEvent,FGroups,FCardMaster,FQueries : Integer; // holds Subscription last + 1 for adding into the table
    FBeforeEditFullName   : String[40];
    FBeforeEditAddress,FBeforePostAddress    : String;

    FBeforeEditFinancial,FBeforeEditPriceLevel,FBeforeEditVisits : ShortInt;
    FBeforeEditVIPPayOnly,FBeforeEditAllowCredit,FBeforeEditEarnPoints,FBeforeEditCardType  : String[1];
    FBeforeEditIS_Active,FBeforeEditAutoPayment, FPay,FBeforeEditIncludeEOD  : string[1];
    FBeforeEditGLAccount,FBeforeEditKnown_As   : String[15];
    FBeforeEditCreditLimit,FBeforeEditDiscount : Extended;
    FBeforeEditTotalEarnt,FBeforeEditTotalRedeemed,FBeforeEditPointsAvailable : Extended;

    FSectionReceipt,FReceipt,FNextMember,FBlankNumber  : LongInt;
    FSentFromHere,FAutoPaymentNoCommit,FInsertingMember,FRemoveHomeClub : Boolean;
    FArevStopFileCtr : SmallInt;
    FKeepPaymentDate,FBeforeEditDOB,FBeforeEditDOBEvent,FBeforeEditExpires : TDateTime;
    FCustomDataSetList,FActiveCustomDataSetList : TStringList;
  public
    { Public declarations }
     Msg,MsgActive : String[100];
     MChg,MAdd,MDel : String[8];
     GroupsPKViolationRetry,SubsPKViolationRetry,SectionsPKViolationRetry,EventsPKViolationRetry : ShortInt;
     ProcessType{,WinMemberFileCtr} : SmallInt;
     PExporting : Boolean;
     //members
     procedure MembersDetailsPopulateGrid( var EventCtr,SectionCtr,SubscriptionCtr,GroupCtr : Integer );
     procedure SetMembersDefaults;
     function DeleteMemberLinks(Member : LongInt ; theIBSQL : TIBSQL) : Boolean;
     procedure ResetMembersDef;
     
     // general
     function  LastNumberPlusOne( TableName,SeqField : string) : LongInt;
     procedure DeleteLog( tblName,tblField : shortstring; removedays : smallint);
     procedure AfterSettings;
     procedure StateChange( Tbl : TIBTable);

     // logging in and out
     function GetSiteDetails : Boolean;
     function GetMenuMateDetails : Boolean;
     function LoggedOnUsers : ShortInt;

     // system
     function  PreCheckLogOn( ScreenCode : integer ; ScreenDesc : string ) : Boolean;
     procedure IBCommitRetain;
     procedure IBTransactionCommit;
     //procedure IBTransactionCommitCount;
     procedure IBRollBackRetain;
     procedure LogProcess ( theSQL : TIBSql; Msg : String );
     procedure StartTrans ( theTrans : TIBTransaction );
     procedure IBRollBackStart ( theTrans : TIBTransaction );
  end;                                             

var
  dm: Tdm;

const
{ **************************************************************************** }
   { Table Names }
   MembersTableName    =   'MEMBERS';

   MEventsTableName   = 'M_EVENTS';
   MSectionsTableName = 'M_SECTIONS';
   MSubsTableName     = 'M_SUBS';
   NumbersTableName   = 'M_NUMBERS';
   MGroupsTableName   = 'M_GROUPS';
   MInterestsTableName   = 'M_INTERESTS';
   MLotsTableName     = 'LOTS';
   MPhotosTableName   = 'M_PHOTOS';

   SubsTableName =      'SUBSCRIPTIONS';
   SectionsTableName =  'SECTIONS';
   EventsTableName =    'EVENTS';
   GroupsTableName =    'GROUPS';

   MiscTableName   =          'MISC';
   SiteTableName   =          'SITE';
   ScreensTableName   =       'SCREENS';
   PrintersTableName     =    'PRINTERS';
   //UserLogTableName      =    'USER_LOG';
   ExportArevTableName   = 'EXPORT_AREV';
   UserSecurityTableName =    'USER_SEC';
   PrcssLogTableName     =    'PRCSS_LOG';
   BackupLogTableName    =    'BACKUP_LOG';
   LettersTableName      =    'LETTERS';
   QueriesTableName      =    'QUERIES';

{ **************************************************************************** }
   { Members Field Names and Indexes }
   Member       = 'MEMBER';
   Surname      = 'SURNAME';
   FullName     = 'FULL_NAME';
   FirstName    = 'FIRST_NAME';
   MembersKey   = 'PK_MEMBER';
   SurnameKey   = 'IX_SURNAME';

{ **************************************************************************** }
implementation

{$R *.DFM}
uses
    MembersEDIT, //StockMaster
    Global,
    MembersPAY,
    MembersMAINT, 
    MembersCLASSES, 
    MembersLetter, MembersSEARCH, MembersEDIT_BRIEF, dmMenuMate;     //StockMaster
var
  ExportTypes : TExportTypes;
  BeforeEditPointsCredit : TBeforeEditPointsCredit;
{ **************************************************************************** }
procedure Tdm.DataModuleCreate(Sender: TObject);
var i : smallint;
begin
     //FUserCode := 1; // The default User if Log_In is not used
     MChg := ' Changed';
     MAdd := ' Added';
     MDel := ' Deleted';
     Msg  := '';
     MsgActive     := '';
     FSentFromHere := False;

     FBeforeEditKnown_As    := '';
     FBeforeEditIS_Active   := '';
     FBeforeEditFullName    := '';
     FBeforeEditFinancial   := 0;
     FBeforeEditPriceLevel  := 0;
     FBeforeEditVisits      := 0;
     FBeforeEditVIPPayOnly  := '';
     FBeforeEditAllowCredit := '';
     FBeforeEditEarnPoints  := '';
     FBeforeEditCardType    := '';
     FBeforeEditGLAccount   := '';
     FBeforeEditCreditLimit := 0.00;
     FBeforeEditDiscount    := 0.00; 
     FBeforeEditTotalEarnt  := 0.00;
     FBeforeEditTotalRedeemed := 0.00;
     FBeforeEditPointsAvailable := 0.00;

     FBeforeEditIncludeEOD  := '';
     FAutoPaymentNoCommit := False;

     // this PC handles the exporting if it's the same PC name
     PExporting := False;
     FArevStopFileCtr  := 0;
     //WinMemberFileCtr := 0;
     FInsertingMember := False;
     FActiveCustomDataSetList := TStringList.Create;
     FCustomDataSetList := TStringList.Create;
     // add all datasets that have IBTranasaction as its Transaction
     for i := 0 to ComponentCount -1 do begin
        if dm.Components[i].inheritsFrom(TIBCustomDataSet) then begin
           if TIBCustomDataSet(dm.Components[i]).transaction.name = IBTransaction.Name  then
              FCustomDataSetList.AddObject(TIBCustomDataSet(dm.Components[i]).Name, TObject(dm.Components[i]));
        end;
     end;
     // initialize uses
     for i := 0 to 30 do begin
        Global.arrayUserNames[i] := '';
        Global.arrayUserCodes[i] := 0;
     end;
     //Global.SysInfoColorPalette := SysInfo.ColorDepth;
end;
{ **************************************************************************** }
procedure Tdm.IBDatabaseAfterConnect(Sender: TObject);
begin
     IBTransaction.Active := True;
     //IBTransaction.StartTransaction;
end;
{ **************************************************************************** }
procedure Tdm.IBCommitRetain;
// test inTransaction and commit the changes whilst retaining the connections
begin
     if IBTransaction.InTransaction then begin
        //try
           IBTransaction.CommitRetaining;
        //except
           //
        //end;   
     end;
end;
{ **************************************************************************** }
function Tdm.TriggerState ( theState : Boolean ): Boolean;
// Sets the Trigger State Active or Inactive
begin
     Result := False;
     
     with IBRunOnce do begin
        try
           if not InTransaction then 
              StartTransaction;
           case theState of   
              true :  dm.sqlExecQuery(dm.sqlRunOnce,'alter trigger "POINTS_UPDATE_TRIGGER" ACTIVE' );
              false : dm.sqlExecQuery(dm.sqlRunOnce,'alter trigger "POINTS_UPDATE_TRIGGER" INACTIVE' );
           end;   
           Commit;
           Result := True;
       except
           Rollback;
           Showmessage('The Trigger Update for Splitting Points did not work - call the HelpDesk 0800 657 300' );
       end;
    end;
end;
{ **************************************************************************** }

{procedure Tdm.IBTransactionCommitCount;
var i : integer;
    temp : TIBCustomDataSet;
    v : Variant;
begin
     { done in form deactivate to commit the changes,
      commit retaining means you cant edit the same record in another
      transaction because it locks - eg Maintenance to rollover
      }
    // if IBTransaction.InTransaction then begin
        // locate for the Maintenance Screen on  qryMembersLook
 {       v := qryMembersLook.FieldByName('MEMBER').AsInteger;
        FCustomDataSetList.Clear;                              
        for i := 0 to IBTransaction.SQLObjectCount -1 do begin
            if IBTransaction.SQLObjects[i].inheritsFrom(TIBCustomDataSet) then begin
              temp := IBTransaction.SQLObjects[i] as TIBCustomDataSet;
              // save the objects for after the commit
              if (temp.Active) and (temp.transaction.name = IBTransaction.Name ) then
                 FCustomDataSetList.AddObject(Temp.Name, TObject(temp));
           end;
        end;
        try
           //Commit
           IBTransaction.Commit;
           IBTransaction.StartTransaction;
        finally
           // active all tables
          { for i := 0 to FCustomDataSetList.Count -1 do begin
              TIBCustomDataSet(FCustomDataSetList.Objects[i]).Active := True;
              // qryLook locate back to the same member
              if TIBCustomDataSet(FCustomDataSetList.Objects[i]).Name = qryMembersLook.Name then
                 qryMembersLook.Locate('MEMBER',v,[]);
           end; }
     {   end;
     end;
end;}
{ **************************************************************************** }
procedure Tdm.IBTransactionCommit;
var i : integer;
    temp : TComponent;
    v : Variant;
begin
     { done in form deactivate to commit the changes,
      commit retaining means you cant edit the same record in another
      transaction because it locks - eg Maintenance to rollover
      }
     if IBTransaction.InTransaction then begin
        // locate for the Maintenance Screen on  qryMembersLook
        v := qryMembersLook.FieldByName('MEMBER').AsInteger;
        FActiveCustomDataSetList.Clear;
        // pass from FCustomDataSetList to FActiveCustomDataSetList any active tables
        for i := 0 to FCustomDataSetList.Count -1 do begin
            if TIBCustomDataSet(FCustomDataSetList.Objects[i]).Active = True then begin
               //FActiveCustomDataSetList.Add(FCustomDataSetList[i]);
               FActiveCustomDataSetList.AddObject(FCustomDataSetList[i],FCustomDataSetList.Objects[i]);
            end;
        end;
        try
           //Commit
           IBTransaction.Commit;
           IBTransaction.StartTransaction;
        finally
           // active all tables
           for i := 0 to FActiveCustomDataSetList.Count -1 do begin
              TIBCustomDataSet(FActiveCustomDataSetList.Objects[i]).Active := True;
              // qryLook locate back to the same member
              if TIBCustomDataSet(FActiveCustomDataSetList.Objects[i]).Name = qryMembersLook.Name then
                 qryMembersLook.Locate('MEMBER',v,[]);
           end;
        end;
     end;
end;
{ **************************************************************************** }
procedure Tdm.IBRollBackRetain;
// test inTransaction and Rollback the changes whilst retaining the connections
begin
     if IBTransaction.InTransaction then
        IBTransaction.RollBackRetaining;
end;
{ **************************************************************************** }
procedure Tdm.StartTrans ( theTrans : TIBTransaction );
begin
      if not theTrans.InTransaction then theTrans.StartTransAction;
end;
{ **************************************************************************** }
procedure Tdm.IBRollBackStart ( theTrans : TIBTransaction );
begin
     if theTrans.InTransaction then begin
        theTrans.Rollback;
        StartTrans ( theTrans );
     end;   
end;
{ **************************************************************************** }
procedure Tdm.MembersDetailsPopulateGrid(var EventCtr,SectionCtr,SubscriptionCtr,GroupCtr : Integer );
// the queries below all look at dsrMembers for the Member Number
var MenuMatePointsUpdate : TMenuMatePointsUpdate;

begin
    try
        // Update the points only for this Member 
       MenuMatePointsUpdate.MembersPointsUpdate(dm.qryMembersLook.FieldByName('MEMBER').AsString);
       with ibdsMembers do begin
         Close; Open;
       end;
       with ibdsM_Events do begin
         Close; Open;
         EventCtr := ibdsM_Events.RecordCount;
       end;
       with ibdsM_Sections do begin
         Close; Open;
         SectionCtr := ibdsM_Sections.RecordCount;
       end;
       with ibdsM_Subs do begin
         Close; Open;
         SubscriptionCtr := ibdsM_Subs.RecordCount;
       end;
       with ibdsM_Groups do begin
         Close; Open;
         GroupCtr := ibdsM_Groups.RecordCount;
       end;
       with ibdsM_Lots do begin
         Close; Open;
       end;
       with ibdsM_Interests do begin
         Close; Open;
       end;
       with ibdsM_Photos do begin
         Close; Open;
       end;
       LastMemberInMaintenance := ibdsMembers.FieldByName('MEMBER').AsString; // pass to payments
       CheckGroupsSubs;
    except
        raise;
    end;
end;
{ **************************************************************************** }
function Tdm.PreCheckLogOn( ScreenCode : integer ; ScreenDesc : string ): Boolean;
var HasAccess : Boolean;
const SQL1 = 'select SCREEN_CODE from SCREENS where SCREEN_CODE = %d';     
      SQL2 = 'Insert into SCREENS values (%d,''%s'',''%s'')';
      SQL3 = 'select USER_CODE,HAS_ACCESS from USER_SEC where USER_CODE ' +
             '= %d and SCREEN_CODE = %d';
      SQL4 = 'Insert into USER_SEC values (%d,%d,''%s'')';
      //SQL5 = 'INSERT INTO USER_LOG (USER_CODE,USER_NAME,SCREEN_CODE,SCREEN_DESC,EDIT_DATE)' +
      //       ' VALUES (%d,''%s'',%d,''%s'',''%s'')';
      msg1 = 'You do not have sufficient authority to access the ''%s''';
// check there is a Screen entry - else add with DEF_ACCESS = Y
// check there is a Security entry else add with HAS_ACCESS = Y
// if HAS_ACCESS then Log the entry in USER_LOG else show a message
begin
     Result := False;
     // check the entry is in the Screens table
     if not sqlExecQueryCheckEOF(qryGeneral,format(SQL1,[ScreenCode])) then begin
        sqlExecQuery(qryGeneral,format(SQL2,[ScreenCode,ScreenDesc,'Y']));
        //IBCommitRetain;
     end;
     // check the entry is in the User_Sec table
     HasAccess := True;
     if not sqlExecQueryCheckEOF(qryGeneral,format(SQL3,[Global.UserCode,ScreenCode])) then begin
        sqlExecQuery(qryGeneral,format(SQL4,[Global.UserCode,ScreenCode,'Y']));
        //IBCommitRetain;
     end else begin
        HasAccess := StrToBoolean(qryGeneral.FieldByName('HAS_ACCESS').AsString);
     end;   
     // tab only - all tabs have braces  
     if Pos('{',ScreenDesc) > 0 then begin
        Result := HasAccess;
        IBCommitRetain;
        exit;                               
     end;
     // Check Access
     if not HasAccess then begin
        if Pos('[',ScreenDesc) > 0 then delete(ScreenDesc,Pos('[',ScreenDesc),1); // remove []
        if Pos(']',ScreenDesc) > 0 then delete(ScreenDesc,Pos(']',ScreenDesc),1);
        if Pos(' ',ScreenDesc) > 0 then delete(ScreenDesc,Pos(' ',ScreenDesc),1);
        MessageDlg(Format(msg1,[ScreenDesc]),mtError,[mbOK],0);
        exit;
     end;
     // insert into User_Log    generator takes care of USERLOGSEQ
     // removed this log 09/04/2001
     //sqlExecQuery(qryGeneral,format(SQL5,[Global.UserCode,Global.UserName,ScreenCode,ScreenDesc,GetShortDateTime(now)]));
     IBCommitRetain;
     Result := True;
end;
{ **************************************************************************** }
function Tdm.LoggedOnUsers : ShortInt;
// returns the Sum of logged on users
begin
     Result := dm.IBDatabaseInfo.UserNames.Count;
end;
{ **************************************************************************** }
function Tdm.LastNumberPlusOne( TableName,SeqField : string) : LongInt;
// find last number in the field and increment - groups/sections/events/subs (not auto inc)
const SQL1 = 'SELECT MAX(%s) FROM %s';
begin
     Result := -1;
     if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL1,[SeqField,TableName])) then 
        Result := qryGeneral.Fields[0].AsInteger + 1;
end;
{ **************************************************************************** }
// gets the sites and misc table details read on StartUp and if Modified
function Tdm.GetMenuMateDetails : Boolean;
// they have to log out then in
const SQL1 = 'SELECT * FROM %s';
begin
     Result := False;
     with qryGeneral do begin
        try
           if sqlExecQuery(qryGeneral,Format( SQL1,[MiscTableName])) then begin
              MenuMateInstalled        := StrToBoolean(FieldByName('MENUMATE_INSTALLED').AsString);
              MenuMateTriggerInstalled := StrToBoolean(FieldByName('TRIGGER_INSTALLED').AsString);
              SwipeCardText            := FieldByName('SWIPE_TEXT').AsString;
              SwipeCardLength          := FieldByName('SWIPE_LENGTH').AsInteger;
              SwipeCardOnInsert        := StrToBoolean(FieldByName('ON_INSERT').AsString);
              MenuMateDiscountsInstalled := StrToBoolean(FieldByName('DISCOUNTS_INSTALLED').AsString);
           end;
           Result := True;
        except
           on E: Exception do begin
              sm(E.Message);
              Result := False;
           end;
        end;
     end;
end;
{ **************************************************************************** }
function Tdm.GetSiteDetails : Boolean;
// gets the sites and misc table details read on StartUp and if Modified
// they have to log out then in
const SQL1 = 'SELECT * FROM %s';
begin
     Result := False;
     with qryGeneral do begin
        try
           if sqlExecQuery(qryGeneral,Format( SQL1,[SiteTableName])) then begin
               CompName       := FieldByName('COMP_NAME').AsString;
               SiteLicense    := FieldByName('SITE_LICENSE').AsInteger;
               HelpDesk       := FieldByName('HELP_DESK').AsString;
               MemberLicense  := FieldByName('MEMBER_LICENSE').AsString;
               PosLicense     := FieldByName('POS_LICENSE').AsString;
               Address1       := FieldByName('ADDRESS1').AsString;
               Address2       := FieldByName('ADDRESS2').AsString;
               Address3       := FieldByName('ADDRESS3').AsString;
               PostCode       := FieldByName('POSTCODE').AsString;
               Phone          := FieldByName('PHONE').AsString;
               GSTNo          := FieldByName('GST_NO').AsString;
               if FieldByName('GST_RATE').IsNull then 
                  GSTRate     := 1.125
               else
                  GSTRate     := (FieldByName('GST_RATE').AsFloat / 100) + 1;
               Fax            := FieldByName('FAX').AsString;
               trim(UserName);
               trim(UserPassword);
               trim(CompName);
               if HelpDesk = '' then HelpDesk := '0800 657-300';
               //if GSTRate = 0.00 then GSTRate := 12.5;
           end else exit;
           if sqlExecQuery(qryGeneral,Format( SQL1,[MiscTableName])) then begin
              MemberExpiry    := FieldByName('MEMBER_EXPIRY').AsDateTime;
              MemberPreExpiry := FieldByName('MEMBER_PRE_EXPIRY').AsInteger;
              KeepInactive    := FieldByName('KEEP_INACTIVE').AsInteger;
              if FieldByName('REVOLVE_DAYS').IsNull then RevolveDays := 0
              else RevolveDays     := FieldByName('REVOLVE_DAYS').AsInteger;
              NameFields          := FieldByName('NAME_FIELDS').AsString;
   
              ManLastBackup    := FieldByName('M_BACKUP_DATE').AsDateTime;
              ManBackupEnabled := StrToBoolean(FieldByName('M_BACKUP_ENABLED').AsString);
              ManBackupPCName  := FieldByName('M_BACKUP_PCNAME').AsString;

              AutoBackupEnabled := StrToBoolean(FieldByName('A_BACKUP_ENABLED').AsString);
              AutoBackupPCName  := FieldByName('A_BACKUP_PCNAME').AsString;
              AutoBackupDate    := FieldByName('A_BACKUP_DATE').AsDateTime;
              AutoBackupTime    := FieldByName('A_BACKUP_TIME').AsDateTime;

              ArevImportFilePath := FieldByName('AREV_IMPORT_FILE_PATH').AsString;
              ArevSecondFilePath := FieldByName('AREV_FILE_PATH').AsString;
              
              if ArevImportFilePath = '' then
                  ArevImportFilePath := GetCurrentDir;
              if ArevImportFilePath[Length(ArevImportFilePath)] = '\' then
                 System.Delete(ArevImportFilePath,Length(ArevImportFilePath),1);
              ControllerPCName := FieldByName('CONTROLLER').AsString;
              EditCapitals     := StrToBoolean(FieldByName('CAPITALS').AsString);
              case FieldByName('PAYMENT_RECEIPT').AsInteger of
                  0 : TPaymentReceiptTypes := prPrint;
                  1 : TPaymentReceiptTypes := prSave;
                  2 : TPaymentReceiptTypes := prBulk;
                  3 : TPaymentReceiptTypes := prLetter;
              end;
              case FieldByName('PAYMENT_CARD').AsInteger of
                  0 : TPaymentCardTypes := pcPrint;
                  1 : TPaymentCardTypes := pcSave;
                  2 : TPaymentCardTypes := pcBulk;
              end;   
              DisableServices  := StrToBoolean(FieldByName('DISABLE_SERVICES').AsString);
              if FieldByName('DEFAULT_CARDID').IsNull then
                 DefaultCardID    := -1
              else
                 DefaultCardID    := FieldByName('DEFAULT_CARDID').AsInteger;
              //Discount2 := FieldByName('DISCOUNT_2').AsFloat;
              //Discount3 := FieldByName('DISCOUNT_3').AsFloat;
              SectionRevolveDays     := FieldByName('SECTION_REVOLVE_DAYS').AsInteger;
              PaymentGrids           := FieldByName('PAYMENT_GRIDS').AsString;
              ConfirmPayment         := StrToBoolean(FieldByName('CONFIRM_PAYMENT').AsString);
              // Y = Member   N = Expires  X = No Encoding
              if FieldByName('CARD_ENCODE_MEMBER').isnull then
                 CardEncodeMemberOnly   := 'E'
              else
                 CardEncodeMemberOnly   := FieldByName('CARD_ENCODE_MEMBER').AsString;
              {if FieldByName('PART_PAY_ADVANCE').IsNull then
                 // default to Yes if null
                 PartPayAdvanceExpires := True
              else
                 PartPayAdvanceExpires  := StrToBoolean(FieldByName('PART_PAY_ADVANCE').AsString);               }
              // check the path for Clubmast
              if (ControllerPCName = ReadComputerName) and (ArevImportFilePath <> '') then begin
                 if not DirectoryExists(ArevImportFilePath) then
                    sm(format('The Network path "%s" for Rollover/Exporting files is not Available!',[ArevImportFilePath]));
              end;
              // check the path for Clubmast
              if (ControllerPCName = ReadComputerName) and (ArevImportFilePath <> '') then begin
                 if not DirectoryExists(ArevImportFilePath) then
                    sm(format('The Network path "%s" for Exporting files is not Available!',[ArevImportFilePath]));
              end;
              // check the path for Clubmast RPNYC has 2 separate systems bar and restaurant
              // so have to export to a second dir 
              if (ControllerPCName = ReadComputerName) and (ArevSecondFilePath <> '') then begin
                 if not DirectoryExists(ArevSecondFilePath) then
                    sm(format('The Network path "%s" for Exporting files is not Available!',[ArevSecondFilePath]));
              end;
              // re use ANNUAL_CARD
              FirstPaymentNonActive := StrToBoolean(FieldByName('ANNUAL_CARD').AsString);
              POSPaymentCardReqd    := StrToBoolean(FieldByName('POS_PAY_CARD_REQD').AsString);
              if FieldByName('BOTTOM_LINE_FEED').IsNull then
                 BottomLineFeed      := 0
              else
                 BottomLineFeed      := FieldByName('BOTTOM_LINE_FEED').AsInteger;
              if FieldByName('TOP_LINE_FEED').IsNull then
                 TopLineFeed      := 0
              else
                 TopLineFeed      := FieldByName('TOP_LINE_FEED').AsInteger;
              CheckedFields := FieldByName('CHECKED_FIELDS').AsString;
              PayBeforeExpires    := StrToBoolean(FieldByName('PAY_BEFORE_EXPIRES').AsString);
              if FieldByName('CARD_DEFAULT').IsNull then
                 CardDefault         := varNo
              else
                 CardDefault         := FieldByName('CARD_DEFAULT').AsString;
              if FieldByName('RECEIPT_DEFAULT').IsNull then
                 ReceiptDefault         := varNo
              else
                 ReceiptDefault         := FieldByName('RECEIPT_DEFAULT').AsString;
              // for those sites that require a file of payments for GL and EOD report
              ExportSubs    := StrToBoolean(FieldByName('EXPORT_SUBS').AsString);
              SetNumberType( FieldByName('DEFAULT_NUMBER').AsInteger );
              PointsPassword    := StrToBoolean(FieldByName('POINTS_PASSWORD').AsString);
              ExportSeconds     := FieldByName('EXPORT_TIMER').AsInteger;
              PrintMemberPayment := StrToBoolean(FieldByName('PRINT_MEMBER_PAYMENT').AsString);
              PHMInstalled := StrToBoolean(FieldByName('PHM_INSTALLED').AsString);
              PHMPath := FieldByName('PHM_PATH').AsString;
              PHMControllerPCName := FieldByName('PHM_CONTROLLER').AsString;
              KnownAsDefaulted    := StrToBoolean(FieldByName('KNOWN_AS_DEFAULTED').AsString);
              ParentCodeAddressChanged  := StrToBoolean(FieldByName('PARENT_CODE_ADDRESS').AsString);
              SubsGroupDelete := StrToBoolean(FieldByName('GROUP_DELETE').AsString);
              if PHMPath <> '' then
                 if PHMPath[Length(PHMPath)] = '\' then
                    System.Delete(PHMPath,Length(PHMPath),1);
              if ExportSeconds = 0 then
                 if PHMInstalled then
                    // default to 1 hour
                    ExportSeconds := 3600000
                 else
                    // default to 15 seconds
                    ExportSeconds := 15000
              // measured in milliseconds so * by 1000
              else   
                 ExportSeconds := ExportSeconds * 1000;

              ReceiptType      := FieldByName('PRINT_RECEIPT_AS_TEXT').AsString;
               // conversion from old Y/N options
              if ReceiptType  = 'Y' then
                 ReceiptType := 'D';
              if ReceiptType  = 'N' then
                 ReceiptType := 'T';
              if ReceiptType  = '' then
                 ReceiptType := 'D';
              VIPCreditName    := FieldByName('VIP_CREDIT_NAME').AsString;
              
              CheckForSubs   := StrToBoolean(FieldByName('CHECK_SUBS').AsString); 
              CheckForGroups := StrToBoolean(FieldByName('CHECK_GROUPS').AsString); 
              MaxSub         := FieldByName('MAX_SUB').AsInteger; 
              MaxGroup       := FieldByName('MAX_GROUP').AsInteger; 
              MinSub         := FieldByName('MIN_SUB').AsInteger;
              MinGroup       := FieldByName('MIN_GROUP').AsInteger; 
              ClearInterestsFlag := StrToBoolean(FieldByName('CLEAR_INTERESTS').AsString); 
              
              // notify if KIOSK Random Lucky Draw did not work   
              // Leave the last entry as sqlExecQuery overwrites above query
              if FieldByName('KIOSK_SUCCESS').AsString = varNo then begin
                 sm(format('The Kiosk Random Lucky draw has not worked - %s- %s %s',[FieldByName('KIOSK_ERROR_MSG').AsString,HelpDeskMsg,HelpDesk]));
                 sqlExecQuery(qryGeneral,'update MISC set KIOSK_SUCCESS = ''Y''');
              end;
              // leave here in case for KIOSK_SUCCESS
              
           end else exit;    
           Result := True;
        except
           on E: Exception do begin
              sm(E.Message);
              Result := False;
           end;
        end;
     end;
end;
{ **************************************************************************** }
procedure Tdm.dsrMembersStateChange(Sender: TObject);
begin
    if frmMembersEDIT = nil then exit;
   // with frmMembersEDIT.stbStatus do begin
    with frmMembersEDIT.stbStatus.Panels[0] do begin
       //frmMembersEDIT.stbStatus.Color := clSilver;
       Bevel := pbLowered;
       case dsrMembers.State of
          dsInactive : Text := StateNames[0];
          dsBrowse   : Text := StateNames[1];
          dsEdit     : begin
                       Text := StateNames[2];
                       Bevel := pbRaised;
                       //frmMembersEDIT.stbStatus.Color := clWhite;
                       end;
          dsInsert   : begin
                       Text := StateNames[3];
                       Bevel := pbRaised;
                       //frmMembersEDIT.stbStatus.Color := clAqua;
                       end;            
       else
          Text := '';
       end;
    end;
end;
{ **************************************************************************** }
procedure Tdm.ibdsMembersAfterCancel(DataSet: TDataSet);
// refresh the dbeProposerName, dbeSeconderName and links grid
var DOB : string[12];
const SQL1 = 'select DATES from M_EVENTS where MEMBER = %d and EVENT = 100';
      SQL2 = 'UPDATE M_EVENTS Set DATES = ''%s'' where MEMBER = %d and EVENT = 100';
      msg1 = 'Unable to check the DOB and Event 100'; 
begin
     //if not NumberSearched then
     //   ResetMemberNumber;
     FInsertingMember := False;
     ReUsingMember := False;
     AfterSettings;
     // check date in M_EVENTS
     try
        // only enable when inserting
        frmMEMBERSEdit.dbeMember.ReadOnly := True;
        if sqlExecQueryCheckEOF(qryGeneral,format(SQL1,[ibdsMembers.FieldByName('MEMBER').AsInteger])) then begin
           if qryGeneral.FieldByName('DATES').AsDateTime <> ibdsMembers.FieldByName('DATE_OF_BIRTH').AsDateTime then begin
              DOB := GetShortDate(ibdsMembers.FieldByName('DATE_OF_BIRTH').AsDateTime);
              sqlExecQuery(qryGeneral,format(SQL2,[DOB,ibdsMembers.FieldByName('MEMBER').AsInteger]));
              IBCommitRetain;
              ibdsM_Events.Refresh;
           end;
        end;
     except
        sm(msg1);
     end;
end;
{ **************************************************************************** }
procedure Tdm.AfterSettings;
begin
     with frmMembersEDIT do begin
        PropSecChange;
     end;
end;
{ **************************************************************************** }
procedure Tdm.ibdsMembersBeforeDelete(DataSet: TDataSet);
var MemberPayments : TMemberPayments;
begin
     with ibdsMembers do begin
         Msg := 'Member ' + FieldByName('MEMBER').AsString + ' ' + FieldByName('FULL_NAME').AsString + MDel;
         // a lot of delete actions before this - Links tables etc
         if not LogExport(qryGeneral,FieldByName('MEMBER').AsInteger,0,False) then begin
            sm(FailedToLogExportMsg);
            IBRollBackRetain;
            abort;
         end;  
         BeforeEditPointsCreditSave(BeforeEditPointsCredit,DataSet);
         //if Member has points 
         with BeforeEditPointsCredit do begin
              if (beTotalEarnt    <> 0.00 ) or
                 (beTotalRedeemed <> 0.00 ) or
                 (beEarntCredit   <> 0.00 ) or
                 (beBonusCredit   <> 0.00 ) or
                 (bePointsAvailable <> 0.00 ) or
                 (bePrePaidCredit <> 0.00) then begin
                 PointsCreditTypes := pcDeletedMember;
                 if not MemberPayments.LogPointsCredit( IBTransaction,'Deleted Member',
                                                BeforeEditPointsCredit,dsrMembers.DataSet,0.00) then begin
                    IBRollBackRetain;
                    Abort;
                    exit;
                 end;                 
             end;               
          end;
     end;
end;
{ **************************************************************************** }
procedure Tdm.ibdsMembersBeforeEdit(DataSet: TDataSet);
begin
     with ibdsMembers do begin
        FBeforeEditAddress     := FieldByName('ADDRESS').AsString +
                                  FieldByName('ADDRESS1').AsString +
                                  FieldByName('DISTRICT').AsString +
                                  FieldByName('CITY').AsString +
                                  FieldByName('COUNTRY').AsString;
        FBeforeEditIS_Active   := FieldByName('IS_ACTIVE').AsString;
        FBeforeEditKnown_As    := FieldByName('KNOWN_AS').AsString;
        FBeforeEditFullName    := FieldByName('FULL_NAME').AsString;
        FBeforeEditDOB         := FieldByName('DATE_OF_BIRTH').AsDateTime;
        FBeforeEditFinancial   := FieldByName('SUB_PAID').AsInteger;
        FBeforeEditPriceLevel  := FieldByName('PRICE_LEVEL').AsInteger;
        FBeforeEditVIPPayOnly  := FieldByName('VIP_PAY_ONLY').AsString;
        FBeforeEditAllowCredit := FieldByName('ALLOW_CREDIT').AsString;
        FBeforeEditEarnPoints  := FieldByName('EARN_POINTS').AsString;
        FBeforeEditCardType    := FieldByName('CARD_TYPE').AsString;
        FBeforeEditGLAccount   := FieldByName('GL_ACCOUNT').AsString;
        FBeforeEditCreditLimit := FieldByName('CREDIT_LIMIT').AsFloat;
        FBeforeEditDiscount    := FieldByName('DISCOUNT').AsFloat;
        FBeforeEditIncludeEOD  := FieldByName('INCL_EOD').AsString;
        FBeforeEditExpires     := FieldByName('EXPIRES').AsDateTime;
        FBeforeEditAutoPayment := FieldByName('AUTO_PAYMENT').AsString;
        FBeforeEditVisits      := FieldByName('VISITS').AsInteger;
        BeforeEditPointsCreditSave(BeforeEditPointsCredit,DataSet);
     end;   
end;
{ **************************************************************************** }
procedure Tdm.ibdsMembersAfterPost(DataSet: TDataSet);
var mem : LongInt;
    DOB : String[12];
    s : string[12];
    MemberAddress : string;
    ctr : integer;
const SQL1 = 'select MEMBER,DATES from M_EVENTS where MEMBER = %d and EVENT = 100';
      SQL2 = 'Insert into M_EVENTS values ( %d,100,''%s'',null,''%s'')';
      SQL3 = 'UPDATE M_EVENTS set DATES = ''%s'' where MEMBER = %d and EVENT = 100';
      SQL4 = 'select Count(MEMBER) from M_SUBS where MEMBER = %d';
      SQL5 = 'Insert into M_EVENTS values ( %d,100,null,null,''%s'')';
      SQL6 = 'UPDATE M_EVENTS set DATES = NULL where MEMBER = %d and EVENT = 100';
      SQL7 = 'select MEMBER from MEMBERS where MAILING_CODE_MEMBER = %d';
                                                                
begin
      // turned off in before edit;
     //if MenuMateTriggerInstalled and not FInsertingMember then
     //   dm.TriggerState(true);
     try
        try
           // only enable when inserting
           frmMEMBERSEdit.dbeMember.ReadOnly := True;
           if Msg <> '' then begin
              LogProcess(qryGeneral,Msg);            // Msg built up in before Post
              Msg := '';
           end;
           if MsgActive <> '' then  begin // may have changed other details as well as IS_ACTIVE
              ProcessType := ord(TProcessTypes(ptMembersActive));
              LogProcess(qryGeneral,MsgActive);     // changes to IS_ACTIVE
              MsgActive := '';
           end;
           // Free Entry into the MEMBER field - only update if they have
           // altered the Member Number themselves
           if FInsertingMember and
              (ibdsMembers.FieldByName('MEMBER').AsInteger = FNextMember) and
              (CurrentNumberTypes = ntNextNumber) then
                 SetMemberNumber( ibdsMembers.FieldByName('MEMBER').AsInteger + 1 );
           // reset to Normal
           frmMEMBERSEdit.popmenuNumbersOnClick(frmMEMBERSEdit.PopupMenuNumbers.Items[0]);
           // edit only
           If not FInsertingMember then begin
              if ParentCodeAddressChanged then begin
                 if FBeforeEditAddress <> FBeforePostAddress then begin
                    ibdsAddress.Close;
                    ibdsAddress.ParamByNAme('MailingListCode').AsInteger := ibdsMembers.FieldByName('MAILING_LIST_CODE').ASInteger;
                    ibdsAddress.Open;
                    ibdsAddress.First;
                    ibdsAddress.FetchAll;
                    MemberAddress := '';
                    while not ibdsAddress.EOF do begin
                       if ibdsAddress.FieldByName('MEMBER').AsInteger <> ibdsMembers.FieldByName('MEMBER').ASInteger then begin
                          MemberAddress := MemberAddress + ibdsAddress.FieldByName('MEMBER').ASString + '  ' +
                                           ibdsAddress.FieldByName('FULL_NAME').ASString + #13#10;
                       end;
                       ibdsAddress.Next;
                    end;
                    if MemberAddress <> '' then begin
                       if IfMsgDlg('Change the following Members Address''s' + #13#10 + MemberAddress) then begin
                          ibdsAddress.First;
                          while not ibdsAddress.EOF do begin
                             if ibdsAddress.FieldByName('MEMBER').AsInteger <> ibdsMembers.FieldByName('MEMBER').ASInteger then begin
                                ibdsAddress.Edit;
                                ibdsAddress.FieldByName('ADDRESS').AsString := ibdsMembers.FieldByName('ADDRESS').ASString;
                                ibdsAddress.FieldByName('ADDRESS1').AsString := ibdsMembers.FieldByName('ADDRESS1').ASString;
                                ibdsAddress.FieldByName('DISTRICT').AsString := ibdsMembers.FieldByName('DISTRICT').ASString;
                                ibdsAddress.FieldByName('CITY').AsString := ibdsMembers.FieldByName('CITY').ASString;
                                ibdsAddress.FieldByName('COUNTRY').AsString := ibdsMembers.FieldByName('COUNTRY').ASString;
                                ibdsAddress.Post;
                             end;
                             ibdsAddress.Next;
                          end;
                       end;
                    end;
                 end;
              end;
           end;
           IBCommitRetain;
           // cww Blenheim Problems
           //IBTransactionCommit;    remove as not looking up subs
        except
           IBRollbackRetain;
           sm(MembersPostErrorMsg);
           exit;
        end;
        // check for EVENT 100 birthday - must be added in
        // do it after commiting the MEMBER
        mem := ibdsMembers.FieldByName('MEMBER').AsInteger;
        DOB := GetShortDate(ibdsMembers.FieldByName('DATE_OF_BIRTH').AsDateTime);
        // only check DOB
        if FInsertingMember  or (FBeforeEditDOB <> ibdsMembers.FieldByName('DATE_OF_BIRTH').AsDateTime) then begin
           if not sqlExecQueryCheckEOF(qryGeneral,format(SQL1,[mem])) then begin
              if DOB = Date1899 then
                 sqlExecQuery(qryGeneral,format(SQL5,[mem,GetShortDateTime(now)]))
              else
                 sqlExecQuery(qryGeneral,format(SQL2,[mem,DOB,GetShortDateTime(now)]));
              ibdsM_Events.Close; // refresh
              ibdsM_Events.Open;
           end else begin   // check date also if already there
              if qryGeneral.FieldByName('DATES').AsDateTime <> ibdsMembers.FieldByName('DATE_OF_BIRTH').AsDateTime then begin
                 if DOB = Date1899 then
                    sqlExecQuery(qryGeneral,format(SQL6,[mem]))
                 else
                    sqlExecQuery(qryGeneral,format(SQL3,[DOB,mem]));
                 ibdsM_Events.Close;
                 ibdsM_Events.Open;
              end;
           end;
        end;
        CheckGroupsSubs;
        // check for a Subscription  - Must have one
        // remove from here if you dont want the Subscriptions Popping Up
        //if not dm.sqlExecQueryCheckEOFCount(dm.qryGeneral,format(SQL4,[ibdsMembers.FieldByName('MEMBER').AsInteger])) then begin
        //   with frmMembersEDIT do begin
        //      PageControl.ActivePage := tsGroupsSubs;
              // set to Subscriptions
        //      dbgSectionsDblClick(frmMembersEDIT.dbgSubs);
        //   end;
        //end;
        // reset qryMembersLook where the Member has just been added
        // so qryMembersLook is just set to that Member rather than its previous query
        if FInsertingMember then begin
           frmMembersEDIT.dblcSurname.Text := ibdsMembers.FieldByName('MEMBER').AsString;
           frmMembersEDIT.dblcSurnameDropDown (frmMembersEDIT.dblcSurname);
        end;
        LastMemberInMaintenance := ibdsMembers.FieldByName('MEMBER').AsString; // pass to payments
        FInsertingMember := False;
        with frmMembersEDIT do begin
           PropSecChange;
           if CheckGroupsSubs then begin
              try
                 dblcSurname.OnEnter := nil;
                 // dont fire this event
                 dblcSurname.Setfocus;
              finally
                 dblcSurname.OnEnter := dblcSurnameEnter;
              end;
           end;
        end;
     finally
        FInsertingMember := False;
     end;
end;
{ **************************************************************************** }
function Tdm.CheckGroupsSubs : Boolean;
var ResultGroupsSubs : Boolean;
begin
     ResultGroupsSubs := true;
     if ibdsMembers.RecordCount = 0 then begin
        Result := ResultGroupsSubs;
        exit;
     end;    
     // dont check for the default Member
     if frmMembersEDIT.SettingMemberDefaults then exit;
     // check for Subs First as it may add a group automatically
     if CheckForSubs   then CheckSubs(ResultGroupsSubs );
     if CheckForGroups then CheckGroups(ResultGroupsSubs);
     Result := ResultGroupsSubs;
end;
{ ****************************************************************************}
procedure Tdm.CheckSubs( var ResultGroupsSubs : Boolean);
const CheckSubSQL  = 'select Count(MEMBER) as cntSub from M_SUBS where MEMBER = %d';
begin
     sqlExecQuery(qryGeneral,format(CheckSubSQL,[ibdsMembers.FieldByName('MEMBER').AsInteger]));
     if qryGeneral.FieldByName('cntSub').AsInteger > MaxSub then begin 
        frmMembersEDIT.PageControl.ActivePage := frmMembersEDIT.tsGroupsSubs;
        showmessage(format('You must delete a Subscription - max allowed = %d',[MaxSub]));
        ResultGroupsSubs := false;
     end;
     // things fire in between
     sqlExecQuery(dm.qryGeneral,format(CheckSubSQL,[ibdsMembers.FieldByName('MEMBER').AsInteger]));
     // If Inserting the just pop up with the box
     if FInsertingMember and (qryGeneral.FieldByName('cntSub').AsInteger < MinSub)and (MinSub > 0) then begin
        frmMembersEDIT.PageControl.ActivePage := frmMembersEDIT.tsGroupsSubs;
        frmMembersEDIT.dbgSectionsDblClick(frmMembersEDIT.dbgSubs);
        ResultGroupsSubs := false;
     end else begin
        if qryGeneral.FieldByName('cntSub').AsInteger < MinSub then begin 
           frmMembersEDIT.PageControl.ActivePage := frmMembersEDIT.tsGroupsSubs;
           showmessage(format('You must add a Subscription - min allowed = %d',[MinSub]));
           ResultGroupsSubs := false;
        end;                         
     end;                         
end;
{ ****************************************************************************}        
procedure Tdm.CheckGroups( var ResultGroupsSubs : Boolean);
const CheckGroupSQL = 'select Count(MEMBER) as cntGroup from M_GROUPS where MEMBER = %d';
begin
     sqlExecQuery(dm.qryGeneral,format(CheckGroupSQL,[ibdsMembers.FieldByName('MEMBER').AsInteger]));
     if qryGeneral.FieldByName('cntGroup').AsInteger > MaxGroup then begin 
        frmMembersEDIT.PageControl.ActivePage := frmMembersEDIT.tsGroupsSubs;
        showmessage(format('You must delete a Group - max allowed = %d',[MaxGroup]));
        ResultGroupsSubs := false;
     end;
     // things fire in between
     sqlExecQuery(dm.qryGeneral,format(CheckGroupSQL,[ibdsMembers.FieldByName('MEMBER').AsInteger]));
     // If Inserting the just pop up with the box
     if FInsertingMember and (qryGeneral.FieldByName('cntGroup').AsInteger < MinGroup) and (MinGroup > 0) then begin
        frmMembersEDIT.PageControl.ActivePage := frmMembersEDIT.tsGroupsSubs;
        frmMembersEDIT.dbgSectionsDblClick(frmMembersEDIT.dbgGroups);
        ResultGroupsSubs := false;
     end else begin
        if qryGeneral.FieldByName('cntGroup').AsInteger < MinGroup then begin 
           frmMembersEDIT.PageControl.ActivePage := frmMembersEDIT.tsGroupsSubs;
           showmessage(format('You must add a Group - min allowed = %d',[MinGroup]));
           ResultGroupsSubs := false;
        end;   
     end;                         
end;
{ ****************************************************************************}
function Tdm.PreFillMember(var Member : String) : String;
var ctr,i : integer;
begin
     ctr := SwipeCardLength - Length(Member);
     Result := Member;
     if ctr > 0 then begin
        for i := 1 to ctr do begin
           Result := '0' + Result; 
        end;
     end;
end;
{ ****************************************************************************}
function Tdm.InsertMemberFullName(theMember,FullName : String) : String;
var i : integer;
    Member : String;
begin
     Member := PreFillMember(theMember);
     // replace each occurance of member in SwipeCardText
     Result := SwipeCardText;
     while Pos('member', Result) > 0 do begin
        i := Pos('member', Result);
        system.delete(Result,i,6);
        system.insert(Member,Result,i);
     end;  
     // replace each occurance of fullname in SwipeCardText
     while Pos('fullname', Result) > 0 do begin
        i := Pos('fullname', Result);
        system.delete(Result,i,8);
        system.insert(trim(FullName),Result,i);
     end;  
end;
{ ****************************************************************************}
procedure Tdm.ibdsMembersBeforePost(DataSet: TDataSet);
const SQL1 = 'select surname from members where member = %d';
      msg1 = '%s Points changed for Member %d from %s to %s';
      SQL8 = 'select ADDRESS, ADDRESS1, DISTRICT, CITY, COUNTRY, MAILING_LIST_CODE, MEMBER from MEMBERS ' +
             'where MAILING_LIST_CODE = %d';
var IsActive : string[1];
    mem : LongInt;
    DOB : string[12];
    LogExportBoolean : Boolean;
    ParentCodeSurname : ShortString;
    MemberPayments : TMemberPayments;
    s,Address : String;
    i : integer;
label fromhere;
begin

     with ibdsMembers do begin
        //sqlExecQuery(qryGeneral,format(sql8,[ibdsMembers.FieldByName('MAILING_LIST_CODE').ASInteger]));
        //showmessage(' Count  qryGeneral ' + inttostr( qryGeneral.RecordCount));

        if FieldByName('MEMBER').AsInteger < 1 then begin
           sm('Cannot use a Member Number which is below 1');
           abort;
        end;
        // MenuMate adds Points in the Query - POINTS_AVAILABLE is null then it does not add the points on to the member
        // fixed in ver 1.43
        if FieldByName('POINTS_AVAILABLE').IsNull then 
           FieldByName('POINTS_AVAILABLE').AsFloat := 0.00; 
        if FieldByName('TOTAL_REDEEMED').IsNull then 
           FieldByName('TOTAL_REDEEMED').AsFloat := 0.00; 
        if FieldByName('TOTAL_EARNT').IsNull then 
           FieldByName('TOTAL_EARNT').AsFloat := 0.00; 

        if FieldByName('EARNT_CREDIT').IsNull then 
           FieldByName('EARNT_CREDIT').AsFloat := 0.00; 
        if FieldByName('BONUS_CREDIT').IsNull then 
           FieldByName('BONUS_CREDIT').AsFloat := 0.00; 
        if FieldByName('PRE_PAID_CREDIT').IsNull then 
           FieldByName('PRE_PAID_CREDIT').AsFloat := 0.00; 
        
        //No Date Selected
        try
           if FieldByName('DATE_OF_BIRTH').AsDateTime = StrToDate(StrToDate1899) then
              FieldByName('DATE_OF_BIRTH').Clear;
           if FieldByName('EXPIRES').AsDateTime = StrToDate(StrToDate1899) then
              FieldByName('EXPIRES').Clear;
           if FieldByName('JOINED').AsDateTime = StrToDate(StrToDate1899) then
              FieldByName('JOINED').Clear;
           if FieldByName('APPROVED').AsDateTime = StrToDate(StrToDate1899) then
              FieldByName('APPROVED').Clear;
        except
           sm('ibdsMembersBeforePost Date Fields have not worked'); 
        end;
        IsActive := FieldByName('IS_ACTIVE').AsString;
        if FieldByName('DECEASED').IsNull then
           FieldByName('DECEASED').AsString := varNo;
        //if FieldByName('ASSOCIATION').IsNull then
        //   FieldByName('ASSOCIATION').AsString := varNo;
        if IsActive = 'N' then
           FieldByName('INACTIVE_DATE').AsDateTime := now
        else
          FieldByName('INACTIVE_DATE').Clear;
        if (FieldByName('MAILING_LIST_CODE').AsInteger = 0) or
           (FieldByName('MAILING_LIST_CODE').IsNull) then
           FieldByName('MAILING_LIST_CODE').AsInteger := FieldByName('MEMBER').AsInteger;
        // maintain an index for pauanui sort of Surname/Parent Code
        if FieldByName('MAILING_LIST_CODE').AsInteger = FieldByName('MEMBER').AsInteger then begin
           FieldByName('SPARE_VARCHAR_50').AsString := FieldByName('SURNAME').AsString +
                                                       FieldByName('MAILING_LIST_CODE').AsString +
                                                       'm';  // master field
        end else begin
           if sqlExecqueryCheckEOF(qryGeneral,format(SQL1,[FieldByName('MAILING_LIST_CODE').AsInteger])) then begin
              ParentCodeSurname := qryGeneral.FieldByName('SURNAME').AsString;
              FieldByName('SPARE_VARCHAR_50').AsString := ParentCodeSurname +
                                                          FieldByName('MAILING_LIST_CODE').AsString +
                                                          'z';  // child field
           end else begin
              FieldByName('SPARE_VARCHAR_50').AsString := FieldByName('MAILING_LIST_CODE').AsString +
                                                          'z';  // child field
           end;
        end;
        // keep the Amount Correct
        { keep the Amount Correct for Auto Payments
         only on edit - no subs added if in insert mode
         ibdsM_Subs is still looking at the previous members - qryMembersLook }
        if (FBeforeEditAutoPayment <> FieldByName('AUTO_PAYMENT').AsString) 
           and (state = dsEdit) then begin
           try
              FAutoPaymentNoCommit := True;
              ibdsM_Subs.DisableControls; 
              ibdsM_Subs.First;
              while not ibdsM_Subs.EOF do begin
                  FVariant := ibdsM_Subs.FieldByName('SUBSCRIPTION').AsInteger;
                  tblSubs.Locate('SUBSCRIPTION',FVariant,[]);  // reposition
                  ibdsM_Subs.Edit;
                  if FieldByName('AUTO_PAYMENT').AsString = 'Y' then begin
                     ibdsM_Subs.FieldByName('AMOUNT').AsFloat := tblSubs.FieldByName('FEE').AsFloat +
                                                               tblSubs.FieldByName('AP_PREMIUM').AsFloat;
                  end else begin
                     ibdsM_Subs.FieldByName('AMOUNT').AsFloat := tblSubs.FieldByName('FEE').AsFloat;
                  end;
                  ibdsM_Subs.Post;
                  ibdsM_Subs.Next;
              end;
            finally
              ibdsM_Subs.Close;
              ibdsM_Subs.Open;
              FAutoPaymentNoCommit := False;
              ibdsM_Subs.EnableControls;
            end;
        end;
        ProcessType := ord(TProcessTypes(ptMembers));
        Msg := 'Member ' + FieldByName('MEMBER').AsString + ' ' + FieldByName('FULL_NAME').AsString;
        LogExportBoolean := False;
        if IsActive <> FBeforeEditIS_Active then begin // change IS_ACTIVE
           LogExportBoolean := True;
           if IsActive = 'N' then
              MsgActive := Msg + ' become InActive'
           else
              MsgActive := Msg + ' become Active';
        end;
        if State = dsEdit then Msg := Msg + MChg
        else Msg := Msg + MAdd;
        // Export for Arev
        case dsrMembers.State of
           dsEdit   : begin
                         if (FBeforeEditExpires    <> FieldByName('EXPIRES').AsDateTime) or
                            (FBeforeEditKnown_As   <> FieldByName('KNOWN_AS').AsString)  or
                            (FBeforeEditDOB        <> FieldByName('DATE_OF_BIRTH').AsDateTime) or
                            (FBeforeEditFullName   <> FieldByName('FULL_NAME').AsString) or
                            (FBeforeEditFinancial   <> FieldByName('SUB_PAID').AsInteger) or
                            (FBeforeEditPriceLevel  <> FieldByName('PRICE_LEVEL').AsInteger) or
                            (FBeforeEditVIPPayOnly  <> FieldByName('VIP_PAY_ONLY').AsString) or
                            (FBeforeEditAllowCredit <> FieldByName('ALLOW_CREDIT').AsString) or
                            (FBeforeEditEarnPoints  <> FieldByName('EARN_POINTS').AsString) or
                            (FBeforeEditCardType    <> FieldByName('CARD_TYPE').AsString) or
                            (FBeforeEditGLAccount   <> FieldByName('GL_ACCOUNT').AsString) or
                            (FBeforeEditCreditLimit <> FieldByName('CREDIT_LIMIT').AsFloat) or
                            (FBeforeEditDiscount    <> FieldByName('DISCOUNT').AsFloat) or
                            (FBeforeEditIncludeEOD  <> FieldByName('INCL_EOD').AsString) then
                            LogExportBoolean := True;
                        FBeforePostAddress  :=  FieldByName('ADDRESS').AsString +
                                     FieldByName('ADDRESS1').AsString +
                                     FieldByName('DISTRICT').AsString +
                                     FieldByName('CITY').AsString +
                                     FieldByName('COUNTRY').AsString;
                        if FBeforeEditVisits <> FieldByName('Visits').AsInteger then begin
                           dm.LogProcess(dm.qryGeneral,format('Member %d, visits changed from %d to %d',[FieldByName('MEMBER').AsInteger,FBeforeEditVisits,FieldByName('VISITS').AsInteger]));
                        end;
                         // logit
                         with BeforeEditPointsCredit do begin
                            if (beTotalEarnt <> FieldByName('TOTAL_EARNT').AsFloat) or
                               (beTotalRedeemed <> FieldByName('TOTAL_REDEEMED').AsFloat) or
                               (beEarntCredit <> FieldByName('EARNT_CREDIT').AsFloat) or
                               (bePointsAvailable <> FieldByName('POINTS_AVAILABLE').AsFloat) or
                               (beBonusCredit <> FieldByName('BONUS_CREDIT').AsFloat) or
                               (bePrePaidCredit <> FieldByName('PRE_PAID_CREDIT').AsFloat) then begin
                                PointsCreditTypes := pcMaintenance;
                                if FieldByName('REASON').AsString = '' then
                                   FieldByName('REASON').AsString := 'Credit/Points in ' + frmMembersEDIT.ScreenName;
                                if not MemberPayments.LogPointsCredit( IBTransaction,FieldByName('REASON').AsString,
                                                 BeforeEditPointsCredit,dsrMembers.DataSet,0.00) then begin
                                   Abort;
                                   exit;
                                end;                 
                                // dont save the string - dont want it to reappear in Settings
                                FieldByName('REASON').AsString := '';
                            end;  
                         end;  
                      end;
           dsInsert : LogExportBoolean := True;
        end;
        fromhere:;
        if SwipeCardOnInsert = true then begin
           // Insert
           if FInsertingMember then
              // format the SWIPE_CARD field
              FieldByName('SWIPE_CARD').AsString := InsertMemberFullName(FieldByName('MEMBER').AsString,FieldByName('FULL_NAME').AsString);
        end else begin
           // Insert and Edit
           FieldByName('SWIPE_CARD').AsString := InsertMemberFullName(FieldByName('MEMBER').AsString,FieldByName('FULL_NAME').AsString);
              
        end;
        // Blenheim,Richmond,Tuskers,Pauanui,Howick,Timaru
        {if CompName = 'Howick Club Inc' then begin 
           // only for inserting a member
           if FInsertingMember then begin
              // prefilled to 5 digits
              i := length(FieldByName('MEMBER').AsString);
              case i of
                 1 : s := '0000' + FieldByName('MEMBER').AsString;
                 2 : s := '000'  + FieldByName('MEMBER').AsString;
                 3 : s := '00'  + FieldByName('MEMBER').AsString;
                 4 : s := '0'  + FieldByName('MEMBER').AsString;
                 5,6,7 : s := FieldByName('MEMBER').AsString;
              end;
              FieldByName('SWIPE_CARD').AsString := ';' + s + '?';
           end;
        end else if CompName = 'Blenheim Working Mens Club' then begin
           i := length(FieldByName('MEMBER').AsString);
           case i of
              1 : s := '000' + FieldByName('MEMBER').AsString;
              2 : s := '00'  + FieldByName('MEMBER').AsString;
              3 : s := '0'  + FieldByName('MEMBER').AsString;
              4,5,6 : s := FieldByName('MEMBER').AsString;
           end;
           FieldByName('SWIPE_CARD').AsString := '%5' + s
                                                 + '30064?;5' + s
                                                 + '30064?';

      
        end else if (CompName = 'Richmond WMC')  then begin
           FieldByName('SWIPE_CARD').AsString := ';' + FieldByName('MEMBER').AsString + '?';
        end else if CompName = 'Tuskers Elmwood' then begin
           i := length(FieldByName('MEMBER').AsString);
           case i of
              1 : s := '00' + FieldByName('MEMBER').AsString;
              2 : s := '0'  + FieldByName('MEMBER').AsString;
           3,4,5,6 : s := FieldByName('MEMBER').AsString;
           end;
           FieldByName('SWIPE_CARD').AsString := ';' + s + '?';

           
        end else if CompName =  'Pauanui Sports & Recreation Club' then begin
           {update members set swipe_card = '%0' || member ||
            '00598AD@@@@^]@@@@@@@@@@@@@@@@H@@@@@' || FULL_NAME || '?;;'
            || member || '?'}
            
            // Note - some members dont match ie the cards full name does not match
            // full name in Members table, only do it when adding
         {  if FInsertingMember then begin
              i := length(FieldByName('MEMBER').AsString);
              case i of
                 1 : s := '000' + FieldByName('MEMBER').AsString;
                 2 : s := '00'  + FieldByName('MEMBER').AsString;
                 3 : s := '0'  + FieldByName('MEMBER').AsString;
                 4,5,6 : s := FieldByName('MEMBER').AsString;
              end;
              FieldByName('SWIPE_CARD').AsString := '%0' + s + '00598AD@@@@^]@@@@@@@@@@@@@@@@H@@@@@' +
                                                    trim(FieldByName('FULL_NAME').AsString) +
                                                    '?;' + s + '?';
           end;                                         
        // Timaru has 11 digit numbers
        end else if pos('Timaru',CompName) > 0 then begin
           i := length(FieldByName('MEMBER').AsString);
           case i of
              1 : s := '0000000000' + FieldByName('MEMBER').AsString;
              2 : s := '000000000'  + FieldByName('MEMBER').AsString;
              3 : s := '00000000'  + FieldByName('MEMBER').AsString;
              4 : s := '0000000'  + FieldByName('MEMBER').AsString;
              5 : s := '000000'  + FieldByName('MEMBER').AsString;
              6 : s := '00000'  + FieldByName('MEMBER').AsString;
           end;
           FieldByName('SWIPE_CARD').AsString := ';' + s + '?';
        end else begin
           FieldByName('SWIPE_CARD').AsString      := ';' + ibdsMembers.FieldByName('MEMBER').AsString + '?';
        end; }


        
        if LogExportBoolean then begin
           if not LogExport(qryGeneral,FieldByName('MEMBER').AsInteger,0,False) then begin
              sm(FailedToLogExportMsg);
              abort;
           end;   
        end;
        // if resusing Active or Non Financial Members then Reset the Points 
        // set in  MembersEDIT proc GetNonActiveOrFinancial
        if ReUsingMember then begin
           if not LogExport(qryGeneral,FieldByName('MEMBER').AsInteger,0,True) then begin
              sm(FailedToLogExportMsg);
              abort;
           end;   
           ReUsingMember := False;
        end; 
    end;
end;
{ **************************************************************************** }
procedure Tdm.ibdsMembersBeforeInsert(DataSet: TDataSet);
begin
end;
{ **************************************************************************** }
procedure Tdm.GetNextNumber;
var tempMember : longint;
    i : shortint;
const SQL1 = 'select NEXT_MEMBER from M_NUMBERS where NUMBER_ID = %d';
begin
     with qryGeneral do begin
         FNextMember := 0;
         for i := 0 to 20 do begin
            try
               if sqlExecQueryCheckEOF(qryGeneral,format(SQL1,[Global.NextMemberIndex])) then  // may return 0 as first member
                  FNextMember := FieldByName('NEXT_MEMBER').AsInteger
               else continue; // try again
            except
               continue; // try again
            end;
            // check for members in each table - if so MEMBERGEN is incremented in Proc GetCount
            // tries to Ensures Integrity of Members M_subs/M_Events/M_sections
            if i = 20 then begin
               sm( 'Please setup Next Member in Member Settings Screen/Other tab/Member Numbers');
               abort;
               exit;
            end;
            if GetCount(MembersTableName)   > 0 then continue;
            if GetCount(MSubsTableName)     > 0 then continue;
            if GetCount(MSectionsTableName) > 0 then continue;
            if GetCount(MEventsTableName)   > 0 then continue;
            if GetCount(MGroupsTableName)   > 0 then continue;
            break;
         end;
     end;
end;
{ **************************************************************************** }
function Tdm.GetBlankNumber : boolean;
const SQL1 = 'select min(member) as MIN_MEMBER,max(MEMBER) as MAX_MEMBER from members';
      SQL3 = 'select MEMBER from MEMBERS where MEMBER Between %d and %d order by MEMBER';
      SQL2 = 'select member from members where member = %s';
var i,MinMember,MaxMember : Longint;
begin
     Result := False;
     FBlankNumber := 1;
     if dm.sqlExecQueryCheckEOF(dm.qryGeneral,SQL1) then begin
        MinMember := dm.qryGeneral.FieldByName('MIN_MEMBER').AsInteger;
        MaxMember := dm.qryGeneral.FieldByName('MAX_MEMBER').AsInteger;
     end else begin
        MinMember := 0;
        MaxMember := 5000;
     end;
     if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL3,[MinMember,MaxMember ])) then begin
        i := dm.qryGeneral.FieldByName('MEMBER').AsInteger;
        while not dm.qryGeneral.EOF do begin
           if i = dm.qryGeneral.FieldByName('MEMBER').AsInteger then begin
              inc(i);
           end else begin
              break;
           end;
           dm.qryGeneral.next;
        end;
        if i > dm.qryGeneral.FieldByName('MEMBER').AsInteger then begin
           sm(format('There are no unused numbers between %d and %d',[MinMember,MaxMember]));
           exit;
        end;
        FBlankNumber := i;
        Result := True;
     end else begin
        sm(format('There are no numbers between %d and %d',[MinMember,MaxMember]));
     end;
end;
{ **************************************************************************** }
function Tdm.GetCount ( tbl : shortstring ) : Integer;
const SQL1 = 'SELECT COUNT(MEMBER) FROM %s WHERE Member = %d';
begin
     Result := 999999;
     try
        if sqlExecQueryCheckEOF(qryGeneral,format(SQL1,[tbl,FNextMember])) then
           Result := qryGeneral.Fields[0].AsInteger;
        if Result > 0 then SetMemberNumber( FNextMember + 1);
     except
        Result := 999999;
     end;
end;
{ **************************************************************************** }
procedure Tdm.SetMemberNumber( NextMember : LongInt);
const SQL1 = 'update M_NUMBERS SET NEXT_MEMBER = %d where NUMBER_ID = %d';
begin
     sqlExecQuery(qryGeneral,format(SQL1,[NextMember,Global.NextMemberIndex]));
end;
{ **************************************************************************** }
procedure Tdm.ibdsMembersAfterInsert(DataSet: TDataSet);
const SQL1 = 'Delete from MEMBERS_DEF where MEMBER = %d';
      SQL2 = 'INSERT into MEMBERS_DEF SELECT * from MEMBERS where MEMBER = %d';
var member : LongInt;
begin
     FInsertingMember := True;
     with ibdsMembers do begin
         case CurrentNumberTypes of
            ntNextNumber   :  begin
                                GetNextNumber;
                                FieldByName('MEMBER').AsInteger := FNextMember;
                              end;
            ntBlankNumber  :  begin
                                 if GetBlankNumber then begin
                                    FieldByName('MEMBER').AsInteger := FBlankNumber;
                                 end else begin
                                    abort;
                                    exit;
                                 end;
                              end;
         end;
         // ParamByName is set from the calling process
         member := dm.ibdsMembersDef.ParamByName('MEMBER').AsInteger;
         if Member <> -1 then begin
            //delete from MEMBERS_DEF in case
            dm.sqlExecQuery(dm.qryGeneral,format(SQL1,[member]));
            // insert into MEMBERS_DEF
            dm.sqlExecQuery(dm.qryGeneral,format(SQL2,[member]));
            // close open with the correct member number
            ResetMembersDef;
            // pass thru the relevant fields
            SetMembersDefaults;
            ibdsMembersDef.Close;
            // delete from MEMBERS_DEF
            dm.sqlExecQuery(dm.qryGeneral,format(SQL1,[member]));
         end else begin
            // Default Member
            ResetMembersDef;
            SetMembersDefaults;
         end;
         AfterSettings;
         // only enable when inserting
         frmMEMBERSEdit.dbeMember.ReadOnly := False;
         frmMEMBERSEdit.dbeMember.SetFocus;
     end;
end;
{ **************************************************************************** }
procedure Tdm.SetMembersDefaults;
var CopyFromMember : Boolean;
begin
     ibdsMembersDef.Refresh;
     // Copy From
     if  dm.ibdsMembersDef.ParamByName('MEMBER').AsInteger = -1 then
        CopyFromMember := False
     else
        CopyFromMember := True;
     with ibdsMembers do begin
        FieldByName('KNOWN_AS').AsString        := ibdsMembersDef.FieldByName('KNOWN_AS').AsString;
        FieldByName('MALE_FEMALE').AsString     := ibdsMembersDef.FieldByName('MALE_FEMALE').AsString;
        FieldByName('DATE_OF_BIRTH').asDateTime := ibdsMembersDef.FieldByName('DATE_OF_BIRTH').asDateTime;
        FieldByName('LUCKY_DATE').Clear;
        FieldByName('ADDRESS').AsString         := ibdsMembersDef.FieldByName('ADDRESS').AsString;
        FieldByName('ADDRESS1').AsString        := ibdsMembersDef.FieldByName('ADDRESS1').AsString;
        FieldByName('DISTRICT').AsString        := ibdsMembersDef.FieldByName('DISTRICT').AsString;
        FieldByName('CITY').AsString            := ibdsMembersDef.FieldByName('CITY').AsString;
        FieldByName('COUNTRY').AsString         := ibdsMembersDef.FieldByName('COUNTRY').AsString;
        FieldByName('STREET_ADDRESS').AsString  := ibdsMembersDef.FieldByName('STREET_ADDRESS').AsString;
        FieldByName('STREET_ADDRESS1').AsString := ibdsMembersDef.FieldByName('STREET_ADDRESS1').AsString;
        FieldByName('STREET_DISTRICT').AsString := ibdsMembersDef.FieldByName('STREET_DISTRICT').AsString;
        FieldByName('STREET_CITY').AsString     := ibdsMembersDef.FieldByName('STREET_CITY').AsString;
        FieldByName('STREET_COUNTRY').AsString  := ibdsMembersDef.FieldByName('STREET_COUNTRY').AsString;
        if ibdsMembersDef.FieldByName('MAILING_CODE').IsNull then
           FieldByName('MAILING_CODE').Clear
        else
           FieldByName('MAILING_CODE').AsInteger := ibdsMembersDef.FieldByName('MAILING_CODE').AsInteger;
        FieldByName('EMAIL').AsString           := ibdsMembersDef.FieldByName('EMAIL').AsString;
        FieldByName('HOME_PHONE').AsString      := ibdsMembersDef.FieldByName('HOME_PHONE').AsString;
        FieldByName('WORK_PHONE').AsString      := ibdsMembersDef.FieldByName('WORK_PHONE').AsString;
        FieldByName('CELL_PHONE').AsString      := ibdsMembersDef.FieldByName('CELL_PHONE').AsString;
        FieldByName('PROPOSER').Clear;
        FieldByName('SECONDER').Clear;
        FieldByName('RECEIPT').Clear;
        FieldByName('TRANS_FROM').AsString      := ibdsMembersDef.FieldByName('TRANS_FROM').AsString;
        //if ibdsMembersDef.FieldByName('GROUPS').IsNull then
        //FieldByName('GROUPS').Clear;
        //else
        //   FieldByName('GROUPS').AsInteger      := ibdsMembersDef.FieldByName('GROUPS').AsInteger;
        FieldByName('PRODUCT_PURCHASE').Clear;
        FieldByName('AMOUNT_DONATED').Clear;
        FieldByName('GL_ACCOUNT').AsString      := ibdsMembersDef.FieldByName('GL_ACCOUNT').AsString;
        FieldByName('POSITIONS').AsString       := ibdsMembersDef.FieldByName('POSITIONS').AsString;
        FieldByName('CHARGE_CODE').AsInteger    := ibdsMembersDef.FieldByName('CHARGE_CODE').AsInteger;
        FieldByName('DEBENTURE').AsInteger      := ibdsMembersDef.FieldByName('DEBENTURE').AsInteger;
        FieldByName('DAIRY_NOTES').AsString     := ibdsMembersDef.FieldByName('DAIRY_NOTES').AsString;
        FieldByName('TITLE').AsString           := ibdsMembersDef.FieldByName('TITLE').AsString;
        FieldByName('INITIALS').AsString        := ibdsMembersDef.FieldByName('INITIALS').AsString;
        FieldByName('FIRST_NAME').AsString      := ibdsMembersDef.FieldByName('FIRST_NAME').AsString;
        FieldByName('SURNAME').AsString         := ibdsMembersDef.FieldByName('SURNAME').AsString;
        FieldByName('FULL_NAME').AsString       := ibdsMembersDef.FieldByName('FULL_NAME').AsString;
        FieldByName('INACTIVE_DATE').Clear;

        FieldByName('CREDIT_LIMIT').AsFloat       := 0.00;
        FieldByName('BONUS_CREDIT').AsFloat       := 0.00;
        FieldByName('EARNT_CREDIT').AsFloat       := 0.00;
        FieldByName('PRE_PAID_CREDIT').AsFloat    := 0.00;
        //FieldByName('VALUE_OF_CREDIT').AsFloat    := 0.00;
        FieldByName('POINTS_AVAILABLE').AsFloat   := 0.00;
        FieldByName('TOTAL_EARNT').AsFloat        := 0.00;
        FieldByName('TOTAL_REDEEMED').AsFloat     := 0.00;
        FieldByName('NEWSLETTER').AsString        := ibdsMembersDef.FieldByName('NEWSLETTER').AsString;

        FieldByName('JOINED').AsDateTime    := Date; // Read Only
        FieldByName('APPROVED').AsDateTime  := Date;
       // FieldByName('EXPIRES').AsDateTime   := Date; // Read Only

        // Copy From
        if CopyFromMember then begin
           FieldByName('EXPIRES').AsDateTime   := ibdsMembersDef.FieldByName('EXPIRES').AsDateTime;
        end else if not FixedExpirySystem then begin // revolving
           FieldByName('EXPIRES').AsDateTime   := Date //global.AddOneYear( now )
        end else begin
           // set it to MemberExpiry where MemberExpiry <=  date - watch could be
           // before a rollover but after the MemberExpiry date
           if Global.MemberExpiry <= date then
              FieldByName('EXPIRES').AsDateTime   := Global.MemberExpiry
           else
              FieldByName('EXPIRES').AsDateTime   := global.AddYears(Global.MemberExpiry, -1);
        end;
        FieldByName('JOINING_FEE').AsFloat     := ibdsMembersDef.FieldByName('JOINING_FEE').AsFloat;
        FieldByName('DISCOUNT').AsFloat        := ibdsMembersDef.FieldByName('DISCOUNT').AsFloat; //Global.Discount;

        //if CopyFromMember then
           FieldByName('SUB_PAID').AsInteger   := ibdsMembersDef.FieldByName('SUB_PAID').AsInteger;
       // else
        //   FieldByName('SUB_PAID').AsInteger   := 0;
        FieldByName('SECTION_PAID').AsInteger  := 0;
        FieldByName('ALLOW_CREDIT').AsString   := ibdsMembersDef.FieldByName('ALLOW_CREDIT').AsString;
        FieldByName('EARN_POINTS').AsString    := ibdsMembersDef.FieldByName('EARN_POINTS').AsString;
        FieldByName('PRICE_LEVEL').AsInteger   := ibdsMembersDef.FieldByName('PRICE_LEVEL').AsInteger;
        FieldByName('VIP_PAY_ONLY').AsString   := ibdsMembersDef.FieldByName('VIP_PAY_ONLY').AsString;
        FieldByName('CARD_TYPE').AsString      := ibdsMembersDef.FieldByName('CARD_TYPE').AsString;
        FieldByName('INCL_EOD').AsString       := ibdsMembersDef.FieldByName('INCL_EOD').AsString;
        FieldByName('DECEASED').AsString       := ibdsMembersDef.FieldByName('DECEASED').AsString;
        FieldByName('SENIOR').AsString         := ibdsMembersDef.FieldByName('SENIOR').AsString;
        FieldByName('FOUNDATION').AsString     := ibdsMembersDef.FieldByName('FOUNDATION').AsString;
        FieldByName('DONATED').AsString        := ibdsMembersDef.FieldByName('DONATED').AsString;
        FieldByName('REFUNDED').AsString       := ibdsMembersDef.FieldByName('REFUNDED').AsString;
        FieldByName('SAVED').AsString          := 'N'; // must be N for a New Member
        FieldByName('EDIT_DATE').AsDateTime    := Now;
        // new Members Set Inactive until Paid or over-ridden
        FieldByName('IS_ACTIVE').AsString      := ibdsMembersDef.FieldByName('IS_ACTIVE').AsString;
        FieldByName('ANALYSE').AsString        := ibdsMembersDef.FieldByName('ANALYSE').AsString;
        FieldByName('MAILING_LIST').AsString   := ibdsMembersDef.FieldByName('MAILING_LIST').AsString;
        FieldByName('CARD_TYPE').AsString      := ibdsMembersDef.FieldByName('CARD_TYPE').AsString; // Normal
        FieldByName('SUB_PAYMENT').AsString    := 'N'; // new member without a first payment
        FieldByName('MAILING_LIST_CODE').AsInteger := FieldByName('MEMBER').AsInteger;

        // use wont be able to see these - in the links Grid
        FieldByName('RECEIVE_EMAIL').AsString   := ibdsMembersDef.FieldByName('RECEIVE_EMAIL').AsString;
        FieldByName('AUTO_PAYMENT').AsString    := ibdsMembersDef.FieldByName('AUTO_PAYMENT').AsString;
        FieldByName('COMPANY').AsString         := ibdsMembersDef.FieldByName('COMPANY').AsString;
        FieldByName('OCCUPATION').AsString      := ibdsMembersDef.FieldByName('OCCUPATION').AsString;
        FieldByName('INVOICE_NUMBER').AsString  := ibdsMembersDef.FieldByName('INVOICE_NUMBER').AsString;
        FieldByName('PAYMENT_PERIOD').AsInteger := ibdsMembersDef.FieldByName('PAYMENT_PERIOD').AsInteger;
        FieldByName('NUMBER_CARDS').AsInteger   := 1;
        FieldByName('PAYMENT_DAY').AsInteger    := 1;
        FieldByName('BARCODE_NUMBER').AsInteger := ibdsMembersDef.FieldByName('BARCODE_NUMBER').AsInteger;
        FieldByName('SWIPE_CARD_NUMBER').AsInteger := ibdsMembersDef.FieldByName('SWIPE_CARD_NUMBER').AsInteger;
        FieldByName('CLUBS').AsInteger          := ibdsMembersDef.FieldByName('CLUBS').AsInteger;
        FieldByName('REASON').AsString          := '';
        FieldByName('INVOICE_NUMBER').AsString  := ibdsMembersDef.FieldByName('INVOICE_NUMBER').AsString;
        FieldByName('VISITS').AsInteger := 0;
        FieldByName('LAST_VISIT').Clear;
        FieldByName('DONATION_MEMBER').Clear;
        FieldByName('DONATION').AsFloat         := 0.00;
        
     end;
end;
{ **************************************************************************** }
procedure Tdm.tblGroupsPostError(DataSet: TDataSet; E: EDatabaseError; var Action: TDataAction);
var TempSeq : Integer;
begin
     if (GroupsPKViolationRetry < 20) and (POS ( PrimaryKeyMsg ,e.Message) > 0) then begin
        inc(GroupsPKViolationRetry);
        TempSeq := tblGroups.FieldByName('GROUPS').AsInteger;
        inc(TempSeq);
        tblGroups.FieldByName('GROUPS').AsInteger := TempSeq;
        Action := daRetry;
     end else begin
        Action := daAbort;
     end;
end;
{ **************************************************************************** }
procedure Tdm.tblGroupsAfterInsert(DataSet: TDataSet);
begin
     tblGroups.FieldByName('GROUPS').AsInteger := FGroups;
     GroupsPKViolationRetry := 0;
end;
{ **************************************************************************** }
procedure Tdm.tblGroupsBeforeInsert(DataSet: TDataSet);
begin
     if tblGroups.RecordCount = 0 then
        FGroups := 100  // default start group
     else begin
        tblGroups.refresh;
        FGroups := LastNumberPlusOne( GroupsTableName,'GROUPS');
     end;
end;
{ **************************************************************************** }
procedure Tdm.tblGroupsBeforeDelete(DataSet: TDataSet);
// checks for Groups in  MembersTableName defore deletion
const SQL1 = 'SELECT COUNT(%s) FROM %s WHERE %s = %d';
begin
     if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL1,[FieldNameArray[3],MGroupsTableName,FieldNameArray[3],tblGroups.FieldByName('GROUPS').AsInteger ])) then begin
        if dm.qryGeneral.Fields[0].AsInteger > 0 then begin
           sm(Format('Group %d %s',[tblGroups.FieldByName('GROUPS').AsInteger,IsStillInUseMsg]));
           Abort;
        end;
     end;
     ProcessType := ord(TProcessTypes(ptMaintain));
     MsgBeforePostTBL( tblGroups,'GROUPS');
end;
{ **************************************************************************** }
procedure Tdm.tblSubsAfterInsert(DataSet: TDataSet);
begin
     tblSubs.FieldByName('SUBSCRIPTION').AsInteger := FSubscription;
     tblSubs.FieldByName('SAVED').AsString := varNo;
     tblSubs.FieldByName('KEEP_EXPIRES_CURRENT').AsString := varNo;
     tblSubs.FieldByName('AP_PREMIUM').AsFloat := 0.00;
     tblSubs.FieldByName('ON_INVOICE').AsString := varYes;
     // used to identify Late Payment Subscription
     tblSubs.FieldByName('SPARE_CHAR').AsString := varNo;
     SubsPKViolationRetry := 0;
end;
{ **************************************************************************** }
procedure Tdm.tblSubsPostError(DataSet: TDataSet; E: EDatabaseError;
  var Action: TDataAction);
var TempSeq : Integer;
begin
     if (SubsPKViolationRetry < 20) and (POS ( PrimaryKeyMsg,e.Message) > 0) then begin
        inc(SubsPKViolationRetry);
        TempSeq := tblSubs.FieldByName('SUBSCRIPTION').AsInteger;
        inc(TempSeq);
        tblSubs.FieldByName('SUBSCRIPTION').AsInteger := TempSeq;
        Action := daRetry;
     end else begin
        Action := daAbort;
     end;
end;
{ **************************************************************************** }
procedure Tdm.tblSubsBeforeInsert(DataSet: TDataSet);
begin
     if tblSubs.RecordCount = 0 then
        FSubscription := 1  // default start group
     else begin
        tblSubs.refresh;
        FSubscription := LastNumberPlusOne( SubsTableName,'SUBSCRIPTION');
     end;
end;
{ **************************************************************************** }
procedure Tdm.tblSubsBeforePost(DataSet: TDataSet);
var s : string[20];
const msg1 = 'Keep Expires Current can only be Yes when there is no Fee';  
begin
     if tblSubs.FieldByName('FEE').IsNull then
        tblSubs.FieldByName('FEE').AsFloat := 0.00;
     if tblSubs.FieldByName('AP_PREMIUM').IsNull then
        tblSubs.FieldByName('AP_PREMIUM').AsFloat := 0.00;   
     // KEEP_EXPIRES_CURRENT can only have a Fee of Zero
     if (tblSubs.FieldByName('FEE').AsFloat > 0.00) and 
        (tblSubs.FieldByName('KEEP_EXPIRES_CURRENT').AsString = 'Y') then begin
        sm(msg1);
        tblSubs.FieldByName('KEEP_EXPIRES_CURRENT').AsString := 'N';
        abort;
     end;   
     
     ProcessType := ord(TProcessTypes(ptMaintain));
     MsgBeforePostTBL( tblSubs,'SUBSCRIPTION');
end;
{ **************************************************************************** }
procedure Tdm.tblSubsBeforeDelete(DataSet: TDataSet);
// checks for SUBSRCIPTIONs in  MSubsTableName defore deletion
const SQL1 = 'SELECT COUNT(%s) FROM %s WHERE %s = %d';
begin
     if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL1,[FieldNameArray[0],MSubsTableName,FieldNameArray[0],tblSubs.FieldByName('SUBSCRIPTION').AsInteger])) then begin
        if dm.qryGeneral.Fields[0].AsInteger > 0 then begin
           sm(Format('Subscription %d %s',[tblSubs.FieldByName('SUBSCRIPTION').AsInteger,IsStillInUseMsg]));
           Abort;
        end;
     end;
    ProcessType := ord(TProcessTypes(ptMaintain));
    MsgBeforePostTBL( tblSubs,'SUBSCRIPTION');
end;
{ **************************************************************************** }
procedure Tdm.tblSubsAfterPost(DataSet: TDataSet);
// called from table Subs/Sections/Events/Groups/Cards AfterPost and AfterDelete events
// LogProcess - see also before/After delete - here handles add & change
begin
     try
        if Msg <> '' then begin
           LogProcess(qryGeneral,Msg);            // Msg built up in before Post
           Msg := '';
        end;
        //IBCommitRetain;
        // cww Blenheim Problems
        IBTransactionCommit;
        
     except 
        IBRollbackRetain;
        sm('The has been a Post error - changes have NOT be accepted');
     end;   
end;
{ **************************************************************************** }
procedure Tdm.ibdsM_SubsAfterPost(DataSet: TDataSet);
const msg1 = 'There has been an error saving %s - changes have NOT been accepted';
//  ibds M_Subs, M_Sections M_Events -
// LogProcess - see also before/After delete - here handles add & change
// AfterPost and AfterDelete events  look here
// work is commited if ibdsMembers is in Update or Delete  mode,
// work is NOT commited if ibdsMembers is in Insert mode (  see ibdsMembersAfterPost )
begin
      if Msg <> '' then begin
         LogProcess(qryGeneral,Msg);            // Msg built up in before Post
         Msg := '';
      end;
      // Remove Home Club if a Group with GOLF in gets deleted
      if FRemoveHomeClub then begin
         ibdsMembers.Edit;
         ibdsMembers.FieldByName('CLUBS').AsInteger := 0;
         ibdsMembers.Post;
      end;
      if dsrMembers.State = dsInsert then begin
         DataSet.Active := False; // not refreshing in Links frmMembersEDIT grid
         DataSet.Active := True;  // REFRESH WONT DO IT 
      end else begin
         try
            IBCommitRetain;
            if Assigned(frmMEmbersEDIT) then
               frmMEmbersEDIT.dbgSubs.Update;
           // DataSet.Active := False; // not refreshing in Links frmMembersEDIT grid
           // DataSet.Active := True;  // REFRESH WONT DO IT 
         except
            DataSet.Cancel;
            IBRollBackRetain;
            sm(format (msg1,[DataSet.Name]));
            DataSet.Active := False; // not refreshing in Links frmMembersEDIT grid
            DataSet.Active := True;  // REFRESH WONT DO IT 
         end;
      end;
end;
{ **************************************************************************** }
procedure Tdm.tblSectionsAfterInsert(DataSet: TDataSet);
begin
     tblSections.FieldByName('SECTION').AsInteger := FSection;
     tblSections.FieldByName('SAVED').AsString := 'N';
     tblSections.FieldByName('EXPIRES').AsDateTime := DATE;
     SectionsPKViolationRetry := 0;
end;
{ **************************************************************************** }

procedure Tdm.tblSectionsPostError(DataSet: TDataSet; E: EDatabaseError;
  var Action: TDataAction);
var TempSeq : Integer;
begin
     if (SectionsPKViolationRetry < 20) and (POS ( PrimaryKeyMsg,e.Message) > 0) then begin
        inc(SectionsPKViolationRetry);
        TempSeq := tblSections.FieldByName('SECTION').AsInteger;
        inc(TempSeq);
        tblSections.FieldByName('SECTION').AsInteger := TempSeq;
        Action := daRetry;
     end else begin
        Action := daAbort;
     end;
end;
{ **************************************************************************** }
procedure Tdm.tblSectionsBeforeInsert(DataSet: TDataSet);
begin
     if tblSections.RecordCount = 0 then
        FSection := 1000  // default start group
     else begin
        tblSections.refresh;
        FSection := LastNumberPlusOne( SectionsTableName,'SECTION');
     end;
end;
{ **************************************************************************** }
procedure Tdm.tblSectionsBeforeDelete(DataSet: TDataSet);
// checks for SECTIONs in  MSectionsTableName defore deletion
const SQL1 = 'SELECT COUNT(%s) FROM %s WHERE %s = %d';
begin
     if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL1,[FieldNameArray[1],MSectionsTableName,FieldNameArray[1],tblSections.FieldByName('SECTION').AsInteger ])) then begin
        if dm.qryGeneral.Fields[0].AsInteger > 0 then begin
           sm(Format('Section %d %s',[tblSections.FieldByName('SECTION').AsInteger,IsStillInUseMsg]));
           Abort;
        end;
     end;
     ProcessType := ord(TProcessTypes(ptMaintain));
     MsgBeforePostTBL( tblSections,'SECTION');
end;
{ **************************************************************************** }
procedure Tdm.tblEventsAfterInsert(DataSet: TDataSet);
begin
     tblEvents.FieldByName('EVENT').AsInteger := FEvent;
     EventsPKViolationRetry := 0;
end;
{ **************************************************************************** }
procedure Tdm.tblEventsPostError(DataSet: TDataSet; E: EDatabaseError;
  var Action: TDataAction);
var Tempseq : Integer;
begin
     if (EventsPKViolationRetry < 20) and (POS ( PrimaryKeyMsg,e.Message) > 0) then begin
        inc(EventsPKViolationRetry);
        TempSeq := tblEvents.FieldByName('EVENT').AsInteger;
        inc(TempSeq);
        tblEvents.FieldByName('EVENT').AsInteger := TempSeq;
        Action := daRetry;
     end else begin
        Action := daAbort;
     end;
end;
{ **************************************************************************** }
procedure Tdm.tblMailCodesPostError(DataSet: TDataSet; E: EDatabaseError;
  var Action: TDataAction);
begin
      //lock conflict on no wait transaction deadlock
     if Pos( LockConflictMsg ,E.Message) > 0 then begin 
         sm('Details have just been changed by another user');
         tblMailCodes.Close;
         tblMailCodes.Open; 
         Action := daAbort;
         exit;
     end;
     if (POS ( ForeignKeyMsg,e.Message) > 0) then begin
        sm('A "Mail Code" is still in use by a Member'); 
        Action := daAbort;
        exit;
     end;   
      if (POS ( PrimaryKeyMsg,e.Message) > 0) then begin
        sm('Duplicate Entry ,"Mail Code" must be a unique entry');
        Action := daAbort;
        exit;
     end;
end;
{ **************************************************************************** }
procedure Tdm.tblEventsBeforeInsert(DataSet: TDataSet);
begin
     if tblEvents.RecordCount = 0 then
        FEvent := 100  // default start group
     else begin
        tblEvents.refresh;
        FEvent := LastNumberPlusOne( EventsTableName,'EVENT');
     end;
end;
{ **************************************************************************** }
procedure Tdm.tblEventsBeforeDelete(DataSet: TDataSet);
// checks for EVENTs in  MEventsTableName defore deletion
const SQL1 = 'SELECT COUNT(%s) FROM %s WHERE %s = %d';
begin
     if tblEvents.FieldByName('EVENT').AsInteger = 100 then begin
        sm('You cannot delete the Birthday Event');
        abort;
     end;
     if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL1,[FieldNameArray[2],MEventsTableName,FieldNameArray[2],tblEvents.FieldByName('EVENT').AsInteger ])) then begin
        if dm.qryGeneral.Fields[0].AsInteger > 0 then begin
           sm(Format('Event %d %s',[tblEvents.FieldByName('EVENT').AsInteger,IsStillInUseMsg]));
           Abort;
        end;
     end;
    ProcessType := ord(TProcessTypes(ptMaintain));
    MsgBeforePostTBL( tblEvents,'EVENT');
end;
{ **************************************************************************** }
procedure Tdm.ibdsM_GroupsAfterInsert(DataSet: TDataSet);
begin
      with ibdsM_Groups do begin
        // pk MEMBER, GROUPS
        FieldByName('MEMBER').AsInteger    := ibdsMembers.FieldByName('MEMBER').AsInteger;
        FieldByName('GROUPS').AsInteger    := tblGroups.FieldByName('GROUPS').AsInteger;
        Post;
     end;   
end;
{ **************************************************************************** }
procedure Tdm.ibdsM_SectionsAfterInsert(DataSet: TDataSet);
const SQL1 = 'update MEMBERS set DONATION = %f,DONATION_MEMBER = %d where MEMBER = %d';
begin
     with ibdsM_Sections do begin
        // pk MEMBER, SECTION
        FieldByName('MEMBER').AsInteger     := ibdsMembers.FieldByName('MEMBER').AsInteger;
        FieldByName('SECTION').AsInteger    := tblSections.FieldByName('SECTION').AsInteger;
        FieldByName('RECEIPT').AsInteger    := 0;
        // if there is no date for the Section
        if tblSections.FieldByName('EXPIRES').IsNull then
           FieldByName('EXPIRES').AsDateTime   := Date
        else if tblSections.FieldByName('EXPIRES').AsDateTime <= Date then
           FieldByName('EXPIRES').AsDateTime   := tblSections.FieldByName('EXPIRES').AsDateTime
        else
           FieldByName('EXPIRES').AsDateTime   := AddYears(tblSections.FieldByName('EXPIRES').AsDateTime,-1);
        FieldByName('PAID').AsString        := 'N';
        FieldByName('SPONSORSHIP').AsString := 'N';
        FieldByName('EDIT_DATE').AsDateTime := now;
        FieldByName('AMOUNT'     ).AsFloat  := tblSections.FieldByName('FEE').AsFloat;
        FieldByName('AMOUNT_PAID').AsFloat  := 0.00;
        Post;
        if tblSections.FieldByName('DONATION').AsFloat <> 0.00 then begin
           sqlExecQuery(qryGeneral,format(SQL1,[ tblSections.FieldByName('DONATION').AsFloat,tblSections.FieldByName('SECTION').AsInteger, ibdsMembers.FieldByName('MEMBER').AsInteger]));
           dm.ibdsMembers.Refresh;
           frmMembersEdit.PropSecChange;
        end;
     end;
end;
{ **************************************************************************** }
procedure Tdm.ibdsMembersPostError(DataSet: TDataSet; E: EDatabaseError;
  var Action: TDataAction);
begin
     //lock conflict on no wait transaction deadlock
     if Pos( LockConflictMsg ,E.Message) > 0 then begin 
         sm('Details have just been changed by another user, check the Member then retry');
         ibdsMembers.Close;
         ibdsMembers.Open;
         Action := daAbort;
         exit;
     end;
     if (POS ( ForeignKeyMsg ,e.Message) > 0) then begin
        sm('A "Group" or "Parent Code" or "Post Code" does not exist, correct then retry');
        Action := daAbort;
        exit;
     end;
      if (POS ( PrimaryKeyMsg ,e.Message) > 0) then begin
        //MemberInsertChange;
        //NumberSearched := False;
         //sm('Details have just been changed by another user, check the Member then retry');
         sm(format('Member %s is being already used',[ibdsMembers.FieldByName('MEMBER').AsString]));
         Action := daAbort;
     end;
end;
{ **************************************************************************** }
{procedure Tdm.MemberInsertChange;
{  gets the next number
  assigns to Member + mail list code
  refreshes links grids
}
{begin
     GetNextNumber;   // gets new Member Number
     ibdsMembers.FieldByName('MEMBER').AsInteger := FNextMember;
     ibdsMembers.FieldByName('MAILING_LIST_CODE').AsInteger := FNextMember;
     frmMembersEdit.dbeParentCodeName.Text :=  ibdsMembers.FieldByName('FULL_NAME').AsString;
     ibdsM_events.Active := False;
     ibdsM_events.Active := True;
     ibdsM_Subs.Active := False;
     ibdsM_Subs.Active := True;
     ibdsM_Sections.Active := False;
     ibdsM_Sections.Active := True;
     sm('You have already used this Member number,' + #13#10 +
        'check Events,Subs,Sections and Parent Code,  then retry Posting it.');
end;
{ **************************************************************************** }
procedure Tdm.ibdsM_SubsPostError(DataSet: TDataSet; E: EDatabaseError;   var Action: TDataAction);
begin
     // pk of MEMBER , SUBSCRIPTION - may have just been added
     if (POS ( PrimaryKeyMsg,e.Message) > 0) then begin
        Sm(format('Member %d with Subscription %d has already been added',[ibdsM_Subs.FieldByName('Member').AsInteger,ibdsM_Subs.FieldByName('SUBSCRIPTION').AsInteger]));
    //    MemberInsertChange; // wont allow you to insert - clash of PK
        Action := daAbort;
     end;
     if (POS ( ForeignKeyMsg,e.Message) > 0) then begin
        sm('A "Member" or "Subscription"' + DoesNotExistMsg ); 
        Action := daAbort;
     end;            
     
end;
{ **************************************************************************** }
procedure Tdm.ibdsM_SectionsPostError(DataSet: TDataSet; E: EDatabaseError;
  var Action: TDataAction);
begin
     // pk of MEMBER , SECTION - may have just been added
     if (POS ( PrimaryKeyMsg,e.Message) > 0) then begin
        Sm(format('Member %d with Section %d has already been added',[ibdsM_Sections.FieldByName('Member').AsInteger,ibdsM_Sections.FieldByName('SUBSCRIPTION').AsInteger]));
        //MemberInsertChange; // wont allow you to insert - clash of PK
        Action := daAbort;
     end;
     if (POS ( ForeignKeyMsg ,e.Message) > 0) then begin
        sm('A "Member" or "Section"' + DoesNotExistMsg);
        Action := daAbort;
     end;                         
end;
{ **************************************************************************** }
procedure Tdm.ibdsM_EventsPostError(DataSet: TDataSet; E: EDatabaseError;
  var Action: TDataAction);
begin
 // pk of MEMBER , EVENT - may have just been added
     if (POS ( PrimaryKeyMsg,e.Message) > 0) then begin
        Sm(format('Member %d with Event %d has already been added',[ibdsM_Events.FieldByName('Member').AsInteger,ibdsM_Events.FieldByName('SUBSCRIPTION').AsInteger]));
        //MemberInsertChange; // wont allow you to insert - clash of PK
        Action := daAbort;
     end;
     if (POS ( ForeignKeyMsg,e.Message) > 0) then begin
        sm('A "Member" or "Subscription"' + DoesNotExistMsg);
        Action := daAbort;
     end;
end;
{ **************************************************************************** }
procedure Tdm.ibdsM_SubsAfterInsert(DataSet: TDataSet);
const SQL1 = 'Select count(*) from m_GROUPS where MEMBER = %d and GROUPS = %d';
      SQL2 = 'Insert into m_GROUPS ( MEMBER,GROUPS ) values ( %d,%d )';
begin
     with ibdsM_Subs do begin
        // pk MEMBER,SUBSCRIPTION
        FieldByName('MEMBER').AsInteger       := ibdsMembers.FieldByName('MEMBER').AsInteger;
        FieldByName('SUBSCRIPTION').AsInteger := tblSubs.FieldByName('SUBSCRIPTION').AsInteger;
        FieldByName('RECEIPT').AsInteger      := 0;
        FieldByName('AMOUNT_PAID').AsFloat    := 0.00; // gets latest price
        if ibdsMembers.FieldByName('AUTO_PAYMENT').AsString = 'Y' then
           FieldByName('AMOUNT').AsFloat    := tblSubs.FieldByName('FEE').AsFloat + tblSubs.FieldByName('AP_PREMIUM').AsFloat
        else
           FieldByName('AMOUNT').AsFloat    := tblSubs.FieldByName('FEE').AsFloat;
        FieldByName('PAID'     ).AsString   := 'N';
        FieldByName('EDIT_DATE').AsDateTime := now;
        Post;
        // add Group in Automatically if linked in  SUB_GROUPS - table Subscriptions
        // and SUB_GROUPS is not null 
        // bug 05/02/03 SUB_GROUPS is null because of ??? use tblSubs
        if not tblSubs.FieldByName('SUB_GROUPS').IsNull then begin
           if not SqlExecQueryCheckEOFCount(qryGeneral,Format(SQL1,[ FieldByName('MEMBER').AsInteger,tblSubs.FieldByName('SUB_GROUPS').AsInteger ])) then begin
              SqlExecQuery(qryGeneral,Format(SQL2,[ FieldByName('MEMBER').AsInteger, tblSubs.FieldByName('SUB_GROUPS').AsInteger]));
              // refresh display
              ibdsM_Groups.Close;
              ibdsM_Groups.Open;
           end;
        end;
     end;
end;
{ **************************************************************************** }
procedure Tdm.ibdsM_SubsBeforeInsert(DataSet: TDataSet);
//const SQL1 = 'Select KEEP_EXPIRES_CURRENT from M_SUBS ms , SUBSCRIPTIONS s ' +
//             ' where ms.MEMBER = %d and ms.SUBSCRIPTION = s.SUBSCRIPTION';
begin
     {
     if sqlExecQueryCheckEOF(qryGeneral,format(SQL1,[ibdsMembers.FieldByName('MEMBER').AsInteger])) then begin
        while not qryGeneral.EOF do begin
           if qryGeneral.FieldByName('KEEP_EXPIRES_CURRENT').AsString = 'Y' then begin
              sm(KeepExpiresCurrentMsg);
              ibdsM_Subs.Cancel;
              abort;
              exit;
           end;
           qryGeneral.Next;
        end;
     end;}
end;
{ **************************************************************************** }
procedure Tdm.ibdsM_EventsAfterInsert(DataSet: TDataSet);
begin
     with ibdsM_Events do begin
        // pk  MEMBER,EVENT
        FieldByName('MEMBER').AsInteger       := ibdsMembers.FieldByName('MEMBER').AsInteger;
        FieldByName('EVENT' ).AsInteger       := tblEvents.FieldByName('EVENT').AsInteger;
        if tblEvents.FieldByName('EVENT').AsInteger = 100 then // birthday
           FieldByName('DATES').AsDateTime := dm.ibdsMembers.FieldByName('DATE_OF_BIRTH').AsDateTime
        else
           FieldByName('DATES').Clear;
        FieldByName('DATE_LAST'  ).clear;
        FieldByName('EDIT_DATE'  ).AsDateTime := now;
        Post;
     end;             
end;
{ **************************************************************************** }
function Tdm.DeleteMemberLinks(Member : LongInt ; theIBSQL : TIBSQL ) : Boolean;
// deletes all Members from the detail tables  M_EVENTS M_SECTIONS M_SUBS,INTERESTS,LOTS
const SQL1 = 'DELETE FROM %s WHERE MEMBER = %d';
begin
     Result := False;
     sqlExecQuery(theIBSQL,format(SQL1,[MEventsTableName,Member]));
     sqlExecQuery(theIBSQL,format(SQL1,[MSectionsTableName,Member]));
     sqlExecQuery(theIBSQL,format(SQL1,[MSubsTableName,Member]));
     sqlExecQuery(theIBSQL,format(SQL1,[MGroupsTableName,Member]));
     sqlExecQuery(theIBSQL,format(SQL1,[MInterestsTableName,Member]));
     sqlExecQuery(theIBSQL,format(SQL1,[MLotsTableName,Member]));
     sqlExecQuery(theIBSQL,format(SQL1,[MPhotosTableName,Member]));
     Result := True;
end;
{ **************************************************************************** }
procedure Tdm.dsrTblGroupsStateChange(Sender: TObject);
begin
     StateChange( TblGroups );
end;
{ **************************************************************************** }
procedure Tdm.dsrTblSubsStateChange(Sender: TObject);
begin
     StateChange( TblSubs);
end;
{ **************************************************************************** }
procedure Tdm.dsrTblSectionsStateChange(Sender: TObject);
begin
     StateChange( TblSections );
end;
{ **************************************************************************** }
procedure Tdm.dsrTblEventsStateChange(Sender: TObject);
begin
     StateChange( TblEvents);
end;
{ **************************************************************************** }
procedure Tdm.dsrTblMailCodesStateChange(Sender: TObject);
begin
     StateChange( TblMailCodes);
end;
{ **************************************************************************** }
procedure Tdm.dsrTblTendersStateChange(Sender: TObject);
begin
     StateChange( TblTenders);
end;
{ **************************************************************************** }
procedure Tdm.dsrTblNumbersStateChange(Sender: TObject);
begin
     StateChange( TblNumbers);
end;
{ **************************************************************************** }
procedure Tdm.dsrCardDetailStateChange(Sender: TObject);
begin
     StateChange( TblCardDetail);
end;
{ **************************************************************************** }
procedure Tdm.dsrCardMasterStateChange(Sender: TObject);
begin
     StateChange( TblCardMaster);
end;
{ **************************************************************************** }
procedure Tdm.dsrCardMemberStateChange(Sender: TObject);
begin
     StateChange( TblCardMember);
end;
{ **************************************************************************** }
procedure Tdm.StateChange( Tbl : TIBTable);
begin
     if frmMembersMAINT = nil then exit;
     if not frmMembersMAINT.Active then exit;
     with frmMembersMAINT.stbStatus.Panels[0] do begin
       Bevel := pbLowered;
       case Tbl.State of
          dsInactive : Text := StateNames[0];
          dsBrowse   : Text := StateNames[1];
          dsEdit     : begin
                       Text := StateNames[2];
                       Bevel := pbRaised;
                       end;
          dsInsert   : begin
                       Text := StateNames[3];
                       Bevel := pbRaised;
                       end;
       else
          Text := '';
       end;
    end;
end;
{ **************************************************************************** }
procedure Tdm.dsrStateChange( theDataSource : TDataSource; theForm : TSMStdForm );
begin
     if not assigned(theForm) then exit;
     if not theForm.Active then exit;
     try
        with theForm.stbStatus.Panels[0] do begin
          Bevel := pbLowered;
          case theDataSource.State of
             dsInactive : Text := StateNames[0];
             dsBrowse   : Text := StateNames[1];
             dsEdit     : begin
                          Text := StateNames[2];
                          Bevel := pbRaised;
                          end;
             dsInsert   : begin
                          Text := StateNames[3];
                          Bevel := pbRaised;
                          end;
          else
             Text := '';
          end;
       end;
    except
       // do nothing as it may be just events
    end;
end;
{ **************************************************************************** }
procedure Tdm.DeleteLog( tblName,tblField : shortstring; removedays : smallint);
// trims the user and process log { when logging on } 
// backup log after auto or manual backups
const edtDAte = 'EDIT_DATE';
      SQL1 = 'DELETE FROM %s WHERE %s < ''%s''';
var ADate : TDateTime;
begin
     try
        with qryGeneral do begin
           if tblField = '' then tblField := edtDate;
           ADate := ( now - removedays );
           //ADate := Trunc( now - removedays );
           sqlExecQuery(qryGeneral,format(SQL1,[tblName,tblField,getShortDate(ADate)]));
        end;
     finally
       IBCommitRetain;
     end;
end;
{ **************************************************************************** }
procedure Tdm.DeleteReportTables;
const SQL1 = 'Drop table %s';
var lstTableNames : TStringList;
    i : smallint;
begin
     try
        lstTableNames := TStringList.Create;
        IBDatabase.GetTableNames(lstTableNames,False);
        for i := 0 to lstTableNames.Count -1 do begin
           if copy(lstTableNames[i],1,4) = 'TEMP' then begin
              try
                 sqlExecQuery(qryGeneral,format(SQL1,[lstTableNames[i]]));
              except
                 // do nothing
              end;
           end; 
        end; 
     finally
        lstTableNames.Free;
     end;
end;
{ **************************************************************************** }
     //if not HandlerRoutine(CTRL_SHUTDOWN_EVENT) then exit;
     //sm('ctrl alt del');
procedure Tdm.PHMExportTimerTimer(Sender: TObject);
const SQL1 = 'Select MEMBER,FULL_NAME,POINTS_AVAILABLE from members order by member';
      SQL2 = 'INSERT into PHM_LOG values( %d,%f,''%s'',''%s'',%d )';
      PHMExportFileName = '\LOYALTY.CSV';
      PHMImportFileName = '\PMSLOYALTY.CSV';
      PHMImportRenamed  = '\PMSLOYALTY.MEM';
var ExportFile : textFile;
    ImportFile : textFile;                              
    s,PA : String;
    Member,ImportDateTime,ImportComment,ImportBooking : String;
    Points : extended;
    ProcessImportFile,PointsEarned : Boolean;
begin
     // only fires because PHMInstalled = Y and PHMControllerPCName = this PC
     try
        if not IBExport.InTransaction then
           IBExport.StartTransaction;
        ProcessImportFile := False;
        { ****************************************************************************}
        { ****************************************************************************}
        // Importing - try to do this first - update points before exporting the file
        // if the Renamed file exists then dont rename it and work off this one first
        // may of not finished properly last time
        { ****************************************************************************}
        { ****************************************************************************}
        if FileExists (PHMPath + PHMImportRenamed) then begin
           ProcessImportFile := True;
        end else begin
           if FileExists (PHMPath + PHMImportFileName) then begin
              if RenameFile(PHMPath + PHMImportFileName,PHMPath + PHMImportRenamed) then begin
                 ProcessImportFile := True;
              end
           end;
        end;
        if ProcessImportFile then begin
           AssignFile( ImportFile,PHMPath + PHMImportRenamed  );
           Reset( ImportFile );
           while not EOF(ImportFile) do begin
              ReadLn(ImportFile,s);
              Member := GetCommaDelimitedField(0,s);
              Points := StrToFloat(GetCommaDelimitedField(1,s));
              // is in cents
              Points := Points / 100;
              PointsEarned := True;
              if Points < 0.00 then
                 PointsEarned := False;
              Points := abs(Points);   
              ImportDateTime := GetCommaDelimitedField(2,s);
              ImportComment  := GetCommaDelimitedField(3,s);
              ImportBooking  := GetCommaDelimitedField(4,s);

              ibdsPHMPoints.Close;
              ibdsPHMPoints.ParamByName('MEMBER').AsString := Member;
              ibdsPHMPoints.Open;
              if ibdsPHMPoints.Eof then begin
                 SM(format('PHM Import for Member %s has not been found',[Member]));
                 continue; 
              end;
              with ibdsPHMPoints do begin
                 Edit;
                 if PointsEarned then begin
                    FieldByName('POINTS_AVAILABLE').AsFloat := FieldByName('POINTS_AVAILABLE').AsFloat + Points;
                    FieldByName('TOTAL_EARNT').AsFloat      := FieldByName('TOTAL_EARNT').AsFloat + Points;
                 end else begin   
                    FieldByName('POINTS_AVAILABLE').AsFloat := FieldByName('POINTS_AVAILABLE').AsFloat - Points;
                    FieldByName('TOTAL_REDEEMED').AsFloat   := FieldByName('TOTAL_REDEEMED').AsFloat + Points;
                 end;   
                 FieldByName('CARD_LAST_USED').AsDateTime := Date;
                 Post;
              end;
              // write to PHM_LOG
              s := format(SQL2,[StrToInt(Member),Points,ConvertDateStrtoStr(ImportDateTime),ImportComment,StrToInt(ImportBooking)]);
              sqlExecQuery(sqlExport,s);
           end;      
           CloseFile(ImportFile);
           // Rollback if the file has not been deleted or an exception
           try
              if not DeleteFile(PHMPath + PHMImportRenamed) then begin
                 if IBExport.InTransaction then
                    IBExport.Rollback;
                 sm( format('PHM Import of Member Points - File not deleted %s - Work has been Rolledback',[PHMPath + PHMImportRenamed]));
              end;
              if IBExport.InTransaction then
                 IBExport.Commit;
           except
              if IBExport.InTransaction then
                 IBExport.Rollback;
              ShowMessage('PHM Importing of Members Points has not worked - Work has been Rolledback');
           end;      
        end;
        { ****************************************************************************}
        { ****************************************************************************}
        // Exporting - overwrite the file each time thru
        { ****************************************************************************}
        { ****************************************************************************}
        if not IBExport.InTransaction then
           IBExport.StartTransaction;
        sqlExecQuery(sqlExport,SQL1);
        AssignFile( ExportFile,PHMPath + PHMExportFileName  );
        // write over if its there
        Rewrite( ExportFile );
        while not sqlExport.EOF do begin
           s := sqlExport.FieldByName('FULL_NAME').AsString;
           { Convert commas to spaces - in case}
           while Pos(',', S) > 0 do
               S[Pos(',', S)] := ' ';
           //PA := sqlExport.FieldByName('POINTS_AVAILABLE').AsString;
           // try to keep to 99.99 format - error reported at the Kelvin 
           PA := FormatFloat('#####0',sqlExport.FieldByName('POINTS_AVAILABLE').AsFloat * 100);
           // remove the decimal point
           System.Delete(PA,POS('.',PA),1);   
           WriteLn(ExportFile,sqlExport.FieldByName('MEMBER').AsString + ',' +
                              S + ',' + PA );
           sqlExport.Next;
        end;
        Flush(ExportFile); 
        CloseFile(ExportFile); 
        try
           if IBExport.InTransaction then
              IBExport.Commit;
        except
           IBExport.Rollback;
           ShowMessage('PHM exporting of Members has not worked');
        end;      
     finally
        //
     end;
end;
{ **************************************************************************** }
procedure Tdm.ibdsPaySectionsAfterEdit(DataSet: TDataSet);
begin
end;
{ **************************************************************************** }
procedure Tdm.ibdsPaySectionsBeforePost(DataSet: TDataSet);
// loaded records must have these configs
// PAID = 'Y' Receipt > 0
// PAID = 'N' Receipt = 0
// possible user changes are
// PAID = 'N' Credit  - paidBy gets points credited if initially used
//                    - PAymentsTend + Payments + PaymentsDet get credit details
// PAID = 'Y' Payment - paidBy gets points
//                    - PAymentsTend + Payments + PaymentsDet get added
var SectionGoodDate : TDateTime;
    Amount,AmountLessPoints,Rounding : Extended;
    s : string[12];
    FullName,SNAme,PaidByName : string[50];
    Member,PaidBy : LongInt;
    PointsUsed : Extended;
    EDate   : String[20];
    SDate   : String[12];
    MemberPayments : TMemberPayments;
const SQL1 = 'UPDATE PAYMENTS SET PAY_AMOUNT = %f,POINTS_USED = %f,PAID_BY = %d,' +
             'DISCOUNT = 0.00,EDIT_DATE = ''%s'',ROUNDING = %f,PAY_CREDIT = 0.00,FULL_NAME = ' +
             '''%s'' where PAYMENT = %d';    
      SQL2 = 'INSERT INTO PAYMENTS_DET (PAYMENT,DESCRIPTION,EXPIRES,' +
             'AMOUNT,AMOUNT_PAID,PAY_FULL,PAY_TYPE,MEMBER,PAY_DATE,FULL_NAME,PAID_BY) ' +
             'VALUES (%d,''%s'',''%s'',%f,%f,2,1,%d,''%s'',''%s'',%d)';       
      SQL3 = 'INSERT INTO PAYMENTS_TEND (PAYMENT,TENDER_TYPE,AMOUNT,DETAILS,EX_RATE) ' +
             'VALUES (%d,''Cash'',%f,null,0.00 )';       
      SQL4 = 'SELECT PAID_BY,POINTS_USED,ROUNDING from PAYMENTS where PAYMENT = %d';
      SQL5 = 'UPDATE MEMBERS SET ' +
             'TOTAL_REDEEMED = (TOTAL_REDEEMED - %f), EDIT_DATE = ''%s'' WHERE MEMBER = %d';


begin
     if ibdsPaySecMem.RecordCount = 0 then begin
        sm('You must enter a Paid By Member');
        abort;
     end;
     SectionGoodDate := frmMembersPAY.SectionGoodDate;
     // get from here - may be paid by Tenders if > 0 else pay by cash here
     FSectionReceipt := frmMembersPAY.SectionReceipt;
     SName      := frmMembersPAY.SectionName + ' Section';
     PointsUsed := frmMembersPAY.PointsUsedSection;
     Rounding   := frmMembersPAY.RoundValue;
     EDate      := GetShortDateTime(now);
     SDate      := GetShortDate(SectionGoodDate);
     frmMembersPAY.Blocked := True;
     with ibdsPaySections do begin
        DisableControls; // must be here
        FPay     := FieldByName('PAID').AsString;
        Amount   := FieldByName('AMOUNT').AsFloat;
        Member   := FieldByName('MEMBER').AsInteger;
        FullName := FieldByName('FULL_NAME').AsString;
        // cash payment if FReceipt = 0 else it has already been paid
        if FPay = 'Y' then begin
           FieldByName('AMOUNT_PAID').AsFloat    :=  FieldByName('AMOUNT').AsFloat;
           FieldByName('AMOUNT_DUE').AsFloat     :=  0.00;
           FieldByName('EDIT_DATE'  ).AsDateTime :=  now;
           FieldByName('EXPIRES'    ).AsDateTime :=
              Global.AlterExpireDateAbove(FieldByName('EXPIRES').AsDateTime,SectionGoodDate);
           s := formatDatetime('mm/dd/yyyy',FieldByName('EXPIRES').AsDateTime);
           PaidByName := ibdsPaySecMem.FieldByName('FULL_NAME').AsString;
           PaidBy   := ibdsPaySecMem.FieldByName('MEMBER').AsInteger;
           if PointsUsed > 0 then
              AmountLessPoints := Amount - PointsUsed
           else
              AmountLessPoints := Amount;
           if FSectionReceipt = 0 then begin // get receipt
             FReceipt  := MemberPayments.GetReceiptNumber(sqlTransPay);
             FieldByName('RECEIPT'    ).AsInteger  :=  FReceipt;
           end else begin    // use this one - already tendered
             FReceipt  := FSectionReceipt;
           end;
           // add all details

           with dm.sqlTransPay do begin
              // Payments
              sqlExecQuery(sqlTransPay,format(SQL1,[AmountLessPoints,PointsUsed,PaidBy,EDate,Rounding,PaidByName,FReceipt]));
              // Payments_det
              sqlExecQuery(sqlTransPay,format(SQL2,[FReceipt,SName,s,Amount,Amount,Member,SDate,FullName,PaidBy]));
              // Payments_tend - if hasnt been paid by tenders button
              // assumes Cash as this is the preffered option
              if FSectionReceipt = 0 then begin
                 sqlExecQuery(sqlTransPay,format(SQL3,[FReceipt,AmountLessPoints]));
              end;
              // if points Used  - update the member
              if PointsUsed > 0 then begin
                 ibdsPaySecMem.Edit;
                 ibdsPaySecMem.FieldByName('POINTS_AVAILABLE').AsFloat :=
                    ibdsPaySecMem.FieldByName('POINTS_AVAILABLE').AsFloat - PointsUsed;
                 ibdsPaySecMem.FieldByName('TOTAL_REDEEMED').AsFloat :=
                    ibdsPaySecMem.FieldByName('TOTAL_REDEEMED').AsFloat + PointsUsed;

                 ibdsPaySecMem.FieldByName('EDIT_DATE').AsDateTime := now;

                 ibdsPaySecMem.Post;
              end;
              with frmMembersPAY do begin
                 stbStatus.Panels[0].Text := Format('Last Receipt %d',[FReceipt]);
                 SectionReceiptList.Add(IntToStr(FReceipt));
              end;
           end;
        end;
        if FPay = 'N' then begin // credit it
           FieldByName('AMOUNT_DUE').AsFloat    :=  FieldByName('AMOUNT').AsFloat;
           FieldByName('AMOUNT_PAID').AsFloat    :=  0.00;
           FieldByName('EDIT_DATE'  ).AsDateTime :=  now;
           FieldByName('EXPIRES'    ).AsDateTime :=
              Global.AlterExpireDateBelow(FieldByName('EXPIRES').AsDateTime,SectionGoodDate);
           s := GetShortDate(FieldByName('EXPIRES').AsDateTime);
           FSectionReceipt   := FieldByName('RECEIPT').AsInteger;
           FReceipt  := MemberPayments.GetReceiptNumber(sqlTransPay); // assign for credits
           FieldByName('RECEIPT' ).AsInteger  :=  0;
           with dm.sqlTransPay do begin
              // get points if Used and paid by - these must be put back to the member
              sqlExecQueryCheckEOF(sqlTransPay,format(SQL4,[FSectionReceipt]));
              PaidBy     := FieldByName('PAID_BY').AsInteger;
              PointsUsed := FieldByName('POINTS_USED').AsFloat;
              if PointsUsed > 0.00 then
                 AmountLessPoints := Amount - (PointsUsed / 100)
              else
                 AmountLessPoints := Amount;
              //Rounding := FieldByName('ROUNDING').AsFloat;
              Rounding   := 0.00;
              Amount     := Amount - (Amount * 2);
              PointsUsed := PointsUsed - (PointsUsed * 2);
              // Payments
              sqlExecQuery(sqlTransPay,format(SQL1,[Amount,PointsUsed,PaidBy,EDate,Rounding,FullName,FReceipt]));
              // Payments_det
              sqlExecQuery(sqlTransPay,format(SQL2,[FReceipt,SName,s,abs(Amount),Amount,Member,SDate,FullName,PaidBy]));
              // Payments_tend - Credit back by Cash
              sqlExecQuery(sqlTransPay,format(SQL3,[FReceipt,Amount]));
              // if points Used  - update the member
              PointsUsed := abs(PointsUsed);
              if PointsUsed > 0 then begin
                 sqlExecQuery(sqlTransPay,format(SQL5,[PointsUsed,PointsUsed,EDate,Member]));
              end;
              with frmMembersPAY do begin
                 stbStatus.Panels[0].Text := Format('Last Receipt %d',[FReceipt]);
                 SectionReceiptList.Add(IntToStr(FReceipt));
              end;   
           end;
        end;
        EnableControls;
        frmMembersPAY.Blocked := False;
     end;
end;
{ **************************************************************************** }
procedure Tdm.ibdsPaySectionsAfterPost(DataSet: TDataSet);
begin
     // make sure points vars get reset
     with frmMembersPAY do begin
        Blocked := True;
        // new settings once the record has changed
        if ibdsPaySections.FieldByName('PAID').AsString = 'Y' then begin
           cbxUsePointsS.Checked     := False;
           dblcSectionPaidBy.Enabled := False;
           cbxUsePointsS.Enabled     := False;
        end else begin
           dblcSectionPaidBy.Enabled := True;
           cbxUsePointsS.Enabled     := True;
        end;
        ibdsPaySecMem.Close; // refresh
        ibdsPaySecMem.Open;
        dblcSectionPaidBy.Text := ibdsPaySecMem.FieldByName('MEMBER').AsString;
        SectionReceipt := 0;
        PointsUsedSection := 0.00;
        RoundValue := 0.00;
        Blocked := False;
     end;
end;
{ **************************************************************************** }
procedure Tdm.LogProcess ( theSQL : TIBSql; Msg : String );
const SQL1 = 'INSERT INTO PRCSS_LOG (PRCSSLOGSEQ,USER_CODE,MSG,PROCESS,EDIT_DATE) ' +
                'Values (0,%d,''%s'',%d,''%s'')';
begin
     sqlExecQuery(theSQL,Format(SQL1,[Global.UserCode,Msg,ProcessType,GetShortDateTime(now)]));
end;
{ **************************************************************************** }
function Tdm.LogExport ( theSQL : TIBSQL ; LogMember : LongInt ; PointsUsed : Integer ; ResetPoints : Boolean) : Boolean;
const SQL1 = 'SELECT COUNT(*) FROM EXPORT_AREV where MEMBER = %d and EXPORTED = ''N'' ' +
             'and IN_TRANS = ''N'' and POINTS_USED = 0';
      SQL2 = 'INSERT INTO EXPORT_AREV ( EXPORT_DATE, MEMBER, EXPORTED,IN_TRANS,POINTS_USED,RESET_POINTS ) ' +
             ' VALUES (''%s'',%d,''N'',''N'',%d,''%s'')';
{ if UniqueExportArevMsg then it's OK because it gets all details for the
  Member when the Export actually Happens - only one entry per Member 

  if PointsUsed <> 0.00 then it is a Points Payment or Manual Adjustment 
  and AREV need to know about it}
var LogIt : Boolean;  
begin
     // allows for a system that is stand alone 
     if (ControllerPCName = 'None') or (ControllerPCName = '') then begin
        Result := True;
        exit;
     end;   
     //try
     Result := False;
     LogIt := False;
     // check for Member that hasnt been exported yet 
     if (PointsUsed = 0) and not ResetPoints then begin
        if not sqlExecQueryCheckEOFCount(theSQL,Format(SQL1,[LogMember])) then LogIt := True;
     end else begin
     // if a Points Adjustment - by a Payment or Manual Adjustment log each one
        LogIt := True;
     end;
     if LogIt then   
        sqlExecQuery(theSQL,Format(SQL2,[GetShortDateTime(now),LogMember,PointsUsed,BooleanStr(ResetPoints)]));
        //sqlExecQuery(theSQL,Format(SQL2,[GetShortDateTime(now),LogMember,PointsUsed]));
     Result := True;
        
     //except             
     //   sm('Cannot Insert into table Export_Arev')
     //end;   
end;
{ **************************************************************************** }
procedure Tdm.tblEventsBeforePost(DataSet: TDataSet);
// for the Process log db
begin
     ProcessType := ord(TProcessTypes(ptMaintain));
     if DataSet.Name = tblEvents.Name then
        MsgBeforePostTBL( tblEvents,'EVENT');
     if DataSet.Name = tblSections.Name then
        MsgBeforePostTBL( tblSections,'SECTION');
     if DataSet.Name = tblGroups.Name then
        MsgBeforePostTBL( tblGroups,'GROUPS');
     if DataSet.Name = tblMailCodes.Name then
        MsgBeforePostTBL( tblMailCodes,'MAIL_CODE');
     if DataSet.Name = tblNumbers.Name then begin
        if pos(' ',tblNumbers.FieldByName('DESCRIPTION').AsString) > 0 then begin
           sm('The Description cannot have a space in it - use "_"');
           abort;
        end;
        MsgBeforePostTBL( tblNumbers,'NUMBER_ID');
     end;
     if DataSet.Name = tblClubs.Name then
        MsgBeforePostTBL( tblClubs,'CLUB_ID');
     if DataSet.Name = tblCardMember.Name then
        MsgBeforePostTBL( tblCardMember,'MEMBER');
end;                                       
{ **************************************************************************** }
procedure Tdm.ibdsM_EventsBeforePost(DataSet: TDataSet);
// for the Process log db   - before delete looks here as well
const  SQL1  = 'select Count(MEMBER) from M_SUBS where MEMBER = %d';
begin
     ProcessType := ord(TProcessTypes(ptMembersLinks));
     if DataSet.Name = ibdsM_Events.Name then begin
        MsgBeforePostDS( ibdsM_Events,'EVENT');
        if ibdsM_Events.FieldByName('DATES').AsDateTime = StrToDate(StrToDate1899) then
           ibdsM_Events.FieldByName('DATES').Clear;
        if FBeforeEditDOBEvent <> ibdsM_Events.FieldByName('DATES').AsDateTime then 
           LogExport(qryGeneral,ibdsM_Events.FieldByName('MEMBER').AsInteger,0,False);
     end;                                                                          
     if DataSet.Name = ibdsM_Sections.Name then
        MsgBeforePostDS( ibdsM_Sections,'SECTION');
     if DataSet.Name = ibdsM_Groups.Name then begin
        MsgBeforePostDS( ibdsM_Groups,'GROUPS');
         // Export for Arev (Groups Added) 11/02/2002
        if dsrM_Groups.State = dsInsert then begin
           if not LogExport(qryGeneral,ibdsMembers.FieldByName('MEMBER').AsInteger,0,False) then begin
              sm(FailedToLogExportMsg);
              abort;
           end;
        end;

     end;
     if DataSet.Name = ibdsM_Interests.Name then begin
        MsgBeforePostDS( ibdsM_Interests,'SECTION');
       
     end;
     if DataSet.Name = ibdsM_Discounts.Name then begin
        MsgBeforePostDS( ibdsM_Discounts,'DISCOUNT');
     end;
     //if DataSet.Name = ibdsPointsAllocation.Name then begin
      //  MsgBeforePostDS( ibdsPointsAllocation,'SECTION');

    // end;
    
     if DataSet.Name = ibdsM_Subs.Name then begin
         {if sqlExecQueryCheckEOF(qryGeneral,format(SQL1,[ibdsMembers.FieldByName('MEMBER').AsInteger])) then begin
            if ( qryGeneral.Fields[0].AsInteger > 0) and
               ( tblSubs.FieldByName('KEEP_EXPIRES_CURRENT').AsString = 'Y') then begin
               sm(KeepExpiresCurrentMsg);
               ibdsM_Subs.Cancel;
               Abort;
           end;
        end;  }
        MsgBeforePostDS( ibdsM_Subs,'SUBSCRIPTION');
         // Export for Arev
        if dsrM_Subs.State = dsInsert then begin
           if not LogExport(qryGeneral,ibdsMembers.FieldByName('MEMBER').AsInteger,0,False) then begin
              sm(FailedToLogExportMsg);
              abort;
           end;
        end;
     end;
end;
{ **************************************************************************** }
procedure Tdm.MsgBeforePostTBL( FromTable : TIBTable ; tblMsg : string);
var s : string[20];
// for the Process log db
begin
      with FromTable do begin
         case state  of
         dsEdit   : s := MChg;
         dsInsert : s := MAdd;
         else       s := MDel;
         end;
         if FromTable.Name = tblCardMember.Name then
            Msg := tblMsg + ' ' + FieldByName(tblMsg).AsString + ' ' + s + ' for a Card'
         else
            Msg := tblMsg + ' ' + FieldByName(tblMsg).AsString + ' ' +
              FieldByName('DESCRIPTION').AsString + s;
     end;
end;
{ **************************************************************************** }
procedure Tdm.MsgBeforePostDS( FromDS : TIBDataSet ; tblMsg : string);
var s : string[20];
// for the Process log db
begin
      with FromDS do begin
         case state  of
         dsEdit   : s := MChg;
         dsInsert : s := MAdd;
         else       s := MDel;
         end;
         if FromDS.Name = ibdsM_Discounts.Name then begin
            Msg := tblMsg + ' ' + FieldByName('DISCOUNT_KEY').AsString + ' ' +
               dmMM.tblDiscounts.FieldByName('DESCRIPTION').AsString + ' ' + s + ' for Member ' +
               FieldByName('MEMBER').AsString;
         end else begin
            Msg := tblMsg + ' ' + FieldByName(tblMsg).AsString + ' ' +
               FieldByName('DESCRIPTION').AsString + ' ' + s + ' for Member ' +
               FieldByName('MEMBER').AsString;
         end;
      end;
end;
{ **************************************************************************** }
procedure Tdm.tblTendersAfterPost(DataSet: TDataSet);
begin
     ProcessType := ord(TProcessTypes(ptMaintain));
     with tblTenders do begin
        LogProcess(sqlTransPay, 'Tender ' + FieldByName('TENDER_TYPE').AsString + ' ' +
                  FloatToStrF(FieldByName('EX_RATE').Asfloat,ffFixed,4,3) + ' Changed');
     end;
     if IBTransPAY.InTransaction then begin
        IBTransPAY.Commit;
        tblTenders.Open;
     end;
     IBCommitRetain;
end;
{ **************************************************************************** }
procedure Tdm.tblScreensAfterPost(DataSet: TDataSet);
begin
     ProcessType := ord(TProcessTypes(ptSystem));
     with tblScreens do begin
        dm.LogProcess( qryGeneral,'Screen Defaults ' + FieldByName('SCREEN_DEF').AsString + ' ' +
                  ' changed to ' + FieldByName('DEF_ACCESS').AsString);
     end;
end;
{ **************************************************************************** }
procedure Tdm.dsrLettersStateChange(Sender: TObject);
begin
     if frmMembersLetter = nil then exit;
     if not frmMembersLetter.Active then exit;
     if not assigned( frmMembersLetter.stbStatus) then exit;
     if TDataSource(Sender).Name = dsrLetters.Name then
        dsrStateChange(dsrLetters,frmMembersLetter)
     else   
        dsrStateChange(dsrQueries,frmMembersLetter);
end;
{ **************************************************************************** }
procedure Tdm.ibdsLettersAfterPost(DataSet: TDataSet);
begin
     IBCommitRetain;
     with frmMembersLetter do begin
        UpdateComboBox;
       // GetDatafromLetter( dbcbLetters.Items.Count -1 );
     end;
end;
{ **************************************************************************** }
procedure Tdm.ibdsLettersBeforePost(DataSet: TDataSet);
const SQL1 = 'Select NAME from LETTERS where NAME = ''%s''';
begin
     if ibdsLetters.State = dsInsert then begin
        if sqlExecQueryCheckEOF(qryGeneral,format(SQL1,[frmMembersLetter.dbeName.Text ])) then begin
           sm('You must enter a Name that is not currently used ');
           frmMembersLetter.dbeName.Text := '';
           abort;
        end;
     end;
end;
{ **************************************************************************** }
procedure Tdm.ibdsM_SubsBeforeDelete(DataSet: TDataSet);
//const SubsMsg = 'You must have at least one Subscription for this Member';
//      SQL1    = 'select Count(MEMBER) from M_SUBS where MEMBER = %d';
begin
     {if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL1,[ibdsMembers.FieldByName('MEMBER').AsInteger])) then begin
        if dm.qryGeneral.Fields[0].AsInteger < 2 then begin
           sm(SubsMsg);
           Abort;
        end;
     end; }
     if not IfMsgDlg(format('Delete Subscription %s?',[DataSet.FieldByName('SUBSCRIPTION').AsString])) then abort;
     ProcessType := ord(TProcessTypes(ptMembersLinks));
     if not LogExport(qryGeneral,ibdsMembers.FieldByName('MEMBER').AsInteger,0,False) then begin
        sm(FailedToLogExportMsg);
        abort;
     end;
     if DataSet.Name = ibdsM_Subs.Name then begin
        // delete the appropriate Group for the associated sub
        if SubsGroupDelete then begin
           ibdsM_Groups.Filtered := False;
           ibdsM_Groups.Filter   := format('GROUPS = %s',[ibdsM_Subs.FieldByName('SUB_GROUPS').AsString]);
           ibdsM_Groups.Filtered := True;
           if not ibdsM_Groups.EOF then begin
              ibdsM_Groups.Delete;
           end;
           ibdsM_Groups.Filtered := False;
        end;
        MsgBeforePostDS( ibdsM_Subs,'SUBSCRIPTION');
     end;
end;
{ **************************************************************************** }
procedure Tdm.ibdsM_EventsBeforeDelete(DataSet: TDataSet);
begin
     if ibdsM_Events.FieldByName('EVENT').AsInteger = 100 then begin
        sm('You cannot delete the Birthday Event from this Member');
        abort;
     end;
     ProcessType := ord(TProcessTypes(ptMembersLinks));
     if DataSet.Name = ibdsM_Events.Name then
        MsgBeforePostDS( ibdsM_Events,'EVENT');
end;
{ **************************************************************************** }
function Tdm.GetUserNames : Boolean;
var i,j : shortint;
begin
     // already read once
     if Global.arrayUserNames[0] <> '' then begin
        result := true;
        exit;
     end;
     result := false;
     // win 95 DNS - fill with defaults - cannot access Services
     if Global.DisableServices then begin
        ArrayUserNames[global.CountArrayUserNames] := Global.DefaultUserName;
        ArrayUserCodes[global.CountArrayUserNames] := 0;
        Global.CountArrayUserNames := 1;
        Global.HighestUserCode := 0;
        exit;
     end;
     with IBSecurityService do begin
       Active := False;
       ServerName := regServerName;
       Username   := DefaultUserName;
       Password   := DefaultPassword;
       Active := True;
       try
         DisplayUsers;
         j := 0;
         for I := 0 to UserInfoCount - 1 do begin
            with UserInfo[i] do begin
              if UserInfo[i].UserName = MasterUserName then continue; // dont need to see this
              Global.arrayUserNames[j] := UserName;   
              Global.arrayUserCodes[j] := UserId;
              inc(j);
            end;
         end;
       finally
         Active := False;
         Global.CountArrayUserNames := j -1;
         Global.HighestUserCode := 0;
         for i := 0 to Global.CountArrayUserNames do begin
            if Global.arrayUserCodes[i] > Global.HighestUserCode then
               Global.HighestUserCode := Global.arrayUserCodes[i];
         end;
       end;
    end;
    result := true;
end;
{ **************************************************************************** }
procedure Tdm.ibdsM_EventsBeforeEdit(DataSet: TDataSet);
// change DOB in the Maintenance tab and not the grid
begin
     // removed for Howick - 02/05/03
     //if ibdsM_Events.FieldByName('EVENT').AsInteger = 100 then begin
     //   MessageDlg('Change this field from the Date of Birth field' + #13#10 +
     //              ' <--  in the Proposer tab ',mtInformation,[mbOK],0);
     //   abort;
     //end;
     FBeforeEditDOBEvent := ibdsM_Events.FieldByName('DATES').AsDateTime;
end;
{ **************************************************************************** }
procedure Tdm.ResetMemberNumber;
// puts the number back only if no one else has used the next number 
var tempMember : longint;
const SQL1 = 'select NEXT_MEMBER from M_NUMBERS where NUMBER_ID = %d';
      SQL2 = 'update M_NUMBERS SET NEXT_MEMBER = %d where NUMBER_ID = %d';
begin
     if sqlExecQueryCheckEOF(qryGeneral,format(SQL1,[Global.NextMemberIndex])) then begin
        tempMember := qryGeneral.FieldByName('NEXT_MEMBER').AsInteger;
        if tempMember <= (FNextMember + 1) then begin// nobody else has used it
           dm.sqlExecQuery(dm.qryGeneral,format(SQL2,[FNextMember,Global.NextMemberIndex]));
           IBCommitRetain;
        end;   
     end;   
     
end;
{ **************************************************************************** }
procedure Tdm.ibdsMembersDefBeforePost(DataSet: TDataSet);
begin
     Msg := 'Members Defaults Changed';
     ProcessType := ord(TProcessTypes(ptMembers));
end;
{ **************************************************************************** }
procedure Tdm.ibdsMembersDefAfterPost(DataSet: TDataSet);
begin
     try
        if Msg <> '' then begin
           LogProcess(qryGeneral,Msg);            // Msg built up in before Post
           Msg := '';
        end;
        IBCommitRetain;
        ResetMemberDefault;
     except
        IBRollbackRetain;
        sm(MembersPostErrorMsg);
     end;
end;
{ **************************************************************************** }
procedure Tdm.ibdsMembersDefBeforeDelete(DataSet: TDataSet);
begin
     sm('You cannot delete or add from the Default Member');
     abort;
end;
{ **************************************************************************** }
procedure Tdm.ibdsMembersDefAfterCancel(DataSet: TDataSet);
begin
     ResetMemberDefault;
end;
{ **************************************************************************** }
procedure Tdm.ResetMemberDefault;
begin
     with frmMembersEdit do begin
        SetUpButtons(True);
        SettingMemberDefaults := False;
        pnlFind.Caption := '&Find';
        dsrMembers.DataSet := ibdsMembers;
     end;   
end;
{ **************************************************************************** }
procedure Tdm.IBEventsEventAlert(Sender: TObject; EventName: String;
  EventCount: Integer; var CancelAlerts: Boolean);
// Seems to only work on NT 
// Events get queued and come up with the correct MSG
// Waits on busy processes 
begin
     try 
        if not FSentFromHere then  // try not to notify self
           if sqlExecQueryCheckEOF(qryGeneral,'Select MSG from MISC') then
              MDlgI(qryGeneral.FieldByName('MSG').AsString);
     finally
        FSentFromHere := False;
     end;   
end; 
{ **************************************************************************** }
procedure Tdm.SendEvent( strMsg : shortstring );
// Seems to only work on NT
// MSG is always used
const SQL1 = 'Update MISC set MSG = ''%s''';     
begin
     try
        sqlExecQuery(qryGeneral,format(SQL1,[strMsg]));
        IBCommitRetain;
     except
        sm('Unable to update the MSG in table MISC');
        exit;
     end;   
     // no Params in StoredPROC - just does post_event 'MSG'
     with IBStoredProc do begin
        Prepare;
        ExecProc;
        dm.IBCommitRetain;
        FSentFromHere := True;
      end;
end;

{ **************************************************************************** }
{ Rules for below 5 functions
  1) if enclosed in a try - except block - the calling process handles the error
     and any other graceful exits
  2) else the exception is raised in the function and execution stops }  
function Tdm.sqlExecQueryCheckEOF( theSQL : TIBSql ; sqlString : String ) : Boolean;
// use with SELECT  - checks for EOF
begin
     with theSQL do begin
        close;
        SQL.TEXT := sqlString;
        //AddToClipBoard(SQL.Text);
        Execquery;
        if EOF then 
           result := False
        else
           Result := True;
     end;
end;
{ **************************************************************************** }
function Tdm.sqlExecQueryCheckEOFCount( theSQL : TIBSql ; sqlString : String ) : Boolean;
// use with SELECT  - checks for EOF and COUNT = 0
begin
     with theSQL do begin
        close;
        SQL.TEXT := sqlString;
        //AddToClipBoard(SQL.Text);
        Execquery;
        if EOF then
           result := False
        else
           if Fields[0].AsInteger = 0 then                                                
              result := False
           else
              result := True;
     end;
end;                                     
{ **************************************************************************** }
function Tdm.sqlExecQuery( theSQL : TIBSql ; sqlString : String ) : Boolean;
// use with DELETES UPDATES and INSERTS 
begin
     Result := False;
     with theSQL do begin
        close;                              
        SQL.TEXT := sqlString;
      // AddToClipBoard(SQL.Text);
        Execquery;
     end;
     Result := True;
end;
{ **************************************************************************** }
function Tdm.qryOpen( theQry : TIBQuery ; sqlString : String ) : Boolean;
// use with DELETES UPDATES and INSERTS
begin
     Result := False;
     with theQry do begin
        close;
        SQL.TEXT := sqlString;
        //AddToClipBoard(SQL.Text);
        Open;
     end;   
     Result := True;
end;
{ **************************************************************************** }
function Tdm.qryOpenCheckEOF( theQry : TIBQuery ; sqlString : String ) : Boolean;
// use with SELECT  - checks for EOF
begin
     with theQry do begin
        close;
        SQL.TEXT := sqlString;
        //AddToClipBoard(SQL.Text);
        Open;
        if EOF then
            result := False
        else
            Result := True;
     end;       
end;
{ **************************************************************************** }
function Tdm.ibdsOpenCheckEOF( theDataSet : TIBDataSet ; sqlString : String ) : Boolean;
// use with SELECT  - checks for EOF
begin
     with theDataSet do begin
        close;
        SelectSQL.TEXT := sqlString;
        //AddToClipBoard(SelectSQL.Text);
        Open;
        if EOF then
            result := False
        else
            Result := True;
     end;       
end;
{ **************************************************************************** }
procedure Tdm.tblNumbersAfterInsert(DataSet: TDataSet);
begin
      tblNumbers.FieldByName('NUMBER_ID').AsInteger := LastNumberPlusOne( NumbersTableName,'NUMBER_ID');
end;
{ **************************************************************************** }
{ ****************************************************************************}
procedure Tdm.tblVersionsAfterPost(DataSet: TDataSet);
begin
     ProcessType := ord(TProcessTypes(ptSystem));
     dm.LogProcess( qryGeneral,'VERSIONS PC name ' + tblVersions.FieldByName('PC_NAME').AsString + 
                  ' details changed ');
end;
{ **************************************************************************** }
function Tdm.CheckForStopFiles(const ArevStopFile : String; const ExportNumberOfFiles : ShortInt): Boolean;
begin
     // if AREV STOP file EXISTS then AREV is processing so exit;
     result := False;
     case ExportNumberOfFiles of
        1 : if FileExists(ArevImportFilePath + ArevStopFile) then begin
               inc(FArevStopFileCtr);
               Result := True;
            end;   
        2 : if (FileExists(ArevImportFilePath + ArevStopFile)) or (FileExists(ArevSecondFilePath + ArevStopFile)) then begin
               inc(FArevStopFileCtr);
               Result := True;
            end;   
     end;
     
end;
{ ****************************************************************************}
procedure Tdm.DeleteExportFiles(const WinStopFile,WinMemberFile,Msg,TmpWinMemberFile : String);
begin
     ExportTimer.Enabled := False;
     DeleteFile(ArevImportFilePath + WinStopFile); 
     DeleteFile(ArevSecondFilePath + WinStopFile); 

     DeleteFile(ArevImportFilePath + WinMemberFile); 
     DeleteFile(ArevSecondFilePath + WinMemberFile);
     // trys to restore the backup TmpWinMemberFile to WinMemberFile if it was already there
     // a copy was made in CheckCreateFile and deleted at the top of ExportTimerTimer
     if FileExists(ArevImportFilePath + TmpWinMemberFile) then
        RenameFile(ArevImportFilePath + TmpWinMemberFile,ArevImportFilePath + WinMemberFile);
     if FileExists(ArevSecondFilePath + TmpWinMemberFile) then
        RenameFile(ArevSecondFilePath + TmpWinMemberFile,ArevSecondFilePath + WinMemberFile);
     sm('There has been an error when creating the Export Files for Clubmaster and Exporting has been disabled' + #13#10 + msg);
end;
{ ****************************************************************************}
procedure Tdm.ExportTimerTimer(Sender: TObject);
{ Create 1 file to ArevImportFilePath WINMEMBER
  The Record can only be a Change or Delete and all the relevant details
  are got and written into the record.
  First set IN_TRANS to Y where EXPORTED = 'N' and commit
  Then SELECT where IN_TRANS = Y and EXPORTED = 'N' - this ensures that
    if a Member if changed during this process then
    it will be written to the file ( because it will set IN_TRANS to 'N' )
    and the next time it passes thru then it will pick up the latest details.
  Then set EXPORTED to 'Y' - so it wont be picked up again

  Delphi -> write WIN  + write/append WINMEM.DAT then delete WIN        <- Arev cant process 
  Arev   -> write AREV + read WINMEMBER  then delete AREV + WINMEM.DAT  <- Delphi cant process}
const SQL1 = 'UPDATE EXPORT_AREV set IN_TRANS = ''Y'' where IN_TRANS = ''N''';  
      SQL3 = 'SELECT MEMBER,FULL_NAME,KNOWN_AS,FIRST_NAME,SURNAME,DATE_OF_BIRTH,IS_ACTIVE,SUB_PAID' +
             ',PRICE_LEVEL,VIP_PAY_ONLY,ALLOW_CREDIT,EARN_POINTS,CARD_TYPE,GL_ACCOUNT' +
             ',CREDIT_LIMIT,DISCOUNT,INCL_EOD,EXPIRES,POINTS_AVAILABLE,TOTAL_REDEEMED,' +
             'TOTAL_EARNT,PRE_PAID_CREDIT,BONUS_CREDIT,EARNT_CREDIT,' +
             'PRE_PAID_CREDIT + BONUS_CREDIT + EARNT_CREDIT as VALUE_OF_CREDIT ' +
             'from MEMBERS where MEMBER = %s';
      SQL4 = 'SELECT M.SUBSCRIPTION,S.DESCRIPTION,S.FEE from M_SUBS M, SUBSCRIPTIONS S' +
             ' where M.SUBSCRIPTION = S.SUBSCRIPTION and M.MEMBER = %s';
      SQL5 = 'SELECT GROUPS from M_GROUPS where MEMBER = %s';
      SQL6 = 'SELECT DATES from M_EVENTS where MEMBER = %s and EVENT = 100';
      WinMemberFile = '\WINMEM.DAT';
      TmpWinMemberFile = '\WINMEM.TMP';
      WinStopFile   = '\WIN';
      ArevStopFile  = '\AREV';
      comma = ',';
      Msg1 = 'Member Information is not being transferred - do you wish to '  + #13#10 +
             'stop transfering information and resolve it later?';
var ExportMember : String[12];
    SubStr,FeeStr,DescStr,GLAccount : ShortString;
    ExportFile,SecondExportFile : TextFile;    
    s,Msg : string;
    SubPaid : string[1];
    CreditLimit,Discount,Groups : String[8];
    DOB : String[14];
    ExportNumberOfFiles : ShortInt;
begin
     // warning if the WinMember File if there repeatedly - appended to rather than rewrite
     // NB only adds if there are records to send
     { if WinMemberFileCtr > 19 then begin // ?? minutes depending on usage
        //sm(format( '%s , %s %s',[Msg1,HelpDeskMsg,HelpDesk]));
        if ifMsgDlg(format( '%s has been there for %d times %s',[WinMemberFile,WinMemberFileCtr,Msg1]) then TemporaryStopTransfer;
        WinMemberFileCtr := 0;
     end; }
     // warning if the ArevStopFile if there repeatedly - ie AREV has not deleted it
     // checked each time thru this process
     if FArevStopFileCtr > 30 then begin
        //PExporting := True;
        ExportTimer.Enabled := False;
        if ifMsgDlg(format( '%s has been there for %d times %s',[ArevStopFile,FArevStopFileCtr,Msg1])) then
           ExportTimer.Enabled := False
        else
           ExportTimer.Enabled := True;
        FArevStopFileCtr := 0;
     end;
     ExportNumberOfFiles := 1; // Default
     if ArevSecondFilePath <> '' then 
        ExportNumberOfFiles := 2;
     // Pre checks for StopFiles put out by Arev to say that it is processing 
     if CheckForStopFiles(ArevStopFile,ExportNumberOfFiles) then exit;
     // reset if not there
     FArevStopFileCtr := 0;
     if PExporting then exit;
     // dont wont to re-fire again until this process has finished
     PExporting := True;
     { delete the backup copied ascii files from last time ( if there was one and were no errors )
     - if the file exists in CheckCreateFile then a backup copy is made - for rollback reasons }
     DeleteFile(ArevImportFilePath + TmpWinMemberFile);
     if ExportNumberOfFiles = 2 then 
        DeleteFile(ArevSecondFilePath + TmpWinMemberFile); 
     if not IBExport.InTransaction then
        IBExport.StartTransaction;
     // set IN_TRANS to Y
     try
        sqlExecQuery(sqlExport,SQL1);
        // commit so that others can see it and ibdsExport can pick up
        IBExport.Commit;
        IBExport.StartTransaction;
     except
        IBExport.RollBack;
        PExporting := false;
        Exit;
     end;
     try
        try
           // if any part below fails - the records will be picked up next time
           // because EXPORTED = N
           with ibdsExport do begin
              Close;
              ParamByName('EXPORTED').AsString := 'N';
              Open;
              FetchAll;
              // no records to process;
              if RecordCount = 0 then begin
                 IBExport.Commit;
                 exit;
              end;
              // check for stop files again just before creating mine
              if CheckForStopFiles(ArevStopFile,ExportNumberOfFiles) then begin
                 IBExport.Commit;
                 exit;   
              end;   
              msg := '';
              // create the Win Stop file 
              if not CreateFileAndClose( ArevImportFilePath + WinStopFile ) then begin
                 msg := 'Cannot Create ' + ArevImportFilePath + WinStopFile;                 
                 DeleteExportFiles(WinStopFile,WinMemberFile,Msg,TmpWinMemberFile);
                 IBExport.Commit;
                 exit;
              end;   
              // create the Second Win Stop file 
              if ExportNumberOfFiles = 2 then begin
                 if not CreateFileAndClose( ArevSecondFilePath + WinStopFile ) then begin
                    msg := 'Cannot Create ' + ArevSecondFilePath + WinStopFile;                 
                    DeleteExportFiles(WinStopFile,WinMemberFile,Msg,TmpWinMemberFile);
                    IBExport.Commit;
                    exit;
                 end;   
              end;   
              { create or append to the WINMEMBER file for AREV
               Append to the file if AREV import is not working
               and once reset it will pick up the file and apply it  }
              if not CheckCreateFile( ExportFile,ArevImportFilePath + WinMemberFile,ArevImportFilePath + TmpWinMemberFile ) then begin
                 msg := 'Cannot Create ' + ArevImportFilePath + WinMemberFile;                 
                 DeleteExportFiles(WinStopFile,WinMemberFile,Msg,TmpWinMemberFile);
                 IBExport.Commit;
                 exit;
              end;   
              if ExportNumberOfFiles = 2 then begin
                 if not CheckCreateFile( SecondExportFile,ArevSecondFilePath + WinMemberFile,ArevSecondFilePath + TmpWinMemberFile ) then begin 
                    msg := 'Cannot Create ' + ArevSecondFilePath + WinMemberFile;                 
                    CloseFile(ExportFile);
                    DeleteExportFiles(WinStopFile,WinMemberFile,Msg,TmpWinMemberFile);
                    IBExport.Commit;
                    exit; 
                 end;   
              end;   
              First;
              s := '';
              while not EOF do begin
                 ExportMember := FieldByName('MEMBER').AsString;
                 // Papatoetoe/Shirley Must have Members < 4 prefixed with zeros
                 if (Global.CompName = 'Papatoetoe Cosmopolitan Club') or
                    (Global.CompName = 'St Albans Shirley Club') then begin
                    case Length(ExportMember) of
                      1 : ExportMember := '000' + ExportMember;
                      2 : ExportMember := '00' + ExportMember;
                      3 : ExportMember := '0' + ExportMember;
                    end;
                 end;
                 // currently in the file - use qryGeneral to get the data from MEMBERS
                 if dm.sqlExecQueryCheckEOF(qryGeneral,format(SQL3,[ExportMember])) then begin
                    if StrToBoolean(FieldByName('RESET_POINTS').AsString) then begin
                       // Reset Points and Credit to what the member has currently
                       s := 'R' + comma +  ExportMember
                                + comma +  FormatFloat('#####0',qryGeneral.FieldByName('TOTAL_EARNT').AsFloat      * 100)
                                + comma +  FormatFloat('#####0',qryGeneral.FieldByName('TOTAL_REDEEMED').AsFloat   * 100)
                                + comma +  FormatFloat('#####0',qryGeneral.FieldByName('POINTS_AVAILABLE').AsFloat * 100)
                                + comma +  FormatFloat('#####0',qryGeneral.FieldByName('PRE_PAID_CREDIT').AsFloat  * 100)
                                + comma +  FormatFloat('#####0',qryGeneral.FieldByName('BONUS_CREDIT').AsFloat     * 100)
                                + comma +  FormatFloat('#####0',qryGeneral.FieldByName('EARNT_CREDIT').AsFloat     * 100)
                                + comma +  FormatFloat('#####0',qryGeneral.FieldByName('VALUE_OF_CREDIT').AsFloat  * 100);
                       WriteLn(ExportFile,s);
                       if ExportNumberOfFiles = 2 then
                          WriteLn(SecondExportFile,s);
                    end else if FieldByName('POINTS_USED').AsInteger <> 0 then begin
                       // points only
                       //if (ArevPAth = ArevSecondFilePath) and (pos('RPNYC',CompName) > 0) then begin
                       s := 'P' + comma + ExportMember + comma + FieldByName('POINTS_USED').AsString;
                       WriteLn(ExportFile,s);
                       // dont write out Points to the restaurant  SecondExportFile
                    end else begin
                       // Member option
                       sqlExecQuery(sqlExport,format(SQL4,[ExportMember]));
                       SubStr  := '';
                       DescStr := '';
                       FeeStr  := '';
                       // Get all the Subscriptions
                       while not sqlExport.EOF do begin
                          SubStr  := SubStr  + sqlExport.FieldByName('SUBSCRIPTION').AsString;
                          DescStr := DescStr + sqlExport.FieldByName('DESCRIPTION').AsString;
                          FeeStr  := FeeStr  + FormatFloat('#####0',sqlExport.FieldByName('FEE').AsFloat * 100);
                          SubStr  := SubStr  + '*';
                          DescStr := DescStr + '*';
                          FeeStr  := FeeStr  + '*';              
                          sqlExport.Next;
                       end;
                       System.Delete(SubStr,length(subStr),1); // remove last *
                       System.Delete(DescStr,length(DescStr),1); // remove last *
                       System.Delete(FeeStr,length(FeeStr),1); // remove last *
                       // get the Event Date for Howick 02/05/2003
                       sqlExecQuery(sqlExport,format(SQL6,[ExportMember]));
                       if (sqlExport.FieldByName('DATES').IsNull) or 
                          (sqlExport.FieldByName('DATES').AsDateTime =  StrToDate(StrToDate1899)) then 
                          DOB := ''
                       else
                          DOB := formatdatetime('dd/mm/yyyy',sqlExport.FieldByName('DATES').AsDateTime);
                       with qryGeneral do begin
                          case FieldByName('SUB_PAID').AsInteger of 
                             0     : SubPaid := '0'; //spZero
                             1,2   : SubPaid := '1'; //spFull,spPart
                          end;
                          GLAccount := FieldByName('GL_ACCOUNT').AsString;
                          // remove the - from GLAccount
                          while pos('-',GLAccount) > 0 do 
                              System.Delete(GLAccount,pos('-',GLAccount),1);
                          if FieldByName('DISCOUNT').IsNull then
                             Discount := ''
                          else 
                             Discount := FormatFloat('####.00',FieldByName('DISCOUNT').AsFloat);
                          // bug fix 13/03/2002 in version 1.01.07
                          // get Groups separately in case they have none
                          if dm.sqlExecQueryCheckEOF(qryGeneral1,format(SQL5,[ExportMember])) then begin
                             if qryGeneral1.FieldByName('GROUPS').IsNull then
                                // Default to 100 if none there
                                Groups := '100'
                             else
                                Groups := qryGeneral1.FieldByName('GROUPS').AsString;
                          end else begin
                             // Default to 100 if none there
                             Groups := '100'
                          end;
                          if FieldByName('CREDIT_LIMIT').IsNull then 
                             CreditLimit := ''
                          else
                             CreditLimit := FormatFloat('#####0',FieldByName('CREDIT_LIMIT').AsFloat * 100);
                          s := 'N' + comma + ExportMember 
                                   + Comma + trim(FieldByName('FULL_NAME').AsString)
                                   + Comma + trim(FieldByName('KNOWN_AS').AsString)
                                   + Comma + SubStr 
                                   + Comma + DescStr
                                   + Comma + FeeStr                                                   
                                   + Comma + DOB // dd/mm/yy
                                   + Comma + ConvertToNumber(FieldByName('IS_ACTIVE').AsString)
                                   + Comma + SubPaid
                                   + Comma + IntToStr(FieldByName('PRICE_LEVEL').AsInteger)
                                   + Comma + ConvertToNumber(FieldByName('VIP_PAY_ONLY').AsString)
                                   + Comma + ConvertToNumber(FieldByName('ALLOW_CREDIT').AsString)
                                   + Comma + CreditLimit
                                   + Comma + ConvertToNumber(FieldByName('EARN_POINTS').AsString)
                                   + Comma + Discount
                                   + Comma + FieldByName('CARD_TYPE').AsString
                                   + Comma + GLAccount
                                   + Comma + ConvertToNumber(FieldByName('INCL_EOD').AsString)
                                   + Comma + Groups
                                   + Comma + FormatDateTime('dd/mm/yy',FieldByName('EXPIRES').asDateTime)
                                   + Comma + trim(FieldByName('FIRST_NAME').AsString)
                                   + Comma + trim(FieldByName('SURNAME').AsString);
                       end;
                       WriteLn(ExportFile,s);
                       if ExportNumberOfFiles = 2 then
                          WriteLn(SecondExportFile,s);
                    end;
                 // been deleted from the file
                 end else begin
                    // Reset the Points in Stockmater - the delete doenst delete the whole record
                    // so reset the points just in case
                    s := 'R,' + ExportMember + ',0' + ',0' + ',0' + ',0' + ',0' + ',0' + ',0';
                    WriteLn(ExportFile,s);
                    if ExportNumberOfFiles = 2 then
                       WriteLn(SecondExportFile,s);
                    // Delete the Member
                    s := 'D' + comma + ExportMember;
                    WriteLn(ExportFile,s);
                    if ExportNumberOfFiles = 2 then
                       WriteLn(SecondExportFile,s);
                 end;
                 Edit;
                 FieldByName('EXPORTED').AsString := 'Y';
                 Post;
                 Next;
              end;
              // I have got my stop file out at this point
              // write the second file first as it has no points
              if ExportNumberOfFiles = 2 then begin
                 Flush(SecondExportFile);
                 CloseFile(SecondExportFile); 
              end;   
              Flush(ExportFile);
              CloseFile(ExportFile); 
              // Remove Win Stop File
              if not DeleteFile(ArevImportFilePath + WinStopFile) then begin
                 msg := 'Cannot Delete ' + ArevImportFilePath + WinStopFile;                 
                 DeleteExportFiles(WinStopFile,WinMemberFile,Msg,TmpWinMemberFile);
                 exit;
              end;
              // Remove the second Win Stop File
              if ExportNumberOfFiles = 2 then begin
                 if not DeleteFile(ArevSecondFilePath + WinStopFile) then begin
                    msg := 'Cannot Delete ' + ArevSecondFilePath + WinStopFile;                 
                    DeleteExportFiles(WinStopFile,WinMemberFile,Msg,TmpWinMemberFile);
                    exit;
                 end; 
              end;
              if IBExport.InTransaction then
                IBExport.Commit;
           end; // with
        except
           On E : Exception do begin
              IBExport.Rollback;
              DeleteExportFiles(WinStopFile,WinMemberFile,E.Message,TmpWinMemberFile);
           end;  
        end;
     finally
         PExporting := False;
     end;
end;
{ **************************************************************************** }
procedure Tdm.tblCardMasterBeforeInsert(DataSet: TDataSet);
begin
     tblCardMaster.Refresh;
     FCardMaster := LastNumberPlusOne( 'CARD_MASTER','CARD_ID' );
end;
{ **************************************************************************** }
procedure Tdm.tblCardMasterAfterInsert(DataSet: TDataSet);
begin
     tblCardMaster.FieldByName('CARD_ID').AsInteger  := FCardMaster;
     tblCardMaster.FieldByName('CARD_NAME').AsString := '';
end;
{ **************************************************************************** }
procedure Tdm.tblCardDetailAfterInsert(DataSet: TDataSet);
begin
     with tblCardDetail do begin
        FieldByName('CARD_DETAIL_ID').AsInteger := FCardMaster;
        FieldByName('CARD_ID').AsInteger        := tblCardMaster.FieldByName('CARD_ID').AsInteger;
        FieldByName('FIELD_NAME').AsString      := '';
        FieldByName('HORIZONTAL').AsInteger     := 100;
        FieldByName('VERTICAL').AsInteger       := 100;
        FieldByName('ROTATION').AsInteger       := 0;
        FieldByName('STYLE').AsInteger          := 1;
        FieldByName('PROPORTION').AsInteger     := 0;
        FieldByName('SIZE').AsInteger           := 50;
        FieldByName('GRAPHIC_MODE').AsInteger   := 1;
     end;
end;
{ **************************************************************************** }
procedure Tdm.tblCardMasterBeforeDelete(DataSet: TDataSet);
const SQL1 = 'DELETE from CARD_DETAIL where CARD_ID = %d';
begin
     // delete all entries from the Card Detail
     sqlExecQuery(dm.qryGeneral,format(SQL1,[tblCardMaster.FieldByName('CARD_ID').AsInteger ]));
     // Refresh
     tblCardDetail.Close;
     tblCardDetail.Open;
     ProcessType := ord(TProcessTypes(ptMaintain));
     Msg := format('Card Master table %d %s deleted',
        [tblCardMaster.FieldByName('CARD_ID').AsInteger,tblCardMaster.FieldByName('CARD_NAME').AsString]);            
     
end;
{ **************************************************************************** }
procedure Tdm.tblCardMasterBeforePost(DataSet: TDataSet);
var s : shortstring;
begin
     ProcessType := ord(TProcessTypes(ptMaintain));
     case DataSet.state  of
        dsEdit   : s := MChg;
        dsInsert : s := MAdd;
     end;
     if DataSet.Name = tblCardMaster.Name then begin
        Msg := format('Card Master table %d %s %s',
           [tblCardMaster.FieldByName('CARD_ID').AsInteger,tblCardMaster.FieldByName('CARD_NAME').AsString,s]);           
     end else begin      
        Msg := format('Card Detail table %d %s %s',
           [tblCardDetail.FieldByName('CARD_ID').AsInteger,tblCardDetail.FieldByName('FIELD_NAME').AsString,s]);
     end;
end;
{ **************************************************************************** }
procedure Tdm.tblCardDetailBeforeDelete(DataSet: TDataSet);
begin
     ProcessType := ord(TProcessTypes(ptMaintain));
     msg := format('Card Detail table %d %s deleted',
        [tblCardDetail.FieldByName('CARD_ID').AsInteger,tblCardDetail.FieldByName('FIELD_NAME').AsString]);            
end;
{ **************************************************************************** }
procedure Tdm.ResetMembersDef;
begin
     //the parambyName is set before calling here
     with ibdsMembersDef do begin
        close;
        open;
     end;      
end;
{ **************************************************************************** }
procedure Tdm.tblCardMemberPostError(DataSet: TDataSet; E: EDatabaseError; var Action: TDataAction);
begin
     if (POS ( PrimaryKeyMsg ,e.Message) > 0) then begin
        sm(format(MemberCardMsg,[tblCardMember.FieldByName('MEMBER').AsInteger]));
        Action := daAbort;
     end;   
end;
{ **************************************************************************** }
procedure Tdm.ibdsM_LotsAfterInsert(DataSet: TDataSet);
begin
     with ibdsM_Lots do begin
        // pk MEMBER, SECTION
        FieldByName('MEMBER').AsInteger := ibdsMembers.FieldByName('MEMBER').AsInteger;
        FieldByName('LOT_ID').AsInteger := LastNumberPlusOne( 'LOTS','LOT_ID');
     end;
end;
{ **************************************************************************** }
procedure Tdm.DataModuleDestroy(Sender: TObject);
begin
     if assigned(FCustomDataSetList) then
        FCustomDataSetList.free;
     if assigned(FActiveCustomDataSetList) then
        FActiveCustomDataSetList.Free;
end;
{ **************************************************************************** }
procedure Tdm.ibdsM_InterestsAfterInsert(DataSet: TDataSet);
begin
    ibdsM_Interests.FieldByName('MEMBER').AsInteger  := ibdsMembers.FieldByName('MEMBER').AsInteger;
    ibdsM_Interests.FieldByName('SECTION').AsInteger := tblSections.FieldByName('SECTION').AsInteger;
    try
       ibdsM_Interests.Post;
    except
       on E : Exception do begin
          if pos( PrimaryKeyMsg, e.message ) > 0 then begin
             sm(format('Section %d has been already added',[tblSections.FieldByName('SECTION').AsInteger]));
             ibdsM_Interests.Cancel;
             abort;
          end else
             raise;
       end;
    end;
end;
{ **************************************************************************** }
procedure Tdm.ibdsQueriesBeforeInsert(DataSet: TDataSet);
begin
     FQueries := LastNumberPlusOne( QueriesTableName,'QUERIES_ID');
end;
{ **************************************************************************** }
procedure Tdm.ibdsQueriesAfterInsert(DataSet: TDataSet);
begin
     ibdsQueries.FieldByName('QUERIES_ID').AsInteger := FQueries;
end;
{ **************************************************************************** }
procedure Tdm.ibdsMembersAfterEdit(DataSet: TDataSet);
begin
     frmMembersEDIT.PropSecChange;
     // refiring on Points with the trigger
  
     
end;
{ **************************************************************************** }
procedure Tdm.ibdsM_GroupsBeforeDelete(DataSet: TDataSet);
begin
     // Pauanui - delete of Groups with golf in the word changes the Home Club back to 0
     FRemoveHomeClub := False;
     if (pos('GOLF',ibdsM_GROUPS.FieldByName('DESCRIPTION').AsString) > 0) and
        (ibdsMembers.FieldByName('CLUBS').AsInteger > 0) then begin
        if not IfMsgDlg('Delete Record and remove the "Home Club"?') then abort;
        FRemoveHomeClub := True;
     end else begin
        if not IfMsgDlg(format('Delete Group %s?',[DataSet.FieldByName('GROUPS').AsString])) then abort;
     end;
      // Export for Arev (Groups Deleted) 11/02/2002
     if not LogExport(qryGeneral,ibdsMembers.FieldByName('MEMBER').AsInteger,0,False) then begin
        sm(FailedToLogExportMsg);
        abort;
     end;
end;
{ **************************************************************************** }
procedure Tdm.tblClubsAfterInsert(DataSet: TDataSet);
begin
     tblClubs.FieldByName('CLUB_ID').AsInteger := LastNumberPlusOne( 'CLUBS','CLUB_ID');
end;
{ **************************************************************************** }
procedure Tdm.ibdsMembersBriefAfterPost(DataSet: TDataSet);
begin
      if not LogExport(qryGeneral,ibdsMembersBrief.FieldByName('MEMBER').AsInteger,0,False) then begin
         sm(FailedToLogExportMsg);
         IBRollBackRetain;
         abort;
      end;
      IBCommitRetain;
end;  
{ **************************************************************************** }
procedure Tdm.dsrMembersBriefStateChange(Sender: TObject);
begin
     dsrStateChange( dsrMembersBrief,frmMembersEditBrief );
end;
{ **************************************************************************** }
procedure Tdm.tblCardMemberAfterInsert(DataSet: TDataSet);
begin
     tblCardMember.FieldByName('NUMBER_CARDS').AsInteger := 1;
end;
{ **************************************************************************** }
procedure Tdm.ibdsMembersCalcFields(DataSet: TDataSet);
begin
     with DataSet do begin
       //FieldByName('POINTS_AVAILABLE').AsFloat :=
       //  FieldByName('TOTAL_EARNT').AsFloat - FieldByName('TOTAL_REDEEMED').AsFloat;
       FieldByName('VALUE_OF_CREDIT').AsFloat :=
         FieldByName('PRE_PAID_CREDIT').AsFloat + FieldByName('BONUS_CREDIT').AsFloat +
         FieldByName('EARNT_CREDIT').AsFloat;
     end;
end;
{ **************************************************************************** }
procedure Tdm.qryPointsLogCalcFields(DataSet: TDataSet);
begin
     ibdsMembersCalcFields(DataSet);
     with DataSet do begin
        //FieldByName('BEF_POINTS_AVAILABLE').AsFloat :=
        //  FieldByName('BEF_TOTAL_EARNT').AsFloat - FieldByName('BEF_TOTAL_REDEEMED').AsFloat;
        FieldByName('BEF_VALUE_OF_CREDIT').AsFloat :=
         FieldByName('BEF_PRE_PAID_CREDIT').AsFloat + FieldByName('BEF_BONUS_CREDIT').AsFloat +
         FieldByName('BEF_EARNT_CREDIT').AsFloat;
     end;
end;
{ **************************************************************************** }
procedure Tdm.ibdsPointsCalcFields(DataSet: TDataSet);
begin
     ibdsMembersCalcFields(DataSet);
end;
{ **************************************************************************** }
procedure Tdm.ibdsMailingLabelsAfterPost(DataSet: TDataSet);
begin
     IBTransaction.Commit;
     ibdsMailingLabels.Open;
end;
{ ****************************************************************************}
procedure Tdm.ibdsPointsAllocationBeforeDelete(DataSet: TDataSet);
begin
     ShowMessage('Cannot delete from Points Allocation');
     abort;
end;
{ ****************************************************************************}
procedure Tdm.ibdsPointsAllocationBeforeInsert(DataSet: TDataSet);
begin
     ShowMessage('Cannot Add to Points Allocation');
     abort;
end;
{ ****************************************************************************}
procedure Tdm.ibdsMembersBeforeCancel(DataSet: TDataSet);
begin
end;
{ ****************************************************************************}
procedure Tdm.tblMiscBeforePost(DataSet: TDataSet);
begin
     with tblMisc do begin
        if FieldByName('MONDAY_END_TIME').AsDateTime = StrToTime('00:00')
          then FieldByName('MONDAY_END_TIME').Clear;
        if FieldByName('TUESDAY_END_TIME').AsDateTime = StrToTime('00:00')
          then FieldByName('TUESDAY_END_TIME').Clear;
        if FieldByName('WEDNESDAY_END_TIME').AsDateTime = StrToTime('00:00')
          then FieldByName('WEDNESDAY_END_TIME').Clear;
        if FieldByName('THURSDAY_END_TIME').AsDateTime = StrToTime('00:00')
          then FieldByName('THURSDAY_END_TIME').Clear;
        if FieldByName('FRIDAY_END_TIME').AsDateTime = StrToTime('00:00')
          then FieldByName('FRIDAY_END_TIME').Clear;
        if FieldByName('SATURDAY_END_TIME').AsDateTime = StrToTime('00:00')
          then FieldByName('SATURDAY_END_TIME').Clear;
        if FieldByName('SUNDAY_END_TIME').AsDateTime = StrToTime('00:00')
          then FieldByName('SUNDAY_END_TIME').Clear;
     end;     
end;
{ ****************************************************************************}
procedure Tdm.tblKioskDrawsAfterInsert(DataSet: TDataSet);
begin
    tblKioskDraws.FieldByName('KIOSK_ID').AsInteger := LastNumberPlusOne('KIOSK_DRAWS','KIOSK_ID');
end;
{ ****************************************************************************}
{procedure Tdm.tmrPointsTimer(Sender: TObject);
const
    SQL1 = 'select MENUMATE_POINTS_DATE from MISC';
    SQL2 = 'update MISC set MENUMATE_POINTS_DATE = ''%s''';
begin
    // fires off every half hour
    //1800000
    if not IBRunOnce.InTransAction then
       IBRunOnce.StartTransaction;
    sqlExecquery(sqlRunOnce,SQL1);
    // is run at 5 am the next day , so put in todays date for MENUMATE_POINTS_DATE
    if (Date > sqlRunOnce.FieldByName('MENUMATE_POINTS_DATE').AsDateTime)
       and (Time >= StrToTime('05:00:00')) then begin
       UpdateMenuMatePoints;
       sqlExecquery(sqlRunOnce,format(SQL2,[GetShortDate(Date)]));
    end;   
    try
      if IBRunOnce.InTransAction then
         IBRunOnce.Commit;
    except
      IBRunOnce.Rollback;
      showmessage('proc tmrPointsTimer - MenuMate Points Update didnt work');
    end;     
end;}
{ ****************************************************************************}
{procedure Tdm.UpdateMenuMatePoints;
const
   SQL1 = 'UPDATE members set PREV_POINTS_AVAILABLE = POINTS_AVAILABLE';
   SQL2 = 'SELECT MEMBER,POINTS_AVAILABLE,PREV_POINTS_AVAILABLE,POINTS_AVAILABLE - PREV_POINTS_AVAILABLE as PA from MEMBERS where PREV_POINTS_AVAILABLE <> POINTS_AVAILABLE';
var
   theDate : String;   
begin
     try
        screen.cursor := crHourGlass;
        if not IBRunOnce.InTransAction then
           IBRunOnce.StartTransaction;
        sqlExecquery(sqlRunOnce,SQL2);
        ibdsMenuMatePoints.open;
        while not sqlRunOnce.EOF do begin
           with ibdsMenuMatePoints do begin
              append;
              FieldByName('MEMBER').AsInteger          := sqlRunOnce.FieldByName('MEMBER').AsInteger;
              FieldByName('POINTS_AVAILABLE').AsFloat  := sqlRunOnce.FieldByName('PREV_POINTS_AVAILABLE').AsFloat;
              FieldByName('POINTS_DIFFERENCE').AsFloat := sqlRunOnce.FieldByName('PA').AsFloat;
              FieldByName('THE_DATE').AsDateTime       := Date -1;
              post;
           end;
           sqlRunOnce.Next;
        end; 
        ibdsMenuMatePoints.Close;
        // update the PREV_POINTS_AVAILABLE field
        sqlExecquery(sqlRunOnce,SQL1);
         
     finally
        screen.cursor := crDefault;
     end;
end; }
{ ****************************************************************************}
procedure Tdm.ibdsPrefHeadingAfterInsert(DataSet: TDataSet);
begin
     DataSet.FieldByName('HEADING_ID').AsInteger := LastNumberPlusOne('PREF_HEADING','HEADING_ID');
end;
{ ****************************************************************************}
procedure Tdm.ibdsPrefSubHeadingAfterInsert(DataSet: TDataSet);
begin
     // 2 datasets look here
     DataSet.FieldByName('HEADING_ID').AsInteger := ibdsPrefHeading.FieldByName('HEADING_ID').AsInteger;
     if DataSet.Name = ibdsPrefSubHeading.Name then begin
        DataSet.FieldByName('DISPLAY_HEADING').AsString := varYes;
        DataSet.FieldByName('SUB_HEADING_ID').AsInteger := LastNumberPlusOne('PREF_SUB_HEADING','SUB_HEADING_ID');
     end else begin
        DataSet.FieldByName('DETAIL_TYPE').AsString := 'C'; // checkbox
        DataSet.FieldByName('DETAIL_ID').AsInteger  := LastNumberPlusOne('PREF_DETAIL','DETAIL_ID');
        DataSet.FieldByName('DISPLAY_DETAIL').AsString  := varYes;
     end;
end;
{ ****************************************************************************}
procedure Tdm.ibdsPrefSubHeadingAfterPost(DataSet: TDataSet);
const SQL1 = 'Select COUNT(LINE_NUMBER) as CntLineNumber from PREF_DETAIL where LINE_NUMBER = %d';
begin
     IBCommitRetain;
     // for Text,YesNo and Numerics controls only
     if ibdsPrefDetail.FieldByName('DETAIL_TYPE').AsString <> 'C' then begin
        sqlExecquery(qryGeneral,format(SQL1,[ibdsPrefDetail.FieldByName('LINE_NUMBER').AsInteger]));
        if qryGeneral.FieldByName('CntLineNumber').AsInteger > 1 then begin
           sm('The Line Number for Numeric,Text or Yes/No options must be unique'); 
        end;
     end;
end; 
{ ****************************************************************************}
procedure Tdm.ibdsPrefDetailBeforePost(DataSet: TDataSet);
var CheckBoxCtr,OtherCtr : smallint;
const SQL1 = 'Select DETAIL_TYPE,DETAIL_ID,LINE_NUMBER from PREF_DETAIL where HEADING_ID = %d and LINE_NUMBER = %d';
begin
     CheckBoxCtr := 0;
     OtherCtr := 0;
     // because the record is not in the DB yet 
     if ibdsPrefDetail.FieldByName('DETAIL_TYPE').AsString = 'C' then
       inc(CheckBoxCtr)
     else 
       inc(OtherCtr);
     with qryGeneral do begin
        sqlExecquery(qryGeneral,format(SQL1,[ibdsPrefDetail.FieldByName('HEADING_ID').AsInteger,ibdsPrefDetail.FieldByName('LINE_NUMBER').AsInteger]));
        while not EOF do begin
            // because the record is not in the DB yet 
            if FieldByName('DETAIL_ID').AsInteger = ibdsPrefDetail.FieldByName('DETAIL_ID').AsInteger then begin
               next;
               continue;
            end;   
            if FieldByName('DETAIL_TYPE').AsString = 'C' then
               inc(CheckBoxCtr)
            else
               inc(OtherCtr);
            next;
        end;
        if (CheckBoxCtr > 0) and (OtherCtr > 0) then begin
           sm('Cannot have a mix of Checkbox''s and Numeric,Text or Yes/No options'); 
        end;
     end;   
end;
{ ****************************************************************************}
procedure Tdm.ibdsPayMembersAfterOpen(DataSet: TDataSet);
var   MenuMatePointsUpdate : TMenuMatePointsUpdate;
begin
     // get the latest points for the paid by Member 
     if ibdsPayMembers.FieldByName('MEMBER').AsInteger <> 0 then begin
        MenuMatePointsUpdate.MembersPointsUpdate(ibdsPayMembers.FieldByName('MEMBER').AsString);
        ibdsPayMembers.Refresh;
     end;   
end;
{ ****************************************************************************}
procedure Tdm.ibdsM_DiscountsAfterInsert(DataSet: TDataSet);
const SQL1 = 'Select max(MEMBER_DISCOUNTS_KEY) as MaxKey from MEMBERS_DISCOUNTS where MEMBER = %d';
begin
     // MEMBER_DISCOUNTS_KEY is used as a sort key ( by each Member )
     sqlExecquery(qryGeneral,format(SQL1,[ibdsMembers.FieldByName('MEMBER').AsInteger]));
     DataSet.FieldByName('MEMBER_DISCOUNTS_KEY').AsInteger := qryGeneral.FieldByName('MaxKey').AsInteger + 1;
     DataSet.FieldByName('MEMBER').AsInteger               := ibdsMembers.FieldByName('MEMBER').AsInteger;
     DataSet.FieldByName('DISCOUNT_KEY').AsInteger         := dmMM.tblDiscounts.FieldByName('DISCOUNT_KEY').AsInteger;
     DataSet.Post;
end;
{ ****************************************************************************}
end.








