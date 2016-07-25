{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit QRvClass;

interface

{$I RpVer.inc}


uses
  Types, QControls, QGraphics, QStdCtrls, QForms, QButtons, QExtCtrls,
  TypInfo, Classes, SysUtils, QRvDefine, QRvUtil, QRpDefine, QRpBase;

type
{!!PORT!! HRgn not defined in CLX}
  HRgn = integer;

  TRaveComponent = class;
  TRaveControl = class;
  TRaveContainerControl = class;
  TRaveProjectItem = class;
  TRavePage = class;
  TRaveDesigner = class;
  TRavePip = class;
  TRaveFiler = class;
  TRaveReader = class;
  TRaveWriter = class;
  TRaveFont = class;
  TRaveModule = class;
  TRaveEvent = class;
  TRaveSimpleEvent = class;

  TRaveComponentStyle = (rcsContainer,rcsInvisible,rcsLoading,rcsReflecting);
  TRaveComponentStyleSet = set of TRaveComponentStyle;
  TRaveListenEvent = procedure(Speaker: TRaveComponent;
                               Msg: TObject) of object;
  TRavePersistentList = class(TList)
  public
    procedure Assign(Source: TRavePersistentList);
    function CreateItem(ItemClass: string;
                        RootOwner: TComponent): TPersistent; virtual;
  end; { TRavePersistentList }

  TRaveComponentList = class(TList)
  end; { TRaveComponentList }

  TRaveMethodList = class(TList)
  public
    destructor Destroy; override;
    procedure AddMethod(Method: TMethod);
    procedure RemoveMethod(Method: TMethod);
    procedure RemoveObject(Obj: TObject);
    procedure ReplaceObject(OldObj: TObject;
                            NewObj: TObject);
  end; { TRaveMethodList }

  TRaveComponentClass = class of TRaveComponent;
  TRaveComponent = class(TComponent)
  protected
    FControlStyle: TRaveComponentStyleSet;
    FDevLocked: boolean;
    FHolder: TObject; { Design Time Representation }
    FParent: TRaveComponent;
    FVisible: boolean;
    FLocked: boolean;
    FPage: TRavePage;
    FReport: TRaveProjectItem;
    FProject: TRaveProjectItem;
    FOnBeforeReport: TRaveSimpleEvent;
    FOnAfterReport: TRaveSimpleEvent;
    FOnBeforePrint: TRaveSimpleEvent;
    FOnAfterPrint: TRaveSimpleEvent;

    class function UseMaster: boolean; virtual;
    class function AcceptParent(NewParent: TClass): boolean; virtual;
    procedure DestroyEvents;
    function AcceptChild(NewChild: TClass): boolean; virtual;
    procedure DestroyChildren; virtual;
    procedure PostLoad; virtual;
    procedure SetParent(Value: TRaveComponent); virtual;
    procedure SetVisible(Value: boolean);
    procedure AddChild(Value: TRaveComponent); virtual;
    procedure RemoveChild(Value: TRaveComponent); virtual;
    function GetChild(Index: integer): TRaveComponent; virtual;
    function GetChildCount: integer; virtual;
    function GetChildIndex: integer; virtual;
    procedure SetParentComponent(Value: TComponent); override;
    function GetPageLeft: TRaveUnits; virtual;
    function GetPageTop: TRaveUnits; virtual;
    function GetPageRight: TRaveUnits; virtual;
    function GetPageBottom: TRaveUnits; virtual;
    procedure SetName(const NewName: TComponentName); override;
    procedure OverrideProperties(Filer: TRaveFiler); virtual;
    function GetDonePrinting: boolean; virtual;
    procedure BeforeReport; virtual;
    procedure AfterReport; virtual;
    procedure BeforePrint; virtual;
    procedure Print(Report: TBaseReport); virtual;
    procedure AfterPrint; virtual;
    procedure InitData; virtual;
    procedure DoneData; virtual;
    procedure SetDepth(Control: TRaveComponent; Adjustment: integer); virtual;
    procedure Changing(OldItem: TRaveComponent; NewItem: TRaveComponent); virtual;
    procedure SetLocked(Value: boolean);
    procedure SetDevLocked(Value: boolean);
    procedure SaveDesigned; virtual;
    procedure RestoreDesigned; virtual;
    procedure SaveAdjusted; virtual;
    procedure RestoreAdjusted; virtual;
    procedure Speak(List: TRaveMethodList; Msg: TObject);
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure Hide; virtual;
    procedure Invalidate;
    procedure Show; virtual;

    function GetParentComponent: TComponent; override;
    procedure MoveForward;
    procedure MoveBehind;
    procedure BringToFront;
    procedure SendToBack;
    function AllowPrint: boolean; virtual;
    procedure PrintAll(Report: TBaseReport); virtual;
    function CreateChild(AClassType: TRaveComponentClass;
                         ABaseName: string = '';
                         ALeft: TRaveUnits = 0.0;
                         ATop: TRaveUnits = 0.0;
                         AWidth: TRaveUnits = 0.0;
                         AHeight: TRaveUnits = 0.0): TRaveComponent;
    function IsLocked: boolean;
    function IsLocker: boolean;
    function GetLocker: TRaveComponent;
    function XU2I(Value: TRaveUnits): TRaveUnits;
    function YU2I(Value: TRaveUnits): TRaveUnits;
    function XI2U(Value: TRaveUnits): TRaveUnits;
    function YI2U(Value: TRaveUnits): TRaveUnits;
    function CanSelect(Item: TRaveComponent): boolean; virtual;
    procedure PrintParams(var HDelta: TRaveUnits;
                          var VDelta: TRaveUnits); virtual;

    procedure AddListener(Conversation: string;
                          ListenMethod: TRaveListenEvent); virtual;
    procedure RemoveListener(Conversation: string;
                             ListenMethod: TRaveListenEvent); virtual;
    function Habla(Conversation: string): boolean; virtual;

    property PageLeft: TRaveUnits read GetPageLeft;
    property PageTop: TRaveUnits read GetPageTop;
    property PageRight: TRaveUnits read GetPageRight;
    property PageBottom: TRaveUnits read GetPageBottom;
    property Child[Index: integer]: TRaveComponent read GetChild;
    property ChildCount: integer read GetChildCount;
    property ChildIndex: integer read GetChildIndex;
    property ControlStyle: TRaveComponentStyleSet read FControlStyle write FControlStyle;
    property DonePrinting: boolean read GetDonePrinting;
    property Page: TRavePage read FPage;
    property Report: TRaveProjectItem read FReport;
    property Project: TRaveProjectItem read FProject;
    property Parent: TRaveComponent read FParent write SetParent;
  published
    property Locked: boolean read FLocked write SetLocked default false;
    property DevLocked: boolean read FDevLocked write SetDevLocked default false;
    property Visible: boolean read FVisible write SetVisible default true;
    property OnBeforeReport: TRaveSimpleEvent read FOnBeforeReport write FOnBeforeReport;
    property OnAfterReport: TRaveSimpleEvent read FOnAfterReport write FOnAfterReport;
    property OnBeforePrint: TRaveSimpleEvent read FOnBeforePrint write FOnBeforePrint;
    property OnAfterPrint: TRaveSimpleEvent read FOnAfterPrint write FOnAfterPrint;
  end; { TRaveComponent }


  TRaveControl = class(TRaveComponent)
  protected
    FMirror: TRaveControl;
    FMirrorList: TList;
    FMirrorPoint: TRavePoint;
    FCursor: TCursor;
    FLeft: TRaveUnits;
    FTop: TRaveUnits;
    FWidth: TRaveUnits;
    FHeight: TRaveUnits;
    FDesLeftx: TRaveUnits;
    FDesTopx: TRaveUnits;
    FDesWidthx: TRaveUnits;
    FDesHeightx: TRaveUnits;
    FAdjLeft: TRaveUnits;
    FAdjTop: TRaveUnits;
    FAdjWidth: TRaveUnits;
    FAdjHeight: TRaveUnits;
    FDisplayOn: TDisplayOn;
    FAnchor: TRaveAnchor;
    DisableResize: boolean;

    procedure AddMirror(Value: TRaveControl); virtual;
    procedure RemoveMirror(Value: TRaveControl); virtual;
    procedure SetMirror(Value: TRaveControl);
    procedure PrintMirror(AReport: TBaseReport;
                          AOrig: TRaveControl;
                          AControl: TRaveControl);
    procedure PaintMirror(Canvas: TCanvas;
                          Orig: TRaveControl;
                          Control: TRaveControl);
    procedure SetFontAttr(Index: TFontAttribute;
                          Value: TRaveFont); virtual;
    function GetFontAttr(Index: TFontAttribute): TRaveFont; virtual;
    function XI2P(Value: TRaveUnits): longint;
    function YI2P(Value: TRaveUnits): longint;
    function XI2D(Value: TRaveUnits): longint;
    function YI2D(Value: TRaveUnits): longint;
    function XD2I(Value: longint): TRaveUnits;
    function YD2I(Value: longint): TRaveUnits;
    function SetClipped(Canvas: TCanvas): HRgn;
    procedure SetUnclipped(Canvas: TCanvas;
                           SaveRgn: HRgn);
    function PageToClient(Point: TRavePoint): TRavePoint;
    function ClientToPage(Point: TRavePoint): TRavePoint;
    procedure AssignTo(Dest: TPersistent); override;
    procedure SetText(Value: string); virtual;
    function GetText: string; virtual;
    procedure SetFontJustify(Value: TPrintJustify); virtual;
    function GetFontJustify: TPrintJustify; virtual;
    procedure SetForeColor(Value: TColor); virtual;
    function GetForeColor: TColor; virtual;
    procedure SetBackColor(Value: TColor); virtual;
    function GetBackColor: TColor; virtual;
    procedure SetFillStyle(Value: TRaveFillStyle); virtual;
    function GetFillStyle: TRaveFillStyle; virtual;
    procedure SetLineWidth(Value: TRaveFloat); virtual;
    function GetLineWidth: TRaveFloat; virtual;
    procedure SetLineWidthType(Value: TLineWidthType); virtual;
    function GetLineWidthType: TLineWidthType; virtual;
    procedure SetLineStyle(Value: TPenStyle); virtual;
    function GetLineStyle: TPenStyle; virtual;
    procedure Changing(OldItem: TRaveComponent;
                       NewItem: TRaveComponent); override;
    function GetDonePrinting: boolean; override;

    procedure AnchorAdjust(WidthChanging: boolean;
                           OldWidth,NewWidth: TRaveUnits;
                           HeightChanging: boolean;
                           OldHeight,NewHeight: TRaveUnits); virtual;
    procedure AnchorAdjustAll(WidthChanging: boolean;
                              OldWidth,NewWidth: TRaveUnits;
                              HeightChanging: boolean;
                              OldHeight,NewHeight: TRaveUnits); virtual;
    procedure ResizeAll;
    function GetLeft: TRaveUnits; virtual;
    procedure SetLeft(Value: TRaveUnits); virtual;
    function GetTop: TRaveUnits; virtual;
    procedure SetTop(Value: TRaveUnits); virtual;
    function GetWidth: TRaveUnits; virtual;
    procedure SetWidth(Value: TRaveUnits); virtual;
    function GetHeight: TRaveUnits; virtual;
    procedure SetHeight(Value: TRaveUnits); virtual;
    function GetRight: TRaveUnits;
    procedure SetRight(Value: TRaveUnits);
    function GetBottom: TRaveUnits;
    procedure SetBottom(Value: TRaveUnits);
    function GetPageLeft: TRaveUnits; override;
    function GetPageTop: TRaveUnits; override;
    function GetPageRight: TRaveUnits; override;
    function GetPageBottom: TRaveUnits; override;
    procedure SetBoundsRect(Rect: TRaveRect);
    function GetBoundsRect: TRaveRect;
    procedure SetAnchor(Value: TRaveAnchor);
    procedure InitData; override;
    procedure DoneData; override;
    procedure SaveDesigned; override;
    procedure RestoreDesigned; override;
    procedure SaveAdjusted; override;
    procedure RestoreAdjusted; override;
    function GetVAnchor: TRaveVAnchor;
    function GetHAnchor: TRaveHAnchor;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure Reflect;
    procedure CreatePips; virtual;
    procedure UpdatePips; virtual;
    procedure PipSize(RavePip: TRavePip;
                      X: TRaveUnits;
                      Y: TRaveUnits); virtual;
    function IsSelectPoint(Point: TRavePoint): boolean; virtual;
    function InSelectRect(Rect: TRaveRect): boolean; virtual;
    class procedure ModifyRect(var P1: TPoint;
                                                      var P2: TPoint;
                                                          PipIndex: byte); virtual;
    procedure PaintAll(Canvas: TCanvas); virtual;
    procedure Paint(Canvas: TCanvas); virtual;
    function FindControlAt(var AControl: TRaveControl;
                               Point: TRavePoint): boolean;
    function FindContainerAt(var Container: TRaveControl;
                                 Point: TRavePoint;
                                 NewChild: TClass): boolean;
    class procedure ModifyRaveRect(var P1: TRavePoint;
                                   var P2: TRavePoint); virtual;
    procedure SetPos(ALeft: TRaveUnits;
                     ATop: TRaveUnits);
    procedure SetSize(P1: TRavePoint;
                      P2: TRavePoint); virtual;
    procedure Resize; virtual;
    function AllowPrint: boolean; override;
    procedure PrintAll(Report: TBaseReport); override;
    function HeightToPrint: TRaveUnits; virtual;

    property xDesLeft: TRaveUnits read FDesLeftx;
    property xDesTop: TRaveUnits read FDesTopx;
    property xDesWidth: TRaveUnits read FDesWidthx;
    property xDesHeight: TRaveUnits read FDesHeightx;
    property AdjLeft: TRaveUnits read FAdjLeft;
    property AdjTop: TRaveUnits read FAdjTop;
    property AdjWidth: TRaveUnits read FAdjWidth;
    property AdjHeight: TRaveUnits read FAdjHeight;
    property VAnchor: TRaveVAnchor read GetVAnchor;
    property HAnchor: TRaveHAnchor read GetHAnchor;
    property Cursor: TCursor read FCursor write FCursor default crDefault;
    property FontAttr[Index: TFontAttribute]: TRaveFont read GetFontAttr write SetFontAttr;
    property FontJustify: TPrintJustify read GetFontJustify write SetFontJustify;
    property BackColor: TColor read GetBackColor write SetBackColor;
    property ForeColor: TColor read GetForeColor write SetForeColor;
    property FillStyle: TRaveFillStyle read GetFillStyle write SetFillStyle;
    property Text: string read GetText write SetText;
    property LineWidth: TRaveFloat read GetLineWidth write SetLineWidth;
    property LineWidthType: TLineWidthType read GetLineWidthType write
     SetLineWidthType;
    property LineStyle: TPenStyle read GetLineStyle write SetLineStyle;
    property BoundsRect: TRaveRect read GetBoundsRect write SetBoundsRect;
    property Left: TRaveUnits read GetLeft write SetLeft;
    property Right: TRaveUnits read GetRight write SetRight;
    property Top: TRaveUnits read GetTop write SetTop;
    property Bottom: TRaveUnits read GetBottom write SetBottom;
    property Width: TRaveUnits read GetWidth write SetWidth;
    property Height: TRaveUnits read GetHeight write SetHeight;
    property MirrorPoint: TRavePoint read FMirrorPoint;
  published
    property Anchor: TRaveAnchor read FAnchor write SetAnchor default 0;
    property Mirror: TRaveControl read FMirror write SetMirror default nil;
    property DisplayOn: TDisplayOn read FDisplayOn write FDisplayOn default doParent;
  end; { TRaveControl }
  TRaveControlClass = class of TRaveControl;

  TRaveContainerControl = class(TRaveControl)
  private
    ChildList: TList;
  protected
    procedure DestroyChildren; override;
    procedure AddChild(Value: TRaveComponent); override;
    procedure RemoveChild(Value: TRaveComponent); override;
    function GetChild(Index: integer): TRaveComponent; override;
    function GetChildCount: integer; override;
    procedure SetDepth(Control: TRaveComponent;
                       Adjustment: integer); override;
  public
    constructor Create(AOwner: TComponent); override;
  end; { TRaveContainerControl }

  TRaveContainerComponent = class(TRaveComponent)
  private
    ChildList: TList;
  protected
    procedure DestroyChildren; override;
    procedure AddChild(Value: TRaveComponent); override;
    procedure RemoveChild(Value: TRaveComponent); override;
    function GetChild(Index: integer): TRaveComponent; override;
    function GetChildCount: integer; override;
    procedure SetDepth(Control: TRaveComponent;
                       Adjustment: integer); override;
  public
    constructor Create(AOwner: TComponent); override;
  end; { TRaveContainerComponent }

  TRaveProjectItem = class(TRaveContainerComponent)
  protected
    FActive: boolean;
    FDescription: TRaveMemoString;
    FFullName: string;
    FParams: TStrings;
    FPIVars: TStrings;
    FModule: TRaveModule;
    FCompileNeeded: boolean;
    FIncluded: boolean;

    procedure ReadModule(Reader: TReader);
    procedure WriteModule(Writer: TWriter);
    procedure SetModule(AModule: TRaveModule);
    procedure DefineProperties(Filer: TFiler); override;
    procedure SetActive(Value: boolean);
    procedure SetName(const NewName: TComponentName); override;
    procedure SetParameters(Value: TStrings);
    procedure SetPIVars(Value: TStrings);
    procedure SetCompileNeeded(Value: boolean);
    function GetModuleName: string;
    procedure PostLoad; override;
    procedure Changing(OldItem: TRaveComponent;
                       NewItem: TRaveComponent); override;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure SaveToStreamHelper(StreamHelper: TStreamHelper); virtual;
    procedure LoadFromStreamHelper(StreamHelper: TStreamHelper); virtual;
    procedure Open; virtual;
    procedure Close; virtual;
    procedure PrepareModule; virtual;

    property Active: boolean read FActive write SetActive;
    property Parameters: TStrings read FParams write SetParameters;
    property PIVars: TStrings read FPIVars write SetPIVars;
    property Module: TRaveModule read FModule write SetModule;
    property ModuleName: string read GetModuleName;
    property Included: boolean read FIncluded write FIncluded;
  published
    property CompileNeeded: boolean read FCompileNeeded write SetCompileNeeded;
    property Description: TRaveMemoString read FDescription write FDescription;
    property FullName: string read FFullName write FFullName;
  end; { TRaveProjectItem }

  TRaveDataObject = class(TRaveProjectItem)
  end; { TRaveDataObject }
  TRaveDataObjectClass = class of TRaveDataObject;

  TRavePage = class(TRaveProjectItem)
  protected
    FDesigner: TRaveDesigner;
    FGlobal: boolean;
    FPaperSize: TRavePaperSize;
    FPageWidth: TRaveUnits;
    FPageHeight: TRaveUnits;
    FStorage: TWinControl; { Where item is stored in IDE }
    FGotoMode: TGotoMode;
    FGotoPage: TRavePage;
    FGridSpacing: TRaveUnits;
    FGridLines: integer;
    FOrientation: TOrientation;
    FBin: TRaveBin;
    FBinCustom: string;
    FWasteFit: boolean;

    procedure SetGridLines(Value: integer);
    procedure SetGridSpacing(Value: TRaveUnits);
    procedure AnchorAdjustAll(WidthChanging: boolean;
                              OldWidth,NewWidth: TRaveUnits;
                              HeightChanging: boolean;
                              OldHeight,NewHeight: TRaveUnits);
    procedure SetPageWidth(Value: TRaveUnits);
    procedure SetPageHeight(Value: TRaveUnits);
    procedure AssignTo(Dest: TPersistent); override;
    procedure Changing(OldItem: TRaveComponent; NewItem: TRaveComponent); override;
    procedure SetPaperSize(Value: TRavePaperSize);
    procedure SetOrientation(Value: TOrientation);
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure LoadFromStreamHelper(StreamHelper: TStreamHelper); override;
    procedure PrintAll(Report: TBaseReport); override;
    procedure Resize;
    function StorePageDim: boolean;
    procedure PrepareModule; override;
    procedure PrepareSize(AWidth: TRaveUnits; AHeight: TRaveUnits);

    procedure Paint(Canvas: TCanvas);
    procedure AddPip(Index: byte; Control: TRaveControl; Cursor: TCursor; X, Y: TRaveUnits);
    procedure UpdatePip(Index: byte; Control: TRaveControl; X, Y: TRaveUnits);
    procedure SwitchPips(RavePip: TRavePip; SwitchIdx: byte);

    property Designer: TRaveDesigner read FDesigner write FDesigner;
    property Global: boolean read FGlobal;
    property Storage: TWinControl read FStorage write FStorage;
  published
    property Bin: TRaveBin read FBin write FBin;
    property BinCustom: string read FBinCustom write FBinCustom;
    property GotoMode: TGotoMode read FGotoMode write FGotoMode default gmGotoDone;
    property GotoPage: TRavePage read FGotoPage write FGotoPage default nil;
    property GridLines: integer read FGridLines write SetGridLines;
    property GridSpacing: TRaveUnits read FGridSpacing write SetGridSpacing;
    property Orientation: TOrientation read FOrientation write SetOrientation;
    property PageHeight: TRaveUnits read FPageHeight write SetPageHeight stored StorePageDim;
    property PageWidth: TRaveUnits read FPageWidth write SetPageWidth stored StorePageDim;
    property PaperSize: TRavePaperSize read FPaperSize write SetPaperSize;
    property Parameters;
    property PIVars;
    property WasteFit: boolean read FWasteFit write FWasteFit default false;
  end; { TRavePage }

  TRaveDesigner = class(TCustomControl)
  protected
    FGridPen: TPen;
    FPage: TRavePage;
    FMinimumBorder: TRaveFloat;
    FZoomFactor: TRaveFloat;

    procedure SetPage(APage: TRavePage);
    procedure SetMinimumBorder(Value: TRaveFloat); virtual; abstract;
    procedure SetGridPen(Value: TPen); virtual; abstract;
    procedure SetZoomFactor(Value: TRaveFloat); virtual; abstract;
    function GetSelections: integer; virtual; abstract;
    function GetSelection(Index: integer): TRaveComponent; virtual; abstract;
    function GetZoomToolActive: boolean; virtual; abstract;
    procedure SetZoomToolActive(Value: boolean); virtual; abstract;
  public
  { Pip methods }
    procedure AddPip(Index: byte;
                     Control: TRaveControl;
                     Cursor: TCursor;
                     X: TRaveUnits;
                     Y: TRaveUnits); virtual; abstract;
    procedure UpdatePip(Index: byte;
                        Control: TRaveControl;
                        X: TRaveUnits;
                        Y: TRaveUnits); virtual; abstract;
    procedure RemovePips(Control: TRaveControl); virtual; abstract;
    procedure MovePip(Index: byte;
                      X: TRaveUnits;
                      Y: TRaveUnits); virtual; abstract;
    procedure SwitchPips(RavePip: TRavePip;
                         SwitchIdx: byte); virtual; abstract;
    procedure Modified; virtual; abstract;

  { Selection methods }
    procedure DeselectControl(Control: TRaveComponent); virtual; abstract;
    procedure ClearSelection; virtual; abstract;
    procedure SelectControl(Control: TRaveComponent); virtual; abstract;
    procedure ToggleControl(Control: TRaveComponent); virtual; abstract;
    function IsSelected(Control: TRaveComponent): boolean; virtual; abstract;
    procedure DeleteSelection; virtual; abstract;
    procedure CopySelection; virtual; abstract;
    procedure PasteSelection; virtual; abstract;
    procedure SelectChildren(Control: TRaveComponent); virtual; abstract;
    procedure SelectType(ProjectItem: TRaveProjectItem;
                         RaveClass: TClass); virtual; abstract;
    procedure MoveSelection(X: TRaveUnits;
                            Y: TRaveUnits); virtual; abstract;
    procedure AlignSelection(AlignType: integer); virtual; abstract;
    procedure PostChanges; virtual; abstract;

  { Find methods }
    function FindControl(Name: string): TRaveComponent; virtual; abstract;
    function FindControlAt(X,Y: TRaveUnits): TRaveControl; virtual; abstract;
    function FindContainerAt(X,Y: TRaveUnits;
                             NewChild: TClass): TRaveControl; virtual; abstract;

  { Position methods }
    function XI2D(Value: TRaveUnits): longint; virtual; abstract;
    function YI2D(Value: TRaveUnits): longint; virtual; abstract;
    function XD2I(Value: longint): TRaveUnits; virtual; abstract;
    function YD2I(Value: longint): TRaveUnits; virtual; abstract;
    function SnapX(Value: TRaveUnits): TRaveUnits; virtual; abstract;
    function SnapY(Value: TRaveUnits): TRaveUnits; virtual; abstract;
    function VisibleWidth: TRaveUnits; virtual; abstract;
    function VisibleHeight: TRaveUnits; virtual; abstract;
    function CenterWidth: TRaveUnits; virtual; abstract;
    function CenterHeight: TRaveUnits; virtual; abstract;

  { Zooming methods }
    function ZoomToRect(X1,Y1,X2,Y2: TRaveUnits): TRaveFloat; virtual; abstract;
    procedure ZoomPage; virtual; abstract;
    procedure ZoomPageWidth; virtual; abstract;
    procedure ZoomSelected; virtual; abstract;
    procedure ZoomIn(X,Y: TRaveUnits); virtual; abstract;
    procedure ZoomOut; virtual; abstract;
    procedure CenterWindow(X,Y: TRaveUnits); virtual; abstract;

    property GridPen: TPen read FGridPen write SetGridPen;
    property MinimumBorder: TRaveFloat read FMinimumBorder write SetMinimumBorder;
    property Page: TRavePage read FPage;
    property Selections: integer read GetSelections;
    property Selection[Index: integer]: TRaveComponent read GetSelection;
    property ZoomFactor: TRaveFloat read FZoomFactor write SetZoomFactor;
    property ZoomToolActive: boolean read GetZoomToolActive write SetZoomToolActive;
  end; { TRaveDesigner }

  TRavePip = class
  protected
    function GetScreenRect: TRect;
  public
    Index: byte; { Reference index of this pip }
    Control: TRaveControl; { Control that this pip modifies }
    Designer: TRaveDesigner; { Designer that manages this pip }
    Cursor: TCursor; { Cursor to display }
    PageX: TRaveUnits; { XPos of pip on the page }
    PageY: TRaveUnits; { YPos of pip on page }
    StartSizePoint: TPoint; { Sizing start point on screen }
    OrigRect: TRaveRect; { Original BoundsRect before sizing started }
    OrigPoint: TRavePoint; { Original X,Y on page before sizing started }

    constructor Create(AIndex: byte;
                       AControl: TRaveControl;
                       ADesigner: TRaveDesigner;
                       ACursor: TCursor;
                       AX: TRaveUnits;
                       AY: TRaveUnits);
    destructor Destroy; override;
    procedure Reset;
    procedure Move(X: TRaveUnits;
                   Y: TRaveUnits);
    procedure Draw(Canvas: TCanvas);

    property ScreenRect: TRect read GetScreenRect; { Screen rect for pip }
  end; { TRavePip }

{ Filer classes }
  TValueKind = (vkID,vkIndex,vkNegIndex,vkInteger,vkChar,vkFloat,vkString,vkSet,
   vkPropList,vkComponentList,vkPersistentList);
  TRaveReaderProc = procedure(Reader: TRaveReader) of object;
  TRaveWriterProc = procedure(Writer: TRaveWriter) of object;
  TRaveFiler = class(TFiler)
  protected
    FStreamHelper: TStreamHelper;
    FOverrideList: TStringList;
    FRootOwner: TRaveComponent;
    FIsReader: boolean;
  public
    constructor Create(AStreamHelper: TStreamHelper);
    destructor Destroy; override;

    procedure OverrideProperty(Name: string;
                               ReadData: TRaveReaderProc;
                               WriteData: TRaveWriterProc);
    function PropertyOverride(    Name: string;
                              var ReadData: TRaveReaderProc;
                              var WriteData: TRaveWriterProc): boolean;
    procedure DefineProperty(const Name: string;
                                   ReadData: TReaderProc;
                                   WriteData: TWriterProc;
                                   HasData: boolean); override;
    procedure DefineBinaryProperty(const Name: string;
                                         ReadData: TStreamProc;
                                         WriteData: TStreamProc;
                                         HasData: boolean); override;
    procedure FlushBuffer; override;

    property IsReader: boolean read FIsReader;
    property OverrideList: TStringList read FOverrideList;
    property RootOwner: TRaveComponent read FRootOwner;
    property StreamHelper: TStreamHelper read FStreamHelper;
  end; { TRaveFiler }

  TRaveWriter = class(TRaveFiler)
  protected
    DefineInstance: TPersistent;
  public
    constructor Create(AStreamHelper: TStreamHelper);
    destructor Destroy; override;
    procedure DefineProperty(const Name: string;
                                   ReadData: TReaderProc;
                                   WriteData: TWriterProc;
                                   HasData: boolean); override;
    procedure DefineBinaryProperty(const Name: string;
                                         ReadData: TStreamProc;
                                         WriteData: TStreamProc;
                                         HasData: boolean); override;
    procedure FlushBuffer; override;
    procedure WriteComponent(Value: TRaveComponent;
                             DoChildren: boolean);
    procedure WriteChildren(Instance: TRaveComponent);
    procedure WritePropName(PropInfo: PPropInfo);
    procedure WriteProperties(Instance: TPersistent);
    procedure WriteProperty(Instance: TPersistent;
                            PropInfo: PPropInfo);
    procedure WriteID(Value: string);
    procedure WriteInteger(Value: longint);
    procedure WriteChar(Value: char);
    procedure WriteFloat(Value: extended);
    procedure WriteString(Value: string);
  end; { TRaveWriter }

  TRaveReader = class(TRaveFiler)
  protected
    PropHandled: boolean; { Property was handled by Instance.DefineProperties }
    PropName: string;
    FAddComponents: boolean; { Call AddComponent for new components? }
    FCheckNames: boolean; { Check names for uniquness and alter if necessary? }
    FAbsorbCount: integer; { Level of absorb calls, used by AbsordProperty }
    FCurrentParent: TRaveComponent;
    FCurrentComp: TRaveComponent;
    FMasterProject: TRaveProjectItem;
  public
    constructor Create(AStreamHelper: TStreamHelper);
    destructor Destroy; override;
    procedure DefineProperty(const Name: string;
                                   ReadData: TReaderProc;
                                   WriteData: TWriterProc;
                                   HasData: boolean); override;
    procedure DefineBinaryProperty(const Name: string;
                                         ReadData: TStreamProc;
                                         WriteData: TStreamProc;
                                         HasData: boolean); override;
    procedure FlushBuffer; override;
    function ReadComponent(aOwner: TRaveComponent;
                           aParent: TRaveComponent): TRaveComponent;
    procedure ReadIntoComponent(Value: TRaveComponent);
    function AbsorbProperty: boolean;
    procedure ReadProperties(Instance: TPersistent);
    procedure ReadProperty(Instance: TPersistent;
                           PropInfo: PPropInfo);
    function ReadID: string;
    function ReadInteger: longint;
    function ReadChar: char;
    function ReadFloat: extended;
    function ReadString: string;

    property AddComponents: boolean read FAddComponents write FAddComponents;
    property CheckNames: boolean read FCheckNames write FCheckNames;
    property CurrentParent: TRaveComponent read FCurrentParent write FCurrentParent;
    property CurrentComp: TRaveComponent read FCurrentComp write FCurrentComp;
    property MasterProject: TRaveProjectItem read FMasterProject write FMasterProject;
  end; { TRaveReader }

  TRaveFont = class(TFont)
  published
    property Height stored false;
    property Size stored true;
  end; { TRaveFont }

  ERaveCompiler = class(Exception);

  TRaveModule = class
  protected
    FModuleName: string;
    procedure PostLoad; virtual; abstract;
  public
    constructor CreateFromStream(Stream: TStream; AOwner: TRaveProjectItem; AProjectItem: TRaveProjectItem); virtual; abstract;
    constructor CreateFromFile(FileName: string; AOwner: TRaveProjectItem); virtual; abstract;
    procedure SaveToStream(Stream: TStream); virtual; abstract;
    procedure Execute(Position: integer; var SuspendState: TObject); virtual; abstract;
    procedure SetStringVar(Name: string; Value: string); virtual; abstract;
    function GetStringVar(Name: string): string; virtual; abstract;
    procedure SetIntVar(Name: string; Value: integer); virtual; abstract;
    function GetIntVar(Name: string): integer; virtual; abstract;
    procedure SetFloatVar(Name: string; Value: extended); virtual; abstract;
    function GetFloatVar(Name: string): extended; virtual; abstract;
    procedure PushParam(var Param); virtual; abstract;
    procedure Changing(OldItem: TRaveModule; NewItem: TRaveModule); virtual; abstract;
    procedure BuildSource; virtual; abstract;
    procedure Compile(Definition: boolean); virtual; abstract;
    procedure EventConnect; virtual; abstract;
    //
    property ModuleName: string read FModuleName write FModuleName;
  end; { TRaveModule }

  TRaveEvent = class(TPersistent)
  protected
    FHeader: string;
    FBody: string;
    FPosition: integer;
    FModuleOwner: TRaveProjectItem;
    FHeaderSourceLine: integer;
    FBodySourceLine: integer;
    FOwner: TRaveComponent;
    FName: string;
    FSuspendState: TObject;

    procedure AssignTo(Dest: TPersistent); override;
    procedure DefineParam(var Param);
    function GetDeclaration: string; virtual;
    function GetFooter: string; virtual;
    function SuspendValid: boolean; virtual;
  public
    procedure Execute;
    procedure DefineParams; virtual;
    function ParamSource: string; virtual;
    function Suspended: boolean;

    property Owner: TRaveComponent read FOwner write FOwner;
    property HeaderSourceLine: integer read FHeaderSourceLine write FHeaderSourceLine;
    property BodySourceLine: integer read FBodySourceLine write FBodySourceLine;
    property Declaration: string read GetDeclaration;
    property Footer: string read GetFooter;
  published
    property Body: string read FBody write FBody;
    property Header: string read FHeader write FHeader;
    property ModuleOwner: TRaveProjectItem read FModuleOwner write FModuleOwner;
    property Position: integer read FPosition write FPosition;
    property Name: string read FName write FName;
  end; { TRaveEvent }

  TRaveSimpleEvent = class(TRaveEvent)
  published
    property Sender: TRaveComponent read FOwner write FOwner;
  end; { TRaveSimpleEvent }

  TRaveStringEvent = class(TRaveSimpleEvent)
  protected
    FValue: string;
  public
    procedure DefineParams; override;
    function ParamSource: string; override;
  published
    property Value: string read FValue write FValue;
  end; { TRaveStringEvent }

  TRaveFloatEvent = class(TRaveSimpleEvent)
  protected
    FValue: extended;
  public
    procedure DefineParams; override;
    function ParamSource: string; override;
  published
    property Value: extended read FValue write FValue;
  end; { TRaveFloatEvent }

  TRaveCompileStatus = class
  public
    ErrorLine: longint;
    ErrorCol: longint;
    ErrorEvent: TRaveEvent;
    ErrorMsg: string;
  end; { TRaveCompileStatus }

  TIncludeFunc = function(Value: TComponent;
                          Data: longint): boolean of object;

  procedure RegisterProc(AKind: string;
                         AProc: TProcedure);
  procedure CallRegisters(AKind: string);
  procedure GetComponentNames(IncludeFunc: TIncludeFunc;
                              Instance: TRaveComponent;
                              Data: longint;
                              NameList: TStrings);
  procedure AddComponent(Component: TRaveComponent);
  function CreateRaveModule(Stream: TStream;
                            Owner: TRaveProjectItem;
                            ProjectItem: TRaveProjectItem): TRaveModule;
  procedure RegisterRaveModule(Module: TRaveModule);
  procedure DeleteComponent(Component: TRaveComponent);
  procedure RegisterRaveProperties(const ComponentClass: TRaveComponentClass;
                                   const Level1Props: string;
                                   const Level2Props: string;
                                   const DevProps: string;
                                   const HideProps: string);
  procedure SetPropDesc(const CompClass: TRaveComponentClass;
                        const PropName: string;
                        const PropDesc: string);
  procedure RegisterRaveGroup(const GroupName: string;
                              const Description: string);
  procedure RegisterRaveComponents(const GroupName: string;
                                   const RaveClasses: array of TRaveComponentClass); overload;
  procedure RegisterRaveComponents(const GroupName: string;
                                   const Description: string;
                                   const ModuleName: string;
                                   const RaveClasses: array of TRaveComponentClass); overload;
  procedure RegisterRaveModuleClasses(const ModuleName: string;
                                      const RaveClasses: array of TRaveComponentClass);
  function FindRaveClassModule(const ClassName: string): string;
  function GetSupportedClass(RaveClass: TClass): string;
  procedure RegisterRaveClasses(const RaveClasses: array of TRaveComponentClass);
  procedure RegisterRaveFunc(const Func;
                             const FuncName: string;
                             const ModuleName: string);
  procedure EnumRaveCompilers(SyntaxNameList: TStrings);
  procedure RegisterFunc(const Proc;
                         const FuncName: string;
                         const ModuleName: string);
  procedure UnregisterFunc(FuncName: string;
                           ModuleName: string);
  procedure RegisterRaveDataObject(const DataObjectName: string;
                                   const DataObjectClass: TRaveDataObjectClass);
  procedure NotifyChanging(OldItem: TRaveComponent;
                           NewItem: TRaveComponent);
  procedure RaveCreateShortCut(Desc: string;
                               Item: TComponent = nil;
                               Name: string = '';
                               Key1: TShortCut = 0;
                               Key2: TShortCut = 0);
  procedure RaveCompileStatus(CompileStatus: TRaveCompileStatus);
  procedure RaveRegister;

(*****************************************************************************}
( These variables are used internally by RAVE and should not be modified
(*****************************************************************************)
(**)var RegisterRaveGroupProc:procedure(const GroupName:string;const
(**)Description:string)=nil;RegisterRaveComponentsProc:procedure(const
(**)GroupName:string;const RaveClasses:array of TRaveComponentClass)=nil;
(**)RegisterRaveClassesProc:procedure(const RaveClasses:array of
(**)TRaveComponentClass)=nil;RegisterRaveFuncProc:procedure(const Func;const
(**)FuncName:string;const ModuleName:string)=nil;RaveNameChangeProc:procedure(
(**)Component:TRaveComponent;NewName:TComponentName)=nil;AddComponentProc:
(**)procedure(Component:TRaveComponent)=nil;DeleteComponentProc:procedure(
(**)Component:TRaveComponent)=nil;OpenProjectItemProc:procedure(Item:
(**)TRaveProjectItem;Open:boolean)=nil;RegisterRavePropertiesProc:procedure(
(**)const ComponentClass:TRaveComponentClass;const Level1Props:string;const
(**)Level2Props:string;const DevProps:string;const HideProps:string)=nil;
(**)GetComponentNamesProc:procedure(IncludeFunc:TIncludeFunc;Instance:
(**)TRaveComponent;Data:longint;NameList:TStrings)=nil;ModifiedProc:procedure
(**)(Refresh:boolean)=nil;RegisterFuncProc:procedure(const Proc;const FuncName:string;
(**)const ModuleName:string)=nil;UnregisterFuncProc:procedure(FuncName:string;
(**)ModuleName:string)=nil;EnumRaveCompilersProc:procedure(SyntaxNameList:
(**)TStrings)=nil;ContinuationComponent:TRaveComponent=nil;SetPropDescProc:
(**)procedure(const CompClass:TRaveComponentClass;const PropName:string;const
(**)PropDesc:string)=nil;GetPropDescProc:function(CompClass:TClass;PropName: string):
(**)string=nil;CreateRaveModuleProc:function(Stream:TStream;Owner:TRaveProjectItem;
(**)ProjectItem:TRaveProjectItem):TRaveModule=nil;PrepareRaveModuleProc:
(**)procedure(ProjectItem:TRaveProjectItem)=nil;RegisterRaveModuleClassesProc:
(**)procedure(const ModuleName:string;const RaveClasses:array of
(**)TRaveComponentClass)=nil;FindRaveClassModuleProc:function(ClassName:string):
(**)string=nil;DefineCompiles:boolean;RegisterRaveDataObjectProc:procedure(const
(**)DataObjectName:string;const DataObjectClass:TRaveDataObjectClass)=nil;
(**)CreateShortCutProc:procedure(Desc:string;Item:TComponent;
(**)Name:string;Key1:TShortCut;Key2:TShortCut);LoadModulesProc:procedure(
(**)StreamHelper:TStreamHelper);SaveModulesProc:procedure(StreamHelper:
(**)TStreamHelper);RegisterRaveModuleProc:procedure(Module:TRaveModule)=nil;
(**)CompileStatusProc:procedure(CompileStatus:TRaveCompileStatus)=nil;
(*****************************************************************************}
( These variables are used internally by RAVE and should not be modified
(*****************************************************************************)

var
  CurrentDesigner: TRaveDesigner = nil;
  CompileModule: TRaveModule = nil;
  ErrorLine: longint;
  ErrorCol: longint;
  ErrorEvent: TRaveEvent;

implementation

uses
  QDialogs,
  QRvProj;

procedure RaveRegister;
begin { RaveRegister }
  RegisterRaveComponents('',[TRavePage]);
  RegisterRaveModuleClasses({Trans-}'RvClass',[TRaveComponent,TRaveControl,
   TRaveContainerControl,TRaveContainerComponent,TRaveProjectItem,TRavePage]);
end;  { RaveRegister }

var
  RegisterProcList: TStringList = nil;

procedure RegisterProc(AKind: string; AProc: TProcedure);
begin { RegisterProc }
  if not Assigned(RegisterProcList) then begin
    RegisterProcList := TStringList.Create;
  end; { if }
  RegisterProcList.AddObject(AKind,TObject(@AProc));
end;  { RegisterProc }

procedure CallRegisters(AKind: string);
var
  I1: integer;
begin { CallRegisters }
  if Assigned(RegisterProcList) then begin
    for I1 := 0 to RegisterProcList.Count - 1 do begin
      if (AKind = '*') or (AnsiCompareText(AKind,RegisterProcList[I1]) = 0) then begin
        TProcedure(RegisterProcList.Objects[I1]);
      end; { if }
    end; { for }
  end; { if }
end;  { CallRegisters }

procedure RegisterRaveProperties(const ComponentClass: TRaveComponentClass;
                                 const Level1Props: string;
                                 const Level2Props: string;
                                 const DevProps: string;
                                 const HideProps: string);
begin { RegisterRaveProperties }
  if Assigned(RegisterRavePropertiesProc) then begin
    RegisterRavePropertiesProc(ComponentClass,Level1Props,Level2Props,
     DevProps,HideProps);
  end; { else }
end;  { RegisterRaveProperties }

procedure SetPropDesc(const CompClass: TRaveComponentClass;
                      const PropName: string;
                      const PropDesc: string);
begin { SetPropDesc }
  if Assigned(SetPropDescProc) then begin
    SetPropDescProc(CompClass,PropName,PropDesc);
  end; { else }
end;  { SetPropDesc }

procedure RegisterRaveGroup(const GroupName: string;
                            const Description: string);
begin { RegisterRaveGroup }
  if Assigned(RegisterRaveGroupProc) then begin
    RegisterRaveGroupProc(GroupName,Description);
  end; { else }
end;  { RegisterRaveGroup }

procedure RegisterRaveComponents(const GroupName: string;
                                 const RaveClasses: array of TRaveComponentClass);
var
  I1: integer;
begin { RegisterRaveComponents }
  if Assigned(RegisterRaveComponentsProc) then begin
    RegisterRaveComponentsProc(GroupName,RaveClasses);
  end else begin
    for I1 := 0 to High(RaveClasses) do begin
      RegisterClass(RaveClasses[I1]);
    end; { for }
  end; { else }
end;  { RegisterRaveComponents }

procedure RegisterRaveComponents(const GroupName: string;
                                 const Description: string;
                                 const ModuleName: string;
                                 const RaveClasses: array of TRaveComponentClass);
begin { RegisterRaveComponents }
  RegisterRaveGroup(GroupName,Description);
  RegisterRaveComponents(GroupName,RaveClasses);
  RegisterRaveModuleClasses(ModuleName,RaveClasses);
end;  { RegisterRaveComponents }

procedure RegisterRaveClasses(const RaveClasses: array of TRaveComponentClass);
begin { RegisterRaveClasses }
  if Assigned(RegisterRaveClassesProc) then begin
    RegisterRaveClassesProc(RaveClasses);
  end; { else }
end;  { RegisterRaveClasses }

procedure RegisterRaveModuleClasses(const ModuleName: string;
                                    const RaveClasses: array of TRaveComponentClass);
begin { RegisterRaveModuleClasses }
  if Assigned(RegisterRaveModuleClassesProc) then begin
    RegisterRaveModuleClassesProc(ModuleName,RaveClasses);
  end; { else }
end;  { RegisterRaveModuleClasses }

function FindRaveClassModule(const ClassName: string): string;
begin { FindRaveClassModule }
  if Assigned(FindRaveClassModuleProc) then begin
    Result := FindRaveClassModuleProc(ClassName);
  end else begin
    Result := '';
  end; { else }
end;  { FindRaveClassModule }

function GetSupportedClass(RaveClass: TClass): string;
begin { GetSupportedClass }
  Result := FindRaveClassModule(RaveClass.ClassName);
  if Result = '' then begin
    Result := GetSupportedClass(RaveClass.ClassParent);
  end else begin
    Result := RaveClass.ClassName;
  end; { else }
end;  { GetSupportedClass }

procedure RegisterRaveFunc(const Func;
                           const FuncName: string;
                           const ModuleName: string);
begin { RegisterRaveFunc }
  if Assigned(RegisterRaveFuncProc) then begin
    RegisterRaveFuncProc(Func,FuncName,ModuleName);
  end; { else }
end;  { RegisterRaveFunc }

procedure AddComponent(Component: TRaveComponent);
begin { AddComponent }
  if HoldAddDeleteComponents then Exit;
  if Assigned(AddComponentProc) then begin
    AddComponentProc(Component);
  end; { else }
end;  { AddComponent }

function CreateRaveModule(Stream: TStream;
                          Owner: TRaveProjectItem;
                          ProjectItem: TRaveProjectItem): TRaveModule;
begin { CreateRaveModule }
  if Assigned(CreateRaveModuleProc) then begin
    Result := CreateRaveModuleProc(Stream,Owner,ProjectItem);
  end else begin
    Result := nil;
  end; { else }
end;  { CreateRaveModule }

procedure RegisterRaveModule(Module: TRaveModule);
begin { RegisterRaveModule }
  if Assigned(RegisterRaveModuleProc) then begin
    RegisterRaveModuleProc(Module);
  end; { else }
end;  { RegisterRaveModule }

procedure DeleteComponent(Component: TRaveComponent);
begin { DeleteComponent }
  if HoldAddDeleteComponents then Exit;
  if Assigned(DeleteComponentProc) then begin
    DeleteComponentProc(Component);
  end; { else }
end;  { DeleteComponent }

procedure GetComponentNames(IncludeFunc: TIncludeFunc;
                            Instance: TRaveComponent;
                            Data: longint;
                            NameList: TStrings);
begin { GetComponentNames }
  if Assigned(GetComponentNamesProc) then begin
    GetComponentNamesProc(IncludeFunc,Instance,Data,NameList);
  end; { else }
end;  { GetComponentNames }

procedure OpenProjectItem(Item: TRaveProjectItem; Open: boolean);
begin { OpenProjectItem }
  if Assigned(OpenProjectItemProc) then begin
    OpenProjectItemProc(Item,Open);
  end else begin
    if Item is TRavePage then begin
      TRavePage(Item).Active := Open;
    end; { if }
  end; { else }
end;  { OpenProjectItem }

procedure NotifyChanging(OldItem: TRaveComponent;
                         NewItem: TRaveComponent);
begin { NotifyChanging }
  if Assigned(ProjectManager) then begin
    ProjectManager.Changing(OldItem,NewItem);
  end; { if }
end;  { NotifyChanging }

procedure EnumRaveCompilers(SyntaxNameList: TStrings);
begin { EnumRaveCompilers }
  if Assigned(EnumRaveCompilersProc) then begin
    EnumRaveCompilersProc(SyntaxNameList);
  end; { if }
end;  { EnumRaveCompilers }

procedure RegisterFunc(const Proc;
                       const FuncName: string;
                       const ModuleName: string);
begin { RegisterFunc }
  if Assigned(RegisterFuncProc) then begin
    RegisterFuncProc(Proc,FuncName,ModuleName);
  end; { if }
end;  { RegisterFunc }

procedure UnregisterFunc(FuncName, ModuleName: string);
begin { UnregisterFunc }
  if Assigned(UnregisterFuncProc) then begin
    UnregisterFuncProc(FuncName,ModuleName);
  end; { if }
end;  { UnregisterFunc }

procedure RegisterRaveDataObject(const DataObjectName: string;
                                 const DataObjectClass: TRaveDataObjectClass);
begin { RegisterRaveDataObject }
  if Assigned(RegisterRaveDataObjectProc) then begin
    RegisterRaveDataObjectProc(DataObjectName,DataObjectClass);
  end; { if }
end;  { RegisterRaveDataObject }

procedure RavePrepareModule(ProjectItem: TRaveProjectItem);
begin { RavePrepareModule }
  if Assigned(PrepareRaveModuleProc) then begin
    PrepareRaveModuleProc(ProjectItem);
  end; { if }
end;  { RavePrepareModule }

procedure RaveCreateShortCut(Desc: string;
                             Item: TComponent = nil;
                             Name: string = '';
                             Key1: TShortCut = 0;
                             Key2: TShortCut = 0);
begin { RaveCreateShortCut }
  if Assigned(CreateShortCutProc) then begin
    CreateShortCutProc(Desc,Item,Name,Key1,Key2);
  end; { if }
end;  { RaveCreateShortCut }

procedure RaveCompileStatus(CompileStatus: TRaveCompileStatus);
begin { RaveCompileStatus }
  if Assigned(CompileStatusProc) then begin
    CompileStatusProc(CompileStatus);
  end; { if }
end;  { RaveCompileStatus }

{ TRavePersistentList }

procedure TRavePersistentList.Assign(Source: TRavePersistentList);
var
  I1: integer;
  Item1: TPersistent;
  Item2: TPersistent;
begin { Assign }
  ClearObjectList(self);
  for I1 := 0 to Source.Count - 1 do begin
    Item1 := TPersistent(Source[I1]);
    if Item1 is TComponent then begin
      Item2 := CreateItem(Item1.ClassName,TComponent(Item1).Owner);
    end else begin
      Item2 := CreateItem(Item1.ClassName,nil);
    end; { else }
    Item2.Assign(Item1);
    Add(Item2);
  end; { for }
end;  { Assign }

function TRavePersistentList.CreateItem(ItemClass: string; RootOwner: TComponent): TPersistent;
var
  TempClass: TClass;
begin { CreateItem }
  TempClass := FindClass(ItemClass);
  if Assigned(TempClass) then begin
    if TempClass.InheritsFrom(TComponent) then begin
      Result := TComponentClass(TempClass).Create(RootOwner);
    end else if TempClass.InheritsFrom(TPersistent) then begin
      Result := TPersistentClass(TempClass).Create;
    end else begin
      Result := nil;
    end; { else }
  end else begin
    Result := nil;
  end; { else }
end;  { CreateItem }

{ TRaveMethodList }

destructor TRaveMethodList.Destroy;
var
  I1: integer;
begin { Destroy }
  for I1 := 0 to (Count - 1) do begin
    FreeMem(Items[I1],SizeOf(TMethod));
  end; { for }
  inherited Destroy;
end;  { Destroy }

procedure TRaveMethodList.AddMethod(Method: TMethod);
var
  P1: ^TMethod;
begin { AddMethod }
  GetMem(P1,SizeOf(TMethod));
  P1^ := Method;
  Add(P1);
end;  { AddMethod }

procedure TRaveMethodList.RemoveMethod(Method: TMethod);
var
  I1: integer;
  P1: pointer;
begin { RemoveMethod }
  for I1 := (Count - 1) downto 0 do begin
    P1 := Items[I1];
    if (Method.Code = TMethod(P1^).Code) and (Method.Data = TMethod(P1^).Data) then begin
      Remove(P1);
      FreeMem(P1,SizeOf(TMethod));
      Break;
    end; { if }
  end; { for }
end;  { RemoveMethod }

procedure TRaveMethodList.RemoveObject(Obj: TObject);
var
  I1: integer;
  P1: pointer;
begin { RemoveObject }
  for I1 := (Count - 1) downto 0 do begin
    P1 := Items[I1];
    if pointer(Obj) = TMethod(P1^).Data then begin
      Remove(P1);
      FreeMem(P1,SizeOf(TMethod));
    end; { if }
  end; { for }
end;  { RemoveObject }

procedure TRaveMethodList.ReplaceObject(OldObj, NewObj: TObject);
var
  I1: integer;
  P1: pointer;
begin { ReplaceObject }
  for I1 := 0 to (Count - 1) do begin
    P1 := Items[I1];
    if pointer(OldObj) = TMethod(P1^).Data then begin
      TMethod(P1^).Data := pointer(NewObj);
    end; { if }
  end; { for }
end;  { RemoveObject }

{ class TRaveComponent }

constructor TRaveComponent.Create(AOwner: TComponent);

  function FindOwner(AOwner: TComponent;
                     AOwnerClass: TClass): TComponent;
  begin { TRaveComponent }
    if AOwner is AOwnerClass then begin
      Result := AOwner;
    end else begin
      if Assigned(AOwner.Owner) then begin
        Result := FindOwner(AOwner.Owner,AOwnerClass);
      end else begin
        Result := nil;
      end; { else }
    end; { else }
  end;  { TRaveComponent }

begin { Create }
  inherited Create(AOwner);
  if AOwner is TRavePage then begin
    FPage := TRavePage(AOwner);
  end else begin
    FPage := nil;
  end; { else }

  // Set project and report props
  if Self is TRaveProjectManager then begin
    FProject := TRaveProjectManager(Self);
  end else if Self is TRaveReport then begin
    FProject := FindOwner(AOwner, TRaveProjectManager) as TRaveProjectItem;
    FReport := TRaveReport(Self);
  end else if Assigned(AOwner) then begin
    FProject := FindOwner(AOwner, TRaveProjectManager) as TRaveProjectItem;
    FReport := FindOwner(AOwner, TRaveReport) as TRaveProjectItem;
  end; { if }

  FVisible := true;
  if InDesigner then begin
    NotifyChanging(nil,self);
  end; { if }
end;  { Create }

destructor TRaveComponent.Destroy;
begin { Destroy }
  DestroyEvents;
  if InDesigner then begin
    NotifyChanging(self,nil);
  end; { if }
  DeleteComponent(self);
  DestroyChildren;
  Parent := nil;
  inherited Destroy;
end; { Destroy }

procedure TRaveComponent.DestroyEvents;
var
  Count: integer;
  PropList: PPropList;
  PropInfo: PPropInfo;
  TypeData: PTypeData;
  I1: integer;
  O1: TObject;
begin { DestroyEvents }
// Free all event property types automatically
  Count := GetTypeData(ClassInfo)^.PropCount;
  if Count > 0 then begin
    GetMem(PropList,Count * SizeOf(pointer));
    try
      GetPropInfos(ClassInfo,PropList);
      for I1 := 0 to (Count - 1) do begin
        PropInfo := PropList^[I1];
        if PropInfo.PropType^.Kind = tkClass then begin
          O1 := TObject(GetOrdProp(self,PropInfo));
          if Assigned(O1) then begin
            TypeData := GetTypeData(PropInfo.PropType^);
            if (TypeData.ClassType.InheritsFrom(TRaveEvent)) then begin
              O1.Free;
            end; { if }
          end; { if }
        end; { if }
      end; { for }
    finally
      FreeMem(PropList,Count * SizeOf(pointer));
    end; { tryf }
  end; { if }
end;  { DestroyEvents }

procedure TRaveComponent.Invalidate;
begin { Invalidate }
  if Assigned(Page) and Assigned(Page.Designer) and (Page.Designer = CurrentDesigner) then begin
    CurrentDesigner.Invalidate;
  end; { if }
end;  { Invalidate }

procedure TRaveComponent.DestroyChildren;
begin { DestoryChildren }
  while ChildCount > 0 do begin
    Child[ChildCount - 1].Free;
  end; { while }
end;  { DestroyChildren }

procedure TRaveComponent.PostLoad;
var
  I1: integer;
begin { PostLoad }
  for I1 := 0 to ChildCount - 1 do begin
    Child[I1].PostLoad;
  end; { for }
end;  { PostLoad }

procedure TRaveComponent.Changing(OldItem: TRaveComponent;
                                  NewItem: TRaveComponent);
var
  I1: integer;
begin { Changing }
  for I1 := 0 to ChildCount - 1 do begin
    Child[I1].Changing(OldItem,NewItem);
  end; { for }
end;  { Changing }

procedure TRaveComponent.SetLocked(Value: boolean);
begin { SetLocked }
  if Value <> FLocked then begin
    FLocked := Value;
  end; { if }
end;  { SetLocked }

procedure TRaveComponent.SetDevLocked(Value: boolean);
begin { SetDevLocked }
  if Value <> FDevLocked then begin
    FDevLocked := Value;
  end; { if }
end;  { SetDevLocked }

function TRaveComponent.GetLocker: TRaveComponent;
begin { GetLocker }
  if Assigned(Parent) then begin
    Result := Parent.GetLocker;
  end else begin
    Result := nil;
  end; { else }
  if not Assigned(Result) and (Locked or DevLocked) then begin
    Result := self;
  end; { if }
end;  { GetLocker }

function TRaveComponent.IsLocked: boolean;
begin { IsLocked }
  Result := Locked or DevLocked;
  if not Result and Assigned(Parent) then begin
    Result := Parent.IsLocked;
  end; { if }
end;  { IsLocked }

function TRaveComponent.IsLocker: boolean;
begin { IsLocker }
  Result := Locked or DevLocked;
  if Result and Assigned(Parent) then begin
    Result := not Parent.IsLocked;
  end; { if }
end;  { IsLocker }

procedure TRaveComponent.SaveDesigned;
var
  I1: integer;
begin { SaveDesigned }
  for I1 := 0 to ChildCount - 1 do begin
    Child[I1].SaveDesigned;
  end; { for }
end;  { SaveDesigned }

procedure TRaveComponent.RestoreDesigned;
var
  I1: integer;
begin { RestoreDesigned }
  for I1 := 0 to ChildCount - 1 do begin
    Child[I1].RestoreDesigned;
  end; { for }
end;  { RestoreDesigned }

procedure TRaveComponent.SaveAdjusted;
var
  I1: integer;
begin { SaveAdjusted }
  for I1 := 0 to ChildCount - 1 do begin
    Child[I1].SaveAdjusted;
  end; { for }
end;  { SaveAdjusted }

procedure TRaveComponent.RestoreAdjusted;
var
  I1: integer;
begin { RestoreAdjusted }
  for I1 := 0 to ChildCount - 1 do begin
    Child[I1].RestoreAdjusted;
  end; { for }
end;  { RestoreAdjusted }

function TRaveComponent.CanSelect(Item: TRaveComponent): boolean;
begin { CanSelect }
  if Assigned(Parent) then begin
    Result := Parent.CanSelect(Item);
  end else begin
    Result := true;
  end; { else }
end;  { CanSelect }

procedure TRaveComponent.PrintParams(var HDelta: TRaveUnits;
                                     var VDelta: TRaveUnits);
begin { PrintParams }
end;  { PrintParams }

procedure TRaveComponent.Speak(List: TRaveMethodList;
                               Msg: TObject);
var
  I1: integer;
begin { Speak }
  if not Assigned(List) then Exit;
  for I1 := 0 to (List.Count - 1) do begin
    TRaveListenEvent(List[I1]^)(self,Msg);
  end; { for }
end;  { Speak }

procedure TRaveComponent.AddListener(Conversation: string;
                                     ListenMethod: TRaveListenEvent);
begin { AddListener }
end;  { AddListener }

procedure TRaveComponent.RemoveListener(Conversation: string;
                                        ListenMethod: TRaveListenEvent);
begin { RemoveListener }
end;  { RemoveListener }

function TRaveComponent.Habla(Conversation: string): boolean;
begin { Habla }
  Result := false;
end;  { Habla }

procedure TRaveComponent.OverrideProperties(Filer: TRaveFiler);
begin { OverrideProperties }
{ Call Filer.OverrideProperty() for each property to override read/write }
end;  { OverrideProperties }

function TRaveComponent.GetDonePrinting: boolean;
var
  I1: integer;
begin { GetDonePrinting }
  Result := true;
  for I1 := 0 to ChildCount - 1 do begin
    if not Child[I1].DonePrinting then begin
      Result := false;
      Exit;
    end; { if }
  end; { for }
end;  { GetDonePrinting }

procedure TRaveComponent.BeforeReport;
var
  I1: integer;
begin { BeforeReport }
  if Assigned(FOnBeforeReport) then begin
    FOnBeforeReport.Execute;
  end; { if }
  for I1 := 0 to ChildCount - 1 do begin
    Child[I1].BeforeReport;
  end; { for }
end;  { BeforeReport }

procedure TRaveComponent.AfterReport;
var
  I1: integer;
begin { AfterReport }
  for I1 := 0 to ChildCount - 1 do begin
    Child[I1].AfterReport;
  end; { for }
  if Assigned(FOnAfterReport) then begin
    FOnAfterReport.Execute;
  end; { if }
end;  { AfterReport }

procedure TRaveComponent.BeforePrint;
begin { BeforePrint }
  if Assigned(FOnBeforePrint) then begin
    FOnBeforePrint.Execute;
  end; { if }
end;  { BeforePrint }

procedure TRaveComponent.AfterPrint;
begin { AfterPrint }
  if Assigned(FOnAfterPrint) then begin
    FOnAfterPrint.Execute;
  end; { if }
end;  { AfterPrint }

procedure TRaveComponent.InitData;
var
  I1: integer;
begin { InitData }
  for I1 := 0 to ChildCount - 1 do begin
    Child[I1].InitData;
  end; { for }
end;  { InitData }

procedure TRaveComponent.DoneData;
var
  I1: integer;
begin { DoneData }
  for I1 := 0 to ChildCount - 1 do begin
    Child[I1].DoneData;
  end; { for }
end;  { DoneData }

procedure TRaveComponent.SetName(const NewName: TComponentName);
begin { SetName }
  if InDesigner and (NewName = '') then Exit;
  if Pos('`' + UpperCase(NewName) + '`',{Trans-}'`IMPORT`CONST`TYPE`VAR`FUNCTION' +
   {Trans-}'`IMPLEMENTATION`END`CREATION`DESTRUCTION`') > 0 then Exit;
  inherited SetName(NewName);
  if Assigned(RaveNameChangeProc) then begin
    RaveNameChangeProc(self,NewName);
  end; { if }
  Invalidate;
end;  { SetName }

procedure TRaveComponent.SetParent(Value: TRaveComponent);
begin { SetParent }
  if Assigned(FParent) then begin { Remove parent }
    Parent.RemoveChild(self);
  end; { if }
  FParent := Value;
  if Assigned(FParent) then begin
    Parent.AddChild(self);
  end; { if }
end;  { SetParent }

procedure TRaveComponent.AddChild(Value: TRaveComponent);
begin { AddChild }
  Raise ERaveClass.Create(Trans(Format({Trans+}
   'Class cannot accept children classes: "%s"',[ClassName])));
end;  { AddChild }

procedure TRaveComponent.RemoveChild(Value: TRaveComponent);
begin { RemoveChild }
end;  { RemoveChild }

function TRaveComponent.GetChildCount: integer;
begin { GetChildCount }
  Result := 0;
end;  { GetChildCount }

function TRaveComponent.GetChildIndex: integer;
var
  I1: integer;
begin { GetChildIndex }
  Result := 0;
  for I1 := 0 to Parent.ChildCount - 1 do begin
    if Parent.Child[I1] = self then begin
      Result := I1;
      Exit;
    end; { if }
  end; { for }
end;  { GetChildIndex }

function TRaveComponent.GetChild(Index: integer): TRaveComponent;
begin { GetChild }
  Result := nil;
end;  { GetChild }

procedure TRaveComponent.SetDepth(Control: TRaveComponent;
                                  Adjustment: integer);
begin { SetDepth }
end;  { SetDepth }

procedure TRaveComponent.MoveForward;
begin { MoveForward }
  if Assigned(Parent) then begin
    Parent.SetDepth(self,3);
  end; { if }
end;  { MoveForward }

procedure TRaveComponent.MoveBehind;
begin { MoveBehind }
  if Assigned(Parent) then begin
    Parent.SetDepth(self,2);
  end; { if }
end;  { MoveBehind }

procedure TRaveComponent.BringToFront;
begin { BringToFront }
  if Assigned(Parent) then begin
    Parent.SetDepth(self,4);
  end; { if }
end;  { BringToFront }

procedure TRaveComponent.SendToBack;
begin { SendToBack }
  if Assigned(Parent) then begin
    Parent.SetDepth(self,1);
  end; { if }
end;  { SendToBack }

function TRaveComponent.AllowPrint: boolean;
begin { AllowPrint }
  Result := Visible;
end;  { AllowPrint }

procedure TRaveComponent.PrintAll(Report: TBaseReport);
var
  I1: integer;
begin { PrintAll }
  BeforePrint;
  if not AllowPrint then Exit;
  Print(Report);
  for I1 := 0 to ChildCount - 1 do begin
    Child[I1].PrintAll(Report);
  end; { for }
  AfterPrint;
end;  { PrintAll }

procedure TRaveComponent.Print(Report: TBaseReport);
begin { Print }
end;  { Print }

function TRaveComponent.CreateChild(AClassType: TRaveComponentClass;
                                    ABaseName: string = '';
                                    ALeft: TRaveUnits = 0.0;
                                    ATop: TRaveUnits = 0.0;
                                    AWidth: TRaveUnits = 0.0;
                                    AHeight: TRaveUnits = 0.0): TRaveComponent;
var
  OrigName: string;
begin { CreateChild }
  if self is TRaveProjectItem then begin
    Result := AClassType.Create(self);
  end else begin
    Result := AClassType.Create(Owner);
  end; { else }
  Result.Parent := self;
  OrigName := ABaseName;
  ABaseName := Result.ClassName;
  if ABaseName[1] = 'T' then begin
    Delete(ABaseName,1,1);
  end; { if }
  if Pos({Trans-}'RAVE',UpperCase(ABaseName)) = 1 then begin
    Delete(ABaseName,1,4);
  end; { if }
  if OrigName = '' then begin
    Result.Name := TRaveProjectManager(Project).GetUniqueName(ABaseName,Result.Owner as
     TRaveComponent,false);
  end else begin
    Result.Name := TRaveProjectManager(Project).GetUniqueName(OrigName + '|' + ABaseName,
     Result.Owner as TRaveComponent,true);
  end; { else }
  if Result is TRaveControl then begin
    with TRaveControl(Result) do begin
      Left := ALeft;
      Top := ATop;
      if AWidth <> 0.0 then begin
        Width := AWidth;
      end; { if }
      if AHeight <> 0.0 then begin
        Height := AHeight;
      end; { if }
    end; { with }
  end; { if }
  AddComponent(Result);
end;  { CreateChild }

function TRaveComponent.GetParentComponent: TComponent;
begin { GetParentComponent }
  Result := FParent;
end;  { GetParentComponent }

procedure TRaveComponent.SetParentComponent(Value: TComponent);
begin { SetParentComponent }
  if Value is TRaveComponent then begin
    Parent := (Value as TRaveComponent);
  end; { if }
end;  { SetParentComponent }

procedure TRaveComponent.SetVisible(Value: boolean);
begin { SetVisible }
  if Value <> Visible then begin
    if Value then begin
      Show;
    end else begin
      Hide;
    end; { else }
  end; { if }
end;  { SetVisible }

procedure TRaveComponent.Show;
begin { Show }
  FVisible := true;
end;  { Show }

procedure TRaveComponent.Hide;
begin { Hide }
  FVisible := false;
end;  { Hide }

function TRaveComponent.GetPageLeft: TRaveUnits;
begin { GetPageLeft }
  Result := 0.0;
end;  { GetPageLeft }

function TRaveComponent.GetPageTop: TRaveUnits;
begin { GetPageTop }
  Result := 0.0;
end;  { GetPageTop }

function TRaveComponent.GetPageRight: TRaveUnits;
begin { GetPageRight }
  Result := 0.0;
end;  { GetPageRight }

function TRaveComponent.GetPageBottom: TRaveUnits;
begin { GetPageBottom }
  Result := 0.0;
end;  { GetPageBottom }

function TRaveComponent.AcceptChild(NewChild: TClass): boolean;
begin { AcceptChild }
  Result := rcsContainer in FControlStyle;
end;  { AcceptChild }

class function TRaveComponent.UseMaster: boolean;
begin { UseMaster }
  Result := false;
end;  { UseMaster }

class function TRaveComponent.AcceptParent(NewParent: TClass): boolean;
begin { AcceptParent }
  Result := true;
end;  { AcceptParent }

function TRaveComponent.XU2I(Value: TRaveUnits): TRaveUnits;
begin { XU2I }
  Result := Value / TRaveProjectManager(Project).UnitsFactor;
end;  { XU2I }

function TRaveComponent.YU2I(Value: TRaveUnits): TRaveUnits;
begin { YU2I }
  Result := Value / TRaveProjectManager(Project).UnitsFactor;
end;  { YU2I }

function TRaveComponent.XI2U(Value: TRaveUnits): TRaveUnits;
begin { XI2U }
  Result := Value * TRaveProjectManager(Project).UnitsFactor;
end;  { XI2U }

function TRaveComponent.YI2U(Value: TRaveUnits): TRaveUnits;
begin { YI2U }
  Result := Value * TRaveProjectManager(Project).UnitsFactor;
end;  { YI2U }

{ class TRaveControl }

constructor TRaveControl.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);
  FCursor := crDefault;
end;  { Create }

destructor TRaveControl.Destroy;
begin { Destroy }
  if Assigned(FMirrorList) then begin
    FreeAndNil(FMirrorList);
  end; { if }
  inherited Destroy;
end;  { Destroy }

procedure TRaveControl.Changing(OldItem: TRaveComponent;
                                NewItem: TRaveComponent);
begin { Changing }
  inherited Changing(OldItem,NewItem);
  if Assigned(Mirror) and (Mirror = OldItem) then begin
    Mirror := NewItem as TRaveControl;
  end; { if }
  if Assigned(OldItem) and (OldItem is TRaveControl) and
   (TRaveControl(OldItem).Mirror = self) and not Assigned(NewItem) then begin
    RemoveMirror(TRaveControl(OldItem));
  end; { if }
end;  { Changing }

function TRaveControl.GetDonePrinting: boolean;
begin { GetDonePrinting }
  Result := inherited GetDonePrinting;
  if Result and Assigned(Mirror) then begin
    Result := Mirror.DonePrinting;
  end; { if }
end;  { GetDonePrinting }

procedure TRaveControl.AddMirror(Value: TRaveControl);
begin { AddMirror }
  if not Assigned(FMirrorList) then begin
    FMirrorList := TList.Create;
  end; { if }
  FMirrorList.Add(Value);
  Include(Value.FControlStyle,rcsReflecting);
  AssignTo(Value);
  Exclude(Value.FControlStyle,rcsReflecting);
end;  { AddMirror }

procedure TRaveControl.RemoveMirror(Value: TRaveControl);
begin { RemoveMirror }
  if Assigned(FMirrorList) then begin
    FMirrorList.Remove(Value);
  end; { if }
end;  { RemoveMirror }

procedure TRaveControl.Reflect;
var
  I1: integer;
begin { Reflect }
  if Assigned(FMirrorList) then begin
    for I1 := 0 to FMirrorList.Count - 1 do begin
      Include(TRaveControl(FMirrorList[I1]).FControlStyle,rcsReflecting);
      AssignTo(TRaveControl(FMirrorList[I1]));
      TRaveControl(FMirrorList[I1]).Invalidate;
      Exclude(TRaveControl(FMirrorList[I1]).FControlStyle,rcsReflecting);
    end; { for }
  end; { if }
end;  { Reflect }

procedure TRaveControl.AssignTo(Dest: TPersistent);
var
  Count: integer;
  PropList: PPropList;
  PropInfo: PPropInfo;
  I1: integer;
  Driver: TPersistent;
begin { AssignTo }
  if Dest is self.ClassType then begin { self is driver }
    Driver := self;
  end else if self is Dest.ClassType then begin { Dest is driver }
    Driver := Dest;
  end else begin { Invalid AssignTo }
    inherited AssignTo(Dest);
    Exit;
  end; { else }
  Count := GetTypeData(Driver.ClassInfo)^.PropCount;
  if Count > 0 then begin
    GetMem(PropList,Count * SizeOf(pointer));
    try
      GetPropInfos(Driver.ClassInfo,PropList);
      for I1 := 0 to (Count - 1) do begin
        PropInfo := PropList^[I1];

      {TransOff}
        if (AnsiCompareText(PropInfo.Name,'Name') = 0) or
         (AnsiCompareText(PropInfo.Name,'DevLocked') = 0) or
         (AnsiCompareText(PropInfo.Name,'Locked') = 0) or
         (AnsiCompareText(PropInfo.Name,'Mirror') = 0) or
         (AnsiCompareText(PropInfo.Name,'Top') = 0) or
         (AnsiCompareText(PropInfo.Name,'Left') = 0) then Continue;
      {TransOn}

        if (PropInfo.PropType^.Kind = tkClass) and
         (GetTypeData(PropInfo.PropType^).ClassType.InheritsFrom(TRaveEvent)) then Continue;

        case PropInfo.PropType^.Kind of
          tkInteger,tkChar,tkEnumeration: SetOrdProp(Dest,PropInfo,GetOrdProp(self,PropInfo));
          tkFloat: SetFloatProp(Dest,PropInfo,GetFloatProp(self,PropInfo));
          tkString: SetStrProp(Dest,PropInfo,GetStrProp(self,PropInfo));
          tkSet,tkClass: SetOrdProp(Dest,PropInfo,GetOrdProp(self,PropInfo));
          tkMethod: SetMethodProp(Dest,PropInfo,GetMethodProp(self,PropInfo));
          tkWChar: SetOrdProp(Dest,PropInfo,GetOrdProp(self,PropInfo));
          tkLString,tkWString: SetStrProp(Dest,PropInfo,GetStrProp(self,PropInfo));
          tkVariant: SetVariantProp(Dest,PropInfo,GetVariantProp(self,PropInfo));
        { Types not supported as properties: tkArray,tkRecord,tkInterface }
        end; { case }
      end; { for }
    finally
      FreeMem(PropList,Count * SizeOf(pointer));
    end; { tryf }
  end; { if }
end;  { AssignTo }

procedure TRaveControl.SetMirror(Value: TRaveControl);
begin { SetMirror }
  if (not Assigned(Value) or (Value is self.ClassType) or
   (self is Value.ClassType)) and (Value <> FMirror) then begin
    if Assigned(FMirror) then begin
      FMirror.RemoveMirror(self);
    end; { if }
    FMirror := Value;
    if Assigned(FMirror) then begin
      FMirror.AddMirror(self);
    end; { if }
  end; { if }
end;  { SetMirror }

procedure TRaveControl.SetText(Value: string);
begin { SetText }
end;  { SetText }

function TRaveControl.GetText: string;
begin { GetText }
  Result := '';
end;  { GetText }

procedure TRaveControl.SetFontAttr(Index: TFontAttribute;
                                   Value: TRaveFont);
begin { SetFontAttr }
end;  { SetFontAttr }

function TRaveControl.GetFontAttr(Index: TFontAttribute): TRaveFont;
begin { GetFontAttr }
  Result := nil;
end;  { GetFontAttr }

procedure TRaveControl.SetFontJustify(Value: TPrintJustify);
begin { SetFontJustify }
end;  { SetFontJustify }

function TRaveControl.GetFontJustify: TPrintJustify;
begin { GetFontJustify }
  Result := pjLeft;
end;  { GetFontJustify }

procedure TRaveControl.SetForeColor(Value: TColor);
begin { SetForeColor }
end;  { SetForeColor }

function TRaveControl.GetForeColor: TColor;
begin { GetForeColor }
  Result := clNone;
end;  { GetForeColor }

procedure TRaveControl.SetBackColor(Value: TColor);
begin { SetBackColor }
end;  { SetBackColor }

function TRaveControl.GetBackColor: TColor;
begin { GetBackColor }
  Result := clNone;
end;  { GetBackColor }

procedure TRaveControl.SetFillStyle(Value: TRaveFillStyle);
begin { SetFillStyle }
end;  { SetFillStyle }

function TRaveControl.GetFillStyle: TRaveFillStyle;
begin { GetFillStyle }
  Result := fsNone;
end;  { GetFillStyle }

procedure TRaveControl.SetLineWidth(Value: TRaveFloat);
begin { SetLineWidth }
end;  { SetLineWidth }

function TRaveControl.GetLineWidth: TRaveFloat;
begin { GetLineWidth }
  Result := -1.0;
end;  { GetLineWidth }

procedure TRaveControl.SetLineWidthType(Value: TLineWidthType);
begin { SetLineWidthType }
end;  { SetLineWidthType }

function TRaveControl.GetLineWidthType: TLineWidthType;
begin { GetLineWidthType }
  Result := wtPixels;
end;  { GetLineWidthType }

procedure TRaveControl.SetLineStyle(Value: TPenStyle);
begin { SetPenStyle }
end;  { SetPenStyle }

function TRaveControl.GetLineStyle: TPenStyle;
begin { GetPenStyle }
  Result := psSolid;
end;  { GetPenStyle }

function TRaveControl.GetLeft: TRaveUnits;
begin { GetLeft }
  Result := FLeft;
end;  { GetLeft }

procedure TRaveControl.SetLeft(Value: TRaveUnits);
begin { SetLeft }
  if FNE(Value,FLeft) then begin
    FLeft := Value;
    ResizeAll;
  end; { if }
end;  { SetLeft }

function TRaveControl.GetTop: TRaveUnits;
begin { GetTop }
  Result := FTop;
end;  { GetTop }

procedure TRaveControl.SetTop(Value: TRaveUnits);
begin { SetTop }
  if FNE(Value,FTop) then begin
    FTop := Value;
    ResizeAll;
  end; { if }
end;  { SetTop }

function TRaveControl.GetWidth: TRaveUnits;
begin { GetWidth }
  Result := FWidth;
end;  { GetWidth }

procedure TRaveControl.SetWidth(Value: TRaveUnits);
begin { SetWidth }
  if FNE(Value,FWidth) then begin
    AnchorAdjustAll(true,FWidth,Value,false,0,0);
    if (HAnchor = anhRight) and not (rcsLoading in ControlStyle) then begin
      FLeft := FLeft + (FWidth - Value);
    end; { if }
    FWidth := Value;
    ResizeAll;
  end; { if }
end;  { SetWidth }

function TRaveControl.GetHeight: TRaveUnits;
begin { GetHeight }
  Result := FHeight;
end;  { GetHeight }

procedure TRaveControl.SetHeight(Value: TRaveUnits);
begin { SetHeight }
  if FNE(Value,FHeight) then begin
    AnchorAdjustAll(false,0,0,true,FHeight,Value);
    if VAnchor = anvBottom then begin
      FTop := FTop + (FHeight - Value);
    end; { if }
    FHeight := Value;
    ResizeAll;
  end; { if }
end;  { SetHeight }

function TRaveControl.GetRight: TRaveUnits;
begin { GetRight }
  Result := Left + Width;
end;  { GetRight }

procedure TRaveControl.SetRight(Value: TRaveUnits);
begin { SetRight }
  if FNE(Left,(Value - Width)) then begin
    Left := Value - Width;
    ResizeAll;
  end; { if }
end;  { SetRight }

function TRaveControl.GetBottom: TRaveUnits;
begin { GetBottom }
  Result := Top + Height;
end;  { GetBottom }

procedure TRaveControl.SetBottom(Value: TRaveUnits);
begin { SetBottom }
  if FNE(Top,(Value - Height)) then begin
    Top := Value - Height;
    ResizeAll;
  end; { if }
end;  { SetBottom }

function TRaveControl.GetPageLeft: TRaveUnits;
begin { GetPageLeft }
  Result := Left + FMirrorPoint.X;
  if Assigned(Parent) then begin
    Result := Result + Parent.PageLeft;
  end; { if }
end;  { GetPageLeft }

function TRaveControl.GetPageTop: TRaveUnits;
begin { GetPageTop }
  Result := Top + FMirrorPoint.Y;
  if Assigned(Parent) then begin
    Result := Result + Parent.PageTop;
  end; { if }
end;  { GetPageTop }

function TRaveControl.GetPageRight: TRaveUnits;
begin { GetPageRight }
  Result := PageLeft + Width;
end;  { GetPageRight }

function TRaveControl.GetPageBottom: TRaveUnits;
begin { GetPageBottom }
  Result := PageTop + Height;
end;  { GetPageBottom }

procedure TRaveControl.SetAnchor(Value: TRaveAnchor);
begin { SetAnchor }
  FAnchor := Value;
end;  { SetAnchor }

procedure TRaveControl.InitData;
var
  I1: integer;
begin { InitData }
  inherited InitData;
  if Assigned(Mirror) then begin
    with Mirror do begin
      for I1 := 0 to (ChildCount - 1) do begin
        Child[I1].InitData;
      end; { for }
    end; { with }
  end; { if }
end;  { InitData }

procedure TRaveControl.DoneData;
var
  I1: integer;
begin { DoneData }
  inherited DoneData;
  if Assigned(Mirror) then begin
    with Mirror do begin
      for I1 := 0 to (ChildCount - 1) do begin
        Child[I1].DoneData;
      end; { for }
    end; { with }
  end; { if }
end;  { DoneData }

procedure TRaveControl.SaveDesigned;
begin { SaveDesigned }
  inherited SaveDesigned;
  FDesLeftx := Left;
  FDesTopx := Top;
  FDesWidthx := Width;
  FDesHeightx := Height;
end;  { SaveDesigned }

procedure TRaveControl.RestoreDesigned;
begin { RestoreDesigned }
  if Assigned(Mirror) then begin
    FLeft := FDesLeftx;
    FTop := FDesTopx;
    Mirror.RestoreDesigned;
  end else begin
    FLeft := FDesLeftx;
    FTop := FDesTopx;
    FWidth := FDesWidthx;
    FHeight := FDesHeightx;
  end; { else }
  inherited RestoreDesigned;
end;  { RestoreDesigned }

procedure TRaveControl.SaveAdjusted;
begin { SaveAdjusted }
  inherited SaveAdjusted;
  if Assigned(Mirror) then begin
    Mirror.SaveAdjusted;
  end; { if }
  FAdjLeft := Left;
  FAdjTop := Top;
  FAdjWidth := Width;
  FAdjHeight := Height;
end;  { SaveAdjusted }

procedure TRaveControl.RestoreAdjusted;
begin { RestoreAdjusted }
  if Assigned(Mirror) then begin
    FLeft := FAdjLeft;
    FTop := FAdjTop;
    Mirror.RestoreAdjusted;
  end else begin
    FLeft := FAdjLeft;
    FTop := FAdjTop;
    FWidth := FAdjWidth;
    FHeight := FAdjHeight;
  end; { else }
  inherited RestoreAdjusted;
end;  { RestoreAdjusted }

function TRaveControl.GetVAnchor: TRaveVAnchor;
begin { GetVAnchor }
  Result := TRaveVAnchor(FAnchor div 16);
end;  { GetVAnchor }

function TRaveControl.GetHAnchor: TRaveHAnchor;
begin { GetHAnchor }
  Result := TRaveHAnchor(FAnchor mod 16);
end;  { GetHAnchor }

procedure TRaveControl.CreatePips;
var
  X1: TRaveUnits;
  Y1: TRaveUnits;
  W: TRaveUnits;
  H: TRaveUnits;
begin { CreatePips }
  X1 := PageLeft;
  Y1 := PageTop;
  W := Width;
  H := Height;
  Page.AddPip(0,self,crSizeNWSE,X1,Y1);
  Page.AddPip(1,self,crSizeNS,X1 + W / 2.0,Y1);
  Page.AddPip(2,self,crSizeNESW,X1 + W,Y1);
  Page.AddPip(3,self,crSizeWE,X1,Y1 + H / 2.0);
  Page.AddPip(4,self,crSizeWE,X1 + W,Y1 + H / 2.0);
  Page.AddPip(5,self,crSizeNESW,X1,Y1 + H);
  Page.AddPip(6,self,crSizeNS,X1 + W / 2.0,Y1 + H);
  Page.AddPip(7,self,crSizeNWSE,X1 + W,Y1 + H);
end;  { CreatePips }

procedure TRaveControl.UpdatePips;
var
  X1: TRaveUnits;
  Y1: TRaveUnits;
  W: TRaveUnits;
  H: TRaveUnits;
begin { UpdatePips }
  X1 := PageLeft;
  Y1 := PageTop;
  W := Width;
  H := Height;
  Page.UpdatePip(0,self,X1,Y1);
  Page.UpdatePip(1,self,X1 + W / 2.0,Y1);
  Page.UpdatePip(2,self,X1 + W,Y1);
  Page.UpdatePip(3,self,X1,Y1 + H / 2.0);
  Page.UpdatePip(4,self,X1 + W,Y1 + H / 2.0);
  Page.UpdatePip(5,self,X1,Y1 + H);
  Page.UpdatePip(6,self,X1 + W / 2.0,Y1 + H);
  Page.UpdatePip(7,self,X1 + W,Y1 + H);
end;  { UpdatePips }

{ This method resizes the control based on the current position, X,Y for the
  pip, RavePip.  This is the standard rectangular sizing method that may be
  overridden for components with other shapes (example: TRaveLine).

                         0   1   2
  Default Pip indexing   3       4
                         5   6   7

}

procedure TRaveControl.PipSize(RavePip: TRavePip;
                               X: TRaveUnits;
                               Y: TRaveUnits);
const
  PipPartner: array[1..3,0..7] of byte = ((2,9,0,4,3,7,9,5),(5,6,7,9,9,0,1,2),
   (7,9,5,9,9,2,9,0));
var
  Temp: TRaveUnits;
  SwitchIdx: byte;
  PageRect: TRaveRect;
  FixedPoint: TRavePoint;
  PipPoint: TRavePoint;
begin { PipSize }
  if RavePip.Index in [0,3,5] then begin { Left }
    FixedPoint.X := RavePip.OrigRect.Right;
    PipPoint.X := RavePip.OrigRect.Left + X;
  end else if RavePip.Index in [1,6] then begin { Middle }
    FixedPoint.X := RavePip.OrigRect.Left;
    PipPoint.X := RavePip.OrigRect.Right;
  end else begin { Right }
    FixedPoint.X := RavePip.OrigRect.Left;
    PipPoint.X := RavePip.OrigRect.Right + X;
  end; { else }

  if RavePip.Index in [0..2] then begin { Top }
    FixedPoint.Y := RavePip.OrigRect.Bottom;
    PipPoint.Y := RavePip.OrigRect.Top + Y;
  end else if RavePip.Index in [3..4] then begin { Middle }
    FixedPoint.Y := RavePip.OrigRect.Top;
    PipPoint.Y := RavePip.OrigRect.Bottom;
  end else begin { Bottom }
    FixedPoint.Y := RavePip.OrigRect.Top;
    PipPoint.Y := RavePip.OrigRect.Bottom + Y;
  end; { else }

  ModifyRaveRect(FixedPoint,PipPoint);

  SwitchIdx := 0;
  if RavePip.Index in [0,3,5] then begin { Left }
    PageRect.Left := PipPoint.X;
  end else begin
    PageRect.Left := FixedPoint.X;
  end; { else }
  if RavePip.Index in [1,2,4,6,7] then begin { Right }
    PageRect.Right := PipPoint.X;
  end else begin
    PageRect.Right := FixedPoint.X;
  end; { else }
  if FLT(PageRect.Right,PageRect.Left) then begin
    Temp := PageRect.Right;
    PageRect.Right := PageRect.Left;
    PageRect.Left := Temp;
    Temp := RavePip.OrigRect.Right;
    RavePip.OrigRect.Right := RavePip.OrigRect.Left;
    RavePip.OrigRect.Left := Temp;
    Inc(SwitchIdx,1);
  end; { if }
  if RavePip.Index in [0..2] then begin { Top }
    PageRect.Top := PipPoint.Y;
  end else begin
    PageRect.Top := FixedPoint.Y;
  end; { else }
  if RavePip.Index in [3..7] then begin { Bottom }
    PageRect.Bottom := PipPoint.Y;
  end else begin
    PageRect.Bottom := FixedPoint.Y;
  end; { else }
  if FLT(PageRect.Bottom,PageRect.Top) then begin
    Temp := PageRect.Bottom;
    PageRect.Bottom := PageRect.Top;
    PageRect.Top := Temp;
    Temp := RavePip.OrigRect.Bottom;
    RavePip.OrigRect.Bottom := RavePip.OrigRect.Top;
    RavePip.OrigRect.Top := Temp;
    Inc(SwitchIdx,2);
  end; { if }
  if SwitchIdx > 0 then begin
    SwitchIdx := PipPartner[SwitchIdx,RavePip.Index];
    Page.SwitchPips(RavePip,SwitchIdx);
  end; { if }
  BoundsRect := PageRect;
end;  { PipSize }

function TRaveControl.IsSelectPoint(Point: TRavePoint): boolean;
begin { IsSelectPoint }
  Result := Visible and PtInRaveRect(BoundsRect,PageToClient(Point)) and
   Parent.CanSelect(self);
end;  { IsSelectPoint }

function TRaveControl.InSelectRect(Rect: TRaveRect): boolean;
var
  SharedRect: TRaveRect;
  SelectRect: TRaveRect;
begin { InSelectRect }
  SelectRect.TopLeft := PageToClient(Rect.TopLeft);
  SelectRect.BottomRight := PageToClient(Rect.BottomRight);
  Result := Visible and IntersectRaveRect(SharedRect,SelectRect,BoundsRect) and
   Parent.CanSelect(self);
end;  { InSelectRect }

function TRaveControl.SetClipped(Canvas: TCanvas): HRgn;
begin { SetClipped }
{!!PORT!!}
  Result := 0;
end;  { SetClipped }

procedure TRaveControl.SetUnclipped(Canvas: TCanvas;
                                    SaveRgn: HRgn);
begin { SetUnclipped }
{!!PORT!!}
end;  { SetUnclipped }

procedure TRaveControl.SetPos(ALeft: TRaveUnits;
                              ATop: TRaveUnits);
begin { SetPos }
  if (ALeft <> Left) or (ATop <> Top) then begin
    DisableResize := true;
    try
      Left := ALeft;
      Top := ATop;
    finally
      DisableResize := false;
    end; { tryf }
    ResizeAll;
  end; { if }
end;  { SetPos }

procedure TRaveControl.SetBoundsRect(Rect: TRaveRect);
var
  OldWidth: TRaveUnits;
  OldHeight: TRaveUnits;

begin { SetBoundsRect }
  DisableResize := true;
  try
    Left := Rect.Left;
    Top := Rect.Top;
    OldWidth := Width;
    Width := Rect.Right - Rect.Left;
    OldHeight := Height;
    Height := Rect.Bottom - Rect.Top;
  finally
    DisableResize := false;
  end; { tryf }
  AnchorAdjustAll(FNE(OldWidth,Width),OldWidth,Width,
   FNE(OldHeight,Height),OldHeight,Height);
  ResizeAll;
end;  { SetBoundsRect }

function TRaveControl.GetBoundsRect: TRaveRect;
begin { GetBoundsRect }
  Result.Left := Left;
  Result.Top := Top;
  Result.Right := Right;
  Result.Bottom := Bottom;
end;  { GetBoundsRect }

{ P1 is the fixed point and P2 is the moving point.  Both points are on
opposite ends of the control but may be on any corner.  }

procedure TRaveControl.SetSize(P1: TRavePoint;
                               P2: TRavePoint);
var
  OldWidth: TRaveUnits;
  OldHeight: TRaveUnits;
begin { SetSize }
  ModifyRaveRect(P1,P2);
  DisableResize := true;
  try
    Left := MinF(P1.X,P2.X);
    Top := MinF(P1.Y,P2.Y);
    OldWidth := Width;
    Width := Abs(P1.X - P2.X);
    OldHeight := Height;
    Height := Abs(P1.Y - P2.Y);
  finally
    DisableResize := false;
  end; { tryf }
  AnchorAdjustAll(FNE(OldWidth,Width),OldWidth,Width,
   FNE(OldHeight,Height),OldHeight,Height);
  ResizeAll;
end;  { SetSize }

procedure TRaveControl.AnchorAdjust(WidthChanging: boolean;
                                    OldWidth,NewWidth: TRaveUnits;
                                    HeightChanging: boolean;
                                    OldHeight,NewHeight: TRaveUnits);
begin { AnchorAdjust }
  if Assigned(Mirror) then begin
    Mirror.AnchorAdjust(WidthChanging,OldWidth,NewWidth,HeightChanging,
     OldHeight,NewHeight);
  end else begin
    if WidthChanging then begin
      case HAnchor of
        anhLeft: begin { Left justified }
        { No need to do anything }
        end;
        anhRight: begin { Right justified }
          Left := Left + (NewWidth - OldWidth);
        end;
        anhCenter: begin { Centered }
          Left := Left + (NewWidth - OldWidth) / 2.0;
        end;
        anhStretch: begin { Stretch }
          Width := Width + (NewWidth - OldWidth);
        end;
        anhResize: begin { Resize }
          if FEQ(OldWidth,0.0) then begin
          //!!!
          end else begin
            Left := Left * (NewWidth / OldWidth);
            Width := Width * (NewWidth / OldWidth);
          end; { else }
        end;
        anhSpread: begin { Spread }
          if FEQ(OldWidth,Width) then begin
          //!!! Handle divide by zero
          end else begin
            Left := Left * (NewWidth - Width) / (OldWidth - Width);
          end; { else }
        end;
      end; { case }
    end; { if }
    if HeightChanging then begin
      case VAnchor of
        anvTop: begin { Top justified }
        { No need to do anything }
        end;
        anvBottom: begin { Bottom justified }
          Top := Top + (NewHeight - OldHeight);
        end;
        anvCenter: begin { Centered }
          Top := Top + (NewHeight - OldHeight) / 2.0;
        end;
        anvStretch: begin { Stretch }
          Height := Height + (NewHeight - OldHeight);
        end;
        anvResize: begin { Resize }
          if FEQ(OldHeight,0.0) then begin
          //!!! Handle divide by zero
          end else begin
            Top := Top * (NewHeight / OldHeight);
            Height := Height * (NewHeight / OldHeight);
          end; { else }
        end;
        anvSpread: begin { Spread }
          if FEQ(OldHeight,Height) then begin
          //!!! Handle divide by zero
          end else begin
            Top := Top * (NewHeight - Height) / (OldHeight - Height);
          end; { else }
        end;
      end; { case }
    end; { if }
  end; { else }
end;  { AnchorAdjust }

procedure TRaveControl.AnchorAdjustAll(WidthChanging: boolean;
                                       OldWidth,NewWidth: TRaveUnits;
                                       HeightChanging: boolean;
                                       OldHeight,NewHeight: TRaveUnits);
var
  I1: integer;
begin { AnchorAdjustAll }
  if DisableResize then Exit;

{ Resize all children }
  for I1 := 0 to (ChildCount - 1) do begin
    if Child[I1] is TRaveControl then begin
      TRaveControl(Child[I1]).AnchorAdjust(WidthChanging,OldWidth,NewWidth,
       HeightChanging,OldHeight,NewHeight);
    end; { if }
  end; { for }
end;  { AnchorAdjustAll }

procedure TRaveControl.ResizeAll;
var
  I1: integer;
begin { ResizeAll }
  if DisableResize then Exit;
  Resize;
{ Resize all children }
  for I1 := 0 to (ChildCount - 1) do begin
    if Child[I1] is TRaveControl then begin
      TRaveControl(Child[I1]).ResizeAll;
    end; { if }
  end; { for }
  Reflect;
end;  { ResizeAll }

procedure TRaveControl.PrintMirror(AReport: TBaseReport;
                                   AOrig: TRaveControl;
                                   AControl: TRaveControl);
var
  I1: integer;
begin { PrintMirror }
  if Assigned(AControl.Mirror) then begin
    with AControl.Mirror do begin
      if Assigned(AControl.Mirror.Mirror) then begin
        PrintMirror(AReport,AOrig,AControl.Mirror);
      end; { if }

      if ChildCount > 0 then begin
        FMirrorPoint := RavePoint((AOrig.PageLeft - PageLeft),
         (AOrig.PageTop - PageTop));
        try
          for I1 := 0 to (ChildCount - 1) do begin
            Child[I1].PrintAll(AReport);
          end; { for }
        finally
          FMirrorPoint := RavePoint(0.0,0.0);
        end; { tryf }
      end; { if }
    end; { with }
  end; { if }
end;  { PrintMirror }

function TRaveControl.HeightToPrint: TRaveUnits;
var
  I1: integer;
  Component: TRaveComponent;
  Control: TRaveControl;
  ChildHeight: TRaveUnits;
begin { HeightToPrint }
  if Assigned(Mirror) then begin
    Result := Mirror.HeightToPrint;
  end else begin
    Result := Height;

    for I1 := 0 to ChildCount - 1 do begin
      Component := Child[I1];
    //If not Component.DonePrinting then begin
        if Component is TRaveControl then begin
          Control := TRaveControl(Component);
          ChildHeight := Control.HeightToPrint + (AdjHeight - Control.AdjHeight);
          if ChildHeight > Result then begin
            Result := ChildHeight;
          end; { if }
        end; { if }
    //end; { if }
    end; { for }
  end; { else }
end;  { HeightToPrint }

function TRaveControl.AllowPrint: boolean;
begin { AllowPrint }
  Result := Visible;
  if Result and Assigned(ContinuationComponent) and
   (ContinuationComponent = Parent) then begin
    Result := (VAnchor = anvStretch);
  end; { if }
end;  { AllowPrint }

type
  TBaseReportAccess = class(TBaseReport)
  end; { TBaseReportAccess }

procedure TRaveControl.PrintAll(Report: TBaseReport);
var
  I1: integer;
begin { PrintAll }
  BeforePrint;
  if not AllowPrint then Exit;

  if DisplayOn = doParent then begin // Set to parent value
    if Parent is TRavePage then begin
      DisplayOn := doAll;
    end else begin
      DisplayOn := TRaveControl(Parent).DisplayOn;
    end; { else }
  end; { if }
  case DisplayOn of
    doAll: Report.AllowAll;
    doPreviewOnly: Report.AllowPreviewOnly;
    doPrinterOnly: Report.AllowPrinterOnly;
  end; { case }

  Print(Report);
  PrintMirror(Report,self,self);

{ Print all children }
  for I1 := 0 to (ChildCount - 1) do begin
    Child[I1].PrintAll(Report);
  end; { for }
  AfterPrint;
end;  { PrintAll }

procedure TRaveControl.Resize;
begin { Resize }
  if InDesigner then begin
    if not ProjectManager.Printing then begin
      Invalidate;
      UpdatePips;
    end; { if }
  end; { if }
end;  { Resize }

class procedure TRaveControl.ModifyRaveRect(var P1: TRavePoint;
                                            var P2: TRavePoint);
begin { ModifyRaveRect }
end;  { ModifyRaveRect }

procedure TRaveControl.PaintMirror(Canvas: TCanvas;
                                   Orig: TRaveControl;
                                   Control: TRaveControl);
var
  I1: integer;
  SaveRgn: HRgn;
begin { PaintMirror }
  if Assigned(Control.Mirror) and Assigned(CurrentDesigner) then begin
    with Control.Mirror do begin
      if Assigned(Control.Mirror.Mirror) then begin
        PaintMirror(Canvas,Orig,Control.Mirror);
      end; { if }

      if ChildCount > 0 then begin
        FMirrorPoint := RavePoint((Orig.PageLeft - PageLeft),
         (Orig.PageTop - PageTop));
        try
          SaveRgn := SetClipped(Canvas);
          for I1 := 0 to (ChildCount - 1) do begin
            if Child[I1] is TRaveControl then begin
              TRaveControl(Child[I1]).PaintAll(Canvas);
            end; { if }
          end; { for }
          SetUnclipped(Canvas,SaveRgn);
        finally
          FMirrorPoint := RavePoint(0.0,0.0);
        end; { tryf }
      end; { if }
    end; { with }
  end; { if }
end;  { PaintMirror }

procedure TRaveControl.Paint(Canvas: TCanvas);
begin { Paint }
end;  { Paint }

procedure TRaveControl.PaintAll(Canvas: TCanvas);
var
  I1: integer;
  SaveRgn: HRgn;
begin { PaintAll }
  if not Visible then Exit;

  Paint(Canvas);
  PaintMirror(Canvas,self,self);

{ Paint all children }
  if ChildCount > 0 then begin
    SaveRgn := SetClipped(Canvas);
    for I1 := 0 to (ChildCount - 1) do begin
      if Child[I1] is TRaveControl then begin
        TRaveControl(Child[I1]).PaintAll(Canvas);
      end; { if }
    end; { for }
    SetUnclipped(Canvas,SaveRgn);
  end; { if }
end;  { PaintAll }

function TRaveControl.XI2P(Value: TRaveUnits): longint;
begin { XI2P }
  Result := CurrentDesigner.XI2D(Value) - CurrentDesigner.XI2D(0.0);
end;  { XI2P }

function TRaveControl.YI2P(Value: TRaveUnits): longint;
begin { YI2P }
  Result := CurrentDesigner.YI2D(Value) - CurrentDesigner.YI2D(0.0);
end;  { YI2P }

function TRaveControl.XI2D(Value: TRaveUnits): longint;
begin { XI2D }
  Result := CurrentDesigner.XI2D(Value);
end;  { XI2D }

function TRaveControl.YI2D(Value: TRaveUnits): longint;
begin { YI2D }
  Result := CurrentDesigner.YI2D(Value);
end;  { YI2D }

function TRaveControl.XD2I(Value: longint): TRaveUnits;
begin { XD2I }
  Result := CurrentDesigner.XD2I(Value);
  if Assigned(Parent) then begin
    Result := Result - Parent.PageLeft;
  end; { if }
end;  { XD2I }

function TRaveControl.YD2I(Value: longint): TRaveUnits;
begin { YD2I }
  Result := CurrentDesigner.YD2I(Value);
  if Assigned(Parent) then begin
    Result := Result - Parent.PageTop;
  end; { if }
end;  { YD2I }

class procedure TRaveControl.ModifyRect(var P1: TPoint;
                                        var P2: TPoint;
                                            PipIndex: byte);
begin { ModifyRect }
end;  { ModifyRect }

function TRaveControl.PageToClient(Point: TRavePoint): TRavePoint;
begin { PageToClient }
  Result := Point;
  if Assigned(Parent) then begin
    Result.X := Result.X - Parent.PageLeft;
    Result.Y := Result.Y - Parent.PageTop;
  end; { if }
end;  { PageToClient }

function TRaveControl.ClientToPage(Point: TRavePoint): TRavePoint;
begin { ClientToPage }
  Result := Point;
  if Assigned(Parent) then begin
    Result.X := Result.X + Parent.PageLeft;
    Result.Y := Result.Y + Parent.PageTop;
  end; { if }
end;  { ClientToPage }

function TRaveControl.FindControlAt(var AControl: TRaveControl;
                                        Point: TRavePoint): boolean;
var
  I1: integer;
begin { FindControlAt }
  Result := IsSelectPoint(Point);
  if Result then begin
  { Look for in children }
    for I1 := (ChildCount - 1) downto 0 do begin
      if Child[I1] is TRaveControl then begin
        AControl := TRaveControl(Child[I1]);
        if AControl.FindControlAt(AControl,Point) then Exit;
      end; { if }
    end; { for }
    AControl := self;
  end; { if }
end;  { FindControlAt }

function TRaveControl.FindContainerAt(var Container: TRaveControl;
                                          Point: TRavePoint;
                                          NewChild: TClass): boolean;
var
  I1: integer;
  TestComp: TRaveControl;
begin { FindContainterAt }
  Result := false;
  if IsSelectPoint(Point) and not IsLocked then begin { Point is within control }
  { Look through children for acceptable container }
    for I1 := (ChildCount - 1) downto 0 do begin
      if Child[I1] is TRaveControl then begin
        TestComp := TRaveControl(Child[I1]);
        if TestComp.FindContainerAt(Container,Point,NewChild) then begin
          Result := true;
          Exit;
        end; { if }
      end; { if }
    end; { for }

  { If valid container not found in children, then see if self qualifies }
    if not Result and TRaveComponentClass(NewChild).AcceptParent(self.ClassType) and
     AcceptChild(NewChild) then begin
      Container := self;
      Result := true;
    end; { if }
  end; { if }
end;  { FindContainerAt }

{ class TRaveContainerControl }

constructor TRaveContainerControl.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);
  FControlStyle := [rcsContainer];
end;  { Create }

procedure TRaveContainerControl.DestroyChildren;
begin { DestroyChildren }
  inherited DestroyChildren;
  if Assigned(ChildList) then begin
    FreeAndNil(ChildList);
  end; { if }
end;  { DestroyChildren }

procedure TRaveContainerControl.AddChild(Value: TRaveComponent);
begin { AddChild }
  if not Assigned(ChildList) then begin
    ChildList := TList.Create;
  end; { if }
  ChildList.Add(Value);
end;  { AddChild }

procedure TRaveContainerControl.RemoveChild(Value: TRaveComponent);
begin { RemoveChild }
  if Assigned(ChildList) then begin
    ChildList.Remove(Value);
  end; { if }
end;  { RemoveChild }

function TRaveContainerControl.GetChildCount: integer;
begin { GetChildCount }
  if Assigned(ChildList) then begin
    Result := ChildList.Count;
  end else begin
    Result := 0;
  end; { else }
end;  { GetChildCount }

function TRaveContainerControl.GetChild(Index: integer): TRaveComponent;
begin { GetChild }
  if (ChildList = nil) or (Index < 0) or (Index >= ChildList.Count) then begin
    Result := nil;
  end else begin
    Result := TRaveComponent(ChildList[Index]);
  end; { else }
end;  { GetChild }

procedure TRaveContainerControl.SetDepth(Control: TRaveComponent;
                                         Adjustment: integer);
var
  I1: integer;
  I2: integer;
begin { SetDepth }
  with ChildList do begin
    I1 := IndexOf(Control);
    case Adjustment of
      1: I2 := 0;           { 1: Send to back }
      2: I2 := I1 - 1;      { 2: Move behind }
      3: I2 := I1 + 1;      { 3: Move forward }
      else I2 := Count - 1; { 4: Bring to front }
    end; { case }
    if (I1 >= 0) and (I2 >= 0) and (I2 < Count) and (I1 <> I2) then begin
      Delete(I1);
      Insert(I2,Control);
      if Control is TRaveControl then begin
        Invalidate;
      end; { if }
    end; { if }
  end; { with }
end;  { SetDepth }

{ class TRaveContainerComponent }

constructor TRaveContainerComponent.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);
  FControlStyle := [rcsContainer];
end;  { Create }

procedure TRaveContainerComponent.DestroyChildren;
begin { DestroyChildren }
  inherited DestroyChildren;
  if Assigned(ChildList) then begin
    FreeAndNil(ChildList);
  end; { if }
end;  { DestroyChildren }

procedure TRaveContainerComponent.AddChild(Value: TRaveComponent);
begin { AddChild }
  if not Assigned(ChildList) then begin
    ChildList := TList.Create;
  end; { if }
  ChildList.Add(Value);
end;  { AddChild }

procedure TRaveContainerComponent.RemoveChild(Value: TRaveComponent);
begin { RemoveChild }
  if Assigned(ChildList) then begin
    ChildList.Remove(Value);
  end; { if }
end;  { RemoveChild }

function TRaveContainerComponent.GetChildCount: integer;
begin { GetChildCount }
  if Assigned(ChildList) then begin
    Result := ChildList.Count;
  end else begin
    Result := 0;
  end; { else }
end;  { GetChildCount }

function TRaveContainerComponent.GetChild(Index: integer): TRaveComponent;
begin { GetChild }
  if (ChildList = nil) or (Index < 0) or (Index >= ChildList.Count) then begin
    Result := nil;
  end else begin
    Result := TRaveComponent(ChildList[Index]);
  end; { else }
end;  { GetChild }

procedure TRaveContainerComponent.SetDepth(Control: TRaveComponent;
                                           Adjustment: integer);
var
  I1: integer;
  I2: integer;
begin { SetDepth }
  with ChildList do begin
    I1 := IndexOf(Control);
    case Adjustment of
      1: I2 := 0;           { 1: Send to back }
      2: I2 := I1 - 1;      { 2: Move behind }
      3: I2 := I1 + 1;      { 3: Move forward }
      else I2 := Count - 1; { 4: Bring to front }
    end; { case }
    if (I1 >= 0) and (I2 >= 0) and (I2 < Count) and (I1 <> I2) then begin
      Delete(I1);
      Insert(I2,Control);
      if Control is TRaveControl then begin
        TRaveControl(Control).Invalidate;
      end; { if }
    end; { if }
  end; { with }
end;  { SetDepth }

{ class TRaveProjectItem }

constructor TRaveProjectItem.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);
  FParams := TStringList.Create;
  FPIVars := TStringList.Create;
end;  { Create }

destructor TRaveProjectItem.Destroy;
begin { Destroy }
  FreeAndNil(FModule);
  FreeAndNil(FPIVars);
  FreeAndNil(FParams);
  inherited Destroy;
end;  { Destroy }

procedure TRaveProjectItem.ReadModule(Reader: TReader);
var
  Stream: TMemoryStream;
  L1: longint;
begin { ReadModule }
  if Assigned(Module) then begin
    FreeAndNil(FModule);
  end; { if }
  Stream := TMemoryStream.Create;
  try
    try
      Reader.Read(L1,SizeOf(L1));
      Stream.SetSize(L1);
      Reader.Read(Stream.Memory^,L1);
      Stream.Position := 0;
      Module := CreateRaveModule(Stream,self,self);
    except
      CompileNeeded := true;
    end;  
  finally
    Stream.Free;
  end; { tryf }
end;  { ReadModule }

procedure TRaveProjectItem.WriteModule(Writer: TWriter);
var
  Stream: TMemoryStream;
  L1: longint;
begin { WriteModule }
  Stream := TMemoryStream.Create;
  try
    Module.SaveToStream(Stream);
    L1 := Stream.Size;
    Writer.Write(L1,SizeOf(L1));
    Writer.Write(Stream.Memory^,L1);
  finally
    Stream.Free;
  end; { tryf }
end;  { WriteModule }

procedure TRaveProjectItem.SetModule(AModule: TRaveModule);
begin { SetModule }
  FModule := AModule;
end;  { SetModule }

procedure TRaveProjectItem.DefineProperties(Filer: TFiler);
begin { DefineProperties }
  inherited DefineProperties(Filer);
  Filer.DefineProperty({Trans-}'Module',ReadModule,WriteModule,Assigned(Module));
end;  { DefineProperties }

procedure TRaveProjectItem.SetActive(Value: boolean);
begin { SetActive }
  if Value = FActive then Exit;
  if Value then begin { Load page with controls }
    Open;
  end else begin { Save page to buffer and remove controls }
    Close;
  end; { else }
end;  { SetActive }

procedure TRaveProjectItem.SetName(const NewName: TComponentName);
begin { SetName }
  if FullName = Name then begin
    FullName := NewName;
  end; { if }
  inherited SetName(NewName);
end;  { SetName }

procedure TRaveProjectItem.SetParameters(Value: TStrings);
var
  I1: integer;
  I2: integer;
  S1: string;
begin { SetParameters }
  FParams.Assign(Value);
  for I1 := 0 to FParams.Count - 1 do begin
    S1 := FParams[I1];
    for I2 := 1 to Length(S1) do begin
      if S1[I2] in ['<','>'] then begin
        S1[I2] := '_';
      end; { for }
    end; { for }
    FParams[I1] := S1;
  end; { for }
end;  { SetParameters }

procedure TRaveProjectItem.SetPIVars(Value: TStrings);
begin { SetPIVars }
  FPIVars.Assign(Value);
end;  { SetPIVars }

procedure TRaveProjectItem.Open;
begin { Open }
  if Active then Exit;
  FActive := true;
  OpenProjectItem(self,true);
end;  { Open }

procedure TRaveProjectItem.Close;
begin { Close }
  if not Active then Exit;
  FActive := false;
  OpenProjectItem(self,false);
end;  { Close }

procedure TRaveProjectItem.SetCompileNeeded(Value: boolean);
begin { SetCompileNeeded }
  FCompileNeeded := Value;
  if CompileNeeded and (Project <> self) then begin
    Project.CompileNeeded := true;
  end; { if }
end;  { SetCompileNeeded }

procedure TRaveProjectItem.PrepareModule;
var
  I1: integer;
begin { PrepareModule }
  RavePrepareModule(self);
  for I1 := 0 to ChildCount - 1 do begin
    if Child[I1] is TRaveProjectItem then begin
      TRaveProjectItem(Child[I1]).PrepareModule;
    end; { if }
  end; { for }
end;  { PrepareModule }

function TRaveProjectItem.GetModuleName: string;
begin { GetModuleName }
  if not (Owner is TRaveProjectItem) or (Owner is TRaveProjectManager) then begin
    Result := Name;
  end else begin
    Result := (Owner as TRaveProjectItem).Name + '_' + Name;
  end; { else }
end;  { GetModuleName }

procedure TRaveProjectItem.PostLoad;
begin { PostLoad }
  if Assigned(Module) then begin
    Module.PostLoad;
  end; { if }
  inherited PostLoad;
end;  { PostLoad }

procedure TRaveProjectItem.Changing(OldItem: TRaveComponent;
                                    NewItem: TRaveComponent);
begin { Changing }
  inherited Changing(OldItem,NewItem);
  if Assigned(Module) and Assigned(OldItem) and (OldItem is TRaveProjectItem) and Assigned(NewItem) then begin
      Module.Changing(TRaveProjectItem(OldItem).Module,TRaveProjectItem(NewItem).Module);
  end; { if }
end;  { Changing }

procedure TRaveProjectItem.SaveToStreamHelper(StreamHelper: TStreamHelper);
begin { SaveToStreamHelper }
  with TRaveWriter.Create(StreamHelper) do try
    WriteComponent(self,true);
  finally
    Free;
  end; { with }
end;  { SaveToStreamHelper }

procedure TRaveProjectItem.LoadFromStreamHelper(StreamHelper: TStreamHelper);
begin { LoadFromStreamHelper }
  with TRaveReader.Create(StreamHelper) do try
    MasterProject := TRaveProjectManager(Project).MasterProject;
    AddComponents := true;
    ReadIntoComponent(self);
  finally
    Free;
  end; { with }
end;  { LoadFromStreamHelper }

{ class TRavePage }

constructor TRavePage.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);
  FPage := self;
  FGlobal := AOwner is TRaveProjectManager;
  FPageWidth := QRvDefine.PaperWidth;
  FPageHeight := QRvDefine.PaperHeight;
  FPaperSize := QRvDefine.PaperSize;
  FBin := -1;
  FVisible := true;
  GridLines := QRvDefine.GridLines;
  GridSpacing := QRvDefine.GridSpacing / TRaveProjectManager(Project).UnitsFactor;
  FOrientation := poDefault;
end;  { Create }

destructor TRavePage.Destroy;
begin { Destroy }
  inherited Destroy;
end;  { Destroy }

procedure TRavePage.LoadFromStreamHelper(StreamHelper: TStreamHelper);
begin { LoadFromStreamHelper }
  with TRaveReader.Create(StreamHelper) do try
    AddComponents := Global;
    ReadIntoComponent(self);
  finally
    Free;
  end; { with }
end;  { LoadFromStreamHelper }

procedure TRavePage.PrepareModule;
begin { PrepareModule }
  RavePrepareModule(self);
end;  { PrepareModule }

procedure TRavePage.PrepareSize(AWidth: TRaveUnits; AHeight: TRaveUnits);
var
  OldWidth: TRaveUnits;
  OldHeight: TRaveUnits;
begin { PrepareSize }
  if FNE(PageWidth,AWidth) or FNE(PageHeight,AHeight) then begin
    RestoreDesigned;
    OldWidth := FPageWidth;
    OldHeight := FPageHeight;
    FPageHeight := AHeight;
    FPageWidth := AWidth;
    FPaperSize := DMPAPER_USER;
    AnchorAdjustAll(FNE(OldWidth,FPageWidth),OldWidth,FPageWidth,
     FNE(OldHeight,FPageHeight),OldHeight,FPageHeight);
    Invalidate;
    SaveDesigned;
  end; { if }
end;  { PrepareSize }

procedure TRavePage.Changing(OldItem: TRaveComponent;
                             NewItem: TRaveComponent);
begin { Changing }
  inherited Changing(OldItem,NewItem);
  if Assigned(GotoPage) and (GotoPage = OldItem) then begin
    GotoPage := NewItem as TRavePage;
  end; { if }
end;  { Changing }

procedure TRavePage.PrintAll(Report: TBaseReport);
var
  I1: integer;
  WX1,WY1: TRaveUnits;
  WX2,WY2: TRaveUnits;
begin { PrintAll }
  BeforePrint;
  if not AllowPrint then Exit;

{ Make sure all components fit within waste area }
  RestoreDesigned;
  if WasteFit then begin
    WX1 := Report.LeftWaste + 0.01;
    WY1 := Report.TopWaste + 0.01;
    WX2 := Report.PageWidth - (Report.RightWaste + 0.01);
    WY2 := Report.PageHeight - (Report.BottomWaste + 0.01);
    for I1 := 0 to ChildCount - 1 do begin
      if Child[I1] is TRaveControl then begin
        with TRaveControl(Child[I1]) do begin
          if FLT(Left,WX1) or FLT(Top,WY1) or FGT(Right,WX2) or FGT(Bottom,WY2) then begin
            SetBoundsRect(RaveRect(MaxF(Left,WX1),MaxF(Top,WY1),
             MinF(Right,WX2),MinF(Bottom,WY2)));
          end; { if }
        end; { with }
      end; { if }
    end; { for }
  end; { if }
  SaveAdjusted;

  for I1 := 0 to (ChildCount - 1) do begin
    Report.AllowAll;
    Child[I1].PrintAll(Report);
  end; { for }
  AfterPrint;
end;  { PrintAll }

procedure TRavePage.Resize;
var
  I1: integer;
begin { Resize }
  if ChildCount > 0 then begin
    for I1 := 0 to (ChildCount - 1) do begin
      if Child[I1] is TRaveControl then begin
        TRaveControl(Child[I1]).ResizeAll;
      end; { if }
    end; { for }
  end; { if }
end;  { Resize }

function TRavePage.StorePageDim: boolean;
begin { StorePageDim }
  Result := (FPaperSize = DMPAPER_USER) or (FPaperSize = -1);
end;  { StorePageDim }

procedure TRavePage.AssignTo(Dest: TPersistent);
var
  P1: TRavePage;
  S1: string;
begin { AssignTo }
  if Dest is TRavePage then begin
    P1 := TRavePage(Dest);
    S1 := Name;
    Name := '';
    P1.Name := S1;
    P1.Tag := Tag;
    P1.PageWidth := PageWidth;
    P1.PageHeight := PageHeight;
  end else begin
    inherited AssignTo(Dest);
  end; { else }
end;  { AssignTo }

procedure TRavePage.Paint(Canvas: TCanvas);
var
  I1: integer;
begin { Paint }
  if not Visible or ProjectManager.Printing then Exit;
  if ChildCount > 0 then begin
    for I1 := 0 to (ChildCount - 1) do begin
      if Child[I1] is TRaveControl then begin
        TRaveControl(Child[I1]).PaintAll(Canvas);
      end; { if }
    end; { for }
  end; { if }
end;  { Paint }

procedure TRavePage.AddPip(Index: byte;
                           Control: TRaveControl;
                           Cursor: TCursor;
                           X: TRaveUnits;
                           Y: TRaveUnits);
begin { AddPip }
  if Assigned(Designer) then begin
    Designer.AddPip(Index,Control,Cursor,X,Y);
  end; { if }
end;  { AddPip }

procedure TRavePage.UpdatePip(Index: byte;
                              Control: TRaveControl;
                              X: TRaveUnits;
                              Y: TRaveUnits);
begin { UpdatePip }
  if Assigned(Designer) then begin
    Designer.UpdatePip(Index,Control,X,Y);
  end; { if }
end;  { UpdatePip }

procedure TRavePage.SwitchPips(RavePip: TRavePip;
                               SwitchIdx: byte);
begin { SwitchPips }
  if Assigned(Designer) then begin
    Designer.SwitchPips(RavePip,SwitchIdx);
  end; { if }
end;  { SwitchPips }

procedure TRavePage.SetGridLines(Value: integer);
begin { SetGridLines }
  if FGridLines <> Value then begin
    FGridLines := Value;
    Invalidate;
  end; { if }
end;  { SetGridLines }

procedure TRavePage.SetGridSpacing(Value: TRaveUnits);
begin { SetGridSpacing }
  if FGridSpacing <> Value then begin
    FGridSpacing := Value;
    Invalidate;
  end; { if }
end;  { SetGridSpacing }

procedure TRavePage.AnchorAdjustAll(WidthChanging: boolean;
                                    OldWidth,NewWidth: TRaveUnits;
                                    HeightChanging: boolean;
                                    OldHeight,NewHeight: TRaveUnits);
var
  I1: integer;
begin { AnchorAdjustAll }
{ Resize all children }
  for I1 := 0 to (ChildCount - 1) do begin
    if Child[I1] is TRaveControl then begin
      TRaveControl(Child[I1]).AnchorAdjust(WidthChanging,OldWidth,NewWidth,
       HeightChanging,OldHeight,NewHeight);
    end; { if }
  end; { for }
end;  { AnchorAdjustAll }

procedure TRavePage.SetOrientation(Value: TOrientation);
var
  TempSize: TRaveUnits;
begin { SetOrientation }
  if Value <> FOrientation then begin
    if (Value = poLandscape) or (FOrientation = poLandscape) then begin
    { Switch page dimensions }
      TempSize := FPageWidth;
      FPageWidth := FPageHeight;
      FPageHeight := TempSize;
      AnchorAdjustAll(FNE(FPageWidth,FPageHeight),FPageHeight,FPageWidth,
       FNE(FPageWidth,FPageHeight),FPageWidth,FPageHeight);
      Invalidate;
    end; { if }
    FOrientation := Value;
  end; { if }
end;  { SetOrientation }

procedure TRavePage.SetPaperSize(Value: TRavePaperSize);
var
  I1: integer;
  OldWidth: TRaveUnits;
  OldHeight: TRaveUnits;
begin { SetPaperSize }
  if Value <> FPaperSize then begin
    OldWidth := FPageWidth;
    OldHeight := FPageHeight;
    for I1 := 1 to PaperSizeNameCnt do begin
      if PaperSizeName[I1].PaperSize = Value then begin
        if PaperSizeName[I1].PaperWidth <> 0 then begin
          if FOrientation = poLandscape then begin
            FPageHeight := PaperSizeName[I1].PaperWidth;
          end else begin
            FPageWidth := PaperSizeName[I1].PaperWidth;
          end; { else }
        end; { if }
        if PaperSizeName[I1].PaperHeight <> 0 then begin
          if FOrientation = poLandscape then begin
            FPageWidth := PaperSizeName[I1].PaperHeight;
          end else begin
            FPageHeight := PaperSizeName[I1].PaperHeight;
          end; { else }
        end; { if }
        Break;
      end; { if }
    end; { for }
    FPaperSize := Value;
    AnchorAdjustAll(FNE(OldWidth,FPageWidth),OldWidth,FPageWidth,
     FNE(OldHeight,FPageHeight),OldHeight,FPageHeight);
    Invalidate;
  end; { if }
end;  { SetPaperSize }

procedure TRavePage.SetPageWidth(Value: TRaveUnits);
var
  OldWidth: TRaveUnits;
begin { SetPageWidth }
  if FGT(Value,0.0) then begin
    OldWidth := FPageWidth;
    FPageWidth := Value;
    FPaperSize := DMPAPER_USER;
    Invalidate;
    AnchorAdjustAll(FNE(OldWidth,FPageWidth),OldWidth,FPageWidth,
     false,FPageHeight,FPageHeight);
  end; { if }
end;  { SetPageWidth }

procedure TRavePage.SetPageHeight(Value: TRaveUnits);
var
  OldHeight: TRaveUnits;
begin { SetPageHeight }
  if FGT(Value,0.0) then begin
    OldHeight := FPageHeight;
    FPageHeight := Value;
    FPaperSize := DMPAPER_USER;
    Invalidate;
    AnchorAdjustAll(false,FPageWidth,FPageWidth,
     FNE(OldHeight,FPageHeight),OldHeight,FPageHeight);
  end; { if }
end;  { SetPageHeight }

{ class TRaveDesigner }

procedure TRaveDesigner.SetPage(APage: TRavePage);
begin { SetPage }
  APage.FPage := APage;
  APage.FDesigner := self;
  FPage := APage;
end;  { SetPage }

{ class TRavePip }

constructor TRavePip.Create(AIndex: byte;
                            AControl: TRaveControl;
                            ADesigner: TRaveDesigner;
                            ACursor: TCursor;
                            AX: TRaveUnits;
                            AY: TRaveUnits);
begin { Create }
  inherited Create;
  Index := AIndex;
  Control := AControl;
  Designer := ADesigner;
  Cursor := ACursor;
  PageX := AX;
  PageY := AY;
  Reset;
end;  { Create }

destructor TRavePip.Destroy;
begin { Destroy }
{ Erase current location }
  Designer.Invalidate;
  inherited Destroy;
end;  { Destroy }

procedure TRavePip.Reset;
begin { Reset }
  Designer.Invalidate;
end;  { Reset }

function TRavePip.GetScreenRect: TRect;
begin { GetScreenRect }
  with Result do begin
    Left := Designer.XI2D(PageX) - 2;
    Right := Left + 5;
    Top := Designer.YI2D(PageY) - 2;
    Bottom := Top + 5;
  end; { with }
end;  { GetScreenRect }

procedure TRavePip.Move(X: TRaveUnits;
                        Y: TRaveUnits);
begin { Move }
  Designer.Invalidate;
  PageX := X;
  PageY := Y;
  Reset;
end;  { Move }

procedure TRavePip.Draw(Canvas: TCanvas);
var
  X,Y: integer;
begin { Draw }
  X := Designer.XI2D(PageX);
  Y := Designer.YI2D(PageY);
  Canvas.Rectangle(X - 2,Y - 2,X + 3,Y + 3);
end;  { Draw }

{ class TPropertyOverride }

type
  TPropertyOverride = class
  protected
    FName: string;
    FReadProc: TRaveReaderProc;
    FWriteProc: TRaveWriterProc;
  public
    constructor Create(AName: string;
                       AReadProc: TRaveReaderProc;
                       AWriteProc: TRaveWriterProc);
    property Name: string read FName;
    property ReadProc: TRaveReaderProc read FReadProc;
    property WriteProc: TRaveWriterProc read FWriteProc;
  end; { TPropertyOverride }

constructor TPropertyOverride.Create(AName: string;
                                     AReadProc: TRaveReaderProc;
                                     AWriteProc: TRaveWriterProc);
begin { Create }
  FName := AName;
  FReadProc := AReadProc;
  FWriteProc := AWriteProc;
end;  { Create }

{ class TRaveFiler }

constructor TRaveFiler.Create(AStreamHelper: TStreamHelper);
begin { Create }
  inherited Create(nil,16);
  FStreamHelper := AStreamHelper;
end;  { Create }

destructor TRaveFiler.Destroy;
begin { Destroy }
  inherited Destroy;
end;  { Destroy }

{ This methods is called to add a property override, usually from
  TRaveComponent.OverrideProperties }

procedure TRaveFiler.OverrideProperty(Name: string;
                                      ReadData: TRaveReaderProc;
                                      WriteData: TRaveWriterProc);
begin { OverrideProperty }
  if (IsReader and Assigned(ReadData)) or (not IsReader and Assigned(WriteData)) then begin
    if not Assigned(FOverrideList) then begin
      FOverrideList := TStringList.Create;
      FOverrideList.Sorted := true;
    end; { if }
    OverrideList.AddObject(Name,TPropertyOverride.Create(Name,ReadData,WriteData));
  end; { if }
end;  { OverrideProperty }

{ This method is called to see if there is an override for the property, Name }

function TRaveFiler.PropertyOverride(    Name: string;
                                     var ReadData: TRaveReaderProc;
                                     var WriteData: TRaveWriterProc): boolean;
var
  Index: integer;
begin { PropertyOverride }
  if Assigned(FOverrideList) then begin
    Result := OverrideList.Find(Name,Index);
    if Result then begin
      with TPropertyOverride(OverrideList.Objects[Index]) do begin
        ReadData := ReadProc;
        WriteData := WriteProc;
      end; { with }
    end; { if }
  end else begin
    Result := false;
  end; { else }
end;  { PropertyOverride }

procedure TRaveFiler.DefineProperty(const Name: string;
                                          ReadData: TReaderProc;
                                          WriteData: TWriterProc;
                                          HasData: boolean);
begin { DefineProperty }
end;  { DefineProperty }

procedure TRaveFiler.DefineBinaryProperty(const Name: string;
                                                ReadData: TStreamProc;
                                                WriteData: TStreamProc;
                                                HasData: boolean);
begin { DefineBinaryProperty }
end;  { DefineBinaryProperty }

procedure TRaveFiler.FlushBuffer;
begin { FlushBuffer }
end;  { FlushBuffer }

{ class TRaveWriter }

constructor TRaveWriter.Create(AStreamHelper: TStreamHelper);
begin { Create }
  inherited Create(AStreamHelper);
end;  { Create }

destructor TRaveWriter.Destroy;
begin { Destroy }
  inherited Destroy;
end;  { Destroy }

procedure TRaveWriter.DefineProperty(const Name: string;
                                           ReadData: TReaderProc;
                                           WriteData: TWriterProc;
                                           HasData: boolean);
var
  Writer: TWriter;
  Stream: TMemoryStream;
begin { DefineProperty }
  if HasData and Assigned(WriteData) then begin
    Stream := TMemoryStream.Create;
    try
      Writer := TWriter.Create(Stream,1024);
      try
        WriteData(Writer);
      finally
        Writer.Free;
      end; { tryf }
      StreamHelper.WriteString(Name);
      StreamHelper.WriteIndex(Stream.Size);
      StreamHelper.WriteBuf(Stream.Memory^,Stream.Size);
    finally
      Stream.Free;
    end; { tryf }
  end; { if }
end;  { DefineProperty }

procedure TRaveWriter.DefineBinaryProperty(const Name: string;
                                                 ReadData: TStreamProc;
                                                 WriteData: TStreamProc;
                                                 HasData: boolean);
var
  Stream: TMemoryStream;
begin { DefineBinaryProperty }
  if HasData and Assigned(WriteData) then begin
    Stream := TMemoryStream.Create;
    try
      WriteData(Stream);
      //!!PORT!! RVCLASS03
//      If (DefineInstance is TMetaFile) and (CompareText(Name,{Trans-}'Data') = 0) and
//       (Stream.Size <= 4) then Exit; { Bypass ReadData bug in TMetaFile VCL4 and earlier }
      
      StreamHelper.WriteString(Name);
      StreamHelper.WriteIndex(Stream.Size);
      StreamHelper.WriteBuf(Stream.Memory^,Stream.Size);
    finally
      Stream.Free;
    end; { tryf }
  end; { if }
end;  { DefineBinaryProperty }

procedure TRaveWriter.FlushBuffer;
begin { FlushBuffer }
end;  { FlushBuffer }

procedure TRaveWriter.WriteComponent(Value: TRaveComponent;
                                     DoChildren: boolean);
begin { WriteComponent }
  with StreamHelper do begin
    FRootOwner := Value.Owner as TRaveComponent;
    WriteByte(0); { Options }
    WriteString(Value.ClassName);
    WriteString(Value.Name);
    WriteProperties(Value);
    if DoChildren then begin
      WriteChildren(Value);
    end else begin
      WriteIndex(0);
    end; { else }
  end; { with }
end;  { WriteComponent }

procedure TRaveWriter.WriteChildren(Instance: TRaveComponent);
var
  I1: integer;
begin { WriteChildren }
  StreamHelper.WriteIndex(Instance.ChildCount);
  for I1 := 0 to Instance.ChildCount - 1 do begin
    WriteComponent(Instance.Child[I1],true);
  end; { for }
end;  { WriteChildren }

procedure TRaveWriter.WriteProperties(Instance: TPersistent);
var
  Count: integer;
  PropList: PPropList;
  PropInfo: PPropInfo;
  I1: integer;
  ReadData: TRaveReaderProc;
  WriteData: TRaveWriterProc;
  SaveOverrideList: TStringList;
begin { WriteProperties }
  with StreamHelper do begin
    WriteByte(Ord(vkPropList));

  { Get list of properties to override }
    FOverrideList := nil;
    if Instance is TRaveComponent then begin
      TRaveComponent(Instance).OverrideProperties(self);
    end; { if }
    SaveOverrideList := FOverrideList;

  { Write out non-published property data }
  { Typecast as TRaveComponent to get access to protected method }
    DefineInstance := Instance;
    TRaveComponent(Instance).DefineProperties(self);

  { Process list of published properties }
    Count := GetTypeData(Instance.ClassInfo)^.PropCount;
    if Count > 0 then begin
      GetMem(PropList,Count * SizeOf(pointer));
      try
        GetPropInfos(Instance.ClassInfo,PropList);
        for I1 := 0 to (Count - 1) do begin
          PropInfo := PropList^[I1];
          if (Instance is TComponent) and (PropInfo.Name = {Trans-}'Name') then Continue;
          FOverrideList := SaveOverrideList;
          if PropertyOverride(PropInfo.Name,ReadData,WriteData) and
           Assigned(WriteData) then begin
            WriteData(self);
          end else begin
            if IsStoredProp(Instance,PropInfo) then begin
              WriteProperty(Instance,PropInfo);
            end; { if }
          end; { else }
        end; { for }
      finally
        FreeMem(PropList,Count * SizeOf(pointer));
      end; { tryf }
    end; { if }
    WriteString(''); { No more properties }

  { Free property override list }
    if Assigned(SaveOverrideList) then begin
      ClearStringList(SaveOverrideList);
      SaveOverrideList.Free;
    end; { if }
  end; { with }
end;  { WriteProperties }

procedure TRaveWriter.WriteProperty(Instance: TPersistent;
                                    PropInfo: PPropInfo);
type
  TWriteSet = set of 0..31;
var
  I1: longint;
  I2: longint;
  S1: string;
  O1: TObject;
  O2: TObject;

  function GetComponentID(C1: TComponent): string;
  begin { GetComponentID }
    if Assigned(C1) then begin
      Result := C1.Name;
      if (RootOwner <> C1.Owner) and (TRaveProjectManager(RootOwner.Project) <> C1.Owner) then begin
        Result := C1.Owner.Name + '.' + Result;
      end; { if }
      if (C1 is TRaveComponent) and TRaveComponent(C1).UseMaster then begin
        Result := '*' + Result;
      end; { if }
    end; { if }
  end;  { GetComponentID }

begin { WriteProperty }
  if not Assigned(PropInfo.SetProc) or
   not IsStoredProp(Instance,PropInfo) then Exit;

  case PropInfo.PropType^.Kind of
    tkInteger: begin
      I1 := GetOrdProp(Instance,PropInfo);
      if I1 <> PropInfo.Default then begin
        WritePropName(PropInfo);
        WriteInteger(I1);
      end; { if }
    end;
    tkChar: begin
      I1 := GetOrdProp(Instance,PropInfo);
      if I1 <> PropInfo.Default then begin
        WritePropName(PropInfo);
        WriteChar(Char(I1));
      end; { if }
    end;
    tkEnumeration: begin
      I1 := GetOrdProp(Instance,PropInfo);
      if I1 <> PropInfo.Default then begin
        WritePropName(PropInfo);
        if AnsiSameText(PropInfo.Name,{Trans-}'CharSet') then begin
          WriteInteger(CLXToVCLCharSet(TFontCharSet(I1)));
        end else begin
          WriteID(GetEnumName(PropInfo.PropType^,I1));
        end; { else }
      end; { if }
    end;
    tkFloat: begin
      WritePropName(PropInfo);
      WriteFloat(GetFloatProp(Instance,PropInfo));
    end;
    tkString: begin
      S1 := GetStrProp(Instance,PropInfo);
      if S1 <> '' then begin
        WritePropName(PropInfo);
        WriteString(S1);
      end; { if }
    end;
    tkSet: begin
      I1 := GetOrdProp(Instance,PropInfo);
      if I1 <> PropInfo.Default then begin
        WritePropName(PropInfo);
        StreamHelper.WriteByte(Ord(vkSet));
        for I2 := 0 to 31 do begin
          if I2 in TWriteSet(I1) then begin
            StreamHelper.WriteString(GetEnumName(GetTypeData(PropInfo.PropType^).CompType^,I2));
          end; { if }
        end; { for }
        StreamHelper.WriteString('');
      end; { if }
    end;
    tkClass: begin
      O1 := TObject(GetOrdProp(Instance,PropInfo));
      if not Assigned(O1) then begin
        if (PropInfo.Default <> $00000000) and
         (PropInfo.Default <> longint($80000000)) then begin
          WritePropName(PropInfo);
          WriteInteger(0);
        end; { if }
      end else begin
        if O1 is TComponent then begin { Write out [Owner.]Name }
          WritePropName(PropInfo);
          WriteID(GetComponentID(TComponent(O1)));
        end else if O1 is TPersistent then begin { Write out properties }
          WritePropName(PropInfo);
          WriteProperties(TPersistent(O1));
        end else if O1 is TRaveComponentList then begin { Write out list of components }
          WritePropName(PropInfo);
          StreamHelper.WriteByte(Ord(vkComponentList));
          with TRaveComponentList(O1) do begin
            for I1 := 0 to Count - 1 do begin
              StreamHelper.WriteString(GetComponentID(TComponent(Items[I1])));
            end; { for }
            StreamHelper.WriteString('');
          end; { with }
        end else if O1 is TRavePersistentList then begin { Write out list of TPersistent }
          WritePropName(PropInfo);
          StreamHelper.WriteByte(Ord(vkPersistentList));
          with TRavePersistentList(O1) do begin
            for I1 := 0 to Count - 1 do begin
              O2 := TObject(Items[I1]);
              if Assigned(O2) then begin
                StreamHelper.WriteString(O2.ClassName);
                WriteProperties(TPersistent(O2));
              end; { if }
            end; { for }
          end; { with }
          StreamHelper.WriteString('');
        end else begin
          RaveError(Trans('Attempt to write invalid class type.'));
        end; { else }
      end; { else }
    end;
    tkLString: begin
      S1 := GetStrProp(Instance,PropInfo);
      if S1 <> '' then begin
        WritePropName(PropInfo);
        WriteString(S1);
      end; { if }
    end;
  end; { case }
end;  { WriteProperty }

procedure TRaveWriter.WritePropName(PropInfo: PPropInfo);
begin { WritePropName }
  StreamHelper.WriteString(PropInfo.Name);
end;  { WritePropName }

procedure TRaveWriter.WriteID(Value: string);
begin { WriteID }
  StreamHelper.WriteByte(Ord(vkID));
  StreamHelper.WriteString(Value);
end;  { WriteID }

procedure TRaveWriter.WriteInteger(Value: longint);
begin { WriteInteger }
  if (Value >= 0) and (Value < $10000000) then begin
    StreamHelper.WriteByte(Ord(vkIndex));
    StreamHelper.WriteIndex(Abs(Value));
  end else if (Value < 0) and (Abs(Value) < $10000000) then begin
    StreamHelper.WriteByte(Ord(vkNegIndex));
    StreamHelper.WriteIndex(Abs(Value));
  end else begin
    StreamHelper.WriteByte(Ord(vkInteger));
    StreamHelper.WriteBuf(Value,SizeOf(Value));
  end; { else }
end;  { WriteInteger }

procedure TRaveWriter.WriteChar(Value: char);
begin { WriteChar }
  StreamHelper.WriteByte(Ord(vkChar));
  StreamHelper.WriteBuf(Value,SizeOf(Value));
end;  { WriteChar }

procedure TRaveWriter.WriteFloat(Value: extended);
begin { WriteFloat }
  StreamHelper.WriteByte(Ord(vkFloat));
  StreamHelper.WriteBuf(Value,SizeOf(Value));
end;  { WriteFloat }

procedure TRaveWriter.WriteString(Value: string);
begin { WriteString }
  StreamHelper.WriteByte(Ord(vkString));
  StreamHelper.WriteString(Value);
end;  { WriteString }

{ class TRaveReader }

constructor TRaveReader.Create(AStreamHelper: TStreamHelper);
begin { Create }
  inherited Create(AStreamHelper);
  FIsReader := true;
end;  { Create }

destructor TRaveReader.Destroy;
begin { Destroy }
  inherited Destroy;
end;  { Destroy }

procedure TRaveReader.DefineProperty(const Name: string;
                                           ReadData: TReaderProc;
                                           WriteData: TWriterProc;
                                           HasData: boolean);
var
  Stream: TMemoryStream;
  Reader: TReader;
  Size: integer;
begin { DefineProperty }
  if not PropHandled and (AnsiCompareText(Name,PropName) = 0) and
   Assigned(ReadData) then begin
    Stream := TMemoryStream.Create;
    try
      Size := StreamHelper.ReadIndex;
      Stream.CopyFrom(StreamHelper.Stream,Size);
      Stream.Position := 0;
      Reader := TReader.Create(Stream,1024);
      try
        ReadData(Reader);
      finally
        Reader.Free;
      end; { tryf }
    finally
      Stream.Free;
    end; { tryf }
    PropHandled := true;
  end; { if }
end;  { DefineProperty }

procedure TRaveReader.DefineBinaryProperty(const Name: string;
                                                 ReadData: TStreamProc;
                                                 WriteData: TStreamProc;
                                                 HasData: boolean);
var
  Stream: TMemoryStream;
  Size: integer;
begin { DefineBinaryProperty }
  if not PropHandled and (AnsiCompareText(Name,PropName) = 0) and
   Assigned(ReadData) then begin
    Stream := TMemoryStream.Create;
    try
      Size := StreamHelper.ReadIndex;
      Stream.CopyFrom(StreamHelper.Stream,Size);
      Stream.Position := 0;
      ReadData(Stream);
    finally
      Stream.Free;
    end; { tryf }
    PropHandled := true;
  end; { if }
end;  { DefineBinaryProperty }

procedure TRaveReader.FlushBuffer;
begin { FlushBuffer }
end;  { FlushBuffer }

function TRaveReader.AbsorbProperty: boolean;
begin { AbsorbProperty }
  Result := FAbsorbCount > 0;
end;  { AbsorbProperty }

function TRaveReader.ReadComponent(aOwner: TRaveComponent;
                                   aParent: TRaveComponent): TRaveComponent;
var
  AClassName: string;
  AName: string;
  AClass: TClass;
  Children: integer;
  I1: integer;
  Accepted: boolean;
  NameRoot: string;
begin { ReadComponent }
  FAbsorbCount := 0;
  with StreamHelper do begin
    FRootOwner := aOwner;
    FCurrentParent := aParent;
    ReadByte; { Options }
    AClassName := ReadString;
    AName := ReadString;
    try
      AClass := FindClass(AClassName);
    except
      AClass := nil;
    end; { tryx }
    if not Assigned(AClass) then begin
    // Error, class not found, skip component
      if InDesigner then begin
        ShowMessage(Trans(Format({Trans+}'Unknown component "%s" found.  You will ' +
         'need to add the package containing this component before you can ' +
         'load this report project.',[AClassName])));
      end else begin
        ShowMessage(Trans(Format({Trans+}'Unknown class "%s" found.  You will ' +
         'need to include the unit containing this component to your uses clause.',
         [AClassName])));
      end; { else }
      Result := nil;
      Inc(FAbsorbCount);
      try
        ReadByte; { Read in vkProperties }
        ReadProperties(Result);

      { Read children }
        Children := ReadIndex;
        for I1 := 1 to Children do begin
          ReadComponent(aOwner,Result);
        end; { for }
      finally
        Dec(FAbsorbCount);
      end; { tryf }
    end else begin
      Result := TRaveComponent(AClass.NewInstance);
      Result.Create(RootOwner);
      Result.ControlStyle := Result.ControlStyle + [rcsLoading];
      Accepted := Result.AcceptParent(aParent.ClassType) and
       aParent.AcceptChild(AClass);
      if Accepted then begin
        CurrentComp := Result;
        Result.Parent := aParent;
        if CheckNames then begin
          NameRoot := AClassName;
          if NameRoot[1] = 'T' then begin
            Delete(NameRoot,1,1);
          end; { if }
          if Pos({Trans-}'RAVE',UpperCase(NameRoot)) = 1 then begin
            Delete(NameRoot,1,4);
          end; { if }
          Result.Name := TRaveProjectManager(AOwner.Project).GetUniqueName(AName + '|' +
           NameRoot,RootOwner,true);
          if Result.Name = AName then begin
            TRaveProjectManager(AOwner.Project).RemoveForwardRefs(Result);
          end else begin
            TRaveProjectManager(AOwner.Project).AddRenameRef(AName,Result);
          end; { else }
        end else begin
          Result.Name := AName;
          TRaveProjectManager(AOwner.Project).RemoveForwardRefs(Result);
        end; { else }
        if AddComponents then begin
          AddComponent(Result);
        end; { if }
      end else begin
        Inc(FAbsorbCount);
      end; { else }

      ReadByte; { Read in vkProperties }
      ReadProperties(Result);

    { Read children }
      Children := ReadIndex;
      for I1 := 1 to Children do begin
        ReadComponent(aOwner,Result);
      end; { for }
      CurrentParent := aParent;
      CurrentComp := Result;

    { Clean up }
      Result.ControlStyle := Result.ControlStyle - [rcsLoading];
      if not Accepted then begin
        Dec(FAbsorbCount);
        FreeAndNil(Result);
      end; { if }
    end; { else }
  end; { with }
end;  { ReadComponent }

procedure TRaveReader.ReadIntoComponent(Value: TRaveComponent);
var
  AClassName: string;
  AName: string;
  I1: integer;
  Children: integer;
begin { ReadIntoComponent }
  FAbsorbCount := 0;
  Value.ControlStyle := Value.ControlStyle + [rcsLoading];
  try
    with StreamHelper do begin
      FRootOwner := Value.Owner as TRaveComponent;
      ReadByte; { Options }
      AClassName := ReadString;
      AName := ReadString;
      if FindClass(AClassName) <> Value.ClassType then begin
      //!!! Invalid class type
      end else begin
        Value.Name := AName;
        TRaveProjectManager(Value.Project).RemoveForwardRefs(Value);
        ReadByte; { Read in vkProperties }
        ReadProperties(Value);

      { Read children }
        Children := ReadIndex;
        for I1 := 1 to Children do begin
          ReadComponent(Value,Value);
        end; { for }
      end; { else }
    end; { with }
  finally
    Value.ControlStyle := Value.ControlStyle - [rcsLoading];
  end; { tryf }
end;  { ReadIntoComponent }

procedure TRaveReader.ReadProperties(Instance: TPersistent);
var
  PropInfo: PPropInfo;
  ReadData: TRaveReaderProc;
  WriteData: TRaveWriterProc;
  SaveOverrideList: TStringList;
begin { ReadProperties }
  with StreamHelper do begin
  { Get list of properties to override }
    FOverrideList := nil;
    if Instance is TRaveComponent then begin
      TRaveComponent(Instance).OverrideProperties(self);
    end; { if }
    SaveOverrideList := FOverrideList;

    repeat
      PropName := ReadString;
      if PropName = '' then Break;
      PropHandled := false;

    { Check for any non-published properties to write }
    { Typecast as TRaveComponent to get access to TComponent protected method }
    { Top and Left are special properties in Delphi used for Form Inheritance,
      so we want to ignore them }
      if (PropName <> {Trans-}'Left') and (PropName <> {Trans-}'Top') and
       Assigned(Instance) then begin
        TRaveComponent(Instance).DefineProperties(self);
      end; { if }

      if not PropHandled then begin
      { Look for property in Override list }
        FOverrideList := SaveOverrideList;
        if PropertyOverride(PropName,ReadData,WriteData) and
         Assigned(ReadData) then begin
          ReadData(self);
        end else begin { Treat as normal published property }
          if not Assigned(Instance) then begin
            PropInfo := nil;
          end else begin
            PropInfo := GetPropInfo(Instance.ClassInfo,PropName);
          end; { else }
          if not Assigned(PropInfo) then begin
            Inc(FAbsorbCount)
          end; { if }
          ReadProperty(Instance,PropInfo);
          if not Assigned(PropInfo) then begin
            Dec(FAbsorbCount);
          end; { if }
        end; { else }
      end; { if }
    until false;

  { Free property override list }
    if Assigned(SaveOverrideList) then begin
      ClearStringList(SaveOverrideList);
      SaveOverrideList.Free;
    end; { if }
  end; { with }
end;  { ReadProperties }

procedure TRaveReader.ReadProperty(Instance: TPersistent; PropInfo: PPropInfo);
const
  TypeFromValue: array[TValueKind] of TTypeKind = (tkEnumeration,tkInteger,
   tkInteger,tkInteger,tkChar,tkFloat,tkString,tkSet,tkClass,tkClass,tkClass);
type
  TReadSet = set of 0..31;
var
  C1: char;
  F1: extended;
  I1: longint;
  S1: string;
  O1: TObject;
  ValueKind: TValueKind;
  TypeKind: TTypeKind;
  PerList: TRavePersistentList;
  CompList: TRaveComponentList;
begin { ReadProperty }
  ValueKind := TValueKind(StreamHelper.ReadByte);
  if Assigned(PropInfo) then begin
    TypeKind := PropInfo.PropType^.Kind;
  end else begin
    TypeKind := TypeFromValue[ValueKind];
  end; { else }
  case TypeKind of
    tkInteger: begin
      case ValueKind of
        vkID: begin
          I1 := 0; {!!! Read from named integer list }
        end;
        vkIndex: begin
          I1 := StreamHelper.ReadIndex;
        end;
        vkNegIndex: begin
          I1 := -StreamHelper.ReadIndex;
        end;
        vkInteger: begin
          I1 := ReadInteger;
        end;
        else begin
        //!!! Error, invalid ValueType
          I1 := 0;
        end;
      end; { case }
      if not AbsorbProperty and Assigned(PropInfo.SetProc) then begin
        SetOrdProp(Instance,PropInfo,I1);
      end; { if }
    end;
    tkChar: begin
      C1 := ReadChar;
      if not AbsorbProperty and Assigned(PropInfo.SetProc) then begin
        SetOrdProp(Instance,PropInfo,integer(C1));
      end; { if }
    end;
    tkEnumeration: begin
      if AnsiSameText(PropInfo.Name,{Trans-}'CharSet') then begin
        I1 := StreamHelper.ReadIndex;
        if not AbsorbProperty and Assigned(PropInfo.SetProc) then begin
          SetOrdProp(Instance,PropInfo,integer(VCLToCLXCharSet(I1)));
        end; { if }
      end else begin
        S1 := ReadString;
        if not AbsorbProperty and Assigned(PropInfo.SetProc) then begin
          SetOrdProp(Instance,PropInfo,GetEnumValue(PropInfo.PropType^,S1));
        end; { if }
      end; { else }
    end;
    tkFloat: begin
      F1 := ReadFloat;
      if not AbsorbProperty and Assigned(PropInfo.SetProc) then begin
        SetFloatProp(Instance,PropInfo,F1);
      end; { if }
    end;
    tkString: begin
      S1 := ReadString;
      if not AbsorbProperty and Assigned(PropInfo.SetProc) then begin
        SetStrProp(Instance,PropInfo,S1);
      end; { if }
    end;
    tkSet: begin
      I1 := 0;
      S1 := ReadString;
      while S1 <> '' do begin
        if not AbsorbProperty then begin
          Include(TReadSet(I1),GetEnumValue(GetTypeData(PropInfo.PropType^).CompType^,S1));
        end; { if }
        S1 := ReadString;
      end; { while }
      if not AbsorbProperty and Assigned(PropInfo.SetProc) then begin
        SetOrdProp(Instance,PropInfo,I1);
      end; { if }
    end;
    tkClass: begin
      case ValueKind of
        vkIndex: begin
          I1 := StreamHelper.ReadIndex;
          if not AbsorbProperty and Assigned(PropInfo.SetProc) then begin
            SetOrdProp(Instance,PropInfo,I1);
          end; { if }
        end;
        vkID: begin
          S1 := ReadString;
          if not AbsorbProperty and Assigned(PropInfo.SetProc) then begin
            if CheckNames then begin
              O1 := nil;
            end else begin
              if Assigned(MasterProject) and (S1 <> '') and (S1[1] = '*') then begin
                Delete(S1,1,1);
                O1 := TRaveProjectManager(MasterProject).FindRaveComponent(S1,RootOwner);
              end else begin
                if S1 = '' then begin
                  O1 := nil;
                end else begin
                  if S1[1] = '*' then begin
                    Delete(S1,1,1);
                  end; { if }
                  if not Assigned(RootOwner.Project) then begin
                  // This usually occurs when reading in properties for the project itself
                    O1 := nil;
                  end else begin
                    O1 := TRaveProjectManager(RootOwner.Project).FindRaveComponent(S1,RootOwner);
                  end; { else }
                end; { else }
              end; { else }
            end; { else }
            if not Assigned(O1) then begin { Register as forward reference }
              if not Assigned(RootOwner.Project) then begin
                TRaveProjectManager(Instance).AddForwardRef(S1,RootOwner,Instance,PropInfo,0);
              end else begin
                TRaveProjectManager(RootOwner.Project).AddForwardRef(S1,RootOwner,Instance,PropInfo,0);
              end; { else }
            end else begin
              SetOrdProp(Instance,PropInfo,integer(O1));
            end; { else }
          end; { if }
        end;
        vkPropList: begin
          if PropInfo = nil then begin
            O1 := Instance;
          end else begin
            O1 := TObject(GetOrdProp(Instance,PropInfo));
            if not Assigned(O1) then begin { Create object }
              O1 := GetTypeData(PropInfo^.PropType^).ClassType.NewInstance;
              O1.Create;
              SetOrdProp(Instance,PropInfo,integer(O1));
              if O1 is TRaveEvent then begin
                TRaveEvent(O1).Owner := CurrentComp;
              end; { if }
            end; { if }
          end; { else }
          ReadProperties(TPersistent(O1));
        end;
        vkComponentList: begin
          CompList := TRaveComponentList.Create;
          try
            CompList.Clear;
            S1 := ReadString;
            while S1 <> '' do begin
              if CheckNames then begin
                O1 := nil;
              end else begin
                if Assigned(MasterProject) and (S1 <> '') and (S1[1] = '*') then begin
                  Delete(S1,1,1);
                  O1 := TRaveProjectManager(MasterProject).FindRaveComponent(S1,RootOwner);
                end else begin
                  if S1[1] = '*' then begin
                    Delete(S1,1,1);
                  end; { if }
                  O1 := TRaveProjectManager(RootOwner.Project).FindRaveComponent(S1,RootOwner);
                end; { else }
              end; { else }
              if not Assigned(O1) then begin { Register as forward reference }
                CompList.Add(nil);
                TRaveProjectManager(RootOwner.Project).AddForwardRef(S1,RootOwner,TPersistent(CompList),nil,
                 CompList.Count - 1);
              end else begin
                CompList.Add(O1);
              end; { else }
              S1 := ReadString;
            end; { while }
            if not AbsorbProperty and Assigned(PropInfo.SetProc) then begin
              SetOrdProp(Instance,PropInfo,integer(CompList));
            end; { if }
          finally
            CompList.Free;
          end; { tryf }
        end;
        vkPersistentList: begin
          PerList := TRavePersistentList(GetOrdProp(Instance,PropInfo));
          PerList.Clear;
          S1 := ReadString;
          while S1 <> '' do begin
            StreamHelper.ReadByte; { Absorb vkPropList }
            O1 := PerList.CreateItem(S1,RootOwner);
            ReadProperties(TPersistent(O1));
            PerList.Add(O1);
            S1 := ReadString;
          end; { while }
        end;
      end; { case }
    end;
    tkLString: begin
      S1 := ReadString;
      if not AbsorbProperty and Assigned(PropInfo.SetProc) then begin
        SetStrProp(Instance,PropInfo,S1);
      end; { if }
    end;
  end; { case }
end;  { ReadProperty }

function TRaveReader.ReadID: string;
begin { ReadID }
  Result := StreamHelper.ReadString;
end;  { ReadID }

function TRaveReader.ReadInteger: longint;
begin { ReadInteger }
  StreamHelper.ReadBuf(Result,SizeOf(Result));
end;  { ReadInteger }

function TRaveReader.ReadChar: char;
begin { ReadChar }
  StreamHelper.ReadBuf(Result,SizeOf(Result));
end;  { ReadChar }

function TRaveReader.ReadFloat: extended;
begin { ReadFloat }
  StreamHelper.ReadBuf(Result,SizeOf(Result));
end;  { ReadFloat }

function TRaveReader.ReadString: string;
begin { ReadString }
  Result := StreamHelper.ReadString;
end;  { ReadString }

{ class TRaveEvent }

function TRaveEvent.SuspendValid: boolean;
begin { SuspendValid }
  Result := false;
end;  { SuspendValid }

procedure TRaveEvent.Execute;
begin { Execute }
  if Assigned(ModuleOwner) and Assigned(ModuleOwner.Module) then begin
    if not Assigned(FSuspendState) then begin
    // Set up stack for new execution
      ModuleOwner.Module.PushParam(FOwner);
      DefineParams;
    end; { else }
    ModuleOwner.Module.Execute(Position,FSuspendState);
    if Assigned(FSuspendState) and not SuspendValid then begin
      RaveError(Trans('Suspend call not valid in this event.'));
    end; { if }
  end; { if }
end;  { Execute }

function TRaveEvent.Suspended: boolean;
begin { Suspended }
  Result := Assigned(FSuspendState);
end;  { Suspended }

procedure TRaveEvent.DefineParams;
begin { DefineParams }
end;  { DefineParams }

function TRaveEvent.ParamSource: string;
begin { ParamSource }
  Result := '';
end;  { ParamSource }

procedure TRaveEvent.AssignTo(Dest: TPersistent);
begin { AssignTo }
  if Dest is TRaveEvent then begin
    with Dest as TRaveEvent do begin
      Header := self.Header;
      Body := self.Body;
      Position := self.Position;
    end; { with }
  end else begin
    inherited AssignTo(Dest);
  end; { else }
end;  { AssignTo }

procedure TRaveEvent.DefineParam(var Param);
begin { DefineParam }
  ModuleOwner.Module.PushParam(Param);
end;  { DefineParam }

function TRaveEvent.GetDeclaration: string;
begin { GetDeclaration }
  Result := #13#10'{ Event for ' + Owner.Name + '.' + Name + ' }'#13#10#13#10 +
   'function ' + Owner.Name + '_' + Name + '(' + 'Self: ' + GetSupportedClass(Owner.ClassType);
  if ParamSource = '' then begin
    Result := Result + ');';
  end else begin
    Result := Result + '; ' + ParamSource + ');';
  end; { else }
end;  { GetDeclaration }

function TRaveEvent.GetFooter: string;
begin { GetFooter }
  Result := 'end ' + Owner.Name + '_' + Name + ';';
end;  { GetFooter }

{ class TRaveStringEvent }

procedure TRaveStringEvent.DefineParams;
begin { DefineParams }
  DefineParam(FValue);
end;  { DefineParams }

function TRaveStringEvent.ParamSource: string;
begin { ParamSource }
  Result := 'var Value: string';
end;  { ParamSource }

{ class TRaveFloatEvent }

procedure TRaveFloatEvent.DefineParams;
begin { DefineParams }
  DefineParam(FValue);
end;  { DefineParams }

function TRaveFloatEvent.ParamSource: string;
begin { ParamSource }
  Result := 'var Value: extended';
end;  { ParamSource }

initialization
  RegisterProc({Trans-}'RVCL',RaveRegister);
finalization
  FreeAndNil(RegisterProcList);
end.