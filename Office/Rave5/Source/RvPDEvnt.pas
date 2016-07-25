{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RvPDEvnt;

interface

uses
  Windows, Messages,
  SysUtils, Classes, Graphics, Controls, Forms, Dialogs, ExtCtrls, StdCtrls, ComCtrls, Buttons, TypInfo,
  RvClass, RpDefine, RvProj, RvTool, RvData, RvLEModule, RvLEUtil, RvLEID, RvLEDefine, RvToolUtil,
  RvToolEvents;

type
  TRaveMemo = class(TCustomMemo)
  private
    FPrevMemo: TRaveMemo;
    FNextMemo: TRaveMemo;
    FEvent: TRaveEvent;
  protected
    FEndingCRLF: boolean;
    //
    procedure Change; override;
    procedure KeyDown(var Key: Word; Shift: TShiftState); override;
    function GetLineCount: integer;
    procedure SetPrevMemo(Value: TRaveMemo);
    procedure SetNextMemo(Value: TRaveMemo);
  public
    constructor Create(AOwner: TComponent); override;
    procedure MarkReadOnly;
    //
    property PrevMemo: TRaveMemo read FPrevMemo write SetPrevMemo;
    property NextMemo: TRaveMemo read FNextMemo write SetNextMemo;
    property LineCount: integer read GetLineCount;
    property Event: TRaveEvent read FEvent write FEvent;
  end; { TRaveMemo }

  TSourceEditorForm = class(TForm)
    SourceEditorPanel: TPanel;
    Panel3: TPanel;
    Splitter2: TSplitter;
    Panel1: TPanel;
    Splitter1: TSplitter;
    DefinedLB: TListBox;
    Panel2: TPanel;
    Panel4: TPanel;
    AvailableCB: TComboBox;
    btnShowSelectedEvents: TSpeedButton;
    btnShowPageEvents: TSpeedButton;
    butnCompile: TButton;
    Label1: TLabel;
    Label2: TLabel;
    EventDesc: TRichEdit;
    sboxEditor: TScrollBox;
    btnDeleteEvent: TSpeedButton;
    StatusLabel: TLabel;
    procedure DefinedLBClick(Sender: TObject);
    procedure AvailableCBClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure btnShowSelectedEventsClick(Sender: TObject);
    procedure btnShowPageEventsClick(Sender: TObject);
    procedure butnCompileClick(Sender: TObject);
    procedure BodyMemoChange(Sender: TObject);
    procedure EventDescEnter(Sender: TObject);
    procedure HeaderMemoChange(Sender: TObject);
    procedure btnDeleteEventClick(Sender: TObject);
  private
    Instance: TRaveComponent;
    CurrPage: TRavePage;
    Event: TRaveEvent;
    DeclareMemo: TRaveMemo;
    HeaderMemo: TRaveMemo;
    BeginMemo: TRaveMemo;
    BodyMemo: TRaveMemo;
    EndMemo: TRaveMemo;

    procedure SaveCurrentEvent;
    procedure SelectEvent(EventName: string);
    procedure SetStatusLabelAttributes(ADoError: boolean);
  public
    procedure FillEventDesc;
    procedure Setup;
    procedure Finish;
  end;

  TRaveEventDesigner = class(TRaveProjectDesigner)
  protected
    function GetTitle: string; override;
  public
    procedure SelectionChanged; override;
    procedure Modified; override;
    procedure Enter; override;
    procedure Exit; override;
    procedure Connect(Parent: TWinControl); override;
    procedure Disconnect; override;
  end; { TRaveEventDesigner }

  TRaveEventProjectEditor = class(TRaveProjectEditor, IRaveModuleEventHandler)
  public
    procedure CompileStatus(Module: TRaveModule);
    procedure CompileError(CompileStatus: TRaveCompileStatus);
    procedure CompileSuccess;
  end; { TRaveEventProjectEditor }

var
  SourceEditorForm: TSourceEditorForm;

  procedure RaveRegister;

implementation

{$R *.dfm}

uses
  Math;

procedure RaveRegister;
begin { RaveRegister }
  SourceEditorForm := TSourceEditorForm.Create(Application);
  RegisterRaveProjectDesigner(TRaveEventDesigner);
  RegisterRaveProjectEditor(TRaveEventProjectEditor);
end;  { RaveRegister }

{ TRaveEventDesigner }

function TRaveEventDesigner.GetTitle: string;
begin { GetTitle }
  Result := Trans('Event Editor');
end;  { GetTitle }

procedure TRaveEventDesigner.SelectionChanged;
begin { SelectionChanged }
  SourceEditorForm.Setup;
end;  { SelectionChanged }

procedure TRaveEventDesigner.Modified;
begin { Modified }
end;  { Modified }

procedure TRaveEventDesigner.Enter;
begin { Enter }
  SourceEditorForm.Setup;
end;  { Enter }

procedure TRaveEventDesigner.Exit;
begin { Exit }
  SourceEditorForm.Finish;
end;  { Exit }

procedure TRaveEventDesigner.Connect(Parent: TWinControl);
begin { Connect }
  SourceEditorForm.SourceEditorPanel.Parent := Parent;
end;  { Connect }

procedure TRaveEventDesigner.Disconnect;
begin { Disconnect }
  SourceEditorForm.SourceEditorPanel.Parent := SourceEditorForm;
end;  { Disconnect }

{ TRaveEventProjectEditor }

procedure TRaveEventProjectEditor.CompileError(CompileStatus: TRaveCompileStatus);

  function GetEventName(Event: TRaveEvent): string;
  var
    Count: integer;
    PropList: PPropList;
    PropInfo: PPropInfo;
    I1: integer;
  begin { GetEventName }
    Result := '';
    Count := GetTypeData(Event.Owner.ClassInfo)^.PropCount;
    if Count > 0 then begin
      GetMem(PropList,Count * SizeOf(pointer));
      try
        GetPropInfos(Event.Owner.ClassInfo,PropList);
        for I1 := 0 to (Count - 1) do begin
          PropInfo := PropList^[I1];
          if (PropInfo.PropType^.Kind = tkClass) and
           (TRaveEvent(GetOrdProp(Event.Owner,PropInfo)) = Event) then begin
            Result := PropInfo.Name;
            Exit;
          end; { if }
        end; { for }
      finally
        FreeMem(PropList,Count * SizeOf(pointer));
      end; { tryf }
    end; { if }
  end;  { GetEventName }

begin
  (SourceEditorForm.SourceEditorPanel.Parent.Parent as TPageControl).ActivePage :=
   (SourceEditorForm.SourceEditorPanel.Parent as TTabSheet);

  with SourceEditorForm do begin
    if Assigned(CompileStatus.ErrorEvent) then begin
      CurrentDesigner.ClearSelection;
      if Assigned(ErrorEvent.Owner.Report) then begin
        ProjectManager.ActivateReport(ErrorEvent.Owner.Report as TRaveReport);
      end; { if }
      CurrentDesigner.SelectControl(ErrorEvent.Owner);
      Setup;
      SelectEvent(GetEventName(ErrorEvent));
      if ErrorLine < ErrorEvent.BodySourceLine then begin { Error in header }
      // Move cursor to the proper line
        while (HeaderMemo.CaretPos.Y < ErrorLine - ErrorEvent.HeaderSourceLine) and
         (HeaderMemo.SelStart < Length(HeaderMemo.Text)) do begin // move to line
          HeaderMemo.SelStart := HeaderMemo.SelStart + 1;
        end; { while }

        ErrorCol := ErrorCol - 1;
        while (ErrorCol < Length(HeaderMemo.Text)) and (HeaderMemo.CaretPos.X < ErrorCol) and
         (HeaderMemo.SelStart < Length(HeaderMemo.Text)) do begin // move to column
          HeaderMemo.SelStart := HeaderMemo.SelStart + 1;
          if HeaderMemo.CaretPos.X = 0 then begin
            HeaderMemo.SelStart := HeaderMemo.SelStart - 1;
            Break;
          end;
        end; { while }
        if HeaderMemo.CaretPos.X = 0 then begin
          HeaderMemo.SelStart := HeaderMemo.SelStart - 1;
        end;
        HeaderMemo.SetFocus;
      end else begin { Error in body }
      // Move cursor to the proper line
        while (BodyMemo.CaretPos.Y < ErrorLine - ErrorEvent.BodySourceLine) and
         (BodyMemo.SelStart < Length(BodyMemo.Text)) do begin // move to line
          BodyMemo.SelStart := BodyMemo.SelStart + 1;
        end; { while }

        ErrorCol := ErrorCol - 1;
        while (ErrorCol < Length(BodyMemo.Text)) and (BodyMemo.CaretPos.X < ErrorCol) and
         (BodyMemo.SelStart < Length(BodyMemo.Text)) do begin // move to column
          BodyMemo.SelStart := BodyMemo.SelStart + 1;
          if BodyMemo.CaretPos.X = 0 then begin
            BodyMemo.SelStart := BodyMemo.SelStart - 1;
            Break;
          end;
        end; { while }
        if BodyMemo.CaretPos.X = 0 then begin
          BodyMemo.SelStart := BodyMemo.SelStart - 1;
        end;
        BodyMemo.SetFocus;
      end; { else }
    end; { if }
    SetStatusLabelAttributes(true);
    StatusLabel.Caption := '  ' + CompileStatus.ErrorMsg + '  ';
  end; { with }
end;

procedure TRaveEventProjectEditor.CompileStatus(Module: TRaveModule);
begin
//
end;

procedure TRaveEventProjectEditor.CompileSuccess;
begin
  SourceEditorForm.SetStatusLabelAttributes(false);
  SourceEditorForm.StatusLabel.Caption := Trans('Compile finished.');
end;

{ TSourceEditorForm }

procedure TSourceEditorForm.FillEventDesc;
var
  S1,S2: string;
  EventType: string;

  procedure SetCellAttribs(NewStyle: TFontStyles);
  begin
    with EventDesc,SelAttributes do begin
      Size := 10;
      Style := NewStyle;
      Color := clBlack;
    end; { with }
  end;

begin { FillEventDesc }
{ Initialize EventDesc }
  with EventDesc do begin
    Text := '';
  {TransOff}

  // Component/Event name line
    with SelAttributes do begin
      Name := {Trans-}'Arial';
      Size := 14;
      Style := [fsBold,fsUnderline];
      Color := clNavy;
    end; { with }
    if Assigned(Instance) then begin
      if DefinedLB.ItemIndex >= 0 then begin
        EventType := DefinedLB.Items[DefinedLB.ItemIndex];
        if Pos('.',EventType) > 0 then begin
          EventType := Copy(EventType,Pos('.',EventType) + 1, Length(EventType));
        end; { if }
        S1 := GetPropDescProc(Instance.ClassType,EventType);
        if Pos('|',S1) > 0 then begin
          S2 := Copy(S1, Pos('|',S1) + 1, Length(S1));
          S1 := Copy(S1, 0, Pos('|',S1) - 1);
        end else begin
          S2 := 'none';
        end; { else }
        if btnShowSelectedEvents.Down then begin
          Lines.Add(Instance.Name + ' ' + DefinedLB.Items[DefinedLB.ItemIndex] + ' event');
        end else begin
          Lines.Add(DefinedLB.Items[DefinedLB.ItemIndex] + ' event');
        end; { else }

      // Parameters and Description
        Lines.Add('');
        SetCellAttribs([]);
        Lines.Add('Parameters: ' + S2);
        Lines.Add('');
        SetCellAttribs([]);
        Lines.Add('Description: ' + S1);
        Lines.Add('');
        EventDesc.SelStart := FindText('Parameters:',0,Length(Lines.Text),[stWholeWord,stMatchCase]);
        EventDesc.SelLength := Length('Parameters:');
        SetCellAttribs([fsBold]);
        EventDesc.SelStart := FindText('Description:',0,Length(Lines.Text),[stWholeWord,stMatchCase]);
        EventDesc.SelLength := Length('Description:');
        SetCellAttribs([fsBold]);
      end else begin
        Lines.Add(Instance.Name + ' (No events defined)');
      end; { else }
    end else begin
      Text := Trans('(No component selected)');
    end; { else }
  {TransOn}
  end; { with }
end;  { FillEventDesc }

procedure TSourceEditorForm.Setup;
var
  Count: integer;
  PropList: PPropList;
  PropInfo: PPropInfo;
  I1: integer;
begin { Setup }
  SetStatusLabelAttributes(false);
  StatusLabel.Caption := '';
  if not Assigned(CurrentDesigner) then Exit;

{ Find selected component }
  if CurrentDesigner.Selections = 0 then begin
    Instance := nil;
  end else begin
    Instance := CurrentDesigner.Selection[0];
  end; { else }

{ Init AvailableCB and DefinedLB }
  AvailableCB.Clear;
  DefinedLB.Clear;
  EventDesc.Text := '';
  Event := nil;

{ Initialize Memo components }
  if Assigned(DeclareMemo) then begin
    HeaderMemo.Clear;//Text := #13#10;
    BodyMemo.Clear;//Text := #13#10;
  end else begin
    DeclareMemo := TRaveMemo.Create(self);
    DeclareMemo.Parent := sboxEditor;
    DeclareMemo.MarkReadOnly;

    HeaderMemo := TRaveMemo.Create(self);
    HeaderMemo.Parent := sboxEditor;
    HeaderMemo.PrevMemo := DeclareMemo;
    HeaderMemo.OnChange := HeaderMemoChange;

    BeginMemo := TRaveMemo.Create(self);
    BeginMemo.Parent := sboxEditor;
    BeginMemo.PrevMemo := HeaderMemo;
    BeginMemo.MarkReadOnly;

    BodyMemo := TRaveMemo.Create(self);
    BodyMemo.Parent := sboxEditor;
    BodyMemo.PrevMemo := BeginMemo;
    BodyMemo.OnChange := BodyMemoChange;

    EndMemo := TRaveMemo.Create(self);
    EndMemo.Parent := sboxEditor;
    EndMemo.PrevMemo := BodyMemo;
    EndMemo.MarkReadOnly;
//    EndMemo.Text := '';

    EndMemo.Align := alTop;
    BodyMemo.Align := alTop;
    BeginMemo.Align := alTop;
    HeaderMemo.Align := alTop;
    DeclareMemo.Align := alTop;
  end; { if }

  if Assigned(Instance) then begin
    AvailableCB.Enabled := true;
    DefinedLB.Enabled := true;
    Count := GetPropList(Instance.ClassInfo,tkProperties,nil);
    GetMem(PropList,Count * SizeOf(pointer));
    try
      GetPropList(Instance.ClassInfo,tkProperties,PropList);
      for I1 := 0 to Count - 1 do begin
        PropInfo := PropList^[I1];

      { Don't display if property is not visible }
//!!!!    If not PropertyVisible(Instance.ClassType,PropInfo.Name) then Continue;

      { Don't show non-event properties }
        if (PropInfo.PropType^.Kind <> tkClass) or not GetTypeData(
         PropInfo.PropType^).ClassType.InheritsFrom(TRaveEvent) then Continue;

        if GetOrdProp(Instance,PropInfo) = 0 then begin
          AvailableCB.Items.Add(PropInfo.Name);
        end else begin
          DefinedLB.Items.Add(PropInfo.Name);
        end; { else }
      end; { for }
    finally
      FreeMem(PropList,Count * SizeOf(pointer));
    end; { tryf }

    if AvailableCB.Items.Count = 0 then begin
      AvailableCB.Enabled := false;
    end; { if }
    if DefinedLB.Items.Count = 0 then begin
      DefinedLB.Enabled := false;
    end else begin
      DefinedLB.ItemIndex := 0;
      DefinedLBClick(DefinedLB);
    end; { else }
  end else begin
    AvailableCB.Enabled := false;
    DefinedLB.Enabled := false;
  end; { else }
  if btnShowPageEvents.Down then begin
    btnShowPageEventsClick(btnShowPageEvents);
  end else begin
    FillEventDesc;
  end; { else }
  btnDeleteEvent.Enabled := DefinedLB.Items.Count > 0;
  sboxEditor.Visible := DefinedLB.Items.Count > 0;
  BodyMemo.ReadOnly := not sboxEditor.Visible;
  HeaderMemo.ReadOnly := not sboxEditor.Visible;
end;  { Setup }

procedure TSourceEditorForm.Finish;
begin { Finish }
  SaveCurrentEvent;
end;  { Finish }

procedure TSourceEditorForm.SaveCurrentEvent;
begin { SaveCurrentEvent }
  if Assigned(Event) then begin
    if Event.Body <> BodyMemo.Text then begin
      ProjectManager.DataChanged := true;
      ProjectManager.CompileNeeded := true;
      Event.Body := BodyMemo.Text;
    end; { if }
    if Event.Header <> HeaderMemo.Text then begin
      ProjectManager.DataChanged := true;
      ProjectManager.CompileNeeded := true;
      Event.Header := HeaderMemo.Text;
    end; { if }
  end; { if }
  SetStatusLabelAttributes(false);
  StatusLabel.Caption := '';
end;  { SaveCurrentEvent }

procedure TSourceEditorForm.DefinedLBClick(Sender: TObject);
var
  I1: integer;
  EventName: string;
begin
  if not DefinedLB.Enabled then Exit;

  SaveCurrentEvent;

  EventName := DefinedLB.Items[DefinedLB.ItemIndex];
  I1 := Pos('.',EventName);
  if I1 > 0 then begin
    Instance := CurrPage.FindComponent(Copy(EventName,1,I1 - 1)) as TRaveComponent;
    Delete(EventName,1,I1);
  end; { if }
  Event := TRaveEvent(GetOrdProp(Instance,
  GetPropInfo(Instance.ClassInfo,EventName)));

  FillEventDesc;

{ Initialize Memos }
  sboxEditor.Visible := true;
  DeclareMemo.Text := Event.Declaration;
  if Event.Header = '' then begin
    HeaderMemo.Text := '';
    HeaderMemo.Change;
  end else begin
    HeaderMemo.Text := Event.Header;
  end; { else }
  HeaderMemo.Event := Event;
  BeginMemo.Text := {Trans-}'begin';
  if Event.Body = '' then begin
    BodyMemo.Text := '';
    BodyMemo.Change;
  end else begin
    BodyMemo.Text := Event.Body;
  end; { else }
  BodyMemo.Event := Event;
  EndMemo.Text := 'end ' + Event.Name + ';';

  btnDeleteEvent.Enabled := DefinedLB.Items.Count > 0;
  BodyMemo.ReadOnly := DefinedLB.Items.Count = 0;
  HeaderMemo.ReadOnly := DefinedLB.Items.Count = 0;
end; { DefinedLBClick }

procedure TSourceEditorForm.AvailableCBClick(Sender: TObject);
var
  PropInfo: PPropInfo;
  TypeData: PTypeData;
begin
  SaveCurrentEvent;
  if AvailableCB.Text = '' then Exit;

{ Create event and open in editor }
  PropInfo := GetPropInfo(Instance.ClassInfo,AvailableCB.Text);
  TypeData := GetTypeData(PropInfo^.PropType^);
  Event := TRaveEvent(GetOrdProp(Instance,PropInfo));
  if not Assigned(Event) then begin { Create event }
    Event := TypeData.ClassType.NewInstance as TRaveEvent;
    Event.Create;
    Event.Owner := Instance;
    SetOrdProp(Instance,PropInfo,integer(Event));
    Event.Name := AvailableCB.Text;
  end else begin
  //!!! Error, defined events shouldn't be in AvailableCB
  end; { if }

{ Remove event name from AvailableCB, add to DefinedLB and make active }
  DefinedLB.Enabled := true;
  DefinedLB.ItemIndex := DefinedLB.Items.Add(AvailableCB.Text);
  AvailableCB.Items.Delete(AvailableCB.ItemIndex);
  if AvailableCB.Items.Count = 0 then begin
    AvailableCB.Enabled := false;
  end; { if }
  DefinedLBClick(DefinedLB);
  ProjectManager.DataChanged := true;
  ProjectManager.CompileNeeded := true;
  BodyMemo.Text := '';
  HeaderMemo.Text := '';
  SetStatusLabelAttributes(false);
  StatusLabel.Caption := '';
end;

procedure TSourceEditorForm.FormCreate(Sender: TObject);
begin
  FormTrans(self);
end;

procedure TSourceEditorForm.btnShowSelectedEventsClick(Sender: TObject);
begin
  SaveCurrentEvent;
  AvailableCB.Enabled := true;
  Setup;
end;

procedure TSourceEditorForm.btnShowPageEventsClick(Sender: TObject);
var
  Count: integer;
  PropList: PPropList;
  PropInfo: PPropInfo;
  I1: integer;
  I2: integer;
begin
  SaveCurrentEvent;

{ Find selected component }
  if CurrentDesigner.Selections = 0 then begin
    CurrPage := nil;
  end else begin
    CurrPage := CurrentDesigner.Selection[0].Page;
  end; { else }

{ Init AvailableCB and DefinedLB }
  AvailableCB.Clear;
  AvailableCB.Enabled := false;
  DefinedLB.Clear;
  EventDesc.Text := '';

  if Assigned(CurrPage) then begin
    DefinedLB.Enabled := true;
    for I2 := 0 to (CurrPage.ComponentCount - 1) do begin
      Instance := CurrPage.Components[I2] as TRaveComponent;

      Count := GetPropList(Instance.ClassInfo,tkProperties,nil);
      GetMem(PropList,Count * SizeOf(pointer));
      try
        GetPropList(Instance.ClassInfo,tkProperties,PropList);
        for I1 := 0 to Count - 1 do begin
          PropInfo := PropList^[I1];

        { Don't display if property is not visible }
//!!!     If not PropertyVisible(Instance.ClassType,PropInfo.Name) then Continue;

        { Don't show non-event properties }
          if (PropInfo.PropType^.Kind <> tkClass) or not GetTypeData(
           PropInfo.PropType^).ClassType.InheritsFrom(TRaveEvent) then Continue;

          if GetOrdProp(Instance,PropInfo) <> 0 then begin
            DefinedLB.Items.Add(Instance.Name + '.' + PropInfo.Name);
          end; { if }
        end; { for }
      finally
        FreeMem(PropList,Count * SizeOf(pointer));
      end; { tryf }
    end; { for }

    if AvailableCB.Items.Count = 0 then begin
      AvailableCB.Enabled := false;
    end; { if }
    if DefinedLB.Items.Count = 0 then begin
      DefinedLB.Enabled := false;
    end else begin
      DefinedLB.ItemIndex := 0;
      DefinedLBClick(DefinedLB);
    end; { else }
  end else begin
    DefinedLB.Enabled := false;
  end; { else }
end;

procedure TSourceEditorForm.SelectEvent(EventName: string);
var
  I1: integer;
begin { SelectEvent }
  for I1 := 0 to DefinedLB.Items.Count - 1 do begin
    if CompareText(EventName,DefinedLB.Items[I1]) = 0 then begin
      DefinedLB.ItemIndex := I1;
    end; { if }
  end; { for }
end;  { SelectEvent }

procedure TSourceEditorForm.butnCompileClick(Sender: TObject);
begin
  SaveCurrentEvent;
  ProjectManager.Compile;
end;

procedure TSourceEditorForm.BodyMemoChange(Sender: TObject);
begin
  if Assigned(Event) then begin
    if Event.Body <> BodyMemo.Text then begin
      ProjectManager.DataChanged := true;
      ProjectManager.CompileNeeded := true;
      Event.Body := BodyMemo.Text;
    end; { if }
  end; { if }
  SetStatusLabelAttributes(false);
  StatusLabel.Caption := '';
end;

procedure TSourceEditorForm.EventDescEnter(Sender: TObject);
begin
  BodyMemo.SetFocus;
end;

procedure TSourceEditorForm.HeaderMemoChange(Sender: TObject);
begin
  if Assigned(Event) then begin
    if Event.Header <> HeaderMemo.Text then begin
      ProjectManager.DataChanged := true;
      ProjectManager.CompileNeeded := true;
      Event.Header := HeaderMemo.Text;
    end; { if }
  end; { if }
  SetStatusLabelAttributes(false);
  StatusLabel.Caption := '';
end;

procedure TSourceEditorForm.btnDeleteEventClick(Sender: TObject);
var
  PropInfo: PPropInfo;
begin
  if true then begin
  end; { if }
  if Assigned(Instance) and Assigned(Event) and (MessageDlg(Trans({Trans+}'Are you sure you want to delete this event?'),
   mtConfirmation,[mbYes,mbNo,mbCancel],0) = mrYes) then begin
    PropInfo := GetPropInfo(Instance.ClassInfo,Event.Name);
    SetOrdProp(Instance,PropInfo,0);
    FreeAndNil(Event);
    Setup;
  end; { if }
end;

procedure TSourceEditorForm.SetStatusLabelAttributes(ADoError: boolean);
begin
  if ADoError then begin
    StatusLabel.Color := clRed;
    StatusLabel.Font.Color := clWhite;
    StatusLabel.Font.Style := StatusLabel.Font.Style + [fsBold];
  end else begin
    StatusLabel.Color := clBtnFace;
    StatusLabel.Font.Color := clBtnText;
    StatusLabel.Font.Style := SourceEditorForm.StatusLabel.Font.Style - [fsBold];
  end; { else }
end;

{ TRaveMemo }

procedure TRaveMemo.Change;
begin
  inherited;
  FEndingCRLF := (Length(Text) > 0) and (Text[Length(Text)] in [#10,#13]);
  Height := 4 + LineCount * 16;
end;

constructor TRaveMemo.Create(AOwner: TComponent);
begin
  inherited;
  BorderStyle := bsNone;
  WordWrap := false;
  Font.Name := 'Courier New';
  Font.Size := 10;
end;

function TRaveMemo.GetLineCount: integer;
begin
  Result := Lines.Count;
  if FEndingCRLF or (Result = 0) then begin
    Inc(Result);
  end; { if }
end;

procedure TRaveMemo.KeyDown(var Key: Word; Shift: TShiftState);
var
  Line: integer;
  Column: integer;
begin
  case Key of
    VK_UP: begin
      Line := CaretPos.Y;
      Column := CaretPos.X;
      if Line = 0 then begin
        if Assigned(PrevMemo) then begin
          PrevMemo.SetFocus;
          PrevMemo.CaretPos := Point(Column,PrevMemo.LineCount - 1);
        end; { if }
        Key := 0;
      end; { if }
    end;
    VK_DOWN: begin
      Line := CaretPos.Y;
      Column := CaretPos.X;
      if (Line = LineCount - 1) then begin
        if Assigned(NextMemo) then begin
          NextMemo.SetFocus;
          NextMemo.CaretPos := Point(Column,0);
        end; { if }
        Key := 0;
      end; { if }
    end;
    VK_LEFT: begin
      if (CaretPos.X = 0) and (CaretPos.Y = 0) then begin
        if Assigned(PrevMemo) then begin
          PrevMemo.SetFocus;
          PrevMemo.CaretPos := Point(Length(PrevMemo.Lines[PrevMemo.LineCount - 1]),PrevMemo.LineCount - 1);
        end; { if }
        Key := 0;
      end; { if }
    end;
    VK_RIGHT: begin
      if (CaretPos.Y = LineCount - 1) and (CaretPos.X = Length(Lines[LineCount - 1])) then begin
        if Assigned(NextMemo) then begin
          NextMemo.SetFocus;
          NextMemo.CaretPos := Point(0,0);
        end; { if }
        Key := 0;
      end; { if }
    end;
    VK_PRIOR: begin
    //!!! Need to handle PgUp key
    end;
    VK_NEXT: begin
    //!!! Need to handle PgDn key
    end;
  end; { case }
end;

procedure TRaveMemo.SetNextMemo(Value: TRaveMemo);
begin
  FNextMemo := Value;
  FNextMemo.FPrevMemo := self;
end;

procedure TRaveMemo.SetPrevMemo(Value: TRaveMemo);
begin
  FPrevMemo := Value;
  FPrevMemo.FNextMemo := self;
end;

procedure TRaveMemo.MarkReadOnly;
begin
  Color := $00EBEBEB;
  ReadOnly := true;
  TabStop := false;
  Font.Name := 'Tahoma';
  Font.Size := 10;
  Font.Style := [fsBold];
  Font.Color := clGray;
end;

procedure CompileStatus(CompileStatus: TRaveCompileStatus);
begin
  if Assigned(CompileStatus) then begin
    ModuleEventHandler.CompileError(CompileStatus);
  end else begin
    ModuleEventHandler.CompileSuccess;
  end; { else }
end;

initialization
  CompileStatusProc := CompileStatus;
  RegisterProc({Trans-}'RVCL',RaveRegister);
end.