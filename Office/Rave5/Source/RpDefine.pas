{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RpDefine;

interface

{.$DEFINE DBG} { Used for data connection thread debugging }

{$I RpVer.inc}

uses
{$IFDEF DEMO}
  Dialogs,
{$ENDIF}
  Windows,
  Graphics,
  Math, DB, SysUtils, Classes;

{ Status format characters }
{ %% - % character }
{ %p - Current Page }
{ %f - First Page }
{ %l - Last Page }
{ %d - Printer Device Name }
{ %r - Printer Driver Name }
{ %t - Printer Port }
{ %c - Current Pass - used for printers that don't support multiple pages }
{ %s - Total Passes - used for printers that don't support multiple pages }
{ %0 through %9 - Status Text Line }

const
{$IFDEF DEMO}
  RunMode = '/Demo';
{$ELSE}
  RunMode = '';
{$ENDIF}

{$IFDEF VER120}
  CompVer = '4';
{$ENDIF}
{$IFDEF VER125}
  CompVer = '4';
{$ENDIF}
{$IFDEF VER130}
  CompVer = '5';
{$ENDIF}
{$IFDEF VER140}
  CompVer = '6';
{$ENDIF}
{$IFDEF VER150}
  CompVer = '7';
{$ENDIF}

  LibType = 'VCL';

  RvMajorVersion = '5';
  RvMinorVersion = '1';
  RvSubVersion = '2';

  RPVersion = RvMajorVersion + '.' + RvMinorVersion + '.' + RvSubVersion + ' (' + LibType + CompVer + RunMode + ')'; { Current revision number }

  NA = -32768; { n/a value }

  BOXLINENONE = 0;
  BOXLINELEFT = 1;
  BOXLINERIGHT = 2;
  BOXLINELEFTRIGHT = 3;
  BOXLINETOP = 4;
  BOXLINELEFTTOP = 5;
  BOXLINERIGHTTOP = 6;
  BOXLINENOBOTTOM = 7;
  BOXLINEBOTTOM = 8;
  BOXLINELEFTBOTTOM = 9;
  BOXLINERIGHTBOTTOM = 10;
  BOXLINENOTOP = 11;
  BOXLINETOPBOTTOM = 12;
  BOXLINENORIGHT = 13;
  BOXLINENOLEFT = 14;
  BOXLINEALL = 15;

  BOXLINEHORIZ = BOXLINETOPBOTTOM; { Don't use, replaced by BOXLINETOPBOTTOM }
  BOXLINEVERT = BOXLINELEFTRIGHT; { Don't use, replaced by BOXLINELEFTRIGHT }

  MAXSAVEFONT = 10;
  MAXSAVETABS = 10;
  MAXSAVEPOS = 10;

  MAXWORDLEN = 65520;
  MAXBUFSIZE = 2000000000;

  MINPREVIEWZOOM = 10.0;
  MAXPREVIEWZOOM = 200.0;

{ ReportPrinter Commands }
{ GDI commands }
  rpcArc = 1;
  rpcBrushCopy = 2;
  rpcChord = 3;
  rpcCopyRect = 4;
  rpcDraw = 5;
  rpcDrawFocusRect = 6;
  rpcEllipse = 7;
  rpcFillRect = 8;
  rpcFloodFill = 9;
  rpcFrameRect = 10;
  rpcLineTo = 11;
  rpcMoveTo = 12;
  rpcPie = 13;
  rpcPolygon = 14;
  rpcPolyline = 15;
  rpcPrintBitmap = 16;
  rpcPrintBitmapRect = 17;
  rpcRectangle = 18;
  rpcTabRectangle = 19;
  rpcRoundRect = 20;
  rpcStretchDraw = 21;
  rpcTextRect = 22;
  rpcCenterText = 23;
  rpcLeftText = 24;
  rpcRightText = 25;
  rpcSetTextBKMode = 26;
  rpcSetBKColor = 27;
  rpcSetScaleX = 28;
  rpcSetScaleY = 29;
  rpcPrintData = 30;
  rpcPrintDataStream = 31;
  rpcPrintSpaces = 32;
  rpcBlockText = 33;
  rpcTransBitmap = 34;
  rpcSetFrameMode = 35;
  rpcPrintImageRect = 36;
  rpcVariableData = 37;

{ Brush commands }
  rpcSetBrush = 40;
  rpcSetBrushColor = 41;
  rpcSetBrushStyle = 42;
  rpcSetBrushBitmap = 43;

{ Font commands }
  rpcSetFont = 50;
  rpcSetFontName = 51;
  rpcSetFontColor = 52;
  rpcSetFontPitch = 53;
  rpcSetFontHeight = 54;
  rpcSetFontBold = 55;
  rpcSetFontUnderline = 56;
  rpcSetFontItalic = 57;
  rpcSetFontStrikeout = 58;
  rpcSetFontSuperscript = 59;
  rpcSetFontSubscript = 60;
  rpcSetFontRotation = 61;
  rpcSetFontAlign = 62;
  rpcSetFontCharset = 63;

{ Pen commands }
  rpcSetPen = 70;
  rpcSetPenColor = 71;
  rpcSetPenMode = 72;
  rpcSetPenStyle = 73;
  rpcSetPenWidth = 74;

{ Misc commands }
  rpcNewPage = 80;
  rpcJobHeader11 = 81; { Old ReportPrinter 1.1 Header }
  rpcEndPage = 82;
  rpcResetLineHeight = 83;
  rpcAllowPreviewOnly = 84;
  rpcAllowPrinterOnly = 85;
  rpcAllowAll = 86;
  rpcSetOrientation = 87;
  rpcSetBin = 88;
  rpcJobHeader20i = 89; { Old ReportPriner Pro 2.0a-i Header }
  rpcJobHeader = 90; { ReportPriner Pro 2.0j- Header }

{ All user created commands should start here }
  rpcUser = 200;

  FontString =
   'abcdefg hijklmnop qrstuv wxyz ABCDEFG HIJKLMNOP QRSTUV WXYZ 0123456789';
 {$EXTERNALSYM FontString}

  RTFIgnoreDest: array[1..32] of string[10] = (
   'author','buptim','comment','creatim','doccomm',
   'footer','footerf','footerl','footerr','footnote',
   'ftncn','ftnsep','ftnsepc','header','headerf',
   'headerl','headerr','info','keywords','operator',
   'pict','printim','private1','revtim','rxe',
   'stylesheet','subject','tc','title','txe','xe','*');

{ *** RPTF Constants *** }
  RPTFPrefix = #1; { Begins an RPTF formatting block }
  RPTFInfix = #2; { Separates commands within an RPTF formatting blocks }
  RPTFSeparator = #3; { Used to separate parameters }
  RPTFSuffix = #4; { Ends an RPTF formatting block }

  RPTFOff = '0';
  RPTFOn = '1';
  RPTFLeft = 'L';
  RPTFCenter = 'C';
  RPTFRight = 'R';
  RPTFBlock = 'B';

{ RPTF Commands }
  RPTFIgnore = ' ';
  RPTFBold = 'B';
  RPTFItalic = 'I';
  RPTFUnderline = 'U';
  RPTFStrikeout = 'K';
  RPTFSuperscript = 'P';
  RPTFSubscript = 'S';
  RPTFName = 'N';
  RPTFSize = 'Z';
  RPTFColor = 'C';
  RPTFPitch = 'H';
  RPTFResetParagraph = 'R';
  RPTFResetStyle = 'r';
  RPTFJustify = 'J';
  RPTFFirstIndent = 'F';
  RPTFLeftIndent = 'L';
  RPTFRightIndent = 'G';

{ RPTF Macros }
{$IFDEF BCB}
  (*$HPPEMIT '#define BoldOn "\x01\x42\x31\x04"' *)
  (*$HPPEMIT '#define BoldOff "\x01\x42\x30\x04"' *)
  (*$HPPEMIT '#define ItalicOn "\x01\x49\x31\x04"' *)
  (*$HPPEMIT '#define ItalicOff "\x01\x49\x30\x04"' *)
  (*$HPPEMIT '#define UnderlineOn "\x01\x55\x31\x04"' *)
  (*$HPPEMIT '#define UnderlineOff "\x01\x55\x30\x04"' *)
  (*$HPPEMIT '#define StrikeoutOn "\x01\x4B\x31\x04"' *)
  (*$HPPEMIT '#define StrikeoutOff "\x01\x4B\x30\x04"' *)
  (*$HPPEMIT '#define SuperscriptOn "\x01\x50\x31\x04"' *)
  (*$HPPEMIT '#define SuperscriptOff "\x01\x50\x30\x04"' *)
  (*$HPPEMIT '#define SubscriptOn "\x01\x53\x31\x04"' *)
  (*$HPPEMIT '#define SubscriptOff "\x01\x53\x30\x04"' *)
  (*$HPPEMIT '#define JustifyLeft "\x01\x4A\x4C\x04"' *)
  (*$HPPEMIT '#define JustifyCenter "\x01\x4A\x43\x04"' *)
  (*$HPPEMIT '#define JustifyRight "\x01\x4A\x52\x04"' *)
  (*$HPPEMIT '#define JustifyBlock "\x01\x4A\x42\x04"' *)
  (*$HPPEMIT '#undef ResetPrinter' *)
{$ELSE}
  BoldOn = RPTFPrefix + RPTFBold + RPTFOn + RPTFSuffix;
  BoldOff = RPTFPrefix + RPTFBold + RPTFOff + RPTFSuffix;
  ItalicOn = RPTFPrefix + RPTFItalic + RPTFOn + RPTFSuffix;
  ItalicOff = RPTFPrefix + RPTFItalic + RPTFOff + RPTFSuffix;
  UnderlineOn = RPTFPrefix + RPTFUnderline + RPTFOn + RPTFSuffix;
  UnderlineOff = RPTFPrefix + RPTFUnderline + RPTFOff + RPTFSuffix;
  StrikeoutOn = RPTFPrefix + RPTFStrikeout + RPTFOn + RPTFSuffix;
  StrikeoutOff = RPTFPrefix + RPTFStrikeout + RPTFOff + RPTFSuffix;
  SuperscriptOn = RPTFPrefix + RPTFSuperscript + RPTFOn + RPTFSuffix;
  SuperscriptOff = RPTFPrefix + RPTFSuperscript + RPTFOff + RPTFSuffix;
  SubscriptOn = RPTFPrefix + RPTFSubscript + RPTFOn + RPTFSuffix;
  SubscriptOff = RPTFPrefix + RPTFSubscript + RPTFOff + RPTFSuffix;
  JustifyLeft = RPTFPrefix + RPTFJustify + RPTFLeft + RPTFSuffix;
  JustifyCenter = RPTFPrefix + RPTFJustify + RPTFCenter + RPTFSuffix;
  JustifyRight = RPTFPrefix + RPTFJustify + RPTFRight + RPTFSuffix;
  JustifyBlock = RPTFPrefix + RPTFJustify + RPTFBlock + RPTFSuffix;
{$ENDIF}

type
  TRavePaperSize = type integer;
  TPaperSizeName = record
    Name: string;
    PaperSize: TRavePaperSize;
    PaperWidth: double;
    PaperHeight: double;
  end; { TPaperSizeName }


const
  PaperSizeNameCnt = 43;
  PaperSizeName: array[1..PaperSizeNameCnt] of TPaperSizeName = (
   (Name:'Default'; PaperSize:-1;PaperWidth:0;PaperHeight:0),
   (Name:'Custom'; PaperSize:DMPAPER_USER;PaperWidth:0;PaperHeight:0),
   (Name:'Letter, 8 1/2- by 11-inches'; PaperSize:DMPAPER_LETTER;PaperWidth:8.5;PaperHeight:11.0),
   (Name:'Legal, 8 1/2- by 14-inches'; PaperSize:DMPAPER_LEGAL;PaperWidth:8.5;PaperHeight:14.0),
   (Name:'A4 Sheet, 210- by 297-millimeters'; PaperSize:DMPAPER_A4;PaperWidth:210/25.4;PaperHeight:297/25.4),
   (Name:'C Sheet, 17- by 22-inches'; PaperSize:DMPAPER_CSHEET;PaperWidth:17.0;PaperHeight:22.0),
   (Name:'D Sheet, 22- by 34-inches'; PaperSize:DMPAPER_DSHEET;PaperWidth:22.0;PaperHeight:34.0),
   (Name:'E Sheet, 34- by 44-inches'; PaperSize:DMPAPER_ESHEET;PaperWidth:34.0;PaperHeight:44.0),
   (Name:'Letter Small, 8 1/2- by 11-inches'; PaperSize:DMPAPER_LETTERSMALL;PaperWidth:8.5;PaperHeight:11.0),
   (Name:'Tabloid, 11- by 17-inches'; PaperSize:DMPAPER_TABLOID;PaperWidth:11.0;PaperHeight:17.0),
   (Name:'Ledger, 17- by 11-inches'; PaperSize:DMPAPER_LEDGER;PaperWidth:17.0;PaperHeight:11.0),
   (Name:'Statement, 5 1/2- by 8 1/2-inches'; PaperSize:DMPAPER_STATEMENT;PaperWidth:5.5;PaperHeight:8.5),
   (Name:'Executive, 7 1/4- by 10 1/2-inches'; PaperSize:DMPAPER_EXECUTIVE;PaperWidth:7.25;PaperHeight:10.5),
   (Name:'A3 sheet, 297- by 420-millimeters'; PaperSize:DMPAPER_A3;PaperWidth:297/25.4;PaperHeight:420/25.4),
   (Name:'A4 small sheet, 210- by 297-millimeters'; PaperSize:DMPAPER_A4SMALL;PaperWidth:210/25.4;PaperHeight:297/25.4),
   (Name:'A5 sheet, 148- by 210-millimeters'; PaperSize:DMPAPER_A5;PaperWidth:148/25.4;PaperHeight:210/25.4),
   (Name:'B4 sheet, 250- by 354-millimeters'; PaperSize:DMPAPER_B4;PaperWidth:250/25.4;PaperHeight:354/25.4),
   (Name:'B5 sheet, 182- by 257-millimeter paper'; PaperSize:DMPAPER_B5;PaperWidth:182/25.4;PaperHeight:257/25.4),
   (Name:'Folio, 8 1/2- by 13-inch paper'; PaperSize:DMPAPER_FOLIO;PaperWidth:8.5;PaperHeight:13.0),
   (Name:'Quarto, 215- by 275-millimeter paper'; PaperSize:DMPAPER_QUARTO;PaperWidth:215/25.4;PaperHeight:275/25.4),
   (Name:'10- by 14-inch sheet'; PaperSize:DMPAPER_10X14;PaperWidth:10.0;PaperHeight:14.0),
   (Name:'11- by 17-inch sheet'; PaperSize:DMPAPER_11X17;PaperWidth:11.0;PaperHeight:17.0),
   (Name:'Note, 8 1/2- by 11-inches'; PaperSize:DMPAPER_NOTE;PaperWidth:8.5;PaperHeight:11.0),
   (Name:'#9 Envelope, 3 7/8- by 8 7/8-inches'; PaperSize:DMPAPER_ENV_9;PaperWidth:3.875;PaperHeight:8.875),
   (Name:'#10 Envelope, 4 1/8- by 9 1/2-inches'; PaperSize:DMPAPER_ENV_10;PaperWidth:4.125;PaperHeight:9.5),
   (Name:'#11 Envelope, 4 1/2- by 10 3/8-inches'; PaperSize:DMPAPER_ENV_11;PaperWidth:4.5;PaperHeight:10.375),
   (Name:'#12 Envelope, 4 3/4- by 11-inches'; PaperSize:DMPAPER_ENV_12;PaperWidth:4.75;PaperHeight:11.0),
   (Name:'#14 Envelope, 5- by 11 1/2-inches'; PaperSize:DMPAPER_ENV_14;PaperWidth:5.0;PaperHeight:11.5),
   (Name:'DL Envelope, 110- by 220-millimeters'; PaperSize:DMPAPER_ENV_DL;PaperWidth:110/25.4;PaperHeight:220/25.4),
   (Name:'C5 Envelope, 162- by 229-millimeters'; PaperSize:DMPAPER_ENV_C5;PaperWidth:162/25.4;PaperHeight:229/25.4),
   (Name:'C3 Envelope,  324- by 458-millimeters'; PaperSize:DMPAPER_ENV_C3;PaperWidth:324/25.4;PaperHeight:458/25.4),
   (Name:'C4 Envelope,  229- by 324-millimeters'; PaperSize:DMPAPER_ENV_C4;PaperWidth:229/25.4;PaperHeight:324/25.4),
   (Name:'C6 Envelope,  114- by 162-millimeters'; PaperSize:DMPAPER_ENV_C6;PaperWidth:114/25.4;PaperHeight:162/25.4),
   (Name:'C65 Envelope, 114- by 229-millimeters'; PaperSize:DMPAPER_ENV_C65;PaperWidth:114/25.4;PaperHeight:229/25.4),
   (Name:'B4 Envelope,  250- by 353-millimeters'; PaperSize:DMPAPER_ENV_B4;PaperWidth:250/25.4;PaperHeight:353/25.4),
   (Name:'B5 Envelope,  176- by 250-millimeters'; PaperSize:DMPAPER_ENV_B5;PaperWidth:176/25.4;PaperHeight:250/25.4),
   (Name:'B6 Envelope,  176- by 125-millimeters'; PaperSize:DMPAPER_ENV_B6;PaperWidth:176/25.4;PaperHeight:125/25.4),
   (Name:'Italy Envelope, 110- by 230-millimeters'; PaperSize:DMPAPER_ENV_ITALY;PaperWidth:110/25.4;PaperHeight:230/25.4),
   (Name:'Monarch Envelope, 3 7/8- by 7 1/2-inches'; PaperSize:DMPAPER_ENV_MONARCH;PaperWidth:3.875;PaperHeight:7.5),
   (Name:'6 3/4 Envelope, 3 5/8- by 6 1/2-inches'; PaperSize:DMPAPER_ENV_PERSONAL;PaperWidth:3.626;PaperHeight:6.5),
   (Name:'US Std Fanfold, 14 7/8- by 11-inches'; PaperSize:DMPAPER_FANFOLD_US;PaperWidth:14.875;PaperHeight:12.0),
   (Name:'German Std Fanfold, 8 1/2- by 12-inches'; PaperSize:DMPAPER_FANFOLD_STD_GERMAN;PaperWidth:8.5;PaperHeight:12.0),
   (Name:'German Legal Fanfold, 8 1/2- by 13-inches'; PaperSize:DMPAPER_FANFOLD_LGL_GERMAN;PaperWidth:8.5;PaperHeight:13.0)
  );


type
  EReportPrinter = class(Exception);

  TPrintPageEvent = function(    Sender: TObject;
                             var PageNum: integer): boolean of object;
  TDecodeImageEvent = procedure(Sender: TObject;
                                ImageStream: TStream;
                                ImageType: string;
                                Bitmap: TBitmap) of object;

  TVersion = string;
  TTitleString = string;
  TFormatString = string;
  TDeviceState = (dsNone,dsIC,dsDC);
  TAccuracyMethod = (amPositioning,amAppearance);
  TMarginMethod = (mmScaled,mmFixed);
  TRulerType = (rtNone,rtHorizCm,rtVertCm,rtBothCm,rtHorizIn,rtVertIn,rtBothIn);
  TPrintJustify = (pjCenter,pjLeft,pjRight,pjBlock);
  TTabJustify = (tjCenter,tjLeft,tjRight,tjBlock,tjNone);
  TPrintJustifyVert = (pjTop,pjMiddle,pjBottom);
  TPrintUnits = (unInch,unMM,unCM,unPoint,unUser);
  TLineHeightMethod = (lhmLinesPerInch,lhmFont,lhmUser);
  TBKMode = (bkTransparent,bkOpaque);
  TDestination = (destPrinter,destPreview,destFile,destNone,destCustomFile);
  TStreamMode = (smMemory,smFile,smUser,smTempFile);
  TOrientation = (poPortrait,poLandScape,poDefault);
  TDuplex = (dupSimplex,dupVertical,dupHorizontal);
  TReportEventType = (reBeginPage,reEndPage);
  TFontAlign = (faBaseline,faTop,faBottom);
  TReportDest = (rdPreview,rdPrinter,rdFile);
  TSystemSetup = (ssAllowSetup,ssAllowCopies,ssAllowCollate,ssAllowDuplex,
   ssAllowDestPreview,ssAllowDestPrinter,ssAllowDestFile,ssAllowPrinterSetup,
   ssAllowPreviewSetup);
  TSystemOption = (soUseFiler,soWaitForOK,soShowStatus,soAllowPrintFromPreview,
   soPreviewModal,soNoGenerate);
  TBoxLines = (blNone,blLeft,blRight,blLeftRight,blTop,blLeftTop,blRightTop,
   blNoBottom,blBottom,blLeftBottom,blRightBottom,blNoTop,blTopBottom,blNoRight,
   blNoLeft,blAll);
  TMacroID = (midCurrDateShort,midCurrDateLong,midCurrDateUS,midCurrDateInter,
   midCurrTimeShort,midCurrTimeLong,midCurrTimeAMPM,midCurrTime24,midFirstPage,
   midLastPage,midTotalPages,midCurrentPage,midRelativePage,
   midPrinterName,midDriverName,midPortName,
   midUser01,midUser02,midUser03,midUser04,midUser05,midUser06,midUser07,
   midUser08,midUser09,midUser10,midUser11,midUser12,midUser13,midUser14,
   midUser15,midUser16,midUser17,midUser18,midUser19,midUser20);
  TRTFTokenType = (ttControlWord,ttControlSymbol,ttGroupBegin,ttGroupEnd,ttText);
  TRTFTokenTypeSet = set of TRTFTokenType;
  PCharArray = ^TCharArray;
  TCharArray = array[0..MAXBUFSIZE] of char;
  TFrameMode = (fmInside,fmSplit,fmOutside);

  TSystemSetups = set of TSystemSetup;
  TSystemOptions = set of TSystemOption;

  TPosition = record
    X: double;
    Y: double;
  end; { TPosition }

  PPositionNode = ^TPositionNode;
  TPositionNode = record
    X: double;
    Y: double;
    Next: PPositionNode;
  end; { TPositionNode }

  PTab = ^TTab;
  TTab = record
    Pos: double; { X Position of tab in inches }
    Justify: TPrintJustify; { Tab justification }
    Width: double; { Width of Tab Box in inches }
    Margin: double; { Left or right margin for Tab Box }
    Left: boolean; { Draw line on left? }
    Right: boolean; { Draw line on right? }
    Top: boolean; { Draw line on top? }
    Bottom: boolean; { Draw line on bottom? }
    Shade: byte; { Percent shading for background }
    RefCount: word; { How many times is this tab list referenced? }
    Next: PTab; { Next tab in list }
    Prev: PTab; { Previous tab in list }
  end; { TTab }

  TSaveTab = record
    Head: PTab;
    Tail: PTab;
  end; { TSaveTab }

  PTabNode = ^TTabNode;
  TTabNode = record
    Head: PTab;
    Tail: PTab;
    Next: PTabNode;
  end; { TTabNode }

  TSaveBrush = record
    Color: TColor;
    Style: TBrushStyle;
    Bitmap: boolean;
  end; { TSaveBrush }

  TSaveFont = record
    Name: string[60];
    Color: TColor;
    Pitch: TFontPitch;
    Size: double; { Point size of normal font }
    Height: double; { Height of normal font in inches }
    Width: double; { Average width of normal font character in inches }
    Ascent: double; { Ascent height of true font in inches }
    Descent: double; { Descent height of true font in inches }
    InternalLeading: double; { Internal Leading height of true font in inches }
    Bold: boolean;
    Underline: boolean;
    Italic: boolean;
    Strikeout: boolean;
    Subscript: boolean; { Is the true font subscript? }
    Superscript: boolean; { Is the true font superscript? }
    Rotation: smallint; { Rotation in degrees }
    Align: TFontAlign; { Alignment for this font }
    Index: word; { Index for this font }
    Handle: Windows.HFont;
    CharSet: integer; //!!PORT!! RPDEFINE01
  end; { TSaveFont }

  PFontNode = ^TFontNode;
  TFontNode = record
    Font: TSaveFont;
    Next: PFontNode;
  end; { TFontNode }

  TSavePen = record
    Color: TColor;
    Mode: TPenMode;
    Style: TPenStyle;
    Width: double;
  end; { TSavePen }

  TFormatPara = record
    Justify: TPrintJustify;
    FirstIndent: double;
    LeftIndent: double;
    RightIndent: double;
  end; { TFormatPara }

  TFormatStyle = record
    Bold: boolean;
    Italic: boolean;
    Underline: boolean;
    Strikeout: boolean;
    Subscript: boolean;
    Superscript: boolean;
    Pitch: TFontPitch;
    Color: TColor;
  end; { TFormatStyle }

  PFormatState = ^TFormatState;
  TFormatState = record
    Name: string[40];
    Size: double;
    Style: TFormatStyle;
    Para: TFormatPara;
    Prev: PFormatState;
  end; { TFormatState }

  PLineList = ^TLineList;
  TLineList = record
    P1,P2: double; { Start and End points, P1 must be <= P2 }
    C1: double; { Common value }
    Color: TColor;
    Mode: TPenMode;
    Style: TPenStyle;
    Width: integer;
    Next: PLineList;
  end; { TLineList }

  TRpComponent = class(TComponent)
  protected
    FVersion: TVersion; { Internal revision number }
    procedure SetVersion(Value: TVersion);
  public
    constructor Create(AOwner: TComponent); override;
  published
    property Version: TVersion read FVersion write SetVersion stored false;
  end; { TRpComponent }

  TRPStringObject = class
    FValue: string;
  public
    constructor Create(InitValue: string);
    property Value: string read FValue write FValue;
  end; { TRPStringObject }

  PRPDevMode = PDeviceMode;

  procedure RaiseError(ErrorMessage: string);
  function ShadeToColor(ShadeColor: TColor;
                        ShadePercent: byte): TColor;
  function MoneyToLongName(Value: extended): string;

{$IFDEF DEMO}
  procedure DisplayWarning;
  function Validate: boolean;
{$ENDIF}

type
  TRPDataType = (dtString,dtInteger,dtBoolean,dtFloat,dtCurrency,dtBCD,dtDate,
   dtTime,dtDateTime,dtBlob,dtMemo,dtGraphic,dtInt64);
  TRPDataTypeSet = set of TRPDataType;

const
  TRPDataSetType: array[low(TFieldType)..high(TFieldType)] of TRPDataType =
   (
    {ftUnknown=}dtString,
    {ftString=}dtString,
    {ftSmallint=}dtInteger,
    {ftInteger=}dtInteger,
    {ftWord=}dtInteger,
    {ftBoolean=}dtBoolean,
    {ftFloat=}dtFloat,
    {ftCurrency=}dtCurrency,
    {ftBCD=}dtBCD,
    {ftDate=}dtDate,
    {ftTime=}dtTime,
    {ftDateTime=}dtDateTime,
    {ftBytes=}dtBlob,
    {ftVarBytes=}dtBlob,
    {ftAutoInc=}dtInteger,
    {ftBlob=}dtBlob,
    {ftMemo=}dtMemo,
    {ftGraphic=}dtGraphic,
    {ftFmtMemo=}dtBlob,
    {ftParadoxOle=}dtBlob,
    {ftDBaseOle=}dtBlob,
    {ftTypedBinary=}dtBlob,
    {ftCursor=}dtBlob,
    {ftFixedChar=}dtString,
    {ftWideString=}dtString,
    {ftLargeint=}dtInt64,
    {ftADT=}dtBlob,
    {ftArray=}dtBlob,
    {ftReference=}dtBlob,
    {ftDataSet=}dtBlob{$IFDEF LEVEL5},
    {ftOraBlob=}dtBlob,
    {ftOraClob=}dtBlob,
    {ftVariant=}dtBlob,
    {ftInterface=}dtBlob,
    {ftIDispatch=}dtBlob,
    {ftGuid=}dtString{$IFDEF LEVEL6},
    {ftTimeStamp}dtDateTime,
    {ftFMTBcd}dtBCD
    {$ENDIF}{$ENDIF}
  );

const
  FileMapSize = 65536;
  FileMapName = 'RAVEBUFFER';
  AltFileMapName = 'RAVEBUFFERALT';
  EVENTPREPARED = 0;

  DATAFIRST = 1;
  DATANEXT = 2;
  DATAEOF = 3;
  DATAGETCOLS = 4;
  DATAGETROW = 5;
  DATASETFILTER = 6;
  DATAGETSORTS = 7;
  DATASETSORT = 8;
  DATAOPEN = 9;
  DATARESTORE = 10;
  DATAACKNOWLEDGE = 11;
  DATAFREEALTBUF = 12;

var
  DataID: string = '';

const
  ControllerMutexName = 'RAVECONTROLER';
  CompletedEventName = 'RAVECOMPLETED';
  AcknowledgeRTEventName = 'RAVERTACK_';
  AcknowledgeDTEventName = 'RAVEDTACK_';
  ConnectEventName = 'RAVECONNECT';
  ErrorEventName = 'RAVEERROR';
  DisconnectEventName = 'RAVEDISCONNECT';
  DataRTEventName = 'RAVERTDATA_';
  DataDTEventName = 'RAVEDTDATA_';

type
  TRaveConnectionError = (ceAbort,ceInform,cePrompt);

  function InitEvent(EventName: string): THandle;
  function InitManualEvent(EventName: string): THandle;
  function InitMutex(MutexName: string): THandle;
  function InitFileMap(MapName: string;
                       MapSize: integer): THandle;
  function InitFileBuf(FileMap: THandle): PChar;
{$IFDEF DBG}
const
  DebugMapSize = 500000;
  DebugMapName = 'RAVEDEBUGBUF';

  procedure WriteDebug(Value: string;
                       DebugBuf: PChar);
  function InitDebugMap: THandle;
{$ENDIF}

  function DefaultTrans(const Value: string): string;
  function ConvertCharset(Charset: byte): byte;

var
  Trans: function(const Value: string): string;

{$IFNDEF LEVEL5}
  procedure FreeAndNil(var Item);
  function IncludeTrailingBackslash(const S: string): string;
  function SameText(const S1, S2: string): Boolean;
  function AnsiSameText(const S1, S2: string): Boolean;
{$ENDIF}

procedure CloseEvent(AEvent: THandle;
                     Force: boolean = false);
procedure CloseMutex(AMutex: THandle;
                     Force: boolean = false);
procedure CloseFileMap(AFileMap: THandle;
                       AFileBuf: PChar);

implementation

{$IFNDEF LEVEL5}
procedure FreeAndNil(var Item);
var
  SaveItem: TObject;
begin { FreeAndNil }
  SaveItem := TObject(Item);
  TObject(Item) := nil;
  SaveItem.Free;
end;  { FreeAndNil }

function IncludeTrailingBackslash(const S: string): string;
begin
  if S[Length(S)] = '\' then begin
    Result := S;
  end else begin
  Result := S + '\';
  end; { else }
end;

function SameText(const S1, S2: string): Boolean;
begin
  Result := CompareText(S1,S1) = 0;
end;

function AnsiSameText(const S1, S2: string): Boolean;
begin
  Result := AnsiCompareText(S1,S2) = 0;
end;
{$ENDIF}

function DefaultTrans(const Value: string): string;
begin { DefaultTrans }
  Result := Value;
end;  { DefaultTrans }


procedure CloseEvent(AEvent: THandle;
                     Force: boolean = false);
begin { CloseEvent }
  CloseHandle(AEvent);
end;  { CloseEvent }

procedure CloseMutex(AMutex: THandle;
                     Force: boolean = false);
begin { CloseMutex }
  CloseHandle(AMutex);
end;  { CloseMutex }

procedure CloseFileMap(AFileMap: THandle;
                       AFileBuf: PChar);
begin { CloseFileMap }
  UnmapViewOfFile(AFileBuf);
  CloseHandle(AFileMap);
end;  { CloseFileMap }

function InitEvent(EventName: string): THandle;
begin { InitEvent }
  EventName := DataID + EventName;
  if EventName = '' then begin
    Result := 0;
  end else begin
  // Open event if it exists, otherwise return 0
    Result := OpenEvent(EVENT_ALL_ACCESS,false,@EventName[1]);
    if Result = 0 then begin
    // Create event
      Result := CreateEvent(nil,false,false,@EventName[1]);
    end; { if }
  end; { else }
end;  { InitEvent }

function InitManualEvent(EventName: string): THandle;
begin { InitManualEvent }
  EventName := DataID + EventName;
  if EventName = '' then begin
    Result := 0;
  end else begin
    Result := OpenEvent(EVENT_ALL_ACCESS,false,@EventName[1]);
    if Result = 0 then begin
      Result := CreateEvent(nil,true,false,@EventName[1]);
    end; { if }
  end; { else }
end;  { InitManualEvent }

function InitMutex(MutexName: string): THandle;
begin { InitMutex }
  MutexName := DataID + MutexName;
  if MutexName = '' then begin
    Result := 0;
  end else begin
    Result := OpenMutex(MUTEX_ALL_ACCESS,false,@MutexName[1]);
    if Result = 0 then begin
      Result := CreateMutex(nil,false,@MutexName[1]);
    end; { if }
  end; { else }
end;  { InitMutex }

function InitFileMap(MapName: string;
                     MapSize: integer): THandle;
begin { InitFileMap }
  MapName := DataID + MapName;
  Result := OpenFileMapping(FILE_MAP_WRITE,false,@MapName[1]);
  if Result = 0 then begin
    Result := CreateFileMapping($FFFFFFFF,nil,PAGE_READWRITE,0,MapSize,@MapName[1]);
  end; { if }
end;  { InitFileMap }

function InitFileBuf(FileMap: THandle): PChar;

begin { InitFileBuf }
  Result := PChar(MapViewOfFile(FileMap,FILE_MAP_READ OR FILE_MAP_WRITE,0,0,0));
end;  { InitFileBuf }

{$IFDEF DBG}
procedure WriteDebug(Value: string;
                     DebugBuf: PChar);

var
 DebugPtr: PChar;
 Offset: integer;
 CS: TRTLCriticalSection;

begin { WriteDebug }
  InitializeCriticalSection(CS);
  try
    EnterCriticalSection(CS);
    Offset := integer(pointer(DebugBuf)^); { Get old offset }
    if (Offset + 128) < DebugMapSize then begin
      DebugPtr := DebugBuf + Offset + 4; { Move DebugPtr to write location }
      StrPCopy(DebugPtr,Value); { Write Value out }
      integer(pointer(DebugBuf)^) := Offset + Length(Value) + 1; { Update offset }
    end; { if }
  finally
    LeaveCriticalSection(CS);
    DeleteCriticalSection(CS);
  end; { with }
end;  { WriteDebug }

function InitDebugMap: THandle;

begin { InitDebugMap }
  Result := OpenFileMapping(FILE_MAP_WRITE,false,DebugMapName);
  if Result = 0 then begin
    Result := CreateFileMapping($FFFFFFFF,nil,PAGE_READWRITE,0,DebugMapSize,
     DebugMapName);
  end; { if }
end;  { InitDebugMap }
{$ENDIF}

procedure RaiseError(ErrorMessage: string);

begin { RaiseError }
  Raise EReportPrinter.Create(ErrorMessage);
end;  { RaiseError }


function ShadeToColor(ShadeColor: TColor;
                      ShadePercent: byte): TColor;

var
  Red: byte;
  Blue: byte;
  Green: byte;
  RGBVal: longint;

begin { ShadeToColor }
  if ShadePercent > 100 then begin
    ShadePercent := 100;
  end; { if }
  RGBVal := ColorToRGB(ShadeColor);
  Red := ((255 - GetRValue(RGBVal)) * (100 - ShadePercent)) div 100 +
   GetRValue(RGBVal);
  Green := ((255 - GetGValue(RGBVal)) * (100 - ShadePercent)) div 100 +
   GetGValue(RGBVal);
  Blue := GetBValue(RGBVal);
  Blue := ((255 - Blue) * (100 - ShadePercent)) div 100 + Blue;
  Result := RGB(Red,Green,Blue);
end;  { ShadeToColor }

function MoneyToLongName(Value: extended): string;

const
  Name1: array[0..9] of string = ('','One ','Two ','Three ','Four ','Five ',
  'Six ','Seven ','Eight ','Nine ');
  Name2: array[0..9] of string = ('Ten ','Eleven ','Twelve ','Thirteen ',
  'Fourteen ','Fifteen ','Sixteen ','Seventeen ','Eighteen ','Nineteen ');
  Name3: array[2..9] of string = ('Twenty ','Thirty ','Forty ','Fifty ',
  'Sixty ','Seventy ','Eighty ','Ninety ');
  Name4: array[0..4] of string = ('Trillion ','Billion ','Million ','Thousand ','');

  function MoneyToShortName(Value: longint;
                            AddName: string): string;
  { TODO : This is more problematic than just MBCS }
  var
    I1: longint;

  begin { MoneyToShortName }
    Result := '';
    if Value = 0 then Exit;
    I1 := Value div 100;
    if I1 > 0 then begin
      Result := Name1[I1] + 'Hundred ';
    end; { if }
    I1 := (Value div 10) mod 10;
    if I1 = 1 then begin
      Result := Result + Name2[Value mod 10];
    end else begin
      if I1 >= 2 then begin
        Result := Result + Name3[I1];
      end; { if }
      Result := Result + Name1[Value mod 10];
    end; { else }
    Result := Result + AddName;
  end;  { MoneyToShortName }

var
  I1: integer;
  S1: string;

begin { MoneyToLongName }
  S1 := FloatToStrF(Abs(Value),ffFixed,18,2);
  if Length(S1) > 18 then begin
    Result := '(overflow)';
    Exit;
  end; { if }
  while Length(S1) < 18 do begin
    S1 := '0' + S1;
  end; { while }
  if Value < 1.0 then begin
    Result := 'Zero ';
  end else begin
    Result := '';
    for I1 := 0 to 4 do begin
      Result := Result + MoneyToShortName(StrToInt(Copy(S1,I1 * 3 + 1,3)),Name4[I1]);
    end; { for }
  end; { else }
  Result := Result + 'and ' + Copy(S1,17,2) + '/100';
end;  { MoneyToLongName }

constructor TRpComponent.Create(AOwner: TComponent);

begin { Create }
  inherited Create(AOwner);
  FVersion := RPVersion;
end;  { Create }

procedure TRpComponent.SetVersion(Value: TVersion);

begin { SetVersion }
{ Don't do anything since version was set in Create }
end;  { SetVersion }

constructor TRpStringObject.Create(InitValue: string);

begin { Create }
  inherited Create;
  FValue := InitValue;
end;  { Create }

{ The file DEMOUTIL.PAS is not included nor is it required
  for the full version of Rave Reports }

{$IFDEF DEMO}
{$I ..\NONDIST\DEMOUTIL.PAS}
{$ENDIF}

{ TMetafile }



function SystemCharset: byte;
begin
  case GetACP of
    874: Result := Windows.THAI_CHARSET;
    932: Result := Windows.SHIFTJIS_CHARSET;
    936: Result := Windows.GB2312_CHARSET;
    949: Result := Windows.HANGEUL_CHARSET;
    950: Result := Windows.CHINESEBIG5_CHARSET;
    1200: Result := Windows.DEFAULT_CHARSET;
    1250: Result := Windows.EASTEUROPE_CHARSET;
    1251: Result := Windows.RUSSIAN_CHARSET;
    1252: Result := Windows.DEFAULT_CHARSET;
    1253: Result := Windows.GREEK_CHARSET;
    1254: Result := Windows.TURKISH_CHARSET;
    1255: Result := Windows.HEBREW_CHARSET;
    1256: Result := Windows.ARABIC_CHARSET;
    1257: Result := Windows.BALTIC_CHARSET;
    else Result := Windows.DEFAULT_CHARSET;
  end;
end;

function ConvertCharset(Charset: byte): byte;
begin
  if Charset = Windows.DEFAULT_CHARSET then begin
    case GetUserDefaultLCID of
      $0401: Result := Windows.ARABIC_CHARSET; // Saudi Arabia (Arabic)
      $0404: Result := Windows.CHINESEBIG5_CHARSET; // Traditional Chinese (Taiwan)
      $0405: Result := Windows.EASTEUROPE_CHARSET; // Czechoslovakia
      $0408: Result := Windows.GREEK_CHARSET; // Greek (Greece)
      $040D: Result := Windows.HEBREW_CHARSET; // Israel (Hebrew)
      $040E: Result := Windows.EASTEUROPE_CHARSET; // Hungary (Hungarian)
      $0411: Result := Windows.SHIFTJIS_CHARSET; // Japan
      $0412: Result := Windows.HANGEUL_CHARSET; // Korea
      $0415: Result := Windows.EASTEUROPE_CHARSET; // Poland (Polish)
      $0418: Result := Windows.EASTEUROPE_CHARSET; // Romania (Romanian)
      $0419: Result := Windows.RUSSIAN_CHARSET; // Russia (Russian)
      $041A: Result := Windows.EASTEUROPE_CHARSET; // Croatia (Croatian)
      $041B: Result := Windows.EASTEUROPE_CHARSET; // Slovakia (Slovak)
      $041E: Result := Windows.THAI_CHARSET; // Thailand (Tha)
      $041F: Result := Windows.TURKISH_CHARSET; // Turkey (Turkish)
      $0424: Result := Windows.EASTEUROPE_CHARSET; // Slovenia (Slovenian)
      $042A: Result := Windows.VIETNAMESE_CHARSET; // Vietnam (Vietnamese)
      $0804: Result := Windows.GB2312_CHARSET;  // Simplified Chinese  (People's Republic of China)
      else Result := SystemCharset;
    end; { case }
  end else begin
    Result := Charset;
  end; { else }
end;

initialization
  Trans := DefaultTrans;
end.