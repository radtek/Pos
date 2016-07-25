{+--------------------------------------------------------------------------+
 | Component:   TmwComboTree
 | Created:     05.98
 | Author:      Martin Waldenburg
 | Copyright    1998, all rights reserved.
 | Description: Study on how to create a ComboBox like Component
 | Status:      FreeWare
 | Version:     1.2
 | DISCLAIMER:  This is provided as is, expressly without a warranty of any kind.
 |              You use it at your own risc.
 +--------------------------------------------------------------------------+}
unit mwCombTree;
                  
interface

uses
  Windows, SysUtils, Messages, Classes, Graphics, Controls, Forms, Dialogs,
  Menus, StdCtrls, ExtCtrls, Buttons, Commctrl, Comctrls, ImgList;

type
  TmwComboTree = class;

  TmwScreenTreeView = class(TTreeView)
  private
    FMouseInControl: Boolean;
	 procedure CMMouseEnter(var Message: TMessage); message CM_MOUSEENTER;
    procedure CMMouseLeave(var Message: TMessage); message CM_MOUSELEAVE;
	 procedure WMMouseMove(var Message: TWMMouseMove); message WM_MOUSEMOVE;
	 procedure WMLButtonDown(var Message: TWMLButtonDown); message WM_LBUTTONDOWN;
  protected
    procedure CreateParams(var Params: TCreateParams); override;
    procedure CreateWnd; override;
  end;

  TmwComboTree = class(TCustomEdit)
    Box: TSpeedButton;
    Tree: TTreeView;
    procedure BoxClick(Sender: TObject);
  private
    fImageIndex: Integer;
    fStateIndex: Integer;
	 FCanvas: TControlCanvas;
    procedure WMPaint(var Message: TWMPaint); message WM_PAINT;
{	 procedure WndProc(var Message: TMessage); override;}
	 procedure SetImageIndex(NewValue: Integer);
	 procedure SetStateIndex(NewValue: Integer);
    function ValidIndex(Index: Integer): Boolean;
    function ValidStateIndex(Index: Integer): Boolean;
    function GetImageIndex: Integer;
    function GetStateIndex: Integer;
    function GetBoxWidth: Integer;
    procedure SetBoxWidth(NewValue: Integer);
    function GetBoxGlyph: TBitmap;
    procedure SetBoxGlyph(NewValue: TBitmap);
    function GetImages: TCustomImageList; //TImageList;
    procedure SetImages(NewValue: TCustomImageList);
    function GetItems: TTreeNodes;
    procedure SetItems(NewValue: TTreeNodes);
    procedure SetStateImages(newValue: TCustomImageList);
    function GetStateImages: TCustomImageList;
    procedure SetTreeCursor(newValue: TCursor);
    function GetTreeCursor: TCursor;
    procedure SetTreeEnabled(newValue: Boolean);
    function GetTreeEnabled: Boolean;
	 procedure SetTreeHeight(newValue: Integer);
    function GetTreeHeight: Integer;
    procedure SetIndent(newValue: Integer);
    function GetIndent: Integer;
    procedure SetTreeReadOnly(newValue: Boolean);
    function GetTreeReadOnly: Boolean;
    procedure SetShowButtons(newValue: Boolean);
    function GetShowButtons: Boolean;
    procedure SetTreeShowHint(newValue: Boolean);
    function GetTreeShowHint: Boolean;
    procedure SetShowLines(newValue: Boolean);
    function GetShowLines: Boolean;
    procedure SetShowRoot(newValue: Boolean);
    function GetShowRoot: Boolean;
    procedure SetSortType(newValue: TSortType);
    function GetSortType: TSortType;
    procedure SetTreeVisible(newValue: Boolean);
    function GetTreeVisible: Boolean;
    function GetOnTreeChange:TTVChangedEvent;
    procedure SetOnTreeChange(NewValue:TTVChangedEvent);
    function GetChanging:TTVChangingEvent;
    procedure SetChanging(NewValue:TTVChangingEvent);
    function GetOnTreeClick:TNotifyEvent;
	 procedure SetOnTreeClick(NewValue:TNotifyEvent);
    function GetOnCollapsed:TTVExpandedEvent;
    procedure SetOnCollapsed(NewValue:TTVExpandedEvent);
    function GetOnCollapsing:TTVCollapsingEvent;
    procedure SetOnCollapsing(NewValue:TTVCollapsingEvent);
    function GetOnCompare:TTVCompareEvent;
    procedure SetOnCompare(NewValue:TTVCompareEvent);
    function GetOnTreeDblClick:TNotifyEvent;
    procedure SetOnTreeDblClick(NewValue:TNotifyEvent);
    function GetOnDeletion:TTVExpandedEvent;
    procedure SetOnDeletion(NewValue:TTVExpandedEvent);
    function GetOnEdited:TTVEditedEvent;
    procedure SetOnEdited(NewValue:TTVEditedEvent);
    function GetOnEditing:TTVEditingEvent;
    procedure SetOnEditing(NewValue:TTVEditingEvent);
    function GetOnTreeEnter:TNotifyEvent;
    procedure SetOnTreeEnter(NewValue:TNotifyEvent);
    function GetOnTreeExit:TNotifyEvent;
    procedure SetOnTreeExit(NewValue:TNotifyEvent);
    function GetOnExpanded:TTVExpandedEvent;
    procedure SetOnExpanded(NewValue:TTVExpandedEvent);
    function GetOnExpanding:TTVExpandingEvent;
    procedure SetOnExpanding(NewValue:TTVExpandingEvent);
	 function GetOnGetImageIndex:TTVExpandedEvent;
    procedure SetOnGetImageIndex(NewValue:TTVExpandedEvent);
    function GetOnGetSelectedIndex:TTVExpandedEvent;
    procedure SetOnGetSelectedIndex(NewValue:TTVExpandedEvent);
  protected
	 procedure WndProc(var Message: TMessage); override;
    procedure CreateWindowHandle(const Params: TCreateParams); override;
    procedure SetEditRect;
{	 procedure SetBounds(Left, Top, Width, Height: Integer); override;}
    procedure CreateParams(var Params: TCreateParams); override;
    procedure CreateWnd; override;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure DrawImage(Index, Position: Integer);
    procedure DrawStateImage(Index, Position: Integer);
	 procedure SetBounds(Left, Top, Width, Height: Integer); override;
  published
    property Font;
    property Text;
    property Color;
	 property Images: TCustomImageList read GetImages write SetImages;
    property ImageIndex: Integer read GetImageIndex write SetImageIndex;
    property StateIndex: Integer read GetStateIndex write SetStateIndex;
    property BoxWidth: Integer read GetBoxWidth write SetBoxWidth;
	 property BoxGlyph: TBitmap read GetBoxGlyph write SetBoxGlyph;
    property Items: TTreeNodes read GetItems write SetItems;
    property StateImages: TCustomImageList read GetStateImages write SetStateImages;
    property Enabled;
    property ReadOnly;
    property ShowHint;
    property Visible;
    property TreeCursor: TCursor read GetTreeCursor write SetTreeCursor;
    property TreeEnabled: Boolean read GetTreeEnabled write SetTreeEnabled;
    property TreeHeight: Integer read GetTreeHeight write SetTreeHeight;
    property Indent: Integer read GetIndent write SetIndent;
    property TreeReadOnly: Boolean read GetTreeReadOnly write SetTreeReadOnly;
    property ShowButtons: Boolean read GetShowButtons write SetShowButtons;
    property TreeShowHint: Boolean read GetTreeShowHint write SetTreeShowHint;
    property ShowLines: Boolean read GetShowLines write SetShowLines;
    property ShowRoot: Boolean read GetShowRoot write SetShowRoot;
    property SortType: TSortType read GetSortType write SetSortType;
    property TreeVisible: Boolean read GetTreeVisible write SetTreeVisible;
    property OnChange;
    property OnClick;
    property OnDblClick;
    property OnEnter;
    property OnExit;
	 property OnTreeChange:TTVChangedEvent read GetOnTreeChange write SetOnTreeChange;
    property Changing:TTVChangingEvent read GetChanging write SetChanging;
    property OnTreeClick:TNotifyEvent read GetOnTreeClick write SetOnTreeClick;
    property OnCollapsed:TTVExpandedEvent read GetOnCollapsed write SetOnCollapsed;
    property OnCollapsing:TTVCollapsingEvent read GetOnCollapsing write SetOnCollapsing;
    property OnCompare:TTVCompareEvent read GetOnCompare write SetOnCompare;
    property OnTreeDblClick:TNotifyEvent read GetOnTreeDblClick write SetOnTreeDblClick;
    property OnDeletion:TTVExpandedEvent read GetOnDeletion write SetOnDeletion;
    property OnEdited:TTVEditedEvent read GetOnEdited write SetOnEdited;
    property OnEditing:TTVEditingEvent read GetOnEditing write SetOnEditing;
    property OnTreeEnter:TNotifyEvent read GetOnTreeEnter write SetOnTreeEnter;
    property OnTreeExit:TNotifyEvent read GetOnTreeExit write SetOnTreeExit;
    property OnExpanded:TTVExpandedEvent read GetOnExpanded write SetOnExpanded;
    property OnExpanding:TTVExpandingEvent read GetOnExpanding write SetOnExpanding;
    property OnGetImageIndex:TTVExpandedEvent read GetOnGetImageIndex write SetOnGetImageIndex;
    property OnGetSelectedIndex:TTVExpandedEvent read GetOnGetSelectedIndex write SetOnGetSelectedIndex;
  end;

procedure Register;

implementation

procedure Register;
begin
  RegisterComponents('Additional', [TmwComboTree]);
end; { Register }

procedure TmwScreenTreeView.CreateParams(var Params: TCreateParams);
begin
  inherited CreateParams(Params);
  with Params do
  begin Style := Style or WS_BORDER; ExStyle := WS_EX_TOOLWINDOW or
    WS_EX_TOPMOST; WindowClass.Style := CS_SAVEBITS;
  end;
end; { CreateParams }

procedure TmwScreenTreeView.CreateWnd;
begin
  inherited CreateWnd;
  Windows.SetParent(Handle, 0); CallWindowProc(DefWndProc, Handle, wm_SetFocus, 0, 0);
end; { CreateWnd }

procedure TmwScreenTreeView.CMMouseEnter(var Message: TMessage);
begin
  inherited;
  FMouseInControl := True;
  ReleaseCapture;
end; { CMMouseEnter }

procedure TmwScreenTreeView.CMMouseLeave(var Message: TMessage);
begin
  inherited;
  FMouseInControl := False;
  SetCaptureControl(Self);
end; { CMMouseLeave }

procedure TmwScreenTreeView.WMMouseMove(var Message: TWMMouseMove);
var
  TreeHitTest: THitTests;
begin
  inherited;
  if FMouseInControl and Enabled then
  begin
    TreeHitTest := GetHitTestInfoAt(Message.XPos, Message.YPos);
    if (htOnItem in TreeHitTest) or (htOnRight in TreeHitTest) then
      Selected := GetNodeAt(Message.XPos, Message.YPos);
  end;
end; { WMMouseMove }

procedure TmwScreenTreeView.WMLButtonDown(var Message: TWMLButtonDown);
var
  HitTest: THitTests;
begin
  inherited;
  Case FMouseInControl of
	 False:
		begin
		  ReleaseCapture;
		  Enabled := False;
		  Visible := False;
		end;
	 True:
		begin
		  SendMessage((Owner.Owner as TWinControl).Handle, WM_LBUTTONDOWN, 0, 0);
		  HitTest := GetHitTestInfoAt(Message.XPos, Message.YPos);
		  if (htOnItem in HitTest) or (htOnRight in HitTest) then
		  begin
			 Selected.EndEdit(True);
			 (Owner as TmwComboTree).Text := Selected.Text;
			 (Owner as TmwComboTree).ImageIndex := Selected.ImageIndex;
			 (Owner as TmwComboTree).StateIndex := Selected.StateIndex;
			 Enabled := False;
			 Visible := False;
		  end;
		end;
  end;
end; { WMLButtonDown }

destructor TmwComboTree.Destroy;
begin
  inherited Destroy;
end; { Destroy }

constructor TmwComboTree.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  Width := 141;
  Height := 21;
  TabOrder := 0;

  FCanvas := TControlCanvas.Create;
  FCanvas.Control := Self;

  Box := TSpeedButton.Create(Self);
  Box.Parent := Self;

  Tree := TmwScreenTreeView.Create(Self);
  Tree.Parent := Self;
  Tree.Height := 97;

  ImageIndex := -1;
  StateIndex := -1;
end; { Create }

procedure TmwComboTree.CreateParams(var Params: TCreateParams);
begin
  inherited CreateParams(Params);
  Params.Style := Params.Style or ES_MULTILINE or WS_CLIPCHILDREN;
end;

procedure TmwComboTree.CreateWnd;
begin
  inherited CreateWnd;
  SetEditRect;
end; { CreateWnd }

procedure TmwComboTree.CreateWindowHandle(const Params: TCreateParams);
begin
  inherited CreateWindowHandle(Params);

  with Box do
  begin
    Left := 120;
    Top := 0;
    Width := 17;
    Height := 17;
    Cursor:= crArrow;
    OnClick := BoxClick;
  end; { Box }

  with Tree do
  begin
    Left := 0;
    Top := 0;
    Width := 0;
    Enabled := False;
    Visible := False;
  end; { Tree }
end; { CreateWindowHandle }

procedure TmwComboTree.SetEditRect;
var
  Loc: TRect;
begin
  if ValidStateIndex(StateIndex) and ValidIndex(ImageIndex) then
  begin
    SetRect(Loc, 45, 0, ClientWidth - Box.Width - 2, ClientHeight + 1);
    SendMessage(Handle, EM_SETRECTNP, 0, LongInt(@Loc));
  end else
    if ValidStateIndex(StateIndex) and not ValidIndex(ImageIndex) then
    begin
      SetRect(Loc, 23, 0, ClientWidth - Box.Width - 2, ClientHeight + 1);
      SendMessage(Handle, EM_SETRECTNP, 0, LongInt(@Loc));
    end else
      if not ValidStateIndex(StateIndex) and ValidIndex(ImageIndex) then
      begin
        SetRect(Loc, 23, 0, ClientWidth - Box.Width - 2, ClientHeight + 1);
        SendMessage(Handle, EM_SETRECTNP, 0, LongInt(@Loc));
      end else
        if not ValidStateIndex(StateIndex) and not ValidIndex(ImageIndex) then
        begin
          SetRect(Loc, 0, 0, ClientWidth - Box.Width - 2, ClientHeight + 1);
          SendMessage(Handle, EM_SETRECTNP, 0, LongInt(@Loc));
        end;
end;

procedure TmwComboTree.BoxClick(Sender: TObject);
var
  CP, SP: TPoint;
begin
  CP.X := Left;
  CP.Y := Top + Height;
  SP := Parent.ClientToScreen(CP);{(Owner as TForm).ClientToScreen(CP);}

  with Tree do
  begin
	 Color := (Owner as TmwComboTree).Color;
	 Left := SP.X;
    Top := SP.Y;
    Width := Self.Width;
    Enabled := True;
    Visible := True;
  end; { Tree }
  Parent.SetFocus;
  SetCaptureControl(Tree);
end; { BoxClick }

procedure TmwComboTree.SetBounds(Left, Top, Width, Height: Integer);
begin
  Case Parent <> nil of
    True:
      begin
        inherited SetBounds(Left, Top, Width, Height);
        with Box do
        begin
          Left := Self.Width - Box.Width - 4;
          Height := Self.Height - 4;
        end;
        SetEditRect;
      end;
    False: inherited SetBounds(Left, Top, Width, Height);
  end;
end; { SetBounds }

procedure TmwComboTree.WMPaint(var Message: TWMPaint);
begin
  inherited PaintHandler(Message);
  if ValidStateIndex(StateIndex) and ValidIndex(ImageIndex) then
  begin
    DrawStateImage(StateIndex, 5);
    DrawImage(ImageIndex, 27);
  end else
    if ValidStateIndex(StateIndex) and not ValidIndex(ImageIndex) then
    begin
      DrawStateImage(StateIndex, 5);
    end else
      if not ValidStateIndex(StateIndex) and ValidIndex(ImageIndex) then
      begin
        DrawImage(ImageIndex, 5);
      end;
end; { WMPaint }

procedure TmwComboTree.WndProc(var Message: TMessage);
begin
  with Message do
    case Msg of
      WM_WINDOWPOSCHANGING: SetEditRect;
    end;
  inherited WndProc(Message);
end; { WndProc }

procedure TmwComboTree.DrawImage(Index, Position: Integer);
var
  YPos: Integer;
begin
  if assigned(Images) then
  begin
    YPos := ClientHeight div 2 - 8;
    Images.Draw(FCanvas, Position, YPos, Index);
  end;
end; { DrawImage }

procedure TmwComboTree.DrawStateImage(Index, Position: Integer);
var
  YPos: Integer;
begin
  if assigned(StateImages) then
  begin
    YPos := ClientHeight div 2 - 8;
    StateImages.Draw(FCanvas, Position, YPos, Index);
  end;
end; { DrawImage }

function TmwComboTree.ValidIndex(Index: Integer): Boolean;
begin
  if assigned(Images) then
  begin
    if (Index >= 0) and (Index < Images.Count) then
      ValidIndex := True else ValidIndex := False;
  end else ValidIndex := False;
end; { ValidIndex }

function TmwComboTree.ValidStateIndex(Index: Integer): Boolean;
begin
  if assigned(StateImages) then
  begin
    if (Index >= 0) and (Index < StateImages.Count) then
      ValidStateIndex := True else ValidStateIndex := False;
  end else ValidStateIndex := False;
end; { ValidIndex }

function TmwComboTree.GetImageIndex: Integer;
begin
  Result := FImageIndex;
end; { GetImageIndex }

procedure TmwComboTree.SetImageIndex(NewValue: Integer);
begin
  FImageIndex := NewValue;
  if not assigned(Images) then exit;
  SetEditRect;
  RePaint;
end; { SetImageIndex }

function TmwComboTree.GetStateIndex: Integer;
begin
  Result := FStateIndex;
end; { GetStateIndex }

procedure TmwComboTree.SetStateIndex(NewValue: Integer);
begin
  FStateIndex := NewValue;
  if not assigned(StateImages) then exit;
  SetEditRect;
  RePaint;
end; { SetStateIndex }

function TmwComboTree.GetBoxWidth: Integer;
begin
  GetBoxWidth := Box.Width;
end; { GetBoxWidth }

procedure TmwComboTree.SetBoxWidth(NewValue: Integer);
begin
  Box.Width := NewValue;
  Box.Left := ClientWidth - Box.Width;
  SetEditRect;
end; { SetBoxWidth }

function TmwComboTree.GetBoxGlyph: TBitmap;
begin
  GetBoxGlyph := Box.Glyph;
end; { GetBoxGlyph }

procedure TmwComboTree.SetBoxGlyph(NewValue: TBitmap);
begin
  Box.Glyph := NewValue;
end; { SetBoxGlyph }

function TmwComboTree.GetImages: TCustomImageList;
begin
  GetImages := Tree.Images;
end; { GetImages }

procedure TmwComboTree.SetImages(NewValue: TCustomImageList);
begin
  Tree.Images := NewValue;
  SetEditRect;
end; { SetImages }

function TmwComboTree.GetItems: TTreeNodes;
begin
  GetItems := Tree.Items;
end; { GetItems }

procedure TmwComboTree.SetItems(NewValue: TTreeNodes);
begin
  Tree.Items := NewValue;
end; { SetItems }

procedure TmwComboTree.SetStateImages(newValue: TCustomImageList);
begin
  Tree.StateImages := newValue;
  SetEditRect;
  RePaint;
end;  { SetStateImages }

function TmwComboTree.GetStateImages: TCustomImageList;
begin
  GetStateImages := Tree.StateImages;
end;  { GetStateImages }

procedure TmwComboTree.SetTreeCursor(newValue: TCursor);
begin
  Tree.Cursor := newValue;
end;  { SetTreeCursor }

function TmwComboTree.GetTreeCursor: TCursor;
begin
  GetTreeCursor := Tree.Cursor;
end;  { GetTreeCursor }

procedure TmwComboTree.SetTreeEnabled(newValue: Boolean);
begin
  Tree.Enabled := newValue;
end;  { SetTreeEnabled }

function TmwComboTree.GetTreeEnabled: Boolean;
begin
  GetTreeEnabled := Tree.Enabled;
end;  { GetTreeEnabled }

procedure TmwComboTree.SetTreeHeight(newValue: Integer);
begin
  Tree.Height := newValue;
end;  { SetTreeHeight }

function TmwComboTree.GetTreeHeight: Integer;
begin
  GetTreeHeight := Tree.Height;
end;  { GetTreeHeight }

procedure TmwComboTree.SetIndent(newValue: Integer);
begin
  Tree.Indent := newValue;
end;  { SetIndent }

function TmwComboTree.GetIndent: Integer;
begin
  GetIndent := Tree.Indent;
end;  { GetIndent }

procedure TmwComboTree.SetTreeReadOnly(newValue: Boolean);
begin
  Tree.ReadOnly := newValue;
end;  { SetTreeReadOnly }

function TmwComboTree.GetTreeReadOnly: Boolean;
begin
  GetTreeReadOnly := Tree.ReadOnly;
end;  { GetTreeReadOnly }

procedure TmwComboTree.SetShowButtons(newValue: Boolean);
begin
  Tree.ShowButtons := newValue;
end;  { SetShowButtons }

function TmwComboTree.GetShowButtons: Boolean;
begin
  GetShowButtons := Tree.ShowButtons;
end;  { GetShowButtons }

procedure TmwComboTree.SetTreeShowHint(newValue: Boolean);
begin
  Tree.ShowHint := newValue;
end;  { SetTreeShowHint }

function TmwComboTree.GetTreeShowHint: Boolean;
begin
  GetTreeShowHint := Tree.ShowHint;
end;  { GetTreeShowHint }

procedure TmwComboTree.SetShowLines(newValue: Boolean);
begin
  Tree.ShowLines := newValue;
end;  { SetShowLines }

function TmwComboTree.GetShowLines: Boolean;
begin
  GetShowLines := Tree.ShowLines;
end;  { GetShowLines }

procedure TmwComboTree.SetShowRoot(newValue: Boolean);
begin
  Tree.ShowRoot := newValue;
end;  { SetShowRoot }

function TmwComboTree.GetShowRoot: Boolean;
begin
  GetShowRoot := Tree.ShowRoot;
end;  { GetShowRoot }

procedure TmwComboTree.SetSortType(newValue: TSortType);
begin
  Tree.SortType := newValue;
end;  { SetSortType }

function TmwComboTree.GetSortType: TSortType;
begin
  GetSortType := Tree.SortType;
end;  { GetSortType }

procedure TmwComboTree.SetTreeVisible(newValue: Boolean);
begin
  Tree.Visible := newValue;
end;  { SetTreeVisible }

function TmwComboTree.GetTreeVisible: Boolean;
begin
  GetTreeVisible := Tree.Visible;
end;  { GetTreeVisible }

function TmwComboTree.GetOnTreeChange:TTVChangedEvent;
begin
  GetOnTreeChange:= Tree.OnChange;
end;   { GetOnTreeChange }

procedure TmwComboTree.SetOnTreeChange(NewValue:TTVChangedEvent);
begin
  Tree.OnChange:= NewValue;
end;   { SetOnTreeChange }

function TmwComboTree.GetChanging:TTVChangingEvent;
begin
  GetChanging:= Tree.OnChanging;
end;   { GetChanging }

procedure TmwComboTree.SetChanging(NewValue:TTVChangingEvent);
begin
  Tree.OnChanging:= NewValue;
end;   { SetChanging }

function TmwComboTree.GetOnTreeClick:TNotifyEvent;
begin
  GetOnTreeClick:= Tree.OnClick;
end;   { GetOnTreeClick }

procedure TmwComboTree.SetOnTreeClick(NewValue:TNotifyEvent);
begin
  Tree.OnClick:= NewValue;
end;   { SetOnTreeClick }

function TmwComboTree.GetOnCollapsed:TTVExpandedEvent;
begin
  GetOnCollapsed:= Tree.OnCollapsed;
end;   { GetOnCollapsed }

procedure TmwComboTree.SetOnCollapsed(NewValue:TTVExpandedEvent);
begin
  Tree.OnCollapsed:= NewValue;
end;   { SetOnCollapsed }

function TmwComboTree.GetOnCollapsing:TTVCollapsingEvent;
begin
  GetOnCollapsing:= Tree.OnCollapsing;
end;   { GetOnCollapsing }

procedure TmwComboTree.SetOnCollapsing(NewValue:TTVCollapsingEvent);
begin
  Tree.OnCollapsing:= NewValue;
end;   { SetOnCollapsing }

function TmwComboTree.GetOnCompare:TTVCompareEvent;
begin
  GetOnCompare:= Tree.OnCompare;
end;   { GetOnCompare }

procedure TmwComboTree.SetOnCompare(NewValue:TTVCompareEvent);
begin
  Tree.OnCompare:= NewValue;
end;   { SetOnCompare }

function TmwComboTree.GetOnTreeDblClick:TNotifyEvent;
begin
  GetOnTreeDblClick:= Tree.OnDblClick;
end;   { GetOnTreeDblClick }

procedure TmwComboTree.SetOnTreeDblClick(NewValue:TNotifyEvent);
begin
  Tree.OnDblClick:= NewValue;
end;   { SetOnTreeDblClick }

function TmwComboTree.GetOnDeletion:TTVExpandedEvent;
begin
  GetOnDeletion:= Tree.OnDeletion;
end;   { GetOnDeletion }

procedure TmwComboTree.SetOnDeletion(NewValue:TTVExpandedEvent);
begin
  Tree.OnDeletion:= NewValue;
end;   { SetOnDeletion }

function TmwComboTree.GetOnEdited:TTVEditedEvent;
begin
  GetOnEdited:= Tree.OnEdited;
end;   { GetOnEdited }

procedure TmwComboTree.SetOnEdited(NewValue:TTVEditedEvent);
begin
  Tree.OnEdited:= NewValue;
end;   { SetOnEdited }

function TmwComboTree.GetOnEditing:TTVEditingEvent;
begin
  GetOnEditing:= Tree.OnEditing;
end;   { GetOnEditing }

procedure TmwComboTree.SetOnEditing(NewValue:TTVEditingEvent);
begin
  Tree.OnEditing:= NewValue;
end;   { SetOnEditing }

function TmwComboTree.GetOnTreeEnter:TNotifyEvent;
begin
  GetOnTreeEnter:= Tree.OnEnter;
end;   { GetOnTreeEnter }

procedure TmwComboTree.SetOnTreeEnter(NewValue:TNotifyEvent);
begin
  Tree.OnEnter:= NewValue;
end;   { SetOnTreeEnter }

function TmwComboTree.GetOnTreeExit:TNotifyEvent;
begin
  GetOnTreeExit:= Tree.OnExit;
end;   { GetOnTreeExit }

procedure TmwComboTree.SetOnTreeExit(NewValue:TNotifyEvent);
begin
  Tree.OnExit:= NewValue;
end;   { SetOnTreeExit }

function TmwComboTree.GetOnExpanded:TTVExpandedEvent;
begin
  GetOnExpanded:= Tree.OnExpanded;
end;   { GetOnExpanded }

procedure TmwComboTree.SetOnExpanded(NewValue:TTVExpandedEvent);
begin
  Tree.OnExpanded:= NewValue;
end;   { SetOnExpanded }

function TmwComboTree.GetOnExpanding:TTVExpandingEvent;
begin
  GetOnExpanding:= Tree.OnExpanding;
end;   { GetOnExpanding }

procedure TmwComboTree.SetOnExpanding(NewValue:TTVExpandingEvent);
begin
  Tree.OnExpanding:= NewValue;
end;   { SetOnExpanding }

function TmwComboTree.GetOnGetImageIndex:TTVExpandedEvent;
begin
  GetOnGetImageIndex:= Tree.OnGetImageIndex;
end;   { GetOnGetImageIndex }

procedure TmwComboTree.SetOnGetImageIndex(NewValue:TTVExpandedEvent);
begin
  Tree.OnGetImageIndex:= NewValue;
end;   { SetOnGetImageIndex }

function TmwComboTree.GetOnGetSelectedIndex:TTVExpandedEvent;
begin
  GetOnGetSelectedIndex:= Tree.OnGetSelectedIndex;
end;   { GetOnGetSelectedIndex }

procedure TmwComboTree.SetOnGetSelectedIndex(NewValue:TTVExpandedEvent);
begin
  Tree.OnGetSelectedIndex:= NewValue;
end;   { SetOnGetSelectedIndex }

end.

