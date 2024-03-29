
{*****************************************************************************}
{                                                                             }
{    Tnt Delphi Unicode Controls                                              }
{      http://home.ccci.org/wolbrink/tnt/delphi_unicode_controls.htm          }
{        Version: 2.0.22                                                      }
{                                                                             }
{    Copyright (c) 2002, 2003 Troy Wolbrink (troy.wolbrink@ccci.org)          }
{                                                                             }
{*****************************************************************************}

unit TntComCtrls;

{$INCLUDE TntCompilers.inc}

interface

uses
  Classes, Controls, {$IFDEF COMPILER_6_UP} ListActns, {$ENDIF} ComCtrls, Messages, Windows,
  CommCtrl, Contnrs, TntControls, TntClasses, Graphics, TntSysUtils;

type
  TTntCustomListView = class;
  TTntListItems = class;

{TNT-WARN TListColumn}
  TTntListColumn = class(TListColumn{TNT-ALLOW TListColumn})
  private
    FCaption: WideString;
    procedure SetInheritedCaption(const Value: AnsiString);
    function GetCaption: WideString;
    procedure SetCaption(const Value: WideString);
  protected
    procedure DefineProperties(Filer: TFiler); override;
  public
    procedure Assign(Source: TPersistent); override;
  published
    property Caption: WideString read GetCaption write SetCaption;
  end;

{TNT-WARN TListColumns}
  TTntListColumns = class(TListColumns{TNT-ALLOW TListColumns})
  private
    function GetItem(Index: Integer): TTntListColumn;
    procedure SetItem(Index: Integer; Value: TTntListColumn);
  public
    constructor Create(AOwner: TTntCustomListView);
    function Add: TTntListColumn;
    function Owner: TTntCustomListView;
    property Items[Index: Integer]: TTntListColumn read GetItem write SetItem; default;
  end;

{TNT-WARN TListItem}
  TTntListItem = class(TListItem{TNT-ALLOW TListItem})
  private
    FCaption: WideString;
    FSubItems: TTntStrings;
    procedure SetInheritedCaption(const Value: AnsiString);
    function GetCaption: WideString;
    procedure SetCaption(const Value: WideString);
    procedure SetSubItems(const Value: TTntStrings);
    function GetListView: TTntCustomListView;
    function GetTntOwner: TTntListItems;
  public
    constructor Create(AOwner: TListItems{TNT-ALLOW TListItems}); virtual;
    destructor Destroy; override;
    property Owner: TTntListItems read GetTntOwner;
    property ListView: TTntCustomListView read GetListView;
    procedure Assign(Source: TPersistent); override;
    property Caption: WideString read GetCaption write SetCaption;
    property SubItems: TTntStrings read FSubItems write SetSubItems;
  end;

{TNT-WARN TListItems}
  TTntListItems = class(TListItems{TNT-ALLOW TListItems})
  private
    function GetItem(Index: Integer): TTntListItem;
    procedure SetItem(Index: Integer; const Value: TTntListItem);
  public
    function Owner: TTntCustomListView;
    property Item[Index: Integer]: TTntListItem read GetItem write SetItem; default;
    function Add: TTntListItem;
    {$IFDEF COMPILER_6_UP}
    function AddItem(Item: TTntListItem; Index: Integer = -1): TTntListItem;
    {$ENDIF}
    function Insert(Index: Integer): TTntListItem;
  end;

  TTntLVEditedEvent = procedure(Sender: TObject; Item: TTntListItem; var S: WideString) of object;
  TTntLVOwnerDataFindEvent = procedure(Sender: TObject; Find: TItemFind;
    const FindString: WideString; const FindPosition: TPoint; FindData: Pointer;
    StartIndex: Integer; Direction: TSearchDirection; Wrap: Boolean;
    var Index: Integer) of object;

{TNT-WARN TCustomListView}
  _TntInternalCustomListView = class(TCustomListView{TNT-ALLOW TCustomListView})
  private
    PWideFindString: PWideChar;
    CurrentDispInfo: PLVDispInfoW;
    OriginalDispInfoMask: Cardinal;
    function OwnerDataFindW(Find: TItemFind; const FindString: WideString;
      const FindPosition: TPoint; FindData: Pointer; StartIndex: Integer;
        Direction: TSearchDirection; Wrap: Boolean): Integer; virtual; abstract;
    function OwnerDataFetchW(Item: TListItem{TNT-ALLOW TListItem}; Request: TItemRequest): Boolean; virtual; abstract;
  protected
    function OwnerDataFind(Find: TItemFind; const FindString: AnsiString;
      const FindPosition: TPoint; FindData: Pointer; StartIndex: Integer;
        Direction: TSearchDirection; Wrap: Boolean): Integer; override;
    function OwnerDataFetch(Item: TListItem{TNT-ALLOW TListItem}; Request: TItemRequest): Boolean; override;
  end;

  TTntCustomListView = class(_TntInternalCustomListView, IWideCustomListControl)
  private
    FEditHandle: THandle;
    FEditInstance: Pointer;
    FDefEditProc: Pointer;
    FOnEdited: TTntLVEditedEvent;
    FOnDataFind: TTntLVOwnerDataFindEvent;
    procedure EditWndProcW(var Message: TMessage);
    procedure BeginChangingWideItem;
    procedure EndChangingWideItem;
    function GetHint: WideString;
    procedure SetHint(const Value: WideString);
    function IsHintStored: Boolean;
    function GetListColumns: TTntListColumns;
    procedure SetListColumns(const Value: TTntListColumns);
    function ColumnFromIndex(Index: Integer): TTntListColumn;
    function GetColumnFromTag(Tag: Integer): TTntListColumn;
    function OwnerDataFindW(Find: TItemFind; const FindString: WideString;
      const FindPosition: TPoint; FindData: Pointer; StartIndex: Integer;
        Direction: TSearchDirection; Wrap: Boolean): Integer; override;
    function OwnerDataFetchW(Item: TListItem{TNT-ALLOW TListItem}; Request: TItemRequest): Boolean; override;
    function GetDropTarget: TTntListItem;
    procedure SetDropTarget(const Value: TTntListItem);
    function GetItemFocused: TTntListItem;
    procedure SetItemFocused(const Value: TTntListItem);
    function GetSelected: TTntListItem;
    procedure SetSelected(const Value: TTntListItem);
    function GetTopItem: TTntListItem;
  private
    FSavedItems: TObjectList;
    FTestingForSortProc: Boolean;
    FChangingWideItemCount: Integer;
    FTempItem: TTntListItem;
    function AreItemsStored: Boolean;
    function GetItems: TTntListItems;
    procedure SetItems(Value: TTntListItems);
    procedure CNNotify(var Message: TWMNotify); message CN_NOTIFY;
    function GetItemW(Value: TLVItemW): TTntListItem;
    procedure WMNotify(var Message: TWMNotify); message WM_NOTIFY;
  protected
    procedure CreateWindowHandle(const Params: TCreateParams); override;
    procedure DefineProperties(Filer: TFiler); override;
    function GetActionLinkClass: TControlActionLinkClass; override;
    procedure ActionChange(Sender: TObject; CheckDefaults: Boolean); override;
    procedure CreateWnd; override;
    procedure DestroyWnd; override;
    procedure WndProc(var Message: TMessage); override;
    function OwnerDataFetch(Item: TListItem{TNT-ALLOW TListItem}; Request: TItemRequest): Boolean; reintroduce; virtual;
    function CreateListItem: TListItem{TNT-ALLOW TListItem}; override;
    {$IFDEF COMPILER_6_UP}
    function CreateListItems: TListItems{TNT-ALLOW TListItems}; override;
    {$ENDIF}
    property Items: TTntListItems read GetItems write SetItems stored AreItemsStored;
    procedure Edit(const Item: TLVItem); override;
    function OwnerDataFind(Find: TItemFind; const FindString: WideString;
      const FindPosition: TPoint; FindData: Pointer; StartIndex: Integer;
      Direction: TSearchDirection; Wrap: Boolean): Integer; reintroduce; virtual;
    property Columns: TTntListColumns read GetListColumns write SetListColumns;
    procedure DrawItem(Item: TListItem{TNT-ALLOW TListItem}; Rect: TRect; State: TOwnerDrawState); override;
    property OnEdited: TTntLVEditedEvent read FOnEdited write FOnEdited;
    property OnDataFind: TTntLVOwnerDataFindEvent read FOnDataFind write FOnDataFind;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    property Column[Index: Integer]: TTntListColumn read ColumnFromIndex;
    {$IFDEF COMPILER_6_UP}
    procedure CopySelection(Destination: TCustomListControl); override;
    {$ENDIF}
    procedure AddItem(const Item: WideString; AObject: TObject); reintroduce; virtual;
    function FindCaption(StartIndex: Integer; Value: WideString; Partial,
      Inclusive, Wrap: Boolean): TTntListItem;
    function GetSearchString: WideString;
    function StringWidth(S: WideString): Integer;
  public
    property DropTarget: TTntListItem read GetDropTarget write SetDropTarget;
    property ItemFocused: TTntListItem read GetItemFocused write SetItemFocused;
    property Selected: TTntListItem read GetSelected write SetSelected;
    property TopItem: TTntListItem read GetTopItem;
  published
    property Hint: WideString read GetHint write SetHint stored IsHintStored;
  end;

{TNT-WARN TListView}
  TTntListView = class(TTntCustomListView)
  published
    property Action;
    property Align;
    property AllocBy;
    property Anchors;
    property BevelEdges;
    property BevelInner;
    property BevelOuter;
    property BevelKind default bkNone;
    property BevelWidth;
    property BiDiMode;
    property BorderStyle;
    property BorderWidth;
    property Checkboxes;
    property Color;
    property Columns;
    property ColumnClick;
    property Constraints;
    property Ctl3D;
    property DragCursor;
    property DragKind;
    property DragMode;
    property Enabled;
    property Font;
    property FlatScrollBars;
    property FullDrag;
    property GridLines;
    property HideSelection;
    property HotTrack;
    property HotTrackStyles;
    property HoverTime;
    property IconOptions;
    property Items;
    property LargeImages;
    property MultiSelect;
    property OwnerData;
    property OwnerDraw;
    property ReadOnly default False;
    property RowSelect;
    property ParentBiDiMode;
    property ParentColor default False;
    property ParentFont;
    property ParentShowHint;
    property PopupMenu;
    property ShowColumnHeaders;
    property ShowWorkAreas;
    property ShowHint;
    property SmallImages;
    property SortType;
    property StateImages;
    property TabOrder;
    property TabStop default True;
    property ViewStyle;
    property Visible;
    property OnAdvancedCustomDraw;
    property OnAdvancedCustomDrawItem;
    property OnAdvancedCustomDrawSubItem;
    property OnChange;
    property OnChanging;
    property OnClick;
    property OnColumnClick;
    property OnColumnDragged;
    property OnColumnRightClick;
    property OnCompare;
    property OnContextPopup;
    property OnCustomDraw;
    property OnCustomDrawItem;
    property OnCustomDrawSubItem;
    property OnData;
    property OnDataFind;
    property OnDataHint;
    property OnDataStateChange;
    property OnDblClick;
    property OnDeletion;
    property OnDrawItem;
    property OnEdited;
    property OnEditing;
    property OnEndDock;
    property OnEndDrag;
    property OnEnter;
    property OnExit;
    property OnGetImageIndex;
    property OnGetSubItemImage;
    property OnDragDrop;
    property OnDragOver;
    property OnInfoTip;
    property OnInsert;
    property OnKeyDown;
    property OnKeyPress;
    property OnKeyUp;
    property OnMouseDown;
    property OnMouseMove;
    property OnMouseUp;
    property OnResize;
    property OnSelectItem;
    property OnStartDock;
    property OnStartDrag;
  end;

type
{TNT-WARN TCustomRichEdit}
  TTntCustomRichEdit = class(TCustomRichEdit{TNT-ALLOW TCustomRichEdit})
  private
    FRichEditStrings: TTntStrings;
    FPrintingTextLength: Integer;
    procedure WMGetTextLength(var Message: TWMGetTextLength); message WM_GETTEXTLENGTH;
    procedure SetRichEditStrings(const Value: TTntStrings);
    function GetWideSelText: WideString;
    function GetText: WideString;
    procedure SetWideSelText(const Value: WideString);
    procedure SetText(const Value: WideString);
    function GetHint: WideString;
    function IsHintStored: Boolean;
    procedure SetHint(const Value: WideString);
    procedure SetRTFText(Flags: DWORD; const Value: AnsiString);
  protected
    procedure CreateParams(var Params: TCreateParams); override;
    procedure CreateWindowHandle(const Params: TCreateParams); override;
    procedure CreateWnd; override;
    procedure DefineProperties(Filer: TFiler); override;
    function GetActionLinkClass: TControlActionLinkClass; override;
    procedure ActionChange(Sender: TObject; CheckDefaults: Boolean); override;
    function GetSelText: string{TNT-ALLOW string}; override;
    function CharPosToGet(CharPos: Integer): Integer;
    function CharPosToSet(CharPos: Integer): Integer;
    function GetSelStart: Integer; reintroduce; virtual;
    procedure SetSelStart(const Value: Integer); reintroduce; virtual;
    function GetSelLength: Integer; reintroduce; virtual;
    procedure SetSelLength(const Value: Integer); reintroduce; virtual;
    function LineBreakStyle: TTntTextLineBreakStyle;
    property Lines: TTntStrings read FRichEditStrings write SetRichEditStrings;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure Print(const Caption: string{TNT-ALLOW string}); override;
    property SelText: WideString read GetWideSelText write SetWideSelText;
    property SelStart: Integer read GetSelStart write SetSelStart;
    property SelLength: Integer read GetSelLength write SetSelLength;
    property Text: WideString read GetText write SetText;
    function FindText(const SearchStr: WideString; StartPos,
      Length: Integer; Options: TSearchTypes): Integer;
  published
    property Hint: WideString read GetHint write SetHint stored IsHintStored;
  end;

{TNT-WARN TRichEdit}
  TTntRichEdit = class(TTntCustomRichEdit)
  published
    property Align;
    property Alignment;
    property Anchors;
    property BevelEdges;
    property BevelInner;
    property BevelOuter;
    property BevelKind default bkNone;
    property BevelWidth;
    property BiDiMode;
    property BorderStyle;
    property BorderWidth;
    property Color;
    property Ctl3D;
    property DragCursor;
    property DragKind;
    property DragMode;
    property Enabled;
    property Font;
    property HideSelection;
    property HideScrollBars;
    property ImeMode;
    property ImeName;
    property Constraints;
    property Lines;
    property MaxLength;
    property ParentBiDiMode;
    property ParentColor;
    property ParentCtl3D;
    property ParentFont;
    property ParentShowHint;
    property PlainText;
    property PopupMenu;
    property ReadOnly;
    property ScrollBars;
    property ShowHint;
    property TabOrder;
    property TabStop default True;
    property Visible;
    property WantTabs;
    property WantReturns;
    property WordWrap;
    property OnChange;
    property OnContextPopup;
    property OnDragDrop;
    property OnDragOver;
    property OnEndDock;
    property OnEndDrag;
    property OnEnter;
    property OnExit;
    property OnKeyDown;
    property OnKeyPress;
    property OnKeyUp;
    property OnMouseDown;
    property OnMouseMove;
    property OnMouseUp;
    property OnMouseWheel;
    property OnMouseWheelDown;
    property OnMouseWheelUp;
    property OnProtectChange;
    property OnResizeRequest;
    property OnSaveClipboard;
    property OnSelectionChange;
    property OnStartDock;
    property OnStartDrag;
  end;

type
{TNT-WARN TCustomTabControl}
  TTntCustomTabControl = class(TCustomTabControl{TNT-ALLOW TCustomTabControl})
  private
    FTabs: TTntStrings;
    FSaveTabIndex: Integer;
    FSaveTabs: TTntStrings;
    procedure SetTabs(const Value: TTntStrings);
    procedure CMDialogChar(var Message: TCMDialogChar); message CM_DIALOGCHAR;
    function GetHint: WideString;
    function IsHintStored: Boolean;
    procedure SetHint(const Value: WideString);
  protected
    procedure CreateWindowHandle(const Params: TCreateParams); override;
    procedure DefineProperties(Filer: TFiler); override;
    function GetActionLinkClass: TControlActionLinkClass; override;
    procedure ActionChange(Sender: TObject; CheckDefaults: Boolean); override;
    procedure CreateWnd; override;
    procedure DestroyWnd; override;
    property Tabs: TTntStrings read FTabs write SetTabs;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
  published
    property Hint: WideString read GetHint write SetHint stored IsHintStored;
  end;

{TNT-WARN TTabControl}
  TTntTabControl = class(TTntCustomTabControl)
  public
    property DisplayRect;
  published
    property Align;
    property Anchors;
    property BiDiMode;
    property Constraints;
    property DockSite;
    property DragCursor;
    property DragKind;
    property DragMode;
    property Enabled;
    property Font;
    property HotTrack;
    property Images;
    property MultiLine;
    property MultiSelect;
    property OwnerDraw;
    property ParentBiDiMode;
    property ParentFont;
    property ParentShowHint;
    property PopupMenu;
    property RaggedRight;
    property ScrollOpposite;
    property ShowHint;
    property Style;
    property TabHeight;
    property TabOrder;
    property TabPosition;
    property Tabs;
    property TabIndex;  // must be after Tabs
    property TabStop;
    property TabWidth;
    property Visible;
    property OnChange;
    property OnChanging;
    property OnContextPopup;
    property OnDockDrop;
    property OnDockOver;
    property OnDragDrop;
    property OnDragOver;
    property OnDrawTab;
    property OnEndDock;
    property OnEndDrag;
    property OnEnter;
    property OnExit;
    property OnGetImageIndex;
    property OnGetSiteInfo;
    property OnMouseDown;
    property OnMouseMove;
    property OnMouseUp;
    property OnResize;
    property OnStartDock;
    property OnStartDrag;
    property OnUnDock;
  end;

type
{TNT-WARN TTabSheet}
  TTntTabSheet = class(TTabSheet{TNT-ALLOW TTabSheet})
  private
    Force_Inherited_WMSETTEXT: Boolean;
    function IsCaptionStored: Boolean;
    function GetCaption: TWideCaption;
    procedure SetCaption(const Value: TWideCaption);
    procedure WMSetText(var Message: TWMSetText); message WM_SETTEXT;
    function GetHint: WideString;
    function IsHintStored: Boolean;
    procedure SetHint(const Value: WideString);
  protected
    procedure CreateWindowHandle(const Params: TCreateParams); override;
    procedure DefineProperties(Filer: TFiler); override;
    function GetActionLinkClass: TControlActionLinkClass; override;
    procedure ActionChange(Sender: TObject; CheckDefaults: Boolean); override;
  published
    property Caption: TWideCaption read GetCaption write SetCaption stored IsCaptionStored;
    property Hint: WideString read GetHint write SetHint stored IsHintStored;
  end;

{TNT-WARN TPageControl}
  TTntPageControl = class(TPageControl{TNT-ALLOW TPageControl})
  private
    FNewDockSheet: TTntTabSheet;
    function IsHintStored: Boolean;
    function GetHint: WideString;
    procedure SetHint(const Value: WideString);
    procedure CMDialogChar(var Message: TCMDialogChar); message CM_DIALOGCHAR;
    procedure CMDockNotification(var Message: TCMDockNotification); message CM_DOCKNOTIFICATION;
    procedure CMDockClient(var Message: TCMDockClient); message CM_DOCKCLIENT;
  protected
    procedure CreateWindowHandle(const Params: TCreateParams); override;
    procedure DefineProperties(Filer: TFiler); override;
    function GetActionLinkClass: TControlActionLinkClass; override;
    procedure ActionChange(Sender: TObject; CheckDefaults: Boolean); override;
    procedure WndProc(var Message: TMessage); override;
    procedure DoAddDockClient(Client: TControl; const ARect: TRect); override;
  published
    property Hint: WideString read GetHint write SetHint stored IsHintStored;
  end;

{TNT-WARN TTrackBar}
  TTntTrackBar = class(TTrackBar{TNT-ALLOW TTrackBar})
  private
    function IsHintStored: Boolean;
    function GetHint: WideString;
    procedure SetHint(const Value: WideString);
  protected
    procedure CreateWindowHandle(const Params: TCreateParams); override;
    procedure DefineProperties(Filer: TFiler); override;
    function GetActionLinkClass: TControlActionLinkClass; override;
    procedure ActionChange(Sender: TObject; CheckDefaults: Boolean); override;
  published
    property Hint: WideString read GetHint write SetHint stored IsHintStored;
  end;

{TNT-WARN TProgressBar}
  TTntProgressBar = class(TProgressBar{TNT-ALLOW TProgressBar})
  private
    function IsHintStored: Boolean;
    function GetHint: WideString;
    procedure SetHint(const Value: WideString);
  protected
    procedure CreateWindowHandle(const Params: TCreateParams); override;
    procedure DefineProperties(Filer: TFiler); override;
    function GetActionLinkClass: TControlActionLinkClass; override;
    procedure ActionChange(Sender: TObject; CheckDefaults: Boolean); override;
  published
    property Hint: WideString read GetHint write SetHint stored IsHintStored;
  end;

{TNT-WARN TCustomUpDown}
  TTntCustomUpDown = class(TCustomUpDown{TNT-ALLOW TCustomUpDown})
  private
    function IsHintStored: Boolean;
    function GetHint: WideString;
    procedure SetHint(const Value: WideString);
  protected
    procedure CreateWindowHandle(const Params: TCreateParams); override;
    procedure DefineProperties(Filer: TFiler); override;
    function GetActionLinkClass: TControlActionLinkClass; override;
    procedure ActionChange(Sender: TObject; CheckDefaults: Boolean); override;
  published
    property Hint: WideString read GetHint write SetHint stored IsHintStored;
  end;

{TNT-WARN TUpDown}
  TTntUpDown = class(TTntCustomUpDown)
  published
    property AlignButton;
    property Anchors;
    property Associate;
    property ArrowKeys;
    property Enabled;
    property Hint;
    property Min;
    property Max;
    property Increment;
    property Constraints;
    property Orientation;
    property ParentShowHint;
    property PopupMenu;
    property Position;
    property ShowHint;
    property TabOrder;
    property TabStop;
    property Thousands;
    property Visible;
    property Wrap;
    property OnChanging;
    property OnChangingEx;
    property OnContextPopup;
    property OnClick;
    property OnEnter;
    property OnExit;
    property OnMouseDown;
    property OnMouseMove;
    property OnMouseUp;
  end;

{TNT-WARN TDateTimePicker}
  TTntDateTimePicker = class(TDateTimePicker{TNT-ALLOW TDateTimePicker})
  private
    function IsHintStored: Boolean;
    function GetHint: WideString;
    procedure SetHint(const Value: WideString);
  protected
    procedure CreateWindowHandle(const Params: TCreateParams); override;
    procedure DefineProperties(Filer: TFiler); override;
    function GetActionLinkClass: TControlActionLinkClass; override;
    procedure ActionChange(Sender: TObject; CheckDefaults: Boolean); override;
  published
    property Hint: WideString read GetHint write SetHint stored IsHintStored;
  end;

{TNT-WARN TMonthCalendar}
  TTntMonthCalendar = class(TMonthCalendar{TNT-ALLOW TMonthCalendar})
  private
    function IsHintStored: Boolean;
    function GetHint: WideString;
    procedure SetHint(const Value: WideString);
    function GetDate: TDate;
    procedure SetDate(const Value: TDate);
  protected
    procedure CreateWindowHandle(const Params: TCreateParams); override;
    procedure DefineProperties(Filer: TFiler); override;
    function GetActionLinkClass: TControlActionLinkClass; override;
    procedure ActionChange(Sender: TObject; CheckDefaults: Boolean); override;
  public
    procedure ForceGetMonthInfo;
  published
    property Date: TDate read GetDate write SetDate;
    property Hint: WideString read GetHint write SetHint stored IsHintStored;
  end;

{TNT-WARN TPageScroller}
  TTntPageScroller = class(TPageScroller{TNT-ALLOW TPageScroller})
  private
    function IsHintStored: Boolean;
    function GetHint: WideString;
    procedure SetHint(const Value: WideString);
  protected
    procedure CreateWindowHandle(const Params: TCreateParams); override;
    procedure DefineProperties(Filer: TFiler); override;
    function GetActionLinkClass: TControlActionLinkClass; override;
    procedure ActionChange(Sender: TObject; CheckDefaults: Boolean); override;
  published
    property Hint: WideString read GetHint write SetHint stored IsHintStored;
  end;

type
{TNT-WARN TStatusPanel}
  TTntStatusPanel = class(TStatusPanel{TNT-ALLOW TStatusPanel})
  private
    FText: WideString;
    function GetText: Widestring;
    procedure SetText(const Value: Widestring);
    procedure SetInheritedText(const Value: AnsiString);
  protected
    procedure DefineProperties(Filer: TFiler); override;
  public
    procedure Assign(Source: TPersistent); override;
  published
    property Text: Widestring read GetText write SetText;
  end;

{TNT-WARN TStatusPanels}
  TTntStatusPanels = class(TStatusPanels{TNT-ALLOW TStatusPanels})
  private
    function GetItem(Index: Integer): TTntStatusPanel;
    procedure SetItem(Index: Integer; Value: TTntStatusPanel);
  public
    {$IFNDEF COMPILER_6_UP}
    constructor Create(StatusBar: TStatusBar{TNT-ALLOW TStatusBar});
    {$ENDIF}
    function Add: TTntStatusPanel;
    {$IFDEF COMPILER_6_UP}
    function AddItem(Item: TTntStatusPanel; Index: Integer): TTntStatusPanel;
    function Insert(Index: Integer): TTntStatusPanel;
    {$ENDIF}
    property Items[Index: Integer]: TTntStatusPanel read GetItem write SetItem; default;
  end;

{$IFNDEF COMPILER_6_UP} // Delphi 5 compatibility
  TCustomStatusBar{TNT-ALLOW TCustomStatusBar} = class(TStatusBar{TNT-ALLOW TStatusBar});
{$ENDIF}

{TNT-WARN TCustomStatusBar}
  TTntCustomStatusBar = class(TCustomStatusBar{TNT-ALLOW TCustomStatusBar})
  private
    FSimpleText: WideString;
    function GetSimpleText: WideString;
    procedure SetSimpleText(const Value: WideString);
    procedure SetInheritedSimpleText(const Value: AnsiString);
    function SyncLeadingTabs(const WideVal: WideString; const AnsiVal: AnsiString): WideString;
    function GetHint: WideString;
    function IsHintStored: Boolean;
    procedure SetHint(const Value: WideString);
    procedure WMGetTextLength(var Message: TWMGetTextLength); message WM_GETTEXTLENGTH;
    function GetPanels: TTntStatusPanels;
    procedure SetPanels(const Value: TTntStatusPanels);
  protected
    procedure DefineProperties(Filer: TFiler); override;
    {$IFDEF COMPILER_6_UP}
    function CreatePanels: TStatusPanels{TNT-ALLOW TStatusPanels}; override;
    function GetPanelClass: TStatusPanelClass; override;
    {$ENDIF}
    procedure CreateWindowHandle(const Params: TCreateParams); override;
    procedure WndProc(var Msg: TMessage); override;
    function GetActionLinkClass: TControlActionLinkClass; override;
    procedure ActionChange(Sender: TObject; CheckDefaults: Boolean); override;
  public
    constructor Create(AOwner: TComponent); override;
    {$IFNDEF COMPILER_6_UP}
    procedure FlipChildren(AllLevels: Boolean); override;
    {$ENDIF}
    function ExecuteAction(Action: TBasicAction): Boolean; override;
    property Panels: TTntStatusPanels read GetPanels write SetPanels;
    property SimpleText: WideString read GetSimpleText write SetSimpleText;
  published
    property Hint: WideString read GetHint write SetHint stored IsHintStored;
  end;

{TNT-WARN TStatusBar}
  TTntStatusBar = class(TTntCustomStatusBar{TNT-ALLOW TStatusBar})
  private
    {$IFDEF COMPILER_6_UP}
    function GetOnDrawPanel: TDrawPanelEvent;
    procedure SetOnDrawPanel(const Value: TDrawPanelEvent);
    {$ELSE}
    function IsFontStored: Boolean;
    {$ENDIF}
  published
    property Action;
    property AutoHint default False;
    property Align default alBottom;
    property Anchors;
    property BiDiMode;
    property BorderWidth;
    property Color default clBtnFace;
    property DragCursor;
    property DragKind;
    property DragMode;
    property Enabled;
    property Font stored IsFontStored;
    property Constraints;
    property Panels;
    property ParentBiDiMode;
    property ParentColor default False;
    property ParentFont default False;
    property ParentShowHint;
    property PopupMenu;
    property ShowHint;
    property SimplePanel {$IFDEF COMPILER_7_UP} default False {$ENDIF};
    property SimpleText;
    property SizeGrip default True;
    property UseSystemFont default True;
    property Visible;
    property OnClick;
    property OnContextPopup;
    {$IFDEF COMPILER_6_UP}
    property OnCreatePanelClass;
    {$ENDIF}
    property OnDblClick;
    property OnDragDrop;
    property OnDragOver;
    property OnEndDock;
    property OnEndDrag;
    property OnHint;
    property OnMouseDown;
    property OnMouseMove;
    property OnMouseUp;
    {$IFDEF COMPILER_6_UP}
    // Required for backwards compatibility with the old event signature
    property OnDrawPanel: TDrawPanelEvent read GetOnDrawPanel write SetOnDrawPanel;
    {$ELSE}
    property OnDrawPanel;
    {$ENDIF}
    property OnResize;
    property OnStartDock;
    property OnStartDrag;
  end;

type
  TTntTreeNodes = class;
  TTntCustomTreeView = class;

{TNT-WARN TTreeNode}
  TTntTreeNode = class(TTreeNode{TNT-ALLOW TTreeNode})
  private
    FText: WideString;
    procedure SetText(const Value: WideString);
    procedure SetInheritedText(const Value: AnsiString);
    function GetText: WideString;
    function GetItem(Index: Integer): TTntTreeNode;
    function GetNodeOwner: TTntTreeNodes;
    function GetParent: TTntTreeNode;
    function GetTreeView: TTntCustomTreeView;
    procedure SetItem(Index: Integer; const Value: TTntTreeNode);
  public
    procedure Assign(Source: TPersistent); override;
    function getFirstChild: TTntTreeNode; {GetFirstChild conflicts with C++ macro}
    function GetLastChild: TTntTreeNode;
    function GetNext: TTntTreeNode;
    function GetNextChild(Value: TTntTreeNode): TTntTreeNode;
    function getNextSibling: TTntTreeNode; {GetNextSibling conflicts with C++ macro}
    function GetNextVisible: TTntTreeNode;
    function GetPrev: TTntTreeNode;
    function GetPrevChild(Value: TTntTreeNode): TTntTreeNode;
    function getPrevSibling: TTntTreeNode; {GetPrevSibling conflicts with a C++ macro}
    function GetPrevVisible: TTntTreeNode;
    property Item[Index: Integer]: TTntTreeNode read GetItem write SetItem; default;
    property Owner: TTntTreeNodes read GetNodeOwner;
    property Parent: TTntTreeNode read GetParent;
    property Text: WideString read GetText write SetText;
    property TreeView: TTntCustomTreeView read GetTreeView;
  end;

  TTntTreeNodeClass = class of TTntTreeNode;

{TNT-WARN TTreeNodes}
  TTntTreeNodes = class(TTreeNodes{TNT-ALLOW TTreeNodes})
  private
    function GetNodeFromIndex(Index: Integer): TTntTreeNode;
    function GetNodesOwner: TTntCustomTreeView;
  public
    procedure Assign(Source: TPersistent); override;
    function Add(Sibling: TTntTreeNode; const S: WideString): TTntTreeNode;
    function AddChild(Parent: TTntTreeNode; const S: WideString): TTntTreeNode;
    function AddChildFirst(Parent: TTntTreeNode; const S: WideString): TTntTreeNode;
    function AddChildObject(Parent: TTntTreeNode; const S: WideString;
      Ptr: Pointer): TTntTreeNode;
    function AddChildObjectFirst(Parent: TTntTreeNode; const S: WideString;
      Ptr: Pointer): TTntTreeNode;
    function AddFirst(Sibling: TTntTreeNode; const S: WideString): TTntTreeNode;
    function AddObject(Sibling: TTntTreeNode; const S: WideString;
      Ptr: Pointer): TTntTreeNode;
    function AddObjectFirst(Sibling: TTntTreeNode; const S: WideString;
      Ptr: Pointer): TTntTreeNode;
    function Insert(Sibling: TTntTreeNode; const S: WideString): TTntTreeNode;
    function InsertObject(Sibling: TTntTreeNode; const S: WideString;
      Ptr: Pointer): TTntTreeNode;
    {$IFDEF COMPILER_6_UP}
    function InsertNode(Node, Sibling: TTntTreeNode; const S: WideString;
      Ptr: Pointer): TTntTreeNode;
    function AddNode(Node, Relative: TTntTreeNode; const S: WideString;
      Ptr: Pointer; Method: TNodeAttachMode): TTntTreeNode;
    {$ELSE}
    function InternalAddObject(Node: TTntTreeNode; const S: WideString;
      Ptr: Pointer; AddMode: TAddMode): TTntTreeNode;
    {$ENDIF}
  public
    function GetFirstNode: TTntTreeNode;
    function GetNode(ItemId: HTreeItem): TTntTreeNode;
    property Item[Index: Integer]: TTntTreeNode read GetNodeFromIndex; default;
    property Owner: TTntCustomTreeView read GetNodesOwner;
  end;

  TTntTVEditedEvent = procedure(Sender: TObject; Node: TTntTreeNode; var S: WideString) of object;

{TNT-WARN TCustomTreeView}
  _TntInternalCustomTreeView = class(TCustomTreeView{TNT-ALLOW TCustomTreeView})
  {$IFDEF COMPILER_6_UP}
  private
    function Wide_FindNextToSelect: TTntTreeNode; virtual; abstract;
    function Inherited_FindNextToSelect: TTreeNode{TNT-ALLOW TTreeNode};
  public
    function FindNextToSelect: TTreeNode{TNT-ALLOW TTreeNode}; override;
  {$ENDIF}
  end;

  TTntCustomTreeView = class(_TntInternalCustomTreeView)
  private
    FSavedNodeText: TTntStrings;
    FOnEdited: TTntTVEditedEvent;
    FTestingForSortProc: Boolean;
    FEditHandle: THandle;
    FEditInstance: Pointer;
    FDefEditProc: Pointer;
    function GetTreeNodes: TTntTreeNodes;
    procedure SetTreeNodes(const Value: TTntTreeNodes);
    procedure CNNotify(var Message: TWMNotify); message CN_NOTIFY;
    procedure WMNotify(var Message: TWMNotify); message WM_NOTIFY;
    function GetNodeFromItem(const Item: TTVItem): TTntTreeNode;
    procedure EditWndProcW(var Message: TMessage);
    {$IFDEF COMPILER_6_UP}
    function Wide_FindNextToSelect: TTntTreeNode; override;
    {$ENDIF}
    function GetDropTarget: TTntTreeNode;
    function GetSelected: TTntTreeNode;
    {$IFDEF COMPILER_6_UP}
    function GetSelection(Index: Integer): TTntTreeNode;
    {$ENDIF}
    function GetTopItem: TTntTreeNode;
    procedure SetDropTarget(const Value: TTntTreeNode);
    procedure SetSelected(const Value: TTntTreeNode);
    procedure SetTopItem(const Value: TTntTreeNode);
    function GetHint: WideString;
    function IsHintStored: Boolean;
    procedure SetHint(const Value: WideString);
  protected
    procedure ActionChange(Sender: TObject; CheckDefaults: Boolean); override;
    function GetActionLinkClass: TControlActionLinkClass; override;
    procedure CreateWindowHandle(const Params: TCreateParams); override;
    procedure CreateWnd; override;
    procedure DestroyWnd; override;
    procedure DefineProperties(Filer: TFiler); override;
    procedure WndProc(var Message: TMessage); override;
    procedure Edit(const Item: TTVItem); override;
    function CreateNode: TTreeNode{TNT-ALLOW TTreeNode}; override;
    {$IFDEF COMPILER_6_UP}
    function CreateNodes: TTreeNodes{TNT-ALLOW TTreeNodes}; override;
    {$ENDIF}
    property Items: TTntTreeNodes read GetTreeNodes write SetTreeNodes;
    property OnEdited: TTntTVEditedEvent read FOnEdited write FOnEdited;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure LoadFromFile(const FileName: WideString);
    procedure LoadFromStream(Stream: TStream);
    procedure SaveToFile(const FileName: WideString);
    procedure SaveToStream(Stream: TStream);
    function GetNodeAt(X, Y: Integer): TTntTreeNode;
    property DropTarget: TTntTreeNode read GetDropTarget write SetDropTarget;
    property Selected: TTntTreeNode read GetSelected write SetSelected;
    property TopItem: TTntTreeNode read GetTopItem write SetTopItem;
    {$IFDEF COMPILER_6_UP}
    property Selections[Index: Integer]: TTntTreeNode read GetSelection;
    function GetSelections(AList: TList): TTntTreeNode;
    function FindNextToSelect: TTntTreeNode; reintroduce; virtual;
    {$ENDIF}
  published
    property Hint: WideString read GetHint write SetHint stored IsHintStored;
  end;

{TNT-WARN TTreeView}
  TTntTreeView = class(TTntCustomTreeView)
  published
    property Align;
    property Anchors;
    property AutoExpand;
    property BevelEdges;
    property BevelInner;
    property BevelOuter;
    property BevelKind default bkNone;
    property BevelWidth;
    property BiDiMode;
    property BorderStyle;
    property BorderWidth;
    property ChangeDelay;
    property Color;
    property Ctl3D;
    property Constraints;
    property DragKind;
    property DragCursor;
    property DragMode;
    property Enabled;
    property Font;
    property HideSelection;
    property HotTrack;
    property Images;
    property Indent;
    {$IFDEF COMPILER_6_UP}
    property MultiSelect;
    property MultiSelectStyle;
    {$ENDIF}
    property ParentBiDiMode;
    property ParentColor default False;
    property ParentCtl3D;
    property ParentFont;
    property ParentShowHint;
    property PopupMenu;
    property ReadOnly;
    property RightClickSelect;
    property RowSelect;
    property ShowButtons;
    property ShowHint;
    property ShowLines;
    property ShowRoot;
    property SortType;
    property StateImages;
    property TabOrder;
    property TabStop default True;
    property ToolTips;
    property Visible;
    {$IFDEF COMPILER_6_UP}
    property OnAddition;
    {$ENDIF}
    property OnAdvancedCustomDraw;
    property OnAdvancedCustomDrawItem;
    property OnChange;
    property OnChanging;
    property OnClick;
    property OnCollapsed;
    property OnCollapsing;
    property OnCompare;
    property OnContextPopup;
    {$IFDEF COMPILER_6_UP}
    property OnCreateNodeClass;
    {$ENDIF}
    property OnCustomDraw;
    property OnCustomDrawItem;
    property OnDblClick;
    property OnDeletion;
    property OnDragDrop;
    property OnDragOver;
    property OnEdited;
    property OnEditing;
    property OnEndDock;
    property OnEndDrag;
    property OnEnter;
    property OnExit;
    property OnExpanding;
    property OnExpanded;
    property OnGetImageIndex;
    property OnGetSelectedIndex;
    property OnKeyDown;
    property OnKeyPress;
    property OnKeyUp;
    property OnMouseDown;
    property OnMouseMove;
    property OnMouseUp;
    property OnStartDock;
    property OnStartDrag;
    { Items must be published after OnGetImageIndex and OnGetSelectedIndex }
    property Items;
  end;

implementation

uses
  SysUtils, Forms, TntGraphics, ImgList, TntSystem, TntStdCtrls, StdCtrls,
  RichEdit, ActiveIMM_TLB, Printers, TntForms, ComStrs,
  TntActnList, TntStdActns;

procedure CreateUnicodeHandle_ComCtl(Control: TWinControl; const Params: TCreateParams;
  const SubClass: WideString);
begin
  Assert(SubClass <> '', 'TNT Internal Error: Only call CreateUnicodeHandle_ComCtl for Common Controls.');
  CreateUnicodeHandle(Control, Params, SubClass);
  if Win32PlatformIsUnicode then
    SendMessageW(Control.Handle, CCM_SETUNICODEFORMAT, Integer(True), 0);
end;

{ TTntListColumn }

procedure TTntListColumn.Assign(Source: TPersistent);
begin
  inherited;
  if Source is TTntListColumn then
    Caption := TTntListColumn(Source).Caption
  else if Source is TListColumn{TNT-ALLOW TListColumn} then
    FCaption := TListColumn{TNT-ALLOW TListColumn}(Source).Caption;
end;

procedure TTntListColumn.DefineProperties(Filer: TFiler);
begin
  inherited;
  TntPersistent_AfterInherited_DefineProperties(Filer, Self);
end;

procedure TTntListColumn.SetInheritedCaption(const Value: AnsiString);
begin
  inherited Caption := Value;
end;

function TTntListColumn.GetCaption: WideString;
begin
  Result := GetSyncedWideString(FCaption, inherited Caption);
end;

procedure TTntListColumn.SetCaption(const Value: WideString);
begin
  SetSyncedWideString(Value, FCaption, inherited Caption, SetInheritedCaption);
end;

{ TTntListColumns }

{$IFDEF COMPILER_5} // verified against VCL source in Delphi 5 and BCB 5
type
  THackCollection = class(TPersistent)
  protected
    FItemClass: TCollectionItemClass;
  end;
{$ENDIF}
{$IFDEF COMPILER_6} // verified against VCL source in Delphi 6 and BCB 6
type
  THackCollection = class(TPersistent)
  protected
    FItemClass: TCollectionItemClass;
  end;
{$ENDIF}
{$IFDEF DELPHI_7}
type
  THackCollection = class(TPersistent)
  protected
    FItemClass: TCollectionItemClass;
  end;
{$ENDIF}

constructor TTntListColumns.Create(AOwner: TTntCustomListView);
begin
  inherited Create(AOwner);
  Assert(THackCollection(Self).FItemClass = Self.ItemClass, 'Internal Error in TTntListColumns.Create().');
  THackCollection(Self).FItemClass := TTntListColumn
end;

function TTntListColumns.Owner: TTntCustomListView;
begin
  Result := inherited Owner as TTntCustomListView;
end;

function TTntListColumns.Add: TTntListColumn;
begin
  Result := (inherited Add) as TTntListColumn;
end;

function TTntListColumns.GetItem(Index: Integer): TTntListColumn;
begin
  Result := inherited Items[Index] as TTntListColumn;
end;

procedure TTntListColumns.SetItem(Index: Integer; Value: TTntListColumn);
begin
  inherited SetItem(Index, Value);
end;

{ TWideSubItems }
type
  TWideSubItems = class(TTntStringList)
  private
    FIgnoreInherited: Boolean;
    FInheritedOwner: TListItem{TNT-ALLOW TListItem};
    FOwner: TTntListItem;
  protected
    procedure Put(Index: Integer; const S: WideString); override;
    function GetObject(Index: Integer): TObject; override;
    procedure PutObject(Index: Integer; AObject: TObject); override;
    procedure SetUpdateState(Updating: Boolean); override;
  public
    procedure Insert(Index: Integer; const S: WideString); override;
    function AddObject(const S: WideString; AObject: TObject): Integer; override;
    procedure Clear; override;
    procedure Delete(Index: Integer); override;
  public
    constructor Create(AOwner: TTntListItem);
  end;

constructor TWideSubItems.Create(AOwner: TTntListItem);
begin
  inherited Create;
  FInheritedOwner := AOwner;
  FOwner := AOwner;
end;

function TWideSubItems.AddObject(const S: WideString; AObject: TObject): Integer;
begin
  FOwner.ListView.BeginChangingWideItem;
  try
    Result := inherited AddObject(S, AObject);
    if (not FIgnoreInherited) then
      FInheritedOwner.SubItems.AddObject(S, AObject);
  finally
    FOwner.ListView.EndChangingWideItem;
  end;
end;

procedure TWideSubItems.Clear;
begin
  FOwner.ListView.BeginChangingWideItem;
  try
    inherited;
    if (not FIgnoreInherited) then
      FInheritedOwner.SubItems.Clear;
  finally
    FOwner.ListView.EndChangingWideItem;
  end;
end;

procedure TWideSubItems.Delete(Index: Integer);
begin
  FOwner.ListView.BeginChangingWideItem;
  try
    inherited;
    if (not FIgnoreInherited) then
      FInheritedOwner.SubItems.Delete(Index);
  finally
    FOwner.ListView.EndChangingWideItem;
  end;
end;

procedure TWideSubItems.Insert(Index: Integer; const S: WideString);
begin
  FOwner.ListView.BeginChangingWideItem;
  try
    inherited;
    if (not FIgnoreInherited) then
      FInheritedOwner.SubItems.Insert(Index, S);
  finally
    FOwner.ListView.EndChangingWideItem;
  end;
end;

procedure TWideSubItems.Put(Index: Integer; const S: WideString);
begin
  FOwner.ListView.BeginChangingWideItem;
  try
    inherited;
    if (not FIgnoreInherited) then
      FInheritedOwner.SubItems[Index] := S;
  finally
    FOwner.ListView.EndChangingWideItem;
  end;
end;

function TWideSubItems.GetObject(Index: Integer): TObject;
begin
  Result := FInheritedOwner.SubItems.Objects[Index];
end;

procedure TWideSubItems.PutObject(Index: Integer; AObject: TObject);
begin
  FInheritedOwner.SubItems.Objects[Index] := AObject;
end;

type TAccessStrings = class(TStrings{TNT-ALLOW TStrings});

procedure TWideSubItems.SetUpdateState(Updating: Boolean);
begin
  inherited;
  TAccessStrings(FInheritedOwner.SubItems).SetUpdateState(Updating);
end;

{ TTntListItem }

constructor TTntListItem.Create(AOwner: TListItems{TNT-ALLOW TListItems});
begin
  inherited Create(AOwner);
  FSubItems := TWideSubItems.Create(Self);
end;

destructor TTntListItem.Destroy;
begin
  inherited;
  FreeAndNil(FSubItems);
end;

function TTntListItem.GetCaption: WideString;
begin
  Result := GetSyncedWideString(FCaption, inherited Caption);
end;

procedure TTntListItem.SetInheritedCaption(const Value: AnsiString);
begin
  inherited Caption := Value;
end;

procedure TTntListItem.SetCaption(const Value: WideString);
begin
  ListView.BeginChangingWideItem;
  try
    SetSyncedWideString(Value, FCaption, inherited Caption, SetInheritedCaption);
  finally
    ListView.EndChangingWideItem;
  end;
end;

procedure TTntListItem.Assign(Source: TPersistent);
begin
  if Source is TTntListItem then
    with Source as TTntListItem do
    begin
      Self.Caption := Caption;
      Self.Data := Data;
      Self.ImageIndex := ImageIndex;
      Self.Indent := Indent;
      Self.OverlayIndex := OverlayIndex;
      Self.StateIndex := StateIndex;
      Self.SubItems := SubItems;
      Self.Checked := Checked;
    end
  else inherited Assign(Source);
end;

procedure TTntListItem.SetSubItems(const Value: TTntStrings);
begin
  if Value <> nil then
    FSubItems.Assign(Value);
end;

function TTntListItem.GetTntOwner: TTntListItems;
begin
  Result := ListView.Items;
end;

function TTntListItem.GetListView: TTntCustomListView;
begin
  Result := ((inherited Owner).Owner as TTntCustomListView);
end;

{ TTntListItems }

function TTntListItems.Add: TTntListItem;
begin
  Result := (inherited Add) as TTntListItem;
end;

{$IFDEF COMPILER_6_UP}
function TTntListItems.AddItem(Item: TTntListItem; Index: Integer): TTntListItem;
begin
  Result := (inherited AddItem(Item, Index)) as TTntListItem;
end;
{$ENDIF}

function TTntListItems.Insert(Index: Integer): TTntListItem;
begin
  Result := (inherited Insert(Index)) as TTntListItem;
end;

function TTntListItems.GetItem(Index: Integer): TTntListItem;
begin
  Result := (inherited Item[Index]) as TTntListItem;
end;

function TTntListItems.Owner: TTntCustomListView;
begin
  Result := (inherited Owner) as TTntCustomListView;
end;

procedure TTntListItems.SetItem(Index: Integer; const Value: TTntListItem);
begin
  inherited Item[Index] := Value;
end;

{ TSavedListItem }
type
  TSavedListItem = class
    FCaption: WideString;
    FSubItems: TTntStrings;
    constructor Create;
    destructor Destroy; override;
  end;

constructor TSavedListItem.Create;
begin
  inherited;
  FSubItems := TTntStringList.Create;
end;

destructor TSavedListItem.Destroy;
begin
  FSubItems.Free;
  inherited;
end;

{ _TntInternalCustomListView }

function _TntInternalCustomListView.OwnerDataFind(Find: TItemFind;
  const FindString: AnsiString; const FindPosition: TPoint;
  FindData: Pointer; StartIndex: Integer; Direction: TSearchDirection;
  Wrap: Boolean): Integer;
var
  WideFindString: WideString;
begin
  if Assigned(PWideFindString) then
    WideFindString := PWideFindString
  else
    WideFindString := FindString;
  Result := OwnerDataFindW(Find, WideFindString, FindPosition, FindData, StartIndex, Direction, Wrap);
end;

function _TntInternalCustomListView.OwnerDataFetch(Item: TListItem{TNT-ALLOW TListItem};
  Request: TItemRequest): Boolean;
begin
  if  (CurrentDispInfo <> nil)
  and (OriginalDispInfoMask and LVIF_TEXT <> 0) then begin
    (Item as TTntListItem).FCaption := CurrentDispInfo.item.pszText
  end;
  (Item as TTntListItem).FSubItems.Clear;
  Result := OwnerDataFetchW(Item, Request);
end;

{ TTntCustomListView }

{$IFDEF COMPILER_5} // verified against VCL source in Delphi 5 and BCB 5
type
  THackCustomListView = class(TWinControl)
  protected
    FxxxCanvas: TCanvas;
    FxxxBorderStyle: TBorderStyle;
    FxxxViewStyle: TViewStyle;
    FxxxReadOnly: Boolean;
    FxxxLargeImages: TCustomImageList;
    FxxxSmallImages: TCustomImageList;
    FxxxStateImages: TCustomImageList;
    FxxxDragImage: TDragImageList;
    FxxxMultiSelect: Boolean;
    FxxxSortType: TSortType;
    FxxxColumnClick: Boolean;
    FxxxShowColumnHeaders: Boolean;
    FListItems: TListItems{TNT-ALLOW TListItems};
    FxxxClicked: Boolean;
    FxxxRClicked: Boolean;
    FxxxIconOptions: TIconOptions;
    FxxxHideSelection: Boolean;
    FListColumns: TListColumns{TNT-ALLOW TListColumns};
  end;
{$ENDIF}
{$IFDEF COMPILER_6} // verified against VCL source in Delphi 6 and BCB 6
type
  THackCustomListView = class(TCustomMultiSelectListControl)
  protected
    FxxxCanvas: TCanvas;
    FxxxBorderStyle: TBorderStyle;
    FxxxViewStyle: TViewStyle;
    FxxxReadOnly: Boolean;
    FxxxLargeImages: TCustomImageList;
    FxxxSmallImages: TCustomImageList;
    FxxxStateImages: TCustomImageList;
    FxxxDragImage: TDragImageList;
    FxxxMultiSelect: Boolean;
    FxxxSortType: TSortType;
    FxxxColumnClick: Boolean;
    FxxxShowColumnHeaders: Boolean;
    FxxxListItems: TListItems{TNT-ALLOW TListItems};
    FxxxClicked: Boolean;
    FxxxRClicked: Boolean;
    FxxxIconOptions: TIconOptions;
    FxxxHideSelection: Boolean;
    FListColumns: TListColumns{TNT-ALLOW TListColumns};
  end;
{$ENDIF}
{$IFDEF DELPHI_7}
type
  THackCustomListView = class(TCustomMultiSelectListControl)
  protected
    FxxxCanvas: TCanvas;
    FxxxBorderStyle: TBorderStyle;
    FxxxViewStyle: TViewStyle;
    FxxxReadOnly: Boolean;
    FxxxLargeImages: TCustomImageList;
    FxxxSmallImages: TCustomImageList;
    FxxxStateImages: TCustomImageList;
    FxxxDragImage: TDragImageList;
    FxxxMultiSelect: Boolean;
    FxxxSortType: TSortType;
    FxxxColumnClick: Boolean;
    FxxxShowColumnHeaders: Boolean;
    FxxxListItems: TListItems{TNT-ALLOW TListItems};
    FxxxClicked: Boolean;
    FxxxRClicked: Boolean;
    FxxxIconOptions: TIconOptions;
    FxxxHideSelection: Boolean;
    FListColumns: TListColumns{TNT-ALLOW TListColumns};
  end;
{$ENDIF}

var
  ComCtrls_DefaultListViewSort: TLVCompare = nil;

{$IFDEF COMPILER_5}
type
  THackListItem = class(TPersistent)
  private
    FOwner: TListItems{TNT-ALLOW TListItems};
  end;
{$ENDIF}

constructor TTntCustomListView.Create(AOwner: TComponent);
begin
  inherited;
  FEditInstance := TntClasses.MakeObjectInstance(EditWndProcW);
  {$IFNDEF COMPILER_6_UP}
  // create list items
  Assert(THackCustomListView(Self).FListItems = inherited Items, 'Internal Error in TTntCustomListView.Create().');
  FreeAndNil(THackCustomListView(Self).FListItems);
  THackCustomListView(Self).FListItems := TTntListItems.Create(Self);
  Assert(FTempItem <> nil);
  THackListItem(FTempItem).FOwner := Items;
  {$ENDIF}
  // create list columns
  Assert(THackCustomListView(Self).FListColumns = inherited Columns, 'Internal Error in TTntCustomListView.Create().');
  FreeAndNil(THackCustomListView(Self).FListColumns);
  THackCustomListView(Self).FListColumns := TTntListColumns.Create(Self);
end;

destructor TTntCustomListView.Destroy;
begin
  inherited;
  TntClasses.FreeObjectInstance(FEditInstance);
end;

procedure TTntCustomListView.CreateWindowHandle(const Params: TCreateParams);

  procedure Capture_ComCtrls_DefaultListViewSort;
  begin
    FTestingForSortProc := True;
    try
      AlphaSort;
    finally
      FTestingForSortProc := False;
    end;
  end;

var
  Column: TLVColumn;
begin
  CreateUnicodeHandle_ComCtl(Self, Params, WC_LISTVIEW);
  if (Win32PlatformIsUnicode) then begin
    if not Assigned(ComCtrls_DefaultListViewSort) then
      Capture_ComCtrls_DefaultListViewSort;
    // the only way I could get editing to work is after a column had been inserted
    Column.mask := 0;
    ListView_InsertColumn(Handle, 0, Column);
    ListView_DeleteColumn(Handle, 0);
  end;
end;

procedure TTntCustomListView.DefineProperties(Filer: TFiler);
begin
  inherited;
  TntPersistent_AfterInherited_DefineProperties(Filer, Self);
end;

procedure TTntCustomListView.CreateWnd;
begin
  inherited;
  FreeAndNil(FSavedItems);
end;

procedure TTntCustomListView.DestroyWnd;
var
  i: integer;
  FSavedItem: TSavedListItem;
  Item: TTntListItem;
begin
  if (not (csDestroying in ComponentState)) and (not OwnerData) then begin
    FSavedItems := TObjectList.Create(True);
    for i := 0 to Items.Count - 1 do begin
      FSavedItem := TSavedListItem.Create;
      Item := Items[i];
      FSavedItem.FCaption := Item.FCaption;
      FSavedItem.FSubItems.Assign(Item.FSubItems);
      FSavedItems.Add(FSavedItem)
    end;
  end;
  inherited;
end;

function TTntCustomListView.GetDropTarget: TTntListItem;
begin
  Result := inherited DropTarget as TTntListItem;
end;

procedure TTntCustomListView.SetDropTarget(const Value: TTntListItem);
begin
  inherited DropTarget := Value;
end;

function TTntCustomListView.GetItemFocused: TTntListItem;
begin
  Result := inherited ItemFocused as TTntListItem;
end;

procedure TTntCustomListView.SetItemFocused(const Value: TTntListItem);
begin
  inherited ItemFocused := Value;
end;

function TTntCustomListView.GetSelected: TTntListItem;
begin
  Result := inherited Selected as TTntListItem;
end;

procedure TTntCustomListView.SetSelected(const Value: TTntListItem);
begin
  inherited Selected := Value;
end;

function TTntCustomListView.GetTopItem: TTntListItem;
begin
  Result := inherited TopItem as TTntListItem;
end;

function TTntCustomListView.GetListColumns: TTntListColumns;
begin
  Result := inherited Columns as TTntListColumns;
end;

procedure TTntCustomListView.SetListColumns(const Value: TTntListColumns);
begin
  inherited Columns := Value;
end;

{$IFDEF COMPILER_5} // verified against VCL source in Delphi 5 and BCB 5
type
  THackListColumn = class(TCollectionItem)
  protected
    FxxxAlignment: TAlignment;
    FxxxAutoSize: Boolean;
    FxxxCaption: AnsiString;
    FxxxMaxWidth: TWidth;
    FxxxMinWidth: TWidth;
    FxxxImageIndex: TImageIndex;
    FxxxPrivateWidth: TWidth;
    FxxxWidth: TWidth;
    FOrderTag: Integer;
  end;
{$ENDIF}
{$IFDEF COMPILER_6} // verified against VCL source in Delphi 6 and BCB 6
type
  THackListColumn = class(TCollectionItem)
  protected
    FxxxAlignment: TAlignment;
    FxxxAutoSize: Boolean;
    FxxxCaption: AnsiString;
    FxxxMaxWidth: TWidth;
    FxxxMinWidth: TWidth;
    FxxxImageIndex: TImageIndex;
    FxxxPrivateWidth: TWidth;
    FxxxWidth: TWidth;
    FOrderTag: Integer;
  end;
{$ENDIF}
{$IFDEF DELPHI_7}
type
  THackListColumn = class(TCollectionItem)
  protected
    FxxxAlignment: TAlignment;
    FxxxAutoSize: Boolean;
    FxxxCaption: AnsiString;
    FxxxMaxWidth: TWidth;
    FxxxMinWidth: TWidth;
    FxxxImageIndex: TImageIndex;
    FxxxPrivateWidth: TWidth;
    FxxxWidth: TWidth;
    FOrderTag: Integer;
  end;
{$ENDIF}

function TTntCustomListView.GetColumnFromTag(Tag: Integer): TTntListColumn;
var
  I: Integer;
begin
  for I := 0 to Columns.Count - 1 do
  begin
    Result := Columns[I];
    if THackListColumn(Result).FOrderTag = Tag then Exit;
  end;
  Result := nil;
end;

function TTntCustomListView.ColumnFromIndex(Index: Integer): TTntListColumn;
begin
  Result := inherited Column[Index] as TTntListColumn;
end;

function TTntCustomListView.AreItemsStored: Boolean;
begin
  {$IFDEF COMPILER_6_UP}
  if Assigned(Action) then
  begin
    if Action is TCustomListAction{TNT-ALLOW TCustomListAction} then
      Result := False
    else
      Result := True;
  end
  else
    Result := not OwnerData;
  {$ELSE}
  Result := not OwnerData;
  {$ENDIF}
end;

function TTntCustomListView.GetItems: TTntListItems;
begin
  Result := inherited Items as TTntListItems;
end;

procedure TTntCustomListView.SetItems(Value: TTntListItems);
begin
  inherited Items := Value;
end;

type TTntListItemClass = class of TTntListItem;

function TTntCustomListView.CreateListItem: TListItem{TNT-ALLOW TListItem};
var
  LClass: TClass;
  TntLClass: TTntListItemClass;
begin
  LClass := TTntListItem;
  {$IFDEF COMPILER_6_UP}
  if Assigned(OnCreateItemClass) then
    OnCreateItemClass(Self, TListItemClass(LClass));
  if not LClass.InheritsFrom(TTntListItem) then
    raise Exception.Create('Internal Error: OnCreateItemClass.ItemClass must inherit from TTntListItem.');
  {$ENDIF}
  TntLClass := TTntListItemClass(LClass);
  Result := TntLClass.Create(inherited Items);
  if FTempItem = nil then
    FTempItem := Result as TTntListItem; { In Delphi 5/6, the first item creates is the temp item }
end;

{$IFDEF COMPILER_6_UP}
function TTntCustomListView.CreateListItems: TListItems{TNT-ALLOW TListItems};
begin
  Result := TTntListItems.Create(Self);
end;
{$ENDIF}

function TTntCustomListView.GetItemW(Value: TLVItemW): TTntListItem;
begin
  with Value do begin
    if OwnerData then
      Result := FTempItem
    else if (mask and LVIF_PARAM) <> 0 then
      Result := TListItem{TNT-ALLOW TListItem}(lParam) as TTntListItem
    else if iItem >= 0 then
      Result := Items[IItem]
    else
      Result := nil
  end;
end;

function TTntCustomListView.OwnerDataFetchW(Item: TListItem{TNT-ALLOW TListItem}; Request: TItemRequest): Boolean;
begin
  Result := OwnerDataFetch(Item, Request);
end;

function TTntCustomListView.OwnerDataFetch(Item: TListItem{TNT-ALLOW TListItem}; Request: TItemRequest): Boolean;
begin
  if Assigned(OnData) then
  begin
    OnData(Self, Item);
    Result := True;
  end
  else Result := False;
end;

function TntDefaultListViewSort(Item1, Item2: TTntListItem; lParam: Integer): Integer; stdcall;
begin
  Assert(Win32PlatformIsUnicode);
  with Item1 do
    if Assigned(ListView.OnCompare) then
      ListView.OnCompare(ListView, Item1, Item2, lParam, Result)
    else Result := lstrcmpw(PWideChar(Item1.Caption), PWideChar(Item2.Caption));
end;

procedure TTntCustomListView.WndProc(var Message: TMessage);
var
  Item: TTntListItem;
  InheritedItem: TListItem{TNT-ALLOW TListItem};
  SubItem: Integer;
  SavedItem: TSavedListItem;
  PCol: PLVColumn;
  Col: TTntListColumn;
begin
  with Message do begin
    // restore previous values (during CreateWnd)
    if (FSavedItems <> nil) and (FChangingWideItemCount = 0) and (Msg = LVM_SETITEMTEXTA) then begin
      Item := Items[wParam];
      SavedItem := TSavedListItem(FSavedItems[wParam]);
      if (Msg = LVM_SETITEMTEXTA) and (PLVItem(lParam).iSubItem = 0) then
        Item.FCaption := SavedItem.FCaption
      else begin
        SubItem := PLVItem(lParam).iSubItem - 1;
        TWideSubItems(Item.SubItems).FIgnoreInherited := True;
        try
          if SubItem < Item.SubItems.Count then begin
            Item.SubItems[SubItem] := SavedItem.FSubItems[SubItem];
            Item.SubItems.Objects[SubItem] := SavedItem.FSubItems.Objects[SubItem]
          end else if SubItem = Item.SubItems.Count then
            Item.SubItems.AddObject(SavedItem.FSubItems[SubItem], SavedItem.FSubItems.Objects[SubItem])
          else
            Item.SubItems.Assign(SavedItem.FSubItems)
        finally
          TWideSubItems(Item.SubItems).FIgnoreInherited := False;
        end;
      end;
    end;

    // sync wide with ansi
    if (FSavedItems = nil) and (FChangingWideItemCount = 0) and (Msg = LVM_UPDATE) then begin
      Item := Items[wParam];
      InheritedItem := Item;
      TWideSubItems(Item.SubItems).FIgnoreInherited := True;
      try
        Item.SubItems.Assign(InheritedItem.SubItems)
      finally
        TWideSubItems(Item.SubItems).FIgnoreInherited := False;
      end;
    end;

    if (FSavedItems = nil) and (FChangingWideItemCount = 0) and (Msg = LVM_SETITEMTEXTA) then begin
      if OwnerData then
        Item := FTempItem
      else
        Item := Items[wParam];
      InheritedItem := Item;
      if (Msg = LVM_SETITEMTEXTA) and (PLVItem(lParam).iSubItem = 0) then
        Item.FCaption := InheritedItem.Caption
      else begin
        SubItem := PLVItem(lParam).iSubItem - 1;
        TWideSubItems(Item.SubItems).FIgnoreInherited := True;
        try
          if SubItem < Item.SubItems.Count then begin
            Item.SubItems[SubItem] := InheritedItem.SubItems[SubItem];
            Item.SubItems.Objects[SubItem] := InheritedItem.SubItems.Objects[SubItem]
          end else if SubItem = Item.SubItems.Count then
            Item.SubItems.AddObject(InheritedItem.SubItems[SubItem], InheritedItem.SubItems.Objects[SubItem])
          else
            Item.SubItems.Assign(InheritedItem.SubItems)
        finally
          TWideSubItems(Item.SubItems).FIgnoreInherited := False;
        end;
      end;
    end;

    // capture ANSI version of DefaultListViewSort from ComCtrls
    if (FTestingForSortProc)
    and (Msg = LVM_SORTITEMS) then begin
      ComCtrls_DefaultListViewSort := Pointer(lParam);
      exit;
    end;

    if (Msg = LVM_SETCOLUMNA) then begin
      // make sure that wide column caption stays in sync with ANSI
      PCol := PLVColumn(lParam);
      if (PCol.mask and LVCF_TEXT) <> 0 then begin
        Col := GetColumnFromTag(wParam);
        if (Col <> nil) and (AnsiString(Col.Caption) <> PCol.pszText) then begin
          Col.FCaption := PCol.pszText;
        end;
      end;
    end;

    if (Win32PlatformIsUnicode)
    and (Msg = LVM_SETITEMTEXTA) and (PLVItem(lParam).pszText = LPSTR_TEXTCALLBACK) then
      // Unicode:: call wide version of text call back instead
      Result := SendMessageW(Handle, LVM_SETITEMTEXTW, WParam, LParam)
    else if (Win32PlatformIsUnicode)
    and (Msg = LVM_SORTITEMS) and (Pointer(lParam) = @ComCtrls_DefaultListViewSort) then
      // Unicode:: call wide version of sort proc instead
      Result := SendMessageW(Handle, LVM_SORTITEMS, wParam, Integer(@TntDefaultListViewSort))
    else if (Win32PlatformIsUnicode)
    and (Msg = LVM_SETCOLUMNA) and ((PLVColumn(lParam).mask and LVCF_TEXT) <> 0)
    and (GetColumnFromTag(wParam) <> nil) then begin
      PLVColumn(lParam).pszText := PAnsiChar(PWideChar(GetColumnFromTag(wParam).FCaption));
      Result := SendMessageW(Handle, LVM_SETCOLUMNW, wParam, lParam);
    end else begin
      if (Msg = LVM_SETEXTENDEDLISTVIEWSTYLE) and CheckBoxes then begin
        { fix a bug in TCustomListView.ResetExStyles }
        lParam := lParam or LVS_EX_SUBITEMIMAGES or LVS_EX_INFOTIP;
      end;
      inherited;
    end;
  end;
end;

procedure TTntCustomListView.WMNotify(var Message: TWMNotify);
begin
  inherited;
  // capture updated info after inherited
  with Message.NMHdr^ do
    case code of
      HDN_ENDTRACKW:
        begin
          Message.NMHdr^.code := HDN_ENDTRACKA;
          try
            inherited
          finally
            Message.NMHdr^.code := HDN_ENDTRACKW;
          end;
        end;
      HDN_DIVIDERDBLCLICKW:
        begin
          Message.NMHdr^.code := HDN_DIVIDERDBLCLICKA;
          try
            inherited
          finally
            Message.NMHdr^.code := HDN_DIVIDERDBLCLICKW;
          end;
        end;
    end;
end;

procedure TTntCustomListView.CNNotify(var Message: TWMNotify);
var
  Item: TTntListItem;
begin
  if (not Win32PlatformIsUnicode) then
    inherited
  else begin
    with Message do
    begin
      case NMHdr^.code of
        HDN_TRACKW:
          begin
            NMHdr^.code := HDN_TRACKA;
            try
              inherited;
            finally
              NMHdr^.code := HDN_TRACKW;
            end;
          end;
        LVN_GETDISPINFOW:
          begin
            // call inherited without the LVIF_TEXT flag
            CurrentDispInfo := PLVDispInfoW(NMHdr);
            try
              OriginalDispInfoMask := PLVDispInfoW(NMHdr)^.item.mask;

              PLVDispInfoW(NMHdr)^.item.mask := PLVDispInfoW(NMHdr)^.item.mask and (not LVIF_TEXT);
              try
                NMHdr^.code := LVN_GETDISPINFOA;
                try
                  inherited;
                finally
                  NMHdr^.code := LVN_GETDISPINFOW;
                end;
              finally
                if (OriginalDispInfoMask and LVIF_TEXT <> 0) then
                  PLVDispInfoW(NMHdr)^.item.mask := PLVDispInfoW(NMHdr)^.item.mask or LVIF_TEXT;
              end;
            finally
              CurrentDispInfo := nil;
            end;

            // handle any text info
            with PLVDispInfoW(NMHdr)^.item do
            begin
              if (mask and LVIF_TEXT) <> 0 then
              begin
                Item := GetItemW(PLVDispInfoW(NMHdr)^.item);
                if iSubItem = 0 then
                  StrLCopyW(pszText, PWideChar(Item.Caption), cchTextMax - 1)
                else begin
                  with Item.SubItems do begin
                    if iSubItem <= Count then
                      StrLCopyW(pszText, PWideChar(Strings[iSubItem - 1]), cchTextMax - 1)
                    else pszText[0] := #0;
                  end;
                end;
              end;
            end;
          end;
        LVN_ODFINDITEMW:
          with PNMLVFindItem(NMHdr)^ do
          begin
            if ((lvfi.flags and LVFI_PARTIAL) <> 0) or ((lvfi.flags and LVFI_STRING) <> 0) then
              PWideFindString := TLVFindInfoW(lvfi).psz
            else
              PWideFindString := nil;
            lvfi.psz := nil;
            NMHdr^.code := LVN_ODFINDITEMA;
            try
              inherited; {will Result in call to OwnerDataFind}
            finally
              TLVFindInfoW(lvfi).psz := PWideFindString;
              NMHdr^.code := LVN_ODFINDITEMW;
              PWideFindString := nil;
            end;
          end;
        LVN_BEGINLABELEDITW:
          begin
            Item := GetItemW(PLVDispInfoW(NMHdr)^.item);
            if not CanEdit(Item) then Result := 1;
            if Result = 0 then
            begin
              FEditHandle := ListView_GetEditControl(Handle);
              FDefEditProc := Pointer(GetWindowLongW(FEditHandle, GWL_WNDPROC));
              SetWindowLongW(FEditHandle, GWL_WNDPROC, LongInt(FEditInstance));
            end;
          end;
        LVN_ENDLABELEDITW:
          with PLVDispInfoW(NMHdr)^ do
            if (item.pszText <> nil) and (item.IItem <> -1) then
              Edit(TLVItemA(item));
        LVN_GETINFOTIPW:
          begin
            NMHdr^.code := LVN_GETINFOTIPA;
            try
              inherited;
            finally
              NMHdr^.code := LVN_GETINFOTIPW;
            end;
          end;
        else
          inherited;
      end;
    end;
  end;
end;

function TTntCustomListView.OwnerDataFindW(Find: TItemFind;
  const FindString: WideString; const FindPosition: TPoint;
  FindData: Pointer; StartIndex: Integer; Direction: TSearchDirection;
  Wrap: Boolean): Integer;
begin
  Result := OwnerDataFind(Find, FindString, FindPosition, FindData, StartIndex, Direction, Wrap);
end;

function TTntCustomListView.OwnerDataFind(Find: TItemFind; const FindString: WideString;
  const FindPosition: TPoint; FindData: Pointer; StartIndex: Integer;
  Direction: TSearchDirection; Wrap: Boolean): Integer;
var
  AnsiEvent: TLVOwnerDataFindEvent;
begin
  Result := -1;
  if Assigned(OnDataFind) then
    OnDataFind(Self, Find, FindString, FindPosition, FindData, StartIndex, Direction, Wrap, Result)
  else if Assigned(inherited OnDataFind) then begin
    AnsiEvent := inherited OnDataFind;
    AnsiEvent(Self, Find, FindString, FindPosition, FindData, StartIndex, Direction,
      Wrap, Result);
  end;
end;

procedure TTntCustomListView.Edit(const Item: TLVItem);
var
  S: WideString;
  AnsiS: AnsiString;
  EditItem: TTntListItem;
  AnsiEvent: TLVEditedEvent;
begin
  if (not Win32PlatformIsUnicode) then
    S := Item.pszText
  else
    S := TLVItemW(Item).pszText;
  EditItem := GetItemW(TLVItemW(Item));
  if Assigned(OnEdited) then
    OnEdited(Self, EditItem, S)
  else if Assigned(inherited OnEdited) then
  begin
    AnsiEvent := inherited OnEdited;
    AnsiS := S;
    AnsiEvent(Self, EditItem, AnsiS);
    S := AnsiS;
  end;
  if EditItem <> nil then
    EditItem.Caption := S;
end;

procedure TTntCustomListView.EditWndProcW(var Message: TMessage);
begin
  Assert(Win32PlatformIsUnicode);
  try
    with Message do
    begin
      case Msg of
        WM_KEYDOWN,
        WM_SYSKEYDOWN: if DoKeyDown(TWMKey(Message)) then Exit;
        WM_CHAR:
          begin
            MakeWMCharMsgSafeForAnsi(Message);
            try
              if DoKeyPress(TWMKey(Message)) then Exit;
            finally
              RestoreWMCharMsg(Message);
            end;
          end;
        WM_KEYUP,
        WM_SYSKEYUP: if DoKeyUp(TWMKey(Message)) then Exit;
        CN_KEYDOWN,
        CN_CHAR, CN_SYSKEYDOWN,
        CN_SYSCHAR:
          begin
            WndProc(Message);
            Exit;
          end;
      end;
      Result := CallWindowProcW(FDefEditProc, FEditHandle, Msg, WParam, LParam);
    end;
  except
    Application.HandleException(Self);
  end;
end;

procedure TTntCustomListView.BeginChangingWideItem;
begin
  Inc(FChangingWideItemCount);
end;

procedure TTntCustomListView.EndChangingWideItem;
begin
  if FChangingWideItemCount > 0 then
    Dec(FChangingWideItemCount);
end;

procedure TTntCustomListView.DrawItem(Item: TListItem{TNT-ALLOW TListItem}; Rect: TRect;
  State: TOwnerDrawState);
begin
  TControlCanvas(Canvas).UpdateTextFlags;
  if Assigned(OnDrawItem) then OnDrawItem(Self, Item, Rect, State)
  else
  begin
    Canvas.FillRect(Rect);
    WideCanvasTextOut(Canvas, Rect.Left + 2, Rect.Top, Item.Caption);
  end;
end;

{$IFDEF COMPILER_6_UP}
procedure TTntCustomListView.CopySelection(Destination: TCustomListControl);
var
  I: Integer;
begin
  for I := 0 to Items.Count - 1 do
    if Items[I].Selected then
      WideListControl_AddItem(Destination, Items[I].Caption, Items[I].Data);
end;
{$ENDIF}

procedure TTntCustomListView.AddItem(const Item: WideString; AObject: TObject);
begin
  with Items.Add do
  begin
    Caption := Item;
    Data := AObject;
  end;
end;

//-------------

function TTntCustomListView.FindCaption(StartIndex: Integer; Value: WideString;
  Partial, Inclusive, Wrap: Boolean): TTntListItem;
const
  FullString: array[Boolean] of Integer = (0, LVFI_PARTIAL);
  Wraps: array[Boolean] of Integer = (0, LVFI_WRAP);
var
  Info: TLVFindInfoW;
  Index: Integer;
begin
  if (not Win32PlatformIsUnicode) then
    Result := inherited FindCaption(StartIndex, Value, Partial, Inclusive, Wrap) as TTntListItem
  else begin
    with Info do
    begin
      flags := LVFI_STRING or FullString[Partial] or Wraps[Wrap];
      psz := PWideChar(Value);
    end;
    if Inclusive then Dec(StartIndex);
    Index := SendMessageW(Handle, LVM_FINDITEMW, StartIndex, Longint(@Info));
    if Index <> -1 then Result := Items[Index]
    else Result := nil;
  end;
end;

function TTntCustomListView.StringWidth(S: WideString): Integer;
begin
  if (not Win32PlatformIsUnicode) then
    Result := inherited StringWidth(S)
  else
    Result := SendMessageW(Handle, LVM_GETSTRINGWIDTHW, 0, Longint(PWideChar(S)))
end;

function TTntCustomListView.GetSearchString: WideString;
var
  Buffer: array[0..1023] of WideChar;
begin
  if (not Win32PlatformIsUnicode) then
    Result := inherited GetSearchString
  else begin
    Result := '';
    if HandleAllocated
    and Bool(SendMessageW(Handle, LVM_GETISEARCHSTRINGW, 0, Longint(PWideChar(@Buffer[0])))) then
      Result := Buffer;
  end;
end;

function TTntCustomListView.IsHintStored: Boolean;
begin
  Result := TntControl_IsHintStored(Self);
end;

function TTntCustomListView.GetHint: WideString;
begin
  Result := TntControl_GetHint(Self)
end;

procedure TTntCustomListView.SetHint(const Value: WideString);
begin
  TntControl_SetHint(Self, Value);
end;

procedure TTntCustomListView.ActionChange(Sender: TObject; CheckDefaults: Boolean);
begin
  TntControl_BeforeInherited_ActionChange(Self, Sender, CheckDefaults);
  inherited;
end;

function TTntCustomListView.GetActionLinkClass: TControlActionLinkClass;
begin
  Result := TntControl_GetActionLinkClass(Self, inherited GetActionLinkClass);
end;

{ TTntRichEditStrings }
type
  TTntRichEditStrings = class(TTntMemoStrings)
  private
    RichEdit: TCustomRichEdit{TNT-ALLOW TCustomRichEdit};
    procedure EnableChange(const Value: Boolean);
  protected
    procedure SetTextStr(const Value: WideString); override;
  public
    constructor Create;
    procedure AddStrings(Strings: TTntStrings); override;
    //--
    procedure LoadFromStream(Stream: TStream; WithBOM: Boolean = True); override;
    procedure SaveToStream(Stream: TStream; WithBOM: Boolean = True); override;
    procedure LoadFromFile(const FileName: WideString); override;
    procedure SaveToFile(const FileName: WideString); override;
  end;

constructor TTntRichEditStrings.Create;
begin
  inherited Create;
  FRichEditMode := True;
end;

procedure TTntRichEditStrings.AddStrings(Strings: TTntStrings);
var
  SelChange: TNotifyEvent;
begin
  SelChange := TTntCustomRichEdit(RichEdit).OnSelectionChange;
  TTntCustomRichEdit(RichEdit).OnSelectionChange := nil;
  try
    inherited;
  finally
    TTntCustomRichEdit(RichEdit).OnSelectionChange := SelChange;
  end;
end;

procedure TTntRichEditStrings.EnableChange(const Value: Boolean);
var
  EventMask: Longint;
begin
  with RichEdit do
  begin
    if Value then
      EventMask := SendMessage(Handle, EM_GETEVENTMASK, 0, 0) or ENM_CHANGE
    else
      EventMask := SendMessage(Handle, EM_GETEVENTMASK, 0, 0) and not ENM_CHANGE;
    SendMessage(Handle, EM_SETEVENTMASK, 0, EventMask);
  end;
end;

procedure TTntRichEditStrings.SetTextStr(const Value: WideString);
begin
  EnableChange(False);
  try
    inherited;
  finally
    EnableChange(True);
  end;
end;

type TAccessCustomRichEdit = class(TCustomRichEdit{TNT-ALLOW TCustomRichEdit});

procedure TTntRichEditStrings.LoadFromStream(Stream: TStream; WithBOM: Boolean = True);
begin
  if TAccessCustomRichEdit(RichEdit).PlainText then
    inherited LoadFromStream(Stream, WithBOM)
  else
    TAccessCustomRichEdit(RichEdit).Lines.LoadFromStream(Stream);
end;

procedure TTntRichEditStrings.SaveToStream(Stream: TStream; WithBOM: Boolean = True);
begin
  if TAccessCustomRichEdit(RichEdit).PlainText then
    inherited SaveToStream(Stream, WithBOM)
  else
    TAccessCustomRichEdit(RichEdit).Lines.SaveToStream(Stream);
end;

procedure TTntRichEditStrings.LoadFromFile(const FileName: WideString);
begin
  if TAccessCustomRichEdit(RichEdit).PlainText then
    inherited LoadFromFile(FileName)
  else
    TAccessCustomRichEdit(RichEdit).Lines.LoadFromFile(FileName);
end;

procedure TTntRichEditStrings.SaveToFile(const FileName: WideString);
begin
  if TAccessCustomRichEdit(RichEdit).PlainText then
    inherited SaveToFile(FileName)
  else
    TAccessCustomRichEdit(RichEdit).Lines.SaveToFile(FileName);
end;

{ TTntCustomRichEdit }

constructor TTntCustomRichEdit.Create(AOwner: TComponent);
begin
  inherited;
  FRichEditStrings := TTntRichEditStrings.Create;
  TTntRichEditStrings(FRichEditStrings).Memo := Self;
  TTntRichEditStrings(FRichEditStrings).RichEdit := Self;
  TTntRichEditStrings(FRichEditStrings).LineBreakStyle := Self.LineBreakStyle;
end;

var
  FRichEdit20Module: THandle = 0;

function IsRichEdit20Available: Boolean;
const
  RICHED20_DLL = 'RICHED20.DLL';
begin
  if FRichEdit20Module = 0 then
    FRichEdit20Module := LoadLibrary(RICHED20_DLL);
  Result := FRichEdit20Module <> 0;
end;

procedure TTntCustomRichEdit.CreateParams(var Params: TCreateParams);
begin
  inherited CreateParams(Params);
  if WordWrap then
    Params.Style := Params.Style and not WS_HSCROLL; // more compatible with RichEdit 1.0
end;

procedure TTntCustomRichEdit.CreateWindowHandle(const Params: TCreateParams);
begin
  if Win32PlatformIsUnicode and IsRichEdit20Available then
    CreateUnicodeHandle(Self, Params, RICHEDIT_CLASSW)
  else
    inherited
end;

var
  AIMM: IActiveIMMApp = nil;

function EnableActiveIMM: Boolean;
begin
  if AIMM <> nil then
    Result := True
  else begin
    Result := False;
    try
      if ClassIsRegistered(CLASS_CActiveIMM) then begin
        AIMM := CoCActiveIMM.Create;
        AIMM.Activate(1);
        Result := True;
      end;
    except
      AIMM := nil;
    end;
  end;
end;

procedure TTntCustomRichEdit.CreateWnd;
const
  EM_SETEDITSTYLE = WM_USER + 204;
  SES_USEAIMM = 64;
begin
  inherited;
  // Only supported in RichEdit 3.0, but this flag is harmless to RichEdit1.0 or RichEdit 2.0
  if EnableActiveIMM then
    SendMessage(Handle, EM_SETEDITSTYLE, SES_USEAIMM, SES_USEAIMM);
end;

procedure TTntCustomRichEdit.DefineProperties(Filer: TFiler);
begin
  inherited;
  TntPersistent_AfterInherited_DefineProperties(Filer, Self);
end;

destructor TTntCustomRichEdit.Destroy;
begin
  FreeAndNil(FRichEditStrings);
  inherited;
end;

function TTntCustomRichEdit.LineBreakStyle: TTntTextLineBreakStyle;
begin
  if Win32PlatformIsUnicode and IsRichEdit20Available then
    Result := tlbsCR
  else
    Result := tlbsCRLF;
end;

procedure TTntCustomRichEdit.SetRichEditStrings(const Value: TTntStrings);
begin
  FRichEditStrings.Assign(Value);
end;

function TTntCustomRichEdit.GetSelText: string{TNT-ALLOW string};
begin
  Result := GetWideSelText;
end;

function TTntCustomRichEdit.GetWideSelText: WideString;
var
  CharRange: TCharRange;
  Length: Integer;
begin
  if (not IsWindowUnicode(Handle)) then
    Result := inherited GetSelText
  else begin
    SendMessageW(Handle, EM_EXGETSEL, 0, Longint(@CharRange));
    SetLength(Result, CharRange.cpMax - CharRange.cpMin + 1);
    Length := SendMessageW(Handle, EM_GETSELTEXT, 0, Longint(PWideChar(Result)));
    SetLength(Result, Length);
  end;
  if LineBreakStyle <> tlbsCRLF then
    Result := TntAdjustLineBreaks(Result, tlbsCRLF)
end;

type
  TSetTextEx = record
    flags:dword;
    codepage:uint;
  end;

procedure TTntCustomRichEdit.SetRTFText(Flags: DWORD; const Value: AnsiString);
const
  EM_SETTEXTEX = (WM_USER + 97);
var
  Info: TSetTextEx;
begin
  Info.flags := Flags;
  Info.codepage := CP_ACP{TNT-ALLOW CP_ACP};
  SendMessage(Handle, EM_SETTEXTEX, Integer(@Info), Integer(PAnsiChar(Value)));
end;

procedure TTntCustomRichEdit.SetWideSelText(const Value: WideString);
const
  ST_SELECTION = 2;
begin
  if Win32PlatformIsUnicode and IsRichEdit20Available and IsRTF(Value) then begin
    // emulate RichEdit 1.0 so that RTF code is inserted as RTF (not plain text)
    SetRTFText(ST_SELECTION, Value)
  end else
    TntCustomEdit_SetSelText(Self, TntAdjustLineBreaks(Value, LineBreakStyle));
end;

function TTntCustomRichEdit.GetText: WideString;
begin
  Result := TntControl_GetText(Self);
  if (LineBreakStyle <> tlbsCRLF) then
    Result := TntAdjustLineBreaks(Result, tlbsCRLF);
end;

procedure TTntCustomRichEdit.SetText(const Value: WideString);
const
  ST_DEFAULT = 0;
begin
  if Win32PlatformIsUnicode and IsRichEdit20Available and IsRTF(Value) then begin
    // emulate RichEdit 1.0 so that RTF code is inserted as RTF (not plain text)
    SetRTFText(ST_DEFAULT, Value)
  end else if Value <> Text then
    TntControl_SetText(Self, TntAdjustLineBreaks(Value, LineBreakStyle));
end;

function TTntCustomRichEdit.IsHintStored: Boolean;
begin
  Result := TntControl_IsHintStored(Self);
end;

function TTntCustomRichEdit.GetHint: WideString;
begin
  Result := TntControl_GetHint(Self);
end;

procedure TTntCustomRichEdit.SetHint(const Value: WideString);
begin
  TntControl_SetHint(Self, Value);
end;

procedure TTntCustomRichEdit.WMGetTextLength(var Message: TWMGetTextLength);
begin
  if FPrintingTextLength <> 0 then
    Message.Result := FPrintingTextLength
  else
    inherited;
end;

procedure TTntCustomRichEdit.Print(const Caption: string{TNT-ALLOW string});
begin
  if (LineBreakStyle <> tlbsCRLF) then
    FPrintingTextLength := TntAdjustLineBreaksLength(Text, LineBreakStyle)
  else
    FPrintingTextLength := 0;
  try
    inherited
  finally
    FPrintingTextLength := 0;
  end;
end;

function TTntCustomRichEdit.CharPosToGet(CharPos: Integer): Integer;
var
  i: Integer;
  ThisLine: Integer;
  CharCount: Integer;
  Line_Start: Integer;
  NumLineBreaks: Integer;
begin
  if (LineBreakStyle = tlbsCRLF) or (CharPos <= 0) then
    Result := CharPos
  else begin
    Assert(Win32PlatformIsUnicode);
    ThisLine := SendMessageW(Handle, EM_EXLINEFROMCHAR, 0, CharPos);
    CharCount := 0;
    for i := 0 to ThisLine - 1 do
      Inc(CharCount, TntMemo_LineLength(Handle, i));
    Line_Start := TntMemo_LineStart(Handle, ThisLine);
    NumLineBreaks := Line_Start - CharCount;
    Result := CharPos + NumLineBreaks; {inflate CR -> CR/LF}
  end;
end;

function TTntCustomRichEdit.CharPosToSet(CharPos: Integer): Integer;
var
  Line: Integer;
  NumLineBreaks: Integer;
  CharCount: Integer;
  Line_Start: Integer;
  LineLength: Integer;
begin
  if (LineBreakStyle = tlbsCRLF) or (CharPos <= 0) then
    Result := CharPos
  else begin
    Assert(Win32PlatformIsUnicode);
    NumLineBreaks := 0;
    CharCount := 0;
    for Line := 0 to Lines.Count - 1 do begin
      Line_Start := TntMemo_LineStart(Handle, Line);
      if CharPos < (Line_Start + NumLineBreaks) then
        break; {found it (it must have been the line separator)}
      if Line_Start > CharCount then begin
        Inc(NumLineBreaks);
        Inc(CharCount);
      end;
      LineLength := TntMemo_LineLength(Handle, Line);
      Inc(CharCount, LineLength);
      if (CharPos >= (Line_Start + NumLineBreaks))
      and (CharPos < (Line_Start + LineLength + NumLineBreaks)) then
        break; {found it}
    end;
    Result := CharPos - NumLineBreaks; {deflate CR/LF -> CR}
  end;
end;

function TTntCustomRichEdit.FindText(const SearchStr: WideString;
  StartPos, Length: Integer; Options: TSearchTypes): Integer;
const
  EM_FINDTEXTEXW = WM_USER + 124;
const
  FR_DOWN        = $00000001;
  FR_WHOLEWORD   = $00000002;
  FR_MATCHCASE   = $00000004;
var
  Find: TFindTextW;
  Flags: Integer;
begin
  if (not Win32PlatformIsUnicode) then
    Result := inherited FindText(SearchStr, StartPos, Length, Options)
  else begin
    with Find.chrg do
    begin
      cpMin := CharPosToSet(StartPos);
      cpMax := CharPosToSet(StartPos + Length);
    end;
    Flags := FR_DOWN; { RichEdit 2.0 and later needs this }
    if stWholeWord in Options then Flags := Flags or FR_WHOLEWORD;
    if stMatchCase in Options then Flags := Flags or FR_MATCHCASE;
    Find.lpstrText := PWideChar(SearchStr);
    Result := SendMessageW(Handle, EM_FINDTEXT, Flags, LongInt(@Find));
    Result := CharPosToGet(Result);
  end;
end;

function TTntCustomRichEdit.GetSelStart: Integer;
begin
  Result := TntCustomEdit_GetSelStart(Self);
  if (LineBreakStyle <> tlbsCRLF) then
    Result := CharPosToGet(Result);
end;

procedure TTntCustomRichEdit.SetSelStart(const Value: Integer);
begin
  if (LineBreakStyle = tlbsCRLF) then
    TntCustomEdit_SetSelStart(Self, Value)
  else
    TntCustomEdit_SetSelStart(Self, CharPosToSet(Value));
end;

function TTntCustomRichEdit.GetSelLength: Integer;
var
  CharRange: TCharRange;
begin
  if (LineBreakStyle = tlbsCRLF) then
    Result := TntCustomEdit_GetSelLength(Self)
  else begin
    Assert(Win32PlatformIsUnicode);
    SendMessageW(Handle, EM_EXGETSEL, 0, Longint(@CharRange));
    Result := CharPosToGet(CharRange.cpMax) - CharPosToGet(CharRange.cpMin);
  end;
end;

procedure TTntCustomRichEdit.SetSelLength(const Value: Integer);
var
  StartPos: Integer;
  SelEnd: Integer;
begin
  if (LineBreakStyle = tlbsCRLF) then
    TntCustomEdit_SetSelLength(Self, Value)
  else begin
    StartPos := Self.SelStart;
    SelEnd := StartPos + Value;
    inherited SetSelLength(CharPosToSet(SelEnd) - CharPosToSet(StartPos));
  end;
end;

procedure TTntCustomRichEdit.ActionChange(Sender: TObject; CheckDefaults: Boolean);
begin
  TntControl_BeforeInherited_ActionChange(Self, Sender, CheckDefaults);
  inherited;
end;

function TTntCustomRichEdit.GetActionLinkClass: TControlActionLinkClass;
begin
  Result := TntControl_GetActionLinkClass(Self, inherited GetActionLinkClass);
end;

{ TTntTabStrings }

type TAccessCustomTabControl = class(TCustomTabControl{TNT-ALLOW TCustomTabControl});

type
  TTntTabStrings = class(TTntStrings)
  private
    FTabControl: TCustomTabControl{TNT-ALLOW TCustomTabControl};
    FAnsiTabs: TStrings{TNT-ALLOW TStrings};
  protected
    function Get(Index: Integer): WideString; override;
    function GetCount: Integer; override;
    function GetObject(Index: Integer): TObject; override;
    procedure Put(Index: Integer; const S: WideString); override;
    procedure PutObject(Index: Integer; AObject: TObject); override;
    procedure SetUpdateState(Updating: Boolean); override;
  public
    procedure Clear; override;
    procedure Delete(Index: Integer); override;
    procedure Insert(Index: Integer; const S: WideString); override;
  end;

procedure TabControlError(const S: WideString);
begin
  raise EListError.Create(S);
end;

procedure TTntTabStrings.Clear;
begin
  FAnsiTabs.Clear;
end;

procedure TTntTabStrings.Delete(Index: Integer);
begin
  FAnsiTabs.Delete(Index);
end;

function TTntTabStrings.GetCount: Integer;
begin
  Result := FAnsiTabs.Count;
end;

function TTntTabStrings.GetObject(Index: Integer): TObject;
begin
  Result := FAnsiTabs.Objects[Index];
end;

procedure TTntTabStrings.PutObject(Index: Integer; AObject: TObject);
begin
  FAnsiTabs.Objects[Index] := AObject;
end;

procedure TTntTabStrings.SetUpdateState(Updating: Boolean);
begin
  inherited;
  TAccessStrings(FAnsiTabs).SetUpdateState(Updating);
end;

function TTntTabStrings.Get(Index: Integer): WideString;
const
  RTL: array[Boolean] of LongInt = (0, TCIF_RTLREADING);
var
  TCItem: TTCItemW;
  Buffer: array[0..4095] of WideChar;
begin
  if (not Win32PlatformIsUnicode) then
    Result := FAnsiTabs[Index]
  else begin
    TCItem.mask := TCIF_TEXT or RTL[FTabControl.UseRightToLeftReading];
    TCItem.pszText := Buffer;
    TCItem.cchTextMax := SizeOf(Buffer);
    if SendMessageW(FTabControl.Handle, TCM_GETITEMW, Index, Longint(@TCItem)) = 0 then
      TabControlError(WideFormat(sTabFailRetrieve, [Index]));
    Result := Buffer;
  end;
end;

function GetTabControlImageIndex(Self: TCustomTabControl{TNT-ALLOW TCustomTabControl}; TabIndex: Integer): Integer;
begin
  Result := TabIndex;
  with TAccessCustomTabControl(Self) do
    if Assigned(OnGetImageIndex) then OnGetImageIndex(Self, TabIndex, Result);
end;

procedure TTntTabStrings.Put(Index: Integer; const S: WideString);
const
  RTL: array[Boolean] of LongInt = (0, TCIF_RTLREADING);
var
  TCItem: TTCItemW;
begin
  if (not Win32PlatformIsUnicode) then
    FAnsiTabs[Index] := S
  else begin
    TCItem.mask := TCIF_TEXT or RTL[FTabControl.UseRightToLeftReading] or TCIF_IMAGE;
    TCItem.pszText := PWideChar(S);
    TCItem.iImage := GetTabControlImageIndex(FTabControl, Index);
    if SendMessageW(FTabControl.Handle, TCM_SETITEMW, Index, Longint(@TCItem)) = 0 then
      TabControlError(WideFormat(sTabFailSet, [S, Index]));
    TAccessCustomTabControl(FTabControl).UpdateTabImages;
  end;
end;

procedure TTntTabStrings.Insert(Index: Integer; const S: WideString);
const
  RTL: array[Boolean] of LongInt = (0, TCIF_RTLREADING);
var
  TCItem: TTCItemW;
begin
  if (not Win32PlatformIsUnicode) then
    FAnsiTabs.Insert(Index, S)
  else begin
    TCItem.mask := TCIF_TEXT or RTL[FTabControl.UseRightToLeftReading] or TCIF_IMAGE;
    TCItem.pszText := PWideChar(S);
    TCItem.iImage := GetTabControlImageIndex(FTabControl, Index);
    if SendMessageW(FTabControl.Handle, TCM_INSERTITEMW, Index, Longint(@TCItem)) < 0 then
      TabControlError(WideFormat(sTabFailSet, [S, Index]));
    TAccessCustomTabControl(FTabControl).UpdateTabImages;
  end;
end;

{ TTntCustomTabControl }

constructor TTntCustomTabControl.Create(AOwner: TComponent);
begin
  inherited;
  FTabs := TTntTabStrings.Create;
  TTntTabStrings(FTabs).FTabControl := Self;
  TTntTabStrings(FTabs).FAnsiTabs := inherited Tabs;
end;

destructor TTntCustomTabControl.Destroy;
begin
  TTntTabStrings(FTabs).FTabControl := nil;
  TTntTabStrings(FTabs).FAnsiTabs := nil;
  FreeAndNil(FTabs);
  FreeAndNil(FSaveTabs);
  inherited;
end;

procedure TTntCustomTabControl.CreateWindowHandle(const Params: TCreateParams);
begin
  CreateUnicodeHandle_ComCtl(Self, Params, WC_TABCONTROL);
end;

procedure TTntCustomTabControl.DefineProperties(Filer: TFiler);
begin
  inherited;
  TntPersistent_AfterInherited_DefineProperties(Filer, Self);
end;

procedure TTntCustomTabControl.CreateWnd;
begin
  inherited;
  if FSaveTabs <> nil then
  begin
    FTabs.Assign(FSaveTabs);
    FreeAndNil(FSaveTabs);
    TabIndex := FSaveTabIndex;
  end;
end;

procedure TTntCustomTabControl.DestroyWnd;
begin
  if (FTabs <> nil) and (FTabs.Count > 0) then
  begin
    FSaveTabs := TTntStringList.Create;
    FSaveTabs.Assign(FTabs);
    FSaveTabIndex := TabIndex;
  end;
  inherited;
end;

procedure TTntCustomTabControl.SetTabs(const Value: TTntStrings);
begin
  FTabs.Assign(Value);
end;

function TTntCustomTabControl.IsHintStored: Boolean;
begin
  Result := TntControl_IsHintStored(Self);
end;

function TTntCustomTabControl.GetHint: WideString;
begin
  Result := TntControl_GetHint(Self);
end;

procedure TTntCustomTabControl.SetHint(const Value: WideString);
begin
  TntControl_SetHint(Self, Value);
end;

procedure TTntCustomTabControl.CMDialogChar(var Message: TCMDialogChar);
var
  I: Integer;
begin
  for I := 0 to Tabs.Count - 1 do
    if IsWideCharAccel(Message.CharCode, Tabs[I]) and CanShowTab(I) and CanFocus then
    begin
      Message.Result := 1;
      if CanChange then
      begin
        TabIndex := I;
        Change;
      end;
      Exit;
    end;
  Broadcast(Message);
end;

procedure TTntCustomTabControl.ActionChange(Sender: TObject; CheckDefaults: Boolean);
begin
  TntControl_BeforeInherited_ActionChange(Self, Sender, CheckDefaults);
  inherited;
end;

function TTntCustomTabControl.GetActionLinkClass: TControlActionLinkClass;
begin
  Result := TntControl_GetActionLinkClass(Self, inherited GetActionLinkClass);
end;

{ TTntTabSheet }

procedure TTntTabSheet.CreateWindowHandle(const Params: TCreateParams);
begin
  CreateUnicodeHandle(Self, Params, '');
end;

function TTntTabSheet.IsCaptionStored: Boolean;
begin
  Result := TntControl_IsCaptionStored(Self);
end;

function TTntTabSheet.GetCaption: TWideCaption;
begin
  Result := TntControl_GetText(Self);
end;

procedure TTntTabSheet.SetCaption(const Value: TWideCaption);
begin
  TntControl_SetText(Self, Value);
end;

procedure TTntTabSheet.DefineProperties(Filer: TFiler);
begin
  inherited;
  TntPersistent_AfterInherited_DefineProperties(Filer, Self);
end;

function TTntTabSheet.IsHintStored: Boolean;
begin
  Result := TntControl_IsHintStored(Self);
end;

function TTntTabSheet.GetHint: WideString;
begin
  Result := TntControl_GetHint(Self);
end;

procedure TTntTabSheet.SetHint(const Value: WideString);
begin
  TntControl_SetHint(Self, Value);
end;

procedure TTntTabSheet.WMSetText(var Message: TWMSetText);
begin
  if (not Win32PlatformIsUnicode)
  or (HandleAllocated)
  or (Message.Text = AnsiString(TntControl_GetText(Self)))
  or (Force_Inherited_WMSETTEXT) then
    inherited
  else begin
    // NT, handle not allocated and text is different
    Force_Inherited_WMSETTEXT := True;
    try
      TntControl_SetText(Self, Message.Text) { sync WideCaption with ANSI Caption }
    finally
      Force_Inherited_WMSETTEXT := FALSE;
    end;
  end;
end;

procedure TTntTabSheet.ActionChange(Sender: TObject; CheckDefaults: Boolean);
begin
  TntControl_BeforeInherited_ActionChange(Self, Sender, CheckDefaults);
  inherited;
end;

function TTntTabSheet.GetActionLinkClass: TControlActionLinkClass;
begin
  Result := TntControl_GetActionLinkClass(Self, inherited GetActionLinkClass);
end;

{ TTntPageControl }

procedure TTntPageControl.CreateWindowHandle(const Params: TCreateParams);
begin
  CreateUnicodeHandle_ComCtl(Self, Params, WC_TABCONTROL);
end;

procedure TTntPageControl.DefineProperties(Filer: TFiler);
begin
  inherited;
  TntPersistent_AfterInherited_DefineProperties(Filer, Self);
end;

function TTntPageControl.IsHintStored: Boolean;
begin
  Result := TntControl_IsHintStored(Self);
end;

function TTntPageControl.GetHint: WideString;
begin
  Result := TntControl_GetHint(Self);
end;

procedure TTntPageControl.SetHint(const Value: WideString);
begin
  TntControl_SetHint(Self, Value);
end;

procedure TTntPageControl.WndProc(var Message: TMessage);
const
  RTL: array[Boolean] of Cardinal = (0, TCIF_RTLREADING);
var
  TCItemA: PTCItemA;
  TabSheet: TTabSheet{TNT-ALLOW TTabSheet};
  Text: WideString;
begin
  if (not Win32PlatformIsUnicode) then
    inherited
  else begin
    case Message.Msg of
      TCM_SETITEMA:
        begin
          TCItemA := PTCItemA(Message.lParam);
          if ((TCItemA.mask and TCIF_PARAM) = TCIF_PARAM) then
            TabSheet := TObject(TCItemA.lParam) as TTabSheet{TNT-ALLOW TTabSheet}
          else if ((TCItemA.mask and TCIF_TEXT) = TCIF_TEXT)
          and (Message.wParam >= 0) and (Message.wParam <= Tabs.Count - 1) then
            TabSheet := Tabs.Objects[Message.wParam] as TTabSheet{TNT-ALLOW TTabSheet}
          else
            TabSheet := nil;

          if TabSheet = nil then begin
            // will always be followed by TCM_SETITEMA(TCIF_PARAM) if TCIF_TEXT is present
            TCItemA.mask := TCItemA.mask and (not TCIF_TEXT);
          end else begin
            // convert message to unicode, add text
            Message.Msg := TCM_SETITEMW;
            TCItemA.mask := TCItemA.mask or TCIF_TEXT or RTL[UseRightToLeftReading];
            if TabSheet is TTntTabSheet then
              Text := TTntTabSheet(TabSheet).Caption
            else
              Text := TabSheet.Caption;
            TCItemA.pszText := PAnsiChar(PWideChar(Text));
          end;
        end;
      TCM_INSERTITEMA:
        begin
          TCItemA := PTCItemA(Message.lParam);
          // will always be followed by TCM_SETITEMA(TCIF_PARAM) if TCIF_TEXT is present
          TCItemA.mask := TCItemA.mask and (not TCIF_TEXT);
        end;
    end;
    inherited;
  end;
end;

procedure TTntPageControl.CMDialogChar(var Message: TCMDialogChar);
var
  I: Integer;
  TabText: WideString;
begin
  for I := 0 to PageCount - 1 do begin
    if Pages[i] is TTntTabSheet then
      TabText := TTntTabSheet(Pages[i]).Caption
    else
      TabText := Pages[i].Caption;
    if IsWideCharAccel(Message.CharCode, TabText) and CanShowTab(Pages[i].TabIndex) and CanFocus then
    begin
      Message.Result := 1;
      if CanChange then
      begin
        TabIndex := Pages[i].TabIndex;
        Change;
      end;
      Exit;
    end;
  end;
  Broadcast(Message);
end;

procedure TTntPageControl.CMDockClient(var Message: TCMDockClient);
var
  IsVisible: Boolean;
  DockCtl: TControl;
begin
  Message.Result := 0;
  FNewDockSheet := TTntTabSheet.Create(Self);
  try
    try
      DockCtl := Message.DockSource.Control;
      if DockCtl is TCustomForm then
        FNewDockSheet.Caption := TntControl_GetText(DockCtl);
      FNewDockSheet.PageControl := Self;
      DockCtl.Dock(Self, Message.DockSource.DockRect);
    except
      FNewDockSheet.Free;
      raise;
    end;
    IsVisible := DockCtl.Visible;
    FNewDockSheet.TabVisible := IsVisible;
    if IsVisible then ActivePage := FNewDockSheet;
    DockCtl.Align := alClient;
  finally
    FNewDockSheet := nil;
  end;
end;

procedure TTntPageControl.DoAddDockClient(Client: TControl; const ARect: TRect);
begin
  if FNewDockSheet <> nil then
    Client.Parent := FNewDockSheet;
end;

procedure TTntPageControl.CMDockNotification(var Message: TCMDockNotification);
var
  I: Integer;
  S: WideString;
  Page: TTabSheet{TNT-ALLOW TTabSheet};
begin
  Page := GetPageFromDockClient(Message.Client);
  if (Message.NotifyRec.ClientMsg <> WM_SETTEXT)
  or (Page = nil) or (not (Page is TTntTabSheet)) then
    inherited
  else begin
    if (Message.Client is TWinControl)
    and (TWinControl(Message.Client).HandleAllocated)
    and IsWindowUnicode(TWinControl(Message.Client).Handle) then
      S := PWideChar(Message.NotifyRec.MsgLParam)
    else
      S := PAnsiChar(Message.NotifyRec.MsgLParam);
    { Search for first CR/LF and end string there }
    for I := 1 to Length(S) do
      if S[I] in [CR, LF] then
      begin
        SetLength(S, I - 1);
        Break;
      end;
    TTntTabSheet(Page).Caption := S;
  end;
end;

procedure TTntPageControl.ActionChange(Sender: TObject; CheckDefaults: Boolean);
begin
  TntControl_BeforeInherited_ActionChange(Self, Sender, CheckDefaults);
  inherited;
end;

function TTntPageControl.GetActionLinkClass: TControlActionLinkClass;
begin
  Result := TntControl_GetActionLinkClass(Self, inherited GetActionLinkClass);
end;

{ TTntTrackBar }

procedure TTntTrackBar.CreateWindowHandle(const Params: TCreateParams);
begin
  CreateUnicodeHandle_ComCtl(Self, Params, TRACKBAR_CLASS);
end;

procedure TTntTrackBar.DefineProperties(Filer: TFiler);
begin
  inherited;
  TntPersistent_AfterInherited_DefineProperties(Filer, Self);
end;

function TTntTrackBar.IsHintStored: Boolean;
begin
  Result := TntControl_IsHintStored(Self);
end;

function TTntTrackBar.GetHint: WideString;
begin
  Result := TntControl_GetHint(Self);
end;

procedure TTntTrackBar.SetHint(const Value: WideString);
begin
  TntControl_SetHint(Self, Value);
end;

procedure TTntTrackBar.ActionChange(Sender: TObject; CheckDefaults: Boolean);
begin
  TntControl_BeforeInherited_ActionChange(Self, Sender, CheckDefaults);
  inherited;
end;

function TTntTrackBar.GetActionLinkClass: TControlActionLinkClass;
begin
  Result := TntControl_GetActionLinkClass(Self, inherited GetActionLinkClass);
end;

{ TTntProgressBar }

procedure TTntProgressBar.CreateWindowHandle(const Params: TCreateParams);
begin
  CreateUnicodeHandle_ComCtl(Self, Params, PROGRESS_CLASS);
end;

procedure TTntProgressBar.DefineProperties(Filer: TFiler);
begin
  inherited;
  TntPersistent_AfterInherited_DefineProperties(Filer, Self);
end;

function TTntProgressBar.IsHintStored: Boolean;
begin
  Result := TntControl_IsHintStored(Self);
end;

function TTntProgressBar.GetHint: WideString;
begin
  Result := TntControl_GetHint(Self);
end;

procedure TTntProgressBar.SetHint(const Value: WideString);
begin
  TntControl_SetHint(Self, Value);
end;

procedure TTntProgressBar.ActionChange(Sender: TObject; CheckDefaults: Boolean);
begin
  TntControl_BeforeInherited_ActionChange(Self, Sender, CheckDefaults);
  inherited;
end;

function TTntProgressBar.GetActionLinkClass: TControlActionLinkClass;
begin
  Result := TntControl_GetActionLinkClass(Self, inherited GetActionLinkClass);
end;

{ TTntCustomUpDown }

procedure TTntCustomUpDown.CreateWindowHandle(const Params: TCreateParams);
begin
  CreateUnicodeHandle_ComCtl(Self, Params, UPDOWN_CLASS);
end;

procedure TTntCustomUpDown.DefineProperties(Filer: TFiler);
begin
  inherited;
  TntPersistent_AfterInherited_DefineProperties(Filer, Self);
end;

function TTntCustomUpDown.IsHintStored: Boolean;
begin
  Result := TntControl_IsHintStored(Self);
end;

function TTntCustomUpDown.GetHint: WideString;
begin
  Result := TntControl_GetHint(Self);
end;

procedure TTntCustomUpDown.SetHint(const Value: WideString);
begin
  TntControl_SetHint(Self, Value);
end;

procedure TTntCustomUpDown.ActionChange(Sender: TObject; CheckDefaults: Boolean);
begin
  TntControl_BeforeInherited_ActionChange(Self, Sender, CheckDefaults);
  inherited;
end;

function TTntCustomUpDown.GetActionLinkClass: TControlActionLinkClass;
begin
  Result := TntControl_GetActionLinkClass(Self, inherited GetActionLinkClass);
end;

{ TTntDateTimePicker }

procedure TTntDateTimePicker.CreateWindowHandle(const Params: TCreateParams);
begin
  CreateUnicodeHandle_ComCtl(Self, Params, DATETIMEPICK_CLASS);
end;

procedure TTntDateTimePicker.DefineProperties(Filer: TFiler);
begin
  inherited;
  TntPersistent_AfterInherited_DefineProperties(Filer, Self);
end;

function TTntDateTimePicker.IsHintStored: Boolean;
begin
  Result := TntControl_IsHintStored(Self);
end;

function TTntDateTimePicker.GetHint: WideString;
begin
  Result := TntControl_GetHint(Self);
end;

procedure TTntDateTimePicker.SetHint(const Value: WideString);
begin
  TntControl_SetHint(Self, Value);
end;

procedure TTntDateTimePicker.ActionChange(Sender: TObject; CheckDefaults: Boolean);
begin
  TntControl_BeforeInherited_ActionChange(Self, Sender, CheckDefaults);
  inherited;
end;

function TTntDateTimePicker.GetActionLinkClass: TControlActionLinkClass;
begin
  Result := TntControl_GetActionLinkClass(Self, inherited GetActionLinkClass);
end;

{ TTntMonthCalendar }

procedure TTntMonthCalendar.CreateWindowHandle(const Params: TCreateParams);
begin
  CreateUnicodeHandle_ComCtl(Self, Params, MONTHCAL_CLASS);
  if Win32PlatformIsUnicode then begin
    { For some reason WM_NOTIFY:MCN_GETDAYSTATE never gets called. }
    ForceGetMonthInfo;
  end;
end;

procedure TTntMonthCalendar.ForceGetMonthInfo;
var
  Hdr: TNMDayState;
  Days: array of TMonthDayState;
  Range: array[1..2] of TSystemTime;
begin
  // populate Days array
  Hdr.nmhdr.hwndFrom := Handle;
  Hdr.nmhdr.idFrom := 0;
  Hdr.nmhdr.code := MCN_GETDAYSTATE;
  Hdr.cDayState := MonthCal_GetMonthRange(Handle, GMR_DAYSTATE, @Range[1]);
  Hdr.stStart := Range[1];
  SetLength(Days, Hdr.cDayState);
  Hdr.prgDayState := @Days[0];
  SendMessage(Handle, CN_NOTIFY, Handle, Integer(@Hdr));
  // update day state
  SendMessage(Handle, MCM_SETDAYSTATE, Hdr.cDayState, Longint(Hdr.prgDayState))
end;

procedure TTntMonthCalendar.DefineProperties(Filer: TFiler);
begin
  inherited;
  TntPersistent_AfterInherited_DefineProperties(Filer, Self);
end;

function TTntMonthCalendar.IsHintStored: Boolean;
begin
  Result := TntControl_IsHintStored(Self);
end;

function TTntMonthCalendar.GetHint: WideString;
begin
  Result := TntControl_GetHint(Self);
end;

procedure TTntMonthCalendar.SetHint(const Value: WideString);
begin
  TntControl_SetHint(Self, Value);
end;

function TTntMonthCalendar.GetDate: TDate;
begin
  Result := Trunc(inherited Date); { Fixes issue where Date always reflects time of saving dfm. }
end;

procedure TTntMonthCalendar.SetDate(const Value: TDate);
begin
  inherited Date := Trunc(Value);
end;

procedure TTntMonthCalendar.ActionChange(Sender: TObject; CheckDefaults: Boolean);
begin
  TntControl_BeforeInherited_ActionChange(Self, Sender, CheckDefaults);
  inherited;
end;

function TTntMonthCalendar.GetActionLinkClass: TControlActionLinkClass;
begin
  Result := TntControl_GetActionLinkClass(Self, inherited GetActionLinkClass);
end;

{ TTntPageScroller }

procedure TTntPageScroller.CreateWindowHandle(const Params: TCreateParams);
begin
  CreateUnicodeHandle_ComCtl(Self, Params, WC_PAGESCROLLER);
end;

procedure TTntPageScroller.DefineProperties(Filer: TFiler);
begin
  inherited;
  TntPersistent_AfterInherited_DefineProperties(Filer, Self);
end;

function TTntPageScroller.IsHintStored: Boolean;
begin
  Result := TntControl_IsHintStored(Self);
end;

function TTntPageScroller.GetHint: WideString;
begin
  Result := TntControl_GetHint(Self);
end;

procedure TTntPageScroller.SetHint(const Value: WideString);
begin
  TntControl_SetHint(Self, Value);
end;

procedure TTntPageScroller.ActionChange(Sender: TObject; CheckDefaults: Boolean);
begin
  TntControl_BeforeInherited_ActionChange(Self, Sender, CheckDefaults);
  inherited;
end;

function TTntPageScroller.GetActionLinkClass: TControlActionLinkClass;
begin
  Result := TntControl_GetActionLinkClass(Self, inherited GetActionLinkClass);
end;

{ TTntStatusPanel }

procedure TTntStatusPanel.Assign(Source: TPersistent);
begin
  inherited;
  if Source is TTntStatusPanel then
    Text := TTntStatusPanel(Source).Text;
end;

procedure TTntStatusPanel.DefineProperties(Filer: TFiler);
begin
  inherited;
  TntPersistent_AfterInherited_DefineProperties(Filer, Self);
end;

function TTntStatusPanel.GetText: Widestring;
begin
  Result := GetSyncedWideString(FText, inherited Text);
end;

procedure TTntStatusPanel.SetInheritedText(const Value: AnsiString);
begin
  inherited Text := Value;
end;

procedure TTntStatusPanel.SetText(const Value: Widestring);
begin
  SetSyncedWideString(Value, FText, inherited Text, SetInheritedText);
end;

{ TTntStatusPanels }

{$IFNDEF COMPILER_6_UP}
constructor TTntStatusPanels.Create(StatusBar: TStatusBar{TNT-ALLOW TStatusBar});
begin
  inherited;
  Assert(THackCollection(Self).FItemClass = ItemClass, 'Internal Error in TTntStatusPanels.Create().');
  THackCollection(Self).FItemClass := TTntStatusPanel;
end;
{$ENDIF}

function TTntStatusPanels.GetItem(Index: Integer): TTntStatusPanel;
begin
  Result := (inherited GetItem(Index)) as TTntStatusPanel;
end;

procedure TTntStatusPanels.SetItem(Index: Integer; Value: TTntStatusPanel);
begin
  inherited SetItem(Index, Value);
end;

function TTntStatusPanels.Add: TTntStatusPanel;
begin
  Result := (inherited Add) as TTntStatusPanel;
end;

{$IFDEF COMPILER_6_UP}
function TTntStatusPanels.AddItem(Item: TTntStatusPanel; Index: Integer): TTntStatusPanel;
begin
  Result := (inherited AddItem(Item, Index)) as TTntStatusPanel;
end;

function TTntStatusPanels.Insert(Index: Integer): TTntStatusPanel;
begin
  Result := (inherited Insert(Index)) as TTntStatusPanel;
end;
{$ENDIF}

{$IFDEF COMPILER_5} // verified against VCL source in Delphi 5 and BCB 5
type
  THackStatusBar = class(TWinControl)
  private
    FPanels: TStatusPanels{TNT-ALLOW TStatusPanels};
  end;
{$ENDIF}

{ TTntCustomStatusBar }

constructor TTntCustomStatusBar.Create(AOwner: TComponent);
begin
  inherited;
  {$IFNDEF COMPILER_6_UP}
  THackStatusBar(Self).FPanels.Free;
  THackStatusBar(Self).FPanels := TTntStatusPanels.Create(Self);
  {$ENDIF}
end;

function TTntCustomStatusBar.GetHint: WideString;
begin
  Result := TntControl_GetHint(Self);
end;

procedure TTntCustomStatusBar.SetHint(const Value: WideString);
begin
  TntControl_SetHint(Self, Value);
end;

function TTntCustomStatusBar.IsHintStored: Boolean;
begin
  Result := TntControl_IsHintStored(Self);
end;

{$IFDEF COMPILER_6_UP}
function TTntCustomStatusBar.CreatePanels: TStatusPanels{TNT-ALLOW TStatusPanels};
begin
  Result := TTntStatusPanels.Create(Self);
end;

function TTntCustomStatusBar.GetPanelClass: TStatusPanelClass;
begin
  Result := TTntStatusPanel;
end;
{$ENDIF}

function TTntCustomStatusBar.SyncLeadingTabs(const WideVal: WideString; const AnsiVal: AnsiString): WideString;

  function CountLeadingTabs(const Val: WideString): Integer;
  var
    i: integer;
  begin
    Result := 0;
    for i := 1 to Length(Val) do begin
      if Val[i] <> #9 then break;
      Inc(Result);
    end;
  end;

var
  AnsiTabCount: Integer;
  WideTabCount: Integer;
begin
  AnsiTabCount := CountLeadingTabs(AnsiVal);
  WideTabCount := CountLeadingTabs(WideVal);
  Result := WideVal;
  while WideTabCount < AnsiTabCount do begin
    Insert(#9, Result, 1);
    Inc(WideTabCount);
  end;
  while WideTabCount > AnsiTabCount do begin
    Delete(Result, 1, 1);
    Dec(WideTabCount);
  end;
end;

function TTntCustomStatusBar.GetSimpleText: WideString;
begin
  FSimpleText := SyncLeadingTabs(FSimpleText, inherited SimpleText);
  Result := GetSyncedWideString(FSimpleText, inherited SimpleText);
end;

procedure TTntCustomStatusBar.SetInheritedSimpleText(const Value: AnsiString);
begin
  inherited SimpleText := Value;
end;

procedure TTntCustomStatusBar.SetSimpleText(const Value: WideString);
begin
  SetSyncedWideString(Value, FSimpleText, inherited SimpleText, SetInheritedSimpleText);
end;

procedure TTntCustomStatusBar.DefineProperties(Filer: TFiler);
begin
  inherited;
  TntPersistent_AfterInherited_DefineProperties(Filer, Self);
end;

procedure TTntCustomStatusBar.CreateWindowHandle(const Params: TCreateParams);
begin
  CreateUnicodeHandle_ComCtl(Self, Params, STATUSCLASSNAME);
end;

procedure TTntCustomStatusBar.WndProc(var Msg: TMessage);
const
  SB_SIMPLEID = Integer($FF);
var
  iPart: Integer;
  szText: PAnsiChar;
  WideText: WideString;
begin
  if Win32PlatformIsUnicode and (Msg.Msg = SB_SETTEXTA) and ((Msg.WParam and SBT_OWNERDRAW) = 0)
  then begin
    // convert SB_SETTEXTA message to Unicode
    iPart := (Msg.WParam and SB_SIMPLEID);
    szText := PAnsiChar(Msg.LParam);
    if iPart = SB_SIMPLEID then
      WideText := SimpleText
    else if Panels.Count > 0 then
      WideText := Panels[iPart].Text
    else begin
      WideText := szText;
    end;
    WideText := SyncLeadingTabs(WideText, szText);
    Msg.Result := SendMessageW(Handle, SB_SETTEXTW, Msg.wParam, Integer(PWideChar(WideText)));
  end else
    inherited;
end;

procedure TTntCustomStatusBar.WMGetTextLength(var Message: TWMGetTextLength);
begin
  Message.Result := Length(SimpleText);
end;

procedure TTntCustomStatusBar.ActionChange(Sender: TObject; CheckDefaults: Boolean);
begin
  TntControl_BeforeInherited_ActionChange(Self, Sender, CheckDefaults);
  inherited;
end;

function TTntCustomStatusBar.GetActionLinkClass: TControlActionLinkClass;
begin
  Result := TntControl_GetActionLinkClass(Self, inherited GetActionLinkClass);
end;

function TTntCustomStatusBar.GetPanels: TTntStatusPanels;
begin
  Result := inherited Panels as TTntStatusPanels;
end;

procedure TTntCustomStatusBar.SetPanels(const Value: TTntStatusPanels);
begin
  inherited Panels := Value;
end;

function TTntCustomStatusBar.ExecuteAction(Action: TBasicAction): Boolean;
begin
  if AutoHint and (Action is TTntHintAction) and not DoHint then
  begin
    if SimplePanel or (Panels.Count = 0) then
      SimpleText := TTntHintAction(Action).Hint else
      Panels[0].Text := TTntHintAction(Action).Hint;
    Result := True;
  end
  else Result := inherited ExecuteAction(Action);
end;

{$IFNDEF COMPILER_6_UP}
procedure TTntCustomStatusBar.FlipChildren(AllLevels: Boolean);
var
  Loop, FirstWidth, LastWidth: Integer;
  APanels: TTntStatusPanels;
begin
  if HandleAllocated and
     (not SimplePanel) and (Panels.Count > 0) then
  begin
    { Get the true width of the last panel }
    LastWidth := ClientWidth;
    FirstWidth := Panels[0].Width;
    for Loop := 0 to Panels.Count - 2 do Dec(LastWidth, Panels[Loop].Width);
    { Flip 'em }
    APanels := TTntStatusPanels.Create(Self);
    try
      for Loop := 0 to Panels.Count - 1 do with APanels.Add do
        Assign(Self.Panels[Loop]);
      for Loop := 0 to Panels.Count - 1 do
        Panels[Loop].Assign(APanels[Panels.Count - Loop - 1]);
    finally
      APanels.Free;
    end;
    { Set the width of the last panel }
    if Panels.Count > 1 then
    begin
      Panels[Panels.Count-1].Width := FirstWidth;
      Panels[0].Width := LastWidth;
    end;
    Perform(CM_BIDIMODECHANGED, 0, 0); // calls UpdatePanels(True, True)
  end;
end;
{$ENDIF}

{ TTntStatusBar }

{$IFDEF COMPILER_6_UP}
function TTntStatusBar.GetOnDrawPanel: TDrawPanelEvent;
begin
  Result := TDrawPanelEvent(inherited OnDrawPanel);
end;

procedure TTntStatusBar.SetOnDrawPanel(const Value: TDrawPanelEvent);
begin
  inherited OnDrawPanel := TCustomDrawPanelEvent(Value);
end;
{$ELSE}
function TTntStatusBar.IsFontStored: Boolean; // Delphi 5 compatibility
begin
  Result := not UseSystemFont and not ParentFont and not DesktopFont;
end;
{$ENDIF}

{ TTntTreeNode }

procedure TTntTreeNode.Assign(Source: TPersistent);
var
  Node: TTntTreeNode;
begin
  inherited;
  if (not Deleting) and (Source is TTntTreeNode) then
  begin
    Node := TTntTreeNode(Source);
    Text := Node.Text;
  end;
end;

function TTntTreeNode.GetText: WideString;
begin
  Result := GetSyncedWideString(FText, inherited Text);
end;

procedure TTntTreeNode.SetInheritedText(const Value: AnsiString);
begin
  inherited Text := Value;
end;

procedure TTntTreeNode.SetText(const Value: WideString);
begin
  SetSyncedWideString(Value, FText, inherited Text, SetInheritedText);
end;

function TTntTreeNode.getFirstChild: TTntTreeNode;
begin
  Result := inherited getFirstChild as TTntTreeNode;
end;

function TTntTreeNode.GetItem(Index: Integer): TTntTreeNode;
begin
  Result := inherited Item[Index] as TTntTreeNode;
end;

procedure TTntTreeNode.SetItem(Index: Integer; const Value: TTntTreeNode);
begin
  inherited Item[Index] := Value;
end;

function TTntTreeNode.GetLastChild: TTntTreeNode;
begin
  Result := inherited GetLastChild as TTntTreeNode;
end;

function TTntTreeNode.GetNext: TTntTreeNode;
begin
  Result := inherited GetNext as TTntTreeNode;
end;

function TTntTreeNode.GetNextChild(Value: TTntTreeNode): TTntTreeNode;
begin
  Result := inherited GetNextChild(Value) as TTntTreeNode;
end;

function TTntTreeNode.getNextSibling: TTntTreeNode;
begin
  Result := inherited getNextSibling  as TTntTreeNode;
end;

function TTntTreeNode.GetNextVisible: TTntTreeNode;
begin
  Result := inherited GetNextVisible as TTntTreeNode;
end;

function TTntTreeNode.GetNodeOwner: TTntTreeNodes;
begin
  Result := inherited Owner as TTntTreeNodes;
end;

function TTntTreeNode.GetParent: TTntTreeNode;
begin
  Result := inherited Parent as TTntTreeNode;
end;

function TTntTreeNode.GetPrev: TTntTreeNode;
begin
  Result := inherited GetPrev as TTntTreeNode;
end;

function TTntTreeNode.GetPrevChild(Value: TTntTreeNode): TTntTreeNode;
begin
  Result := inherited GetPrevChild(Value) as TTntTreeNode;
end;

function TTntTreeNode.getPrevSibling: TTntTreeNode;
begin
  Result := inherited getPrevSibling as TTntTreeNode;
end;

function TTntTreeNode.GetPrevVisible: TTntTreeNode;
begin
  Result := inherited GetPrevVisible as TTntTreeNode;
end;

function TTntTreeNode.GetTreeView: TTntCustomTreeView;
begin
  Result := inherited TreeView as TTntCustomTreeView;
end;

{ TTntTreeNodes }

procedure SaveNodeTextToStrings(Nodes: TTntTreeNodes; sList: TTntStrings);
var
  ANode: TTntTreeNode;
begin
  sList.Clear;
  if Nodes.Count > 0 then
  begin
    ANode := Nodes[0];
    while ANode <> nil do
    begin
      sList.Add(ANode.Text);
      ANode := ANode.GetNext;
    end;
  end;
end;

procedure SaveStringsToNodeText(Nodes: TTntTreeNodes; sList: TTntStrings);
var
  ANode: TTntTreeNode;
  Idx: Integer;
begin
  if Nodes.Count > 0 then
  begin
    Nodes.BeginUpdate;
    try
      Idx := 0;
      ANode := Nodes[0];
      while ANode <> nil do
      begin
        if Idx <= sList.Count - 1 then
          ANode.FText := sList[Idx];
        ANode := ANode.GetNext;
        Inc(Idx);
      end;
    finally
      Nodes.EndUpdate;
    end;
  end;
end;

procedure TTntTreeNodes.Assign(Source: TPersistent);
var
  TreeNodes: TTntTreeNodes;
  SourceStrings: TTntStringList;
begin
  if Source is TTntTreeNodes then
  begin
    SourceStrings := TTntStringList.Create;
    try
      TreeNodes := TTntTreeNodes(Source);
      SaveNodeTextToStrings(TreeNodes, SourceStrings);
      BeginUpdate;
      try
        inherited;
        SaveStringsToNodeText(Self, SourceStrings);
      finally
        EndUpdate;
      end;
    finally
      SourceStrings.Free;
    end;
  end else
    inherited Assign(Source);
end;

function TTntTreeNodes.GetNodeFromIndex(Index: Integer): TTntTreeNode;
begin
  Result := inherited Item[Index] as TTntTreeNode;
end;

function TTntTreeNodes.AddChildFirst(Parent: TTntTreeNode; const S: WideString): TTntTreeNode;
begin
  {$IFDEF COMPILER_6_UP}
  Result := AddNode(nil, Parent, S, nil, naAddChildFirst);
  {$ELSE}
  Result := AddChildObjectFirst(Parent, S, nil);
  {$ENDIF}
end;

function TTntTreeNodes.AddChildObjectFirst(Parent: TTntTreeNode; const S: WideString;
  Ptr: Pointer): TTntTreeNode;
begin
  {$IFDEF COMPILER_6_UP}
  Result := AddNode(nil, Parent, S, Ptr, naAddChildFirst);
  {$ELSE}
  Result := InternalAddObject(Parent, S, Ptr, taAddFirst);
  {$ENDIF}
end;

function TTntTreeNodes.AddChild(Parent: TTntTreeNode; const S: WideString): TTntTreeNode;
begin
  {$IFDEF COMPILER_6_UP}
  Result := AddNode(nil, Parent, S, nil, naAddChild);
  {$ELSE}
  Result := AddChildObject(Parent, S, nil);
  {$ENDIF}
end;

function TTntTreeNodes.AddChildObject(Parent: TTntTreeNode; const S: WideString;
  Ptr: Pointer): TTntTreeNode;
begin
  {$IFDEF COMPILER_6_UP}
  Result := AddNode(nil, Parent, S, Ptr, naAddChild);
  {$ELSE}
  Result := InternalAddObject(Parent, S, Ptr, taAdd);
  {$ENDIF}
end;

function TTntTreeNodes.AddFirst(Sibling: TTntTreeNode; const S: WideString): TTntTreeNode;
begin
  {$IFDEF COMPILER_6_UP}
  Result := AddNode(nil, Sibling, S, nil, naAddFirst);
  {$ELSE}
  Result := AddObjectFirst(Sibling, S, nil);
  {$ENDIF}
end;

function TTntTreeNodes.AddObjectFirst(Sibling: TTntTreeNode; const S: WideString;
  Ptr: Pointer): TTntTreeNode;
begin
  {$IFDEF COMPILER_6_UP}
  Result := AddNode(nil, Sibling, S, Ptr, naAddFirst);
  {$ELSE}
  if Sibling <> nil then Sibling := Sibling.Parent;
  Result := InternalAddObject(Sibling, S, Ptr, taAddFirst);
  {$ENDIF}
end;

function TTntTreeNodes.Add(Sibling: TTntTreeNode; const S: WideString): TTntTreeNode;
begin
  {$IFDEF COMPILER_6_UP}
  Result := AddNode(nil, Sibling, S, nil, naAdd);
  {$ELSE}
  Result := AddObject(Sibling, S, nil);
  {$ENDIF}
end;

function TTntTreeNodes.AddObject(Sibling: TTntTreeNode; const S: WideString;
  Ptr: Pointer): TTntTreeNode;
begin
  {$IFDEF COMPILER_6_UP}
  Result := AddNode(nil, Sibling, S, Ptr, naAdd);
  {$ELSE}
  if Sibling <> nil then Sibling := Sibling.Parent;
  Result := InternalAddObject(Sibling, S, Ptr, taAdd);
  {$ENDIF}
end;

function TTntTreeNodes.Insert(Sibling: TTntTreeNode; const S: WideString): TTntTreeNode;
begin
  {$IFDEF COMPILER_6_UP}
  Result := AddNode(nil, Sibling, S, nil, naInsert);
  {$ELSE}
  Result := InsertObject(Sibling, S, nil);
  {$ENDIF}
end;

function TTntTreeNodes.InsertObject(Sibling: TTntTreeNode; const S: WideString;
  Ptr: Pointer): TTntTreeNode;
begin
  {$IFDEF COMPILER_6_UP}
  Result := AddNode(nil, Sibling, S, Ptr, naInsert);
  {$ELSE}
  Result := inherited InsertObject(Sibling, '', Ptr) as TTntTreeNode;
  Result.Text := S;
  {$ENDIF}
end;

{$IFDEF COMPILER_6_UP}

function TTntTreeNodes.InsertNode(Node, Sibling: TTntTreeNode; const S: WideString;
  Ptr: Pointer): TTntTreeNode;
begin
  Result := AddNode(Node, Sibling, S, Ptr, naInsert);
end;

function TTntTreeNodes.AddNode(Node, Relative: TTntTreeNode; const S: WideString;
  Ptr: Pointer; Method: TNodeAttachMode): TTntTreeNode;
begin
  Result := inherited AddNode(Node, Relative, '', Ptr, Method) as TTntTreeNode;
  Result.Text := S;
end;

{$ELSE}

function TTntTreeNodes.InternalAddObject(Node: TTntTreeNode; const S: WideString;
  Ptr: Pointer; AddMode: TAddMode): TTntTreeNode;
begin
  Result := inherited InternalAddObject(Node, '', Ptr, AddMode) as TTntTreeNode;
  Result.Text := S;
end;

{$ENDIF}

function TTntTreeNodes.GetNode(ItemId: HTreeItem): TTntTreeNode;
begin
  Result := inherited GetNode(ItemID) as TTntTreeNode;
end;

function TTntTreeNodes.GetFirstNode: TTntTreeNode;
begin
  Result := inherited GetFirstNode as TTntTreeNode;
end;

function TTntTreeNodes.GetNodesOwner: TTntCustomTreeView;
begin
  Result := inherited Owner as TTntCustomTreeView;
end;

{ TTntTreeStrings }

type
  TTntTreeStrings = class(TTntStringList)
  protected
    function GetBufStart(Buffer: PWideChar; var Level: Integer): PWideChar;
  public
    procedure SaveToTree(Tree: TTntCustomTreeView);
    procedure LoadFromTree(Tree: TTntCustomTreeView);
  end;

function TTntTreeStrings.GetBufStart(Buffer: PWideChar; var Level: Integer): PWideChar;
begin
  Level := 0;
  while Buffer^ in [WideChar(' '), WideChar(#9)] do
  begin
    Inc(Buffer);
    Inc(Level);
  end;
  Result := Buffer;
end;

procedure TTntTreeStrings.SaveToTree(Tree: TTntCustomTreeView);
var
  ANode, NextNode: TTntTreeNode;
  ALevel, i: Integer;
  CurrStr: WideString;
  Owner: TTntTreeNodes;
begin
  Owner := Tree.Items;
  Owner.BeginUpdate;
  try
    try
      Owner.Clear;
      ANode := nil;
      for i := 0 to Count - 1 do
      begin
        CurrStr := GetBufStart(PWideChar(Strings[i]), ALevel);
        if ANode = nil then
          ANode := Owner.AddChild(nil, CurrStr)
        else if ANode.Level = ALevel then
          ANode := Owner.AddChild(ANode.Parent, CurrStr)
        else if ANode.Level = (ALevel - 1) then
          ANode := Owner.AddChild(ANode, CurrStr)
        else if ANode.Level > ALevel then
        begin
          NextNode := ANode.Parent;
          while NextNode.Level > ALevel do
            NextNode := NextNode.Parent;
          ANode := Owner.AddChild(NextNode.Parent, CurrStr);
        end
        else
          raise ETreeViewError.CreateFmt(sInvalidLevelEx, [ALevel, CurrStr]);
      end;
    finally
      Owner.EndUpdate;
    end;
  except
    Owner.Owner.Invalidate;  // force repaint on exception
    raise;
  end;
end;

procedure TTntTreeStrings.LoadFromTree(Tree: TTntCustomTreeView);
const
  TabChar = #9;
var
  i: Integer;
  ANode: TTntTreeNode;
  NodeStr: WideString;
  Owner: TTntTreeNodes;
begin
  Clear;
  Owner := Tree.Items;
  if Owner.Count > 0 then
  begin
    ANode := Owner[0];
    while ANode <> nil do
    begin
      NodeStr := '';
      for i := 0 to ANode.Level - 1 do NodeStr := NodeStr + TabChar;
      NodeStr := NodeStr + ANode.Text;
      Add(NodeStr);
      ANode := ANode.GetNext;
    end;
  end;
end;

{$IFDEF COMPILER_5_UP}
type
  THackCustomTreeView = class(TWinControl)
  protected
    F_xxxxxxx_AutoExpand: Boolean;
    F_xxxxxxx_BorderStyle: TBorderStyle;
    F_xxxxxxx_Canvas: TCanvas;
    F_xxxxxxx_CanvasChanged: Boolean;
    F_xxxxxxx_DefEditProc: Pointer;
    F_xxxxxxx_Dragged: Boolean;
    F_xxxxxxx_DragImage: TDragImageList;
    F_xxxxxxx_DragNode: TTreeNode{TNT-ALLOW TTreeNode};
    F_xxxxxxx_EditHandle: HWND;
    F_xxxxxxx_EditInstance: Pointer;
    F_xxxxxxx_HideSelection: Boolean;
    F_xxxxxxx_HotTrack: Boolean;
    F_xxxxxxx_ImageChangeLink: TChangeLink;
    F_xxxxxxx_Images: TCustomImageList;
    F_xxxxxxx_LastDropTarget: TTreeNode{TNT-ALLOW TTreeNode};
    F_xxxxxxx_MemStream: TMemoryStream{TNT-ALLOW TMemoryStream};
    F_xxxxxxx_RClickNode: TTreeNode{TNT-ALLOW TTreeNode};
    F_xxxxxxx_RightClickSelect: Boolean;
    F_xxxxxxx_ManualNotify: Boolean;
    F_xxxxxxx_ReadOnly: Boolean;
    F_xxxxxxx_RowSelect: Boolean;
    F_xxxxxxx_SaveIndex: Integer;
    F_xxxxxxx_SaveIndent: Integer;
    F_xxxxxxx_SaveItems: TStringList{TNT-ALLOW TStringList};
    F_xxxxxxx_SaveTopIndex: Integer;
    F_xxxxxxx_ShowButtons: Boolean;
    F_xxxxxxx_ShowLines: Boolean;
    F_xxxxxxx_ShowRoot: Boolean;
    F_xxxxxxx_SortType: TSortType;
    F_xxxxxxx_StateChanging: Boolean;
    F_xxxxxxx_StateImages: TCustomImageList;
    F_xxxxxxx_StateChangeLink: TChangeLink;
    F_xxxxxxx_ToolTips: Boolean;
    FTreeNodes: TTreeNodes{TNT-ALLOW TTreeNodes};
  end;
{$ENDIF}

{ _TntInternalCustomTreeView }

{$IFDEF COMPILER_6_UP}
function _TntInternalCustomTreeView.FindNextToSelect: TTreeNode{TNT-ALLOW TTreeNode};
begin
  Result := Wide_FindNextToSelect;
end;

function _TntInternalCustomTreeView.Inherited_FindNextToSelect: TTreeNode{TNT-ALLOW TTreeNode};
begin
  Result := inherited FindNextToSelect;
end;
{$ENDIF}

{ TTntCustomTreeView }

function TntDefaultTreeViewSort(Node1, Node2: TTntTreeNode; lParam: Integer): Integer; stdcall;
begin
  with Node1 do
    if Assigned(TreeView.OnCompare) then
      TreeView.OnCompare(TreeView, Node1, Node2, lParam, Result)
    else Result := lstrcmpw(PWideChar(Node1.Text), PWideChar(Node2.Text));
end;

constructor TTntCustomTreeView.Create(AOwner: TComponent);
begin
  inherited;
  FEditInstance := TntClasses.MakeObjectInstance(EditWndProcW);
  {$IFNDEF COMPILER_6_UP}
  // create nodes
  Assert(THackCustomTreeView(Self).FTreeNodes = inherited Items, 'Internal Error in TTntCustomTreeView.Create().');
  FreeAndNil(THackCustomTreeView(Self).FTreeNodes);
  THackCustomTreeView(Self).FTreeNodes := TTntTreeNodes.Create(Self);
  {$ENDIF}
end;

destructor TTntCustomTreeView.Destroy;
begin
  TntClasses.FreeObjectInstance(FEditInstance);
  inherited;
end;

var
  ComCtrls_DefaultTreeViewSort: TTVCompare = nil;

procedure TTntCustomTreeView.CreateWindowHandle(const Params: TCreateParams);

  procedure Capture_ComCtrls_DefaultTreeViewSort;
  begin
    FTestingForSortProc := True;
    try
      AlphaSort;
    finally
      FTestingForSortProc := False;
    end;
  end;

begin
  CreateUnicodeHandle_ComCtl(Self, Params, WC_TREEVIEW);
  if (Win32PlatformIsUnicode) then begin
    if not Assigned(ComCtrls_DefaultTreeViewSort) then
      Capture_ComCtrls_DefaultTreeViewSort;
  end;
end;

procedure TTntCustomTreeView.CreateWnd;
begin
  inherited;
  FreeAndNil(FSavedNodeText);
end;

procedure TTntCustomTreeView.DestroyWnd;
begin
  FSavedNodeText := TTntStringList.Create;
  SaveNodeTextToStrings(Items, FSavedNodeText);
  inherited;
end;

procedure TTntCustomTreeView.DefineProperties(Filer: TFiler);
begin
  inherited;
  TntPersistent_AfterInherited_DefineProperties(Filer, Self);
end;

function TTntCustomTreeView.IsHintStored: Boolean;
begin
  Result := TntControl_IsHintStored(Self);
end;

function TTntCustomTreeView.GetHint: WideString;
begin
  Result := TntControl_GetHint(Self)
end;

procedure TTntCustomTreeView.SetHint(const Value: WideString);
begin
  TntControl_SetHint(Self, Value);
end;

procedure TTntCustomTreeView.ActionChange(Sender: TObject; CheckDefaults: Boolean);
begin
  TntControl_BeforeInherited_ActionChange(Self, Sender, CheckDefaults);
  inherited;
end;

function TTntCustomTreeView.GetActionLinkClass: TControlActionLinkClass;
begin
  Result := TntControl_GetActionLinkClass(Self, inherited GetActionLinkClass);
end;

function TTntCustomTreeView.CreateNode: TTreeNode{TNT-ALLOW TTreeNode};
var
  LClass: TClass;
  TntLClass: TTntTreeNodeClass;
begin
  LClass := TTntTreeNode;
  {$IFDEF COMPILER_6_UP}
  if Assigned(OnCreateNodeClass) then
    OnCreateNodeClass(Self, TTreeNodeClass(LClass));
  if not LClass.InheritsFrom(TTntTreeNode) then
    raise Exception.Create('Internal Error: OnCreateNodeClass.ItemClass must inherit from TTntTreeNode.');
  {$ENDIF}
  TntLClass := TTntTreeNodeClass(LClass);
  Result := TntLClass.Create(inherited Items);
end;

{$IFDEF COMPILER_6_UP}
function TTntCustomTreeView.CreateNodes: TTreeNodes{TNT-ALLOW TTreeNodes};
begin
  Result := TTntTreeNodes.Create(Self);
end;
{$ENDIF}

function TTntCustomTreeView.GetTreeNodes: TTntTreeNodes;
begin
  Result := inherited Items as TTntTreeNodes;
end;

procedure TTntCustomTreeView.SetTreeNodes(const Value: TTntTreeNodes);
begin
  Items.Assign(Value);
end;

function TTntCustomTreeView.GetNodeFromItem(const Item: TTVItem): TTntTreeNode;
begin
  Result := nil;
  if Items <> nil then
    with Item do
      if (state and TVIF_PARAM) <> 0 then
        Result := Pointer(lParam)
      else
        Result := Items.GetNode(hItem);
end;

{$IFDEF COMPILER_6_UP}
function TTntCustomTreeView.Wide_FindNextToSelect: TTntTreeNode;
begin
  Result := FindNextToSelect;
end;

function TTntCustomTreeView.FindNextToSelect: TTntTreeNode;
begin
  Result := Inherited_FindNextToSelect as TTntTreeNode;
end;
{$ENDIF}

function TTntCustomTreeView.GetDropTarget: TTntTreeNode;
begin
  Result := inherited DropTarget as TTntTreeNode;
end;

function TTntCustomTreeView.GetNodeAt(X, Y: Integer): TTntTreeNode;
begin
  Result := inherited GetNodeAt(X, Y) as TTntTreeNode;
end;

function TTntCustomTreeView.GetSelected: TTntTreeNode;
begin
  Result := inherited Selected as TTntTreeNode;
end;

{$IFDEF COMPILER_6_UP}
function TTntCustomTreeView.GetSelection(Index: Integer): TTntTreeNode;
begin
  Result := inherited Selections[Index] as TTntTreeNode;
end;

function TTntCustomTreeView.GetSelections(AList: TList): TTntTreeNode;
begin
  Result := inherited GetSelections(AList) as TTntTreeNode;
end;
{$ENDIF}

function TTntCustomTreeView.GetTopItem: TTntTreeNode;
begin
  Result := inherited TopItem as TTntTreeNode;
end;

procedure TTntCustomTreeView.SetDropTarget(const Value: TTntTreeNode);
begin
  inherited DropTarget := Value;
end;

procedure TTntCustomTreeView.SetSelected(const Value: TTntTreeNode);
begin
  inherited Selected := Value;
end;

procedure TTntCustomTreeView.SetTopItem(const Value: TTntTreeNode);
begin
  inherited TopItem := Value;
end;

procedure TTntCustomTreeView.WndProc(var Message: TMessage);
type
  PTVSortCB = ^TTVSortCB;
begin
  with Message do begin
    // capture ANSI version of DefaultTreeViewSort from ComCtrls
    if (FTestingForSortProc)
    and (Msg = TVM_SORTCHILDRENCB) then begin
      ComCtrls_DefaultTreeViewSort := PTVSortCB(lParam).lpfnCompare;
      exit;
    end;

    if (Win32PlatformIsUnicode)
    and (Msg = TVM_SORTCHILDRENCB)
    and (@PTVSortCB(lParam).lpfnCompare = @ComCtrls_DefaultTreeViewSort) then
    begin
      // Unicode:: call wide version of sort proc instead
      PTVSortCB(lParam)^.lpfnCompare := TTVCompare(@TntDefaultTreeViewSort);
      Result := SendMessageW(Handle, TVM_SORTCHILDRENCB, wParam, lParam);
    end else
      inherited;
  end;
end;

procedure TTntCustomTreeView.CNNotify(var Message: TWMNotify);
var
  Node: TTntTreeNode;
begin
  if (not Win32PlatformIsUnicode) then
    inherited
  else begin
    with Message do begin
      case NMHdr^.code of
        TVN_BEGINDRAGW:
          begin
            NMHdr^.code := TVN_BEGINDRAGA;
            try
              inherited;
            finally
              NMHdr^.code := TVN_BEGINDRAGW;
            end;
          end;
        TVN_BEGINLABELEDITW:
          begin
            with PTVDispInfo(NMHdr)^ do
              if Dragging or not CanEdit(GetNodeFromItem(item)) then
                Result := 1;
            if Result = 0 then
            begin
              FEditHandle := TreeView_GetEditControl(Handle);
              FDefEditProc := Pointer(GetWindowLongW(FEditHandle, GWL_WNDPROC));
              SetWindowLongW(FEditHandle, GWL_WNDPROC, LongInt(FEditInstance));
            end;
          end;
        TVN_ENDLABELEDITW:
          Edit(PTVDispInfo(NMHdr)^.item);
        TVN_ITEMEXPANDINGW:
          begin
            NMHdr^.code := TVN_ITEMEXPANDINGA;
            try
              inherited;
            finally
              NMHdr^.code := TVN_ITEMEXPANDINGW;
            end;
          end;
        TVN_ITEMEXPANDEDW:
          begin
            NMHdr^.code := TVN_ITEMEXPANDEDA;
            try
              inherited;
            finally
              NMHdr^.code := TVN_ITEMEXPANDEDW;
            end;
          end;
        TVN_DELETEITEMW:
          begin
            NMHdr^.code := TVN_DELETEITEMA;
            try
              inherited;
            finally
              NMHdr^.code := TVN_DELETEITEMW;
            end;
          end;
        TVN_SETDISPINFOW:
          with PTVDispInfo(NMHdr)^ do
          begin
            Node := GetNodeFromItem(item);
            if (Node <> nil) and ((item.mask and TVIF_TEXT) <> 0) then
              Node.Text := TTVItemW(item).pszText;
          end;
        TVN_GETDISPINFOW:
          with PTVDispInfo(NMHdr)^ do
          begin
            Node := GetNodeFromItem(item);
            if Node <> nil then
            begin
              if (item.mask and TVIF_TEXT) <> 0 then begin
                if (FSavedNodeText <> nil)
                and (FSavedNodeText.Count > 0)
                and (AnsiString(FSavedNodeText[0]) = AnsiString(Node.Text)) then
                begin
                  Node.FText := FSavedNodeText[0]; // recover saved text
                  FSavedNodeText.Delete(0);
                end;
                StrLCopyW(TTVItemW(item).pszText, PWideChar(Node.Text), item.cchTextMax - 1);
              end;

              if (item.mask and TVIF_IMAGE) <> 0 then
              begin
                GetImageIndex(Node);
                item.iImage := Node.ImageIndex;
              end;
              if (item.mask and TVIF_SELECTEDIMAGE) <> 0 then
              begin
                GetSelectedIndex(Node);
                item.iSelectedImage := Node.SelectedIndex;
              end;
            end;
          end;
        else
          inherited;
      end;
    end;
  end;
end;

procedure TTntCustomTreeView.WMNotify(var Message: TWMNotify);
var
  Node: TTntTreeNode;
  FWideText: WideString;
  MaxTextLen: Integer;
  Pt: TPoint;
begin
  with Message do
    if NMHdr^.code = TTN_NEEDTEXTW then
    begin
      // Work around NT COMCTL32 problem with tool tips >= 80 characters
      GetCursorPos(Pt);
      Pt := ScreenToClient(Pt);
      Node := GetNodeAt(Pt.X, Pt.Y);
      if (Node = nil) or (Node.Text = '') or
        (PToolTipTextW(NMHdr)^.uFlags and TTF_IDISHWND = 0) then Exit;
      if (GetComCtlVersion >= ComCtlVersionIE4)
      or {Borland's VCL wrongly uses "and"} (Length(Node.Text) < 80) then
      begin
        DefaultHandler(Message);
        Exit;
      end;
      FWideText := Node.Text;
      MaxTextLen := SizeOf(PToolTipTextW(NMHdr)^.szText) div SizeOf(WideChar);
      if Length(FWideText) >= MaxTextLen then
        SetLength(FWideText, MaxTextLen - 1);
      PToolTipTextW(NMHdr)^.lpszText := PWideChar(FWideText);
      FillChar(PToolTipTextW(NMHdr)^.szText, MaxTextLen, 0);
      Move(Pointer(FWideText)^, PToolTipTextW(NMHdr)^.szText, Length(FWideText) * SizeOf(WideChar));
      PToolTipTextW(NMHdr)^.hInst := 0;
      SetWindowPos(NMHdr^.hwndFrom, HWND_TOP, 0, 0, 0, 0, SWP_NOACTIVATE or
        SWP_NOSIZE or SWP_NOMOVE or SWP_NOOWNERZORDER);
      Result := 1;
    end
    else inherited;
end;

procedure TTntCustomTreeView.Edit(const Item: TTVItem);
var
  S: WideString;
  AnsiS: AnsiString;
  Node: TTntTreeNode;
  AnsiEvent: TTVEditedEvent;
begin
  with Item do
  begin
    Node := GetNodeFromItem(Item);
    if pszText <> nil then
    begin
      if Win32PlatformIsUnicode then
        S := TTVItemW(Item).pszText
      else
        S := pszText;

      if Assigned(FOnEdited) then
        FOnEdited(Self, Node, S)
      else if Assigned(inherited OnEdited) then
      begin
        AnsiEvent := inherited OnEdited;
        AnsiS := S;
        AnsiEvent(Self, Node, AnsiS);
        S := AnsiS;
      end;

      if Node <> nil then Node.Text := S;
    end
    {$IFDEF COMPILER_6_UP}
    else if Assigned(OnCancelEdit) then OnCancelEdit(Self, Node);
    {$ENDIF}
  end;
end;

procedure TTntCustomTreeView.EditWndProcW(var Message: TMessage);
begin
  Assert(Win32PlatformIsUnicode);
  try
    with Message do
    begin
      case Msg of
        WM_KEYDOWN,
        WM_SYSKEYDOWN: if DoKeyDown(TWMKey(Message)) then Exit;
        WM_CHAR:
          begin
            MakeWMCharMsgSafeForAnsi(Message);
            try
              if DoKeyPress(TWMKey(Message)) then Exit;
            finally
              RestoreWMCharMsg(Message);
            end;
          end;
        WM_KEYUP,
        WM_SYSKEYUP: if DoKeyUp(TWMKey(Message)) then Exit;
        CN_KEYDOWN,
        CN_CHAR, CN_SYSKEYDOWN,
        CN_SYSCHAR:
          begin
            WndProc(Message);
            Exit;
          end;
      end;
      Result := CallWindowProcW(FDefEditProc, FEditHandle, Msg, WParam, LParam);
    end;
  except
    Application.HandleException(Self);
  end;
end;

procedure TTntCustomTreeView.LoadFromFile(const FileName: WideString);
var
  TreeStrings: TTntTreeStrings;
begin
  TreeStrings := TTntTreeStrings.Create;
  try
    TreeStrings.LoadFromFile(FileName);
    TreeStrings.SaveToTree(Self);
  finally
    TreeStrings.Free;
  end;
end;

procedure TTntCustomTreeView.LoadFromStream(Stream: TStream);
var
  TreeStrings: TTntTreeStrings;
begin
  TreeStrings := TTntTreeStrings.Create;
  try
    TreeStrings.LoadFromStream(Stream);
    TreeStrings.SaveToTree(Self);
  finally
    TreeStrings.Free;
  end;
end;

procedure TTntCustomTreeView.SaveToFile(const FileName: WideString);
var
  TreeStrings: TTntTreeStrings;
begin
  TreeStrings := TTntTreeStrings.Create;
  try
    TreeStrings.LoadFromTree(Self);
    TreeStrings.SaveToFile(FileName);
  finally
    TreeStrings.Free;
  end;
end;

procedure TTntCustomTreeView.SaveToStream(Stream: TStream);
var
  TreeStrings: TTntTreeStrings;
begin
  TreeStrings := TTntTreeStrings.Create;
  try
    TreeStrings.LoadFromTree(Self);
    TreeStrings.SaveToStream(Stream);
  finally
    TreeStrings.Free;
  end;
end;

initialization

finalization
  if Assigned(AIMM) then
    AIMM.Deactivate;
  if FRichEdit20Module <> 0 then
    FreeLibrary(FRichEdit20Module);

end.
