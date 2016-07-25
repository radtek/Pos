{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RpCEdFrm;

{$I RpVer.inc}

interface

uses
  SysUtils, WinTypes, WinProcs, Messages, Classes, Graphics, Controls,
  Forms, Dialogs, StdCtrls, Buttons, Clipbrd,
  {$IFDEF LEVEL6}DesignIntf, DesignWindows,{$ELSE}DsgnIntf, DsgnWnds,{$ENDIF}
  RpTable, ExtCtrls, TabNotBk, RpDBTabl, RpAddFrm, DB, DBTables, RpDefine, ComCtrls;

type
  TColumnEditorForm = class(TDesignWindow)
    TabbedNotebook1: TTabbedNotebook;
    LBColumns: TListBox;
    BBAdd: TBitBtn;
    BBDelete: TBitBtn;
    SBUp: TSpeedButton;
    SBDown: TSpeedButton;
    BBOk: TBitBtn;
    Label1: TLabel;
    LBActive: TListBox;
    LBAvailable: TListBox;
    SBLeftAll: TSpeedButton;
    SBRight: TSpeedButton;
    SBLeft: TSpeedButton;
    SBRightAll: TSpeedButton;
    Label2: TLabel;
    Label3: TLabel;
    BitBtn1: TBitBtn;
    Label4: TLabel;
    CurrentWidthLabel: TLabel;
    Label5: TLabel;
    DesiredWidthEdit: TEdit;
    BitBtn3: TBitBtn;
    BitBtn4: TBitBtn;
    BitBtn5: TBitBtn;
    procedure FormClose(    Sender: TObject;
                        var Action: TCloseAction);
    procedure BBOkClick(Sender: TObject);
    procedure BBAddClick(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure LBColumnsClick(Sender: TObject);
    procedure LBColumnsDragOver(Sender, Source: TObject; X, Y: integer;
      State: TDragState; var Accept: Boolean);
    procedure LBColumnsMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: integer);
    procedure LBColumnsDragDrop(Sender, Source: TObject; X, Y: integer);
    procedure SBUpClick(Sender: TObject);
    procedure SBDownClick(Sender: TObject);
    procedure BBDeleteClick(Sender: TObject);
    procedure SBLeftClick(Sender: TObject);
    procedure LBAvailableDrawItem(Control: TWinControl; Index: integer;
      Rect: TRect; State: TOwnerDrawState);
    procedure SBRightClick(Sender: TObject);
    procedure SBLeftAllClick(Sender: TObject);
    procedure SBRightAllClick(Sender: TObject);
    procedure LBActiveClick(Sender: TObject);
    procedure LBAvailableClick(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure BitBtn1Click(Sender: TObject);
    procedure TabbedNotebook1Change(Sender: TObject; NewTab: integer;
      var AllowChange: Boolean);
    procedure BitBtn4Click(Sender: TObject);
    procedure BitBtn3Click(Sender: TObject);
  private
    TablePrinter: TTablePrinter;
    TablePrinterForm: TForm;
    DisableSelection: boolean;
    TableWidth: double;

    procedure MoveColumn(FromIndex, ToIndex: integer);
    procedure AddSections(All: boolean);
    procedure DelSections(All: boolean);
  protected
    function UniqueName(Component: TComponent): string; override;
  public
  {$IFDEF LEVEL6}
    procedure DesignerClosed(const Designer: IDesigner; AGoingDormant: Boolean); override;
    procedure SelectionChanged(const ADesigner: IDesigner; const ASelection: IDesignerSelections); override;
    procedure ItemDeleted(const ADesigner: IDesigner; Item: TPersistent); override;
    procedure ItemsModified(const Designer: IDesigner); override;
  {$ELSE}
    procedure FormClosed(AForm: TCustomForm); override;
    procedure SelectionChanged(ASelection: {$IFDEF LEVEL5}TDesignerSelectionList{$ELSE}TComponentList{$ENDIF}); override;
    procedure ComponentDeleted(Component: IPersistent); override;
    procedure FormModified; override;
  {$ENDIF}
  end; { TColumnEditorForm }

  procedure ColumnEditor(Designer: {$IFDEF LEVEL6}IDesigner{$ELSE}IFormDesigner{$ENDIF};
                         TablePrinter: TTablePrinter);

implementation

{$R *.dfm}

var
  SaveList: TList;

procedure AddForm(ColumnEditor: TColumnEditorForm);
begin { AddForm }
  if SaveList = nil then begin
    SaveList := TList.Create;
  end; { if }
  SaveList.Add(ColumnEditor);
end;  { AddForm }

procedure RemoveForm(ColumnEditor: TColumnEditorForm);
begin { RemoveForm }
  if SaveList <> nil then begin
    SaveList.Remove(ColumnEditor);
    if SaveList.Count = 0 then begin
      SaveList.Free;
      SaveList := nil;
    end; { if }
  end; { if }
end;  { RemoveForm }

function FindForm(TablePrinter: TTablePrinter): TColumnEditorForm;
var
  I1: integer;
  Form: TColumnEditorForm;
begin { FindForm }
  Result := nil;
  if SaveList <> nil then begin
    for I1 := 1 to SaveList.Count do begin
      Form := TColumnEditorForm(SaveList[I1 - 1]);
      if Form.TablePrinter = TablePrinter then begin
        Result := Form;
        Exit;
      end; { if }
    end; { for }
  end; { if }
end;  { FindForm }

procedure ColumnEditor(Designer: {$IFDEF LEVEL6}IDesigner{$ELSE}IFormDesigner{$ENDIF};
                        TablePrinter: TTablePrinter);
var
  ColumnEditor: TColumnEditorForm;
begin { ColumnEditor }
{ Look for a ColumnEditorForm already open for this table }
  ColumnEditor := FindForm(TablePrinter);
  if ColumnEditor <> nil then begin { Found }
    ColumnEditor.Show;
    ColumnEditor.WindowState := wsNormal;
  end else begin
    ColumnEditor := TColumnEditorForm.Create(nil);
    AddForm(ColumnEditor);
    ColumnEditor.TablePrinter := TablePrinter;
    ColumnEditor.TablePrinterForm := TablePrinter.Owner as TForm;
    ColumnEditor.Designer := Designer;
    ColumnEditor.Caption := TablePrinter.Owner.Name + '.' + TablePrinter.Name +
      ' ' + Trans('Table Editor');
    ColumnEditor.Show;
  end; { else }
end;  { ColumnEditor }

{ TColumnEditorForm }

function TColumnEditorForm.UniqueName(Component: TComponent): string;
begin { UniqueName }
  Result := 'ReportPrinterTableColumnEditor';
end;  { UniqueName }

procedure TColumnEditorForm.FormClose(    Sender: TObject;
                                      var Action: TCloseAction);
begin { FormClose }
  Action := caFree;
end;  { FormClose }

procedure TColumnEditorForm.FormDestroy(Sender: TObject);
begin { FormDestroy }
  RemoveForm(self);
end;  { FormDestroy }

{$IFDEF LEVEL6}
procedure TCOlumnEditorForm.ItemsModified(const Designer: IDesigner);
{$ELSE}
procedure TColumnEditorForm.FormModified;
{$ENDIF}
var
  S1: string[128];
  I1: integer;
  TC: TTableColumn;
begin { FormModified }
{ See if Form or TablePrinter Name has changed }
  S1 := TablePrinterForm.Name + '.' + TablePrinter.Name + ' ' + Trans('Table Editor');
  if S1 <> Caption then begin
    Caption := S1;
  end; { if }

{ See if the names of any TableColumns have changed }
  TableWidth := 0.0;
  for I1 := 1 to TablePrinter.TableColumns do begin
    TC := TablePrinter.TableColumn[I1];
    TableWidth := TableWidth + TC.Width;
    if TC.Description <> LBColumns.Items[I1 - 1] then begin
      LBColumns.Items[I1 - 1] := TC.Description;
    end; { if }
  end; { for }

  Str(TableWidth:2:2,S1);
  CurrentWidthLabel.Caption := S1;
end;  { FormModified }

{$IFDEF LEVEL6}
procedure TColumnEditorForm.DesignerClosed(const Designer: IDesigner; AGoingDormant: Boolean);
begin { DesignerClosed }
  if Designer.Root = TablePrinterForm then begin
    DisableSelection := true;
    Close;
  end; { if }
end;  { DesignerClosed }
{$ELSE}
procedure TColumnEditorForm.FormClosed(AForm: TCustomForm);
begin { FormClosed }
  if AForm = TablePrinterForm then begin
    DisableSelection := true;
    Close;
  end; { if }
end;  { FormClosed }
{$ENDIF}

{$IFDEF LEVEL6}
procedure TColumnEditorForm.SelectionChanged(const ADesigner: IDesigner; const ASelection: IDesignerSelections);
{$ELSE}
procedure TColumnEditorForm.SelectionChanged(ASelection: {$IFDEF LEVEL5}TDesignerSelectionList{$ELSE}TComponentList{$ENDIF});
{$ENDIF}
var
  I1: integer;
  I2: integer;
  TC1: TTableColumn;
  TC2: TTableColumn;
  TS: TTableSection;
begin { SelectionChanged }
  if DisableSelection then Exit;

{ Deselect all items in selection bar }
  with LBColumns do begin
    for I1 := 0 to (Items.Count - 1) do begin
      Selected[I1] := false;
    end; { for }
  end; { with }
  with LBActive do begin
    for I1 := 0 to (Items.Count - 1) do begin
      Selected[I1] := false;
    end; { for }
  end; { with }
  with LBAvailable do begin
    for I1 := 0 to (Items.Count - 1) do begin
      Selected[I1] := false;
    end; { for }
  end; { with }

{ Change selection on light bar to match new selection in object inspector }
  for I1 := 0 to (ASelection.Count - 1) do begin
    if ASelection[I1] is TTableColumn then begin
      TC1 := ASelection[I1] as TTableColumn;
      for I2 := 0 to TablePrinter.TableColumnList.Count - 1 do begin
        TC2 := TablePrinter.TableColumnList[I2];
        if TC1 = TC2 then begin
          LBColumns.Selected[I2] := true;
          TabbedNotebook1.PageIndex := 1;
        end; { if }
      end; { for }
    end else if ASelection[I1] is TTableSection then begin
      TS := ASelection[I1] as TTableSection;
      if TS = TablePrinter.TableSection[TS.SectionType] then begin
        LBActive.Selected[Ord(TS.SectionType)] :=true;
        TabbedNotebook1.PageIndex := 0;
      end; { if }
    end; { else }
  end; { for }
end;  { SelectionChanged }

{$IFDEF LEVEL6}
procedure TColumnEditorForm.ItemDeleted(const ADesigner: IDesigner; Item: TPersistent);
begin { ItemDeleted }
  if Item = TablePrinter then begin
    DisableSelection := true;
    Close;
  end; { if }
end;  { ItemDeleted }
{$ELSE}
procedure TColumnEditorForm.ComponentDeleted(Component: IPersistent);
begin { ComponentDeleted }
  if Component.Equals(MakeIPersistent(TablePrinter)) then begin
    DisableSelection := true;
    Close;
  end; { if }
end;  { ComponentDeleted }
{$ENDIF}

procedure TColumnEditorForm.BBOkClick(Sender: TObject);
begin
  Close;
end;

procedure TColumnEditorForm.BBAddClick(Sender: TObject);
var
  ColumnName: string;
  I1: integer;
  Field: TField;
  DataSetActive: boolean;
  Handled: boolean;
begin
  Handled := false;

  if TablePrinter is TDBTablePrinter then begin
    try
      with TAddColumnForm.Create(Application) do begin
        DataSetActive := false;
        try
          DataSet := (TablePrinter as TDBTablePrinter).DataSet;
          if DataSet <> nil then begin
            DataSetActive := DataSet.Active;
            if not DataSetActive then begin
              DataSet.Active := true;
            end; { if }

            ShowModal;
            Handled := true;

          { Add all columns in FieldList }
            for I1 := 1 to FieldList.Count do begin
              with TablePrinter.CreateColumn(FieldList[I1 - 1]) as TDBTableColumn do begin
                if AddFields then begin
                  FieldName := FieldList[I1 - 1];
                  Field := DataSet.FindField(FieldList[I1 - 1]);
                  if Field <> nil then begin
                    Width := Field.DisplayWidth / 10.0;
                  end; { if }
                end; { if }
                Heading := FieldList[I1 - 1];
              end; { with }
              with LBColumns do begin
                Items.Add(FieldList[I1 - 1]);
                ItemIndex := Items.Count - 1;
                Selected[ItemIndex] := true;
              end; { with }
            end; { for }
          end; { if }
        finally
          if not DataSetActive and (DataSet <> nil) then begin
            DataSet.Active := false;
          end; { if }
          Free;
        end; { tryf }
      end; { with }
      Designer.Modified;
    except
    end; { tryx }
  end; { if }

  if not Handled then begin
  { Ask for name of column to add }
    ColumnName := '';
    if InputQuery(Trans('Add Column'),Trans('Enter a description for this column'),
      ColumnName) then begin
      with TablePrinter.CreateColumn(ColumnName) do begin
        Heading := ColumnName;
      end; { with }
      with LBColumns do begin
        Items.Add(ColumnName);
        ItemIndex := Items.Count - 1;
        Selected[ItemIndex] := true;
      end; { with }
      Designer.Modified;
    end; { if }
  end; { else }
end;

procedure TColumnEditorForm.BBDeleteClick(Sender: TObject);
var
  I1: integer;
begin
  I1 := LBColumns.ItemIndex;
  if (I1 < 0) or (LBColumns.Items.Count = 0) then Exit;

{ Free up TableColumn component }
  TablePrinter.TableColumn[I1 + 1].Free;

{ Delete from listbox }
  with LBColumns do begin
    Items.Delete(I1);
    if Items.Count > 0 then begin
      if I1 >= Items.Count then begin
        I1 := Items.Count - 1;
      end; { if }
      Selected[I1] := true;
      ItemIndex := I1;
    end; { if }
  end; { with }

  LBColumnsClick(self);
  Designer.Modified;
end;

procedure TColumnEditorForm.SBUpClick(Sender: TObject);
begin
  if LBColumns.ItemIndex > 0 then begin
    MoveColumn(LBColumns.ItemIndex,LBColumns.ItemIndex - 1);
  end; { if }
end;

procedure TColumnEditorForm.SBDownClick(Sender: TObject);
begin
  if LBColumns.ItemIndex < (LBColumns.Items.Count - 1) then begin
    MoveColumn(LBColumns.ItemIndex,LBColumns.ItemIndex + 1);
  end; { if }
end;

procedure TColumnEditorForm.FormShow(Sender: TObject);
var
  I1: integer;
  SectionType: TSectionType;
begin
  with LBColumns.Items do begin
    BeginUpdate;
  { Delete all item in LBColumns }
    while Count > 0 do begin
      Delete(Count);
    end; { while }

  { Fill LBColumns with names of columns }
    with TablePrinter do begin
      for I1 := 1 to TableColumns do begin
        Add(TableColumn[I1].Description);
      end; { for }
    end; { with }
    EndUpdate;
  end; { with }

  with TablePrinter do begin
    for SectionType := stReportHeader to stReportFooter do begin
      LBActive.Items.AddObject(SectionName[SectionType],
       pointer(TableSection[SectionType] <> nil));
      LBAvailable.Items.AddObject(SectionName[SectionType],
       pointer(TableSection[SectionType] = nil));
    end; { for }
  end; { with }
end;

procedure TColumnEditorForm.MoveColumn(FromIndex, ToIndex: integer);
var
  I1: integer;
begin { MoveColumn }
  TablePrinter.TableColumnList.Move(FromIndex,ToIndex);
  LBColumns.Items.Move(FromIndex,ToIndex);
{ LBColumns.ItemIndex := ToIndex; { Select item just moved }
  for I1 := 0 to LBColumns.Items.Count - 1 do begin
    LBColumns.Selected[I1] := (I1 = ToIndex);
  end; { for }
  LBColumnsClick(self); { Reselect since selected changed }
  Designer.Modified;
end;  { MoveColumn }

procedure TColumnEditorForm.LBColumnsClick(Sender: TObject);
var
  CompList: {$IFDEF LEVEL6}IDesignerSelections{$ELSE}
   {$IFDEF LEVEL5}TDesignerSelectionList{$ELSE}TComponentList{$ENDIF}{$ENDIF};
  I1: integer;
begin { LBColumnsClick }
{ Deselect any items in LBAvailable and LBActive }
  for I1 := 1 to LBAvailable.Items.Count do begin
    LBAvailable.Selected[I1 - 1] := false;
  end; { for }
  for I1 := 1 to LBActive.Items.Count do begin
    LBActive.Selected[I1 - 1] := false;
  end; { for }

{ Set SpeedButton's to active/inactive base on selection }
  if (LBColumns.ItemIndex < 0) or (LBColumns.SelCount <> 1) then begin
    SBUp.Enabled := false;
    SBDown.Enabled := false;
    BBDelete.Enabled := false;
  end else begin
    SBUp.Enabled := (LBColumns.ItemIndex > 0);
    SBDown.Enabled := (LBColumns.ItemIndex < (LBColumns.Items.Count - 1));
    BBDelete.Enabled := true;
  end; { else }

  CompList := {$IFDEF LEVEL6}TDesignerSelections{$ELSE}
   {$IFDEF LEVEL5}TDesignerSelectionList{$ELSE}TComponentList{$ENDIF}{$ENDIF}.Create;
  if LBColumns.SelCount = 0 then begin { Select the TablePrinter in the OI }
    CompList.Add(TablePrinter);
  end else begin { Select the TTableColumn componenets in the OI }
    for I1 := 1 to LBColumns.Items.Count do begin
      if LBColumns.Selected[I1 - 1] then begin
        CompList.Add(TablePrinter.TableColumn[I1]);
      end; { if }
    end; { for }
  end; { else }
  DisableSelection := true;
  SetSelection(CompList);
  DisableSelection := false;
{ CompList.Free; Don't free CompList since it is now owned by the OI }
end;  { LBColumnsClick }

procedure TColumnEditorForm.LBColumnsDragOver(    Sender, Source: TObject;
                                                  X,Y: integer;
                                                  State: TDragState;
                                              var Accept: Boolean);
var
  I1: integer;
begin
  I1 := LBColumns.ItemAtPos(Point(X,Y),true);
  Accept := LBColumns.SelCount = 1;
  if Accept then begin
    Accept := (Source = LBColumns) and (I1 <> LBColumns.ItemIndex) and (I1 >= 0);
  end; { if }
end;

procedure TColumnEditorForm.LBColumnsMouseDown(Sender: TObject;
                                              Button: TMouseButton;
                                              Shift: TShiftState;
                                              X, Y: integer);
begin
  if (Button = mbLeft) and (([ssShift,ssAlt,ssCtrl] * Shift) = []) then begin
    with Sender as TListBox do begin
      if ItemAtPos(Point(X,Y),true) >= 0 then begin
        BeginDrag(false);
      end; { if }
    end; { with }
  end; { if }
end;

procedure TColumnEditorForm.LBColumnsDragDrop(Sender, Source: TObject;
                                              X, Y: integer);
begin
  MoveColumn(LBColumns.ItemIndex,LBColumns.ItemAtPos(Point(X,Y),true));
end;

procedure TColumnEditorForm.AddSections(All: boolean);
var
  I1: integer;
  SectionType: TSectionType;
  TS: TTableSection;
  Modified: boolean;
begin { AddSections }
{ Move TableSections from LBAvailable to LBActive }
  LBActive.Items.BeginUpdate;
  LBAvailable.Items.BeginUpdate;
  Modified := false;
  for I1 := 0 to (LBAvailable.Items.Count - 1) do begin
    if LBAvailable.Selected[I1] or All then begin
      SectionType := TSectionType(I1);
      TS := TablePrinter.TableSection[SectionType];
      if not Assigned(TS) then begin { Add the new TTableSection }
        TablePrinter.CreateSection(SectionName[SectionType],SectionType);
      end; { if }

    { Update list boxes }
      LBActive.Items.Objects[I1] := pointer(true);
      LBActive.Selected[I1] := true;
      LBAvailable.Items.Objects[I1] := pointer(false);
      LBAvailable.Selected[I1] := false;
      Modified := true;
    end; { if }
  end; { for }
  if Modified then begin
    LBActive.SetFocus;
    LBActiveClick(self);
    Designer.Modified;
  end; { if }
  LBAvailable.Items.EndUpdate;
  LBActive.Items.EndUpdate;
end;  { AddSections }

procedure TColumnEditorForm.DelSections(All: boolean);
var
  I1: integer;
  TS: TTableSection;
  Modified: boolean;
begin { DelSections }
  LBActive.Items.BeginUpdate;
  LBAvailable.Items.BeginUpdate;
  Modified := false;
  for I1 := 0 to (LBActive.Items.Count - 1) do begin
    if LBActive.Selected[I1] or All then begin
      TS := TablePrinter.TableSection[TSectionType(I1)];
      if Assigned(TS) then begin { Free TableSection component }
        TS.Free;
      end; { if }

    { Move item from LBActive to LBAvailable }
      LBActive.Items.Objects[I1] := pointer(false);
      LBActive.Selected[I1] := false;
      LBAvailable.Items.Objects[I1] := pointer(true);
      LBAvailable.Selected[I1] := true;
      Modified := true;
    end; { if }
  end; { for }
  if Modified then begin
    LBAvailable.SetFocus;
    LBAvailableClick(self);
    Designer.Modified;
  end; { if }
  LBAvailable.Items.EndUpdate;
  LBActive.Items.EndUpdate;
end;  { DelSections }

procedure TColumnEditorForm.SBLeftClick(Sender: TObject);
begin
  AddSections(false);
end;

procedure TColumnEditorForm.SBLeftAllClick(Sender: TObject);
begin
  AddSections(true);
end;

procedure TColumnEditorForm.SBRightClick(Sender: TObject);
begin
  DelSections(false);
end;

procedure TColumnEditorForm.SBRightAllClick(Sender: TObject);
begin
  DelSections(true);
end;

procedure TColumnEditorForm.LBAvailableDrawItem(Control: TWinControl;
                                                Index: integer;
                                                Rect: TRect;
                                                State: TOwnerDrawState);
begin
{ Draw the item as either black or gray depending upon Objects value }
  with (Control as TListBox),(Control as TListBox).Canvas do begin
    FillRect(Rect);
    if longint(Items.Objects[Index]) = 0 then begin { Inactive }
      if odSelected in State then begin
        Font.Color := clDkGray;
      end else begin
        Font.Color := clLtGray;
      end; { else }
    end else begin { Active }
      if odSelected in State then begin
        Font.Color := clWhite;
      end else begin
        Font.Color := clBlack;
      end; { else }
    end; { else }
    TextOut(Rect.Left + 2,Rect.Top,Items[Index]);
  end; { with }
end;

procedure TColumnEditorForm.LBActiveClick(Sender: TObject);
var
  I1: integer;
  CompList: {$IFDEF LEVEL6}IDesignerSelections{$ELSE}
   {$IFDEF LEVEL5}TDesignerSelectionList{$ELSE}TComponentList{$ENDIF}{$ENDIF};
  TS: TTableSection;
begin
{ Deselect any items in LBColumns and LBAvailable }
  for I1 := 1 to LBColumns.Items.Count do begin
    LBColumns.Selected[I1 - 1] := false;
  end; { for }
  for I1 := 1 to LBAvailable.Items.Count do begin
    LBAvailable.Selected[I1 - 1] := false;
  end; { for }

  CompList := {$IFDEF LEVEL6}TDesignerSelections{$ELSE}
   {$IFDEF LEVEL5}TDesignerSelectionList{$ELSE}TComponentList{$ENDIF}{$ENDIF}.Create;
  for I1 := 1 to LBActive.Items.Count do begin
    if LBActive.Selected[I1 - 1] then begin
      TS := TablePrinter.TableSection[TSectionType(I1 - 1)];
      if Assigned(TS) then begin
        CompList.Add(TS);
      end; { if }
    end; { if }
  end; { for }
  if CompList.Count = 0 then begin
    CompList.Add(TablePrinter);
  end; { if }
  DisableSelection := true;
  SetSelection(CompList);
  DisableSelection := false;
end;

procedure TColumnEditorForm.LBAvailableClick(Sender: TObject);
var
  I1: integer;
  CompList: {$IFDEF LEVEL6}IDesignerSelections{$ELSE}
    {$IFDEF LEVEL5}TDesignerSelectionList{$ELSE}TComponentList{$ENDIF}{$ENDIF};
begin
{ Deselect any items in LBColumns and LBActive }
  for I1 := 1 to LBColumns.Items.Count do begin
    LBColumns.Selected[I1 - 1] := false;
  end; { for }
  for I1 := 1 to LBActive.Items.Count do begin
    LBActive.Selected[I1 - 1] := false;
  end; { for }

{ Just display the TablePrinter component }
  CompList := {$IFDEF LEVEL6}TDesignerSelections{$ELSE}
   {$IFDEF LEVEL5}TDesignerSelectionList{$ELSE}TComponentList{$ENDIF}{$ENDIF}.Create;
  CompList.Add(TablePrinter);
  DisableSelection := true;
  SetSelection(CompList);
  DisableSelection := false;
end;

procedure TColumnEditorForm.BitBtn1Click(Sender: TObject);
begin
  Close;
end;

procedure TColumnEditorForm.TabbedNotebook1Change(Sender: TObject;
  NewTab: integer; var AllowChange: Boolean);
var
  I1: integer;
  S1: string[20];
  TC: TTableColumn;
begin
  if NewTab = 2 then begin
  { Add up widths of all columns }
    TableWidth := 0.0;
    with TablePrinter do begin
      for I1 := 1 to TableColumns do begin
        TC := TableColumn[I1];
        TableWidth := TableWidth + TC.Width;
      end; { for }
    end; { with }
    Str(TableWidth:2:2,S1);
    CurrentWidthLabel.Caption := S1;
    DesiredWidthEdit.Text := S1;
  end; { if }
  AllowChange := true;
end;

procedure TColumnEditorForm.BitBtn4Click(Sender: TObject);
var
  ErrCode: integer;
  Factor: double;
  DesiredWidth: double;
  TC: TTableColumn;
  I1: integer;
  S1: string[20];
begin
  if DesiredWidthEdit.Text <> CurrentWidthLabel.Caption then begin
  { Figure out conversion factor }
    Val(DesiredWidthEdit.Text,DesiredWidth,ErrCode);
    Factor := DesiredWidth / TableWidth;
    with TablePrinter do begin
      for I1 := 1 to TableColumns do begin
        TC := TableColumn[I1];
        TC.Width := TC.Width * Factor;
      end; { for }
      TableWidth := DesiredWidth;
      Str(TableWidth:2:2,S1);
      CurrentWidthLabel.Caption := S1;
      Designer.Modified;
    end; { with }
  end; { if }
end;

procedure TColumnEditorForm.BitBtn3Click(Sender: TObject);
begin
  Close;
end;

initialization
  SaveList := nil;
end.