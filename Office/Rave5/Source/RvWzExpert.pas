{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RvWzExpert;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Buttons, ExtCtrls, RvClass, RvProj, RvTool, RvToolEvents,
  CheckLst;

const
  RaveReportExpertVersion: integer = 50001; { 3.0001 }

type
  TFormReportExpert = class(TForm)
    Button1: TButton;
    ItemList: TCheckListBox;
    Button2: TButton;
    Label1: TLabel;
    Label2: TLabel;
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure FormShow(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

  TMessageType = (mtHint,mtWarning,mtError);

  TReportExpertProjectEditor = class(TRaveMasterProjectEditor,IRaveToolMenuEventHandler)
  protected
    procedure AddItem(Comp: TRaveComponent; MsgType: TMessageType; ItemText: string);
    procedure AddHint(Comp: TRaveComponent; HintIndex: integer);
    procedure AddWarning(Comp: TRaveComponent; WarningIndex: integer);
    procedure AddError(Comp: TRaveComponent; ErrorIndex: integer);
    procedure ProcessComps(Item: TRaveProjectItem);
  public
    constructor Create; override;
    destructor Destroy; override;

    procedure AddToMenu(AddMenuItem: TAddMenuItemProc);
    procedure RunFromMenu(ID: integer);
    function BeforeReportPrint: boolean; override;
    function AnalyzeReport(Report: TRaveReport): boolean;
  end; { TReportExpertProjectEditor }

var
  FormReportExpert: TFormReportExpert;

  procedure RaveRegister;

implementation

{$R *.DFM}

uses
  RvCsStd,
  RvCsDraw,
  RvCsData,
  RvCsRpt,
  RvDriverDataView,
  RvToolUtil;

var
  ReportExpertEnabled: boolean;

procedure RaveRegister;
begin { RaveRegister }
  RegisterRaveProjectEditor(TReportExpertProjectEditor);
end;  { RaveRegister }

{ TFormReportExpert }

procedure TFormReportExpert.FormCreate(Sender: TObject);
begin
  FormTrans(self);
  PrepareToolWin(self);
end;

procedure TFormReportExpert.FormDestroy(Sender: TObject);
begin
  SaveToolWin(self);
end;

procedure TFormReportExpert.FormShow(Sender: TObject);
begin
  RestoreToolWin(self);
end;

{ TReportExpertProjectEditor }

constructor TReportExpertProjectEditor.Create;
begin { Create }
  inherited Create;
{ Read in registry info }
  with RaveSettings.LockSection('ReportExpert') do try
    ReportExpertEnabled := ReadBool('Enabled',true);
  finally
    UnlockSection;
  end; { with }
end;  { Create }

destructor TReportExpertProjectEditor.Destroy;
begin { Destroy }
  inherited Destroy;
end;  { Destroy }

procedure TReportExpertProjectEditor.AddToMenu(AddMenuItem: TAddMenuItemProc);
begin { AddToMenu }
  AddMenuItem(self,'Report Expert',1,0);
end;  { AddToMenu }

procedure TReportExpertProjectEditor.RunFromMenu(ID: integer);
var
  S1: string;
begin { RunFromMenu }
{ Prompt for enabled status }
  if ReportExpertEnabled then begin
    S1 := 'The Report Expert is currently enabled.  Do you want to disable it?';
  end else begin
    S1 := 'The Report Expert is currently disabled.  Do you want to enable it?';
  end; { else }
  if MessageDlg(S1,mtConfirmation,[mbYes,mbNo,mbCancel],0) = mrYes then begin
    ReportExpertEnabled := not ReportExpertEnabled;

  { Write out new state }
    with RaveSettings.LockSection('ReportExpert') do try
      WriteBool('Enabled',ReportExpertEnabled);
    finally
      UnlockSection;
    end; { with }
  end; { if }
end;  { RunFromMenu }

function TReportExpertProjectEditor.BeforeReportPrint: boolean;
begin
  if ReportExpertEnabled then begin
    Result := AnalyzeReport(ProjectManager.ActiveReport);
  end else begin
    Result := true;
  end; { else }
end;

function TReportExpertProjectEditor.AnalyzeReport(Report: TRaveReport): boolean;
begin
  Result := true;

// Build project item / data object list
  TRaveProjectManager(Report.Project).SetIncluded(Report);

// Iterate through all components
  FormReportExpert := TFormReportExpert.Create(Application);
  try
    FormReportExpert.ItemList.Clear;
    ProcessComps(Report.Project);
    if FormReportExpert.ItemList.Items.Count > 0 then begin
      Result := FormReportExpert.ShowModal = mrOK;
    end; { if }
  finally
    FreeAndNil(FormReportExpert);
  end; { with }
end;

procedure TReportExpertProjectEditor.AddItem(Comp: TRaveComponent; MsgType: TMessageType; ItemText: string);
var
  S1: string;
begin
  case MsgType of
    mtHint: begin
      S1 := '[Hint] ' + Comp.Name + ': ' + ItemText + ' - ' + DateTimeToStr(Now);
    end;
    mtWarning: begin
      S1 := '[Warning] ' + Comp.Name + ': ' + ItemText + ' - ' + DateTimeToStr(Now);
    end;
    mtError: begin
      S1 := '[Error] ' + Comp.Name + ': ' + ItemText + ' - ' + DateTimeToStr(Now);
    end;
  end; { case }
  FormReportExpert.ItemList.Items.Add(S1);
end;

procedure TReportExpertProjectEditor.AddHint(Comp: TRaveComponent; HintIndex: integer);
begin
  case HintIndex of
    10001: begin
      AddItem(Comp,mtHint,'GroupKey or GroupDataView properties are set on a non-group band.');
    end;
  end; { case }
end;

procedure TReportExpertProjectEditor.AddWarning(Comp: TRaveComponent; WarningIndex: integer);
begin
  case WarningIndex of
    20001: begin
      AddItem(Comp,mtWarning,'ControllerBand property not assigned.  Band will only print once as a master level band.');
    end;
    20002: begin
      AddItem(Comp,mtWarning,'BandStyles are set to header or footer values.  Databands should not be headers or footers.');
    end;
  end; { case }
end;

procedure TReportExpertProjectEditor.AddError(Comp: TRaveComponent; ErrorIndex: integer);
begin
  case ErrorIndex of
    30001: begin
      AddItem(Comp,mtError,'DataView property is not assigned.  Databand will not print.');
    end;
    30002: begin
      AddItem(Comp,mtError,'Database property is not assigned.  DriverDataView will not connect.');
    end;
  end; { case }
end;

procedure TReportExpertProjectEditor.ProcessComps(Item: TRaveProjectItem);
var
  I1: integer;
  Comp: TRaveComponent;
  DataBand: TRaveDataBand;
  Band: TRaveBand;
begin
  if Item.Included then begin
    for I1 := 0 to Item.ComponentCount - 1 do begin
      Comp := TRaveComponent(Item.Components[I1]);

      if Comp is TRaveDataBand then begin
        DataBand := TRaveDataBand(Comp);

        if DataBand.DataView = nil then begin
          AddError(Comp,30001);
        end; { if }

        if (DataBand.BandStyle.PrintLoc <> []) and (DataBand.BandStyle.PrintLoc <> [plDetail]) then begin
          AddWarning(Comp,20002);
        end; { if }
      end; { if }

      if Comp is TRaveBand then begin
        Band := TRaveBand(Comp);

        if (Band.ControllerBand = nil) and (Band.BandStyle.PrintLoc <> []) then begin
          AddWarning(Comp,20001);
        end; { if }

        if ((Band.GroupKey <> '') or (Band.GroupDataView <> nil)) and
         not ((plGroupHeader in Band.BandStyle.PrintLoc) or (plGroupFooter in Band.BandStyle.PrintLoc)) then begin
          AddHint(Comp,10001);
        end; { if }
      end; { if }

      if Comp is TRaveDriverDataView then begin
        if TRaveDriverDataView(Comp).Database = nil then begin
          AddError(Comp,30002);
        end; { if }
      end; { if }


    // Call and children projectitems
      if Comp is TRaveProjectItem then begin
        ProcessComps(TRaveProjectItem(Comp));
      end; { if }
    end; { for }
  end; { if }
end;

initialization
  RegisterProc({Trans-}'RVCL',RaveRegister);
end.