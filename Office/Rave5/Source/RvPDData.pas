{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RvPDData;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, Grids, ExtCtrls, RpDefine, RvTool, RvToolUtil, RvClass, RvProj, RvData, RvDirectDataView,
  StdCtrls;

type
  TDataViewerForm = class(TForm)
    DataViewerPanel: TPanel;
    DataViewerGrid: TStringGrid;
    TitlePanel: TPanel;
    DataViewNameLabel: TLabel;
    Image1: TImage;
    procedure FormCreate(Sender: TObject);
  private
    FInsideSetup: boolean;
    FRequestStop: boolean;
  public
    procedure Setup;
    //
    property InsideSetup: boolean read FInsideSetup write FInsideSetup;
    property RequestStop: boolean read FRequestStop write FRequestStop;
  end;

  TRaveDataViewer = class(TRaveProjectDesigner)
  protected
    function GetTitle: string; override;
  public
    procedure SelectionChanged; override;
    procedure Modified; override;
    procedure Enter; override;
    procedure Exit; override;
    procedure Connect(Parent: TWinControl); override;
    procedure Disconnect; override;
  end; { TRaveDataViewer }

var
  DataViewerForm: TDataViewerForm;

  procedure RaveRegister;

implementation

{$R *.dfm}

procedure RaveRegister;
begin { RaveRegister }
  DataViewerForm := TDataViewerForm.Create(Application);
  RegisterRaveProjectDesigner(TRaveDataViewer);
end;  { RaveRegister }

{ TRaveDataViewer }

procedure TRaveDataViewer.Connect(Parent: TWinControl);
begin
  DataViewerForm.DataViewerPanel.Parent := Parent;
end;

procedure TRaveDataViewer.Disconnect;
begin
  DataViewerForm.DataViewerPanel.Parent := DataViewerForm;
end;

procedure TRaveDataViewer.Enter;
begin
  DataViewerForm.Setup;
end;

procedure TRaveDataViewer.Exit;
begin
  DataViewerForm.RequestStop := true;
end;

function TRaveDataViewer.GetTitle: string;
begin
  Result := Trans('Data Viewer');
end;

procedure TRaveDataViewer.Modified;
begin
end;

procedure TRaveDataViewer.SelectionChanged;
begin
  DataViewerForm.Setup;
end;

{ TDataViewerForm }

procedure TDataViewerForm.Setup;
var
  DataView: TRaveBaseDataView;
  DataFields: TList;
  I1: integer;
  I2: integer;
begin
  if InsideSetup then begin
    RequestStop := true;
    Exit;
  end; { if }
  try
    InsideSetup := true;
    RequestStop := false;
    if Assigned(CurrentDesigner) and (CurrentDesigner.Selections > 0) and (CurrentDesigner.Selection[0] is TRaveBaseDataView) then begin
      DataView := TRaveBaseDataView(CurrentDesigner.Selection[0]);
      DataViewNameLabel.Caption := DataView.Name;
      TitlePanel.Visible := true;
      DataViewerGrid.Visible := true;
    end else begin
      DataViewerGrid.Visible := false;
      TitlePanel.Visible := false;
      Exit;
    end; { else }

    if (RaveDataSystem <> nil) and RaveDataSystem.GainControl then try
      DataFields := TList.Create;
      try
        DataView.CreateFields(DataFields);
        DataViewerGrid.ColCount := DataFields.Count + 1;
        DataViewerGrid.Cells[0,0] := Trans('Record #');
        for I1 := 0 to DataFields.Count - 1 do begin
          DataViewerGrid.Cells[I1 + 1,0] := TRaveDataFieldInfo(DataFields[I1]).FieldName;
          DataViewerGrid.ColWidths[I1 + 1] := TRaveDataFieldInfo(DataFields[I1]).Width * 8;
        end; { for }

        DataView.Open;
        DataView.First;
        DataViewerGrid.RowCount := 2;
        while not DataView.Eof do begin
          I1 := DataViewerGrid.RowCount - 1;
          DataViewerGrid.RowCount := DataViewerGrid.RowCount + 1;
          DataViewerGrid.Cells[0,I1] := IntToStr(I1);

          for I2 := 0 to DataFields.Count - 1 do begin
            if RequestStop then begin
              Exit;
            end; { if }
            DataViewerGrid.Cells[I2 + 1,I1] := DataView.FieldByName(TRaveDataFieldInfo(DataFields[I2]).FieldName).AsString;
          end; { for }

          DataView.Next;
        end; { while }

        DataViewerGrid.Cells[0,DataViewerGrid.RowCount - 1] := 'EOF';
        for I2 := 0 to DataFields.Count - 1 do begin
          if RequestStop then begin
            Exit;
          end; { if }
          DataViewerGrid.Cells[I2 + 1,DataViewerGrid.RowCount - 1] := '';
        end; { for }
      finally
        DataView.Close;
        FreeAndNil(DataFields);
      end; { tryf }
    finally
      RaveDataSystem.ReleaseControl;
    end; { if }
  finally
    InsideSetup := false;
    if RequestStop then begin
      RequestStop := false;
      Setup;
    end; { if }
  end; { tryf }
end;

procedure TDataViewerForm.FormCreate(Sender: TObject);
begin
  FormTrans(self);
end;

initialization
  RegisterProc({Trans-}'RVCL',RaveRegister);
end.
