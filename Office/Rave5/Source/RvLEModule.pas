{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RvLEModule;

interface

{.$DEFINE DOMAP}

uses
  Windows, Dialogs,
  TypInfo, Classes, SysUtils, RvClass, RvLEDefine,
  RvLEID, RvLEType, RvDefine, RvUtil, RpDefine;

type
  TRCPUModule = class;
  TRaveCompiler = class;

  TScopeManager = class
  private
    DataIdx: longint;
  protected
    FOwner: TRCPUModule;
    FParent: TScopeManager;
    FTypeManager: TTypeManager;
    FIDManager: TIDManager;
    FObjectScope: TList;
  public
    constructor Create(AParent: TScopeManager;
                       AOwner: TRCPUModule);
    destructor Destroy; override;
    procedure LoadFromStreamHelper(StreamHelper: TStreamHelper);
    procedure SaveToStreamHelper(StreamHelper: TStreamHelper);
    function FindID(ID: TIdentifier): TBaseID;
    function AddType(ID: TIdentifier;
                     BaseType: TBaseType): TTypeID;
    function AddVar(ID: TIdentifier;
                    BaseType: TBaseType): TVarID;
    function AddOp(ID: TIdentifier;
                   OpKind: TOpKind): TOpID;
    function AddFunc(ID: TIdentifier;
                     BaseType: TBaseType): TFuncID;
    function AddConst(    ID: TIdentifier;
                          BaseType: TBaseType;
                      var Data): TConstID;
    procedure ClearData;
    procedure PushObjectScope(AObject: TVarID);
    procedure PopObjectScope;
    function FindObjectScope(ID: TIdentifier): TVarID;

    property Owner: TRCPUModule read FOwner;
    property Parent: TScopeManager read FParent;
    property TypeManager: TTypeManager read FTypeManager;
    property IDManager: TIDManager read FIDManager;
    property DataSize: longint read DataIdx write DataIdx;
  end; { TScopeManager }

  TModuleTypeHelper = class(TTypeHelper)
    FModule: TRCPUModule;
  public
    constructor Create(AModule: TRCPUModule);

    function IndexToType(ModuleIndex: longint;
                         TypeIndex: longint): TBaseType; override;
    procedure TypeToIndex(    BaseType: TBaseType;
                          var ModuleIndex: longint;
                          var TypeIndex: longint); override;

    property Module: TRCPUModule read FModule;
  end; { TModuleTypeHelper }

  TRCPUModule = class(TRaveModule)
  protected
    UsedModules: TList; { List of other modules used by this module }
    ExternalList: TStringList; { List of external functions }
    ForwardRefList: TStringList; { List of ProjectItem modules to point to }
    Params: TList; { List of parameters to push on stack before executing }

    FMajorVersion: word;
    FMinorVersion: word;
    FCompileDate: TDateTime;
    FScope: TScopeManager;
    FTypeHelper: TTypeHelper;

    FDataBuf: pointer;
    FDataSize: longint;
    FCodeBuf: pointer;
    FCodeSize: longint;
    FStartCO: longint; { Starting CO if this module can be executed }
    FCreateCO: longint; { CO to start at when creating }
    FDestroyCO: longint; { CO to start at when destroying }
    FStackSize: longint; { Starting stack size }
    FSourceStream: TMemoryStream; { Stream containing original source }
    FEventList: TStrings; { Links functions to owner events }
    FSourceRefList: TList; { Links events to source code positions }
    FProjectItem: TRaveProjectItem;
    FProjectManager: TRaveProjectItem;
    FCompiler: TRaveCompiler;
    FParentModule: TRCPUModule;

    function GetParentModule: TRCPUModule;
    function GetDataBuf: pointer;
    procedure SetDataSize(Value: longint);
    procedure PostLoad; override;
  public
    constructor Create;
    constructor CreateFromStream(Stream: TStream;
                                 aOwner: TRaveProjectItem;
                                 aProjectItem: TRaveProjectItem); override;
    constructor CreateFromFile(FileName: string;
                               AOwner: TRaveProjectItem); override;
    destructor Destroy; override;
    procedure SaveToStream(Stream: TStream); override;
    procedure SaveToFile(FileName: string);
    procedure Changing(OldItem: TRaveModule;
                       NewItem: TRaveModule); override;

    function AddType(ID: TIdentifier;
                     BaseType: TBaseType): TTypeID;
    function CreateType(BaseType: TBaseType): TBaseType;
    function AddVar(ID: TIdentifier;
                    BaseType: TBaseType): TVarID;
    function AddOp(ID: TIdentifier;
                   OpKind: TOpKind): TOpID;
    function AddFunc(ID: TIdentifier;
                     BaseType: TBaseType): TFuncID;
    function AddConst(    ID: TIdentifier;
                          BaseType: TBaseType;
                      var Data): TConstID;
    function AddModule(ModuleName: TIdentifier): TRCPUModule;
    procedure AddExternal(AID: TIdentifier;
                          AOffset: longint);

    function FindInternalID(ID: TIdentifier): TBaseID;
    function FindID(ID: TIdentifier): TBaseID;
    function FindType(ID: TIdentifier): TBaseType;
    function FindExternal(ID: TIdentifier): PExternalFunc;
    function IndexToModule(Index: longint): TRCPUModule;
    function ModuleToIndex(Module: TRCPUModule): longint;
    function IndexToType(Index: longint): TBaseType;
    function TypeToIndex(BaseType: TBaseType): longint;
    procedure CreateScopeLevel;
    procedure FreeScopeLevel;
    procedure Execute(Position: integer; var SuspendState: TObject); override;
    procedure Init;
    procedure BuildSource; override;
    procedure Compile(Definition: boolean); override;
    procedure EventConnect; override;
    procedure SetStringVar(Name: string; Value: string); override;
    function GetStringVar(Name: string): string; override;
    procedure SetIntVar(Name: string; Value: integer); override;
    function GetIntVar(Name: string): integer; override;
    procedure SetFloatVar(Name: string; Value: extended); override;
    function GetFloatVar(Name: string): extended; override;
    function GetDataPtr(VarID: TVarID): pointer;
    procedure PushParam(var Param); override;

    property Scope: TScopeManager read FScope;
    property TypeHelper: TTypeHelper read FTypeHelper;
    property DataSeg: pointer read GetDataBuf;
    property DataSize: longint read FDataSize write SetDataSize;
    property CodeSeg: pointer read FCodeBuf write FCodeBuf;
    property CodeSize: longint read FCodeSize write FCodeSize;
    property StartCO: longint read FStartCO write FStartCO;
    property CreateCO: longint read FCreateCO write FCreateCO;
    property DestroyCO: longint read FDestroyCO write FDestroyCO;
    property StackSize: longint read FStackSize write FStackSize;
    property SourceStream: TMemoryStream read FSourceStream write FSourceStream;
    property SourceRefList: TList read FSourceRefList write FSourceRefList;
    property Compiler: TRaveCompiler read FCompiler write FCompiler;
    property EventList: TStrings read FEventList write FEventList;
    property ProjectItem: TRaveProjectItem read FProjectItem write FProjectItem;
    property ProjectManager: TRaveProjectItem read FProjectManager write FProjectManager;
    property ParentModule: TRCPUModule read GetParentModule write FParentModule;
  end; { TRCPUModule }

  TRaveCompiler = class
  public
    class function SyntaxName: string; virtual;
    constructor Create; virtual;
    procedure CompileStream(Stream: TStream;
                            FileName: string;
                            AModule: TRCPUModule;
                            Definition: boolean); virtual; abstract;
  end; { TRaveCompiler }
  TRaveCompilerClass = class of TRaveCompiler;

  procedure AddRaveCompiler(CompilerClass: TRaveCompilerClass);
  function CreateRaveCompiler(SyntaxName: string): TRaveCompiler;

var
  CompilerList: TList = nil;

implementation

uses
{$IFDEF DOMAP}
  RvLEDebug,
{$ENDIF}
  RvLEUtil, RvLERun, RvProj, RvData;

var
  CRLFStr: string[2] = #13#10;

procedure AddRaveCompiler(CompilerClass: TRaveCompilerClass);
begin { AddRaveCompiler }
  if not Assigned(CompilerList) then begin
    CompilerList := TList.Create;
  end; { if }
  CompilerList.Add(CompilerClass);
end;  { AddRaveCompiler }

function CreateRCPUModule(Stream: TStream;
                          Owner: TRaveProjectItem;
                          ProjectItem: TRaveProjectItem): TRaveModule;
begin { CreateRCPUModule }
  if Assigned(Stream) then begin
    Result := TRCPUModule.CreateFromStream(Stream,Owner,ProjectItem);
  end else begin
    Result := TRCPUModule.Create;
    TRCPUModule(Result).ProjectManager := Owner.Project;
    TRCPUModule(Result).ProjectItem := ProjectItem;
  end; { else }
end;  { CreateRCPUModule }

var
TestInstance: TRaveComponent;

procedure PrepareRCPUModule(ProjectItem: TRaveProjectItem);
var
  I1: integer;
  Instance: TRaveComponent;
  Module: TRCPUModule;
begin { PrepareRCPUModule }
  Module := ProjectItem.Module as TRCPUModule;
  if Assigned(Module) then begin
  // Go through all components owned by ProjectItem and init data segment
    FillChar(Module.DataSeg^,Module.DataSize,0);
    if ProjectItem is TRaveProjectManager then begin
      Module.SetIntVar(ProjectItem.Name,integer(ProjectItem));
    end; { if }
    for I1 := 0 to ProjectItem.ComponentCount - 1 do begin
      Instance := ProjectItem.Components[I1] as TRaveComponent;
      TestInstance := Instance;
      Module.SetIntVar(Instance.Name,integer(Instance));
    end; { for }
  end; { if }
end;  { PrepareRCPUModule }

function CreateRaveCompiler(SyntaxName: string): TRaveCompiler;
var
  I1: integer;
begin { CreateRaveCompiler }
  Result := nil;
  if Assigned(CompilerList) then begin
    for I1 := 0 to CompilerList.Count - 1 do begin
      if AnsiCompareText(TRaveCompilerClass(CompilerList[I1]).SyntaxName,
       SyntaxName) = 0 then begin
        Result := TRaveCompilerClass(CompilerList[I1]).Create;
        Exit;
      end; { if }
    end; { for }
  end; { if }
end;  { CreateRaveCompiler }

procedure RCPUEnumRaveCompilers(List: TStrings);
var
  I1: integer;
begin { RCPUEnumRaveCompilers }
  List.Clear;
  if Assigned(CompilerList) then begin
    for I1 := 0 to CompilerList.Count - 1 do begin
      List.Add(TRaveCompilerClass(CompilerList[I1]).SyntaxName);
    end; { for }
  end; { if }
end;  { RCPUEnumRaveCompilers }

(*
procedure RCPUCompile(ProjectItem: TRaveProjectItem);
var
  Module: TRCPUModule;
  RaveEvent: TRaveEvent;
  I1: integer;
  BaseID: TBaseID;
  Compiler: TRaveCompiler;
begin { RCPUCompile }
  Compiler := CreateRaveCompiler({Trans-}'Delphi');
  with Compiler do try // Hard code as Delphi for now
  { Compile source stream }
    Module := TRCPUModule.Create;
    Module.ProjectItem := ProjectItem;
    Module.BuildSource;
    Module.Compile(false);

  { Point Rave events to function offsets }
    for I1 := 0 to Module.EventList.Count - 1 do begin
      RaveEvent := TRaveEvent(Module.EventList.Objects[I1]);
      BaseID := Module.FindID(Module.EventList[I1]);
      if BaseID is TFuncID then begin
        RaveEvent.Position := TFuncID(BaseID).CodeLabel.Offset;
      end; { if }
      RaveEvent.ModuleOwner := ProjectItem;
    end; { for }

  { Copy compiled stream to ProjectItem.Module }
    if Assigned(ProjectItem.Module) then begin
      ProjectItem.Module.Free;
      ProjectItem.Module := nil;
    end; { if }
    ProjectItem.Module := Module;
  finally
    Free;
  end; { with }
end;  { RCPUCompile }
*)

{ class TScopeManager }

constructor TScopeManager.Create(AParent: TScopeManager;
                                 AOwner: TRCPUModule);
begin { Create }
  inherited Create;
  FOwner := AOwner;
  FParent := AParent;
  Owner.FScope := self;
  ClearData;
  FTypeManager := TTypeManager.Create(Owner,Owner.TypeHelper);
  FIDManager := TIDManager.Create(Owner.TypeHelper);
  FObjectScope := TList.Create;
end;  { Create }

destructor TScopeManager.Destroy;
begin { Destroy }
  FreeAndNil(FObjectScope);
  FreeAndNil(FIDManager);
  FreeAndNil(FTypeManager);
  inherited Destroy;
end;  { Destroy }

procedure TScopeManager.LoadFromStreamHelper(StreamHelper: TStreamHelper);
begin { LoadFromStreamHelper }
  FTypeManager.LoadFromStreamHelper(StreamHelper);
  FIDManager.LoadFromStreamHelper(StreamHelper);
end;  { LoadFromStreamHelper }

procedure TScopeManager.SaveToStreamHelper(StreamHelper: TStreamHelper);
begin { SaveToStreamHelper }
  if Assigned(FParent) then begin
    RaiseError(Trans('Cannot save a nested module state.'));
  end; { if }
  FTypeManager.SaveToStreamHelper(StreamHelper);
  FIDManager.SaveToStreamHelper(StreamHelper);
end;  { SaveToStreamHelper }

function TScopeManager.FindID(ID: TIdentifier): TBaseID;
begin { FindID }
  Result := IDManager.Find(ID);
  if not Assigned(Result) then begin
    if Assigned(Parent) then begin
      Result := Parent.FindID(ID);
    end else begin
      Result := nil;
    end; { else }
  end; { if }
end;  { FindID }

function TScopeManager.AddType(ID: TIdentifier;
                               BaseType: TBaseType): TTypeID;
var
  TypeID: TTypeID;
begin { AddType }
{ Create ID }
  TypeID := TTypeID.Create(ID,BaseType);

{ Add to IDManager }
  Result := IDManager.Add(TypeID) as TTypeID;
  if not Assigned(Result) then begin
    TypeID.Free;
  end; { if }
end;  { AddType }

function TScopeManager.AddVar(ID: TIdentifier;
                              BaseType: TBaseType): TVarID;
var
  VarID: TVarID;
begin { AddVar }
  if Assigned(Parent) then begin { Stack segment variable }
    VarID := TVarID.Create(ID,BaseType,-(DataIdx + BaseType.Size),alBP);
  end else begin { Data segment variable }
    VarID := TVarID.Create(ID,BaseType,DataIdx,alDS);
  end; { else }

{ Add to IDManager }
  Result := IDManager.Add(VarID) as TVarID;
  if Assigned(Result) then begin
    Inc(DataIdx,BaseType.Size);
  end else begin
    VarID.Free;
  end; { if }
end;  { AddVar }

function TScopeManager.AddOp(ID: TIdentifier;
                             OpKind: TOpKind): TOpID;
var
  OpID: TOpID;
begin { AddOp }
  OpID := TOpID.Create(ID,OpKind);

{ Add to IDManager }
  Result := IDManager.Add(OpID) as TOpID;
  if not Assigned(Result) then begin
    OpID.Free;
  end; { if }
end;  { AddOp }

function TScopeManager.AddFunc(ID: TIdentifier;
                               BaseType: TBaseType): TFuncID;
var
  FuncID: TFuncID;
begin { AddFunc }
  FuncID := TFuncID.Create(ID,BaseType);

{ Add to IDManager }
  Result := IDManager.Add(FuncID) as TFuncID;
  if not Assigned(Result) then begin
    FuncID.Free;
  end; { if }
end;  { AddFunc }

function TScopeManager.AddConst(    ID: TIdentifier;
                                    BaseType: TBaseType;
                                var Data): TConstID;
var
  ConstID: TConstID;
begin { AddConst }
{ Create ID }
  if BaseType.TypeKind = tkString then begin
    if string(Data) = '' then begin
      ConstID := TConstID.Create(ID,BaseType,string(Data),Length(string(Data)));
    end else begin
      ConstID := TConstID.Create(ID,BaseType,string(Data)[1],Length(string(Data)));
    end; { else }
  end else if BaseType.TypeKind <> tkComplex then begin
    ConstID := TConstID.Create(ID,BaseType,Data,BaseType.Size);
  end else begin
    RaiseError(Trans('Only simple constant types allowed'));
    ConstID := nil;
  end; { else }

{ Add to IDManager }
  Result := IDManager.Add(ConstID) as TConstID;
  if not Assigned(Result) then begin
    ConstID.Free;
  end; { if }
end;  { AddConst }

function TScopeManager.FindObjectScope(ID: TIdentifier): TVarID;
var
  I1: integer;
  ObjectClass: TClassType;
  Offset: integer;
  Member: TStructureMember;
  Index: integer;
begin
  for I1 := 0 to FObjectScope.Count - 1 do begin
    Result := TVarID(FObjectScope[I1]);
    ObjectClass := Result.BaseType as TClassType;
    if ObjectClass.FindMember(ID,[scPublic],Offset,Member,Index) then begin
      Exit;
    end; { if }
  end; { for }
  Result := nil;
end;

procedure TScopeManager.PopObjectScope;
begin
  FObjectScope.Delete(0);
end;

procedure TScopeManager.PushObjectScope(AObject: TVarID);
begin
  FObjectScope.Insert(0,AObject);
end;

procedure TScopeManager.ClearData;
begin { ClearData }
  DataIdx := 0;
end;  { ClearData }

{ class TModuleTypeHelper }

constructor TModuleTypeHelper.Create(AModule: TRCPUModule);
begin { Create }
  inherited Create;
  FModule := AModule;
end;  { Create }

function TModuleTypeHelper.IndexToType(ModuleIndex: longint;
                                       TypeIndex: longint): TBaseType;
var
  SearchModule: TRCPUModule;
begin { IndexToType }
  if (ModuleIndex = 0) and (TypeIndex = 0) then begin
    Result := nil;
  end else begin
    if ModuleIndex = 0 then begin
      SearchModule := Module;
    end else begin { Find module }
      SearchModule := Module.IndexToModule(ModuleIndex);
    end; { else }
//!!! Fails here because SearchModule is nil
    Result := SearchModule.IndexToType(TypeIndex);
  end; { else }
end;  { IndexToType }

procedure TModuleTypeHelper.TypeToIndex(    BaseType: TBaseType;
                                        var ModuleIndex: longint;
                                        var TypeIndex: longint);
var
  SearchModule: TRCPUModule;
begin { TypeToIndex }
  SearchModule := BaseType.Owner as TRCPUModule;
  ModuleIndex := Module.ModuleToIndex(SearchModule);
  TypeIndex := SearchModule.TypeToIndex(BaseType);
end;  { TypeToIndex }

{ class TRCPUModule }

constructor TRCPUModule.Create;
begin { Create }
  inherited Create;

  FTypeHelper := TModuleTypeHelper.Create(self);
  FMajorVersion := RaveMajorVersion;
  FMinorVersion := RaveMinorVersion;
  FCompileDate := Now;
  FModuleName := {Trans-}'Module1';
  UsedModules := TList.Create;
  Params := TList.Create;
  FScope := TScopeManager.Create(nil,self);
  FDataSize := 0;
  FDataBuf := nil;
  FCodeSize := 0;
  FCodeBuf := nil;
  FStackSize := 16384;
  ExternalList := TStringList.Create;
  ExternalList.Sorted := true;
end;  { Create }

constructor TRCPUModule.CreateFromStream(Stream: TStream;
                                         aOwner: TRaveProjectItem;
                                         aProjectItem: TRaveProjectItem);
var
  StreamHelper: TStreamHelper;
  I1: integer;
  I2: integer;
  ID: string;
  Offset: longint;
begin { CreateFromStream }
  inherited Create;

  FProjectItem := aProjectItem;
  FProjectManager := aOwner.Project;
  FTypeHelper := TModuleTypeHelper.Create(self);
  ExternalList := TStringList.Create;
  ExternalList.Sorted := true;
  StreamHelper := TStreamHelper.Create(Stream);
{ Read in module data from stream }
  with StreamHelper do try
    FModuleName := ReadString;
    ReadByte;
    ReadBuf(FMajorVersion,SizeOf(FMajorVersion));
    ReadBuf(FMinorVersion,SizeOf(FMinorVersion));
    ReadBuf(FCompileDate,SizeOf(FCompileDate));
    UsedModules := TList.Create;
    Params := TList.Create;
    ForwardRefList := TStringList.Create;
    I2 := ReadIndex;
    for I1 := 1 to I2 do begin
      ID := ReadString;
      if (ID <> '') and (ID[1] = '+') then begin
        ForwardRefList.AddObject(Copy(ID,2,Length(ID) - 1),TObject(UsedModules.Count));
        UsedModules.Add(nil);
      end else begin
        AddModule(ID);
      end; { else }
    end; { for }
    FScope := TScopeManager.Create(nil,self);
    FScope.LoadFromStreamHelper(StreamHelper);
    FDataSize := ReadIndex;
    FCodeSize := ReadIndex;
    GetMem(FCodeBuf,FCodeSize);
    ReadBuf(FCodeBuf^,FCodeSize);

    FStackSize := ReadIndex;
    FStartCO := ReadIndex;
    FCreateCO := ReadIndex;
    FDestroyCO := ReadIndex;
    I2 := ReadIndex;
    for I1 := 1 to I2 do begin
      ID := ReadString;
      Offset := ReadIndex;
      AddExternal(ID,Offset);
    end; { for }
  finally
    Free;
  end; { with }
end;  { CreateFromStream }

constructor TRCPUModule.CreateFromFile(FileName: string;
                                       AOwner: TRaveProjectItem);
var
  Stream: TMemoryStream;
begin { CreateFromFile }
  Stream := TMemoryStream.Create;
  try
    Stream.LoadFromFile(FileName + {Trans-}'.rvc');
    CreateFromStream(Stream,AOwner,nil);
  finally
    Stream.Free;
  end; { tryf }
end;  { CreateFromFile }

destructor TRCPUModule.Destroy;
var
  I1: integer;
begin { Destroy }
  if Assigned(ProjectManager) and Assigned(TRaveProjectManager(ProjectManager).ModuleManager) then begin
    TRaveProjectManager(ProjectManager).ModuleManager.ReleaseModule(self);
  end; { if }
  if Assigned(FSourceStream) then begin
    FreeAndNil(FSourceStream);
  end; { if }
  FreeAndNil(FSourceRefList);
  FreeAndNil(FEventList);
  FreeAndNil(UsedModules);
  FreeAndNil(Params);
  FreeAndNil(FScope);
  FreeAndNil(FTypeHelper);
  if Assigned(FDataBuf) then begin
    FreeMem(FDataBuf,FDataSize);
    FDataBuf := nil;
  end; { if }
  if Assigned(FCodeBuf) then begin
    FreeMem(FCodeBuf,FCodeSize);
    FCodeBuf := nil;
  end; { if }
  for I1 := 0 to ExternalList.Count - 1 do begin
    Dispose(PExternalFunc(ExternalList.Objects[I1]));
  end; { for }
  FreeAndNil(ExternalList);
  FreeAndNil(ForwardRefList);
  inherited Destroy;
end;  { Destroy }

procedure TRCPUModule.PostLoad;
var
  I1: integer;
  Module: TRCPUModule;

  function FindModule(aProjectItem: TRaveProjectItem;
                      aModuleName: string): TRCPUModule;
  var
    I1: integer;

  begin { FindModule }
    for I1 := 0 to aProjectItem.ComponentCount - 1 do begin
      if aProjectItem.Components[I1] is TRaveProjectItem then begin
        Result := TRCPUModule(TRaveProjectItem(aProjectItem.Components[I1]).Module);
        if Assigned(Result) and SameStr(Result.ModuleName,aModuleName) then begin
          Exit;
        end; { if }
      end; { if }
    end; { for }

    if aProjectItem.Owner is TRaveProjectItem then begin
      Result := FindModule(TRaveProjectItem(aProjectItem.Owner),aModuleName);
    end else begin
      Result := TRCPUModule(aProjectItem.Module);
      if Assigned(Result) and not SameStr(Result.ModuleName,aModuleName) then begin
        Result := nil;
      end; { if }
    end; { else }
  end;  { FindModule }

begin { PostLoad }
  if not Assigned(ForwardRefList) then Exit;
  for I1 := 0 to ForwardRefList.Count - 1 do begin
    if (ProjectItem is TRaveProjectManager) or (ProjectItem is TRaveReport) then begin
      Module := FindModule(ProjectItem,ForwardRefList[I1]);
    end else begin
      Module := FindModule(TRaveProjectItem(ProjectItem.Owner),ForwardRefList[I1]);
    end; { else }
    UsedModules[integer(ForwardRefList.Objects[I1])] := Module;
  end; { for }
  FreeAndNil(ForwardRefList);
end;  { PostLoad }

procedure TRCPUModule.Changing(OldItem: TRaveModule;
                               NewItem: TRaveModule);
begin { Changing }
//!!! Eldon - remove Olditem from UsedModules if NewItem is nil
  if (Assigned(OldItem) and (OldItem is TRaveModule)) or
   (Assigned(NewItem) and (NewItem is TRaveModule)) then begin
    if UsedModules.IndexOf(OldItem) >= 0 then begin
      UsedModules.Remove(OldItem);
    end; { if }
  end; { if }
end;  { Changing }

function TRCPUModule.GetParentModule: TRCPUModule;
begin { GetParentModule }
  if not Assigned(FParentModule) then begin
    if Assigned(ProjectItem) and not (ProjectItem is TRaveProjectManager) then begin
      FParentModule := TRCPUModule((ProjectItem.Owner as TRaveProjectItem).Module);
    end; { if }
  end; { if }
  Result := FParentModule;
end;  { GetParentModule }

function TRCPUModule.GetDataBuf: pointer;
begin { GetDataBuf }
  if not Assigned(FDataBuf) then begin
    GetMem(FDataBuf,FDataSize);
    FillChar(FDataBuf^,FDataSize,0);
  end; { if }
  Result := FDataBuf;
end;  { GetDataBuf }

procedure TRCPUModule.SetDataSize(Value: longint);
begin { SetDataSize }
  if (FDataSize <> Value) and Assigned(FDataBuf) then begin
    FreeMem(FDataBuf,FDataSize);
  end; { if }
  FDataSize := Value;
  GetMem(FDataBuf,FDataSize);
  FillChar(FDataBuf^,FDataSize,0);
end;  { SetDataSize }

procedure TRCPUModule.SaveToStream(Stream: TStream);
var
  StreamHelper: TStreamHelper;
  I1: integer;
  P1: pointer;
  TempCodeBuf: pointer;
begin { SaveToStream }
  StreamHelper := TStreamHelper.Create(Stream);
  with StreamHelper do try
  { Write out module header }
    WriteString(FModuleName);
    WriteByte(26);
    WriteBuf(FMajorVersion,SizeOf(FMajorVersion));
    WriteBuf(FMinorVersion,SizeOf(FMinorVersion));
    WriteBuf(FCompileDate,SizeOf(FCompileDate));
    WriteIndex(UsedModules.Count);
    for I1 := 0 to UsedModules.Count - 1 do begin
      if Assigned(UsedModules[I1]) then begin
      if Assigned(TRCPUModule(UsedModules[I1]).ProjectItem) then begin
        WriteString('+' + TRCPUModule(UsedModules[I1]).ModuleName);
      end else begin
        WriteString(TRCPUModule(UsedModules[I1]).ModuleName);
      end; { else }
      end else begin
        WriteString('');
      end; { else }
    end; { for }
    Scope.SaveToStreamHelper(StreamHelper);
    WriteIndex(FDataSize);
    WriteIndex(FCodeSize);

  // Before writing out stream, clear out all external references
    GetMem(TempCodeBuf,FCodeSize);
    try
      Move(FCodeBuf^,TempCodeBuf^,FCodeSize);
      for I1 := 0 to ExternalList.Count - 1 do begin
        P1 := pointer(longint(TempCodeBuf) + PExternalFunc(ExternalList.Objects[I1]).Offset);
        pointer(P1^) := nil;
      end; { for }
      WriteBuf(TempCodeBuf^,FCodeSize);
    finally
      FreeMem(TempCodeBuf,FCodeSize);
    end; { tryf }

    WriteIndex(StackSize);
    WriteIndex(StartCO);
    WriteIndex(CreateCO);
    WriteIndex(DestroyCO);
    WriteIndex(ExternalList.Count);
    for I1 := 0 to ExternalList.Count - 1 do begin
      WriteString(ExternalList.Strings[I1]);
      with PExternalFunc(ExternalList.Objects[I1])^ do begin
        WriteIndex(Offset);
      end; { with }
    end; { for }
  finally
    StreamHelper.Free;
  end; { tryf }
end;  { SaveToStream }

procedure TRCPUModule.SaveToFile(FileName: string);
var
  Stream: TMemoryStream;
begin { SaveToFile }
  Stream := TMemoryStream.Create;
  try
    SaveToStream(Stream);
    Stream.SaveToFile(FileName + {Trans-}'.rvc');
  finally
    Stream.Free;
  end; { tryf }
end;  { SaveToFile }

procedure TRCPUModule.Init;
begin { Init }
  if not Assigned(FDataBuf) then begin
    GetMem(FDataBuf,FDataSize);
  end; { if }
  FillChar(FDataBuf^,FDataSize,0);
end;  { Init }

procedure TRCPUModule.BuildSource;
var
  Instance: TComponent;
  PropCount: integer;
  PropList: PPropList;
  PropInfo: PPropInfo;
  I1: integer;
  I2: integer;
  RaveEvent: TRaveEvent;
  HasVars: boolean;
  ModuleList: TStringList;
  LineCount: integer;

  procedure AddLine(Value: string);
  var
    I1: integer;
  begin { AddLine }
    if Value <> '' then begin
      SourceStream.Write(Value[1],Length(Value));
    end; { if }
    SourceStream.Write(CRLFStr[1],2);
    Inc(LineCount);
    for I1 := 1 to Length(Value) do begin
      if Value[I1] = #13 then begin
        Inc(LineCount);
      end; { if }
    end; { for }
  end;  { AddLine }

  procedure AddModules(aProjectItem: TRaveProjectItem;
                       aModuleList: TStringList);
  var
    I1: integer;
  begin { AddModules }
    for I1 := 0 to aProjectItem.ComponentCount - 1 do begin
      if (aProjectItem.Components[I1] is TRaveProjectItem) and
       (aProjectItem.Components[I1] <> self.ProjectItem) then begin
        aModuleList.Add(TRaveProjectItem(aProjectItem.Components[I1]).ModuleName);
      end; { if }
    end; { for }
    if aProjectItem.Owner is TRaveProjectItem then begin
      AddModules(TRaveProjectItem(aProjectItem.Owner),aModuleList);
    end else begin
      aModuleList.Add(aProjectItem.ModuleName); { Add ProjectManager reference }
    end; { if }
  end;  { AddModules }

begin { BuildSource }
{ Create/Clear SourceStream and EventList }
  if Assigned(SourceStream) then begin
    SourceStream.Clear;
  end else begin
    SourceStream := TMemoryStream.Create;
  end; { else }
  if Assigned(EventList) then begin
    EventList.Clear;
  end else begin
    EventList := TStringList.Create;
  end; { else }
  if Assigned(SourceRefList) then begin
    SourceRefList.Clear;
  end else begin
    SourceRefList := TList.Create;
  end; { else }
  LineCount := 0;

{TransOff}
{ Build source stream of all components and events on page }

{ Add module header }
  AddLine('module ' + ProjectItem.ModuleName + ';');
  AddLine('');
  AddLine('definition');

{ Add global page references }
  ModuleList := TStringList.Create;
  try
    ModuleList.Sorted := true;
    ModuleList.Duplicates := dupIgnore;

  { Add base modules }
    ModuleList.Add('SYSFUNC');
    ModuleList.Add('RVCLASS');
    ModuleList.Add('RVDATA');
    ModuleList.Add('RVBASE');

  { Add modules for each component }
    for I1 := 0 to ProjectItem.ComponentCount - 1 do begin
      ModuleName := FindRaveClassModule(ProjectItem.Components[I1].ClassName);
      if ModuleName <> '' then begin
        ModuleList.Add(UpperCase(ModuleName));
      end; { if }
    end; { for }

  { Build module list }
    AddLine('');
    AddLine('import');
    for I1 := 0 to ModuleList.Count - 2 do begin
      AddLine('  ' + ModuleList[I1] + ',');
    end; { for }
    AddLine('  ' + ModuleList[ModuleList.Count - 1] + ';');
  finally
    FreeAndNil(ModuleList);
  end; { tryf }

{ Add vars for all components on page }
  HasVars := false;
  for I1 := 0 to ProjectItem.ComponentCount - 1 do begin
    Instance := ProjectItem.Components[I1];
    if Instance.Name = '' then Continue; // Don't add components with no name
    if not HasVars then begin
      HasVars := true;
      AddLine('');
      AddLine('var');
    end; { if }
    AddLine('  ' + Instance.Name + ': ' + GetSupportedClass(Instance.ClassType) + ';');
  end; { for }
  if ProjectItem is TRaveProjectManager then begin
    if not HasVars then begin
      AddLine('');
      AddLine('var');
    end; { if }
    AddLine('  ' + ProjectItem.Name + ': ' + GetSupportedClass(ProjectItem.ClassType) + ';');
  end; { if }

{ Start implementation }
  AddLine('');
  AddLine('implementation');

{ Add import for parents and cross reference modules }
  ModuleList := TStringList.Create;
  try
    ModuleList.Sorted := true;
    ModuleList.Duplicates := dupIgnore;
    ModuleName := '';

    if (ProjectItem is TRaveProjectManager) or (ProjectItem is TRaveReport) then begin
      AddModules(ProjectItem,ModuleList);
    end else begin
      AddModules(TRaveProjectItem(ProjectItem.Owner),ModuleList);
    end; { else }

    if ModuleList.Count > 0 then begin
      AddLine('');
      AddLine('import');
      for I1 := 0 to ModuleList.Count - 2 do begin
        AddLine('  ' + ModuleList[I1] + ',');
      end; { for }
      AddLine('  ' + ModuleList[ModuleList.Count - 1] + ';');
    end; { if }
  finally
    FreeAndNil(ModuleList);
  end; { tryf }

{ Include source for all events }
  for I1 := 0 to ProjectItem.ComponentCount - 1 do begin
    Instance := ProjectItem.Components[I1];
    if Instance.Name = '' then Continue; // Don't process components with no name

    PropCount := GetPropList(Instance.ClassInfo,tkProperties,nil);
    GetMem(PropList,PropCount * SizeOf(pointer));
    try
      GetPropList(Instance.ClassInfo,tkProperties,PropList);
      for I2 := 0 to PropCount - 1 do begin
        PropInfo := PropList^[I2];

      { Don't show non-event properties }
        if (PropInfo.PropType^.Kind <> tkClass) or not GetTypeData(
         PropInfo.PropType^).ClassType.InheritsFrom(TRaveEvent) then Continue;

        RaveEvent := TRaveEvent(GetOrdProp(Instance,PropInfo));
        if Assigned(RaveEvent) then begin
          RaveEvent.Name := PropInfo.Name;
          SourceRefList.Add(RaveEvent);
          AddLine('');
          EventList.AddObject(Instance.Name + '_' + PropInfo.Name,RaveEvent);
          AddLine(RaveEvent.Declaration);
          RaveEvent.HeaderSourceLine := LineCount + 1;
          AddLine(RaveEvent.Header);
          AddLine('begin');
          RaveEvent.BodySourceLine := LineCount + 1;
          AddLine(RaveEvent.Body);
          AddLine(RaveEvent.Footer);
        end; { if }
      end; { for }
    finally
      FreeMem(PropList,PropCount * SizeOf(pointer));
    end; { tryf }
  end; { for }

{ Finish module }
  AddLine('');
  AddLine('end ' + ProjectItem.ModuleName + ';');
  {$IFDEF DOMAP}SourceStream.SaveToFile(ProjectItem.ModuleName + '.rvs');{$ENDIF}
  SourceStream.Position := 0;
{TransOn}
end;  { BuildSource }

procedure TRCPUModule.Compile(Definition: boolean);
begin { Compile }
  if Definition then begin
    if Assigned(FCompiler) then begin
      FreeAndNil(FCompiler);
    end; { if }
    FCompiler := CreateRaveCompiler({Trans-}'Delphi');
  end; { if }
  Compiler.CompileStream(SourceStream,ProjectItem.ModuleName,self,Definition);
  if not Definition then begin
    FreeAndNil(FCompiler);
  end; { if }
  DataSize := Scope.DataIdx;
end;  { Compile }

procedure TRCPUModule.EventConnect;
var
  I1: integer;
  RaveEvent: TRaveEvent;
  BaseID: TBaseID;
begin { EventConnect }
{ Point Rave events to function offsets }
  for I1 := 0 to EventList.Count - 1 do begin
    RaveEvent := TRaveEvent(EventList.Objects[I1]);
    BaseID := FindID(EventList[I1]);
    if BaseID is TFuncID then begin
      RaveEvent.Position := TFuncID(BaseID).CodeLabel.Offset;
    end; { if }
    RaveEvent.ModuleOwner := ProjectItem;
  end; { for }
end;  { EventConnect }

procedure TRCPUModule.SetStringVar(Name: string;
                                   Value: string);
var
  P1: pointer;
begin { SetStringVar }
  P1 := GetDataPtr(FindID(Name) as TVarID);
  string(longint(P1^)) := Value;
end;  { SetStringVar }

function TRCPUModule.GetStringVar(Name: string): string;
begin { GetStringVar }
  Result := string(longint(GetDataPtr(FindID(Name) as TVarID)^));
end;  { GetStringVar }

procedure TRCPUModule.SetIntVar(Name: string;
                                Value: integer);
var
  VarID: TVarID;
begin { SetIntVar }
  VarID := FindID(Name) as TVarID;
  if Assigned(VarID) then begin
    integer(GetDataPtr(VarID)^) := Value;
  end; { if }
end;  { SetIntVar }

function TRCPUModule.GetIntVar(Name: string): integer;
var
  VarID: TVarID;
begin { GetIntVar }
  VarID := FindID(Name) as TVarID;
  if Assigned(VarID) then begin
    Result := integer(GetDataPtr(VarID)^);
  end else begin
    Result := 0;
  end; { if }
end;  { GetIntVar }

procedure TRCPUModule.SetFloatVar(Name: string;
                                  Value: extended);
begin { SetFloatVar }
  extended(GetDataPtr(FindID(Name) as TVarID)^) := Value;
end;  { SetFloatVar }

function TRCPUModule.GetFloatVar(Name: string): extended;
begin { GetFloatVar }
  Result := extended(GetDataPtr(FindID(Name) as TVarID)^);
end;  { GetFloatVar }

function TRCPUModule.GetDataPtr(VarID: TVarID): pointer;
begin { GetDataPtr }
  Result := pointer(longint(DataSeg) + VarID.Address.Offset);
end;  { GetDataPtr }

procedure TRCPUModule.PushParam(var Param);
begin { PushParam }
  Params.Add(@Param);
end;  { PushParam }

function TRCPUModule.AddType(ID: TIdentifier;
                             BaseType: TBaseType): TTypeID;
begin { AddType }
  Result := Scope.AddType(ID,BaseType);
end;  { AddType }

function TRCPUModule.CreateType(BaseType: TBaseType): TBaseType;
begin { CreateType }
  Result := Scope.TypeManager.Add(BaseType);
end;  { CreateType }

function TRCPUModule.AddVar(ID: TIdentifier;
                            BaseType: TBaseType): TVarID;
begin { AddVar }
  Result := Scope.AddVar(ID,BaseType);
end;  { AddVar }

function TRCPUModule.AddOp(ID: TIdentifier;
                           OpKind: TOpKind): TOpID;
begin { AddOp }
  Result := Scope.AddOp(ID,OpKind);
end;  { AddOp }

function TRCPUModule.AddFunc(ID: TIdentifier;
                             BaseType: TBaseType): TFuncID;
begin { AddFunc }
  Result := Scope.AddFunc(ID,BaseType);
end;  { AddFunc }

function TRCPUModule.AddConst(    ID: TIdentifier;
                                  BaseType: TBaseType;
                              var Data): TConstID;
begin { AddConst }
  Result := Scope.AddConst(ID,BaseType,Data);
end;  { AddConst }

function TRCPUModule.AddModule(ModuleName: TIdentifier): TRCPUModule;

  function SeekModule(aProjectItem: TRaveProjectItem): TRCPUModule;
  var
    I1: integer;
  begin { SeekModule }
    for I1 := 0 to aProjectItem.ComponentCount - 1 do begin
      if aProjectItem.Components[I1] is TRaveProjectItem then begin
        Result := TRCPUModule(TRaveProjectItem(aProjectItem.Components[I1]).Module);
        if Assigned(Result) and SameStr(Result.ModuleName,ModuleName) then begin
          Exit;
        end; { if }
      end; { if }
    end; { for }
    if aProjectItem.Owner is TRaveProjectItem then begin
      Result := SeekModule(TRaveProjectItem(aProjectItem.Owner));
    end else begin
      Result := TRCPUModule(aProjectItem.Module);
      if Assigned(Result) and not SameStr(Result.ModuleName,ModuleName) then begin
        Result := nil;
      end; { if }
    end; { else }
  end;  { SeekModule }

begin { AddModule }
{ Look for module locally }
  if Assigned(ProjectItem) then begin
    if (ProjectItem is TRaveProjectManager) or (ProjectItem is TRaveReport) then begin
      Result := SeekModule(ProjectItem);
    end else begin
      Result := SeekModule(TRaveProjectItem(ProjectItem.Owner));
    end; { else }
    if Assigned(Result) then begin
      UsedModules.Add(Result);
    end; { if }
  end else begin
    Result := nil;
  end; { else }

  if not Assigned(Result) and (ModuleName <> '') then begin { Look for module in module manager }
    Result := TRaveProjectManager(ProjectManager).ModuleManager.GetModule(
     ModuleName) as TRCPUModule;
    if Assigned(Result) then begin
      UsedModules.Add(Result);
    {$IFDEF DOMAP}
      if Assigned(MapFile) then begin
        Map({Trans-}'Import ' + ModuleName + '(' + IntToStr(UsedModules.Count) + ')');
        MapLine;
      end; { if }
    {$ENDIF}
    end; { if }
  end; { if }
end;  { AddModule }

procedure TRCPUModule.AddExternal(AID: TIdentifier;
                                  AOffset: longint);
var
  ExternalFunc: PExternalFunc;
begin { AddExternal }
  New(ExternalFunc);
  with ExternalFunc^ do begin
    Offset := AOffset;
  end; { with }
  ExternalList.AddObject(UpperCase(AID),TObject(ExternalFunc));;
end;  { AddExternal }

function TRCPUModule.FindInternalID(ID: TIdentifier): TBaseID;
begin { FindInternalID }
  Result := Scope.FindID(ID);
end;  { FindInternalID }

function TRCPUModule.FindID(ID: TIdentifier): TBaseID;
var
  BaseID: TBaseID;
  SearchList: TStringList;
  FoundModule: TRCPUModule;

  procedure SearchModule(Module: TRCPUModule;
                         Depth: integer);
  var
    I1: integer;
    Index: integer;
    LModule: TRCPUModule;
  begin { SearchModule }
    BaseID := Module.FindInternalID(ID);
    if Assigned(BaseID) then begin
      FoundModule := Module;
    end else if Assigned(SearchList) or (Depth = 0) then begin
      for I1 := 0 to (Module.UsedModules.Count - 1) do begin
        LModule := TRCPUModule(Module.UsedModules[I1]);
        if Assigned(LModule) then begin
          if Assigned(SearchList) then begin
            if SearchList.Find(LModule.ModuleName,Index) then begin
              Continue; // Already searched this module
            end else begin
              SearchList.Add(LModule.ModuleName);
            end; { else }
          end; { if }
          SearchModule(LModule,Depth + 1);
        end; { if }
        if Assigned(BaseID) then Break;
      end; { for }
    end; { else }
  end;  { SearchModule }

begin { FindID }
  FoundModule := nil;
  BaseID := nil;
  if Pos('.',ID) > 0 then begin // Only perform deep searches for Class.Method code IDs
    SearchList := TStringList.Create;
  end else begin
    SearchList := nil;
  end; { else }
  try
    if Assigned(SearchList) then begin
      SearchList.Sorted := true;
      SearchList.Duplicates := dupIgnore;
      SearchList.Add(ModuleName);
    end; { if }
    SearchModule(self,0);

    Result := BaseID;
    if Assigned(Result) then begin
      if FoundModule = self then begin
        Result.Module := 0;
      end else begin
        Result.Module := UsedModules.IndexOf(FoundModule) + 1;
        if Result.Module < 1 then begin // Module not being used, add it
          UsedModules.Add(FoundModule);
          Result.Module := UsedModules.Count;
        end; { if }
      end; { else }
    end; { if }
  finally
    FreeAndNil(SearchList);
  end; { tryf }
end;  { FindID }

function TRCPUModule.FindType(ID: TIdentifier): TBaseType;
var
  BaseID: TBaseID;
begin { FindType }
  Result := IntegerType;
  BaseID := FindID(ID);
  if Assigned(BaseID) then begin
    if BaseID is TTypeID then begin
      Result := BaseID.BaseType;
    end else begin
      AddError(23,'');
    end; { else }
  end else begin
    AddError(22,'');
  end; { else }
end;  { FindType }

function TRCPUModule.FindExternal(ID: TIdentifier): PExternalFunc;
var
  I1: integer;
begin { FindExternal }
  if ExternalList.Find(ID,I1) then begin
    Result := PExternalFunc(ExternalList.Objects[I1]);
  end else begin
    Result := nil;
  end; { else }
end;  { FindExternal }

function TRCPUModule.IndexToModule(Index: longint): TRCPUModule;
begin { IndexToModule }
  if Index = 0 then begin
    Result := self;
  end else begin
    Result := TRCPUModule(UsedModules[Index - 1]);
  end; { else }
end;  { IndexToModule }

function TRCPUModule.ModuleToIndex(Module: TRCPUModule): longint;
begin { ModuleToIndex }
  if Module = self then begin
    Result := 0;
  end else begin
    Result := UsedModules.IndexOf(Module) + 1;
  end; { else }
end;  { ModuleToIndex }

function TRCPUModule.IndexToType(Index: longint): TBaseType;
begin { IndexToType }
  Result := Scope.TypeManager.Get(Index - 1);
end;  { IndexToType }

function TRCPUModule.TypeToIndex(BaseType: TBaseType): longint;
begin { TypeToIndex }
  Result := BaseType.Index;
end;  { TypeToIndex }

procedure TRCPUModule.CreateScopeLevel;
begin { CreateScopeLevel }
  FScope := TScopeManager.Create(FScope,self);
end;  { CreateScopeLevel }

procedure TRCPUModule.FreeScopeLevel;
var
  NewScope: TScopeManager;
begin { FreeScopeLevel }
  NewScope := Scope.Parent;
  if Assigned(NewScope) then begin
    FreeAndNil(FScope);
    FScope := NewScope;
  end; { if }
end;  { FreeScopeLevel }

procedure TRCPUModule.Execute(Position: integer; var SuspendState: TObject);
begin { Execute }
  StartCO := Position;
  with TRCPURunner.Create do try
    Execute(self,Params,SuspendState);
    Params.Clear;
  finally
    Free;
  end; { with }
end;  { Execute }

{ class TRaveCompiler }

constructor TRaveCompiler.Create;
begin { Create }
  inherited Create;
end;  { Create }

class function TRaveCompiler.SyntaxName: string;
begin
  Result := '';
end;

initialization
  EnumRaveCompilersProc := RCPUEnumRaveCompilers;
//  RaveCompileProc := RCPUCompile;
  CreateRaveModuleProc := CreateRCPUModule;
  PrepareRaveModuleProc := PrepareRCPUModule;
finalization
  FreeAndNil(CompilerList);
end.
