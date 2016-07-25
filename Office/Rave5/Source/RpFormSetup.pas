{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RpFormSetup;

interface

uses
  Windows, Messages, Graphics, Controls, Forms, StdCtrls, Buttons, Dialogs, ComCtrls,
  SysUtils, Classes, RpDevice, RpDefine, RpSystem, RpRenderPrinter;

type
  TRPSetupForm = class(TForm)
    DestGB: TGroupBox;
    PrinterRB: TRadioButton;
    PreviewRB: TRadioButton;
    FileRB: TRadioButton;
    editFileName: TEdit;
    bbtnOK: TButton;
    CancelBB: TButton;
    SetupBB: TButton;
    RangeGB: TGroupBox;
    FileNameSB: TSpeedButton;
    dlogSave: TSaveDialog;
    AllRB: TRadioButton;
    SelectionRB: TRadioButton;
    PagesRB: TRadioButton;
    FromLabel: TLabel;
    FromED: TEdit;
    ToLabel: TLabel;
    ToED: TEdit;
    SelectionLabel: TLabel;
    SelectionED: TEdit;
    GroupBox1: TGroupBox;
    PrinterLabel: TLabel;
    GroupBox2: TGroupBox;
    CopiesED: TEdit;
    CopiesLabel: TLabel;
    CollateCK: TCheckBox;
    DuplexCK: TCheckBox;
    PageControl1: TPageControl;
    TabSheet1: TTabSheet;
    TabSheet2: TTabSheet;
    cboxFormat: TComboBox;
    Label1: TLabel;
    procedure SetupBBClick(Sender: TObject);
    procedure FileNameSBClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure PagesRBClick(Sender: TObject);
    procedure SelectionRBClick(Sender: TObject);
    procedure AllRBClick(Sender: TObject);
    procedure PrinterRBClick(Sender: TObject);
    procedure FileRBClick(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure bbtnOKClick(Sender: TObject);
    procedure editFileNameChange(Sender: TObject);
    procedure FormKeyPress(Sender: TObject; var Key: Char);
    procedure SelectionEDKeyPress(Sender: TObject; var Key: Char);
    procedure cboxFormatChange(Sender: TObject);
  private
    procedure UpdateForm;
    procedure GetRenderList;
  public
    ReportSystem: TRvSystem;
    FilePrinter: TRvRenderPrinter;
    PreviewSetup: boolean;
  end;

var
  RPSetupForm: TRPSetupForm;

implementation

uses
  RpRender;

{$R *.dfm}

procedure TRPSetupForm.UpdateForm;

begin { UpdateForm }
  if (RPDev = nil) or (RPDev.Printers.Count = 0) then begin
    PrinterLabel.Caption := Trans('No printers installed!');
  end else begin
    RPDev.State := dsIC;
    if RPDev.Output = '' then begin
      PrinterLabel.Caption := RPDev.Device;
    end else begin
      PrinterLabel.Caption := Trans(Format({Trans+}'%s on %s',[RPDev.Device,RPDev.Output]));
    end; { else }
  end; { if }
end;  { UpdateForm }

procedure TRPSetupForm.SetupBBClick(Sender: TObject);
begin
  if RPDev <> nil then begin
    RPDev.PrinterSetupDialog;
    UpdateForm;
  end; { if }
end;

procedure TRPSetupForm.FileNameSBClick(Sender: TObject);
begin
  if dlogSave.Execute then begin
    editFileName.Text := dlogSave.FileName;
    cboxFormat.ItemIndex := dlogSave.FilterIndex - 1;
  end; { if }
end;

procedure TRPSetupForm.FormCreate(Sender: TObject);
begin
  FromLabel.Hide;
  FromED.Hide;
  ToLabel.Hide;
  ToED.Hide;
  PageControl1.ActivePage := nil;
  SelectionLabel.Enabled := false;
  SelectionED.Enabled := false;
  editFileName.Enabled := false;
  FileNameSB.Enabled := false;
end;

procedure TRPSetupForm.PagesRBClick(Sender: TObject);
begin
  with Sender as TRadioButton do begin
    if Checked then begin
      SelectionLabel.Hide;
      SelectionED.Hide;
      FromLabel.Enabled := true;
      FromLabel.Show;
      FromED.Enabled := true;
      FromED.Show;
      ToLabel.Enabled := true;
      ToLabel.Show;
      ToED.Enabled := true;
      ToED.Show;
    end; { if }
  end; { with }
end;

procedure TRPSetupForm.SelectionRBClick(Sender: TObject);
begin
  with Sender as TRadioButton do begin
    if Checked then begin
      FromLabel.Hide;
      FromED.Hide;
      ToLabel.Hide;
      ToED.Hide;
      SelectionLabel.Enabled := true;
      SelectionLabel.Show;
      SelectionED.Enabled := true;
      SelectionED.Show;
    end; { if }
  end; { with }
end;

procedure TRPSetupForm.AllRBClick(Sender: TObject);
begin
  SelectionLabel.Enabled := false;
  SelectionED.Enabled := false;
  FromLabel.Enabled := false;
  FromED.Enabled := false;
  ToLabel.Enabled := false;
  ToED.Enabled := false;
end;

procedure TRPSetupForm.PrinterRBClick(Sender: TObject);
begin
  editFileName.Enabled := false;
  FileNameSB.Enabled := false;
  cboxFormat.Enabled := false;
  bbtnOk.Enabled := true;
  SetupBB.enabled := true;
//  GroupBox2.Enabled := Sender = PrinterRB;
  CopiesED.Enabled := Sender = PrinterRB;
  CollateCK.Enabled := Sender = PrinterRB;
  DuplexCK.Enabled := Sender = PrinterRB;
end;

procedure TRPSetupForm.FileRBClick(Sender: TObject);
begin
  editFileName.Enabled := true;
  FileNameSB.Enabled := true;
  cboxFormat.Enabled := true;
//  GroupBox2.Enabled := false;
  SetupBB.enabled := false;
  bbtnOK.Enabled := Trim(editFileName.Text) <> '';
end;

procedure TRPSetupForm.FormShow(Sender: TObject);
begin
  GetRenderList;
{ Enable/Disable items on setup form }
  with ReportSystem do begin
    UpdateForm;
    if PreviewSetup then begin
      PageControl1.ActivePage := TabSheet2;
    { Init Page range variables here for FilePrinter.ExecuteCustom }
      FromED.Text := '1';
      ToED.Text := IntToStr(FilePrinter.Pages);
      SelectionED.Text := {Trans-}'1-' + IntToStr(FilePrinter.Pages);
    end else begin { Initial setup, ask for destination }
      PageControl1.ActivePage := TabSheet1;
      if cboxFormat.ItemIndex < 0 then begin
        cboxFormat.ItemIndex := 0;
      end;
      if (RPDev = nil) or RPDev.InvalidPrinter then begin
        PrinterRB.Enabled := false;
        FileRB.Enabled := false;
        if not (ssAllowDestPreview in SystemSetups) then begin
          PreviewRB.Enabled := false;
        end else begin
          PreviewRB.Checked := true;
        end; { else }
      end else begin
        if not (ssAllowDestPrinter in SystemSetups) then begin
          PrinterRB.Enabled := false;
        end; { if }
        if not (ssAllowDestPreview in SystemSetups) then begin
          PreviewRB.Enabled := false;
        end; { if }
        if not (ssAllowDestFile in SystemSetups) then begin
          FileRB.Enabled := false;
        end; { if }
        case DefaultDest of
          rdPrinter: begin
            PrinterRB.Checked := true;
          end;
          rdPreview: begin
            PreviewRB.Checked := true;
          end;
          rdFile: begin
            FileRB.Checked := true;
          end;
        end; { case }
      end; { else }
    end; { else }
    if (RPDev = nil) or RPDev.InvalidPrinter then begin
      CollateCK.Enabled := false;
      CopiesED.Enabled := false;
      CopiesLabel.Enabled := false;
      CopiesED.Text := IntToStr(SystemPrinter.Copies);
      DuplexCK.Enabled := false;
      SetupBB.Enabled := false;
    end else begin
      if not (ssAllowCollate in SystemSetups) then begin
        CollateCK.Enabled := false;
      end; { if }
      if not (ssAllowCopies in SystemSetups) then begin
        CopiesED.Enabled := false;
        CopiesLabel.Enabled := false;
      end; { if }
      CopiesED.Text := IntToStr(SystemPrinter.Copies);
      if not (ssAllowDuplex in SystemSetups) then begin
        DuplexCK.Enabled := false;
      end; { if }
      if not (ssAllowPrinterSetup in SystemSetups) then begin
        SetupBB.Enabled := false;
      end; { if }
    end; { else }
  end; { with }
end;

procedure TRPSetupForm.bbtnOKClick(Sender: TObject);
var
  I1: integer;
  ErrCode: integer;
  DefExtension: string;
begin
  with ReportSystem do begin
  { Gather information that was changed in setup window }
    if PreviewSetup then begin
    { Put code here to retrieve page range info }
      FilePrinter.FirstPage := 1;
      FilePrinter.LastPage := 9999;
      FilePrinter.Selection := '';
      if SelectionRB.Checked then begin
        FilePrinter.Selection := SelectionED.Text;
      end else if PagesRB.Checked then begin
        FilePrinter.FirstPage := StrToInt(FromED.Text);
        FilePrinter.LastPage := StrToInt(ToED.Text);
      end; { else }

      if ssAllowCopies in SystemSetups then begin
        Val(CopiesED.Text,I1,ErrCode);
        if ErrCode = 0 then begin
          FilePrinter.Copies := I1;
        end; { if }
      end; { if }
      if ssAllowCollate in SystemSetups then begin
        FilePrinter.Collate := CollateCK.Checked;
      end; { if }
      if (ssAllowDuplex in SystemSetups) then begin
        if DuplexCK.Checked then begin
          if FilePrinter.Duplex = dupSimplex then begin
            FilePrinter.Duplex := dupVertical;
          end; { if }
        end else begin
          FilePrinter.Duplex := dupSimplex;
        end; { else }
      end; { if }
    end else begin
      if PrinterRB.Checked then begin
        ReportDest := rdPrinter;
        OutputFileName := '';
      end else if PreviewRB.Checked then begin
        ReportDest := rdPreview;
        OutputFileName := '';
      end else begin
        ReportDest := rdFile;
        OutputFileName := Trim(editFileName.Text);

        if cboxFormat.ItemIndex = 0 then begin // Do NDR
          if Pos('.',OutputFileName) < 1 then begin
            OutputFileName := OutputFileName + {Trans-}'.ndr';
          end; { if }
          ReportSystem.DoNativeOutput := false;
          ReportSystem.RenderObject := nil;
        end else if cboxFormat.ItemIndex = 1 then begin // Do Native Output PRN
          if Pos('.',OutputFileName) < 1 then begin
            OutputFileName := OutputFileName + {Trans-}'.prn';
          end; { if }
          ReportSystem.DoNativeOutput := true;
          ReportSystem.RenderObject := nil;
        end else begin // Do Renderer
          ReportSystem.DoNativeOutput := false;
          ReportSystem.RenderObject :=
           TRPRender(cboxFormat.Items.Objects[cboxFormat.ItemIndex]);
          if Pos('.',OutputFileName) < 1 then begin
            with ReportSystem.RenderObject do begin
              DefExtension := Copy(FileExtension, 2, Length(FileExtension) - 1);
            end; { with }
            OutputFileName := OutputFileName + DefExtension;
          end; { if }
        end; { else }
      end; { else }
    end; { else }

    if ssAllowCopies in SystemSetups then begin
      Val(CopiesED.Text,I1,ErrCode);
      if ErrCode = 0 then begin
        SystemPrinter.Copies := I1;
      end; { if }
    end; { if }
    if ssAllowCollate in SystemSetups then begin
      SystemPrinter.Collate := CollateCK.Checked;
    end; { if }
    if (ssAllowDuplex in SystemSetups) then begin
      if DuplexCK.Checked then begin
        if SystemPrinter.Duplex = dupSimplex then begin
          SystemPrinter.Duplex := dupVertical;
        end; { if }
      end else begin
        SystemPrinter.Duplex := dupSimplex;
      end; { else }
    end; { if }
  end; { with }
end;

procedure TRPSetupForm.GetRenderList;
const
  DefOutputStr: array[0..1] of string = ({Trans+}'Rave Snapshot File (NDR)',
                                         {Trans+}'Native Printer Output (PRN)');
begin
  cboxFormat.Items.Clear;
  if Assigned(RpRender.RenderList) then begin
    RpRender.GetRenderList(cboxFormat.Items);
  end; { if }
  cboxFormat.Items.Insert(0,Trans(DefOutputStr[0]));
  cboxFormat.Items.Insert(1,Trans(DefOutputStr[1]));

  dlogSave.Filter := DefOutputStr[0] + {Trans-}'|*.NDR|' + DefOutputStr[1] + {Trans-}'|*.PRN';
  if RpRender.GetRenderFilter <> '' then begin
    dlogSave.Filter := dlogSave.Filter + '|' + RpRender.GetRenderFilter;
  end; { if }
end;

procedure TRPSetupForm.editFileNameChange(Sender: TObject);
begin
  bbtnOK.Enabled := Trim(editFileName.Text) <> '';
end;

procedure TRPSetupForm.FormKeyPress(Sender: TObject; var Key: Char);
var
  ValidKeys: string;
begin
  ValidKeys := {Trans-}'0123456789'#8;
  if (Sender <> editFileName) and (Pos(Key,ValidKeys) <= 0) then begin
    Key := #0;
  end;
end;

procedure TRPSetupForm.SelectionEDKeyPress(Sender: TObject; var Key: Char);
var
  ValidKeys: string;
begin
  ValidKeys := {Trans-}'0123456789-,'#8;
  if (Sender <> editFileName) and (Pos(Key,ValidKeys) <= 0) then begin
    Key := #0;
  end;
end;

procedure TRPSetupForm.cboxFormatChange(Sender: TObject);
begin
  dlogSave.FilterIndex := (Sender as TComboBox).ItemIndex + 1;
end;

end.
