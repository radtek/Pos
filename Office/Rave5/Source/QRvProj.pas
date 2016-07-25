{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit QRvProj;

interface

{$I RpVer.inc}

uses
  QComCtrls, QControls, QForms, QDialogs, QGraphics,
  TypInfo, SysUtils, Classes, QRvDefine, QRvUtil, QRvClass, QRvData,
  QRpDefine, QRpBase, QRpSystem, QRpDevice, QRvSecurity;

type
  TTagKind = (tkProject,tkExport,tkDataView,tkPage,tkReport,tkReportPage,tkDataObject);

  TImportConflictEvent = procedure(CurrentItem: TRaveProjectItem;
                                   var ImportName: string) of object;

  TRaveProjectManager = class;

  TRaveReport = class(TRaveProjectItem)
  protected
    FCategory: string;
    FLastActivePage: string;
    SaveOnPrint: TNotifyEvent;
    SaveBeforePrint: TNotifyEvent;
    SaveAfterPrint: TNotifyEvent;
    LoadedList: TList; { Loaded pages for this report }
    FixUpList: TStrings; { List of loaded modules that may not be ready until
                           after all report items are loaded }
    FFirstPage: TRavePage; { Page to begin execution for }
    FCopies: integer;
    FPageList: TRaveComponentList; { List of pages to print }
    FAlwaysGenerate: boolean;
    FCollate: TRavePrinterCollate;
    FDuplex: TRavePrinterDuplex;
    FPrinter: string;
    FResolution: TRavePrinterResolution;
    FMaxPages: integer; { Max number of pages to print }
    FSecurityControl: TRaveBaseSecurity;
    FExecProject: TRaveProjectManager;

    procedure SetPageList(Value: TRaveComponentList);
    function GetLoadedCount: integer;
    function GetLoaded(Index: integer): TRavePage;
    procedure RSPrint(Sender: TObject);
    procedure RSBeforePrint(Sender: TObject);
    procedure RSAfterPrint(Sender: TObject);
    procedure Changing(OldItem: TRaveComponent;
                       NewItem: TRaveComponent); override;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure SaveToStreamHelper(StreamHelper: TStreamHelper); override;
    procedure LoadFromStreamHelper(StreamHelper: TStreamHelper); override;
    procedure Open; override;
    procedure Close; override;
    function NewPage: TRavePage;
    procedure LoadPage(Page: TRavePage);
    procedure UnloadPage(Page: TRavePage);
    procedure ProcessLoaded;
    procedure Execute(Engine: TRPComponent);
    procedure InternalExecute(Engine: TRPComponent);

    property LoadedPageCount: integer read GetLoadedCount;
    property LoadedPage[Index: integer]: TRavePage read GetLoaded;
    property LastActivePage: string read FLastActivePage write FLastActivePage;
    property ExecProject: TRaveProjectManager read FExecProject;
  published
    property AlwaysGenerate: boolean read FAlwaysGenerate write FAlwaysGenerate default false;
    property Category: string read FCategory write FCategory;
    property Copies: integer read FCopies write FCopies default 0;
    property FirstPage: TRavePage read FFirstPage write FFirstPage default nil;
    property PageList: TRaveComponentList read FPageList write SetPageList;
    property Collate: TRavePrinterCollate read FCollate write FCollate
     default pcDefault;
    property Duplex: TRavePrinterDuplex read FDuplex write FDuplex
     default pdDefault;
    property MaxPages: integer read FMaxPages write FMaxPages default 0;
    property Parameters;
    property PIVars;
    property Printer: string read FPrinter write FPrinter;
    property Resolution: TRavePrinterResolution read FResolution
     write FResolution default prDefault;
    property SecurityControl: TRaveBaseSecurity read FSecurityControl write
     FSecurityControl;
  end; { TRaveReport }

  TRaveModuleManager = class
  protected
    ModuleList: TList;
    FProjectManager: TRaveProjectManager;
    //
    function ModuleIndex(ModuleName: string): integer;
    function LoadModule(ModuleName: string): integer;
  public
    constructor Create(AProjectManager: TRaveProjectManager);
    destructor Destroy; override;
    function GetModule(ModuleName: string): TRaveModule;
    function FindModule(ModuleName: string): TRaveModule;
    procedure LoadModules(StreamHelper: TStreamHelper);
    procedure SaveModules(StreamHelper: TStreamHelper);
    procedure ReleaseModule(AModule: TRaveModule);
    //
    property ProjectManager: TRaveProjectManager read FProjectManager;
  end; { TRaveModuleManager }

  TRaveProjectManager = class(TRaveProjectItem)
  protected
    ForwardRefList: TStringList;
    RenameRefList: TStringList;
    LastActiveReport: string;
    FReportList: TList; { List of all reports }
    FGlobalPageList: TList; { List of all global pages }
    FDataObjectList: TList; { List of all data objects }
    FActiveReport: TRaveReport;
    FSaved: boolean; { Has this project ever been saved }
    FFileName: string; { Filename of this project }
    Signature: string;
    FDataChanged: boolean;
    FPrinting: boolean;
    ExportList: TList;
    Importing: boolean;
    ImportReplace: boolean;
    FOnImportConflict: TImportConflictEvent;
    FVersion: integer;
    FCategories: TStrings;
    Params: TStringList;
    FUnits: TPrintUnits; { Units type }
    FUnitsFactor: TRaveFloat; { Units factor }
    FStreamParamValues: boolean; { Put parameter values in save stream? }
    IsLoading: boolean;
    FMasterProject: TRaveProjectManager;
    FNoDesigner: boolean;
    FSecurityControl: TRaveBaseSecurity;
    FAdminPassword: string;
    FBaseReport: TBaseReport;
    FModuleManager: TRaveModuleManager;

    procedure SetDepth(Control: TRaveComponent; Adjustment: integer); override;
    procedure SetCategories(Value: TStrings);
    procedure SetUnits(Value: TPrintUnits);
    procedure SetUnitsFactor(Value: double);
    procedure SetDataChanged(Value: boolean);
    procedure DefineProperties(Filer: TFiler); override;
    procedure ReadParamValues(Reader: TReader);
    procedure WriteParamValues(Writer: TWriter);
    procedure Changing(OldItem: TRaveComponent;
                       NewItem: TRaveComponent); override;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure SetIncluded(AReport: TRaveReport);
    procedure ReportToStream(AReport: TRaveReport;
                             AStream: TStream);
    function FindReport(const AName: string; const AIsFullName: boolean): TRaveReport;
    procedure AddForwardRef(AName: string; AOwner: TRaveComponent; AInstance: TPersistent;
     APropInfo: PPropInfo; AParam: longint);
    procedure AddRenameRef(OrigName: string; NewInstance: TRaveComponent);
    procedure FreeForwardRefs;
    procedure New;
    procedure RemoveForwardRefs(Target: TRaveComponent);
    procedure Save;
    procedure Load;
    procedure Unload;
    procedure Clear;
    procedure LoadFromStreamHelper(StreamHelper: TStreamHelper); override;
    procedure SaveToStreamHelper(StreamHelper: TStreamHelper); override;
    procedure LoadFromStream(Stream: TStream);
    procedure SaveToStream(Stream: TStream);
    procedure ExportProject(ExportFileName: string;
                            Items: TList);
    function ImportProject(ImportFileName: string;
                           AutoReplace: boolean): boolean;
    procedure DeactivateReport;
    procedure ActivateReport(Report: TRaveReport);
    function FindRaveComponent(Name: string;
                               DefRoot: TRaveComponent): TRaveComponent;
    function GetUniqueName(BaseName: string;
                           NameOwner: TRaveComponent;
                           UseCurrent: boolean): string;

    function NewReport: TRaveReport;
    function NewGlobalPage: TRavePage;
    function NewDataObject(DataObjectClass: TRaveDataObjectClass): TRaveDataObject;
    procedure DeleteItem(Item: TRaveProjectItem;
                         Notify: boolean);
    procedure SetParam(Param: string;
                       Value: string);
    function GetParam(Param: string): string;
    procedure ClearParams;
    procedure ClearChanged;
    function Compile: boolean;

    property ReportList: TList read FReportList;
    property GlobalPageList: TList read FGlobalPageList;
    property DataObjectList: TList read FDataObjectList;

    property ActiveReport: TRaveReport read FActiveReport;
    property Saved: boolean read FSaved write FSaved;
  { DataChanged will only change value when set to true, to clear call ClearChanged }
    property DataChanged: boolean read FDataChanged write SetDataChanged;
    property Printing: boolean read FPrinting;
    property Version: integer read FVersion;
    property OnImportConflict: TImportConflictEvent read FOnImportConflict write
     FOnImportConflict;
    property StreamParamValues: boolean read FStreamParamValues write FStreamParamValues;
    property FileName: string read FFileName write FFileName stored false;
    property MasterProject: TRaveProjectManager read FMasterProject write FMasterProject;
    property NoDesigner: boolean read FNoDesigner write FNoDesigner;
    property BaseReport: TBaseReport read FBaseReport write FBaseReport;
    property ModuleManager: TRaveModuleManager read FModuleManager;
  published
    property AdminPassword: string read FAdminPassword write FAdminPassword;
    property Categories: TStrings read FCategories write SetCategories;
    property Parameters;
    property PIVars;
    property SecurityControl: TRaveBaseSecurity read FSecurityControl write
     FSecurityControl;
    property Units: TPrintUnits read FUnits write SetUnits stored false;
    property UnitsFactor: TRaveFloat read FUnitsFactor write SetUnitsFactor;
  end; { TRaveProjectManager }

// Procs
  procedure RaveRegister;

var
  ProjectManager: TRaveProjectManager = nil;

implementation

uses
  {$IFNDEF RaveServer}QRvDirectDataView,{$ENDIF} QRvDataField, QRpMBCS;

{ Forward References Handling }

type
  TRaveForwardRef = class
  protected
    FParam: longint;
    FInstance: TPersistent;
    FPropInfo: PPropInfo;
  public
    constructor Create(AInstance: TPersistent;
                       APropInfo: PPropInfo;
                       AParam: longint);
    property Param: longint read FParam;
    property Instance: TPersistent read FInstance;
    property PropInfo: PPropInfo read FPropInfo;
  end; { TRaveForwardRef }

constructor TRaveForwardRef.Create(AInstance: TPersistent; APropInfo: PPropInfo; AParam: longint);
begin { Create }
  FInstance := AInstance;
  FPropInfo := APropInfo;
  FParam := AParam;
end;  { Create }

procedure TRaveProjectManager.AddForwardRef(AName: string; AOwner: TRaveComponent; AInstance: TPersistent;
 APropInfo: PPropInfo; AParam: longint);
begin { AddForwardRef }
  if not Assigned(ForwardRefList) then begin
    ForwardRefList := TStringList.Create;
    ForwardRefList.Sorted := true;
    ForwardRefList.Duplicates := dupAccept;
  end; { if }
  if Pos('.',AName) = 0 then begin
    AName := AOwner.Name + '.' + AName;
  end; { if }
  ForwardRefList.AddObject(AName,TRaveForwardRef.Create(AInstance,APropInfo,AParam));
end;  { AddForwardRef }

procedure TRaveProjectManager.AddRenameRef(OrigName: string; NewInstance: TRaveComponent);
begin { AddRenameRef }
  if not Assigned(RenameRefList) then begin
    RenameRefList := TStringList.Create;
    RenameRefList.Sorted := true;
    RenameRefList.Duplicates := dupIgnore;
  end; { if }
  RenameRefList.AddObject(NewInstance.Owner.Name + '.' + OrigName,NewInstance);
end;  { AddRenameRef }

procedure TRaveProjectManager.RemoveForwardRefs(Target: TRaveComponent);
var
  TargetName: string;
  Index: integer;
begin { RemoveForwardRefs }
  TargetName := Target.Owner.Name + '.' + Target.Name;
  if Assigned(ForwardRefList) and ForwardRefList.Find(TargetName,Index) then begin
    repeat
      with TRaveForwardRef(ForwardRefList.Objects[Index]) do begin
        if not Assigned(PropInfo) then begin
          TRaveComponentList(Instance).Items[Param] := Target;
        end else begin
          SetOrdProp(Instance,PropInfo,integer(Target));
        end; { else }
      end; { with }
      ForwardRefList.Objects[Index].Free;
      ForwardRefList.Delete(Index);
    until (Index >= ForwardRefList.Count) or
     (AnsiCompareText(ForwardRefList[Index],TargetName) <> 0);
  end; { if }
end;  { RemoveForwardRefs }

procedure RaveRegister;
begin { RaveRegister }
  RegisterRaveComponents('',[TRaveReport,TRaveProjectManager]);
  RegisterRaveModuleClasses({Trans-}'RvProj',[TRaveReport,TRaveProjectManager]);
end;  { RaveRegister }

type
  TRaveComponentAccess = class(TRaveComponent)
  end; { TRaveComponentAccess }

{ class TRaveReport }

constructor TRaveReport.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);
  LoadedList := TList.Create;
  FCopies := 0;
  FResolution := prDefault;
  FCollate := pcDefault;
  FDuplex := pdDefault;
end;  { Create }

destructor TRaveReport.Destroy;
begin { Destroy }
  FreeAndNil(LoadedList);
  FreeAndNil(FPageList);
  inherited Destroy;
end;  { Destroy }

procedure TRaveReport.Changing(OldItem: TRaveComponent;
                               NewItem: TRaveComponent);
var
  I1: integer;
begin { Changing }
  inherited Changing(OldItem,NewItem);
  if Assigned(FirstPage) and (FirstPage = OldItem) then begin
    FirstPage := NewItem as TRavePage;
  end; { if }
  if Assigned(SecurityControl) and (OldItem = SecurityControl) then begin
    SecurityControl := NewItem as TRaveBaseSecurity;
  end; { if }
  if Assigned(PageList) then begin
    I1 := 0;
    while I1 < PageList.Count do begin
      if Assigned(PageList[I1]) and (PageList[I1] = OldItem) then begin
        if Assigned(NewItem) then begin
          PageList[I1] := NewItem; { Replace with NewItem }
        end else begin
          PageList.Delete(I1); { Remove OldItem from PageList }
          Dec(I1);
        end; { else }
      end; { if }
      Inc(I1);
    end; { while }
  end; { if }
end;  { Changing }

procedure TRaveReport.SaveToStreamHelper(StreamHelper: TStreamHelper);
var
  I1: integer;
begin { SaveToStreamHelper }
  with StreamHelper do begin
    WriteString(LastActivePage);

  { Write out list of owned editors }
    WriteIndex(ChildCount);
    for I1 := 0 to (ChildCount - 1) do begin
      WriteByte(0); { Marker for TRaveProjectItem type }
      WriteString(Child[I1].Name);
    end; { for }

  { Write out list of loaded editors }
    WriteIndex(LoadedList.Count);
    for I1 := 0 to (LoadedList.Count - 1) do begin
      WriteString(TRavePage(LoadedList[I1]).Name);
    end; { for }

  { Write out report properties }
    with TRaveWriter.Create(StreamHelper) do try
      WriteComponent(self,false);
    finally
      Free;
    end; { with }

  { Write out pages }
    for I1 := 0 to (ChildCount - 1) do begin
      StreamHelper.StartWriteBlock;
      TRavePage(Child[I1]).SaveToStreamHelper(StreamHelper);
      StreamHelper.FinishWriteBlock;
    end; { for }
  end; { with }
end;  { SaveToStreamHelper }

procedure TRaveReport.LoadFromStreamHelper(StreamHelper: TStreamHelper);
var
  I1: integer;
  I2: integer;
  NameFound: boolean;
  TestChild: TRaveComponent;
  Items: integer;
  Item: TRaveProjectItem;
begin { LoadFromStreamHelper }
  with StreamHelper do begin
    if TRaveProjectManager(Project).Version >= 30004 then begin {!!! Remove check before release }
      LastActivePage := ReadString;
    end; { if }

    FixUpList := TStringList.Create;
  { Read in list of owned pages }
    Items := ReadIndex;
    for I1 := 1 to Items do begin
      case ReadByte of
        0: Item := TRavePage.Create(self);
        else Raise EClassNotFound.Create(Trans('TRaveProjectItem class not found'));
      end; { case }
      Item.Parent := self;
      Item.Name := ReadString;
    end; { for }

  { Read in list of loaded pages }
    Items := ReadIndex;
    for I1 := 1 to Items do begin
      FixupList.Add(ReadString);
    end; { for }

  { Add all owned pages if not already in FixupList to fix 3.0D bug }
    for I1 := 0 to ChildCount - 1 do begin
      TestChild := TRaveComponent(Child[I1]);
      NameFound := false;
      for I2 := 0 to FixupList.Count - 1 do begin
        if AnsiCompareText(TestChild.Name,FixupList[I2]) = 0 then begin
          NameFound := true;
          Break;
        end; { if }
      end; { for }
      if not NameFound then begin
        FixupList.Add(TestChild.Name);
      end; { if }
    end; { for }

  { Read in report properties }
    with TRaveReader.Create(StreamHelper) do try
      MasterProject := TRaveProjectManager(Project).MasterProject;
      ReadIntoComponent(self);
    finally
      Free;
    end; { with }

  { Read in pages }
    for I1 := 0 to (ChildCount - 1) do begin
      StreamHelper.StartReadBlock;
      TRavePage(Child[I1]).LoadFromStreamHelper(StreamHelper);
      StreamHelper.FinishReadBlock;
    end; { for }
  end; { with }
end;  { LoadFromStreamHelper }

procedure TRaveReport.ProcessLoaded;
var
  I1: integer;
  Item: TRaveComponent;
begin { ProcessLoaded }
  if not Assigned(FixUpList) then Exit;
  for I1 := 0 to FixUpList.Count - 1 do begin
    Item := TRaveProjectManager(Project).FindRaveComponent(FixUpList[I1],self);
    if Assigned(Item) and (Item is TRavePage) then begin
      LoadedList.Add(TRavePage(Item));
    end else begin
    //!!! Error, editor not found
    end; { else }
  end; { for }
  FixUpList.Free;
  FixUpList := nil;
end;  { ProcessLoaded }

function TRaveReport.GetLoadedCount: integer;
begin { GetLoadedCount }
  Result := LoadedList.Count;
end;  { GetLoadedCount }

function TRaveReport.GetLoaded(Index: integer): TRavePage;
begin { GetLoaded }
  if (Index >= 0) and (Index <= LoadedList.Count) then begin
    Result := LoadedList[Index];
  end else begin
    Result := nil;
  end; { else }
end;  { GetLoaded }

procedure TRaveReport.SetPageList(Value: TRaveComponentList);
begin { SetPageList }
  if Assigned(Value) and (Value.Count > 0) then begin
    if not Assigned(FPageList) then begin
      FPageList := TRaveComponentList.Create;
    end; { if }
    CopyList(Value,FPageList);
  end else begin
    if Assigned(FPageList) then begin
      FPageList.Free;
      FPageList := nil;
    end; { if }
  end; { else }
end;  { SetPageList }

procedure TRaveReport.LoadPage(Page: TRavePage);
begin { LoadPage }
  LoadedList.Add(Page);
  Page.Open;
  if SaveEnvOnly then begin
    TRaveProjectManager(Project).DataChanged := true;
  end; { if }
end;  { LoadPage }

procedure TRaveReport.UnloadPage(Page: TRavePage);
begin { UnloadPage }
  Page.Close;
  LoadedList.Remove(Page);
  if SaveEnvOnly then begin
    TRaveProjectManager(Project).DataChanged := true;
  end; { if }
end;  { UnloadPage }

procedure TRaveReport.Open;
var
  I1: integer;
  Page: TRavePage;
begin { Open }
  if not Active then begin
    inherited Open;
    for I1 := 0 to ChildCount - 1 do begin
      TRaveProjectItem(Child[I1]).Open;
    end; { for }
    for I1 := 0 to LoadedList.Count - 1 do begin
      Page := TRavePage(LoadedList[I1]);
      if Page.Global then begin
        Page.Open;
      end; { if }
    end; { for }
  end; { if }
  TRaveProjectManager(Project).FreeForwardRefs;
end;  { Open }

procedure TRaveReport.Close;
var
  I1: integer;
begin { Close }
  if not Active then Exit;
  inherited Close;
  for I1 := 0 to LoadedList.Count - 1 do begin
    TRaveProjectItem(LoadedList[I1]).Close;
  end; { for }
end;  { Close }

function TRaveReport.NewPage: TRavePage;
begin { NewPage }
  Result := TRavePage.Create(self);
  Result.Parent := self;
  Result.Name := TRaveProjectManager(Project).GetUniqueName({Trans-}'Page',self,false);
  LoadPage(Result);
  TRaveProjectManager(Project).DataChanged := true;
end;  { NewPage }

procedure TRaveReport.RSPrint(Sender: TObject);
var
  CurrPage: TRavePage;
  Beginning: boolean;
  PageStack: TRaveStackList;
  PageIdx: integer;
  DonePrint: boolean;
  I1: integer;
  Pages: integer;
begin { RSPrint }
{ BaseReport is initialized to printing engine }
  (Sender as TBaseReport).NoBufferLine := true;

  Pages := MaxPages;
  PageIdx := 0;
  if Assigned(PageList) and (PageList.Count > 0) then begin
    CurrPage := nil;
  end else begin
    CurrPage := FirstPage;
  end; { else }
  Beginning := true;
  PageStack := TRaveStackList.Create;
  try
    repeat
      if not Assigned(CurrPage) then begin { Get new page }
        if PageStack.Empty then begin
          Inc(PageIdx);
          if Assigned(PageList) and (PageList.Count >= PageIdx) then begin
            CurrPage := TRavePage(PageList[PageIdx - 1]);
          end else begin
            Break; { Done printing pages }
          end; { else }
        end else begin
          CurrPage := TRavePage(PageStack.Pop);
        end; { else }
      end; { if }

      if not Beginning then begin
      { Set up new page }
        if CurrPage.Orientation <> poDefault then begin
          (Sender as TBaseReport).Orientation := CurrPage.Orientation;
        end; { if }
        if CurrPage.BinCustom <> '' then begin
          (Sender as TBaseReport).SelectBin(CurrPage.BinCustom);
        end else if CurrPage.Bin <> -1 then begin
          if RPDev <> nil then begin
            for I1 := 0 to RPDev.Bins.Count - 1 do begin
              if longint(RPDev.Bins.Objects[I1]) = CurrPage.Bin then begin
                (Sender as TBaseReport).SelectBin(RPDev.Bins[I1]);
              end; { if }
            end; { for }
          end; { if }
        end; { else }
      { Go to new page }
        (Sender as TBaseReport).NewPage;
      end else begin
        Beginning := false;
      end; { else }
      CurrPage.PrepareSize((Sender as TBaseReport).PageWidth,(Sender as TBaseReport).PageHeight);
      CurrPage.PrintAll((Sender as TBaseReport));
      DonePrint := CurrPage.DonePrinting;
      case CurrPage.GotoMode of
        gmGotoDone: begin
          if DonePrint then begin
            CurrPage := CurrPage.GotoPage;
          end; { if }
        end;
        gmGotoNotDone: begin
          if not DonePrint then begin
            CurrPage := CurrPage.GotoPage;
          end else begin
            CurrPage := nil;
          end; { else }
        end;
        gmCallEach: begin
          if Assigned(CurrPage.GotoPage) then begin
            if not DonePrint then begin
              PageStack.Push(CurrPage);
            end; { if }
            CurrPage := CurrPage.GotoPage;
          {!!! Init CurrPage }
          end else begin
            if DonePrint then begin
              CurrPage := nil;
            end; { if }
          end; { else }
        end;
      end; { case }
      if Pages > 0 then begin
        Dec(Pages);
        if Pages = 0 then Break;
      end; { if }
    until false;
  finally
    PageStack.Free;
  end; { tryf }
end;  { RSPrint }

procedure TRaveReport.RSBeforePrint(Sender: TObject);
var
  Page: TRavePage;
  I1: integer;
begin { RSBeforePrint }
  TRaveProjectManager(Project).BaseReport := (Sender as TBaseReport);

{ Configure BaseReport }
  if Printer <> '' then begin
    (Sender as TBaseReport).SelectPrinter(Printer);
  end; { if }
  if Collate <> pcDefault then begin
    (Sender as TBaseReport).Collate := boolean(Ord(Collate));
  end; { if }
  if Duplex <> pdDefault then begin
    (Sender as TBaseReport).Duplex := TDuplex(Ord(Duplex));
  end; { if }
  //!!PORT!! #138 RPBASE02
  if Copies > 0 then begin
    (Sender as TBaseReport).Copies := Copies;
  end; { if }

{ Get first page to be printed }
  if Assigned(PageList) and (PageList.Count > 0) then begin
    Page := TRavePage(PageList[0]);
  end else begin
    Page := FirstPage;
  end; { else }

  if Assigned(Page) then begin { Configure first page - PaperSize, Orientation, Bin }
    if Page.PaperSize = DMPAPER_USER then begin
      (Sender as TBaseReport).SetPaperSize(0,Page.PageWidth,Page.PageHeight);
    end else if Page.PaperSize <> -1 then begin
      (Sender as TBaseReport).SetPaperSize(Page.PaperSize,0,0);
    end; { else }
    if Page.Orientation <> poDefault then begin
      (Sender as TBaseReport).Orientation := Page.Orientation;
    end; { if }

    if Page.BinCustom <> '' then begin
      (Sender as TBaseReport).SelectBin(Page.BinCustom);
    end else if Page.Bin <> -1 then begin
      if RPDev <> nil then begin
        for I1 := 0 to RPDev.Bins.Count - 1 do begin
          if longint(RPDev.Bins.Objects[I1]) = Page.Bin then begin
            (Sender as TBaseReport).SelectBin(RPDev.Bins[I1]);
          end; { if }
        end; { for }
      end; { if }
    end; { else }
  end; { if }

  if Assigned(SaveBeforePrint) then begin
    SaveBeforePrint(Sender);
  end; { if }
end;  { RSBeforePrint }

procedure TRaveReport.RSAfterPrint(Sender: TObject);
begin { RSAfterPrint }
  TRaveProjectManager(Project).BaseReport := GBaseReport;

  if Assigned(SaveAfterPrint) then begin
    SaveAfterPrint(Sender);
  end; { if }
end;  { RSAfterPrint }

procedure TRaveReport.InternalExecute(Engine: TRPComponent);
var
  I1: integer;
  LHaveControl: boolean;
  SavedUnitsFactor: TRaveFloat;
begin { InternalExecute }
  TRaveProjectManager(Project).PrepareModule;

  if Assigned(CurrentDesigner) then begin
    CurrentDesigner.ClearSelection;
  end; { if }

{ Save designed state for report and all global pages }
  SaveDesigned;
  for I1 := 0 to TRaveProjectManager(Project).GlobalPageList.Count - 1 do begin
    TRaveReport(TRaveProjectManager(Project).GlobalPageList[I1]).SaveDesigned;
  end; { for }
  if RaveDataSystem <> nil then begin
    RaveDataSystem.AutoUpdate := true;
  end;
  TRaveProjectManager(Project).BeforeReport; { Initialize all components }
  TRaveProjectManager(Project).FPrinting := true;

  LHaveControl := true;
  if RaveDataSystem <> nil then begin
    LHaveControl := RaveDataSystem.GainControl;
  end;
  if LHaveControl then try
    if Assigned(Engine) then begin
      if Engine is TRvSystem then begin
        with Engine as TRvSystem do begin
          SavedUnitsFactor := SystemPrinter.UnitsFactor;
          SystemPrinter.UnitsFactor := 1;
          if BaseReport <> nil then begin
            try
              TRaveProjectManager(Project).BaseReport := BaseReport;
              RSPrint(BaseReport);
            finally
              SystemPrinter.UnitsFactor := SavedUnitsFactor;
            end; { tryf }
          end else begin
            SaveOnPrint := OnPrint;
            OnPrint := RSPrint;
            SaveBeforePrint := OnBeforePrint;
            OnBeforePrint := RSBeforePrint;
            SaveAfterPrint := OnAfterPrint;
            OnAfterPrint := RSAfterPrint;
            if AlwaysGenerate then begin
              SystemOptions := SystemOptions + [soUseFiler];
            end; { if }
            try
              Execute;
            finally
              OnPrint := SaveOnPrint;
              OnBeforePrint := SaveBeforePrint;
              OnAfterPrint := SaveAfterPrint;
              SystemPrinter.UnitsFactor := SavedUnitsFactor;
            end; { tryf }
          end; { else }
        end; { with }
      end else begin { Not a TRvSystem, treat as TBaseReport }
        with Engine as TBaseReport do begin
          SavedUnitsFactor := UnitsFactor;
          UnitsFactor := 1;
          if Printing then begin
            try
              TRaveProjectManager(Project).BaseReport := Engine as TBaseReport;
              RSPrint(Engine as TBaseReport);
            finally
              UnitsFactor := SavedUnitsFactor;
            end; { tryf }
          end else begin { Not printing }
            SaveOnPrint := OnPrint;
            OnPrint := RSPrint;
            SaveBeforePrint := OnBeforePrint;
            OnBeforePrint := RSBeforePrint;
            SaveAfterPrint := OnAfterPrint;
            OnAfterPrint := RSAfterPrint;
            try
              Execute;
            finally
              OnPrint := SaveOnPrint;
              OnBeforePrint := SaveBeforePrint;
              OnAfterPrint := SaveAfterPrint;
              UnitsFactor := SavedUnitsFactor;
            end; { tryf }
          end; { else }
        end; { with }
      end; { else }
    end else begin { No Engine assigned, create TRvSystem component }
      with TRvSystem.Create(Application.MainForm) do try
        DefaultDest := PrintDestination;
        TitleSetup := Trans('Output Options');
        TitleStatus := Trans('Report Status');
        TitlePreview := Trans('Report Preview');
        if AllowSetup then begin
          SystemSetups := SystemSetups + [ssAllowSetup];
        end else begin
          SystemSetups := SystemSetups - [ssAllowSetup];
        end; { else }
        SystemPreview.GridHoriz := PreviewGridHoriz;
        SystemPreview.GridVert := PreviewGridVert;
        SystemPreview.GridPen.Color := PreviewGridColor;
        SystemPreview.GridPen.Style := PreviewGridPenStyle;
        SystemPreview.RulerType := PreviewRulerType;
        SystemPreview.MarginPercent := 2.5;
        SystemPreview.ShadowDepth := PreviewShadowDepth;
        SystemPreview.ZoomInc := PreviewZoomInc;
        SystemPreview.ZoomFactor := PreviewZoomFactor;
        SystemPreview.FormState := PreviewWindowState;
        SystemPreview.Monochrome := PreviewMonochrome;
        SystemPrinter.Orientation := poDefault;
        SystemFiler.AccuracyMethod := amAppearance;
        if AlwaysGenerate then begin
          SystemOptions := SystemOptions + [soUseFiler];
        end; { if }
        SaveOnPrint := OnPrint;
        OnPrint := RSPrint;
        SaveBeforePrint := nil;
        OnBeforePrint := RSBeforePrint;
        SaveAfterPrint := nil;
        OnAfterPrint := RSAfterPrint;
        SystemFiler.StreamMode := smTempFile;
        try
          Execute;
        finally
          OnPrint := SaveOnPrint;
          OnBeforePrint := SaveBeforePrint;
          OnAfterPrint := SaveAfterPrint;
        end; { tryf }
      finally
        Free;
      end; { with }
    end; { else }
  finally
    if RaveDataSystem <> nil then begin
      RaveDataSystem.ReleaseControl;
    end;
    TRaveProjectManager(Project).AfterReport;

    if Assigned(CurrentDesigner) then begin
      CurrentDesigner.Invalidate;
    end; { if }

  { Restore designed state for report and all global pages }
(*
    RestoreDesigned;
    For I1 := 0 to TRaveProjectManager(Project).GlobalPageList.Count - 1 do begin
      TRaveReport(TRaveProjectManager(Project).GlobalPageList[I1]).RestoreDesigned;
    end; { for }
*)
    TRaveProjectManager(Project).FPrinting := false;
  end; { if }
end;  { InternalExecute }

procedure TRaveReport.Execute(Engine: TRPComponent);
var
  LRaveContainer: TRaveContainerControl;
  LStream: TStream;
begin { Execute }
// Check to see if any modules need to be recompiled
  if TRaveProjectManager(Project).CompileNeeded then begin
    if not TRaveProjectManager(Project).Compile then begin
      Exit;
    end; { if }
  end; { if }

// Make a copy of the report components and execute
  HoldAddDeleteComponents := true;
  LStream := TMemoryStream.Create;
  try
    TRaveProjectManager(Project).ReportToStream(Self, LStream);
    LRaveContainer := TRaveContainerControl.Create(nil);
    try
      FExecProject := TRaveProjectManager.Create(LRaveContainer);
      try
        FExecProject.MasterProject := TRaveProjectManager(Project);
        try
          LStream.Position := 0;
          FExecProject.NoDesigner := true;
          FExecProject.LoadFromStream(LStream);
          FExecProject.Params.Assign(TRaveProjectManager(Project).Params);
        finally
          FExecProject.MasterProject := nil;
        end;
        FExecProject.FindReport(FullName,true).InternalExecute(Engine);
      finally
        FreeAndNil(FExecProject);
      end;
    finally
      FreeAndNil(LRaveContainer);
    end;
  finally
    HoldAddDeleteComponents := false;
    FreeAndNil(LStream);
  end;
end;  { Execute }

{ class TRaveModuleManager }

constructor TRaveModuleManager.Create(AProjectManager: TRaveProjectManager);
begin { Create }
  inherited Create;
  ModuleList := TList.Create;
  FProjectManager := AProjectManager;
end;  { Create }

destructor TRaveModuleManager.Destroy;
var
  I1: integer;
begin { Destroy }
  for I1 := 0 to ModuleList.Count - 1 do begin
    TRaveModule(ModuleList[I1]).Free;
  end; { for }
  FreeAndNil(ModuleList);
  inherited Destroy;
end;  { Destroy }

function TRaveModuleManager.ModuleIndex(ModuleName: string): integer;
var
  I1: integer;
begin { ModuleIndex }
  for I1 := 0 to ModuleList.Count - 1 do begin
    if SameStr(TRaveModule(ModuleList[I1]).ModuleName,ModuleName) then begin
      Result := I1;
      Exit;
    end; { if }
  end; { for }
  Result := -1;
end;  { ModuleIndex }

function TRaveModuleManager.LoadModule(ModuleName: string): integer;
var
  Module: TRaveModule;
  Stream: TMemoryStream;

  { included for D4 compatibility }
  function IncludeTrailingBackslash(const S: string): string;
  begin
    Result := S;
    if not IsPathDelimiter(Result, Length(Result)) then Result := Result + '\';
  end;
begin { LoadModule }
  Stream := TMemoryStream.Create;
  try
    Stream.LoadFromFile(IncludeTrailingBackslash(ExtractFilePath(Application.ExeName)) +
     ModuleName + {Trans-}'.RVC');
    Module := CreateRaveModule(Stream,ProjectManager,nil);
  finally
    Stream.Free;
  end; { tryf }
  Result := ModuleList.Add(Module);
  RegisterRaveModule(Module);
end;  { LoadModule }

function TRaveModuleManager.GetModule(ModuleName: string): TRaveModule;
var
  I1: integer;
begin { GetModule }
  I1 := ModuleIndex(ModuleName);
  if I1 < 0 then begin { Try to load module }
    I1 := LoadModule(ModuleName);
    if I1 < 0 then begin
      RaiseError(Trans(Format({Trans+}'Could not load module [%s]',[ModuleName])));
    end; { if }
  end; { if }
  Result := TRaveModule(ModuleList[I1]);
end;  { GetModule }

function TRaveModuleManager.FindModule(ModuleName: string): TRaveModule;
var
  I1: integer;
begin { FindModule }
  I1 := ModuleIndex(ModuleName);
  if I1 < 0 then begin
    Result := nil;
  end else begin
    Result := TRaveModule(ModuleList[I1]);
  end; { else }
end;  { FindModule }

procedure TRaveModuleManager.LoadModules(StreamHelper: TStreamHelper);
var
  I1: integer;
  I2: integer;
  ModCount: integer;
  Module: TRaveModule;
  OldModule: TRaveModule;
  S1: string;

  { included for D4 compatibility }
  function IncludeTrailingBackslash(const S: string): string;
  begin
    Result := S;
    if not IsPathDelimiter(Result, Length(Result)) then Result := Result + '\';
  end;
begin { LoadModules }
  with StreamHelper do begin
    ModCount := ReadIndex;
    for I1 := 1 to ModCount do begin
      S1 := ReadString; // Module name
      if FileExists(IncludeTrailingBackslash(ExtractFilePath(Application.ExeName)) + S1 + {Trans-}'.RVC') then begin
        SkipBlock; // Skip loaded module
        if not Assigned(FindModule(S1)) then begin
          LoadModule(S1);
        end; { if }
      end else begin
        StartReadBlock;
      // Load the module from the stream
        Module := CreateRaveModule(Stream,ProjectManager,nil);
      // Register the module
        I2 := ModuleIndex(Module.ModuleName);
        if I2 >= 0 then begin // Already registered, replace
          OldModule := ModuleList[I2];
          ModuleList[I2] := Module;
          FreeAndNil(OldModule);
        end else begin // Add module
          ModuleList.Add(Module);
        end; { else }
        FinishReadBlock;
        RegisterRaveModule(Module);
      end; { else }
    end; { for }
  end; { with }
end;  { LoadModules }

procedure TRaveModuleManager.SaveModules(StreamHelper: TStreamHelper);
var
  I1: integer;
begin { SaveModules }
  with StreamHelper do begin
    WriteIndex(ModuleList.Count);
    for I1 := 0 to ModuleList.Count - 1 do begin
      WriteString(TRaveModule(ModuleList[I1]).ModuleName);
      StartWriteBlock;
      TRaveModule(ModuleList[I1]).SaveToStream(StreamHelper.Stream);
      FinishWriteBlock;
    end; { for }
  end; { with }
end;  { SaveModules }

procedure TRaveModuleManager.ReleaseModule(AModule: TRaveModule);
var
  I1: integer;

  procedure ProcessProjectItem(AProjectItem: TRaveProjectItem);
  var
    I1: integer;
  begin { ProcessProjectItem }
    if Assigned(AProjectItem.Module) then begin
      AProjectItem.Module.Changing(AModule,nil);
    end; { if }
    for I1 := 0 to AProjectItem.ComponentCount - 1 do begin
      if AProjectItem.Components[I1] is TRaveProjectItem then begin
        ProcessProjectItem(TRaveProjectItem(AProjectItem.Components[I1]));
      end; { if }
    end; { for }
  end;  { ProcessProjectItem }

begin { ReleaseModule }
  if not Assigned(AModule) then Exit;
  for I1 := 0 to ModuleList.Count - 1 do begin
    TRaveModule(ModuleList[I1]).Changing(AModule,nil);
  end; { for }
  ProcessProjectItem(ProjectManager);
end;  { ReleaseModule }

{ class TRaveProjectManager }

constructor TRaveProjectManager.Create(AOwner: TComponent);
begin { Create }
  inherited Create(AOwner);

  FReportList := TList.Create;
  FGlobalPageList := TList.Create;
  FDataObjectList := TList.Create;
  FCategories := TStringList.Create;
  Params := TStringList.Create;
  FBaseReport := GBaseReport;
  FModuleManager := TRaveModuleManager.Create(self);

  Signature := {Trans-}'RAV'#26;
  UnitsFactor := ProjectUnitsFactor;
end;  { Create }

destructor TRaveProjectManager.Destroy;
begin { Destroy }
  Clear;
  FreeAndNil(FModuleManager);
  FreeAndNil(Params);
  FreeAndNil(FCategories);
  FreeAndNil(FReportList);
  FreeAndNil(FGlobalPageList);
  FreeAndNil(FDataObjectList);

  inherited Destroy;
end;  { Destroy }

procedure TRaveProjectManager.SetIncluded(AReport: TRaveReport);

  procedure ClearProjectItem(ProjectItem: TRaveProjectItem);
  var
    I1: integer;
  begin { ClearProjectItem }
    ProjectItem.Included := false;
    for I1 := 0 to ProjectItem.ComponentCount - 1 do begin
      if ProjectItem.Components[I1] is TRaveProjectItem then begin
        ClearProjectItem(TRaveProjectItem(ProjectItem.Components[I1]));
      end; { if }
    end; { for }
  end;  { ClearProjectItem }

  procedure ClearProjectItems(ProjectItemList: TList);
  var
    I1: integer;
  begin { ClearProjectItems }
    for I1 := 0 to ProjectItemList.Count - 1 do begin
      ClearProjectItem(TRaveProjectItem(ProjectItemList[I1]));
    end; { for }
  end;  { ClearProjectItems }

  procedure SetIncludedInternal(Item: TPersistent;
                                DoOwned: boolean);
  var
    Count: integer;
    PropList: PPropList;
    PropInfo: PPropInfo;
    I1: integer;
    I2: integer;
    O1: TObject;

    procedure CheckComponent(C1: TComponent);
    begin { CheckComponent }
      if Assigned(C1) then begin
        if C1 is TRaveProjectItem then begin
          if not TRaveComponentAccess(C1).UseMaster and not TRaveProjectItem(C1).Included then begin
            TRaveProjectItem(C1).Included := true;
            SetIncludedInternal(TRaveProjectItem(C1),true);
          end; { if }
        end; { if }
      end; { if }
    end;  { CheckComponent }

  begin { SetIncluded }
    if Assigned(Item) then begin
      Count := GetTypeData(Item.ClassInfo)^.PropCount;
      if Count > 0 then begin
        GetMem(PropList,Count * SizeOf(pointer));
        try
          GetPropInfos(Item.ClassInfo,PropList);
          for I1 := 0 to (Count - 1) do begin
            PropInfo := PropList^[I1];
            if PropInfo.PropType^.Kind = tkClass then begin
              O1 := TObject(GetOrdProp(Item,PropInfo));
              if Assigned(O1) then begin
                if O1 is TComponent then begin
                  CheckComponent(TComponent(O1).Owner);
                  CheckComponent(TComponent(O1));
                end else if O1 is TPersistent then begin
                  SetIncludedInternal(TPersistent(O1),false);
                end else if O1 is TRaveComponentList then begin
                  for I2 := 0 to TRaveComponentList(O1).Count - 1 do begin
                    CheckComponent(TComponent(TRaveComponentList(O1)[I2]).Owner);
                    CheckComponent(TComponent(TRaveComponentList(O1)[I2]));
                  end; { for }
                end else if O1 is TRavePersistentList then begin
                  for I2 := 0 to TRavePersistentList(O1).Count - 1 do begin
                    SetIncludedInternal(TPersistent(TRavePersistentList(O1)[I2]),false);
                  end; { for }
                end; { else }
              end; { if }
            end; { if }
          end; { for }
        finally
          FreeMem(PropList,Count * SizeOf(pointer));
        end; { tryf }
      end; { if }

      if DoOwned and (Item is TComponent) then begin
        for I1 := 0 to TComponent(Item).ComponentCount - 1 do begin
          SetIncludedInternal(TComponent(Item).Components[I1],true);
        end; { for }
      end; { if }
    end; { if }
  end;  { SetIncludedInternal }

begin { SetIncluded }
  ClearProjectItems(ReportList);
  ClearProjectItems(GlobalPageList);
  ClearProjectItems(DataObjectList);
  Included := true;
  AReport.Included := true;
  SetIncludedInternal(AReport,false);
end;  { SetIncluded }

procedure TRaveProjectManager.ReportToStream(AReport: TRaveReport;
                                             AStream: TStream);

var
  StreamHelper: TStreamHelper;

  procedure AddProjectItems(ProjectItemList: TList);
  var
    I1: integer;
  begin { AddProjectItems }
    for I1 := 0 to ProjectItemList.Count - 1 do begin
      if TRaveProjectItem(ProjectItemList[I1]).Included then begin
        ExportList.Add(ProjectItemList[I1]);
      end; { if }
    end; { for }
  end;  { AddProjectItems }

begin { ReportToStream }
// Include all other project items that it references
  SetIncluded(AReport);

// Create ExportList
  ExportList := TList.Create;
  ExportList.Add(AReport);
  AddProjectItems(GlobalPageList);
  AddProjectItems(DataObjectList);

// Write out items to stream
  StreamHelper := TStreamHelper.Create(AStream);
  with StreamHelper do try
    CompressMethod := NoCompression;
    SaveToStreamHelper(StreamHelper);
  finally
    Free;
  end; { with }
  FreeAndNil(ExportList);
end;  { ReportToStream }

procedure TRaveProjectManager.Clear;
begin { Clear }
  ClearObjectList(ReportList);
  ClearObjectList(GlobalPageList);
  ClearObjectList(DataObjectList);
end;  { Clear }

procedure TRaveProjectManager.DefineProperties(Filer: TFiler);
begin { DefineProperties }
  inherited DefineProperties(Filer);
  Filer.DefineProperty({Trans-}'Params', ReadParamValues, WriteParamValues,
   StreamParamValues and (Params.Count > 0));
end;  { DefineProperties }

procedure TRaveProjectManager.ReadParamValues(Reader: TReader);
var
  ParamBuf: string;
  ParamPtr: PChar;
  ParamName: string;
  ParamData: string;
begin { ReadParamValues }
  ParamBuf := Reader.ReadString;
  if ParamBuf <> '' then begin
    ParamPtr := @ParamBuf[1];
    while ParamPtr^ <> #0 do begin
      ParamName := AnsiExtractQuotedStr(ParamPtr,'"');
      Inc(ParamPtr); { Skip comma }
      ParamData := AnsiExtractQuotedStr(ParamPtr,'"');
      SetParam(ParamName, ParamData);
      Inc(ParamPtr); { Skip semicolon }
    end; { while }
  end; { else }
end;  { ReadParamValues }

procedure TRaveProjectManager.WriteParamValues(Writer: TWriter);
var
  ParamBuf: string;
  I1: integer;
begin { WriteParamValues }
  ParamBuf := '';
  for I1 := 0 to (Params.Count - 1) do begin
    ParamBuf := ParamBuf + AnsiQuotedStr(Params.Names[I1], '"') + ',' +
     AnsiQuotedStr(Params.Values[Params.Names[I1]] ,'"') + ';';
  end; { for }
  Writer.WriteString(ParamBuf);
end;  { WriteParamValues }

procedure TRaveProjectManager.LoadFromStreamHelper(StreamHelper: TStreamHelper);
var
  TagKind: TTagKind;
  Item: TRaveProjectItem;
  ItemName: string;
  I1: integer;
  RaveReport: TRaveReport;
  TestSig: string;
begin { LoadFromStreamHelper }
  with StreamHelper do begin
    SetLength(TestSig,4);
    Stream.Read(TestSig[1],4);
    if TestSig <> Signature then begin
      RaveError(Trans('Invalid Project Format'));
      Exit;
    end; { if }
    Stream.Read(FVersion,SizeOf(FVersion));

  { Read project header information }
    TagKind := TTagKind(ReadByte); { Should be tkProject }
    if TagKind <> tkProject then begin
      RaveError(Trans('Invalid Project Header Code'));
      Exit;
    end; { if }

  { Read in library modules }
    if Version >= 40005 then begin
      ModuleManager.LoadModules(StreamHelper);
    end; { if }

    if Importing then begin
    { Absorb and ignore project header }
      ReadString; { Absorb LastActivePage }
      with TRaveReader.Create(StreamHelper) do try
        MasterProject := self.FMasterProject;
        ReadComponent(self,self).Free;
      finally
        Free;
      end; { with }
    end else begin
    { Read in project header }
      LastActiveReport := ReadString;
      with TRaveReader.Create(StreamHelper) do try
        MasterProject := self.FMasterProject;
        ReadIntoComponent(self);
      finally
        Free;
      end; { with }
    end; { else }

  { Read in ProjectItem blocks }
    repeat
      TagKind := TTagKind(ReadByte);
      ItemName := ReadString;
      if Importing then begin
        Item := FindRaveComponent(ItemName, TRaveProjectManager(Project)) as TRaveProjectItem;
        if Assigned(Item) then begin
          if Assigned(FOnImportConflict) then begin
            FOnImportConflict(Item,ItemName);
          end else begin
            if ImportReplace then begin { Delete and replace old item }
              DeleteItem(Item,false);
            //!!! Need to make it so that links to this item or components on
            //!!! this item are preserved.
            end else begin { Get new name }
              ItemName := GetUniqueName(ItemName, TRaveProjectManager(Project), false);
            end; { else }
          end; { else }
        end; { if }
      end; { if }

      case TagKind of
        tkDataView: begin // Only used to load pre 4.0 projects
          Item := TRaveDataView.Create(self);
          DataObjectList.Add(Item);
        end;
        tkDataObject: begin
          Item := TRaveDataObjectClass(FindClass(ReadString)).Create(self);
          DataObjectList.Add(Item);
        end;
        tkPage: begin
          Item := TRavePage.Create(self);
          GlobalPageList.Add(Item);
        end;
        else begin { tkReport }
          Item := TRaveReport.Create(self);
          ReportList.Add(Item);
          FActiveReport := Item as TRaveReport;
        end;
      end; { case }
      Item.Parent := self;
      if ItemName <> '' then begin
        Item.Name := ItemName;
        AddComponent(Item);
      end; { if }
      StartReadBlock;
      Item.LoadFromStreamHelper(StreamHelper);
      FinishReadBlock;
      FActiveReport := nil;
      if ItemName = '' then begin
        case TagKind of
          tkDataView,tkDataObject: DataObjectList.Remove(Item);
          tkPage: GlobalPageList.Remove(Item);
          else ReportList.Remove(Item);
        end; { case }
        Item.Free;
      end; { if }
    until Empty;
  end; { with }
  FreeForwardRefs;

{ Activate report }
  RaveReport := nil;
  for I1 := 0 to ReportList.Count - 1 do begin
    TRaveReport(ReportList[I1]).ProcessLoaded;
    if (LastActiveReport <> '') and
     (TRaveReport(ReportList[I1]).Name = LastActiveReport) then begin
      RaveReport := TRaveReport(ReportList[I1]);
    { Break; Don't break since we need to call ProcessLoaded for each report }
    end; { if }
  end; { for }

  if Assigned(RaveReport) then begin
    ActivateReport(RaveReport);
  end else if ReportList.Count > 0 then begin
    ActivateReport(TRaveReport(ReportList[0]));
  end; { else }
  FreeForwardRefs;
  PostLoad;
  ClearChanged;
  Saved := true;
end;  { LoadFromStreamHelper }

procedure TRaveProjectManager.SaveToStreamHelper(StreamHelper: TStreamHelper);

  procedure WriteListNames(List: TList;
                           TagKind: TTagKind);

  var
    I1: longint;

  begin { WriteListNames }
    with StreamHelper do begin
      for I1 := 0 to (List.Count - 1) do begin
        if Assigned(ExportList) then begin { Look for item in list }
          if ExportList.IndexOf(List[I1]) < 0 then Continue; { Don't export item }
        end; { if }
        WriteByte(Ord(TagKind));
        WriteString(TRaveProjectItem(List[I1]).Name);
        if TagKind = tkDataObject then begin
          WriteString(TRaveDataObject(List[I1]).ClassName);
        end; { if }
        StartWriteBlock;
        TRaveProjectItem(List[I1]).SaveToStreamHelper(StreamHelper);
        FinishWriteBlock;
      end; { for }
    end; { with }
  end;  { WriteListNames }

begin { SaveToStreamHelper }
  StreamHelper.WriteBuf(Signature[1],4);
  FVersion := RaveVersion;
  StreamHelper.WriteBuf(FVersion,SizeOf(FVersion));

  StreamHelper.WriteByte(Ord(tkProject));
  ModuleManager.SaveModules(StreamHelper);
  StreamHelper.WriteString(LastActiveReport);
  with TRaveWriter.Create(StreamHelper) do try
    WriteComponent(self,false);
  finally
    Free;
  end; { with }
  WriteListNames(DataObjectList,tkDataObject);
  WriteListNames(GlobalPageList,tkPage);
  WriteListNames(ReportList,tkReport);
  if not Assigned(ExportList) then begin
    ClearChanged;
    Saved := true;
  end; { if }
end;  { SaveToStreamHelper }

procedure TRaveProjectManager.LoadFromStream(Stream: TStream);
var
  StreamHelper: TStreamHelper;
begin { LoadFromStream }
  StreamHelper := TStreamHelper.Create(Stream);
  with StreamHelper do try
    LoadFromStreamHelper(StreamHelper);
  finally
    Free;
  end; { with }
end;  { LoadFromStream }

procedure TRaveProjectManager.SaveToStream(Stream: TStream);
var
  StreamHelper: TStreamHelper;
begin { SaveToStream }
  StreamHelper := TStreamHelper.Create(Stream);
  with StreamHelper do try
    SaveToStreamHelper(StreamHelper);
  finally
    Free;
  end; { with }
end;  { SaveToStream }

procedure TRaveProjectManager.SetCategories(Value: TStrings);
begin { SetCategories }
  FCategories.Assign(Value);
end;  { SetCategories }

procedure TRaveProjectManager.Changing(OldItem: TRaveComponent;
                                       NewItem: TRaveComponent);
begin { Changing }
  inherited Changing(OldItem,NewItem);
  if Assigned(SecurityControl) and (OldItem = SecurityControl) then begin
    SecurityControl := NewItem as TRaveBaseSecurity;
  end; { if }
end;  { Changing }

procedure TRaveProjectManager.SetDepth(Control: TRaveComponent; Adjustment: integer);
var
  I1: integer;
  I2: integer;
  ChildList: TList;
begin { SetDepth }
  if Control is TRaveReport then begin
    ChildList := ReportList;
  end else if Control is TRavePage then begin
    ChildList := GlobalPageList;
  end else if Control is TRaveDataObject then begin
    ChildList := DataObjectList;
  end else begin
    Exit;
  end; { else }

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

function TRaveProjectManager.FindRaveComponent(Name: string;
                                               DefRoot: TRaveComponent): TRaveComponent;
var
  I1: integer;
  I2: integer;
  P1: TRavePage;
  Item: TRaveProjectItem;
  ObjectName: string;
  Root: TRaveComponent;
begin { FindRaveComponent }
//TODO: Add capability to process Owner.Owner.Component strings
  I1 := AnsiPos('.',Name);
  if I1 > 0 then begin
  { Find owner }
    ObjectName := MBCSCopy(Name,1,I1 - 1); { Owner portion }

  { Look for owner in ActiveReport pages }
    Root := nil;
    if Assigned(ActiveReport) then begin
      with ActiveReport do begin
        for I2 := 0 to ChildCount - 1 do begin
          if Child[I2] is TRavePage then begin
            P1 := TRavePage(Child[I2]);
            if AnsiCompareText(P1.Name,ObjectName) = 0 then begin
              Root := P1;
              Break;
            end; { if }
          end; { if }
        end; { for }
      end; { with }
    end; { if }

  { Look for owner in ProjectItems (Reports/Global Pages/Data Views) }
    if not Assigned(Root) then begin
      for I2 := 0 to ChildCount - 1 do begin
        Item := TRaveProjectItem(Child[I2]);
        if AnsiCompareText(Item.Name,ObjectName) = 0 then begin
          Root := Item;
          Break;
        end; { if }
      end; { for }
    end; { if }

  { Second half is object's name on Root }
    if Assigned(Root) then begin
      ObjectName := MBCSCopy(Name,I1 + 1,Length(Name) - I1);
    end; { if }
  end else begin
    if Assigned(DefRoot) then begin
      Root := DefRoot;
    end else begin
      Root := TRaveProjectManager(Project);
    end; { else }
    ObjectName := Name;
  end; { else }

  if not Assigned(Root) then begin
    Result := nil;
  end else begin
    Result := TRaveComponent(Root.FindComponent(ObjectName));
    if not Assigned(Result) then begin
      Result := TRaveComponent(TRaveProjectManager(Project).FindComponent(ObjectName));
    end; { if }
  end; { else }
end;  { FindRaveComponent }

function TRaveProjectManager.GetUniqueName(BaseName: string;
                                           NameOwner: TRaveComponent;
                                           UseCurrent: boolean): string;
var
  I1: integer;
  OrigName: string;
  SepCh: char;
begin { GetUniqueName }
  if UseCurrent then begin { BaseName='OrigName|BaseName' }
  { Try to use OrigName first, then switch to BaseName }
    OrigName := CutWord(BaseName,SepCh,'|');
    Result := OrigName;
    if not Assigned(NameOwner.FindComponent(Result)) then Exit;
  end; { if }
  for I1 := 1 to 9999 do begin
    Result := BaseName + IntToStr(I1);
    if not Assigned(NameOwner.FindComponent(Result)) then Exit;
  end; { for }
end;  { GetUniqueName }

procedure TRaveProjectManager.DeactivateReport;
begin { DeactivateReport }
  if Assigned(FActiveReport) then begin
    ActiveReport.Close;
    FActiveReport := nil;
    CurrentDesigner := nil;
  end; { if }
end;  { DeactivateReport }

procedure TRaveProjectManager.ActivateReport(Report: TRaveReport);
begin { ActivateReport }
  DeactivateReport;
  FActiveReport := Report;
  ActiveReport.Open;
  LastActiveReport := ActiveReport.Name;
  if SaveEnvOnly then begin
    DataChanged := true;
  end; { if }
end;  { ActivateReport }

procedure TRaveProjectManager.ExportProject(ExportFileName: string;
                                            Items: TList);
var
  Stream: TStream;
begin { ExportProject }
  Stream := TFileStream.Create(ExportFileName,fmCreate);
  try
    ExportList := Items;
    SaveToStream(Stream);
    ExportList := nil;
  finally
    Stream.Free;
  end; { tryf }
end;  { ExportProject }

function TRaveProjectManager.ImportProject(ImportFileName: string;
                                           AutoReplace: boolean): boolean;
var
  Stream: TStream;
begin { ImportProject }
  Result := false;
  if FileExists(ImportFileName) then begin
    Stream := TFileStream.Create(ImportFileName,fmOpenRead);
    if Stream.Size > 0 then begin
      try
        Importing := true;
        ImportReplace := AutoReplace;
        LoadFromStream(Stream);
        Importing := false;
      finally
        Stream.Free;
      end; { tryf }
      DataChanged := true;
      Result := true;
    end; { if }
  end; { if }
end;  { ImportProject }

procedure TRaveProjectManager.Save;
var
  Stream: TStream;
begin { Save }
  Stream := TFileStream.Create(ChangeFileExt(FileName,{Trans-}'.$$$'),fmCreate);
  try
    SaveToStream(Stream);
  finally
    Stream.Free;
  end; { tryf }

{ Rename file from FileName.$$$ to FileName.rav and save backup *.~ra }
  DeleteFile(ChangeFileExt(FileName,{Trans-}'.~ra'));
  RenameFile(FileName,ChangeFileExt(FileName,{Trans-}'.~ra'));
  RenameFile(ChangeFileExt(FileName,{Trans-}'.$$$'),FileName);
end;  { Save }

procedure TRaveProjectManager.Load;
var
  Stream: TStream;
begin { Load }
  IsLoading := true;
  try
    if FileExists(FileName) then begin
      Stream := TFileStream.Create(FileName,fmOpenRead);
      if Stream.Size > 0 then begin
        try
          LoadFromStream(Stream);
        finally
          Stream.Free;
        end; { tryf }
      end else begin
        Stream.Free;
        New;
      end; { else }
    end else begin
      New;
    end; { else }
  finally
    IsLoading := false;
  end; { tryf }
end;  { Load }

procedure TRaveProjectManager.New;
begin { New }
  try
    IsLoading := true;
    FileName := {Trans-}'Project1.rav';
    NewReport;
    ClearChanged;
    Saved := false;
  finally
    IsLoading := false;
  end; { tryf }
end;  { New }

procedure TRaveProjectManager.Unload;
begin { Unload }
  DeactivateReport;
  Clear;
end;  { Unload }

{ File methods }

function TRaveProjectManager.NewReport: TRaveReport;
begin { NewReport }
  Result := TRaveReport.Create(self);
  Result.Parent := self;
  Result.Name := GetUniqueName({Trans-}'Report', TRaveProjectManager(Project), false);
  AddComponent(Result);
  ReportList.Add(Result);
  ActivateReport(Result);
  Result.NewPage;
  Result.FirstPage := Result.Child[0] as TRavePage;
  DataChanged := true;
end;  { NewReport }

function TRaveProjectManager.NewGlobalPage: TRavePage;
begin { NewGlobalPage }
  Result := TRavePage.Create(self);
  Result.Parent := self;
  Result.Name := GetUniqueName({Trans-}'GlobalPage', TRaveProjectManager(Project), false);
  GlobalPageList.Add(Result);
  AddComponent(Result);
  ActiveReport.LoadPage(Result);
  DataChanged := true;
end;  { NewGlobalPage }

function TRaveProjectManager.NewDataObject(DataObjectClass: TRaveDataObjectClass): TRaveDataObject;
var
  NewName: string;
begin { NewDataObject }
  Result := DataObjectClass.Create(self);
  Result.Parent := self;
  NewName := DataObjectClass.ClassName;
  if NewName[1] = 'T' then begin
    Delete(NewName,1,1);
  end; { if }
  if Pos({Trans-}'RAVE',UpperCase(NewName)) = 1 then begin
    Delete(NewName,1,4);
  end; { if }
  Result.Name := TRaveProjectManager(Project).GetUniqueName(NewName,Result.Owner as TRaveComponent,false);
  DataObjectList.Add(Result);
  AddComponent(Result);
  DataChanged := true;
end;  { NewDataObject }

procedure TRaveProjectManager.DeleteItem(Item: TRaveProjectItem;
                                         Notify: boolean);
var
  I1: integer;
  CheckReportPage: boolean;
begin { DeleteItem }
  CheckReportPage := false;
  if Item is TRaveReport then begin
    if ReportList.Count = 1 then begin
      NewReport;
    end else if ActiveReport = Item then begin
      I1 := ReportList.IndexOf(Item);
      if I1 = 0 then begin
        I1 := 1;
      end else begin
        Dec(I1);
      end; { else }
      ActivateReport(TRaveReport(ReportList[I1]));
    end; { else }
    ReportList.Remove(Item);
  end else if Item is TRavePage then begin
    ActiveReport.UnloadPage(TRavePage(Item));
    if TRavePage(Item).Global then begin
      GlobalPageList.Remove(Item);
    end else begin
      CheckReportPage := true;
    end; { else }
  end else if Item is TRaveDataObject then begin
    DataObjectList.Remove(Item);
  end; { else }
  DeleteComponent(Item);
  Item.Free;
  if CheckReportPage and (ActiveReport.ChildCount = 0) then begin
    ActiveReport.NewPage;
  end; { if }
end;  { DeleteItem }

procedure TRaveProjectManager.SetParam(Param: string; Value: string);
begin { SetParam }
  Params.Values[Param] := Value;
end;  { SetParam }

function TRaveProjectManager.GetParam(Param: string): string;
begin { GetParam }
  Result := Params.Values[Param];
end;  { GetParam }

procedure TRaveProjectManager.ClearParams;
begin { ClearParams }
  Params.Clear;
end;  { ClearParams }

procedure TRaveProjectManager.SetUnits(Value: TPrintUnits);
begin { SetUnits }
  FUnits := Value;
  case FUnits of
    unInch: FUnitsFactor := 1.0;
    unMM: FUnitsFactor := 25.4;
    unCM: FUnitsFactor := 2.54;
    unPoint: FUnitsFactor := 72.0;
    unUser: begin
    { Don't change FUnitsFactor }
    end;
  end; { case }
  if Assigned(CurrentDesigner) then begin
    CurrentDesigner.Modified;
  end; { if }
end;  { SetUnits }

procedure TRaveProjectManager.SetUnitsFactor(Value: double);
begin { SetUnitsFactor }
  if Value > 0.0 then begin
    FUnitsFactor := Value;
    if FEQ(FUnitsFactor,1.0) then begin
      FUnits := unInch;
    end else if FEQ(FUnitsFactor,25.4) then begin
      FUnits := unMM;
    end else if FEQ(FUnitsFactor,2.54) then begin
      FUnits := unCM;
    end else if FEQ(FUnitsFactor,72.0) then begin
      FUnits := unPoint;
    end else begin
      FUnits := unUser;
    end; { else }
  end; { if }
  if Assigned(CurrentDesigner) then begin
    CurrentDesigner.Modified;
  end; { if }
end;  { SetUnitsFactor }

procedure TRaveProjectManager.SetDataChanged(Value: boolean);
var
{$IFNDEF LEVEL6}
  FileProps: integer;
{$ENDIF}
  ReadOnly: boolean;
begin { SetDataChanged }
{ DataChanged will only change value when set to true, to clear call ClearChanged }
  if Value then begin
    if not FDataChanged and not IsLoading and Saved then begin
      if not FileExists(FileName) then begin
        ReadOnly := false;
      end else begin
      {$IFDEF Level6}
        ReadOnly := FileIsReadOnly(FileName);
      {$ELSE}
        FileProps := FileGetAttr(FileName);
        ReadOnly := (FileProps >= 0) and ((FileProps and faReadOnly) = faReadOnly);
      {$ENDIF}
      end; { else }
      if ReadOnly then begin
        ShowMessage(Trans('The report project file you are modifying is marked read only.  ' +
         'You will be prompted for another file name when saving.'));
      end; { if }
    end; { if }
    FDataChanged := true;
  end; { if }
end;  { SetDataChanged }

procedure TRaveProjectManager.ClearChanged;
begin { ClearChanged }
  FDataChanged := false;
end;  { ClearChanged }

function TRaveProjectManager.Compile: boolean;

type
  TActionProc = procedure(aProjectItem: TRaveProjectItem);

var
  CompileStatus: TRaveCompileStatus;

  procedure ActionBuildSource(aProjectItem: TRaveProjectItem);
  begin { ActionBuildSource }
    if Assigned(aProjectItem.Module) then begin
      aProjectItem.Module.Free;
    end; { if }
    aProjectItem.Module := CreateRaveModule(nil,aProjectItem.Project,aProjectItem);
    aProjectItem.Module.BuildSource;
  end;  { ActionBuildSource }

  procedure ActionDefCompile(aProjectItem: TRaveProjectItem);
  begin { ActionDefCompile }
    CompileModule := aProjectItem.Module;
    CompileModule.Compile(true);
  end;  { ActionDefCompile }

  procedure ActionImpCompile(aProjectItem: TRaveProjectItem);
  begin { ActionImpCompile }
    CompileModule := aProjectItem.Module;
    CompileModule.Compile(false);
  end;  { ActionImpCompile }

  procedure ActionFixUp(aProjectItem: TRaveProjectItem);
  begin { ActionFixUp }
  // Fix up any cross module references
  end;  { ActionFixUp }

  procedure ActionEventConnect(aProjectItem: TRaveProjectItem);
  begin { ActionEventConnect }
    aProjectItem.Module.EventConnect;
  end;  { ActionEventConnect }

  procedure PerformAction(aProjectItem: TRaveProjectItem;
                          ActionProc: TActionProc);
  var
    I1: integer;
  begin { PerformAction }
    ActionProc(aProjectItem);
    if (aProjectItem is TRaveProjectManager) or (aProjectItem is TRaveReport) then begin
      for I1 := 0 to aProjectItem.ComponentCount - 1 do begin
        if aProjectItem.Components[I1] is TRaveProjectItem then begin
          PerformAction(TRaveProjectItem(aProjectItem.Components[I1]),ActionProc);
        end; { if }
      end; { for }
    end; { if }
  end;  { PerformAction }

begin { Compile }
  Result := false;
  try
  // Create source for all modules
    PerformAction(ProjectManager,@ActionBuildSource);
  // Perform Definition only compiles
    PerformAction(ProjectManager,@ActionDefCompile);
  // Compile Implementation compiles
    PerformAction(ProjectManager,@ActionImpCompile);
  // Fix up cross module references
    PerformAction(ProjectManager,@ActionFixUp);
  // Point events to function locations
    PerformAction(ProjectManager,@ActionEventConnect);
    CompileNeeded := false;
    RaveCompileStatus(nil);
    Result := true;
  except
    on E: ERaveCompiler do begin
      CompileStatus := TRaveCompileStatus.Create;
      CompileStatus.ErrorLine := ErrorLine;
      CompileStatus.ErrorCol := ErrorCol;
      CompileStatus.ErrorEvent := ErrorEvent;
      CompileStatus.ErrorMsg := E.Message;
      RaveCompileStatus(CompileStatus);
      CompileStatus.Free;
    end;
    on E: Exception do begin
      CompileStatus := TRaveCompileStatus.Create;
      CompileStatus.ErrorLine := 0;
      CompileStatus.ErrorCol := 0;
      CompileStatus.ErrorEvent := nil;
      CompileStatus.ErrorMsg := 'General Compiler Error - ' + E.Message;
      RaveCompileStatus(CompileStatus);
      CompileStatus.Free;
    end;
  end; { tryx }
end;  { Compile }

function TRaveProjectManager.FindReport(const AName: string;
 const AIsFullName: boolean): TRaveReport;
var
  i: integer;
  LReport: TRaveReport;
begin
  Result := nil;
  for i := 0 to ReportList.Count - 1 do begin
    LReport := TRaveReport(ReportList.Items[i]);
    if (AIsFullName and (AnsiCompareText(LReport.FullName, AName) = 0))
     or (AnsiCompareText(LReport.Name, AName) = 0) then begin
      Result := LReport;
      Break;
    end;
  end;
end;

procedure TRaveProjectManager.FreeForwardRefs;
var
  I1: integer;
  Index: integer;
  Target: TRaveComponent;
begin { FreeForwardRefs }
  if Assigned(ForwardRefList) then begin
    for I1 := 0 to ForwardRefList.Count - 1 do begin
      with TRaveForwardRef(ForwardRefList.Objects[I1]) do begin
      { Figure out Target component }
        if Assigned(RenameRefList) and
         RenameRefList.Find(ForwardRefList[I1],Index) then begin
        { Target was renamed }
          Target := TRaveComponent(RenameRefList.Objects[Index]);
        end else begin
        { Target was not renamed }
          Target := FindRaveComponent(ForwardRefList[I1],
           TRaveComponent(TRaveForwardRef(ForwardRefList.Objects[I1]).
           Instance).Parent);
          if not Assigned(Target) then begin
            Continue; {!!! Unresolved forward reference }
          end; { if }
        end; { else }

      { Assign Target to forward reference }
        if not Assigned(PropInfo) then begin
          TRaveComponentList(Instance).Items[Param] := Target;
        end else begin
          SetOrdProp(Instance,PropInfo,integer(Target));
        end; { else }
      end; { with }
    end; { for }
    ClearStringList(ForwardRefList);
    ForwardRefList.Free;
    ForwardRefList := nil;
    RenameRefList.Free;
    RenameRefList := nil;
  end; { if }
end;

initialization
  RegisterProc({Trans-}'RVCL', RaveRegister);
end.
