{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit QRpBase;

{$T-}

interface

{$I RpVer.inc}

uses
  // Windows MUST be before Graphics/QGraphics because of TBitmap
  Windows,
  Types, Qt, QGraphics, QControls, QForms, QDialogs, QStdCtrls, QExtCtrls,
  Classes, SysUtils, QRpDefine, QRpDevice;

{ class TBaseReport }

{ TBaseReport is the base class for all of the reporting components.      }
{                                                                         }
{ Its functionality includes coding of all non-direct-output functions    }
{ (such as Print, but not PrintLeftInch) and setting up abstract          }
{ declarations of direct-output functions (such as PrintLeftInch and Arc) }
{                                                                         }
{ It will also contain code to retrieve printer specific information      }
{ since even in file mode, output must be directed towards a specific     }
{ printer for sizing and limits.                                          }

{$IFDEF BCB}
  (*$HPPEMIT '#undef CreateFont' *)
  (*$HPPEMIT '#undef ResetPrinter' *)
{$ENDIF}

type
  TRPBaseComponent = class(TRPComponent)
  protected
  end; { TRPBaseComponent }

  TBaseMemoBuf = class;
  TBaseReport = class(TRPBaseComponent)
  protected
  { Internal variables }
    TabHead: PTab; { Head of tab list }
    TabTail: PTab; { Tail of tab list }
    SavePosArr: array[1..MAXSAVEPOS] of TPosition; { Array of saved positions }
    SaveTabsArr: array[1..MAXSAVETABS] of TSaveTab; { Array of saved tabs }
    SaveFontArr: array[1..MAXSAVEFONT] of TSaveFont; { Array of saved fonts }
    PosStack: PPositionNode; { Saved position stack }
    FontStack: PFontNode; { Saved font stack }
    TabsStack: PTabNode; { Saved tabs stack }
    BlockTextOp: boolean;

    BaseFont: TSaveFont; { Saved font settings } {!!!jrg}
    FontList: PFontNode; { List of fonts that are used }
    FontSaveOnChange: TNotifyEvent; { Saved Canvas.Font.OnChange event handler }
    SaveFontHandle: HFont;
    BaseFontChanged: boolean; { Has the base font changed? }
    DisableFontChange: boolean;
    SaveCanvasFont: HFont;
    ValidDestination: set of TDestination; { Used with OutputInvalid }
    FJobPages: integer; { Total Pages in job }
    FMacroData: TStrings;
    FParaJustify: TTabJustify;
    FFixedTabWidth: double;
    FFrameMode: TFrameMode; { How is border for graphic shapes drawn? }

    CurrentTab: PTab; { Current tab in list }
    OnTab: boolean; { Are we on a tab right now? }
    BeforeTabs: boolean; { Are we before any tabs? }
    AddX: longint; { Amount to add to all measurements for preview }
    AddY: longint; { Amount to add to all measurements for preview }

    BeginPageList: TList; { List of begin page events }
    EndPageList: TList; { List of end page events }

    VLineList: PLineList; { Contains all vertical lines }
    HLineList: PLineList; { Contains all horizontal lines }
    LastLineX: double; { Last line point X }
    LastLineY: double; { Last line point Y }
    FNoBufferLine: boolean; { Whether to buffer lines or not }
    FNoNTColorFix: boolean; { Whether to fix font colors for NT or not }
    FUseDrawText: boolean; { Use DrawText instead of TextOut }

  { Printer defined properties }
    FLeftWaste: double; { Unprintable area on left side of page in inches }
    FRightWaste: double; { Unprintable area on right side of page in inches }
    FTopWaste: double; { Unprintable area on top of page in inches }
    FBottomWaste: double; { Unprintable area on bottom of page in inches }
    Metrict: QFontMetricsH;
    FAscentHeight: double; { Metrics.tmAscent in inches }
    FDescentHeight: double; { Metrics.tmDescent in inches }
    FLineHeight: double; { 1/FLinesPerInch or 1.2 * FontHeight or user defined }
    FOutputName: string; { Name of alternate output device }
    FPrinterIndex: integer; { Index of currently selected printer }

  { Printer dependent properties }
    FLeftMargin: double; { Position of left margin in inches }
    FRightMargin: double; { Position of right margin in inches }
    FTopMargin: double; { Position of top margin in inches }
    FBottomMargin: double; { Position of bottom margin in inches }
    FLeftSection: double; { Position of section left in inches }
    FRightSection: double; { Position of section right in inches }
    FTopSection: double; { Position of section top in inches }
    FBottomSection: double; { Position of section bottom in inches }
    FCursorXPos: double; { Current horizontal cursor position in inches }
    FCursorYPos: double; { Current position of baseline of text in inches }
    FColumnWidth: double; { Width of column in inches }
    FColumnBetween: double; { Width between columns in inches }

  { Property fields }
    FThreaded: boolean;
    FReportDateTime: TDateTime; { DateTime was started }
    FTabJustify: TTabJustify; { Tab justification override }
    FCurrentPage: integer; { Current page of report }
    FPageInvalid: boolean; { Is current page outside of FFirstPage-FLastPage }
    FXDPI: integer; { Dots per inch horizontally }
    FYDPI: integer; { Dots per inch vertically }
    FLineNum: integer; { Current Line Number }
    FColumnNum: integer; { Which column are we on }
    FColumns: integer; { How many columns are there? }
    FDevice: PChar; { Device name }
    FDriver: PChar; { Driver name }
    FPort: PChar; { Port of printer driver }
    FAborted: boolean; { Is current print job aborted }
    FPrinting: boolean; { Is current print job printing }
    FOriginX: double; { Horizontal origin in inches }
    FOriginY: double; { Vertical origin in inches }
    FFirstPage: integer; { First valid page }
    FLastPage: integer; { Last valid page }
    FSelection: TFormatString; { Page selection string }
    FStatusLabel: TLabel; { Label to display status in }
    FStatusFormat: TFormatString; { Format of status display }
    FStatusText: TStrings; { Info strings for status display }
    FLineHeightMethod: TLineHeightMethod; { How line height is figured }
    FLinesPerInch: integer; { Vertical lines per inch }
    FUnits: TPrintUnits; { Units type }
    FUnitsFactor: double; { Units factor }
    FLeftMarginWidth: double; { Width of left margin in units }
    FRightMarginWidth: double; { Width of right margin in units }
    FTopMarginWidth: double; { Width of top margin in units }
    FBottomMarginWidth: double; { Width of bottom margin in units }
    FTabShade: integer; { Shade for tab boxes if nothing else }
    FTabColor: TColor; { Color for tab box shading }
    FBoxLineColor: TColor; { Color for tab box lines }
    FTitle: TTitleString; { Title of report }
    FTextBKMode: TBKMode; { Current text background mode }
    FOrientation: TOrientation; { Orientation of page }
    FNewOrientation: TOrientation; { Orientation set to this on next NewPage }
    FCopies: integer; { DevMode^.dmCopies }
    FScaleX: double; { X Scale of printout - 100% norm, 10% small, 200% large }
    FScaleY: double; { Y Scale of printout - 100% norm, 10% small, 200% large }
    FZoom: double; { Internal zoom factor -  100% norm, 10% small, 200% large }
    FNoPrinterPageHeight: double; { PageHeight when no printers are defined }
    FNoPrinterPageWidth: double; { PageWidth when no printers are defined }
    FTransparentBitmaps: boolean; { Controls bitmap draw mode }
    FIgnoreRPTF: boolean; { Ignores RPTF commands }
    FCurrentPass: integer; { Used to determine which pass printer is on for
     printers that don't support printing multiple copies }
    FTotalPasses: integer;
    FTextFieldWidth: double; { Used by Rave for width of current text field }
    FPageNumOffset: integer; { Offset used by RelativePage }
    FLocalDevice: TRPBaseDevice; // Local Device class to use

  { Event fields }
    FOnPrint: TNotifyEvent;
    FOnPrintPage: TPrintPageEvent;
    FOnBeforePrint: TNotifyEvent;
    FOnAfterPrint: TNotifyEvent;
    FOnNewPage: TNotifyEvent;
    FOnNewColumn: TNotifyEvent;
    FOnPrintHeader: TNotifyEvent;
    FOnPrintFooter: TNotifyEvent;
    FOnEndOfSection: TNotifyEvent;
    FOnDecodeImage: TDecodeImageEvent;

  {$IFDEF DEMO}
    procedure PlaceHolder; virtual;
  {$ENDIF}
    procedure Notification(AComponent: TComponent;
                           Operation: TOperation); override;
    procedure SetStatusLabel(Value: TLabel);
    procedure PrintEvent; virtual;
    function PrintPageEvent: boolean; virtual;
    procedure BeforePrintEvent; virtual;
    procedure AfterPrintEvent; virtual;
    procedure NewPageEvent; virtual;
    procedure NewColumnEvent; virtual;
    procedure PrintHeaderEvent; virtual;
    procedure PrintFooterEvent; virtual;
    procedure EndOfSectionEvent; virtual;

    procedure AssignTo(Dest: TPersistent); override;
    procedure ExecuteReportEvent(ReportEventList: TList);
    procedure FontOnChange(Sender: TObject); virtual;
    procedure GrabFontOnChange; virtual;
    procedure ReleaseFontOnChange; virtual;
    procedure AdjustXY(Width: double);
    procedure ReleaseTabs(var TabPtr: PTab);
    procedure UpdateFont;
    procedure InitPrinter;
    procedure ClearLineLists;
    function BufferLine(X1,Y1: double): boolean;
    procedure DrawLineLists(Sender: TObject);

  { Report Methods - protected }
    function GetCursorXPos: longint;
    function GetCursorYPos: longint;
    function Destination: TDestination; virtual;
    procedure PrintPiece(Text: string);
    procedure PrintCenterInch(Text: string;
                              Pos: double); virtual; abstract;
    procedure PrintLeftInch(Text: string;
                            Pos: double); virtual; abstract;
    procedure PrintRightInch(Text: string;
                             Pos: double); virtual; abstract;
    procedure PrintBlockInch(Text: string;
                             Pos: double;
                             Width: double); virtual; abstract;
    procedure DrawTabLine(DrawLine: boolean;
                          LineWidth: integer;
                          X1,Y1: double;
                          XAdd,YAdd: double;
                          SavePen: TPen);
    function ConvertMacro(MacroID: TMacroID): string;
    procedure ProcessString(var Text: string);
    procedure CheckPrinting;
    procedure CheckNotPrinting;
    function PageInRange(PageNum: integer): boolean;
    procedure ClearFontList;

  { Property methods for public properties - protected }
    function GetCanvas: TCanvas; virtual;
    procedure SetLine(Value: integer);
    procedure SetColumn(Value: integer);
    function GetLeftWaste: double;
    function GetRightWaste: double;
    function GetTopWaste: double;
    function GetBottomWaste: double;
    function GetXPos: double;
    procedure SetXPos(Value: double);
    function GetYPos: double;
    procedure SetYPos(Value: double);
    function GetPageHeight: double;
    function GetPageWidth: double;
    function GetBold: boolean;
    procedure SetBold(Value: boolean);
    function GetUnderline: boolean;
    procedure SetUnderline(Value: boolean);
    function GetItalic: boolean;
    procedure SetItalic(Value: boolean);
    function GetStrikeout: boolean;
    procedure SetStrikeout(Value: boolean);
    function GetFontRotation: integer;
    procedure SetFontRotation(Value: integer);
    function GetFontColor: TColor;
    procedure SetFontColor(Value: TColor);
    function GetFontName: string;
    procedure SetFontName(Value: string);
    function GetFontSize: double;
    procedure SetFontSize(Value: double);
    function GetFontHandle: HFont;
    function GetFontPitch: TFontPitch;
    procedure SetFontPitch(Value: TFontPitch);
    function GetFontCharset: byte;
    procedure SetFontCharset(Value: byte);
    function GetFontAlign: TFontAlign;
    procedure SetFontAlign(Value: TFontAlign);
    function GetSuperscript: boolean;
    procedure SetSuperscript(Value: boolean);
    function GetSubscript: boolean;
    procedure SetSubscript(Value: boolean);
    function GetFontTop: double;
    procedure SetFontTop(Value: double);
    function GetFontBaseline: double;
    procedure SetFontBaseline(Value: double);
    function GetFontBottom: double;
    procedure SetFontBottom(Value: double);
    function GetLineTop: double;
    procedure SetLineTop(Value: double);
    function GetLineMiddle: double;
    procedure SetLineMiddle(Value: double);
    function GetLineBottom: double;
    procedure SetLineBottom(Value: double);
    function GetLineHeight: double;
    procedure SetLineHeight(Value: double);
    function GetAscentHeight: double;
    function GetDescentHeight: double;
    function GetFontHeight: double;
    procedure SetFontHeight(Value: double);
    function GetFontWidth: double;
    procedure SetFontWidth(Value: double);
    function GetOutputInvalid: boolean;
    procedure SetFrameMode(Value: TFrameMode); virtual;

    function GetDevMode: PDevMode;
    function GetPrinterIndex: integer;
    procedure SetPrinterIndex(Value: integer);
    function GetDeviceName: string;
    function GetDriverName: string;
    function GetPort: string;
    function GetOriginX: double;
    procedure SetOriginX(Value: double);
    function GetOriginY: double;
    procedure SetOriginY(Value: double);
    function GetBKColor: TColor;
    procedure SetBKColor(Value: TColor); virtual;
    procedure SetScaleX(Value: double); virtual;
    procedure SetScaleY(Value: double); virtual;
    procedure SetTextBKMode(Value: TBKMode); virtual;
    function GetLeftSection: double;
    procedure SetLeftSection(Value: double);
    function GetRightSection: double;
    procedure SetRightSection(Value: double);
    function GetTopSection: double;
    procedure SetTopSection(Value: double);
    function GetBottomSection: double;
    procedure SetBottomSection(Value: double);
  { Property methods for published properties - protected }
    procedure SetStatusText(Value: TStrings);
    procedure SetMacroData(Value: TStrings);
    procedure SetLinesPerInch(Value: integer);
    procedure SetUnits(Value: TPrintUnits);
    procedure SetUnitsFactor(Value: double);
    function GetLeftMargin: double;
    procedure SetLeftMargin(Value: double);
    function GetRightMargin: double;
    procedure SetRightMargin(Value: double);
    function GetTopMargin: double;
    procedure SetTopMargin(Value: double);
    function GetBottomMargin: double;
    procedure SetBottomMargin(Value: double);
    procedure SetTabShade(Value: integer);
    procedure SetCopies(Value: integer);
    procedure SetOrientation(Value: TOrientation); virtual;
    procedure SetOrientationActive;
    function GetColumnStart: double;
    function GetColumnEnd: double;
    function GetColumnWidth: double;
    procedure SetOutputName(Value: string);
    function GetBins: TStrings;
    function GetPapers: TStrings;
    function GetFonts: TStrings;
    function GetPrinters: TStrings;
    function GetMaxCopies: longint;
    function GetDuplex: TDuplex;
    procedure SetDuplex(Value: TDuplex);
    function GetCollate: boolean;
    procedure SetCollate(Value: boolean);
    function GetRelativePage: integer;
    function GetLocalDevice: TRPBaseDevice;
    procedure ProcessMessages;
  public
    function GetBaseFont: TSaveFont;
    procedure SetBaseFont(Font: TSaveFont);
    procedure SelectRPFont;
    procedure SelectCanvasFont;
    function LineStartPos: double;
    procedure AddReportEvent(ReportEventType: TReportEventType;
                             ReportEvent: TNotifyEvent);
    procedure RemoveReportEvent(ReportEventType: TReportEventType;
                                ReportEvent: TNotifyEvent);
    procedure TabRectangle(X1,Y1,X2,Y2: double); virtual; abstract;
    function GetTextExtent(Value: string): TSize; virtual;

  { Deprecated Output Methods }

    procedure CopyRect(const Dest: TRect;
                             Canvas: TCanvas;
                       const Source: TRect); virtual; {$IFDEF LEVEL6}deprecated;{$ENDIF} abstract;
    procedure FillRect(const Rect: TRect); virtual; {$IFDEF LEVEL6}deprecated;{$ENDIF} abstract;
    procedure DrawFocusRect(const Rect: TRect); virtual; {$IFDEF LEVEL6}deprecated;{$ENDIF} abstract;

  { Abstract Output Methods - public }
    procedure Arc(X1,Y1,X2,Y2,X3,Y3,X4,Y4: double); virtual; abstract;
    procedure Chord(X1,Y1,X2,Y2,X3,Y3,X4,Y4: double); virtual; abstract;
    procedure Draw(X,Y: double;
                    Graphic: TGraphic); virtual; abstract;
    procedure Ellipse(X1,Y1,X2,Y2: double); virtual; abstract;
    procedure LineTo(X,Y: double); virtual; abstract;
    procedure MoveTo(X,Y: double); virtual; abstract;
    procedure Pie(X1,Y1,X2,Y2,X3,Y3,X4,Y4: double); virtual; abstract;
    procedure Polygon(const Points: array of TPoint); virtual; abstract;
    procedure Polyline(const Points: array of TPoint); virtual; abstract;
    procedure PrintBitmap(X,Y: double;
                          ScaleX,ScaleY: double;
                          Bitmap: TBitmap); virtual; abstract;
    procedure PrintBitmapRect(X1,Y1,X2,Y2: double;
                              Bitmap: TBitmap); virtual; abstract;
    procedure PrintImageRect(X1,Y1,X2,Y2: double;
                             ImageStream: TStream;
                             ImageType: string); virtual; abstract;
    function CalcGraphicWidth(Height: double; Graphic: TGraphic): double;
    function CalcGraphicHeight(Width: double; Graphic: TGraphic): double;
    procedure Rectangle(X1,Y1,X2,Y2: double); virtual; abstract;
    procedure RoundRect(X1,Y1,X2,Y2,X3,Y3: double); virtual; abstract;
    procedure StretchDraw(const Rect: TRect;
                                Graphic: TGraphic); virtual; abstract;
    procedure TextRect(      Rect: TRect;
                             X,Y: double;
                       const Text: string); virtual; abstract;
    procedure PrintData(Value: string); virtual; abstract;
    procedure PrintDataStream(Stream: TStream;
                              BufSize: longint); virtual; abstract;
    function NoPrinters: boolean;
    procedure AllowPreviewOnly; virtual;
    procedure AllowPrinterOnly; virtual;
    procedure AllowAll; virtual;
    procedure AssignFont(Font: TFont);
    procedure UnregisterGraphic(Index: integer); virtual;
    procedure RegisterGraphic(Index: integer); virtual;
    procedure ReuseGraphic(Index: integer); virtual;

  { Report unit conversion methods }
    function XI2D(Pos: double): longint;
    function YI2D(Pos: double): longint;
    function XU2D(Pos: double): longint;
    function YU2D(Pos: double): longint;
    function XD2I(Pos: longint): double;
    function YD2I(Pos: longint): double;
    function XD2U(Pos: longint): double;
    function YD2U(Pos: longint): double;
    function XI2U(Pos: double): double;
    function YI2U(Pos: double): double;
    function XU2I(Pos: double): double;
    function YU2I(Pos: double): double;

  { Report methods - public }
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure Start; virtual; abstract;
    procedure Finish; virtual; abstract;
    procedure Execute; virtual;
    procedure NewPage; virtual;
    procedure NewPara; virtual;
    procedure AbortPage; virtual;

    function ShowPrintDialog: boolean;
    function ShowPrinterSetupDialog: boolean;
    procedure Abort;
    procedure UpdateStatus;
    procedure Reset;
    procedure ResetSection;
    procedure ResetPrinter;
    function SetPaperSize(Size: integer;
                          Width: double;
                          Height: double): boolean;
    procedure ReleasePrinter;
    procedure RecoverPrinter;
    function SelectBin(BinName: string): boolean; virtual;
    function SelectPaper(PaperName: string): boolean;
    function SelectPrinter(PrinterName: string): boolean;
    function SupportBin(BinNum: integer): boolean;
    function SupportPaper(PaperNum: integer): boolean;
    function SupportOrientation: boolean;
    function SupportDuplex: boolean;
    function SupportCollate: boolean;
    property Bins: TStrings read GetBins;
    property Papers: TStrings read GetPapers;
    property Fonts: TStrings read GetFonts;
    property Printers: TStrings read GetPrinters;
    property MaxCopies: longint read GetMaxCopies;
    property Duplex: TDuplex read GetDuplex write SetDuplex;
    property Collate: boolean read GetCollate write SetCollate;
    property ReportDateTime: TDateTime read FReportDateTime write FReportDateTime;
    property FrameMode: TFrameMode read FFrameMode write SetFrameMode default fmInside;
    property LocalDevice: TRPBaseDevice read GetLocalDevice write FLocalDevice;
    property JobPages: integer read FJobPages;

    procedure Cr;
    procedure Lf;
    procedure ResetLineHeight; virtual;
    procedure AdjustLine;
    procedure CrLf;
    procedure NewLine;
    procedure SoftLine;
    procedure NewColumn;
    procedure GotoHeader;
    procedure GotoFooter;
    procedure GotoXY(NewXPos: double;
                      NewYPos: double);
    procedure Home;
    procedure SetTopOfPage;
    function LinesLeft: integer;
    function ColumnLinesLeft: integer;
    function PopPos: boolean;
    function PushPos: boolean;
    function RestorePos(Index: integer): boolean;
    function SavePos(Index: integer): boolean;
    function PopTabs: boolean;
    function PushTabs: boolean;
    function RestoreTabs(Index: integer): boolean;
    function SaveTabs(Index: integer): boolean;
    function PopFont: boolean;
    function PushFont: boolean;
    function RestoreFont(Index: integer): boolean;
    function SaveFont(Index: integer): boolean;

  { Columns Methods }
    procedure SetColumns(NewColumns: integer;
                         Between: double);
    procedure SetColumnWidth(Width: double;
                             Between: double);
    procedure ClearColumns;

  { Tab methods }
    procedure SetTab(NewPos: double;
                     NewJustify: TPrintJustify;
                     NewWidth: double;
                     NewMargin: double;
                     NewLines: byte;
                     NewShade: byte);
    procedure ClearTabs;
    procedure ClearAllTabs;
    procedure ResetTabs;
    procedure Tab(LeftWidth: integer;
                  RightWidth: integer;
                  TopWidth: integer;
                  BottomWidth: integer;
                  ShadeOverride: integer); virtual;
    function GetTab(Index: integer): PTab;
    function TabStart(Index: integer): double;
    function TabEnd(Index: integer): double;
    function TabWidth(Index: integer): double;
    procedure FinishTabBox(Width: integer);

  { Misc methods }
    function CreatePoint(X,Y: double): TPoint;
    function CreateRect(X1,Y1,X2,Y2: double): TRect;
    function TextWidth(Text: string): double; virtual;

  { Memo methods }
    function MemoLines(MemoBuf: TBaseMemoBuf): longint;
    function GetMemoLine(    MemoBuf: TBaseMemoBuf;
                         var Eol: boolean): string;
    procedure PrintMemo(MemoBuf: TBaseMemoBuf;
                        Lines: longint;
                        PrintTabs: boolean); virtual;

  { Report Text Printing Methods }
    procedure PrintJustify(Text: string;
                           Pos: double;
                           Justify: TPrintJustify;
                           Margin: double;
                           Width: double);
    procedure Print(Text: string);
    procedure PrintCenter(Text: string;
                          Pos: double);
    procedure PrintHeader(Text: string;
                          Justify: TPrintJustify);
    procedure PrintFooter(Text: string;
                          Justify: TPrintJustify);
    procedure PrintLeft(Text: string;
                        Pos: double);
    procedure PrintBlock(Text: string;
                         Pos: double;
                         Width: double);
    procedure Println(Text: string);
    procedure PrintRight(Text: string;
                          Pos: double);
    procedure PrintXY(X,Y: double;
                      Text: string);
    procedure PrintCharJustify(Text: string;
                               Ch: char;
                               Pos: double);
    procedure PrintTab(Text: string); virtual;
    function TruncateText(Value: string;
                          Width: double): string;
    function Macro(MacroID: TMacroID): string;
    function PIVar(VarName: string): string; virtual;
    procedure SetPIVar(VarName: string;
                         VarValue: string); virtual;
    function GetPIVar(VarName: string): string; virtual;

  { Canvas Drawing Object Methods }
    function CreateBrush(NewColor: TColor;
                         NewStyle: TBrushStyle;
                         NewBitmap: TBitmap): TBrush;
    function CreateFont(NewName: string;
                        NewSize: integer): TFont;
    function CreatePen(NewColor: TColor;
                       NewStyle: TPenStyle;
                       NewWidth: integer;
                       NewMode: TPenMode): TPen;

    procedure SetBrush(NewColor: TColor;
                       NewStyle: TBrushStyle;
                       NewBitmap: TBitmap); virtual;
    procedure SetFont(NewName: string;
                      NewSize: double); virtual;
    procedure SetPen(NewColor: TColor;
                     NewStyle: TPenStyle;
                     NewWidth: integer;
                     NewMode: TPenMode); virtual;

  { Properties }
    property TextFieldWidth: double read FTextFieldWidth write FTextFieldWidth;
    property Threaded: boolean read FThreaded write FThreaded;
    property IgnoreRPTF: boolean read FIgnoreRPTF write FIgnoreRPTF;
    property CursorXPos: longint read GetCursorXPos;
    property CursorYPos: longint read GetCursorYPos;
    property XPos: double read GetXPos write SetXPos;
    property YPos: double read GetYPos write SetYPos;
    property Canvas: TCanvas read GetCanvas;
    property CurrentPage: integer read FCurrentPage;
    property RelativePage: integer read GetRelativePage;
    property PageNumOffset: integer read FPageNumOffset write FPageNumOffset;
    property PageInvalid: boolean read FPageInvalid;
    property OutputInvalid: boolean read GetOutputInvalid;
    property XDPI: integer read FXDPI;
    property YDPI: integer read FYDPI;
    property LineNum: integer read FLineNum write SetLine;
    property ColumnNum: integer read FColumnNum write SetColumn;
    property Columns: integer read FColumns;
    property ColumnStart: double read GetColumnStart;
    property ColumnEnd: double read GetColumnEnd;
    property ColumnWidth: double read GetColumnWidth;
    property LeftWaste: double read GetLeftWaste;
    property RightWaste: double read GetRightWaste;
    property TopWaste: double read GetTopWaste;
    property BottomWaste: double read GetBottomWaste;
    property SectionLeft: double read GetLeftSection write SetLeftSection;
    property SectionRight: double read GetRightSection write SetRightSection;
    property SectionTop: double read GetTopSection write SetTopSection;
    property SectionBottom: double read GetBottomSection write SetBottomSection;
    property PageHeight: double read GetPageHeight;
    property PageWidth: double read GetPageWidth;
    property Bold: boolean read GetBold write SetBold;
    property Underline: boolean read GetUnderline write SetUnderline;
    property Italic: boolean read GetItalic write SetItalic;
    property Strikeout: boolean read GetStrikeout write SetStrikeout;
    property Superscript: boolean read GetSuperscript write SetSuperscript;
    property Subscript: boolean read GetSubscript write SetSubscript;
    property FontRotation: integer read GetFontRotation write SetFontRotation;
    property FontColor: TColor read GetFontColor write SetFontColor;
    property FontName: string read GetFontName write SetFontName;
    property FontSize: double read GetFontSize write SetFontSize;
    property FontHandle: HFont read GetFontHandle;
    property FontPitch: TFontPitch read GetFontPitch write SetFontPitch;
    property FontCharset: byte read GetFontCharset write SetFontCharset;
    property FontAlign: TFontAlign read GetFontAlign write SetFontAlign;
    property LineTop: double read GetLineTop write SetLineTop;
    property LineMiddle: double read GetLineMiddle write SetLineMiddle;
    property LineBottom: double read GetLineBottom write SetLineBottom;
    property FontTop: double read GetFontTop write SetFontTop;
    property FontBaseline: double read GetFontBaseline write SetFontBaseline;
    property FontBottom: double read GetFontBottom write SetFontBottom;
    property TabJustify: TTabJustify read FTabJustify write FTabJustify;
    property TransparentBitmaps: boolean read FTransparentBitmaps write
     FTransparentBitmaps;
    property ParaJustify: TTabJustify read FParaJustify write FParaJustify default
     tjNone;
    property NoBufferLine: boolean read FNoBufferLine write FNoBufferLine;
    property NoNTColorFix: boolean read FNoNTColorFix write FNoNTColorFix;
    property UseDrawText: boolean read FUseDrawText write FUseDrawText;
    property CurrentPass: integer read FCurrentPass write FCurrentPass;
    property TotalPasses: integer read FTotalPasses write FTotalPasses;
    property FixedTabWidth: double read FFixedTabWidth write FFixedTabWidth;

    property PrinterIndex: integer read GetPrinterIndex write SetPrinterIndex
     nodefault;
    property DeviceName: string read GetDeviceName;
    property DriverName: string read GetDriverName;
    property Port: string read GetPort;
    property Aborted: boolean read FAborted;
    property Printing: boolean read FPrinting;
    property OriginX: double read GetOriginX write SetOriginX;
    property OriginY: double read GetOriginY write SetOriginY;
    property DevMode: PDevMode read GetDevMode;
    property BKColor: TColor read GetBKColor write SetBKColor;
    property LineHeight: double read GetLineHeight write SetLineHeight;
    property AscentHeight: double read GetAscentHeight;
    property DescentHeight: double read GetDescentHeight;
    property FontHeight: double read GetFontHeight write SetFontHeight;
    property FontWidth: double read GetFontWidth write SetFontWidth;
    property OutputName: string read FOutputName write SetOutputName;
    property NoPrinterPageHeight: double read FNoPrinterPageHeight write
     FNoPrinterPageHeight;
    property NoPrinterPageWidth: double read FNoPrinterPageWidth write
     FNoPrinterPageWidth;
    property MacroData: TStrings read FMacroData write SetMacroData;

  { Properties - potential published }
    property FirstPage: integer read FFirstPage write FFirstPage default 1;
    property LastPage: integer read FLastPage write FLastPage default 9999;
    property Selection: TFormatString read FSelection write FSelection;
    property StatusLabel: TLabel read FStatusLabel write SetStatusLabel;
    property StatusFormat: TFormatString read FStatusFormat write FStatusFormat;
    property StatusText: TStrings read FStatusText write SetStatusText;
    property LineHeightMethod: TLineHeightMethod read FLineHeightMethod write
     FLineHeightMethod default lhmFont;
    property LinesPerInch: integer read FLinesPerInch write SetLinesPerInch
     default 6;
    property Units: TPrintUnits read FUnits write SetUnits default unInch;
    property UnitsFactor: double read FUnitsFactor write SetUnitsFactor;
    property MarginLeft: double read GetLeftMargin write SetLeftMargin;
    property MarginRight: double read GetRightMargin write SetRightMargin;
    property MarginTop: double read GetTopMargin write SetTopMargin;
    property MarginBottom: double read GetBottomMargin write SetBottomMargin;
    property TabShade: integer read FTabShade write SetTabShade default 0;
    property TabColor: TColor read FTabColor write FTabColor default clBlack;
    property BoxLineColor: TColor read FBoxLineColor write FBoxLineColor default clBlack;
    property Title: TTitleString read FTitle write FTitle;
    property TextBKMode: TBKMode read FTextBKMode write SetTextBKMode
     default bkTransparent;
    property Copies: integer read FCopies write SetCopies default 1;
    property Orientation: TOrientation read FOrientation write
     SetOrientation nodefault;
    property ScaleX: double read FScaleX write SetScaleX;
    property ScaleY: double read FScaleY write SetScaleY;

  { Event properties - potential published }
    property OnPrint: TNotifyEvent read FOnPrint write FOnPrint;
    property OnPrintPage: TPrintPageEvent read FOnPrintPage write FOnPrintPage;
    property OnBeforePrint: TNotifyEvent read FOnBeforePrint write
     FOnBeforePrint;
    property OnAfterPrint: TNotifyEvent read FOnAfterPrint write FOnAfterPrint;
    property OnNewPage: TNotifyEvent read FOnNewPage write FOnNewPage;
    property OnNewColumn: TNotifyEvent read FOnNewColumn write FOnNewColumn;
    property OnPrintHeader: TNotifyEvent read FOnPrintHeader write
     FOnPrintHeader;
    property OnPrintFooter: TNotifyEvent read FOnPrintFooter write
     FOnPrintFooter;
    property OnEndOfSection: TNotifyEvent read FOnEndOfSection write
     FOnEndOfSection;
    property OnDecodeImage: TDecodeImageEvent read FOnDecodeImage write
     FOnDecodeImage;
  end; { TBaseReport }

  TBaseMemoBuf = class
  protected
    FBaseReport: TBaseReport;
    FHardLines: boolean;
    FJustify: TPrintJustify; { Justification of memo in print box }
    FKeepSpaces: boolean; { Keep spaces on the beginning of each line }
    FPrintStart: double; { Start of printing column in units }
    FPrintEnd: double; { End of printing column in units }
    FRTFMode: byte;
  public
    function GetNextLine(var Eol: boolean): string; virtual; abstract;
    function MemoLinesLeft: longint; virtual; abstract;
    function PrintLines(Lines: longint;
                        PrintTabs: boolean): double; virtual; abstract;

    property BaseReport: TBaseReport read FBaseReport write FBaseReport;
    property HardLines: boolean read FHardLines write FHardLines;
    property Justify: TPrintJustify read FJustify write FJustify;
    property KeepSpaces: boolean read FKeepSpaces write FKeepSpaces;
    property PrintStart: double read FPrintStart write FPrintStart;
    property PrintEnd: double read FPrintEnd write FPrintEnd;
    property RTFMode: byte read FRTFMode write FRTFMode;
  end; { TBaseMemoBuf }

implementation

uses
  QRpRPTF;

var
  CurrCharset: byte;

  function EnumFontFamExProc(var EnumLogFontEx: TEnumLogFontEx;
                                 TextMetric: PNewTextMetric;
                                 FontType: integer;
                                 Data: LPARAM): integer; stdcall;

  begin { EnumFontFamExProc }
    with TextMetric^ do begin
      if CurrCharset = Windows.DEFAULT_CHARSET then begin
        CurrCharset := tmCharSet;
        Result := 1; { get next font }
      end else begin
        if CurrCharset = tmCharSet then begin
          Result := 1; { get next font }
        end else begin
          CurrCharset := Windows.DEFAULT_CHARSET;
          Result := 0; { more than one type found, we're done }
        end; { else }
      end; { else }
    end; { with }
  end;  { EnumFontFamExProc }

  constructor TBaseReport.Create(AOwner: TComponent);

  var
    I1: integer;

  begin { Create }
    inherited Create(AOwner);

  {$IFDEF DEMO}
    if not Validate then begin
      DisplayWarning;
      Halt(1);
    end; { if }
  {$ENDIF}

  { Misc Inits }
    AddX := 0;
    AddY := 0;
    FScaleX := 100.0;
    FScaleY := 100.0;
    FXDPI := 300;
    FYDPI := 300;
    FZoom := 100.0;
    FUnits := unInch;
    FUnitsFactor := 1.0;
    FDevice := StrAlloc(256);
    FDriver := StrAlloc(256);
    FPort := StrAlloc(256);
    TabHead := nil;
    TabTail := nil;
    OnTab := false;
    BeforeTabs := false;
    CurrentTab := nil;
    PosStack := nil;
    FontStack := nil;
    TabsStack := nil;
    FillChar(SavePosArr,SizeOf(SavePosArr),0);
    FillChar(SaveTabsArr,SizeOf(SaveTabsArr),0);
    FillChar(SaveFontArr,SizeOf(SaveFontArr),0);
    FPageInvalid := false;
    ValidDestination := [destPrinter,destPreview,destFile,destNone,destCustomFile];
    FAborted := false;
    FPrinting := false;
    FNoPrinterPageWidth := 8.5;
    FNoPrinterPageHeight := 11.0;
    FTabJustify := tjNone;
    FTransparentBitmaps := false;
    FParaJustify := tjNone;
  { Init BaseFont }
    with BaseFont do begin
      Name := {Trans-}'Arial';
      Color := clBlack;
      //TODO: Fix this
      //Pitch :=  fpDefault;
      
      CharSet := Ord(fcsDefaultCharSet);
      Size := 10;
      Height := Size / 72.0;
      Width := 0.0;
      Ascent := Height / 2.0;
      Descent := Ascent;
      InternalLeading := 0.0;
      Bold := false;
      Underline := false;
      Italic := false;
      Strikeout := false;
      Subscript := false;
      Superscript := false;
      Rotation := 0;
      Align := faBaseline;
    end; { with }
    for I1 := 1 to MAXSAVEFONT do begin
      SaveFont(I1);
    end; { for }
    BaseFontChanged := true;
    FontList := nil;
    VLineList := nil;
    HLineList := nil;

  { Property Inits }
    if NoPrinters or (RPDev = nil) then begin
      FPrinterIndex := -1;
    end else begin
      FPrinterIndex := RPDev.DeviceIndex;
    end; { else }
    ResetPrinter;
    FCurrentPage := 0;
    FStatusLabel := nil;
    FStatusText := TStringList.Create;
    FStatusFormat := Trans('Printing page %p');
    FMacroData := TStringList.Create;
    Copies := 1;
    FirstPage := 1;
    LastPage := 9999;
    Selection := '';
    FJobPages := LastPage;
    LineHeightMethod := lhmFont; //!!! Need to change to lhmLinesPerInch for Archive
    LinesPerInch := 6;
    TabShade := 0;
    TabColor := clBlack;
    BoxLineColor := clBlack;
    TextBKMode := bkTransparent;
    Title := {Trans-}'Rave Report';
    CurrentPass := 1;
    TotalPasses := 1;
    BeginPageList := TList.Create;
    EndPageList := TList.Create;
    AddReportEvent(reEndPage,DrawLineLists);
    FixedTabWidth := 0.5;

  {$IFDEF DEMO}
    if not Validate then begin
      Halt(1);
    end; { if }
  {$ENDIF}
  end;  { Create }

  destructor TBaseReport.Destroy;

    procedure FreeList(List: TList);

    var
      P1: pointer;

    begin { FreeList }
      if Assigned(List) then begin
        while List.Count > 0 do begin
          P1 := List[0];
          FreeMem(P1,SizeOf(TNotifyEvent));
          List.Delete(0);
        end; { while }
        List.Free;
      end; { if }
    end;  { FreeList }

  begin { Destroy }
    ClearFontList;
    ClearLineLists;
    FreeList(EndPageList);
    FreeList(BeginPageList);
    while PopPos do begin
    end; { while }
    while PopTabs do begin
    end; { while }
    while PopFont do begin
    end; { while }
    ClearAllTabs;
    FreeAndNil(FStatusText);
    FreeAndNil(FMacroData);
    StrDispose(FDevice);
    StrDispose(FDriver);
    StrDispose(FPort);
    if not NoPrinters then begin
      if LocalDevice.Printing then begin
        LocalDevice.Abort;
      end; { if }
    end; { if }
    inherited Destroy;
  end;  { Destroy }

{$IFDEF DEMO}
  procedure TBaseReport.PlaceHolder; begin end;
{$ENDIF}

  procedure TBaseReport.AssignTo(Dest: TPersistent);

  begin { AssignTo }
  { Assign all published properties }
    with Dest as TBaseReport do begin
      FirstPage := self.FirstPage;
      LastPage := self.LastPage;
      Selection := self.Selection;
      StatusLabel := self.StatusLabel;
      StatusFormat := self.StatusFormat;
      StatusText := self.StatusText;
      LineHeightMethod := self.LineHeightMethod;
      FLineHeight := self.FLineHeight;
      LinesPerInch := self.LinesPerInch;
      Units := self.Units;
      UnitsFactor := self.UnitsFactor;
      MarginLeft := self.MarginLeft;
      MarginRight := self.MarginRight;
      MarginTop := self.MarginTop;
      MarginBottom := self.MarginBottom;
      TabShade := self.TabShade;
      TabColor := self.TabColor;
      BoxLineColor := self.BoxLineColor;
      Title := self.Title;
      TextBKMode := self.TextBKMode;
      Copies := self.Copies;
      Orientation := self.Orientation;
      ScaleX  := self.ScaleX;
      ScaleY := self.ScaleY;
      MacroData := self.MacroData;

    { Assign all published events }
      OnPrint := self.OnPrint;
      OnPrintPage := self.OnPrintPage;
      OnBeforePrint := self.OnBeforePrint;
      OnAfterPrint := self.OnAfterPrint;
      OnNewPage := self.OnNewPage;
      OnNewColumn := self.OnNewColumn;
      OnPrintHeader := self.OnPrintHeader;
      OnPrintFooter := self.OnPrintFooter;
    end; { with }
  end;  { AssignTo }

  procedure TBaseReport.AddReportEvent(ReportEventType: TReportEventType;
                                       ReportEvent: TNotifyEvent);

  var
    P1: ^TMethod;

  begin { AddReportEvent }
    GetMem(P1,SizeOf(TMethod));
    P1^ := TMethod(ReportEvent);

    case ReportEventType of
      reBeginPage: begin
        if BeginPageList.Count = 0 then begin
          BeginPageList.Add(P1);
        end else begin
          BeginPageList.Insert(0,P1);
        end; { else }
      end;
      reEndPage: begin
        if EndPageList.Count = 0 then begin
          EndPageList.Add(P1);
        end else begin
          EndPageList.Insert(0,P1);
        end; { else }
      end;
    end; { case }
  end;  { AddReportEvent }

  procedure TBaseReport.RemoveReportEvent(ReportEventType: TReportEventType;
                                          ReportEvent: TNotifyEvent);

    procedure RemoveEventFromList(ReportEvent: TNotifyEvent;
                                  EventList: TList);

    var
      I1: integer;
      P1: pointer;

    begin { RemoveEventFromList }
      for I1 := 0 to (EventList.Count - 1) do begin
        P1 := EventList[I1];
        if (TMethod(ReportEvent).Code = TMethod(P1^).Code) and
         (TMethod(ReportEvent).Data = TMethod(P1^).Data) then begin
          EventList.Remove(P1);
          FreeMem(P1,SizeOf(TMethod));
          Break;
        end; { if }
      end; { for }
    end;  { RemoveEventFromList }

  begin { RemoveReportEvent }
    case ReportEventType of
      reBeginPage: RemoveEventFromList(ReportEvent,BeginPageList);
      reEndPage: RemoveEventFromList(ReportEvent,EndPageList);
    end; { case }
  end;  { RemoveReportEvent }

  function TBaseReport.GetTextExtent(Value: string): TSize;

  begin { GetTextExtent }
    Result.cX := 0;
    Result.cY := 0;
    if Value <> '' then begin
      Canvas.Start;
      Windows.GetTextExtentPoint32(QPainter_Handle(Canvas.Handle),@Value[1],Length(Value),Result);
      Canvas.Stop;
    end; { if }
  end;  { GetTextExtent }

  procedure TBaseReport.AdjustXY(Width: double);

  begin { AdjustXY }
    if BaseFont.Rotation = 0 then begin
      FCursorXPos := FCursorXPos + ((XU2I(Width) * 10000.0) / (FScaleX * FZoom));
    end else begin
      FCursorXPos := FCursorXPos + ((Cos(Pi * BaseFont.Rotation / 180.0) *
       XU2I(Width) * 10000.0) / (FScaleX * FZoom));
      FCursorYPos := FCursorYPos - ((Sin(Pi * BaseFont.Rotation / 180.0) *
       XU2I(Width) * 10000.0) / (FScaleY * FZoom));
    end; { else }
  end;  { AdjustXY }

  procedure TBaseReport.ExecuteReportEvent(ReportEventList: TList);

  var
    I1: integer;
    ReportEvent: TNotifyEvent;
    P1: pointer;

  begin { ExecuteReportEvent }
    for I1 := 0 to (ReportEventList.Count - 1) do begin
      P1 := ReportEventList[I1];
      ReportEvent := TNotifyEvent(P1^);
      ReportEvent(self);
    end; { for }
  end;  { ExecuteReportEvent }

  procedure TBaseReport.Notification(AComponent: TComponent;
                                     Operation: TOperation);

  begin { Notification }
    inherited Notification(AComponent,Operation);
    if (Operation = opRemove) and (AComponent = FStatusLabel) then begin
      StatusLabel := nil;
    end; { if }
  end;  { Notification }

  procedure TBaseReport.SetStatusLabel(Value: TLabel);

  begin { SetStatusLabel }
    FStatusLabel := Value;
    if Assigned(Value) then begin
      Value.FreeNotification(self);
    end; { if }
  end;  { SetStatusLabel }

  procedure TBaseReport.PrintEvent;

  begin { PrintEvent }
    if Assigned(FOnPrint) then begin
      FOnPrint(self);
    end; { if }
  end;  { PrintEvent }

  function TBaseReport.PrintPageEvent: boolean;

  begin { PrintPageEvent }
    if Assigned(FOnPrintPage) then begin
      Result := FOnPrintPage(self,FCurrentPage);
    end else begin
      Result := false;
    end; { else }
  end;  { PrintPageEvent }

  procedure TBaseReport.BeforePrintEvent;

  begin { BeforePrintEvent }
    if Assigned(FOnBeforePrint) then begin
      FOnBeforePrint(self);
      if (FOrientation <> FNewOrientation) or (FOrientation <> poDefault) then begin
        SetOrientationActive;
      end; { if }
    end; { if }
  end;  { BeforePrintEvent }

  procedure TBaseReport.AfterPrintEvent;

  begin { AfterPrintEvent }
    if Assigned(FOnAfterPrint) then begin
      FOnAfterPrint(self);
    end; { if }
  end;  { AfterPrintEvent }

  procedure TBaseReport.NewPageEvent;

  begin { NewPageEvent }
    if Assigned(FOnNewPage) then begin
      FOnNewPage(self);
    end; { if }
  end;  { NewPageEvent }

  procedure TBaseReport.NewColumnEvent;

  begin { NewColumnEvent }
    if Assigned(FOnNewColumn) then begin
      FOnNewColumn(self);
    end; { if }
  end;  { NewColumnEvent }

  procedure TBaseReport.PrintHeaderEvent;

  begin { PrintHeaderEvent }
    if Assigned(FOnPrintHeader) then begin
      FOnPrintHeader(self);
    end; { if }
  end;  { PrintHeaderEvent }

  procedure TBaseReport.PrintFooterEvent;

  begin { PrintFooterEvent }
    if Assigned(FOnPrintFooter) then begin
      FOnPrintFooter(self);
    end; { if }
  end;  { PrintFooterEvent }

  procedure TBaseReport.EndOfSectionEvent;

  begin { EndOfSectionEvent }
    if Assigned(FOnEndOfSection) then begin
      FOnEndOfSection(self);
    end; { if }
  end;  { EndOfSectionEvent }

{ Report Methods - protected }
  function TBaseReport.GetCursorXPos: longint;

  begin { GetCursorXPos }
    Result := XI2D(FCursorXPos);
  end;  { GetCursorXPos }

  function TBaseReport.GetCursorYPos: longint;

  begin { GetCursorYPos }
    Result := YI2D(FCursorYPos);
  end;  { GetCursorYPos }

  function TBaseReport.Destination: TDestination;

  begin { Destination }
    Result := destPrinter;
  end;  { Destination }

  function TBaseReport.LineStartPos: double;

  begin { LineStartPos }
    Result := FLeftSection + (ColumnNum - 1) * FColumnWidth;
  end;  { LineStartPos }

{ Property read/write methods }

  function TBaseReport.GetCanvas: TCanvas;
  begin { GetCanvas }
    if LocalDevice.Printing then begin
      Result := LocalDevice.Canvas;
    end else begin
      Result := nil;
      RaiseError(Trans('Error!  Canvas not yet active.'));
    end; { else }
  end;  { GetCanvas }

  procedure TBaseReport.SetStatusText(Value: TStrings);

  begin { SetStatusText }
    FStatusText.Assign(Value);
  end;  { SetStatusText }

  procedure TBaseReport.SetMacroData(Value: TStrings);

  begin { SetMacroData }
    FMacroData.Assign(Value);
  end;  { SetMacroData }

  procedure TBaseReport.SetLinesPerInch(Value: integer);

  begin { SetLinesPerInch }
    if Value > 0 then begin
      FLinesPerInch := Value;
      ResetLineHeight;
    end; { if }
  end;  { SetLinesPerInch }

  procedure TBaseReport.SetLine(Value: integer);

  begin { SetLine }
    FLineNum := Value;
    ResetLineHeight;
    ResetTabs;
    FCursorXPos := LineStartPos;
    FCursorYPos := FTopSection + FLineHeight * FLineNum - FDescentHeight;
  end;  { SetLine }

  procedure TBaseReport.SetColumn(Value: integer);

  begin { SetColumn }
    FColumnNum := Value;
    FCursorXPos := LineStartPos;
    ResetTabs;
  end;  { SetColumn }

  procedure TBaseReport.SetLeftMargin(Value: double);

  begin { SetLeftMargin }
    FLeftMargin := XU2I(Value);
    SectionLeft := XI2U(FLeftMargin);
    FLeftMarginWidth := Value;
  end;  { SetLeftMargin }

  function TBaseReport.GetLeftMargin: double;

  begin { GetLeftMargin }
    Result := FLeftMarginWidth;
  end;  { GetLeftMargin }

  procedure TBaseReport.SetRightMargin(Value: double);

  begin { SetRightMargin }
    if NoPrinters then begin
      FRightMargin := XU2I(FNoPrinterPageWidth - Value);
    end else begin
      FRightMargin := (LocalDevice.PageWidth / FXDPI) + FLeftWaste + FRightWaste -
       XU2I(Value);
    end; { else }
    SectionRight := XI2U(FRightMargin);
    FRightMarginWidth := Value;
  end;  { SetRightMargin }

  function TBaseReport.GetRightMargin: double;

  begin { GetRightMargin }
    Result := FRightMarginWidth;
  end;  { GetRightMargin }

  procedure TBaseReport.SetTopMargin(Value: double);

  begin { SetTopMargin }
    FTopMargin := YU2I(Value);
    SectionTop := YI2U(FTopMargin);
    FTopMarginWidth := Value;
  end;  { SetTopMargin }

  function TBaseReport.GetTopMargin: double;

  begin { GetTopMargin }
    Result := FTopMarginWidth;
  end;  { GetTopMargin }

  procedure TBaseReport.SetBottomMargin(Value: double);

  begin { SetBottomMargin }
    if NoPrinters then begin
      FBottomMargin := YU2I(FNoPrinterPageHeight - Value);
    end else begin
      FBottomMargin := (LocalDevice.PageHeight / FYDPI) + FTopWaste + FBottomWaste -
       YU2I(Value);
    end; { else }
    SectionBottom := YI2U(FBottomMargin);
    FBottomMarginWidth := Value;
  end;  { SetBottomMargin }

  function TBaseReport.GetBottomMargin: double;

  begin { GetBottomMargin }
    Result := FBottomMarginWidth;
  end;  { GetBottomMargin }

  procedure TBaseReport.SetLeftSection(Value: double);

  begin { SetLeftSection }
    FLeftSection := XU2I(Value);
    if FCursorXPos < FLeftSection then begin
      FCursorXPos := FLeftSection;
    end; { if }
  end;  { SetLeftSection }

  function TBaseReport.GetLeftSection: double;

  begin { GetLeftSection }
    Result := XI2U(FLeftSection);
  end;  { GetLeftSection }

  procedure TBaseReport.SetRightSection(Value: double);

  begin { SetRightSection }
    FRightSection := XU2I(Value);
    if FCursorXPos > FRightSection then begin
      FCursorXPos := FRightSection;
    end; { if }
  end;  { SetRightSection }

  function TBaseReport.GetRightSection: double;

  begin { GetRightSection }
    Result := XI2U(FRightSection);
  end;  { GetRightSection }

  procedure TBaseReport.SetTopSection(Value: double);

  begin { SetTopSection }
    FTopSection := YU2I(Value);
    if FCursorYPos < FTopSection then begin
      FCursorYPos := FTopSection;
    end; { if }
  end;  { SetTopSection }

  function TBaseReport.GetTopSection: double;

  begin { GetTopSection }
    Result := YI2U(FTopSection);
  end;  { GetTopSection }

  procedure TBaseReport.SetBottomSection(Value: double);

  begin { SetBottomSection }
    FBottomSection := YU2I(Value);
    if FCursorYPos > FBottomSection then begin
      FCursorYPos := FBottomSection;
    end; { if }
  end;  { SetBottomSection }

  function TBaseReport.GetBottomSection: double;

  begin { GetBottomSection }
    Result := YI2U(FBottomSection);
  end;  { GetBottomSection }

  procedure TBaseReport.SetTabShade(Value: integer);

  begin { SetTabShade }
    if Value >= 100 then begin
      FTabShade := 100;
    end else if Value <= 0 then begin
      FTabShade := 0;
    end else begin
      FTabShade := Value;
    end; { else }
  end;  { SetTabShade }

  function TBaseReport.GetLeftWaste: double;

  begin { GetLeftWaste }
    Result := XI2U(FLeftWaste);
  end;  { GetLeftWaste }

  function TBaseReport.GetRightWaste: double;

  begin { GetRightWaste }
    Result := XI2U(FRightWaste);
  end;  { GetRightWaste }

  function TBaseReport.GetTopWaste: double;

  begin { GetTopWaste }
    Result := YI2U(FTopWaste);
  end;  { GetTopWaste }

  function TBaseReport.GetBottomWaste: double;

  begin { GetBottomWaste }
    Result := YI2U(FBottomWaste);
  end;  { GetBottomWaste }

  function TBaseReport.GetXPos: double;

  begin { GetXPos }
    Result := XI2U(FCursorXPos);
  end;  { GetXPos }

  procedure TBaseReport.SetXPos(Value: double);

  begin { SetXPos }
    FCursorXPos := XU2I(Value);
  end;  { SetXPos }

  function TBaseReport.GetYPos: double;

  begin { GetYPos }
    Result := YI2U(FCursorYPos);
  end;  { GetYPos }

  procedure TBaseReport.SetYPos(Value: double);

  begin { SetYPos }
    ResetLineHeight;
    FCursorYPos := YU2I(Value);
  end;  { SetYPos }

  procedure TBaseReport.SetUnits(Value: TPrintUnits);

  begin { SetUnits }
    FUnits := Value;
    case FUnits of
      unInch: begin
        FUnitsFactor := 1.0;
      end;
      unMM: begin
        FUnitsFactor := 25.4;
      end;
      unCM: begin
        FUnitsFactor := 2.54;
      end;
      unPoint: begin
        FUnitsFactor := 72.0;
      end;
      unUser: begin
      { Don't change FUnitsFactor }
      end;
    end; { case }
  end;  { SetUnits }

  procedure TBaseReport.SetUnitsFactor(Value: double);

  var
    R1: array [1..4] of double;

  begin { SetUnitsFactor }
    if Value > 0.0 then begin
      FUnitsFactor := Value;
      R1[1] := 1.0;
      R1[2] := 25.4;
      R1[3] := 2.54;
      R1[4] := 72.0;
      if (FUnitsFactor = R1[1]) then begin
        FUnits := unInch;
      end else if (FUnitsFactor = R1[2]) then begin
        FUnits := unMM;
      end else if (FUnitsFactor = R1[3]) then begin
        FUnits := unCM;
      end else if (FUnitsFactor = R1[4]) then begin
        FUnits := unPoint;
      end else begin
        FUnits := unUser;
      end; { else }
    end; { if }
  end;  { SetUnitsFactor }

  function TBaseReport.GetPageHeight: double;

  begin { GetPageHeight }
    if NoPrinters then begin
      Result := NoPrinterPageHeight;
    end else begin
      Result := YI2U((LocalDevice.PageHeight / FYDPI) + FTopWaste + FBottomWaste);
    end; { else }
  end;  { GetPageHeight }

  function TBaseReport.GetPageWidth: double;

  begin { GetPageWidth }
    if NoPrinters then begin
      Result := NoPrinterPageWidth;
    end else begin
      Result := XI2U((LocalDevice.PageWidth / FXDPI) + FLeftWaste + FRightWaste);
    end; { else }
  end;  { GetPageWidth }

  procedure TBaseReport.SetOrientation(Value: TOrientation);

  begin { SetOrientation }
    FNewOrientation := Value;
    if not Printing then begin
      SetOrientationActive;
    end; { if }
  end;  { SetOrientation }

  procedure TBaseReport.SetOrientationActive;

  var
    D1: double;

  begin { SetOrientationActive }
    if not NoPrinters then begin
      if FNewOrientation <> poDefault then begin
        LocalDevice.Orientation := FNewOrientation;
        InitPrinter;
      end; { if }
    end else begin
      if (FNewOrientation <> FOrientation) and
         ((FOrientation = poLandscape) or (FNewOrientation = poLandscape)) then begin
        D1 := NoPrinterPageWidth;
        NoPrinterPageWidth := NoPrinterpageHeight;
        NoPrinterPageHeight := D1;
      end; { if }
    end; { else }
    FOrientation := FNewOrientation;
  end;  { SetOrientationActive }

  procedure TBaseReport.SetCopies(Value: integer);

  begin { SetCopies }
    CheckNotPrinting;
    if not NoPrinters then begin
      if Value > 0 then begin
        LocalDevice.Copies := Value;
      end; { if }
    end; { if }
    FCopies := Value;
  end;  { SetCopies }

  function TBaseReport.GetDevMode: PDevMode;

  begin { GetDevMode }
    if RPDev = nil then begin
      Result := nil;
    end else begin
      Result := RPDev.DevMode;
    end; { else }
  end;  { GetDevMode }

  function TBaseReport.GetPrinterIndex: integer;

  begin { GetPrinterIndex }
    if NoPrinters or (RPDev = nil) then begin
      Result := -1;
    end else begin
      Result := RPDev.DeviceIndex;
    end; { else }
  end;  { GetPrinterIndex }

  procedure TBaseReport.SetPrinterIndex(Value: integer);

  begin { SetPrinterIndex }
    CheckNotPrinting;
    if Value >= -1 then begin
      FPrinterIndex := Value;
      if not NoPrinters and (RPDev <> nil) then begin
        RPDev.DeviceIndex := Value;
      end; { if }
      ResetPrinter;
    end; { if }
  end;  { SetPrinterIndex }

  function TBaseReport.SetPaperSize(Size: integer;
                                    Width: double;
                                    Height: double): boolean;

  begin { SetPaperSize }
    CheckNotPrinting;
    if NoPrinters then begin
      if (Width <> 0) and (Height <> 0) then begin
        FNoPrinterPageWidth := Width;
        FNoPrinterPageHeight := Height;
      end; { if }
      Result := true;
    end else begin
      Result := LocalDevice.SetPaperSize(Size,Round(XU2I(Width) * 254),
       Round(YU2I(Height) * 254));
      InitPrinter;
    end; { else }
  end;  { SetPaperSize }

  procedure TBaseReport.ReleasePrinter;

  begin { ReleasePrinter }
    if NoPrinters then Exit;
    CheckNotPrinting;

  { Release printer handle and save to TPrinter }
    if (RPDev <> nil) then begin
      RPDev.SaveToPrinter;
    end; { if }
  end;  { ReleasePrinter }

  procedure TBaseReport.RecoverPrinter;

  begin { RecoverPrinter }
    if NoPrinters then Exit;
    CheckNotPrinting;

  { Grab settings from TPrinter object }
    if RPDev <> nil then begin
      RPDev.LoadFromPrinter;
    end; { if }
  end;  { RecoverPrinter }

  procedure TBaseReport.SetOutputName(Value: string);

  begin { SetOutputName }
    if NoPrinters then Exit;
    CheckNotPrinting;

    FOutputName := Value;
    if RPDev <> nil then begin
      RPDev.OutputFile := Value;
    end; { if }
  end;  { SetOutputName }

  function TBaseReport.GetDeviceName: string;

  begin { GetDeviceName }
    if Destination = destPreview then begin
      Result := Trans('Preview');
    end else begin
      if RPDev = nil then begin
        Result := Trans('Render');
      end else begin
        Result := RPDev.Device;
      end; { else }
    end; { else }
  end;  { GetDeviceName }

  function TBaseReport.GetDriverName: string;

  begin { GetDriverName }
    if Destination = destPreview then begin
      Result := Trans('Preview');
    end else begin
      if RPDev = nil then begin
        Result := Trans('Render');
      end else begin
        Result := RPDev.Driver;
      end; { else }
    end; { else }
  end;  { GetDriverName }

  function TBaseReport.GetPort: string;

  begin { GetPort }
    if Destination = destPreview then begin
      Result := Trans('Screen');
    end else begin
      if RPDev = nil then begin
        Result := Trans('Render');
      end else begin
        Result := RPDev.Output;
      end; { else }
    end; { else }
  end;  { GetPort }

  procedure TBaseReport.SetOriginX(Value: double);

  begin { SetOriginX }
    FOriginX := XU2I(Value);
  end;  { SetOriginX }

  function TBaseReport.GetOriginX: double;

  begin { GetOriginX }
    Result := XI2U(FOriginX);
  end;  { GetOriginX }

  procedure TBaseReport.SetOriginY(Value: double);

  begin { SetOriginY }
    FOriginY := YU2I(Value);
  end;  { SetOriginY }

  function TBaseReport.GetOriginY: double;

  begin { GetOriginY }
    Result := YI2U(FOriginY);
  end;  { GetOriginY }

  procedure TBaseReport.SetScaleX(Value: double);

  begin { SetScaleX }
    FScaleX := Value;
  end;  { SetScaleX }

  procedure TBaseReport.SetScaleY(Value: double);

  begin { SetScaleY }
    FScaleY := Value;
  end;  { SetScaleY }

  procedure TBaseReport.SetTextBKMode(Value: TBKMode);

  begin { SetTextBKMode }
    FTextBKMode := Value;
  end;  { SetTextBKMode }

  procedure TBaseReport.SetBKColor(Value: TColor);

  begin { SetBKColor }
    CheckPrinting;
    //!!PORT!! RPBASE04
  end;  { SetBKColor }

  function TBaseReport.GetBKColor: TColor;

  begin { GetBKColor }
    CheckPrinting;
    //!!PORT!! RPBASE05
    Result := clBlack;
  end;  { GetBKColor }

{ Report Methods }

  procedure TBaseReport.CheckPrinting;

  begin { CheckPrinting }
    if not Printing then begin
      RaiseError(Trans('Invalid method call.  ReportPrinter is not printing.'));
    end; { if }

    if FAborted then begin
      SysUtils.Abort;
    end; { if }
  end;  { CheckPrinting }

  procedure TBaseReport.CheckNotPrinting;

  begin { CheckNotPrinting }
    if Printing then begin
      RaiseError(Trans('Invalid method call.  ReportPrinter already printing.'));
    end; { if }
  end;  { CheckNotPrinting }

  function TBaseReport.NoPrinters: boolean;

  begin { NoPrinters }
    if Assigned(FLocalDevice) and
       (RPDev <> nil) and not RPDev.InvalidPrinter then begin
      Result := false;
    end else if RPDev = nil then begin
      Result := true;
    end else begin
      Result := RPDev.InvalidPrinter;
    end; { else }
  end;  { NoPrinters }

  function TBaseReport.ShowPrintDialog: boolean;

  var
    PrintDialogRec: TPrintDialogRec;

  begin { ShowPrintDialog }
    if RPDev = nil then begin
      Result := false;
    end else begin
      FillChar(PrintDialogRec,SizeOf(PrintDialogRec),0);
      with PrintDialogRec do begin
        Collate := LocalDevice.Collate;
        Copies := LocalDevice.Copies;
        FromPage := FirstPage;
        ToPage := LastPage;
        MinPage := 1;
        MaxPage := LastPage;
        Options := [poWarning];
        PrintToFile := false;
        PrintRange := prAllPages;
      end; { with }
      Result := RPDev.PrintDialog(PrintDialogRec);
      with PrintDialogRec do begin
        LocalDevice.Collate := Collate;
        SetCopies(Copies);
        if PrintRange = prPageNums then begin
          FirstPage := FromPage;
          LastPage := ToPage;
        end else if PrintRange = prAllPages then begin
          FirstPage := 1;
          LastPage := 9999;
        end; { else }
      end; { with }
    end; { else }
  end;  { ShowPrintDialog }

  function TBaseReport.ShowPrinterSetupDialog: boolean;

  begin { ShowPrinterSetupDialog }
    if RPDev = nil then begin
      Result := false;
    end else begin
      Result := RPDev.PrinterSetupDialog;
    end; { else }
  end;  { ShowPrinterSetupDialog }

  procedure TBaseReport.Abort;

  begin { Abort }
    FAborted := true;
    FPageInvalid := true;
  end;  { Abort }

  procedure TBaseReport.AbortPage;

  begin { AbortPage }
    CheckPrinting;
    if FPageInvalid then Exit;
    FPageInvalid := true;
    Dec(FCurrentPage);
  end;  { AbortPage }

  function TBaseReport.GetOutputInvalid: boolean;

  begin { GetOutputInvalid }
    Result := FPageInvalid or not (Destination in ValidDestination);
  end;  { GetOutputInvalid }

  procedure TBaseReport.SetFrameMode(Value: TFrameMode);

  begin { SetFrameMode }
    FFrameMode := Value;
  end;  { SetFrameMode }

  procedure TBaseReport.AllowPreviewOnly;

  begin { AllowPreviewOnly }
    Exclude(ValidDestination,destPrinter);
    Include(ValidDestination,destPreview);
  end;  { AllowPreviewOnly }

  procedure TBaseReport.AllowPrinterOnly;

  begin { AllowPrinterOnly }
    Exclude(ValidDestination,destPreview);
    Include(ValidDestination,destPrinter);
  end;  { AllowPrinterOnly }

  procedure TBaseReport.AllowAll;

  begin { AllowAll }
    Include(ValidDestination,destPrinter);
    Include(ValidDestination,destPreview);
  end;  { AllowAll }

  procedure TBaseReport.Execute;

  begin { Execute }
    try
      SetOrientationActive;
      Start;

    { Call the print events }
      if Assigned(FOnPrint) then begin { Print entire report in one call }
        PrintEvent;
      end else if Assigned(FOnPrintPage) then begin { Print each page }
        while PrintPageEvent and not Aborted do begin
          NewPage;
        end; { while }
      end else begin
        PrintEvent;
      end; { else }
    finally
      Finish;
    end; { tryf }
  end;  { Execute }

  function TBaseReport.PageInRange(PageNum: integer): boolean;

  var
    Sel: TFormatString;
    First: integer;
    Last: integer;
    I1: integer;

  begin { PageInRange }
    if FSelection = '' then begin
      Result := (PageNum >= FFirstPage) and (PageNum <= FLastPage);
    end else begin
    { Handle formats like 1-3,5,7,11-,12,even,odd,all or -20 }
      I1 := 0;
      First := -1;
      Sel := '';
      Result := false;
      repeat
        Inc(I1);
        if (I1 > Length(FSelection)) or (FSelection[I1] = ',') then begin
          Sel := AnsiUpperCase(Sel);
          if Sel = '' then begin
            if First > -1 then begin
              Result := (PageNum >= First);
            end; { if }
          end else if (Sel = 'E') or (Sel = {Trans-}'EVEN') then begin { Even }
            Result := not Odd(PageNum);
          end else if (Sel = 'R') then begin { Reverse Print }
          end else if (Sel = 'O') or (Sel = {Trans-}'ODD') then begin { Odd }
            Result := Odd(PageNum);
          end else if (Sel = 'A') or (Sel = {Trans-}'ALL') then begin { All }
            Result := true;
          end else begin { Check numeric range }
            Last := StrToInt(Sel);
            if First > -1 then begin
              Result := (PageNum >= First) and (PageNum <= Last);
            end else begin
              Result := (PageNum = Last);
            end; { else }
          end; { else }
          First := -1;
          Sel := '';
        end else if FSelection[I1] = '-' then begin
          if Sel = '' then begin
            First := 1;
          end else begin
            First := StrToInt(Sel);
          end; { else }
          Sel := '';
        end else begin
          Sel := Sel + FSelection[I1];
        end; { else }
      until (I1 > Length(FSelection)) or Result;
    end; { else }
  end;  { PageInRange }

  procedure TBaseReport.NewPage;

  begin { NewPage }
    SetOrientationActive;
    CheckPrinting;

    Inc(FCurrentPage);
    UpdateStatus;
    FPageInvalid := not PageInRange(FCurrentPage) or FAborted;
    OriginX := 0.0;
    OriginY := 0.0;
    ResetTabs;
    SetBottomMargin(FBottomMarginWidth);
    SetRightMargin(FRightMarginWidth);
    ResetSection;
    Home;
  end;  { NewPage }

  procedure TBaseReport.NewPara;

  begin { NewPara }
  end;  { NewPara }

  procedure TBaseReport.UpdateStatus;

  var
    NewCaption: string;
    I1: integer;
    Ch: char;

    procedure InsertText(Text: string);

    begin { InsertText }
      Insert(Text,NewCaption,I1);
      I1 := I1 + (Length(Text) - 1);
    end;  { InsertText }

  begin { UpdateStatus }
    if Assigned(FStatusLabel) then begin
      NewCaption := FStatusFormat;
      I1 := Pos('%',NewCaption);
      while (I1 > 0) and (I1 < Length(NewCaption)) do begin
        if NewCaption[I1] = '%' then begin
          Ch := UpCase(NewCaption[I1 + 1]);
          Delete(NewCaption,I1,2);
          case Ch of
            '%': begin
              InsertText('%');
            end;
            '0'..'9': begin
              if FStatusText.Count > (Ord(Ch) - Ord('0')) then begin
                InsertText(FStatusText[Ord(Ch) - Ord('0')]);
              end; { if }
            end;
            'C': begin
              InsertText(IntToStr(CurrentPass));
            end;
            'D': begin
              InsertText(GetDeviceName);
            end;
            'F': begin
              InsertText(IntToStr(FFirstPage));
            end;
            'L': begin
              InsertText(IntToStr(FLastPage));
            end;
            'N': begin
              InsertText(#13);
            end;
            'P': begin
              InsertText(IntToStr(FCurrentPage));
            end;
            'R': begin
              InsertText(GetDriverName);
            end;
            'S': begin
              InsertText(IntToStr(TotalPasses));
            end;
            'T': begin
              InsertText(GetPort);
            end;
            else begin { Insert error }
              InsertText({Trans-}'<ERROR-%' + Ch + '>');
            end;
          end; { case }
        end; { if }
        Inc(I1);
      end; { while }
      FStatusLabel.Caption := NewCaption;
      ProcessMessages; { Allow to redraw }
    end; { if }
  end;  { UpdateStatus }

  procedure TBaseReport.Reset;

  begin { Reset }
    SetBrush(clBlack,bsClear,nil);
    SetPen(clBlack,psSolid,1,pmCopy);
    OriginX := 0.0;
    OriginY := 0.0;
    ClearColumns;
    ClearTabs;
    ResetSection;
    Home;
    TabJustify := tjNone;
  end;  { Reset }

  procedure TBaseReport.ResetSection;

  begin { ResetSection }
    FLeftSection := FLeftMargin;
    FRightSection := FRightMargin;
    FTopSection := FTopMargin;
    FBottomSection := FBottomMargin;
  end;  { ResetSection }

  procedure TBaseReport.InitPrinter;

  var
    Rect: TRect;

  begin { InitPrinter }
    FLeftWaste := 0.0;
    FRightWaste := 0.0;
    FTopWaste := 0.0;
    FBottomWaste := 0.0;
    if NoPrinters then begin
      FXDPI := LocalDevice.XDPI;
      FYDPI := LocalDevice.YDPI;
    end else begin
      case Destination of
        destPrinter,destFile,destCustomFile: begin
          if RPDev <> nil then begin
            RPDev.ResetHandle(false);
          end; { if }
          FXDPI := LocalDevice.XDPI;
          FYDPI := LocalDevice.YDPI;
          Rect := LocalDevice.Waste;
          FLeftWaste := Rect.Left / FXDPI;
          FRightWaste := Rect.Right / FXDPI;
          FTopWaste := Rect.Top / FYDPI;
          FBottomWaste := Rect.Bottom / FYDPI;
        end;
        destPreview: begin
        end;
      end; { case }
    end; { if }
  end;  { InitPrinter }

  procedure TBaseReport.ClearLineLists;

  var
    ListNode: PLineList;

  begin { ClearLineLists }
    while Assigned(HLineList) do begin
      ListNode := HLineList;
      HLineList := HLineList^.Next;
      FreeMem(ListNode,SizeOf(TLineList));
    end; { while }
    while Assigned(VLineList) do begin
      ListNode := VLineList;
      VLineList := VLineList^.Next;
      FreeMem(ListNode,SizeOf(TLineList));
    end; { while }
  end;  { ClearLineLists }

  function TBaseReport.BufferLine(X1,Y1: double): boolean;

  const
    Delta = 1.0 / 10000.0; { Resolution to 10000 DPI }

    procedure AddLineList(var LineList: PLineList;
                              X1,X2,Common: double);

    var
      PrevNode: PLineList;
      ListNode: PLineList;
      Between1: boolean;
      Between2: boolean;

      procedure DelCurrNode;

      begin { DelCurrNode }
        if Assigned(PrevNode) then begin
          PrevNode^.Next := ListNode^.Next;
          FreeMem(ListNode,SizeOf(TLineList));
          ListNode := PrevNode^.Next;
        end else begin
          LineList := ListNode^.Next;
          FreeMem(ListNode,SizeOf(TLineList));
          ListNode := LineList;
        end; { else }
      end;  { DelCurrNode }

    begin { AddLineList }
    { Process through all lines, removing any that are the same vector, }
    { same pen settings and that overlap or connect points }
      PrevNode := nil;
      ListNode := LineList;
      while Assigned(ListNode) do begin
        with ListNode^ do begin
          if (Abs(Common - C1) <= Delta) and (Canvas.Pen.Color = Color) and
           (Canvas.Pen.Mode = Mode) and (Canvas.Pen.Style = Style) and
           (Canvas.Pen.Width = Width) then begin
            Between1 := ((P1 - X1) <= Delta) and ((X1 - P2) <= Delta);
            Between2 := ((P1 - X2) <= Delta) and ((X2 - P2) <= Delta);
            if Between1 and Between2 then begin { Current absorbs test line }
              Exit;
            end else if Between1 then begin { Test line joins with current }
              X1 := P1;
              DelCurrNode;
            end else if Between2 then begin { Test line joins with current }
              X2 := P2;
              DelCurrNode;
            end else if (X1 < P1) and (X2 > P2) then begin { Test line absorbs current }
              DelCurrNode;
            end else begin { Test line is separate from current }
              PrevNode := ListNode;
              ListNode := ListNode^.Next;
            end; { else }
          end else begin
            PrevNode := ListNode;
            ListNode := ListNode^.Next;
          end; { else }
        end; { with }
      end; { while }

    { Add current line values }
      GetMem(ListNode,SizeOf(TLineList));
      with ListNode^ do begin
        P1 := X1;
        P2 := X2;
        C1 := Common;
        Color := Canvas.Pen.Color;
        Mode := Canvas.Pen.Mode;
        Style := Canvas.Pen.Style;
        Width := Canvas.Pen.Width;
        Next := LineList;
      end; { with }
      LineList := ListNode;
    end;  { AddLineList }

  begin { BufferLine }
    X1 := X1 + OriginX;
    Y1 := Y1 + OriginY;
    if NoBufferLine then begin { don't buffer }
      Result := false;
    end else if X1 = LastLineX then begin { Vertical Line }
      Result := true;
      if Y1 <= LastLineY then begin
        AddLineList(VLineList,Y1,LastLineY,X1);
      end else begin
        AddLineList(VLineList,LastLineY,Y1,X1);
      end; { else }
    end else if Y1 = LastLineY then begin { Horizontal Line }
      Result := true;
      if X1 <= LastLineX then begin
        AddLineList(HLineList,X1,LastLineX,Y1);
      end else begin
        AddLineList(HLineList,LastLineX,X1,Y1);
      end; { else }
    end else begin { Skew Line }
      Result := false;
    end; { else }
    LastLineX := X1;
    LastLineY := Y1;
  end;  { BufferLine }

  procedure TBaseReport.DrawLineLists;

  var
    SavePen: TPen;

    procedure DrawList(var LineList: PLineList;
                           Vertical: boolean);

    var
      PrevNode: PLineList;
      ListNode: PLineList;
      SaveState: boolean;

    begin { DrawList }
      SaveState := NoBufferLine;
      NoBufferLine := true;
      ListNode := LineList;
      while Assigned(ListNode) do begin
        with ListNode^ do begin
          Canvas.Pen.Color := Color;
          Canvas.Pen.Mode := Mode;
          Canvas.Pen.Style := Style;
          Canvas.Pen.Width := Width;
          if Vertical then begin
            MoveTo(C1 - OriginX,P1 - OriginY);
            LineTo(C1 - OriginX,P2 - OriginY);
          end else begin
            MoveTo(P1 - OriginX,C1 - OriginY);
            LineTo(P2 - OriginX,C1 - OriginY);
          end; { else }
        end; { with }
        PrevNode := ListNode;
        ListNode := ListNode^.Next;
        FreeMem(PrevNode,SizeOf(TLineList));
      end; { while }
      LineList := nil;
      NoBufferLine := SaveState;
    end;  { DrawList }

  begin { DrawLineLists }
    SavePen := TPen.Create;
    with SavePen do begin
      Color := Canvas.Pen.Color;
      Mode := Canvas.Pen.Mode;
      Style := Canvas.Pen.Style;
      Width := Canvas.Pen.Width;
    end; { with }
    try
      DrawList(VLineList,true);
      DrawList(HLineList,false);
    finally
      with SavePen do begin
        Canvas.Pen.Color := Color;
        Canvas.Pen.Mode := Mode;
        Canvas.Pen.Style := Style;
        Canvas.Pen.Width := Width;
        Free;
      end; { with }
    end; { tryf }
  end;  { DrawLineLists }

  procedure TBaseReport.ResetPrinter;

  begin { ResetPrinter }
    CheckNotPrinting;
    InitPrinter;

    MarginLeft := FLeftMarginWidth;
    MarginRight := FRightMarginWidth;
    MarginTop := FTopMarginWidth;
    MarginBottom := FBottomMarginWidth;
    ResetSection;
  end;  { ResetPrinter }

{ RPDev Access methods }

  function TBaseReport.SelectBin(BinName: string): boolean;

  begin { SelectBin }
  { Allow bin changes even during printing }
    Result := LocalDevice.SelectBin(BinName,false);
  end;  { SelectBin }

  function TBaseReport.SelectPaper(PaperName: string): boolean;

  begin { SelectPaper }
    CheckNotPrinting;
    Result := LocalDevice.SelectPaper(PaperName,false);
  end;  { SelectPaper }

  function TBaseReport.SelectPrinter(PrinterName: string): boolean;

  begin { SelectPrinter }
    CheckNotPrinting;
  { Try exact name match first }
    if RPDev = nil then begin
      Result := false;
    end else begin
      Result := RPDev.SelectPrinter(PrinterName,true);
      if not Result then begin { If not found, then try partial name match }
        Result := RPDev.SelectPrinter(PrinterName,false);
      end; { if }
    end; { else }
  end;  { SelectPrinter }

  function TBaseReport.SupportBin(BinNum: integer): boolean;

  begin { SupportBin }
    Result := LocalDevice.SupportBin(BinNum);
  end;  { SupportBin }

  function TBaseReport.SupportPaper(PaperNum: integer): boolean;

  begin { SupportPaper }
    Result := LocalDevice.SupportPaper(PaperNum);
  end;  { Supportpaper }

  function TBaseReport.SupportOrientation: boolean;

  begin { SupportOrientation }
    Result := LocalDevice.SupportOrientation;
  end;  { SupportOrientation }

  function TBaseReport.SupportDuplex: boolean;

  begin { SupportDuplex }
    Result := LocalDevice.SupportDuplex;
  end;  { SupportDuplex }

  function TBaseReport.SupportCollate: boolean;

  begin { SupportCollate }
    Result := LocalDevice.SupportCollate;
  end;  { SupportCollate }

  function TBaseReport.GetBins: TStrings;

  begin { GetBins }
    Result := LocalDevice.Bins;
  end;  { GetBins }

  function TBaseReport.GetPapers: TStrings;

  begin { GetPapers }
    Result := LocalDevice.Papers;
  end;  { GetPapers }

  function TBaseReport.GetFonts: TStrings;

  begin { GetFonts }
    Result := LocalDevice.Fonts;
  end;  { GetFonts }

  function TBaseReport.GetPrinters: TStrings;

  begin { GetPrinters }
    Result := LocalDevice.Printers;
  end;  { GetPrinters }

  function TBaseReport.GetMaxCopies: longint;

  begin { GetMaxCopies }
    Result := LocalDevice.MaxCopies;
  end;  { GetMaxCopies }

  function TBaseReport.GetDuplex: TDuplex;

  begin { GetDuplex }
    Result := LocalDevice.Duplex;
  end;  { GetDuplex }

  procedure TBaseReport.SetDuplex(Value: TDuplex);

  begin { SetDuplex }
    LocalDevice.Duplex := Value;
  end;  { SetDuplex }

  function TBaseReport.GetCollate: boolean;

  begin { GetCollate }
    Result := LocalDevice.Collate;
  end;  { GetCollate }

  procedure TBaseReport.SetCollate(Value: boolean);

  begin { SetCollate }
    LocalDevice.Collate := Value;
  end;  { SetCollate }

  function TBaseReport.GetRelativePage: integer;

  begin { GetRelativePage }
    Result := CurrentPage + PageNumOffset;
  end;  { GetRelativePage }

{ Positioning Methods }

  function TBaseReport.XI2D(Pos: double): longint;

  begin { XI2D }
    Result := Round((((Pos + FOriginX) * (FScaleX * FZoom / 10000.0)) -
     FLeftWaste) * XDPI) + AddX;
  end;  { XI2D }

  function TBaseReport.YI2D(Pos: double): longint;

  begin { YI2D }
    Result := Round((((Pos + FOriginY) * (FScaleY * FZoom / 10000.0)) -
     FTopWaste) * YDPI) + AddY;
  end;  { YI2D }

  function TBaseReport.XU2D(Pos: double): longint;

  begin { XU2D }
    Result := XI2D(XU2I(Pos));
  end;  { XU2D }

  function TBaseReport.YU2D(Pos: double): longint;

  begin { YU2D }
    Result := YI2D(YU2I(Pos));
  end;  { YU2D }

  function TBaseReport.XD2I(Pos: longint): double;

  begin { XD2I }
    Result := (((Pos - AddX) / FXDPI) + FLeftWaste) * (10000.0 / (FScaleX *
     FZoom)) - FOriginX;
  end;  { XD2I }

  function TBaseReport.YD2I(Pos: longint): double;

  begin { YD2I }
    Result := (((Pos - AddY) / FYDPI) + FTopWaste) * (10000.0 / (FScaleY *
     FZoom)) - FOriginY;
  end;  { YD2I }

  function TBaseReport.XD2U(Pos: longint): double;

  begin { XD2U }
    Result := XI2U(XD2I(Pos));
  end;  { XD2U }

  function TBaseReport.YD2U(Pos: longint): double;

  begin { YD2U }
    Result := YI2U(YD2I(Pos));
  end;  { YD2U }

  function TBaseReport.XI2U(Pos: double): double;

  begin { XI2U }
    Result := Pos * FUnitsFactor;
  end;  { XI2U }

  function TBaseReport.YI2U(Pos: double): double;

  begin { YI2U }
    Result := Pos * FUnitsFactor;
  end;  { YI2U }

  function TBaseReport.XU2I(Pos: double): double;

  begin { XU2I }
    Result := Pos / FUnitsFactor;
  end;  { XU2I }

  function TBaseReport.YU2I(Pos: double): double;

  begin { YU2I }
    Result := Pos / FUnitsFactor;
  end;  { YU2I }

  procedure TBaseReport.Cr;

  begin { Cr }
    FCursorXPos := LineStartPos;
  end;  { Cr }

  procedure TBaseReport.Lf;

  begin { Lf }
    ResetLineHeight;
    FLineNum := FLineNum + 1;
    FCursorYPos := FCursorYPos + FLineHeight;
  end;  { Lf }

  procedure TBaseReport.ResetLineHeight;

  begin { ResetLineHeight }
    if Printing then begin
      UpdateFont;
      FAscentHeight := BaseFont.Ascent;
      FDescentHeight := BaseFont.Descent;
      case LineHeightMethod of
        lhmLinesPerInch: begin
          FLineHeight := 1.0 / FLinesPerInch;
        end;
        lhmFont: begin
          FLineHeight := BaseFont.Size / 60.0;
        end;
        lhmUser: begin { Don't do anything }
        end;
      end; { case }
    end; { if }
  end;  { ResetLineHeight }

  procedure TBaseReport.AdjustLine;

  begin { AdjustLine }
    if Printing then begin
      LineTop := LineTop;
    end; { if }
  end;  { AdjustLine }

  procedure TBaseReport.CrLf;

  begin { CrLf }
    NewLine;
  end;  { CrLf }

  procedure TBaseReport.NewLine;

  begin { NewLine }
    SoftLine;
    NewPara;
  end;  { NewLine }

  procedure TBaseReport.SoftLine;

  begin { SoftLine }
    if (ColumnNum < Columns) and (LinesLeft <= 1) then begin
      LineNum := 1;
      ColumnNum := ColumnNum + 1;

    { Call the OnNewColumn event }
      NewColumnEvent;
    end else begin
      Cr;
      Lf;
    end; { else }
    ResetTabs;

  { See if we have gone past the current section }
    if LineBottom >= SectionBottom then begin
      EndOfSectionEvent;
    end; { if }
  end;  { SoftLine }

  procedure TBaseReport.NewColumn;

  begin { NewColumn }
    if (ColumnNum < Columns) then begin
      LineNum := 1;
      ColumnNum := ColumnNum + 1;
      NewColumnEvent;
    end else begin
      NewPage;
    end; { else }
  end;  { NewColumn }

  procedure TBaseReport.GotoHeader;

  begin { GotoHeader }
    ResetLineHeight;
    FCursorXPos := LineStartPos;
    FCursorYPos := FTopSection + (FLineHeight - FDescentHeight);
  end;  { GotoHeader }

  procedure TBaseReport.GotoFooter;

  begin { GotoFooter }
    ResetLineHeight;
    FCursorXPos := LineStartPos;
    FCursorYPos := FBottomSection - FDescentHeight;
  end;  { GotoFooter }

  procedure TBaseReport.GotoXY(NewXPos: double;
                               NewYPos: double);

  begin { GotoXY }
    XPos := NewXPos;
    YPos := NewYPos;
  end;  { GotoXY }

  procedure TBaseReport.Home;

  begin { Home }
    ColumnNum := 1;
    LineNum := 1;
  end;  { Home }

  procedure TBaseReport.SetTopOfPage;

  begin { SetTopOfPage }
    FTopSection := YU2I(FontBottom); { Set to bottom of current text }
    LineNum := 0;
  end;  { SetTopOfPage }

  function TBaseReport.LinesLeft: integer;

  begin { LinesLeft }
    if YU2I(FontBottom) > FBottomSection then begin
      Result := 0;
    end else begin
      Result := Trunc((FBottomSection - YU2I(FontBottom)) / FLineHeight) + 1;
    end; { else }
  end;  { LinesLeft }

  function TBaseReport.ColumnLinesLeft: integer;

  begin { ColumnLinesLeft }
    if (YU2I(FontBottom) > FBottomSection) and (ColumnNum >= Columns) then begin
      Result := 0;
    end else begin
      Result := LinesLeft;
      if ColumnNum < Columns then begin
        Result := Result + (Trunc((FBottomSection - FTopSection) /
         FLineHeight)) * (Columns - ColumnNum);
      end; { else }
    end; { else }
  end;  { ColumnLinesLeft }

{ Printing Methods }

  procedure TBaseReport.Print(Text: string);

  var
    FoundPos: integer;
    Piece: string;

  begin { Print }
    Piece := Text;
    FoundPos := Pos(#9,Piece); { Look for a tab }
    while FoundPos > 0 do begin { Process substrings }
      if FoundPos > 1 then begin { There is something to print }
        PrintPiece(Copy(Piece,1,FoundPos - 1));
      end; { if }
      Delete(Piece,1,FoundPos);
      Tab(NA,NA,NA,NA,NA);
      FoundPos := Pos(#9,Piece); { Look for another tab }
    end; { while }
    PrintPiece(Piece); { Print remaining piece }
  end;  { Print }

  procedure TBaseReport.PrintCenter(Text: string;
                                    Pos: double);

  begin { PrintCenter }
    PrintJustify(Text,XU2I(Pos),pjCenter,0,0);
  end;  { PrintCenter }

  procedure TBaseReport.PrintHeader(Text: string;
                                    Justify: TPrintJustify);

  begin { PrintHeader }
    GotoHeader;
    PrintJustify(Text,FLeftSection,Justify,0.0,FRightSection - FLeftSection);
  end;  { PrintHeader }

  procedure TBaseReport.PrintFooter(Text: string;
                                    Justify: TPrintJustify);

  begin { PrintFooter }
    GotoFooter;
    PrintJustify(Text,FLeftSection,Justify,0.0,FRightSection - FLeftSection);
  end;  { PrintFooter }

  procedure TBaseReport.PrintJustify(Text: string;
                                     Pos: double;
                                     Justify: TPrintJustify;
                                     Margin: double;
                                     Width: double);

  var
    FoundPos: integer;
    Piece: string;
    X1: double;

  begin { PrintJustify }
    case Justify of
      pjCenter: begin
        PrintCenterInch(Text,Pos + (Width / 2.0));
      end;
      pjLeft: begin
        BlockTextOp := true;
        Piece := Text;
        X1 := Pos + Margin;
        FoundPos := System.Pos(#9,Piece); { Look for a tab }
        while FoundPos > 0 do begin { Process substrings }
          if FoundPos > 1 then begin { There is something to print }
            PrintLeftInch(Copy(Piece,1,FoundPos - 1),X1);
            X1 := FCursorXPos;
          end; { if }
          X1 := Pos + (Trunc((X1 - Pos + 0.001) / XU2I(FixedTabWidth)) + 1) *
           XU2I(FixedTabWidth);
          Delete(Piece,1,FoundPos);
          FoundPos := System.Pos(#9,Piece); { Look for another tab }
        end; { while }
        BlockTextOp := false;
        PrintLeftInch(Piece,X1);
      end;
      pjRight: begin
        PrintRightInch(Text,Pos + Width - Margin);
      end;
      pjBlock: begin
        PrintBlockInch(Text,Pos + Margin,Width - (Margin * 2.0));
      end;
    end; { case }
  end;  { PrintJustify }

  procedure TBaseReport.PrintBlock(Text: string;
                                   Pos: double;
                                   Width: double);

  begin { PrintBlock }
    PrintJustify(Text,XU2I(Pos),pjBlock,0,XU2I(Width));
  end;  { PrintBlock }

  procedure TBaseReport.PrintLeft(Text: string;
                                  Pos: double);

  begin { PrintLeft }
    PrintJustify(Text,XU2I(Pos),pjLeft,0,0);
  end;  { PrintLeft }

  procedure TBaseReport.Println(Text: string);

  begin { Println }
    Print(Text);
    NewLine;
  end;  { Println }

  procedure TBaseReport.PrintPiece(Text: string);

  var
    ThisJustify: TPrintJustify;

  begin { PrintPiece }
    if not OnTab or not Assigned(TabHead) then begin { Print from current pos }
      PrintJustify(Text,FCursorXPos,pjLeft,0,0);
    end else begin
      with CurrentTab^ do begin
        ThisJustify := Justify;
        if TabJustify <> tjNone then begin
          ThisJustify := TPrintJustify(TabJustify);
        end; { if }
        PrintJustify(Text,Pos + FColumnWidth * (ColumnNum - 1),ThisJustify,
         Margin,Width);
        OnTab := false;
      end; { with }
    end; { else }
  end;  { PrintPiece }

  procedure TBaseReport.PrintRight(Text: string;
                                   Pos: double);

  begin { PrintRight }
    PrintJustify(Text,XU2I(Pos),pjRight,0,0);
  end;  { PrintRight }

  procedure TBaseReport.PrintXY(X,Y: double;
                                Text: string);

  begin { PrintXY }
    YPos := Y;
    PrintJustify(Text,XU2I(X),pjLeft,0,0);
  end;  { PrintXY }

  procedure TBaseReport.PrintCharJustify(Text: string;
                                         Ch: char;
                                         Pos: double);

  var
    I1: integer;

  begin { PrintCharJustify }
    I1 := System.Pos(Ch,Text);
    if I1 = 0 then begin
      I1 := Length(Text) + 1;
    end; { if }
    PrintJustify(Copy(Text,1,I1 - 1),XU2I(Pos),pjRight,0,0);
    if I1 <= Length(Text) then begin
      PrintJustify(Copy(Text,I1,Length(Text) - I1 + 1),XU2I(Pos),pjLeft,0,0);
    end; { if }
  end;  { PrintCharJustify }

  function TBaseReport.TruncateText(Value: string;
                                    Width: double): string;

  begin { TruncateText }
    Result := Value;
    if Pos(#253 + {Trans-}'$RPC',Value) > 0 then Exit; { Don't truncate macros or PIVars }
    while (Result <> '') and (TextWidth(Result) > Width) do begin
      while (Result <> '') and (Result[Length(Result)] = RPTFSuffix) do begin
        repeat
          SetLength(Result,Length(Result) - 1);
          if Result = '' then Exit;
        until Result[Length(Result)] = RPTFPrefix;
        if ByteType(Result,Length(Result)) = mbTrailByte then begin
          SetLength(Result,Length(Result) - 2); { Delete RPTFPrefix }
        end else begin
          SetLength(Result,Length(Result) - 1); { Delete RPTFPrefix }
        end; { else }
      end; { while }
      if ByteType(Result,Length(Result)) = mbTrailByte then begin
        SetLength(Result,Length(Result) - 2);
      end else begin
        SetLength(Result,Length(Result) - 1);
      end; { else }
    end; { while }
  end;  { TruncateText }

  procedure TBaseReport.PrintTab(Text: string);

  begin { PrintTab }
    Tab(NA,NA,NA,NA,NA);
    Print(TruncateText(Text,TabWidth(0)));
  end;  { PrintTab }

  function TBaseReport.ConvertMacro(MacroID: TMacroID): string;

  var
    I1: integer;

  begin { ConvertMacro }
    case MacroID of
      midCurrDateShort: begin
        Result := FormatDateTime({Trans-}'ddddd',ReportDateTime);
      end;
      midCurrDateLong: begin
        Result := FormatDateTime({Trans-}'dddddd',ReportDateTime);
      end;
      midCurrDateUS: begin
        Result := FormatDateTime({Trans-}'mm/dd/yy',ReportDateTime);
      end;
      midCurrDateInter: begin
        Result := FormatDateTime({Trans-}'dd/mm/yy',ReportDateTime);
      end;
      midCurrTimeShort: begin
        Result := FormatDateTime({Trans-}'t',ReportDateTime);
      end;
      midCurrTimeLong: begin
        Result := FormatDateTime({Trans-}'tt',ReportDateTime);
      end;
      midCurrTimeAMPM: begin
        Result := FormatDateTime({Trans-}'hh:nn ampm',ReportDateTime);
      end;
      midCurrTime24: begin
        Result := FormatDateTime({Trans-}'hh:nn',ReportDateTime);
      end;
      midFirstPage: begin
        Result := IntToStr(FFirstPage);
      end;
      midLastPage: begin
        Result := IntToStr(FLastPage);
      end;
      midTotalPages: begin
        Result := IntToStr(JobPages);
      end;
      midCurrentPage: begin
        Result := IntToStr(FCurrentPage);
      end;
      midRelativePage: begin
        Result := IntToStr(RelativePage);
      end;
      midPrinterName: begin
        Result := DeviceName;
      end;
      midDriverName: begin
        Result := DriverName;
      end;
      midPortName: begin
        Result := Port;
      end;
      midUser01..midUser20: begin
        I1 := Ord(MacroID) - Ord(midUser01);
        if I1 < MacroData.Count then begin
          Result := MacroData[I1];
        end else begin
          Result := '';
        end; { else }
      end;
      else begin
        Result := '';
      end;
    end; { case }
  end;  { ConvertMacro }

  procedure TBaseReport.ProcessString(var Text: string);

  var
    I1: integer;
    I2: integer;
    I3: integer;
    MacroStr: string;
    MacroID: TMacroID;
    VarName: string;

  begin { ProcessString }
  { Look for any macro strings in Text and replace }
    repeat
      I1 := Pos({Trans-}#253'$RPCM',Text);
      if I1 > 0 then begin
        MacroStr := Copy(Text,I1,8);
        MacroID := TMacroID(StrToInt(Copy(MacroStr,7,2)));
        Delete(Text,I1,8);
        Insert(ConvertMacro(MacroID),Text,I1);
      end; { if }
      I2 := Pos({Trans-}#253'$RPCV',Text);
      if I2 > 0 then begin
        I3 := I2 + 6;
        VarName := '';
        if (I3 < Length(Text)) and (Text[I3] = #252) then begin
          Inc(I3);
          while I3 <= Length(Text) do begin
            if Text[I3] = #251 then begin
              Break;
            end else begin
              VarName := VarName + Text[I3];
            end; { else }
            Inc(I3);
          end; { while }
        end; { if }
        if I3 <= Length(Text) then begin
          Delete(Text,I2,I3 - I2 + 1);
        end else begin
          Delete(Text,I2,Length(Text) - I2 + 1);
        end; { else }
        Insert(GetPIVar(VarName),Text,I2);
      end; { if }
    until (I1 = 0) and (I2 = 0);
  end;  { ProcessString }

  function TBaseReport.Macro(MacroID: TMacroID): string;

  begin { Macro }
  { Always return plain-text value for midRelativePage }
    if (Destination = destFile) and (MacroID <> midRelativePage) then begin
      Result := {Trans-}#253'$RPCM' + Format({Trans-}'%.2d',[Ord(MacroID)]);
    end else begin
      Result := ConvertMacro(MacroID);
    end; { else }
  end;  { Macro }

  function TBaseReport.PIVar(VarName: string): string;

  begin { PIVar }
    Result := '';
  end;  { PIVar }

  procedure TBaseReport.SetPIVar(VarName: string;
                                 VarValue: string);

  begin { SetPIVar }
  end;  { SetPIVar }

  function TBaseReport.GetPIVar(VarName: string): string;

  begin { GetPIVar }
    Result := VarName;
  end;  { GetPIVar }

{ Positioning Methods }

  function TBaseReport.PopPos: boolean;

  var
    TempPos: PPositionNode;

  begin { PopPos }
    if Assigned(PosStack) then begin
      FCursorXPos := PosStack^.X;
      FCursorYPos := PosStack^.Y;
      TempPos := PosStack^.Next;
      FreeMem(PosStack,SizeOf(TPositionNode));
      PosStack := TempPos;
      Result := true;
    end else begin
      Result := false;
    end; { else }
  end;  { PopPos }

  function TBaseReport.PushPos: boolean;

  var
    TempPos: PPositionNode;

  begin { PushPos }
    GetMem(TempPos,SizeOf(TPositionNode));
    TempPos^.X := FCursorXPos;
    TempPos^.Y := FCursorYPos;
    TempPos^.Next := PosStack;
    PosStack := TempPos;
    Result := true;
  end;  { PushPos }

  function TBaseReport.RestorePos(Index: integer): boolean;

  begin { RestorePos }
    if (Index >= 1) and (Index <= MAXSAVEPOS) then begin
      FCursorXPos := SavePosArr[Index].X;
      FCursorYPos := SavePosArr[Index].Y;
      Result := true;
    end else begin
      Result := false;
    end; { else }
  end;  { RestorePos }

  function TBaseReport.SavePos(Index: integer): boolean;

  begin { SavePos }
    if (Index >= 1) and (Index <= MAXSAVEPOS) then begin
      SavePosArr[Index].X := FCursorXPos;
      SavePosArr[Index].Y := FCursorYPos;
      Result := true;
    end else begin
      Result := false;
    end; { else }
  end;  { SavePos }

  function TBaseReport.PopTabs: boolean;

  var
    TempTabs: PTabNode;

  begin { PopTabs }
    if Assigned(TabsStack) then begin
      ReleaseTabs(TabHead);
      TabHead := TabsStack^.Head;
      TabTail := TabsStack^.Tail;
      ResetTabs;
      TempTabs := TabsStack^.Next;
      FreeMem(TabsStack,SizeOf(TTabNode));
      TabsStack := TempTabs;
      Result := true;
    end else begin
      Result := false;
    end; { else }
  end;  { PopTabs }

  function TBaseReport.PushTabs: boolean;

  var
    TempTabs: PTabNode;

  begin { PushTabs }
    if TabHead = nil then begin
      Result := false;
      Exit;
    end; { if }
    GetMem(TempTabs,SizeOf(TTabNode));
    TabHead^.RefCount := TabHead^.RefCount + 1;
    TempTabs^.Head := TabHead;
    TempTabs^.Tail := TabTail;
    TempTabs^.Next := TabsStack;
    TabsStack := TempTabs;
    Result := true;
  end;  { PushTabs }

  function TBaseReport.RestoreTabs(Index: integer): boolean;

  begin { RestoreTabs }
    if (Index >= 1) and (Index <= MAXSAVETABS) then begin
      ReleaseTabs(TabHead);
      TabHead := SaveTabsArr[Index].Head;
      TabTail := SaveTabsArr[Index].Tail;
      if Assigned(TabHead) then begin
        TabHead^.RefCount := TabHead^.RefCount + 1;
      end; { if }
      ResetTabs;
      Result := true;
    end else begin
      Result := false;
    end; { else }
  end;  { RestoreTabs }

  function TBaseReport.SaveTabs(Index: integer): boolean;

  begin { SaveTabs }
    if TabHead = nil then begin
      Result := false;
      Exit;
    end; { if }
    if (Index >= 1) and (Index <= MAXSAVETABS) then begin
      ReleaseTabs(SaveTabsArr[Index].Head);
      SaveTabsArr[Index].Head := TabHead;
      SaveTabsArr[Index].Tail := TabTail;
      TabHead^.RefCount := TabHead^.RefCount + 1;
      Result := true;
    end else begin
      Result := false;
    end; { else }
  end;  { SaveTabs }

  procedure TBaseReport.AssignFont(Font: TFont);

  begin { AssignFont }
    with Font do begin
      DisableFontChange := true;
      FontName := Name;
      FontColor := Color;
      FontPitch := Pitch;
      FontCharSet := Ord(CharSet);
      FontSize := Size;
      self.Bold := fsBold in Style;
      self.Underline := fsUnderline in Style;
      self.Italic := fsItalic in Style;
      self.Strikeout := fsStrikeout in Style;
      DisableFontChange := false;
      Subscript := false;
      Superscript := false;
      FontRotation := 0;
    end; { with }
    BaseFontChanged := true;
    FontOnChange(self);
  end;  { AssignFont }

  function TBaseReport.GetBaseFont: TSaveFont;

  begin { GetBaseFont }
    Result := BaseFont;
  end;  { GetBaseFont }

  procedure TBaseReport.SetBaseFont(Font: TSaveFont);

  begin { SetBaseFont }
    with Font do begin
      DisableFontChange := true;
      FontName := Name;
      FontColor := Color;
      FontPitch := Pitch;
      FontCharset := Charset;
      FontHeight := YI2U(Height);
      self.Bold := Bold;
      self.Underline := Underline;
      self.Italic := Italic;
      self.Strikeout := Strikeout;
      DisableFontChange := false;
    {$IFNDEF APOLLO}
      self.Subscript := Subscript;
      self.Superscript := Superscript;
      FontRotation := Rotation;
      FontAlign := Align;
    {$ENDIF}
    end; { with }
    BaseFontChanged := true;
    FontOnChange(self);
  end;  { SetBaseFont }

  function TBaseReport.PopFont: boolean;

  var
    TempFont: PFontNode;

  begin { PopFont }
    Result := true;
    if BlockTextOp then Exit;
    if Assigned(FontStack) then begin
      SetBaseFont(FontStack^.Font);
      TempFont := FontStack^.Next;
      FreeMem(FontStack,SizeOf(TFontNode));
      FontStack := TempFont;
    end else begin
      Result := false;
    end; { else }
  end;  { PopFont }

  function TBaseReport.PushFont: boolean;

  var
    TempFont: PFontNode;

  begin { PushFont }
    Result := true;
    if BlockTextOp then Exit;
    GetMem(TempFont,SizeOf(TFontNode));
    TempFont^.Font := BaseFont;
    TempFont^.Next := FontStack;
    FontStack := TempFont;
  end;  { PushFont }

  function TBaseReport.RestoreFont(Index: integer): boolean;

  begin { RestoreFont }
    if (Index >= 1) and (Index <= MAXSAVEFONT) and
     (SaveFontArr[Index].Name <> '') then begin
      SetBaseFont(SaveFontArr[Index]);
      Result := true;
    end else begin
      Result := false;
    end; { else }
  end;  { RestoreFont }

  function TBaseReport.SaveFont(Index: integer): boolean;

  begin { SaveFont }
    if (Index >= 1) and (Index <= MAXSAVEFONT) then begin
      SaveFontArr[Index] := BaseFont;
      Result := true;
    end else begin
      Result := false;
    end; { else }
  end;  { SaveFont }

{ Canvas Drawing Methods }

  procedure TBaseReport.SetBrush(NewColor: TColor;
                                 NewStyle: TBrushStyle;
                                 NewBitmap: TBitmap);

  begin { SetBrush }
    with Canvas.Brush do begin
      Color := NewColor;
      Style := NewStyle;
      if Assigned(NewBitMap) then begin
        Bitmap := NewBitmap;
      end; { if }
    end; { with }
  end;  { SetBrush }

  function TBaseReport.CreateBrush(NewColor: TColor;
                                   NewStyle: TBrushStyle;
                                   NewBitmap: TBitmap): TBrush;

  begin { CreateBrush }
    Result := TBrush.Create;
    Result.Color := NewColor;
    Result.Style := NewStyle;
    if Assigned(NewBitmap) then begin
      Result.Bitmap := NewBitmap;
    end; { if }
  end;  { CreateBrush }

  procedure TBaseReport.SetPen(NewColor: TColor;
                               NewStyle: TPenStyle;
                               NewWidth: integer;
                               NewMode: TPenMode);

  begin { SetPen }
    with Canvas.Pen do begin
      Color := NewColor;
      Mode := NewMode;
      if NewWidth < 0 then begin
        if NewWidth < -10000 then begin { Width is in 1/10000ths }
          Width := Round((Abs(NewWidth + 10000) / 10000.0) * XDPI * FScaleX *
           FZoom / 10000.0);
        end else begin { Width is in 1/100ths }
          Width := Round((Abs(NewWidth) / 100.0) * XDPI * FScaleX * FZoom / 10000.0);
        end; { else }
        if Width = 0 then begin
          Width := 1;
        end; { if }
      end else begin { Use width }
        Width := NewWidth;
      end; { else }
      Style := NewStyle;
    end; { with }
  end;  { SetPen }

  function TBaseReport.CreatePen(NewColor: TColor;
                                 NewStyle: TPenStyle;
                                 NewWidth: integer;
                                 NewMode: TPenMode): TPen;

  begin { CreatePen }
    Result := TPen.Create;
    Result.Color := NewColor;
    Result.Style := NewStyle;
    Result.Mode := NewMode;
    if NewWidth < 0 then begin { Width is in 1/100ths }
      Result.Width := Round(Abs(NewWidth) * XDPI * FScaleX * FZoom / 1000000.0);
      if Result.Width = 0 then begin
        Result.Width := 1;
      end; { if }
    end else begin { Use width }
      Result.Width := NewWidth;
    end; { else }
  end;  { CreatePen }

{ Column Methods }

  procedure TBaseReport.SetColumns(NewColumns: integer;
                                   Between: double);

  begin { SetColumns }
    if NewColumns < 1 then begin
      NewColumns := 1;
    end; { if }
    FColumnWidth := ((FRightSection - FLeftSection) + XU2I(Between)) /
     NewColumns;
    FColumnBetween := XU2I(Between);
    FColumns := NewColumns;
    ColumnNum := 1;

  { Call the OnNewColumn event }
    NewColumnEvent;
  end;  { SetColumns }

  procedure TBaseReport.SetColumnWidth(Width: double;
                                       Between: double);

  begin { SetColumnWidth }
    FColumnWidth := XU2I(Width + Between);
    if FColumnWidth <= 0.0 then begin
      FColumnWidth := FRightSection - FLeftSection;
    end; { if }
    FColumnBetween := XU2I(Between);
    FColumns := Trunc((FRightSection - FLeftSection) / FColumnWidth);
    ColumnNum := 1;

  { Call the OnNewColumn event }
    NewColumnEvent;
  end;  { SetColumnWidth }

  procedure TBaseReport.ClearColumns;

  begin { ClearColumns }
    FColumnWidth := FRightSection - FLeftSection;
    FColumnBetween := 0.0;
    FColumns := 1;
    ColumnNum := 1;
  end;  { ClearColumns }

  function TBaseReport.GetColumnStart: double;

  begin { GetColumnStart }
    Result := XI2U(LineStartPos);
  end;  { GetColumnStart }

  function TBaseReport.GetColumnEnd: double;

  begin { GetColumnEnd }
    Result := XI2U(LineStartPos + FColumnWidth - FColumnBetween);
  end;  { GetColumnEnd }

  function TBaseReport.GetColumnWidth: double;

  begin { GetColumnWidth }
    Result := XI2U(FColumnWidth - FColumnBetween);
  end;  { GetColumnWidth }

{ Tab Methods }

  procedure TBaseReport.SetTab(NewPos: double;
                               NewJustify: TPrintJustify;
                               NewWidth: double;
                               NewMargin: double;
                               NewLines: byte;
                               NewShade: byte);

  var
    Tab: PTab;

  begin { SetTab }
    GetMem(Tab,SizeOf(TTab));
    with Tab^ do begin
      Prev := TabTail;
      Next := nil;
      if Assigned(TabHead) then begin
        TabTail^.Next := Tab;
        TabTail := Tab;
      end else begin { No tabs in list yet }
        TabHead := Tab;
        TabHead^.RefCount := 1;
        TabTail := Tab;
        BeforeTabs := true;
      end; { else }

      if NewPos < 0 then begin { Append from previous tab, if any }
        if Assigned(Prev) then begin
          Pos := Prev^.Pos + Prev^.Width;
        end else begin
          Pos := FLeftMargin;
        end; { else }
      end else begin
        Pos := XU2I(NewPos);
      end; { else }

      Justify := NewJustify;
      Width := 0.0;
      Margin := 0.0;
      Left := false;
      Right := false;
      Top := false;
      Bottom := false;
      Shade := 0;
      if NewWidth > 0 then begin
        Width := XU2I(NewWidth);
        Margin := NewMargin / 100.0;
        Left := (NewLines and BOXLINELEFT) > 0;
        Right := (NewLines and BOXLINERIGHT) > 0;
        Top := (NewLines and BOXLINETOP) > 0;
        Bottom := (NewLines and BOXLINEBOTTOM) > 0;
        Shade := NewShade;
      end; { if }
    end; { with }
  end;  { SetTab }

  procedure TBaseReport.ReleaseTabs(var TabPtr: PTab);

  var
    TempTab: PTab;

  begin { ReleaseTabs }
    if not Assigned(TabPtr) then Exit;

    Dec(TabPtr^.RefCount);
    if TabPtr^.RefCount = 0 then begin { Delete }
      while Assigned(TabPtr) do begin
        TempTab := TabPtr^.Next;
        FreeMem(TabPtr,SizeOf(TTab));
        TabPtr := TempTab;
      end; { while }
    end; { if }
  end;  { ReleaseTabs }

  procedure TBaseReport.ClearTabs;

  begin { ClearTabs }
    ReleaseTabs(TabHead);
    TabHead := nil;
    TabTail := nil;
    ResetTabs;
  end;  { ClearTabs }

  procedure TBaseReport.ClearAllTabs;

  var
    I1: integer;

  begin { ClearAllTabs }
    ClearTabs;
    for I1 := 1 to MAXSAVETABS do begin
      ReleaseTabs(SaveTabsArr[I1].Head);
      SaveTabsArr[I1].Tail := nil;
    end; { for }
  end;  { ClearAllTabs }

  procedure TBaseReport.ResetTabs;

  begin { ResetTabs }
    CurrentTab := nil;
    OnTab := false;
    BeforeTabs := Assigned(TabHead);
  end;  { ResetTabs }

  procedure TBaseReport.DrawTabLine(DrawLine: boolean;
                                    LineWidth: integer;
                                    X1,Y1: double;
                                    XAdd,YAdd: double;
                                    SavePen: TPen);

  var
    NewPen: TPen;

  begin { DrawTabLine }
    if (DrawLine and (LineWidth = NA)) or ((LineWidth <> 0) and
     (LineWidth <> NA)) then begin
      if LineWidth <> NA then begin { Create pen acc. to LineWidth }
        NewPen := CreatePen(clBlack,psSolid,LineWidth,pmCopy);
        try
          Canvas.Pen.Assign(NewPen);
        finally
          NewPen.Free;
        end; { try }
      end else begin
        Canvas.Pen.Assign(SavePen);
      end; { else }

      MoveTo(XI2U(X1),YI2U(Y1));
      LineTo(XI2U(X1 + XAdd),YI2U(Y1 + YAdd));
    end; { if }
  end;  { DrawTabLine }

  procedure TBaseReport.Tab(LeftWidth: integer;
                            RightWidth: integer;
                            TopWidth: integer;
                            BottomWidth: integer;
                            ShadeOverride: integer);

  var
    ColumnAdd: double;
    ShadePercent: byte;
    SavePen: TPen;
    SaveBrush: TBrush;
    NewBrush: TBrush;
    NewPen: TPen;
  begin { Tab }
    CheckPrinting;
    if OutputInvalid then Exit;

    if Assigned(CurrentTab) then begin { Get next tab }
      CurrentTab := CurrentTab^.Next;
    end else if Assigned(TabHead) and BeforeTabs then begin { Get first tab }
      CurrentTab := TabHead;
      BeforeTabs := false;
    end; { else }

    if not Assigned(CurrentTab) then begin
      OnTab := false;
    end else begin
      with CurrentTab^ do begin
        ColumnAdd := FColumnWidth * (ColumnNum - 1);
        OnTab := true;
        if Width > 0 then begin { Draw Tab Box }
        { See if we need to shade the tab box }
          ShadePercent := 0;
          if ShadeOverride <> NA then begin
            ShadePercent := ShadeOverride;
          end else if Shade > 0 then begin
            ShadePercent := Shade;
          end else if TabShade > 0 then begin
            ShadePercent := TabShade;
          end; { else }

          if ShadePercent > 0 then begin
          { Save off current brush and pen }
            SaveBrush := TBrush.Create;
            SaveBrush.Assign(Canvas.Brush);
            SavePen := TPen.Create;
            SavePen.Assign(Canvas.Pen);

          { Create new brush and pen }
            NewBrush := TBrush.Create;
            NewBrush.Color := ShadeToColor(TabColor,ShadePercent);
            NewBrush.Style := bsSolid;
            NewPen := CreatePen(BoxLineColor,psClear,1,pmMask);

          { Assign new brush and pen }
            Canvas.Brush.Assign(NewBrush);
            NewBrush.Free;
            Canvas.Pen.Assign(NewPen);
            NewPen.Free;

          { Draw rectangle }
            //!!PORT!! RPBASE05

            TabRectangle(XI2U(Pos + ColumnAdd),LineTop,
             XI2U(Pos + ColumnAdd + Width),LineBottom);
            //!!PORT!! RPBASE04

          { Reset Brush and Pen }
            Canvas.Brush.Assign(SaveBrush);
            SaveBrush.Free;
            Canvas.Pen.Assign(SavePen);
            SavePen.Free;
          end; { if }

        { Save current pen }
          SavePen := TPen.Create;
          try
            SavePen.Assign(Canvas.Pen);

            DrawTabLine(Left,LeftWidth,Pos + ColumnAdd,YU2I(LineBottom),
             0,-FLineHeight,SavePen);
            DrawTabLine(Right,RightWidth,Pos + ColumnAdd + Width,YU2I(LineTop),
             0,FLineHeight,SavePen);
            DrawTabLine(Top,TopWidth,Pos + ColumnAdd,YU2I(LineTop),
             Width,0,SavePen);
            DrawTabLine(Bottom,BottomWidth,Pos + ColumnAdd + Width,YU2I(LineBottom),
             -Width,0,SavePen);

          { Restore previous pen }
            Canvas.Pen.Assign(SavePen);
          finally
            SavePen.Free;
          end; { try }
        end; { if }
      end; { with }
    end; { if }
  end;  { Tab }

  function TBaseReport.GetTab(Index: integer): PTab;

  var
    I1: integer;

  begin { GetTab }
    if Index = 0 then begin
      Result := CurrentTab;
    end else begin
      Result := TabHead;
      I1 := 1;
      while (I1 < Index) and Assigned(Result) do begin
        Inc(I1);
        Result := Result^.Next;
      end; { while }
    end; { if }
  end;  { GetTab }

  function TBaseReport.TabStart(Index: integer): double;

  var
    Tab: PTab;

  begin { TabStart }
    Tab := GetTab(Index);
    if Assigned(Tab) then begin
      Result := XI2U(Tab^.Pos + Tab^.Margin);
    end else begin
      Result := 0.0;
    end; { else }
  end;  { TabStart }

  function TBaseReport.TabEnd(Index: integer): double;

  var
    Tab: PTab;

  begin { TabEnd }
    Tab := GetTab(Index);
    if Assigned(Tab) then begin
      Result := XI2U(Tab^.Pos + Tab^.Width - Tab^.Margin);
    end else begin
      Result := 0.0;
    end; { else }
  end;  { TabEnd }

  function TBaseReport.TabWidth(Index: integer): double;

  var
    Tab: PTab;

  begin { TabWidth }
    Tab := GetTab(Index);
    if Assigned(Tab) then begin
      Result := XI2U(Tab^.Width - (Tab^.Margin * 2.0));
    end else begin
      Result := 0.0;
    end; { else }
  end;  { TabWidth }

  procedure TBaseReport.FinishTabBox(Width: integer);

  begin { FinishTabBox }
    CheckPrinting;
    if OutputInvalid then Exit;

    ResetTabs;
    OnTab := true;
    while OnTab do begin
      Tab(0,0,Width,0,0);
    end; { while }
    ResetTabs;
  end;  { FinishTabBox }

{ Misc Methods }

  function TBaseReport.CreatePoint(X,Y: double): TPoint;

  begin { CreatePoint }
    Result.X := XU2D(X);
    Result.Y := YU2D(Y);
  end;  { CreatePoint }

  function TBaseReport.CreateRect(X1,Y1,X2,Y2: double): TRect;

  begin { CreateRect }
    Result.Left := XU2D(X1);
    Result.Top := YU2D(Y1);
    Result.Right := XU2D(X2);
    Result.Bottom := YU2D(Y2);
  end;  { CreateRect }

  function TBaseReport.TextWidth(Text: string): double;

  var
    Size: integer;

  begin { TextWidth }
    CheckPrinting;
    if Pos(RPTFPrefix,Text) = 0 then begin
      if Text = '' then begin
        Size := 1;
      end else begin
        SelectRPFont;
        Size := LocalDevice.TextWidth(Text);
        SelectCanvasFont;
      end; { else }
    end else begin
      Size := RPTFTextWidth(Self,Text);
    end; { else }
    Result := XI2U(Size * FScaleX / (FXDPI * 100.0));
  end;  { TextWidth }

{ Memo Methods }

  function TBaseReport.MemoLines(MemoBuf: TBaseMemoBuf): longint;

  begin { MemoLines }
    CheckPrinting;

    MemoBuf.BaseReport := self;
    Result := MemoBuf.MemoLinesLeft;
  end;  { MemoLines }

  function TBaseReport.GetMemoLine(    MemoBuf: TBaseMemoBuf;
                                   var Eol: boolean): string;

  begin { GetMemoLine }
    CheckPrinting;

    MemoBuf.BaseReport := self;
    Result := MemoBuf.GetNextLine(EOL);
  end;  { GetMemoLine }

  procedure TBaseReport.PrintMemo(MemoBuf: TBaseMemoBuf;
                                  Lines: longint;
                                  PrintTabs: boolean);

  begin { PrintMemo }
    CheckPrinting;

    MemoBuf.BaseReport := self;
    MemoBuf.PrintLines(Lines,PrintTabs);
  end;  { PrintMemo }

{ Font Routines }

  procedure TBaseReport.ClearFontList;

  var
    TempFont: PFontNode;

  begin { ClearFontList }
    BaseFont.Handle := 0;
    while Assigned(FontList) do begin
      if FontList^.Font.Handle <> 0 then begin
        DeleteObject(FontList^.Font.Handle);
      end; { if }
      TempFont := FontList;
      FontList := FontList^.Next;
      Dispose(TempFont);
    end; { while }
  end;  { ClearFontList }

  procedure TBaseReport.SelectRPFont;

  var
    LogFont: TLogFont;
    TextMetric: TTextMetric;
    TmpLogFont: TLogFont;
    TempFont: PFontNode;
    PrevFont: PFontNode;
    Fonts: word;

  begin { SelectRPFont }
    if BaseFontChanged then begin { Create new font handle based on BaseFont }
      BaseFont.Handle := 0;
      BaseFontChanged := false;
    end; { if }

    Canvas.Start;

  { Search for BaseFont in FontList }
    Fonts := 0;
    if BaseFont.Handle = 0 then begin
      PrevFont := nil;
      TempFont := FontList;
      while Assigned(TempFont) do begin
        Inc(Fonts);
        with TempFont^.Font do begin
          if (Name = BaseFont.Name) and (Pitch = BaseFont.Pitch) and
           (Height = BaseFont.Height) and (Bold = BaseFont.Bold) and
           (Underline = BaseFont.Underline) and (Italic = BaseFont.Italic) and
           (Strikeout = BaseFont.Strikeout) and (Rotation = BaseFont.Rotation) and
           (Superscript = BaseFont.Superscript) and (Subscript = BaseFont.Subscript) and
           (Charset = BaseFont.Charset) then begin
            BaseFont.Handle := Handle;
            BaseFont.Ascent := Ascent;
            BaseFont.Descent := Descent;
            BaseFont.InternalLeading := InternalLeading;
            BaseFont.Index := Index;
            if Assigned(PrevFont) then begin { Move font to front of list }
              PrevFont^.Next := TempFont^.Next;
              TempFont^.Next := FontList;
              FontList := TempFont;
            end; { if }
            Break;
          end; { if }
        end; { with }
        PrevFont := TempFont;
        TempFont := TempFont^.Next;
      end; { while }
    end; { if }

  { Assign or create new font handle }
    if BaseFont.Handle = 0 then begin { Create new font from BaseFont and save }
    { Create Font }
      with LogFont do begin
        if BaseFont.Superscript or BaseFont.Subscript then begin
          lfHeight := -Round((BaseFont.Height / 1.667) * FYDPI * (FZoom / 100.0));
        end else begin
          lfHeight := -Round(BaseFont.Height * FYDPI * (FZoom / 100.0));
        end; { else }
        lfWidth := Round(BaseFont.Width * FXDPI * (FZoom / 100.0));
        lfEscapement := BaseFont.Rotation * 10;
        lfOrientation := 0;
        if BaseFont.Bold then begin
          lfWeight := FW_BOLD;
        end else begin
          lfWeight := FW_NORMAL;
        end; { else }
        lfItalic := byte(BaseFont.Italic);
        lfUnderline := byte(BaseFont.Underline);
        lfStrikeOut := byte(BaseFont.StrikeOut);
        lfCharSet := ConvertCharset(BaseFont.Charset);
        StrPCopy(lfFaceName,BaseFont.Name);
        TmpLogFont := LogFont;
        TmpLogFont.lfPitchAndFamily := 0;
        if TmpLogFont.lfCharset = Windows.DEFAULT_CHARSET then begin
          CurrCharset := Windows.DEFAULT_CHARSET;
          EnumFontFamiliesEx(QPainter_Handle(Canvas.Handle),TmpLogFont,@EnumFontFamExProc,0,0);
          LogFont.lfCharSet := ConvertCharset(CurrCharset);
        end; { if }
        lfQuality := DEFAULT_QUALITY;
        lfOutPrecision := OUT_DEFAULT_PRECIS;
        lfClipPrecision := CLIP_DEFAULT_PRECIS;
        case BaseFont.Pitch of
          fpVariable: begin
            lfPitchAndFamily := VARIABLE_PITCH;
          end;
          fpFixed: begin
            lfPitchAndFamily := FIXED_PITCH;
          end;
          else begin
            lfPitchAndFamily := DEFAULT_PITCH;
          end;
        end; { case }
      end; { with }

      BaseFont.Handle := CreateFontIndirect(LogFont);
      SaveCanvasFont := SelectObject(QPainter_Handle(Canvas.Handle),BaseFont.Handle);

      if not GetTextMetrics(QPainter_Handle(Canvas.Handle),TextMetric) then begin
        {$IFDEF LEVEL6}
        RaiseLastOSError;
        {$ELSE}
        RaiseLastWin32Error;
        {$ENDIF}
      end; { if }
      BaseFont.Ascent := (TextMetric.tmAscent * 10000.0) / (FYDPI * FScaleY * FZoom);
      BaseFont.Descent := (TextMetric.tmDescent * 10000.0) / (FYDPI * FScaleY * FZoom);
      BaseFont.InternalLeading := (TextMetric.tmInternalLeading * 10000.0) / (FYDPI * FScaleY * FZoom);
      if BaseFont.SuperScript or BaseFont.Subscript then begin
        BaseFont.Ascent := BaseFont.Ascent / 2.0;
        BaseFont.Descent := BaseFont.Descent / 2.0;
        BaseFont.InternalLeading := BaseFont.InternalLeading / 2.0;
      end; { if }
      BaseFont.Index := Fonts;
    { Save BaseFont on FontList }
      New(TempFont);
      TempFont^.Font := BaseFont;
      TempFont^.Next := FontList;
      FontList := TempFont;
    end else begin { Use saved font handle }
      SaveCanvasFont := SelectObject(QPainter_Handle(Canvas.Handle),BaseFont.Handle);
    end; { else }

 { Since NT doesn't print fonts of any color other than black to
   monochrome printers, we have to force the color to black. }
    if (Win32Platform = VER_PLATFORM_WIN32_NT) and not NoNTColorFix and
     (GetDeviceCaps(Windows.HDC(QPainter_Handle(Canvas.Handle)),NUMCOLORS) = 2) and
     (Destination = destPrinter) and (BaseFont.Color <> clWhite) then begin
      SetTextColor(QPainter_Handle(Canvas.Handle),clBlack);
    end else begin
      SetTextColor(QPainter_Handle(Canvas.Handle),ColorToRGB(BaseFont.Color));
    end; { else }
    Canvas.Stop;
  end;  { SelectRPFont }

  procedure TBaseReport.SelectCanvasFont;

  begin { SelectCanvasFont }
    Canvas.Start;
    SelectObject(QPainter_Handle(Canvas.Handle),SaveCanvasFont);
    SetTextColor(QPainter_Handle(Canvas.Handle),ColorToRGB(Canvas.Font.Color));
    Canvas.Stop;
  end;  { SelectCanvasFont }

  procedure TBaseReport.UpdateFont;

  begin { UpdateFont }
    if BaseFontChanged then begin
      SelectRPFont;
      SelectCanvasFont;
    end; { if }
  end;  { UpdateFont }

{ Set to virtual and override in TFileReport }
  procedure TBaseReport.FontOnChange(Sender: TObject);

  begin { FontOnChange }
    if Assigned(FontSaveOnChange) then begin
      FontSaveOnChange(Sender);
    end; { if }

    if DisableFontChange then Exit;

  { Compare Canvas.Font with BaseFont to see if there are any differences }
    with Canvas.Font do begin
      if Name <> BaseFont.Name then begin
        BaseFontChanged := true;
        BaseFont.Name := Name;
      end; { if }
      if Color <> BaseFont.Color then begin
        BaseFontChanged := true;
        BaseFont.Color := Color;
      end; { if }
      if Ord(CharSet) <> BaseFont.CharSet then begin
        BaseFontChanged := true;
        BaseFont.Charset := Ord(Charset);
      end; { if }
      if Pitch <> BaseFont.Pitch then begin
        BaseFontChanged := true;
        BaseFont.Pitch := Pitch;
      end; { if }
      if Abs(Height) <> Round(BaseFont.Height * FYDPI * FZoom / 100.0) then begin
        BaseFontChanged := true;
        BaseFont.Height := (Abs(Height) * 100.0) / (FYDPI * FZoom);
        BaseFont.Size := BaseFont.Height * 72.0;
      end; { if }
      if (fsBold in Style) <> BaseFont.Bold then begin
        BaseFontChanged := true;
        BaseFont.Bold := (fsBold in Style);
      end; { if }
      if (fsItalic in Style) <> BaseFont.Italic then begin
        BaseFontChanged := true;
        BaseFont.Italic := (fsItalic in Style);
      end; { if }
      if (fsUnderline in Style) <> BaseFont.Underline then begin
        BaseFontChanged := true;
        BaseFont.Underline := (fsUnderline in Style);
      end; { if }
      if (fsStrikeout in Style) <> BaseFont.Strikeout then begin
        BaseFontChanged := true;
        BaseFont.Strikeout := (fsStrikeout in Style);
      end; { if }
    end; { with }
  end;  { FontOnChange }

  procedure TBaseReport.SetBold(Value: boolean);

  begin { SetBold }
    with Canvas.Font do begin
      if Value then begin
        Style := Style + [fsBold];
      end else begin
        Style := Style - [fsBold];
      end; { else }
    end; { with }
  end;  { SetBold }

  function TBaseReport.GetBold: boolean;

  begin { GetBold }
    Result := BaseFont.Bold;
  end;  { GetBold }

  procedure TBaseReport.SetUnderline(Value: boolean);

  begin { SetUnderline }
    with Canvas.Font do begin
      if Value then begin
        Style := Style + [fsUnderline];
      end else begin
        Style := Style - [fsUnderline];
      end; { else }
    end; { with }
  end;  { SetUnderline }

  function TBaseReport.GetUnderline: boolean;

  begin { GetUnderline }
    Result := BaseFont.Underline;
  end;  { GetUnderline }

  procedure TBaseReport.SetItalic(Value: boolean);

  begin { SetItalic }
    with Canvas.Font do begin
      if Value then begin
        Style := Style + [fsItalic];
      end else begin
        Style := Style - [fsItalic];
      end; { else }
    end; { with }
  end;  { SetItalic }

  function TBaseReport.GetItalic: boolean;

  begin { GetItalic }
    Result := BaseFont.Italic;
  end;  { GetItalic }

  procedure TBaseReport.SetStrikeout(Value: boolean);

  begin { SetStrikeout }
    with Canvas.Font do begin
      if Value then begin
        Style := Style + [fsStrikeout];
      end else begin
        Style := Style - [fsStrikeout];
      end; { else }
    end; { with }
  end;  { SetStrikeout }

  function TBaseReport.GetStrikeout: boolean;

  begin { GetStrikeout }
    Result := BaseFont.Strikeout;
  end;  { GetStrikeout }

  procedure TBaseReport.SetSuperscript(Value: boolean);

  begin { SetSuperscript }
    if Value then begin
      BaseFont.Subscript := false;
    end; { if }
    BaseFont.Superscript := Value;
    BaseFontChanged := true;
    FontOnChange(self);
  end;  { SetSuperscript }

  function TBaseReport.GetSuperscript: boolean;

  begin { GetSuperscript }
    Result := BaseFont.Superscript;
  end;  { GetSuperscript }

  procedure TBaseReport.SetSubscript(Value: boolean);

  begin { SetSubscript }
    if Value then begin
      BaseFont.Superscript := false;
    end; { if }
    BaseFont.Subscript := Value;
    BaseFontChanged := true;
    FontOnChange(self);
  end;  { SetSubscript }

  function TBaseReport.GetSubscript: boolean;

  begin { GetSubscript }
    Result := BaseFont.Subscript;
  end;  { GetSubscript }

  procedure TBaseReport.SetFontRotation(Value: integer);

  begin { SetFontRotation }
    BaseFont.Rotation := Value mod 360;
    BaseFontChanged := true;
    FontOnChange(self);
  end;  { SetFontRotation }

  function TBaseReport.GetFontRotation: integer;

  begin { GetFontRotation }
    Result := BaseFont.Rotation;
  end;  { GetFontRotation }

  procedure TBaseReport.SetFontColor(Value: TColor);

  begin { SetFontColor }
    Canvas.Font.Color := Value;
  end;  { SetFontColor }

  function TBaseReport.GetFontColor: TColor;

  begin { GetFontColor }
    Result := BaseFont.Color;
  end;  { GetFontColor }

  procedure TBaseReport.SetFontName(Value: string);

  begin { SetFontName }
    Canvas.Font.Name := Value;
  end;  { SetFontName }

  function TBaseReport.GetFontName: string;

  begin { GetFontName }
    Result := BaseFont.Name;
  end;  { GetFontName }

  function TBaseReport.GetFontSize: double;

  begin { GetFontSize }
    Result := BaseFont.Size;
  end;  { GetFontSize }

  procedure TBaseReport.SetFontSize(Value: double);

  begin { SetFontSize }
    Canvas.Font.Height := Round((Value * FYDPI / 72.0) * FScaleY *
     FZoom / 10000.0);
  end;  { SetFontSize }

  function TBaseReport.GetFontHeight: double;

  begin { GetFontHeight }
    Result := YI2U(BaseFont.Height);
  end;  { GetFontHeight }

  procedure TBaseReport.SetFontHeight(Value: double);

  var
    I1: integer;

  begin { SetFontHeight }
    I1 := Round((YU2I(Value) * FYDPI) * FScaleY * FZoom / 10000.0);
    if I1 <= 0 then begin
      I1 := 1;
    end; { if }
    Canvas.Font.Height := I1;
  end;  { SetFontHeight }

  function TBaseReport.GetFontWidth: double;

  begin { GetFontWidth }
    Result := BaseFont.Width;
  end;  { GetFontWidth }

  procedure TBaseReport.SetFontWidth(Value: double);

  begin { SetFontWidth }
    BaseFont.Width := XU2I(Value);
    BaseFontChanged := true;
    FontOnChange(self);
  end;  { SetFontWidth }

  function TBaseReport.GetFontHandle: HFont;

  begin { GetFontHandle }
    if BaseFont.Handle = 0 then begin
      SelectRPFont;
      SelectCanvasFont;
    end; { if }
    Result := BaseFont.Handle;
  end;  { GetFontHandle }

  procedure TBaseReport.SetFontPitch(Value: TFontPitch);

  begin { SetFontPitch }
    Canvas.Font.Pitch := Value;
  end;  { SetFontPitch }

  function TBaseReport.GetFontPitch: TFontPitch;

  begin { GetFontPitch }
    Result := BaseFont.Pitch;
  end;  { GetFontPitch }

  procedure TBaseReport.SetFontCharset(Value: byte);

  begin { SetFontCharset }
    //!!PORT!! RPBASE10
    BaseFont.Charset := Value;
    BaseFontChanged := true;
    FontOnChange(self);
  end;  { SetFontCharset }

  function TBaseReport.GetFontCharset: byte;

  begin { GetFontCharset }
    Result := BaseFont.Charset;
  end;  { GetFontCharset }

  procedure TBaseReport.SetFontAlign(Value: TFontAlign);

  begin { SetFontAlign }
    BaseFont.Align := Value;
    FontOnChange(self);
  end;  { SetFontAlign }

  function TBaseReport.GetFontAlign: TFontAlign;

  begin { GetFontAlign }
    Result := BaseFont.Align;
  end;  { GetFontAlign }

  procedure TBaseReport.GrabFontOnChange;

  begin { GrabFontOnChange }
    FontSaveOnChange := Canvas.Font.OnChange;
    Canvas.Font.OnChange := FontOnChange;
    DisableFontChange := false;
    Canvas.Font.PixelsPerInch := FYDPI;
  end;  { GrabFontOnChange }

  procedure TBaseReport.ReleaseFontOnChange;

  begin { ReleaseFontOnChange }
    Canvas.Font.OnChange := FontSaveOnChange;
  end;  { ReleaseFontOnChange }

  procedure TBaseReport.SetFont(NewName: string;
                                NewSize: double);

  begin { SetFont }
    if NewSize > 0 then begin
      DisableFontChange := true;
      FontName := NewName;
      FontColor := clBlack;
      FontPitch := fpDefault;
      //!!PORT!! RPBASE10
      FontSize := NewSize;
      Bold := false;
      Underline := false;
      Italic := false;
      Strikeout := false;
      DisableFontChange := false;
      with BaseFont do begin
        Superscript := false;
        Subscript := false;
        Rotation := 0;
        Width := 0.0;
        Align := faBaseline;
      end; { with }
      FontOnChange(self); { Copy over items from Canvas.Font }
    end; { if }
  end;  { SetFont }

  function TBaseReport.CreateFont(NewName: string;
                                  NewSize: integer): TFont;

  begin { CreateFont }
    Result := TFont.Create;
    with Result do begin
      Name := NewName;
      Size := NewSize;
      Pitch := fpDefault;
      Color := clBlack;
      Style := [];
    end; { with }
  end;  { CreateFont }

  function TBaseReport.GetFontTop: double;

  begin { GetFontTop }
    Result := YI2U(FCursorYPos - FAscentHeight);
  end;  { GetFontTop }

  procedure TBaseReport.SetFontTop(Value: double);

  begin { SetFontTop }
    ResetLineHeight;
    FCursorYPos := YU2I(Value) + FAscentHeight;
  end;  { SetFontTop }

  function TBaseReport.GetFontBaseline: double;

  begin { GetFontBaseline }
    Result := YI2U(FCursorYPos);
  end;  { GetFontBaseline }

  procedure TBaseReport.SetFontBaseline(Value: double);

  begin { SetFontBaseline }
    ResetLineHeight;
    FCursorYPos := YU2I(Value);
  end;  { SetFontBaseline }

  function TBaseReport.GetFontBottom: double;

  begin { GetFontBottom }
    Result := YI2U(FCursorYPos + FDescentHeight);
  end;  { GetFontBottom }

  procedure TBaseReport.SetFontBottom(Value: double);

  begin { SetFontBottom }
    ResetLineHeight;
    FCursorYPos := YU2I(Value) - FDescentHeight;
  end;  { SetFontBottom }

  function TBaseReport.GetLineTop: double;

  begin { GetLineTop }
    Result := FontBottom - YI2U(FLineHeight);
  end;  { GetLineTop }

  procedure TBaseReport.SetLineTop(Value: double);

  begin { SetLineTop }
    ResetLineHeight;
    FCursorYPos := YU2I(Value) + FLineHeight - FDescentHeight;
  end;  { SetLineTop }

  function TBaseReport.GetLineMiddle: double;

  begin { GetLineMiddle }
    Result := FontBottom - (YI2U(FLineHeight) / 2.0);
  end;  { GetLineMiddle }

  procedure TBaseReport.SetLineMiddle(Value: double);

  begin { SetLineMiddle }
    ResetLineHeight;
    FCursorYPos := YU2I(Value) + (FLineHeight / 2.0) - FDescentHeight;
  end;  { SetLineMiddle }

  function TBaseReport.GetLineBottom: double;

  begin { GetLineBottom }
    Result := FontBottom;
  end;  { GetLineBottom }

  procedure TBaseReport.SetLineBottom(Value: double);

  begin { SetLineBottom }
    SetFontBottom(Value);
  end;  { SetLineBottom }

  function TBaseReport.GetLineHeight: double;

  begin { GetLineHeight }
    Result := YI2U(FLineHeight);
  end;  { GetLineHeight }

  procedure TBaseReport.SetLineHeight(Value: double);

  begin { SetLineHeight }
    FLineHeightMethod := lhmUser;
    FLineHeight := YU2I(Value);
  end;  { SetLineHeight }

  function TBaseReport.GetAscentHeight: double;

  begin { GetAscentHeight }
    Result := YI2U(FAscentHeight);
  end;  { GetAscentHeight }

  function TBaseReport.GetDescentHeight: double;

  begin { GetDescentHeight }
    Result := YI2U(FDescentHeight);
  end;  { GetDescentHeight }

  procedure TBaseReport.UnregisterGraphic(Index: integer);

  begin { UnregisterGraphic }
  end;  { UnregisterGraphic }

  procedure TBaseReport.RegisterGraphic(Index: integer);

  begin { RegisterGraphic }
  end;  { RegisterGraphic }

  procedure TBaseReport.ReuseGraphic(Index: integer);

  begin { ReuseGraphic }
  end;  { ReuseGraphic }

  function TBaseReport.CalcGraphicHeight(Width: double;
                                         Graphic: TGraphic): double;

  begin { CalcGraphicHeight }
    Result := Graphic.Height * Width / Graphic.Width;
  end;  { CalcGraphicHeight }

  function TBaseReport.CalcGraphicWidth(Height: double;
                                        Graphic: TGraphic): double;

  begin { CalcGraphicWidth }
    Result := Graphic.Width * Height / Graphic.Height;
  end;  { CalcGraphicWidth }

  function TBaseReport.GetLocalDevice: TRPBaseDevice;

  begin { GetLocalDevice }
    if Assigned(FLocalDevice) then begin
      Result := FLocalDevice;
    end else begin
      Result := GlobalDevice;
    end; { else }
  end;  { GetLocalDevice }

procedure TBaseReport.ProcessMessages;
begin
  if not Threaded then begin
    Application.ProcessMessages;
  end;
end;

end.
