{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RaveReg;

{$I RpVer.inc}

{$IFNDEF LEVEL6}
  {$DEFINE MSWindows}
{$ENDIF}

interface

uses
{$IFDEF LEVEL6}
  DesignIntf,
  DesignEditors,
{$ELSE}
  DsgnIntf,
{$ENDIF}
{$IFDEF LEVEL6}
  Qt,
  QControls,
  QRpDefine,
  QRpDevice,
  QRpFont,
  QRvDefine,
  QRpBase,
  QRpFiler,
  QRpSystem,
  QRpMemo,
  QRpBars,
  QRpDBUtil,
  QRpCon,
  QRpConDS,
  QRpConBDE,
  QRpRave,
  QRpRPTF,
  QRpRender,
  QRpRenderPreview,
  QRpRenderPrinter,
  QRpRenderHTML,
  QRpRenderPDF,
  QRpRenderRTF,
  QRpRenderText,
  QRpMBCS,
{$ENDIF}
  Windows,
  DB,
  Forms,
  Controls,
  Dialogs,
  SysUtils,
  TypInfo,
  Registry,
  ShellAPI,
  RpDefine,
  RpBase,
  RpFiler,
  RpSystem,
  RpMemo,
  RpBars,
  RpDBUtil,
  RvDefine,
  RvUtil,
  RpCon,
  RpConDS,
  RpConBDE,
  RpRave,
  RpExeFrm,
  RpRender,
  RpRenderPreview,
  RpRenderPrinter,
  RpRenderPDF,
  RpRenderHTML,
  RpRenderRTF,
  RpRenderText,
// Archived units

  RpAddFrm,
  RpCEdFrm,
  RpDBTabl,
  RpHTFilr,
  RpLabel,
  Rprinter,
  RpRTFilr,
  RpSecFrm,
  RpSect,
  RpShell,
  RpTable,
  RpTXFilr,
  RpCanvas,
  RpFPrint,
  Rpreview,
  RpRender_HTML,
  RpRender_PDF,
  Classes,
  IniFiles;

type
  TRaveProjectEditor = class(TComponentEditor)
    procedure ExecuteVerb(Index: integer); override;
    function GetVerb(Index: integer): string; override;
    function GetVerbCount: integer; override;
  end; { TRaveProjectEditor }

  TReportEngineProperty = class(TComponentProperty)
    function GetAttributes: TPropertyAttributes; override;
    procedure GetValues(Proc: TGetStrProc); override;
  end; { TReportEngineProperty }

  TRaveProjectFileProperty = class(TStringProperty)
  public
    function GetAttributes: TPropertyAttributes; override;
    procedure Edit; override;
  end; { TRaveProjectFileProperty }

  TRaveStoreRAVProperty = class(TStringProperty)
  public
    function GetAttributes: TPropertyAttributes; override;
    function GetValue: string; override;
    procedure Edit; override;
  end; { TRaveStoreRAVProperty }

  TColumnEditor = class(TComponentEditor)
    procedure ExecuteVerb(Index: integer); override;
    function GetVerb(Index: integer): string; override;
    function GetVerbCount: integer; override;
  end; { TColumnEditor }

  TReportSectionProperty = class(TClassProperty)
    procedure Edit; override;
    function GetAttributes: TPropertyAttributes; override;
  end; { TReportSectionProperty }

  TReportShellProperty = class(TComponentProperty)
    function GetAttributes: TPropertyAttributes; override;
    procedure GetValues(Proc: TGetStrProc); override;
  end; { TReportShellProperty }

  TTablePrinterProperty = class(TComponentProperty)
    function GetAttributes: TPropertyAttributes; override;
    procedure GetValues(Proc: TGetStrProc); override;
  end; { TTablePrinterProperty }

  TFieldNameProperty = class(TStringProperty)
    function GetAttributes: TPropertyAttributes; override;
    procedure GetValues(Proc: TGetStrProc); override;
  end; { TFieldNameProperty }

  TMasterKeyProperty = class(TStringProperty)
    function GetAttributes: TPropertyAttributes; override;
    procedure GetValues(Proc: TGetStrProc); override;
  end; { TMasterKeyProperty }

  procedure Register;

implementation

{$R *.dcr}

{ TRaveProjectEditor }

procedure TRaveProjectEditor.ExecuteVerb(Index: integer);
var
  DesignerPath: string;
  TempStr: string[1];
  IniFile: TIniFile;
  IniFileName: string;
begin { ExecuteVerb }
{ Get designer DLL filename }
  with TRegistry.Create do try
    RootKey := HKEY_LOCAL_MACHINE;
    if OpenKey(RegRoot,true) then begin
      if ValueExists('DesignerPath') then begin
        DesignerPath := ReadString('DesignerPath') + '\RAVE.EXE' + #0;
      end else begin
        DesignerPath := 'C:\Nevrona\Rave\RAVE.EXE' + #0;
      end; { else }
    end; { if }
    With Component as TRvProject do begin
      If ProjectFile <> '' then begin
        IniFileName := DesignerPath;
        IniFileName := System.Copy(IniFileName, 1, Length(IniFileName) - 9);
        IniFileName := IniFileName + 'Rave.ini';
        IniFile := TIniFile.Create(IniFileName);
        try
          IniFile.WriteString('History', 'ActiveProject', ProjectFile);
        finally
          IniFile.Free;
        end; { tryf }
      end; { if }
    end;
  finally
    Free;
  end; { with }
  TempStr := #0;
  ShellExecute(0,nil,@DesignerPath[1],@TempStr[1],@TempStr[1],SW_SHOWMAXIMIZED);
end;  { ExecuteVerb }

function TRaveProjectEditor.GetVerb(Index: integer): string;
begin { GetVerb }
  Result := 'Rave Visual &Designer...';
end;  { GetVerb }

function TRaveProjectEditor.GetVerbCount: integer;
begin { GetVerbCount }
  Result := 1;
end;  { GetVerbCount }

{ TRaveProjectFileProperty }

function TRaveProjectFileProperty.GetAttributes: TPropertyAttributes;
begin { GetAttributes }
  Result := [paDialog, paMultiSelect];
end;  { GetAttributes }

procedure TRaveProjectFileProperty.Edit;
begin { Edit }
  with TOpenDialog.Create(Application) do try
    FileName := GetValue;
    Title := 'Select Rave Project File';
    Filter := 'Rave Project (*.rav)|*.rav';
    if Execute then begin
      SetValue(FileName);
(*!!PORT
      ShowOptionalDialog(nil,'WARNING',
      'The full path has been saved with the ProjectFile property.'#13 +
      'This may cause problems if your application''s project file'#13 +
      'is not deployed to this exact same location.','WarningProjectFileName');
*)
    end; { if }
  finally
    Free;
  end; { with }
end;  { Edit }

{ TRaveStoreRAVProperty }

function TRaveStoreRAVProperty.GetAttributes: TPropertyAttributes;
begin { GetAttributes }
  Result := [paDialog,paReadOnly];
end;  { GetAttributes }

function TRaveStoreRAVProperty.GetValue: string;
var
  BlobDateTime: TDateTime;
begin { GetValue }
  if GetComponent(0) is RpRave.TRvProject then begin
    BlobDateTime := RpRave.TRvProject(GetComponent(0)).RaveBlobDateTime;
{$IFDEF Level6}
  end else if GetComponent(0) is QRpRave.TRvProject then begin
    BlobDateTime := QRpRave.TRvProject(GetComponent(0)).RaveBlobDateTime;
{$ENDIF}
  end else begin
    BlobDateTime := 0;
  end; { else }
  if BlobDateTime < 1.0 then begin { Check for 0 value w/ buffer }
    Result := '(none)';
  end else begin
    Result := FormatDateTime('ddddd t',BlobDateTime);
  end; { else }
end;  { GetValue }

procedure TRaveStoreRAVProperty.Edit;
begin { Edit }
  if GetComponent(0) is RpRave.TRvProject then begin
    with TExeForm.Create(Application) do try
      RaveProject := RpRave.TRvProject(GetComponent(0));
      ShowModal;
      if RaveProjectModified then begin
        Modified;
      end; { if }
    finally
      Free;
    end; { with }
{$IFDEF Level6}
  end else if GetComponent(0) is QRpRave.TRvProject then begin
  // Show CLX version of EXE form
{$ENDIF}
  end; { else }
end;  { Edit }

{ TReportEngineProperty }

function TReportEngineProperty.GetAttributes: TPropertyAttributes;
begin { GetAttributes }
  Result := [paValueList,paSortList];
end;  { GetAttributes }

procedure TReportEngineProperty.GetValues(Proc: TGetStrProc);
var
  I1: integer;
  Component: TComponent;
begin { GetValues }
  for I1 := 0 to Designer.GetRoot.ComponentCount - 1 do begin
    Component := Designer.GetRoot.Components[I1];
//!!PORT!! Need to handle the archived component types in here - Class function?
    if (Component is RpSystem.TRvSystem) or (Component is RpFiler.TRvNDRWriter){$IFDEF LEVEL6} or
     (Component is QRpSystem.TRvSystem) or (Component is QRpFiler.TRvNDRWriter){$ENDIF} then begin
      Proc(Component.Name);
    end; { if }
  end; { for }
end;  { GetValues }


{ TColumnEditor }

procedure TColumnEditor.ExecuteVerb(Index: integer);
begin { ExecuteVerb }
  ColumnEditor(Designer,Component as TTablePrinter);
end;  { ExecuteVerb }

function TColumnEditor.GetVerb(Index: integer): string;
begin { GetVerb }
  Result := 'Table &Editor...';
end;  { GetVerb }

function TColumnEditor.GetVerbCount: integer;
begin { GetVerbCount }
  Result := 1;
end;  { GetVerbCount }

{ TReportSectionProperty }

procedure TReportSectionProperty.Edit;
var
  ReportSectionForm: TSectionEditForm;
  I1: integer;
begin { Edit }
  ReportSectionForm := TSectionEditForm.Create(Application);
  with ReportSectionForm do begin
    try
      RS.Assign(TReportSection(GetOrdValue));
      Caption := 'Section Editor (' + TComponent(GetComponent(0)).Name + '.' +
        GetName + ')';
      ShowModal;
      if SaveSettings then begin
        for I1 := 0 to PropCount - 1 do begin
          TReportSection(GetOrdValueAt(I1)).Assign(RS);
        end; { for }
        Modified;
      end; { if }
    finally
      Free;
    end; { tryf }
  end; { with }
end;  { Edit }

function TReportSectionProperty.GetAttributes: TPropertyAttributes;
begin { GetAttributes }
  Result := [paMultiSelect,paSubProperties,paDialog];
end;  { GetAttributes }

{ TReportShellProperty }

function TReportShellProperty.GetAttributes: TPropertyAttributes;
begin { GetAttributes }
  Result := [paValueList,paSortList];
end;  { GetAttributes }

procedure TReportShellProperty.GetValues(Proc: TGetStrProc);
var
  I1: integer;
  Component: TComponent;
begin { GetValues }
  for I1 := 0 to Designer.GetRoot.ComponentCount - 1 do begin
    Component := Designer.GetRoot.Components[I1];
    if (Component is TDetailShell) and not (Component is TReportShell) and
      (Component <> GetComponent(0)) then begin
      Proc(Component.Name);
    end; { if }
  end; { for }
end;  { GetValues }

{ TTablePrinterProperty }

function TTablePrinterProperty.GetAttributes: TPropertyAttributes;
begin { GetAttributes }
  Result := [paValueList,paSortList];
end;  { GetAttributes }

procedure TTablePrinterProperty.GetValues(Proc: TGetStrProc);
var
  I1: integer;
  Component: TComponent;
begin { GetValues }
  for I1 := 0 to Designer.GetRoot.ComponentCount - 1 do begin
    Component := Designer.GetRoot.Components[I1];
    if (Component is TTablePrinter) and (Component <> GetComponent(0)) then begin
      Proc(Component.Name);
    end; { if }
  end; { for }
end;  { GetValues }

{ TFieldNameProperty }

procedure LoadFieldNames(Proc: TGetStrProc; DataSet: TDataSet);
var
  I1: integer;
  FieldNames: TStrings;
begin { LoadFieldNames }
  if Assigned(DataSet) then begin
    FieldNames := TStringList.Create;
    try
      DataSet.GetFieldNames(FieldNames);
      for I1 := 1 to FieldNames.Count do begin
        Proc(FieldNames[I1 - 1]);
      end; { for }
    finally
      FieldNames.Free;
    end; { tryf }
  end; { if }
end;  { LoadFieldNames }

function TFieldNameProperty.GetAttributes: TPropertyAttributes;
begin { GetAttributes }
  Result := [paValueList,paSortList];
end;  { GetAttributes }

procedure TFieldNameProperty.GetValues(Proc: TGetStrProc);
begin { GetValues }
  if GetComponent(0) is TDBTableColumn then begin
    if Pos('Lookup',GetName) <> 0 then begin
      LoadFieldNames(Proc,(GetComponent(0) as TDBTableColumn).LookupDataSet);
    end else begin
      LoadFieldNames(Proc,((GetComponent(0) as TDBTableColumn).TablePrinter as
       TDBTablePrinter).DataSet);
    end; { else }
  end else if GetComponent(0) is TDBTablePrinter then begin
    LoadFieldNames(Proc,(GetComponent(0) as TDBTablePrinter).DataSet);
  end; { else }
end;  { GetValues }

{ TMasterKeyProperty }

function TMasterKeyProperty.GetAttributes: TPropertyAttributes;
begin { GetAttributes }
  Result := [paValueList,paSortList];
end;  { GetAttributes }

procedure TMasterKeyProperty.GetValues(Proc: TGetStrProc);
var
  I1: integer;
  Component: TComponent;
begin { GetValues }
  for I1 := 0 to Designer.GetRoot.ComponentCount - 1 do begin
    Component := Designer.GetRoot.Components[I1];
    if (Component is TDBTablePrinter) then begin
      with Component as TDBTablePrinter do begin
        if DetailTablePrinter = GetComponent(0) then begin
          LoadFieldNames(Proc,DataSet);
          Exit;
        end; { if }
      end; { if }
    end; { if }
  end; { for }
end;  { GetValues }


procedure Register;
begin { Register }
  RegisterPropertyEditor(TypeInfo(boolean),RpRave.TRvProject,'StoreRAV',TRaveStoreRAVProperty);
  RegisterPropertyEditor(TypeInfo(string),RpRave.TRvProject,'ProjectFile',TRaveProjectFileProperty);
  RegisterPropertyEditor(TypeInfo(RpDefine.TRPComponent),RpRave.TRvProject,'Engine',TReportEngineProperty);
  RegisterComponentEditor(RpRave.TRvProject,TRaveProjectEditor);

  RegisterPropertyEditor(TypeInfo(TReportSection),nil,'',TReportSectionProperty);
  RegisterPropertyEditor(TypeInfo(TDetailShell),nil,'',TReportShellProperty);
  RegisterPropertyEditor(TypeInfo(TBaseShell),TTablePrinter,'DetailTablePrinter',TTablePrinterProperty);
  RegisterPropertyEditor(TypeInfo(TRPComponent),TBaseShell,'Engine',TReportEngineProperty);
  RegisterPropertyEditor(TypeInfo(TFieldName),TDBTableColumn,'FieldName',TFieldNameProperty);
  RegisterPropertyEditor(TypeInfo(TFieldName),TDBTableColumn,'LookupDisplay',TFieldNameProperty);
  RegisterPropertyEditor(TypeInfo(TFieldName),TDBTableColumn,'LookupKey',TFieldNameProperty);
  RegisterPropertyEditor(TypeInfo(TFieldName),TDBTablePrinter,'DetailKey',TFieldNameProperty);
  RegisterPropertyEditor(TypeInfo(TFieldName),TDBTablePrinter,'MasterKey',TMasterKeyProperty);

  RegisterClass(TTableColumn);
  RegisterClass(TDBTableColumn);
  RegisterClass(TTableSection);
  RegisterNoIcon([TTableColumn,TDBTableColumn,TTableSection]);
  RegisterComponentEditor(TTablePrinter,TColumnEditor);

  RegisterClassAlias(RpRave.TRvProject,'TRaveProject');
  RegisterClassAlias(RpSystem.TRvSystem,'TReportSystem');
  RegisterClassAlias(RpFiler.TRvNDRWriter,'TReportFiler');

  RegisterClassAlias(RpCon.TRvCustomConnection,'TRPCustomConnection');
  RegisterClassAlias(RpConDS.TRvDataSetConnection,'TRPDataSetConnection');
  RegisterClassAlias(RpConBDE.TRvTableConnection,'TRPTableConnection');
  RegisterClassAlias(RpConBDE.TRvQueryConnection,'TRPQueryConnection');

  RegisterClassAlias(RpRenderPDF.TRvRenderPDF,'TRPRenderPDF');
  RegisterClassAlias(RpRenderHTML.TRvRenderHTML,'TRPRenderHTML');

  RegisterComponents('Rave',[
   RpRave.TRvProject,
   RpSystem.TRvSystem,
   RpFiler.TRvNDRWriter,

   RpCon.TRvCustomConnection,
   RpConDS.TRvDataSetConnection,
   RpConBDE.TRvTableConnection,
   RpConBDE.TRvQueryConnection,

   RpRenderPreview.TRvRenderPreview,
   RpRenderPrinter.TRvRenderPrinter,
   RpRenderPDF.TRvRenderPDF,
   RpRenderHTML.TRvRenderHTML,
   RpRenderRTF.TRvRenderRTF,
   RpRenderText.TRvRenderText

,
   TReportPrinter,
   TFilePreview,
   TFilePrinter,
   TDetailShell,
   TMasterShell,
   TReportShell,
   TLabelShell,
   TDBTablePrinter,
   TTablePrinter,
   TRTFFiler,
   TRPHTMLFiler,
   TTextFiler
  ]);

{$IFDEF LEVEL6}
  RegisterPropertyEditor(TypeInfo(boolean),QRpRave.TRvProject,'StoreRAV',TRaveStoreRAVProperty);
  RegisterPropertyEditor(TypeInfo(string),QRpRave.TRvProject,'ProjectFile',TRaveProjectFileProperty);
  RegisterPropertyEditor(TypeInfo(QRpDefine.TRPComponent),QRpRave.TRvProject,'Engine',TReportEngineProperty);
  RegisterComponentEditor(QRpRave.TRvProject,TRaveProjectEditor);

  GroupDescendentsWith(QRpDefine.TRpComponent,TWidgetControl);
  GroupDescendentsWith(RpDefine.TRpComponent,TWinControl);

  RegisterComponents('Rave',[
   QRpRave.TRvProject,
   QRpSystem.TRvSystem,
   QRpFiler.TRvNDRWriter,

   QRpCon.TRvCustomConnection,
   QRpConDS.TRvDataSetConnection,
   QRpConBDE.TRvTableConnection,
   QRpConBDE.TRvQueryConnection,

   QRpRenderPreview.TRvRenderPreview,
   QRpRenderPrinter.TRvRenderPrinter,
   QRpRenderPDF.TRvRenderPDF,
   QRpRenderHTML.TRvRenderHTML,
   QRpRenderRTF.TRvRenderRTF,
   QRpRenderText.TRvRenderText
  ]);

  GroupDescendentsWith(QRpSystem.TRvSystem,TWidgetControl);
  GroupDescendentsWith(RpSystem.TRvSystem,TWinControl);
{$ENDIF}
end;  { Register }

end.