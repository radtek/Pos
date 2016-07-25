{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit QRvDefine;

{$I RpVer.inc}

interface

uses
  Windows,
  QControls, QForms, QGraphics, Types,
  Classes, SysUtils, QRpBase, QRpDefine;

var
  InDesigner: boolean = false;

const
  RaveVersion = 40005; { 4.0005 }
  RegRoot = {Trans-}'\Software\Nevrona Designs\Rave5BEX\';
  RaveMajorVersion = 100;
  RaveMinorVersion = 1;
  RaveDelta: double = 0.0001;
  HexDigits = {Trans-}'0123456789ABCDEF';

{ TTabSheet Tag property types }
  SourceSheet = 1;
  DesignerSheet = 2;
  DataViewSheet = 3;
  QuerySheet = 4;

{ Alignment types }
  RaveAlignLeft = 1;
  RaveAlignHCenter = 2;
  RaveAlignRight = 3;
  RaveAlignHCenterInParent = 4;
  RaveAlignHSpace = 5;
  RaveAlignEquateWidths = 6;
  RaveAlignTop = 7;
  RaveAlignVCenter = 8;
  RaveAlignBottom = 9;
  RaveAlignVCenterInParent = 10;
  RaveAlignVSpace = 11;
  RaveAlignEquateHeights = 12;
  RaveAlignMoveForward = 13;
  RaveAlignMoveBehind = 14;
  RaveAlignBringToFront = 15;
  RaveAlignSendToBack = 16;
  RaveAlignTapLeft = 17;
  RaveAlignTapRight = 18;
  RaveAlignTapUp = 19;
  RaveAlignTapDown = 20;
  RaveAlignTapHSizeDown = 21;
  RaveAlignTapHSizeUp = 22;
  RaveAlignTapVSizeDown = 23;
  RaveAlignTapVSizeUp = 24;

type
  ERaveClass = class(Exception);
  ERaveException = class(Exception);

  TUserLevel = (ulBeginner,ulIntermediate,ulAdvanced);
  TLineWidthType = (wtPoints,wtPixels);
  TFontAttribute = (faAll,faName,faSize,faBold,faItalic,faUnderline);
  TDisplayOn = (doParent,doAll,doPreviewOnly,doPrinterOnly);
  TGotoMode = (gmGotoDone,gmGotoNotDone,gmCallEach);
  TRaveAnchor = type byte;
  TRaveVAnchor = (anvTop,anvBottom,anvCenter,anvStretch,anvResize,anvSpread);
  TRaveHAnchor = (anhLeft,anhRight,anhCenter,anhStretch,anhResize,anhSpread);
  TRaveFillStyle = (fsSolid,fsClear,fsHorizontal,fsVertical,fsFDiagonal,
   fsBDiagonal,fsCross,fsDiagCross,fsNone);
  TRaveCalcType = (ctCount,ctSum,ctMin,ctMax,ctAverage);

  TRaveFloat = double;
  TRaveUnits = type TRaveFloat;
  TRaveFieldName = type string;
  TRaveMemoString = type string;

  PRavePoint = ^TRavePoint;
  TRavePoint = record
    X: TRaveUnits;
    Y: TRaveUnits;
  end; { TRavePoint }

  PRaveRect = ^TRaveRect;
  TRaveRect = record
    case byte of
      0:(Left: TRaveUnits;
         Top: TRaveUnits;
         Right: TRaveUnits;
         Bottom: TRaveUnits);
      1:(TopLeft: TRavePoint;
         BottomRight: TRavePoint);
  end; { TRaveRect }

  TRaveBin = type integer;
  TRaveBinName = record
    Name: string;
    RaveBin: TRaveBin;
  end; { TRaveBinName }

  TRavePrinterDuplex = (pdSimplex,pdHorizontal,pdVertical,pdDefault);
  TRavePrinterCollate = (pcFalse,pcTrue,pcDefault);
  TRavePrinterResolution = (prDraft,prLow,prMedium,prHigh,prDefault);
  TRaveSaveCallbackProc = procedure(Buf: pointer;
                                    Size: longint;
                                    Data: pointer); stdcall;

const
  RavePrinterResolution: array[TRavePrinterResolution] of DWORD =
   (DMRES_DRAFT,DMRES_LOW,DMRES_MEDIUM,DMRES_HIGH,0);
{TransOff}
  RaveBinNameCnt = 14;
  RaveBinName: array[1..RaveBinNameCnt] of TRaveBinName = (
   (Name:'Default'; RaveBin:-1),
   (Name:'Upper/Primary Tray'; RaveBin:DMBIN_UPPER),
   (Name:'Lower Tray'; RaveBin:DMBIN_LOWER),
   (Name:'Middle Tray'; RaveBin:DMBIN_MIDDLE),
   (Name:'Manual Feed'; RaveBin:DMBIN_MANUAL),
   (Name:'Envelope'; RaveBin:DMBIN_ENVELOPE),
   (Name:'Envelope Manual Feed'; RaveBin:DMBIN_ENVMANUAL),
   (Name:'Auto Select'; RaveBin:DMBIN_AUTO),
   (Name:'Tractor Feed'; RaveBin:DMBIN_TRACTOR),
   (Name:'SmallFmt'; RaveBin:DMBIN_SMALLFMT),
   (Name:'LargeFmt'; RaveBin:DMBIN_LARGEFMT),
   (Name:'Large Capacity'; RaveBin:DMBIN_LARGECAPACITY),
   (Name:'Cassette'; RaveBin:DMBIN_CASSETTE),
   (Name:'Form Source'; RaveBin:DMBIN_FORMSOURCE)
  );

  LineWidths: array[0..11] of TRaveFloat = (1.0,0.25,0.50,0.75,1.0,1.5,2.25,3.0,
   4.5,6.0,7.5,0.0);
{TransOn}

var
  RaveTitle: string = {Trans-}'Rave Reports 5.1'; { Title to be displayed in Visual Designer }
  DeveloperLevel: boolean = false; { Is the developer version of RAVE running? }
  UserLevel: TUserLevel; { Current user level - beginner, intermediate or advanced }
  FloatPropPrecision: integer; { Specifies how many decimal digets to show }
  GBaseReport: TBaseReport = nil; { Global BaseReport object }
{ DataConnection timeout values }
  EventDelays: array[0..13] of word = (600,600,600,600,600,600,600,600,600,600,600,600,600,600);
  LargeTimeouts: array[0..13] of word = (50,50,50,50,50,50,50,50,50,50,50,50,50,50);
  AlwaysShowHeaders: boolean; { Always show band headers? }
  AdminMode: boolean; { Running in admin mode? }
  SaveEnvOnly: boolean; { Prompt to Save Environment Only Settings On Exit? }
{ Printing Preferences Values }
  PrintDestination: TReportDest = rdPreview;
  AllowSetup: boolean = true;
  PreviewShadowDepth: integer = 5;
  PreviewWindowState: TWindowState = wsNormal;
  PreviewGridHoriz: TRaveFloat = 0.0;
  PreviewGridVert: TRaveFloat = 0.0;
  PreviewZoomFactor: TRaveFloat = 100.0;
  PreviewZoomInc: integer = 10;
  PreviewMonochrome: boolean = false;
  PreviewGridColor: TColor = clAqua;
  PreviewGridPenStyle: TPenStyle = psSolid;
  PreviewRulerType: TRulerType = rtNone;
  ProjectUnitsFactor: TRaveFloat = 1.0;
  PaperSize: TRavePaperSize = -1;
  PaperWidth: TRaveUnits = 8.5;
  PaperHeight: TRaveUnits = 11.0;
  GridSpacing: double = 0.1; { Spacing between each grid line }
  GridLines: integer = 5; { Draw grid line every GridLine spacing }
  RaveInitialized: boolean = false;
  DeveloperRave: boolean = false;
  TrueTypeFontImage: TBitmap = nil;
  DeviceFontImage: TBitmap = nil;
  TapDist: TRaveUnits; { Distance to move when tapping controls }
  ShowTapButtons: boolean; { Show tapping buttons }
  ShowOrderButtons: boolean; { Show order buttons }
  HoldAddDeleteComponents: boolean = false;

  procedure RaveError(ErrorMessage: string);

implementation

procedure RaveError(ErrorMessage: string);
begin { RaveError }
  Raise ERaveException.Create(ErrorMessage);
end;  { RaveError }

end.